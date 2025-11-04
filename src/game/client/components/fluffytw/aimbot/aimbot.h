#pragma once

#include "game/client/components/fluffytw/f_component.h"
#include <game/client/gameclient.h>

enum class TOOL {
	Hook = 0,
	Hammer = 1,
	Gun = 2,
	Shotgun = 3,
	Grenade = 4,
	Laser = 5,
};

class FAimbot : public FComponent
{
public:
	FAimbot(CGameClient *pClient) :
		FComponent(pClient) {}

	void Aimbot();
	void HookVisible(vec2 TargetPos);

	// Gets
	void GetClosestHitpoint(TOOL Tool);
	int GetClosestId(int Fov = 360, float Range = 395.f);
	[[nodiscard]] float GetPing() const;

	// Helpers
	bool PredictTool(TOOL Tool, vec2 &MyPos, vec2 MyVel, vec2 &TargetPos, vec2 TargetVel);
	bool HitScanTool(TOOL Tool, vec2 InitPos, vec2 TargetPos, vec2 ScanDir);
	bool IntersectCharacter(vec2 HookPos, vec2 TargetPos, vec2 &NewPos);

	// Scans
	vec2 EdgeScan(TOOL Tool);

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
