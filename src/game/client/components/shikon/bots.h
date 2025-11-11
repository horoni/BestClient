#pragma once

#include <game/client/component.h>

class CSHBots : public CComponent
{
public:
	int Sizeof() const override { return sizeof(*this); }

	void Run();
};
