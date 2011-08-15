using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Net;
using System.Globalization;

namespace IdleClient.Realm
{
	/// <summary>
	/// Values that represent realm server packet IDs.
	/// </summary>
	enum RealmServerPacketType
	{
		STARTUP = 0x01,
		CHARCREATE = 0x02,
		CREATEGAME = 0x03,
		JOINGAME = 0x04,
		GAMELIST = 0x05,
		GAMEINFO = 0x06,
		CHARLOGON = 0x07,
		CHARDELETE = 0x0A,
		REQUESTLADDERDATA = 0x11,
		MOTD = 0x12,
		CANCELGAMECREATE = 0x13,
		CREATEQUEUE = 0x14,
		CHARLIST = 0x17,
		CHARUPGRADE = 0x18,
		CHARLIST2 = 0x19
	}

	/// <summary>
	/// Realm server packet.
	/// </summary>
	struct RealmServerPacket
	{
		public ushort Length;
		public RealmServerPacketType Id;
		public byte[] Data;

		public override string ToString()
		{
			return String.Format("{0} [{1:X2}]", Id.ToString(), (byte)Id);
		}

		public byte[] GetBytes()
		{
			byte[] packetBytes = new byte[Length];

			MemoryStream ms = new MemoryStream(packetBytes);
			BinaryWriter bw = new BinaryWriter(ms);
			bw.Write(Length);
			bw.Write((byte)Id);
			bw.Write(Data);

			return packetBytes;
		}
	}

	/// <summary>
	/// Request to login to realm server
	/// </summary>
	struct StartupOut : IOutPacket
	{
		public uint MCPCookie;
		public uint MCPStatus;
		public byte[] MCPChunk1;
		public byte[] MCPChunk2;
		public string UniqueName;

		public StartupOut(Chat.RealmServerArgs data)
		{
			MCPCookie = data.MCPCookie;
			MCPStatus = data.MCPStatus;
			MCPChunk1 = data.MCPChunk1;
			MCPChunk2 = data.MCPChunk2;
			UniqueName = data.UniqueName;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(MCPCookie);
			bw.Write(MCPStatus);
			bw.Write(MCPChunk1);
			bw.Write(MCPChunk2);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(UniqueName));
			bw.Write((byte)0);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)RealmServerPacketType.STARTUP; }
		}
	}

	/// <summary>
	/// Response to inform us if our connection to the realm server was successfully.
	/// Contains a status that should be checked to see if request was accepted.
	/// </summary>
	struct StartupIn
	{
		public uint Result;

		public StartupIn(RealmServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			Result = br.ReadUInt32();
		}

		public override string ToString()
		{
			switch (Result)
			{
				case 0x00:
					return "Startup -> Success";
				case 0x0C:
					return "Startup -> No Battle.net connection detected";
				case 0x7F:
					return "Startup -> Temporary IP ban";
				default:
					return String.Format("Startup -> Unkown result {0:X2}", Result);
			}
		}

		/// <summary>
		/// Check if the result is successful. 
		/// </summary>
		/// <returns>true if successful, false if not.</returns>
		public bool IsSuccessful()
		{
			return Result == 0;
		}
	}

	/// <summary>
	/// Request a list of characters on this account.
	/// </summary>
	struct CharList2Out : IOutPacket
	{
		uint CharacterCount;

		public CharList2Out(uint count)
		{
			CharacterCount = count;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(CharacterCount);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)RealmServerPacketType.CHARLIST2; }
		}
	}

	/// <summary>
	/// Response containing list of characters on the account.
	/// </summary>
	struct CharList2In
	{
		public struct CharListData
		{
			public DateTime ExpirationDate;
			public string CharacterName;
			public string CharacterStatString;

			public override string ToString()
			{
				return String.Format("{0,-20} {1}", CharacterName, ExpirationDate);
			}
		}

		public ushort RequestedCharacters;
		public uint TotalCharacters;
		public ushort RetrievedCharacters;
		public CharListData[] charData;

		public CharList2In(RealmServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			RequestedCharacters = br.ReadUInt16();
			TotalCharacters = br.ReadUInt32();
			RetrievedCharacters = br.ReadUInt16();
			charData = new CharListData[RetrievedCharacters];

			for (int i = 0; i < RetrievedCharacters; i++)
			{
				uint timeStamp = br.ReadUInt32();

				charData[i].ExpirationDate = new DateTime(1970, 1, 1).AddSeconds(timeStamp);
				charData[i].CharacterName = Util.ReadSpecialString(br);
				charData[i].CharacterStatString = Util.ReadSpecialString(br);
			}
		}

		public bool CharacterExists(string name)
		{
			return charData.Any(n => n.CharacterName.ToLower() == name.ToLower());
		}

		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();

			sb.AppendLine("Characters on this account:");

			sb.AppendLine(String.Format("  {0,-20} {1}", "Name", "Expiration Date"));
			sb.AppendLine("  -----------------------------------------");
			foreach (var item in charData)
			{
				sb.AppendLine("  " + item.ToString());
			}

			return sb.ToString();
		}
	}

	/// <summary>
	/// Request to logon to realm with a specific character
	/// </summary>
	struct CharLogonOut : IOutPacket
	{
		public string CharacterName;

		public CharLogonOut(string name)
		{
			CharacterName = name;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(ASCIIEncoding.ASCII.GetBytes(CharacterName));
			bw.Write((byte)0);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)RealmServerPacketType.CHARLOGON; }
		}
	}

	/// <summary>
	/// Response to inform us if the character was logged in successfully. Contains a 
	/// status that should be checked to see if request was accepted.
	/// </summary>
	struct CharLogonIn
	{
		public uint Result;

		public CharLogonIn(RealmServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			Result = br.ReadUInt32();
		}

		public override string ToString()
		{
			switch (Result)
			{
				case 0x00:
					return "Character successfully logged in";
				case 0x46:
					return "CharLogon -> Player not found";
				case 0x7A:
					return "CharLogon -> Logon failed";
				case 0x7B:
					return "CharLogon -> Character expired";
				default:
					return String.Format("CharLogon -> Unkown result {0:X2}", Result);
			}
		}

		/// <summary>
		/// Check if the result is successful. 
		/// </summary>
		/// <returns>true if successful, false if not.</returns>
		public bool IsSuccessful()
		{
			return Result == 0x00;
		}
	}

	/// <summary>
	/// Requests a list of games to join
	/// </summary>
	struct GameListOut : IOutPacket
	{
		public ushort RequestID;
		public uint Unknown;
		public string SearchString;

		public GameListOut(string query)
		{
			RequestID = (ushort)new Random().Next(0, short.MaxValue);
			SearchString = query;
			Unknown = 0;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(RequestID);
			bw.Write(Unknown);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(SearchString));
			bw.Write((byte)0);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)RealmServerPacketType.GAMELIST; }
		}
	}

	/// <summary>
	/// Response containing info of a single game. This packet is sent once for every
	/// game in the server's game list. Contains a status that should be checked to see
	/// if the game is available for joining.
	/// </summary>
	struct GameListIn
	{
		public ushort RequestId;
		public uint Index;
		public byte PlayerCount;
		public uint Status;
		public string GameName;
		public string GameDescription;

		public GameListIn(RealmServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			RequestId = br.ReadUInt16();
			Index = br.ReadUInt32();
			PlayerCount = br.ReadByte();
			Status = br.ReadUInt32();
			GameName = Util.ReadSpecialString(br);
			GameDescription = Util.ReadSpecialString(br);
		}

		public override string ToString()
		{
			string statusString = "";

			switch (Status)
			{
				case 0x00300004:
					statusString = "Game is available to join";
					break;
				case 0xFFFFFFFF:
					statusString = "Server is down";
					break;
				default:
					statusString = String.Format("Unkown status {0:X2}", Status);
					break;
			}

			return String.Format("{0,-2}/8 {1,-16} {2,-32} {3}", PlayerCount, GameName, GameDescription, statusString);
		}

		/// <summary>
		/// Check if the result is successful. 
		/// </summary>
		/// <returns>true if successful, false if not.</returns>
		public bool IsSuccessful()
		{
			return Status == 0x00300004;
		}
	}

	/// <summary>
	/// Request to create a game. Game counter is suppose to start at 1 and increase for each
	/// game requested to create. This is untested.
	/// </summary>
	struct CreateGameOut : IOutPacket
	{
		public ushort RequestId;
		public uint Difficulty;
		public byte Unknown;
		public byte PlayerDifference;
		public byte MaximumPlayers;
		public string GameName;
		public string GamePassword;
		public string GameDescription;

		public CreateGameOut(string name, string pass, string description, DifficultyType difficulty, ushort gameCounter)
		{
			RequestId = (ushort)(gameCounter * 2);
			Difficulty = (uint)difficulty;
			Unknown = 0;
			PlayerDifference = 0xFF;
			MaximumPlayers = 8;
			GameName = name;
			GamePassword = pass;
			GameDescription = description;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(RequestId);
			bw.Write(Difficulty);
			bw.Write(Unknown);
			bw.Write(PlayerDifference);
			bw.Write(MaximumPlayers);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(GameName));
			bw.Write((byte)0);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(GamePassword));
			bw.Write((byte)0);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(GameDescription));
			bw.Write((byte)0);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)RealmServerPacketType.CREATEGAME; }
		}
	}

	/// <summary>
	/// Response containing token of game we attempted to create. Contains a status that should be
	/// checked to see if request was accepted.
	/// </summary>
	struct CreateGameIn
	{
		public ushort RequestId;
		public ushort GameToken;
		public ushort Unknown;
		public uint Result;

		public CreateGameIn(RealmServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			RequestId = br.ReadUInt16();
			GameToken = br.ReadUInt16();
			Unknown = br.ReadUInt16();
			Result = br.ReadUInt32();
		}

		public override string ToString()
		{
			switch (Result)
			{
				case 0x00:
					return "Game creation succeeded.";
				case 0x1E:
					return "CreateGame -> Invalid game name.";
				case 0x1F:
					return "CreateGame -> Game already exists.";
				case 0x20:
					return "CreateGame -> Game servers are down.";
				case 0x6E:
					return "CreateGame -> A dead hardcore character cannot create games.";
				default:
					return String.Format("CreateGame -> Unkown result {0:X2}", Result);
			}
		}

		/// <summary>
		/// Check if the result is successful. 
		/// </summary>
		/// <returns>true if successful, false if not.</returns>
		public bool IsSuccessful()
		{
			return Result == 0x00 || Result == 0x1F;
		}
	}

	/// <summary>
	/// Response from attempting to join a game, but server placing us in a queue
	/// </summary>
	struct CreateQueueIn
	{
		public uint Position;

		public CreateQueueIn(RealmServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			Position = br.ReadUInt32();
		}

		public override string ToString()
		{
			return "Your are in a queue: Position " + Position;
		}
	}

	/// <summary>
	/// Response containing game info if game exists. Contains a status that should be
	/// checked to see if request was accepted.
	/// </summary>
	struct GameInfoIn
	{
		public ushort RequestID;
		public uint Status;
		public uint GameUptime;
		public ushort Unknown;
		public byte MaximumPlayers;
		public byte PlayerCount;
		public CharacterClassType[] PlayerClasses;
		public byte[] PlayerLevels;
		public string Description;
		public string[] CharacterNames;

		public GameInfoIn(RealmServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			RequestID = br.ReadUInt16();
			Status = br.ReadUInt32();
			GameUptime = br.ReadUInt32();
			Unknown = br.ReadUInt16();
			MaximumPlayers = br.ReadByte();
			PlayerCount = br.ReadByte();

			PlayerClasses = new CharacterClassType[16];
			for (int i = 0; i < PlayerClasses.Length; i++)
			{
				PlayerClasses[i] = (CharacterClassType)br.ReadByte();
			}

			PlayerLevels = new byte[16];
			for (int i = 0; i < PlayerLevels.Length; i++)
			{
				PlayerLevels[i] = br.ReadByte();
			}

			Description = Util.ReadSpecialString(br);

			CharacterNames = new string[16];
			for (int i = 0; i < PlayerCount; i++)
			{
				CharacterNames[i] = Util.ReadSpecialString(br);
			}
		}

		public override string ToString()
		{
			if (Status != 0x00300004)
			{
				return String.Format("GameInfoIn -> Unkown result {0:X8}", Status);
			}

			StringBuilder sb = new StringBuilder();
			sb.AppendLine("Players: " + PlayerCount + " / " + MaximumPlayers);
			sb.AppendLine();
			sb.AppendLine(string.Format("{0,-20} {1,-6} {2}", "Names", "Level", "Class"));
			sb.AppendLine("---------------------------------------------------------");
			for (int i = 0; i < PlayerCount; i++)
			{
				sb.AppendLine(string.Format("{0,-20} {1,-3} {2}", CharacterNames[i], PlayerLevels[i], PlayerClasses[i]));
			}
			return sb.ToString();
		}

		public bool IsSuccessful()
		{
			return Status == 0x00300004;
		}
	}

	/// <summary>
	/// Request game info for a unique game name
	/// </summary>
	struct GameInfoOut : IOutPacket
	{
		public ushort RequestID;
		public string GameName;

		public GameInfoOut(string name)
		{
			RequestID = (ushort)new Random().Next(0, short.MaxValue);
			GameName = name;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(RequestID);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(GameName));
			bw.Write((byte)0);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)RealmServerPacketType.GAMEINFO; }
		}
	}

	/// <summary>
	/// Request to join a game
	/// </summary>
	struct JoinGameOut : IOutPacket
	{
		ushort RequestID;
		string GameName;
		string GamePassword;

		public JoinGameOut(string name, string pass)
		{
			RequestID = (ushort)new Random().Next(0, short.MaxValue);
			GameName = name;
			GamePassword = pass;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(RequestID);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(GameName));
			bw.Write((byte)0);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(GamePassword));
			bw.Write((byte)0);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)RealmServerPacketType.JOINGAME; }
		}
	}

	/// <summary>
	/// Response from the server after requesting to join a game. Contains a status that
	/// should be checked to see if request was accepted. Contains game server arguments
	/// that we must pass to the game server when connecting. 
	/// </summary>
	struct JoinGameIn
	{
		public ushort RequestID;
		public ushort GameToken;
		public ushort Unknown;
		public IPAddress GameServerIp;
		public uint GameHash;
		public uint Result;

		public JoinGameIn(RealmServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			RequestID = br.ReadUInt16();
			GameToken = br.ReadUInt16();
			Unknown = br.ReadUInt16();
			GameServerIp = new IPAddress(br.ReadBytes(4));
			GameHash = br.ReadUInt32();
			Result = br.ReadUInt32();
		}

		public override string ToString()
		{
			switch (Result)
			{
				case 0x00:
					return "Game joining succeeded. Game server IP = " + GameServerIp.ToString();
				case 0x29:
					return "JoinGameIn -> Password incorrect.";
				case 0x2A:
					return "JoinGameIn -> Game does not exist.";
				case 0x2B:
					return "JoinGameIn -> Game is full.";
				case 0x2C:
					return "JoinGameIn -> You do not meet the level requirements for this game.";
				case 0x6E:
					return "JoinGameIn -> A dead hardcore character cannot join a game.";
				case 0x71:
					return "JoinGameIn -> A non-hardcore character cannot join a game created by a Hardcore character.";
				case 0x73:
					return "JoinGameIn -> Unable to join a Nightmare game.";
				case 0x74:
					return "JoinGameIn -> Unable to join a Hell game.";
				case 0x78:
					return "JoinGameIn -> A non-expansion character cannot join a game created by an Expansion character.";
				case 0x79:
					return "JoinGameIn -> A Expansion character cannot join a game created by a non-expansion character.";
				case 0x7D:
					return "JoinGameIn -> A non-ladder character cannot join a game created by a Ladder character.";
				default:
					return String.Format("JoinGameIn -> Unkown result {0:X2}", Result);
			}
		}

		/// <summary>
		/// Check if the result is successful. 
		/// </summary>
		/// <returns>true if successful, false if not.</returns>
		public bool IsSuccessful()
		{
			return Result == 0;
		}
	}
}
