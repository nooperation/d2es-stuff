using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace IdleClient.Game
{
	partial class GameServer : ClientBase
	{
		/// <summary> 
		/// Player count in the game has changed. 
		/// </summary>
		public event EventHandler<PlayerCountArgs> OnPlayerCountChanged;

		/// <summary> Raised when client successfully enters a game. </summary>
		public event EventHandler OnEnterGame;

		/// <summary>
		/// Client is in the game
		/// </summary>
		public bool IsInGame { get; protected set; }

		/// <summary>
		/// Maximum amount of players for this game. Retrieved during connection to game server.
		/// </summary>
		public int MaxPlayers { get; protected set; }

		/// <summary>
		/// Gets the number of players currently in game.
		/// </summary>
		public int PlayerCount { get; protected set; }

		/// <summary>
		/// Client has sent LeaveGame packet to server, expecting connection loss
		/// </summary>
		public bool IsExiting { get; set; }

		private Thread pingThread;
		private Realm.GameServerArgs gameServerArgs;

		/// <summary>
		/// Creates a new client.
		/// </summary>
		/// <param name="settings">Options for controlling the operation.</param>
		/// <param name="characterName">Name of the character.</param>
		public GameServer(Config settings, string characterName) : base(settings, characterName)
		{
			ClientName = "GAME";
		}

		/// <summary>
		/// Initialises this object.
		/// </summary>
		/// <param name="args">The arguments to pass to this client.</param>
		protected override void Init(object args)
		{
			gameServerArgs = args as Realm.GameServerArgs;

			this.address = gameServerArgs.Address;
			this.port = gameServerArgs.Port;
			this.MaxPlayers = gameServerArgs.MaxPlayers;
			this.PlayerCount = gameServerArgs.PlayerCount;
		}

		/// <summary>
		/// The main loop for communicating with the server.
		/// </summary>
		protected override void MainLoop()
		{
			byte[] buffer = new byte[0];

			while (client.Connected)
			{
				List<GameServerPacket> packets;

				try
				{
					packets = ReceivePackets(ref buffer);
				}
				catch (Exception ex)
				{
					if (!IsDisconnecting && !IsExiting)
					{
						Fail(FailureArgs.FailureTypes.FailedToReceive, "Failed to receive game server packets: " + ex.Message);
					}
					break;
				}

				foreach (var packet in packets)
				{
					if (settings.ShowPackets)
					{
						LogDebug("S -> C: " + packet);
					}
					if (settings.ShowPacketData)
					{
						LogDebug(String.Format("Data: {0:X2} {1}", (byte)packet.Id, Util.GetStringOfBytes(packet.Data, 0, packet.Data.Length)));
					}

					switch (packet.Id)
					{
						case GameServerInPacketType.GameLogonReceipt:
							OnGameLogonReceipt(packet);
							break;
						case GameServerInPacketType.GameLogonSuccess:
							OnGameLogonSuccess(packet);
							break;
						case GameServerInPacketType.RequestLogonInfo:
							OnRequestLogonInfo(packet);
							break;
						case GameServerInPacketType.BadSave:
							Fail(FailureArgs.FailureTypes.BadCharacterData, "Invalid character data, unable to join game");
							break;
						default:
							break;
					}
					
					OnPacket(packet);
				}
			}

			if (pingThread != null && pingThread.IsAlive)
			{
				LogServer("Terminating ping thread");
				pingThread.Abort();
			}
		}

		/// <summary>
		/// Game server has completed our request to join the game. We request to enter the
		/// game along with starting the ping thread.
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnGameLogonSuccess(GameServerPacket packet)
		{
			SendPacket(new EnterGameOut());

			// Ping thread used to keep our connection to the server alive
			pingThread = new Thread(() =>
			{
				LogServer("Ping thread started");
				while (client.Connected)
				{
					Thread.Sleep(7000);
					SendPing();
				}
				LogServer("Ping thread ended");
			});

			pingThread.Name = "PING:" + characterName;

			pingThread.Start();
			IsInGame = true;
			FireOnEnterGameEvent();

		}

		/// <summary>
		/// Handles the GameLogonReceipt packet. Should contain some game information, but 
		/// we queried the game before joining. Just wait for the GameLogonSuccess packet.
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnGameLogonReceipt(GameServerPacket packet)
		{

		}

		/// <summary>
		/// Handles the GameLogonRequestIn packet. This packet is sent to us when we first connect
		/// to the game server. We reply with a logon request and a ping
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnRequestLogonInfo(GameServerPacket packet)
		{
			GameLogonRequestOut toServer = new GameLogonRequestOut(
				gameServerArgs.GameHash,
				gameServerArgs.GameToken,
				settings.ClientVersion,
				(byte)gameServerArgs.CharacterClass,
				gameServerArgs.CharacterName
			);

			SendPacket(toServer);
			SendPing();
		}

		/// <summary>
		/// Receives a list of packets from the game server
		/// </summary>
		/// <returns>List of packets read from the game server</returns>
		private List<GameServerPacket> ReceivePackets(ref byte[] buffer)
		{
			NetworkStream ns = client.GetStream();
			List<GameServerPacket> packets = new List<GameServerPacket>();

			bool needsMoreData = false;

			while (true)
			{
				if (buffer.Length == 0 || needsMoreData)
				{
					Util.Receive(ns, ref buffer);
					needsMoreData = false;
				}

				byte[] decompressedData;
				if (!Compression.DecompressAndConsumeBuffer(ref buffer, out decompressedData))
				{
					needsMoreData = true;
					continue;
				}

				while (decompressedData.Length > 0)
				{
					GameServerPacket packet;

					try
					{
						packet = GetNextDecompressedPacket(ref decompressedData);
					}
					catch (Exception)
					{
						return packets;
					}

					packets.Add(packet);
				}

				return packets;
			}
		}

		/// <summary>
		/// Gets the next decompressed packet from byte array. Processed data will be removed from
		/// input array.
		/// </summary>
		/// <exception cref="ApplicationException">Thrown when encountering an unhandled packet with no size.</exception>
		/// <param name="decompressedData">[in,out] Byte array containing decompressed data.</param>
		/// <returns>The next decompressed packet.</returns>
		private GameServerPacket GetNextDecompressedPacket(ref byte[] decompressedData)
		{
			GameServerPacket packet = new GameServerPacket();
			int decompressedPacketSize = Compression.GetDecompressedPacketSize(decompressedData);

			if (decompressedPacketSize == 0xFF)
			{
				Fail(FailureArgs.FailureTypes.UnknownPacketSize, String.Format("Unhandled special packet {0:X2}", decompressedData[0]));
				throw new ApplicationException("Bad packet data encountered (No size)");
			}

			packet.Id = (GameServerInPacketType)decompressedData[0];
			packet.Data = new byte[decompressedPacketSize - 1];

			if (packet.Data.Length > 0)
			{
				Array.Copy(decompressedData, 1, packet.Data, 0, packet.Data.Length);
			}

			Util.RemoveBeginningBytes(ref decompressedData, decompressedPacketSize);

			return packet;
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
				LogDebug("C -> S: " + packet);
			}
			if (settings.ShowPacketData)
			{
				LogDebug(String.Format("Data: {0:X2} {1}", (byte)packet.Id, Util.GetStringOfBytes(packet.Data, 0, packet.Data.Length)));
			}

			byte[] packetBytes = packet.GetBytes();
			try
			{
				client.GetStream().Write(packetBytes, 0, packetBytes.Length);
			}
			catch (Exception)
			{
				if (!IsDisconnecting && !IsExiting)
				{
					Fail(FailureArgs.FailureTypes.FailedToSend, "Failed to send packet to game server");
				}
				return;
			}
		}

		/// <summary>
		/// Raises the on enter game event. 
		/// </summary>
		private void FireOnEnterGameEvent()
		{
			EventHandler tempHandler = OnEnterGame;
			if (tempHandler != null)
			{
				tempHandler(this, new EventArgs());
			}
		}

		/// <summary>
		/// Raises the on player count event. 
		/// </summary>
		/// <param name="args">The arguments.</param>
		private void FireOnPlayerCountEvent(PlayerCountArgs args)
		{
			EventHandler<PlayerCountArgs> tempHandler = OnPlayerCountChanged;
			if (tempHandler != null)
			{
				tempHandler(this, args);
			}
		}
	}
}
