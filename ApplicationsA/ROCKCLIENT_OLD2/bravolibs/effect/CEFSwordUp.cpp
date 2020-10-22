#include "CEFSwordUp.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

CEFSwordUp::CEFSwordUp()
{
	m_Level      = 0;
	m_WeaponType = 0;
	m_Count = 0 ;

}

CEFSwordUp::~CEFSwordUp()
{
}


void CEFSwordUp::Init(void)
{
	// 인첸트 레벨 체크
	int ParNum = -1;

	// 대검
/*
	if( m_WeaponType == 0 )
	{
		switch( m_Level )
		{
			case 0:	
				ParNum = 21061;
				break;
			case 1:	
				ParNum = 21062;
				break;
			case 2:	
				ParNum = 21063;
				break;
			case 3:	
				ParNum = 21064;
				break;
			case 4:	
				ParNum = 21065;
				break;
			default:	
				SetDestroy();
				return;
		}
	}
	// 한손검	
	else
	*/

	{
		switch( m_Level )
		{
			case 0:	
				ParNum = 21011;
				break;
			case 1:	
				ParNum = 21012;
				break;
			case 2:	
				ParNum = 21013;
				break;
			case 3:	
				ParNum = 21014;
				break;
			case 4:	
				ParNum = 21015;
//				ParNum = 21743;
				break;
			default:	
				SetDestroy();
				return;
		}
	}

	CParticleData	*pData = NULL;

	if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( ParNum ) ) == NULL )
		return;

#ifdef EFFECT_SWORD_UP_EXP
	//칼의 처음과 끝의 길이를 잰다 .. 
	//얼마만큼의 간격을 두고 그린다.  
	// 카운트를 샌다 .. 	

	if( m_WeaponType == 0 )
		m_Count = 7;
	else 
		m_Count = 5; 

	for( int i = 0 ; i < m_Count ; ++i )
	{

		m_ParticleLink[ i ].MakeLink( pData );

		ENCHANT_POINT TempPoint;
	
		if( i == 0 )
			TempPoint.Range = 0.0f;
		else
			TempPoint.Range = 1.0f * ( ( float ) i / ( float ) (m_Count-1) );
		
		m_EnChantList.push_back( TempPoint );
	}



#else 
	for( int i = 0 ; i < 5 ; ++i )
	{

		m_ParticleLink[ i ].MakeLink( pData );

		ENCHANT_POINT TempPoint;
	
		if( i == 0 )
			TempPoint.Range = 0.0f;
		else
			TempPoint.Range = 1.0f * ( ( float ) i / ( float ) 4 );
		
		m_EnChantList.push_back( TempPoint );
	}

#endif

}

int CEFSwordUp::Update(float dtime)
{
	return TRUE;
}

int	CEFSwordUp::SortRender(float dtime)
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
	
	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetLighting( TRUE );
	
	for(int i = 0 ; i < m_EnChantList.size() ; ++i )
	{
		m_ParticleLink[ i ].SetLinkPos( m_EnChantList[ i ].vPos );
	
		m_ParticleLink[ i ].UpdateLink( dtime );

		m_ParticleLink[ i ].Render();
	}

	g_RenderManager.SetLighting( FALSE );

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable(  FALSE );

	return TRUE;
}


int	CEFSwordUp::Render(float dtime)
{
	return TRUE;
}
