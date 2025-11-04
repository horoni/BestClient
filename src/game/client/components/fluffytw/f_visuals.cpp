#include "f_helper.h"
#include "f_visuals.h"

void FVisuals::Run(int ClientID, float Angle, vec2 Position)
{
	if(!g_Config.m_ClEsp)
		return;
	Graphics()->TextureClear();
	DrawFov();
}

void FVisuals::DrawFov()
{
	if(g_Config.m_ClEspHookFov)
		DrawFovF(g_Config.m_ClAimbotHookFov, g_Config.m_ClEspHookFovCol);

	if(g_Config.m_ClEspWeaponFov) {
		int SelectedWeapon;
		if (m_pClient->m_Snap.m_pLocalCharacter)
			SelectedWeapon = m_pClient->m_Snap.m_pLocalCharacter->m_Weapon;
		else return;

		switch (SelectedWeapon) {
			case 0:
				DrawFovF(g_Config.m_ClAimbotHammerFov, g_Config.m_ClEspWeaponFovCol);
				break;
			case 1:
				DrawFovF(g_Config.m_ClAimbotGunFov, g_Config.m_ClEspWeaponFovCol);
				break;
			case 2:
				DrawFovF(g_Config.m_ClAimbotShotgunFov, g_Config.m_ClEspWeaponFovCol);
				break;
			case 3:
				DrawFovF(g_Config.m_ClAimbotGrenadeFov, g_Config.m_ClEspWeaponFovCol);
				break;
			case 4:
				DrawFovF(g_Config.m_ClAimbotLaserFov, g_Config.m_ClEspWeaponFovCol);
				break;
		}
	}
}

void FVisuals::DrawFovF(int Fov, ColorRGBA Color) {
	DrawFovLine(Fov * 0.01f, Color);
	DrawFovLine(Fov * -0.01f, Color);
}

void FVisuals::DrawFovLine(float Offset, ColorRGBA Color)
{
	const float Angle = angle(Controls()->m_aMousePos[g_Config.m_ClDummy]) + Offset;
	const vec2 ExDirection = normalize(direction(Angle));

	const vec2 InitPos = m_pClient->m_LocalCharacterPos;
	vec2 FinishPos = InitPos + ExDirection * (Tuning()->m_HookLength);

	int TeleNr = 0;
	Collision()->IntersectLineTeleHook(InitPos, FinishPos, &FinishPos, nullptr, &TeleNr);

	DrawLine(InitPos, FinishPos, Color);
}
