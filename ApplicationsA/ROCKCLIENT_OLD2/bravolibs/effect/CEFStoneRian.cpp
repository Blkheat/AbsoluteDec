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
	// �ߵ� ����
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	CBillboardData *pData = NULL ;
	
	///-- � ��� �� ������ ������ ������ 
	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 24179 ) ) )
	{
		return;
	}

	///-- ������ �������� ��ũ
	m_BillboardStone.MakeLink( pData );

	// ��ų ����
	m_InitTime = g_nowTime;

	WORD aSkillCode = 0;
	
	if(m_pExtraInfo)
	{
	   aSkillCode = m_pExtraInfo->SkillCode;
	   m_fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );	   
	}	

	///-- Init(�߽� X��ǥ, �߽� Z��ǥ, ����, Z��ȸ������, �ּҳ��Ͻð�, �ִ볫�Ͻð�,
	///-- ���ϰ���, ȿ������, ����ð�, ����Ÿ��(����Ÿ���� ���ϱ����� �뵵), ���Ϲݺ�����
	m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 150.0f , 
	D3DXToRadian( -25 ) , 700 , 1200 , 8 , m_fRadius , g_nowTime , 2000 , TRUE );

	m_ISDamageProcess = TRUE;
	m_ISDisplay = TRUE;
	m_IsStart = FALSE;
}

int CEFStoneRian::Update(float dtime)
{
	///-- �������Ʈ�� ������Ʈ
	m_EffectDrop.Update( ( DWORD )g_nowTime );
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_ISDamageProcess )
	{
		///-- ����Ʈ ������.. 1�ʰ� ������ ������ ó��
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , 1000 ) )
		{
			if( m_pExtraInfo != NULL )
			{
				// 1:N ������
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
				///-- ���� ���¿��� ����Ÿ�� ����Ʈ�� �������Ѵ�.
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


