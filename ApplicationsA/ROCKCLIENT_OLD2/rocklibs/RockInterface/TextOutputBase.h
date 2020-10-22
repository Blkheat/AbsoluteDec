#ifndef __TEXTOUTPUTBASE_H__
#define __TEXTOUTPUTBASE_H__
//-----------------------------------------------------------------------------

#include "Define.h"

#define MAX_CHATDATA_NUM   100
#define MAX_SYSTEMDATA_NUM 100

enum NTxtViewState
{
	NTxtNone = 0,
	NTxtView,
	NTxtFadeout,
};

enum NChatOutActState
{
	COutState_None = 0,
	COutState_Act,
	COutState_NonAct,
};

enum NChatOutFilterState
{
	COutNoneView = 0,
	COutTotalView,			//전체뷰	
	COutPartyView,			//파티뷰	
	COutGuildView,			//길드뷰	
	COutAnnounView,			//공지뷰
	COutHelpView,			//도움말뷰 	
};

struct STitleSize
{
	int tWidth;
	int tHeight;

	STitleSize()
	{
		tWidth = -1;
		tHeight = -1;
	}

	void operator=(const STitleSize &temp)
	{
		tWidth = temp.tWidth;
		tHeight = temp.tHeight;
	}
};

struct SChatTxtData
{
	static float ViewTime;
	static float FadingTime;
	
	ICOLOR4		Rcolor;			//Ref Color
	TCHAR		Dest[256];		//대상이름
	STitleSize	TSize;			//타이틀 사이즈
	TCHAR		Text[256];		//출력 텍스트
	UINT		ChatState;		//채팅종류
	float		CumTime;		//시간누적값
	UINT		ViewState;		//뷰타입
	
	SChatTxtData()
	{
		Rcolor[A] = Rcolor[R] = Rcolor[G] = Rcolor[B] = 0;
		Rstrcpy( RWCHAR(Dest), _RT("") );	
		Rstrcpy( RWCHAR(Text), _RT("") );
		ChatState = 0;
		CumTime = 0.0f;
		ViewState = NTxtView;
	}
	void operator=(SChatTxtData &temp)
	{
		Rcolor[A] = temp.Rcolor[A];
		Rcolor[R] = temp.Rcolor[R];
		Rcolor[G] = temp.Rcolor[G];
		Rcolor[B] = temp.Rcolor[B];	
		Rstrcpy( RWCHAR(Dest), RWCHAR(temp.Dest ));		
		TSize = temp.TSize;		
		Rstrcpy( RWCHAR(Text), RWCHAR(temp.Text));
		ChatState = temp.ChatState;
		CumTime = temp.CumTime;
		ViewState = temp.ViewState;
	}
};

struct SSystemTxtData
{
	static float ViewTime;
	static float FadingTime;
	
	ICOLOR4		Rcolor;			//Ref Color
	TCHAR		Text[256];		//출력 텍스트
	float		CumTime;		//시간누적값
	UINT		ViewState;		//뷰타입

	SSystemTxtData()
	{
		Rcolor[A] = Rcolor[R] = Rcolor[G] = Rcolor[B] = 0;
		Rstrcpy( RWCHAR(Text), _RT("") );
		CumTime = 0.0f;
		ViewState = NTxtView;
	}
	
	void operator=( SSystemTxtData &temp)
	{
		Rcolor[A] = temp.Rcolor[A];
		Rcolor[R] = temp.Rcolor[R];
		Rcolor[G] = temp.Rcolor[G];
		Rcolor[B] = temp.Rcolor[B];			
		Rstrcpy( RWCHAR(Text), RWCHAR(temp.Text) );	
		CumTime = temp.CumTime;
		ViewState = temp.ViewState;
	}
};

//-----------------------------------------------------------------------------
#endif	__TEXTOUTPUTBASE_H__