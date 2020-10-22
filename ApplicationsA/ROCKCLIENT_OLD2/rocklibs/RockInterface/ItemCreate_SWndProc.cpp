
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
#include "ItemCreateWndProc.h"
#include "ItemCreate_SWndProc.h"
#include "stringmanager.h"


CItemCreate_S_WndProc	g_ItemCreate_S_WndProc;


//-----------------------------------------------------------------------------
void CItemCreate_S_WndProc::Init()
{
}

///---------------------------------------------------------------------------
///-- OpenEnchantWnd
///---------------------------------------------------------------------------
///-- 개조 창을 연다
///-- Slot 을 초기화 한다.
///-- 
void CItemCreate_S_WndProc::OpenCreate_S_WndProc( bool bResult, const int ai_ItemUID , const DWORD ai_exp, int ai_penalty )
{
	
	int i;
	TCHAR tmpStr[128] = {0,};
	
	if( IsVisible )
	{
		return;
	}
	
	mcp_item_slot->PcItem->Clear();
	nRui->SendEvent( WID_ITEMCREATES_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	mcp_icon_x->SetVisible( false );
	mcp_fw->SetCaption( G_STRING_CHAR( IDS_WND_UPGRADE_SUC ) );
	
	if(false == bResult)
	{
		mcp_icon_x->SetVisible( true );
		mcp_fw->SetCaption( G_STRING_CHAR( IDS_WND_UPGRADE_FAIL ) );
		
		memcpy(mcp_item_slot->PcItem, g_ItemCreate_WndProc.GetTarGetSlot()->PcItem, sizeof(SPcItem));
		
		mcp_remodel_Name->SetText( (TCHAR *)(mcp_item_slot->PcItem->ItemTable->theName) );
		mcp_remodel_Desc->SetText( G_STRING_CHAR(IDS_WND_UPGRADE_FAIL) );
		mcp_exp_text->SetText( _RT( "" ) );
		mcp_exp_penalty->SetText( _RT("") );

		return;	
	}

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
			mcp_item_slot->PcItem->Amount = 1;
			mcp_item_slot->PcItem->WearTable.theEndurance = mcp_item_slot->PcItem->ItemTable->theMaxEndurance;						

			mcp_remodel_Name->SetText( (TCHAR *)(mcp_item_slot->PcItem->ItemTable->theName) );
			mcp_remodel_Desc->SetText( G_STRING_CHAR(IDS_WND_UPGRADE_SUC) );
			
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_WND_ITEM_GET_SUBEXP ), ai_exp );
			mcp_exp_text->SetText( tmpStr );

			if( ai_penalty )
			{			
				Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_WND_ITEM_LEVEL_PENALTY ), ai_penalty );
				mcp_exp_penalty->SetText( tmpStr );
			}
			else
			{
				mcp_exp_penalty->SetText( _RT("") );				
			}
			return;
		}	
	}

	Rsprintf( RWCHAR(tmpStr), _RT( "" ) ); 
	mcp_remodel_Desc->SetText( tmpStr );
	mcp_exp_text->SetText( tmpStr );
}

///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CItemCreate_S_WndProc::Composition()
{
	CFrameWnd*		fw;
	CTextBox*		tbox;
	CButton*		btn;
	CImageBox*		img;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 204;
	enchantwnd.h	= 220;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	///-- Frame Wnd
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ITEMCREATES_WND, WID_None, 
		enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );

	fw->SetCaption( G_STRING_CHAR( IDS_WND_UPGRADE_SUC ) );		
	fw->SetTitle( true );	
	fw->SetWndProc( this );
	fw->SetVisible( false );
	fw->Initialize();
	mcp_fw = fw;

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATES_WND,  1, 111, 202, 75, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATES_WND, 1, 110, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATES_WND, 1, 186, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATES_WND, 84, 56, 35, 35, false );         
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
		
	///-- 결과( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATES_WND, 45, 95, 110, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_COM_RESULT ));

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ITEMCREATES_BT_X, WID_ITEMCREATES_WND, 185, 6, 13, 13, false );	
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- 확인 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ITEMCREATES_BT_OK, WID_ITEMCREATES_WND, 74, 191, 58, 25, false );
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );

	///-- 아이템 Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_ITEMCREATES_SLOT, WID_ITEMCREATES_WND, 
		84, 56, 35, 35, false );	
	mcp_item_slot->PcItem = &mc_itemslot_item;	
	mcp_item_slot->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot->SetHID( HID_ITEMTIP );

	mcp_icon_x = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ITEMCREATEF_ICON_3, WID_ITEMCREATES_WND, 
		87, 61, 27, 27, false );
	mcp_icon_x->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x->SetVisible( false );

	mcp_remodel_Name = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATES_WND, 5, 120, 190, 14, false );
	mcp_remodel_Name->SetFontR( n_fRsvFontWndStatic );	
	
	///-- 획득
	mcp_remodel_Desc = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_ITEMCREATES_TEXT, WID_ITEMCREATES_WND, 5, 135, 190, 14, false );
	mcp_remodel_Desc->SetFontR( n_fRsvFontWndStatic );	

	///-- Exp	
	mcp_exp_text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATES_WND, 5, 150, 190, 14, false );
	mcp_exp_text->SetFontR( n_fRsvFontWndStatic );	

	///-- Penalty 
	mcp_exp_penalty = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATES_WND, 5, 165, 190, 14, false );
	mcp_exp_penalty->SetFontR( n_fRsvFontWndStatic );	
	
}

//-----------------------------------------------------------------------------
void CItemCreate_S_WndProc::Update()
{
}


//-----------------------------------------------------------------------------
void CItemCreate_S_WndProc::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_ITEMCREATES_BT_OK:
	case WID_ITEMCREATES_BT_X:

		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_ITEMCREATES_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
	}
}
