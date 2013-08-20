#include "RuneDowngrade.h"

#include <vector>
#include <string>

BOOL CALLBACK EnumCubeItems(LPCITEM lpItem, LPARAM lParam)
{
	((std::vector<LPCITEM> *)lParam)->push_back(lpItem);
	return true;
};

RuneDowngrade::RuneDowngrade()
{
	m_currentState = STATE_Inactive;
	m_isExtractorInCube = false;

	runes.insert("r01"); // ÿc/I Rune
	runes.insert("r02"); // ÿc/U Rune
	runes.insert("r03"); // ÿc/Shi Rune
	runes.insert("r04"); // ÿc/Ka Rune
	runes.insert("r05"); // ÿc/N Rune
	runes.insert("r06"); // ÿc/Ku Rune
	runes.insert("r07"); // ÿc/Yo Rune
	runes.insert("r08"); // ÿc1Ki Rune
	runes.insert("r09"); // ÿc1Ri Rune
	runes.insert("r10"); // ÿc1Mi Rune
	runes.insert("r11"); // ÿc1Ya Rune
	runes.insert("r12"); // ÿc1A Rune
	runes.insert("r13"); // ÿc1Tsu Rune
	runes.insert("r14"); // ÿc1Chi Rune
	runes.insert("r15"); // ÿc9Sa Rune
	runes.insert("r16"); // ÿc9Yu Rune
	runes.insert("r17"); // ÿc9Ke Rune
	runes.insert("r18"); // ÿc9E Rune
	runes.insert("r19"); // ÿc9Ko Rune
	runes.insert("r20"); // ÿc9Ra Rune
	runes.insert("r21"); // ÿc9O Rune
	runes.insert("r22"); // ÿc8Ho Rune
	runes.insert("r23"); // ÿc8Me Rune
	runes.insert("r24"); // ÿc8Ru Rune
	runes.insert("r25"); // ÿc8Ta Rune
	runes.insert("r26"); // ÿc8To Rune
	runes.insert("r27"); // ÿc8Wa Rune
	runes.insert("r28"); // ÿc8Ha Rune
	runes.insert("r29"); // ÿc2Na Rune
	runes.insert("r30"); // ÿc2Ni Rune
	runes.insert("r31"); // ÿc2Se Rune
	runes.insert("r32"); // ÿc2Fu Rune
	runes.insert("r33"); // ÿc2Ma Rune
	runes.insert("r34"); // ÿc2Hi Rune
	runes.insert("r35"); // ÿc2Mo Rune
	runes.insert("r36"); // ÿc4No Rune
	runes.insert("r37"); // ÿc4Te Rune
	runes.insert("r38"); // ÿc4Ro Rune
	runes.insert("r39"); // ÿc4So Rune
	runes.insert("r40"); // ÿc4Mu Rune
	runes.insert("r41"); // ÿc4Ne Rune
	runes.insert("r42"); // ÿc4Re Rune
}

/// <summary>
/// Locates the extractor (town portal scroll) in the player's cube
/// </summary>
/// <returns>
/// true on success, false when extractor is missing or invalid items are in the cube
/// </returns>
bool RuneDowngrade::CheckCubeForExtractor()
{
	bool hasStocker = false;
	std::vector<LPCITEM> itemsInCube;

	me->EnumStorageItems(STORAGE_CUBE, EnumCubeItems, (LPARAM)(&itemsInCube));

	if(itemsInCube.size() != 1)
	{
		return false;
	}

	for each(auto item in itemsInCube)
	{
		if(_stricmp(item->szItemCode, "tsc") == 0)
		{
			return true;
		}
	}
	
	return false;
}

void RuneDowngrade::Start(const std::vector<std::string> &runeTypesToDowngrade)
{
	m_indexOfCurrentItemToDowngrade = 0;
	m_itemTypesToDowngrade = runeTypesToDowngrade;
	m_itemTypeToDowngrade = runeTypesToDowngrade[m_indexOfCurrentItemToDowngrade];

	DowngradeRune();
}

/// <summary>
/// Begins the process of downgrading runes in the player's inventory.
/// </summary>
void RuneDowngrade::DowngradeRune()
{
	if(!me->IsUIOpened(UI_CUBE))
	{
		me->OpenCube();
	}

	if(!CheckCubeForExtractor())
	{
		server->GameStringf("RuneDowngrade: Cube must contain town portal scroll");
		m_currentState = STATE_Inactive;
		return;
	}
	
	m_currentState = STATE_WaitingForItemToCube;
	m_isExtractorInCube = false;
	m_isRuneItemInCube = false;
	m_extractedItemID = -1;

	GetRuneFromInventory();
}

/// <summary>
/// Picks up a rune that needs to be downgraded from the player's inventory 
/// </summary>
void RuneDowngrade::GetRuneFromInventory()
{
	std::vector<LPITEM> itemsInInventory;

	me->EnumStorageItems(STORAGE_INVENTORY, EnumCubeItems, (LPARAM)&itemsInInventory);

	DWORD itemToDowngradeID = -1;

	// Go through all of the item codes that we need to downgrade until we find a type we have in our inventory
	for(; m_indexOfCurrentItemToDowngrade < m_itemTypesToDowngrade.size(); ++m_indexOfCurrentItemToDowngrade)
	{
		m_itemTypeToDowngrade = m_itemTypesToDowngrade[m_indexOfCurrentItemToDowngrade];

		for each(auto item in itemsInInventory)
		{
			if(_stricmp(item->szItemCode, m_itemTypeToDowngrade.c_str()) == 0)
			{
				itemToDowngradeID = item->dwItemID;
				break;
			}
		}

		if(itemToDowngradeID != -1)
		{
			break;
		}
	}

	// Failed to find any items to downgrade
	if(itemToDowngradeID == -1)
	{
		server->GameStringf("RuneDowngrade: Failed to find rune of type %s", m_itemTypeToDowngrade.c_str());
		m_currentState = STATE_Inactive;
		return;
	}

	m_itemToDowngradeID = itemToDowngradeID;
	m_currentState = STATE_WaitingForItemFromInventory;

	if(!me->PickStorageItemToCursor(m_itemToDowngradeID))
	{
		server->GameStringf("RuneDowngrade: Failed to pickup inventory item");
		m_currentState = STATE_Inactive;
		return;
	}
}

/// <summary>
/// Called whenever an item is picked up from the inventory
/// </summary>
/// <param name="item">The item that was picked up.</param>
void RuneDowngrade::OnItemFromInventory(const ITEM &item)
{
	if(m_currentState != STATE_WaitingForItemFromInventory)
	{
		return;
	}

	m_currentState = STATE_WaitingForItemToCubeFromHand;
	if(!me->DropCursorItemToStorage(STORAGE_CUBE))
	{
		server->GameStringf("RuneDowngrade: Failed to drop item to cube");
		m_currentState = STATE_Inactive;
		return;
	}
}

/// <summary>
/// Called whenever an item is moved to the cube
/// </summary>
/// <param name="item">The item that was moved to the cube.</param>
void RuneDowngrade::OnItemToCube(const ITEM &item)
{
	if(m_currentState == STATE_WaitingForItemToCubeFromHand)
	{
		// Dropping item from hand to cube (ready to downgrade)
		m_currentState = STATE_WaitingForItemToCube;
		me->Transmute();
		return;
	}
	else if(m_currentState != STATE_WaitingForItemToCube)
	{
		return;
	}

	if(_stricmp(item.szItemCode, "tsc") == 0)
	{
		m_isExtractorInCube = true;
	}
	else if(runes.count(item.szItemCode) != 0)
	{
		m_extractedItemID = item.dwItemID;
	}
	else 
	{
		server->GameStringf("RuneDowngrade: Invalid item in cube! = %s", item.szItemCode);
		m_currentState = STATE_Inactive;
		return;
	}

	// Both portal scroll and downgraded rune are in the cube, pick up the downgraded rune to move to inventory
	if(m_isExtractorInCube && m_extractedItemID != -1)
	{
		m_currentState = STATE_WaitingForItemFromCube;
		if(!me->PickStorageItemToCursor(m_extractedItemID))
		{
			server->GameStringf("RuneDowngrade: Failed to pickup extracted item!");
			m_currentState = STATE_Inactive;
			return;
		}
	}		
}

/// <summary>
/// Called whenever an item is pickedup to the cursor from the cube
/// </summary>
/// <param name="item">Item picked up from the cube.</param>
void RuneDowngrade::OnItemFromCube(const ITEM &item)
{
	if(m_currentState != STATE_WaitingForItemFromCube)
	{
		return;
	}

	m_currentState = STATE_WaitingForItemToInventory;
	if(!me->DropCursorItemToStorage(STORAGE_INVENTORY))
	{
		server->GameStringf("RuneDowngrade: Failed to drop extracted item to inventory!");
		m_currentState = STATE_Inactive;
		return;
	}
}

/// <summary>
/// Called whenever an item is moved to the player's inventory
/// </summary>
/// <param name="item">The item that was moved to the player's inventory.</param>
void RuneDowngrade::OnItemToInventory(const ITEM &item)
{
	if(m_currentState != STATE_WaitingForItemToInventory)
	{
		return;
	}

	m_currentState = STATE_Inactive;
	DowngradeRune();
}