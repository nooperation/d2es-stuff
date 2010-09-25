using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;

namespace IdleClient.Game
{
	class GameClient
	{
		/// <summary>
		/// Gets the number of players currently in game.
		/// </summary>
		public int PlayerCount { get; protected set; }

		private bool waitingForExitMessageResponse;
		private TcpClient client;
		private Config settings;
		private string characterName;
		private GameServer gameServer;

		/// <summary>
		/// Creates a new game client using a previous game server connection and settings
		/// </summary>
		/// <param name="gameServerClient">The game server connection.</param>
		/// <param name="settings">Options for controlling the operation.</param>
		public GameClient(GameServer gameServer, TcpClient gameServerClient, Config settings, int playerCount, string characterName)
		{
			this.settings = settings;
			client = gameServerClient;
			this.characterName = characterName;
			this.PlayerCount = playerCount;
			this.gameServer = gameServer;
		}

		/// <summary>
		/// Handles packets sent from the game server
		/// </summary>
		/// <param name="packet">The packet.</param>
		public void OnPacket(GameServerPacket packet)
		{
			switch (packet.Id)
			{
				case GameServerInPacketType.GameMessage:
					OnGameMessage(packet);
					break;
				case GameServerInPacketType.InformationMessage:
					OnInformationMessage(packet);
					break;
				default:
					break;
			}
		}

		/// <summary>
		/// Handles InformationMessageIn packets. Keeps track of total number of players
		/// in game.
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnInformationMessage(GameServerPacket packet)
		{
			InformationMessageIn fromServer = new InformationMessageIn(packet);
			Console.WriteLine(fromServer);

			switch (fromServer.Event)
			{
				case InformationMessageIn.InformationEvents.PlayerTimeout:
				case InformationMessageIn.InformationEvents.PlayerDropped:
				case InformationMessageIn.InformationEvents.PlayerQuit:
					PlayerCount--;
					Console.WriteLine("{0} players remaining", PlayerCount);
					break;
				case InformationMessageIn.InformationEvents.PlayerJoined:
					PlayerCount++;
					Console.WriteLine("{0} players total", PlayerCount);
					break;
				default:
					break;
			}
		}

		/// <summary>
		/// Handles chat messages from other players and objects in game
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnGameMessage(GameServerPacket packet)
		{
			GameMessageIn fromServer = new GameMessageIn(packet);
			Console.WriteLine(fromServer);

			if (fromServer.ChatType == GameMessageIn.ChatTypes.ChatMessage)
			{
				if (fromServer.CharacterName.ToLower() == characterName.ToLower())
				{
					if (waitingForExitMessageResponse)
					{
						gameServer.Disconnect();
						return;
					}
				}
				else if (fromServer.CharacterName.ToLower() == settings.MasterName.ToLower())
				{
					if (fromServer.Message == "#exit")
					{
						Say("Bye");
						SendPacket(new ExitGameOut());
						waitingForExitMessageResponse = true;
						return;
					}
				}
			}
		}

		/// <summary>
		/// Causes this client to send a chat message to the game
		/// </summary>
		/// <param name="message">The message.</param>
		public void Say(string message)
		{
			SendMessageOut toServer = new SendMessageOut(message);
			SendPacket(toServer);
		}

		/// <summary>
		/// Causes this client to send a chat message to a player.
		/// </summary>
		/// <param name="message">The message.</param>
		/// <param name="target">Player to send message to.</param>
		public void Say(string message, string target)
		{
			SendMessageOut toServer = new SendMessageOut(message, target);
			SendPacket(toServer);
		}

		/// <summary>
		/// Pings the game server
		/// </summary>
		public void SendPing()
		{
			PingOut toServer;

			// 0x35 is just some number I threw in there that was close to what the real d2 client
			// was sending, probably works fine with 0.
			toServer = new PingOut((uint)DateTime.Now.Ticks, 0x35);

			SendPacket(toServer);
		}

		/// <summary>
		/// Sends a packet to the game server. 
		/// </summary>
		/// <param name="packet">The packet.</param>
		public void SendPacket(IOutPacket packet)
		{
			SendPacket((GameServerOutPacketType)packet.Id, packet.GetBytes());
		}

		/// <summary>
		/// Sends a packet to the game server.
		/// </summary>
		/// <param name="type">The type of packet to send.</param>
		/// <param name="data">The packet data (Not the packet header).</param>
		public void SendPacket(GameServerOutPacketType type, byte[] data)
		{
			GameClientPacket packet = new GameClientPacket();
			packet.Id = type;
			packet.Data = data;

			if (settings.ShowPackets)
			{
				Console.WriteLine("C -> S: " + packet);
			}
			if (settings.ShowPacketData)
			{
				Console.WriteLine("Data: {0:X2} {1}", (byte)packet.Id, Util.GetStringOfBytes(packet.Data, 0, packet.Data.Length));
			}

			byte[] packetBytes = packet.GetBytes();
			try
			{
				client.GetStream().Write(packetBytes, 0, packetBytes.Length);
			}
			catch (Exception ex)
			{
				if (!gameServer.IsDisconnecting)
				{
					Console.WriteLine("Failed to send packet to game server: " + ex.Message);
					gameServer.Disconnect();
				}
				return;
			}
		}
	}
}
