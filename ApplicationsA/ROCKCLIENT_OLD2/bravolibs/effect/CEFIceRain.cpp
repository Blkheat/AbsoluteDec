#include "CEFIceRain.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "CEFArrow.h"
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFIceRain::CEFIceRain()
{
	m_ISDisplay = FALSE;
	m_Tex = NULL;
	m_IsConEffect = TRUE;
}

CEFIceRain::~CEFIceRain()
{
	m_Tex = NULL;
}

void CEFIceRain::Init(void)
{
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	
	//g_Particle.SetEffSetCon( 0.0f , m_ID , 100 , 2.1f , -1 , -1 , vPos );

	CBillboardData *pData = NULL ;
	
	///-- ���� ��� �� ������ ������ ������ 
	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 22315 ) ) )
	{
		return;
	}

	///-- ������ �������� ��ũ
	m_BillboardIce.MakeLink( pData );

	// ��ų ����
	m_OldTime = g_nowTime;	
	
	// �׽�Ʈ �ڵ�
	if( m_EffectTargetInfo.vPos.x == 0.0f &&  m_EffectTargetInfo.vPos.z == 0.0f )
	{
		float fRadius = 0.0f;
		fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );	
		
		m_EffectDrop.Init( g_Pc.GetPlayer()->GetPosTM()._41 , g_Pc.GetPlayer()->GetPosTM()._43 , 150.0f , 
			D3DXToRadian( -25 ) , 700 , 1200 , 8 , fRadius , g_nowTime , 2000 , TRUE );
	}
	else
	{
		WORD aSkillCode = 0;
		///-- ȿ�� ����
		float fRadius = 0.0f;
		
		if(m_pExtraInfo)
		{
		   aSkillCode = m_pExtraInfo->SkillCode;
		   fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );
		}

		///-- Init(�߽� X��ǥ, �߽� Z��ǥ, ����, Z��ȸ������, �ּҳ��Ͻð�, �ִ볫�Ͻð�,
		///-- ���ϰ���, ȿ������, ����ð�, ����Ÿ��(����Ÿ���� ���ϱ����� �뵵), ���Ϲݺ�����
		m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 150.0f , 
		D3DXToRadian( -25 ) , 700 , 1200 , 8 , fRadius , g_nowTime , 2000 , TRUE );	
	}

	m_ISDamageProcess = TRUE;

	//m_Tex = g_Particle.m_EffectTextureManager.GetTexture( "icerain_01.tga" );
	CStringToCode	HashString( "icerain_01.tga" );
	m_HashCode = HashString.GetHashCode();

	m_Tail.InitDevice( g_lpDevice );

	m_ISDisplay = TRUE;
}

int CEFIceRain::Update(float dtime)
{
/*
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}
*/
	D3DXVECTOR3	vPlayer( g_Pc.GetPlayer()->GetPosTM()._41 , 0.0f , g_Pc.GetPlayer()->GetPosTM()._43 );

	vPlayer -= m_EffectDrop.GetCenterPos();
	float fDistance = D3DXVec3Length( &vPlayer);
	
	if( fDistance >= 600.0f )
	{
		//m_ISDisplay = FALSE;		
		//return TRUE;
	}
	else
		m_ISDisplay = TRUE;

	
	///-- �������Ʈ�� ������Ʈ
	m_EffectDrop.Update( ( DWORD )g_nowTime );
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_ISDamageProcess )
	{
		///-- ����Ʈ ������.. 1�ʰ� ������ ������ ó��
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 1000 ) )
		{
			if( m_pExtraInfo != NULL )
			{
				// 1:N ������
				int Count = m_pExtraInfo->GetTargetCount();			
				
				for( int i = 0 ; i < Count ; ++i )
				{
					if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
					{
						Info.AfterEffect = EF_ICE_RAINED;
						Info.DestType = 0;
						Info.lifeTime = 5.1f;
						
						g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );					
						
						g_Particle.SetEffSetCon( 0.0f , EF_ICE_RAINED , 0 , 5.1f , 
							pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
					}

					Info.Clear();
				}
			}
		
			m_ISDamageProcess = FALSE;
		}
	}

	///-- ���̽� ������ �����ϴ� ��ƾ�� ����... ���̽� ������ ���ѹݺ��̶� 
	///-- �̷��� ���� ������ �������� ����	
	///-- �ϴ� ������Ŵ By simwoosung
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 5100 ) )
	{
		m_EffectDrop.SetIsLoop(FALSE);
		return TRUE;
	}

	return TRUE;
}

int	CEFIceRain::Render(float dtime)
{
	if( !m_ISDisplay )
		return TRUE;
	
	if( m_EffectDrop.GetDataSize() )
	{
		D3DXVECTOR3 vPos = D3DXVECTOR3(m_EffectTargetInfo.vPos.x, 0.0f, m_EffectTargetInfo.vPos.z);			
		vPos.y = 0.0f;
		
		D3DXVECTOR3 vPlayerPos = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41,
											  0.0f,
											  g_Pc.GetPlayer()->GetPosTM()._43 );			
		
		int nPlayerDistance = (int)D3DXVec3Length( &(vPlayerPos - vPos) );	
		if( nPlayerDistance > g_Max_Effect_Display )
		{
			return TRUE;
		}		
		
		DROP_EFFECT_DATA *pData = NULL;
		D3DXMATRIX		  matLocal;

		D3DXMatrixIdentity( &matLocal );

		m_EffectDrop.StartPosition();

		g_RenderManager.SetCullMode( D3DCULL_NONE );
		
		g_RenderManager.SetZWriteEnable( FALSE );
		g_RenderManager.SetZEnable( D3DZB_TRUE );
		
		D3DXVECTOR3 vDir = g_Camera.m_vDir;
		D3DXVec3Normalize( &vDir , &vDir );

		while( ( pData = m_EffectDrop.GetDropData() ) != NULL )
		{
			if( pData->State == DROPST_DROP	|| pData->State == DROPST_END )
			{
				///-- ���� ���¿��� ����Ÿ�� ����Ʈ�� �������Ѵ�.
				if( pData->State == DROPST_END )
				{
					g_Particle.SetEffSetCon( 0.0f , EF_ICE_RAIN , 100 , 1.6f , -1 , -1 , pData->vPos );
				}
/*				
				matLocal._41 = pData->vPos.x;
				matLocal._42 = pData->vPos.y;
				matLocal._43 = pData->vPos.z;
*/
				g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );				
				g_RenderManager.SetTexture( 0 , g_Particle.m_EffectTextureManager.GetTexture(m_HashCode) );				
				///-- ������ ���� ������Ʈ
				m_Tail.Update( pData->vPos , pData->vStartPos , 2.0f , vDir , g_lpDevice );

				g_RenderManager.SetAlphaBlendEnable( TRUE );
				g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
				g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );

				m_Tail.Render( g_lpDevice );

				m_BillboardIce.m_vLoc = pData->vPos;

				g_Billboard.DisplayManager( m_BillboardIce , dtime );
#ifdef BATCH_RENDER
				m_BillboardIce.DrawBatchBillboard();
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
				m_BillboardIce.DrawBillboard();
#endif	//	BATCH_RENDER
								
			}
		}

		g_RenderManager.SetCullMode( D3DCULL_CCW );
		
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( D3DZB_TRUE );		
	}
	else
	{
		SetDestroy();
		return TRUE;
	}
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	return TRUE;
}

