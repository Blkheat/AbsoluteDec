#ifndef __CRENDERSURFACE_H
#define __CRENDERSURFACE_H
#include <RockPCH.h>

#ifdef DIRECT_VERSION_9_MJH
#include "d3d9.h"
#include "d3dx9math.h"
#else
#include "d3d8.h"
#include "d3dx8math.h"
#endif // DIRECT_VERSION_9_MJH

class CRenderSurface
{
public:
	 
	 CRenderSurface();
	~CRenderSurface();

public:

#ifdef DIRECT_VERSION_9_MJH
	int		Init(UINT Width,UINT Height,LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9	pDevice);
#else
	int		Init(UINT Width,UINT Height,LPDIRECT3D8 pD3D,LPDIRECT3DDEVICE8	pDevice);
#endif // DIRECT_VERSION_9_MJH

	UINT	GetWidth(void)						{ return m_Width;  }
	UINT	GetHeight(void)						{ return m_Height; }
	
	BOOL	GetReady(void)						{ return m_ISInit; }

#ifdef DIRECT_VERSION_9_MJH
	HRESULT BeginScene(LPDIRECT3DSURFACE9  pSurface);
#else
	HRESULT BeginScene(LPDIRECT3DSURFACE8  pSurface);
#endif // DIRECT_VERSION_9_MJH
	HRESULT BeginScene(void);
	HRESULT EndScene(void);
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		GetTexture(void)
#else
	LPDIRECT3DTEXTURE8		GetTexture(void)
#endif // DIRECT_VERSION_9_MJH	
	{ 
		if( !GetReady() )
			return NULL;
		
		return m_pTexture; 
	}
	
	void operator=(const CRenderSurface &temp)
	{
		m_pTexture			=	temp.m_pTexture;
		m_pRenderToSurface	=	temp.m_pRenderToSurface;
		m_pSurface			=	temp.m_pSurface;

		m_ISInit			=	temp.m_ISInit;

		m_Width				=	temp.m_Width;
		m_Height			=	temp.m_Height;
	}

private:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		m_pTexture;
	LPDIRECT3DSURFACE9		m_pSurface;
#else
	LPDIRECT3DTEXTURE8		m_pTexture;
	LPDIRECT3DSURFACE8		m_pSurface;
#endif // DIRECT_VERSION_9_MJH
	LPD3DXRENDERTOSURFACE	m_pRenderToSurface;	

	BOOL					m_ISInit;

	UINT					m_Width;
	UINT					m_Height;

private:

	void	DeleteMemory(void);



};

#endif
