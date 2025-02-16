#include "Window.h"

bool showConsole = true;
bool showMenu = false;
bool showOverlay = false;

HWND hWnd;
IDXGISwapChain* swapChain;
ID3D11Device* device;
ID3D11DeviceContext* deviceContext;
ID3D11RenderTargetView* backBuffer;
POINTS position;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

Console console(&hWnd);
ImVector<Player> BaseHack::players;

ImVector<BaseHack*> Menu::hacks;
Menu menu;

void CleanupRenderTarget();
void CreateRenderTarget();

void InitWindow(HINSTANCE hInstance, int nCmdShow, LONG x, LONG y) {
	WNDCLASSEXW wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEXW));

	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"WindowClass";

	RegisterClassExW(&wc);

	hWnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT, L"WindowClass", L"ESP", WS_POPUP, x, y, 4000, 4000, NULL, NULL, hInstance, NULL);
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

	{
		RECT clientArea{};
		GetClientRect(hWnd, &clientArea);

		RECT windowArea{};
		GetWindowRect(hWnd, &windowArea);

		POINT diff{};
		ClientToScreen(hWnd, &diff);

		const MARGINS margins{
			windowArea.left + (diff.x - windowArea.left),
			windowArea.top + (diff.y - windowArea.top),
			clientArea.right,
			clientArea.bottom
		};

		DwmExtendFrameIntoClientArea(hWnd, &margins);
	}


	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
	switch (message) {
	case WM_SYSCOMMAND: {
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
	} break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	} return 0;

	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}

void InitD3D() {
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 2;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapChain, &device, NULL, &deviceContext);

	ID3D11Texture2D* pBackBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	device->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	pBackBuffer->Release();

	deviceContext->OMSetRenderTargets(1, &backBuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 4000;
	viewport.Height = 4000;
	deviceContext->RSSetViewports(1, &viewport);
}

void InitImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, deviceContext);

	SetWindowPos(hWnd, NULL, 0, 0, 100, 100, SWP_NOREDRAW);
}

void NewFrame(MSG msg) {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Draw(float posX, float posY, float width, float height) {
	InvalidateRect(hWnd, NULL, FALSE);
	if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
	{
		CleanupRenderTarget();
		swapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
		g_ResizeWidth = g_ResizeHeight = 0;
		CreateRenderTarget();
	}
	
	ImVec2 mainPos = ImGui::GetMainViewport()->Pos;

	if (showConsole && !console.finished) {
		console.Draw(!showMenu);
		showMenu = showOverlay = console.finished;
	}

	if (showMenu && console.finished) {
		menu.Draw();
	}

	if (showOverlay && console.finished) {
		SetWindowPos(hWnd, NULL, posX, posY, width, height, NULL);
		menu.DrawOverlay(mainPos, {width, height});
	}

	LONG style = GetWindowLong(hWnd, GWL_EXSTYLE);
	if (!menu.withinWindow && !(style & WS_EX_LAYERED)) {
		style |= WS_EX_LAYERED;
		SetWindowLong(hWnd, GWL_EXSTYLE, style);
	}
}

void Render() {
	ImGui::Render();
	const float clear_color_with_alpha[4] = { 0, 0, 0, 0};
	deviceContext->OMSetRenderTargets(1U,  &backBuffer, NULL);
	deviceContext->ClearRenderTargetView(backBuffer, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	swapChain->Present(1, 0);
}

void CleanImGui() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void CleanD3D() {
	swapChain->Release();
	backBuffer->Release();
	device->Release();
	deviceContext->Release();
}

void CleanWindow() {
}

void ToggleMenu() {
	if (!console.finished)
		return;
	if (!menu.withinWindow)
		return;
	LONG style = GetWindowLong(hWnd, GWL_EXSTYLE);
	if (showMenu) {
		showMenu = false;
		style |= WS_EX_LAYERED;
	}
	else {
		showMenu = true;
		style &= ~WS_EX_LAYERED;
	}

	SetWindowLong(hWnd, GWL_EXSTYLE, style);
}

void AddConsole(const char* text, int colour) {
	console.AddLog(text, colour);
}

void ConsoleDone() {
	console.Done();
}

ImVector<Player> GetPlayerList() {
	return BaseHack::players;
}

void AddPlayer(Player player) {
	BaseHack::players.push_back(player);
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	device->CreateRenderTargetView(pBackBuffer, nullptr, &backBuffer);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (backBuffer) { backBuffer->Release(); backBuffer = nullptr; }
}

Menu getMenu() {
	return menu;
}
