 #include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "..\\..\\brother.h"
#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
//#include "pc.h"

#include "..\\..\\bravolibs\\Sound\\Sound.h"

#include "PayItemWndProc.h"
#include "MainMenuWndProc.h"
#include "StoreWndProc.h"
#include "NumberInputWnd.h"
#include "ChatBoxWndProc.h"
#include "CDelayBarWnd.h"
#include "InventoryWndProc.h"
#include "ExchangeWndProc.h"
#include "stringmanager.h"
#include "ExchangeWndProc.h"


CPayItemWnd		g_PayItemWnd;


extern long		g_nowTime;



//-----------------------------------------------------------------------------
void CPayItemWnd::Init()
{
	m_StartSlotIndex = 0;

//	m_SrcSlot = NULL;			
//	m_TargetSlot = NULL;

	return;	
}

//-----------------------------------------------------------------------------
void CPayItemWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CButton*		btn = NULL;
	CStateButton*	sbtn = NULL;	
	CImageBox*		img;

	SRect			WndRect;
 
	WndRect.w = 220;
	WndRect.h = 275;
	WndRect.x = 250; //center_x - ( WndRect.w / 2 );
	WndRect.y = 284; //245; //center_y - ( WndRect.h / 2 );
	
	///-- Frame Wnd
	m_FrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_PAYITEM_WND, WID_None, WndRect.x, WndRect.y, WndRect.w, WndRect.h, false );
	m_FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_FrameWnd->SetWndTileSizeLR( 40, 29 );
	m_FrameWnd->SetWndTileSizeTB( 31, 11 );
	m_FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_WAREHOUSE ) );	// 구매 아이템 보관창 
	m_FrameWnd->SetTitle( true );
	
	m_FrameWnd->SetWndProc( this );	
	m_FrameWnd->SetVisible( false );
	m_FrameWnd->Initialize();
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PAYITEM_WND, 1, 50, 218, 198, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PAYITEM_WND, 1, 49, 218, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PAYITEM_WND, 1, 250, 218, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- Item Slots
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PAYITEM_WND, 11, 56, 183, 187, false );    
	img->SetClientImage( TID_C_BGSLOT_INVEN );	

	///-- Elda Text
//	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PAYITEM_WND, 36, 255, 132, 23, false );      
//	img->SetClientImage( TID_CTRL_IMG_ELDTEXT132 );

	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PAYITEM_WND, 195, 6, 13, 13, false );   
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
		
	m_ItemSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  11, 56, 35, 35, false );
	m_ItemSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  48, 56, 35, 35, false );
	m_ItemSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  85, 56, 35, 35, false );
	m_ItemSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND, 122, 56, 35, 35, false );
	m_ItemSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND, 159, 56, 35, 35, false );
	m_ItemSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  11, 94, 35, 35, false );
	m_ItemSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  48, 94, 35, 35, false );
	m_ItemSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  85, 94, 35, 35, false );
	m_ItemSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND, 122, 94, 35, 35, false );
	m_ItemSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND, 159, 94, 35, 35, false );
	m_ItemSlot[10] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  11, 132, 35, 35, false ); 
	m_ItemSlot[11] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  48, 132, 35, 35, false ); 
	m_ItemSlot[12] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  85, 132, 35, 35, false ); 
	m_ItemSlot[13] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND, 122, 132, 35, 35, false ); 
	m_ItemSlot[14] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND, 159, 132, 35, 35, false ); 
	m_ItemSlot[15] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  11, 170, 35, 35, false ); 
	m_ItemSlot[16] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  48, 170, 35, 35, false ); 
	m_ItemSlot[17] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  85, 170, 35, 35, false ); 
	m_ItemSlot[18] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND, 122, 170, 35, 35, false ); 
	m_ItemSlot[19] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND, 159, 170, 35, 35, false ); 
	m_ItemSlot[20] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  11, 208, 35, 35, false ); 
	m_ItemSlot[21] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  48, 208, 35, 35, false ); 
	m_ItemSlot[22] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND,  85, 208, 35, 35, false ); 
	m_ItemSlot[23] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND, 122, 208, 35, 35, false ); 
	m_ItemSlot[24] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PAYITEM_WND, 159, 208, 35, 35, false ); 

	m_ItemSlot[0]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 0 );
	m_ItemSlot[0]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[1]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 1 );
	m_ItemSlot[1]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[2]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 2 );
	m_ItemSlot[2]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[3]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 3 );
	m_ItemSlot[3]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[4]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 4 );
	m_ItemSlot[4]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[5]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 5 );
	m_ItemSlot[5]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[6]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 6 );
	m_ItemSlot[6]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[7]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 7 );
	m_ItemSlot[7]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[8]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 8 );
	m_ItemSlot[8]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[9]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 9 );
	m_ItemSlot[9]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[10]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 10 );
	m_ItemSlot[10]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[11]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 11 );
	m_ItemSlot[11]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[12]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 12 );
	m_ItemSlot[12]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[13]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 13 );
	m_ItemSlot[13]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[14]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 14 );
	m_ItemSlot[14]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[15]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 15 );
	m_ItemSlot[15]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[16]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 16 );
	m_ItemSlot[16]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[17]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 17 );
	m_ItemSlot[17]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[18]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 18 );
	m_ItemSlot[18]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[19]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 19 );
	m_ItemSlot[19]->SetHID( HID_ITEMTIP );
	m_ItemSlot[20]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 20 );
	m_ItemSlot[20]->SetHID( HID_ITEMTIP );
	m_ItemSlot[21]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 21 );
	m_ItemSlot[21]->SetHID( HID_ITEMTIP );
	m_ItemSlot[22]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 22 );
	m_ItemSlot[22]->SetHID( HID_ITEMTIP );
	m_ItemSlot[23]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 23 );
	m_ItemSlot[23]->SetHID( HID_ITEMTIP );
	m_ItemSlot[24]->SetSlotGroup( n_SGROUP_PAYITEM_WND, 24 );
	m_ItemSlot[24]->SetHID( HID_ITEMTIP );
		
	m_pScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None, WID_PAYITEM_WND, 200, 56, 15, 190, false );
	m_pScrollBar->InitSlide( stVertSld, 0, 45, 0, 13 );//Type, min, max, value, 썸의 폭
	m_pScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	m_pScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	//초기화
	Init();

	UpdatePayItemPcItem();

	return;
}

//-----------------------------------------------------------------------------
void CPayItemWnd::Update()
{	
	return;
}

void CPayItemWnd::UpdatePayItemPcItem()
{
	for( int i = 0; i < c_MaxPayItemDrawNum; ++i )
	{
		m_ItemSlot[i]->PcItem = &m_PayPcItem[m_StartSlotIndex + i];
	}

	return;
}


//-----------------------------------------------------------------------------
void CPayItemWnd::Proc( SEventMessage* EMsg )
{
	if( m_pCloseBtn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ClosePayItemWnd();			
		}
	}

	if( m_pScrollBar->GetID() == EMsg->FocusWnd )
	{	
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || 
			( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetSlotScroll( (int)EMsg->fParam );
		}
	}

	//-------------------------------------------------------------------------
	if( m_ItemSlot[0]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[1]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[2]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[3]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[4]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[5]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[6]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[7]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[8]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[9]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[10]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[11]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[12]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[13]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[14]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[15]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[16]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[17]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[18]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[19]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[20]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[20]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[21]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[22]->GetID() == EMsg->FocusWnd || 
		m_ItemSlot[23]->GetID() == EMsg->FocusWnd ||
		m_ItemSlot[24]->GetID() == EMsg->FocusWnd )
	{
		if( g_ExchangeWnd.m_bIsExchangeLock == FALSE )
		{
			if( EMsg->Notify == NM_SLOT_FOCUS )			{	PayItemSlotFocus( EMsg );		}
			else if( EMsg->Notify == NM_SLOT_PRESS )	{	PayItemSlotFocus( EMsg );		}
			else if( EMsg->Notify == NM_SLOT_CLICK )	{	PayItemSlotClick( EMsg );		}
			else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	PayItemSlotDBClick( EMsg );		}
			else if( EMsg->Notify == NM_SLOT_DRAG )		{	PayItemSlotDrag( EMsg );		}
			else if( EMsg->Notify == NM_SLOT_DROP )		{	PayItemSlotDrop( EMsg );		}
		}
	}

	return;
}


//-----------------------------------------------------------------------------
void CPayItemWnd::SetWheelValue( short _wheel )
{
	int value = 0;
	int nMax = 0;

	m_pScrollBar->GetValue( &value );
	nMax = m_pScrollBar->GetSldMax();

	if( ( _wheel < 0 ) && ( value < nMax ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	m_pScrollBar->SetValue( value );
	SetSlotScroll( value );
}

//-----------------------------------------------------------------------------
void CPayItemWnd::SetSlotScroll( int linenum )
{
	m_StartSlotIndex = linenum * 5;
	
	for( int i = 0; i < c_MaxPayItemDrawNum; ++i )
	{
		m_ItemSlot[i]->PcItem = &m_PayPcItem[m_StartSlotIndex + i];
	}

	return;
}


void CPayItemWnd::PayItemSlotFocus( SEventMessage* EMsg )
{
	static CSlot* tslot = NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
	if( ( tslot->PcItem != NULL ) && ( nRui->GetCursorItem() == NULL ) )
	{
		if( tslot->PcItem->IType != n_ITYPE_NONE )
		{
			nRui->theCursorState = n_CT_HAND;
		}
	}

	return;
}

//-----------------------------------------------------------------------------
void CPayItemWnd::PayItemSlotClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	// 클릭된 교환창 슬롯
	CSlot*		sslot	= NULL;	// 선택된(pick) 아이템 슬롯

	if( nRui->GetCursorItem() != NULL )
	{
		tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		sslot = nRui->GetItemSelectedSlot();
		if( sslot->PcItem == NULL )
		{
			return;
		}
		
		if( tslot->PcItem && (tslot->PcItem == sslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		else
		{				
			if( sslot->Group )
			{
				nRui->ResetCursorItem();
			}			
		}
	}
	else
	{		
		//커서에 아이템 붙이기 메세지.
		tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );

		if( tslot->PcItem->ItemTable )
		{
			nRui->SetCursorItem( tslot->PcItem );
			nRui->SetItemSelectedSlot( tslot );

			nRui->theCursorState = n_CT_ITEMDROP;	
		}
	}

	return;
}

//-----------------------------------------------------------------------------
void CPayItemWnd::PayItemSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();
	
	// 자동 인벤에 넣기 처리 
//	DWORD			thePayItemID;	// PayItemList중에서 제거될 아이템 
//	URpdItem		theItem;		// 인벤에 옮겨진 아이템의 정보 



		
/*	if( tslot->PcItem->IType != n_ITYPE_NONE )
	{
		if( tslot->PcItem->ItemTable->theBundle != 1 && tslot->PcItem->Amount > 1 ) 
		{
			g_NumberInputWnd.Open( n_NUMBERINPUT_WAREHOUSE_INVENTORY, tslot->PcItem );			
		}
		else
		{
			g_RockClient.Send_RpCsDraw( tslot->PcItem->ItemUID, tslot->PcItem->Amount );
		}

		nRui->ResetCursorItem();

//		tslot->PcItem->Clear();
	}
*/
	return;
}

//-----------------------------------------------------------------------------
void CPayItemWnd::PayItemSlotDrag( SEventMessage* EMsg )
{
	int idx = 0;
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );
	
	if( ( slot->PcItem != NULL ) && ( EMsg->FocusWnd != (WNDID)EMsg->fParam ) && ( slot->PcItem->IType != n_ITYPE_NONE ) )
	{
		if( slot->PcItem->IType != n_ITYPE_NONE )
		{
			nRui->theCursorState = n_CT_ITEMDROP;

			//커서에 아이템 붙이기
			if( nRui->GetCursorItem() == NULL )
			{
				nRui->SetCursorItem( slot->PcItem );
				nRui->SetItemSelectedSlot( slot );
			}
		}
	}

	return;
}

//-----------------------------------------------------------------------------
void CPayItemWnd::PayItemSlotDrop( SEventMessage* EMsg )
{
	CSlot* sslot = (CSlot*)Container->Get( EMsg->FocusWnd );		// 들고있는 Src Slot
	CSlot* tslot = (CSlot*)Container->Get( (WNDID)EMsg->fParam );	// 내려놓는 Target Slot
	
	if( ((WNDID)EMsg->fParam != WID_None ) && ( tslot->GetWindowType() == n_wtSlot ) )
	{
		if( sslot == tslot )
		{
			nRui->ResetCursorItem();
		}
		else if( ( sslot->PcItem != NULL ) && ( sslot->PcItem->IType != n_ITYPE_NONE ) ) 
		{
			if( sslot->Group )
			{
				switch( sslot->Group )
				{
				case n_SGROUP_PAYITEM_WND:
					{	
						g_RockClient.Send_CsGetPayItem( sslot->PcItem->ItemUID );
					}
					break;
				}

				nRui->ResetCursorItem();
			}			
		}
	}
	return;
}


void CPayItemWnd::SetPayItemWndPcItem( SPayItem aItemList )
{
	SItemBaseInfo*	item_info = NULL;	
	item_info = g_Pc_Manager.GetItemBaseInfoPtr( aItemList.thePayItemCode );
 
	if( item_info )
	{
		int i = 0;

		for( i = 0; i < c_MaxPayPcItemNum; ++i )
		{
			if( m_PayPcItem[i].ItemUID == aItemList.thePayItemID )	
			{
				return;		// 무시 ? 아마도 갱신될 일 없으니 그냥 무시~ ㅋㅋ				 
//				break;;		// 갱신 ? 
			}

			if( m_PayPcItem[i].ItemUID == 0 )
			{
				SPcItem* addPcItem = NULL;
				
				addPcItem = &m_PayPcItem[i];
				
				WORD ItemType = g_Pc_Manager.GetItemClsss( aItemList.thePayItemCode );
				if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
				{
					addPcItem->IType		= n_ITYPE_WEAR;
					addPcItem->Amount		= 1;
				}
				else if( ItemType == n_Class_Use )
				{
					addPcItem->IType	= n_ITYPE_USE;			//소모품
					addPcItem->Amount	= item_info->theBundle;
				}
				else if( ItemType == n_Class_Useless )
				{
					addPcItem->IType	= n_ITYPE_UNUSE;		//비소모품
					addPcItem->Amount	= item_info->theBundle;
				}
				else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
					|| ItemType == n_Class_FrontierPet || ItemType == n_Class_Weapon_StrengthStone 
					|| ItemType == n_Class_Armor_StrengthStone )
				{
									
					addPcItem->IType	= n_ITYPE_SPECIAL; 
					addPcItem->Amount	= 1;
				}
				else if( ItemType == n_Class_PersonalPet )
				{
					addPcItem->IType	= n_ITYPE_PET_USE; 
					addPcItem->Amount	= item_info->theBundle;
				}
				else if( ItemType == n_Class_Riding )
				{
					addPcItem->IType	= n_ITYPE_RIDE; 
					addPcItem->Amount	= 1;
				}
				else if( ItemType == n_Class_RidingItem )
				{
					addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
					addPcItem->Amount	= 1;
				}
				else if( ItemType == n_Class_Skin )
				{
					addPcItem->IType = n_ITYPE_SKIN_WEAR;
					addPcItem->Amount	= 1;
				}
						

				addPcItem->Class		= ItemType;
				addPcItem->ItemUID		= aItemList.thePayItemID;
				addPcItem->Code			= aItemList.thePayItemCode;
				addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aItemList.thePayItemCode );
				addPcItem->ItemTable	= item_info;
				
				addPcItem->WearTable.theEndurance = item_info->theMaxEndurance;
				
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
				break;
			}
		}
	}
	
	return;
}


void CPayItemWnd::DeletePcItem( DWORD aPayItemID )
{
	for( int i = 0; i < c_MaxPayPcItemNum; ++i )
	{
		if( m_PayPcItem[i].ItemUID == aPayItemID ) 
		{
			m_PayPcItem[i].Clear();
			break;
		}
	}

	return;
}


void CPayItemWnd::Open()
{
	m_FrameWnd->Reposition( 250, 284 );
	InventoryWnd.FrameWnd->Reposition( 480, 284 );

	nRui->SendEvent( WID_StoreWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );	
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
	InventoryWnd.Open();
#else
	nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
	nRui->SendEvent( WID_PAYITEM_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	m_StartSlotIndex = 0;

//	m_SrcSlot = NULL;			
//	m_TargetSlot = NULL;

	UpdatePayItemPcItem();

	return;
}

void CPayItemWnd::LogOff()
{
	for( int i = 0; i < c_MaxPayPcItemNum; ++i )
	{
		m_PayPcItem[i].Clear();
	}

	ClosePayItemWnd();

	return;
}

void CPayItemWnd::ClosePayItemWnd()
{
	nRui->SendEvent( WID_PAYITEM_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );			
	nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장
	nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );//유료인벤확장
#endif
	return;
}

void CPayItemWnd::ResetCursorPcItem()
{
	InventoryWnd.ResetWearSlotHighlight();									// 장비 슬롯의 하이라이트를 끈다....
		
	if( nRui->thePcItem != NULL )											// NULL이 들어오는건 아이템이 떨어지는것으로 간주한다...
	{
		if( nRui->thePcItem->IType != n_ITYPE_NONE )
		{
			if( nRui->thePcItem->IType == n_ITYPE_SKILL )
			{
				g_Sound.Play( 28 );											// 스킬 놓기 사운드
			}
			else
			{
				g_Sound.Play( nRui->thePcItem->ItemTable->theSoundFile );	// 떨어뜨릴때는 아이템 고유 사운드 출력
			}		
		}
	}
	return;
}

/*
void CPayItemWnd::CloseNumberInputWnd( int aNumItem )
{
	if( !m_SrcSlot )
	{
		return;
	}

	if( m_SrcSlot->PcItem )
	{
		if( m_SrcSlot->PcItem->Amount >= aNumItem )
		{
			g_RockClient.Send_RpCsDeposit( m_SrcSlot->PcItem->ItemUID, aNumItem, m_StartSlotIndex + m_TargetSlot->SlotNum );

//			m_TargetSlot->SetSlotPcItem( *(m_SrcSlot->PcItem) );				
//			m_TargetSlot->PcItem->Amount = aNumItem;
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_NUM ) );
		}
	}

	
	return;
}
*/