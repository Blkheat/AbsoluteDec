#include "CEFLandDamage.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "CEFArrow.h"


extern long		 g_nowTime;

CEFLandDamage::CEFLandDamage()
{
	m_IsConEffect = TRUE;
}

CEFLandDamage::~CEFLandDamage()
{
}

void CEFLandDamage::Init(void)
{
	m_OldTime = g_nowTime;
}

int CEFLandDamage::Update(float dtime)
{
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , m_WaitTime ) )
	{
		if( m_pExtraInfo != NULL )
		{
			// 1:N µ¥¹ÌÁö
			int Count = m_pExtraInfo->GetTargetCount();			
			
			for( int i = 0 ; i < Count ; ++i )
			{
				if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
				{
					g_Particle.SetEffSetCon( 0.0f , EF_ICE_RAINED , 0 , 3.1f , 
						pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
					
					Info.AfterEffect = EF_ICE_RAINED;
					Info.DestType = 0;
					Info.lifeTime = 3.1f;		
					
					g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
				}

				Info.Clear();
			}
		}

		SetDestroy();
	}

	return TRUE;
}


