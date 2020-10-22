
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
#include "ItemRemodel_SWndProc.h"
#include "stringmanager.h"


CItemRemodel_S_WndProc	g_ItemRemodel_S_WndProc;


//-----------------------------------------------------------------------------
void CItemRemodel_S_WndProc::Init()
{
}

///---------------------------------------------------------------------------
///-- OpenEnchantWnd
///---------------------------------------------------------------------------
///-- 개조 창을 연다
///-- Slot 을 초기화 한다.
///-- 
void CItemRemodel_S_WndProc::OpenRemodel_S_WndProc( const int ai_ItemUID )
{
	
	int i;
	TCHAR tmpStr[128] = {0,};
	
	if( IsVisible )
	{
		return;
	}
	mcp_item_slot->PcItem->Clear();
	nRui->SendEvent( WID_SUB_REMODEL_SUC, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

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
						
			Rsprintf( RWCHAR(tmpStr) , _T( G_STRING_CHAR( IDS_WND_ITEM_MSG_GET ) ), 
				mcp_item_slot->PcItem->ItemTable->theName);	

			mcp_remodel_Desc->SetText( tmpStr );
			return;
		}	
	}

	Rsprintf( RWCHAR(tmpStr), _RT( "" ) ); 
	mcp_remodel_Desc->SetText( tmpStr );
}

///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CItemRemodel_S_WndProc::Composition()
{
	CFrameWnd*		fw;
	CTextBox*		tbox;
	CImageBox*		img;
	CButton*		btn;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 204;
	enchantwnd.h	= 200;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	///-- Frame Wnd
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SUB_REMODEL_SUC, WID_None, enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );
	
	fw->SetCaption( G_STRING_CHAR( IDS_WND_REMODEL_SUC ) );		
	fw->SetTitle( true );	
	fw->SetWndProc( this );
	fw->SetVisible( false );
	fw->Initialize();
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL_SUC, 1, 111, 202, 55, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL_SUC,  1, 110, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL_SUC, 1, 166, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL_SUC, 84, 56, 35, 35, false );         
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
		
	///-- 결과( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_REMODEL_SUC, 45, 95, 110, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_COM_RESULT ) );
	
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_REMODEL_SUC_BT_CLOSE, WID_SUB_REMODEL_SUC, 185, 6, 13, 13, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- 확인 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_REMODEL_SUC_BT_OK, WID_SUB_REMODEL_SUC, 74, 171, 58, 25, false );	
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );

	///-- 아이템 Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_REMODEL_SUC_SLOT, WID_SUB_REMODEL_SUC, 84, 56, 35, 35, false );	
	mcp_item_slot->PcItem = &mc_itemslot_item;	
	mcp_item_slot->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot->SetHID( HID_ITEMTIP );
	
	///-- mcp_remodel_Desc
	mcp_remodel_Desc = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_REMODEL_SUC_TEXT1, WID_SUB_REMODEL_SUC, 5, 132, 190, 14, false );
	mcp_remodel_Desc->SetFontR( n_fRsvFontWndStatic );	
	mcp_remodel_Desc->SetBlank( 0, 0 );	
}

//-----------------------------------------------------------------------------
void CItemRemodel_S_WndProc::Update()
{
}


//-----------------------------------------------------------------------------
void CItemRemodel_S_WndProc::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_SUB_REMODEL_SUC_BT_OK:
	case WID_SUB_REMODEL_SUC_BT_CLOSE:

		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SUB_REMODEL_SUC, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
	}
}
