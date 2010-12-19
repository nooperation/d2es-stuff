using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using IdleClient.Game;
using IdleClient.Chat;
using IdleClient.Realm;
using System.Threading;

namespace IdleClient
{
	public class ClientDriver
	{
		/// <summary> Client had a nonrecoverable error. </summary>
		public event EventHandler<FailureArgs> OnFailure;

		/// <summary> Current player count of the game has changed. </summary>
		public event EventHandler<PlayerCountArgs> OnPlayerCountChanged;

		/// <summary> Client disconnected from the game server. </summary>
		public event EventHandler OnClientDisconnect;

		/// <summary> Client successfully entered the game. </summary>
		public event EventHandler OnEnterGame;

		/// <summary> Raised when client is shut down, such as from the #exit of from a player </summary>
		public event EventHandler OnShutdown;

		/// <summary> Index of this client, assigned and used by ClientDriver manager </summary>
		public int ClientIndex { get; protected set; }

		/// <summary>
		/// Client is marked as disconnecting.
		/// TODO: Client might not throw the disconnected event if gameserver isn't connected
		/// </summary>
		public bool IsDisconnecting { get; protected set; }

		/// <summary>
		/// Gets or sets a value indicating whether this object is running.
		/// TODO: Only used in testing so far
		/// </summary>
		public bool IsRunning { get; protected set; }

		/// <summary>
		/// Gets the maximum players count retrieved upon joining the game. Can only be checked
		/// after client is initalized, otherwise null exception
		/// </summary>
		public int MaxPlayers
		{
			get { return gameServer.MaxPlayers; }
		}

		/// <summary> List of player names currently in the game </summary>
		public List<string> PlayerNames
		{
			get
			{
				if (gameServer.IsInGame)
				{
					return gameServer.PlayerNames;
				}

				return new List<string>();
			}
		}

		/// <summary> Name of the character </summary>
		public string CharacterName 
		{ 
			get { return settings.BotNames[ClientIndex]; } 
		}

		private Thread chatServerThread = null;
		private Thread realmServerThread = null;
		private Thread gameServerThread = null;

		private Config settings;
		private ChatServer chatServer;
		private RealmServer realmServer;
		private GameServer gameServer;

		/// <summary>
		/// Creates a new client based on specified settings. Bot name determined by clientIndex
		/// using list of names found in settings.
		/// </summary>
		/// <param name="settings">Idleclient settings</param>
		/// <param name="clientIndex">Index of bot name from settings.BotNames</param>
		public ClientDriver(Config settings, int clientIndex)
		{
			this.settings = settings;
			this.ClientIndex = clientIndex;
		}

		/// <summary>
		/// Terminates all connections.
		/// TODO: Call terminate from destructor or just forcefully disconnect everything here?
		/// </summary>
		~ClientDriver()
		{
			Terminate();
		}

		/// <summary>
		/// Causes the client to send a chat message to the game
		/// </summary>
		/// <param name="message">Message to say to player in the game</param>
		public void Say(string message)
		{
			if (gameServer != null && gameServer.IsInGame)
			{
				gameServer.Say(message);
			}
		}

		/// <summary>
		/// Terminates all connetions made by this client
		/// </summary>
		public void Terminate()
		{
			IsDisconnecting = true;

			// If we're connected to a game then we will want to let the server know we're disconnecting
			// so our character is disconnected from the server itself. Otherwise we will just 'drop due to timeout'
			// and probably fill the server's error logs up with garbage.
			if (gameServerThread != null && gameServerThread.IsAlive && gameServer != null)
			{
				if (gameServer.IsInGame)
				{
					gameServer.LeaveGame();

					// Give the client some time to send the leave game packet to the server before
					// disconnecting. After server get the message it should close our connection so
					// it shouldn't take a full 1000ms
					gameServerThread.Join(1000);
				}

				gameServer.Disconnect();

				if (!gameServerThread.Join(1000))
				{
					gameServerThread.Abort();
				}
			}
			if (chatServerThread != null && chatServerThread.IsAlive && chatServer != null)
			{
				chatServer.Disconnect();
				if (!chatServerThread.Join(1000))
				{
					chatServerThread.Abort();
				}
			}
			if (realmServerThread != null && realmServerThread.IsAlive && realmServer != null)
			{
				realmServer.Disconnect();
				if (!realmServerThread.Join(1000))
				{
					realmServerThread.Abort();
				}
			}
		}

		/// <summary>
		/// Asynchronously creates a new client and starts its connection to the game servers
		/// </summary>
		public void Start()
		{
			chatServer = new ChatServer(settings, settings.BotNames[ClientIndex]);
			realmServer = new RealmServer(settings, settings.BotNames[ClientIndex]);
			gameServer = new GameServer(settings, settings.BotNames[ClientIndex]);

			chatServerThread = new Thread(() =>
			{
				chatServer.Run(null);
			});

			realmServerThread = new Thread((args) =>
			{
				realmServer.Run(args);
			});

			gameServerThread = new Thread((args) =>
			{
				gameServer.Run(args);
			});

			// Named threads for debugging purposes only
			chatServerThread.Name = "CHAT:" + CharacterName;
			realmServerThread.Name = "REALM:" + CharacterName;
			gameServerThread.Name = "GAME:" + CharacterName;

			chatServerThread.IsBackground = true;
			realmServerThread.IsBackground = true;
			gameServerThread.IsBackground = true;

			chatServer.ReadyToConnectToRealmServer += new EventHandler<RealmServerArgs>(chatServer_ReadyToConnectToRealmServer);
			chatServer.OnFailure += new EventHandler<FailureArgs>(chatServer_OnFailure);

			realmServer.ReadyToConnectToGameServer += new EventHandler<GameServerArgs>(realmServer_ReadyToConnectToGameServer);
			realmServer.OnDisconnect += new EventHandler(realmServer_OnDisconnect);
			realmServer.OnFailure += new EventHandler<FailureArgs>(realmServer_OnFailure);

			gameServer.OnEnterGame += new EventHandler(gameServer_OnEnterGame);
			gameServer.OnDisconnect += new EventHandler(gameServer_OnDisconnect);
			gameServer.OnFailure += new EventHandler<FailureArgs>(gameServer_OnFailure);
			gameServer.OnPlayerCountChanged += new EventHandler<PlayerCountArgs>(gameClient_OnPlayerCountChanged);
			gameServer.OnShutdown += new EventHandler(gameServer_OnShutdown);

			IsRunning = true;
			chatServerThread.Start();
		}

		/// <summary>
		/// Fired when client(s) have been ordered to shutdown
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Unused</param>
		void gameServer_OnShutdown(object sender, EventArgs e)
		{
			FireOnShutdownEvent();
		}

		/// <summary>
		/// Fired when client fails when communicating with the chat server
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Details of the failure</param>
		private void chatServer_OnFailure(object sender, FailureArgs e)
		{
			Log("ChatServer failed -> " + e.ToString());
			FireOnFailureEvent(e.Type, e.Message);
		}

		/// <summary>
		/// Fired when client fails when communicating with the realm server
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Details of the failure</param>
		private void realmServer_OnFailure(object sender, FailureArgs e)
		{
			Log("RealmServer failed -> " + e.ToString());
			FireOnFailureEvent(e.Type, e.Message);
		}

		/// <summary>
		/// Fired when client fails when communicating with the game server
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Details of the failure</param>
		private void gameServer_OnFailure(object sender, FailureArgs e)
		{
			Log("GameServer failed -> " + e.ToString());
			FireOnFailureEvent(e.Type, e.Message);
		}

		/// <summary>
		/// Raised when client is disconnected from the game server
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Unused</param>
		private void gameServer_OnDisconnect(object sender, EventArgs e)
		{
			Log("GameServer disconnected");
			chatServer.Disconnect();
			realmServer.Disconnect();
			FireOnClientDisconnectEvent();
		}

		/// <summary>
		/// Raised when client is disconnected from the realm server
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Unused</param>
		private void realmServer_OnDisconnect(object sender, EventArgs e)
		{
			Log("RealmServer disconnected");
			chatServer.Disconnect();
		}

		/// <summary>
		/// raised when client is ready to connect to the game server
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Arguments needed to connect to the game server</param>
		private void realmServer_ReadyToConnectToGameServer(object sender, GameServerArgs e)
		{
			Log("RealmServer says we're ready to connect to game server");
			gameServerThread.Start(e);
			realmServer.Disconnect();
		}

		/// <summary>
		/// raised when client is ready to connect to the realm server
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Arguments needed to connect to the realm server</param>
		private void chatServer_ReadyToConnectToRealmServer(object sender, RealmServerArgs e)
		{
			Log("ChatServer says we're ready to connect to realm server");
			realmServerThread.Start(e);
		}

		/// <summary>
		/// Raised when player count changes within the game.
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Args containing new player count</param>
		private void gameClient_OnPlayerCountChanged(object sender, PlayerCountArgs e)
		{
			//Log("Player count changed -> " + e.ToString());
			FireOnPlayerCountEvent(e);
		}

		/// <summary>
		/// Raised when the client enters the game
		/// </summary>
		/// <param name="sender">Object causing this event</param>
		/// <param name="e">Unused</param>
		private void gameServer_OnEnterGame(object sender, EventArgs e)
		{
			Log("Bot successfully connected to game, ready for more");
			FireOnEnterGameEvent();
		}

		/// <summary>
		/// Raises a Failure event
		/// </summary>
		/// <param name="failureTypes">Type of failure</param>
		/// <param name="message">Failure message</param>
		private void FireOnFailureEvent(FailureArgs.FailureTypes failureTypes, string message)
		{
			EventHandler<FailureArgs> tempHandler = OnFailure;

			if (tempHandler != null)
			{
				tempHandler.BeginInvoke(this, new FailureArgs(failureTypes, message), null, null);
			}
		}

		/// <summary>
		/// Raises a Disconnect event
		/// </summary>
		private void FireOnClientDisconnectEvent()
		{
			EventHandler tempHandler = OnClientDisconnect;

			if (tempHandler != null)
			{
				tempHandler.BeginInvoke(this, new EventArgs(), null, null);
			}
		}

		/// <summary>
		/// Raises an EnterGame event
		/// </summary>
		private void FireOnEnterGameEvent()
		{
			EventHandler tempHandler = OnEnterGame;

			if (tempHandler != null)
			{
				tempHandler.BeginInvoke(this, new EventArgs(), null, null);
			}
		}

		/// <summary>
		/// Raises a PlayerCount event
		/// </summary>
		/// <param name="args"></param>
		private void FireOnPlayerCountEvent(PlayerCountArgs args)
		{
			EventHandler<PlayerCountArgs> tempHandler = OnPlayerCountChanged;
			if (tempHandler != null)
			{
				tempHandler.BeginInvoke(this, args, null, null);
			}
		}

		/// <summary>
		/// Asynchronously raises the on player count event. 
		/// </summary>
		/// <param name="args">The arguments.</param>
		private void FireOnShutdownEvent()
		{
			EventHandler tempHandler = OnShutdown;
			if (tempHandler != null)
			{
				tempHandler.BeginInvoke(this, new EventArgs(), null, null);
			}
		}

		/// <summary>
		/// Logs a message
		/// </summary>
		/// <param name="message">Message to log</param>
		public void Log(object message)
		{
			Logger.Instance.LogDriver(this, CharacterName + " -> " + message.ToString());
		}

		/// <summary>
		/// The string representation of this client. Currently used for determining where log
		/// messages originate from (eg. GAME, RELM, CHAT)
		/// </summary>
		/// <returns>String representation of this object</returns>
		public override string ToString()
		{
			return "MAIN";
		}
	}
}
