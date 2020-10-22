#include "RockPCH.h"
#include "CLogWnd.h"
#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"
#include "VFileSystem.h"
#include "MessageWndProc.h"
#include "ItemRemodelWndProc.h"
#include "ItemCreateWndProc.h"
#include "stringmanager.h"

#include "InventoryWndProc.h"


CLogWnd		g_LogWnd;

CLogWnd::CLogWnd()
{
	m_DisplayType = CLogWnd::COMBO_TAB;
	
	memset( m_theSaveLog , 0 , sizeof( char ) * 70 );

	m_ComboTopNum = 0;
	m_ComboSelectNum = -1;

	memset( m_SubTabType , 0 , sizeof( m_SubTabType ) );
	m_SubTabNum = 0;
	m_SubLevel  = 0;
	m_SubTopNum = 0;

	m_bEnableCombo = true;
}

CLogWnd::~CLogWnd()
{
}

void CLogWnd::Init()
{
}

void CLogWnd::Composition()
{
	CImageBox*		img;
	
	//.........................................................................................................
	// Frame
	//.........................................................................................................
	int center_x = nRui->Desktop.Width  / 2;
	int center_y = nRui->Desktop.Height / 2;

	SRect R;
	
	R.w	= 256;
	R.h	= 516;
	R.x	= center_x - ( R.w / 2 );
	R.y	= center_y - ( R.h / 2 );
	
	m_pFrame = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None , WID_None, R.x, R.y, R.w, R.h, false );
	m_pFrame->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrame->SetClientImage( TID_CTRL_WND_L1 );
	m_pFrame->SetIconTID( TID_CTRL_ICON_LOG );
	m_pFrame->SetWndTileSizeLR( 40, 29 );
	m_pFrame->SetWndTileSizeTB( 31, 11 ); 
	m_pFrame->SetCaption( G_STRING_CHAR( IDS_WND_LOG ) );		
	m_pFrame->SetTitle( true );
	m_pFrame->SetWndProc( this );		
	m_pFrame->SetZorder( n_zoNormal );
	m_pFrame->Initialize();
	m_pFrame->SetVisible( false );
	
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 1, 48, 254, 432, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 1, 47, 254, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 1, 480, 254, 1, false ); 
	img ->SetClientImage( TID_CTRL_LINEDOT );
	img->SetFocusCheck( false );
	
	///-- 보조직업 Line
	//.........................................................................................................
	// 가로선
	//.........................................................................................................
	m_pLineImg[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 
					3 , 355 , 236 , 1 , false );	
	m_pLineImg[0]->SetClientImage( TID_CLIENT );
	m_pLineImg[0]->SetVisible( FALSE );
		
	///-- 상부라인
	m_pLineImg[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 
					1, 75, 254, 1, false ); 
	m_pLineImg[1]->SetClientImage( TID_CTRL_LINEDOT );	
	m_pLineImg[1]->SetVisible( FALSE );
	
	///-- combo Back
	///-- Image9 - 콥보 배경 이미지
	m_img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrame->GetID(),  
			  2, 75, 239, 405, false );	
	m_img9->SetWndTileSizeLR( 9, 9 );
	m_img9->SetWndTileSizeTB( 8, 8 );
	m_img9->SetFocusCheck( false );
	m_img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	m_img9->SetAdjustPosition( 0, 0 );
	m_img9->SetVisible( FALSE );
	
	//.........................................................................................................
	// Tab Combo / SubClass
	//.........................................................................................................
	m_pComboTab = (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_None , m_pFrame->GetID() , 
										3, 28, 78, 20, false );
	m_pComboTab->SetClientImage(TID_CTRL_TAB78);
	m_pComboTab->SetBtnImage( TID_CTRL_TAB78_DIS , TID_CTRL_TAB78_DIS , TID_CTRL_TAB78 , TID_CTRL_TAB78_DIS);
	m_pComboTab->SetDirectCheck( TRUE );

	m_pSubClassTab = (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_None , m_pFrame->GetID() , 
		81, 28, 78, 20, false );
	m_pSubClassTab->SetClientImage(TID_CTRL_TAB78);
	m_pSubClassTab->SetBtnImage( TID_CTRL_TAB78_DIS , TID_CTRL_TAB78_DIS , TID_CTRL_TAB78, TID_CTRL_TAB78_DIS );
	m_pSubClassTab->SetDirectCheck( FALSE );
#if defined (GLOBAL_VERSION) || defined C_SUBJOB_RENEW
	m_pSubClassTab->SetVisible(false);
#endif

	nRui->SetGroupWnd( 2 , m_pComboTab->GetID(),
						   m_pComboTab->GetID(),
						   m_pSubClassTab->GetID() );

	//.........................................................................................................
	// 스크롤 
	//.........................................................................................................
	
	m_pComboScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None , m_pFrame->GetID() ,  
					  241, 75, 14, 405, false );
	m_pComboScroll->InitSlide( stVertSld, 0, 20, 0, 13 );//Type, min, max, value, 썸의 폭
	m_pComboScroll->SetClientImage( TID_CTRL_SCR_BG );
	m_pComboScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pComboScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pComboScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	m_pSubJobScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None , m_pFrame->GetID() ,  
					  241, 75, 14, 405, false );
	m_pSubJobScroll->InitSlide( stVertSld, 0, 20, 0, 13 );//Type, min, max, value, 썸의 폭
	m_pSubJobScroll->SetClientImage( TID_CTRL_SCR_BG );
	m_pSubJobScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pSubJobScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pSubJobScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	//.........................................................................................................
	// 콤보 등록시 버튼 - 등록된 콤보키옆에 나오는 표시
	//.........................................................................................................
	
	m_pComboRegImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 
					  0 , 0 , 10 , 10 , false );	
	m_pComboRegImg->SetClientImage( TID_CTRL_RAD_DIS );
	m_pComboRegImg->SetVisible( FALSE );

	//.........................................................................................................
	// 콤보 선택 버튼 - 이건 의미없는 것인듯..;;;;
	//.........................................................................................................
	m_pComboSelectImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 
						 2 , 77 , 237 , 20 , false );	
	m_pComboSelectImg->SetClientImage( TID_CTRL_TEXT_OVER );
	m_pComboSelectImg->SetVisible( FALSE );

	for( int i = 0 ; i < COMBO_LIST_MAXCOUNT ; ++i )
	{
		m_pComboBtnList[ i ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() , 2 , 
			77 + ( i * 20 ) , 237 , 20 , false );
		m_pComboBtnList[ i ]->SetClientImage( TID_None );
		m_pComboBtnList[ i ]->SetBtnImage( TID_None,  TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );
	}
	
	//.........................................................................................................
	// 콤보 등록 버튼
	//.........................................................................................................
	m_pComboRegBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  
					  8, 485, 58, 25, false );
	m_pComboRegBtn->SetClientImage( TID_CTRL_BTN58 );
	m_pComboRegBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pComboRegBtn->SetFontR( n_fRsvFontWndButton );   
	m_pComboRegBtn->SetText( G_STRING_CHAR( IDS_WND_REGISTER ) );	

	//.........................................................................................................
	// 콤보 등록 취소 버튼
	//.........................................................................................................
	m_pComboRegCancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  
							68, 485, 58, 25, false );	
	m_pComboRegCancelBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );	
	m_pComboRegCancelBtn->SetFontR( n_fRsvFontWndButton );   
	m_pComboRegCancelBtn->SetText( G_STRING_CHAR( IDS_WND_REGCANCEL ) );	
	
	//.........................................................................................................
	// 확인 버튼
	//.........................................................................................................
	m_pOKBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  
				194, 485, 58, 25, false );	
	m_pOKBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pOKBtn->SetFontR( n_fRsvFontWndButton );   
	m_pOKBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ));	
	
	//.........................................................................................................
	// 닫기 버튼
	//.........................................................................................................
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() , 237, 6, 13, 13, false );
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	m_pCloseBtn->SetVisible( TRUE );

	//.........................................................................................................
	// Text Label
	//.........................................................................................................
	CTextBox*		TBox = NULL;

	TBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 3, 28, 78, 20, false );
	TBox->SetFontR( n_fRsvFontWndStatic );
	TBox->SetAlignText( n_atCenter, n_atCenter );
	TBox->SetText( G_STRING_CHAR( IDS_COMBO ) ); 
	TBox->SetTextColor( 0 , 0 , 0 );
	
	TBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 81, 28, 78, 20, false );
	TBox->SetFontR( n_fRsvFontWndStatic );
	TBox->SetAlignText( n_atCenter, n_atCenter );
	TBox->SetText( G_STRING_CHAR( IDS_CHARSTATE_SUBCLASS ) ); 
	TBox->SetTextColor( 0 , 0 , 0 );
#if defined (GLOBAL_VERSION) || defined C_SUBJOB_RENEW
	TBox->SetVisible(false);
#endif
	TComboLabel0 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
					44, 60, 45, 11, false );
	TComboLabel0->SetFontR( n_fRsvFontWndStatic );
	TComboLabel0->SetAlignText( n_atCenter, n_atCenter );
	TComboLabel0->SetText( G_STRING_CHAR( IDS_WND_ORDER ) ); 
	TComboLabel0->SetTextColor( 0 , 0 , 0 );
	
	TComboLabel1 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
					167, 60, 35, 11, false );
	TComboLabel1->SetFontR( n_fRsvFontWndStatic );
	TComboLabel1->SetAlignText( n_atCenter, n_atCenter );
	TComboLabel1->SetText( G_STRING_CHAR( IDS_WND_EFFECT ) ); 
	TComboLabel1->SetTextColor( 0 , 0 , 0 );

	//.........................................................................................................
	// 서브 직업 탭 - 서브직업부에서 웨폰크래프트, 아머크래프트의 제조아이템 타입에 따라 나옴
	//.........................................................................................................
	m_SubTab[ 0 ] = (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_None , m_pFrame->GetID() , 
					3, 54, 56, 21, false );
	m_SubTab[ 0 ]->SetClientImage( TID_C_WND_CLOG_SUB_UP_BTN );
	m_SubTab[ 0 ]->SetBtnImage( TID_C_WND_CLOG_SUB_UP_BTN , TID_C_WND_CLOG_SUB_ON_BTN , TID_C_WND_CLOG_SUB_ON_BTN , TID_None );
	m_SubTab[ 0 ]->SetDirectCheck( TRUE );

	m_SubTab[ 1 ] = (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_None , m_pFrame->GetID() , 
					58, 54, 56, 21, false );
	m_SubTab[ 1 ]->SetClientImage( TID_C_WND_CLOG_SUB_UP_BTN );
	m_SubTab[ 1 ]->SetBtnImage( TID_C_WND_CLOG_SUB_UP_BTN , TID_C_WND_CLOG_SUB_ON_BTN , TID_C_WND_CLOG_SUB_ON_BTN , TID_None );
	m_SubTab[ 1 ]->SetDirectCheck( FALSE );
	 
	m_SubTab[ 2 ] = (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_None , m_pFrame->GetID() , 
					113, 54, 56, 21, false );
	m_SubTab[ 2 ]->SetClientImage( TID_C_WND_CLOG_SUB_UP_BTN );
	m_SubTab[ 2 ]->SetBtnImage( TID_C_WND_CLOG_SUB_UP_BTN , TID_C_WND_CLOG_SUB_ON_BTN , TID_C_WND_CLOG_SUB_ON_BTN , TID_None );
	m_SubTab[ 2 ]->SetDirectCheck( FALSE );

	nRui->SetGroupWnd( 3 , m_SubTab[ 0 ]->GetID() ,
						   m_SubTab[ 0 ]->GetID() ,
						   m_SubTab[ 1 ]->GetID() ,
						   m_SubTab[ 2 ]->GetID() );

	m_SubTabTextBox[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
					3, 54, 56, 21, false );
	m_SubTabTextBox[0]->SetFontR( n_fRsvFontWndStatic );
	m_SubTabTextBox[0]->SetAlignText( n_atCenter, n_atCenter );
	m_SubTabTextBox[0]->SetText(_RT( "") );
	m_SubTabTextBox[0]->SetTextColor( 0 , 0 , 0 );
	m_SubTabTextBox[0]->SetVisible( TRUE );

	m_SubTabTextBox[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
					58, 54, 56, 21, false );
	m_SubTabTextBox[1]->SetFontR( n_fRsvFontWndStatic );
	m_SubTabTextBox[1]->SetAlignText( n_atCenter, n_atCenter );
	m_SubTabTextBox[1]->SetText(_RT("") );
	m_SubTabTextBox[1]->SetTextColor( 0 , 0 , 0 );
	m_SubTabTextBox[1]->SetVisible( FALSE );

	m_SubTabTextBox[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
					113, 54, 56, 21, false );
	m_SubTabTextBox[2]->SetFontR( n_fRsvFontWndStatic );
	m_SubTabTextBox[2]->SetAlignText( n_atCenter, n_atCenter );
	m_SubTabTextBox[2]->SetText(_RT("") );
	m_SubTabTextBox[2]->SetTextColor( 0 , 0 , 0 );
	m_SubTabTextBox[2]->SetVisible( FALSE );

	//.........................................................................................................
	// 레벨 버튼
	//.........................................................................................................
	m_pLevelImage = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
					3, 485, 19, 28, false );
	m_pLevelImage->SetFontR( n_fRsvFontWndStatic );
	m_pLevelImage->SetAlignText( n_atCenter , n_atCenter );
	m_pLevelImage->SetText( "LV" );	
	m_pLevelImage->SetTextColor( 0 , 0 , 0 );
	m_pLevelImage->SetClientImage( TID_None );
	
	m_pLevelBtn[0]= (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_None , m_pFrame->GetID() ,
					24, 485, 39, 28, false );
	m_pLevelBtn[0]->SetClientImage( TID_C_WND_CLOG_LV_UP_BTN );
	m_pLevelBtn[0]->SetBtnImage( TID_C_WND_CLOG_LV_UP_BTN , TID_C_WND_CLOG_LV_ON_BTN , TID_C_WND_CLOG_LV_DN_BTN , TID_None );
	m_pLevelBtn[0]->SetDirectCheck( TRUE );

	m_pLevelBtn[1]= (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_None , m_pFrame->GetID() ,
					65, 485, 39, 28, false );
	m_pLevelBtn[1]->SetClientImage( TID_C_WND_CLOG_LV_UP_BTN );
	m_pLevelBtn[1]->SetBtnImage( TID_C_WND_CLOG_LV_UP_BTN , TID_C_WND_CLOG_LV_ON_BTN , TID_C_WND_CLOG_LV_DN_BTN , TID_None );
	m_pLevelBtn[1]->SetDirectCheck( FALSE );

	m_pLevelBtn[2]= (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_None , m_pFrame->GetID() , 
					106, 485, 39, 28, false );
	m_pLevelBtn[2]->SetClientImage( TID_C_WND_CLOG_LV_UP_BTN );
	m_pLevelBtn[2]->SetBtnImage( TID_C_WND_CLOG_LV_UP_BTN , TID_C_WND_CLOG_LV_ON_BTN , TID_C_WND_CLOG_LV_DN_BTN , TID_None );
	m_pLevelBtn[2]->SetDirectCheck( FALSE );

	m_pLevelBtn[3]= (CRadioButton *)nRui->CreateWnd( n_wtRadioButton, WID_None , m_pFrame->GetID() , 
					147, 485, 39, 28, false );
	m_pLevelBtn[3]->SetClientImage( TID_C_WND_CLOG_LV_UP_BTN );
	m_pLevelBtn[3]->SetBtnImage( TID_C_WND_CLOG_LV_UP_BTN , TID_C_WND_CLOG_LV_ON_BTN , TID_C_WND_CLOG_LV_DN_BTN , TID_None );
	m_pLevelBtn[3]->SetDirectCheck( FALSE );

	m_pLevelTextBox[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
					24, 485, 39, 28, false );
	m_pLevelTextBox[0]->SetFontR( n_fRsvFontWndStatic );
	m_pLevelTextBox[0]->SetAlignText( n_atCenter, n_atCenter );
	m_pLevelTextBox[0]->SetText( _RT("1~2") ); 
	m_pLevelTextBox[0]->SetTextColor( 0 , 0 , 0 );
	m_pLevelTextBox[0]->SetVisible( FALSE );

	m_pLevelTextBox[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
					65, 485, 39, 28, false );
	m_pLevelTextBox[1]->SetFontR( n_fRsvFontWndStatic );
	m_pLevelTextBox[1]->SetAlignText( n_atCenter, n_atCenter );
	m_pLevelTextBox[1]->SetText( _RT("3") ); 
	m_pLevelTextBox[1]->SetTextColor( 0 , 0 , 0 );
	m_pLevelTextBox[1]->SetVisible( FALSE );

	m_pLevelTextBox[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
					106, 485, 39, 28, false );
	m_pLevelTextBox[2]->SetFontR( n_fRsvFontWndStatic );
	m_pLevelTextBox[2]->SetAlignText( n_atCenter, n_atCenter );
	m_pLevelTextBox[2]->SetText( _RT("4") ); 
	m_pLevelTextBox[2]->SetTextColor( 0 , 0 , 0 );
	m_pLevelTextBox[2]->SetVisible( FALSE );

	m_pLevelTextBox[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
					147, 485, 39, 28, false );
	m_pLevelTextBox[3]->SetFontR( n_fRsvFontWndStatic );
	m_pLevelTextBox[3]->SetAlignText( n_atCenter, n_atCenter );
	m_pLevelTextBox[3]->SetText(_RT("5") ); 
	m_pLevelTextBox[3]->SetTextColor( 0 , 0 , 0 );
	m_pLevelTextBox[3]->SetVisible( FALSE );

	nRui->SetGroupWnd( 4 , m_pLevelBtn[ 0 ]->GetID() ,
						   m_pLevelBtn[ 0 ]->GetID() ,
						   m_pLevelBtn[ 1 ]->GetID() ,
						   m_pLevelBtn[ 2 ]->GetID() , 
						   m_pLevelBtn[ 3 ]->GetID());

	//.........................................................................................................
	// 콤보리스트 내용
	//.........................................................................................................

	for(short i = 0 ; i < COMBO_LIST_MAXCOUNT ; ++i )
	{
		TComboContext[i][0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
			24, 82 + ( i * 20 ) , 89, 10, false );
		TComboContext[i][0]->SetFontR( n_fRsvFontWndStatic );
		TComboContext[i][0]->SetAlignText( n_atLeft, n_atCenter );
		TComboContext[i][0]->SetText(_RT("") );
		TComboContext[i][0]->SetTextColor( 255 , 255 , 255 );
	
#ifdef APPLY_BOLD_FONT
		TComboContext[i][1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
			139, 82 + ( i * 20 ) , 89, 11, false );
#else
		TComboContext[i][1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
			129, 82 + ( i * 20 ) , 89, 11, false );
#endif	
		
		TComboContext[i][1]->SetFontR( n_fRsvFontWndStatic );
		TComboContext[i][1]->SetAlignText( n_atCenter, n_atCenter );
		TComboContext[i][1]->SetText(_RT("") );
		TComboContext[i][1]->SetTextColor( 255 , 255 , 255 );	
	}

	
	//.........................................................................................................
	// 서브 텍스트 박스
	//.........................................................................................................

	for(short i = 0 ; i < SULOG_LIST_MAXCOUNT ; ++i )
	{
		TSubContext[i][0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
						6,  82 + (i * 96), 228, 11, false );
		TSubContext[i][0]->SetFontR( n_fRsvFontWndStatic );
		TSubContext[i][0]->SetAlignText( n_atLeft, n_atCenter );
		TSubContext[i][0]->SetText(_RT("") );
		TSubContext[i][0]->SetTextColor( 0 , 0 , 0 );

		TSubContext[i][1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
						30,  96 + (i * 96), 204, 11, false );
		TSubContext[i][1]->SetFontR( n_fRsvFontWndStatic );
		TSubContext[i][1]->SetAlignText( n_atLeft, n_atCenter );
		TSubContext[i][1]->SetText(_RT("") );
		TSubContext[i][1]->SetTextColor( 0 , 0 , 0 );

		TSubContext[i][2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 
						30, 110 + (i * 96), 204, 11, false );
		TSubContext[i][2]->SetFontR( n_fRsvFontWndStatic );
		TSubContext[i][2]->SetAlignText( n_atLeft, n_atCenter );
		TSubContext[i][2]->SetText(_RT("") );
		TSubContext[i][2]->SetTextColor( 0 , 0 , 0 );

		TSubContext[i][3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(),
						30, 124 + (i * 96), 204, 11, false );
		TSubContext[i][3]->SetFontR( n_fRsvFontWndStatic );
		TSubContext[i][3]->SetAlignText( n_atLeft, n_atCenter );
		TSubContext[i][3]->SetText(_RT("") );
		TSubContext[i][3]->SetTextColor( 0 , 0 , 0 );
		
		TSubContext[i][4] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(),
						30, 138 + (i * 96), 204, 11, false );
		TSubContext[i][4]->SetFontR( n_fRsvFontWndStatic );
		TSubContext[i][4]->SetAlignText( n_atLeft, n_atCenter );
		TSubContext[i][4]->SetText(_RT("") );
		TSubContext[i][4]->SetTextColor( 0 , 0 , 0 );
		
		TSubContext[i][5] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(),
						30, 152 + (i * 96), 204, 11, false );
		TSubContext[i][5]->SetFontR( n_fRsvFontWndStatic );
		TSubContext[i][5]->SetAlignText( n_atLeft, n_atCenter );
		TSubContext[i][5]->SetText(_RT("") );
		TSubContext[i][5]->SetTextColor( 0 , 0 , 0 );

		TSubContext[i][6] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(),
						30, 164 + (i * 96), 204, 11, false );
		TSubContext[i][6]->SetFontR( n_fRsvFontWndStatic );
		TSubContext[i][6]->SetAlignText( n_atLeft, n_atCenter );
		TSubContext[i][6]->SetText(_RT("") );
		TSubContext[i][6]->SetTextColor( 0 , 0 , 0 );
		
		TSubContext[i][0]->SetVisible( FALSE );
		TSubContext[i][1]->SetVisible( FALSE );
		TSubContext[i][2]->SetVisible( FALSE );
		TSubContext[i][3]->SetVisible( FALSE );
		TSubContext[i][4]->SetVisible( FALSE );
		TSubContext[i][5]->SetVisible( FALSE );
		TSubContext[i][6]->SetVisible( FALSE );
	}
	
	//.........................................................................................................
	
	ComboControl();
}

void CLogWnd::SetWheelValue( short _wheel )
{
	int value = 0;
	int Max   = 0;
	//.........................................................................................................
	// 콤보 탭 일때
	//.........................................................................................................
	if( m_DisplayType == CLogWnd::COMBO_TAB )
	{
		Max   = m_pComboScroll->GetSldMax();
		m_pComboScroll->GetValue( &value );

		if( ( _wheel < 0 ) && ( value < Max ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}

		m_pComboScroll->SetValue( value );

		m_ComboTopNum = value;
	}
	else
	{
		Max   = m_pSubJobScroll->GetSldMax();
		m_pSubJobScroll->GetValue( &value );

		if( ( _wheel < 0 ) && ( value < Max ) )
		{
			value += 1;
		}
		else if( ( _wheel > 0 ) && ( value > 0 ) )
		{
			value -= 1;
		}

		m_pSubJobScroll->SetValue( value );

		m_SubTopNum = value;
	}

}


void CLogWnd::Update()
{
	//.........................................................................................................
	// 콤보 탭 일때
	//.........................................................................................................
	if( m_DisplayType == CLogWnd::COMBO_TAB && m_bEnableCombo )
	{
		char TempStr[ 1024 ] = "";
		
		int Num = 0;
		
		m_pComboSelectImg->SetVisible( FALSE );
		m_pComboRegImg->SetVisible( FALSE );

		for( int i = 0 ; i < COMBO_LIST_MAXCOUNT ; ++i )
		{
			TComboContext[ i ][0]->SetVisible( FALSE );
			TComboContext[ i ][1]->SetVisible( FALSE );

			Num = m_ComboTopNum + i;
			
			if( Num >= m_ComboTextList.size() )
				continue;
			
			if( Num == m_ComboSelectNum )
			{
				m_pComboSelectImg->SetWndRepositionR( 2 , 77 + ( i * 20 ) );
				m_pComboSelectImg->SetVisible( true );
			}
			
			//..............................................................................................
			// 등록된 콤보 표시
			//..............................................................................................
			if(  m_ComboTextList[ Num ].Index == g_Pc.theSkillSystem.theShortCut_SkillCode[2] )
			{
				m_pComboRegImg->SetWndRepositionR( 10 , 81 + ( i * 20 ) );
				m_pComboRegImg->SetVisible( true );
			}
			//..............................................................................................

			
			TComboContext[ i ][0]->SetVisible( TRUE );
			TComboContext[ i ][1]->SetVisible( TRUE );
		
			Rsprintf( RWCHAR(TempStr ) , _RT("%s"), m_ComboTextList[ Num ].StrSequence.c_str() );
			TComboContext[ i ][0]->SetText( TempStr );

			Rsprintf( RWCHAR(TempStr ) , _RT("%s") , m_ComboTextList[ Num ].StrDamage.c_str());
			TComboContext[ i ][1]->SetText( TempStr );
		}
	}
	//.........................................................................................................
	// 서브직업 탭일때
	//.........................................................................................................
	else if(m_DisplayType == CLogWnd::SUBCLASS_TAB)
	{
		m_pComboSelectImg->SetVisible( FALSE );
		m_pComboRegImg->SetVisible( FALSE );

		char TempStr[ 1024 ] = "";
		
		int Num = 0;

		for(int i = 0 ; i < SULOG_LIST_MAXCOUNT ; ++i )
		{
			TSubContext[i][0]->SetVisible( FALSE );
			TSubContext[i][1]->SetVisible( FALSE );
			TSubContext[i][2]->SetVisible( FALSE );
			TSubContext[i][3]->SetVisible( FALSE );
			TSubContext[i][4]->SetVisible( FALSE );
			TSubContext[i][5]->SetVisible( FALSE );
			TSubContext[i][6]->SetVisible( FALSE );
		}

		for(int i = 0 ; i < SULOG_LIST_MAXCOUNT ; ++i )
		{
			Num = m_SubTopNum + i;

			if( Num >= m_SubSortList.size() )
				break;

			for( int x = 0 ; x < 7 ; ++x )
			{
#ifdef UNICODE_ROCK
				if( wcslen( m_SubLogData[ m_SubSortList[ Num ] ].StrLine[ x ].c_str() ) )
#else
				if( strlen( m_SubLogData[ m_SubSortList[ Num ] ].StrLine[ x ].c_str() ) )
#endif
	
				{
					Rsprintf( RWCHAR(TempStr ), _RT("%s"), m_SubLogData[ m_SubSortList[ Num ] ].StrLine[ x ].c_str() );
					TSubContext[ i ][ x ]->SetText( TempStr );
					TSubContext[ i ][ x ]->SetVisible( TRUE );
				}
			}
		}
	}
}

void CLogWnd::Proc( SEventMessage* EMsg )
{
	//.........................................................................................................	
	// 콤보 탭
	//.........................................................................................................	
	if( EMsg->FocusWnd == m_pComboTab->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ComboControl();
		}
	}
	//.........................................................................................................	
	// 서브 직업 탭
	//.........................................................................................................	
	else if( EMsg->FocusWnd == m_pSubClassTab->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SubJobControl();
		}
	}
	//.........................................................................................................	
	// 콤보 스크롤
	//.........................................................................................................	
	else if( EMsg->FocusWnd == m_pComboScroll->GetID() ) 
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			m_ComboTopNum = EMsg->fParam;
		}
	}
	//.........................................................................................................	
	// 서브 직업 스크롤
	//.........................................................................................................	
	else if( EMsg->FocusWnd == m_pSubJobScroll->GetID() ) 
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			m_SubTopNum = EMsg->fParam;
		}
	}
	else if( EMsg->FocusWnd == m_pComboRegBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ComboKeyReg();
		}
	}
	else if( EMsg->FocusWnd == m_pComboRegCancelBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ComboKeyCancel();
		}
	}
	else if( EMsg->FocusWnd == m_pOKBtn->GetID() ||
			 EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_pFrame->SetVisible( FALSE );
			this->IsVisible = FALSE;
		}
	}
	//.........................................................................................................	
	// 서브 레벨 라디오 버튼
	//.........................................................................................................	
	else if( EMsg->FocusWnd == m_pLevelBtn[0]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pLevelBtn[0]->GetID() , n_emSetChecked, 0, 0, 0, 0 );

			SetJobLevel( 0 );
		}
	}
	else if( EMsg->FocusWnd == m_pLevelBtn[1]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pLevelBtn[1]->GetID() , n_emSetChecked, 0, 0, 0, 0 );

			SetJobLevel( 1 );
		}
	}
	else if( EMsg->FocusWnd == m_pLevelBtn[2]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pLevelBtn[2]->GetID() , n_emSetChecked, 0, 0, 0, 0 );

			SetJobLevel( 2 );
		}
	}
	else if( EMsg->FocusWnd == m_pLevelBtn[3]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_pLevelBtn[3]->GetID() , n_emSetChecked, 0, 0, 0, 0 );

			SetJobLevel( 3 );
		}
	}
	//.........................................................................................................	
	// 서브 타입별 라디오 버튼
	//.........................................................................................................	
	else if( EMsg->FocusWnd == m_SubTab[0]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_SubTab[0]->GetID() , n_emSetChecked, 0, 0, 0, 0 );

			SetJobType( 0 );
		}
	}
	else if( EMsg->FocusWnd == m_SubTab[1]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_SubTab[1]->GetID() , n_emSetChecked, 0, 0, 0, 0 );

			SetJobType( 1 );
		}
	}
	else if( EMsg->FocusWnd == m_SubTab[2]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( m_SubTab[2]->GetID() , n_emSetChecked, 0, 0, 0, 0 );

			SetJobType( 2 );
		}
	}
	
	for( int i = 0 ; i < COMBO_LIST_MAXCOUNT ; i++ )
	{
		if( EMsg->FocusWnd == m_pComboBtnList[ i ]->GetID() )
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				ComboSelect( i );
				break;
			}
		}	
	}

}

void CLogWnd::SetJobLevel(int Num)
{
	m_SubLevel = Num;

	SetSubSort();
}

void CLogWnd::SetJobType(int Num)
{
	m_SubTabNum = Num;

	SetSubSort();
}

void CLogWnd::Open(void)
{
	m_pFrame->SetVisible( TRUE );
	IsVisible = TRUE;
}

void CLogWnd::ComboSelect(int Num)
{
	int ComboSelect = m_ComboTopNum + Num;		

	if( ComboSelect >= m_ComboTextList.size() )
		return;
		
	m_ComboSelectNum = ComboSelect;
}

void CLogWnd::ClearLog(void)
{
	m_ComboTextList.clear();
	m_SubLogData.clear();
	m_SubSortList.clear();

	m_SubTabTextBox[0]->SetText(_RT("") ); 
	m_SubTabType[0] = 0;
	m_SubTabTextBox[1]->SetText(_RT("") ); 
	m_SubTabType[1] = 0;
	m_SubTabTextBox[2]->SetText(_RT("") ); 
	m_SubTabType[2] = 0;

	ComboControl();
}

void CLogWnd::InitComboTextList(void)
{
		
	char buff[2] = "";
	char TempBuf[512] = "";
	
	ClearLog();

	InitSubtextList();
	
	m_ComboTextList.clear();
	
	// 무기 착용 스킬에 따른 콤보 타입 설정 
	int CurtComboType = nComboType_OneHand_Sword;
	CurtComboType = nRui->m_Combo2DTimingEffect.GetCurtComboTypeSecond();	
	if( CurtComboType == -1 ) 														// 무기 없음 (맨손 공격)
	{
		return;
	}
	
	int LevelCount = 5;
	
	if( LevelCount != -1 )
	{
		LevelCount += 3;

		for( int i = 1 ; i < 21 ; ++i )
		{
			if( GetSavedLog( i ) != 0 )
			{
				COMBO_LOG_DATA TempData;
				int			   ComboKey;

				int nCount = 0;
#ifdef UNICODE_ROCK
	wstring sSequence;
#else
	string sSequence;
#endif
	
				
				for( int n = 0 ; n < 8 ; ++n )
				{	
					ComboKey = g_Pc.GetPlayer()->theActionSystem.theCombo_Info[ CurtComboType ].combo_sequence[ i - 1 ][ n ];				
					
					if( n >= LevelCount )
						break;

					if( ComboKey == 0 )
					{
						break;
					}
					else
					{					
						switch( ComboKey / 10 )
						{
						case 1:
							if( n == 0) 
							{
								buff[0] = 'Z';
								sSequence += 'Z';
							}
							else if( n < 4 )
							{
								sSequence += _RT("-Z");
							}
							break;
						
						case 2:
							if( n == 0) 
							{
								buff[0] = 'X';
								sSequence += 'X';
							}
							else if( n < 4 )
							{
								sSequence += _RT("-X");
							}
							break;
						
						case 3:
							if( n == 0) 
							{
								buff[0] = 'C';
								sSequence += 'C';
							}
							else if( n < 4 )
							{
								sSequence += _RT("-C");
							}
							break;
						}

						nCount++;
					}
				}

				if(nCount == 4)
				{
					TempData.StrSequence = sSequence;
				}
				else if( nCount > 4 )
				{
					nCount -= 3;
					Rsprintf( RWCHAR(TempBuf), _RT("%s + Rnd %d"), sSequence.c_str() , nCount - 1);			
					TempData.StrSequence = RWCHAR(TempBuf);

				}
			
				if( CurtComboType >= m_ComboBonusList.size() )
					continue;
			
				float fBonus    = m_ComboBonusList[ CurtComboType ].Info[ i - 1 ].Bonus;				
				WORD  SkillCode = m_ComboBonusList[ CurtComboType ].Info[ i - 1 ].SkillCode;
				
				SSkillBaseInfo* skill_Info;
				
				skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( SkillCode ); 
				
				if( SkillCode != 0 )
				{
					if( skill_Info == NULL )
						continue;
					
					if( fBonus > 1.0f )
					{
						char StrTemp[ 1024 ] = {0,};
						Rsprintf( RWCHAR(StrTemp) , G_STRING_CHAR( IDS_WND_LOG_COMBO_OP ) , RWCHAR(skill_Info->theSkillName) , fBonus );	
						TempData.StrDamage = RWCHAR(StrTemp);
					}
					else
					{
						TempData.StrDamage = RWCHAR(skill_Info->theSkillName);
					}
				}
				
				TempData.Index = i;
//				TempData.Index = g_Pc.GetPlayer()->theActionSystem.theCombo_Info[ CurtComboType ].combo_index[ i - 1 ];				
				m_ComboTextList.push_back( TempData );
			}
		}
	}

	int ScrollMax = m_ComboTextList.size() - COMBO_LIST_MAXCOUNT;
	
	if( ScrollMax <= 0 )
	{
		m_pComboScroll->ReSetScroll( 0 , 0 , 0 );
		m_ComboTopNum = 0;
		m_ComboSelectNum = -1;
	}
	else
	{
		m_pComboScroll->ReSetScroll( 0 , ScrollMax , 0 );
		m_ComboTopNum = 0;
		m_ComboSelectNum = -1;
	}
}


void CLogWnd::LoadData(void)
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	char FileName[ MAX_PATH ] = "";

	wsprintf( FileName , "%s\\chr\\%s" , g_RockClient.GetRootDir() , "combo.dat" );

	FILE *fp = NULL;

	fp = fopen( FileName , "rb+" );

	if( fp == NULL )
	{
		RLGS( "Error CLogWnd::LoadData\n" );
	
		return;
	}

	COMBO_BONUS_INFO TempData;
	COMBO_BONUS_DATA Temp;

	do 
	{
		Size = fread( &Temp.Index , sizeof( Temp.Index ) , 1 , fp );
			
		if( Size <= 0 )
			break;
		
		Size = fread( &Temp.Bonus     , sizeof( Temp.Bonus ) , 1 , fp );
		Size = fread( &Temp.SkillCode , sizeof( Temp.SkillCode ) , 1 , fp );

		memcpy( &TempData.Info[ Count ] , &Temp , sizeof( Temp ) , 1 , fp );

		Count++;

		if( Count >= 20 )
		{
			m_ComboBonusList.push_back( TempData );
		
			Count = 0;
		}

	}while( TRUE );
	
	fclose( fp );


///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( "combo.dat" );

	if( pFH == NULL ) 
	{
		RLGS( "Error CLogWnd::LoadData\n" );
	
		return;
	}
	
	int  Size  = 0;
	int  Count = 0;

	COMBO_BONUS_INFO TempData;
	COMBO_BONUS_DATA Temp;

	do 
	{
		Size = pFH->Read( &Temp.Index , sizeof( Temp.Index ) );
			
		if( Size <= 0 )
			break;		

		int nTemp = 0;
		pFH->Read( &nTemp     , sizeof( nTemp ) );
		pFH->Read( &nTemp     , sizeof( nTemp ) );
		pFH->Read( &nTemp     , sizeof( nTemp ) );
		pFH->Read( &nTemp     , sizeof( nTemp ) );
		pFH->Read( &nTemp     , sizeof( nTemp ) );
		pFH->Read( &nTemp     , sizeof( nTemp ) );
		pFH->Read( &nTemp     , sizeof( nTemp ) );
		pFH->Read( &nTemp     , sizeof( nTemp ) );
		Temp.Bonus = 0;
		Size = pFH->Read( &Temp.SkillCode , sizeof( Temp.SkillCode ) );

		memcpy( &TempData.Info[ Count ] , &Temp , sizeof( Temp ) );

		Count++;

		if( Count >= 20 )
		{
			m_ComboBonusList.push_back( TempData );		
			Count = 0;
		}

	} while( TRUE );

	g_VFChrRoot.CloseFile( pFH );
	
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
}

void CLogWnd::ResetLog(char *pData)
{
	memcpy( &m_theSaveLog , pData , 70 );
}

BYTE CLogWnd::GetSavedLog(int aSlot)
{
	if( aSlot/8 >= sizeof( m_theSaveLog ) )
	{
		return 0;
	}

	BYTE temp = 1;
	temp <<= aSlot%8;
	return( m_theSaveLog[aSlot/8] & temp );
}


void CLogWnd::SetSavedLog( int aSlot , BYTE ISOn )
{
	if( aSlot/8 >= sizeof( m_theSaveLog ) )
	{
		return;
	}

	if( ISOn )
	{
		BYTE temp = 1;
		temp <<= aSlot%8;
		m_theSaveLog[aSlot/8] |= temp;
	}
	else
	{
		BYTE temp = 1;
		temp <<= (aSlot%8);
		m_theSaveLog[aSlot/8] ^= temp;
	}
}


void CLogWnd::ComboControl(void)
{
	nRui->SendEvent( m_pComboTab->GetID() , n_emSetChecked, 0, 0, 0, 0 );
			
	m_DisplayType = CLogWnd::COMBO_TAB;
	
	m_pComboScroll->SetVisible( TRUE );
	m_pSubJobScroll->SetVisible( FALSE );

	int i = 0;

	if(m_bEnableCombo)
	{
		TComboLabel0->SetVisible( TRUE );
		TComboLabel1->SetVisible( TRUE );

		for( i = 0 ; i < COMBO_LIST_MAXCOUNT ; ++i )
		{
			TComboContext[i][0]->SetVisible( TRUE );
			TComboContext[i][1]->SetVisible( TRUE );
		}
		
		for( i = 0 ; i < COMBO_LIST_MAXCOUNT ; ++i )
		{
			m_pComboBtnList[ i ]->SetVisible( TRUE );
		}
	}	

	m_pComboSelectImg->SetVisible( FALSE );
	
	m_pComboRegBtn->SetVisible( TRUE );
	m_pComboRegCancelBtn->SetVisible( TRUE );

	for( i = 0 ; i < 3 ; ++i )
	{
		m_SubTab[ i ]->SetVisible( FALSE );
		m_SubTabTextBox[i]->SetVisible( FALSE );
	}

	for( i = 0 ; i < 4 ; ++i )
	{
		m_pLevelBtn[i]->SetVisible( FALSE );
		m_pLevelTextBox[i] ->SetVisible( FALSE );
	}

	m_pLevelImage->SetVisible( FALSE );

	for( i = 0 ; i < SULOG_LIST_MAXCOUNT ; ++i )
	{
		TSubContext[i][0]->SetVisible( FALSE );
		TSubContext[i][1]->SetVisible( FALSE );
		TSubContext[i][2]->SetVisible( FALSE );
		TSubContext[i][3]->SetVisible( FALSE );
		TSubContext[i][4]->SetVisible( FALSE );
		TSubContext[i][5]->SetVisible( FALSE );
		TSubContext[i][6]->SetVisible( FALSE );
	}

	m_pLineImg[0]->SetVisible( FALSE );
	m_pLineImg[1]->SetVisible( FALSE );
	m_img9->SetVisible( true );

}

void CLogWnd::SubJobControl(void)
{
	nRui->SendEvent( m_pSubClassTab->GetID() , n_emSetChecked, 0, 0, 0, 0 );
	m_DisplayType = CLogWnd::SUBCLASS_TAB;
	
	m_pComboScroll->SetVisible( FALSE );
	m_pSubJobScroll->SetVisible( TRUE );

	TComboLabel0->SetVisible( FALSE );
	TComboLabel1->SetVisible( FALSE );

	for(short i = 0 ; i < COMBO_LIST_MAXCOUNT ; ++i )
	{
		TComboContext[i][0]->SetVisible( FALSE );
		TComboContext[i][1]->SetVisible( FALSE );
	}

	for(short i = 0 ; i < COMBO_LIST_MAXCOUNT ; ++i )
	{
		m_pComboBtnList[ i ]->SetVisible( FALSE );
	}

	m_pComboSelectImg->SetVisible( FALSE );

	m_pComboRegBtn->SetVisible( FALSE );
	m_pComboRegCancelBtn->SetVisible( FALSE );

	for(short i = 0 ; i < 4 ; ++i )
	{
		m_pLevelBtn[i]->SetVisible( TRUE );
		m_pLevelTextBox[i] ->SetVisible( TRUE );
	}

	m_pLevelImage->SetVisible( TRUE );

	for(short i = 0 ; i < SULOG_LIST_MAXCOUNT ; ++i )
	{
		TSubContext[i][0]->SetVisible( TRUE );
		TSubContext[i][1]->SetVisible( TRUE );
		TSubContext[i][2]->SetVisible( TRUE );
		TSubContext[i][3]->SetVisible( TRUE );
		TSubContext[i][4]->SetVisible( TRUE );
		TSubContext[i][5]->SetVisible( TRUE );
		TSubContext[i][6]->SetVisible( TRUE );
	}

	switch( nRui->thePcParam.SubClass )	
	{
		// 웨폰 크래프트
		case 	ArmCraft	: 
		// 아머 크래프트	
		case 	ArmorCraft	:
		
			m_SubTab[ 0 ]->SetVisible( TRUE );
			m_SubTab[ 1 ]->SetVisible( TRUE );
			m_SubTab[ 2 ]->SetVisible( TRUE );
			
			m_SubTabTextBox[ 0 ]->SetVisible( TRUE );
			m_SubTabTextBox[ 1 ]->SetVisible( TRUE );
			m_SubTabTextBox[ 2 ]->SetVisible( TRUE );

			break;

		// 엘다 크래프트
		case 	EldaCraft	:
			m_SubTab[ 0 ]->SetVisible( TRUE  );
			m_SubTab[ 1 ]->SetVisible( FALSE );
			m_SubTab[ 2 ]->SetVisible( FALSE );

			m_SubTabTextBox[ 0 ]->SetVisible( TRUE  );
			m_SubTabTextBox[ 1 ]->SetVisible( FALSE );
			m_SubTabTextBox[ 2 ]->SetVisible( FALSE );

			break;

		case	NoSubClass	:
		case 	Artisan		:
		case 	Merchant	:
		case 	Leader		:
		case 	Negotiator	:
		case 	Cook		:
		case 	Scientist	:
			m_SubTab[ 0 ]->SetVisible( FALSE );
			m_SubTab[ 1 ]->SetVisible( FALSE );
			m_SubTab[ 2 ]->SetVisible( FALSE );
			
			m_SubTabTextBox[ 0 ]->SetVisible( FALSE );
			m_SubTabTextBox[ 1 ]->SetVisible( FALSE );
			m_SubTabTextBox[ 2 ]->SetVisible( FALSE );
			break;
	}

	m_pLineImg[0]->SetVisible( TRUE );
	m_pLineImg[1]->SetVisible( TRUE );
	m_img9->SetVisible( FALSE );

	SetSubTabType();
}

void CLogWnd::ComboKeyReg(void)
{
	if( m_ComboSelectNum == -1 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NOTEXIST_SELCOMBO ) , 1000 , NULL );	
	
		return;
	}	

	int nComTotCount = GetComboTotCount( m_ComboTextList[ m_ComboSelectNum ].Index );
	int nSkillLevel = g_Pc.GetPlayer()->theActionSystem.GetEnableSkillLevel();

	TCHAR strString[512] = {0,};
	TCHAR strSkill[512] = {0,};	
	if( nComTotCount > (nSkillLevel + 3) )
	{				
		nRui->m_Combo2DTimingEffect.GetNeedSkill(strSkill);
		
		if( strcmp(strSkill,"") !=0 )		
		{
			int nNeedLevel = nComTotCount - 3;
			Rsprintf( RWCHAR(strString), _RT("%s-[%s %d]"), G_STRING_CHAR( IDS_SYSPR_SHORT_COMBOLEVEL ), RWCHAR(strSkill), nNeedLevel);
		}
		else
		{
			Rsprintf( RWCHAR(strString), _RT("%s"), G_STRING_CHAR( IDS_SYSPR_SHORT_COMBOLEVEL ));
		}
			
		MessageBoxWnd.Open( strString , 1000 , NULL );		
		return;
	}
	
	if( g_Pc.theSkillSystem.theShortCut_SkillCode[2] == m_ComboTextList[ m_ComboSelectNum ].Index )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_ALREADY_REG ) , 1000 , NULL );	

		return;
	}

	g_RockClient.Send_RpCsHotKey( 2 , ( WORD )m_ComboTextList[ m_ComboSelectNum ].Index );

	SetComboHotKey( m_ComboTextList[ m_ComboSelectNum ].Index );

	MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_DONE_REG ) , 1000 , NULL );	
}

void CLogWnd::ComboKeyCancel(void)
{
	if( g_Pc.theSkillSystem.theShortCut_SkillCode[2] == 0 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NOTEXIST_REGCOMBO ) , 1000 , NULL );	
	
		return;
	}

	memset( g_Pc.theSkillSystem.theStrHotKey , 0 , sizeof( g_Pc.theSkillSystem.theStrHotKey ) );

	g_RockClient.Send_RpCsHotKey( 2 , 0 );
	SetComboRegKey(0);

	MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_CALCEL_COMBO ), 1000 , NULL );	
}

int  CLogWnd::GetComboTotCount(int Num)
{
	int nCount = 0;
	
	if( Num == 0 )
		return 0;

	// 무기 착용 스킬에 따른 콤보 타입 설정 
	int CurtComboType = nComboType_OneHand_Sword;
	CurtComboType = nRui->m_Combo2DTimingEffect.GetCurtComboTypeSecond();
	
	if(CurtComboType == -1)
	{
		return 0;
	}

	COMBO_LOG_DATA TempData;
	int			   ComboKey;

	for( int n = 0 ; n < 8 ; ++n )
	{	
		ComboKey = g_Pc.GetPlayer()->theActionSystem.theCombo_Info[ CurtComboType ].combo_sequence[ Num - 1 ][ n ];				
				
		if( ComboKey == 0 )
		{
			break;
		}
		else
		{
			nCount++;			
		}
	}

	return nCount;
}

void CLogWnd::SetComboHotKey(int Num)
{
	memset( g_Pc.theSkillSystem.theStrHotKey , 0 , sizeof( g_Pc.theSkillSystem.theStrHotKey ) );

	if( Num == 0 )
		return;

	// 무기 착용 스킬에 따른 콤보 타입 설정 
	int CurtComboType = nComboType_OneHand_Sword;
	CurtComboType = nRui->m_Combo2DTimingEffect.GetCurtComboTypeSecond();

	if(CurtComboType == -1)
	{
		return;
	}	

	COMBO_LOG_DATA TempData;
	int			   ComboKey;

	int nCount = 0;

	for( int n = 0 ; n < 8 ; ++n )
	{	
		ComboKey = g_Pc.GetPlayer()->theActionSystem.theCombo_Info[ CurtComboType ].combo_sequence[ Num - 1 ][ n ];				
				
		if( ComboKey == 0 )
		{
			break;
		}
		else
		{
			nCount++;
			
			switch( ComboKey / 10 )
			{
				case 1:
					g_Pc.theSkillSystem.theStrHotKey[ n ] = 'Z';
					break;
				case 2:
					g_Pc.theSkillSystem.theStrHotKey[ n ] = 'X';
					break;
				case 3:
					g_Pc.theSkillSystem.theStrHotKey[ n ] = 'C';
					break;
			}
		}
	}

	SetComboRegKey(Num, nCount);
}

int CLogWnd::IsAddTable(WORD Code)
{
	SItemBaseInfo *pItem = g_Pc_Manager.GetItemBaseInfoPtr( Code );

	if( pItem == NULL )
		return FALSE;

	switch( nRui->thePcParam.SubClass )	
	{
		// 웨폰 크래프트
		case 	ArmCraft	: 
			if( nRui->thePcParam.Citizen == MILLENA )
			{
				if( pItem->theType == n_Type_Sword		  ||
					pItem->theType == n_Type_TwohandSword ||
					pItem->theType == n_Type_Bow			)
				{
					return TRUE;
				}
			}
			else
			{
				if( pItem->theType == n_Type_MagicBook	 ||
					pItem->theType == n_Type_CrystalBall ||
					pItem->theType == n_Type_Staff			)
				{
					return TRUE;
				}
			}
			break;
		// 엘다 크래프트
		case 	EldaCraft	:
			break;
		// 아머 크래프트	
		case 	ArmorCraft	:
			if( nRui->thePcParam.Citizen == MILLENA )
			{
				if( pItem->theType == n_Type_LightCoat	     ||
					pItem->theType == n_Type_LightCoat + 1   ||
					pItem->theType == n_Type_LightCoat + 2   ||
					pItem->theType == n_Type_LightCoat + 3   ||
					pItem->theType == n_Type_MiddleCoat	 ||
					pItem->theType == n_Type_MiddleCoat + 1 ||
					pItem->theType == n_Type_MiddleCoat + 2 ||
					pItem->theType == n_Type_MiddleCoat + 3 ||
					pItem->theType == n_Type_HeavyCoat	+ 1		 ||
					pItem->theType == n_Type_HeavyCoat	+ 2		 ||
					pItem->theType == n_Type_HeavyCoat	+ 3		 ||
					pItem->theType == n_Type_HeavyCoat			||
					pItem->theType == n_Type_Shield )
				{
					return TRUE;
				}
			}
			else
			{
				if( pItem->theType == n_Type_MagicCoat	     ||
					pItem->theType == n_Type_MagicCoat + 1   ||
					pItem->theType == n_Type_MagicCoat + 2   ||
					pItem->theType == n_Type_MagicCoat + 3   ||

					pItem->theType == n_Type_SpiritCoat	 ||
					pItem->theType == n_Type_SpiritCoat + 1 ||
					pItem->theType == n_Type_SpiritCoat + 2 ||
					pItem->theType == n_Type_SpiritCoat + 3 ||
					
					pItem->theType == n_Type_PriestCoat	+ 1		 ||
					pItem->theType == n_Type_PriestCoat + 2		 ||
					pItem->theType == n_Type_PriestCoat	+ 3		 ||
					pItem->theType == n_Type_PriestCoat			)
				{
					return TRUE;
				}
			}
			break;


		case	NoSubClass	:
		case 	Artisan		:
		case 	Merchant	:
		case 	Leader		:
		case 	Negotiator	:
		case 	Cook		:
		case 	Scientist	:
			break;
	}

	return FALSE;
}

int	CLogWnd::IsAddTableType(int Type )
{
	
	switch( nRui->thePcParam.SubClass )	
	{
		// 웨폰 크래프트
		case 	ArmCraft	: 
			if( nRui->thePcParam.Citizen == MILLENA )
			{
				if( Type == n_Type_Sword		  ||
					Type == n_Type_TwohandSword ||
					Type == n_Type_Bow			)
				{
					return TRUE;
				}
			}
			else
			{
				if( Type == n_Type_MagicBook	 ||
					Type == n_Type_CrystalBall ||
					Type == n_Type_Staff			)
				{
					return TRUE;
				}
			}
			break;
		// 엘다 크래프트
		case 	EldaCraft	:
			break;
		// 아머 크래프트	
		case 	ArmorCraft	:
			if( nRui->thePcParam.Citizen == MILLENA )
			{
				if( Type == n_Type_LightCoat	     ||
					Type == n_Type_LightCoat + 1   ||
					Type == n_Type_LightCoat + 2   ||
					Type == n_Type_LightCoat + 3   ||
					Type == n_Type_MiddleCoat	 ||
					Type == n_Type_MiddleCoat + 1 ||
					Type == n_Type_MiddleCoat + 2 ||
					Type == n_Type_MiddleCoat + 3 ||
					Type == n_Type_HeavyCoat	+ 1		 ||
					Type == n_Type_HeavyCoat	+ 2		 ||
					Type == n_Type_HeavyCoat	+ 3		 ||
					Type == n_Type_HeavyCoat			||
					Type == n_Type_Shield )
				{
					return TRUE;
				}
			}
			else
			{
				if( Type == n_Type_MagicCoat	     ||
					Type == n_Type_MagicCoat + 1   ||
					Type == n_Type_MagicCoat + 2   ||
					Type == n_Type_MagicCoat + 3   ||

					Type == n_Type_SpiritCoat	 ||
					Type == n_Type_SpiritCoat + 1 ||
					Type == n_Type_SpiritCoat + 2 ||
					Type == n_Type_SpiritCoat + 3 ||
					
					Type == n_Type_PriestCoat	+ 1		 ||
					Type == n_Type_PriestCoat + 2		 ||
					Type == n_Type_PriestCoat	+ 3		 ||
					Type == n_Type_PriestCoat			)
				{
					return TRUE;
				}
			}
			break;


		case	NoSubClass	:
		case 	Artisan		:
		case 	Merchant	:
		case 	Leader		:
		case 	Negotiator	:
		case 	Cook		:
		case 	Scientist	:
			break;
	}

	return FALSE;
}

void CLogWnd::SetSubTabType(void)
{
	switch( nRui->thePcParam.SubClass )	
	{
		// 웨폰 크래프트
		case 	ArmCraft	: 
			if( nRui->thePcParam.Citizen == MILLENA )
			{
				m_SubTabTextBox[0]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_SWORD ) ); 
				m_SubTabType[0] = n_Type_Sword;
				m_SubTabTextBox[1]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_GSWORD )); 
				m_SubTabType[1] = n_Type_TwohandSword;
				m_SubTabTextBox[2]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_BOW ) ); 
				m_SubTabType[2] = n_Type_Bow;
			}
			else
			{
				m_SubTabTextBox[0]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_MBOOK ) ); 
				m_SubTabType[0] = n_Type_MagicBook;
				m_SubTabTextBox[1]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_CRYBALL ) ); 
				m_SubTabType[1] = n_Type_CrystalBall;
				m_SubTabTextBox[2]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_STAFF ) ); 
				m_SubTabType[2] = n_Type_Staff;
			}
			break;
		// 엘다 크래프트
		case 	EldaCraft	:
			m_SubTabTextBox[0]->SetText( G_STRING_CHAR( IDS_WND_LOG_ELDA_REMODEL )); 
				m_SubTabType[0]=0;
			break;
		// 아머 크래프트	
		case 	ArmorCraft	:
			if( nRui->thePcParam.Citizen == MILLENA )
			{
				m_SubTabTextBox[0]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_LARMOR ) ); 
				m_SubTabType[0]=n_Type_LightCoat;
				m_SubTabTextBox[1]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_MARMOR ) ); 
				m_SubTabType[1]=n_Type_MiddleCoat;
				m_SubTabTextBox[2]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_HARMOR ) ); 
				m_SubTabType[2]=n_Type_HeavyCoat;
			}
			else
			{
				m_SubTabTextBox[0]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_MAGIC_ARMOR ) ); 
				m_SubTabType[0]=n_Type_MagicCoat;
				m_SubTabTextBox[1]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_SPRIT_ARMOR ) ); 
				m_SubTabType[1]=n_Type_SpiritCoat;
				m_SubTabTextBox[2]->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_PRIEST_ARMOR ) ); 
				m_SubTabType[2]=n_Type_PriestCoat;
			}
			break;
			
		case	NoSubClass	:
		case 	Artisan		:
		case 	Merchant	:
		case 	Leader		:
		case 	Negotiator	:
		case 	Cook		:
		case 	Scientist	:
			break;
	}
}

int CLogWnd::GetEldaStoneLevel(int Value)
{
		
	RTCHAR Str[128] = {0,};

	Rsprintf( RWCHAR(Str) , _RT("%d") , Value );

	int Num = ( int ) ( Str[ Rstrlen( Str ) - 1 ] - 48 );

	if( Num == 0 )
		return 10;

	return Num;
}

void CLogWnd::InitSubtextList(void)
{
	m_SubLogData.clear();

	SetSubTabType();
	
	int Num = 0;
	//m_SubLogData
	for( int i = 21 ; i < sizeof( char ) * 8 * 70 ; ++i )
	{
		if( GetSavedLog( i ) != 0 )
		{
			Num = i - 20;

			///--  ( 무구 제조 테이블 참조 ) 
			for( int n = 0 ; n < g_ItemCreate_WndProc.GetTableCount() ; ++n )
			{				
				if( g_CreatelTable[ n ].theIndex == Num )
				{
						SUB_LOG_DATA TempData;

#ifdef ITEM_UPGRADE_2						
						if( !IsAddTableType( g_CreatelTable[ n ].theType ) )
									continue;

						TempData.StrLine[ 0 ] += RWCHAR(g_CreatelTable[ n ].theName);
						TempData.Type  = g_CreatelTable[ n ].theType;
						TempData.Level = g_CreatelTable[ n ].theGrade * 100;

#else
						if( !IsAddTable( g_CreatelTable[ n ].theResultItemCode ) )
									continue;

						SItemBaseInfo *pItem     = g_Pc_Manager.GetItemBaseInfoPtr( g_CreatelTable[ n ].theResultItemCode );
						SItemBaseInfo *pMakeItem = g_Pc_Manager.GetItemBaseInfoPtr( g_CreatelTable[ n ].theAddItem[ 0 ].theItemCode );

						if( pItem == NULL || pMakeItem == NULL )
							break;

						TempData.Type  = pItem->theType;
						TempData.Level = pItem->theGrade;			  //레벨

						TempData.StrLine[ 0 ] += RWCHAR(pItem->theName);
#endif											

//						TempData.StrLine[ 1 ] += "+ ";
//						TempData.StrLine[ 1 ] += pStoneItem->theName;

						int Count = 1; // 2;
						int nAddItemNum = 0;

#ifdef ITEM_UPGRADE_2
						nAddItemNum = 5;
#else
						nAddItemNum = 6;
#endif			

						for( int x = 0 ; x < nAddItemNum ; ++x )
						{
							if( g_CreatelTable[ n ].theAddItem[ x ].theItemCode != 0 )
							{
								SItemBaseInfo *pAddItem = g_Pc_Manager.GetItemBaseInfoPtr( g_CreatelTable[ n ].theAddItem[ x ].theItemCode );
							
								if( pAddItem != NULL )
								{
									char TempStr[ 1024 ] = "";

									Rsprintf(  RWCHAR(TempStr) , G_STRING_CHAR( IDS_WND_LOG_ELDA_OP ),
										RWCHAR(pAddItem->theName) ,
									    g_CreatelTable[ n ].theAddItem[ x ].theBundle );

									TempData.StrLine[ Count ] = RWCHAR(TempStr);

									Count++;
								}
							}
							
						}

						m_SubLogData.push_back( TempData );

						break;
				}
			}

			// 엘다 크래프트 일때 ( 제조 테이블 참조 ) - 엘다크래프트는 없어짐 
			/*( nRui->thePcParam.SubClass == EldaCraft )
			{
				for( int n = 0 ; n < g_ItemRemodel_WndProc.GetTableCount() ; ++n )
				{
					if( g_RemodelTable[ n ].theIndex == Num )
					{
						SItemBaseInfo *pItem      = g_Pc_Manager.GetItemBaseInfoPtr( g_RemodelTable[ n ].theStoneCode );
						SItemBaseInfo *pStoneItem = g_Pc_Manager.GetItemBaseInfoPtr( g_RemodelTable[ n ].theGemStoneCode );

						if( pItem == NULL || pStoneItem == NULL )
							break;

						SUB_LOG_DATA TempData;

						TempData.Type  = 0;
						TempData.Level = GetEldaStoneLevel( g_RemodelTable[ n ].theStoneCode );

						TempData.StrLine[ 0 ] += pItem->theName;

						TempData.StrLine[ 1 ] += "+ ";
						TempData.StrLine[ 1 ] += pStoneItem->theName;

						int Count = 2;

						for( int x = 0 ; x < 2 ; ++x )
						{
							if( g_RemodelTable[ n ].theAddItem[ x ].theItemCode != 0 )
							{
								SItemBaseInfo *pAddItem = g_Pc_Manager.GetItemBaseInfoPtr( g_RemodelTable[ n ].theAddItem[ x ].theItemCode );
							
								if( pAddItem != NULL )
								{
									char TempStr[ 1024 ] = "";

									wsprintf(  TempStr , G_STRING_CHAR( IDS_WND_LOG_ELDA_OP ) ,
										pAddItem->theName ,
									    g_RemodelTable[ n ].theAddItem[ x ].theBundle );

									TempData.StrLine[ Count ] = TempStr;

									Count++;
								}
							}
							
						}

						m_SubLogData.push_back( TempData );

						break;
					}
				}
			}
			// 그외일때
			else*/
			//
			//g_ItemCreate_WndProc.GetTableCount();
			//IsAddTable
		}
	}	
	
	SetSubSort();
}

int CLogWnd::SubSortLevel(int Level)
{
	Level = Level/100;
	
	switch( m_SubLevel )
	{
		case 0:
			if( 1 <= Level && Level <= 2 )
				return TRUE;
			break;

		case 1:
			if( 3 <= Level && Level <= 3 )
				return TRUE;
			break;

		case 2:
			if( 4 <= Level && Level <= 4 )
				return TRUE;
			break;
		case 3:
			if( 5 <= Level && Level <= 5 )
				return TRUE;
			break;
	}
	
	return FALSE;
}

int	CLogWnd::SubSortType(int Type)
{
	if( m_SubTabType[ m_SubTabNum ] < 10 )
	{
		if( m_SubTabType[ m_SubTabNum ] == Type )
			return TRUE;
	}
	else
	{
		if( Type < 10 )
		{
			return FALSE;
		}

		//by simwoosung
		if( n_Type_LightCoat == m_SubTabType[ m_SubTabNum ] )
		{
			if(n_Type_Shield == Type)
			{
				return TRUE;
			}
		}		
		
		int Compare0 = m_SubTabType[ m_SubTabNum ] / 10;
		int Compare1 = Type / 10;

		if( Compare0 == Compare1 )
			return TRUE;
	}
	
	return FALSE;
}


void CLogWnd::SetSubSort(void)
{
	m_SubSortList.clear();

	int Count = 0;

	std::vector <SUB_LOG_DATA>::iterator	i = m_SubLogData.begin();
	
	for( ; i != m_SubLogData.end() ; ++i , ++Count )
	{
		if( nRui->thePcParam.SubClass == EldaCraft )
		{
			// 레벨 체크
			if( !SubSortLevel( i->Level ) )
				continue;

			m_SubSortList.push_back( Count );
		}
		else
		{
			// 레벨 체크
			if( !SubSortLevel( i->Level ) )
				continue;

			// 타입 체크
			if( !SubSortType( i->Type ) )
				continue;

			m_SubSortList.push_back( Count );
		}
	}

	int ScrollMax = m_SubSortList.size() - 2;
	
	if( ScrollMax <= 0 )
	{
		m_pSubJobScroll->ReSetScroll( 0 , 0 , 0 );
		m_SubTopNum = 0;
	}
	else
	{
		m_pSubJobScroll->ReSetScroll( 0 , ScrollMax , 0 );
		m_SubTopNum = 0;
	}
}

COMBO_LOG_DATA *CLogWnd::GetComboLogData(int SlotNum)
{
	std::vector	<COMBO_LOG_DATA>::iterator i = m_ComboTextList.begin();
	
	for( ; i != m_ComboTextList.end() ; ++i )
	{
		if( i->Index == SlotNum )
			return &(*i);
	}

	return NULL;
}

int	 CLogWnd::GetComboLogSize()
{
	return (int)m_ComboTextList.size();
}

bool CLogWnd::CheckComboEnable( int aIndex )
{
	std::vector	<COMBO_LOG_DATA>::iterator ir = m_ComboTextList.begin();
	
	for( ; ir != m_ComboTextList.end() ; ++ir )
	{
		if( ir->Index == aIndex )
		{
			return true;
		}			
	}

	return false; 
}

void  CLogWnd::SetEnableCombo(int nClassType)
{
	m_bEnableCombo = true;

	if( Priest == nClassType || Holy_Avenger == nClassType ||
		Archer == nClassType || Sniper == nClassType 
		|| Magician == nClassType || Prominas == nClassType	)
	{
		m_bEnableCombo = false;
	}
	
	if(m_bEnableCombo)
	{
		for( int i = 0 ; i < COMBO_LIST_MAXCOUNT ; ++i )
		{
			m_pComboBtnList[ i ]->SetBtnHold( false );
		}
		
		m_pComboRegBtn->SetBtnHold( false );
		m_pComboRegBtn->SetClientImage(TID_None);
		m_pComboRegCancelBtn->SetBtnHold( false );
		m_pComboRegCancelBtn->SetClientImage(TID_None);
	}
	else
	{
		for( int i = 0 ; i < COMBO_LIST_MAXCOUNT ; ++i )
		{
			m_pComboBtnList[ i ]->SetBtnHold( true );
		}

		m_pComboRegBtn->SetBtnHold( true );
		m_pComboRegBtn->SetClientImage(TID_CTRL_BTN58_DIS);
		m_pComboRegCancelBtn->SetBtnHold( true  );
		m_pComboRegCancelBtn->SetClientImage(TID_CTRL_BTN58_DIS);
	}
	
	SubJobControl();
	ComboControl();
}

void CLogWnd::SetComboRegKey(int nNum, int nCount)
{
	g_Pc.theSkillSystem.theShortCut_SkillCode[2] = nNum;

	if(g_Pc.theSkillSystem.theShortCut_SkillCode[2])
	{
		g_Pc.GetPlayer()->theActionSystem.theTotComboStep = nCount;
	}
	else
	{
		g_Pc.GetPlayer()->theActionSystem.theTotComboStep = 0;
	}

}


