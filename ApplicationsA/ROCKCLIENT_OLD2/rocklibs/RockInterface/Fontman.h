///////////////////////////////////////////////////////////////////////////////
///
///		File		: Fontman.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-06
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __FONTMAN_H__
#define __FONTMAN_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"

#include "Define.h"
#include "Fontg.h"



#define		MAX_FONTG		1024



class CFontman
{
public:
	CFontman(){}
#ifdef DIRECT_VERSION_9_MJH
	CFontman( HWND hWnd, LPDIRECT3DDEVICE9 d3dDevice );
#else
	CFontman( HWND hWnd, LPDIRECT3DDEVICE8 d3dDevice );
#endif // DIRECT_VERSION_9_MJH
	~CFontman();

	
	CFontg* GetFont( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco, bool ab_smooth = false, int al_width = 0 );
	
	int GetNumTTFont();										// TTFont ����
	LOGFONT* GetTTFont( TCHAR* Name );						// �̸��˻�
	LOGFONT* GetTTFont( int Idx );							// �ε����˻�
	LOGFONT* GetTTFont( int Idx, NFontCharset Charset );	// ���ǰ˻�( ĳ���ͼ����� )

private:

	int			theNumFontg;
	CFontg		*theFontg[MAX_FONTG];
};


extern	CFontman*		Fontman;
//-----------------------------------------------------------------------------
#endif	__FONTMAN_H__