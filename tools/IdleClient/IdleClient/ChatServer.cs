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

	class ChatServer
	{
		/// <summary> Raised when the client disconnects. </summary>
		public event EventHandler OnDisconnect;
		/// <summary> Raised when a failure occurs. This is most likely nonrecoverable. </summary>
		public event EventHandler<FailureArgs> OnFailure;
		/// <summary> Raised when ready to connect to Realm Server. Contains argumentss for RealmServer connection. </summary>
		public event EventHandler<RealmServerArgs> ReadyToConnectToRealmServer;

		/// <summary>
		/// Client has been marked as disconnecting. Errors from loss of connection should be silenced
		/// until main loop dies.
		/// </summary>
		public bool IsDisconnecting { get; protected set; }

		/// <summary>
		/// Client has failed and requested to disconnect. Failure event will be rasied after main loop ends
		/// instead of disconnect event.
		/// </summary>
		public bool HasFailed { get; protected set; }

		/// <summary> The failure arguments if a failure has occured</summary>
		private FailureArgs failureArgs;

		private TcpClient client = new TcpClient();
		private Config settings;
		private uint clientToken = 0;
		private uint serverToken = 0;
		private string characterName;

		public ChatServer(Config settings, string characterName)
		{
			this.settings = settings;
			this.characterName = characterName;
		}

		//C > S [0x50] SID_AUTH_INFO
		//S > C [0x25] SID_PING
		//S > C [0x50] SID_AUTH_INFO
		//C > S [0x51] SID_AUTH_CHECK
		//S > C [0x51] SID_AUTH_CHECK
		//C > S [0x3A] SID_LOGONRESPONSE2
		//S > C [0x3A] SID_LOGONRESPONSE2
		//C > S [0x0A] SID_ENTERCHAT		<--
		//S > C [0x0A] SID_ENTERCHAT
		// http://www.bnetdocs.org/old/content9d2c.html?Section=d&id=6
		public void Run()
		{
			Console.WriteLine("Connecting to chat server " + settings.Address + ":" + settings.Port);

			try
			{
				client = new TcpClient(settings.Address, settings.Port);
			}
			catch (SocketException ex)
			{
				Fail(FailureArgs.FailureTypes.UnableToConnect, "Failed to connect to chat server: " + ex.Message);
				FireOnFailureEvent(failureArgs.Type, failureArgs.Message);
				return;
			}

			using (client)
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
					catch (IOException ex)
					{
						if (!IsDisconnecting)
						{
							Fail(FailureArgs.FailureTypes.FailedToReceive, "Failed to receive chat packet: " + ex.Message);
						}
						break;
					}

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
							Console.WriteLine("Unhandled Chat server packet");
							Console.WriteLine(packet);
							break;
					}
				}
			}

			Console.WriteLine("Chat server: Disconnected");
			if (HasFailed)
			{
				FireOnFailureEvent(failureArgs.Type, failureArgs.Message);
			}
			else
			{
				FireOnDisconnectEvent();
			}
		}

		/// <summary>Handles the LogonResponse2 packet. Responds by sending a QueryRealms2 Packet </summary>
		/// <param name="packet">The packet.</param>
		private void OnLogonResponse2(ChatServerPacket packet)
		{
			LogonResponse2In fromServer = new LogonResponse2In(packet);
			Console.WriteLine(fromServer.ToString());

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
			Console.WriteLine(fromServer.ToString());

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
			Console.WriteLine(fromServer.ToString());

			if (!fromServer.IsSuccessful())
			{
				Fail(FailureArgs.FailureTypes.RealmLoginFailed, fromServer.ToString());
				return;
			}

			EventHandler<RealmServerArgs> tempHandler = ReadyToConnectToRealmServer;
			if (tempHandler != null)
			{
				tempHandler(this, new RealmServerArgs(fromServer));
			}
		}

		/// <summary>Handles the AuthCheck packet. Responds with LogonRequest2 packet</summary>
		/// <param name="packet">The packet.</param>
		private void OnAuthCheck(ChatServerPacket packet)
		{
			AuthCheckIn fromServer = new AuthCheckIn(packet);
			Console.WriteLine(fromServer.ToString());

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
			Console.WriteLine(fromServer.ToString());

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
					Util.Receive(ns, ref buffer);
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
		/// Forcefully disconnects from the realm server.
		/// </summary>
		public void Disconnect()
		{
			IsDisconnecting = true;

			if (client.Connected)
			{
				Console.WriteLine("Chat server: Disconnect requested");
				client.Close();
			}
		}

		/// <summary>
		/// Handles failure and disconnects.
		/// </summary>
		/// <param name="failureType">Type of the failure.</param>
		/// <param name="message">The error message.</param>
		public void Fail(FailureArgs.FailureTypes failureType, string message)
		{
			if (HasFailed)
			{
				failureArgs.Message += ". " + message;
			}
			else
			{
				failureArgs = new FailureArgs(failureType, message);
			}
			HasFailed = true;
			IsDisconnecting = true;
			Disconnect();
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
	}

}
