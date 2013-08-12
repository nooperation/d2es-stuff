using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace IdleClient.Game
{
	/// <summary>
	/// Game client packet. 
	/// </summary>
	struct GameClientPacket
	{
		public GameServerOutPacketType Id;
		public byte[] Data;

		public override string ToString()
		{
			return String.Format("{0} [{1:X2}]", Id.ToString(), (byte)Id);
		}

		public byte[] GetBytes()
		{
			byte[] packetBytes = new byte[1 + Data.Length];

			MemoryStream ms = new MemoryStream(packetBytes);
			BinaryWriter bw = new BinaryWriter(ms);
			bw.Write((byte)Id);
			bw.Write(Data);

			return packetBytes;
		}
	}

	/// <summary>
	/// Ping the server
	/// </summary>
	struct PingOut : IOutPacket
	{
		public uint TickCount;
		public uint UnknownA;
		public uint UnknownB;

		public PingOut(uint tickCount)
		{
			TickCount = tickCount;
			UnknownA = 0;
			UnknownB = 0;
		}
		public PingOut(uint tickCount, uint dongs)
		{
			TickCount = tickCount;
			UnknownA = 0;
			UnknownB = dongs;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(TickCount);
			if ((byte)this.Id > 0x6c) // TODO: TEMP!
			{
				bw.Write(UnknownA);
			}
			bw.Write(UnknownB);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)GameServerOutPacketType.Ping; }
		}
	}

	/// <summary>
	/// Sends an overhead message
	/// </summary>
	struct SendOverheadMessageOut : IOutPacket
	{
		public string Message;

		public SendOverheadMessageOut(string message)
		{
			this.Message = message;

			if (this.Message.Length >= 255)
			{
				this.Message = Message.Substring(0, 254);
			}
		}

		#region IOutPacket Members

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write((short)0);
			Util.WriteString(bw, Message);
			bw.Write((short)0);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)GameServerOutPacketType.SendOverheadMessage; }
		}

		#endregion
	}

	/// <summary>
	/// Sends a chat message.
	/// </summary>
	struct SendMessageOut : IOutPacket
	{
		public ushort MessageType;
		public string Message;
		public string WhisperTarget;
		public string Unknown;


		/// <summary>
		/// Normal message
		/// </summary>
		/// <param name="message">The message.</param>
		public SendMessageOut(string message)
		{
			MessageType = 0x01;
			Message = message;
			WhisperTarget = "";
			Unknown = "";

			if (this.Message.Length >= 255)
			{
				this.Message = Message.Substring(0, 254);
			}
		}

		/// <summary>
		/// Whisper message
		/// </summary>
		/// <param name="message">The message.</param>
		/// <param name="whisperTo">The whisper to.</param>
		public SendMessageOut(string message, string whisperTo)
		{
			MessageType = 0x02;
			Message = message;
			WhisperTarget = whisperTo;
			Unknown = "";
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(MessageType);
			Util.WriteString(bw, Message);
			Util.WriteString(bw, WhisperTarget);
			Util.WriteString(bw, Unknown);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)GameServerOutPacketType.SendMessage; }
		}
	}


	/// <summary>
	/// Requests to enter the game we just joined.
	/// </summary>
	struct EnterGameOut : IOutPacket
	{
		public byte[] GetBytes()
		{
			return new byte[0];
		}

		public byte Id
		{
			get { return (byte)GameServerOutPacketType.EnterGame; }
		}
	}

	/// <summary>
	/// Requests to enter the game we just joined.
	/// </summary>
	struct ExitGameOut : IOutPacket
	{
		public byte[] GetBytes()
		{
			return new byte[0];
		}

		public byte Id
		{
			get { return (byte)GameServerOutPacketType.ExitGame; }
		}
	}

	/// <summary>
	/// Request to join a specific game.
	/// </summary>
	struct GameLogonRequestOut : IOutPacket
	{
		public uint ServerHash;
		public ushort ServerToken;
		public byte CharacterID;
		public uint VersionByte;
		public uint UnknownA;
		public uint UnknownB;
		public byte UnknownC;
		public byte[] CharacterName;

		public GameLogonRequestOut(uint serverHash, ushort serverToken, uint versionByte, byte characterID, byte[] characterName)
		{
			ServerHash = serverHash;
			ServerToken = serverToken;
			CharacterID = characterID;
			VersionByte = versionByte;
			UnknownA = 0xED5DCC50;
			UnknownB = 0x91A519B6;
			UnknownC = 0;
			CharacterName = characterName;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(ServerHash);
			bw.Write(ServerToken);
			bw.Write(CharacterID);
			bw.Write(VersionByte);
			if (VersionByte > 0x0a) // TODO: TEMP!
			{
				bw.Write(UnknownA);
				bw.Write(UnknownB);
			}
			bw.Write(UnknownC);
			bw.Write(CharacterName);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)GameServerOutPacketType.GameLogonRequest; }
		}
	}


	/// <summary>
	/// Requests to enter the game we just joined.
	/// </summary>
	struct PartyRequestOut : IOutPacket
	{
		public enum RequestType
		{
			Invite = 0x06,
			Cancel,
			Accept
		}

		public uint PlayerID;
		public RequestType Request;

		public PartyRequestOut(RequestType request, uint playerID)
		{
			Request = request;
			PlayerID = playerID;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write((byte)Request);
			bw.Write(PlayerID);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)GameServerOutPacketType.PartyRequest; }
		}
	}
}
