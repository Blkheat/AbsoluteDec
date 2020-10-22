// Reflection.cpp: implementation of the CReflection class.
//
//////////////////////////////////////////////////////////////////////
#include "..\\RockPCH.h"

#include "CReflection.h"
#include "..\\..\\CRenderManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReflection::CReflection()
{
	m_pTexturePrj    = NULL;
	m_pSurfacePrj    = NULL;
	m_pOldSurface    = NULL;
	m_pRenderSurface = NULL;
	
	m_Width = 0;
	m_Height = 0;
}

CReflection::~CReflection()
{
    InvalidateDeviceObject();
}

HRESULT	 CReflection::InvalidateDeviceObject()
{
	if( NULL != m_pRenderSurface )
	{
		m_pRenderSurface->Release();
		m_pRenderSurface = NULL;
	}	
	
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

	m_pOldSurface = NULL;

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	 CReflection::RestoreDeviceObject(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	 CReflection::RestoreDeviceObject(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{

	Init(pD3D, m_Width, m_Height, pDevice);	

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
int CReflection::Init(LPDIRECT3D9 pD3D,UINT Width,UINT Height,LPDIRECT3DDEVICE9 pDevice)
#else
int CReflection::Init(LPDIRECT3D8 pD3D,UINT Width,UINT Height,LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	D3DDISPLAYMODE	d3ddm;

	m_Width = Width;
	m_Height = Height;

	pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &d3ddm );
	
	if( FAILED( g_RenderManager.CreateRenderToSurface( Width , Height , D3DFMT_A8R8G8B8 , FALSE , D3DFMT_UNKNOWN , &m_pRenderSurface ) ) )
	{
		return FALSE;
	}
    	
	//D3DPOOL_DEFAULT플래그로 만든 리소스는 디바이스가 리셋될때 사라진다.
	//차후 고려 Alt-Tab를 누르는 순간에 해당 리소스는 소멸되는가...
	if( FAILED( g_RenderManager.CreateTexture( Width , Height , 1 , D3DUSAGE_RENDERTARGET , D3DFMT_A8R8G8B8 , D3DPOOL_DEFAULT , &m_pTexturePrj ) ) )
	{
		return FALSE;
	}
	if( D3D_OK != m_pTexturePrj->GetSurfaceLevel( 0 , &m_pSurfacePrj ) )
	{
		return FALSE;
	}

	return TRUE;
}

void CReflection::DInit()
{

}

#ifdef DIRECT_VERSION_9_MJH
int CReflection::Start(LPDIRECT3DDEVICE9 pDevice)
#else
int CReflection::Start(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	m_pRenderSurface->BeginScene( m_pSurfacePrj ,NULL);

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CReflection::End(LPDIRECT3DDEVICE9 pDevice)
#else
int CReflection::End(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
#ifdef DIRECT_VERSION_9_MJH
	m_pRenderSurface->EndScene( D3DX_FILTER_LINEAR );
#else
	m_pRenderSurface->EndScene();
#endif // DIRECT_VERSION_9_MJH

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CReflection::Clear(LPDIRECT3DDEVICE9 pDevice, D3DCOLOR cClColor)
#else
int CReflection::Clear(LPDIRECT3DDEVICE8 pDevice, D3DCOLOR cClColor)
#endif // DIRECT_VERSION_9_MJH
{
    g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET , cClColor , 0.0f, 0L );

	return TRUE;
}
