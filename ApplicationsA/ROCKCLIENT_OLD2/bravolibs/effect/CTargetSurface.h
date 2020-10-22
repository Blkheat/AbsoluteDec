#ifndef __CTARGETSURFACE_H
#define __CTARGETSURFACE_H

#include <RockPCH.h>


#ifdef DIRECT_VERSION_9_MJH
#include "d3d9.h"
#include "d3dx9math.h"
#else
#include "d3d8.h"
#include "d3dx8math.h"
#endif // DIRECT_VERSION_9_MJH

class CTargetSurface
{
public:
	
	 CTargetSurface();
	~CTargetSurface();

public:

#ifdef DIRECT_VERSION_9_MJH
	int Init(LPDIRECT3D9 pD3D,UINT Width,UINT Height,LPDIRECT3DDEVICE9 pDevice);
	int Start(LPDIRECT3DDEVICE9 pDevice);
	int End(LPDIRECT3DDEVICE9 pDevice);
	int Clear(LPDIRECT3DDEVICE9 pDevice);
	LPDIRECT3DTEXTURE9		GetTexture(void)		{ return m_pTexturePrj; }
#else
	int Init(LPDIRECT3D8 pD3D,UINT Width,UINT Height,LPDIRECT3DDEVICE8 pDevice);
	int Start(LPDIRECT3DDEVICE8 pDevice);
	int End(LPDIRECT3DDEVICE8 pDevice);
	int Clear(LPDIRECT3DDEVICE8 pDevice);
	LPDIRECT3DTEXTURE8		GetTexture(void)		{ return m_pTexturePrj; }
#endif // DIRECT_VERSION_9_MJH

	//by simwoosung
	HRESULT	InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT	RestoreDeviceObject(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT	RestoreDeviceObject(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

private:

	LPD3DXRENDERTOSURFACE   m_pRenderSurface;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	    m_pTexturePrj;
	LPDIRECT3DSURFACE9      m_pSurfacePrj;
	LPDIRECT3DSURFACE9      m_pOldSurface;
#else
	LPDIRECT3DTEXTURE8	    m_pTexturePrj;
	LPDIRECT3DSURFACE8      m_pSurfacePrj;
	LPDIRECT3DSURFACE8      m_pOldSurface;
#endif // DIRECT_VERSION_9_MJH
	
	UINT	m_Width;
	UINT	m_Height;
};

#endif