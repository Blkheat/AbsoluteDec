#include "CRenderSurface.h"
#include "..\\..\\CRenderManager.h"

CRenderSurface::CRenderSurface()
{
	m_pTexture			= NULL;
	m_pRenderToSurface  = NULL;
	m_pSurface			= NULL;
	m_ISInit			= FALSE;
	m_Width				= 0;
	m_Height			= 0;
}

CRenderSurface::~CRenderSurface()
{
	DeleteMemory();
}

void CRenderSurface::DeleteMemory(void)
{
	if( NULL != m_pTexture )
	{
		m_pTexture->Release();

		m_pTexture = NULL;
	}

	if( NULL != m_pRenderToSurface )
	{
		m_pRenderToSurface->Release();

		m_pRenderToSurface = NULL;
	}

	if( NULL != m_pSurface )
	{
		m_pSurface->Release();

		m_pSurface = NULL;
	}

	m_ISInit			= FALSE;
}

#ifdef DIRECT_VERSION_9_MJH
int CRenderSurface::Init(UINT Width,UINT Height,LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9	pDevice)
#else
int CRenderSurface::Init(UINT Width,UINT Height,LPDIRECT3D8 pD3D,LPDIRECT3DDEVICE8	pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	D3DDISPLAYMODE	d3ddm;
	HRESULT			hr;

	pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &d3ddm );

	if( FAILED( hr = g_RenderManager.CreateRenderToSurface( Width , Height , D3DFMT_A8R8G8B8 , TRUE, D3DFMT_D16 , &m_pRenderToSurface ) ) )
	{
		DeleteMemory();
		
		return FALSE;
	}
	
	if( FAILED( hr = g_RenderManager.CreateTexture( Width , Height , 1 , D3DUSAGE_RENDERTARGET , D3DFMT_A8R8G8B8 , D3DPOOL_DEFAULT, &m_pTexture ) ) )
	{
		DeleteMemory();
		
		return FALSE;
	}
	
	if( FAILED( hr = m_pTexture->GetSurfaceLevel( 0, &m_pSurface ) ) )
	{
		DeleteMemory();
		
		return FALSE;
	}

	m_Width  = Width;
	m_Height = Height;

	m_ISInit = TRUE;

	return TRUE;
}


HRESULT CRenderSurface::BeginScene(void)
{
	if( !GetReady() )
		return -1;
	
	return m_pRenderToSurface->BeginScene( m_pSurface , NULL );
}
#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderSurface::BeginScene(LPDIRECT3DSURFACE9  pSurface)
#else
HRESULT CRenderSurface::BeginScene(LPDIRECT3DSURFACE8  pSurface)
#endif // DIRECT_VERSION_9_MJH
{
	if( !GetReady() )
		return -1;
	
	return m_pRenderToSurface->BeginScene( pSurface , NULL );
}

HRESULT CRenderSurface::EndScene(void)
{
	if( !GetReady() )
		return -1;
#ifdef DIRECT_VERSION_9_MJH
	return m_pRenderToSurface->EndScene(D3DX_FILTER_LINEAR);
#else
	return m_pRenderToSurface->EndScene();
#endif // DIRECT_VERSION_9_MJH
}
