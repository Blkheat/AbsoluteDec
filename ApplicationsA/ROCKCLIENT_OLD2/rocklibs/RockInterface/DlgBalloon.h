///////////////////////////////////////////////////////////////////////////////
///
///		File		: DlgBalloon.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __DLGBALLOON_H__
#define __DLGBALLOON_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#include "..\\..\\bravolibs\\obj\\character.h"

#include "Define.h"
#include "Fontman.h"
#include "Render.h"

#define MAX_DLGBALLOON		129


class CDlgBalloon
{
public:
	CDlgBalloon(){}
	CDlgBalloon( SDesktop* DT );
	~CDlgBalloon();
	
	void Initialize( SDesktop* DT );
	void Set( NCHARTYPE ctype, DWORD id, SPcInfo* pc_info, SDlgBorder* border );
	
	bool IsExistence( NCHARTYPE ctype, DWORD id );
	bool IsLife();
	void Die();

	void RenderProc();
	
protected:
	SDesktop*		theDT;
	
	CFontg*			theFontg;
	SStringInfo		theStrInfo;

	TCHAR			theText[256];
	D3DCOLOR		theColor;
	NBalloonType	theBType;//frame type

	SDlgBorder*		theBorder;
	
	SRect			theClient;
	SRect			theNorthFrm;
	SRect			theNorthEastFrm;
	SRect			theEastFrm;
	SRect			theSouthEastFrm;
	SRect			theSouthFrm;
	SRect			theSouthWestFrm;
	SRect			theWestFrm;
	SRect			theNorthWestFrm;
	SRect			theTail;


	int				theBlankLR;
	int				theBlankTB;

	int				theMaxWidth;

	int				theLifeTime;


	int				theBGAlpha;
	int				theTXTAlpha;
	//-------------------------------------------------------------------------
	NCHARTYPE		theCType;
	DWORD			theID;

	int				thePcIdx;
	SPcInfo*		thePcInfo;
	//-------------------------------------------------------------------------
	
	void CalcClientSize();//calculation
	void CalcFrameSize();
	
	//by simwoosung
	void RenderDlgBalloonBill();
    void RenderDlgBalloon(Character*  & pCharacter);
};

//-----------------------------------------------------------------------------
#endif	__DLGBALLOON_H__