#include "CEFFireSmack.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long g_nowTime;

CEFFireSmack::CEFFireSmack()
{
	m_IsConEffect = TRUE;
}

CEFFireSmack::~CEFFireSmack()
{
}

void CEFFireSmack::Init(void)
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

	g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , 2.5f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

}


int CEFFireSmack::CheckFrame(void)
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
		return TRUE;
	}

/*
	DebugPrint( "Fire Smack : %d\n" , GetSrcAniCount() );
*/

	switch( m_ID )
	{
		case EF_FIRE_SMACK_0:
			
			if( GetSrcAniCount() == 51 )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_FIRE_SMACK_ATTACKED_0 , 0 , 2.5f , 
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

		case EF_FIRE_SMACK_1:
			
			if( GetSrcAniCount() == 37 )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_FIRE_SMACK_ATTACKED_0 , 0 , 2.5f , 
					m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
			}
			else if( GetSrcAniCount() == 58 )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_FIRE_SMACK_ATTACKED_1 , 0 , 2.5f , 
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


		case EF_FIRE_SMACK_2:
			
			if( GetSrcAniCount() == 34 )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_FIRE_SMACK_ATTACKED_1 , 0 , 2.5f , 
					m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

			}
			else if( GetSrcAniCount() == 46 )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_FIRE_SMACK_ATTACKED_1 , 0 , 2.5f , 
					m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

			}
			else if( GetSrcAniCount() == 69 )
			{
				g_Particle.SetEffSetCon( 0.0f , EF_FIRE_SMACK_ATTACKED_2 , 0 , 2.5f , 
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

int CEFFireSmack::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	return TRUE;
}

int	CEFFireSmack::Render(float dtime)
{
	return TRUE;
}

//................................................................................................................
// 이펙트 처리후 뒤로올 이펙트 셋업
//................................................................................................................
int CEFFireSmack::SetAfterEffect(void)
{
	// 데미지 효과

	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
