#include "Rui.h"
#include "ServerListWndProc.h"
#include "MessageWndProc.h"
#include "StringManager.h"
#include "ScrollBar.h"

CServerListWnd	ServerListWnd;


//-----------------------------------------------------------------------------
void CServerListWnd::Init()
{
	m_ServerListWnd = NULL;
	m_ConnectBtn	= NULL;
	m_CancelBtn		= NULL;
	m_ScrollBtn_svr     = NULL;
	m_iServerTab = 1 ;
	
	m_bRunServerListName = false ; 

	
	for( int i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT ; ++i )
	{
		m_BtnList[ i ] = NULL;		
	}

	m_TopNum = 0;
}

///---------------------------------------------------------------------------
///-- Open_ServerList_Wnd
///---------------------------------------------------------------------------
void CServerListWnd::Open_ServerList_Wnd( )
{
	
	//각탭에 의한 정보 ..
	//서버설정 한부분을 얻어와서 어느탭이니 구분한다. 
	mb_server = true; //리로그인 창을 띄우기 위해서

	m_TopNum    = 0;
	m_SelectNum = -1;
	int i = 0 ;

	if( m_ServerListWnd != NULL )
	{
		nRui->SendEvent( WID_ServerListWnd , n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	}

	///-- 버튼 초기화
	for(i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT ; ++i )
	{
		m_BtnList[ i ]->SetText(_RT(""));		
		m_BtnList[ i ]->SetBtnHold( false );
		m_BtnList[ i ]->SetTextColor( 255, 255, 255 );
		//m_Serve[ i ]->SetVisible( false );
		m_BtnServerTab[ i ]->SetVisible(false);
	}
#ifdef EXPANSION_SERVERTAB
	SServerSetData *pServerSet = g_RockClient.GetServerList();			
	int MaxWorldIndex =0  ;

	for(i = 0 ; i < g_RockClient.GetServSetCount() ; ++i )
	{
		
		//우선 탭의 인덱스로 처음시작하는 월드 인덱스를 찾음 .. 
		if( pServerSet[i].theWorldIndex > MaxWorldIndex )
		{
			MaxWorldIndex = pServerSet[i].theWorldIndex;
		}
	}
     
	for( i = 0 ; i <= MaxWorldIndex ; ++i)
	{
		m_BtnServerTab[ i ]->SetVisible(true);
	}
	
	//유져모드에서는 보여주지 않는다. . 
	bool bflag = false; 

	for(i = 0 ; i < g_RockClient.GetServSetCount() ; ++i )
	{
		//우선 탭의 인덱스로 처음시작하는 월드 인덱스를 찾음 .. 
		if( pServerSet[i].theWorldIndex == 0  )
		{
			if(pServerSet[i].theStatus != 0) 
			{
				bflag = true; 
				break; 
			}
		}
	}

	if(bflag == false )
	{
		m_BtnServerTab[ 0 ]->SetVisible(false);
		m_iServerTab = 1 ;
	}
	

#endif




	///---------------------------------------------------------------------------
	///-- 서버선택창
	
	for( i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT ; ++i )
	{
		if( m_iServerTab == i)
		{
			m_BtnServerTab[ i ]->SetBtnHold(true);
		}
		else
		{
			m_BtnServerTab[ i ]->SetBtnHold(false);
		}
	
#ifdef APPLY_BOLD_FONT
		m_BtnList[ i ]->SetWndRepositionR( 24 + StartX, 50 + 18 * i );
#else		
		m_BtnList[ i ]->SetWndRepositionR( 34 +StartX, 50 + 18 * i );
#endif
	}
	m_ServerListWnd->SetCaption( G_STRING_CHAR( IDS_WND_SVRSEL_SVR ) );
	//mcp_title->SetText(  );
	m_ConnectBtn->SetText( G_STRING_CHAR( IDS_WND_COM_SELECT ) );
	///mcp_sel_server->SetVisible( false );
	///mcp_sersel_img->SetVisible( false );
	///mcp_chasel_img->SetVisible( false );
	m_BtnList[ SERVER_LIST_SELECT_BTN_COUNT -1 ]->SetBtnHold( false );


#ifdef EXPANSION_SERVERTAB
	m_ScrollBtn_svr->SetVisible( false );
#endif 
	
	if( g_RockClient.GetServSetCount() - SERVER_LIST_SELECT_BTN_COUNT > 0 )
	{
		int ExtraLine = g_RockClient.GetServSetCount() - SERVER_LIST_SELECT_BTN_COUNT;			
		m_ScrollBtn_svr->InitSlide( stVertSld , 0 , ExtraLine , 0 , 13 );//Type, min, max, value, 썸의 폭

		m_ScrollBtn_svr->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
		m_ScrollBtn_svr->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
		m_ScrollBtn_svr->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );	
		m_ScrollBtn_svr->SetFocusCheck( true );
	}
	else
	{
		///m_ScrollBtn_svr->SetVisible( FALSE );
		m_ScrollBtn_svr->InitSlide( stVertSld , 0 , 0 , 0 , 13 );//Type, min, max, value, 썸의 폭

		m_ScrollBtn_svr->SetThumbImage( TID_None, TID_None, TID_None, TID_None );
		m_ScrollBtn_svr->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
		m_ScrollBtn_svr->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );	
		m_ScrollBtn_svr->SetFocusCheck( false );
		
		
	}

	/*
	///---------------------------------------------------------------------------
	///-- 채널 선택참

	else
	{
		for( int i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT ; ++i )
		{
			m_BtnList[ i ]->SetWndRepositionR( 34, 68 + 18 * i );
		}
		m_ServerListWnd->SetCaption( G_STRING_CHAR( IDS_WND_SVRSEL_CHN ) );
		//mcp_title->SetText( G_STRING_CHAR( IDS_WND_SVRSEL_CHN ) );
		m_ConnectBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CONNECT ) );		
		mcp_sel_server->SetVisible( true );
		mcp_sersel_img->SetVisible( false );
		mcp_chasel_img->SetVisible( false );
		m_BtnList[ SERVER_LIST_SELECT_BTN_COUNT -1 ]->SetBtnHold( true );
		
		m_ScrollBtn_svr->SetVisible( false );
		m_ScrollBtn_chn->SetVisible( true );
		
		if( g_RockClient.GetChannelCount() - ( SERVER_LIST_SELECT_BTN_COUNT - 1 ) > 0 )
		{
			int ExtraLine = g_RockClient.GetChannelCount() - ( SERVER_LIST_SELECT_BTN_COUNT - 1 );			
			m_ScrollBtn_chn->InitSlide( stVertSld , 0 , ExtraLine , 0 , 13 );//Type, min, max, value, 썸의 폭

			m_ScrollBtn_chn->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
			m_ScrollBtn_chn->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
			m_ScrollBtn_chn->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );	
			m_ScrollBtn_chn->SetFocusCheck( true );
		}
		else
		{
			m_ScrollBtn_chn->InitSlide( stVertSld , 0 , 0 , 0 , 13 );//Type, min, max, value, 썸의 폭

			m_ScrollBtn_chn->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
			m_ScrollBtn_chn->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
			m_ScrollBtn_chn->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
			m_ScrollBtn_chn->SetFocusCheck( false );
		}		
	}
	*/
}

void CServerListWnd::CompositionOld()
{
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect sFrameRect;
	
	sFrameRect.w	= 270;
	sFrameRect.h	= 310;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sFrameRect.w	= 320;
	sFrameRect.h	= 310;

#endif
	
	sFrameRect.x	= center_x - ( sFrameRect.w / 2 );
	sFrameRect.y	= center_y - ( sFrameRect.h / 2 );
	
	//..........................................................................................................
	// 서버 리스트 윈도우
	//..........................................................................................................
	m_ServerListWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ServerListWnd, WID_None, 
												   sFrameRect.x , sFrameRect.y , sFrameRect.w , sFrameRect.h , false );
	m_ServerListWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_ServerListWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_ServerListWnd->SetWndTileSizeLR( 40, 29 );
	m_ServerListWnd->SetWndTileSizeTB( 31, 11 );	
		
	m_ServerListWnd->SetWndProc( this );	
	m_ServerListWnd->SetTitle( true );
	
	m_ServerListWnd->Initialize();
	m_ServerListWnd->SetVisible( FALSE );
	m_ServerListWnd->SetMove( FALSE );
	
	SRect sImageRect;
	
	sImageRect.w	= 233;
	sImageRect.h	= 230;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sImageRect.w	= 290;
	sImageRect.h	= 230;

#endif	
	
	///-- Image 
	mcp_black_back9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_ServerListWnd, 
													15, 40, sImageRect.w, sImageRect.h, false );       	
	mcp_black_back9->SetWndTileSizeLR( 9, 9 );
	mcp_black_back9->SetWndTileSizeTB( 8, 8 );
	mcp_black_back9->SetFocusCheck( false );
	mcp_black_back9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	mcp_black_back9->SetAdjustPosition( 0, 0 );

	//..........................................................................................................
	// 선택 버튼 SERVER_LIST_SELECT_BTN_COUNT 개 
	//..........................................................................................................

	SRect sBtnPos;	

	for( int i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT; ++i )
	{
		sBtnPos.x = 34;
		sBtnPos.y = 55 + ( 18 * i );
		sBtnPos.w = 198;
		sBtnPos.h = 15;	

#ifdef APPLY_BOLD_FONT		
			
		sBtnPos.x = 24;	
		
#endif	
		
#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
		sBtnPos.w = 248;

#endif	

		m_BtnList[ i ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , WID_ServerListWnd , 
													sBtnPos.x , sBtnPos.y , sBtnPos.w , sBtnPos.h , false );
		m_BtnList[ i ]->SetClientImage( TID_None );
		m_BtnList[ i ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );	
		m_BtnList[ i ]->SetAlignText( n_atLeft , n_atCenter );
	}
	
	//서버 탭버튼 
	char StrServer[256]="";

	SRect sTabPos;
	
	for(short i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT; ++i )
	{		
		sTabPos.x = 20 + ( 124 * i );
		sTabPos.y = 25;
		sTabPos.w = 124;
		sTabPos.h = 16;		
		
		m_BtnServerTab[ i ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , WID_ServerListWnd , 
														 sTabPos.x , sTabPos.y , sTabPos.w , sTabPos.h , false );
		m_BtnServerTab[ i ]->SetBtnImage( TID_CTRL_BTN58,  TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_OVR , TID_None );
		m_BtnServerTab[ i ]->SetFontR( n_fRsvFontWndButton ); 
		m_BtnServerTab[ i ]->SetAlignText( n_atCenter , n_atCenter );
		Rsprintf( RWCHAR(StrServer) , _RT("%d"), G_STRING_CHAR(IDS_WND_SERVERNAME00 + i));
		m_BtnServerTab[ i ]->SetText(StrServer);
		m_BtnServerTab[ i ]->SetVisible(false);	
	}

	SRect sTxtPos;

	for(short i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT; ++i )
	{

		sTxtPos.x = 180;
		sTxtPos.y = 48 + ( 18 * i );
		sTxtPos.w = 50;
		sTxtPos.h = 17;	

#ifdef APPLY_BOLD_FONT	

		sTxtPos.x = 190;
#endif	

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
		sTxtPos.x = 195;

#endif	

		m_Statetext[ i ] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None , WID_ServerListWnd , 
													   sTxtPos.x , sTxtPos.y , sTxtPos.w , sTxtPos.h , false );

		m_Statetext[ i ]->SetFontR( n_fRsvFontWndStatic2 );				
	}
	
	//..........................................................................................................
	// 선택 hightlight ( Quest_List )
	//..........................................................................................................

	sImageRect.x	= 34;
	sImageRect.y	= 55;
	sImageRect.w	= 205;
	sImageRect.h	= 15;
	
#ifdef APPLY_BOLD_FONT

	sImageRect.x	= 20;
	sImageRect.w	= 220;

#endif

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sImageRect.x	= 20;
	sImageRect.w	= 245;

#endif	
	
	mcp_sersel_img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ServerListWnd, 
												  sImageRect.x, sImageRect.y, sImageRect.w, sImageRect.h, false );	

	mcp_sersel_img->SetClientImage( TID_Server_List_Select_Svr );

	sBtnPos.x = 60;
	sBtnPos.y = 275;
	sBtnPos.w = 58;
	sBtnPos.h = 25;
	
#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sBtnPos.x = 85;

#endif	
	
	//..........................................................................................................
	// 접속 버튼
	//..........................................................................................................
	m_ConnectBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ServerConnectBtn , WID_ServerListWnd , 
								   sBtnPos.x , sBtnPos.y , sBtnPos.w , sBtnPos.h , false );	
	m_ConnectBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK, TID_None );	
	m_ConnectBtn->SetFontR( n_fRsvFontWndButton );
	
	sBtnPos.x = 159;
	sBtnPos.y = 275;
	sBtnPos.w = 58;
	sBtnPos.h = 25;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sBtnPos.x = 174;

#endif	

	//..........................................................................................................
	// 뒤로 버튼
	//..........................................................................................................
	m_CancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ServerCancelBtn , WID_ServerListWnd , 
											 sBtnPos.x , sBtnPos.y, sBtnPos.w , sBtnPos.h , false );	
	m_CancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_None );
	m_CancelBtn->SetFontR( n_fRsvFontWndButton ); 
	m_CancelBtn->SetText( G_STRING_CHAR( IDS_WND_COM_BACK ) );

	sBtnPos.x = 247;
	sBtnPos.y = 41;
	sBtnPos.w = 15;
	sBtnPos.h = 228;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sBtnPos.x = 290;

#endif	

	//..........................................................................................................
	// 스크롤바 ( Quest_List )
	//..........................................................................................................
	m_ScrollBtn_svr = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_ServerScrollBtn_S , WID_ServerListWnd , 
													sBtnPos.x , sBtnPos.y , sBtnPos.w , sBtnPos.h , false );
	m_ScrollBtn_svr->InitSlide( stVertSld , 0 , 20 , 0 , 13 );//Type, min, max, value, 썸의 폭

	m_ScrollBtn_svr->SetClientImage( TID_CTRL_SCR_BG );
	m_ScrollBtn_svr->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_ScrollBtn_svr->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_ScrollBtn_svr->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

}

void CServerListWnd::CompositionNew()
{
		//..........................................................................................................
	// 서버 리스트 윈도우
	StartX = 62 +32 ;
    
	//..........................................................................................................
	m_ServerListWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ServerListWnd, WID_None, 
												   390 , 266 ,  278 + StartX , 310 , false );
	m_ServerListWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_ServerListWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_ServerListWnd->SetWndTileSizeLR( 40, 29 );
	m_ServerListWnd->SetWndTileSizeTB( 31, 11 );	
		
	m_ServerListWnd->SetWndProc( this );	
	m_ServerListWnd->SetTitle( true );
	
	m_ServerListWnd->Initialize();
	m_ServerListWnd->SetVisible( FALSE );
	m_ServerListWnd->SetMove( FALSE );

	///-- Image 
	mcp_black_back9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_ServerListWnd, 15+StartX , 40 , 233, 230, false );       	
	mcp_black_back9->SetWndTileSizeLR( 9, 9 );
	mcp_black_back9->SetWndTileSizeTB( 8, 8 );
	mcp_black_back9->SetFocusCheck( false );
	mcp_black_back9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	mcp_black_back9->SetAdjustPosition( 0, 0 );

	/*
	///-- Image 
	mcp_black_server9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_ServerListWnd, 15, 37, 276, 23, false );       	
	mcp_black_server9->SetWndTileSizeLR( 9, 9 );
	mcp_black_server9->SetWndTileSizeTB( 8, 8 );
	mcp_black_server9->SetFocusCheck( false );
	mcp_black_server9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	mcp_black_server9->SetAdjustPosition( 0, 0 );
	mcp_black_server9->SetVisible( false );

	///-- 선택된 Server 명

	/*
	mcp_sel_server = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ServerListWnd, 40, 42, 213, 17, false );
	mcp_sel_server->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdShadow);
	mcp_sel_server->SetClientImage( TID_None );	
	mcp_sel_server->SetAlignText( n_atLeft, n_atCenter );
	mcp_sel_server->SetTextColor( 225, 225, 160 );	
*/
	//..........................................................................................................
	// 선택 버튼 SERVER_LIST_SELECT_BTN_COUNT 개 
	//..........................................................................................................	
	for( int i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT; ++i )
	{
#ifdef APPLY_BOLD_FONT		
		m_BtnList[ i ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , WID_ServerListWnd , 24 +StartX, 55 + ( 18 * i ) , 198 , 15 , false );
#else
		m_BtnList[ i ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , WID_ServerListWnd , 34 +StartX, 55 + ( 18 * i ) , 198 , 15 , false );
#endif		
		m_BtnList[ i ]->SetClientImage( TID_None );
		m_BtnList[ i ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );	
		m_BtnList[ i ]->SetAlignText( n_atLeft , n_atCenter );
	}
	
	//서버 탭버튼 
	char StrServer[256]="";

	for(short i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT; ++i )
	{
		m_BtnServerTab[ i ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , WID_ServerListWnd ,  10  , 30+( 22* i )   , 90 , 22 , false );
	
	//	mcp_create_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
		m_BtnServerTab[ i ]->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
		m_BtnServerTab[ i ]->SetFontR( n_fRsvFontWndButton ); 
		m_BtnServerTab[ i ]->SetAlignText( n_atCenter , n_atCenter );
		Rsprintf(RWCHAR(StrServer) , _RT("%s"), G_STRING_CHAR(IDS_WND_SERVERNAME00 + i));
		m_BtnServerTab[ i ]->SetText(StrServer);
		m_BtnServerTab[ i ]->SetVisible(false);
		
	}



	for(int i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT; ++i )
	{
#ifdef APPLY_BOLD_FONT	
		m_Statetext[ i ] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None , WID_ServerListWnd , 190 +StartX, 48 + ( 18 * i ) , 50 , 17 , false );
#else
		m_Statetext[ i ] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None , WID_ServerListWnd , 180 +StartX, 48 + ( 18 * i ) , 50 , 17 , false );
#endif	
		m_Statetext[ i ]->SetFontR( n_fRsvFontWndStatic2 );				
	}

	
	//..........................................................................................................
	// 선택 hightlight ( Quest_List )
	//..........................................................................................................
	
#ifdef APPLY_BOLD_FONT
	mcp_sersel_img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ServerListWnd, 20+StartX, 55, 220, 15, false );	
#else
	mcp_sersel_img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ServerListWnd, 34+StartX, 55, 205, 15, false );	
#endif	
	mcp_sersel_img->SetClientImage( TID_Server_List_Select_Svr );

	//..........................................................................................................
	// 접속 버튼
	//..........................................................................................................
	m_ConnectBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ServerConnectBtn , WID_ServerListWnd , 60+StartX , 275 , 58 , 25 , false );	
	m_ConnectBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK, TID_None );	
	m_ConnectBtn->SetFontR( n_fRsvFontWndButton ); 

	//..........................................................................................................
	// 뒤로 버튼
	//..........................................................................................................
	m_CancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ServerCancelBtn , WID_ServerListWnd , 159+StartX , 275, 58 , 25 , false );	
	m_CancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_None );
	m_CancelBtn->SetFontR( n_fRsvFontWndButton ); 
	m_CancelBtn->SetText( G_STRING_CHAR( IDS_WND_COM_BACK ) );

	//..........................................................................................................
	// 스크롤바 ( Quest_List )
	//..........................................................................................................
	
	m_ScrollBtn_svr = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_ServerScrollBtn_S , WID_ServerListWnd , 247 +StartX, 41 , 15 , 228 , false );
	m_ScrollBtn_svr->InitSlide( stVertSld , 0 , 20 , 0 , 13 );//Type, min, max, value, 썸의 폭

	m_ScrollBtn_svr->SetClientImage( TID_CTRL_SCR_BG );
	m_ScrollBtn_svr->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_ScrollBtn_svr->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_ScrollBtn_svr->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );	
	m_ScrollBtn_svr->SetVisible(false);
	

}
//-----------------------------------------------------------------------------
void CServerListWnd::Composition()
{
	//중국을 제외한 나라
	//CompositionOld();
	#ifdef EXPANSION_SERVERTAB
		CompositionNew();
	#else 
		CompositionOld();
	#endif

}

//..............................................................................................................
// 출력 루틴
//..............................................................................................................
void CServerListWnd::Update()
{


	static char TempStr[ 1024 ] = {0,};
/*
	///-- 서버선책창
	if( mb_server )
	{
		
		*/
	///		SWorldData *pServerList = g_RockClient.GetWorldList();

	int Num = 0;
	SServerSetData *pServerSet = g_RockClient.GetServerList();			
	mcp_sersel_img->SetVisible( false );
	//mcp_black_server9->SetVisible( false );
	//mcp_black_back9->SetClientSize( 233, 160 );
	//mcp_black_back9->SetWndRepositionR( 15, 40 );
	
	
	///int Range;
	int ListCount = 0 ; 
	int i = 0;

#ifdef EXPANSION_SERVERTAB

	bool bOneMemory = FALSE;

	for(int j = 0 ; j < g_RockClient.GetServSetCount() ; ++j )
	{
		
		//우선 탭의 인덱스로 처음시작하는 월드 인덱스를 찾음 .. 
		if( pServerSet[j].theWorldIndex == m_iServerTab )
		{
			
			if(!bOneMemory )
			{
				m_iCurIndex = j;
				bOneMemory = true; 
			}
			++ListCount;
		}
	}

#else
	ListCount = SERVER_LIST_SELECT_BTN_COUNT ; 

#endif
	
	for(  i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT ; ++i )
	{
			m_BtnList[ i ]->SetVisible( FALSE );
			m_Statetext[ i ]->SetVisible( FALSE );
	}
#ifdef EXPANSION_SERVERTAB
	if( !m_bRunServerListName )
	{
		m_bRunServerListName =true;
		//	SServerSetData *pServerSet = g_RockClient.GetServerList();
		for( i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT; ++i )
		{
			for(int j = 0 ; j < g_RockClient.GetServSetCount() ; ++j )
			{
				//우선 탭의 인덱스로 처음시작하는 월드 인덱스를 찾음 .. 
				if( pServerSet[j].theWorldIndex == i && pServerSet[j].theChannelIndex == 0 )
				{
					char ServerName[RP_MAX_SERVER_NAME]= {0,};
					SAFE_STR_CPY(ServerName ,  pServerSet[ j ].theName ,  RP_MAX_SERVER_NAME );
					GetServerName(ServerName);
					m_BtnServerTab[i]->SetText(ServerName);
					break;
				}
			}
		}
	}
#endif


	for(  i = 0 ; i < ListCount ; ++i )
	{
		m_BtnList[ i ]->SetBtnImage( TID_None , TID_None , TID_None , TID_None );
		
		//m_ServerPB[ i ]->SetVisible( FALSE );
		
		Num = m_TopNum + i;
		
		if( Num >= g_RockClient.GetServSetCount() )
			break;
		
		if( Num == m_SelectNum )
		{
#ifdef APPLY_BOLD_FONT
			mcp_sersel_img->SetWndRepositionR( 20 +StartX, i * 18 + 49);
#else
			mcp_sersel_img->SetWndRepositionR( 30 +StartX , i * 18 + 49);
#endif			
			mcp_sersel_img->SetVisible( true );
		}
		

#ifdef EXPANSION_SERVERTAB
	
	#ifdef SERVERLIST_TEXT_NON_PVP
		if( (pServerSet[m_iCurIndex + Num ].theChannelIndex + 1) == 2  )
		{
			Rsprintf( RWCHAR(TempStr) , _RT("NON_PVP"));
		}
		else 
		{
			Rsprintf( RWCHAR(TempStr) , _RT(" %d %s"), pServerSet[m_iCurIndex + Num ].theChannelIndex + 1 , G_STRING_CHAR(IDS_WND_CHANAL) );
		}
	#else 
		Rsprintf( RWCHAR(TempStr) , _RT(" %d %s"), pServerSet[m_iCurIndex + Num ].theChannelIndex + 1 , G_STRING_CHAR(IDS_WND_CHANAL) );
	#endif 

#else
		Rsprintf( RWCHAR(TempStr) , _RT("%s"),RWCHAR(pServerSet[ Num ].theName) );
#endif
	
		
		m_BtnList[ i ]->SetText( TempStr );
		m_BtnList[ i ]->SetVisible( TRUE );
		m_Statetext[ i ]->SetVisible( true );
#ifdef EXPANSION_SERVERTAB
		switch( pServerSet[m_iCurIndex +  Num ].theStatus ) 
#else
		switch( pServerSet[ Num ].theStatus ) 
#endif
		{
		case n_Closed:
			m_Statetext[i]->SetText( G_STRING_CHAR( IDS_SYS_STATE_CLOSE ) );
			m_Statetext[i]->SetTextColor( 0, 0, 0 );
			break;
		case n_Open:
			m_Statetext[i]->SetText( G_STRING_CHAR( IDS_SVR_STATE_NOR ) );
			m_Statetext[i]->SetTextColor( 100, 255, 100);
			break;
		case n_NotCongesion:
			m_Statetext[i]->SetText( G_STRING_CHAR( IDS_SVR_STATE_NORMAL ) );
		//	m_Statetext[i]->SetTextColor( 255, 128, 64 );
			m_Statetext[i]->SetTextColor( 255, 255, 0 );
			break; 
		case n_Congestion:
			m_Statetext[i]->SetText( G_STRING_CHAR( IDS_SYS_STATE_CONG ) );
			m_Statetext[i]->SetTextColor( 255, 128, 64 );
			break;
		case n_Hold:
			m_Statetext[i]->SetText( G_STRING_CHAR( IDS_SYS_STATE_FULL ) );
			m_Statetext[i]->SetTextColor( 255, 10, 10 );
			break;		
		}
    
	}

	/*}
	///-- 채널 선택창
	else
	{
		int Num = 0;
		
		///SChannelData *pChannelList = g_RockClient.GetChannelList();
		mcp_chasel_img->SetVisible( false );
		mcp_black_server9->SetVisible( true );
		mcp_black_back9->SetClientSize( 205, 140 );
		mcp_black_back9->SetWndRepositionR( 15, 60 );
		
		int Range;
		
		for( int i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT - 1; ++i )
		{
			m_ServerPB[ i ]->SetVisible( FALSE );
			m_BtnList[ i ]->SetVisible( FALSE );
			

			
			Num = m_TopNum + i;
			
//			if( Num >= g_RockClient.GetChannelCount() )
			//	break;
			
			if( Num == m_SelectNum )
			{
				mcp_chasel_img->SetWndRepositionR( 30, i * 18 + 67);
				mcp_chasel_img->SetVisible( true );
			}
				
			
			// 서버 ( x / 3000 ) * 1000 값을 써 Boa 요

//			Range = ( int ) ( ( ( float ) pChannelList[ i ].theConnectUser / 1000.0f ) * 100.0f );
			m_ServerPB[ i ]->InitProgress( Range );			
			m_ServerPB[ i ]->SetVisible( TRUE );
			
//			wsprintf( TempStr , G_STRING_CHAR( IDS_WND_SVRSEL_CHN_NUM ) , pChannelList [ Num ].theChannelIndex + 1 );
			
			m_BtnList[ i ]->SetText( TempStr );
			m_BtnList[ i ]->SetVisible( TRUE );
			
			///-- 접속 불가 채널
			///-- 현재 선택된 거라면  -1 로 !!!!
//			if( pChannelList[ Num ].theStatus == n_Fail )
			{
				m_BtnList[ i ]->SetBtnHold( true );
				m_BtnList[ i ]->SetTextColor( 110, 110, 110 );				
			}
			///-- 접속 가능 채널
			else
			{
				m_BtnList[ i ]->SetBtnHold( false );
				m_BtnList[ i ]->SetTextColor( 255, 255, 255 );
			}				
			
		}
		
	}
	*/
	

}

void CServerListWnd::GetServerName(char *ServerNameStr)
{
	// [  , ]  찾아서 그안에 있는 데이터만 복사한다. 
	
	RTCHAR StartChar = '[';
	RTCHAR EndChar	=	']';
#ifdef UNICODE_ROCK
	RTCHAR* pStartDest = wcsrchr( RWCHAR(ServerNameStr) , StartChar) +1 ;
	RTCHAR* pEndDest= wcsrchr(RWCHAR(ServerNameStr) , EndChar) ;
#else
	char* pStartDest = strrchr( ServerNameStr , StartChar) +1 ;
	char* pEndDest= strrchr(ServerNameStr , EndChar)- 1 ;
#endif
	
	RTCHAR TmpStr[128]  = {0,};
	if(pStartDest == NULL || pEndDest == NULL)
	{
		Rstrcpy( TmpStr , ServerNameStr );
	}
	else 
	{
		Rstrncpy( TmpStr , pStartDest , pEndDest - pStartDest );
	}
	
	Rsprintf( RWCHAR(ServerNameStr) , _RT("%s"), RWCHAR(TmpStr) );

}


//..............................................................................................................
// 메세지 처리
//..............................................................................................................
void CServerListWnd::Proc( SEventMessage* EMsg )
{
	int i;
	for( i = 0 ; i < SERVER_LIST_SELECT_BTN_COUNT; ++i )
	{
		if( m_BtnList[i]->GetID() == EMsg->FocusWnd ) 
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SelectServerList( i );
			}
		}
	}
#ifdef EXPANSION_SERVERTAB
	
	for(i =0 ; i < SERVER_LIST_SELECT_BTN_COUNT; ++i )
	{
		if( m_BtnServerTab[i]->GetID() == EMsg->FocusWnd ) 
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_iServerTab = i;
				SServerSetData *pServerSet = g_RockClient.GetServerList();

				for(int j = 0 ; j < g_RockClient.GetServSetCount() ; ++j )
				{
					
					//우선 탭의 인덱스로 처음시작하는 월드 인덱스를 찾음 .. 
					if( pServerSet[j].theWorldIndex == m_iServerTab )
					{
						m_iCurIndex = j ;
						break;
					}
					
				}

				for(j =0 ; j < SERVER_LIST_SELECT_BTN_COUNT; ++j )
				{
					if( m_iServerTab == j)
					{
						m_BtnServerTab[ j ]->SetBtnHold(true);
					}
					else
					{
						m_BtnServerTab[ j ]->SetBtnHold(false);
					}
				}
			}
		}
	}

#endif
	


	switch( EMsg->FocusWnd )
	{
		case WID_ServerConnectBtn:

			if( EMsg->Notify == NM_BTN_CLICK )
			{
				ConnectProcess();
			}
			
			break;

		case WID_ServerCancelBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				CancelProcess(true);
			}
			break;

		case WID_ServerScrollBtn_C:
		case WID_ServerScrollBtn_S:
			{
				if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
				{
					SetScroll( EMsg->fParam );
				}
			}
			break;

	}
}

//-----------------------------------------------------------------------------
void CServerListWnd::SetWheelValue( short _wheel )
{
	int value = 0;

#ifndef EXPANSION_SERVERTAB



	//if( mb_server )
	//{
	m_ScrollBtn_svr->GetValue( &value );
	if( ( _wheel < 0 ) && ( value < g_RockClient.GetServSetCount() - SERVER_LIST_SELECT_BTN_COUNT) )
	{
		value += 1;			
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;			
	}
	
	m_ScrollBtn_svr->SetValue( value );
	SetScroll( value );
	//	}
	/*
	else
	{
	m_ScrollBtn_chn->GetValue( &value );
	if( ( _wheel < 0 ) && ( value < g_RockClient.GetServSetCount() - SERVER_LIST_SELECT_BTN_COUNT + 1 ) )
	{
	value += 1;			
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
	value -= 1;			
	}
	
	  m_ScrollBtn_chn->SetValue( value );
	  SetScroll( value );
	  }
	*/
	
#endif 
}

//..............................................................................................................
// 서버 리스트 선택 
//..............................................................................................................
void  CServerListWnd::SelectServerList(int Num)
{
   
	int ServerNum = m_TopNum + Num;		
	if( ServerNum >= g_RockClient.GetServSetCount() )
		return;
	
	m_SelectNum = ServerNum;	

	/*
	if( mb_server )
	{
	int ServerNum = m_TopNum + Num;		
	//		if( ServerNum >= g_RockClient.GetWorldCount() )
	return;
	
		m_SelectNum = ServerNum;		
	}
	else
	{
		int chnNum = m_TopNum + Num;		
		if( chnNum >= g_RockClient.GetChannelCount() )
			return;		
		m_SelectNum = chnNum;

	}
	*/
}

//..............................................................................................................
// 서버 리스트 스크롤 
//..............................................................................................................
void CServerListWnd::SetScroll(int Num)
{	

#ifndef EXPANSION_SERVERTAB
	m_TopNum = Num;	
#endif 

}

//..............................................................................................................
// 서버 접속 버튼
//..............................................................................................................
void CServerListWnd::ConnectProcess(void)
{
	if( m_SelectNum == -1 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_SELECT_CHN ) , 1500 , NULL );			
		return;
	}

	//m_SelectNum 네트윅 전송시에는 다시자리 계산해서 보내준다 .. 
#ifdef EXPANSION_SERVERTAB
//	int CurIndex =0 ; 
	SServerSetData *pServerSet = g_RockClient.GetServerList();

	if(pServerSet[m_iCurIndex + m_SelectNum].theStatus == n_Hold )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_CURRENT_CHANNAL_FULL ) , 1500 , NULL );
		return;
	}

	if( g_RockClient.ConnectToGameServer( 	m_iCurIndex + m_SelectNum  ) == FALSE )	/// Send_RpCsGameLogin(); 동시 처리 
	{			
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_FAIL_CONNECT ) , 1500 , NULL );
		return;
	}
	
	nRui->SendEvent( WID_ServerListWnd , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	
	g_RockClient.SetLoginStep( nServerSelectOK );
	

	nRui->m_WorldIndex = pServerSet[	m_iCurIndex + m_SelectNum ].theWorldIndex;	
	nRui->m_ChannelIndex = pServerSet[	m_iCurIndex + m_SelectNum ].theChannelIndex;	

#else
	
	SServerSetData *pServerSet = g_RockClient.GetServerList();
	nRui->m_WorldIndex =	pServerSet[m_SelectNum].theWorldIndex;	
	nRui->m_ChannelIndex =	pServerSet[m_SelectNum].theChannelIndex;	

	if(pServerSet[m_SelectNum].theStatus == n_Hold )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_CURRENT_CHANNAL_FULL ) , 1500 , NULL );
		return;
	}

	if( g_RockClient.ConnectToGameServer( m_SelectNum ) == FALSE )	/// Send_RpCsGameLogin(); 동시 처리 
	{			
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_FAIL_CONNECT ) , 1500 , NULL );
		return;
	}
	
	nRui->SendEvent( WID_ServerListWnd , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	
	g_RockClient.SetLoginStep( nServerSelectOK );
	
#endif
	

	/*
	
	static char TempStr[ 1024 ] = "";
	  ///---------------------------------------------------------------------------
	  ///-- 서버 선택 완료

	if( mb_server )
	{
		if( m_SelectNum == -1 )
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_SELECT_SVR ), 1500 , NULL );			
			return;
		}
///		SWorldData *pServerList = g_RockClient.GetWorldList();
		///g_RockClient.Send_RpCsSelectWorld( pServerList[ m_SelectNum ].theWorldIndex );		
		wsprintf( TempStr, G_STRING_CHAR( IDS_WND_SVRSEL_SVR_NAME ), pServerList[ m_SelectNum ].theName );
		mcp_sel_server->SetText( TempStr );

		//By simwoosung
		g_MCommunityWnd.m_MSettingTabWnd.SetServerName((TCHAR *)(pServerList[ m_SelectNum ].theName));		
				
	}
	///---------------------------------------------------------------------------
	///-- 채널 선택 완료 접속
	else
	{

		if( m_SelectNum == -1 )
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_SELECT_CHN ) , 1500 , NULL );			
			return;
		}
		
		if( g_RockClient.ConnectToGameServer( m_SelectNum ) == FALSE )	/// Send_RpCsGameLogin(); 동시 처리 
		{			
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_FAIL_CONNECT ) , 1500 , NULL );
			return;
		}
			
		//by simwoosung
		///SChannelData *pChannelList = g_RockClient.GetChannelList();
		g_MCommunityWnd.m_MSettingTabWnd.SetChannelNum( pChannelList[m_SelectNum].theChannelIndex + 1 );
		
		nRui->SendEvent( WID_ServerListWnd , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		
		
	}
	*/
}

//..............................................................................................................
// 서버 취소 버튼
//..............................................................................................................
void CServerListWnd::CancelProcess(bool bFlag)
{

	if(  n_Normal == g_RockClient.m_GameTypeInfo.theFrom 
		|| g_RockClient.GetLanguage() == eLang_Korea ) 
	{
		g_RockClient.CloseLoginState();
	
		nRui->SendEvent( WID_ServerListWnd , n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
		CFrameWnd* LoginWnd = ( CFrameWnd* ) Container->Get( WID_LoginWnd );	
		
		nRui->SendEvent( WID_LoginWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		LoginWnd ->SetVisible( true );
	}
	else
	{
		if(!bFlag)
		{
			Sleep(5000);
		}
		
		SendMessage( g_RockClient.GetApp(), WM_CLOSE, NULL, 1 );
	}	
}

