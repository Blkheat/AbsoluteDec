#include "CGlow.h"
#include "..\\..\\CRenderManager.h"

CGlow::CGlow()
{
	m_Width  = 0;
	m_Height = 0;

	m_Color        = D3DCOLOR_ARGB( 0 , 255 , 255 , 255 );

	m_pBackBuffer  = NULL;
	m_pBackSurface = NULL;
	m_pBackTexture = NULL;

	for( int i = 0 ; i < GLOW_TEX_COUNT ; ++i )
	{
		m_pVB[ i ]              = NULL;
		m_pTex[ i ]             = NULL;
		m_pRenderToSurface[ i ] = NULL;
		m_pSurface[ i ]         = NULL;
	}

	m_pIB		   = NULL;
	m_pBackVB	   = NULL;

	m_GlowSizeTable[0] = 256;
	m_GlowSizeTable[1] = 128;
	m_GlowSizeTable[2] = 64;

	m_ISSetting = FALSE;

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

	 m_bEnable = true;

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)
}

CGlow::~CGlow()
{
	InvalidateDeviceObject();
}

HRESULT	CGlow::InvalidateDeviceObject()
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

	for( int i = 0 ; i < GLOW_TEX_COUNT ; ++i )
	{
		if( m_pVB[ i ] != NULL )
		{
			m_pVB[ i ]->Release();

			m_pVB[ i ] = NULL;
		}

		if( m_pTex[ i ] != NULL )
		{
			m_pTex[ i ]->Release();

			m_pTex[ i ] = NULL;
		}

		if( m_pRenderToSurface[ i ] != NULL )
		{
			m_pRenderToSurface[ i ]->Release();

			m_pRenderToSurface[ i ] = NULL;
		}

		if( m_pSurface[ i ] != NULL )
		{
			m_pSurface[ i ]->Release();

			m_pSurface[ i ] = NULL;
		}

	}
		
	m_ISSetting = FALSE;

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CGlow::RestoreDeviceObject(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT CGlow::RestoreDeviceObject(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	#ifdef HHM_USER_RESOLUTION_SELECT
		Init(m_Width, m_Height, pD3D, pDevice, m_bEnable);	
	#else 
		Init(m_Width, m_Height, pD3D, pDevice );	
	#endif 

	return S_OK;
}

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

#ifdef DIRECT_VERSION_9_MJH
int	CGlow::Init(UINT Width,UINT Height,LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pDevice, bool bEnable)
#else
int	CGlow::Init(UINT Width,UINT Height,LPDIRECT3D8 pD3D,LPDIRECT3DDEVICE8 pDevice, bool bEnable)
#endif // DIRECT_VERSION_9_MJH
{
	m_bEnable = bEnable;

	if(!m_bEnable)
		return FALSE;

#else

#ifdef DIRECT_VERSION_9_MJH
	int	CGlow::Init(UINT Width,UINT Height,LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pDevice)
#else
	int	CGlow::Init(UINT Width,UINT Height,LPDIRECT3D8 pD3D,LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

	D3DDISPLAYMODE	d3ddm;
	D3DSURFACE_DESC Desc;
	HRESULT			hr;

	m_Width  = Width;
	m_Height = Height;

	pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &d3ddm );
#ifdef DIRECT_VERSION_9_MJH
	if( D3D_OK != g_RenderManager.GetBackBuffer( 0,
#else
	if( D3D_OK != g_RenderManager.GetBackBuffer(
#endif // DIRECT_VERSION_9_MJH
		0,// �����ִ� �� ���� ��ü�� �ε���. �� ���۴� 0 ���κ��� �� ������ �Ѽ� - 1 ���� �ѹ��� �ȴ�.
		  // 0�� ��ȯ���� ������ �� ���۸� ��Ÿ��, ����Ʈ ���۴� �ƴϴ�. �� �޼��带 ����� ����Ʈ ���ۿ� �׼��� �� �� ����.
		  D3DBACKBUFFER_TYPE_MONO,// Microsoft�� DirectX�� 9.0 ������ ���׷��� ��� �������� �ʱ� ������,
								  // �� �Ķ������ ��ȿ�� ���� D3DBACKBUFFER_TYPE_MONO ���̴�. 
		&m_pBackBuffer ) )
	{
		m_ISSetting = FALSE;
		
		return FALSE;
	}

	m_pBackBuffer->GetDesc( &Desc );// ǥ�� ������ ��´�
	
	// ���� VGA(Radeon 9550 ��)�� 2^ ũ��� �����ؾ� ��. �ֽ����� �������.
	UINT uiWidth = 1024, uiHeight = 1024;
	
#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)
	
	if(m_Width > 1024 || m_Height > 1024)
		uiWidth = uiHeight = 1024 << 1;

#endif // #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)

	if( FAILED( hr = g_RenderManager.CreateTexture( uiWidth , uiHeight , 1 , D3DUSAGE_RENDERTARGET ,  Desc.Format ,
		D3DPOOL_DEFAULT , &m_pBackTexture ) ) )// �ؽ�ó ����
	{
		m_ISSetting = FALSE;

		return FALSE;
	}

	if( D3D_OK != m_pBackTexture->GetSurfaceLevel( 0 , &m_pBackSurface ) )// ������ �ؽ�ó ǥ���� ������ ���´�
	{
		m_ISSetting = FALSE;

		return FALSE;
	}
	
	float tv = ( float ) m_Height / uiHeight;
	float tu = ( float ) m_Width / uiWidth;
	
	// ��ҿ� �ؽ�ó ����
	for( int i = 0 ; i < GLOW_TEX_COUNT ; ++i )
	{
		if( FAILED( hr = g_RenderManager.CreateRenderToSurface( m_GlowSizeTable[ i ] , m_GlowSizeTable[ i ] , Desc.Format,
			FALSE, D3DFMT_UNKNOWN, &m_pRenderToSurface[ i ] ) ) )// ������ ǥ���� ���� �Ѵ�. 
		{
			m_ISSetting = FALSE;

			return FALSE;
		}

		if( FAILED( hr = g_RenderManager.CreateTexture( m_GlowSizeTable[ i ] , m_GlowSizeTable[ i ] , 1 ,
			D3DUSAGE_RENDERTARGET , Desc.Format , D3DPOOL_DEFAULT, &m_pTex[ i ] ) ) )
		{
			m_ISSetting = FALSE;

			return FALSE;
		}
		
		if( FAILED( hr = m_pTex[ i ]->GetSurfaceLevel( 0, &m_pSurface[ i ] ) ) )
		{
			m_ISSetting = FALSE;

			return FALSE;
		}


		m_VertexList[ i ][ 0 ].pos.x = ( float ) 0.0f - 0.5f;// rhw ���ؽ� ������ ����ϱ⿡ x, y��ǥ���� ���� 0.5 �� 
		m_VertexList[ i ][ 0 ].pos.y = ( float ) m_GlowSizeTable[ i ] - 0.5f;
		m_VertexList[ i ][ 0 ].tu0   = 0.0f;
		m_VertexList[ i ][ 0 ].tv0   = tv;
		
		m_VertexList[ i ][ 1 ].pos.x = ( float ) 0.0f - 0.5f;
		m_VertexList[ i ][ 1 ].pos.y = ( float ) 0.0f - 0.5f;
		m_VertexList[ i ][ 1 ].tu0   = 0.0f;
		m_VertexList[ i ][ 1 ].tv0   = 0.0f;

		m_VertexList[ i ][ 2 ].pos.x = ( float ) m_GlowSizeTable[ i ] - 0.5f;
		m_VertexList[ i ][ 2 ].pos.y = ( float ) m_GlowSizeTable[ i ] - 0.5f;
		m_VertexList[ i ][ 2 ].tu0   = tu;
		m_VertexList[ i ][ 2 ].tv0   = tv;
		
		m_VertexList[ i ][ 3 ].pos.x = ( float ) m_GlowSizeTable[ i ] - 0.5f;
		m_VertexList[ i ][ 3 ].pos.y = ( float ) 0.0f - 0.5f;
		m_VertexList[ i ][ 3 ].tu0   = tu;
		m_VertexList[ i ][ 3 ].tv0   = 0.0f;

		m_VertexList[ i ][ 0 ].color = m_VertexList[ i ][ 1 ].color = 
		m_VertexList[ i ][ 2 ].color = m_VertexList[ i ][ 3 ].color = D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
	
		//............................................................................................................
		// ���ؽ� ���� �����
		//............................................................................................................
		if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( VERTEX_GLOW_2D ) * 4 ) ,
			D3DUSAGE_WRITEONLY, D3DFVF_VERTEXT_GLOW_2D ,
			D3DPOOL_MANAGED, &m_pVB[ i ] ) ) )
		{
			m_ISSetting = FALSE;
	
			return FALSE;		
		}

		VERTEX_GLOW_2D   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
		if( FAILED ( m_pVB[ i ]->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
		if( FAILED ( m_pVB[ i ]->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		{
			m_ISSetting = FALSE;

			return FALSE;
		}

		memcpy( pVertice , m_VertexList[ i ] , ( sizeof( VERTEX_GLOW_2D ) * 4 ) );

		m_pVB[ i ]->Unlock();
	}
	
	{
		//............................................................................................................
		// ����� ���ؽ� ���� �����
		//............................................................................................................
		if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( VERTEX_GLOW_2D ) * 4 ) ,
			D3DUSAGE_WRITEONLY, D3DFVF_VERTEXT_GLOW_2D ,
			D3DPOOL_MANAGED, &m_pBackVB ) ) )
		{
			m_ISSetting = FALSE;
	
			return FALSE;		
		}
		
		VERTEX_GLOW_2D   *pVertice = NULL;
#ifdef DIRECT_VERSION_9_MJH
		if( FAILED ( m_pBackVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
		if( FAILED ( m_pBackVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH		
		{
			m_ISSetting = FALSE;
	
			return FALSE;
		}
		
		VERTEX_GLOW_2D vBack[4];
		
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
		
		vBack[ 0 ].color = vBack[ 1 ].color = vBack[ 2 ].color = vBack[ 3 ].color = m_Color;
		
		memcpy( pVertice , vBack , ( sizeof( VERTEX_GLOW_2D ) * 4 ) );
		
		m_pBackVB->Unlock();
	}
	
	//............................................................................................................
	// �ε��� ���� �����
	//............................................................................................................
	if( FAILED( 
		g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * 4 )   ,
		D3DUSAGE_WRITEONLY	     ,
		D3DFMT_INDEX16 		 ,
		D3DPOOL_MANAGED		 ,
		&m_pIB )) )
	{
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
		m_ISSetting = FALSE;

		return FALSE;
	}
	
	for(int i = 0 ; i < 4 ; i++ )
	{
		pIndexBuffer[ i ] = ( WORD ) i;
	}
	
	m_pIB->Unlock();

	m_ISSetting = TRUE;

	return TRUE;
}

//void write_JPEG_file( char* filename, int quality, int image_width, int image_height, BYTE* image_buffer );
// ����� ǥ���� ��ҿ� ǥ�鿡 ����. Rander() ���� ���� ȣ��
#ifdef DIRECT_VERSION_9_MJH
int CGlow::Process(LPDIRECT3DDEVICE9 pDevice,DWORD dwShader)
#else
int CGlow::Process(LPDIRECT3DDEVICE8 pDevice,DWORD dwShader)
#endif // DIRECT_VERSION_9_MJH
{
#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

	if(!m_bEnable)
		return FALSE;

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

	if( FALSE == m_ISSetting )
		return FALSE;

	HRESULT hr;
#ifdef DIRECT_VERSION_9_MJH
	RECT DestRect;
    // ��ü ȭ�� ����� StretchRect �Լ�Ư���� ��� ȭ�� ����  MJH 
	SetRect( &DestRect , 0 , 0 , m_Width , m_Height );  

	//DX8 CopyRects()  - >  StretchRect()  ����
	if( D3D_OK != ( hr = g_RenderManager.StretchRect(
		m_pBackBuffer,// ���� ��� ������
		NULL,// ��ü����� NULL
		m_pBackSurface,// ���� ���� ������
		&DestRect , D3DTEXF_NONE ) ) )// ��ü����� NULL
	{
		return FALSE;
		m_ISSetting = FALSE;
	}
#else
	// pSourceSurface�� pDestinationSurface�� ����. DX9�� UpdateSurface() ���
	if( D3D_OK != ( hr = g_RenderManager.CopyRects(
		m_pBackBuffer,// ���� ��� ������
		NULL,// ��ü����� NULL
		0,// ���� ����
		m_pBackSurface,// ���� ���� ������
		NULL) ) )// ��ü����� NULL
	{
		return FALSE;
		m_ISSetting = FALSE;
	}
#endif // DIRECT_VERSION_9_MJH	


// 	D3DLOCKED_RECT lockRect;    
//     if( SUCCEEDED( hr = m_pBackBuffer->LockRect( &lockRect, NULL, D3DLOCK_READONLY ) ) )
//     {
//         DWORD* pdwBase=(DWORD*)lockRect.pBits;
// 		UINT uPitch=lockRect.Pitch / sizeof(DWORD);
// 		
// 		UINT uWidth= 2048;
//         UINT uHeight= 2048;
// 		
// 		//BYTE* pSrc = (BYTE*)( pdwBase+rcWnd.top*uPitch+rcWnd.left );
// 		BYTE* pSrc = (BYTE*)( pdwBase+0*uPitch+0 );
// 		BYTE* pSrc24 = NULL;
// 		pSrc24 = SAFE_NEW_ARRAY( BYTE , ( uWidth * 3 ) * uHeight );		
// 		
// 		
// 		int idx = 0;
// 		
// 		for( int h = 0; h < uHeight; ++h )
// 		{
// 			for( int w = 0; w < uWidth; ++w )	/// BGRA => RGB�� ��ȯ 
// 			{
// 				pSrc24[3 * uWidth * h + 3 * w    ] = pSrc[lockRect.Pitch * h + 4 * w + 2];	/// R
// 				pSrc24[3 * uWidth * h + 3 * w + 1] = pSrc[lockRect.Pitch * h + 4 * w + 1];	/// G
// 				pSrc24[3 * uWidth * h + 3 * w + 2] = pSrc[lockRect.Pitch * h + 4 * w];		/// B
// 			}
// 		}		
// 		
// 		write_JPEG_file( "D:\SiGong\RockClient\Bin\Client\ScreenShot\BackBuffer", 90, uWidth, uHeight, pSrc24 );		
// 		
// 		SAFE_DELETE_ARRAY(pSrc24);
// 		m_pBackBuffer->UnlockRect(); 
// 	}      

	for( int i = 0 ; i < GLOW_TEX_COUNT ; ++i )
	{
		if( SUCCEEDED( ( hr = m_pRenderToSurface[ i ]->BeginScene( m_pSurface[ i ] , NULL ) ) ) )
		{
			g_RenderManager.SetTexture( 0 , m_pBackTexture );
			g_RenderManager.SetFVF( D3DFVF_VERTEXT_GLOW_2D );
			g_RenderManager.SetIndices( m_pIB , 0 );
			g_RenderManager.SetStreamSource( 0 , m_pVB[ i ] , sizeof( VERTEX_GLOW_2D ) );
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
#ifdef DIRECT_VERSION_9_MJH
			m_pRenderToSurface[ i ]->EndScene(D3DX_FILTER_LINEAR);
#else
			m_pRenderToSurface[ i ]->EndScene();
#endif // DIRECT_VERSION_9_MJH		
		}
	}

	return TRUE;
}

int CGlow::SetColor(D3DCOLOR Color)
{
#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

	if(!m_bEnable)
		return FALSE;

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

	if( FALSE == m_ISSetting )
		return FALSE;

	if( NULL == m_pBackVB )
	{
		return FALSE;
	}
	
	VERTEX_GLOW_2D   *pVertice = NULL;
	
	m_Color = Color;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pBackVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pBackVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		m_ISSetting = FALSE;
		
		return FALSE;
	}
	
	pVertice[ 0 ].color = pVertice[ 1 ].color = pVertice[ 2 ].color = pVertice[ 3 ].color = m_Color;
	
	m_pBackVB->Unlock();

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CGlow::Render(LPDIRECT3DDEVICE9 pDevice)
#else
int CGlow::Render(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

	if(!m_bEnable)
		return FALSE;

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

	if( m_ISSetting == FALSE )
		return FALSE;

	g_RenderManager.SetFillMode( D3DFILL_SOLID );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE  );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE);
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	g_RenderManager.SetFVF( D3DFVF_VERTEXT_GLOW_2D );

	g_RenderManager.SetIndices( m_pIB , 0 );
	g_RenderManager.SetStreamSource( 0 , m_pBackVB , sizeof( VERTEX_GLOW_2D ) );

	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend(  D3DBLEND_ONE );

	for( int i = 0 ; i < GLOW_TEX_COUNT ; ++i )
	{
		g_RenderManager.SetTexture( 0 , m_pTex[ i ] );
		g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
	}

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	return TRUE;
};

void CGlow::DrawTexture( float x, float y, float w, float h, 
					  float tx, float ty, float tw, float th,
#ifdef DIRECT_VERSION_9_MJH
					  D3DCOLOR color , LPDIRECT3DDEVICE9 pDevice )
#else
					  D3DCOLOR color , LPDIRECT3DDEVICE8 pDevice )
#endif // DIRECT_VERSION_9_MJH
{
	static VERTEX_GLOW_2D	v[4];
	
	v[0].color = v[1].color = v[2].color = v[3].color = color;
	
	v[0].pos.x = (float)x - 0.5f;
	v[0].pos.y = (float)y - 0.5f;
	v[1].pos.x = v[0].pos.x + w;
	v[1].pos.y = v[0].pos.y;
	v[2].pos.x = v[0].pos.x + w;
	v[2].pos.y = v[0].pos.y + h;
	v[3].pos.x = v[0].pos.x;
	v[3].pos.y = v[0].pos.y + h;

	v[0].tu0 = (float)tx;
	v[0].tv0 = (float)ty;
	v[1].tu0 = v[0].tu0 + tw;
	v[1].tv0 = v[0].tv0;
	v[2].tu0 = v[0].tu0 + tw;
	v[2].tv0 = v[0].tv0 + th;
	v[3].tu0 = v[0].tu0;
	v[3].tv0 = v[0].tv0 + th;

	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, v , sizeof( VERTEX_GLOW_2D ) );
}
