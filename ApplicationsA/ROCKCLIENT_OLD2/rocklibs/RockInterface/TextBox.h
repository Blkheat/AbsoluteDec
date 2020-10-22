///////////////////////////////////////////////////////////////////////////////
///
///		File		: TextBox.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__
//-----------------------------------------------------------------------------
#include "Wnd.h"


class CTextBox : public CWnd
{
public:
	CTextBox()
	{
		Text     = NULL;
		theFontg = NULL;
		memset(TextLine, 0, sizeof(TCHAR *) * 10);		
	}

	CTextBox( SDesktop* DT );
	virtual ~CTextBox();

	//-------------------------------------------------------------------------
	virtual void RenderProc();//OR
	virtual SEventMessage* EventProc( SEventMessage* EMsg );
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	//-------------------------------------------------------------------------

	virtual void SetAdjustPosition( int PosX, int PosY );//OR

	virtual void SetFontg( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco );
	virtual void SetFontg5( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco , bool ab_smooth , int al_width  );
	virtual void SetFontR( NFontResv ae_fonttype );
	
	virtual void SetText( void* Str );//함수 수정 필요...폰트장식과..정렬을 나중에 셋팅하면..문자열정보와 정렬이 제대로 안된다.
	virtual void GetText( TCHAR* Str, int MaxLen );	
	virtual void SetFontDecoration( NFontDecoration DecorationType );
	virtual void SetLimitLength(const int nLength);
		
	virtual void SetTextSize( int LimitLen );
	virtual void SetAlignText( NAlignText AlignX, NAlignText AlignY );
	virtual void SetBlank( int BlankLR, int BlankTB );
	virtual void SetBlank( int BlankL, int BlankR, int BlankT, int BlankB );
	virtual void SetBlankLine( bool BlankLine );
	virtual void SetCenterLine( bool CenterLine );
	virtual void SetTextColor( UCHAR Red, UCHAR Green, UCHAR Blue );
	virtual void SetTextAlpha( UCHAR Alpha );
	virtual void SetStartLine( int line );

	virtual void SetMultiLine( bool MultiLine );
	virtual void SetLineInterval( int LineInterval );
	virtual void SetImageNum( bool ab_imgnum);
	virtual void SetIntegralText( bool Integral );

	virtual void SetDrawTextTime( long aDrawTextTime );
	virtual void SetScaleTrans( bool bFlag );


	virtual int  GetTextLimitLen()		{ return TextLimitLen; }
	virtual void ResetSize( SRect* size );

	virtual void SetMultiLineAsign(bool bFlag);
	virtual void DrawMutiAlignText();

#ifdef HHM_TEXTBOX_ENABLE// 활성화에 따라 텍스트 색 변경. 비활성색(128, 128, 128) (2009/05/13)

	virtual void SetEnable(bool bEnable);

#endif// #define HHM_TEXTBOX_ENABLE// 활성화에 따라 텍스트 색 변경. 비활성색(128, 128, 128) (2009/05/13)
	
public:
	//by simwoosung
	SStringInfo * GetStringInfo()		{ return &StrInfo; }
	CFontg*		GetFontg(){return theFontg;}
protected:
	CFontg*			theFontg;
	int			    aFontResv;
	
	TCHAR*			Text;//문자열 버퍼
	int				m_AllocTextLen;		///-- 문자열버퍼 할당길이	
	int				TextLength;//문자열 길이
	int				TextLimitLen;//문자열의 최대 길이	
	int				NumLine;

	int				Tx; // Blank가 포함된 Text Box
	int				Ty;
	int				Tw;
	int				Th;
	
	int				Ax; // Text Box에서 정열된 Text 좌표
	int				Ay;

	NFontDecoration DecorationType;
	D3DCOLOR		Tcolor;//Text Color
	ICOLOR4			Rtcolor;//Ref Text Color.... Max Alpha는 255로 고정

	SStringInfo		StrInfo;

	int				StartLine;

	int				BlankLeft;
	int				BlankRight;
	int				BlankTop;
	int				BlankBottom;
	
	NAlignText		AlignX;
	NAlignText		AlignY;

	bool			isBlankLine;
	bool			isCenterLine;
	bool			isMultiLine;
	int				m_LineInterval;
	bool			isIntegralText;//문자열을 폭과 높이에 맞춘다
	bool			isImageNum;		///-- Image Num 을 출력 숫자만지원
	
	long			m_DrawStartTime;
	long			m_DrawTextTime;
	
	bool			IsScaleTrans;
	bool			m_IsMultiLineAsign;
	int				m_MultiLineCount;
	
	TCHAR		*	TextLine[10]; //문자열 버퍼

	virtual int GetTextLine();
	virtual int GetAlignPosX();
	virtual int GetAlignPosY();

	virtual int GetAlignPosX(SStringInfo & tempStrInfo);
};

//-----------------------------------------------------------------------------
#endif  __TEXTBOX_H__
