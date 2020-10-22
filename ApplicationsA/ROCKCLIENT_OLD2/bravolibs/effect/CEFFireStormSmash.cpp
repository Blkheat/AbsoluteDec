#include "CEFFireStormSmash.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFFireStormSmash::CEFFireStormSmash()
{
	m_IsConEffect = TRUE;
}

CEFFireStormSmash::~CEFFireStormSmash()
{

}

void CEFFireStormSmash::Init(void)
{

	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
	
}

int CEFFireStormSmash::CheckFrame(void)
{
	if(m_State == ES_SELF_DESTROY)
	{
		return TRUE;
	}

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		SetAfterEffect();
		SetDestroy();
		return TRUE;
	}
	
	if( GetSrcAniCount() == 53 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}	
	
	m_pSrcChr->BoneRender();

	D3DXVECTOR3 vPos = D3DXVECTOR3( m_pSrcChr->GetRealRHandTM()._41,
								   	m_pSrcChr->GetRealRHandTM()._42,
									m_pSrcChr->GetRealRHandTM()._43);
	
	g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , vPos );
	
	if( GetSrcAniCount() == 53 )
	{
		//................................................................................	
		// 어택 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
		
		
		g_Particle.SetEffSetCon( 0.0f , EF_FIRESTORM_SMASH , ESLT_POS , 3.1f , 
								-1 , -1 , vPos, m_pSrcChr->GetDirection() );
		
		g_Particle.SetEffSetCon( 0.0f , EF_FIRESTORM_SMASHED , ESLT_CHARPOS , 
								 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(),
								 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

		SetAfterEffect();
		SetDestroy();
		return TRUE;
	}
	
	return TRUE;
}

int CEFFireStormSmash::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}
	
	return TRUE;
}

int	CEFFireStormSmash::Render(float dtime)
{
	return TRUE;
}

int CEFFireStormSmash::SetAfterEffect(void)
{
	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_FIRESTORM_SMASHED;
		m_pExtraInfo->DestType = ESLT_CHARPOS;
		m_pExtraInfo->lifeTime = 6.1f;
	}		
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
