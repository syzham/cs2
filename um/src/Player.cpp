#include "Player.h"


void Player::init() {
	playerName = getName();
	gameSceneNode = driver::read_memory<uintptr_t>(driver,playerPawn + Offset::clientOffset["C_BaseEntity"]["fields"]["m_pGameSceneNode"].asUInt64());
	m_pClippingWeapon = driver::read_memory<uintptr_t>(driver, playerPawn + Offset::clientOffset["C_CSPlayerPawnBase"]["fields"]["m_pClippingWeapon"].asUInt64());
	weaponName = driver::read_memory<uintptr_t>(driver, m_pClippingWeapon + 0x360);
	m_szname = driver::read_memory<uintptr_t>(driver, weaponName + Offset::clientOffset["CCSWeaponBaseVData"]["fields"]["m_szName"].asUInt64());
	boneArray = driver::read_memory<uintptr_t>(driver, gameSceneNode + Offset::clientOffset["CSkeletonInstance"]["fields"]["m_modelState"].asUInt64() + 0x80);
}

int Player::getTeam() {
	int team = 3;
	uint8_t m_iTeamNum = driver::read_memory<std::uint8_t>(driver, playerPawn + Offset::clientOffset["C_BaseEntity"]["fields"]["m_iTeamNum"].asUInt64());
	return (int)m_iTeamNum;
}

const char* Player::getName() {
	const char* m_iszPlayerName = driver::read_string32(driver, playerController + Offset::clientOffset["CBasePlayerController"]["fields"]["m_iszPlayerName"].asUInt64());
	return m_iszPlayerName;
}

uint32_t Player::getHealth() {
	uint32_t m_iHealth = driver::read_memory<std::uint32_t>(driver, playerPawn + Offset::clientOffset["C_BaseEntity"]["fields"]["m_iHealth"].asUInt64());
	return m_iHealth;
}

Vector Player::getPos() {
	Vector pos = driver::read_memory<Vector>(driver, gameSceneNode + Offset::clientOffset["CGameSceneNode"]["fields"]["m_vecAbsOrigin"].asUInt64());
	return pos;
}

Vector Player::getAng() {
	Vector ang = driver::read_memory<Vector>(driver, client + Offset::offset["client.dll"]["dwViewAngles"].asUInt64());
	return ang;
}

float Player::getSens() {
	return driver::read_memory<float>(driver, sens + Offset::offset["client.dll"]["dwSensitivity_sensitivity"].asUInt64());
}

void Player::setAng(Vector angle) {
	angle = angle - getAng();
	mouse_event(MOUSEEVENTF_MOVE, -(int)(angle.y / (0.022 * getSens())), (int)(angle.x / (0.022 * getSens())), 0, 0);
}

uint32_t Player::getAttack() {
	return driver::read_memory<uint32_t>(driver, client + 25284960);
}

void Player::setAttack(bool value) {
	if (value)
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	else
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

const char* Player::getWeapon() {
		const char* name = driver::read_string32(driver, m_szname, 7);

	return name;
}

ViewMatrix Player::getViewMatrix() {
	ViewMatrix dwViewMatrix = driver::read_memory<ViewMatrix>(driver, client + Offset::offset["client.dll"]["dwViewMatrix"].asUInt64());
	return dwViewMatrix;
}

Bone Player::getBone(int boneId) {
	Bone bone = driver::read_memory<Bone>(driver, boneArray + (boneId * 32));
	return bone;
}

Vector Player::worldToScreen(Vector worldPos) {
	ViewMatrix vm = Player::getViewMatrix();
	Vector screenPos;
	screenPos.x = vm.viewMatrix[0][0] * worldPos.x + vm.viewMatrix[0][1] * worldPos.y + vm.viewMatrix[0][2] * worldPos.z + vm.viewMatrix[0][3];
	screenPos.y = vm.viewMatrix[1][0] * worldPos.x + vm.viewMatrix[1][1] * worldPos.y + vm.viewMatrix[1][2] * worldPos.z + vm.viewMatrix[1][3];
	float w = vm.viewMatrix[3][0] * worldPos.x + vm.viewMatrix[3][1] * worldPos.y + vm.viewMatrix[3][2] * worldPos.z + vm.viewMatrix[3][3];

	if (w > 0.01f) {
		screenPos.x /= w;
		screenPos.y /= w;

		const ImVec2 size = ImGui::GetIO().DisplaySize;
		screenPos.x = (size.x / 2) + (screenPos.x * size.x) / 2;
		screenPos.y = (size.y / 2) - (screenPos.y * size.y) / 2;
	}
	else {
		screenPos.x = 1000;
		screenPos.y = 1000;
	}
	return screenPos;
}

void Player::printInfo() {
	//Vector pos = getAng();
	//ImGui::Text("Pos: %f, %f, %f", pos.x, pos.y, pos.z);
	uint32_t s = driver::read_memory<uint32_t>(driver, client + 25205952);
	ImGui::Text("%d", s);
}