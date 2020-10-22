///////////////////////////////////////////////////////////////////////////////
///
///		File		: ScrollBar.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__
//-----------------------------------------------------------------------------
#include "SlideBar.h"
#include "Button.h"

enum enScrollBarFocusPos
{
	sbfNoneFocus,
	sbfClientFocus,
	sbfBtnAFocus,
	sbfBtnBFocus,
	sbfThumbFocus
};

class CScrollBar : public CSlideBar
{
public:
	CScrollBar(){}
	CScrollBar( SDesktop* DT );
	virtual ~CScrollBar();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );//OR
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	virtual WNDID FocusCheck( int Mx, int My, bool IsChildCheck );//OR
	//-------------------------------------------------------------------------

	void SetScrollBtnA( TEXID Normal, TEXID Focus, TEXID Down, TEXID Hold );
	void SetScrollBtnB( TEXID Normal, TEXID Focus, TEXID Down, TEXID Hold );
	virtual void InitSlide( enSlideType SlideType, int Min, int Max, int Value, int ThumbWidth );//OR
	virtual void SetClientAlpah( UCHAR Alpha );	
	

	virtual void ResetSize( SRect* size );
	
	//by simwoosung
	void ReSetScroll(int Min, int Max, int Value);
	virtual void Reposition( int x, int y );
	virtual void SetWndPositionR( int PosX, int PosY );

protected:

	CButton* BtnA;//Left Top
	CButton* BtnB;//Right Bottom

	enScrollBarFocusPos	FocusPos;
	
	bool	 m_bIsBtnPress;				//버튼 프레스여부

	virtual void SetThumbPos();//OR
	virtual void SetSlideMovePos( int Mx, int My );//OR
	virtual void SlideValueRate();//OR

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
#endif  __SCROLLBAR_H__
