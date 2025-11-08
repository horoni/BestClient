#include "aimbot/aimbot.h"
#include "f_helper.h"
#include "f_visuals.h"

void FVisuals::Run(int ClientID, float Angle, vec2 Position)
{
	if(!g_Config.m_ShEsp)
		return;
	Graphics()->TextureClear();
	DrawFov();
}

void FVisuals::DrawFov()
{
	if(g_Config.m_ShEspHookFov)
		DrawFovF(g_Config.m_ShAimHookFov, g_Config.m_ShEspHookFovCol);

	if(g_Config.m_ShEspWeaponFov) {
		EWeapon Weapon;
		if (m_pClient->m_Snap.m_pLocalCharacter)
			Weapon = (EWeapon)m_pClient->m_Snap.m_pLocalCharacter->m_Weapon;
		else return;

		switch (Weapon) {
			case EWeapon::Hammer:
				DrawFovF(g_Config.m_ShAimHammerFov, g_Config.m_ShEspWeaponFovCol);
				break;
			case EWeapon::Gun:
				DrawFovF(g_Config.m_ShAimGunFov, g_Config.m_ShEspWeaponFovCol);
				break;
			case EWeapon::Shotgun:
				DrawFovF(g_Config.m_ShAimShotgunFov, g_Config.m_ShEspWeaponFovCol);
				break;
			case EWeapon::Grenade:
				DrawFovF(g_Config.m_ShAimGrenadeFov, g_Config.m_ShEspWeaponFovCol);
				break;
			case EWeapon::Laser:
				DrawFovF(g_Config.m_ShAimLaserFov, g_Config.m_ShEspWeaponFovCol);
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
