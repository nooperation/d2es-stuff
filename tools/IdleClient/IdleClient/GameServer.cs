using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.IO;
using System.Threading;

namespace IdleClient.Game
{
	class GameServer
	{
		/// <summary> Raised when the client disconnects. </summary>
		public event EventHandler OnDisconnect;
		/// <summary> Raised when a failure occurs. This is most likely nonrecoverable. </summary>
		public event EventHandler<FailureArgs> OnFailure;
		/// <summary> Raised when client successfully enters a game. </summary>
		public event EventHandler OnEnterGame;

		public event EventHandler<PlayerCountArgs> OnPlayerCountChanged;

		/// <summary>
		/// Gets value indicating whether this object is disconnecting. Used to ignore exceptions
		/// such as Sending a packet while a planned disconnect is going on.
		/// </summary>
		public bool IsDisconnecting { get; protected set; }

		/// <summary>
		/// Maximum amount of players for this game. Retrieved during connection to game server.
		/// </summary>
		public int MaxPlayers { get; protected set; }

		private GameClient gameClient;
		private Config settings;
		private Thread pingThread;
		private TcpClient client = new TcpClient();
		private Realm.GameServerArgs gameServerArgs;
		private string characterName;

		/// <summary>
		/// Creates a new game server client with specified settings
		/// </summary>
		/// <param name="settings">Options for controlling the operation.</param>
		public GameServer(Config settings, string characterName)
		{
			this.settings = settings;
			this.characterName = characterName;
		}

		/// <summary>
		/// Entry point for game server thread. 
		/// </summary>
		/// <param name="args">The game server arguments from the realm server.</param>
		public void Run(object args)
		{
			gameServerArgs = args as Realm.GameServerArgs;
			this.MaxPlayers = gameServerArgs.MaxPlayers;
			Console.WriteLine("Connecting to game server " + gameServerArgs.Address + ":" + gameServerArgs.Port);

			try
			{
				client = new TcpClient(gameServerArgs.Address, gameServerArgs.Port);
			}
			catch (SocketException ex)
			{
				Console.WriteLine("Failed to connect to game server: " + ex.Message);
				FireOnFailureEvent(FailureArgs.FailureTypes.UnableToConnect, "Failed to connect to game server: " + ex.Message);
				FireOnDisconnectEvent();
				return;
			}

			using (client)
			{
				gameClient = new GameClient(this, client, settings, gameServerArgs.PlayerCount, characterName);
				gameClient.OnPlayerCountChanged += new EventHandler<PlayerCountArgs>(gameClient_OnPlayerCountChanged);
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
						if (!IsDisconnecting)
						{
							Console.WriteLine("Failed to receive game server packets: " + ex.Message);
							FireOnFailureEvent(FailureArgs.FailureTypes.FailedToReceive, "Failed to receive game server packets: " + ex.Message);
							Disconnect();
						}
						break;
					}

					foreach (var packet in packets)
					{
						if (settings.ShowPackets)
						{
							Console.WriteLine("S -> C: " + packet);
						}
						if (settings.ShowPacketData)
						{
							Console.WriteLine("Data: {0:X2} {1}", (byte)packet.Id, Util.GetStringOfBytes(packet.Data, 0, packet.Data.Length));
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
								Console.WriteLine("Error: The save file for this character is invalid");
								Disconnect();
								break;
							default:
								break;
						}

						if (gameClient != null)
						{
							gameClient.OnPacket(packet);
						}
					}
				}
			}

			Console.WriteLine("Game server: Disconnected");
			FireOnDisconnectEvent();
		}

		/// <summary>
		/// Game server has completed our request to join the game. We request to enter the
		/// game along with starting the ping thread.
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnGameLogonSuccess(GameServerPacket packet)
		{
			gameClient.SendPacket(new EnterGameOut());

			// Ping thread used to keep our connection to the server alive
			pingThread = new Thread(() =>
			{
				Console.WriteLine("Ping thread started");
				while (client.Connected)
				{
					Thread.Sleep(7000);
					gameClient.SendPing();
				}
				Console.WriteLine("Ping thread ended");
			});

			pingThread.Start();

			EventHandler temp = OnEnterGame;
			if (temp != null)
			{
				OnEnterGame(this, new EventArgs());
			}
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

			gameClient.SendPacket(toServer);
			gameClient.SendPing();
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
				// TODO: All this duplicate message spam seen everywhere will be gone soon
				Console.WriteLine("Ohno, unhandled special packet with no size {0:X2}", decompressedData[0]);
				FireOnFailureEvent(FailureArgs.FailureTypes.UnknownPacketSize, String.Format("Unhandled special packet {0:X2}", decompressedData[0]));
				Disconnect();
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
		/// Forcefully disconnects from the game server.
		/// </summary>
		public void Disconnect()
		{
			if (client.Connected)
			{
				Console.WriteLine("Game server: Disconnect requested");
				IsDisconnecting = true;

				if (pingThread != null && pingThread.IsAlive)
				{
					Console.WriteLine("Game server: Terminating ping thread");
					pingThread.Abort();
				}

				client.Close();
			}
		}


		void gameClient_OnPlayerCountChanged(object sender, PlayerCountArgs e)
		{
			FireOnPlayerCountEvent(e);
		}

		/// <summary>
		/// Raises the on disconnect event. 
		/// </summary>
		private void FireOnDisconnectEvent()
		{
			EventHandler tempHandler = OnDisconnect;
			if (tempHandler != null)
			{
				tempHandler(this, new EventArgs());
			}
		}

		/// <summary>
		/// Raises the on failure event. 
		/// </summary>
		/// <param name="failureTypes">Type of failures.</param>
		/// <param name="message">The error message.</param>
		private void FireOnFailureEvent(FailureArgs.FailureTypes failureTypes, string message)
		{
			EventHandler<FailureArgs> tempHandler = OnFailure;
			if (tempHandler != null)
			{
				tempHandler(this, new FailureArgs(failureTypes, message));
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
	}
}
