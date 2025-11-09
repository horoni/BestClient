#pragma once

#include "base/logger.h"

#include "game/client/components/shikon/aimbot/aimbot.h"
#include "game/client/components/shikon/bots.h"
#include "game/client/components/shikon/visuals.h"

#include <memory>

class CSHHelper
{
protected:
	[[nodiscard]] CGameClient*	  GameClient()	  const { return m_pClient; }
	[[nodiscard]] CControls*	  Controls()	  const { return &m_pClient->m_Controls; }
	[[nodiscard]] IConsole*		  Console()		  const { return m_pClient->Console(); }
	[[nodiscard]] IInput*		  Input()		  const { return m_pClient->Input(); }
	[[nodiscard]] CCollision*	  Collision()	  const { return m_pClient->Collision(); }
	[[nodiscard]] IClient*		  Client()		  const { return m_pClient->Client(); }
	[[nodiscard]] IGraphics*	  Graphics()	  const { return m_pClient->Graphics(); }
	[[nodiscard]] CGameWorld*	  GameWorld()	  const { return &m_pClient->m_GameWorld; }
	[[nodiscard]] CGameWorld*     PredGameWorld() const { return &m_pClient->m_PredictedWorld; }
	[[nodiscard]] CTuningParams*  Tuning()		  const { return m_pClient->m_aTuning; }
	[[nodiscard]] IServerBrowser* ServerBrowser() const { return m_pClient->ServerBrowser(); }

public:
	CSHHelper(CGameClient *Client) noexcept;

	// Main pointer(needs to be raw)
	CGameClient* m_pClient = nullptr;

	// Pointers
	std::unique_ptr<CSHBots>    m_pBots;
	std::unique_ptr<CSHAimbot>  m_pAimbot;
	std::unique_ptr<CSHVisuals> m_pVisuals;

	void TickPredict(CNetObj_Character *pCharacter, int t, vec2 *m_pPosArray);
	int GetCustomTile(float x, float y) const;

	bool IsLocalActive();
	bool IsValidId(int Id);
	bool IsGrounded(int Id, vec2 Pos = vec2(0, 0));

	// Example usage fHelper->dbg_msg("bot", "bot: hook = %d", Controls()->m_aInputData[LOCAL].m_Hook)
	// prints to the chat as echo for some simple debug messages or information
	void dbg_msg(const char *Sys, const char *Fmt, ...)
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
		m_pClient->Echo(pMessage);
		va_end(Args);
	}
};

extern std::unique_ptr<CSHHelper> shHelper;
