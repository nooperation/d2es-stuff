using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.IO;
using System.Net;

namespace IdleClient.Chat
{
	/// <summary>
	/// Arguments for the realm server. 
	/// </summary>
	public class RealmServerArgs : EventArgs
	{
		internal RealmServerArgs(LogonRealmExIn packet)
		{
			MCPCookie = packet.MCPCookie;
			MCPStatus = packet.MCPStatus;
			Ip = packet.Ip;
			Port = packet.Port;
			UniqueName = packet.UniqueName;

			MCPChunk1 = (byte[])packet.MCPChunk1.Clone();
			MCPChunk2 = (byte[])packet.MCPChunk2.Clone();
		}

		public uint MCPCookie { get; set; }
		public uint MCPStatus { get; set; }
		public byte[] MCPChunk1 { get; set; }
		public IPAddress Ip { get; set; }
		public short Port { get; set; }
		public byte[] MCPChunk2 { get; set; }
		public string UniqueName { get; set; }
	}

	class ChatServer : ClientBase
	{
		/// <summary> Raised when ready to connect to Realm Server. Contains argumentss for RealmServer connection. </summary>
		public event EventHandler<RealmServerArgs> ReadyToConnectToRealmServer;

		private uint clientToken = 0;
		private uint serverToken = 0;

		/// <summary>
		/// Creates a new client.
		/// </summary>
		/// <param name="settings">Options for controlling the operation.</param>
		/// <param name="characterName">Name of the character.</param>
		public ChatServer(Config settings, string characterName)
			: base(settings, characterName)
		{
			ClientName = "CHAT";
		}

		/// <summary>
		/// Initialises this object.
		/// </summary>
		/// <param name="args">The arguments to pass to this client.</param>
		protected override void Init(object args)
		{
			this.address = settings.Address;
			this.port = settings.Port;
		}

		/// <summary>
		/// The main loop for communicating with the server.
		/// </summary>
		protected override void MainLoop()
		{
			// Used to store the unprocessed packet data from ReceivePacket
			byte[] buffer = new byte[0];

			// When connecting to the server we must specify the protocol to use.
			NetworkStream ns = client.GetStream();
			ns.WriteByte(0x01);

			// Must also send auth info to the server after the protocol
			SendPacket(new AuthInfoOut(settings.ClientVersion));

			while (client.Connected)
			{
				ChatServerPacket packet;

				try
				{
					packet = ReceivePacket(ref buffer);
				}
				catch (Exception ex)
				{
					if (!IsDisconnecting)
					{
						Fail(FailureArgs.FailureTypes.FailedToReceive, "Failed to receive chat packet: " + ex.Message);
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
					case ChatServerPacketType.PING:
						OnPing(packet);
						break;
					case ChatServerPacketType.AUTH_INFO:
						OnAuthInfo(packet);
						break;
					case ChatServerPacketType.AUTH_CHECK:
						OnAuthCheck(packet);
						break;
					case ChatServerPacketType.LOGONRESPONSE2:
						OnLogonResponse2(packet);
						break;
					case ChatServerPacketType.QUERYREALMS2:
						OnQueryRealms2(packet);
						break;
					case ChatServerPacketType.LOGONREALMEX:
						OnLogonRealmEx(packet);
						break;
					default:
						LogServer("Unhandled Chat server packet");
						LogServer(packet.ToString());
						break;
				}
			}
		}

		/// <summary>Handles the LogonResponse2 packet. Responds by sending a QueryRealms2 Packet </summary>
		/// <param name="packet">The packet.</param>
		private void OnLogonResponse2(ChatServerPacket packet)
		{
			LogonResponse2In fromServer = new LogonResponse2In(packet);
			LogServer(fromServer.ToString());

			if (!fromServer.IsSuccessful())
			{
				Fail(FailureArgs.FailureTypes.LoginFailed, fromServer.ToString());
				return;
			}

			SendPacket(ChatServerPacketType.QUERYREALMS2, new byte[0]);
		}

		/// <summary>Handles the QueryRealms2 packet. Responds by sending LogonRealmEx packet.</summary>
		/// <param name="packet">The packet.</param>
		private void OnQueryRealms2(ChatServerPacket packet)
		{
			QueryRealms2In fromServer = new QueryRealms2In(packet);
			LogServer(fromServer.ToString());

			if (fromServer.Count == 0)
			{
				Fail(FailureArgs.FailureTypes.NoRealmsAvailable, "No realms available");
				return;
			}

			LogonRealmExOut toServer = new LogonRealmExOut(clientToken, serverToken, fromServer.Realms[0].Title);
			SendPacket(toServer);
		}

		/// <summary>Handles the LogonRealmEx packet. Signals manager to start up Realm Server handler</summary>
		/// <param name="packet">The packet.</param>
		private void OnLogonRealmEx(ChatServerPacket packet)
		{
			LogonRealmExIn fromServer = new LogonRealmExIn(packet);
			LogServer(fromServer.ToString());

			if (!fromServer.IsSuccessful())
			{
				Fail(FailureArgs.FailureTypes.RealmLoginFailed, fromServer.ToString());
				return;
			}

			FireReadyToConnectToRealmServerEvent(new RealmServerArgs(fromServer));
		}

		/// <summary>Handles the AuthCheck packet. Responds with LogonRequest2 packet</summary>
		/// <param name="packet">The packet.</param>
		private void OnAuthCheck(ChatServerPacket packet)
		{
			AuthCheckIn fromServer = new AuthCheckIn(packet);
			LogServer(fromServer.ToString());

			if (!fromServer.IsSuccessful())
			{
				Fail(FailureArgs.FailureTypes.FailedAuthCheck, fromServer.ToString());
				return;
			}

			LogonRequest2Out toServer = new LogonRequest2Out(settings.Username, settings.Password, clientToken, serverToken);
			SendPacket(toServer);
		}

		/// <summary>Handles the AuthInfo packet. Responds with AuthCheck packet</summary>
		/// <param name="packet">The packet.</param>
		private void OnAuthInfo(ChatServerPacket packet)
		{
			AuthInfoIn fromServer = new AuthInfoIn(packet);
			//LogServer(fromServer.ToString());

			AuthCheckOut toServer = new AuthCheckOut(settings.Owner);

			serverToken = fromServer.ServerToken;
			clientToken = toServer.ClientToken;

			SendPacket(toServer);
		}

		/// <summary>Handles the Ping packet. Responds with the same packet</summary>
		/// <param name="packet">The packet.</param>
		private void OnPing(ChatServerPacket packet)
		{
			SendPacket(ChatServerPacketType.PING, packet.Data);
		}

		/// <summary>
		/// Sends a packet to the chat server.
		/// </summary>
		/// <param name="packet">The packet.</param>
		public void SendPacket(IOutPacket packet)
		{
			SendPacket((ChatServerPacketType)packet.Id, packet.GetBytes());
		}

		/// <summary>
		/// Sends a packet to the chat server.
		/// </summary>
		/// <param name="type">The type of packet to send.</param>
		/// <param name="data">The packet data (Not the packet header).</param>
		private void SendPacket(ChatServerPacketType type, byte[] data)
		{
			ChatServerPacket packet;

			packet.Magic = 0xff;
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
					Fail(FailureArgs.FailureTypes.FailedToSend, "Failed to send packet to chat server: " + ex.Message);
					Disconnect();
				}
				return;
			}
		}

		/// <summary>
		/// Receives a chat server packet. 
		/// </summary>
		/// <param name="buffer">
		/// [in,out] The buffer used to hold excess data retrieved from the server for use in future
		/// calls to ReceivePacket.
		/// </param>
		/// <returns>Chat server packet </returns>
		private ChatServerPacket ReceivePacket(ref byte[] buffer)
		{
			NetworkStream ns = client.GetStream();
			ChatServerPacket packet;
			bool needsMoreData = false;
			packet.Magic = 0xff;

			while (true)
			{
				if (buffer.Length == 0 || needsMoreData)
				{
					while (buffer.Length == 0)
					{
						Util.Receive(ns, ref buffer);
					}
					needsMoreData = false;
				}

				// All chat server packets begin with 0xff
				if (buffer[0] != 0xff)
				{
					throw new ApplicationException("Invalid data received");
				}

				// Needs enough data for header portion
				if (buffer.Length < 4)
				{
					needsMoreData = true;
					continue;
				}

				// Packet header contains enough bytes for packet header, read it
				BinaryReader br = new BinaryReader(new MemoryStream(buffer));
				packet.Magic = br.ReadByte();
				packet.Id = (ChatServerPacketType)br.ReadByte();
				packet.Length = br.ReadUInt16();

				// Now that we have the packet length, check to make sure we have 
				//   enough bytes in the buffer to read the entire packet.
				if (packet.Length > buffer.Length)
				{
					needsMoreData = true;
					continue;
				}

				packet.Data = br.ReadBytes(packet.Length - 4);
				break;
			}

			// Remove the processed packet data from buffer
			int newBufferLength = buffer.Length - packet.Length;
			Array.Copy(buffer, packet.Length, buffer, 0, newBufferLength);
			Array.Resize(ref buffer, newBufferLength);

			return packet;
		}

		/// <summary>
		/// Asynchronously raises the ReadyToConnectToRealmServer event
		/// </summary>
		/// <param name="args"></param>
		private void FireReadyToConnectToRealmServerEvent(RealmServerArgs args)
		{
			EventHandler<RealmServerArgs> tempHandler = ReadyToConnectToRealmServer;
			if (tempHandler != null)
			{
				tempHandler.BeginInvoke(this, args, null, null);
			}
		}
	}

}
