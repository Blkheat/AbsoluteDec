#include "RockPCH.h"
#include "CScreenBlur.h"
#include "..\\..\\CRenderManager.h"

CScreenBlur::CScreenBlur()
{
	m_Width  = 0;
	m_Height = 0;

	m_pBackBuffer  = NULL;
	m_pBackSurface = NULL;
	m_pBackTexture = NULL;

	m_pVB          = NULL;
	m_pIB		   = NULL;
	m_pBackVB	   = NULL;

	m_ISSetting	   = FALSE;

	m_SaveCount    = 0;
}

CScreenBlur::~CScreenBlur()
{
	InvalidateDeviceObject();
}

HRESULT	CScreenBlur::InvalidateDeviceObject()
{
	if( m_pIB != NULL )
	{
		m_pIB->Release();
		
		m_pIB = NULL;
	}

	if( m_pBackVB != NULL )
	{
		m_pBackVB->Release();
		
		m_pBackVB = NULL;
	}
	
	if( m_pBackBuffer )
	{
		m_pBackBuffer->Release();
		
		m_pBackBuffer = NULL;
	}
	
	if( m_pBackSurface )
	{
		m_pBackSurface->Release();
		
		m_pBackSurface = NULL;
	}
	
	if( m_pBackTexture )
	{
		m_pBackTexture->Release();
		
		m_pBackTexture = NULL;
	}

	std::list<LPBLUR_SURFACE >::iterator i = m_BlurSurfaceList.begin();
	
	for( ; i != m_BlurSurfaceList.end() ; ++i )
	{
		LPBLUR_SURFACE pData = (*i);
		SAFE_DELETE( pData );
		(*i) = NULL;
	}
	
	m_ISSetting = FALSE;

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CScreenBlur::RestoreDeviceObject(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT CScreenBlur::RestoreDeviceObject(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	Init(m_Width, m_Height, pD3D, pDevice);
	
	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
int	CScreenBlur::Init(UINT Width,UINT Height,LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pDevice)
#else
int	CScreenBlur::Init(UINT Width,UINT Height,LPDIRECT3D8 pD3D,LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	D3DDISPLAYMODE	d3ddm;
	D3DSURFACE_DESC Desc;
	HRESULT			hr;

	m_Width  = Width;
	m_Height = Height;

	pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &d3ddm );
#ifdef DIRECT_VERSION_9_MJH
	if( D3D_OK != g_RenderManager.GetBackBuffer( 0 , 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer ) )
#else
	if( D3D_OK != g_RenderManager.GetBackBuffer( 0 , D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer ) )
#endif // DIRECT_VERSION_9_MJH
	{
		InvalidateDeviceObject();
		
		m_ISSetting = FALSE;
		
		return FALSE;
	}

	m_pBackBuffer->GetDesc( &Desc );

	UINT uiHeight = 1024;
	
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)
	
	uiHeight = m_Height;
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)

	if( FAILED( hr = g_RenderManager.CreateTexture( m_Width , uiHeight , 1 , D3DUSAGE_RENDERTARGET ,  Desc.Format , D3DPOOL_DEFAULT , &m_pBackTexture ) ) )
	{
		m_ISSetting = FALSE;

		return FALSE;
	}

	if( D3D_OK != m_pBackTexture->GetSurfaceLevel( 0 , &m_pBackSurface ) )
	{
		m_ISSetting = FALSE;

		return FALSE;
	}
	
	float tv = ( float ) m_Height / ( float ) m_Width;
	UINT  SurfaceSize = 256.0f;
	int   Count		  = 8;

	for( int i = 0 ; i < Count ; ++i )
	{
		LPBLUR_SURFACE pBlurSurface = NULL;

		pBlurSurface = SAFE_NEW( BLUR_SURFACE );	

		if( NULL == pBlurSurface)
		{
			InvalidateDeviceObject();
			
			m_ISSetting = FALSE;

			return FALSE;
		}

		pBlurSurface->pRenderSurface = SAFE_NEW( CRenderSurface );		

		if( NULL == pBlurSurface->pRenderSurface )
		{
			InvalidateDeviceObject();
			
			m_ISSetting = FALSE;

			return FALSE;
		}
		
		if( !pBlurSurface->pRenderSurface->Init( SurfaceSize , SurfaceSize , pD3D , pDevice ) )
		{
			InvalidateDeviceObject();
			
			m_ISSetting = FALSE;

			return FALSE;
		}

		m_BlurSurfaceList.push_back( pBlurSurface );
	}

	{
		VERTEX_BLUR_2D	Vertices[ 4 ];
		
		Vertices[ 0 ].pos.x = ( float ) 0.0f - 0.5f;
		Vertices[ 0 ].pos.y = ( float ) SurfaceSize - 0.5f;
		Vertices[ 0 ].tu0   = 0.0f;
		Vertices[ 0 ].tv0   = tv;
		
		Vertices[ 1 ].pos.x = ( float ) 0.0f - 0.5f;
		Vertices[ 1 ].pos.y = ( float ) 0.0f - 0.5f;
		Vertices[ 1 ].tu0   = 0.0f;
		Vertices[ 1 ].tv0   = 0.0f;
		
		Vertices[ 2 ].pos.x = ( float ) SurfaceSize - 0.5f;
		Vertices[ 2 ].pos.y = ( float ) SurfaceSize - 0.5f;
		Vertices[ 2 ].tu0   = 1.0f;
		Vertices[ 2 ].tv0   = tv;
		
		Vertices[ 3 ].pos.x = ( float ) SurfaceSize - 0.5f;
		Vertices[ 3 ].pos.y = ( float ) 0.0f - 0.5f;
		Vertices[ 3 ].tu0   = 1.0f;
		Vertices[ 3 ].tv0   = 0.0f;
		
		Vertices[ 0 ].color = Vertices[ 1 ].color = 
		Vertices[ 2 ].color = Vertices[ 3 ].color = D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
		
		//............................................................................................................
		// 버텍스 버퍼 만들기
		//............................................................................................................
		if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( VERTEX_BLUR_2D ) * 4 ) ,
			D3DUSAGE_WRITEONLY, VERTEX_BLUR_2D::FVF ,
			D3DPOOL_MANAGED, &m_pVB ) ) )
		{
			InvalidateDeviceObject();
			
			m_ISSetting = FALSE;
			
			return FALSE;
		}
		
		VERTEX_BLUR_2D   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
		if( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
		if( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH		
		{
			InvalidateDeviceObject();
			
			m_ISSetting = FALSE;
			
			return FALSE;
		}
		
		memcpy( pVertice , Vertices , ( sizeof( VERTEX_BLUR_2D ) * 4 ) );
		
		m_pVB->Unlock();
	}
	
	{
		//............................................................................................................
		// 버텍스 버퍼 만들기
		//............................................................................................................
		if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( VERTEX_BLUR_2D ) * 4 ) ,
			D3DUSAGE_WRITEONLY, VERTEX_BLUR_2D::FVF ,
			D3DPOOL_MANAGED, &m_pBackVB ) ) )
		{
			InvalidateDeviceObject();
			
			m_ISSetting = FALSE;
			
			return FALSE;
		}
		
		VERTEX_BLUR_2D   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
		if( FAILED ( m_pBackVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
		if( FAILED ( m_pBackVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH		
		{
			InvalidateDeviceObject();
			
			m_ISSetting = FALSE;
			
			return FALSE;
		}
		
		VERTEX_BLUR_2D vBack[4];
		
		vBack[ 0 ].pos.x = ( float ) 0.0f - 0.5f;
		vBack[ 0 ].pos.y = ( float ) m_Height - 0.5f;
		vBack[ 0 ].tu0   = 0.0f;
		vBack[ 0 ].tv0   = 1.0f;
		
		vBack[ 1 ].pos.x = ( float ) 0.0f - 0.5f;
		vBack[ 1 ].pos.y = ( float ) 0.0f - 0.5f;
		vBack[ 1 ].tu0   = 0.0f;
		vBack[ 1 ].tv0   = 0.0f;
		
		vBack[ 2 ].pos.x = ( float ) m_Width - 0.5f;
		vBack[ 2 ].pos.y = ( float ) m_Height - 0.5f;
		vBack[ 2 ].tu0   = 1.0f;
		vBack[ 2 ].tv0   = 1.0f;
		
		vBack[ 3 ].pos.x = ( float ) m_Width - 0.5f;
		vBack[ 3 ].pos.y = ( float ) 0.0f - 0.5f;
		vBack[ 3 ].tu0   = 1.0f;
		vBack[ 3 ].tv0   = 0.0f;
		

		float Alpha = ( float ) ( m_BlurSurfaceList.size() + 1 ) / 255.0f;
		vBack[ 0 ].color = vBack[ 1 ].color = vBack[ 2 ].color = vBack[ 3 ].color = 
			D3DCOLOR_ARGB( 20 , 255 , 255 , 255 );
			//D3DCOLOR_COLORVALUE( 1.0f , 1.0f , 1.0f , 0.6f );
		
		memcpy( pVertice , vBack , ( sizeof( VERTEX_BLUR_2D ) * 4 ) );
		
		m_pBackVB->Unlock();
	}
	
	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * 4 )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB )) )
	{
		InvalidateDeviceObject();
		
		m_ISSetting = FALSE;
		
		return FALSE;
	}

	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH	
	{
		InvalidateDeviceObject();
		
		m_ISSetting = FALSE;
		
		return FALSE;
	}
	
	for(int i = 0 ; i < 4 ; i++ )
	{
		pIndexBuffer[ i ] = ( WORD ) i;
	}
	
	m_pIB->Unlock(); 

	m_ISSetting  = TRUE;

	m_SaveCount  = 0;
	m_ISRenderOK = FALSE;

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CScreenBlur::Process(LPDIRECT3DDEVICE9 pDevice,DWORD dwShader)
#else
int CScreenBlur::Process(LPDIRECT3DDEVICE8 pDevice,DWORD dwShader)
#endif // DIRECT_VERSION_9_MJH
{
	if( FALSE == m_ISSetting )
		return FALSE;	

	/*static DWORD OldTimer = timeGetTime();

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , OldTimer ) , > , 10 ) )
	{
		OldTimer = timeGetTime();
	}
	else
	{
		return TRUE;
	}*/

	HRESULT hr;
#ifdef DIRECT_VERSION_9_MJH
	if( D3D_OK != ( hr = g_RenderManager.StretchRect( m_pBackBuffer, NULL  , m_pBackSurface, NULL , D3DTEXF_NONE ) ) )
#else
	if( D3D_OK != ( hr = g_RenderManager.CopyRects( m_pBackBuffer, NULL , 0 , m_pBackSurface, NULL) ) )
#endif // DIRECT_VERSION_9_MJH	
	
	{
		return FALSE;
		m_ISSetting = FALSE;
	}

	std::list<LPBLUR_SURFACE>::iterator i = m_BlurSurfaceList.begin();
	///--int Count = 0;

	if( i != m_BlurSurfaceList.end() )
	{
		if( SUCCEEDED( (*i)->pRenderSurface->BeginScene() ) )
		{
			g_RenderManager.SetTexture( 0 , m_pBackTexture );
			g_RenderManager.SetFVF( VERTEX_BLUR_2D::FVF );
			g_RenderManager.SetIndices( m_pIB , 0 );
			g_RenderManager.SetStreamSource( 0 , m_pVB , sizeof( VERTEX_BLUR_2D ) );
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
		
			(*i)->pRenderSurface->EndScene();
		}
		else
		{
			return FALSE;
		}
	}

	m_ISRenderOK = TRUE;

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CScreenBlur::Render(LPDIRECT3DDEVICE9 pDevice)
#else
int CScreenBlur::Render(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if( !m_ISSetting && !m_ISRenderOK )
		return FALSE;
	
	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE  );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	g_RenderManager.SetFVF( VERTEX_BLUR_2D::FVF );

	g_RenderManager.SetIndices( m_pIB , 0 );
	g_RenderManager.SetStreamSource( 0 , m_pBackVB , sizeof( VERTEX_BLUR_2D ) );

	g_RenderManager.SetAlphaBlendEnable( TRUE );	

	std::list<LPBLUR_SURFACE>::iterator i = m_BlurSurfaceList.begin();
	
	int j = 0;

	if( i != m_BlurSurfaceList.end() )
	{
		for( j = 0 ; j < 7 ; j++ ) 
		{
			VERTEX_BLUR_2D   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
			if( FAILED ( m_pBackVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
			if( FAILED ( m_pBackVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH		
			{
				InvalidateDeviceObject();				
				m_ISSetting = FALSE;				
				return FALSE;
			}

			DWORD dSrcAlpah = D3DBLEND_SRCALPHA;
			DWORD dDestAlpah = D3DBLEND_INVSRCALPHA;

			if( j < 2 )
			{
				dDestAlpah = D3DBLEND_ONE;
			}
			
			g_RenderManager.SetSrcBlend( dSrcAlpah );
			g_RenderManager.SetDestBlend( dDestAlpah );	
		
			VERTEX_BLUR_2D vBack[4];

			int nHeight = m_Height;
			int nWithd = m_Width;

			float fAddHeight = (float)nHeight * ( (float)( 6 - j ) / 6.0f );
			float fAddWidth = (float)nWithd * ( (float)( 6 - j ) / 6.0f );			
			
			vBack[ 0 ].pos.x = ( float ) ( 0.0f - fAddWidth ) - 0.5f;
			vBack[ 0 ].pos.y = ( float ) ( nHeight + fAddHeight ) - 0.5f;
			vBack[ 0 ].tu0   = 0.0f;
			vBack[ 0 ].tv0   = 1.0f;
			
			vBack[ 1 ].pos.x = ( float ) ( 0.0f - fAddWidth ) - 0.5f;
			vBack[ 1 ].pos.y = ( float ) ( 0.0f - fAddHeight ) - 0.5f;
			vBack[ 1 ].tu0   = 0.0f;
			vBack[ 1 ].tv0   = 0.0f;
			
			vBack[ 2 ].pos.x = ( float ) ( nWithd + fAddWidth ) - 0.5f;
			vBack[ 2 ].pos.y = ( float ) ( nHeight + fAddHeight ) - 0.5f;
			vBack[ 2 ].tu0   = 1.0f;
			vBack[ 2 ].tv0   = 1.0f;
			
			vBack[ 3 ].pos.x = ( float ) ( nWithd + fAddWidth ) - 0.5f;
			vBack[ 3 ].pos.y = ( float ) ( 0.0f - fAddHeight ) - 0.5f;
			vBack[ 3 ].tu0   = 1.0f;
			vBack[ 3 ].tv0   = 0.0f;
			
			vBack[ 0 ].color = vBack[ 1 ].color = vBack[ 2 ].color = vBack[ 3 ].color = 
				D3DCOLOR_ARGB( 40 , 255 , 255 , 255 );
			
			memcpy( pVertice , vBack , ( sizeof( VERTEX_BLUR_2D ) * 4 ) );
			
			m_pBackVB->Unlock();			
			
			g_RenderManager.SetTexture( 0 , (*i)->pRenderSurface->GetTexture() );
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
		}
	}

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
};