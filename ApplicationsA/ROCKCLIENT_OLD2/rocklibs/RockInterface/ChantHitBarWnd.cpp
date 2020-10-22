// ChantHitBarWnd.cpp: implementation of the CChantHitBarWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "..\\RockPCH.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"
#include "Rui.h"

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Spin.h"
#include "StateButton.h"
#include "ChatOutput.h"
#include "Slot.h"

#include "ChantHitBarWnd.h"


#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"


CChantHitBarWnd g_ChantHitBarWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChantHitBarWnd::CChantHitBarWnd()
{

}

CChantHitBarWnd::~CChantHitBarWnd()
{
	
}

//-----------------------------------------------------------------------------
void CChantHitBarWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CChantHitBarWnd::Composition()
{
	CFrameWnd*		fw;
//	CTextBox*		tb;	
	CProgressBar*	pb;
	CButton*		btn;
//	CStateButton*	sbtn;
//	CEditBox*		eb;
//	CSpin*			spin;
//	CImageBox*		img;
//	CScrollBar*		sb;


	int center_x	= nRui->Desktop.Width / 2;
//	int center_y	= nRui->Desktop.Height / 2;

	//-------------------------------------------------------------------------
	//
	//	영창 포인트 히트 바 (ChantPoint_HitBar)
	//
	//-------------------------------------------------------------------------
	// 영창 스킬 레벨에 따라서 히트 포인트가 변화된다.
	// 2 -> 1/2, 3 -> 3/1, 2/3, 4 -> 1/4, 2/4, 3/4, 5 -> 1/5, 2/5, 3/5, 4/5
	// 영창 히트바가 모두 채워지면 1 영창 게이지가 채워진다 
	// 영창 게이지는 레벨당 Level + 1 만큼 채워지게 된다. 

	SRect skillwnd;
	skillwnd.w	= 329;
	skillwnd.h	= 24;
	skillwnd.x	= center_x - ( skillwnd.w / 2 );
//	skillwnd.y	= center_y - ( skillwnd.h * 2 );
	skillwnd.y	= nRui->Desktop.Height - 150;

	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ChantHitBar_Wnd, WID_None, skillwnd.x, skillwnd.y, skillwnd.w, skillwnd.h, false );
	fw->SetClientImage( TID_ChantHitBar_Wnd );
	fw->SetCaption( _RT("ChantHitBar") );
	fw->SetWndProc( this );	
	fw->SetTitle( false );	
	fw->SetMove( false );
	fw->SetVisible( false );
	fw->SetZorder( n_zoTop );
	fw->Initialize();
		
	pb = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_ChantHitBar_TimeBar, WID_ChantHitBar_Wnd, 13, 3, 304, 18, false );
	pb->SetClientImage( TID_None );
	pb->SetBarImage( TID_ChantHitBar_TimeBar );
	pb->SetBarBlank( 0, 0 );
	pb->InitProgress( 50 );
	g_Pc.GetPlayer()->theActionSystem.theChantTimeBar = pb;
	
//		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ChantHitBar_Complete, WID_ChantHitBar_Wnd, 0, 0, 300, 9, false );
//		img->SetClientImage( TID_ChantHitBar_Complete );
//		g_Pc.GetPlayer()->theActionSystem.theChantCompleteImg = img;

	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ChantHitBar_Point1, WID_ChantHitBar_Wnd, 0, 0, 9, 18, false );
//	btn->SetBtnImage( TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Normal, TID_ChantHitBar_Point_Down, TID_None );
	btn->SetBtnImage( TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Down, TID_None );
	btn->SetHID( HID_None );
	btn->SetBtnPosWnd( 120, 16 );	
	btn->SetBtnHold( true );
	btn->SetImgBtn( true );
	g_Pc.GetPlayer()->theActionSystem.theChantPoint1 = btn;
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ChantHitBar_Point2, WID_ChantHitBar_Wnd, 0, 0, 9, 18, false );
	btn->SetBtnImage( TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Down, TID_None );
	btn->SetHID( HID_None );
	btn->SetBtnPosWnd( 120, 16 );	
	btn->SetBtnHold( true );
	btn->SetImgBtn( true );
	g_Pc.GetPlayer()->theActionSystem.theChantPoint2 = btn;
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ChantHitBar_Point3, WID_ChantHitBar_Wnd, 0, 0, 9, 18, false );
	btn->SetBtnImage( TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Down, TID_None );
	btn->SetHID( HID_None );
	btn->SetBtnPosWnd( 120, 16 );	
	btn->SetBtnHold( true );
	btn->SetImgBtn( true );
	g_Pc.GetPlayer()->theActionSystem.theChantPoint3 = btn;
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ChantHitBar_Point4, WID_ChantHitBar_Wnd, 0, 0, 9, 18, false );
	btn->SetBtnImage( TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Down, TID_None );
	btn->SetHID( HID_None );
	btn->SetBtnPosWnd( 120, 16 );	
	btn->SetBtnHold( true );
	btn->SetImgBtn( true );
	g_Pc.GetPlayer()->theActionSystem.theChantPoint4 = btn;
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ChantHitBar_Point5, WID_ChantHitBar_Wnd, 0, 0, 9, 18, false );
	btn->SetBtnImage( TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Over, TID_ChantHitBar_Point_Down, TID_None );
	btn->SetHID( HID_None );
	btn->SetBtnPosWnd( 120, 16 );	
	btn->SetBtnHold( true );
	btn->SetImgBtn( true );
	g_Pc.GetPlayer()->theActionSystem.theChantPoint5 = btn;

	
}

//-----------------------------------------------------------------------------
void CChantHitBarWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CChantHitBarWnd::Proc( SEventMessage* EMsg )
{
//	switch( EMsg->FocusWnd )
//	{	
	// 각각의 컨트롤에 따라서 처리 	
//	default:
//		break;
//	}

	return;
}

//-----------------------------------------------------------------------------
