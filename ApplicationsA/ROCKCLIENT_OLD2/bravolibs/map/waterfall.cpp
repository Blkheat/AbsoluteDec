#include "RockPCH.h"

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <time.h>
#ifdef DIRECT_VERSION_9_MJH
#include "d3d9.h"
#include "d3dx9.h"
#else
#include "d3d8.h"
#include "d3dx8.h"
#endif // DIRECT_VERSION_9_MJH
#include "base\\dxutil.h"
//#include "d3drenderobj.h"
//#include "d3dfvf.h"
#include "water.h"
#include "waterfall.h"
#include "..\\quadlist.h"
#include "field.h"

#include "..\\..\\CRenderManager.h"

extern	char	g_szRoot[128];
#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9	g_FallEffectTex[10];
#else
LPDIRECT3DTEXTURE8	g_FallEffectTex[10];
#endif // DIRECT_VERSION_9_MJH

CEffectiveWater::CEffectiveWater()
{
	m_pTexture	= NULL;
	m_pFinal	= NULL;
	m_byRunsDir	= RD_NONE;
}

CEffectiveWater::~CEffectiveWater()
{
}

// Attributes
void	CEffectiveWater::BuildTable(const float& fRefractionIndex, const float& fDepth)
{
	int		nArraySize	= sizeof(m_lutDisplacement);
	float	fHeightDiff, fAngle, fBeamAngle;

	for ( int i = -(nArraySize>>1); i < (nArraySize>>1)-1; i ++ )
	{
		fHeightDiff	= i*fDepth;
		fAngle	= (float)atan(fHeightDiff);
		fBeamAngle	= (float)asin(sin(fAngle) / fRefractionIndex);
		m_lutDisplacement[i+(nArraySize>>1)]	= (int)(tan(fBeamAngle)*fHeightDiff);
	}
}

void	CEffectiveWater::Process()
{
	for ( int y = 0; y < ETEXTURESIZE; y ++ )
	{
		for ( int x = 0; x < ETEXTURESIZE; x ++ )
		{
			int value;
			int xminus1 = x-1; if (xminus1 < 0) xminus1 = 0;
			int xminus2 = x-2; if (xminus2 < 0) xminus2 = 0;
			int yminus1 = y-1; if (yminus1 < 0) yminus1 = 0;
			int yminus2 = y-2; if (yminus2 < 0) yminus2 = 0;

			int xplus1 = x+1; if (xplus1 >= ETEXTURESIZE) xplus1 = ETEXTURESIZE-1;
			int xplus2 = x+2; if (xplus2 >= ETEXTURESIZE) xplus2 = ETEXTURESIZE-1;
			int yplus1 = y+1; if (yplus1 >= ETEXTURESIZE) yplus1 = ETEXTURESIZE-1;
			int yplus2 = y+2; if (yplus2 >= ETEXTURESIZE) yplus2 = ETEXTURESIZE-1;


			{
				value  = m_pWaterActive[((y)      *ETEXTURESIZE)+xminus1];
				value += m_pWaterActive[((y)      *ETEXTURESIZE)+xplus1];
				value += m_pWaterActive[((yminus1)*ETEXTURESIZE)+x];
				value += m_pWaterActive[((yplus1) *ETEXTURESIZE)+x];
				value /= 2;
			}
			value -= m_pWaterScratch[(y*ETEXTURESIZE)+x];
			value = (int)((float)value / 1.05f);
			m_pWaterScratch[(y*ETEXTURESIZE)+x] = value;
		}
	}
}

HRESULT	CEffectiveWater::BuildFinal()
{
	// lock texture
	D3DLOCKED_RECT rect_src, rect_dest;
	::ZeroMemory(&rect_src, sizeof(rect_src));
	::ZeroMemory(&rect_dest, sizeof(rect_dest));

	if (FAILED(m_pTexture->LockRect(0, &rect_src, NULL, 0))) return	Error("LockSrc");
	if (FAILED(m_pFinal->LockRect(0, &rect_dest, NULL, 0))) return	Error("LockDest");

	// our texture surface is now locked, and we can use the pitch to traverse it.
	DWORD *pSrc = (DWORD *)(rect_src.pBits);
	DWORD *pDest= (DWORD *)(rect_dest.pBits);

	int dest_index=0;
	int src_pitch = rect_src.Pitch/4; // in DWORDS

	// this could be optimized a LOT.  It's this way so you can learn the technique.

	for (int y=0; y < ETEXTURESIZE; y++) 
	{
		for (int x=0; x < ETEXTURESIZE; x++) 
		{
			int xdiff = (x == ETEXTURESIZE-1)  ? 0 : m_pWaterActive[(y*ETEXTURESIZE)+x+1]   - m_pWaterActive[(y*ETEXTURESIZE)+x];
			int ydiff = (y == ETEXTURESIZE-1) ? 0 : m_pWaterActive[((y+1)*ETEXTURESIZE)+x] - m_pWaterActive[(y*ETEXTURESIZE)+x];

			if ( xdiff > ETEXTURESIZE || xdiff < -ETEXTURESIZE)
				xdiff	= 0;
			if ( ydiff > ETEXTURESIZE || ydiff < -ETEXTURESIZE)
				ydiff	= 0;

			int xdisp = m_lutDisplacement[(xdiff+ETEXTURESIZE) % (ETEXTURESIZE*2)];
			int ydisp = m_lutDisplacement[(ydiff+ETEXTURESIZE) % (ETEXTURESIZE*2)];

			if (xdiff < 0) 
			{
				if (ydiff < 0) 
				{
					if (y-ydisp < 0 || y-ydisp >= ETEXTURESIZE || x-xdisp < 0 || x-xdisp >= ETEXTURESIZE)
						pDest[dest_index++] = pSrc[0];    
					else
						pDest[dest_index++] = pSrc[((y-ydisp)*src_pitch)+x-xdisp];  
				}
				else 
				{
					if (y+ydisp < 0 || y+ydisp >= ETEXTURESIZE || x-xdisp < 0 || x-xdisp >= ETEXTURESIZE)
						pDest[dest_index++] = pSrc[0];    
					else
						pDest[dest_index++] = pSrc[((y+ydisp)*src_pitch)+x-xdisp];  
				}
			}
			else 
			{
				if (ydiff < 0) 
				{
					if (y-ydisp < 0 || y-ydisp >= ETEXTURESIZE || x+xdisp < 0 || x+xdisp >= ETEXTURESIZE)
						pDest[dest_index++] = pSrc[0];    
					else
						pDest[dest_index++] = pSrc[((y-ydisp)*src_pitch)+x+xdisp];  
				}
				else 
				{
					if (y+ydisp < 0 || y+ydisp >= ETEXTURESIZE || x+xdisp < 0 || x+xdisp >= ETEXTURESIZE)
						pDest[dest_index++] = pSrc[0];    
					else
						pDest[dest_index++] = pSrc[((y+ydisp)*src_pitch)+x+xdisp];  
				}
			}
		}
		// next line
		dest_index += (rect_dest.Pitch - (ETEXTURESIZE*4))/4;
	}

	// unlock texture surface
	if (FAILED(m_pTexture->UnlockRect(0))) return	Error("UnlockSrc");
	if (FAILED(m_pFinal->UnlockRect(0))) return	Error("UnlockDest");

	return S_OK;
}

void	CEffectiveWater::BuildDroplet(int iX, int iY, int iSize, int iSplashStrength)
{
	for (int x=iX-iSize; x <= iX+iSize; x++) 
	{
		for (int y=iY-iSize; y <= iY+iSize; y++) 
		{
			// make sure we're in bounds
			if (x < 0 || x >= ETEXTURESIZE || y < 0 || y >= ETEXTURESIZE) 
				continue;

			// see if the point at (x,y) is within the circle of radius size
			int square_x    = (x-iX)*(x-iX);
			int square_y    = (y-iY)*(y-iY);
			int square_size = iSize*iSize;

			if (square_x+square_y <= square_size) 
			{
				// it's within the size circle!  apply it to the water buffer.
				m_pWaterActive[(y*ETEXTURESIZE)+x] += (int)(((float)iSplashStrength)*sqrt((float)square_x+(float)square_y));
			}
		}
	}
}

// Direct3D rendering method
#ifdef DIRECT_VERSION_9_MJH
HRESULT	CEffectiveWater::OneTimeSceneInit(const float& x, const float& y, const float& z, LPDIRECT3DTEXTURE9 pTex)
#else
HRESULT	CEffectiveWater::OneTimeSceneInit(const float& x, const float& y, const float& z, LPDIRECT3DTEXTURE8 pTex)
#endif // DIRECT_VERSION_9_MJH
{
	//BuildTable(10.333f, 5.1f);
	BuildTable(1.333f, 0.5f);
	m_dwFVF	= D3DFVF_PDT;
	D3DXMatrixIdentity(&m_matPos);
	D3DXMatrixIdentity(&m_matTexPos);
	m_matPos._41	= x;
	m_matPos._42	= y;
	m_matPos._43	= z;
	m_bProcess		= true;
	m_pTexture		= pTex;
	return	S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CEffectiveWater::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev)
#else
HRESULT	CEffectiveWater::InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev)
#endif // DIRECT_VERSION_9_MJH
{
	return	CRenderObj::InitDeviceObjects(pd3ddev);
}

HRESULT	CEffectiveWater::RestoreDeviceObjects(float afMin, float afMax)
{
	FVF_PDT	*	pVertices;
	WORD	*	pIndices;
	float		fHeight	= 0.0f, fMin = afMin, fMax = afMax;

	if ( FAILED ( g_RenderManager.CreateIndexBuffer(6*sizeof(WORD), D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_SYSTEMMEM, &m_pIB) ) )
		return	Error("Create IB");
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(4*sizeof(FVF_PDT), D3DUSAGE_DYNAMIC, m_dwFVF, D3DPOOL_SYSTEMMEM, &m_pVB) ) )
		return	Error("Create VB");
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0,0,(void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB->Lock(0,0,(BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock IB");
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0,0,(void**)&pVertices, 0) ) )
#else
	if ( FAILED ( m_pVB->Lock(0,0,(BYTE**)&pVertices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock VB");

	pVertices[0].v	= D3DXVECTOR3(fMin, fHeight, fMin);
	pVertices[0].t	= D3DXVECTOR2(0,1);
	pVertices[0].color	= 0xaaffffff;

	pVertices[1].v	= D3DXVECTOR3(fMin, fHeight, fMax);
	pVertices[1].t	= D3DXVECTOR2(0,0);
	pVertices[1].color	= 0xaaffffff;

	pVertices[2].v	= D3DXVECTOR3(fMax, fHeight, fMax);
	pVertices[2].t	= D3DXVECTOR2(1,0);
	pVertices[2].color	= 0xaaffffff;

	pVertices[3].v	= D3DXVECTOR3(fMax, fHeight, fMin);
	pVertices[3].t	= D3DXVECTOR2(1,1);
	pVertices[3].color	= 0xaaffffff;

	pIndices[0]	= 0;
	pIndices[1]	= 1;
	pIndices[2]	= 2;
	pIndices[3]	= 2;
	pIndices[4]	= 3;
	pIndices[5]	= 0;

	m_pIB->Unlock();
	m_pVB->Unlock();

	m_nVtxCount	= 4;
	m_nIdxCount	= 6;

//	char	szpath[128];
//	sprintf(szpath, "%s\\env\\watereffect.tga", g_szRoot);


	memset(m_nBuffer,  0, ETEXTURESIZE*ETEXTURESIZE*sizeof(int));
	memset(m_nBuffer2, 0, ETEXTURESIZE*ETEXTURESIZE*sizeof(int));

	m_pWaterActive = m_nBuffer;
	m_pWaterScratch = m_nBuffer2;

	
	return	S_OK;
}

HRESULT	CEffectiveWater::InvalidateDeviceObjects()
{
//	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pFinal);
	return	CRenderObj::InvalidateDeviceObjects();
}

HRESULT	CEffectiveWater::DeleteDeviceObjects()
{
	return	CRenderObj::DeleteDeviceObjects();
}

HRESULT	CEffectiveWater::FinalCleanup()
{
	return	S_OK;
}

HRESULT	CEffectiveWater::Render()
{
	if ( m_bProcess )
		g_RenderManager.SetTexture(0, m_pFinal);
	else
		g_RenderManager.SetTexture(0, m_pTexture);

	switch ( m_byRunsDir )
	{
	case	RD_LEFT:
	case	RD_LEFTUP:
	case	RD_LEFTDOWN:
		m_matTexPos._31	= (m_matTexPos._31<=0.0f) ? 1.0f : m_matTexPos._31 - 0.01f;
		break;
	case	RD_RIGHT:
	case	RD_RIGHTUP:
	case	RD_RIGHTDOWN:
		m_matTexPos._31	= (m_matTexPos._31>=1.0f) ? 0.0f : m_matTexPos._31 + 0.01f;
		break;
	case	RD_NONE:
	case	RD_UP:
	case	RD_DOWN:
		m_matTexPos._31	= 0.0f;
		break;
	}
	// process y axis
	switch ( m_byRunsDir )
	{
	case	RD_UP:
	case	RD_LEFTUP:
	case	RD_RIGHTUP:
		m_matTexPos._32	= (m_matTexPos._32>=1.0f) ? 0.0f : m_matTexPos._32 + 0.01f;
		break;
	case	RD_DOWN:
	case	RD_LEFTDOWN:
	case	RD_RIGHTDOWN:
		m_matTexPos._32	= (m_matTexPos._32<=0.0f) ? 1.0f : m_matTexPos._32 - 0.01f;
		break;
	case	RD_NONE:
	case	RD_LEFT:
	case	RD_RIGHT:
		m_matTexPos._32	= 0.0f;
		break;
	}

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT2 );

	g_RenderManager.SetTransform(D3DTS_TEXTURE0, &m_matTexPos);
	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matPos);
	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(FVF_PDT));
	g_RenderManager.SetFVF(m_dwFVF);
	g_RenderManager.SetIndices(m_pIB, 0);
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nVtxCount, 0, m_nIdxCount/3);

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
	return	S_OK;
}

HRESULT	CEffectiveWater::FrameMove()
{
	Process();
	BuildFinal();

	int *temp = m_pWaterActive;
	m_pWaterActive = m_pWaterScratch;
	m_pWaterScratch = temp;

	return	S_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Waterfall Routines

CBravoWaterfall::CBravoWaterfall()
{
	m_pTexture	= NULL;
	m_bEnable	= false;
	m_byCurrentFall	= 0;
	m_dwPreCount	= timeGetTime();
}

CBravoWaterfall::~CBravoWaterfall()
{
	FinalCleanup();
}
#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoWaterfall::OneTimeSceneInit(LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DTEXTURE9* pFallTex)
#else
HRESULT	CBravoWaterfall::OneTimeSceneInit(LPDIRECT3DTEXTURE8 pTex, LPDIRECT3DTEXTURE8* pFallTex)
#endif // DIRECT_VERSION_9_MJH
{
	m_pTexture	= pTex;
	m_pFallTexture	= pFallTex;
	D3DXMatrixIdentity(&m_matPos);
	D3DXMatrixIdentity(&m_matTexPos);
	m_dwFVF	= D3DFVF_PDT;
//	m_Water.OneTimeSceneInit(m_matPos._41,m_matPos._42,m_matPos._43, pFallTex);
	return	S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoWaterfall::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev)
#else
HRESULT	CBravoWaterfall::InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev)
#endif // DIRECT_VERSION_9_MJH
{
//	m_Water.InitDeviceObjects(pd3ddev);
	return	CRenderObj::InitDeviceObjects(pd3ddev);
}

const float	WATERFALL_YLIST[5]		= {100,90,70,40,0};
const float	WATERFALL_ZLIST[4][5]	= {{50,45,40,35,35},{50,40,35,30,30},{50,40,35,30,30},{50,45,40,35,35}};
const float	WATERFALL_WIDTH			= 10;
const float	WATERFALL_U[4]			= {0.1f,0.4f,0.6f,0.9f};
const WORD	WATERFALL_IDX[6]		= {4,0,1,1,5,4};

HRESULT	CBravoWaterfall::RestoreDeviceObjects()
{
	FVF_PDT	*	pVertices;
	WORD	*	pIndices;

	if ( FAILED ( g_RenderManager.CreateIndexBuffer(78*sizeof(WORD), D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_SYSTEMMEM, &m_pIB) ) )
		return	Error("Create IB");
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(24*sizeof(FVF_PDT), D3DUSAGE_DYNAMIC, m_dwFVF, D3DPOOL_SYSTEMMEM, &m_pVB) ) )
		return	Error("Create VB");
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0,0,(void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB->Lock(0,0,(BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock IB");
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0,0,(void**)&pVertices, 0) ) )
#else
	if ( FAILED ( m_pVB->Lock(0,0,(BYTE**)&pVertices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock VB");

	int	x, y;
	for ( int i = 0; i < 20; i ++ )
	{
		x	= i%4;
		y	= i/4;

		pVertices[i].v	= D3DXVECTOR3((10+x*WATERFALL_WIDTH)-25, WATERFALL_YLIST[y], WATERFALL_ZLIST[x][y]-25);
		pVertices[i].t	= D3DXVECTOR2(WATERFALL_U[x], y*0.25f);
		pVertices[i].color	= D3DCOLOR_ARGB(200,255,255,255);//0x55ffffff;
	}

	for ( y = 0; y < 4; y ++ )
	{
		for ( x = 0; x < 3; x ++ )
		{
			pIndices[(((y*3)+x)*6)+0]	= WATERFALL_IDX[0]+((y*4)+x);
			pIndices[(((y*3)+x)*6)+1]	= WATERFALL_IDX[1]+((y*4)+x);
			pIndices[(((y*3)+x)*6)+2]	= WATERFALL_IDX[2]+((y*4)+x);
			pIndices[(((y*3)+x)*6)+3]	= WATERFALL_IDX[3]+((y*4)+x);
			pIndices[(((y*3)+x)*6)+4]	= WATERFALL_IDX[4]+((y*4)+x);
			pIndices[(((y*3)+x)*6)+5]	= WATERFALL_IDX[5]+((y*4)+x);
		}
	}

	m_nVtxCount	= 20;
	m_nIdxCount	= 72;

	pVertices[20].v		= D3DXVECTOR3(-35, 0, -35);
	pVertices[20].color	= D3DCOLOR_ARGB(200,255,255,255);
	pVertices[20].t		= D3DXVECTOR2(0,1);
	pVertices[21].v		= D3DXVECTOR3(-35, 0, 35);
	pVertices[21].color	= D3DCOLOR_ARGB(200,255,255,255);
	pVertices[21].t		= D3DXVECTOR2(0,0);
	pVertices[22].v		= D3DXVECTOR3(35, 0, 35);
	pVertices[22].color	= D3DCOLOR_ARGB(200,255,255,255);
	pVertices[22].t		= D3DXVECTOR2(1,0);
	pVertices[23].v		= D3DXVECTOR3(35, 0, -35);
	pVertices[23].color	= D3DCOLOR_ARGB(200,255,255,255);
	pVertices[23].t		= D3DXVECTOR2(1,1);

	pIndices[72]	= 20;
	pIndices[73]	= 21;
	pIndices[74]	= 22;
	pIndices[75]	= 22;
	pIndices[76]	= 23;
	pIndices[77]	= 20;

	m_pIB->Unlock();
	m_pVB->Unlock();

//	m_Water.RestoreDeviceObjects(-10-25, 60-25);
	return	S_OK;
}

HRESULT	CBravoWaterfall::InvalidateDeviceObjects()
{
//	m_Water.InvalidateDeviceObjects();
//	SAFE_RELEASE(m_pTexture);
	return	CRenderObj::InvalidateDeviceObjects();
}

HRESULT	CBravoWaterfall::DeleteDeviceObjects()
{
//	m_Water.DeleteDeviceObjects();
	return	CRenderObj::DeleteDeviceObjects();
}

// external call
HRESULT	CBravoWaterfall::FinalCleanup()
{
//	m_Water.FinalCleanup();
	InvalidateDeviceObjects();
	DeleteDeviceObjects();
	m_bEnable	= false;
	return	S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoWaterfall::Build(LPDIRECT3DDEVICE9 pd3ddev, LPDIRECT3DTEXTURE9 pTex, LPDIRECT3DTEXTURE9* pFallTex, const unsigned short& idx)
#else
HRESULT	CBravoWaterfall::Build(LPDIRECT3DDEVICE8 pd3ddev, LPDIRECT3DTEXTURE8 pTex, LPDIRECT3DTEXTURE8* pFallTex, const unsigned short& idx)
#endif // DIRECT_VERSION_9_MJH
{
	m_nTexIndex	= idx;
	OneTimeSceneInit(pTex, pFallTex);
	InitDeviceObjects(pd3ddev);
	RestoreDeviceObjects();
	m_bEnable	= true;
	return	S_OK;
}

HRESULT	CBravoWaterfall::Render()
{
	if ( m_pd3dDevice == NULL )
		return	S_OK;

	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0x00000000 );
	g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matPos);
	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(FVF_PDT));
	g_RenderManager.SetFVF(m_dwFVF);
	g_RenderManager.SetIndices(m_pIB, 0);

	g_RenderManager.SetTexture(0, m_pFallTexture[m_byCurrentFall]);
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 20, 4, 72, 2);

//	m_Water.SetPosTM(m_matPos);
//	m_Water.Render();

	g_RenderManager.SetTexture(0, m_pTexture);
	g_RenderManager.SetTransform( D3DTS_TEXTURE0, &m_matTexPos );

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT2 );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nVtxCount, 0, m_nIdxCount/3);

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
	
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetAlphaTestEnable( FALSE );

	return	S_OK;
}

HRESULT	CBravoWaterfall::FrameMove()
{
	if ( m_bEnable )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_dwPreCount ) , > , 50 ) )
		{
			m_byCurrentFall	= (m_byCurrentFall>=7) ? 0 : m_byCurrentFall+1;
//			m_Water.BuildDroplet((rand()%(ETEXTURESIZE/4))+(ETEXTURESIZE/2),(rand()%(ETEXTURESIZE/4))+(ETEXTURESIZE/2), 5, 10);
//			m_Water.FrameMove();
			m_dwPreCount	= timeGetTime();
		}

		m_matTexPos._32	= -(float)(timeGetTime()/1500.0f);
	}

	return	S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	CBravoEffectiveWater class

CBravoEffectiveWater::~CBravoEffectiveWater()
{
	FinalCleanup();
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoEffectiveWater::Build(const int& mapsize, LPDIRECT3DDEVICE9 pd3ddev, CBravoCamera* pCamera, LPDIRECT3DTEXTURE9 pTex)
#else
HRESULT	CBravoEffectiveWater::Build(const int& mapsize, LPDIRECT3DDEVICE8 pd3ddev, CBravoCamera* pCamera, LPDIRECT3DTEXTURE8 pTex)
#endif // DIRECT_VERSION_9_MJH
{
	m_nSplit		= mapsize / (BEWATERSIZE/16);
	m_pWater		= SAFE_NEW_ARRAY( CEffectiveWater , m_nSplit*m_nSplit );	
	m_dwPreCount	= timeGetTime();

	D3DXMatrixIdentity(&m_matPos);

	OneTimeSceneInit(pTex);
	InitDeviceObjects(pd3ddev);
	RestoreDeviceObjects();
	m_pCamera	= pCamera;
	return	S_OK;
}

HRESULT	CBravoEffectiveWater::FinalCleanup()
{
	InvalidateDeviceObjects();
	DeleteDeviceObjects();
	SAFE_DELETE_ARRAY(m_pWater);

	for ( int i = 0; i < m_nSplit*m_nSplit; i ++ )
		m_pWater[i].FinalCleanup();
	return	S_OK;
}
#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoEffectiveWater::ChangeTexture(LPDIRECT3DTEXTURE9 pTex)
#else
HRESULT	CBravoEffectiveWater::ChangeTexture(LPDIRECT3DTEXTURE8 pTex)
#endif // DIRECT_VERSION_9_MJH
{
	if( pTex )
	{
		for( int i = 0; i < m_nSplit*m_nSplit; i ++ )
		{
			m_pWater[i].SetBaseTexture(pTex);
		}
		
	}
	return	S_OK;
}

void	CBravoEffectiveWater::SetRunsDir(BYTE runsdir)
{
	m_byRunsDir	= runsdir;

	for( int i = 0; i < m_nSplit*m_nSplit; i ++ )
	{
		m_pWater[i].m_byRunsDir	= runsdir;
	}
}

//	Direct3D 
#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoEffectiveWater::OneTimeSceneInit(LPDIRECT3DTEXTURE9 pTex)
#else
HRESULT	CBravoEffectiveWater::OneTimeSceneInit(LPDIRECT3DTEXTURE8 pTex)
#endif // DIRECT_VERSION_9_MJH
{
	CEffectiveWater	*	pWater;
	int	x, y;
	for ( int i = 0; i < m_nSplit*m_nSplit; i ++ )
	{
		x	= i%m_nSplit;
		y	= i/m_nSplit;
		pWater	= &m_pWater[i];
		pWater->OneTimeSceneInit((float)(x*BEWATERSIZE), 0, (float)(y*BEWATERSIZE), pTex);
	}
	m_nCY			= 0;
	m_nCX			= 0;

	return	S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoEffectiveWater::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev)
#else
HRESULT	CBravoEffectiveWater::InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev)
#endif // DIRECT_VERSION_9_MJH
{
	m_pd3dDevice	= pd3ddev;
	for ( int i = 0; i < m_nSplit*m_nSplit; i ++ )
		m_pWater[i].InitDeviceObjects(pd3ddev);
	return	S_OK;
}

HRESULT	CBravoEffectiveWater::RestoreDeviceObjects()
{
	for ( int i = 0; i < m_nSplit*m_nSplit; i ++ )
		m_pWater[i].RestoreDeviceObjects();
	return	S_OK;
}

HRESULT	CBravoEffectiveWater::InvalidateDeviceObjects()
{
	for ( int i = 0; i < m_nSplit*m_nSplit; i ++ )
		m_pWater[i].InvalidateDeviceObjects();
	return	S_OK;
}

HRESULT	CBravoEffectiveWater::DeleteDeviceObjects()
{
	for ( int i = 0; i < m_nSplit*m_nSplit; i ++ )
		m_pWater[i].DeleteDeviceObjects();
	return	S_OK;
}

HRESULT	CBravoEffectiveWater::Render()
{
	if ( m_pd3dDevice == NULL )
		return	S_OK;

	CEffectiveWater	*	pWater;

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matPos);
	for ( int i = 0; i < m_nSplit*m_nSplit; i ++ )
	{
		pWater	= &m_pWater[i];
		pWater->SetHeight(m_matPos._42);
		if ( pWater->IsVisible() )
			pWater->Render();
	}
	return	S_OK;
}

HRESULT	CBravoEffectiveWater::FrameMove()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_dwPreCount ) , <= , 40 ) )
		return	S_OK;

	m_dwPreCount	= timeGetTime();

	CEffectiveWater	*	pWater;
	int	x, y;
	
	for ( int i = 0; i < m_nSplit*m_nSplit; i ++ )
	{
		m_pWater[i].SetProcess(false);
		m_pWater[i].SetVisible(false);
		x	= i%m_nSplit;
		y	= i/m_nSplit;
		if ( true == m_pCamera->CubeInFrustum((float)((x*BEWATERSIZE)+(BEWATERSIZE/2)), 0, (float)((y*BEWATERSIZE)+(BEWATERSIZE/2)), (float)BEWATERSIZE) )
			m_pWater[i].SetVisible(true);
	}

	for ( y = m_nCY-1; y < m_nCY+2; y ++ )
	{
		for ( x = m_nCX-1; x < m_nCX+2; x ++ )
		{
			if ( y >= m_nSplit || y < 0 || x >= m_nSplit || x < 0 )
				continue;
			pWater	= &m_pWater[(y*m_nSplit)+x];
			if ( pWater->IsVisible() )
			{
				pWater->FrameMove();
				pWater->SetVisible(true);
				pWater->SetProcess(true);
			}
		}
	}
	return	S_OK;
}

HRESULT	CBravoEffectiveWater::PlayWave(const float& x, const float& y)
{
	int	nEleX	= (int)(x/BEWATERSIZE);//>>6
	int	nEleY	= (int)(y/BEWATERSIZE);//>>6
	int	nPosX	= (int)x;
	int	nPosY	= (int)y;

	int	nWaterPosX	= nPosX%BEWATERSIZE;
	int	nWaterPosY	= (int)(((m_nSplit*(BEWATERSIZE/16))*16.0f)-nPosY)%BEWATERSIZE;
	int	nWaterSize	= rand()%2 + 2; // 5-15
	int	nWaterStrength	= rand()%3 + 4; // 10-25
	m_pWater[(nEleY*m_nSplit)+nEleX].BuildDroplet(nWaterPosX/2,nWaterPosY/2,nWaterSize, nWaterStrength);
	return	S_OK;
}

HRESULT	CBravoEffectiveWater::PlayWave(const D3DXMATRIX& matPos)
{
	int	nEleX	= (int)(matPos._41/BEWATERSIZE);//>>6
	int	nEleY	= (int)(matPos._43/BEWATERSIZE);//>>6
	int	nPosX	= (int)matPos._41;
	int	nPosY	= (int)matPos._43;

	int	nWaterPosX	= nPosX%BEWATERSIZE;
	int	nWaterPosY	= (int)(((m_nSplit*(BEWATERSIZE/16))*16.0f)-nPosY)%BEWATERSIZE;
	int	nWaterSize	= rand()%3 + 2; // 5-15
	int	nWaterStrength	= rand()%4 + 3; // 10-25
	m_pWater[(nEleY*m_nSplit)+nEleX].BuildDroplet(nWaterPosX/2,nWaterPosY/2,nWaterSize, nWaterStrength);
	return	S_OK;
}


HRESULT	CBravoEffectiveWater::UpdateCenter(const D3DXMATRIX& matPos)
{
	m_nCX	= (int)(matPos._41/BEWATERSIZE);
	m_nCY	= (int)(matPos._43/BEWATERSIZE);
	return	S_OK;
}

HRESULT	CBravoEffectiveWater::UpdateCenter(const float& x, const float& y)
{
	m_nCX	= (int)(x/BEWATERSIZE);
	m_nCY	= (int)(y/BEWATERSIZE);
	return	S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Water wave

CWaterWave::CWaterWave()
{
	m_bIsPlaying	= false;
}

HRESULT		CWaterWave::OneTimeSceneInit()
{
	m_dwFVF	= D3DFVF_PT;
	D3DXMatrixIdentity(&m_matPos);
	m_dwPreCount	= 0;
	m_fScale		= 1;
	return	S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT		CWaterWave::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev)
#else
HRESULT		CWaterWave::InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev)
#endif // DIRECT_VERSION_9_MJH
{
	return	CRenderObj::InitDeviceObjects(pd3ddev);
}

HRESULT		CWaterWave::RestoreDeviceObjects()
{
	FVF_PT	*	pVertices;
	WORD	*	pIndices;
	float		fHeight	= 10.0f, fMin = -1.0f, fMax = 1.0f;

	if ( FAILED ( g_RenderManager.CreateIndexBuffer(6*sizeof(WORD), D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_SYSTEMMEM, &m_pIB) ) )
		return	Error("Create IB");
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(4*sizeof(FVF_PT), D3DUSAGE_DYNAMIC, m_dwFVF, D3DPOOL_SYSTEMMEM, &m_pVB) ) )
		return	Error("Create VB");
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0,0,(void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB->Lock(0,0,(BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock IB");
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0,0,(void**)&pVertices, 0) ) )
#else
	if ( FAILED ( m_pVB->Lock(0,0,(BYTE**)&pVertices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock VB");

	pVertices[0].x	= fMin;
	pVertices[0].y	= 0;
	pVertices[0].z	= fMin;
	pVertices[0].tu	= 0;
	pVertices[0].tv	= 1;

	pVertices[1].x	= fMin;
	pVertices[1].y	= 0;
	pVertices[1].z	= fMax;
	pVertices[1].tu	= 0;
	pVertices[1].tv	= 0;

	pVertices[2].x	= fMax;
	pVertices[2].y	= 0;
	pVertices[2].z	= fMax;
	pVertices[2].tu	= 1;
	pVertices[2].tv	= 0;

	pVertices[3].x	= fMax;
	pVertices[3].y	= 0;
	pVertices[3].z	= fMin;
	pVertices[3].tu	= 1;
	pVertices[3].tv	= 1;

/*	pVertices[0].v	= D3DXVECTOR3(fMin, fHeight, fMin);
	pVertices[0].t	= D3DXVECTOR2(0,1);

	pVertices[1].v	= D3DXVECTOR3(fMin, fHeight, fMax);
	pVertices[1].t	= D3DXVECTOR2(0,0);

	pVertices[2].v	= D3DXVECTOR3(fMax, fHeight, fMax);
	pVertices[2].t	= D3DXVECTOR2(1,0);

	pVertices[3].v	= D3DXVECTOR3(fMax, fHeight, fMin);
	pVertices[3].t	= D3DXVECTOR2(1,1);
*/
	pIndices[0]	= 0;
	pIndices[1]	= 1;
	pIndices[2]	= 2;
	pIndices[3]	= 2;
	pIndices[4]	= 3;
	pIndices[5]	= 0;

	m_pIB->Unlock();
	m_pVB->Unlock();

	m_nVtxCount	= 4;
	m_nIdxCount	= 6;

	m_Material.Diffuse.r = m_Material.Diffuse.g = m_Material.Diffuse.b = m_Material.Diffuse.a = 
	m_Material.Ambient.r = m_Material.Ambient.g = m_Material.Ambient.b = m_Material.Ambient.a = 1.0f;
	m_Material.Emissive.r = m_Material.Emissive.g = m_Material.Emissive.b = m_Material.Emissive.a = 
	m_Material.Specular.r = m_Material.Specular.g = m_Material.Specular.b = m_Material.Specular.a = 1.0f;
	m_Material.Power	= -10;

	return	S_OK;
}

HRESULT		CWaterWave::InvalidateDeviceObjects()
{
	return	CRenderObj::InvalidateDeviceObjects();
}

HRESULT		CWaterWave::DeleteDeviceObjects()
{
	return	CRenderObj::DeleteDeviceObjects();
}

HRESULT		CWaterWave::Render()
{
	if ( !m_pd3dDevice || m_bIsPlaying == false )
		return	S_OK;

	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0x00000000 );
	g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );

	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE);
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE);
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_DIFFUSE);


	g_RenderManager.SetMaterial(&m_Material);
	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matPos);
	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(FVF_PT));
	g_RenderManager.SetFVF(m_dwFVF);
	g_RenderManager.SetIndices(m_pIB, 0);
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nVtxCount, 0, m_nIdxCount/3);

	return	g_RenderManager.SetAlphaBlendEnable(  FALSE );;
}

HRESULT		CWaterWave::FrameMove()
{
	if ( !m_bIsPlaying )
		return	E_FAIL;

	if ( m_fScale == 10.0f )
	{
		m_bIsPlaying	= false;
	}
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() ,m_dwPreCount ) , > , 20 ) )
	{
		m_fScale	+= 0.25;
		m_Material.Diffuse.a	= (m_Material.Diffuse.a<=0) ? 0.0f : m_Material.Diffuse.a - 0.025f;
		D3DXMatrixScaling(&m_matPos, m_fScale, 1.0f, m_fScale);
		m_matPos._41	= m_fX;
		m_matPos._42	= m_fY;
		m_matPos._43	= m_fZ;
		m_dwPreCount	= timeGetTime();
	}
	return	S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT		CWaterWave::Build(IDirect3DDevice9* pDev)
#else
HRESULT		CWaterWave::Build(IDirect3DDevice8* pDev)
#endif // DIRECT_VERSION_9_MJH
{
	OneTimeSceneInit();
	InitDeviceObjects(pDev);
	RestoreDeviceObjects();
	return	S_OK;
}

HRESULT		CWaterWave::FinalCleanup()
{
	InvalidateDeviceObjects();
	DeleteDeviceObjects();
	return	S_OK;
}

void		CWaterWave::SetPosTM(const float& x, const float& y, const float& z)
{
	m_fX	= x;
	m_fY	= y;
	m_fZ	= z;
}

void		CWaterWave::SetPlayEnable()
{
	m_bIsPlaying	= true;
	m_fScale		= 1.0f;
	m_Material.Diffuse.a	= 1.0f;
}

void		CSimpleWave::SetPlayEnable()
{
	m_Wave[0].SetPlayEnable();
	m_Wave[1].SetPlayEnable();
	m_Wave[2].SetPlayEnable();
	m_dwPreCount	= timeGetTime();
	m_byPlayStep	= 0;
}

unsigned short		WAVE_INTERVAL[4]	= {10,300,350,350};

HRESULT		CSimpleWave::PlayWave()
{
	switch ( m_byPlayStep )
	{
	case	1:
		m_Wave[0].FrameMove();
		m_Wave[0].Render();
		break;
	case	2:
		m_Wave[0].FrameMove();
		m_Wave[0].Render();
		m_Wave[1].FrameMove();
		m_Wave[1].Render();
		break;
	case	3:
		m_Wave[0].FrameMove();
		m_Wave[0].Render();
		m_Wave[1].FrameMove();
		m_Wave[1].Render();
		m_Wave[2].FrameMove();
		m_Wave[2].Render();
		break;
	}

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_dwPreCount ) , >= , WAVE_INTERVAL[m_byPlayStep] ) )
	{
		m_byPlayStep	= (m_byPlayStep>=3) ? 3 : m_byPlayStep += 1;
		m_dwPreCount	= timeGetTime();
	}

	return	S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT		CSimpleWave::Build(LPDIRECT3DDEVICE9 pd3dDev)
#else
HRESULT		CSimpleWave::Build(LPDIRECT3DDEVICE8 pd3dDev)
#endif // DIRECT_VERSION_9_MJH
{
	m_Wave[0].Build(pd3dDev);
	m_Wave[1].Build(pd3dDev);
	m_Wave[2].Build(pd3dDev);
	m_dwPreCount	= timeGetTime();
	m_byPlayStep	= 0;
	return	S_OK;
}

HRESULT		CSimpleWave::FinalCleanup()
{
	m_Wave[0].FinalCleanup();
	m_Wave[1].FinalCleanup();
	m_Wave[2].FinalCleanup();
	return	S_OK;
}

void		CSimpleWave::SetPosition(const float& x, const float& y, const float& z)
{
	m_Wave[0].SetPosTM(x,y,z);
	m_Wave[1].SetPosTM(x,y,z);
	m_Wave[2].SetPosTM(x,y,z);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Wave List

CWaveList::~CWaveList()
{
	FinalCleanup();
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT		CWaveList::BuildAllUnits(LPDIRECT3DDEVICE9 pd3dDev)
#else
HRESULT		CWaveList::BuildAllUnits(LPDIRECT3DDEVICE8 pd3dDev)
#endif // DIRECT_VERSION_9_MJH
{
	for ( int i = 0; i < MAX_WATERWAVE; i ++ )
		m_List[i].Build(pd3dDev);

	return	S_OK;
}

HRESULT		CWaveList::PlayAll()
{
	for ( int i = 0; i < MAX_WATERWAVE; i ++ )
	{
		if ( m_List[i].GetLastWave().IsPlaying() )
			m_List[i].PlayWave();
	}
	return	S_OK;
}

HRESULT		CWaveList::AddWave(const D3DXMATRIX& matPos)
{
	int i = 0;
	for ( i = 0; i < MAX_WATERWAVE; i ++ )
	{
		if ( !m_List[i].GetLastWave().IsPlaying() )
			break;
	}
	if ( i >= MAX_WATERWAVE )
		return	E_FAIL;
	m_List[i].SetPlayEnable();
	return	S_OK;
}

HRESULT		CWaveList::AddWave(const float& x, const float& y, const float& z)
{
	int i = 0;
	for ( i = 0; i < MAX_WATERWAVE; i ++ )
	{
		if ( !m_List[i].GetLastWave().IsPlaying() )
			break;
	}

	if ( i >= MAX_WATERWAVE )
		return	E_FAIL;
	m_List[i].SetPlayEnable();
	m_List[i].SetPosition(x,y,z);
	return	S_OK;
}

HRESULT		CWaveList::FinalCleanup()
{
	for ( int i = 0; i < MAX_WATERWAVE; i ++ )
		m_List[i].FinalCleanup();
	return	S_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 스카이 피라미드

D3DXVECTOR3		SP[8]	= 
{
	D3DXVECTOR3(-1,-1,-1),
	D3DXVECTOR3(-1,-1,1),
	D3DXVECTOR3(1,-1,1),
	D3DXVECTOR3(1,-1,-1),
	D3DXVECTOR3(-0.5,1,-0.5),
	D3DXVECTOR3(-0.5,1,0.5),
	D3DXVECTOR3(0.5,1,0.5),
	D3DXVECTOR3(0.5,1,-0.5),
};

D3DXVECTOR2		SUV[4]	= 
{
	D3DXVECTOR2(0,1),
	D3DXVECTOR2(0,0),
	D3DXVECTOR2(1,0),
	D3DXVECTOR2(1,1),
};

BYTE			SFACEIDX[5][4]	= 
{
	{0,4,5,1},
	{1,5,6,2},
	{2,6,7,3},
	{3,7,4,0},
};

BYTE			SFACEBASIC[24]	= {0,1,2,2,3,0,4,5,6,6,7,4,8,9,10,10,11,8,12,13,14,14,15,12};

HRESULT		CCubeSky::OneTimeSceneInit()
{
	m_dwFVF		= D3DFVF_PDT;
	m_fRadius	= 8000.0f;	/// by sooree
	m_fLow		= 1000.0f;
	D3DXMatrixIdentity(&m_matPos);
	return S_OK;
}

HRESULT		CCubeSky::RestoreDeviceObjects()
{
	if ( FAILED ( g_RenderManager.CreateIndexBuffer(24*sizeof(WORD), D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_SYSTEMMEM, &m_pIB) ) )
		return	Error("Create IB");
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(16*sizeof(FVF_PDT), D3DUSAGE_DYNAMIC, m_dwFVF, D3DPOOL_SYSTEMMEM, &m_pVB) ) )
		return	Error("Create VB");

	FVF_PDT	*	pVertices;
	WORD	*	pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0,0,(void**)&pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB->Lock(0,0,(BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock IB");
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0,0,(void**)&pVertices, 0) ) )
#else
	if ( FAILED ( m_pVB->Lock(0,0,(BYTE**)&pVertices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock VB");
	int y = 0;
	for ( y = 0; y < 4; y ++ )
	{
		for ( int x = 0; x < 4; x ++ )
		{
			pVertices[(y*4)+x].v		= SP[SFACEIDX[y][x]] * m_fRadius;
			pVertices[(y*4)+x].color	= D3DCOLOR_XRGB(255,255,255);
		}
	}
	pVertices[0].t	= D3DXVECTOR2(0.0f,1.0f);
	pVertices[1].t	= D3DXVECTOR2(0.0f,0.0f);
	pVertices[2].t	= D3DXVECTOR2(0.25f,0.0f);
	pVertices[3].t	= D3DXVECTOR2(0.25f,1.0f);

	pVertices[4].t	= D3DXVECTOR2(0.25f,1.0f);
	pVertices[5].t	= D3DXVECTOR2(0.25f,0.0f);
	pVertices[6].t	= D3DXVECTOR2(0.5f,0.0f);
	pVertices[7].t	= D3DXVECTOR2(0.5f,1.0f);

	pVertices[8].t	= D3DXVECTOR2(0.5f,1.0f);
	pVertices[9].t	= D3DXVECTOR2(0.5f,0.0f);
	pVertices[10].t	= D3DXVECTOR2(0.75f,0.0f);
	pVertices[11].t	= D3DXVECTOR2(0.75f,1.0f);

	pVertices[12].t	= D3DXVECTOR2(0.75f,1.0f);
	pVertices[13].t	= D3DXVECTOR2(0.75f,0.0f);
	pVertices[14].t	= D3DXVECTOR2(1.0f,0.0f);
	pVertices[15].t	= D3DXVECTOR2(1.0f,1.0f);

	for ( y = 0; y < 24; y ++ )
		pIndices[y]	= (unsigned short)(SFACEBASIC[y]);

	m_pIB->Unlock();
	m_pVB->Unlock();

	m_nVtxCount	= 16;
	m_nIdxCount	= 24;

	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT		CCubeSky::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDev)
#else
HRESULT		CCubeSky::InitDeviceObjects(LPDIRECT3DDEVICE8 pd3dDev)
#endif // DIRECT_VERSION_9_MJH
{
	return CRenderObj::InitDeviceObjects(pd3dDev);
}

HRESULT		CCubeSky::InvalidateDeviceObjects()
{
	return CRenderObj::InvalidateDeviceObjects();
}

HRESULT		CCubeSky::DeleteDeviceObjects()
{
	return CRenderObj::DeleteDeviceObjects();
}

HRESULT		CCubeSky::FinalCleanup()
{
	InvalidateDeviceObjects();
	DeleteDeviceObjects();
	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT		CCubeSky::Render(LPDIRECT3DTEXTURE9 pd3dTex)
#else
HRESULT		CCubeSky::Render(LPDIRECT3DTEXTURE8 pd3dTex)
#endif // DIRECT_VERSION_9_MJH
{
	m_matPos._41	= g_Camera.m_vEye.x;
	m_matPos._42	= g_Camera.m_vEye.y + 1000.0f;
	m_matPos._43	= g_Camera.m_vEye.z;

	g_RenderManager.SetTexture(0,pd3dTex);
	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matPos);
	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(FVF_PDT));
	g_RenderManager.SetFVF(m_dwFVF);
	g_RenderManager.SetIndices(m_pIB, 0);
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nVtxCount, 0, m_nIdxCount/3);
	return S_OK;
}

HRESULT		CCubeSky::FrameMove()
{
	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT		CCubeSky::Build(LPDIRECT3DDEVICE9 pd3dDev)
#else
HRESULT		CCubeSky::Build(LPDIRECT3DDEVICE8 pd3dDev)
#endif // DIRECT_VERSION_9_MJH
{
	OneTimeSceneInit();
	InitDeviceObjects(pd3dDev);
	RestoreDeviceObjects();
	return S_OK;
}