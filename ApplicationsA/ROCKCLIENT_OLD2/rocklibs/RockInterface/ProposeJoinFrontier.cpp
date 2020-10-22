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
#include "ProposeJoinFrontier.h"
#include "StringManager.h"
#include "InventoryWndProc.h"
#include "NonPlayerInfoWnd.h"
#include "NumberInputWnd.h"


CProposeJoinFrontier	g_ProposeJoinFrontier;



//-----------------------------------------------------------------------------
void CProposeJoinFrontier::Init()
{
	
	mcp_Item = NULL;
	temp_src_slot = NULL;
	temp_tag_slot = NULL;

	mcp_effect[0] = NULL;
	mcp_effect[1] = NULL;
	mcp_effect[2] = NULL;
}

///---------------------------------------------------------------------------
///-- OpenEnchantWnd
///---------------------------------------------------------------------------
///-- ���� â�� ����
///-- Slot �� �ʱ�ȭ �Ѵ�.
///-- 
void CProposeJoinFrontier::OpenProposeWnd()
{
	int li_max = 0;
	m_Eld      = 0;
	
	TCHAR tmpStr[128] ={0,};
	if( IsVisible )
	{
		return;
	}
	
//	mi_skill_level = li_max;
	
	nRui->SendEvent( WID_PROPOSE_FRONTIER_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
	Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );
	mcp_effect[0]->SetText( tmpStr );				
	mcp_effect[1]->SetText( tmpStr );			
	mcp_effect[2]->SetText( tmpStr );
	
	temp_src_slot = NULL;
	temp_tag_slot = NULL;
	
	mcp_item_slot->PcItem->Clear();
//	mcp_production_slot->PcItem->Clear();
//	mcp_start_btn->SetBtnHold( true );
//	mb_button_lock = false;	
}
///---------------------------------------------------------------------------
///-- CloseEnchantWnd
///---------------------------------------------------------------------------

void CProposeJoinFrontier::Close()
{
	nRui->SendEvent( WID_PROPOSE_FRONTIER_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	///TID_ELD_WND_UpGrade ����

}
 
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CProposeJoinFrontier::Composition()
{
	CFrameWnd*		fw;
	CImageBox*		img;
	CImageBox9*     img9;


	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 304;
	enchantwnd.h	= 143;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	///-- Frame Wnd

	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_PROPOSE_FRONTIER_WND, WID_None, enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );	
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );


	fw->SetCaption( _RT("��� �ʴ�") );	//HAN_MSG						
	fw->SetTitle( true );	
	fw->SetWndProc( this );
	fw->SetVisible( false );
	fw->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PROPOSE_FRONTIER_WND,  1, 103, 302, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, 
										WID_PROPOSE_FRONTIER_WND, 123, 60, 120, 21, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

//	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ACCESSORY_ENCHANT, 1, 166, 202, 1, false );
//	img ->SetClientImage( TID_CTRL_LINEDOT );
		
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PROPOSE_FRONTIER_WND, 40, 56, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	///-- Slot
//	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PROPOSE_FRONTIER_WND, 132, 56, 35, 35, false );            
//	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );


	///-- ������( Static Text )
	mcp_Item = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PROPOSE_FRONTIER_WND, 18, 36, 80, 17, false );
	mcp_Item->SetFontR( n_fRsvFontWndStatic );
	mcp_Item->SetText( _RT("����������"));

	///-- ���ۼ�( Static Text )
//	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PROPOSE_FRONTIER_WND, 110, 36, 80, 17, false );
//	tbox->SetFontR( n_fRsvFontWndStatic );	
//	tbox->SetText( _RT("���ۼ�")); //HAN_MSG
		
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- ���� ��ư

	///-- �ݱ�
	CloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PROPOSE_FRONTIER_WND, 285, 6, 13, 13, false );
	CloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	mcp_start_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PROPOSE_FRONTIER_WND, 81, 110, 58, 25, false );
	mcp_start_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	mcp_start_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_start_btn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) ); 
	mcp_start_btn->SetBtnHold( false );

	///-- �ߴ�
	StopBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PROPOSE_FRONTIER_WND, 169, 110, 58, 25, false );
	StopBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );	
	StopBtn->SetFontR( n_fRsvFontWndButton ); 
	StopBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 
	
		
	///-- ������ Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None ,  WID_PROPOSE_FRONTIER_WND, 40, 56, 35, 35, false );	
	mcp_item_slot->SetSlotGroup( n_SGROUP_PROPOSE_FRONTIER );

	///-- Elda Button
	m_Eldbtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_PROPOSE_FRONTIER_ELDBTN, WID_PROPOSE_FRONTIER_WND, 100, 60, 23, 23, false );     
	m_Eldbtn->SetBtnImage( TID_CTRL_BTN_ELD, TID_CTRL_BTN_ELD_OVR, TID_CTRL_BTN_ELD_CLK, TID_None );
	

	m_EldText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PROPOSE_FRONTIER_ELDTEXT, WID_PROPOSE_FRONTIER_WND, 123, 60, 120, 21, false );
	m_EldText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_EldText->SetClientImage( TID_None );
	m_EldText->SetAlignText( n_atRight, n_atCenter );
	m_EldText->SetText( _RT("0") );
	m_EldText->SetTextColor( 255, 255, 255 );
	m_EldText->SetBlank( 2, 0 );

	///-- Elda Slot
//	mcp_production_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PROPOSE_FRONTIER_WND, 132, 56, 35, 35, false );
//	mcp_production_slot->SetSlotGroup( n_SGROUP_TUNING );

	///-- None Item ����
	mcp_item_slot->PcItem = &mc_itemslot_item;		
//	mcp_production_slot->PcItem = &mc_production_slot;

	mcp_effect[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT1, WID_PROPOSE_FRONTIER_WND, 5, 114, 190, 14, false );	
	mcp_effect[0]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[0]->SetBlank( 0, 0 );	

	mcp_effect[1]= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT2, WID_PROPOSE_FRONTIER_WND, 5, 132, 190, 14, false );
	mcp_effect[1]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[1]->SetBlank( 0, 0 );	

	mcp_effect[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT3, WID_PROPOSE_FRONTIER_WND, 5, 150, 190, 14, false );	
	mcp_effect[2]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[2]->SetBlank( 0, 0 );	
	



}

///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
///-- �� ���� ���濡 ���� Upgrade ����� �����ش�.
void CProposeJoinFrontier::Update()
{
	/*
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
	*/
}


///---------------------------------------------------------------------------
///-- CheckItemList - Inven ���濡 ���� ����ʴ� â�� ����
///---------------------------------------------------------------------------
BOOL CProposeJoinFrontier::CheckProposeFrontierItemList()
{
	int i = 0;
	int j = 0;
	
	bool lb_isFind_item = FALSE;
	bool lb_isFind_elda = FALSE;
	
	
	if( !mcp_item_slot->PcItem  ) 
	{
		return FALSE;
	}

	///-----------------------------------------------------------------------
	///-- ��ü �κ��丮 Access
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
/*
		if( mcp_production_slot->PcItem && 
			mcp_production_slot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_elda = true;
		}
*/
	}

	///-----------------------------------------------------------------------
	///-- item �� ã�� �� ���� ���
	if( !lb_isFind_item )
	{
		mcp_item_slot->PcItem->Clear();
	}
	///-- elda �� ã�� �� ���� ���
/*
	if( !lb_isFind_elda )
	{
		mcp_production_slot->PcItem->Clear();
	}
*/	
	///-----------------------------------------------------------------------
	///-- �� Slot �� ��� Item �� �ִٸ� ���� �غ�
	if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE )
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
			// ���� ������ �ִ°� ���� ���� ��ȯ�Ϸ��� �ϸ�...��ȯâ�� �ְ� ������ �����Ծ�...�Ѥ�; ����!!! 
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
void CProposeJoinFrontier::Proc( SEventMessage* EMsg )
{

	static DWORD	dTempItem[3];
	_SExItem        ItemInfo;
	memset( dTempItem, 0, sizeof(dTempItem) );

	if( EMsg->FocusWnd == CloseBtn->GetID() ||
		EMsg->FocusWnd == StopBtn->GetID())
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
			IsVisible = false;
		}
	}
	else if( EMsg->FocusWnd == m_Eldbtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_MoneyThrowWnd.Open( n_ELDINPUT_FRONTIER_PRESENT );
		}
	}
	// ���ǰ , ���ۼ� 
	else if( EMsg->FocusWnd == mcp_item_slot->GetID() )
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
		if( EMsg->Notify == NM_BTN_CLICK )
		{
					
			ItemInfo.theBundle = mcp_item_slot->PcItem->Amount;
			ItemInfo.theItemUID = mcp_item_slot->PcItem->ItemUID;
#ifdef DECO_RENEWAL_GUILD_MJH	
			g_RockClient.Send_CsMsngrProposeJoinFrontier((DWORD)g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lUnique,
				m_Eld , ItemInfo );
#endif // DECO_RENEWAL_GUILD_MJH
			Close();
		}
	}

	///-- �� Slot �� ��� Item �� �ִٸ� ���� �غ�
	
	//if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE  )
	//{
	//	mcp_start_btn->SetBtnHold( false );				
	//}
//	else
//	{
//		mcp_start_btn->SetBtnHold( true );
//	}
	

}

///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item ����
void CProposeJoinFrontier::ItemSlotDBClick( SEventMessage* EMsg )
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
///-- Item �ø���
void CProposeJoinFrontier::ItemSlotClick( SEventMessage* EMsg )
{
	CSlot*		targetslot	= NULL;//���� ��� ����
	CSlot*		srcslot		= NULL;//���õ�(pick) �κ� ����
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;

	///-- �������� ��� ���� ���
	if( nRui->GetCursorItem() != NULL )
	{
		targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		srcslot = nRui->GetItemSelectedSlot();

		if( srcslot->PcItem == NULL )
		{
			return;
		}		
		// ���� �������̸� Ŀ������ 
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
				if( srcslot->PcItem->ItemTable->theBundle != 1 && srcslot->PcItem->Amount > 1 ) 
						{
							g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_PROPOSEFRON, targetslot->PcItem );
							temp_src_slot = srcslot->SlotAddr;			
							temp_tag_slot = targetslot;
							nRui->ResetCursorItem();
						}
				//if(srcslot->PcItem->WearTable.theType == n_Accessory)
			//	{
				else
					targetslot->SetSlotPcItem( *(srcslot->PcItem) );								
			//	}
			//	else
			//	{
			//		SystemPrint( n_DarkRed, n_SystemMsg, "�Ǽ����� �������� �ƴմϴ�." ); //HAN_MSG
			//	}

				nRui->ResetCursorItem();					
			}
			else if ( srcslot->Group == n_SGROUP_WEAR)
			{				
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_ENCHANT_WEAR ) );			
			}
		}
		///-------------------------------------------------------------------
		///-- elda Slot
/*
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
*/

	}
	/*
	else
	{
	//Ŀ���� ������ ���̱�
	targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
	
	  nRui->SetCursorItem( targetslot->PcItem );
	  nRui->SetItemSelectedSlot( targetslot );
	  }
	*/
}
///---------------------------------------------------------------------------
///-- CloseNumberInputWnd
///---------------------------------------------------------------------------
void CProposeJoinFrontier::CloseNumberInputWnd( int aNumItem )
{
	if( !temp_src_slot )
	{
		return;
	}

	if( temp_src_slot->PcItem )
	{
		if( temp_src_slot->PcItem->Amount >= aNumItem )
		{
			temp_tag_slot->SetSlotPcItem( *(temp_src_slot->PcItem) );				
			temp_tag_slot->PcItem->Amount = aNumItem;
			mcp_item_slot = temp_tag_slot;
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_VOLUME ) );
		}
	}

	return;
}
/*
bool CProposeJoinFrontier::IsEnchantItemSubJob(CSlot * pSlot)
{
	if(NULL == pSlot)
	{
		return false;
	}	
	
	bool bIsEnchantItemSubJob = false;
	
	switch(pSlot->PcItem->IType)
	{
				 
		case n_Type_Sword:								///-- ��			
		case n_Type_TwohandSword:						///-- ���
		case n_Type_Bow:								///-- Ȱ
			{
				bIsEnchantItemSubJob = true;					
			}
			break;

	}


	return bIsEnchantItemSubJob;
}

bool CProposeJoinFrontier::IsEnchantEldaSubJob(CSlot * pSlot)
{
	if(NULL == pSlot)
	{
		return false;
	}	
	
	bool bIsEnchantEldaSubJob = true;
	SItemBaseInfo * pItemBaseInfo = pSlot->PcItem->ItemTable;

	if(mb_craft_weapon)
	{
		//���� ũ����Ʈ�� ��� - ���Ӽ��� ���ٽ��� ���� �Ұ�
		if(  pItemBaseInfo->theAttackDef  ||  pItemBaseInfo->theMagicRes )	
		//���ֻ�(�������) ���λ�(�������)
		{
			bIsEnchantEldaSubJob = false;
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ));	   
		}
	}
	else
	{
		//�Ƹ� ũ����Ʈ�� ��� - ���ݼӼ��� ���ٽ��� ���� �Ұ�
		if(  pItemBaseInfo->theMinAttackBonus || pItemBaseInfo->theMaxAttackBonus ||
			 pItemBaseInfo->theMinMagicBonus || pItemBaseInfo->theMaxMagicBonus )	
		//Ǫ����(�������ݷ�) ������(�������ݷ�)
		{
			bIsEnchantEldaSubJob = false;			
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_ARMOR ));	
		}
	}

	return bIsEnchantEldaSubJob;
}
*/
///---------------------------------------------------------------------------
///-- EnchantText
///---------------------------------------------------------------------------
/*
void CProposeJoinFrontier::EnchantText()
{
	int li_text_idx = 0;
	TCHAR tmpStr[128] = {0,};
	
	///-- Clear Text
	Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );
	mcp_effect[0]->SetText( tmpStr );				
	mcp_effect[1]->SetText( tmpStr );			
	mcp_effect[2]->SetText( tmpStr );	
	
	///-- ���� ���ݷ�
	if( mc_production_slot.ItemTable->theMaxAttackBonus )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ) , mc_production_slot.ItemTable->theMaxAttackBonus );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- ���� ���ݷ�
	if( mc_production_slot.ItemTable->theMaxMagicBonus )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), mc_production_slot.ItemTable->theMaxMagicBonus );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- ���� ����
	if( mc_production_slot.ItemTable->theAttackDef )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ), mc_production_slot.ItemTable->theAttackDef );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}

	///-- ���� ����
	if( mc_production_slot.ItemTable->theMagicRes )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ), mc_production_slot.ItemTable->theMagicRes );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	
	///-- ������ ����� ȿ�� 
	///-- �Ŀ�
	if( mc_production_slot.ItemTable->thePower )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_POWER ), mc_production_slot.ItemTable->thePower );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	
	///-- ü��
	if( mc_production_slot.ItemTable->theVital )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_VITAL ), mc_production_slot.ItemTable->theVital );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- ���� 
	if( mc_production_slot.ItemTable->theSympathy )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_SYMPATHY ), mc_production_slot.ItemTable->theSympathy );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- ����
	if( mc_production_slot.ItemTable->theInt )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_INTELLI ), mc_production_slot.ItemTable->theInt );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- ������
	if( mc_production_slot.ItemTable->theStamina )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_STAMINA ), mc_production_slot.ItemTable->theStamina );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- ��ø
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
*/