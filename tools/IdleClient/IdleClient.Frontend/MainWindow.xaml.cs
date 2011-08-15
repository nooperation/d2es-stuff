using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using System.Collections.ObjectModel;

namespace IdleClient.Frontend
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		private readonly string settingsPath = "IdleClient.xml";
		private object outputLock = new object();
		StringBuilder outputBuffer = new StringBuilder();
		Driver driver = new Driver();
		Config settings = new Config();

		public MainWindow()
		{
			try
			{
				settings = Config.ReadConfig(settingsPath);
			}
			catch (Exception)
			{
				MessageBox.Show("Failed to read settings file " + settingsPath + ", delete file to regenerate new config");
				Application.Current.Shutdown();
			}

			InitializeComponent();

			ReloadGUIUserSettings();
		}

		/// <summary>
		/// Fills in settings on GUI with values from settings file
		/// </summary>
		private void ReloadGUIUserSettings()
		{
			ReloadBotList();

			comboBoxDifficulty.ItemsSource = Enum.GetValues(typeof(DifficultyType));
			textBoxGameName.Text = settings.GameName;
			textBoxGamePass.Text = settings.GamePass;
			comboBoxDifficulty.SelectedItem = settings.GameDifficulty;

			textBoxAccountName.Text = settings.Username;
			textBoxAccountPassword.Password = settings.Password;
			textBoxServer.Text = settings.Address;
			textBoxPort.Text = settings.Port.ToString();
			textBoxMaster.Text = settings.MasterName;
			textBoxOwner.Text = settings.Owner;
			textBoxJoinDelay.Text = settings.JoinDelay.ToString();
			checkBoxShowPackets.IsChecked = settings.ShowPackets;
			checkBoxShowPacketData.IsChecked = settings.ShowPacketData;
			checkBoxSinglePlayerMode.IsChecked = settings.IsSinglePlayerMode;
			sliderMaxBots.Value = settings.MaxBots;
		}

		/// <summary>
		/// Reads settings from GUI and stores them in settings class
		/// </summary>
		private void ReaderUserSettingsFromGUI()
		{
			settings.GameName = textBoxGameName.Text.Trim();
			settings.GamePass = textBoxGamePass.Text.Trim(); 
			settings.GameDifficulty = (DifficultyType)comboBoxDifficulty.SelectedItem;

			settings.Username = textBoxAccountName.Text.Trim();
			settings.Password = textBoxAccountPassword.Password;
			settings.Address = textBoxServer.Text.Trim();
			settings.Port = int.Parse(textBoxPort.Text);
			settings.MasterName = textBoxMaster.Text.Trim();
			settings.Owner = textBoxOwner.Text.Trim();
			settings.JoinDelay = int.Parse(textBoxJoinDelay.Text);
			settings.ShowPackets = checkBoxShowPackets.IsChecked == true;
			settings.ShowPacketData = checkBoxShowPacketData.IsChecked == true;
			settings.IsSinglePlayerMode = checkBoxSinglePlayerMode.IsChecked == true;
			settings.MaxBots = (int)sliderMaxBots.Value;
		}

		/// <summary>
		/// Starts the IdleClient
		/// </summary>
		private void StartIdleClient()
		{
			if (driver.IsInitalized)
			{
				driver.Terminate();
			}

			gridConnect.Visibility = System.Windows.Visibility.Collapsed;
			gridDisconnect.Visibility = System.Windows.Visibility.Visible;

			ReaderUserSettingsFromGUI();

			tabControlMain.SelectedIndex = 0;

			driver = new Driver();

			driver.OnOutput = Output;
			driver.OnPlayerCountChange = PlayerCountChanged;
			driver.OnCompletion = DriverCompleted;

			driver.Initalize(settings);
			if (!driver.IsInitalized)
			{
				return;
			}

			DifficultyType difficulty = (DifficultyType)comboBoxDifficulty.SelectedItem;
			string gameName = textBoxGameName.Text.Trim();
			string gamePass = textBoxGamePass.Text.Trim();

			Thread idleClientStarterThread = new Thread(() =>
			{
				driver.Start(gameName, gamePass, difficulty);
			});

			idleClientStarterThread.IsBackground = true;
			idleClientStarterThread.Name = "idleClientStarter";
			idleClientStarterThread.Start();
		}

		/// <summary>
		/// Shutsdown IdleClient and saves config
		/// </summary>
		private void Shutdown()
		{
			ReaderUserSettingsFromGUI();
			settings.WriteConfig(settingsPath);
			driver.Terminate();

			// Reset
			PlayerCountChanged(new List<string>());
		}

		/// <summary>
		/// Adds a bot to settings
		/// </summary>
		/// <param name="name"></param>
		void AddBot(string name)
		{
			if (String.IsNullOrWhiteSpace(name))
			{
				return;
			}

			settings.BotNames.Add(name.Trim());
			ReloadBotList();
		}

		/// <summary>
		/// Removes a bot from settings
		/// </summary>
		/// <param name="name">Bot to remove</param>
		void DeleteBot(string name)
		{
			settings.BotNames.Remove(name);
			ReloadBotList();
		}

		/// <summary>
		/// Reloads list of bots
		/// </summary>
		void ReloadBotList()
		{
			listBoxBots.Items.Clear();
			foreach (var item in settings.BotNames)
			{
				listBoxBots.Items.Add(item);
			}
		}

		private void DriverCompleted()
		{
			this.Dispatcher.BeginInvoke(
				new Action(() =>
				{
					gridConnect.Visibility = System.Windows.Visibility.Visible;
					gridDisconnect.Visibility = System.Windows.Visibility.Collapsed;
					listBoxNames.Items.Clear();

					// Settings might of been changed by the driver
					ReloadGUIUserSettings();
				}),
				System.Windows.Threading.DispatcherPriority.Normal
			);

			Output("Driver completed");
		}

		/// <summary>
		/// Handles output messages from IdleClient
		/// </summary>
		/// <param name="message">Message</param>
		private void Output(string message)
		{
			lock (outputLock)
			{
				outputBuffer.AppendLine(message);
			}

			// FIXME: Out of bounds index when setting textServerOutput.Text
			// TODO: Find a real way to communicate with UI from other threads, this seems pretty bad
			textServerOutput.Dispatcher.BeginInvoke(
				new Action(() =>
				{
					lock (outputLock)
					{
						textServerOutput.Text = outputBuffer.ToString();
						textServerOutput.ScrollToEnd();
					}
				}),
				System.Windows.Threading.DispatcherPriority.Normal
			);
		}

		/// <summary>
		/// Handles PlayerCountChanged events from IdleClient, used to display list of players in game
		/// </summary>
		/// <param name="playerNames"></param>
		private void PlayerCountChanged(List<string> playerNames)
		{
			// TODO: Find a real way to communicate with UI from other threads, this seems pretty bad
			textBotCount.Dispatcher.BeginInvoke(
				new Action(() =>
				{
					listBoxNames.Items.Clear();
					foreach (var item in playerNames)
					{
						listBoxNames.Items.Add(item);
					}
					textBotCount.Text = driver.clients.Count + " player(s) online";
				}),
				System.Windows.Threading.DispatcherPriority.Normal
			);
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			Shutdown();
		}

		private void textInput_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.Key == Key.Enter)
			{
				driver.Say(textInput.Text);
				textInput.Text = "";
			}
		}

		private void buttonConnect_Click(object sender, RoutedEventArgs e)
		{
			StartIdleClient();
		}

		private void buttonAddBot_Click(object sender, RoutedEventArgs e)
		{
			AddBot(textBoxAddBot.Text);
			textBoxAddBot.Text = "";
		}

		private void buttonRemoveBot_Click(object sender, RoutedEventArgs e)
		{
			List<string> toRemove = new List<string>();

			foreach (var item in listBoxBots.SelectedItems)
			{
				toRemove.Add(item.ToString());
			}

			foreach (var item in toRemove)
			{
				DeleteBot(item.ToString());
			}
		}

		private void textBoxAddBot_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.Key == Key.Enter)
			{
				AddBot(textBoxAddBot.Text);
				textBoxAddBot.Text = "";
			}
		}

		private void buttonDisconnect_Click(object sender, RoutedEventArgs e)
		{
			driver.Terminate();
		}
	}
}
