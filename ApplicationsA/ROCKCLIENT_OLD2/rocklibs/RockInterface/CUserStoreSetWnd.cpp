#include "..\\RockPCH.h"
#include "Rui.h"
#include "CUserStoreSetWnd.h"
#include "CUserStoreSellWnd.h"
#include "MessageWndProc.h"
#include "..\\..\\stringmanager.h"

CUserStoreSetWnd g_UserStoreSetWnd;

CUserStoreSetWnd::CUserStoreSetWnd()
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
	m_pCountEditLabel	= NULL;

	m_Price				= 0;
	m_Count				= 1;

	m_IsVisible			= FALSE;
	m_pIconImg			= NULL;
	m_pPcItem			= NULL;
	IsVisible			= FALSE;
}

CUserStoreSetWnd::~CUserStoreSetWnd()
{
}

void CUserStoreSetWnd::Composition()
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
	WndRect.h = 118;
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
	m_pFrame->SetCaption( G_STRING_CHAR( IDS_USERSTORE_SETITEM ) );					// 아이템 등록
	m_pFrame->SetTitle( true );
	
	m_pFrame->SetWndProc( this );	
	m_pFrame->SetVisible( false );
	m_pFrame->SetZorder( n_zoTop );
	m_pFrame->Initialize();	

	//...........................................................................................................
	// 에디트 박스 뒷 배경
	//...........................................................................................................
	CImageBox *img = NULL;
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 1, 31, 202, 56, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(),  1, 30, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 1, 87, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 16 , 39, 41, 41, false );
	img->SetClientImage( TID_CTRL_SLOT_BIG );
	img->SetFocusCheck( false );
	

	m_pEditBackImage[0] = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrame->GetID(),  62, 36, 125, 23, false );
	m_pEditBackImage[0]->SetWndTileSizeLR( 9, 9 );
	m_pEditBackImage[0]->SetWndTileSizeTB( 8, 8 );
	m_pEditBackImage[0]->SetFocusCheck( false );
	m_pEditBackImage[0]->SetClientImage( TID_CTRL_CONTBACK_L1 );
	m_pEditBackImage[0]->SetAdjustPosition( 0, 0 );
	m_pEditBackImage[0]->SetVisible( TRUE );

	m_pEditBackImage[1] = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrame->GetID(),  62, 59, 125, 23, false );
	m_pEditBackImage[1]->SetWndTileSizeLR( 9, 9 );
	m_pEditBackImage[1]->SetWndTileSizeTB( 8, 8 );
	m_pEditBackImage[1]->SetFocusCheck( false );
	m_pEditBackImage[1]->SetClientImage( TID_CTRL_CONTBACK_L1 );
	m_pEditBackImage[1]->SetAdjustPosition( 0, 0 );
	m_pEditBackImage[1]->SetVisible( TRUE );

	m_pIconImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID() , 19, 42, 35, 35, false );
	m_pIconImg->SetClientImage( TID_None );			// 해당하는 ITEM의 아이콘 표시 
	m_pIconImg->SetClientColor( 255, 255, 255 );

		

	//...........................................................................................................
	// 에디트 박스
	//...........................................................................................................
	m_pCountEditLabel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 117, 42, 59, 10, false );
	m_pCountEditLabel->SetFontR( n_fRsvFontWndStatic );
	m_pCountEditLabel->SetAlignText( n_atLeft, n_atCenter );
	m_pCountEditLabel->SetText(_RT("") );
	m_pCountEditLabel->SetTextColor( 255 , 255 , 255 );

	m_pCountEditBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, m_pFrame->GetID(), 117, 42, 59, 10, false );
	m_pCountEditBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
#ifdef UNICODE_ROCK
	m_pCountEditBox->SetTextSize( 8 );
#else 
	m_pCountEditBox->SetTextSize( 4 );
#endif 
	
	m_pCountEditBox->SetClientImage( TID_None );
	m_pCountEditBox->SetClientColor( 255, 255, 255 );
	m_pCountEditBox->SetAlignText( n_atLeft, n_atCenter );
	m_pCountEditBox->SetBlank( 2, 0, 0, 0 );
	m_pCountEditBox->SetEditStyle( esOnlyNumeric );
	m_pCountEditBox->SetReturnActionClear( false );
	m_pCountEditBox->SetText( _RT("") );
	m_pCountEditBox->InitEditBox();


	m_pPriceEditBox = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, m_pFrame->GetID(), 117, 65, 59, 10, false );
	m_pPriceEditBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
#ifdef UNICODE_ROCK
	m_pPriceEditBox->SetTextSize( 20 );
#else 
	m_pPriceEditBox->SetTextSize( 16 );
#endif
	m_pPriceEditBox->SetClientImage( TID_None );
	m_pPriceEditBox->SetClientColor( 255, 255, 255 );
	m_pPriceEditBox->SetAlignText( n_atLeft, n_atCenter );
	m_pPriceEditBox->SetBlank( 2, 0, 0, 0 );
	m_pPriceEditBox->SetEditStyle( esOnlyNumeric );
	m_pPriceEditBox->SetReturnActionClear( false );
	m_pPriceEditBox->SetText( _RT("") );
	m_pPriceEditBox->InitEditBox();	

	

	//...........................................................................................................
	// 각 레이블
	//...........................................................................................................
	m_pCountLabel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 71, 42, 34, 11, false );
	m_pCountLabel->SetFontR( n_fRsvFontWndStatic );
	m_pCountLabel->SetAlignText( n_atLeft, n_atCenter );
	m_pCountLabel->SetText( G_STRING_CHAR( IDS_USERSTORE_COUNT ) );		// 개수 
	m_pCountLabel->SetTextColor( 255 , 255 , 255 );

	m_pPriceLabel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 71, 65, 34, 11, false );       
	m_pPriceLabel->SetFontR( n_fRsvFontWndStatic );
	m_pPriceLabel->SetAlignText( n_atLeft, n_atCenter );
	m_pPriceLabel->SetText( G_STRING_CHAR( IDS_USERSTORE_PRICE ) );		// 가격
	m_pPriceLabel->SetTextColor( 255 , 255 , 255 );
	

	//...........................................................................................................
	// 버튼
	//...........................................................................................................
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  107, 90, 58, 25, false ); 
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pCloseBtn->SetFontR( n_fRsvFontWndButton );   
	m_pCloseBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );				// 취소
	m_pCloseBtn->SetVisible( TRUE );
	

	m_pXBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  184, 7, 13, 13, false ); 
	m_pXBtn->SetBtnImage( TID_CTRL_BTN_X , TID_CTRL_BTN_X_OVR , TID_CTRL_BTN_X_CLK , TID_None );
	m_pXBtn->SetFontR( n_fRsvFontWndButton );   
	m_pXBtn->SetText( _RT("") );
	m_pXBtn->SetVisible( TRUE );	
	
	m_pOKBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  40, 90, 58, 25, false );        
	m_pOKBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pOKBtn->SetFontR( n_fRsvFontWndButton );   
	m_pOKBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );				// 확인
	m_pOKBtn->SetVisible( TRUE );
	
}

void CUserStoreSetWnd::Update()
{
}

void CUserStoreSetWnd::Proc( SEventMessage* EMsg )
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
			
			m_pPriceEditBox->GetText( TempStr , 64 );

			if( Rstrlen( TempStr ) <= 0 )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_USERSTORE_INPUTPRICE ), 1500 , NULL );

				return;
			}

			m_Price = Ratoi( RWCHAR(TempStr) );

			if( m_Price <= 0 )
			{
				MessageBoxWnd.Open( G_STRING_CHAR( IDS_USERSTORE_INPUTPRICE ) , 1500 , NULL );
				return;
			}

			
			if( m_pCountEditBox->IsVisible() )
			{
				m_pCountEditBox->GetText( TempStr , 64 );
				m_Count = Ratoi( RWCHAR(TempStr) );

				if( m_Count <= 0 )
				{
					return;
				}
				else if( m_pPcItem->Amount < m_Count )
				{
					MessageBoxWnd.Open( G_STRING_CHAR( IDS_USERSTORE_MORECOUNT ) , 1500 , NULL );
					return;
				}
			}
			else
			{
				m_Count = 1;
			}
			
			m_pFrame->SetVisible( FALSE );
			m_IsVisible = FALSE;
			IsVisible	= FALSE;

			// 상점에 아이템 넣어준다
			g_UserStoreSellWnd.InsertSaveItem( m_Price , m_Count );
		}	
	}
}

void CUserStoreSetWnd::Open(void)
{
	if( NULL == m_pFrame )
		return;
	
	m_pFrame->SetVisible( TRUE );
	
	m_IsVisible = TRUE;
	IsVisible	= TRUE;

	nRui->SendEvent( m_pPriceEditBox->GetID(), n_emSetFocus, 0, 0, 0, 0 );

	// 기본 값
	m_Price				= 0;

	char TempStr[ 64 ] = {0,};
	Rsprintf( RWCHAR(TempStr) , _RT("%d") , m_Count );
	
	if( m_Count == 1 )
	{
		m_pCountEditLabel->SetText( TempStr );
		
		m_pCountEditLabel->SetVisible( TRUE );
		m_pCountEditBox->SetVisible( FALSE );
	}
	else
	{
		m_pCountEditBox->SetVisible( TRUE );
		m_pCountEditBox->SetText( TempStr );
		m_pCountEditBox->GetWindowEditText();

		m_pCountEditLabel->SetVisible( FALSE );
	}

}

void CUserStoreSetWnd::Close(void)
{
	if( NULL == m_pFrame )
		return;
	
	m_pFrame->SetVisible( FALSE );
	m_IsVisible = FALSE;
	IsVisible	= FALSE;
}

void CUserStoreSetWnd::SetItemInfo( SPcItem* _pc_item , int Count )
{
	char str[40] = "";

	m_pPcItem	= _pc_item;

	m_pIconImg->SetClientImage( m_pPcItem->Tid );
	m_pIconImg->SetVisible( TRUE );

	m_pPriceEditBox->SetText(_RT("") );

	m_Count				= Count;
}
