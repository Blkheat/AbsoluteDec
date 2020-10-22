               #include "RockPCH.h"

#include	"..\\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<BASE\\d3dapp.h>
#include	<BASE\\d3dfont.h>
//#include	<BASE\\Fps.h>
#include	"Field.h"
#include	<..\\Camera.h>
#include    "..\\pc.H"
#include    "..\\Effect.H"
#include	"..\\RockClient.h"
#include	"..\\..\\CRenderManager.h"
//#include	"..\\Weather_Manager.H"

//extern	char	g_szRoot[128];

extern	long	g_nowTime;

#define	MAX_COLOR			230
#define	CHANGECOLOR_TIME	1000

float	CField::m_fBaseHeightWater;
BYTE	CField::m_byHighestWaterPlus;

typedef	FVF_BRAVOCLOUD	FVF_BRAVOSKY;
#define	D3DFVF_BRAVOSKY	(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define	SKYX 0.0625f
#define SKYY 1.0625f

CBravoSky::CBravoSky()
{
	m_pVB	= NULL;
	m_pIB	= NULL;
	D3DXMatrixIdentity(&m_matWorld);
}

CBravoSky::~CBravoSky()
{
	FinalCleanup();
}

void	CBravoSky::FinalCleanup()
{
	if ( m_pVB )
	{
		m_pVB->Release();
		m_pVB	= NULL;
	}
	if ( m_pIB )
	{
		m_pIB->Release();
		m_pIB	= NULL;
	}
	D3DXMatrixIdentity(&m_matWorld);
}

#ifdef DIRECT_VERSION_9_MJH
bool	CBravoSky::Create(LPDIRECT3DDEVICE9 pDevice, const short& size)
#else
bool	CBravoSky::Create(LPDIRECT3DDEVICE8 pDevice, const short& size)
#endif // DIRECT_VERSION_9_MJH
{
	m_nSize	= size;

	if ( FAILED ( g_RenderManager.CreateIndexBuffer(36*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB) ) )
	{
///		MessageBox(NULL, "index buffer failed", "bravo water", MB_OK);
		return false;
	}
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(24*sizeof(FVF_BRAVOSKY), D3DUSAGE_WRITEONLY, D3DFVF_BRAVOSKY, D3DPOOL_MANAGED, &m_pVB) ) )
	{
///		MessageBox(NULL, "vertex buffer failed", "bravo water", MB_OK);
		return false;
	}

//	m_nHighest	= (m_nSize<<3);// + m_nSize;
//	m_nHighest	= (m_nSize*12)-300.0f;// + m_nSize;
//	m_nLowest	= -(m_nSize*4)-300.0f;

/*	if ( m_nSize <= 64 )
		m_nHighest	= m_nSize*10;
	else
		m_nHighest	= m_nSize*6;
	m_nLowest	= -(m_nSize*4);
*/
	m_nLowest	= -500;
	m_nHighest	= m_nLowest	+ 2000;//(m_nSize<<2) + m_nSize;

	return	true;
}

bool	CBravoSky::UpdateColor()
{
	short	nvtxcount	= 0;
	short	nidxcount	= 0;
	FVF_BRAVOSKY*	pVertices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertices, 0 ) ) )
#else
	if ( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock VertexBuffer", "Bravo Cloud", MB_OK);
		return false;
	}
	
	if( g_Map.m_bChangeColor == true )
	{		
		// front	
		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		// right
		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		// back
		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		// left
		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		// bottom
		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		// top
		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

		pVertices[nvtxcount].color	= g_Map.m_vertex_color;		
		nvtxcount++;

	}
	else
	{
		// front
		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);	
		nvtxcount++;
		
		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;
		
		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;
		

		// right
		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		// back
		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		// left
		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		// bottom
		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		// top
		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;

		pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
		nvtxcount++;
	}

	m_pVB->Unlock();
	return true;
}

bool	CBravoSky::Update()
{
	short	nvtxcount	= 0;
	short	nidxcount	= 0;
	FVF_BRAVOSKY	*	pVertices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertices, 0 ) ) )
#else
	if ( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock VertexBuffer", "Bravo Cloud", MB_OK);
		return false;
	}
	WORD *	pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0, 0, (void**)&pIndices, 0 ) ) )
#else
	if ( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&pIndices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock IndexBuffer", "Bravo Cloud", MB_OK);
		return false;
	}

	const double	txcut	= 0.001f;
	const float		radius	= 1000.0f; // m_nSize * 16

	// front
	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
	pVertices[nvtxcount].u		= 0;
	pVertices[nvtxcount].v		= 1-txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0;
	pVertices[nvtxcount].v		= 0+txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.125;
	pVertices[nvtxcount].v		= 0+txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.125;
	pVertices[nvtxcount].v		= 1-txcut;
	nvtxcount	++;
	
	pIndices[nidxcount++]		= (WORD)nvtxcount-4;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-2;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;

	// right
	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
	pVertices[nvtxcount].u		= 0.125;
	pVertices[nvtxcount].v		= 1-txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.125;
	pVertices[nvtxcount].v		= 0+txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.25;
	pVertices[nvtxcount].v		= 0+txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.25;
	pVertices[nvtxcount].v		= 1-txcut;
	nvtxcount	++;
	
	pIndices[nidxcount++]		= (WORD)nvtxcount-4;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-2;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;

	// back
	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
	pVertices[nvtxcount].u		= 0.25;
	pVertices[nvtxcount].v		= 1-txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.25;
	pVertices[nvtxcount].v		= 0+txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.375;
	pVertices[nvtxcount].v		= 0+txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.375;
	pVertices[nvtxcount].v		= 1-txcut;
	nvtxcount	++;
	
	pIndices[nidxcount++]		= (WORD)nvtxcount-4;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-2;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;

	// left
	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
	pVertices[nvtxcount].u		= 0.375;
	pVertices[nvtxcount].v		= 1-txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.375;
	pVertices[nvtxcount].v		= 0+txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.5;
	pVertices[nvtxcount].v		= 0+txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.5;
	pVertices[nvtxcount].v		= 1-txcut;
	nvtxcount	++;
	
	pIndices[nidxcount++]		= (WORD)nvtxcount-4;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-2;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;

	// bottom
	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
	pVertices[nvtxcount].u		= (float)0.625+txcut;
	pVertices[nvtxcount].v		= (1-txcut);
	nvtxcount	++;

	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= (float)0.625+txcut;
	pVertices[nvtxcount].v		= (0+txcut);
	nvtxcount	++;

	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.75-txcut;
	pVertices[nvtxcount].v		= (0+txcut);
	nvtxcount	++;

	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nLowest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 0.75-txcut;
	pVertices[nvtxcount].v		= (1-txcut);
	nvtxcount	++;
	
	pIndices[nidxcount++]		= (WORD)nvtxcount-4;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-2;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;

	// top
	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);
	pVertices[nvtxcount].u		= (float)0.875-(float)0.0625;
	pVertices[nvtxcount].v		= 1-txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= -radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= (float)0.875-(float)0.0625;
	pVertices[nvtxcount].v		= 0+txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= -radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 1-(float)0.0625;
	pVertices[nvtxcount].v		= 0+txcut;
	nvtxcount	++;

	pVertices[nvtxcount].x		= radius;
	pVertices[nvtxcount].y		= m_nHighest;
	pVertices[nvtxcount].z		= radius;
	pVertices[nvtxcount].color	= D3DCOLOR_XRGB(255,255,255);;
	pVertices[nvtxcount].u		= 1-(float)0.0625;
	pVertices[nvtxcount].v		= 1-txcut;
	nvtxcount	++;
	
	pIndices[nidxcount++]		= (WORD)nvtxcount-4;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-2;
	pIndices[nidxcount++]		= (WORD)nvtxcount-1;
	pIndices[nidxcount++]		= (WORD)nvtxcount-3;

	m_pIB->Unlock();
	m_pVB->Unlock();

	m_nIdxCount		= nidxcount;
	m_nVtxCount		= nvtxcount;

	return	true;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoSky::Display(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CBravoSky::Display(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	m_matWorld._41	= g_Camera.m_vEye.x;
	m_matWorld._42	= g_Camera.m_vEye.y - 100;
	m_matWorld._43	= g_Camera.m_vEye.z;
	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorld);
	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(FVF_BRAVOSKY));
	g_RenderManager.SetFVF(D3DFVF_BRAVOSKY);
	g_RenderManager.SetIndices(m_pIB, 0);
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nVtxCount, 0, m_nIdxCount/3);
	return	S_OK;
}

/*
	* Cloud
	64		
-------------------------------------------------
|		|		|		|		|		|		|
|		|		|		|		|		|		|
|		|		|		|		|		|		|
|		|		|		|		|		|		|
-------------------------------------------------
*/

#define	D3DFVF_BRAVOCLOUD	(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)


CBravoCloud::CBravoCloud()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_pIB	= NULL;
	m_pVB	= NULL;
	m_nCloudCount	= 16;
	m_nCloudSize	= 10;
	m_nRadius		= 10;
	m_nBaseHeight	= -30;
	m_nCX	= m_nCY	= 0;
	m_byDepth		= 1;
}

CBravoCloud::~CBravoCloud()
{
}

void	CBravoCloud::FinalCleanup()
{
	if ( m_pIB )
	{
		m_pIB->Release();
		m_pIB	= NULL;
	}

	if ( m_pVB )
	{
		m_pVB->Release();
		m_pVB	= NULL;
	}
}

#ifdef DIRECT_VERSION_9_MJH
bool	CBravoCloud::Create(LPDIRECT3DDEVICE9 pDevice, const short& size)
#else
bool	CBravoCloud::Create(LPDIRECT3DDEVICE8 pDevice, const short& size)
#endif // DIRECT_VERSION_9_MJH
{
	short	nidxcount	= (m_nCloudCount*6)*m_byDepth;
	short	nvtxcount	= (m_nCloudCount*4)*m_byDepth;

	m_nCX	= (size>>1)<<4;
	m_nCY	= (size>>1)<<4;
	m_nRadius	= size<<4;

	if ( FAILED ( g_RenderManager.CreateIndexBuffer(nidxcount*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB) ) )
	{
///		MessageBox(NULL, "index buffer failed", "bravo water", MB_OK);
		return false;
	}
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(nvtxcount*sizeof(FVF_BRAVOCLOUD), D3DUSAGE_WRITEONLY, D3DFVF_BRAVOCLOUD, D3DPOOL_MANAGED, &m_pVB) ) )
	{
///		MessageBox(NULL, "vertex buffer failed", "bravo water", MB_OK);
		return false;
	}

	return	true;
}

bool	CBravoCloud::Update()
{
	if ( m_nCloudCount <= 0 )
		return	false;

	FVF_BRAVOCLOUD	*	pVertices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertices, 0 ) ) )
#else
	if ( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock VertexBuffer", "Bravo Cloud", MB_OK);
		return false;
	}

	/*
	*/
	short	nvtxcount = 0, nidxcount = 0;
	float	angleoffset	= 360/m_nCloudCount;
	float	curangle	= 360;
	double	sx, sy, ex, ey;
	float	uoffset		= 1.0f/m_nCloudCount;
	float	txsplit		= m_nCloudCount/4;

	//8	4	8	16	 32
	//	32, 64, 128, 256
	//	16/4 16/8 16/16
	short	split		= ((m_nRadius>>4)/8)/2;
	split	= (split>16) ? 16 : split;
	double	split2		= 16/(split);
	split	= 16/(split);
	float	splitu		= 1.0f/split2;//m_nCloudCount/split;

	for ( short j = m_byDepth-1; j >= 0; j -- )
	{
		curangle	= 360;
		for ( short i = 0; i < m_nCloudCount; i ++, curangle-=angleoffset )
		{
			sx	= cosf(curangle*0.01745) * (m_nRadius)*(j+1);
			sy	= sinf(curangle*0.01745) * (m_nRadius)*(j+1);
			ex	= cosf((curangle-angleoffset)*0.01745) * (m_nRadius)*(j+1);
			ey	= sinf((curangle-angleoffset)*0.01745) * (m_nRadius)*(j+1);

			pVertices[nvtxcount].x		= sx;
			pVertices[nvtxcount].y		= m_nBaseHeight;
			pVertices[nvtxcount].z		= sy;
			pVertices[nvtxcount].color	= D3DCOLOR_ARGB(255,255,255,255);
			pVertices[nvtxcount].u		= (i%split)*splitu;
//			pVertices[nvtxcount].u		= (j%2) ? (i%5)*0.2f : 1.0f - (i%5)*0.2f;
			pVertices[nvtxcount++].v	= 1.0f;

			pVertices[nvtxcount].x		= sx;
			pVertices[nvtxcount].y		= m_nBaseHeight+(m_nRadius>>2);
			pVertices[nvtxcount].z		= sy;
			pVertices[nvtxcount].color	= D3DCOLOR_ARGB(255,255,255,255);
			pVertices[nvtxcount].u		= (i%split)*splitu;
//			pVertices[nvtxcount].u		= (j%2) ? (i%5)*0.2f : 1.0f - (i%5)*0.2f;
			pVertices[nvtxcount++].v	= 0.0f;

			pVertices[nvtxcount].x		= ex;
			pVertices[nvtxcount].y		= m_nBaseHeight+(m_nRadius>>2);
			pVertices[nvtxcount].z		= ey;
			pVertices[nvtxcount].color	= D3DCOLOR_ARGB(255,255,255,255);
			pVertices[nvtxcount].u		= (i%split)*splitu + splitu;
//			pVertices[nvtxcount].u		= (j%2) ? (i%5)*0.2f + 0.2f : 1.0f-((i%5)*0.2f + 0.2f);
			pVertices[nvtxcount++].v	= 0.0f;

			pVertices[nvtxcount].x		= ex;
			pVertices[nvtxcount].y		= m_nBaseHeight;
			pVertices[nvtxcount].z		= ey;
			pVertices[nvtxcount].color	= D3DCOLOR_ARGB(255,255,255,255);
			pVertices[nvtxcount].u		= (i%split)*splitu + splitu;
//			pVertices[nvtxcount].u		= (j%2) ? (i%5)*0.2f + 0.2f : 1.0f-((i%5)*0.2f + 0.2f);
			pVertices[nvtxcount++].v	= 1.0f;
		}
	}
	
	m_pVB->Unlock();

	WORD *	pIndices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0, 0, (void**)&pIndices, 0 ) ) )
#else
	if ( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&pIndices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock IndexBuffer", "Bravo Cloud", MB_OK);
		return false;
	}
	nvtxcount	= 0;
	nidxcount	= 0;

	for ( short y = 0; y < m_byDepth; y ++ )
	{
		for ( short x = 0; x < m_nCloudCount; x ++ )
		{
			nvtxcount	+= 4;
			pIndices[nidxcount++]			= (WORD)nvtxcount-4;
			pIndices[nidxcount++]			= (WORD)nvtxcount-3;
			pIndices[nidxcount++]			= (WORD)nvtxcount-1;
			pIndices[nidxcount++]			= (WORD)nvtxcount-2;
			pIndices[nidxcount++]			= (WORD)nvtxcount-1;
			pIndices[nidxcount++]			= (WORD)nvtxcount-3;
		}
	}
	m_pIB->Unlock();

	m_nIdxCount		= nidxcount;
	m_nVtxCount		= nvtxcount;

	return	true;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoCloud::Display(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CBravoCloud::Display(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	static double	angle	= 0.0f;
	angle =	(angle>=360) ? 0 : angle+=0.01;
	D3DXMatrixRotationY(&m_matWorld, (angle*0.01745));
	m_matWorld._41	= m_nCX;
	m_matWorld._43	= m_nCY;
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCCOLOR );

	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0x0000008f );
	g_RenderManager.SetAlphaFunc( D3DCMP_GREATEREQUAL );

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorld);
	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(FVF_BRAVOCLOUD));

	g_RenderManager.SetFVF(D3DFVF_BRAVOCLOUD);
	
	g_RenderManager.SetIndices(m_pIB, 0);
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_nVtxCount, 0, m_nIdxCount/3);

	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetAlphaTestEnable( FALSE );

	g_RenderManager.SetCullMode( D3DCULL_CCW );

	return	S_OK;
}


/* constructor & destructor */
CField::CField()
{
/* Major */
	m_cSize			= 0;
//	m_pDisplayMap	= NULL;
	D3DXMatrixIdentity(&m_matWorld);

	m_pDevice		= NULL;	
	m_pVBuffer		= NULL;
	m_pIBuffer		= NULL;
	m_pVtxList		= NULL;
	m_pIdxList		= NULL;
	m_dwVtxCount	= 0;
	m_dwIdxCount	= 0;

/* Attr Map */
//	m_pAttrMap		= NULL;
	m_pAttrDisp		= NULL;
	D3DXMatrixIdentity(&m_matWorldAttr);

	m_pVBufferAttr	= NULL;
	m_pIBufferAttr	= NULL;
	m_pVtxListAttr	= NULL;
	m_pIdxListAttr	= NULL;
	m_dwVtxCountAttr	= 0;
	m_dwIdxCountAttr	= 0;
	m_pAttrTex		= NULL;
	m_dwEditableFloor	= MF_1;

/* Obj */
	//m_dwObjCount	= 0;
	m_nCurObj		= -1;
	m_nPickedObj	= -1;
	m_dwObjRotDir	= 0;

	m_ObjCount		= 0;
	m_bObjSortInit	= false;
	m_NowMapID		= -1;
	m_PreMapID		= -1;

	m_bShadowMakeObj	= true;
	m_bShadowDisplayObj = false;
	m_bChangeColor		= false;

/* Npc */
	m_dwNpcCount	= 0;
	m_nCurNpc		= -1;
	m_nPickedNpc	= -1;
	InitNpcTable();
	
/* Geometry Editing */
	m_dwGEditDir	= GED_UP;
	m_dwGEditType	= GEV_VOLUME;

/* Terrain Mapping */
	m_dwPickedX		= 0;
	m_dwPickedY		= 0;
	m_dwSelectedEle	= 0;
	m_bLibMapping	= true;
	m_dwPickedIdx	= 0;

	m_dwHighestFloor	= MF_1;
	m_dwCurFloor	= MF_1;
}

CField::~CField()
{
	FinalCleanup();
}

/*
	* 
*/
#ifdef DIRECT_VERSION_9_MJH
bool		CField::Create(LPDIRECT3DDEVICE9 pDevice)
#else
bool		CField::Create(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if ( pDevice == NULL )
	{
///		MessageBox(NULL, "INVALID DEVICE", "FIELD", MB_OK);
		return false;
	}
	m_pDevice	= pDevice;

	char	buffer[128];
	ZeroMemory(buffer, sizeof(char)*128);
//	sprintf(buffer, "%s\\attr.tga", g_szRoot);

	if ( FAILED ( g_RenderManager.CreateTextureFromFile( buffer, &m_pAttrTex ) ) )
	{
///		MessageBox(NULL, "Error - Create", "ATTR TEX", MB_OK);
		return	false;
	}
	return true;
}

bool		CField::Init()
{
//	Cleanup();
	return	true;
}

void		CField::Cleanup()
{
	short	i;
	_RELEASE(m_pVBuffer);
	_RELEASE(m_pIBuffer);
	m_pVtxList		= NULL;
	m_pIdxList		= NULL;
//	for ( DWORD i = 0; i < m_cSize; i ++ )
//		_DELETE_ARRAY(m_pDisplayMap[i]);
//	_DELETE_ARRAY(m_pDisplayMap);
//	for ( i = 0; i < m_cSize*2; i ++ )
//		_DELETE_ARRAY(m_pAttrMap[i]);
//	_DELETE_ARRAY(m_pAttrMap);
	for ( i = 0; i < m_cSize*2; i ++ )
		_DELETE_ARRAY(m_pAttrDisp[i]);
	_DELETE_ARRAY(m_pAttrDisp);

	m_dwEditableFloor	= MF_1;
	m_dwHighestFloor	= MF_1;	

}

void		CField::FinalCleanup()
{
	short	i;
	m_pDevice		= NULL;
	_RELEASE(m_pVBuffer);
	_RELEASE(m_pIBuffer);
//	for ( DWORD i = 0; i < m_cSize; i ++ )
//		_DELETE_ARRAY(m_pDisplayMap[i]);
//	_DELETE_ARRAY(m_pDisplayMap);
//	for ( i = 0; i < m_cSize*2; i ++ )
//		_DELETE_ARRAY(m_pAttrMap[i]);
//	_DELETE_ARRAY(m_pAttrMap);
	for ( i = 0; i < m_cSize*2; i ++ )
		_DELETE_ARRAY(m_pAttrDisp[i]);
	_DELETE_ARRAY(m_pAttrDisp);
	_RELEASE(m_pAttrTex);
	m_BravoCloud.FinalCleanup();
//	m_BravoSky.FinalCleanup();
}

bool		CField::BuildTerrain()
{
	DWORD	x, y, i;

	for ( i = 0; i < 9; i ++ )
	{
		m_WarpInfo[i].nDestMapID	= -1;
		m_WarpInfo[i].nDestWarpIdx	= -1;
	}

/////////////////////////////////////////////////////////////////////////////
	// Base Texture uv좌표 만들기
	for ( y = 0; y < m_cSize; y ++ )
	{
		for ( x = 0; x < m_cSize; x ++ )
		{
			m_pDisplayMap[y][x].Pos[0].tu	= (float)((float)x / (float)m_cSize);
			m_pDisplayMap[y][x].Pos[0].tv	= (float)((float)((m_cSize-1)-y+1) / (float)m_cSize);

			m_pDisplayMap[y][x].Pos[1].tu	= (float)((float)x / (float)m_cSize);
			m_pDisplayMap[y][x].Pos[1].tv	= (float)((float)((m_cSize-1)-y) / (float)m_cSize);

			m_pDisplayMap[y][x].Pos[2].tu	= (float)((float)(x+1) / (float)m_cSize);
			m_pDisplayMap[y][x].Pos[2].tv	= (float)((float)((m_cSize-1)-y) / (float)m_cSize);

			m_pDisplayMap[y][x].Pos[3].tu	= (float)((float)(x+1) / (float)m_cSize);
			m_pDisplayMap[y][x].Pos[3].tv	= (float)((float)((m_cSize-1)-y+1) / (float)m_cSize);
			
		}
	}
//////////////////////////////////////////////////////////////////////////////

	for ( y = 0; y < m_cSize; y ++ )
	{
		for ( x = 0; x < m_cSize; x ++ )
		{
			m_pDisplayMap[y][x].bIsVisible	= false;
			m_pDisplayMap[y][x].nTexIndex	= 0;
/*			m_pDisplayMap[y][x].dwSingleTex	= 0;
			m_pDisplayMap[y][x].dwMultiTex	= -1;
			m_pDisplayMap[y][x].dwSingleIdx	= -1;
			m_pDisplayMap[y][x].dwMultiIdx	= -1;
*/			
			m_pDisplayMap[y][x].Pos[0].x	= x * TILE_SIZE;
			m_pDisplayMap[y][x].Pos[0].y	= 0.0f;
			m_pDisplayMap[y][x].Pos[0].z	= y * TILE_SIZE;
//			m_pDisplayMap[y][x].Pos[0].tu2	= 0.0f;
//			m_pDisplayMap[y][x].Pos[0].tv2	= 1.0f;

			m_pDisplayMap[y][x].Pos[1].x	= x * TILE_SIZE;
			m_pDisplayMap[y][x].Pos[1].y	= 0.0f;
			m_pDisplayMap[y][x].Pos[1].z	= (y+1) * TILE_SIZE;
//			m_pDisplayMap[y][x].Pos[1].tu2	= 0.0f;
//			m_pDisplayMap[y][x].Pos[1].tv2	= 1.0f;

			m_pDisplayMap[y][x].Pos[2].x	= (x+1) * TILE_SIZE;
			m_pDisplayMap[y][x].Pos[2].y	= 0.0f;
			m_pDisplayMap[y][x].Pos[2].z	= (y+1) * TILE_SIZE;
//			m_pDisplayMap[y][x].Pos[2].tu2	= 0.0f;
//			m_pDisplayMap[y][x].Pos[2].tv2	= 1.0f;

			m_pDisplayMap[y][x].Pos[3].x	= (x+1) * TILE_SIZE;
			m_pDisplayMap[y][x].Pos[3].y	= 0.0f;
			m_pDisplayMap[y][x].Pos[3].z	= y * TILE_SIZE;
//			m_pDisplayMap[y][x].Pos[3].tu2	= 0.0f;
//			m_pDisplayMap[y][x].Pos[3].tv2	= 1.0f;
		}
	}
	const int size	= 400*6;

	if ( FAILED ( g_RenderManager.CreateIndexBuffer(size*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIBuffer) ) )
	{
///		MessageBox(NULL, "Error - Create(IB)", "indexbuffer-field", MB_OK);
		return false;
	}
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(size*sizeof(TERVTX), D3DUSAGE_WRITEONLY, D3DFVF_TERVTX, D3DPOOL_MANAGED, &m_pVBuffer) ) )
	{
///		MessageBox(NULL, "Error - Create(VB)", "vertexbuffer-field", MB_OK);
		return false;
	}

	SetupDefaultTexIndex();
#ifdef DIRECT_VERSION_9_MJH
	ZeroMemory( &m_Material, sizeof(D3DMATERIAL9) );
#else
	ZeroMemory( &m_Material, sizeof(D3DMATERIAL8) );
#endif // DIRECT_VERSION_9_MJH
/*
m_Material.Specular.r = 1.0f;
m_Material.Specular.g = 1.0f;
m_Material.Specular.b = 1.0f;
m_Material.Specular.a = 1.0f;
m_Material.Emissive.r = 1.0f;
m_Material.Emissive.g = 1.0f;
m_Material.Emissive.b = 1.0f;
m_Material.Emissive.a = 1.0f;
m_Material.Power		= 10.0f;
*/

	m_Material.Diffuse.r = 0.5f;
	m_Material.Diffuse.g = 0.5f;
	m_Material.Diffuse.b = 0.5f;	

	m_Material.Ambient.r = 0.5f;
	m_Material.Ambient.g = 0.5f;
	m_Material.Ambient.b = 0.5f;	
#ifdef DIRECT_VERSION_9_MJH
	ZeroMemory(&m_GridMaterial, sizeof(D3DMATERIAL9));
#else
	ZeroMemory(&m_GridMaterial, sizeof(D3DMATERIAL8));
#endif // DIRECT_VERSION_9_MJH
	m_GridMaterial.Diffuse.r = 0.0f;
	m_GridMaterial.Diffuse.g = 0.0f;
	m_GridMaterial.Diffuse.b = 1.0f;

	m_GridMaterial.Ambient.r = 0.0f;
	m_GridMaterial.Ambient.g = 0.0f;
	m_GridMaterial.Ambient.b = 1.0f;

#ifdef DIRECT_VERSION_9_MJH
	ZeroMemory(&m_AttrMaterial, sizeof(D3DMATERIAL9));
#else
	ZeroMemory(&m_AttrMaterial, sizeof(D3DMATERIAL8));
#endif // DIRECT_VERSION_9_MJH
	m_AttrMaterial.Diffuse.r = 1.0f;
	m_AttrMaterial.Diffuse.g = 1.0f;
	m_AttrMaterial.Diffuse.b = 1.0f;

	m_AttrMaterial.Ambient.r = 1.0f;
	m_AttrMaterial.Ambient.g = 1.0f;
	m_AttrMaterial.Ambient.b = 1.0f;

	/*
		* LIGHT CREATE
	*/
#ifdef DIRECT_VERSION_9_MJH
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
#else
	ZeroMemory(&m_Light, sizeof(D3DLIGHT8));
#endif // DIRECT_VERSION_9_MJH
	m_Light.Type		= D3DLIGHT_DIRECTIONAL;
	/*
	m_Light.Diffuse.r	= 0.8f;
	m_Light.Diffuse.g	= 0.8f;
	m_Light.Diffuse.b	= 0.8f;
	m_Light.Ambient.r	= 0.7f;
	m_Light.Ambient.g	= 0.7f;
	m_Light.Ambient.b	= 0.7f;
	m_Light.Direction.x	= 100;
	m_Light.Direction.y	= -100.0f;
	m_Light.Direction.z	= 100;

	m_Light.Range		= 1000.0f;
	//*/
	m_Light.Diffuse.r	= 1.0f;
	m_Light.Diffuse.g	= 1.0f;
	m_Light.Diffuse.b	= 1.0f;
	m_Light.Ambient.r	= 1.0f;
	m_Light.Ambient.g	= 1.0f;
	m_Light.Ambient.b	= 1.0f;
	m_Light.Direction.x	= 100;
	m_Light.Direction.y	= -100.0f;
	m_Light.Direction.z	= 100;

	m_Light.Range		= 1000.0f;

	g_RenderManager.SetLight(0, &m_Light);
	g_RenderManager.LightEnable(0, TRUE);
	
	g_RenderManager.SetLighting( TRUE );

	/* ATTR */
//	BuildAttr();
	CleanupObj();
	CleanupNpc();

	return true;
}

bool		CField::LoadTerrainNew(short* pHeightList, WORD* pTexIdxList, COLOR_RGB* pColorList)
{
	DWORD	x, y;
	
/////////////////////////////////////////////////////////////////////////////
	// Base Texture uv좌표 만들기
	///-- 해당 u,v값은 사용안하는듯...
	for ( y = 0; y < m_cSize; y++ )
	{
		for ( x = 0; x < m_cSize; x++ )
		{
			m_pDisplayMap[y][x].Pos[0].tu	= (float)((float)x / (float)m_cSize);
			m_pDisplayMap[y][x].Pos[0].tv	= (float)((float)((m_cSize-1)-y+1) / (float)m_cSize);

			m_pDisplayMap[y][x].Pos[1].tu	= (float)((float)x / (float)m_cSize);
			m_pDisplayMap[y][x].Pos[1].tv	= (float)((float)((m_cSize-1)-y) / (float)m_cSize);

			m_pDisplayMap[y][x].Pos[2].tu	= (float)((float)(x+1) / (float)m_cSize);
			m_pDisplayMap[y][x].Pos[2].tv	= (float)((float)((m_cSize-1)-y) / (float)m_cSize);

			m_pDisplayMap[y][x].Pos[3].tu	= (float)((float)(x+1) / (float)m_cSize);
			m_pDisplayMap[y][x].Pos[3].tv	= (float)((float)((m_cSize-1)-y+1) / (float)m_cSize);			
		}
	}
//////////////////////////////////////////////////////////////////////////////

	for ( y = 0; y < m_cSize; y++ )
	{
		for ( x = 0; x < m_cSize; x++ )
		{
			m_pDisplayMap[y][x].bIsVisible	= false;

			/// 서브타일 변형 정보 = 1000 자리수. 따라서 1000자리 수를 필터링 
			m_pDisplayMap[y][x].nTexIndex = ( int )( pTexIdxList[(y*m_cSize)+x] / 10000 ) * 10000 +
											( pTexIdxList[(y*m_cSize)+x] % 1000 );
			m_pDisplayMap[y][x].bySelTex	= 0;

			m_pDisplayMap[y][x].Pos[0].x	= x * TILE_SIZE;
			m_pDisplayMap[y][x].Pos[0].y	= pHeightList[((m_cSize+1)*y)+x];
			m_pDisplayMap[y][x].Pos[0].z	= y * TILE_SIZE;

			m_pDisplayMap[y][x].Pos[1].x	= x * TILE_SIZE;
			m_pDisplayMap[y][x].Pos[1].y	= pHeightList[((m_cSize+1)*(y+1))+x];
			m_pDisplayMap[y][x].Pos[1].z	= (y+1) * TILE_SIZE;

			m_pDisplayMap[y][x].Pos[2].x	= (x+1) * TILE_SIZE;
			m_pDisplayMap[y][x].Pos[2].y	= pHeightList[((m_cSize+1)*(y+1))+x+1];
			m_pDisplayMap[y][x].Pos[2].z	= (y+1) * TILE_SIZE;

			m_pDisplayMap[y][x].Pos[3].x	= (x+1) * TILE_SIZE;
			m_pDisplayMap[y][x].Pos[3].y	= pHeightList[((m_cSize+1)*y)+x+1];
			m_pDisplayMap[y][x].Pos[3].z	= y * TILE_SIZE;

			///-- 사용안함..
			SetupNewUV(x,y);
		}
	}
	
	///-- 사용안함..
	BuildNormalFull();

/// sooree mempool : CBravoMap::Create()에서 한번 생성하고 계속 가지고 간다. 

	return true;
}

bool		CField::UpdateTerrain(DWORD cx, DWORD cy)
{
	if ( m_pDisplayMap == NULL )
		return false;
	DWORD	y, x;

	sx	= (((cx>>1)-DISPLAY_RANGE)<0) ? sx = 0 : (cx>>1)-DISPLAY_RANGE;
	sy	= (((cy>>1)-DISPLAY_RANGE)<0) ? sy = 0 : (cy>>1)-DISPLAY_RANGE;
	ex	= (((cx>>1)-DISPLAY_RANGE)>=m_cSize) ? ex = m_cSize : (cx>>1)-DISPLAY_RANGE;
	ey	= (((cy>>1)-DISPLAY_RANGE)>=m_cSize) ? ey = m_cSize : (cy>>1)-DISPLAY_RANGE;

/*	sx = (cx/2)-DISPLAY_RANGE;
	sy = (cy/2)-DISPLAY_RANGE;
	ex = (cx/2)+DISPLAY_RANGE;
	ey = (cy/2)+DISPLAY_RANGE;

	if ( sx < 0 || sx >= m_cSize )		sx = 0;
	if ( sy < 0 || sy >= m_cSize )		sy = 0;
	if ( ex > m_cSize )					ex = m_cSize;
	if ( ey > m_cSize )					ey = m_cSize;
*/
	dwTileVisibleCount	= 0;
	/* VISIBLE CHECK */
/*	for ( y = sy; y < ey; y ++ )
	{
		for ( x = sx; x < ex; x ++ )
			//m_pDisplayMap[y][x].bIsVisible	= g_Camera.CubeInFrustum(x*TILE_SIZE+TILE_SIZE/2, TILE_SIZE/2,y*TILE_SIZE+TILE_SIZE/2, TILE_SIZE);
		{
			if ( true == (m_pDisplayMap[y][x].bIsVisible	= g_Camera.CubeInFrustum(m_pDisplayMap[y][x].Pos[0].x, m_pDisplayMap[y][x].Pos[0].y,m_pDisplayMap[y][x].Pos[0].z, TILE_SIZE)) )
				dwTileVisibleCount	++;
		}
	}

*/
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIBuffer->Lock(0, 0, (void**)&m_pIdxList, 0 ) ) )
#else
	if ( FAILED ( m_pIBuffer->Lock(0, 0, (BYTE**)&m_pIdxList, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock", "IB", MB_OK);
		return false;
	}
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVBuffer->Lock(0, 0, (void**)&m_pVtxList, 0 ) ) )
#else
	if ( FAILED ( m_pVBuffer->Lock(0, 0, (BYTE**)&m_pVtxList, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock", "VB", MB_OK);
		return false;
	}

	DWORD		dwVtxCount	= 0;
	DWORD		dwIdxCount	= 0;
	m_dwVtxCount	= 0;
	m_dwIdxCount	= 0;


	for ( y = sy; y < ey; y ++ )
	{
		for ( x = sx; x < ex; x ++ )
		{
			//if ( m_pDisplayMap[y][x].bIsVisible )
			if ( true == (m_pDisplayMap[y][x].bIsVisible = g_Camera.CubeInFrustum(m_pDisplayMap[y][x].Pos[0].x, m_pDisplayMap[y][x].Pos[0].y,m_pDisplayMap[y][x].Pos[0].z, TILE_SIZE)) )
			{
				dwTileVisibleCount	++;

				m_pVtxList[dwVtxCount++]	= m_pDisplayMap[y][x].Pos[0];
				m_pVtxList[dwVtxCount++]	= m_pDisplayMap[y][x].Pos[1];
				m_pVtxList[dwVtxCount++]	= m_pDisplayMap[y][x].Pos[2];
				m_pVtxList[dwVtxCount++]	= m_pDisplayMap[y][x].Pos[3];

				m_pIdxList[dwIdxCount++]	= (WORD)dwVtxCount-4;
				m_pIdxList[dwIdxCount++]	= (WORD)dwVtxCount-3;
				m_pIdxList[dwIdxCount++]	= (WORD)dwVtxCount-1;
				m_pIdxList[dwIdxCount++]	= (WORD)dwVtxCount-2;
				m_pIdxList[dwIdxCount++]	= (WORD)dwVtxCount-1;
				m_pIdxList[dwIdxCount++]	= (WORD)dwVtxCount-3;

			}
		}
	}

	m_dwVtxCount	= dwVtxCount;
	m_dwIdxCount	= dwIdxCount;

	m_pIBuffer->Unlock();
	m_pVBuffer->Unlock();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
// 지형에 라이트 효과 나오게 하는것 
// 옵션은 라이트 효과의 디스플레이 위치 확정후에 글로벌로 한번만 셋업하고 해제 한다.
// 텍스처 매트릭스는 내부에서 설정한다.


//-----------------------------------------------------------------------------
// Desc: 타일색깔 변하게하기
//-----------------------------------------------------------------------------
void		CField::SetTileColor( short cx, short cy, short R, short G, short B, float fChangeTime, float fEndTime )
{
	if( g_Map.m_byMapType == MS_INDOOR )
		return;
	//-----------------------------------------------------------------------------
	// fChangeTime -1 이면 원래색으로 복구
	//-----------------------------------------------------------------------------
	if( fChangeTime == -1 )
	{
		m_bChangeColor = false;	
		g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
//		if ( g_Map.m_bShowSky )
//			g_Map.m_BravoSky.UpdateColor();
	}
	else
	{
		//-----------------------------------------------------------------------------
		// 현재시간, 칼라변경 플래그 true, R G B 값으로 색저장
		//-----------------------------------------------------------------------------
		m_change_x		= cx;
		m_change_y		= cy;
		m_bChangeColor	= true;	
		m_fPreTime		= g_nowTime;
		m_fChangeTime	= fChangeTime;
		if( fEndTime == -1 )
			m_fEndTime = fEndTime;
		else
		{
			m_fEndTime = SAFE_TIME_ADD( g_nowTime , fEndTime );
		}

		// 변경되는 색 범위저장	
		m_ChangeColor.r	= R;
		m_ChangeColor.g	= G;
		m_ChangeColor.b	= B;	
		m_vertex_color	= D3DCOLOR_XRGB( m_ChangeColor.r, m_ChangeColor.g, m_ChangeColor.b );

		// 맵, 스카이박스 업데이트
		g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );		
//		if ( g_Map.m_bShowSky )
//			g_Map.m_BravoSky.UpdateColor();
	}
}

void		CField::UpdateTileColor()
{
	if( m_bChangeColor == true )
	{
		//-----------------------------------------------------------------------------
		// 현재시간이 끝내야할 시간보다 같거나 크다면 타일 색 초기화시키기
		// -1 이면 변한색으로 계속 지속된다
		//-----------------------------------------------------------------------------
		if( m_fEndTime != -1 && SAFE_TIME_COMPARE( m_fEndTime , <= , g_nowTime ) )
		{
			// 플래그 false
			m_bChangeColor = false;					
			
			// 맵, 스카이박스 업데이트			
			g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
//			if ( g_Map.m_bShowSky )
//				g_Map.m_BravoSky.UpdateColor();
		}
	}
}


// 타일버텍스 칼라와 현재 변하는 칼라와 합치기
DWORD		CField::SetVertexColor( DWORD color )
{		
	// 현재타일색
	BYTE b = (color & 255);	
	BYTE g = ((color >> 8)  & 255);
	BYTE r = ((color >> 16) & 255);	
	BYTE a = ((color >> 24) & 255);
			
	// 변하는색과 현재의%값을 곱하기
	float	value;	
	value	= (float)r/255.0f;
	r = m_ChangeColor.r*value;
	
	value	= (float)g/255.0f;
	g = m_ChangeColor.g*value;
	
	value	= (float)b/255.0f;
	b = m_ChangeColor.b*value;

	// DWORD 값으로 만들어서 리턴하기
	return D3DCOLOR_ARGB( a, r, g, b );
}


//-----------------------------------------------------------------------------
// Desc: 라이트타일 그리기
// scale: 1.0f 가장 큰 상태 값이 커질수록 작아진다.
//-----------------------------------------------------------------------------
#define		LIGHT_TILE_ROT_TIME		500.0f;

void		CField::ClearLightTile( int Index, long lUnique )
{
	for( int i = 0; i < LIGHTTALBE_COUNT; ++i )
	{
		if( m_LightTable[i].Index == Index && m_LightTable[i].lUnique == lUnique )
		{
			m_LightTable[i].Clear();
		}
	}
}

void		CField::SetLightTile( D3DXMATRIX mat, int Display, int Index, long lUnique )
{
	// 테이블에서 bDisplay가 false인 배열에 메트릭스 넣는다
	for( int i = 0; i < LIGHTTALBE_COUNT; ++i )
	{			
		if( m_LightTable[i].Display == -2 )
		{		
			m_LightTable[i].fRot		= 0.0f;
			m_LightTable[i].matPos		= mat;		
			m_LightTable[i].Display		= Display;
			
			// 대상인덱스, 유니크
			m_LightTable[i].Index		= Index;
			m_LightTable[i].lUnique		= lUnique;
			break;
		}
	}	
}

void		CField::UpdateLightTile()
{
	//-----------------------------------------------------------------------------
	// 옵션설정
	//-----------------------------------------------------------------------------
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0 );
	g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );

	g_RenderManager.SetTextureAddressU( 1 , D3DTADDRESS_CLAMP );
	g_RenderManager.SetTextureAddressV( 1 , D3DTADDRESS_CLAMP );
	g_RenderManager.SetTextureAddressW( 1 , D3DTADDRESS_CLAMP );

	g_RenderManager.SetTextureCoordIndex( 1 , D3DTSS_TCI_CAMERASPACEPOSITION );

	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_COUNT4|D3DTTFF_PROJECTED );

	//-----------------------------------------------------------------------------
	// 렌더링, 텍스쳐 셋팅
	//-----------------------------------------------------------------------------
	g_RenderManager.SetDestBlend(  D3DBLEND_DESTALPHA );
	
	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_TEXTURE     );


	g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_MODULATE   );
	g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_TEXTURE     );
//	g_RenderManager.SetTextureAlphaArg2( 1 , D3DTA_CURRENT     );	// by wxywxy 추가
	
	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_TEXTURE );

	g_RenderManager.SetTextureMipFilter( 0 , D3DTEXF_NONE);
	g_RenderManager.SetTextureMagFilter( 0 , D3DTEXF_NONE);
	g_RenderManager.SetTextureMinFilter( 0 , D3DTEXF_NONE);

	g_RenderManager.SetTextureMipFilter( 1 , D3DTEXF_NONE);
	g_RenderManager.SetTextureMagFilter( 1 , D3DTEXF_NONE);
	g_RenderManager.SetTextureMinFilter( 1 , D3DTEXF_NONE);

	//-----------------------------------------------------------------------------
	// 그림자그리기
	//-----------------------------------------------------------------------------	
	g_Map.DisplayShadow();

	//-----------------------------------------------------------------------------
	// 옵션돌려놓기
	//-----------------------------------------------------------------------------
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	g_RenderManager.SetTexture(1, NULL);		

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );


	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE   );

	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_TEXTURE );


	g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_DISABLE );
	g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_TEXTURE   );

	g_RenderManager.SetTextureCoordIndex( 1 , D3DTSS_TCI_PASSTHRU );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );

	g_RenderManager.SetTextureAddressU( 1 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressV( 1 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressW( 1 , D3DTADDRESS_WRAP );

	g_RenderManager.SetTextureMipFilter( 0 , D3DTEXF_LINEAR);
	g_RenderManager.SetTextureMagFilter( 0 , D3DTEXF_LINEAR);
	g_RenderManager.SetTextureMinFilter( 0 , D3DTEXF_LINEAR);

	g_RenderManager.SetTextureMipFilter( 1 , D3DTEXF_LINEAR);
	g_RenderManager.SetTextureMagFilter( 1 , D3DTEXF_LINEAR);
	g_RenderManager.SetTextureMinFilter( 1 , D3DTEXF_LINEAR);
}




void		CField::DisplayLightTile(const D3DXMATRIX& mat, int type, float fRot )
{	
	if( m_pDisplayMap == NULL )
		return;
				
	g_RenderManager.SetStreamSource(0, m_pVBuffer, sizeof(TERVTX));
	
	g_RenderManager.SetFVF( D3DFVF_TERVTX );
	g_RenderManager.SetIndices(m_pIBuffer, 0);	

	short	x	= mat._41 / 16;
	short	y	= mat._43 / 16;	
	short	idx;
	

	//-----------------------------------------------------------------------------
	// 라이트 타일 메트릭스 구하기
	//-----------------------------------------------------------------------------
	// 프로젝션
	D3DXMATRIX	matMiddle;	
	D3DXMatrixPerspectiveFovLH( &matMiddle, D3DX_PI/4, 1.3f, 1.0f, 10000.0f );
	switch( type )
	{
	// 바닥광원
	case -1:
		{
			matMiddle._11 = 1.0f;
			matMiddle._22 = 1.0f;
		}
		break;						
	default:
		{
			matMiddle._11 = 1.5f;
			matMiddle._22 = 1.5f;			
		}
		break;
	}		

	// 라이트위치
	D3DXMATRIX	matWorldToLight;
	D3DXVECTOR3	vPos = D3DXVECTOR3(mat._41, mat._42+20.0f, mat._43-1.0f);	
	D3DXMatrixLookAtLH(&matWorldToLight, &vPos, &D3DXVECTOR3(mat._41,mat._42,mat._43),&D3DXVECTOR3(0.0f,1.0f,0.0f));

	// 텍스쳐에서 그려질 위치
	D3DXMATRIX	matOffset;					
	D3DXMatrixIdentity( &matOffset );
	matOffset._11 =  0.5f;
	matOffset._22 = -0.5f;	
	matOffset._31 =  0.5f;
	matOffset._32 =  0.5f;

	// 회전값
	D3DXMATRIX	matRot;
	D3DXMatrixRotationZ( &matRot, fRot );

	// 텍스쳐행렬만들기
	D3DXMATRIX	matTexPos;
	D3DXMatrixMultiply( &matTexPos, &g_Camera.m_matInvView, &matWorldToLight );
	D3DXMatrixMultiply( &matTexPos, &matTexPos, &matRot );
	D3DXMatrixMultiply( &matTexPos, &matTexPos, &matMiddle );	
	D3DXMatrixMultiply( &matTexPos, &matTexPos, &matOffset );		
	
	//-----------------------------------------------------------------------------
	// 타일에 그려지 범위와 타일크기 설정
	//-----------------------------------------------------------------------------
	float	scale	= 1.0f;
	BYTE	tilenum = 1;	
	switch( type )
	{		
	case -1:	// 바닥광원
		{
			g_RenderManager.SetTexture( 1, g_Pc.m_pEffectTex[7].pTex );
		}
		break;	
	case 0:		// 차지
		{
			g_RenderManager.SetTexture( 1, g_Pc.m_pEffectTex[0].pTex );
		}
		break;	
	case 1:		// 합체
		{
			g_RenderManager.SetTexture( 1, g_Pc.m_pEffectTex[1].pTex );
		}
		break;
	// 요정스킬종류	
	case ELLIADEN_SKILL_104:	// 서펜트(마법 공격력 UP)
		{
			g_RenderManager.SetTexture( 1, g_Pc.m_pEffectTex[6].pTex );
		}
		break;	
	case ELLIADEN_SKILL_105:	// 블레스(물리 방어력 UP)
		{
			g_RenderManager.SetTexture( 1, g_Pc.m_pEffectTex[2].pTex );
		}
		break;	
	case ROIEL_SKILL_156:		// 스트렝스(물리 공격력 UP)
		{
			g_RenderManager.SetTexture( 1, g_Pc.m_pEffectTex[3].pTex );
		}
		break;
	default:
		{
			return;
		}
	}		
	g_RenderManager.SetTransform(D3DTS_TEXTURE1, &matTexPos);


	//-----------------------------------------------------------------------------
	// 렌더링
	//-----------------------------------------------------------------------------
	short	minx	= x-tilenum;
	short	miny	= y-tilenum;
	short	maxx	= x+tilenum+1;
	short	maxy	= y+tilenum+1;
	if ( minx < 0 && miny < 0 && maxx >= m_cSize && maxy >= m_cSize )
		return;
	for ( short ay = miny; ay < maxy; ay ++ )
	{
		for ( short ax = minx; ax < maxx; ax ++ )
		{
			if ( ax < 0 || ax >= m_cSize || ay < 0 || ay >= m_cSize )
				continue;
			idx	= m_pDisplayMap[ay][ax].vidx;	
			unsigned short ntidx	= (m_pDisplayMap[ay][ax].nTexIndex/10000)+(((m_pDisplayMap[ay][ax].nTexIndex%10000)/14)*5);
			g_RenderManager.SetTexture(0, g_TileTexture.GetTextureSelection(ntidx,m_pDisplayMap[ay][ax].bySelTex));
			g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, idx, 4, (idx>>2)*6, 2);
		}
	}


	//-----------------------------------------------------------------------------
	// 중복되서 그리는 경우
	//-----------------------------------------------------------------------------
	bool bFind = false;
	switch( type )
	{
		case ELLIADEN_SKILL_105:	// 블레스(마법 공격력 UP)
		{
			bFind = true;
			g_RenderManager.SetTexture( 1, g_Pc.m_pEffectTex[5].pTex );
		}
		break;	
	}
	if( bFind == true )
	{		
		// 렌더링		
		short	minx	= x-tilenum;
		short	miny	= y-tilenum;
		short	maxx	= x+tilenum+1;
		short	maxy	= y+tilenum+1;
		if ( minx < 0 && miny < 0 && maxx >= m_cSize && maxy >= m_cSize )
			return;
		for ( short ay = miny; ay < maxy; ay ++ )
		{
			for ( short ax = minx; ax < maxx; ax ++ )
			{
				if ( ax < 0 || ax >= m_cSize || ay < 0 || ay >= m_cSize )
					continue;
				idx	= m_pDisplayMap[ay][ax].vidx;	
				unsigned short ntidx	= (m_pDisplayMap[ay][ax].nTexIndex/10000)+(((m_pDisplayMap[ay][ax].nTexIndex%10000)/14)*5);
				g_RenderManager.SetTexture(0, g_TileTexture.GetTextureSelection(ntidx,m_pDisplayMap[ay][ax].bySelTex));			
				g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, idx, 4, (idx>>2)*6, 2);
			}
		}
	}
}


void		CField::DisplayShadowTile2(const short& px, const short& py)
{
	if ( m_pDisplayMap == NULL )
		return;

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorld);
	g_RenderManager.SetStreamSource(0, m_pVBuffer, sizeof(TERVTX));
	
	g_RenderManager.SetFVF( D3DFVF_TERVTX );
	g_RenderManager.SetIndices(m_pIBuffer, 0);	

	short	curidx	= 0;
	short	curvtx	= 0;

	int px_2 = px >> 1;
	int py_2 = py >> 1;

	short idx = 0;
	int x = 0;
	int y = 0;
	int realX = 0;
	int realY = 0;
	
	g_RenderManager.SetZBias( 2 );	

	realX = px_2;
	realY = py_2;

	int aBlockSize = 2;

	map <int, int> tempList;
	
	for( x = -( aBlockSize - 1 ); x <= ( aBlockSize - 1 ); ++x )
	{
		for( y = -( aBlockSize - 1 ); y <= ( aBlockSize - 1 ); ++y )
		{
			realX = px_2 + x;
			realY = py_2 + y;
			if( realX < 0 || realY < 0 || realX >= MAX_MAP_BLOCK || realY >= MAX_MAP_BLOCK )
			{
				continue;
			}
						
			idx	= m_pDisplayMap[ realY ][ realX ].vidx;
			
			map < int, int >::iterator fPos = tempList.find(idx);
			if(fPos != tempList.end())
			{
				continue;
			}
			
			tempList.insert(map <int, int>::value_type(idx, idx));
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, idx, 4, (idx>>2)*6, 2 );					
		}
	}

	g_RenderManager.SetZBias( 0 );
}

void CField::DisplayOnTile2(float px,float pz,float xsize,float zsize)
{
	int TileCenterX   = ( int ) ( px / 16.0f );
	int TileCenterZ   = ( int ) ( pz / 16.0f );

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorld);
	g_RenderManager.SetStreamSource(0, m_pVBuffer, sizeof(TERVTX));
	g_RenderManager.SetFVF( D3DFVF_TERVTX );
	g_RenderManager.SetIndices(m_pIBuffer, 0);	

	D3DXMATRIX matInvCamera;
	D3DXMATRIX matOffset;
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;

	float AddPosX = ( px  ) - ( TileCenterX * 16.0f );
	float AddPosZ = ( pz  ) - ( TileCenterZ * 16.0f );
#ifdef DIRECT_VERSION_9_MJH
	FLOAT fDeterminantD3DMatView = D3DXMatrixDeterminant( &g_Camera.m_matView );
#else
	FLOAT fDeterminantD3DMatView = D3DXMatrixfDeterminant( &g_Camera.m_matView );
#endif // DIRECT_VERSION_9_MJH	

	D3DXMatrixInverse( &matInvCamera, &fDeterminantD3DMatView, &g_Camera.m_matView );
	
	memset( &matOffset , 0 , sizeof( D3DXMATRIX ) );
	
	matOffset._11 = 1.0f / ( 16.0f );
	matOffset._32 = 1.0f / ( 16.0f );

	matOffset._41 =  -TileCenterX + ( -AddPosX / 16.0f ) + 0.5f + ( xsize - 16.0f ) / 32.0f;
	matOffset._42 =  -TileCenterZ + ( -AddPosZ / 16.0f ) + 0.5f + ( zsize - 16.0f ) / 32.0f;

	memset( &matScale , 0 , sizeof( D3DXMATRIX ) );

	matScale._11 = 1.0f / ( xsize / 16.0f ) ;
	matScale._22 = 1.0f / ( zsize / 16.0f ) ;

	D3DXMatrixMultiply( &matOffset , &matOffset , &matScale );
	D3DXMatrixMultiply( &matTrans  , &matInvCamera , &matOffset );

	g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_CLAMP );
	g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_CLAMP );
	g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_CLAMP );

	g_RenderManager.SetTextureCoordIndex( 0 , D3DTSS_TCI_CAMERASPACEPOSITION );
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT2 );

	g_RenderManager.SetTransform(D3DTS_TEXTURE0, &matTrans); 

	int TileXSize = ( xsize * 0.5f ) / 16.0f;
	int TileZSize = ( zsize * 0.5f ) / 16.0f;

	if( TileXSize <= 0 )
	{
		TileXSize = 1;
	}
	else
	if( xsize - ( TileXSize * 16.0f ) >= 0 )
	{
		TileXSize++;
	}

	if( TileZSize <= 0 )
	{
		TileZSize = 1;
	}
	else
	if( zsize - ( TileZSize * 16.0f ) >= 0 )
	{
		TileZSize++;
	}

	int idx;

	for( int x = TileCenterX - TileXSize ; x <= TileCenterX + TileXSize ; ++x )
	{
		for( int z = TileCenterZ - TileZSize ; z <= TileCenterZ + TileZSize ; ++z )
		{
			if( x < 0 || z < 0 || x >= m_cSize || z >= m_cSize )
				continue;
			
			if( m_pDisplayMap[z][x].bIsVisible )
			{
				idx	= m_pDisplayMap[z][x].vidx;

				g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, idx, 4, (idx>>2)*6, 2);
			}
		}
	}

	D3DXMatrixIdentity( &matTrans );
	g_RenderManager.SetTransform( D3DTS_TEXTURE0 , &matTrans );
	g_RenderManager.SetTextureCoordIndex( 0 , 0 );
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );

	g_RenderManager.SetTexture(0, NULL);		
}


//.............................................................................................................
// 타일 위에 그리기 ( 실시간 루틴 ) By wxywxy
//.............................................................................................................
void		CField::DisplayOnTile(float px,float pz,float xsize,float zsize)
{	
	///-- px, pz X, Y 중심 위치 xsize, zsize 타일 사이즈 
	///-- 타일 사이즈로 나누어 셀 위치를 구한다.	
	int TileCenterX   = ( int ) ( px / 16.0f );	 		
	int TileCenterZ   = ( int ) ( pz / 16.0f );

	///-- 단위행렬로 월드행렬을 셋팅
	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorld);
	
	g_RenderManager.SetFVF( D3DFVF_TERVTX );

	D3DXMATRIX matInvCamera;
	D3DXMATRIX matOffset;
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;

	float AddPosX = ( px  ) - ( TileCenterX * 16.0f );
	float AddPosZ = ( pz  ) - ( TileCenterZ * 16.0f );
	///-- 구해진 셀위치에서 실제위치로의 추가값을 구한다.
	
	///--FLOAT fDeterminantD3DMatView = D3DXMatrixfDeterminant( &g_Camera.m_matView );
	///--D3DXMatrixInverse( &matInvCamera, &fDeterminantD3DMatView, &g_Camera.m_matView );
	D3DXMatrixInverse( &matInvCamera, NULL, &g_Camera.m_matView );
	
	memset( &matOffset , 0 , sizeof( D3DXMATRIX ) );
	
	matOffset._11 = 1.0f / ( 16.0f );
	matOffset._32 = 1.0f / ( 16.0f );
	//-- u에 들어가는 값은 x, v 에 들어가는 값은 z 이므로 _11, _32의 값에 세팅

	matOffset._41 =  -TileCenterX + ( -AddPosX / 16.0f ) + 0.5f + ( xsize - 16.0f ) / 32.0f;
	matOffset._42 =  -TileCenterZ + ( -AddPosZ / 16.0f ) + 0.5f + ( zsize - 16.0f ) / 32.0f;
	///-- 여기서 0.5f는 마법의 수
	///-- 해당위치를 중심으로 u,v값을 계산한다.

	memset( &matScale , 0 , sizeof( D3DXMATRIX ) );
	matScale._11 = 1.0f / ( xsize / 16.0f ) ;
	matScale._22 = 1.0f / ( zsize / 16.0f ) ;
	///-- 계산된 u,v값을 스케일로 나눠준다.
	///-- By simwoosung
	///-- 이 유도식은 따로 구해진듯....;;;; 대단하다....(ㅡㅡ)b
	///-- UV 자동생성방식은 다시 봐야 할 필요가 있을듯...

	D3DXMatrixMultiply( &matOffset , &matOffset , &matScale );
	///-- 텍스쳐 uv값을 구하기 위해 입력값으로 카메라 공간전환된 위치값을 사용하기 때문에...
	///-- 해당 값을 다시 월드좌표로 바꿔주고 matOffset행렬을 계산한다.
	D3DXMatrixMultiply( &matTrans  , &matInvCamera , &matOffset );

	g_RenderManager.SetTextureCoordIndex(  0 , D3DTSS_TCI_CAMERASPACEPOSITION );
	///-- 이 스테이지의 텍스처 변환에 대한 입력 텍스처 좌표로서 
	///-- 카메라 공간에 변환 된 정점 위치를 사용한다 0번 텍스쳐 스테이지
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT2 );
	///- 래스터라이저는, 2D 의 텍스처 좌표를 상정한다.

	g_RenderManager.SetTransform(D3DTS_TEXTURE0, &matTrans); 

	g_RenderManager.SetZBias( 2 );

	int TileXSize = ( xsize * 0.5f ) / 16.0f;
	int TileZSize = ( zsize * 0.5f ) / 16.0f;

	if( TileXSize <= 0 )
	{
		TileXSize = 1;
	}
	else
	if( xsize - ( TileXSize * 16.0f ) >= 0 )
	{
		TileXSize++;
	}

	if( TileZSize <= 0 )
	{
		TileZSize = 1;
	}
	else
	if( zsize - ( TileZSize * 16.0f ) >= 0 )
	{
		TileZSize++;
	}
	////-- 타일을 그리기 위한 영역을 구한다.

	//int idx;

	TERVTX TileVertex[ 4 ];

	for( int x = TileCenterX - TileXSize ; x <= TileCenterX + TileXSize ; ++x )
	{
		for( int z = TileCenterZ - TileZSize ; z <= TileCenterZ + TileZSize ; ++z )
		{
			if( x < 0 || z < 0 || x >= m_cSize || z >= m_cSize )
				continue;
			
			//if( m_pDisplayMap[z][x].bIsVisible )
			{
				//idx	= m_pDisplayMap[z][x].vidx;
				memcpy( &TileVertex[ 0 ] , &m_pDisplayMap[z][x].Pos[ 3 ] , sizeof( TERVTX ) );
				memcpy( &TileVertex[ 1 ] , &m_pDisplayMap[z][x].Pos[ 0 ] , sizeof( TERVTX ) );
				memcpy( &TileVertex[ 2 ] , &m_pDisplayMap[z][x].Pos[ 2 ] , sizeof( TERVTX ) );
				memcpy( &TileVertex[ 3 ] , &m_pDisplayMap[z][x].Pos[ 1 ] , sizeof( TERVTX ) );

				g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2 , TileVertex , sizeof(TERVTX) );
			
			}
		}
	}

	D3DXMatrixIdentity( &matTrans );
	g_RenderManager.SetTransform( D3DTS_TEXTURE0 , &matTrans );
	
	g_RenderManager.SetTextureCoordIndex( 0 , 0 );
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
	
	g_RenderManager.SetTexture(0, NULL);		
	g_RenderManager.SetZBias( 0 );
}


void CField::DisplayShadowTile( const short& px, const short& py, int aBlockSize )
{
	if ( m_pDisplayMap == NULL )
		return;

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorld);
	g_RenderManager.SetStreamSource(0, m_pVBuffer, sizeof(TERVTX));

	g_RenderManager.SetFVF( D3DFVF_TERVTX );
	
	g_RenderManager.SetIndices(m_pIBuffer, 0);	

	int px_2 = px >> 1;
	int py_2 = py >> 1;

	short idx = 0;
	int x = 0;
	int y = 0;
	int realX = 0;
	int realY = 0;

	aBlockSize += 1;
	
	map <int, int> tempList;
	
	g_RenderManager.SetZBias( 2 );
	
	for( x = -( aBlockSize - 1 ); x <= ( aBlockSize - 1 ); ++x )
	{
		for( y = -( aBlockSize - 1 ); y <= ( aBlockSize - 1 ); ++y )
		{
			realX = px_2 + x;
			realY = py_2 + y;
			if( realX < 0 || realY < 0 || realX >= MAX_MAP_BLOCK || realY >= MAX_MAP_BLOCK )
			{
				continue;
			}
						
			idx	= m_pDisplayMap[ realY ][ realX ].vidx;
			
			map < int, int >::iterator fPos = tempList.find(idx);
			if(fPos != tempList.end())
			{
				continue;
			}
			
			tempList.insert(map <int, int>::value_type(idx, idx));
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, idx, 4, (idx>>2)*6, 2 );					
		}
	}
	
	tempList.clear();

	g_RenderManager.SetZBias( 0 );

	return;
}

void		CField::DisplayShadowTile(const D3DXMATRIX& mat, const D3DXVECTOR3& min, const D3DXVECTOR3& max)
{
	if ( m_pDisplayMap == NULL )
		return;

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorld);	
	g_RenderManager.SetStreamSource(0, m_pVBuffer, sizeof(TERVTX));
	
	g_RenderManager.SetFVF( D3DFVF_TERVTX );
	g_RenderManager.SetIndices(m_pIBuffer, 0);	

	short	nXsize, nYsize, nXstart, nYstart;
	POINT	list[512];
	short	listcount	= 0;

	nXsize	= (max.x - min.x)/(TILE_SIZE);
	nYsize	= (max.z - min.z)/(TILE_SIZE);

	if ( nXsize <= 0 )
		nXsize	= 1;
	if ( nYsize <= 0 )
		nYsize	= 1;

//	nXstart	= (mat._41/TILE_SIZE)-nXsize;
//	nYstart	= (mat._43/TILE_SIZE)-nYsize-2;
//	nXsize	+= 6;
//	nYsize	+= 12;

	nXstart	= (mat._41/TILE_SIZE)-nXsize;
	nYstart	= (mat._43/TILE_SIZE)-nYsize-2;

	nYstart	-= 3;

	nXsize	+= 6;
	nYsize	+= 12;


	short	y, x;
	for ( y = nYstart; y < nYstart+nYsize; y ++ )
	{
		for ( x = nXstart; x < nXstart+nXsize; x ++ )
		{
			list[listcount].x	= x;
			list[listcount++].y	= y;
		}
	}

	short	curidx	= 0;
	short	curvtx	= 0;

	for ( y = sy; y < ey; y ++ )
	{
		for ( x = sx; x < ex; x ++ )
		{
			if ( m_pDisplayMap[y][x].bIsVisible )
			{
				for ( short i = 0; i < listcount; i ++ )
				{
					if ( list[i].x == x && list[i].y == y )
					{
						unsigned short ntidx	= (m_pDisplayMap[y][x].nTexIndex/10000)+(((m_pDisplayMap[y][x].nTexIndex%10000)/14)*5);
						g_RenderManager.SetTexture(0, g_TileTexture.GetTextureSelection(ntidx,m_pDisplayMap[y][x].bySelTex));
						g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, curvtx, 4, curidx, 2);
					}
				}

				curidx	+= 6;
				curvtx	+= 4;
			}
		}
	}
}



HRESULT		CField::Display()
{
	if ( m_pDisplayMap == NULL )
		return E_FAIL;

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorld);
	g_RenderManager.SetStreamSource(0, m_pVBuffer, sizeof(TERVTX));
	g_RenderManager.SetFVF( D3DFVF_TERVTX );
	g_RenderManager.SetIndices(m_pIBuffer, 0);

	g_RenderManager.SetFillMode( D3DFILL_SOLID );
	g_RenderManager.SetMaterial(&m_Material);
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_dwVtxCount, 0, m_dwIdxCount/3);


	return	S_OK;
}

BOOL		CField::IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v)
{
	// Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &dir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 ) 
    {
        tvec = orig - v0;
    }
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

    if( det < 0.0001f )
	{
        return FALSE;
	}

    // Calculate U parameter and test bounds
    *u = D3DXVec3Dot( &tvec, &pvec );
    if( *u < 0.0f || *u > det )
	{
        return FALSE;
	}

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    *v = D3DXVec3Dot( &dir, &qvec );
    if( *v < 0.0f || *u + *v > det )
	{
        return FALSE;
	}

    // Calculate t, scale parameters, ray intersects triangle
    *t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return TRUE;
}

BOOL		CField::IntersectTriangle2(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v, BOOL* fb )
{
	// Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &dir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = orig - v0;
		*fb = true;
    }
    else
    {
        tvec = v0 - orig;
        det = -det;
		*fb = false;
    }

    if( det < 0.0001f )
        return FALSE;

    // Calculate U parameter and test bounds
    *u = D3DXVec3Dot( &tvec, &pvec );
    if( *u < 0.0f || *u > det )
        return FALSE;

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    *v = D3DXVec3Dot( &dir, &qvec );
    if( *v < 0.0f || *u + *v > det )
        return FALSE;

    // Calculate t, scale parameters, ray intersects triangle
    *t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return TRUE;
}

/****************************************************************************************************
	* FILE
****************************************************************************************************/
void	CField::StoreHeight(float *height)
{
	for ( DWORD y = 0; y < m_cSize; y ++ )
	{
		for ( DWORD x = 0; x < m_cSize; x ++ )
		{
			height[(y*(m_cSize+1))+x]	= m_pDisplayMap[y][x].Pos[0].y;
			if ( x == m_cSize-1 )
				height[(y*(m_cSize+1))+x+1]	= m_pDisplayMap[y][x].Pos[3].y;
		}
		if ( y == m_cSize-1)
		{
			for (DWORD x = 0; x < m_cSize; x ++ )
			{
				height[((y+1)*(m_cSize+1))+x]	= m_pDisplayMap[y][x].Pos[1].y;
				if ( x == m_cSize-1 )
					height[((y+1)*(m_cSize+1))+x+1]	= m_pDisplayMap[y][x].Pos[2].y;
			}
		}
	}
}
  
void	CField::StoreTexIdx(WORD *texidx)
{
	for ( DWORD y = 0; y < m_cSize; y ++ )
	{
		for ( DWORD x = 0; x < m_cSize; x ++ )
		{
//			texidx[(y*m_cSize)+x]	= (WORD)m_pDisplayMap[y][x].dwSingleIdx;
		}
	}
}

void	CField::StoreObj(OBJ_INFO *obj)
{
	/*
	DWORD	count	= 0;
	for ( DWORD i = 0; i < MAX_OBJ; i ++ )
	{
		if ( m_Object[i].m_bIsLoaded )
		{
			obj[count].wIndex	= (WORD)m_Object[i].m_dwIndex;
			obj[count].matPos	= m_Object[i].GetPosTM();
			count	++;
		}
	}
	*/
}

void	CField::StoreNpc(NPC_INFO* npc)
{
/*	DWORD	count	= 0;
	for ( DWORD i = 0; i < MAX_NPC; i ++ )
	{
		if ( m_Npc[i].m_bIsLoaded )
		{
			npc[count].wIndex	= (WORD)m_Npc[i].m_dwIndex;
			npc[count].matPos	= m_Npc[i].GetPosTM();
			count	++;
		}
	}
	*/
}

void	CField::StoreAttr(attr* attr)
{
	CalculateAttrHeight();
	for ( DWORD y = 0; y < m_cSize*2; y ++ )
	{
		for ( DWORD x = 0; x < m_cSize*2; x ++ )
		{
			attr[(y*(m_cSize*2))+x].wAttr	= (WORD)m_pAttrMap[y][x].byAttr;
			attr[(y*(m_cSize*2))+x].wType	= 0;
			attr[(y*(m_cSize*2))+x].fHeight	= (float)m_pAttrMap[y][x].nHeight;
		}
	}
}


/* for server file */
void	CField::StoreBlock(Block* block)
{
	DWORD	index	= 0;
	// store 1f
	for ( DWORD y = 0; y < m_cSize*2; y ++ )
	{
		for ( DWORD x = 0; x < m_cSize*2; x ++ )
		{
			index	= (y*(m_cSize*2))+x;
			switch ( m_pAttrMap[y][x].byAttr )
			{
			case	MATTR_NONE:
				block[index].m_is_blocked	= false;
				break;
			case	MATTR_OBJ:
				block[index].m_is_blocked	= true;
				break;
//			case	MATTR_2F:
//				block[index].m_to			= 1;
//				break;
			case	MATTR_WARP:
				block[index].m_is_linked			= true;
				block[index].m_Warp.m_My_ID			= 0;
				block[index].m_Warp.m_Map_ID		= m_WarpInfo[0].nDestMapID;
				block[index].m_Warp.m_Map_Warp		= m_WarpInfo[0].nDestWarpIdx;
				break;
			case	MATTR_WARP2:
				block[index].m_is_linked			= true;
				block[index].m_Warp.m_My_ID			= 1;
				block[index].m_Warp.m_Map_ID		= m_WarpInfo[1].nDestMapID;
				block[index].m_Warp.m_Map_Warp		= m_WarpInfo[1].nDestWarpIdx;
				break;
			case	MATTR_WARP3:
				block[index].m_is_linked			= true;
				block[index].m_Warp.m_My_ID			= 2;
				block[index].m_Warp.m_Map_ID		= m_WarpInfo[2].nDestMapID;
				block[index].m_Warp.m_Map_Warp		= m_WarpInfo[2].nDestWarpIdx;
				break;
			case	MATTR_WARP4:
				block[index].m_is_linked			= true;
				block[index].m_Warp.m_My_ID			= 3;
				block[index].m_Warp.m_Map_ID		= m_WarpInfo[3].nDestMapID;
				block[index].m_Warp.m_Map_Warp		= m_WarpInfo[3].nDestWarpIdx;
				break;
			case	MATTR_WARP5:
				block[index].m_is_linked			= true;
				block[index].m_Warp.m_My_ID			= 4;
				block[index].m_Warp.m_Map_ID		= m_WarpInfo[4].nDestMapID;
				block[index].m_Warp.m_Map_Warp		= m_WarpInfo[4].nDestWarpIdx;
				break;
			case	MATTR_WARP6:
				block[index].m_is_linked			= true;
				block[index].m_Warp.m_My_ID			= 5;
				block[index].m_Warp.m_Map_ID		= m_WarpInfo[5].nDestMapID;
				block[index].m_Warp.m_Map_Warp		= m_WarpInfo[5].nDestWarpIdx;
				break;
			case	MATTR_WARP7:
				block[index].m_is_linked			= true;
				block[index].m_Warp.m_My_ID			= 6;
				block[index].m_Warp.m_Map_ID		= m_WarpInfo[6].nDestMapID;
				block[index].m_Warp.m_Map_Warp		= m_WarpInfo[6].nDestWarpIdx;
				break;
			case	MATTR_WARP8:
				block[index].m_is_linked			= true;
				block[index].m_Warp.m_My_ID			= 7;
				block[index].m_Warp.m_Map_ID		= m_WarpInfo[7].nDestMapID;
				block[index].m_Warp.m_Map_Warp		= m_WarpInfo[7].nDestWarpIdx;
				break;
			case	MATTR_WARP9:
				block[index].m_is_linked			= true;
				block[index].m_Warp.m_My_ID			= 8;
				block[index].m_Warp.m_Map_ID		= m_WarpInfo[8].nDestMapID;
				block[index].m_Warp.m_Map_Warp		= m_WarpInfo[8].nDestWarpIdx;
				break;
			}
		}
	}
}

void	CField::StoreWarp(FILEWARP* warp)
{
	for ( DWORD i = 0; i < 9; i ++ )
	{
		warp[i].nDestMapID		= m_WarpInfo[i].nDestMapID;
		warp[i].nDestWarpIdx	= m_WarpInfo[i].nDestWarpIdx;
	}
}



/****************************************************************************************************
	* end of FILE
****************************************************************************************************/
