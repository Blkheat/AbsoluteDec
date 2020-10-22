#ifndef __CSPOTEFFECT_H
#define __CSPOTEFFECT_H

#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

typedef struct s_SPOT2DVERTEX
{
    D3DXVECTOR4 pos;
    D3DCOLOR    color;
    FLOAT       tu0, tv0;

	s_SPOT2DVERTEX()
	{
		pos.z = pos.w = 1.0f;

		color = D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
	}

}SPOT2DVERTEX;

#define D3DFVF_SPORT2DVERTEXT ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )

typedef enum 
{
	
	DISPLAY_NONE ,
	DISPLAY_PROCESS
	
}SPOT_EFFECT_STATE;

class CSpotEffect
{
public:
	
	 CSpotEffect();
	~CSpotEffect();

#ifdef DIRECT_VERSION_9_MJH
	int	 Init(LPDIRECT3DDEVICE9 pDevice);
#else
	int	 Init(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void SetStart(DWORD Time);

public:

#ifdef DIRECT_VERSION_9_MJH
	void Update(LPDIRECT3DDEVICE9 pDevice);
#else
	void Update(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

private:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		m_Tex;
#else
	LPDIRECT3DTEXTURE8		m_Tex;
#endif // DIRECT_VERSION_9_MJH
	 SPOT_EFFECT_STATE		m_State;
	 DWORD					m_OldTimer;
	 DWORD					m_RangeTimer;
	 SPOT2DVERTEX			m_VertexList[4];
};

#endif