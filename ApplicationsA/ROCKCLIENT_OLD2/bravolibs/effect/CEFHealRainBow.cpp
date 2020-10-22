#include "CEFHealRainBow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "CEFArrow.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFHealRainBow::CEFHealRainBow()
{
	m_IsConEffect = TRUE;
	m_ISShot = FALSE;
}

CEFHealRainBow::~CEFHealRainBow()
{
}

int	CEFHealRainBow::CheckFrame(void)
{
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
	
		if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 354 ) ) )
		{
			return FALSE;
		}

		m_Billboard.MakeLink( pData );

		// 스킬 범위
		m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 150.0f , 0.0f , 1000 , 1500 , 15 , 
			g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode ) , g_nowTime , 3000 );

		m_ISDamageProcess = TRUE;
		return TRUE;
	}	

	// 임펙트 프레임 화살 나가는 시점
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index || GetSrcAniCount() == 44 )
	{
		m_ISShot = TRUE;
		
		//................................................................................	
		// 발동 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );	

		CBillboardData *pData = NULL ;
		
		if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 354 ) ) )
		{
			return FALSE;
		}

		m_Billboard.MakeLink( pData );

		// 스킬 범위
		m_OldTime = g_nowTime;
		m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 150.0f , 0.0f , 1000 , 1500 , 15 , g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode ) , g_nowTime , 3000 );

		m_ISDamageProcess = TRUE;
	}	
	
	return TRUE;
}

void CEFHealRainBow::Init(void)
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

int CEFHealRainBow::Update(float dtime)
{
	if( FALSE == m_ISShot )
	{
		return TRUE;
	}	
	
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		///-- 공격 주체가 죽는다해도 3.2초후까지는 데미지처리를 해준다.	
		/*		
		SetDestroy();

		return TRUE;*/
	}
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 3100 ) )
	{
		SetDestroy();

		return TRUE;
	}

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
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
						g_Particle.SetEffSetCon( 0.0f , EF_HEAL_RAIN_BOWED , 0 , 3.1f , 
							pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
						
						Info.AfterEffect = EF_HEAL_RAIN_BOWED;
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

int	CEFHealRainBow::Render(float dtime)
{
	if( m_ISShot )
	{	
		if( m_EffectDrop.GetDataSize() )
		{
			DROP_EFFECT_DATA *pData = NULL;
			D3DXMATRIX		  matLocal;

			D3DXMatrixIdentity( &matLocal );

			m_EffectDrop.StartPosition();

			while( ( pData = m_EffectDrop.GetDropData() ) != NULL )
			{
				if( pData->State == DROPST_DROP	|| pData->State == DROPST_END )
				{
					if( pData->State == DROPST_END )
					{
						g_Particle.SetEffSetCon( 0.0f , EF_HEALED0 , 100 , 1.6f , -1 , -1 , pData->vPos );
					}
	/*				
					matLocal._41 = pData->vPos.x;
					matLocal._42 = pData->vPos.y;
					matLocal._43 = pData->vPos.z;
	*/
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
