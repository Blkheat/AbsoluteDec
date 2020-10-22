#ifndef __EFFITEMMANAGER_H
#define __EFFITEMMANAGER_H

#pragma warning(disable:4786)
#include <windows.h>
#include <map>
#include "Model.H"
#include "animation.h"
#include "texture.h"

using namespace std;

#define EFFITEM_DATA_FILE		"EffItem.dat"
#define NEW_EFFITEMTYPE

#pragma pack(push,1)
struct SEffItemInfo
{
	DWORD	nCode;
#ifdef NEW_EFFITEMTYPE
	BYTE	isVtxAni;
	BYTE	isSemiTrans;
	BYTE	isMotionSync;
#endif
	
	BYTE	isChange;
	BYTE	isEmblem;
	DWORD	ModelCode;
	DWORD	TexCode;
	
#ifdef NEW_EFFITEMTYPE
	DWORD	AniCode;
#endif

	SEffItemInfo()
	{
		nCode = 0;
#ifdef NEW_EFFITEMTYPE
		isVtxAni = 0;
		isSemiTrans = 0;
		isMotionSync = 0;
#endif
		isChange = 0;
		isEmblem = 0;
		ModelCode = 0;
		TexCode = 0;
		
#ifdef NEW_EFFITEMTYPE
		AniCode = 0;
#endif
	}
};
#pragma pack(pop)

struct SModelList
{
	Model *pModel;
	Model *pMoveModel;

	SModelList()
	{
		pModel = NULL;
		pMoveModel = NULL;
	}
};

struct SEItemTexture
{
	DWORD	dTexCode;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 lpTexture;
#else
	LPDIRECT3DTEXTURE8 lpTexture;
#endif // DIRECT_VERSION_9_MJH
	long	lPrevTime;

	SEItemTexture()
	{
		dTexCode = 0;
		lpTexture = NULL;
		lPrevTime = 0;	
	}
};

typedef map < DWORD, SEffItemInfo > ItemInfoList;
typedef map < DWORD, SEffItemInfo >::iterator ItemInfoListPos;

typedef map < DWORD, SModelList > ModelList;
typedef map < DWORD, SModelList >::iterator ModelListPos;

typedef map < DWORD, SEItemTexture > TexList;
typedef map < DWORD, SEItemTexture >::iterator TexListPos;

typedef map < DWORD, Bone * > AniList;
typedef map < DWORD, Bone * >::iterator AniListPos;

class CEffItemManager
{
public:	 
	 CEffItemManager();
	~CEffItemManager();

	BOOL LoadData(void);

	void ClearData();

	SEffItemInfo FindItemInfo(DWORD nCode);
	SModelList FindModelList(DWORD ModelCode);
	SModelList FindModelList(SEffItemInfo & sEItmInfo);
	Bone * FineAniList(DWORD AniCode);
	Bone * FineAniList(SEffItemInfo & sEItmInfo);
	void SetTexture(DWORD dTexCode);
	SEItemTexture * FindEItemTexture(SEffItemInfo & sEItmInfo);

	void	DeleteNotUsedTex();							// 쓰지 않는 텍스쳐 지우기
	void	AllDeleteUsedTex();

protected:
	ItemInfoList m_ItemInfoList;
	ModelList m_ModelList;
	TexList m_TexList;
	AniList m_AniList;
};

extern CEffItemManager g_EffItemManager;

#endif
