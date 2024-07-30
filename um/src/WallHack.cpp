#include "WallHack.h"

void WallHack::RenderMenu() {
	ImGui::Checkbox("Enable##WallHack", &activateWallhack);
	ImGui::Separator();
	ImGui::Checkbox("Box##Wallhack", &enableBox);
	ImGui::SameLine();
	ImGui::ColorEdit4("Colour##Box", (float*)&colourBox, ImGuiColorEditFlags_NoInputs);

	ImGui::Checkbox("Skeleton##Wallhack", &enableSkel);
	ImGui::SameLine();
	ImGui::ColorEdit4("Colour##Skeleton", (float*)&colourSkel, ImGuiColorEditFlags_NoInputs);
}

void WallHack::Tick(ImVec2 windowPos, ImVec2 windowSize) {
	if (!activateWallhack)
		return;
	for (Player player : players) {
		if (player.getTeam() == localPlayer.getTeam())
			continue;
		Vector worldPos = player.getPos();
		Vector worldPos1 = player.getPos();
		worldPos1.z += 70;

		Vector bottom = Player::worldToScreen(worldPos) + windowPos;
		Vector top = Player::worldToScreen(worldPos1) + windowPos;
		float w = (top.y - bottom.y) * 0.35f;

		if (enableBox)
			ImGui::GetBackgroundDrawList()->AddRect({ bottom.x + w, bottom.y }, { bottom.x - w, top.y }, ImGui::ColorConvertFloat4ToU32(colourBox));

		if (enableSkel)
			RenderBone(player, windowPos);

		ImGui::GetBackgroundDrawList()->AddText({ bottom.x + w, top.y - 12 }, ImGui::ColorConvertFloat4ToU32(colourBox), player.playerName);
		ImGui::GetBackgroundDrawList()->AddText({ bottom.x + w, top.y - 24 }, ImGui::ColorConvertFloat4ToU32(colourBox), player.getWeapon());
		float height = (top.y - bottom.y) * ((float)player.getHealth() / 100) + bottom.y;
		ImGui::GetBackgroundDrawList()->AddLine({ bottom.x + w - 5, height }, { bottom.x + w - 5, bottom.y }, IM_COL32(0, 255, 0, 255), 2.f);
	}
}

const char* WallHack::GetTitle() {
	return title;
}

void WallHack::RenderBone(Player player, ImVec2 windowPos) {
		Vector pelvis = Player::worldToScreen(player.getBone(BoneStruct::pelvis).getVector()) + windowPos;
		Vector neck = Player::worldToScreen(player.getBone(BoneStruct::neck).getVector()) + windowPos;
		Vector head = Player::worldToScreen(player.getBone(BoneStruct::head).getVector()) + windowPos;

		Vector Rshoulder = Player::worldToScreen(player.getBone(BoneStruct::Rshoulder).getVector()) + windowPos;
		Vector Relbow = Player::worldToScreen(player.getBone(BoneStruct::Relbow).getVector()) + windowPos;
		Vector Rwrist = Player::worldToScreen(player.getBone(BoneStruct::Rwrist).getVector()) + windowPos;

		Vector Lshoulder = Player::worldToScreen(player.getBone(BoneStruct::Lshoulder).getVector()) + windowPos;
		Vector Lelbow = Player::worldToScreen(player.getBone(BoneStruct::Lelbow).getVector()) + windowPos;
		Vector Lwrist = Player::worldToScreen(player.getBone(BoneStruct::Lwrist).getVector()) + windowPos;

		Vector Rknee = Player::worldToScreen(player.getBone(BoneStruct::Rknee).getVector()) + windowPos;
		Vector Rfeet = Player::worldToScreen(player.getBone(BoneStruct::Rfeet).getVector()) + windowPos;

		Vector Lknee = Player::worldToScreen(player.getBone(BoneStruct::Lknee).getVector()) + windowPos;
		Vector Lfeet = Player::worldToScreen(player.getBone(BoneStruct::Lfeet).getVector()) + windowPos;

		ImGui::GetBackgroundDrawList()->AddLine({ pelvis.x, pelvis.y }, { neck.x, neck.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));
		ImGui::GetBackgroundDrawList()->AddLine({ neck.x, neck.y }, { head.x, head.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));

		ImGui::GetBackgroundDrawList()->AddLine({ neck.x, neck.y }, { Rshoulder.x, Rshoulder.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));
		ImGui::GetBackgroundDrawList()->AddLine({ Relbow.x, Relbow.y }, { Rshoulder.x, Rshoulder.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));
		ImGui::GetBackgroundDrawList()->AddLine({ Relbow.x, Relbow.y }, { Rwrist.x, Rwrist.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));

		ImGui::GetBackgroundDrawList()->AddLine({ neck.x, neck.y }, { Lshoulder.x, Lshoulder.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));
		ImGui::GetBackgroundDrawList()->AddLine({ Lelbow.x, Lelbow.y }, { Lshoulder.x, Lshoulder.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));
		ImGui::GetBackgroundDrawList()->AddLine({ Lelbow.x, Lelbow.y }, { Lwrist.x, Lwrist.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));

		ImGui::GetBackgroundDrawList()->AddLine({ pelvis.x, pelvis.y }, { Rknee.x, Rknee.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));
		ImGui::GetBackgroundDrawList()->AddLine({ Rfeet.x, Rfeet.y }, { Rknee.x, Rknee.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));

		ImGui::GetBackgroundDrawList()->AddLine({ pelvis.x, pelvis.y }, { Lknee.x, Lknee.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));
		ImGui::GetBackgroundDrawList()->AddLine({ Lfeet.x, Lfeet.y }, { Lknee.x, Lknee.y }, ImGui::ColorConvertFloat4ToU32(colourSkel));
}

bool WallHack::HandleKeyBinds(WPARAM wParam, KBDLLHOOKSTRUCT* pKeyboard) {
	return false;
}
