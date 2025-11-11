#pragma once

#include <base/logger.h>
#include <base/system.h>
#include <base/vmath.h>
#include <game/client/component.h>

#include <memory>

class CSHHelper : public CComponent
{
public:
	int Sizeof() const override { return sizeof(*this); }

	int GetCustomTile(float x, float y) const;

	bool IsLocalActive();
	bool IsValidId(int Id);
	bool IsGrounded(int Id, vec2 Pos = vec2(0, 0));

	// Example usage fHelper->dbg_msg("bot", "bot: hook = %d", Controls()->m_aInputData[LOCAL].m_Hook)
	// prints to the chat as echo for some simple debug messages or information
	void dbg_msg(const char *Sys, const char *Fmt, ...);
};
