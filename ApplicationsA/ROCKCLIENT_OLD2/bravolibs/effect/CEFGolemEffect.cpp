#include "CEFGTEffect.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "CEFGolemEffect.h"
	 
CEFGolemEffect::CEFGolemEffect()
{
	m_IsConEffect = TRUE;
	
	m_nMiddleImpacFrame = 0;
	m_nLastImpacFrame = 0;
	m_uTaleType = 0;
	m_uRightHandEffId = 0;
	m_uRightHandType = 0;
	m_uLeftHandEffId = 0;
	m_uLeftHandType = 0;
	m_uExploEffId = 0;
	m_uExploEffType = 0;
	m_uAttackedEffect = 0;
	m_uAttackedType = 0;
}

CEFGolemEffect::~CEFGolemEffect()
{


}

int	CEFGolemEffect::CheckFrame(void)
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

	if( GetSrcAniCount() == m_nMiddleImpacFrame )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	if( GetSrcAniCount() == m_nLastImpacFrame )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	m_pSrcChr->BoneRender();

	D3DXVECTOR3 vLeftHandPos, vRightHandPos;
	D3DXMATRIX matBone;
	///-- 왼손
	m_pSrcChr->GetBoneLinkTM( 4 , matBone, FALSE );
	vLeftHandPos = D3DXVECTOR3(matBone._41, matBone._42, matBone._43);
	
	if( m_uTaleType && (GetSrcAniCount() % 3 == 0) )
	{
		g_Particle.DisplayUpdate( m_uTaleType , vLeftHandPos );
	}

	///-- 오른손
	m_pSrcChr->GetBoneLinkTM( 5 , matBone, FALSE );
	vRightHandPos = D3DXVECTOR3(matBone._41, matBone._42, matBone._43);

	if( m_uTaleType && (GetSrcAniCount() % 3 == 0) )
	{
		g_Particle.DisplayUpdate( m_uTaleType , vRightHandPos );
	}

	if( m_pSrcChr->m_ani_frame == (m_nMiddleImpacFrame - 5) )
	{
		int nNowFrame = m_pSrcChr->m_ani_frame;
		m_pSrcChr->m_ani_frame = m_nMiddleImpacFrame;		
		m_pSrcChr->m_IsApplyVA = TRUE;		
		m_pSrcChr->BoneRender();
		
		m_pSrcChr->GetBoneLinkTM( 4 , matBone, FALSE );
		vLeftHandPos = D3DXVECTOR3(matBone._41, matBone._42, matBone._43);
		m_pSrcChr->GetBoneLinkTM( 5 , matBone, FALSE );
		vRightHandPos = D3DXVECTOR3(matBone._41, matBone._42, matBone._43);
		
		D3DXVECTOR3 vTemPos = vLeftHandPos;
		vTemPos.y = g_Map.PHF_GetHeight( vTemPos.x , vTemPos.z );
		g_Particle.SetEffSetCon( 0.0f , EF_GIANT_GOLEM_ATTACK , ESLT_POS , 1.5f , -1 , -1 , vTemPos, 0.0f );
		vTemPos = vRightHandPos;
		vTemPos.y = g_Map.PHF_GetHeight( vTemPos.x , vTemPos.z );
		g_Particle.SetEffSetCon( 0.0f , EF_GIANT_GOLEM_ATTACK , ESLT_POS , 1.5f , -1 , -1 , vTemPos, 0.0f );
		
		m_pSrcChr->m_ani_frame = nNowFrame;
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->BoneRender();
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	if( m_pSrcChr->m_ani_frame == (m_nLastImpacFrame - 5) )
	{
		int nNowFrame = m_pSrcChr->m_ani_frame;
		m_pSrcChr->m_ani_frame = m_nLastImpacFrame;
		
		BOOL bFlag = m_pSrcChr->m_IsApplyVA;
		m_pSrcChr->m_IsApplyVA = TRUE;		
		m_pSrcChr->BoneRender();
		
		m_pSrcChr->GetBoneLinkTM( 4 , matBone, FALSE );
		vLeftHandPos = D3DXVECTOR3(matBone._41, matBone._42, matBone._43);
		m_pSrcChr->GetBoneLinkTM( 5 , matBone, FALSE );
		vRightHandPos = D3DXVECTOR3(matBone._41, matBone._42, matBone._43);		
		
		D3DXVECTOR3 vTemPos = vLeftHandPos;
		vTemPos.y = g_Map.PHF_GetHeight( vTemPos.x , vTemPos.z );
		g_Particle.SetEffSetCon( 0.0f , EF_GIANT_GOLEM_ATTACK , ESLT_POS , 1.5f , -1 , -1 , vTemPos, 0.0f );
		vTemPos = vRightHandPos;
		vTemPos.y = g_Map.PHF_GetHeight( vTemPos.x , vTemPos.z );
		g_Particle.SetEffSetCon( 0.0f , EF_GIANT_GOLEM_ATTACK , ESLT_POS , 1.5f , -1 , -1 , vTemPos, 0.0f );
		
		m_pSrcChr->m_ani_frame = nNowFrame;
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->BoneRender();
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}	
	
	if( GetSrcAniCount() == m_nMiddleImpacFrame )
	{
		///-- 주변 광역 폭팔 이펙트 시전
		if(m_uExploEffId)
		{
			///-- 오른손과 왼손사이 시전
			D3DXVECTOR3 vDir = vLeftHandPos - vRightHandPos;
			D3DXVECTOR3 vTemPos = vRightHandPos + (0.5f * vDir);
			vTemPos.y = g_Map.PHF_GetHeight( vTemPos.x , vTemPos.z );
			g_Particle.SetEffSetCon( 0.0f , m_uExploEffId , ESLT_POS , 1.5f , -1 , -1 , vTemPos, 0.0f );
		}		

		SetNoDamageEffect();		
		return TRUE;
	}	

	if( GetSrcAniCount() == m_nLastImpacFrame )
	{
		///-- 양손 링크이펙트 지우기
		if(m_uRightHandEffId)
		{
			g_Particle.DelEffSetCon( m_uRightHandEffId , m_uRightHandType , 
									  m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
		}

		if(m_uLeftHandEffId)
		{
			g_Particle.DelEffSetCon( m_uLeftHandEffId , m_uLeftHandType , 
									 m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
		}		
		
		///-- 주변 광역 폭팔 이펙트 시전
		if(m_uExploEffId)
		{
			///-- 오른손과 왼손사이 시전
			D3DXVECTOR3 vDir = vLeftHandPos - vRightHandPos;
			D3DXVECTOR3 vTemPos = vRightHandPos + (0.5f * vDir);
			vTemPos.y = g_Map.PHF_GetHeight( vTemPos.x , vTemPos.z );
			g_Particle.SetEffSetCon( 0.0f , m_uExploEffId , ESLT_POS , 1.5f , -1 , -1 , vTemPos, 0.0f );
		}		

		SetAfterEffect();
		SetDestroy();
		return TRUE;
	}	
	
	return TRUE;
}

int	CEFGolemEffect::Render(float dtime)
{
	return TRUE;
}	
						
int CEFGolemEffect::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}	
	
	return TRUE;
}

void CEFGolemEffect::SetNoDamageEffect()
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
				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );				
				
				if(m_uAttackedType == ESLT_SRC_RAD)
				{
					g_Particle.SetEffSetCon( 0.0f , m_uAttackedEffect , m_uAttackedType , 2.1f , pTempDest->GetUnique() , 
										 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ), m_pSrcChr->GetDirection() + D3DXToRadian( 180 ) );
				}
				else
				{	
					g_Particle.SetEffSetCon( 0.0f , m_uAttackedEffect , m_uAttackedType , 2.1f , pTempDest->GetUnique() , 
										 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				}		
			}

			Info.Clear();
		}
	}	

}

int CEFGolemEffect::SetAfterEffect(void)
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
				//................................................................................	
				// 어택 사운드
				//................................................................................	
				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , pTempDest );				
				
				if(m_uAttackedType == ESLT_SRC_RAD)
				{
					g_Particle.SetEffSetCon( 0.0f , m_uAttackedEffect , m_uAttackedType , 2.1f , pTempDest->GetUnique() , 
										 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ), m_pSrcChr->GetDirection() + D3DXToRadian( 180 ) );
				}
				else
				{	
					g_Particle.SetEffSetCon( 0.0f , m_uAttackedEffect , m_uAttackedType , 2.1f , pTempDest->GetUnique() , 
										 pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				}		
				
				Info.AfterEffect = m_uAttackedEffect;
				Info.DestType = m_uAttackedType;
				Info.lifeTime = 5.1f;
					
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );		
			}

			Info.Clear();
		}
	}	
	
	return TRUE;
}

void CEFGolemEffect::Init(void)
{
	if( m_pExtraInfo == NULL )
	{
		SetDestroy();
		return;
	}

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
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
	
	///-- 오른손과 왼손에 링크이펙트 붙이기
	if(m_uRightHandEffId)
	{
		g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
		g_Particle.SetEffSetCon( 0.0f , m_uRightHandEffId , m_uRightHandType , 
			EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
	}

	if(m_uLeftHandEffId)
	{
		g_Particle.m_IsGUnitLinkBoneAndSizeFix = TRUE;
		g_Particle.SetEffSetCon( 0.0f , m_uLeftHandEffId , m_uLeftHandType , 
			EFFECT_SET_LOOP_TIME , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );
	}
}

