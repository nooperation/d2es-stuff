using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Net;
using System.Globalization;

namespace IdleClient.Chat
{
	/// <summary>
	/// Values that represent chat server packet IDs. 
	/// </summary>
	enum ChatServerPacketType
	{
		NULL = 0x00,
		STOPADV = 0x02,
		CLIENTID = 0x05,
		STARTVERSIONING = 0x06,
		REPORTVERSION = 0x07,
		STARTADVEX = 0x08,
		GETADVLISTEX = 0x09,
		ENTERCHAT = 0x0A,
		GETCHANNELLIST = 0x0B,
		JOINCHANNEL = 0x0C,
		CHATCOMMAND = 0x0E,
		CHATEVENT = 0x0F,
		LEAVECHAT = 0x10,
		LOCALEINFO = 0x12,
		FLOODDETECTED = 0x13,
		UDPPINGRESPONSE = 0x14,
		CHECKAD = 0x15,
		CLICKAD = 0x16,
		MESSAGEBOX = 0x19,
		STARTADVEX3 = 0x1C,
		LOGONCHALLENGEEX = 0x1D,
		CLIENTID2 = 0x1E,
		LEAVEGAME = 0x1F,
		DISPLAYAD = 0x21,
		NOTIFYJOIN = 0x22,
		PING = 0x25,
		READUSERDATA = 0x26,
		WRITEUSERDATA = 0x27,
		LOGONCHALLENGE = 0x28,
		LOGONRESPONSE = 0x29,
		CREATEACCOUNT = 0x2A,
		GAMERESULT = 0x2C,
		GETICONDATA = 0x2D,
		GETLADDERDATA = 0x2E,
		FINDLADDERUSER = 0x2F,
		CDKEY = 0x30,
		CHANGEPASSWORD = 0x31,
		CHECKDATAFILE = 0x32,
		GETFILETIME = 0x33,
		QUERYREALMS = 0x34,
		PROFILE = 0x35,
		CDKEY2 = 0x36,
		LOGONRESPONSE2 = 0x3A,
		CHECKDATAFILE2 = 0x3C,
		CREATEACCOUNT2 = 0x3D,
		LOGONREALMEX = 0x3E,
		STARTVERSIONING2 = 0x3F,
		QUERYREALMS2 = 0x40,
		QUERYADURL = 0x41,
		WARCRAFTGENERAL = 0x44,
		NETGAMEPORT = 0x45,
		NEWS_INFO = 0x46,
		OPTIONALWORK = 0x4A,
		EXTRAWORK = 0x4B,
		REQUIREDWORK = 0x4C,
		AUTH_INFO = 0x50,
		AUTH_CHECK = 0x51,
		AUTH_ACCOUNTCREATE = 0x52,
		AUTH_ACCOUNTLOGON = 0x53,
		AUTH_ACCOUNTLOGONPROOF = 0x54,
		AUTH_ACCOUNTCHANGE = 0x55,
		AUTH_ACCOUNTCHANGEPROOF = 0x56,
		AUTH_ACCOUNTUPGRADE = 0x57,
		AUTH_ACCOUNTUPGRADEPROOF = 0x58,
		SETEMAIL = 0x59,
		RESETPASSWORD = 0x5A,
		CHANGEEMAIL = 0x5B,
		SWITCHPRODUCT = 0x5C,
		GAMEPLAYERSEARCH = 0x60,
		FRIENDSLIST = 0x65,
		FRIENDSUPDATE = 0x66,
		FRIENDSADD = 0x67,
		FRIENDSREMOVE = 0x68,
		FRIENDSPOSITION = 0x69,
		CLANFINDCANDIDATES = 0x70,
		CLANINVITEMULTIPLE = 0x71,
		CLANCREATIONINVITATION = 0x72,
		CLANDISBAND = 0x73,
		CLANMAKECHIEFTAIN = 0x74,
		CLANINFO = 0x75,
		CLANQUITNOTIFY = 0x76,
		CLANINVITATION = 0x77,
		CLANREMOVEMEMBER = 0x78,
		CLANINVITATIONRESPONSE = 0x79,
		CLANRANKCHANGE = 0x7A,
		CLANSETMOTD = 0x7B,
		CLANMOTD = 0x7C,
		CLANMEMBERLIST = 0x7D,
		CLANMEMBERREMOVED = 0x7E,
		CLANMEMBERSTATUSCHANGE = 0x7F,
		CLANMEMBERRANKCHANGE = 0x81,
		CLANMEMBERINFORMATION = 0x82
	};


	/// <summary>
	/// Chat server packet. 
	/// </summary>
	struct ChatServerPacket
	{
		public byte Magic;
		public ChatServerPacketType Id;
		public ushort Length;
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
			bw.Write(Magic);
			bw.Write((byte)Id);
			bw.Write(Length);
			bw.Write(Data);

			return packetBytes;
		}
	}

	/// <summary>
	/// Ping packet
	/// </summary>
	struct PingOut : IOutPacket
	{
		public uint PingValue;

		public PingOut(uint pingValue)
		{
			PingValue = pingValue;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(PingValue);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)ChatServerPacketType.PING; }
		}
	}

	/// <summary>
	/// Coontains authentication information. Only thing we need from this is the server token
	/// </summary>
	struct AuthInfoIn
	{
		public uint LogonType;
		public uint ServerToken;
		public uint UDPValue;
		public uint FileTimeA;
		public uint FileTimeB;
		public string FileName;
		public string ValueString;

		public AuthInfoIn(ChatServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			LogonType = br.ReadUInt32();
			ServerToken = br.ReadUInt32();
			UDPValue = br.ReadUInt32();
			FileTimeA = br.ReadUInt32();
			FileTimeB = br.ReadUInt32();
			FileName = Util.ReadSpecialString(br);
			ValueString = Util.ReadSpecialString(br);
		}

		public override string ToString()
		{
			return string.Format("ServerToken = {0}\nFileName = {1}\nValueString = {2}", ServerToken, FileName, ValueString);
		}
	}

	/// <summary>
	/// Sends information about our game and keys to the server. Doesn't actually send keys since
	/// PVPGN doesn't support about them.
	/// </summary>
	struct AuthCheckOut : IOutPacket
	{
		public struct KeyInfo
		{
			public uint Length;
			public uint ProductValue;
			public uint PublicValue;
			public uint Unknown;
			public uint[] KeyHash;

			// Completely untested, should remove since we don't use keys
			public KeyInfo(string key)
			{
				Length = (uint)key.Length;
				ProductValue = 0;
				PublicValue = 0;
				Unknown = 0;
				KeyHash = new uint[] { 0, 0, 0, 0, 0 };
			}
		}

		public uint ClientToken;
		public uint ExeVersion;
		public uint ExeHash;
		public uint KeyCount;
		public bool IsSpawn;
		public KeyInfo[] keys;
		public string ExeString;
		public string KeyOwner;

		public AuthCheckOut(string owner)
		{
			// TODO: Move some of these values to config?
			ExeVersion = 0x01000a00;
			ExeHash = 0x210126ec;
			ClientToken = (uint)DateTime.Today.Ticks;
			KeyCount = 0;
			keys = null;
			IsSpawn = false;
			ExeString = "Game.exe 10/13/03 08:35:30 1198857";
			KeyOwner = owner;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(ClientToken);
			bw.Write(ExeVersion);
			bw.Write(ExeHash);
			bw.Write(KeyCount);
			bw.Write(IsSpawn);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(ExeString));
			bw.Write((byte)0);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(KeyOwner));
			bw.Write((byte)0);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)ChatServerPacketType.AUTH_CHECK; }
		}
	}

	/// <summary>
	/// A response to our authentication informion to see if it was accepted. Contains a status that
	/// should be checked to see if request was accepted.
	/// </summary>
	struct AuthCheckIn
	{
		public uint Result;
		public string Message;

		public AuthCheckIn(ChatServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			Result = br.ReadUInt32();
			Message = br.ReadString();
		}

		public override string ToString()
		{
			switch (Result)
			{
				case 0x000:
					return "AuthCheck -> Passed challenge";
				case 0x100:
					return "AuthCheck ->Old game version: " + Message;
				case 0x101:
					return "AuthCheck ->Invalid version";
				case 0x102:
					return "AuthCheck ->Game version must be downgraded to: " + Message;
				case 0x200:
					return "AuthCheck ->Invalid CD key";
				case 0x201:
					return "AuthCheck ->CD key in use by: " + Message;
				case 0x202:
					return "AuthCheck ->Banned key";
				case 0x203:
					return "AuthCheck ->Wrong product";
				default:
					return String.Format("AuthCheck ->Unkown result {0:X2}: {1}", Result, Message);
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
	/// Requests to logon to realm using specified account credentials.
	/// </summary>
	struct LogonRequest2Out : IOutPacket
	{
		public uint ClientToken;
		public uint ServerToken;
		public byte[] PasswordHash;
		public string Username;

		public LogonRequest2Out(string username, string password, uint clientToken, uint serverToken)
		{
			ClientToken = clientToken;
			ServerToken = serverToken;
			PasswordHash = MBNCSUtil.OldAuth.DoubleHashPassword(password, clientToken, serverToken);
			Username = username;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(ClientToken);
			bw.Write(ServerToken);
			bw.Write(PasswordHash);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(Username));
			bw.Write((byte)0);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)ChatServerPacketType.LOGONRESPONSE2; }
		}
	}

	/// <summary>
	/// Response to see if our logon credentials were accepted. Contains a status that should be
	/// checked to see if request was accepted.
	/// </summary>
	struct LogonResponse2In
	{
		public uint Result;
		public string Message;

		public LogonResponse2In(ChatServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			Result = br.ReadUInt32();
			if (packet.Data.Length > 4)
			{
				Message = br.ReadString();
			}
			else
			{
				Message = "";
			}
		}

		public override string ToString()
		{
			switch (Result)
			{
				case 0x00:
					return "LogonResponse2 ->Success";
				case 0x01:
					return "LogonResponse2 ->Account Does Not Exist";
				case 0x02:
					return "LogonResponse2 ->Invalid Password";
				case 0x06:
					return "LogonResponse2 ->Account Closed: " + Message;
				default:
					return String.Format("LogonResponse2 ->Unkown result {0:X2}: {1}", Result, Message);
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
	/// Contains a list of realm information
	/// </summary>
	struct QueryRealms2In
	{
		public struct RealmInfo
		{
			public uint Unknown;
			public string Title;
			public string Description;
		}
		public uint Unknown;
		public uint Count;
		public RealmInfo[] Realms;

		public QueryRealms2In(ChatServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			Unknown = br.ReadUInt32();
			Count = br.ReadUInt32();

			Realms = new RealmInfo[Count];
			for (int i = 0; i < Count; i++)
			{
				Realms[i].Unknown = br.ReadUInt32();
				Realms[i].Title = Util.ReadSpecialString(br);
				Realms[i].Description = Util.ReadSpecialString(br);
			}
		}

		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();

			sb.AppendLine("Total of " + Count + " realms found:");
			foreach (var item in Realms)
			{
				sb.AppendLine("  " + item.Title + " - " + item.Description);
			}

			return sb.ToString();
		}
	}

	/// <summary>
	/// Requests to logon to a realm server
	/// </summary>
	struct LogonRealmExOut : IOutPacket
	{
		public uint ClientToken;
		public byte[] RealmPasswordHash;
		public string RealmTitle;

		public LogonRealmExOut(uint clientToken, uint serverToken, string realmTitle)
		{
			this.ClientToken = clientToken;
			this.RealmTitle = realmTitle;
			RealmPasswordHash = MBNCSUtil.OldAuth.DoubleHashPassword("password", clientToken, serverToken);
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(ClientToken);
			bw.Write(RealmPasswordHash);
			bw.Write(ASCIIEncoding.ASCII.GetBytes(RealmTitle));
			bw.Write((byte)0);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)ChatServerPacketType.LOGONREALMEX; }
		}
	}

	/// <summary>
	/// Response containing arguments for connecting to realm server if request was accepted.
	/// Contains a status that should be checked to see if request was accepted.
	/// </summary>
	struct LogonRealmExIn
	{
		public uint MCPCookie;
		public uint MCPStatus;
		public byte[] MCPChunk1;
		public IPAddress Ip;
		public short Port;
		public byte[] MCPChunk2;
		public string UniqueName;

		public LogonRealmExIn(ChatServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			MCPCookie = br.ReadUInt32();
			MCPStatus = br.ReadUInt32();
			if (packet.Data.Length > 8)
			{
				MCPChunk1 = br.ReadBytes(2 * 4);
				Ip = new IPAddress(br.ReadBytes(4));
				Port = IPAddress.NetworkToHostOrder((short)br.ReadUInt32());
				MCPChunk2 = br.ReadBytes(12 * 4);
				UniqueName = Util.ReadSpecialString(br);

			}
			else
			{
				MCPChunk1 = null;
				Ip = null;
				Port = 0;
				MCPChunk2 = null;
				UniqueName = "";
			}
		}

		public override string ToString()
		{
			switch (MCPStatus)
			{
				case 0:
					return "Realm server address: " + Ip + ":" + Port;
				case 0x80000001:
					return "Realm is unavailable";
				case 0x80000002:
					return "Realm logon failed";
				default:
					return String.Format("Unkown result {0:X2}", MCPStatus);
			}
		}

		/// <summary>
		/// Check if the result is successful. 
		/// </summary>
		/// <returns>true if successful, false if not.</returns>
		public bool IsSuccessful()
		{
			return MCPStatus == 0;
		}
	}

	/// <summary>
	/// Auth info we need to send to the chat server when connecting.
	/// </summary>
	struct AuthInfoOut : IOutPacket
	{
		public uint ProtocolID;
		public uint PlatformID;
		public uint ProductID;
		public uint VersionByte;
		public uint ProductLanguage;
		public uint LocalIP;
		public uint TimeZoneBias;
		public uint LocaleID;
		public uint LanguageID;
		public string CountryAbreviation;
		public string Country;

		public AuthInfoOut(uint versionByte)
		{
			ProtocolID = 0;
			PlatformID = 0x49583836; // IX86
			ProductID = 0x44325850; // D2LoD
			VersionByte = versionByte; // 0x0c?
			ProductLanguage = 0;
			LocalIP = BitConverter.ToUInt32(IPAddress.Loopback.GetAddressBytes(), 0);
			TimeZoneBias = 0;
			LocaleID = (uint)CultureInfo.CurrentCulture.LCID;
			LanguageID = 0;
			CountryAbreviation = RegionInfo.CurrentRegion.ThreeLetterISORegionName;
			Country = RegionInfo.CurrentRegion.EnglishName;
		}

		public byte[] GetBytes()
		{
			MemoryStream ms = new MemoryStream();
			BinaryWriter bw = new BinaryWriter(ms);

			bw.Write(ProtocolID);
			bw.Write(PlatformID);
			bw.Write(ProductID);
			bw.Write(VersionByte);
			bw.Write(ProductLanguage);
			bw.Write(LocalIP);
			bw.Write(TimeZoneBias);
			bw.Write(LocaleID);
			bw.Write(LanguageID);
			Util.WriteString(bw, CountryAbreviation);
			Util.WriteString(bw, Country);

			return ms.ToArray();
		}

		public byte Id
		{
			get { return (byte)ChatServerPacketType.AUTH_INFO; }
		}
	}
}
