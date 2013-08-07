#include <fstream>
#include "../../Includes/ClientCore.cpp"
#include "gambler.h"

Gambler gambler;
bool blockId = false;
bool goldDrop = false;
bool goldPick = false;
bool autostock = false;
char masterPlayer[128];

BOOL PRIVATE Start(char** argv, int argc)
{
	std::string itemCode;

	if(argc < 3)
	{
		return FALSE;
	}

	std::vector<std::string> gambleFor;

	for(int i = 2; i < argc; i++)
	{
		if(strlen(argv[i]) == 3)
		{
			gambleFor.push_back(argv[i]);
		}
		else
		{
			server->GameStringf("ÿc3Gambleÿc0: Invalid item code %s (must be 3 characters)", argv[i]);
			return FALSE;
		}
	}

	if(!gambler.Init(gambleFor))
	{
		return TRUE;
	}

	gambler.StartGambling();

	return TRUE;
}

BOOL PRIVATE NoSell(char **argv, int argc)
{
	std::string itemCode;
	std::vector<std::string> gambleFor;

	if(argc < 3)
	{
		gambler.SetNoSellList(gambleFor);
		return TRUE;
	}
	
	for(int i = 2; i < argc; i++)
	{
		if(strlen(argv[i]) == 3)
		{
			gambleFor.push_back(argv[i]);
		}
		else
		{
			server->GameStringf("ÿc3Gambleÿc0: Invalid item code %s (must be 3 characters)", argv[i]);
			return FALSE;
		}
	}

	gambler.SetNoSellList(gambleFor);
	return TRUE;
}

BOOL PRIVATE StartAmulets(char **argv, int argc)
{
	std::vector<std::string> jewellery;

	jewellery.push_back("amu"); //Amulet");
	jewellery.push_back("zam"); //Amazonian"); //Pin");
	jewellery.push_back("sam"); //Sorcerer's"); //Talisman");
	jewellery.push_back("nam"); //Death's"); //Locket");
	jewellery.push_back("pam"); //Holy"); //Pendant");
	jewellery.push_back("bam"); //Totemic"); //Pebble");
	jewellery.push_back("dam"); //Druidic"); //Necklace");
	jewellery.push_back("aam"); //Assassin's"); //Choker");

	if(!gambler.Init(jewellery))
	{
		return TRUE;
	}
	gambler.StartGambling();

	return TRUE;
}

BOOL PRIVATE StartJewellery(char** argv, int argc)
{
	std::vector<std::string> jewellery;

	jewellery.push_back("rin"); //Ring");
	jewellery.push_back("amu"); //Amulet");
	jewellery.push_back("zrn"); //Amazonian"); //Loop");
	jewellery.push_back("srn"); //Sorcerer's"); //Band");
	jewellery.push_back("nrn"); //Necromancer's"); //Stone");
	jewellery.push_back("prn"); //Paladic"); //Halo");
	jewellery.push_back("brg"); //Barbaric"); //Hoop");
	jewellery.push_back("drn"); //Druid's"); //Seal");
	jewellery.push_back("arn"); //Assassin's"); //Spiral");
	jewellery.push_back("zam"); //Amazonian"); //Pin");
	jewellery.push_back("sam"); //Sorcerer's"); //Talisman");
	jewellery.push_back("nam"); //Death's"); //Locket");
	jewellery.push_back("pam"); //Holy"); //Pendant");
	jewellery.push_back("bam"); //Totemic"); //Pebble");
	jewellery.push_back("dam"); //Druidic"); //Necklace");
	jewellery.push_back("aam"); //Assassin's"); //Choker");

	if(!gambler.Init(jewellery))
	{
		return TRUE;
	}
	gambler.StartGambling();

	return TRUE;
}

BOOL PRIVATE StartRings(char** argv, int argc)
{
	std::vector<std::string> jewellery;

	jewellery.push_back("rin"); //Ring");
	//jewellery.push_back("amu"); //Amulet");
	jewellery.push_back("zrn"); //Amazonian"); //Loop");
	jewellery.push_back("srn"); //Sorcerer's"); //Band");
	jewellery.push_back("nrn"); //Necromancer's"); //Stone");
	jewellery.push_back("prn"); //Paladic"); //Halo");
	jewellery.push_back("brg"); //Barbaric"); //Hoop");
	jewellery.push_back("drn"); //Druid's"); //Seal");
	jewellery.push_back("arn"); //Assassin's"); //Spiral");
	//jewellery.push_back("zam"); //Amazonian"); //Pin");
	//jewellery.push_back("sam"); //Sorcerer's"); //Talisman");
	//jewellery.push_back("nam"); //Death's"); //Locket");
	//jewellery.push_back("pam"); //Holy"); //Pendant");
	//jewellery.push_back("bam"); //Totemic"); //Pebble");
	//jewellery.push_back("dam"); //Druidic"); //Necklace");
	//jewellery.push_back("aam"); //Assassin's"); //Choker");

	if(!gambler.Init(jewellery))
	{
		return TRUE;
	}
	gambler.StartGambling();

	return TRUE;
}

BOOL PRIVATE Rings(char **argv, int argc)
{
	server->GamePrintString("rin Ring");
	server->GamePrintString("amu Amulet");
	server->GamePrintString("zrn Amazonian Loop");
	server->GamePrintString("srn Sorcerer's Band");
	server->GamePrintString("nrn Necromancer's Stone");
	server->GamePrintString("prn Paladic Halo");
	server->GamePrintString("brg Barbaric Hoop");
	server->GamePrintString("drn Druid's Seal");
	server->GamePrintString("arn Assassin's Spiral");
	server->GamePrintString("zam Amazonian Pin");
	server->GamePrintString("sam Sorcerer's Talisman");
	server->GamePrintString("nam Death's Locket");
	server->GamePrintString("pam Holy Pendant");
	server->GamePrintString("bam Totemic Pebble");
	server->GamePrintString("dam Druidic Necklace");
	server->GamePrintString("aam Assassin's Choker");

	return TRUE;
}

BOOL PRIVATE StartWraithShroud(char **argv, int argc)
{
	server->GameCommandLine("gamble sell magic rare set");
	server->GameCommandLine("gamble nosell yrb");
	server->GameCommandLine("gamble start rob");

	return TRUE;
}

BOOL PRIVATE StartExNihilo(char **argv, int argc)
{
	server->GameCommandLine("gamble sell magic rare set");
	server->GameCommandLine("gamble nosell amf");
	server->GameCommandLine("gamble start am5");

	return TRUE;
}

BOOL PRIVATE StartDeathwatchGuard(char **argv, int argc)
{
	server->GameCommandLine("gamble sell magic rare set");
	server->GameCommandLine("gamble nosell yft");
	server->GameCommandLine("gamble start fts");

	return TRUE;
}

BOOL PRIVATE StartPolarStar(char **argv, int argc)
{
	server->GameCommandLine("gamble sell magic rare set");
	server->GameCommandLine("gamble nosell ci3");
	server->GameCommandLine("gamble start ci0");

	return TRUE;
}

BOOL PRIVATE StartHeavyMetal(char **argv, int argc)
{
	server->GameCommandLine("gamble sell magic rare set");
	server->GameCommandLine("gamble nosell 0gi");
	server->GameCommandLine("gamble start gix");

	return TRUE;
}

BOOL PRIVATE StartRadiance(char **argv, int argc)
{
	server->GameCommandLine("gamble sell magic rare set");
	server->GameCommandLine("gamble nosell 0cr");
	server->GameCommandLine("gamble start crs");

	return TRUE;
}

BOOL PRIVATE StartFoc(char **argv, int argc)
{

	server->GameCommandLine("gamble sell magic rare set");
	server->GameCommandLine("gamble nosell yul");
	server->GameCommandLine("gamble start ful");

	return TRUE;
}

BOOL PRIVATE StartID(char **argv, int argc)
{
	server->GameCommandLine("gamble sell magic rare set");
	server->GameCommandLine("gamble nosell yul 0cr 0gi ci3 yft amf yrb");
	server->GameCommandLine("gamble start ful gix crs ci0 fts am5 rob");

	return TRUE;
}

BOOL PRIVATE SetMaster(char **argv, int argc)
{
	if(argc != 3 || strlen(argv[2]) == 0 || strlen(argv[2]) >= sizeof(masterPlayer)/sizeof(masterPlayer[0]))
	{
		server->GameStringf("No master set", masterPlayer);
		server->GameStringf("Gold dropping ÿc1disabledÿc0", masterPlayer);
		return FALSE;
	}

	strcpy_s(masterPlayer, argv[2]);

	goldDrop = true;
	server->GameStringf("Setting master to %s", masterPlayer);
	server->GameStringf("Gold dropping ÿc2enabledÿc0 for \"%s\"", masterPlayer);
	server->GameCommandLine("pick townpick 0");

	return TRUE;
}

BOOL PRIVATE ToggleRequestGold(char **argv, int argc)
{
	int requestSplitBy = 1;

	if(argc == 3)
	{
		requestSplitBy = atoi(argv[2]);

		if(requestSplitBy <= 0)
			requestSplitBy = 1;
	}

	gambler.ToggleRequestGold(requestSplitBy);

	server->GameCommandLine("pick townpick 1");
	server->GameCommandLine("pick radius 40");

	return TRUE;
}

BOOL PRIVATE ToggleGoldDrop(char **argv, int argc)
{
	goldDrop = !goldDrop;

	server->GameCommandLine("pick townpick 0");

	if(goldDrop)
		server->GameStringf("Gold dropping ÿc2enabledÿc0 for \"%s\"", masterPlayer);
	else
		server->GameStringf("Gold dropping ÿc1disabledÿc0 for \"%s\"", masterPlayer);

	return TRUE;
}

BOOL PRIVATE ToggleGambleSell(char **argv, int argc)
{
	bool uniques = false;
	bool rares = false;
	bool sets = false;
	bool magics = false;

	for(int i = 2; i < argc; i++)
	{
		if(_stricmp(argv[i], "magics") == 0 || _stricmp(argv[i], "magic") == 0)
		{
			magics = true;
		}
		if(_stricmp(argv[i], "sets") == 0 || _stricmp(argv[i], "set") == 0)
		{
			sets = true;
		}
		else if(_stricmp(argv[i], "rares") == 0 || _stricmp(argv[i], "rare") == 0)
		{
			rares = true;
		}
		else if(_stricmp(argv[i], "uniques") == 0 || _stricmp(argv[i], "unique") == 0)
		{
			uniques = true;
		}
	}

	gambler.ToggleGambleSell(magics, sets, rares, uniques);

	return TRUE;
}

BOOL PRIVATE Autostock(char **argv, int argc)
{
	bool uniques = false;
	bool rares = false;
	bool sets = false;

	if(argc < 2)
		return FALSE;

	for(int i = 2; i < argc; i++)
	{
		if(_stricmp(argv[i], "sets") == 0 || _stricmp(argv[i], "set") == 0)
		{
			sets = true;
		}
		else if(_stricmp(argv[i], "rares") == 0 || _stricmp(argv[i], "rare") == 0)
		{
			rares = true;
		}
		else if(_stricmp(argv[i], "uniques") == 0 || _stricmp(argv[i], "unique") == 0)
		{
			uniques = true;
		}
	}

	autostock = !autostock;
	gambler.ToggleAutostock(sets, rares, uniques);

	return TRUE;
}

BOOL PRIVATE ToggleIDBlock(char **argv, int argc)
{
	blockId = !blockId;

	if(blockId)
	{
		server->GamePrintString("Now blocking ID on gamble");
	}
	else
	{
		server->GamePrintString("No longer blocking ID on gamble");
	}

	return TRUE;
}

////////////////////////////////////////////
//
//               EXPORTS
//
/////////////////////////////////////////////


DWORD lastGoldAmount = 0;

DWORD EXPORT OnGameTimerTick()
{
	if(me->GetStat(STAT_GOLD) != lastGoldAmount)
	{
		gambler.OnGoldPickup();

		lastGoldAmount = me->GetStat(STAT_GOLD);
	}
	else
	{
		gambler.OnTick();
	}

	return 0;
}

BOOL EXPORT OnClientStart()
{
	memset(&masterPlayer, 0, sizeof(masterPlayer)/sizeof(masterPlayer[0]));
	DWORD lastGoldAmount = me->GetStat(STAT_GOLD);

	return TRUE;
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{

	if(aPacket[0] == 0x37)
	{
		//DWORD itemID = *((DWORD*)(aPacket+1));

		//gambler.OnItemIdentified(itemID);

		if(blockId)
		{
			return 0;
		}
	}
	else if(autostock && aPacket[0] == 0x15 && aPacket[1] == 0x01)
	{
		char *chatMessage = (char *)(aPacket+3);

		//server->GameStringf("Msg: %s", chatMessage);

		// Sneaky message passing, doesn't send autostocker chat messages out
		if(strncmp(chatMessage, "ÿc:Autostockerÿc0:", 18) == 0)
		{
			if(strcmp(chatMessage, "ÿc:Autostockerÿc0: Autostocker Ended") == 0)
			{
				if(!gambler.OnAutostockerEnded())
				{
					return aLen;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
	}

	return aLen;
}

/// <summary>
/// Process game packets that have already been processed by d2hackit and the game. Internal inventory structure
///   for d2hackit is up to date at this time.
/// </summary>
VOID EXPORT OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen)
{
	if(aPacket[0] == 0x2a) // npc transation
	{
		if(aPacket[2] == 0x01)
		{
			gambler.OnItemSold();
		}
	}
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{

	// 0x27 D2GS_NPCINTERACT

	//if((aPacket[0] < 0x67 || aPacket[0] > 0x6D) && aPacket[0] != 0x0A && aPacket[0] != 0x8f) 
	//{
	//	char buff[1024];
	//	memset(buff, 0, sizeof(buff));
	//	for(int i = 0; i < aLen; ++i)
	//	{
	//		sprintf(buff, "%s %02X", buff, aPacket[i]);
	//	}
	//	server->GameStringf("IN: %s", buff);
	//}
	if(aPacket[0] == 0x2c)
	{
		BYTE entityType = *((BYTE *)(aPacket+1));
		DWORD entityID = *((DWORD *)(aPacket+2));
		WORD speechID = *((WORD *)(aPacket+6));

		// Identify sound
		if(speechID == 6)
		{
			gambler.OnItemIdentifiedSounded();
		}
	}
	else if(aPacket[0] == 0x9c)
	{
		ITEM currentItem;

		if(!server->ParseItem(aPacket, aLen, currentItem))
		{
			return aLen;
		}

		if(currentItem.iGamble)
		{
			gambler.OnNpcGambleItemList(currentItem);
		}
		else if(currentItem.iAction == ITEM_ACTION_TO_STORAGE)
		{
			gambler.OnItemToStorage(currentItem);
		}
	}
	else if(aPacket[0] == 0x2a) // npc transation
	{
		if(aPacket[2] == 0x0C) // 0x0C = Not enough money, 0x00 = bought, 0x01 = sold
		{
			gambler.OnNotEnoughMoney();
		}
	}
	else if(goldDrop && aPacket[0] == 0x26 && aPacket[1] == 0x01)
	{
		char *chatName = (char *)(aPacket+10);
		char *chatMessage = NULL;

		if(_stricmp(masterPlayer, chatName) == 0)
		{
			chatMessage = (char *)(aPacket + 11 + strlen(chatName));

			DWORD goldAmount = atoi(chatMessage);

			if(goldAmount == 0)
				return aLen;

			if(goldAmount > me->GetStat(STAT_GOLD))
				goldAmount = me->GetStat(STAT_GOLD);

			if(goldAmount == 0)
			{
				me->Say("no gold left");
			}
			else
			{
				me->DropGold(goldAmount);
			}
		}
	}

	return aLen;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == PM_UICLOSED)
	{
		gambler.OnUIClosed();
	}
	if(nMessage == PM_NPCSESSION)
	{
		gambler.OnNpcSession(wParam);
	}
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	switch(iKeyCode)
	{
		case VK_SPACE:
			gambler.StopGambling();
			break;
	}

	return iKeyCode;
}


CLIENTINFO
(
	0,1,
	"",
	"",
	"Gamble",
	""
)

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	{
		"Start",
		Start,
		"Start Gambling"
	},
	{
		"StartRings",
		StartRings,
		"Starts gambling for rings"
	},
	{
		"StartAmulets",
		StartAmulets,
		"Starts gambling for amulets"
	},
	{
		"StartJewellery",
		StartJewellery,
		"Start gambling for rings and amulets",
	},
	{
		"StartFoc",
		StartFoc,
		"Starts gambling for FoC"
	},
	{
		"StartRadiance",
		StartRadiance,
		"Starts gambling for Radiance"
	},
	{
		"StartHeavyMetal",
		StartHeavyMetal,
		"Starts gambling for HeavyMetal"
	},
	{
		"StartPolarStar",
		StartPolarStar,
		"Starts gambling for PolarStar"
	},
	{
		"StartDeathwatchGuard",
		StartDeathwatchGuard,
		"Starts gambling for DeathwatchGuard"
	},
	{
		"StartExNihilo",
		StartExNihilo,
		"Starts gambling for ExNihilo"
	},
	{
		"StartWraithShroud",
		StartWraithShroud,
		"Starts gambling for WraithShroud"
	},
	//{
	//	"Startiddqd",
	//	StartID,
	//	"Starts gambling for all IDDQD's uniques"
	//},
	//{
	//	"Block",
	//	ToggleIDBlock,
	//	"Blocks the identify process when gambling"
	//},
	{
		"Sell",
		ToggleGambleSell,
		"Sells magic items that were gambled when inventory's full"
	},
	{
		"NoSell",
		NoSell,
		"Sets the list of items that should not be sold when gambling. Set ToggleSell to magic set rare and set NoSell to a specific item code to sell all items that are not unique version of the specified item code(s).",
	},
	//{
	//	"Gold",
	//	ToggleGoldDrop,
	//	"Drops gold when number is said in chat by [master]"
	//},
	{
		"Master",
		SetMaster,
		"Sets master for gold drop requests"
	},
	{
		"Request",
		ToggleRequestGold,
		"Usage: request [splitAmount]"
	},
	{
		"Autostock",
		Autostock,
		"Usage: autostock [sets] [rares] [uniques]"
	},
	{NULL}
};
