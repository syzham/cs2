#include "AimHack.h"

void AimHack::RenderMenu() {
	ImGui::Checkbox("Enabled##AimHack", &enabled);
	ImGui::InputFloat("FOV", &fov);
	ImGui::SliderFloat("Smoothing", &smoothing, 0.f, 1.f);
}

void AimHack::Tick(ImVec2 windowPos, ImVec2 windowSize) {
	if (!activateAimhack || !enabled)
		return;

	for (Player player : players) {
		Vector OppPos = player.getBone(BoneStruct::head).getVector() - localPlayer.getBone(BoneStruct::head).getVector();
		float yaw = toDegrees(std::atan2(OppPos.y, OppPos.x));
		float pitch = toDegrees(std::atan2(-OppPos.z, std::hypot(OppPos.x, OppPos.y)));
		Vector currentAngle = Player::getAng();
		Vector delta = Vector(pitch, yaw) - currentAngle;
		delta = delta * (1 - smoothing);
		Vector fin = currentAngle + delta;
		ClampAngles(fin);
		Player::setAng(fin);
		break;
	}
}

const char* AimHack::GetTitle() {
	return title;
}

float AimHack::toDegrees(float rads) {
	return rads * 180.f / std::numbers::pi_v<float>;
}

void AimHack::ClampAngles(Vector& angle) {
	if (angle.x < -89.f) angle.x = -89.f;
	else if (angle.x > 89.f) angle.x = 89.f;
	while (angle.y < -180.f) angle.y += 360.f;
	while (angle.y > 180.f) angle.y -= 360.f;
	angle.z = 0.f;
}

bool AimHack::HandleKeyBinds(WPARAM wParam, KBDLLHOOKSTRUCT* pKeyboard) {
	if (wParam == WM_KEYDOWN && pKeyboard->vkCode == VK_SPACE && !activateAimhack) {
		activateAimhack = true;
		return true;
	}
	else if (wParam == WM_KEYUP && pKeyboard->vkCode == VK_SPACE && activateAimhack) {
		activateAimhack = false;
		return true;
	}
	else if (wParam == WM_KEYUP && pKeyboard->vkCode == VK_RCONTROL)
		test = true;
		Tick({0, 0}, {0, 0});
		test = false;
	return false;
}
