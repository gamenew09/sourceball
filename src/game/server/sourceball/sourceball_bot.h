//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef SB_BOT_TEMP_H
#define SB_BOT_TEMP_H
#ifdef _WIN32
#pragma once
#endif

#include "player.h"

// If iTeam or iClass is -1, then a team or class is randomly chosen.
extern CBasePlayer *SBBotPutInServer(bool bFrozen, int iTeam, int iClass);

extern void Bot_RunAll();


#endif // SDK_BOT_TEMP_H
