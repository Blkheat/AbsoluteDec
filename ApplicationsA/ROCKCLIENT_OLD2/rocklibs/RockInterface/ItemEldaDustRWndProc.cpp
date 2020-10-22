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
#include "ItemEldaDustRWndProc.h"
#include "stringmanager.h"

CItemEldaDustRWndProc	g_ItemEldaDust_RWndProc;


//-----------------------------------------------------------------------------
void CItemEldaDustRWndProc::Init()
{
}

//-----------------------------------------------------------------------------
void CItemEldaDustRWndProc::Composition()
{

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
	m_FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );	
	m_FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_FrameWnd->SetWndTileSizeLR( 40, 29 );
	m_FrameWnd->SetWndTileSizeTB( 31, 11 );

	m_FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_ENCHANT_RESULT ) );	
	m_FrameWnd->SetTitle( true );
	m_FrameWnd->SetWndProc( this );
	m_FrameWnd->SetVisible( false );
	m_FrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 1, 111, 202, 72, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(),  1, 110, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 1, 183, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 85, 52, 35, 35, false );  
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	///-- 원석 (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(),  52, 93, 100, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_ENCHANT_RESULT_TEXT ) );

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_FrameWnd->GetID(), 185, 6, 13, 13, false );    
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- 아이템 Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, m_FrameWnd->GetID(), 85, 52, 35, 35, false );
	mcp_item_slot->PcItem = &mc_itemslot_item;		
	mcp_item_slot->SetSlotGroup( n_SGROUP_NONE );
	mcp_item_slot->SetHID( HID_ITEMTIP );

	///-- 확인 버튼
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_FrameWnd->GetID(), 74, 189, 58, 25, false );
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );		
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );

	///-- Exp
	mcp_exp = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(), 5, 111, 190, 14, false );
	mcp_exp->SetFontR( n_fRsvFontWndStatic );	

	///-- Penalty 
	mcp_exp_penalty = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(), 15, 124, 190, 14, false );
	mcp_exp_penalty->SetFontR( n_fRsvFontWndStatic );	
	
	///-- Effect
	mcp_effect[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(), 5, 139, 190, 14, false );
	mcp_effect[0]->SetFontR( n_fRsvFontWndStatic );		

	///-- Effect
	mcp_effect[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(), 5, 154, 190, 14, false );
	mcp_effect[1]->SetFontR( n_fRsvFontWndStatic );		
	
	///-- Effect
	mcp_effect[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(), 5, 169, 190, 14, false );
	mcp_effect[2]->SetFontR( n_fRsvFontWndStatic );	

	///-- image	
	mcp_icon_x = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_FrameWnd->GetID(), 89, 56, 27, 27, false );
	mcp_icon_x->SetClientImage( TID_C_WND_ELDA_SB_X );

	///-- Result text
	mcp_result_text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_FrameWnd->GetID(), 50, 37, 104, 12, false );
	mcp_result_text->SetFontR( n_fRsvFontWndStatic );

}
///---------------------------------------------------------------------------
///-- OpenEnchantRWnd
///---------------------------------------------------------------------------
///-- 아이템 강화 성공시 Wnd
void CItemEldaDustRWndProc::OpenEnchantR_SWnd( DWORD ai_ItemUID , DWORD ai_eldaID , DWORD ai_exp, int ai_penalty )
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
	nRui->SendEvent( m_FrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}
///---------------------------------------------------------------------------
///-- OpenEnchantR_FWnd
///---------------------------------------------------------------------------
///-- 아이템 강화 실패시 Wnd
void CItemEldaDustRWndProc::OpenEnchantR_FWnd( DWORD ai_DelStoneID , DWORD ai_exp, int ai_penalty )
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
	nRui->SendEvent( m_FrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );

}


///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CItemEldaDustRWndProc::Update()
{
	
	
	
}


///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CItemEldaDustRWndProc::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_SUB_ENCHANT_RESULT_BTN_OK:
	case WID_SUB_ENCHANT_RESULT_CLOSE:

		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_None, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
	
	}
	

}


///---------------------------------------------------------------------------
///-- EnchantText
///---------------------------------------------------------------------------
void CItemEldaDustRWndProc::EnchantText(	SPcItem * ap_Item )
{
				
}