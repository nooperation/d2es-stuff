#include <sstream>
#include <iostream>
#include "../../Includes/ClientCore.cpp"
#include "../../Includes/itemPrefix.h"
#include "../../Includes/itemSuffix.h"
#include "ItemWatcher.h"

#undef min
#undef max

BOOL PRIVATE Radius(char** argv, int argc);
BOOL PRIVATE GoldSpeed(char** argv, int argc);
BOOL PRIVATE MinGold(char** argv, int argc);
BOOL PRIVATE ToggleShowItemLevel(char** argv, int argc);
BOOL PRIVATE ReloadItemDefs(char** argv, int argc);

ItemWatcher itemWatcher;
bool enabled = true;
bool waitingForItemPickup = false;
bool waitingForItemPickupDebug = false;

BOOL PRIVATE Enable(char** argv, int argc)
{
	server->GameInfof("Pick Enabled");
	enabled = true;

	return TRUE;
}

BOOL PRIVATE Disable(char** argv, int argc)
{
	server->GameInfof("Pick Disabled");
	enabled = false;

	return TRUE;
}

BOOL PRIVATE Toggle(char** argv, int argc)
{
	if(enabled)
		return Disable(argv, argc);

	return Enable(argv, argc);
}


BOOL PRIVATE SetTownPickup(char **argv, int argc)
{
	if(argc == 3)
	{
		itemWatcher.SetTownPickup(atoi(argv[2]) == 1);
		return TRUE;
	}

	server->GameErrorf("Invalid input, correct usage is .pick townpick n");
	return FALSE;
}

BOOL PRIVATE Radius(char** argv, int argc)
{
	if(argc == 3)
	{
		itemWatcher.SetRadius(atoi(argv[2]));
		return TRUE;
	}

	server->GameErrorf("Invalid input, correct usage is .pick radius n");
	return FALSE;
}

BOOL PRIVATE GoldSpeed(char** argv, int argc)
{
	if (argc == 3)
	{
		itemWatcher.SetGoldSpeed(atoi(argv[2]));
		return TRUE;
	}

	server->GameErrorf("Invalid input, correct usage is .pick goldspeed n");
	return FALSE;
}

BOOL PRIVATE Mute(char** argv, int argc)
{
	itemWatcher.Mute();
	return TRUE;
}

BOOL PRIVATE Items(char **argv, int argc)
{
	itemWatcher.TogglePickItems();
	return TRUE;
}

BOOL PRIVATE MinGold(char** argv, int argc)
{
	if(argc == 3)
	{
		itemWatcher.SetMinGold(atoi(argv[2]));
		return TRUE;
	}

	server->GameErrorf("Invalid input, correct usage is .pick min n");
	return FALSE;
}

BOOL PRIVATE ToggleShowItemLevel(char** argv, int argc)
{
	waitingForItemPickup = !waitingForItemPickup;

	if(waitingForItemPickup)
	{
		server->GameInfof("Waiting for item pickup to cursor...");
	}
	else
	{
		server->GameInfof("No longer waiting for item pick to cursor");
	}

	return TRUE;
}

BOOL PRIVATE ToggleShowItemDebug(char** argv, int argc)
{
	waitingForItemPickupDebug = !waitingForItemPickupDebug;

	if(waitingForItemPickupDebug)
	{
		server->GameInfof("Waiting for item pickup to cursor...");
	}
	else
	{
		server->GameInfof("No longer waiting for item pick to cursor");
	}

	return TRUE;
}

BOOL PRIVATE ShowColors(char** argv, int argc)
{
	server->GameStringf("ÿc1 1 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc2 2 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc3 3 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc4 4 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc5 5 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc6 6 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc7 7 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc8 8 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc9 9 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc; ; - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc: : - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc0 0 - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc/ / - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc( ( - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc++ - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");
	server->GameStringf("ÿc- - - abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+1234567890-=");

	return TRUE;
}

BOOL PRIVATE ReloadItemDefs(char** argv, int argc)
{
	server->GamePrintInfo("Reloading item definitions...");

	if(!itemWatcher.LoadItems())
	{
		server->GamePrintError("There was an error loading the item files");
	}

	return TRUE;
}

////////////////////////////////////////////
//
//               EXPORTS
//
/////////////////////////////////////////////


CLIENTINFO
(
	0,1,
	"",
	"",
	"Gold picker",
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
		"on",
		Enable,
		"Enable pick"
	},
	{
		"off",
		Disable,
		"Diable pick"
	},
	{
		"toggle",
		Toggle,
		"Toggle pick"
	},
	{
		"mute",
		Mute,
		"Mute/unmute pick"
	},
	{
		"unmute",
		Mute,
		"Mute/unmute pick"
	},
	{
		"radius",
		Radius,
		"Set radius, default = 4"
	},
	{
		"goldspeed",
		GoldSpeed,
		"Set gold pickup speed, default = 1. 3 risks disconnects, above will disconnect."
	},
	{
		"min",
		MinGold,
		"Set minimum gold to pickup, default = 50"
	},
	{
		"lvl",
		ToggleShowItemLevel,
		"Shows the item level"
	},

	{
		"debug",
		ToggleShowItemDebug,
		"Shows the item stat values along with max value before overflow"
	},
	{
		"colors",
		ShowColors,
		"Displays sample colors"
	},
	{
		"reload",
		ReloadItemDefs,
		"Reloads the item definitions"
	},
	{
		"townpick",
		SetTownPickup,
		"Sets town pickit on or off. Args: 1|0"
	},
	{
		"items",
		Items,
		"Toggles picking of items"
	},
	{NULL}
};

VOID EXPORT OnGameLeave(THISGAMESTRUCT* thisgame)
{
	itemWatcher.OnGameLeave();
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	itemWatcher.OnGameJoin();
}

BOOL EXPORT OnClientStart()
{
	if(!itemWatcher.LoadItems())
	{
		server->GameErrorf("Failed to load item tables. Do you have pickItems.txt?");
		server->GameErrorf(" in the plugin directory?");
		return FALSE;
	}

	enabled = true;

	return TRUE;
}

int ticksLeft = 3;
DWORD EXPORT OnGameTimerTick()
{
	if(enabled)
	{
		itemWatcher.OnTick();
	}

	return 0;
}

VOID EXPORT OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam)
{
	if(nMessage == UM_DESTROY)
	{
		if(lpUnit->dwUnitType == UNIT_TYPE_ITEM)
		{
			itemWatcher.OnItemDestroy(lpUnit->dwUnitID);
		}
	}
}

bool IsPerfectRare(const ITEM& item)
{
	auto dataTables = (D2DataTablesStrc*)server->GetDataTables();
	return true;
}

const char* eventNames[]{
	"hit by a missile"		  ,
	"damaged in melee"		  ,
	"damaged by missile"	  ,
	"melee attack atttempt"	  ,
	"do active state skill"	  ,
	"do damage in melee"	  ,
	"do missile damage"		  ,
	"do melee attack"		  ,
	"do missile attack"		  ,
	"killed something"		  ,
	"killed by something"	  ,
	"dealt damage"			  ,
	"gain a level"			  ,
};


std::string CheckProperty(GAMEUNIT &itemUnit, UnitAny *unit, const D2PropertyStrc& property, D2DataTablesStrc *dataTables)
{
	std::stringstream ss("");

	auto propertyTxt = &dataTables->pPropertiesTxt[property.nProperty];
	if (propertyTxt == nullptr)
	{
		return "ERROR";
	}

	auto statTxt = server->GetItemStatCostTxtRecord(propertyTxt->wStat[0]);
	if (statTxt == nullptr)
	{
		return "ERROR";
	}

	auto minimumValue = std::min(property.nMin, property.nMax);
	auto maximumValue = std::max(property.nMin, property.nMax);

	std::string propertyName = server->GetPropertyName(property.nProperty);
	switch (propertyTxt->nFunc[0])
	{
		case 8:
		case 1:
		{
			auto actualValue = (int32_t)server->GetUnitStat(&itemUnit, propertyTxt->wStat[0]);
			actualValue >>= statTxt->nValShift;

			ss << " " << propertyName << " [" << property.nMin << " - " << property.nMax << "] ";

			if (property.nMax != property.nMin)
			{
				if (actualValue > maximumValue || actualValue < minimumValue)
				{
					ss << " ÿc9Abnormalÿc0 value of " << actualValue;
				}
				else if (actualValue == property.nMax)
				{
					ss << " ÿc2PERFECTÿc0 value of " << actualValue;
				}
				else
				{
					ss << " ÿc1Flawedÿc0 value of " << actualValue;
				}
			}

			break;
		}
		case 11: 
		{
			const auto chargeLevel = property.nMax;
			const auto chanceToCast = property.nMin;
			const auto skillChargeId = property.nLayer;

			if (skillChargeId >= dataTables->nSkillsTxtRecordCount)
			{
				server->GameStringf("Invalid skillChargeId %d", skillChargeId);
				return "ERROR";
			}

			auto skillTxt = &dataTables->pSkillsTxt[skillChargeId];
			auto skillDesc = &dataTables->pSkillDescTxt[skillTxt->wSkillDesc];

			std::wstring skillNameW = server->GetStringFromTblIndex(skillDesc->wStrName);
			std::string skillName(skillNameW.begin(), skillNameW.end());

			ss << " " << chanceToCast << "% chance to cast level " << chargeLevel << " " << skillName << " on ";

			for (int i = 0; i < 2; i++) {
				auto eventIndex = statTxt->wItemEvent[i];
				if (eventIndex < 0) {
					break;
				}

				if (eventIndex > sizeof(eventNames) / sizeof(eventNames[0])) 
				{
					ss << "ERROR";
				}
				else
				{
					ss << eventNames[eventIndex];
				}
				if (i == 0 && statTxt->wItemEvent[1] != -1) 
				{
					ss << " OR ";
				}
			}

			break;
		}
		case 17:
		{
			auto actualValue = server->GetUnitStat(&itemUnit, propertyTxt->wStat[0]);
			actualValue >>= statTxt->nValShift;

			ss << " " << actualValue << " " << propertyName;
			break;
		}
		case 19: // Skill charges
		{
			const auto chargeLevel = property.nMax;
			const auto totalCharges = property.nMin;
			const auto skillChargeId = property.nLayer;

			if (skillChargeId >= dataTables->nSkillsTxtRecordCount)
			{
				server->GameStringf("Invalid skillChargeId %d", skillChargeId);
				return "ERROR";
			}

			auto skillTxt = &dataTables->pSkillsTxt[skillChargeId];
			auto skillDesc = &dataTables->pSkillDescTxt[skillTxt->wSkillDesc];

			std::wstring skillNameW = server->GetStringFromTblIndex(skillDesc->wStrName);
			std::string skillName(skillNameW.begin(), skillNameW.end());

			ss << " Level " << chargeLevel << " " << skillName << " (" << totalCharges << " charges)";

			break;
		}
		case 21: // +class skills
		{
			auto actualValue = server->GetUnitStatBonus(unit, propertyTxt->wStat[0], propertyTxt->wVal[0]);
			actualValue >>= statTxt->nValShift;

			ss << " " << propertyName << " [" << property.nMin << " - " << property.nMax << "] ";

			if (property.nMin != property.nMax) 
			{
				if (actualValue > maximumValue || actualValue < minimumValue)
				{
					ss << " ÿc9Abnormalÿc0 value of " << actualValue;
				}
				else if (actualValue == property.nMax)
				{
					ss << " ÿc2PERFECTÿc0 value of " << actualValue;
				}
				else
				{
					ss << " ÿc1Flawedÿc0 value of " << actualValue;
				}
			}

			break;
		}
		case 22: // oskills
		{
			auto oskillId = property.nLayer;
			if (oskillId >= dataTables->nSkillsTxtRecordCount)
			{
				server->GameStringf("Invalid oskillID %d", oskillId);
				return "ERROR";
			}

			auto actualValue = server->GetUnitStatBonus(unit, propertyTxt->wStat[0], property.nLayer);

			auto skillTxt = &dataTables->pSkillsTxt[oskillId];
			auto skillDesc = &dataTables->pSkillDescTxt[skillTxt->wSkillDesc];

			std::wstring skillNameW = server->GetStringFromTblIndex(skillDesc->wStrName);
			std::string skillName(skillNameW.begin(), skillNameW.end());

			ss << " " << skillName << " [" << property.nMin << " - " << property.nMax << "] ";

			if (property.nMin != property.nMax)
			{
				if (actualValue > maximumValue || actualValue < minimumValue)
				{
					ss << " ÿc9Abnormalÿc0 value of " << actualValue;
				}
				else if (actualValue == property.nMax)
				{
					ss << " ÿc2PERFECTÿc0 value of " << actualValue;
				}
				else
				{
					ss << " ÿc1Flawedÿc0 value of " << actualValue;
				}
			}
			break;
		}
		default:
		{
			ss << " " << propertyName << " [" << property.nMin << " - " << property.nMax << "] ";
			break;
		}
	}

	return ss.str();
}

bool DumpUniqueProperties(const ITEM& item)
{
	auto dataTables = (D2DataTablesStrc*)server->GetDataTables();

	if (item.wSetUniqueID >= dataTables->nUniqueItemsTxtRecordCount) 
	{
		server->GameStringf("Invalid wSetUniqueID of wSetUniqueID", item.wSetUniqueID);
		return true;
	}
	auto uniqueTxt = &dataTables->pUniqueItemsTxt[item.wSetUniqueID];

	GAMEUNIT itemUnit;
	itemUnit.dwUnitID = item.dwItemID;
	itemUnit.dwUnitType = UNIT_TYPE_ITEM;

	auto unit = (UnitAny*)server->VerifyUnit(&itemUnit);
	if (unit == nullptr)
	{
		server->GameStringf("Failed to verify unit");
		return "";
	}

	for (auto i = 0; i < sizeof(uniqueTxt->pProperties)/sizeof(uniqueTxt->pProperties[0]); ++i)
	{
		const auto& currentProperty = uniqueTxt->pProperties[i];
		if (currentProperty.nProperty < 0) 
		{
			break;
		}

		auto result = CheckProperty(itemUnit, unit, currentProperty, dataTables);
		server->GameStringf("  ÿc5Propertyÿc0 %d: %s", currentProperty.nProperty, result.c_str());

		// fix the odd negative cases of "min = -27 max = -80" -> "min = -80 max = -27"
		//auto minimumValue = std::min(currentProperty->nMin, currentProperty->nMax);
		//auto maximumValue = std::max(currentProperty->nMin, currentProperty->nMax);
	}

	return false;
}

void DumpItemDebug(const ITEM& item) 
{
	server->GameStringf("STATS:");
	GAMEUNIT unit;
	unit.dwUnitID = item.dwItemID;
	unit.dwUnitType = UNIT_TYPE_ITEM;

	auto numStats = server->GetNumStats();
	for (size_t i = 0; i < numStats; i++)
	{
		auto statValue = server->GetUnitStat(&unit, i);
		if (statValue != 0)
		{
			auto statTxt = server->GetItemStatCostTxtRecord(i);
			statValue >>= statTxt->nValShift;

			server->GameStringf(" [%d] %s = %d / %d", i, server->GetStatName(i), statValue, (int)pow(2, statTxt->nSaveBits) - statTxt->dwSaveAdd);
		}
	}
}

std::string GetAffixDetails(GAMEUNIT &itemUnit, LPD2MagicAffixTxt affix)
{
	auto dataTables = (D2DataTablesStrc*)server->GetDataTables();

	std::stringstream ss("");

	auto unit = (UnitAny *)server->VerifyUnit(&itemUnit);
	if (unit == nullptr)
	{
		server->GameStringf("Failed to verify unit");
		return "";
	}

	for (auto i = 0; i < 3; ++i)
	{
		auto& currentProperty = affix->pProperties[i];
		if (currentProperty.nProperty < 0) 
		{
			break;
		}

		ss << CheckProperty(itemUnit, unit, currentProperty, dataTables);
	}

	return ss.str();
}

void DumpItemInfo(const ITEM& item)
{
	auto dataTables = (D2DataTablesStrc*)server->GetDataTables();
	char itemColor[4];

	switch(item.iQuality)
	{
		case ITEM_QUALITY_MAGIC:
			strcpy_s(itemColor, 4, "ÿc3");
			break;
		case ITEM_QUALITY_RARE:
			strcpy_s(itemColor, 4, "ÿc9");
			break;
		case ITEM_QUALITY_CRAFT:
			strcpy_s(itemColor, 4, "ÿc8");
			break;
		case ITEM_QUALITY_UNIQUE:
 			strcpy_s(itemColor, 4, "ÿc4");
			break;
		default:
			return;
	}

	if (item.iQuality == ITEM_QUALITY_UNIQUE)
	{
		server->GameStringf("%s%sÿc0 (%d) properties:", itemColor, server->GetItemName(item.szItemCode), item.wSetUniqueID);

		DumpUniqueProperties(item);
	}
	else 
	{
		auto numPrefixes = dataTables->pMagicAffixDataTables.pAutoMagic - dataTables->pMagicAffixDataTables.pMagicPrefix;
		auto numSuffixes = dataTables->pMagicAffixDataTables.pMagicPrefix - dataTables->pMagicAffixDataTables.pMagicSuffix;

		GAMEUNIT unit;
		unit.dwUnitID = item.dwItemID;
		unit.dwUnitType = UNIT_TYPE_ITEM;

		server->GameStringf("%s%sÿc0 affixes:", itemColor, server->GetItemName(item.szItemCode));

		for (int i = 0; i < 3; i++)
		{
			if (item.wPrefix[i] != 0)
			{
				if (item.wPrefix[i] >= numPrefixes)
				{
					server->GameStringf("Unknown prefix %d", item.wPrefix[i]);
					continue;
				}

				auto prefixTxt = &dataTables->pMagicAffixDataTables.pMagicPrefix[item.wPrefix[i] - 1];
				auto prefixDetails = GetAffixDetails(unit, prefixTxt);

				server->GameStringf("  ÿc;Prefixÿc0 %d: %s - %s", item.wPrefix[i], prefixTxt->szName, prefixDetails.c_str());
			}
		}
		for (int i = 0; i < 3; i++)
		{
			if (item.wSuffix[i] != 0)
			{
				if (item.wSuffix[i] >= numSuffixes)
				{
					server->GameStringf("Unknown suffix %d", item.wSuffix[i]);
					continue;
				}

				auto suffixTxt = &dataTables->pMagicAffixDataTables.pMagicSuffix[item.wSuffix[i] - 1];
				auto suffixDetails = GetAffixDetails(unit, suffixTxt);

				server->GameStringf("  ÿc:Suffixÿc0 %d: %s - %s", item.wSuffix[i], suffixTxt->szName, suffixDetails.c_str());
			}
		}
	}
}

DWORD EXPORT OnGamePacketBeforeSent(BYTE* aPacket, DWORD aLen)
{
	if(aPacket[0] == 0x68)
	{
		itemWatcher.OnGameLeave();
	}

	return aLen;
}

VOID EXPORT OnGamePacketAfterReceived(BYTE* aPacket, DWORD aLen)
{   

	if(enabled && (aPacket[0] == 0x9c || aPacket[0] == 0x9d))
	{
		ITEM item;
		if (!server->ParseItem(aPacket, aLen, item))
		{
			return;
		}

		if(aPacket[1] == ITEM_ACTION_FROM_STORAGE || aPacket[1] == ITEM_ACTION_PICK_UP || aPacket[1] == ITEM_ACTION_TO_CURSOR)
		{
			if(waitingForItemPickup)
			{
				server->GameStringf("%s: Level %d", item.szItemCode, item.iLevel);
				DumpItemInfo(item);
			}	
			if(waitingForItemPickupDebug)
			{
				DumpItemDebug(item);
			}
		}
		else
		{
			itemWatcher.OnItemAction(item);
		}
	}
}
