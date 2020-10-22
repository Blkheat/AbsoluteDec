#include "..\\RockPCH.h"

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

#include "StoreWndProc.h"
#include "NumberInputWnd.h"
#include "InventoryWndProc.h"
#include "ChatBoxWndProc.h"
#include "stringmanager.h"
#include "NpcChatWnd.h"

CStoreWnd		StoreWnd;
CStoreBuyWnd	StoreBuyWnd;
CStoreSellWnd	StoreSellWnd;


//-----------------------------------------------------------------------------
void CStoreWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CStoreWnd::Composition()
{
	CTextBox*		tbox;
	CImageBox*		img;
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	
	SRect store_wnd;
	store_wnd.w	= 204;
	store_wnd.h	= 288;
	store_wnd.x	= 250;
	store_wnd.y	= center_y - ( store_wnd.h / 2 );
	
	///-- Frame Wnd
	FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_StoreWnd, WID_None, store_wnd.x, store_wnd.y, store_wnd.w, store_wnd.h, false );
	FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	FrameWnd->SetWndTileSizeLR( 40, 29 );
	FrameWnd->SetWndTileSizeTB( 31, 11 );

	FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_STORE ) );	 
	FrameWnd->SetTitle( true );	
	FrameWnd->SetWndProc( this );

	FrameWnd->SetVisible( false );
	FrameWnd->Initialize();
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StoreWnd, 1, 78, 202, 195, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StoreWnd, 1, 77, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StoreWnd, 1, 273, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	

	///-- 인벤토리 TEST(STATIC)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StoreWnd, 7, 30, 80, 17,  false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter );		
	tbox->SetText( G_STRING_CHAR( IDS_WND_STORE ) ); 

	///-- Item Slots
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StoreWnd, 4, 82, 183, 187, false );    
	img->SetClientImage( TID_C_BGSLOT_INVEN );

	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	CButton* btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Store_CloseBtn, WID_StoreWnd, 185, 6, 13, 13, false );   
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );


	SBtn[0] = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_Store_Item1SBtn, WID_StoreWnd, 7, 60, 78, 18, false );
	SBtn[0]->InitStateButton( n_cmUnChecked, _RT("") );
	SBtn[0]->SetClientImage( TID_None );
	SBtn[0]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
	SBtn[0]->SetVisible( false );
	SBtn[0]->SetAlignText( n_atCenter, n_atCenter );
	SBtn[0]->SetTextColor( 0, 0, 0 );	
	SBtn[0]->SetHID( HID_None );
	
	SBtn[1] = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_Store_Item2SBtn, WID_StoreWnd, 85, 60, 78, 18, false );
	SBtn[1]->InitStateButton( n_cmUnChecked,_RT("") );
	SBtn[1]->SetClientImage( TID_None );
	SBtn[1]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
	SBtn[1]->SetVisible( false );
	SBtn[1]->SetHID( HID_None );
	SBtn[1]->SetAlignText( n_atCenter, n_atCenter );
	SBtn[1]->SetTextColor( 0, 0, 0 );
	
	
	nRui->SetGroupWnd( 2, WID_Store_Item1SBtn, WID_Store_Item1SBtn, WID_Store_Item2SBtn  );

	///-- 스크롤 Bar
	ScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_Store_Scroll, WID_StoreWnd, 188, 78, 15, 195, false );
	
	///--JAPAN_BUG_MODIFY
	ScrollBar->InitSlide( stVertSld, 0, 7, 0, 13 );//Type, min, max, value, 썸의 폭

	ScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	ScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_OVR, TID_CTRL_SCR_BAR_CLK, TID_None );
	ScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_OVR, TID_CTRL_SCR2_UBT_CLK, TID_None );
	ScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_OVR, TID_CTRL_SCR2_DBT_CLK, TID_None );
	


	CSlot* Slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_BgSlot, WID_StoreWnd, 1, 70, 189, 208, false );
	Slot->SetSlotGroup( n_SGROUP_STORE, 255 );
	Slot->SetDisableHighlight();
	Slot->SetHID( HID_None );

	StoreSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot0, WID_StoreWnd,   4, 82, 35, 35, false );  
	StoreSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot1, WID_StoreWnd,  41, 82, 35, 35, false );  
	StoreSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot2, WID_StoreWnd,  78, 82, 35, 35, false );  
	StoreSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot3, WID_StoreWnd, 115, 82, 35, 35, false );  
	StoreSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot4, WID_StoreWnd, 152, 82, 35, 35, false );  
	StoreSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot5, WID_StoreWnd,   4, 120, 35, 35, false ); 
	StoreSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot6, WID_StoreWnd,  41, 120, 35, 35, false ); 
	StoreSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot7, WID_StoreWnd,  78, 120, 35, 35, false ); 
	StoreSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot8, WID_StoreWnd, 115, 120, 35, 35, false ); 
	StoreSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot9, WID_StoreWnd, 152, 120, 35, 35, false ); 

	StoreSlot[10] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot10, WID_StoreWnd,    4, 158, 35, 35, false ); 
	StoreSlot[11] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot11, WID_StoreWnd,   41, 158, 35, 35, false ); 
	StoreSlot[12] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot12, WID_StoreWnd,   78, 158, 35, 35, false ); 
	StoreSlot[13] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot13, WID_StoreWnd,  115, 158, 35, 35, false ); 
	StoreSlot[14] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot14, WID_StoreWnd,  152, 158, 35, 35, false ); 
	StoreSlot[15] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot15, WID_StoreWnd,    4, 196, 35, 35, false ); 
	StoreSlot[16] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot16, WID_StoreWnd,   41, 196, 35, 35, false ); 
	StoreSlot[17] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot17, WID_StoreWnd,   78, 196, 35, 35, false ); 
	StoreSlot[18] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot18, WID_StoreWnd,  115, 196, 35, 35, false ); 
	StoreSlot[19] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot19, WID_StoreWnd,  152, 196, 35, 35, false ); 
	StoreSlot[20] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot20, WID_StoreWnd,    4, 234, 35, 35, false ); 
	StoreSlot[21] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot21, WID_StoreWnd,   41, 234, 35, 35, false ); 
	StoreSlot[22] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot22, WID_StoreWnd,   78, 234, 35, 35, false ); 
	StoreSlot[23] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot23, WID_StoreWnd,  115, 234, 35, 35, false ); 
	StoreSlot[24] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Store_ItemSlot24, WID_StoreWnd,  152, 234, 35, 35, false ); 


	StoreSlot[0]->SetSlotGroup( n_SGROUP_STORE, 0 );
	StoreSlot[0]->SetHID( HID_ITEMTIP );	
	StoreSlot[1]->SetSlotGroup( n_SGROUP_STORE, 1 );
	StoreSlot[1]->SetHID( HID_ITEMTIP );	
	StoreSlot[2]->SetSlotGroup( n_SGROUP_STORE, 2 );
	StoreSlot[2]->SetHID( HID_ITEMTIP );	
	StoreSlot[3]->SetSlotGroup( n_SGROUP_STORE, 3 );
	StoreSlot[3]->SetHID( HID_ITEMTIP );
	StoreSlot[4]->SetSlotGroup( n_SGROUP_STORE, 4 );
	StoreSlot[4]->SetHID( HID_ITEMTIP );	
	StoreSlot[5]->SetSlotGroup( n_SGROUP_STORE, 5 );
	StoreSlot[5]->SetHID( HID_ITEMTIP );
	StoreSlot[6]->SetSlotGroup( n_SGROUP_STORE, 6 );
	StoreSlot[6]->SetHID( HID_ITEMTIP );
	StoreSlot[7]->SetSlotGroup( n_SGROUP_STORE, 7 );
	StoreSlot[7]->SetHID( HID_ITEMTIP );
	StoreSlot[8]->SetSlotGroup( n_SGROUP_STORE, 8 );
	StoreSlot[8]->SetHID( HID_ITEMTIP );
	StoreSlot[9]->SetSlotGroup( n_SGROUP_STORE, 9 );
	StoreSlot[9]->SetHID( HID_ITEMTIP );
	StoreSlot[10]->SetSlotGroup( n_SGROUP_STORE, 10 );
	StoreSlot[10]->SetHID( HID_ITEMTIP );
	StoreSlot[11]->SetSlotGroup( n_SGROUP_STORE, 11 );
	StoreSlot[11]->SetHID( HID_ITEMTIP );
	StoreSlot[12]->SetSlotGroup( n_SGROUP_STORE, 12 );
	StoreSlot[12]->SetHID( HID_ITEMTIP );
	StoreSlot[13]->SetSlotGroup( n_SGROUP_STORE, 13 );
	StoreSlot[13]->SetHID( HID_ITEMTIP );
	StoreSlot[14]->SetSlotGroup( n_SGROUP_STORE, 14 );
	StoreSlot[14]->SetHID( HID_ITEMTIP );
	StoreSlot[15]->SetSlotGroup( n_SGROUP_STORE, 15 );
	StoreSlot[15]->SetHID( HID_ITEMTIP );
	StoreSlot[16]->SetSlotGroup( n_SGROUP_STORE, 16 );
	StoreSlot[16]->SetHID( HID_ITEMTIP );
	StoreSlot[17]->SetSlotGroup( n_SGROUP_STORE, 17 );
	StoreSlot[17]->SetHID( HID_ITEMTIP );
	StoreSlot[18]->SetSlotGroup( n_SGROUP_STORE, 18 );
	StoreSlot[18]->SetHID( HID_ITEMTIP );
	StoreSlot[19]->SetSlotGroup( n_SGROUP_STORE, 19 );
	StoreSlot[19]->SetHID( HID_ITEMTIP );
	StoreSlot[20]->SetSlotGroup( n_SGROUP_STORE, 20 );
	StoreSlot[20]->SetHID( HID_ITEMTIP );
	StoreSlot[21]->SetSlotGroup( n_SGROUP_STORE, 21 );
	StoreSlot[21]->SetHID( HID_ITEMTIP );
	StoreSlot[22]->SetSlotGroup( n_SGROUP_STORE, 22 );
	StoreSlot[22]->SetHID( HID_ITEMTIP );
	StoreSlot[23]->SetSlotGroup( n_SGROUP_STORE, 23 );
	StoreSlot[23]->SetHID( HID_ITEMTIP );
	StoreSlot[24]->SetSlotGroup( n_SGROUP_STORE, 24 );
	StoreSlot[24]->SetHID( HID_ITEMTIP );
}

//-----------------------------------------------------------------------------
void CStoreWnd::Update()
{

	for( int i = 0; i < c_MaxStoreItemDrawNum; ++i )
	{
		StoreSlot[i]->theSlotState = n_Slot_Enable;

		// 구입할 수 없는 아이템들은 Disable 시킨다 
		if( StoreSlot[i]->PcItem && StoreSlot[i]->PcItem->ItemTable )
		{			
			// 살돈이 있는지 체크 
			if( StoreSlot[i]->PcItem->ItemTable->thePrice > nRui->thePcParam.Money )
			{
				StoreSlot[i]->theSlotState = n_Slot_Disable;		
			}

			// 필요 레벨 체크 
			if( StoreSlot[i]->PcItem->ItemTable->theLimitLevel > nRui->thePcParam.Level )
			{
				StoreSlot[i]->theSlotState = n_Slot_Disable;		
			}

			// 착용할 수 있는 아이템인지 체크 
			if( StoreSlot[i]->PcItem->IType == n_ITYPE_WEAR )
			{
				if( InventoryWnd.CheckWearPassiveSkill( StoreSlot[i]->PcItem->ItemTable ) == false )
				{					
					StoreSlot[i]->theSlotState = n_Slot_Disable;		
				}
			}
			// 사용할 수 있는 아이템인지 체크 
			else if( StoreSlot[i]->PcItem->IType == n_ITYPE_USE )
			{
				if( InventoryWnd.CheckUseItemPassiveSkill( StoreSlot[i]->PcItem->ItemTable ) == false )
				{					
					StoreSlot[i]->theSlotState = n_Slot_Disable;		
				}
			}		

			//명성치 아이템 자기 명성에 맞는것만 체크.. 
			if( nRui->GetPcParamInfo()->Fame < StoreSlot[i]->PcItem->ItemTable->theLimitFame  )
			{
				StoreSlot[i]->theSlotState = n_Slot_Disable;												
			}

			// 신수의 경우 따로 처리 
			if( StoreSlot[i]->PcItem->Class == n_Class_FrontierPet )
			{
				StoreSlot[i]->theSlotState = n_Slot_Enable;

				if( nRui->GetPcInvenInfo()->FrontierInfo )
				{
					if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
					{
						StoreSlot[i]->theSlotState = n_Slot_Disable;
					}
					else
					{
						if( StoreSlot[i]->PcItem->ItemTable->thePrice > nRui->GetPcInvenInfo()->FrontierInfo->theDues )
						{
							StoreSlot[i]->theSlotState = n_Slot_Disable;
						}
					}
				}
			}

		}

	}
	
	return;
}

//-----------------------------------------------------------------------------
void CStoreWnd::Proc( SEventMessage* EMsg )
{
	
	switch( EMsg->FocusWnd )
	{
	case WID_Store_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_StoreWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
		break;
	case WID_StoreWnd:
		if( EMsg->Notify == NM_FWND_CLICK )
		{
			StoreWndClick( EMsg );
		}
		break;
	case WID_Store_Item1SBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetStoreTab( DealingItem[0] );
		}
		break;
	case WID_Store_Item2SBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( DealingItem[1] != n_STORE_NONE )
			{
				SetStoreTab( DealingItem[1] );
			}
		}
		break;
	case WID_Store_Scroll:
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetSlotScroll( (int)EMsg->fParam );
		}
		break;
	
	case WID_Store_BgSlot:
		if( EMsg->Notify == NM_SLOT_CLICK )		{	StoreBgSlotClick( EMsg );		}
		break;

	case WID_Store_ItemSlot0:
	case WID_Store_ItemSlot1:
	case WID_Store_ItemSlot2:
	case WID_Store_ItemSlot3:
	case WID_Store_ItemSlot4:
	case WID_Store_ItemSlot5:
	case WID_Store_ItemSlot6:
	case WID_Store_ItemSlot7:
	case WID_Store_ItemSlot8:
	case WID_Store_ItemSlot9:
	case WID_Store_ItemSlot10:
	case WID_Store_ItemSlot11:
	case WID_Store_ItemSlot12:
	case WID_Store_ItemSlot13:
	case WID_Store_ItemSlot14:
	case WID_Store_ItemSlot15:
	case WID_Store_ItemSlot16:
	case WID_Store_ItemSlot17:
	case WID_Store_ItemSlot18:
	case WID_Store_ItemSlot19:
	case WID_Store_ItemSlot20:
	case WID_Store_ItemSlot21:
	case WID_Store_ItemSlot22:
	case WID_Store_ItemSlot23:
	case WID_Store_ItemSlot24:
		if( EMsg->Notify == NM_SLOT_CLICK )		{	StoreSlotClick( EMsg );		}
		if( EMsg->Notify == NM_SLOT_DBCLICK )	{	StoreSlotDBClick( EMsg );	}
		if( EMsg->Notify == NM_SLOT_DRAG )		{	StoreSlotDrag( EMsg );		}
		if( EMsg->Notify == NM_SLOT_DROP )		{	StoreSlotDrop( EMsg );		}
		break;
	}
	
}

//-----------------------------------------------------------------------------
void CStoreWnd::SetWheelValue( short _wheel )
{
	int value = 0;

	ScrollBar->GetValue( &value );
	if( ( _wheel < 0 ) && ( value < 7 ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	ScrollBar->SetValue( value );
	SetSlotScroll( value );
}

//-----------------------------------------------------------------------------
void CStoreWnd::SetStoreTab( N_DEALINGITEM _dealing_item )
{
	StartSlotNum	= 0;
	ScrollBar->SetValue( StartSlotNum );

	CurDealingItem = _dealing_item;

	for( int i = 0; i < c_MaxStoreItemDrawNum; ++i )
	{
		switch( CurDealingItem )
		{
		case n_STORE_WEAPON:	//무기( 무기, 방패 )
			StoreSlot[i]->PcItem = ItemWeapon[StartSlotNum + i];
			break;
		case n_STORE_ARMOR:		//방어구( 상의, 하의, 장갑, 부츠 )
			StoreSlot[i]->PcItem = ItemArmor[StartSlotNum + i];
			break;
		case n_STORE_SUNDRIES:	//잡화( 서브장비, 장신구 )
			StoreSlot[i]->PcItem = ItemSundries[StartSlotNum + i];
			break;
		case n_STORE_USE:		//소모품( 소모품, 비소모품 )	
			StoreSlot[i]->PcItem = ItemUse[StartSlotNum + i];
			break;
		case n_STORE_POSITION:	//진지
			StoreSlot[i]->PcItem = ItemPosition[StartSlotNum + i];
			break;
		case n_STORE_ATTCKARMS:	//공성병기	
			StoreSlot[i]->PcItem = ItemAtkArms[StartSlotNum + i];
			break;
		case n_STORE_BULDING:  //성문/성벽
			StoreSlot[i]->PcItem = ItemBuilding[StartSlotNum + i];
			break;
		case n_STORE_GTOWER:  //가드타워
			StoreSlot[i]->PcItem = ItemGTower[StartSlotNum + i];
			break;
		case n_STORE_ETC_DESTORY:
		case n_STORE_ETC_SEALED:
			StoreSlot[i]->PcItem = ItemEtc[StartSlotNum + i];
			break; 
		}
	}
}

//-----------------------------------------------------------------------------
void CStoreWnd::SetSlotScroll( int linenum )
{
	StartSlotNum = linenum * 5;

	for( int i = 0; i < c_MaxStoreItemDrawNum; ++i )
	{
		switch( CurDealingItem )
		{
		case n_STORE_WEAPON:	//무기( 무기, 방패 )
			StoreSlot[i]->PcItem = ItemWeapon[StartSlotNum + i];
			break;
		case n_STORE_ARMOR:		//방어구( 상의, 하의, 장갑, 부츠 )
			StoreSlot[i]->PcItem = ItemArmor[StartSlotNum + i];
			break;
		case n_STORE_SUNDRIES:	//잡화( 서브장비, 장신구 )
			StoreSlot[i]->PcItem = ItemSundries[StartSlotNum + i];
			break;
		case n_STORE_USE:		//소모품( 소모품, 비소모품 )	
			StoreSlot[i]->PcItem = ItemUse[StartSlotNum + i];
			break;
		case n_STORE_POSITION:	//진지
			StoreSlot[i]->PcItem = ItemPosition[StartSlotNum + i];
			break;
		case n_STORE_ATTCKARMS:	//공성병기	
			StoreSlot[i]->PcItem = ItemAtkArms[StartSlotNum + i];
			break;
		case n_STORE_BULDING:  //성문/성벽
			StoreSlot[i]->PcItem = ItemBuilding[StartSlotNum + i];
			break;
		case n_STORE_GTOWER:  //가드타워
			StoreSlot[i]->PcItem = ItemGTower[StartSlotNum + i];
			break;
		case n_STORE_ETC_DESTORY:
		case n_STORE_ETC_SEALED:
			StoreSlot[i]->PcItem = ItemEtc[StartSlotNum + i];
		break; 
		}
	}
}

//-----------------------------------------------------------------------------
void CStoreWnd::StoreWndClick( SEventMessage* EMsg )
{
}

//-----------------------------------------------------------------------------
void CStoreWnd::StoreBgSlotClick( SEventMessage* EMsg )
{
	CSlot*	cslot	= NULL;//현재 장비 슬롯
	CSlot*	pslot	= NULL;//선택된(picked) 인벤 슬롯

	if( nRui->GetCursorItem() != NULL )
	{
		cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		pslot = nRui->GetItemSelectedSlot();
		if( pslot->PcItem == NULL )
		{
			return;
		}
		
		if( cslot->PcItem && (cslot->PcItem == pslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		else if( pslot->Group == n_SGROUP_INVENTORY )
		{
			//아이템 판매 창 띄우기
			if( pslot->PcItem->Amount > 1 )
			{
				g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_SELLSTORE, pslot->PcItem );
			}
			else
			{
				StoreSellWnd.SetSellItemInfo( pslot->PcItem, n_InvenTab );
			}

			nRui->ResetCursorItem();
		}
		else if( pslot->Group == n_SGROUP_WEAR )
		{
			//아이템 판매 창 띄우기
			//StoreSellWnd.SetSellItemInfo( pslot->PcItem, n_WearTab );
			//nRui->ResetCursorItem();
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SELL_IN_WEAR ) );	
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SELL_ITEM ) );			
//			nRui->TextOutChat( _T("SYSTEM_MSG : 아이템 판매를 할 수 없습니다."), D3DCOLOR_XRGB( 255, 255, 0 ) );
		}
	}
}

//-----------------------------------------------------------------------------
void CStoreWnd::StoreSlotClick( SEventMessage* EMsg )
{
	CSlot*	cslot	= NULL;//현재 장비 슬롯
	CSlot*	pslot	= NULL;//선택된(picked) 인벤 슬롯

	if( nRui->GetCursorItem() != NULL )
	{
		cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		pslot = nRui->GetItemSelectedSlot();
		if( pslot->PcItem == NULL )
		{
			return;
		}
		
		if( cslot->PcItem && (cslot->PcItem == pslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		else if( pslot->Group == n_SGROUP_INVENTORY )
		{
			//아이템 판매 창 띄우기
			if( pslot->PcItem->Amount > 1 )
			{
				g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_SELLSTORE, pslot->PcItem );
			}
			else
			{
				StoreSellWnd.SetSellItemInfo( pslot->PcItem, n_InvenTab );
			}

			nRui->ResetCursorItem();
		}
		else if( pslot->Group == n_SGROUP_WEAR )
		{
			//아이템 판매 창 띄우기
			//StoreSellWnd.SetSellItemInfo( pslot->PcItem, n_WearTab );
			//nRui->ResetCursorItem();
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SELL_IN_WEAR ) );			
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SELL_ITEM ) );			
//			nRui->TextOutChat( _T("SYSTEM_MSG : 아이템 판매를 할 수 없습니다."), D3DCOLOR_XRGB( 255, 255, 0 ) );
		}
	}
	else
	{
		//커서에 아이템 붙이기 메세지.
		cslot = (CSlot*)Container->Get( EMsg->FocusWnd );

		nRui->SetCursorItem( cslot->PcItem );
		nRui->SetItemSelectedSlot( cslot );
	}	
}


void CStoreWnd::StoreSlotDBClick( SEventMessage* EMsg )
{
	CSlot*	cslot	= NULL;//현재 장비 슬롯
	cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );

	if( cslot && cslot->PcItem )
	{
		// 신수의 경우 따로 처리 
		if( cslot->PcItem->Class == n_Class_FrontierPet )
		{
			if( nRui->GetPcInvenInfo()->FrontierInfo )
			{
				if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );									
				}
				else
				{
					if( nRui->GetPcInvenInfo()->FrontierInfo->theDues >= cslot->PcItem->ItemTable->thePrice )
					{
						g_StrInputWnd.Open( n_STRINPUT_FRONTIERPET_NAME, cslot->PcItem->Code );
					}
					else
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );							
						//					nRui->TextOutChat( "돈이 부족해서 아이템을 구입할 수 없습니다.", 0xffff0000 );
					}
				}
			}
			
			nRui->ResetCursorItem();
			
			return;
		}
		
		//아이템 구입 창 띄우기
		StoreBuyWnd.SetBuyStoreOwner( StoreWnd.OwnerUID );
		if( cslot->PcItem->ItemTable->theBundle != 1 )
		{
			g_NumberInputWnd.Open( n_NUMBERINPUT_BUYSTORE_INVENTORY, cslot->PcItem );
		}
		else
		{
			if( nRui->thePcParam.Money >= cslot->PcItem->ItemTable->thePrice )
			{
				StoreBuyWnd.SetBuyItemInfo( cslot->PcItem );
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );							
//				nRui->TextOutChat( "돈이 부족해서 아이템을 구입할 수 없습니다.", 0xffff0000 );
			}
		}		
	}
	return;
}

//-----------------------------------------------------------------------------
void CStoreWnd::StoreSlotDrag( SEventMessage* EMsg )
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

//-----------------------------------------------------------------------------
void CStoreWnd::StoreSlotDrop( SEventMessage* EMsg )
{
	CSlot* cslot = (CSlot*)Container->Get( EMsg->FocusWnd );
	CSlot* tslot = (CSlot*)Container->Get( (WNDID)EMsg->fParam );
	

	if( ((WNDID)EMsg->fParam != WID_None ) && ( tslot->GetWindowType() == n_wtSlot ) )
	{
		if( cslot == tslot )
		{
			nRui->ResetCursorItem();
		}
		else if( ( cslot->PcItem != NULL ) && ( tslot->Group == n_SGROUP_INVENTORY ) )
		{

			InventoryWnd.SetSlotPosition(tslot->SlotNum);

			// 신수의 경우 따로 처리 
			if( cslot->PcItem->Class == n_Class_FrontierPet )
			{
				if( nRui->GetPcInvenInfo()->FrontierInfo )
				{
					if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );									
					}
					else
					{
						if( nRui->GetPcInvenInfo()->FrontierInfo->theDues >= cslot->PcItem->ItemTable->thePrice )
						{
							g_StrInputWnd.Open( n_STRINPUT_FRONTIERPET_NAME, cslot->PcItem->Code );
						}
						else
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );							
							//					nRui->TextOutChat( "돈이 부족해서 아이템을 구입할 수 없습니다.", 0xffff0000 );
						}
					}
				}
				
				nRui->ResetCursorItem();
				
				return;
			}	
			
			//아이템 구입 창 띄우기
			StoreBuyWnd.SetBuyStoreOwner( OwnerUID );
			if( cslot->PcItem->ItemTable->theBundle != 1 )
			{
				g_NumberInputWnd.Open( n_NUMBERINPUT_BUYSTORE_INVENTORY, cslot->PcItem );
			}
			else
			{
				if( nRui->thePcParam.Money >= cslot->PcItem->ItemTable->thePrice )
				{
					StoreBuyWnd.SetBuyItemInfo( cslot->PcItem );
				}
				else
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );							
//					nRui->TextOutChat( "돈이 부족해서 아이템을 구입할 수 없습니다.", 0xffff0000 );
				}
			}

			nRui->ResetCursorItem();
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_BUY ) );							
//			nRui->TextOutChat( _T("SYSTEM_MSG : 아이템 구입을 할 수 없습니다."), D3DCOLOR_XRGB( 255, 255, 0 ) );
		}
	}	
}

//-----------------------------------------------------------------------------
N_DEALINGITEM CStoreWnd::GetDealingItem( int _dealing_item )
{
	for( int i = 0; i < Shop->NumItem; ++i )
	{
		switch( Shop->Item[i].DealingItemType )
		{
		case n_STORE_WEAPON://무기( 무기, 방패 )
			if( ( _dealing_item == n_STORE_NONE ) || ( _dealing_item != n_STORE_WEAPON ) )
			{
				return( n_STORE_WEAPON );
			}
			break;
		case n_STORE_ARMOR://방어구( 상의, 하의, 장갑, 부츠 )
			if( ( _dealing_item == n_STORE_NONE ) || ( _dealing_item != n_STORE_ARMOR ) )
			{
				return( n_STORE_ARMOR );
			}
			break;
		case n_STORE_SUNDRIES://잡화( 서브장비, 장신구 )
			if( ( _dealing_item == n_STORE_NONE ) || ( _dealing_item != n_STORE_SUNDRIES ) )
			{
				return( n_STORE_SUNDRIES );
			}
			break;
		case n_STORE_USE://소모품( 소모품, 비소모품 )
			if( ( _dealing_item == n_STORE_NONE ) || ( _dealing_item != n_STORE_USE ) )
			{
				return( n_STORE_USE );
			}
			break;
		case n_STORE_POSITION://진지
			if( ( _dealing_item == n_STORE_NONE ) || ( _dealing_item != n_STORE_POSITION ) )
			{
				return( n_STORE_POSITION );
			}
			break;
		case n_STORE_ATTCKARMS://공성병기
			if( ( _dealing_item == n_STORE_NONE ) || ( _dealing_item != n_STORE_ATTCKARMS ) )
			{
				return( n_STORE_ATTCKARMS );
			}
			break;
		case n_STORE_BULDING://성문/성벽
			if( ( _dealing_item == n_STORE_NONE ) || ( _dealing_item != n_STORE_BULDING ) )
			{
				return( n_STORE_BULDING );
			}
			break;
		case n_STORE_GTOWER://가드타워
			if( ( _dealing_item == n_STORE_NONE ) || ( _dealing_item != n_STORE_GTOWER ) )
			{
				return( n_STORE_GTOWER );
			}
			break;
		case n_STORE_ETC_DESTORY:
			if( (_dealing_item == n_STORE_NONE)  || ( _dealing_item != n_STORE_ETC_DESTORY) )
			{
				return( n_STORE_ETC_DESTORY );
			}
			break; 
		case n_STORE_ETC_SEALED:
			if( (_dealing_item == n_STORE_NONE) || _dealing_item != n_STORE_ETC_SEALED )
			{
				return( n_STORE_ETC_SEALED);
			}
			break;
		}		
	}

	return( n_STORE_NONE );
}

//-----------------------------------------------------------------------------
void CStoreWnd::SetShopItem()
{
	int i = 0;

	NumDealingItem	= 2;
	DealingItem[0]	= n_STORE_NONE;//거래 품목1
	DealingItem[1]	= n_STORE_NONE;//거래 품목2
	
	NumItemWeapon	= 0;
	NumItemArmor	= 0;
	NumItemSundries	= 0;
	NumItemUse		= 0;

	///-- 콜로니 관련
	NumItemPosition = 0;
	NumItemAtkArms	= 0;
	NumItemBuilding	= 0;
	NumItemGTower	= 0;
	
	NumItemEtc		= 0; 

	for( i = 0; i < c_MaxStoreItemDrawNum; ++i )
	{
		if( StoreSlot[i]->PcItem != NULL ){	StoreSlot[i]->PcItem = NULL;	}
	}

	for( i = 0; i < MAX_STORE_ITEM; ++i )
	{
		SAFE_DELETE(ItemWeapon[i]);
		SAFE_DELETE(ItemArmor[i]);		
		SAFE_DELETE(ItemSundries[i]);		
		SAFE_DELETE(ItemUse[i]);

		///-- 콜로니 관련
		SAFE_DELETE(ItemPosition[i]);
		SAFE_DELETE(ItemAtkArms[i]);
		SAFE_DELETE(ItemBuilding[i]);
		SAFE_DELETE(ItemGTower[i]);	
		
		SAFE_DELETE(ItemEtc[i] );
	}
	
	
	DealingItem[0] = GetDealingItem( n_STORE_NONE );//
	DealingItem[1] = GetDealingItem( DealingItem[0] );

	SBtn[0]->SetVisible( false );
	SBtn[1]->SetVisible( false );
	SBtn[0]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
	SBtn[1]->SetBtnImage( TID_None, TID_None, TID_None, TID_None );
	
	SBtn[0]->SetChecked();

	if( DealingItem[1] == n_STORE_NONE )
	{
		NumDealingItem = 1;
	}	
	
	for( i = 0; i < NumDealingItem; ++i )
	{
		switch( DealingItem[i] )
		{
		case n_STORE_NONE:		//
			break;
		case n_STORE_WEAPON:	//무기( 무기, 방패 )
			SetItemWeapon();
			SBtn[i]->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
			SBtn[i]->SetVisible( true );
			SBtn[i]->SetText( G_STRING_CHAR( IDS_ARM_ITEM ) );
			break;
		case n_STORE_ARMOR:		//방어구( 상의, 하의, 장갑, 부츠 )
			SetItemArmor();
			SBtn[i]->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
			SBtn[i]->SetVisible( true );
			SBtn[i]->SetText( G_STRING_CHAR( IDS_GUARD_ITEM ) );
			break;
		case n_STORE_SUNDRIES:	//잡화( 서브장비, 장신구 )
			SetItemSundries();
			SBtn[i]->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
			SBtn[i]->SetVisible( true );
			SBtn[i]->SetText( G_STRING_CHAR( IDS_EXT_ITEM ) );
			break;
		case n_STORE_USE:		//소모품( 소모품, 비소모품 )	
			SetItemUse();
			SBtn[i]->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
			SBtn[i]->SetVisible( true );
			SBtn[i]->SetText( G_STRING_CHAR( IDS_ITEM_USE ) );
			break;
		case n_STORE_POSITION:	//진지
			SetItemPosition();
			SBtn[i]->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
			SBtn[i]->SetVisible( true );
			SBtn[i]->SetText( _RT("진지") );				///-- HAN_MSG			
			break;
		case n_STORE_ATTCKARMS:	//공성병기
			SetItemAtkArms();
			SBtn[i]->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
			SBtn[i]->SetVisible( true );
			SBtn[i]->SetText( _RT("공성병기") );			///-- HAN_MSG			
			break;
		case n_STORE_BULDING:	//성벽/성문
			SetItemBuilding();
			SBtn[i]->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
			SBtn[i]->SetVisible( true );
			SBtn[i]->SetText(_RT("성문/성벽") );		///-- HAN_MSG			
			break;
		case n_STORE_GTOWER:
			SetItemGTower();
			SBtn[i]->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
			SBtn[i]->SetVisible( true );
			SBtn[i]->SetText( _RT("가드타워") );		///-- HAN_MSG
			break;
		case n_STORE_ETC_DESTORY:
			SetItemEtc();
			SBtn[i]->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
			SBtn[i]->SetVisible( true );
			SBtn[i]->SetText( G_STRING_CHAR(IDS_WND_DESTORY_EQUIP));		///-- HAN_MSG
			break; 
		case n_STORE_ETC_SEALED:
			SetItemEtc();
			SBtn[i]->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
			SBtn[i]->SetVisible( true );
			SBtn[i]->SetText( G_STRING_CHAR(IDS_WND_SEAL_EQUIP));		///-- HAN_MSG
			break; 
		}
	}
}

//-----------------------------------------------------------------------------
void CStoreWnd::SetItemWeapon()
{
	for( int i = 0; i < Shop->NumItem; ++i )
	{
		if( Shop->Item[i].DealingItemType == n_STORE_WEAPON )
		{
			//
			SPcItem * tmp_pcitem = NULL;
			tmp_pcitem = SAFE_NEW( SPcItem );
			
			tmp_pcitem->IType		= n_ITYPE_WEAR;
			tmp_pcitem->Class		= g_Pc_Manager.GetItemClsss( Shop->Item[i].Code );
			tmp_pcitem->ItemUID		= 0;
			tmp_pcitem->Code		= Shop->Item[i].Code;
			tmp_pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( Shop->Item[i].Code );
			tmp_pcitem->Amount		= Shop->Item[i].Amount;
			tmp_pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( Shop->Item[i].Code );
			
			memset( &(tmp_pcitem->WearTable), 0 , sizeof(SRpdWearItemBase) );
			
			// 왼손,오른손....
			//---------------------------------------------------------------------
			if( ( tmp_pcitem->ItemTable->theType == n_Type_MagicBook ) || ( tmp_pcitem->ItemTable->theType == n_Type_Bow ) || ( tmp_pcitem->ItemTable->theType == n_Type_Shield ) )
			{
				tmp_pcitem->Class = n_Class_Shield;
			}
			else if( ( tmp_pcitem->ItemTable->theType == n_Type_Sword ) || ( tmp_pcitem->ItemTable->theType == n_Type_TwohandSword ) ||
						( tmp_pcitem->ItemTable->theType == n_Type_Staff ) || ( tmp_pcitem->ItemTable->theType == n_Type_CrystalBall ) )
			{
				tmp_pcitem->Class = n_Class_Weapon;
			}
			//---------------------------------------------------------------------

			///-- 내구도 Settring
			tmp_pcitem->WearTable.theEndurance = tmp_pcitem->ItemTable->theMaxEndurance;
			
			SAFE_DELETE( ItemWeapon[NumItemWeapon] );			
			ItemWeapon[NumItemWeapon] = tmp_pcitem;
			
			NumItemWeapon++;
		}
	}
}

//-----------------------------------------------------------------------------
void CStoreWnd::SetItemArmor()
{
	for( int i = 0; i < Shop->NumItem; ++i )
	{
		if( Shop->Item[i].DealingItemType == n_STORE_ARMOR )
		{
			//
			SPcItem * tmp_pcitem = NULL;
			tmp_pcitem = SAFE_NEW( SPcItem );

			tmp_pcitem->IType		= n_ITYPE_WEAR;
			tmp_pcitem->Class		= g_Pc_Manager.GetItemClsss( Shop->Item[i].Code );
			tmp_pcitem->ItemUID		= 0;
			tmp_pcitem->Code		= Shop->Item[i].Code;
			tmp_pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( Shop->Item[i].Code );
			tmp_pcitem->Amount		= Shop->Item[i].Amount;
			tmp_pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( Shop->Item[i].Code );
			memset( &(tmp_pcitem->WearTable), 0 , sizeof(SRpdWearItemBase) );

			// 왼손,오른손....
			//---------------------------------------------------------------------
			if( ( tmp_pcitem->ItemTable->theType == n_Type_MagicBook ) || ( tmp_pcitem->ItemTable->theType == n_Type_Bow ) || ( tmp_pcitem->ItemTable->theType == n_Type_Shield ) )
			{
				tmp_pcitem->Class = n_Class_Shield;
			}
			else if( ( tmp_pcitem->ItemTable->theType == n_Type_Sword ) || ( tmp_pcitem->ItemTable->theType == n_Type_TwohandSword ) ||
						( tmp_pcitem->ItemTable->theType == n_Type_Staff ) || ( tmp_pcitem->ItemTable->theType == n_Type_CrystalBall ) )
			{
				tmp_pcitem->Class = n_Class_Weapon;
			}
			//---------------------------------------------------------------------

			///-- 내구도 Settring
			tmp_pcitem->WearTable.theEndurance = tmp_pcitem->ItemTable->theMaxEndurance;
			
			SAFE_DELETE( ItemArmor[NumItemArmor] );			
			ItemArmor[NumItemArmor] = tmp_pcitem;			
			NumItemArmor++;
		}
	}
}

//-----------------------------------------------------------------------------
void CStoreWnd::SetItemSundries()
{
	for( int i = 0; i < Shop->NumItem; ++i )
	{
		if( Shop->Item[i].DealingItemType == n_STORE_SUNDRIES )
		{
			//
			SPcItem * tmp_pcitem = NULL;
			tmp_pcitem = SAFE_NEW( SPcItem );
			
			tmp_pcitem->IType		= n_ITYPE_WEAR;
			tmp_pcitem->Class		= g_Pc_Manager.GetItemClsss( Shop->Item[i].Code );
			tmp_pcitem->ItemUID		= 0;
			tmp_pcitem->Code		= Shop->Item[i].Code;
			tmp_pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( Shop->Item[i].Code );
			tmp_pcitem->Amount		= Shop->Item[i].Amount;
			tmp_pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( Shop->Item[i].Code );
			memset( &(tmp_pcitem->WearTable), 0 , sizeof(SRpdWearItemBase) );

			// 왼손,오른손....
			//---------------------------------------------------------------------
			if( ( tmp_pcitem->ItemTable->theType == n_Type_MagicBook ) || ( tmp_pcitem->ItemTable->theType == n_Type_Bow ) || ( tmp_pcitem->ItemTable->theType == n_Type_Shield ) )
			{
				tmp_pcitem->Class = n_Class_Shield;
			}
			else if( ( tmp_pcitem->ItemTable->theType == n_Type_Sword ) || ( tmp_pcitem->ItemTable->theType == n_Type_TwohandSword ) ||
						( tmp_pcitem->ItemTable->theType == n_Type_Staff ) || ( tmp_pcitem->ItemTable->theType == n_Type_CrystalBall ) )
			{
				tmp_pcitem->Class = n_Class_Weapon;
			}
			//---------------------------------------------------------------------
			tmp_pcitem->WearTable.theEndurance = tmp_pcitem->ItemTable->theMaxEndurance;
			
			SAFE_DELETE( ItemSundries[NumItemSundries] );		
			ItemSundries[NumItemSundries] = tmp_pcitem;
			NumItemSundries++;
		}
	}
}

//-----------------------------------------------------------------------------
void CStoreWnd::SetItemUse()
{
	for( int i = 0; i < Shop->NumItem; ++i )
	{
		if( Shop->Item[i].DealingItemType == n_STORE_USE )
		{
			
			SPcItem * tmp_pcitem = NULL;
			tmp_pcitem = SAFE_NEW( SPcItem );

			if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Use )
			{
				tmp_pcitem->IType	= n_ITYPE_USE;//소모품
			}
			else if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Useless )
			{
				tmp_pcitem->IType	= n_ITYPE_UNUSE;//비소모품
			}
			else if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Special ||
					 g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Eldastone ||
					 g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_FrontierPet ||
					 g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Weapon_StrengthStone ||
					 g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Armor_StrengthStone )
			{
				tmp_pcitem->IType	= n_ITYPE_SPECIAL;
			}
			else if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Riding )
			{
				tmp_pcitem->IType	= n_ITYPE_RIDE;
			}
			else if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_RidingItem )
			{
				tmp_pcitem->IType	= n_ITYPE_RIDE_WEAR;
			}
			else if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Skin )
			{
				tmp_pcitem->IType	= n_ITYPE_SKIN_WEAR;
			}
			
			tmp_pcitem->Class		= g_Pc_Manager.GetItemClsss( Shop->Item[i].Code );
			tmp_pcitem->ItemUID		= 0;
			tmp_pcitem->Code		= Shop->Item[i].Code;
			tmp_pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( Shop->Item[i].Code );
			tmp_pcitem->Amount		= Shop->Item[i].Amount;
			tmp_pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( Shop->Item[i].Code );

			SAFE_DELETE( ItemUse[NumItemUse] );

			ItemUse[NumItemUse] = tmp_pcitem;
			NumItemUse++;
		}
	}
}

void CStoreWnd::SetItemPosition()
{
	for( int i = 0; i < Shop->NumItem; ++i )
	{
		if( Shop->Item[i].DealingItemType == n_STORE_POSITION )
		{
			//
			SPcItem * tmp_pcitem = NULL;
			tmp_pcitem = SAFE_NEW( SPcItem );

			tmp_pcitem->IType	= n_ITYPE_USE;//소모품

			if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Use )
			{
				tmp_pcitem->IType	= n_ITYPE_USE;//소모품
			}
			else if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Useless )
			{
				tmp_pcitem->IType	= n_ITYPE_UNUSE;//비소모품
			}
			
			tmp_pcitem->Class		= g_Pc_Manager.GetItemClsss( Shop->Item[i].Code );
			tmp_pcitem->ItemUID		= 0;
			tmp_pcitem->Code		= Shop->Item[i].Code;
			tmp_pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( Shop->Item[i].Code );
			tmp_pcitem->Amount		= Shop->Item[i].Amount;
			tmp_pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( Shop->Item[i].Code );

			SAFE_DELETE( ItemPosition[NumItemPosition] );		
			
			ItemPosition[NumItemPosition] = tmp_pcitem;
			NumItemPosition++;
		}
	}
}

void CStoreWnd::SetItemAtkArms()
{
	for( int i = 0; i < Shop->NumItem; ++i )
	{
		if( Shop->Item[i].DealingItemType == n_STORE_ATTCKARMS )
		{
			//
			SPcItem * tmp_pcitem = NULL;
			tmp_pcitem = SAFE_NEW( SPcItem );

			tmp_pcitem->IType	= n_ITYPE_USE;//소모품

			if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Use )
			{
				tmp_pcitem->IType	= n_ITYPE_USE;//소모품
			}
			else if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Useless )
			{
				tmp_pcitem->IType	= n_ITYPE_UNUSE;//비소모품
			}
			
			tmp_pcitem->Class		= g_Pc_Manager.GetItemClsss( Shop->Item[i].Code );
			tmp_pcitem->ItemUID		= 0;
			tmp_pcitem->Code		= Shop->Item[i].Code;
			tmp_pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( Shop->Item[i].Code );
			tmp_pcitem->Amount		= Shop->Item[i].Amount;
			tmp_pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( Shop->Item[i].Code );

			SAFE_DELETE( ItemAtkArms[NumItemAtkArms] );			
			ItemAtkArms[NumItemAtkArms] = tmp_pcitem;
			NumItemAtkArms++;
		}
	}
}

void CStoreWnd::SetItemBuilding()
{
	for( int i = 0; i < Shop->NumItem; ++i )
	{
		if( Shop->Item[i].DealingItemType == n_STORE_BULDING )
		{
			SPcItem * tmp_pcitem = NULL;
			tmp_pcitem = SAFE_NEW( SPcItem );

			tmp_pcitem->IType	= n_ITYPE_USE;//소모품

			if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Use )
			{
				tmp_pcitem->IType	= n_ITYPE_USE;//소모품
			}
			else if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Useless )
			{
				tmp_pcitem->IType	= n_ITYPE_UNUSE;//비소모품
			}
			
			tmp_pcitem->Class		= g_Pc_Manager.GetItemClsss( Shop->Item[i].Code );
			tmp_pcitem->ItemUID		= 0;
			tmp_pcitem->Code		= Shop->Item[i].Code;
			tmp_pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( Shop->Item[i].Code );
			tmp_pcitem->Amount		= Shop->Item[i].Amount;
			tmp_pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( Shop->Item[i].Code );

			SAFE_DELETE( ItemBuilding[NumItemBuilding] );			
			ItemBuilding[NumItemBuilding] = tmp_pcitem;
			
			NumItemBuilding++;
		}
	}
}

void CStoreWnd::SetItemGTower()
{
	for( int i = 0; i < Shop->NumItem; ++i )
	{
		if( Shop->Item[i].DealingItemType == n_STORE_GTOWER )
		{
			//
			SPcItem * tmp_pcitem = NULL;
			tmp_pcitem = SAFE_NEW( SPcItem );

			tmp_pcitem->IType	= n_ITYPE_USE;//소모품

			if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Use )
			{
				tmp_pcitem->IType	= n_ITYPE_USE;//소모품
			}
			else if( g_Pc_Manager.GetItemClsss( Shop->Item[i].Code ) == n_Class_Useless )
			{
				tmp_pcitem->IType	= n_ITYPE_UNUSE;//비소모품
			}
			
			tmp_pcitem->Class		= g_Pc_Manager.GetItemClsss( Shop->Item[i].Code );
			tmp_pcitem->ItemUID		= 0;
			tmp_pcitem->Code		= Shop->Item[i].Code;
			tmp_pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( Shop->Item[i].Code );
			tmp_pcitem->Amount		= Shop->Item[i].Amount;
			tmp_pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( Shop->Item[i].Code );

			SAFE_DELETE( ItemGTower[NumItemGTower] );			
			ItemGTower[NumItemGTower] = tmp_pcitem;
			NumItemGTower++;
		}
	}
}

void CStoreWnd::SetItemEtc()
{
	for( int i = 0; i < Shop->NumItem; ++i )
	{
		if( Shop->Item[i].DealingItemType == n_STORE_ETC_DESTORY || 
			Shop->Item[i].DealingItemType == n_STORE_ETC_SEALED )
		{
			SPcItem * tmp_pcitem = NULL;
			tmp_pcitem = SAFE_NEW( SPcItem );
			
			//////////////////////////////////////////////////////////////////////////
			WORD ItemType = g_Pc_Manager.GetItemClsss( Shop->Item[i].Code );
			if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) ) 
			{
				tmp_pcitem->IType	= n_ITYPE_WEAR;
				memset( &(tmp_pcitem->WearTable), 0 , sizeof(SRpdWearItemBase) );
			}
			else if( ItemType == n_Class_Use )
			{
				tmp_pcitem->IType	= n_ITYPE_USE;
			}
			else if( ItemType == n_Class_Useless )
			{
				tmp_pcitem->IType	= n_ITYPE_UNUSE;
			}
			//else if( ItemType == 12 ) 얼굴
			else if( ItemType == n_Class_Quest )
			{
				tmp_pcitem->IType	= n_ITYPE_QUEST;
			}
			else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
				|| ItemType == n_Class_FrontierPet ||ItemType == n_Class_Weapon_StrengthStone 
				|| ItemType == n_Class_Armor_StrengthStone )
			{
				tmp_pcitem->IType	= n_ITYPE_SPECIAL; 
			}		
			else if( ItemType == n_Class_PersonalPet )
			{
				tmp_pcitem->IType	= n_ITYPE_PET_USE; 
			}
			else if( ItemType == n_Class_Riding )
			{
				tmp_pcitem->IType	= n_ITYPE_RIDE; 
				//말에 말굽내구도가 있기 땜시 .. 장비테이블에 설정해준다 .. 
			}
			else if( ItemType == n_Class_RidingItem )
			{
				tmp_pcitem->IType	= n_ITYPE_RIDE_WEAR; 
			}
			else if( ItemType == n_Class_Skin )
			{
				tmp_pcitem->IType = n_ITYPE_SKIN_WEAR;
			}
			memset( &(tmp_pcitem->WearTable), 0 , sizeof(SRpdWearItemBase) );
			
			
			tmp_pcitem->Class		= g_Pc_Manager.GetItemClsss( Shop->Item[i].Code );
			tmp_pcitem->ItemUID		= 0;
			tmp_pcitem->Code		= Shop->Item[i].Code;
			tmp_pcitem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( Shop->Item[i].Code );
			tmp_pcitem->Amount		= Shop->Item[i].Amount;
			tmp_pcitem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( Shop->Item[i].Code );
			tmp_pcitem->WearTable.theEndurance = tmp_pcitem->ItemTable->theMaxEndurance;
			
			
			// 왼손,오른손....
			//---------------------------------------------------------------------
			if( ( tmp_pcitem->ItemTable->theType == n_Type_MagicBook ) || ( tmp_pcitem->ItemTable->theType == n_Type_Bow ) || 
				( tmp_pcitem->ItemTable->theType == n_Type_Shield ) )
			{
				tmp_pcitem->Class = n_Class_Shield;
			}
			else if( ( tmp_pcitem->ItemTable->theType == n_Type_Sword ) || ( tmp_pcitem->ItemTable->theType == n_Type_TwohandSword ) ||
				( tmp_pcitem->ItemTable->theType == n_Type_Staff ) || ( tmp_pcitem->ItemTable->theType == n_Type_CrystalBall ) )
			{
				tmp_pcitem->Class = n_Class_Weapon;
			}
		
			SAFE_DELETE( ItemEtc[NumItemEtc] );			
			ItemEtc[NumItemEtc] = tmp_pcitem;
			NumItemEtc++;
		}
	}
}

//-----------------------------------------------------------------------------
void CStoreWnd::SetShopInfo( SStoreShop* _shop )
{
	SPcItem* tmpPcItem = NULL;
	char str[40] = "";
	
	if( _shop == NULL )
	{
//		MessageBox( NULL, "샾 정보가 없습니다.", "에러", MB_OK );
		SystemPrint( n_DarkRed, n_SystemMsg, "!Shop 정보가 없습니다." );
		return;
	}
	Shop = _shop;//shop
	SetShopItem();
	
	if( DealingItem[0] == n_STORE_ETC_DESTORY || 
		DealingItem[0] == n_STORE_ETC_SEALED) 
	{
		FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_HERO_EQUIP) );	 
	}
	else 
	{
		FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_STORE ) );	 
	}


	CurDealingItem = DealingItem[0];

	for( int i = 0; i < c_MaxStoreItemDrawNum; ++i )
	{
		switch( CurDealingItem )
		{
		case n_STORE_WEAPON:	//무기( 무기, 방패 )
			StoreSlot[i]->PcItem = ItemWeapon[i];
			break;
		case n_STORE_ARMOR:		//방어구( 상의, 하의, 장갑, 부츠 )
			StoreSlot[i]->PcItem = ItemArmor[i];
			break;
		case n_STORE_SUNDRIES:	//잡화( 서브장비, 장신구 )
			StoreSlot[i]->PcItem = ItemSundries[i];
			break;
		case n_STORE_USE:		//소모품( 소모품, 비소모품 )	
			StoreSlot[i]->PcItem = ItemUse[i];
			break;
		case n_STORE_POSITION:	//진지
			StoreSlot[i]->PcItem = ItemPosition[i];
			break;
		case n_STORE_ATTCKARMS:	//공성병기
			StoreSlot[i]->PcItem = ItemAtkArms[i];
			break;
		case n_STORE_BULDING:	//성문/성벽
			StoreSlot[i]->PcItem = ItemBuilding[i];
			break;
		case n_STORE_GTOWER:	//가드타워
			StoreSlot[i]->PcItem = ItemGTower[i];
			break;
		case n_STORE_ETC_DESTORY:
		case n_STORE_ETC_SEALED:
			StoreSlot[i]->PcItem = ItemEtc[i];
			break; 
		}
	}
}

//----------------------------------------------------------------------------
bool CStoreWnd::CompareSlot( CSlot* cslot, CSlot* tslot )
{
	if( ( cslot != NULL ) && ( tslot != NULL ) )
	{
		if( ( cslot->Group == tslot->Group ) &&
			( cslot->SlotNum == tslot->SlotNum ) &&
			( cslot->PcItem->ItemUID == tslot->PcItem->ItemUID ) &&
			( cslot->PcItem->Code == tslot->PcItem->Code ) )
		{
			return( true );
		}
	}

	return( false );
}

//-----------------------------------------------------------------------------




















//-----------------------------------------------------------------------------
void CStoreBuyWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CStoreBuyWnd::Composition()
{
	CFrameWnd*		fw;
	CButton*		btn;
	CImageBox*		img;

		
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect store_buy_wnd;
	store_buy_wnd.w	= 204;
	store_buy_wnd.h	= 132;
	store_buy_wnd.x	= center_x - ( store_buy_wnd.w / 2 );
	store_buy_wnd.y	= center_y - ( store_buy_wnd.h / 2 );
	
	///-- Frame Wnd	
	fw = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_ItemBuyWnd, WID_None, store_buy_wnd.x, store_buy_wnd.y, store_buy_wnd.w, store_buy_wnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );
	
	fw->SetCaption( G_STRING_CHAR( IDS_WND_ITEM_BUY ) );
	fw->SetTitle( true );
	fw->SetWndProc( this );		
	fw->SetVisible( false );
	fw->SetZorder( n_zoTop );
	fw->Initialize();
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ItemBuyWnd, 1, 33, 202, 63, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ItemBuyWnd,  1, 32, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ItemBuyWnd, 1, 96, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ItemBuyWnd, 85, 37, 35, 35, false );         
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	 
	IconImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ItemBuy_IconImg, WID_ItemBuyWnd, 85, 37, 35, 35, false );
	IconImg->SetClientImage( TID_None );			// 해당하는 ITEM의 아이콘 표시 
	IconImg->SetClientColor( 255, 255, 255 );

	NumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ItemBuyWnd, 82, 35, 35, 35, false );
	NumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	NumTxt->SetClientImage( TID_None );
	NumTxt->SetAlignText( n_atRight, n_atBottom );
	NumTxt->SetText( _RT("") );
	NumTxt->SetTextColor( 255, 255, 255 );
	NumTxt->SetHID( HID_None );
	

	PriceTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_ItemBuy_PriceText, WID_ItemBuyWnd, 15, 74, 170, 16, false );
	PriceTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	PriceTxt->SetClientImage( TID_None );
	PriceTxt->SetAlignText( n_atCenter, n_atCenter );
	PriceTxt->SetText( _RT("") );
	PriceTxt->SetTextColor( 100, 100, 0 );

	///-- 구입 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ItemBuy_OkBtn, WID_ItemBuyWnd, 29, 101, 58, 25, false );  
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_BUY ) ); 

	///-- 취소 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ItemBuy_CancelBtn, WID_ItemBuyWnd, 117, 101, 58, 25, false ); 
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 
	
}

//-----------------------------------------------------------------------------
void CStoreBuyWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CStoreBuyWnd::Proc( SEventMessage* EMsg )
{	
	switch( EMsg->FocusWnd )
	{
	case WID_ItemBuy_OkBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			OkClose();
		}
		break;	
	case WID_ItemBuy_CancelBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			CancelClose();
		}
		break;
	}
}

//-----------------------------------------------------------------------------
void CStoreBuyWnd::OkClose()
{
	nRui->SendEvent( WID_ItemBuyWnd, n_emSetVisible, false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	if( (NumItem != 0) && ( IsOverNumItem == false ) && ( IsOverMoney == false ) )
	{
		if( InventoryWnd.GetSlotPosition() != 0)
		{
			if(nRui->GetPcInvenInfo()->InvenItem[InventoryWnd.GetSlotPosition()] != NULL)
			{
				g_RockClient.Send_RpCsBuy( OwnerUID, PcItem->Code, NumItem, InventoryWnd.GetBlankSlotNum() );
			}
			else
			{
				g_RockClient.Send_RpCsBuy( OwnerUID, PcItem->Code , NumItem , InventoryWnd.GetSlotPosition());
			}
		}
		else
		{
			g_RockClient.Send_RpCsBuy( OwnerUID, PcItem->Code, NumItem, InventoryWnd.GetFirstSlotNum() );
		}
	}

	if( ChatBoxWnd.IsVisible == true )
	{
		nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetFocus, 0, 0, 0, 0 );
	}
}

//-----------------------------------------------------------------------------
void CStoreBuyWnd::CancelClose()
{
	nRui->SendEvent( WID_ItemBuyWnd, n_emSetVisible, false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	if( ChatBoxWnd.IsVisible == true )
	{
		nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetFocus, 0, 0, 0, 0 );
	}
}

//-----------------------------------------------------------------------------
void CStoreBuyWnd::SetBuyItemInfo( SPcItem* _pc_item )
{
	char str[40] = "";

	PcItem	= _pc_item;
	NumItem	= 1;

	IconImg->SetClientImage( PcItem->Tid );
	NumTxt->SetText( _RT("") );
	
	if( StoreWnd.theDCRate )
	{
		int price = (int)( PcItem->ItemTable->thePrice * ( StoreWnd.theDCRate * 0.01f ) ) + 1;
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_PRICE2 ), price, StoreWnd.theDCRate );	
	}
	else
	{
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_PRICE ), RWCHAR(PcItem->ItemTable->thePrice) );	
	}
	PriceTxt->SetText( str);

	nRui->SendEvent( WID_ItemBuyWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SetModal( WID_ItemBuyWnd ); 

	if( ChatBoxWnd.IsVisible == true )
	{
		nRui->SendEvent( WID_ChatBox_MessageEdit, n_emEditDisable, 0, 0, 0, 0 );
	}
}

//-----------------------------------------------------------------------------
void CStoreBuyWnd::CloseNumberInputWnd( SPcItem* _pc_item, int _num_item, BYTE _item_tab )
{
	char str[40] = "";

	IsOverNumItem	= false;
	IsOverMoney		= false;

	PcItem	= _pc_item;
	NumItem	= _num_item;

	int Bundle = PcItem->ItemTable->theBundle;
	DWORD Price = PcItem->ItemTable->thePrice;
	
	IconImg->SetClientImage( PcItem->Tid );

	if( NumItem > Bundle )
	{
		NumItem = Bundle;
	}

	Rsprintf( RWCHAR(str), _RT("%d"), NumItem );
	NumTxt->SetText( str );

	if( NumItem <= Bundle )
	{
		if( nRui->thePcParam.Money >= ( Price * NumItem ) )
		{
			if( StoreWnd.theDCRate )
			{
				int price = (int)( Price * ( StoreWnd.theDCRate * 0.01f ) ) + 1;
				Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_PRICE2 ), price * NumItem, StoreWnd.theDCRate );	
			}
			else
			{
				Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_PRICE ), Price * NumItem );				
			}
			PriceTxt->SetText( str );
		}
		else
		{
			PriceTxt->SetText( G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
			IsOverMoney = true;
		}
		
	}
	else
	{
		IsOverNumItem = true;
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_MAX_BOUND ), Bundle );
		PriceTxt->SetText( str );
	}
	
	nRui->SendEvent( WID_ItemBuyWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SetModal( WID_ItemBuyWnd );
}

//-----------------------------------------------------------------------------
void CStoreBuyWnd::SetBuyStoreOwner( DWORD _owner_uid )
{
	OwnerUID = _owner_uid;
}

//-----------------------------------------------------------------------------





//-----------------------------------------------------------------------------
void CStoreSellWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CStoreSellWnd::Composition()
{
	CFrameWnd*		fw;
	CButton*		btn;
	CImageBox*		img;

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect store_sell_wnd;
	store_sell_wnd.w	= 204;
	store_sell_wnd.h	= 132;
	store_sell_wnd.x	= center_x - ( store_sell_wnd.w / 2 );
	store_sell_wnd.y	= center_y - ( store_sell_wnd.h / 2 );
	

	///-- Frame Wnd	
	fw = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_ItemSellWnd, WID_None, store_sell_wnd.x, store_sell_wnd.y, store_sell_wnd.w, store_sell_wnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );

	fw->SetCaption( G_STRING_CHAR( IDS_WND_ITEM_SELL ) );
	fw->SetTitle( true );
	fw->SetWndProc( this );	
	fw->SetVisible( false );
	fw->SetZorder( n_zoTop );
	fw->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ItemSellWnd, 1, 33, 202, 63, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ItemSellWnd,  1, 32, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ItemSellWnd, 1, 96, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ItemSellWnd, 85, 37, 35, 35, false );         
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	IconImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ItemSell_IconImg, WID_ItemSellWnd, 85, 37, 35, 35, false );
	IconImg->SetClientImage( TID_None );			// 해당하는 ITEM의 아이콘 표시 
	IconImg->SetClientColor( 255, 255, 255 );

	NumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ItemSellWnd, 82, 35, 35, 35, false );
	NumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	NumTxt->SetClientImage( TID_None );
	NumTxt->SetAlignText( n_atRight, n_atBottom );
	NumTxt->SetText( _RT("") );
	NumTxt->SetTextColor( 255, 255, 255 );
	NumTxt->SetHID( HID_None );

	PriceTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_ItemSell_PriceText, WID_ItemSellWnd, 15, 74, 170, 16, false );
	PriceTxt->SetFontR( n_fRsvFontWndStatic );

	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ItemSell_OkBtn, WID_ItemSellWnd, 29, 101, 58, 25, false );  
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_SELL ) ); 

	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ItemSell_CancelBtn, WID_ItemSellWnd, 117, 101, 58, 25, false ); 
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL )); 
}

//-----------------------------------------------------------------------------
void CStoreSellWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CStoreSellWnd::Proc( SEventMessage* EMsg )
{	
	switch( EMsg->FocusWnd )
	{		
	case WID_ItemSell_OkBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			OkClose();
		}
		break;	
	case WID_ItemSell_CancelBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			CancelClose();
		}
		break;
	}
}

//-----------------------------------------------------------------------------
void CStoreSellWnd::OkClose()
{
	nRui->SendEvent( WID_ItemSellWnd, n_emSetVisible, false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	if( NumItem != 0 )
	{
		g_RockClient.Send_RpCsSell( StoreWnd.OwnerUID, PcItem->ItemUID, NumItem, ItemTab );
	}

	if( ChatBoxWnd.IsVisible == true )
	{
		nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetFocus, 0, 0, 0, 0 );
	}
}

//-----------------------------------------------------------------------------
void CStoreSellWnd::CancelClose()
{
	nRui->SendEvent( WID_ItemSellWnd, n_emSetVisible, false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	if( ChatBoxWnd.IsVisible == true )
	{
		nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetFocus, 0, 0, 0, 0 );
	}
}

//-----------------------------------------------------------------------------
void CStoreSellWnd::SetSellItemInfo( SPcItem* _pc_item, BYTE _item_tab )
{
	PcItem	= _pc_item;
	ItemTab	= _item_tab;
	NumItem	= PcItem->Amount;

	IconImg->SetClientImage( PcItem->Tid );
	NumTxt->SetText( _RT("") );

	char str[128] = "";

#ifdef CSELL_ITEMSHOP

	float Price = 0.0f;   
	#ifdef CPK_SYSTEM
		Price = (float)PcItem->ItemTable->thePrice * ::GetSellShopRatioFromPKPoint( nRui->GetPcParam()->thePKPoint );
	#else
		Price = (float)PcItem->ItemTable->thePrice * SELLSTOR_RATIO;
	#endif

	if( PcItem->IType == n_ITYPE_WEAR && PcItem->ItemTable->theMaxEndurance )
	{
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_SELL_PRICE ), (int)Price * PcItem->WearTable.theEndurance / PcItem->ItemTable->theMaxEndurance );
	}
	else
	{
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_SELL_PRICE ), (int)Price );
	}

#else
	
	if( PcItem->IType == n_ITYPE_WEAR && PcItem->ItemTable->theMaxEndurance )
	{
		//PcItem->ItemTable->thePrice / 2 * PcItem->WearTable.theEndurance / PcItem->ItemTable->theMaxEndurance 
		double price = PcItem->ItemTable->thePrice * 0.5;
		double price2 = (float)PcItem->WearTable.theEndurance / (float)PcItem->ItemTable->theMaxEndurance ;
		double Result =  (price * price2) + 0.5f ;
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_SELL_PRICE ), (DWORD)(Result ) );
		
	}
	else
	{
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_SELL_PRICE ), PcItem->ItemTable->thePrice / 2 );
	}

#endif	
	
	PriceTxt->SetText( str );

	nRui->SendEvent( WID_ItemSellWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SetModal( WID_ItemSellWnd );

	if( ChatBoxWnd.IsVisible == true )
	{
		nRui->SendEvent( WID_ChatBox_MessageEdit, n_emEditDisable, 0, 0, 0, 0 );
	}
}

//-----------------------------------------------------------------------------
void CStoreSellWnd::CloseNumberInputWnd( SPcItem* _pc_item, int _num_item, BYTE _item_tab )
{
	char str[128] = "";

	PcItem	= _pc_item;
	NumItem	= _num_item;
	ItemTab	= _item_tab;

	IconImg->SetClientImage( PcItem->Tid );

	Rsprintf( RWCHAR(str), _RT("%d"), NumItem );
	NumTxt->SetText( str );
	
#ifdef CSELL_ITEMSHOP
	float Price = 0.0f;   
	#ifdef CPK_SYSTEM
		Price = (float)PcItem->ItemTable->thePrice * ::GetSellShopRatioFromPKPoint( nRui->GetPcParam()->thePKPoint );
	#else
		Price = (float)PcItem->ItemTable->thePrice * SELLSTOR_RATIO;
	#endif
	Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_SELL_PRICE ), (int)Price * NumItem );
#else
	Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_WND_ITEM_SELL_PRICE ), (PcItem->ItemTable->thePrice / 2 ) * NumItem );
#endif
	
	PriceTxt->SetText( str );

	nRui->SendEvent( WID_ItemSellWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SetModal( WID_ItemSellWnd );
}

//-----------------------------------------------------------------------------
