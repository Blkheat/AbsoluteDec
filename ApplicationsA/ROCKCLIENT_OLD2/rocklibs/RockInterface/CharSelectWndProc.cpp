//----------------------------------------------------------------------------------------------------------------------
// 파일명 : CharSelectWndProc.cpp
//----------------------------------------------------------------------------------------------------------------------
#include "CharSelectWndProc.h"
#include "Rui.h"

#include "EditBox.h"
#include "CheckButton.h"
#include "RadioButton.h"
 
#include "..\\..\\Effect.h"
#include "LoadingWndProc.h"
#include "stringmanager.h"

#include "MessageWndProc.h"
#include "CharCreateWndProc.h"
#include "CharDelWndProc.h"

#include "LoginWndProc.h"
#include "MapDataManager.h"


CCharSelectWnd			CharSelectWnd;


 
#define CLASS_UNIQUE		10							// 클래스 캐릭터 유니크
#define KOREA_EFFECT		EF_KOREA_BACK_EFFECT
#define CREATE_ROTY_ANGLE	1.0f

	
//..............................................................................................................
// 시작시 세팅 값
//..............................................................................................................
void CCharSelectWnd::StartUp(void)
{
	if( m_SelectWnd != NULL )
		nRui->SendEvent( WID_CharSelect_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
	m_State     = CSW_SELECT_NONE;

#ifdef DECO_RENEWAL_MJH 
	m_SelectNum = 0 ;

#else
	m_SelectNum = -1;
#endif //DECO_RENEWAL_MJH

	m_SEQState  = SEQ_CAMERA_ANI;


	g_RockClient.m_IsReConnect = TRUE;

	pCharList[ 0 ] = pCharList[ 1 ] = pCharList[ 2 ] = pCharList[ 3 ] = pCharList[ 4 ] = NULL;

	SelectCharacter();

	// 창 닫기
	SetCloseAllBtn();
	
	// 카메라 워킹 선택창으로
	StartSelectCameraWalkPostion();
	g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::SelectCameraWalkEnd);
	
#ifdef 	LOGINPAGE_BGIMG_CHANGE
	g_RockClient.m_LoginShutter.Start( SHUTTER_UP_PROCESS , 0 );
#else
	g_RockClient.m_LoginShutter.Start( SHUTTER_UP_PROCESS , 1000 );
#endif
}

//..............................................................................................................
// 닫기
//..............................................................................................................
void CCharSelectWnd::CloseAll(void)
{
	if( m_SelectWnd != NULL )
	{
		nRui->SendEvent( WID_CharSelect_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
}

//
void CCharSelectWnd::Composition()
{

	// 캐릭터 위치 설정

	//밀레나		캐릭터 선택		레인
	//					 0
	//				  1		2	
	//				3		  4	
	//       6					 	 7
	//					 5
	//
#ifdef DECO_RENEWAL_MJH 
	// 캐릭터 선택 0
	PosInfo[ 0 ].SetPos( 336 , 259, 0 );
	// 캐릭터 선택 1
	PosInfo[ 1 ].SetPos( 336 , 259 , 1 );
	// 캐릭터 선택 2
	PosInfo[ 2 ].SetPos( 336 , 259 , 2 );
	// 캐릭터 선택 3
	PosInfo[ 3 ].SetPos( 336 , 259, 3 );
	// 캐릭터 선택 4
	PosInfo[ 4 ].SetPos( 336 , 259, 4 );	
	// 선택 된 곳 5
	PosInfo[ 5 ].SetPos( 336 , 259 , 5 );
#else
	// 캐릭터 선택 0
	PosInfo[ 0 ].SetPos( 336 , 269, 0 );
	// 캐릭터 선택 1
	PosInfo[ 1 ].SetPos( 334 , 268 , 1 );
	// 캐릭터 선택 2
	PosInfo[ 2 ].SetPos( 338 , 268 , 2 );
	// 캐릭터 선택 3
	PosInfo[ 3 ].SetPos( 332 , 267, 3 );
	// 캐릭터 선택 4
	PosInfo[ 4 ].SetPos( 340 , 267, 4 );
	// 선택 된 곳 5
	PosInfo[ 5 ].SetPos( 336 , 259 , 5 );
#endif //DECO_RENEWAL_MJH	
	// 밀레나 생성
#ifdef DECO_RENEWAL_MJH
	PosInfo[ 6 ].SetPos( 335 , 259 , 6 );
	PosInfo[ 7 ].SetPos( 336 , 259 , 7 );
#else
	PosInfo[ 6 ].SetPos( 321 , 265 , 6 );
	PosInfo[ 7 ].SetPos( 350 , 266 , 7 );
#endif //DECO_RENEWAL_MJH
	
	//국내 버전 - 이펙트 위치
	PosInfo[ 8 ].SetPos( 328 , 285 , 8 );
	
	SRect FrameRect;

	FrameRect.w	= nRui->Desktop.Width;
	FrameRect.h	= nRui->Desktop.Height;
	
	FrameRect.x	= 0;
	FrameRect.y	= 0;

	//..........................................................................................................
	// 선택창
	//..........................................................................................................
	m_SelectWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_CharSelect_Wnd, WID_None, FrameRect.x , FrameRect.y, FrameRect.w, FrameRect.h , false );
	
	m_SelectWnd->SetClientImage( TID_None );
	m_SelectWnd->SetCaption( _RT("Character Select Window") );
	m_SelectWnd->SetWndProc( this );
	m_SelectWnd->SetTitle( false );	
	m_SelectWnd->Initialize();
	m_SelectWnd->SetVisible( false );
	m_SelectWnd->SetMove( FALSE );


#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	m_SelectWnd->SetExtentWH(402.0f, 7.0f);
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	//..........................................................................................................
	// 캐릭터 선택 이름 배경 그림
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_SelectNameBoxImage = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_CharSelect_Name_BoxImg, WID_CharSelect_Wnd , 102, 177, 219, 393, false );
	m_SelectNameBoxImage->SetClientImage( TID_CharSel_Name_Img );
#else
	m_SelectNameBoxImage = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_CharSelect_Name_BoxImg, WID_CharSelect_Wnd , 402, 7, 219, 143, false );
	m_SelectNameBoxImage->SetClientImage( TID_CharSel_Name_Img );
#endif // DECO_RENEWAL_MJH
	m_SelectNameBoxImage->SetVisible(false);
#ifdef DECO_RENEWAL_MJH
	m_SelectCharBoxImage = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_CharSelect_BoxImg, WID_CharSelect_Wnd , 402, 7, 219, 43, false );
	m_SelectCharBoxImage->SetClientImage( TID_CTRL_BTN175 );
	m_SelectCharBoxImage->SetVisible(false);
#endif // DECO_RENEWAL_MJH

#ifdef DECO_RENEWAL_MJH
	selectchartext = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, 402 ,	 7 ,	219, 32, false );
#else 
	selectchartext = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, 402 ,	 7 ,	219, 32, false );
#endif // DECO_RENEWAL_MJH
	selectchartext->SetFontR( n_fRsvFontTitle );
	selectchartext->SetAlignText( n_atCenter, n_atCenter );
	selectchartext->SetText( G_STRING_CHAR( IDS_WND_SEL_CHAR ) ); 

	//..........................................................................................................
	// 캐릭터 레벨 텍스트 박스
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_Select_Level_Edit = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_CharSelect_Level_TextWnd, WID_CharSelect_Wnd , 120 , 307 + ( 20 * 0 ), 33 , 16 , false );
#else
	m_Select_Level_Edit = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_CharSelect_Level_TextWnd, WID_CharSelect_Wnd , 440 , 60 + ( 20 * 0 ), 33 , 16 , false );
#endif // DECO_RENEWAL_MJH
	
	m_Select_Level_Edit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_Select_Level_Edit->SetClientImage( TID_None );
	m_Select_Level_Edit->SetClientColor( 180, 175, 160 );
	m_Select_Level_Edit->SetAlignText( n_atLeft, n_atCenter );
	m_Select_Level_Edit->SetText( _RT("") );
	m_Select_Level_Edit->SetTextColor( 255, 255, 255 );
	m_Select_Level_Edit->SetVisible(false);

	//..........................................................................................................
	// 직업 텍스트 박스
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_SelectClassWnd = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_CharSelect_Class_TextWnd, WID_CharSelect_Wnd , 120 , 307 + ( 20 * 1 ), 80 , 16 , false );
#else
	m_SelectClassWnd = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_CharSelect_Class_TextWnd, WID_CharSelect_Wnd , 440 , 60 + ( 20 * 1 ), 80 , 16 , false );
#endif // DECO_RENEWAL_MJH	
	m_SelectClassWnd->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_SelectClassWnd->SetClientImage( TID_None );
	m_SelectClassWnd->SetClientColor( 180, 175, 160 );
	m_SelectClassWnd->SetAlignText( n_atLeft, n_atCenter );
	m_SelectClassWnd->SetText( G_STRING_CHAR( IDS_CLASS_WANDERER ) );
	m_SelectClassWnd->SetTextColor( 255, 255, 255 );
	m_SelectClassWnd->SetVisible(false);

	//..........................................................................................................
	// 캐릭터 이름 텍스트 박스
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_Select_Name_Edit = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_CharSelect_Name_TextWnd, WID_CharSelect_Wnd , 120 , 307 + ( 20 * 2 ), 200 , 50 , false );
#else 
	m_Select_Name_Edit = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_CharSelect_Name_TextWnd, WID_CharSelect_Wnd , 440 , 60 + ( 20 * 2 ), 200 , 50 , false );
#endif // DECO_RENEWAL_MJH
	m_Select_Name_Edit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_Select_Name_Edit->SetClientImage( TID_None );
	m_Select_Name_Edit->SetClientColor( 180, 175, 160 );
	m_Select_Name_Edit->SetAlignText( n_atLeft, n_atCenter );
	m_Select_Name_Edit->SetMultiLine( true );
	m_Select_Name_Edit->SetText( _RT("") );
	m_Select_Name_Edit->SetTextColor( 255, 255, 255 );
	m_Select_Name_Edit->SetVisible(false);
#ifdef DECO_RENEWAL_MJH
//케릭터정보
	m_Chracterinfotext = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, 120 ,	 287 + ( 20 * 0 ) ,	219, 16, false );
	m_Chracterinfotext->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_Chracterinfotext->SetClientImage( TID_None );
	m_Chracterinfotext->SetClientColor( 180, 175, 160 );
	m_Chracterinfotext->SetAlignText( n_atLeft, n_atCenter );
	m_Chracterinfotext->SetText( "케릭터 정보" );
	m_Chracterinfotext->SetTextColor( 255, 255, 255 );
	m_Chracterinfotext->SetVisible(false);
//길드정보
	m_Guildinfotext = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, 120 ,	 307 + ( 20 * 3 ) ,	219, 16, false );
	m_Guildinfotext->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_Guildinfotext->SetClientImage( TID_None );
	m_Guildinfotext->SetClientColor( 180, 175, 160 );
	m_Guildinfotext->SetAlignText( n_atLeft, n_atCenter );
	m_Guildinfotext->SetText( "프론티어 정보" );
	m_Guildinfotext->SetTextColor( 255, 255, 255 );
	m_Guildinfotext->SetVisible(false);
//가입길드정보	
	m_JoinGuildtext = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, 120 ,	 307 + ( 20 * 4 ) ,	219, 16, false );
	m_JoinGuildtext->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_JoinGuildtext->SetClientImage( TID_None );
	m_JoinGuildtext->SetClientColor( 180, 175, 160 );
	m_JoinGuildtext->SetAlignText( n_atLeft, n_atCenter );
	m_JoinGuildtext->SetText( "소속길드 " );
	m_JoinGuildtext->SetTextColor( 255, 255, 255 );
	m_JoinGuildtext->SetVisible(false);
//군단정보	
	m_JoinCorpstext = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, 120 ,	 307 + ( 20 * 5 ) ,	219, 16, false );
	m_JoinCorpstext->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_JoinCorpstext->SetClientImage( TID_None );
	m_JoinCorpstext->SetClientColor( 180, 175, 160 );
	m_JoinCorpstext->SetAlignText( n_atLeft, n_atCenter );
	m_JoinCorpstext->SetText( "소속군단 :" );
	m_JoinCorpstext->SetTextColor( 255, 255, 255 );
	m_JoinCorpstext->SetVisible(false);
//현위치
	m_MapPositiontext = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, 120 ,	 307 + ( 20 * 6 ) ,	219, 16, false );
	m_MapPositiontext->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_MapPositiontext->SetClientImage( TID_None );
	m_MapPositiontext->SetClientColor( 180, 175, 160 );
	m_MapPositiontext->SetAlignText( n_atLeft, n_atCenter );
	m_MapPositiontext->SetText( "현위치 " );
	m_MapPositiontext->SetTextColor( 255, 255, 255 );
	m_MapPositiontext->SetVisible(false);
//명성치
	m_FameText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, 120 ,	 307 + ( 20 * 7 ) ,	219, 16, false );
	m_FameText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_FameText->SetClientImage( TID_None );
	m_FameText->SetClientColor( 180, 175, 160 );
	m_FameText->SetAlignText( n_atLeft, n_atCenter );
	m_FameText->SetText( "명성치" );
	m_FameText->SetTextColor( 255, 255, 255 );
	m_FameText->SetVisible(false);
//개인명성
	m_PcFameText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, 120 ,	 307 + ( 20 * 8 ) ,	219, 16, false );
	m_PcFameText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_PcFameText->SetClientImage( TID_None );
	m_PcFameText->SetClientColor( 180, 175, 160 );
	m_PcFameText->SetAlignText( n_atLeft, n_atCenter );
	m_PcFameText->SetText( "개인/누적 " );
	m_PcFameText->SetTextColor( 255, 255, 255 );
	m_PcFameText->SetVisible(false);
//길드명성
	m_GuildFameText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, 120 ,	 307 + ( 20 * 9 ) ,	219, 16, false );
	m_GuildFameText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_GuildFameText->SetClientImage( TID_None );
	m_GuildFameText->SetClientColor( 180, 175, 160 );
	m_GuildFameText->SetAlignText( n_atLeft, n_atCenter );
	m_GuildFameText->SetText( "기여도 :" );
	m_GuildFameText->SetTextColor( 255, 255, 255 );
	m_GuildFameText->SetVisible(false);
#endif // DECO_RENEWAL_MJH

	//..........................................................................................................
	// 캐릭터 선택 생성 배경 그림
	//..........................................................................................................
	m_SelectCreateBoxImage = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_CharSelect_Create_BoxImg, WID_CharSelect_Wnd , 354 , 605 , 316 , 91 , false );
	m_SelectCreateBoxImage->SetVisible(false);

	m_Create_HelpImage = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_CharCreate_Help, WID_CharSelect_Wnd , 0 , 0 , 256 , 256 , false );
	m_Create_HelpImage->SetVisible(false);

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	m_SelectWnd->SetExtentWH(402.0f, 646.0f);
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	//..........................................................................................................
	// 시작 버튼
	//..........................................................................................................
	m_Select_Start_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect_Start_Btn , WID_CharSelect_Wnd , 424, 646, 175, 25, false ); 
	m_Select_Start_Btn->SetClientImage( TID_None );
	m_Select_Start_Btn->SetBtnImage( TID_CTRL_BTN175 , TID_CTRL_BTN175_OVR , TID_CTRL_BTN175_CLK , TID_CTRL_BTN175_DIS );
	m_Select_Start_Btn->SetVisible(false);
	m_Select_Start_Btn->SetFontR( n_fRsvFontWndButton );
	m_Select_Start_Btn->SetText( G_STRING_CHAR( IDS_WND_BT_START_GAME ) );	

	//..........................................................................................................
	// 캐릭터 선택 생성 버튼
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_Select_Create_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect_Create_Btn , WID_CharSelect_Wnd ,20, 730, 175, 25, false );    
#else 
	m_Select_Create_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect_Create_Btn , WID_CharSelect_Wnd ,424, 672, 175, 25, false );    
#endif // DECO_RENEWAL_MJH	
	m_Select_Create_Btn->SetClientImage( TID_None );
	m_Select_Create_Btn->SetBtnImage( TID_CTRL_BTN175 , TID_CTRL_BTN175_OVR , TID_CTRL_BTN175_CLK , TID_CTRL_BTN175_DIS );
	m_Select_Create_Btn->SetVisible(false);
	m_Select_Create_Btn->SetFontR( n_fRsvFontWndButton );
	m_Select_Create_Btn->SetText( G_STRING_CHAR( IDS_WND_BT_CREATE_CH ) );	

	//..........................................................................................................
	// 삭제 버튼
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_Select_Delete_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect_Delete_Btn , WID_CharSelect_Wnd , 215, 730, 175, 25, false );   
#else 
	m_Select_Delete_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect_Delete_Btn , WID_CharSelect_Wnd , 424, 698, 175, 25, false );   
#endif // DECO_RENEWAL_MJH	
	m_Select_Delete_Btn->SetClientImage( TID_None );
	m_Select_Delete_Btn->SetBtnImage( TID_CTRL_BTN175 , TID_CTRL_BTN175_OVR , TID_CTRL_BTN175_CLK , TID_CTRL_BTN175_DIS );
	m_Select_Delete_Btn->SetVisible(false);
	m_Select_Delete_Btn->SetFontR( n_fRsvFontWndButton );
	m_Select_Delete_Btn->SetText( G_STRING_CHAR( IDS_WND_BT_DEL_CH ));	

	//..........................................................................................................
	// 서버리스트 버튼 0 
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_Select_Server_Btn0 = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect_Server_Btn0 , WID_CharSelect_Wnd ,634, 730, 175, 25, false );
#else 
	m_Select_Server_Btn0 = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect_Server_Btn0 , WID_CharSelect_Wnd ,424, 724, 175, 25, false );
#endif // DECO_RENEWAL_MJH
	m_Select_Server_Btn0->SetClientImage( TID_None );
	m_Select_Server_Btn0->SetBtnImage( TID_CTRL_BTN175 , TID_CTRL_BTN175_OVR , TID_CTRL_BTN175_CLK , TID_CTRL_BTN175_DIS );
	m_Select_Server_Btn0->SetVisible(false);
	m_Select_Server_Btn0->SetFontR( n_fRsvFontWndButton );
	m_Select_Server_Btn0->SetText( G_STRING_CHAR( IDS_WND_SVRSEL_SVR ) );
#ifdef DECO_RENEWAL_MJH
	//..........................................................................................................
	// 게임종료 
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_Select_exit_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect_exit_Btn , WID_CharSelect_Wnd ,829, 730, 175, 25, false );
#else 
	m_Select_exit_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect_exit_Btn , WID_CharSelect_Wnd ,424, 752, 175, 25, false );
#endif // DECO_RENEWAL_MJH
	m_Select_exit_Btn->SetClientImage( TID_None );
	m_Select_exit_Btn->SetBtnImage( TID_CTRL_BTN175 , TID_CTRL_BTN175_OVR , TID_CTRL_BTN175_CLK , TID_CTRL_BTN175_DIS );
	m_Select_exit_Btn->SetVisible(false);
	m_Select_exit_Btn->SetFontR( n_fRsvFontWndButton );
	m_Select_exit_Btn->SetText( G_STRING_CHAR( IDS_WND_GAME_EXIT ) );
#endif // DECO_RENEWAL_MJH 

	//..........................................................................................................
	// 캐릭터 생성 다이얼로그 배경
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	int PosX = 804;
#else
	int PosX = 804;
#endif // DECO_RENEWAL_MJH
	int PosY = 208;

	m_CreateDlgmage = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_CharCreate_Dlg , WID_CharSelect_Wnd , PosX , PosY , 189, 351, false );
	m_CreateDlgmage->SetClientImage( TID_CharCreate_Dlg );
	m_CreateDlgmage->SetVisible(false);
	
	///-- 캐릭터 생성
	tbox[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 18, PosY + 7, 154, 17, false );
	tbox[0]->SetFontR( n_fRsvFontTitle );		
	tbox[0]->SetAlignText( n_atCenter, n_atCenter );
	tbox[0]->SetText( G_STRING_CHAR( IDS_WND_BT_CREATE_CH ) );
	
	///-- 이름(Static)
	tbox[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 20, PosY + 35, 110, 17, false );
	tbox[1]->SetFontR( n_fRsvFontWndStatic );	
	tbox[1]->SetAlignText( n_atLeft, n_atCenter );
	tbox[1]->SetText( G_STRING_CHAR( IDS_CHARSTATE_NAME ) );
	
	///-- 국가(Static)
	tbox[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 20, PosY + 85, 110, 17, false );
	tbox[2]->SetFontR( n_fRsvFontWndStatic );	
	tbox[2]->SetAlignText( n_atLeft, n_atCenter );
	tbox[2]->SetText( G_STRING_CHAR( IDS_CHARSTATE_NATION ) ); 
	
	///-- 머리(Static)
	tbox[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 40, PosY + 229, 110, 17, false );
	tbox[3]->SetFontR( n_fRsvFontWndStatic2 );		
	tbox[3]->SetText( G_STRING_CHAR( IDS_WND_HEAD ) ); 
	
	///-- 얼굴(Static)
	tbox[4] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 40, PosY + 253, 110, 17, false );
	tbox[4]->SetFontR( n_fRsvFontWndStatic2 );		
	tbox[4]->SetText( G_STRING_CHAR( IDS_WND_FACE ) ); 

	///--  회전(Static)
	tbox[5] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 40, PosY + 280, 110, 17, false );
	tbox[5]->SetFontR( n_fRsvFontWndStatic2 );		
	tbox[5]->SetText( G_STRING_CHAR( IDS_WND_ROTATE ) );
	
	///-- 확대(Static)
	tbox[6] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 40, PosY + 304, 110, 17, false );
	tbox[6]->SetFontR( n_fRsvFontWndStatic2 );		
	tbox[6]->SetText( G_STRING_CHAR( IDS_WND_ZOOM ) );
		
	//..........................................................................................................
	// 캐릭터 생성 왼쪽    버튼
	//..........................................................................................................
	m_Create_Rot_Left_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Rot_Left_Btn , WID_CharSelect_Wnd ,  PosX + 20, PosY + 278, 19, 19, false );
	m_Create_Rot_Left_Btn->SetClientImage( TID_None );
	m_Create_Rot_Left_Btn->SetBtnImage( TID_CharCreate_Left , TID_CharCreate_Left_OVR , TID_CharCreate_Left_CLK, TID_None );
	m_Create_Rot_Left_Btn->SetVisible(false);

	//..........................................................................................................
	// 캐릭터 생성 오른 쪽 버튼
	//..........................................................................................................
	m_Create_Rot_Right_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Rot_Right_Btn , WID_CharSelect_Wnd , PosX + 149,PosY +  278, 19, 19, false );
	m_Create_Rot_Right_Btn->SetClientImage( TID_None );
	m_Create_Rot_Right_Btn->SetBtnImage( TID_CharCreate_Right , TID_CharCreate_Right_OVR, TID_CharCreate_Right_CLK, TID_None );
	m_Create_Rot_Right_Btn->SetVisible(false);

	//..........................................................................................................
	// 캐릭터 생성 머리 왼쪽   버튼
	//..........................................................................................................
	m_Create_Hair_Left_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Hair_Left_Btn , WID_CharSelect_Wnd , PosX + 20, PosY + 227, 19, 19, false );
	m_Create_Hair_Left_Btn->SetClientImage( TID_None );
	m_Create_Hair_Left_Btn->SetBtnImage( TID_CharCreate_Left , TID_CharCreate_Left_OVR , TID_CharCreate_Left_CLK, TID_None );
	m_Create_Hair_Left_Btn->SetVisible(false);

	//..........................................................................................................
	// 캐릭터 생성 머리 오른쪽 버튼
	//..........................................................................................................
	m_Create_Hair_Right_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Hair_Right_Btn , WID_CharSelect_Wnd , PosX + 149, PosY + 227, 19, 19, false );
	m_Create_Hair_Right_Btn->SetClientImage( TID_None );
	m_Create_Hair_Right_Btn->SetBtnImage( TID_CharCreate_Right , TID_CharCreate_Right_OVR, TID_CharCreate_Right_CLK, TID_None );
	m_Create_Hair_Right_Btn->SetVisible(false);

	//..........................................................................................................
	// 캐릭터 생성 얼굴 왼쪽   버튼
	//..........................................................................................................
	m_Create_Face_Left_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Face_Left_Btn , WID_CharSelect_Wnd , PosX + 20, PosY + 251, 19, 19, false ); 
	m_Create_Face_Left_Btn->SetClientImage( TID_None );
	m_Create_Face_Left_Btn->SetBtnImage( TID_CharCreate_Left , TID_CharCreate_Left_OVR , TID_CharCreate_Left_CLK, TID_None );
	m_Create_Face_Left_Btn->SetVisible(false);

	//..........................................................................................................
	// 캐릭터 생성 얼굴 오른쪽 버튼
	//..........................................................................................................
	m_Create_Face_Right_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Face_Right_Btn , WID_CharSelect_Wnd , PosX + 149, PosY + 251, 19, 19, false );
	m_Create_Face_Right_Btn->SetClientImage( TID_None );
	m_Create_Face_Right_Btn->SetBtnImage( TID_CharCreate_Right , TID_CharCreate_Right_OVR, TID_CharCreate_Right_CLK, TID_None );
	m_Create_Face_Right_Btn->SetVisible(false);

	//..........................................................................................................
	// 캐릭터 생성 밀레나 버튼
	//..........................................................................................................
	m_Create_Millena_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Millena_Btn , WID_CharSelect_Wnd, PosX + 30, PosY + 106, 60, 76, false );	
	m_Create_Millena_Btn->SetClientImage( TID_None );
	m_Create_Millena_Btn->SetBtnImage( TID_CharCreate_Millena, TID_CharCreate_Millena_On , TID_CharCreate_Millena_Da, TID_None );
	m_Create_Millena_Btn->SetVisible(false);
	
	//..........................................................................................................
	// 캐릭터 생성 레인 버튼
	//..........................................................................................................
	m_Create_Rain_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Rain_Btn , WID_CharSelect_Wnd,  PosX + 102, PosY + 106, 59, 76, false );	
	m_Create_Rain_Btn->SetClientImage( TID_None );
	m_Create_Rain_Btn->SetBtnImage( TID_CharCreate_Rain, TID_CharCreate_Rain_On , TID_CharCreate_Rain_Da , TID_None );
	m_Create_Rain_Btn->SetVisible(false);
	
	///-- 국가(Static)
	m_nationtext_millena= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd,PosX + 28, PosY + 142, 69, 17, false );
	m_nationtext_millena->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdOutline );
	m_nationtext_millena->SetTextColor( 255, 255, 200 );
	m_nationtext_millena->SetClientImage( TID_None );			
	m_nationtext_millena->SetAlignText( n_atCenter, n_atCenter );
	m_nationtext_millena->SetText( G_STRING_CHAR( IDS_MILLENA ) ); 
	m_nationtext_millena->SetFocusCheck( false );
	
	m_nationtext_rain= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd,PosX + 97, PosY + 142, 69, 17, false );
	m_nationtext_rain->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdOutline );
	m_nationtext_rain->SetTextColor( 255, 255, 200 );
	m_nationtext_rain->SetClientImage( TID_None );			
	m_nationtext_rain->SetAlignText( n_atCenter, n_atCenter );
	m_nationtext_rain->SetText( G_STRING_CHAR( IDS_RAIN ) ); 
	m_nationtext_rain->SetFocusCheck( false );
		
	//..........................................................................................................
	// 캐릭터 생성 남자 버튼 
	//..........................................................................................................
	m_Create_Male_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_MALE_Btn , WID_CharSelect_Wnd , PosX + 22, PosY +  198, 73, 17, false );
	m_Create_Male_Btn->SetClientImage( TID_None );
	m_Create_Male_Btn->SetBtnImage( TID_None, TID_CharCreate_BACK_SEX, TID_CharCreate_BACK_SEX, TID_None );	
	m_Create_Male_Btn->SetFontR( n_fRsvFontWndButton );
	m_Create_Male_Btn->SetTextColor( 255, 255, 255 );	
	m_Create_Male_Btn->SetText( G_STRING_CHAR( IDS_MAN ) );	
	m_Create_Male_Btn->SetVisible(false);

	//..........................................................................................................
	// 캐릭터 생성 여자 버튼
	//..........................................................................................................
	m_Create_Female_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_FEMALE_Btn , WID_CharSelect_Wnd , PosX +95, PosY +198, 73, 17, false ); 
	m_Create_Female_Btn->SetClientImage( TID_None );
	m_Create_Female_Btn->SetBtnImage( TID_None, TID_CharCreate_BACK_SEX, TID_CharCreate_BACK_SEX, TID_None );	
	m_Create_Female_Btn->SetFontR( n_fRsvFontWndButton );
	m_Create_Female_Btn->SetTextColor( 255, 255, 255 );	
	m_Create_Female_Btn->SetText( G_STRING_CHAR( IDS_WOMAN ) );	
	m_Create_Female_Btn->SetVisible(false);

	//..........................................................................................................
	// 줌 버튼
	//..........................................................................................................
	m_pZoomBtn = (CCheckButton*)nRui->CreateWnd( n_wtCheckButton , WID_CharacterZoom , WID_CharSelect_Wnd , PosX + 21 , PosY + 303 , 148 , 19 , false );
	m_pZoomBtn->SetClientImage( TID_None );
	m_pZoomBtn->SetBtnImage( TID_None , TID_None , TID_None, TID_None );
	m_pZoomBtn->SetVisible(false);

	
	//..........................................................................................................
	// 캐릭터 생성 생성 버튼
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_Create_Create_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Create_Btn , WID_CharSelect_Wnd , 424, 672, 175, 25 , false );
#else
	m_Create_Create_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Create_Btn , WID_CharSelect_Wnd , PosX +5 , PosY + 360 , 175 , 25 , false );
#endif // DECO_RENEWAL_MJH
	m_Create_Create_Btn->SetClientImage( TID_None );
	m_Create_Create_Btn->SetBtnImage( TID_CTRL_BTN175 , TID_CTRL_BTN175_OVR , TID_CTRL_BTN175_CLK , TID_CTRL_BTN175_DIS );
	m_Create_Create_Btn->SetVisible(false);
	m_Create_Create_Btn->SetFontR( n_fRsvFontWndButton );
	m_Create_Create_Btn->SetText( G_STRING_CHAR( IDS_WND_BT_CREATE_CH ));	

	//..........................................................................................................
	// 캐릭터 생성 취소 버튼
	//..........................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_Create_Cancel_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Cancel_Btn , WID_CharSelect_Wnd , 424, 698, 175, 25 , false );
#else
	m_Create_Cancel_Btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharCreate_Cancel_Btn , WID_CharSelect_Wnd , PosX +5, PosY + 420 , 175 , 25 , false );
#endif // DECO_RENEWAL_MJH
	m_Create_Cancel_Btn->SetClientImage( TID_None );
	m_Create_Cancel_Btn->SetBtnImage( TID_CTRL_BTN175 , TID_CTRL_BTN175_OVR , TID_CTRL_BTN175_CLK , TID_CTRL_BTN175_DIS );
	m_Create_Cancel_Btn->SetVisible(false);
	m_Create_Cancel_Btn->SetFontR( n_fRsvFontWndButton );
	m_Create_Cancel_Btn->SetText( G_STRING_CHAR( IDS_WND_COM_BACK ) );	
	
	//..........................................................................................................
	// 클래스 미리보기
	//..........................................................................................................
		
	m_pClassViewBtn = (CButton*)nRui->CreateWnd( n_wtButton , WID_CharacterClassView , WID_CharSelect_Wnd , PosX +5, PosY + 390 , 175 , 25 , false );
	m_pClassViewBtn->SetClientImage( TID_None );
	m_pClassViewBtn->SetBtnImage( TID_CTRL_BTN175 , TID_CTRL_BTN175_OVR , TID_CTRL_BTN175_CLK , TID_CTRL_BTN175_DIS );
	m_pClassViewBtn->SetVisible(false);
	m_pClassViewBtn->SetFontR( n_fRsvFontWndButton );
	m_pClassViewBtn->SetText( G_STRING_CHAR( IDS_WND_PREVIEW ) );	
	
	//..........................................................................................................
	// 캐릭터 생성 이름 에디트 박스
	//..........................................................................................................
	m_Create_Name_Edit = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_CharCreate_Name_EditBox, WID_CharSelect_Wnd, PosX + 24 , PosY + 58 , 142 , 17 , false );
	m_Create_Name_Edit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_Create_Name_Edit->SetTextSize( RP_MAX_PC_NAME - sizeof(RTCHAR) );
	m_Create_Name_Edit->SetClientImage( TID_None );
	m_Create_Name_Edit->SetClientColor( 0, 0, 0 );
	m_Create_Name_Edit->SetAlignText( n_atLeft, n_atCenter );
	m_Create_Name_Edit->SetBlank( 1, 1, 0, 0 );
	m_Create_Name_Edit->SetText(_RT("") );
	m_Create_Name_Edit->SetEditStyle( esText );
	m_Create_Name_Edit->SetReturnActionClear( false );
	m_Create_Name_Edit->InitEditBox();
	m_Create_Name_Edit->SetVisible(false);
	m_Create_Name_Edit->SetTextColor( 255, 255, 255 );

#ifdef DECO_RENEWAL_MJH
	
	for( int i = 0 ; i < MAX_PC_SLOT_NUM ; ++i )
	{
		int TxtPosY = 100*i;
		m_PcText[i].PcImgBox = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , 750 , 175+TxtPosY , 50, 50, false );
		m_PcText[i].PcImgBox->SetClientImage( TID_Tiris );
		m_PcText[i].PcImgBox->SetVisible(false);
		m_PcText[i].PcNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd , 810 , 175+TxtPosY, 200 , 50 , false );
		m_PcText[i].PcNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal ); 
		m_PcText[i].PcNameTxt->SetClientImage( TID_None );
		m_PcText[i].PcNameTxt->SetClientColor( 180, 175, 160 );
		m_PcText[i].PcNameTxt->SetAlignText( n_atLeft, n_atCenter );
		m_PcText[i].PcNameTxt->SetMultiLine( true );
		m_PcText[i].PcNameTxt->SetText(_RT("") );
		m_PcText[i].PcNameTxt->SetTextColor( 0, 0, 0 );
		m_PcText[i].PcNameTxt->SetVisible(false);

		m_PcText[i].PcClassTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd , 810 , 195+TxtPosY, 200 , 50 , false );
		m_PcText[i].PcClassTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal ); 
		m_PcText[i].PcClassTxt->SetClientImage( TID_None );
		m_PcText[i].PcClassTxt->SetClientColor( 180, 175, 160 );
		m_PcText[i].PcClassTxt->SetAlignText( n_atLeft, n_atCenter );
		m_PcText[i].PcClassTxt->SetMultiLine( true );
		m_PcText[i].PcClassTxt->SetText(_RT("") );
		m_PcText[i].PcClassTxt->SetTextColor( 0, 0, 0 );
		m_PcText[i].PcClassTxt->SetVisible(false);

		m_PcText[i].PcLevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd , 810 , 215+TxtPosY, 200 , 50 , false );
		m_PcText[i].PcLevelTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal ); 
		m_PcText[i].PcLevelTxt->SetClientImage( TID_None );
		m_PcText[i].PcLevelTxt->SetClientColor( 180, 175, 160 );
		m_PcText[i].PcLevelTxt->SetAlignText( n_atLeft, n_atCenter );
		m_PcText[i].PcLevelTxt->SetMultiLine( true );
		m_PcText[i].PcLevelTxt->SetText(_RT("") );
		m_PcText[i].PcLevelTxt->SetTextColor( 0, 0, 0 );
		m_PcText[i].PcLevelTxt->SetVisible(false);
	}
	
#endif // DECO_RENEWAL_MJH


#ifdef DECO_RENEWAL_MJH
	PosX = 790;
	PosY = 160;
#endif // DECO_RENEWAL_MJH
	
	// 캐릭터 선택 버튼들(버튼 클릭에서 캐릭터 오브젝트 클릭으로 바뀜 - 2009/04/27)
#ifdef DECO_RENEWAL_MJH	
	m_pSelectBtn[ 0 ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect0 , WID_CharSelect_Wnd , PosX , PosY  , 200  , 85  , false );
	//m_pSelectBtn[ 0 ]->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pSelectBtn[ 0 ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
#else
	m_pSelectBtn[ 0 ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect0 , WID_CharSelect_Wnd , 454 , 227  , 113  , 228  , false );
	m_pSelectBtn[ 0 ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
#endif // DECO_RENEWAL_MJH
	m_pSelectBtn[ 0 ]->SetClientImage( TID_None );
	m_pSelectBtn[ 0 ]->SetVisible(false);

#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)
	
	m_pSelectBtn[ 0 ]->SetEnable(false);
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)

#ifdef DECO_RENEWAL_MJH	
	m_pSelectBtn[ 1 ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect1 , WID_CharSelect_Wnd , PosX , PosY+100  , 200  , 85  , false );
	//m_pSelectBtn[ 1 ]->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pSelectBtn[ 1 ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
#else
	m_pSelectBtn[ 1 ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect1 , WID_CharSelect_Wnd , 314 , 227  , 113 , 228 , false );
	m_pSelectBtn[ 1 ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
#endif // DECO_RENEWAL_MJH
	m_pSelectBtn[ 1 ]->SetClientImage( TID_None );
	m_pSelectBtn[ 1 ]->SetVisible(false);

#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)
	
	m_pSelectBtn[ 1 ]->SetEnable(false);
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)

#ifdef DECO_RENEWAL_MJH	
	m_pSelectBtn[ 2 ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect2 , WID_CharSelect_Wnd , PosX , PosY+200  , 200  , 85  , false );
	//m_pSelectBtn[ 2 ]->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pSelectBtn[ 2 ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
#else
	m_pSelectBtn[ 2 ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect2 , WID_CharSelect_Wnd , 609 , 227 , 113 , 228 , false );
	m_pSelectBtn[ 2 ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
#endif
	m_pSelectBtn[ 2 ]->SetClientImage( TID_None );
	m_pSelectBtn[ 2 ]->SetVisible(false);

#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)
	
	m_pSelectBtn[ 2 ]->SetEnable(false);
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)

#ifdef DECO_RENEWAL_MJH	
	m_pSelectBtn[ 3 ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect3 , WID_CharSelect_Wnd , PosX , PosY+300  , 200  , 85  , false );
	//m_pSelectBtn[ 3 ]->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pSelectBtn[ 3 ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
#else
	m_pSelectBtn[ 3 ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect3 , WID_CharSelect_Wnd , 171 , 227 , 113 , 228 , false );	
	m_pSelectBtn[ 3 ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
#endif
	m_pSelectBtn[ 3 ]->SetClientImage( TID_None );
	m_pSelectBtn[ 3 ]->SetVisible(false);

#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)
	
	m_pSelectBtn[ 3 ]->SetEnable(false);
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)

#ifdef DECO_RENEWAL_MJH	
	m_pSelectBtn[ 4 ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect4 , WID_CharSelect_Wnd , PosX , PosY+400  , 200  , 85  , false );
//	m_pSelectBtn[ 4 ]->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_pSelectBtn[ 4 ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
#else
	m_pSelectBtn[ 4 ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_CharSelect4 , WID_CharSelect_Wnd , 750 , 227 , 113 , 228 , false );
	m_pSelectBtn[ 4 ]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
#endif
	m_pSelectBtn[ 4 ]->SetClientImage( TID_None );
	m_pSelectBtn[ 4 ]->SetVisible(false);

#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)
	
	m_pSelectBtn[ 4 ]->SetEnable(false);
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)
#ifdef DECO_RENEWAL_MJH
	PosX = 0;
#else
	PosX = 734;
#endif // DECO_RENEWAL_MJH
	
	//............................................................................................................
	// 클래스 미리보기 뒷 배경
	//............................................................................................................
	// 밀레나 클래스 뷰 뒷 배경
#ifdef DECO_RENEWAL_MJH
	m_ClassViewImage = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ClassView_Millena_Image , WID_CharSelect_Wnd ,  PosX, 80, 250, 60, false ); 
	m_ClassViewImage->SetClientImage( TID_CTRL_BTN175 );
#else
	m_ClassViewImage = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ClassView_Millena_Image , WID_CharSelect_Wnd ,  PosX, 191, 282, 230, false );
	m_ClassViewImage->SetClientImage( TID_PreView_Image );
#endif // DECO_RENEWAL_MJH
	m_ClassViewImage->SetVisible(false);	
	
	m_ClassViewImageSiGong = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_CharSelect_Wnd ,  PosX, 191, 130, 61, false );    
	
	if( g_RockClient.GetLanguage() == eLang_Korea )
	{
		m_ClassViewImageSiGong->SetClientImage( TID_prejob_sikongchanga );
	}
	else if( g_RockClient.GetLanguage() == eLang_Japan ) 
	{
		m_ClassViewImageSiGong->SetClientImage( TID_japan_preview_logo );
	}
	else   
	{
		m_ClassViewImageSiGong->SetClientImage( TID_china_preview_logo );
	}
		
	m_ClassViewImageSiGong->SetVisible(false);	
	
	//............................................................................................................
	// 클래스 미리보기 버튼
	//............................................................................................................
	
#ifdef DECO_RENEWAL_MJH
	nationimg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX , 140, 100, 125, false );
#else
	nationimg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 234, 191 + 6, 41, 50, false );
#endif // DECO_RENEWAL_MJH
	
#ifdef DECO_RENEWAL_MJH
	racetext = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 50,	 85 + 11,	91, 17, false );
	racetext->SetTextColor(0 ,0 ,0);
#else
	racetext = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 133,	 191 + 11,	91, 17, false );
#endif // DECO_RENEWAL_MJH
	racetext->SetFontR( n_fRsvFontWndStatic2 );

	///-- 공용 직업 TEXT
#ifdef DECO_RENEWAL_MJH
	jobtbox[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 133,	 191 + 36,	250, 17, false );
	jobtbox[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX ,	 191 + 78,	250, 17, false );
	jobtbox[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX ,	 191 + 196,	250, 17, false );
	jobtbox[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX ,	 191 + 113, 250, 17, false );
	jobtbox[4] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX ,	 191 + 141, 250, 17, false );
	jobtbox[5] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX ,	 191 + 169, 250, 17, false );
	jobtbox[6] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX ,	 191 + 223, 250, 17, false );
	jobtbox[7] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX ,	 191 + 250, 250, 17, false );
#else
	jobtbox[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 133,	 191 + 36,	91, 17, false );
	jobtbox[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 40,	 191 + 78,	91, 17, false );
	jobtbox[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 184,	 191 + 78,	91, 17, false );
	jobtbox[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 40,	 191 + 113, 91, 17, false );
	jobtbox[4] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 40,	 191 + 141, 91, 17, false );
	jobtbox[5] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 40,	 191 + 169, 91, 17, false );
	jobtbox[6] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX + 184,	 191 + 113, 91, 17, false );
#endif // DECO_RENEWAL_MJH
		
	jobtbox[0]->SetFontR( n_fRsvFontWndStatic2 );		
	jobtbox[1]->SetFontR( n_fRsvFontWndStatic2 );		
	jobtbox[2]->SetFontR( n_fRsvFontWndStatic2 );		
	jobtbox[3]->SetFontR( n_fRsvFontWndStatic2 );		
	jobtbox[4]->SetFontR( n_fRsvFontWndStatic2 );		
	jobtbox[5]->SetFontR( n_fRsvFontWndStatic2 );		
	jobtbox[6]->SetFontR( n_fRsvFontWndStatic2 );
#ifdef DECO_RENEWAL_MJH
	jobtbox[7]->SetFontR( n_fRsvFontWndStatic2 );
#endif // DECO_RENEWAL_MJH

	///-- 공용 직업 이미지
	///-- 이방인 / 방랑자 생략
#ifdef DECO_RENEWAL_MJH
	jobtimg[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 5,  191 + 75, 29, 22, false );
	jobtimg[2] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 149, 191 + 75, 29, 22, false );
	jobtimg[3] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 5,	191 + 110, 29, 22, false ); 
	jobtimg[4] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 5, 191 + 138, 29, 22, false );
	jobtimg[5] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 5, 191 + 166, 29, 22, false ); 
	jobtimg[6] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 149, 191 + 110, 29, 22, false );
	jobtimg[7] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 149, 191 + 138, 29, 22, false );
#else
	jobtimg[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 5,  191 + 75, 29, 22, false );
	jobtimg[2] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 149, 191 + 75, 29, 22, false );
	jobtimg[3] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 5,	191 + 110, 29, 22, false ); 
	jobtimg[4] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 5, 191 + 138, 29, 22, false );
	jobtimg[5] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 5, 191 + 166, 29, 22, false ); 
	jobtimg[6] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None , WID_CharSelect_Wnd , PosX + 149, 191 + 110, 29, 22, false );
#endif // DECO_RENEWAL_MJH	
	
	jobtimg[1]->SetClientImage( TID_PreView_Image );
	jobtimg[2]->SetClientImage( TID_PreView_Image );
	jobtimg[3]->SetClientImage( TID_PreView_Image );
	jobtimg[4]->SetClientImage( TID_PreView_Image );
	jobtimg[5]->SetClientImage( TID_PreView_Image );
	jobtimg[6]->SetClientImage( TID_PreView_Image );
#ifdef DECO_RENEWAL_MJH
	jobtimg[7]->SetClientImage( TID_PreView_Image );
#endif // DECO_RENEWAL_MJH

#ifdef DECO_RENEWAL_MJH
	m_SwordManBtn     = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_SwordmanBtn     , WID_CharSelect_Wnd, PosX + 40,	 191 + 78,	250, 17, false );
	m_ArcherBtn       = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_ArcherBtn       , WID_CharSelect_Wnd, PosX + 184, 191 + 78,	250, 17, false );
	m_KnightBtn       = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_KnightBtn       , WID_CharSelect_Wnd, PosX ,	 191 + 78,	250, 17, false );
	m_SlayerBtn       = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_SlayerBtn       , WID_CharSelect_Wnd, PosX ,	 191 + 113,	250, 17, false );
	m_MercenaryBtn    = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_MercenaryBtn    , WID_CharSelect_Wnd, PosX ,	 191 + 141, 250, 17, false ); 
	m_Soul_BreakerBtn = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_Soul_BreakerBtn , WID_CharSelect_Wnd, PosX ,	 191 + 169, 250, 17, false );
#else 
	m_SwordManBtn     = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_SwordmanBtn     , WID_CharSelect_Wnd, PosX + 40,	 191 + 78,	91, 16, false );
	m_ArcherBtn       = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_ArcherBtn       , WID_CharSelect_Wnd, PosX + 184,	 191 + 78,	91, 16, false );
	m_KnightBtn       = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_KnightBtn       , WID_CharSelect_Wnd, PosX + 40,	 191 + 113, 91, 16, false ); 
	m_SlayerBtn       = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_SlayerBtn       , WID_CharSelect_Wnd, PosX + 40,	 191 + 141, 91, 16, false );
	m_MercenaryBtn    = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_MercenaryBtn    , WID_CharSelect_Wnd, PosX + 40,	 191 + 169, 91, 16, false );
	m_Soul_BreakerBtn = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_Soul_BreakerBtn , WID_CharSelect_Wnd, PosX + 184,	 191 + 113, 91, 16, false );
#endif // DECO_RENEWAL_MJH


	m_SwordManBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_SwordManBtn->SetClientImage( TID_None );
	m_SwordManBtn->SetBtnImage( TID_None , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR );
	m_SwordManBtn->SetVisible(false);
	
	
	m_ArcherBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_ArcherBtn->SetClientImage( TID_None );
	m_ArcherBtn->SetBtnImage( TID_None  , TID_prejob_OVR , TID_prejob_OVR, TID_prejob_OVR );
	m_ArcherBtn->SetVisible(false);

	
	m_KnightBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_KnightBtn->SetClientImage( TID_None );
	m_KnightBtn->SetBtnImage( TID_None , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR );
	m_KnightBtn->SetVisible(false);

	
	m_SlayerBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_SlayerBtn->SetClientImage( TID_None );
	m_SlayerBtn->SetBtnImage( TID_None  , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR);
	m_SlayerBtn->SetVisible(false);
	
	
	m_MercenaryBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_MercenaryBtn->SetClientImage( TID_None );
	m_MercenaryBtn->SetBtnImage( TID_None  , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR );
	m_MercenaryBtn->SetVisible(false);

	
	m_Soul_BreakerBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_Soul_BreakerBtn->SetClientImage( TID_None );
	m_Soul_BreakerBtn->SetBtnImage( TID_None  , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR );
	m_Soul_BreakerBtn->SetVisible(false);
#ifdef DECO_RENEWAL_MJH
	m_MagicianBtn     = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_MagicianBtn     , WID_CharSelect_Wnd, PosX + 40,	 191 + 78,	250 ,17, false );
	m_ShamanBtn       = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_ShamanBtn       , WID_CharSelect_Wnd, PosX + 184,	 191 + 78,	250, 17, false );
	m_Holy_AvengerBtn = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_Holy_AvengerBtn , WID_CharSelect_Wnd, PosX + 40,	 191 + 113, 250, 17, false ); 
	m_PriestBtn       = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_PriestBtn       , WID_CharSelect_Wnd, PosX ,	 191 + 196 , 250, 17, false );
	m_ProminasBtn     = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_ProminasBtn     , WID_CharSelect_Wnd, PosX ,	 191 + 223, 250, 17, false );
	m_SummonerBtn     = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_SummonerBtn     , WID_CharSelect_Wnd, PosX ,	 191 + 250, 250, 17, false );
#else
	m_MagicianBtn     = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_MagicianBtn     , WID_CharSelect_Wnd, PosX + 40,	 191 + 78,	91, 16, false );
	m_ShamanBtn       = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_ShamanBtn       , WID_CharSelect_Wnd, PosX + 184,	 191 + 78,	91, 16, false );
	m_Holy_AvengerBtn = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_Holy_AvengerBtn , WID_CharSelect_Wnd, PosX + 40,	 191 + 113, 91, 16, false ); 
	m_ProminasBtn     = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_ProminasBtn     , WID_CharSelect_Wnd, PosX + 40,	 191 + 141, 91, 16, false );
	m_PriestBtn       = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_PriestBtn       , WID_CharSelect_Wnd, PosX + 40,	 191 + 169, 91, 16, false );
	m_SummonerBtn     = (CRadioButton*)nRui->CreateWnd( n_wtRadioButton, WID_ClassView_SummonerBtn     , WID_CharSelect_Wnd, PosX + 184,	 191 + 113, 91, 16, false );
#endif // DECO_RENEWAL_MJH
	

	m_MagicianBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_MagicianBtn->SetClientImage( TID_None );
	m_MagicianBtn->SetBtnImage( TID_None  , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR);
	m_MagicianBtn->SetVisible(false);

	
	m_ShamanBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_ShamanBtn->SetClientImage( TID_None );
	m_ShamanBtn->SetBtnImage( TID_None , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR );
	m_ShamanBtn->SetVisible(false);

	
	m_ProminasBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_ProminasBtn->SetClientImage( TID_None );
	m_ProminasBtn->SetBtnImage( TID_None , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR );
	m_ProminasBtn->SetVisible(false);

	
	m_PriestBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_PriestBtn->SetClientImage( TID_None );
	m_PriestBtn->SetBtnImage( TID_None , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR );
	m_PriestBtn->SetVisible(false);

	
	m_Holy_AvengerBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_Holy_AvengerBtn->SetClientImage( TID_None );
	m_Holy_AvengerBtn->SetBtnImage( TID_None , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR );
	m_Holy_AvengerBtn->SetVisible(false);

	
	m_SummonerBtn->InitRadioButton( n_cmUnChecked, _RT("") );
	m_SummonerBtn->SetClientImage( TID_None );
	m_SummonerBtn->SetBtnImage( TID_None , TID_prejob_OVR , TID_prejob_OVR , TID_prejob_OVR );
	m_SummonerBtn->SetVisible(false);

	// 라디오 버튼 그룹화
	nRui->SetGroupWnd( 12 , WID_ClassView_SwordmanBtn	   ,
							WID_ClassView_SwordmanBtn      ,  
							WID_ClassView_ArcherBtn        , 		
							WID_ClassView_MagicianBtn      ,
							WID_ClassView_ShamanBtn        ,
							WID_ClassView_KnightBtn		   ,		
							WID_ClassView_MercenaryBtn     ,	
							WID_ClassView_SlayerBtn        ,		
							WID_ClassView_Soul_BreakerBtn  ,
							WID_ClassView_ProminasBtn      ,		
							WID_ClassView_PriestBtn        ,		
							WID_ClassView_Holy_AvengerBtn  ,	
							WID_ClassView_SummonerBtn 
					 );

// 헬프 뷰 뒷 배경
	m_HelpImage = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_Help_Image , WID_CharSelect_Wnd , PosX, 423, 282, 180, false );    		
	m_HelpImage->SetWndTileSizeLR( 40, 29 );
	m_HelpImage->SetWndTileSizeTB( 31, 11 ); 	
	m_HelpImage->SetFocusCheck( false );
	m_HelpImage->SetClientImage( TID_CTRL_WND_L1 );		
	m_HelpImage->SetVisible(false);
	m_HelpImage->SetAdjustPosition( 0, 0 );

		///-- Image9
#ifdef DECO_RENEWAL_MJH
	m_HelpContentImage = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_CharSelect_Wnd, PosX+130, 160, 230, 100, false );       	
#else
	m_HelpContentImage = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_CharSelect_Wnd, PosX+12, 460, 258, 100, false );       	
#endif //DECO_RENEWAL_MJH
	m_HelpContentImage->SetWndTileSizeLR( 9, 9 );
	m_HelpContentImage->SetWndTileSizeTB( 8, 8 );
	m_HelpContentImage->SetFocusCheck( false );
	m_HelpContentImage->SetClientImage( TID_CTRL_CONTBACK_L1 );
	m_HelpContentImage->SetAdjustPosition( 0, 0 );
	
	
	
	
	//............................................................................................................
	// 도움말 창
	//............................................................................................................
#ifdef DECO_RENEWAL_MJH
	m_pHelpText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_ClassView_HelpText, WID_CharSelect_Wnd, 
											  PosX + 133 , 163 , 250 - 14 , 7 * 14 , false );//281
#else
	m_pHelpText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_ClassView_HelpText, WID_CharSelect_Wnd, 
											  PosX + 15 , 423 + 40 , 281 - 14 , 6 * 14 , false );
#endif // DECO_RENEWAL_MJH
	m_pHelpText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pHelpText->SetClientImage( TID_None );
	m_pHelpText->SetClientColor( 180, 175, 160 );
	m_pHelpText->SetMultiLine( true );	
	m_pHelpText->SetLineInterval(3);
	m_pHelpText->SetTextColor( 255 ,255 ,255 );
	m_pHelpText->SetHID( HID_None );
	m_pHelpText->SetText(_RT("") );

	racehelptext = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_CharSelect_Wnd, PosX ,	 423 ,	280, 30, false );
	racehelptext->SetFontR( n_fRsvFontTitle );
	racehelptext->SetAlignText( n_atCenter, n_atCenter );
	racehelptext->SetText( G_STRING_CHAR( IDS_WND_DESC_CLASS ) ); 

	
	//............................................................................................................
	// 클래스 미리보기 뒤로 버튼
	//............................................................................................................
	m_ClassViewReturn = (CButton*)nRui->CreateWnd( n_wtButton , WID_CharacterClassViewReturn , WID_CharSelect_Wnd , PosX + 109 , 570 , 58 , 25 , false );
	m_ClassViewReturn->SetClientImage( TID_None );
	m_ClassViewReturn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_ClassViewReturn->SetFontR( n_fRsvFontWndButton );
	m_ClassViewReturn->SetText( G_STRING_CHAR( IDS_WND_COM_BACK ) );	
	m_ClassViewReturn->SetVisible(false);
}

void CCharSelectWnd::ChangeFaceAndHair(void)
{
	DWORD Unique = -1;

	
	switch( m_SEQState )
	{
#ifdef DECO_RENEWAL_MJH
	case SEQ_MILENA_CLASS:
		Unique  = CLASS_UNIQUE;
#else
	case SEQ_MILENA_CREATE:
		Unique = PosInfo[ CSW_MILLENA_POS ].Unique;

#endif // DECO_RENEWAL_MJH
		
			
			
			break;
#ifdef DECO_RENEWAL_MJH
	case SEQ_RAIN_CLASS:
#else
	case SEQ_RAIN_CREATE:
#endif // DECO_RENEWAL_MJH
			
			Unique = PosInfo[ CSW_RAIN_POS ].Unique;

			break;

		default:
			return;
	}
	
	int table_index = g_Pc.FindUser( Unique );

	int AttrIndex = g_Pc.GetAttrIndex( m_Citizen , m_Gender );

	g_Pc.m_PcTable[ g_Pc.m_Pc[table_index].m_nTableIndex ].char_info.theFace = m_pSelectFaceCode[ m_Gender ][ m_SelectFaceNum ];

	g_Pc.m_PcTable[ g_Pc.m_Pc[table_index].m_nTableIndex ].char_info.theWearItem[n_WearSlot_Helmet].theCode =
		m_pSelectHairCode[ m_Gender ][ m_SelectHairNum ];

	
	g_Pc.m_Pc[table_index].m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( AttrIndex , m_pSelectHairCode[ m_Gender ][ m_SelectHairNum ] );
		g_Pc.m_Pc[table_index].SetItemBuffer( n_Wear_Hair );	
	
	g_Pc.m_Pc[table_index].m_pItm_index[n_Wear_Face] = g_Pc_Manager.GetItemInfo_FileCode12( AttrIndex , m_pSelectFaceCode[ m_Gender ][ m_SelectFaceNum ] );
		g_Pc.m_Pc[table_index].SetItemBuffer( n_Wear_Face );	
	
	RLG2( "Change Hair ( %d ) , Face ( %d )\n" , m_pSelectHairCode[ m_Gender ][ m_SelectHairNum ] , m_pSelectFaceCode[ m_Gender ][ m_SelectFaceNum ] );
}

//업데이트케릭터
void CCharSelectWnd::Update()
{
		
	if( m_SEQState == SEQ_SELECT_ANI )
	{
		BOOL Find = FALSE;

		for( int i = 0 ; i < 5 ; ++i )
		{
			if( pCharList[ i ] != NULL )
			{
				if( pCharList[ i ]->m_curt_event.type != 0 )
				{
					Find = TRUE;

					break;
				}
			}
		}

		if( Find == FALSE )
		{
			m_SEQState = SEQ_SELECT_CHAR;
		}
	
		return;
	}

#ifdef HHM_IF_CHAR_DEL_OPEN_DONT_BT_CLICK// 캐릭터 삭제창이 열렸을 때는 버튼들이 클릭되면 안된다(2009/06/15)
	
	if(m_SEQState == SEQ_SELECT_CHAR && !g_CharDelWndProc.IsVisible)
	{
		if(m_Select_Start_Btn)
			m_Select_Start_Btn->SetVisible(true);
		
		if(m_Select_Create_Btn)
			m_Select_Create_Btn->SetVisible(true);
		
		if(m_Select_Delete_Btn)
			m_Select_Delete_Btn->SetVisible(true);
		
		if(m_Select_Server_Btn0)
			m_Select_Server_Btn0->SetVisible(true);
	}
	
#endif// #define HHM_IF_CHAR_DEL_OPEN_DONT_BT_CLICK// 캐릭터 삭제창이 열렸을 때는 버튼들이 클릭되면 안된다(2009/06/15)
}

void CCharSelectWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
		case WID_CharCreate_Hair_Left_Btn:	
	
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_SelectHairNum--;

				if( m_SelectHairNum < 0 )
					m_SelectHairNum = ( TABLE_HAIR_MAX_COUNT - 1 );
				
				ChangeFaceAndHair();
				ChangeType();
			}			
			
			break;
		
		case WID_CharCreate_Hair_Right_Btn:	
			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_SelectHairNum++;

				if( m_SelectHairNum >= TABLE_HAIR_MAX_COUNT )
					m_SelectHairNum = 0;
				
				ChangeFaceAndHair();
				ChangeType();
			}			
			
			break;

		case WID_CharCreate_Face_Left_Btn:	
	
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_SelectFaceNum--;

				if( m_SelectFaceNum < 0 )
					m_SelectFaceNum = ( TABLE_FACE_MAX_COUNT - 1 );
				
				ChangeFaceAndHair();
				ChangeType();
			}			
			
			break;
		
		case WID_CharCreate_Face_Right_Btn:	
			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_SelectFaceNum++;

				if( m_SelectFaceNum >= TABLE_FACE_MAX_COUNT )
					m_SelectFaceNum = 0;
				
				ChangeFaceAndHair();
				ChangeType();
			}			
			
			break;

		case WID_CharCreate_Cancel_Btn:
			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
#ifdef DECO_RENEWAL_MJH
				if( m_SEQState == SEQ_MILENA_CLASS )
#else
				if( m_SEQState == SEQ_MILENA_CREATE )
#endif //DECO_RENEWAL_MJH
				{
					StartReturnMillenaToSelect();
				}
#ifdef DECO_RENEWAL_MJH
				else if( m_SEQState == SEQ_RAIN_CLASS )
#else
				else if( m_SEQState == SEQ_RAIN_CREATE )
#endif //DECO_RENEWAL_MJH	
				{
					StartReturnRainToSelect();
				}
#ifdef DECO_RENEWAL_MJH
				g_Pc.DisappearPC( CLASS_UNIQUE );
#endif //DECO_RENEWAL_MJH
			}
			

			break;
		
		case WID_MessageWnd:
			if( EMsg->Notify == NM_MSG_OK )
			{
				CharSelectWnd.DeletePlayer();
			}			
			break;
			
		case WID_CharSelect_Delete_Btn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				char tmp[128]={0,};
				g_CharDelWndProc.Open();

			#ifdef HHM_IF_CHAR_DEL_OPEN_DONT_BT_CLICK// 캐릭터 삭제창이 열렸을 때는 버튼들이 클릭되면 안된다(2009/06/15)
				
				if(m_Select_Start_Btn)
					m_Select_Start_Btn->SetVisible(false);
				
				if(m_Select_Create_Btn)
					m_Select_Create_Btn->SetVisible(false);
				
				if(m_Select_Delete_Btn)
					m_Select_Delete_Btn->SetVisible(false);
				
				if(m_Select_Server_Btn0)
					m_Select_Server_Btn0->SetVisible(false);
				
			#endif// #define HHM_IF_CHAR_DEL_OPEN_DONT_BT_CLICK// 캐릭터 삭제창이 열렸을 때는 버튼들이 클릭되면 안된다(2009/06/15)
			}
			break;

		case WID_CharCreate_Create_Btn:

			if( EMsg->Notify == NM_BTN_CLICK )
			{
				CreateCharacterOK();
			}

			break;

		// 캐릭터 선택창에 생성 버튼
		case WID_CharSelect_Create_Btn:

			if( EMsg->Notify == NM_BTN_CLICK )
			{
				bool IsView = g_CharCreateWndProc.IsVisible;
				IsView = !IsView;
#ifdef DECO_RENEWAL_MJH
					m_ViewClass = Knight;
					StartMillenaCameraWalkPostion();
					
#else
				if( IsView )
				{
					g_CharCreateWndProc.Open();
									
				#ifdef HHM_IF_CHAR_DEL_OPEN_DONT_BT_CLICK// 캐릭터 삭제창이 열렸을 때는 버튼들이 클릭되면 안된다(2009/06/15)
					
					if(m_Select_Start_Btn)
						m_Select_Start_Btn->SetVisible(false);
					
					if(m_Select_Create_Btn)
						m_Select_Create_Btn->SetVisible(false);
					
					if(m_Select_Delete_Btn)
						m_Select_Delete_Btn->SetVisible(false);
					
					if(m_Select_Server_Btn0)
						m_Select_Server_Btn0->SetVisible(false);
					
				#endif// #define HHM_IF_CHAR_DEL_OPEN_DONT_BT_CLICK// 캐릭터 삭제창이 열렸을 때는 버튼들이 클릭되면 안된다(2009/06/15)
				}
				else
					g_CharCreateWndProc.Close();
#endif //DECO_RENEWAL_MJH			
			}
			
			break;
		
		case WID_CharCreate_Millena_Btn:
			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				if( m_Citizen == MILLENA )
					break;
				
				StartRainToMillena();
			}

			break;

		case WID_CharCreate_Rain_Btn:
			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				if( m_Citizen == RAIN )
					break;

				StartMillenaToRain();
			}

			break;

		case WID_CharCreate_MALE_Btn:
			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				if( m_Gender == MALE )
					break;

				m_Gender = MALE;
#ifdef DECO_RENEWAL_MJH
#else
				CreateCharacter( m_Citizen , m_Gender );
#endif // DECO_RENEWAL_MJH	
				ChangeFaceAndHair();
			}

			break;

		case WID_CharCreate_FEMALE_Btn:
			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				if( m_Gender == FEMALE )
					break;

				m_Gender = FEMALE;
#ifdef DECO_RENEWAL_MJH
#else
				CreateCharacter( m_Citizen , m_Gender );
#endif // DECO_RENEWAL_MJH
				ChangeFaceAndHair();
			}
			
			break;


		case WID_CharSelect_Start_Btn:

			if( EMsg->Notify == NM_BTN_CLICK )
			{
				g_RockClient.SelectCharacter( m_SelectNum );
				g_RockClient.SetLoginStep( nServerSelectOK );

				g_Camera.Reset();
				CloseAll();

				g_RockClient.m_IsReConnect = FALSE;
			
				m_ISChangeSkyBox = TRUE;
				
				LoadingBarWnd.m_IsMakeImg = true;
				
				g_Particle.DelEffSetCon( EF_CORE0 , 100 , -1 , -1 );	
			}
			
			break;

		case WID_CharCreate_Rot_Left_Btn:
			
			if( EMsg->Notify == NM_BTN_PRESS )
			{
				ChangeRotY( CREATE_ROTY_ANGLE );
			}

			break;

		case WID_CharCreate_Rot_Right_Btn:
			
			if( EMsg->Notify == NM_BTN_PRESS )
			{
				ChangeRotY( -CREATE_ROTY_ANGLE );
			}

			break;

		case WID_CharSelect_Server_Btn0:
			//서버선택 수정
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				g_Particle.DelEffSetCon( EF_CORE0 , 100 , -1 , -1 );				
				g_RockClient.SelecttoServerList();

				g_RockClient.CloseLoginState();
				g_Particle.m_Fade.Start( FADE_STATE_LOOP , g_Particle.m_Fade.GetColor() , 
										   D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) ,
										   D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) ,
											1 , 2500 , FALSE );
				g_RockClient.LogOff();
				if( g_RockClient.ReLoginProc( ) )
				{	
					nRui->SendEvent( WID_CharSelect_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

					g_RockClient.m_IsReConnect = TRUE;
					m_ISChangeSkyBox = TRUE;

					g_Camera.SetStartFrame( 0 , g_lpDevice );
					g_RockClient.m_LoginShutter.Reset();
				}
				

			}
			break;

		// 줌 버튼
		case WID_CharacterZoom:	
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					// 카메라가 에니메이션 중일때 버튼 눌러져도 무시
					switch( m_SEQState )
					{
						case SEQ_CAMERA_ZOOM_MILLENA:
						case SEQ_CAMERA_ZOOM_OUT_MILLENA:
						case SEQ_CAMERA_ZOOM_RAIN:
						case SEQ_CAMERA_ZOOM_OUT_RAIN:
							{
								if( m_ISZoom )
								{
									nRui->SendEvent( WID_CharacterZoom ,n_emSetChecked , 0 , 0 , 0 , 0 );
								}
								else
								{
									nRui->SendEvent( WID_CharacterZoom , n_emSetUnChecked , 0 , 0 , 0 , 0 );
								}
							}
							break;
						
						default:
						{
							SetCloseAllBtn();

							if( n_cmChecked == m_pZoomBtn->GetCheckMark() )
							{
								m_ISZoom = TRUE;
							}
							else
							{
								m_ISZoom = FALSE;
							}
						
							if( m_ISZoom )
							{
								if( m_SEQState == SEQ_MILENA_CREATE )
								{
									m_SEQState = SEQ_CAMERA_ZOOM_MILLENA;
									
									g_Camera.SetAnimation( 8 );
								}
								else if( m_SEQState == SEQ_RAIN_CREATE )
								{
									m_SEQState = SEQ_CAMERA_ZOOM_RAIN;
									g_Camera.SetAnimation( 10 );
								}
							}
							else
							{
								if( m_SEQState == SEQ_MILENA_CREATE )
								{
									m_SEQState = SEQ_CAMERA_ZOOM_OUT_MILLENA;
									g_Camera.SetAnimation( 7 );
								}
								else if( m_SEQState == SEQ_RAIN_CREATE )
								{
									m_SEQState = SEQ_CAMERA_ZOOM_OUT_RAIN;
									g_Camera.SetAnimation( 9 );
								}
							}

							g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::EndZoomProcess);
						}
						break;
					
					}
					
				}
				
			}
			break;
		
		//........................................................................................................
		// 클래스 미리보기
		//........................................................................................................
		case WID_CharacterClassView:
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					
					if( m_SEQState == SEQ_MILENA_CREATE )
					{
#ifdef DECO_RENEWAL_MJH 
						m_ViewClass = Knight;
						m_KnightBtn->InitRadioButton( n_cmChecked, _RT("") );
#else
						m_ViewClass = Swordman;
						m_SwordManBtn->InitRadioButton( n_cmChecked, _RT("") );
#endif // DECO_RENEWAL_MJH 	
					}
					else if( m_SEQState == SEQ_RAIN_CREATE )
					{
						m_ViewClass = Magician;
						m_MagicianBtn->InitRadioButton( n_cmChecked, _RT("") );
					}

					CreateClassCharacter();
				}
			}
			break;

		case WID_CharacterClassViewReturn:
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					if( m_SEQState == SEQ_MILENA_CLASS )
					{
						m_SEQState = SEQ_MILENA_CREATE;
					}
					else if( m_SEQState == SEQ_RAIN_CLASS )
					{
						m_SEQState = SEQ_RAIN_CREATE;
					}

					g_Pc.DisappearPC( CLASS_UNIQUE );

					if( g_RockClient.GetLanguage() == eLang_Korea )
					{
						g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , CLASS_UNIQUE , 
												 -1 );		
					}

					CreateCharacter( m_Citizen , m_Gender );
					ChangeFaceAndHair();

					SetCreateCharacterBtn();
				}
			}
			break;

		case WID_ClassView_SummonerBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Psyche;

				CreateClassCharacter();
			}
			break;

		case WID_ClassView_SwordmanBtn:   
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Swordman;

				CreateClassCharacter();
			}
			break;
		case WID_ClassView_ArcherBtn:      
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Archer;

				CreateClassCharacter();
			}
			break;
		case WID_ClassView_MagicianBtn:    
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Magician;

				CreateClassCharacter();
			}
			break;
		case WID_ClassView_ShamanBtn:      
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Shaman;

				CreateClassCharacter();
			}
			break;
		case WID_ClassView_KnightBtn:		 
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Knight;

				CreateClassCharacter();
			}
			break;
		case WID_ClassView_MercenaryBtn:   
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Mercenary;

				CreateClassCharacter();
			}
			break;
		case WID_ClassView_SlayerBtn:      
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Slayer;

				CreateClassCharacter();
			}
			break;
		case WID_ClassView_Soul_BreakerBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Sniper;

				CreateClassCharacter();
			}
			break;
		case WID_ClassView_ProminasBtn:    
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Prominas;

				CreateClassCharacter();
			}
			break;
		case WID_ClassView_PriestBtn:      
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Priest;

				CreateClassCharacter();
			}
			break;
		case WID_ClassView_Holy_AvengerBtn:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				m_ViewClass = Holy_Avenger;

				CreateClassCharacter();
			}
			break;

		case WID_CharSelect0:
#ifdef DECO_RENEWAL_MJH

			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SelectCharacter( (int)( EMsg->FocusWnd - WID_CharSelect0 ) );
			}
			break;
#endif //DECO_RENEWAL_MJH
		case WID_CharSelect1:
#ifdef DECO_RENEWAL_MJH
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SelectCharacter( (int)( EMsg->FocusWnd - WID_CharSelect0 ) );
			}
			break;
#endif //DECO_RENEWAL_MJH
		case WID_CharSelect2:
#ifdef DECO_RENEWAL_MJH

			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SelectCharacter( (int)( EMsg->FocusWnd - WID_CharSelect0 ) );
			}
			break;
#endif //DECO_RENEWAL_MJH
		case WID_CharSelect3:
#ifdef DECO_RENEWAL_MJH
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				SelectCharacter( (int)( EMsg->FocusWnd - WID_CharSelect0 ) );
			}
			break;
#endif //DECO_RENEWAL_MJH
		case WID_CharSelect4:
			if( EMsg->Notify == NM_BTN_CLICK )
			{
#ifdef DECO_RENEWAL_MJH
				SelectCharacter( (int)( EMsg->FocusWnd - WID_CharSelect0 ) );
#else
				SelectCharacterMove( (int)( EMsg->FocusWnd - WID_CharSelect0 ) );
#endif //DECO_RENEWAL_MJH
			}
			break;
		
		case WID_ClassView_HelpScrollBar:
			if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
			{
					m_pHelpText->SetStartLine( (int)EMsg->fParam );
			}
#ifdef DECO_RENEWAL_MJH
		case WID_CharSelect_exit_Btn: // 게임종료
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				nRui->SendEvent( WID_CharSelect_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				SendMessage( g_RockClient.GetApp(), WM_CLOSE, NULL, 1 );
			}
			break;
#endif //DECO_RENEWAL_MJH

	}
}

void CCharSelectWnd::SelectCharacterMove(int Num)
{
	
	g_CharDelWndProc.Close();

	if( Num == m_SelectNum )
	{
		return;
	}
	

	m_Select_Start_Btn->SetVisible( TRUE );
	m_Select_Start_Btn->SetBtnHold( TRUE );

	m_Select_Delete_Btn->SetVisible( TRUE );
	m_Select_Delete_Btn->SetBtnHold( TRUE );

	g_Map.InitMapData();
	
	if( m_SelectNum != -1 )
	{
		CM_MOVE( m_SelectNum , m_SelectNum );
	}
	
	CM_MOVE( Num , 5 );
	
	m_SelectNum = Num;

	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();

	m_pSelectChar = &pChar[ m_SelectNum ];

	if( m_SelectNum != -1 )
	{
		if( ( pChar[ m_SelectNum ].theCore%10 ) == n_CharacterCore )
		{
			m_Select_Start_Btn->SetBtnHold( FALSE );
			m_Select_Delete_Btn->SetBtnHold( FALSE );

			char TempStr[ 1024 ] = "";

			Rsprintf( RWCHAR(TempStr) , _RT("%s : %s") ,G_STRING_CHAR( IDS_CHARSTATE_NAME2 ), RWCHAR(pChar[ m_SelectNum ].theData.thePCName) );
			m_Select_Name_Edit->SetText( TempStr );
			
			Rsprintf( RWCHAR(TempStr) , _RT("%s : %d") ,G_STRING_CHAR( IDS_CHARSTATE_LEVEL2 ), RWCHAR(pChar[ m_SelectNum ].theData.theLevel) );
			m_Select_Level_Edit->SetText( TempStr );

		
			SetMainClassNameString( pChar[ m_SelectNum ].theData.theMainClass );

			m_SelectClassWnd->SetVisible( TRUE );
			m_Select_Name_Edit->SetVisible( TRUE );
			m_Select_Level_Edit->SetVisible( TRUE );


		}
		else if( ( pChar[ m_SelectNum ].theCore%10 ) == n_CoreItSelf )
		{
			m_SelectClassWnd->SetText( G_STRING_CHAR( IDS_WND_CHAR_CORE ) );

			m_SelectClassWnd->SetVisible( TRUE );
			m_Select_Name_Edit->SetVisible( FALSE );
			m_Select_Level_Edit->SetVisible( FALSE );
		}
			
	}
}

void CCharSelectWnd::ChangeRotY(float Angle)
{
	DWORD Unique = -1;
	
	switch( m_SEQState )
	{
#ifdef DECO_RENEWAL_MJH
	case SEQ_MILENA_CLASS:
		Unique  = CLASS_UNIQUE;
#else
	case SEQ_MILENA_CREATE:
		Unique = PosInfo[ CSW_MILLENA_POS ].Unique;
#endif //DECO_RENEWAL_MJH
				
			break;

#ifdef DECO_RENEWAL_MJH
	case SEQ_RAIN_CLASS:
#else
	case SEQ_RAIN_CREATE:
#endif //DECO_RENEWAL_MJH
			Unique = PosInfo[ CSW_RAIN_POS ].Unique;
			break;

		default:
			return;
	}
	
	int table_index = g_Pc.FindUser( Unique );

	g_Pc.m_Pc[table_index].m_Mov.now_Yrad = g_Pc.m_Pc[table_index].m_Mov.now_Yrad + D3DXToRadian( Angle );
}


void CCharSelectWnd::ControlAllColse(void)
{
	m_SelectClassWnd->SetVisible(false);					
	m_SelectNameBoxImage->SetVisible(false);					// 캐릭터 선택 이름 배경 그림
	selectchartext->SetVisible(false);					
	m_SelectCreateBoxImage->SetVisible(false);
	m_Select_Start_Btn->SetVisible(false);						// 캐릭터 선택 시작 버튼
	m_Select_Server_Btn0->SetVisible(false);					// 캐릭터 선택 서버 버튼
	m_Select_Create_Btn->SetVisible(false);						// 캐릭터 선택 생성 버튼

	m_Select_Name_Edit->SetVisible(false);						// 캐릭터 이름 에디트 박스

	m_Select_Level_Edit->SetVisible(false);						// 캐릭터 레벨 박스

	m_Create_Rot_Left_Btn->SetVisible(false);					// 캐릭터 생성 왼쪽    버튼
	m_Create_Rot_Right_Btn->SetVisible(false);					// 캐릭터 생성 오른 쪽 버튼
	tbox[0]->SetVisible( false );
	tbox[1]->SetVisible( false );
	tbox[2]->SetVisible( false );
	tbox[3]->SetVisible( false );
	tbox[4]->SetVisible( false );
	tbox[5]->SetVisible( false );
	tbox[6]->SetVisible( false );

	racehelptext->SetVisible( false );
	

	
	m_Create_Millena_Btn->SetVisible(false);					// 캐릭터 생성 밀레나 버튼
	m_Create_Rain_Btn->SetVisible(false);						// 캐릭터 생성 레인 버튼
	m_nationtext_rain->SetVisible(false);						
	m_nationtext_millena->SetVisible(false);						

	m_Create_Male_Btn->SetVisible(false);						// 캐릭터 생성 남자 버튼
	m_Create_Female_Btn->SetVisible(false);						// 캐릭터 생성 여자 버튼

	m_Create_Hair_Left_Btn->SetVisible(false);					// 캐릭터 생성 머리 왼쪽   버튼
	m_Create_Hair_Right_Btn->SetVisible(false);					// 캐릭터 생성 머리 오른쪽 버튼

	m_Create_Face_Left_Btn->SetVisible(false);					// 캐릭터 생성 얼굴 왼쪽   버튼
	m_Create_Face_Right_Btn->SetVisible(false);					// 캐릭터 생성 얼굴 오른쪽 버튼

	m_Create_Create_Btn->SetVisible(false);						// 캐릭터 생성 생성 버튼
	m_Create_Cancel_Btn->SetVisible(false);						// 캐릭터 생성 취소 버튼

	m_Create_HelpImage->SetVisible(false);					// 캐릭터 생성 이름 배경 그림
	m_Create_Name_Edit->SetVisible(false);						// 캐릭터 생성 이름 에디트 박스
	m_CreateDlgmage->SetVisible(false);							// 캐릭터 생성 뒷 배경

	m_Select_Delete_Btn->SetVisible( false );

#ifdef DECO_RENEWAL_MJH

#endif // DECO_RENEWAL_MJH
}

//
void CCharSelectWnd::ControlStateOpen(void)
{
	ControlAllColse();
	
	switch( m_State )
	{
		//......................................................................................................
		// 아무것도 없을때
		//......................................................................................................
		case CSW_SELECT_NONE:
			break;

		//......................................................................................................
		// 코어만 있을때
		//......................................................................................................
		case CSW_SELECT_CORE:
			
			m_SelectCreateBoxImage->SetVisible(TRUE);
			m_Select_Server_Btn0->SetVisible(true);						// 캐릭터 선택 서버 버튼
			m_Select_Create_Btn->SetVisible(true);						// 캐릭터 선택 생성 버튼
			break;

		//......................................................................................................
		// 캐릭터 있을때
		//......................................................................................................
		case CSW_SELECT_EXIST:

			m_SelectClassWnd->SetVisible(true);					
			
			m_SelectNameBoxImage->SetVisible(true);						// 캐릭터 선택 이름 배경 그림
			selectchartext->SetVisible(true);					
			
			m_Select_Start_Btn->SetVisible(true);						// 캐릭터 선택 시작 버튼
			m_Select_Name_Edit->SetVisible(true);						// 캐릭터 이름 에디트 박스
			m_Select_Level_Edit->SetVisible(true);						// 캐릭터 레벨 에디트 박스
			break;

		//......................................................................................................
		// 캐릭터 지워저 있을때
		//......................................................................................................
		case CSW_SELECT_DELETE:
			
			m_SelectClassWnd->SetVisible(true);					
			m_SelectNameBoxImage->SetVisible(true);						// 캐릭터 선택 이름 배경 그림
			selectchartext->SetVisible(true);
			m_Select_Name_Edit->SetVisible(true);						// 캐릭터 이름 에디트 박스
			m_Select_Level_Edit->SetVisible(true);						// 캐릭터 레벨 에디트 박스
			break;

		//......................................................................................................
		// 캐릭터 생성창이래 
		//......................................................................................................
		case CSW_CREATE:
			m_Create_Rot_Left_Btn->SetVisible(true);					// 캐릭터 생성 왼쪽    버튼
			m_Create_Rot_Right_Btn->SetVisible(true);					// 캐릭터 생성 오른 쪽 버튼
						
			m_Create_Millena_Btn->SetVisible(true);						// 캐릭터 생성 밀레나 버튼
			m_Create_Rain_Btn->SetVisible(true);						// 캐릭터 생성 레인 버튼
			m_nationtext_rain->SetVisible(true);						
			m_nationtext_millena->SetVisible(true);						
			
			m_Create_Male_Btn->SetVisible(true);						// 캐릭터 생성 남자 버튼
			m_Create_Female_Btn->SetVisible(true);						// 캐릭터 생성 여자 버튼
			
			m_Create_Hair_Left_Btn->SetVisible(true);					// 캐릭터 생성 머리 왼쪽   버튼
			m_Create_Hair_Right_Btn->SetVisible(true);					// 캐릭터 생성 머리 오른쪽 버튼
			
			m_Create_Face_Left_Btn->SetVisible(true);					// 캐릭터 생성 얼굴 왼쪽   버튼
			m_Create_Face_Right_Btn->SetVisible(true);					// 캐릭터 생성 얼굴 오른쪽 버튼
			
			m_Create_Create_Btn->SetVisible(true);						// 캐릭터 생성 생성 버튼
			m_Create_Cancel_Btn->SetVisible(true);						// 캐릭터 생성 취소 버튼
			
			m_Create_HelpImage->SetVisible(true);						// 캐릭터 생성 이름 배경 그림
			m_Create_Name_Edit->SetVisible(true);						// 캐릭터 생성 이름 에디트 박스
			m_CreateDlgmage->SetVisible(true);							// 캐릭터 생성 뒷 배경
			
			tbox[0]->SetVisible( true );
			tbox[1]->SetVisible( true );
			tbox[2]->SetVisible( true );
			tbox[3]->SetVisible( true );
			tbox[4]->SetVisible( true );
			tbox[5]->SetVisible( true );
			tbox[6]->SetVisible( true );

			//edit enable
			nRui->SendEvent( WID_CharCreate_Name_EditBox, n_emSetFocus, 0, 0, 0, 0 );
			
			break;
	}
}

void CCharSelectWnd::ChangeType(void)
{
}


void CCharSelectWnd::DeleteCharacter(void)
{
	g_Pc.DisappearPC( PosInfo[ CSW_SELECT_POS0 ].Unique );
	g_Pc.DisappearPC( PosInfo[ CSW_SELECT_POS1 ].Unique );
	g_Pc.DisappearPC( PosInfo[ CSW_SELECT_POS2 ].Unique );
	g_Pc.DisappearPC( PosInfo[ CSW_SELECT_POS3 ].Unique );
	g_Pc.DisappearPC( PosInfo[ CSW_SELECT_POS4 ].Unique );

	if( g_RockClient.GetLanguage() == eLang_Korea )
	{
		g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_SELECT_POS0 ].Unique , 
								 -1 );
		g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_SELECT_POS1 ].Unique , 
								 -1 );
		g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_SELECT_POS2 ].Unique , 
								 -1 );
		g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_SELECT_POS3 ].Unique , 
								 -1 );
		g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_SELECT_POS4 ].Unique , 
								 -1 );
	}
}

// 생성된 캐릭터 셋팅
void CCharSelectWnd::SelectCharacter(void)
{
	//edit disable
	nRui->SendEvent( WID_CharCreate_Name_EditBox, n_emEditDisable, 0, 0, 0, 0 );

	DeleteCharacter();

	g_Particle.DelEffSetCon( EF_CORE0 , 100 , -1 , -1 );
	
	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();
#ifdef DECO_RENEWAL_MJH 
#else
	m_SelectNum = -1;
#endif //DECO_RENEWAL_MJH
	

	m_pSelectChar = NULL;
#ifdef DECO_RENEWAL_MJH
	for( int i = m_SelectNum ; i < m_SelectNum+1 ; ++i )
#else
	for( int i = 0 ; i < g_RockClient.GetCharacterCount() ; ++i )
#endif //DECO_RENEWAL_MJH
	{
		//..........................................................................................................
		// 코어 체크 
		//..........................................................................................................
		m_Core = ( pChar[ i ].theCore%10 );
		
		switch( m_Core )
		{
			//......................................................................................................
			// 캐릭터가 생성되지 않은 Core
			//......................................................................................................
		case n_CoreItSelf:
			{
				m_Core  = pChar[ i ].theCore / 10;
				
				m_State = CSW_SELECT_CORE;
				
				m_pSelectChar = NULL;
				
				// 캐릭터선택 
				D3DXVECTOR3 vPos = GetPos( PosInfo[ i ].CelPos[ 0 ] , PosInfo[ i ].CelPos[ 1 ]  );
				
				vPos.y += 10.0f;
				
				g_Particle.SetEffSetCon( 0 , EF_CORE0 , 100 , EFFECT_SET_LOOP_TIME , -1 , -1 , vPos );

			}
			
			
			pCharList[ i ] = NULL;

			break;
			
			//......................................................................................................
			// 캐릭터가 생성된 Core
			//......................................................................................................
		case n_CharacterCore:
			{			
				m_pSelectChar = &pChar[ i ];
				
				m_State = CSW_SELECT_EXIST;
				
				SelectCreateCharacter( i );
			}
			break;
			
			//......................................................................................................
			// 삭제예정 캐릭터가 있는 Core
			//......................................................................................................
		case n_DelCharCore:
			{
				m_State = CSW_SELECT_DELETE;
				m_pSelectChar = &pChar[ i ];
				SelectCreateCharacter( i );
			}
			break;
		}
	}
}
#ifdef DECO_RENEWAL_MJH

void CCharSelectWnd::SelectCharacter(int SelNum)
{
	//edit disable
	nRui->SendEvent( WID_CharCreate_Name_EditBox, n_emEditDisable, 0, 0, 0, 0 );


	//m_SelectNum = -1;

	m_pSelectChar = NULL;

////////////////
	
	if( SelNum == m_SelectNum )
	{
		return;
	}
	DeleteCharacter();

	g_Particle.DelEffSetCon( EF_CORE0 , 100 , -1 , -1 );
	m_SelectNum = SelNum;

	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();

	m_pSelectChar = &pChar[ m_SelectNum ];
	
	m_Select_Start_Btn->SetVisible( TRUE );
	m_Select_Start_Btn->SetBtnHold( TRUE );

	m_Select_Delete_Btn->SetVisible( TRUE );
	m_Select_Delete_Btn->SetBtnHold( TRUE );

	if( m_SelectNum != -1 )
	{
		if( ( pChar[ m_SelectNum ].theCore%10 ) == n_CharacterCore )
		{
			m_Select_Start_Btn->SetBtnHold( FALSE );
			m_Select_Delete_Btn->SetBtnHold( FALSE );

			char TempStr[ 1024 ] = "";

			Rsprintf( RWCHAR(TempStr) , _RT("%s : %s") ,G_STRING_CHAR( IDS_CHARSTATE_NAME2 ), RWCHAR(pChar[ m_SelectNum ].theData.thePCName) );
			m_Select_Name_Edit->SetText( TempStr );
			
			Rsprintf( RWCHAR(TempStr) , _RT("%s : %d") ,G_STRING_CHAR( IDS_CHARSTATE_LEVEL2 ), RWCHAR(pChar[ m_SelectNum ].theData.theLevel) );
			m_Select_Level_Edit->SetText( TempStr );
#ifdef DECO_RENEWAL_MJH
			Rsprintf( RWCHAR(TempStr) , _RT("%s : %s") ,RWCHAR("현위치"), G_STRING_CHAR( g_MapDataManager.GetStrID( pChar[ m_SelectNum ].theData.theMap ) ) );
			m_MapPositiontext->SetText( TempStr );

			Rsprintf( RWCHAR(TempStr) , _RT("%s : %s") ,RWCHAR("소속길드"), RWCHAR(pChar[ m_SelectNum ].theData.theFrontierName) );
			m_JoinGuildtext->SetText( TempStr );

			Rsprintf( RWCHAR(TempStr) , _RT("%s : %d/%d") ,RWCHAR("개인/누적"), RWCHAR(pChar[ m_SelectNum ].theData.theFame) , 
				RWCHAR(pChar[ m_SelectNum ].theData.theTotalFame));
			m_PcFameText->SetText( TempStr );
#endif //DECO_RENEWAL_MJH

			SetMainClassNameString( pChar[ m_SelectNum ].theData.theMainClass );

			m_SelectClassWnd->SetVisible( TRUE );
			m_Select_Name_Edit->SetVisible( TRUE );
			m_Select_Level_Edit->SetVisible( TRUE );
			m_Chracterinfotext->SetVisible( TRUE );
			m_Guildinfotext->SetVisible( TRUE );
			m_JoinGuildtext->SetVisible( TRUE );
			m_JoinCorpstext->SetVisible( TRUE );
			m_MapPositiontext->SetVisible( TRUE );
			m_FameText->SetVisible( TRUE );
			m_PcFameText->SetVisible( TRUE );
			m_GuildFameText->SetVisible( TRUE );

		}
		else if( ( pChar[ m_SelectNum ].theCore%10 ) == n_CoreItSelf )
		{
			m_SelectClassWnd->SetText( G_STRING_CHAR( IDS_WND_CHAR_CORE ) );

			m_SelectClassWnd->SetVisible( TRUE );
			m_Select_Name_Edit->SetVisible( FALSE );
			m_Select_Level_Edit->SetVisible( FALSE );
			m_Chracterinfotext->SetVisible( FALSE );
			m_Guildinfotext->SetVisible( FALSE );
			m_JoinGuildtext->SetVisible( FALSE );
			m_JoinCorpstext->SetVisible( FALSE );
			m_MapPositiontext->SetVisible( FALSE );
			m_FameText->SetVisible( FALSE );
			m_PcFameText->SetVisible( FALSE );
			m_GuildFameText->SetVisible( FALSE );
		}
			
	}

///////////////
	
	//SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();

	//m_SelectNum = -1;

	//m_pSelectChar = NULL;

	for( int i = SelNum ; i < SelNum+1 ; ++i )
	{
		//..........................................................................................................
		// 코어 체크 
		//..........................................................................................................
		m_Core = ( pChar[ i ].theCore%10 );
		
		switch( m_Core )
		{
			//......................................................................................................
			// 캐릭터가 생성되지 않은 Core
			//......................................................................................................
		case n_CoreItSelf:
			{
				m_Core  = pChar[ i ].theCore / 10;
				
				m_State = CSW_SELECT_CORE;
				
				m_pSelectChar = NULL;
				
				// 캐릭터선택 
				D3DXVECTOR3 vPos = GetPos( PosInfo[ i ].CelPos[ 0 ] , PosInfo[ i ].CelPos[ 1 ]  );
				
				vPos.y += 10.0f;
				
				g_Particle.SetEffSetCon( 0 , EF_CORE0 , 100 , EFFECT_SET_LOOP_TIME , -1 , -1 , vPos );

			}
			
			
			pCharList[ i ] = NULL;

			break;
			
			//......................................................................................................
			// 캐릭터가 생성된 Core
			//......................................................................................................
		case n_CharacterCore:
			{			
				m_pSelectChar = &pChar[ i ];
				
				m_State = CSW_SELECT_EXIST;
				
				SelectCreateCharacter( i );
			}
			break;
			
			//......................................................................................................
			// 삭제예정 캐릭터가 있는 Core
			//......................................................................................................
		case n_DelCharCore:
			{
				m_State = CSW_SELECT_DELETE;
				m_pSelectChar = &pChar[ i ];
				SelectCreateCharacter( i );
			}
			break;
		}
	}
}
#endif //DECO_RENEWAL_MJH
void CCharSelectWnd::CreateCharacter(BYTE Citizen,BYTE Gender)
{
	SetCreatePos();

	m_fRotY = 0.0f;
	
	PCTABLE	pc;

	pc.Init();

	
	if( Citizen == MILLENA )
	{
		g_Pc.DisappearPC( PosInfo[ CSW_MILLENA_POS ].Unique );

		if( g_RockClient.GetLanguage() == eLang_Korea )
		{
		   g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_MILLENA_POS ].Unique , 
												 -1 );		
		}

		pc.lUnique = PosInfo[ CSW_MILLENA_POS ].Unique;
#ifdef DECO_RENEWAL_MJH
	pc.lBlock     = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
	pc.lDestBlock = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
#else
	pc.lBlock     = ( PosInfo[ CSW_MILLENA_POS ].CelPos[ 0 ] + PosInfo[ CSW_MILLENA_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
    pc.lDestBlock = ( PosInfo[ CSW_MILLENA_POS ].CelPos[ 0 ] + PosInfo[ CSW_MILLENA_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
#endif // DECO_RENEWAL_MJH
		memcpy( &m_pSelectHairCode , &m_MillenaHairCodeTable , sizeof( m_MillenaHairCodeTable ) );
		memcpy( &m_pSelectFaceCode , &m_MillenaFaceCodeTable , sizeof( m_MillenaFaceCodeTable ) );
	
	}
	else
	{
		g_Pc.DisappearPC( PosInfo[ CSW_RAIN_POS ].Unique );

		if( g_RockClient.GetLanguage() == eLang_Korea )
		{
		   g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_RAIN_POS ].Unique , 
												 -1 );		
		}

		pc.lUnique    = PosInfo[ CSW_RAIN_POS ].Unique;

		pc.lBlock     = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
		pc.lDestBlock = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
	
		memcpy( &m_pSelectHairCode , &m_RainHairCodeTable , sizeof( m_RainHairCodeTable ) );
		memcpy( &m_pSelectFaceCode , &m_RainFaceCodeTable , sizeof( m_RainFaceCodeTable ) );
	}

	pc.nIndex  = 0;

	pc.nIndex = g_Pc.GetAttrIndex( Citizen , Gender );

	pc.char_info.theHair = 	m_pSelectHairCode[ m_Gender ][ m_SelectHairNum ];
	pc.char_info.theFace = 	m_pSelectFaceCode[ m_Gender ][ m_SelectFaceNum ];

	pc.lCurMap    = 0;

	g_Pc.AppearPC( pc.lUnique , pc );
	
	if( g_RockClient.GetLanguage() == eLang_Korea )
	{
		g_Particle.SetEffSetCon( 0 , KOREA_EFFECT , 0 , 
								  EFFECT_SET_LOOP_TIME , pc.lUnique , -1  );
		g_Particle.SetEffSetCon( 0 , KOREA_EFFECT , 0 , 
								  EFFECT_SET_LOOP_TIME , pc.lUnique , -1  );
	}

	int table_index = g_Pc.FindUser( pc.lUnique );

	g_Pc.m_Pc[table_index].m_pUserInfo->ArmorInfo.Clear();

	g_Pc.m_Pc[table_index].SetDirection( g_Camera.m_vEye.x , g_Camera.m_vEye.z );
	
	int TableIndex = 0;

	if( Citizen == RAIN )
	{
		if( m_Gender == MALE )
		{
			TableIndex = 2;
		}
		else
		{
			TableIndex = 3;
		}
#ifdef DECO_RENEWAL_MJH
		
#else
		g_Pc.m_Pc[table_index].m_Mov.now_Yrad = -D3DXToRadian( 90 );
#endif	//DECO_RENEWAL_MJH
	}
	else
	{
		if( m_Gender == MALE )
		{
			TableIndex = 0;
		}
		else
		{
			TableIndex = 1;
		}
#ifdef DECO_RENEWAL_MJH
		
#else
		g_Pc.m_Pc[table_index].m_Mov.now_Yrad = D3DXToRadian( 90 );
		
#endif	//DECO_RENEWAL_MJH
		
	}
	
	m_Citizen = Citizen;
	m_Gender  = Gender;

	g_Pc.m_Pc[table_index].SetWearItem();
	g_Pc.m_Pc[table_index].AnimationPlayNoneBlend( n_Stand_Select9 );
	g_Pc.m_Pc[table_index].PreTextureLoad();
}
BOOL CCharSelectWnd::ItemNormalCheck(void *_pc , TCHAR *PcName)
{
	PCTABLE *pc = (PCTABLE *)_pc;
	
    
	int ItemCode = NULL ;
	int ItemClass = NULL ;
	TCHAR str[256] = "" ; 
		
	SItemBaseInfo* item_Info = NULL;
	
	
	for( int i =0 ; i < RP_MAX_EQUIP ; ++i )
	{
		ItemCode  = pc->char_info.theWearItem[i].theCode;
		if( ItemCode == NULL)
		{
			continue;
		}

		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( ItemCode );	
		
		if(item_Info == NULL)
			return FALSE;
		
		bool bCheckMessageBox = false ;
		

		switch(i)
		{
		case n_WearSlot_CoatArmor:   
				{
					if( (item_Info->theClass != n_Class_CoatArmor) && (item_Info->theClass != n_Class_Skin))
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_PantsArmor:	
				{
					if( item_Info->theClass != n_Class_PantsArmor )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
			
		case n_WearSlot_Weapon:	
				{
					if( item_Info->theClass != n_Class_Weapon )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_Shield:	
				{
					if( item_Info->theClass != n_Class_Shield && item_Info->theClass != n_Class_Weapon )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_Helmet:	
				{
					if( (item_Info->theClass != n_Class_Helmet) &&	(item_Info->theClass != n_Class_Skin))
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_Serve:	
				{
					if( (item_Info->theClass != n_Class_Serve)	&&(item_Info->theClass != n_Class_Skin) )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_Gloves:	
				{
					if( item_Info->theClass != n_Class_Gloves )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_Shoes:	
				{
					if( item_Info->theClass != n_Class_Shoes )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_Accessory1:	
				{
					if( item_Info->theClass != n_Class_Accessory )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_Accessory2:	
				{
					if( item_Info->theClass != n_Class_Accessory )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_Riding:	
				{
					if( item_Info->theClass != n_Class_Riding )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_RidingItem1:	
				{
					if( item_Info->theClass != n_Class_RidingItem )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_RidingItem2:	
				{
					if( item_Info->theClass != n_Class_RidingItem )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
		case n_WearSlot_RidingItem3:	
				{
					if( item_Info->theClass != n_Class_RidingItem )
					{
						pc->char_info.theWearItem[i].theCode = 0 ;
						bCheckMessageBox = true ;
					}
					break;
				}
			
		}
	}
	

return FALSE;
}

void CCharSelectWnd::SelectCreateCharacter(int i)
{
	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();

	PCTABLE	pc;

	pc.Init();

	pc.lUnique = PosInfo[ i ].Unique;

	pc.nIndex = g_Pc.GetAttrIndex( pChar[ i ].theData.theCitizen, pChar[ i ].theData.theGender );

	pc.char_info.theHair = pChar[ i ].theData.theHair;
	pc.char_info.theFace = pChar[ i ].theData.theFace;

	pc.lBlock     = ( PosInfo[ i ].CelPos[ 0 ] + PosInfo[ i ].CelPos[ 1 ] * g_Pc.m_cSize );
	pc.lDestBlock = ( PosInfo[ i ].CelPos[ 0 ] + PosInfo[ i ].CelPos[ 1 ] * g_Pc.m_cSize );
	pc.lCurMap    = 0;

	memcpy( &pc.char_info.theWearItem , pChar[ i ].theData.theWearItem , sizeof( pChar[ i ].theData.theWearItem ) );
	
	//현재 들어온 아이템을 조사해서... 비정상적인 아이템은 클라이언트를 경고후 종료 시킨다. 
	
	if(ItemNormalCheck( &pc , pChar[ i ].theData.thePCName ) == TRUE)
		return ;


	g_Pc.AppearPC( PosInfo[ i ].Unique , pc );
	
	
#ifdef LOGINPAGE_BGIMG_CHANGE
		g_Particle.SetEffSetCon( 0 , KOREA_EFFECT , 0 , 
								  EFFECT_SET_LOOP_TIME , PosInfo[ i ].Unique , -1 );
		g_Particle.SetEffSetCon( 0 , KOREA_EFFECT , 0 , 
								  EFFECT_SET_LOOP_TIME , PosInfo[ i ].Unique , -1 );
#endif

	int table_index = g_Pc.FindUser( PosInfo[ i ].Unique );

	// 얼굴 / 머리 부분 초기화 
	g_Pc.m_Pc[table_index].AnimationPlayNoneBlend( n_Stand_Select9 );
	g_Pc.m_Pc[table_index].SetDirection( 0.0f );

	pCharList[ i ] = &g_Pc.m_Pc[table_index];
	
	Rsprintf( RWCHAR(g_Pc.m_PcTable[ g_Pc.m_Pc[table_index].m_nTableIndex ].char_info.thePCName) ,_RT("%s") , 
		RWCHAR(pChar[ i ].theData.thePCName) );

	g_Pc.m_Pc[table_index].PreTextureLoad();
}

D3DXVECTOR3 CCharSelectWnd::GetPos(int x,int z, bool bFlag)
{
	float tposx = PosInfo[ 0 ].CelPos[ 0 ] * 8 + 4;
	float tposz = PosInfo[ 0 ].CelPos[ 1 ] * 8 + 4;
	float tposy = g_Map.PHF_GetHeight( tposx , tposz );
	
	float posx = x * 8 + 4;
	float posz = z * 8 + 4;	
	  
	float posy = g_Map.PHF_GetHeight( posx , posz );

	if(bFlag)
	{
		return	D3DXVECTOR3( posx , tposy , posz );
	}
	
	return D3DXVECTOR3( posx , posy , posz );
}

void CCharSelectWnd::CreateOK(void)
{
	int Num = GetCoreNum();
#ifdef DECO_RENEWAL_MJH
	m_SelectNum = Num;
#endif // DECO_RENEWAL_MJH
	
#ifdef DECO_RENEWAL_MJH
	g_Pc.DisappearPC( CLASS_UNIQUE ); //클래스케릭터 삭제
#endif //DECO_RENEWAL_MJH	
	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();

	pChar[ Num ].theCore += 1;
	
	char Name[ 100 ] = "";

	m_Create_Name_Edit->GetText( Name , 100 );

	Rsprintf( RWCHAR(pChar[ Num ].theData.thePCName) , _RT("%s") , RWCHAR(Name) );

	pChar[ Num ].theData.theCitizen = m_Citizen;
	pChar[ Num ].theData.theGender  = m_Gender;
	
	pChar[ Num ].theData.theFace    = m_pSelectFaceCode[ m_Gender ][ m_SelectFaceNum ];
	pChar[ Num ].theData.theHair    = m_pSelectHairCode[ m_Gender ][ m_SelectHairNum ];

	pChar[ Num ].theData.theLevel   = 0;
#ifdef DECO_RENEWAL_MJH
	pChar[ Num ].theData.theMainClass = m_ViewClass;
#else
	if( m_Citizen == RAIN )
	{
		pChar[ Num ].theData.theMainClass = Stranger;
	}
	else
	{
		pChar[ Num ].theData.theMainClass = Wanderer;
	}

#endif // DECO_RENEWAL_MJH
	
	int table_index = g_Pc.FindUser( PosInfo[ Num ].Unique );

	memset( pChar[ Num ].theData.theWearItem , 0 , sizeof( m_pSelectChar->theData.theWearItem ) );

	MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_CREATE_CH ) , 1000 , NULL );
#ifdef DECO_RENEWAL_MJH
	if( m_SEQState == SEQ_MILENA_CLASS )
#else
	if( m_SEQState == SEQ_MILENA_CREATE )
#endif	// DECO_RENEWAL_MJH
	{
		StartReturnMillenaToSelect();
	}
#ifdef DECO_RENEWAL_MJH
	else if( m_SEQState == SEQ_RAIN_CLASS )
#else
	else if( m_SEQState == SEQ_RAIN_CREATE )
#endif	// DECO_RENEWAL_MJH
	{
		StartReturnRainToSelect();
	}
}

void CCharSelectWnd::CreateCharacterOK(void)
{

	char Name[ 100 ] = {0,};

	m_Create_Name_Edit->GetText( Name , 100 );

	char ChName[256] ={0,};
#ifdef GANJA_WORD_BLOCK_20090630

	DXUtil_ConvertWideStringToAnsi( ChName , RWCHAR(Name) , -1 );
	int cchDestChar = wcslen(RWCHAR(Name))+1;
    WideCharToMultiByte( 950, 0,  RWCHAR(Name), -1, ChName,  cchDestChar-1, NULL, NULL );// 950 : Big5 번체

	int len = strlen(ChName);
	char Ch[3]={0,};
	for(int i = 0 ; i < len ;)
	{
		DetectionDBCS( &i, ChName, Ch);
		i++;

		if(Ch[0] == '?')
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
			return;
		}
	}
#endif 




	if( Rstrlen( RWCHAR(Name) ) <= 0 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_INPUT_NAME ) , 1500 , NULL );

		return;
	}
#ifdef TEXFILTERING_TEST 
	if( !g_RockClient.m_SlangFilter.IsValidString( Name ) )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
		
		return;
	}
	
	if( g_RockClient.m_SlangFilter.ISFindFilterName( Name ) )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
		return;
	}
	
	if( g_RockClient.m_SlangFilter.Filter( Name ) )
	{
		if( g_RockClient.m_SlangFilter.ISSlangFound() )
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SLANG_TEXT ) , 1500 , NULL );
			
			return;
		}
	}
#else
	#ifndef _DEBUG 
	if( !g_RockClient.m_SlangFilter.IsValidString( Name ) )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
		
		return;
	}
	
	if( g_RockClient.m_SlangFilter.ISFindFilterName( Name ) )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
		return;
	}
	
	if( g_RockClient.m_SlangFilter.Filter( Name ) )
	{
		if( g_RockClient.m_SlangFilter.ISSlangFound() )
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SLANG_TEXT ) , 1500 , NULL );
			
			return;
		}
	}
	#endif 
#endif //TEXFILTERING_TEST

	int Num = GetCoreNum();

	if( Num == -1 )
	{
		// 혹시 , 설마...
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NOTEXIST_CORE ) , 1500 , NULL );
	
		return;
	}
	else
	{
#ifdef DECO_RENEWAL_MJH
		if ( m_ViewClass == Psyche || m_ViewClass == Prominas || m_ViewClass == Priest)
		{
			m_Citizen = RAIN;
		}
		g_RockClient.Send_RpCsCreateCharacter( Num , m_ViewClass , Name , m_Citizen , m_Gender , m_pSelectHairCode[ m_Gender ][ m_SelectHairNum ] , m_pSelectFaceCode[ m_Gender ][ m_SelectFaceNum ] );
#else 
		g_RockClient.Send_RpCsCreateCharacter( Num , Name , m_Citizen , m_Gender , m_pSelectHairCode[ m_Gender ][ m_SelectHairNum ] , m_pSelectFaceCode[ m_Gender ][ m_SelectFaceNum ] );
#endif // DECO_RENEWAL_MJH
		
	}

	MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_DO_CREATE_CH ) );
}

void CCharSelectWnd::DeleteOK(void)
{
	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();

	pChar[ m_SelectNum ].theCore = 10;
	
	MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_DELETED_CH ), 1000 , NULL );

	SelectCharacter();

	m_Select_Delete_Btn->SetVisible( TRUE );
	m_Select_Delete_Btn->SetBtnHold( TRUE );

	m_Select_Start_Btn->SetVisible( TRUE );
	m_Select_Start_Btn->SetBtnHold( TRUE );
#ifdef DECO_RENEWAL_MJH
	m_pSelectChar = &pChar[ m_SelectNum ];

	if( m_SelectNum != -1 )
	{
		if( ( pChar[ m_SelectNum ].theCore%10 ) == n_CharacterCore )
		{
			m_Select_Start_Btn->SetBtnHold( FALSE );
			m_Select_Delete_Btn->SetBtnHold( FALSE );
		}
			
	}
#endif //DECO_RENEWAL_MJH


	if( GetCoreNum() != -1 )
	{
		m_Select_Create_Btn->SetVisible( TRUE );
#ifdef CREATE_CHRACTER_BTN

		if( nRui->m_WorldIndex == 2 ) 
		{
			m_Select_Create_Btn->SetBtnHold( TRUE );
		}
		else 
		{
			m_Select_Create_Btn->SetBtnHold( FALSE );
		}
	
		
#else
		
		m_Select_Create_Btn->SetBtnHold( FALSE );
#endif	
		
	}
	else
	{
		m_Select_Create_Btn->SetVisible( TRUE );
		m_Select_Create_Btn->SetBtnHold( TRUE );
	}
#ifdef DECO_RENEWAL_MJH
#else	
	m_SelectClassWnd->SetVisible( FALSE );
	m_Select_Name_Edit->SetVisible( FALSE );
	m_Select_Level_Edit->SetVisible( FALSE );
#endif // DECO_RENEWAL_MJH

#ifdef DECO_RENEWAL_MJH
	CharacterInfoList();
#endif // DECO_RENEWAL_MJH
}

//
void CCharSelectWnd::SetCreatePos(void)
{
	m_ISChangeSkyBox = TRUE;
}

void CCharSelectWnd::SetMainClassNameString( BYTE _mainclass )
{
	TCHAR Tmp[256]= {0,};
	
	switch( _mainclass )
	{
	//기본, 초급, 중금 직업군(Lv 1~100)
	//(기본직업)
	case Wanderer:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_WANDERER ) );	break;
	case Stranger:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_STRANGER ) );	break;
	//(초급직업군)
	case Swordman:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_SWORDMAN ) );	break;
	case Archer:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_ARCHER ) );	break;
	case Magician:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_MAGICIAN ) );	break;
	case Shaman:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_SHAMAN ) );	break;
	//(중급직업군)
	case Knight:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_KNIGHT ) );	break;
	case Mercenary:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_MERCENARY ) );	break;
	case Slayer:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_SLAYER ) );	break;
	case Sniper:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_SNIPER ) );	break;
	case Prominas:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_PROMINAS ) );	break;
	case Priest:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_PRIEST ) );	break;
	case Holy_Avenger:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_HOLYAVENGER ) );	break;
	case Psyche:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_PSYCHE ) );	break;
	}

	m_SelectClassWnd->SetText( Tmp  );	
	
}
#ifdef DECO_RENEWAL_MJH

TCHAR* CCharSelectWnd::SetRenewalClassNameString( BYTE _mainclass )
{	
	switch( _mainclass )
	{
	//기본, 초급, 중금 직업군(Lv 1~100)
	//(기본직업)
	case Wanderer:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_WANDERER ) );	break;
	case Stranger:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_STRANGER ) );	break;
	//(초급직업군)
	case Swordman:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_SWORDMAN ) );	break;
	case Archer:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_ARCHER ) );	break;
	case Magician:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_MAGICIAN ) );	break;
	case Shaman:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_SHAMAN ) );	break;
	//(중급직업군)
	case Knight:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_KNIGHT ) );	break;
	case Mercenary:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_MERCENARY ) );	break;
	case Slayer:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_SLAYER ) );	break;
	case Sniper:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_SNIPER ) );	break;
	case Prominas:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_PROMINAS ) );	break;
	case Priest:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_PRIEST ) );	break;
	case Holy_Avenger:	Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_HOLYAVENGER ) );	break;
	case Psyche:		Rsprintf( RWCHAR(Tmp), _RT("%s : %s"), G_STRING_CHAR( IDS_CHARSTATE_CLASS2 ), G_STRING_CHAR( IDS_CLASS_PSYCHE ) );	break;
	}
	
	return Tmp;	
	
}

int CCharSelectWnd::SetRenewalClassImg( BYTE _mainclass )
{
	switch( _mainclass )
	{
		case Wanderer:  return TID_prejob_holy;      break;
		case Stranger:  return TID_prejob_holy;      break;
		case Swordman:  return TID_Ohigen;           break;
		case Archer:    return TID_Arbess;           break;
		case Magician:  return TID_Ohigen;			 break;
		case Shaman:    return TID_Ohigen;           break;
		case Knight:    return TID_prejob_knight ;   break;
		case Mercenary: return TID_prejob_mercynary; break;
		case Slayer:    return TID_prejob_slayer;    break;
		case Sniper:    return TID_prejob_soul;      break;
		case Prominas:  return TID_prejob_prom;      break;
		case Priest:    return TID_prejob_prist;     break;
		case Holy_Avenger: return TID_prejob_holy;   break;
		case Psyche:    return TID_prejob_shamer;    break;
		default:        return TID_prejob_magician;	 break;											
	}
}
#endif // DECO_RENEWAL_MJH

int CCharSelectWnd::GetItemIndex(int nFileName)
{
	for( int i = 0; i < c_Chr_Itm_Count; i++ )
	{					
		if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)nFileName )
		{
			return i;
		}
	}

	return 0;
}


//................................................................................................................
// 카메라 첫 위치
//................................................................................................................
int CCharSelectWnd::StartCameraPosition(void)
{
	g_Camera.Create( g_lpDevice );
	
	if( m_ISChangeSkyBox )
	{
		if( g_Map.m_SetEnv.Set( 0 , ENV_DAYLIGHT ) )
		{
			LPENV_DATA SetEnvData = g_Map.m_SetEnv.GetSet();
			
			g_Map.m_SkyBox.EnableSkyBox( TRUE );
			
#ifdef LOGINPAGE_BGIMG_CHANGE
			{
				g_Map.m_SkyBox.LoadTexture( g_lpDevice , 18 , 0 );
			}
#else
			{
				g_Map.m_SkyBox.LoadTexture( g_lpDevice , SetEnvData->SkyIndex , 0 );
			}
#endif		
			g_Map.m_Cloud.EnableCloud( TRUE );
			g_Map.m_Cloud.LoadTexture( g_lpDevice , SetEnvData->CloudIndex , ENV_DAYLIGHT );
		}
		
		m_ISChangeSkyBox = FALSE;
	}

	g_Camera.SetStartFrame(  0 , g_lpDevice );

	return TRUE;
}

//................................................................................................................
// 카메라 선택 위치로 에니메이션
//................................................................................................................
int CCharSelectWnd::StartSelectCameraWalkPostion(void)
{
	g_Map.InitMapData();
	g_Camera.SetAnimation( 0 );
	
	return TRUE;
}

//................................................................................................................
// 카메라 밀레나 생성창 으로
//................................................................................................................
int CCharSelectWnd::StartMillenaCameraWalkPostion(void)
{
	
#ifdef DECO_RENEWAL_MJH
	g_Camera.SetAnimation( 0 );
	m_KnightBtn->InitRadioButton( n_cmChecked, _RT("") );
#else 
	g_Camera.SetAnimation( 15 );
#endif  //DECO_RENEWAL_MJH
	
	
	g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::CreateMillenaWalkEnd);

	g_Map.InitMapData();
	
	m_SEQState = SEQ_CAMERA_ANI;
					
	m_SelectHairNum = 0;
	m_SelectFaceNum = 0;
				
	m_Citizen = MILLENA;
	m_Gender  = MALE;

	nRui->SendEvent( WID_CharCreate_Rain_Btn , n_emSetUnChecked , 0 , 0 , 0 , 0 );
	nRui->SendEvent( WID_CharCreate_Millena_Btn , n_emSetChecked , 0 , 0 , 0 , 0 );
				
	CreateCharacter( m_Citizen , m_Gender );
				
	ChangeFaceAndHair();

	SetCloseAllBtn();

	return TRUE;
}

// 밀레나 생성창 카메라 워킹 끝났을때 처리
void CCharSelectWnd::CreateMillenaWalkEnd()
{
	m_SEQState = SEQ_MILENA_CREATE;

	SetCreateCharacterBtn();

	g_Particle.DelEffSetCon( EF_CORE0 , 100 , -1 , -1 );

	DeleteCharacter();
}

int	 CCharSelectWnd::StartRainCameraWalkPosition(void)
{
	g_Camera.SetAnimation( 16 );
	g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::CreateRainWalkEnd);

	g_Map.InitMapData();
	
	m_SEQState = SEQ_CAMERA_ANI;
					
	m_SelectHairNum = 0;
	m_SelectFaceNum = 0;
				
	m_Citizen = RAIN;
	m_Gender  = MALE;

	nRui->SendEvent( WID_CharCreate_Rain_Btn , n_emSetUnChecked , 0 , 0 , 0 , 0 );
	nRui->SendEvent( WID_CharCreate_Millena_Btn , n_emSetChecked , 0 , 0 , 0 , 0 );
				
	CreateCharacter( m_Citizen , m_Gender );
				
	ChangeFaceAndHair();

	SetCloseAllBtn();
	
	return TRUE;
}

void CCharSelectWnd::CreateRainWalkEnd()
{
	m_SEQState = SEQ_RAIN_CREATE;

	SetCreateCharacterBtn();

	g_Particle.DelEffSetCon( EF_CORE0 , 100 , -1 , -1 );

	DeleteCharacter();
}

void CCharSelectWnd::SelectCameraWalkEnd()
{
	m_SEQState = SEQ_SELECT_CHAR;
	SetNoneCharacterSelectBtn();

	if( GetCoreNum() != -1 )
	{
		m_Select_Create_Btn->SetVisible( TRUE );
#ifdef CREATE_CHRACTER_BTN
		if( nRui->m_WorldIndex == 2 ) 
		{
			m_Select_Create_Btn->SetBtnHold( TRUE );
		}
		else 
		{
			m_Select_Create_Btn->SetBtnHold( FALSE );
		}
#else
		m_Select_Create_Btn->SetBtnHold( FALSE );		
#endif
	
	}
}

void CCharSelectWnd::CM_MOVE(int i,int npos)
{
	if( g_RockClient.GetCharacterCount() <= i )
	{
		return;
	}

	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();

	if( ( pChar[ i ].theCore%10 ) != n_CharacterCore )
	{
		return;
	}
		
	int table_index = g_Pc.FindUser( i );

	EVENT_DATA_INFO	data;
	
	data.x    = PosInfo[ npos ].CelPos[ 0 ];
	data.y    = PosInfo[ npos ].CelPos[ 1 ];
	data.type = SM_MOVE_BROADCAST;
				
	g_Pc.m_Pc[table_index].PushQueue( data, TRUE );
				
	data.type = SM_MOVE_TURN;
	g_Pc.m_Pc[table_index].m_save_event = data;

	m_SEQState = SEQ_SELECT_ANI;
}
#ifdef DECO_RENEWAL_MJH

void CCharSelectWnd::CharacterInfoList(void)
{
	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();

	m_pSelectChar = &pChar[ m_SelectNum ];

	if( m_SelectNum != -1 )
	{
		if( ( pChar[ m_SelectNum ].theCore%10 ) == n_CharacterCore )
		{
			m_Select_Start_Btn->SetBtnHold( FALSE );
			m_Select_Delete_Btn->SetBtnHold( FALSE );

			char TempStr[ 1024 ] = "";

			Rsprintf( RWCHAR(TempStr) , _RT("%s : %s") ,G_STRING_CHAR( IDS_CHARSTATE_NAME2 ), RWCHAR(pChar[ m_SelectNum ].theData.thePCName) );
			m_Select_Name_Edit->SetText( TempStr );
			
			Rsprintf( RWCHAR(TempStr) , _RT("%s : %d") ,G_STRING_CHAR( IDS_CHARSTATE_LEVEL2 ), RWCHAR(pChar[ m_SelectNum ].theData.theLevel) );
			m_Select_Level_Edit->SetText( TempStr );
#ifdef DECO_RENEWAL_MJH
			Rsprintf( RWCHAR(TempStr) , _RT("%s : %s") ,RWCHAR("현위치"), G_STRING_CHAR( g_MapDataManager.GetStrID( pChar[ m_SelectNum ].theData.theMap ) ) );
			m_MapPositiontext->SetText( TempStr );
			
			Rsprintf( RWCHAR(TempStr) , _RT("%s : %s") ,RWCHAR("소속길드"), RWCHAR(pChar[ m_SelectNum ].theData.theFrontierName) );
			m_JoinGuildtext->SetText( TempStr );

			Rsprintf( RWCHAR(TempStr) , _RT("%s : %d/%d") ,RWCHAR("개인/누적"), RWCHAR(pChar[ m_SelectNum ].theData.theFame) , 
				RWCHAR(pChar[ m_SelectNum ].theData.theTotalFame));
			m_PcFameText->SetText( TempStr );
#endif //DECO_RENEWAL_MJH
			SetMainClassNameString( pChar[ m_SelectNum ].theData.theMainClass );

			m_SelectClassWnd->SetVisible( TRUE );
			m_Select_Name_Edit->SetVisible( TRUE );
			m_Select_Level_Edit->SetVisible( TRUE );
#ifdef DECO_RENEWAL_MJH
			m_Chracterinfotext->SetVisible( TRUE );
			m_Guildinfotext->SetVisible( TRUE );
			m_JoinCorpstext->SetVisible( TRUE );
			m_JoinGuildtext->SetVisible( TRUE );
			m_MapPositiontext->SetVisible( TRUE );
			m_FameText->SetVisible( TRUE );
			m_PcFameText->SetVisible( TRUE );
			m_GuildFameText->SetVisible( TRUE );
#endif //DECO_RENEWAL_MJH

		}
		else if( ( pChar[ m_SelectNum ].theCore%10 ) == n_CoreItSelf )
		{
			m_SelectClassWnd->SetText( G_STRING_CHAR( IDS_WND_CHAR_CORE ) );

			m_SelectClassWnd->SetVisible( TRUE );
			m_Select_Name_Edit->SetVisible( FALSE );
			m_Select_Level_Edit->SetVisible( FALSE );
#ifdef DECO_RENEWAL_MJH
			m_Chracterinfotext->SetVisible( FALSE );
			m_Guildinfotext->SetVisible( FALSE );
			m_JoinCorpstext->SetVisible( FALSE );
			m_JoinGuildtext->SetVisible( FALSE );
			m_MapPositiontext->SetVisible( FALSE );
			m_FameText->SetVisible( FALSE );
			m_PcFameText->SetVisible( FALSE );
			m_GuildFameText->SetVisible( FALSE );
#endif //DECO_RENEWAL_MJH
		}
			
	}
#ifdef DECO_RENEWAL_MJH

	if( m_SelectNum != -1)
	{
		for( int i = 0 ; i < MAX_PC_SLOT_NUM ; ++i )
		{
			if( ( pChar[ i ].theCore%10 ) == n_CharacterCore )
			{
				char TempStr[ 1024 ] = "";

				Rsprintf( RWCHAR(TempStr) , _RT("%s : %s") ,G_STRING_CHAR( IDS_CHARSTATE_NAME2 ), RWCHAR(pChar[ i ].theData.thePCName) );
				m_PcText[i].PcNameTxt->SetText( TempStr );
				
				Rsprintf( RWCHAR(TempStr) , _RT("%s : %d") ,G_STRING_CHAR( IDS_CHARSTATE_LEVEL2 ), RWCHAR(pChar[ i ].theData.theLevel) );
				m_PcText[i].PcLevelTxt->SetText( TempStr );

				Rsprintf( RWCHAR(TempStr) , _RT("%s") ,SetRenewalClassNameString( pChar[ i ].theData.theMainClass ) );
				m_PcText[i].PcClassTxt->SetText( TempStr ); 

				m_PcText[i].PcImgBox->SetClientImage( SetRenewalClassImg( pChar[ i ].theData.theMainClass ) );
				
				m_PcText[i].PcNameTxt->SetVisible(TRUE);
				m_PcText[i].PcLevelTxt->SetVisible(TRUE);
				m_PcText[i].PcClassTxt->SetVisible(TRUE);
			}
			else if( ( pChar[ i ].theCore%10 ) == n_CoreItSelf )
			{
				m_PcText[i].PcClassTxt->SetText( G_STRING_CHAR( IDS_WND_CHAR_CORE ) );
				m_PcText[i].PcImgBox->SetClientImage( TID_prejob_sword );

				m_PcText[i].PcNameTxt->SetVisible(FALSE);
				m_PcText[i].PcLevelTxt->SetVisible(FALSE);
				m_PcText[i].PcClassTxt->SetVisible(TRUE);
			}
		}
	}
#endif // DECO_RENEWAL_MJH
}
#endif // DECO_RENEWAL_MJH
//................................................................................................................
// 캐릭터 선택창 ( 아무것도 선택 안함 )
//................................................................................................................
void CCharSelectWnd::SetNoneCharacterSelectBtn(void)
{
	SetCloseAllBtn();
#ifdef DECO_RENEWAL_MJH

	for( int j = 0 ; j < MAX_PC_SLOT_NUM ; ++j )
	{
		m_PcText[j].PcImgBox->SetVisible(TRUE);
		m_PcText[j].PcNameTxt->SetVisible(TRUE);
		m_PcText[j].PcLevelTxt->SetVisible(TRUE);
		m_PcText[j].PcClassTxt->SetVisible(TRUE);
	}
	CharacterInfoList();
	m_Select_exit_Btn->SetVisible( TRUE);
	m_SelectCharBoxImage->SetVisible( TRUE);

#endif // DECO_RENEWAL_MJH
	

#ifndef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	m_pSelectBtn[ 0 ]->SetVisible( TRUE );
	m_pSelectBtn[ 1 ]->SetVisible( TRUE );
	m_pSelectBtn[ 2 ]->SetVisible( TRUE );
	m_pSelectBtn[ 3 ]->SetVisible( TRUE );
	m_pSelectBtn[ 4 ]->SetVisible( TRUE );

#endif// #ifndef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	m_Select_Server_Btn0->SetVisible( TRUE );

	m_Select_Create_Btn->SetVisible( TRUE );
	m_Select_Create_Btn->SetBtnHold( TRUE );

	m_Select_Start_Btn->SetVisible( TRUE );
	m_Select_Start_Btn->SetBtnHold( TRUE );

	m_Select_Delete_Btn->SetVisible( TRUE );
	m_Select_Delete_Btn->SetBtnHold( TRUE );

	m_SelectNameBoxImage->SetVisible( TRUE );
	selectchartext->SetVisible(true);

	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();
#ifdef DECO_RENEWAL_MJH
	//////////////////////////

	m_Select_Start_Btn->SetVisible( TRUE );
	m_Select_Start_Btn->SetBtnHold( TRUE );

	m_Select_Delete_Btn->SetVisible( TRUE );
	m_Select_Delete_Btn->SetBtnHold( TRUE );

	m_pSelectChar = &pChar[ m_SelectNum ];

	if( m_SelectNum != -1 )
	{
		if( ( pChar[ m_SelectNum ].theCore%10 ) == n_CharacterCore )
		{
			m_Select_Start_Btn->SetBtnHold( FALSE );
			m_Select_Delete_Btn->SetBtnHold( FALSE );
		}		
	}

/////////////////////////////////////////////////
#else
	m_SelectNum = -1;
#endif // DECO_RENEWAL_MJH

	BOOL ISFindCore = FALSE;

	for( int i = 0 ; i < g_RockClient.GetCharacterCount() ; ++i )
	{
		//..........................................................................................................
		// 코어 체크 
		//..........................................................................................................
		m_Core = ( pChar[ i ].theCore%10 );
		
		switch( m_Core )
		{
			//......................................................................................................
			// 캐릭터가 생성되지 않은 Core
			//......................................................................................................
			case n_CoreItSelf:
			{
				m_pSelectBtn[ i ]->SetHID( HID_EXIT );
			
				ISFindCore = TRUE;
			}
			break;
		}
	}

	//............................................................................................................
	// 코어있을때 생성버튼 활성화 하기
	//............................................................................................................
	if( ISFindCore )
	{
		m_Select_Create_Btn->SetVisible( TRUE );
		
#ifdef CREATE_CHRACTER_BTN
		if( nRui->m_WorldIndex == 2 ) 
		{
			m_Select_Create_Btn->SetBtnHold( TRUE );
		}
		else 
		{
			m_Select_Create_Btn->SetBtnHold( FALSE );
		}
#else
		m_Select_Create_Btn->SetBtnHold( FALSE );		
#endif
	}

}

//................................................................................................................
// 코어 번호 얻기
//................................................................................................................
int CCharSelectWnd::GetCoreNum(void)
{
	SRpdCharacterDataWithCore  *pChar = g_RockClient.GetCharacterList();

	for( int i = 0 ; i < g_RockClient.GetCharacterCount() ; ++i )
	{
		//..........................................................................................................
		// 코어 체크 
		//..........................................................................................................
		m_Core = ( pChar[ i ].theCore%10 );
		
		switch( m_Core )
		{
			//......................................................................................................
			// 캐릭터가 생성되지 않은 Core
			//......................................................................................................
			case n_CoreItSelf:
			{
				return i;
			}

			break;
			
		}
	}

	return -1;
}

//................................................................................................................
// 전체 버튼 닫기
//................................................................................................................
void CCharSelectWnd::SetCloseAllBtn(void)
{
	m_SelectNameBoxImage->SetVisible( FALSE );
	selectchartext->SetVisible(false);
	m_SelectCreateBoxImage->SetVisible( FALSE );
	m_Create_HelpImage->SetVisible( FALSE );
	m_CreateDlgmage->SetVisible( FALSE );
	m_Select_Start_Btn->SetVisible( FALSE );
	m_Select_Server_Btn0->SetVisible( FALSE );
	m_Select_Create_Btn->SetVisible( FALSE );
	m_Create_Rot_Left_Btn->SetVisible( FALSE );
	m_Create_Rot_Right_Btn->SetVisible( FALSE );
	m_Create_Hair_Left_Btn->SetVisible( FALSE );
	m_Create_Hair_Right_Btn->SetVisible( FALSE );
	m_Create_Face_Left_Btn->SetVisible( FALSE );
	m_Create_Face_Right_Btn->SetVisible( FALSE );
	m_Create_Create_Btn->SetVisible( FALSE );
	m_Create_Cancel_Btn->SetVisible( FALSE );
	
	m_Create_Millena_Btn->SetVisible( FALSE );
	m_Create_Rain_Btn->SetVisible( FALSE );
	m_nationtext_rain->SetVisible(false);						
	m_nationtext_millena->SetVisible(false);						

	m_Create_Male_Btn->SetVisible( FALSE );
	m_Create_Female_Btn->SetVisible( FALSE );
	m_Create_Name_Edit->SetVisible( FALSE );
	m_Create_Name_Edit->EditDisable();
	m_Select_Name_Edit->SetVisible( FALSE );
	m_SelectClassWnd->SetVisible( FALSE );
	m_Select_Level_Edit->SetVisible( FALSE );
	m_Create_Rain_Btn->SetVisible( FALSE );
	m_Create_Rain_Btn->SetVisible( FALSE );

#ifndef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	m_pSelectBtn[ 0 ]->SetVisible( FALSE );
	m_pSelectBtn[ 1 ]->SetVisible( FALSE );
	m_pSelectBtn[ 2 ]->SetVisible( FALSE );
	m_pSelectBtn[ 3 ]->SetVisible( FALSE );
	m_pSelectBtn[ 4 ]->SetVisible( FALSE );

#endif// #ifndef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	m_pSelectBtn[ 0 ]->SetHID( HID_None );
	m_pSelectBtn[ 1 ]->SetHID( HID_None );
	m_pSelectBtn[ 2 ]->SetHID( HID_None );
	m_pSelectBtn[ 3 ]->SetHID( HID_None );
	m_pSelectBtn[ 4 ]->SetHID( HID_None );

	m_pZoomBtn->SetVisible( FALSE );
	m_pClassViewBtn->SetVisible( FALSE );

	m_ClassViewReturn->SetVisible( FALSE );

	m_MagicianBtn->SetVisible( FALSE );
	m_ShamanBtn->SetVisible( FALSE );
	m_ProminasBtn->SetVisible( FALSE );
	m_PriestBtn->SetVisible( FALSE );
	m_Holy_AvengerBtn->SetVisible( FALSE );
	m_SummonerBtn->SetVisible( FALSE );
	m_ArcherBtn->SetVisible( FALSE );
	m_SwordManBtn->SetVisible( FALSE );
	m_KnightBtn->SetVisible( FALSE );
	m_MercenaryBtn->SetVisible( FALSE );
	m_SlayerBtn->SetVisible( FALSE );
	m_Soul_BreakerBtn->SetVisible( FALSE );
	
	m_pHelpText->SetVisible( FALSE );
	m_HelpImage->SetVisible( FALSE );
	m_HelpContentImage->SetVisible( FALSE );
	m_Select_Delete_Btn->SetVisible( FALSE );

	m_ClassViewImage->SetVisible( FALSE );	
	m_ClassViewImageSiGong->SetVisible( FALSE );	
	

	tbox[0]->SetVisible( FALSE );
	tbox[1]->SetVisible( FALSE );
	tbox[2]->SetVisible( FALSE );
	tbox[3]->SetVisible( FALSE );
	tbox[4]->SetVisible( FALSE );
	tbox[5]->SetVisible( FALSE );
	tbox[6]->SetVisible( FALSE );
	
	jobtbox[0]->SetVisible( FALSE );
	jobtbox[1]->SetVisible( FALSE );		
	jobtbox[2]->SetVisible( FALSE );		
	jobtbox[3]->SetVisible( FALSE );		
	jobtbox[4]->SetVisible( FALSE );		
	jobtbox[5]->SetVisible( FALSE );		
	jobtbox[6]->SetVisible( FALSE );
#ifdef DECO_RENEWAL_MJH
	jobtbox[7]->SetVisible( FALSE );
#endif // DECO_RENEWAL_MJH

	jobtimg[1]->SetVisible( FALSE );
	jobtimg[2]->SetVisible( FALSE );
	jobtimg[3]->SetVisible( FALSE );
	jobtimg[4]->SetVisible( FALSE );
	jobtimg[5]->SetVisible( FALSE );
	jobtimg[6]->SetVisible( FALSE );
#ifdef DECO_RENEWAL_MJH
	jobtimg[7]->SetVisible( FALSE );
#endif // DECO_RENEWAL_MJH

	nationimg->SetVisible( FALSE );
	racetext->SetVisible( FALSE );
	racehelptext->SetVisible( false );

#ifdef DECO_RENEWAL_MJH
	for( int i = 0 ; i < MAX_PC_SLOT_NUM ; ++i )
	{
		m_PcText[i].PcImgBox->SetVisible(FALSE);
		m_PcText[i].PcNameTxt->SetVisible(FALSE);
		m_PcText[i].PcLevelTxt->SetVisible(FALSE);
		m_PcText[i].PcClassTxt->SetVisible(FALSE);
	}
	m_Chracterinfotext->SetVisible( FALSE );
	m_Guildinfotext->SetVisible( FALSE );
	m_JoinCorpstext->SetVisible( FALSE );
	m_JoinGuildtext->SetVisible( FALSE );
	m_MapPositiontext->SetVisible( FALSE );
	m_FameText->SetVisible( FALSE );
	m_PcFameText->SetVisible( FALSE );
	m_GuildFameText->SetVisible( FALSE );

	m_Select_exit_Btn->SetVisible( FALSE);

#endif //DECO_RENEWAL_MJH


		
	
}

//................................................................................................................
// 생성창 버튼
//................................................................................................................
void CCharSelectWnd::SetCreateCharacterBtn(void)
{
	SetCloseAllBtn();
	
	m_Create_Rot_Left_Btn->SetVisible( TRUE );
	m_Create_Rot_Right_Btn->SetVisible( TRUE );
	m_Create_Hair_Left_Btn->SetVisible( TRUE );
	m_Create_Hair_Right_Btn->SetVisible( TRUE );
	m_Create_Face_Left_Btn->SetVisible( TRUE );
	m_Create_Face_Right_Btn->SetVisible( TRUE );
	m_Create_Create_Btn->SetVisible( TRUE );
	m_Create_Cancel_Btn->SetVisible( TRUE );
	m_Create_Male_Btn->SetVisible( TRUE );
	m_Create_Female_Btn->SetVisible( TRUE );
	m_Create_Name_Edit->SetVisible( TRUE );
#ifdef DECO_RENEWAL_MJH
	selectchartext->SetVisible( TRUE );
	nationimg->SetClientImage( SetRenewalClassImg(m_ViewClass) );
#else
	m_Create_Millena_Btn->SetVisible( TRUE );
	m_nationtext_rain->SetVisible(true);						
	m_nationtext_millena->SetVisible(true);	
	m_Create_Rain_Btn->SetVisible( TRUE );
	m_pClassViewBtn->SetVisible( TRUE );
	tbox[2]->SetVisible( true );
#endif // DECO_RENEWAL_MJH

	nRui->SendEvent( WID_CharacterZoom , n_emSetUnChecked , 0 , 0 , 0 , 0 );
	
	m_ISZoom = FALSE;
	m_pZoomBtn->SetVisible( TRUE );
	m_CreateDlgmage->SetVisible( TRUE );
	m_Create_Name_Edit->SetText(_RT("") );
	tbox[0]->SetVisible( true );
	tbox[1]->SetVisible( true );
	tbox[3]->SetVisible( true );
	tbox[4]->SetVisible( true );
	tbox[5]->SetVisible( true );
	tbox[6]->SetVisible( true );

#ifdef DECO_RENEWAL_MJH	
	
	m_ClassViewReturn->SetVisible( false );
	jobtbox[0]->SetVisible( true );
	jobtbox[1]->SetVisible( true );
	jobtbox[2]->SetVisible( true );
	jobtbox[3]->SetVisible( true );
	jobtbox[4]->SetVisible( true );
	jobtbox[5]->SetVisible( true );
	jobtbox[6]->SetVisible( true );
#ifdef DECO_RENEWAL_MJH
	jobtbox[7]->SetVisible( true );
	jobtbox[0]->SetVisible( false );
#endif // DECO_RENEWAL_MJH		
		
	jobtimg[1]->SetVisible( true );
	jobtimg[2]->SetVisible( true );
	jobtimg[3]->SetVisible( true );
	jobtimg[4]->SetVisible( true );
	jobtimg[5]->SetVisible( true );
	jobtimg[6]->SetVisible( true );
#ifdef DECO_RENEWAL_MJH
	jobtimg[7]->SetVisible( true );
#endif // DECO_RENEWAL_MJH
	nationimg->SetVisible( true ); //국가 이미지
	racetext->SetVisible( true );  //국가 텍스트
	
	m_pHelpText->SetVisible( TRUE );
	m_pHelpText->SetStartLine( 0 );
	//m_HelpImage->SetVisible( TRUE );  //헬프 배경이미지
	//racehelptext->SetVisible( true ); //헬프 텍스트
	m_HelpContentImage->SetVisible( TRUE );

	float fRotY = 0.0f;

	///-- 밀레나
	if( m_SEQState == SEQ_MILENA_CREATE ||
		m_SEQState == SEQ_MILENA_CLASS
      )
	{
		m_SEQState  = SEQ_MILENA_CLASS;

		fRotY = D3DXToRadian( 90 ); 
#ifdef DECO_RENEWAL_MJH
		m_KnightBtn->SetVisible( TRUE );      
		m_SlayerBtn->SetVisible( TRUE );      
		m_MercenaryBtn->SetVisible( TRUE );   
		m_Soul_BreakerBtn->SetVisible( TRUE );
		m_PriestBtn->SetVisible( TRUE );      
		m_ProminasBtn->SetVisible( TRUE );    
		m_SummonerBtn->SetVisible( TRUE );
		m_ClassViewImage->SetVisible( TRUE ); 
#else 
		m_SwordManBtn->SetVisible( TRUE );
		m_KnightBtn->SetVisible( TRUE );
		m_MercenaryBtn->SetVisible( TRUE );
		m_SlayerBtn->SetVisible( TRUE );
		m_Soul_BreakerBtn->SetVisible( TRUE );
		m_ArcherBtn->SetVisible( TRUE );
		m_ClassViewImage->SetVisible( TRUE );
#endif // DECO_RENEWAL_MJH


		
#ifdef DECO_RENEWAL_MJH  
		jobtbox[1]->SetText( G_STRING_CHAR( IDS_CLASS_KNIGHT )); 
		jobtbox[2]->SetText( G_STRING_CHAR( IDS_CLASS_PRIEST )); 
		jobtbox[3]->SetText( G_STRING_CHAR( IDS_CLASS_SLAYER )); 
		jobtbox[4]->SetText( G_STRING_CHAR( IDS_CLASS_MERCENARY )); 
		jobtbox[5]->SetText( G_STRING_CHAR( IDS_CLASS_SNIPER )); 
		jobtbox[6]->SetText( G_STRING_CHAR( IDS_CLASS_PROMINAS ) ); 
		jobtbox[7]->SetText( G_STRING_CHAR( IDS_CLASS_PSYCHE ) );

#else 	
		jobtbox[0]->SetText( G_STRING_CHAR( IDS_CLASS_WANDERER ) );
		jobtbox[1]->SetText( G_STRING_CHAR( IDS_CLASS_SWORDMAN ));
		jobtbox[2]->SetText( G_STRING_CHAR( IDS_CLASS_ARCHER ));
		jobtbox[3]->SetText( G_STRING_CHAR( IDS_CLASS_KNIGHT ));
		jobtbox[4]->SetText( G_STRING_CHAR( IDS_CLASS_SLAYER ));
		jobtbox[5]->SetText( G_STRING_CHAR( IDS_CLASS_MERCENARY ));
		jobtbox[6]->SetText( G_STRING_CHAR( IDS_CLASS_SNIPER ) );
		if( g_RockClient.GetLanguage() == eLang_Korea || 
			g_RockClient.GetLanguage() == eLang_China ||
			g_RockClient.GetLanguage() == eLang_Japan )
		{
			m_ClassViewImageSiGong->SetVisible( TRUE );
			}				
#endif // DECO_RENEWAL_MJH

#ifdef DECO_RENEWAL_MJH	
		jobtimg[1]->SetClientImage( TID_None );
		jobtimg[2]->SetClientImage( TID_None ); 
		jobtimg[3]->SetClientImage( TID_None );
		jobtimg[4]->SetClientImage( TID_None );
		jobtimg[5]->SetClientImage( TID_None );
		jobtimg[6]->SetClientImage( TID_None );
		jobtimg[7]->SetClientImage( TID_None );
		//nationimg->SetClientImage( TID_prejob_knight );
		racetext->SetText(  "클래스 선택"  );
#else
		jobtimg[1]->SetClientImage( TID_prejob_sword );
		jobtimg[2]->SetClientImage( TID_prejob_archer	 );
		jobtimg[3]->SetClientImage( TID_prejob_knight	 );
		jobtimg[4]->SetClientImage( TID_prejob_slayer	 );
		jobtimg[5]->SetClientImage( TID_prejob_mercynary );
		jobtimg[6]->SetClientImage( TID_prejob_soul		 );
		nationimg->SetClientImage( TID_Millena_Previwe );
		racetext->SetText( G_STRING_CHAR( IDS_MILLENA ) );

		m_HelpImage->SetVisible( TRUE );  //헬프 배경이미지
		racehelptext->SetVisible( TRUE ); //헬프 텍스트
#endif // DECO_RENEWAL_MJH
			
			

			
			
		}
		
#ifdef DECO_RENEWAL_MJH
		g_Pc.DisappearPC( PosInfo[ CSW_MILLENA_POS ].Unique );
		g_Pc.DisappearPC( PosInfo[ CSW_RAIN_POS ].Unique );
#else
		g_Pc.DisappearPC( PosInfo[ CSW_MILLENA_POS ].Unique );
		g_Pc.DisappearPC( PosInfo[ CSW_RAIN_POS ].Unique );
#endif
		g_Pc.DisappearPC( CLASS_UNIQUE );

#ifdef LOGINPAGE_BGIMG_CHANGE
			g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_MILLENA_POS ].Unique , 
									 -1 );
			g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_RAIN_POS ].Unique , 
									 -1 );
			g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , CLASS_UNIQUE , 
									 -1 );			
#endif
		CLASS_VIEW_HELP	*pHelp = NULL;

		pHelp     = g_RockClient.m_ClassView.GetHelpData( m_ViewClass );

		if( pHelp != NULL )
		{
#ifdef UNICODE_ROCK
			wstring  TempStr;
#else
			string  TempStr;
#endif

			for( int i = 0 ; i < pHelp->StrHelp.size() ; ++i )
			{
				TempStr = TempStr + pHelp->StrHelp[ i ];			
				TempStr = TempStr + _RT("##");
			}
				
			m_pHelpText->SetText( (void*)TempStr.c_str() );
		}
		
		CLASS_VIEW_DATA	*pCharInfo;			

		pCharInfo = g_RockClient.m_ClassView.GetData( m_ViewClass );

		if( NULL == pCharInfo )
			return;

		PCTABLE	pc;

		pc.Init();

		pc.lUnique  = CLASS_UNIQUE;

		if( m_SEQState == SEQ_MILENA_CLASS )	
		{
#ifdef DECO_RENEWAL_MJH
			pc.lBlock     = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
			pc.lDestBlock = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
#else
		    pc.lBlock     = ( PosInfo[ CSW_MILLENA_POS ].CelPos[ 0 ] + PosInfo[ CSW_MILLENA_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
			pc.lDestBlock = ( PosInfo[ CSW_MILLENA_POS ].CelPos[ 0 ] + PosInfo[ CSW_MILLENA_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
#endif
		}
		else if( m_SEQState == SEQ_RAIN_CLASS )
		{
			pc.lBlock     = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
			pc.lDestBlock = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
		}
		
		pc.nIndex = g_Pc.GetAttrIndex( pCharInfo->Race , pCharInfo->Sex );

#ifdef DECO_RENEWAL_MJH
		pc.char_info.theHair = m_pSelectHairCode[ m_Gender ][ m_SelectHairNum ];
		pc.char_info.theFace = m_pSelectFaceCode[ m_Gender ][ m_SelectFaceNum ];
#else
		pc.char_info.theHair = pCharInfo->Hair;
		pc.char_info.theFace = pCharInfo->Face;
#endif // DECO_RENEWAL_MJH

		pc.lCurMap    = 0;

		pc.char_info.theWearItem[ n_WearSlot_CoatArmor  ].theCode = ( WORD )pCharInfo->Coat;
		pc.char_info.theWearItem[ n_WearSlot_PantsArmor ].theCode = ( WORD )pCharInfo->Pants;

		if( -1 != pCharInfo->Armor )	
			pc.char_info.theWearItem[ n_WearSlot_Weapon		].theCode = ( WORD )pCharInfo->Armor;
		else
			pc.char_info.theWearItem[ n_WearSlot_Weapon		].theCode = 0;

		if( -1 != pCharInfo->Shiled )	
			pc.char_info.theWearItem[ n_WearSlot_Shield     ].theCode = ( WORD )pCharInfo->Shiled;
		else
			pc.char_info.theWearItem[ n_WearSlot_Shield     ].theCode = 0;

	    if( -1 != pCharInfo->Helmet )
			pc.char_info.theWearItem[ n_WearSlot_Helmet     ].theCode = pCharInfo->Helmet;
		else
			pc.char_info.theWearItem[ n_WearSlot_Helmet     ].theCode = 0;
		
		pc.char_info.theWearItem[ n_WearSlot_Gloves     ].theCode = ( WORD )pCharInfo->Gloves;
		pc.char_info.theWearItem[ n_WearSlot_Shoes      ].theCode = ( WORD )pCharInfo->Shoes;

		g_Pc.AppearPC( CLASS_UNIQUE , pc );

#ifdef LOGINPAGE_BGIMG_CHANGE
			g_Particle.SetEffSetCon( 0 , KOREA_EFFECT , 0 , 
								  EFFECT_SET_LOOP_TIME , CLASS_UNIQUE , -1  );
			g_Particle.SetEffSetCon( 0 , KOREA_EFFECT , 0 , 
								  EFFECT_SET_LOOP_TIME , CLASS_UNIQUE , -1  );
#endif
		int table_index = g_Pc.FindUser( CLASS_UNIQUE );

#ifdef DECO_RENEWAL_MJH
#else 
		g_Pc.m_Pc[table_index].m_Mov.now_Yrad = -90;
#endif // DECO_RENEWAL_MJH
		g_Pc.m_Pc[table_index].SetDirection( g_Camera.m_vEye.x , g_Camera.m_vEye.z );

		// 얼굴 / 머리 부분 초기화 
		g_Pc.m_Pc[table_index].SetWearItem();

		g_Pc.m_Pc[table_index].AnimationPlayNoneBlend( n_Stand_Select9 );

		if( m_SEQState == SEQ_MILENA_CLASS )	
		{
			g_Pc.m_PcTable[ g_Pc.m_Pc[table_index].m_nTableIndex ].char_info.theCitizen = MILLENA;
		}
		else if( m_SEQState == SEQ_RAIN_CLASS )
		{
			g_Pc.m_PcTable[ g_Pc.m_Pc[table_index].m_nTableIndex ].char_info.theCitizen = RAIN;
		}

		g_Pc.m_Pc[table_index].m_pUserInfo->bAttack = TRUE;


		if( pCharInfo->AniCount )
		{
			EVENT_DATA_INFO temp;

			temp.Clear();
			temp.type   = SM_ANI_QUEUE_PROCESS;
			temp.motion = pCharInfo->AniList[ 0 ];

			NonPlayer::m_pCharInfo = pCharInfo;
			g_Pc.m_Pc[table_index].PushQueue( temp, TRUE );
			
			if( g_Pc.m_PcTable[ g_Pc.m_Pc[table_index].m_nTableIndex ].char_info.theCitizen == MILLENA )
			{
				switch( m_ViewClass )
				{
					case Sniper:
					case Mercenary:
#ifdef DECO_RENEWAL_MJH
#else 
						g_Pc.m_Pc[table_index].m_Mov.now_Yrad = D3DXToRadian( 90 );
#endif // DECO_RENEWAL_MJH
						
						break;

					default:
#ifdef DECO_RENEWAL_MJH
#else 
						g_Pc.m_Pc[table_index].m_Mov.now_Yrad = D3DXToRadian( 135 );
#endif // DECO_RENEWAL_MJH
						break;
				}

			}
			else
			{
				switch( m_ViewClass )
				{
					case Psyche:
#ifdef DECO_RENEWAL_MJH
#else 
						g_Pc.m_Pc[table_index].m_Mov.now_Yrad = -D3DXToRadian( 135 );
#endif // DECO_RENEWAL_MJH	
						break;
					
					default:
#ifdef DECO_RENEWAL_MJH
#else 
						g_Pc.m_Pc[table_index].m_Mov.now_Yrad = -D3DXToRadian( 90 );
#endif // DECO_RENEWAL_MJH
						
						break;
				}
			}
		}
#endif 
}

//................................................................................................................
// 밀레나 생성창 -> 선택창으로
//................................................................................................................
void CCharSelectWnd::StartReturnMillenaToSelect(void)
{
	if( m_ISZoom )
	{
		g_Camera.SetAnimation( 7 );
	

		g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::StartReturnMillenaToSelect);
		SetCloseAllBtn();
		m_ISZoom = FALSE;
	}
	else
	{
		SetCloseAllBtn();
		
		pCharList[ 0 ] = pCharList[ 1 ] = pCharList[ 2 ] = pCharList[ 3 ] = pCharList[ 4 ] = NULL;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/29)
		
		// 생성창에서 미리보기 후 선택창으로 돌아오는 과정에서 캐릭터 생성/삭제 순서를 삭제/생성으로 바꿔줌.
		// 기존 캐릭터 선택을 UI 버튼으로 하던걸 3D 픽킹으로 바꾸는 과정에서 생긴 코드
		g_Pc.DisappearPC( PosInfo[ CSW_MILLENA_POS ].Unique );
		g_Pc.DisappearPC( PosInfo[ CSW_RAIN_POS ].Unique );
		
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/29)

		
		SelectCharacter();
#ifdef DECO_RENEWAL_MJH
	g_Camera.SetAnimation( 0 );
#else
	g_Camera.SetAnimation( 2 );
#endif//DECO_RENEWAL_MJH	

		g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::StartReturnMillenaToSelectCameraWalkEnd);
		
		g_Map.InitMapData();
		
		m_SEQState = SEQ_CAMERA_ANI;
	}
}

//................................................................................................................
// 밀레나 생성창 -> 선택창으로 ( 끝났을때 )
//................................................................................................................
void CCharSelectWnd::StartReturnMillenaToSelectCameraWalkEnd(void)
{
	g_Pc.DisappearPC( PosInfo[ CSW_MILLENA_POS ].Unique );
	g_Pc.DisappearPC( PosInfo[ CSW_RAIN_POS ].Unique );

	
#ifdef LOGINPAGE_BGIMG_CHANGE
	
		g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_MILLENA_POS ].Unique , 
								 -1 );
		g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_RAIN_POS ].Unique , 
								 -1 );	
#endif

	m_SEQState = SEQ_SELECT_CHAR;
	SetNoneCharacterSelectBtn();
}

//................................................................................................................
// 밀레나 -> 레인 
//................................................................................................................
void CCharSelectWnd::StartMillenaToRain(void)
{
	if( m_ISZoom )
	{
		g_Camera.SetAnimation( 7 );
		g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::ZoomOutEndProcess);
		SetCloseAllBtn();
		m_ISZoom = FALSE;
	}
	else
	{
		SetCloseAllBtn();
		
		pCharList[ 0 ] = pCharList[ 1 ] = pCharList[ 2 ] = pCharList[ 3 ] = pCharList[ 4 ] = NULL;
		
		g_Camera.SetAnimation( 6 );
		g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::StartMillenaToRainCameraWalkEnd);
		
		m_SEQState = SEQ_CAMERA_ANI;
		
		m_SelectHairNum = 0;
		m_SelectFaceNum = 0;
		
		m_Citizen = RAIN;
		m_Gender  = MALE;
		
		CreateCharacter( m_Citizen , m_Gender );
		
		ChangeFaceAndHair();
	}
}

//................................................................................................................
// 밀레나 -> 레인 ( 끝났을때 )
//................................................................................................................
void CCharSelectWnd::StartMillenaToRainCameraWalkEnd(void)
{
	m_SEQState = SEQ_RAIN_CREATE;
	SetCreateCharacterBtn();
}

//................................................................................................................
// 레인 -> 밀레나  
//................................................................................................................
void CCharSelectWnd::StartRainToMillena(void)
{
	if( m_ISZoom )
	{
		g_Camera.SetAnimation( 9 );

		g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::ZoomOutEndProcess);
		SetCloseAllBtn();
		m_ISZoom = FALSE;
	}
	else
	{
		SetCloseAllBtn();
		
		pCharList[ 0 ] = pCharList[ 1 ] = pCharList[ 2 ] = pCharList[ 3 ] = pCharList[ 4 ] = NULL;
		
		g_Camera.SetAnimation( 5 );
		g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::StartRainToMillenaCameraWalkEnd);
		
		m_SEQState = SEQ_CAMERA_ANI;
		
		m_SelectHairNum = 0;
		m_SelectFaceNum = 0;
		
		m_Citizen = MILLENA;
		m_Gender  = MALE;
		
		CreateCharacter( m_Citizen , m_Gender );
		
		ChangeFaceAndHair();
	}
}

//................................................................................................................
// 줌 아웃후 레인 밀레나 창으로 이동
//................................................................................................................
void CCharSelectWnd::ZoomOutEndProcess()
{
	if( m_SEQState == SEQ_MILENA_CREATE )
	{
		StartMillenaToRain();
	}
	else if( m_SEQState == SEQ_RAIN_CREATE )
	{
		StartRainToMillena();
	}
	
}

//................................................................................................................
// 레인 -> 밀레나 ( 끝났을때 )
//................................................................................................................
void CCharSelectWnd::StartRainToMillenaCameraWalkEnd(void)
{
	m_SEQState = SEQ_MILENA_CREATE;
	SetCreateCharacterBtn();
}



//................................................................................................................
// 레인 생성창 -> 선택창으로
//................................................................................................................
void CCharSelectWnd::StartReturnRainToSelect(void)
{
	if( m_ISZoom )
	{
		g_Camera.SetAnimation( 9 );
		g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::StartReturnRainToSelect);
		SetCloseAllBtn();
		m_ISZoom = FALSE;
	}
	else
	{
		SetCloseAllBtn();
		
		pCharList[ 0 ] = pCharList[ 1 ] = pCharList[ 2 ] = pCharList[ 3 ] = pCharList[ 4 ] = NULL;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/29)
		
		// 생성창에서 미리보기 후 선택창으로 돌아오는 과정에서 캐릭터 생성/삭제 순서를 삭제/생성으로 바꿔줌.
		// 기존 캐릭터 선택을 UI 버튼으로 하던걸 3D 픽킹으로 바꾸는 과정에서 생긴 코드
		g_Pc.DisappearPC( PosInfo[ CSW_MILLENA_POS ].Unique );
		g_Pc.DisappearPC( PosInfo[ CSW_RAIN_POS ].Unique );
		
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/29)

		
		SelectCharacter();
		
		g_Camera.SetAnimation( 4 );
		g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::StartReturnRainToSelectCameraWalkEnd);
		
		g_Map.InitMapData();
		
		m_SEQState = SEQ_CAMERA_ANI;
	}
}

//................................................................................................................
// 레인 생성창 -> 선택창으로 ( 끝났을때 )
//................................................................................................................
void CCharSelectWnd::StartReturnRainToSelectCameraWalkEnd(void)
{
	g_Pc.DisappearPC( PosInfo[ CSW_MILLENA_POS ].Unique );
	g_Pc.DisappearPC( PosInfo[ CSW_RAIN_POS ].Unique );

#ifdef LOGINPAGE_BGIMG_CHANGE
		g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_MILLENA_POS ].Unique , 
								 -1 );
		g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_RAIN_POS ].Unique , 
								 -1 );	
#endif
	m_SEQState = SEQ_SELECT_CHAR;
	SetNoneCharacterSelectBtn();
}

void CCharSelectWnd::EndZoomProcess()
{
	switch( m_SEQState )
	{
		// 줌 밀레나
		case SEQ_CAMERA_ZOOM_MILLENA:
		case SEQ_CAMERA_ZOOM_OUT_MILLENA:
	
			m_SEQState = SEQ_MILENA_CREATE;

			break;
		
		// 줌 레인
		case SEQ_CAMERA_ZOOM_RAIN:
		case SEQ_CAMERA_ZOOM_OUT_RAIN:

			m_SEQState = SEQ_RAIN_CREATE;

			break;
	}


	m_Create_Rot_Left_Btn->SetVisible( TRUE );
	m_Create_Rot_Right_Btn->SetVisible( TRUE );
	m_Create_Hair_Left_Btn->SetVisible( TRUE );
	m_Create_Hair_Right_Btn->SetVisible( TRUE );
	m_Create_Face_Left_Btn->SetVisible( TRUE );
	m_Create_Face_Right_Btn->SetVisible( TRUE );
	m_Create_Create_Btn->SetVisible( TRUE );
	m_Create_Cancel_Btn->SetVisible( TRUE );
	m_Create_Millena_Btn->SetVisible( TRUE );
	m_nationtext_rain->SetVisible(true);						
	m_nationtext_millena->SetVisible(true);						
	m_Create_Male_Btn->SetVisible( TRUE );
	m_Create_Rain_Btn->SetVisible( TRUE );
	m_Create_Rain_Btn->SetVisible( TRUE );
	m_Create_Female_Btn->SetVisible( TRUE );
	m_Create_Name_Edit->SetVisible( TRUE );
	
	m_pClassViewBtn->SetVisible( TRUE );
	
	m_pZoomBtn->SetVisible( TRUE );
	m_CreateDlgmage->SetVisible( TRUE );
	tbox[0]->SetVisible( true );
	tbox[1]->SetVisible( true );
	tbox[2]->SetVisible( true );
	tbox[3]->SetVisible( true );
	tbox[4]->SetVisible( true );
	tbox[5]->SetVisible( true );
	tbox[6]->SetVisible( true );
}

//
void CCharSelectWnd::DeletePlayer(void)
{
	if( CharSelectWnd.m_SEQState != SEQ_SELECT_CHAR )
		return;


	switch( ( CharSelectWnd.m_pSelectChar->theCore%10 ) )
	{
		case n_CharacterCore:
			g_RockClient.Send_RpCsDeleteCharacter( CharSelectWnd.m_pSelectChar->theData.thePCName );
			break;

		case n_CoreItSelf:
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_CANT_DEL_CORE ) , 1000 , NULL );	
			break;

		case n_DelCharCore:
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_REDEL_CH ) , 1000 , NULL );    
			break;
	}
	
}

//................................................................................................................
// 클래스 캐릭터 생성
//................................................................................................................
void CCharSelectWnd::CreateClassCharacter()
{
	if( m_ISZoom )
	{
		SetCloseAllBtn();

		if( m_SEQState == SEQ_MILENA_CREATE )
		{
			g_Camera.SetAnimation( 7 );
		}
		else if( m_SEQState == SEQ_RAIN_CREATE )
		{
			g_Camera.SetAnimation( 9 );
		}
		
		g_Camera.m_NextFunc.Insert(&CharSelectWnd, &CCharSelectWnd::CreateClassCharacter);
		m_ISZoom = FALSE;
	}
	else
	{
#ifdef DECO_RENEWAL_MJH
		SetCreateCharacterBtn();
		nationimg->SetClientImage( SetRenewalClassImg(m_ViewClass) );
		m_ClassViewReturn->SetVisible( false );
		selectchartext->SetVisible( TRUE );
#else
		SetCloseAllBtn();
		m_ClassViewReturn->SetVisible( true );
		m_HelpImage->SetVisible( TRUE );  //헬프 배경이미지
		racehelptext->SetVisible( true ); //헬프 텍스트
#endif	//DECO_RENEWAL_MJH	
		jobtbox[0]->SetVisible( true );
		jobtbox[1]->SetVisible( true );
		jobtbox[2]->SetVisible( true );
		jobtbox[3]->SetVisible( true );
		jobtbox[4]->SetVisible( true );
		jobtbox[5]->SetVisible( true );
		jobtbox[6]->SetVisible( true );
#ifdef DECO_RENEWAL_MJH
		jobtbox[7]->SetVisible( true );
		jobtbox[0]->SetVisible( false );
#endif // DECO_RENEWAL_MJH		
		
		jobtimg[1]->SetVisible( true );
		jobtimg[2]->SetVisible( true );
		jobtimg[3]->SetVisible( true );
		jobtimg[4]->SetVisible( true );
		jobtimg[5]->SetVisible( true );
		jobtimg[6]->SetVisible( true );
#ifdef DECO_RENEWAL_MJH
		jobtimg[7]->SetVisible( true );
#endif // DECO_RENEWAL_MJH
		nationimg->SetVisible( true ); //국가 이미지
		racetext->SetVisible( true );  //국가 텍스트
		
		m_pHelpText->SetVisible( TRUE );
		m_pHelpText->SetStartLine( 0 );
		m_HelpContentImage->SetVisible( TRUE );

		float fRotY = 0.0f;

		///-- 밀레나
		if( m_SEQState == SEQ_MILENA_CREATE ||
			m_SEQState == SEQ_MILENA_CLASS
     	  )
		{
			m_SEQState  = SEQ_MILENA_CLASS;

			fRotY = D3DXToRadian( 90 ); 
#ifdef DECO_RENEWAL_MJH
			m_KnightBtn->SetVisible( TRUE );      
			m_SlayerBtn->SetVisible( TRUE );      
			m_MercenaryBtn->SetVisible( TRUE );   
			m_Soul_BreakerBtn->SetVisible( TRUE );
			m_PriestBtn->SetVisible( TRUE );      
			m_ProminasBtn->SetVisible( TRUE );    
			m_SummonerBtn->SetVisible( TRUE );
			m_ClassViewImage->SetVisible( TRUE );
			
#else 
			m_SwordManBtn->SetVisible( TRUE );
			m_KnightBtn->SetVisible( TRUE );
			m_MercenaryBtn->SetVisible( TRUE );
			m_SlayerBtn->SetVisible( TRUE );
			m_Soul_BreakerBtn->SetVisible( TRUE );
			m_ArcherBtn->SetVisible( TRUE );
			m_ClassViewImage->SetVisible( TRUE );

#endif // DECO_RENEWAL_MJH


		
#ifdef DECO_RENEWAL_MJH  
			jobtbox[1]->SetText( G_STRING_CHAR( IDS_CLASS_KNIGHT )); 
			jobtbox[2]->SetText( G_STRING_CHAR( IDS_CLASS_PRIEST )); 
			jobtbox[3]->SetText( G_STRING_CHAR( IDS_CLASS_SLAYER )); 
			jobtbox[4]->SetText( G_STRING_CHAR( IDS_CLASS_MERCENARY )); 
			jobtbox[5]->SetText( G_STRING_CHAR( IDS_CLASS_SNIPER )); 
			jobtbox[6]->SetText( G_STRING_CHAR( IDS_CLASS_PROMINAS ) ); 
			jobtbox[7]->SetText( G_STRING_CHAR( IDS_CLASS_PSYCHE ) );

#else 	
			jobtbox[0]->SetText( G_STRING_CHAR( IDS_CLASS_WANDERER ) );
			jobtbox[1]->SetText( G_STRING_CHAR( IDS_CLASS_SWORDMAN ));
			jobtbox[2]->SetText( G_STRING_CHAR( IDS_CLASS_ARCHER ));
			jobtbox[3]->SetText( G_STRING_CHAR( IDS_CLASS_KNIGHT ));
			jobtbox[4]->SetText( G_STRING_CHAR( IDS_CLASS_SLAYER ));
			jobtbox[5]->SetText( G_STRING_CHAR( IDS_CLASS_MERCENARY ));
			jobtbox[6]->SetText( G_STRING_CHAR( IDS_CLASS_SNIPER ) );
			if( g_RockClient.GetLanguage() == eLang_Korea || 
				g_RockClient.GetLanguage() == eLang_China ||
				g_RockClient.GetLanguage() == eLang_Japan )
			{
				m_ClassViewImageSiGong->SetVisible( TRUE );
			}				
#endif // DECO_RENEWAL_MJH

#ifdef DECO_RENEWAL_MJH	
			jobtimg[1]->SetClientImage( TID_None );
			jobtimg[2]->SetClientImage( TID_None ); 
			jobtimg[3]->SetClientImage( TID_None );
			jobtimg[4]->SetClientImage( TID_None );
			jobtimg[5]->SetClientImage( TID_None );
			jobtimg[6]->SetClientImage( TID_None );
			jobtimg[7]->SetClientImage( TID_None );
			racetext->SetText(  "클래스 선택"  );
#else
			jobtimg[1]->SetClientImage( TID_prejob_sword );
			jobtimg[2]->SetClientImage( TID_prejob_archer	 );
			jobtimg[3]->SetClientImage( TID_prejob_knight	 );
			jobtimg[4]->SetClientImage( TID_prejob_slayer	 );
			jobtimg[5]->SetClientImage( TID_prejob_mercynary );
			jobtimg[6]->SetClientImage( TID_prejob_soul		 );
			nationimg->SetClientImage( TID_Millena_Previwe );
			racetext->SetText( G_STRING_CHAR( IDS_MILLENA ) );

			m_HelpImage->SetVisible( TRUE );  //헬프 배경이미지
			racehelptext->SetVisible( TRUE ); //헬프 텍스트
#endif // DECO_RENEWAL_MJH
			
			
		}
		///-- 레인
		else if( m_SEQState == SEQ_RAIN_CREATE ||
			m_SEQState == SEQ_RAIN_CLASS
			)
		{
			m_SEQState = SEQ_RAIN_CLASS;
			
			fRotY = -D3DXToRadian( 90 ); 
			
			m_MagicianBtn->SetVisible( TRUE );
			m_ShamanBtn->SetVisible( TRUE );
			m_ProminasBtn->SetVisible( TRUE );
			m_PriestBtn->SetVisible( TRUE );
			m_Holy_AvengerBtn->SetVisible( TRUE );
			m_SummonerBtn->SetVisible( TRUE );
			m_ClassViewImage->SetVisible( TRUE );
			if( g_RockClient.GetLanguage() == eLang_Korea || 
				g_RockClient.GetLanguage() == eLang_China ||
				g_RockClient.GetLanguage() == eLang_Japan )
			{
				m_ClassViewImageSiGong->SetVisible( TRUE );
			}

			jobtbox[0]->SetText( G_STRING_CHAR( IDS_CLASS_STRANGER ) );
			jobtbox[1]->SetText( G_STRING_CHAR( IDS_CLASS_MAGICIAN ) );
			jobtbox[2]->SetText( G_STRING_CHAR( IDS_CLASS_SHAMAN ) );
			jobtbox[3]->SetText( G_STRING_CHAR( IDS_CLASS_HOLYAVENGER ) );
			jobtbox[4]->SetText( G_STRING_CHAR( IDS_CLASS_PROMINAS ) );
			jobtbox[5]->SetText( G_STRING_CHAR( IDS_CLASS_PRIEST ) );
			jobtbox[6]->SetText( G_STRING_CHAR( IDS_CLASS_PSYCHE ) );
			
			jobtimg[1]->SetClientImage( TID_prejob_magician	 );
			jobtimg[2]->SetClientImage( TID_prejob_shaman );
			jobtimg[3]->SetClientImage( TID_prejob_holy	 );			
			jobtimg[4]->SetClientImage( TID_prejob_prom	 );
			jobtimg[5]->SetClientImage( TID_prejob_prist );
			jobtimg[6]->SetClientImage( TID_prejob_shamer );
			
			
			nationimg->SetClientImage( TID_Rain_Previwe );
			racetext->SetText(G_STRING_CHAR( IDS_RAIN ));
		}
		
	//	m_pHelpText->SetVisible( TRUE );
	//	m_pHelpText->SetStartLine( 0 );
	//	m_HelpImage->SetVisible( TRUE );
	//	m_HelpContentImage->SetVisible( TRUE );

		g_Pc.DisappearPC( PosInfo[ CSW_MILLENA_POS ].Unique );
		g_Pc.DisappearPC( PosInfo[ CSW_RAIN_POS ].Unique );

		g_Pc.DisappearPC( CLASS_UNIQUE );

#ifdef LOGINPAGE_BGIMG_CHANGE
			g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_MILLENA_POS ].Unique , 
									 -1 );
			g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , PosInfo[ CSW_RAIN_POS ].Unique , 
									 -1 );
			g_Particle.DelEffSetCon( KOREA_EFFECT , 0 , CLASS_UNIQUE , 
									 -1 );			
#endif
		CLASS_VIEW_HELP	*pHelp = NULL;

		pHelp     = g_RockClient.m_ClassView.GetHelpData( m_ViewClass );

		if( pHelp != NULL )
		{
#ifdef UNICODE_ROCK
			wstring  TempStr;
#else
			string  TempStr;
#endif

			for( int i = 0 ; i < pHelp->StrHelp.size() ; ++i )
			{
				TempStr = TempStr + pHelp->StrHelp[ i ];			
				TempStr = TempStr + _RT("##");
			}
				
			m_pHelpText->SetText( (void*)TempStr.c_str() );
		}
		
		CLASS_VIEW_DATA	*pCharInfo;			

		pCharInfo = g_RockClient.m_ClassView.GetData( m_ViewClass );

		if( NULL == pCharInfo )
			return;

		PCTABLE	pc;

		pc.Init();

		pc.lUnique  = CLASS_UNIQUE;

		if( m_SEQState == SEQ_MILENA_CLASS )	
		{
#ifdef DECO_RENEWAL_MJH
			pc.lBlock     = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
			pc.lDestBlock = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
#else
		    pc.lBlock     = ( PosInfo[ CSW_MILLENA_POS ].CelPos[ 0 ] + PosInfo[ CSW_MILLENA_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
			pc.lDestBlock = ( PosInfo[ CSW_MILLENA_POS ].CelPos[ 0 ] + PosInfo[ CSW_MILLENA_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
#endif
		}
		else if( m_SEQState == SEQ_RAIN_CLASS )
		{
			pc.lBlock     = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
			pc.lDestBlock = ( PosInfo[ CSW_RAIN_POS ].CelPos[ 0 ] + PosInfo[ CSW_RAIN_POS ].CelPos[ 1 ] * g_Pc.m_cSize );
		}
		
		pc.nIndex = g_Pc.GetAttrIndex( pCharInfo->Race , pCharInfo->Sex );

		
#ifdef DECO_RENEWAL_MJH
		if( m_ViewClass == Sniper || m_ViewClass == Prominas || m_ViewClass == Priest || m_ViewClass == Mercenary )
		{
			m_Gender = 1;
		}
		else
		{
			m_Gender = 0;
		}
		pc.char_info.theHair = m_pSelectHairCode[ m_Gender ][ m_SelectHairNum ];
		pc.char_info.theFace = m_pSelectFaceCode[ m_Gender ][ m_SelectFaceNum ];
#else
		pc.char_info.theHair = pCharInfo->Hair;
		pc.char_info.theFace = pCharInfo->Face;
#endif // DECO_RENEWAL_MJH
		

		pc.lCurMap    = 0;

		pc.char_info.theWearItem[ n_WearSlot_CoatArmor  ].theCode = ( WORD )pCharInfo->Coat;
		pc.char_info.theWearItem[ n_WearSlot_PantsArmor ].theCode = ( WORD )pCharInfo->Pants;

		if( -1 != pCharInfo->Armor )	
			pc.char_info.theWearItem[ n_WearSlot_Weapon		].theCode = ( WORD )pCharInfo->Armor;
		else
			pc.char_info.theWearItem[ n_WearSlot_Weapon		].theCode = 0;

		if( -1 != pCharInfo->Shiled )	
			pc.char_info.theWearItem[ n_WearSlot_Shield     ].theCode = ( WORD )pCharInfo->Shiled;
		else
			pc.char_info.theWearItem[ n_WearSlot_Shield     ].theCode = 0;

	    if( -1 != pCharInfo->Helmet )
			pc.char_info.theWearItem[ n_WearSlot_Helmet     ].theCode = pCharInfo->Helmet;
		else
			pc.char_info.theWearItem[ n_WearSlot_Helmet     ].theCode = 0;
		
		pc.char_info.theWearItem[ n_WearSlot_Gloves     ].theCode = ( WORD )pCharInfo->Gloves;
		pc.char_info.theWearItem[ n_WearSlot_Shoes      ].theCode = ( WORD )pCharInfo->Shoes;

		g_Pc.AppearPC( CLASS_UNIQUE , pc );

#ifdef LOGINPAGE_BGIMG_CHANGE
			g_Particle.SetEffSetCon( 0 , KOREA_EFFECT , 0 , 
								  EFFECT_SET_LOOP_TIME , CLASS_UNIQUE , -1  );
			g_Particle.SetEffSetCon( 0 , KOREA_EFFECT , 0 , 
								  EFFECT_SET_LOOP_TIME , CLASS_UNIQUE , -1  );
#endif
		int table_index = g_Pc.FindUser( CLASS_UNIQUE );
#ifdef DECO_RENEWAL_MJH
#else 
		g_Pc.m_Pc[table_index].m_Mov.now_Yrad = fRotY;
#endif // DECO_RENEWAL_MJH	
		g_Pc.m_Pc[table_index].SetDirection( g_Camera.m_vEye.x , g_Camera.m_vEye.z );

		// 얼굴 / 머리 부분 초기화 
		g_Pc.m_Pc[table_index].SetWearItem();

		g_Pc.m_Pc[table_index].AnimationPlayNoneBlend( n_Stand_Select9 );

		if( m_SEQState == SEQ_MILENA_CLASS )	
		{
			g_Pc.m_PcTable[ g_Pc.m_Pc[table_index].m_nTableIndex ].char_info.theCitizen = MILLENA;
		}
		else if( m_SEQState == SEQ_RAIN_CLASS )
		{
			g_Pc.m_PcTable[ g_Pc.m_Pc[table_index].m_nTableIndex ].char_info.theCitizen = RAIN;
		}

		g_Pc.m_Pc[table_index].m_pUserInfo->bAttack = TRUE;


		if( pCharInfo->AniCount )
		{
			EVENT_DATA_INFO temp;

			temp.Clear();
			temp.type   = SM_ANI_QUEUE_PROCESS;
			temp.motion = pCharInfo->AniList[ 0 ];

			NonPlayer::m_pCharInfo = pCharInfo;
			g_Pc.m_Pc[table_index].PushQueue( temp, TRUE );
			
			if( g_Pc.m_PcTable[ g_Pc.m_Pc[table_index].m_nTableIndex ].char_info.theCitizen == MILLENA )
			{
				switch( m_ViewClass )
				{
					case Sniper:
					case Mercenary:
#ifdef DECO_RENEWAL_MJH
#else 
						g_Pc.m_Pc[table_index].m_Mov.now_Yrad = D3DXToRadian( 90 );
#endif // DECO_RENEWAL_MJH
						
						break;

					default:
#ifdef DECO_RENEWAL_MJH
#else 
						g_Pc.m_Pc[table_index].m_Mov.now_Yrad = D3DXToRadian( 135 );
#endif // DECO_RENEWAL_MJH					
						break;
				}

			}
			else
			{
				switch( m_ViewClass )
				{
					case Psyche:
#ifdef DECO_RENEWAL_MJH
#else 
						g_Pc.m_Pc[table_index].m_Mov.now_Yrad = -D3DXToRadian( 135 );
#endif // DECO_RENEWAL_MJH			
						break;
					
					default:
#ifdef DECO_RENEWAL_MJH
#else 
						g_Pc.m_Pc[table_index].m_Mov.now_Yrad = -D3DXToRadian( 90 );
#endif // DECO_RENEWAL_MJH					
						break;
				}
			}
		}
		
	}
}
#ifdef DECO_RENEWAL_MJH

int CCharSelectWnd::GetCharacterSelectNum()
{
	return m_SelectNum;
}

#endif // DECO_RENEWAL_MJH

