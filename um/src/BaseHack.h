#pragma once
#include "Player.h"

class BaseHack {
public:
	const char* title;
	static Player localPlayer;
	static ImVector<Player> players;
	virtual void RenderMenu() = 0;
	virtual void Tick(ImVec2 windowPos, ImVec2 windowSize) = 0;
	virtual const char* GetTitle() = 0;
	virtual bool HandleKeyBinds(WPARAM wParam, KBDLLHOOKSTRUCT* pKeyboard) = 0;
};
