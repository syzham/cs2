#pragma once
#include "BaseHack.h"

class TriggerHack : public BaseHack {
public:
	const char* title = "Trigger";
	bool activateTrigger = false;
	virtual void RenderMenu();
	virtual void Tick(ImVec2 windowPos, ImVec2 windowSize);
	virtual const char* GetTitle();
	bool HandleKeyBinds(WPARAM wParam, KBDLLHOOKSTRUCT* pKeyboard);
};
