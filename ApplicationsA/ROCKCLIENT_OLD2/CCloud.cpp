#include "RockPCH.h"
#include "VFileSystem.h"
#include "CCloud.h"
#include "CRenderManager.h"

CCloud::CCloud()
{
	
	m_pTextureList[ 0 ] =
	m_pTextureList[ 1 ] =
	m_pTextureList[ 2 ] = NULL;

	D3DXMatrixIdentity( &m_matWorld );

	m_IsCloudDisplay = TRUE;

	m_pVB			 = NULL;
	m_pIB			 = NULL;

	m_CloudMove0     = 0.0f;
	m_CloudMove1     = 0.0f;

	m_Loop			 = 4.0f;

	m_OldTimer0		 = timeGetTime();
	m_OldTimer1      = timeGetTime();
}

CCloud::~CCloud()
{
	DeleteAllMemory();
}

#ifdef DIRECT_VERSION_9_MJH
BOOL CCloud::LoadTexture(LPDIRECT3DDEVICE9 pDevice,int Index,int CycleType)
#else
BOOL CCloud::LoadTexture(LPDIRECT3DDEVICE8 pDevice,int Index,int CycleType)
#endif // DIRECT_VERSION_9_MJH
{
	DeleteAllTexture();

	char TempStr[ 1024 ] = "";
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for( int i = 0 ; i < 3 ; ++i )
	{
		if( 2 == i )
			wsprintf( TempStr , "Env\\mask.dds" );
		else 
			wsprintf( TempStr , "Env\\cloud%d_%d_%d.dds" , Index , CycleType , i );

		if ( FAILED ( g_RenderManager.CreateTextureFromFile( TempStr , &m_pTextureList[ i ] ) ) )
		{
///			MessageBox( NULL, "Error SkyBox Texture Load", "Notice", NULL );
			return FALSE;
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	for( int i = 0 ; i < 3 ; ++i )
	{
		if( 2 == i )
			wsprintf( TempStr , "mask.dds" );
		else 
			wsprintf( TempStr , "cloud%d_%d_%d.dds" , Index , CycleType , i );
		
		VFileHandle* pFH = g_VFEnv.OpenFile( TempStr );	/// OpenFile(...)에서 이미 메모리에 올라와 있음 
		if( pFH )
		{
			HRESULT hr = g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		D3DX_DEFAULT, D3DX_DEFAULT, 
																		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		0, NULL, NULL, &m_pTextureList[ i ] );
			
			g_VFEnv.CloseFile( pFH );	/// 파일 반드시 닫아야 한다. 
		}		
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}


#ifdef DIRECT_VERSION_9_MJH
BOOL CCloud::Display(LPDIRECT3DDEVICE9 pDevice)
#else
BOOL CCloud::Display(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	g_RenderManager.SetTransform( D3DTS_WORLD , &m_matWorld );

	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetIndexBuffer( m_pIB );
	g_RenderManager.SetFVF( D3DFVF_CLOUD_VERTEX );
	g_RenderManager.SetVertexBuffer( m_pVB , sizeof( CLOUD_VERTEX ) );
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_WRAP );
	
	g_RenderManager.SetTextureAddressU( 1 , D3DTADDRESS_MIRROR );
	g_RenderManager.SetTextureAddressV( 1 , D3DTADDRESS_MIRROR );
	g_RenderManager.SetTextureAddressW( 1 , D3DTADDRESS_MIRROR );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_OldTimer0 ) , >= , MAX_CLOUD0_TIMER ) ) 
	{
		m_OldTimer0 = timeGetTime();
	}

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_OldTimer1 ) , >= , MAX_CLOUD1_TIMER ) ) 
	{
		m_OldTimer1 = timeGetTime();
	}

	m_Move0 =  ( float ) SAFE_TIME_SUB( timeGetTime() , m_OldTimer0 ) / ( float ) MAX_CLOUD0_TIMER;
	m_Move1 =  ( float ) SAFE_TIME_SUB( timeGetTime() , m_OldTimer1 ) / ( float ) MAX_CLOUD1_TIMER;

	if( m_Move0 >= m_Loop )
		m_Move0 = 0.0f;
	
	if( m_Move1 >= m_Loop )
		m_Move1 = 0.0f;

	D3DXMATRIX matTrans;

	D3DXMatrixIdentity( &matTrans );

	matTrans._31 = m_Move0;

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT3 );

	g_RenderManager.SetTransform( D3DTS_TEXTURE0 , &matTrans );

	g_RenderManager.SetTextureColorOP( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureAlphaOP( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureColorOP( 1 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureAlphaOP( 1 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 1 , D3DTA_TEXTURE );

	// Mask
	g_RenderManager.SetTexture( 1 , m_pTextureList[ 2 ] );
	g_RenderManager.SetTexture( 0 , m_pTextureList[ 0 ] );

	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
	
	matTrans._31 = m_Move1;

	g_RenderManager.SetTransform( D3DTS_TEXTURE0 , &matTrans );

	g_RenderManager.SetTexture( 0 , m_pTextureList[ 1 ] );

    g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );


	g_RenderManager.SetZEnable( TRUE ); 
	g_RenderManager.SetZWriteEnable( TRUE ); 
	g_RenderManager.SetLighting( FALSE ); 

	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );

	D3DXMatrixIdentity( &matTrans );
	g_RenderManager.SetTransform( D3DTS_TEXTURE0 , &matTrans );
	
	g_RenderManager.DisableTexture( 0 );
	g_RenderManager.DisableTexture( 1 );

	g_RenderManager.DisableVertexBuffer();
	g_RenderManager.DisableTextureStageState( 1 );

	return TRUE;
}
#ifdef DIRECT_VERSION_9_MJH
BOOL CCloud::InitDevice(LPDIRECT3DDEVICE9 pDevice,float Loop)
#else
BOOL CCloud::InitDevice(LPDIRECT3DDEVICE8 pDevice,float Loop)
#endif // DIRECT_VERSION_9_MJH
{
	DeleteAllVertex();

	if( FAILED( g_RenderManager.CreateVertexBuffer( 4 * sizeof( CLOUD_VERTEX ) ,
			D3DUSAGE_WRITEONLY , D3DFVF_CLOUD_VERTEX ,
			D3DPOOL_MANAGED, &m_pVB ) ) )
	{
		return FALSE;
	}
		
	m_Loop = Loop;
	
	CLOUD_VERTEX *vtx;
	
	float fSize	= 1.0f;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB->Lock( 0, 0, (void**)&vtx, 0 ) ) )
#else
	if( FAILED( m_pVB->Lock( 0, 0, (BYTE**)&vtx, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;		
	}
	
	float YPos = 0.0f;

	float offset = 0.5f / 512.0f;

	vtx[0].vPos = D3DXVECTOR3( -fSize , YPos , -fSize );
	vtx[0].tu   = 0.0f + offset;
	vtx[0].tv   = Loop - ( Loop * offset );
	vtx[0].tu1  = 0.0f;
	vtx[0].tv1  = 1.0f - offset;

	vtx[1].vPos = D3DXVECTOR3( -fSize ,  YPos , fSize );
	vtx[1].tu   = 0.0f + offset;
	vtx[1].tv   = 0.0f + offset;
	vtx[1].tu1  = 0.0f + offset;
	vtx[1].tv1  = 0.0f + offset;

	vtx[2].vPos = D3DXVECTOR3(  fSize , YPos , -fSize );
	vtx[2].tu   = Loop - ( Loop * offset );
	vtx[2].tv   = Loop - ( Loop * offset );
	vtx[2].tu1  = 1.0f - offset;
	vtx[2].tv1  = 1.0f - offset;

	vtx[3].vPos = D3DXVECTOR3(  fSize ,  YPos , fSize );
	vtx[3].tu   = Loop - ( Loop * offset );
	vtx[3].tv   = 0.0f + offset;
	vtx[3].tu1  = 1.0f - offset;
	vtx[3].tv1  = 0.0f + offset;


	m_pVB->Unlock();
		
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * 4 )   ,
		D3DUSAGE_WRITEONLY	 ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_DEFAULT		 ,
		&m_pIB )) )
	{
		return FALSE;		
	}
		
	WORD *pIB = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pIB->Lock( 0, 0, (void**)&pIB, 0 ) ) )
#else
	if( FAILED( m_pIB->Lock( 0, 0, (BYTE**)&pIB, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;		
	}
		
	for( int i = 0 ; i < 4 ; ++i )
	{
		*pIB = ( WORD ) i;
	
		 pIB++;	
	}

	m_pIB->Unlock();

	return TRUE;
}

HRESULT	CCloud::InvalidateDeviceObject()
{
    DeleteAllVertex();

    return S_OK;
}
#ifdef DIRECT_VERSION_9_MJH
HRESULT CCloud::RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT CCloud::RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	InitDevice(pDevice, 4.0f);
	
	return S_OK;
}

void CCloud::DeleteAllMemory(void)
{
	DeleteAllVertex();
	DeleteAllTexture();
}

void CCloud::DeleteAllVertex(void)
{
	if( NULL != m_pIB )
	{
		m_pIB->Release();
		m_pIB = NULL;
	}

	if( NULL != m_pVB )
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

void CCloud::DeleteAllTexture(void)
{
	for( int i = 0 ; i < 3 ; ++i )
	{
		if( NULL != m_pTextureList[ i ] )
		{
			m_pTextureList[ i ]->Release();
			
			m_pTextureList[ i ] = NULL;
		}
	}
}
