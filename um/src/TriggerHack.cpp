#include "TriggerHack.h"

void TriggerHack::RenderMenu() {
	ImGui::Text("Attack: %d", localPlayer.getAttack());
	ImGui::Checkbox("Activate##Trigger", &activateTrigger);
}

void TriggerHack::Tick(ImVec2 windowPos, ImVec2 windowSize) {
	if (!activateTrigger) return;
	for (int i = 0; i < 1; i++) {
		localPlayer.setAttack(false);
		DWORD handle = driver::read_memory<DWORD>(Player::driver, localPlayer.playerPawn + Offset::clientOffset["C_CSPlayerPawnBase"]["fields"]["m_iIDEntIndex"].asUInt64());
		if (handle <= 0) continue;

		uintptr_t dwEntityList = driver::read_memory<uintptr_t>(Player::driver, Player::client + Offset::offset["client.dll"]["dwEntityList"].asUInt64());
		uintptr_t han = driver::read_memory<uintptr_t>(Player::driver, dwEntityList + (0x8 * (handle >> 9) + 0x10));
		uintptr_t list = driver::read_memory<uintptr_t>(Player::driver, han);
		DWORD64 entryList = driver::read_memory<DWORD64>(Player::driver, list);
		if (entryList == 0) continue;

		DWORD64 pawn = driver::read_memory<DWORD64>(Player::driver, entryList + 0x78 * (handle & 0x1FF));
		if (pawn == 0) continue;


		localPlayer.setAttack(true);
	}
}

const char* TriggerHack::GetTitle() {
	return title;
}

bool TriggerHack::HandleKeyBinds(WPARAM wParam, KBDLLHOOKSTRUCT* pKeyboard) {
	return false;
}
