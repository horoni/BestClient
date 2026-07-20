#include "game/client/components/shikon/aimbot/aimbot.h"
#include "game/client/components/shikon/defs.h"
#include "game/client/prediction/entities/character.h"
#include <game/client/gameclient.h>

#include <cmath>

#define INSTANT_SPEED 10000.f

void CSHAimbot::Aimbot()
{
	if(!g_Config.m_ShAim)
		return;

	EWeapon Weapon;
	if (GameClient()->m_Snap.m_pLocalCharacter)
		Weapon = (EWeapon)GameClient()->m_Snap.m_pLocalCharacter->m_Weapon;
	else return;

	if (g_Config.m_ShAimLaserAuto && Weapon == EWeapon::Laser)
	{
		if (AutoLaser())
			return;
	}

	const int LocalDataId = GetLocalData();

	if(GameClient()->m_Controls.m_aInputData[LocalDataId].m_Hook == 1 && g_Config.m_ShAimHook)
	{
		auto Target = GetClosestTarget(EWeapon::Hook);
		if (Target.has_value())
			Aim(NormalizeAim(Target->m_AimDir));
	} else if (GameClient()->m_Controls.m_aInputData[LocalDataId].m_Fire % 2 == 1)
	{
		if (g_Config.m_ShAimLaserAuto && Weapon == EWeapon::Laser)
			return;

		auto Target = GetClosestTarget(Weapon);

		if (g_Config.m_ShDbg && Target.has_value()) {
			GameClient()->m_Helper.dbg_msg("bot", "bot: fire = %d; cursor w = %d; snap w = %d",
				GameClient()->m_Controls.m_aInputData[LocalDataId].m_Fire,
				GameClient()->m_CursorInfo.Weapon(),
				GameClient()->m_Snap.m_pLocalCharacter ?
				GameClient()->m_Snap.m_pLocalCharacter->m_Weapon : -1
			);
			GameClient()->m_Helper.dbg_msg("bot", "bot: closest_hitpoint = %f %f",
				Target->m_AimDir.x, Target->m_AimDir.y);
		}

		if ( Target.has_value() &&
			 ((Weapon == EWeapon::Hammer && g_Config.m_ShAimHammer)
			|| (Weapon == EWeapon::Gun && g_Config.m_ShAimGun)
			|| (Weapon == EWeapon::Shotgun && g_Config.m_ShAimShotgun)
			|| (Weapon == EWeapon::Grenade && g_Config.m_ShAimGrenade)
			|| (Weapon == EWeapon::Laser && g_Config.m_ShAimLaser)
			 )) {
			Aim(NormalizeAim(Target->m_AimDir));
		}
	}
	else
		m_CanAim = true;
}

bool CSHAimbot::AutoLaser()
{
	static bool s_Fired = false;
	const int LocalDataId = GetLocalData();

	if (s_Fired)
	{
		GameClient()->m_Controls.m_aLastData[LocalDataId].m_Fire = 0;
		GameClient()->m_Controls.m_aInputData[LocalDataId].m_Fire = 0;
		s_Fired = false;
		return false;
	}

	if (GameClient()->m_GameWorld.GetCharacterById(GetLocalId(GameClient()))->GetReloadTimer() > 0)
		return false;

	auto Target = GetClosestTarget(EWeapon::Laser);
	if (!Target.has_value())
		return false;

	Aim(NormalizeAim(Target->m_AimDir));
	GameClient()->m_Controls.m_aInputData[LocalDataId].m_Fire = 1;
	s_Fired = true;

	return true;
}

std::optional<CSHAimbot::CAimTargetInfo>
CSHAimbot::GetClosestTarget(EWeapon Weapon)
{
	const int LocalId = GetLocalId(GameClient());
	int TargetId = -1;
	float WReach = GetWeaponReach(Weapon) + 15.f;

	switch(Weapon) {
		case EWeapon::Hook:
			TargetId = GetClosestId(Weapon, g_Config.m_ShAimHookFov, WReach);
			break;
		case EWeapon::Hammer:
			TargetId = GetClosestId(Weapon, g_Config.m_ShAimHammerFov, WReach);
			break;
		case EWeapon::Gun:
			TargetId = GetClosestId(Weapon, g_Config.m_ShAimGunFov, WReach);
			break;
		case EWeapon::Shotgun:
			if (GameClient()->m_GameWorld.m_WorldConfig.m_IsDDRace)
				TargetId = GetClosestId(Weapon, g_Config.m_ShAimShotgunFov, GetWeaponReach(EWeapon::Laser) + 15.f);
			else TargetId = GetClosestId(Weapon, g_Config.m_ShAimShotgunFov, WReach);
			break;
		case EWeapon::Grenade:
			// TODO(horoni): Implement grenade prediction
			TargetId = GetClosestId(Weapon, g_Config.m_ShAimGrenadeFov, 1.f);
			break;
		case EWeapon::Laser:
			TargetId = GetClosestId(Weapon, g_Config.m_ShAimLaserFov, WReach);
			break;
	}

	if(!GameClient()->m_Helper.IsValidId(TargetId) || !GameClient()->m_Snap.m_pLocalCharacter)
	{
		if (g_Config.m_ShDbg)
			GameClient()->m_Helper.dbg_msg("bot", "bot: InvalidClosestID");
		return std::nullopt;
	}

	if (g_Config.m_ShDbg)
		GameClient()->m_Helper.dbg_msg("bot", "bot: ValidClosestID");

	const vec2 MyPos = GameClient()->m_aClients[LocalId].m_Active ?
		GameClient()->m_aClients[LocalId].m_Predicted.m_Pos : vec2(0.f, 0.f);
	const vec2 MyVel = GameClient()->m_aClients[LocalId].m_Active ?
		GameClient()->m_aClients[LocalId].m_Predicted.m_Vel : vec2(0.f, 0.f);

	const vec2 TargetPos = GameClient()->m_aClients[TargetId].m_Predicted.m_Pos;
	const vec2 TargetVel = GameClient()->m_aClients[TargetId].m_Predicted.m_Vel;

	std::optional<vec2> AimDir = EdgeScan(Weapon, MyPos, MyVel, TargetPos, TargetVel, TargetId);

	if (!AimDir.has_value()) {
		return std::nullopt;
	}

	return CAimTargetInfo{TargetId, TargetPos, AimDir.value()};
}

int CSHAimbot::GetClosestId(EWeapon Weapon, int Fov, float Range)
{
	const int LocalId = GetLocalId(GameClient());
	const vec2 MyPos = GameClient()->m_aClients[LocalId].m_Active ?
		GameClient()->m_aClients[LocalId].m_Predicted.m_Pos : vec2(0.f, 0.f);
	float Distance = Range;
	int ClosestID = -1;

	bool IsFNG = GameClient()->m_GameWorld.m_WorldConfig.m_IsFNG || g_Config.m_ShAimForceFng;

	const CGameClient::CClientData OwnClientData = GameClient()->m_aClients[LocalId];

	auto *Player = dynamic_cast<CCharacter *>(GameClient()->m_GameWorld.FindFirst(GameClient()->m_GameWorld.ENTTYPE_CHARACTER));
	for(; Player; Player = dynamic_cast<CCharacter *>(Player->TypeNext()))
	{
		int i = Player->GetId();
		if(i == LocalId)
			continue;

		const CGameClient::CClientData ClData = GameClient()->m_aClients[i];
		if(!ClData.m_Active)
			continue;
		vec2 Position = GameClient()->m_aClients[i].m_Predicted.m_Pos;

		const bool IsOneSolo = ClData.m_Solo || OwnClientData.m_Solo;
		const bool IsOneSpec = ClData.m_Spec || OwnClientData.m_Spec;

		if(IsOneSpec || IsOneSolo)
			continue;

		if(Weapon == EWeapon::Hook && OwnClientData.m_HookHitDisabled)
			continue;

		bool IsSameTeam = GameClient()->m_Teams.SameTeam(i, LocalId);
		if(!IsFNG && !IsSameTeam)
			continue;

		if(!InFov(Fov, Position - MyPos))
			continue;

		const bool IsFrozen = IsPlayerFrozen(i);
		if (g_Config.m_ShDbg)
			GameClient()->m_Helper.dbg_msg("bot", "bot: w:%d i:%d fr:%d fng:%d", Weapon, i, IsFrozen, IsFNG);
		// FNG: Skip if Tee is frozen and current weapon is Laser
		if (Weapon == EWeapon::Laser) {
			if(IsFNG && IsFrozen)
				continue;
			else if ((GameClient()->m_GameWorld.m_WorldConfig.m_IsDDRace && !IsFNG) && !IsFrozen)
				continue;
		}

		if(distance(MyPos, Position) < Distance)
		{
			ClosestID = i;
			Distance = distance(MyPos, Position);
		}
	}
	return ClosestID;
}

float CSHAimbot::GetPing() const
{
	const auto RealPing = static_cast<float>(Client()->GetPredictionTime());
	const auto TickSpeed = static_cast<float>(Client()->GameTickSpeed());
	const float Ping = RealPing / 1000.f * TickSpeed;
	return Ping;
}

float CSHAimbot::GetExtrapolationPing() const
{
	if (g_Config.m_ClAntiPing && g_Config.m_ClAntiPingPlayers)
		return 0.f;
	return GetPing();
}

// <><><> Helpers <><><><><>

bool CSHAimbot::PredictWeapon(EWeapon Weapon, vec2 &MyPos, vec2 MyVel, vec2 &TargetPos, vec2 TargetVel)
{
	float WSpeed = GetWeaponSpeed(Weapon);
	TargetPos += TargetVel * GetExtrapolationPing();

	if (WSpeed >= INSTANT_SPEED) {
		return true;
	}

	const vec2 Delta = TargetPos - MyPos;
	const vec2 DeltaVel = TargetVel;

	const float a = dot(DeltaVel, DeltaVel) - (WSpeed * WSpeed);
	const float b = 2.f * dot(DeltaVel, Delta);
	const float c = dot(Delta, Delta);

	const float Sol = (b * b) - 4.f * a * c;
	if (Sol > 0.f)
	{
		float t1 = (-b - sqrtf(Sol)) / (2.f * a);
		float t2 = (-b + sqrtf(Sol)) / (2.f * a);

		float Time = -1.f;
		if (t1 > 0.f && t2 > 0.f) Time = minimum(t1, t2);
		else if (t1 > 0.f) Time = t1;
		else if (t2 > 0.f) Time = t2;

		if (Time > 0.f) {
			TargetPos += TargetVel * Time;
			MyPos += MyVel * Time;
			return true;
		}
	}
	return false;
}


bool CSHAimbot::HitScanWeapon(EWeapon Weapon, vec2 InitPos, vec2 TargetPos, vec2 ScanDir, int TargetId)
{
	float WReach = GetWeaponReach(Weapon);

	if (Weapon == EWeapon::Hammer) {
		return distance(InitPos, TargetPos) <= WReach;
	}
	if (TargetId != -1 && PlayerInWay(InitPos, TargetPos, TargetId))
		return false;

	vec2 Dir = normalize(ScanDir);
	if (length(Dir) < 0.1f)
		return false;

	vec2 EndPos = InitPos + Dir * WReach;
	vec2 HitPos;
	int TeleNr = 0;
	int Hit = 0;

	if (Weapon == EWeapon::Hook) {
		Hit = Collision()->IntersectLineTeleHook(InitPos, EndPos, &HitPos, nullptr, &TeleNr);
	} else {
		Hit = Collision()->IntersectLineTeleWeapon(InitPos, EndPos, &HitPos, nullptr, &TeleNr);
	}

	vec2 CharHitPos = Hit ? HitPos : EndPos;
	return IntersectCharacter(InitPos, TargetPos, CharHitPos);
}

bool CSHAimbot::IntersectCharacter(vec2 HookPos, vec2 TargetPos, vec2 &NewPos)
{
	vec2 ClosestPoint;
	if(closest_point_on_line(HookPos, NewPos, TargetPos, ClosestPoint))
	{
		if(distance(TargetPos, ClosestPoint) <= GetPhysSize())
		{
			NewPos = ClosestPoint;
			return true;
		}
	}
	return false;
}

bool CSHAimbot::IsPlayerFrozen(int TargetId)
{
	const auto& ClData = GameClient()->m_aClients[TargetId];
	int CurTick = Client()->PredGameTick(GetLocalData());

	if (ClData.m_Predicted.m_FreezeEnd > CurTick || ClData.m_Predicted.m_IsInFreeze)
		return true;

	if (GameClient()->m_Snap.m_aCharacters[TargetId].m_Active)
	{
		const auto& Cur = GameClient()->m_Snap.m_aCharacters[TargetId].m_Cur;
		bool IsFNG = GameClient()->m_GameWorld.m_WorldConfig.m_IsFNG || g_Config.m_ShAimForceFng;
		if (IsFNG)
			if (Cur.m_Weapon == 5 || Cur.m_Armor > 0)
				return true;
	}

	return false;
}

bool CSHAimbot::PlayerInWay(vec2 InitPos, vec2 TargetPos, int TargetId)
{
	const int LocalId = GetLocalId(GameClient());

	auto *Player = dynamic_cast<CCharacter *>(GameClient()->m_GameWorld.FindFirst(GameClient()->m_GameWorld.ENTTYPE_CHARACTER));
	for(; Player; Player = dynamic_cast<CCharacter *>(Player->TypeNext()))
	{
		int i = Player->GetId();
		if(i == LocalId || i == TargetId || !Player)
			continue;

		const CGameClient::CClientData ClData = GameClient()->m_aClients[i];

		if(!ClData.m_Active || ClData.m_Team == TEAM_SPECTATORS)
			continue;

		vec2 PlayerPos = ClData.m_Predicted.m_Pos;

		vec2 ClosestPoint;
		if(closest_point_on_line(InitPos, TargetPos, PlayerPos, ClosestPoint))
		{
			if(distance(PlayerPos, ClosestPoint) < GetPhysSize() + 1.f)
			{
				float DistToPlayer = distance(InitPos, PlayerPos);
				float DistToTarget = distance(InitPos, TargetPos);

				if (DistToPlayer < DistToTarget)
				{
					return true;
				}
			}
		}
	}
	return false;
}

// Aim
vec2 CSHAimbot::NormalizeAim(vec2 Pos)
{
	constexpr float CameraMaxDistance = 200.f;
	const float FollowFactor = (g_Config.m_ClDyncam ? g_Config.m_ClDyncamFollowFactor : g_Config.m_ClMouseFollowfactor) / 100.f;
	const float DeadZone = g_Config.m_ClDyncam ? g_Config.m_ClDyncamDeadzone : g_Config.m_ClMouseDeadzone;
	const float MaxDistance = g_Config.m_ClMouseMaxDistance;
	const float MouseMax = minimum((FollowFactor != 0.f ? CameraMaxDistance / FollowFactor + DeadZone : MaxDistance), MaxDistance);
	const float MDistance = length(Pos);
	Pos = normalize_pre_length(Pos, MDistance) * MouseMax;
	Pos = vec2(static_cast<int>(Pos.x), static_cast<int>(Pos.y));
	return Pos;
}

void CSHAimbot::Aim(vec2 Pos)
{
	if(!m_CanAim)
		return;
	const int LocalDataId = GetLocalData();

	// Aimbot will aim -> update `m_CanAim`
	m_CanAim = false;

	// Aim using desired way
	if(!g_Config.m_ShAimSilent)
	{
		GameClient()->m_Controls.m_aMousePos[LocalDataId] = Pos;
		GameClient()->m_Controls.m_aInputData[LocalDataId].m_TargetX = static_cast<int>(GameClient()->m_Controls.m_aMousePos[LocalDataId].x);
		GameClient()->m_Controls.m_aInputData[LocalDataId].m_TargetY = static_cast<int>(GameClient()->m_Controls.m_aMousePos[LocalDataId].y);
	}
	else
	{
		GameClient()->m_Controls.m_aInputData[LocalDataId].m_TargetX = static_cast<int>(Pos.x);
		GameClient()->m_Controls.m_aInputData[LocalDataId].m_TargetY = static_cast<int>(Pos.y);
	}
}

bool CSHAimbot::InFov(float Fov, vec2 Dir)
{
	const int LocalDataId = GetLocalData();
	const vec2 MousePos = GameClient()->m_Controls.m_aMousePos[LocalDataId];
	float MouseAngle = angle(MousePos);
	float DirAngle = angle(Dir);
	float Diff = std::abs(MouseAngle - DirAngle);

	while(Diff > pi) Diff -= 2.f * pi;
	while(Diff < -pi) Diff += 2.f * pi;
	Diff = std::abs(Diff);

	float DiffDeg = Diff * 180.f / pi;
	return DiffDeg <= (Fov / 2.f);
}

float CSHAimbot::GetWeaponReach(EWeapon Weapon)
{
	const int LocalId = GetLocalId(GameClient());
	const vec2 MyPos = GameClient()->m_aClients[LocalId].m_Active ?
		GameClient()->m_aClients[LocalId].m_Predicted.m_Pos : vec2(0.f, 0.f);
	const CTuningParams* pTuning = GameClient()->m_Helper.GetTuningAt(MyPos);

	switch (Weapon) {
		case EWeapon::Hook: return pTuning->m_HookLength;
		case EWeapon::Hammer: return 63.f;
		case EWeapon::Gun: return pTuning->m_GunSpeed * pTuning->m_GunLifetime;
		// TODO(horoni): Maybe there is a better way to detect shotgun mode?
		case EWeapon::Shotgun: return GameClient()->m_GameWorld.m_WorldConfig.m_IsDDRace ? pTuning->m_LaserReach : 400.f;
		case EWeapon::Grenade: return pTuning->m_GrenadeSpeed * pTuning->m_GrenadeLifetime;
		case EWeapon::Laser:
			if (pTuning->m_LaserReach < 1.f)
				return 800.f;
			return pTuning->m_LaserReach;
	}
}

float CSHAimbot::GetWeaponSpeed(EWeapon Weapon)
{
	const int LocalId = GetLocalId(GameClient());
	const vec2 MyPos = GameClient()->m_aClients[LocalId].m_Active ?
		GameClient()->m_aClients[LocalId].m_Predicted.m_Pos : vec2(0.f, 0.f);
	const CTuningParams* pTuning = GameClient()->m_Helper.GetTuningAt(MyPos);

	switch (Weapon) {
		case EWeapon::Hook: return pTuning->m_HookFireSpeed;
		case EWeapon::Hammer: return INSTANT_SPEED;
		case EWeapon::Gun: return pTuning->m_GunSpeed;
		// TODO(horoni): Maybe there is a better way to detect shotgun mode?
		case EWeapon::Shotgun: return GameClient()->m_GameWorld.m_WorldConfig.m_IsDDRace ? INSTANT_SPEED : pTuning->m_ShotgunSpeed;
		case EWeapon::Grenade: return pTuning->m_GrenadeSpeed;
		case EWeapon::Laser: return INSTANT_SPEED;
	}
}
