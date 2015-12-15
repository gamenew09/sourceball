//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Projectile shot from the AR2 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef	WEAPONAK47_H
#define	WEAPONAK47_H

#include "weapon_hl2mpbase_machinegun.h"

#ifdef CLIENT_DLL
#define CWeaponAK47 C_WeaponAK47
#endif

class CWeaponAK47 : public CHL2MPMachineGun
{
public:
	DECLARE_CLASS(CWeaponAK47, CHL2MPMachineGun);

	CWeaponAK47();

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	void	ItemPostFrame(void);
	void	Precache(void);

	const char *GetTracerType(void) { return "AR2Tracer"; }

	void	AddViewKick(void);

	int		GetMinBurst(void) { return 2; }
	int		GetMaxBurst(void) { return 5; }
	float	GetFireRate(void) { return 0.1f; }

	bool	CanHolster(void);
	bool	Reload(void);

	Activity	GetPrimaryAttackActivity(void);

	void	DoImpactEffect(trace_t &tr, int nDamageType);

	virtual bool Deploy(void);


	virtual const Vector& GetBulletSpread(void)
	{
		static Vector cone;

		cone = VECTOR_CONE_3DEGREES;

		return cone;
	}

	const WeaponProficiencyInfo_t *GetProficiencyValues();

private:
	CWeaponAK47(const CWeaponAK47 &);

protected:

	float					m_flDelayedFire;
	bool					m_bShotDelayed;
	int						m_nVentPose;

#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif
};


#endif	//WEAPONAR2_H
