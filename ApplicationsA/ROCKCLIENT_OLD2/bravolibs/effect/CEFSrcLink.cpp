#include "CEFSrcLink.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long		 g_nowTime;


CEFSrcLink::CEFSrcLink()
{
	m_IsGiantUnit = FALSE;
	///--PRINT_DLG_LOG("ÀÌÆåÆ® »ý¼º");
}

CEFSrcLink::~CEFSrcLink()
{
	///--JAPAN_BUG_MODIFY
	///--PRINT_DLG_LOG("ÀÌÆåÆ® »ç¶óÁü");
	g_Particle.DelEffSetCon( m_ID , 0 , m_pSrcChr->GetUnique() , 
										m_pSrcChr->GetIndex(), FALSE, FALSE );
}

void CEFSrcLink::Init(void)
{
	if(m_IsGiantUnit)
	{
		g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
	}
	
	g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , EFFECT_SET_LOOP_TIME , 
		m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
}

int CEFSrcLink::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}	

	return TRUE;
}