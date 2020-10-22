#include "..\\RockPCH.h"

#include "Rui.h"
#include "..\\..\\brother.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"

#include "Resource.h"
#include "Chain.h"
#include "Container.h"



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\bravolibs\\obj\\SkillSystem.h"

//#include "pc.h"
#include "ItemAccEnchant.h"
#include "StringManager.h"


CItemAccEnchant	g_ItemAccEnchant;



//-----------------------------------------------------------------------------
void CItemAccEnchant::Init()
{
	
	mcp_Item = NULL;

	mcp_effect[0] = NULL;
	mcp_effect[1] = NULL;
	mcp_effect[2] = NULL;
}

///---------------------------------------------------------------------------
///-- OpenEnchantWnd
///---------------------------------------------------------------------------
///-- 개조 창을 연다
///-- Slot 을 초기화 한다.
///-- 
void CItemAccEnchant::OpenEnchantWnd()
{
	int li_max =0;
	
	
	TCHAR tmpStr[128] ={0,};
	if( IsVisible )
	{
		return;
	}
	
//	mi_skill_level = li_max;
	
	nRui->SendEvent( WID_ACCESSORY_ENCHANT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
	Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );
	mcp_effect[0]->SetText( tmpStr );				
	mcp_effect[1]->SetText( tmpStr );			
	mcp_effect[2]->SetText( tmpStr );	
	
	mcp_item_slot->PcItem->Clear();
	mcp_production_slot->PcItem->Clear();
	mcp_start_btn->SetBtnHold( true );
	mb_button_lock = false;	
}
///---------------------------------------------------------------------------
///-- CloseEnchantWnd
///---------------------------------------------------------------------------
///-- 개조 창을 닫는다.
///-- 각 Resource 를 해제한다. ( Dynamic Texture_M 와 연동되어야 함 ) 
void CItemAccEnchant::CloseEnchantWnd()
{
	nRui->SendEvent( WID_SUB_ENCHANT, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	///TID_ELD_WND_UpGrade 해제

}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CItemAccEnchant::Composition()
{
	CFrameWnd*		fw;
	CImageBox*		img;
	CTextBox*		tbox;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 204;
	enchantwnd.h	= 143;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	///-- Frame Wnd

	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ACCESSORY_ENCHANT, WID_None, enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );	
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );


	fw->SetCaption( _RT("악세서리 가공") );	//HAN_MSG						
	fw->SetTitle( true );	
	fw->SetWndProc( this );
	fw->SetVisible( false );
	fw->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ACCESSORY_ENCHANT,  1, 103, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

//	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ACCESSORY_ENCHANT, 1, 166, 202, 1, false );
//	img ->SetClientImage( TID_CTRL_LINEDOT );
		
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ACCESSORY_ENCHANT, 40, 56, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ACCESSORY_ENCHANT, 132, 56, 35, 35, false );            
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );


	///-- 아이템( Static Text )
	mcp_Item = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ACCESSORY_ENCHANT, 18, 36, 80, 17, false );
	mcp_Item->SetFontR( n_fRsvFontWndStatic );
	mcp_Item->SetText( _RT("악세서리"));

	///-- 제작서( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ACCESSORY_ENCHANT, 110, 36, 80, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( _RT("제작서")); //HAN_MSG
		
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- 시작 버튼

	///-- 닫기
	CloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ACCESSORY_ENCHANT, 185, 6, 13, 13, false );
	CloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	mcp_start_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ACCESSORY_ENCHANT, 31, 110, 58, 25, false );
	mcp_start_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	mcp_start_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_start_btn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) ); 
	mcp_start_btn->SetBtnHold( false );

	///-- 중단
	StopBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_ACCESSORY_ENCHANT, 119, 110, 58, 25, false );
	StopBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );	
	StopBtn->SetFontR( n_fRsvFontWndButton ); 
	StopBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 
	
		
	///-- 아이템 Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None ,  WID_ACCESSORY_ENCHANT, 40, 56, 35, 35, false );	
	mcp_item_slot->SetSlotGroup( n_SGROUP_TUNING );

	///-- Elda Slot
	mcp_production_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_ACCESSORY_ENCHANT, 132, 56, 35, 35, false );
	mcp_production_slot->SetSlotGroup( n_SGROUP_TUNING );

	///-- None Item 지정
	mcp_item_slot->PcItem = &mc_itemslot_item;		
	mcp_production_slot->PcItem = &mc_production_slot;

	mcp_effect[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT1, WID_ACCESSORY_ENCHANT, 5, 114, 190, 14, false );	
	mcp_effect[0]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[0]->SetBlank( 0, 0 );	

	mcp_effect[1]= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT2, WID_ACCESSORY_ENCHANT, 5, 132, 190, 14, false );
	mcp_effect[1]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[1]->SetBlank( 0, 0 );	

	mcp_effect[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT3, WID_ACCESSORY_ENCHANT, 5, 150, 190, 14, false );	
	mcp_effect[2]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[2]->SetBlank( 0, 0 );	
	



}

///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
///-- 각 엘다 스톤에 따른 Upgrade 결과를 보여준다.
void CItemAccEnchant::Update()
{
	static TCHAR tmpStr[128] ={0,};	

	///mcp_production_slot->PcItem->ItemTable->
	if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE && 
		mcp_production_slot->PcItem->IType != n_ITYPE_NONE )
	{	
		///-- enchant Text
		EnchantText();

		///-- Level
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), mcp_item_slot->PcItem->ItemTable->theGrade );
	
	}
	else
	{
		//-- Effect 1
		Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );
		mcp_effect[0]->SetText( tmpStr );				
		mcp_effect[1]->SetText( tmpStr );			
		mcp_effect[2]->SetText( tmpStr );	
		
	}
}


///---------------------------------------------------------------------------
///-- CheckItemList - Inven 변경에 따른 인첸트 창의 변경
///---------------------------------------------------------------------------
BOOL CItemAccEnchant::CheckEnchantItemList()
{
	int i = 0;
	int j = 0;
	
	bool lb_isFind_item = FALSE;
	bool lb_isFind_elda = FALSE;
	
	
	if( !mcp_item_slot->PcItem && !mcp_production_slot->PcItem ) 
	{
		return FALSE;
	}

	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( j = 0; j < c_MaxInvenItemNum; ++j )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
		{
			continue;
		}
		
		///-- item slot
		if( mcp_item_slot->PcItem && 
			mcp_item_slot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_item = true;			
		}

		///-- Elda slot
		if( mcp_production_slot->PcItem && 
			mcp_production_slot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_elda = true;
		}
	}

	///-----------------------------------------------------------------------
	///-- item 을 찾지 못 했을 경우
	if( !lb_isFind_item )
	{
		mcp_item_slot->PcItem->Clear();
	}
	///-- elda 를 찾지 못 했을 경우
	if( !lb_isFind_elda )
	{
		mcp_production_slot->PcItem->Clear();
	}
	
	///-----------------------------------------------------------------------
	///-- 두 Slot 에 모두 Item 이 있다면 시작 준비
	if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE && 
		mcp_production_slot->PcItem->IType != n_ITYPE_NONE )
	{
		mcp_start_btn->SetBtnHold( false );				
	}
	else
	{
		mcp_start_btn->SetBtnHold( true );
	}

	/*
	if( isFind )
	{
	if( mcp_item_slot->PcItem->IType == n_ITYPE_USE )
		{
			// 실제 가지고 있는것 보다 많이 교환하려구 하면...교환창에 넣고 아이템 떨군게야...ㅡㅡ; 사기꾼!!! 
			if( mcp_item_slot->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
			{
				mcp_item_slot->PcItem = NULL;
				//tslot->PcItem = NULL;//->Clear();
				return TRUE;
			}
		}			
	}
	else
	{
		mcp_item_slot->PcItem = NULL;
		return TRUE;
	}*/

	return FALSE;
}


//-----------------------------------------------------------------------------
void CItemAccEnchant::Proc( SEventMessage* EMsg )
{

	static DWORD	dTempItem[3];
	memset( dTempItem, 0, sizeof(dTempItem) );

	if( EMsg->FocusWnd == CloseBtn->GetID() ||
		EMsg->FocusWnd == StopBtn->GetID())
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_ACCESSORY_ENCHANT, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
	}
	// 장식품 , 제작서 
	else if( EMsg->FocusWnd == mcp_item_slot->GetID() ||  
		EMsg->FocusWnd == mcp_production_slot->GetID())
	{
		if(EMsg->Notify == NM_SLOT_CLICK)
		{
			ItemSlotClick(EMsg);
		}
		else if(EMsg->Notify == NM_SLOT_DBCLICK)
		{
			ItemSlotDBClick(EMsg);
		}
	}
	else if(EMsg->FocusWnd == mcp_start_btn->GetID())
	{
		if( EMsg->Notify == NM_BTN_CLICK && !mb_button_lock )
		{
			  
			
			g_RockClient.Send_CsSetStone( mcp_item_slot->PcItem->ItemUID, 
										  mcp_production_slot->PcItem->ItemUID, dTempItem );
			mb_button_lock = true;
		}
	}

	///-- 두 Slot 에 모두 Item 이 있다면 시작 준비
	if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE && 
		mcp_production_slot->PcItem->IType != n_ITYPE_NONE )
	{
		mcp_start_btn->SetBtnHold( false );				
	}
	else
	{
		mcp_start_btn->SetBtnHold( true );
	}

}

///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item 제거
void CItemAccEnchant::ItemSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();
		
	if( tslot->PcItem )
	{
		tslot->PcItem->Clear();		
	}

	return;
}
///---------------------------------------------------------------------------
///-- ItemSlotClick
///---------------------------------------------------------------------------
///-- Item 올리기
///-- !!!! 원석과 스톤의 구분은 Hard Coding
void CItemAccEnchant::ItemSlotClick( SEventMessage* EMsg )
{
	CSlot*		targetslot	= NULL;//현재 장비 슬롯
	CSlot*		srcslot		= NULL;//선택된(pick) 인벤 슬롯
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;

	///-- 아이템을 들고 있을 경우
	if( nRui->GetCursorItem() != NULL )
	{
		targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		srcslot = nRui->GetItemSelectedSlot();

		if( srcslot->PcItem == NULL )
		{
			return;
		}		
		// 같은 아이템이면 커서삭제 
		if( targetslot->PcItem && (targetslot->PcItem == srcslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}


		///-------------------------------------------------------------------
		///-- Item Slot
		if( targetslot == mcp_item_slot )
		{
			if(  srcslot->Group == n_SGROUP_INVENTORY   )
			{
			
				if(srcslot->PcItem->WearTable.theType == n_Accessory)
				{
					targetslot->SetSlotPcItem( *(srcslot->PcItem) );								
				}
				else
				{
					SystemPrint( n_DarkRed, n_SystemMsg, "악세서리 아이템이 아닙니다." ); //HAN_MSG
				}

				nRui->ResetCursorItem();					
			}
			else if ( srcslot->Group == n_SGROUP_WEAR)
			{				
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_ENCHANT_WEAR ) );			
			}
		}
		///-------------------------------------------------------------------
		///-- elda Slot
		else if( targetslot == mcp_production_slot && srcslot->PcItem->Code > 6210)
		{
			if( ( srcslot->Group == n_SGROUP_INVENTORY ) && ( srcslot->PcItem->Class == n_Class_Eldastone ) )
			{	
							
				///-- Copy item
				targetslot->SetSlotPcItem( *(srcslot->PcItem) );	
				targetslot->PcItem->Amount = 1;
				//targetslot->PcItem = srcslot->PcItem;

				///-- Del Cursor
				nRui->ResetCursorItem();

			}
		}

	}
	/*
	else
	{
	//커서에 아이템 붙이기
	targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
	
	  nRui->SetCursorItem( targetslot->PcItem );
	  nRui->SetItemSelectedSlot( targetslot );
	  }
	*/
}

bool CItemAccEnchant::IsEnchantItemSubJob(CSlot * pSlot)
{
	if(NULL == pSlot)
	{
		return false;
	}	
	
	bool bIsEnchantItemSubJob = false;
	
	switch(pSlot->PcItem->IType)
	{
				 
		case n_Type_Sword:								///-- 검			
		case n_Type_TwohandSword:						///-- 대검
		case n_Type_Bow:								///-- 활
			{
				bIsEnchantItemSubJob = true;					
			}
			break;

	}


	return bIsEnchantItemSubJob;
}

bool CItemAccEnchant::IsEnchantEldaSubJob(CSlot * pSlot)
{
	if(NULL == pSlot)
	{
		return false;
	}	
	
	bool bIsEnchantEldaSubJob = true;
	SItemBaseInfo * pItemBaseInfo = pSlot->PcItem->ItemTable;

	if(mb_craft_weapon)
	{
		//웨폰 크래프트일 경우 - 방어속성의 엘다스톤 장착 불가
		if(  pItemBaseInfo->theAttackDef  ||  pItemBaseInfo->theMagicRes )	
		//자주색(마법방어) 연두색(물리방어)
		{
			bIsEnchantEldaSubJob = false;
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ));	   
		}
	}
	else
	{
		//아머 크래프트일 경우 - 공격속성의 엘다스톤 장착 불가
		if(  pItemBaseInfo->theMinAttackBonus || pItemBaseInfo->theMaxAttackBonus ||
			 pItemBaseInfo->theMinMagicBonus || pItemBaseInfo->theMaxMagicBonus )	
		//푸른색(마법공격력) 붉은색(물리공격력)
		{
			bIsEnchantEldaSubJob = false;			
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_ARMOR ));	
		}
	}

	return bIsEnchantEldaSubJob;
}

///---------------------------------------------------------------------------
///-- EnchantText
///---------------------------------------------------------------------------
void CItemAccEnchant::EnchantText()
{
	int li_text_idx = 0;
	TCHAR tmpStr[128] = {0,};
	
	///-- Clear Text
	Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );
	mcp_effect[0]->SetText( tmpStr );				
	mcp_effect[1]->SetText( tmpStr );			
	mcp_effect[2]->SetText( tmpStr );	
	
	///-- 물리 공격력
	if( mc_production_slot.ItemTable->theMaxAttackBonus )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ) , mc_production_slot.ItemTable->theMaxAttackBonus );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 마법 공격력
	if( mc_production_slot.ItemTable->theMaxMagicBonus )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), mc_production_slot.ItemTable->theMaxMagicBonus );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 물리 방어력
	if( mc_production_slot.ItemTable->theAttackDef )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ), mc_production_slot.ItemTable->theAttackDef );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}

	///-- 마법 방어력
	if( mc_production_slot.ItemTable->theMagicRes )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ), mc_production_slot.ItemTable->theMagicRes );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	
	///-- 아이템 착용시 효과 
	///-- 파워
	if( mc_production_slot.ItemTable->thePower )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_POWER ), mc_production_slot.ItemTable->thePower );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	
	///-- 체력
	if( mc_production_slot.ItemTable->theVital )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_VITAL ), mc_production_slot.ItemTable->theVital );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 교감 
	if( mc_production_slot.ItemTable->theSympathy )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_SYMPATHY ), mc_production_slot.ItemTable->theSympathy );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 지능
	if( mc_production_slot.ItemTable->theInt )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_INTELLI ), mc_production_slot.ItemTable->theInt );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 지구력
	if( mc_production_slot.ItemTable->theStamina )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_STAMINA ), mc_production_slot.ItemTable->theStamina );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 민첩
	if( mc_production_slot.ItemTable->theDex )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_DEX ), mc_production_slot.ItemTable->theDex );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
				
				
}