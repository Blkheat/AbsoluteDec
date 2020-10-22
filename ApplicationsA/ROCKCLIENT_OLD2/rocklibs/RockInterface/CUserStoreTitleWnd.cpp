#include "Rui.h"
#include "CUserStoreTitleWnd.h"
#include "CUserStoreSellWnd.h"
#include "InventoryWndProc.h"
#include "..\\..\\stringmanager.h"
#include "..\\..\\brother.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\Map.h"
#include "UiUtil.h"
#include "MessageWndProc.h"

CUserStoreTitleWnd g_UserStoreTitleWnd;

CUserStoreTitleWnd::CUserStoreTitleWnd()
{
	m_pFrame				= NULL;
	m_pCloseBtn				= NULL;
	m_pXBtn					= NULL;
	m_pOKBtn				= NULL;
	m_pStoreNameLabel		= NULL;	
	m_pStoreContextLabel	= NULL;
	m_pHelpLabel			= NULL;
	m_pEditBackImage[0]		= NULL;
	m_pEditBackImage[1]		= NULL;

	memset( m_StoreNameStr    , 0 , sizeof( m_StoreNameStr    ) );
	memset( m_StoreContextStr , 0 , sizeof( m_StoreContextStr ) );

	m_IsVisible = FALSE;
	IsVisible	= FALSE;
}

CUserStoreTitleWnd::~CUserStoreTitleWnd()
{
}

void CUserStoreTitleWnd::Open(void)
{
	if( NULL == m_pFrame )
		return;
	
	m_pFrame->SetVisible( TRUE );
	
	m_IsVisible = TRUE;
	IsVisible	= TRUE;

	char TempStr[ 256 ] = {0,};

	Rsprintf( RWCHAR(TempStr) , G_STRING_CHAR( IDS_USERSTORE_NAME_EDIT_SET ) , RWCHAR(g_Pc.m_MainPC.char_info.thePCName) );
	
	m_pStoreNameEditBox->SetText( TempStr );
	m_pStoreNameEditBox->GetWindowEditText();

	Rsprintf( RWCHAR(TempStr) , G_STRING_CHAR( IDS_USERSTORE_CONTEXT_EDIT_SET ) );
	m_pStoreContextEditBox->SetText( TempStr );
	m_pStoreContextEditBox->GetWindowEditText();	
	
	nRui->SendEvent( m_pStoreNameEditBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );
}

void CUserStoreTitleWnd::Composition()
{
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)
	
	int center_x	= nRui->Desktop.Width >> 1;
	int center_y	= nRui->Desktop.Height >> 1;
	
#else
	
	int center_x	= 1024.0f / 2.0f;
	int center_y	= 768.0f  / 2.0f;
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)

	SRect			WndRect;
 
	WndRect.w = 343;
	WndRect.h = 162;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	WndRect.w = 393;	

#endif

	WndRect.x = center_x - ( WndRect.w / 2 );
	WndRect.y = center_y - ( WndRect.h / 2 );

	//...........................................................................................................
	// 메인 프레임
	//...........................................................................................................
	m_pFrame = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None , WID_None , WndRect.x, WndRect.y, WndRect.w, WndRect.h, false );
	m_pFrame->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrame->SetClientImage( TID_CTRL_WND_L1 );
	m_pFrame->SetIconTID( TID_CTRL_ICON_NONE );	
	m_pFrame->SetWndTileSizeLR( 40, 29 );
	m_pFrame->SetWndTileSizeTB( 31, 11 );
	m_pFrame->SetCaption( G_STRING_CHAR( IDS_USERSTORE_INIT ) );					// 상점 초기 설정
	m_pFrame->SetTitle( true );
	
	m_pFrame->SetWndProc( this );	
	m_pFrame->SetVisible( false );
	m_pFrame->Initialize();


	SRect sFrameRect;
	SRect sTempRect;
	
	sFrameRect.x	= 1;
	sFrameRect.y	= 70;
	sFrameRect.w	= 341;
	sFrameRect.h	= 56;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sFrameRect.w	= 391;	

#endif
	//...........................................................................................................
	// 에디트 박스 뒷 배경
	//...........................................................................................................
	CImageBox *img = NULL;
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 
									   sFrameRect.x, sFrameRect.y, sFrameRect.w, sFrameRect.h, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	sFrameRect.x	= 1;
	sFrameRect.y	= 69;
	sFrameRect.w	= 341;
	sFrameRect.h	= 1;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sFrameRect.w	= 391;	

#endif	
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(),  
									   sFrameRect.x, sFrameRect.y, sFrameRect.w, sFrameRect.h, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	sFrameRect.x	= 1;
	sFrameRect.y	= 126;
	sFrameRect.w	= 341;
	sFrameRect.h	= 1;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sFrameRect.w	= 391;	

#endif	

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 
									   sFrameRect.x, sFrameRect.y, sFrameRect.w, sFrameRect.h, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );	

	sFrameRect.x	= 6;
	sFrameRect.y	= 75;
	sFrameRect.w	= 331;
	sFrameRect.h	= 23;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sFrameRect.w	= 381;	

#endif	

	m_pEditBackImage[0] = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrame->GetID(), 
														sFrameRect.x, sFrameRect.y, sFrameRect.w, sFrameRect.h, false );	
	m_pEditBackImage[0]->SetWndTileSizeLR( 9, 9 );
	m_pEditBackImage[0]->SetWndTileSizeTB( 8, 8 );
	m_pEditBackImage[0]->SetFocusCheck( false );
	m_pEditBackImage[0]->SetClientImage( TID_CTRL_CONTBACK_L1 );
	m_pEditBackImage[0]->SetAdjustPosition( 0, 0 );
	m_pEditBackImage[0]->SetVisible( TRUE );

	sFrameRect.x	= 6;
	sFrameRect.y	= 98;
	sFrameRect.w	= 331;
	sFrameRect.h	= 23;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sFrameRect.w	= 381;	

#endif	

	m_pEditBackImage[1] = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrame->GetID(),  
														sFrameRect.x, sFrameRect.y, sFrameRect.w, sFrameRect.h, false );	
	m_pEditBackImage[1]->SetWndTileSizeLR( 9, 9 );
	m_pEditBackImage[1]->SetWndTileSizeTB( 8, 8 );
	m_pEditBackImage[1]->SetFocusCheck( false );
	m_pEditBackImage[1]->SetClientImage( TID_CTRL_CONTBACK_L1 );
	m_pEditBackImage[1]->SetAdjustPosition( 0, 0 );
	m_pEditBackImage[1]->SetVisible( TRUE );
	
	//...........................................................................................................
	// 각 레이블
	//...........................................................................................................
	m_pHelpLabel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 13, 44, 212, 11, false );
	m_pHelpLabel->SetFontR( n_fRsvFontWndStatic );
	m_pHelpLabel->SetAlignText( n_atLeft, n_atCenter );
	m_pHelpLabel->SetText( G_STRING_CHAR( IDS_USERSTORE_HELP0 ) );		// 상점 이름과 인사말을 정해주세요 최대 20자
	m_pHelpLabel->SetTextColor( 0 , 0 , 0 );

	m_pStoreNameLabel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 16, 81, 42, 11, false );
	m_pStoreNameLabel->SetFontR( n_fRsvFontWndStatic );
	m_pStoreNameLabel->SetAlignText( n_atLeft, n_atCenter );
	m_pStoreNameLabel->SetText( G_STRING_CHAR( IDS_USERSTORE_STORE_NAME ) );		// 상점명 :
	m_pStoreNameLabel->SetTextColor( 255 , 255 , 255 );

	m_pStoreContextLabel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 16, 104, 42, 11, false );
	m_pStoreContextLabel->SetFontR( n_fRsvFontWndStatic );
	m_pStoreContextLabel->SetAlignText( n_atLeft, n_atCenter );
	m_pStoreContextLabel->SetText( G_STRING_CHAR( IDS_USERSTORE_STORE_CONTEXT ) );		// 인사말 :
	m_pStoreContextLabel->SetTextColor( 255 , 255 , 255 );	
	
	sTempRect.x	= 85;
	sTempRect.y	= 81;
	sTempRect.w	= 264;
	sTempRect.h	= 11;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 135;

#endif

	//...........................................................................................................
	// 에디트 박스
	//...........................................................................................................
	m_pStoreNameEditBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, m_pFrame->GetID(), 
						   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	m_pStoreNameEditBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pStoreNameEditBox->SetTextSize( 40 );
	m_pStoreNameEditBox->SetClientImage( TID_None );
	m_pStoreNameEditBox->SetClientColor( 255, 255, 255 );
	m_pStoreNameEditBox->SetAlignText( n_atLeft, n_atCenter );
	m_pStoreNameEditBox->SetBlank( 2, 0, 0, 0 );
	m_pStoreNameEditBox->SetEditStyle( esText );
	m_pStoreNameEditBox->SetReturnActionClear( false );
	m_pStoreNameEditBox->SetText( _RT("") );						// 상점 이름을 입력하세요
	m_pStoreNameEditBox->InitEditBox();

	sTempRect.x	= 85;
	sTempRect.y	= 104;
	sTempRect.w	= 264;
	sTempRect.h	= 11;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 135;	

#endif	
	
	m_pStoreContextEditBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, m_pFrame->GetID(), 
							sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );       
	m_pStoreContextEditBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pStoreContextEditBox->SetTextSize( 40 );
	m_pStoreContextEditBox->SetClientImage( TID_None );
	m_pStoreContextEditBox->SetClientColor( 255, 255, 255 );
	m_pStoreContextEditBox->SetAlignText( n_atLeft, n_atCenter );
	m_pStoreContextEditBox->SetBlank( 2, 0, 0, 0 );
	m_pStoreContextEditBox->SetEditStyle( esText );
	m_pStoreContextEditBox->SetReturnActionClear( false );				
	m_pStoreContextEditBox->SetText( _RT("") );					// 상점 인사말을 입력하세요
	m_pStoreContextEditBox->InitEditBox();

	sTempRect.x	= 176;
	sTempRect.y	= 131;
	sTempRect.w	= 58;
	sTempRect.h	= 25;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 201;	

#endif	

	//...........................................................................................................
	// 버튼
	//...........................................................................................................
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID(),  
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pCloseBtn->SetFontR( n_fRsvFontWndButton );   
	m_pCloseBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );				// 취소
	m_pCloseBtn->SetVisible( TRUE );

	sTempRect.x	= 109;
	sTempRect.y	= 131;
	sTempRect.w	= 58;
	sTempRect.h	= 25;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 134;	

#endif

	m_pOKBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID(),  
										  sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	m_pOKBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pOKBtn->SetFontR( n_fRsvFontWndButton );   
	m_pOKBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK )  );				// 확인
	m_pOKBtn->SetVisible( TRUE );

	sTempRect.x	= 323;
	sTempRect.y	= 7;
	sTempRect.w	= 13;
	sTempRect.h	= 13;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 373;	

#endif

	m_pXBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None ,m_pFrame->GetID() ,  
										 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false ); 
	m_pXBtn->SetBtnImage( TID_CTRL_BTN_X , TID_CTRL_BTN_X_OVR , TID_CTRL_BTN_X_CLK , TID_None );
	m_pXBtn->SetFontR( n_fRsvFontWndButton );   
	m_pXBtn->SetText(_RT("") );
	m_pXBtn->SetVisible( TRUE );
}

void CUserStoreTitleWnd::Update()
{
}

void CUserStoreTitleWnd::Proc( SEventMessage* EMsg )
{
	//...........................................................................................................
	// 취소 버튼
	//...........................................................................................................
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() || EMsg->FocusWnd == m_pXBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_pFrame->SetVisible( FALSE );
			
			m_IsVisible = FALSE;
			IsVisible	= FALSE;

			g_UserStoreSellWnd.SetUserSellMode( FALSE );  // 상점 상태를 해제해준다
		}
	}
	//...........................................................................................................
	// 확인 버튼
	//...........................................................................................................
	else if( EMsg->FocusWnd == m_pOKBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_pStoreNameEditBox->GetText( m_StoreNameStr , 41 );
			::DeleteTokenInString(m_StoreNameStr, _RT('\\'));			
			m_pStoreContextEditBox->GetText( m_StoreContextStr , 41 );

			if( Rstrlen( RWCHAR(m_StoreNameStr) ) <= 0 )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_USERSTORE_HELP0 ) , 1500 , NULL );		// 개수를 입력하세요

				return;
			}

			if( Rstrlen( RWCHAR(m_StoreContextStr) ) <= 0 )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_USERSTORE_HELP0 ) , 1500 , NULL );		// 개수를 입력하세요

				return;
			}


			m_pFrame->SetVisible( FALSE );
			m_IsVisible = FALSE;
			IsVisible	= FALSE;

			// 상점 창을 열어 준다
			if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_StoreNameStr ) )
			{
				Rsprintf( RWCHAR(m_StoreNameStr) , _RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
			}

			if( g_RockClient.m_SlangFilter.Filter( ( char * ) m_StoreContextStr ) )
			{
				Rsprintf( RWCHAR(m_StoreContextStr) ,_RT("%s") , RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
			}

			g_UserStoreSellWnd.m_pStoreNameText->SetText( m_StoreNameStr );
			g_UserStoreSellWnd.m_pStoreContextText->SetText( m_StoreContextStr );
			g_UserStoreSellWnd.Open();

			POINT pt;
			SRect RStore;
			SRect RInven;

			g_UserStoreSellWnd.m_pFrame->GetClientSize( &RStore );
			InventoryWnd.FrameWnd->GetClientSize( &RInven );

		#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)
			
			pt.x = ( nRui->Desktop.Width - ( RStore.w + RInven.w ) ) / 2;
			pt.y = ( nRui->Desktop.Height -  RStore.h ) / 2;
			
		#else
			
			pt.x = ( 1024 - ( RStore.w + RInven.w ) ) / 2;
			pt.y = ( 768 -  RStore.h ) / 2;
			
		#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)

			g_UserStoreSellWnd.m_pFrame->Reposition( pt.x , pt.y );
			
			pt.x += RStore.w;
			
			InventoryWnd.FrameWnd->Reposition( pt.x , pt.y );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
			InventoryWnd.Open();
#else
			nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
		}	
	}


}

void CUserStoreTitleWnd::Close()
{
	m_pFrame->SetVisible( FALSE );

	m_IsVisible = FALSE;
	IsVisible	= FALSE;

	g_UserStoreSellWnd.SetUserSellMode( FALSE );  // 상점 상태를 해제해준다
	
	return;
}

