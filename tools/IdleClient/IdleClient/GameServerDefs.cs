using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace IdleClient.Game
{
	// GameServerInPacketType and GameServerOutPacketType enums taken from BlueVex

	/// <summary>
	/// Values that represent IDs of packets sent to the game server. 
	/// </summary>
	enum GameServerOutPacketType
	{
		WalkToLocation = 0x01,
		WalkToTarget = 0x02,
		RunToLocation = 0x03,
		RunToTarget = 0x04,
		CastLeftSkill = 0x05,
		CastLeftSkillOnTarget = 0x06,
		CastLeftSkillOnTargetStopped = 0x07,
		RecastLeftSkill = 0x08,
		RecastLeftSkillOnTarget = 0x09,
		RecastLeftSkillOnTargetStopped = 0x0A,
		CastRightSkill = 0x0C,
		CastRightSkillOnTarget = 0x0D,
		CastRightSkillOnTargetStopped = 0x0E,
		RecastRightSkill = 0xF,
		RecastRightSkillOnTarget = 0x10,
		RecastRightSkillOnTargetStopped = 0x11,
		UnitInteract = 0x13,
		SendOverheadMessage = 0x14,
		SendMessage = 0x15,
		PickItem = 0x16,
		DropItem = 0x17,
		DropItemToContainer = 0x18,
		PickItemFromContainer = 0x19,
		EquipItem = 0x1a,
		UnequipItem = 0x1c,
		SwapEquippedItem = 0x1d,
		SwapContainerItem = 0x1f,
		UseInventoryItem = 0x20,
		StackItems = 0x21,
		AddBeltItem = 0x23,
		RemoveBeltItem = 0x24,
		SwapBeltItem = 0x25,
		UseBeltItem = 0x26,
		IdentifyItem = 0x27,
		EmbedItem = 0x29,
		ItemToCube = 0x2a,
		TownFolkInteract = 0x2f,
		TownFolkCancelInteraction = 0x30,
		DisplayQuestMessage = 0x31,
		BuyItem = 0x32,
		SellItem = 0x33,
		CainIdentifyItems = 0x34,
		TownFolkRepair = 0x35,
		HireMercenary = 0x36,
		IdentifyGambleItem = 0x37,
		TownFolkMenuSelect = 0x38,
		IncrementAttribute = 0x3a,
		IncrementSkill = 0x3b,
		SelectSkill = 0x3C,
		HoverUnit = 0x3d,
		SendCharacterSpeech = 0x3f,
		RequestQuestLog = 0x40,
		Respawn = 0x41,
		WaypointInteract = 0x49,
		RequestReassign = 0x4b,
		ClickButton = 0x4f,
		DropGold = 0x50,
		SetSkillHotkey = 0x51,
		CloseQuest = 0x58,
		GoToTownFolk = 0x59,
		SetPlayerRelation = 0x5d,
		PartyRequest = 0x5e,
		UpdatePosition = 0x5f,
		SwitchWeapons = 0x60,
		ChangeMercEquipment = 0x61,
		ResurrectMerc = 0x62,
		InventoryItemToBelt = 0x63,
#if OLDVERSION
		GameLogonRequest = 0x67,
		ExitGame = 0x68,
		EnterGame = 0x6A,
		Ping = 0x6C,
#else
		WardenResponse = 0x66, 
		GameLogonRequest = 0x68, 
		ExitGame = 0x69,
		EnterGame = 0x6b,
		Ping = 0x6D, 
#endif
	}


	/// <summary>
	/// Values that represent IDs of packets from the game server. 
	/// </summary>
	enum GameServerInPacketType
	{
		GameLoading = 0x00,
		GameLogonReceipt = 0x01,
		GameLogonSuccess = 0x02,
		LoadAct = 0x03,
		LoadDone = 0x04,
		UnloadDone = 0x05,
		GameLogoutSuccess = 0x06,
		MapAdd = 0x07,
		MapRemove = 0x08,
		AssignWarp = 0x09,
		RemoveGroundUnit = 0x0A,
		GameHandshake = 0x0B,
		NPCGetHit = 0x0C,
		PlayerStop = 0x0D,
		SetGameObjectMode = 0x0E,
		PlayerMove = 0x0F,
		PlayerMoveToTarget = 0x10,
		ReportKill = 0x11,
		PlayerReassign = 0x15,
		SmallGoldAdd = 0x19,
		ByteToExperience = 0x1a,
		WordToExperience = 0x1b,
		DWordToExperience = 0x1c,
		AttributeByte = 0x1d,
		AttributeWord = 0x1E,
		AttributeDWord = 0x1f,
		PlayerAttributeNotification = 0x20,
		UpdateSkill = 0x21,
		UpdatePlayerItemSkill = 0x22,
		AssignSkill = 0x23,
		GameMessage = 0x26,
		NPCInfo = 0x27,
		UpdateQuestInfo = 0x28,
		UpdateGameQuestLog = 0x29,
		TransactionComplete = 0x2a,
		PlaySound = 0x2c,
		UpdateItemStats = 0x3e,
		UseStackableItem = 0x3f,
		PlayerClearCursor = 0x42,
		Relator1 = 0x47,
		Relator2 = 0x48,
		UnitUseSkillOnTarget = 0x4c,
		UnitUseSkill = 0x4d,
		MercForHire = 0x4e,
		MercForHireListStart = 0x4f,
		AssignGameObject = 0x51,
		UpdateQuestLog = 0x52,
		PartyRefresh = 0x53,
		AssignPlayer = 0x59,
		InformationMessage = 0x5A,
		PlayerInGame = 0x5b,
		PlayerLeaveGame = 0x5c,
		QuestItemState = 0x5d,
		PortalInfo = 0x60,
		OpenWaypoint = 0x63,
		PlayerKillCount = 0x65,
		NPCMove = 0x67,
		NPCMoveToTarget = 0x68,
		SetNPCMode = 0x69,
		NPCAction = 0x6b,
		MonsterAttack = 0x6c,
		NPCStop = 0x6d,
		PlayerCorpseVisible = 0x74,
		AboutPlayer = 0x75,
		PlayerInSight = 0x76,
		UpdateItemUI = 0x77,
		AcceptTrade = 0x78,
		GoldTrade = 0x79,
		SummonAction = 0x7a,
		AssignSkillHotkey = 0x7b,
		UseSpecialItem = 0x7c,
		SetItemState = 0x7d,
		PartyMemberUpdate = 0x7f,
		AssignMerc = 0x81,
		PortalOwnership = 0x82,
		NPCWantsInteract = 0x8a,
		PlayerPartyRelationship = 0x8b,
		PlayerRelationship = 0x8C,
		AssignPlayerToParty = 0x8d,
		AssignPlayerCorpse = 0x8e,
		Pong = 0x8f,
		PartyMemberPulse = 0x90,
		SkillsLog = 0x94,
		PlayerLifeManaChange = 0x95,
		WalkVerify = 0x96,
		SwitchWeaponSet = 0x97,
		ItemTriggerSkill = 0x99,
		WorldItemAction = 0x9c,
		OwnedItemAction = 0x9d,
		MercAttributeByte = 0x9e,
		MercAttributeWord = 0x9F,
		MercAttributeDWord = 0xA0,
		MercByteToExperience = 0xA1,
		MercWordToExperience = 0xA2,
		DelayedState = 0xA7,
		SetState = 0xA8,
		EndState = 0xA9,
		AddUnit = 0xAA,
		NPCHeal = 0xAB,
#if OLDVERSION
		RequestLogonInfo = 0xAE,
		AssignNPC = 0xAB,
		GameOver = 0xAF,
		Invalid = 0xB0,
		BadSave = 0xB3,
#else
		WardenCheck = 0xAE,
		RequestLogonInfo = 0xAF, // Based on version! 0xAE for 1.10 0xAF for 1.11+
		AssignNPC = 0xAC,
		GameOver = 0xB0,
		Invalid = 0xB1,
		BadSave = 0xB4,
#endif
	}


	/// <summary>
	/// Game server packet. 
	/// </summary>
	struct GameServerPacket
	{
		public GameServerInPacketType Id;
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
	/// Sent whenever the clients UI changes, such as when trading or opening inventory
	/// </summary>
	struct UpdateItemUIIn
	{
		public enum IdTypes
		{
			AskToTrade = 0x00,
			AskedToTrade = 0x01,
			PlayerAcceptsTrade = 0x05,
			TradeWindowOpens = 0x06,
			NoRoomForItems = 0x09,
			PersonHasNoRoom = 0x0a,
			ClosedTradeWindow = 0x0C,
			TradeSuccessful = 0x0D,
			RedCheck = 0x0E,
			NormalCheck = 0x0F,
			OpenStash = 0x10,
			OpenCube = 0x15,
		}

		public IdTypes Id;

		public UpdateItemUIIn(GameServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			Id = (IdTypes)br.ReadByte();
		}
	}

	/// <summary>
	/// Sent whenever a player talks or shrine causes an overhead message.
	/// </summary>
	struct GameMessageIn
	{
		public enum ChatTypes
		{
			ChatMessage = 0x01,
			Wisper = 0x02,
			ServerMessage = 0x04,
			OverHeadChat = 0x05,
			SentWisper = 0x06,
		}

		public ChatTypes ChatType;
		public byte UnknownA;
		public byte EntityType; // (if not over head, then 0x02?)
		public uint EntityID; // (if not over head, then 0x00)
		public byte UnknownB;
		public byte UnknownC; //(0x05 chat, 0x01 recv-wisp)
		public string CharacterName;
		public string Message;

		public GameMessageIn(GameServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			ChatType = (ChatTypes)br.ReadByte();
			UnknownA = br.ReadByte();
			EntityType = br.ReadByte();
			EntityID = br.ReadUInt32();
			UnknownB = br.ReadByte();
			UnknownC = br.ReadByte();
			CharacterName = Util.ReadSpecialString(br);
			Message = Util.ReadSpecialString(br); ;
		}

		public override string ToString()
		{
			return ChatType + " " + CharacterName + " " + Message;
		}
	}


	/// <summary>
	/// Sent as an update for various events such as players joining/leaving/being killed
	/// and server events
	/// </summary>
	struct InformationMessageIn
	{
		public enum InformationEvents
		{
			PlayerTimeout = 0x00,
			PlayerDropped = 0x01,
			PlayerJoined = 0x02,
			PlayerQuit = 0x03,
			PlayerNotInGame = 0x04,
			PlayerNotLoggedInToGame = 0x05,
			PlayerDied = 0x06,
			PlayerRelation = 0x07,
			PlayerBusy = 0x08,
			TradeWait = 0x09,
			ItemsInBox = 0x0A, // ???
			PlayerIgnoringYou = 0x0D,
			NotEnoughMana = 0x0E,
			RealmGoingDown = 0x0F,
			HostileWait = 0x10,
			ServerEventCount = 0x11,
			ServerEvent = 0x12,
		}

		public InformationEvents Event;
		public byte Action;
		public uint EntityID;
		public byte EntityType;
		public string First;
		public string Second;

		public InformationMessageIn(GameServerPacket packet)
		{
			BinaryReader br = new BinaryReader(new MemoryStream(packet.Data));
			Event = (InformationEvents)br.ReadByte();
			Action = br.ReadByte();
			EntityID = br.ReadUInt32();
			EntityType = br.ReadByte();

			First = Util.ReadAndUnpadString(br, 16);
			Second = Util.ReadAndUnpadString(br, 16);
		}

		public override string ToString()
		{
			switch (Event)
			{
				case InformationEvents.PlayerTimeout:
					return String.Format("{0} ({1}) Dropped Due To Timeout", First, Second);
				case InformationEvents.PlayerDropped:
					return String.Format("{0} ({1}) Dropped Due To Errors", First, Second);
				case InformationEvents.PlayerJoined:
					return String.Format("{0} ({1}) Joined The Game", First, Second);
				case InformationEvents.PlayerQuit:
					return String.Format("{0} ({1}) Quits The Game", First, Second);
				case InformationEvents.PlayerNotInGame:
					return String.Format("{0} Is Not In The Game", First);
				case InformationEvents.PlayerNotLoggedInToGame:
					return String.Format("{0} Is Not Logged In The Game", First);
				case InformationEvents.PlayerDied:
					return String.Format("{0} Was Slain", First);
				case InformationEvents.PlayerRelation:
					return String.Format("Player Relation");
				case InformationEvents.PlayerBusy:
					return String.Format("{0} Is Busy", First);
				case InformationEvents.TradeWait:
					return String.Format("Wait A Short Time Before Going Into Trade");
				case InformationEvents.ItemsInBox:
					return String.Format("Item's In Box");
				case InformationEvents.PlayerIgnoringYou:
					return String.Format("{0} Is Not Listening To You", First);
				case InformationEvents.NotEnoughMana:
					return String.Format("Not Enough Mana");
				case InformationEvents.RealmGoingDown:
					return String.Format("Realm Is Going Down In {0} Mins", EntityID);
				case InformationEvents.HostileWait:
					return String.Format("You must wait awhile before hostiling {0}", First);
				case InformationEvents.ServerEventCount:
					return String.Format("{0} Soj's Have Been Sold", EntityID);
				case InformationEvents.ServerEvent:
					return String.Format("Diablo Walks The Earth");
				default:
					return String.Format("InformationMessage -> Unkown event {0:X2}", Event);
			}
		}
	}
}
