#include "CEFIceBowRain.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "CEFArrow.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFIceBowRain::CEFIceBowRain()
{
	m_IsConEffect = TRUE;
	m_ISShot = FALSE;
	
	m_dNpcRealFileCode = 0;
}

CEFIceBowRain::~CEFIceBowRain()
{

}

int	CEFIceBowRain::CheckFrame(void)
{
	WORD aSkillCode = 0;

	if( m_pExtraInfo != NULL )
	{
		aSkillCode = m_pExtraInfo->SkillCode;
	}	
	
	if( m_ISShot == TRUE )
	{
		return TRUE;
	}

	if( m_SrcAniFrame < m_pSrcChr->m_ani_frame )
	{
		m_SrcAniFrame = m_pSrcChr->m_ani_frame;
	}
	
	if(m_Src_Ani_Index != m_pSrcChr->m_ani_index)
	{
		m_pSrcChr->m_start_blend_frame = 0;
	}

	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		m_ISShot = TRUE;

		m_OldTime = g_nowTime;

		CBillboardData *pData = NULL ;
	
		if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 24350 ) ) )
		{
			return FALSE;
		}

		m_Billboard.MakeLink( pData );

		// 스킬 범위
		m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 
			150.0f , 0.0f , 800 , 1000 , 15  , g_Particle.m_EffectManager.GetAttackRange( aSkillCode ) , 
			g_nowTime , 3000 );

		m_ISDamageProcess = TRUE;
		return TRUE;
	}
	
	int nCheckFrame = 56;

	if( m_dNpcRealFileCode )
	{
		if( m_dNpcRealFileCode == 21100019 )
		{
			nCheckFrame = 28;
		}
	}

	// 임펙트 프레임 화살 나가는 시점
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index || GetSrcAniCount() == nCheckFrame )
	{
		m_ISShot = TRUE;
		
		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );	

		CBillboardData *pData = NULL ;
		
		if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 24350 ) ) )
		{
			return FALSE;
		}

		m_Billboard.MakeLink( pData );

		// 스킬 범위
		m_OldTime = g_nowTime;
		m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 
			150.0f , 0.0f , 800 , 1000 , 15 , g_Particle.m_EffectManager.GetAttackRange( aSkillCode ) , 
			g_nowTime , 3000 );

		m_ISDamageProcess = TRUE;
	}	
	
	return TRUE;
}

void CEFIceBowRain::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();
		return;
	}

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

	if( !m_pSrcChr->m_bPC && m_pSrcChr->m_State.nCharacterType != nCharacterTypePlayerNPC )
	{
		m_dNpcRealFileCode = g_Pc_Manager.GetNPCRealFileCode( g_Map.m_NpcTable[ m_pSrcChr->m_nTableIndex ].code );
	}
	
	if( m_dNpcRealFileCode )
	{
		return;
	}
	
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

int CEFIceBowRain::Update(float dtime)
{
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;	
	
	if( FALSE == m_ISShot )
	{
		return TRUE;
	}	
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 2100 ) )
	{
		SetDestroy();
		return TRUE;
	}	
	
	if( m_ISDamageProcess )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 1000 ) )
		{
			if( m_pExtraInfo != NULL )
			{
				// 1:N 데미지
				int Count = m_pExtraInfo->GetTargetCount();
				
				for( int i = 0 ; i < Count ; ++i )
				{
					if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
					{
						g_Particle.SetEffSetCon( 0.0f , EF_ICE_SPEARED , 0 , 3.1f , 
							pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
						
						Info.AfterEffect = EF_ICE_SPEARED;
						Info.DestType = 0;
						Info.lifeTime = 3.1f;
						
						g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
					}

					Info.Clear();
				}
			}
		
			m_ISDamageProcess = FALSE;
		}
	}


	m_EffectDrop.Update( ( DWORD )g_nowTime );

	return TRUE;
}

int	CEFIceBowRain::Render(float dtime)
{
	if( m_ISShot )
	{	
		D3DXMATRIX		  matIdent;
		D3DXMatrixIdentity( &matIdent );

		if( m_EffectDrop.GetDataSize() )
		{
			DROP_EFFECT_DATA *pData = NULL;			

			m_EffectDrop.StartPosition();

			while( ( pData = m_EffectDrop.GetDropData() ) != NULL )
			{
				if( pData->State == DROPST_DROP	|| pData->State == DROPST_END )
				{
					if( pData->State == DROPST_END )
					{
						g_Particle.SetEffSetCon( 0.0f , EF_ICE_RAIN , 100 , 1.6f , -1 , -1 , pData->vPos );
					}
	
					m_Billboard.m_vLoc = pData->vPos;

					g_Billboard.DisplayManager( m_Billboard , dtime );

					g_RenderManager.SetCullMode( D3DCULL_NONE );
					
					g_RenderManager.SetZWriteEnable( FALSE );
					g_RenderManager.SetZEnable( D3DZB_TRUE );
					
					g_RenderManager.SetAlphaBlendEnable( TRUE );
					g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
					g_RenderManager.SetDestBlend( D3DBLEND_ONE  );


#ifdef BATCH_RENDER
					m_Billboard.DrawBatchBillboard();
#ifdef DIRECT_VERSION_9_MJH
					LPDIRECT3DTEXTURE9	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#else
					LPDIRECT3DTEXTURE8	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#endif // DIRECT_VERSION_9_MJH
					if(prevBillboardTexture)
					{
						g_RenderManager.SetTexture( 0, prevBillboardTexture );
						g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
						g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
						g_RenderManager.EndBatchPrimitive();
						g_RenderManager.SetBeginBatchRendering(FALSE);
	}
#else	//	BATCH_RENDER
					m_Billboard.DrawBillboard();
#endif	//	BATCH_RENDER

					g_RenderManager.SetTransform( D3DTS_WORLD , &matIdent );
					
					g_Particle.DisplayUpdate( PT_ICESPEAR_TAIL , pData->vPos );
					
					g_RenderManager.SetCullMode( D3DCULL_CCW );
					
					g_RenderManager.SetZWriteEnable( TRUE );
					g_RenderManager.SetZEnable( D3DZB_TRUE );
					
					g_RenderManager.SetAlphaBlendEnable( FALSE );
				}
			}
		}
	}
	else
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
