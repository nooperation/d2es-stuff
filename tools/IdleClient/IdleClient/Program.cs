using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.IO;
using System.Collections;
using System.Threading;
using MBNCSUtil;
using System.Reflection;

using IdleClient.Chat;
using IdleClient.Realm;
using IdleClient.Game;

namespace IdleClient
{
	class Program
	{
		static void Main(string[] args)
		{
			new Program();
		}

		public event EventHandler<FailureArgs> OnFailure;
		public event EventHandler<PlayerCountArgs> PlayerCountChanged;
		public event EventHandler OnClientDisconnect;

		private Thread chatServerThread = null;
		private Thread realmServerThread = null;
		private Thread gameServerThread = null;

		private Config settings;
		private ChatServer chatServer;
		private RealmServer realmServer;
		private GameServer gameServer;

		public Program()
		{
			settings = new Config("IdleClient.ini");

			chatServer = new ChatServer(settings, settings.BotNames[0]);
			realmServer = new RealmServer(settings, settings.BotNames[0]);
			gameServer = new GameServer(settings, settings.BotNames[0]);

			chatServerThread = new Thread(() =>
			{
				chatServer.Run();
			});

			realmServerThread = new Thread((args) =>
			{
				realmServer.Run(args);
			});

			gameServerThread = new Thread((args) =>
			{
				gameServer.Run(args);
			});

			chatServer.ReadyToConnectToRealmServer += new EventHandler<RealmServerArgs>(chatServer_ReadyToConnectToRealmServer);
			chatServer.OnFailure += new EventHandler<FailureArgs>(chatServer_OnFailure);

			realmServer.ReadyToConnectToGameServer += new EventHandler<GameServerArgs>(realmServer_ReadyToConnectToGameServer);
			realmServer.OnDisconnect += new EventHandler(realmServer_OnDisconnect);
			realmServer.OnFailure += new EventHandler<FailureArgs>(realmServer_OnFailure);

			gameServer.OnEnterGame += new EventHandler(gameServer_OnEnterGame);
			gameServer.OnDisconnect += new EventHandler(gameServer_OnDisconnect);
			gameServer.OnFailure += new EventHandler<FailureArgs>(gameServer_OnFailure);

			chatServerThread.Start();
		}

		void chatServer_OnFailure(object sender, FailureArgs e)
		{
			Console.WriteLine("Driver: ChatServer failed -> " + e.ToString());
			FireOnFailureEvent(e.Type, e.Message);
		}

		void realmServer_OnFailure(object sender, FailureArgs e)
		{
			Console.WriteLine("Driver: RealmServer failed -> " + e.ToString());
			FireOnFailureEvent(e.Type, e.Message);
		}

		void gameServer_OnFailure(object sender, FailureArgs e)
		{
			Console.WriteLine("Driver: GameServer failed -> " + e.ToString());
			FireOnFailureEvent(e.Type, e.Message);
		}

		void gameServer_OnEnterGame(object sender, EventArgs e)
		{
			Console.WriteLine("Driver: Bot successfully connected to game, ready for more");
		}

		void gameServer_OnDisconnect(object sender, EventArgs e)
		{
			Console.WriteLine("Driver: GameServer disconnected");
			chatServer.Disconnect();
			realmServer.Disconnect();
		}

		void realmServer_OnDisconnect(object sender, EventArgs e)
		{
			Console.WriteLine("Driver: RealmServer disconnected");
			chatServer.Disconnect();
		}

		void realmServer_ReadyToConnectToGameServer(object sender, GameServerArgs e)
		{
			Console.WriteLine("Driver: RealmServer says we're ready to connect to game server");
			gameServerThread.Start(e);
			realmServer.Disconnect();
		}

		void chatServer_ReadyToConnectToRealmServer(object sender, RealmServerArgs e)
		{
			Console.WriteLine("Driver: ChatServer says we're ready to connect to realm server");
			realmServerThread.Start(e);
		}

		private void FireOnFailureEvent(FailureArgs.FailureTypes failureTypes, string message)
		{
			EventHandler<FailureArgs> tempHandler = OnFailure;
			if (tempHandler != null)
			{
				tempHandler(this, new FailureArgs(failureTypes, message));
			}
		}

		private void FireOnClientDisconnect()
		{
			EventHandler tempHandler = OnClientDisconnect;

			if (tempHandler != null)
			{
				tempHandler(this, new EventArgs());
			}
		}
	}
}
