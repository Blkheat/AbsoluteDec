#include "CEFCastStone.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFCastStone::CEFCastStone()
{
	m_IsConEffect = TRUE;

	m_InitTime = 0;
	D3DXMatrixIdentity( &m_matLocal );
	m_RangeTime = 0;
	m_vStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	m_fXRadian = 0.0f;
	m_fYRadian = 0.0f;
}

CEFCastStone::~CEFCastStone()
{

}

void CEFCastStone::Init()
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	CBillboardData *pData = NULL;
	///-- 돌 모양 모델 빌보드 데이터 가져옴 
	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 24211 ) ) )
	{
		return;
	}

	///-- 빌보드 렌더러에 링크
	m_BillboardStone.MakeLink( pData );
	m_BillboardStone.m_vLoc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	m_InitTime = g_nowTime;

	m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealRHandTM()._41 , 
								 m_pSrcChr->GetRealRHandTM()._42 ,
								 m_pSrcChr->GetRealRHandTM()._43  );

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	m_RangeTime	  = PositionToTimer( 50 , m_vStartPos , vDestPos );

	D3DXMATRIX	m_matRotX, m_matRotY;
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
	
	m_fXRadian = 0.0f;
	m_fYRadian = m_pSrcChr->m_radian;

	GetXAndYRadian(m_fXRadian, m_fYRadian, m_vStartPos, vDestPos);

	D3DXMATRIX matRotX, matRotY;

	D3DXMatrixRotationX( &matRotX , m_fXRadian + D3DXToRadian( 90 ) );
	D3DXMatrixRotationY( &matRotY , m_fYRadian );
	D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );

	m_matLocal._41 = m_vStartPos.x;
	m_matLocal._42 = m_vStartPos.y;
	m_matLocal._43 = m_vStartPos.z;

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
}

int CEFCastStone::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}

	float Range = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_InitTime ) ) / ( float ) m_RangeTime ) );

	if( Range >= 1.0f ) 
	{
		Range = 1.0f;

		SetDestroy();	
		SetAfterEffect();
		
		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
	D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );

	GetXAndYRadian(m_fXRadian, m_fYRadian, vPos, vDestPos);

	D3DXMATRIX matRotX, matRotY;

	D3DXMatrixRotationX( &matRotX , m_fXRadian + D3DXToRadian( 90 ) );
	D3DXMatrixRotationY( &matRotY , m_fYRadian );
	D3DXMatrixMultiply( &m_matLocal , &matRotX , &matRotY );
	
	m_matLocal._41 = vPos.x;
	m_matLocal._42 = vPos.y;
	m_matLocal._43 = vPos.z;	
	
	return TRUE;
}

int	CEFCastStone::Render(float dtime)
{
	D3DXMATRIX matIdent;
	D3DXMatrixIdentity( &matIdent );
	D3DXVECTOR3 vPos = D3DXVECTOR3(m_matLocal._41, m_matLocal._42, m_matLocal._43);
		
	///-- 아이스 결정체 그리기
	g_RenderManager.SetTransform( D3DTS_WORLD , &matIdent );
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetZWriteEnable( TRUE );
	
	m_BillboardStone.m_vLoc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BillboardStone.m_matPostWorld = m_matLocal;

	g_Billboard.DisplayManager( m_BillboardStone , dtime );	
#ifdef BATCH_RENDER
	m_BillboardStone.DrawBatchBillboard(TRUE);
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
	m_BillboardStone.DrawBillboard(TRUE);
#endif	//	BATCH_RENDER
	
	
	///-- 아이스 꼬리 그리기
	g_RenderManager.SetTransform( D3DTS_WORLD , &matIdent );
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetZWriteEnable( FALSE );
	
	g_Particle.DisplayUpdate( PT_CASTSTONE_TAIL , vPos );

	g_RenderManager.SetCullMode( D3DCULL_CCW );		
	g_RenderManager.SetZWriteEnable( TRUE );
	
	
	return TRUE;	
}

int CEFCastStone::SetAfterEffect(void)
{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );

	g_Particle.SetEffSetCon( 0.0f , EF_CAST_STONEED , 0 , 2.1f , m_pDestChr->GetUnique() , 
							 m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_CAST_STONEED;
		m_pExtraInfo->DestType = 0;
		m_pExtraInfo->lifeTime = 2.1f;	
	
		g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	}
	
	return TRUE;
}