// Reflection.h: interface for the CReflection class.

//

//////////////////////////////////////////////////////////////////////



#if !defined(AFX_REFLECTION_H__90A8955A_8E3A_433E_B55E_835EDCA4DB39__INCLUDED_)
#define AFX_REFLECTION_H__90A8955A_8E3A_433E_B55E_835EDCA4DB39__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReflection  
{

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
	
	UINT m_Width;
	UINT m_Height;

public:

	CReflection();
	virtual ~CReflection();
public:
#ifdef DIRECT_VERSION_9_MJH
	int Init(LPDIRECT3D9 pD3D,UINT Width,UINT Height,LPDIRECT3DDEVICE9 pDevice);
#else
	int Init(LPDIRECT3D8 pD3D,UINT Width,UINT Height,LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void DInit();
#ifdef DIRECT_VERSION_9_MJH
	int Start(LPDIRECT3DDEVICE9 pDevice);
	int End(LPDIRECT3DDEVICE9 pDevice);
	int Clear(LPDIRECT3DDEVICE9 pDevice, D3DCOLOR cClColor);
#else
	int Start(LPDIRECT3DDEVICE8 pDevice);
	int End(LPDIRECT3DDEVICE8 pDevice);
	int Clear(LPDIRECT3DDEVICE8 pDevice, D3DCOLOR cClColor);
#endif // DIRECT_VERSION_9_MJH

	HRESULT	 InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT	 RestoreDeviceObject(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice);
	LPDIRECT3DTEXTURE9		GetTexture(void)		{ return m_pTexturePrj; }
#else
	HRESULT	 RestoreDeviceObject(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice);
	LPDIRECT3DTEXTURE8		GetTexture(void)		{ return m_pTexturePrj; }
#endif // DIRECT_VERSION_9_MJH

};



#endif // !defined(AFX_REFLECTION_H__90A8955A_8E3A_433E_B55E_835EDCA4DB39__INCLUDED_)











































