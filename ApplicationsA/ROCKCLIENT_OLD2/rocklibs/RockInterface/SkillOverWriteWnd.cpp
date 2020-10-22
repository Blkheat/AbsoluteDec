// SkillOverWriteWnd.cpp: implementation of the CSkillOverWriteWnd class.
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
#include "SkillOverWriteWnd.h"


extern bool	gVisibleSkillWnd;


CSkillOverWriteWnd	SkillOverWriteWnd;


//-----------------------------------------------------------------------------
void CSkillOverWriteWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CSkillOverWriteWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CFrameWnd*		fw;
	CTextBox*		tb;	
	CButton*		btn;

	
	//-------------------------------------------------------------------------
	//
	//	스킬
	//	
	//-------------------------------------------------------------------------
	SRect skillwnd;
	skillwnd.w	= 370;
	skillwnd.h	= 180;
	skillwnd.x	= center_x - ( skillwnd.w / 2 );
	skillwnd.y	= center_y - ( skillwnd.h / 2 );
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SkillOverWriteWnd, WID_None, skillwnd.x, skillwnd.y, skillwnd.w, skillwnd.h, false );
	//fw->SetClientColor( 160, 160, 160 );
	fw->SetClientImage( TID_SkillOverWriteWnd );
	fw->SetCaption( _T("Skill OverWrite Window") );
	fw->SetWndProc( this );
	fw->SetFrame( false );
	fw->SetTitle( false );
	fw->SetCloseBtn( false );
	fw->SetVisible( false );
	fw->Initialize();

	
	CurSkillItem.AttrIcon = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_SkillOverWrite_CurtSkillImg, WID_SkillOverWriteWnd, 119, 13, 35, 35, false );
	CurSkillItem.AttrIcon->SetClientImage( TID_None );			// 해당하는 스킬의 아이콘 표시 
	CurSkillItem.AttrIcon->SetClientColor( 255, 255, 255 );
	
	CurSkillItem.LevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillOverWrite_CurtLevelText, WID_SkillOverWriteWnd, 165, 13, 52, 15, false );
	CurSkillItem.LevelTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	CurSkillItem.LevelTxt->SetClientImage( TID_None );
	CurSkillItem.LevelTxt->SetClientColor( 50, 50, 50 );
	CurSkillItem.LevelTxt->SetAlignText( n_atCenter, n_atCenter );
	CurSkillItem.LevelTxt->SetText( _T("!스킬 레벨") );
	CurSkillItem.LevelTxt->SetTextColor( 255, 255, 255 );
	CurSkillItem.LevelTxt->SetHID( HID_None );
	
	CurSkillItem.NameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillOverWrite_CurtNameText, WID_SkillOverWriteWnd, 165, 35, 194, 15, false );
	CurSkillItem.NameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	CurSkillItem.NameTxt->SetClientImage( TID_None );
	CurSkillItem.NameTxt->SetClientColor( 50, 50, 50 );
	CurSkillItem.NameTxt->SetAlignText( n_atLeft, n_atCenter );
	CurSkillItem.NameTxt->SetText( _T("!스킬 이름") );
	CurSkillItem.NameTxt->SetTextColor( 255, 255, 255 );
	CurSkillItem.NameTxt->SetHID( HID_None );




	NextSkillItem.AttrIcon = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_SkillOverWrite_NewSkillImg, WID_SkillOverWriteWnd, 119, 64, 35, 35, false );
	NextSkillItem.AttrIcon->SetClientImage( TID_None );			// 해당하는 스킬의 아이콘 표시 
	NextSkillItem.AttrIcon->SetClientColor( 255, 255, 255 );
	
	NextSkillItem.LevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillOverWrite_NewLevelText, WID_SkillOverWriteWnd, 165, 63, 52, 15, false );
	NextSkillItem.LevelTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	NextSkillItem.LevelTxt->SetClientImage( TID_None );
	NextSkillItem.LevelTxt->SetClientColor( 50, 50, 50 );
	NextSkillItem.LevelTxt->SetAlignText( n_atCenter, n_atCenter );
	NextSkillItem.LevelTxt->SetText( _T("!스킬 레벨") );
	NextSkillItem.LevelTxt->SetTextColor( 255, 255, 255 );
	NextSkillItem.LevelTxt->SetHID( HID_None );
	
	NextSkillItem.NameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillOverWrite_NewNameText, WID_SkillOverWriteWnd, 165, 85, 194, 15, false );
	NextSkillItem.NameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	NextSkillItem.NameTxt->SetClientImage( TID_None );
	NextSkillItem.NameTxt->SetClientColor( 50, 50, 50 );
	NextSkillItem.NameTxt->SetAlignText( n_atLeft, n_atCenter );
	NextSkillItem.NameTxt->SetText( _T("!스킬 이름") );
	NextSkillItem.NameTxt->SetTextColor( 255, 255, 255 );
	NextSkillItem.NameTxt->SetHID( HID_None );

	tb = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SkillOverWrite_InfoText1, WID_SkillOverWriteWnd, 90, 114, 200, 32, false );
	tb->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tb->SetClientImage( TID_None );
	tb->SetClientColor( 255, 255, 255 );
	tb->SetAlignText( n_atLeft, n_atCenter );
	tb->SetText( _T("!이미 등록된 스킬이 있습니다.\n스킬 등록을 바꾸시겠습니까?") );
	tb->SetTextColor( 255, 255, 255 );
	tb->SetMultiLine( true );
	tb->SetHID( HID_None );
	

	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillOverWrite_OKBtn, WID_SkillOverWriteWnd, 80, 151, 61, 20, false );
	btn->SetBtnImage( TID_None, TID_SkillOverWrite_OKBtn_Focus, TID_SkillOverWrite_OKBtn_Select, TID_None );
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillOverWrite_CancelBtn, WID_SkillOverWriteWnd, 230, 151, 61, 20, false );
	btn->SetBtnImage( TID_None, TID_SkillOverWrite_CancelBtn_Focus, TID_SkillOverWrite_CancelBtn_Select, TID_None );
	
}


//-----------------------------------------------------------------------------
void CSkillOverWriteWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CSkillOverWriteWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_SkillOverWrite_OKBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SkillOverWriteWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

			if( NextSkillItem.Slot->PcItem )
			{
				if( ShortcutType == n_SHORTCUT_LCONTROL )
				{
					SkillSystem->theShortCut_SkillCode[0] = NextSkillItem.Slot->PcItem->Code;
					g_RockClient.Send_RpCsHotKey( 0, NextSkillItem.Slot->PcItem->Code );
				}
				else //n_SHORTCUT_SPACE
				{	
					SkillSystem->theShortCut_SkillCode[1] = NextSkillItem.Slot->PcItem->Code;
					g_RockClient.Send_RpCsHotKey( 1, NextSkillItem.Slot->PcItem->Code );
				}
			}
		}
		break;
	case WID_SkillOverWrite_CancelBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SkillOverWriteWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
		break;
	}
}

//-----------------------------------------------------------------------------
void CSkillOverWriteWnd::SetSkillSystem( CSkillSystem* _skillsystem )
{
	SkillSystem = _skillsystem;
}

//-----------------------------------------------------------------------------
void CSkillOverWriteWnd::SetCurSkillItemCode( int _skillcode )
{
	SSkillBaseInfo* SkillTable = g_Pc_Manager.GetSkillBaseInfoPtr( _skillcode );

	char str[40] = "";

	sprintf( str, "%s", SkillTable->theSkillName );
	CurSkillItem.NameTxt->SetText( str );
	sprintf( str, "Level %d", SkillTable->theSkillLevel );
	CurSkillItem.LevelTxt->SetText( str );

	CurSkillItem.AttrIcon->SetClientImage( (TEXID)g_Pc_Manager.GetSkillIconTID( _skillcode ) );
}

//-----------------------------------------------------------------------------
void CSkillOverWriteWnd::SetNextSkillItem( SSkillPack* _skillitem )
{
	if( _skillitem->Slot->PcItem == NULL )
	{
		return;
	}

	char str[40] = "";

	//이게 왜 에러쥐?....흠흠
	//memcpy( &NextSkillItem, _skillitem, sizeof( SSkillPack ) );

	NextSkillItem.Slot = _skillitem->Slot;
	
	sprintf( str, "%s", _skillitem->Slot->PcItem->SkillTable->theSkillName );
	NextSkillItem.NameTxt->SetText( str );
	sprintf( str, "Level %d", _skillitem->Slot->PcItem->SkillTable->theSkillLevel  );
	NextSkillItem.LevelTxt->SetText( str );

	NextSkillItem.AttrIcon->SetClientImage( _skillitem->Slot->PcItem->Tid );
}

//-----------------------------------------------------------------------------
void CSkillOverWriteWnd::SetShortcutType( N_SHORTCUT_TYPE _shortcut_type )
{
	ShortcutType = _shortcut_type;
}

//-----------------------------------------------------------------------------
*/