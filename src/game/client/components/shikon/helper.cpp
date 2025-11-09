#include "game/client/components/shikon/helper.h"

#include <algorithm>
#include <game/client/prediction/entities/character.h>

CSHHelper::CSHHelper(CGameClient *Client) noexcept
	: m_pClient(Client),
	m_pBots(std::make_unique<CSHBots>(Client)),
	m_pAimbot(std::make_unique<CSHAimbot>(Client)),
	m_pVisuals(std::make_unique<CSHVisuals>(Client)) {}

void CSHHelper::TickPredict(CNetObj_Character *pCharacter, int t, vec2 *m_pPosArray)
{
	CWorldCore TmpWorld;
	auto TmpCore = CCharacterCore();
	auto TmpTeams = CTeamsCore();
	TmpCore.Init(&TmpWorld, Collision(), &TmpTeams);
	TmpCore.Read(pCharacter);

	for(int i = 0; i < t; i++)
	{
		TmpCore.Tick(false);
		TmpCore.Move();
		m_pPosArray[i] = TmpCore.m_Pos + TmpCore.m_Vel;
	}
}

int CSHHelper::GetCustomTile(float x, float y) const
{
	if(!m_pClient->Collision()->GetTiles())
		return 0;

	int Nx = std::clamp(static_cast<int>(x / 32), 0, m_pClient->Collision()->GetWidth() - 1);
	int Ny = std::clamp(static_cast<int>(y / 32), 0, m_pClient->Collision()->GetHeight() - 1);
	int Pos = Ny * m_pClient->Collision()->GetWidth() + Nx;

	return m_pClient->Collision()->GetTiles()[Pos].m_Index;
}


bool CSHHelper::IsGrounded(int Id, vec2 Pos)
{
	if(!IsValidId(Id) && Pos == vec2(0, 0))
		return false;
	if(Pos == vec2(0, 0))
		Pos = m_pClient->m_aClients[Id].m_Predicted.m_Pos;
	if(m_pClient->Collision()->CheckPoint(Pos.x + PHYS_SIZE / 2, Pos.y + PHYS_SIZE / 2 + 5))
		return true;
	if(m_pClient->Collision()->CheckPoint(Pos.x - PHYS_SIZE / 2, Pos.y + PHYS_SIZE / 2 + 5))
		return true;
	int MoveRestrictionsBelow = m_pClient->Collision()->GetMoveRestrictions(Pos + vec2(0, PHYS_SIZE / 2 + 4), 0.0f);
	if(MoveRestrictionsBelow & CANTMOVE_DOWN)
		return true;
	return false;
}

bool CSHHelper::IsLocalActive()
{
	if(Controls()->m_aInputData[LOCAL].m_PlayerFlags & PLAYERFLAG_PLAYING && Client()->State() == IClient::STATE_ONLINE)
		return true;
	return false;
}

bool CSHHelper::IsValidId(int Id)
{
	if(Id > -1 && Id <= MAX_CLIENTS)
		return true;
	return false;
}
