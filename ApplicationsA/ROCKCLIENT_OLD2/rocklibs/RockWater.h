///////////////////////////////////////////////////////////////////////////////////
///		File	: RockWater.h
///		Desc	: Horizontal  Water
///
///		Author	: Cho Hong-Seob
///		Team	: Program - Client Team
///		Date	: 2004-04-23
///
///		Copyright (c) 2004, RockSoft Co., Ltd. ALL RIGHTS RESERVED
///////////////////////////////////////////////////////////////////////////////////

#if !defined(__ROCKWATER_H_INCLUDED__)
#define __ROCKWATER_H_INCLUDED__
#include "RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3d9.h>
#include <d3dx9.h>
#else
#include <d3d8.h>
#include <d3dx8.h>
#endif // DIRECT_VERSION_9_MJH


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////////////////
///		*.orm file 구조 변경 
///	
///		# 기존 구조 
///		fread(&m_NewFile.byWaterTextureIndex	,sizeof(BYTE		),1,	fp);
///		fread(&m_NewFile.bShowWater				,sizeof(BOOL		),1,	fp);
///		fread(&m_NewFile.byWaterHeightVariance	,sizeof(BYTE		),1,	fp);
///		fread(&m_NewFile.fWaterBaseHeight		,sizeof(float		),1,	fp);
///		fread(&m_NewFile.byWaterRunsDir			,sizeof(BYTE		),1,	fp);
///	
///		#변경 
///		fread(&m_NewFile.byWaterTextureIndex	,sizeof(BYTE		),1,	fp);
///			-> 이 BYTE 값을 Water 갯수로 사용. 0 이면 물이 없는 것 
///			-> 뒤의 BOOL, BYTE, float, BYTE 값은 무시... 10BYTE 낭비네.. 나중에 한꺼번에 컨버팅 하자. 
///		Water 갯수에 따라 *.orm file 맨 뒤에 SRockWaterInfo 가 attach 된다. 
///		Water 갯수가 0 이라면 attach size도 0
///	
////////////////////////////////////////////////////////////////////////////////////////


enum	RW_RUNSDIR
{
	RW_RD_LEFTUP	= 0,
	RW_RD_UP,
	RW_RD_RIGHTUP,
	RW_RD_LEFT,
	RW_RD_NONE,
	RW_RD_RIGHT,
	RW_RD_LEFTDOWN,
	RW_RD_DOWN,
	RW_RD_RIGHTDOWN,
};

typedef struct _SRockWaterInfo
{
	float	theHeightBase;			/// 기준 높이 
	BYTE	theHeightVariance;		/// 출렁일 높이 
	BYTE	theRunDirs;				/// 흐르는 방향.			하위 4bit 0번 텍스쳐, 상위 4bit 1번 텍스쳐 
	BYTE	theRunSpeeds;			/// 흐르는 속도. 0 ~ 15		하위 4bit 0번 텍스쳐, 상위 4bit 1번 텍스쳐 
	BYTE	theTextureIndexs;		/// 텍스쳐 인덱스. 0 ~ 15	하위 4bit 0번 텍스쳐, 상위 4bit 1번 텍스쳐 
									/// 0xf 는 텍스쳐 지정되지 않은 것으로 처리 

	/// 물이 그려질 영역. 물 버텍스 간격은 16.0f -> 타일 크기. 영역도 cell 단위로 지정 
	/// left-bottom 기준 Cell 지정. 폭과 너비 지정 
	short	theLeftX;
	short	theBottomZ;
	short	theWidth;
	short	theHeight;
}SRockWaterInfo, * SRockWaterInfoPtr;

class CRockWater  
{
public:
	CRockWater();
	virtual ~CRockWater();

	/// Texture는 외부에서 미리 생성하여 포인터만 넘깁니다. 
#ifdef DIRECT_VERSION_9_MJH
	bool		Create( LPDIRECT3DDEVICE9 aD3DDevice, LPDIRECT3DTEXTURE9 aTex0, LPDIRECT3DTEXTURE9 aTex1, SRockWaterInfoPtr aWaterInfoPtr );
#else
	bool		Create( LPDIRECT3DDEVICE8 aD3DDevice, LPDIRECT3DTEXTURE8 aTex0, LPDIRECT3DTEXTURE8 aTex1, SRockWaterInfoPtr aWaterInfoPtr );
#endif // DIRECT_VERSION_9_MJH

	bool		Update();
#ifdef DIRECT_VERSION_9_MJH
	bool		Render( LPDIRECT3DDEVICE9 aD3DDevice );
#else
	bool		Render( LPDIRECT3DDEVICE8 aD3DDevice );
#endif // DIRECT_VERSION_9_MJH

	void		CleanUp();

	/// for editing with map tool --------------------------->
	void				ResetHeight();
	SRockWaterInfoPtr	GetWaterInfo();
	void				SetHeightBase( float aHeightBase );
	void				SetHeightVariance( BYTE aHeightVariance );
	void				SetRunDirs( BYTE aIndex, BYTE aRunDir );
	void				SetRunSpeeds( BYTE aIndex, BYTE aRunSpeed );
#ifdef DIRECT_VERSION_9_MJH
	void				SetTextureIndexs( BYTE aIndex, BYTE aTextureIndex, LPDIRECT3DTEXTURE9 aTexture );
#else
	void				SetTextureIndexs( BYTE aIndex, BYTE aTextureIndex, LPDIRECT3DTEXTURE8 aTexture );
#endif // DIRECT_VERSION_9_MJH

	float				GetHeight(int Num) { return theHeightList[ Num ]; }


protected:

	SRockWaterInfo	theWaterInfo;

	int				thePatchWidth;
	int				thePatchHeight;
	int				theVtxCount;
	int				theIdxCount;
	float			theInterval;	/// 몇 그리드 단위로 폴리곤을 그릴 것인가? 

	/// 출렁임 표현을 위한 변수들 
	float*			theHeightList;	
	bool*			theUpDownFlagList;

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	theVB;
	LPDIRECT3DINDEXBUFFER9	theIB;
	LPDIRECT3DTEXTURE9		theTexture[2];
#else
	LPDIRECT3DVERTEXBUFFER8	theVB;
	LPDIRECT3DINDEXBUFFER8	theIB;
	LPDIRECT3DTEXTURE8		theTexture[2];
#endif // DIRECT_VERSION_9_MJH

	D3DXMATRIX				theMatWorld;
	D3DXMATRIX				theMatTexturePos[2];	/// texture UV animation을 위한 matrix
};


#endif 
