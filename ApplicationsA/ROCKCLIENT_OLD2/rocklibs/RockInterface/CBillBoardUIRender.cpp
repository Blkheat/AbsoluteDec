// BillBoardUIRender.cpp: implementation of the CBillBoardUIRender class.
//
//////////////////////////////////////////////////////////////////////

#include "..\\RockPCH.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\camera.h"

#include "Define.h"
#include "Render.h"
#include "Rui.h"

#include "CBillBoardUIRender.h"
#include "..\\..\\CRenderManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CBravoCamera		g_Camera;


CBillBoardUIRender::CBillBoardUIRender()
{
   
}

CBillBoardUIRender::~CBillBoardUIRender()
{
	
}

HRESULT	 CBillBoardUIRender::InvalidateDeviceObject()
{
	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	 CBillBoardUIRender::RestoreDeviceObject(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	 CBillBoardUIRender::RestoreDeviceObject(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	return S_OK;
}

#ifdef DIRECT_VERSION_9_MJH
void CBillBoardUIRender::Init(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice, SPcInfo * pPcInfo)
#else
void CBillBoardUIRender::Init(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice, SPcInfo * pPcInfo)
#endif // DIRECT_VERSION_9_MJH
{

}

void CBillBoardUIRender::DInit()
{
    
}

void CBillBoardUIRender::BeginTexRender(D3DCOLOR cClColor)
{

}

void CBillBoardUIRender::EndTexRender()
{
   
}

void CBillBoardUIRender::RenderBillUi(int iPcInfoIndex, nBillUIType nUIType, float fBillHeight)
{
   
}

void CBillBoardUIRender::RenderIDOutBox(SStringInfo * pStrInfo, D3DCOLOR cOutColor)
{
	
}