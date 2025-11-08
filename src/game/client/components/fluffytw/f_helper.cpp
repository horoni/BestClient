#include <game/client/prediction/entities/character.h>

#include "f_helper.h"

#include <algorithm>

FHelper::FHelper(CGameClient *client) noexcept
	: m_pClient(client),
	m_pBots(std::make_unique<FBots>(client)),
	m_pAimbot(std::make_unique<FAimbot>(client)),
	m_pVisuals(std::make_unique<FVisuals>(client)) {}

void FHelper::TickPredict(CNetObj_Character *pCharacter, int t, vec2 *m_pPosArray)
{
	CWorldCore tmpWorld;
	auto tmpCore = CCharacterCore();
	auto tmpTeams = CTeamsCore();
	tmpCore.Init(&tmpWorld, Collision(), &tmpTeams);
	tmpCore.Read(pCharacter);

	for(int i = 0; i < t; i++)
	{
		tmpCore.Tick(false);
		tmpCore.Move();
		m_pPosArray[i] = tmpCore.m_Pos + tmpCore.m_Vel;
	}
}

int FHelper::GetCustomTile(float x, float y) const
{
	if(!m_pClient->Collision()->GetTiles())
		return 0;

	int Nx = std::clamp(static_cast<int>(x / 32), 0, m_pClient->Collision()->GetWidth() - 1);
	int Ny = std::clamp(static_cast<int>(y / 32), 0, m_pClient->Collision()->GetHeight() - 1);
	int pos = Ny * m_pClient->Collision()->GetWidth() + Nx;

	return m_pClient->Collision()->GetTiles()[pos].m_Index;
}


bool FHelper::IsGrounded(int id, vec2 pos)
{
	if(!IsValidId(id) && pos == vec2(0, 0))
		return false;
	if(pos == vec2(0, 0))
		pos = m_pClient->m_aClients[id].m_Predicted.m_Pos;
	if(m_pClient->Collision()->CheckPoint(pos.x + PHYS_SIZE / 2, pos.y + PHYS_SIZE / 2 + 5))
		return true;
	if(m_pClient->Collision()->CheckPoint(pos.x - PHYS_SIZE / 2, pos.y + PHYS_SIZE / 2 + 5))
		return true;
	int MoveRestrictionsBelow = m_pClient->Collision()->GetMoveRestrictions(pos + vec2(0, PHYS_SIZE / 2 + 4), 0.0f);
	if(MoveRestrictionsBelow & CANTMOVE_DOWN)
		return true;
	return false;
}

bool FHelper::IsLocalActive()
{
	if(Controls()->m_aInputData[LOCAL].m_PlayerFlags & PLAYERFLAG_PLAYING && Client()->State() == IClient::STATE_ONLINE)
		return true;
	return false;
}

bool FHelper::IsValidId(int id)
{
	if(id > -1 && id <= MAX_CLIENTS)
		return true;
	return false;
}
