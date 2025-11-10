#include "game/client/components/shikon/aimbot/aimbot.h"
#include "game/client/components/shikon/helper.h"
#include "game/client/components/shikon/visuals.h"
#include <array>

void CSHVisuals::Run(int ClientID, float Angle, vec2 Position)
{
	if(!g_Config.m_ShEsp)
		return;
	Graphics()->TextureClear();
	if(g_Config.m_ShEspPredict)
		DrawPredict();
	DrawFov();
}

void CSHVisuals::DrawFov()
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

void CSHVisuals::DrawFovF(int Fov, ColorHSLA Color) {
	ColorRGBA Col = color_cast<ColorRGBA>(Color);
	DrawFovLine(Fov * 0.01f, Col);
	DrawFovLine(Fov * -0.01f, Col);
}

void CSHVisuals::DrawFovLine(float Offset, ColorRGBA Color)
{
	const float Angle = angle(Controls()->m_aMousePos[g_Config.m_ClDummy]) + Offset;
	const vec2 ExDirection = normalize(direction(Angle));

	const vec2 InitPos = m_pClient->m_LocalCharacterPos;
	vec2 FinishPos = InitPos + ExDirection * (Tuning()->m_HookLength);

	int TeleNr = 0;
	Collision()->IntersectLineTeleHook(InitPos, FinishPos, &FinishPos, nullptr, &TeleNr);

	DrawLine(InitPos, FinishPos, Color);
}

void CSHVisuals::DrawPredict()
{
	// TODO(horoni): Make dots red if it go through freeze
	static std::array<vec2, 32> s_aPos;
	const int Ticks = g_Config.m_ShEspPredictTicks;
	CNetObj_Character *pLocalCharacter = const_cast<CNetObj_Character *>(m_pClient->m_Snap.m_pLocalCharacter);
	if (!pLocalCharacter)
		return;

	shHelper->TickPredict(pLocalCharacter, Ticks, s_aPos.data());

	for (int i = 0; i < Ticks; i++)
	{
		if (g_Config.m_ShEspPredictEven && i % 2 == 1)
			continue;
		DrawCircle(s_aPos[i], 4, ColorRGBA(1, 255, 1, 0.8));
	}
}

void CSHVisuals::DrawLine(vec2 Pos1, vec2 Pos2, ColorRGBA Color)
{
	Graphics()->LinesBegin();
	const IGraphics::CLineItem LineItem(Pos1.x, Pos1.y, Pos2.x, Pos2.y);
	Graphics()->SetColor(Color);
	Graphics()->LinesDraw(&LineItem, 1);
	Graphics()->LinesEnd();
}

void CSHVisuals::DrawCircle(vec2 Pos, float Size, ColorRGBA Color)
{
	const float Step = 0.1f;
	const float FirstX = cos(-Step) * Size + Pos.x;
	const float FirstY = sin(-Step) * Size + Pos.y;

	vec2 PrevPos = vec2(FirstX, FirstY);
	vec2 NewPos = PrevPos;

	for (float i = 0; i < 2 * pi; i += Step)
	{
		const float X = cos(i) * Size + Pos.x;
		const float Y = sin(i) * Size + Pos.y;

		PrevPos = NewPos;
		NewPos = vec2(X, Y);

		DrawLine(PrevPos, NewPos, Color);
	}
}

void CSHVisuals::DrawBox(vec2 Center, float Size, ColorRGBA Color)
{
	Graphics()->LinesBegin();
	const IGraphics::CLineItem aLines[4] =
	{
		IGraphics::CLineItem(Center.x - Size, Center.y - Size, Center.x + Size, Center.y - Size),
		IGraphics::CLineItem(Center.x + Size, Center.y - Size, Center.x + Size, Center.y + Size),
		IGraphics::CLineItem(Center.x + Size, Center.y + Size, Center.x - Size, Center.y + Size),
		IGraphics::CLineItem(Center.x - Size, Center.y + Size, Center.x - Size, Center.y - Size)
	};
	Graphics()->SetColor(Color);
	Graphics()->LinesDraw(aLines, 4);
	Graphics()->LinesEnd();
}

