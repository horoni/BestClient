#pragma once

#include "game/client/components/shikon/component.h"
#include <game/client/gameclient.h>

enum class EWeapon {
	Hammer = 0,
	Gun = 1,
	Shotgun = 2,
	Grenade = 3,
	Laser = 4,
	Hook = 5,
};

class CSHAimbot : public CSHComponent
{
public:
	CSHAimbot(CGameClient *pClient) :
		CSHComponent(pClient) {}

	void Aimbot();

	// Gets
	void GetClosestHitpoint(EWeapon Weapon);
	int GetClosestId(int Fov = 360, float Range = 395.f);
	[[nodiscard]] float GetPing() const;

	// Helpers
	bool PredictWeapon(EWeapon Weapon, vec2 &MyPos, vec2 MyVel, vec2 &TargetPos, vec2 TargetVel);
	bool HitScanWeapon(EWeapon Weapon, vec2 InitPos, vec2 TargetPos, vec2 ScanDir);
	bool IntersectCharacter(vec2 HookPos, vec2 TargetPos, vec2 &NewPos);

	// Scans
	vec2 EdgeScan(EWeapon Weapon);

	// Aim
	vec2 NormalizeAim(vec2 Pos);
	void Aim(vec2 Pos);

	// Check
	bool InFov(float Fov, vec2 Dir);

	// Globals
	bool m_CanAim = true;
	bool m_TargetVisible = false;
	int m_TargetId = 0;

	vec2 m_MyPos = vec2(0, 0);
	vec2 m_MyVel = vec2(0, 0);
	vec2 m_TargetPos = vec2(0, 0);
	vec2 m_TargetVel = vec2(0, 0);
};
