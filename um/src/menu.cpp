#include "menu.h"

Menu::Menu() {
	hacks.push_back(new AimHack());
	hacks.push_back(new WallHack());
	hacks.push_back(new TriggerHack());
}

void Menu::Draw() {
	ImGui::Begin("Teddy Cheats", nullptr);
	withinWindow = ImGui::GetWindowViewport() == ImGui::GetMainViewport();
	if (ImGui::BeginTabBar("Hacks")) {
		for (BaseHack* hack : hacks) {
			if (ImGui::BeginTabItem(hack->GetTitle())) {
				hack->RenderMenu();
				ImGui::EndTabItem();
			}
		}
		if (ImGui::BeginTabItem("Debug")) {
			ImGui::Checkbox("Enable##Debug", &enableDebug);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();

	if (enableDebug) {
		ImGui::Begin("Debug");
		Vector pos = BaseHack::players[0].getPos();
		ImGui::Text("%f, %f, %f", pos.x, pos.y, pos.z);
		ImGui::End();
	}
}

void Menu::DrawOverlay(ImVec2 windowPos, ImVec2 windowSize) {
	ImGui::SetNextWindowSize({ 10, 10 });
	ImGui::SetNextWindowPos(windowPos);
	ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	std::thread processes[3];
	
	for (int i = 0; i < hacks.Size; i++) {
		processes[i] = std::thread(&BaseHack::Tick, &*hacks[i], windowPos, windowSize);
	}

	for (int i = 0; i < hacks.Size; i++) {
		processes[i].join();
	}
	
	ImGui::End();
}

bool Menu::HandleKeyBinds(WPARAM wParam, KBDLLHOOKSTRUCT* pKeyboard) {
	bool ret = false;
	for (BaseHack* hack : hacks) {
		if (hack->HandleKeyBinds(wParam, pKeyboard))
			ret = true;
	}

	return ret;
}
