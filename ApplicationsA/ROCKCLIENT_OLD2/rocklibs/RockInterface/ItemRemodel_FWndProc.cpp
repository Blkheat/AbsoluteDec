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
#include "ItemRemodel_FWndProc.h"
#include "stringmanager.h"


CItemResult_F_WndProc	g_ItemRemodel_F_WndProc;


//-----------------------------------------------------------------------------
void CItemResult_F_WndProc::Init()
{
}

///---------------------------------------------------------------------------
///-- OpenRemodel_F_WndProc
///---------------------------------------------------------------------------
///-- 가공 결과창
///-- Slot 을 초기화 한다.
///-- 
void CItemResult_F_WndProc::OpenRemodel_F_WndProc( const SBundleItem * const aDelitems )
{
	int i,j;
	int li_slot_idx = 0;
	int li_text_idx = 0;
	
	TCHAR tmpStr[128] ={0,};
		
	
	if( IsVisible )
	{
		return;
	}
	nRui->SendEvent( WID_SUB_REMODEL_FA, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	mcp_item_slot[0]->PcItem->Clear();
	mcp_item_slot[1]->PcItem->Clear();
	mcp_item_slot[2]->PcItem->Clear();

	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[i] == NULL )
		{
			continue;
		}
		
		for( j = 0 ; j < 3 ; ++j )
		{
			///-- item slot
			if( nRui->GetPcInvenInfo()->InvenItem[i]->ItemUID == aDelitems[j].theItemID )
			{
				///-- Copy Item
				mcp_item_slot[li_slot_idx]->SetSlotPcItem( *(nRui->GetPcInvenInfo()->InvenItem[i]) );
				mcp_item_slot[li_slot_idx]->PcItem->Amount = nRui->GetPcInvenInfo()->InvenItem[i]->Amount - aDelitems[j].theBundle;
				mcp_item_slot[li_slot_idx]->PcItem->IType = n_ITYPE_SPECIAL;
				
				///-- TEXT
				if( mcp_item_slot[li_slot_idx]->PcItem->IType != n_ITYPE_NONE )
				{	
					Rsprintf( RWCHAR(tmpStr), _RT( "%s" ), RWCHAR(mcp_item_slot[li_slot_idx]->PcItem->ItemTable->theName) );
					mcp_remodel_Desc[li_text_idx]->SetText( tmpStr );
					li_text_idx ++;
				}
				li_slot_idx ++;
			}	
		}
	}

	


}

///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CItemResult_F_WndProc::Composition()
{
	CFrameWnd*		fw;
	CTextBox*		tbox;
	CButton*		btn;
	CImageBox*		img;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 204;
	enchantwnd.h	= 200;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	///-- Frame Wnd
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SUB_REMODEL_FA, WID_None, enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );

	fw->SetCaption( G_STRING_CHAR( IDS_WND_REMODEL_FAIL ) );		
	fw->SetTitle( true );	
	fw->SetWndProc( this );
	fw->SetVisible( false );
	fw->Initialize();
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL_FA, 1, 111, 202, 55, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL_FA,  1, 110, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL_FA, 1, 166, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL_FA, 34, 56, 35, 35, false );         
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL_FA, 84, 56, 35, 35, false );         
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
		
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_REMODEL_FA, 134, 56, 35, 35, false );               
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
				
	///-- 결과( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_REMODEL_FA, 45, 95, 110, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_COM_RESULT ) );

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_REMODEL_FA_BT_CLOSE, WID_SUB_REMODEL_FA, 185, 6, 13, 13, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- 확인 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_REMODEL_FA_BT_OK, WID_SUB_REMODEL_FA, 74, 171, 58, 25, false );
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
	
	///-- 아이템 Slot
	mcp_item_slot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_REMODEL_FA_SLOT_1, WID_SUB_REMODEL_FA, 34, 56, 35, 35, false );
	mcp_item_slot[0]->SetSlotGroup( n_SGROUP_TUNING );
	
	///-- 아이템 Slot
	mcp_item_slot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_REMODEL_FA_SLOT_2, WID_SUB_REMODEL_FA, 84, 56, 35, 35, false );
	mcp_item_slot[1]->SetSlotGroup( n_SGROUP_TUNING );
	
	///-- 아이템 Slot
	mcp_item_slot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_REMODEL_FA_SLOT_3, WID_SUB_REMODEL_FA, 134, 56, 35, 35, false );
	mcp_item_slot[2]->SetSlotGroup( n_SGROUP_TUNING );
	
	///-- None Item 지정
	mcp_item_slot[0]->PcItem = &mc_itemslot_item[0];		
	mcp_item_slot[1]->PcItem = &mc_itemslot_item[1];		
	mcp_item_slot[2]->PcItem = &mc_itemslot_item[2];	
	
	///-- TEXT
	mcp_remodel_Desc[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_REMODEL_FA_TEXT1, WID_SUB_REMODEL_FA, 5, 114, 190, 14, false );
	mcp_remodel_Desc[0]->SetFontR( n_fRsvFontWndStatic );		
	mcp_remodel_Desc[0]->SetBlank( 0, 0 );	

	mcp_remodel_Desc[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_REMODEL_FA_TEXT2, WID_SUB_REMODEL_FA, 5, 132, 190, 14, false );
	mcp_remodel_Desc[1]->SetFontR( n_fRsvFontWndStatic );
	mcp_remodel_Desc[1]->SetBlank( 0, 0 );	

	mcp_remodel_Desc[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_REMODEL_FA_TEXT3, WID_SUB_REMODEL_FA, 5, 150, 190, 14, false );
	mcp_remodel_Desc[2]->SetFontR( n_fRsvFontWndStatic );	
	mcp_remodel_Desc[2]->SetBlank( 0, 0 );	

	
	///-- X Icon
	mcp_icon_x[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_SUB_REMODEL_FA_IMG_X1, WID_SUB_REMODEL_FA, 39, 61, 27, 27, false );
	mcp_icon_x[0]->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x[0]->SetVisible( true );	

	///-- X Icon
	mcp_icon_x[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_SUB_REMODEL_FA_IMG_X2, WID_SUB_REMODEL_FA, 89, 61, 27, 27, false );
	mcp_icon_x[1]->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x[1]->SetVisible( true );	

	///-- X Icon
	mcp_icon_x[2] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_SUB_REMODEL_FA_IMG_X3, WID_SUB_REMODEL_FA, 139, 61, 27, 27, false );
	mcp_icon_x[2]->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x[2]->SetVisible( true );	

	
}

//-----------------------------------------------------------------------------
void CItemResult_F_WndProc::Update()
{
}


//-----------------------------------------------------------------------------
void CItemResult_F_WndProc::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_SUB_REMODEL_FA_BT_CLOSE:
	case WID_SUB_REMODEL_FA_BT_OK:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SUB_REMODEL_FA, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
	}
	

}
