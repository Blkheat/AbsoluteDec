#include "CTargetSurface.h"
#include "..\\..\\CRenderManager.h"

CTargetSurface::CTargetSurface()
{
	m_pTexturePrj    = NULL;
	m_pSurfacePrj    = NULL;
	m_pOldSurface    = NULL;
	m_pRenderSurface = NULL;
	
	m_Width  = 0;
	m_Height = 0;
}

CTargetSurface::~CTargetSurface()
{
	InvalidateDeviceObject();
}

HRESULT	CTargetSurface::InvalidateDeviceObject()
{
	if( NULL != m_pTexturePrj )
	{
		m_pTexturePrj->Release();

		m_pTexturePrj = NULL;
	}

	if( NULL != m_pSurfacePrj )
	{
		m_pSurfacePrj->Release();

		m_pSurfacePrj = NULL;
	}

	if( NULL != m_pRenderSurface )
	{
		m_pRenderSurface->Release();

		m_pRenderSurface = NULL;
	}

	m_pOldSurface = NULL;
	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CTargetSurface::RestoreDeviceObject(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CTargetSurface::RestoreDeviceObject(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	
	Init(pD3D, m_Width, m_Height, pDevice);

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
int CTargetSurface::Init(LPDIRECT3D9 pD3D,UINT Width,UINT Height,LPDIRECT3DDEVICE9 pDevice)
#else
int CTargetSurface::Init(LPDIRECT3D8 pD3D,UINT Width,UINT Height,LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	m_Width  = Width;
	m_Height = Height;
	
	D3DDISPLAYMODE	d3ddm;

	pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &d3ddm );
	
	HRESULT hr;
	
	if( FAILED( hr = g_RenderManager.CreateRenderToSurface( Width , Height , d3ddm.Format , FALSE , D3DFMT_UNKNOWN , &m_pRenderSurface ) ))
	{
		return FALSE;
	}

	if( FAILED( hr = g_RenderManager.CreateTexture( Width , Height , 1 , D3DUSAGE_RENDERTARGET , d3ddm.Format , D3DPOOL_DEFAULT , &m_pTexturePrj ) ))
	{
		return FALSE;
	}


	if( FAILED( hr = m_pTexturePrj->GetSurfaceLevel( 0 , &m_pSurfacePrj ) ) )
	{
		return FALSE;
	}

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CTargetSurface::Start(LPDIRECT3DDEVICE9 pDevice)
#else
int CTargetSurface::Start(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hr;

	if( FAILED( hr = m_pRenderSurface->BeginScene( m_pSurfacePrj ,NULL) ) )
	{
		return FALSE;
	}

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CTargetSurface::End(LPDIRECT3DDEVICE9 pDevice)
#else
int CTargetSurface::End(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hr;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( hr = m_pRenderSurface->EndScene( D3DX_FILTER_LINEAR ) ) )
#else
	if( FAILED( hr = m_pRenderSurface->EndScene() ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;
	}

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CTargetSurface::Clear(LPDIRECT3DDEVICE9 pDevice)
#else
int CTargetSurface::Clear(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET , D3DCOLOR_XRGB( 0 , 0 ,0 ) , 0.0f, 0L );

	return TRUE;
}