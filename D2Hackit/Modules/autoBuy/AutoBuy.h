#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "../../Includes/D2Client.h"

enum class State
{
	Uninitialized = 0,
	CloseMerchantUiAndRestart,
	WaitForNPC,

	OpenMerchantUi,

	NpcListingitems,
	NpcDonelistingitems,
	PurchaseNextitem,
	PurchaseWaitforitem,
	CloseMerchantUiAndRunAutostock,
	RunAutostocker,
};


class AutoBuy
{
public:
	AutoBuy();

	bool Start(int quantity, const std::string& itemCode, bool isAutomaticMode);
	void Stop();
	void OnNPCShopScreenOpened();
	void ProcessInventoryItem(const ITEM *item);
	void OnNpcItemList(const ITEM &merchantItem);
	void OnUIClosed();
	void OnNpcSession(int success);
	void OnItemToStorageFromStore(ITEM& item);

	bool OnAutostockerMessage(const std::string_view& message);

private:
	void BuyOurStuff();
	void RestockScrolls();
	bool BuyItemInQuantity(DWORD dwItemID);

	DWORD FindMerchant();
	bool WillItemFit(DWORD dwItemId);

	bool isAutomaticallyRefillTP = 0;
	int refillTpAtCharge = 0;

	int numTPTomesToRefill = 0;
	bool hasAlreadyRestockedTps = false;
	bool isAutomaticMode = false;

	int itemQuantityToBuy = 0;
	std::string itemCodeToBuy = "";

	State currentState = State::Uninitialized;
	std::unordered_map<std::string, DWORD> merchantItems;

	std::string merchantName;
	GAMEUNIT merchantNpc;
};
