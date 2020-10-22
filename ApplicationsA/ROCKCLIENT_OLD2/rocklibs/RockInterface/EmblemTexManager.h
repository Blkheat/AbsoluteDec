#ifndef __EMBLEMTEXMANAGER_H__
#define __EMBLEMTEXMANAGER_H__
//-----------------------------------------------------------------------------
#pragma warning(disable:4786)

#ifdef DIRECT_VERSION_9_MJH
#include <d3d9.h>
#else
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH
#include <map>

#include "..\\..\\bravolibs\effect\CRenderSurface.h"

using namespace std;

struct VESIONTEX
{
	BYTE							bEmblemVer;
	BYTE							bWorldIndex;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9				lpTexture;
#else
	LPDIRECT3DTEXTURE8				lpTexture;
#endif // DIRECT_VERSION_9_MJH

	CRenderSurface					*pRenderTexture;
	
	VESIONTEX()
	{
		bEmblemVer     = 0;
		bWorldIndex    = 0;
		lpTexture      = NULL;	
		pRenderTexture = NULL;
	}	
};

typedef map <DWORD , VESIONTEX *> EmblemTexList;
typedef map <DWORD , VESIONTEX *>::iterator EmblemTexListPos;

class CEmblemTexManager
{
private:
	EmblemTexList m_EmblemTexList;
	HANDLE		  m_hFileFind;

public:
	CEmblemTexManager();
	virtual ~CEmblemTexManager();

	void Clear();
	void LoadEmblemFiles();
	void LoadEmblemFilesW();
	void LoadEmblemFilesA();
	void ReadEmbFile( char * file_path, char * file_Name);
	void ConFirmEmblemVersion( DWORD dFrontierID, BYTE bEmblemVer ); 
#ifdef DECO_RENEWAL_GUILD_MJH
	void UpdateEmblenVersion( DWORD dFrontierID, BYTE itemIndex ,char * cEmblem);
#else	
	void UpdateEmblenVersion( DWORD dFrontierID, BYTE bVersion, 
		    				  WORD wEmblemSize, char * cEmblem );
#endif // DECO_RENEWAL_GUILD_MJH	
	VESIONTEX * FindForntierVersionTex(DWORD dFrontierID);
	void RenderFrontierImg(VESIONTEX * pVesionTex, int nx, int ny, int nw, int nh, float zPos);
	void RenderFrontierImg(VESIONTEX * pVesionTex, int nx, int ny, int nw, int nh);
	void DeleteEmblenFile(DWORD dFrontierID);
#ifdef DIRECT_VERSION_9_MJH
	int	 MakeTexture(VESIONTEX *pData,LPDIRECT3DTEXTURE9 pTex);
	LPDIRECT3DTEXTURE9	m_MEmbTexture;
	LPDIRECT3DTEXTURE9	m_REmbTexture;
#else
	int	 MakeTexture(VESIONTEX *pData,LPDIRECT3DTEXTURE8 pTex);
	LPDIRECT3DTEXTURE8	m_MEmbTexture;
	LPDIRECT3DTEXTURE8	m_REmbTexture;
#endif // DIRECT_VERSION_9_MJH
};

extern CEmblemTexManager g_EmblemTexManager;

//-----------------------------------------------------------------------------
#endif	__EMBLEMTEXMANAGER_H__