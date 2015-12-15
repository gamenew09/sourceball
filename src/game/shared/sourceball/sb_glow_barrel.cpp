//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ========
//
// Purpose: Simple model entity that randomly moves and changes direction
//			when activated.
//
//=============================================================================

#include "cbase.h"
#include "props.h"
#include "networkvar.h"
#include "sendproxy.h"

class CGlowBarrel : public CPhysicsProp
{
public:
	DECLARE_CLASS(CGlowBarrel, CPhysicsProp);
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

	void Spawn(void);
	void Precache(void);
};

LINK_ENTITY_TO_CLASS(sb_glow_barrel, CGlowBarrel);

// Start of our data description for the class
BEGIN_DATADESC(CGlowBarrel)

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST(CGlowBarrel, DT_GlowBarrel)
END_SEND_TABLE();

// Name of our entity's model
#define	ENTITY_MODEL	"models/props_c17/oildrum001_explosive.mdl"

//-----------------------------------------------------------------------------
// Purpose: Precache assets used by the entity
//-----------------------------------------------------------------------------
void CGlowBarrel::Precache(void)
{
	PrecacheModel(ENTITY_MODEL);

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
void CGlowBarrel::Spawn(void)
{
	Precache();

	SetModel(ENTITY_MODEL);
	VPhysicsInitNormal(SOLID_VPHYSICS, 0, false);

	BaseClass::Spawn();
}