//.......................................................................................................
// 
// Renderer Header																			By wxywxy
//
//.......................................................................................................

#ifndef __CRENDERER_H
#define __CRENDERER_H

#include "RockPCH.h"

//#include "RockClient.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3d9.h>
#include <D3dx9tex.h>
#else
#include <d3d8.h>
#include <D3dx8tex.h>
#endif // DIRECT_VERSION_9_MJH

 

#include "CDynamicVertexBuffer.h"
#include <EFFECT\\EffectVertex.h>


class CBillboardData;

class CRenderer
{
public:
	
	 CRenderer();
	~CRenderer();

public:

	void InitRenderer(void);

	HRESULT Clear( DWORD Count, D3DRECT* pRects, DWORD Flags, D3DCOLOR Color,
				   float Z, DWORD Stencil );

	HRESULT EndScene();
	HRESULT BeginScene();

	HRESULT Present( RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride,
					 RGNDATA* pDirtyRegion = NULL );

	BOOL	ShowCursor( BOOL bShow );
	void	SetCursorPosition( UINT XScreenSpace, UINT YScreenSpace, DWORD Flags );
#ifdef DIRECT_VERSION_9_MJH
	HRESULT StretchRect( IDirect3DSurface9* pSourceSurface, RECT* pSourceRectsArray,
					    IDirect3DSurface9* pDestinationSurface,
					   RECT* pDestPointsArray ,D3DTEXTUREFILTERTYPE Filter);
#else
	HRESULT CopyRects( IDirect3DSurface8* pSourceSurface, RECT* pSourceRectsArray,
				   UINT cRects, IDirect3DSurface8* pDestinationSurface,POINT* pDestPointsArray );
#endif // DIRECT_VERSION_9_MJH
					   
#ifdef DIRECT_VERSION_9_MJH
	HRESULT GetDeviceCaps( D3DCAPS9* pCaps );
#else
	HRESULT GetDeviceCaps( D3DCAPS8* pCaps );
#endif // DIRECT_VERSION_9_MJH	

	
#ifdef DIRECT_VERSION_9_MJH
	HRESULT GetBackBuffer(  UINT SwapChain ,UINT BackBuffer, D3DBACKBUFFER_TYPE Type,
						IDirect3DSurface9** ppBackBuffer );
#else
	HRESULT GetBackBuffer( UINT BackBuffer, D3DBACKBUFFER_TYPE Type,
						IDirect3DSurface8** ppBackBuffer );
#endif // DIRECT_VERSION_9_MJH						   

	HRESULT CreateVertexShader( DWORD* pDeclaration, DWORD* pFunction,
							    DWORD* pHandle, DWORD Usage );
#ifdef DIRECT_VERSION_9_MJH	
	HRESULT CreateOffscreenPlainSurface( UINT Width, UINT Height, D3DFORMAT Format,
								D3DPOOL pool ,IDirect3DSurface9** ppSurface , HANDLE* pShareHandle );
#else
	HRESULT CreateImageSurface( UINT Width, UINT Height, D3DFORMAT Format,
							IDirect3DSurface8** ppSurface );
#endif // DIRECT_VERSION_9_MJH
	
	HRESULT SetCursorProperties( UINT XHotSpot, UINT YHotSpot,
#ifdef DIRECT_VERSION_9_MJH
								IDirect3DSurface9* pCursorBitmap );
#else
								IDirect3DSurface8* pCursorBitmap );
#endif // DIRECT_VERSION_9_MJH

	
	HRESULT	SetFVF(DWORD Value);
#ifdef DIRECT_VERSION_9_MJH
	HRESULT SetIndexBuffer(LPDIRECT3DINDEXBUFFER9 pIndexData,UINT StartIndexNum = 0 );
	HRESULT SetVertexBuffer(LPDIRECT3DVERTEXBUFFER9 pVertexData,UINT VertexSize,UINT StartNum = 0 );
#else
	HRESULT SetIndexBuffer(LPDIRECT3DINDEXBUFFER8 pIndexData,UINT StartIndexNum = 0 );
	HRESULT SetVertexBuffer(LPDIRECT3DVERTEXBUFFER8 pVertexData,UINT VertexSize,UINT StartNum = 0 );
#endif // DIRECT_VERSION_9_MJH

	HRESULT DiableIndexBuffer(void);
	HRESULT DisableVertexBuffer(void);
	
	DWORD   GetFVF()  { return m_FVF; }
#ifdef DIRECT_VERSION_9_MJH
	HRESULT SetLight( DWORD Index, D3DLIGHT9* pLight );
	HRESULT SetMaterial( D3DMATERIAL9* pMaterial );

	HRESULT GetLight( DWORD Index, D3DLIGHT9* pLight );
	HRESULT GetMaterial( D3DMATERIAL9*pMaterial );
#else
	HRESULT SetLight( DWORD Index, D3DLIGHT8* pLight );
	HRESULT SetMaterial( D3DMATERIAL8* pMaterial );

	HRESULT GetLight( DWORD Index, D3DLIGHT8* pLight );
	HRESULT GetMaterial( D3DMATERIAL8*pMaterial );
#endif // DIRECT_VERSION_9_MJH

	HRESULT LightEnable( DWORD LightIndex, BOOL bEnable );
	
	HRESULT Reset( D3DPRESENT_PARAMETERS * pd3dpp );
	HRESULT TestCooperativeLevel();// D3D 디바이스의 현재 협력레벨 상태를 리턴

#ifdef DIRECT_VERSION_9_MJH
	HRESULT GetViewport( D3DVIEWPORT9* pViewport );
#else
	HRESULT GetViewport( D3DVIEWPORT8* pViewport );
#endif // DIRECT_VERSION_9_MJH
	HRESULT GetTransform( D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix );
	
#ifdef DIRECT_VERSION_9_MJH
	HRESULT SetIndices( IDirect3DIndexBuffer9* pIndexData, UINT BaseVertexIndex );
	HRESULT SetStreamSource( UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData,
#else
	HRESULT SetIndices( IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex );
	HRESULT SetStreamSource( UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData,
#endif // DIRECT_VERSION_9_MJH
							 UINT Stride );	
	
	HRESULT CreateVertexBuffer( UINT Length, DWORD Usage, DWORD FVF,
#ifdef DIRECT_VERSION_9_MJH
								D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer );
#else
								D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer );	
#endif // DIRECT_VERSION_9_MJH							  
	HRESULT CreateIndexBuffer( UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
#ifdef DIRECT_VERSION_9_MJH
								IDirect3DIndexBuffer9** ppIndexBuffer );
#else
								IDirect3DIndexBuffer8** ppIndexBuffer );
#endif // DIRECT_VERSION_9_MJH							   	
	
	HRESULT SetTransform( D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix );



	HRESULT CreateTextureFromFileEx( LPCTSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels,
									 DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter,
									 DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo,
#ifdef DIRECT_VERSION_9_MJH
									 PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture );
#else
									 PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE8* ppTexture );
#endif // DIRECT_VERSION_9_MJH	
#ifdef DIRECT_VERSION_9_MJH
	HRESULT CreateTextureFromFile( LPCTSTR pSrcFile, LPDIRECT3DTEXTURE9* ppTexture );
#else
	HRESULT CreateTextureFromFile( LPCTSTR pSrcFile, LPDIRECT3DTEXTURE8* ppTexture );
#endif // DIRECT_VERSION_9_MJH							

	HRESULT CreateTextureFromFileInMemoryEx( LPCVOID pSrcData, UINT SrcData, UINT Width,
											 UINT Height, UINT MipLevels, DWORD Usage,
											 D3DFORMAT Format, D3DPOOL Pool, DWORD Filter,
											 DWORD MipFilter, D3DCOLOR ColorKey, 
											 D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, 
#ifdef DIRECT_VERSION_9_MJH
											 LPDIRECT3DTEXTURE9* ppTexture );
#else
											 LPDIRECT3DTEXTURE8* ppTexture );
#endif // DIRECT_VERSION_9_MJH								

	HRESULT CreateRenderToSurface( UINT Width, UINT Height, D3DFORMAT Format, BOOL DepthStencil,
								   D3DFORMAT DepthStencilFormat, 
								   LPD3DXRENDERTOSURFACE* ppRenderToSurface );
	HRESULT CreateTexture( UINT Width, UINT Height, UINT MipLevels, DWORD Usage,
#ifdef DIRECT_VERSION_9_MJH
		D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DTEXTURE9* ppTexture );
#else
		D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DTEXTURE8* ppTexture );
#endif // DIRECT_VERSION_9_MJH						   


	HRESULT DrawPrimitive( D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex,
						   UINT PrimitiveCount );	
	
	HRESULT DrawPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount,
							 void* pVertexStreamZeroData, UINT VertexStreamZeroStride );

	HRESULT DrawIndexedPrimitive( D3DPRIMITIVETYPE Type, UINT MinIndex, UINT NumVertices,
								  UINT StartIndex, UINT PrimitiveCount );


	void	BeginBatchPrimitive();
	void	EndBatchPrimitive(void);
   	void	AddBatchIndexedVertex( VERTEX_XYZ_DIFFUSE_TEX1 *pVertexList, int vertexCount,
   		WORD *indexList, int indexCount );
	void	CopyBatchVertex( VERTEX_XYZ_DIFFUSE_TEX1 *pVertexList, int vertexCount);
	void	CopyBatchIndex( WORD *indexList, int indexCount, int indexOffset);
	void	FlushBatchPrimitive();

	BOOL	GetBeginBatchRendering()							{	return m_bBatchRending;				}
	void	SetBeginBatchRendering(BOOL bBatchRending)			{	m_bBatchRending = bBatchRending;	}

	CBillboardData*	GetPrevBillboardData()						{	return m_pPrevBillboardData;		}
	void			SetPrevBillboardData(CBillboardData	* PrevBillboardData)
																{	m_pPrevBillboardData = PrevBillboardData;}

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	GetPrevBillboardTexture()					{	return m_pPrevBillboardTexture;		}
	void			SetPrevBillboardTexture(LPDIRECT3DTEXTURE9	pPrevBillboardTexture) 
																{	m_pPrevBillboardTexture = pPrevBillboardTexture;}
#else
	LPDIRECT3DTEXTURE8	GetPrevBillboardTexture()					{	return m_pPrevBillboardTexture;		}
	void			SetPrevBillboardTexture(LPDIRECT3DTEXTURE8	pPrevBillboardTexture) 
																{	m_pPrevBillboardTexture = pPrevBillboardTexture;}
#endif // DIRECT_VERSION_9_MJH

public:	

	CBillboardData*		m_pPrevBillboardData;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	m_pPrevBillboardTexture;
#else
	LPDIRECT3DTEXTURE8	m_pPrevBillboardTexture;
#endif // DIRECT_VERSION_9_MJH
	BYTE				m_PrevSrcBlend;
	BYTE				m_PrevDestBlend;
	unsigned long		m_PrevHashCode;
	BOOL				m_bBatchRending;
	int					m_iPrevType;
//#endif	//	BATCH_RENDER

protected:

	DWORD	m_FVF;
#ifdef DIRECT_VERSION_9_MJH
	IDirect3DIndexBuffer9 * m_pIndexData;
	IDirect3DVertexBuffer9* m_pStreamData;
#else
	IDirect3DIndexBuffer8 * m_pIndexData;
	IDirect3DVertexBuffer8* m_pStreamData;
#endif // DIRECT_VERSION_9_MJH
	UINT m_BaseVertexIndex;
	UINT m_StreamNumber;
	UINT m_Stride;	
	
	D3DMATRIX  m_pMatrixList[50];
//#ifdef BATCH_RENDER
public:
	DWORD							m_dwIndexBase;
	DWORD							m_dwBatchIndexBase;
	CDynamicVertexBuffer			m_BatchVB;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DINDEXBUFFER9			m_pBatchIB;
#else
	LPDIRECT3DINDEXBUFFER8			m_pBatchIB;
#endif // DIRECT_VERSION_9_MJH
	
	DWORD							m_dwMaxVertexCount;
	DWORD							m_dwMaxIndexCount;
	DWORD							m_dwMaxPrimitiveCount;
//#endif	//	BATCH_RENDER

};
 
extern CRenderer g_Renderer;



#endif