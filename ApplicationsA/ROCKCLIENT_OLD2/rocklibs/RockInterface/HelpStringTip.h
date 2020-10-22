///////////////////////////////////////////////////////////////////////////////
///
///		File		: HelpStringTip.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __HELPSTRINGTIP_H__
#define __HELPSTRINGTIP_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"

#include "Define.h"
#include "Fontman.h"
#include "Render.h"
	

class CHelpTip
{
public:
	CHelpTip(){}
	CHelpTip( SDesktop* DT );
	~CHelpTip();
	
	void Initialize();
	
	void Set( HLPID Hid, SRect* wc_size, TCHAR* str, D3DCOLOR color );	
	void RenderProc();
	
	//by simwoosung
	void SetUpPosString(SRect* wc_size, TCHAR* str, D3DCOLOR color );
	void SetString(SRect* wc_size, TCHAR* str, D3DCOLOR color );
	void RenderProc( float fAlphaRatio, D3DCOLOR texColor );
	
public:	
    CFontg*			theFontg;   //by simwoosung 위치변경
	
protected:
	SDesktop*		pDT;
	
	HLPID			theHid;
	int				theCharIndex;

	BOOL			IsHid;
	
	SStringInfo		theStrInfo;
	
	TCHAR			theText[256];
	
	SRect			theClient;
	SRect			theNorthFrm;
	SRect			theNorthEastFrm;
	SRect			theEastFrm;
	SRect			theSouthEastFrm;
	SRect			theSouthFrm;
	SRect			theSouthWestFrm;
	SRect			theWestFrm;
	SRect			theNorthWestFrm;
	
	int				theBlankLR;
	int				theBlankTB;
	
	int				theMaxWidth;
	int				theFrameWidth;
	
	void CalcClientSize( SRect* wc_size );//calculation
	void CalcFrameSize();

	//by simwoosung
	void CalcUpPosClientSize( SRect* wc_size );
};

//-----------------------------------------------------------------------------
#endif	__HELPSTRINGTIP_H__