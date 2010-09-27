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

		public int MaxPlayerCount { get; set; }
		List<ClientDriver> clients = new List<ClientDriver>();
		Config settings;

		// Temp hack to make sure PushBot() isn't called by PlayerChanged event during the inital
		//  set of bots being pushed.
		//TODO: Needs a much better way to keep track of the bots
		private bool initalBotPushing = false;


		public Program()
		{
			settings = new Config("IdleClient.ini");
			MaxPlayerCount = 8;

			if (settings.BotNames.Count == 0)
			{
				Console.WriteLine("Main: No bots are defined");
				return;
			}

			initalBotPushing = true;
			PushBot();
			clients[0].OnPlayerCountChanged += new EventHandler<PlayerCountArgs>(firstClient_OnPlayerCountChanged);
		}


		private void PushBot()
		{
			if (clients.Count + 1 >= MaxPlayerCount || clients.Count >= settings.BotNames.Count)
			{
				initalBotPushing = false;
				return;
			}

			ClientDriver newClient = new ClientDriver(settings, clients.Count);
			newClient.OnFailure += new EventHandler<FailureArgs>(newClient_OnFailure);
			newClient.OnEnterGame += new EventHandler(newClient_OnEnterGame);
			newClient.OnClientDisconnect += new EventHandler(newClient_OnClientDisconnect);
			newClient.Start();

			clients.Add(newClient);
		}

		private void Popbot()
		{
			Console.WriteLine("Main: PopBot()");
			if (clients.Count <= 1)
			{
				return;
			}

			clients[clients.Count - 1].Disconnect();
			Console.WriteLine("Main: ordered client " + (clients.Count - 1) + " to disconnect");
		}

		void firstClient_OnPlayerCountChanged(object sender, PlayerCountArgs e)
		{
			Console.WriteLine("Main: " + e.PlayerCount + " >=? " + MaxPlayerCount);

			if (!initalBotPushing)
			{
				if (e.PlayerCount >= MaxPlayerCount)
				{
					Popbot();
				}
				else if (e.PlayerCount < MaxPlayerCount - 1)
				{
					PushBot();
				}
			}
		}

		void newClient_OnClientDisconnect(object sender, EventArgs e)
		{
			ClientDriver source = sender as ClientDriver;
			if (source.ClientIndex == clients.Count - 1)
			{
				Console.WriteLine("Main: Removing disconnected client " + (clients.Count - 1));
				clients.Remove(source);
			}
		}

		void newClient_OnEnterGame(object sender, EventArgs e)
		{
			ClientDriver source = sender as ClientDriver;

			if (source.ClientIndex == 0)
			{
				MaxPlayerCount = source.MaxPlayers;
			}

			PushBot();
		}

		void newClient_OnFailure(object sender, FailureArgs e)
		{

		}

	}
}
