#include "CEFIceSprie.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long g_nowTime;

CEFIceSpire::CEFIceSpire()
{
	m_IsConEffect = TRUE;
}

CEFIceSpire::~CEFIceSpire()
{
}

void CEFIceSpire::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy();

		return;
	}

	if( m_pSrcChr == g_Pc.GetPlayer() )
		g_Particle.RockCamera( ( rand() % 200 ) + 300 );

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	SetAfterEffect();
}


//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFIceSpire::SetAfterEffect(void)
{
	int EffectID = 0;
	
	switch( m_ID )
	{
		case EF_ICESPIRE0:
		case EF_ICESPIRE1:
			EffectID = EF_ICESPIRE_ATTACKED0;
			break;
		case EF_ICESPIRE2:
		case EF_ICESPIRE3:
			EffectID = EF_ICESPIRE_ATTACKED2;
			break;
		case EF_ICESPIRE4:
			EffectID = EF_ICESPIRE_ATTACKED4;
			break;
	}

	// 데미지 효과

	//................................................................................	
	// 어택 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EffectID , 0 , 1.1f , 
							 m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EffectID;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 1.1f;
	}
	
	/*
	g_Particle.SetEffSetCon( 0.0f , EffectID , 100 , 3.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex() , 
		D3DXVECTOR3( m_pDestChr->m_matBoneCenter._41 , m_pDestChr->m_matBoneCenter._42 , m_pDestChr->m_matBoneCenter._43 ) );
*/
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	
	return TRUE;
}
