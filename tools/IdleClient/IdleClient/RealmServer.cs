using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.IO;

namespace IdleClient.Realm
{
	/// <summary>Arguments for game server. </summary>
	public class GameServerArgs : EventArgs
	{
		/// <summary>Gets or sets the game server address.</summary>
		public string Address { get; set; }

		/// <summary>Gets or sets the game server port.</summary>
		public int Port { get; set; }

		/// <summary>Gets or sets the game hash.</summary>
		public uint GameHash { get; set; }

		/// <summary>Gets or sets the game token.</summary>
		public ushort GameToken { get; set; }

		/// <summary>Gets or sets the character class.</summary>
		public byte CharacterClass { get; set; }

		/// <summary>Gets or sets the name of the character. Must be padded to 16 bytes.</summary>
		public byte[] CharacterName { get; set; }

		/// <summary>Gets or sets the number of players in the game being joining.</summary>
		public int PlayerCount { get; set; }

		/// <summary>
		/// Gets or sets the maximum players for the game being joined.
		/// </summary>
		public int MaxPlayers { get; set; }

		/// <summary>
		/// List of all the character names in game before joining
		/// </summary>
		public List<string> PlayerNames { get; set; }
	}

	/// <summary>
	/// Realm server connection manager.  
	/// </summary>
	class RealmServer : ClientBase
	{
		/// <summary> Rasied when ready to connect to game server. Contains arguments for GameServer connection.</summary>
		public event EventHandler<GameServerArgs> ReadyToConnectToGameServer;

		private int playerCount;
		private int maxPlayers;
		private List<string> playerNames;
		private Chat.RealmServerArgs realmServerArgs;

		/// <summary>
		/// Creates a new client.
		/// </summary>
		/// <param name="settings">Options for controlling the operation.</param>
		/// <param name="characterName">Name of the character.</param>
		public RealmServer(Config settings, string characterName)
			: base(settings, characterName)
		{
			ClientName = "RELM"; // Realm, it looks better when everything lines up
		}

		/// <summary>
		/// Initialises this object.
		/// </summary>
		/// <param name="args">The arguments to pass to this client.</param>
		protected override void Init(object args)
		{
			this.realmServerArgs = args as Chat.RealmServerArgs;

			this.address = realmServerArgs.Ip.ToString();
			this.port = realmServerArgs.Port;
		}

		/// <summary>
		/// The main loop for communicating with the server.
		/// </summary>
		protected override void MainLoop()
		{
			// Used to store the unprocessed packet data from ReceivePacket
			byte[] buffer = new byte[0];

			// When connecting to the realm server we must specify the protocol to use.
			NetworkStream ns = client.GetStream();
			ns.WriteByte(0x01);

			SendPacket(new StartupOut(realmServerArgs));

			while (client.Connected)
			{
				RealmServerPacket packet;

				try
				{
					packet = ReceivePacket(ref buffer);
				}
				catch (Exception ex)
				{
					if (!IsDisconnecting)
					{
						Fail(FailureArgs.FailureTypes.FailedToReceive, "Failed to receive realm server packet: " + ex.Message);
					}
					break;
				}

				if (settings.ShowPackets)
				{
					LogDebug("S -> C: " + packet);
				}
				if (settings.ShowPacketData)
				{
					LogDebug(Util.GetPacketDump(packet.GetBytes(), true));
				}

				switch (packet.Id)
				{
					case RealmServerPacketType.STARTUP:
						OnStartup(packet);
						break;
					case RealmServerPacketType.CREATEGAME:
						OnCreateGame(packet);
						break;
					case RealmServerPacketType.JOINGAME:
						OnJoinGame(packet);
						break;
					case RealmServerPacketType.GAMEINFO:
						OnGameInfo(packet);
						break;
					case RealmServerPacketType.CHARLOGON:
						OnCharLogon(packet);
						break;
					case RealmServerPacketType.CREATEQUEUE:
						OnCreateQueue(packet);
						break;
					case RealmServerPacketType.CHARLIST2:
						OnCharList2(packet);
						break;
					default:
						break;
				}
			}
		}

		/// <summary>
		/// Handles the JoinGameIn packet. This packet is sent in response to our request to join
		/// a game. If successful, we fire an event to notify the driver to start the game server.
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnJoinGame(RealmServerPacket packet)
		{
			JoinGameIn fromServer = new JoinGameIn(packet);
			LogServer(fromServer);

			if (!fromServer.IsSuccessful())
			{
				Fail(FailureArgs.FailureTypes.FailedToJoinGame, fromServer.ToString());
				return;
			}

			GameServerArgs args = new GameServerArgs();
			args.Address = fromServer.GameServerIp.ToString();
			args.Port = 4000;
			args.GameHash = fromServer.GameHash;
			args.GameToken = fromServer.GameToken;
			args.CharacterClass = (byte)CharacterClassType.Barbarian;
			args.PlayerCount = playerCount;
			args.MaxPlayers = maxPlayers;
			args.PlayerNames = playerNames;

			// Pad the character name to 16 bytes. This is required by the game server.
			byte[] charNameBytes = ASCIIEncoding.ASCII.GetBytes(characterName);
			Array.Resize(ref charNameBytes, 16);
			args.CharacterName = (byte[])charNameBytes.Clone();

			ReadyToConnectToGameServer(this, args);
		}

		/// <summary>
		/// Handles the GameInfoIn packet.
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnGameInfo(RealmServerPacket packet)
		{
			GameInfoIn fromServer = new GameInfoIn(packet);
			LogServer(fromServer);

			playerCount = fromServer.PlayerCount;
			maxPlayers = fromServer.MaximumPlayers;
			playerNames = new List<string>(fromServer.CharacterNames);
			playerNames.RemoveAll(n => String.IsNullOrEmpty(n));

			JoinGameOut toServer = new JoinGameOut(settings.GameName, settings.GamePass);
			SendPacket(RealmServerPacketType.JOINGAME, toServer.GetBytes());
		}

		/// <summary>
		/// Handles the CreateGameIn packet. This packet is sent in response to our CreateGameOut
		/// request. If successful, we will request the game info.
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnCreateGame(RealmServerPacket packet)
		{
			CreateGameIn fromServer = new CreateGameIn(packet);
			LogServer(fromServer);

			if (!fromServer.IsSuccessful())
			{
				Fail(FailureArgs.FailureTypes.FailedToCreateGame, fromServer.ToString());
				return;
			}

			GameInfoOut toServer = new GameInfoOut(settings.GameName);
			SendPacket(RealmServerPacketType.GAMEINFO, toServer.GetBytes());
		}

		/// <summary>
		/// Handles the CreateQueueIn packet. This packet is sent when you're forced to enter a
		/// join game queue. CreateQueueIn will be resent every now and then to update your position
		/// until your request is finally accepted, which will send us a JoinGameIn packet.
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnCreateQueue(RealmServerPacket packet)
		{
			CreateQueueIn fromServer = new CreateQueueIn(packet);
			LogServer(fromServer);
		}

		/// <summary>
		/// Handles the CharLogonIn packet. This packet is sent in response to a CharLogonOut
		/// packet. If this packet status is successful, we respond by requesting to create a game.
		/// </summary>
		/// <remarks>
		/// Requesting to create a game that already exists is correct because we're told if the
		/// game already exists or has been created successfully when the server responds with a
		/// CreateGameIn packet. From there we just query and join it.
		/// </remarks>
		/// <param name="packet">The packet.</param>
		private void OnCharLogon(RealmServerPacket packet)
		{
			CharLogonIn fromServer = new CharLogonIn(packet);
			LogServer(fromServer);

			if (!fromServer.IsSuccessful())
			{
				Fail(FailureArgs.FailureTypes.FailedToLoginToChat, fromServer.ToString());
				return;
			}

			CreateGameOut toServer = new CreateGameOut(settings.GameName, settings.GamePass, settings.GameDescription, settings.GameDifficulty, 1);
			SendPacket(RealmServerPacketType.CREATEGAME, toServer.GetBytes());
		}

		/// <summary>
		/// Handles the CharList2In packet. This packet contains a list of all the characters on the
		/// account. We respond by sending a CharLogonOut packet containing the name of the character
		/// to logon as.
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnCharList2(RealmServerPacket packet)
		{
			CharList2In fromServer = new CharList2In(packet);
			LogServer(fromServer);

			if (!fromServer.CharacterExists(characterName))
			{
				Fail(FailureArgs.FailureTypes.CharacterNotFound, "Character not found");
				return;
			}

			CharLogonOut toServer = new CharLogonOut(characterName);
			SendPacket(RealmServerPacketType.CHARLOGON, toServer.GetBytes());
		}

		/// <summary>
		/// Handles the StartupIn packet. This packet is sent in response to our StartupOut packet when
		/// we initially connected to the Realm Server. If successful, we request a list of characters
		/// on the account.
		/// </summary>
		/// <param name="packet">The packet.</param>
		private void OnStartup(RealmServerPacket packet)
		{
			StartupIn fromServer = new StartupIn(packet);
			LogServer(fromServer);

			if (!fromServer.IsSuccessful())
			{
				Fail(FailureArgs.FailureTypes.ServerDeniedConnection, fromServer.ToString());
				return;
			}

			CharList2Out toServer = new CharList2Out(8);
			SendPacket(RealmServerPacketType.CHARLIST2, toServer.GetBytes());
		}

		/// <summary>
		/// Sends a packet to the realm server. 
		/// </summary>
		/// <param name="packet">The packet.</param>
		public void SendPacket(IOutPacket packet)
		{
			SendPacket((RealmServerPacketType)packet.Id, packet.GetBytes());
		}

		/// <summary>
		/// Sends a packet to the realm server.
		/// </summary>
		/// <param name="type">The type of packet to send.</param>
		/// <param name="data">The packet data (Not the packet header).</param>
		public void SendPacket(RealmServerPacketType type, byte[] data)
		{
			RealmServerPacket packet;

			// Header length is 4 bytes, so total packet length is 4 + data length
			packet.Length = (ushort)(data.Length + 4);
			packet.Id = type;
			packet.Data = data;

			if (settings.ShowPackets)
			{
				LogDebug("C -> S: " + packet);
			}
			if (settings.ShowPacketData)
			{
				LogDebug(Util.GetPacketDump(packet.GetBytes(), false));
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
					Fail(FailureArgs.FailureTypes.FailedToSend, "Failed to send packet to realm server: " + ex.Message);
				}
				return;
			}
		}

		/// <summary>
		/// Receives a realm server packet. 
		/// </summary>
		/// <param name="buffer">
		/// [in,out] The buffer used to hold excess data retrieved from the server for use in future
		/// calls to ReceivePacket.
		/// </param>
		/// <returns>Realm server packet </returns>
		private RealmServerPacket ReceivePacket(ref byte[] buffer)
		{
			RealmServerPacket packet;
			bool needsMoreData = false;

			while (true)
			{
				if (buffer.Length == 0 || needsMoreData)
				{
					while (buffer.Length == 0)
					{
						Util.Receive(client.GetStream(), ref buffer);
					}
					needsMoreData = false;
				}

				// Needs enough data for header portion
				if (buffer.Length < 3)
				{
					needsMoreData = true;
					continue;
				}

				// Create the packet header since we've read enough bytes for it to exist
				BinaryReader br = new BinaryReader(new MemoryStream(buffer));
				packet.Length = br.ReadUInt16();
				packet.Id = (RealmServerPacketType)br.ReadByte();

				// We need more data for the packet
				if (packet.Length > buffer.Length)
				{
					needsMoreData = true;
					continue;
				}

				packet.Data = br.ReadBytes(packet.Length - 3);
				break;
			}

			// Remove the processed portion of the packet data from the buffer
			int newBufferLength = buffer.Length - packet.Length;
			Array.Copy(buffer, packet.Length, buffer, 0, newBufferLength);
			Array.Resize(ref buffer, newBufferLength);

			return packet;
		}
	}
}
