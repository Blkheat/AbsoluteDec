#ifndef __CBILLBOARDLINE_H
#define __CBILLBOARDLINE_H

#include <RockPCH.h>

#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

typedef struct 
{

    D3DXVECTOR3 pos;
	float		u,v;			

}BILLBOARD_LINE_VERTEX;

#define D3DFVF_BILLBOARD_LINE_VERTEX		(D3DFVF_XYZ|D3DFVF_TEX1)

class CBillboardLine
{
public:
	 
	 CBillboardLine();
	~CBillboardLine();

public:

#ifdef DIRECT_VERSION_9_MJH
	int Render(LPDIRECT3DDEVICE9 pDevice);
	int InitDevice(LPDIRECT3DDEVICE9 pDevice);
#else
	int Render(LPDIRECT3DDEVICE8 pDevice);
	int InitDevice(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	//...............................................................................................................................
	// v0 ~ v1 ∫Ù∫∏µÂ √‡ , ≥–¿Ã , ƒ´∏ﬁ∂Û πÊ«‚ ∫§≈Õ
	//...............................................................................................................................
#ifdef DIRECT_VERSION_9_MJH
	int Update(D3DXVECTOR3 v0,D3DXVECTOR3 v1,float fWidth,D3DXVECTOR3 vEyeDir,LPDIRECT3DDEVICE9 pDevice);
#else
	int Update(D3DXVECTOR3 v0,D3DXVECTOR3 v1,float fWidth,D3DXVECTOR3 vEyeDir,LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
		
private:
	
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9      m_pIB;
#else
	LPDIRECT3DVERTEXBUFFER8		m_pVB;
	LPDIRECT3DINDEXBUFFER8      m_pIB;
#endif // DIRECT_VERSION_9_MJH
	
};

#endif