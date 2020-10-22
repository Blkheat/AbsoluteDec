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
#include "NumberInputWnd.h"



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\bravolibs\\obj\\SkillSystem.h"

//#include "pc.h"F
#include "InventoryWndProc.h"
#include "ItemCreateWndProc.h"
#include "stringmanager.h"


CItemCreateWndProc	g_ItemCreate_WndProc;
SMaKeItemTable * g_CreatelTable;

///---------------------------------------------------------------------------
///-- Constructor
///---------------------------------------------------------------------------
CItemCreateWndProc::CItemCreateWndProc()
{
	g_CreatelTable = NULL;
	mb_loadCreatelTable = false; 	
	mi_table_count = 0;
	mi_make_type = 1;

	mb_craft_weapon = true;

	mcp_Item = NULL;

	m_nIndexiTable = -1;

	m_IsViewchargeWindow = false;
}
///---------------------------------------------------------------------------
///-- Destructror
///---------------------------------------------------------------------------
CItemCreateWndProc::~CItemCreateWndProc()
{
	SAFE_DELETE_ARRAY( g_CreatelTable );
}
//-----------------------------------------------------------------------------
void CItemCreateWndProc::Init()
{
}
///---------------------------------------------------------------------------
///-- Change_make_type
///---------------------------------------------------------------------------
void CItemCreateWndProc::Change_make_type(CSlot *pSlot /* = NULL */)
{
	switch( mi_make_type ) 
	{
	
		///---------------------------------------------------------------------------
		///-- �з��� /����  : �� ��� Ȱ
	case 1:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_SWORD ));
		mcp_item_type->SetClientImage( 316 );break;
	
	case 2: 	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_GSWORD ) );
		mcp_item_type->SetClientImage( 335 );break;

	case 3:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_BOW ) );
		mcp_item_type->SetClientImage( 353 );break;		
		
		///---------------------------------------------------------------------------
		///-- ����  /����   : ������ ������ ������
		
	case 4:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_MBOOK ) );
		mcp_item_type->SetClientImage( 381 );break;
	case 5:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_STAFF ) );
		mcp_item_type->SetClientImage( 395 );break;	
	case 6: 
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_CRYBALL ) );
		mcp_item_type->SetClientImage( 1619 );break;	
		
		///---------------------------------------------------------------------------
		///-- �з��� /�Ƹ�  : ����� ����� ������ ����
	case 21:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_LARMOR ) );
		mcp_item_type->SetClientImage( 198 );break;	
	case 31:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_MARMOR ) );
		mcp_item_type->SetClientImage( 1379 );break;
	case 41:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_HARMOR ) );
		mcp_item_type->SetClientImage( 1391 );break;
	
		///-- ���� ����
		/*
	case 100:	///-- ���� / 337
		mi_make_type = 21;
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_LARMOR ) );
		mcp_item_type->SetClientImage( 198 );break;
		*/
		
		///---------------------------------------------------------------------------
		///-- ����  /�Ƹ�	: ������ ���ź� ������ 
	case 51:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_MAGIC_ARMOR ) );
		mcp_item_type->SetClientImage( 154 );break;
	case 61:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_PRIEST_ARMOR ) );
		mcp_item_type->SetClientImage( 182 );break;	
	case 71:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_SPRIT_ARMOR ) );
		mcp_item_type->SetClientImage( 126 );break;
	default:
		
		if( pSlot )
		{
			mcp_create_type->SetText( _RT("") );
			mcp_item_type->SetClientImage( pSlot->PcItem->Tid );
			
		}
		else 
		{
			mcp_create_type->SetText( _RT("") );
			mcp_item_type->SetClientImage( TID_None );
		}

		break;			
	}


	CheckCreate();
}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CItemCreateWndProc::Composition()
{
	CButton*		btn;
	CTextBox*		tbox;
	CImageBox*		img;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;
	enchantwnd.w	= 286;
	enchantwnd.h	= 256;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );

	m_pWholeFrame = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_ITEMCREATE_WND, WID_None, 
		enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );
	m_pWholeFrame->SetClientImage( TID_None );	
	m_pWholeFrame->SetTitle( true );
	m_pWholeFrame->SetIconTID( TID_CTRL_ICON_INVEN );
	m_pWholeFrame->SetCaption( G_STRING_CHAR( IDS_WND_UPGRADE ) );
	m_pWholeFrame->SetTitle( true );
	m_pWholeFrame->SetWndProc( this );	
	m_pWholeFrame->SetVisible( false );
	m_pWholeFrame->Initialize();	
	
	///-- Frame Wnd
	mpC_framewnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None , WID_ITEMCREATE_WND, 
		0, 0, enchantwnd.w, enchantwnd.h, false );
	mpC_framewnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	mpC_framewnd->SetClientImage( TID_CTRL_WND_L1 );		
	mpC_framewnd->SetWndTileSizeLR( 40, 29 );
	mpC_framewnd->SetWndTileSizeTB( 31, 11 );
	mpC_framewnd->SetTitle( false );
	mpC_framewnd->SetMove( false );
	mpC_framewnd->SetWndProc( this );	
	mpC_framewnd->SetVisible( true );
	mpC_framewnd->Initialize();
	
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 1, 147, 284, 75, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 1, 146, 284, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 1, 222, 284, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 12, 92, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 78, 92, 35, 35, false );      
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 118, 92, 35, 35, false );     
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 158, 92, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 198, 92, 35, 35, false );     
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 238, 92, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );



	///-- ���� �������� (Static)
	mcp_Item = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 12, 130, 40, 17, false );
	mcp_Item->SetFontR( n_fRsvFontWndStatic );
	mcp_Item->SetAlignText( n_atLeft, n_atCenter );
	mcp_Item->SetText( G_STRING_CHAR( IDS_ITEM ) );

	///-- ÷���� (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 149, 130, 55, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_ITEM_ADDITIVE ) );

	///-- ���� ������ (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 105, 198, 90, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR( IDS_WND_ITEM_OUTPUT_ITEM ) );

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ITEMCREATE_BT_X, mpC_framewnd->GetID(), 267, 6, 13, 13, false ); 
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );	
	
	
	///-- ������ ��ȭ�� ���� �غ��� �������� �̸������� Slot
	mcp_preitem_slot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
												mpC_framewnd->GetID(), 78, 52, 35, 35, false );
	mcp_preitem_slot[0]->PcItem = &(mc_preitemslot_item[0]);
	mcp_preitem_slot[0]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_preitem_slot[0]->SetHID( HID_ITEMTIP );

	mcp_preitem_slot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
												mpC_framewnd->GetID(), 118, 52, 35, 35, false );
	mcp_preitem_slot[1]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_preitem_slot[1]->PcItem = &(mc_preitemslot_item[1]);
	mcp_preitem_slot[1]->SetHID( HID_ITEMTIP );

	mcp_preitem_slot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
												mpC_framewnd->GetID(), 158, 52, 35, 35, false );
	mcp_preitem_slot[2]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_preitem_slot[2]->PcItem = &(mc_preitemslot_item[2]);
	mcp_preitem_slot[2]->SetHID( HID_ITEMTIP );

	mcp_preitem_slot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
												mpC_framewnd->GetID(), 198, 52, 35, 35, false );
	mcp_preitem_slot[3]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_preitem_slot[3]->PcItem = &(mc_preitemslot_item[3]);
	mcp_preitem_slot[3]->SetHID( HID_ITEMTIP );

	mcp_preitem_slot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
												mpC_framewnd->GetID(), 238, 52, 35, 35, false );
	mcp_preitem_slot[4]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_preitem_slot[4]->PcItem = &(mc_preitemslot_item[4]);
	mcp_preitem_slot[4]->SetHID( HID_ITEMTIP );	
	
	
	///-- ������ Slot
	mcp_item_slot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_ITEMCREATE_SLOT_1, mpC_framewnd->GetID(), 78, 92, 35, 35, false );
	mcp_item_slot[0]->PcItem = &(mc_itemslot_item[0]);
	mcp_item_slot[0]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot[0]->SetHID( HID_ITEMTIP );

	mcp_item_slot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_ITEMCREATE_SLOT_2, mpC_framewnd->GetID(), 118, 92, 35, 35, false );
	mcp_item_slot[1]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot[1]->PcItem = &(mc_itemslot_item[1]);
	mcp_item_slot[1]->SetHID( HID_ITEMTIP );

	mcp_item_slot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_ITEMCREATE_SLOT_3, mpC_framewnd->GetID(), 158, 92, 35, 35, false );
	mcp_item_slot[2]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot[2]->PcItem = &(mc_itemslot_item[2]);
	mcp_item_slot[2]->SetHID( HID_ITEMTIP );

	mcp_item_slot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, mpC_framewnd->GetID(), 198, 92, 35, 35, false );
	mcp_item_slot[3]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot[3]->PcItem = &(mc_itemslot_item[3]);
	mcp_item_slot[3]->SetHID( HID_ITEMTIP );

	mcp_item_slot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, mpC_framewnd->GetID(), 238, 92, 35, 35, false );
	mcp_item_slot[4]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot[4]->PcItem = &(mc_itemslot_item[4]);
	mcp_item_slot[4]->SetHID( HID_ITEMTIP );
	
	mcp_elda_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_ITEMCREATE_SLOT_ELDA, mpC_framewnd->GetID(), 12, 92, 35, 35, false );
	mcp_elda_slot->PcItem = &mc_eldaslot_item;
	mcp_elda_slot->SetSlotGroup( n_SGROUP_TUNING );
	mcp_elda_slot->SetHID( HID_ITEMTIP );
	
	///-- ����  ��ư
	mcp_create_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ITEMCREATE_BT_CREATE, mpC_framewnd->GetID(), 
//#ifdef C_PAY_SYSTEM
		               9, 227, 58, 25, false );
//#else
//					   72, 227, 58, 25, false );
//#endif
	mcp_create_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	mcp_create_btn->SetBtnHold( true );	
	mcp_create_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_create_btn->SetText( G_STRING_CHAR( IDS_UPGRADE ) );
	
	///-- �ߴ� ��ư
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ITEMCREATE_BT_CANCEL, mpC_framewnd->GetID(), 
//#ifdef C_PAY_SYSTEM
									74, 227, 58, 25, false );
//#else
//									166, 227, 58, 25, false );
//#endif
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );			
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );

	///-- item Type image
	mcp_item_type = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_ITEMCREATE_IMG_TYPE, mpC_framewnd->GetID(), 16, 35, 35, 35, false );	
	mcp_item_type->SetClientImage( TID_None );
	mcp_item_type->SetVisible(false);

	///-- Ÿ�� ����
	mcp_change_type = (CButton*)nRui->CreateWnd( n_wtButton, WID_ITEMCREATE_BT_TYPE, mpC_framewnd->GetID(), 13, 32, 42, 41, false );
	mcp_change_type->SetBtnImage( TID_C_WND_ELDA_SB_SLOTBT, TID_SlotHighLight, TID_C_WND_ELDA_SB_SLOTBT_CLK, TID_None );
	mcp_change_type->SetVisible(false);

	///-- TEXT ���� Ÿ��
	mcp_create_type = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_ITEMCREATE_TEXT_TYPE, 
												  mpC_framewnd->GetID(), 70, 32, 115, 17, false );
	mcp_create_type->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdShadow );
	mcp_create_type->SetClientColor( 255, 255, 255 );
	mcp_create_type->SetClientImage( TID_None );	
	mcp_create_type->SetAlignText( n_atLeft, n_atCenter );	
	mcp_create_type->SetTextColor( 145, 155, 255 );
	mcp_create_type->SetBlank( 0, 0 );	
	mcp_create_type->SetVisible(false);

	
	///-- TEXT ���� ������
	mcp_target_slot= (CSlot*)nRui->CreateWnd( n_wtSlot, WID_ITEMCREATE_SLOT_TARGET, mpC_framewnd->GetID(), 
		131, 160, 35, 35, false );
	mcp_target_slot->PcItem = &mc_target_item;
	mcp_target_slot->SetSlotGroup( n_SGROUP_TUNING );
	mcp_target_slot->SetHID( HID_ITEMTIP );
	
	/*
	mcp_create_item = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_ITEMCREATE_TEXT_TARGET, WID_ITEMCREATE_WND, 70, 165, 110, 14, false );
	mcp_create_item->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	mcp_create_item->SetClientColor( 255, 255, 255 );
	mcp_create_item->SetClientImage( TID_None );	
	mcp_create_item->SetAlignText( n_atCenter, n_atCenter );	
	mcp_create_item->SetTextColor( 255, 255, 255 );
	mcp_create_item->SetBlank( 0, 0 );	
	*/

	///-- ���� ������ ��ư
	mcp_chargeWindow_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, mpC_framewnd->GetID(), 
			190, 227, 87, 25, false );
	mcp_chargeWindow_btn->SetBtnImage( TID_CTRL_BTN87, TID_CTRL_BTN87_OVR, TID_CTRL_BTN87_CLK, TID_None );			
	mcp_chargeWindow_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_chargeWindow_btn->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );										
	mcp_chargeWindow_btn->SetVisible(false);
	
#ifdef C_PAY_SYSTEM	
	mcp_chargeWindow_btn->SetVisible(true);
#endif
	
	///-- ���� ������ ��� ������
	mpCharge_framewnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_ITEMCREATE_WND, 
		0, enchantwnd.h + 2, enchantwnd.w, 80, false );
	mpCharge_framewnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	mpCharge_framewnd->SetClientImage( TID_CTRL_WND_NL1 );
	mpCharge_framewnd->SetWndTileSizeLR( 5, 5 );
	mpCharge_framewnd->SetWndTileSizeTB( 5, 5 );	
	mpCharge_framewnd->SetTitle( false );
	mpCharge_framewnd->SetWndProc( this );
	mpCharge_framewnd->SetVisible( false );
	mpCharge_framewnd->SetMove(false);
	mpCharge_framewnd->Initialize();
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpCharge_framewnd->GetID(), 
		88, 14, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpCharge_framewnd->GetID(), 
		128, 14, 35, 35, false );      
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpCharge_framewnd->GetID(), 
		168, 14, 35, 35, false );     
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	
	mcp_Chargeitem_slot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  mpCharge_framewnd->GetID(), 88, 14, 35, 35, false );
	mcp_Chargeitem_slot[0]->PcItem = &(mc_Chargeslot_item[0]);
	mcp_Chargeitem_slot[0]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[0]->SetHID( HID_ITEMTIP );

	mcp_Chargeitem_slot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  mpCharge_framewnd->GetID(), 128, 14, 35, 35, false );
	mcp_Chargeitem_slot[1]->PcItem = &(mc_Chargeslot_item[1]);
	mcp_Chargeitem_slot[1]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[1]->SetHID( HID_ITEMTIP );

	mcp_Chargeitem_slot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  mpCharge_framewnd->GetID(), 168, 14, 35, 35, false );
	mcp_Chargeitem_slot[2]->PcItem = &(mc_Chargeslot_item[2]);
	mcp_Chargeitem_slot[2]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[2]->SetHID( HID_ITEMTIP );


	///-- ���� ������ (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpCharge_framewnd->GetID(), 
			105, 52, 90, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );						

	
	CloseCharge_Wnd();	
	
	///-- ���� Table
	if( !mb_loadCreatelTable )
	{
		LoadCreateTable();
	}
}

///---------------------------------------------------------------------------
///-- OpenEnchantRWnd
///---------------------------------------------------------------------------
void CItemCreateWndProc::OpenCreate_Wnd( NCraftType ae_type , BYTE aB_citizen )
{
	int j = 0;
	int li_max  =0;
	static TCHAR tmpStr[128] = {0,};
	int li_skill_max;	///-- ����/�� ��ȭ���� ���� �ְ� ��ų����
	int li_skill_min;	

	memset(dPayItem,NULL,sizeof(dPayItem));

	nRui->SendEvent( WID_ITEMCREATE_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		
	ClearComponentItemList();	
	
	mcp_item_slot[0]->PcItem->Clear();
	mcp_item_slot[1]->PcItem->Clear();
	mcp_item_slot[2]->PcItem->Clear();
	mcp_item_slot[3]->PcItem->Clear();
	mcp_item_slot[4]->PcItem->Clear();

	mcp_elda_slot->PcItem->Clear();
	mcp_create_btn->SetBtnHold( true );			 //������ ������ư Ȧ��
	mb_button_lock = false;
	
	m_IsViewchargeWindow = false;
	CloseCharge_Wnd();	
	
	
	///-- ���� �Ұ�						
	Rsprintf( RWCHAR(tmpStr), _RT(""));
	mcp_create_type->SetText( tmpStr );
		
	///---------------------------------------------------------------------------
	///-- ���� / �Ƹ� ũ����Ʈ ����
	if( ae_type == n_MakeWeapon )
	{
		mb_craft_weapon = true;
		mcp_Item->SetText( G_STRING_CHAR(IDS_ARM_ITEM) );							
	}
	else
	{
		mb_craft_weapon = false;
		mcp_Item->SetText( G_STRING_CHAR(IDS_GUARD_ITEM) );							
	}

	if( mb_craft_weapon )
	{
		li_skill_min = c_SkillCode_WeaponUpgrade_Min;
		li_skill_max = c_SkillCode_WeaponUpgrade_Max;
	}
	else
	{
		li_skill_min = c_SkillCode_ArmorUpgrade_Min;
		li_skill_max = c_SkillCode_ArmorUpgrade_Max;
	}

	///---------------------------------------------------------------------------	
	///-- ��ü Skill �߿� ���� �ְ� ���� ã��
	for( j = 0; j < RP_MAX_USER_SKILL; ++j )
	{	
		if( nRui->GetPcInvenInfo()->SkillItem[j] == NULL )
		{
			continue;
		}
		
		///-- Max Level ã�� - ���⼭�� ��ų�� �ϳ��� �����Ѵٴ� ����....
		if( nRui->GetPcInvenInfo()->SkillItem[j]->Code >= li_skill_min &&  
			nRui->GetPcInvenInfo()->SkillItem[j]->Code <= li_skill_max )
		{
			if( li_max < nRui->GetPcInvenInfo()->SkillItem[j]->Code - li_skill_min + 1 )
			{
				li_max = nRui->GetPcInvenInfo()->SkillItem[j]->Code - li_skill_min + 1;
			}
		}
	}	
	
	mi_skill_level = li_max;

	///---------------------------------------------------------------------------	
	///-- ������ ���� ���� Type ����
	if( aB_citizen == MILLENA )
	{
		if( mb_craft_weapon )
		{
			mi_make_type = 1;
		}
		else
		{
			mi_make_type = 21;
		}
	}
	else
	{
		if( mb_craft_weapon )
		{
			mi_make_type = 4;
		}
		else
		{
			mi_make_type = 51;
		}
	}
	
	Change_make_type();	
}

void CItemCreateWndProc::CloseCreate_Wnd()
{
	nRui->SendEvent( WID_ITEMCREATE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

//-----------------------------------------------------------------------------
void CItemCreateWndProc::Update()
{
	
}

void CItemCreateWndProc::OpenCharge_Wnd()
{
	SRect WholeFrameRect;
	SRect C_framewndRect;
	SRect Charge_frameRect;

	mpC_framewnd->GetClientSize(&C_framewndRect);
	mpCharge_framewnd->GetClientSize(&Charge_frameRect);

	WholeFrameRect = C_framewndRect;
	WholeFrameRect.h += (2 + Charge_frameRect.h);  

	m_pWholeFrame->ResetSize(&WholeFrameRect);
	mpC_framewnd->SetWndRepositionR(0, 0);
	mpCharge_framewnd->SetWndRepositionR(0, C_framewndRect.h + 2);

	nRui->SendEvent( mpCharge_framewnd->GetID(), n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );

}

void CItemCreateWndProc::CloseCharge_Wnd()
{
	SRect WholeFrameRect;
	SRect C_framewndRect;
	
	mpC_framewnd->GetClientSize(&C_framewndRect);
	WholeFrameRect = C_framewndRect;

	m_pWholeFrame->ResetSize(&WholeFrameRect);
	mpC_framewnd->SetWndRepositionR(0, 0);
	
	nRui->SendEvent( mpCharge_framewnd->GetID(), n_emSetVisible, 
			(EPARAM)false, 0, 0, 0 );

	mcp_Chargeitem_slot[0]->PcItem->Clear();
	mcp_Chargeitem_slot[1]->PcItem->Clear();
	mcp_Chargeitem_slot[2]->PcItem->Clear();
}

//-----------------------------------------------------------------------------
void CItemCreateWndProc::Proc( SEventMessage* EMsg )
{	
	static SBundleItem lc_item[6];


	int i;
	
	switch( EMsg->FocusWnd )
	{
		///-- Type ����
	case WID_ITEMCREATE_BT_TYPE:

		///-- Image �� ��ư ȿ��
		if( mcp_change_type->GetBtnState() == n_bsSelect )
		{
			mcp_item_type->SetWndRepositionR(17,36);
		}
		else
		{
			mcp_item_type->SetWndRepositionR(16,35);		
		}
		
		///-- Type ����
		if( EMsg->Notify == NM_BTN_CLICK )
		{	
			//Change_make_type();			
		}
		
		break;
		
		///-- Close
	case WID_ITEMCREATE_BT_X:
	case WID_ITEMCREATE_BT_CANCEL:
				
		if( EMsg->Notify == NM_BTN_CLICK )    
		{
			nRui->SendEvent( WID_ITEMCREATE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;		
		
		///-- ����
	case WID_ITEMCREATE_BT_CREATE:
	
		if( EMsg->Notify == NM_BTN_CLICK && !mb_button_lock)
		{	
			if( mc_eldaslot_item.IType != n_ITYPE_NONE )
			{	
				lc_item[0].theItemID = mc_eldaslot_item.ItemUID;
				lc_item[0].theBundle = mc_eldaslot_item.Amount;
			}
			else
			{
				lc_item[0].theItemID = 0;
				lc_item[0].theBundle = 0;
			}

			if( mc_eldaslot_item.IType != n_ITYPE_NONE )
			{	
				///-- Data Copy
				for( i = 0 ; i < 5; ++i )
				{
					if( mc_itemslot_item[i].IType == n_ITYPE_NONE )
					{
						lc_item[i+1].theItemID = 0;
						lc_item[i+1].theBundle = 0;
					}
					else
					{						
						lc_item[i+1].theItemID = mc_itemslot_item[i].ItemUID;
						lc_item[i+1].theBundle = mc_itemslot_item[i].Amount;
					}
				}
			}

//#ifdef C_PAY_SYSTEM

			if( mc_eldaslot_item.IType != n_ITYPE_NONE )
			{	
				///-- ���� ������ ī��
				for( i = 0 ; i < 3; ++i )
				{
					if( mc_Chargeslot_item[i].IType == n_ITYPE_NONE )
					{
						dPayItem[i] = 0;						
					}
					else
					{						
						dPayItem[i] = mc_Chargeslot_item[i].ItemUID;						
					}
				}			
			}
//#endif

			g_RockClient.Send_CsMakeItem( lc_item, mi_item_type, dPayItem );			
			
			nRui->SendEvent( WID_ITEMCREATE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			mb_button_lock = true;
		}
		break;
		
	case WID_ITEMCREATE_SLOT_1:
	case WID_ITEMCREATE_SLOT_2:
	case WID_ITEMCREATE_SLOT_3:
	case WID_ITEMCREATE_SLOT_ELDA:
		{
			if( EMsg->Notify == NM_SLOT_CLICK )			
			{	
				ItemSlotClick( EMsg );
				///-- ���� �������� Check
				CheckCreate();

				if(EMsg->FocusWnd == WID_ITEMCREATE_SLOT_ELDA)
				{
					ClearComponentItemList();
					ViewComponentItemList();
				}
			}
			else if( EMsg->Notify == NM_SLOT_DBCLICK )	
			{	
				ItemSlotDBClick( EMsg );
				CheckCreate();

				if(EMsg->FocusWnd == WID_ITEMCREATE_SLOT_ELDA)
				{
					ClearComponentItemList();
				}
			}			
			
		}
		break;
		
	}

	if( EMsg->FocusWnd == mcp_item_slot[3]->GetID() || 
		EMsg->FocusWnd == mcp_item_slot[4]->GetID() ||
		
		EMsg->FocusWnd == mcp_Chargeitem_slot[0]->GetID() ||
		EMsg->FocusWnd == mcp_Chargeitem_slot[1]->GetID() ||
		EMsg->FocusWnd == mcp_Chargeitem_slot[2]->GetID() )
	{	
		if( EMsg->Notify == NM_SLOT_CLICK )
		{
			ItemSlotClick( EMsg );
			CheckCreate();
		}
		else if( EMsg->Notify == NM_SLOT_DBCLICK )
		{
			ItemSlotDBClick( EMsg );
			CheckCreate();
		}
	}

	if( EMsg->FocusWnd == mcp_chargeWindow_btn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_IsViewchargeWindow = !m_IsViewchargeWindow;
			if(m_IsViewchargeWindow)
			{
				OpenCharge_Wnd();
			}
			else
			{
				CloseCharge_Wnd();
			}
		}
	}	
}
///---------------------------------------------------------------------------
///-- CloseNumberInputWnd
///---------------------------------------------------------------------------
void CItemCreateWndProc::CloseNumberInputWnd( int aNumItem )
{
	if( !mcp_temp_num_src_slot )
	{
		return;
	}

	if( mcp_temp_num_src_slot->PcItem )
	{
		if( mcp_temp_num_src_slot->PcItem->Amount >= aNumItem )
		{
			mcp_temp_num_tga_slot->SetSlotPcItem( *(mcp_temp_num_src_slot->PcItem) );				
			mcp_temp_num_tga_slot->PcItem->Amount = aNumItem;
		}
		else
		{			
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_VOLUME ) );
		}
	}

	///-- ���� �������� Check
	CheckCreate();
	return;
}

void CItemCreateWndProc::ViewComponentItemList()
{
	if ( n_ITYPE_NONE == mcp_elda_slot->PcItem->IType  )
	{
		m_nIndexiTable = -1;
		return;
	}

	int i, j, k;
	i = j = k = 0;

	if( mcp_elda_slot->PcItem->ItemTable == NULL)
	{
		return;
	}

	for( i = 0 ; i < mi_table_count ; ++i )  //���� ���̺� ������ŭ �ݺ�
	{
		///-- �� ���� �����۰� ������ �������̺��� ���Ѵ�.
		int nSindex = 0;
		int nEindex = 5;

#ifdef ITEM_UPGRADE_2
		if( !IsMakeItemSubJob(g_CreatelTable[i].theType) )
			continue;
		
		if( mcp_elda_slot->PcItem->ItemTable &&
			mcp_elda_slot->PcItem->ItemTable->theUpgradeForm == g_CreatelTable[i].theIndex )
#else
		nSindex = 1;
		nEindex = 6;

		if( mcp_elda_slot->PcItem->Code == g_CreatelTable[i].theAddItem[0].theItemCode )
			 
#endif	
		{
			for( j = nSindex, k = 0 ; j < nEindex ; ++j )
			{
				if( 0 == g_CreatelTable[i].theAddItem[j].theItemCode )
				{
					continue;
				}
				
				SItemBaseInfo* item_Info = NULL;
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( g_CreatelTable[i].theAddItem[j].theItemCode );
				if( !item_Info )
				{
					continue;
				}
				
				mcp_preitem_slot[k]->theSlotState = n_Slot_Disable;
				mcp_preitem_slot[k]->PcItem->Class = g_Pc_Manager.GetItemClsss( g_CreatelTable[i].theAddItem[j].theItemCode  );
				mcp_preitem_slot[k]->PcItem->ItemUID = 1;				
				mcp_preitem_slot[k]->PcItem->IType	= n_ITYPE_UNUSE;
				if( g_Pc_Manager.GetItemClsss( g_CreatelTable[i].theAddItem[j].theItemCode ) == n_Class_Use )
				{
					mcp_preitem_slot[k]->PcItem->IType	= n_ITYPE_USE;//�Ҹ�ǰ
				}
				else if( g_Pc_Manager.GetItemClsss( g_CreatelTable[i].theAddItem[j].theItemCode ) == n_Class_Useless )
				{
					mcp_preitem_slot[k]->PcItem->IType	= n_ITYPE_UNUSE;//��Ҹ�ǰ
				}
				else if( g_Pc_Manager.GetItemClsss( g_CreatelTable[i].theAddItem[j].theItemCode ) == n_Class_Special || 
						 g_Pc_Manager.GetItemClsss( g_CreatelTable[i].theAddItem[j].theItemCode ) == n_Class_Eldastone ||
						 g_Pc_Manager.GetItemClsss( g_CreatelTable[i].theAddItem[j].theItemCode ) == n_Class_Weapon_StrengthStone ||
						 g_Pc_Manager.GetItemClsss( g_CreatelTable[i].theAddItem[j].theItemCode ) == n_Class_Armor_StrengthStone)
				{
					mcp_preitem_slot[k]->PcItem->IType	= n_ITYPE_SPECIAL;
				}				
				mcp_preitem_slot[k]->PcItem->Amount = g_CreatelTable[i].theAddItem[j].theBundle;
				///mcp_preitem_slot[k]->PcItem->Code	= g_CreatelTable[i].theResultItemCode ;			
				mcp_preitem_slot[k]->PcItem->Tid	= (TEXID)g_Pc_Manager.GetItemTID(  g_CreatelTable[i].theAddItem[j].theItemCode  );
				mcp_preitem_slot[k]->PcItem->ItemTable	= item_Info;			
				k++;
			}			
			
			m_nIndexiTable = i;
			return;
		}
	}

	m_nIndexiTable = -1;
}

void CItemCreateWndProc::ClearComponentItemList()
{
	int  i = 0;
	
	for( i = 0 ; i < 5 ; ++i )
	{
		mcp_preitem_slot[i]->PcItem->Clear();
	}
	
	m_nIndexiTable = -1;
}

///---------------------------------------------------------------------------
///-- CheckExtract_ItemList
///---------------------------------------------------------------------------
///-- ���� �������� Check �ϰ� Button �� Ȱ��ȭ �Ѵ�.
void CItemCreateWndProc::CheckCreate()
{	
	int i, j, k;

	bool lb_match[5] = { false};

	bool lb_ck_table[5] = { false,};

	int li_suc_percnet = 0;	

	mi_item_type  = 0;
	
	if ( !mb_loadCreatelTable )
	{
		return;
	}
	
	///-- ��ü Table Access
	for( i = 0 ; i < mi_table_count ; ++i )  //���� ���̺� ������ŭ �ݺ�
	{
			///-- �� ���� �����۰� ������ �������̺��� ���Ѵ�.
			if ( mcp_elda_slot->PcItem->ItemTable && 
				 mcp_elda_slot->PcItem->ItemTable->theUpgradeForm == g_CreatelTable[i].theIndex )
			{
				///-- flag �ʱ�ȭ
				lb_match[0] = lb_match[1] = lb_match[2] = lb_match[3] = lb_match[4] = false;
				lb_ck_table[0] = lb_ck_table[1] = lb_ck_table[2] = lb_ck_table[3] = lb_ck_table[4] = false; 
				
				///-- ���� ������ ���԰� �������̺��� �񱳷�ƾ..
				///-- ������ ���� Access
				for( j = 0 ; j < 5 ; ++j )
				{
					///-- ���� Table 
					for( k = 0 ; k < 5 ; ++k )
					{
						///-- �� Slot
						if( mcp_item_slot[j]->PcItem->IType == n_ITYPE_NONE)
						{
							if( g_CreatelTable[i].theAddItem[k].theItemCode == 0 && !lb_ck_table[k])
							{
								lb_match[j] = true;
								lb_ck_table[k] = true;
								k = 6;
							}
						}
						///-- �������� �ִ� Slot
						else
						{
							if( mcp_item_slot[j]->PcItem->Code == g_CreatelTable[i].theAddItem[k].theItemCode &&
								mcp_item_slot[j]->PcItem->Amount == g_CreatelTable[i].theAddItem[k].theBundle &&
								!lb_ck_table[k])
							{
								lb_match[j] = true;
								lb_ck_table[k] = true;
								k = 6;
							}
						}
					}///-- end of for Table Slot				
				}///-- end of for Item Slot
				
				///---------------------------------------------------------------
				///-- ���հ��� , ��ư �� Text Set
				if( lb_match[0] && lb_match[1] && lb_match[2] && lb_match[3] && lb_match[4]
					/*!( mcp_item_slot[0]->PcItem->IType == n_ITYPE_NONE &&
					mcp_item_slot[1]->PcItem->IType == n_ITYPE_NONE &&
					mcp_item_slot[2]->PcItem->IType == n_ITYPE_NONE )*/ ///<--�� ��ƾ�� �� ���ִ°���.. 
					)
				{
					///-- Button
					mcp_create_btn->SetBtnHold( false );
					///-- real type;
					mi_item_type = g_CreatelTable[i].theType;					
					
					///-- ��� ������
					SItemBaseInfo* item_Info = NULL;
					
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( mcp_elda_slot->PcItem->ItemTable->theUpgradeItem );
					
					if( !item_Info )
					{
						MessageBox( NULL, "Critical Error: Missiong Item From Makeitem Table", NULL, NULL);
					}
					
					///-- ��� item 
					mcp_target_slot->PcItem->Class	= g_Pc_Manager.GetItemClsss( mcp_elda_slot->PcItem->ItemTable->theUpgradeItem  );				
					mcp_target_slot->PcItem->ItemUID = 1;
					mcp_target_slot->PcItem->IType = n_ITYPE_WEAR;	///-- Wear
					mcp_target_slot->PcItem->Amount = 1;
					mcp_target_slot->PcItem->Code	= mcp_elda_slot->PcItem->ItemTable->theUpgradeItem ;
					///-- ������ ���� ������ ����� ���̵�
					mcp_target_slot->PcItem->Tid	= (TEXID)g_Pc_Manager.GetItemTID( mcp_elda_slot->PcItem->ItemTable->theUpgradeItem  );
					mcp_target_slot->PcItem->ItemTable	= item_Info;
					mcp_target_slot->PcItem->WearTable.theEndurance = mcp_target_slot->PcItem->ItemTable->theMaxEndurance;					
					memcpy( &(mcp_target_slot->PcItem->WearTable), &(mcp_elda_slot->PcItem->WearTable), sizeof( SRpdWearItemBase ) );
										
					///-- ���� ���������� ������ �簻���Ѵ�.
					nRui->Refresh_ItemHelpTip();
					return;
				}
			}///-- End of if ���� ����
		

	}///-- ��üTable Access ��
	
	
	///-- ���� �Ұ�						
	mcp_target_slot->PcItem->Clear();	
	mcp_create_btn->SetBtnHold( true );	///-- Button
}
///---------------------------------------------------------------------------
///-- CheckExtract_ItemList
///---------------------------------------------------------------------------
BOOL CItemCreateWndProc::CheckCreate_ItemList()
{
	int i = 0;
	int j = 0;
	
	bool lb_isFind_item[5] = { FALSE, FALSE, FALSE, FALSE, FALSE };
	bool lb_isFind_elda = FALSE;
	bool lb_isFind_Chargeitem[3] = { FALSE, FALSE, FALSE };	
	
	if( !mcp_item_slot[0]->PcItem && !mcp_item_slot[1]->PcItem && 
		!mcp_item_slot[2]->PcItem && !mcp_item_slot[3]->PcItem &&
		!mcp_item_slot[4]->PcItem && !mcp_elda_slot->PcItem &&
		!mcp_Chargeitem_slot[0]->PcItem && !mcp_Chargeitem_slot[1]->PcItem &&
		!mcp_Chargeitem_slot[2]->PcItem ) 
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
		
		///-- Item Check
		for( i = 0; i < 5; i++)
		{
			///-- item slot
			if( mcp_item_slot[i]->PcItem && 
				mcp_item_slot[i]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
			{
				lb_isFind_item[i] = true;

				///-- ���� üũ
				if( mcp_item_slot[i]->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
				{
					mcp_item_slot[i]->PcItem->Clear();
				}
			}
		}
		
		///-- ���� ������ slot
		if( mcp_elda_slot->PcItem && 
			mcp_elda_slot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_elda = true;

			///-- ���� üũ
			if( mcp_elda_slot->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
			{
				mcp_elda_slot->PcItem->Clear();				
			}
		}

		///-- ���� ������ slot
		for( i = 0; i < 3; i++)
		{
			///-- item slot
			if( mcp_Chargeitem_slot[i]->PcItem && 
				mcp_Chargeitem_slot[i]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
			{
				lb_isFind_Chargeitem[i] = true;

				///-- ���� üũ
				if( mcp_Chargeitem_slot[i]->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
				{
					mcp_Chargeitem_slot[i]->PcItem->Clear();
				}
			}
		}
	}
	
	///-----------------------------------------------------------------------
	///-- item �� ã�� �� ���� ���
	///-- Item Check
	for( i = 0; i < 5; i++)
	{
		///-- item slot
		if( !lb_isFind_item[i] )
		{
			mcp_item_slot[i]->PcItem->Clear();
		}
	}

	///-- ���� �������� ã�� �� ���� ���
	if( !lb_isFind_elda )
	{
		mcp_elda_slot->PcItem->Clear();
		ClearComponentItemList();
	}

	///------------------------------------------------------------------------
	///-- ���� item �� ã�� �� ���� ���
	///-- Item Check
	for( i = 0; i < 3; i++)
	{
		///-- item slot
		if( !lb_isFind_Chargeitem[i] )
		{
			mcp_Chargeitem_slot[i]->PcItem->Clear();
		}
	}

	CheckCreate();

	return FALSE;
}


///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item ����
void CItemCreateWndProc::ItemSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();	
	
	if( tslot->PcItem )
	{
		tslot->PcItem->Clear();
		
		if(mb_craft_weapon)
		{
			mcp_Item->SetText( G_STRING_CHAR(IDS_ARM_ITEM) );
		}
		else
		{
			mcp_Item->SetText( G_STRING_CHAR(IDS_GUARD_ITEM) );							
		}
	}

	return;
}
///---------------------------------------------------------------------------
///-- ItemSlotClick
///---------------------------------------------------------------------------
///-- Item �ø���
void CItemCreateWndProc::ItemSlotClick( SEventMessage* EMsg )
{
	CSlot*		targetslot	= NULL;//���� ��� ����
	CSlot*		srcslot		= NULL;//���õ�(pick) �κ� ����
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;

	RLG1("EMsg->FocusWnd %d",EMsg->FocusWnd);
	///-- �������� ��� ���� ���
	if( nRui->GetCursorItem() != NULL )
	{
		//��Ŀ�� ������ ID �� �Ѱ� �����̳ʸ� ���� �ش� ��ġ �ڽ��� ã�� �����Ѵ� ..  �����Ű�� ���� .. ����
		targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		//�ҽ����� �� ���콺 �� �پ� �ִ� ������ �����´� .. 
		srcslot = nRui->GetItemSelectedSlot();

		///- ���õ� �������� �������
		if( srcslot->PcItem == NULL )	
		{
			return;
		}	
		
		///- �̷� ��Ȳ�� ������ ������....Ÿ�ٽ����� ��ü PcItem������ �������־...
		if( targetslot->PcItem && (targetslot->PcItem == srcslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}

		///-------------------------------------------------------------------
		///-- Item Slot
		if( targetslot == mcp_item_slot[0] || targetslot == mcp_item_slot[1] || 
			targetslot == mcp_item_slot[2] || targetslot == mcp_item_slot[3] || targetslot == mcp_item_slot[4])
		{
			if( srcslot->Group == n_SGROUP_INVENTORY )
			{
				///-- ���� �������� �ߺ� ��� ����
				if( ( mcp_item_slot[0]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_item_slot[0]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_item_slot[1]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_item_slot[1]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_item_slot[2]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_item_slot[2]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_item_slot[3]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_item_slot[3]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_item_slot[4]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_item_slot[4]->PcItem->Code == srcslot->PcItem->Code ) )
				{
					return;
				}

				///-- Amount ������ 1�̻��� �������ϰ��... 
				/*
				if( srcslot->PcItem->ItemTable->theBundle != 1 && srcslot->PcItem->Amount > 1 ) 								
				{
					
					mcp_temp_num_src_slot = srcslot->SlotAddr;
					mcp_temp_num_tga_slot = targetslot;
					

					//������ �Է� �޴´� .. 
					g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_CREATE, targetslot->PcItem );


					///-- Del Cursor					
					nRui->ResetCursorItem();	
					
				}
				else
				{
					///-- Copy item - Ÿ�ٿ� PcItem�� ��°�� ī��
			
					//targetslot->SetSlotPcItem( *(srcslot->PcItem) );								

					///-- Del Cursor					
					nRui->ResetCursorItem();	
				}
				*/
						
					//dogns �ҽ�Ÿ�� �� .. �̹� �����ִ� �����۰� ���Ͽ� ���� üũ �ڵ� üũ �Ͽ� .. �ѷ��ش� �ڵ����� ���� 
					//�ڵ� ó�� �Ѵ� .. 
					for(int i=0; i < 5 ; ++i)
					{
						
						if(targetslot ==  mcp_item_slot[i])
						{
							if( mcp_preitem_slot[i]->PcItem->ItemTable &&
								mcp_preitem_slot[i]->PcItem->ItemTable->theCode	== srcslot->PcItem->Code)
							{
								//�Ѱ��� �� �ʿ��� ���� ���� ũ�� 
								if(srcslot->PcItem->Amount >=  mcp_preitem_slot[i]->PcItem->Amount)
								{
									targetslot->SetSlotPcItem( *(srcslot->PcItem) );				
									targetslot->PcItem->Amount = mcp_preitem_slot[i]->PcItem->Amount;
									
								}
								else
								{			
									SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_VOLUME ) );
								}
								
							}
														
						}
						
					}
					
					nRui->ResetCursorItem();	
					//////////////////////////////////////////////////////////////////////////
					

			}
			else if ( srcslot->Group == n_SGROUP_WEAR)
			{				
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_UPGRADE_WEAR ) );				
			}
		}
		///-------------------------------------------------------------------
		///-- ���� ������ Slot
		else if( targetslot == mcp_elda_slot )
		{
			if(  srcslot->Group == n_SGROUP_INVENTORY  )
			{
				///-- ���� üũ 
				// ����������� ��ų������ üũ�����ʴ´�.  
				if( (srcslot->PcItem->Code < c_ItemCode_Cash_Min )  &&(mi_skill_level < (int)(srcslot->PcItem->ItemTable->theGrade)/100) )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVER_ITEM_LEVEL ) );	
					return;
				}

				bool bIsMakeItemSubJob = true ; 

				///-- By simwoosung �� �κп��� ���� ũ����Ʈ�� 
				///-- �Ƹ� ũ����Ʈ ���� ���� ������ Ŭ���� ���� ��ƾ �߰�
				bIsMakeItemSubJob = IsMakeItemSubJob(srcslot);
				
				//���� �������� ��ȭ �ȵǰ�..
				if( srcslot->PcItem->ItemTable->theType == n_Type_Scroll)
				{
					bIsMakeItemSubJob = false; 
				}

				if(!bIsMakeItemSubJob)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOTUPGRADE_THEITEM) );		
					return;
				}
				
				///-- Amount 

				if( srcslot->PcItem->ItemTable->theBundle != 1 && srcslot->PcItem->Amount > 1 ) 								
				{
					g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_CREATE, targetslot->PcItem );
					mcp_temp_num_src_slot = srcslot->SlotAddr;
					mcp_temp_num_tga_slot = targetslot;
					
					///-- Del Cursor					
					nRui->ResetCursorItem();	
					
				}
				else
				{
					///-- Copy item
					targetslot->SetSlotPcItem( *(srcslot->PcItem) );								
					targetslot->PcItem->Amount = 1;
					
					///-- Del Cursor					
					nRui->ResetCursorItem();	
				}
		
			}
			
		}
		///-------------------------------------------------------------------
		///-- ���� ������ Slot
		else if( targetslot == mcp_Chargeitem_slot[0] || targetslot == mcp_Chargeitem_slot[1] || 
				 targetslot == mcp_Chargeitem_slot[2] )
		{
			if( srcslot->Group == n_SGROUP_INVENTORY )
			{
				///-- ���� �������� �ߺ� ��� ����
				if( ( mcp_Chargeitem_slot[0]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_Chargeitem_slot[0]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_Chargeitem_slot[1]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_Chargeitem_slot[1]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_Chargeitem_slot[2]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_Chargeitem_slot[2]->PcItem->Code == srcslot->PcItem->Code )  )
				{
					return;
				}
				
				///-- �̴� ���� ���� ��ũ�� ���������� �Ǻ�
				int nItemType =	  srcslot->PcItem->ItemTable->theType;
				int nScrollType = srcslot->PcItem->ItemTable->theReHP;
				
				//-- ��ũ�� Ÿ���� �ƴϸ�
				if( n_Type_Scroll != nItemType )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ) );
					return;
				}

				///-- �̴ϰ��� ���� ��ũ���� �ƴϸ�
				if( ( n_Scroll_MGTimeBonus != nScrollType ) && ( n_Scroll_MGLevelBonus != nScrollType ) &&
					( n_Scroll_MGItemBonus != nScrollType ) )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ) );
					return;
				}
				
				///-- Copy item
				targetslot->SetSlotPcItem( *(srcslot->PcItem) );								
				targetslot->PcItem->Amount = 1;
					
				///-- Del Cursor					
				nRui->ResetCursorItem();				
			}		
		}
	}

}

bool CItemCreateWndProc::IsMakeItemSubJob(CSlot * pSlot)
{
	if(NULL == pSlot)
	{
		return false;
	}	
	//�̾������� �������� Ȯ��..
	BYTE aItemLimitFunc = pSlot->PcItem->ItemTable->theItemLimitFunc;
	if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotUpgrade ) )
	{
		return false;
	}

	bool bIsMakeItemSubJob = false;
	int  n_make_type = 0;
	
	//���� �������� ... üũŸ��
	WORD wCode = 0;

	if( pSlot->PcItem->ItemTable )
	{
		WORD wCode = pSlot->PcItem->ItemTable->theCode;
		if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max ) )
		{
			n_make_type = pSlot->PcItem->ItemTable->theType;		
			bIsMakeItemSubJob = true;
			mcp_Item->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );
			mi_make_type = n_make_type;
			goto S_END;
		}

		///--JAPAN_BUG_MODIFY
		///-- �� �̻� ��ȭ�Ұ�
		if(!(pSlot->PcItem->ItemTable->theUpgradeForm))
		{
			bIsMakeItemSubJob = false;
			n_make_type = 0;
			goto S_END;
		}
	}

	if(pSlot->PcItem->ItemTable->theCitizen == 2 )
	{
		
	}
	else if(pSlot->PcItem->ItemTable && 
		(pSlot->PcItem->ItemTable->theCitizen != nRui->GetPcParamInfo()->Citizen ))
	{
		bIsMakeItemSubJob = false;
		n_make_type = 0;
		goto S_END;

	}

	if( 
	//	pSlot->PcItem->ItemTable->theType == n_Type_ChangeHair ||
	//	pSlot->PcItem->ItemTable->theType == n_Type_KeepupHair ||
		pSlot->PcItem->ItemTable->theType == n_Type_Wing)
	{
		n_make_type = pSlot->PcItem->ItemTable->theType;		
		bIsMakeItemSubJob = true;
		mcp_Item->SetText( G_STRING_CHAR(IDS_CHARGE_ITEM) );
		mi_make_type = n_make_type;
		goto S_END;
	}

	
	if( nRui->GetPcParamInfo()->Citizen == MILLENA )
	{
		if( mb_craft_weapon )
		{
			switch(pSlot->PcItem->ItemTable->theType)
			{
				case n_Type_Sword:				///-- ��
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Sword;
					}
					break;
				case n_Type_TwohandSword:	   ///-- ���
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_TwohandSword;
					}
					break;
				case n_Type_Bow:				///-- Ȱ
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Bow;
					}
					break;
			}
		}
		else
		{
			switch(pSlot->PcItem->ItemTable->theType)
			{
				case n_Type_LightCoat:	   ///-- �з��� �Ƹ� �����
				case n_Type_LightPants:	
				case n_Type_LightGlove:	 
				case n_Type_LightShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_LightCoat;
					}
					break;
				case n_Type_MiddleCoat:    ///-- �з��� �Ƹ� �����
				case n_Type_MiddlePants:	
				case n_Type_MiddleGlove:	 
				case n_Type_MiddleShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MiddleCoat;
					}
					break;
				case n_Type_HeavyCoat:    ///-- �з��� �Ƹ� ������
				case n_Type_HeavyPants:	
				case n_Type_HeavyGlove:	 
				case n_Type_HeavyShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_HeavyCoat;
					}
					break;
				case n_Type_Shield:		  ///-- ����	
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Shield;
					}
					break;
				case n_Type_KeepupHair:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_KeepupHair;
					}
					break;
				case n_Type_ChangeHair:
					{	
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_ChangeHair;
					}
					break;
				case n_Type_SetCloth: 
					{	
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_SetCloth;
					}
					break;

			}	
		}
	}
	else
	{
		if( mb_craft_weapon )
		{
			switch(pSlot->PcItem->ItemTable->theType)
			{
				case n_Type_MagicBook:			///-- ������
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MagicBook;
					}
					break;
				case n_Type_Staff:				///-- ������
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Staff;
					}
					break;
				case n_Type_CrystalBall:	   ///-- ������
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_CrystalBall;
					}
					break;
			}			
		}
		else
		{
			switch(pSlot->PcItem->ItemTable->theType)
			{
				case n_Type_MagicCoat:    ///-- ���� ������
				case n_Type_MagicPants:	
				case n_Type_MagicGlove:	 
				case n_Type_MagicShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MagicCoat;
					}
					break;
				case n_Type_PriestCoat:    ///-- ���� ������
				case n_Type_PriestPants:	
				case n_Type_PriestGlove:	 
				case n_Type_PriestShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_PriestCoat;
					}
					break;
				case n_Type_SpiritCoat:    ///-- ���� ���ź�
				case n_Type_SpiritPants:	
				case n_Type_SpiritGlove:	 
				case n_Type_SpiritShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_SpiritCoat;
					}
					break;
				case n_Type_KeepupHair:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_KeepupHair;
					}
					break;
				case n_Type_ChangeHair:
					{	
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_ChangeHair;
					}
					break;
				case n_Type_SetCloth: 
					{	
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_SetCloth;
					}
					break;
			}
		}
	}

	if(bIsMakeItemSubJob)
	{
		if(mb_craft_weapon)
		{
			mcp_Item->SetText( G_STRING_CHAR(IDS_ARM_ITEM) );
		}
		else
		{
			mcp_Item->SetText( G_STRING_CHAR(IDS_GUARD_ITEM) );							
		}
		
		mi_make_type = n_make_type;
		Change_make_type(pSlot);
	}
	
S_END:
	
	return bIsMakeItemSubJob;
}

bool CItemCreateWndProc::IsMakeItemSubJob(BYTE aType)
{
	bool bIsMakeItemSubJob = false;
	int  n_make_type = 0;

	WORD wCode = 0;

	if( mcp_elda_slot->PcItem->ItemTable )
	{
		WORD wCode = mcp_elda_slot->PcItem->ItemTable->theCode;
		if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max ) )
		{
			n_make_type = mcp_elda_slot->PcItem->ItemTable->theType;		
			bIsMakeItemSubJob = true;
			mcp_Item->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );
			mi_make_type = n_make_type;
			goto S_END;
		}
	}

	if(	
	//aType == n_Type_ChangeHair ||
	//aType == n_Type_KeepupHair 
		aType == n_Type_Wing )
	{
		n_make_type = aType;
		bIsMakeItemSubJob = true;
		goto S_END;
	}
	
	if( nRui->GetPcParamInfo()->Citizen == MILLENA )
	{
		if( mb_craft_weapon )
		{
			switch(aType)
			{
				case n_Type_Sword:				///-- ��
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Sword;
					}
					break;
				case n_Type_TwohandSword:	   ///-- ���
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_TwohandSword;
					}
					break;
				case n_Type_Bow:				///-- Ȱ
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Bow;
					}
					break;
			}
		}
		else
		{
			switch(aType)
			{
				case n_Type_LightCoat:	   ///-- �з��� �Ƹ� �����
				case n_Type_LightPants:	
				case n_Type_LightGlove:	 
				case n_Type_LightShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_LightCoat;
					}
					break;
				case n_Type_MiddleCoat:    ///-- �з��� �Ƹ� �����
				case n_Type_MiddlePants:	
				case n_Type_MiddleGlove:	 
				case n_Type_MiddleShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MiddleCoat;
					}
					break;
				case n_Type_HeavyCoat:    ///-- �з��� �Ƹ� ������
				case n_Type_HeavyPants:	
				case n_Type_HeavyGlove:	 
				case n_Type_HeavyShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_HeavyCoat;
					}
					break;
				case n_Type_Shield:		  ///-- ����	
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Shield;
					}
					break;
				case n_Type_KeepupHair:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_KeepupHair;
					}
					break;
				case n_Type_ChangeHair:
					{	
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_ChangeHair;
					}
				case n_Type_SetCloth: 
					{	
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_SetCloth;
					}
					break;
			}	
		}
	}
	else
	{
		if( mb_craft_weapon )
		{
			switch(aType)
			{
				case n_Type_MagicBook:			///-- ������
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MagicBook;
					}
					break;
				case n_Type_Staff:				///-- ������
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Staff;
					}
					break;
				case n_Type_CrystalBall:	   ///-- ������
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_CrystalBall;
					}
					break;
			}			
		}
		else
		{
			switch(aType)
			{
				case n_Type_MagicCoat:    ///-- ���� ������
				case n_Type_MagicPants:	
				case n_Type_MagicGlove:	 
				case n_Type_MagicShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MagicCoat;
					}
					break;
				case n_Type_PriestCoat:    ///-- ���� ������
				case n_Type_PriestPants:	
				case n_Type_PriestGlove:	 
				case n_Type_PriestShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_PriestCoat;
					}
					break;
				case n_Type_SpiritCoat:    ///-- ���� ���ź�
				case n_Type_SpiritPants:	
				case n_Type_SpiritGlove:	 
				case n_Type_SpiritShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_SpiritCoat;
					}
					break;
				case n_Type_KeepupHair:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_KeepupHair;
					}
					break;
				case n_Type_ChangeHair:
					{	
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_ChangeHair;
					}
					break;
				case n_Type_SetCloth: 
					{	
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_SetCloth;
					}
					break;
			}
		}
	}

   
S_END:

	if(bIsMakeItemSubJob)
	{
		mi_make_type = n_make_type;		
	}	

	return bIsMakeItemSubJob;

}


///---------------------------------------------------------------------------
///-- LoadCreatelTable
///---------------------------------------------------------------------------
///-- Remodel Table �� �о� ���δ�.
///-- ���� Wnd �� Open �� �� �ѹ��� ����
BOOL CItemCreateWndProc::LoadCreateTable()
{	
	int i;

	TCHAR filename[50];
	
///////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	FILE* fp;
	
	sprintf( filename, "makeitem.dat");
	
	fp = _tfopen( filename, _T("rb") );//VSS�ø���...�ɼ� ����(�б�����)�϶�....
	if( fp == NULL )
	{
		MessageBox( NULL, _T("!makeitem.dat ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return false;
	}
	
	struct _stat file_buf;
	int li_file_size = _stat( filename, &file_buf );
		
	///-- ������ ���Լ� Check
	if( li_file_size % sizeof( SMaKeItemTable ) != 0 )
	{
		return( false );
	}

	///-- Set Count
	mi_table_count = li_file_size / sizeof( SMaKeItemTable );

	int a = sizeof( SMaKeItemTable );
	///-- read data
	g_CreatelTable = SAFE_NEW_ARRAY( SMaKeItemTable , mi_table_count );	

	if( g_CreatelTable == NULL )
	{
		return FALSE;
	}

	for( i = 0; i < mi_table_count; ++i )
	{
		///-- Read Header
		fread( ( void * )&g_CreatelTable[i], sizeof( SMaKeItemTable ), 1, fp);
	}


	mb_loadCreatelTable = true;
	return true;

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	///-- File Size Check
	wsprintf( filename, _T( "makeitem.dat" ));
	
	///-- File Open
	VFileHandle* pFH = g_VFEtc.OpenFile( filename );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!makeitem.dat ���� ���� ����!!!"), _T("�˸�"), MB_OK );
		return false;
	}

	int li_file_size = pFH->GetSize();
	

	
	///-- ������ ���Լ� Check
	if( li_file_size % sizeof( SMaKeItemTable ) != 0 )
	{
		return( false );
	}
	
	///-- Set Count
	mi_table_count = li_file_size / sizeof( SMaKeItemTable );

	int a = sizeof( SMaKeItemTable );
	///-- read data
	g_CreatelTable = SAFE_NEW_ARRAY( SMaKeItemTable , mi_table_count );	
	if( g_CreatelTable == NULL )
	{
		return FALSE;
	}

	for( i = 0; i < mi_table_count; ++i )
	{
		///-- Read Header
		pFH->Read( ( void * )&g_CreatelTable[i], sizeof( SMaKeItemTable ));	
	}


	mb_loadCreatelTable = true;
	g_VFEtc.CloseFile( pFH );
	return true;
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
}


