#include "game/client/prediction/entities/character.h"
#include "game/client/components/fluffytw/f_helper.h"
#include <game/client/components/fluffytw/f_component.h>
#include "aimbot.h"

#define INSTANT_SPEED 10000.f

void FAimbot::Aimbot()
{
	if(!g_Config.m_ShAim)
		return;

	EWeapon Weapon;
	if (m_pClient->m_Snap.m_pLocalCharacter)
		Weapon = (EWeapon)m_pClient->m_Snap.m_pLocalCharacter->m_Weapon;
	else return;

	if (g_Config.m_ShAimLaserAuto && Weapon == EWeapon::Laser)
	{
		GetClosestHitpoint(EWeapon::Laser);
		Aim(NormalizeAim(m_TargetPos));
		Controls()->m_aInputData[LOCAL].m_Fire = 1;
	}

	if(Controls()->m_aInputData[LOCAL].m_Hook == 1 && g_Config.m_ShAimHook)
	{
		GetClosestHitpoint(EWeapon::Hook);
		Aim(NormalizeAim(m_TargetPos));
	} else if (Controls()->m_aInputData[LOCAL].m_Fire % 2 == 1)
	{
		if (g_Config.m_ShAimLaserAuto && Weapon == EWeapon::Laser)
			return;

		GetClosestHitpoint(Weapon);

		if (g_Config.m_ShDbg) {
			fHelper->dbg_msg("bot", "bot: fire = %d; cursor w = %d; snap w = %d",
				Controls()->m_aInputData[LOCAL].m_Fire,
				m_pClient->m_CursorInfo.Weapon(),
				m_pClient->m_Snap.m_pLocalCharacter ?
				m_pClient->m_Snap.m_pLocalCharacter->m_Weapon : -1
			);
			fHelper->dbg_msg("bot", "bot: closest_hitpoint = %f %f",
				m_TargetPos.x, m_TargetPos.y);
		}

		if ( (Weapon == EWeapon::Hammer && g_Config.m_ShAimHammer)
			|| (Weapon == EWeapon::Gun && g_Config.m_ShAimGun)
			|| (Weapon == EWeapon::Shotgun && g_Config.m_ShAimShotgun)
			|| (Weapon == EWeapon::Grenade && g_Config.m_ShAimGrenade)
			|| (Weapon == EWeapon::Laser && g_Config.m_ShAimLaser)
		) {
			Aim(NormalizeAim(m_TargetPos));
		}
	}
	else
		m_CanAim = true;
}

void FAimbot::GetClosestHitpoint(EWeapon Weapon)
{
	switch(Weapon) {
		case EWeapon::Hook:
			m_TargetId = GetClosestId(g_Config.m_ShAimHookFov);
			break;
		case EWeapon::Hammer:
			m_TargetId = GetClosestId(g_Config.m_ShAimHammerFov, 21.f);
			break;
		case EWeapon::Gun:
			m_TargetId = GetClosestId(g_Config.m_ShAimGunFov, 815.f);
			break;
		case EWeapon::Shotgun:
			if (m_pClient->m_GameWorld.m_WorldConfig.m_IsDDRace)
				m_TargetId = GetClosestId(g_Config.m_ShAimShotgunFov, Tuning()->m_LaserReach + 15.f);
			else m_TargetId = GetClosestId(g_Config.m_ShAimShotgunFov, 415.f);
			break;
		case EWeapon::Grenade:
			// TODO(horoni): Implement grenade prediction
			m_TargetId = GetClosestId(g_Config.m_ShAimGrenadeFov, 1.f);
			break;
		case EWeapon::Laser:
			// Range is shorter than 815 but we predicting
			m_TargetId = GetClosestId(g_Config.m_ShAimLaserFov, Tuning()->m_LaserReach + 15.f);
			break;
	}

	if(!fHelper->IsValidId(m_TargetId))
	{
		if (g_Config.m_ShDbg)
			fHelper->dbg_msg("bot", "bot: InvalidClosestID");
		m_TargetPos = vec2(0.f, 0.f);
		m_TargetVel = vec2(0, 0);
		m_TargetVisible = false;
		m_TargetId = -1;
		return;
	}

	if (g_Config.m_ShDbg)
		fHelper->dbg_msg("bot", "bot: ValidClosestID");
	m_MyPos = m_pClient->m_PredictedChar.m_Pos;
	m_MyVel = m_pClient->m_PredictedChar.m_Vel;
	m_TargetPos = m_pClient->m_aClients[m_TargetId].m_Predicted.m_Pos;
	m_TargetVel = m_pClient->m_aClients[m_TargetId].m_Predicted.m_Vel;
	m_TargetPos = EdgeScan(Weapon);
}

int FAimbot::GetClosestId(int Fov, float Range)
{
	const vec2 Pos = m_pClient->m_PredictedChar.m_Pos;
	float Distance = Range;
	int ClosestID = -1;

	const CGameClient::CClientData OwnClientData = m_pClient->m_aClients[LOCAL_ID];

	auto *Player = dynamic_cast<CCharacter *>(m_pClient->m_GameWorld.FindFirst(m_pClient->m_GameWorld.ENTTYPE_CHARACTER));
	for(; Player; Player = dynamic_cast<CCharacter *>(Player->TypeNext()))
	{
		int i = Player->GetId();
		if(i == LOCAL_ID || !Player)
			continue;

		const CGameClient::CClientData ClData = m_pClient->m_aClients[i];
		if(!ClData.m_Active)
			continue;
		vec2 Position = m_pClient->m_aClients[i].m_Predicted.m_Pos;

		const bool IsOneSolo = ClData.m_Solo || OwnClientData.m_Solo;
		const bool IsOneSpec = ClData.m_Spec || OwnClientData.m_Spec;

		if(IsOneSpec || IsOneSolo)
			continue;

		if(!m_pClient->m_Teams.SameTeam(i, LOCAL_ID) || OwnClientData.m_HookHitDisabled)
			continue;

		if(!InFov(Fov, Position - Pos))
			continue;

		// FNG: Skip if Tee is frozen and current weapon is Laser
		if((m_pClient->m_GameWorld.m_WorldConfig.m_IsFNG || g_Config.m_ShAimForceFng)
				&& m_pClient->m_Snap.m_pLocalCharacter->m_Weapon == (int)EWeapon::Laser
				&& m_pClient->m_aClients[i].m_LiveFrozen)
				continue;

		if(ClosestID != -1 && GameWorld()->m_GameTick % 150 != 0)
			return ClosestID;

		// FIX?: Only if Weapon is Hook?
		static int s_LastHookedId = m_pClient->m_Snap.m_pLocalCharacter->m_HookedPlayer;
		if(fHelper->IsValidId(s_LastHookedId)
				&& length(m_pClient->m_aClients[s_LastHookedId].m_Predicted.m_Pos - ClData.m_Predicted.m_Pos) < Tuning()->m_HookLength + PHYS_SIZE * 0.5f)
			return ClosestID;

		if(ClosestID == -1 && distance(Pos, Position) < Distance)
		{
			ClosestID = i;
			Distance = distance(Pos, Position);
		}
	}
	return ClosestID;
}

float FAimbot::GetPing() const
{
	const auto RealPing = static_cast<float>(Client()->GetPredictionTime());
	const float Ping = RealPing / 100.f;
	return Ping;
}

// <><><> Helpers <><><><><>

bool FAimbot::PredictWeapon(EWeapon Weapon, vec2 &MyPos, vec2 MyVel, vec2 &TargetPos, vec2 TargetVel)
{
	const vec2 Delta = TargetPos - MyPos;
	const vec2 DeltaVel = TargetVel - MyVel;

	float WSpeed;
	switch(Weapon) {
		case EWeapon::Hook:
			WSpeed = Tuning()->m_HookFireSpeed;
			break;
		case EWeapon::Hammer:
			WSpeed = INSTANT_SPEED;
			break;
		case EWeapon::Gun:
			WSpeed = Tuning()->m_GunSpeed;
			break;
		case EWeapon::Shotgun:
			// TODO(horoni): Maybe there is a better way to detect shotgun mode? 
			if (m_pClient->m_GameWorld.m_WorldConfig.m_IsDDRace)
				WSpeed = INSTANT_SPEED;
			else WSpeed = Tuning()->m_ShotgunSpeed;
			break;
		case EWeapon::Grenade:
			return false;
			break;
		case EWeapon::Laser:
			WSpeed = INSTANT_SPEED;
			break;
	}

	const float WeaponSpeed = length(TargetVel) + WSpeed;
	const float a = dot(DeltaVel, DeltaVel) - powf(WeaponSpeed, 2);

	const float b = 2.f * dot(DeltaVel, Delta);
	const float c = dot(Delta, Delta);

	const float Sol = powf(b, 2) - 4.f * a * c;
	if(Sol > 0.f)
	{
		// qTime is the same as time
		// const float qTime = (-sqrt(sol) - b) / (2 * a);
		const float Time = abs(2.f * c / (sqrtf(Sol) - b)) + GetPing();
		TargetPos += TargetVel * Time;
		return true;
	}
	return false;
}


bool FAimbot::HitScanWeapon(EWeapon Weapon, vec2 InitPos, vec2 TargetPos, vec2 ScanDir)
{
	float WSpeed;
	float WReach;

	switch(Weapon) {
		case EWeapon::Hook:
			WSpeed = Tuning()->m_HookFireSpeed;
			WReach = Tuning()->m_HookLength;
			break;
		case EWeapon::Hammer:
			// FIX: Hammer reach
			WSpeed = INSTANT_SPEED;
			WReach = 20.f; // I dont know how much really
		case EWeapon::Gun:
			// FIX: Gun reach
			WSpeed = Tuning()->m_GunSpeed;
			WReach = 800.f;
			break;
		case EWeapon::Shotgun:
			// TODO(horoni): Maybe there is a better way to detect shotgun mode? 
			if (m_pClient->m_GameWorld.m_WorldConfig.m_IsDDRace) {
				WSpeed = INSTANT_SPEED;
				WReach = Tuning()->m_LaserReach;
			} else {
				// FIX?: Reach distance
				WSpeed = Tuning()->m_ShotgunSpeed;
				WReach = 400.f;
			}
			break;
		case EWeapon::Grenade:
			return false;
			break;
		case EWeapon::Laser:
			WSpeed = INSTANT_SPEED;
			WReach = Tuning()->m_LaserReach;
			break;
	}

	vec2 ExDirection = normalize(ScanDir);
	vec2 FinishPos = InitPos + ExDirection * (WReach - PHYS_SIZE * 1.5f);

	vec2 OldPos = InitPos + ExDirection * PHYS_SIZE * 1.5f;
	vec2 NewPos = OldPos;

	bool DoBreak = false;

	do
	{
		OldPos = NewPos;
		NewPos = OldPos + ExDirection * WSpeed;

		if(distance(InitPos, NewPos) > WReach)
		{
			NewPos = InitPos + normalize(NewPos - InitPos) * WReach;
			DoBreak = true;
		}

		// TODO(horoni): Can it be used with any weapon?
		int TeleNr = 0;
		const int Hit = Collision()->IntersectLineTeleHook(OldPos, NewPos, &FinishPos, nullptr, &TeleNr);

		if(IntersectCharacter(OldPos, TargetPos, FinishPos))
			return true;

		if(Hit)
			break;

		NewPos.x = round_to_int(NewPos.x);
		NewPos.y = round_to_int(NewPos.y);

		if(OldPos == NewPos)
			break;

		ExDirection.x = round_to_int(ExDirection.x * 256.0f) / 256.0f;
		ExDirection.y = round_to_int(ExDirection.y * 256.0f) / 256.0f;
	} while(!DoBreak);
	return false;
}

bool FAimbot::IntersectCharacter(vec2 HookPos, vec2 TargetPos, vec2 &NewPos)
{
	vec2 ClosestPoint;
	if(closest_point_on_line(HookPos, NewPos, TargetPos, ClosestPoint))
	{
		if(distance(TargetPos, ClosestPoint) < PHYS_SIZE + 2.f)
		{
			NewPos = ClosestPoint;
			return true;
		}
	}
	return false;
}

// Aim
vec2 FAimbot::NormalizeAim(vec2 Pos)
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

void FAimbot::Aim(vec2 Pos)
{
	if(!m_CanAim)
		return;

	// Aimbot will aim -> update `m_CanAim`
	m_CanAim = false;

	if(!m_TargetVisible)
		return;

	// Aim using desired way
	if(!g_Config.m_ShAimSilent)
	{
		Controls()->m_aMousePos[LOCAL] = Pos;
		Controls()->m_aInputData[LOCAL].m_TargetX = static_cast<int>(Controls()->m_aMousePos[LOCAL].x);
		Controls()->m_aInputData[LOCAL].m_TargetY = static_cast<int>(Controls()->m_aMousePos[LOCAL].y);
	}
	else
	{
		Controls()->m_aInputData[LOCAL].m_TargetX = static_cast<int>(Pos.x);
		Controls()->m_aInputData[LOCAL].m_TargetY = static_cast<int>(Pos.y);
	}
}

bool FAimbot::InFov(float Fov, vec2 Dir)
{
	const float DifferenceAngle = abs(atan2(sin(angle(Dir) - angle(Controls()->m_aMousePos[LOCAL])),
		cos(angle(Dir) - angle(Controls()->m_aMousePos[LOCAL])))) * 100.f;
	if(DifferenceAngle > Fov)
		return false;
	return true;
}
