#pragma once

#include "game/client/components/shikon/component.h"
#include <game/client/gameclient.h>

class CSHVisuals : public CSHComponent
{
public:
	CSHVisuals(CGameClient *pClient) :
		CSHComponent(pClient){}

	void Run(int ClientID, float Angle, vec2 Position);

	// Draw
	void DrawLine(vec2 Pos1, vec2 Pos2, ColorRGBA Color = {1, 1, 1, 1});
	void DrawCircle(vec2 Pos, float Size, ColorRGBA Color = {1, 1, 1, 1});
	void DrawBox(vec2 Center, float Size, ColorRGBA Color = {1, 1, 1, 1});

private:
	// Aimbot Fov
	void DrawFov();
	void DrawFovF(int Fov, ColorRGBA Color);
	void DrawFovLine(float Offset, ColorRGBA Color);
};
