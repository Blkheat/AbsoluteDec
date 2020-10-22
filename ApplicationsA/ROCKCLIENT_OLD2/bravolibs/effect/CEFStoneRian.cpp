#include "CEFStoneRian.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFStoneRian::CEFStoneRian()
{
	m_ISDisplay = FALSE;	
	m_IsConEffect = TRUE;

	m_fRadius = 0.0f;
}

CEFStoneRian::~CEFStoneRian()
{

}

void CEFStoneRian::Init(void)
{
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	CBillboardData *pData = NULL ;
	
	///-- 운석 모양 모델 빌보드 데이터 가져옴 
	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 24179 ) ) )
	{
		return;
	}

	///-- 빌보드 렌더러에 링크
	m_BillboardStone.MakeLink( pData );

	// 스킬 범위
	m_InitTime = g_nowTime;

	WORD aSkillCode = 0;
	
	if(m_pExtraInfo)
	{
	   aSkillCode = m_pExtraInfo->SkillCode;
	   m_fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );	   
	}	

	///-- Init(중심 X좌표, 중심 Z좌표, 높이, Z축회전각도, 최소낙하시간, 최대낙하시간,
	///-- 낙하개수, 효과범위, 현재시간, 러닝타임(간격타임을 구하기위한 용도), 낙하반복여부
	m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 150.0f , 
	D3DXToRadian( -25 ) , 700 , 1200 , 8 , m_fRadius , g_nowTime , 2000 , TRUE );

	m_ISDamageProcess = TRUE;
	m_ISDisplay = TRUE;
	m_IsStart = FALSE;
}

int CEFStoneRian::Update(float dtime)
{
	///-- 드롭이펙트위 업데이트
	m_EffectDrop.Update( ( DWORD )g_nowTime );
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_ISDamageProcess )
	{
		///-- 이펙트 시전후.. 1초가 지나면 데미지 처리
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , 1000 ) )
		{
			if( m_pExtraInfo != NULL )
			{
				// 1:N 데미지
				int Count = m_pExtraInfo->GetTargetCount();			
				
				for( int i = 0 ; i < Count ; ++i )
				{
					if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
					{
						Info.AfterEffect = EF_STONE_RAINED;
						Info.DestType = 0;
						Info.lifeTime = 5.1f;
						
						g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );					
						
						g_Particle.SetEffSetCon( 0.0f , EF_STONE_RAINED , 0 , 5.1f , 
							pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );						
					}

					Info.Clear();
				}
			}
		
			m_ISDamageProcess = FALSE;
		}
	}

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , 5100 ) )
	{
		m_EffectDrop.SetIsLoop(FALSE);
		return TRUE;
	}
	
	return TRUE;
}

int	CEFStoneRian::Render(float dtime)
{
	if( !m_ISDisplay )
		return TRUE;

	D3DXMATRIX		  matLocal;
	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetZEnable( D3DZB_TRUE );
	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	
	if( m_EffectDrop.GetDataSize() )
	{
		DROP_EFFECT_DATA *pData = NULL;	
		m_EffectDrop.StartPosition();

		while( ( pData = m_EffectDrop.GetDropData() ) != NULL )
		{
			if( pData->State == DROPST_DROP	|| pData->State == DROPST_END )
			{
				///-- 종료 상태에서 지형타격 이펙트를 렌더링한다.
				if( pData->State == DROPST_END )
				{
					g_Particle.SetEffSetCon( 0.0f , EF_STONE_RAIN_STONE , 100 , 1.6f , -1 , -1 , pData->vPos );
					g_Particle.SetEffSetCon( 0.0f , EF_STONE_RAIN , 100 , 1.6f , -1 , -1 , pData->vPos );
			
					if(!m_IsStart)
					{
						D3DXVECTOR3  vPos = D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		
						vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );						
						g_Particle.SetEffSetCon( 0.0f , EF_STONE_RAIN_RING , 100 , 1.6f , -1 , -1 , vPos );
						m_IsStart = TRUE;
					}
				}				

				g_RenderManager.SetCullMode( D3DCULL_CCW );		
				g_RenderManager.SetZWriteEnable( TRUE );							

				m_BillboardStone.m_vLoc = pData->vPos;

				g_Billboard.DisplayManager( m_BillboardStone , dtime );
#ifdef BATCH_RENDER
				m_BillboardStone.DrawBatchBillboard();
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
				m_BillboardStone.DrawBillboard();
#endif	//	BATCH_RENDER
				
				
				g_RenderManager.SetCullMode( D3DCULL_NONE );		
				g_RenderManager.SetZWriteEnable( FALSE );

				g_Particle.SetEffSetCon( 0.0f , EF_STONE_RAIN_TAIL , 100 , 1.6f , -1 , -1 , pData->vPos );
				
				g_RenderManager.SetCullMode( D3DCULL_CCW );		
				g_RenderManager.SetZWriteEnable( TRUE );	
			}
		}		
	}
	else
	{
		SetDestroy();
		return TRUE;
	}
	
	return TRUE;
}


