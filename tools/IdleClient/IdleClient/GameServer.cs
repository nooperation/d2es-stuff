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
		/// <summary> Event queue for all listeners interested in OnDisconnect events. </summary>
		public event EventHandler OnDisconnect;
		public event EventHandler OnEnterGame;

		public bool IsDisconnecting { get; protected set; }

		private GameClient gameClient;
		private Config settings;
		private Thread pingThread;
		private TcpClient client = new TcpClient();
		private Realm.GameServerArgs gameServerArgs;
		private string characterName;

#if OLDVERSION
		/* D2Net.dll (1.10 ES) StartOffset: 00008148, EndOffset: 000085D7, Length: 00000490 */
		byte[] gameServerPacketSizes = 
		{
			0x01, 0x08, 0x01, 0x0C, 0x01, 0x01, 0x01, 0x06, 0x06, 0x0B, 0x06, 0x06, 0x09, 0x0D, 0x0C, 0x10,
			0x10, 0x08, 0x1A, 0x0E, 0x12, 0x0B, 0xFF, 0x00, 0x0F, 0x02, 0x02, 0x03, 0x05, 0x03, 0x04, 0x06,
			0x0A, 0x0C, 0x0C, 0x0D, 0x5A, 0x5A, 0xFF, 0x28, 0x67, 0x61, 0x0F, 0x00, 0x08, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x08,
			0x0D, 0x00, 0x06, 0x00, 0x00, 0x0D, 0x00, 0x0B, 0x0B, 0x00, 0x00, 0x00, 0x10, 0x11, 0x07, 0x01,
			0x0F, 0x0E, 0x2A, 0x0A, 0x03, 0x00, 0x00, 0x0E, 0x07, 0x1A, 0x28, 0xFF, 0x05, 0x06, 0x26, 0x05,
			0x07, 0x02, 0x07, 0x15, 0x00, 0x07, 0x07, 0x10, 0x15, 0x0C, 0x0C, 0x10, 0x10, 0x0A, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x20, 0x0A, 0x0D, 0x06, 0x02, 0x15, 0x06, 0x0D, 0x08, 0x06, 0x12, 0x05, 0x0A,
			0x00, 0x14, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x06, 0x0B, 0x07, 0x0A, 0x21,
			0x0D, 0x1A, 0x06, 0x08, 0xFF, 0x0D, 0x09, 0x01, 0x07, 0x10, 0x11, 0x07, 0xFF, 0xFF, 0x07, 0x08,
			0x0A, 0x07, 0x08, 0x18, 0x03, 0x08, 0xFF, 0x07, 0xFF, 0x07, 0xFF, 0x07, 0xFF, 0x09, 0xFF, 0x01,
			0x00, 0x35, 0xFF, 0x05, 0x00, 
		};

		byte[] gameClientPacketSizes = 
		{
			0x05, 0x09, 0x05, 0x09, 0x05, 0x09, 0x09, 0x05, 0x09, 0x09, 0x01, 0x05, 0x09, 0x09, 0x05, 0x09,
			0x09, 0x01, 0x09, 0xFF, 0xFF, 0x0D, 0x05, 0x11, 0x05, 0x09, 0x09, 0x03, 0x09, 0x09, 0x11, 0x0D,
			0x09, 0x05, 0x09, 0x05, 0x09, 0x0D, 0x09, 0x09, 0x09, 0x09, 0x00, 0x00, 0x01, 0x03, 0x09, 0x09,
			0x09, 0x11, 0x11, 0x05, 0x11, 0x09, 0x05, 0x0D, 0x05, 0x03, 0x03, 0x09, 0x05, 0x05, 0x03, 0x01,
			0x01, 0x01, 0x01, 0x11, 0x09, 0x0D, 0x0D, 0x01, 0x09, 0x00, 0x09, 0x05, 0x03, 0x00, 0x07, 0x09,
			0x09, 0x05, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x11, 0x00, 0x00, 0x00, 0x07, 0x06, 0x05, 0x01,
			0x03, 0x05, 0x05, 0x09, 0x11, 0x2E, 0x1D, 0x01, 0x01, 0x01, 0xFF, 0x09, 0x01, 0x00, 0x01,
		};
#else
		/* D2Net.dll (1.13c) StartOffset: 0000A900, EndOffset: 0000ADDF, Length: 000004E0 */
		byte[] gameServerPacketSizes = 
		{
			0x01, 0x08, 0x01, 0x0C, 0x01, 0x01, 0x01, 0x06, 0x06, 0x0B, 0x06, 0x06, 0x09, 0x0D, 0x0C, 0x10,
			0x10, 0x08, 0x1A, 0x0E, 0x12, 0x0B, 0xFF, 0x00, 0x0F, 0x02, 0x02, 0x03, 0x05, 0x03, 0x04, 0x06,
			0x0A, 0x0C, 0x0C, 0x0D, 0x5A, 0x5A, 0xFF, 0x28, 0x67, 0x61, 0x0F, 0x00, 0x08, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x08,
			0x0D, 0x00, 0x06, 0x00, 0x00, 0x0D, 0x00, 0x0B, 0x0B, 0x00, 0x00, 0x00, 0x10, 0x11, 0x07, 0x01,
			0x0F, 0x0E, 0x2A, 0x0A, 0x03, 0x00, 0x00, 0x0E, 0x07, 0x1A, 0x28, 0xFF, 0x05, 0x06, 0x26, 0x05,
			0x07, 0x02, 0x07, 0x15, 0x00, 0x07, 0x07, 0x10, 0x15, 0x0C, 0x0C, 0x10, 0x10, 0x0A, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x20, 0x0A, 0x0D, 0x06, 0x02, 0x15, 0x06, 0x0D, 0x08, 0x06, 0x12, 0x05, 0x0A,
			0x00, 0x14, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x06, 0x0B, 0x07, 0x0A, 0x21,
			0x0D, 0x1A, 0x06, 0x08, 0xFF, 0x0D, 0x09, 0x01, 0x07, 0x10, 0x11, 0x07, 0xFF, 0xFF, 0x07, 0x08,
			0x0A, 0x07, 0x08, 0x18, 0x03, 0x08, 0xFF, 0x07, 0xFF, 0x07, 0xFF, 0x07, 0xFF, 0x00, 0xFF, 0xFF,
			0x01, 0x00, 0x35, 0xFF, 0x05, 0x00, 0x00,
		};

		byte[] gameClientPacketSizes = 
		{
			0x05, 0x09, 0x05, 0x09, 0x05, 0x09, 0x09, 0x05, 0x09, 0x09, 0x01, 0x05, 0x09, 0x09, 0x05, 0x09,
			0x09, 0x01, 0x09, 0xFF, 0xFF, 0x0D, 0x05, 0x11, 0x05, 0x09, 0x09, 0x03, 0x09, 0x09, 0x11, 0x0D,
			0x09, 0x05, 0x09, 0x05, 0x09, 0x0D, 0x09, 0x09, 0x09, 0x09, 0x00, 0x00, 0x01, 0x03, 0x09, 0x09,
			0x09, 0x11, 0x11, 0x05, 0x11, 0x09, 0x05, 0x0D, 0x05, 0x03, 0x03, 0x09, 0x05, 0x05, 0x03, 0x01,
			0x01, 0x01, 0x01, 0x11, 0x09, 0x0D, 0x0D, 0x01, 0x09, 0x00, 0x09, 0x05, 0x03, 0x00, 0x07, 0x09,
			0x09, 0x05, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x11, 0x00, 0x00, 0x00, 0x07, 0x06, 0x05, 0x01,
			0x03, 0x05, 0x05, 0x00, 0x00, 0xFF, 0x2E, 0x25, 0x01, 0x01, 0x01, 0xFF, 0x0D, 0x01, 0x00, 0x01,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		};
#endif
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
			Console.WriteLine("Connected to game server " + gameServerArgs.Address + ":" + gameServerArgs.Port);

			try
			{
				client = new TcpClient(gameServerArgs.Address, gameServerArgs.Port);
			}
			catch (SocketException ex)
			{
				Console.WriteLine("Failed to connect to game server: " + ex.Message);
				FireOnDisconnectEvent();
				return;
			}

			using (client)
			{
				gameClient = new GameClient(this, client, settings, gameServerArgs.PlayerCount, characterName);
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
					GameServerPacket packet = new GameServerPacket();
					int decompressedPacketSize = GetDecompressedPacketSize(decompressedData);

					if (decompressedPacketSize == 0xFF)
					{
						Console.WriteLine("Ohno, unhandled special packet with no size {0:X2}", decompressedData[0]);
						Disconnect();
						return packets;
					}

					packet.Id = (GameServerInPacketType)decompressedData[0];
					packet.Data = new byte[decompressedPacketSize-1];

					if (packet.Data.Length > 0)
					{
						Array.Copy(decompressedData, 1, packet.Data, 0, packet.Data.Length);
					}

					packets.Add(packet);

					Util.RemoveBeginningBytes(ref decompressedData, decompressedPacketSize);
				}

				return packets;
			}
		}

		/// <summary>
		/// Gets a chat packet size. 
		/// </summary>
		/// <remarks>From redvex</remarks>
		/// <param name="data">Bytes containing a chat packet data.</param>
		/// <returns>The chat packet size.</returns>
		private int GetChatPacketSize(byte[] data)
		{
			if (data.Length >= 12)
			{
				MemoryStream ms = new MemoryStream(data);
				BinaryReader br = new BinaryReader(ms);

				ms.Position = 10;
				string name = Util.ReadSpecialString(br);
				string message = Util.ReadSpecialString(br);

				return 10 + name.Length + 1 + message.Length + 1;
			}

			return -1;
		}

		/// <summary>
		/// Gets a decompressed packet size. 
		/// </summary>
		/// <remarks>From redvex</remarks>
		/// <param name="data">The data.</param>
		/// <returns>The decompressed packet size.</returns>
		private int GetDecompressedPacketSize(byte[] data)
		{
			byte packetId = data[0];

			switch (packetId)
			{
				case 0x26:
					return GetChatPacketSize(data);
				case 0x5b:
					return BitConverter.ToInt16(data, 1);
				case 0x94:
					if (data.Length >= 2)
					{
						return (int)data[1] * 3 + 6;
					}
					break;

				case 0xa8:
					if (data.Length >= 7)
					{
						return data[6];
					}
					break;

				case 0xaa:
					if (data.Length >= 7)
					{
						return data[6];
					}
					break;

				case 0xac:
					if (data.Length >= 13)
					{
						return data[12];
					}
					break;

				case 0x9c:
					if (data.Length >= 3)
					{
						return data[2];
					}
					break;

				case 0x9d:
					if (data.Length >= 3)
					{
						return data[2];
					}
					break;

#if OLDVERSION
				case (byte)GameServerInPacketType.RequestLogonInfo:
					return 2;
#else
				case (byte)GameServerInPacketType.WardenCheck:
					if (data.Length >= 3)
					{
						return 3 + BitConverter.ToInt16(data, 1);
					}
					break;
				case (byte)GameServerInPacketType.RequestLogonInfo:
					return 2;
#endif

				default:
					if (packetId < gameServerPacketSizes.Length)
					{
						return gameServerPacketSizes[packetId];
					}
					break;
			}

			return 0xFF;
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
	}
}
