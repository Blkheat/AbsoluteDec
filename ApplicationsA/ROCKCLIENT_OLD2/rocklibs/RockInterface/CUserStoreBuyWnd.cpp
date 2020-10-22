#include "Rui.h"
#include "CUserStoreBuyWnd.h"
#include "CUserStoreSetBuyWnd.h"
#include "..\\..\\stringmanager.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "StateButton.h"
#include "MCommunityWnd.h"
#include "InventoryWndProc.h"
CUserStoreBuyWnd g_UserStoreBuyWnd;


CUserStoreBuyWnd::CUserStoreBuyWnd()
{
	m_pFrame    = NULL;
	m_pVScroll  = NULL;
	m_pCloseBtn = NULL;
	m_pStartBtn = NULL;
	m_pSortBtn  = NULL;

	for( int i = 0 ; i <USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ; ++i )
	{
		m_TextBox[USER_STORE_SELL_ITEM_SLOT_MAX_COUNT][0] = NULL;
		m_TextBox[USER_STORE_SELL_ITEM_SLOT_MAX_COUNT][1] = NULL;
	}

	ResetItemList();

	m_IsVisible = FALSE;
	m_UserStoreBuyMode = FALSE;
	m_ShopPCID = 0;
	IsVisible			= FALSE;
}

CUserStoreBuyWnd::~CUserStoreBuyWnd()
{
}

void CUserStoreBuyWnd::ResetItemList(void)
{
	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ; ++i ) 	
	{
		m_pItemSlotList[ i ] = NULL;

		if( NULL != m_TextBox[ i ][0] )
		{
			m_TextBox[ i ][ 1 ]->SetText( _RT("") );
		}

		if( NULL != m_TextBox[ i ][1] )
		{
			m_TextBox[ i ][ 1 ]->SetText( _RT("") );
		}

	}

	ItemClear();

	m_TopSlotIndex = 0;
}

void CUserStoreBuyWnd::SetWheelValue( short _wheel )
{
	int value = 0;
	int Max   = 0;

	Max   = m_pVScroll->GetSldMax();
	m_pVScroll->GetValue( &value );

	if( ( _wheel < 0 ) && ( value < Max ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}

	m_pVScroll->SetValue( value );
	m_TopSlotIndex = value;

	UpdateSlot();
}

void CUserStoreBuyWnd::Open(void)
{
	if( NULL == m_pFrame )
		return;
	
	m_pFrame->SetVisible( TRUE );
	m_UserStoreBuyMode = TRUE;
	m_IsVisible = TRUE;
	IsVisible			= TRUE;
}

void CUserStoreBuyWnd::Composition()
{
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)
	
	int center_x	= nRui->Desktop.Width >> 1;
	int center_y	= nRui->Desktop.Height >> 1;
	
#else
	
	int center_x	= 1024.0f / 2.0f;
	int center_y	= 768.0f  / 2.0f;
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/24)

	SRect			WndRect;
 
	WndRect.w = 380;
	WndRect.h = 271;
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
	m_pFrame->SetCaption( G_STRING_CHAR( IDS_USERSTORE_TITLE ) ); // 개인상점 구입
	m_pFrame->SetTitle( true );
	
	m_pFrame->SetWndProc( this );	
	m_pFrame->SetVisible( false );
	m_pFrame->Initialize();

	//...........................................................................................................
	// 뒷 배경
	//...........................................................................................................
	CImageBox   *img  = NULL;
	CImageBox9  *img9 = NULL;
	CTextBox	*Text = NULL;

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 1, 62, 365, 174, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(),  1,  61, 365, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), 1,  236, 365, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	POINT pt[ 8 ] = { { 54 , 64 } , { 234 , 64 } , { 54 , 107 } , { 234 , 107 }  ,
	{ 54 ,150 } , { 234 , 150 } , { 54 , 193 } , { 234 , 193 }   };
	
	POINT ptItem[ 8 ] = { { 8 , 64 } , { 188 , 64 } , { 8 , 107 } , { 188 , 107 }  ,
	{ 8 ,150 } , { 188 , 150 } , { 8 , 193 } , { 188 , 193 }   };

	POINT ptCount[ 8 ] = { { 63 , 71 } , { 243 , 71 } , { 63 , 114 } , { 243 , 114 }  ,
	{ 63 ,157 } , { 243 , 157 } , { 63 , 200 } , { 243 , 200 }   };

	POINT ptPrice[ 8 ] = { { 63 , 87 } , { 243 , 87 } , { 63 , 130 } , { 243 , 130 }  ,
	{ 63 ,173 } , { 243 , 173 } , { 63 , 216 } , { 243 , 216 }   };

	POINT ptCount2[ 8 ] = { { 111 , 71 } , { 289 , 71 } , { 111 , 114 } , { 289 , 114 }  ,
	{ 111 ,157 } , { 289 , 157 } , { 111 , 200 } , { 289 , 200 }   };

	POINT ptPrice2[ 8 ] = { { 111 , 87 } , { 289 , 87 } , { 111 , 130 } , { 289 , 130 }  ,
	{ 111 ,173 } , { 289 , 173 } , { 111 , 216 } , { 289 , 216 }   };

	for( int i = 0 ; i < 8 ; ++i )
	{
		img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrame->GetID(),  pt[ i ].x , pt[ i ].y , 125, 41, false );
		img9->SetWndTileSizeLR( 9, 9 );
		img9->SetWndTileSizeTB( 8, 8 );
		img9->SetFocusCheck( false );
		img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
		img9->SetAdjustPosition( 0, 0 );
		img9->SetVisible( TRUE );

		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrame->GetID(), ptItem[ i ].x , ptItem[ i ].y , 41, 41, false );
		img->SetClientImage( TID_CTRL_SLOT_BIG );
		img->SetFocusCheck( false );
	
		
		m_TextBox[i][2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), ptCount[ i ].x , ptCount[ i ].y , 34, 11, false );        
		m_TextBox[i][2]->SetFontR( n_fRsvFontWndStatic );
		m_TextBox[i][2]->SetAlignText( n_atLeft, n_atCenter );
		m_TextBox[i][2]->SetText( G_STRING_CHAR( IDS_USERSTORE_COUNT )  );		// 수량
		m_TextBox[i][2]->SetTextColor( 255 , 255 , 255 );

		m_TextBox[i][3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), ptPrice[ i ].x , ptPrice[ i ].y , 34, 11, false );        
		m_TextBox[i][3]->SetFontR( n_fRsvFontWndStatic );
		m_TextBox[i][3]->SetAlignText( n_atLeft, n_atCenter );
		m_TextBox[i][3]->SetText( G_STRING_CHAR( IDS_USERSTORE_PRICE ) );		// 가격
		m_TextBox[i][3]->SetTextColor( 255 , 255 , 255 );

		m_TextBox[i][0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), ptCount2[ i ].x , ptCount2[ i ].y , 59, 11, false );        
		m_TextBox[i][0]->SetFontR( n_fRsvFontWndStatic );
		m_TextBox[i][0]->SetAlignText( n_atRight, n_atCenter );
		m_TextBox[i][0]->SetText( _RT("") );
		m_TextBox[i][0]->SetTextColor( 255 , 255 , 255 );

		m_TextBox[i][1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), ptPrice2[ i ].x , ptPrice2[ i ].y , 59, 11, false );        
		m_TextBox[i][1]->SetFontR( n_fRsvFontWndStatic );
		m_TextBox[i][1]->SetAlignText( n_atRight, n_atCenter );
		m_TextBox[i][1]->SetText( _RT("") );
		m_TextBox[i][1]->SetTextColor( 255 , 255 , 255 );
			
		m_pItemSlotList[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrame->GetID() , ptItem[ i ].x + 3 , ptItem[ i ].y + 3 , 35, 35, false );
		m_pItemSlotList[i]->PcItem = NULL;
		m_pItemSlotList[i]->SetSlotGroup( n_SGROUP_USER_STORE_BUY , i );
		m_pItemSlotList[i]->SetHID( HID_ITEMTIP );	
	}

	m_pStoreNameText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 12, 30, 346, 11, false );        
	m_pStoreNameText->SetFontR( n_fRsvFontWndStatic );
	m_pStoreNameText->SetAlignText( n_atLeft, n_atCenter );
	m_pStoreNameText->SetText( _RT("") );
	m_pStoreNameText->SetTextColor( 0 , 0 , 0 );

	m_pStoreContextText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 20, 46, 338, 11, false );        
	m_pStoreContextText->SetFontR( n_fRsvFontWndStatic );
	m_pStoreContextText->SetAlignText( n_atLeft, n_atCenter );
	m_pStoreContextText->SetText( _RT("") );
	m_pStoreContextText->SetTextColor( 0 , 0 , 0 );

	//...........................................................................................................
	// 버튼
	//...........................................................................................................
	m_pStartBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  272, 241, 87, 25, false );        
	m_pStartBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pStartBtn->SetFontR( n_fRsvFontWndButton );   
	m_pStartBtn->SetText( G_STRING_CHAR( IDS_USERSTORE_MASTERCHAT ) );				// 1:1대화
	m_pStartBtn->SetVisible( TRUE );

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  360, 7, 13, 13, false ); 
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X , TID_CTRL_BTN_X_OVR , TID_CTRL_BTN_X_CLK , TID_None );
	m_pCloseBtn->SetFontR( n_fRsvFontWndButton );   
	m_pCloseBtn->SetText(_RT("") );
	m_pCloseBtn->SetVisible( TRUE );

	m_pSortBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  175, 241, 87, 25, false );        
	m_pSortBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pSortBtn->SetFontR( n_fRsvFontWndButton );   
	m_pSortBtn->SetText( G_STRING_CHAR( IDS_USERSTORE_SORT ) );				// 정렬
	m_pSortBtn->SetVisible( TRUE );

	//...........................................................................................................
	// 스크롤
	//...........................................................................................................
	m_pVScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None , m_pFrame->GetID() ,  366,  62, 13, 175, false );
	m_pVScroll->InitSlide( stVertSld, 0, ( USER_STORE_SELL_ITEM_SAVE_MAX_COUNT - USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ) / 2 , 0, 13 );//Type, min, max, value, 썸의 폭
	m_pVScroll->SetClientImage( TID_CTRL_SCR_BG );
	m_pVScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pVScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pVScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
}

void CUserStoreBuyWnd::SetSlotLabel( int slot , int price , int count )
{
	for( int i = 0 ; i < 4 ; ++i )
		m_TextBox[ slot ][ i ]->SetVisible( TRUE );

	char TempStr[ 256 ] = "";
	
	Rsprintf( RWCHAR(TempStr) , _RT("%d") , count );
	m_TextBox[ slot ][ 0 ]->SetText( TempStr );
	//Rsprintf( RWCHAR(TempStr ), _RT("%d") , price );
	InventoryWnd.SetEldText(TempStr , price);
	m_TextBox[ slot ][ 1 ]->SetText( TempStr );
}

void CUserStoreBuyWnd::SetSlotVisible( int slot , BOOL Visible )
{
	for( int i = 0 ; i < 4 ; ++i )
		m_TextBox[ slot ][ i ]->SetVisible( Visible );
}

void CUserStoreBuyWnd::Update()
{
}

void CUserStoreBuyWnd::Proc( SEventMessage* EMsg )
{
	//...........................................................................................................
	// 취소 버튼
	//...........................................................................................................
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_pFrame->SetVisible( FALSE );
			g_UserStoreSetBuyWnd.Close();
	
			m_IsVisible = FALSE;
			m_UserStoreBuyMode = FALSE;
			m_ShopPCID = 0;
			IsVisible			= FALSE;
		}
	}
	//...........................................................................................................
	// 확인 버튼
	//...........................................................................................................
	else if( EMsg->FocusWnd == m_pStartBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			//m_pFrame->SetVisible( FALSE );
			//m_IsVisible = FALSE;
			//IsVisible			= FALSE;
			
			// 1 : 1 대화창 열기
			g_MCommunityWnd.OpenMessenOneChatWnd( m_Name );
		}	
	}
	//...........................................................................................................
	// 소트 버튼
	//...........................................................................................................
	else if( EMsg->FocusWnd == m_pSortBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SortSlot();
			UpdateSlot();
		}	
	}
	else if( EMsg->FocusWnd == m_pVScroll->GetID() ) 
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			m_TopSlotIndex = EMsg->fParam;

			UpdateSlot();
		}
	}
	else
	{
		for( int i = 0 ; i < USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ; ++i )
		{
			if( m_pItemSlotList[ i ]->GetID() == EMsg->FocusWnd )
			{
				if( EMsg->Notify == NM_SLOT_FOCUS )			{	StoreBuySlotFocus( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_PRESS )	{	StoreBuySlotFocus( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_CLICK )	{	StoreBuySlotClick( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	StoreBuySlotDBClick( EMsg );	}
				else if( EMsg->Notify == NM_SLOT_DRAG )		{	StoreBuySlotDrag( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DROP )		{	StoreBuySlotDrop( EMsg );		}
			
				break;
			}
		}
	}
}

void CUserStoreBuyWnd::StoreBuySlotFocus(SEventMessage*  EMsg )	
{
	static CSlot* cslot = NULL;
	
	cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );

	if( ( cslot->PcItem != NULL ) && ( nRui->GetCursorItem() == NULL ) )
	{
		nRui->theCursorState = n_CT_HAND;
	}
}

void CUserStoreBuyWnd::StoreBuySlotClick(SEventMessage*  EMsg )	
{
	int			idx		= 0;
	CSlot*		cslot	= NULL;	// 현재 클릭된 슬롯
	CSlot*		pslot	= NULL;	// 이전 선택 된 슬롯
	SPcItem*	tmpItem	= NULL;

	//......................................................................................................
	// 커서에 붙어있는 아이템 있을때
	//......................................................................................................
	if( nRui->GetCursorItem() != NULL )
	{
		nRui->ResetCursorItem();
	}
	//......................................................................................................
	// 커서에 붙어 있는 아이템 없을때
	//......................................................................................................
	else
	{
		//커서에 아이템 붙이기 메세지.
		cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );

		nRui->SetCursorItem( cslot->PcItem );
		nRui->SetItemSelectedSlot( cslot );

		nRui->theCursorState = n_CT_ITEMDROP;
	}
}

void CUserStoreBuyWnd::StoreBuySlotDBClick(SEventMessage*  EMsg )
{
	CSlot* cslot = (CSlot*)Container->Get( EMsg->FocusWnd );

	// 더블클릭한 위치에 아이템이 있을때
	if( cslot->PcItem != NULL )
	{
		// 사기창 열기
		g_UserStoreSetBuyWnd.SetItemInfo( cslot->PcItem , m_PriceList[ cslot->SlotNum + ( m_TopSlotIndex * 2 ) ] );
	}
}

void CUserStoreBuyWnd::StoreBuySlotDrag(SEventMessage*  EMsg )	
{
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );
	
	if( ( slot->PcItem != NULL ) && ( EMsg->FocusWnd != (WNDID)EMsg->fParam ) )
	{
		//커서에 아이템 붙이기

		if( nRui->GetCursorItem() == NULL )
		{
			nRui->SetCursorItem( slot->PcItem );
			nRui->SetItemSelectedSlot( slot );
		}
	}
}

void CUserStoreBuyWnd::StoreBuySlotDrop(SEventMessage*  EMsg )	
{
	CSlot* cslot = (CSlot*)Container->Get( EMsg->FocusWnd );
	CSlot* tslot = (CSlot*)Container->Get( (WNDID)EMsg->fParam );
	
	if( ((WNDID)EMsg->fParam != WID_None ) && ( tslot->GetWindowType() == n_wtSlot ) )
	{
		if( cslot == tslot )
		{
			nRui->ResetCursorItem();
		}
		else if( cslot->PcItem != NULL )
		{
			switch( tslot->Group )
			{
				case n_SGROUP_INVENTORY:
					// 사기 창 열기
					g_UserStoreSetBuyWnd.SetItemInfo( cslot->PcItem , m_PriceList[ cslot->SlotNum + ( m_TopSlotIndex * 2 ) ] );
					g_UserStoreSetBuyWnd.Open();
					nRui->ResetCursorItem();
					break;

				default:
					nRui->ResetCursorItem();
					break;
			}
		}
	}
}

void CUserStoreBuyWnd::UpdateSlot(void)
{
	for( int i = 0; i < USER_STORE_SELL_ITEM_SLOT_MAX_COUNT; ++i )
	{
		if( ( ( m_TopSlotIndex * 2 ) + i ) >= USER_STORE_SELL_ITEM_SAVE_MAX_COUNT )
			__asm int 3

		m_pItemSlotList[ i ]->PcItem = &m_WarehousePcItem[ ( m_TopSlotIndex * 2 ) + i ];
	
		if( m_pItemSlotList[ i ]->PcItem->Amount )
		{
			SetSlotVisible( i , TRUE );
		
			SetSlotLabel( i , m_PriceList[ ( m_TopSlotIndex * 2 ) + i ] , m_pItemSlotList[ i ]->PcItem->Amount );
		}
		else
			SetSlotVisible( i , FALSE );
	}
}

void CUserStoreBuyWnd::SortSlot(void)
{
	SPcItem		TempPcItem[ USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ];
	int			TempPriceList[ USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ];

	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		TempPcItem[ i ].Clear();
		TempPriceList[ i ] = 0;
	}

	int Count = 0;

	for(int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		if( m_WarehousePcItem[ i ].Amount )
		{
			TempPcItem[ Count ]    = m_WarehousePcItem[ i ];
			TempPriceList[ Count ] = m_PriceList[ i ];
		
			Count++;
		}
	}

	for(int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		m_WarehousePcItem[ i ].Clear();
		m_PriceList[ i ] = 0;
	}

	for(int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		if( TempPcItem[ i ].Amount )
		{
			m_WarehousePcItem[ i ] = TempPcItem[ i ];
			m_PriceList[ i ]	   = TempPriceList[ i ];
		}
	}
}


void CUserStoreBuyWnd::Close(void)
{
	m_pFrame->SetVisible( FALSE );
	
	m_IsVisible = FALSE;
	m_UserStoreBuyMode = FALSE;
	IsVisible			= FALSE;
	g_UserStoreSetBuyWnd.Close();
}

void CUserStoreBuyWnd::ItemClear(void)
{
	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		m_WarehousePcItem[ i ].Clear();
		m_PriceList[ i ] = 0;
	}
}

void CUserStoreBuyWnd::SetItemList(char *pName,char *pContext,int Num,URpdItem *pItemList,DWORD *m_pPriceList)
{
	ItemClear();
	
	m_pStoreNameText->SetText( pName );
	m_pStoreContextText->SetText( pContext );
	
	int i = 0;

	for( i = 0; i < Num ; ++i )
	{	
		SPcItem* addPcItem;
		addPcItem = &m_WarehousePcItem[i];
		
		m_PriceList[ i ] = ( int ) m_pPriceList[ i ];

		WORD ItemType = g_Pc_Manager.GetItemClsss( pItemList[i].theItem.theCode );
		if( ( ItemType >= 0 ) && ( ItemType <= 8 ) )
		{
			addPcItem->IType		= n_ITYPE_WEAR;
			addPcItem->Amount		= 1;
			
			memcpy( &(addPcItem->WearTable), &pItemList[i], sizeof( URpdItem ) );
		}
		else if( ItemType == 9 )
		{
			addPcItem->IType	= n_ITYPE_USE;//소모품
			addPcItem->Amount	= pItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == 10 )
		{
			addPcItem->IType	= n_ITYPE_UNUSE;//비소모품
			addPcItem->Amount	= pItemList[i].theWasteItem.theBundle;
		}
#ifdef ITEM6331_BUG_TEST
		else if( ItemType == 11 || ItemType == 14 || ItemType == 21 || ItemType == 22 )
#else 
		else if( ItemType == 11 || ItemType == 14 )
#endif //ITEM6331_BUG_TEST
		{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= pItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_PersonalPet )
		{
			addPcItem->IType	= n_ITYPE_PET_USE; 
			addPcItem->Amount	= pItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			addPcItem->IType	= n_ITYPE_RIDE; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &pItemList[i], sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{
			addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			addPcItem->Amount	= 1;

			memcpy( &(addPcItem->WearTable), &pItemList[i], sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{
			addPcItem->IType = n_ITYPE_SKIN_WEAR;
			addPcItem->Amount	= 1;
			memcpy( &(addPcItem->WearTable), &pItemList[i] , sizeof( URpdItem ) );
		}

		
		addPcItem->Class		= ItemType;
		addPcItem->ItemUID		= pItemList[i].theItem.theItemUID;
		addPcItem->Code			= pItemList[i].theItem.theCode;
		addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( pItemList[i].theItem.theCode );
		addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( pItemList[i].theItem.theCode );
		
		// 왼손,오른손....
		//---------------------------------------------------------------------
		if( ( addPcItem->ItemTable->theType == n_Type_MagicBook ) || ( addPcItem->ItemTable->theType == n_Type_Bow ) || ( addPcItem->ItemTable->theType == n_Type_Shield ) )
		{
			addPcItem->Class = n_Class_Shield;
		}
		else if( ( addPcItem->ItemTable->theType == n_Type_Sword ) || ( addPcItem->ItemTable->theType == n_Type_TwohandSword ) ||
			( addPcItem->ItemTable->theType == n_Type_Staff ) || ( addPcItem->ItemTable->theType == n_Type_CrystalBall ) )
		{
			addPcItem->Class = n_Class_Weapon;
		}
		//---------------------------------------------------------------------					
	}
	
	return;
}


void CUserStoreBuyWnd::ChangeItem(DWORD ItemID,BYTE Count)
{
	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		if( m_WarehousePcItem[ i ].ItemUID == ItemID )
		{
			if( Count == 0 )
			{
				m_WarehousePcItem[ i ].Clear();
			}
			else
			{
				m_WarehousePcItem[ i ].Amount = Count;
			}

			break;
		}
	}
	
	UpdateSlot();
}