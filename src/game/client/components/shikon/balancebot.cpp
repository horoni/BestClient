#include "game/client/components/shikon/balancebot.h"
#include <game/client/components/shikon/aimbot/aimbot.h>
#include <game/client/gameclient.h>
#include <engine/shared/config.h>
#include "game/client/components/shikon/defs.h"

void CSHBalanceBot::Balance()
{
	if (!g_Config.m_ShBalance || !GameClient()->m_Snap.m_pLocalCharacter)
		return;

	const auto& Client = GameClient()->m_aClients[LOCAL_ID];
	const vec2 Pos = Client.m_Predicted.m_Pos;
	const vec2 Vel = Client.m_Predicted.m_Vel;
	const vec2 PredPos = Pos + Vel;

	const auto MayTarget = FindTarget(PredPos, GameClient()->GetTuning(0)->m_HookLength);
	if (!MayTarget.has_value()) return;

	const auto& Target = MayTarget.value();
	const float XDist = PredPos.x - Target.m_Pos.x;
	const float StopDist = 0.25f; // Not precise -> 0.5f

	if (std::abs(XDist) > StopDist)
	{
		const int Dir = (XDist > 0) ? -1 : 1;
		GameClient()->m_Controls.m_aInputData[LOCAL].m_Direction = Dir;
	} else GameClient()->m_Controls.m_aInputData[LOCAL].m_Direction = 0;
}

bool CSHBalanceBot::IsValidVerticalPosition(const vec2& TargetPos, const vec2& LocalPos) const
{
	switch(g_Config.m_ShBalanceVMode)
	{
		case 0: return TargetPos.y > LocalPos.y; // Below only
		case 1: return TargetPos.y < LocalPos.y; // Above only
		case 2: [[fallthrough]];
		default: return true; // Both
	}
}

std::optional<CSHBalanceBot::CTargetInfo> CSHBalanceBot::FindTarget(const vec2& LocalPos, float MaxDistance) const
{
	float MinDist = std::numeric_limits<float>::max();
	std::optional<CSHBalanceBot::CTargetInfo> BestTarget;

	for(int i = 0; i < MAX_CLIENTS; ++i)
	{
		if(i == GameClient()->m_Snap.m_LocalClientId)
			continue;

		const auto& Client = GameClient()->m_aClients[i];
		if(!Client.m_Active || Client.m_Team == TEAM_SPECTATORS)
			continue;

		const vec2& PlayerPos = Client.m_Predicted.m_Pos;

		if (!IsValidVerticalPosition(PlayerPos, LocalPos))
			continue;

		if(!GameClient()->m_Aimbot.HitScanWeapon(EWeapon::Hook, LocalPos, PlayerPos, PlayerPos - LocalPos))
			continue;

		const float Dist = distance(PlayerPos, LocalPos);
		if(Dist < MinDist && Dist < MaxDistance)
		{
			MinDist = Dist;
			BestTarget.emplace(i, PlayerPos, Dist);
		}
	}

	return BestTarget;
}
