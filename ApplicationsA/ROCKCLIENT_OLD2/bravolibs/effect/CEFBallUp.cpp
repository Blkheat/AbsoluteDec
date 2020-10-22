#include "CEFBallUp.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

CEFBallUp::CEFBallUp()
{
	m_Level		 = 0;
	m_WeaponType = 0;
}

CEFBallUp::~CEFBallUp()
{
}

void CEFBallUp::Init(void)
{
	// 인첸트 레벨 체크
	int ParNum = -1;

	switch( m_Level )
	{
	case 0:	
		ParNum = 21041;
		break;
	case 1:	
		ParNum = 21042;
		break;
	case 2:	
		ParNum = 21043;
		break;
	case 3:	
		ParNum = 21044;
		break;
	case 4:	
		ParNum = 21045;
		break;
	default:	
		SetDestroy();
		return;
	}

	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( ParNum ) ) == NULL )
		return;

	ENCHANT_POINT TempPoint;
	
	TempPoint.Range = 0.5f;
		
	m_EnChantList.push_back( TempPoint );

	m_ParticleLink.MakeLink( pData );
}

int CEFBallUp::Update(float dtime)
{
	return TRUE;
}

int	CEFBallUp::Render(float dtime)
{
	return TRUE;
}

int	CEFBallUp::SortRender(float dtime)
{
	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		SetDestroy();
		return TRUE;
	}
	
	if( g_Particle.m_EffectManager.ISOverLength( m_pSrcChr ) )
	{
		return TRUE;
	}

	if( !m_pSrcChr->DrawWeaponCheckStatus() )
		return TRUE;

	if( !m_pSrcChr->m_IsAllLTexture )
		return TRUE;

	
	D3DXVec3Lerp( &m_EnChantList[ 0 ].vPos , &m_pSrcChr->m_pUserInfo->vSwordAxis[ 1 ] , &m_pSrcChr->m_pUserInfo->vSwordAxis[ 0 ] , m_EnChantList[ 0 ].Range );

	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetLighting( TRUE );
	
	m_ParticleLink.SetLinkPos( m_EnChantList[ 0 ].vPos );
	
	m_ParticleLink.UpdateLink( dtime );

	m_ParticleLink.Render();

	g_RenderManager.SetCullMode( D3DCULL_CCW );

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	g_RenderManager.SetLighting( FALSE );

	return TRUE;
}
