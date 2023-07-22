#include "../../Includes/ClientCore.cpp"
#include <vector>

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

void RequestTP(bool enterPortal);
void ReadConfig();
void WriteConfig();
void CheckPlayerHPMana(WORD wNewHP, WORD wNewMana);
BOOL CALLBACK enumItemFindTP(DWORD dwItemID, LPARAM lParam);

DWORD portalId = 0;
bool fleePortalRequested = false;
bool portalRequested = false;
bool hasSentInteractionPacket = false;
bool saveAndExitEnabled = false;
bool saveAndExitWhenNoTpEnabled = false;
bool hotkeyInTown = false;
bool createPortalWhenLeavingTown = false;
bool isSuppressingAutoTpWhenLeavingTown = false;
std::vector<int> waypointObjectIds;

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
	saveAndExitEnabled = GetPrivateProfileInt("Flee", "SaveAndExit", FALSE, CONFIG_FILE) == TRUE;
	saveAndExitWhenNoTpEnabled = GetPrivateProfileInt("Flee", "SaveAndExitWhenNoTp", FALSE, CONFIG_FILE) == TRUE;
	hotkeyInTown = GetPrivateProfileInt("Flee", "HotkeyInTown", FALSE, CONFIG_FILE) == TRUE;
	fleeAmount = GetPrivateProfileInt("Flee", "HpPercentage", 25, CONFIG_FILE);
	portalKey = GetPrivateProfileInt("Flee", "PortalKey", VK_BACK, CONFIG_FILE);
	fleeKey = GetPrivateProfileInt("Flee", "FleeKey", 0, CONFIG_FILE);
	createPortalWhenLeavingTown = GetPrivateProfileInt("Flee", "CreatePortalWhenLeavingTown", FALSE, CONFIG_FILE) == TRUE;
	hasSentInteractionPacket = false;

	server->GameStringf("ÿc5Fleeÿc0: Config loaded, %s when hp drops below %d%%", saveAndExitEnabled?"ÿc1exitÿc0":"ÿc2TPÿc0", fleeAmount);


	waypointObjectIds.clear();
	waypointObjectIds.push_back(119); // waypoint portal
	waypointObjectIds.push_back(145); // waypointi inner hell
	waypointObjectIds.push_back(156); // waypoint act 2
	waypointObjectIds.push_back(157); // waypoint act 1 wilderness
	waypointObjectIds.push_back(237); // act3waypoint town
	waypointObjectIds.push_back(238); // waypointh
	waypointObjectIds.push_back(288); // waypoint, celler
	waypointObjectIds.push_back(323); // waypoint act2 sewer
	waypointObjectIds.push_back(324); // waypoint act3 travincal
	waypointObjectIds.push_back(398); // waypoint pandamonia fortress
	waypointObjectIds.push_back(402); // waypoint valleywaypoint
	waypointObjectIds.push_back(429); // expansion no snow
	waypointObjectIds.push_back(494); // baals_waypoint
	waypointObjectIds.push_back(496); // wilderness_waypoint
	waypointObjectIds.push_back(511); // icecave 
	waypointObjectIds.push_back(539); // temple
}

void WriteConfig()
{
	char configBuff[64];

	//WritePrivateProfileString("Flee", "FastPortal", fastPortalEnabled?"1":"0", CONFIG_FILE);
	WritePrivateProfileString("Flee", "SaveAndExit", saveAndExitEnabled?"1":"0", CONFIG_FILE);
	WritePrivateProfileString("Flee", "SaveAndExitWhenNoTp", saveAndExitWhenNoTpEnabled?"1":"0", CONFIG_FILE);
	WritePrivateProfileString("Flee", "HotkeyInTown", hotkeyInTown?"1":"0", CONFIG_FILE);
	WritePrivateProfileString("Flee", "CreatePortalWhenLeavingTown", createPortalWhenLeavingTown?"1":"0", CONFIG_FILE);

	sprintf_s(configBuff, sizeof(configBuff)/sizeof(configBuff[0]), "%d", fleeAmount);
	WritePrivateProfileString("Flee", "HpPercentage", configBuff, CONFIG_FILE);

	sprintf_s(configBuff, sizeof(configBuff)/sizeof(configBuff[0]), "%d", portalKey);
	WritePrivateProfileString("Flee", "PortalKey", configBuff, CONFIG_FILE);

	sprintf_s(configBuff, sizeof(configBuff)/sizeof(configBuff[0]), "%d", fleeKey);
	WritePrivateProfileString("Flee", "FleeKey", configBuff, CONFIG_FILE);

	server->GameStringf("ÿc5Fleeÿc0: Config saved");
}

void RequestTP(bool enterPortal)
{
	if (me->IsInTown())
	{
		return;
	}

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

	if (enterPortal)
	{
		fleePortalRequested = true;
	}
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
				RequestTP(true);
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


/// <summary>
/// Checks to see if there is a waypoint object nearby.
/// </summary>
/// <param name="lpPresetUnit">Preset game unit, see /data/global/excel/Objects.txt for list of all objects.</param>
/// <param name="lParam">Pointer to a PRESETUNIT to be filled out if waypoint object is found.</param>
/// <returns>TRUE if waypoint was not found. FALSE if waypoint was found.</returns>
BOOL CALLBACK enumPresetsForWaypoints(LPCPRESETUNIT lpPresetUnit, LPARAM lParam)
{
	if(lpPresetUnit->dwType != UNIT_TYPE_OBJECT)
	{
		return TRUE;
	}

	if(std::find(waypointObjectIds.begin(), waypointObjectIds.end(), lpPresetUnit->dwID) == waypointObjectIds.end())
	{
		return TRUE;
	}

	memcpy((void *)lParam, lpPresetUnit, sizeof(PRESETUNIT));

	return FALSE;
}


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
	RequestTP(true);

	return TRUE;
}

BOOL PRIVATE SuppressAutoTp(char** argv, int argc)
{
	if(argc != 3)
	{
		server->GameStringf("ÿc5Fleeÿc0: Usage: SuppressAutoTp 0/1");
		return FALSE;
	}

	isSuppressingAutoTpWhenLeavingTown = atoi(argv[2]) != 0;

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
	hasSentInteractionPacket = false;

	return TRUE;
}

VOID EXPORT OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	switch(nMessage)
	{
		case PM_DEATH:
			hasSentInteractionPacket = false;
			portalRequested = false;
			fleePortalRequested = false;
			break;
		case PM_ENTERTOWN:
			hasSentInteractionPacket = false;
			portalRequested = false;
			fleePortalRequested = false;
			break;
		case PM_LEAVETOWN:
			// EnumPresetUnits can cause a crash so, enjoy your portals every time you leave town!
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
		const auto life = ((aPacket[1] & 0b11111111) >> 0) | ((aPacket[2] & 0b01111111) << 8);
		const auto mana = ((aPacket[2] & 0b10000000) >> 7) | ((aPacket[3] & 0b11111111) << 1) | ((aPacket[4] & 0b00111111) << 9);

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

		if(me->GetHPPercent() > 0 && (hotkeyInTown || !me->IsInTown()))
		{
			if (!hasSentInteractionPacket)
			{
				hasSentInteractionPacket = true;
				server->GameSendPacketToServer(interactPacket, 9);
			}
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
		RequestTP(true);
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
		"SuppressAutoTp",
		SuppressAutoTp,
		"Disabled auto-tp when leaving town [0/1]",
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
