#include "CEFLightning.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

CEFLightning::CEFLightning()
{
	m_IsConEffect = TRUE;
}

CEFLightning::~CEFLightning()
{
}

void CEFLightning::Init(void)
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

	int EffectID = 0;
	
	switch( m_ID )
	{
		case EF_LIGHTNING0:
		case EF_LIGHTNING1:
			
			EffectID = EF_LIGHTNING_ATTACKED0;

			break;
		
		case EF_LIGHTNING2:
		case EF_LIGHTNING3:
			
			EffectID = EF_LIGHTNING_ATTACKED2;

			break;
		case EF_LIGHTNING4:
			
			EffectID = EF_LIGHTNING_ATTACKED4;

			break;
	}

	// 데미지 효과
	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EffectID , 0 , 2.1f , 
							 m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
	
	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EffectID;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 2.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	if( m_pSrcChr == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );
	
	SetDestroy();
}
