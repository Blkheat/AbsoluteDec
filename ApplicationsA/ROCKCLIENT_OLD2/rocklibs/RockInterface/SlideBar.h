///////////////////////////////////////////////////////////////////////////////
///
///		File		: SlideBar.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __SLIDEBAR_H__
#define __SLIDEBAR_H__
//-----------------------------------------------------------------------------
#include "Wnd.h"


enum enSlideType
{
	stHorzSld,
	stVertSld
};


class CSlideBar : public CWnd
{
public:
	CSlideBar(){}
	CSlideBar( SDesktop* DT );
	virtual ~CSlideBar();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );//OR
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	virtual WNDID FocusCheck( int Mx, int My, bool IsChildCheck );//OR
	//-------------------------------------------------------------------------

	void SetThumbImage( TEXID Normal, TEXID Focus, TEXID Down, TEXID Hold );
	virtual void InitSlide( enSlideType SlideType, int Min, int Max, int Value, int ThumbWidth );

	virtual void GetValue( int* Value );
    virtual void GetValue( int* Value, int* Max );
    virtual void SetValue( int Value );
	virtual void SetScope( int Min, int Max );
	virtual void ResetSldExtentSync( int BasisValue, int RelativeValue );
	virtual void SetSlideMovePos( int Mx, int My );
	
	virtual void Reposition( int x, int y );
	virtual void SetWndPositionR( int PosX, int PosY );
	
protected:
	enSlideType		SlideType;// stHorzSld(수평 슬라이드), stVertSld(수직 슬라이드)

	bool			isThumbFocus;

	NButtonState	ThumbState;
	TEXID			ThumbImg[4];//기본, 포커스, 눌림, 홀더

	int				DeltaX;//마우스 이동의 상대 좌표
	int				DeltaY;

	int				ThumbX;//썸의 Slide에서의 이동 위치
	int				ThumbY;
	int				ThumbW;//썸의 크기....Thumb(엄지손가락), Shaft(몸통), Arrow(화살표)
	int				ThumbH;

	int				SldValue;//현재 슬라이드바 위치의 값
	int				SldMin;//설정값의 범위(최소, 최대)
	int				SldMax;

	int				SldMinX;//Slide의 이동 가능 범위
	int				SldMaxX;
	int				SldMinY;
	int				SldMaxY;

	
	virtual void SetThumbSize();//지금은 안씀...
	virtual void SetThumbPos();
	virtual void SlideValueRate();
	
	virtual void DrawThumb();

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
	
public:
	//by simwoosung
	int	GetSldValue()	{ return SldValue; }
	int GetSldMin()		{ return SldMin; }
	int GetSldMax()		{ return SldMax; }
};

//-----------------------------------------------------------------------------
#endif  __SLIDEBAR_H__
