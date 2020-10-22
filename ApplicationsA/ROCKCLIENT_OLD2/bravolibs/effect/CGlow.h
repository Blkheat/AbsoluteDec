//----------------------------------------------------------------------------------------------------------------------
// ���ϸ� : CGlow.h
// ��  �� : �ǻ���. �⺻ ���� �ؽ�ó ũ�Ⱑ 1024 x 1024 �̱⿡ �ؽ�ó Caps�� MaxTextureWidth�� MaxTextureHeight �� �߿�
//			�ϳ��� 1024���� ������ m_bEnable�� false
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

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

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

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

#ifdef DIRECT_VERSION_9_MJH
	int Process(LPDIRECT3DDEVICE9 pDevice,DWORD dwShader);// ����� ǥ���� ��ҿ� ǥ�鿡 ����. Rander() ���� ���� ȣ��
	int Render(LPDIRECT3DDEVICE9 pDevice);
#else
	int Process(LPDIRECT3DDEVICE8 pDevice,DWORD dwShader);// ����� ǥ���� ��ҿ� ǥ�鿡 ����. Rander() ���� ���� ȣ��
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
	LPDIRECT3DSURFACE9		m_pBackBuffer;// �����
	LPDIRECT3DTEXTURE9		m_pBackTexture;// ����� ����� �ؽ�ó
	LPDIRECT3DSURFACE9		m_pBackSurface;// ����� ����� ǥ��

	LPDIRECT3DTEXTURE9		m_pTex[GLOW_TEX_COUNT];// ��� �ؽ�ó
	LPD3DXRENDERTOSURFACE	m_pRenderToSurface[GLOW_TEX_COUNT];// ǥ�鿡�� �������� ���μ����� ����ȭ�ϱ� ���ؼ�(������) ����Ѵ�. 
	LPDIRECT3DSURFACE9		m_pSurface[GLOW_TEX_COUNT];//��� ǥ��
#else
	LPDIRECT3DSURFACE8		m_pBackBuffer;// �����
	LPDIRECT3DTEXTURE8		m_pBackTexture;// ����� ����� �ؽ�ó
	LPDIRECT3DSURFACE8		m_pBackSurface;// ����� ����� ǥ��

	LPDIRECT3DTEXTURE8		m_pTex[GLOW_TEX_COUNT];// ��� �ؽ�ó
	LPD3DXRENDERTOSURFACE	m_pRenderToSurface[GLOW_TEX_COUNT];// ǥ�鿡�� �������� ���μ����� ����ȭ�ϱ� ���ؼ�(������) ����Ѵ�. 
	LPDIRECT3DSURFACE8		m_pSurface[GLOW_TEX_COUNT];//��� ǥ��
#endif // DIRECT_VERSION_9_MJH	

	VERTEX_GLOW_2D			m_VertexList[GLOW_TEX_COUNT][4];// ��� ���ؽ� FVF

	int						m_GlowSizeTable[GLOW_TEX_COUNT];// 64, 128, 256 ���� ȿ�� ���� ��� ������

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	m_pVB[GLOW_TEX_COUNT];// ��� ���ؽ�
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	LPDIRECT3DVERTEXBUFFER9	m_pBackVB;// ����� ���ؽ�
#else
	LPDIRECT3DVERTEXBUFFER8	m_pVB[GLOW_TEX_COUNT];// ��� ���ؽ�
	LPDIRECT3DINDEXBUFFER8	m_pIB;

	LPDIRECT3DVERTEXBUFFER8	m_pBackVB;// ����� ���ؽ�
#endif // DIRECT_VERSION_9_MJH
	
	BOOL					m_ISSetting;// Init() ����?

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)

	bool					m_bEnable;// 1024 x 1024 ũ���� �ؽ�ó�� ������ �� �ִ°�? false�� ��Ȱ��

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/13)
};

#endif