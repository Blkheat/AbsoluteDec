#include "CEFKeepUpPump.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long		 g_nowTime;


CEFKeepUpPump::CEFKeepUpPump()
{
	m_IsConEffect = TRUE;
}

CEFKeepUpPump::~CEFKeepUpPump()
{
	g_Particle.DelEffSetCon( m_ID , 0 , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
}

void CEFKeepUpPump::Init(void)
{
	m_NowTimer = 0.0f;

	g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int CEFKeepUpPump::Update(float dtime)
{
	m_NowTimer += dtime;
	
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) ||
		m_NowTimer >= m_MaxTimer )
	{
		SetDestroy();

		return TRUE;
	}
	

	return TRUE;
}