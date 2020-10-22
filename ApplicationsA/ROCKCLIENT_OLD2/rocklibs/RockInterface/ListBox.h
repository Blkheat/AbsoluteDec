///////////////////////////////////////////////////////////////////////////////
///
///		File		: ListBox.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __LISTBOX_H__
#define __LISTBOX_H__
//-----------------------------------------------------------------------------
#include "TextBox.h"
#include "ScrollBar.h"

enum enListBoxFocusPos
{
	lbfClientFocus,
	lbfScrollBarFocus,
};


enum enInsertOrder
{
	ioNone,
	ioFirst,
	ioLast
};


struct stTextList
{
	TCHAR*	Str;
	int		Len;
	int		value;

	stTextList* Next;
};


struct stVisibleTextBox
{
	int Idx;

	int X;
	int Y;
	int W;
	int H;

	D3DCOLOR Color;
};




class CListBox : public CTextBox
{
public:
	CListBox(){}
	CListBox( SDesktop* DT );
	virtual ~CListBox();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );//OR	
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	virtual WNDID FocusCheck( int Mx, int My, bool IsChildCheck );//OR
	
	stTextList* AddItem( TCHAR* Str , const int al_value_D = 0 );
	void DeleteItem( int Idx );
	void ClearItem();
	bool GetSelectedText( TCHAR* Str, int MaxLen );
	virtual	bool GetSelectedItem( int * const rapl_idx, int * const rapl_Value );
	TCHAR* GetItem( int Idx );
	enListBoxFocusPos GetFocusPos();

	void SetScrollBar();
	void SetScrollBarWidth( int Width );
	void SetInsertOrder( enInsertOrder InsertOrder );
	void SetLBoxBlank( int BlankL, int BlankR, int BlankT, int BlankB );
	void SetLineHeight( int Height );
	void SetLineTextAlign( NAlignText LineTextAlign );
	void InitScrollBar();
	virtual bool SetCurSelIdx( const int al_idx );
	virtual void SetClientColor( UCHAR Red, UCHAR Green, UCHAR Blue );
	virtual void SetClientAlpah( UCHAR Alpha );

	int GetNumItem()			{ return NumItem; }
	
	void SetPosition(int PosX, int PosY);	
	
protected:
	enListBoxFocusPos FocusPos;
	
	bool isScrollBarView;
	int ScrollBarWidth;
	CScrollBar* ScrollBar;

	int ScrbX;
	int ScrbY;
	int ScrbW;
	int ScrbH;

	int LBoxX;
	int LBoxY;
	int LBoxW;
	int LBoxH;

	int	LBoxBlankLeft;
	int	LBoxBlankRight;
	int	LBoxBlankTop;
	int	LBoxBlankBottom;

	NAlignText LineTextAlign;
	int	LineHeight;
	
	int NumVisibleLine;
	int FocusLine;
	int SelectedLine;
	int FirstVisibleIdx;
	stVisibleTextBox* VBox;

	enInsertOrder InsertOrder;
	int NumItem;

	//-------------------------------------------------------------------------
	stTextList* Head;
	stTextList* Tail;

	
	const int GetValue( int Idx );	

	stTextList* GetHead();
	stTextList* GetTail();
	stTextList* GetNext( stTextList* Node );
	
	void InsertNodeFirst( stTextList* Node );
	void InsertNodeLast( stTextList* Node );
	//-------------------------------------------------------------------------
	
	int GetLineAlignY( TCHAR* Str, int Ly, int Lh );
	
	
	void SetVBoxState();
	void SetVBox();
	void DrawVBox();
	int CheckFocusVBoxLine( int Mx, int My );

	
	virtual void SetAdjustPosition( int PosX, int PosY );//OR
	virtual void UpdateHandler();//OR
	
	virtual void OnMouseFocus( int Mx, int My );//OR
	virtual void OnMouseLButtonDown( int Mx, int My );//OR
	//virtual void OnMouseLButtonPress( int Mx, int My );//OR
	//virtual void OnMouseLButtonUp( int Mx, int My );//OR
	virtual void OnMouseLDrag( int Mx, int My );//OR
	virtual void OnMouseLDrop( int Mx, int My );//OR
};

//-----------------------------------------------------------------------------
#endif  __LISTBOX_H__
