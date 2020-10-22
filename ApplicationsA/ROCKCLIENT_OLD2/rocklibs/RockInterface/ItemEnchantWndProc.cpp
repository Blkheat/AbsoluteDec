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

#include "..\\..\\bravolibs\\obj\\SkillSystem.h"

//#include "pc.h"
#include "InventoryWndProc.h"
#include "ItemEnchantWndProc.h"
#include "StringManager.h"


CItemEnchantWndProc	g_ItemEnchantWndProc;
CFontg*				l_Fontg;	
extern CFontman*	Fontman ;

DWORD g_NeedCost[10] = {10000 ,	20000 ,	50000 , 80000 , 150000 , 300000 , 600000 , 1000000 , 2000000 , 3000000 };


CItemEnchantWndProc::CItemEnchantWndProc()
{
	m_IsViewchargeWindow = FALSE;

	m_pWholeFrame = NULL;				
	mpC_framewnd = NULL;

	

}

CItemEnchantWndProc::~CItemEnchantWndProc()
{

}

//-----------------------------------------------------------------------------
void CItemEnchantWndProc::Init()
{
	mcp_Level = NULL;
	mcp_Item = NULL;

	mcp_effect[0] = NULL;
	mcp_effect[1] = NULL;
	mcp_effect[2] = NULL;
	mcp_effect[3] = NULL;
	mcp_effect[4] = NULL; 
	mcp_effect[5] = NULL; 
	mcp_effect[6] = NULL;

}

void CItemEnchantWndProc::SetSkillLevel()
{
	int li_max =0;
	int li_skill_max;	///-- ����/�� �������� ���� �ְ� ��ų����
	int li_skill_min;		
	
	if( IsVisible )
	{
		return;
	}
	
	///-- ���� ũ����Ʈ�� �Ƹ�ũ����Ʈ�� ����
	int nSubClass = nRui->GetPcParamInfo()->SubClass;
	if( nSubClass != ArmCraft && nSubClass != ArmorCraft )
	{
		return;
	}

	if(nSubClass == ArmCraft)
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
		li_skill_min = c_SkillCode_WeaponEnchant_Min;
		li_skill_max = c_SkillCode_WeaponEnchant_Max;
	}
	else
	{
		li_skill_min = c_SkillCode_ArmorEnchant_Min;
		li_skill_max = c_SkillCode_ArmorEnchant_Max;
	}

	///---------------------------------------------------------------------------	
	///-- ��ü Skill �߿� ���� �ְ� ���� ã��
	for(int j = 0; j < RP_MAX_USER_SKILL; ++j )
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
}

///---------------------------------------------------------------------------
///-- OpenEnchantWnd
///---------------------------------------------------------------------------
///-- ���� â�� ����
///-- Slot �� �ʱ�ȭ �Ѵ�.
///-- 
void CItemEnchantWndProc::OpenEnchantWnd()
{
	TCHAR tmpStr[128] = {0,};

#ifndef C_SUBJOB_RENEW
	SetSkillLevel();
#endif 


	memset( m_dPayItem, NULL, sizeof( m_dPayItem ) );
	
	nRui->SendEvent( WID_SUB_ENCHANT, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
	Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT(""));
	mcp_effect[0]->SetText( tmpStr );				
	mcp_effect[1]->SetText( tmpStr );			
	mcp_effect[2]->SetText( tmpStr );
	
#ifdef MIX_ELDASTONE
	mcp_effect[3]->SetText( tmpStr );			
	mcp_effect[4]->SetText( tmpStr );
	mcp_effect[5]->SetText(	tmpStr );
#endif 

#ifdef C_SUBJOB_RENEW
	mcp_effect[6]->SetText(	tmpStr );
#endif 

	mcp_Level->SetText( tmpStr );
	
	mcp_item_slot->PcItem->Clear();
	mcp_elda_slot->PcItem->Clear();
	mcp_start_btn->SetBtnHold( true );
	mb_button_lock = false;		

	mcp_Chargeitem_slot[0]->PcItem->Clear();
	mcp_Chargeitem_slot[1]->PcItem->Clear();
	mcp_Chargeitem_slot[2]->PcItem->Clear();
}
///---------------------------------------------------------------------------
///-- CloseEnchantWnd
///---------------------------------------------------------------------------
///-- ���� â�� �ݴ´�.
///-- �� Resource �� �����Ѵ�. ( Dynamic Texture_M �� �����Ǿ�� �� ) 
void CItemEnchantWndProc::CloseEnchantWnd()
{
	nRui->SendEvent( WID_SUB_ENCHANT, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	///TID_ELD_WND_UpGrade ����

}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------

void CItemEnchantWndProc::ComposionNew()
{
	
	CFrameWnd*		fw = NULL;
	CImageBox*		img;
	CButton*		btn;
	CTextBox*		tbox;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;

	enchantwnd.w	= 204;
	enchantwnd.h	= 200;

	int AddHeight = 36 + 18 + 18  ; 

#if defined( PAYITEM_6 ) && defined( C_PAY_SYSTEM )
	
	enchantwnd.w	= 254;
	enchantwnd.h	= 235 + AddHeight ;

#endif

	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	


	///-- Frame Wnd
	m_pWholeFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SUB_ENCHANT, WID_None, 
									  enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );	
	m_pWholeFrame->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pWholeFrame->SetClientImage( TID_CTRL_WND_L1 );
	m_pWholeFrame->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pWholeFrame->SetWndTileSizeLR( 40, 29 );
	m_pWholeFrame->SetWndTileSizeTB( 31, 11 );

	m_pWholeFrame->SetCaption( G_STRING_CHAR(IDS_WND_ENCHANT) );							
	m_pWholeFrame->SetTitle( true );	
	m_pWholeFrame->SetWndProc( this );
	m_pWholeFrame->SetVisible( false );
	m_pWholeFrame->Initialize();


	///-- Frame Wnd
	mpC_framewnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None , WID_SUB_ENCHANT, 
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

	SRect sTempRect;

	sTempRect.x	= 1;
	sTempRect.y	= 111;
	sTempRect.w	= 252;
	sTempRect.h	= 55 + AddHeight ;

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	sTempRect.x	= 1;
	sTempRect.y	= 110;
	sTempRect.w	= 252;
	sTempRect.h	= 1;


	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(),  
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	sTempRect.x	= 1;
	sTempRect.y	= 166 + AddHeight;
	sTempRect.w	= 252;
	sTempRect.h	= 1;


	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );	
	
	sTempRect.x	= 58;
	sTempRect.y	= 56;
	sTempRect.w	= 35;
	sTempRect.h	= 35;

	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	sTempRect.x	= 157;
	sTempRect.y	= 56;
	sTempRect.w	= 35;
	sTempRect.h	= 35;


	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );            
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	sTempRect.x	= 115;
	sTempRect.y	= 51;
	sTempRect.w	= 21;
	sTempRect.h	= 35;


	///-- Level
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );                
	img ->SetClientImage( TID_C_WND_ELDA_SB_LV );

	sTempRect.x	= 37;
	sTempRect.y	= 36;
	sTempRect.w	= 80;
	sTempRect.h	= 17;
	
	///-- ������( Static Text )
	mcp_Item = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	mcp_Item->SetFontR( n_fRsvFontWndStatic );
	mcp_Item->SetText( G_STRING_CHAR( IDS_ITEM ) );

	sTempRect.x	= 135;
	sTempRect.y	= 36;
	sTempRect.w	= 80;
	sTempRect.h	= 17;

	///-- ���ٽ���( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR( IDS_ELDASTONE ));

	sTempRect.x	= 70;
	sTempRect.y	= 95;
	sTempRect.w	= 110;
	sTempRect.h	= 17;

	
	///-- �ռ�ȿ��  ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_ENCHANT_EFFECT ) );
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- ���� ��ư

	sTempRect.x	= 235;
	sTempRect.y	= 6;
	sTempRect.w	= 13;
	sTempRect.h	= 13;

	///-- �ݱ�
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_ENCHANT_BTN_CLOSE, mpC_framewnd->GetID(), 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	sTempRect.x	= 56;
	sTempRect.y	= 171 + AddHeight ;
	sTempRect.w	= 58;
	sTempRect.h	= 25;


	mcp_start_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_ENCHANT_BTN_OK, mpC_framewnd->GetID(), 
											   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	mcp_start_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	mcp_start_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_start_btn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) ); 
	mcp_start_btn->SetBtnHold( true );

	sTempRect.x	= 144;
	sTempRect.y	= 171 + AddHeight;
	sTempRect.w	= 58;
	sTempRect.h	= 25;

	///-- �ߴ�
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_ENCHANT_BTN_ABT, mpC_framewnd->GetID(), 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );	
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 

	sTempRect.x	= 58;
	sTempRect.y	= 56;
	sTempRect.w	= 35;
	sTempRect.h	= 35;

		
	///-- ������ Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_ENCHANT_SLOT_ITEM, mpC_framewnd->GetID(), 
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_item_slot->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot->SetHID(HID_ITEMTIP);
	sTempRect.x	= 157;
	sTempRect.y	= 56;
	sTempRect.w	= 35;
	sTempRect.h	= 35;

	///-- Elda Slot
	mcp_elda_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_ENCHANT_SLOT_ELDA, mpC_framewnd->GetID(), 
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	mcp_elda_slot->SetSlotGroup( n_SGROUP_TUNING );
	mcp_elda_slot->SetHID(HID_ITEMTIP);
	///-- None Item ����
	mcp_item_slot->PcItem = &mc_itemslot_item;		
	mcp_elda_slot->PcItem = &mc_eldaslot_item;


	sTempRect.x	= 30;
	sTempRect.y	= 114;
	sTempRect.w	= 190;
	sTempRect.h	= 14;

	mcp_effect[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT1, mpC_framewnd->GetID(), 
												sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[0]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[0]->SetBlank( 0, 0 );

	sTempRect.x	= 30;
	sTempRect.y	= 132;
	sTempRect.w	= 190;
	sTempRect.h	= 14;

	mcp_effect[1]= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT2, mpC_framewnd->GetID(), 
											   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	mcp_effect[1]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[1]->SetBlank( 0, 0 );	

	sTempRect.x	= 30;
	sTempRect.y	= 150;
	sTempRect.w	= 190;
	sTempRect.h	= 14;

	mcp_effect[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT3, mpC_framewnd->GetID(), 
												sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[2]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[2]->SetBlank( 0, 0 );	


	sTempRect.x	= 30;
	sTempRect.y	= 168;
	sTempRect.w	= 190;
	sTempRect.h	= 14;
		
	mcp_effect[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[3]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[3]->SetBlank( 0, 0 );	
	
	
	
	sTempRect.x	= 30;
	sTempRect.y	= 186 ;
	sTempRect.w	= 190;
	sTempRect.h	= 14;
	
	mcp_effect[4] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[4]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[4]->SetBlank( 0, 0 );	

	sTempRect.x	= 30;
	sTempRect.y	= 204;
	sTempRect.w	= 190;
	sTempRect.h	= 14;

	mcp_effect[5] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[5]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[5]->SetBlank( 0, 0 );	

	sTempRect.x	= 30;
	sTempRect.y	= 222;
	sTempRect.w	= 190;
	sTempRect.h	= 14;

	mcp_effect[6] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[6]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[6]->SetBlank( 0, 0 );	

	sTempRect.x	= 115;
	sTempRect.y	= 67;
	sTempRect.w	= 20;
	sTempRect.h	= 14;

	///-- Skill Level
	mcp_Level = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT_LEVEL, mpC_framewnd->GetID(), 
								sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_Level->SetFontR( n_fRsvFontWndStatic );	
	mcp_Level->SetBlank( 0, 0 );		

	///-- ���� ������ ��ư
	mcp_chargeWindow_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, mpC_framewnd->GetID(), 
			84, 202 + AddHeight, 87, 25, false );
	mcp_chargeWindow_btn->SetBtnImage( TID_CTRL_BTN87, TID_CTRL_BTN87_OVR, TID_CTRL_BTN87_CLK, TID_None );			
	mcp_chargeWindow_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_chargeWindow_btn->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );										
	mcp_chargeWindow_btn->SetVisible(false);
	
#if defined( PAYITEM_6 ) && defined( C_PAY_SYSTEM )
	mcp_chargeWindow_btn->SetVisible(true);
#endif

	///-- ���� ������ ��� ������
	mpCharge_framewnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_SUB_ENCHANT, 
		0, enchantwnd.h-2 , enchantwnd.w, 70, false );
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
		70, 14, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpCharge_framewnd->GetID(), 
		110, 14, 35, 35, false );      
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpCharge_framewnd->GetID(), 
		150, 14, 35, 35, false );     
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
	
	mcp_Chargeitem_slot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  mpCharge_framewnd->GetID(), 70, 14, 35, 35, false );
	mcp_Chargeitem_slot[0]->PcItem = &(mc_Chargeslot_item[0]);
	mcp_Chargeitem_slot[0]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[0]->SetHID( HID_ITEMTIP );

	mcp_Chargeitem_slot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  mpCharge_framewnd->GetID(), 110, 14, 35, 35, false );
	mcp_Chargeitem_slot[1]->PcItem = &(mc_Chargeslot_item[1]);
	mcp_Chargeitem_slot[1]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[1]->SetHID( HID_ITEMTIP );

	mcp_Chargeitem_slot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  mpCharge_framewnd->GetID(), 150, 14, 35, 35, false );
	mcp_Chargeitem_slot[2]->PcItem = &(mc_Chargeslot_item[2]);
	mcp_Chargeitem_slot[2]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[2]->SetHID( HID_ITEMTIP );
	
	CloseCharge_Wnd();	

}


void CItemEnchantWndProc::Composition()
{
	#ifdef C_SUBJOB_RENEW
	ComposionNew();	
	#else	
	ComposionOld();
	#endif
}

void CItemEnchantWndProc::OpenCharge_Wnd()
{
	SRect WholeFrameRect;
	SRect C_framewndRect;
	SRect Charge_frameRect;

	mpC_framewnd->GetClientSize(&C_framewndRect);
	mpCharge_framewnd->GetClientSize(&Charge_frameRect);

	WholeFrameRect = C_framewndRect;
	WholeFrameRect.h += (Charge_frameRect.h );  

	m_pWholeFrame->ResetSize(&WholeFrameRect);
	mpC_framewnd->SetWndRepositionR(0, 0);
	mpCharge_framewnd->SetWndRepositionR(0, C_framewndRect.h );

	nRui->SendEvent( mpCharge_framewnd->GetID(), n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );

}

void CItemEnchantWndProc::CloseCharge_Wnd()
{
	SRect WholeFrameRect;
	SRect C_framewndRect;
	
	mpC_framewnd->GetClientSize(&C_framewndRect);
	WholeFrameRect = C_framewndRect;

	m_pWholeFrame->ResetSize(&WholeFrameRect);
	mpC_framewnd->SetWndRepositionR(0, 0);

	//������ �ڽ� �簻�� 
	m_pWholeFrame->SetWndPositionA(WholeFrameRect.x , WholeFrameRect.y);


	nRui->SendEvent( mpCharge_framewnd->GetID(), n_emSetVisible, 
		(EPARAM)false, 0, 0, 0 );

	mcp_Chargeitem_slot[0]->PcItem->Clear();
	mcp_Chargeitem_slot[1]->PcItem->Clear();
	mcp_Chargeitem_slot[2]->PcItem->Clear();
}

///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
///-- �� ���� ���濡 ���� Upgrade ����� �����ش�.
void CItemEnchantWndProc::Update()
{
	static TCHAR tmpStr[128] = {0,};	

	///mcp_elda_slot->PcItem->ItemTable->
	if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE && 
		mcp_elda_slot->PcItem->IType != n_ITYPE_NONE )
	{	
		///-- enchant Text
		EnchantText();

		///-- Level
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), mcp_item_slot->PcItem->ItemTable->theGrade/100 );
		mcp_Level->SetText( tmpStr );

	}
#ifdef C_SUBJOB_RENEW
	else if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE )
	{
		///-- Level
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), mcp_item_slot->PcItem->ItemTable->theGrade/100 );
		mcp_Level->SetText( tmpStr );
		
		int index = (mcp_item_slot->PcItem->ItemTable->theGrade/100)-1;
		if( index < 0 ) index = 0; 
		Rsprintf(RWCHAR(tmpStr) , G_STRING_CHAR(IDS_WND_COMMISION) , g_NeedCost[index] );
		mcp_effect[6]->SetText(tmpStr) ;

	}
#endif 
	else
	{
		//-- Effect 1
		Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );
		mcp_effect[0]->SetText( tmpStr );				
		mcp_effect[1]->SetText( tmpStr );			
		mcp_effect[2]->SetText( tmpStr );
#ifdef MIX_ELDASTONE
		mcp_effect[3]->SetText( tmpStr );
		mcp_effect[4]->SetText( tmpStr );
		mcp_effect[5]->SetText(	tmpStr );
#endif

#ifdef C_SUBJOB_RENEW
		mcp_effect[6]->SetText(	tmpStr );
#endif 

		mcp_Level->SetText( tmpStr );
	}
}


///---------------------------------------------------------------------------
///-- CheckItemList - Inven ���濡 ���� ��þƮ â�� ����
///---------------------------------------------------------------------------
BOOL CItemEnchantWndProc::CheckEnchantItemList()
{
	int i = 0;
	int j = 0;
	
	BOOL lb_isFind_item = FALSE;
	BOOL lb_isFind_elda = FALSE;	
	BOOL lb_isFind_Chargeitem[3] = { FALSE, FALSE, FALSE };	
	
	if( !mcp_item_slot->PcItem && !mcp_elda_slot->PcItem &&
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
		
		///-- item slot
		if( mcp_item_slot->PcItem && 
			mcp_item_slot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_item = true;			
		}

		///-- Elda slot
		if( mcp_elda_slot->PcItem && 
			mcp_elda_slot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_elda = true;
		}

		///-- ���� ������ slot
		for( i = 0; i < 3; i++)
		{
			///-- item slot
			if( mcp_Chargeitem_slot[i]->PcItem && 
				mcp_Chargeitem_slot[i]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
			{
				lb_isFind_Chargeitem[i] = TRUE;

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
	if( !lb_isFind_item )
	{
		mcp_item_slot->PcItem->Clear();
	}
	///-- elda �� ã�� �� ���� ���
	if( !lb_isFind_elda )
	{
		mcp_elda_slot->PcItem->Clear();
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
	
	///-----------------------------------------------------------------------
	///-- �� Slot �� ��� Item �� �ִٸ� ���� �غ�
	if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE && 
		mcp_elda_slot->PcItem->IType != n_ITYPE_NONE )
	{
		mcp_start_btn->SetBtnHold( false );				
	}
	else
	{
		mcp_start_btn->SetBtnHold( true );
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
void CItemEnchantWndProc::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_SUB_ENCHANT_BTN_CLOSE:
	case WID_SUB_ENCHANT_BTN_ABT:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SUB_ENCHANT, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
	case WID_SUB_ENCHANT_SLOT_ELDA:
	case WID_SUB_ENCHANT_SLOT_ITEM:
		{
			if( EMsg->Notify == NM_SLOT_CLICK )			
			{	
				ItemSlotClick( EMsg );		
			}
			else if( EMsg->Notify == NM_SLOT_DBCLICK )	
			{	
				ItemSlotDBClick( EMsg );	
			}
			
			///-- �� Slot �� ��� Item �� �ִٸ� ���� �غ�
			if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE && 
				mcp_elda_slot->PcItem->IType != n_ITYPE_NONE )
			{
				mcp_start_btn->SetBtnHold( false );				
			}
			else
			{
				mcp_start_btn->SetBtnHold( true );
			}
		}
		break;

	case WID_SUB_ENCHANT_BTN_OK:
		if( EMsg->Notify == NM_BTN_CLICK && !mb_button_lock )
		{	
			if( mc_eldaslot_item.IType != n_ITYPE_NONE &&
				mc_itemslot_item.IType != n_ITYPE_NONE )
			{	
				///-- ���� ������ ī��
				for( int i = 0 ; i < 3; ++i )
				{
					if( mc_Chargeslot_item[i].IType == n_ITYPE_NONE )
					{
						m_dPayItem[i] = 0;						
					}
					else
					{						
						m_dPayItem[i] = mc_Chargeslot_item[i].ItemUID;						
					}
				}			
			}
			
			g_RockClient.Send_CsSetStone( mcp_item_slot->PcItem->ItemUID, mcp_elda_slot->PcItem->ItemUID, m_dPayItem );
			mb_button_lock = true;
		}
		break;
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

	if( EMsg->FocusWnd == mcp_Chargeitem_slot[0]->GetID() ||
		EMsg->FocusWnd == mcp_Chargeitem_slot[1]->GetID() ||
		EMsg->FocusWnd == mcp_Chargeitem_slot[2]->GetID() )
	{	
		if( EMsg->Notify == NM_SLOT_CLICK )
		{
			ItemSlotClick( EMsg );
		}
		else if( EMsg->Notify == NM_SLOT_DBCLICK )
		{
			ItemSlotDBClick( EMsg );
		}
	}
}

///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item ����
void CItemEnchantWndProc::ItemSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();	
	
	if( tslot->PcItem )
	{
		tslot->PcItem->Clear();		
	}

	return;
}
///---------------------------------------------------------------------------
///-- ItemSlotClick
///---------------------------------------------------------------------------
///-- Item �ø���
///-- !!!! ������ ������ ������ Hard Coding
void CItemEnchantWndProc::ItemSlotClick( SEventMessage* EMsg )
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
		
		if( srcslot->PcItem->ItemTable == NULL )
		{
			return;
		}
		
		if( targetslot->PcItem && (targetslot->PcItem == srcslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		
		///-------------------------------------------------------------------
		///-- Item Slot
		if( targetslot == mcp_item_slot )
		{
			if(  srcslot->Group == n_SGROUP_INVENTORY   )
			{
#ifdef C_SUBJOB_RENEW
				bool bIsEnchantItemSubJob = IsEnchantItemSubJobNew(srcslot);
				
				if(!bIsEnchantItemSubJob)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOTENCHANT_THEITEM) );
					return;
				}
	
#else 
				///-- ���� üũ 
				if( (srcslot->PcItem->Code < c_ItemCode_Cash_Min ) && mi_skill_level < (int)(srcslot->PcItem->ItemTable->theGrade)/100 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVER_ITEM_LEVEL ) );	
					return;
				}				
				
				///-- By simwoosung �� �κп��� ���� ũ����Ʈ�� 
				///-- �Ƹ� ũ����Ʈ ���� ���� ������ Ŭ���� ���� ��ƾ �߰�

				bool bIsEnchantItemSubJob = IsEnchantItemSubJob(srcslot);
				
				if(!bIsEnchantItemSubJob)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOTENCHANT_THEITEM) );
					return;
				}
#endif 
				///-- Copy item
				targetslot->SetSlotPcItem( *(srcslot->PcItem) );								
				///-- Del Cursor
				//targetslot->PcItem = srcslot->PcItem;
				nRui->ResetCursorItem();					
			}
			else if ( srcslot->Group == n_SGROUP_WEAR)
			{				
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_ENCHANT_WEAR ) );			
			}
		}
		///-------------------------------------------------------------------
		///-- elda Slot
		else if( targetslot == mcp_elda_slot )
		{
			if( ( srcslot->Group == n_SGROUP_INVENTORY ) && ( srcslot->PcItem->Class == n_Class_Eldastone ) )
			{	
				

#ifdef C_SUBJOB_RENEW
				if( mcp_item_slot->PcItem->IType == n_ITYPE_NONE ) 
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FIRSTITEM_WEAPONS_OR_ARMOR)); 
					return;
				}
#endif 

				if(	srcslot->PcItem->ItemTable->theSetID == 0 && srcslot->PcItem->Code < 60000)
				{
					SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_CANT_USE_ITEM));
					return;
				}

#ifdef C_SUBJOB_RENEW
				bool bIsEnchantEldaSubJob = IsEnchantEldaSubJobNew(srcslot);
				
				if(!bIsEnchantEldaSubJob)
				{
					return;
				}				
				
#else 
				bool bIsEnchantEldaSubJob = IsEnchantEldaSubJob(srcslot);
				
				if(!bIsEnchantEldaSubJob)
				{
					return;
				}				
#endif 	
				///-- Copy item
				targetslot->SetSlotPcItem( *(srcslot->PcItem) );	
				targetslot->PcItem->Amount = 1;
				//targetslot->PcItem = srcslot->PcItem;
				///-- Del Cursor
				nRui->ResetCursorItem();
			}
			else 
			{
				SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_NOT_ELDA_ITEM));	
			}
		
  
		}
		//-- ���� ������ Slot
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

				///-- ��æƮ ���� ��ũ���� �ƴϸ�
				if( ( n_Scroll_SetStoneInsurance != nScrollType ) && 
					( n_Scroll_SetStoneBonus != nScrollType ) )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ) );
					return;
				}
				
				
				//���� ������ �������� �ִ��� �˻��Ѵ�..
				bool bFindSameItem = false; 
				
				for(int i=0; i< 3; ++i)
				{
					if( mcp_Chargeitem_slot[i]->PcItem->IType != n_ITYPE_NONE )
					{
						if( nScrollType == mcp_Chargeitem_slot[i]->PcItem->ItemTable->theReHP)
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERRAPEDITEMEFFECT ) );
							return;
						}
					}
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
bool CItemEnchantWndProc::IsEnchantItemSubJobNew(CSlot * pSlot)
{

	if(NULL == pSlot || NULL == pSlot->PcItem || NULL == pSlot->PcItem->ItemTable )
	{
		return false;
	}	

	//�̾������� �������� Ȯ��..
	BYTE aItemLimitFunc = pSlot->PcItem->ItemTable->theItemLimitFunc;
	if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotSetStone ) )
	{
		return false;
	}
	
	if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotUpgradeSetStome ) )
	{
		return false;
	}
	

	bool bIsEnchantItemSubJob = false;

	BYTE aType = pSlot->PcItem->ItemTable->theType;

	switch( aType )
	{
		case n_Type_ChangeHair:
		case n_Type_KeepupHair:
		case n_Type_Wing:
		case n_Type_Shield:
		case n_Type_Riding_Head:
		case n_Type_Riding_Saddle:
		case n_Type_Riding_Leg:
		case n_Type_Riding_Full:
		{
			return false;
		}
	}

	///-- ������ �ƴϰ� ���뱹���� Ʋ����
	if( ( pSlot->PcItem->ItemTable->theCitizen != 2 ) && 
		( nRui->GetPcParamInfo()->Citizen != pSlot->PcItem->ItemTable->theCitizen ) )
	{
		return false;
	}

	///-- ���� �������̶��
	WORD wCode = pSlot->PcItem->ItemTable->theCode;
	if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max ) )
	{
		BOOL IsWeapon = FALSE;
		BOOL IsCanEnchant = FALSE;
			
		switch( pSlot->PcItem->ItemTable->theType )
		{
			case n_Type_Sword:								///-- ��			
			case n_Type_TwohandSword:						///-- ���
			case n_Type_Bow:								///-- Ȱ
			case n_Type_MagicBook:							///-- ������
			case n_Type_Staff:								///-- ������
			case n_Type_CrystalBall:						///-- ������
				{
					IsWeapon = TRUE;
					IsCanEnchant = TRUE;
				}
				break;
			case n_Type_LightGlove:	 
			case n_Type_LightShoes:
			case n_Type_MiddleCoat:							///-- �з��� �Ƹ� �����
			case n_Type_MiddlePants:	
			case n_Type_MiddleGlove:	 
			case n_Type_MiddleShoes:
			case n_Type_HeavyCoat:							///-- �з��� �Ƹ� ������
			case n_Type_HeavyPants:	
			case n_Type_HeavyGlove:	 
			case n_Type_HeavyShoes:
			case n_Type_MagicPants:
			case n_Type_MagicGlove:	 
			case n_Type_MagicShoes:
			case n_Type_PriestCoat:							///-- ���� ������
			case n_Type_PriestPants:	
			case n_Type_PriestGlove:	 
			case n_Type_PriestShoes:
			case n_Type_SpiritCoat:							///-- ���� ���ź�
			case n_Type_SpiritPants:	
			case n_Type_SpiritGlove:	 
			case n_Type_SpiritShoes:
			case n_Type_SetCloth:
				{
					IsCanEnchant = TRUE;
				}
				break;
		}

		if( !IsCanEnchant )
		{
			return false;
		}		
		
		if( mcp_elda_slot->PcItem->ItemTable )
		{
			SItemBaseInfo * pItemBaseInfo = mcp_elda_slot->PcItem->ItemTable;
			
			if(IsWeapon)
			{
				//���� ũ����Ʈ�� ��� - ���Ӽ��� ���ٽ��� ���� �Ұ�
				if(  pItemBaseInfo->theAttackDef  ||  pItemBaseInfo->theMagicRes )	
				//���ֻ�(�������) ���λ�(�������)
				{
					return false;
				}
			}
			else
			{
				/*
				///- ��
				if( nRui->GetPcParamInfo()->Citizen == MILLENA )
				{
					///-- �з��� �� ��Ȳ�� ���Ұ�
					if(  pItemBaseInfo->thePower )
					{
						return false;
					}
				}
				else
				{
					///-- ���� �� ����� ���Ұ�
					if(  pItemBaseInfo->theSympathy )
					{
						return false;
					}
				}				
				*/
				//�Ƹ� ũ����Ʈ�� ��� - ���ݼӼ��� ���ٽ��� ���� �Ұ�
				if(  pItemBaseInfo->theMinAttackBonus || pItemBaseInfo->theMaxAttackBonus ||
					 pItemBaseInfo->theMinMagicBonus || pItemBaseInfo->theMaxMagicBonus )	
				//Ǫ����(�������ݷ�) ������(�������ݷ�)
				{
					return false;
				}
			}	
		}

		return true;
	}				
	
	if( nRui->GetPcParamInfo()->Citizen == MILLENA )
	{
		switch(pSlot->PcItem->ItemTable->theType)
		{
		case n_Type_Sword:								///-- ��			
		case n_Type_TwohandSword:						///-- ���
		case n_Type_Bow:								///-- Ȱ
			{
				mb_craft_weapon = true; 
				bIsEnchantItemSubJob = true;
			
			}
			break; 
			//////////////////////////////////////////////////////////////////////////
		case n_Type_LightCoat:							///-- �з��� �Ƹ� �����
		case n_Type_LightPants:	
		case n_Type_LightGlove:	 
		case n_Type_LightShoes:
		case n_Type_MiddleCoat:							///-- �з��� �Ƹ� �����
		case n_Type_MiddlePants:	
		case n_Type_MiddleGlove:	 
		case n_Type_MiddleShoes:
		case n_Type_HeavyCoat:							///-- �з��� �Ƹ� ������
		case n_Type_HeavyPants:	
		case n_Type_HeavyGlove:	 
		case n_Type_HeavyShoes:
			//	case n_Type_Shield:								///-- ����
		case n_Type_SetCloth:	
			{
				mb_craft_weapon = false; 
				bIsEnchantItemSubJob = true;
			}
			break;
		}	
		
	}
	else
	{

		switch(pSlot->PcItem->ItemTable->theType)
		{
			case n_Type_MagicBook:							///-- ������
			case n_Type_Staff:								///-- ������
			case n_Type_CrystalBall:						///-- ������
				{
					mb_craft_weapon = true; 
					bIsEnchantItemSubJob = true;
					
				}
				break; 
//////////////////////////////////////////////////////////////////////////
			case n_Type_MagicCoat:							///-- ���� ������
			case n_Type_MagicPants:
			case n_Type_MagicGlove:	 
			case n_Type_MagicShoes:
			case n_Type_PriestCoat:							///-- ���� ������
			case n_Type_PriestPants:	
			case n_Type_PriestGlove:	 
			case n_Type_PriestShoes:
			case n_Type_SpiritCoat:							///-- ���� ���ź�
			case n_Type_SpiritPants:	
			case n_Type_SpiritGlove:	 
			case n_Type_SpiritShoes:
			case n_Type_SetCloth:
				{
					mb_craft_weapon = false;  
					bIsEnchantItemSubJob = true;
				}
				break;
		}
		
	}

	return bIsEnchantItemSubJob;

}

bool CItemEnchantWndProc::IsEnchantItemSubJob(CSlot * pSlot)
{
	if(NULL == pSlot || NULL == pSlot->PcItem || NULL == pSlot->PcItem->ItemTable )
	{
		return false;
	}	
	//�̾������� �������� Ȯ��..
	BYTE aItemLimitFunc = pSlot->PcItem->ItemTable->theItemLimitFunc;
	if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotSetStone ) )
	{
		return false;
	}

	bool bIsEnchantItemSubJob = false;

	BYTE aType = pSlot->PcItem->ItemTable->theType;
	switch( aType )
	{
		case n_Type_ChangeHair:
		case n_Type_KeepupHair:
		case n_Type_Wing:
		case n_Type_Shield:
		case n_Type_Riding_Head:
		case n_Type_Riding_Saddle:
		case n_Type_Riding_Leg:
		case n_Type_Riding_Full:
		{
			return false;
		}
	}

	///-- ������ �ƴϰ� ���뱹���� Ʋ����
	if( ( pSlot->PcItem->ItemTable->theCitizen != 2 ) && 
		( nRui->GetPcParamInfo()->Citizen != pSlot->PcItem->ItemTable->theCitizen ) )
	{
		return false;
	}

	///-- ���� �������̶��
	WORD wCode = pSlot->PcItem->ItemTable->theCode;
	if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max ) )
	{
		BOOL IsWeapon = FALSE;
		BOOL IsCanEnchant = FALSE;
			
		switch( pSlot->PcItem->ItemTable->theType )
		{
			case n_Type_Sword:								///-- ��			
			case n_Type_TwohandSword:						///-- ���
			case n_Type_Bow:								///-- Ȱ
			case n_Type_MagicBook:							///-- ������
			case n_Type_Staff:								///-- ������
			case n_Type_CrystalBall:						///-- ������
				{
					IsWeapon = TRUE;
					IsCanEnchant = TRUE;
				}
				break;
			case n_Type_LightGlove:	 
			case n_Type_LightShoes:
			case n_Type_MiddleCoat:							///-- �з��� �Ƹ� �����
			case n_Type_MiddlePants:	
			case n_Type_MiddleGlove:	 
			case n_Type_MiddleShoes:
			case n_Type_HeavyCoat:							///-- �з��� �Ƹ� ������
			case n_Type_HeavyPants:	
			case n_Type_HeavyGlove:	 
			case n_Type_HeavyShoes:
			case n_Type_MagicPants:
			case n_Type_MagicGlove:	 
			case n_Type_MagicShoes:
			case n_Type_PriestCoat:							///-- ���� ������
			case n_Type_PriestPants:	
			case n_Type_PriestGlove:	 
			case n_Type_PriestShoes:
			case n_Type_SpiritCoat:							///-- ���� ���ź�
			case n_Type_SpiritPants:	
			case n_Type_SpiritGlove:	 
			case n_Type_SpiritShoes:
			case n_Type_SetCloth:
				{
					IsCanEnchant = TRUE;
				}
				break;
		}

		if( !IsCanEnchant )
		{
			return false;
		}		
		
		if( mcp_elda_slot->PcItem->ItemTable )
		{
			SItemBaseInfo * pItemBaseInfo = mcp_elda_slot->PcItem->ItemTable;
			
			if(IsWeapon)
			{
				//���� ũ����Ʈ�� ��� - ���Ӽ��� ���ٽ��� ���� �Ұ�
				if(  pItemBaseInfo->theAttackDef  ||  pItemBaseInfo->theMagicRes )	
				//���ֻ�(�������) ���λ�(�������)
				{
					return false;
				}
			}
			else
			{
				/*
				///- ��
				if( nRui->GetPcParamInfo()->Citizen == MILLENA )
				{
					///-- �з��� �� ��Ȳ�� ���Ұ�
					if(  pItemBaseInfo->thePower )
					{
						return false;
					}
				}
				else
				{
					///-- ���� �� ����� ���Ұ�
					if(  pItemBaseInfo->theSympathy )
					{
						return false;
					}
				}				
				*/
				//�Ƹ� ũ����Ʈ�� ��� - ���ݼӼ��� ���ٽ��� ���� �Ұ�
				if(  pItemBaseInfo->theMinAttackBonus || pItemBaseInfo->theMaxAttackBonus ||
					 pItemBaseInfo->theMinMagicBonus || pItemBaseInfo->theMaxMagicBonus )	
				//Ǫ����(�������ݷ�) ������(�������ݷ�)
				{
					return false;
				}
			}	
		}

		return true;
	}				
	
	if( nRui->GetPcParamInfo()->Citizen == MILLENA )
	{
		if( mb_craft_weapon )
		{
			switch(pSlot->PcItem->ItemTable->theType)
			{
				case n_Type_Sword:								///-- ��			
				case n_Type_TwohandSword:						///-- ���
				case n_Type_Bow:								///-- Ȱ
					{
						bIsEnchantItemSubJob = true;					
					}
					break;
			}
		}
		else
		{
			switch(pSlot->PcItem->ItemTable->theType)
			{
				case n_Type_LightCoat:							///-- �з��� �Ƹ� �����
				case n_Type_LightPants:	
				case n_Type_LightGlove:	 
				case n_Type_LightShoes:
				case n_Type_MiddleCoat:							///-- �з��� �Ƹ� �����
				case n_Type_MiddlePants:	
				case n_Type_MiddleGlove:	 
				case n_Type_MiddleShoes:
				case n_Type_HeavyCoat:							///-- �з��� �Ƹ� ������
				case n_Type_HeavyPants:	
				case n_Type_HeavyGlove:	 
				case n_Type_HeavyShoes:
			//	case n_Type_Shield:								///-- ����
				case n_Type_SetCloth:	
					{
						bIsEnchantItemSubJob = true;
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
				case n_Type_MagicBook:							///-- ������
				case n_Type_Staff:								///-- ������
				case n_Type_CrystalBall:						///-- ������
					{
						bIsEnchantItemSubJob = true;
					}
					break;
			}			
		}
		else
		{
			switch(pSlot->PcItem->ItemTable->theType)
			{
				case n_Type_MagicCoat:							///-- ���� ������
				case n_Type_MagicPants:
				case n_Type_MagicGlove:	 
				case n_Type_MagicShoes:
				case n_Type_PriestCoat:							///-- ���� ������
				case n_Type_PriestPants:	
				case n_Type_PriestGlove:	 
				case n_Type_PriestShoes:
				case n_Type_SpiritCoat:							///-- ���� ���ź�
				case n_Type_SpiritPants:	
				case n_Type_SpiritGlove:	 
				case n_Type_SpiritShoes:
				case n_Type_SetCloth:
					{
						bIsEnchantItemSubJob = true;
					}
					break;
			}
		}
	}

	return bIsEnchantItemSubJob;
}

bool CItemEnchantWndProc::IsEnchantEldaSubJobNew(CSlot * pSlot)
{
	if(NULL == pSlot)
	{
		return false;
	}	
	
	bool bIsEnchantEldaSubJob = true;
	SItemBaseInfo * pItemBaseInfo = NULL;
	SItemBaseInfo * pItemBaseInfoTemp = NULL;

	
	if(pSlot->PcItem->Code > 60000 )		
	{
		pItemBaseInfoTemp = pSlot->PcItem->ItemTable;

		pItemBaseInfo  =  g_Pc_Manager.GetItemBaseInfoPtr( pItemBaseInfoTemp->theAttackSuccSnd3 );
		
		if( pItemBaseInfo == NULL) 
		{
			pItemBaseInfo = pSlot->PcItem->ItemTable;
		}

	}
	else 
	{
		pItemBaseInfo = pSlot->PcItem->ItemTable;

	}
/*
		
	if( pItemBaseInfo )
	{
		if( pItemBaseInfo->theCode > 60000)
		{
			pItemBaseInfo->theAttackSuccSnd3			

		}
	}
	

*/


	if( pItemBaseInfo )
	{
		if( nRui->GetPcParamInfo()->Citizen == MILLENA )
		{
			///-- �з��� ����� ���� ���Ұ�
			if( pItemBaseInfo->theSympathy )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
			
			///-- �з��� ����� ���� ���Ұ�
			if( pItemBaseInfo->theInt )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
		}
		else
		{
			///-- ���� ��Ȳ�� ���� ���Ұ�
			if( pItemBaseInfo->thePower )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
			
			///-- ���� ���� ���� ���Ұ�
			if( pItemBaseInfo->theStamina )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
		}
	}
	
	if( pItemBaseInfo && mb_craft_weapon )
	{
		if( nRui->GetPcParamInfo()->Citizen == MILLENA )
		{
			///-- �з��� ������ Ǫ���� ���� ���Ұ�
			if( pItemBaseInfo->theMinMagicBonus || pItemBaseInfo->theMaxMagicBonus )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ));
				return false;
			}
		}
		else
		{
			///-- ���� ������ ������ ���� ���Ұ�
			if( pItemBaseInfo->theMinAttackBonus || pItemBaseInfo->theMaxAttackBonus )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ));
				return false;
			}
		}
	}

	if( mcp_item_slot->PcItem->ItemTable )
	{
		///-- ���� �������� ������ ���Կ� �ö� ������
		WORD wCode = mcp_item_slot->PcItem->ItemTable->theCode;
		if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max) )
		{
			BOOL IsWeapon = FALSE;

			switch( mcp_item_slot->PcItem->ItemTable->theType )
			{
				case n_Type_Sword:								///-- ��			
				case n_Type_TwohandSword:						///-- ���
				case n_Type_Bow:								///-- Ȱ
				case n_Type_MagicBook:							///-- ������
				case n_Type_Staff:								///-- ������
				case n_Type_CrystalBall:						///-- ������
					{
						IsWeapon = TRUE;	
					}
			}

			if(IsWeapon)
			{
				//���� ũ����Ʈ�� ��� - ���Ӽ��� ���ٽ��� ���� �Ұ�
				if(  pItemBaseInfo->theAttackDef  ||  pItemBaseInfo->theMagicRes )	
				//���ֻ�(�������) ���λ�(�������)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ));
					return false;
				}
			}
			else
			{
				/*
				///- ��
				if( nRui->GetPcParamInfo()->Citizen == MILLENA )
				{
					///-- �з��� �� ��Ȳ�� ���Ұ�
					if(  pItemBaseInfo->thePower )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
						return false;
					}
				}
				else
				{
					///-- ���� �� ����� ���Ұ�
					if(  pItemBaseInfo->theSympathy )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
						return false;
					}
				}				
				*/

				//�Ƹ� ũ����Ʈ�� ��� - ���ݼӼ��� ���ٽ��� ���� �Ұ�
				if(  pItemBaseInfo->theMinAttackBonus || pItemBaseInfo->theMaxAttackBonus ||
					 pItemBaseInfo->theMinMagicBonus || pItemBaseInfo->theMaxMagicBonus )	
				//Ǫ����(�������ݷ�) ������(�������ݷ�)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_ARMOR ));
					return false;
				}
			}
			
			return true;
		}
	}

	if(mb_craft_weapon)
	{
		//���� ũ����Ʈ�� ��� - ���Ӽ��� ���ٽ��� ���� �Ұ�
		if(  pItemBaseInfo->theAttackDef  ||  pItemBaseInfo->theMagicRes )	
		//���ֻ�(�������) ���λ�(�������)
		{
			bIsEnchantEldaSubJob = false;
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ));	   
		}
	}
	else
	{
		/*
		///- ��
		if( nRui->GetPcParamInfo()->Citizen == MILLENA )
		{
			///-- �з��� �� ��Ȳ�� ���Ұ�
			if(  pItemBaseInfo->thePower )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
		}
		else
		{
			///-- ���� �� ����� ���Ұ�
			if(  pItemBaseInfo->theSympathy )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
		}		
		*/
		//�Ƹ� ũ����Ʈ�� ��� - ���ݼӼ��� ���ٽ��� ���� �Ұ�
		if(  pItemBaseInfo->theMinAttackBonus || pItemBaseInfo->theMaxAttackBonus ||
			 pItemBaseInfo->theMinMagicBonus || pItemBaseInfo->theMaxMagicBonus )	
		//Ǫ����(�������ݷ�) ������(�������ݷ�)
		{
			bIsEnchantEldaSubJob = false;			
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_ARMOR ));	
		}
	}

	return bIsEnchantEldaSubJob;



}

bool CItemEnchantWndProc::IsEnchantEldaSubJob(CSlot * pSlot)
{
	if(NULL == pSlot)
	{
		return false;
	}	
	
	bool bIsEnchantEldaSubJob = true;
	SItemBaseInfo * pItemBaseInfo = NULL;
	SItemBaseInfo * pItemBaseInfoTemp = NULL;

	
	if(pSlot->PcItem->Code > 60000 )		
	{
		pItemBaseInfoTemp = pSlot->PcItem->ItemTable;

		pItemBaseInfo  =  g_Pc_Manager.GetItemBaseInfoPtr( pItemBaseInfoTemp->theAttackSuccSnd3 );
		
		if( pItemBaseInfo == NULL) 
		{
			pItemBaseInfo = pSlot->PcItem->ItemTable;
		}

	}
	else 
	{
		pItemBaseInfo = pSlot->PcItem->ItemTable;

	}
/*
		
	if( pItemBaseInfo )
	{
		if( pItemBaseInfo->theCode > 60000)
		{
			pItemBaseInfo->theAttackSuccSnd3			

		}
	}
	

*/


	if( pItemBaseInfo )
	{
		if( nRui->GetPcParamInfo()->Citizen == MILLENA )
		{
			///-- �з��� ����� ���� ���Ұ�
			if( pItemBaseInfo->theSympathy )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
			
			///-- �з��� ����� ���� ���Ұ�
			if( pItemBaseInfo->theInt )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
		}
		else
		{
			///-- ���� ��Ȳ�� ���� ���Ұ�
			if( pItemBaseInfo->thePower )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
			
			///-- ���� ���� ���� ���Ұ�
			if( pItemBaseInfo->theStamina )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
		}
	}
	
	if( pItemBaseInfo && mb_craft_weapon )
	{
		if( nRui->GetPcParamInfo()->Citizen == MILLENA )
		{
			///-- �з��� ������ Ǫ���� ���� ���Ұ�
			if( pItemBaseInfo->theMinMagicBonus || pItemBaseInfo->theMaxMagicBonus )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ));
				return false;
			}
		}
		else
		{
			///-- ���� ������ ������ ���� ���Ұ�
			if( pItemBaseInfo->theMinAttackBonus || pItemBaseInfo->theMaxAttackBonus )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ));
				return false;
			}
		}
	}

	if( mcp_item_slot->PcItem->ItemTable )
	{
		///-- ���� �������� ������ ���Կ� �ö� ������
		WORD wCode = mcp_item_slot->PcItem->ItemTable->theCode;
		if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max) )
		{
			BOOL IsWeapon = FALSE;

			switch( mcp_item_slot->PcItem->ItemTable->theType )
			{
				case n_Type_Sword:								///-- ��			
				case n_Type_TwohandSword:						///-- ���
				case n_Type_Bow:								///-- Ȱ
				case n_Type_MagicBook:							///-- ������
				case n_Type_Staff:								///-- ������
				case n_Type_CrystalBall:						///-- ������
					{
						IsWeapon = TRUE;	
					}
			}

			if(IsWeapon)
			{
				//���� ũ����Ʈ�� ��� - ���Ӽ��� ���ٽ��� ���� �Ұ�
				if(  pItemBaseInfo->theAttackDef  ||  pItemBaseInfo->theMagicRes )	
				//���ֻ�(�������) ���λ�(�������)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ));
					return false;
				}
			}
			else
			{
				/*
				///- ��
				if( nRui->GetPcParamInfo()->Citizen == MILLENA )
				{
					///-- �з��� �� ��Ȳ�� ���Ұ�
					if(  pItemBaseInfo->thePower )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
						return false;
					}
				}
				else
				{
					///-- ���� �� ����� ���Ұ�
					if(  pItemBaseInfo->theSympathy )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
						return false;
					}
				}				
				*/

				//�Ƹ� ũ����Ʈ�� ��� - ���ݼӼ��� ���ٽ��� ���� �Ұ�
				if(  pItemBaseInfo->theMinAttackBonus || pItemBaseInfo->theMaxAttackBonus ||
					 pItemBaseInfo->theMinMagicBonus || pItemBaseInfo->theMaxMagicBonus )	
				//Ǫ����(�������ݷ�) ������(�������ݷ�)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_ARMOR ));
					return false;
				}
			}
			
			return true;
		}
	}

	if(mb_craft_weapon)
	{
		//���� ũ����Ʈ�� ��� - ���Ӽ��� ���ٽ��� ���� �Ұ�
		if(  pItemBaseInfo->theAttackDef  ||  pItemBaseInfo->theMagicRes )	
		//���ֻ�(�������) ���λ�(�������)
		{
			bIsEnchantEldaSubJob = false;
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ));	   
		}
	}
	else
	{
		/*
		///- ��
		if( nRui->GetPcParamInfo()->Citizen == MILLENA )
		{
			///-- �з��� �� ��Ȳ�� ���Ұ�
			if(  pItemBaseInfo->thePower )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
		}
		else
		{
			///-- ���� �� ����� ���Ұ�
			if(  pItemBaseInfo->theSympathy )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ));
				return false;
			}
		}		
		*/
		//�Ƹ� ũ����Ʈ�� ��� - ���ݼӼ��� ���ٽ��� ���� �Ұ�
		if(  pItemBaseInfo->theMinAttackBonus || pItemBaseInfo->theMaxAttackBonus ||
			 pItemBaseInfo->theMinMagicBonus || pItemBaseInfo->theMaxMagicBonus )	
		//Ǫ����(�������ݷ�) ������(�������ݷ�)
		{
			bIsEnchantEldaSubJob = false;			
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_ARMOR ));	
		}
	}

	return bIsEnchantEldaSubJob;
}

///---------------------------------------------------------------------------
///-- EnchantText
///---------------------------------------------------------------------------
void CItemEnchantWndProc::EnchantText()
{
	int li_text_idx = 0;
	TCHAR tmpStr[128] = {0,};
	
	///-- Clear Text
	Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );

	int MaxCount = 3;
#ifdef MIX_ELDASTONE
	MaxCount = 6;
#endif

	for(int i =0 ; i < MaxCount ; ++i)
	{
		mcp_effect[i]->SetText( tmpStr );
	}


	///-- ���� ���ݷ�
	if( mc_eldaslot_item.ItemTable->theMaxAttackBonus )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ) , mc_eldaslot_item.ItemTable->theMaxAttackBonus );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > MaxCount ) 
		{
			return;
		}
	}
	///-- ���� ���ݷ�
	if( mc_eldaslot_item.ItemTable->theMaxMagicBonus )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ), mc_eldaslot_item.ItemTable->theMaxMagicBonus );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > MaxCount ) 
		{
			return;
		}
	}
	///-- ���� ����
	if( mc_eldaslot_item.ItemTable->theAttackDef )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ), mc_eldaslot_item.ItemTable->theAttackDef );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > MaxCount ) 
		{
			return;
		}
	}

	///-- ���� ����
	if( mc_eldaslot_item.ItemTable->theMagicRes )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ), mc_eldaslot_item.ItemTable->theMagicRes );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > MaxCount ) 
		{
			return;
		}
	}
	
	///-- ������ ����� ȿ�� 
	///-- �Ŀ�
	if( mc_eldaslot_item.ItemTable->thePower )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_POWER ), (int)mc_eldaslot_item.ItemTable->thePower );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > MaxCount ) 
		{
			return;
		}
	}
	
	///-- ü��
	if( mc_eldaslot_item.ItemTable->theVital )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_VITAL ), (int)mc_eldaslot_item.ItemTable->theVital );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > MaxCount ) 
		{
			return;
		}
	}
	///-- ���� 
	if( mc_eldaslot_item.ItemTable->theSympathy )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_SYMPATHY ), (int)mc_eldaslot_item.ItemTable->theSympathy );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > MaxCount ) 
		{
			return;
		}
	}
	///-- ����
	if( mc_eldaslot_item.ItemTable->theInt )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_INTELLI ), (int)mc_eldaslot_item.ItemTable->theInt );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > MaxCount ) 
		{
			return;
		}
	}
	///-- ������
	if( mc_eldaslot_item.ItemTable->theStamina )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_STAMINA ), (int)mc_eldaslot_item.ItemTable->theStamina );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > MaxCount ) 
		{
			return;
		}
	}
	///-- ��ø
	if( mc_eldaslot_item.ItemTable->theDex )
	{
		Rsprintf( RWCHAR(tmpStr), _RT("%s +%d"), G_STRING_CHAR( IDS_CHARSTATE_DEX ), (int)mc_eldaslot_item.ItemTable->theDex );
		mcp_effect[li_text_idx]->SetText( tmpStr );	
		if( ++li_text_idx > MaxCount ) 
		{
			return;
		}
	}
				
				
}


void CItemEnchantWndProc::ComposionOld()
{
	CFrameWnd*		fw = NULL;
	CImageBox*		img;
	CButton*		btn;
	CTextBox*		tbox;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;

	enchantwnd.w	= 204;
	enchantwnd.h	= 200;

	int AddHeight = 36 + 18 ; 

#if defined( PAYITEM_6 ) && defined( C_PAY_SYSTEM )
	
	enchantwnd.w	= 254;
	enchantwnd.h	= 235 + AddHeight ;

#endif

	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	


	///-- Frame Wnd
	m_pWholeFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SUB_ENCHANT, WID_None, 
									  enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );	
	m_pWholeFrame->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pWholeFrame->SetClientImage( TID_CTRL_WND_L1 );
	m_pWholeFrame->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pWholeFrame->SetWndTileSizeLR( 40, 29 );
	m_pWholeFrame->SetWndTileSizeTB( 31, 11 );

	m_pWholeFrame->SetCaption( G_STRING_CHAR(IDS_WND_ENCHANT) );							
	m_pWholeFrame->SetTitle( true );	
	m_pWholeFrame->SetWndProc( this );
	m_pWholeFrame->SetVisible( false );
	m_pWholeFrame->Initialize();


	///-- Frame Wnd
	mpC_framewnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None , WID_SUB_ENCHANT, 
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

	SRect sTempRect;

	sTempRect.x	= 1;
	sTempRect.y	= 111;
	sTempRect.w	= 252;
	sTempRect.h	= 55 + AddHeight ;

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	sTempRect.x	= 1;
	sTempRect.y	= 110;
	sTempRect.w	= 252;
	sTempRect.h	= 1;


	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(),  
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	sTempRect.x	= 1;
	sTempRect.y	= 166 + AddHeight;
	sTempRect.w	= 252;
	sTempRect.h	= 1;


	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );	
	
	sTempRect.x	= 58;
	sTempRect.y	= 56;
	sTempRect.w	= 35;
	sTempRect.h	= 35;

	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	sTempRect.x	= 157;
	sTempRect.y	= 56;
	sTempRect.w	= 35;
	sTempRect.h	= 35;


	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );            
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	sTempRect.x	= 115;
	sTempRect.y	= 51;
	sTempRect.w	= 21;
	sTempRect.h	= 35;


	///-- Level
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );                
	img ->SetClientImage( TID_C_WND_ELDA_SB_LV );

	sTempRect.x	= 37;
	sTempRect.y	= 36;
	sTempRect.w	= 80;
	sTempRect.h	= 17;
	
	///-- ������( Static Text )
	mcp_Item = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	mcp_Item->SetFontR( n_fRsvFontWndStatic );
	mcp_Item->SetText( G_STRING_CHAR( IDS_ITEM ) );

	sTempRect.x	= 135;
	sTempRect.y	= 36;
	sTempRect.w	= 80;
	sTempRect.h	= 17;

	///-- ���ٽ���( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR( IDS_ELDASTONE ));

	sTempRect.x	= 70;
	sTempRect.y	= 95;
	sTempRect.w	= 110;
	sTempRect.h	= 17;

	
	///-- �ռ�ȿ��  ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR( IDS_WND_ENCHANT_EFFECT ) );
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- ���� ��ư

	sTempRect.x	= 235;
	sTempRect.y	= 6;
	sTempRect.w	= 13;
	sTempRect.h	= 13;

	///-- �ݱ�
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_ENCHANT_BTN_CLOSE, mpC_framewnd->GetID(), 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	sTempRect.x	= 56;
	sTempRect.y	= 171 + AddHeight ;
	sTempRect.w	= 58;
	sTempRect.h	= 25;


	mcp_start_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_ENCHANT_BTN_OK, mpC_framewnd->GetID(), 
											   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	mcp_start_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	mcp_start_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_start_btn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) ); 
	mcp_start_btn->SetBtnHold( true );

	sTempRect.x	= 144;
	sTempRect.y	= 171 + AddHeight;
	sTempRect.w	= 58;
	sTempRect.h	= 25;

	///-- �ߴ�
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_SUB_ENCHANT_BTN_ABT, mpC_framewnd->GetID(), 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );	
	btn->SetFontR( n_fRsvFontWndButton ); 
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 

	sTempRect.x	= 58;
	sTempRect.y	= 56;
	sTempRect.w	= 35;
	sTempRect.h	= 35;

		
	///-- ������ Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_ENCHANT_SLOT_ITEM, mpC_framewnd->GetID(), 
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_item_slot->SetSlotGroup( n_SGROUP_TUNING );

	sTempRect.x	= 157;
	sTempRect.y	= 56;
	sTempRect.w	= 35;
	sTempRect.h	= 35;

	///-- Elda Slot
	mcp_elda_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_SUB_ENCHANT_SLOT_ELDA, mpC_framewnd->GetID(), 
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	mcp_elda_slot->SetSlotGroup( n_SGROUP_TUNING );

	///-- None Item ����
	mcp_item_slot->PcItem = &mc_itemslot_item;		
	mcp_elda_slot->PcItem = &mc_eldaslot_item;


	sTempRect.x	= 30;
	sTempRect.y	= 114;
	sTempRect.w	= 190;
	sTempRect.h	= 14;

	mcp_effect[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT1, mpC_framewnd->GetID(), 
												sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[0]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[0]->SetBlank( 0, 0 );

	sTempRect.x	= 30;
	sTempRect.y	= 132;
	sTempRect.w	= 190;
	sTempRect.h	= 14;

	mcp_effect[1]= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT2, mpC_framewnd->GetID(), 
											   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	mcp_effect[1]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[1]->SetBlank( 0, 0 );	

	sTempRect.x	= 30;
	sTempRect.y	= 150;
	sTempRect.w	= 190;
	sTempRect.h	= 14;

	mcp_effect[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT3, mpC_framewnd->GetID(), 
												sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[2]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[2]->SetBlank( 0, 0 );	


	sTempRect.x	= 30;
	sTempRect.y	= 168;
	sTempRect.w	= 190;
	sTempRect.h	= 14;
		
	mcp_effect[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[3]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[3]->SetBlank( 0, 0 );	
	
	
	
	sTempRect.x	= 30;
	sTempRect.y	= 186 ;
	sTempRect.w	= 190;
	sTempRect.h	= 14;
	
	mcp_effect[4] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[4]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[4]->SetBlank( 0, 0 );	

	sTempRect.x	= 30;
	sTempRect.y	= 204;
	sTempRect.w	= 190;
	sTempRect.h	= 14;

	mcp_effect[5] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_effect[5]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[5]->SetBlank( 0, 0 );	

	sTempRect.x	= 115;
	sTempRect.y	= 67;
	sTempRect.w	= 20;
	sTempRect.h	= 14;

	///-- Skill Level
	mcp_Level = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_SUB_ENCHANT_TEXT_LEVEL, mpC_framewnd->GetID(), 
								sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_Level->SetFontR( n_fRsvFontWndStatic );	
	mcp_Level->SetBlank( 0, 0 );		

	///-- ���� ������ ��ư
	mcp_chargeWindow_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, mpC_framewnd->GetID(), 
			84, 202 + AddHeight, 87, 25, false );
	mcp_chargeWindow_btn->SetBtnImage( TID_CTRL_BTN87, TID_CTRL_BTN87_OVR, TID_CTRL_BTN87_CLK, TID_None );			
	mcp_chargeWindow_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_chargeWindow_btn->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );										
	mcp_chargeWindow_btn->SetVisible(false);
	
#if defined( PAYITEM_6 ) && defined( C_PAY_SYSTEM )
	mcp_chargeWindow_btn->SetVisible(true);
#endif

	///-- ���� ������ ��� ������
	mpCharge_framewnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_SUB_ENCHANT, 
		0, enchantwnd.h-2 , enchantwnd.w, 70, false );
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
		70, 14, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpCharge_framewnd->GetID(), 
		110, 14, 35, 35, false );      
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpCharge_framewnd->GetID(), 
		150, 14, 35, 35, false );     
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
	
	mcp_Chargeitem_slot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  mpCharge_framewnd->GetID(), 70, 14, 35, 35, false );
	mcp_Chargeitem_slot[0]->PcItem = &(mc_Chargeslot_item[0]);
	mcp_Chargeitem_slot[0]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[0]->SetHID( HID_ITEMTIP );

	mcp_Chargeitem_slot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  mpCharge_framewnd->GetID(), 110, 14, 35, 35, false );
	mcp_Chargeitem_slot[1]->PcItem = &(mc_Chargeslot_item[1]);
	mcp_Chargeitem_slot[1]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[1]->SetHID( HID_ITEMTIP );

	mcp_Chargeitem_slot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  mpCharge_framewnd->GetID(), 150, 14, 35, 35, false );
	mcp_Chargeitem_slot[2]->PcItem = &(mc_Chargeslot_item[2]);
	mcp_Chargeitem_slot[2]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[2]->SetHID( HID_ITEMTIP );
	
	CloseCharge_Wnd();	


	
}
