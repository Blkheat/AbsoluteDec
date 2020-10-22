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
///		*.orm file ���� ���� 
///	
///		# ���� ���� 
///		fread(&m_NewFile.byWaterTextureIndex	,sizeof(BYTE		),1,	fp);
///		fread(&m_NewFile.bShowWater				,sizeof(BOOL		),1,	fp);
///		fread(&m_NewFile.byWaterHeightVariance	,sizeof(BYTE		),1,	fp);
///		fread(&m_NewFile.fWaterBaseHeight		,sizeof(float		),1,	fp);
///		fread(&m_NewFile.byWaterRunsDir			,sizeof(BYTE		),1,	fp);
///	
///		#���� 
///		fread(&m_NewFile.byWaterTextureIndex	,sizeof(BYTE		),1,	fp);
///			-> �� BYTE ���� Water ������ ���. 0 �̸� ���� ���� �� 
///			-> ���� BOOL, BYTE, float, BYTE ���� ����... 10BYTE �����.. ���߿� �Ѳ����� ������ ����. 
///		Water ������ ���� *.orm file �� �ڿ� SRockWaterInfo �� attach �ȴ�. 
///		Water ������ 0 �̶�� attach size�� 0
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
	float	theHeightBase;			/// ���� ���� 
	BYTE	theHeightVariance;		/// �ⷷ�� ���� 
	BYTE	theRunDirs;				/// �帣�� ����.			���� 4bit 0�� �ؽ���, ���� 4bit 1�� �ؽ��� 
	BYTE	theRunSpeeds;			/// �帣�� �ӵ�. 0 ~ 15		���� 4bit 0�� �ؽ���, ���� 4bit 1�� �ؽ��� 
	BYTE	theTextureIndexs;		/// �ؽ��� �ε���. 0 ~ 15	���� 4bit 0�� �ؽ���, ���� 4bit 1�� �ؽ��� 
									/// 0xf �� �ؽ��� �������� ���� ������ ó�� 

	/// ���� �׷��� ����. �� ���ؽ� ������ 16.0f -> Ÿ�� ũ��. ������ cell ������ ���� 
	/// left-bottom ���� Cell ����. ���� �ʺ� ���� 
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

	/// Texture�� �ܺο��� �̸� �����Ͽ� �����͸� �ѱ�ϴ�. 
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
	float			theInterval;	/// �� �׸��� ������ �������� �׸� ���ΰ�? 

	/// �ⷷ�� ǥ���� ���� ������ 
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
	D3DXMATRIX				theMatTexturePos[2];	/// texture UV animation�� ���� matrix
};


#endif 
