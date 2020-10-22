#include "CEFPossessed.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
extern long		 g_nowTime;


CEFPossessed::CEFPossessed()
{
	m_IsConEffect = TRUE;
}

CEFPossessed::~CEFPossessed()
{
	if( m_ISAppear )
	{
		g_Particle.DelEffSetCon( m_AppearID , 0 , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
	}
}

void CEFPossessed::Init(void)
{
	m_RangeTimer = 6000;
	m_OldTimer   = g_nowTime;
	m_ISAppear   = FALSE;

	if( m_pSrcChr->m_ani_index == n_Skill_Possessed )
	{
		g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , 1.0f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

		m_ISIng = TRUE;
	}
	else
	{
		m_ISIng = FALSE;

		switch( m_ID )
		{	
			case EF_POSSESSED_0:
				m_AppearID = EF_APPEAR_BING0;
				break;

			case EF_POSSESSED_1:
				m_AppearID = EF_APPEAR_BING1;
				break;

			case EF_POSSESSED_2:
				m_AppearID = EF_APPEAR_BING2;
				break;

			case EF_POSSESSED_3:
				m_AppearID = EF_APPEAR_BING3;
				break;

			case EF_POSSESSED_4:
				m_AppearID = EF_APPEAR_BING4;
				break;
		}
		
		g_Particle.SetEffSetCon( 0.0f , m_AppearID , 0 , EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

		m_ISAppear = TRUE;
	}
}

int CEFPossessed::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	if( m_ISAppear )
		return TRUE;

	DWORD timer = SAFE_TIME_SUB( g_nowTime , m_OldTimer );
	
	if( timer >= 1000 )
	{
		m_ISAppear = TRUE;
		
		switch( m_ID )
		{	
			case EF_POSSESSED_0:
				m_AppearID = EF_APPEAR_BING0;
				break;

			case EF_POSSESSED_1:
				m_AppearID = EF_APPEAR_BING1;
				break;

			case EF_POSSESSED_2:
				m_AppearID = EF_APPEAR_BING2;
				break;

			case EF_POSSESSED_3:
				m_AppearID = EF_APPEAR_BING3;
				break;

			case EF_POSSESSED_4:
				m_AppearID = EF_APPEAR_BING4;
				break;
		}
		
		g_Particle.SetEffSetCon( 0.0f , m_AppearID , 0 , EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

		g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	}

	return TRUE;
}
