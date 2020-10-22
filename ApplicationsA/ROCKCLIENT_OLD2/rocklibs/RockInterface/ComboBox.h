///////////////////////////////////////////////////////////////////////////////
///
///		File		: ComboBox.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __COMBOBOX_H__
#define __COMBOBOX_H__
//-----------------------------------------------------------------------------
#include "Wnd.h"
#include "TextBox.h"
#include "Button.h"
#include "ListBox.h"

#define MAX_TEXT_LENGTH		256

enum enComboBoxFocusPos
{
	cbfClientFocus,
	cbfTextBoxFocus,
	cbfButtonFocus,
	cbfListBoxFocus
};

class CImageBox9;
class CComboBox : public CWnd
{
public:
	CComboBox(){}
	CComboBox( SDesktop* DT );
	virtual ~CComboBox();

	//-------------------------------------------------------------------------
	virtual void CreateWnd( NWindowType WType, WNDID Iid, WNDID Pid, float PosX, float PosY, int Width, int Height, bool isRatio );
	virtual SEventMessage* EventProc( SEventMessage* EMsg );//OR
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	virtual WNDID FocusCheck( int Mx, int My, bool IsChildCheck );//OR
	virtual stTextList* AddItem( void* Str, const int al_value_D = 0 );
	virtual void ClearItem();	
	virtual int GetCurSelIdx();		///-- 선택된 Text의 순차적인 Index
	virtual void SetCurSelIdx( const int al_idx );
	virtual int GetCurSelValue();	///-- 선택된 Text의 Value( AddItem 의 두번째 인자로 지정된 값 )
	virtual	void SetListboxTileSizeTB( const int al_left, const int al_right );
	virtual void SetListboxTileSizeLR( const int al_left, const int al_right );
	virtual void SetListboxImage9( TEXID Cimg  );
	virtual void SetVisible( bool Visible );

	virtual void SetClientColor( UCHAR Red, UCHAR Green, UCHAR Blue );
	virtual void SetClientAlpah( UCHAR Alpha );
	//-------------------------------------------------------------------------

	void SetSelectText( void* strTxt );

	CTextBox* GetTextBox() { return TextBox; }

public:
	
	
protected:
	TCHAR Text[MAX_TEXT_LENGTH];

	enComboBoxFocusPos FocusPos;

	CTextBox*		TextBox;
	CButton*		Button;

	bool			isActiveListBox;	
	CListBox*		ListBox;
	CImageBox9*		img_outline;
	

	int ComboHeight;

	int ml_sel_idx;
	int ml_sel_value;


	//-------------------------------------------------------------------------
	virtual void SetAdjustPosition( int PosX, int PosY );//OR
	virtual void UpdateHandler();//OR

	virtual void OnMouseFocus( int Mx, int My );//OR
	virtual void OnMouseLButtonDown( int Mx, int My );//OR
	virtual void OnMouseLButtonPress( int Mx, int My );//OR
	virtual void OnMouseLButtonUp( int Mx, int My );//OR
	virtual void OnMouseLDrag( int Mx, int My );//OR
	virtual void OnMouseLDrop( int Mx, int My );//OR
	//-------------------------------------------------------------------------
};



//-----------------------------------------------------------------------------
#endif  __COMBOBOX_H__
