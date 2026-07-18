#pragma once

#include <engine/shared/config.h>
#include <game/client/gameclient.h>

inline int GetLocalData() { return g_Config.m_ClDummy; }
inline int GetLocalId(CGameClient* pClient) { return pClient->m_aLocalIds[GetLocalData()]; }

inline int GetDummyData() { return !g_Config.m_ClDummy; }
inline int GetDummyId(CGameClient* pClient) { return pClient->m_aLocalIds[GetDummyData()]; }

inline float GetPhysSize() { return CCharacterCore::PhysicalSize(); }
