#ifndef __CFIXSIZEBILLBOARD_H
#define __CFIXSIZEBILLBOARD_H

#include <RockPCH.h>

#ifdef DIRECT_VERSION_9_MJH
#include <d3d9.h>
#include <d3dx9math.h>
#else
#include <d3d8.h>
#include <d3dx8math.h>
#endif // DIRECT_VERSION_9_MJH

typedef struct s_VERTEX_FIXSIZE_BILLBOARD
{
    D3DXVECTOR3 pos;
    FLOAT       tu,tv;

	s_VERTEX_FIXSIZE_BILLBOARD()
	{
		pos.z = 0.0f;
	}

}VERTEX_FIXSIZE_BILLBOARD;

#define D3DFVF_VERTEXT_FIXSIZE_BILLBOARD ( D3DFVF_XYZ|D3DFVF_TEX1 )

class CFixSizeBillboard
{
public:
	
	 CFixSizeBillboard();
	~CFixSizeBillboard();

#ifdef DIRECT_VERSION_9_MJH
	void Init(LPDIRECT3DDEVICE9 pDevice);
#else
	void Init(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	 void SetCamera(float Fov,D3DXVECTOR3 vEyePos,D3DXMATRIX matView);
	 
	 //By simwoosung
	 HRESULT	InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	 HRESULT	RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice);
#else
	 HRESULT	RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

public:

#ifdef DIRECT_VERSION_9_MJH
	void Draw(D3DXVECTOR3 vPos,float width,float height,D3DCOLORVALUE color,LPDIRECT3DDEVICE9 pDevice);
#else
	void Draw(D3DXVECTOR3 vPos,float width,float height,D3DCOLORVALUE color,LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

private:

	 float			m_fScale;
	 D3DXVECTOR3	m_vEyePos;
	 D3DXMATRIX		m_matRot;
	 D3DXMATRIX		m_matScale;
	 D3DXMATRIX		m_matWorld;
#ifdef DIRECT_VERSION_9_MJH
	 D3DMATERIAL9	m_mtrl;
	 LPDIRECT3DVERTEXBUFFER9	m_VB;
	 LPDIRECT3DINDEXBUFFER9		m_IB;
#else
	 D3DMATERIAL8	m_mtrl;
	 LPDIRECT3DVERTEXBUFFER8	m_VB;
	 LPDIRECT3DINDEXBUFFER8		m_IB;
#endif // DIRECT_VERSION_9_MJH
	 
};

#endif