#include "CEFBigNpcSkill0.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

#include "..\\..\\CRenderManager.h"

extern long g_nowTime;


CEFBigNpcSkill0::CEFBigNpcSkill0()
{
	m_IsConEffect = TRUE;
}


CEFBigNpcSkill0::~CEFBigNpcSkill0()
{

}

void CEFBigNpcSkill0::Init(void)
{

	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return;
	}
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	m_Code = g_Map.m_NpcTable[ m_pSrcChr->m_nTableIndex ].code;

	
	int SoundNum = 0;

	switch( m_Code )
	{
		case 3800:
			m_ImpactFrame = 164;
			SoundNum = 284;
			g_Particle.SetEffSetCon( 0.5f , EF_BIG_NPC_SKILL0 , ESLT_CHARPOS , 6.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
			m_AttackEDEffectID = EF_BIG_NPC_SKILL0_ED;
			break;
		case 3801:
			m_ImpactFrame = 200;
			SoundNum = 285;
			g_Particle.SetEffSetCon( 0.0f , EF_BIG_NPC_SKILL1 , ESLT_CHARPOS , 7.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
			m_AttackEDEffectID = EF_BIG_NPC_SKILL1_ED;
			break;
	}

	D3DXVECTOR3 vDistance;

	vDistance = D3DXVECTOR3( m_pSrcChr->GetPosTM()._41  , 0.0f , m_pSrcChr->GetPosTM()._43 ) - 
				D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41 , 0.0f , g_Pc.GetPlayer()->GetPosTM()._43 );
		
	if( D3DXVec3Length( &vDistance ) <= 800 )
	{
		//................................................................................	
		// 스킬 발동 사운드
		//................................................................................	
		g_Sound.Play( SoundNum , NULL );
	}

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;	
	
	if( NULL != m_pExtraInfo )
	{
		int Count = m_pExtraInfo->GetTargetCount();		
		
		for( int i = 0 ; i < Count ; ++i )
		{
			if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
			{
				if( pTempDest == g_Pc.GetPlayer() )
				{
					if( Info.TargetInfo.theDieFlag == TRUE )
					{
						switch( m_Code )
						{
						case 3800:
							g_Particle.RockCamera( 5000 );
							break;
							
						case 3801:
							g_Particle.RockCamera( 6000 );
							break;
						}
					}
				}
			}

			Info.Clear();
		}
	}
}


int	CEFBigNpcSkill0::CheckFrame(void)
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
	
	if( m_pSrcChr->m_ani_frame == m_ImpactFrame )
	{
		SetAfterEffect();
		SetDestroy();
	}

	return TRUE;
}

int	CEFBigNpcSkill0::Render(float dtime)
{
	return TRUE;
}

int CEFBigNpcSkill0::Update(float dtime)
{
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;	
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	return TRUE;
}

int CEFBigNpcSkill0::SetAfterEffect(void)
{
	// 데스트 없으면 소스쪽만 이펙트 출력
	if( m_pExtraInfo == NULL )
		return TRUE;

	// 1:N 데미지
	int Count = m_pExtraInfo->GetTargetCount();

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	for( int i = 0 ; i < Count ; ++i )
	{
		if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
		{
			g_Particle.SetEffSetCon( 0.0f , m_AttackEDEffectID , 0 , 2.1f , 
				pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

			Info.AfterEffect = m_AttackEDEffectID;
			Info.DestType = 0;
			Info.lifeTime = 2.1f;
			
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );

			//................................................................................	
			// 어택 사운드
			//................................................................................	
			if( pTempDest != NULL )
				g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );
		}

		Info.Clear();
	}


	return TRUE;
}
