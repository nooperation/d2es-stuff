#include "../../d2hackit/includes/ClientCore.cpp"
#include <vector>
#include <fstream>
#include <string>
#include <hash_map>

int NumberOfItemsInCube();
BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam);
BOOL CALLBACK enumItemCountProc(LPCITEM item, LPARAM lParam);

enum StockerType
{
	STOCKER_GEMCAN = 0,
	STOCKER_RUNESA = 100,
	STOCKER_RUNESB = 200,
	STOCKER_DECALS = 300,
	STOCKER_REROLLINGORB = 400,
	STOCKER_MULTISTOCKER = 500,
	STOCKER_ANCIENTA = 600,
	STOCKER_ANCIENTB = 700,
	STOCKER_CRYSTAL = 800,
	STOCKER_CANOPENER = 900,
};

stdext::hash_map<std::string, int> stockers;

std::vector<std::string> gemCan;
std::vector<std::string> runesA;
std::vector<std::string> runesB;
std::vector<std::string> decals;
std::vector<std::string> rerollingOrb;
std::vector<std::string> multistocker;
std::vector<std::string> ancientA;
std::vector<std::string> ancientB;
std::vector<std::string> crystal;
std::vector<std::string> canopener;

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	stockers.clear();
	runesA.clear();
	runesB.clear();
	decals.clear();
	rerollingOrb.clear();
	multistocker.clear();
	ancientA.clear();
	ancientB.clear();
	crystal.clear();
	gemCan.clear();
	canopener.clear();

	int i = 0;

	i = STOCKER_GEMCAN;
	stockers["kv0"] = i++;
	stockers["ky0"] = i++;
	stockers["kb0"] = i++;
	stockers["kg0"] = i++;
	stockers["kr0"] = i++;
	stockers["kw0"] = i++;
	stockers["ks0"] = i++;
	stockers["kk0"] = i++;

	i = STOCKER_CANOPENER;
	stockers["ko1"] = i++;
	stockers["ko2"] = i++;
	stockers["ko3"] = i++;
	stockers["ko4"] = i++;
	stockers["ko5"] = i++;
	stockers["ko0"] = i++;

	i = STOCKER_RUNESA;
	stockers["s01"] = i++;
	stockers["s02"] = i++;
	stockers["s03"] = i++;
	stockers["s04"] = i++;
	stockers["s05"] = i++;
	stockers["s06"] = i++;
	stockers["s07"] = i++;
	stockers["s08"] = i++;
	stockers["s09"] = i++;
	stockers["s10"] = i++;
	stockers["s11"] = i++;
	stockers["s12"] = i++;
	stockers["s13"] = i++;
	stockers["s14"] = i++;
	stockers["s15"] = i++;
	stockers["s16"] = i++;
	stockers["s17"] = i++;
	stockers["s18"] = i++;
	stockers["s19"] = i++;
	stockers["s20"] = i++;
	stockers["s21"] = i++;

	i = STOCKER_RUNESB;
	stockers["s22"] = i++;
	stockers["s23"] = i++;
	stockers["s24"] = i++;
	stockers["s25"] = i++;
	stockers["s26"] = i++;
	stockers["s27"] = i++;
	stockers["s28"] = i++;
	stockers["s29"] = i++;
	stockers["s30"] = i++;
	stockers["s31"] = i++;
	stockers["s32"] = i++;
	stockers["s33"] = i++;
	stockers["s34"] = i++;
	stockers["s35"] = i++;
	stockers["s36"] = i++;
	stockers["s37"] = i++;
	stockers["s38"] = i++;
	stockers["s39"] = i++;
	stockers["s40"] = i++;
	stockers["s41"] = i++;
	stockers["s42"] = i++;
	stockers["s43"] = i++;
	stockers["s44"] = i++;
	stockers["s45"] = i++;
	stockers["s46"] = i++;
	stockers["s50"] = i++;

	i = STOCKER_DECALS;
	stockers["s51"] = i++;
	stockers["s52"] = i++;
	stockers["s53"] = i++;
	stockers["s54"] = i++;
	stockers["s55"] = i++;
	stockers["s56"] = i++;
	stockers["s57"] = i++;
	stockers["s58"] = i++;
	stockers["s59"] = i++;
	stockers["s60"] = i++;
	stockers["s61"] = i++;
	stockers["s62"] = i++;
	stockers["s63"] = i++;
	stockers["s64"] = i++;
	stockers["s65"] = i++;
	stockers["s66"] = i++;
	stockers["s67"] = i++;
	stockers["s68"] = i++;
	stockers["s69"] = i++;
	stockers["s70"] = i++;
	stockers["s71"] = i++;
	stockers["s72"] = i++;
	stockers["s73"] = i++;
	stockers["s74"] = i++;
	stockers["s75"] = i++;
	stockers["s76"] = i++;
	stockers["s77"] = i++;
	stockers["s78"] = i++;
	stockers["s79"] = i++;
	stockers["s80"] = i++;
	stockers["s81"] = i++;
	stockers["s82"] = i++;
	stockers["s83"] = i++;

	i = STOCKER_REROLLINGORB;

	stockers["t01"] = i++;
	stockers["t02"] = i++;
	stockers["t03"] = i++;
	stockers["t04"] = i++;
	stockers["t05"] = i++;
	stockers["t06"] = i++;
	stockers["t07"] = i++;
	stockers["t08"] = i++;
	stockers["t09"] = i++;
	stockers["t10"] = i++;
	stockers["t11"] = i++;
	stockers["t12"] = i++;
	stockers["t13"] = i++;
	stockers["t14"] = i++;
	stockers["t15"] = i++;
	stockers["t16"] = i++;
	stockers["t17"] = i++;
	stockers["t18"] = i++;
	stockers["t19"] = i++;
	stockers["t20"] = i++;
	stockers["t21"] = i++;
	//stockers["t22"] = i++;
	//stockers["t23"] = i++;
	stockers["t24"] = i++;
	stockers["t25"] = i++;
	//stockers["t26"] = i++;
	//stockers["t27"] = i++;
	//stockers["t28"] = i++;
	stockers["t29"] = i++;
	stockers["t30"] = i++;

	i = STOCKER_MULTISTOCKER;
	stockers["t51"] = i++;
	stockers["t52"] = i++;
	stockers["t53"] = i++;
	stockers["t75"] = i++;
	stockers["t54"] = i++;
	stockers["t55"] = i++;
	stockers["t56"] = i++;
	stockers["t57"] = i++;
	stockers["t58"] = i++;
	stockers["t59"] = i++;
	stockers["t60"] = i++;
	stockers["t61"] = i++;
	stockers["t62"] = i++;
	stockers["t63"] = i++;
	stockers["t64"] = i++;
	stockers["t65"] = i++;
	stockers["t66"] = i++;
	stockers["t67"] = i++;
	stockers["t68"] = i++;
	stockers["t69"] = i++;
	stockers["t70"] = i++;
	stockers["t71"] = i++;
	stockers["t72"] = i++;
	stockers["t73"] = i++;
	stockers["t74"] = i++;

	i = STOCKER_ANCIENTA;
	stockers["w31"] = i++;
	stockers["w32"] = i++;
	stockers["w33"] = i++;
	stockers["w34"] = i++;
	stockers["w35"] = i++;
	stockers["w36"] = i++;
	stockers["w37"] = i++;
	stockers["w38"] = i++;
	stockers["w39"] = i++;
	stockers["w40"] = i++;
	stockers["w41"] = i++;
	stockers["w42"] = i++;
	stockers["w43"] = i++;
	stockers["w44"] = i++;
	stockers["w45"] = i++;
	stockers["w46"] = i++;
	stockers["w47"] = i++;
	stockers["w48"] = i++;
	stockers["w49"] = i++;
	stockers["w50"] = i++;
	stockers["w51"] = i++;
	stockers["w52"] = i++;
	stockers["w53"] = i++;
	stockers["w54"] = i++;
	stockers["w55"] = i++;

	i = STOCKER_ANCIENTB;
	stockers["w56"] = i++;
	stockers["w57"] = i++;
	stockers["w58"] = i++;
	stockers["w59"] = i++;
	stockers["w60"] = i++;
	stockers["w61"] = i++;
	stockers["w62"] = i++;
	stockers["w63"] = i++;
	stockers["w64"] = i++;
	stockers["w65"] = i++;
	stockers["w66"] = i++;
	stockers["w67"] = i++;
	stockers["w68"] = i++;
	stockers["w69"] = i++;
	stockers["w70"] = i++;
	stockers["w71"] = i++;
	stockers["w72"] = i++;
	stockers["w73"] = i++;
	stockers["w74"] = i++;
	stockers["w75"] = i++;
	stockers["w76"] = i++;
	stockers["w77"] = i++;
	stockers["w78"] = i++;
	stockers["w79"] = i++;
	stockers["w80"] = i++;

	i = STOCKER_CRYSTAL;
	stockers["xu0"] = i++;
	stockers["xu1"] = i++;
	stockers["xu2"] = i++;
	stockers["xu3"] = i++;
	stockers["xu4"] = i++;
	stockers["xu5"] = i++;
	stockers["xu6"] = i++;
	stockers["xu7"] = i++;
	stockers["xu8"] = i++;
	stockers["xu9"] = i++;
	stockers["xux"] = i++;
	stockers["xuy"] = i++;


	canopener.push_back("chipped gem");
	canopener.push_back("flawed gem");
	canopener.push_back("standard gem");
	canopener.push_back("flawless gem");
	canopener.push_back("blemished gem");
	canopener.push_back("perfect gem");

	gemCan.push_back("amethyst");
	gemCan.push_back("topaz");
	gemCan.push_back("sapphire");
	gemCan.push_back("emerald");
	gemCan.push_back("ruby");
	gemCan.push_back("diamond");
	gemCan.push_back("skull");
	gemCan.push_back("obsidian");

	runesA.push_back("I");
	runesA.push_back("U");
	runesA.push_back("Shi");
	runesA.push_back("Ka");
	runesA.push_back("N");
	runesA.push_back("Ku");
	runesA.push_back("Yo");
	runesA.push_back("Ki");
	runesA.push_back("Ri");
	runesA.push_back("Mi");
	runesA.push_back("Ya");
	runesA.push_back("A");
	runesA.push_back("Tsu");
	runesA.push_back("Chi");
	runesA.push_back("Sa");
	runesA.push_back("Yu");
	runesA.push_back("Ke");
	runesA.push_back("E");
	runesA.push_back("Ko");
	runesA.push_back("Ra");
	runesA.push_back("O");


	runesB.push_back("Ho");
	runesB.push_back("Me");
	runesB.push_back("Ru");
	runesB.push_back("Ta");
	runesB.push_back("To");
	runesB.push_back("Wa");
	runesB.push_back("Ha");
	runesB.push_back("Na");
	runesB.push_back("Ni");
	runesB.push_back("Se");
	runesB.push_back("Fu");
	runesB.push_back("Ma");
	runesB.push_back("Hi");
	runesB.push_back("Mo");
	runesB.push_back("No");
	runesB.push_back("Te");
	runesB.push_back("Ro");
	runesB.push_back("So");
	runesB.push_back("Mu");
	runesB.push_back("Ne");
	runesB.push_back("Re");
	runesB.push_back("Su");
	runesB.push_back("He");
	runesB.push_back("Nu");
	runesB.push_back("Wo");
	runesB.push_back("Null");

	decals.push_back("El");
	decals.push_back("Eld");
	decals.push_back("Tir");
	decals.push_back("Nef");
	decals.push_back("Eth");
	decals.push_back("Ith");
	decals.push_back("Tal");
	decals.push_back("Ral");
	decals.push_back("Ort");
	decals.push_back("Thul");
	decals.push_back("Amn");
	decals.push_back("Sol");
	decals.push_back("Shael");
	decals.push_back("Dol");
	decals.push_back("Hel");
	decals.push_back("Io");
	decals.push_back("Lum");
	decals.push_back("Ko");
	decals.push_back("Fal");
	decals.push_back("Lem");
	decals.push_back("Pul");
	decals.push_back("Um");
	decals.push_back("Mal");
	decals.push_back("Ist");
	decals.push_back("Gul");
	decals.push_back("Vex");
	decals.push_back("Ohm");
	decals.push_back("Lo");
	decals.push_back("Sur");
	decals.push_back("Ber");
	decals.push_back("Jah");
	decals.push_back("Cham");
	decals.push_back("Zod");

	rerollingOrb.push_back("magic -> magic ring");
	rerollingOrb.push_back("magic -> rare ring");
	rerollingOrb.push_back("rare -> rare ring");
	rerollingOrb.push_back("rare -> unique ring");
	rerollingOrb.push_back("magic -> magic amulet");
	rerollingOrb.push_back("magic -> rare amulet");
	rerollingOrb.push_back("rare -> rare amulet");
	rerollingOrb.push_back("rare -> unique amulet");
	rerollingOrb.push_back("magic -> magic jewel");
	rerollingOrb.push_back("magic -> rare jewel");
	rerollingOrb.push_back("rare -> rare jewel");
	rerollingOrb.push_back("rare -> unique jewel");
	rerollingOrb.push_back("magic -> magic sc");
	rerollingOrb.push_back("magic -> rare sc");
	rerollingOrb.push_back("rare -> rare sc");
	rerollingOrb.push_back("magic -> magic lc");
	rerollingOrb.push_back("magic -> rare lc");
	rerollingOrb.push_back("rare -> rare lc");
	rerollingOrb.push_back("magic -> magic gc");
	rerollingOrb.push_back("magic -> rare gc");
	rerollingOrb.push_back("rare -> rare gc");
	//rerollingOrb.push_back("magic -> magic arrow");
	//rerollingOrb.push_back("magic -> rare arrow");
	rerollingOrb.push_back("rare -> rare arrow");
	rerollingOrb.push_back("rare -> unique arrow");
	//rerollingOrb.push_back("magic -> magic bolt");
	//rerollingOrb.push_back("magic -> rare bolt");
	//rerollingOrb.push_back("rare -> rare bolt");
	//rerollingOrb.push_back("rare -> unique bolt");
	rerollingOrb.push_back("devil\'s food ");

	multistocker.push_back("maple leaf");
	multistocker.push_back("dragon stone");
	multistocker.push_back("decipherer");
	multistocker.push_back("anvil stone");
	multistocker.push_back("cookbook");
	multistocker.push_back("steak");
	multistocker.push_back("brain");
	multistocker.push_back("eye");
	multistocker.push_back("quill");
	multistocker.push_back("spleen");
	multistocker.push_back("tail");
	multistocker.push_back("jawbone");
	multistocker.push_back("fang");
	multistocker.push_back("horn");
	multistocker.push_back("scalp");
	multistocker.push_back("heart");
	multistocker.push_back("soul");
	multistocker.push_back("ear");
	multistocker.push_back("flag");
	multistocker.push_back("socket donut");
	multistocker.push_back("coupon (nor)");
	multistocker.push_back("coupon (exc)");
	multistocker.push_back("coupon (eli)");
	multistocker.push_back("wild card");
	multistocker.push_back("full rejuv pot");

	ancientA.push_back("#1");
	ancientA.push_back("#2");
	ancientA.push_back("#3");
	ancientA.push_back("#4");
	ancientA.push_back("#5");
	ancientA.push_back("#6");
	ancientA.push_back("#7");
	ancientA.push_back("#8");
	ancientA.push_back("#9");
	ancientA.push_back("#10");
	ancientA.push_back("#11");
	ancientA.push_back("#12");
	ancientA.push_back("#13");
	ancientA.push_back("#14");
	ancientA.push_back("#15");
	ancientA.push_back("#16");
	ancientA.push_back("#17");
	ancientA.push_back("#18");
	ancientA.push_back("#19");
	ancientA.push_back("#20");
	ancientA.push_back("#21");
	ancientA.push_back("#22");
	ancientA.push_back("#23");
	ancientA.push_back("#24");
	ancientA.push_back("#25");

	ancientB.push_back("#26");
	ancientB.push_back("#27");
	ancientB.push_back("#28");
	ancientB.push_back("#29");
	ancientB.push_back("#30");
	ancientB.push_back("#31");
	ancientB.push_back("#32");
	ancientB.push_back("#33");
	ancientB.push_back("#34");
	ancientB.push_back("#35");
	ancientB.push_back("#36");
	ancientB.push_back("#37");
	ancientB.push_back("#38");
	ancientB.push_back("#39");
	ancientB.push_back("#40");
	ancientB.push_back("#41");
	ancientB.push_back("#42");
	ancientB.push_back("#43");
	ancientB.push_back("#44");
	ancientB.push_back("#45");
	ancientB.push_back("#46");
	ancientB.push_back("#47");
	ancientB.push_back("#48");
	ancientB.push_back("#49");
	ancientB.push_back("#50");

	crystal.push_back("frozen soul");
	crystal.push_back("shadow quartz");
	crystal.push_back("bleeding stone");
	crystal.push_back("burning sulphur");
	crystal.push_back("dark azurite");
	crystal.push_back("bitter peridot");
	crystal.push_back("pulsing opal");
	crystal.push_back("enigmatic cinnabar");
	crystal.push_back("tomb jade");
	crystal.push_back("solid mercury");
	crystal.push_back("storm amber");
	crystal.push_back("tainted tourmaline");
}

BOOL PRIVATE TransmuteTo(char** argv, int argc)
{
	if(argc != 3)
	{
		return FALSE;
	}

	std::vector<std::string> itemsInCube;
	std::vector<std::string> *stockerTarget = NULL;

	me->EnumStorageItems(STORAGE_CUBE, enumItemProc, (LPARAM)&itemsInCube);

	if(itemsInCube.size() > 1)
	{
		server->GameStringf("ÿc:Transmuteÿc0: Too many items in cube");
	}
	else if(itemsInCube.size() == 0)
	{
		server->GameStringf("ÿc:Transmuteÿc0: Nothing in clube");
		return TRUE;
	}

	if(stockers.count(itemsInCube[0]) == 0)
	{
		server->GameStringf("ÿc:Transmuteÿc0: %s is not a stocker", server->GetItemName(itemsInCube[0].c_str()));
		return TRUE;
	}

	int stockerIndex = stockers[itemsInCube[0]];
	int stockerOffset = 0;

	if(stockerIndex >= STOCKER_CANOPENER)
	{
		stockerTarget = &canopener;
		stockerOffset = stockerIndex - STOCKER_CANOPENER;
	}
	else if(stockerIndex >= STOCKER_CRYSTAL)
	{
		stockerTarget = &crystal;
		stockerOffset = stockerIndex - STOCKER_CRYSTAL;
	}
	else if(stockerIndex >= STOCKER_ANCIENTB)
	{
		stockerTarget = &ancientB;
		stockerOffset = stockerIndex - STOCKER_ANCIENTB;
	}
	else if(stockerIndex >= STOCKER_ANCIENTA)
	{
		stockerTarget = &ancientA;
		stockerOffset = stockerIndex - STOCKER_ANCIENTA;
	}
	else if(stockerIndex >= STOCKER_MULTISTOCKER)
	{
		stockerTarget = &multistocker;
		stockerOffset = stockerIndex - STOCKER_MULTISTOCKER;
	}			
	else if(stockerIndex >= STOCKER_REROLLINGORB)
	{
		stockerTarget = &rerollingOrb;
		stockerOffset = stockerIndex - STOCKER_REROLLINGORB;
	}
	else if(stockerIndex >= STOCKER_DECALS)
	{
		stockerTarget = &decals;
		stockerOffset = stockerIndex - STOCKER_DECALS;
	}	
	else if(stockerIndex >= STOCKER_RUNESB)
	{
		stockerTarget = &runesB;
		stockerOffset = stockerIndex - STOCKER_RUNESB;
	}
	else if(stockerIndex >= STOCKER_RUNESA)
	{
		stockerTarget = &runesA;
		stockerOffset = stockerIndex - STOCKER_RUNESA;
	}
	else
	{
		stockerTarget = &gemCan;
		stockerOffset = stockerIndex - STOCKER_GEMCAN;
	}

	for(size_t i = 0; i < stockerTarget->size(); i++)
	{
		if(strstr((*stockerTarget)[i].c_str(), argv[2]) != NULL)
		{
			int transmuteCount = i - stockerOffset;

			if(transmuteCount < 0)
				transmuteCount += stockerTarget->size();

			server->GameCommandf("t count %d", transmuteCount);

			return TRUE;
		}
	}

	server->GameStringf("ÿc:Transmuteÿc0: %s not found, check case?", argv[2]);

	return TRUE;

}

BOOL PRIVATE TransmuteRepeat(char** argv, int argc)
{
	if(argc == 3)
	{
		int count = atoi(argv[2]);

		if(count < 0)
			count = 0;

		server->GameStringf("ÿc:Transmuteÿc0: transmuting %d times", count);

		for(int i = 0; i < count; i++)
		{
			me->Transmute();
		}

		return TRUE;
	}

	return FALSE;
}

DWORD EXPORT OnGamePacketBeforeReceived(BYTE* aPacket, DWORD aLen)
{   
	return aLen;
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
		"count",
		TransmuteRepeat,
		"transmutes N times"
	},
	{
		"start",
		TransmuteTo,
		"Transmutes N times"
	},
	{
		"t",
		TransmuteTo,
		"Transmutes N times"
	},
	{NULL}
};


BOOL CALLBACK enumItemProc(LPCITEM item, LPARAM lParam)
{
	((std::vector<const std::string>*)lParam)->push_back((*item).szItemCode);

	return TRUE;
}