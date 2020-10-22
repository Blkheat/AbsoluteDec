///////////////////////////////////////////////////////////////////////////////
///
///		File		: ChatOutput.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __CHATOUTPUT_H__
#define __CHATOUTPUT_H__
//-----------------------------------------------------------------------------
#include "Wnd.h"


#define MAX_CHATITEM		100

struct SChatItem
{
	TCHAR		Text[256];
	D3DCOLOR	Color;

	SChatItem(){}
	SChatItem( TCHAR* str, D3DCOLOR color )
	{
		SAFE_STR_CPY( Text, str, 256 );
		Color = color;
	}
};



class CChatOutput : public CWnd
{
public:
	CChatOutput(){}
	CChatOutput( SDesktop* DT );
	~CChatOutput();

	//-------------------------------------------------------------------------
	virtual void RenderProc();//OR
	//-------------------------------------------------------------------------

	void SetChatItem( void* pstr, D3DCOLOR color );
	void SetOutputLevel( int level );
	void SetOutputStep( int step );
	void InitChatOutput();

	void SetLineInit();
	void SetCurLine( int curline );
	int GetNumLine();
	int GetCurLine();
	
	void SetMaxLine( int max_line );
	void ResetChatOutput();

private:
	CFontg*			theFontg;
	
	SChatItem		theChatItem[MAX_CHATITEM];
	int				theCurLine;
	int				theNumLine;
	int				theLimitLine;

	int				theOutputLine;
	int				theMaxLine;

	void ResetSize();
	void AddChatItem( TCHAR* str, D3DCOLOR color );
};

//-----------------------------------------------------------------------------
#endif	__CHATOUTPUT_H__