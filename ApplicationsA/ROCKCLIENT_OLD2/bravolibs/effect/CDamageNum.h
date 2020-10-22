#ifndef CDAMAGENUM_H
#define CDAMAGENUM_H

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

}HITNUM_VERTEX;

#define D3DFVF_HITNUM_VERTEX			(D3DFVF_XYZ|D3DFVF_TEX1)

const 
		float HitNum_Width( 1.0f );
const
		float HitNum_Height( 1.0f );
const
		float HitNum_Range( HitNum_Width + HitNum_Width / 1.0f );

class CHitNum
{
public:

	  CHitNum() {}
	 ~CHitNum() {}

public:

	int Update(float Time);

public:

	Character		*m_pCharacter;
	
	D3DXVECTOR3		m_vVelocity;
	D3DXVECTOR3		m_vLocation;
	D3DXVECTOR3		m_vPrevLocation;

	D3DXVECTOR3		m_vGravity;
	D3DXVECTOR3		m_vGravityDelta;

	D3DXCOLOR		m_Color;
	D3DXCOLOR		m_ColorDelta;

	D3DXMATRIX		m_matLocal;

	float			m_Size;
	float			m_SizeDelta;

	float		    m_LifeTime;				
	float		    m_NowTime;				

	std::vector		<int>	m_NumList;
};

class CDamageNum
{
public:
	 
	 CDamageNum();
	~CDamageNum();

public:

	int	 Add(D3DXVECTOR3 vPos,DWORD Damage,int DamageLevel, Character * pCharacter );
	int  Add(Character *pSrc,DWORD Damage,BYTE Critical);
	
#ifdef DIRECT_VERSION_9_MJH
	int Render(LPDIRECT3DDEVICE9 pDevice,D3DXMATRIX matView);
#else
	int Render(LPDIRECT3DDEVICE8 pDevice,D3DXMATRIX matView);
#endif // DIRECT_VERSION_9_MJH
		int Update(float Time);

public:

#ifdef DIRECT_VERSION_9_MJH
	int InitDevice(LPDIRECT3DDEVICE9 pDevice);
#else
	int InitDevice(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	std::list		<CHitNum>	m_HitNumList;

public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9			m_pTexture;
	LPDIRECT3DVERTEXBUFFER9		m_pVB[10];
	LPDIRECT3DINDEXBUFFER9      m_pIB;
#else
	LPDIRECT3DTEXTURE8			m_pTexture;
	LPDIRECT3DVERTEXBUFFER8		m_pVB[10];
	LPDIRECT3DINDEXBUFFER8      m_pIB;
#endif // DIRECT_VERSION_9_MJH

};

#endif