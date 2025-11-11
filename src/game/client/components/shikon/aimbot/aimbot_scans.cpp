#include "game/client/components/shikon/aimbot/aimbot.h"
#include "game/client/components/shikon/defs.h"
#include "game/client/prediction/gameworld.h"
#include <engine/shared/config.h>

#define MAX_HITPOINTS 32

vec2 CSHAimbot::EdgeScan(EWeapon Weapon)
{
	int HitPointsCount = 0;
	vec2 HitPoints[MAX_HITPOINTS];

	vec2 MyPos = m_MyPos;
	vec2 TargetPos = m_TargetPos;

	m_TargetVisible = false;

	// Predict hook and return, if it's impossible
	if(!PredictWeapon(Weapon, MyPos, m_MyVel, TargetPos, m_TargetVel))
		return vec2(0, 0);

	// If player is hookable right away, return the position
	if(HitScanWeapon(Weapon, MyPos, TargetPos, TargetPos - MyPos))
	{
		m_TargetVisible = true;
		return TargetPos - MyPos;
	}

	// If hitpoint scan is disabled and normal scan failed, return
	if(!g_Config.m_ShAimHookEdge || Weapon != EWeapon::Hook)
		return vec2(0, 0);

	/* Gets the angle we should be able to hook
	 *
	 * a = visibleAngle 
	 *
	 * myPos
	 * |\
	 * | \
	 * |  \
	 * |__a\
	 *      targetPos
	*/
	const float VisibleAngle = atan2(TargetPos.y - MyPos.y, TargetPos.x - MyPos.x) + pi * 0.5f;
	for(float i = VisibleAngle; i < pi + VisibleAngle; i += 1.f / g_Config.m_ShAimHookEdgeAccuracy)
	{
		// Return if we have enough hitpoints
		if(HitPointsCount >= MAX_HITPOINTS)
			break;

		// Convert desired angle(hitpoint) to Cartesian coordinates
		auto Pos = vec2(static_cast<int>(TargetPos.x + cosf(i) * PHYS_SIZE),
			static_cast<int>(TargetPos.y + sinf(i) * PHYS_SIZE));
		const vec2 Dir = Pos - MyPos;

		// Check if hitpoint is hookable and if it is
		// append it to `hitPoints` and increase `hitPointsCount`
		if(HitScanWeapon(Weapon, MyPos, TargetPos, Dir))
		{
			HitPoints[HitPointsCount] = Dir;
			HitPointsCount++;
		}
	}

	// If hitpoints were found
	// return the best(i.e. middle) hitpoint position
	if(HitPointsCount > 0)
	{
		// Calculate the middle index of `hitPoints` array
		const int MiddleIndex = (HitPointsCount - 1) / 2;
		m_TargetVisible = true;
		m_TargetPos = HitPoints[MiddleIndex];
		return HitPoints[MiddleIndex];
	}
	return vec2(0, 0);
}

