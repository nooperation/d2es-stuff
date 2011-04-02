using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace IdleClient
{
	class Driver
	{
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
		public Config settings;

		/// <summary> IdleClient is shutting down, no more bots should join </summary>
		private bool isShuttindDown = false;

		/// <summary> Lock for output, haven't tested it yet. </summary>
		private object outputLock = new object();

		/// <summary> Captures output from IdleClient </summary>
		public Action<string> OnOutput;

		/// <summary> Captures OnPlayerCountChange events </summary>
		public Action<List<string>> OnPlayerCountChange;

		/// <summary> Driver has completed all operations, no clients are running </summary>
		public Action OnCompletion;

		/// <summary> Determins if driver has been initalized </summary>
		public bool IsInitalized { get; protected set; }

		/// <summary> Map of each client and number of times it's failed </summary>
		private Dictionary<int, int> FailedClientCounts = new Dictionary<int, int>();

		/// <summary>
		/// Additional time to delay before starting next client. Mainly used when server
		/// kills our connection because we connected too fast
		/// </summary>
		private int TemporaryJoinDelay = 0;

		/// <summary>
		/// Initalize driver.
		/// </summary>
		/// <param name="settings">Settings to use</param>
		public void Initalize(Config settings)
		{
			FailedClientCounts.Clear();
			this.settings = settings;

			if (settings.BotNames.Count == 0)
			{
				Output("no bots defined");
				return;
			}

			// Initalize all bots
			for (int i = 0; i < settings.BotNames.Count; i++)
			{
				availableClients.Enqueue(new ClientDriver(settings, i));
				FailedClientCounts.Add(i, 0);
			}

			Logger.Instance.OnDebugMessage = OnLoggerMessage;
			Logger.Instance.OnDriverMessage = OnLoggerMessage;
			Logger.Instance.OnErrorMessage = OnLoggerMessage;
			Logger.Instance.OnGameMessage = OnLoggerMessage;
			Logger.Instance.OnServerMessage = OnLoggerMessage;

			IsInitalized = true;
		}

		/// <summary>
		/// Starts driver
		/// </summary>
		public void Start()
		{
			if (!IsInitalized)
			{
				throw new Exception("Driver not initalized!");
			}

			PushBot();
		}

		/// <summary>
		/// Starts driver
		/// </summary>
		/// <param name="game">Game name to join/create</param>
		/// <param name="pass">Game pass to join/create</param>
		/// <param name="difficulty">Game difficulty</param>
		public void Start(string game, string pass, DifficultyType difficulty)
		{
			if (!IsInitalized)
			{
				throw new Exception("Driver not initalized!");
			}

			settings.GameName = game;
			settings.GamePass = pass;
			settings.GameDifficulty = difficulty;
			settings.GameDescription = difficulty.ToString();

			PushBot();
		}

		/// <summary>
		/// Terminate all bots
		/// </summary>
		public void Terminate()
		{
			if (!IsInitalized)
			{
				return;
			}
			Output("Terminating");

			isShuttindDown = true;

			while (clients.Count > 0)
			{
				PopBot();
			}

			IsInitalized = false;
		}

		/// <summary>
		/// Starts and new bot
		/// </summary>
		private void PushBot()
		{
			ClientDriver newClient = null;

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
				Output("Waiting " + (settings.JoinDelay + TemporaryJoinDelay) + "ms...");
				for (int i = 0; i < (settings.JoinDelay + TemporaryJoinDelay) / 100; i++)
				{
					System.Threading.Thread.Sleep(100);

					if (isShuttindDown)
					{
						Output("Canceling PushBot()");
						OnCompletion();
						return;
					}
				}

				TemporaryJoinDelay = 0;

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
		//void OnLoggerMessage(object sender, Logger.LoggerArgs e)
		//{
		//    Output("[" + e.Source + "] " + e.Message);
		//}
		void OnLoggerMessage(string source, string message)
		{
			Output("[" + source + "] " + message);
		}

		/// <summary>
		/// Fired when player count has changed
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">New player count and information about player who joined/left game</param>
		void newClient_OnPlayerCountChanged(object sender, PlayerCountArgs e)
		{
			// Let our listeners know the player count has changed
			var tempHandler = OnPlayerCountChange;
			if (OnPlayerCountChange != null)
			{
				tempHandler.BeginInvoke(clients[0].PlayerNames, null, null);
			}

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

			FailedClientCounts[source.ClientIndex]++;
			Output("Client failure #" + FailedClientCounts[source.ClientIndex]);

			if (FailedClientCounts[source.ClientIndex] <= 3)
			{
				RecycleClient(source);
			}
			else
			{
				Output("Client failed too many times, ignoring it from now on");
			}

			if (e.Type == FailureArgs.FailureTypes.GameserverDeniedConnection)
			{
				settings.JoinDelay += 500;
				TemporaryJoinDelay = 10000;
				Output("Increasing JoinDelay by 500ms and adding a 10second temporary join delay");
			}

			Output("Attempting next client");
			PushBot();
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

			RecycleClient(source);
		}

		/// <summary>
		/// Moves specified client from pool of running clients to pool of available clients
		/// </summary>
		/// <param name="deadClient"></param>
		void RecycleClient(ClientDriver deadClient)
		{
			lock (clients)
			{
				clients.Remove(deadClient);
				if (clients.Count == 0)
				{
					// If the last client failed or disconnected then we can no longer operate
					OnCompletion();
				}
			}

			AddBotAsAvailable(deadClient);
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
