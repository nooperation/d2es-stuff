#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "../../Includes/D2Client.h"

enum class State
{
	Uninitialized = 0,
	WaitForNPC,
	SellingItem,
	WaitForItemToSell,
};

struct ItemToSell {
	DWORD ItemId;
	std::string ItemCode;

	ItemToSell(DWORD itemId, const std::string &itemCode)
		: ItemId(itemId), ItemCode(itemCode)
	{
	}
};

class AutoSell
{
public:
	AutoSell();
	void OnTick();

	bool Start();
	void OnItemSold();
	void Stop();
	void ProcessInventoryItem(const ITEM *item);

private:
	bool LoadItemMap(const std::string &fileName, std::unordered_map<std::string, std::string> &itemMap);
	void SellQueuedItems();
	bool SellItem(DWORD dwItemID) const;

	std::queue<ItemToSell> itemsToSell;

	State currentState;
	std::unordered_map<std::string, std::string> targetItems;
};

BOOL CALLBACK FindStuffToSellCallback(LPCPRESETUNIT presetUnit, LPARAM lParam);