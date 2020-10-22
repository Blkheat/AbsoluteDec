#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
//#include "pc.h"
#include "ItemEnchantRWndProc.h"
#include "stringmanager.h"

CItemEnchantRWndProc	g_ItemEnchant_RWndProc;


//-----------------------------------------------------------------------------
void CItemEnchantRWndProc::Init()
{
}

//-----------------------------------------------------------------------------
void CItemEnchantRWndProc::Composition()
{
	CFrameWnd*		fw;
	CButton*		btn;
	CTextBox*		tbox;
	CImageBox*		img;
		
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 204;
	enchantwnd.h	= 220;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	///-- Frame Wnd
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SUB_ENCHANT_RESULT, WID_None, enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );	
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );

	fw->SetCaption( G_STRING_CHAR( IDS_WND_ENCHANT_RESULT ) );	
	fw->SetTitle( true );
	fw->SetWndProc( this );
	fw->SetVisible( false );
	fw->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_ENCHANT_RESULT, 1, 111, 202, 72, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_ENCHANT_RESULT,  1, 110, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_ENCHANT_RESULT, 1, 183, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_ENCHANT_RESULT, 85, 52, 35, 35, false );  
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	///-- 원석 (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_ENCHANT_RESULT,  52, 93, 100, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_ENCHANT_RESULT_TEXT ) );

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_ENCHANT_RESULT_CLOSE, WID_SUB_ENCHANT_RESULT, 185, 6, 13, 13, false );    
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- 아이템 Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_ENCHANT_RESULT_SLOT_R, WID_SUB_ENCHANT_RESULT, 85, 52, 35, 35, false );
	mcp_item_slot->PcItem = &mc_itemslot_item;		
	mcp_item_slot->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot->SetHID( HID_ITEMTIP );

	///-- 확인 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_ENCHANT_RESULT_BTN_OK, WID_SUB_ENCHANT_RESULT, 74, 189, 58, 25, false );
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );		
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );

	///-- Exp
	mcp_exp = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_RESULT_TEXT1, WID_SUB_ENCHANT_RESULT, 5, 111, 190, 14, false );
	mcp_exp->SetFontR( n_fRsvFontWndStatic );	

	///-- Penalty 
	mcp_exp_penalty = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_ENCHANT_RESULT, 15, 124, 190, 14, false );
	mcp_exp_penalty->SetFontR( n_fRsvFontWndStatic );	
	
	///-- Effect
	mcp_effect[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_RESULT_TEXT2, WID_SUB_ENCHANT_RESULT, 5, 139, 190, 14, false );
	mcp_effect[0]->SetFontR( n_fRsvFontWndStatic );		

	///-- Effect
	mcp_effect[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_RESULT_TEXT3, WID_SUB_ENCHANT_RESULT, 5, 154, 190, 14, false );
	mcp_effect[1]->SetFontR( n_fRsvFontWndStatic );		
	
	///-- Effect
	mcp_effect[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_RESULT_TEXT4, WID_SUB_ENCHANT_RESULT, 5, 169, 190, 14, false );
	mcp_effect[2]->SetFontR( n_fRsvFontWndStatic );	

	///-- image	
	mcp_icon_x = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_SUB_ENCHANT_RESULT_IMG_X, WID_SUB_ENCHANT_RESULT, 89, 56, 27, 27, false );
	mcp_icon_x->SetClientImage( TID_C_WND_ELDA_SB_X );

	///-- Result text
	mcp_result_text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_ENCHANT_RESULT, 50, 37, 104, 12, false );
	mcp_result_text->SetFontR( n_fRsvFontWndStatic );

}
///---------------------------------------------------------------------------
///-- OpenEnchantRWnd
///---------------------------------------------------------------------------
///-- 아이템 강화 성공시 Wnd
void CItemEnchantRWndProc::OpenEnchantR_SWnd( DWORD ai_ItemUID , DWORD ai_eldaID , DWORD ai_exp, int ai_penalty )
{
	int i;
	TCHAR tmpStr[128] = {0,};

	if( IsVisible )
	{
		return;
	}
	mcp_item_slot->PcItem->Clear();	

	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[i] == NULL )
		{
			continue;
		}
		
		///-- item slot
		if( nRui->GetPcInvenInfo()->InvenItem[i]->ItemUID == ai_ItemUID)
		{
			///-- Copy Item
			mcp_item_slot->SetSlotPcItem( *(nRui->GetPcInvenInfo()->InvenItem[i]) );
		}	

		///-- 효과
		if( nRui->GetPcInvenInfo()->InvenItem[i]->ItemUID == ai_eldaID )
		{
			///-- Text Set
			EnchantText( nRui->GetPcInvenInfo()->InvenItem[i] );			
		}	

	}

	///-- Text Exp
	Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_WND_ITEM_GET_SUBEXP ), ai_exp );
	mcp_exp->SetText( tmpStr );
	
	if( ai_penalty )
	{			
		Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_WND_ITEM_LEVEL_PENALTY ), ai_penalty );
		mcp_exp_penalty->SetText( tmpStr );
	}
	else
	{
		mcp_exp_penalty->SetText( _RT("") );				
	}
	
	mcp_result_text->SetText( G_STRING_CHAR( IDS_SUCCESS ) );
	mcp_icon_x->SetVisible( false );	
	nRui->SendEvent( WID_SUB_ENCHANT_RESULT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}
///---------------------------------------------------------------------------
///-- OpenEnchantR_FWnd
///---------------------------------------------------------------------------
///-- 아이템 강화 실패시 Wnd
void CItemEnchantRWndProc::OpenEnchantR_FWnd( DWORD ai_DelStoneID , DWORD ai_exp, int ai_penalty )
{
	int i;
	TCHAR tmpStr[128] = {0,};

	if( IsVisible )
	{
		return;
	}
	mcp_item_slot->PcItem->Clear();	

	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[i] == NULL )
		{
			continue;
		}
		
		///-- item slot
		if( nRui->GetPcInvenInfo()->InvenItem[i]->ItemUID == ai_DelStoneID)
		{
			///-- Copy Item
			mcp_item_slot->SetSlotPcItem( *(nRui->GetPcInvenInfo()->InvenItem[i]) );
			mcp_item_slot->PcItem->Amount = 1;
		}	
	}
	
	///-- Text Exp	
	Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_WND_ITEM_GET_SUBEXP ), ai_exp );
	mcp_exp->SetText( tmpStr );
	
	if( ai_penalty )
	{			
		Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_WND_ITEM_LEVEL_PENALTY ), ai_penalty );
		mcp_exp_penalty->SetText( tmpStr );
	}
	else
	{
		mcp_exp_penalty->SetText( _RT("") );				
	}
		
	///-- Text Effect	
	Rsprintf( RWCHAR(tmpStr), _RT("%s"),_RT("") );
	mcp_exp->SetText( tmpStr );	
	mcp_effect[1]->SetText( tmpStr );
	mcp_effect[2]->SetText( tmpStr );

	///-- Image
	mcp_result_text->SetText( G_STRING_CHAR( IDS_FAIL )); 
	mcp_icon_x->SetVisible( true );	
	nRui->SendEvent( WID_SUB_ENCHANT_RESULT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

}


///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CItemEnchantRWndProc::Update()
{
	
	
	
}


///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CItemEnchantRWndProc::Proc( SEventMessage* EMsg )
{
		switch( EMsg->FocusWnd )
	{
	case WID_SUB_ENCHANT_RESULT_BTN_OK:
	case WID_SUB_ENCHANT_RESULT_CLOSE:

		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SUB_ENCHANT_RESULT, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
	
	}
	

}


///---------------------------------------------------------------------------
///-- EnchantText
///---------------------------------------------------------------------------
void CItemEnchantRWndProc::EnchantText(	SPcItem * ap_Item )
{
	int li_text_idx = 0;
	TCHAR tmpStr[128] = {0,};
	
	///-- Clear Text
	Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );
	mcp_effect[0]->SetText( tmpStr );				
	mcp_effect[1]->SetText( tmpStr );			
	mcp_effect[2]->SetText( tmpStr );	
	
	///-- 물리 공격력
	if( ap_Item->ItemTable->theMaxAttackBonus )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ) , ap_Item->ItemTable->theMaxAttackBonus );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 마법 공격력
	if( ap_Item->ItemTable->theMaxMagicBonus )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), ap_Item->ItemTable->theMaxMagicBonus );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	
	///-- 아이템 착용시 효과 
	///-- 파워
	if( ap_Item->ItemTable->thePower )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_POWER ), ap_Item->ItemTable->thePower );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	
	///-- 체력
	if( ap_Item->ItemTable->theVital )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_VITAL ), ap_Item->ItemTable->theVital );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 교감 
	if( ap_Item->ItemTable->theSympathy )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_SYMPATHY ), ap_Item->ItemTable->theSympathy );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 지능
	if( ap_Item->ItemTable->theInt )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_INTELLI ), ap_Item->ItemTable->theInt );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 지구력
	if( ap_Item->ItemTable->theStamina )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_STAMINA ), ap_Item->ItemTable->theStamina );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
	///-- 민첩
	if( ap_Item->ItemTable->theDex )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_DEX ), ap_Item->ItemTable->theDex );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > 2 ) 
		{
			return;
		}
	}
				
				
}