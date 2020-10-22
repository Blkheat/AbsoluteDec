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
#include "ItemCreate_FWndProc.h"
#include "stringmanager.h"


CItemCreate_F_WndProc	g_ItemCreate_F_WndProc;


//-----------------------------------------------------------------------------
void CItemCreate_F_WndProc::Init()
{
}

///---------------------------------------------------------------------------
///-- OpenRemodel_F_WndProc
///---------------------------------------------------------------------------
///-- 가공 결과창
///-- Slot 을 초기화 한다.
///-- 
void CItemCreate_F_WndProc::OpenCreate_F_WndProc( const SBundleItem * const aDelitems , const int ai_exp, int ai_penalty )
{
	int i,j;
	int li_slot_idx = 0;
	int li_text_idx = 0;
	
	TCHAR tmpStr[128] = {0,};
		
	
	if( IsVisible )
	{
		return;
	}
	nRui->SendEvent( WID_ITEMCREATEF_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	mcp_item_slot[0]->PcItem->Clear();
	mcp_item_slot[1]->PcItem->Clear();
	mcp_item_slot[2]->PcItem->Clear();
	mcp_item_slot[3]->PcItem->Clear();
	mcp_item_slot[4]->PcItem->Clear();
	mcp_item_slot[5]->PcItem->Clear();

	mcp_remodel_Desc[0]->SetText(_RT(""));
	mcp_remodel_Desc[1]->SetText(_RT(""));
	mcp_remodel_Desc[2]->SetText(_RT(""));
	mcp_remodel_Desc[3]->SetText(_RT(""));
	mcp_remodel_Desc[4]->SetText(_RT(""));
	mcp_remodel_Desc[5]->SetText(_RT(""));

	
	mcp_icon_x[0]->SetVisible( false );
	mcp_icon_x[1]->SetVisible( false );
	mcp_icon_x[2]->SetVisible( false );
	mcp_icon_x[3]->SetVisible( false );
	mcp_icon_x[4]->SetVisible( false );
	mcp_icon_x[5]->SetVisible( false );
	

	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[i] == NULL )
		{
			continue;
		}
		
		for( j = 0 ; j < 6 ; ++j )
		{
			///-- item slot
			if( nRui->GetPcInvenInfo()->InvenItem[i]->ItemUID == aDelitems[j].theItemID )
			{
				///-- Copy Item
				mcp_item_slot[li_slot_idx]->SetSlotPcItem( *(nRui->GetPcInvenInfo()->InvenItem[i]) );
				mcp_item_slot[li_slot_idx]->PcItem->Amount = nRui->GetPcInvenInfo()->InvenItem[i]->Amount - aDelitems[j].theBundle;
				mcp_item_slot[li_slot_idx]->PcItem->IType = nRui->GetPcInvenInfo()->InvenItem[i]->IType;
				mcp_item_slot[li_slot_idx]->PcItem->WearTable.theEndurance = nRui->GetPcInvenInfo()->InvenItem[i]->WearTable.theEndurance;
				
				///-- TEXT
				if( mcp_item_slot[li_slot_idx]->PcItem->IType != n_ITYPE_NONE )
				{	
					Rsprintf( RWCHAR(tmpStr), _RT( "%s" ), RWCHAR(mcp_item_slot[li_slot_idx]->PcItem->ItemTable->theName) );
					mcp_remodel_Desc[li_text_idx]->SetText( tmpStr );
					mcp_icon_x[li_text_idx]->SetVisible( true );
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
void CItemCreate_F_WndProc::Composition()
{
	CFrameWnd*		fw;
	CImageBox*		img;
	CButton*		btn;
	CTextBox*		tbox;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 284;
	enchantwnd.h	= 260;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	///-- Frame Wnd
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_ITEMCREATEF_WND, WID_None, 
		enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );

	fw->SetCaption( G_STRING_CHAR( IDS_WND_UPGRADE_FAIL ) );	
	fw->SetTitle( true );
	fw->SetWndProc( this );
	fw->SetVisible( false );
	fw->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 1, 111, 282, 112, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 1, 110, 282, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 1, 223, 282, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 22, 56, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 64, 56, 35, 35, false );      
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 106, 56, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 148, 56, 35, 35, false );        
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 190, 56, 35, 35, false );        
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 232, 56, 35, 35, false );        
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	///-- 소멸된 아이템 (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATEF_WND, 92, 93, 100, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR( IDS_WND_ITEM_DESTROYED_ITEM ) );
	

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X
	btn = (CButton*)nRui->CreateWnd( n_wtButton,  WID_ITEMCREATEF_BT_X, WID_ITEMCREATEF_WND, 265, 6, 13, 13, false ); 
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );	
	
	///-- 확인 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ITEMCREATEF_BT_OK, WID_ITEMCREATEF_WND, 
		114, 229, 58, 25, false );
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );		
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) ); 
	
	///-- TEXT
	mcp_remodel_Desc[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATEF_WND, 
		45, 115, 190, 14, false );
	mcp_remodel_Desc[0] ->SetFontR( n_fRsvFontWndStatic );

	mcp_remodel_Desc[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATEF_WND, 
		45, 133, 190, 14, false );
	mcp_remodel_Desc[1] ->SetFontR( n_fRsvFontWndStatic );	

	mcp_remodel_Desc[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATEF_WND, 
		45, 151, 190, 14, false );
	mcp_remodel_Desc[2] ->SetFontR( n_fRsvFontWndStatic );	

	mcp_remodel_Desc[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATEF_WND, 
		45, 169, 190, 14, false );
	mcp_remodel_Desc[3] ->SetFontR( n_fRsvFontWndStatic );

	mcp_remodel_Desc[4] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATEF_WND, 
		45, 187, 190, 14, false );
	mcp_remodel_Desc[4] ->SetFontR( n_fRsvFontWndStatic );
	
	mcp_remodel_Desc[5] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_ITEMCREATEF_WND, 
		45, 205, 190, 14, false );
	mcp_remodel_Desc[5] ->SetFontR( n_fRsvFontWndStatic );

	
	///-- 아이템 Slot
	mcp_item_slot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_ITEMCREATEF_SLOT1, WID_ITEMCREATEF_WND, 
		22, 56, 35, 35, false );
	mcp_item_slot[0]->SetSlotGroup( n_SGROUP_TUNING );
	
	///-- 아이템 Slot
	mcp_item_slot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_ITEMCREATEF_SLOT2, WID_ITEMCREATEF_WND, 
		64, 56, 35, 35, false );
	mcp_item_slot[1]->SetSlotGroup( n_SGROUP_TUNING );
	
	///-- 아이템 Slot
	mcp_item_slot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_ITEMCREATEF_SLOT3, WID_ITEMCREATEF_WND, 
		106, 56, 35, 35, false );
	mcp_item_slot[2]->SetSlotGroup( n_SGROUP_TUNING );

	///-- 아이템 Slot
	mcp_item_slot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_ITEMCREATEF_SLOT4, WID_ITEMCREATEF_WND, 
		148, 56, 35, 35, false );
	mcp_item_slot[3]->SetSlotGroup( n_SGROUP_TUNING );

	///-- 아이템 Slot
	mcp_item_slot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_ITEMCREATEF_WND, 
		190, 56, 35, 35, false );
	mcp_item_slot[4]->SetSlotGroup( n_SGROUP_TUNING );

	mcp_item_slot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_ITEMCREATEF_WND, 
		232, 56, 35, 35, false );
	mcp_item_slot[5]->SetSlotGroup( n_SGROUP_TUNING );

	
	///-- None Item 지정
	mcp_item_slot[0]->PcItem = &mc_itemslot_item[0];		
	mcp_item_slot[1]->PcItem = &mc_itemslot_item[1];		
	mcp_item_slot[2]->PcItem = &mc_itemslot_item[2];	
	mcp_item_slot[3]->PcItem = &mc_itemslot_item[3];
	mcp_item_slot[4]->PcItem = &mc_itemslot_item[4];
	mcp_item_slot[5]->PcItem = &mc_itemslot_item[5];

	///-- X Icon
	mcp_icon_x[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ITEMCREATEF_ICON_1, WID_ITEMCREATEF_WND, 
		25, 61, 27, 27, false );
	mcp_icon_x[0]->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x[0]->SetVisible( true );	

	///-- X Icon
	mcp_icon_x[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ITEMCREATEF_ICON_2, WID_ITEMCREATEF_WND, 
		68, 61, 27, 27, false );
	mcp_icon_x[1]->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x[1]->SetVisible( true );	

	///-- X Icon
	mcp_icon_x[2] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ITEMCREATEF_ICON_3, WID_ITEMCREATEF_WND, 
		110, 61, 27, 27, false );
	mcp_icon_x[2]->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x[2]->SetVisible( true );	

	///-- X Icon
	mcp_icon_x[3] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ITEMCREATEF_ICON_4, WID_ITEMCREATEF_WND, 
		152, 61, 27, 27, false );
	mcp_icon_x[3]->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x[3]->SetVisible( true );
	
	///-- X Icon
	mcp_icon_x[4] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 
		194, 61, 27, 27, false );
	mcp_icon_x[4]->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x[4]->SetVisible( true );
	
	///-- X Icon
	mcp_icon_x[5] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_ITEMCREATEF_WND, 
		236, 61, 27, 27, false );
	mcp_icon_x[5]->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x[5]->SetVisible( true );
	
	/*
	
	///-- TEXT
	mcp_remodel_Desc[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_REMODEL_FA_TEXT1, WID_ITEMCREATEF_WND, 5, 114, 190, 14, false );
	mcp_remodel_Desc[0]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	mcp_remodel_Desc[0]->SetClientColor( 255, 255, 255 );
	mcp_remodel_Desc[0]->SetClientImage( TID_None );	
	mcp_remodel_Desc[0]->SetAlignText( n_atCenter, n_atCenter );	
	mcp_remodel_Desc[0]->SetTextColor( 255, 255, 255 );
	mcp_remodel_Desc[0]->SetBlank( 0, 0 );	

	mcp_remodel_Desc[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_REMODEL_FA_TEXT2, WID_ITEMCREATEF_WND, 5, 132, 190, 14, false );
	mcp_remodel_Desc[1]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	mcp_remodel_Desc[1]->SetClientColor( 255, 255, 255 );
	mcp_remodel_Desc[1]->SetClientImage( TID_None );	
	mcp_remodel_Desc[1]->SetAlignText( n_atCenter, n_atCenter );	
	mcp_remodel_Desc[1]->SetTextColor( 255, 255, 255 );
	mcp_remodel_Desc[1]->SetBlank( 0, 0 );	

	mcp_remodel_Desc[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_REMODEL_FA_TEXT3, WID_ITEMCREATEF_WND, 5, 150, 190, 14, false );
	mcp_remodel_Desc[2]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	mcp_remodel_Desc[2]->SetClientColor( 255, 255, 255 );
	mcp_remodel_Desc[2]->SetClientImage( TID_None );	
	mcp_remodel_Desc[2]->SetAlignText( n_atCenter, n_atCenter );	
	mcp_remodel_Desc[2]->SetTextColor( 255, 255, 255 );
	mcp_remodel_Desc[2]->SetBlank( 0, 0 );	

	
	
*/
	
}

//-----------------------------------------------------------------------------
void CItemCreate_F_WndProc::Update()
{
}


//-----------------------------------------------------------------------------
void CItemCreate_F_WndProc::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_ITEMCREATEF_BT_OK:
	case WID_ITEMCREATEF_BT_X:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_ITEMCREATEF_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
	}
	

}
