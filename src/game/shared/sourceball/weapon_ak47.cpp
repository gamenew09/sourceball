//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"

#ifdef CLIENT_DLL
#include "c_hl2mp_player.h"
#include "c_te_effect_dispatch.h"
#else
#include "hl2mp_player.h"
#include "te_effect_dispatch.h"
#include "prop_combine_ball.h"
#endif

#include "weapon_ak47.h"
#include "effect_dispatch_data.h"


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/*
#ifndef CLIENT_DLL
ConVar sk_weapon_ar2_alt_fire_radius("sk_weapon_ar2_alt_fire_radius", "10");
ConVar sk_weapon_ar2_alt_fire_duration("sk_weapon_ar2_alt_fire_duration", "4");
ConVar sk_weapon_ar2_alt_fire_mass("sk_weapon_ar2_alt_fire_mass", "150");
#endif
*/

//=========================================================
//=========================================================


IMPLEMENT_NETWORKCLASS_ALIASED(WeaponAK47, DT_WeaponAK47)

BEGIN_NETWORK_TABLE(CWeaponAK47, DT_WeaponAK47)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponAK47)
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS(weapon_ak47, CWeaponAK47);
PRECACHE_WEAPON_REGISTER(weapon_ak47);


#ifndef CLIENT_DLL

acttable_t	CWeaponAK47::m_acttable[] =
{
	{ ACT_HL2MP_IDLE, ACT_HL2MP_IDLE_AR2, false },
	{ ACT_HL2MP_RUN, ACT_HL2MP_RUN_AR2, false },
	{ ACT_HL2MP_IDLE_CROUCH, ACT_HL2MP_IDLE_CROUCH_AR2, false },
	{ ACT_HL2MP_WALK_CROUCH, ACT_HL2MP_WALK_CROUCH_AR2, false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK, ACT_HL2MP_GESTURE_RANGE_ATTACK_AR2, false },
	{ ACT_HL2MP_GESTURE_RELOAD, ACT_HL2MP_GESTURE_RELOAD_AR2, false },
	{ ACT_HL2MP_JUMP, ACT_HL2MP_JUMP_AR2, false },
	{ ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_AR2, false },
};

IMPLEMENT_ACTTABLE(CWeaponAK47);

#endif

CWeaponAK47::CWeaponAK47()
{
	m_fMinRange1 = 65;
	m_fMaxRange1 = 2048;

	m_fMinRange2 = 256;
	m_fMaxRange2 = 1024;

	m_nShotsFired = 0;
	m_nVentPose = -1;
}

void CWeaponAK47::Precache(void)
{
	BaseClass::Precache();

#ifndef CLIENT_DLL

	UTIL_PrecacheOther("prop_combine_ball");
	UTIL_PrecacheOther("env_entity_dissolver");
#endif

}

//-----------------------------------------------------------------------------
// Purpose: Handle grenade detonate in-air (even when no ammo is left)
//-----------------------------------------------------------------------------
void CWeaponAK47::ItemPostFrame(void)
{

	// Update our pose parameter for the vents
	CBasePlayer *pOwner = ToBasePlayer(GetOwner());

	if (pOwner)
	{
		CBaseViewModel *pVM = pOwner->GetViewModel();

		if (pVM)
		{
			if (m_nVentPose == -1)
			{
				m_nVentPose = pVM->LookupPoseParameter("VentPoses");
			}

			float flVentPose = RemapValClamped(m_nShotsFired, 0, 5, 0.0f, 1.0f);
			pVM->SetPoseParameter(m_nVentPose, flVentPose);
		}
	}

	BaseClass::ItemPostFrame();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Activity
//-----------------------------------------------------------------------------
Activity CWeaponAK47::GetPrimaryAttackActivity(void)
{
	if (m_nShotsFired < 2)
		return ACT_VM_PRIMARYATTACK;

	if (m_nShotsFired < 3)
		return ACT_VM_RECOIL1;

	if (m_nShotsFired < 4)
		return ACT_VM_RECOIL2;

	return ACT_VM_RECOIL3;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &tr - 
//			nDamageType - 
//-----------------------------------------------------------------------------
void CWeaponAK47::DoImpactEffect(trace_t &tr, int nDamageType)
{
	CEffectData data;

	data.m_vOrigin = tr.endpos + (tr.plane.normal * 1.0f);
	data.m_vNormal = tr.plane.normal;

	DispatchEffect("AR2Impact", data);

	BaseClass::DoImpactEffect(tr, nDamageType);
}

//-----------------------------------------------------------------------------
// Purpose: Override if we're waiting to release a shot
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponAK47::CanHolster(void)
{
	if (m_bShotDelayed)
		return false;

	return BaseClass::CanHolster();
}


bool CWeaponAK47::Deploy(void)
{
	m_bShotDelayed = false;
	m_flDelayedFire = 0.0f;

	return BaseClass::Deploy();
}

//-----------------------------------------------------------------------------
// Purpose: Override if we're waiting to release a shot
//-----------------------------------------------------------------------------
bool CWeaponAK47::Reload(void)
{
	if (m_bShotDelayed)
		return false;

	return BaseClass::Reload();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponAK47::AddViewKick(void)
{
#define	EASY_DAMPEN			0.5f
#define	MAX_VERTICAL_KICK	8.0f	//Degrees
#define	SLIDE_LIMIT			5.0f	//Seconds

	//Get the view kick
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
		return;

	DoMachineGunKick(pPlayer, EASY_DAMPEN, MAX_VERTICAL_KICK, m_fFireDuration, SLIDE_LIMIT);
}

//-----------------------------------------------------------------------------
const WeaponProficiencyInfo_t *CWeaponAK47::GetProficiencyValues()
{
	static WeaponProficiencyInfo_t proficiencyTable[] =
	{
		{ 7.0, 0.75 },
		{ 5.00, 0.75 },
		{ 3.0, 0.85 },
		{ 5.0 / 3.0, 0.75 },
		{ 1.00, 1.0 },
	};

	COMPILE_TIME_ASSERT(ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1);

	return proficiencyTable;
}
