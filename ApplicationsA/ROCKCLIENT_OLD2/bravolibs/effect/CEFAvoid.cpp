 #include "CEFAvoid.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include <SOUND\Sound.H>
#include <NETWORK\\Protocol.h>
#include <Base\\D3DBase.h>
#include "..\\RockClient.h"
#include "..\\quadlist.h"
#include <Map\\Field.h>
#include "..\\Map.H"
#include "..\\PC.H"
#include "..\\Effect.H"
#include "..\\Camera.H"
#include "..\\RockClient.h"

extern long		 g_nowTime;

CEFAvoid::CEFAvoid()
{
}

CEFAvoid::~CEFAvoid()
{
}

void CEFAvoid::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return;
	}

	g_Particle.SetEffSetCon( 0.0f , EF_AVOID0 , 0 , 2.6f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}