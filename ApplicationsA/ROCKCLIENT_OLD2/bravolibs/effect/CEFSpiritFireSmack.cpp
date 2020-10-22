#include "CEFSpiritFireSmack.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFSpiritFireSmack::CEFSpiritFireSmack()
{
	m_IsConEffect = TRUE;
	m_fRightHandHeight = 0.0f;
}

CEFSpiritFireSmack::~CEFSpiritFireSmack()
{

}

void CEFSpiritFireSmack::Init(void)
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

int CEFSpiritFireSmack::CheckFrame(void)
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

	if( GetSrcAniCount() == 38 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}

	if( GetSrcAniCount() == 60 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
	}
	
	m_pSrcChr->BoneRender();

	D3DXVECTOR3 vPos; 
	
	if( GetSrcAniCount() <= 38 )
	{
	
		vPos = D3DXVECTOR3( m_pSrcChr->GetRealRHandTM()._41,
							m_pSrcChr->GetRealRHandTM()._42,
							m_pSrcChr->GetRealRHandTM()._43 );	
	}
	else
	{
		vPos = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41,
							m_pSrcChr->GetRealLHandTM()._42,
							m_pSrcChr->GetRealLHandTM()._43 );
	}

	g_Particle.DisplayUpdate( PT_ICESPEAR_TAIL , vPos );
	
	if( GetSrcAniCount() == 38 )
	{
		//................................................................................	
		// 어택 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );	
		
		
		m_fRightHandHeight = vPos.y;

		g_Particle.SetEffSetCon( 0.0f , EF_FIRESTORM_SMASH , ESLT_POS , 3.1f , 
								-1 , -1 , vPos, m_pSrcChr->GetDirection() );
		g_Particle.SetEffSetCon( 0.0f , EF_FIRESTORM_SMASHED , ESLT_CHARPOS , 
								 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	}
	else if( GetSrcAniCount() == 60 )
	{
		//................................................................................	
		// 어택 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
		
		D3DXVECTOR3 vTempPos;
		D3DXMATRIX matRotY;			
		D3DXMatrixRotationY( &matRotY , m_pSrcChr->GetDirection() );		
		D3DXVec3TransformCoord( &vTempPos , &D3DXVECTOR3(0.0f, 0.0f, -1.0f) , &matRotY );

		vPos = D3DXVECTOR3( m_pSrcChr->GetBonCenterTM()._41,
							m_pSrcChr->GetBonCenterTM()._42,
							m_pSrcChr->GetBonCenterTM()._43 ) + (2.5f*vTempPos);

		vPos.y = m_fRightHandHeight;
		
		g_Particle.SetEffSetCon( 0.0f , EF_FIRESTORM_SMASH , ESLT_POS , 3.1f , 
								-1 , -1 , vPos, m_pSrcChr->GetDirection() );
		g_Particle.SetEffSetCon( 0.0f , EF_SPIRIT_FIRE_SMACKED , ESLT_CHARPOS , 
								 6.1f , m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

		SetAfterEffect();
		SetDestroy();		
		return TRUE;
	}
	
	return TRUE;
}

int CEFSpiritFireSmack::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr, m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}	
	
	return TRUE;
}

int	CEFSpiritFireSmack::Render(float dtime)
{
	return TRUE;
}

int	CEFSpiritFireSmack::SetAfterEffect(void)
{
	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_SPIRIT_FIRE_SMACKED;
		m_pExtraInfo->DestType = ESLT_CHARPOS;
		m_pExtraInfo->lifeTime = 6.1f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	
	return TRUE;
}