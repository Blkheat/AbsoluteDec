#include "..\\RockPCH.h"

#include "Rui.h"
#include "..\\..\\brother.h"


#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\bravolibs\\obj\\SkillSystem.h"

//#include "pc.h"
#include "ItemRepairWndproc.h"
#include "StringManager.h"
#include "messageBox.h"




CItemRepair_WndProc	g_ItemRepair_WndProc;

 
//-----------------------------------------------------------------------------
void CItemRepair_WndProc::Init()
{
}

//-----------------------------------------------------------------------------
void CItemRepair_WndProc::Composition()
{

	CButton*		btn;
	CTextBox*		tbox;
	CImageBox*		img;
	

	int i,j;

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect wndrect;
	wndrect.w	= 204;
	wndrect.h	= 288;
	wndrect.x	= center_x - ( wndrect.w / 2 );
	wndrect.y	= center_y - ( wndrect.h / 2 );
	
	///-- Frame Wnd	
	FrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_REPAIR_WND, WID_None, wndrect.x, wndrect.y, wndrect.w, wndrect.h, false );
	FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	FrameWnd->SetWndTileSizeLR( 40, 29 );
	FrameWnd->SetWndTileSizeTB( 31, 11 );

	FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_REPAIR ) );
	FrameWnd->SetTitle( true );
	FrameWnd->SetWndProc( this );	
	FrameWnd->SetVisible( false );
	FrameWnd->Initialize();

	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_REPAIR_WND,  1, 45, 202, 155, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_REPAIR_WND, 1, 44, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_REPAIR_WND, 1, 200, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_REPAIR_WND, 1, 219, 202, 31, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_REPAIR_WND, 1, 218, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_REPAIR_WND, 1, 250, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- slot
	img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_REPAIR_WND, 4, 47, 183, 149, false );       
	img->SetClientImage( TID_C_BGSLOT_REPAIR ); 

	///-- eld icon
	img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_REPAIR_WND, 13, 223, 23, 23, false );            
	img->SetClientImage( TID_CTRL_BTN_ELD );
	
	///-- eld text
	img= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_REPAIR_WND, 36, 223, 132, 23, false );        
	img->SetClientImage( TID_CTRL_IMG_ELDTEXT132 ); 

		///-- 수리비용 ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_REPAIR_WND, 0 , 202 , 204 , 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic2 );
	tbox->SetText( G_STRING_CHAR( IDS_WND_REPAIR_COST ) );
	tbox->SetAlignText(n_atCenter , n_atCenter );
	tbox->SetTextColor( 0 , 0 , 0);



	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------		
	///-- 닫기
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_REPAIR_BT_CLOSE, WID_REPAIR_WND, 185, 6, 13, 13, false ); 
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- 수리비용  
	mcp_cost_text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_REPAIR_BT_TEXT, WID_REPAIR_WND, 95, 226, 70, 17, false );
	mcp_cost_text->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	mcp_cost_text->SetClientImage( TID_None );
	mcp_cost_text->SetClientColor( 50, 50, 50 );
	mcp_cost_text->SetAlignText( n_atRight, n_atCenter );
	mcp_cost_text->SetTextColor( 255, 255, 255 );	

	///-- 수리
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_REPAIR_BT_REPAIR, WID_REPAIR_WND, 125, 256, 58, 25, false );   
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );		
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_REPAIR_BT ) );	
	
	///-- 전체선택
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_REPAIR_BT_ALLITEM, WID_REPAIR_WND,  29, 256, 87, 25, false );   
	btn->SetBtnImage( TID_CTRL_BTN87, TID_CTRL_BTN87_OVR, TID_CTRL_BTN87_CLK, TID_None );				
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_REPAIR_ALLITEM ));	
	
	
	///-- Slot
	for( i = 0 ; i< 4; i++)
	{
		for( j = 0 ; j < 5 ; ++j )
		{			
			mpc_slots[i][j] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_REPAIR_WND, 4 + j * 37,  47 + i * 38 , 35, 35, false );
			mpc_slots[i][j]->PcItem = &mpc_items[i][j];	
			mpc_slots[i][j]->SetSlotGroup( n_SGROUP_NONE );
			mpc_slots[i][j]->SetHID( HID_ITEMTIP );
			///mpc_slots[i][j]->SetSlotNum( 0 );
			
		}
	}

	mpc_scrollbar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_REPAIR_BT_SCROLL, WID_REPAIR_WND, 188, 47, 15, 149, false );	
	mpc_scrollbar->InitSlide( stVertSld, 0, 10, 0, 13 );//Type, min, max, value, 썸의 폭
	mpc_scrollbar->SetClientImage( TID_CTRL_SCR_BG );
	mpc_scrollbar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	mpc_scrollbar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	mpc_scrollbar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	
	/*
	
	m_pMapComboBox = (CComboBox*)nRui->CreateWnd( n_wtComboBox, WID_None, WID_REPAIR_WND, 0, 120, 184, 86, false );			
	m_pMapComboBox->SetListboxTileSizeTB( 5, 5 );
	m_pMapComboBox->SetListboxTileSizeLR( 5, 5 );
	m_pMapComboBox->SetListboxImage9( TID_CTRL_OUTLINE_L1 );
	m_pMapComboBox->SetVisible( true );

	m_pMapComboBox->AddItem( "test1", 0 );		
	m_pMapComboBox->AddItem( "test2", 1 );		
	m_pMapComboBox->AddItem( "test3", 2 );		
	*/

//	m_pMapComboBox->AddItem( G_STRING_CHAR( IDS_MAP_PVP_ZONE ), c_RockMap_Millena_Rain_PVP_Zone );
//	m_pMapComboBox->AddItem( G_STRING_CHAR( IDS_MAP_PVP_ZONE ), c_RockMap_Millena_City );
//	
	/*
	
	
	
	///-- 아이템 Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_EXTRACT_SLOT_ITEM, WID_SUB_EXTRACT, 33, 56, 35, 35, false );	
	mcp_item_slot->SetSlotGroup( n_SGROUP_TUNING );

	///-- Elda Slot
	mcp_elda_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_EXTRACT_SLOT_TARGET, WID_SUB_EXTRACT, 132, 56, 35, 35, false );	
	mcp_elda_slot->SetSlotGroup( n_SGROUP_TUNING );

	///-- None Item 지정
	mcp_item_slot->PcItem = &mc_itemslot_item;		
	mcp_elda_slot->PcItem = &mc_eldaslot_item;
	
	///-- Exp
	mcp_extract_Desc = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_EXTRACT_TEXT1, WID_SUB_EXTRACT, 5, 132, 190, 14, false );
	mcp_extract_Desc->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	mcp_extract_Desc->SetClientColor( 255, 255, 255 );
	mcp_extract_Desc->SetClientImage( TID_None );	
	mcp_extract_Desc->SetAlignText( n_atCenter, n_atCenter );	
	mcp_extract_Desc->SetTextColor( 255, 255, 255 );
	mcp_extract_Desc->SetBlank( 0, 0 );	
	
*/

}

//-----------------------------------------------------------------------------
void CItemRepair_WndProc::SetWheelValue( short _wheel )
{
	int value = 0;

	mpc_scrollbar->GetValue( &value );
	if( ( _wheel < 0 ) && ( value < 10 ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	mpc_scrollbar->SetValue( value );
	SetSlotScroll( value );
}


//-----------------------------------------------------------------------------
void CItemRepair_WndProc::SetSlotScroll( int linenum )
{
	int i ,j;
	
	
	for( i = 0 ; i < 4; ++i )
	{
		for( j = 0 ; j < 5 ; ++j )
		{	
			mpc_slots[i][j]->PcItem = &mpc_items[ linenum + i][j];	
		}
	}	
}

///---------------------------------------------------------------------------
///-- Clear
///---------------------------------------------------------------------------
void CItemRepair_WndProc::Clear()
{
	int i,j;
	for( i = 0 ; i < 14 ; ++i )
	{
		for( j = 0 ; j < 5 ; ++j )
		{
			mpc_items[i][j].Clear();
		}
	}

	mb_button_lock = false;
	mpc_scrollbar->SetValue( 0 );
	//nRui->SendEvent( WID_REPAIR_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
	Gen_Cost();	

}
///---------------------------------------------------------------------------
///-- OpenEnchantRWnd
///---------------------------------------------------------------------------
void CItemRepair_WndProc::OpenRepair_Wnd( const bool ab_npc_repair  )
{
	int i,j;
	int li_max = 0;
	for( i = 0 ; i < 14 ; ++i )
	{
		for( j = 0 ; j < 5 ; ++j )
		{
			mpc_items[i][j].Clear();
		}
	}

	mpc_scrollbar->SetValue( 0 );
	mb_npc_repair = ab_npc_repair;
	mb_button_lock = false;
	mi_skill_max_code = 0;

	nRui->SendEvent( WID_REPAIR_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	
	///-- 전체 Skill 중에 개조 최고 레벨 찾음
	for( j = 0; j < RP_MAX_USER_SKILL; ++j )
	{	
		if( nRui->GetPcInvenInfo()->SkillItem[j] == NULL )
		{
			continue;
		}
		
		///-- Max Level 찾음
		if( nRui->GetPcInvenInfo()->SkillItem[j]->Code >= c_SkillCode_ItemRepair_Min &&  nRui->GetPcInvenInfo()->SkillItem[j]->Code <= c_SkillCode_ItemRepair_Max )
		{
			if( li_max < nRui->GetPcInvenInfo()->SkillItem[j]->Code - c_SkillCode_ItemRepair_Min +1 )
			{
				li_max = nRui->GetPcInvenInfo()->SkillItem[j]->Code - c_SkillCode_ItemRepair_Min +1;
				mi_skill_max_code = nRui->GetPcInvenInfo()->SkillItem[j]->Code;
			}
			
		}
	}

	mi_skill_level = li_max;
	

	Gen_Cost();	
}


//-----------------------------------------------------------------------------
void CItemRepair_WndProc::Proc( SEventMessage* EMsg )
{
	int i,j;
	int index = 0;
	///-- !!!!
	static int test =0 ;
	
	switch( EMsg->FocusWnd )
	{
		///-- 수리
	case WID_REPAIR_BT_REPAIR:		
		if( EMsg->Notify == NM_BTN_CLICK && !mb_button_lock && ml_count)
		{			
			SRpCsRequstRepair lMsg;


			Gen_Cost();
			///-- 소지금 Check
			if( ml_cost > nRui->thePcParam.Money)
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
				return;
			}
			
			for( i = 0 ; i < 14 ; ++i )
			{
				for( j = 0 ; j < 5 ; ++j )
				{
				if( mpc_items[i][j].IType != n_ITYPE_NONE )
					{
						lMsg.theItemID[index] = mpc_items[i][j].ItemUID ;
						++index;
					}
				}
			}
			if( mb_npc_repair )
			{
				lMsg.theRepairType = n_ByRepairNPC;
			}
			else
			{
				lMsg.theRepairType = n_ByRepairSkill;
			}
			lMsg.theCount = ml_count;
			lMsg.theSkillCode = mi_skill_max_code;

			g_RockClient.Send_CsRequstRepair( &lMsg );
			mb_button_lock = true;
		}
		break;
	case WID_REPAIR_BT_ALLITEM:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			//m_pMapComboBox->ClearItem();
			Select_All();
		}
		break;
	case WID_REPAIR_BT_SCROLL:
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetSlotScroll( (int)EMsg->fParam );
		}
	case WID_REPAIR_BT_CLOSE:
		
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_REPAIR_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;

		
	}

	///-- Slot Click
	for( i = 0 ; i < 4 ; ++i )
	{
		for( j = 0 ; j < 5 ; ++j )
		{
			if( mpc_slots[i][j]->GetID() == EMsg->FocusWnd )
			{
				if( EMsg->Notify == NM_SLOT_CLICK )			{	ItemSlotClick( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	ItemSlotDBClick( EMsg );	}				
			}
		}
	}

}
///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item 제거
void CItemRepair_WndProc::ItemSlotDBClick( SEventMessage* EMsg )
{

	int i,j;
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();
	
	
	for( i = 0 ; i < 4 ; ++i )
	{
		for( j = 0 ; j < 5 ; ++j )
		{
			if( tslot == mpc_slots[i][j] && tslot->PcItem )
			{
				tslot->PcItem->Clear();
				Gen_Cost();
				return;
			}
			
		}
	}
	Gen_Cost();
	return;
}
///---------------------------------------------------------------------------
///-- ItemSlotClick
///---------------------------------------------------------------------------
///-- Item 올리기
void CItemRepair_WndProc::ItemSlotClick( SEventMessage* EMsg )
{
	
	CSlot*		targetslot	= NULL;//현재 장비 슬롯
	CSlot*		srcslot		= NULL;//선택된(pick) 인벤 슬롯
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;
	int i,j;
	int k,l;

	///-- 아이템을 들고 있을 경우
	if( nRui->GetCursorItem() != NULL )
	{
		targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		srcslot = nRui->GetItemSelectedSlot();
		
		if( srcslot->PcItem == NULL )
		{
			return;
		}
		if( targetslot->PcItem && (targetslot->PcItem == srcslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		///-------------------------------------------------------------------
		///-- Item Slot
		for( i = 0 ; i < 4 ; ++i )
		{
			for( j = 0 ; j < 5 ; ++j )
			{
				if( targetslot == mpc_slots[i][j] && mpc_slots[i][j]->PcItem->IType == n_ITYPE_NONE )
				{
					if( srcslot->PcItem->Code >= c_NotRepairitem_Start &&  
						(srcslot->PcItem->ItemTable->theClass == n_Class_Weapon || srcslot->PcItem->ItemTable->theClass == n_Class_Accessory ) &&
						srcslot->PcItem->ItemTable->theType != n_Type_ShortWeapon ) 
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_DONTREPAIR ) );	
						nRui->ResetCursorItem();
						return;
					}
									
					///-- 수리 가능 조건 Check
					if( srcslot->PcItem->ItemTable->theMaxEndurance == 0 ||
						srcslot->PcItem->ItemTable->theMaxEndurance == srcslot->PcItem->WearTable.theEndurance )						
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_NEED_REPAIR ) );
						nRui->ResetCursorItem();
						return;
					}
					///-- Level Check
					if( mi_skill_level  < srcslot->PcItem->ItemTable->theGrade && !mb_npc_repair)						
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVER_ITEM_LEVEL ) );	
						nRui->ResetCursorItem();
						return;
					}
					

					///-- 중복 검사
					for( k = 0 ; k < 14; ++k )
					{
						for( l= 0 ; l < 5 ; ++l )
						{
							if( mpc_items[k][l].IType != n_ITYPE_NONE &&  mpc_items[k][l].ItemUID == srcslot->PcItem->ItemUID )								
							{
								nRui->ResetCursorItem();
								return;
							}
						}
					}
										
					///-- 아이템 올리기
					if( ( srcslot->Group == n_SGROUP_INVENTORY ||  srcslot->Group == n_SGROUP_WEAR ) && 
						( srcslot->PcItem->IType == n_ITYPE_WEAR ) )
					{
						///-- Copy item
						targetslot->SetSlotPcItem( *(srcslot->PcItem) );								
						///-- Del Cursor
						//targetslot->PcItem = srcslot->PcItem;
						nRui->ResetCursorItem();
					}	
					else
					{						 
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_DONTREPAIR ) );	
						nRui->ResetCursorItem();
						return;
					}
				}
			}
		}
	}	
	Gen_Cost();
}



///---------------------------------------------------------------------------
///-- CheckItemList
///---------------------------------------------------------------------------
BOOL CItemRepair_WndProc::CheckRepair_ItemList()
{
	
	int i = 0;
	int j = 0;
	int k = 0;
	
	bool lb_isFind_item = FALSE;
	bool lb_isFind_elda = FALSE;
	
	

	for( i = 0 ; i < 14; ++i )
	{
		for( j = 0 ; j < 5; ++j )
		{
			/*
			if( mpc_items[i][j ) 
			{
				return FALSE;
			}*/
			lb_isFind_item = false;
			///-----------------------------------------------------------------------
			///-- 전체 인벤토리 Access
			for( k = 0; k < c_MaxInvenItemNum; ++k )
			{	
				if( nRui->GetPcInvenInfo()->InvenItem[k] == NULL )
				{
					continue;
				}
				
				///-- item slot	
				if( mpc_items[i][j].ItemUID == nRui->GetPcInvenInfo()->InvenItem[k]->ItemUID )
				{
					lb_isFind_item = true;			
				}	
				
			}
			
			///-----------------------------------------------------------------------
			///-- 장착 인벤토리 Access
			for( k = 0; k < RP_MAX_EQUIP; ++k )
			{	
				if( nRui->GetPcInvenInfo()->WearItem[k] == NULL )
				{
					continue;
				}
				
				///-- item slot	
				if( mpc_items[i][j].ItemUID == nRui->GetPcInvenInfo()->WearItem[k]->ItemUID )
				{
					lb_isFind_item = true;			
				}	
				
			}
			
			///-----------------------------------------------------------------------
			///-- item 을 찾지 못 했을 경우
			if( !lb_isFind_item )
			{
				mpc_items[i][j].Clear();
			}
		}
	}
	Gen_Cost();
	return FALSE;
}
///---------------------------------------------------------------------------
///-- Select_All
///---------------------------------------------------------------------------
void CItemRepair_WndProc::Select_All()
{
	int i,j,k;
	for( i = 0 ; i < 14 ; ++i )
	{
		for( j = 0 ; j < 5 ; ++j )
		{
			mpc_items[i][j].Clear();
		}
	}
	
	i = 0;
	j = 0;
	///-----------------------------------------------------------------------
	///-- 장착 인벤토리 Access
	for( k = 0; k < RP_MAX_EQUIP; ++k )
	{	
		if( nRui->GetPcInvenInfo()->WearItem[k] == NULL )
		{
			continue;
		}

			
		///-- item slot	
		if( nRui->GetPcInvenInfo()->WearItem[k]->IType == n_ITYPE_WEAR ) 			
		{
				if( nRui->GetPcInvenInfo()->WearItem[k]->Code >= c_NotRepairitem_Start &&  
					(nRui->GetPcInvenInfo()->WearItem[k]->ItemTable->theClass == n_Class_Weapon || nRui->GetPcInvenInfo()->WearItem[k]->ItemTable->theClass == n_Class_Accessory ) &&
					nRui->GetPcInvenInfo()->WearItem[k]->ItemTable->theType != n_Type_ShortWeapon ) 			
		//	if( nRui->GetPcInvenInfo()->WearItem[k]->Code >= c_NotRepair_PayItemCode_Min && nRui->GetPcInvenInfo()->WearItem[k]->Code <= c_NotRepair_PayItemCode_Max )
			{
				continue;
			}

			///-- 조건  Check
			if( nRui->GetPcInvenInfo()->WearItem[k]->ItemTable->theMaxEndurance != 0 &&
				nRui->GetPcInvenInfo()->WearItem[k]->ItemTable->theMaxEndurance > nRui->GetPcInvenInfo()->WearItem[k]->WearTable.theEndurance )
			{
				if( mi_skill_level >= nRui->GetPcInvenInfo()->WearItem[k]->ItemTable->theGrade || mb_npc_repair )					
				{
					mpc_items[i][j] = *(nRui->GetPcInvenInfo()->WearItem[k]);
					++j;
					if( j >4 )
					{
						++i;
						j = 0;
					}
				}
			}
			
		}
		
	}
	///-- 전체 인벤토리 Access
	for( k = 0; k < c_MaxInvenItemNum; ++k )
	{
		if( nRui->GetPcInvenInfo()->InvenItem[k] == NULL )
		{
			continue;
		}
		if( nRui->GetPcInvenInfo()->InvenItem[k]->IType == n_ITYPE_WEAR ) 			
		{
			if( nRui->GetPcInvenInfo()->InvenItem[k]->Code >= c_NotRepairitem_Start &&  
			(nRui->GetPcInvenInfo()->InvenItem[k]->ItemTable->theClass == n_Class_Weapon || nRui->GetPcInvenInfo()->InvenItem[k]->ItemTable->theClass == n_Class_Accessory ) &&
			nRui->GetPcInvenInfo()->InvenItem[k]->ItemTable->theType != n_Type_ShortWeapon ) 	
			
			//if( nRui->GetPcInvenInfo()->InvenItem[k]->Code >= c_NotRepair_PayItemCode_Min && nRui->GetPcInvenInfo()->InvenItem[k]->Code <= c_NotRepair_PayItemCode_Max )
			{
				continue;
			}

			///-- 조건 Check
			if( nRui->GetPcInvenInfo()->InvenItem[k]->ItemTable->theMaxEndurance != 0 &&
				nRui->GetPcInvenInfo()->InvenItem[k]->ItemTable->theMaxEndurance > nRui->GetPcInvenInfo()->InvenItem[k]->WearTable.theEndurance )
			{
				if( mi_skill_level >= nRui->GetPcInvenInfo()->InvenItem[k]->ItemTable->theGrade || mb_npc_repair )					
				{
					mpc_items[i][j] = *(nRui->GetPcInvenInfo()->InvenItem[k]);
					++j;
					if( j >4 )
					{
						++i;
						j = 0;
					}
				}
			}
		}
	}
	///-- 전체 장비 Access
	
	Gen_Cost();
}
///---------------------------------------------------------------------------
///-- Refresh
///---------------------------------------------------------------------------
void CItemRepair_WndProc::Refresh( DWORD ad_uid )
{
	int i,j,k;
	///-- 전체 검색
	for( i = 0 ; i < 14; ++i )
	{
		for( j = 0 ; j < 5; ++j )
		{
			///-- 해당 Item 찾음
			if( mpc_items[i][j].IType != n_ITYPE_NONE && mpc_items[i][j].ItemUID == ad_uid )								
			{					
				///-----------------------------------------------------------------------
				///-- 장착 인벤토리 Access
				for( k = 0; k < RP_MAX_EQUIP; ++k )
				{	
					if( nRui->GetPcInvenInfo()->WearItem[k] == NULL )
					{
						continue;
					}					
					///-- 변경 Item 
					if( nRui->GetPcInvenInfo()->WearItem[k]->ItemUID == mpc_items[i][j].ItemUID )
					{
						mpc_items[i][j].Clear();
						mpc_items[i][j] = *(nRui->GetPcInvenInfo()->WearItem[k]);

					}
				}
				
				///-- 전체 인벤토리 Access
				for( k = 0; k < c_MaxInvenItemNum; ++k )
				{
					if( nRui->GetPcInvenInfo()->InvenItem[k] == NULL )
					{
						continue;
					}
					///-- 변경 Item 
					if( nRui->GetPcInvenInfo()->InvenItem[k]->ItemUID == mpc_items[i][j].ItemUID )
					{
						mpc_items[i][j].Clear();
						mpc_items[i][j] = *(nRui->GetPcInvenInfo()->InvenItem[k]);
					}
				}
			}
		}
	}
	Gen_Cost();
}
///---------------------------------------------------------------------------
///-- Gen_Cost
///---------------------------------------------------------------------------
void CItemRepair_WndProc::Gen_Cost()
{
	int i,j;
	char temp[100]={0,};
	DWORD ll_cost;	

	ml_cost = 0;
	ml_count = 0;

	///-- 전체 검색
	for( i = 0 ; i < 14; ++i )
	{
		for( j = 0 ; j < 5; ++j )
		{
			if( mpc_items[i][j].IType != n_ITYPE_NONE )								
			{				
				ml_count ++;	
				float price_rate1 = 60.0f;
				float price_rate2 = 40.0f; 
					
			//	SItemBaseInfo* ItemTable = mpc_items[i][j].ItemTable;
				
				if( mpc_items[i][j].Code > 10000 && mpc_items[i][j].ItemTable->theLimitFame > 0  )
				{
					price_rate1 = 80.0f;
					price_rate2 = 60.0f;
				}

				///-- 아이템 별 가격 결정
				if( mpc_items[i][j].WearTable.theEndurance == 0 )
				{
					ll_cost = (DWORD)((float)mpc_items[i][j].ItemTable->thePrice * price_rate1 / 100.0f) ;
				}
				else
				{
					
					DWORD tempItemCost =  (DWORD)((float)mpc_items[i][j].ItemTable->thePrice * price_rate2 /100.0f );
					DWORD tempEnduranceGap = mpc_items[i][j].ItemTable->theMaxEndurance - mpc_items[i][j].WearTable.theEndurance; 

					ll_cost =  ( tempItemCost * tempEnduranceGap ) /  mpc_items[i][j].ItemTable->theMaxEndurance  ;
					
				//ll_cost = mpc_items[i][j].ItemTable->thePrice* 40 / 100 * ( mpc_items[i][j].ItemTable->theMaxEndurance - mpc_items[i][j].WearTable.theEndurance ) / mpc_items[i][j].ItemTable->theMaxEndurance;
				}	
				

				///-- 수리 비용 결정
				if( mb_npc_repair )
				{
					ml_cost += ll_cost;
				}
				else
				{
					ml_cost += (DWORD)((float)ll_cost/2.0f);
				}
				
			}
			
		}

		
	}

	///-- Cost Text 
	Rsprintf( RWCHAR(temp), G_STRING_CHAR( IDS_WND_REPAIR_ELDA ), ml_cost );
	mcp_cost_text->SetText( temp );

}