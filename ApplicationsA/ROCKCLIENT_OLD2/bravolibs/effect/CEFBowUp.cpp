#include "CEFBowUp.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

CEFBowUp::CEFBowUp()
{
	m_Level      = 0;
	m_WeaponType = 0;
}

CEFBowUp::~CEFBowUp()
{
}

void CEFBowUp::Init(void)
{
	// 인첸트 레벨 체크
	m_ParNum = -1;

	switch( m_Level )
	{
	case 0:	
		m_ParNum = 21021;
		break;
	case 1:	
		m_ParNum = 21022;
		break;
	case 2:	
		m_ParNum = 21023;
		break;
	case 3:	
		m_ParNum = 21024;
		break;
	case 4:	
		m_ParNum = 21025;
		break;
	default:	
		SetDestroy();
		return;
	}

	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( m_ParNum ) ) == NULL )
		return;

	for( int i = 0 ; i < 2 ; ++i )
	{

		m_ParticleLink[ i ].MakeLink( pData );

		ENCHANT_POINT TempPoint;
	
		if( i == 0 )
			TempPoint.Range = 0.0f;
		else
			TempPoint.Range = 1.0f;
		
		m_EnChantList.push_back( TempPoint );
	}


}

int CEFBowUp::Update(float dtime)
{

	return TRUE;
}

int	CEFBowUp::Render(float dtime)
{
	return TRUE;
}

int	CEFBowUp::SortRender(float dtime)
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

	
	D3DXMATRIX matPos;
	
	for( int i = 0 ; i < m_EnChantList.size() ; ++i )
	{
		D3DXVec3Lerp( &m_EnChantList[ i ].vPos , &m_pSrcChr->m_pUserInfo->vSwordAxis[ 1 ] , &m_pSrcChr->m_pUserInfo->vSwordAxis[ 0 ] , m_EnChantList[ i ].Range );
	}

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
	
	for(int i = 0 ; i < m_EnChantList.size() ; ++i )
	{
		m_ParticleLink[ i ].SetLinkPos( m_EnChantList[ i ].vPos );
	
		m_ParticleLink[ i ].UpdateLink( dtime );

		m_ParticleLink[ i ].Render();
		//g_Particle.DisplayUpdate( m_ParNum , m_EnChantList[ i ].vPos );
	}

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	g_RenderManager.SetLighting( FALSE );

	return TRUE;
}
