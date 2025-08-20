/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023-2024 Source2ZE
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "platform.h"
#include "stdint.h"
#include "utils/module.h"
#include "utlstring.h"
#include "variant.h"
#include "gametrace.h"

namespace modules
{
	inline CModule *engine;
	inline CModule *tier0;
	inline CModule *server;
	inline CModule *schemasystem;
	inline CModule *vscript;
	inline CModule *client;
	inline CModule* networksystem;
#ifdef _WIN32
	inline CModule *hammer;
#endif
}

class CEntityInstance;
class CEntityIdentity;
class CBasePlayerController;
class CCSPlayerController;
class CCSPlayerPawn;
class CBaseModelEntity;
class CBaseEntity;
class CGameConfig;
class CEntitySystem;
class IEntityFindFilter;
class CGameRules;
class CEntityKeyValues;
class IRecipientFilter;
class CNetworkStateChangedInfo;
struct bbox_t;

struct SndOpEventGuid_t;

namespace addresses
{
	bool Initialize(CGameConfig *g_GameConfig);

	inline void(FASTCALL* SetGroundEntity)(CBaseEntity* ent, CBaseEntity* ground, CBaseEntity* unk3);
	inline void(FASTCALL* SetGravityScale)(CBaseEntity*, float);
	inline void(FASTCALL* CBasePlayerController_SetPawn)(CBasePlayerController* pController, CCSPlayerPawn* pPawn, bool a3, bool a4, bool a5, bool a6);
	inline void(FASTCALL* NetworkStateChanged)(void* chainEntity, CNetworkStateChangedInfo& info);
}
