///////////////////////////////////////////////////////////////////////////////
///
///		File		: ScrollBar.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "ScrollBar.h"
#include "FrameWnd.h"

extern long			g_nowTime;

//-----------------------------------------------------------------------------
CScrollBar::CScrollBar( SDesktop* DT ) : CSlideBar( DT )
{
	BtnA = SAFE_NEW( CButton( DT ) );		
	BtnB = SAFE_NEW( CButton( DT ) );

	BtnA->SetImgBtn(true);
	BtnB->SetImgBtn(true);

	FocusPos = sbfClientFocus;
}

//-----------------------------------------------------------------------------
CScrollBar::~CScrollBar()
{
	SAFE_DELETE(BtnA);
	SAFE_DELETE(BtnB);
}

void CScrollBar::SetClientAlpah( UCHAR Alpha )
{
	CWnd::SetClientAlpah( Alpha );
	BtnA->SetClientAlpah( Alpha );
	BtnB->SetClientAlpah( Alpha );
}

//-----------------------------------------------------------------------------
SEventMessage* CScrollBar::EventProc( SEventMessage* EMsg )//OR
{
	CFrameWnd* fwnd = NULL;

	DeltaX = EMsg->Mouse.Rposition.x;
	DeltaY = EMsg->Mouse.Rposition.y;

	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;
	
	if( FocusPos == sbfBtnAFocus ){	BtnA->EventProc( EMsg );	}
	if( FocusPos == sbfBtnBFocus ){	BtnB->EventProc( EMsg );	}
		
	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
		OnMouseFocus( Mx, My );
		{
			EMsg->FocusWnd	= Iid;
			EMsg->Notify = 0;
		}
		break;
	case MA_LDOWN:
		OnMouseLButtonDown( Mx, My );
		if( FocusPos == sbfClientFocus )
		{
			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_SLD_CLICKMOVEBAR;
			EMsg->fParam	= SldValue;	
		}
		break;
	case MA_LPRESS:
		OnMouseLButtonPress( Mx, My );
		if( ( FocusPos == sbfBtnAFocus ) || ( FocusPos == sbfBtnBFocus ) )
		{
			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_SLD_BTNMOVERBAR;			
			EMsg->fParam	= SldValue;	
		}
		break;
	case MA_LUP:
		OnMouseLButtonUp( Mx, My );
		if( ( FocusPos == sbfBtnAFocus ) || ( FocusPos == sbfBtnBFocus ) )
		{
			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_SLD_BTNMOVERBAR;
			EMsg->fParam	= SldValue;	
		}
		break;
	case MA_LDRAG:
		if( ( FocusPos == sbfBtnAFocus ) || ( FocusPos == sbfBtnBFocus ) )
		{
			OnMouseLButtonPress( Mx, My );
		}

		OnMouseLDrag( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_SLD_MOVEBAR;
		EMsg->fParam	= SldValue;

		//드래그 중일때는 부모 윈도우로 통지가 안되기 때문에.... 메세지 처리를 못한다.
		//그래서 지금은 편법으로 사용....
		fwnd = (CFrameWnd*)Pwnd;
		//if( ( GetParentID() != WID_None ) && ( fwnd->WndProc != NULL ) )
		if( ( GetParentID() != WID_None ) && ( fwnd->theWnd != NULL ) )
		{
			fwnd->theWnd->Proc( EMsg );
		}
		break;
	case MA_LDROP:
		OnMouseLDrop( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_SLD_MOVEBAR;
		EMsg->fParam	= SldValue;

		fwnd = (CFrameWnd*)Pwnd;
		//if( ( GetParentID() != WID_None ) && ( fwnd->WndProc != NULL ) )
		if( ( GetParentID() != WID_None ) && ( fwnd->theWnd != NULL ) )
		{
			fwnd->theWnd->Proc( EMsg );
		}
		break;
	}

	return( EMsg );
}

//-----------------------------------------------------------------------------
void CScrollBar::RenderProc()//OR
{
	UpdateHandler();

	Render->DrawState();	
	Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );

	BtnA->RenderProc();
	BtnB->RenderProc();	

	DrawThumb();
}

//-----------------------------------------------------------------------------
IRESULT CScrollBar::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )//OR
{
	int Iresult( 0 );
	Iresult = CWnd::ReceiveEvent( Event, fParam, sParam, xParam, exParam );

	switch( Event )
	{
	case n_emSetSldScope:
		SetScope( (int)fParam, (int)sParam );
		break;
	case n_emSetSldValue:
		SetValue( (int)fParam );
		break;
	case n_emGetSldValue:
		if( sParam != 0 )
        {
            GetValue( (int*)fParam, (int*)sParam );
        }
        else
        {
            GetValue( (int*)fParam );
        }
		break;
	case n_emResetSldExtentSync:
		break;
	}

	return( Iresult );
}

//-----------------------------------------------------------------------------
WNDID CScrollBar::FocusCheck( int Mx, int My, bool IsChildCheck )//OR
{
	IsChildCheck;

	if(FocusPos != sbfThumbFocus)
	{
		FocusPos = sbfNoneFocus;
	}

	if( isFocusCheck )
	{
		//Client BoundingBox 계산
		int Bx = Cx; 
		int By = Cy;
		int Bw = Cw + Cx;
		int Bh = Ch + Cy;

		//Thumb BoundingBox 계산
		int Tx = Bx + ThumbX;
		int Ty = By + ThumbY;
		int Tw = Bx + ThumbX + ThumbW;
		int Th = By + ThumbY + ThumbH;

		if( ( (Bx <= Mx) && (Bw >= Mx) ) && ( (By <= My) && (Bh >= My) ) )
		{
			FocusPos = sbfClientFocus;

			if( BtnA->FocusCheck( Mx, My, false ) != WID_None )
			{
				FocusPos = sbfBtnAFocus;
			}
			if( BtnB->FocusCheck( Mx, My, false ) != WID_None )
			{
				FocusPos = sbfBtnBFocus;
			}

			if( ( (Tx <= Mx) && (Tw >= Mx) ) && ( (Ty <= My) && (Th >= My) ) )
			{
				FocusPos = sbfThumbFocus;
			}

			return( Iid );
		}
	}
	
	return( WID_None );
}

//-----------------------------------------------------------------------------
void CScrollBar::SetScrollBtnA( TEXID Normal, TEXID Focus, TEXID Down, TEXID Hold )
{
	BtnA->SetBtnImage( Normal, Focus, Down, Hold );
}

//-----------------------------------------------------------------------------
void CScrollBar::SetScrollBtnB( TEXID Normal, TEXID Focus, TEXID Down, TEXID Hold )
{
	BtnB->SetBtnImage( Normal, Focus, Down, Hold );
}

//-----------------------------------------------------------------------------
void CScrollBar::InitSlide( enSlideType SlideType, int Min, int Max, int Value, int ThumbWidth )//OR
{
	this->SlideType = SlideType;

	int Cwidth = Cx + Cw;
	int Cheight = Cy + Ch;
	int BtnWidth( 0 );//버튼의 폭

	switch( SlideType )
    {
    case stHorzSld://slide의 이동가능 범위 설정
		{
			BtnWidth = Ch;
			BtnA->CreateWnd( n_wtButton, Iid, WID_None, Cx, Cy, BtnWidth, BtnWidth, false );
			BtnB->CreateWnd( n_wtButton, Iid, WID_None, Cwidth-BtnWidth, Cy, BtnWidth, BtnWidth, false);
			
			BtnA->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
			BtnB->SetBtnImage( TID_None, TID_None, TID_None, TID_None );

			ThumbW = ThumbWidth;
			ThumbH = Ch;//Thumb세로폭;
			SldMinX = BtnWidth; 
			SldMaxX = (Cw - BtnWidth) - ThumbW;
		}
		break;
    case stVertSld://slide의 이동가능 범위 설정
		{
			BtnWidth = Cw;
			BtnA->CreateWnd( n_wtButton, Iid, WID_None, Cx, Cy, BtnWidth, BtnWidth, false );
			BtnB->CreateWnd( n_wtButton, Iid, WID_None, Cx, Cheight-BtnWidth, BtnWidth, BtnWidth, false );
			
			BtnA->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
			BtnB->SetBtnImage( TID_None, TID_None, TID_None, TID_None );

			ThumbW = Cw;//Thumb가로폭;
			ThumbH = ThumbWidth;
			SldMinY = BtnWidth; 
			SldMaxY = (Ch - BtnWidth) - ThumbH;
		}
		break;
    }

//	BtnA->SetImgBtn( true );
//	BtnB->SetImgBtn( true );
	
	SldValue = Value;
	SldMin = Min;
	SldMax = Max;
	
	SetThumbPos();
}

//-----------------------------------------------------------------------------
void CScrollBar::SetThumbPos()
{
    //max    : 최대 범위...
    //value  : 처음 초기값
    //Scope : 슬라이드바의 이동가능 범위
    //move_pos[1] : 초기값에 따른 썸의 초기 위치
	int Scope( 0 );
	
    switch( SlideType )
    {
    case stHorzSld:
		if( SldMax != 0 )
		{
			Scope  = (Cw - (Ch*2)) - ThumbW;
			ThumbX = (int)( ( Scope * SldValue ) / SldMax ) + Ch;// + Ch : 버튼의 폭을 더해준다
		}
		else if( SldMax == 0 )
		{
			ThumbX = Ch;
		}
		
		if( ThumbX <= SldMinX ){	ThumbX = SldMinX;	}
		if( ThumbX >= SldMaxX ){	ThumbX = SldMaxX;	}
		break;
    case stVertSld:
		if( SldMax != 0 )
		{
			Scope  = (Ch - (Cw*2)) - ThumbH;
			ThumbY = (int)( ( Scope * SldValue) / SldMax ) + Cw;// + Cw : 버튼의 폭을 더해준다
		}
		else if( SldMax == 0 )
		{
			ThumbY = Cw;
		}

		if( ThumbY <= SldMinY ){	ThumbY = SldMinY;	}
		if( ThumbY >= SldMaxY ){	ThumbY = SldMaxY;	}
		break;
    }
}

//-----------------------------------------------------------------------------
void CScrollBar::SetSlideMovePos( int Mx, int My )
{
	int Dx( 0 );
	int Dy( 0 );
	int Dw( 0 );
	int Dh( 0 );
	
	//변화된 이동 위치를 적용시킨다.
    switch( SlideType )
    {
    case stHorzSld:
		Dx = Cx + Ch;
		Dw = (Cx + Cw) - Ch;

        if( ( Mx > Dx ) && ( Mx < Dw ) ){	ThumbX += DeltaX;	}//마우스 x축의 상대 좌표
		else if( Mx < Dx ){	ThumbX = SldMinX;	}
		else if( Mx > Dw ){	ThumbX = SldMaxX;	}

		if( ThumbX <= SldMinX ){	ThumbX = SldMinX;	}
		if( ThumbX >= SldMaxX ){	ThumbX = SldMaxX;	}
        SlideValueRate();
        break;

    case stVertSld:
		Dy = Cy + Cw;
		Dh = (Cy + Ch) - Cw;	

        if( ( My > Dy ) && ( My < Dh ) ){	ThumbY += DeltaY;	}//마우스 y축의 상대 좌표
        else if( My < Dy ){	ThumbY = SldMinY;	}
		else if( My > Dh ){	ThumbY = SldMaxY;	}

		if( ThumbY <= SldMinY ){	ThumbY = SldMinY;	}
		if( ThumbY >= SldMaxY ){	ThumbY = SldMaxY;	}
        SlideValueRate();
        break;
    }
}

//-----------------------------------------------------------------------------
void CScrollBar::SlideValueRate()
{
	//max    : 최대 범위...
    //Delta  : 슬라이드바의 썸 위치
    //Scope  : 슬라이드바의 이동가능 범위
    //value  : 썸의 위치에 따른 값

    int Scope( 0 );
    int Delta( 0 );

    switch( SlideType )
    {
    case stHorzSld:
		Scope = ( (Cw - (Ch*2)) - ThumbW );
		Delta = ThumbX - Ch;// - Ch : 계산할때는 버튼의 폭을 빼줘야 된다.
		break;
    case stVertSld:
		Scope = ( (Ch - (Cw*2)) - ThumbH );
		Delta = ThumbY - Cw;// - Cw : 계산할때는 버튼의 폭을 빼줘야 된다.
		break;
    }
      
    if( Scope != 0 )
    {
        if( Delta >= Scope ){	Delta = Scope;	}
        SldValue = (int)( ( SldMax * Delta ) / Scope );
    }
}

//-----------------------------------------------------------------------------
void CScrollBar::SetAdjustPosition( int PosX, int PosY )
{
	int Cwidth = Cx + Cw;
	int Cheight = Cy + Ch;
	int BtnWidth( 0 );//버튼의 폭

	SRect SSize;	
	
	switch( SlideType )
    {
    case stHorzSld://slide의 이동가능 범위 설정
		{
			BtnWidth = Ch;
			SSize = SRect(Cx, Cy, BtnWidth, BtnWidth);
			BtnA->ResetSize(&SSize);
			SSize = SRect(Cwidth-BtnWidth, Cy, BtnWidth, BtnWidth);
			BtnB->ResetSize(&SSize);			
		}
		break;
    case stVertSld://slide의 이동가능 범위 설정
		{
			BtnWidth = Cw;
			SSize = SRect(Cx, Cy, BtnWidth, BtnWidth);
			BtnA->ResetSize(&SSize);
			SSize = SRect(Cx, Cheight-BtnWidth, BtnWidth, BtnWidth);
			BtnB->ResetSize(&SSize);	
		}
		break;
    }
}

//-----------------------------------------------------------------------------
void CScrollBar::ResetSize( SRect* size )
{
	/*
	Cx = size->x;
	Cy = size->y;
	Apos[0]	= size->x;
	Apos[1] = size->y;
	//*/	
	
	Cx = size->x;
	Cy = size->y;
	Cw = size->w;
	Ch = size->h;

	Apos[0]		= Cx;
	Apos[1]		= Cy;
	Csize[2]	= Cw;
	Csize[3]	= Ch;

	int Cwidth = Cx + Cw;
	int Cheight = Cy + Ch;
	int BtnWidth( 0 );//버튼의 폭

	SRect SSize;

	switch( SlideType )
    {
    case stHorzSld://slide의 이동가능 범위 설정
		{
			BtnWidth = Ch;
			SSize = SRect(Cx, Cy, BtnWidth, BtnWidth);
			BtnA->ResetSize(&SSize);
			SSize = SRect(Cwidth-BtnWidth, Cy, BtnWidth, BtnWidth);
			BtnB->ResetSize(&SSize);
		
			SldMinX = BtnWidth; 
			SldMaxX = (Cw - BtnWidth) - ThumbW;
		}
		break;
    case stVertSld://slide의 이동가능 범위 설정
		{
			BtnWidth = Cw;
			SSize = SRect(Cx, Cy, BtnWidth, BtnWidth);
			BtnA->ResetSize(&SSize);
			SSize = SRect(Cx, Cheight-BtnWidth, BtnWidth, BtnWidth);
			BtnB->ResetSize(&SSize);	
	
			SldMinY = BtnWidth; 
			SldMaxY = (Ch - BtnWidth) - ThumbH;
		}
		break;
    }
}

//-----------------------------------------------------------------------------
void CScrollBar::UpdateHandler()
{
	if( !isInit )
	{
		//SetAdjustPosition( 0, 0 );
		isInit = true;
	}
}

//-----------------------------------------------------------------------------
void CScrollBar::OnMouseFocus( int Mx, int My )
{
	Mx;	My;
	ThumbState = n_bsFocus;
}

//-----------------------------------------------------------------------------
void CScrollBar::OnMouseLButtonDown( int Mx, int My )
{
	Mx;	My;

	ThumbState = n_bsSelect;
	
	if( FocusPos == sbfClientFocus )
	{
		switch( SlideType )
		{
		case stHorzSld:
			ThumbX = Mx - Cx;
			ThumbX -= (int)( ThumbW / 2 );
			if( ThumbX <= SldMinX ){	ThumbX = SldMinX;	}
			if( ThumbX >= SldMaxX ){	ThumbX = SldMaxX;	}
			SlideValueRate();
			break;
		case stVertSld:
			ThumbY = My - Cy;
			ThumbY -= (int)( ThumbH / 2 );
			if( ThumbY <= SldMinY ){	ThumbY = SldMinY;	}
			if( ThumbY >= SldMaxY ){	ThumbY = SldMaxY;	}
			SlideValueRate();
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void CScrollBar::OnMouseLButtonPress( int Mx, int My )
{
	m_bIsBtnPress = true;

	long lElapsedTime = SAFE_TIME_SUB( g_nowTime , NowTime );
	if( lElapsedTime < 75 && lElapsedTime > 0 )
	{
		return;
	}
	else
	{
		NowTime = g_nowTime;
	}
	
	if( FocusPos == sbfBtnAFocus )
	{
		SldValue--;
		if( SldValue < SldMin ){	SldValue = SldMin;	}
		SetThumbPos();
	}
	if( FocusPos == sbfBtnBFocus )
	{
		SldValue++;
		
		if( SldValue > SldMax ){	SldValue = SldMax;	}
		SetThumbPos();		
	}
}

//-----------------------------------------------------------------------------
void CScrollBar::OnMouseLButtonUp( int Mx, int My )
{
	if(m_bIsBtnPress)
	{
		m_bIsBtnPress = false;
		return;
	}

	ThumbState = n_bsNormal;
}

//-----------------------------------------------------------------------------
void CScrollBar::OnMouseLDrag( int Mx, int My )
{
	if( FocusPos == sbfClientFocus || FocusPos == sbfThumbFocus )
	{
		SetSlideMovePos( Mx, My );
	}
}

//-----------------------------------------------------------------------------
void CScrollBar::OnMouseLDrop( int Mx, int My )
{
	if( FocusPos == sbfClientFocus || FocusPos == sbfThumbFocus )
	{
		SetSlideMovePos( Mx, My );
	}
}


void CScrollBar::ReSetScroll(int Min, int Max, int Value)
{	
	SldValue = Value;
	SldMin = Min;
	SldMax = Max;
	
	SetThumbPos();
}

void CScrollBar::Reposition( int x, int y )
{
	if( Pwnd != NULL )
	{
		int nDelX = x + Rpos[0] - Cx;
		int nDelY = y + Rpos[1] - Cy;
		
		Cx = x + Rpos[0];
		Cy = y + Rpos[1];

		Apos[0] = Cx;
		Apos[1] = Cy;

		BtnA->SetWndPositionR( nDelX, nDelY );
		BtnB->SetWndPositionR( nDelX, nDelY );		
	}
}

void CScrollBar::SetWndPositionR( int PosX, int PosY )
{
	int nDelX = Apos[0];
	int nDelY = Apos[1];	
	
	if(Pwnd != NULL)
	{
        SRect rect;
		Pwnd->GetClientSize(&rect);	
		
		Apos[0] = rect.x + Rpos[0];
		Apos[1] = rect.y + Rpos[1];
	}
	else
	{
		Apos[0] += PosX;
		Apos[1] += PosY;
	}
		
	Cx = Apos[0];
	Cy = Apos[1];
	Cw = Csize[2];
	Ch = Csize[3];

	nDelX = Apos[0] - nDelX;
	nDelY = Apos[1] - nDelY;
	
	BtnA->SetWndPositionR( nDelX, nDelY );
	BtnB->SetWndPositionR( nDelX, nDelY );
	
	SetAdjustPosition( PosX, PosY );
}

//-----------------------------------------------------------------------------
