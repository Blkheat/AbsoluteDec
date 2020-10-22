#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
//#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
//#include "pc.h"
#include "ItemSkillPotionRWndProc.h"
#include "stringmanager.h"
#include "ItemSelectPotionWndProc.h"

CItemSkillPotionRWndProc	g_ItemSkillPotionRWndProc;


//-----------------------------------------------------------------------------
void CItemSkillPotionRWndProc::Init()
{
}

//-----------------------------------------------------------------------------
void CItemSkillPotionRWndProc::Composition()
{
	CFrameWnd*		fw= NULL;
	CButton*		btn= NULL;
	CTextBox*		tbox= NULL;
	CImageBox*		img= NULL;
		
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 204;
	enchantwnd.h	= 220;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	///-- Frame Wnd
	FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None , WID_None, enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );	
	FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	FrameWnd->SetWndTileSizeLR( 40, 29 );
	FrameWnd->SetWndTileSizeTB( 31, 11 );

	FrameWnd->SetCaption( G_STRING_CHAR(IDS_WND_RESULT_WINDOWS) );	
	FrameWnd->SetTitle( true );
	FrameWnd->SetWndProc( this );
	FrameWnd->SetVisible( false );
	FrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(), 1, 111, 202, 72, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(),  1, 110, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(), 1, 183, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(), 85, 52, 35, 35, false );  
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	///-- 원석 (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(),  52, 93, 100, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_ENCHANT_RESULT_TEXT ) );

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	Exit_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , FrameWnd->GetID(), 185, 6, 13, 13, false );    
	Exit_btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- 아이템 Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, FrameWnd->GetID(), 85, 52, 35, 35, false );
	mcp_item_slot->PcItem = &mc_itemslot_item;		
	mcp_item_slot->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot->SetHID( HID_ITEMTIP );

	///-- 확인 버튼
	Ok_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, FrameWnd->GetID(), 74, 189, 58, 25, false );
	Ok_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );		
	Ok_btn->SetFontR( n_fRsvFontWndButton );   
	Ok_btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );

	///-- Exp
	mcp_exp = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 5, 111, 190, 14, false );
	mcp_exp->SetFontR( n_fRsvFontWndStatic );	

	///-- Penalty 
	mcp_exp_penalty = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 15, 124, 190, 14, false );
	mcp_exp_penalty->SetFontR( n_fRsvFontWndStatic );	
	
	///-- Effect
	mcp_effect[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 5, 139, 190, 14, false );
	mcp_effect[0]->SetFontR( n_fRsvFontWndStatic );		

	///-- Effect
	mcp_effect[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 5, 154, 190, 14, false );
	mcp_effect[1]->SetFontR( n_fRsvFontWndStatic );		
	
	///-- Effect
	mcp_effect[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 5, 169, 190, 14, false );
	mcp_effect[2]->SetFontR( n_fRsvFontWndStatic );	

	///-- image	
	mcp_icon_x = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(), 89, 56, 27, 27, false );
	mcp_icon_x->SetClientImage( TID_C_WND_ELDA_SB_X );

	///-- Result text
	mcp_result_text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 50, 37, 104, 12, false );
	mcp_result_text->SetFontR( n_fRsvFontWndStatic );

}
///---------------------------------------------------------------------------
///-- OpenEnchantRWnd
///---------------------------------------------------------------------------
///-- 아이템 강화 성공시 Wnd
void CItemSkillPotionRWndProc::OpenEnchantR_SWnd(const int ItemUID , const int bundle /* = 0  */)
{
	int i = 0;
	TCHAR tmpStr[128] = _T("");

	if( IsVisible )
	{
		return;
	}
	SPcDataInven* PcInven =  nRui->GetPcInven();
	bool bFind = false; 
	///-- item slot
	for(i = 0 ; i < c_MaxInvenItemNum ; ++i)
	{
		if(PcInven->InvenItem[i] && (PcInven->InvenItem[i]->ItemUID == ItemUID) )
		{
			bFind = true; 
			///-- Copy Item
			mcp_item_slot->SetSlotPcItem( *(nRui->GetPcInvenInfo()->InvenItem[i]) );
			if(	bundle  == 0 )
			{
				mcp_item_slot->PcItem->Amount = 1;
			}
			else 
			{
				mcp_item_slot->PcItem->Amount = bundle;
			}
			break;
		}	

	}

//	mcp_item_slot->PcItem->Clear();	
	if( bFind ) 
	{
		Rsprintf( RWCHAR(tmpStr) , _RT("%s") , mcp_item_slot->PcItem->ItemTable->theName );
		mcp_effect[0]->SetText(tmpStr);	
		mcp_effect[1]->SetText(G_STRING_CHAR(IDS_WND_GET_ITEM));
		
		mcp_result_text->SetText( G_STRING_CHAR( IDS_SUCCESS ) );
		mcp_item_slot->SetVisible(true);
		mcp_icon_x->SetVisible( false );	
		nRui->SendEvent( FrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	}
}

void CItemSkillPotionRWndProc::OpenEnchantRCode_SWnd(const int Code , const int bundle /* = 0  */)
{
	int i = 0;
	TCHAR tmpStr[128] = _T("");
	
	if( IsVisible )
	{
		return;
	}
	SPcItem PcItem;

	

	SPcDataInven* PcInven =  nRui->GetPcInven();
	
	g_ItemSelectPotionWndProc.SetAddPcItem(&PcItem , Code , bundle );
	
	mcp_item_slot->SetSlotPcItem(PcItem);
	
	if(	bundle  == 0 )
	{
		mcp_item_slot->PcItem->Amount = 1;
	}
	else 
	{
		mcp_item_slot->PcItem->Amount = bundle;
	}
		
	//	mcp_item_slot->PcItem->Clear();	
	Rsprintf( RWCHAR(tmpStr) , _RT("%s") , mcp_item_slot->PcItem->ItemTable->theName );
	mcp_effect[0]->SetText(tmpStr);	
	mcp_effect[1]->SetText(G_STRING_CHAR(IDS_WND_GET_ITEM));
	
	mcp_result_text->SetText( G_STRING_CHAR( IDS_SUCCESS ) );
	mcp_item_slot->SetVisible(true);
	mcp_icon_x->SetVisible( false );	
	nRui->SendEvent( FrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
}


///---------------------------------------------------------------------------
///-- OpenEnchantR_FWnd
///---------------------------------------------------------------------------
///-- 아이템 강화 실패시 Wnd
void CItemSkillPotionRWndProc::OpenEnchantR_FWnd()
{
	int i = 0 ;
	TCHAR tmpStr[128] = _T("");

	if( IsVisible )
	{
		return;
	}
		
	//mcp_item_slot->PcItem->Clear();	
	
	mcp_effect[0]->SetText(G_STRING_CHAR(IDS_WND_NEXT_CHANCE));
	mcp_effect[1]->SetText("");

	///-- Image
	mcp_result_text->SetText( G_STRING_CHAR( IDS_FAIL )); 

	mcp_item_slot->SetVisible(false);
	mcp_icon_x->SetVisible( true );	
	nRui->SendEvent( FrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );

}


///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CItemSkillPotionRWndProc::Update()
{
	

}


///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CItemSkillPotionRWndProc::Proc( SEventMessage* EMsg )
{

	if( EMsg->FocusWnd == Exit_btn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( FrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		
		}
	}
	else if( EMsg->FocusWnd == Ok_btn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( FrameWnd->GetID() , n_emSetVisible , (EPARAM)false , 0 , 0 , 0 );
		
		}
	}
}


///---------------------------------------------------------------------------
///-- EnchantText
///---------------------------------------------------------------------------
void CItemSkillPotionRWndProc::EnchantText( bool flag )
{
		
	///-- 물리 공격력
	if( flag == true )
	{
	
		
	}
	else 
	{
		mcp_effect[0]->SetText(G_STRING_CHAR(IDS_FAIL));
		mcp_effect[1]->SetText(G_STRING_CHAR(IDS_WND_NEXT_CHANCE));
	}


				
}