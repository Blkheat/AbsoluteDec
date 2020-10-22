#include "CEFFPopoiAttack.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long g_nowTime;

CEFFPopoiAttack::CEFFPopoiAttack()
{
	m_IsConEffect = TRUE;
}

CEFFPopoiAttack::~CEFFPopoiAttack()
{
}

void CEFFPopoiAttack::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return;
	}
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );	

}


int CEFFPopoiAttack::CheckFrame(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		SetAfterEffect();
		SetDestroy();
	}

	switch( m_ID )
	{
		case EF_POPOI_ATTACK_0:
			
			if( GetSrcAniCount() == 11 )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_POPOI_ATTACKED , 0 , 2.5f , 
					m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

				SetDestroy();
	
				SetAfterEffect();
			}

			break;

		case EF_POPOI_ATTACK_1:
			
			if( GetSrcAniCount() == 5 )		//도약
			{
				//g_Particle.SetEffSetCon( 0.0f , EF_POPOI_JUMP , ESLT_CHARPOS , 2.5f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
			}
			else if( GetSrcAniCount() == 31 )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_POPOI_ATTACKED , 0 , 2.5f , 
										 m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

				SetDestroy();
	
				SetAfterEffect();
			}

			break;


		case EF_POPOI_ATTACK_2:				
			
			if( GetSrcAniCount() == 5 )		//도약
			{
				//g_Particle.SetEffSetCon( 0.0f , EF_POPOI_JUMP , ESLT_CHARPOS , 2.5f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
			}
			else if( GetSrcAniCount() == 20 )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_POPOI_ATTACKED , 0 , 2.5f , m_pDestChr->GetUnique() , 
										 m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

			}
			else if( GetSrcAniCount() == 40 )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_POPOI_ATTACKED , 0 , 2.5f , 
					m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
				
				SetDestroy();
	
				SetAfterEffect();
			}

			break;
		
		default:

			SetDestroy();

			break;
	}

	return TRUE;
}

int CEFFPopoiAttack::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	return TRUE;
}

int	CEFFPopoiAttack::Render(float dtime)
{
	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFFPopoiAttack::SetAfterEffect(void)
{
	// 데미지 효과

	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
