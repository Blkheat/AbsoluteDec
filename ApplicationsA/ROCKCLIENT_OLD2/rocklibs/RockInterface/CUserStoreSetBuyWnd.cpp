#include "..\\RockPCH.h"
#include "Rui.h"
#include "CUserStoreSetBuyWnd.h"
#include "MessageWndProc.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\stringmanager.h"
#include "InventoryWndProc.h"

CUserStoreSetBuyWnd g_UserStoreSetBuyWnd;

CUserStoreSetBuyWnd::CUserStoreSetBuyWnd()
{
	m_pFrame			= NULL;
	m_pCloseBtn			= NULL;
	m_pXBtn				= NULL;
	m_pOKBtn			= NULL;

	m_pPriceLabel		= NULL;
	m_pCountLabel		= NULL;
	
	m_pEditBackImage[0]	= NULL;
	m_pEditBackImage[1]	= NULL;
	
	m_pPriceEditBox		= NULL;
	m_pCountEditBox		= NULL;

	m_Price				= 0;
	m_Count				= 1;

	m_IsVisible			= FALSE;
	m_pIconImg			= NULL;
	m_pPcItem			= NULL;
	IsVisible			= FALSE;
}

CUserStoreSetBuyWnd::~CUserStoreSetBuyWnd()
{
}

void CUserStoreSetBuyWnd::Composition()
{
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)
	
	int center_x	= nRui->Desktop.Width >> 1;
	int center_y	= nRui->Desktop.Height >> 1;
	
#else
	
	int center_x	= 1024.0f / 2.0f;
	int center_y	= 768.0f  / 2.0f;
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)

	SRect			WndRect;
 
	WndRect.w = 204;
	WndRect.h = 136;
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
	m_pFrame->SetCaption( G_STRING_CHAR( IDS_USERSTORE_ITEMBUY ) );  // 아이템 구입
	m_pFrame->SetTitle( true );
	
	m_pFrame->SetWndProc( this );	
	m_pFrame->SetVisible( false );
	m_pFrame->SetZorder( n_zoTop );
	m_pFrame->Initialize();

	//...........................................................................................................
	// 에디트 박스 뒷 배경
	//...........................................................................................................
	CImageBox *img = NULL;
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 1,  31, 202, 72, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(),  1,  30, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 1, 103, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 11, 47, 41, 41, false );
	img->SetClientImage( TID_CTRL_SLOT_BIG );
	img->SetFocusCheck( false );

	m_pEditBackImage[0] = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrame->GetID(),  62, 36, 133, 39, false );
	m_pEditBackImage[0]->SetWndTileSizeLR( 9, 9 );
	m_pEditBackImage[0]->SetWndTileSizeTB( 8, 8 );
	m_pEditBackImage[0]->SetFocusCheck( false );
	m_pEditBackImage[0]->SetClientImage( TID_CTRL_CONTBACK_L1 );
	m_pEditBackImage[0]->SetAdjustPosition( 0, 0 );
	m_pEditBackImage[0]->SetVisible( TRUE );

	m_pEditBackImage[1] = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrame->GetID(),  62, 76, 133, 23, false );
	m_pEditBackImage[1]->SetWndTileSizeLR( 9, 9 );
	m_pEditBackImage[1]->SetWndTileSizeTB( 8, 8 );
	m_pEditBackImage[1]->SetFocusCheck( false );
	m_pEditBackImage[1]->SetClientImage( TID_CTRL_CONTBACK_L1 );
	m_pEditBackImage[1]->SetAdjustPosition( 0, 0 );
	m_pEditBackImage[1]->SetVisible( TRUE );

	m_pIconImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 14, 50, 35, 35, false );
	m_pIconImg->SetClientImage( TID_None );			// 해당하는 ITEM의 아이콘 표시 
	m_pIconImg->SetClientColor( 255, 255, 255 );

	int nXPos = 104;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	nXPos = 119;

#endif		

	//...........................................................................................................
	// 에디트 박스
	//...........................................................................................................
	m_pCountEditBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, m_pFrame->GetID(), nXPos, 59, 83, 10, false );  
	m_pCountEditBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pCountEditBox->SetTextSize( 10 );
	
	m_pCountEditBox->SetClientImage( TID_None );
	m_pCountEditBox->SetClientColor( 255, 255, 255 );
	m_pCountEditBox->SetAlignText( n_atLeft, n_atCenter );
	m_pCountEditBox->SetBlank( 2, 0, 0, 0 );
	m_pCountEditBox->SetEditStyle( esOnlyNumeric );
	m_pCountEditBox->SetReturnActionClear( false );
	m_pCountEditBox->SetText(_RT("") );
	m_pCountEditBox->InitEditBox();	

	m_pPriceEditBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), nXPos, 41, 83, 10, false );  
	m_pPriceEditBox->SetFontR( n_fRsvFontWndStatic );
	m_pPriceEditBox->SetAlignText( n_atLeft, n_atCenter );
	m_pPriceEditBox->SetText(_RT("") );
	m_pPriceEditBox->SetTextColor( 255 , 255 , 255 );

	m_pTotalEditBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), nXPos, 81, 83, 10, false );  
	m_pTotalEditBox->SetFontR( n_fRsvFontWndStatic );
	m_pTotalEditBox->SetAlignText( n_atLeft, n_atCenter );
	m_pTotalEditBox->SetText(_RT("") );
	m_pTotalEditBox->SetTextColor( 255 , 255 , 255 );

	//...........................................................................................................
	// 각 레이블
	//...........................................................................................................
	m_pCountLabel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 70, 59, 21, 11, false );       
	m_pCountLabel->SetFontR( n_fRsvFontWndStatic );
	m_pCountLabel->SetAlignText( n_atLeft, n_atCenter );
	m_pCountLabel->SetText( G_STRING_CHAR( IDS_USERSTORE_COUNT ) );		// 수량
	m_pCountLabel->SetTextColor( 255 , 255 , 255 );

	m_pPriceLabel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 70, 41, 22, 11, false );         
	m_pPriceLabel->SetFontR( n_fRsvFontWndStatic );
	m_pPriceLabel->SetAlignText( n_atLeft, n_atCenter );
	m_pPriceLabel->SetText( G_STRING_CHAR( IDS_USERSTORE_PRICE ) );		// 가격
	m_pPriceLabel->SetTextColor( 255 , 255 , 255 );

	m_pTotalLabel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 69, 81, 22, 11, false );      
	m_pTotalLabel->SetFontR( n_fRsvFontWndStatic );
	m_pTotalLabel->SetAlignText( n_atLeft, n_atCenter );
	m_pTotalLabel->SetText( G_STRING_CHAR( IDS_USERSTORE_TOTAL ) );
	m_pTotalLabel->SetTextColor( 255 , 255 , 255 );

	//...........................................................................................................
	// 버튼
	//...........................................................................................................
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  107, 107, 58, 25, false );  
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pCloseBtn->SetFontR( n_fRsvFontWndButton );   
	m_pCloseBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );				// 취소
	m_pCloseBtn->SetVisible( TRUE );

	m_pXBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  184, 7, 13, 13, false ); 
	m_pXBtn->SetBtnImage( TID_CTRL_BTN_X , TID_CTRL_BTN_X_OVR , TID_CTRL_BTN_X_CLK , TID_None );
	m_pXBtn->SetFontR( n_fRsvFontWndButton );   
	m_pXBtn->SetText(_RT("") );
	m_pXBtn->SetVisible( TRUE );
	
	m_pOKBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  40, 107, 58, 25, false );        
	m_pOKBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pOKBtn->SetFontR( n_fRsvFontWndButton );   
	m_pOKBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );				// 확인
	m_pOKBtn->SetVisible( TRUE );
}

void CUserStoreSetBuyWnd::Update()
{
	CalTotal();

	if( m_Count > m_pPcItem->Amount )
	{
		char TempStr[ 256 ] = "";

		Rsprintf( RWCHAR(TempStr) ,_RT("%d") , m_pPcItem->Amount );

		m_pCountEditBox->SetText( TempStr );

		CalTotal();

		MessageBoxWnd.Open( G_STRING_CHAR( IDS_USERSTORE_MORECOUNT ) , 500 , NULL );
	}
}

void CUserStoreSetBuyWnd::Proc( SEventMessage* EMsg )
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
			IsVisible			= FALSE;
		}
	}
	//...........................................................................................................
	// 확인 버튼
	//...........................................................................................................
	else if( EMsg->FocusWnd == m_pOKBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			char TempStr[ 64 ] = {0,};
			
			m_pCountEditBox->GetText( TempStr , 64 );
			if( Rstrlen( RWCHAR(TempStr) ) <= 0 )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_USERSTORE_INPUTCOUNT ) , 1500 , NULL );		// 개수를 입력하세요

				return;
			}

			m_Count = Ratoi( RWCHAR(TempStr) );

			if( m_Count > m_pPcItem->Amount || m_Count <= 0 )
			{
				if( m_Count == 0 )
				{
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_USERSTORE_INPUTCOUNT ) , 1500 , NULL );		// 개수를 입력하세요
					
					return;
				}

				MessageBoxWnd.Open( G_STRING_CHAR( IDS_USERSTORE_MORECOUNT ), 1500 , NULL );		// 아이템 개수보다 더 많이 입력하셨습니다.

				return;
			}

			m_pFrame->SetVisible( FALSE );
			m_IsVisible = FALSE;
			IsVisible	= FALSE;

			// 서버에 살꺼 보내준다
			g_RockClient.Send_CsBuyInPCShop( g_UserStoreBuyWnd.m_ShopPCID , m_pPcItem->ItemUID , ( BYTE ) m_Count ); 
		}	
	}
	else if( EMsg->FocusWnd == m_pCountEditBox->GetID() )
	{
	}
}

void CUserStoreSetBuyWnd::Open(void)
{
	if( NULL == m_pFrame )
		return;
	
	m_pFrame->SetVisible( TRUE );
	
	m_IsVisible = TRUE;
	IsVisible	= TRUE;

	char TempStr[ 256 ] = {0,};

	nRui->SendEvent( m_pCountEditBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );

	Rsprintf( RWCHAR(TempStr) , _RT("%d") , m_pPcItem->Amount );

	m_pCountEditBox->SetText( TempStr );
	m_pCountEditBox->GetWindowEditText();

	m_Count				= m_pPcItem->Amount;

	CalTotal();
}

void CUserStoreSetBuyWnd::CalTotal(void)
{
	static int count = -1;
	static int Price = -1;

	int NowCount = 0;

	char TempStr[ 256 ] = {0,};
	
	m_pCountEditBox->GetText( TempStr , 256 );
	
	if( Rstrlen( TempStr ) <= 0 )
	{
		NowCount = 0;
	}
	else
	{
		NowCount = Ratoi( RWCHAR(TempStr) );
	}
	
	if( NowCount != count || Price != m_Price )
	{
		m_Count = NowCount;
		Price = m_Price;
		count = NowCount;

		m_pTotalEditBox->SetText( TempStr );
				
		LONGLONG total;
		total = (LONGLONG)NowCount * (LONGLONG)m_Price;
	
		Rsprintf(RWCHAR(TempStr) , _RT("%I64u") , total );
		InventoryWnd.SetEldText(TempStr);
		m_pTotalEditBox->SetText( TempStr );
	}
}

void CUserStoreSetBuyWnd::Close(void)
{
	if( NULL == m_pFrame )
		return;
	
	m_pFrame->SetVisible( FALSE );
	
	m_IsVisible = FALSE;
}

void CUserStoreSetBuyWnd::SetItemInfo( SPcItem* _pc_item , int Price )
{
	char str[128] = {0,};

	m_pPcItem	= _pc_item;

	m_pIconImg->SetClientImage( m_pPcItem->Tid );
	m_pIconImg->SetVisible( TRUE );
	
	//Rsprintf( RWCHAR(str) , _RT("%d") , Price );
	InventoryWnd.SetEldText(str , Price);
	m_pPriceEditBox->SetText( str );

	m_Price = Price;
}
