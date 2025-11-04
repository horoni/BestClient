#pragma once

#include <game/client/gameclient.h>
#include "game/client/components/fluffytw/f_component.h"

class FVisuals : public FComponent
{
public:
	FVisuals(CGameClient *pClient) :
		FComponent(pClient){}

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
