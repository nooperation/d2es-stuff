#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "../../Includes/D2Client.h"

enum class State
{
	Uninitialized = 0,
	WaitForNPC,
};


class AutoBuy
{
public:
	AutoBuy();

	bool Start(int quantity, const std::string& itemCode);
	void Stop();
	void OnNPCShopScreenOpened();
	void ProcessInventoryItem(const ITEM *item);
	void OnNpcItemList(const ITEM &merchantItem);

private:
	void BuyOurStuff();
	void RestockScrolls();
	bool BuyItemInQuantity(DWORD dwItemID) const;

	bool isAutomaticallyRefillTP = 0;
	int numTPTomesToRefill = 0;
	bool hasAlreadyRestockedTps = false;

	int itemQuantityToBuy = 0;
	std::string itemCodeToBuy = "";

	State currentState = State::Uninitialized;
	std::unordered_map<std::string, DWORD> merchantItems;
};
