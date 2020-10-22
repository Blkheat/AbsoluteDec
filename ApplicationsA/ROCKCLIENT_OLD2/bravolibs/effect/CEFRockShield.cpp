#include "RockPCH.h"
#include "VFileSystem.h"

#include "CEFRockShield.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include <SOUND\Sound.H>
#include <NETWORK\\Protocol.h>
#include <Base\\D3DBase.h>
#include "..\\RockClient.h"
#include "..\\quadlist.h"
#include <Map\\Field.h>
#include "..\\Map.H"
#include "..\\PC.H"
#include "..\\Effect.H"
#include "..\\Camera.H"
#include "..\\RockClient.h"

#include "..\\..\\CRenderManager.h"

extern CBravoMap g_Map;
extern long		 g_nowTime;

#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	CEFRockShield::m_pTextureList[ 3 ]  = { NULL , };
#else
LPDIRECT3DTEXTURE8	CEFRockShield::m_pTextureList[ 3 ]  = { NULL , };
#endif // DIRECT_VERSION_9_MJH
Model			   *CEFRockShield::m_pModelList[ 3 ]    = { NULL , };

int CEFRockShield::InitDevice(void)
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	try
	{
		if( FAILED ( g_RenderManager.CreateTextureFromFile( "Effect\\Static\\RockShield.jpg"  , &m_pTextureList[ 0 ] ) ) )
			throw(0);

		if( FAILED ( g_RenderManager.CreateTextureFromFile( "Effect\\Static\\RockShield1.jpg" , &m_pTextureList[ 1 ] ) ) )
			throw(0);

		if( FAILED ( g_RenderManager.CreateTextureFromFile( "Effect\\Static\\RockShield2.jpg" , &m_pTextureList[ 2 ] ) ) )
			throw(0);

		m_pModelList[ 0 ] = SAFE_NEW( Model );
		m_pModelList[ 1 ] = SAFE_NEW( Model );
		m_pModelList[ 2 ] = SAFE_NEW( Model );
		
		if( NULL == m_pModelList[ 0 ] || NULL == m_pModelList[ 1 ] || NULL == m_pModelList[ 2 ] )
			throw(1);

		if( !m_pModelList[ 0 ]->Read( "Effect\\Static\\RockShield.msh"  ,  TRUE ) )
			throw(1);

		if( !m_pModelList[ 1 ]->Read( "Effect\\Static\\RockShield1.msh" ,  TRUE ) )
			throw(1);

		if( !m_pModelList[ 2 ]->Read( "Effect\\Static\\RockShield2.msh" ,  TRUE ) )
			throw(1);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


	try
	{
		VFileHandle* pFH = g_VFEffectStatic.OpenFile( "RockShield.jpg" );	
		if( pFH )
		{
			if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_pTextureList[ 0 ] ) ) )
			{
				g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
				throw(0);
			}			
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}		
		
		pFH = g_VFEffectStatic.OpenFile( "RockShield1.jpg" );	
		if( pFH )
		{
			if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_pTextureList[ 1 ] ) ) )
			{
				g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
				throw(0);
			}			
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}		

		pFH = g_VFEffectStatic.OpenFile( "RockShield2.jpg" );	
		if( pFH )
		{
			if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_pTextureList[ 2 ] ) ) )
			{
				g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
				throw(0);
			}			
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}		

		m_pModelList[ 0 ] = SAFE_NEW( Model );
		m_pModelList[ 1 ] = SAFE_NEW( Model );
		m_pModelList[ 2 ] = SAFE_NEW( Model );
		
		if( NULL == m_pModelList[ 0 ] || NULL == m_pModelList[ 1 ] || NULL == m_pModelList[ 2 ] )
			throw(1);

		if( !m_pModelList[ 0 ]->ReadVF( &g_VFEffectStatic, "RockShield.msh"  ,  TRUE ) )
			throw(1);

		if( !m_pModelList[ 1 ]->ReadVF( &g_VFEffectStatic, "RockShield1.msh" ,  TRUE ) )
			throw(1);

		if( !m_pModelList[ 2 ]->ReadVF( &g_VFEffectStatic, "RockShield2.msh" ,  TRUE ) )
			throw(1);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	catch(int Type)
	{
		switch( Type )
		{
			case 0:
///				MessageBox( NULL, "Error Texture Load", "Effect WoodenSword", MB_OK );
				break;
			case 1:
///				MessageBox( NULL , "Error Load Model" , "Effect WoodenSword" , MB_OK );
				break;
		}

		CEFRockShield::DeleteDevice();
		
		return FALSE;
	}
	
	return TRUE;
}

int	CEFRockShield::DeleteDevice(void)
{
	SAFE_RELEASE( m_pTextureList[ 0 ] );
	SAFE_RELEASE( m_pTextureList[ 1 ] );
	SAFE_RELEASE( m_pTextureList[ 2 ] );
	
	SAFE_DELETE( m_pModelList[ 0 ] );
	SAFE_DELETE( m_pModelList[ 1 ] );
	SAFE_DELETE( m_pModelList[ 2 ] );
	
	return TRUE;
}


CEFRockShield::CEFRockShield()
{
}

CEFRockShield::~CEFRockShield()
{
}

void CEFRockShield::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return;
	}
	m_IsRender = true;

	// 초기 값
	for( int i = 0 ; i < 3 ; i++ )
	{
		m_Angle[ i ] = ( rand()%360 );
	}

	m_fHeight[ 0 ] = 0.8f;
	m_fHeight[ 1 ] = 0.5f;
	m_fHeight[ 2 ] = 0.3f;

	m_AddRange[ 0 ] = 1;
	m_AddRange[ 1 ] = -1;
	m_AddRange[ 2 ] = 1;


	m_OldTime = g_nowTime;

	switch( m_ID )
	{
		case EF_ROCK_SHIELD0:
		case EF_ROCK_SHIELD1:
			
			m_Type = 1;

			break;

		case EF_ROCK_SHIELD2:
		case EF_ROCK_SHIELD3:

			m_Type = 2;

			break;

		case EF_ROCK_SHIELD4:

			m_Type = 3;

			break;
	}
	
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );


}


int CEFRockShield::Render(float dtime)
{
	D3DXMATRIX	matScale, matWorld;
	
	m_IsRender = true ; 

	D3DXMatrixIdentity( &matWorld );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );

	for( int i = 0 ; i < m_Type ; ++i )
	{
		g_RenderManager.SetTexture( 0, m_pTextureList[ i ] );			

		g_RenderManager.SetTransform( D3DTS_WORLD, &m_matWorld[ i ] );	
	
		g_RenderManager.SetIndices( m_pModelList[ i ]->m_pIB , 0 );
		g_RenderManager.SetStreamSource( 0 , m_pModelList[ i ]->m_pVB , sizeof(OBJECTVERTEX) );

		DrawIndexedPrimitive( 
			m_pModelList[ i ]->m_header.m_num_vertex , 
			m_pModelList[ i ]->m_header.m_num_face     );
	
	}
	
	g_RenderManager.SetTexture( 0, NULL );			
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetAlphaBlendEnable(  FALSE );

	return TRUE;
}

int CEFRockShield::CheckFrame(void)
{
	return TRUE;
}

int CEFRockShield::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy();

		return TRUE;
	}

	D3DXVECTOR3 vDir( 9.0f , 0.0f , 0.0f );
	D3DXMATRIX  matRotY;
	D3DXVECTOR3 vPos;

	m_AddRange[ 0 ] = 10;
	m_AddRange[ 1 ] = ( rand() % 10 ) + 2;
	m_AddRange[ 2 ] = ( rand() % 10 ) + 2;

	if( SAFE_TIME_SUB( g_nowTime , m_OldTime ) >= 50 )
	{
		m_OldTime = g_nowTime;
	
		for( int i = 0 ; i < m_Type ; ++i )
		{
			m_Angle[ i ] += m_AddRange[ i ];
			
			if( m_Angle[ i ] >= 360 )
				m_Angle[ i ] -= 360;
			
			if( m_Angle[ i ] <= -360 )
				m_Angle[ i ] += 360;
			
			D3DXMatrixIdentity( &m_matWorld[ i ] );
			D3DXMatrixIdentity( &matRotY         );
			
			
			D3DXMatrixRotationY( &matRotY , D3DXToRadian( m_Angle[ i ] ) );
			
			D3DXVec3TransformCoord( &vDir , &vDir , &matRotY );

			m_matWorld[ i ]._41 +=
			m_pSrcChr->GetBonCenterTM()._41 + vDir.x;
			
			m_matWorld[ i ]._42 =
			m_pSrcChr->GetBonCenterTM()._42 + m_pSrcChr->GetBoundMaxPos().y * m_fHeight[ i ];

			m_matWorld[ i ]._43 +=
			m_pSrcChr->GetBonCenterTM()._43 + vDir.z;


			vPos.x = m_matWorld[ i ]._41;
			vPos.y = m_matWorld[ i ]._42;
			vPos.z = m_matWorld[ i ]._43;
			if( m_IsRender ) 
			g_Particle.DisplayUpdate( PT_BOOK_STAND_ATTACK_TAIL , vPos );

		}

		m_IsRender = false;		

	}

	return TRUE;
}
