#include "CEFCircleCrash.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;
	 
CEFCircleCrash::CEFCircleCrash()
{
	m_InitTime = 0;
	m_IsDamageProc = FALSE;
}

CEFCircleCrash::~CEFCircleCrash()
{

}

void CEFCircleCrash::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy();
		return;
	}

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	g_Particle.SetEffSetCon( 0.0f , EF_CIRCLE_CRASH , 0 , 5.1f , 
									m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

	m_InitTime = g_nowTime;
	m_IsDamageProc = FALSE;
}

int CEFCircleCrash::CheckFrame(void)
{
	if(m_State == ES_SELF_DESTROY)
	{
		return TRUE;
	}
	
	return TRUE;
}

int CEFCircleCrash::Update(float dtime)
{
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , 600 ) )
	{
		if(!m_IsDamageProc)
		{
			SetAfterEffect();	
			m_IsDamageProc = TRUE;
		}
	}	
	
	return TRUE;
}

int	CEFCircleCrash::Render(float dtime)
{
	return TRUE;
}

int CEFCircleCrash::SetAfterEffect(void)
{
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_pExtraInfo != NULL )
	{
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();			
		
		for( int i = 0 ; i < Count ; ++i )
		{
			if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
			{
				Info.AfterEffect = EF_CIRCLE_CHANGE;
				Info.DestType = 0;
				Info.lifeTime = 5.1f;
				
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );	
		
				g_Particle.SetEffSetCon( 0.0f , EF_CIRCLE_CHANGE , 0 , 5.1f , 
										pTempDest->GetUnique() , pTempDest->GetIndex(),::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );				
			}

			Info.Clear();
		}
	}	
	
	return TRUE;
}