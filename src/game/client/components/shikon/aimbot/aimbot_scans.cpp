#include <optional>

#include "game/client/components/shikon/aimbot/aimbot.h"
#include "game/client/components/shikon/defs.h"
#include <engine/shared/config.h>

#define MAX_HITPOINTS 32

std::optional<vec2> CSHAimbot::EdgeScan(EWeapon Weapon, vec2 MyPos, vec2 MyVel, vec2 TargetPos, vec2 TargetVel, int TargetId)
{
	int HitPointsCount = 0;
	vec2 HitPoints[MAX_HITPOINTS];

	vec2 PredTargetPos = TargetPos;
	vec2 PredMyPos = MyPos;

	// Predict hook and return, if it's impossible
	if(!PredictWeapon(Weapon, PredMyPos, MyVel, PredTargetPos, TargetVel))
		return std::nullopt;

	// If distance between me and player greater than hook length, we cant hook him, return
	if (Weapon == EWeapon::Hook && distance(PredMyPos, PredTargetPos) > GetWeaponReach(Weapon))
		return std::nullopt;

	// If player is hookable right away, return the position
	if(HitScanWeapon(Weapon, MyPos, PredTargetPos, PredTargetPos - MyPos, TargetId))
	{
		return PredTargetPos - MyPos;
	}

	// If hitpoint scan is disabled and normal scan failed, return
	if(!g_Config.m_ShAimHookEdge || (Weapon != EWeapon::Hook && Weapon != EWeapon::Laser))
		return std::nullopt;

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
	const float VisibleAngle = atan2(PredTargetPos.y - MyPos.y, PredTargetPos.x - MyPos.x) + pi * 0.5f;
	for(float i = VisibleAngle; i < pi + VisibleAngle; i += 1.f / g_Config.m_ShAimHookEdgeAccuracy)
	{
		// Return if we have enough hitpoints
		if(HitPointsCount >= MAX_HITPOINTS)
			break;

		// Convert desired angle(hitpoint) to Cartesian coordinates
		auto Pos = vec2(static_cast<int>(PredTargetPos.x + cosf(i) * GetPhysSize()),
			static_cast<int>(PredTargetPos.y + sinf(i) * GetPhysSize()));
		const vec2 Dir = Pos - MyPos;

		// Check if hitpoint is hookable and if it is
		// append it to `hitPoints` and increase `hitPointsCount`
		if(HitScanWeapon(Weapon, MyPos, PredTargetPos, Dir, TargetId))
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
		return HitPoints[MiddleIndex];
	}
	return std::nullopt;
}
