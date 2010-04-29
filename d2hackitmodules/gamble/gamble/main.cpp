#include <fstream>
#include "../../d2hackit/includes/clientcore.cpp"
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
			server->GameStringf("ÿc3Gambleÿc0: Invalid item code %s", argv[i]);
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
	//jewellery.push_back("amu"); //Amulet");
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

BOOL PRIVATE UniqueArmor(char **argv, int argc)
{
	server->GamePrintString("eht Spirit Crown");
	server->GamePrintString("crn Crown");
	server->GamePrintString("msk Mask");
	server->GamePrintString("hgl Gauntlets");
	server->GamePrintString("hbt Greaves");
	server->GamePrintString("msb Muscle Wrap");
	server->GamePrintString("ne5 Demon Head");
	server->GamePrintString("pa4 Aerin Shield");
	server->GamePrintString("pa5 Crown Shield");

	return TRUE;
}

BOOL PRIVATE UniqueWeapons(char **argv, int argc)
{
	server->GamePrintString("wst War Staff");
	server->GamePrintString("dgr Dagger");
	server->GamePrintString("bld Blade");

	return TRUE;
}

BOOL PRIVATE SetMaster(char **argv, int argc)
{
	if(argc != 3 || strlen(argv[2]) == 0 || strlen(argv[2]) >= sizeof(masterPlayer)/sizeof(masterPlayer[0]))
	{
		return FALSE;
	}

	strcpy_s(masterPlayer, argv[2]);
	server->GameStringf("Setting master to %s", masterPlayer);

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
	gambler.ToggleGambleSell();

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

BOOL PRIVATE AutostockDelay(char **argv, int argc)
{
	int delay = 0;

	if(argc != 3)
		return FALSE;

	delay = atoi(argv[2]);
	if(delay < 0)
		delay = 0;

	gambler.SetAutostockStartDelay(delay);

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
	if(blockId && aPacket[0] == 0x37)
	{
		return 0;
	}
	else if(autostock && aPacket[0] == 0x15 && aPacket[1] == 0x01)
	{
		char *chatMessage = (char *)(aPacket+3);

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

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{
	if(aPacket[0] == 0x9c)
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
		else if(aPacket[2] == 0x01)
		{
			gambler.OnItemSold();
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

	/*else if((aPacket[0] < 0x67 || aPacket[0] > 0x6D) && aPacket[0] != 0x0A && aPacket[0] != 0x8f) 
	{
		server->GameStringf("%02X", aPacket[0]);
	}*/
	return aLen;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
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
		"Starts gambling for rings and amulets"
	},
	{
		"Rings",
		Rings,
		"Show ring codes"
	},
	{
		"Armors",
		UniqueArmor,
		"Show various armor codes"
	},
	{
		"Weapons",
		UniqueWeapons,
		"Show a few weapon codes"
	},
	{
		"Block",
		ToggleIDBlock,
		"Blocks the identify process when gambling"
	},
	{
		"Sell",
		ToggleGambleSell,
		"Sells magic items that were gambled when inventory's full"
	},
	{
		"Gold",
		ToggleGoldDrop,
		"Drops gold when number is said in chat by [master]"
	},
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
	{
		"AutostockDelay",
		AutostockDelay,
		"Sets a delay of N ticks before starting autostock. Each tick is 100ms, default 10"
	},
	{
		"Jewellery",
		StartJewellery,
		"Start gambling for Jewellery except for generic amulet",
	},
	{NULL}
};
