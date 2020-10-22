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
		///-- 밀레나 /웨폰  : 검 대검 활
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
		///-- 레인  /웨폰   : 마법서 수정구 스태프
		
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
		///-- 밀레나 /아머  : 경장비 중장비 헤비장비 방패
	case 21:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_LARMOR ) );
		mcp_item_type->SetClientImage( 198 );break;	
	case 31:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_MARMOR ) );
		mcp_item_type->SetClientImage( 1379 );break;
	case 41:	
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_HARMOR ) );
		mcp_item_type->SetClientImage( 1391 );break;
	
		///-- 방패 삭제
		/*
	case 100:	///-- 방패 / 337
		mi_make_type = 21;
		mcp_create_type->SetText( G_STRING_CHAR( IDS_ITEM_TYPE_LARMOR ) );
		mcp_item_type->SetClientImage( 198 );break;
		*/
		
		///---------------------------------------------------------------------------
		///-- 레인  /아머	: 마도복 강신복 사제복 
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



	///-- 제조 원아이템 (Static)
	mcp_Item = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 12, 130, 40, 17, false );
	mcp_Item->SetFontR( n_fRsvFontWndStatic );
	mcp_Item->SetAlignText( n_atLeft, n_atCenter );
	mcp_Item->SetText( G_STRING_CHAR( IDS_ITEM ) );

	///-- 첨가물 (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 149, 130, 55, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_ITEM_ADDITIVE ) );

	///-- 예상 아이템 (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 105, 198, 90, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR( IDS_WND_ITEM_OUTPUT_ITEM ) );

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_ITEMCREATE_BT_X, mpC_framewnd->GetID(), 267, 6, 13, 13, false ); 
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );	
	
	
	///-- 아이템 강화를 위해 준비할 아이템을 미리보여줌 Slot
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
	
	
	///-- 아이템 Slot
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
	
	///-- 제조  버튼
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
	
	///-- 중단 버튼
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

	///-- 타입 변경
	mcp_change_type = (CButton*)nRui->CreateWnd( n_wtButton, WID_ITEMCREATE_BT_TYPE, mpC_framewnd->GetID(), 13, 32, 42, 41, false );
	mcp_change_type->SetBtnImage( TID_C_WND_ELDA_SB_SLOTBT, TID_SlotHighLight, TID_C_WND_ELDA_SB_SLOTBT_CLK, TID_None );
	mcp_change_type->SetVisible(false);

	///-- TEXT 제조 타입
	mcp_create_type = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_ITEMCREATE_TEXT_TYPE, 
												  mpC_framewnd->GetID(), 70, 32, 115, 17, false );
	mcp_create_type->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdShadow );
	mcp_create_type->SetClientColor( 255, 255, 255 );
	mcp_create_type->SetClientImage( TID_None );	
	mcp_create_type->SetAlignText( n_atLeft, n_atCenter );	
	mcp_create_type->SetTextColor( 145, 155, 255 );
	mcp_create_type->SetBlank( 0, 0 );	
	mcp_create_type->SetVisible(false);

	
	///-- TEXT 예상 아이템
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

	///-- 유료 아이템 버튼
	mcp_chargeWindow_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, mpC_framewnd->GetID(), 
			190, 227, 87, 25, false );
	mcp_chargeWindow_btn->SetBtnImage( TID_CTRL_BTN87, TID_CTRL_BTN87_OVR, TID_CTRL_BTN87_CLK, TID_None );			
	mcp_chargeWindow_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_chargeWindow_btn->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );										
	mcp_chargeWindow_btn->SetVisible(false);
	
#ifdef C_PAY_SYSTEM	
	mcp_chargeWindow_btn->SetVisible(true);
#endif
	
	///-- 유료 아이템 등록 윈도우
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


	///-- 예상 아이템 (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpCharge_framewnd->GetID(), 
			105, 52, 90, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );						

	
	CloseCharge_Wnd();	
	
	///-- 제조 Table
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
	int li_skill_max;	///-- 무기/방어구 강화술에 따른 최고 스킬레벨
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
	mcp_create_btn->SetBtnHold( true );			 //아이템 생성버튼 홀딩
	mb_button_lock = false;
	
	m_IsViewchargeWindow = false;
	CloseCharge_Wnd();	
	
	
	///-- 조합 불가						
	Rsprintf( RWCHAR(tmpStr), _RT(""));
	mcp_create_type->SetText( tmpStr );
		
	///---------------------------------------------------------------------------
	///-- 웨폰 / 아머 크래프트 결정
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
	///-- 전체 Skill 중에 개조 최고 레벨 찾음
	for( j = 0; j < RP_MAX_USER_SKILL; ++j )
	{	
		if( nRui->GetPcInvenInfo()->SkillItem[j] == NULL )
		{
			continue;
		}
		
		///-- Max Level 찾음 - 여기서는 스킬이 하나만 존재한다는 전제....
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
	///-- 종족에 따른 제조 Type 결정
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
		///-- Type 변경
	case WID_ITEMCREATE_BT_TYPE:

		///-- Image 의 버튼 효과
		if( mcp_change_type->GetBtnState() == n_bsSelect )
		{
			mcp_item_type->SetWndRepositionR(17,36);
		}
		else
		{
			mcp_item_type->SetWndRepositionR(16,35);		
		}
		
		///-- Type 변경
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
		
		///-- 제조
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
				///-- 유료 아이템 카피
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
				///-- 제조 가능한지 Check
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

	///-- 가공 가능한지 Check
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

	for( i = 0 ; i < mi_table_count ; ++i )  //제조 테이블 갯수만큼 반복
	{
		///-- 원 제조 아이템과 동일한 제조테이블을 비교한다.
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
					mcp_preitem_slot[k]->PcItem->IType	= n_ITYPE_USE;//소모품
				}
				else if( g_Pc_Manager.GetItemClsss( g_CreatelTable[i].theAddItem[j].theItemCode ) == n_Class_Useless )
				{
					mcp_preitem_slot[k]->PcItem->IType	= n_ITYPE_UNUSE;//비소모품
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
///-- 가공 가능한지 Check 하고 Button 을 활성화 한다.
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
	
	///-- 전체 Table Access
	for( i = 0 ; i < mi_table_count ; ++i )  //제조 테이블 갯수만큼 반복
	{
			///-- 원 제조 아이템과 동일한 제조테이블을 비교한다.
			if ( mcp_elda_slot->PcItem->ItemTable && 
				 mcp_elda_slot->PcItem->ItemTable->theUpgradeForm == g_CreatelTable[i].theIndex )
			{
				///-- flag 초기화
				lb_match[0] = lb_match[1] = lb_match[2] = lb_match[3] = lb_match[4] = false;
				lb_ck_table[0] = lb_ck_table[1] = lb_ck_table[2] = lb_ck_table[3] = lb_ck_table[4] = false; 
				
				///-- 현재 아이템 슬롯과 제조테이블의 비교루틴..
				///-- 아이템 슬롯 Access
				for( j = 0 ; j < 5 ; ++j )
				{
					///-- 제조 Table 
					for( k = 0 ; k < 5 ; ++k )
					{
						///-- 빈 Slot
						if( mcp_item_slot[j]->PcItem->IType == n_ITYPE_NONE)
						{
							if( g_CreatelTable[i].theAddItem[k].theItemCode == 0 && !lb_ck_table[k])
							{
								lb_match[j] = true;
								lb_ck_table[k] = true;
								k = 6;
							}
						}
						///-- 아이템이 있는 Slot
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
				///-- 조합가능 , 버튼 및 Text Set
				if( lb_match[0] && lb_match[1] && lb_match[2] && lb_match[3] && lb_match[4]
					/*!( mcp_item_slot[0]->PcItem->IType == n_ITYPE_NONE &&
					mcp_item_slot[1]->PcItem->IType == n_ITYPE_NONE &&
					mcp_item_slot[2]->PcItem->IType == n_ITYPE_NONE )*/ ///<--이 루틴은 왜 들어가있는건지.. 
					)
				{
					///-- Button
					mcp_create_btn->SetBtnHold( false );
					///-- real type;
					mi_item_type = g_CreatelTable[i].theType;					
					
					///-- 결과 아이템
					SItemBaseInfo* item_Info = NULL;
					
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( mcp_elda_slot->PcItem->ItemTable->theUpgradeItem );
					
					if( !item_Info )
					{
						MessageBox( NULL, "Critical Error: Missiong Item From Makeitem Table", NULL, NULL);
					}
					
					///-- 결과 item 
					mcp_target_slot->PcItem->Class	= g_Pc_Manager.GetItemClsss( mcp_elda_slot->PcItem->ItemTable->theUpgradeItem  );				
					mcp_target_slot->PcItem->ItemUID = 1;
					mcp_target_slot->PcItem->IType = n_ITYPE_WEAR;	///-- Wear
					mcp_target_slot->PcItem->Amount = 1;
					mcp_target_slot->PcItem->Code	= mcp_elda_slot->PcItem->ItemTable->theUpgradeItem ;
					///-- 성별에 따른 아이템 결과물 아이디
					mcp_target_slot->PcItem->Tid	= (TEXID)g_Pc_Manager.GetItemTID( mcp_elda_slot->PcItem->ItemTable->theUpgradeItem  );
					mcp_target_slot->PcItem->ItemTable	= item_Info;
					mcp_target_slot->PcItem->WearTable.theEndurance = mcp_target_slot->PcItem->ItemTable->theMaxEndurance;					
					memcpy( &(mcp_target_slot->PcItem->WearTable), &(mcp_elda_slot->PcItem->WearTable), sizeof( SRpdWearItemBase ) );
										
					///-- 헬프 아이템팁의 정보를 재갱신한다.
					nRui->Refresh_ItemHelpTip();
					return;
				}
			}///-- End of if 원석 동일
		

	}///-- 전체Table Access 끝
	
	
	///-- 조합 불가						
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
	///-- 전체 인벤토리 Access
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

				///-- 수량 체크
				if( mcp_item_slot[i]->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
				{
					mcp_item_slot[i]->PcItem->Clear();
				}
			}
		}
		
		///-- 제조 아이템 slot
		if( mcp_elda_slot->PcItem && 
			mcp_elda_slot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_elda = true;

			///-- 수량 체크
			if( mcp_elda_slot->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
			{
				mcp_elda_slot->PcItem->Clear();				
			}
		}

		///-- 유료 아이템 slot
		for( i = 0; i < 3; i++)
		{
			///-- item slot
			if( mcp_Chargeitem_slot[i]->PcItem && 
				mcp_Chargeitem_slot[i]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
			{
				lb_isFind_Chargeitem[i] = true;

				///-- 수량 체크
				if( mcp_Chargeitem_slot[i]->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
				{
					mcp_Chargeitem_slot[i]->PcItem->Clear();
				}
			}
		}
	}
	
	///-----------------------------------------------------------------------
	///-- item 을 찾지 못 했을 경우
	///-- Item Check
	for( i = 0; i < 5; i++)
	{
		///-- item slot
		if( !lb_isFind_item[i] )
		{
			mcp_item_slot[i]->PcItem->Clear();
		}
	}

	///-- 제조 아이템을 찾지 못 했을 경우
	if( !lb_isFind_elda )
	{
		mcp_elda_slot->PcItem->Clear();
		ClearComponentItemList();
	}

	///------------------------------------------------------------------------
	///-- 유료 item 을 찾지 못 했을 경우
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
///-- Item 제거
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
///-- Item 올리기
void CItemCreateWndProc::ItemSlotClick( SEventMessage* EMsg )
{
	CSlot*		targetslot	= NULL;//현재 장비 슬롯
	CSlot*		srcslot		= NULL;//선택된(pick) 인벤 슬롯
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;

	RLG1("EMsg->FocusWnd %d",EMsg->FocusWnd);
	///-- 아이템을 들고 있을 경우
	if( nRui->GetCursorItem() != NULL )
	{
		//포커스 윈도우 ID 를 넘겨 컨테이너를 돌아 해당 우치 박스를 찾아 리턴한다 ..  연결시키는 거지 .. ㅋㅋ
		targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		//소스슬롯 은 마우스 에 붙어 있는 정보를 가져온다 .. 
		srcslot = nRui->GetItemSelectedSlot();

		///- 선택된 아이템이 없을경우
		if( srcslot->PcItem == NULL )	
		{
			return;
		}	
		
		///- 이런 상황은 있을수 없지만....타겟슬롯은 자체 PcItem정보를 가지고있어서...
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
				///-- 같은 아이템의 중복 등록 방지
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

				///-- Amount 개수가 1이상인 아이템일경우... 
				/*
				if( srcslot->PcItem->ItemTable->theBundle != 1 && srcslot->PcItem->Amount > 1 ) 								
				{
					
					mcp_temp_num_src_slot = srcslot->SlotAddr;
					mcp_temp_num_tga_slot = targetslot;
					

					//갯수를 입력 받는다 .. 
					g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_CREATE, targetslot->PcItem );


					///-- Del Cursor					
					nRui->ResetCursorItem();	
					
				}
				else
				{
					///-- Copy item - 타겟에 PcItem을 통째로 카피
			
					//targetslot->SetSlotPcItem( *(srcslot->PcItem) );								

					///-- Del Cursor					
					nRui->ResetCursorItem();	
				}
				*/
						
					//dogns 소스타켓 과 .. 이미 나와있는 아이템과 비교하여 수량 체크 코드 체크 하여 .. 뿌려준다 자동으로 루팅 
					//자동 처리 한다 .. 
					for(int i=0; i < 5 ; ++i)
					{
						
						if(targetslot ==  mcp_item_slot[i])
						{
							if( mcp_preitem_slot[i]->PcItem->ItemTable &&
								mcp_preitem_slot[i]->PcItem->ItemTable->theCode	== srcslot->PcItem->Code)
							{
								//총갯수 가 필요한 갯수 보다 크면 
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
		///-- 제조 아이템 Slot
		else if( targetslot == mcp_elda_slot )
		{
			if(  srcslot->Group == n_SGROUP_INVENTORY  )
			{
				///-- 레벨 체크 
				// 유료아이템은 스킬레벨을 체크하지않는다.  
				if( (srcslot->PcItem->Code < c_ItemCode_Cash_Min )  &&(mi_skill_level < (int)(srcslot->PcItem->ItemTable->theGrade)/100) )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVER_ITEM_LEVEL ) );	
					return;
				}

				bool bIsMakeItemSubJob = true ; 

				///-- By simwoosung 이 부분에서 웨폰 크래프트와 
				///-- 아머 크래프트 제조 가능 아이템 클래스 구별 루틴 추가
				bIsMakeItemSubJob = IsMakeItemSubJob(srcslot);
				
				//유료 아이템은 강화 안되게..
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
		///-- 유료 아이템 Slot
		else if( targetslot == mcp_Chargeitem_slot[0] || targetslot == mcp_Chargeitem_slot[1] || 
				 targetslot == mcp_Chargeitem_slot[2] )
		{
			if( srcslot->Group == n_SGROUP_INVENTORY )
			{
				///-- 같은 아이템의 중복 등록 방지
				if( ( mcp_Chargeitem_slot[0]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_Chargeitem_slot[0]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_Chargeitem_slot[1]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_Chargeitem_slot[1]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_Chargeitem_slot[2]->PcItem->IType != n_ITYPE_NONE && 
					  mcp_Chargeitem_slot[2]->PcItem->Code == srcslot->PcItem->Code )  )
				{
					return;
				}
				
				///-- 미니 게임 관련 스크롤 아이템인지 판별
				int nItemType =	  srcslot->PcItem->ItemTable->theType;
				int nScrollType = srcslot->PcItem->ItemTable->theReHP;
				
				//-- 스크롤 타입이 아니면
				if( n_Type_Scroll != nItemType )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ) );
					return;
				}

				///-- 미니게임 관련 스크롤이 아니면
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
	//이아이템이 가공여부 확인..
	BYTE aItemLimitFunc = pSlot->PcItem->ItemTable->theItemLimitFunc;
	if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotUpgrade ) )
	{
		return false;
	}

	bool bIsMakeItemSubJob = false;
	int  n_make_type = 0;
	
	//유료 아이템은 ... 체크타입
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
		///-- 더 이상 강화불가
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
				case n_Type_Sword:				///-- 검
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Sword;
					}
					break;
				case n_Type_TwohandSword:	   ///-- 대검
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_TwohandSword;
					}
					break;
				case n_Type_Bow:				///-- 활
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
				case n_Type_LightCoat:	   ///-- 밀레나 아머 경장비
				case n_Type_LightPants:	
				case n_Type_LightGlove:	 
				case n_Type_LightShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_LightCoat;
					}
					break;
				case n_Type_MiddleCoat:    ///-- 밀레나 아머 중장비
				case n_Type_MiddlePants:	
				case n_Type_MiddleGlove:	 
				case n_Type_MiddleShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MiddleCoat;
					}
					break;
				case n_Type_HeavyCoat:    ///-- 밀레나 아머 헤비장비
				case n_Type_HeavyPants:	
				case n_Type_HeavyGlove:	 
				case n_Type_HeavyShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_HeavyCoat;
					}
					break;
				case n_Type_Shield:		  ///-- 방패	
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
				case n_Type_MagicBook:			///-- 마법서
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MagicBook;
					}
					break;
				case n_Type_Staff:				///-- 스태프
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Staff;
					}
					break;
				case n_Type_CrystalBall:	   ///-- 수정구
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
				case n_Type_MagicCoat:    ///-- 레인 마도복
				case n_Type_MagicPants:	
				case n_Type_MagicGlove:	 
				case n_Type_MagicShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MagicCoat;
					}
					break;
				case n_Type_PriestCoat:    ///-- 레인 사제복
				case n_Type_PriestPants:	
				case n_Type_PriestGlove:	 
				case n_Type_PriestShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_PriestCoat;
					}
					break;
				case n_Type_SpiritCoat:    ///-- 레인 강신복
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
				case n_Type_Sword:				///-- 검
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Sword;
					}
					break;
				case n_Type_TwohandSword:	   ///-- 대검
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_TwohandSword;
					}
					break;
				case n_Type_Bow:				///-- 활
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
				case n_Type_LightCoat:	   ///-- 밀레나 아머 경장비
				case n_Type_LightPants:	
				case n_Type_LightGlove:	 
				case n_Type_LightShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_LightCoat;
					}
					break;
				case n_Type_MiddleCoat:    ///-- 밀레나 아머 중장비
				case n_Type_MiddlePants:	
				case n_Type_MiddleGlove:	 
				case n_Type_MiddleShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MiddleCoat;
					}
					break;
				case n_Type_HeavyCoat:    ///-- 밀레나 아머 헤비장비
				case n_Type_HeavyPants:	
				case n_Type_HeavyGlove:	 
				case n_Type_HeavyShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_HeavyCoat;
					}
					break;
				case n_Type_Shield:		  ///-- 방패	
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
				case n_Type_MagicBook:			///-- 마법서
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MagicBook;
					}
					break;
				case n_Type_Staff:				///-- 스태프
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_Staff;
					}
					break;
				case n_Type_CrystalBall:	   ///-- 수정구
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
				case n_Type_MagicCoat:    ///-- 레인 마도복
				case n_Type_MagicPants:	
				case n_Type_MagicGlove:	 
				case n_Type_MagicShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_MagicCoat;
					}
					break;
				case n_Type_PriestCoat:    ///-- 레인 사제복
				case n_Type_PriestPants:	
				case n_Type_PriestGlove:	 
				case n_Type_PriestShoes:
					{
						bIsMakeItemSubJob = true;
						n_make_type = n_Type_PriestCoat;
					}
					break;
				case n_Type_SpiritCoat:    ///-- 레인 강신복
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
///-- Remodel Table 을 읽어 들인다.
///-- 최초 Wnd 가 Open 될 때 한번만 읽음
BOOL CItemCreateWndProc::LoadCreateTable()
{	
	int i;

	TCHAR filename[50];
	
///////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	FILE* fp;
	
	sprintf( filename, "makeitem.dat");
	
	fp = _tfopen( filename, _T("rb") );//VSS올리때...옵션 주의(읽기전용)일때....
	if( fp == NULL )
	{
		MessageBox( NULL, _T("!makeitem.dat 파일 열기 실패!!!"), _T("알림"), MB_OK );
		return false;
	}
	
	struct _stat file_buf;
	int li_file_size = _stat( filename, &file_buf );
		
	///-- 사이즈 정규성 Check
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
		MessageBox( NULL, _T("!makeitem.dat 파일 열기 실패!!!"), _T("알림"), MB_OK );
		return false;
	}

	int li_file_size = pFH->GetSize();
	

	
	///-- 사이즈 정규성 Check
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


