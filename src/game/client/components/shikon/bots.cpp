#include "game/client/components/shikon/bots.h"
#include "game/client/components/shikon/helper.h"

void CSHBots::Run()
{
	shHelper->m_pAimbot->Aimbot();
}
