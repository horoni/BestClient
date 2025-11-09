#pragma once

#include "game/client/components/shikon/component.h"
#include <game/client/gameclient.h>

class CSHBots : public CSHComponent
{
public:
	CSHBots(CGameClient *pClient) :
		CSHComponent(pClient){}
	void Run();
};
