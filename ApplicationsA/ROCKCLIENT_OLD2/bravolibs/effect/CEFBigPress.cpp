#include "CEFBigPress.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>

extern long		 g_nowTime;

CEFBigPress::CEFBigPress()
{
	m_IsConEffect = TRUE;
}

CEFBigPress::~CEFBigPress()
{
}

void CEFBigPress::Init(void)
{
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	
	CBillboardData *pData = NULL ;
	
	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 22110 ) ) )
	{
		return;
	}

	m_BillRock.MakeLink( pData );

	m_OldTime = g_nowTime;

	m_vEndPos   = m_EffectTargetInfo.vPos;
	m_vEndPos.y = g_Map.Get__MapHeightF( m_vEndPos.x , m_vEndPos.z );
	
	D3DXMATRIX matRot;
	D3DXVECTOR3 vDir( 0.0f , 1.0f , 0.0f );

	D3DXMatrixRotationZ( &matRot , D3DXToRadian( -25 ) );
	D3DXVec3TransformCoord( &vDir , &vDir , &matRot );

	D3DXVec3Normalize( &vDir , &vDir );

	m_vStartPos = m_vEndPos + ( vDir * 300.0f );
	
	m_Range.SetMaxTime( 400 );
	m_Range.Insert( 0    , 0.0f );
	m_Range.Insert( 400  , 1.0f );
}

int CEFBigPress::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		///-- 공격주체가 죽는다해도 이미들어온 데미지 처리는 해야한다.
		
		/*SetDestroy();
		return TRUE;*/
	}
	
	float Range = 0.0f;

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_Range.GetValue( ( SAFE_TIME_SUB( g_nowTime , m_OldTime ) ) , Range ) == FALSE )
	{
		
		g_Particle.SetEffSetCon( 0.0f , EF_BIG_PRESS , 100 , 4.1f , -1 , -1 , m_vEndPos );
		
		if( m_pExtraInfo != NULL )
		{
			// 1:N 데미지
			int Count = m_pExtraInfo->GetTargetCount();			
			
			for( int i = 0 ; i < Count ; ++i )
			{
				if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
				{
					g_Particle.SetEffSetCon( 0.0f , EF_BIG_PRESSED , 0 , 4.1f , 
											pTempDest->GetUnique() , pTempDest->GetIndex(), 
											::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
					
					Info.AfterEffect = EF_BIG_PRESSED;
					Info.DestType = 0;
					Info.lifeTime = 4.1f;
					
					g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
				}

				Info.Clear();
			}
		}

		if( m_pSrcChr == g_Pc.GetPlayer() )
			g_Particle.RockCamera( ( rand() % 200 ) + 300 );

		SetDestroy();
	}
	else
	{
		D3DXVec3Lerp( &m_vPos , &m_vStartPos , &m_vEndPos , Range );

		m_BillRock.m_vLoc = m_vPos;
		
		g_Billboard.DisplayManager( m_BillRock , dtime );	
		
		///-- 파티클 꼬리를 붙인다.
		g_Particle.SetEffSetCon( 0.0f , EF_BIG_PRESS_TAIL , 100 , 1.1f , -1 , -1 , m_vPos );
	}
	
	return TRUE;
}

int	CEFBigPress::Render(float dtime)
{
	D3DXMATRIX		  matLocal;

	D3DXMatrixIdentity( &matLocal );
	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );

#ifdef BATCH_RENDER
	m_BillRock.DrawBatchBillboard();
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
	m_BillRock.DrawBillboard();
#endif	//	BATCH_RENDER
	

	return TRUE;
}
