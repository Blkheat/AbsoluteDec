// SkillInfoWnd.cpp: implementation of the CSkillInfoWnd class.
//
//////////////////////////////////////////////////////////////////////
/*
#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "StateButton.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
//#include "pc.h"
#include "SkillInfoWnd.h"

extern bool	gVisibleSkillWnd;


CSkillInfoWnd	SkillInfoWnd;


//-----------------------------------------------------------------------------
void CSkillInfoWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CSkillInfoWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CFrameWnd*		fw;
	CTextBox*		tb;	
//	CProgressBar*	pb;
	CButton*		btn;
//	CStateButton*	sbtn;
//	CEditBox*		eb;
//	CSpin*			spin;
	CImageBox*		img;
//	CScrollBar*		sb;
	

	SRect skillwnd;
	skillwnd.w	= 370;
	skillwnd.h	= 170;
	skillwnd.x	= center_x - ( skillwnd.w / 2 );
	skillwnd.y	= center_y - ( skillwnd.h / 2 );

	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SkillInfoWnd, WID_None, skillwnd.x, skillwnd.y, skillwnd.w, skillwnd.h, false );
	//fw->SetClientColor( 160, 160, 160 );
	fw->SetClientImage( TID_SkillInfoWnd );
	fw->SetCaption( _T("Skill Info Window") );
	fw->SetWndProc( this );
	fw->SetFrame( false );
	fw->SetTitle( false );
	fw->SetCloseBtn( false );
	fw->SetVisible( false );
	fw->Initialize();
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_SkillInfo_SkillImg, WID_SkillInfoWnd, 11, 13, 35, 35, false );
	img->SetClientImage( TID_CLIENT );			// 해당하는 스킬의 아이콘 표시 
	img->SetClientColor( 255, 0, 0 );
	
	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillInfo_NameText, WID_SkillInfoWnd, 57, 34, 194, 15, false );
	tb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tb->SetClientImage( TID_None );
	tb->SetClientColor( 50, 50, 50 );
	tb->SetAlignText( n_atLeft, n_atCenter );
	tb->SetText( _T("!스킬 이름") );
	tb->SetTextColor( 255, 255, 255 );
	tb->SetHID( HID_None );
	
	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillInfo_LevelText, WID_SkillInfoWnd, 57, 12, 52, 15, false );
	tb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tb->SetClientImage( TID_None );
	tb->SetClientColor( 50, 50, 50 );
	tb->SetAlignText( n_atLeft, n_atCenter );
	tb->SetText( _T("!스킬 레벨") );
	tb->SetTextColor( 255, 255, 255 );
	tb->SetHID( HID_None );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_SkillInfo_SkillAttrImg, WID_SkillInfoWnd, 261, 15, 35, 35, false );
	img->SetClientImage( TID_None );			// 해당하는 스킬의 아이콘 표시 
	img->SetClientColor( 255, 255, 255 );

	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillInfo_SkillTreeBtn, WID_SkillInfoWnd, 303, 8, 61, 20, false );
	btn->SetBtnImage( TID_None, TID_SkillInfo_TreeBtn_Focus, TID_SkillInfo_TreeBtn_Select, TID_None );
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillInfo_OKBtn, WID_SkillInfoWnd, 303, 142, 61, 20, false );
	btn->SetBtnImage( TID_None, TID_SkillInfo_OKBtn_Focus, TID_SkillInfo_OKBtn_Select, TID_None );
	
	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillInfo_InfoText1, WID_SkillInfoWnd, 20, 66, 264, 95, false );
	tb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tb->SetClientImage( TID_None );
	tb->SetClientColor( 50, 50, 50 );
	tb->SetAlignText( n_atLeft, n_atCenter );
	tb->SetText( _T("!스킬 정보 1"));
	tb->SetTextColor( 255, 255, 255 );
	tb->SetHID( HID_None );

	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillInfo_InfoText2, WID_SkillInfoWnd, 20, 77, 264, 95, false );
	tb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tb->SetClientImage( TID_None );
	tb->SetClientColor( 50, 50, 50 );
	tb->SetAlignText( n_atLeft, n_atCenter );
	tb->SetText( _T("!스킬 정보 2") );
	tb->SetTextColor( 255, 255, 255 );
	tb->SetHID( HID_None );

	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillInfo_InfoText3, WID_SkillInfoWnd, 20, 88, 264, 95, false );
	tb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tb->SetClientImage( TID_None );
	tb->SetClientColor( 50, 50, 50 );
	tb->SetAlignText( n_atLeft, n_atCenter );
	tb->SetText( _T("!스킬 정보 3") );
	tb->SetTextColor( 255, 255, 255 );
	tb->SetHID( HID_None );
	
}


//-----------------------------------------------------------------------------
void CSkillInfoWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CSkillInfoWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_SkillInfo_OKBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SkillInfoWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
		break;
	}
}

//-----------------------------------------------------------------------------
*/