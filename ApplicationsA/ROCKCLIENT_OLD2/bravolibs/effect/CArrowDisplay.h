#ifndef __CARROWDISPLAY_H
#define __CARROWDISPLAY_H

#include "RockPCH.h"
#include "..\\Obj\\Character.h"
#include "EffectID.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

//extern CRockClient			g_RockClient;

class CArrowDisplay
{
public:
	 
	 CArrowDisplay();
	~CArrowDisplay();

public:

	 void DeleteAllMemory(void);
#ifdef DIRECT_VERSION_9_MJH
	 int  Init(LPDIRECT3DDEVICE9 pDevice);
	 int  Render(LPDIRECT3DDEVICE9 pDevice);
	 LPDIRECT3DTEXTURE9	m_pTexture;
#else
	 int  Init(LPDIRECT3DDEVICE8 pDevice);
	 int  Render(LPDIRECT3DDEVICE8 pDevice);
	 LPDIRECT3DTEXTURE8	m_pTexture;
#endif // DIRECT_VERSION_9_MJH

	 Model			   *m_pModel;
};


#endif