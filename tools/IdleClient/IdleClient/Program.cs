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

		Thread chatServerThread = null;
		Thread realmServerThread = null;
		Thread gameServerThread = null;

		Config settings;
		ChatServer chatServer;
		RealmServer realmServer;
		GameServer gameServer;

		void CurrentDomain_ProcessExit(object sender, EventArgs e)
		{
			chatServer.Disconnect();
			realmServer.Disconnect();
			gameServer.Disconnect();

			if (chatServerThread != null && chatServerThread.IsAlive)
			{
				chatServerThread.Join();
			}
			if (realmServerThread != null && realmServerThread.IsAlive)
			{
				realmServerThread.Join();
			}
			if (gameServerThread != null && gameServerThread.IsAlive)
			{
				gameServerThread.Join();
			}
		}

		public Program()
		{
			settings = new Config();
			chatServer = new ChatServer(settings);
			realmServer = new RealmServer(settings);
			gameServer = new GameServer(settings);

			AppDomain.CurrentDomain.ProcessExit += new EventHandler(CurrentDomain_ProcessExit);
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
			chatServerThread.Start();
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
