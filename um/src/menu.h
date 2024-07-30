#include <thread>
#include "WallHack.h"
#include "AimHack.h"
#include "TriggerHack.h"

class Menu {
public:
	Menu();
	bool withinWindow;
	bool enableDebug = false;
	static ImVector<BaseHack*> hacks;
	void Draw();
	void DrawOverlay(ImVec2 windowPos, ImVec2 windowSize);
	bool HandleKeyBinds(WPARAM wParam, KBDLLHOOKSTRUCT* pKeyboard);
};
