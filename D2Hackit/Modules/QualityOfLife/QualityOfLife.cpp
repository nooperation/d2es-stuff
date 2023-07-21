#include "../../Includes/ClientCore.cpp"

#define CONFIG_FILE ".\\plugin\\QualityOfLife.ini"

int isRightClickItemTransfer = 0;

void ReadConfig();
void OnRightClick();

CLIENTINFO
(
	0,1,
	"",
	"",
	"QualityOfLife",
	""
)

BOOL CALLBACK ClientWndProc(LRESULT *pResult, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
		{
			OnRightClick();
			break;
		}
	}

	return FALSE;
}

void OnRightClick()
{
	if (!isRightClickItemTransfer)
	{
		return;
	}

	const auto isStashOpen = me->IsUIOpened(UI_STASH);
	const auto isInventoryOpen = me->IsUIOpened(UI_INVENTORY);
	const auto isCubeOpen = me->IsUIOpened(UI_CUBE);

	if (!isStashOpen && !isInventoryOpen && !isCubeOpen)
	{
		return;
	}

	const auto cursorItem = me->GetCursorItem();
	if (cursorItem == 0)
	{
		return;
	}

	const auto mousePositionGame = screen->GetMousePosition();
	const auto screenDimensions = screen->GetScreenSize();
	const auto isOnRightSideOfScreen = mousePositionGame.x > (screenDimensions.cx / 2);

	if (isOnRightSideOfScreen)
	{
		if (isStashOpen)
		{
			me->DropCursorItemToStorage(STORAGE_STASH);
		}
		else if (isCubeOpen)
		{
			me->DropCursorItemToStorage(STORAGE_CUBE);
		}
		else
		{
			me->DropCursorItemToGround();
		}
	}
	else
	{
		me->DropCursorItemToStorage(STORAGE_INVENTORY);
	}
}


BOOL EXPORT OnGameJoin()
{
	server->SendKey(189);

	return TRUE;
}

BOOL EXPORT OnClientStart()
{
	if (!GetModuleHandle("d2maphack.dll") && !GetModuleHandle("d2hackmap.dll"))
	{
		server->HookD2WndProc(g_hModule, ClientWndProc);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void ReadConfig()
{
	isRightClickItemTransfer = GetPrivateProfileInt("Main", "RightClickItemTransfer", 1, CONFIG_FILE);
}

VOID EXPORT OnGameJoin(THISGAMESTRUCT* thisgame)
{
	ReadConfig();
}

MODULECOMMANDSTRUCT ModuleCommands[]=
{
	{
		"help",
		OnGameCommandHelp,
		"helpÿc0 List commands available in this module.\n"
		"<command> helpÿc0 Shows detailed help for <command> in this module."
	},
	{NULL}
};