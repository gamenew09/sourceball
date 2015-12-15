// Includes required for Glow and Entity
// cbase.h - entity includes.
#include "cbase.h"
#include "c_physicsprop.h"
// glow_outline_effect.h - The glow methods.
#include "glow_outline_effect.h"
#include "dt_recv.h"

// The class that is for the client Model entity, everything in this file would go in Client (HL2MP).
class C_GlowBarrel : public C_PhysicsProp
{
public:
	DECLARE_CLASS(C_GlowBarrel, C_PhysicsProp);
	DECLARE_CLIENTCLASS();

	void	Spawn();
	void	HandleGlowEffect();
	void	ClientThink();
private:
	int nGlowObjectHandle;
};

// Makes the class C_GlowBarrel as the entity model_testglow, this is done server side aswell.
LINK_ENTITY_TO_CLASS(sb_glow_barrel, C_GlowBarrel);

// This is the table for data being transmitted, it's empty since I don't need to use anything.
// DT_MyModelEntity is just a name for the model table.
// CMyModelEntity is the class name for the server side entity.
IMPLEMENT_CLIENTCLASS_DT(C_GlowBarrel, DT_GlowBarrel, CGlowBarrel)
END_RECV_TABLE()

// Called when the entity is spawned, we don't worry about models since the server declares the
// model to use and the back end class renders it.
// We call ClientThink to have the glow on the first frame of rendering.
void C_GlowBarrel::Spawn()
{
	BaseClass::Spawn();
	ClientThink();
}

// Called when the model decides to Tick or Think.
// This is where we establish the glow.
void C_GlowBarrel::ClientThink()
{
	HandleGlowEffect();

	// Makes sure the ClientThink method always gets called.
	SetNextClientThink(CLIENT_THINK_ALWAYS);
}

// Called by ClientThink, we create the glow based on the Color red, fully opaque, and is drawn always (even through walls).
void C_GlowBarrel::HandleGlowEffect()
{
	if (this->IsAlive()){
		// Creates a glow for this entity.
		if (!g_GlowObjectManager.HasGlowEffect(this))
		{
			nGlowObjectHandle = g_GlowObjectManager.RegisterGlowObject(this, GLOW_COLOR_RED, 1.0f, false, true, 0);
		}
		// The GLOW_COLOR_RED is in glow_outline_effect.h added in by me with the value of: Vector(255, 0, 0)
	}
	else{
		// Removes glow from this entity, should prevent the game from showing a already destroyed object.
		if (g_GlowObjectManager.HasGlowEffect(this))
		{
			g_GlowObjectManager.UnregisterGlowObject(nGlowObjectHandle);
			nGlowObjectHandle = 0; // Prevents bad stuff from happening, hopefully. :D
		}
	}

	
}