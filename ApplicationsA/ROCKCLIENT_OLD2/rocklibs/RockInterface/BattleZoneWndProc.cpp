#include "..\\RockPCH.h"

#include <string.h>
#include "Rui.h"
#include "..\\..\\brother.h"
#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"
#include "..\\..\\MapDataManager.h"

#include "..\\..\\bravolibs\\obj\\SkillSystem.h"

#include "BattleZoneWndProc.h"
#include "NumberInputWnd.h"
#include "MCommunityWnd.h"
#include "FrontierMainWndProc.h"

#include "ChatMainProc.h"
#include "stringmanager.h"

#include "UiUtil.h"

#include "CDelayBarWnd.h"
 

CBZ_SelectZoneWndProc		g_BZ_SelectZoneWnd;
CBZ_SelectGradeWndProc		g_BZ_SelectGradeWnd;
CBZ_RoomListWndProc			g_BZ_RoomListWnd;		
CBZ_CreateRoomWndProc		g_BZ_CreateRoomWnd;		
CBZ_WaitingRoomWndProc		g_BZ_WaitingRoomWnd;	
CBZ_GameResultWndProc		g_BZ_GameResultWnd;

extern long					g_nowTime;


static bool	g_IsAscend_Sort = true; // or Descend_Sort	



//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
// Sort Function 
//=============================================================================================================
//=============================================================================================================
int SortRoomNumList( const void *arg1, const void *arg2 );
int SortRoomNameList( const void *arg1, const void *arg2 );
int SortGameTypeList( const void *arg1, const void *arg2 );
int SortUserNumList( const void *arg1, const void *arg2 );
int SortRoomStateList( const void *arg1, const void *arg2 );

int SortRoomNumList( const void *arg1, const void *arg2 )
{
	SRpScFightRoomList::_SFightRoom* a1 = ( SRpScFightRoomList::_SFightRoom* ) arg1;
	SRpScFightRoomList::_SFightRoom* a2 = ( SRpScFightRoomList::_SFightRoom* ) arg2;


	// 0 same, > 0 arg1 win, < 0 arg2 win

	if( g_IsAscend_Sort )
	{
		if( a1->theRoomID == a2->theRoomID )
		{
			return 0;
		}
		else if( a1->theRoomID < a2->theRoomID )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( a1->theRoomID == a2->theRoomID )
		{
			return 0;
		}
		else if( a1->theRoomID > a2->theRoomID )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	return 0;
}

int SortRoomNameList( const void *arg1, const void *arg2 )
{
	SRpScFightRoomList::_SFightRoom* a1 = ( SRpScFightRoomList::_SFightRoom* ) arg1;
	SRpScFightRoomList::_SFightRoom* a2 = ( SRpScFightRoomList::_SFightRoom* ) arg2;


	int comp_result = 0;

	if( g_IsAscend_Sort )
	{
		comp_result = Rstrcmp( a1->theName, a2->theName );

		if( comp_result < 0 )
		{
			return -1;
		}
		else if( comp_result > 0 )
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		comp_result = Rstrcmp( a1->theName, a2->theName );

		if( comp_result < 0 )
		{
			return 1;			
		}
		else if( comp_result > 0 )
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

int SortGameTypeList( const void *arg1, const void *arg2 )
{
	SRpScFightRoomList::_SFightRoom* a1 = ( SRpScFightRoomList::_SFightRoom* ) arg1;
	SRpScFightRoomList::_SFightRoom* a2 = ( SRpScFightRoomList::_SFightRoom* ) arg2;

	
	if( g_IsAscend_Sort )
	{
		if( a1->theGameType == a2->theGameType )
		{
			return 0;
		}
		else if( a1->theGameType < a2->theGameType )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( a1->theGameType == a2->theGameType )
		{
			return 0;
		}
		else if( a1->theGameType > a2->theGameType )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

int SortUserNumList( const void *arg1, const void *arg2 )
{
	SRpScFightRoomList::_SFightRoom* a1 = ( SRpScFightRoomList::_SFightRoom* ) arg1;
	SRpScFightRoomList::_SFightRoom* a2 = ( SRpScFightRoomList::_SFightRoom* ) arg2;

	
	if( g_IsAscend_Sort )
	{
		if( a1->theNum == a2->theNum )
		{
			return 0;
		}
		else if( a1->theNum < a2->theNum )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( a1->theNum == a2->theNum )
		{
			return 0;
		}
		else if( a1->theNum > a2->theNum )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	return 0;
}

int SortRoomStateList( const void *arg1, const void *arg2 )
{
	SRpScFightRoomList::_SFightRoom* a1 = ( SRpScFightRoomList::_SFightRoom* ) arg1;
	SRpScFightRoomList::_SFightRoom* a2 = ( SRpScFightRoomList::_SFightRoom* ) arg2;


	if( g_IsAscend_Sort )
	{
		if( a1->theStatus == a2->theStatus )
		{
			return 0;
		}
		else if( a1->theStatus < a2->theStatus )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( a1->theStatus == a2->theStatus )
		{
			return 0;
		}
		else if( a1->theStatus > a2->theStatus )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	return 0;
}
//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//=============================================================================================================



///---------------------------------------------------------------------------
///-- Constructor
///---------------------------------------------------------------------------
CBZ_SelectZoneWndProc::CBZ_SelectZoneWndProc()
{
	Init();
}
///---------------------------------------------------------------------------
///-- Destructror
///---------------------------------------------------------------------------
CBZ_SelectZoneWndProc::~CBZ_SelectZoneWndProc()
{
}
///---------------------------------------------------------------------------
///-- Init
///---------------------------------------------------------------------------
void CBZ_SelectZoneWndProc::Init()
{
	m_BZ_WndType = n_BattleZone_SelectZoneWnd;
	m_BZ_ZoneType = n_BattleZone_Nation;

	
	return;
}

//#define FREE_PVP

///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CBZ_SelectZoneWndProc::Composition()
{

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	CImageBox*		img;	
	SRect wnd_rect;



#ifndef NEW_PVP_DECO
	wnd_rect.w	= 1024;
	wnd_rect.h	= 276;
	wnd_rect.x	= center_x - ( wnd_rect.w / 2 );
	wnd_rect.y	= center_y - ( wnd_rect.h / 2 );

		///-- Frame Window 
	m_pFrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_BZ_SELECTZONE_WND, WID_None, 0, 248, wnd_rect.w, wnd_rect.h, false );	
	m_pFrameWnd->SetClientImage( TID_None );	
	//m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_SELECTZONE ) );
	//m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	///-- back image
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTZONE_WND,0, 0, 1024, 272, false );
	img->SetClientImage( TID_PVP_GATE_BACK );
	img->SetGlow( false );
	img->SetFocusCheck( false );
	
	///-- Button Focus Img 
	m_pSelectZoneBtn_FocusImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_BZ_SELECTZONE_BTNFOCUS_IMG, WID_BZ_SELECTZONE_WND,  0, 8, 512, 256, false );
	m_pSelectZoneBtn_FocusImg->SetClientImage( TID_None );
	m_pSelectZoneBtn_FocusImg->SetFocusCheck( false );

	///-- title image
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTZONE_WND, 417, 11, 189, 32, false );     
	img->SetClientImage( TID_PVP_TITLE );
	img->SetFocusCheck( false );
	
	///-- Title
	m_pNationBattle_DescTxt1 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND, 417, 20, 189, 16, false );
	m_pNationBattle_DescTxt1->SetFontR( n_fRsvFontTitle );
	m_pNationBattle_DescTxt1->SetAlignText( n_atCenter, n_atCenter );
	m_pNationBattle_DescTxt1->SetText( G_STRING_CHAR( IDS_WND_SELECTZONE ) );
	
	///-- left image
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTZONE_WND, 295, 32, 204, 193, false );     
	img->SetClientImage( TID_PVP_BTN_L );
	img->SetFocusCheck( false );

	///-- right image
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTZONE_WND, 524, 32, 204, 193, false );   
	img->SetClientImage( TID_PVP_BTN_R );
	img->SetFocusCheck( false );
	
	///-- Nation Battle Button 
	m_pSelectZone_NationBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_BZ_SELECTZONE_NATION_BTN, WID_BZ_SELECTZONE_WND,   0, 8, 512, 256, false );
	m_pSelectZone_NationBtn->SetBtnImage( TID_None, TID_None, TID_None, TID_None );	

	///-- Free Battle Button 
	m_pSelectZone_FreeBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_BZ_SELECTZONE_FREE_BTN, WID_BZ_SELECTZONE_WND, 512, 8, 512, 256, false );
	m_pSelectZone_FreeBtn->SetBtnImage( TID_None, TID_None, TID_None, TID_None );	

	///-- Nation Batttle Zone Name
	m_pNationBattleTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND, 329, 50, 100, 20, false );
	m_pNationBattleTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNationBattleTxt->SetClientImage( TID_None );	
	m_pNationBattleTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pNationBattleTxt->SetTextColor( 0, 0, 0 );
	m_pNationBattleTxt->SetText( G_STRING_CHAR( IDS_WND_NATION_BATTLE ) );
	m_pNationBattleTxt->SetFocusCheck( false );

	///-- Nation Batttle Zone Desc1
	m_pNationBattle_DescTxt1 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND, 345, 100, 126, 110, false );    
	m_pNationBattle_DescTxt1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNationBattle_DescTxt1->SetClientImage( TID_None );	
	m_pNationBattle_DescTxt1->SetAlignText( n_atCenter, n_atCenter );
	m_pNationBattle_DescTxt1->SetTextColor( 255, 255, 255 );
	m_pNationBattle_DescTxt1->SetText( G_STRING_CHAR( IDS_WND_NATION_BATTLE_DESC1 ) );
	m_pNationBattle_DescTxt1->SetMultiLine( true );
	m_pNationBattle_DescTxt1->SetFocusCheck( false );

	///-- Nation Batttle Zone Desc2
	m_pNationBattle_DescTxt2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND, 345, 140, 140, 110, false );    
	m_pNationBattle_DescTxt2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNationBattle_DescTxt2->SetClientImage( TID_None );	
	m_pNationBattle_DescTxt2->SetAlignText( n_atCenter, n_atCenter );
	m_pNationBattle_DescTxt2->SetTextColor( 255, 255, 255 );
	m_pNationBattle_DescTxt2->SetText( G_STRING_CHAR( IDS_WND_NATION_BATTLE_DESC2 ) );
	m_pNationBattle_DescTxt2->SetMultiLine( true );
	m_pNationBattle_DescTxt2->SetFocusCheck( false );

	///-- Free Batttle Zone Name
	m_pFreeBattleTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND, 590, 50, 100, 20, false );
	m_pFreeBattleTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pFreeBattleTxt->SetClientImage( TID_None );	
	m_pFreeBattleTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pFreeBattleTxt->SetTextColor( 0, 0, 0 );
	m_pFreeBattleTxt->SetText( G_STRING_CHAR( IDS_WND_FREE_BATTLE ) );
	m_pFreeBattleTxt->SetFocusCheck( false );

	///-- Free Batttle Zone Desc1
	m_pFreeBattle_DescTxt1 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND, 550, 100, 126, 110, false );    
	m_pFreeBattle_DescTxt1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pFreeBattle_DescTxt1->SetClientImage( TID_None );	
	m_pFreeBattle_DescTxt1->SetAlignText( n_atCenter, n_atCenter );
	m_pFreeBattle_DescTxt1->SetTextColor( 255, 255, 255 );
	m_pFreeBattle_DescTxt1->SetText( G_STRING_CHAR( IDS_WND_FREE_BATTLE_DESC1 ) );
	m_pFreeBattle_DescTxt1->SetMultiLine( true );
	m_pFreeBattle_DescTxt1->SetFocusCheck( false );

	///-- Free Batttle Zone Desc2
	m_pFreeBattle_DescTxt2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND, 550, 140, 140, 110, false ); 
	m_pFreeBattle_DescTxt2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pFreeBattle_DescTxt2->SetClientImage( TID_None );	
	m_pFreeBattle_DescTxt2->SetAlignText( n_atCenter, n_atCenter );
	m_pFreeBattle_DescTxt2->SetTextColor( 255, 255, 255 );
	m_pFreeBattle_DescTxt2->SetText( G_STRING_CHAR( IDS_WND_FREE_BATTLE_DESC2 ) );
	m_pFreeBattle_DescTxt2->SetMultiLine( true );
	m_pFreeBattle_DescTxt2->SetFocusCheck( false );

	///-- Close Button 
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_BZ_SELECTZONE_CLOSE_BTN, WID_BZ_SELECTZONE_WND, 468, 237, 87, 25, false ); 
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN87, TID_CTRL_BTN87_OVR, TID_CTRL_BTN87_CLK, TID_None );
	m_pCloseBtn->SetFontR( n_fRsvFontWndButton );
	m_pCloseBtn->SetText( G_STRING_CHAR( IDS_WND_GOOUT ) );	

	m_pCongestion_Nation = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_None, WID_BZ_SELECTZONE_WND, 340, 185, 140, 9, false ); 
	m_pCongestion_Nation->SetClientImage( TID_None );
	m_pCongestion_Nation->SetBarImage( TID_MAIN_PC_GAUGE_MP );
	m_pCongestion_Nation->SetBarBlank( 0, 0 );	
	m_pCongestion_Nation->InitProgress( 100 );	
	m_pCongestion_Nation->SetFitImg( true );
	m_pCongestion_Nation->SetFocusCheck( false );

	m_pCongestion_Free = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_None, WID_BZ_SELECTZONE_WND, 545, 185, 140, 9, false ); 
	m_pCongestion_Free->SetClientImage( TID_None );
	m_pCongestion_Free->SetBarImage( TID_MAIN_PC_GAUGE_MP );
	m_pCongestion_Free->SetBarBlank( 0, 0 );	
	m_pCongestion_Free->InitProgress( 100 );	
	m_pCongestion_Free->SetFitImg( true );
	m_pCongestion_Free->SetFocusCheck( false );
#else 

	wnd_rect.w	= 189;
	wnd_rect.h	= 210;
	wnd_rect.x	= center_x - ( wnd_rect.w / 2 );
	wnd_rect.y	= center_y - ( wnd_rect.h / 2 );

	
		///-- Frame Window 
	m_pFrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_BZ_SELECTZONE_WND, WID_None, wnd_rect.x, wnd_rect.y, wnd_rect.w, wnd_rect.h, false );	
	m_pFrameWnd->SetClientImage( TID_None );	
	//m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_SELECTZONE ) );
	//m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	///-- title image
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTZONE_WND, 18, 14 , 189, 32, false );     
	img->SetClientImage( TID_PVP_TITLE_white );
	img->SetFocusCheck( false );
	//img->SetClientColor(223,242,244);
	
	///-- right image
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTZONE_WND, 0, 32 , 226, 193, false );   
	img->SetClientImage( TID_PVP_BTN_M );
	img->SetFocusCheck( false );
	
	///-- Close Button 
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_BZ_SELECTZONE_CLOSE_BTN, WID_BZ_SELECTZONE_WND, 122, 181, 58, 25, false ); 
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pCloseBtn->SetFontR( n_fRsvFontWndButton );
	m_pCloseBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );	
	

	///-- 입장하기 .. 
	m_pSelectZone_FreeBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_BZ_SELECTZONE_FREE_BTN, WID_BZ_SELECTZONE_WND, 48 , 181 , 58, 25, false );
	m_pSelectZone_FreeBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pSelectZone_FreeBtn->SetFontR( n_fRsvFontWndButton );
	m_pSelectZone_FreeBtn->SetText( G_STRING_CHAR(IDS_WND_ENTRANCE));	
	
	// 39 0 149 29 	
	CTextBox * text = NULL;

	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND, 39, 17, 149, 29, false );
	text->SetFontg( n_ftGulimChe, 15, n_fwBold, n_fdNormal );
//	text->SetWndTexStyle( fwfTex_Style_Tile9 );
	text->SetClientImage( TID_None );	
	text->SetAlignText( n_atCenter, n_atCenter );
//	text->SetTextColor( 255, 60, 60 );
	text->SetTextColor( 0, 0, 0 );
	text->SetText( G_STRING_CHAR(IDS_WND_PVP_ROOM) );
	text->SetFocusCheck( false );


	///-- Free Batttle Zone Name
	m_pFreeBattleTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND, 39, 45, 149, 26, false );
	m_pFreeBattleTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pFreeBattleTxt->SetClientImage( TID_None );	
	m_pFreeBattleTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pFreeBattleTxt->SetTextColor( 0, 0, 0 );
	m_pFreeBattleTxt->SetText( G_STRING_CHAR( IDS_WND_FREE_BATTLE ) );
	m_pFreeBattleTxt->SetFocusCheck( false );

	SRect sTempRect;

	sTempRect.x	= 45;
	sTempRect.y	= 100;
	sTempRect.w	= 126;
	sTempRect.h	= 110;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.y	= 85;

#endif

	///-- Free Batttle Zone Desc1
	m_pFreeBattle_DescTxt1 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND,
														 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );    
	m_pFreeBattle_DescTxt1->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pFreeBattle_DescTxt1->SetClientImage( TID_None );	
	m_pFreeBattle_DescTxt1->SetAlignText( n_atCenter, n_atCenter );
	m_pFreeBattle_DescTxt1->SetTextColor( 0, 0, 0 );
	m_pFreeBattle_DescTxt1->SetText( G_STRING_CHAR( IDS_WND_FREE_BATTLE_DESC1 ) );
	m_pFreeBattle_DescTxt1->SetMultiLine( true );
	m_pFreeBattle_DescTxt1->SetFocusCheck( false );

	sTempRect.x	= 45;
	sTempRect.y	= 140;
	sTempRect.w	= 140;
	sTempRect.h	= 110;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.y	= 130;

#endif

	///-- Free Batttle Zone Desc2
	m_pFreeBattle_DescTxt2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTZONE_WND, 
														 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false ); 
	m_pFreeBattle_DescTxt2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pFreeBattle_DescTxt2->SetClientImage( TID_None );	
	m_pFreeBattle_DescTxt2->SetAlignText( n_atCenter, n_atCenter );
	m_pFreeBattle_DescTxt2->SetTextColor( 0, 0, 0 );
	m_pFreeBattle_DescTxt2->SetText( G_STRING_CHAR( IDS_WND_FREE_BATTLE_DESC2 ) );
	m_pFreeBattle_DescTxt2->SetMultiLine( true );
	m_pFreeBattle_DescTxt2->SetFocusCheck( false );

#endif








	return;
}

///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CBZ_SelectZoneWndProc::Update()
{
}


///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CBZ_SelectZoneWndProc::Proc( SEventMessage* EMsg )
{
#ifndef NEW_PVP_DECO
	
	switch( EMsg->FocusWnd )
	{
	case WID_BZ_SELECTZONE_CLOSE_BTN:
		{	
			if( EMsg->Notify == NM_BTN_CLICK )
			{				
				nRui->SendEvent( WID_BZ_SELECTZONE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			}
		}
		break;

	case WID_BZ_SELECTZONE_NATION_BTN:
		{	
			if( EMsg->Notify == NM_BTN_CLICK )
			{	
				nRui->SendEvent( WID_BZ_SELECTZONE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

				m_BZ_ZoneType = n_BattleZone_Nation;

				g_RockClient.Send_CsSelectFightGameType( n_NationGame );
			}

			if( EMsg->Notify == NM_BTN_FOCUS )
			{	
				m_pSelectZoneBtn_FocusImg->SetClientImage( TID_PVP_SHD );				
				m_pSelectZoneBtn_FocusImg->SetWndRepositionR( 0, 8 );
			}
			else
			{
				m_pSelectZoneBtn_FocusImg->SetClientImage( TID_None );
			}
		}		
		break;	
		
	case WID_BZ_SELECTZONE_FREE_BTN:
		{	
			if( EMsg->Notify == NM_BTN_CLICK )
			{			
				nRui->SendEvent( WID_BZ_SELECTZONE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

				m_BZ_ZoneType = n_BattleZone_Free;			
				g_RockClient.Send_CsSelectFightGameType( n_FreeBattleGame );	
			
			}

			if( EMsg->Notify == NM_BTN_FOCUS )
			{		
				m_pSelectZoneBtn_FocusImg->SetClientImage( TID_PVP_SHD );
				m_pSelectZoneBtn_FocusImg->SetWndRepositionR( 512, 8 );
			}
			else
			{
				m_pSelectZoneBtn_FocusImg->SetClientImage( TID_None );
			}
		}		
		break;	
	default:
		break;
	}
	
#else
	
	switch( EMsg->FocusWnd )
	{
	case WID_BZ_SELECTZONE_CLOSE_BTN:
		{	
			if( EMsg->Notify == NM_BTN_CLICK )
			{				
				
				if( g_DelayBarWnd.IsVisible && (g_DelayBarWnd.GetDelay_Type()  == DELAY_PVP_ROOM))
				{
					g_DelayBarWnd.Clear();
				}	
				nRui->SendEvent( WID_BZ_SELECTZONE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			}
		}
		break;
	case WID_BZ_SELECTZONE_FREE_BTN:
		{	
			if( EMsg->Notify == NM_BTN_CLICK )
			{			
				nRui->SendEvent( WID_BZ_SELECTZONE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

				m_BZ_ZoneType = n_BattleZone_Free;
			
				bool bFlag = true ; 

				if( g_Pc.GetPlayer()->m_curt_event.type == SM_MOVE_ATTACK || 
					g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
					g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY )
				{
					SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_PVP_NOT_ENTRANCE));	
					bFlag  = false ; 
				}
				
				if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
				{
					SystemPrint( n_DarkRed, n_SystemMsg ,G_STRING_CHAR(IDS_SYSPR_USE_SKILL_NOT_ENTRANCE) );	
					bFlag = false; 
				}	 
				
				if(bFlag)
				{
					nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
					bool bDie  = false; 
					Player*	user_pc = g_Pc.GetPlayer();

					///--JAPAN_BUG_MODIFY
					if(user_pc->IsRealDie())
					{
						bDie = true; 
					}
					
					g_DelayBarWnd.ResetDelay(DELAY_PVP_ROOM, 0, false);	
				}
			}
		}		
		break;	
	default:
		break;
	}

#endif

	return;
}


void CBZ_SelectZoneWndProc::Open( int aUserNum_Nation, int aUserNum_Free )
{
	m_pCongestion_Nation->SetPercent( aUserNum_Nation / 200 );
	m_pCongestion_Free->SetPercent( aUserNum_Free / 200 );

	nRui->SendEvent( WID_BZ_SELECTZONE_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );			

	return;
}


//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
///---------------------------------------------------------------------------
///-- Constructor
///---------------------------------------------------------------------------
CBZ_SelectGradeWndProc::CBZ_SelectGradeWndProc()
{
	Init();
}
///---------------------------------------------------------------------------
///-- Destructror
///---------------------------------------------------------------------------
CBZ_SelectGradeWndProc::~CBZ_SelectGradeWndProc()
{
}
///---------------------------------------------------------------------------
///-- Init
///---------------------------------------------------------------------------
void CBZ_SelectGradeWndProc::Init()
{
	m_BZ_GradeType = n_BattleZone_21_30_Grade;
	
	Rsprintf( RWCHAR(m_BZ_GradeRange[n_BattleZone_21_30_Grade]), _RT("21 ~ 30") );
	Rsprintf( RWCHAR(m_BZ_GradeRange[n_BattleZone_31_40_Grade]), _RT("31 ~ 40") );
	Rsprintf( RWCHAR(m_BZ_GradeRange[n_BattleZone_41_50_Grade]), _RT("41 ~ 50") );
	Rsprintf( RWCHAR(m_BZ_GradeRange[n_BattleZone_51_65_Grade]), _RT("50 ~ 65") );
	Rsprintf( RWCHAR(m_BZ_GradeRange[n_BattleZone_66_80_Grade]), _RT("66 ~ 80") );
	Rsprintf( RWCHAR(m_BZ_GradeRange[n_BattleZone_81_100_Grade]), _RT("81 ~ 100") );	

	return;
}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CBZ_SelectGradeWndProc::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect wnd_rect;
	wnd_rect.w	= 420;
	wnd_rect.h	= 256;
	wnd_rect.x	= center_x - ( wnd_rect.w / 2 );
	wnd_rect.y	= center_y - ( wnd_rect.h / 2 );

	char temp[100] = {0,};

	///-- Frame Window 
	m_pFrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_BZ_SELECTGRADE_WND, WID_None, wnd_rect.x, wnd_rect.y, wnd_rect.w, wnd_rect.h, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );	
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 ); 

	m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_SELECTGRADE ) );
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();
	
	///-- Batttle Zone Name
	m_pSelectZoneTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 100, 4, 60, 20, false );	
	m_pSelectZoneTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pSelectZoneTxt->SetClientImage( TID_None );	
	m_pSelectZoneTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pSelectZoneTxt->SetTextColor( 5, 5, 5 );
	Rsprintf( RWCHAR(temp), _RT("< %s>"), G_STRING_CHAR( IDS_WND_NATION_BATTLE ) );
	m_pSelectZoneTxt->SetText( temp );
	m_pSelectZoneTxt->SetFocusCheck( false );

	///-- Close Button 
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_BZ_SELECTGRADE_CLOSE_BTN, WID_BZ_SELECTGRADE_WND, 398, 6, 13, 13, false );
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	///-- 1th Class Img 
	m_pClass_1thImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTGRADE_WND, 130, 41, 171, 21, false );
	m_pClass_1thImg->SetClientImage( TID_BZ_SELECTGRADE_CLASS_IMG );

	///-- 1th Class Txt
	m_pClass_1thTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 130, 43, 171, 21, false );
	m_pClass_1thTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pClass_1thTxt->SetClientImage( TID_None );	
	m_pClass_1thTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pClass_1thTxt->SetTextColor( 5, 5, 5 );
	m_pClass_1thTxt->SetText( G_STRING_CHAR( IDS_WND_SELECTGRADE_CLASS_1TH ) );
	m_pClass_1thTxt->SetFocusCheck( false );

	///-- 21 ~ 30 Button 
	m_pLevel_21_30Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_SELECTGRADE_WND, 2, 64, 138, 40, false );
	m_pLevel_21_30Btn->SetBtnImage( TID_BZ_SELECTGRADE_BTN_NRL, TID_BZ_SELECTGRADE_BTN_OVR, TID_BZ_SELECTGRADE_BTN_CLK, TID_None );		

	///-- 31 ~ 40 Button 
	m_pLevel_31_40Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_SELECTGRADE_WND, 141, 64, 138, 40, false );
	m_pLevel_31_40Btn->SetBtnImage( TID_BZ_SELECTGRADE_BTN_NRL, TID_BZ_SELECTGRADE_BTN_OVR, TID_BZ_SELECTGRADE_BTN_CLK, TID_None );		

	///-- 41 ~ 50 Button 
	m_pLevel_41_50Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_SELECTGRADE_WND, 280, 64, 138, 40, false );
	m_pLevel_41_50Btn->SetBtnImage( TID_BZ_SELECTGRADE_BTN_NRL, TID_BZ_SELECTGRADE_BTN_OVR, TID_BZ_SELECTGRADE_BTN_CLK, TID_None );		

	///-- Basic Level Txt
	m_pBasicLevel_1thTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 6, 70, 128, 15, false );
	m_pBasicLevel_1thTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pBasicLevel_1thTxt->SetClientImage( TID_None );	
	m_pBasicLevel_1thTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pBasicLevel_1thTxt->SetTextColor( 5, 5, 5 );
	m_pBasicLevel_1thTxt->SetText( G_STRING_CHAR( IDS_WND_SELECTGRADE_BASICLEVEL ) );
	m_pBasicLevel_1thTxt->SetFocusCheck( false );

	///-- Normal Level Txt
	m_pNormalLevel_1thTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 146, 70, 128, 15, false );
	m_pNormalLevel_1thTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNormalLevel_1thTxt->SetClientImage( TID_None );	
	m_pNormalLevel_1thTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pNormalLevel_1thTxt->SetTextColor( 5, 5, 5 );
	m_pNormalLevel_1thTxt->SetText( G_STRING_CHAR( IDS_WND_SELECTGRADE_NORMALLEVEL ) );
	m_pNormalLevel_1thTxt->SetFocusCheck( false );

	///-- High Level Txt
	m_pHighLevel_1thTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 285, 70, 128, 15, false );
	m_pHighLevel_1thTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pHighLevel_1thTxt->SetClientImage( TID_None );	
	m_pHighLevel_1thTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pHighLevel_1thTxt->SetTextColor( 5, 5, 5 );
	m_pHighLevel_1thTxt->SetText( G_STRING_CHAR( IDS_WND_SELECTGRADE_HIGHLEVEL ) );
	m_pHighLevel_1thTxt->SetFocusCheck( false );

	///-- Level 21 ~ 30 Txt
	m_pLevel_21_30Txt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 6, 84, 128, 15, false );
	m_pLevel_21_30Txt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pLevel_21_30Txt->SetClientImage( TID_None );	
	m_pLevel_21_30Txt->SetAlignText( n_atCenter, n_atCenter );
	m_pLevel_21_30Txt->SetTextColor( 5, 5, 5 );
	Rsprintf( RWCHAR(temp), _RT( "( %s %s )"), G_STRING_CHAR( IDS_CHARSTATE_LEVEL ), RWCHAR(m_BZ_GradeRange[n_BattleZone_21_30_Grade]) );
	m_pLevel_21_30Txt->SetText( temp );
	m_pLevel_21_30Txt->SetFocusCheck( false );


	///-- Level 31 ~ 40 Txt
	m_pLevel_31_40Txt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 147, 84, 128, 15, false );
	m_pLevel_31_40Txt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pLevel_31_40Txt->SetClientImage( TID_None );	
	m_pLevel_31_40Txt->SetAlignText( n_atCenter, n_atCenter );
	m_pLevel_31_40Txt->SetTextColor( 5, 5, 5 );
	Rsprintf( RWCHAR(temp), _RT( "( %s %s )"), G_STRING_CHAR( IDS_CHARSTATE_LEVEL ), RWCHAR(m_BZ_GradeRange[n_BattleZone_31_40_Grade]) );
	m_pLevel_31_40Txt->SetText( temp );
	m_pLevel_31_40Txt->SetFocusCheck( false );

	///-- Level 41 ~ 50 Txt
	m_pLevel_41_50Txt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 285, 84, 128, 15, false );
	m_pLevel_41_50Txt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pLevel_41_50Txt->SetClientImage( TID_None );	
	m_pLevel_41_50Txt->SetAlignText( n_atCenter, n_atCenter );
	m_pLevel_41_50Txt->SetTextColor( 5, 5, 5 );
	Rsprintf( RWCHAR(temp), _RT( "( %s %s )"), G_STRING_CHAR( IDS_CHARSTATE_LEVEL ), RWCHAR(m_BZ_GradeRange[n_BattleZone_41_50_Grade]) );
	m_pLevel_41_50Txt->SetText( temp );
	m_pLevel_41_50Txt->SetFocusCheck( false );

	///-- Level 21 ~ 30 ProgressBar BK Img 
	m_pLevel_21_30BKImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTGRADE_WND, 2, 104, 138, 29, false );
	m_pLevel_21_30BKImg->SetClientImage( TID_BZ_SELECTGRADE_PERCENTBG_IMG );

	///-- Level 31 ~ 40 ProgressBar BK Img 
	m_pLevel_31_40BKImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTGRADE_WND, 141, 104, 138, 29, false );
	m_pLevel_31_40BKImg->SetClientImage( TID_BZ_SELECTGRADE_PERCENTBG_IMG );

	///-- Level 41 ~ 50 ProgressBar BK Img 
	m_pLevel_41_50BKImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTGRADE_WND, 280, 104, 138, 29, false );
	m_pLevel_41_50BKImg->SetClientImage( TID_BZ_SELECTGRADE_PERCENTBG_IMG );

	///-- Level 21 ~ 30 ProgressBar
	m_pLevel_21_30PGBar = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_BZ_SELECTGRADE_21_30_PGBAR, WID_BZ_SELECTGRADE_WND, 7, 107, 128, 21, false );
	m_pLevel_21_30PGBar->SetClientImage( TID_None );
	m_pLevel_21_30PGBar->SetBarImage( TID_BZ_SELECTGRADE_PERCENTBAR_IMG );
	m_pLevel_21_30PGBar->SetBarBlank( 0, 0 );	
	m_pLevel_21_30PGBar->InitProgress( 100 );	

	///-- Level 31 ~ 40 ProgressBar
	m_pLevel_31_40PGBar = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_BZ_SELECTGRADE_31_40_PGBAR, WID_BZ_SELECTGRADE_WND, 146, 107, 128, 21, false );
	m_pLevel_31_40PGBar->SetClientImage( TID_None );
	m_pLevel_31_40PGBar->SetBarImage( TID_BZ_SELECTGRADE_PERCENTBAR_IMG );
	m_pLevel_31_40PGBar->SetBarBlank( 0, 0 );	
	m_pLevel_31_40PGBar->InitProgress( 100 );	

	///-- Level 41 ~ 50 ProgressBar 
	m_pLevel_41_50PGBar = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_BZ_SELECTGRADE_41_50_PGBAR, WID_BZ_SELECTGRADE_WND, 285, 107, 128, 21, false );
	m_pLevel_41_50PGBar->SetClientImage( TID_None );
	m_pLevel_41_50PGBar->SetBarImage( TID_BZ_SELECTGRADE_PERCENTBAR_IMG );
	m_pLevel_41_50PGBar->SetBarBlank( 0, 0 );	
	m_pLevel_41_50PGBar->InitProgress( 100 );	

	///-- 2th Class Img 
	m_pClass_2thImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTGRADE_WND, 130, 143, 171, 21, false );
	m_pClass_2thImg->SetClientImage( TID_BZ_SELECTGRADE_CLASS_IMG );

	///-- 2th Class Txt
	m_pClass_2thTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 130, 145, 171, 21, false );
	m_pClass_2thTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pClass_2thTxt->SetClientImage( TID_None );	
	m_pClass_2thTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pClass_2thTxt->SetTextColor( 5, 5, 5 );
	m_pClass_2thTxt->SetText( G_STRING_CHAR( IDS_WND_SELECTGRADE_CLASS_2TH ) );
	m_pClass_2thTxt->SetFocusCheck( false );

	///-- 51 ~ 65 Button 
	m_pLevel_51_65Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_SELECTGRADE_WND, 2, 166, 138, 40, false );
	m_pLevel_51_65Btn->SetBtnImage( TID_BZ_SELECTGRADE_BTN_NRL, TID_BZ_SELECTGRADE_BTN_OVR, TID_BZ_SELECTGRADE_BTN_CLK, TID_None );		

	///-- 66 ~ 80 Button 
	m_pLevel_66_80Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_SELECTGRADE_WND, 141, 166, 138, 40, false );
	m_pLevel_66_80Btn->SetBtnImage( TID_BZ_SELECTGRADE_BTN_NRL, TID_BZ_SELECTGRADE_BTN_OVR, TID_BZ_SELECTGRADE_BTN_CLK, TID_None );		

	///-- 81 ~ 100 Button 
	m_pLevel_81_100Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_SELECTGRADE_WND, 280, 166, 138, 40, false );
	m_pLevel_81_100Btn->SetBtnImage( TID_BZ_SELECTGRADE_BTN_NRL, TID_BZ_SELECTGRADE_BTN_OVR, TID_BZ_SELECTGRADE_BTN_CLK, TID_None );		
	
	///-- Basic Level Txt
	m_pBasicLevel_2thTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 6, 172, 128, 15, false );
	m_pBasicLevel_2thTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pBasicLevel_2thTxt->SetClientImage( TID_None );	
	m_pBasicLevel_2thTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pBasicLevel_2thTxt->SetTextColor( 5, 5, 5 );
	m_pBasicLevel_2thTxt->SetText( G_STRING_CHAR( IDS_WND_SELECTGRADE_BASICLEVEL ) );
	m_pBasicLevel_2thTxt->SetFocusCheck( false );

	///-- Normal Level Txt
	m_pNormalLevel_2thTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 146, 172, 128, 15, false );
	m_pNormalLevel_2thTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNormalLevel_2thTxt->SetClientImage( TID_None );	
	m_pNormalLevel_2thTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pNormalLevel_2thTxt->SetTextColor( 5, 5, 5 );
	m_pNormalLevel_2thTxt->SetText( G_STRING_CHAR( IDS_WND_SELECTGRADE_NORMALLEVEL ) );
	m_pNormalLevel_2thTxt->SetFocusCheck( false );

	///-- High Level Txt
	m_pHighLevel_2thTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 285, 172, 128, 15, false );
	m_pHighLevel_2thTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pHighLevel_2thTxt->SetClientImage( TID_None );	
	m_pHighLevel_2thTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pHighLevel_2thTxt->SetTextColor( 5, 5, 5 );
	m_pHighLevel_2thTxt->SetText( G_STRING_CHAR( IDS_WND_SELECTGRADE_HIGHLEVEL ) );
	m_pHighLevel_2thTxt->SetFocusCheck( false );

	///-- Level 51 ~ 65 Txt
	m_pLevel_51_65Txt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 6, 186, 128, 15, false );
	m_pLevel_51_65Txt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pLevel_51_65Txt->SetClientImage( TID_None );	
	m_pLevel_51_65Txt->SetAlignText( n_atCenter, n_atCenter );
	m_pLevel_51_65Txt->SetTextColor( 5, 5, 5 );
	Rsprintf( RWCHAR(temp), _RT( "( %s %s )"), G_STRING_CHAR( IDS_CHARSTATE_LEVEL ), RWCHAR(m_BZ_GradeRange[n_BattleZone_51_65_Grade]) );
	m_pLevel_51_65Txt->SetText( temp );
	m_pLevel_51_65Txt->SetFocusCheck( false );

	///-- Level 66 ~ 80 Txt
	m_pLevel_66_80Txt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 147, 186, 128, 15, false );
	m_pLevel_66_80Txt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pLevel_66_80Txt->SetClientImage( TID_None );	
	m_pLevel_66_80Txt->SetAlignText( n_atCenter, n_atCenter );
	m_pLevel_66_80Txt->SetTextColor( 5, 5, 5 );
	Rsprintf( RWCHAR(temp), _RT( "( %s %s )"), G_STRING_CHAR( IDS_CHARSTATE_LEVEL ), RWCHAR(m_BZ_GradeRange[n_BattleZone_66_80_Grade]) );
	m_pLevel_66_80Txt->SetText( temp );
	m_pLevel_66_80Txt->SetFocusCheck( false );

	///-- Level 81 ~ 100 Txt
	m_pLevel_81_100Txt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_SELECTGRADE_WND, 285, 186, 128, 15, false );
	m_pLevel_81_100Txt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pLevel_81_100Txt->SetClientImage( TID_None );	
	m_pLevel_81_100Txt->SetAlignText( n_atCenter, n_atCenter );
	m_pLevel_81_100Txt->SetTextColor( 5, 5, 5 );
	Rsprintf( RWCHAR(temp), _RT( "( %s %s )"), G_STRING_CHAR( IDS_CHARSTATE_LEVEL ),RWCHAR(m_BZ_GradeRange[n_BattleZone_81_100_Grade]) );
	m_pLevel_81_100Txt->SetText( temp );
	m_pLevel_81_100Txt->SetFocusCheck( false );

	///-- Level 51 ~ 65 ProgressBar BK Img 
	m_pLevel_51_65BKImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTGRADE_WND, 2, 206, 138, 29, false );
	m_pLevel_51_65BKImg->SetClientImage( TID_BZ_SELECTGRADE_PERCENTBG_IMG );

	///-- Level 66 ~ 80 ProgressBar BK Img 
	m_pLevel_66_80BKImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTGRADE_WND, 141, 206, 138, 29, false );
	m_pLevel_66_80BKImg->SetClientImage( TID_BZ_SELECTGRADE_PERCENTBG_IMG );

	///-- Level 81 ~ 100 ProgressBar BK Img 
	m_pLevel_81_100BKImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_SELECTGRADE_WND, 280, 206, 138, 29, false );
	m_pLevel_81_100BKImg->SetClientImage( TID_BZ_SELECTGRADE_PERCENTBG_IMG );

	///-- Level 51 ~ 65 ProgressBar
	m_pLevel_51_65PGBar = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_BZ_SELECTGRADE_51_65_PGBAR, WID_BZ_SELECTGRADE_WND, 7, 209, 128, 21, false );
	m_pLevel_51_65PGBar->SetClientImage( TID_None );
	m_pLevel_51_65PGBar->SetBarImage( TID_BZ_SELECTGRADE_PERCENTBAR_IMG );
	m_pLevel_51_65PGBar->SetBarBlank( 0, 0 );	
	m_pLevel_51_65PGBar->InitProgress( 100 );	

	///-- Level 66 ~ 80 ProgressBar
	m_pLevel_66_80PGBar = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_BZ_SELECTGRADE_66_80_PGBAR, WID_BZ_SELECTGRADE_WND, 146, 209, 128, 21, false );
	m_pLevel_66_80PGBar->SetClientImage( TID_None );
	m_pLevel_66_80PGBar->SetBarImage( TID_BZ_SELECTGRADE_PERCENTBAR_IMG );
	m_pLevel_66_80PGBar->SetBarBlank( 0, 0 );	
	m_pLevel_66_80PGBar->InitProgress( 100 );	

	///-- Level 81 ~ 100 ProgressBar 
	m_pLevel_81_100PGBar = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_BZ_SELECTGRADE_81_100_PGBAR, WID_BZ_SELECTGRADE_WND, 285, 209, 128, 21, false );
	m_pLevel_81_100PGBar->SetClientImage( TID_None );
	m_pLevel_81_100PGBar->SetBarImage( TID_BZ_SELECTGRADE_PERCENTBAR_IMG );
	m_pLevel_81_100PGBar->SetBarBlank( 0, 0 );	
	m_pLevel_81_100PGBar->InitProgress( 100 );	

	return;
}

///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CBZ_SelectGradeWndProc::Update()
{
	
	return;
}


///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CBZ_SelectGradeWndProc::Proc( SEventMessage* EMsg )
{	
	char temp[100] = {0,};

	//====================================================================================================
	if( EMsg->FocusWnd == WID_BZ_SELECTGRADE_CLOSE_BTN )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{				
			nRui->SendEvent( WID_BZ_SELECTGRADE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			g_BZ_SelectZoneWnd.m_BZ_WndType = n_BattleZone_SelectZoneWnd;
		}
	}
	//====================================================================================================
	else if( EMsg->FocusWnd == m_pLevel_21_30Btn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{				
			nRui->SendEvent( WID_BZ_SELECTGRADE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			
			m_BZ_GradeType = n_BattleZone_21_30_Grade;			
			
//			g_RockClient.Send_CsSelectFightGrade( n_BattleZone_21_30_Grade );
		}
	}
	else if( EMsg->FocusWnd == m_pLevel_31_40Btn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{	
			nRui->SendEvent( WID_BZ_SELECTGRADE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

			m_BZ_GradeType = n_BattleZone_31_40_Grade;			
			
//			g_RockClient.Send_CsSelectFightGrade( n_BattleZone_31_40_Grade );
		}
	}
	else if( EMsg->FocusWnd == m_pLevel_41_50Btn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{	
			nRui->SendEvent( WID_BZ_SELECTGRADE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

			m_BZ_GradeType = n_BattleZone_41_50_Grade;						

//			g_RockClient.Send_CsSelectFightGrade( n_BattleZone_41_50_Grade );
		}
	}
	else if( EMsg->FocusWnd == m_pLevel_51_65Btn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_BZ_SELECTGRADE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			
			m_BZ_GradeType = n_BattleZone_51_65_Grade;			
			
//			g_RockClient.Send_CsSelectFightGrade( n_BattleZone_51_65_Grade );
		}
	}
	else if( EMsg->FocusWnd == m_pLevel_66_80Btn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{	
			nRui->SendEvent( WID_BZ_SELECTGRADE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			
			m_BZ_GradeType = n_BattleZone_66_80_Grade;			

//			g_RockClient.Send_CsSelectFightGrade( n_BattleZone_66_80_Grade );
		}
	}
	else if( EMsg->FocusWnd == m_pLevel_81_100Btn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_BZ_SELECTGRADE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

			m_BZ_GradeType = n_BattleZone_81_100_Grade;			

//			g_RockClient.Send_CsSelectFightGrade( n_BattleZone_81_100_Grade );
		}
	}

	return;
}

void CBZ_SelectGradeWndProc::SetSelectZoneTxt()
{
	char temp[100] = {0,};

	if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Nation )
	{
		Rsprintf(RWCHAR(temp), _RT("< %s>"), G_STRING_CHAR( IDS_WND_NATION_BATTLE ) );
		m_pSelectZoneTxt->SetText( temp );
	}
	else if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Free )
	{
		Rsprintf(RWCHAR(temp), _RT("< %s>"), G_STRING_CHAR( IDS_WND_FREE_BATTLE ) );
		m_pSelectZoneTxt->SetText( temp );
	}

	return;
}


void CBZ_SelectGradeWndProc::SetCongestionPGBar()
{
	// 혼잡도 최대 인원을 대략 200명 정도로 설정...
	// 최대 인원을 아직 없음 
	
	m_pLevel_21_30PGBar->SetPercent( m_BZ_Congestion[n_BattleZone_21_30_Grade] / 200 );
	m_pLevel_31_40PGBar->SetPercent( m_BZ_Congestion[n_BattleZone_31_40_Grade] / 200 );
	m_pLevel_41_50PGBar->SetPercent( m_BZ_Congestion[n_BattleZone_41_50_Grade] / 200 );
	m_pLevel_51_65PGBar->SetPercent( m_BZ_Congestion[n_BattleZone_51_65_Grade] / 200 );
	m_pLevel_66_80PGBar->SetPercent( m_BZ_Congestion[n_BattleZone_66_80_Grade] / 200 );
	m_pLevel_81_100PGBar->SetPercent( m_BZ_Congestion[n_BattleZone_81_100_Grade] / 200 );

	return;
}

//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
///---------------------------------------------------------------------------
///-- Constructor
///---------------------------------------------------------------------------
CBZ_RoomListWndProc::CBZ_RoomListWndProc()
{
	Init();
}
///---------------------------------------------------------------------------
///-- Destructror
///---------------------------------------------------------------------------
CBZ_RoomListWndProc::~CBZ_RoomListWndProc()
{
}
///---------------------------------------------------------------------------
///-- Init
///---------------------------------------------------------------------------
void CBZ_RoomListWndProc::Init()
{
	m_SelectRoomIndex = -1;

	m_StartLineNum_RoomList = 0;
	m_StartLineNum_UserList = 0;
	m_StartLineNum_ChatMsg = 0;

	m_ScrollFocus = n_BZ_Focus_RoomList;
	
	ChatMsgInit();

	m_bIsOnlyWaitingRoom = FALSE;

	m_lPrevTime = 0;

	//=============================================================================
	//=============================================================================
	// test code
	//=============================================================================
//	struct _SFightRoom
//	{
//		DWORD	theRoomID;
//		char	theName[40];
//		WORD	theLock:1;			// 잠긴 방 여부 
//		WORD	theGameType:6;		// 게임타입 
//		WORD	theNum:4;			// 참가한 인원
//		WORD	theMaxNum:4;		// 게임인원
//		WORD	theStatus:1;				// 상태(준비, 게임)
//	};

/*	int i = 0;

	for( i = 0; i < c_Max_BZRoomList_Num; ++i )
	{
		SRpScFightRoomList::_SFightRoom	fight_room;

		fight_room.theRoomID = i;
		sprintf( fight_room.theName, "!테스트용 게임룸 No.%d", i );
		fight_room.theLock = rand() % 2;
		fight_room.theGameType = rand() % 4;
		fight_room.theNum = rand() % 12;
		fight_room.theMaxNum = 12;
		fight_room.theStatus = rand() % 2;

		m_BZ_RoomInfoList.push_back( fight_room );
	}

	for( i = 0; i < c_Max_BZUserList_Num; ++i )
	{	
		SBZ_UserList user_list;

		sprintf( user_list.UserName, "!유저 No.%d", i );
	
		m_BZ_UserNameList.push_back( user_list );
	}
*/
	//=============================================================================
	//=============================================================================
	//=============================================================================

	return;
}

//by simwoosung
void CBZ_RoomListWndProc::ChatMsgInit()
{
	m_uChatType = n_BZ_Normal_Chat;
	
	SAFE_STR_CPY( m_strChatstring, _RT("") , c_Max_ChatMsg_Length * 2  );
	SAFE_STR_CPY( m_strTempBuf1, _RT("") , c_Max_ChatMsg_Length * 2  );
	SAFE_STR_CPY( m_strTempBuf2, _RT("") , c_Max_ChatMsg_Length * 2 );
	SAFE_STR_CPY( m_strWhisDest, _RT("") , 256 );	
	
	memset( m_BZ_ChatMsgList, 0, sizeof(SBZ_ChatMsgList) * (c_Max_BZChatMsg_Num + 1));
	
	m_nInsertIndex = 0;	
}

//by simwoosung
void  CBZ_RoomListWndProc::SetChatType(UINT uChatType)
{
	if( !(uChatType == n_BZ_Normal_Chat || uChatType == n_BZ_Whisper_Chat) )
	{
		return;
	}

	if( m_uChatType != uChatType )
	{
		return;
	}

	m_uChatType = uChatType;
}

//by simwoosung
void CBZ_RoomListWndProc::SetWhisDest(void * pstrDest)
{
	char* strDest = (char*)(pstrDest);
	SAFE_STR_CPY(m_strWhisDest, strDest, 256);
	Rsprintf( RWCHAR(m_strChatstring) , _RT("/w %s ") , RWCHAR(m_strWhisDest));				
	m_pChatInputBox->SetText(m_strChatstring);
	PostMessage( m_pChatInputBox->GetHEdit(), WM_USER + 44, NULL, NULL );
	nRui->SendEvent( m_pWhisperBtn->GetID(), n_emSetFocus, 0, 0, 0, 0 );
}

///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CBZ_RoomListWndProc::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	

	CImageBox* img = NULL		;

	
	SRect wnd_rect;
	wnd_rect.w	= 1024;
	wnd_rect.h	= 768;
	wnd_rect.x	= 0; // center_x - ( wnd_rect.w / 2 );
	wnd_rect.y	= 0; // center_y - ( wnd_rect.h / 2 );

	char temp[100] = {0,};

	///-- Frame Window 
//	m_pFrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_BZ_ROOMLIST_WND, WID_None, wnd_rect.x, wnd_rect.y, wnd_rect.w, wnd_rect.h, false );
//	m_pFrameWnd->SetClientImage( TID_BZ_SELECTGRADE_LWND );
//	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile3 );
//	m_pFrameWnd->SetWndTileSizeLR( 11, 3 );
//	m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_BATTLEZONE_LIST ) );
//	m_pFrameWnd->SetTitle( true, 2, 30 );
//	m_pFrameWnd->SetWndProc( this );	
//	m_pFrameWnd->SetFrame( false );
//	m_pFrameWnd->SetCloseBtn( false );
//	m_pFrameWnd->SetVisible( false );
//	m_pFrameWnd->Initialize();

	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_BZ_ROOMLIST_WND, WID_None, wnd_rect.x, wnd_rect.y, wnd_rect.w, wnd_rect.h, false );	
	m_pFrameWnd->SetClientImage( TID_PVP_WND_BG );
	m_pFrameWnd->SetCaption( _RT( "" ) );
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetTitle( false );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();
	m_pFrameWnd->SetMove( false );
	
	//Logo Change
	if( g_RockClient.GetLanguage() == eLang_Korea )
	{		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 122, 109, 126, 35, false );
		img->SetClientImage( TID_pvp_sikongchanga );
	}
	else if( g_RockClient.GetLanguage() == eLang_China )
	{		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 122, 109, 126, 35, false );
		img->SetClientImage( TID_china_pvp_logo );
		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 784, 109, 116, 35, false );
		img->SetClientImage( TID_china_pvp );
	}
	else if( g_RockClient.GetLanguage() == eLang_Japan )
	{
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 122, 109, 122, 35, false );
		img->SetClientImage( TID_japan_pvp_logo );	
	}

	///-- Go Out Button 
	m_pGoOutBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 794, 592, 100, 36, false ); 
	m_pGoOutBtn->SetBtnImage( TID_ROOMLIST_BTN_NORMAL, TID_ROOMLIST_BTN_FOCUS, TID_ROOMLIST_BTN_SELECT, TID_ROOMLIST_BTN_DISABLE );
	m_pGoOutBtn->SetFontg( n_ftGulimChe, 13, n_fwBold, n_fdNormal );	
	m_pGoOutBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pGoOutBtn->SetTextColor( 255, 255, 255 );
	m_pGoOutBtn->SetText( G_STRING_CHAR( IDS_WND_GOOUT ) );
		
	///-- Room List Images
	m_pRoomListImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 127, 164, 768, 256, false );
	m_pRoomListImg->SetClientImage( TID_ROOMLIST_IMG );

	m_pChatWndImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 128, 446, 490, 178, false );
	m_pChatWndImg->SetClientImage( TID_CHATWND_IMG );

	m_pUserListImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 625, 446, 269, 137, false );
	m_pUserListImg->SetClientImage( TID_USERLIST_IMG );


	///-- Grade Type Txt
	m_pGradeTypeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 171, 174, 270, 22, false );	
	m_pGradeTypeTxt->SetFontg( n_ftGulimChe, 13, n_fwNormal, n_fdNormal );
	m_pGradeTypeTxt->SetClientImage( TID_None );	
	m_pGradeTypeTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pGradeTypeTxt->SetTextColor( 255, 255, 255 );
	Rsprintf(RWCHAR(temp), _RT("%s ( %s 21 ~ 30 )"), G_STRING_CHAR( IDS_WND_SELECTGRADE_BASICLEVEL ),		// 초   급 ( 레벨 00 ~ 00 ) 
					 					  G_STRING_CHAR( IDS_CHARSTATE_LEVEL ) );
	m_pGradeTypeTxt->SetText( temp );
	m_pGradeTypeTxt->SetFocusCheck( false );

	///-- Room Number Txt
	m_pRoomNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 133, 212, 52, 25, false );	
	m_pRoomNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pRoomNumTxt->SetClientImage( TID_None );	
	m_pRoomNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pRoomNumTxt->SetTextColor( 255, 255, 255 );
	m_pRoomNumTxt->SetText( G_STRING_CHAR( IDS_WND_NUMBER ) );
	m_pRoomNumTxt->SetFocusCheck( false );

	///-- Room Name Txt
	m_pRoomNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 185, 212, 358, 25, false );	
	m_pRoomNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pRoomNameTxt->SetClientImage( TID_None );	
	m_pRoomNameTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pRoomNameTxt->SetTextColor( 255, 255, 255 );
	m_pRoomNameTxt->SetText( G_STRING_CHAR( IDS_WND_ROOMNAME ) );
	m_pRoomNameTxt->SetFocusCheck( false );

	///-- Can View Txt
	m_pCanViewTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 543, 212, 61, 25, false );	
	m_pCanViewTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pCanViewTxt->SetClientImage( TID_None );	
	m_pCanViewTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pCanViewTxt->SetTextColor( 255, 255, 255 );
	m_pCanViewTxt->SetText( G_STRING_CHAR( IDS_VIEW ) );		
	m_pCanViewTxt->SetFocusCheck( false );

	///-- Game Type Txt
	m_pGameTypeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 607, 212, 92, 25, false );	
	m_pGameTypeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pGameTypeTxt->SetClientImage( TID_None );	
	m_pGameTypeTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pGameTypeTxt->SetTextColor( 255, 255, 255 );
	m_pGameTypeTxt->SetText( G_STRING_CHAR( IDS_WND_GAMETYPE ) );
	m_pGameTypeTxt->SetFocusCheck( false );

	///-- Current User Number Txt
	m_pCurtUserNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 701, 212, 60, 25, false );	
	m_pCurtUserNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pCurtUserNumTxt->SetClientImage( TID_None );	
	m_pCurtUserNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pCurtUserNumTxt->SetTextColor( 255, 255, 255 );
	m_pCurtUserNumTxt->SetText( G_STRING_CHAR( IDS_WND_USERNUM ) );
	m_pCurtUserNumTxt->SetFocusCheck( false );

	///-- Room State Txt
	m_pRoomStateTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 762, 212, 126, 25, false );	
	m_pRoomStateTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pRoomStateTxt->SetClientImage( TID_None );	
	m_pRoomStateTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pRoomStateTxt->SetTextColor( 255, 255, 255 );
	m_pRoomStateTxt->SetText( G_STRING_CHAR( IDS_WND_STATE2 ) );
	m_pRoomStateTxt->SetFocusCheck( false );
	
//	width : 738			height : 132
//	RoomList Button Pos 
//	start : x = 135, y = 239, width = 123, height =  22		135, 239, 123, 22, false );	
//			x = 135, y = 261, width = 123, height =  22		135, 261, 123, 22, false );	 
//			x = 135, y = 283, width = 123, height =  22		135, 283, 123, 22, false );	 
//			x = 135, y = 305, width = 123, height =  22		135, 305, 123, 22, false );	 
//			x = 135, y = 327, width = 123, height =  22		135, 327, 123, 22, false );	 
//			x = 135, y = 349, width = 123, height =  22		135, 349, 123, 22, false );	 

//	m_pRoomNumTxt = 133, 239, 52, 25, false );	
//	m_pRoomNameTxt = 185 + 24, 239, 420, 25, false );	//	비번방 표시 24 
//	m_pGameTypeTxt = 607, 239, 92, 25, false );	
//	m_pCurtUserNumTxt = 701, 239, 60, 25, false );	
//	m_pRoomStateTxt = 762, 239, 126, 25, false );	

	///-- Draw Room List 		
	m_BZ_RoomList[0].RoomNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 133, 239, 52, 25, false );
	m_BZ_RoomList[0].RoomNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[0].RoomNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[0].RoomNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[0].RoomNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[0].RoomNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[0].RoomNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[0].PassWordImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 191, 245, 12, 12, false );
	m_BZ_RoomList[0].PassWordImg->SetClientImage( TID_None );

	m_BZ_RoomList[0].RoomNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 209, 239, 358, 25, false );
	m_BZ_RoomList[0].RoomNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[0].RoomNameTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[0].RoomNameTxt->SetAlignText( n_atLeft, n_atCenter );
	m_BZ_RoomList[0].RoomNameTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[0].RoomNameTxt->SetText(_RT( "") );
	m_BZ_RoomList[0].RoomNameTxt->SetFocusCheck( false );

	m_BZ_RoomList[0].CanViewTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 543, 239, 61, 25, false );
	m_BZ_RoomList[0].CanViewTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[0].CanViewTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[0].CanViewTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[0].CanViewTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[0].CanViewTxt->SetText(_RT( "") );
	m_BZ_RoomList[0].CanViewTxt->SetFocusCheck( false );	

	m_BZ_RoomList[0].GameTypeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 607, 239, 92, 25, false );	
	m_BZ_RoomList[0].GameTypeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[0].GameTypeTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[0].GameTypeTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[0].GameTypeTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[0].GameTypeTxt->SetText(_RT( "") );
	m_BZ_RoomList[0].GameTypeTxt->SetFocusCheck( false );

	m_BZ_RoomList[0].CurtUserNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 701, 239, 60, 25, false );	
	m_BZ_RoomList[0].CurtUserNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[0].CurtUserNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[0].CurtUserNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[0].CurtUserNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[0].CurtUserNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[0].CurtUserNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[0].RoomStateTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 763, 239, 110, 25, false );	
	m_BZ_RoomList[0].RoomStateTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[0].RoomStateTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[0].RoomStateTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[0].RoomStateTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[0].RoomStateTxt->SetText(_RT( "") );
	m_BZ_RoomList[0].RoomStateTxt->SetFocusCheck( false );

	m_BZ_RoomList[0].RoomListBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 135, 239, 738, 22, false );
	m_BZ_RoomList[0].RoomListBtn->SetBtnImage( TID_None, TID_LISTBTN_SELECT, TID_LISTBTN_SELECT, TID_None );
	m_BZ_RoomList[0].RoomListBtn->SetBtnHold( true );
	
	m_BZ_RoomList[1].RoomNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 133, 261, 52, 25, false );
	m_BZ_RoomList[1].RoomNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[1].RoomNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[1].RoomNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[1].RoomNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[1].RoomNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[1].RoomNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[1].PassWordImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 191, 267, 12, 12, false );
	m_BZ_RoomList[1].PassWordImg->SetClientImage( TID_None );

	m_BZ_RoomList[1].RoomNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 209, 261, 358, 25, false );
	m_BZ_RoomList[1].RoomNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[1].RoomNameTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[1].RoomNameTxt->SetAlignText( n_atLeft, n_atCenter );
	m_BZ_RoomList[1].RoomNameTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[1].RoomNameTxt->SetText(_RT( "") );
	m_BZ_RoomList[1].RoomNameTxt->SetFocusCheck( false );

	m_BZ_RoomList[1].CanViewTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 543, 261, 61, 25, false );
	m_BZ_RoomList[1].CanViewTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[1].CanViewTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[1].CanViewTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[1].CanViewTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[1].CanViewTxt->SetText(_RT( "") );
	m_BZ_RoomList[1].CanViewTxt->SetFocusCheck( false );

	m_BZ_RoomList[1].GameTypeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 607, 261, 92, 25, false );	
	m_BZ_RoomList[1].GameTypeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[1].GameTypeTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[1].GameTypeTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[1].GameTypeTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[1].GameTypeTxt->SetText(_RT( "") );
	m_BZ_RoomList[1].GameTypeTxt->SetFocusCheck( false );

	m_BZ_RoomList[1].CurtUserNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 701, 261, 60, 25, false );	
	m_BZ_RoomList[1].CurtUserNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[1].CurtUserNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[1].CurtUserNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[1].CurtUserNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[1].CurtUserNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[1].CurtUserNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[1].RoomStateTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 763, 261, 110, 25, false );	
	m_BZ_RoomList[1].RoomStateTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[1].RoomStateTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[1].RoomStateTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[1].RoomStateTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[1].RoomStateTxt->SetText(_RT( "") );
	m_BZ_RoomList[1].RoomStateTxt->SetFocusCheck( false );
	
	m_BZ_RoomList[1].RoomListBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 135, 261, 738, 22, false );
	m_BZ_RoomList[1].RoomListBtn->SetBtnImage( TID_None, TID_LISTBTN_SELECT, TID_LISTBTN_SELECT, TID_None );
	m_BZ_RoomList[1].RoomListBtn->SetBtnHold( true );
	
	m_BZ_RoomList[2].RoomNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 133, 283, 52, 25, false );
	m_BZ_RoomList[2].RoomNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[2].RoomNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[2].RoomNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[2].RoomNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[2].RoomNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[2].RoomNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[2].PassWordImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 191, 289, 12, 12, false );
	m_BZ_RoomList[2].PassWordImg->SetClientImage( TID_None );

	m_BZ_RoomList[2].RoomNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 209, 283, 358, 25, false );
	m_BZ_RoomList[2].RoomNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[2].RoomNameTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[2].RoomNameTxt->SetAlignText( n_atLeft, n_atCenter );
	m_BZ_RoomList[2].RoomNameTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[2].RoomNameTxt->SetText(_RT( "") );
	m_BZ_RoomList[2].RoomNameTxt->SetFocusCheck( false );

	m_BZ_RoomList[2].CanViewTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 543, 283, 61, 25, false );
	m_BZ_RoomList[2].CanViewTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[2].CanViewTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[2].CanViewTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[2].CanViewTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[2].CanViewTxt->SetText(_RT( "") );
	m_BZ_RoomList[2].CanViewTxt->SetFocusCheck( false );

	m_BZ_RoomList[2].GameTypeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 607, 283, 92, 25, false );	
	m_BZ_RoomList[2].GameTypeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[2].GameTypeTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[2].GameTypeTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[2].GameTypeTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[2].GameTypeTxt->SetText(_RT( "") );
	m_BZ_RoomList[2].GameTypeTxt->SetFocusCheck( false );

	m_BZ_RoomList[2].CurtUserNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 701, 283, 60, 25, false );	
	m_BZ_RoomList[2].CurtUserNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[2].CurtUserNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[2].CurtUserNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[2].CurtUserNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[2].CurtUserNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[2].CurtUserNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[2].RoomStateTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 763, 283, 110, 25, false );	
	m_BZ_RoomList[2].RoomStateTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[2].RoomStateTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[2].RoomStateTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[2].RoomStateTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[2].RoomStateTxt->SetText(_RT( "") );
	m_BZ_RoomList[2].RoomStateTxt->SetFocusCheck( false );

	m_BZ_RoomList[2].RoomListBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 135, 283, 738, 22, false );
	m_BZ_RoomList[2].RoomListBtn->SetBtnImage( TID_None, TID_LISTBTN_SELECT, TID_LISTBTN_SELECT, TID_None );
	m_BZ_RoomList[2].RoomListBtn->SetBtnHold( true );
	
	m_BZ_RoomList[3].RoomNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 133, 305, 52, 25, false );
	m_BZ_RoomList[3].RoomNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[3].RoomNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[3].RoomNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[3].RoomNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[3].RoomNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[3].RoomNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[3].PassWordImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 191, 311, 12, 12, false );
	m_BZ_RoomList[3].PassWordImg->SetClientImage( TID_None );

	m_BZ_RoomList[3].RoomNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 209, 305, 358, 25, false );
	m_BZ_RoomList[3].RoomNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[3].RoomNameTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[3].RoomNameTxt->SetAlignText( n_atLeft, n_atCenter );
	m_BZ_RoomList[3].RoomNameTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[3].RoomNameTxt->SetText(_RT( "") );
	m_BZ_RoomList[3].RoomNameTxt->SetFocusCheck( false );

	m_BZ_RoomList[3].CanViewTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 543, 305, 61, 25, false );
	m_BZ_RoomList[3].CanViewTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[3].CanViewTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[3].CanViewTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[3].CanViewTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[3].CanViewTxt->SetText(_RT( "") );
	m_BZ_RoomList[3].CanViewTxt->SetFocusCheck( false );

	m_BZ_RoomList[3].GameTypeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 607, 305, 92, 25, false );	
	m_BZ_RoomList[3].GameTypeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[3].GameTypeTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[3].GameTypeTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[3].GameTypeTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[3].GameTypeTxt->SetText(_RT( "") );
	m_BZ_RoomList[3].GameTypeTxt->SetFocusCheck( false );

	m_BZ_RoomList[3].CurtUserNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 701, 305, 60, 25, false );	
	m_BZ_RoomList[3].CurtUserNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[3].CurtUserNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[3].CurtUserNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[3].CurtUserNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[3].CurtUserNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[3].CurtUserNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[3].RoomStateTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 763, 305, 110, 25, false );	
	m_BZ_RoomList[3].RoomStateTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[3].RoomStateTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[3].RoomStateTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[3].RoomStateTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[3].RoomStateTxt->SetText(_RT( "") );
	m_BZ_RoomList[3].RoomStateTxt->SetFocusCheck( false );
	
	m_BZ_RoomList[3].RoomListBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 135, 305, 738, 22, false );
	m_BZ_RoomList[3].RoomListBtn->SetBtnImage( TID_None, TID_LISTBTN_SELECT, TID_LISTBTN_SELECT, TID_None );
	m_BZ_RoomList[3].RoomListBtn->SetBtnHold( true );

	m_BZ_RoomList[4].RoomNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 133, 327, 52, 25, false );
	m_BZ_RoomList[4].RoomNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[4].RoomNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[4].RoomNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[4].RoomNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[4].RoomNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[4].RoomNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[4].PassWordImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 191, 334, 12, 12, false );
	m_BZ_RoomList[4].PassWordImg->SetClientImage( TID_None );

	m_BZ_RoomList[4].RoomNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 209, 327, 358, 25, false );
	m_BZ_RoomList[4].RoomNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[4].RoomNameTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[4].RoomNameTxt->SetAlignText( n_atLeft, n_atCenter );
	m_BZ_RoomList[4].RoomNameTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[4].RoomNameTxt->SetText(_RT( "") );
	m_BZ_RoomList[4].RoomNameTxt->SetFocusCheck( false );

	m_BZ_RoomList[4].CanViewTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 543, 327, 61, 25, false );
	m_BZ_RoomList[4].CanViewTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[4].CanViewTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[4].CanViewTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[4].CanViewTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[4].CanViewTxt->SetText(_RT( "") );
	m_BZ_RoomList[4].CanViewTxt->SetFocusCheck( false );

	m_BZ_RoomList[4].GameTypeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 607, 327, 92, 25, false );	
	m_BZ_RoomList[4].GameTypeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[4].GameTypeTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[4].GameTypeTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[4].GameTypeTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[4].GameTypeTxt->SetText(_RT( "") );
	m_BZ_RoomList[4].GameTypeTxt->SetFocusCheck( false );

	m_BZ_RoomList[4].CurtUserNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 701, 327, 60, 25, false );	
	m_BZ_RoomList[4].CurtUserNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[4].CurtUserNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[4].CurtUserNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[4].CurtUserNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[4].CurtUserNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[4].CurtUserNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[4].RoomStateTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 763, 327, 110, 25, false );	
	m_BZ_RoomList[4].RoomStateTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[4].RoomStateTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[4].RoomStateTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[4].RoomStateTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[4].RoomStateTxt->SetText(_RT( "") );
	m_BZ_RoomList[4].RoomStateTxt->SetFocusCheck( false );

	m_BZ_RoomList[4].RoomListBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 135, 327, 738, 22, false );
	m_BZ_RoomList[4].RoomListBtn->SetBtnImage( TID_None, TID_LISTBTN_SELECT, TID_LISTBTN_SELECT, TID_None );
	m_BZ_RoomList[4].RoomListBtn->SetBtnHold( true );
	
	m_BZ_RoomList[5].RoomNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 133, 349, 52, 25, false );
	m_BZ_RoomList[5].RoomNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[5].RoomNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[5].RoomNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[5].RoomNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[5].RoomNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[5].RoomNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[5].PassWordImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 191, 355, 12, 12, false );
	m_BZ_RoomList[5].PassWordImg->SetClientImage( TID_None );

	m_BZ_RoomList[5].RoomNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 209, 349, 358, 25, false );
	m_BZ_RoomList[5].RoomNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[5].RoomNameTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[5].RoomNameTxt->SetAlignText( n_atLeft, n_atCenter );
	m_BZ_RoomList[5].RoomNameTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[5].RoomNameTxt->SetText(_RT( "") );
	m_BZ_RoomList[5].RoomNameTxt->SetFocusCheck( false );

	m_BZ_RoomList[5].CanViewTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 543, 349, 61, 25, false );
	m_BZ_RoomList[5].CanViewTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[5].CanViewTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[5].CanViewTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[5].CanViewTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[5].CanViewTxt->SetText(_RT( "") );
	m_BZ_RoomList[5].CanViewTxt->SetFocusCheck( false );

	m_BZ_RoomList[5].GameTypeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 607, 349, 92, 25, false );	
	m_BZ_RoomList[5].GameTypeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[5].GameTypeTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[5].GameTypeTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[5].GameTypeTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[5].GameTypeTxt->SetText(_RT( "") );
	m_BZ_RoomList[5].GameTypeTxt->SetFocusCheck( false );

	m_BZ_RoomList[5].CurtUserNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 701, 349, 60, 25, false );	
	m_BZ_RoomList[5].CurtUserNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[5].CurtUserNumTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[5].CurtUserNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[5].CurtUserNumTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[5].CurtUserNumTxt->SetText(_RT( "") );
	m_BZ_RoomList[5].CurtUserNumTxt->SetFocusCheck( false );

	m_BZ_RoomList[5].RoomStateTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 763, 349, 110, 25, false );	
	m_BZ_RoomList[5].RoomStateTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_BZ_RoomList[5].RoomStateTxt->SetClientImage( TID_None );	
	m_BZ_RoomList[5].RoomStateTxt->SetAlignText( n_atCenter, n_atCenter );
	m_BZ_RoomList[5].RoomStateTxt->SetTextColor( 5, 5, 5 );
	m_BZ_RoomList[5].RoomStateTxt->SetText(_RT( "") );
	m_BZ_RoomList[5].RoomStateTxt->SetFocusCheck( false );

	m_BZ_RoomList[5].RoomListBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 135, 349, 738, 22, false );
	m_BZ_RoomList[5].RoomListBtn->SetBtnImage( TID_None, TID_LISTBTN_SELECT, TID_LISTBTN_SELECT, TID_None );
	m_BZ_RoomList[5].RoomListBtn->SetBtnHold( true );

	m_BZ_RoomList_SelectImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 135, 239, 738, 22, false );
	m_BZ_RoomList_SelectImg->SetClientImage( TID_None );
	m_BZ_RoomList_SelectImg->SetFocusCheck( false );
//	m_BZ_RoomList_SelectImg->SetWndRepositionR( 135, 239 );		//	239, 261, 283, 305, 327, 349, 

	m_pRoomListScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, WID_BZ_ROOMLIST_WND, 875, 239, 13, 133, false ); 
	m_pRoomListScroll->SetClientImage( TID_None );
	m_pRoomListScroll->InitSlide( stVertSld, 0, c_Max_BZRoomList_Num - c_Max_Draw_BZRoomList_Num, 0, 11 );//Type, min, max, value, 썸의 폭
	m_pRoomListScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_None );	
	m_pRoomListScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pRoomListScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	///-- Find Ready Room Button 
	m_pFindReadyRoomBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 477, 380, 98, 34, false );
	m_pFindReadyRoomBtn->SetBtnImage( TID_ROOMLIST_BTN_NORMAL, TID_ROOMLIST_BTN_FOCUS, TID_ROOMLIST_BTN_SELECT, TID_ROOMLIST_BTN_DISABLE );
	m_pFindReadyRoomBtn->SetFontg( n_ftGulimChe, 13, n_fwNormal, n_fdNormal );
	m_pFindReadyRoomBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pFindReadyRoomBtn->SetTextColor( 255, 255, 255 );
	m_pFindReadyRoomBtn->SetText( G_STRING_CHAR( IDS_WND_VIEW_WAITINGROOM ) );
//	m_pFindReadyRoomBtn->SetText( G_STRING_CHAR( IDS_WND_VIEW_ALLROOM ) );

	///-- Create Room Button 
	m_pCreateRoomBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 581, 380, 98, 34, false );
	m_pCreateRoomBtn->SetBtnImage( TID_ROOMLIST_BTN_NORMAL, TID_ROOMLIST_BTN_FOCUS, TID_ROOMLIST_BTN_SELECT, TID_ROOMLIST_BTN_DISABLE );
	m_pCreateRoomBtn->SetFontg( n_ftGulimChe, 13, n_fwNormal, n_fdNormal );
	m_pCreateRoomBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pCreateRoomBtn->SetTextColor( 255, 255, 255 );
	m_pCreateRoomBtn->SetText( G_STRING_CHAR( IDS_WND_CREATEROOM ) );

	///-- CanView Room Button
	m_pCanViewRoomBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 685, 380, 98, 34, false );
	m_pCanViewRoomBtn->SetBtnImage( TID_ROOMLIST_BTN_NORMAL, TID_ROOMLIST_BTN_FOCUS, TID_ROOMLIST_BTN_SELECT, TID_ROOMLIST_BTN_DISABLE );
	m_pCanViewRoomBtn->SetFontg( n_ftGulimChe, 13 , n_fwNormal, n_fdNormal );
	m_pCanViewRoomBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pCanViewRoomBtn->SetTextColor( 255, 255, 255 );
	m_pCanViewRoomBtn->SetText( G_STRING_CHAR( IDS_GAME_VIEW ) );	
	
#ifndef JUST_WATCH
	m_pCanViewRoomBtn->SetBtnHold(true);
#endif	

	///-- Join Room Button 
	m_pJoinRoomBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 789, 380, 98, 34, false );
	m_pJoinRoomBtn->SetBtnImage( TID_ROOMLIST_BTN_NORMAL, TID_ROOMLIST_BTN_FOCUS, TID_ROOMLIST_BTN_SELECT, TID_ROOMLIST_BTN_DISABLE );
	m_pJoinRoomBtn->SetFontg( n_ftGulimChe, 13, n_fwNormal, n_fdNormal );
	m_pJoinRoomBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pJoinRoomBtn->SetTextColor( 255, 255, 255 );
	m_pJoinRoomBtn->SetText( G_STRING_CHAR( IDS_WND_JOINROOM ) );

	///-- Chating Window Txt 
	m_pChatWndTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 171, 457, 270, 22, false );	
	m_pChatWndTxt->SetFontg( n_ftGulimChe, 13, n_fwNormal, n_fdNormal );
	m_pChatWndTxt->SetClientImage( TID_None );	
	m_pChatWndTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pChatWndTxt->SetTextColor( 255, 255, 255 );
	m_pChatWndTxt->SetText( G_STRING_CHAR( IDS_WND_CHATWND ) );
	m_pChatWndTxt->SetFocusCheck( false );
		
	///-- Draw Chating Message Txt 
	m_pChatMsgTxt[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 137, 495, 462, 23, false );
//	m_pChatMsgTxt[0] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 398, 6, 13, 13, false );
//	m_pChatMsgTxt[0]->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );		
	m_pChatMsgTxt[0]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatMsgTxt[0]->SetClientImage( TID_None );	
	m_pChatMsgTxt[0]->SetAlignText( n_atLeft, n_atCenter );
	m_pChatMsgTxt[0]->SetTextColor( 5, 5, 5 );
	m_pChatMsgTxt[0]->SetText( _RT( "" ) );
	m_pChatMsgTxt[0]->SetFocusCheck( false );

	m_pChatMsgTxt[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 137, 518, 462, 23, false );
//	m_pChatMsgTxt[1] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 398, 6, 13, 13, false );
//	m_pChatMsgTxt[1]->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );		
	m_pChatMsgTxt[1]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatMsgTxt[1]->SetClientImage( TID_None );	
	m_pChatMsgTxt[1]->SetAlignText( n_atLeft, n_atCenter );
	m_pChatMsgTxt[1]->SetTextColor( 5, 5, 5 );
	m_pChatMsgTxt[1]->SetText( _RT( "" ) );
	m_pChatMsgTxt[1]->SetFocusCheck( false );

	m_pChatMsgTxt[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 137, 541, 462, 23, false );
//	m_pChatMsgTxt[2] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 398, 6, 13, 13, false );
//	m_pChatMsgTxt[2]->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );		
	m_pChatMsgTxt[2]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatMsgTxt[2]->SetClientImage( TID_None );	
	m_pChatMsgTxt[2]->SetAlignText( n_atLeft, n_atCenter );
	m_pChatMsgTxt[2]->SetTextColor( 5, 5, 5 );
	m_pChatMsgTxt[2]->SetText( _RT( "" ) );
	m_pChatMsgTxt[2]->SetFocusCheck( false );

	m_pChatMsgTxt[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 137, 564, 462, 23, false );	
//	m_pChatMsgTxt[3] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 398, 6, 13, 13, false );
//	m_pChatMsgTxt[3]->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );		
	m_pChatMsgTxt[3]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatMsgTxt[3]->SetClientImage( TID_None );	
	m_pChatMsgTxt[3]->SetAlignText( n_atLeft, n_atCenter );
	m_pChatMsgTxt[3]->SetTextColor( 5, 5, 5 );
	m_pChatMsgTxt[3]->SetText( _RT( "" ) );
	m_pChatMsgTxt[3]->SetFocusCheck( false );

	m_pChatMsgScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, WID_BZ_ROOMLIST_WND, 596, 493, 13, 96, false );
	m_pChatMsgScroll->SetClientImage( TID_None );
	m_pChatMsgScroll->InitSlide( stVertSld, 0, c_Max_BZChatMsg_Num - c_Max_Draw_BZChatMsg_Num, 0, 11 );//Type, min, max, value, 썸의 폭
	m_pChatMsgScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_None );	
	m_pChatMsgScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pChatMsgScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

#ifdef BATTLEROOM_HELPBTN_ERASE

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND , 516, 593, 46, 23, false );
	img->SetClientImage( TID_ROOM_CHAT_BLOCKBOX );

	m_pChatInputBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, WID_BZ_ROOMLIST_WND, 138, 595, 377 + 47 , 19, false );
	m_pChatInputBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatInputBox->SetTextSize( c_Max_ChatMsg_Length );
	m_pChatInputBox->SetClientImage( TID_None );
	m_pChatInputBox->SetClientColor( 5, 5, 5 );
	m_pChatInputBox->SetAlignText( n_atLeft, n_atCenter );
	m_pChatInputBox->SetBlank( 4, 0, 0, 0 );
	m_pChatInputBox->SetEditStyle( esText );
	m_pChatInputBox->SetTextColor( 255, 255, 255 );
	m_pChatInputBox->SetText( _RT("") );
	m_pChatInputBox->SetReturnActionClear( true );
	m_pChatInputBox->InitEditBox();
//	m_pChatInputBox->ReceiveEvent( n_emSetFocus, 0, 0, 0, 0 );
	
	///-- Whisper Button 
	m_pWhisperBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 563, 593, 46, 23, false );
	m_pWhisperBtn->SetBtnImage( TID_CHATWND_BTN_NORMAL, TID_CHATWND_BTN_FOCUS, TID_CHATWND_BTN_SELECT, TID_CHATWND_BTN_DISABLE );
	m_pWhisperBtn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pWhisperBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pWhisperBtn->SetTextColor( 255, 255, 255 );
	m_pWhisperBtn->SetText( G_STRING_CHAR( IDS_WND_WHISPER ) );
	
#else 

	
	m_pChatInputBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, WID_BZ_ROOMLIST_WND, 138, 595, 377, 19, false );
	m_pChatInputBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatInputBox->SetTextSize( c_Max_ChatMsg_Length );
	m_pChatInputBox->SetClientImage( TID_None );
	m_pChatInputBox->SetClientColor( 5, 5, 5 );
	m_pChatInputBox->SetAlignText( n_atLeft, n_atCenter );
	m_pChatInputBox->SetBlank( 4, 0, 0, 0 );
	m_pChatInputBox->SetEditStyle( esText );
	m_pChatInputBox->SetTextColor( 255, 255, 255 );
	m_pChatInputBox->SetText( _RT("") );
	m_pChatInputBox->SetReturnActionClear( true );
	m_pChatInputBox->InitEditBox();
	//	m_pChatInputBox->ReceiveEvent( n_emSetFocus, 0, 0, 0, 0 );
	
	
	///-- Whisper Button 
	m_pWhisperBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 516, 593, 46, 23, false );
	m_pWhisperBtn->SetBtnImage( TID_CHATWND_BTN_NORMAL, TID_CHATWND_BTN_FOCUS, TID_CHATWND_BTN_SELECT, TID_CHATWND_BTN_DISABLE );
	m_pWhisperBtn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pWhisperBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pWhisperBtn->SetTextColor( 255, 255, 255 );
	m_pWhisperBtn->SetText( G_STRING_CHAR( IDS_WND_WHISPER ) );
	
	
	///-- Chating Help Button 
	m_pChatHelpBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 563, 593, 46, 23, false );
	m_pChatHelpBtn->SetBtnImage( TID_CHATWND_BTN_NORMAL, TID_CHATWND_BTN_FOCUS, TID_CHATWND_BTN_SELECT, TID_CHATWND_BTN_DISABLE );
	m_pChatHelpBtn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatHelpBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pChatHelpBtn->SetTextColor( 255, 255, 255 );
	m_pChatHelpBtn->SetText( G_STRING_CHAR( IDS_WND_HELP ) );


#endif 


//	m_pModeListBox = (CListBox*)nRui->CreateWnd( n_wtListBox, WID_None, WID_BZ_ROOMLIST_WND, 398, 6, 13, 13, false );
//	m_pModeListBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
//	m_pModeListBox->SetScrollBarWidth( 16 );
//	m_pUserListzBox->SetTextColor( 0, 0, 0 );
//	m_pUserListBox->SetLBoxBlank( 2, 2, 2, 2 );
//	m_pUserListBox->SetInsertOrder( ioLast );
//	m_pUserListBox->SetLineHeight( 14 );
//	m_pUserListBox->SetScrollBar();

	m_pUserListTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_ROOMLIST_WND, 668, 457, 90, 22, false );	
	m_pUserListTxt->SetFontg( n_ftGulimChe, 13, n_fwNormal, n_fdNormal );
	m_pUserListTxt->SetClientImage( TID_None );	
	m_pUserListTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pUserListTxt->SetTextColor( 255, 255, 255 );
	m_pUserListTxt->SetText( G_STRING_CHAR( IDS_WND_USER_LIST ) );
	m_pUserListTxt->SetFocusCheck( false );
			
	m_pUserListBtn[0] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 639, 494, 228, 20, false );	
	m_pUserListBtn[0]->SetBtnImage( TID_None, TID_LISTBTN_SELECT, TID_LISTBTN_SELECT, TID_None );
	m_pUserListBtn[0]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pUserListBtn[0]->SetAlignText( n_atLeft, n_atCenter );
	m_pUserListBtn[0]->SetTextColor( 5, 5, 5 );
	m_pUserListBtn[0]->SetText( _RT( "" ) );
	m_pUserListBtn[0]->SetBtnHold( true );

	m_pUserListBtn[1] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 639, 514, 228, 20, false );	
	m_pUserListBtn[1]->SetBtnImage( TID_None, TID_LISTBTN_SELECT, TID_LISTBTN_SELECT, TID_None );
	m_pUserListBtn[1]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pUserListBtn[1]->SetAlignText( n_atLeft, n_atCenter );
	m_pUserListBtn[1]->SetTextColor( 5, 5, 5 );
	m_pUserListBtn[1]->SetText( _RT( "" ) );
	m_pUserListBtn[1]->SetBtnHold( true );

	m_pUserListBtn[2] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 639, 534, 228, 20, false );	
	m_pUserListBtn[2]->SetBtnImage( TID_None, TID_LISTBTN_SELECT, TID_LISTBTN_SELECT, TID_None );
	m_pUserListBtn[2]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pUserListBtn[2]->SetAlignText( n_atLeft, n_atCenter );
	m_pUserListBtn[2]->SetTextColor( 5, 5, 5 );
	m_pUserListBtn[2]->SetText( _RT( "" ) );
	m_pUserListBtn[2]->SetBtnHold( true );

	m_pUserListBtn[3] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 639, 554, 228, 20, false );	
	m_pUserListBtn[3]->SetBtnImage( TID_None, TID_LISTBTN_SELECT, TID_LISTBTN_SELECT, TID_None );
	m_pUserListBtn[3]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pUserListBtn[3]->SetAlignText( n_atLeft, n_atCenter );
	m_pUserListBtn[3]->SetTextColor( 5, 5, 5 );
	m_pUserListBtn[3]->SetText( _RT( "" ) );
	m_pUserListBtn[3]->SetBtnHold( true );
	
	///-- User List Scroll 
	m_pUserListScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, WID_BZ_ROOMLIST_WND, 872, 493, 13, 85, false ); 
	m_pUserListScroll->SetClientImage( TID_None );	
	m_pUserListScroll->InitSlide( stVertSld, 0, c_Max_BZUserList_Num - c_Max_Draw_BZUserList_Num, 0, 11 );//Type, min, max, value, 썸의 폭
	m_pUserListScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_None );	
	m_pUserListScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pUserListScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	m_BZ_UserList_SelectImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 639, 494, 228, 20, false );
	m_BZ_UserList_SelectImg->SetClientImage( TID_None );
	m_BZ_UserList_SelectImg->SetFocusCheck( false );
//	m_BZ_UserList_SelectImg->SetWndRepositionR( 639, 494 );		//	494, 514, 534, 554

	m_pDisableWndImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_ROOMLIST_WND, 0, 0, 1024, 768, false );
	m_pDisableWndImg->SetClientImage( TID_COMBOBOX_LIST_IMG_CENTER_CENTER );
	m_pDisableWndImg->SetClientColor( 0, 0, 0 );
	m_pDisableWndImg->SetClientAlpah( 100 );
	m_pDisableWndImg->SetVisible( false );

	m_pSortRoomListBtn[n_BZ_Sort_RoomNum] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 133, 212, 52, 25, false );
	m_pSortRoomListBtn[n_BZ_Sort_RoomNum]->SetBtnImage( TID_None, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_None );

	m_pSortRoomListBtn[n_BZ_Sort_RoomName] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 184, 212, 360, 25, false );
	m_pSortRoomListBtn[n_BZ_Sort_RoomName]->SetBtnImage( TID_None, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_None );

	m_pSortRoomListBtn[n_BZ_Sort_CanView] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 543, 212, 63, 25, false );
	m_pSortRoomListBtn[n_BZ_Sort_CanView]->SetBtnImage( TID_None, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_None );
	
	m_pSortRoomListBtn[n_BZ_Sort_GameType] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 605, 212, 95, 25, false );
	m_pSortRoomListBtn[n_BZ_Sort_GameType]->SetBtnImage( TID_None, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_None );

	m_pSortRoomListBtn[n_BZ_Sort_UserNum] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 700, 212, 62, 25, false );
	m_pSortRoomListBtn[n_BZ_Sort_UserNum]->SetBtnImage( TID_None, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_None );

	m_pSortRoomListBtn[n_BZ_Sort_RoomState] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_ROOMLIST_WND, 761, 212, 128, 25, false );
	m_pSortRoomListBtn[n_BZ_Sort_RoomState]->SetBtnImage( TID_None, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_BZ_SELECTZONE_BTNFOCUS_IMG, TID_None );


	return;
}

///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CBZ_RoomListWndProc::Update()
{
	return;
}

///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CBZ_RoomListWndProc::Proc( SEventMessage* EMsg )
{	
	//=========================================================================================
	if( EMsg->FocusWnd == m_pGoOutBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{				
			g_RockClient.Send_CsOutWaitingRoom(); // 바로 처리? 응답없이 창닫기? ㅡㅡ; 아닐듯...
		}
	}
	//=========================================================================================
	else if( EMsg->FocusWnd == m_pFindReadyRoomBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_bIsOnlyWaitingRoom = !(m_bIsOnlyWaitingRoom);

			if( m_bIsOnlyWaitingRoom )
			{
				m_pFindReadyRoomBtn->SetText( G_STRING_CHAR( IDS_WND_VIEW_ALLROOM ) );

				m_BZ_RoomInfoList_Waiting.clear();
				
				std::vector < SRpScFightRoomList::_SFightRoom > :: iterator ir;							
				
				ir = m_BZ_RoomInfoList.begin();
				
				for( ; ir != m_BZ_RoomInfoList.end() ; ++ir )
				{
					if( (*ir).theStatus == 0 )					// 대기중인 방 
					{
						m_BZ_RoomInfoList_Waiting.push_back( (*ir) );						
					}
				}				
			}
			else
			{
				m_pFindReadyRoomBtn->SetText( G_STRING_CHAR( IDS_WND_VIEW_WAITINGROOM ) );
			}
		}
	}
	else if( EMsg->FocusWnd == m_pCreateRoomBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pChatInputBox->GetID(), n_emEditDisable, 0, 0, 0, 0 );	

			m_pDisableWndImg->SetVisible( true );

//			nRui->SendEvent( WID_BZ_CREATEROOM_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
			g_BZ_CreateRoomWnd.Open();

			nRui->SetModal( WID_BZ_CREATEROOM_WND );

			g_BZ_SelectZoneWnd.m_BZ_WndType = n_BattleZone_CreateRoomWnd;			
		}
	}
	else if( EMsg->FocusWnd == m_pCanViewRoomBtn->GetID() )		///-- 게임 관전
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_SelectRoomIndex != -1 )
			{
				int room_index = m_StartLineNum_RoomList + m_SelectRoomIndex;
				
				if( !m_BZ_RoomInfoList[room_index].theLock && 
					 m_BZ_RoomInfoList[room_index].theStatus == 0 
#ifdef JUST_WATCH					 
					 && m_BZ_RoomInfoList[room_index].theWatchable 
#endif									 
					 )
				{
					g_RockClient.Send_CsJoinFightRoom( m_BZ_RoomInfoList[room_index].theRoomID, _RT( "" ), 1 );				
				}			
			}	
		}	
	}
	else if( EMsg->FocusWnd == m_pJoinRoomBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_SelectRoomIndex != -1 )
			{
				int room_index = m_StartLineNum_RoomList + m_SelectRoomIndex;			

				if( m_BZ_RoomInfoList[room_index].theLock )
				{
					g_StrInputWnd.Open( n_STRINPUT_BZ_PASSWORD, m_BZ_RoomInfoList[room_index].theRoomID );
				}
				else
				{			
					if( m_BZ_RoomInfoList[room_index].theStatus == 0 )		// 대기중인 방이라면...
					{
						g_RockClient.Send_CsJoinFightRoom( m_BZ_RoomInfoList[room_index].theRoomID, _RT( "" ) );
					}
				}
			}
		}
	}
#ifndef BATTLEROOM_HELPBTN_ERASE
	//=========================================================================================
	else if( EMsg->FocusWnd == m_pChatHelpBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			int i = 0;
			char str_temp[512] = {0,};

			for( i = 0; i < g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat; ++i )
			{
//				InsertBZHelpChatMsg(_RT("도움말 리스트가 보여집니다."));				 
				Rsprintf( RWCHAR(str_temp), _RT("[%s] %s"), G_STRING_CHAR( IDS_WND_HELP ), RWCHAR(g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipAdminMsg_PVPChat[i]) ); 
				InsertBZHelpChatMsg( str_temp );		
			}
		}
	}
#endif 
	else if( EMsg->FocusWnd == m_pWhisperBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_SelectUserIndex == -1 )
			{
				InsertBZHelpChatMsg( G_STRING_CHAR( IDS_GAME_WHISPER ) );	 
				m_pChatInputBox->SetText(_RT("/w "));								 	
				PostMessage( m_pChatInputBox->GetHEdit(), WM_USER + 44, NULL, NULL );
				nRui->SendEvent( m_pWhisperBtn->GetID(), n_emSetFocus, 0, 0, 0, 0 );					
			}
			else
			{
				int user_index = m_StartLineNum_UserList + m_SelectUserIndex;			
				char whisper_temp[128] = {0,};

				Rsprintf( RWCHAR(whisper_temp), _RT("/w %s "), RWCHAR(m_BZ_UserNameList[user_index].UserName) );
				m_pChatInputBox->SetText( whisper_temp );
				PostMessage( m_pChatInputBox->GetHEdit(), WM_USER + 44, NULL, NULL );
				nRui->SendEvent( m_pWhisperBtn->GetID(), n_emSetFocus, 0, 0, 0, 0 );					

				m_SelectUserIndex = -1;
				m_BZ_UserList_SelectImg->SetClientImage( TID_None );
			}
		}
	}
	//=========================================================================================
	else if( EMsg->FocusWnd == m_pSortRoomListBtn[n_BZ_Sort_RoomNum]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_Draw_SortType = n_BZ_Sort_RoomNum;
			SortDrawRoomList();
		}
	}
	else if( EMsg->FocusWnd == m_pSortRoomListBtn[n_BZ_Sort_RoomName]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_Draw_SortType = n_BZ_Sort_RoomName;
			SortDrawRoomList();
		}
	}
	else if( EMsg->FocusWnd == m_pSortRoomListBtn[n_BZ_Sort_GameType]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_Draw_SortType = n_BZ_Sort_GameType;
			SortDrawRoomList();
		}
	}
	else if( EMsg->FocusWnd == m_pSortRoomListBtn[n_BZ_Sort_UserNum]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_Draw_SortType = n_BZ_Sort_UserNum;
			SortDrawRoomList();
		}
	}
	else if( EMsg->FocusWnd == m_pSortRoomListBtn[n_BZ_Sort_RoomState]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_Draw_SortType = n_BZ_Sort_RoomState;
			SortDrawRoomList();
		}
	}	
	//=========================================================================================
	else if( EMsg->FocusWnd == m_BZ_RoomList[0].RoomListBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			m_BZ_RoomList_SelectImg->SetClientImage( TID_LISTBTN_SELECT );
			m_BZ_RoomList_SelectImg->SetWndRepositionR( 135, 239 );		//	239, 261, 283, 305, 327, 349, 
			m_SelectRoomIndex = 0;
			m_ScrollFocus = n_BZ_Focus_RoomList;
		}
	}
	else if( EMsg->FocusWnd == m_BZ_RoomList[1].RoomListBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_RoomList_SelectImg->SetClientImage( TID_LISTBTN_SELECT );
			m_BZ_RoomList_SelectImg->SetWndRepositionR( 135, 261 );		//	239, 261, 283, 305, 327, 349, 
			m_SelectRoomIndex = 1;
			m_ScrollFocus = n_BZ_Focus_RoomList;
		}
	}
	else if( EMsg->FocusWnd == m_BZ_RoomList[2].RoomListBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_RoomList_SelectImg->SetClientImage( TID_LISTBTN_SELECT );
			m_BZ_RoomList_SelectImg->SetWndRepositionR( 135, 283 );		//	239, 261, 283, 305, 327, 349, 
			m_SelectRoomIndex = 2;
			m_ScrollFocus = n_BZ_Focus_RoomList;
		}
	}
	else if( EMsg->FocusWnd == m_BZ_RoomList[3].RoomListBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_RoomList_SelectImg->SetClientImage( TID_LISTBTN_SELECT );
			m_BZ_RoomList_SelectImg->SetWndRepositionR( 135, 305 );		//	239, 261, 283, 305, 327, 349, 
			m_SelectRoomIndex = 3;
			m_ScrollFocus = n_BZ_Focus_RoomList;
		}
	}
	else if( EMsg->FocusWnd == m_BZ_RoomList[4].RoomListBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_RoomList_SelectImg->SetClientImage( TID_LISTBTN_SELECT );
			m_BZ_RoomList_SelectImg->SetWndRepositionR( 135, 327 );		//	239, 261, 283, 305, 327, 349, 
			m_SelectRoomIndex = 4;
			m_ScrollFocus = n_BZ_Focus_RoomList;
		}
	}
	else if( EMsg->FocusWnd == m_BZ_RoomList[5].RoomListBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_RoomList_SelectImg->SetClientImage( TID_LISTBTN_SELECT );
			m_BZ_RoomList_SelectImg->SetWndRepositionR( 135, 349 );		//	239, 261, 283, 305, 327, 349, 
			m_SelectRoomIndex = 5;
			m_ScrollFocus = n_BZ_Focus_RoomList;
		}
	}	
	else if( EMsg->FocusWnd == m_pRoomListScroll->GetID() )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || 
			( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || 
			( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			m_BZ_RoomList_SelectImg->SetClientImage( TID_None );
			m_SelectRoomIndex = -1;
			m_ScrollFocus = n_BZ_Focus_RoomList;

			SetRoomListScroll( (int)EMsg->fParam );				
		}
	}
	//=========================================================================================
	else if( EMsg->FocusWnd == m_pUserListBtn[0]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{		
			m_BZ_UserList_SelectImg->SetClientImage( TID_LISTBTN_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( 639, 494 );		//	494, 514, 534, 554

			m_SelectUserIndex = 0;
			m_ScrollFocus = n_BZ_Focus_UserList;
		}
	}
	else if( EMsg->FocusWnd == m_pUserListBtn[1]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_UserList_SelectImg->SetClientImage( TID_LISTBTN_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( 639, 514 );		//	494, 514, 534, 554

			m_SelectUserIndex = 1;
			m_ScrollFocus = n_BZ_Focus_UserList;
		}
	}
	else if( EMsg->FocusWnd == m_pUserListBtn[2]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_UserList_SelectImg->SetClientImage( TID_LISTBTN_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( 639, 534 );		//	494, 514, 534, 554

			m_SelectUserIndex = 2;
			m_ScrollFocus = n_BZ_Focus_UserList;
		}
	}
	else if( EMsg->FocusWnd == m_pUserListBtn[3]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_BZ_UserList_SelectImg->SetClientImage( TID_LISTBTN_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( 639, 554 );		//	494, 514, 534, 554

			m_SelectUserIndex = 3;
			m_ScrollFocus = n_BZ_Focus_UserList;
		}
	}	
	else if( EMsg->FocusWnd == m_pUserListScroll->GetID() )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || 
			( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || 
			( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			m_BZ_UserList_SelectImg->SetClientImage( TID_None );
			m_SelectRoomIndex = -1;
			m_ScrollFocus = n_BZ_Focus_UserList;
			
			SetUserListScroll( (int)EMsg->fParam );			
		}
	}
	else if( EMsg->FocusWnd == m_pChatMsgScroll->GetID() )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || 
			( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || 
			( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{			
			m_ScrollFocus = n_BZ_Focus_ChatWnd;

			SetChatMsgScroll( (int)EMsg->fParam );			
		}
	}
	else if( EMsg->FocusWnd == m_pChatInputBox->GetID() )
	{
		//by simwoosung
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			m_uChatType = n_BZ_Normal_Chat;
			
			SPcDataParam* PcParam = nRui->GetPcParamInfo();
			SAFE_STR_CPY( m_strChatstring, _RT("") , c_Max_ChatMsg_Length * 2 * sizeof(TCHAR) );
			SAFE_STR_CPY( m_strTempBuf1, _RT("") , c_Max_ChatMsg_Length * 2 * sizeof(TCHAR) );
			SAFE_STR_CPY( m_strTempBuf2, _RT("") , c_Max_ChatMsg_Length * 2 * sizeof(TCHAR) );
			SAFE_STR_CPY( m_strWhisDest, _RT("") , 256 );
			
			nRui->SendEvent( m_pChatInputBox->GetID(), n_emGetText, (EPARAM)m_strChatstring, 
							c_Max_ChatMsg_Length + 1, 0, 0 );										
			nRui->SetEnterFlag( n_nfEditEnter );
			
			RTCHAR *pDest = NULL;					
			pDest = RWCHAR(m_strChatstring);
#ifdef UNICODE_ROCK
			pDest = ::GetStringReturnInStringW(' ', false, 100, pDest, m_strTempBuf1);
		
#else
			pDest = ::GetStringReturnInString(' ', false, 100, pDest, m_strTempBuf1);
#endif
			if( (Rstrcmp(m_strTempBuf1, _RT("/w") ) == 0) || 
				(Rstrcmp(m_strTempBuf1, _RT("/W") ) == 0) )			
			{
#ifdef UNICODE_ROCK				
				pDest = ::GetStringReturnInStringW(' ', false, 100, pDest, m_strWhisDest);

				while( (Rstrcmp(m_strWhisDest, _RT("") ) == 0) && (NULL != pDest) )
				{
					pDest = ::GetStringReturnInStringW(' ', false, 100, pDest, m_strWhisDest);
				}
				
				int nSize = Rstrlen(RWCHAR(m_strWhisDest))*2;
				
#else
				pDest = ::GetStringReturnInString(' ', false, 100, pDest, m_strWhisDest);

				while( (Rstrcmp(m_strWhisDest, _RT("") ) == 0) && (NULL != pDest) )
				{
					pDest = ::GetStringReturnInString(' ', false, 100, pDest, m_strWhisDest);

				}
				int nSize = Rstrlen(RWCHAR(m_strWhisDest));
#endif

	
				if( nSize >= RP_MAX_PC_NAME )
				{
					InsertBZHelpChatMsg( G_STRING_CHAR( IDS_SYSPR_OVER_FLOW_WHIS_TEXT ) );		
					return;
				}
				
				if( (Rstrcmp(m_strWhisDest, _RT("") ) != 0) && (NULL != pDest) )
				{
					m_uChatType = n_BZ_Whisper_Chat;
					SAFE_STR_CPY( m_strChatstring, pDest, c_Max_ChatMsg_Length * 2 );
				}			
			}

			if(Rstrcmp(m_strChatstring, _RT("") ) != 0)
			{
				bool bCoverFlag = false;						//도배방지 플래그
	
				if(  SAFE_TIME_COMPARE( g_nowTime , < , SAFE_TIME_ADD( m_lPrevTime , 1200 ) )  )	//1.2초 이전에 입력		
				{
					bCoverFlag = true;
				}				
				
				if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_strChatstring ) )
				{
					memset( m_strChatstring , 0 , sizeof(TCHAR) * (c_Max_ChatMsg_Length * 2) );
					Rsprintf( RWCHAR(m_strChatstring) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
				}				
				
				switch(m_uChatType)
				{
					case n_BZ_Normal_Chat:
						{
							if(!bCoverFlag)
							{
								//이곳에서 채팅문자 네트워크로 전송				
								g_RockClient.Send_RpCsSay( m_strChatstring );
								//InsertBZNomalChatMsg(PcParam->PCName, m_strChatstring);
							}
							else
							{
								InsertBZNomalChatMsg(PcParam->PCName, 
									 RCHAR(G_STRING_CHAR( IDS_COVER_PREVENT_APPLY )));		
							}
						}
						break;
					case n_BZ_Whisper_Chat:
						{
							if(!bCoverFlag)
							{							
								//이곳에서 귓속말 네트워크로 전송
								if(g_MCommunityWnd.m_MBlockTabWnd.ExistBlockName(m_strWhisDest))
								{
									if(!g_RockClient.m_bIsDrawFullScreenUI)
									{
										SystemPrint( n_DarkGreen ,n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER));	
									}
									else
									{
										if(g_BZ_RoomListWnd.IsVisible)
										{
											g_BZ_RoomListWnd.InsertBZHelpChatMsg(G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER));
										}
										else if(g_BZ_WaitingRoomWnd.IsVisible)
										{
											g_BZ_WaitingRoomWnd.InsertBZHelpChatMsg(G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER));
										}
									}
								}
								else
								{
									g_RockClient.Send_RpCsWhisper(m_strWhisDest, m_strChatstring);
								}								
							}
						}
						break;				
				}
				
				m_lPrevTime = g_nowTime;
			}		

			if(m_uChatType == n_BZ_Whisper_Chat)
			{
				Rsprintf( RWCHAR(m_strChatstring) , _RT("/w %s ") , RWCHAR(m_strWhisDest));			
				m_pChatInputBox->SetText(m_strChatstring);
				PostMessage( m_pChatInputBox->GetHEdit(), WM_USER + 44, NULL, NULL );
			}
			nRui->SendEvent( m_pChatInputBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );		
		}
	}
					
	return;
}

void CBZ_RoomListWndProc::Open()
{
	if(g_RockClient.m_IsMouseRotate)
	{
		nRui->SetCursorType(n_CT_NORMAL);
		g_RockClient.m_IsMouseRotate = false;
	}
	g_MCommunityWnd.SetVisible(false);
	nRui->SendEvent( WID_SkillWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장
	nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );//유료인벤확장
#endif
	g_FrontierMainWnd.Close();	
	
	g_RockClient.m_bIsDrawFullScreenUI = true;		

	char temp[100] = {0.};

	g_BZ_RoomListWnd.m_pDisableWndImg->SetVisible( false );

	g_BZ_WaitingRoomWnd.Close();	

	g_BZ_SelectZoneWnd.m_BZ_WndType = n_BattleZone_RoomListWnd;

	ChatMsgInit();
	SetChatMsgScroll( 0 );

	nRui->SendEvent( WID_BZ_ROOMLIST_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				

	nRui->SendEvent( m_pChatInputBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( m_pChatInputBox->GetID(), n_emClearText, 0, 0, 0, 0 );
	
//	Rsprintf(RWCHAR(temp), "%s ( %s %s )", G_STRING_CHAR( IDS_WND_SELECTGRADE_BASICLEVEL ),		// 초   급 ( 레벨 00 ~ 00 ) 
//					 				 G_STRING_CHAR( IDS_CHARSTATE_LEVEL ), 
//									 g_BZ_SelectGradeWnd.m_BZ_GradeRange[ g_BZ_SelectGradeWnd.m_BZ_GradeType ] );

	if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Nation )
	{
		m_pGradeTypeTxt->SetText( G_STRING_CHAR( IDS_WND_NATION_BATTLE ) );
	}
	else
	{
		m_pGradeTypeTxt->SetText( G_STRING_CHAR( IDS_WND_FREE_BATTLE ) );
	}
	
	m_StartLineNum_RoomList = 0;
	m_StartLineNum_ChatMsg = 0;
	m_StartLineNum_UserList = 0;	

	if(g_ChatMainProc.m_ChatActPrintWnd.IsVisible)
	{
		g_ChatMainProc.SetOutState(COutState_NonAct);
	}

	int i = 0; 

	for( i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( g_Pc.m_PcTable[i].lUnique != -1 )
		{
			g_Pc.DisappearPC( g_Pc.m_PcTable[i].lUnique );		
		}
	}

	m_BZ_Draw_SortType = n_BZ_Sort_RoomState;
	SortDrawRoomList();

	return;
}

void CBZ_RoomListWndProc::Close()
{
	nRui->SendEvent( WID_BZ_ROOMLIST_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

	nRui->SendEvent( m_pChatInputBox->GetID(), n_emEditDisable, 0, 0, 0, 0 );	

	ChatMsgInit();
	SetChatMsgScroll( 0 );
	
	SetRefreshChatMsg();

	return;
}

void CBZ_RoomListWndProc::SetFocusWndColor( bool aIsFocus )
{
	m_pFrameWnd->SetFocusWndColor( aIsFocus );
	
	return;
}

//by simwoosung
void CBZ_RoomListWndProc::InsertBZChatMsg()
{
	if( m_nInsertIndex >= c_Max_BZChatMsg_Num )
	{
		for(int i = 0 ; i < c_Max_BZChatMsg_Num ; i++)
		{
			m_BZ_ChatMsgList[i] = m_BZ_ChatMsgList[i + 1];			
		}
		m_BZ_ChatMsgList[m_nInsertIndex-1] = m_tempChatMsgList;
	}
	else
	{
		m_BZ_ChatMsgList[m_nInsertIndex] = m_tempChatMsgList;
		m_nInsertIndex++;
	}
	
	if( m_nInsertIndex > c_Max_Draw_BZChatMsg_Num )
	{		
		m_StartLineNum_ChatMsg = m_nInsertIndex - c_Max_Draw_BZChatMsg_Num;
		m_pChatMsgScroll->ReSetScroll(0, (c_Max_BZChatMsg_Num - c_Max_Draw_BZChatMsg_Num), m_StartLineNum_ChatMsg);
		if( m_StartLineNum_ChatMsg > (c_Max_BZChatMsg_Num - c_Max_Draw_BZChatMsg_Num) )
		{
			m_StartLineNum_ChatMsg = (c_Max_BZChatMsg_Num - c_Max_Draw_BZChatMsg_Num);	
		}
	}
	
	SetRefreshChatMsg();
}

void CBZ_RoomListWndProc::InsertBZChatMsg(TCHAR * strInString)
{
	RTCHAR * pTempBuf = RWCHAR(strInString);
	RTCHAR * pTempOut = RWCHAR(m_strTempBuf2);

	memset(m_strTempBuf2, NULL, sizeof(TCHAR) *  (c_Max_ChatMsg_Length * 2));

	int nCount = 0;	
	while(pTempBuf)
	{
		if(nCount == 0)
		{
#ifdef UNICODE_ROCK
			pTempBuf = ::GetStringReturnInStringW('\\', false, c_Max_ChatMsg_Length, 
				pTempBuf, pTempOut);
#else
			pTempBuf = ::GetStringReturnInString('\\', false, c_Max_ChatMsg_Length, 
				pTempBuf, pTempOut);
#endif
			Rsprintf(RWCHAR(m_tempChatMsgList.ChatMsg) , _RT("%s"), RWCHAR(pTempOut));
			InsertBZChatMsg();
		}
		else
		{
#ifdef UNICODE_ROCK
			pTempBuf = ::GetStringReturnInStringW('\\', false, (c_Max_ChatMsg_Length - 6), 
				pTempBuf, pTempOut);
#else
			pTempBuf = ::GetStringReturnInString('\\', false, (c_Max_ChatMsg_Length - 6), 
				pTempBuf, pTempOut);
#endif
			Rsprintf(RWCHAR(m_tempChatMsgList.ChatMsg), _RT("    %s"), RWCHAR(pTempOut));
			InsertBZChatMsg();
		}

		SAFE_STR_CPY( pTempOut, _RT(""),  c_Max_ChatMsg_Length * 2 );		
		nCount++;
	}
}

//by simwoosung
void CBZ_RoomListWndProc::InsertBZNomalChatMsg(TCHAR *strDest, TCHAR *strMsg)
{
	Rsprintf(RWCHAR(m_strTempBuf1), _RT("%s : %s"),RWCHAR(strDest), RWCHAR(strMsg));			

	m_tempChatMsgList.ChatType = n_BZ_Normal_Chat;	
	InsertBZChatMsg(m_strTempBuf1);
}

void CBZ_RoomListWndProc::InsertBZWhisperChatMsg(TCHAR *strDest, TCHAR *strMsg, bool bToFlag)
{
	if(bToFlag)
	{
		Rsprintf(RWCHAR(m_strTempBuf1), _RT("To%s : %s"),RWCHAR(strDest), RWCHAR(strMsg));		
	}
	else
	{
		Rsprintf(RWCHAR(m_strTempBuf1), _RT("From%s : %s"),RWCHAR(strDest), RWCHAR(strMsg));		
	}
	m_tempChatMsgList.ChatType = n_BZ_Whisper_Chat;
	InsertBZChatMsg(m_strTempBuf1);
}

void CBZ_RoomListWndProc::InsertBZHelpChatMsg(void *pstrMsg)
{
	char *strMsg = (char*)pstrMsg;
	
	Rsprintf(RWCHAR(m_strTempBuf1), _RT("%s"), RWCHAR(strMsg));
	m_tempChatMsgList.ChatType = n_BZ_Help_Chat;

	InsertBZChatMsg(m_strTempBuf1);	
}


void CBZ_RoomListWndProc::SortDrawRoomList()
{	
	if( m_bIsOnlyWaitingRoom )
	{
		switch( m_BZ_Draw_SortType )
		{
		case n_BZ_Sort_RoomNum:
			{
				qsort( (void *) &m_BZ_RoomInfoList_Waiting[0], (size_t) m_BZ_RoomInfoList_Waiting.size(), sizeof( SRpScFightRoomList::_SFightRoom ), SortRoomNumList );
			}
			break;
		case n_BZ_Sort_RoomName:
			{
				qsort( (void *) &m_BZ_RoomInfoList_Waiting[0], (size_t) m_BZ_RoomInfoList_Waiting.size(), sizeof( SRpScFightRoomList::_SFightRoom ), SortRoomNameList );
			}
			break;
		case n_BZ_Sort_GameType:
			{
				qsort( (void *) &m_BZ_RoomInfoList_Waiting[0], (size_t) m_BZ_RoomInfoList_Waiting.size(), sizeof( SRpScFightRoomList::_SFightRoom ), SortGameTypeList );
			}
			break;
		case n_BZ_Sort_UserNum:
			{
				qsort( (void *) &m_BZ_RoomInfoList_Waiting[0], (size_t) m_BZ_RoomInfoList_Waiting.size(), sizeof( SRpScFightRoomList::_SFightRoom ), SortUserNumList );
			}
			break;
		case n_BZ_Sort_RoomState:
			{
				qsort( (void *) &m_BZ_RoomInfoList_Waiting[0], (size_t) m_BZ_RoomInfoList_Waiting.size(), sizeof( SRpScFightRoomList::_SFightRoom ), SortRoomStateList );
			}
			break;
		default:
			break;
		}
	}
	else
	{
		switch( m_BZ_Draw_SortType )
		{
		case n_BZ_Sort_RoomNum:
			{
				qsort( (void *) &m_BZ_RoomInfoList[0], (size_t) m_BZ_RoomInfoList.size(), sizeof( SRpScFightRoomList::_SFightRoom ), SortRoomNumList );
			}
			break;
		case n_BZ_Sort_RoomName:
			{
				qsort( (void *) &m_BZ_RoomInfoList[0], (size_t) m_BZ_RoomInfoList.size(), sizeof( SRpScFightRoomList::_SFightRoom ), SortRoomNameList );
			}
			break;
		case n_BZ_Sort_GameType:
			{
				qsort( (void *) &m_BZ_RoomInfoList[0], (size_t) m_BZ_RoomInfoList.size(), sizeof( SRpScFightRoomList::_SFightRoom ), SortGameTypeList );
			}
			break;
		case n_BZ_Sort_UserNum:
			{
				qsort( (void *) &m_BZ_RoomInfoList[0], (size_t) m_BZ_RoomInfoList.size(), sizeof( SRpScFightRoomList::_SFightRoom ), SortUserNumList );
			}
			break;
		case n_BZ_Sort_RoomState:
			{
				qsort( (void *) &m_BZ_RoomInfoList[0], (size_t) m_BZ_RoomInfoList.size(), sizeof( SRpScFightRoomList::_SFightRoom ), SortRoomStateList );
			}
			break;
		default:
			break;
		}
	}

	g_IsAscend_Sort = !( g_IsAscend_Sort );

	m_StartLineNum_RoomList = 0;

	SetRefreshRoomList();

	return;
}

void CBZ_RoomListWndProc::SetRefreshRoomList()
{	
/*
typedef struct SRpScFightRoomList::_SFightRoom
{
	DWORD					theRoomID;
	char					theName[40];
	WORD					theLock:1;			// 잠긴 방 여부 
	WORD					theGameType:6;		// 게임타입 
	WORD					theNum:4;			// 참가한 인원
	WORD					theMaxNum:4;		// 게임인원
	WORD					theStatus:1;		// 상태(준비, 게임)

} SBZ_RoomList;


typedef struct SBattleZoneRoomList
{
	CButton*				RoomListBtn;

	CTextBox*				RoomNumTxt;

	CImageBox*				PassWordImg;

	CTextBox*				RoomNameTxt;
	CTextBox*				GameTypeTxt;
	CTextBox*				CurtUserNumTxt;
	CTextBox*				RoomStateTxt;

} SBattleZoneRoomList;
*/
	
	int i = 0;
	char strTemp[128] = {0,};

	int room_list_num = 0;
	int room_index = 0;

	if( m_bIsOnlyWaitingRoom )
	{
		room_list_num = m_BZ_RoomInfoList_Waiting.size();
	}
	else
	{
		room_list_num = m_BZ_RoomInfoList.size();
	}

	for( i = 0; i < c_Max_Draw_BZRoomList_Num; ++i )
	{
		room_index = m_StartLineNum_RoomList + i;

		if( room_index < room_list_num )
		{
			if( m_bIsOnlyWaitingRoom )
			{
				// 방 리스트 버튼 동작 
				m_BZ_RoomList[i].RoomListBtn->SetBtnHold( false );

				// 방 번호 
				Rsprintf( RWCHAR(strTemp), _RT("%d"), m_BZ_RoomInfoList_Waiting[room_index].theRoomID );
				m_BZ_RoomList[i].RoomNumTxt->SetText( strTemp );

				if( m_BZ_RoomInfoList_Waiting[room_index].theLock )
				{
					// 비방 이미지 
					m_BZ_RoomList[i].PassWordImg->SetClientImage( TID_PASSWORD_ROOM_IMG );
				}
				else
				{
					m_BZ_RoomList[i].PassWordImg->SetClientImage( TID_None );
				}

				// 방 이름 
				m_BZ_RoomList[i].RoomNameTxt->SetText( m_BZ_RoomInfoList_Waiting[room_index].theName );

#ifdef JUST_WATCH
				//관람 여부
				if( m_BZ_RoomInfoList_Waiting[room_index].theWatchable )
				{
					m_BZ_RoomList[i].CanViewTxt->SetText("O");
				}
				else
				{
					m_BZ_RoomList[i].CanViewTxt->SetText("X");
				}
#endif				
				// 게임 타입 
				SetGameTypeText( m_BZ_RoomInfoList_Waiting[room_index].theGameType, strTemp );		
				m_BZ_RoomList[i].GameTypeTxt->SetText( strTemp );

				// 인 원
				Rsprintf( RWCHAR(strTemp), _RT("%d/%d"), m_BZ_RoomInfoList_Waiting[room_index].theNum, m_BZ_RoomInfoList_Waiting[room_index].theMaxNum );
				m_BZ_RoomList[i].CurtUserNumTxt->SetText( strTemp );

				// 상 태 
				SetRoomStateText( m_BZ_RoomInfoList_Waiting[room_index].theStatus, strTemp );		
				m_BZ_RoomList[i].RoomStateTxt->SetText( strTemp );
			}
			else
			{			
				// 방 리스트 버튼 동작 
				m_BZ_RoomList[i].RoomListBtn->SetBtnHold( false );

				// 방 번호 
				Rsprintf( RWCHAR(strTemp), _RT("%d"), m_BZ_RoomInfoList[room_index].theRoomID );
				m_BZ_RoomList[i].RoomNumTxt->SetText( strTemp );

				if( m_BZ_RoomInfoList[room_index].theLock )
				{
					// 비방 이미지 
					m_BZ_RoomList[i].PassWordImg->SetClientImage( TID_PASSWORD_ROOM_IMG );
				}
				else
				{
					m_BZ_RoomList[i].PassWordImg->SetClientImage( TID_None );
				}

				// 방 이름 
				m_BZ_RoomList[i].RoomNameTxt->SetText( m_BZ_RoomInfoList[room_index].theName );

#ifdef JUST_WATCH
				if( m_BZ_RoomInfoList[room_index].theWatchable )
				{
					m_BZ_RoomList[i].CanViewTxt->SetText("O");
				}
				else
				{
					m_BZ_RoomList[i].CanViewTxt->SetText("X");
				}
#endif
				// 게임 타입 
				SetGameTypeText( m_BZ_RoomInfoList[room_index].theGameType, strTemp );		
				m_BZ_RoomList[i].GameTypeTxt->SetText( strTemp );

				// 인 원
				Rsprintf( RWCHAR(strTemp), _RT("%d/%d") , m_BZ_RoomInfoList[room_index].theNum, m_BZ_RoomInfoList[room_index].theMaxNum );
				m_BZ_RoomList[i].CurtUserNumTxt->SetText( strTemp );

				// 상 태 
				SetRoomStateText( m_BZ_RoomInfoList[room_index].theStatus, strTemp );		
				m_BZ_RoomList[i].RoomStateTxt->SetText( strTemp );
			}
		}
		else		// 리스트에 없는 빈방들은 초기화!!
		{
			m_BZ_RoomList[i].RoomListBtn->SetBtnHold( true );
			m_BZ_RoomList[i].RoomNumTxt->SetText( _RT( "" ) );
			m_BZ_RoomList[i].PassWordImg->SetClientImage( TID_None );
			m_BZ_RoomList[i].RoomNameTxt->SetText( _RT( "" ) );
			m_BZ_RoomList[i].CanViewTxt->SetText( _RT( "" ) );
			m_BZ_RoomList[i].GameTypeTxt->SetText( _RT( "" ) );
			m_BZ_RoomList[i].CurtUserNumTxt->SetText( _RT( "" ) );
			m_BZ_RoomList[i].RoomStateTxt->SetText( _RT( "" ) );
		}
	}	
				
	return;
}

void CBZ_RoomListWndProc::SetGameTypeText( int aGameType, char* aGameTypeTxt )
{		
	switch( aGameType )
	{
	case n_DeathMatch:				// 데스 매치 
		{
			Rsprintf( RWCHAR(aGameTypeTxt), _RT("%s"), G_STRING_CHAR( IDS_WND_DEATH_MATCH ) );
		}
		break;
	case n_PointMatch:				// 보스 매치 
		{
			Rsprintf(RWCHAR(aGameTypeTxt), _RT("%s"), G_STRING_CHAR( IDS_WND_BOSS_MATCH ) );
		}
		break;
	case n_CapturetheFlag:				// 깃발 뺏기 
		{
			Rsprintf(RWCHAR(aGameTypeTxt), _RT("%s"), G_STRING_CHAR( IDS_WND_CATURE_THE_FLAG ) );
		}
		break;
	case n_CapturetheHill:				// 고지 점령 
		{
			Rsprintf(RWCHAR(aGameTypeTxt), _RT("%s"), G_STRING_CHAR( IDS_WND_CATURE_THE_GROUND ) );
		}
		break;
	case n_Survival:					// 서바이벌 
		{
			Rsprintf(RWCHAR(aGameTypeTxt), _RT("%s"), G_STRING_CHAR( IDS_WND_SURVIVAL ) );
		}
		break;		
	default:
		{
			Rsprintf(RWCHAR(aGameTypeTxt), _RT("%s"), G_STRING_CHAR( IDS_WND_DEATH_MATCH ) );
		}
		break;
	}

	return;
}


void CBZ_RoomListWndProc::SetRoomStateText( int aRoomState, char* aRoomStateTxt )
{		
	switch( aRoomState )
	{
	case 0:				// 대 기  
		{
			Rsprintf( RWCHAR(aRoomStateTxt), _RT("%s"), G_STRING_CHAR( IDS_WND_WAITING ) );
		}
		break;
	case 1:				// 게임중 
		{
			Rsprintf( RWCHAR(aRoomStateTxt), _RT("%s"), G_STRING_CHAR( IDS_WND_PLAYING ) );
		}
		break;
	default:
		{
			Rsprintf( RWCHAR(aRoomStateTxt), _RT("%s"), G_STRING_CHAR( IDS_WND_WAITING ) );
		}
		break;
	}

	return;
}

void CBZ_RoomListWndProc::SetRefreshChatMsg()
{
	int i = 0;
	int chat_msg_num = 0;
	int chat_index = 0;

	chat_msg_num = m_nInsertIndex;

	for( i = c_Max_Draw_BZChatMsg_Num - 1; i >= 0; --i )
	{
		chat_index = m_StartLineNum_ChatMsg + i;

		if( chat_index < chat_msg_num )
		{
			switch(m_BZ_ChatMsgList[chat_index].ChatType)
			{
				case n_BZ_Normal_Chat:
					{
						m_pChatMsgTxt[i]->SetTextColor(0, 0, 0);
					}
					break;
				case n_BZ_Whisper_Chat:
					{
						m_pChatMsgTxt[i]->SetTextColor(145, 0, 145);
					}
					break;
				case n_BZ_Help_Chat:
					{
						m_pChatMsgTxt[i]->SetTextColor(145, 125, 0);
					}
					break;
			}
			m_pChatMsgTxt[i]->SetText( m_BZ_ChatMsgList[chat_index].ChatMsg );
		}
		else		// 리스트에 없는 메세지들은 초기화!!
		{
			m_pChatMsgTxt[i]->SetText( _RT("") );			
		}
	}	

	return;
}

void CBZ_RoomListWndProc::SetRefreshUserList()
{
	int i = 0;
	int user_list_num = 0;
	int user_index = 0;

	user_list_num = m_BZ_UserNameList.size();

	for( i = 0; i < c_Max_Draw_BZUserList_Num; ++i )
	{
		user_index = m_StartLineNum_UserList + i;

		if( user_index < user_list_num )
		{
			m_pUserListBtn[i]->SetBtnHold( false );
			m_pUserListBtn[i]->SetText( m_BZ_UserNameList[user_index].UserName );
		}
		else		// 리스트에 없는 유저 버튼들은 초기화!!
		{
			m_pUserListBtn[i]->SetBtnHold( true );
			m_pUserListBtn[i]->SetText( _RT( "" ) );			
		}
	}	

	return;
}

//-----------------------------------------------------------------------------
void CBZ_RoomListWndProc::SetWheelValue( short _wheel )
{
	int value = 0;

	if( m_ScrollFocus == n_BZ_Focus_RoomList )
	{
		m_pRoomListScroll->GetValue( &value );

		if( ( _wheel < 0 ) && ( value < ( c_Max_BZRoomList_Num - c_Max_Draw_BZRoomList_Num ) ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}
		
		m_pRoomListScroll->SetValue( value );

		SetRoomListScroll( value );
	}
	else if( m_ScrollFocus == n_BZ_Focus_ChatWnd ) 	
	{
		m_pChatMsgScroll->GetValue( &value );

		if( ( _wheel < 0 ) && ( value < ( c_Max_BZChatMsg_Num - c_Max_Draw_BZChatMsg_Num ) ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}
		
		m_pChatMsgScroll->SetValue( value );

		SetChatMsgScroll( value );
	}
	else if( m_ScrollFocus == n_BZ_Focus_UserList ) 	
	{
		m_pUserListScroll->GetValue( &value );

		if( ( _wheel < 0 ) && ( value < ( c_Max_BZUserList_Num - c_Max_Draw_BZUserList_Num ) ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}
		
		m_pUserListScroll->SetValue( value );

		SetUserListScroll( value );
	}

	return;
}

void CBZ_RoomListWndProc::SetRoomListScroll( int aLineNum )
{	
	m_SelectRoomIndex = -1;	
	
	m_StartLineNum_RoomList = aLineNum * 1;
	
//	for( int i = 0; i < c_Max_Draw_BZRoomList_Num; ++i )
//	{
//	}

	SetRefreshRoomList();

	return;
}

void CBZ_RoomListWndProc::SetChatMsgScroll( int aLineNum )
{	
	m_StartLineNum_ChatMsg = aLineNum * 1;

//	CTextBox*				m_pChatMsgTxt[ c_Max_Draw_BZChatMsg_Num ];

//	char					m_pChatMsg[ c_Max_BZChatMsg_Num ];

//	for( int i = 0; i < c_Max_Draw_BZChatMsg_Num; ++i )
//	{
//		m_pChatMsgTxt[i]->SetText( &m_pChatMsg[ aLineNum + i ] );
//	}	

	SetRefreshChatMsg();

	return;
}


void CBZ_RoomListWndProc::SetUserListScroll( int aLineNum )
{	
	m_SelectUserIndex = -1;

	m_StartLineNum_UserList = aLineNum * 1;

	SetRefreshUserList();

	return;
}

/*
void CBZ_RoomListWndProc::InsertChatMsg( char* aChatMsg )
{

	m_StartLineNum_ChatMsg = m_BZ_UserNameList.size() - c_Max_Draw_BZChatMsg_Num;

	SetRefreshChatMsg();

	return;
}
*/


//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
///---------------------------------------------------------------------------
///-- Constructor
///---------------------------------------------------------------------------
CBZ_CreateRoomWndProc::CBZ_CreateRoomWndProc()
{
	Init();
}
///---------------------------------------------------------------------------
///-- Destructror
///---------------------------------------------------------------------------
CBZ_CreateRoomWndProc::~CBZ_CreateRoomWndProc()
{
}
///---------------------------------------------------------------------------
///-- Init
///---------------------------------------------------------------------------
void CBZ_CreateRoomWndProc::Init()
{
	m_GameMode = n_BattleZone_DeathMatch;
	m_MaxUserNum = n_BattleZone_Max2;
	m_MaxGameTime = n_BattleZone_Minute3;
	m_UseItem = n_BattleZone_UseItem_On;
	
#ifdef JUST_WATCH
	m_CanView = n_BattleZone_CanView_On;
#else
	m_CanView = n_BattleZone_CanView_Off;
#endif	
	
	m_GameModeValue[ n_BattleZone_DeathMatch ] = n_DeathMatch;
	m_GameModeValue[ n_BattleZone_BossMatch ] = n_PointMatch;
	m_GameModeValue[ n_BattleZone_CaptureTheFlag ] = n_CapturetheFlag;
	m_GameModeValue[ n_BattleZone_CaptureTheGround ] = n_CapturetheHill;
	m_GameModeValue[ n_BattleZone_Survival ] = n_Survival;
	
	m_MaxUserNumValue[ n_BattleZone_Max2 ] = 2;
	m_MaxUserNumValue[ n_BattleZone_Max4 ] = 4; 
	m_MaxUserNumValue[ n_BattleZone_Max6 ] = 6;
	m_MaxUserNumValue[ n_BattleZone_Max8 ] = 8;
	m_MaxUserNumValue[ n_BattleZone_Max12 ] = 12;

	m_MaxGameTimeValue[ n_BattleZone_Minute3 ] = 3;
	m_MaxGameTimeValue[ n_BattleZone_Minute5 ] = 5; 
	m_MaxGameTimeValue[ n_BattleZone_Minute7 ] = 7;
	m_MaxGameTimeValue[ n_BattleZone_Minute10 ] = 10;
	m_MaxGameTimeValue[ n_BattleZone_Minute15 ] = 15;

	m_UseItemValue[ n_BattleZone_UseItem_On ] = 1;
	m_UseItemValue[ n_BattleZone_UseItem_Off ] = 0; 

	m_CanViewValue[ n_BattleZone_CanView_On ] = 1;
	m_CanViewValue[ n_BattleZone_CanView_Off ] = 0;

	return;
}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CBZ_CreateRoomWndProc::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	CImageBox* img =NULL;


	SRect wnd_rect;
	wnd_rect.w	= 570;
	wnd_rect.h	= 410;
	wnd_rect.x	= 227; // center_x - ( wnd_rect.w / 2 );
	wnd_rect.y	= 179; // center_y - ( wnd_rect.h / 2 );

	char temp[100] = {0,};


	///-- Frame Window 
	m_pFrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_BZ_CREATEROOM_WND, WID_None, wnd_rect.x, wnd_rect.y, wnd_rect.w, wnd_rect.h, false );
	m_pFrameWnd->SetClientImage( TID_CREATEROOM_BG_LWND );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile3 );
 	m_pFrameWnd->SetWndTileSizeLR( 131, 100 );
//	m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_CRAETE_GAMEROOM ) );
//	m_pFrameWnd->SetTitle( true, 2, 30 );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();
	m_pFrameWnd->SetMove( false );
	
	//Logo Change
	if( g_RockClient.GetLanguage() == eLang_Korea )
	{		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_CREATEROOM_WND, 1, 2, 126, 38, false );
		img->SetClientImage( TID_pvp_sikongchanga );
	}
	else if( g_RockClient.GetLanguage() == eLang_Japan )
	{		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_CREATEROOM_WND, 2, 2, 122, 38, false );
		img->SetClientImage( TID_japan_pvp_logo2 );
	}
	else if( g_RockClient.GetLanguage() == eLang_China )
	{		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_CREATEROOM_WND, 1, 2, 126, 38, false );
		img->SetClientImage( TID_china_pvp_logo );
	}

	m_pRoomNameImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_CREATEROOM_WND, 11, 44, 546, 118, false );
	m_pRoomNameImg->SetClientImage( TID_ROOMNAME_IMG );

	m_pGameModeImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_CREATEROOM_WND, 10, 175, 270, 209, false );
	m_pGameModeImg->SetClientImage( TID_GAMEMODE_IMG );

	m_pOptionImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_CREATEROOM_WND, 287, 174, 270, 177, false );
	m_pOptionImg->SetClientImage( TID_OPTION_CREATEROOM_IMG );


	m_pCreateRoomTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 52, 56, 270, 22, false );	
	m_pCreateRoomTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pCreateRoomTxt->SetClientImage( TID_None );	
	m_pCreateRoomTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pCreateRoomTxt->SetTextColor( 255, 255, 255 );	
	m_pCreateRoomTxt->SetText( G_STRING_CHAR( IDS_WND_CRAETE_GAMEROOM ) );
	m_pCreateRoomTxt->SetFocusCheck( false );
	

	///-- Room Name Txt
	m_pRoomNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 27, 99, 76, 22, false );	
	m_pRoomNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pRoomNameTxt->SetClientImage( TID_None );	
	m_pRoomNameTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pRoomNameTxt->SetTextColor( 5, 5, 5 );	
	m_pRoomNameTxt->SetText( G_STRING_CHAR( IDS_WND_ROOMNAME ) );
	m_pRoomNameTxt->SetFocusCheck( false );

	///-- Room Name Input 
	m_pNameInputBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, WID_BZ_CREATEROOM_WND,  113, 100, 426, 20, false );
	m_pNameInputBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNameInputBox->SetTextSize( c_Max_RoomName_Length );
	m_pNameInputBox->SetClientImage( TID_None );
	m_pNameInputBox->SetClientColor( 5, 5, 5 );
	m_pNameInputBox->SetAlignText( n_atLeft, n_atCenter );
	m_pNameInputBox->SetBlank( 4, 0, 0, 0 );
	m_pNameInputBox->SetEditStyle( esText );
	m_pNameInputBox->SetTextColor( 255, 255, 255 );
	m_pNameInputBox->SetText( _RT("") );
	m_pNameInputBox->SetReturnActionClear( false );
	m_pNameInputBox->InitEditBox();
//	m_pNameInputBox->ReceiveEvent( n_emSetFocus, 0, 0, 0, 0 );

	///-- Password Txt
	m_pPasswordTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 27, 126, 76, 22, false );	
	m_pPasswordTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPasswordTxt->SetClientImage( TID_None );	
	m_pPasswordTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pPasswordTxt->SetTextColor( 5, 5, 5 );	
	m_pPasswordTxt->SetText( G_STRING_CHAR( IDS_WND_PASSWORD ) );
	m_pPasswordTxt->SetFocusCheck( false );

	///-- Password Input 
	m_pPasswordInputBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, WID_BZ_CREATEROOM_WND, 113, 128, 426, 20, false );
	m_pPasswordInputBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pPasswordInputBox->SetTextSize( c_Max_PassWord_Length );
	m_pPasswordInputBox->SetClientImage( TID_None );
	m_pPasswordInputBox->SetClientColor( 5, 5, 5 );
	m_pPasswordInputBox->SetAlignText( n_atLeft, n_atCenter );
	m_pPasswordInputBox->SetBlank( 4, 0, 0, 0 );
	m_pPasswordInputBox->SetEditStyle( esText );
	m_pPasswordInputBox->SetTextColor( 255, 255, 255 );
	m_pPasswordInputBox->SetText( _RT("") );
	m_pPasswordInputBox->SetReturnActionClear( false );
	m_pPasswordInputBox->InitEditBox();
	
	m_pGameModeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 52, 187, 116, 22, false );	
	m_pGameModeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pGameModeTxt->SetClientImage( TID_None );	
	m_pGameModeTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pGameModeTxt->SetTextColor( 255, 255, 255 );	
	m_pGameModeTxt->SetText( G_STRING_CHAR( IDS_WND_GAMEMODE ) );
	m_pGameModeTxt->SetFocusCheck( false );

//	m_pModeListBox = (CListBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 100, 4, 60, 20, false );	
//	m_pModeListBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
//	m_pModeListBox->SetScrollBarWidth( 16 );
//	m_pModeListBox->SetTextColor( 0, 0, 0 );
//	m_pModeListBox->SetLBoxBlank( 2, 2, 2, 2 );
//	m_pModeListBox->SetInsertOrder( ioLast );
//	m_pModeListBox->SetLineHeight( 14 );
//	m_pModeListBox->SetScrollBar();

//	m_pModeList1Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_CREATEROOM_WND, 398, 6, 13, 13, false );
//	m_pModeList1Btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
//	m_pModeList1Btn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
//	m_pModeList1Btn->SetAlignText( n_atCenter, n_atCenter );
//	m_pModeList1Btn->SetTextColor( 5, 5, 5 );
//	m_pModeList1Btn->SetText( _RT( "" ) );

//	m_pModeList2Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_CREATEROOM_WND, 398, 6, 13, 13, false );
//	m_pModeList2Btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
//	m_pModeList2Btn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
//	m_pModeList2Btn->SetAlignText( n_atCenter, n_atCenter );
//	m_pModeList2Btn->SetTextColor( 5, 5, 5 );
//	m_pModeList2Btn->SetText( _RT( "" ) );

//	m_pModeList3Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_CREATEROOM_WND, 398, 6, 13, 13, false );
//	m_pModeList3Btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
//	m_pModeList3Btn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
//	m_pModeList3Btn->SetAlignText( n_atCenter, n_atCenter );
//	m_pModeList3Btn->SetTextColor( 5, 5, 5 );
//	m_pModeList3Btn->SetText( _RT( "" ) );

//	m_pModeList4Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_CREATEROOM_WND, 398, 6, 13, 13, false );
//	m_pModeList4Btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
//	m_pModeList4Btn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
//	m_pModeList4Btn->SetAlignText( n_atCenter, n_atCenter );
//	m_pModeList4Btn->SetTextColor( 5, 5, 5 );
//	m_pModeList4Btn->SetText( _RT( "" ) );

//	m_pModeList5Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_CREATEROOM_WND, 398, 6, 13, 13, false );
//	m_pModeList5Btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
//	m_pModeList5Btn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
//	m_pModeList5Btn->SetAlignText( n_atCenter, n_atCenter );
//	m_pModeList5Btn->SetTextColor( 5, 5, 5 );
//	m_pModeList5Btn->SetText( _RT( "" ) );

//	m_pModeListScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, WID_BZ_CREATEROOM_WND, 484, 447, 13, 90, false ); 
//	m_pModeListScroll->SetClientImage( TID_None );
//	m_pModeListScroll->InitSlide( stVertSld, 0, c_Max_BZChatMsg_Num, 0, 11 );//Type, min, max, value, 썸의 폭
//	m_pModeListScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_None );	
//	m_pModeListScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
//	m_pModeListScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

//	m_pGameModeDescTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 100, 4, 60, 20, false );	
///	m_pGameModeDescTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
//	m_pGameModeDescTxt->SetClientImage( TID_None );	
//	m_pGameModeDescTxt->SetAlignText( n_atCenter, n_atCenter );
//	m_pGameModeDescTxt->SetTextColor( 5, 5, 5 );	
//	m_pGameModeDescTxt->SetText( G_STRING_CHAR( IDS_WND_MODEDESC ) );
//	m_pGameModeDescTxt->SetFocusCheck( false );

	m_pModeDescTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 32, 263, 216, 102, false );	
	m_pModeDescTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pModeDescTxt->SetClientImage( TID_None );	
	m_pModeDescTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pModeDescTxt->SetTextColor( 255, 255, 255 );	
	m_pModeDescTxt->SetText( _RT( "" ) );
	m_pModeDescTxt->SetFocusCheck( false );
	m_pModeDescTxt->SetMultiLine( true );

	m_pModeDescScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, WID_BZ_CREATEROOM_WND, 257, 254, 13, 118, false ); 
	m_pModeDescScroll->SetClientImage( TID_None );	
	m_pModeDescScroll->InitSlide( stVertSld, 0, 10, 0, 11 );//Type, min, max, value, 썸의 폭
	m_pModeDescScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_None );	
	m_pModeDescScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pModeDescScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	m_pModeListCBox = (CComboBox*)nRui->CreateWnd( n_wtComboBox, WID_None, WID_BZ_CREATEROOM_WND, 21, 226, 251, 100, false );	
	m_pModeListCBox->SetListboxTileSizeTB( 5, 5 );
	m_pModeListCBox->SetListboxTileSizeLR( 5, 5 );
	m_pModeListCBox->SetListboxImage9( TID_CTRL_OUTLINE_L1 );
//	m_pModeListCBox->AddItem( G_STRING_CHAR( IDS_WND_DEATH_MATCH ), n_BattleZone_DeathMatch );
//	m_pModeListCBox->AddItem( G_STRING_CHAR( IDS_WND_SURVIVAL ), n_BattleZone_Survival );
//	m_pModeListCBox->AddItem( G_STRING_CHAR( IDS_WND_CATURE_THE_GROUND ), n_BattleZone_CaptureTheGround );
//	m_pModeListCBox->AddItem( G_STRING_CHAR( IDS_WND_BOSS_MATCH ), n_BattleZone_BossMatch );
//	m_pModeListCBox->AddItem( G_STRING_CHAR( IDS_WND_CATURE_THE_FLAG ), n_BattleZone_CaptureTheFlag );

		
	m_pOtherOptionTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 330, 187, 90, 22, false );	
	m_pOtherOptionTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pOtherOptionTxt->SetClientImage( TID_None );	
	m_pOtherOptionTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pOtherOptionTxt->SetTextColor( 255, 255, 255 );	
	m_pOtherOptionTxt->SetText( G_STRING_CHAR( IDS_WND_OPTION ) );
	m_pOtherOptionTxt->SetFocusCheck( false );

	m_pMaxUserNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 308, 235, 51, 18, false );	
	m_pMaxUserNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMaxUserNumTxt->SetClientImage( TID_None );	
	m_pMaxUserNumTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pMaxUserNumTxt->SetTextColor( 5, 5, 5 );	
	m_pMaxUserNumTxt->SetText( G_STRING_CHAR( IDS_WND_USERNUM ) );
	m_pMaxUserNumTxt->SetFocusCheck( false );

	m_pMaxUserNumRBtn[0] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 363, 232, 35, 23, false );
	m_pMaxUserNumRBtn[0]->InitRadioButton( n_cmChecked, _RT( "2" ) );
	m_pMaxUserNumRBtn[0]->SetTextColor( 255, 255, 255 );	
	m_pMaxUserNumRBtn[0]->SetClientImage( TID_None );
	m_pMaxUserNumRBtn[0]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxUserNumRBtn[0]->SetDirectCheck( true );
	
	m_pMaxUserNumRBtn[1] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 399, 232, 35, 23, false );
	m_pMaxUserNumRBtn[1]->InitRadioButton( n_cmUnChecked, _RT( "4" ) );
	m_pMaxUserNumRBtn[1]->SetTextColor( 255, 255, 255 );	
	m_pMaxUserNumRBtn[1]->SetClientImage( TID_None );
	m_pMaxUserNumRBtn[1]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxUserNumRBtn[1]->SetDirectCheck( false );

	m_pMaxUserNumRBtn[2] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 435, 232, 35, 23, false );
	m_pMaxUserNumRBtn[2]->InitRadioButton( n_cmUnChecked, _RT( "6" ) );
	m_pMaxUserNumRBtn[2]->SetTextColor( 255, 255, 255 );	
	m_pMaxUserNumRBtn[2]->SetClientImage( TID_None );
	m_pMaxUserNumRBtn[2]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxUserNumRBtn[2]->SetDirectCheck( false );

	m_pMaxUserNumRBtn[3] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 471, 232, 35, 23, false );
	m_pMaxUserNumRBtn[3]->InitRadioButton( n_cmUnChecked, _RT( "8" ) );
	m_pMaxUserNumRBtn[3]->SetTextColor( 255, 255, 255 );	
	m_pMaxUserNumRBtn[3]->SetClientImage( TID_None );
	m_pMaxUserNumRBtn[3]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxUserNumRBtn[3]->SetDirectCheck( false );

	m_pMaxUserNumRBtn[4] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 507, 232, 35, 23, false );
	m_pMaxUserNumRBtn[4]->InitRadioButton( n_cmUnChecked, _RT( "12" ) );
	m_pMaxUserNumRBtn[4]->SetTextColor( 255, 255, 255 );	
	m_pMaxUserNumRBtn[4]->SetClientImage( TID_None );
	m_pMaxUserNumRBtn[4]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxUserNumRBtn[4]->SetDirectCheck( false );

	nRui->SetGroupWnd( 5, m_pMaxUserNumRBtn[0]->GetID(), m_pMaxUserNumRBtn[0]->GetID(),
														  m_pMaxUserNumRBtn[1]->GetID(), 
														  m_pMaxUserNumRBtn[2]->GetID(), 
														  m_pMaxUserNumRBtn[3]->GetID(), 
														  m_pMaxUserNumRBtn[4]->GetID() );

	m_pMaxTimeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 308, 263, 51, 18, false );	
	m_pMaxTimeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMaxTimeTxt->SetClientImage( TID_None );	
	m_pMaxTimeTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pMaxTimeTxt->SetTextColor( 5, 5, 5 );	
	m_pMaxTimeTxt->SetText( G_STRING_CHAR( IDS_WND_TIME ) );
	m_pMaxTimeTxt->SetFocusCheck( false );

	m_pMaxTimeRBtn[0] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 363, 259, 35, 23, false );
	m_pMaxTimeRBtn[0]->InitRadioButton( n_cmChecked, _RT( "3" ) );
	m_pMaxTimeRBtn[0]->SetTextColor( 255, 255, 255 );	
	m_pMaxTimeRBtn[0]->SetClientImage( TID_None );
	m_pMaxTimeRBtn[0]->SetBtnImage( TID_None, TID_None, TID_OPTION_UNLIMIT_BTN_FOCUS, TID_None );
	m_pMaxTimeRBtn[0]->SetDirectCheck( true );

	m_pMaxTimeRBtn[1] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 399, 259, 35, 23, false );
	m_pMaxTimeRBtn[1]->InitRadioButton( n_cmUnChecked, _RT( "5" ) );
	m_pMaxTimeRBtn[1]->SetTextColor( 255, 255, 255 );	
	m_pMaxTimeRBtn[1]->SetClientImage( TID_None );
	m_pMaxTimeRBtn[1]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxTimeRBtn[1]->SetDirectCheck( false );

	m_pMaxTimeRBtn[2] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 435, 259, 35, 23, false );
	m_pMaxTimeRBtn[2]->InitRadioButton( n_cmUnChecked, _RT( "7" ) );
	m_pMaxTimeRBtn[2]->SetTextColor( 255, 255, 255 );	
	m_pMaxTimeRBtn[2]->SetClientImage( TID_None );
	m_pMaxTimeRBtn[2]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxTimeRBtn[2]->SetDirectCheck( false );

	m_pMaxTimeRBtn[3] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 471, 259, 35, 23, false );
	m_pMaxTimeRBtn[3]->InitRadioButton( n_cmUnChecked, _RT( "10" ) );
	m_pMaxTimeRBtn[3]->SetTextColor( 255, 255, 255 );	
	m_pMaxTimeRBtn[3]->SetClientImage( TID_None );
	m_pMaxTimeRBtn[3]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxTimeRBtn[3]->SetDirectCheck( false );

	m_pMaxTimeRBtn[4] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 507, 259, 35, 23, false );
	m_pMaxTimeRBtn[4]->InitRadioButton( n_cmUnChecked, _RT( "15" ) );
	m_pMaxTimeRBtn[4]->SetTextColor( 255, 255, 255 );	
	m_pMaxTimeRBtn[4]->SetClientImage( TID_None );
	m_pMaxTimeRBtn[4]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxTimeRBtn[4]->SetDirectCheck( false );

	nRui->SetGroupWnd( 5, m_pMaxTimeRBtn[0]->GetID(), m_pMaxTimeRBtn[0]->GetID(),
													  m_pMaxTimeRBtn[1]->GetID(), 
													  m_pMaxTimeRBtn[2]->GetID(), 
													  m_pMaxTimeRBtn[3]->GetID(), 
													  m_pMaxTimeRBtn[4]->GetID() );

	m_pUseItemTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 308, 291, 51, 18, false );	
	m_pUseItemTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pUseItemTxt->SetClientImage( TID_None );	
	m_pUseItemTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pUseItemTxt->SetTextColor( 5, 5, 5 );	
	m_pUseItemTxt->SetText( G_STRING_CHAR( IDS_ITEM ) );
	m_pUseItemTxt->SetFocusCheck( false );

	m_pUseItemRBtn[0] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 363, 288, 89, 23, false );
	m_pUseItemRBtn[0]->InitRadioButton( n_cmChecked, G_STRING_CHAR( IDS_WND_ON ) );
	m_pUseItemRBtn[0]->SetTextColor( 255, 255, 255 );	
	m_pUseItemRBtn[0]->SetClientImage( TID_None );
	m_pUseItemRBtn[0]->SetBtnImage( TID_None, TID_None, TID_OPTION_LONG_RBTN_FOCUS, TID_None );
	m_pUseItemRBtn[0]->SetDirectCheck( true );

	m_pUseItemRBtn[1] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 453, 288, 89, 23, false );
	m_pUseItemRBtn[1]->InitRadioButton( n_cmUnChecked, G_STRING_CHAR( IDS_WND_OFF ) );
	m_pUseItemRBtn[1]->SetTextColor( 255, 255, 255 );	
	m_pUseItemRBtn[1]->SetClientImage( TID_None );
	m_pUseItemRBtn[1]->SetBtnImage( TID_None, TID_None, TID_OPTION_LONG_RBTN_FOCUS, TID_None );
	m_pUseItemRBtn[1]->SetDirectCheck( false );

	nRui->SetGroupWnd( 2, m_pUseItemRBtn[0]->GetID(), m_pUseItemRBtn[0]->GetID(),
													  m_pUseItemRBtn[1]->GetID() );


	m_pCanViewTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_CREATEROOM_WND, 308, 319, 51, 18, false );	
	m_pCanViewTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pCanViewTxt->SetClientImage( TID_None );	
	m_pCanViewTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pCanViewTxt->SetTextColor( 5, 5, 5 );	
	m_pCanViewTxt->SetText( G_STRING_CHAR( IDS_VIEW ) );					
	m_pCanViewTxt->SetFocusCheck( false );

	m_pCanViewRBtn[0] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 363, 317, 89, 23, false );
	m_pCanViewRBtn[0]->InitRadioButton( n_cmChecked, G_STRING_CHAR( IDS_WND_ON ) );
	m_pCanViewRBtn[0]->SetTextColor( 255, 255, 255 );	
	m_pCanViewRBtn[0]->SetClientImage( TID_None );
	m_pCanViewRBtn[0]->SetBtnImage( TID_None, TID_None, TID_OPTION_LONG_RBTN_FOCUS, TID_None );
	m_pCanViewRBtn[0]->SetDirectCheck( true );

	m_pCanViewRBtn[1] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, 453, 317, 89, 23, false );
	m_pCanViewRBtn[1]->InitRadioButton( n_cmUnChecked, G_STRING_CHAR( IDS_WND_OFF ) );
	m_pCanViewRBtn[1]->SetTextColor( 255, 255, 255 );	
	m_pCanViewRBtn[1]->SetClientImage( TID_None );
	m_pCanViewRBtn[1]->SetBtnImage( TID_None, TID_None, TID_OPTION_LONG_RBTN_FOCUS, TID_None );
	m_pCanViewRBtn[1]->SetDirectCheck( false );

#ifdef JUST_WATCH	
	nRui->SetGroupWnd( 2, m_pCanViewRBtn[0]->GetID(), m_pCanViewRBtn[0]->GetID(),
													  m_pCanViewRBtn[1]->GetID() );
#else
	nRui->SetGroupWnd( 2, m_pCanViewRBtn[1]->GetID(), m_pCanViewRBtn[0]->GetID(),
													  m_pCanViewRBtn[1]->GetID() );
#endif

	m_pOKBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_CREATEROOM_WND, 345, 354, 100, 25, false );
	m_pOKBtn->SetBtnImage( TID_CREATEROOM_OK_CANCEL_BTN_NORMAL, TID_CREATEROOM_OK_CANCEL_BTN_FOCUS, TID_CREATEROOM_OK_CANCEL_BTN_SELECT, TID_CREATEROOM_OK_CANCEL_BTN_DISABLE );	
	m_pOKBtn->SetFontR( n_fRsvFontWndButton );   
	m_pOKBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
	m_pOKBtn->SetTextColor( 255, 255, 255 );

	m_pCancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_CREATEROOM_WND, 449, 354, 100, 25, false );
	m_pCancelBtn->SetBtnImage( TID_CREATEROOM_OK_CANCEL_BTN_NORMAL, TID_CREATEROOM_OK_CANCEL_BTN_FOCUS, TID_CREATEROOM_OK_CANCEL_BTN_SELECT, TID_CREATEROOM_OK_CANCEL_BTN_DISABLE );
	m_pCancelBtn->SetFontR( n_fRsvFontWndButton );   
	m_pCancelBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );
	m_pCancelBtn->SetTextColor( 255, 255, 255 );

	return;
}

///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CBZ_CreateRoomWndProc::Update()
{
	return;
}

///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CBZ_CreateRoomWndProc::Proc( SEventMessage* EMsg )
{	
	//=========================================================================================
	if( EMsg->FocusWnd == m_pNameInputBox->GetID() )
	{
		if( EMsg->Notify == NM_EDT_RETURN_ACTION ) 
		{			
			nRui->SetEnterFlag( n_nfEditEnter );
			nRui->SetEnterWndID( m_pNameInputBox->GetID() );

			nRui->SendEvent( m_pPasswordInputBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );
			nRui->SendEvent( m_pPasswordInputBox->GetID(), n_emClearText, 0, 0, 0, 0 );
		}
	}
	else if( EMsg->FocusWnd == m_pPasswordInputBox->GetID() )
	{
		if( EMsg->Notify == NM_EDT_RETURN_ACTION ) 
		{			
			nRui->SetEnterFlag( n_nfEditEnter );
			nRui->SetEnterWndID( m_pPasswordInputBox->GetID() );

//			nRui->SendEvent( m_pPasswordInputBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );
//			nRui->SendEvent( m_pPasswordInputBox->GetID(), n_emClearText, 0, 0, 0, 0 );
		}
	}
	else if( EMsg->FocusWnd == m_pModeListCBox->GetID() )
	{
		if( EMsg->Notify == NM_CB_TEXT_EXCHANGE )	
		{
//			m_pModeListCBox->GetCurSelIdx();			///-- 선택된 Text의 순차적인 Index
//			m_pModeListCBox->GetCurSelValue();			///-- 선택된 Text의 Value( AddItem 의 두번째 인자로 지정된 값 )

			m_GameMode = (NBattleZoneGameMode) m_pModeListCBox->GetCurSelValue();
			m_pModeDescTxt->SetText( g_Pc_Manager.theGameModeDesc[m_GameMode] );
			m_pModeDescTxt->SetDrawTextTime( 25 );
		}
	}
	//=========================================================================================
	else if( EMsg->FocusWnd == m_pModeDescScroll->GetID() )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || 
			( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || 
			( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetModeDescScroll( (int)EMsg->fParam );			
		}
	}
	//=========================================================================================
	else if( EMsg->FocusWnd == m_pMaxUserNumRBtn[n_BattleZone_Max2]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pMaxUserNumRBtn[n_BattleZone_Max2]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_MaxUserNum = n_BattleZone_Max2;
		}
	}
	else if( EMsg->FocusWnd == m_pMaxUserNumRBtn[n_BattleZone_Max4]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pMaxUserNumRBtn[n_BattleZone_Max4]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_MaxUserNum = n_BattleZone_Max4;
		}
	}
	else if( EMsg->FocusWnd == m_pMaxUserNumRBtn[n_BattleZone_Max6]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pMaxUserNumRBtn[n_BattleZone_Max6]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_MaxUserNum = n_BattleZone_Max6;
		}
	}
	else if( EMsg->FocusWnd == m_pMaxUserNumRBtn[n_BattleZone_Max8]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pMaxUserNumRBtn[n_BattleZone_Max8]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_MaxUserNum = n_BattleZone_Max8;
		}
	}
	else if( EMsg->FocusWnd == m_pMaxUserNumRBtn[n_BattleZone_Max12]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pMaxUserNumRBtn[n_BattleZone_Max12]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_MaxUserNum = n_BattleZone_Max12;
		}
	}
	else if( EMsg->FocusWnd == m_pMaxTimeRBtn[n_BattleZone_Minute3]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pMaxTimeRBtn[n_BattleZone_Minute3]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_MaxGameTime = n_BattleZone_Minute3;
		}
	}
	else if( EMsg->FocusWnd == m_pMaxTimeRBtn[n_BattleZone_Minute5]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pMaxTimeRBtn[n_BattleZone_Minute5]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_MaxGameTime = n_BattleZone_Minute5;
		}
	}
	else if( EMsg->FocusWnd == m_pMaxTimeRBtn[n_BattleZone_Minute7]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pMaxTimeRBtn[n_BattleZone_Minute7]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_MaxGameTime = n_BattleZone_Minute7;
		}
	}
	else if( EMsg->FocusWnd == m_pMaxTimeRBtn[n_BattleZone_Minute10]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pMaxTimeRBtn[n_BattleZone_Minute10]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_MaxGameTime = n_BattleZone_Minute10;
		}
	}
	else if( EMsg->FocusWnd == m_pMaxTimeRBtn[n_BattleZone_Minute15]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pMaxTimeRBtn[n_BattleZone_Minute15]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_MaxGameTime = n_BattleZone_Minute15;
		}
	}
	else if( EMsg->FocusWnd == m_pUseItemRBtn[n_BattleZone_UseItem_On]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pUseItemRBtn[n_BattleZone_UseItem_On]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_UseItem = n_BattleZone_UseItem_On;
		}
	}	
	else if( EMsg->FocusWnd == m_pUseItemRBtn[n_BattleZone_UseItem_Off]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pUseItemRBtn[n_BattleZone_UseItem_Off]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_UseItem = n_BattleZone_UseItem_Off;
		}
	}
	else if( EMsg->FocusWnd == m_pCanViewRBtn[n_BattleZone_CanView_On]->GetID() )
	{
#ifdef JUST_WATCH	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pCanViewRBtn[n_BattleZone_CanView_On]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_CanView = n_BattleZone_CanView_On;
		}
#endif
	}
	else if( EMsg->FocusWnd == m_pCanViewRBtn[n_BattleZone_CanView_Off]->GetID() )
	{
#ifdef JUST_WATCH
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pCanViewRBtn[n_BattleZone_CanView_Off]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			m_CanView = n_BattleZone_CanView_Off;
		}
#endif
	}
	//=========================================================================================
	else if( EMsg->FocusWnd == m_pOKBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pNameInputBox->GetID(), n_emGetText, (EPARAM) m_RoomName, c_Max_RoomName_Length, 0, 0 );
			nRui->SendEvent( m_pPasswordInputBox->GetID(), n_emGetText, (EPARAM) m_PassWord, c_Max_PassWord_Length, 0, 0 );

			if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_RoomName ) )
			{
				memset( m_RoomName , 0 , sizeof(TCHAR) * (c_Max_RoomName_Length) );
				Rsprintf( RWCHAR(m_RoomName) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
			}				
			
			_SFightRoomInfo		room_info;

			Rsprintf( RWCHAR(room_info.theName), _RT("%s"), RWCHAR(m_RoomName) );
//			sprintf( g_BZ_WaitingRoomWnd.m_RoomInfo.theName, "%s", m_RoomName );

			if( Rstrlen( RWCHAR(m_PassWord) ) > 0 )
			{
				Rsprintf( RWCHAR(room_info.thePassword), _RT("%s"), RWCHAR(m_PassWord ));
				room_info.theLockFlag = 1;		
			}
			else
			{
				Rsprintf( RWCHAR(room_info.thePassword), _RT("") );
				room_info.theLockFlag = 0;
			}

			// 다른 모드가 준비 되지 않았으므로 일단은 데쓰매치만 되도록 생성 
//			m_GameMode = n_BattleZone_DeathMatch; 

			room_info.theGameType = m_GameModeValue[ m_GameMode ];			
			room_info.theMemberNum = m_MaxUserNumValue[ m_MaxUserNum ] ;
			room_info.theTimeOut = m_MaxGameTimeValue[ m_MaxGameTime ];
			room_info.theItemFlag = m_UseItemValue[ m_UseItem ];

#ifdef JUST_WATCH
			room_info.theWatchable = m_CanViewValue[ m_CanView ];
#endif
			room_info.theMapCode = g_BZ_WaitingRoomWnd.m_FightMapInfo[ room_info.theGameType ].MapIdx[ 0 ];
//			room_info.theMapCode = c_RockMap_Millena_Rain_PVP_Zone;		// 맨 처음 방 생성시에는 초기값 그대로 전송 
			
			g_RockClient.Send_CsMakeFightRoom( room_info );

			g_BZ_WaitingRoomWnd.m_RoomInfo = room_info;
		}
	}
	else if( EMsg->FocusWnd == m_pCancelBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_BZ_CreateRoomWnd.Close();			
			nRui->SetModal( WID_None );
			
			g_BZ_SelectZoneWnd.m_BZ_WndType = n_BattleZone_RoomListWnd;

			nRui->SendEvent( m_pNameInputBox->GetID(), n_emClearText, 0, 0, 0, 0 );
			nRui->SendEvent( m_pPasswordInputBox->GetID(), n_emClearText, 0, 0, 0, 0 );

			g_BZ_RoomListWnd.m_pDisableWndImg->SetVisible( false );
			g_BZ_RoomListWnd.SetFocusWndColor( true );
			
			nRui->SendEvent( g_BZ_RoomListWnd.GetChatInputBox()->GetID() , n_emSetFocus, 0, 0, 0, 0 );
		}
	}
	//=========================================================================================

	return;
}

void CBZ_CreateRoomWndProc::Open()
{
	char temp[100] = {0.};

	nRui->SendEvent( WID_BZ_CREATEROOM_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				

	nRui->SendEvent( m_pPasswordInputBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( m_pPasswordInputBox->GetID(), n_emClearText, 0, 0, 0, 0 );
	nRui->SendEvent( m_pNameInputBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );
	nRui->SendEvent( m_pNameInputBox->GetID(), n_emClearText, 0, 0, 0, 0 );
	
	
//	Rsprintf(RWCHAR(temp), "%s ( %s %s )", G_STRING_CHAR( IDS_WND_SELECTGRADE_BASICLEVEL ),		// 초   급 ( 레벨 00 ~ 00 ) 
//					 				 G_STRING_CHAR( IDS_CHARSTATE_LEVEL ), 
//									 g_BZ_SelectGradeWnd.m_BZ_GradeRange[ g_BZ_SelectGradeWnd.m_BZ_GradeType ] );
//	m_pGradeTypeTxt->SetText( temp );

	m_GameMode = n_BattleZone_DeathMatch;

	m_pModeListCBox->ClearItem();

	if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Free )
	{
		m_pModeListCBox->AddItem( G_STRING_CHAR( IDS_WND_DEATH_MATCH ), n_BattleZone_DeathMatch );
		m_pModeListCBox->AddItem( G_STRING_CHAR( IDS_WND_SURVIVAL ), n_BattleZone_Survival );
		m_pModeListCBox->AddItem( G_STRING_CHAR( IDS_WND_CATURE_THE_GROUND ), n_BattleZone_CaptureTheGround );
	}
	else
	{
		m_pModeListCBox->AddItem( G_STRING_CHAR( IDS_WND_DEATH_MATCH ), n_BattleZone_DeathMatch );		
		m_pModeListCBox->AddItem( G_STRING_CHAR( IDS_WND_CATURE_THE_GROUND ), n_BattleZone_CaptureTheGround );
	}

	m_pModeListCBox->SetSelectText( G_STRING_CHAR( IDS_WND_DEATH_MATCH ) );

	m_pModeDescTxt->SetText( g_Pc_Manager.theGameModeDesc[n_BattleZone_DeathMatch] );

	return;
}


void CBZ_CreateRoomWndProc::Close()
{
	nRui->SendEvent( WID_BZ_CREATEROOM_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );				

	
	nRui->SendEvent( m_pNameInputBox->GetID(), n_emEditDisable, 0, 0, 0, 0 );	

	nRui->SendEvent( m_pPasswordInputBox->GetID(), n_emEditDisable, 0, 0, 0, 0 );	

	return;
}


void CBZ_CreateRoomWndProc::SetModeDescScroll( int aLineNum )
{	
	m_pModeDescTxt->SetStartLine( aLineNum );
	
	return;
}


//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
///---------------------------------------------------------------------------
///-- Constructor
///---------------------------------------------------------------------------
CBZ_WaitingRoomWndProc::CBZ_WaitingRoomWndProc()
{
	Init();
}
///---------------------------------------------------------------------------
///-- Destructror
///---------------------------------------------------------------------------
CBZ_WaitingRoomWndProc::~CBZ_WaitingRoomWndProc()
{
}
///---------------------------------------------------------------------------
///-- Init
///---------------------------------------------------------------------------
void CBZ_WaitingRoomWndProc::Init()
{
	m_MaxGameTime = n_BattleZone_Minute3;
	m_UseItem = n_BattleZone_UseItem_On;
	m_ZoneViewType = n_BattleZone_NormalZone;

	m_WaitingRoomID = 0;
	m_MasterID = 0;		

//	m_SelfSlotIdx = 0;

	m_bIsReady = FALSE;
	m_bIsMaster = FALSE;

	m_SelectUserIndex = -1;

	int i = 0;

	for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{
		m_WaitingUserInfo[ i ].FighterTeam = n_BattleZone_TeamFlag_Red;
		m_WaitingUserInfo[ i ].FighterState = n_BZ_State_None;
	}

	m_StartLineNum_ChatMsg = 0;

	ChatMsgInit();

	m_lPrevTime = 0;
	
	return;
}

//by simwoosung
void CBZ_WaitingRoomWndProc::ChatMsgInit()
{
	m_uChatType = n_BZ_Normal_Chat;
	
	SAFE_STR_CPY( m_strChatstring, _RT("") , c_Max_ChatMsg_Length * 2 );
	SAFE_STR_CPY( m_strTempBuf1, _RT("") , c_Max_ChatMsg_Length * 2 );
	SAFE_STR_CPY( m_strTempBuf2, _RT("") , c_Max_ChatMsg_Length * 2 );
	SAFE_STR_CPY( m_strWhisDest, _RT("") , 256 );
	
	memset( m_BZ_ChatMsgList, 0, sizeof(SBZ_ChatMsgList) * (c_Max_BZChatMsg_Num + 1));
	
	m_nInsertIndex = 0;	
}

//by simwoosung
void  CBZ_WaitingRoomWndProc::SetChatType(UINT uChatType)
{
	if( !(uChatType == n_BZ_Normal_Chat || uChatType == n_BZ_Whisper_Chat) )
	{
		return;
	}

	if( m_uChatType != uChatType )
	{
		return;
	}

	m_uChatType = uChatType;
}

//by simwoosung
void CBZ_WaitingRoomWndProc::SetWhisDest(void * pstrDest)
{
	char* strDest = (char*)(pstrDest);
	SAFE_STR_CPY(m_strWhisDest, strDest, 256);
	Rsprintf( RWCHAR(m_strChatstring) , _RT("/w %s ") , RWCHAR(m_strWhisDest));				
	m_pChatInputBox->SetText(m_strChatstring);
	PostMessage( m_pChatInputBox->GetHEdit(), WM_USER + 44, NULL, NULL );
	nRui->SendEvent( m_pWhisperBtn->GetID(), n_emSetFocus, 0, 0, 0, 0 );
}

///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CBZ_WaitingRoomWndProc::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	CImageBox* img =NULL;

	SRect wnd_rect;
	wnd_rect.w	= 1024;
	wnd_rect.h	= 768;
	wnd_rect.x	= 0; // center_x - ( wnd_rect.w / 2 );
	wnd_rect.y	= 0; // center_y - ( wnd_rect.h / 2 );

	char temp[100] = {0,};

	///-- Frame Window 
	m_pFrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_BZ_WAITINGROOM_WND, WID_None, wnd_rect.x, wnd_rect.y, wnd_rect.w, wnd_rect.h, false );
	m_pFrameWnd->SetClientImage( TID_PVP_WND_BG );
	m_pFrameWnd->SetCaption( _RT( "" ) );
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetTitle( false );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();
	m_pFrameWnd->SetMove( false );
		
	//Logo Change
	if( g_RockClient.GetLanguage() == eLang_Korea )
	{		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 122, 109, 126, 35, false );
		img->SetClientImage( TID_pvp_sikongchanga );
	}
	else if( g_RockClient.GetLanguage() == eLang_Japan )
	{
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 122, 109, 122, 35, false );
		img->SetClientImage( TID_japan_pvp_logo );	
	}
	else if( g_RockClient.GetLanguage() == eLang_China )
	{		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 122, 109, 126, 35, false );
		img->SetClientImage( TID_china_pvp_logo );

		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 784, 109, 116, 35, false );
		img->SetClientImage( TID_china_pvp );
	}

	m_pCharListImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 127, 150, 562, 277, false );
	m_pCharListImg->SetClientImage( TID_WAITINGCHARLIST_IMG );

	m_pMapSelectImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 701, 150, 194, 277, false );
	m_pMapSelectImg->SetClientImage( TID_MAPSELECT_IMG );

	m_pChatWndImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 128, 446, 490, 178, false );
	m_pChatWndImg->SetClientImage( TID_CHATWND_IMG );

	m_pOptionImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND,  625, 446, 270, 123, false );
	m_pOptionImg->SetClientImage( TID_OPTION_WAITINGROOM_IMG );

	m_pRoomNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, 145, 161, 529, 17, false );	
	m_pRoomNameTxt->SetFontg( n_ftGulimChe, 13, n_fwNormal, n_fdNormal );
	m_pRoomNameTxt->SetClientImage( TID_None );	
	m_pRoomNameTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pRoomNameTxt->SetTextColor( 255, 255, 255 );		
	m_pRoomNameTxt->SetText( _RT( "" ) );
	m_pRoomNameTxt->SetFocusCheck( false );

	m_pTeam1Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND,  583, 210, 100, 63, false  );
	m_pTeam1Btn->SetBtnImage( TID_GAMEREADY_TEAM_BTN_NORMAL, TID_GAMEREADY_TEAM_BTN_FOCUS, TID_GAMEREADY_TEAM_BTN_SELECT, TID_None );
	m_pTeam1Btn->SetFontg( n_ftGulimChe, 18, n_fwBold, n_fdNormal );
	m_pTeam1Btn->SetClientImage( TID_None );	
	m_pTeam1Btn->SetAlignText( n_atCenter, n_atCenter );
	m_pTeam1Btn->SetTextColor( 255, 255, 255 );		
	m_pTeam1Btn->SetBlank( 20, 0, 0, 0 );
	Rsprintf(RWCHAR(temp), _RT("1%s"), G_STRING_CHAR( IDS_WND_TEAM ) ); 
	m_pTeam1Btn->SetText( temp );

	m_pTeam2Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, 583, 276, 100, 63, false );
	m_pTeam2Btn->SetBtnImage( TID_GAMEREADY_TEAM_BTN_NORMAL, TID_GAMEREADY_TEAM_BTN_FOCUS, TID_GAMEREADY_TEAM_BTN_SELECT, TID_None );
	m_pTeam2Btn->SetFontg( n_ftGulimChe, 18, n_fwBold, n_fdNormal );
	m_pTeam2Btn->SetClientImage( TID_None );	
	m_pTeam2Btn->SetAlignText( n_atCenter, n_atCenter );
	m_pTeam2Btn->SetTextColor( 255, 255, 255 );		
	m_pTeam2Btn->SetBlank( 20, 0, 0, 0 );
	Rsprintf(RWCHAR(temp), _RT("2%s"), G_STRING_CHAR( IDS_WND_TEAM ) ); 
	m_pTeam2Btn->SetText( temp );
	
	m_pReadyORStartBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, 583, 342, 100, 63, false );
	m_pReadyORStartBtn->SetBtnImage( TID_GAMEREADY_START_BTN_NORMAL, TID_GAMEREADY_START_BTN_FOCUS, TID_GAMEREADY_START_BTN_SELECT, TID_None );
	m_pReadyORStartBtn->SetFontg( n_ftGulimChe, 18, n_fwBold, n_fdNormal );
	m_pReadyORStartBtn->SetClientImage( TID_None );	
	m_pReadyORStartBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pReadyORStartBtn->SetTextColor( 255, 255, 255 );		
//	m_pReadyORStartBtn->SetText( G_STRING_CHAR( IDS_WND_READY ) );
	m_pReadyORStartBtn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) );

	
	///-- Team1 Img 
	m_pTeam1Img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 591, 219, 36, 46, false );
	m_pTeam1Img->SetClientImage( TID_NATIONFLAG_MILLENA_IMG );
	m_pTeam1Img->SetFocusCheck( false );

	///-- Team2 Img 
	m_pTeam2Img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 591, 285, 36, 46, false );
	m_pTeam2Img->SetClientImage( TID_NATIONFLAG_RAIN_IMG );
	m_pTeam2Img->SetFocusCheck( false );
	
	
	int i = 0;	
	int x_pos_slot = 0;
	int y_pos_slot = 0;
	int x_pos_nation = 0;
	int y_pos_nation = 0;
	int x_pos_charimg = 0;
	int y_pos_charimg = 0;
	int x_pos_charname = 0;
	int y_pos_charname = 0;
	int x_pos_teamflag_red = 0;
	int y_pos_teamflag_red = 0;
	int x_pos_teamflag_green = 0;
	int y_pos_teamflag_green = 0;
	int x_pos_ready = 0;
	int y_pos_ready = 0;
	int x_pos_kick = 0;
	int y_pos_kick = 0;
	int y_pos_add = 0;
	
	for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{	
		y_pos_add = ( i / 2 ) * 37;

		y_pos_slot = 192 + y_pos_add;
		y_pos_nation = 193 + y_pos_add;
		y_pos_charimg = 194 + y_pos_add;
		y_pos_charname = 195 + y_pos_add;
		y_pos_teamflag_red = 214 + y_pos_add;
		y_pos_teamflag_green = 214 + y_pos_add;	
		y_pos_ready = 215 + y_pos_add;		
		y_pos_kick = 192 + y_pos_add;		

		if( ( i % 2 ) == 0 )		// 짝수일 경우 처리 
		{
			x_pos_slot = 138;
			x_pos_nation = 138;					
			x_pos_charimg = 161;			
			x_pos_charname = 196;			
			x_pos_teamflag_red = 194;
			x_pos_teamflag_green = 242;
			x_pos_ready = 302;
			x_pos_kick = 341;
		}
		else
		{
			x_pos_slot = 358;
			x_pos_nation = 358;			
			x_pos_charimg = 381;
			x_pos_charname = 416;
			x_pos_teamflag_red = 414;
			x_pos_teamflag_green = 462;
			x_pos_ready = 521;
			x_pos_kick = 561;
		}
				
		m_BZ_UserList[i].SlotBGImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, x_pos_slot, y_pos_slot, 219, 36, false );
		m_BZ_UserList[i].SlotBGImg->SetClientImage( TID_None ); // TID_GAMEREADY_CHARSLOT_IMG );		
			
		m_BZ_UserList[i].NationImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, x_pos_nation, y_pos_nation, 21, 34, false );
		m_BZ_UserList[i].NationImg->SetClientImage( TID_None ); // TID_NATIONFLAG_MILLENA_IMG );
		
		m_BZ_UserList[i].CharImgBtn = (CCharImgButton*)nRui->CreateWnd( n_wtChaImgButton, WID_None, WID_BZ_WAITINGROOM_WND, x_pos_charimg, y_pos_charimg, 32, 32, false );
		m_BZ_UserList[i].CharImgBtn->SetClientImage( TID_None );
		m_BZ_UserList[i].CharImgBtn->SetBtnImage( TID_None, TID_PARTY_MEMBER_CHAR_SELECT, TID_PARTY_MEMBER_CHAR_SELECT, TID_None );
		
		m_BZ_UserList[i].CharNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, x_pos_charname, y_pos_charname, 157, 17, false );	
		m_BZ_UserList[i].CharNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_BZ_UserList[i].CharNameTxt->SetClientImage( TID_None );	
		m_BZ_UserList[i].CharNameTxt->SetAlignText( n_atLeft, n_atCenter );
		m_BZ_UserList[i].CharNameTxt->SetTextColor( 255, 255, 255 );		
		m_BZ_UserList[i].CharNameTxt->SetText( _RT( "" ) );
		m_BZ_UserList[i].CharNameTxt->SetFocusCheck( false );
		
		m_BZ_UserList[i].TeamFlagRedImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, x_pos_teamflag_red, y_pos_teamflag_red, 47, 12, false );
		m_BZ_UserList[i].TeamFlagRedImg->SetClientImage( TID_None ); // TID_TEAMSTATE_RED_IMG );		

		m_BZ_UserList[i].TeamFlagGreenImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, x_pos_teamflag_green, y_pos_teamflag_green, 47, 12, false );
		m_BZ_UserList[i].TeamFlagGreenImg->SetClientImage( TID_None ); // TID_TEAMSTATE_GREEN_IMG );		

/*		m_BZ_UserList[i].TeamFlagImg[0] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, x_pos_teamflag_red, y_pos_teamflag_red, 47, 12, false );
		m_BZ_UserList[i].TeamFlagImg[0]->InitRadioButton( n_cmChecked, _RT( "red" ) );
		m_BZ_UserList[i].TeamFlagImg[0]->SetTextColor( 255, 255, 255 );	
		m_BZ_UserList[i].TeamFlagImg[0]->SetClientImage( TID_None );
		m_BZ_UserList[i].TeamFlagImg[0]->SetBtnImage( TID_None, TID_None, TID_TEAMSTATE_RED_IMG, TID_None );
		m_BZ_UserList[i].TeamFlagImg[0]->SetDirectCheck( true );
		
		m_BZ_UserList[i].TeamFlagImg[1] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_CREATEROOM_WND, x_pos_teamflag_green, y_pos_teamflag_green, 47, 12, false );
		m_BZ_UserList[i].TeamFlagImg[1]->InitRadioButton( n_cmUnChecked, _RT( "green" ) );
		m_BZ_UserList[i].TeamFlagImg[1]->SetTextColor( 255, 255, 255 );	
		m_BZ_UserList[i].TeamFlagImg[1]->SetClientImage( TID_None );
		m_BZ_UserList[i].TeamFlagImg[1]->SetBtnImage( TID_None, TID_None, TID_TEAMSTATE_GREEN_IMG, TID_None );
		m_BZ_UserList[i].TeamFlagImg[1]->SetDirectCheck( false );
		
		nRui->SetGroupWnd( 2, m_BZ_UserList[i].TeamFlagImg[0]->GetID(), m_BZ_UserList[i].TeamFlagImg[0]->GetID(),
																		m_BZ_UserList[i].TeamFlagImg[1]->GetID() );
*/		
		m_BZ_UserList[i].CharReadyTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, x_pos_ready, y_pos_ready, 54, 13, false );	
		m_BZ_UserList[i].CharReadyTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_BZ_UserList[i].CharReadyTxt->SetClientImage( TID_None );	
		m_BZ_UserList[i].CharReadyTxt->SetAlignText( n_atCenter, n_atCenter );
		m_BZ_UserList[i].CharReadyTxt->SetTextColor( 255, 255, 255 );		
		m_BZ_UserList[i].CharReadyTxt->SetText( _RT( "" ) );
		m_BZ_UserList[i].CharReadyTxt->SetFocusCheck( false );
		
		m_BZ_UserList[i].KickBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, x_pos_kick, y_pos_kick, 16, 16, false );
		m_BZ_UserList[i].KickBtn->SetBtnImage( TID_KICK_BTN_NORMAL, TID_KICK_BTN_FOCUS, TID_KICK_BTN_SELECT, TID_KICK_BTN_DISABLE );
		m_BZ_UserList[i].KickBtn->SetVisible( false );
		
	}
	
	m_BZ_UserList_SelectImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 161, 381, 32, 32, false );
	m_BZ_UserList_SelectImg->SetClientImage( TID_None );
	m_BZ_UserList_SelectImg->SetFocusCheck( false );
//	m_BZ_UserList_SelectImg->SetWndRepositionR( 639, 494 );		//	494, 514, 534, 554

	m_pModeNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, 738, 161, 114, 23, false );	
	m_pModeNameTxt->SetFontg( n_ftGulimChe, 13, n_fwNormal, n_fdNormal );
	m_pModeNameTxt->SetClientImage( TID_None );	
	m_pModeNameTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pModeNameTxt->SetTextColor( 255, 255, 255 );	
	m_pModeNameTxt->SetText( G_STRING_CHAR( IDS_WND_DEATH_MATCH ) );
	m_pModeNameTxt->SetFocusCheck( false );
	
	m_pMapComboBox = (CComboBox*)nRui->CreateWnd( n_wtComboBox, WID_None, WID_BZ_WAITINGROOM_WND, 706, 400, 184, 86, false );			
	m_pMapComboBox->SetListboxTileSizeTB( 5, 5 );
	m_pMapComboBox->SetListboxTileSizeLR( 5, 5 );
	m_pMapComboBox->SetListboxImage9( TID_CTRL_OUTLINE_L1 );
//	m_pMapComboBox->AddItem( G_STRING_CHAR( IDS_MAP_PVP_ZONE ), c_RockMap_Millena_Rain_PVP_Zone );
//	m_pMapComboBox->AddItem( G_STRING_CHAR( IDS_MAP_PVP_ZONE ), c_RockMap_Millena_City );
//	m_pMapComboBox->AddItem( G_STRING_CHAR( IDS_MAP_PVP_ZONE ), c_RockMap_Rain_City );	
	
	m_pMiniMapImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND, 706, 202, 184, 184, false );
	m_pMiniMapImg->SetClientImage( TID_None );
	m_pMiniMapImg->SetFitImg( true );

	m_pChatWndTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, 175, 459, 293, 21, false );	
	m_pChatWndTxt->SetFontg( n_ftGulimChe, 13, n_fwNormal, n_fdNormal );
	m_pChatWndTxt->SetClientImage( TID_None );	
	m_pChatWndTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pChatWndTxt->SetTextColor( 255, 255, 255 );	
	m_pChatWndTxt->SetText( G_STRING_CHAR( IDS_WND_CHATWND ) );
	m_pChatWndTxt->SetFocusCheck( false );

	///-- Draw Chating Message Txt 
	m_pChatMsgTxt[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, 137, 495, 462, 23, false );
//	m_pChatMsgTxt[0] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, 398, 6, 13, 13, false );
//	m_pChatMsgTxt[0]->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );		
	m_pChatMsgTxt[0]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatMsgTxt[0]->SetClientImage( TID_None );	
	m_pChatMsgTxt[0]->SetAlignText( n_atLeft, n_atCenter );
	m_pChatMsgTxt[0]->SetTextColor( 5, 5, 5 );
	m_pChatMsgTxt[0]->SetText( _RT( "" ) );
	m_pChatMsgTxt[0]->SetFocusCheck( false );

	m_pChatMsgTxt[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, 137, 518, 462, 23, false );
//	m_pChatMsgTxt[1] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, 398, 6, 13, 13, false );
//	m_pChatMsgTxt[1]->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );		
	m_pChatMsgTxt[1]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatMsgTxt[1]->SetClientImage( TID_None );	
	m_pChatMsgTxt[1]->SetAlignText( n_atLeft, n_atCenter );
	m_pChatMsgTxt[1]->SetTextColor( 5, 5, 5 );
	m_pChatMsgTxt[1]->SetText( _RT( "" ) );
	m_pChatMsgTxt[1]->SetFocusCheck( false );

	m_pChatMsgTxt[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, 137, 541, 462, 23, false );
//	m_pChatMsgTxt[2] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, 398, 6, 13, 13, false );
//	m_pChatMsgTxt[2]->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );		
	m_pChatMsgTxt[2]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatMsgTxt[2]->SetClientImage( TID_None );	
	m_pChatMsgTxt[2]->SetAlignText( n_atLeft, n_atCenter );
	m_pChatMsgTxt[2]->SetTextColor( 5, 5, 5 );
	m_pChatMsgTxt[2]->SetText( _RT( "" ) );
	m_pChatMsgTxt[2]->SetFocusCheck( false );

	m_pChatMsgTxt[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, 137, 564, 462, 23, false );	
//	m_pChatMsgTxt[3] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, 398, 6, 13, 13, false );
//	m_pChatMsgTxt[3]->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );		
	m_pChatMsgTxt[3]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatMsgTxt[3]->SetClientImage( TID_None );	
	m_pChatMsgTxt[3]->SetAlignText( n_atLeft, n_atCenter );
	m_pChatMsgTxt[3]->SetTextColor( 5, 5, 5 );
	m_pChatMsgTxt[3]->SetText( _RT( "" ) );
	m_pChatMsgTxt[3]->SetFocusCheck( false );

	m_pChatMsgScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, WID_BZ_WAITINGROOM_WND, 596, 493, 13, 96, false );
	m_pChatMsgScroll->SetClientImage( TID_None );
	m_pChatMsgScroll->InitSlide( stVertSld, 0, c_Max_BZChatMsg_Num - c_Max_Draw_BZChatMsg_Num, 0, 11 );//Type, min, max, value, 썸의 폭
	m_pChatMsgScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_None );	
	m_pChatMsgScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pChatMsgScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

#ifdef BATTLEROOM_HELPBTN_ERASE


	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_WAITINGROOM_WND , 516, 593, 46, 23, false );
	img->SetClientImage( TID_ROOM_CHAT_BLOCKBOX );

	m_pChatInputBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, WID_BZ_WAITINGROOM_WND, 138, 595, 377 + 47, 19, false );
	m_pChatInputBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatInputBox->SetTextSize( c_Max_ChatMsg_Length );
	m_pChatInputBox->SetClientImage( TID_None );
	m_pChatInputBox->SetClientColor(50, 50, 50);
	m_pChatInputBox->SetAlignText( n_atLeft, n_atCenter );
	m_pChatInputBox->SetBlank( 4, 0, 0, 0 );
	m_pChatInputBox->SetEditStyle( esText );
	m_pChatInputBox->SetText( _RT("") );
	m_pChatInputBox->SetTextColor(255, 255, 255);
	m_pChatInputBox->SetReturnActionClear( true );
	m_pChatInputBox->InitEditBox();
//	m_pChatInputBox->SetVisible( false );

	///-- Whisper Button 
	m_pWhisperBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, 563 ,  593, 46, 23, false );
	m_pWhisperBtn->SetBtnImage( TID_CHATWND_BTN_NORMAL, TID_CHATWND_BTN_FOCUS, TID_CHATWND_BTN_SELECT, TID_CHATWND_BTN_DISABLE );
	m_pWhisperBtn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pWhisperBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pWhisperBtn->SetTextColor( 255, 255, 255 );
	m_pWhisperBtn->SetText( G_STRING_CHAR( IDS_WND_WHISPER ) );

#else 

	m_pChatInputBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, WID_BZ_WAITINGROOM_WND, 138, 595, 377 + 45 , 19, false );
	m_pChatInputBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatInputBox->SetTextSize( c_Max_ChatMsg_Length );
	m_pChatInputBox->SetClientImage( TID_None );
	m_pChatInputBox->SetClientColor(50, 50, 50);
	m_pChatInputBox->SetAlignText( n_atLeft, n_atCenter );
	m_pChatInputBox->SetBlank( 4, 0, 0, 0 );
	m_pChatInputBox->SetEditStyle( esText );
	m_pChatInputBox->SetText( _RT("") );
	m_pChatInputBox->SetTextColor(255, 255, 255);
	m_pChatInputBox->SetReturnActionClear( true );
	m_pChatInputBox->InitEditBox();
	//	m_pChatInputBox->SetVisible( false );
	
	///-- Whisper Button 
	m_pWhisperBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, 516, 593, 46, 23, false );
	m_pWhisperBtn->SetBtnImage( TID_CHATWND_BTN_NORMAL, TID_CHATWND_BTN_FOCUS, TID_CHATWND_BTN_SELECT, TID_CHATWND_BTN_DISABLE );
	m_pWhisperBtn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pWhisperBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pWhisperBtn->SetTextColor( 255, 255, 255 );
	m_pWhisperBtn->SetText( G_STRING_CHAR( IDS_WND_WHISPER ) );
	
	///-- Chating Help Button 
	m_pChatHelpBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, 563, 593, 46, 23, false );
	m_pChatHelpBtn->SetBtnImage( TID_CHATWND_BTN_NORMAL, TID_CHATWND_BTN_FOCUS, TID_CHATWND_BTN_SELECT, TID_CHATWND_BTN_DISABLE );
	m_pChatHelpBtn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pChatHelpBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pChatHelpBtn->SetTextColor( 255, 255, 255 );
	m_pChatHelpBtn->SetText( G_STRING_CHAR( IDS_WND_HELP ) );
#endif 
	

	m_pOptionTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, 667, 459, 92, 21, false );	
	m_pOptionTxt->SetFontg( n_ftGulimChe, 13, n_fwNormal, n_fdNormal );
	m_pOptionTxt->SetClientImage( TID_None );	
	m_pOptionTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pOptionTxt->SetTextColor( 255, 255, 255 );	
	m_pOptionTxt->SetText( G_STRING_CHAR( IDS_WND_OPTION ) );
	m_pOptionTxt->SetFocusCheck( false );
	
	m_pMaxTimeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, 646, 507, 52, 19, false );	
	m_pMaxTimeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMaxTimeTxt->SetClientImage( TID_None );	
	m_pMaxTimeTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pMaxTimeTxt->SetTextColor( 5, 5, 5 );	
	m_pMaxTimeTxt->SetText( G_STRING_CHAR( IDS_WND_TIME ) );
	m_pMaxTimeTxt->SetFocusCheck( false );

	m_pMaxTimeRBtn[0] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_WAITINGROOM_WND, 701, 504, 35, 23, false );  
	m_pMaxTimeRBtn[0]->InitRadioButton( n_cmChecked, _RT( "3" ) );
	m_pMaxTimeRBtn[0]->SetTextColor( 255, 255, 255 );	
	m_pMaxTimeRBtn[0]->SetClientImage( TID_None );
	m_pMaxTimeRBtn[0]->SetBtnImage( TID_None, TID_None, TID_OPTION_UNLIMIT_BTN_FOCUS, TID_None );
	m_pMaxTimeRBtn[0]->SetDirectCheck( true );

	m_pMaxTimeRBtn[1] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_WAITINGROOM_WND, 737, 504, 35, 23, false ); 
	m_pMaxTimeRBtn[1]->InitRadioButton( n_cmUnChecked, _RT( "5" ) );
	m_pMaxTimeRBtn[1]->SetTextColor( 255, 255, 255 );	
	m_pMaxTimeRBtn[1]->SetClientImage( TID_None );
	m_pMaxTimeRBtn[1]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxTimeRBtn[1]->SetDirectCheck( false );

	m_pMaxTimeRBtn[2] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_WAITINGROOM_WND, 773, 504, 35, 23, false ); 
	m_pMaxTimeRBtn[2]->InitRadioButton( n_cmUnChecked, _RT( "7" ) );
	m_pMaxTimeRBtn[2]->SetTextColor( 255, 255, 255 );	
	m_pMaxTimeRBtn[2]->SetClientImage( TID_None );
	m_pMaxTimeRBtn[2]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxTimeRBtn[2]->SetDirectCheck( false );

	m_pMaxTimeRBtn[3] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_WAITINGROOM_WND, 809, 504, 35, 23, false ); 
	m_pMaxTimeRBtn[3]->InitRadioButton( n_cmUnChecked, _RT( "10" ) );
	m_pMaxTimeRBtn[3]->SetTextColor( 255, 255, 255 );	
	m_pMaxTimeRBtn[3]->SetClientImage( TID_None );
	m_pMaxTimeRBtn[3]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxTimeRBtn[3]->SetDirectCheck( false );

	m_pMaxTimeRBtn[4] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_WAITINGROOM_WND, 845, 504, 35, 23, false );   
	m_pMaxTimeRBtn[4]->InitRadioButton( n_cmUnChecked, _RT( "15" ) );
	m_pMaxTimeRBtn[4]->SetTextColor( 255, 255, 255 );	
	m_pMaxTimeRBtn[4]->SetClientImage( TID_None );
	m_pMaxTimeRBtn[4]->SetBtnImage( TID_None, TID_None, TID_OPTION_SHORT_RBTN_FOCUS, TID_None );
	m_pMaxTimeRBtn[4]->SetDirectCheck( false );

	nRui->SetGroupWnd( 5, m_pMaxTimeRBtn[0]->GetID(), m_pMaxTimeRBtn[0]->GetID(),
													  m_pMaxTimeRBtn[1]->GetID(), 
													  m_pMaxTimeRBtn[2]->GetID(), 
													  m_pMaxTimeRBtn[3]->GetID(), 
													  m_pMaxTimeRBtn[4]->GetID() );

	m_pUseItemTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_WAITINGROOM_WND, 646, 535, 52, 19, false );	
	m_pUseItemTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pUseItemTxt->SetClientImage( TID_None );	
	m_pUseItemTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pUseItemTxt->SetTextColor( 5, 5, 5 );	
	m_pUseItemTxt->SetText( G_STRING_CHAR( IDS_ITEM ) );
	m_pUseItemTxt->SetFocusCheck( false );

	m_pUseItemRBtn[0] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_WAITINGROOM_WND, 701, 532, 89, 23, false ); 
	m_pUseItemRBtn[0]->InitRadioButton( n_cmChecked, G_STRING_CHAR( IDS_WND_ON ) );
	m_pUseItemRBtn[0]->SetTextColor( 255, 255, 255 );	
	m_pUseItemRBtn[0]->SetClientImage( TID_None );
	m_pUseItemRBtn[0]->SetBtnImage( TID_None, TID_None, TID_OPTION_LONG_RBTN_FOCUS, TID_None );
	m_pUseItemRBtn[0]->SetDirectCheck( true );

	m_pUseItemRBtn[1] = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_None, WID_BZ_WAITINGROOM_WND, 791, 532, 89, 23, false );
	m_pUseItemRBtn[1]->InitRadioButton( n_cmUnChecked, G_STRING_CHAR( IDS_WND_OFF ) );
	m_pUseItemRBtn[1]->SetTextColor( 255, 255, 255 );	
	m_pUseItemRBtn[1]->SetClientImage( TID_None );
	m_pUseItemRBtn[1]->SetBtnImage( TID_None, TID_None, TID_OPTION_LONG_RBTN_FOCUS, TID_None );
	m_pUseItemRBtn[1]->SetDirectCheck( false );

	nRui->SetGroupWnd( 2, m_pUseItemRBtn[0]->GetID(), m_pUseItemRBtn[0]->GetID(),
													  m_pUseItemRBtn[1]->GetID() );

	m_pExitBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_BZ_WAITINGROOM_WND, 794, 572, 100, 56, false ); 
	m_pExitBtn->SetBtnImage( TID_GAMEREADY_GOOUT_BTN_NORMAL, TID_GAMEREADY_GOOUT_BTN_FOCUS, TID_GAMEREADY_GOOUT_BTN_SELECT, TID_GAMEREADY_GOOUT_BTN_DISABLE );
	m_pExitBtn->SetFontg( n_ftGulimChe, 15, n_fwBold, n_fdNormal );
	m_pExitBtn->SetClientImage( TID_None );	
	m_pExitBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pExitBtn->SetTextColor( 255, 255, 255 );		
	m_pExitBtn->SetText( G_STRING_CHAR( IDS_WND_GOOUT ) );
	
	return;
}

///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CBZ_WaitingRoomWndProc::Update()
{
	return;
}

///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CBZ_WaitingRoomWndProc::Proc( SEventMessage* EMsg )
{	
	//=========================================================================================
	if( EMsg->FocusWnd == m_BZ_UserList[0].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[0].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[1].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[1].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[2].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[2].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[3].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[3].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[4].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[4].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[5].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[5].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[6].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[6].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[7].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[7].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[8].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[8].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[9].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[9].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[10].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[10].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[11].KickBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( m_WaitingUserInfo[11].FighterInfo.thePCID ); 
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[0].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 0; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[0].CharImgBtn->GetXpos(), m_BZ_UserList[0].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[1].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 1; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[1].CharImgBtn->GetXpos(), m_BZ_UserList[1].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[2].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 2; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[2].CharImgBtn->GetXpos(), m_BZ_UserList[2].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[3].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 3; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[3].CharImgBtn->GetXpos(), m_BZ_UserList[3].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[4].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 4; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[4].CharImgBtn->GetXpos(), m_BZ_UserList[4].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[5].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 5; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[5].CharImgBtn->GetXpos(), m_BZ_UserList[5].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[6].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 6; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[6].CharImgBtn->GetXpos(), m_BZ_UserList[6].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[7].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 7; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[7].CharImgBtn->GetXpos(), m_BZ_UserList[7].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[8].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 8; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[8].CharImgBtn->GetXpos(), m_BZ_UserList[8].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[9].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 9; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[9].CharImgBtn->GetXpos(), m_BZ_UserList[9].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[10].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 10; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[10].CharImgBtn->GetXpos(), m_BZ_UserList[10].CharImgBtn->GetYpos() );
		}
	}
	else if( EMsg->FocusWnd == m_BZ_UserList[11].CharImgBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_SelectUserIndex = 11; 			
			m_BZ_UserList_SelectImg->SetClientImage( TID_PARTY_MEMBER_CHAR_SELECT );
			m_BZ_UserList_SelectImg->SetWndRepositionR( m_BZ_UserList[11].CharImgBtn->GetXpos(), m_BZ_UserList[11].CharImgBtn->GetYpos() );
		}
	}
	//=========================================================================================
	else if( EMsg->FocusWnd == m_pTeam1Btn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_bIsReady == FALSE )
			{
				g_RockClient.Send_CsSelectTeam( 0 );
			}
		}
	}
	else if( EMsg->FocusWnd == m_pTeam2Btn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_bIsReady == FALSE )
			{
				g_RockClient.Send_CsSelectTeam( 1 );
			}
		}
	}
	else if( EMsg->FocusWnd == m_pReadyORStartBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_bIsMaster )					// 방장이라면...게임 시작 
			{
				g_RockClient.Send_CsFightStart();
			}
			else
			{
				m_bIsReady = !( m_bIsReady );			// 토글형 버튼 

				if( m_bIsReady )
				{
					g_RockClient.Send_CsFightReady( n_Success );
				}
				else
				{
					g_RockClient.Send_CsFightReady( n_Fail );
				}
			}
		}
	}
	else if( EMsg->FocusWnd == m_pMapComboBox->GetID() )
	{
		if( EMsg->Notify == NM_CB_TEXT_EXCHANGE )	
		{
//			m_pModeListCBox->GetCurSelIdx();			///-- 선택된 Text의 순차적인 Index
//			m_pModeListCBox->GetCurSelValue();			///-- 선택된 Text의 Value( AddItem 의 두번째 인자로 지정된 값 )

			if( m_bIsMaster )
			{
				int MiniMapIdx = m_pMapComboBox->GetCurSelValue();
				m_RoomInfo.theMapCode = MiniMapIdx; // c_RockMap_Millena_Rain_PVP_Zone; 

				SendChangeRoomInfo();				
			}
		}
	}
	//=========================================================================================	
	else if( EMsg->FocusWnd == m_pMaxTimeRBtn[n_BattleZone_Minute3]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_bIsMaster )
			{
//				nRui->SendEvent( m_pMaxTimeRBtn[n_BattleZone_Minute3]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_MaxGameTime = n_BattleZone_Minute3;
				SendChangeRoomInfo();
			}
		}
	}
	else if( EMsg->FocusWnd == m_pMaxTimeRBtn[n_BattleZone_Minute5]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_bIsMaster )
			{
//				nRui->SendEvent( m_pMaxTimeRBtn[n_BattleZone_Minute5]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_MaxGameTime = n_BattleZone_Minute5;
				SendChangeRoomInfo();
			}
		}
	}
	else if( EMsg->FocusWnd == m_pMaxTimeRBtn[n_BattleZone_Minute7]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_bIsMaster )
			{
//				nRui->SendEvent( m_pMaxTimeRBtn[n_BattleZone_Minute7]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_MaxGameTime = n_BattleZone_Minute7;
				SendChangeRoomInfo();
			}
		}
	}
	else if( EMsg->FocusWnd == m_pMaxTimeRBtn[n_BattleZone_Minute10]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_bIsMaster )
			{
//				nRui->SendEvent( m_pMaxTimeRBtn[n_BattleZone_Minute10]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_MaxGameTime = n_BattleZone_Minute10;
				SendChangeRoomInfo();
			}
		}
	}
	else if( EMsg->FocusWnd == m_pMaxTimeRBtn[n_BattleZone_Minute15]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_bIsMaster )
			{
//				nRui->SendEvent( m_pMaxTimeRBtn[n_BattleZone_Minute15]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_MaxGameTime = n_BattleZone_Minute15;
				SendChangeRoomInfo();
			}
		}
	}
	else if( EMsg->FocusWnd == m_pUseItemRBtn[n_BattleZone_UseItem_On]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_bIsMaster )
			{
//				nRui->SendEvent( m_pUseItemRBtn[n_BattleZone_UseItem_On]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_UseItem = n_BattleZone_UseItem_On;
				SendChangeRoomInfo();
			}
		}
	}	
	else if( EMsg->FocusWnd == m_pUseItemRBtn[n_BattleZone_UseItem_Off]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_bIsMaster )
			{
//				nRui->SendEvent( m_pUseItemRBtn[n_BattleZone_UseItem_Off]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
				m_UseItem = n_BattleZone_UseItem_Off;
				SendChangeRoomInfo();
			}
		}
	}	
	//=========================================================================================
	else if( EMsg->FocusWnd == m_pExitBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			//====================================================================================
			//====================================================================================
			// test code
			//====================================================================================
/*			SetInitUserSlot();

			SetChangeRoomInfo(	2 + (BYTE) rand() % 19, // c_RockMap_Millena_Rain_PVP_Zone,
								g_BZ_CreateRoomWnd.m_MaxGameTimeValue[ rand() % c_Max_OptionBtn_Time_Num ], 
								g_BZ_CreateRoomWnd.m_UseItemValue[ rand() % c_Max_OptionBtn_Item_Num ] );

			int i = 0;
			int pc_id = 0;
		
			for( i = 1; i < c_Max_WaitingRoom_UserNum; ++i )
			{
				SRpdOtherPC	pc_info;					
				BYTE		team_info;

				pc_info = m_WaitingUserInfo[ 0 ].FighterInfo;
				pc_info.theCitizen = rand() % 2; // RAIN or MILLENA;
				pc_info.theGender = rand() % 2;

				pc_info.thePCID = 100 + i;

				pc_info.theFace = g_Pc.m_MainPC.char_info.theFace;
				pc_info.theHair = g_Pc.m_MainPC.char_info.theHair;
				
				sprintf( pc_info.thePCName, "Fighter No.%d", i );				
				
				for( int idx = 0; idx < RP_MAX_VISIBLE_EQUIP; ++idx )
				{
					pc_info.theWearItem[idx] = g_Pc.m_MainPC.char_info.theWearItem[idx];
				}

				team_info = rand() % 2; 

				JoinedUserListSlot( pc_info, team_info );

				if( rand() % 2 )
				{
					SetChangeFightReady( i, n_BZ_State_Ready );
				}
				else
				{
					SetChangeFightReady( i, n_BZ_State_Wait );

				}

				if( rand() % 2 )
				{
					SetChangeSelectTeam( i, n_BattleZone_TeamFlag_Red ); 
				}
				else
				{
					SetChangeSelectTeam( i, n_BattleZone_TeamFlag_Green ); 
				}				
			}

			pc_id = rand() % c_Max_WaitingRoom_UserNum;		

			if( pc_id == 7 )
			{
				SetChangeRoomMaster( g_Pc.m_MainPC.lUnique );
			}
			else
			{
				SetChangeRoomMaster( rand() % c_Max_WaitingRoom_UserNum );
			}
						
			int out_num = rand() % ( c_Max_WaitingRoom_UserNum - 1 );

			for( i = 0; i < out_num; ++i )
			{
				pc_id = 100 + ( rand() % c_Max_WaitingRoom_UserNum );		

				OutUserListSlot( pc_id );
			}
*/
			//====================================================================================
			//====================================================================================
			//====================================================================================
			g_RockClient.Send_CsOutFightRoom( g_Pc.m_MainPC.lUnique );
		}
	}
	//=========================================================================================
#ifndef BATTLEROOM_HELPBTN_ERASE
	else if( EMsg->FocusWnd == m_pChatHelpBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			int i = 0;
			char str_temp[256] = {0,};

			for( i = 0; i < g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat; ++i )
			{
//				InsertBZHelpChatMsg(_RT("도움말 리스트가 보여집니다."));				 
				Rsprintf( RWCHAR(str_temp), _RT("[%s] %s"), G_STRING_CHAR( IDS_WND_HELP ), RWCHAR(g_Pc_Manager.theAutoHelpTipMsg.theAutoHelpTipAdminMsg_PVPChat[i]) ); 
				InsertBZHelpChatMsg( str_temp );		
			}
		}
	}
#endif 
	else if( EMsg->FocusWnd == m_pWhisperBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_SelectUserIndex == -1 )
			{
				InsertBZHelpChatMsg( G_STRING_CHAR( IDS_GAME_WHISPER ) );	 
				m_pChatInputBox->SetText(_RT("/w "));								 	
				PostMessage( m_pChatInputBox->GetHEdit(), WM_USER + 44, NULL, NULL );
				nRui->SendEvent( m_pWhisperBtn->GetID(), n_emSetFocus, 0, 0, 0, 0 );					
			}
			else
			{
				char whisper_temp[128] = {0,};

				Rsprintf( RWCHAR(whisper_temp), _RT("/w %s "), RWCHAR(m_WaitingUserInfo[m_SelectUserIndex].FighterInfo.thePCName) );
				m_pChatInputBox->SetText( whisper_temp );
				PostMessage( m_pChatInputBox->GetHEdit(), WM_USER + 44, NULL, NULL );
				nRui->SendEvent( m_pWhisperBtn->GetID(), n_emSetFocus, 0, 0, 0, 0 );					

				m_SelectUserIndex = -1;
				m_BZ_UserList_SelectImg->SetClientImage( TID_None );
			}
		}
	}
	else if( EMsg->FocusWnd == m_pChatMsgScroll->GetID() )
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || 
			( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || 
			( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{			
			SetChatMsgScroll( (int)EMsg->fParam );			
		}
	}
	//=========================================================================================
	else if( EMsg->FocusWnd == m_pChatInputBox->GetID() )
	{
		//by simwoosung
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			m_uChatType = n_BZ_Normal_Chat;
			
			SPcDataParam* PcParam = nRui->GetPcParamInfo();
			SAFE_STR_CPY( m_strChatstring, _RT("") , c_Max_ChatMsg_Length * 2 );
			SAFE_STR_CPY( m_strTempBuf1, _RT("") , c_Max_ChatMsg_Length * 2 );
			SAFE_STR_CPY( m_strTempBuf2, _RT("") , c_Max_ChatMsg_Length * 2 );
			SAFE_STR_CPY( m_strWhisDest, _RT("") , 256 );
			
			nRui->SendEvent( m_pChatInputBox->GetID(), n_emGetText, (EPARAM)m_strChatstring, 
							c_Max_ChatMsg_Length + 1, 0, 0 );			
			nRui->SetEnterFlag( n_nfEditEnter );

			RTCHAR *pDest = NULL;					
			pDest = RWCHAR(m_strChatstring);
#ifdef UNICODE_ROCK
			pDest = ::GetStringReturnInStringW(' ', false, 100, pDest, m_strTempBuf1);
#else 
			pDest = ::GetStringReturnInString(' ', false, 100, pDest, m_strTempBuf1);
#endif
			
			if( (Rstrcmp(m_strTempBuf1, _RT("/w") ) == 0) || 
				(Rstrcmp(m_strTempBuf1, _RT("/W") ) == 0) )			
			{
#ifdef UNICODE_ROCK
				pDest = ::GetStringReturnInStringW(' ', false, 100, pDest, m_strWhisDest);
				
				while( (Rstrcmp(m_strWhisDest, _RT("") ) == 0) && (NULL != pDest) )
				{
					pDest = ::GetStringReturnInStringW(' ', false, 100, pDest, m_strWhisDest);
				}
				
				int nSize = Rstrlen(m_strWhisDest)*2;

#else
				pDest = ::GetStringReturnInString(' ', false, 100, pDest, m_strWhisDest);
				
				while( (Rstrcmp(m_strWhisDest, _RT("") ) == 0) && (NULL != pDest) )
				{
					pDest = ::GetStringReturnInString(' ', false, 100, pDest, m_strWhisDest);
				}
				
				int nSize = Rstrlen(m_strWhisDest);

#endif
				
				if( nSize >= RP_MAX_PC_NAME )
				{
					InsertBZHelpChatMsg( G_STRING_CHAR( IDS_SYSPR_OVER_FLOW_WHIS_TEXT ) );	
					return;
				}				
				
				if( (Rstrcmp(RCHAR(m_strWhisDest), _RT("") ) != 0) && (NULL != pDest) )
				{
					m_uChatType = n_BZ_Whisper_Chat;
					SAFE_STR_CPY( m_strChatstring, pDest, c_Max_ChatMsg_Length * 2 );
				}
			}

			if(Rstrcmp(m_strChatstring, _RT("") ) != 0)
			{
				bool bCoverFlag = false;						//도배방지 플래그
	
				if(  SAFE_TIME_COMPARE( g_nowTime , < , SAFE_TIME_ADD( m_lPrevTime , 1200 ) )  )	//1.2초 이전에 입력		
				{
					bCoverFlag = true;
				}				
				
				if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_strChatstring ) )
				{
					memset( m_strChatstring , 0 , sizeof(TCHAR) * (c_Max_ChatMsg_Length * 2) );
					Rsprintf( RWCHAR(m_strChatstring) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
				}				
				
				switch(m_uChatType)
				{
					case n_BZ_Normal_Chat:
						{
							if(!bCoverFlag)
							{
								//이곳에서 채팅문자 네트워크로 전송	
								g_RockClient.Send_RpCsSay( m_strChatstring );

								InsertBZNomalChatMsg(PcParam->PCName, m_strChatstring);

							}
							else
							{
								InsertBZNomalChatMsg(PcParam->PCName, 
									RCHAR(G_STRING_CHAR( IDS_COVER_PREVENT_APPLY )));			
							}
						}
						break;
					case n_BZ_Whisper_Chat:
						{
							if(!bCoverFlag)	
							{
								//이곳에서 귓속말 네트워크로 전송
#ifdef	NETWORK_CLIENT

								if(g_MCommunityWnd.m_MBlockTabWnd.ExistBlockName(m_strWhisDest))
								{
									if(!g_RockClient.m_bIsDrawFullScreenUI)
									{
										SystemPrint( n_DarkGreen ,n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER));	
									}
									else
									{
										if(g_BZ_RoomListWnd.IsVisible)
										{
											g_BZ_RoomListWnd.InsertBZHelpChatMsg(G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER));
										}
										else if(g_BZ_WaitingRoomWnd.IsVisible)
										{
											g_BZ_WaitingRoomWnd.InsertBZHelpChatMsg(G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER));
										}
									}
								}
								else
								{
									g_RockClient.Send_RpCsWhisper(m_strWhisDest , m_strChatstring);
								}
							
#endif						
								//InsertBZWhisperChatMsg(m_strWhisDest, m_strChatstring, true);
							}
						}
						break;				
				}
				
				m_lPrevTime = g_nowTime;
			}		

			if(m_uChatType == n_BZ_Whisper_Chat)
			{
				Rsprintf( RWCHAR(m_strChatstring) , _RT("/w %s ") , RWCHAR(m_strWhisDest));			
				m_pChatInputBox->SetText(m_strChatstring);
				PostMessage( m_pChatInputBox->GetHEdit(), WM_USER + 44, NULL, NULL );
			}
			nRui->SendEvent( m_pChatInputBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );		
		}
	}

	return;
}

//by simwoosung
void CBZ_WaitingRoomWndProc::InsertBZChatMsg()
{
	if( m_nInsertIndex >= c_Max_BZChatMsg_Num )
	{
		for(int i = 0 ; i < c_Max_BZChatMsg_Num ; i++)
		{
			m_BZ_ChatMsgList[i] = m_BZ_ChatMsgList[i + 1];			
		}
		m_BZ_ChatMsgList[m_nInsertIndex-1] = m_tempChatMsgList;
	}
	else
	{
		m_BZ_ChatMsgList[m_nInsertIndex] = m_tempChatMsgList;
		m_nInsertIndex++;
	}
	
	if( m_nInsertIndex > c_Max_Draw_BZChatMsg_Num )
	{		
		m_StartLineNum_ChatMsg = m_nInsertIndex - c_Max_Draw_BZChatMsg_Num;
		m_pChatMsgScroll->ReSetScroll(0, (c_Max_BZChatMsg_Num - c_Max_Draw_BZChatMsg_Num), m_StartLineNum_ChatMsg);
		if( m_StartLineNum_ChatMsg > (c_Max_BZChatMsg_Num - c_Max_Draw_BZChatMsg_Num) )
		{
			m_StartLineNum_ChatMsg = (c_Max_BZChatMsg_Num - c_Max_Draw_BZChatMsg_Num);	
		}
	}
	
	SetRefreshChatMsg();
}

void CBZ_WaitingRoomWndProc::SetRefreshChatMsg()
{
	int i = 0;
	int chat_msg_num = 0;
	int chat_index = 0;

	chat_msg_num = m_nInsertIndex;

	for( i = c_Max_Draw_BZChatMsg_Num - 1; i >= 0; --i )
	{
		chat_index = m_StartLineNum_ChatMsg + i;

		if( chat_index < chat_msg_num )
		{
			switch(m_BZ_ChatMsgList[chat_index].ChatType)
			{
				case n_BZ_Normal_Chat:
					{
						m_pChatMsgTxt[i]->SetTextColor(0, 0, 0);
					}
					break;
				case n_BZ_Whisper_Chat:
					{
						m_pChatMsgTxt[i]->SetTextColor(145, 0, 145);
					}
					break;
				case n_BZ_Help_Chat:
					{
						m_pChatMsgTxt[i]->SetTextColor(145, 125, 0);
					}
					break;
			}
			m_pChatMsgTxt[i]->SetText( m_BZ_ChatMsgList[chat_index].ChatMsg );
		}
		else		// 리스트에 없는 메세지들은 초기화!!
		{
			m_pChatMsgTxt[i]->SetText( _RT( "" ) );			
		}
	}	

	return;
}

void CBZ_WaitingRoomWndProc::InsertBZChatMsg(TCHAR * strInString)
{
	RTCHAR * pTempBuf = RWCHAR(strInString);
	RTCHAR * pTempOut = RWCHAR(m_strTempBuf2);

	memset(m_strTempBuf2, NULL, sizeof(TCHAR) *  c_Max_ChatMsg_Length * 2);

	int nCount = 0;	
	while(pTempBuf)
	{
		if(nCount == 0)
		{
#ifdef UNICODE_ROCK
			pTempBuf = ::GetStringReturnInStringW('\\', false, c_Max_ChatMsg_Length, 
				pTempBuf, pTempOut);
#else 
			pTempBuf = ::GetStringReturnInString('\\', false, c_Max_ChatMsg_Length, 
				pTempBuf, pTempOut);
#endif 
			Rsprintf(RWCHAR(m_tempChatMsgList.ChatMsg), _RT("%s"), RWCHAR(pTempOut));
			InsertBZChatMsg();
		}
		else
		{
#ifdef UNICODE_ROCK
			pTempBuf = ::GetStringReturnInStringW('\\', false, (c_Max_ChatMsg_Length - 6), 
				pTempBuf, pTempOut);
#else 
			pTempBuf = ::GetStringReturnInString('\\', false, (c_Max_ChatMsg_Length - 6), 
												pTempBuf, pTempOut);
#endif 

	
			Rsprintf(RWCHAR(m_tempChatMsgList.ChatMsg), _RT("    %s"), RWCHAR(pTempOut));
			InsertBZChatMsg();
		}

		SAFE_STR_CPY( pTempOut, _RT(""), c_Max_ChatMsg_Length * 2 );
		
		nCount++;
	}
}

//by simwoosung
void CBZ_WaitingRoomWndProc::InsertBZNomalChatMsg(TCHAR *strDest, TCHAR *strMsg)
{
	Rsprintf(RWCHAR(m_strTempBuf1), _RT("%s : %s"),RWCHAR(strDest), RWCHAR(strMsg));			

	m_tempChatMsgList.ChatType = n_BZ_Normal_Chat;	
	InsertBZChatMsg(m_strTempBuf1);
}

void CBZ_WaitingRoomWndProc::InsertBZWhisperChatMsg(TCHAR *strDest, TCHAR *strMsg, bool bToFlag)
{
	if(bToFlag)
	{
		Rsprintf(RWCHAR(m_strTempBuf1), _RT("To%s : %s"),RWCHAR(strDest), RWCHAR(strMsg));		
	}
	else
	{
		Rsprintf(RWCHAR(m_strTempBuf1), _RT("From%s : %s"),RWCHAR(strDest), RWCHAR(strMsg));		
	}
	m_tempChatMsgList.ChatType = n_BZ_Whisper_Chat;
	InsertBZChatMsg(m_strTempBuf1);
}

void CBZ_WaitingRoomWndProc::InsertBZHelpChatMsg(void *pstrMsg)
{
	char* strMsg = (char*)pstrMsg;
	
	Rsprintf(RWCHAR(m_strTempBuf1), _RT("%s"), RWCHAR(strMsg));
	m_tempChatMsgList.ChatType = n_BZ_Help_Chat;

	InsertBZChatMsg(m_strTempBuf1);	
}

void CBZ_WaitingRoomWndProc::Open( bool aIsNewGame )
{
	if(g_RockClient.m_IsMouseRotate)
	{
		nRui->SetCursorType(n_CT_NORMAL);
		g_RockClient.m_IsMouseRotate = false;
	}	
	
	int i = 0;
	char temp[100] = {0,};
	

	nRui->ResetCursorItem();
	nRui->SendEvent( WID_AliveWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

	g_BZ_RoomListWnd.Close();
	
	ChatMsgInit();
	SetChatMsgScroll( 0 );

	nRui->SendEvent( WID_BZ_WAITINGROOM_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SendEvent( m_pChatInputBox->GetID(), n_emSetFocus, (EPARAM)true, 0, 0, 0 );
	
/*
typedef struct _SFightRoomInfo
{
	char		theName[40];
	char		thePassword[10];
	WORD		theLockFlag:1;
	WORD		theGameType:5;
	WORD		theMemberNum:4;
	WORD		theTimeOut:5;			// 0 : 무제한 
	WORD		theItemFlag:1;
	WORD		theMapCode;
} SFightRoomInfo, * SFightRoomInfoPtr;
*/

	if( aIsNewGame )
	{	
		Rsprintf(RWCHAR(temp), _RT("[%d] %s"), m_WaitingRoomID, m_RoomInfo.theName ); 
		m_pRoomNameTxt->SetText( temp );
		m_pReadyORStartBtn->SetBtnHold( false );
		
		if( m_MasterID == g_Pc.m_MainPC.lUnique )
		{
			m_bIsMaster = TRUE;
		}
		else
		{
			m_bIsMaster = FALSE;
		}

		if( m_bIsMaster )
		{
			m_pReadyORStartBtn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) );

			SetChangeRoomInfo( m_RoomInfo.theMapCode, g_BZ_CreateRoomWnd.m_MaxGameTimeValue[ g_BZ_CreateRoomWnd.m_MaxGameTime ], 
								  g_BZ_CreateRoomWnd.m_UseItemValue[ g_BZ_CreateRoomWnd.m_UseItem ], NULL );

			m_pMapComboBox->SetFocusCheck( true );
		}
		else
		{
			m_pReadyORStartBtn->SetText( G_STRING_CHAR( IDS_WND_READY ) );

			SetChangeRoomInfo( m_RoomInfo.theMapCode, m_RoomInfo.theTimeOut, m_RoomInfo.theItemFlag, NULL );

			m_pMapComboBox->SetFocusCheck( false );
		}
		
		
		g_BZ_RoomListWnd.SetGameTypeText( m_RoomInfo.theGameType, temp );		
		m_pModeNameTxt->SetText( temp );
		
		
		m_pMapComboBox->ClearItem();
		
		for( i = 0; i < m_FightMapInfo[ m_RoomInfo.theGameType ].MapNum; ++i )
		{
			GetAreaStrName( temp, m_FightMapInfo[ m_RoomInfo.theGameType ].MapIdx[ i ] );
			
			m_pMapComboBox->AddItem( temp, m_FightMapInfo[ m_RoomInfo.theGameType ].MapIdx[ i ] );		
		}
		
		if( m_FightMapInfo[ m_RoomInfo.theGameType ].MapNum )
		{
			GetAreaStrName( temp, m_RoomInfo.theMapCode ); //m_FightMapInfo[ m_RoomInfo.theGameType ].MapIdx[ 0 ] );
			m_pMapComboBox->SetSelectText( temp );
		}
		else
		{
			m_pMapComboBox->SetSelectText( G_STRING_CHAR( IDS_MAP_PVP_ZONE ) );
		}		
		
		if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Nation )
		{
			m_pTeam1Img->SetClientImage( TID_PVP_TEAMMARK_MILLENA );
			m_pTeam2Img->SetClientImage( TID_PVP_TEAMMARK_RAIN );
			
			m_pTeam1Btn->SetBtnHold( true );
			m_pTeam2Btn->SetBtnHold( true );
		}
		else
		{
			m_pTeam1Img->SetClientImage( TID_PVP_TEAMMARK_NO1 );
			m_pTeam2Img->SetClientImage( TID_PVP_TEAMMARK_NO2 );
			
			m_pTeam1Btn->SetBtnHold( false );
			m_pTeam2Btn->SetBtnHold( false );
		}
		
		if( m_RoomInfo.theGameType == n_Survival )
		{
			m_pTeam1Btn->SetBtnHold( true );
			m_pTeam2Btn->SetBtnHold( true );
			
			for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
			{
				m_BZ_UserList[i].TeamFlagRedImg->SetClientImage( TID_None );
				m_BZ_UserList[i].TeamFlagGreenImg->SetClientImage( TID_None );
			}
		}

		if(m_ZoneViewType == n_BattleZone_ViewZone)
		{
			m_pTeam1Btn->SetBtnHold( true );
			m_pTeam2Btn->SetBtnHold( true );
			m_pReadyORStartBtn->SetBtnHold( true );
		}
	}

	m_bIsReady = FALSE;
	
	for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{			
		if( m_WaitingUserInfo[ i ].FighterState != n_BZ_State_None )
		{			
			if( m_WaitingUserInfo[i].FighterInfo.thePCID == m_MasterID )
			{
				m_BZ_UserList[i].CharReadyTxt->SetText( G_STRING_CHAR( IDS_WND_COM_START ) );
			}
			else
			{
				m_WaitingUserInfo[ i ].FighterState = n_BZ_State_Wait;

				m_BZ_UserList[ i ].CharReadyTxt->SetText( _RT( "" ) );
			}
		}		
	}

	if( g_ChatMainProc.m_ChatActPrintWnd.IsVisible )
	{
		g_ChatMainProc.SetOutState( COutState_NonAct );
	}

	return;
}

void CBZ_WaitingRoomWndProc::Close()
{
	nRui->SendEvent( WID_BZ_WAITINGROOM_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

	nRui->SendEvent( m_pChatInputBox->GetID(), n_emEditDisable, 0, 0, 0, 0 );	

	ChatMsgInit();
	SetChatMsgScroll( 0 );

	SetRefreshChatMsg();

	for( int i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{
		if( m_WaitingUserInfo[i].FighterState == n_BZ_State_None )
		{
			OutDisappearPC( i );
		}
	}
	
	return;
}


void CBZ_WaitingRoomWndProc::SetFightMapInfo()
{
/*
typedef struct _SFightMapInfo
{
	struct _SFightPos
	{
		WORD	theXCell;
		WORD	theZCell;
	};
	WORD		theMapCode;
	BYTE		theGameType;
	_SFightPos	thePosition[12];	// 0-5 : 1팀 , 6-11 : 2팀 
} SFightMapInfo, * SFightMapInfoPtr;
*/

	int type = 0;
	int idx = 0;
	int idx_num = 0;
	BYTE bit_temp = 1;			

	// 4가지 게임 타입 
	for( type = 0; type < c_Max_GameMode_Num; ++type )
	{
		idx_num = 0;

		for( idx = 0; idx < c_Max_FightMap_Num; ++idx )
		{
			if( g_Pc_Manager.theFightMapInfoTable[idx].theMapCode == 0 )
			{
				m_FightMapInfo[type].MapNum = idx_num;

				break;
			}
			
			// 타입별 비트당 정보 저장, 각각의 비트당 게임 타입 설정 
			bit_temp = 1;
			bit_temp <<= type;		
				
			if( g_Pc_Manager.theFightMapInfoTable[idx].theGameType & bit_temp ) 
			{			
				m_FightMapInfo[type].MapIdx[idx_num] = g_Pc_Manager.theFightMapInfoTable[idx].theMapCode;
				++idx_num;
			}
		}
	}

	return;
}


void CBZ_WaitingRoomWndProc::SetInitUserSlot()
{
/*
typedef struct SBZ_WaitingRoomInfo
{
	NBZ_UserState			FighterState;			// 빈슬롯, 준비 상태, 대기중 
	SRpdOtherPC				FighterInfo;			// 국가, 이름, 캐릭터 얼굴 
	BYTE					FighterTeam;			// 1팀, 2팀 
	
} SBZ_WaitingRoomInfo;

typedef struct SBattleZoneUserList
{
	CImageBox*				SlotBGImg;
	CImageBox*				NationImg;
	CCharImgButton*			CharImgBtn;
	CTextBox*				CharNameTxt;
	CImageBox*				TeamFlagRedImg;
	CImageBox*				TeamFlagGreenImg;
//	CRadioButton*			TeamFlagImg[2]; 
	CTextBox*				CharReadyTxt;
	CButton*				KickBtn;

} SBattleZoneUserList;
*/

	int i = 0;
	
	for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{
		m_WaitingUserInfo[i].FighterState = n_BZ_State_None;
		m_WaitingUserInfo[i].FighterInfo.thePCID = 0;
		m_WaitingUserInfo[i].FighterTeam = n_BattleZone_TeamFlag_Red;

		m_BZ_UserList[i].SlotBGImg->SetClientImage( TID_None );
		m_BZ_UserList[i].NationImg->SetClientImage( TID_None );
		m_BZ_UserList[i].CharImgBtn->SetClientImage( TID_None );
		m_BZ_UserList[i].CharImgBtn->SetIsCharImgRender( false );
		m_BZ_UserList[i].CharImgBtn->SetBtnHold( true );
		m_BZ_UserList[i].CharNameTxt->SetText( _RT( "" ) );
		m_BZ_UserList[i].TeamFlagRedImg->SetClientImage( TID_None );
		m_BZ_UserList[i].TeamFlagGreenImg->SetClientImage( TID_None );
		m_BZ_UserList[i].CharReadyTxt->SetText( _RT( "" ) );
		m_BZ_UserList[i].KickBtn->SetVisible( false );
	}		

	return;
}


void CBZ_WaitingRoomWndProc::JoinedAppearPC( int aSlotIndex )
{	
	PCTABLE addUser;

	addUser.Init();
	addUser.char_info.thePCID = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.thePCID;
	memcpy( addUser.char_info.thePCName, m_WaitingUserInfo[ aSlotIndex ].FighterInfo.thePCName, sizeof(addUser.char_info.thePCName) ); 
	addUser.char_info.theLevel = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theLevel;
	addUser.char_info.theCitizen = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theCitizen;
	addUser.char_info.theGender = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theGender;
	addUser.char_info.theFace = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theFace;
	addUser.char_info.theHair = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theHair;
	addUser.char_info.theMainClass = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theMainClass;
	
	// 다른 유저 체력 표시 
	addUser.char_info.theHP = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theHP;
	addUser.char_info.theSMP = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theSMP;
	addUser.char_info.theMaxHP = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theMaxHP;
	addUser.char_info.theMaxSMP = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theMaxSMP;	
	addUser.char_info.theRealHP = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theHP;
	
	int i = 0;
	
	for( i = 0; i < RP_MAX_EQUIP; ++i )
	{
		addUser.char_info.theWearItem[i] = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theWearItem[i];
	}
	
	addUser.nIndex = g_Pc.GetAttrIndex( m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theCitizen, m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theGender );
	addUser.lCurMap = g_Pc.GetMainPC().lCurMap;
	addUser.lBlock = addUser.lDestBlock = m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theCell;

	if( m_RoomInfo.theGameType == n_Survival )
	{
		g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ aSlotIndex ].FighterTeam = aSlotIndex;
	}

	addUser.nFlag = g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ aSlotIndex ].FighterTeam;

	if( m_WaitingUserInfo[ aSlotIndex ].FighterInfo.thePCID == g_Pc.m_MainPC.lUnique )
	{
		g_Pc.m_MainPC.nFlag = g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ aSlotIndex ].FighterTeam;

//		m_SelfSlotIdx = aSlotIndex;
	}
		
	int pc_index = g_Pc.AppearPC( m_WaitingUserInfo[ aSlotIndex ].FighterInfo.thePCID, addUser );
	if( pc_index == MAX_USER_INTABLE )
	{
		return;
	}

	BOOL create_img = g_Pc_Manager.m_CharImage.Create( m_WaitingUserInfo[ aSlotIndex ].FighterInfo.thePCID, c_PcRace_User, g_lpDevice );
	
	if( create_img )
	{
		if( g_Pc_Manager.m_CharImage.Find( m_WaitingUserInfo[ aSlotIndex ].FighterInfo.thePCID, c_PcRace_User ) )
		{
			m_BZ_UserList[ aSlotIndex ].CharImgBtn->SetClientImage( TID_None );
			m_BZ_UserList[ aSlotIndex ].CharImgBtn->SetIsCharImgRender( true );
			m_BZ_UserList[ aSlotIndex ].CharImgBtn->SetBtnHold( false );

			SRect BtnSize;
			m_BZ_UserList[ aSlotIndex ].CharImgBtn->GetClientSize( &BtnSize );


			// 임시방편 
//			if( m_WaitingUserInfo[ aSlotIndex ].FighterInfo.theCitizen == g_Pc.m_MainPC.char_info.theCitizen )
//			{
				m_BZ_UserList[ aSlotIndex ].CharImgBtn->SetRenderCharImgProperty( BtnSize.x, BtnSize.y, 
																				m_WaitingUserInfo[ aSlotIndex ].FighterInfo.thePCID, 
																				c_PcRace_User, CHARIMAGE_32SIZE );			
//			}
//			else
//			{
//				m_BZ_UserList[ aSlotIndex ].CharImgBtn->SetRenderCharImgProperty( BtnSize.x, BtnSize.y, 
//																				g_Pc.m_MainPC.lUnique, 
//																				c_PcRace_User, CHARIMAGE_32SIZE );			
//			}
		}
	}

//	g_Pc_Manager.m_CharImage.Change( m_WaitingUserInfo[ i ].FighterInfo.thePCID, c_PcRace_User , g_lpDevice );
	
	return;
}


void CBZ_WaitingRoomWndProc::OutDisappearPC( int aSlotIndex )
{
	g_Pc_Manager.m_CharImage.Delete( m_WaitingUserInfo[ aSlotIndex ].FighterInfo.thePCID, c_PcRace_User );

	m_BZ_UserList[ aSlotIndex ].CharImgBtn->SetClientImage( TID_None );
	m_BZ_UserList[ aSlotIndex ].CharImgBtn->SetIsCharImgRender( false );
	m_BZ_UserList[ aSlotIndex ].CharImgBtn->SetBtnHold( true );
	
	g_Pc.DisappearPC( m_WaitingUserInfo[ aSlotIndex ].FighterInfo.thePCID );
	
	return;
}


void CBZ_WaitingRoomWndProc::JoinedUserListSlot( SRpdOtherPC aPcInfo, int aTeamInfo, NBZ_UserState aUserState )
{
	int i = 0;
	char temp[255] = {0,};

	for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{
		if( m_WaitingUserInfo[ i ].FighterState == n_BZ_State_None ) 
		{	
			m_WaitingUserInfo[ i ].FighterInfo = aPcInfo;
			m_WaitingUserInfo[ i ].FighterTeam = aTeamInfo;
			m_WaitingUserInfo[ i ].FighterState = aUserState; // n_BZ_State_Wait;

			m_BZ_UserList[i].SlotBGImg->SetClientImage( TID_GAMEREADY_CHARSLOT_IMG );
			
			if( m_WaitingUserInfo[i].FighterInfo.theCitizen == RAIN )
			{
				m_BZ_UserList[i].NationImg->SetClientImage( TID_NATIONFLAG_RAIN_IMG );
			}
			else if( m_WaitingUserInfo[i].FighterInfo.theCitizen == MILLENA )
			{
				m_BZ_UserList[i].NationImg->SetClientImage( TID_NATIONFLAG_MILLENA_IMG );
			}
			
			JoinedAppearPC( i );	

			char class_name[255] = {0,};

			GetClassText( m_WaitingUserInfo[i].FighterInfo.theMainClass, class_name );
			 
			Rsprintf( RWCHAR(temp), _RT("%s [Lv%d %s]"), RWCHAR(m_WaitingUserInfo[i].FighterInfo.thePCName), m_WaitingUserInfo[i].FighterInfo.theLevel, RWCHAR(class_name) ); 
 
			m_BZ_UserList[i].CharNameTxt->SetText( temp );		

			if( m_WaitingUserInfo[i].FighterInfo.thePCID == m_MasterID )
			{
				m_BZ_UserList[i].CharNameTxt->SetTextColor( 150, 150, 255 );
				m_BZ_UserList[i].CharReadyTxt->SetTextColor( 150, 150, 255 );
			}
			else
			{
				m_BZ_UserList[i].CharNameTxt->SetTextColor( 255, 255, 255 );
				m_BZ_UserList[i].CharReadyTxt->SetTextColor( 255, 255, 255 );
			}
			
			if( m_RoomInfo.theGameType != n_Survival )
			{
				if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Nation )
				{
					if( m_WaitingUserInfo[i].FighterInfo.theCitizen == RAIN )
					{
						m_BZ_UserList[i].TeamFlagRedImg->SetClientImage( TID_None );
						m_BZ_UserList[i].TeamFlagGreenImg->SetClientImage( TID_TEAMSTATE_GREEN_IMG );
					}
					else if( m_WaitingUserInfo[i].FighterInfo.theCitizen == MILLENA )
					{
						m_BZ_UserList[i].TeamFlagRedImg->SetClientImage( TID_TEAMSTATE_RED_IMG );
						m_BZ_UserList[i].TeamFlagGreenImg->SetClientImage( TID_None );
					}
				}
				else
				{
					if( m_WaitingUserInfo[i].FighterTeam == n_BattleZone_TeamFlag_Red )
					{
						m_BZ_UserList[i].TeamFlagRedImg->SetClientImage( TID_TEAMSTATE_RED_IMG );
						m_BZ_UserList[i].TeamFlagGreenImg->SetClientImage( TID_None );
					}
					else if( m_WaitingUserInfo[i].FighterTeam == n_BattleZone_TeamFlag_Green )
					{
						m_BZ_UserList[i].TeamFlagRedImg->SetClientImage( TID_None );
						m_BZ_UserList[i].TeamFlagGreenImg->SetClientImage( TID_TEAMSTATE_GREEN_IMG );
					}
				}
			}

			if( m_WaitingUserInfo[i].FighterInfo.thePCID == m_MasterID )
			{
				m_BZ_UserList[i].CharReadyTxt->SetText( G_STRING_CHAR( IDS_WND_COM_START ) );				
			}
			else
			{
				if( m_WaitingUserInfo[i].FighterState == n_BZ_State_Wait )
				{
					m_BZ_UserList[i].CharReadyTxt->SetText( _RT( "" ) );
				}	
				else if( m_WaitingUserInfo[i].FighterState == n_BZ_State_Ready )
				{
					m_BZ_UserList[i].CharReadyTxt->SetText( G_STRING_CHAR( IDS_WND_READY ) );
				}	
			}
			
			if( m_bIsMaster )
			{
				// 자기 자신이 방장인데...자신은 강퇴 없음...^ ^;;
				if( m_WaitingUserInfo[i].FighterInfo.thePCID != m_MasterID )
				{
					m_BZ_UserList[i].KickBtn->SetVisible( true );							
				}

				m_pMapComboBox->SetFocusCheck( true );
			}

			break;
		}
	}		

	return;
}


void CBZ_WaitingRoomWndProc::OutUserListSlot( DWORD aPCID )
{
	int i = 0;
	
	for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{
		if( m_WaitingUserInfo[i].FighterState != n_BZ_State_None &&
			m_WaitingUserInfo[i].FighterInfo.thePCID == aPCID )
		{	
			m_WaitingUserInfo[i].FighterState = n_BZ_State_None;
			m_WaitingUserInfo[i].FighterInfo.thePCID = 0;
			m_WaitingUserInfo[i].FighterTeam = n_BattleZone_TeamFlag_Red;

			OutDisappearPC( i );

			m_BZ_UserList[i].SlotBGImg->SetClientImage( TID_None );
			m_BZ_UserList[i].NationImg->SetClientImage( TID_None );			
			m_BZ_UserList[i].CharImgBtn->SetClientImage( TID_None );
			m_BZ_UserList[i].CharImgBtn->SetIsCharImgRender( false );
			m_BZ_UserList[i].CharImgBtn->SetBtnHold( true );
			m_BZ_UserList[i].CharNameTxt->SetText( _RT( "" ) );
			m_BZ_UserList[i].TeamFlagRedImg->SetClientImage( TID_None );
			m_BZ_UserList[i].TeamFlagGreenImg->SetClientImage( TID_None );
			m_BZ_UserList[i].CharReadyTxt->SetText( _RT( "" ) );
			m_BZ_UserList[i].KickBtn->SetVisible( false );
		}
	}		

	return;
}


void CBZ_WaitingRoomWndProc::SetChangeRoomMaster( DWORD aMasterID )
{		
	int i = 0;

	for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{
		if( m_WaitingUserInfo[i].FighterInfo.thePCID == m_MasterID )
		{			
			m_BZ_UserList[i].CharNameTxt->SetTextColor( 255, 255, 255 );	
			m_BZ_UserList[i].CharReadyTxt->SetTextColor( 255, 255, 255 );

			if( m_WaitingUserInfo[i].FighterState == n_BZ_State_Wait )
			{
				m_BZ_UserList[i].CharReadyTxt->SetText( _RT( "" ) );
			}	
			else if( m_WaitingUserInfo[i].FighterState == n_BZ_State_Ready )
			{
				m_BZ_UserList[i].CharReadyTxt->SetText( G_STRING_CHAR( IDS_WND_READY ) );
			}				
			break;
		}
	}

	m_MasterID = aMasterID;
	
	if( g_Pc.m_MainPC.lUnique == aMasterID )
	{
		m_bIsMaster = TRUE;
		m_bIsReady = FALSE;

		m_pReadyORStartBtn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) );

		for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
		{
			if( m_WaitingUserInfo[i].FighterState != n_BZ_State_None )
			{
				if( m_WaitingUserInfo[i].FighterInfo.thePCID == g_Pc.m_MainPC.lUnique )
				{
					m_BZ_UserList[i].KickBtn->SetVisible( false );							
				}
				else
				{
					m_BZ_UserList[i].KickBtn->SetVisible( true );							
				}
			}
		}

		m_pMapComboBox->SetFocusCheck( true );
	}
	else
	{
		m_bIsMaster = FALSE;

		m_pReadyORStartBtn->SetText( G_STRING_CHAR( IDS_WND_READY ) );

		for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
		{
			if( m_WaitingUserInfo[i].FighterInfo.thePCID != m_MasterID )
			{
				m_BZ_UserList[i].KickBtn->SetVisible( false );							
			}
		}

		m_pMapComboBox->SetFocusCheck( false );
	}

	for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{
		if( m_WaitingUserInfo[i].FighterState != n_BZ_State_None &&
			m_WaitingUserInfo[i].FighterInfo.thePCID == aMasterID )
		{			
			m_BZ_UserList[i].CharNameTxt->SetTextColor( 150, 150, 255 );
			m_BZ_UserList[i].CharReadyTxt->SetTextColor( 150, 150, 255 );
			m_BZ_UserList[i].CharReadyTxt->SetText( G_STRING_CHAR( IDS_WND_COM_START ) );
			break;
		}
	}

	return;
}

void CBZ_WaitingRoomWndProc::SetChangeSelectTeam( DWORD aPCID, int aTeamFlag )
{	
	int i = 0;

	for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{
		if( m_WaitingUserInfo[i].FighterInfo.thePCID == aPCID )
		{	
			if( m_WaitingUserInfo[i].FighterInfo.thePCID == g_Pc.m_MainPC.lUnique )
			{
				g_Pc.m_MainPC.nFlag = aTeamFlag;
			}			

			m_WaitingUserInfo[i].FighterTeam = aTeamFlag;

			if( m_WaitingUserInfo[i].FighterTeam == n_BattleZone_TeamFlag_Red )
			{
				m_BZ_UserList[i].TeamFlagRedImg->SetClientImage( TID_TEAMSTATE_RED_IMG );
				m_BZ_UserList[i].TeamFlagGreenImg->SetClientImage( TID_None );
			}
			else if( m_WaitingUserInfo[i].FighterTeam == n_BattleZone_TeamFlag_Green )
			{
				m_BZ_UserList[i].TeamFlagRedImg->SetClientImage( TID_None );
				m_BZ_UserList[i].TeamFlagGreenImg->SetClientImage( TID_TEAMSTATE_GREEN_IMG );
			}			
			break;
		}
	}

	return;
}
	
void CBZ_WaitingRoomWndProc::SetChangeFightReady( DWORD aPCID, int aStateFlag )
{	
	int i = 0;
		
	for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
	{		
		if( g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ i ].FighterState != n_BZ_State_None )
		{
			if( g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ i ].FighterInfo.thePCID == aPCID	)
			{
				if( aStateFlag == n_Success )
				{
					m_WaitingUserInfo[ i ].FighterState = n_BZ_State_Ready;

					m_BZ_UserList[ i ].CharReadyTxt->SetText( G_STRING_CHAR( IDS_WND_READY ) );
				}
				else
				{
					g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ i ].FighterState = n_BZ_State_Wait;

					m_BZ_UserList[ i ].CharReadyTxt->SetText( _RT( "" ) );
				}				
				break;
			}			
		}		
	}

	return;
}

void CBZ_WaitingRoomWndProc::SetChangeRoomInfo( BYTE aMapCode, BYTE aTimeOut, BYTE aItemFlag, char* aRoomName )
{
	char strTemp[128] = {0,};

	if( aRoomName )
	{
		SAFE_STR_CPY( m_RoomInfo.theName, aRoomName, 40 );

		Rsprintf(RWCHAR(strTemp), _RT("[%d] %s") , m_WaitingRoomID, RWCHAR(m_RoomInfo.theName) ); 
		m_pRoomNameTxt->SetText( strTemp );
	}

	m_RoomInfo.theMapCode = aMapCode;
	m_RoomInfo.theTimeOut = aTimeOut;
	m_RoomInfo.theItemFlag = aItemFlag;

	GetAreaStrName( strTemp, m_RoomInfo.theMapCode ); 
	m_pMapComboBox->SetSelectText( strTemp );
	m_pMiniMapImg->SetClientImage( G_MAP_MINIMAPID(aMapCode) );			

	int i = 0;

	for( i = 0; i < c_Max_OptionBtn_Time_Num; ++i )
	{
		if( g_BZ_CreateRoomWnd.m_MaxGameTimeValue[ i ] == aTimeOut )
		{
			m_MaxGameTime = (NBattleZoneGameTime) i;
			nRui->SendEvent( m_pMaxTimeRBtn[m_MaxGameTime]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			break;
		}
	}

	for( i = 0; i < c_Max_OptionBtn_Item_Num; ++i )
	{
		if( g_BZ_CreateRoomWnd.m_UseItemValue[ i ] == aItemFlag )
		{
			m_UseItem = (NBattleZoneUseItem) i;
			nRui->SendEvent( m_pUseItemRBtn[m_UseItem]->GetID(), n_emSetChecked, 0, 0, 0, 0 );
			break;
		}
	}

	return;
}



void CBZ_WaitingRoomWndProc::SendChangeRoomInfo()
{
	m_RoomInfo.theTimeOut = g_BZ_CreateRoomWnd.m_MaxGameTimeValue[ m_MaxGameTime ];
	m_RoomInfo.theItemFlag = g_BZ_CreateRoomWnd.m_UseItemValue[ m_UseItem ];
	
	g_RockClient.Send_CsChangeFightInfo( m_RoomInfo.theMapCode, m_RoomInfo.theTimeOut, m_RoomInfo.theItemFlag );

	return;
}

void CBZ_WaitingRoomWndProc::SetChatMsgScroll( int aLineNum )
{	
	m_StartLineNum_ChatMsg = aLineNum * 1;

	SetRefreshChatMsg();

	return;
}


//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================

///---------------------------------------------------------------------------
///-- Constructor
///---------------------------------------------------------------------------
CBZ_GameResultWndProc::CBZ_GameResultWndProc()
{
	Init();
}
///---------------------------------------------------------------------------
///-- Destructror
///---------------------------------------------------------------------------
CBZ_GameResultWndProc::~CBZ_GameResultWndProc()
{
}
///---------------------------------------------------------------------------
///-- Init
///---------------------------------------------------------------------------
void CBZ_GameResultWndProc::Init()
{	
	return;
}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CBZ_GameResultWndProc::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect wnd_rect;
	wnd_rect.w	= 237;
	wnd_rect.h	= 217;
	wnd_rect.x	= center_x - ( wnd_rect.w / 2 );
	wnd_rect.y	= center_y - ( wnd_rect.h / 2 ) - 50;
	
	///-- Frame Window 
	m_pFrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_BZ_GAMERESULT_WND, WID_None, wnd_rect.x, wnd_rect.y, wnd_rect.w, wnd_rect.h, false );
	m_pFrameWnd->SetClientImage( TID_None );
	m_pFrameWnd->SetCaption( _RT( "" ) );
	m_pFrameWnd->SetTitle( false );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();
	m_pFrameWnd->SetMove( false );

	m_pWinImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_GAMERESULT_WND, 4, 6, 227, 71, false );
	m_pWinImg->SetClientImage( TID_GAMERESULT_WIN_IMG );

	m_pLostImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_GAMERESULT_WND, 7, 33, 225, 43, false );
	m_pLostImg->SetClientImage( TID_GAMERESULT_LOST_IMG );

	m_pDrawImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_BZ_GAMERESULT_WND, 7, 33, 225, 44, false );
	m_pDrawImg->SetClientImage( TID_GAMERESULT_DRAW_IMG );

	m_pResultImg = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_BZ_GAMERESULT_WND, 8, 83, 222, 124 + 50, false );	
	m_pResultImg->SetWndTileSizeLR( 100, 100 );
	m_pResultImg->SetWndTileSizeTB( 50, 50 );
	m_pResultImg->SetClientImage( TID_GAMERESULT_WND_BG_L1 );		
	m_pResultImg->SetAdjustPosition( 0, 0 );

	//TID_GAMERESULT_WND_BG_L1

	m_pContributionTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_GAMERESULT_WND, 30, 105, 90, 25, false );
	m_pContributionTxt->SetFontg( n_ftGulimChe, 17, n_fwBold, n_fdNormal );
	m_pContributionTxt->SetClientImage( TID_None );	
	m_pContributionTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pContributionTxt->SetTextColor( 255, 255, 255 );
	m_pContributionTxt->SetText( G_STRING_CHAR( IDS_GAMERESULT_CONTRIBUTION ) );		
	m_pContributionTxt->SetFocusCheck( false );

	m_pContributionValueTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_GAMERESULT_WND, 30, 130, 70, 25, false );
	m_pContributionValueTxt->SetFontg( n_ftGulimChe, 17, n_fwBold, n_fdNormal );
	m_pContributionValueTxt->SetClientImage( TID_None );	
	m_pContributionValueTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pContributionValueTxt->SetTextColor( 255, 255, 255 );
	m_pContributionValueTxt->SetText( _RT( "+ 0" ) );	
	m_pContributionValueTxt->SetFocusCheck( false );

	m_pRespectTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_GAMERESULT_WND, 30, 130, 90, 25, false );
	m_pRespectTxt->SetFontg( n_ftGulimChe, 17, n_fwBold, n_fdNormal );
	m_pRespectTxt->SetClientImage( TID_None );	
	m_pRespectTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pRespectTxt->SetTextColor( 255, 255, 255 );
	m_pRespectTxt->SetText( G_STRING_CHAR( IDS_GAMERESULT_RESPECT ) );	
	m_pRespectTxt->SetFocusCheck( false );

	m_pRespectValueTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_GAMERESULT_WND, 149, 130, 70, 25, false );
	m_pRespectValueTxt->SetFontg( n_ftGulimChe, 17, n_fwBold, n_fdNormal );
	m_pRespectValueTxt->SetClientImage( TID_None );	
	m_pRespectValueTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pRespectValueTxt->SetTextColor( 255, 255, 255 );
	m_pRespectValueTxt->SetText( _RT( "+ 0" ) );	
	m_pRespectValueTxt->SetFocusCheck( false );

	m_pExpTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_GAMERESULT_WND, 30, 155, 90, 25, false );
	m_pExpTxt->SetFontg( n_ftGulimChe, 17, n_fwBold, n_fdNormal );
	m_pExpTxt->SetClientImage( TID_None );	
	m_pExpTxt->SetAlignText( n_atCenter, n_atCenter );
	m_pExpTxt->SetTextColor( 255, 255, 255 );
	m_pExpTxt->SetText( G_STRING_CHAR( IDS_CHARSTATE_EXP ) );	
	m_pExpTxt->SetFocusCheck( false );

	m_pExpValueTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_GAMERESULT_WND, 149, 155, 70, 25, false );
	m_pExpValueTxt->SetFontg( n_ftGulimChe, 17, n_fwBold, n_fdNormal );
	m_pExpValueTxt->SetClientImage( TID_None );	
	m_pExpValueTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pExpValueTxt->SetTextColor( 255, 255, 255 );
	m_pExpValueTxt->SetText( _RT( "+ 0" ) );	
	m_pExpValueTxt->SetFocusCheck( false );

	m_pEldTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_GAMERESULT_WND, 30, 195, 90, 25, false );
	m_pEldTxt->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_pEldTxt->SetClientImage( TID_None );	
	m_pEldTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pEldTxt->SetTextColor( 200, 200, 255 );
	m_pEldTxt->SetText( G_STRING_CHAR( IDS_SYSPR_GET_ELD ) );	
	m_pEldTxt->SetFocusCheck( false );

	m_pPresentItemTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_BZ_GAMERESULT_WND, 30, 215, 90, 25, false );
	m_pPresentItemTxt->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_pPresentItemTxt->SetClientImage( TID_None );	
	m_pPresentItemTxt->SetAlignText( n_atLeft, n_atCenter );
	m_pPresentItemTxt->SetTextColor( 200, 200, 255 );
	m_pPresentItemTxt->SetText( G_STRING_CHAR( IDS_SYSPR_GET_ITEM ) );			
	m_pPresentItemTxt->SetFocusCheck( false );	

	return;
}

///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CBZ_GameResultWndProc::Update()
{		
	if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( m_pViewTime , 5500 ) ) )
	{
		Close();

		m_pViewTime = g_nowTime;
	}

	return;
}

///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CBZ_GameResultWndProc::Proc( SEventMessage* EMsg )
{	
	return;
}



void CBZ_GameResultWndProc::Open( BYTE aResult, DWORD aFame, DWORD aContribution, DWORD aExp, DWORD aEld, WORD aItemCode, BYTE aItemNum )
{	
	nRui->SendEvent( WID_AliveWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

	nRui->SendEvent( WID_BZ_GAMERESULT_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
	m_pContributionTxt->SetVisible( false ); 
	m_pContributionValueTxt->SetVisible( false ); 	
	m_pRespectTxt->SetVisible( false ); 
	m_pRespectValueTxt->SetVisible( false ); 
	m_pExpTxt->SetVisible( false ); 
	m_pExpValueTxt->SetVisible( false ); 
	m_pEldTxt->SetVisible( false ); 
	m_pPresentItemTxt->SetVisible( false ); 
	//모든정보가 0이 들어오면 .. 
	if( !aFame && !aContribution && !aExp && !aEld && !aItemCode)
	{
		m_pResultImg->SetVisible(false);
	}
	else 
	{
		m_pResultImg->SetVisible(true);			
	}

	
	char str_temp[64] = {0,};
	
	if( aResult == 1 )		// WIN 
	{
		m_pWinImg->SetClientImage( TID_GAMERESULT_WIN_IMG);
		m_pLostImg->SetClientImage( TID_None );		
		m_pDrawImg->SetClientImage( TID_None );				
		
		if( aFame >= 0 )
		{
			Rsprintf( RWCHAR(str_temp), _RT("+ %d"), aFame );
		}
		else
		{
			Rsprintf( RWCHAR(str_temp), _RT("- %d"), aFame );
		}
		m_pRespectValueTxt->SetText( str_temp );
		
		if( aFame >= 0 )
		{
			Rsprintf( RWCHAR(str_temp), _RT("+ %d"), aContribution );	
		}
		else
		{
			Rsprintf( RWCHAR(str_temp), _RT("- %d"), aContribution );	
		}
		m_pContributionValueTxt->SetText( str_temp );
		
		if( aExp >= 0 )
		{
			Rsprintf( RWCHAR(str_temp), _RT("+ %d"), aExp );	
		}
		else
		{
			Rsprintf( RWCHAR(str_temp), _RT("- %d"), aExp );	
		}
		m_pExpValueTxt->SetText( str_temp );
		
		if( aEld > 0 )
		{
			Rsprintf( RWCHAR(str_temp), G_STRING_CHAR( IDS_SYSPR_GET_ELD ) , aEld );	
			
			m_pEldTxt->SetText( str_temp );
		}
		else
		{
			m_pEldTxt->SetText( _RT( "" ) );
		}
		
		if( aItemCode )
		{
			SItemBaseInfo* item_Info = NULL;
			item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode );			
			
			Rsprintf( RWCHAR(str_temp),  G_STRING_CHAR( IDS_WND_ITEM_MSG_GET ) , RWCHAR(item_Info->theName) );	
			//		sprintf( str_temp, _RT( G_STRING_CHAR( IDS_SYSPR_GET_ITEM ) ), item_Info->theName, aItemNum );	
			
			m_pPresentItemTxt->SetText( str_temp );
		}
		else
		{
			m_pPresentItemTxt->SetText( _RT( "" ) );
		}
		
		if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_DeathMatch ) 
		{
			if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Nation )
			{
				// 국가 공헌도, 명성치, 경험치 
				int nPos = 105;

				if( 0 !=aContribution )
				{
					m_pContributionTxt->SetWndRepositionR( 30, nPos );		///1
					m_pContributionValueTxt->SetWndRepositionR( 149, nPos );
					m_pContributionTxt->SetVisible( true );
					m_pContributionValueTxt->SetVisible( true );
					
					nPos += 25;
				}

				if( 0 != aFame )
				{
					m_pRespectTxt->SetWndRepositionR( 30, nPos );			///2
					m_pRespectValueTxt->SetWndRepositionR( 149, nPos );
					m_pRespectTxt->SetVisible( true );
					m_pRespectValueTxt->SetVisible( true );

					nPos += 25;
				}				

				if( 0 != aExp )
				{	
					m_pExpTxt->SetWndRepositionR( 30, nPos );
					m_pExpValueTxt->SetWndRepositionR( 149, nPos );
					m_pExpTxt->SetVisible( true );
					m_pExpValueTxt->SetVisible( true );
					
					nPos += 25;
				}

				nPos += 10;
				
				if( 0 != aEld )
				{
					m_pEldTxt->SetWndRepositionR( 30, nPos );
					m_pEldTxt->SetVisible( true );

					nPos += 25;
				}		
				
				m_pPresentItemTxt->SetWndRepositionR( 30, nPos );				
				m_pPresentItemTxt->SetVisible( true );			
			}
			else // n_BattleZone_Free
			{
				int nPos = 105;
				
				if( 0 != aExp )
				{	
					m_pExpTxt->SetWndRepositionR( 30, nPos );
					m_pExpValueTxt->SetWndRepositionR( 149, nPos );
					m_pExpTxt->SetVisible( true );
					m_pExpValueTxt->SetVisible( true );
					
					nPos += 25;
				}
				
				nPos += 10;
				
				if( 0 != aEld )
				{
					m_pEldTxt->SetWndRepositionR( 30, nPos );
					m_pEldTxt->SetVisible( true );

					nPos += 25;
				}
				
				m_pPresentItemTxt->SetWndRepositionR( 30, nPos );				
				m_pPresentItemTxt->SetVisible( true );			
			}
		}
		else if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_Survival ) 
		{
			if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Free )
			{
				int nPos = 105;

				if( 0 != aEld )
				{
					m_pEldTxt->SetWndRepositionR( 30, nPos );
					m_pEldTxt->SetVisible( true );

					nPos += 25;
				}
			}
		}
		else if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_CapturetheHill ) 
		{
			if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Nation )
			{
				// 국가 공헌도, 명성치, 경험치 
				int nPos = 105;

				if( 0 !=aContribution )
				{
					m_pContributionTxt->SetWndRepositionR( 30, nPos );		///1
					m_pContributionValueTxt->SetWndRepositionR( 149, nPos );
					m_pContributionTxt->SetVisible( true );
					m_pContributionValueTxt->SetVisible( true );
					
					nPos += 25;
				}

				if( 0 != aFame )
				{
					m_pRespectTxt->SetWndRepositionR( 30, nPos );			///2
					m_pRespectValueTxt->SetWndRepositionR( 149, nPos );
					m_pRespectTxt->SetVisible( true );
					m_pRespectValueTxt->SetVisible( true );

					nPos += 25;
				}				

				if( 0 != aExp )
				{	
					m_pExpTxt->SetWndRepositionR( 30, nPos );
					m_pExpValueTxt->SetWndRepositionR( 149, nPos );
					m_pExpTxt->SetVisible( true );
					m_pExpValueTxt->SetVisible( true );
					
					nPos += 25;
				}

				nPos += 10;
				
				if( 0 != aEld )
				{
					m_pEldTxt->SetWndRepositionR( 30, nPos );
					m_pEldTxt->SetVisible( true );

					nPos += 25;
				}		
				
				m_pPresentItemTxt->SetWndRepositionR( 30, nPos );				
				m_pPresentItemTxt->SetVisible( true );			
			}
			else // n_BattleZone_Free
			{
				int nPos = 105;				
				
				if( 0 != aExp )
				{	
					m_pExpTxt->SetWndRepositionR( 30, nPos );
					m_pExpValueTxt->SetWndRepositionR( 149, nPos );
					m_pExpTxt->SetVisible( true );
					m_pExpValueTxt->SetVisible( true );
					
					nPos += 25;
				}
				
				nPos += 10;
				
				if( 0 != aEld )
				{
					m_pEldTxt->SetWndRepositionR( 30, nPos );
					m_pEldTxt->SetVisible( true );

					nPos += 25;
				}
				
				m_pPresentItemTxt->SetWndRepositionR( 30, nPos );				
				m_pPresentItemTxt->SetVisible( true );				
			}
		} 
		/*	else if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_CapturetheFlag ) 
		{
		if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Nation )
		{
		}
		else // n_BattleZone_Free
		{
		}
		}
		else if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_PointMatch ) 
		{
		if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Nation )
		{
		}
		else // n_BattleZone_Free
		{
		}
		}
		*/	


	}
	else if( aResult == 2 )		// DRAW
	{
		m_pWinImg->SetClientImage( TID_None );
		m_pLostImg->SetClientImage( TID_None );		
		m_pDrawImg->SetClientImage( TID_GAMERESULT_DRAW_IMG );				

		if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_Survival ) 
		{
			if( g_BZ_SelectZoneWnd.m_BZ_ZoneType == n_BattleZone_Free )
			{
				int nPos = 105;

				if( 0 != aEld )
				{
					m_pEldTxt->SetWndRepositionR( 30, nPos );
					m_pEldTxt->SetVisible( true );					
				}
			}
		}
		else
		{
			if( aFame >= 0 )
			{
				Rsprintf( RWCHAR(str_temp), _RT("+ %d"), aFame );
			}
			else
			{
				Rsprintf( RWCHAR(str_temp), _RT("- %d"), aFame );
			}
			
			int nPos = 105;

			if( 0 != aFame )
			{
				m_pRespectTxt->SetWndRepositionR( 30, nPos );			///2
				m_pRespectValueTxt->SetWndRepositionR( 149, nPos );
				m_pRespectTxt->SetVisible( true );
				m_pRespectValueTxt->SetVisible( true );

				nPos += 25;
			}	
		}
	}
	else						// LOSE
	{
		m_pWinImg->SetClientImage( TID_None );
		m_pLostImg->SetClientImage( TID_GAMERESULT_LOST_IMG );		
		m_pDrawImg->SetClientImage( TID_None );				
 
		if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_Survival ) 
		{
		}
		else
		{
			if( aFame >= 0 )
			{
				Rsprintf( RWCHAR(str_temp), _RT("+ %d"), aFame );
			}
			else
			{
				Rsprintf( RWCHAR(str_temp), _RT("- %d"), aFame );
			}

			int nPos = 105;
		
			if( 0 != aFame )
			{
				m_pRespectTxt->SetWndRepositionR( 30, nPos );			///2
				m_pRespectValueTxt->SetWndRepositionR( 149, nPos );
				m_pRespectTxt->SetVisible( true );
				m_pRespectValueTxt->SetVisible( true );

				nPos += 25;
			}
		}
	}
	 
	m_pViewTime = g_nowTime;
	
	return;
}


void CBZ_GameResultWndProc::Close()
{	
	nRui->SendEvent( WID_BZ_GAMERESULT_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

	return;
}

