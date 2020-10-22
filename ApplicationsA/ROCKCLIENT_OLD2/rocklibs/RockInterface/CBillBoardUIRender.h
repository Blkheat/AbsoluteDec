// BillBoardUIRender.h: interface for the CBillBoardUIRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BILLBOARDUIRENDER_H__6991656B_18AA_4F5F_A6CF_B5A218CDCB83__INCLUDED_)
#define AFX_BILLBOARDUIRENDER_H__6991656B_18AA_4F5F_A6CF_B5A218CDCB83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CReflection.h"

#define D3DFVF_VERTEXT_BILLBOARD ( D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1 )

typedef struct s_VERTEX_BILLBOARD
{
    D3DXVECTOR3 pos;
    D3DCOLOR    color;
    FLOAT       tu, tv;

	s_VERTEX_BILLBOARD()
	{
		pos.z = 0.0f;
		color = D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );	
	}

}VERTEX_BILLBOARD;

enum nBillUIType
{
   BILLGAMEUI = 0,
   BILLCAHROUT,
   BILLCAHRID,
   BILLDLGBLN,
   BILLDLGBTX,
};

class CBillBoardUIRender  
{
public:
    UINT						m_uiTexWidth;
	UINT						m_uiTexHeight; 

protected:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3D9					m_pD3D;
	LPDIRECT3DDEVICE9			m_pDevice;
#else
	LPDIRECT3D8					m_pD3D;
	LPDIRECT3DDEVICE8			m_pDevice;
#endif // DIRECT_VERSION_9_MJH
	CReflection				    m_Surface;
	SPcInfo*					m_pPcInfo;
	
public:
	CBillBoardUIRender();
	virtual ~CBillBoardUIRender();

public:
#ifdef DIRECT_VERSION_9_MJH
	void Init(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice, SPcInfo * pPcInfo);
#else
	void Init(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice, SPcInfo * pPcInfo);
#endif // DIRECT_VERSION_9_MJH
	void DInit();

	HRESULT	 InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT	 RestoreDeviceObject(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT	 RestoreDeviceObject(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	void BeginTexRender(D3DCOLOR cClColor);
	void EndTexRender();  
    void RenderIDOutBox(SStringInfo * pStrInfo, D3DCOLOR cOutColor);

	void RenderBillUi(int iPcInfoIndex, nBillUIType nUIType, float fBillHeight);	
};

#endif // !defined(AFX_BILLBOARDUIRENDER_H__6991656B_18AA_4F5F_A6CF_B5A218CDCB83__INCLUDED_)
