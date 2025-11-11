#include "game/client/components/shikon/bots.h"
#include <game/client/gameclient.h>

void CSHBots::Run()
{
	GameClient()->m_Aimbot.Aimbot();
}
