#pragma once
#include "BaseHack.h"

class WallHack : public BaseHack {
public:
	const char* title = "Wallhack";
	bool activateWallhack = true;
	bool enableBox = true;
	bool enableSkel = true;
	ImVec4 colourBox = ImVec4(0, 0, 0, 1);
	ImVec4 colourSkel = ImVec4(0, 0, 0, 1);
	void RenderMenu();
	void Tick(ImVec2 windowPos, ImVec2 windowSize);
	void RenderBone(Player player, ImVec2 windowPos);
	const char* GetTitle();
	bool HandleKeyBinds(WPARAM wParam, KBDLLHOOKSTRUCT* pKeyboard);
};
