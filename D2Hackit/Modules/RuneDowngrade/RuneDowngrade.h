#pragma once

#include <vector>
#include <string>
#include <hash_set>
#include "../../Includes/D2Client.h"

enum States
{
	STATE_Inactive,
	STATE_WaitingForItemToCube,
	STATE_WaitingForItemFromCube,
	STATE_WaitingForItemToInventory,
	STATE_WaitingForItemFromInventory,
	STATE_WaitingForItemToCubeFromHand,
};

class RuneDowngrade
{
public:
	RuneDowngrade();
	void Start(std::vector<std::string> runeTypesToDowngrade);
	void OnItemToCube(const ITEM &item);
	void OnItemFromCube(const ITEM &item);
	void OnItemToInventory(const ITEM &item);
	void OnItemFromInventory(const ITEM &item);
	


private:
	bool CheckCubeForExtractor();
	void DowngradeRune();
	void GetRuneFromInventory();

	States m_currentState;

	bool m_isExtractorInCube;
	bool m_isRuneItemInCube;

	std::vector<std::string> m_itemTypesToDowngrade;
	std::string m_itemTypeToDowngrade;
	int m_indexOfCurrentItemToDowngrade;

	DWORD m_extractedItemID;
	DWORD m_itemToDowngradeID;

	stdext::hash_set<std::string> runes;
};