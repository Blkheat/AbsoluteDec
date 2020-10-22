///////////////////////////////////////////////////////////////////////////////
///
///		File		: SlideBar.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "SlideBar.h"
#include "FrameWnd.h"


//-----------------------------------------------------------------------------
CSlideBar::CSlideBar( SDesktop* DT ) : CWnd( DT )
{
	SlideType = stHorzSld;// stHorzSld(수평 슬라이드), stVertSld(수직 슬라이드)
	
	ThumbState = n_bsNormal;
	ThumbImg[n_bsNormal] = TID_None;
	ThumbImg[n_bsFocus] = TID_None;
	ThumbImg[n_bsSelect] = TID_None;
	ThumbImg[n_bsHold] = TID_None;

	ThumbX = 0;//썸의 Slide에서의 이동 위치
	ThumbY = 0;
	ThumbW = 0;//썸의 크기....Thumb(엄지손가락), Shaft(몸통), Arrow(화살표)
	ThumbH = 0;

	SldValue = 0;//현재 슬라이드바 위치의 값
	SldMin = 0;//설정값의 범위(최소, 최대)
	SldMax = 0;

	isThumbFocus = false;

	SldMinX = 0;//Slide의 이동 가능 범위
	SldMaxX = 0;
	SldMinY = 0;
	SldMaxY = 0;
}

//-----------------------------------------------------------------------------
CSlideBar::~CSlideBar()
{
}

//-----------------------------------------------------------------------------
SEventMessage* CSlideBar::EventProc( SEventMessage* EMsg )
{
#ifdef HHM_SLIDEBAR_ENABLE// 활성화에 따라 EventProc() 변경(2009/05/13)

	if(!isEnable)
		return EMsg;

#endif// #define HHM_SLIDEBAR_ENABLE// 활성화에 따라 EventProc() 변경(2009/05/13)

	CFrameWnd* fwnd = NULL;

	DeltaX = EMsg->Mouse.Rposition.x;
	DeltaY = EMsg->Mouse.Rposition.y;

	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;

	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
		OnMouseFocus( Mx, My );
		break;
	case MA_LDOWN:
		OnMouseLButtonDown( Mx, My );
		EMsg->FocusWnd = Iid;
		EMsg->Notify = NM_SLD_MOVEBAR;
		EMsg->fParam = Iid;
		EMsg->sParam = SldValue;
	
		break;

	case MA_LPRESS:

		OnMouseLButtonPress( Mx, My );
		EMsg->FocusWnd  = Iid;
		EMsg->Notify	= NM_SLD_MOVEBAR;
		EMsg->fParam	= Iid;
		EMsg->sParam	= SldValue;

		break;
	case MA_LUP:
		OnMouseLButtonUp( Mx, My );
		EMsg->FocusWnd  = Iid;
		EMsg->Notify	= NM_SLD_MOVEBAR;
		EMsg->fParam	= Iid;
		EMsg->sParam	= SldValue;
		break;

	case MA_LDRAG:
		OnMouseLDrag( Mx, My );
		EMsg->FocusWnd  = Iid;
		EMsg->Notify	= NM_SLD_MOVEBAR;
		EMsg->fParam	= Iid;
		EMsg->sParam	= SldValue;

		fwnd = (CFrameWnd*)Pwnd;
		
		if( ( GetParentID() != WID_None ) && ( fwnd->theWnd != NULL ) )
		{
			fwnd->theWnd->Proc( EMsg );
		}
		
		break;
	case MA_LDROP:
		OnMouseLDrop( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_SLD_MOVEBAR;
		EMsg->fParam	= Iid;
		EMsg->sParam	= SldValue;

		fwnd = (CFrameWnd*)Pwnd;
		
		if( ( GetParentID() != WID_None ) && ( fwnd->theWnd != NULL ) )
		{
			fwnd->theWnd->Proc( EMsg );
		}
	
		break;
	}

	return( EMsg );
}

//-----------------------------------------------------------------------------
void CSlideBar::RenderProc()
{
	UpdateHandler();

	Render->DrawState();	
	Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );

	DrawThumb();
}

//-----------------------------------------------------------------------------
IRESULT CSlideBar::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
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
	case n_emResetSldExtentSync://바의 길이를 창과 내용의 길이에 따라..동기화 시키는 메세지( 지금 안씀 )
		break;
	}

	return( Iresult );
}

//-----------------------------------------------------------------------------
WNDID CSlideBar::FocusCheck( int Mx, int My, bool IsChildCheck )
{
	IsChildCheck;

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
			isThumbFocus = false;

			if( ( (Tx <= Mx) && (Tw >= Mx) ) && ( (Ty <= My) && (Th >= My) ) )
			{
				isThumbFocus = true;
			}

			return( Iid );
		}
	}
	
	return( WID_None );
}

//-----------------------------------------------------------------------------
void CSlideBar::DrawThumb()
{
	Render->Draw( ThumbImg[ThumbState], ( Cx + ThumbX ), ( Cy + ThumbY ), ThumbW, ThumbH, D3DCOLOR_ARGB( Rcolor[A] , 255, 255, 255 ) );
}

//-----------------------------------------------------------------------------
void CSlideBar::SetThumbImage( TEXID Normal, TEXID Focus, TEXID Down, TEXID Hold )
{
	/*
	if( Normal == TID_None ){	Normal = TID_NORMAL_SLD;	}
	if( Focus == TID_None ){	Focus = Normal;	}
	if( Down == TID_None ){	Down = TID_SELECT_SLD;	}
	if( Hold == TID_None ){	Hold = Normal;	}
//*/
	ThumbImg[n_bsNormal]	= Normal;
	ThumbImg[n_bsFocus]		= Focus;
	ThumbImg[n_bsSelect]	= Down;
	ThumbImg[n_bsHold]		= Hold;
}

//-----------------------------------------------------------------------------
void CSlideBar::SetThumbSize()
{
	switch( SlideType )
    {
    case stHorzSld:
		ThumbW = (int)( Ch / 2 );
		ThumbH = Ch;
		break;
    case stVertSld:
		ThumbW = Cw;
		ThumbH = (int)( Cw / 2 );
		break;
    }
}

//-----------------------------------------------------------------------------
void CSlideBar::InitSlide( enSlideType SlideType, int Min, int Max, int Value, int ThumbWidth )
{
	this->SlideType = SlideType;

	switch( SlideType )
    {
    case stHorzSld://slide의 이동가능 범위 설정
		ThumbW = ThumbWidth;
		ThumbH = Ch;
		SldMinX = 0; 
		SldMaxX = Cw - ThumbW;
		break;
    case stVertSld://slide의 이동가능 범위 설정
		ThumbW = Cw;
		ThumbH = ThumbWidth;
		SldMinY = 0; 
		SldMaxY = Ch - ThumbH;
		break;
    }

	SldValue = Value;
	SldMin = Min;
	SldMax = Max;
	
	SetThumbPos();
}

//-----------------------------------------------------------------------------
void CSlideBar::SetThumbPos()
{
    //max    : 최대 범위...
    //value  : 처음 초기값
    //Scope : 슬라이드바의 이동가능 범위
    //move_pos[1] : 초기값에 따른 썸의 초기 위치

	int Scope( 0 );

    switch( SlideType )
    {
    case stHorzSld:
		Scope  = Cw - ThumbW;
		ThumbX = (int)( ( Scope * SldValue ) / SldMax );
		break;
    case stVertSld:
		Scope  = Ch - ThumbH;
		ThumbY = (int)( ( Scope * SldValue) / SldMax );
		break;
    }
}

//-----------------------------------------------------------------------------
void CSlideBar::SlideValueRate()
{
    int Scope( 0 );
    int Delta( 0 );

    switch( SlideType )
    {
    case stHorzSld:
		Scope = Cw - ThumbW;
		Delta = ThumbX;
		break;
    case stVertSld:
		Scope = Ch - ThumbH;
		Delta = ThumbY;
		break;
    }
      
    //max    : 최대 범위...
    //delta  : 슬라이드바의 썸 위치
    //Scope : 슬라이드바의 이동가능 범위
    //value  : 썸의 위치에 따른 값
    if( Scope != 0 )
    {
        if( Delta >= Scope ){	Delta = Scope;	}
        SldValue = (int)( ( SldMax * Delta ) / Scope );
	}
}

//-----------------------------------------------------------------------------
void CSlideBar::GetValue( int* Value )
{
    *Value = this->SldValue;
}

//-----------------------------------------------------------------------------
void CSlideBar::GetValue( int* Value, int* Max )
{
    *Value = SldValue;
    *Max   = SldMax;
}

//-----------------------------------------------------------------------------
void CSlideBar::SetValue( int Value )
{
	if( Value <= SldMin ){	Value = SldMin;	}
	if( Value >= SldMax ){	Value = SldMax;	}

    SldValue = Value;
    SetThumbPos();
}

//-----------------------------------------------------------------------------
void CSlideBar::SetScope( int Min, int Max )
{
	SldMin = Min;//설정상 기본적으로 항상 0...
	SldMax = Max;

	if( Max == 0 ){	SldValue = 0;	}
	SetValue( SldValue );
}

//-----------------------------------------------------------------------------
void CSlideBar::ResetSldExtentSync( int BasisValue, int RelativeValue )
{
	if( BasisValue > RelativeValue )//창크기가 내용보다 길다면 
	{
		switch( SlideType )
		{
		case stHorzSld:
			break;
		case stVertSld:
			break;
		}
	}
	else if( BasisValue < RelativeValue )//창크기 보다 내용이 길다면
	{
		switch( SlideType )
		{
		case stHorzSld:
			break;

		case stVertSld:
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void CSlideBar::SetSlideMovePos( int Mx, int My )
{
	int Dx = Cx;
	int Dy = Cy;
	int Dw = Cx + Cw;
	int Dh = Cy + Ch;

	//변화된 이동 위치를 적용시킨다.
    switch( SlideType )
    {
    case stHorzSld:
        if( ( Mx > Dx ) && ( Mx < Dw ) ){	ThumbX += DeltaX;	}//마우스 x축의 상대 좌표
		else if( Mx < Dx ){	ThumbX = SldMinX;	}
		else if( Mx > Dw ){	ThumbX = SldMaxX;	}

		if( ThumbX <= SldMinX ){	ThumbX = SldMinX;	}
		if( ThumbX >= SldMaxX ){	ThumbX = SldMaxX;	}
        SlideValueRate();
        break;

    case stVertSld:
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
void CSlideBar::SetAdjustPosition( int PosX, int PosY )
{
	PosX, PosY;
}

//-----------------------------------------------------------------------------
void CSlideBar::UpdateHandler()
{
	if( !isInit )
	{
		SetAdjustPosition( 0, 0 );
		isInit = true;
	}
}

//-----------------------------------------------------------------------------
void CSlideBar::OnMouseFocus( int Mx, int My )
{
	Mx;	My;

	ThumbState = n_bsFocus;
}

//-----------------------------------------------------------------------------
void CSlideBar::OnMouseLButtonDown( int Mx, int My )
{
	Mx;	My;

	if( !isThumbFocus )
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
	ThumbState = n_bsSelect;
}

//-----------------------------------------------------------------------------
void CSlideBar::OnMouseLButtonPress( int Mx, int My )
{
	Mx;	My;
}

//-----------------------------------------------------------------------------
void CSlideBar::OnMouseLButtonUp( int Mx, int My )
{
	Mx;	My;
	
	ThumbState = n_bsNormal;
}

//-----------------------------------------------------------------------------
void CSlideBar::OnMouseLDrag( int Mx, int My )
{
	SetSlideMovePos( Mx, My );
}

//-----------------------------------------------------------------------------
void CSlideBar::OnMouseLDrop( int Mx, int My )
{
	SetSlideMovePos( Mx, My );
}

void CSlideBar::Reposition( int x, int y )
{
	CWnd::Reposition( x, y );
}

void CSlideBar::SetWndPositionR( int PosX, int PosY )
{
	CWnd::SetWndPositionR(PosX, PosY);
}

//-----------------------------------------------------------------------------




/*
//-----------------------------------------------------------------------------
void CSlideBar::ResetSldExtentSync( int BasisValue, int RelativeValue )
{
	if( BasisValue > RelativeValue )//창크기가 내용보다 길다면 
	{
		switch( SlideType )
		{
		case stHorzSld:
			//썸의 크기 설정
			ThumbPos[0] = 0;
			ThumbSize[2] = Csize[2];
        
			SldMinX = 0;//slide의 이동가능 범위 설정
			SldMaxX = 0; 
			break;
		case stVertSld:
			//썸의 크기 설정
			ThumbPos[1] = 0;
			ThumbSize[3] = Csize[3] - 8;
        
			SldMinY = 0;//slide의 이동가능 범위 설정
			SldMaxY = 0; 
			break;
		}
	}
	else if( BasisValue < RelativeValue )//창크기 보다 내용이 길다면
	{
		switch( SlideType )
		{
		case stHorzSld:
			//썸의 크기 설정(비례식을 이용)
			ThumbSize[2] = (int)( ( Csize[2] * BasisValue ) / RelativeValue );
    
			//slide의 이동가능 범위 설정
			SldMinX = 0; 
			SldMaxX = (SldMinX + Csize[2]) - ThumbSize[2];
    
			//현재 슬라이드바 초기위치의 값
			SldMax = RelativeValue - BasisValue;
			//if( tparam==1 )
			//{
			//	SldValue = SldMax;
			//	SlideInitPos();
			//}
    
			//썸의 최소크기 설정
			if( ThumbSize[2] <= 4 ){	ThumbSize[2] = 4;	}
			break;

		case stVertSld:
			//썸의 크기 설정(비례식을 이용)
			ThumbSize[3] = (int)( ( Csize[3] * BasisValue ) / RelativeValue);
    
			//slide의 이동가능 범위 설정
			SldMinY = 0; 
			SldMaxY = ( SldMinY + Csize[3] ) - ThumbSize[3];
    
			//현재 슬라이드바 초기위치의 값
			SldMax = RelativeValue - BasisValue;
			//if( tparam==1 )
			//{
			//	SldValue = SldMax;
			//	SlideInitPos();
			//}
    
			//썸의 최소크기 설정
			if( ThumbSize[3] <= 1 ){	ThumbSize[3] = 1;	}
			break;
		}
	}
}
//*/