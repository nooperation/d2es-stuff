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
	class ClientDriver
	{

		/// <summary> Client had a nonrecoverable error. </summary>
		public event EventHandler<FailureArgs> OnFailure;

		/// <summary> Current player count of the game has changed. </summary>
		public event EventHandler<PlayerCountArgs> OnPlayerCountChanged;

		/// <summary> Client disconnected from the game server. </summary>
		public event EventHandler OnClientDisconnect;

		/// <summary> Client successfully entered the game. </summary>
		public event EventHandler OnEnterGame;

		/// <summary>
		/// Index of this client, assigned and used by ClientDriver manager
		/// </summary>
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

		/// <summary>
		/// Name of the character
		/// </summary>
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

		public ClientDriver(Config settings, int clientIndex)
		{
			this.settings = settings;
			this.ClientIndex = clientIndex;
		}

		~ClientDriver()
		{
			if (chatServer != null)
			{
				chatServer.Disconnect();
			}
			if (realmServer != null)
			{
				realmServer.Disconnect();
			}
			if (gameServer != null)
			{
				gameServer.Disconnect();
			}
		}

		/// <summary>
		/// Asynchronously creates a new client and starts its connection to the game
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

			chatServerThread.Name = "CHAT:" + CharacterName;
			realmServerThread.Name = "REALM:" + CharacterName;
			gameServerThread.Name = "GAME:" + CharacterName;

			chatServer.ReadyToConnectToRealmServer += new EventHandler<RealmServerArgs>(chatServer_ReadyToConnectToRealmServer);
			chatServer.OnFailure += new EventHandler<FailureArgs>(chatServer_OnFailure);

			realmServer.ReadyToConnectToGameServer += new EventHandler<GameServerArgs>(realmServer_ReadyToConnectToGameServer);
			realmServer.OnDisconnect += new EventHandler(realmServer_OnDisconnect);
			realmServer.OnFailure += new EventHandler<FailureArgs>(realmServer_OnFailure);

			gameServer.OnEnterGame += new EventHandler(gameServer_OnEnterGame);
			gameServer.OnDisconnect += new EventHandler(gameServer_OnDisconnect);
			gameServer.OnFailure += new EventHandler<FailureArgs>(gameServer_OnFailure);
			gameServer.OnPlayerCountChanged += new EventHandler<PlayerCountArgs>(gameClient_OnPlayerCountChanged);

			IsRunning = true;
			chatServerThread.Start();
		}

		private void chatServer_OnFailure(object sender, FailureArgs e)
		{
			Log("ChatServer failed -> " + e.ToString());
			FireOnFailureEvent(e.Type, e.Message);
		}

		private void realmServer_OnFailure(object sender, FailureArgs e)
		{
			Log("RealmServer failed -> " + e.ToString());
			FireOnFailureEvent(e.Type, e.Message);
		}

		private void gameServer_OnFailure(object sender, FailureArgs e)
		{
			Log("GameServer failed -> " + e.ToString());
			FireOnFailureEvent(e.Type, e.Message);
		}

		private void gameServer_OnDisconnect(object sender, EventArgs e)
		{
			Log("GameServer disconnected");
			chatServer.Disconnect();
			realmServer.Disconnect();
			FireOnClientDisconnectEvent();
		}

		private void realmServer_OnDisconnect(object sender, EventArgs e)
		{
			Log("RealmServer disconnected");
			chatServer.Disconnect();
		}

		private void realmServer_ReadyToConnectToGameServer(object sender, GameServerArgs e)
		{
			Log("RealmServer says we're ready to connect to game server");
			gameServerThread.Start(e);
			realmServer.Disconnect();
		}

		private void chatServer_ReadyToConnectToRealmServer(object sender, RealmServerArgs e)
		{
			Log("ChatServer says we're ready to connect to realm server");
			realmServerThread.Start(e);
		}

		private void gameClient_OnPlayerCountChanged(object sender, PlayerCountArgs e)
		{
			//Log("Player count changed -> " + e.ToString());
			FireOnPlayerCountEvent(e);
		}

		private void gameServer_OnEnterGame(object sender, EventArgs e)
		{
			Log("Bot successfully connected to game, ready for more");
			FireOnEnterGameEvent();
		}

		private void FireOnFailureEvent(FailureArgs.FailureTypes failureTypes, string message)
		{
			EventHandler<FailureArgs> tempHandler = OnFailure;
			if (tempHandler != null)
			{
				tempHandler(this, new FailureArgs(failureTypes, message));
			}
		}

		private void FireOnClientDisconnectEvent()
		{
			EventHandler tempHandler = OnClientDisconnect;

			if (tempHandler != null)
			{
				tempHandler(this, new EventArgs());
			}
		}

		private void FireOnEnterGameEvent()
		{
			EventHandler tempHandler = OnEnterGame;

			if (tempHandler != null)
			{
				tempHandler(this, new EventArgs());
			}
		}

		private void FireOnPlayerCountEvent(PlayerCountArgs args)
		{
			EventHandler<PlayerCountArgs> tempHandler = OnPlayerCountChanged;
			if (tempHandler != null)
			{
				tempHandler(this, args);
			}
		}

		public void Disconnect()
		{
			IsDisconnecting = true;

			if (chatServer != null)
			{
				chatServer.Disconnect();
			}
			if (realmServer != null)
			{
				realmServer.Disconnect();
			}
			if (gameServer != null)
			{
				gameServer.Disconnect();
			}
		}

		public void Log(object message)
		{
			Logger.Instance.LogDriver(this, CharacterName + " -> " + message.ToString());
		}

		public override string ToString()
		{
			return "MAIN";
		}
	}
}
