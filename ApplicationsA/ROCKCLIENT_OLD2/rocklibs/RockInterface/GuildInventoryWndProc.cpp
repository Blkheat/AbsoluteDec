#include "RockPCH.h"
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

#include "GuildInventoryWndProc.h"
#include "MainMenuWndProc.h"
#include "StoreWndProc.h"
#include "NumberInputWnd.h"
#include "ChatBoxWndProc.h"
#include "CDelayBarWnd.h"
#include "InventoryWndProc.h"
#include "ExchangeWndProc.h"
#include "stringmanager.h"

#ifdef DECO_RENEWAL_GUILD_MJH
CGuildInvenWnd	g_GuildInvenWnd;

extern long		g_nowTime;



//-----------------------------------------------------------------------------
void CGuildInvenWnd::Init()
{
	m_GuildInvenState = n_GUILDINVEN_SHOP1;

	m_StartSlotIndex = 0;

	m_SrcSlot = NULL;			
	m_TargetSlot = NULL;

	return;	
}

//-----------------------------------------------------------------------------
void CGuildInvenWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CButton*		btn = NULL;
	CStateButton*	sbtn = NULL;	
	CImageBox*		img;

	SRect			WndRect;
 
	WndRect.w = 204;
	WndRect.h = 258;
	WndRect.x = 250; //center_x - ( WndRect.w / 2 );
	WndRect.y = 394; //245; //center_y - ( WndRect.h / 2 );
	
	///-- Frame Wnd
	m_FrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_GuildInven_Wnd, WID_None, WndRect.x, WndRect.y, WndRect.w, WndRect.h, false );
	m_FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_FrameWnd->SetWndTileSizeLR( 40, 29 );
	m_FrameWnd->SetWndTileSizeTB( 31, 11 );
	m_FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_GUILD ) );
	m_FrameWnd->SetTitle( true );
	
	m_FrameWnd->SetWndProc( this );	
	m_FrameWnd->SetVisible( false );
	m_FrameWnd->Initialize();
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_GuildInven_Wnd, 1, 50, 202, 198, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_GuildInven_Wnd, 1, 49, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_GuildInven_Wnd, 1, 250, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- Item Slots
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_GuildInven_Wnd, 11, 56, 183, 187, false );    
	img->SetClientImage( TID_C_BGSLOT_INVEN );	

	///-- Elda Text
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_GuildInven_Wnd, 36, 255, 132, 23, false );      
	img->SetClientImage( TID_CTRL_IMG_ELDTEXT132 );
	img->SetVisible( false );

	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_GuildInven_CloseBtn, WID_GuildInven_Wnd, 185, 6, 13, 13, false );   
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- Elda Button
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_GuildInven_DrawOutEldBtn, WID_GuildInven_Wnd, 13, 255, 23, 23, false );     
	btn->SetBtnImage( TID_CTRL_BTN_ELD, TID_CTRL_BTN_ELD_OVR, TID_CTRL_BTN_ELD_CLK, TID_None );
	btn->SetVisible( false );

	char temp[300];
	
	sbtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_GuildInven_Box1StateBtn, WID_GuildInven_Wnd, 7, 31, 38, 19, false );	
	sbtn->SetBtnImage( TID_CTRL_TAB38_DIS, TID_CTRL_TAB38_DIS, TID_CTRL_TAB38, TID_None );
	sbtn->SetFontR( n_fRsvFontWndButton );
	Rsprintf(RWCHAR(temp), _RT("%s%d"), G_STRING_CHAR( IDS_WND_WAREHOUSE_BOX ), 1);
	sbtn->SetText( temp ); 

	sbtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_GuildInven_Box2StateBtn, WID_GuildInven_Wnd, 45, 31, 38, 19, false );	
	sbtn->SetBtnImage( TID_CTRL_TAB38_DIS, TID_CTRL_TAB38_DIS, TID_CTRL_TAB38, TID_None );
	sbtn->SetFontR( n_fRsvFontWndButton );
	Rsprintf(RWCHAR(temp), _RT("%s%d"), G_STRING_CHAR( IDS_WND_WAREHOUSE_BOX ), 2);
	sbtn->SetText( temp ); 

	sbtn->SetText( temp ); 

	nRui->SetGroupWnd( 2, WID_GuildInven_Box1StateBtn, WID_GuildInven_Box1StateBtn, WID_GuildInven_Box2StateBtn );

	m_ItemSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot0, WID_GuildInven_Wnd,  11, 56, 35, 35, false );
	m_ItemSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot1, WID_GuildInven_Wnd,  48, 56, 35, 35, false );
	m_ItemSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot2, WID_GuildInven_Wnd,  85, 56, 35, 35, false );
	m_ItemSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot3, WID_GuildInven_Wnd, 122, 56, 35, 35, false );
	m_ItemSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot4, WID_GuildInven_Wnd, 159, 56, 35, 35, false );
	m_ItemSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot5, WID_GuildInven_Wnd,  11, 94, 35, 35, false );
	m_ItemSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot6, WID_GuildInven_Wnd,  48, 94, 35, 35, false );
	m_ItemSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot7, WID_GuildInven_Wnd,  85, 94, 35, 35, false );
	m_ItemSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot8, WID_GuildInven_Wnd, 122, 94, 35, 35, false );
	m_ItemSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot9, WID_GuildInven_Wnd, 159, 94, 35, 35, false );
	m_ItemSlot[10] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot10, WID_GuildInven_Wnd,  11, 132, 35, 35, false ); 
	m_ItemSlot[11] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot11, WID_GuildInven_Wnd,  48, 132, 35, 35, false ); 
	m_ItemSlot[12] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot12, WID_GuildInven_Wnd,  85, 132, 35, 35, false ); 
	m_ItemSlot[13] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot13, WID_GuildInven_Wnd, 122, 132, 35, 35, false ); 
	m_ItemSlot[14] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot14, WID_GuildInven_Wnd, 159, 132, 35, 35, false ); 
	m_ItemSlot[15] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot15, WID_GuildInven_Wnd,  11, 170, 35, 35, false ); 
	m_ItemSlot[16] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot16, WID_GuildInven_Wnd,  48, 170, 35, 35, false ); 
	m_ItemSlot[17] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot17, WID_GuildInven_Wnd,  85, 170, 35, 35, false ); 
	m_ItemSlot[18] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot18, WID_GuildInven_Wnd, 122, 170, 35, 35, false ); 
	m_ItemSlot[19] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot19, WID_GuildInven_Wnd, 159, 170, 35, 35, false ); 
	m_ItemSlot[20] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot20, WID_GuildInven_Wnd,  11, 208, 35, 35, false ); 
	m_ItemSlot[21] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot21, WID_GuildInven_Wnd,  48, 208, 35, 35, false ); 
	m_ItemSlot[22] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot22, WID_GuildInven_Wnd,  85, 208, 35, 35, false ); 
	m_ItemSlot[23] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot23, WID_GuildInven_Wnd, 122, 208, 35, 35, false ); 
	m_ItemSlot[24] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_GuildInven_ItemSlot24, WID_GuildInven_Wnd, 159, 208, 35, 35, false ); 

	m_ItemSlot[0]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 0 );
	m_ItemSlot[0]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[1]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 1 );
	m_ItemSlot[1]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[2]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 2 );
	m_ItemSlot[2]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[3]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 3 );
	m_ItemSlot[3]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[4]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 4 );
	m_ItemSlot[4]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[5]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 5 );
	m_ItemSlot[5]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[6]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 6 );
	m_ItemSlot[6]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[7]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 7 );
	m_ItemSlot[7]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[8]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 8 );
	m_ItemSlot[8]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[9]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 9 );
	m_ItemSlot[9]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[10]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 10 );
	m_ItemSlot[10]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[11]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 11 );
	m_ItemSlot[11]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[12]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 12 );
	m_ItemSlot[12]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[13]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 13 );
	m_ItemSlot[13]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[14]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 14 );
	m_ItemSlot[14]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[15]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 15 );
	m_ItemSlot[15]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[16]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 16 );
	m_ItemSlot[16]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[17]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 17 );
	m_ItemSlot[17]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[18]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 18 );
	m_ItemSlot[18]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[19]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 19 );
	m_ItemSlot[19]->SetHID( HID_ITEMTIP );
	m_ItemSlot[20]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 20 );
	m_ItemSlot[20]->SetHID( HID_ITEMTIP );
	m_ItemSlot[21]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 21 );
	m_ItemSlot[21]->SetHID( HID_ITEMTIP );
	m_ItemSlot[22]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 22 );
	m_ItemSlot[22]->SetHID( HID_ITEMTIP );
	m_ItemSlot[23]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 23 );
	m_ItemSlot[23]->SetHID( HID_ITEMTIP );
	m_ItemSlot[24]->SetSlotGroup( n_SGROUP_GUILDINVENTORY, 24 );
	m_ItemSlot[24]->SetHID( HID_ITEMTIP );
	
	

	//초기화
	Init();

	UpdateGuildInvenPcItem();

	return;
}

//-----------------------------------------------------------------------------
void CGuildInvenWnd::Update()
{
	

	return;
}

void CGuildInvenWnd::UpdateGuildInvenPcItem()
{
	for( int i = 0; i < c_MaxGuildInvenItemDrawNum; ++i )
	{
		m_ItemSlot[i]->PcItem = &m_GuildInvenPcItem[m_StartSlotIndex + i];
	}

	return;
}


//-----------------------------------------------------------------------------
void CGuildInvenWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_GuildInven_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			CloseGuildInvenWnd();			
		}
		break;
	case WID_GuildInven_Box1StateBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetGuildInvenState( n_GUILDINVEN_SHOP1 );
		}
		break;
	case WID_GuildInven_Box2StateBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetGuildInvenState( n_GUILDINVEN_SHOP2 );
		}
		break;
	//-------------------------------------------------------------------------
	case WID_GuildInven_ItemSlot0:
	case WID_GuildInven_ItemSlot1:
	case WID_GuildInven_ItemSlot2:
	case WID_GuildInven_ItemSlot3:
	case WID_GuildInven_ItemSlot4:
	case WID_GuildInven_ItemSlot5:
	case WID_GuildInven_ItemSlot6:
	case WID_GuildInven_ItemSlot7:
	case WID_GuildInven_ItemSlot8:
	case WID_GuildInven_ItemSlot9:
	case WID_GuildInven_ItemSlot10:
	case WID_GuildInven_ItemSlot11:
	case WID_GuildInven_ItemSlot12:
	case WID_GuildInven_ItemSlot13:
	case WID_GuildInven_ItemSlot14:
	case WID_GuildInven_ItemSlot15:
	case WID_GuildInven_ItemSlot16:
	case WID_GuildInven_ItemSlot17:
	case WID_GuildInven_ItemSlot18:
	case WID_GuildInven_ItemSlot19:
	case WID_GuildInven_ItemSlot20:
	case WID_GuildInven_ItemSlot21:
	case WID_GuildInven_ItemSlot22:
	case WID_GuildInven_ItemSlot23:
	case WID_GuildInven_ItemSlot24:
		{
			if( g_ExchangeWnd.m_bIsExchangeLock == FALSE )
			{
				if( EMsg->Notify == NM_SLOT_FOCUS )			{	GuildInvenSlotFocus( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_PRESS )	{	GuildInvenSlotFocus( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_CLICK )	{	GuildInvenSlotClick( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	GuildInvenSlotDBClick( EMsg );	}
				else if( EMsg->Notify == NM_SLOT_DRAG )		{	GuildInvenSlotDrag( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DROP )		{	GuildInvenSlotDrop( EMsg );		}
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------
void CGuildInvenWnd::SetGuildInvenState( N_GUILDINVENSTATE state )
{
	int i = 0;
	m_GuildInvenState = state;
	
	// 실제 슬롯의 위치 
	m_StartSlotIndex = m_GuildInvenState * c_MaxGuildInvenItemDrawNum;

	if( m_GuildInvenState == n_GUILDINVEN_SHOP1 )
	{	
		for( i = 0; i < c_MaxGuildInvenItemDrawNum; ++i )
		{
			m_ItemSlot[i]->PcItem = &m_GuildInvenPcItem[m_StartSlotIndex + i];
		}
	}

	UpdateGuildInvenPcItem();

	return;
}

void CGuildInvenWnd::GuildInvenSlotFocus( SEventMessage* EMsg )
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
void CGuildInvenWnd::GuildInvenSlotClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	// 클릭된 교환창 슬롯
	CSlot*		sslot	= NULL;	// 선택된(pick) 아이템 슬롯

	if(!nRui->m_KeyLock.GuildInven.GetCheckTime(g_nowTime))
		return;
	

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
				switch( sslot->Group )
				{
				case n_SGROUP_WEAR:
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_WAREHOUSE_WEAR ) );
						nRui->ResetCursorItem();
					}
					break;
				case n_SGROUP_INVENTORY:
					{							

#ifdef ITEM_LIMIT_FUNC
						BYTE aItemLimitFunc = 0;

						if( sslot->PcItem->ItemTable )
						{
							aItemLimitFunc = sslot->PcItem->ItemTable->theItemLimitFunc;
							
							///-- 가공 장신구는 보관 할수 없다.
							BYTE aClass = sslot->PcItem->ItemTable->theClass;
							if( n_Class_Accessory == aClass )
							{
								if( sslot->PcItem->WearTable.theEldaStone )
								{
									SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_KEEPITEM ) );					
									nRui->ResetCursorItem();									
									return;
								}
							}
						}

					#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625
						
						if(sslot->PcItem->IsItemCode60000Above())
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_KEEPITEM ) );					
							nRui->ResetCursorItem();
							return;
						}
						else
						{
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotKeep ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_KEEPITEM ) );					
								nRui->ResetCursorItem();				
								return;
							}
						}

					#else

						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotKeep ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_KEEPITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}
						
					#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625


					#ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

						if(sslot->PcItem->IsItemCode12180_12188_Above())
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_KEEPITEM ) );					
							nRui->ResetCursorItem();
							return;
						}
						else
						{
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotKeep ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_KEEPITEM ) );					
								nRui->ResetCursorItem();				
								return;
							}
						}
						
					#else
						
						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotKeep ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_KEEPITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}

					#endif// #ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

#endif						
						
						if( sslot->PcItem->ItemTable->theBundle != 1 && sslot->PcItem->Amount > 1 ) 
						{
							g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_GUILDINVEN, sslot->PcItem );
							m_SrcSlot = sslot->SlotAddr;			
							m_TargetSlot = tslot;
							nRui->ResetCursorItem();
						}
						else
						{	
							//길드창고 보관함 메세지전송
#ifdef DECO_RENEWAL_GUILD_MJH
							g_RockClient.Send_RpCsDepositFrontier( sslot->PcItem->ItemUID, sslot->PcItem->Amount, m_StartSlotIndex + tslot->SlotNum );
#endif // DECO_RENEWAL_GUILD_MJH	
//							tslot->SetSlotPcItem( *(sslot->PcItem) );				
							nRui->ResetCursorItem();						
						}
					}
					break;
				case n_SGROUP_GUILDINVENTORY:
					{
						if( tslot->PcItem->IType == n_ITYPE_NONE )
						{	
							//길드창고 보관함 메세지전송
#ifdef DECO_RENEWAL_GUILD_MJH
							g_RockClient.Send_RpCsChangeFrontierSlot( sslot->PcItem->ItemUID, m_StartSlotIndex + tslot->SlotNum );
#endif // DECO_RENEWAL_GUILD_MJH
//							tslot->SetSlotPcItem( *(sslot->PcItem) );				
							ResetCursorPcItem();						
//							sslot->SlotAddr->PcItem->Clear();	
							nRui->thePcItem = NULL;
							nRui->SetItemSelectedSlot( NULL );
						}
						else
						{
							//길드창고 보관함 메세지전송
#ifdef DECO_RENEWAL_GUILD_MJH
							g_RockClient.Send_RpCsChangeFrontierSlot( sslot->PcItem->ItemUID, m_StartSlotIndex + tslot->SlotNum );
#endif // DECO_RENEWAL_GUILD_MJH
//							SPcItem		pSrctmpItem;
//							pSrctmpItem = *(sslot->PcItem);
///							SPcItem		pTargettmpItem;
//							pTargettmpItem = *(tslot->PcItem);
							
//							tslot->SetSlotPcItem( pSrctmpItem );				
							ResetCursorPcItem();			
//							sslot->SlotAddr->SetSlotPcItem( pTargettmpItem );							
							nRui->thePcItem = NULL;
							nRui->SetItemSelectedSlot( NULL );
						}
					}
					break;
				default:
					{
						nRui->ResetCursorItem();
					}
					break;
				}
			}			
		}
	}
	else
	{		
		//커서에 아이템 붙이기 메세지.
		tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );

		if( tslot->PcItem->IType != n_ITYPE_NONE )
		{
			nRui->SetCursorItem( tslot->PcItem );
			nRui->SetItemSelectedSlot( tslot );

			nRui->theCursorState = n_CT_ITEMDROP;	
		}
	}

	return;
}

//-----------------------------------------------------------------------------
void CGuildInvenWnd::GuildInvenSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();	
	
	if( tslot->PcItem->IType != n_ITYPE_NONE )
	{
		if( tslot->PcItem->ItemTable->theBundle != 1 && tslot->PcItem->Amount > 1 ) 
		{
			g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_GUILDINVEN, tslot->PcItem );			
		}
		else
		{
            //길드창고 보관함 메세지전송
#ifdef DECO_RENEWAL_GUILD_MJH
			g_RockClient.Send_RpCsDrawFrontier( tslot->PcItem->ItemUID, tslot->PcItem->Amount );
#endif // DECO_RENEWAL_GUILD_MJH
		}
		nRui->ResetCursorItem();

//		tslot->PcItem->Clear();
	}

	return;
}

//-----------------------------------------------------------------------------
void CGuildInvenWnd::GuildInvenSlotDrag( SEventMessage* EMsg )
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
void CGuildInvenWnd::GuildInvenSlotDrop( SEventMessage* EMsg )
{
	CSlot* sslot = (CSlot*)Container->Get( EMsg->FocusWnd );		// 들고있는 Src Slot
	CSlot* tslot = (CSlot*)Container->Get( (WNDID)EMsg->fParam );	// 내려놓는 Target Slot
	
	if(!nRui->m_KeyLock.GuildInven.GetCheckTime(g_nowTime))
		return;

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
				case n_SGROUP_WEAR:
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_WAREHOUSE_WEAR ) );
						nRui->ResetCursorItem();
					}
					break;
				case n_SGROUP_INVENTORY:
					{							
						
#ifdef ITEM_LIMIT_FUNC
						BYTE aItemLimitFunc = 0;

						if( sslot->PcItem->ItemTable ) 
						{
							aItemLimitFunc = sslot->PcItem->ItemTable->theItemLimitFunc;

							///-- 가공 장신구는 보관 할수 없다.
							BYTE aClass = sslot->PcItem->ItemTable->theClass;
							if( n_Class_Accessory == aClass )
							{
								if( sslot->PcItem->WearTable.theEldaStone )
								{
									SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_KEEPITEM ) );					
									nRui->ResetCursorItem();									
									return;
								}
							}
						}

						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotKeep ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_KEEPITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}			
#endif					
						if( sslot->PcItem->ItemTable->theBundle != 1 && sslot->PcItem->Amount > 1 ) 
						{
							g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_GUILDINVEN, tslot->PcItem );
							m_SrcSlot = sslot->SlotAddr;			
							m_TargetSlot = tslot;
							nRui->ResetCursorItem();
						}
						else
						{	
							//길드창고 보관함 메세지전송
#ifdef DECO_RENEWAL_GUILD_MJH
							g_RockClient.Send_RpCsDepositFrontier( sslot->PcItem->ItemUID, sslot->PcItem->Amount, m_StartSlotIndex + tslot->SlotNum );
#endif // DECO_RENEWAL_GUILD_MJH
//							tslot->SetSlotPcItem( *(sslot->PcItem) );				
							nRui->ResetCursorItem();						
						}
					}
					break;
				case n_SGROUP_GUILDINVENTORY:
					{
						if( tslot->Group == n_SGROUP_EXCHANGE )
						{
							if( tslot->PcItem->IType == n_ITYPE_NONE )
							{	
								//길드창고 보관함 메세지전송
#ifdef DECO_RENEWAL_GUILD_MJH
								g_RockClient.Send_RpCsChangeFrontierSlot( sslot->PcItem->ItemUID, m_StartSlotIndex + tslot->SlotNum );
#endif // DECO_RENEWAL_GUILD_MJH
//								tslot->SetSlotPcItem( *(sslot->PcItem) );											
								ResetCursorPcItem();							
//								sslot->PcItem->Clear();		
								nRui->thePcItem = NULL;
								nRui->SetItemSelectedSlot( NULL );
							}
							else
							{
								//길드창고 보관함 메세지전송
#ifdef DECO_RENEWAL_GUILD_MJH
								g_RockClient.Send_RpCsChangeFrontierSlot( sslot->PcItem->ItemUID, m_StartSlotIndex + tslot->SlotNum );
#endif // DECO_RENEWAL_GUILD_MJH

//								SPcItem		pSrctmpItem;
//								pSrctmpItem = *(sslot->PcItem);
//								SPcItem		pTargettmpItem;
//								pTargettmpItem = *(tslot->PcItem);
								
//								tslot->SetSlotPcItem( pSrctmpItem );				
								ResetCursorPcItem();			
//								sslot->SetSlotPcItem( pTargettmpItem );							
								nRui->thePcItem = NULL;
								nRui->SetItemSelectedSlot( NULL );								
							}
						}
					}
					break;
				default:
					{
						nRui->ResetCursorItem();
					}
					break;
				}
			}
			
		}
	}
	return;
}


void CGuildInvenWnd::SetGuildInvenPcItem(  BYTE aItemNum, URpdItem aItemList[] )
{
	int i = 0;
	int ItemPos = 0; 

	for( i = 0; i < aItemNum; ++i )
	{	
		ItemPos = aItemList[i].theItem.thePosition;
		
		SPcItem* addPcItem;
		addPcItem = &m_GuildInvenPcItem[ItemPos];
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( aItemList[i].theItem.theCode );
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
		{
			addPcItem->IType		= n_ITYPE_WEAR;
			addPcItem->Amount		= 1;
			
			memcpy( &(addPcItem->WearTable), &aItemList[i], sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			addPcItem->IType	= n_ITYPE_USE;//소모품
			addPcItem->Amount	= aItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			addPcItem->IType	= n_ITYPE_UNUSE;//비소모품
			addPcItem->Amount	= aItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
		{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= aItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_PersonalPet )
		{
			addPcItem->IType	= n_ITYPE_PET_USE; 
			addPcItem->Amount	= aItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			addPcItem->IType	= n_ITYPE_RIDE; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &aItemList[i], sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{	
			addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			addPcItem->Amount	= 1;

			memcpy( &(addPcItem->WearTable), &aItemList[i], sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{	
			addPcItem->IType	= n_ITYPE_SKIN_WEAR; 
			addPcItem->Amount	= 1;
			memcpy( &(addPcItem->WearTable), &aItemList[i], sizeof( URpdItem ) );
		}
		
		


		addPcItem->Class		= ItemType;
		addPcItem->ItemUID		= aItemList[i].theItem.theItemUID;
		addPcItem->Code			= aItemList[i].theItem.theCode;
		addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aItemList[i].theItem.theCode );
		addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aItemList[i].theItem.theCode );
		
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
		
#ifdef C_TIME_SUIT_UPDATE		
		if(addPcItem->ItemTable &&  addPcItem->ItemTable->theRare == 255 && addPcItem->ItemTable->theMaxEndurance == 0 )
		{
			DWORD dwTime = 0 ; 
			for( int j = 0; j < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++j )
			{
				if(nRui->GetPcInvenInfo()->PayTimeItemList[j] == NULL)
				{
					//두곳 장착과 인벤 아이템 들어가 있는것. . 
					nRui->GetPcInvenInfo()->PayTimeItemList[j] = SAFE_NEW( SPayTimeItem );
					nRui->GetPcInvenInfo()->PayTimeItemList[j]->theItemCode = aItemList[i].theItem.theCode;
					nRui->GetPcInvenInfo()->PayTimeItemList[j]->theItemID = aItemList[i].theItem.theItemUID;
					memcpy(&dwTime , &aItemList[i].theWearItem.theEndurance , sizeof(DWORD));
					nRui->GetPcInvenInfo()->PayTimeItemList[j]->theTime =dwTime ;
					break;
				}
			}	
		}
#endif





	}
	
	return;
}

void CGuildInvenWnd::Open()
{
	nRui->SendEvent( WID_StoreWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	InventoryWnd.FrameWnd->Reposition( 470, 394 );
/*
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
	InventoryWnd.Open();
#else
	nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
*/
	nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SendEvent( WID_GuildInven_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	nRui->SendEvent( WID_GuildInven_Box1StateBtn, n_emSetChecked, 0, 0, 0, 0 );

	m_GuildInvenState = n_GUILDINVEN_SHOP1;
	m_StartSlotIndex = 0;

	m_SrcSlot = NULL;			
	m_TargetSlot = NULL;

	UpdateGuildInvenPcItem();

	return;
}

void CGuildInvenWnd::LogOff()
{
	for( int i = 0; i < c_MaxGuildInvenPcItemNum; ++i )
	{
		m_GuildInvenPcItem[i].Clear();
	}

	CloseGuildInvenWnd();

	return;
}
void CGuildInvenWnd::ResetFrontierBank()
{
	for( int i = 0; i < c_MaxGuildInvenPcItemNum; ++i )
	{
		m_GuildInvenPcItem[i].Clear();
	}
	return;
}

void CGuildInvenWnd::CloseGuildInvenWnd()
{
	if( IsVisible )
	{
		//길드창고 보관함 메세지전송
		g_RockClient.Send_RpCsCloseFrontierBank( nRui->thePcParam.PCName );	
	}
	
	nRui->SendEvent( WID_GuildInven_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );			
	nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장
	nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );//유료인벤확장
#endif
	return;
}

void CGuildInvenWnd::ResetCursorPcItem()
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


void CGuildInvenWnd::CloseNumberInputWnd( int aNumItem )
{
	if( !m_SrcSlot )
	{
		return;
	}

	if( m_SrcSlot->PcItem )
	{
		if( m_SrcSlot->PcItem->Amount >= aNumItem )
		{
			//길드창고 보관함 메세지전송
#ifdef DECO_RENEWAL_GUILD_MJH
			g_RockClient.Send_RpCsDepositFrontier( m_SrcSlot->PcItem->ItemUID, aNumItem, m_StartSlotIndex + m_TargetSlot->SlotNum );
#endif // DECO_RENEWAL_GUILD_MJH
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
#endif // DECO_RENEWAL_GUILD_MJH