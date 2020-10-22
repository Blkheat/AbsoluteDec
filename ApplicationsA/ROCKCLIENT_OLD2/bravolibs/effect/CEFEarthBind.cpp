#include "CEFEarthBind.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFEarthBind::CEFEarthBind()
{
	m_IsConEffect = TRUE;
	
	m_InitTime = 0;
	m_PrevTime = 0;
	m_GapTime = 0;

	m_fRadius = 0.0f;

	m_ISDisplay = FALSE;
	m_ISShot = FALSE;
}

CEFEarthBind::~CEFEarthBind()
{

}

int	CEFEarthBind::CheckFrame(void)
{
	if( m_ISShot == TRUE )
	{
		return TRUE;
	}	
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		m_ISShot = TRUE;	
		m_ISDamageProcess = TRUE;
	}	
	
	if( m_SrcAniFrame < m_pSrcChr->m_ani_frame )
	{
		m_SrcAniFrame = m_pSrcChr->m_ani_frame;
	}

	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index )
	{
		m_pSrcChr->m_start_blend_frame = 0;

		m_ISShot = TRUE;
		m_ISDamageProcess = TRUE;	
	}

	// 임펙트 프레임 화살 나가는 시점
	if( m_ISShot || GetSrcAniCount() == 44 )
	{
		m_ISShot = TRUE;
		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

		m_fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );
		
		m_InitTime = g_nowTime;
		m_PrevTime = g_nowTime;
		m_GapTime = 0;
		
		m_ISDamageProcess = TRUE;
		m_ISDisplay = TRUE;

		D3DXVECTOR3  vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vPos.x = m_EffectTargetInfo.vPos.x;
		vPos.z = m_EffectTargetInfo.vPos.z;

		g_Particle.SetEffSetCon( 0.0f , EF_EARTH_BIND , ESLT_POS , 4.1f , -1 , -1 , vPos, 0.0f );

		for(int i = 0; i < 20 ; i++)
		{
			float fR = Random( -m_fRadius , m_fRadius );
			D3DXVECTOR3  vPos = D3DXVECTOR3( fR , 0.0f , 0.0f );
			D3DXMATRIX   matRot;
			D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
			///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
			D3DXVec3TransformCoord( &vPos , &vPos , &matRot );
			vPos += D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		

			vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );

			g_Particle.SetEffSetCon( 0.0f , EF_EARTH_BIND_SMOG , ESLT_POS , 2.1f , -1 , -1 , vPos, 0.0f );
		}

		m_ISDamageProcess = TRUE;
	}	
	
	return TRUE;
}

void CEFEarthBind::Init()
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();
		return;
	}

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	///-- 이곳에서 스킬의 정보를 알아옴
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( m_pSrcChr->m_curt_event.count );
	if( skill_Info != NULL )
	{
		if(skill_Info->theNeedSpelling)
		{
#ifdef SNIPER_MOD_MOTION			
			m_pSrcChr->m_ani_frame = 44 - 10;
#else
			m_pSrcChr->m_ani_frame = 20;
#endif							
		}
	}
}

int CEFEarthBind::Update(float dtime)
{
	if( !m_ISShot )
	{
		return TRUE;
	}	
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_ISDamageProcess )
	{
		if( m_pExtraInfo != NULL )
		{
			// 1:N 데미지
			int Count = m_pExtraInfo->GetTargetCount();
			
			for( int i = 0 ; i < Count ; ++i )
			{
				if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
				{
					g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
				}

				Info.Clear();
			}
		}	
		
		m_ISDamageProcess = FALSE;	
	}

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , 5100 ) )
	{
		SetDestroy();
		return TRUE;
	}
	
	return TRUE;
}

int	CEFEarthBind::Render(float dtime)
{
	if( !m_ISShot )
	{
		if(m_pSrcChr->m_pre_ani_number != -1)
		{
			return TRUE;
		}		
		
		// 임펙트 프레임이 1 이라서 여기서 찍어준다 된장
		g_RenderManager.SetTransform( D3DTS_WORLD, &m_pSrcChr->GetRHandTM() );
								
		g_RenderManager.SetCullMode( D3DCULL_CCW );
		
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );
		
		g_RenderManager.SetAlphaBlendEnable( FALSE );

		g_Particle.m_ArrowDisplay.Render( g_lpDevice );
	}
	
	return TRUE;	
}