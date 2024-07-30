#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dwmapi.h>
#include <sstream>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "console.h"
#include "menu.h"

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitWindow(HINSTANCE hInstance, int nCmdShow, LONG x, LONG y);
void InitD3D();
void InitImGui();
void CleanImGui();
void CleanD3D(void);
void CleanWindow();
void NewFrame(MSG msg);
void Draw(float posX, float posY, float width, float height);
void Render();
void ToggleMenu();
void AddConsole(const char* text, int colour=logStatus::NORMAL);
ImVector<Player> GetPlayerList();
void AddPlayer(Player player);
void ConsoleDone();
Menu getMenu();
using RGBA = float[4];


