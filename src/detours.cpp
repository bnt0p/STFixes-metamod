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

#include "networkbasetypes.pb.h"
#include "usercmd.pb.h"
#include "cs_usercmd.pb.h"

#include "cdetour.h"
#include "module.h"
#include "addresses.h"
#include "detours.h"
#include "entity/ccsplayercontroller.h"
#include "entity/ccsplayerpawn.h"
#include "entity/cbasemodelentity.h"
#include "entity/ctriggerpush.h"
#include "entity/cgamerules.h"
#include "entity/services.h"
#include "playermanager.h"
#include "igameevents.h"
#include "gameconfig.h"
#include "serversideclient.h"
#include "tracefilter.h"

#define VPROF_ENABLED
#include "tier0/vprof.h"

#include "tier0/memdbgon.h"

extern CGlobalVars *gpGlobals;
extern CGameEntitySystem *g_pEntitySystem;
extern CCSGameRules *g_pGameRules;

CUtlVector<CDetourBase *> g_vecDetours;

DECLARE_DETOUR(TriggerPush_Touch, Detour_TriggerPush_Touch);

#define f32 float32
#define i32 int32_t
#define u32 uint32_t

void FASTCALL Detour_TriggerPush_Touch(CTriggerPush* pPush, Z_CBaseEntity* pOther)
{
	// Fitting both handles into a single uint64
	uint64 iPushID = ((uint64)pPush->GetHandle().ToInt() << 32) + pOther->GetHandle().ToInt();

	// We're inserting the push ID into a set and if that fails it means this trigger already pushed the other ent in this tick
	// The set is cleared on the next tick
	if (g_bPreventMultiPush && !g_PushEntSet.insert(iPushID).second)
		return;

	// This trigger pushes only once (and kills itself) or pushes only on StartTouch, both of which are fine already
	if (!g_bUseOldPush || pPush->m_spawnflags() & SF_TRIG_PUSH_ONCE || pPush->m_bTriggerOnStartTouch())
	{
		TriggerPush_Touch(pPush, pOther);
		return;
	}

	MoveType_t movetype = pOther->m_nActualMoveType();

	// VPhysics handling doesn't need any changes
	if (movetype == MOVETYPE_VPHYSICS)
	{
		TriggerPush_Touch(pPush, pOther);
		return;
	}

	if (movetype == MOVETYPE_NONE || movetype == MOVETYPE_PUSH || movetype == MOVETYPE_NOCLIP)
		return;

	CCollisionProperty* collisionProp = pOther->m_pCollision();
	if (!IsSolid(collisionProp->m_nSolidType(), collisionProp->m_usSolidFlags()))
		return;

	if (!pPush->PassesTriggerFilters(pOther))
		return;

	if (pOther->m_CBodyComponent()->m_pSceneNode()->m_pParent())
		return;

	Vector vecAbsDir;
	matrix3x4_t matTransform = pPush->m_CBodyComponent()->m_pSceneNode()->EntityToWorldTransform();

	Vector vecPushDir = pPush->m_vecPushDirEntitySpace();
	VectorRotate(vecPushDir, matTransform, vecAbsDir);

	Vector vecPush = vecAbsDir * pPush->m_flSpeed();

	uint32 flags = pOther->m_fFlags();

	if (flags & (FL_BASEVELOCITY))
	{
		vecPush = vecPush + pOther->m_vecBaseVelocity();
	}

	if (vecPush.z > 0 && (flags & FL_ONGROUND))
	{
		pOther->SetGroundEntity(nullptr);
		Vector origin = pOther->GetAbsOrigin();
		origin.z += 1.0f;

		pOther->Teleport(&origin, nullptr, nullptr);
	}

	if (g_bLogPushes)
	{
		Vector vecEntBaseVelocity = pOther->m_vecBaseVelocity;
		Vector vecOrigPush = vecAbsDir * pPush->m_flSpeed();

		Message("Pushing entity %i | frametime = %.3f | entity basevelocity = %.2f %.2f %.2f | original push velocity = %.2f %.2f %.2f | final push velocity = %.2f %.2f %.2f\n",
			pOther->GetEntityIndex(),
			gpGlobals->frametime,
			vecEntBaseVelocity.x, vecEntBaseVelocity.y, vecEntBaseVelocity.z,
			vecOrigPush.x, vecOrigPush.y, vecOrigPush.z,
			vecPush.x, vecPush.y, vecPush.z);
	}

	pOther->m_vecBaseVelocity(vecPush);

	flags |= (FL_BASEVELOCITY);
	pOther->m_fFlags(flags);
}

bool InitDetours(CGameConfig *gameConfig)
{
	bool success = true;

	FOR_EACH_VEC(g_vecDetours, i)
	{
		if (!g_vecDetours[i]->CreateDetour(gameConfig))
			success = false;
		
		g_vecDetours[i]->EnableDetour();
	}

	return success;
}

void FlushAllDetours()
{
	g_vecDetours.Purge();
}
