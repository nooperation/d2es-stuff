//////////////////////////////////////////////////////////////
// Map info access
//
// Written by Abin
///////////////////////////////////////////////////////////////
#include "MapDef.h"
#include <tchar.h>
#include <string.h>
#include "Constants.h"

static const WPINFO WPS[] = 
{
	{0x01, 0, 0},
	{0x03, 0, 1},
	{0x04, 0, 2},
	{0x05, 0, 3},
	{0x06, 0, 4},
	{0x1b, 0, 5},
	{0x1d, 0, 6},
	{0x20, 0, 7},
	{0x23, 0, 8},

	{0x28, 1, 0},
	{0x30, 1, 1},
	{0x2a, 1, 2},
	{0x39, 1, 3},
	{0x2b, 1, 4},
	{0x2c, 1, 5},
	{0x34, 1, 6},
	{0x4a, 1, 7},
	{0x2e, 1, 8},

	{0x4b, 2, 0},
	{0x4c, 2, 1},
	{0x4d, 2, 2},
	{0x4e, 2, 3},
	{0x4f, 2, 4},
	{0x50, 2, 5},
	{0x51, 2, 6},
	{0x53, 2, 7},
	{0x65, 2, 8},

	{0x67, 3, 0},
	{0x6a, 3, 1},
	{0x6b, 3, 2},

	{0x6d, 4, 0},
	{0x6f, 4, 1},
	{0x70, 4, 2},
	{0x71, 4, 3},
	{0x73, 4, 4},
	{0x7b, 4, 5},
	{0x75, 4, 6},
	{0x76, 4, 7},
	{0x81, 4, 8},

	{0x00, 0, 0} // end
};

BOOL D2GetWPInfo(BYTE iMapID, LPWPINFO lpBuffer)
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(WPINFO));
	if (iMapID == 0)
		return FALSE;

	for (int i = 0; WPS[i].iMapID; i++)
	{
		if (iMapID == WPS[i].iMapID)
		{
			::memcpy(lpBuffer, &WPS[i], sizeof(WPINFO));
			return TRUE;
		}
	}

	return FALSE;
}

BOOL D2MapIsTown(BYTE iMapID)
{
	return iMapID == MAP_A1_ROGUE_ENCAMPMENT
		|| iMapID == MAP_A2_LUT_GHOLEIN
		|| iMapID == MAP_A3_KURAST_DOCKS
		|| iMapID == MAP_A4_THE_PANDEMONIUM_FORTRESS
		|| iMapID == MAP_A5_HARROGATH;
}

DWORD D2GetMapName(BYTE iMapID, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;
	
	lpszBuffer[0] = _T('\0');
	::memset(lpszBuffer, 0, sizeof(TCHAR) * dwMaxChars);
	if (dwMaxChars == 0)
		return 0;

	switch (iMapID)
	{	
///////////////////////////////////////////////////
// Act 1 Maps
///////////////////////////////////////////////////
	case MAP_A1_ROGUE_ENCAMPMENT:
		_tcsncpy(lpszBuffer, _T("Rogue Encampment"), dwMaxChars);
		break;

	case MAP_A1_BLOOD_MOOR:
		_tcsncpy(lpszBuffer, _T("Blood Moor"), dwMaxChars);
		break;

	case MAP_A1_COLD_PLAINS:
		_tcsncpy(lpszBuffer, _T("Cold Plains"), dwMaxChars);
		break;

	case MAP_A1_STONY_FIELD:
		_tcsncpy(lpszBuffer, _T("Stony Field"), dwMaxChars);
		break;

	case MAP_A1_DARK_WOOD:
		_tcsncpy(lpszBuffer, _T("Dark Wood"), dwMaxChars);
		break;

	case MAP_A1_BLACK_MARSH:
		_tcsncpy(lpszBuffer, _T("Black Marsh"), dwMaxChars);
		break;

	case MAP_A1_DEN_OF_EVIL:
		_tcsncpy(lpszBuffer, _T("Den of Evil"), dwMaxChars);
		break;

	case MAP_A1_CAVE_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Cave Level 1"), dwMaxChars);
		break;

	case MAP_A1_UNDERGROUND_PASSAGE_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Underground Passage Level 1"), dwMaxChars);
		break;

	case MAP_A1_HOLE_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Hole Level 1"), dwMaxChars);
		break;

	case MAP_A1_PIT_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Pit Level 1"), dwMaxChars);
		break;

	case MAP_A1_CAVE_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Cave Level 2"), dwMaxChars);
		break;

	case MAP_A1_UNDERGROUND_PASSAGE_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Underground Passage Level 2"), dwMaxChars);
		break;

	case MAP_A1_HOLE_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Hole Level 2"), dwMaxChars);
		break;

	case MAP_A1_PIT_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Pit Level 2"), dwMaxChars);
		break;

	case MAP_A1_BURIAL_GROUNDS:
		_tcsncpy(lpszBuffer, _T("Burial Grounds"), dwMaxChars);
		break;

	case MAP_A1_CRYPT:
		_tcsncpy(lpszBuffer, _T("Crypt"), dwMaxChars);
		break;

	case MAP_A1_MAUSOLEUM:
		_tcsncpy(lpszBuffer, _T("Mausoleum"), dwMaxChars);
		break;

	case MAP_A1_FORGOTTEN_TOWER:
		_tcsncpy(lpszBuffer, _T("Forgotten Tower"), dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Tower Cellar Level 1"), dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Tower Cellar Level 2"), dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Tower Cellar Level 3"), dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_4:
		_tcsncpy(lpszBuffer, _T("Tower Cellar Level 4"), dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_5:
		_tcsncpy(lpszBuffer, _T("Tower Cellar Level 5"), dwMaxChars);
		break;

	case MAP_A1_TAMOE_HIGHLAND:
		_tcsncpy(lpszBuffer, _T("Tamoe Highland"), dwMaxChars);
		break;

	case MAP_A1_MONASTERY_GATE:
		_tcsncpy(lpszBuffer, _T("Monastery Gate"), dwMaxChars);
		break;

	case MAP_A1_OUTER_CLOISTER:
		_tcsncpy(lpszBuffer, _T("Outer Cloister"), dwMaxChars);
		break;

	case MAP_A1_BARRACKS:
		_tcsncpy(lpszBuffer, _T("Barracks"), dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Jail Level 1"), dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Jail Level 2"), dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Jail Level 3"), dwMaxChars);
		break;

	case MAP_A1_INNER_CLOISTER:
	case MAP_A1_INNER_CLOISTER_2:
		_tcsncpy(lpszBuffer, _T("Inner Cloister"), dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Catacombs Level 1"), dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Catacombs Level 2"), dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Catacombs Level 3"), dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_4:
		_tcsncpy(lpszBuffer, _T("Catacombs Level 4"), dwMaxChars);
		break;

	case MAP_A1_TRISTRAM:
		_tcsncpy(lpszBuffer, _T("Tristram"), dwMaxChars);
		break;

	case MAP_A1_THE_SECRET_COW_LEVEL:
		_tcsncpy(lpszBuffer, _T("The Secret Cow Level"), dwMaxChars);
		break;


///////////////////////////////////////////////////
// Act 2 Maps
///////////////////////////////////////////////////
	case MAP_A2_LUT_GHOLEIN:
		_tcsncpy(lpszBuffer, _T("Lut Gholein"), dwMaxChars);
		break;

	case MAP_A2_ROCKY_WASTE:
		_tcsncpy(lpszBuffer, _T("Rocky Waste"), dwMaxChars);
		break;

	case MAP_A2_DRY_HILLS:
		_tcsncpy(lpszBuffer, _T("Dry Hills"), dwMaxChars);
		break;

	case MAP_A2_FAR_OASIS:
		_tcsncpy(lpszBuffer, _T("Far Oasis"), dwMaxChars);
		break;

	case MAP_A2_LOST_CITY:
		_tcsncpy(lpszBuffer, _T("Lost City"), dwMaxChars);
		break;

	case MAP_A2_VALLEY_OF_SNAKES:
		_tcsncpy(lpszBuffer, _T("Valley of Snakes"), dwMaxChars);
		break;

	case MAP_A2_CAYON_OF_THE_MAGI:
		_tcsncpy(lpszBuffer, _T("Cayon of the Magi"), dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Sewers Level 1"), dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Sewers Level 2"), dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Sewers Level 3"), dwMaxChars);
		break;

	case MAP_A2_HAREM_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Harem Level 1"), dwMaxChars);
		break;

	case MAP_A2_HAREM_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Harem Level 2"), dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Palace Cellar Level 1"), dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Palace Cellar Level 2"), dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Palace Cellar Level 3"), dwMaxChars);
		break;

	case MAP_A2_STONY_TOMB_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Stony Tomb Level 1"), dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Halls of the Dead Level 1"), dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Halls of the Dead Level 2"), dwMaxChars);
		break;

	case MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Claw Viper Temple Level 1"), dwMaxChars);
		break;

	case MAP_A2_STONY_TOMB_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Stony Tomb Level 2"), dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Halls of the Dead Level 3"), dwMaxChars);
		break;

	case MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Claw Viper Temple Level 2"), dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Maggot Lair Level 1"), dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Maggot Lair Level 2"), dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Maggot Lair Level 3"), dwMaxChars);
		break;

	case MAP_A2_ANCIENT_TUNNELS:
		_tcsncpy(lpszBuffer, _T("Ancient Tunnels"), dwMaxChars);
		break;

	case MAP_A2_TAL_RASHAS_TOMB_1:
	case MAP_A2_TAL_RASHAS_TOMB_2:
	case MAP_A2_TAL_RASHAS_TOMB_3:
	case MAP_A2_TAL_RASHAS_TOMB_4:
	case MAP_A2_TAL_RASHAS_TOMB_5:
	case MAP_A2_TAL_RASHAS_TOMB_6:
	case MAP_A2_TAL_RASHAS_TOMB_7:
		_tcsncpy(lpszBuffer, _T("Tal Rasha's Tomb"), dwMaxChars);
		break;

	case MAP_A2_TAL_RASHAS_CHAMBER:
		_tcsncpy(lpszBuffer, _T("Tal Rasha's Chamber"), dwMaxChars);
		break;

	case MAP_A2_ARCANE_SANCTUARY:
		_tcsncpy(lpszBuffer, _T("Arcane Sanctuary"), dwMaxChars);
		break;


///////////////////////////////////////////////////
// Act 3 Maps
///////////////////////////////////////////////////
	case MAP_A3_KURAST_DOCKS:
		_tcsncpy(lpszBuffer, _T("Kurast Docks"), dwMaxChars);
		break;

	case MAP_A3_SPIDER_FOREST:
		_tcsncpy(lpszBuffer, _T("Spider Forest"), dwMaxChars);
		break;

	case MAP_A3_GREAT_MARSH:
		_tcsncpy(lpszBuffer, _T("Great Marsh"), dwMaxChars);
		break;

	case MAP_A3_FLAYER_JUNGLE:
		_tcsncpy(lpszBuffer, _T("Flayer Jungle"), dwMaxChars);
		break;

	case MAP_A3_LOWER_KURAST:
		_tcsncpy(lpszBuffer, _T("Lower Kurast"), dwMaxChars);
		break;

	case MAP_A3_KURAST_BAZAAR:
		_tcsncpy(lpszBuffer, _T("Kurast Bazaar"), dwMaxChars);
		break;

	case MAP_A3_UPPER_KURAST:
		_tcsncpy(lpszBuffer, _T("Upper Kurast"), dwMaxChars);
		break;

	case MAP_A3_KURAST_CAUSEWAY:
		_tcsncpy(lpszBuffer, _T("Kurast Causeway"), dwMaxChars);
		break;

	case MAP_A3_TRAVINCAL:
		_tcsncpy(lpszBuffer, _T("Travinval"), dwMaxChars);
		break;

	case MAP_A3_ARCHNID_LAIR:
		_tcsncpy(lpszBuffer, _T("Archnid Lair"), dwMaxChars);
		break;

	case MAP_A3_SPIDER_CAVERN:
		_tcsncpy(lpszBuffer, _T("Spider Cavern"), dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Swampy Pit Level 1"), dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Swampy Pit Level 2"), dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Flayer Dungeon Level 1"), dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Flayer Dungeon Level 2"), dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Swampy Pit Level 3"), dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Flayer Dungeon Level 3"), dwMaxChars);
		break;

	case MAP_A3_SEWERS_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Sewers Level 1"), dwMaxChars);
		break;

	case MAP_A3_SEWERS_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Sewers Level 2"), dwMaxChars);
		break;

	case MAP_A3_RUINED_TEMPLE:
		_tcsncpy(lpszBuffer, _T("Ruined Temple"), dwMaxChars);
		break;

	case MAP_A3_DISUSED_FANE:
		_tcsncpy(lpszBuffer, _T("Disused Fane"), dwMaxChars);
		break;

	case MAP_A3_FORGOTTEN_RELIQUARY:
		_tcsncpy(lpszBuffer, _T("Forgotten Reliquary"), dwMaxChars);
		break;

	case MAP_A3_FORGOTTEN_TEMPLE:
		_tcsncpy(lpszBuffer, _T("Forgotten Temple"), dwMaxChars);
		break;

	case MAP_A3_RUINED_FANE:
		_tcsncpy(lpszBuffer, _T("Ruined Fane"), dwMaxChars);
		break;

	case MAP_A3_DISUSED_RELIQUARY:
		_tcsncpy(lpszBuffer, _T("Disused Reliquary"), dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Durance of Hate Level 1"), dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Durance of Hate Level 2"), dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Durance of Hate Level 3"), dwMaxChars);
		break;

///////////////////////////////////////////////////
// Act 4 Maps
///////////////////////////////////////////////////
	case MAP_A4_THE_PANDEMONIUM_FORTRESS:
		_tcsncpy(lpszBuffer, _T("The Pandemonium Fortress"), dwMaxChars);
		break;

	case MAP_A4_OUTER_STEPPES:
		_tcsncpy(lpszBuffer, _T("Outer Steppes"), dwMaxChars);
		break;

	case MAP_A4_PLAINS_OF_DESPAIR:
		_tcsncpy(lpszBuffer, _T("Plains of Despair"), dwMaxChars);
		break;

	case MAP_A4_CITY_OF_THE_DAMNED:
		_tcsncpy(lpszBuffer, _T("City of the Damned"), dwMaxChars);
		break;

	case MAP_A4_RIVER_OF_FLAME:
		_tcsncpy(lpszBuffer, _T("River of Flame"), dwMaxChars);
		break;

	case MAP_A4_THE_CHAOS_SANCTUARY:
		_tcsncpy(lpszBuffer, _T("The Chaos Sanctuary"), dwMaxChars);
		break;

///////////////////////////////////////////////////
// Act 5 Maps
///////////////////////////////////////////////////
	case MAP_A5_HARROGATH:
		_tcsncpy(lpszBuffer, _T("Harrogath"), dwMaxChars);
		break;

	case MAP_A5_THE_BLOODY_FOOTHILLS:
		_tcsncpy(lpszBuffer, _T("The Bloody Foothills"), dwMaxChars);
		break;

	case MAP_A5_FRIGID_HIGHLANDS:
		_tcsncpy(lpszBuffer, _T("Frigid Highlands"), dwMaxChars);
		break;

	case MAP_A5_ARREAT_PLATEAU:
		_tcsncpy(lpszBuffer, _T("Arreat Plateau"), dwMaxChars);
		break;

	case MAP_A5_CRYSTALLINE_PASSAGE:
		_tcsncpy(lpszBuffer, _T("Crystalline Passage"), dwMaxChars);
		break;

	case MAP_A5_FROZEN_RIVER:
		_tcsncpy(lpszBuffer, _T("Frozen River"), dwMaxChars);
		break;

	case MAP_A5_GLACIAL_TRAIL:
		_tcsncpy(lpszBuffer, _T("Glacial Trail"), dwMaxChars);
		break;

	case MAP_A5_DRIFTER_CAVERN:
		_tcsncpy(lpszBuffer, _T("Drifter Cavern"), dwMaxChars);
		break;

	case MAP_A5_FROZEN_TUNDRA:
		_tcsncpy(lpszBuffer, _T("Frozen Tundra"), dwMaxChars);
		break;

	case MAP_A5_THE_ANCIENTS_WAY:
		_tcsncpy(lpszBuffer, _T("The Ancients' Way"), dwMaxChars);
		break;

	case MAP_A5_ICY_CELLAR:
		_tcsncpy(lpszBuffer, _T("Icy Cellar"), dwMaxChars);
		break;

	case MAP_A5_ARREAT_SUMMIT:
		_tcsncpy(lpszBuffer, _T("Arreat Summit"), dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_PAIN:
		_tcsncpy(lpszBuffer, _T("Halls of Pain"), dwMaxChars);
		break;

	case MAP_A5_ABADDON:
		_tcsncpy(lpszBuffer, _T("Abaddon"), dwMaxChars);
		break;

	case MAP_A5_PIT_OF_ACHERON:
		_tcsncpy(lpszBuffer, _T("Pit of Acheron"), dwMaxChars);
		break;

	case MAP_A5_INFERNAL_PIT:
		_tcsncpy(lpszBuffer, _T("Infernal Pit"), dwMaxChars);
		break;

	case MAP_A5_NIHLATHAKS_TEMPLE:
		_tcsncpy(lpszBuffer, _T("Nihlathak's Temple"), dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_ANGUISH:
		_tcsncpy(lpszBuffer, _T("Halls of Anguish"), dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_VAUGHT:
		_tcsncpy(lpszBuffer, _T("Halls of Vaught"), dwMaxChars);
		break;	

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_1:
		_tcsncpy(lpszBuffer, _T("Worldstone Keep Level 1"), dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_2:
		_tcsncpy(lpszBuffer, _T("Worldstone Keep Level 2"), dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_3:
		_tcsncpy(lpszBuffer, _T("Worldstone Keep Level 3"), dwMaxChars);
		break;

	case MAP_A5_THRONE_OF_DESTRUCTION:
		_tcsncpy(lpszBuffer, _T("Throne of Destruction"), dwMaxChars);
		break;

	case MAP_A5_THE_WORLDSTONE_CHAMBER:
		_tcsncpy(lpszBuffer, _T("The Worldstone Chamber"), dwMaxChars);
		break;

	default:
		return 0;
		break;
	}

	return _tcslen(lpszBuffer);
}

BYTE D2GetTownMap(BYTE iAct)
{
	BYTE iMapID = 0;
	switch (iAct)
	{
	case 0:
		iMapID = MAP_A1_ROGUE_ENCAMPMENT;
		break;

	case 1:
		iMapID = MAP_A2_LUT_GHOLEIN;
		break;

	case 2:
		iMapID = MAP_A3_KURAST_DOCKS;
		break;

	case 3:
		iMapID = MAP_A4_THE_PANDEMONIUM_FORTRESS;
		break;

	case 4:
		iMapID = MAP_A5_HARROGATH;
		break;

	default:
		break;
	}

	return iMapID;
}

BYTE D2GetActFromMap(BYTE iMapID)
{
	if (iMapID < MAP_A2_LUT_GHOLEIN)
		return 0;

	if (iMapID < MAP_A3_KURAST_DOCKS)
		return 1;

	if (iMapID < MAP_A4_THE_PANDEMONIUM_FORTRESS)
		return 2;

	if (iMapID < MAP_A5_HARROGATH)
		return 3;

	if (iMapID <= MAP_A5_THE_WORLDSTONE_CHAMBER)
		return 4;

	return 0;
}
