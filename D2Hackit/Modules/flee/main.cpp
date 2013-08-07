#include "../../Includes/ClientCore.cpp"

#define CONFIG_FILE ".\\plugin\\flee.ini"

struct Packet_SC_PortalOwnership
{
	DWORD OwnerID;
	char OwnerName[16];
	DWORD LocalID;
	DWORD RemoteID;
};

struct Packet_CS_InteractWithEntity
{
	DWORD EntityKind;
	DWORD ID;
};

void RequestTP();
void ReadConfig();
void WriteConfig();
void CheckPlayerHPMana(WORD wNewHP, WORD wNewMana);
BOOL CALLBACK enumItemFindTP(DWORD dwItemID, LPARAM lParam);

DWORD portalId = 0;
bool fleePortalRequested = false;
bool portalRequested = false;
bool saveAndExitEnabled = false;
bool saveAndExitWhenNoTpEnabled = false;
bool hotkeyInTown = false;
int fleeAmount = 25;
BYTE portalKey = VK_BACK;
BYTE fleeKey = 0;
int settingPortalKey = false;
int settingFleeKey = false;

void ReadConfig()
{
	portalRequested = false;
	portalId = 0;

	//fastPortalEnabled = GetPrivateProfileInt("Flee", "FastPortal", TRUE, CONFIG_FILE);
	saveAndExitEnabled = GetPrivateProfileInt("Flee", "SaveAndExit", FALSE, CONFIG_FILE);
	saveAndExitWhenNoTpEnabled = GetPrivateProfileInt("Flee", "SaveAndExitWhenNoTp", FALSE, CONFIG_FILE);
	hotkeyInTown = GetPrivateProfileInt("Flee", "HotkeyInTown", FALSE, CONFIG_FILE);
	fleeAmount = GetPrivateProfileInt("Flee", "HpPercentage", 25, CONFIG_FILE);
	portalKey = GetPrivateProfileInt("Flee", "PortalKey", VK_BACK, CONFIG_FILE);
	fleeKey = GetPrivateProfileInt("Flee", "FleeKey", 0, CONFIG_FILE);

	server->GameStringf("ÿc5Fleeÿc0: Config loaded, %s when hp drops below %d%%", saveAndExitEnabled?"ÿc1exitÿc0":"ÿc2TPÿc0", fleeAmount);
}

void WriteConfig()
{
	char configBuff[64];

	//WritePrivateProfileString("Flee", "FastPortal", fastPortalEnabled?"1":"0", CONFIG_FILE);
	WritePrivateProfileString("Flee", "SaveAndExit", saveAndExitEnabled?"1":"0", CONFIG_FILE);
	WritePrivateProfileString("Flee", "SaveAndExitWhenNoTp", saveAndExitWhenNoTpEnabled?"1":"0", CONFIG_FILE);
	WritePrivateProfileString("Flee", "HotkeyInTown", hotkeyInTown?"1":"0", CONFIG_FILE);

	sprintf_s(configBuff, sizeof(configBuff)/sizeof(configBuff[0]), "%d", fleeAmount);
	WritePrivateProfileString("Flee", "HpPercentage", configBuff, CONFIG_FILE);

	sprintf_s(configBuff, sizeof(configBuff)/sizeof(configBuff[0]), "%d", portalKey);
	WritePrivateProfileString("Flee", "PortalKey", configBuff, CONFIG_FILE);

	sprintf_s(configBuff, sizeof(configBuff)/sizeof(configBuff[0]), "%d", fleeKey);
	WritePrivateProfileString("Flee", "FleeKey", configBuff, CONFIG_FILE);

	server->GameStringf("ÿc5Fleeÿc0: Config saved");
}

void RequestTP()
{
	DWORD townPortalItem = 0;

	if(me->GetSpellCharges(D2S_TOMEOFTOWNPORTAL) <= 0 && me->GetSpellCharges(D2S_SCROLLOFTOWNPORTAL) <= 0)
	{
		if(saveAndExitWhenNoTpEnabled)
		{
			server->GameSaveAndExit();
		}
		else
		{
			server->GameStringf("ÿc5Fleeÿc0: No more tp charges");
		}

		return;
	}

	me->EnumStorageItems(STORAGE_INVENTORY, (fnEnumItemProc)enumItemFindTP, (LPARAM)&townPortalItem);

	if(townPortalItem == 0)
	{
		server->GameStringf("ÿc5Fleeÿc0: No town portal items found");
		return;
	}

	portalId = townPortalItem;
	fleePortalRequested = true;
	me->DrinkInventoryPotion(townPortalItem);

	server->GameStringf("ÿc5Fleeÿc0: Opening portal... %d charges left", me->GetSpellCharges(D2S_TOMEOFTOWNPORTAL)-1);
}

void CheckPlayerHPMana(WORD wNewHP, WORD wNewMana)
{
	if(wNewHP < me->GetStat(STAT_HP))
	{
		int hpPercentage = (int)(((float)wNewHP/(float)me->GetStat(STAT_MAXHP))*100.0f);

		if(hpPercentage <= fleeAmount && !portalRequested)
		{
			if(saveAndExitEnabled)
			{
				server->GameSaveAndExit();
			}
			else
			{
				RequestTP();
			}
		}
	}
}

BOOL CALLBACK enumItemFindTP(DWORD dwItemID, LPARAM lParam)
{
	char itemCode[4];

	server->GetItemCode(*(DWORD *)dwItemID, itemCode, sizeof(itemCode)/sizeof(itemCode[0]));

	if(strcmp(itemCode, "tbk") == 0 || strcmp(itemCode, "tsc") == 0)
	{
		*(DWORD *)lParam = *(DWORD *)dwItemID;
		return FALSE;
	}

	return TRUE;
}
///


BOOL PRIVATE ToggleHotkeyInTown(char** argv, int argc)
{
	hotkeyInTown = !hotkeyInTown;

	if(hotkeyInTown)
	{
		server->GameStringf("ÿc5Fleeÿc0: Hotkeys ÿc2enabledÿc0 in town");
	}
	else
	{
		server->GameStringf("ÿc5Fleeÿc0: Hotkeys ÿc1disabledÿc0 in town");
	}

	return TRUE;
}

BOOL PRIVATE SetFleeKey(char** argv, int argc)
{
	settingFleeKey = true;

	server->GameStringf("ÿc5Fleeÿc0: Next key pressed will be the flee key...");

	return TRUE;
}

BOOL PRIVATE SetPortalKey(char** argv, int argc)
{
	settingPortalKey = true;

	server->GameStringf("ÿc5Fleeÿc0: Next key pressed will be the portal key...");

	return TRUE;
}

BOOL PRIVATE TP(char** argv, int argc)
{
	RequestTP();

	return TRUE;
}

BOOL PRIVATE HP(char** argv, int argc)
{
	if(argc != 3)
		return FALSE;

	fleeAmount = atoi(argv[2]);

	if(fleeAmount < 0)
		fleeAmount = 0;

	server->GameStringf("ÿc5Fleeÿc0: Fleeing when hp drops below %d%%", fleeAmount);

	return TRUE;
}

BOOL PRIVATE Mode(char** argv, int argc)
{
	saveAndExitEnabled = !saveAndExitEnabled;
	server->GameStringf("ÿc5Fleeÿc0: %s when hp drops below %d%%", saveAndExitEnabled?"ÿc1exitÿc0":"ÿc2TPÿc0", fleeAmount);

	return TRUE;
}

BOOL PRIVATE Save(char** argv, int argc)
{
	WriteConfig();

	return TRUE;
}

BOOL PRIVATE Load(char** argv, int argc)
{
	ReadConfig();

	return TRUE;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	switch(nMessage)
	{
		case PM_DEATH:
			portalRequested = false;
			fleePortalRequested = false;
			break;
		case PM_ENTERTOWN:
			portalRequested = false;
			fleePortalRequested = false;
			break;
	}
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	ReadConfig();
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if(fleePortalRequested && aPacket[0] == 0x20)
	{		
		fleePortalRequested = false;
		DWORD itemId = 0;
		memcpy(&itemId, aPacket+1, sizeof(DWORD));

		if(itemId == 0)
			return aLen;

		if(itemId == portalId)
		{
			portalRequested = true;
		}
	}

	return aLen;
}


DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{   
	if(aPacket[0] == 0x95)
	{
		int life = ((aPacket[2]&63)<<8)|aPacket[1];
		int mana = ((aPacket[4]&63)<<9)|((aPacket[3]<<1)+1);

		CheckPlayerHPMana(life, mana);
	}
	else if(portalRequested && aPacket[0] == 0x82)
	{
		Packet_SC_PortalOwnership portal;
		Packet_CS_InteractWithEntity interact;

		memcpy(&portal, aPacket+1, sizeof(Packet_SC_PortalOwnership));

		interact.EntityKind = 0x02;
		interact.ID = portal.LocalID;

		BYTE interactPacket[9];
		interactPacket[0] = 0x13;
		memcpy(interactPacket+1, &interact, sizeof(Packet_CS_InteractWithEntity));

		if(me->GetHPPercent() > 0)
		{
			server->GameSendPacketToServer(interactPacket, 9);
		}

		portalRequested = false;
	}
	else if(aPacket[0] == 0xa7)
	{
		// No more tp delay :)
		if(aPacket[6] == 102)
		{
			return 0;
		}
	}

	return aLen;
}

BYTE EXPORT OnGameKeyDown(BYTE iKeyCode)
{
	if(settingPortalKey)
	{
		settingPortalKey = false;

		if(iKeyCode == VK_ESCAPE)
		{
			server->GameStringf("ÿc5Fleeÿc0: Unbound portal key");
			portalKey = 0;
		}
		else
		{
			portalKey = iKeyCode;
			server->GameStringf("ÿc5Fleeÿc0: Set portal key to %d", iKeyCode);
		}
	}
	else if(settingFleeKey)
	{
		settingFleeKey = false;

		if(iKeyCode == VK_ESCAPE)
		{
			server->GameStringf("ÿc5Fleeÿc0: Unbound flee key");
			fleeKey = 0;
		}
		else
		{
			fleeKey = iKeyCode;
			server->GameStringf("ÿc5Fleeÿc0: Set flee key to %d", iKeyCode);
		}
	}
	else if(iKeyCode == portalKey && (hotkeyInTown || !me->IsInTown()))
	{
		RequestTP();
	}
	else if(iKeyCode == fleeKey && (hotkeyInTown || !me->IsInTown()))
	{
		server->GameSaveAndExit();
	}
	else
	{
		return iKeyCode;
	}

	return 0;
}

CLIENTINFO
(
	0,1,
	"",
	"",
	"",
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
		"tp",
		TP,
		"tp"
	},
	{
		"town",
		ToggleHotkeyInTown,
		"Toggles hotkey in town",
	},
	{
		"tpkey",
		SetPortalKey,
		"Sets the key for fast portal",
	},
	{
		"fleekey",
		SetFleeKey,
		"Sets the key for fast exit game",
	},
	{
		"mode",
		Mode,
		"Toggles flee mode"
	},
	{
		"hp",
		HP,
		"hp n"
	},
	{
		"reload",
		Load,
		"Reload config"
	},
	{
		"save",
		Save,
		"save config"
	},
	{NULL}
};
