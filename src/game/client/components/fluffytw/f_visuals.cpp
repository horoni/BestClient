#include "f_helper.h"
#include "f_visuals.h"

void FVisuals::Run(int ClientID, float Angle, vec2 Position)
{
	if(!g_Config.m_ClEsp)
		return;
	Graphics()->TextureClear();
	DrawFov();
}


// Fov
void FVisuals::DrawFov()
{
	if(!g_Config.m_ClEspFov)
		return;
	DrawFovLine(g_Config.m_ClAimbotHookFov * 0.01f);
	DrawFovLine(g_Config.m_ClAimbotHookFov * -0.01f);
  // TODO(horoni): Draw weapon FOV 
}

void FVisuals::DrawFovLine(float offset)
{
	const float Angle = angle(Controls()->m_aMousePos[g_Config.m_ClDummy]) + offset;
	const vec2 ExDirection = normalize(direction(Angle));

	const vec2 InitPos = m_pClient->m_LocalCharacterPos;
	vec2 FinishPos = InitPos + ExDirection * (Tuning()->m_HookLength);

	int teleNr = 0;
	Collision()->IntersectLineTeleHook(InitPos, FinishPos, &FinishPos, nullptr, &teleNr);

	const ColorRGBA HookCollColor(0.0f, 0.0f, 0.0f, 0.5f);
	DrawLine(InitPos, FinishPos, HookCollColor);
}

