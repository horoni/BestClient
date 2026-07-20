#pragma once

#include <optional>

#include <game/client/component.h>

enum class EWeapon {
	Hammer = 0,
	Gun = 1,
	Shotgun = 2,
	Grenade = 3,
	Laser = 4,
	Hook = 5,
};

class CSHAimbot : public CComponent
{
public:
	int Sizeof() const override { return sizeof(*this); }
	void OnReset() override;

    struct CAimTargetInfo {
        int m_Id;
        vec2 m_Pos;
        vec2 m_AimDir;
    };

	void Aimbot();
	bool AutoLaser();

	// Gets
    std::optional<CAimTargetInfo> GetClosestTarget(EWeapon Weapon);
	int GetClosestId(EWeapon Weapon, int Fov, float Range);
	[[nodiscard]] float GetPing() const;
	[[nodiscard]] float GetExtrapolationPing() const;
	float GetWeaponReach(EWeapon Weapon);
	float GetWeaponSpeed(EWeapon Weapon);

	// Helpers
	bool PredictWeapon(EWeapon Weapon, vec2 &MyPos, vec2 MyVel, vec2 &TargetPos, vec2 TargetVel);
	bool HitScanWeapon(EWeapon Weapon, vec2 InitPos, vec2 TargetPos, vec2 ScanDir, int TargetId);
	bool IntersectCharacter(vec2 HookPos, vec2 TargetPos, vec2 &NewPos);
	bool IsPlayerFrozen(int TargetId);
	bool PlayerInWay(vec2 InitPos, vec2 TargetPos, int TargetId);

	// Scans
    std::optional<vec2> EdgeScan(EWeapon Weapon, vec2 MyPos, vec2 MyVel, vec2 TargetPos, vec2 TargetVel, int TargetId);

	// Aim
	vec2 NormalizeAim(vec2 Pos);
	void Aim(vec2 Pos);

	// Check
	bool InFov(float Fov, vec2 Dir);

	// Globals
	bool m_CanAim = true;
	bool m_LaserFired = false;
};
