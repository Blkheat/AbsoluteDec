///////////////////////////////////////////////////////////////////////////////
///
///		File		: Fontg.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-06
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __FONTG_H__
#define __FONTG_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9.h>
#else
#include <d3dx8.h>
#endif // DIRECT_VERSION_9_MJH

#include "Define.h"
#include "Fontasm.h"



#define		MAX_FONTASM			65536
#define		MAX_INDEX_BUFFER	1024






class CFontg
{
public:
	CFontg(){}	
	CFontg( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco, bool ab_smooth , int al_width );
	~CFontg();
#ifdef DIRECT_VERSION_9_MJH
	void Out( TCHAR* Str, int x, int y, D3DCOLOR Color, LPDIRECT3DDEVICE9 D3DDevice );
#else
	void Out( TCHAR* Str, int x, int y, D3DCOLOR Color, LPDIRECT3DDEVICE8 D3DDevice );
#endif // DIRECT_VERSION_9_MJH
	bool Existence( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco, bool ab_smooth , int al_width );

	//BY Simwoosung - 실제 글자너비로 계산하는함수
	void GetRIStringInfo(TCHAR* Str, SStringInfo* StrInfo);
	
	void GetStringInfo( void* Str, SStringInfo* StrInfo );
	void GetStringInfo( void* Str, SStringInfo* StrInfo, int MaxWidth );
	void GetStringInfo( void* Str, SStringInfo* StrInfo, int MaxWidth, bool n );

	void GetStringDistribute( TCHAR* LineStr, TCHAR* DescStr );
	int GetStringCount( TCHAR* Str );
	
	int	GetPostoIndex( TCHAR* Str, int x, int y );
	int DetectionDBCS( int* Count, char* Str );

	int  GetRefCount();
	void AddRef();
	void Release();

	int Confirm( void* Str, FONTID* Buf );
	SFont* GetFontasm( int Idx );

	TCHAR* GetName();
	int GetSize();
	NFontWeights GetWeight();
	NFontDecoration GetDecoration();
	NFontCharset GetCharset();

private:

	TCHAR			theName[MAX_FONTNAME];
	NFontType		theFontType;
	int				theSize;
	NFontWeights	theWeight;
	NFontDecoration	theDeco;
	NFontCharset	theCharset;
	bool			theSmooth;
	int				theFontWidth;

	SFont*			theFontasm[MAX_FONTASM];

	int				theRefCount;
};



//-----------------------------------------------------------------------------
#endif	__FONTG_H__