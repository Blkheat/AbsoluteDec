// SkillRegistWnd.cpp: implementation of the CSkillRegistWnd class.
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



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
//#include "pc.h"

#include "SkillRegistWnd.h"
#include "SkillOverWriteWnd.h"



CSkillRegistWnd	SkillRegistWnd;


//-----------------------------------------------------------------------------
void CSkillRegistWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CSkillRegistWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	
	SRect skillwnd;
	skillwnd.w	= 65;
	skillwnd.h	= 40;
	skillwnd.x	= center_x - ( skillwnd.w / 2 );
	skillwnd.y	= center_y - ( skillwnd.h / 2 );
	
	FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SkillRegistWnd, WID_None, skillwnd.x, skillwnd.y, skillwnd.w, skillwnd.h, false );
	FrameWnd->SetClientImage( TID_SkillRegistWnd );
	FrameWnd->SetCaption( _T("SkillRegist Window") );
	FrameWnd->SetWndProc( this );
	FrameWnd->SetFrame( false );
	FrameWnd->SetTitle( false );
	FrameWnd->SetCloseBtn( false );
	FrameWnd->SetVisible( false );
	FrameWnd->Initialize();
	
	LControlBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillRegist_LControlBtn, WID_SkillRegistWnd, 2, 3, 61, 17, false );
	LControlBtn->SetBtnImage( TID_None, TID_SkillRegist_LControlBtn_Focus, TID_SkillRegist_LControlBtn_Select, TID_None );
	
	SpaceBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SkillRegist_SpaceBtn, WID_SkillRegistWnd, 2, 21, 61, 17, false );
	SpaceBtn->SetBtnImage( TID_None, TID_SkillRegist_SpaceBtn_Focus, TID_SkillRegist_SpaceBtn_Select, TID_None );
	
}


//-----------------------------------------------------------------------------
void CSkillRegistWnd::Update()
{	
}

//-----------------------------------------------------------------------------
void CSkillRegistWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_SkillRegist_LControlBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SkillRegistWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

			//쉬프트키에 ...스킬이 등록되어 있는지 확인
			//있으면..확인창 띄우고 없으면...기냥..등록
			if( SkillItem.Slot->PcItem )
			{
				if( SkillSystem->theShortCut_SkillCode[0] == 0 )
				{
					SkillSystem->theShortCut_SkillCode[0] = SkillItem.Slot->PcItem->Code;
					g_RockClient.Send_RpCsHotKey( 0, SkillItem.Slot->PcItem->Code );
				}
				else
				{
					nRui->SendEvent( WID_SkillOverWriteWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
					SkillOverWriteWnd.SetShortcutType( n_SHORTCUT_LCONTROL );
					SkillOverWriteWnd.SetCurSkillItemCode( SkillSystem->theShortCut_SkillCode[0] );
					SkillOverWriteWnd.SetNextSkillItem( &SkillItem );
					
				}
			}
		}
		break;
	case WID_SkillRegist_SpaceBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SkillRegistWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

			//스페이스키에 ...스킬이 등록되어 있는지 확인
			//있으면..확인창 띄우고 없으면...기냥..등록
			if( SkillItem.Slot->PcItem )
			{
				if( SkillSystem->theShortCut_SkillCode[1] == 0 )
				{
					SkillSystem->theShortCut_SkillCode[1] = SkillItem.Slot->PcItem->Code;
					g_RockClient.Send_RpCsHotKey( 1, SkillItem.Slot->PcItem->Code );
				}
				else
				{
					nRui->SendEvent( WID_SkillOverWriteWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
					SkillOverWriteWnd.SetShortcutType( n_SHORTCUT_SPACE );
					SkillOverWriteWnd.SetCurSkillItemCode( SkillSystem->theShortCut_SkillCode[1] );
					SkillOverWriteWnd.SetNextSkillItem( &SkillItem );
				}
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------
void CSkillRegistWnd::SetSkillItem( SSkillPack* _skillitem )
{
	memcpy( &SkillItem, _skillitem, sizeof( SSkillPack ) );
}

//-----------------------------------------------------------------------------
void CSkillRegistWnd::SetSkillSystem( CSkillSystem* _skillsystem )
{
	SkillSystem = _skillsystem;
}

//-----------------------------------------------------------------------------

*/
