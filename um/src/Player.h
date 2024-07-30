#pragma once
#include "memory.h"
#include "offset.h"
#include <numbers>
#include "ImGUI/imgui.h"

struct Vector {
	Vector operator-(Vector A) {
		Vector newVector;
		newVector.x = x - A.x;
		newVector.y = y - A.y;
		newVector.z = z - A.z;
		return newVector;
	}
	Vector operator/(float A) {
		Vector newVector;
		newVector.x = x / A;
		newVector.y = y / A;
		newVector.z = z / A;
		return newVector;
	}	
	Vector operator*(float A) {
		Vector newVector;
		newVector.x = x * A;
		newVector.y = y * A;
		newVector.z = z * A;
		return newVector;
	}

	Vector operator+(Vector A) {
		Vector newVector;
		newVector.x = x + A.x;
		newVector.y = y + A.y;
		newVector.z = z + A.z;
		return newVector;
	}
	Vector operator+(ImVec2 A) {
		Vector newVector;
		newVector.x = x + A.x;
		newVector.y = y + A.y;
		newVector.z = z;
		return newVector;
	}
	float x;
	float y;
	float z;
};

struct ViewMatrix {
	float viewMatrix[4][4];
};

enum Team {
	CounterTerrorist = 3,
	Terrorist = 2
};

struct Bone {
	float data[4][2];
	
	Vector getVector() {
		return Vector(data[0][0], data[0][1], data[0][2]);
	}
};

enum BoneStruct {
	pelvis = 0,
	neck = 5,
	head = 6,
	Lshoulder = 8,
	Lelbow = 9,
	Lwrist = 10,
	Rshoulder = 13,
	Relbow = 14,
	Rwrist = 15,
	Lknee = 23,
	Lfeet = 24,
	Rknee = 26,
	Rfeet = 27
};

class Player {
public:
	static HANDLE driver;
	static uintptr_t client;
	uintptr_t playerPawn;
	uintptr_t playerController;
	uintptr_t gameSceneNode;
	uintptr_t m_pClippingWeapon;
	uintptr_t weaponName;
	uintptr_t m_szname;
	uintptr_t boneArray;
	static uintptr_t sens;

	const char* playerName;
	void init();
	const char* getName();
	int getTeam();
	uint32_t getHealth();
	Vector getPos();
	const char* getWeapon();
	void printInfo();
	uint32_t getAttack();
	void setAttack(bool value);
	Bone getBone(int boneId);

	static float getSens();
	static Vector getAng();
	static void setAng(Vector angle);
	static ViewMatrix getViewMatrix();
	static Vector worldToScreen(Vector worldPos);
};
