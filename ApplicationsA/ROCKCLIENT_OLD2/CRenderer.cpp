#pragma once

#include <RockPCH.h>

#include "CRenderer.h"
#include "CRenderManager.h"

#include "effect.h"
// #include "RockPCH.h"
// #include "effect.h"
//extern CBravoParticle		g_Particle;

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH


CRenderer g_Renderer;

#define CHECK_SAME_VALUE(n,v) \
	if( (n) == (v) ) \
		return S_OK;


CRenderer::CRenderer()
{
	m_FVF				= NULL;
	
	m_pIndexData		= NULL;
	m_BaseVertexIndex	= 0;

	m_pStreamData		= NULL;
	m_StreamNumber		= 0;
	m_Stride			= 0;

	m_bBatchRending			= FALSE;
	m_pPrevBillboardData	= NULL;
	m_pPrevBillboardTexture	= NULL;
	memset( m_pMatrixList, 0,  sizeof( m_pMatrixList ) );
}

CRenderer::~CRenderer()
{
}

void CRenderer::InitRenderer(void)
{
#ifdef BATCH_RENDER
	HRESULT hr;
#ifdef DIRECT_VERSION_9_MJH
	D3DCAPS9	d3dDevCaps;
#else
	D3DCAPS8	d3dDevCaps;
#endif // DIRECT_VERSION_9_MJH
	g_lpDevice->GetDeviceCaps(&d3dDevCaps);
	m_dwMaxPrimitiveCount = d3dDevCaps.MaxPrimitiveCount;

// 	m_dwMaxVertexCount = MAX_VERTEXCOUNT;
// 	m_dwMaxIndexCount = MAX_INDEXCOUNT;
	m_dwMaxVertexCount = 4000;
	m_dwMaxIndexCount = 6000;
//	m_dwMaxIndexCount = 6000;
	if( m_dwMaxVertexCount > m_dwMaxPrimitiveCount / 3 )
		m_dwMaxVertexCount = m_dwMaxPrimitiveCount / 3;

	if(	!m_BatchVB.Create( g_lpDevice, D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1, sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
		m_dwMaxVertexCount, D3DPOOL_SYSTEMMEM ) )  //관련 리소스를 시스템 메모리에 저장한다
//		m_dwMaxVertexCount, D3DPOOL_DEFAULT ) )
	{
		return;
	}
	
	if(FAILED(hr = g_lpDevice->CreateIndexBuffer( m_dwMaxIndexCount * sizeof(WORD),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
#ifdef DIRECT_VERSION_9_MJH
		D3DFMT_INDEX16, D3DPOOL_SYSTEMMEM, &m_pBatchIB , NULL ) ) )  //관련 리소스를 시스템 메모리에 저장한다
#else
		D3DFMT_INDEX16, D3DPOOL_SYSTEMMEM, &m_pBatchIB) ) )  //관련 리소스를 시스템 메모리에 저장한다
#endif // DIRECT_VERSION_9_MJH
//		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pBatchIB) ) )
	{ 
		return;
	}

#endif	//	BATCH_RENDER
}

HRESULT CRenderer::Clear( DWORD Count, D3DRECT* pRects, DWORD Flags, D3DCOLOR Color,
						  float Z, DWORD Stencil )
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->Clear( Count, pRects, Flags, Color,  Z, Stencil );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}

HRESULT CRenderer::BeginScene()
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->BeginScene();
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::EndScene()
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->EndScene();
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}

HRESULT CRenderer::Present( RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride,
					 RGNDATA* pDirtyRegion )
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::StretchRect( IDirect3DSurface9* pSourceSurface, RECT* pSourceRectsArray,
					    IDirect3DSurface9* pDestinationSurface,
					   RECT* pDestPointsArray ,D3DTEXTUREFILTERTYPE Filter)
#else
HRESULT CRenderer::CopyRects( IDirect3DSurface8* pSourceSurface, RECT* pSourceRectsArray,
					   UINT cRects, IDirect3DSurface8* pDestinationSurface,POINT* pDestPointsArray )
#endif // DIRECT_VERSION_9_MJH
					   
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->StretchRect( pSourceSurface, pSourceRectsArray, 
									  pDestinationSurface, pDestPointsArray, Filter );
#else
	hResult = g_lpDevice->CopyRects( pSourceSurface, pSourceRectsArray, 
									 cRects, pDestinationSurface, pDestPointsArray );
#endif // DIRECT_VERSION_9_MJH

	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}


#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::GetBackBuffer( UINT SwapChain ,UINT BackBuffer, D3DBACKBUFFER_TYPE Type,
								 IDirect3DSurface9** ppBackBuffer )
#else
HRESULT CRenderer::GetBackBuffer( UINT BackBuffer, D3DBACKBUFFER_TYPE Type,
								 IDirect3DSurface8** ppBackBuffer )
#endif // DIRECT_VERSION_9_MJH						  
{
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->GetBackBuffer( SwapChain ,BackBuffer, Type, ppBackBuffer );
#else
	hResult = g_lpDevice->GetBackBuffer( BackBuffer, Type, ppBackBuffer );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::GetDeviceCaps( D3DCAPS9* pCaps )
#else
HRESULT CRenderer::GetDeviceCaps( D3DCAPS8* pCaps )
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->GetDeviceCaps( pCaps );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}
#ifdef DIRECT_VERSION_9_MJH 
HRESULT CRenderer::CreateOffscreenPlainSurface( UINT Width, UINT Height, D3DFORMAT Format,
							D3DPOOL pool , IDirect3DSurface9** ppSurface ,HANDLE* pShareHandle )
#else
HRESULT CRenderer::CreateImageSurface( UINT Width, UINT Height, D3DFORMAT Format,
									  IDirect3DSurface8** ppSurface )
#endif // DIRECT_VERSION_9_MJH
						
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->CreateOffscreenPlainSurface( Width, Height, Format, pool, ppSurface , pShareHandle );
#else
	hResult = g_lpDevice->CreateImageSurface( Width, Height, Format, ppSurface );
#endif // DIRECT_VERSION_9_MJH
	
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::CreateVertexShader( DWORD* pDeclaration, DWORD* pFunction,
							    DWORD* pHandle, DWORD Usage )
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
#else
	hResult = g_lpDevice->CreateVertexShader( pDeclaration, pFunction, pHandle, Usage );
#endif // DIRECT_VERSION_9_MJH
	
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}

HRESULT CRenderer::SetCursorProperties( UINT XHotSpot, UINT YHotSpot,
#ifdef DIRECT_VERSION_9_MJH
									   IDirect3DSurface9* pCursorBitmap )
#else
									   IDirect3DSurface8* pCursorBitmap )
#endif // DIRECT_VERSION_9_MJH
							 
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetCursorProperties( XHotSpot, YHotSpot,
											   pCursorBitmap );
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}

BOOL CRenderer::ShowCursor( BOOL bShow )
{
	BOOL bResult = TRUE;

	EnterCriticalSection( &g_cDeviceFlag );
	bResult = g_lpDevice->ShowCursor(bShow);
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return bResult;
}

void CRenderer::SetCursorPosition( UINT XScreenSpace, UINT YScreenSpace, DWORD Flags )
{
	g_lpDevice->SetCursorPosition(XScreenSpace, YScreenSpace, Flags);
}
	
HRESULT	CRenderer::SetFVF(DWORD Value)
{
	CHECK_SAME_VALUE( m_FVF , Value );
	m_FVF = Value;

	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetFVF( m_FVF ) ;
#else
	hResult = g_lpDevice->SetVertexShader( m_FVF ) ;
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}	

#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::SetIndexBuffer(LPDIRECT3DINDEXBUFFER9 pIndexData,UINT StartIndexNum )
#else
HRESULT CRenderer::SetIndexBuffer(LPDIRECT3DINDEXBUFFER8 pIndexData,UINT StartIndexNum )
#endif // DIRECT_VERSION_9_MJH
{
	if( NULL == pIndexData )
		return S_FALSE;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult =  g_lpDevice->SetIndices( pIndexData );
#else
	hResult =  g_lpDevice->SetIndices( pIndexData , StartIndexNum );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::SetVertexBuffer(LPDIRECT3DVERTEXBUFFER9 pVertexData,UINT VertexSize,UINT StartNum)
#else
HRESULT CRenderer::SetVertexBuffer(LPDIRECT3DVERTEXBUFFER8 pVertexData,UINT VertexSize,UINT StartNum)
#endif // DIRECT_VERSION_9_MJH
{
	if( NULL == pVertexData ) 
		return S_FALSE;

	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetStreamSource( StartNum , pVertexData , 0 , VertexSize );
#else
	hResult = g_lpDevice->SetStreamSource( StartNum , pVertexData , VertexSize );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::DiableIndexBuffer(void)
{
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetIndices( NULL );
#else
	hResult = g_lpDevice->SetIndices( NULL , NULL );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::DisableVertexBuffer(void)
{
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetStreamSource( NULL , NULL , NULL , NULL);
#else
	hResult = g_lpDevice->SetStreamSource( NULL , NULL , NULL );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::Reset( D3DPRESENT_PARAMETERS * pd3dpp )
{
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->Reset( pd3dpp );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::TestCooperativeLevel()
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->TestCooperativeLevel();
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}
#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::GetViewport( D3DVIEWPORT9* pViewport )
#else
HRESULT CRenderer::GetViewport( D3DVIEWPORT8* pViewport )
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->GetViewport(pViewport);
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::GetTransform( D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix )
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->GetTransform(State, pMatrix);
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}
#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::SetLight( DWORD Index, D3DLIGHT9* pLight )
#else
HRESULT CRenderer::SetLight( DWORD Index, D3DLIGHT8* pLight )
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );	
	hResult = g_lpDevice->SetLight(Index, pLight);
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}
#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::GetLight( DWORD Index, D3DLIGHT9* pLight )
#else
HRESULT CRenderer::GetLight( DWORD Index, D3DLIGHT8* pLight )
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );	
	hResult = g_lpDevice->GetLight(Index, pLight);
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::SetMaterial( D3DMATERIAL9* pMaterial )
#else
HRESULT CRenderer::SetMaterial( D3DMATERIAL8* pMaterial )
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hResult = S_FALSE;	

	EnterCriticalSection( &g_cDeviceFlag );	
	hResult = g_lpDevice->SetMaterial(pMaterial);
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::GetMaterial( D3DMATERIAL9*pMaterial )
#else
HRESULT CRenderer::GetMaterial( D3DMATERIAL8*pMaterial )
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hResult = S_FALSE;	

	EnterCriticalSection( &g_cDeviceFlag );	
	hResult = g_lpDevice->GetMaterial(pMaterial);
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::CreateVertexBuffer( UINT Length, DWORD Usage, DWORD FVF,
#ifdef DIRECT_VERSION_9_MJH
									  D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer )
#else
									  D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer )
#endif // DIRECT_VERSION_9_MJH							 
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer , NULL );
#else
	hResult = g_lpDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer);
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::CreateIndexBuffer( UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
#ifdef DIRECT_VERSION_9_MJH
									 IDirect3DIndexBuffer9** ppIndexBuffer )
#else
									 IDirect3DIndexBuffer8** ppIndexBuffer )
#endif // DIRECT_VERSION_9_MJH
							 
{
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer , NULL );
#else
	hResult = g_lpDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer);
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::LightEnable( DWORD LightIndex, BOOL bEnable )
{
	HRESULT hResult = S_FALSE;
	
	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->LightEnable(LightIndex, bEnable);
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::SetTransform( D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix )
{
	HRESULT hResult = S_FALSE;

	if( State < 50 )
	{
		m_pMatrixList[State] = *pMatrix;
	}

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->SetTransform(State, pMatrix);
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}

HRESULT CRenderer::DrawPrimitive( D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex,
						   UINT PrimitiveCount )
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->DrawPrimitive( PrimitiveType, StartVertex, 
										 PrimitiveCount );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::DrawPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount,
						 void* pVertexStreamZeroData, UINT VertexStreamZeroStride )
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = g_lpDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, 
										  pVertexStreamZeroData, VertexStreamZeroStride );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::DrawIndexedPrimitive( D3DPRIMITIVETYPE Type, UINT MinIndex, UINT NumVertices,
								  UINT StartIndex, UINT PrimitiveCount )
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->DrawIndexedPrimitive( Type, 0 , MinIndex, NumVertices, 
												StartIndex, PrimitiveCount );
#else
	hResult = g_lpDevice->DrawIndexedPrimitive( Type, MinIndex, NumVertices, 
												StartIndex, PrimitiveCount );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::CreateTextureFromFileEx( LPCTSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels,
									     	DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter,
								            DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo,
#ifdef DIRECT_VERSION_9_MJH
											PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture )
#else
											PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE8* ppTexture )
#endif // DIRECT_VERSION_9_MJH								         
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = D3DXCreateTextureFromFileEx(g_lpDevice, pSrcFile, Width, Height, MipLevels,
										  Usage, Format, Pool, Filter, MipFilter, ColorKey,
										  pSrcInfo, pPalette, ppTexture);
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}
#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::CreateTextureFromFile( LPCTSTR pSrcFile, LPDIRECT3DTEXTURE9* ppTexture )
#else
HRESULT CRenderer::CreateTextureFromFile( LPCTSTR pSrcFile, LPDIRECT3DTEXTURE8* ppTexture )
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = D3DXCreateTextureFromFile(g_lpDevice, pSrcFile, ppTexture);
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}

HRESULT CRenderer::CreateTextureFromFileInMemoryEx( LPCVOID pSrcData, UINT SrcData, UINT Width,
										 UINT Height, UINT MipLevels, DWORD Usage,
										 D3DFORMAT Format, D3DPOOL Pool, DWORD Filter,
										 DWORD MipFilter, D3DCOLOR ColorKey, 
										 D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, 
#ifdef DIRECT_VERSION_9_MJH
										 LPDIRECT3DTEXTURE9* ppTexture )
#else
										 LPDIRECT3DTEXTURE8* ppTexture )
#endif // DIRECT_VERSION_9_MJH									
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = D3DXCreateTextureFromFileInMemoryEx( g_lpDevice, pSrcData, SrcData, Width,
												   Height, MipLevels, Usage, Format, 
												   Pool, Filter, MipFilter, ColorKey, 
												   pSrcInfo, pPalette, ppTexture );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::CreateRenderToSurface( UINT Width, UINT Height, D3DFORMAT Format, BOOL DepthStencil,
							   D3DFORMAT DepthStencilFormat, 
							   LPD3DXRENDERTOSURFACE* ppRenderToSurface )
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = D3DXCreateRenderToSurface( g_lpDevice, Width, Height, Format, DepthStencil,
										 DepthStencilFormat, ppRenderToSurface);
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

HRESULT CRenderer::CreateTexture( UINT Width, UINT Height, UINT MipLevels, DWORD Usage,
#ifdef DIRECT_VERSION_9_MJH
								 D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DTEXTURE9* ppTexture )
#else
								 D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DTEXTURE8* ppTexture )
#endif // DIRECT_VERSION_9_MJH			
{
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	hResult = D3DXCreateTexture( g_lpDevice, Width, Height, MipLevels, Usage, Format,
								 Pool, ppTexture );
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::SetIndices( IDirect3DIndexBuffer9* pIndexData, UINT BaseVertexIndex )
#else
HRESULT CRenderer::SetIndices( IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex )
#endif // DIRECT_VERSION_9_MJH
{
	
	///-- 버텍스 버퍼와 인덱스 버퍼는 다이나믹버퍼를 사용하기때문에...
	///-- 해당 방법을 사용할수 없음 
	/*if( (m_pIndexData == pIndexData) && (m_BaseVertexIndex == BaseVertexIndex) )
	{
		return S_OK;
	}*/
	
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	m_pIndexData = pIndexData;
	m_BaseVertexIndex = BaseVertexIndex;
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetIndices( pIndexData );
#else
	hResult = g_lpDevice->SetIndices( pIndexData, BaseVertexIndex );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );

	return hResult;
}
#ifdef DIRECT_VERSION_9_MJH
HRESULT CRenderer::SetStreamSource( UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData,
#else
HRESULT CRenderer::SetStreamSource( UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData,
#endif // DIRECT_VERSION_9_MJH
									UINT Stride )
{
	/*if( (m_pStreamData == pStreamData) && (m_StreamNumber == StreamNumber) && (m_Stride == Stride) )
	{
		return S_OK;
	}*/
	
	HRESULT hResult = S_FALSE;

	EnterCriticalSection( &g_cDeviceFlag );
	m_pStreamData = pStreamData;
	m_StreamNumber = StreamNumber;
	m_Stride = Stride;
#ifdef DIRECT_VERSION_9_MJH
	hResult = g_lpDevice->SetStreamSource( StreamNumber, pStreamData, 0 ,Stride );
#else
	hResult = g_lpDevice->SetStreamSource( StreamNumber, pStreamData, Stride );
#endif // DIRECT_VERSION_9_MJH
	LeaveCriticalSection( &g_cDeviceFlag );
	
	return hResult;
}

//----------------------------------------------------------------------------------------



void CRenderer::BeginBatchPrimitive()
{
//	SetCurrentStateValue(eVType);
	m_BatchVB.BeginBatch();
// 	m_dwBatchIndexBase = m_dwIndexBase;
// 	m_BatchVB.m_dwBatchBase = m_BatchVB.m_dwBase;
// 	m_BatchVB.m_dwBase = m_BatchVB.m_dwBatchBase;
//  	m_dwIndexBase = m_dwBatchIndexBase;	

}

void CRenderer::EndBatchPrimitive()
{
	FlushBatchPrimitive();
// 	m_eVertexType				= eVertexType_None;
// 	m_nVertexSize				= -1;
// 	m_dwFVF						= 0;
	m_pPrevBillboardTexture		= NULL;
	m_PrevSrcBlend				= 0;
	m_PrevDestBlend				= 0;
	m_PrevHashCode				= -1;
	m_bBatchRending				= FALSE;
}

void CRenderer::FlushBatchPrimitive()
{
	int		vertexCount;
	int		triCount;
	
	vertexCount = m_BatchVB.m_dwBatchBase - m_BatchVB.m_dwBase;	
//	triCount = vertexCount / 2;
	triCount = ( m_dwBatchIndexBase - m_dwIndexBase ) / 3;

	if( triCount )
	{
		SetFVF( D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1 );
		SetStreamSource( 0, m_BatchVB.m_pVB, sizeof(VERTEX_XYZ_DIFFUSE_TEX1) );
#if 0
		SetIndices( m_pBatchIB, m_BatchVB.m_dwBase);
		DrawIndexedPrimitive( D3DPT_TRIANGLELIST, m_dwIndexBase, m_BatchVB.m_dwBatchBase, 0, triCount);
#else
		SetIndices( m_pBatchIB, 0);
		DrawIndexedPrimitive( D3DPT_TRIANGLELIST, m_BatchVB.m_dwBase,
			m_BatchVB.m_dwBatchBase, m_dwIndexBase, triCount);
#endif
		m_BatchVB.m_dwBase = m_BatchVB.m_dwBatchBase;
		m_dwIndexBase = m_dwBatchIndexBase;	
	}
}

void CRenderer::AddBatchIndexedVertex( VERTEX_XYZ_DIFFUSE_TEX1 *pVertexList, int vertexCount,
							  WORD *indexList, int indexCount )
{
	BOOL	vertexExceed = ( m_BatchVB.m_dwBatchBase + vertexCount ) >= m_dwMaxVertexCount;
	BOOL	indexExceed = ( m_dwBatchIndexBase + indexCount ) >= m_dwMaxIndexCount;

	int		nIndexOffset;
		
	if( vertexExceed || indexExceed )
	{
		if(m_pPrevBillboardTexture)
		{
			g_RenderManager.SetTexture( 0, m_pPrevBillboardTexture );
			g_RenderManager.SetSrcBlend( m_PrevSrcBlend );
			g_RenderManager.SetDestBlend( m_PrevDestBlend );
		}

		FlushBatchPrimitive();

// 		if( vertexExceed )
// 		{
// 			m_BatchVB.m_dwBase = 0;
// 			m_BatchVB.m_dwBatchBase = 0;
// 		}
// 
// 		if( indexExceed )
// 		{
// 			m_dwIndexBase = 0;
// 			m_dwBatchIndexBase = 0;
// 		}
		
		m_BatchVB.m_dwBase = 0;
		m_BatchVB.m_dwBatchBase = 0;
		m_dwIndexBase = 0;
		m_dwBatchIndexBase = 0;
	}

	nIndexOffset = m_BatchVB.m_dwBatchBase;

	CopyBatchVertex( pVertexList, vertexCount);
	CopyBatchIndex( indexList, indexCount, nIndexOffset);
}

void CRenderer::CopyBatchIndex( WORD *indexList, int indexCount, int indexOffset)
{
	WORD		*pIndex, *pTmpIndex, *pIndexList;
	HRESULT		hr;
	int			i;
	int			indexStart = m_dwBatchIndexBase - m_dwIndexBase;
	DWORD		dwFlag = 0;
	

	
	/*	Lock Index Buffer
	*/
#ifdef BATCH_OPTIMIZATION
	if(m_dwBatchIndexBase == 0)
		dwFlag		= D3DLOCK_DISCARD;
	else
		dwFlag		= D3DLOCK_NOOVERWRITE;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( hr = m_pBatchIB->Lock( m_dwBatchIndexBase * sizeof( WORD ),
									indexCount * sizeof( WORD ),
									(void**)&pIndex, dwFlag ) ) )
#else
	if( FAILED( hr = m_pBatchIB->Lock( m_dwBatchIndexBase * sizeof( WORD ),
									indexCount * sizeof( WORD ),
									(BYTE**)&pIndex, dwFlag ) ) )
#endif // DIRECT_VERSION_9_MJH
#else	//	BATCH_OPTIMIZATION
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( hr = m_pBatchIB->Lock( m_dwBatchIndexBase * sizeof( WORD ),
	indexCount * sizeof( WORD ),
	(void**)&pIndex,
	( m_dwIndexBase == m_dwBatchIndexBase ) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE ) ) )
#else
	if( FAILED( hr = m_pBatchIB->Lock( m_dwBatchIndexBase * sizeof( WORD ),
	indexCount * sizeof( WORD ),
	(BYTE**)&pIndex,
	( m_dwIndexBase == m_dwBatchIndexBase ) ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE ) ) )
#endif // DIRECT_VERSION_9_MJH
#endif	//	BATCH_OPTIMIZATION
	{
		return;
	}


	/*	Copy Index
	*/

	pTmpIndex = pIndex;
	pIndexList = indexList;
	
	for( i = 0; i < indexCount; i++, pTmpIndex++, pIndexList++ )
	{
		*pTmpIndex = ( *pIndexList ) + indexOffset;
	}

	m_pBatchIB->Unlock();

	m_dwBatchIndexBase += indexCount;	
}


//----------------------------------------------------------------------------------------

void CRenderer::CopyBatchVertex( VERTEX_XYZ_DIFFUSE_TEX1 *pVertexList, int vertexCount)
{
	VERTEX_XYZ_DIFFUSE_TEX1	*pBuffer;
	VERTEX_XYZ_DIFFUSE_TEX1	*pVertex;
	HRESULT		hr;
	int			nSizeToLock = 0;
	DWORD		dwFlag = 0;

	if(m_BatchVB.m_dwBatchBase == 0)
		dwFlag		= D3DLOCK_DISCARD;
	else
		dwFlag		= D3DLOCK_NOOVERWRITE;

#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( hr = m_BatchVB.m_pVB->Lock( m_BatchVB.m_dwBatchBase * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
						vertexCount * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
						(void**)&pBuffer, dwFlag ) ) )
#else
	if( FAILED( hr = m_BatchVB.m_pVB->Lock( m_BatchVB.m_dwBatchBase * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
						vertexCount * sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
						(BYTE**)&pBuffer, dwFlag ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return;
	}
	
	pVertex = pVertexList;

#if 0
	for( int i = 0; i < vertexCount; i++, pBuffer++, pVertex++ )
	{
		pBuffer->v.x = pVertex->v.x;
		pBuffer->v.y = pVertex->v.y;
		pBuffer->v.z = pVertex->v.z;
		
		pBuffer->color		= pVertex->color;
		
		pBuffer->tu = pVertex->tu;
		pBuffer->tv = pVertex->tv;
	}
#else
	CopyMemory( pBuffer, pVertex, sizeof(VERTEX_XYZ_DIFFUSE_TEX1) * vertexCount );
#endif
	m_BatchVB.m_pVB->Unlock();
	
	m_BatchVB.m_dwBatchBase += vertexCount;
}