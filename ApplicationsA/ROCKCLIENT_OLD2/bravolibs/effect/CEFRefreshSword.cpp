#include "CEFRefreshSword.h"
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

CEFRefreshSword::CEFRefreshSword()
{
}

CEFRefreshSword::~CEFRefreshSword()
{
}

void CEFRefreshSword::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return;
	}

	g_Particle.SetEffSetCon( 0.0f , EF_REFRESH_SWORD0 , 0 , 2.6f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}