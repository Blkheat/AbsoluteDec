///////////////////////////////////////////////////////////////////////////////
///
///		File		: CheckButton.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "CheckButton.h"
#include "FrameWnd.h"


//-----------------------------------------------------------------------------
CCheckButton::CCheckButton( SDesktop* DT ) : CButton( DT )
{
	CheckMark = n_cmUnChecked;
	
	IsDirectCheck = true;
}

//-----------------------------------------------------------------------------
CCheckButton::~CCheckButton()
{
}

void CCheckButton::SetDirectCheck(bool bFlag)
{
	IsDirectCheck = bFlag;
}

//-----------------------------------------------------------------------------
SEventMessage* CCheckButton::EventProc( SEventMessage* EMsg )
{
	CFrameWnd*	fwnd			= NULL;

	if( IsHold )
	{
		return( EMsg );
	}

	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;

	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
		OnMouseFocus( Mx, My );
		break;
	case MA_LDOWN:
		OnMouseLButtonDown( Mx, My );
		break;
	case MA_LPRESS:
		OnMouseLButtonPress( Mx, My );
		break;
	//case MA_LDRAG_END:
	case MA_LUP:
		OnMouseLButtonUp( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_CLICK;
		
		//현재.... 드래그 상태에서 UP으로 바뀔 경우....UP처리가 되지 않기때문에....
		if( pDT->SelectedWnd != WID_None )
		{
			fwnd = (CFrameWnd*)Pwnd;
			if( ( GetParentID() != WID_None ) && ( fwnd->theWnd != NULL ) )
			{
				fwnd->theWnd->Proc( EMsg );
				EMsg->Notify = NM_NONE;
				return( NULL );
			}
		}
		break;
	}

	return( EMsg );
} 

//-----------------------------------------------------------------------------
void CCheckButton::RenderProc()
{
	UpdateHandler();

	Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	Render->Draw( BtnImg[BtnState], Bx, By, Bw, Bh, Ccolor );
	Render->DrawAlignText( theFontg, Text, Ax, Ay, Tx, Ty, Tw, Th, Tcolor );
}

//-----------------------------------------------------------------------------
IRESULT CCheckButton::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	int Iresult( 0 );
	Iresult = CWnd::ReceiveEvent( Event, fParam, sParam, xParam, exParam );

	switch( Event )
	{
	case n_emSetChecked:
		SetChecked();
		break;
	case n_emSetUnChecked:
		SetUnChecked();
		break;
	case n_emGetCheckMark:
		Iresult = GetCheckMark();
		break;
	}

	return( Iresult );
}

//-----------------------------------------------------------------------------
void CCheckButton::SetAdjustPosition( int PosX, int PosY )//OR
{
	PosX; PosY;

	Bx = Cx;
	By = Cy;
//	Bw = Ch;//체크박스의 가로폭은 크기는 세로폭에 맞춘다.
	Bw = Cw; // 카메라 워킹 관련 By wxywxy
	Bh = Ch;

	Tx = Cx + Ch + BlankLeft;
	Ty = Cy + BlankTop;
	Tw = Cw - ( BlankRight * BlankLeft );
	Th = Ch - ( BlankBottom * BlankTop );

	Ax = GetAlignPosX();
	Ay = GetAlignPosY();
}

//-----------------------------------------------------------------------------
void CCheckButton::InitCheckButton( NCheckMark CheckMark, TCHAR* Caption )
{
	if( CheckMark == n_cmChecked ){	SetChecked();	}
	if( CheckMark == n_cmUnChecked ){	SetUnChecked();	}

	SetText( Caption );
	SetBlank( 2, 0 );
	SetAlignText( n_atLeft, n_atCenter );

	SetAdjustPosition( 0, 0 );
}

//-----------------------------------------------------------------------------
void CCheckButton::SetChecked()
{
	CheckMark = n_cmChecked;
	BtnState = n_bsSelect;
}

//-----------------------------------------------------------------------------
void CCheckButton::SetUnChecked()
{
	CheckMark = n_cmUnChecked;
	BtnState = n_bsNormal;
}

//-----------------------------------------------------------------------------
NCheckMark CCheckButton::GetCheckMark()
{
	return( CheckMark );
}

//-----------------------------------------------------------------------------
void CCheckButton::OnMouseFocus( int Mx, int My )
{
	Mx;	My;
}

//-----------------------------------------------------------------------------
void CCheckButton::OnMouseLButtonDown( int Mx, int My )
{
	Mx;	My;
}

//-----------------------------------------------------------------------------
void CCheckButton::OnMouseLButtonPress( int Mx, int My )
{
	Mx;	My;
}

//-----------------------------------------------------------------------------
void CCheckButton::OnMouseLButtonUp( int Mx, int My )
{
	Mx;	My;
	if(IsDirectCheck)
	{
		if( GetCheckMark() == n_cmChecked )
		{	
			SetUnChecked();
		}
		else
		{
			SetChecked();
		}
	}
}

//-----------------------------------------------------------------------------