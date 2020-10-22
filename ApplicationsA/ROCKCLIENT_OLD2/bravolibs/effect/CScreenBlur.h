#ifndef __CScreenBlur_H
#define __CScreenBlur_H

#include "RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include "d3d9.h"
#include "d3dx9math.h"
#else
#include "d3d8.h"
#include "d3dx8math.h"
#endif // DIRECT_VERSION_9_MJH
#include "CRenderSurface.h"
#include <iostream>
#include <vector>
#include <list>

using namespace	std;

typedef struct s_VERTEX_BLUR_2D
{
	enum { FVF = D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 };
    
	D3DXVECTOR4 pos;
    D3DCOLOR    color;
    FLOAT       tu0, tv0;

	s_VERTEX_BLUR_2D()
	{
		pos.z = 0.0f;
		pos.w = 1.0f;

		color = D3DCOLOR_ARGB( 0 , 255 , 255 , 255 );
	}

}VERTEX_BLUR_2D;

typedef struct s_BLUR_SURFACE
{
	CRenderSurface		   *pRenderSurface;
	BOOL					m_ISRendered;

	s_BLUR_SURFACE()
	{
		pRenderSurface     = NULL;
		m_ISRendered       = FALSE;
	}

	~s_BLUR_SURFACE()
	{
		SAFE_DELETE( pRenderSurface );
	}

}BLUR_SURFACE,*LPBLUR_SURFACE;


class CScreenBlur
{
public:
	 
	 CScreenBlur();
	~CScreenBlur();

public:

#ifdef DIRECT_VERSION_9_MJH
	int	Init(UINT Width,UINT Height,LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pDevice);
	int Process(LPDIRECT3DDEVICE9 pDevice,DWORD dwShader);
	int Render(LPDIRECT3DDEVICE9 pDevice);
#else
	int	Init(UINT Width,UINT Height,LPDIRECT3D8 pD3D,LPDIRECT3DDEVICE8 pDevice);
	int Process(LPDIRECT3DDEVICE8 pDevice,DWORD dwShader);
	int Render(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	
	HRESULT	InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT RestoreDeviceObject(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT RestoreDeviceObject(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

private:
	
	UINT							m_Width;
	UINT							m_Height;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSURFACE9				m_pBackBuffer;
	LPDIRECT3DTEXTURE9				m_pBackTexture;
	LPDIRECT3DSURFACE9				m_pBackSurface;
#else
	LPDIRECT3DSURFACE8				m_pBackBuffer;
	LPDIRECT3DTEXTURE8				m_pBackTexture;
	LPDIRECT3DSURFACE8				m_pBackSurface;
#endif // DIRECT_VERSION_9_MJH

	std::list<LPBLUR_SURFACE >		m_BlurSurfaceList;

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;
	LPDIRECT3DVERTEXBUFFER9			m_pBackVB;
#else
	LPDIRECT3DVERTEXBUFFER8			m_pVB;
	LPDIRECT3DINDEXBUFFER8			m_pIB;
	LPDIRECT3DVERTEXBUFFER8			m_pBackVB;
#endif // DIRECT_VERSION_9_MJH
	
	BOOL							m_ISSetting;

	int								m_SaveCount;
	BOOL							m_ISRenderOK;

	DWORD							m_dTimeList[10];
};

#endif