//#include "stdafx.h"
#include "RockPCH.h"
#include "..\\..\\RockClient.h"

#include "base\\d3dapp.h"
#include "base\\dxutil.h"
#include "d3dfvf.h"
#include "d3drenderobj.h"

#include "..\\obj\\character.h"
#include "..\\..\\CRenderManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

CRenderObj::CRenderObj()
{
	m_pd3dDevice	= NULL;
	m_pVB	= NULL;
	m_pIB	= NULL;
}

HRESULT	CRenderObj::Error(const char* strError)
{
	MessageBox(NULL, strError, "CRenderObj", MB_OK);
	return	E_FAIL;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CRenderObj::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev)
#else
HRESULT	CRenderObj::InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev)
#endif // DIRECT_VERSION_9_MJH
{
	m_pd3dDevice	= pd3ddev;
	D3DXMatrixIdentity(&m_matPos);

	if ( m_pd3dDevice )
		return	S_OK;

	return	E_FAIL;
}

HRESULT	CRenderObj::RestoreDeviceObjects()
{
	return	S_OK;
}

HRESULT	CRenderObj::InvalidateDeviceObjects()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);

	return	S_OK;
}

HRESULT	CRenderObj::DeleteDeviceObjects()
{
	m_pd3dDevice	= NULL;
	return	S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CRenderBound::CRenderBound()
{
	m_pDebugVB = NULL;
	m_bIsReady = false; 
}
CRenderBound::~CRenderBound()
{
	m_pDebugVB = NULL;
	m_bIsReady = false; 
}
//	Rendering Bounding Area
#ifdef DIRECT_VERSION_9_MJH
HRESULT	CRenderBound::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDev)
#else
HRESULT	CRenderBound::InitDeviceObjects(LPDIRECT3DDEVICE8 pd3dDev)
#endif // DIRECT_VERSION_9_MJH
{
	SAFE_RELEASE(m_pDebugVB);
	return	CRenderObj::InitDeviceObjects(pd3dDev);
}

HRESULT	CRenderBound::RestoreDeviceObjects()
{
	m_dwFVF	= D3DFVF_PD;
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(24*sizeof(FVF_PD), D3DUSAGE_DYNAMIC, m_dwFVF, D3DPOOL_SYSTEMMEM, &m_pVB) ) )
	{
		return	Error("Create error CRenderBound::m_pVB");
	}
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(12*sizeof(FVF_PD), D3DUSAGE_DYNAMIC, m_dwFVF, D3DPOOL_SYSTEMMEM, &m_pDebugVB) ) )
	{
		return	Error("Create error CRenderBound::m_pDebugVB");
	}
	return	S_OK;
		
}

HRESULT	CRenderBound::Update(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	m_vMin = min;
	m_vMax = max;

	FVF_PD	*	pVertices;

	m_nIdxCount	= m_nVtxCount	= 0;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0,0,(void**)&pVertices, 0) ) )
#else
	if ( FAILED ( m_pVB->Lock(0,0,(BYTE**)&pVertices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock VB");
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,min.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,max.z);
	pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

	m_pVB->Unlock();

	m_bIsReady	= true;

	return	S_OK;
}

HRESULT	CRenderBound::Render(const D3DXMATRIX& matPos)
{
	if ( m_pd3dDevice == NULL || m_pVB == NULL || m_pDebugVB == NULL || !m_bIsReady )
		return	E_FAIL;

	g_RenderManager.SetLighting( FALSE );

	D3DXMATRIX matTemp = matPos;

	g_RenderManager.SetTransform(D3DTS_WORLD, &matTemp);
	g_RenderManager.SetTexture(0, NULL);
	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(FVF_PD));
	
	g_RenderManager.SetFVF(m_dwFVF);
	
	g_RenderManager.DrawPrimitive(D3DPT_LINELIST, 0, 12);

	return g_RenderManager.SetLighting( TRUE );
}

HRESULT CRenderBound::RenderBox(void * pCharacter)
{

	if(pCharacter == NULL)
	{
		return S_OK;
	}
	
	if( NULL == g_lpDevice )
	{
		return S_OK;
	}
	
	//By simwoosung
	D3DXMATRIX			TempMat;
	D3DXMatrixIdentity( &TempMat );
	g_RenderManager.SetTransform( D3DTS_WORLD, &TempMat );
	
	Character * pc = (Character *)pCharacter;
	D3DXVECTOR3 min = pc->m_minBoundVec;
	D3DXVECTOR3 max = pc->m_maxBoundVec;
	
	if( g_Map.IsGiantUnit( pc ) )
	{
		min = pc->GetCBoundMinPos();
		max = pc->GetCBoundMaxPos();
	}
	
	D3DXMATRIX		matWld;
	D3DXMATRIX		matpos;
	D3DXMATRIX		matrot;

	D3DXMatrixIdentity( &matpos );		
	D3DXMatrixIdentity( &matrot );
	
	matpos._41 = pc->GetPosTM()._41;
	matpos._42 = pc->GetPosTM()._42;
	matpos._43 = pc->GetPosTM()._43;
	
	D3DXMatrixRotationYawPitchRoll( &matrot,pc->m_Mov.now_Yrad, pc->m_Mov.now_Xrad, 0.0f );

	D3DXMatrixMultiply(&matWld, &matrot, &matpos);

	static FVF_PD	Vertices[24];

	int nVtxCount = 0;
	
	///--1
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,min.y,min.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,max.y,min.z);	
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

	///--2
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,max.y,min.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,max.y,min.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

	///--3
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,max.y,min.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,min.y,min.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	
	///--4
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,min.y,min.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,min.y,min.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

	///--5
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,min.y,max.z);	
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,max.y,max.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

	///--6
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,max.y,max.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,min.y,max.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	
	///--7
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,min.y,max.z);	
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,min.y,max.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,max.y,max.z);

	///--8
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,max.y,max.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

	///--9
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,min.y,min.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,min.y,max.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	
	///--10
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,max.y,min.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(min.x,max.y,max.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	
	///--11
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,min.y,min.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,min.y,max.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,max.y,min.z);

	///-- 12
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
	Vertices[nVtxCount].v		= D3DXVECTOR3(max.x,max.y,max.z);
	D3DXVec3TransformCoord( &Vertices[nVtxCount].v, &Vertices[nVtxCount].v, &matWld );
	Vertices[nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

	g_RenderManager.SetLighting( FALSE );
	
	g_RenderManager.SetTexture(0, NULL);
	
	DWORD dFvF = g_RenderManager.GetFVF();	
	g_RenderManager.SetFVF(D3DFVF_PD);
	g_RenderManager.DrawPrimitiveUP(D3DPT_LINELIST, 12, Vertices, sizeof(FVF_PD));
	g_RenderManager.SetLighting( TRUE );
	g_RenderManager.SetFVF(dFvF);
	
	return S_OK;
}

HRESULT	CRenderBound::InvalidateDeviceObjects()
{
	return	CRenderObj::InvalidateDeviceObjects();
}

HRESULT	CRenderBound::DeleteDeviceObjects()
{
	return	CRenderObj::DeleteDeviceObjects();
}

HRESULT	CRenderBound::FinalCleanup()
{
	InvalidateDeviceObjects();
	DeleteDeviceObjects();
	m_bIsReady	= false;
	return	S_OK;
}