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
		/// Gets or sets a value indicating whether the game client has a network error.
		/// </summary>
		public bool HasNetworkError { get; set; }

		/// <summary>
		/// Gets or sets a value indicating whether the game client is disconnecting.
		/// </summary>
		public bool IsDisconnecting { get; set; }


		private bool waitingForExitMessageResponse;
		private TcpClient client;
		private Config settings;


		/// <summary>
		/// Creates a new game client using a previous game server connection and settings
		/// </summary>
		/// <param name="gameServerClient">The game server connection.</param>
		/// <param name="settings">Options for controlling the operation.</param>
		public GameClient(TcpClient gameServerClient, Config settings)
		{
			this.settings = settings;
			client = gameServerClient;
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
				if (fromServer.CharacterName.ToLower() == settings.CharacterName.ToLower())
				{
					if (waitingForExitMessageResponse)
					{
						IsDisconnecting = true;
					}
				}
				else if (fromServer.CharacterName.ToLower() == settings.MasterName.ToLower())
				{
					if (fromServer.Message == "#exit")
					{
						Say("Bye");
						SendPacket(new ExitGameOut());
						waitingForExitMessageResponse = true;
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
				if (!IsDisconnecting)
				{
					Console.WriteLine("Failed to send packet to game server: " + ex.Message);
					IsDisconnecting = true;
				}
				return;
			}
		}
	}
}
