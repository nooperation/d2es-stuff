using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;

namespace IdleClient.Game
{
	partial class GameServer : ClientBase
	{
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
			Log(fromServer);

			switch (fromServer.Event)
			{
				case InformationMessageIn.InformationEvents.PlayerTimeout:
				case InformationMessageIn.InformationEvents.PlayerDropped:
				case InformationMessageIn.InformationEvents.PlayerQuit:
					PlayerNames.Remove(fromServer.First);
					PlayerCount--;
					Log(String.Format("{0}/{1} players remaining", PlayerCount, MaxPlayers));
					FireOnPlayerCountEvent(new PlayerCountArgs(PlayerCount, MaxPlayers, fromServer.First, false, PlayerNames.Contains(fromServer.First.ToLower())));
					break;
				case InformationMessageIn.InformationEvents.PlayerJoined:
					PlayerNames.Add(fromServer.First);
					PlayerCount++;
					Log(String.Format("{0}/{1} players total", PlayerCount, MaxPlayers));
					FireOnPlayerCountEvent(new PlayerCountArgs(PlayerCount, MaxPlayers, fromServer.First, true, PlayerNames.Contains(fromServer.First.ToLower())));
					break;
				default:
					break;
			}
		}

		public void LeaveGame()
		{
			if (IsInGame)
			{
				IsExiting = true;
				SendPacket(new ExitGameOut());
			}
		}

		/// <summary>
		/// Handles chat messages from other players and objects in game
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnGameMessage(GameServerPacket packet)
		{
			GameMessageIn fromServer = new GameMessageIn(packet);
			Log(fromServer);

			if (fromServer.ChatType == GameMessageIn.ChatTypes.ChatMessage)
			{
				if (fromServer.CharacterName.ToLower() == settings.MasterName.ToLower())
				{
					if (fromServer.Message == "#exit")
					{
						Say("Bye");
						LeaveGame();
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

	}
}
