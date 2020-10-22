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
//#include "pc.h"
#include "ItemExtractWndProc.h"
#include "stringmanager.h"

CItemExtract_WndProc	g_ItemExtract_WndProc;


//-----------------------------------------------------------------------------
void CItemExtract_WndProc::Init()
{
}

//-----------------------------------------------------------------------------
void CItemExtract_WndProc::Composition()
{
	CFrameWnd*		fw;
	CButton*		btn;
	CTextBox*		tbox;
	CImageBox*		img;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 204;
	enchantwnd.h	= 200;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	
	///-- Frame Wnd
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SUB_EXTRACT, WID_None, enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );
	fw->SetWndTexStyle( fwfTex_Style_Tile9 );
	fw->SetClientImage( TID_CTRL_WND_L1 );
	fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	fw->SetWndTileSizeLR( 40, 29 );
	fw->SetWndTileSizeTB( 31, 11 );

	fw->SetCaption( G_STRING_CHAR( IDS_WND_EXTRACT ) );		
	fw->SetTitle( true  );	
	fw->SetWndProc( this );
	fw->SetVisible( false );
	fw->Initialize();

	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_EXTRACT, 1, 111, 202, 55, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_EXTRACT, 1, 110, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_EXTRACT, 1, 166, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_EXTRACT, 33, 56, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_EXTRACT, 132, 56, 35, 35, false );            
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	///-- -> Image
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SUB_EXTRACT, 84, 64, 37, 20, false );                  
	img ->SetClientImage( TID_C_WND_ELDA_SB_ARROW );
	
	///-- ������( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_EXTRACT, 12, 36, 80, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_ITEM ) );

	///-- ���ٽ���( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_EXTRACT, 110, 36, 80, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR( IDS_ELDASTONE ));
	
	///-- ����� ���� ����( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SUB_EXTRACT, 45, 95, 110, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_EXTRACT_GUESS ) );
	
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_EXTRACT_BT_CLOSE, WID_SUB_EXTRACT, 185, 6, 13, 13, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	///-- ����
	mcp_extract_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_EXTRACT_BT_EXT, WID_SUB_EXTRACT, 31, 171, 58, 25, false );
	mcp_extract_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	mcp_extract_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_extract_btn->SetText( G_STRING_CHAR( IDS_WND_EXTRACT_BT ) ); 
	mcp_extract_btn->SetBtnHold( true );

	///-- �ߴ�
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_EXTRACT_BT_ABT, WID_SUB_EXTRACT, 119, 171, 58, 25, false );
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 		
	
	///-- �ݱ�
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_EXTRACT_BT_CLOSE, WID_SUB_EXTRACT, 182, 5, 13, 13, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///-- ������ Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_EXTRACT_SLOT_ITEM, WID_SUB_EXTRACT, 33, 56, 35, 35, false );	
	mcp_item_slot->SetSlotGroup( n_SGROUP_TUNING );

	///-- Elda Slot
	mcp_elda_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_EXTRACT_SLOT_TARGET, WID_SUB_EXTRACT, 132, 56, 35, 35, false );	
	mcp_elda_slot->SetSlotGroup( n_SGROUP_TUNING );

	///-- None Item ����
	mcp_item_slot->PcItem = &mc_itemslot_item;		
	mcp_elda_slot->PcItem = &mc_eldaslot_item;
	
	///-- ��� Text
	mcp_extract_Desc = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_EXTRACT_TEXT1, WID_SUB_EXTRACT, 5, 132, 190, 14, false );
	mcp_extract_Desc->SetFontR( n_fRsvFontWndStatic );	
	mcp_extract_Desc->SetBlank( 0, 0 );	
	
}
///---------------------------------------------------------------------------
///-- OpenExtract_Wnd
///---------------------------------------------------------------------------
void CItemExtract_WndProc::CloseExtract_Wnd()
{
	nRui->SendEvent( WID_SUB_EXTRACT, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}
///---------------------------------------------------------------------------
///-- OpenExtract_Wnd
///---------------------------------------------------------------------------
void CItemExtract_WndProc::OpenExtract_Wnd()
{
	static TCHAR tmpStr[128] = {0,};
	if( IsVisible )
	{
		return;
	}
	nRui->SendEvent( WID_SUB_EXTRACT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
	
	Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );
	mcp_extract_Desc->SetText( tmpStr );
	mcp_item_slot->PcItem->Clear();
	mcp_elda_slot->PcItem->Clear();
	mcp_extract_btn->SetBtnHold( true );
	mb_button_lock = false;
}


///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
///-- �ش� Item �� Grade �� ���� ������ ����
///-- !!!! ���ٽ��� Code �� HardCoding ���� 
void CItemExtract_WndProc::Update()
{
	static TCHAR tmpStr[128] = {0,};
	
	///-- ��� ���ٽ����� �ּ� Level
	int li_min_level;
	///-- ������� ������ EldaStone �� Code
	int li_temp_stoneCode = 6200;
	

	///-----------------------------------------------------------------------
	///-- ���� ��� ǥ�� 
	if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE )
	{
		///-- Level
		li_min_level = mcp_item_slot->PcItem->ItemTable->theGrade -2;
		///-- Code
		li_temp_stoneCode += mcp_item_slot->PcItem->ItemTable->theGrade;
			
		if( li_min_level < 1 )
		{
			li_min_level =1;
		}

		if( li_min_level == mcp_item_slot->PcItem->ItemTable->theGrade )
		{
			///-- ����� Text
			Rsprintf( RWCHAR(tmpStr), _RT("%s Lv%d"), G_STRING_CHAR( IDS_ELDASTONE_ROUGH ), li_min_level);
			mcp_extract_Desc->SetText( tmpStr );		
		}
		else
		{	
			///-- ����� Text
			Rsprintf( RWCHAR(tmpStr), _RT("%s Lv%d ~ Lv%d"),G_STRING_CHAR( IDS_ELDASTONE_ROUGH ), li_min_level, mcp_item_slot->PcItem->ItemTable->theGrade );
			mcp_extract_Desc->SetText( tmpStr );		
		}
		///-- ��� Elda Stone
		mcp_elda_slot->PcItem->Class	= g_Pc_Manager.GetItemClsss( li_temp_stoneCode );				
		mcp_elda_slot->PcItem->Class	= g_Pc_Manager.GetItemClsss( li_temp_stoneCode );
		mcp_elda_slot->PcItem->ItemUID	= 1;
		mcp_elda_slot->PcItem->IType = n_ITYPE_SPECIAL;	///-- Elda Stone
		mcp_elda_slot->PcItem->Amount = 1;
		mcp_elda_slot->PcItem->Code			= li_temp_stoneCode;
		mcp_elda_slot->PcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( li_temp_stoneCode );				
		mcp_elda_slot->PcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( li_temp_stoneCode );
		
	}
	///-----------------------------------------------------------------------
	///-- ���� ��� ����
	else
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );
		mcp_extract_Desc->SetText( tmpStr );
		mcp_elda_slot->PcItem->Clear();
	}
}

//-----------------------------------------------------------------------------
void CItemExtract_WndProc::Proc( SEventMessage* EMsg )
{
	
	switch( EMsg->FocusWnd )
	{
	case WID_SUB_EXTRACT_BT_EXT:		
		if( EMsg->Notify == NM_BTN_CLICK && !mb_button_lock )
		{			
			//g_RockClient.Send_CsExtractStone( mcp_item_slot->PcItem->ItemUID );
			mb_button_lock = true;
		}
		break;
		
	case WID_SUB_EXTRACT_BT_ABT:
	case WID_SUB_EXTRACT_BT_CLOSE:

		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SUB_EXTRACT, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
		
	case WID_SUB_EXTRACT_SLOT_ITEM:	
		{
			if( EMsg->Notify == NM_SLOT_CLICK )			{	ItemSlotClick( EMsg );		}
			else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	ItemSlotDBClick( EMsg );	}		
			
			///-- Slot �� Item �� �ִٸ� ���� �غ�
			if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE )//-- && mcp_elda_slot->PcItem->IType != n_ITYPE_NONE )
			{
				mcp_extract_btn->SetBtnHold( false );				
			}	
			else
			{
				mcp_extract_btn->SetBtnHold( true );
			}	
		}
		break;
		
	}
	

}
///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item ����
void CItemExtract_WndProc::ItemSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();
	
	
	if( tslot == mcp_item_slot && tslot->PcItem )
	{
		tslot->PcItem->Clear();
		///tslot->PcItem = NULL;//->Clear();
	}

	return;
}
///---------------------------------------------------------------------------
///-- ItemSlotClick
///---------------------------------------------------------------------------
///-- Item �ø���
void CItemExtract_WndProc::ItemSlotClick( SEventMessage* EMsg )
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
		if( targetslot->PcItem && (targetslot->PcItem == srcslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		///-------------------------------------------------------------------
		///-- Item Slot
		if( targetslot == mcp_item_slot && srcslot->PcItem->ItemTable )
		{
			if( srcslot->PcItem->ItemTable->theGrade == 0 )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EXTRACT ) );
			}
			else if( ( srcslot->Group == n_SGROUP_INVENTORY ) && ( srcslot->PcItem->IType == n_ITYPE_WEAR ) )
			{
				///-- Copy item
				targetslot->SetSlotPcItem( *(srcslot->PcItem) );								
				///-- Del Cursor
				//targetslot->PcItem = srcslot->PcItem;
				nRui->ResetCursorItem();
			}
			else if ( srcslot->Group == n_SGROUP_WEAR)
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EXTRACT_WEAR ) );
			}
		}
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
///-- CheckItemList
///---------------------------------------------------------------------------
BOOL CItemExtract_WndProc::CheckExtract_ItemList()
{
	int i = 0;
	int j = 0;
	
	bool lb_isFind_item = FALSE;
	bool lb_isFind_elda = FALSE;
	
	
	if( !mcp_item_slot->PcItem && !mcp_elda_slot->PcItem ) 
	{
		return FALSE;
	}

	///-----------------------------------------------------------------------
	///-- ��ü �κ��丮 Access
	for( j = 0; j <c_MaxInvenItemNum; ++j )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
		{
			continue;
		}
		
		///-- item slot
		if( mcp_item_slot->PcItem && mcp_item_slot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_item = true;			
		}	
	}

	///-----------------------------------------------------------------------
	///-- item �� ã�� �� ���� ���
	if( !lb_isFind_item )
	{
		mcp_item_slot->PcItem->Clear();
	}
	
	
	///-----------------------------------------------------------------------
	///-- �� Slot �� ��� Item �� �ִٸ� ���� �غ�
	if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE) //-- && mcp_elda_slot->PcItem->IType != n_ITYPE_NONE)
	{
		mcp_extract_btn->SetBtnHold( false );				
	}
	else
	{
		mcp_extract_btn->SetBtnHold( true );
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
