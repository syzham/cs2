#include "Window.h"

static int init();
void SetHook();
void RemoveHook();
void HandleMessage(MSG msg);
void InitPlayers(HANDLE driver, uintptr_t client);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

HHOOK g_hHook = NULL;
#define WM_USER_ACTION (WM_USER + 1)


HWND cs2 = NULL;
BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == lParam)
	{
		cs2 = hwnd;
		return FALSE;
	}
	return TRUE;
}

HANDLE Player::driver;
uintptr_t Player::client;
uintptr_t Player::sens;

Json::Value Offset::offset;
Json::Value Offset::clientOffset;

Player BaseHack::localPlayer;

bool enableDebug = false;

uint32_t height;
uint32_t width;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	InitWindow(hInstance, nCmdShow, 0, 0);
	InitD3D();
	InitImGui();
	SetHook();

	init();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		NewFrame(msg);
		HandleMessage(msg);

		if (enableDebug)
			ImGui::ShowDemoWindow();

		RECT rect;
		GetWindowRect(cs2, &rect);

		Draw(rect.left + 8, rect.top + 32, width, height);

		Render();
	}


	RemoveHook();
	CleanImGui;
	CleanD3D();
	return msg.wParam;
}

static int init() {
	for (int i = 0; i < 0; i++) {
		AddConsole("test");
	}
	const DWORD pid = get_process_id(L"cs2.exe");
	if (pid == 0) {
		AddConsole("Failed to find cs2.exe", logStatus::ERR);
		return 0;
	}

	AddConsole("cs2.exe found", logStatus::SUCCESS);

	HANDLE driver = CreateFile(L"\\\\.\\asdf", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (driver == INVALID_HANDLE_VALUE) {
		AddConsole("Failed to create driver handle.", logStatus::ERR);
		return 0;
	}

	if (!driver::attach_to_process(driver, pid)) {
		AddConsole("Attachment failed.", logStatus::ERR);
		return 0;
	}

	AddConsole("Attachment successful.", logStatus::SUCCESS);

	uintptr_t client = get_module_base(pid, L"client.dll");
	if (client == 0) {
		AddConsole("Client not found.", logStatus::ERR);
		return 0;
	}
	
	AddConsole("client.dll found", logStatus::SUCCESS);

	uintptr_t engine = get_module_base(pid, L"engine2.dll");
	if (engine == 0) {
		AddConsole("engine2.dll not found", logStatus::ERR);
		return 0;
	}


	ConsoleDone();
	EnumWindows(EnumWindowsProcMy, pid);
	if (cs2 == NULL) {
		AddConsole("Can't get cs2 hWnd", logStatus::ERR);
		return 0;
	}
	
	std::ifstream offsetFile("offsets.json", std::ifstream::binary);
	offsetFile >> Offset::offset;

	std::ifstream clientFile("client.dll.json", std::ifstream::binary);
	clientFile >> Offset::clientOffset;
	Offset::clientOffset = Offset::clientOffset["client.dll"]["classes"];

	Player::client = client;
	Player::driver = driver;
	Player::sens = driver::read_memory<uintptr_t>(driver, client + Offset::offset["client.dll"]["dwSensitivity"].asUInt64());

	BaseHack::localPlayer.playerController = driver::read_memory<uintptr_t>(driver, client + Offset::offset["client.dll"]["dwLocalPlayerController"].asUInt64());
	BaseHack::localPlayer.playerPawn = driver::read_memory<uintptr_t>(driver, client + Offset::offset["client.dll"]["dwLocalPlayerPawn"].asUInt64());
	BaseHack::localPlayer.init();

	InitPlayers(driver, client);

	height = driver::read_memory<uint32_t>(driver, engine + Offset::offset["engine2.dll"]["dwWindowHeight"].asUInt64());
	width = driver::read_memory<uint32_t>(driver, engine + Offset::offset["engine2.dll"]["dwWindowWidth"].asUInt64());

	return 1;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
		if (getMenu().HandleKeyBinds(wParam, pKeyboard)) {
			return CallNextHookEx(g_hHook, nCode, wParam, lParam);
		}
		if (wParam == WM_KEYDOWN) {
			if (pKeyboard->vkCode == VK_RSHIFT) { 
				PostMessage(NULL, WM_USER_ACTION, 0, 0);
			}
			else if (pKeyboard->vkCode == VK_DELETE) {
				enableDebug = !enableDebug;
			}		
		}
	}
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

void SetHook() {
	if (!(g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0))) {
	}
}

void RemoveHook() {
	if (g_hHook) {
		UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}
}

void HandleMessage(MSG msg) {
	if (msg.message == WM_USER_ACTION) {
		ToggleMenu();
	}
}

void InitPlayers(HANDLE driver, uintptr_t client) {

	uintptr_t dwEntityList = driver::read_memory<uintptr_t>(driver, client + Offset::offset["client.dll"]["dwEntityList"].asUInt64());
	uintptr_t ControllerEntry = driver::read_memory<uintptr_t>(driver, dwEntityList + 0x10);
	for (int i = 0; i < 64; i++) {
		uintptr_t playerController = driver::read_memory<uintptr_t>(driver, ControllerEntry + 120 * (i & 0x1FF));
		if (playerController == 0 || playerController == BaseHack::localPlayer.playerController)
			continue;
		uintptr_t pawnHandle = driver::read_memory<uintptr_t>(driver, playerController + Offset::clientOffset["CBasePlayerController"]["fields"]["m_hPawn"].asUInt64());
		if (pawnHandle == 0)
			continue;
		uintptr_t entry2 = driver::read_memory<uintptr_t>(driver, dwEntityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16);
		if (entry2 == 0)
			continue;
		uintptr_t pawn = driver::read_memory<uintptr_t>(driver, entry2 + 120 * (pawnHandle & 0x1FF));
		if (pawn == 0 || pawn == BaseHack::localPlayer.playerPawn)
			continue;
		Player player;
		player.playerPawn = pawn;
		player.playerController = playerController;
		player.init();
		AddPlayer(player);
	}
}