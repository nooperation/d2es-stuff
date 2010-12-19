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

namespace IdleClient.Frontend
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();

			driver.Initalize();
			driver.OnOutput = Output;
			StartBots();
		}

		StringBuilder outputBuffer = new StringBuilder();
		Driver driver = new Driver();

		public void StartBots()
		{
			driver.Start();
		}

		private void Shutdown()
		{
			driver.Terminate();
			UpdateBotCountText();
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			driver.Terminate();
		}

		private void textInput_KeyUp(object sender, KeyEventArgs e)
		{
			if (e.Key == Key.Enter)
			{
				driver.Say(textInput.Text);
				textInput.Text = "";
			}
		}

		private object outputLock = new object();

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

		private void UpdateBotCountText()
		{
			// TODO: Find a real way to communicate with UI from other threads, this seems pretty bad
			textBotCount.Dispatcher.BeginInvoke(
				new Action(() =>
				{
					listBoxNames.Items.Clear();

					if (driver.clients.Count > 0)
					{
						foreach (var item in driver.clients[0].PlayerNames)
						{
							listBoxNames.Items.Add(item);
						}
					}
					textBotCount.Text = driver.clients.Count + " bot(s) online";
				}),
				System.Windows.Threading.DispatcherPriority.Normal
			);
		}
	}
}
