#include "CEFTigerAttack.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long g_nowTime;

CEFTigerAttack::CEFTigerAttack()
{
	m_IsConEffect = TRUE;
}

CEFTigerAttack::~CEFTigerAttack()
{
}

void CEFTigerAttack::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return;
	}	
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );	

}


int CEFTigerAttack::CheckFrame(void)
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
		case EF_TIGER_SKILL_4:				

			if( GetSrcAniCount() == 38 )
			{
				g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , 4.5f , 
					m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				//................................................................................	
				// ���� ����
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

			}
			else if( GetSrcAniCount() == 55 )
			{
				//g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , 2.5f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex() );

				//................................................................................	
				// ���� ����
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
				
				SetDestroy();

				if(m_pExtraInfo)
				{
					m_pExtraInfo->AfterEffect = m_ID;
					m_pExtraInfo->DestType = ESLT_CHARPOS;
					m_pExtraInfo->lifeTime = 4.5f;
				}
	
				SetAfterEffect();
			}

			break;
		
		default:

			SetDestroy();

			break;
	}

	return TRUE;
}

int CEFTigerAttack::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	return TRUE;
}

int	CEFTigerAttack::Render(float dtime)
{
	return TRUE;
}

//................................................................................................................
// ����Ʈ ó���� �ڷο� ����Ʈ �¾�
//................................................................................................................
int CEFTigerAttack::SetAfterEffect(void)
{
	// ������ ȿ��
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );

	return TRUE;
}
