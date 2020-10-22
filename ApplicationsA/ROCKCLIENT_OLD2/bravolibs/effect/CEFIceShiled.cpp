#include "CEFIceShiled.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long		 g_nowTime;


CEFIceShiled::CEFIceShiled()
{
}

CEFIceShiled::~CEFIceShiled()
{
	g_Particle.DelEffSetCon( m_ID , 0 , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

void CEFIceShiled::Init(void)
{
	g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int CEFIceShiled::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}
	

	return TRUE;
}