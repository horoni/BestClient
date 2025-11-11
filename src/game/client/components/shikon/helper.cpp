#include "game/client/components/shikon/helper.h"
#include <game/client/gameclient.h>
#include <algorithm>
#include "game/client/components/shikon/defs.h"

int CSHHelper::GetCustomTile(float x, float y) const
{
	if(!GameClient()->Collision()->GetTiles())
		return 0;

	int Nx = std::clamp(static_cast<int>(x / 32), 0, GameClient()->Collision()->GetWidth() - 1);
	int Ny = std::clamp(static_cast<int>(y / 32), 0, GameClient()->Collision()->GetHeight() - 1);
	int Pos = Ny * GameClient()->Collision()->GetWidth() + Nx;

	return GameClient()->Collision()->GetTiles()[Pos].m_Index;
}


bool CSHHelper::IsGrounded(int Id, vec2 Pos)
{
	if(!IsValidId(Id) && Pos == vec2(0, 0))
		return false;
	if(Pos == vec2(0, 0))
		Pos = GameClient()->m_aClients[Id].m_Predicted.m_Pos;
	if(GameClient()->Collision()->CheckPoint(Pos.x + PHYS_SIZE / 2, Pos.y + PHYS_SIZE / 2 + 5))
		return true;
	if(GameClient()->Collision()->CheckPoint(Pos.x - PHYS_SIZE / 2, Pos.y + PHYS_SIZE / 2 + 5))
		return true;
	int MoveRestrictionsBelow = GameClient()->Collision()->GetMoveRestrictions(Pos + vec2(0, PHYS_SIZE / 2 + 4), 0.0f);
	if(MoveRestrictionsBelow & CANTMOVE_DOWN)
		return true;
	return false;
}

bool CSHHelper::IsLocalActive()
{
	if(GameClient()->m_Controls.m_aInputData[LOCAL].m_PlayerFlags & PLAYERFLAG_PLAYING && Client()->State() == IClient::STATE_ONLINE)
		return true;
	return false;
}

bool CSHHelper::IsValidId(int Id)
{
	if(Id > -1 && Id <= MAX_CLIENTS)
		return true;
	return false;
}

void CSHHelper::dbg_msg(const char *Sys, const char *Fmt, ...)
{
	va_list Args;
	va_start(Args, Fmt);
	CLogMessage Msg;
	Msg.m_Level = LEVEL_INFO;
	str_timestamp_format(Msg.m_aTimestamp, sizeof(Msg.m_aTimestamp), FORMAT_SPACE);
	Msg.m_TimestampLength = str_length(Msg.m_aTimestamp);
	str_copy(Msg.m_aSystem, Sys);
	Msg.m_SystemLength = str_length(Msg.m_aSystem);

	// TODO: Add level?
	str_format(Msg.m_aLine, sizeof(Msg.m_aLine), "%s %c %s: ", Msg.m_aTimestamp, "EWIDT"[LEVEL_INFO], Msg.m_aSystem);
	Msg.m_LineMessageOffset = str_length(Msg.m_aLine);

	char *pMessage = Msg.m_aLine + Msg.m_LineMessageOffset;
	int MessageSize = sizeof(Msg.m_aLine) - Msg.m_LineMessageOffset;
	str_format_v(pMessage, MessageSize, Fmt, Args);
	GameClient()->Echo(pMessage);
	va_end(Args);
}
