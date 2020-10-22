//----------------------------------------------------------------------------------------------------------------------
// 파일명 : CGlow.h
// 용  도 : 뽀샤시. 기본 복사 텍스처 크기가 1024 x 1024 이기에 텍스처 Caps의 MaxTextureWidth와 MaxTextureHeight 둘 중에
//			하나라도 1024보다 작으면 m_bEnable은 false
//----------------------------------------------------------------------------------------------------------------------
#ifndef __CGLOW_H
#define __CGLOW_H

#include <RockPCH.h>

#ifdef DIRECT_VERSION_9_MJH
#include "d3d9.h"
#include "d3dx9math.h"
#else
#include "d3d8.h"
#include "d3dx8math.h"
#endif // DIRECT_VERSION_9_MJH


typedef struct s_VERTEX_GLOW_2D
{
    D3DXVECTOR4 pos;
    D3DCOLOR    color;
    FLOAT       tu0, tv0;

	s_VERTEX_GLOW_2D()
	{
		pos.z = 0.0f;
		pos.w = 1.0f;

		color = D3DCOLOR_ARGB( 0 , 255 , 255 , 255 );
	}

}VERTEX_GLOW_2D;

#define D3DFVF_VERTEXT_GLOW_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )

#define GLOW_TEX_COUNT 3

class CGlow
{
public:
	 
	 CGlow();
	~CGlow();

public:

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/13)

#ifdef DIRECT_VERSION_9_MJH
	int	Init(UINT Width,UINT Height,LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pDevice, bool bEnable = true);
#else
	int	Init(UINT Width,UINT Height,LPDIRECT3D8 pD3D,LPDIRECT3DDEVICE8 pDevice, bool bEnable = true);
#endif // DIRECT_VERSION_9_MJH

#else

#ifdef DIRECT_VERSION_9_MJH
	int	Init(UINT Width,UINT Height,LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pDevice);
#else
	int	Init(UINT Width,UINT Height,LPDIRECT3D8 pD3D,LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/13)

#ifdef DIRECT_VERSION_9_MJH
	int Process(LPDIRECT3DDEVICE9 pDevice,DWORD dwShader);// 백버퍼 표면을 축소용 표면에 복사. Rander() 보다 먼저 호출
	int Render(LPDIRECT3DDEVICE9 pDevice);
#else
	int Process(LPDIRECT3DDEVICE8 pDevice,DWORD dwShader);// 백버퍼 표면을 축소용 표면에 복사. Rander() 보다 먼저 호출
	int Render(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	int SetColor(D3DCOLOR Color);
	
	HRESULT	InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT RestoreDeviceObject(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pDevice);
	LPDIRECT3DTEXTURE9	GetBackTexture(void)	{ return m_pBackTexture;}
#else
	HRESULT RestoreDeviceObject(LPDIRECT3D8 pD3D, LPDIRECT3DDEVICE8 pDevice);
	LPDIRECT3DTEXTURE8	GetBackTexture(void)	{ return m_pBackTexture;}
#endif // DIRECT_VERSION_9_MJH

	D3DCOLOR			GetColor(void)			{ return m_Color; }


public:
	void DrawTexture( float x, float y, float w, float h, 
					  float tx, float ty, float tw, float th,
#ifdef DIRECT_VERSION_9_MJH
					  D3DCOLOR color , LPDIRECT3DDEVICE9 pDevice );
#else
					  D3DCOLOR color , LPDIRECT3DDEVICE8 pDevice );
#endif // DIRECT_VERSION_9_MJH

private:
	
	UINT					m_Width;
	UINT					m_Height;

	D3DCOLOR				m_Color;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSURFACE9		m_pBackBuffer;// 백버퍼
	LPDIRECT3DTEXTURE9		m_pBackTexture;// 백버퍼 복사용 텍스처
	LPDIRECT3DSURFACE9		m_pBackSurface;// 백버퍼 복사용 표면

	LPDIRECT3DTEXTURE9		m_pTex[GLOW_TEX_COUNT];// 축소 텍스처
	LPD3DXRENDERTOSURFACE	m_pRenderToSurface[GLOW_TEX_COUNT];// 표면에의 렌더링의 프로세스를 범용화하기 위해서(때문에) 사용한다. 
	LPDIRECT3DSURFACE9		m_pSurface[GLOW_TEX_COUNT];//축소 표면
#else
	LPDIRECT3DSURFACE8		m_pBackBuffer;// 백버퍼
	LPDIRECT3DTEXTURE8		m_pBackTexture;// 백버퍼 복사용 텍스처
	LPDIRECT3DSURFACE8		m_pBackSurface;// 백버퍼 복사용 표면

	LPDIRECT3DTEXTURE8		m_pTex[GLOW_TEX_COUNT];// 축소 텍스처
	LPD3DXRENDERTOSURFACE	m_pRenderToSurface[GLOW_TEX_COUNT];// 표면에의 렌더링의 프로세스를 범용화하기 위해서(때문에) 사용한다. 
	LPDIRECT3DSURFACE8		m_pSurface[GLOW_TEX_COUNT];//축소 표면
#endif // DIRECT_VERSION_9_MJH	

	VERTEX_GLOW_2D			m_VertexList[GLOW_TEX_COUNT][4];// 축소 버텍스 FVF

	int						m_GlowSizeTable[GLOW_TEX_COUNT];// 64, 128, 256 번짐 효과 위한 축소 사이즈

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	m_pVB[GLOW_TEX_COUNT];// 축소 버텍스
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	LPDIRECT3DVERTEXBUFFER9	m_pBackVB;// 백버퍼 버텍스
#else
	LPDIRECT3DVERTEXBUFFER8	m_pVB[GLOW_TEX_COUNT];// 축소 버텍스
	LPDIRECT3DINDEXBUFFER8	m_pIB;

	LPDIRECT3DVERTEXBUFFER8	m_pBackVB;// 백버퍼 버텍스
#endif // DIRECT_VERSION_9_MJH
	
	BOOL					m_ISSetting;// Init() 성공?

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/13)

	bool					m_bEnable;// 1024 x 1024 크기의 텍스처를 생성할 수 있는가? false면 비활성

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/13)
};

#endif