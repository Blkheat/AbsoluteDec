#include "RockPCH.h"
#include "VFileSystem.h"

#include	"..\\quadlist.h"
#include	<windows.h>
#include	<stdio.h>
#include	<base\\d3dapp.h>
#include	<base\\d3dfont.h>
#include	"skybox.h"
#include	"RockClient.h"
#include	<map\\d3dfvf.h>
#include	 "profile\\profile.h"
#include     "CRenderManager.h"
#include	"effect.h"

    
CSkyBox::CSkyBox()
{  
	m_IsSkyBoxDisplay = TRUE;
	
	D3DXMatrixIdentity( &m_matWorld );

	m_pVB = NULL;

	for( int i = 0 ; i < SKYBOX_DIR_MAX ; ++i )
		m_pTextureList[ i ] = NULL;

	m_fZSize = m_fYSize = m_fXSize = 1.0f;

}

CSkyBox::~CSkyBox()
{
}

HRESULT	CSkyBox::InvalidateDeviceObject()
{
	DeleteAllVertex();

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CSkyBox::RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT CSkyBox::RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	
	InitDevice(pDevice, m_fXSize, m_fYSize, m_fZSize);

	return S_OK;
}

void CSkyBox::SetPos(float x,float y,float z)
{
	m_matWorld._41 = x;
	m_matWorld._42 = y;
	m_matWorld._43 = z;
}

#ifdef DIRECT_VERSION_9_MJH
BOOL CSkyBox::InitDevice(LPDIRECT3DDEVICE9 pDevice,float fXSize,float fYSize,float fZSize)
#else
BOOL CSkyBox::InitDevice(LPDIRECT3DDEVICE8 pDevice,float fXSize,float fYSize,float fZSize)
#endif // DIRECT_VERSION_9_MJH
{
	DeleteAllVertex();
	
	m_fXSize = fXSize;
	m_fYSize = fYSize;
	m_fZSize = fZSize;
	
	// 버텍스

	float w = m_fXSize / 2.0f;
	float h = m_fYSize / 2.0f;
	float d = m_fZSize / 2.0f;
	
	float offset = 0.5f / 512.0f;

	SKYBOX_VERTEX	Vertices[ 24 ] =
	{
		// Front 
		{ w,-h, d, 1.0f - offset , 1.0f - offset },
		{-w,-h, d, 0.0f + offset , 1.0f - offset },
		{-w, h, d, 0.0f + offset , 0.0f + offset },
		{ w, h, d, 1.0f - offset , 0.0f + offset },
		// Right
		{ w,-h,-d, 1.0f - offset , 1.0f - offset },
		{ w,-h, d, 0.0f + offset , 1.0f - offset },
		{ w, h, d, 0.0f + offset , 0.0f + offset },
		{ w, h,-d, 1.0f - offset , 0.0f + offset },
		// Back
		{ w, h,-d, 0.0f + offset , 0.0f + offset },
		{-w, h,-d, 1.0f - offset , 0.0f + offset },
		{-w,-h,-d, 1.0f - offset , 1.0f - offset },
		{ w,-h,-d, 0.0f + offset , 1.0f - offset },
		// Left
		{-w, h,-d, 0.0f + offset , 0.0f + offset },
		{-w, h, d, 1.0f - offset , 0.0f + offset },
		{-w,-h, d, 1.0f - offset , 1.0f - offset },
		{-w,-h,-d, 0.0f + offset , 1.0f - offset },
		// Up
		{ w, h, d, 1.0f - offset , 1.0f - offset },
		{-w, h, d, 0.0f + offset , 1.0f - offset },
		{-w, h,-d, 0.0f + offset , 0.0f + offset },
		{ w, h,-d, 1.0f - offset , 0.0f + offset },
		// Bottom
		{ w,-h,-d, 1.0f - offset , 1.0f - offset },
		{-w,-h,-d, 0.0f + offset , 1.0f - offset },
		{-w,-h, d, 0.0f + offset , 0.0f + offset },
		{ w,-h, d, 1.0f - offset , 0.0f + offset }

	};
	
	if( FAILED( g_RenderManager.CreateVertexBuffer( 24 * sizeof( SKYBOX_VERTEX ) ,
			D3DUSAGE_WRITEONLY , D3DFVF_SKYBOX_VERTEX ,
			D3DPOOL_MANAGED, &m_pVB ) ) )
	{
		return FALSE;
	}

	SKYBOX_VERTEX *vtx;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB->Lock( 0, 0, (void**)&vtx, 0 ) ) )
#else
	if( FAILED( m_pVB->Lock( 0, 0, (BYTE**)&vtx, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;		
	}

	memcpy( vtx , Vertices , sizeof( SKYBOX_VERTEX ) * 24 );
	
	m_pVB->Unlock();

	return TRUE;
}

void CSkyBox::DeleteAllMemory(void)
{
	DeleteAllVertex();
	DeleteAllTexture();
}

void CSkyBox::DeleteAllVertex(void)
{
	if( NULL != m_pVB )
	{
		m_pVB->Release();

		m_pVB = NULL;
	}
}

void CSkyBox::DeleteAllTexture(void)
{
	for( int i = 0 ; i < SKYBOX_DIR_MAX ; ++i )
	{
		if( NULL != m_pTextureList[ i ] )
		{
			m_pTextureList[ i ]->Release();
			
			m_pTextureList[ i ] = NULL;
		}
	}
}

#ifdef DIRECT_VERSION_9_MJH
BOOL CSkyBox::LoadTexture(LPDIRECT3DDEVICE9 pDevice,int Index,int CycleType)
#else
BOOL CSkyBox::LoadTexture(LPDIRECT3DDEVICE8 pDevice,int Index,int CycleType)
#endif // DIRECT_VERSION_9_MJH
{
	//ProfileBegin( "< CSkyBox::LoadTexture >" );
	DeleteAllTexture();

	char TempStr[ 1024 ] = "";
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for( int i = 0 ; i < SKYBOX_DIR_MAX ; ++i )
	{
		wsprintf( TempStr , "Env\\Sky%d_%d_%d.jpg" , Index , CycleType , i );

		if ( FAILED ( g_RenderManager.CreateTextureFromFile( TempStr , &m_pTextureList[ i ] ) ) )
		{
///			MessageBox( NULL, "Error SkyBox Texture Load", "Notice", NULL );

			return FALSE;
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = NULL;

	for( int i = 0 ; i < SKYBOX_DIR_MAX ; ++i )
	{
		wsprintf( TempStr , "Sky%d_%d_%d.jpg" , Index , CycleType , i );

#ifdef DO_PROFILE
		TCHAR temp[300];
		wsprintf( temp, _T("Sky Texture Load %s") , TempStr);
#endif

		pFH = g_VFEnv.OpenFile( TempStr );	/// OpenFile(...)에서 이미 메모리에 올라와 있음 
		if( pFH )
		{
			//ProfileBegin( temp );
			HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_pTextureList[ i ] );
			//ProfileEnd( temp );
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}				
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ProfileEnd( "< CSkyBox::LoadTexture >" );
	return TRUE;
}


#ifdef DIRECT_VERSION_9_MJH
BOOL CSkyBox::Display(LPDIRECT3DDEVICE9 pDevice)
#else
BOOL CSkyBox::Display(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	g_RenderManager.SetTransform( D3DTS_WORLD , &m_matWorld );
	
	g_RenderManager.SetMaterial( &g_Pc_Manager.theSky_Material );	
	g_RenderManager.SetLight( 0, &g_Particle.m_WeatherDrop.m_Sky_Light );	
	
	g_RenderManager.LightEnable(0,TRUE);
	g_RenderManager.LightEnable(1,FALSE);

	g_RenderManager.SetLighting( TRUE );
	g_RenderManager.SetZEnable( FALSE );
	g_RenderManager.SetZWriteEnable( FALSE );
	
	g_RenderManager.SetTextureColorOP( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetVertexBuffer( m_pVB , sizeof( SKYBOX_VERTEX ) );
	g_RenderManager.SetFVF( D3DFVF_SKYBOX_VERTEX );
	
	for( int i = 0; i<6; ++i )
	{
		g_RenderManager.SetTexture( 0 , m_pTextureList[i] );

		g_RenderManager.DrawPrimitive( D3DPT_TRIANGLEFAN, i*4, 2);
	}
	
	g_RenderManager.DisableTexture( 0 );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.DisableVertexBuffer();

	return TRUE;
}
