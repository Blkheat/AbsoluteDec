#include "CEFRoundLight.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFRoundLight::CEFRoundLight()
{
	m_fRadius = 0.0f;
	m_InitTime  = 0;
	m_PrevTime = 0;
	m_RunningTime = 500;
}

CEFRoundLight::~CEFRoundLight()
{


}

void CEFRoundLight::Init()
{
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	CBillboardData *pData = NULL;
	///-- 번개 빌보드 데이터 가져옴 
	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 24194 ) ) )
	{
		return;
	}

	m_fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );
	float fMinRadius = 0.0f;

	m_InitTime = g_nowTime;
	m_PrevTime = g_nowTime;
	
	m_ISDamageProcess = TRUE;
	m_ISDisplay = TRUE;

	D3DXVECTOR3 vMaxRad(m_fRadius, 0.0f, 0.0f);
	D3DXVECTOR3 vMinRad(fMinRadius, 0.0f, 0.0f);

	D3DXVECTOR3 vPos;	
	
	for(int i = 0 ; i < 18 ; i++ )
	{
		///-- 빌보드 렌더러에 링크
		m_BillboardLight[i].MakeLink( pData );
		m_BillboardLight[i].m_vLoc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	
		
		float fRad = (2 * D3DX_PI) * ( i / 18.0f );
		D3DXMATRIX	matRot;
		D3DXMatrixRotationY( &matRot, fRad );
		D3DXVec3TransformCoord( &m_vStartPos[i] , &vMinRad , &matRot );
		D3DXVec3TransformCoord( &m_vEndPos[i] , &vMaxRad , &matRot );
		
		if( i%3 == 0 )
		{
			D3DXVec3Lerp( &vPos , &m_vStartPos[i] , &m_vEndPos[i] , 0.3f );
			vPos += D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		
			vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
			g_Particle.SetEffSetCon( 0.0f , EF_LIGHT_STORM , ESLT_POS , 2.1f , -1 , -1 , 
									vPos, 0.0f, -1, 0.0f );		
		}	
	}
				
	vPos = D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );
	vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
	g_Particle.SetEffSetCon( 0.0f , EF_LIGHT_LANDSPARK , ESLT_POS , 0.5f , -1 , -1 , vPos, 0.0f );

	m_Tail.InitDevice( g_lpDevice );

	m_ISDamageProcess = TRUE;
	m_ISDisplay = TRUE;
}

int CEFRoundLight::Update(float dtime)
{
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
					Info.AfterEffect = EF_LIGHTNING_ATTACKED2;
					Info.DestType = 0;
					Info.lifeTime = 5.1f;
					
					g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );					
					
					g_Particle.SetEffSetCon( 0.0f , EF_LIGHTNING_ATTACKED2 , 0 , 5.1f , 
						pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				}

				Info.Clear();
			}
		}
	
		m_ISDamageProcess = FALSE;
	}	
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , m_RunningTime ) )
	{
		SetDestroy();		
		return TRUE;
	}
	
	return TRUE;
}

int	CEFRoundLight::Render(float dtime)
{
	D3DXVECTOR3 vPos = D3DXVECTOR3(m_EffectTargetInfo.vPos.x, 0.0f, m_EffectTargetInfo.vPos.z);	
	
	D3DXVECTOR3 vPlayerPos = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41,
										  0.0f,
										  g_Pc.GetPlayer()->GetPosTM()._43 );	
	
	int nPlayerDistance = (int)D3DXVec3Length( &(vPlayerPos - vPos) );	
	
	if( nPlayerDistance > g_Max_Effect_Display )
	{
		return TRUE;
	}	
	
	D3DXMATRIX		  matLocal;
	D3DXMatrixIdentity( &matLocal );		
	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	DWORD dFVF = g_RenderManager.GetFVF();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dLighting = g_RenderManager.GetLighting();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend(); 

	DWORD dColOp = g_RenderManager.GetTextureColorOP( 0 );
	DWORD dColArg1 = g_RenderManager.GetTextureColorArg1( 0 );
	DWORD dColArg2 = g_RenderManager.GetTextureColorArg2( 0 );

	DWORD dAlpOp = g_RenderManager.GetTextureAlphaOP( 0 );
	DWORD dAlpArg1 = g_RenderManager.GetTextureAlphaArg1( 0 );
	DWORD dAlpArg2 = g_RenderManager.GetTextureAlphaArg2( 0 );	
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	g_RenderManager.SetZWriteEnable( FALSE );	

	D3DXVECTOR3 vCPos;
	float fRange = SAFE_TIME_SUB(g_nowTime , m_InitTime)/(float)m_RunningTime;	
	if(fRange > 1.0f)
	{
	    fRange = 1.0f;
	}
	
	D3DXVECTOR3 vCamDir = g_Camera.m_vDir;
	D3DXVec3Normalize( &vCamDir , &vCamDir );
	float fSize = 2.0f + (8.0f - 2.0f) * fRange;

	for(int i = 0 ; i < 18 ; i++ )
	{
		D3DXVec3Lerp( &vPos , &m_vStartPos[i] , &m_vEndPos[i] , fRange );
		vCPos = D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		
		vCPos.y = g_Map.PHF_GetHeight( vCPos.x , vCPos.z ) + 5.0f;		
		//m_BillboardLight[i].m_vLoc = vPos;
		g_Billboard.DisplayManager( m_BillboardLight[i] , dtime );	
#ifdef BATCH_RENDER
		
		m_BillboardLight[i].DrawBatchBillboard(FALSE, TRUE);
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
		m_BillboardLight[i].DrawBillboard(FALSE, TRUE);
#endif	//	BATCH_RENDER
		

		m_Tail.Update(vPos + vCPos , m_vStartPos[i] + vCPos , fSize , vCamDir , g_lpDevice );
		m_Tail.Render(g_lpDevice);
	}

	g_RenderManager.SetFVF( dFVF );
	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
	g_RenderManager.SetCullMode( dCullMode );
	g_RenderManager.SetZEnable( dZEnable );
	g_RenderManager.SetZWriteEnable( dZWriteEnable );
	g_RenderManager.SetLighting( dLighting );
	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );

	g_RenderManager.SetTextureColorOP  ( 0 , dColOp );
	g_RenderManager.SetTextureColorArg1( 0 , dColArg1 );
	g_RenderManager.SetTextureColorArg2( 0 , dColArg2 );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , dAlpOp );
	g_RenderManager.SetTextureAlphaArg1( 0 , dAlpArg1 );
	g_RenderManager.SetTextureAlphaArg2( 0 , dAlpArg2 );
	
	return TRUE;	
}