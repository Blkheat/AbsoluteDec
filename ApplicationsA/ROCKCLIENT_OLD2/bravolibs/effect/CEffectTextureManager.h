#ifndef __CEFFECTTEXTUREMANAGER_H
#define __CEFFECTTEXTUREMANAGER_H

#include "RockPCH.h"
#include "VFileSystem.h"
#include "quadlist.h"
#include <NETWORK\\Protocol.h>
#include <Base\\D3DBase.h>
#include <EFFECT\\Particle.h>
#include <Map\\Field.h>
#include "Map.h"
//#include "Effect.h"
#include "Camera.h"
#include <EFFECT\\EffectVertex.h>
#include <OBJ\\Object_Manager.H>
#include "brother.h"
#include <assert.h>
#include "RockClient.h"
#include "CStringMap.h"

// #define __EF_MANAGER_PROFILE

const WORD c_EffectDeleteTex_Update_Time( 30 );

typedef struct s_EffectTexture
{

#ifdef __EF_MANAGER_PROFILE		
	int ReferenceCount;
#endif

	string	KeyName;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	pTexture;
#else
	LPDIRECT3DTEXTURE8	pTexture;
#endif // DIRECT_VERSION_9_MJH
	long m_lPreTime;	

	s_EffectTexture()
	{
		pTexture = NULL;

#ifdef __EF_MANAGER_PROFILE	
	
		ReferenceCount = 0;

#endif
	}

	~s_EffectTexture()
	{
		if( NULL != pTexture )
		{
			pTexture->Release();

			pTexture = NULL;
		}
	}
	

}EFFECT_TEXTURE,*LPEFFECT_TEXTURE;


class CEffectTextureManager
{
public:
	
	 CEffectTextureManager();
	~CEffectTextureManager();

public:

	void				DeleteAllMemory(void);
	int					AllLoadFromDirectory(char *pPath);
	int					AllLoadFromVirtualFile(CVFileSystem *pVF);
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	LoadFile(char *pKeyName,char *pFileName);		
	LPDIRECT3DTEXTURE9	LoadVirtualFile(CVFileSystem *pVF,char *pFileName);	
	LPDIRECT3DTEXTURE9	GetTexture(char *pFileName);
	LPDIRECT3DTEXTURE9	GetTexture(unsigned long nHashCode);
#else
	LPDIRECT3DTEXTURE8	LoadFile(char *pKeyName,char *pFileName);		
	LPDIRECT3DTEXTURE8	LoadVirtualFile(CVFileSystem *pVF,char *pFileName);	
	LPDIRECT3DTEXTURE8	GetTexture(char *pFileName);
	LPDIRECT3DTEXTURE8	GetTexture(unsigned long nHashCode);
#endif // DIRECT_VERSION_9_MJH
	void				WriteProfile(void);

	HRESULT				InvalidateDeviceObjects();
	HRESULT				RestoreDeviceObjects();

	void DeleteNotUsedTex();

public:

	CStringMap<EFFECT_TEXTURE>		m_TexMap;
	long		m_lPreTime;			// 이전검색시간
	WORD		m_wDeleteCnt;
};

#endif