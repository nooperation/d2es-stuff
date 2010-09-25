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

		public EventHandler OnReadyForNextClient;

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
			realmServer.ReadyToConnectToGameServer += new EventHandler<GameServerArgs>(realmServer_ReadyToConnectToGameServer);
			realmServer.OnDisconnect += new EventHandler(realmServer_OnDisconnect);
			gameServer.OnDisconnect += new EventHandler(gameServer_OnDisconnect);
			gameServer.OnEnterGame += new EventHandler(gameServer_OnEnterGame);
			chatServerThread.Start();
		}

		void gameServer_OnEnterGame(object sender, EventArgs e)
		{
			EventHandler temp = OnReadyForNextClient;
			if (temp != null)
			{
				OnReadyForNextClient(this, new EventArgs());
			}
		}

		void gameServer_OnDisconnect(object sender, EventArgs e)
		{
			chatServer.Disconnect();
			realmServer.Disconnect();
		}

		void realmServer_ReadyToConnectToGameServer(object sender, GameServerArgs e)
		{
			gameServerThread.Start(e);
			realmServer.Disconnect();
		}

		void realmServer_OnDisconnect(object sender, EventArgs e)
		{
			chatServer.Disconnect();
		}

		void chatServer_ReadyToConnectToRealmServer(object sender, RealmServerArgs e)
		{
			realmServerThread.Start(e);
		}

	}
}
