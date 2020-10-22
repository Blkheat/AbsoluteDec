#include "CEFDetect.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "CEFArrow.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFDetect::CEFDetect()
{
}

CEFDetect::~CEFDetect()
{
}

void CEFDetect::Init(void)
{
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	
	g_Particle.SetEffSetCon( 0.0f , m_ID , 0 , 2.1f , m_pSrcChr->GetUnique() , m_pSrcChr->GetIndex() );

	CBillboardData *pData = NULL ;
	
	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 22728 ) ) )
	{
		return;
	}

	m_Bill0.MakeLink( pData );

	if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 22729 ) ) )
	{
		return;
	}

	m_Bill1.MakeLink( pData );

	// 스킬 범위
	if( m_pExtraInfo != NULL )
		m_AttackRange = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );
	else
		m_AttackRange = 8.0f * 5;

	m_Range.SetMaxTime( 3000 );

	m_Range.Insert( 0 , 0.0f );
	m_Range.Insert( 3000 , D3DXToRadian( 360 * 1 ) );

	m_OldTime = g_nowTime;
	
}

int CEFDetect::Update(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}
	
	
	float Value;
	
	if( m_Range.GetValue( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , Value ) == FALSE )
	{
		SetDestroy();

		return TRUE;
	}
	
	m_vPos = D3DXVECTOR3( m_AttackRange , 0.0f , 0.0f );

	D3DXMATRIX matRotY;

	D3DXMatrixRotationY( &matRotY , Value );

	D3DXVec3TransformCoord( &m_vPos , &m_vPos , &matRotY );

	m_vPos.x += m_pSrcChr->GetPosTM()._41;
	m_vPos.z += m_pSrcChr->GetPosTM()._43;
	m_vPos.y  = g_Map.Get__MapHeightF( m_vPos.x , m_vPos.z ) + ( m_pSrcChr->GetBoundMaxVec().y - m_pSrcChr->GetBoundMinVec().y ) * 1.3;

	m_Bill0.m_vLoc = m_Bill1.m_vLoc = m_vPos;

	g_Billboard.DisplayManager( m_Bill0 , dtime );
	g_Billboard.DisplayManager( m_Bill1 , dtime );

	return TRUE;
}

int	CEFDetect::Render(float dtime)
{
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );

#ifdef BATCH_RENDER
	m_Bill1.DrawBatchBillboard();
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
	m_Bill0.DrawBatchBillboard();
	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
	if(prevBillboardTexture)
	{
		g_RenderManager.SetTexture( 0, prevBillboardTexture );
 		g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
 		g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
		g_RenderManager.EndBatchPrimitive();
		g_RenderManager.SetBeginBatchRendering(FALSE);
	}
#else	//	BATCH_RENDER
	m_Bill1.DrawBillboard();
	m_Bill0.DrawBillboard();
#endif	//	BATCH_RENDER
	
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
}
