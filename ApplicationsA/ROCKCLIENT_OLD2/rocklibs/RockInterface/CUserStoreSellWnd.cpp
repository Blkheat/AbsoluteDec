#include "Rui.h"
#include "CUserStoreSellWnd.h"
#include "SystemPrintWnd.h"
#include "CUserStoreSetWnd.h"
#include "CUserStoreTitleWnd.h"
#include "InventoryWndProc.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\stringmanager.h"
#include "..\\..\\brother.h"
#include "..\\..\\RockClient.h"


#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\Map.h"

CUserStoreSellWnd g_UserStoreSellWnd;


CUserStoreSellWnd::CUserStoreSellWnd()
{
	m_pFrame    = NULL;
	m_pVScroll  = NULL;
	m_pCloseBtn = NULL;
	m_pStartBtn = NULL;
	
	for( int i = 0 ; i <USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ; ++i )
	{
		m_TextBox[USER_STORE_SELL_ITEM_SLOT_MAX_COUNT][0] = NULL;
		m_TextBox[USER_STORE_SELL_ITEM_SLOT_MAX_COUNT][1] = NULL;
	}
	
	ResetItemList();
	
	m_IsVisible = FALSE;
	IsVisible			= FALSE;
	
	m_UserStoreSellMode = FALSE;
	m_UserStoreSellOpenMode = FALSE;
}

CUserStoreSellWnd::~CUserStoreSellWnd()
{
}

void CUserStoreSellWnd::ResetItemList(void)
{
	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ; ++i ) 	
	{
		m_pItemSlotList[ i ] = NULL;
		
		if( NULL != m_TextBox[ i ][0] )
		{
			m_TextBox[ i ][ 1 ]->SetText(_RT("") );
		}
		
		if( NULL != m_TextBox[ i ][1] )
		{
			m_TextBox[ i ][ 1 ]->SetText(_RT("") );
		}
	}
	
	m_TopSlotIndex = 0;
}

void CUserStoreSellWnd::ItemClear(void)
{
	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i ) 	
	{
		m_WarehousePcItem[ i ].Clear();
		m_PriceList[ i ] = 0;
	}
}

void CUserStoreSellWnd::SetWheelValue( short _wheel )
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

int CUserStoreSellWnd::GetItemCountInIventory(DWORD UID)
{
	// 인벤토리 처리
	int Count = 0;
	int i     = 0;
	
	for( i = 0; i < RP_MAX_EQUIP; ++i )
	{
		if( ( g_RockClient.m_PcInven->WearItem[i] != NULL ) && ( g_RockClient.m_PcInven->WearItem[i]->ItemUID == UID ) )
		{
			Count = g_RockClient.m_PcInven->WearItem[ i ]->Amount;
			
			return Count;
		}
	}
	
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{
		if( ( g_RockClient.m_PcInven->InvenItem[i] != NULL ) && ( g_RockClient.m_PcInven->InvenItem[i]->ItemUID == UID ) )
		{
			Count = g_RockClient.m_PcInven->InvenItem[i]->Amount;
			
			return Count;
		}
	}
	
	return Count;
}

void CUserStoreSellWnd::Open(void)
{
	if( NULL == m_pFrame )
		return;
	
	//......................................................................................................
	// 전에 올려놓은 아이템이 있다면 인벤토리 창에서 검색후 개수 조정
	//......................................................................................................
	int Count = 0;
	
	for( int i = 0; i < USER_STORE_SELL_ITEM_SLOT_MAX_COUNT; ++i )
	{
		if( m_WarehousePcItem[ i ].Amount > 0 )
		{
			Count = GetItemCountInIventory( m_WarehousePcItem[ i ].ItemUID );
			
			if( Count == 0 )
			{
				m_WarehousePcItem[ i ].Clear();
				m_PriceList[ i ] = 0;
			}
			else if( m_WarehousePcItem[ i ].Amount > Count )
			{
				m_WarehousePcItem[ i ].Amount = Count;
			}
		}
		else
		{
			m_WarehousePcItem[ i ].Clear();
			m_PriceList[ i ] = 0;
		}
	}
	
	//......................................................................................................
	
	m_ISSellLockMode = FALSE;
	
	m_pStartBtn->SetBtnHold( FALSE );
	m_pCloseBtn->SetBtnHold( FALSE );
	
	m_pStartBtn->SetText( G_STRING_CHAR( IDS_USERSTORE_START ) );				// 영업개시
	
	
	UpdateSlot();
	
	m_pFrame->SetVisible( TRUE );
	
	m_IsVisible = TRUE;
	IsVisible			= TRUE;
	
	m_pStartBtn->SetEnable( TRUE );
	m_pCloseBtn->SetEnable( TRUE );
	m_UserStoreSellMode = TRUE;
}

void CUserStoreSellWnd::Composition()
{
	int center_x	= 1024.0f / 2.0f;
	int center_y	= 768.0f  / 2.0f;
	
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
	m_pFrame->SetCaption( G_STRING_CHAR( IDS_USERSTORE_NAME ) );					// 개인상점
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
		m_TextBox[i][2]->SetText( G_STRING_CHAR( IDS_USERSTORE_COUNT ) );		// 수량
		m_TextBox[i][2]->SetTextColor( 255 , 255 , 255 );
		
		m_TextBox[i][3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), ptPrice[ i ].x , ptPrice[ i ].y , 34, 11, false );        
		m_TextBox[i][3]->SetFontR( n_fRsvFontWndStatic );
		m_TextBox[i][3]->SetAlignText( n_atLeft, n_atCenter );
		m_TextBox[i][3]->SetText( G_STRING_CHAR( IDS_USERSTORE_PRICE ) );		// 가격
		m_TextBox[i][3]->SetTextColor( 255 , 255 , 255 );
		
		m_TextBox[i][0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), ptCount2[ i ].x , ptCount2[ i ].y , 59, 11, false );        
		m_TextBox[i][0]->SetFontR( n_fRsvFontWndStatic );
		m_TextBox[i][0]->SetAlignText( n_atRight, n_atCenter );
		m_TextBox[i][0]->SetText(_RT("") );		
		m_TextBox[i][0]->SetTextColor( 255 , 255 , 255 );
		
		m_TextBox[i][1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), ptPrice2[ i ].x , ptPrice2[ i ].y , 59, 11, false );        
		m_TextBox[i][1]->SetFontR( n_fRsvFontWndStatic );
		m_TextBox[i][1]->SetAlignText( n_atRight, n_atCenter );
		m_TextBox[i][1]->SetText(_RT("") );		
		m_TextBox[i][1]->SetTextColor( 255 , 255 , 255 );
		
		m_pItemSlotList[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrame->GetID() , ptItem[ i ].x + 3 , ptItem[ i ].y + 3 , 35, 35, false );
		m_pItemSlotList[i]->PcItem = NULL;
		m_pItemSlotList[i]->SetSlotGroup( n_SGROUP_USER_STORE_SELL , i );
		m_pItemSlotList[i]->SetHID( HID_ITEMTIP );	
	}
	
	m_pStoreNameText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 12, 30, 346, 11, false );        
	m_pStoreNameText->SetFontR( n_fRsvFontWndStatic );
	m_pStoreNameText->SetAlignText( n_atLeft, n_atCenter );
	m_pStoreNameText->SetText(_RT("") );
	m_pStoreNameText->SetTextColor( 0 , 0 , 0 );
	
	m_pStoreContextText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrame->GetID(), 20, 46, 338, 11, false );        
	m_pStoreContextText->SetFontR( n_fRsvFontWndStatic );
	m_pStoreContextText->SetAlignText( n_atLeft, n_atCenter );
	m_pStoreContextText->SetText(_RT("") );
	m_pStoreContextText->SetTextColor( 0 , 0 , 0 );
	
	//...........................................................................................................
	// 버튼
	//...........................................................................................................

	SRect sTempRect;

	sTempRect.x	= 272;
	sTempRect.y	= 241;
	sTempRect.w	= 87;
	sTempRect.h	= 25;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x = 222;
	sTempRect.w	= 137;	

#endif

	m_pStartBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  
											sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );        
	m_pStartBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pStartBtn->SetFontR( n_fRsvFontWndButton );   
	m_pStartBtn->SetText( G_STRING_CHAR( IDS_USERSTORE_START ) );				// 영업개시
	m_pStartBtn->SetVisible( TRUE );	

	sTempRect.x	= 175;
	sTempRect.y	= 241;
	sTempRect.w	= 87;
	sTempRect.h	= 25;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 75;
	sTempRect.w	= 137;	

#endif	
	
	m_pSortBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  
											sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	m_pSortBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pSortBtn->SetFontR( n_fRsvFontWndButton );   
	m_pSortBtn->SetText( G_STRING_CHAR( IDS_USERSTORE_SORT ) );				// 정렬
	m_pSortBtn->SetVisible( TRUE );
	
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pFrame->GetID() ,  360, 7, 13, 13, false ); 
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X , TID_CTRL_BTN_X_OVR , TID_CTRL_BTN_X_CLK , TID_CTRL_BTN_X_DIS );
	m_pCloseBtn->SetFontR( n_fRsvFontWndButton );   
	m_pCloseBtn->SetText(_RT("") );
	m_pCloseBtn->SetVisible( TRUE );
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

void CUserStoreSellWnd::SetSlotLabel( int slot , int price , int count )
{
	for( int i = 0 ; i < 4 ; ++i )
		m_TextBox[ slot ][ i ]->SetVisible( TRUE );
	
	char TempStr[ 256 ] = {0,};
	
	Rsprintf( RWCHAR(TempStr) , _RT("%d") , count );
	m_TextBox[ slot ][ 0 ]->SetText( TempStr );
	//Rsprintf( RWCHAR(TempStr) , _RT("%d") , (DWORD)price );
	InventoryWnd.SetEldText(TempStr , price);
	m_TextBox[ slot ][ 1 ]->SetText( TempStr );
}

void CUserStoreSellWnd::SetSlotVisible( int slot , BOOL Visible )
{
	for( int i = 0 ; i < 4 ; ++i )
		m_TextBox[ slot ][ i ]->SetVisible( Visible );
}

void CUserStoreSellWnd::Update()
{
	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ; ++i )
	{
		if( m_UserStoreSellOpenMode == TRUE )
		{
			m_pItemSlotList[i]->theSlotState = n_Slot_Disable;
		}
		else
		{
			m_pItemSlotList[i]->theSlotState = n_Slot_Enable;
		}
	}
}

void CUserStoreSellWnd::Proc( SEventMessage* EMsg )
{
	//...........................................................................................................
	// 취소 버튼
	//...........................................................................................................
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_pFrame->SetVisible( FALSE );
			
			m_IsVisible = FALSE;
			m_UserStoreSellMode = FALSE;
			IsVisible	= FALSE;
			
			if( m_ISSellLockMode == TRUE )
			{
				g_RockClient.Send_CsClosePCShop();
			}
			
			g_UserStoreSetWnd.Close();
		}
	}
	//...........................................................................................................
	// 확인 버튼
	//...........................................................................................................
	else if( EMsg->FocusWnd == m_pStartBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			//			m_pFrame->SetVisible( FALSE );
			//			m_IsVisible = FALSE;
			if( ISEmptyItem() )
			{
				return;
			}
			
			if( m_ISSellLockMode == FALSE )
			{
				m_pStartBtn->SetText( G_STRING_CHAR( IDS_USERSTORE_END ) );
				
				// 상점 창을 열기 서버에 보내요
				g_RockClient.Send_CsOpenPCShop();
				m_ISSellLockMode = !m_ISSellLockMode;
			}
			else
			{
				m_ISSellLockMode = !m_ISSellLockMode;
				g_RockClient.Send_CsClosePCShop();
				
				g_UserStoreSellWnd.m_pStoreNameText->SetText( g_UserStoreTitleWnd.m_StoreNameStr );
				g_UserStoreSellWnd.m_pStoreContextText->SetText( g_UserStoreTitleWnd.m_StoreContextStr );
				
				g_UserStoreSellWnd.Open();

				// 스킬 시전이 아니라 개인상점창에 관련된 패킷처리(차후 정확한 사용의도 확인 필요)
				g_RockClient.Send_RpCsSkill( g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetUnique() ,
					n_PC , c_SkillCode_UserSellStore );
			}
			
			
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
	else if( EMsg->FocusWnd == m_pSortBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SortSlot();
			UpdateSlot();
		}
	}
	else 
	{
		if( m_UserStoreSellOpenMode == FALSE )
		{
			for( int i = 0 ; i < USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ; ++i )
			{
				if( m_pItemSlotList[ i ]->GetID() == EMsg->FocusWnd )
				{
					if( EMsg->Notify == NM_SLOT_FOCUS )			{	StoreSellSlotFocus( EMsg );		}
					else if( EMsg->Notify == NM_SLOT_PRESS )	{	StoreSellSlotFocus( EMsg );		}
					else if( EMsg->Notify == NM_SLOT_CLICK )	{	StoreSellSlotClick( EMsg );		}
					else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	StoreSellSlotDBClick( EMsg );	}
					else if( EMsg->Notify == NM_SLOT_DRAG )		{	StoreSellSlotDrag( EMsg );		}
					else if( EMsg->Notify == NM_SLOT_DROP )		{	StoreSellSlotDrop( EMsg );		}
					
					break;
				}
			}
		}
	}
}

void CUserStoreSellWnd::StoreSellSlotFocus(SEventMessage* EMsg)	
{
	static CSlot* cslot = NULL;
	
	cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
	
	if( ( cslot->PcItem != NULL ) && ( nRui->GetCursorItem() == NULL ) )
	{
		nRui->theCursorState = n_CT_HAND;
	}
}

void CUserStoreSellWnd::StoreSellSlotClick(SEventMessage* EMsg)
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
		cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		pslot = nRui->GetItemSelectedSlot();
		
		// 선택된 슬록에 아무것도 없을때 
		if( pslot->PcItem == NULL )
		{
			return;
		}
		
		//인벤과 웨어 슬롯상의 유료 아이템일경우
		if( n_SGROUP_WEAR == pslot->Group || n_SGROUP_INVENTORY == pslot->Group )
		{
			
#ifdef ITEM_LIMIT_FUNC
			BYTE aItemLimitFunc = 0;

			if( pslot->PcItem->ItemTable )
			{
				aItemLimitFunc = pslot->PcItem->ItemTable->theItemLimitFunc;
				
				///-- 가공 장신구는 팔 수 없다.
				BYTE aClass = pslot->PcItem->ItemTable->theClass;
				if( n_Class_Accessory == aClass )
				{
					if( pslot->PcItem->WearTable.theEldaStone )
					{
						SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_SELLITEM ) );					
						nRui->ResetCursorItem();						
						return;
					}
				}
			}

		#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

			if(pslot->PcItem->IsItemCode60000Above())
			{
				SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_SELLITEM ) );					
				nRui->ResetCursorItem();
				return;
			}
			else
			{
				if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotSell ) )
				{
					SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_SELLITEM ) );					
					nRui->ResetCursorItem();				
					return;
				}
			}

		#else

			if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotSell ) )
			{
				SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_SELLITEM ) );					
				nRui->ResetCursorItem();				
				return;
			}

		#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

		#ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

			if(pslot->PcItem->IsItemCode12180_12188_Above())
			{
				SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_SELLITEM ) );					
				nRui->ResetCursorItem();
				return;
			}
			else
			{
				if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotSell ) )
				{
					SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_SELLITEM ) );					
					nRui->ResetCursorItem();				
					return;
				}
			}
			
		#else
			
			if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotSell ) )
			{
				SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_SELLITEM ) );					
				nRui->ResetCursorItem();				
				return;
			}

		#endif// #ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

#endif
			
			///- 선택 아이템이 유료 아이템일 경우			
			WORD wCode = pslot->PcItem->ItemTable->theCode;
			if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max) )
			{
				
#ifdef  C_DROP_WASTEPAYITEM
				if( pslot->PcItem->IType != n_ITYPE_USE)
				{
				#ifdef HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707

					if(pslot->PcItem->Class != n_Class_Use &&
						pslot->PcItem->Class != n_Class_FrontierPet &&
						pslot->PcItem->Class != n_Class_PersonalPet)
					{
						SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
						nRui->ResetCursorItem();						
						return;						
					}

				#else

					SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
					nRui->ResetCursorItem();						
					return;						

				#endif// #ifdef HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707
				}
#else
				SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
				nRui->ResetCursorItem();	
				return;		
#endif
							
			}		
		}		
		
		// 선택된 슬롯과 타겟슬롯이 같을때 커서 초기화
		if( cslot->PcItem && (cslot->PcItem == pslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		else
		{
			if( n_SGROUP_USER_STORE_SELL == pslot->Group )
			{
				nRui->ResetCursorItem();
			}
			else
			{
				InsertItem( cslot , pslot );
			}
		}
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

void CUserStoreSellWnd::StoreSellSlotDBClick(SEventMessage* EMsg)
{
}

void CUserStoreSellWnd::StoreSellSlotDrag(SEventMessage* EMsg)
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

void CUserStoreSellWnd::StoreSellSlotDrop(SEventMessage* EMsg)	
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
			//인벤과 웨어 슬롯상의 유료 아이템일경우
			if( n_SGROUP_WEAR == tslot->Group || n_SGROUP_INVENTORY == tslot->Group )
			{
				
#ifdef ITEM_LIMIT_FUNC
				BYTE aItemLimitFunc = 0;

				if( tslot->PcItem->ItemTable )
				{
					aItemLimitFunc = tslot->PcItem->ItemTable->theItemLimitFunc;

					///-- 가공 장신구는 팔 수 없다.
					BYTE aClass = tslot->PcItem->ItemTable->theClass;
					if( n_Class_Accessory == aClass )
					{
						if( tslot->PcItem->WearTable.theEldaStone )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_SELLITEM ) );					
							nRui->ResetCursorItem();						
							return;
						}
					}
				}

				if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotSell ) )
				{
					SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_SELLITEM ) );					
					nRui->ResetCursorItem();				
					return;
				}			
#endif					
				///- 선택 아이템이 유료 아이템일 경우			
				if( tslot->PcItem && tslot->PcItem->ItemTable )
				{
					WORD wCode = tslot->PcItem->ItemTable->theCode;
					if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max) )
					{

#ifdef  C_DROP_WASTEPAYITEM
						if(tslot->PcItem->IType != n_ITYPE_USE )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
							nRui->ResetCursorItem();						
							return;						
						}
#else
						SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
						nRui->ResetCursorItem();						
						return;		
#endif

					}
				}
				else
				{
					///-- 메모리가 널이면....
					nRui->ResetCursorItem();
					return;
				}
			}			
			
			switch( tslot->Group )
			{
			case n_SGROUP_INVENTORY:
			case n_SGROUP_WEAR:
				g_UserStoreSellWnd.DeleteItem( cslot );
				break;
				
			case n_SGROUP_USER_STORE_SELL:
				nRui->ResetCursorItem();
				break;
			}
		}
	}
}

void CUserStoreSellWnd::UpdateSlot(void)
{
	for( int i = 0; i < USER_STORE_SELL_ITEM_SLOT_MAX_COUNT; ++i )
	{
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

BOOL CUserStoreSellWnd::ISFindItem(SPcItem *pItem)
{
	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		if( m_WarehousePcItem[ i ].Amount )
		{
			if( m_WarehousePcItem[ i ].ItemUID == pItem->ItemUID )
				return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CUserStoreSellWnd::ISEmptyItem(void)
{
	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		if( m_WarehousePcItem[ i ].Amount )
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

void CUserStoreSellWnd::InsertItem(CSlot *cslot,CSlot *pslot)
{
	if( pslot->Group == n_SGROUP_INVENTORY || pslot->Group == n_SGROUP_WEAR )
	{
#ifdef CREATE_CHARACTER_EVENT_ITEMSELL 
		if( pslot->PcItem->Code >= 12180  &&  pslot->PcItem->Code <= 12188 ) // 케릭터 신규생성 이벤트 아이템 개인상점 판매금지
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) ); 
			return;
		}
#endif //CREATE_CHARACTER_EVENT_ITEMSELL
		if( ISFindItem( pslot->PcItem ) )
		{
			// 있는아이템 다시넣을때
			g_SystemPrintWnd.InsertSystemMsg( RCHAR(G_STRING_CHAR( IDS_USERSTORE_REINSERT )) , (UINT)n_LightBlue);			
		}
		else
		{
			// 내구도 맥스치만 
			if( pslot->PcItem->IType == n_ITYPE_WEAR )
			{
				if( pslot->PcItem->WearTable.theEndurance != pslot->PcItem->ItemTable->theMaxEndurance )
				{
					g_SystemPrintWnd.InsertSystemMsg( RWCHAR(G_STRING_CHAR( IDS_USERSTORE_REPAIR_INSERT )), (UINT)n_LightBlue);
					
					nRui->ResetCursorItem();
					
					return;	
				}
			}
			
			m_SaveSrcItem  = pslot->PcItem;
			
			m_SaveSlotNum  = cslot->SlotNum + ( m_TopSlotIndex * 2 );
			
			g_UserStoreSetWnd.SetItemInfo( pslot->PcItem , pslot->PcItem->Amount );
			g_UserStoreSetWnd.Open();
			//nRui->SetModal( g_UserStoreSetWnd.m_pFrame->GetID() );
			
			nRui->SendEvent( g_UserStoreSetWnd.m_pFrame->GetID() , n_emSetFocus, 0, 0, 0, 0 );
		}
		
		nRui->ResetCursorItem();
	}
}

void CUserStoreSellWnd::DeleteItem(CSlot *cslot)
{
	m_WarehousePcItem[ cslot->SlotNum + ( m_TopSlotIndex * 2 ) ].Clear();
	
	nRui->ResetCursorItem();
	
	UpdateSlot();
}

void CUserStoreSellWnd::InsertSaveItem(int Price,int Count)
{
	m_WarehousePcItem[ m_SaveSlotNum ]        = *m_SaveSrcItem;
	m_WarehousePcItem[ m_SaveSlotNum ].Amount = Count;
	m_PriceList 	 [ m_SaveSlotNum ]        = Price;
	UpdateSlot();
}

void CUserStoreSellWnd::UserStoreStart(void)
{
	//	m_pFrame->SetVisible( FALSE );
	//	m_IsVisible = FALSE;
}

void CUserStoreSellWnd::SortSlot()
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
			m_PriceList[ i ]       = TempPriceList[ i ];
		}
	}
}

void CUserStoreSellWnd::Close(void)
{
	m_pFrame->SetVisible( FALSE );
	
	IsVisible			= FALSE;
	m_IsVisible = FALSE;
	m_UserStoreSellMode = FALSE;
	m_UserStoreSellOpenMode = FALSE;
	g_UserStoreSetWnd.Close();
}

void CUserStoreSellWnd::SellInMyStore(DWORD PCID,DWORD Eld,DWORD ItemID,BYTE Count,int *pSubValue)
{
	DWORD ItemCode;
	
	for( int i = 0 ; i < USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ; ++i )
	{
		if( m_WarehousePcItem[ i ].ItemUID == ItemID )
		{
			ItemCode = m_WarehousePcItem[ i ].Code;
			
			DWORD ItemPrice = m_PriceList[ i ];

			if( Count == 0 )
			{
				*pSubValue = m_WarehousePcItem[ i ].Amount;
				
				m_WarehousePcItem[ i ].Clear();
				m_PriceList[ i ] = 0;
			}
			else
			{
				*pSubValue = m_WarehousePcItem[ i ].Amount - Count;
				
				m_WarehousePcItem[ i ].Amount = Count;
			}
		
			UpdateSlot();
			
			// System Message
			SItemBaseInfo *pItem = g_Pc_Manager.GetItemBaseInfoPtr( ItemCode );
			
			if( pItem == NULL )
				return;
			
			int PcIndex = g_Pc.FindUser( PCID );
			
			if( PcIndex == MAX_USER_INTABLE )
				return;
					
			char TempStr[ 1024 ] = {0,};
						
			Eld = ItemPrice * (*pSubValue);

			Rsprintf( RWCHAR(TempStr) , G_STRING_CHAR( IDS_USERSTORE_SELL_MSG ) , 
				RWCHAR(g_Pc.m_PcTable[ PcIndex ].char_info.thePCName ),
				RWCHAR(pItem->theName) , Eld );
			
			SystemPrint( n_DarkRed, n_SystemMsg , TempStr );
			
			return;
		}
		
	}
	
	*pSubValue = 0;
}
