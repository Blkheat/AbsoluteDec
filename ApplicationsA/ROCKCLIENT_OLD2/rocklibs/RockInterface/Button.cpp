///////////////////////////////////////////////////////////////////////////////
///
///		File		: Button.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "Button.h"

#include "FrameWnd.h"


#include "..\\..\\bravolibs\\Sound\\Sound.h"

extern long			g_nowTime;
#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

#ifndef _LEFT 

#define _LEFT 0
#define _TOP 1
#define _WIDTH 2
#define _HEIGHT 3
#define _TILE_SIZE 30
#define _TILE_SIZEX2 60

#endif

//-----------------------------------------------------------------------------
CButton::CButton( SDesktop* DT ) : CTextBox( DT )
{
	BtnState = n_bsNormal;
	IsImgBtn = false;
	IsHold = false;
	ReactionDelta = 0;
	
	Bx = 0;
	By = 0;
	Bw = 0;
	Bh = 0;

	Refx = 0;
	Refy = 0;

	Cimg = TID_None;
	BtnImg[n_bsNormal]	= TID_None;
	BtnImg[n_bsFocus]	= TID_None;
	BtnImg[n_bsSelect]	= TID_None;
	BtnImg[n_bsHold]	= TID_None;

	/*
	RBtnColor[A] = 255;
	RBtnColor[R] = 255;
	RBtnColor[G] = 255;
	RBtnColor[B] = 255;
	BtnColor = D3DCOLOR_ARGB( RBtnColor[A], RBtnColor[R], RBtnColor[G], RBtnColor[B] );
	*/

	NowTime = g_nowTime;
	
	IsClickTxt = true;
}

//-----------------------------------------------------------------------------
CButton::~CButton()
{
}

void CButton::SetEnable( bool Enable )
{
	isEnable = Enable;
	if(!Enable)
	{
		BtnState = n_bsNormal;
	}
}

void CButton::ResetSize( SRect* size )
{
	CTextBox::ResetSize(size);
	SetAdjustPosition( Cx, Cy );
}

//-----------------------------------------------------------------------------
SEventMessage* CButton::EventProc( SEventMessage* EMsg )
{
	
	if(!isEnable)
	  return EMsg;
	
	int x=0;
	CFrameWnd*	fwnd			= NULL;

	if( IsHold )
	{	
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_HOLD;
		return( EMsg );
	}

	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;	
	
	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
		OnMouseFocus( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_FOCUS;
		break;

	case MA_LDOWN:
		OnMouseLButtonDown( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_DOWN;
		break;

	case MA_LPRESS:
		OnMouseLButtonPress( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_PRESS;
		break;

	case MA_RUP:
		EMsg->FocusWnd = Iid;
		EMsg->Notify	= NM_BTN_RCLICK;
		break;	
		
	case MA_LUP:
		OnMouseLButtonUp( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_CLICK;
		
		g_Sound.Play( 3 );//버튼 클릭 사운드

		//현재.... 드래그 상태에서 UP으로 바뀔 경우....UP처리가 되지 않기때문에....임시 처리
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
void CButton::RenderProc()
{
	static int Rx( 0 );//Reaction X
	static int Ry( 0 );//Reaction Y

	UpdateHandler();

	Rx = Ax + ReactionDelta;
	Ry = Ay + ReactionDelta;
	
	if( IsImgBtn == false )
	{
		if( pDT->CurFocusWnd != Iid )
		{
			BtnState = n_bsNormal; 
		}
		if( IsHold )
		{
			BtnState = n_bsHold; 
		}
	}		
	
	Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	Render->Draw( BtnImg[BtnState], Bx, By, Bw, Bh, Ccolor );
	
	if( isMultiLine && m_IsMultiLineAsign )
	{
		DrawMutiAlignText();
	}
	else
	{
		Render->DrawAlignText( theFontg, Text, Rx, Ry, Tx, Ty, Tw, Th, Tcolor );
	}

	if(CExtraimg != TID_None)
	{
		Render->Draw( CExtraimg, Cx, Cy, Cw, Ch, Ccolor );	
	}

	return; 
}

//-----------------------------------------------------------------------------
WNDID CButton::FocusCheck( int Mx, int My, bool IsChildCheck )
{
	IsChildCheck;

	if( isFocusCheck )
	{
		int Bx = Cx + Refx; //BoundingBox
		int By = Cy + Refy;
		int Bw = (Cx + Refx ) + Cw;
		int Bh = (Cy + Refy ) + Ch;

		if( ( (Bx <= Mx) && (Bw >= Mx) ) && ( (By <= My) && (Bh >= My) ) )
		{
			if( isRegion )
			{
				if( Cimg == TID_None ){	return( Iid );	}

				int Lx = Mx - Bx;//Apos[0];
				int Ly = My - By;//Apos[1];
				return RegionFocusCheck( Lx, Ly );
			}
			else
			{
				return( Iid );
			}
		}
	}
	
	return( WID_None );
}

//-----------------------------------------------------------------------------
IRESULT CButton::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	int Iresult( 0 );
	Iresult = CTextBox::ReceiveEvent( Event, fParam, sParam, xParam, exParam );

	switch( Event )
	{
	case n_emSetHold:
		SetBtnHold( (bool)fParam );
		break;
	}

	return( Iresult );
}

//-----------------------------------------------------------------------------
void CButton::SetAdjustPosition( int PosX, int PosY )//OR
{
	//PosX; PosY;

	Bx = Cx + Refx;
	By = Cy + Refy;
	Bw = Cw;
	Bh = Ch;

	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;
	Tw = Cw - ( BlankRight * BlankLeft );
	Th = Ch - ( BlankBottom * BlankTop );
	
	Ax = GetAlignPosX();
	Ay = GetAlignPosY();
}

//-----------------------------------------------------------------------------
void CButton::SetBtnImage( TEXID Normal, TEXID Focus, TEXID Select, TEXID Hold )
{
	BtnImg[n_bsNormal]	= Normal;
	BtnImg[n_bsFocus]	= Focus;
	BtnImg[n_bsSelect]	= Select;
	BtnImg[n_bsHold]	= Hold;
}
/*
//-----------------------------------------------------------------------------
void CButton::SetBtnColor( UCHAR Red, UCHAR Green, UCHAR Blue )
{
	RBtnColor[R] = Red;
	RBtnColor[G] = Green;
	RBtnColor[B] = Blue;
	BtnColor = D3DCOLOR_ARGB( RBtnColor[A], RBtnColor[R], RBtnColor[G], RBtnColor[B] );
}

//-----------------------------------------------------------------------------
void CButton::SetBtnAlpha( UCHAR Alpha )
{
	RBtnColor[A] = Alpha;
	BtnColor = D3DCOLOR_ARGB( RBtnColor[A], RBtnColor[R], RBtnColor[G], RBtnColor[B] );
}
*/

//-----------------------------------------------------------------------------
void CButton::SetBtnHold( bool IsHold )
{
	this->IsHold = IsHold;

	if( IsHold )
	{
		BtnState = n_bsHold;
	}
	else
	{
		BtnState = n_bsNormal;
	}
}

//-----------------------------------------------------------------------------
void CButton::UpdateHandler()//OR
{
	if( !isInit )
	{
		SetAdjustPosition( 0, 0 );
		isInit = true;
	}
}

//-----------------------------------------------------------------------------
void CButton::OnMouseFocus( int Mx, int My )
{
	BtnState = n_bsFocus;
}

//-----------------------------------------------------------------------------
void CButton::OnMouseLButtonDown( int Mx, int My )
{
	BtnState = n_bsSelect;
	
	if(IsClickTxt)
	{
		ReactionDelta = 1;
	}
}

//-----------------------------------------------------------------------------
void CButton::OnMouseLButtonPress( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CButton::OnMouseLButtonUp( int Mx, int My )
{
	BtnState = n_bsFocus;
	
	if(IsClickTxt)
	{
		ReactionDelta = 0;
	}	
}

//-----------------------------------------------------------------------------
void CButton::SetBtnState( NButtonState aState )
{
	BtnState = aState;
}

//-----------------------------------------------------------------------------
void CButton::SetImgBtn( bool aIsImgBtn )
{
	IsImgBtn = aIsImgBtn;
}

//-----------------------------------------------------------------------------
void CButton::SetBtnPosWnd( int addBx, int addBy )
{
	Refx = addBx;
	Refy = addBy;
	Bx = Cx + Refx;
	By = Cy + Refy;
}

//-----------------------------------------------------------------------------

///---------------------------------------------------------------------------
///-- CImageBox9
///---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
CButton9::CButton9( SDesktop* DT ) : CButton( DT )
{
	
}

//-----------------------------------------------------------------------------
CButton9::~CButton9()
{
}

///---------------------------------------------------------------------------
///-- RenderProc
///---------------------------------------------------------------------------
void CButton9::RenderProc()
{
	int i;	
	
	if( IsImgBtn == false )
	{
		if( pDT->CurFocusWnd != Iid )
		{
			BtnState = n_bsNormal; 
		}
		if( IsHold )
		{
			BtnState = n_bsHold; 
		}
	}
	
	for( i = 0 ; i < 9; ++i )
	{
		if( BtnImg[BtnState] != TID_None )
		{
			Render->Draw( BtnImg[BtnState] + i, maal_tile[i][_LEFT], maal_tile[i][_TOP], 
				maal_tile[i][_WIDTH], maal_tile[i][_HEIGHT], Ccolor );
		}
	}
}

///---------------------------------------------------------------------------
///-- SetWndTileSizeLR
///---------------------------------------------------------------------------
void CButton9::SetWndTileSizeLR( const int al_left, const int al_right )
{
	ml_Wnd_width[0] = al_left;
	ml_Wnd_width[1] = al_right;
}
///---------------------------------------------------------------------------
///-- SetWndTileSizeTB
///---------------------------------------------------------------------------
void CButton9::SetWndTileSizeTB( const int al_top, const int al_bottom )
{
	ml_Wnd_height[0] = al_top;
	ml_Wnd_height[1] = al_bottom;
}
///---------------------------------------------------------------------------
///-- SetAdjustPosition
///---------------------------------------------------------------------------
void CButton9::SetAdjustPosition( int PosX, int PosY )//OR
{
 	// 원 루틴

	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;

	Tw = Cw - ( BlankRight + BlankLeft );
	Th = Ch - ( BlankBottom + BlankTop );

	Ax = GetAlignPosX();
	Ay = GetAlignPosY();
	
	int ll_stretch_width = Cw - ml_Wnd_width[0] - ml_Wnd_width[1];
	int ll_stretch_height= Ch - ml_Wnd_height[0] - ml_Wnd_height[1]; 
	
	
	maal_tile[0][_LEFT] = maal_tile[3][_LEFT] = maal_tile[6][_LEFT] = Cx;
	maal_tile[1][_LEFT] = maal_tile[4][_LEFT] = maal_tile[7][_LEFT] = Cx + ml_Wnd_width[0];
	maal_tile[2][_LEFT] = maal_tile[5][_LEFT] = maal_tile[8][_LEFT] = maal_tile[1][_LEFT] + ll_stretch_width;
	
	maal_tile[0][_TOP] = maal_tile[1][_TOP] = maal_tile[2][_TOP] = Cy;
	maal_tile[3][_TOP] = maal_tile[4][_TOP] = maal_tile[5][_TOP] = Cy + ml_Wnd_height[0];
	maal_tile[6][_TOP] = maal_tile[7][_TOP] = maal_tile[8][_TOP] = maal_tile[3][_TOP] + ll_stretch_height;
	
	maal_tile[0][_WIDTH] = maal_tile[3][_WIDTH] = maal_tile[6][_WIDTH] = ml_Wnd_width[0];
	maal_tile[1][_WIDTH] = maal_tile[4][_WIDTH] = maal_tile[7][_WIDTH] = ll_stretch_width ;
	maal_tile[2][_WIDTH] = maal_tile[5][_WIDTH] = maal_tile[8][_WIDTH] = ml_Wnd_width[1];
	
	maal_tile[0][_HEIGHT] = maal_tile[1][_HEIGHT] = maal_tile[2][_HEIGHT] = ml_Wnd_height[0];
	maal_tile[3][_HEIGHT] = maal_tile[4][_HEIGHT] = maal_tile[5][_HEIGHT] = ll_stretch_height ;
	maal_tile[6][_HEIGHT] = maal_tile[7][_HEIGHT] = maal_tile[8][_HEIGHT] = ml_Wnd_height[1];
	
}

