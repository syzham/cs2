#include "BaseHack.h"

class AimHack : public BaseHack {
public:
	const char* title = "Aim";
	bool enabled = true;
	bool activateAimhack = false;
	bool test = false;
	float fov = 5;
	float smoothing = 0.7f;
	void RenderMenu();
	void Tick(ImVec2 windowPos, ImVec2 windowSize);
	const char* GetTitle();
	float toDegrees(float rads);
	void ClampAngles(Vector& angle);
	bool HandleKeyBinds(WPARAM wParam, KBDLLHOOKSTRUCT* pKeyboard);
};
