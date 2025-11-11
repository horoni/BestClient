#include "game/client/components/shikon/aimbot/aimbot.h"
#include "game/client/components/shikon/helper.h"
#include "game/client/components/shikon/visuals.h"
#include "game/client/prediction/entities/character.h"
#include <array>

void CSHVisuals::Run(int ClientID, float Angle, vec2 Position)
{
	if(!g_Config.m_ShEsp)
		return;
	if(g_Config.m_ShEspPredict)
		DrawPredict();
	DrawFov();
}

void CSHVisuals::DrawFov()
{
	if(g_Config.m_ShEspHookFov)
		DrawFovLines(g_Config.m_ShAimHookFov, g_Config.m_ShEspHookFovCol);

	if(g_Config.m_ShEspWeaponFov) {
		EWeapon Weapon;
		if (m_pClient->m_Snap.m_pLocalCharacter)
			Weapon = (EWeapon)m_pClient->m_Snap.m_pLocalCharacter->m_Weapon;
		else return;

		switch (Weapon) {
			case EWeapon::Hammer:
				DrawFovLines(g_Config.m_ShAimHammerFov, g_Config.m_ShEspWeaponFovCol);
				break;
			case EWeapon::Gun:
				DrawFovLines(g_Config.m_ShAimGunFov, g_Config.m_ShEspWeaponFovCol);
				break;
			case EWeapon::Shotgun:
				DrawFovLines(g_Config.m_ShAimShotgunFov, g_Config.m_ShEspWeaponFovCol);
				break;
			case EWeapon::Grenade:
				DrawFovLines(g_Config.m_ShAimGrenadeFov, g_Config.m_ShEspWeaponFovCol);
				break;
			case EWeapon::Laser:
				DrawFovLines(g_Config.m_ShAimLaserFov, g_Config.m_ShEspWeaponFovCol);
				break;
		}
	}
}

void CSHVisuals::DrawFovLines(int Fov, ColorHSLA Color) {
	ColorRGBA Col = color_cast<ColorRGBA>(Color);
	const float Angle = angle(Controls()->m_aMousePos[LOCAL]);
	const float FovRadians = (Fov * pi) / 180.f;
	const float UpAngle = Angle - FovRadians / 2;
	const float DwAngle = Angle + FovRadians / 2;

	const vec2 MyPos = m_pClient->m_LocalCharacterPos;
	vec2 UpPos = MyPos + direction(UpAngle) * (Tuning()->m_HookLength);
	vec2 DwPos = MyPos + direction(DwAngle) * (Tuning()->m_HookLength);

	Collision()->IntersectLine(MyPos, UpPos, &UpPos, nullptr);
	Collision()->IntersectLine(MyPos, DwPos, &DwPos, nullptr);

	DrawLine(MyPos, UpPos, Col);
	DrawLine(MyPos, DwPos, Col);
}

void CSHVisuals::DrawPredict()
{
	// TODO(horoni): Optimize
	CGameWorld TmpWorld;
	TmpWorld.CopyWorld(&m_pClient->m_PredictedWorld);

	Graphics()->TextureClear();
	Graphics()->QuadsBegin();
	for (int i = 0; i < g_Config.m_ShEspPredictTicks; i++)
	{
		TmpWorld.Tick();
		if (g_Config.m_ShEspPredictEven && i % 2 == 1)
			continue;

		CCharacter *LocalChar = TmpWorld.GetCharacterById(m_pClient->m_Snap.m_LocalClientId);
		if (!LocalChar)
			break;

		vec2 Pos = LocalChar->m_Pos;
		ColorRGBA Col = LocalChar->m_FreezeTime > 0 ? ColorRGBA(0xFF0000) : ColorRGBA(0x1FFF01);
		Col.a = 0.6f;

		Graphics()->SetColor(Col);
		Graphics()->DrawCircle(Pos.x, Pos.y, 4, 16);
	}
	Graphics()->QuadsEnd();
	Graphics()->SetColor(1.f, 1.f, 1.f, 1.f);
}

void CSHVisuals::DrawLine(vec2 Pos1, vec2 Pos2, ColorRGBA Color)
{
	Graphics()->TextureClear();
	Graphics()->LinesBegin();
	const IGraphics::CLineItem LineItem(Pos1.x, Pos1.y, Pos2.x, Pos2.y);
	Graphics()->SetColor(Color);
	Graphics()->LinesDraw(&LineItem, 1);
	Graphics()->LinesEnd();
	Graphics()->SetColor(1.f, 1.f, 1.f, 1.f);
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

