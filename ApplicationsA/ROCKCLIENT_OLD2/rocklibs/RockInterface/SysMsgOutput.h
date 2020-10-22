///////////////////////////////////////////////////////////////////////////////
///
///		File		: SysMsgOutput.h
///		Desc		:
///
///		Author		: Niceguy
///		Team		: Program - Client Team
///		Date		: 2005-01-06
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __SYSMSGOUTPUT_H__
#define __SYSMSGOUTPUT_H__
//-----------------------------------------------------------------------------
#include "..\\..\\brother.h"


#include "Wnd.h"


#define MAX_SYSTEMMSG		3

/*
typedef struct SSystemMsg
{	
	TCHAR		Text[256];
	D3DCOLOR	Color;

	SSystemMsg(){}
	SSystemMsg( TCHAR* str, D3DCOLOR color )
	{
		_tcscpy( Text, str );
		Color = color;
	}
} SSystemMsg;



class CSysMsgOutput : public CWnd
{
public:
	CSysMsgOutput(){}
	CSysMsgOutput( SDesktop* DT );
	~CSysMsgOutput();

	//-------------------------------------------------------------------------
	virtual void RenderProc();//OR
	//-------------------------------------------------------------------------

	void SetSystemMsg( TCHAR* str, D3DCOLOR color );
	void SetOutputLevel( int level );
	void SetOutputStep( int step );
	void InitChatOutput();

	void SetLineInit();
	void SetCurLine( int curline );
	int GetNumLine();
	int GetCurLine();
	
	void SetMaxLine( int max_line );
	void ResetSysMsgOutput();

private:
	CFontg*			theFontg;
	
	SSystemMsg		theSystmeMsg[MAX_SYSTEMMSG];
	int				theCurLine;
	int				theNumLine;
	int				theLimitLine;

	int				theOutputLine;
	int				theMaxLine;

	long			theNowTime;

	void ResetSize();
	void AddSystemMsg( TCHAR* str, D3DCOLOR color );
};
*/

// 화면 중앙의 큰 시스템 메세지 (공지 및 맵 이동 메세지)
typedef struct SSystemMsg
{	
	TCHAR		Text[256];
	D3DCOLOR	Color;
	WORD		Size;
	long		Time;

	SSystemMsg()
	{
		memset( Text, NULL , 256);
		Color = 0;
		Size = 0;
		Time = 0;
	}
	SSystemMsg( TCHAR* str, D3DCOLOR color )
	{
		Rstrcpy( RWCHAR(Text), RWCHAR(str) );
		Color = color;
		Size = 0;
		Time = 0;
	}
} SSystemMsg;


class CSysMsgOutput : public CWnd
{
public:
	CSysMsgOutput(){}
	CSysMsgOutput( SDesktop* DT );
	~CSysMsgOutput();

	//-------------------------------------------------------------------------
	virtual void RenderProc();//OR
	//-------------------------------------------------------------------------

	void SetSysMsgType( NSysMsgType type );
	void SetSystemMsg( TCHAR* str, D3DCOLOR color );
	void SetOutputStep( int step );
	
	void SetLineInit();
	void SetCurLine( int curline );
	int GetNumLine();
	int GetCurLine();
	
	void SetMaxLine( int max_line );
	void ResetSysMsgOutput();

private:
	NSysMsgType		theMsgType;

	CFontg*			theFontg;
	
	SSystemMsg		theSystemMsg[MAX_SYSTEMMSG];

	int				theCurLine;
	
	int				theLimitLine;

	int				theOutputLine;
	int				theMaxLine;

	void ResetSize();
	void AddSystemMsg( TCHAR* str, D3DCOLOR color );
};


//-----------------------------------------------------------------------------
#endif	__SYSMSGOUTPUT_H__