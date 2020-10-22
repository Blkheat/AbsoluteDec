#ifndef __CEXPDISPLAY_H
#define __CEXPDISPLAY_H

#include "RockPCH.h"
#include "..\\Obj\\Character.h"
#include <iostream>
#include <vector>
#include <list>
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

}EXPNUM_VERTEX;

#define D3DFVF_EXPNUM_VERTEX			(D3DFVF_XYZ|D3DFVF_TEX1)

const 
		float ExpNum_Width( 1.0f );
const
		float ExpNum_Height( 1.0f );
const
		float ExpNum_Range( ExpNum_Width + ExpNum_Width / 1.0f );


class CExpNum
{
public:
	 
	 CExpNum(); 
	~CExpNum() {}

public:
	
	std::vector	<int> m_NumList;

	BOOL Update(DWORD Time);

	float GetAlpha(void)	{ return m_fAlphaValue; }

public:

	D3DXVECTOR3 m_vLocation;
	BOOL	m_ISWaitTime;

private:
	
	DWORD	m_OldTimer;
	DWORD   m_ISFade;
	float   m_fAlphaValue;
	
};


class CExpDisplay
{
public:
	 
	 CExpDisplay();
	~CExpDisplay();

#ifdef DIRECT_VERSION_9_MJH
	void InitDevice(LPDIRECT3DDEVICE9 pDevice);
	void Update(LPDIRECT3DDEVICE9 pDevice);
#else
	void InitDevice(LPDIRECT3DDEVICE8 pDevice);
	void Update(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void Clear(void);
	void Add(int Num,D3DXVECTOR3 vPos);
#ifdef DIRECT_VERSION_9_MJH
	void Render(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView);
#else
	void Render(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView);
#endif // DIRECT_VERSION_9_MJH

private:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		m_pTexture;
#else
	LPDIRECT3DTEXTURE8		m_pTexture;
#endif // DIRECT_VERSION_9_MJH	
	unsigned long			m_HashCode;

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	m_pVB[ 11 ];
	LPDIRECT3DINDEXBUFFER9  m_pIB;
#else
	LPDIRECT3DVERTEXBUFFER8	m_pVB[ 11 ];
	LPDIRECT3DINDEXBUFFER8  m_pIB;
#endif // DIRECT_VERSION_9_MJH

	std::list		<CExpNum>	m_ExpNumList;
};

#endif

