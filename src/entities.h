/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023-2025 Source2ZE
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

class CBaseEntity;
class Z_CBaseEntity;
class CCSPlayerPawn;
class CEntityKeyValues;

namespace CTriggerGravityHandler
{
	void OnPrecache(CBaseEntity* pEntity, const CEntityKeyValues* kv);
	bool GravityTouching(CBaseEntity* pEntity, CBaseEntity* pOther);
	void OnEndTouch(CBaseEntity* pEntity, CBaseEntity* pOther);
} // namespace CTriggerGravityHandler

void EntityHandler_OnLevelInit();