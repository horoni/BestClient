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
	void HookVisible(vec2 targetPos);

	// Gets
	void GetClosestHitpoint(TOOL tool);
	int GetClosestId(int fov = 360, float range = 395.f);
	[[nodiscard]] float GetPing() const;

	// Helpers
	bool PredictTool(TOOL tool, vec2 &myPos, vec2 myVel, vec2 &targetPos, vec2 targetVel);
	bool HitScanTool(TOOL tool, vec2 InitPos, vec2 targetPos, vec2 scanDir);
	bool IntersectCharacter(vec2 hookPos, vec2 targetPos, vec2 &newPos);

	// Scans
	vec2 EdgeScan(TOOL tool);

	// Aim
	vec2 NormalizeAim(vec2 Pos);
	void Aim(vec2 Pos);

	// Check
	bool InFov(float fov, vec2 dir);

	// Globals
	bool m_CanAim = true;
	bool m_TargetVisible = false;
	int m_TargetId = 0;

	vec2 m_MyPos = vec2(0, 0);
	vec2 m_MyVel = vec2(0, 0);
	vec2 m_TargetPos = vec2(0, 0);
	vec2 m_TargetVel = vec2(0, 0);
};
