///////////////////////////////////////////////////////////////////////////////
///
///		File		: Resource.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __RESOURCE_H__
#define __RESOURCE_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9.h>
#else
#include <d3dx8.h>
#endif // DIRECT_VERSION_9_MJH

#include "Define.h"
#include "DefineHID.h"
#include "..\\CSlangFilter.h"

const WORD c_UIDeleteTex_Update_Time( 30 );

/*
//애니메이션 포멧
enum enImageType{ itNormal, itAnimation };
struct stImage
{
	enImageType ImgT;
	
	int Frame;
//	stRuiPanel* Image;
};
//*/

#define G_HELPTIP_STR(a) (char &)(Resource.GetHelpTip(a))

class CResource
{
public:
	CResource(){}
#ifdef DIRECT_VERSION_9_MJH
	CResource( LPDIRECT3DDEVICE9 D3DDevice );
#else
	CResource( LPDIRECT3DDEVICE8 D3DDevice );
#endif // DIRECT_VERSION_9_MJH
	~CResource();

	
	STexture* GetTexture( TEXID Tid );
	SImage* GetImageInfo( int ImgId );
	TCHAR* GetNotice();

	SHelpTip* GetHelpTipInfo( HLPID Hid );
	
	SCursor* GetCursorInfo( NCursorType cursor_type );
	SDlgBorder* GetBorderInfo();

public:
	SImage		Img[MAX_IMG_AMOUNT];		///-- Dynamic Texture_M 을 위해 Public 으로
private:

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9       theD3DDevice;
#else
	LPDIRECT3DDEVICE8       theD3DDevice;
#endif // DIRECT_VERSION_9_MJH
	STexture*	Tex[MAX_TEX_AMOUNT];
	TCHAR		Notice[10000];
	
	SHelpTip	HelpTip[1024];

	SCursor		Cursor[20];
	SDlgBorder	Border[10];

	long		m_lPreTime;			// 이전검색시간
	WORD		m_wDeleteCnt;

	//Data File Load
	void LoadNoticeFile();
	void LoadTextureInfoFile();					///-- 초기화 때 텍스쳐를 로딩한다.
public:
	void LoadTextureDynamic( int a_nTID );		///-- 동적으로 텍스쳐를 로딩한다.
	void DestroyDynamic( int a_nTID );			///-- 동적으로 텍스쳐를 제거한다.
	
	void DeleteNotUsedTex();
private:
	void LoadImage( SImage* Img, TCHAR* FileName, int FS_W, int FS_H );

	void LoadHelpStringInfoFile();
	void LoadHelpStringInfoFileW();
	void LoadHelpItemInfoFile();
	void LoadCBT();
	
	
	const char *GetHelpTip(int Index);


	void LoadSlangFile();

	void SetTextureImageInfo( SImage* Img, int ImgId, int Tid, int X, int Y, int W, int H );
	void SetTextureFontInfo( SImage* Img, int ImgId, int Tid, int X, int Y, int W, int H );

	//Texture Load
#ifdef DIRECT_VERSION_9_MJH
	bool LoadTexture( LPDIRECT3DTEXTURE9* ppTexture, const TCHAR* FileName, bool bUseTransparency, D3DCOLOR TransparencyColor );
#else
	bool LoadTexture( LPDIRECT3DTEXTURE8* ppTexture, const TCHAR* FileName, bool bUseTransparency, D3DCOLOR TransparencyColor );
#endif // DIRECT_VERSION_9_MJH

	void Release();
};

extern CResource* Resource;
//---------------------------------------------------------------------------//
#endif  __RESOURCE_H__