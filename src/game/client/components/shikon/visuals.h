#pragma once

#include <base/color.h>
#include <game/client/component.h>

class CSHVisuals : public CComponent
{
public:
	int Sizeof() const override { return sizeof(*this); }

	void Run(int ClientID, float Angle, vec2 Position);

	// Draw
	void DrawLine(vec2 Pos1, vec2 Pos2, ColorRGBA Color = {1, 1, 1, 1});
	void DrawCircle(vec2 Pos, float Size, ColorRGBA Color = {1, 1, 1, 1});
	void DrawBox(vec2 Center, float Size, ColorRGBA Color = {1, 1, 1, 1});

private:
	// Aimbot Fov
	void DrawFov();
	void DrawFovLines(int Fov, ColorHSLA Color);

	void DrawPredict();
};
