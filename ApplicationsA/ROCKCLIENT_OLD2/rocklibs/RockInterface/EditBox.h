///////////////////////////////////////////////////////////////////////////////
///
///		File		: EditBox.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __EDITBOX_H__
#define __EDITBOX_H__
//-----------------------------------------------------------------------------
#include "FrameWnd.h"
#include "TextBox.h"

enum enEditStyle
{
	esText,
	esOnlyAlphaNumeric,
	esOnlyNumeric,
	esPassword
};

struct stCaret
{
	bool        Use;
    
	int         FchVisibleIdx;//EditBox에서 보이는 첫번째문자의 인덱스
	bool		isDirectionMove;//true

    int         SelStart;
	int			SelEnd;
	
	bool		isBlock;
	int			BlockX;
	int			BlockW;
    	
	TEXID		Img;
	int			X;
	int			Y;
	int			W;
	int			H;
	D3DCOLOR	Color;
};



class CEditBox : public CTextBox
{
public:
	CEditBox() { hEdit = NULL; hEditSave= NULL;}
	CEditBox( SDesktop* DT );
	virtual ~CEditBox();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );//OR
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	//-------------------------------------------------------------------------

	virtual void SetAdjustPosition( int PosX, int PosY );//OR
	
	void InitEditBox();
	void SetEditStyle( enEditStyle EditStyle );
	void ChangeEditStyle( enEditStyle aEditStyle );
	void SetAutoScroll( bool bAutoScroll );
	void SetReturnActionClear( bool bReturnActionClear );
	void SetNumberComma(bool bFlag ){isNumberComma = bFlag;}
	virtual void SetText( void* Str );//OR
	virtual void SetTextSize( int LimitLen );//OR
	void GetDeleteComma(TCHAR* Str);


	void EditDisable();
	void SetEditFocus();
	void ClearText();
	
	//by simwoosung;
	void SetCheckSlashCommand(BOOL BFlag)		{ BCheckSlashCommand = BFlag; }
	HWND GetHEdit()								{ return hEdit;	} 
	void GetWindowEditText();
	void GetWindowEditTextA();
	void GetWindowEditTextW();
	stCaret		Caret;
	
	//NEWIME


protected:

	CFrameWnd*	FrameWnd;
		
	TCHAR*		EditText;
	TCHAR*		PwText;
	int			m_AllocEditTextLen;
	int			m_AllocPwTextLen;
	
	enEditStyle EditStyle;
	bool		isAutoScroll;
	bool		isReturnActionClear;
	bool		isAlphaNumeric;//입력모드 -> 영어이면 True
	Bool		isNumberComma;	// true 이면 콤마로 표기한다. 


	SStringInfo FontInfo;
	
	int			TabNum;
	
	//-------------------------------------------------------------------------
	HWND		hEdit;
	HWND		hEditSave;
	HFONT		hFont;
	HIMC		hImc;
	DWORD		Con;// Conversion	
	DWORD		Sen;// Sentence

	//by simwoosung;
	//슬래쉬 명령어의 체크 여부
	BOOL		BCheckSlashCommand;

	virtual void CreateWindowEditBox();	
	virtual void GetWindowEditCaretInfo();
	void SetCaretSelBlock( int BlockX, int BlockY, int BlockW, int BlockH, D3DCOLOR Ccolor );
	void GetImmConversionStatus();

	WNDPROC OldEditProc;
	virtual LRESULT CALLBACK EditSubProc( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam );
	virtual LRESULT CALLBACK EditSubProc_JP( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam );
	virtual LRESULT CALLBACK EditSubProc_IME( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam );
	
	static CEditBox* AgentEditBox;
	static LRESULT CALLBACK AgentEditSubProc( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam );
	//-------------------------------------------------------------------------



	void GetCaretIdx( int* SelStart, int* SelEnd );
	void GetRealCaretIdx( int* SelStart, int* SelEnd );
	
	virtual void DrawText();
	virtual void DrawCaret();

	virtual void UpdateHandler();//OR

	virtual void OnMouseFocus( int Mx, int My );//OR
	virtual void OnMouseLButtonDown( int Mx, int My );//OR
	virtual void OnMouseLButtonPress( int Mx, int My );//OR
	virtual void OnMouseLButtonUp( int Mx, int My );//OR
};

//-----------------------------------------------------------------------------
#endif  __EDITBOX_H__
