using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IdleClient
{
	class Driver
	{
		/// <summary> Number of milliseconds to wait before attempting to connect </summary>
		public int JoinDelay { get; set; }

		/// <summary> Determins if IdleClient is running. If not running then it's complete. </summary>
		public bool IsRunning
		{
			get { return clients.Count != 0; }
		}

		/// <summary> List of currently running clients </summary>
		public List<ClientDriver> clients = new List<ClientDriver>();

		/// <summary> List of clients available to join the game </summary>
		public Queue<ClientDriver> availableClients = new Queue<ClientDriver>();

		/// <summary> IdleClient settings </summary>
		private Config settings;

		/// <summary> IdleClient is shutting down, no more bots should join </summary>
		private bool isShuttindDown = false;

		/// <summary> Lock for output, haven't tested it yet. </summary>
		private object outputLock = new object();

		/// <summary> Captures output from IdleClient </summary>
		public Action<string> OnOutput;

		public Driver()
		{

		}

		public void Initalize()
		{
			settings = new Config("IdleClient.ini");

			if (settings.BotNames.Count == 0)
			{
				Output("no bots defined");
				return;
			}

			JoinDelay = 3000;

			// Initalize all bots
			for (int i = 0; i < settings.BotNames.Count; i++)
			{
				availableClients.Enqueue(new ClientDriver(settings, i));
			}

			Logger.Instance.OnDebugMessage += new EventHandler<Logger.LoggerArgs>(OnLoggerMessage);
			Logger.Instance.OnDriverMessage += new EventHandler<Logger.LoggerArgs>(OnLoggerMessage);
			Logger.Instance.OnErrorMessage += new EventHandler<Logger.LoggerArgs>(OnLoggerMessage);
			Logger.Instance.OnGameMessage += new EventHandler<Logger.LoggerArgs>(OnLoggerMessage);
			Logger.Instance.OnServerMessage += new EventHandler<Logger.LoggerArgs>(OnLoggerMessage);
		}

		/// <summary>
		/// Starts driver
		/// </summary>
		public void Start()
		{
			PushBot();
		}

		/// <summary>
		/// Terminate all bots
		/// </summary>
		public void Terminate()
		{
			isShuttindDown = true;

			while (clients.Count > 0)
			{
				PopBot();
			}
		}

		/// <summary>
		/// Starts and new bot
		/// </summary>
		private void PushBot()
		{
			ClientDriver newClient;

			lock (availableClients)
			{
				if (availableClients.Count == 0)
				{
					Output("PushBot(): No bots available!");
					return;
				}

				newClient = availableClients.Dequeue();
			}

			lock (clients)
			{
				Output("Waiting...");
				for (int i = 0; i < JoinDelay / 100; i++)
				{
					System.Threading.Thread.Sleep(100);

					if (isShuttindDown)
					{
						Output("Canceling PushBot()");
						return;
					}
				}

				newClient.OnClientDisconnect += new EventHandler(newClient_OnClientDisconnect);
				newClient.OnEnterGame += new EventHandler(newClient_OnEnterGame);
				newClient.OnFailure += new EventHandler<FailureArgs>(newClient_OnFailure);
				newClient.OnShutdown += new EventHandler(newClient_OnShutdown);

				if (clients.Count == 0)
				{
					// Only one client should handle player count change events since this is where we handle adding/removing bots
					newClient.OnPlayerCountChanged += new EventHandler<PlayerCountArgs>(newClient_OnPlayerCountChanged);

					// We only want to read game chat from one client
					Logger.Instance.GameMessageFilter.Add(newClient.CharacterName);

					// TODO: Why only one client for driver messages...?
					Logger.Instance.DriverMessageFilter.Add(newClient.CharacterName);
				}

				clients.Add(newClient);
				newClient.Start();
			}
		}

		/// <summary>
		/// Removes last bot
		/// </summary>
		private void PopBot()
		{
			lock (clients)
			{
				// This is checked again after removing client
				if (clients.Count == 0)
				{
					return;
				}

				ClientDriver clientToRemove = clients[clients.Count - 1];
				clientToRemove.Terminate();
				clients.Remove(clientToRemove);

				AddBotAsAvailable(clientToRemove);
			}
		}

		/// <summary>
		/// Make the first client say a message in game
		/// </summary>
		/// <param name="message">Chat message</param>
		public void Say(string message)
		{
			if (clients.Count > 0 && clients[0] != null)
			{
				clients[0].Say(message);
			}
		}

		/// <summary>
		/// Adds client to list of available clients if it's not already present
		/// </summary>
		/// <param name="client">Bot to add</param>
		private void AddBotAsAvailable(ClientDriver client)
		{
			AddBotAsAvailable(client.ClientIndex);
		}

		/// <summary>
		/// Adds client to list of available clients if it's not already present
		/// </summary>
		/// <param name="client">Bot index in config to add</param>
		private void AddBotAsAvailable(int clientIndex)
		{
			lock (availableClients)
			{
				// Add removed client to pool of available clients if not already added
				if (!availableClients.Any(n => n.ClientIndex == clientIndex))
				{
					availableClients.Enqueue(new ClientDriver(settings, clientIndex));
				}
			}
		}

		/// <summary>
		/// Fired when message sent to logger
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Logger args</param>
		void OnLoggerMessage(object sender, Logger.LoggerArgs e)
		{
			Output("[" + e.Source + "] " + e.Message);
		}

		/// <summary>
		/// Fired when player count has changed
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">New player count and information about player who joined/left game</param>
		void newClient_OnPlayerCountChanged(object sender, PlayerCountArgs e)
		{
			if (isShuttindDown)
			{
				return;
			}

			Output(String.Format("Player count changed: {0} / {1}", e.PlayerCount, e.MaxPlayers));

			if (e.IsBot)
			{
				Output("It's a bot, ignoring");
				return;
			}

			if (clients.Count == 0)
			{
				Output("!!! PlayerCountChanged event raised without any clients");
				return;
			}

			if (e.PlayerCount < e.MaxPlayers - 1)
			{
				PushBot();
			}
			else if (e.PlayerCount == e.MaxPlayers && clients.Count > 0)
			{
				PopBot();
			}
		}

		/// <summary>
		/// Fired when client successfully enteres game
		/// </summary>
		/// <param name="sender">DriverClient that caused this event</param>
		/// <param name="e">Unused</param>
		void newClient_OnEnterGame(object sender, EventArgs e)
		{
			if (isShuttindDown)
			{
				PopBot();
				return;
			}

			ClientDriver source = sender as ClientDriver;

			Output("Client entered game");

			// PlayerNames doesn't include our bot's name yet
			if (source.PlayerNames.Count + 1 < source.MaxPlayers - 1)
			{
				Output("Room for another bot, adding...");
				PushBot();
			}
			else if (source.PlayerNames.Count + 1 == source.MaxPlayers && clients.Count > 0)
			{
				Output("oops, this bot wasn't needed, removing...");
				PopBot();
			}
		}

		/// <summary>
		/// Fired when client fails to join game
		/// </summary>
		/// <param name="sender">DriverClient that caused this event</param>
		/// <param name="e">Failure information</param>
		void newClient_OnFailure(object sender, FailureArgs e)
		{
			ClientDriver source = sender as ClientDriver;

			Output("Client failed");

			lock (clients)
			{
				clients.Remove(source);
			}

			AddBotAsAvailable(source);
		}

		/// <summary>
		/// Fired when client disconnects from game
		/// </summary>
		/// <param name="sender">DriverClient that caused this event</param>
		/// <param name="e">Unused</param>
		void newClient_OnClientDisconnect(object sender, EventArgs e)
		{
			ClientDriver source = sender as ClientDriver;

			Output("Client disconnected");

			lock (clients)
			{
				clients.Remove(source);
			}

			AddBotAsAvailable(source);
		}

		/// <summary>
		/// Fired when client is ordered to shut down. We should shutdown entire IdleClient
		/// </summary>
		/// <param name="sender">DriverClient that caused this event</param>
		/// <param name="e">Unused</param>
		void newClient_OnShutdown(object sender, EventArgs e)
		{
			isShuttindDown = true;
		}

		/// <summary>
		/// Output message to interface
		/// </summary>
		/// <param name="message">Output message</param>
		void Output(string message)
		{
			// TODO: Test
			lock (outputLock)
			{
				if (OnOutput != null)
				{
					OnOutput.BeginInvoke(message, null, null);
				}
				else
				{
					Console.WriteLine(message);
				}
			}
		}
	}
}
