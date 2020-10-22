///////////////////////////////////////////////////////////////////////////////
///
///		File		: Spin.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "Spin.h"

#include "FrameWnd.h"

//-----------------------------------------------------------------------------
CSpin::CSpin( SDesktop* DT ) : CTextBox( DT )
{
	theUpBtn	= SAFE_NEW( CButton( DT ) );
	theDownBtn	= SAFE_NEW( CButton( DT ) );

	theUpHold	= false;
	theDownHold	= false;

	theValue	= 0;
	theMaxValue	= 0;
	theMinValue	= 0;
	theIsText	= true;

	theFocusFixed = n_spSpinClient;
}

//-----------------------------------------------------------------------------
CSpin::~CSpin()
{
	SAFE_DELETE(theUpBtn);
	SAFE_DELETE(theDownBtn);
}

//-----------------------------------------------------------------------------
SEventMessage* CSpin::EventProc( SEventMessage* EMsg )
{
	CFrameWnd*	fwnd			= NULL;

	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;
	

	if( theFocusPos == n_spDownBtnFocus )
	{
		theDownBtn->EventProc( EMsg );
		if( EMsg->Notify == NM_BTN_DOWN )
		{
			theFocusFixed = n_spDownBtnFocus;
		}
	}
	if( theFocusPos == n_spUpBtnFocus )
	{
		theUpBtn->EventProc( EMsg );
		if( EMsg->Notify == NM_BTN_DOWN )
		{
			theFocusFixed = n_spUpBtnFocus;
		}
	}
	
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
	case MA_LUP:
		if( EMsg->Notify != NM_BTN_HOLD )
		{
			OnMouseLButtonUp( Mx, My );
				
			EMsg->Notify	= NM_NONE;
			EMsg->FocusWnd	= Iid;
			EMsg->fParam	= theValue;

			if( ( theFocusPos == n_spDownBtnFocus ) && ( theFocusFixed == n_spDownBtnFocus ) )
			{
				EMsg->Notify = NM_SPIN_DOWN;
			}
			if( ( theFocusPos == n_spUpBtnFocus ) && ( theFocusFixed == n_spUpBtnFocus ) )
			{
				EMsg->Notify = NM_SPIN_UP;
			}
		}

		theFocusFixed = n_spSpinClient;

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
void CSpin::RenderProc()
{
	UpdateHandler();

	if( theIsText )
	{
		Tx = Cx + BlankLeft;
		Ty = Cy + BlankTop;
		Tw = Cw - ( ( BlankRight + BlankLeft ) + Ch );
		Th = Ch - ( BlankBottom + BlankTop );
		Ax = GetAlignPosX();
		Ay = GetAlignPosY();
	
		Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
		Render->DrawAlignText( theFontg, Text, Ax, Ay, Tx, Ty, Tw, Th, Tcolor );
	}



	switch( theFocusPos )
	{
	case n_spUpBtnFocus:
		theUpBtn->RenderProc();
		break;
	case n_spDownBtnFocus:
		theDownBtn->RenderProc();
		break;
	}

	theUpBtn->RenderProc();
	theDownBtn->RenderProc();
	/*
	if( theUpHold )
	{
		theUpBtn->RenderProc();
	}
	if( theDownHold )
	{
		theDownBtn->RenderProc();
	}*/
}

//-----------------------------------------------------------------------------
IRESULT CSpin::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	int Iresult( 0 );
	Iresult = CTextBox::ReceiveEvent( Event, fParam, sParam, xParam, exParam );

	switch( Event )
	{
	case n_emUpBtnDisable:
		theUpHold = true;
		theUpBtn->SetBtnHold( true );		
		break;

	case n_emDownBtnDisable:
		theDownHold = true;
		theDownBtn->SetBtnHold( true );
		break;

	case n_emUpBtnEnable:
		theUpHold = false;
		theUpBtn->SetBtnHold( false );
		break;

	case n_emDownBtnEnable:
		theDownHold = false;
		theDownBtn->SetBtnHold( false );
		break;
	}

	return( Iresult );
}

//-----------------------------------------------------------------------------
WNDID CSpin::FocusCheck( int Mx, int My, bool IsChildCheck )
{
	IsChildCheck;

	if( isFocusCheck )
	{
		//Client BoundingBox 계산
		int Bx = Cx; 
		int By = Cy;
		int Bw = Cw + Cx;
		int Bh = Ch + Cy;

		if( ( (Bx <= Mx) && (Bw >= Mx) ) && ( (By <= My) && (Bh >= My) ) )
		{
			theFocusPos = n_spSpinClient;

			if( theDownBtn->FocusCheck( Mx, My, false ) != WID_None )
			{
				theFocusPos = n_spDownBtnFocus;
			}
			if( theUpBtn->FocusCheck( Mx, My, false ) != WID_None )
			{
				theFocusPos = n_spUpBtnFocus;
			}

			return( Iid );
		}
	}
	
	return( WID_None );
}

//-----------------------------------------------------------------------------
void CSpin::SetAdjustPosition( int PosX, int PosY )//OR
{
	PosX, PosY;

	int BtnUp_x( 0 );
	int BtnUp_y( 0 );
	int BtnDown_x( 0 );
	int BtnDown_y( 0 );
	

	if( theIsText )
	{
		Tx = Cx + BlankLeft;
		Ty = Cy + BlankTop;
		Tw = Cw - ( ( BlankRight + BlankLeft ) + Ch );
		Th = Ch - ( BlankBottom + BlankTop );
		//Ax = GetAlignPosX();
		//Ay = GetAlignPosY();

		BtnUp_x		= Cx + Cw - ( Ch );
		BtnUp_y		= Cy;
		
		BtnDown_x	= BtnUp_x;
		BtnDown_y	= Cy + ( Ch / 2 );
		
	}
	else
	{
		BtnUp_x		= Cx;
		BtnUp_y		= Cy;
		
		BtnDown_x	= BtnUp_x;
		BtnDown_y	= Cy + ( Ch / 2 );
		
	}

	theUpBtn->SetWndPositionA( BtnUp_x, BtnUp_y );
	theDownBtn->SetWndPositionA( BtnDown_x, BtnDown_y );
}

//-----------------------------------------------------------------------------
void CSpin::InitSpin( int Value, int Min, int Max, bool IsText, int al_bt_width, int al_btheight )
{
	theValue	= Value;
	theMinValue	= Min;
	theMaxValue	= Max;
	theIsText	= IsText;

	SetAdjustPosition( 0, 0 );

	int BtnUp_x( 0 );
	int BtnUp_y( 0 );
	int BtnUp_w( 0 );
	int BtnUp_h( 0 );
	int BtnDown_x( 0 );
	int BtnDown_y( 0 );
	int BtnDown_w( 0 );
	int BtnDown_h( 0 );
	

	if( theIsText )
	{
		Tx = Cx + BlankLeft;
		Ty = Cy + BlankTop;
		Tw = Cw - ( ( BlankRight + BlankLeft ) + Ch );
		Th = Ch - ( BlankBottom + BlankTop );
		
		BtnUp_x		= Cx + Cw - ( Ch );
		BtnUp_y		= Cy;
		BtnUp_w		= Ch;
		BtnUp_h		= Ch / 2;
	
		BtnDown_x	= BtnUp_x;
		BtnDown_y	= Cy + ( Ch / 2 );
		BtnDown_w	= Ch;
		BtnDown_h	= Ch / 2;
		
	}
	else
	{
		BtnUp_x		= Cx;
		BtnUp_y		= Cy;
		BtnUp_w		= Cw;
		BtnUp_h		= Ch / 2;
		
		BtnDown_x	= Cx;
		BtnDown_y	= Cy + ( Ch / 2 );
		BtnDown_w	= Cw;
		BtnDown_h	= Ch / 2;
	}
	
	if( al_bt_width )
	{
		BtnUp_w = al_bt_width;
		BtnDown_w = al_bt_width;
	}

	if( al_btheight )
	{
		BtnUp_h = al_btheight;
		BtnDown_h = al_btheight;
	}
	
	
	theUpBtn->CreateWnd( n_wtButton, WID_None, WID_None, BtnUp_x, BtnUp_y, BtnUp_w, BtnUp_h, false );
	theUpBtn->SetID( Iid );
	//theUpBtn->SetBtnImage( TID_None, TID_None, TID_None, TID_None );

	theDownBtn->CreateWnd( n_wtButton, WID_None, WID_None, BtnDown_x, BtnDown_y, BtnDown_w, BtnDown_h, false );
	theDownBtn->SetID( Iid );
	//theDownBtn->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
	
	static TCHAR str[40] = {0,};
	Rsprintf( RWCHAR(str), _RT("%d"), theValue );
	SetText( str );
}

//-----------------------------------------------------------------------------
int CSpin::GetValue()
{
	return( theValue );
}

//-----------------------------------------------------------------------------
void CSpin::SetValue( int Value )
{
	theValue = Value;

	SetDownHold( false );
	SetUpHold( false );

	if( theValue <= theMinValue ){	SetDownHold( true );	}
	if( theValue >= theMaxValue ){	SetUpHold( true );		}
	

	static TCHAR str[40] = {0,};
	Rsprintf( RWCHAR(str), _RT("%d"), theValue );
	SetText( str );
}

//-----------------------------------------------------------------------------
void CSpin::SetValue( int Value, int Min, int Max )
{
	theValue	= Value;
	theMinValue	= Min;
	theMaxValue	= Max;

	SetDownHold( false );
	SetUpHold( false );

	if( theValue <= theMinValue ){	SetDownHold( true );	}
	if( theValue >= theMaxValue ){	SetUpHold( true );		}

	
	static TCHAR str[40] = {0,};
	Rsprintf( RWCHAR(str), _RT("%d"), theValue );
	SetText( str );
}

//-----------------------------------------------------------------------------
void CSpin::SetValue( int Min, int Max )
{
	theMinValue	= Min;
	theMaxValue	= Max;

	SetDownHold( false );
	SetUpHold( false );

	if( theValue <= theMinValue ){	SetDownHold( true );	}
	if( theValue >= theMaxValue ){	SetUpHold( true );		}
}

//-----------------------------------------------------------------------------
void CSpin::SetUpHold( bool hold )
{
	theUpHold = hold;
	theUpBtn->SetBtnHold( theUpHold );
}

//-----------------------------------------------------------------------------
void CSpin::SetDownHold( bool hold )
{
	theDownHold = hold;
	theDownBtn->SetBtnHold( theDownHold );
}

//-----------------------------------------------------------------------------
void CSpin::OnMouseFocus( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CSpin::OnMouseLButtonDown( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CSpin::OnMouseLButtonPress( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CSpin::OnMouseLButtonUp( int Mx, int My )
{
	if( ( theFocusPos == n_spDownBtnFocus ) && ( theFocusFixed == n_spDownBtnFocus ) )
	{
		theValue -= 1;
		if( theValue <= theMinValue)
		{
			theValue = theMinValue;
			SetDownHold( true );
		}
	}
	if( ( theFocusPos == n_spUpBtnFocus ) && ( theFocusFixed == n_spUpBtnFocus ) )
	{
		theValue += 1;
		if( theValue >= theMaxValue )
		{
			theValue = theMaxValue;
			SetUpHold( true );
		}
	}

	if( theValue > theMinValue ){	SetDownHold( false );	}
	if( theValue < theMaxValue ){	SetUpHold( false );		}


	static TCHAR str[40] = {0,};
	Rsprintf( RWCHAR(str), _RT("%d"), theValue );
	SetText( str );
}

//-----------------------------------------------------------------------------
void CSpin::SetUpBtnImage( TEXID Normal, TEXID Focus, TEXID Select, TEXID Hold )
{
	theUpBtn->SetBtnImage( Normal, Focus, Select, Hold );
}

//-----------------------------------------------------------------------------
void CSpin::SetDownBtnImage( TEXID Normal, TEXID Focus, TEXID Select, TEXID Hold )
{
	theDownBtn->SetBtnImage( Normal, Focus, Select, Hold );
}

//-----------------------------------------------------------------------------

