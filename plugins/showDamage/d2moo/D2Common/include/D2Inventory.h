#pragma once

#include "CommonDefinitions.h"


struct D2UnitStrc;
struct D2InventoryGridInfoStrc;

#pragma pack(1)

struct D2InvRectStrc
{
	int32_t nLeft;		//0x00
	int32_t nRight;		//0x04
	int32_t nTop;		//0x08
	int32_t nBottom;	//0x0C
};

enum D2C_PlayerBodyLocs
{
	BODYLOC_NONE,		//Not Equipped
	BODYLOC_HEAD,		//Helm
	BODYLOC_NECK,		//Amulet
	BODYLOC_TORSO,		//Body Armor
	BODYLOC_RARM,		//Right-Hand
	BODYLOC_LARM,		//Left-Hand
	BODYLOC_RRIN,		//Right Ring
	BODYLOC_LRIN,		//Left Ring
	BODYLOC_BELT,		//Belt
	BODYLOC_FEET,		//Boots
	BODYLOC_GLOVES,		//Gloves
	BODYLOC_SWRARM,		//Right-Hand on Switch
	BODYLOC_SWLARM,		//Left-Hand on Switch
	NUM_BODYLOC
};

#define D2C_InventoryHeader 0x1020304

enum D2C_ItemInvPage
{
	INVPAGE_INVENTORY = 0,
	INVPAGE_EQUIP = 1,
	INVPAGE_TRADE = 2,
	INVPAGE_CUBE = 3,
	INVPAGE_STASH = 4,
	INVPAGE_BELT = 5,
	INVPAGE_NULL = 255
};

enum D2C_InventoryRecords
{
	// 640x420
	INVENTORYRECORD_AMAZON,
	INVENTORYRECORD_SORCERESS,
	INVENTORYRECORD_NECROMANCER,
	INVENTORYRECORD_PALADIN,
	INVENTORYRECORD_BARBARIAN,
	INVENTORYRECORD_MONSTER,
	INVENTORYRECORD_TRADE_PAGE_1,
	INVENTORYRECORD_TRADE_PAGE_2,
	INVENTORYRECORD_BANK_PAGE_1,
	INVENTORYRECORD_TRANSMOGRIFY_BOX_PAGE_1,
	INVENTORYRECORD_GUILD_VAULT_PAGE_1,
	INVENTORYRECORD_TROPHY_CASE_PAGE_1,
	INVENTORYRECORD_BIG_BANK_PAGE_1,
	INVENTORYRECORD_HIRELING,
	INVENTORYRECORD_DRUID,
	INVENTORYRECORD_ASSASSIN,
	NUM_INVENTORY_PAGE_STATS,

	// 800x600
	INVENTORYRECORD_AMAZON2 = NUM_INVENTORY_PAGE_STATS,
	INVENTORYRECORD_SORCERESS2,
	INVENTORYRECORD_NECROMANCER2,
	INVENTORYRECORD_PALADIN2,
	INVENTORYRECORD_BARBARIAN2,
	INVENTORYRECORD_MONSTER2,
	INVENTORYRECORD_TRADE_PAGE_1_2,
	INVENTORYRECORD_TRADE_PAGE_2_2,
	INVENTORYRECORD_BANK_PAGE2,
	INVENTORYRECORD_TRANSMOGRIFY_BOX2,
	INVENTORYRECORD_GUILD_VAULT_PAGE2,
	INVENTORYRECORD_TROPHY_CASE_PAGE2,
	INVENTORYRECORD_BIG_BANK_PAGE2,
	INVENTORYRECORD_HIRELING2,
	INVENTORYRECORD_DRUID2,
	INVENTORYRECORD_ASSASSIN2
};

enum D2C_NodePages
{
	NODEPAGE_STORAGE = 1,
	NODEPAGE_BELTSLOTS = 2,
	NODEPAGE_EQUIP = 3
};

enum D2C_InventoryGrids
{
	INVGRID_BODYLOC,
	INVGRID_BELT,
	INVGRID_INVENTORY,
};

enum D2TradeStates
{
	TRADESTATE_OTHERNOROOM,
	TRADESTATE_SELFNOROOM,
};

struct D2InventoryGridStrc
{
	D2UnitStrc* pItem;						//0x00
	D2UnitStrc* pLastItem;					//0x04
	uint8_t nGridWidth;						//0x08
	uint8_t nGridHeight;					//0x09
	uint16_t pad0x0A;						//0x0A
	D2UnitStrc** ppItems;					//0x0C
};

struct D2CorpseStrc
{
	uint32_t unk0x00;						//0x00
	uint32_t dwUnitId;						//0x04
	uint32_t unk0x08;						//0x08
	D2CorpseStrc* pNextCorpse;				//0x0C
};

struct D2InventoryNodeStrc
{
	int32_t nItemId;						//0x00
	D2InventoryNodeStrc* pNext;				//0x04
};

struct D2InventoryStrc
{
	uint32_t dwSignature;					//0x00
	void* pMemPool;							//0x04
	D2UnitStrc* pOwner;						//0x08
	D2UnitStrc* pFirstItem;					//0x0C
	D2UnitStrc* pLastItem;					//0x10
	D2InventoryGridStrc* pGrids;			//0x14
	int32_t nGridCount;						//0x18
	uint32_t dwLeftItemGUID;				//0x1C
	D2UnitStrc* pCursorItem;				//0x20
	uint32_t dwOwnerGuid;					//0x24
	uint32_t dwItemCount;					//0x28
	D2InventoryNodeStrc* pFirstNode;		//0x2C
	D2InventoryNodeStrc* pLastNode;			//0x30
	D2CorpseStrc* pFirstCorpse;				//0x34
	D2CorpseStrc* pLastCorpse;				//0x38
	int32_t nCorpseCount;					//0x3C
};

struct D2ItemExtraDataStrc
{
	D2InventoryStrc* pParentInv;			//0x00
	D2UnitStrc* pPreviousItem;				//0x04
	D2UnitStrc* pNextItem;					//0x08
	char nNodePos;							//0x0C
	char nNodePosOther;						//0x0D
	uint16_t unk0x0E;						//0x0E
	D2UnitStrc* unk0x10;					//0x10
	D2UnitStrc* unk0x14;					//0x14
};
#pragma pack()
