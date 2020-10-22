#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "..\\..\\brother.h"
#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\effect.h"
#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"
    

#include "InventoryWndProc.h"
#include "MainMenuWndProc.h"
#include "StoreWndProc.h"
#include "NumberInputWnd.h"
#include "ChatBoxWndProc.h"
#include "CDelayBarWnd.h"

#include "ExchangeWndProc.h"
#include "WarehouseWndProc.h"
#include "ItemEnchantWndProc.h"
#include "ItemExtractWndProc.h"
#include "ItemRemodelWndProc.h"
#include "ItemRepairWndProc.h"
#include "ItemCreateWndProc.h"
#include "stringmanager.h"
#include "CUserStoreSellWnd.h"
#include "CUserStoreBuyWnd.h"
#include "CUserStoreSetBuyWnd.h"
#include "PayItemWndProc.h"
#include "FrontierMainWndProc.h"
#include "ChatMainProc.h"
#include "UIMsgWnd.h"

#include "PetInventoryWnd.h"
#include "PetMainWndProc.h"
#include "CColMapPositionWnd.h"

#include "CAcryEnchantWnd.h"
#include "CChangeEldaColorWnd.h"

#include "StrengthStoneWndProc.h"
#include "GuildInventoryWndProc.h"
#include "FrontierCreateWnd.h"
#include "ProposeJoinFrontier.h"

#include "..\\..\\bravolibs\\obj\\player_manager.h"


CInventoryWnd	InventoryWnd;
CMoneyThrowWnd	g_MoneyThrowWnd;


extern long		g_nowTime;
long            g_slotNum;


TCHAR	gEldStr[64] = {0,};
int		gEldBufLen( 64 );

//-----------------------------------------------------------------------------
void CInventoryWnd::Init()
{
	int i = 0;
	bHorseInvenImg = false ;


	PcParam = nRui->GetPcParamInfo();
	PcInven = nRui->GetPcInvenInfo();
	
	
	for( i = 0; i < RP_MAX_EQUIP ; ++i )
	{
		WearSlot[i]->PcItem = PcInven->WearItem[i];
	}
	
	InvenState = n_INVEN_ITEM;

	for( i = 0; i < c_MaxInventoryItemDrawNum; ++i )
	{
		InvenSlot[i]->PcItem = PcInven->InvenItem[i];
	}

	theSkillCode_BasicCure				= 0;	
	theSkillCode_MasicPharmacy			= 0;

	theSkillCode_Sword					= 0;
	theSkillCode_TwohandSword			= 0;
	theSkillCode_Bow					= 0;
	theSkillCode_Shield					= 0;
	theSkillCode_MagicBook				= 0;
	theSkillCode_Staff					= 0;
	theSkillCode_CrystalBall			= 0;

	theSkillCode_LightArmor				= 0;
	theSkillCode_MasicArmor 			= 0;
	theSkillCode_PriestArmor 			= 0;
	theSkillCode_SpiritArmor			= 0;

	theSkillCode_CitizenBless			= 0;
	m_SrcSlot = NULL;
	m_TargetSlot = NULL;

	InvenPosChange(false);
	SetInvenState( n_INVEN_ITEM );

#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��
	
	SetPayInvenState(n_PAYINVEN_SHOP1);
	m_PayInvenState = n_PAYINVEN_SHOP1;
	PayStartSlotNum = 0;
	g_slotNum = 0; 	
	for( i = 0; i < c_MaxPayInvenItemDrawNum; ++i )
	{
		m_ItemSlot[i]->PcItem = PcInven->InvenItem[(PayStartSlotNum + i)+60];
	}
	
#endif //WIDE_PAY_INVENTORY

	ScrollBar->SetVisible( true );

#ifdef RIDING_SYSTEM
	nRui->SetGroupWnd( 3, WID_Inven_NormalSBtn,WID_Inven_NormalSBtn, WID_Inven_QuestSBtn,WID_Inven_HorseSBtn  );	
#else
	nRui->SetGroupWnd( 2, WID_Inven_NormalSBtn, WID_Inven_NormalSBtn, WID_Inven_QuestSBtn );
#endif	
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��
	nRui->SetGroupWnd( 5, WID_PayInven_Box1StateBtn, WID_PayInven_Box1StateBtn, WID_PayInven_Box2StateBtn, WID_PayInven_Box3StateBtn, WID_PayInven_Box4StateBtn, WID_PayInven_Box5StateBtn );
#endif //WIDE_PAY_INVENTORY
}

//-----------------------------------------------------------------------------
void CInventoryWnd::Composition()
{


	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect invenwnd;
	SRect WndRect;
	CStateButton* sbtn = NULL;
	CImageBox*		img = NULL;
	CTextBox*		tbox = NULL;
	CButton*		Pay_btn = NULL;
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��

	WndRect.w = 204;
	WndRect.h = 256;
	WndRect.x = 250; //center_x - ( WndRect.w / 2 );
	WndRect.y = 394; //245; //center_y - ( WndRect.h / 2 );

	PayFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_PayInventory_Wnd, WID_None, WndRect.x, WndRect.y, WndRect.w, WndRect.h, false );

	PayFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	PayFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	PayFrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	PayFrameWnd->SetWndTileSizeLR( 40, 29 );
	PayFrameWnd->SetWndTileSizeTB( 31, 11 );

	PayFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_PAYINVENTORY ) );	 
	PayFrameWnd->SetTitle( true );
	PayFrameWnd->SetWndProc( this );
	PayFrameWnd->SetVisible( false );
	PayFrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PayInventory_Wnd, 1, 50, 202, 198, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PayInventory_Wnd, 1, 49, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PayInventory_Wnd, 1, 250, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- Item Slots
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PayInventory_Wnd, 11, 56, 183, 187, false );    
	img->SetClientImage( TID_C_BGSLOT_INVEN );	

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	Pay_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_PayInven_CloseBtn, WID_PayInventory_Wnd, 185, 6, 13, 13, false );   
	Pay_btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	char temp[300];

	Statebtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_PayInven_Box1StateBtn, WID_PayInventory_Wnd, 7, 31, 38, 19, false );	
	Statebtn->SetBtnImage( TID_CTRL_TAB38_DIS, TID_CTRL_TAB38_DIS, TID_CTRL_TAB38, TID_None );
	Statebtn->SetFontR( n_fRsvFontWndButton );
	Rsprintf(RWCHAR(temp), _RT("%s%d"), G_STRING_CHAR( IDS_WND_WAREHOUSE_BOX ), 1);
	Statebtn->SetText( temp ); 
	Statebtn->SetVisible(false);

	Statebtn1 = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_PayInven_Box2StateBtn, WID_PayInventory_Wnd, 45, 31, 38, 19, false );	
	Statebtn1->SetBtnImage( TID_CTRL_TAB38_DIS, TID_CTRL_TAB38_DIS, TID_CTRL_TAB38, TID_None );
	Statebtn1->SetFontR( n_fRsvFontWndButton );
	Rsprintf(RWCHAR(temp), _RT("%s%d"), G_STRING_CHAR( IDS_WND_WAREHOUSE_BOX ), 2);
	Statebtn1->SetText( temp ); 
	Statebtn1->SetVisible(false);

	Statebtn2 = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_PayInven_Box3StateBtn, WID_PayInventory_Wnd, 83, 31, 38, 19, false );	
	Statebtn2->SetBtnImage( TID_CTRL_TAB38_DIS, TID_CTRL_TAB38_DIS, TID_CTRL_TAB38, TID_None );	
	Statebtn2->SetFontR( n_fRsvFontWndButton );
	Rsprintf(RWCHAR(temp), _RT("%s%d"), G_STRING_CHAR( IDS_WND_WAREHOUSE_BOX ), 3);
	Statebtn2->SetText( temp ); 
	Statebtn2->SetVisible(false);

	Statebtn3 = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_PayInven_Box4StateBtn, WID_PayInventory_Wnd,  121, 31, 38, 19, false );	
	Statebtn3->SetBtnImage( TID_CTRL_TAB38_DIS, TID_CTRL_TAB38_DIS, TID_CTRL_TAB38, TID_None );	
	Statebtn3->SetFontR( n_fRsvFontWndButton );
	Rsprintf(RWCHAR(temp), _RT("%s%d"), G_STRING_CHAR( IDS_WND_WAREHOUSE_BOX ), 4);
	Statebtn3->SetText( temp ); 
	Statebtn3->SetVisible(false);

	Statebtn4 = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_PayInven_Box5StateBtn, WID_PayInventory_Wnd, 159, 31, 38, 19, false );
	Statebtn4->SetBtnImage( TID_CTRL_TAB38_DIS, TID_CTRL_TAB38_DIS, TID_CTRL_TAB38, TID_None );
	Statebtn4->SetFontR( n_fRsvFontWndButton );
	Rsprintf(RWCHAR(temp), _RT("%s%d"), G_STRING_CHAR( IDS_WND_WAREHOUSE_BOX ), 5);
	Statebtn4->SetText( temp );
	Statebtn4->SetVisible(false);

	m_ItemSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot0, WID_PayInventory_Wnd,  11, 56, 35, 35, false );
	m_ItemSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot1, WID_PayInventory_Wnd,  48, 56, 35, 35, false );
	m_ItemSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot2, WID_PayInventory_Wnd,  85, 56, 35, 35, false );
	m_ItemSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot3, WID_PayInventory_Wnd, 122, 56, 35, 35, false );
	m_ItemSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot4, WID_PayInventory_Wnd, 159, 56, 35, 35, false );
	m_ItemSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot5, WID_PayInventory_Wnd,  11, 94, 35, 35, false );
	m_ItemSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot6, WID_PayInventory_Wnd,  48, 94, 35, 35, false );
	m_ItemSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot7, WID_PayInventory_Wnd,  85, 94, 35, 35, false );
	m_ItemSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot8, WID_PayInventory_Wnd, 122, 94, 35, 35, false );
	m_ItemSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot9, WID_PayInventory_Wnd, 159, 94, 35, 35, false );
	m_ItemSlot[10] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot10, WID_PayInventory_Wnd,  11, 132, 35, 35, false ); 
	m_ItemSlot[11] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot11, WID_PayInventory_Wnd,  48, 132, 35, 35, false ); 
	m_ItemSlot[12] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot12, WID_PayInventory_Wnd,  85, 132, 35, 35, false ); 
	m_ItemSlot[13] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot13, WID_PayInventory_Wnd, 122, 132, 35, 35, false ); 
	m_ItemSlot[14] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot14, WID_PayInventory_Wnd, 159, 132, 35, 35, false ); 
	m_ItemSlot[15] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot15, WID_PayInventory_Wnd,  11, 170, 35, 35, false ); 
	m_ItemSlot[16] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot16, WID_PayInventory_Wnd,  48, 170, 35, 35, false ); 
	m_ItemSlot[17] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot17, WID_PayInventory_Wnd,  85, 170, 35, 35, false ); 
	m_ItemSlot[18] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot18, WID_PayInventory_Wnd, 122, 170, 35, 35, false ); 
	m_ItemSlot[19] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot19, WID_PayInventory_Wnd, 159, 170, 35, 35, false ); 
	m_ItemSlot[20] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot20, WID_PayInventory_Wnd,  11, 208, 35, 35, false ); 
	m_ItemSlot[21] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot21, WID_PayInventory_Wnd,  48, 208, 35, 35, false ); 
	m_ItemSlot[22] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot22, WID_PayInventory_Wnd,  85, 208, 35, 35, false ); 
	m_ItemSlot[23] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot23, WID_PayInventory_Wnd, 122, 208, 35, 35, false ); 
	m_ItemSlot[24] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_PayInven_ItemSlot24, WID_PayInventory_Wnd, 159, 208, 35, 35, false ); 

	m_ItemSlot[0]->SetSlotGroup( n_SGROUP_INVENTORY, 60 );
	m_ItemSlot[0]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[1]->SetSlotGroup( n_SGROUP_INVENTORY, 61 );
	m_ItemSlot[1]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[2]->SetSlotGroup( n_SGROUP_INVENTORY, 62 );
	m_ItemSlot[2]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[3]->SetSlotGroup( n_SGROUP_INVENTORY, 63 );
	m_ItemSlot[3]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[4]->SetSlotGroup( n_SGROUP_INVENTORY, 64 );
	m_ItemSlot[4]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[5]->SetSlotGroup( n_SGROUP_INVENTORY, 65 );
	m_ItemSlot[5]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[6]->SetSlotGroup( n_SGROUP_INVENTORY, 66 );
	m_ItemSlot[6]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[7]->SetSlotGroup( n_SGROUP_INVENTORY, 67 );
	m_ItemSlot[7]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[8]->SetSlotGroup( n_SGROUP_INVENTORY, 68 );
	m_ItemSlot[8]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[9]->SetSlotGroup( n_SGROUP_INVENTORY, 69 );
	m_ItemSlot[9]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[10]->SetSlotGroup( n_SGROUP_INVENTORY, 70 );
	m_ItemSlot[10]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[11]->SetSlotGroup( n_SGROUP_INVENTORY, 71 );
	m_ItemSlot[11]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[12]->SetSlotGroup( n_SGROUP_INVENTORY, 72 );
	m_ItemSlot[12]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[13]->SetSlotGroup( n_SGROUP_INVENTORY, 73 );
	m_ItemSlot[13]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[14]->SetSlotGroup( n_SGROUP_INVENTORY, 74 );
	m_ItemSlot[14]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[15]->SetSlotGroup( n_SGROUP_INVENTORY, 75 );
	m_ItemSlot[15]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[16]->SetSlotGroup( n_SGROUP_INVENTORY, 76 );
	m_ItemSlot[16]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[17]->SetSlotGroup( n_SGROUP_INVENTORY, 77 );
	m_ItemSlot[17]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[18]->SetSlotGroup( n_SGROUP_INVENTORY, 78 );
	m_ItemSlot[18]->SetHID( HID_ITEMTIP );	
	m_ItemSlot[19]->SetSlotGroup( n_SGROUP_INVENTORY, 79 );
	m_ItemSlot[19]->SetHID( HID_ITEMTIP );
	m_ItemSlot[20]->SetSlotGroup( n_SGROUP_INVENTORY, 80 );
	m_ItemSlot[20]->SetHID( HID_ITEMTIP );
	m_ItemSlot[21]->SetSlotGroup( n_SGROUP_INVENTORY, 81 );
	m_ItemSlot[21]->SetHID( HID_ITEMTIP );
	m_ItemSlot[22]->SetSlotGroup( n_SGROUP_INVENTORY, 82 );
	m_ItemSlot[22]->SetHID( HID_ITEMTIP );
	m_ItemSlot[23]->SetSlotGroup( n_SGROUP_INVENTORY, 83 );
	m_ItemSlot[23]->SetHID( HID_ITEMTIP );
	m_ItemSlot[24]->SetSlotGroup( n_SGROUP_INVENTORY, 84 );
	m_ItemSlot[24]->SetHID( HID_ITEMTIP );

#endif //WIDE_PAY_INVENTORY

#ifdef RIDING_SYSTEM	//���̵� �۾� 

	invenwnd.w	= 355;
	invenwnd.h	= 300;

	invenwnd.x	= center_x - ( invenwnd.w / 2 );
	invenwnd.y	= center_y - ( invenwnd.h / 2 );
	

	///-- Frame Wnd

	FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_InventoryWnd, WID_None, invenwnd.x, invenwnd.y, invenwnd.w, invenwnd.h, false );

	FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	FrameWnd->SetWndTileSizeLR( 40, 29 );
	FrameWnd->SetWndTileSizeTB( 31, 11 );

	FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_INVENTORY ) );	 
	FrameWnd->SetTitle( true );
	FrameWnd->SetWndProc( this );
	FrameWnd->SetVisible( false );
	FrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------			
	///-- Line
	//�ڽ����� ���� �κ�  ��׶��� 

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 1, 61, 353, 202, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	//���� 
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 1, 60, 353, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	//���� 
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 1, 261 , 353, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	//���� 
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 1, 223, 151, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	//���� 
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 151 , 27, 1, 272, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
   
	///-- Slot
	///-- Wear Slot
	WearBgRain = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 24, 71, 109, 149, false );    
	WearBgRain->SetClientImage( TID_C_BGSLOT_WEAR_RAIN );	
	
	
	WearBgMillena= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 24, 71, 109, 149, false );    
	WearBgMillena->SetClientImage( TID_C_BGSLOT_WEAR_MILLENA );	


	///-- Item Slots
	m_BgSlotInven = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 155, 65, 183, 187, false );    
	m_BgSlotInven->SetClientImage( TID_C_BGSLOT_INVEN );	

	m_BgSlotInvenHorse = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 170, 102, 146, 111, false );    
	m_BgSlotInvenHorse->SetClientImage( TID_C_riding_inven );	
	m_BgSlotInvenHorse->SetVisible(false);
	
	///-- ���ǰ
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_InventoryWnd, 7, 30, 80, 17,  false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter );		
	tbox->SetText( G_STRING_CHAR( IDS_ITEM_EQUIP )); 

	///-- ����ǰ
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_InventoryWnd, 160, 30, 80, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter );		
	tbox->SetText( G_STRING_CHAR( IDS_ITEM_HAVING ) );

	//���� ������ �������� ��ư 
	m_pPopC_ItemBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_InventoryWnd, 
					 121, 30, 28, 28, false );   
	m_pPopC_ItemBtn->SetBtnImage( TID_POPC_ITEMBTN, TID_POPC_ITEMBTN_OVR, TID_POPC_ITEMBTN_CLK, TID_None );
	m_pPopC_ItemBtn->SetVisible(false);
	m_pPopC_ItemBtn->SetHID(HD_I_POP_CASH_ITEM);
#ifdef C_PAY_SYSTEM
	m_pPopC_ItemBtn->SetVisible(true);
#endif	

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	CButton* btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Inven_CloseBtn, WID_InventoryWnd, 336, 6, 13, 13, false );   
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	///-- Elda Text
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd,  153 + 26 , 264, 110, 23, false );  
	img->SetClientImage( TID_CTRL_IMG_ELDTEXT90 );


	///-- Elda Button
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Inven_MoneyBtn, WID_InventoryWnd, 153 , 264, 23, 23, false );
	btn->SetBtnImage( TID_CTRL_BTN_ELD, TID_CTRL_BTN_ELD_OVR, TID_CTRL_BTN_ELD_CLK, TID_None );

	///-- Money Text
	MoneyTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Inven_MoneyText, WID_InventoryWnd, 153 + 23 , 264, 110, 23, false );  
	MoneyTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );	
	MoneyTxt->SetClientImage( TID_None );
	MoneyTxt->SetAlignText( n_atRight, n_atCenter );
	MoneyTxt->SetText( _RT("") );
	MoneyTxt->SetTextColor( 255, 255, 255 );
	MoneyTxt->SetBlank( 2, 0 );

	
	///-- �Ϲ� button		size 78,16
	sbtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_Inven_NormalSBtn, WID_InventoryWnd, 155, 45, 62, 16, false );
	sbtn->SetBtnImage( TID_CTRL_BTN62, TID_CTRL_BTN62, TID_CTRL_BTN62_OVR, TID_None );
	sbtn->SetFontR( n_fRsvFontWndButton ); 
	sbtn->SetText( G_STRING_CHAR( IDS_NORMAL ) ); 
	

	///-- ����Ʈ button
	sbtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_Inven_QuestSBtn, WID_InventoryWnd, 217, 45, 62, 16, false );	
	sbtn->SetBtnImage( TID_CTRL_BTN62, TID_CTRL_BTN62, TID_CTRL_BTN62_OVR, TID_None );
	sbtn->SetFontR( n_fRsvFontWndButton ); 
	sbtn->SetText( G_STRING_CHAR( IDS_WND_QUEST ) );
		
	///-- ��  button
	sbtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_Inven_HorseSBtn, WID_InventoryWnd, 279, 45, 62, 16, false );	
	sbtn->SetBtnImage( TID_CTRL_BTN62, TID_CTRL_BTN62, TID_CTRL_BTN62_OVR, TID_None );
	sbtn->SetFontR( n_fRsvFontWndButton ); 
	sbtn->SetText( G_STRING_CHAR( IDS_RIDING ) );			
	


	nRui->SetGroupWnd( 3, WID_Inven_NormalSBtn,WID_Inven_NormalSBtn, WID_Inven_QuestSBtn,WID_Inven_HorseSBtn  );
	nRui->SetGroupWnd( 5, WID_PayInven_Box1StateBtn, WID_PayInven_Box1StateBtn, WID_PayInven_Box2StateBtn, WID_PayInven_Box3StateBtn, WID_PayInven_Box4StateBtn, WID_PayInven_Box5StateBtn );

/*
	ScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_Inven_Scroll, WID_InventoryWnd, 356, 77, 15, 193, false );
	//sb->SetClientColor( 255, 255, 0 );
	ScrollBar->InitSlide( stVertSld, 0, 7, 0, 8 );//Type, min, max, value, ���� ��

	ScrollBar->SetClientImage( TID_None );
	ScrollBar->SetThumbImage( TID_Inven_Scroll_Thumb, TID_Inven_Scroll_Thumb, TID_Inven_Scroll_Thumb, TID_None );
	ScrollBar->SetScrollBtnA( TID_None, TID_None, TID_Inven_ScrollUp_Select, TID_None );
	ScrollBar->SetScrollBtnB( TID_None, TID_None, TID_Inven_ScrollDown_Select, TID_None );
	*/
	ScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_Inven_Scroll, WID_InventoryWnd, 339, 61, 15, 195, false );
	ScrollBar->InitSlide( stVertSld, 0, 7, 0, 13 );//Type, min, max, value, ���� ��

	ScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	ScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	ScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	ScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	//����(���)
	WearSlot[n_SlotIdx_Hair] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot4, WID_InventoryWnd, 61, 71, 35, 35, false );
	WearSlot[n_SlotIdx_Hair]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Hair]->SetSlotNum( 4 );
	WearSlot[n_SlotIdx_Hair]->SetWearMark( n_Class_Helmet );
	WearSlot[n_SlotIdx_Hair]->SetHID( HID_ITEMTIP );
	//����
	WearSlot[n_SlotIdx_Coat] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot0, WID_InventoryWnd, 61, 109, 35, 35, false );
	WearSlot[n_SlotIdx_Coat]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Coat]->SetSlotNum( 0 );
	WearSlot[n_SlotIdx_Coat]->SetWearMark( n_Class_CoatArmor );
	WearSlot[n_SlotIdx_Coat]->SetHID( HID_ITEMTIP );
	//����, 
	WearSlot[n_SlotIdx_Pants] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot1, WID_InventoryWnd, 61, 147, 35, 35, false );
	WearSlot[n_SlotIdx_Pants]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Pants]->SetSlotNum( 1 );
	WearSlot[n_SlotIdx_Pants]->SetWearMark( n_Class_PantsArmor );
	WearSlot[n_SlotIdx_Pants]->SetHID( HID_ITEMTIP );
	//����, 
	WearSlot[n_SlotIdx_Boots] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot7, WID_InventoryWnd, 61, 185, 35, 35, false );
	WearSlot[n_SlotIdx_Boots]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Boots]->SetSlotNum( 7 );
	WearSlot[n_SlotIdx_Boots]->SetWearMark( n_Class_Shoes );
	WearSlot[n_SlotIdx_Boots]->SetHID( HID_ITEMTIP );
	//////////////////////////////////////////////////////////////////////////


	//����, 
	WearSlot[n_SlotIdx_Sub] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot5, WID_InventoryWnd, 98, 71, 35, 35, false );
	WearSlot[n_SlotIdx_Sub]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Sub]->SetSlotNum( 5 );
	WearSlot[n_SlotIdx_Sub]->SetWearMark( n_Class_Serve );
	WearSlot[n_SlotIdx_Sub]->SetHID( HID_ITEMTIP );
	//�尩, 
	WearSlot[n_SlotIdx_Glove] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot6, WID_InventoryWnd, 98, 109, 35, 35, false );
	WearSlot[n_SlotIdx_Glove]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Glove]->SetSlotNum( 6 );
	WearSlot[n_SlotIdx_Glove]->SetWearMark( n_Class_Gloves );
	WearSlot[n_SlotIdx_Glove]->SetHID( HID_ITEMTIP );
	//����, 
	WearSlot[n_SlotIdx_Shield] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot3, WID_InventoryWnd, 98, 147, 35, 35, false );
	WearSlot[n_SlotIdx_Shield]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Shield]->SetSlotNum( 3 );
	WearSlot[n_SlotIdx_Shield]->SetWearMark( n_Class_Shield );
	WearSlot[n_SlotIdx_Shield]->SetHID( HID_ITEMTIP );
	//////////////////////////////////////////////////////////////////////////
	
	//��ű�1,
	WearSlot[n_SlotIdx_Accessory1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot8, WID_InventoryWnd, 24, 71, 35, 35, false );
	WearSlot[n_SlotIdx_Accessory1]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Accessory1]->SetSlotNum( 8 );
	WearSlot[n_SlotIdx_Accessory1]->SetWearMark( n_Class_Accessory );
	WearSlot[n_SlotIdx_Accessory1]->SetHID( HID_ITEMTIP );
	//��ű�2
	WearSlot[n_SlotIdx_Accessory2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot9, WID_InventoryWnd, 24, 109, 35, 35, false );
	WearSlot[n_SlotIdx_Accessory2]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Accessory2]->SetSlotNum( 9 );
	WearSlot[n_SlotIdx_Accessory2]->SetWearMark( n_Class_Accessory );
	WearSlot[n_SlotIdx_Accessory2]->SetHID( HID_ITEMTIP );
	//����, 
	WearSlot[n_SlotIdx_Weapon] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot2, WID_InventoryWnd, 24, 147, 35, 35, false );
	WearSlot[n_SlotIdx_Weapon]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Weapon]->SetSlotNum( 2 );
	WearSlot[n_SlotIdx_Weapon]->SetWearMark( n_Class_Weapon );
	WearSlot[n_SlotIdx_Weapon]->SetHID( HID_ITEMTIP );


	//////////////////////////////////////////////////////////////////////////
	//dogns �߰� ���� �߰� �κ� 
	//////////////////////////////////////////////////////////////////////////
	
	//��
	WearSlot[n_Slotidx_Horse] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot10, WID_InventoryWnd, 24, 185, 35, 35, false );
	WearSlot[n_Slotidx_Horse]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_Slotidx_Horse]->SetSlotNum( 10 );
	WearSlot[n_Slotidx_Horse]->SetWearMark( n_Class_Riding );

	
#ifdef SINGAPORE_BLOCK_RIDING
	WearSlot[n_Slotidx_Horse]->SetHID( HID_WEAR_RIDING_SLOT);
#else
	WearSlot[n_Slotidx_Horse]->SetHID( HID_ITEMTIP);
#endif		
	
	///-- Item Slots


	for(int i=0; i < 3 ; ++i )
	{
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 24+(i*37), 225 , 35, 35, false );    
		img->SetClientImage( TID_C_riding_inven_Top + i );		
	}


	//���ǻ�� 
	WearSlot[n_Slotidx_Horse_Top] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot11, WID_InventoryWnd, 24, 225, 35, 35, false );
	WearSlot[n_Slotidx_Horse_Top]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_Slotidx_Horse_Top]->SetSlotNum( 11 );
	WearSlot[n_Slotidx_Horse_Top]->SetWearMark( n_Class_RidingItem );
	WearSlot[n_Slotidx_Horse_Top]->SetWearType( n_Type_Riding_Head );
#ifdef SINGAPORE_BLOCK_RIDING
	WearSlot[n_Slotidx_Horse_Top]->SetHID( HID_WEAR_RIDING_SLOT);
#else
	WearSlot[n_Slotidx_Horse_Top]->SetHID( HID_ITEMTIP);
#endif
	
	//�����ߴ� 
	WearSlot[n_Slotidx_Horse_Middle] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot12, WID_InventoryWnd, 61, 225, 35, 35, false );
	WearSlot[n_Slotidx_Horse_Middle]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_Slotidx_Horse_Middle]->SetSlotNum( 12 );
	WearSlot[n_Slotidx_Horse_Middle]->SetWearMark( n_Class_RidingItem );
	WearSlot[n_Slotidx_Horse_Middle]->SetWearType( n_Type_Riding_Saddle );
#ifdef SINGAPORE_BLOCK_RIDING
	WearSlot[n_Slotidx_Horse_Middle]->SetHID( HID_WEAR_RIDING_SLOT);
#else
	WearSlot[n_Slotidx_Horse_Middle]->SetHID( HID_ITEMTIP);
#endif
//�����ϴ� 
	WearSlot[n_Slotidx_Horse_Bottom] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot13, WID_InventoryWnd, 98, 225, 35, 35, false );
	WearSlot[n_Slotidx_Horse_Bottom]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_Slotidx_Horse_Bottom]->SetSlotNum( 13 );
	WearSlot[n_Slotidx_Horse_Bottom]->SetWearMark( n_Class_RidingItem );
	WearSlot[n_Slotidx_Horse_Bottom]->SetWearType( n_Type_Riding_Leg );
#ifdef SINGAPORE_BLOCK_RIDING
	WearSlot[n_Slotidx_Horse_Bottom]->SetHID( HID_WEAR_RIDING_SLOT);
#else
	WearSlot[n_Slotidx_Horse_Bottom]->SetHID( HID_ITEMTIP);
#endif
	


	for(short i=0; i < 3 ; ++i )
	{
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 24+(i*37),  263 , 35, 35, false );    
		img->SetClientImage( TID_C_skin_suit + i );	
	}

	WearSlot[n_Slotidx_Skin_Suit] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot14, WID_InventoryWnd, 24, 263, 35, 35, false );
	WearSlot[n_Slotidx_Skin_Suit]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_Slotidx_Skin_Suit]->SetSlotNum( 14 );
	WearSlot[n_Slotidx_Skin_Suit]->SetWearMark( n_Class_Skin );
	WearSlot[n_Slotidx_Skin_Suit]->SetHID( HID_ITEMTIP );

	WearSlot[n_Slotidx_Skin_Hair] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot15, WID_InventoryWnd, 61, 263 , 35, 35, false );
	WearSlot[n_Slotidx_Skin_Hair]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_Slotidx_Skin_Hair]->SetSlotNum( 15 );
	WearSlot[n_Slotidx_Skin_Hair]->SetWearMark( n_Class_Skin );
	WearSlot[n_Slotidx_Skin_Hair]->SetHID( HID_ITEMTIP );

	WearSlot[n_Slotidx_Skin_Wing] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot16, WID_InventoryWnd, 98, 263 , 35, 35, false );
	WearSlot[n_Slotidx_Skin_Wing]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_Slotidx_Skin_Wing]->SetSlotNum( 16 );
	WearSlot[n_Slotidx_Skin_Wing]->SetWearMark( n_Class_Skin );
	WearSlot[n_Slotidx_Skin_Wing]->SetHID( HID_ITEMTIP );

	
/*
	�� �ִ��� �𸣰��� ����
	CSlot* Slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_BgSlot, WID_InventoryWnd, 152, 70, 207, 208, false );
	Slot->SetSlotGroup( n_SGROUP_INVENTORY, 255 );
	Slot->SetDisableHighlight();
	Slot->SetHID( HID_None );
 */

	InvenSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot0, WID_InventoryWnd, 155, 65, 35, 35, false );
	InvenSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot1, WID_InventoryWnd, 192, 65, 35, 35, false );
	InvenSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot2, WID_InventoryWnd, 229, 65, 35, 35, false );
	InvenSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot3, WID_InventoryWnd, 266, 65, 35, 35, false );
	InvenSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot4, WID_InventoryWnd, 303, 65, 35, 35, false );
	
	InvenSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot5, WID_InventoryWnd, 155, 103, 35, 35, false );
	InvenSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot6, WID_InventoryWnd, 192, 103, 35, 35, false );
	InvenSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot7, WID_InventoryWnd, 229, 103, 35, 35, false );
	InvenSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot8, WID_InventoryWnd, 266, 103, 35, 35, false );
	InvenSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot9, WID_InventoryWnd, 303, 103, 35, 35, false );

	InvenSlot[10] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot10, WID_InventoryWnd, 155, 141, 35, 35, false );
	InvenSlot[11] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot11, WID_InventoryWnd, 192, 141, 35, 35, false );
	InvenSlot[12] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot12, WID_InventoryWnd, 229, 141, 35, 35, false );
	InvenSlot[13] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot13, WID_InventoryWnd, 266, 141, 35, 35, false );
	InvenSlot[14] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot14, WID_InventoryWnd, 303, 141, 35, 35, false );
	
	InvenSlot[15] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot15, WID_InventoryWnd, 155, 179, 35, 35, false );
	InvenSlot[16] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot16, WID_InventoryWnd, 192, 179, 35, 35, false );
	InvenSlot[17] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot17, WID_InventoryWnd, 229, 179, 35, 35, false );
	InvenSlot[18] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot18, WID_InventoryWnd, 266, 179, 35, 35, false );
	InvenSlot[19] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot19, WID_InventoryWnd, 303, 179, 35, 35, false );

	InvenSlot[20] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot20, WID_InventoryWnd, 155, 217, 35, 35, false );
	InvenSlot[21] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot21, WID_InventoryWnd, 192, 217, 35, 35, false );
	InvenSlot[22] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot22, WID_InventoryWnd, 229, 217, 35, 35, false );
	InvenSlot[23] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot23, WID_InventoryWnd, 266, 217, 35, 35, false );
	InvenSlot[24] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot24, WID_InventoryWnd, 303, 217, 35, 35, false );
#else //RIDING//���̵��� �ƴϸ� .. 
	
//////////////////////////////////////////////////////////////////////////
//	���̵� �۾� �������� ..
//////////////////////////////////////////////////////////////////////////
	invenwnd.w	= 355;
	invenwnd.h	= 274;
	invenwnd.x	= center_x - ( invenwnd.w / 2 );
	invenwnd.y	= center_y - ( invenwnd.h / 2 );
	
	///-- Frame Wnd
	FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_InventoryWnd, WID_None, invenwnd.x, invenwnd.y, invenwnd.w, invenwnd.h, false );
	FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	FrameWnd->SetIconTID( TID_CTRL_ICON_INVEN );	
	FrameWnd->SetWndTileSizeLR( 40, 29 );
	FrameWnd->SetWndTileSizeTB( 31, 11 );

	FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_INVENTORY ) );	 
	FrameWnd->SetTitle( true );
	FrameWnd->SetWndProc( this );
	FrameWnd->SetVisible( false );
	FrameWnd->Initialize();


	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------			
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 1, 61, 353, 195, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 1, 60, 353, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 1, 256, 353, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 151, 27, 1, 245, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
      
	///-- Slot
	///-- Wear Slot
	WearBgRain = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 24, 71, 109, 149, false );    
	WearBgRain->SetClientImage( TID_C_BGSLOT_WEAR_RAIN2 );	
	
	
	WearBgMillena= (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 24, 71, 109, 149, false );    
	WearBgMillena->SetClientImage( TID_C_BGSLOT_WEAR_MILLENA2 );	

	///-- Item Slots
	m_BgSlotInven = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 155, 65, 183, 187, false );    
	m_BgSlotInven->SetClientImage( TID_C_BGSLOT_INVEN );	

	///-- Elda Text
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_InventoryWnd, 34, 225, 90, 23, false );  
	img->SetClientImage( TID_CTRL_IMG_ELDTEXT90 );

	///-- ���ǰ
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_InventoryWnd, 7, 30, 80, 17,  false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter );		
	tbox->SetText( G_STRING_CHAR( IDS_ITEM_EQUIP )); 

	///-- ����ǰ
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_InventoryWnd, 160, 30, 80, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetAlignText( n_atLeft, n_atCenter );		
	tbox->SetText( G_STRING_CHAR( IDS_ITEM_HAVING ) );

	//���� ������ �������� ��ư 
	m_pPopC_ItemBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_InventoryWnd, 
					 121, 30, 28, 28, false );   
	m_pPopC_ItemBtn->SetBtnImage( TID_POPC_ITEMBTN, TID_POPC_ITEMBTN_OVR, TID_POPC_ITEMBTN_CLK, TID_None );
	m_pPopC_ItemBtn->SetVisible(false);
	m_pPopC_ItemBtn->SetHID(HD_I_POP_CASH_ITEM);
#ifdef C_PAY_SYSTEM
	m_pPopC_ItemBtn->SetVisible(true);
#endif	

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X		
	CButton* btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Inven_CloseBtn, WID_InventoryWnd, 336, 6, 13, 13, false );   
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	///-- Elda Button
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Inven_MoneyBtn, WID_InventoryWnd, 11, 225, 23, 23, false );
	btn->SetBtnImage( TID_CTRL_BTN_ELD, TID_CTRL_BTN_ELD_OVR, TID_CTRL_BTN_ELD_CLK, TID_None );

	///-- Money Text
	MoneyTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Inven_MoneyText, WID_InventoryWnd, 30, 225, 86, 23, false );  
	MoneyTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );	
	MoneyTxt->SetClientImage( TID_None );
	MoneyTxt->SetAlignText( n_atRight, n_atCenter );
	MoneyTxt->SetText( _RT("") );
	MoneyTxt->SetTextColor( 255, 255, 255 );
	MoneyTxt->SetBlank( 2, 0 );
	
	///-- �Ϲ� button
	sbtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_Inven_NormalSBtn, WID_InventoryWnd, 155, 45, 78, 16, false );
	sbtn->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );
	sbtn->SetFontR( n_fRsvFontWndButton ); 
	sbtn->SetText( G_STRING_CHAR( IDS_NORMAL ) ); 
	
	///-- ����Ʈ button
	sbtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_Inven_QuestSBtn, WID_InventoryWnd, 233, 45, 78, 16, false );	
	sbtn->SetBtnImage( TID_CTRL_TAB78_DIS, TID_CTRL_TAB78_DIS, TID_CTRL_TAB78, TID_None );	
	sbtn->SetFontR( n_fRsvFontWndButton ); 
	sbtn->SetText( G_STRING_CHAR( IDS_WND_QUEST ) );
		
	nRui->SetGroupWnd( 2, WID_Inven_NormalSBtn, WID_Inven_NormalSBtn, WID_Inven_QuestSBtn );

/*
	ScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_Inven_Scroll, WID_InventoryWnd, 356, 77, 15, 193, false );
	//sb->SetClientColor( 255, 255, 0 );
	ScrollBar->InitSlide( stVertSld, 0, 7, 0, 8 );//Type, min, max, value, ���� ��

	ScrollBar->SetClientImage( TID_None );
	ScrollBar->SetThumbImage( TID_Inven_Scroll_Thumb, TID_Inven_Scroll_Thumb, TID_Inven_Scroll_Thumb, TID_None );
	ScrollBar->SetScrollBtnA( TID_None, TID_None, TID_Inven_ScrollUp_Select, TID_None );
	ScrollBar->SetScrollBtnB( TID_None, TID_None, TID_Inven_ScrollDown_Select, TID_None );
*/
	ScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_Inven_Scroll, WID_InventoryWnd, 339, 61, 15, 195, false );
	ScrollBar->InitSlide( stVertSld, 0, 7, 0, 13 );//Type, min, max, value, ���� ��

	ScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	ScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	ScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	ScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );
	


	//����
	WearSlot[n_SlotIdx_Coat] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot0, WID_InventoryWnd, 61, 109, 35, 35, false );
	WearSlot[n_SlotIdx_Coat]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Coat]->SetSlotNum( 0 );
	WearSlot[n_SlotIdx_Coat]->SetWearMark( n_Class_CoatArmor );
	WearSlot[n_SlotIdx_Coat]->SetHID( HID_ITEMTIP );
	//����, 
	WearSlot[n_SlotIdx_Pants] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot1, WID_InventoryWnd, 61, 147, 35, 35, false );
	WearSlot[n_SlotIdx_Pants]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Pants]->SetSlotNum( 1 );
	WearSlot[n_SlotIdx_Pants]->SetWearMark( n_Class_PantsArmor );
	WearSlot[n_SlotIdx_Pants]->SetHID( HID_ITEMTIP );
	//����, 
	WearSlot[n_SlotIdx_Weapon] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot2, WID_InventoryWnd, 24, 166, 35, 35, false );
	WearSlot[n_SlotIdx_Weapon]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Weapon]->SetSlotNum( 2 );
	WearSlot[n_SlotIdx_Weapon]->SetWearMark( n_Class_Weapon );
	WearSlot[n_SlotIdx_Weapon]->SetHID( HID_ITEMTIP );
	//����, 
	WearSlot[n_SlotIdx_Shield] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot3, WID_InventoryWnd, 98, 166, 35, 35, false );
	WearSlot[n_SlotIdx_Shield]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Shield]->SetSlotNum( 3 );
	WearSlot[n_SlotIdx_Shield]->SetWearMark( n_Class_Shield );
	WearSlot[n_SlotIdx_Shield]->SetHID( HID_ITEMTIP );
	//����(���)
	WearSlot[n_SlotIdx_Hair] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot4, WID_InventoryWnd, 61, 71, 35, 35, false );
	WearSlot[n_SlotIdx_Hair]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Hair]->SetSlotNum( 4 );
	WearSlot[n_SlotIdx_Hair]->SetWearMark( n_Class_Helmet );
	WearSlot[n_SlotIdx_Hair]->SetHID( HID_ITEMTIP );
	//����, 
	WearSlot[n_SlotIdx_Sub] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot5, WID_InventoryWnd, 98, 90, 35, 35, false );
	WearSlot[n_SlotIdx_Sub]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Sub]->SetSlotNum( 5 );
	WearSlot[n_SlotIdx_Sub]->SetWearMark( n_Class_Serve );
	WearSlot[n_SlotIdx_Sub]->SetHID( HID_ITEMTIP );
	//�尩, 
	WearSlot[n_SlotIdx_Glove] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot6, WID_InventoryWnd, 98, 128, 35, 35, false );
	WearSlot[n_SlotIdx_Glove]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Glove]->SetSlotNum( 6 );
	WearSlot[n_SlotIdx_Glove]->SetWearMark( n_Class_Gloves );
	WearSlot[n_SlotIdx_Glove]->SetHID( HID_ITEMTIP );
	//����, 
	WearSlot[n_SlotIdx_Boots] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot7, WID_InventoryWnd, 61, 185, 35, 35, false );
	WearSlot[n_SlotIdx_Boots]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Boots]->SetSlotNum( 7 );
	WearSlot[n_SlotIdx_Boots]->SetWearMark( n_Class_Shoes );
	WearSlot[n_SlotIdx_Boots]->SetHID( HID_ITEMTIP );
	//��ű�1,
	WearSlot[n_SlotIdx_Accessory1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot8, WID_InventoryWnd, 24, 90, 35, 35, false );
	WearSlot[n_SlotIdx_Accessory1]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Accessory1]->SetSlotNum( 8 );
	WearSlot[n_SlotIdx_Accessory1]->SetWearMark( n_Class_Accessory );
	WearSlot[n_SlotIdx_Accessory1]->SetHID( HID_ITEMTIP );
	//��ű�2
	WearSlot[n_SlotIdx_Accessory2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_WearSlot9, WID_InventoryWnd, 24, 128, 35, 35, false );
	WearSlot[n_SlotIdx_Accessory2]->SetSlotGroup( n_SGROUP_WEAR );
	WearSlot[n_SlotIdx_Accessory2]->SetSlotNum( 9 );
	WearSlot[n_SlotIdx_Accessory2]->SetWearMark( n_Class_Accessory );
	WearSlot[n_SlotIdx_Accessory2]->SetHID( HID_ITEMTIP );

/*
	�� �ִ��� �𸣰��� ����
	CSlot* Slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_BgSlot, WID_InventoryWnd, 152, 70, 207, 208, false );
	Slot->SetSlotGroup( n_SGROUP_INVENTORY, 255 );
	Slot->SetDisableHighlight();
	Slot->SetHID( HID_None );
 */

	InvenSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot0, WID_InventoryWnd, 155, 65, 35, 35, false );
	InvenSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot1, WID_InventoryWnd, 192, 65, 35, 35, false );
	InvenSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot2, WID_InventoryWnd, 229, 65, 35, 35, false );
	InvenSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot3, WID_InventoryWnd, 266, 65, 35, 35, false );
	InvenSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot4, WID_InventoryWnd, 303, 65, 35, 35, false );
	InvenSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot5, WID_InventoryWnd, 155, 103, 35, 35, false );
	InvenSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot6, WID_InventoryWnd, 192, 103, 35, 35, false );
	InvenSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot7, WID_InventoryWnd, 229, 103, 35, 35, false );
	InvenSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot8, WID_InventoryWnd, 266, 103, 35, 35, false );
	InvenSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot9, WID_InventoryWnd, 303, 103, 35, 35, false );
	
	InvenSlot[10] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot10, WID_InventoryWnd, 155, 141, 35, 35, false );
	InvenSlot[11] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot11, WID_InventoryWnd, 192, 141, 35, 35, false );
	InvenSlot[12] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot12, WID_InventoryWnd, 229, 141, 35, 35, false );
	InvenSlot[13] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot13, WID_InventoryWnd, 266, 141, 35, 35, false );
	InvenSlot[14] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot14, WID_InventoryWnd, 303, 141, 35, 35, false );
	InvenSlot[15] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot15, WID_InventoryWnd, 155, 179, 35, 35, false );
	InvenSlot[16] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot16, WID_InventoryWnd, 192, 179, 35, 35, false );
	InvenSlot[17] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot17, WID_InventoryWnd, 229, 179, 35, 35, false );
	InvenSlot[18] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot18, WID_InventoryWnd, 266, 179, 35, 35, false );
	InvenSlot[19] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot19, WID_InventoryWnd, 303, 179, 35, 35, false );
	InvenSlot[20] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot20, WID_InventoryWnd, 155, 217, 35, 35, false );
	InvenSlot[21] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot21, WID_InventoryWnd, 192, 217, 35, 35, false );
	InvenSlot[22] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot22, WID_InventoryWnd, 229, 217, 35, 35, false );
	InvenSlot[23] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot23, WID_InventoryWnd, 266, 217, 35, 35, false );
	InvenSlot[24] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Inven_ItemSlot24, WID_InventoryWnd, 303, 217, 35, 35, false );

#endif	//RIDING_UI_SYSTEM
	
	//�̰� ���� 
	InvenSlot[0]->SetSlotGroup( n_SGROUP_INVENTORY, 0 );	
	InvenSlot[0]->SetHID( HID_ITEMTIP );	
	InvenSlot[1]->SetSlotGroup( n_SGROUP_INVENTORY, 1 );
	InvenSlot[1]->SetHID( HID_ITEMTIP );	
	InvenSlot[2]->SetSlotGroup( n_SGROUP_INVENTORY, 2 );
	InvenSlot[2]->SetHID( HID_ITEMTIP );	
	InvenSlot[3]->SetSlotGroup( n_SGROUP_INVENTORY, 3 );
	InvenSlot[3]->SetHID( HID_ITEMTIP );	
	InvenSlot[4]->SetSlotGroup( n_SGROUP_INVENTORY, 4 );
	InvenSlot[4]->SetHID( HID_ITEMTIP );
	InvenSlot[5]->SetSlotGroup( n_SGROUP_INVENTORY, 5 );
	InvenSlot[5]->SetHID( HID_ITEMTIP );	
	InvenSlot[6]->SetSlotGroup( n_SGROUP_INVENTORY, 6 );
	InvenSlot[6]->SetHID( HID_ITEMTIP );	
	InvenSlot[7]->SetSlotGroup( n_SGROUP_INVENTORY, 7 );
	InvenSlot[7]->SetHID( HID_ITEMTIP );	
	InvenSlot[8]->SetSlotGroup( n_SGROUP_INVENTORY, 8 );
	InvenSlot[8]->SetHID( HID_ITEMTIP );
	InvenSlot[9]->SetSlotGroup( n_SGROUP_INVENTORY, 9 );
	InvenSlot[9]->SetHID( HID_ITEMTIP );
	InvenSlot[10]->SetSlotGroup( n_SGROUP_INVENTORY, 10 );
	InvenSlot[10]->SetHID( HID_ITEMTIP );
	InvenSlot[11]->SetSlotGroup( n_SGROUP_INVENTORY, 11 );
	InvenSlot[11]->SetHID( HID_ITEMTIP );
	InvenSlot[12]->SetSlotGroup( n_SGROUP_INVENTORY, 12 );
	InvenSlot[12]->SetHID( HID_ITEMTIP );	
	InvenSlot[13]->SetSlotGroup( n_SGROUP_INVENTORY, 13 );
	InvenSlot[13]->SetHID( HID_ITEMTIP );
	InvenSlot[14]->SetSlotGroup( n_SGROUP_INVENTORY, 14 );
	InvenSlot[14]->SetHID( HID_ITEMTIP );	
	InvenSlot[15]->SetSlotGroup( n_SGROUP_INVENTORY, 15 );
	InvenSlot[15]->SetHID( HID_ITEMTIP );
	InvenSlot[16]->SetSlotGroup( n_SGROUP_INVENTORY, 16 );
	InvenSlot[16]->SetHID( HID_ITEMTIP );	
	InvenSlot[17]->SetSlotGroup( n_SGROUP_INVENTORY, 17 );
	InvenSlot[17]->SetHID( HID_ITEMTIP );	
	InvenSlot[18]->SetSlotGroup( n_SGROUP_INVENTORY, 18 );
	InvenSlot[18]->SetHID( HID_ITEMTIP );	
	InvenSlot[19]->SetSlotGroup( n_SGROUP_INVENTORY, 19 );
	InvenSlot[19]->SetHID( HID_ITEMTIP );	
	InvenSlot[20]->SetSlotGroup( n_SGROUP_INVENTORY, 20 );
	InvenSlot[20]->SetHID( HID_ITEMTIP );	
	InvenSlot[21]->SetSlotGroup( n_SGROUP_INVENTORY, 21 );
	InvenSlot[21]->SetHID( HID_ITEMTIP );	
	InvenSlot[22]->SetSlotGroup( n_SGROUP_INVENTORY, 22 );
	InvenSlot[22]->SetHID( HID_ITEMTIP );	
	InvenSlot[23]->SetSlotGroup( n_SGROUP_INVENTORY, 23 );
	InvenSlot[23]->SetHID( HID_ITEMTIP );	
	InvenSlot[24]->SetSlotGroup( n_SGROUP_INVENTORY, 24 );
	InvenSlot[24]->SetHID( HID_ITEMTIP );

	//�ʱ�ȭ
	Init();
}

void CInventoryWnd::GetInvenSlot( DWORD nSlot) //�����κ�Ȯ��
{
	g_slotNum = nSlot; //�������� �κ����� ���� �޾ƿ´�
}

void CInventoryWnd::InvenPosChange(bool bRide)
{
	
#ifdef RIDING_SYSTEM
	if(!bRide)
	{
		m_BgSlotInven->SetVisible(true);
		m_BgSlotInvenHorse->SetVisible(false);
		//�Ϲ��κ��� 
		const int StartX = 155;
		const int StartY = 65;
		
		int xPos = StartX;
		int yPos = StartY;
		
		for(int i =0 ; i < 25 ; ++i)
		{
			//148 �κ� 5�� �� 
			if(xPos > StartX+(37*4))
			{
				xPos = StartX;
				yPos += 38;
			}
			
			InvenSlot[i]->SetWndRepositionR(xPos,yPos );
			InvenSlot[i]->SetVisible(true);

			xPos += 37;
		}
	}
	else
	{
		m_BgSlotInven->SetVisible(false);
		m_BgSlotInvenHorse->SetVisible(true);

		//���κ��� �� 9�� �� ���� .. 

		InvenSlot[4]->SetVisible(false);
		InvenSlot[9]->SetVisible(false);
		InvenSlot[14]->SetVisible(false);

		//���κ� 
		const int StartX = 170;
		const int StartY = 102;
		
		int xPos = StartX;
		int yPos = StartY;
		
		for(int i =0 ; i < 25 ; ++i)
		{
			//111 �κ� 4�� �� 
			if(xPos > StartX+(37*4))
			{
				xPos = StartX;
				yPos += 38;
			}
			
			InvenSlot[i]->SetWndRepositionR(xPos,yPos );			
			xPos += 37;			

			if(i >= 15)
			{
				InvenSlot[i]->SetVisible(false);	
			}			
		}
	}

#endif	
}

void CInventoryWnd::FindDeleteItem(DWORD ItemUID)
{
	int i = 0 ;

	//�κ��˻� 
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{
		if( PcInven->InvenItem[i] && PcInven->InvenItem[i]->ItemUID == ItemUID )
		{
			delete PcInven->InvenItem[i];
			PcInven->InvenItem[i] = NULL;
		}
	}
	//���̵��� 
	for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
	{
		if(PcInven->InvenQuest[i] && PcInven->InvenQuest[i]->ItemUID == ItemUID )
		{
			delete 	PcInven->InvenQuest[i];
			PcInven->InvenQuest[i] = NULL;
		}

	}
	//����Ʈ�� 
	for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
	{
		if(PcInven->InvenHorse[i] && PcInven->InvenHorse[i]->ItemUID == ItemUID )
		{
			delete PcInven->InvenHorse[i];
			PcInven->InvenHorse[i] = NULL;
		}
	}
	//����� 
	for( i = 0; i < RP_MAX_EQUIP ; ++i )
	{
		if(PcInven->WearItem[i] && PcInven->WearItem[i]->ItemUID == ItemUID )
		{
			//ĳ���Ϳ� ���� ����....
			g_Pc.GetPlayer()->Item_TakeOff( i );
			
			delete PcInven->WearItem[i];
			PcInven->WearItem[i]= NULL;

			if( i == n_WearSlot_SKIN_Serve  && nRui->thePcInven.WearItem[ n_WearSlot_Serve] ) 
			{
				g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[ n_WearSlot_Serve]->Code , g_Pc.GetPlayer() );
			}

			
		
		}
	}
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��
	
	for( i = 0; i < RP_MAX_PAYINVENTORY; ++i )
	{
		if(PcInven->InvenItem[i+RP_MAX_INVENTORY] && PcInven->InvenItem[i+RP_MAX_INVENTORY]->ItemUID == ItemUID )
		{
			delete PcInven->InvenItem[i+RP_MAX_INVENTORY];
			PcInven->InvenItem[i+RP_MAX_INVENTORY] = NULL;
		}
	}	
#endif //WIDE_PAY_INVENTORY
	UpdateInvenSlotItem();	
	UpdateWearSlotItem();
}


//-----------------------------------------------------------------------------
void CInventoryWnd::Update()
{
	static char str[256] = {0,};
	
	//�����̳Ĺз����� ..  dongs	
	
	if( m_IsSendSetItemEquip )
	{
		if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( m_SendSetItmCheckTime, 300 ) ) )
		{
			m_IsSendSetItemEquip = FALSE;
		}
	}

	if(  g_Pc.m_MainPC.char_info.theCitizen == MILLENA)
	{
		WearBgRain->SetVisible(false);
		WearBgMillena->SetVisible(true);
	}
	//RAIN
	else 
	{
		WearBgRain->SetVisible(true);
		WearBgMillena->SetVisible(false);
	}	

	if( g_RockClient.GetGameMode() == GMODE_NORMAL )
	{
//		sprintf( str, "%d", PcParam->Money );
		SetEldText( str, PcParam->Money );
		MoneyTxt->SetText( str );
	}
		
	for( int i = 0; i < c_MaxInventoryItemDrawNum; ++i )
	{
		if( g_ExchangeWnd.m_bIsExchangeLock )
		{
			InvenSlot[i]->theSlotState = n_Slot_Disable;
		}
		else if( g_UserStoreSellWnd.ISUserSellMode() )
		{
			if( g_UserStoreSellWnd.m_UserStoreSellOpenMode && InvenSlot[i]->PcItem != NULL )
			{
				if( InvenSlot[i]->PcItem->SkillTable != NULL )
				{
					if( InvenSlot[i]->PcItem->SkillTable->theSkillCode == c_SkillCode_UserSellStore )
					{
						InvenSlot[i]->theSlotState = n_Slot_Enable;
					}
					else
					{
						InvenSlot[i]->theSlotState = n_Slot_Disable;
					}
				}
				else
				{
					InvenSlot[i]->theSlotState = n_Slot_Disable;
				}
			}
			else
				InvenSlot[i]->theSlotState = n_Slot_Enable;
		}
		else
		{
			InvenSlot[i]->theSlotState = n_Slot_Enable;

			// ������ �� ���� �����۵��� Disable ��Ų�� 
			if( InvenSlot[i]->PcItem && InvenSlot[i]->PcItem->ItemTable )
			{
				// �ʿ� ���� üũ 
				if( InvenSlot[i]->PcItem->ItemTable->theLimitLevel > nRui->thePcParam.Level )
				{
					InvenSlot[i]->theSlotState = n_Slot_Disable;
			
				}
				
				// ������ �� �ִ� ���������� üũ 
				if( InvenSlot[i]->PcItem->IType == n_ITYPE_WEAR )
				{
					if( CheckWearPassiveSkill( InvenSlot[i]->PcItem->ItemTable ) == false )
					{					
						InvenSlot[i]->theSlotState = n_Slot_Disable;		
					}
				}
				// ����� �� �ִ� ���������� üũ 
				else if( InvenSlot[i]->PcItem->IType == n_ITYPE_USE )
				{
					if( CheckUseItemPassiveSkill( InvenSlot[i]->PcItem->ItemTable ) == false )
					{					
						InvenSlot[i]->theSlotState = n_Slot_Disable;		
					}

					///--JAPAN_BUG_MODIFY
					if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotItem))
					{
						InvenSlot[i]->theSlotState = n_Slot_Disable;
					}
				}
				
				//��ġ ������ �ڱ� ���� �´°͸�  
				if( nRui->GetPcParamInfo()->Fame < InvenSlot[i]->PcItem->ItemTable->theLimitFame  )
				{
					InvenSlot[i]->theSlotState = n_Slot_Disable;					
				}
#ifdef PREMIUMZONE_KEY		
				if( InvenSlot[i]->PcItem->Code == 21560 || InvenSlot[i]->PcItem->Code == 21561 ||
					InvenSlot[i]->PcItem->Code == 21562 || InvenSlot[i]->PcItem->Code == 21563  ) 
				{
					if( InvenSlot[i]->PcItem->ItemTable->theLimitLevel > nRui->thePcParam.Level
					|| InvenSlot[i]->PcItem->ItemTable->theLimitLevel +19  < nRui->thePcParam.Level )
				{
					InvenSlot[i]->theSlotState = n_Slot_Disable;
				}
				}
#endif //PREMIUMZONE_KEY	
			}			
		}
	}

	return;
}

//-----------------------------------------------------------------------------
void CInventoryWnd::Proc( SEventMessage* EMsg )
{
	///-- ���� ������ �������� ��ư Ŭ��	
	if( m_pPopC_ItemBtn->GetID() == EMsg->FocusWnd )	
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_PayItemWnd.Open(); 
			
			g_RockClient.Send_CsOpenPayShop();
		}
	}	
	
	switch( EMsg->FocusWnd )
	{
///// �����κ�Ȯ�� ////////////////////////////////////////////////////////////////////////////////////////////////
	case WID_Inven_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

			IsVisible = false;

			if(g_PayItemWnd.IsVisible)
			{
				g_PayItemWnd.ClosePayItemWnd();
			}
			
		}
		break;
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��
	case WID_PayInven_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
		}
		break;
	case WID_PayInven_Box1StateBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetPayInvenState( n_PAYINVEN_SHOP1 );
		}
		break;
	case WID_PayInven_Box2StateBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetPayInvenState( n_PAYINVEN_SHOP2 );
		}
		break;
	case WID_PayInven_Box3StateBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetPayInvenState( n_PAYINVEN_SHOP3 );
		}
		break;
	case WID_PayInven_Box4StateBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetPayInvenState( n_PAYINVEN_SHOP4 );
		}
		break;
		
	case WID_PayInven_Box5StateBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetPayInvenState( n_PAYINVEN_SHOP5 );
		}
		break;
#endif //WIDE_PAY_INVENTORY
	case WID_Inven_MoneyBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( g_ExchangeWnd.m_bIsExchangeLock == FALSE && g_UserStoreSellWnd.ISUserSellMode() == FALSE )
			{
				if( g_ExchangeWnd.IsVisible )
				{
					g_MoneyThrowWnd.Open( n_ELDINPUT_INVENTORY_EXCHANGE );
				}
				else if( g_WarehouseWnd.IsVisible )
				{
					g_MoneyThrowWnd.Open( n_ELDINPUT_INVENTORY_WAREHOUSE );
				}
#ifdef DECO_RENEWAL_GUILD_MJH
				else if( g_GuildInvenWnd.IsVisible )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_WAREHOUSE_ON_EX ) );
				}
#endif // DECO_RENEWAL_GUILD_MJH
				else
				{
					g_MoneyThrowWnd.Open( n_ELDINPUT_INVENTORY_MONEYTHROW );
				}
			}
		}
		break;
	case WID_Inven_NormalSBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			InvenPosChange(false);
			SetInvenState( n_INVEN_ITEM );			
			ScrollBar->SetVisible( true );
		}
		break;
	case WID_Inven_QuestSBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			InvenPosChange(false);
			SetInvenState( n_INVEN_QUEST );
			ScrollBar->SetVisible( false );
		}
		break;
	case WID_Inven_HorseSBtn:
		if(EMsg->Notify == NM_BTN_CLICK)
		{
#ifdef SINGAPORE_BLOCK_RIDING
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_COMING_SOON_RIDING ) );
			return;
#endif 
	
			InvenPosChange(true);
			SetInvenState(n_INVEN_HORSE);
			ScrollBar->SetVisible(false);
		}
		break;

	case WID_Inven_Scroll:
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetSlotScroll( (int)EMsg->fParam );
		}
		break;
	case WID_Inven_WearSlot0:
	case WID_Inven_WearSlot1:
	case WID_Inven_WearSlot2:
	case WID_Inven_WearSlot3:
	case WID_Inven_WearSlot4:
	case WID_Inven_WearSlot5:
	case WID_Inven_WearSlot6:
	case WID_Inven_WearSlot7:
	case WID_Inven_WearSlot8:
	case WID_Inven_WearSlot9:
	case WID_Inven_WearSlot10:
	case WID_Inven_WearSlot11:
	case WID_Inven_WearSlot12:
	case WID_Inven_WearSlot13:
	case WID_Inven_WearSlot14:
	case WID_Inven_WearSlot15:
	case WID_Inven_WearSlot16:
		{
			if( g_ExchangeWnd.m_bIsExchangeLock == FALSE && g_UserStoreSellWnd.m_UserStoreSellOpenMode == FALSE ) 
			{
				if( EMsg->Notify == NM_SLOT_CLICK )			{	WearSlotClick( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	WearSlotDBClick( EMsg );	}
				else if( EMsg->Notify == NM_SLOT_DRAG )		{	WearSlotDrag( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DROP )		{	WearSlotDrop( EMsg );		}
			}
		}
		break;
	//-------------------------------------------------------------------------
	case WID_Inven_BgSlot:
		if( EMsg->Notify == NM_SLOT_CLICK )		{	}//InvenBgSlotClick( EMsg );		}
		break;
	case WID_Inven_ItemSlot0:
	case WID_Inven_ItemSlot1:
	case WID_Inven_ItemSlot2:
	case WID_Inven_ItemSlot3:
	case WID_Inven_ItemSlot4:
	case WID_Inven_ItemSlot5:
	case WID_Inven_ItemSlot6:
	case WID_Inven_ItemSlot7:
	case WID_Inven_ItemSlot8:
	case WID_Inven_ItemSlot9:
	case WID_Inven_ItemSlot10:
	case WID_Inven_ItemSlot11:
	case WID_Inven_ItemSlot12:
	case WID_Inven_ItemSlot13:
	case WID_Inven_ItemSlot14:
	case WID_Inven_ItemSlot15:
	case WID_Inven_ItemSlot16:
	case WID_Inven_ItemSlot17:
	case WID_Inven_ItemSlot18:
	case WID_Inven_ItemSlot19:
	case WID_Inven_ItemSlot20:
	case WID_Inven_ItemSlot21:
	case WID_Inven_ItemSlot22:
	case WID_Inven_ItemSlot23:
	case WID_Inven_ItemSlot24:
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��
	case WID_PayInven_ItemSlot0:
	case WID_PayInven_ItemSlot1:
	case WID_PayInven_ItemSlot2:
	case WID_PayInven_ItemSlot3:
	case WID_PayInven_ItemSlot4:
	case WID_PayInven_ItemSlot5:
	case WID_PayInven_ItemSlot6:
	case WID_PayInven_ItemSlot7:
	case WID_PayInven_ItemSlot8:
	case WID_PayInven_ItemSlot9:
	case WID_PayInven_ItemSlot10:
	case WID_PayInven_ItemSlot11:
	case WID_PayInven_ItemSlot12:
	case WID_PayInven_ItemSlot13:
	case WID_PayInven_ItemSlot14:
	case WID_PayInven_ItemSlot15:
	case WID_PayInven_ItemSlot16:
	case WID_PayInven_ItemSlot17:
	case WID_PayInven_ItemSlot18:
	case WID_PayInven_ItemSlot19:
	case WID_PayInven_ItemSlot20:
	case WID_PayInven_ItemSlot21:
	case WID_PayInven_ItemSlot22:
	case WID_PayInven_ItemSlot23:
	case WID_PayInven_ItemSlot24:
#endif //WIDE_PAY_INVENTORY
		{
			if( g_ExchangeWnd.m_bIsExchangeLock == FALSE && g_UserStoreSellWnd.m_UserStoreSellOpenMode == FALSE ) 
			{
				if( EMsg->Notify == NM_SLOT_FOCUS )			{	InvenSlotFocus( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_PRESS )	{	InvenSlotFocus( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_CLICK )	{	InvenSlotClick( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	InvenSlotDBClick( EMsg );	}
				else if( EMsg->Notify == NM_SLOT_DRAG )		{	InvenSlotDrag( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DROP )		{	InvenSlotDrop( EMsg );		}

			}
		}		
		break;
	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::SetWheelValue( short _wheel )
{
	int value = 0;

	ScrollBar->GetValue( &value );
	if( ( _wheel < 0 ) && ( value < 7 ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	ScrollBar->SetValue( value );
	SetSlotScroll( value );
}


//-----------------------------------------------------------------------------
void CInventoryWnd::UpdateInvenSlotItem()
{
	int i = 0;
	m_SlotPosition = 0 ; 
	

	if( InvenState == n_INVEN_ITEM )
	{
		for( i = 0; i < c_MaxInventoryItemDrawNum; ++i )
		{
			InvenSlot[i]->PcItem = PcInven->InvenItem[StartSlotNum + i];
		}
	}
	else if( InvenState == n_INVEN_QUEST )
	{
		for( i = 0; i < c_MaxInventoryItemDrawNum; ++i )
		{
			InvenSlot[i]->PcItem = PcInven->InvenQuest[i];
		}
	}
	else if( InvenState == n_INVEN_HORSE )
	{
		for( i = 0; i < c_MaxInventoryItemDrawNum; ++i )
		{
			InvenSlot[i]->PcItem = PcInven->InvenHorse[i];
		}
	}
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��
	
	for( i = 0; i < c_MaxPayInvenItemDrawNum; ++i )
	{
		m_ItemSlot[i]->PcItem = PcInven->InvenItem[(PayStartSlotNum + i)+RP_MAX_INVENTORY];
	}
	
#endif //WIDE_PAY_INVENTORY

	///-- Inven ���濡 ���� ExchangeWnd �� ó��
	if( g_ExchangeWnd.IsVisible ) 
	{
		g_ExchangeWnd.CheckExChangeList();
	}

	///-- Inven ���濡 ���� EnchantWnd �� ó��
	if( g_ItemEnchantWndProc.IsVisible ) 
	{
		g_ItemEnchantWndProc.CheckEnchantItemList();
	}

	///-- Inven ���濡 ���� ExtractWnd �� ó��
	if( g_ItemExtract_WndProc.IsVisible )
	{
		g_ItemExtract_WndProc.CheckExtract_ItemList();
	}

	///-- Inven ���濡 ���� RemodelWnd �� ó��
	if( g_ItemRemodel_WndProc.IsVisible )
	{
		g_ItemRemodel_WndProc.CheckRemodel_ItemList();
	}
 
	///-- Inven ���濡 ���� RepairWnd �� ó��
	if( g_ItemRepair_WndProc.IsVisible )
	{
		g_ItemRepair_WndProc.CheckRepair_ItemList();
	}
	
	///-- Inven ���濡 ���� CreateWnd �� ó��
	if( g_ItemCreate_WndProc.IsVisible )
	{
		g_ItemCreate_WndProc.CheckCreate_ItemList();
	}

	///-- Inven ���濡 ���� MapBatch â�� ó��
	if( g_ColMapPositionWnd->IsVisible )
	{
		g_ColMapPositionWnd->Check_BatchItem();
	}
	
	///-- Inven ���濡 ���� ��ű� �ֹ�â�� ó��
	if( g_pAcryEnchantWnd && g_pAcryEnchantWnd->IsVisible )
	{
		g_pAcryEnchantWnd->CheckAEnchant_ItemList();
	}
	
	///-- Inven ���濡 ���� ���� ���󺯰�â�� ó��
	if( g_pChangeEldaColorWnd && g_pChangeEldaColorWnd->IsVisible )
	{
		g_pChangeEldaColorWnd->CheckElda_ItemList();
	}
#ifdef DECO_RENEWAL_GUILD_MJH
	if( g_ProposeJoinFrontier.IsVisible )
	{
		g_ProposeJoinFrontier.CheckProposeFrontierItemList();
	}			
#endif // DECO_RENEWAL_GUILD_MJH

	return;
}
// �����κ� ������ ����Ʈ �׷��ֱ�
void CInventoryWnd::SetPayInvenPcItem( BYTE aItemNum, URpdItem aItemList[] )
{
	int i = 0;
	char eldstr[256] = {0,};

	//m_Eld = aEld; 	
	//InventoryWnd.SetEldText( eldstr, m_Eld );
	//m_EldText->SetText( eldstr );

	int ItemPos = 0; 

	for( i = 0; i < aItemNum; ++i )
	{	
		ItemPos = aItemList[i].theItem.thePosition;
		
		SPcItem* addPcItem = NULL ;
		addPcItem = SAFE_NEW( SPcItem );
		//addPcItem = &m_PayInven[ItemPos];
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( aItemList[i].theItem.theCode );
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
		{
			addPcItem->IType		= n_ITYPE_WEAR;
			addPcItem->Amount		= 1;
			
			memcpy( &(addPcItem->WearTable), &aItemList[i], sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			addPcItem->IType	= n_ITYPE_USE;//�Ҹ�ǰ
			addPcItem->Amount	= aItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			addPcItem->IType	= n_ITYPE_UNUSE;//��Ҹ�ǰ
			addPcItem->Amount	= aItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
		{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= aItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_PersonalPet )
		{
			addPcItem->IType	= n_ITYPE_PET_USE; 
			addPcItem->Amount	= aItemList[i].theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			addPcItem->IType	= n_ITYPE_RIDE; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &aItemList[i], sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{	
			addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			addPcItem->Amount	= 1;

			memcpy( &(addPcItem->WearTable), &aItemList[i], sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{	
			addPcItem->IType	= n_ITYPE_SKIN_WEAR; 
			addPcItem->Amount	= 1;
			memcpy( &(addPcItem->WearTable), &aItemList[i], sizeof( URpdItem ) );
		}
		
		


		addPcItem->Class		= ItemType;
		addPcItem->ItemUID		= aItemList[i].theItem.theItemUID;
		addPcItem->Code			= aItemList[i].theItem.theCode;
		addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aItemList[i].theItem.theCode );
		addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aItemList[i].theItem.theCode );
		
		// �޼�,������....
		//---------------------------------------------------------------------
		if( ( addPcItem->ItemTable->theType == n_Type_MagicBook ) || ( addPcItem->ItemTable->theType == n_Type_Bow ) || ( addPcItem->ItemTable->theType == n_Type_Shield ) )
		{
			addPcItem->Class = n_Class_Shield;
		}
		else if( ( addPcItem->ItemTable->theType == n_Type_Sword ) || ( addPcItem->ItemTable->theType == n_Type_TwohandSword ) ||
			( addPcItem->ItemTable->theType == n_Type_Staff ) || ( addPcItem->ItemTable->theType == n_Type_CrystalBall ) )
		{
			addPcItem->Class = n_Class_Weapon;
		}
		//---------------------------------------------------------------------					
		
#ifdef C_TIME_SUIT_UPDATE		
		if(addPcItem->ItemTable &&  addPcItem->ItemTable->theRare == 255 && addPcItem->ItemTable->theMaxEndurance == 0 )
		{
			DWORD dwTime = 0 ; 
			for( int j = 0; j < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++j )
			{
				if(nRui->GetPcInvenInfo()->PayTimeItemList[j] == NULL)
				{
					//�ΰ� ������ �κ� ������ �� �ִ°�. . 
					nRui->GetPcInvenInfo()->PayTimeItemList[j] = SAFE_NEW( SPayTimeItem );
					nRui->GetPcInvenInfo()->PayTimeItemList[j]->theItemCode = aItemList[i].theItem.theCode;
					nRui->GetPcInvenInfo()->PayTimeItemList[j]->theItemID = aItemList[i].theItem.theItemUID;
					memcpy(&dwTime , &aItemList[i].theWearItem.theEndurance , sizeof(DWORD));
					nRui->GetPcInvenInfo()->PayTimeItemList[j]->theTime =dwTime ;
					break;
				}
			}	
		}
#endif
	PcInven->InvenItem[ItemPos]	= addPcItem ;

	}
	UpdateInvenSlotItem();
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
void CInventoryWnd::UpdateWearSlotItem()
{
	for( int i = 0; i < RP_MAX_EQUIP ; ++i )
	{
		WearSlot[i]->PcItem = PcInven->WearItem[i];
	}
	
	///-- Inven ���濡 ���� RepairWnd �� ó��
	if( g_ItemRepair_WndProc.IsVisible )
	{
		g_ItemRepair_WndProc.CheckRepair_ItemList();
	}
}

//-----------------------------------------------------------------------------

void CInventoryWnd::SetInvenState( N_INVENSTATE state )
{
	int i = 0;
	InvenState = state;
	
	StartSlotNum	= 0;
	ScrollBar->SetValue( StartSlotNum );

	if( InvenState == n_INVEN_ITEM )
	{
		TabType = n_InvenTab;
		for( i = 0; i < c_MaxInventoryItemDrawNum; ++i )
		{
			InvenSlot[i]->PcItem = PcInven->InvenItem[StartSlotNum + i];
		}
	}
	else if( InvenState == n_INVEN_QUEST )
	{
		TabType = n_QuestTab;
		for( i = 0; i < c_MaxInventoryItemDrawNum; ++i )
		{
			InvenSlot[i]->PcItem = PcInven->InvenQuest[i];
		}
	}
	else if ( InvenState == n_INVEN_HORSE)
	{
		TabType = n_RidingTab;
	
		for( i = 0; i < c_MaxInventoryItemDrawNum; ++i )
		{
			InvenSlot[i]->PcItem = PcInven->InvenHorse[i];
		}

	}
}
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��

void CInventoryWnd::SetPayInvenState( N_PAYINVENSTATE state )
{

	int i = 0;
	m_PayInvenState = state;
	
	// ���� ������ ��ġ 
	PayStartSlotNum = m_PayInvenState * c_MaxPayInvenItemDrawNum;

	if( m_PayInvenState == n_PAYINVEN_SHOP1 )
	{	
		TabType = n_InvenTab;
		for( i = 0; i < c_MaxPayInvenItemDrawNum; ++i )
		{
			m_ItemSlot[i]->PcItem = PcInven->InvenItem[(PayStartSlotNum + i)+RP_MAX_INVENTORY];
		}
	}

	UpdateInvenSlotItem();

	return;
}

#endif //WIDE_PAY_INVENTORY
//-----------------------------------------------------------------------------
void CInventoryWnd::SetSlotScroll( int linenum )
{
	int i = 0;
	
	
	StartSlotNum = linenum * 5;

	if( InvenState == n_INVEN_ITEM )
	{
		for( i = 0; i < c_MaxInventoryItemDrawNum; ++i )
		{
			InvenSlot[i]->PcItem = PcInven->InvenItem[StartSlotNum + i];
		}
	}
	/*
	else if( InvenState == n_INVEN_QUEST )
	{
		for( i = 0; i < c_MaxInventoryItemDrawNum; ++i )
		{
			InvenSlot[i]->PcItem = PcInven->InvenQuest[i];
		}
	}
	//*/
}
void CInventoryWnd::SetSrcSlot(CSlot* Src)
{
	m_SrcSlot	 = Src;

}

void CInventoryWnd::SetTargetSlot(CSlot* Target)
{
	m_TargetSlot = Target;
}


//-----------------------------------------------------------------------------
void CInventoryWnd::WearSlotClick( SEventMessage* EMsg )
{
	CSlot*		cslot		= NULL;//���� ��� ����
	CSlot*		pslot		= NULL;//���õ�(pick) �κ� ����
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;

	if( nRui->GetCursorItem() != NULL )			//Ŀ���� �������� �����Ұ��
	{
		cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		pslot = nRui->GetItemSelectedSlot();
		if( pslot->PcItem == NULL )
		{
			return;
		}
		
		// �κ�â���� ���� ��� �����۸� ó��
		if( cslot->PcItem && (cslot->PcItem == pslot->PcItem) )
		{
			// �������� ���Ƶ� ���λ��� â ��Ͽ��� �����ش�. By wxywxy
			if( pslot->Group == n_SGROUP_USER_STORE_SELL )
			{
				g_UserStoreSellWnd.DeleteItem( pslot );
			}

			nRui->ResetCursorItem();
		}
		// ���� ���� ���
		else if( pslot->Group == n_SGROUP_USER_STORE_BUY )
		{
			//g_UserStoreSetBuyWnd.SetItemInfo( pslot->PcItem , g_UserStoreBuyWnd.m_PriceList[ pslot->SlotNum + ( g_UserStoreBuyWnd.m_TopSlotIndex * 2 ) ] );
			//g_UserStoreSetBuyWnd.Open();

			nRui->ResetCursorItem();
		}
		// ���� ���� �ȱ�
		else if( pslot->Group == n_SGROUP_USER_STORE_SELL )
		{
			g_UserStoreSellWnd.DeleteItem( pslot );
		}		
		else if( ( pslot->Group == n_SGROUP_INVENTORY ) && ( pslot->PcItem->IType == n_ITYPE_WEAR ) && 
			g_UserStoreSellWnd.ISUserSellMode() == FALSE )
		{
			
			//���� üũ ��ƾ
			WORD Citizen = (WORD)(pslot->PcItem->ItemTable->theCitizen);
			if( RAIN == Citizen|| MILLENA == Citizen )	//�������� ���� ���� �������̶��
			{
				if( Citizen != nRui->thePcParam.Citizen )		//�÷��̾� ���� �������� �ƴ϶��
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OTHER_NATION_ITEM )  );	
					return;
				}
			}		
			
			SItemBaseInfo * pItemBaseInfo = pslot->PcItem->ItemTable;
			if(pItemBaseInfo)
			{
				if( !(!pItemBaseInfo->theFileCode1 && !pItemBaseInfo->theFileCode2) )
				{
					int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();			
					DWORD dFileCode =  g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, 
													pslot->PcItem->ItemTable->theCode );
					if(0 == dFileCode)
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTSEX_WEAR ) ); 
						return;
					}	
				}
			}		
			
			///-- ���õ� �������� ��Ʈ �������� ���
			int nType = pslot->PcItem->ItemTable->theType;
			//���� �����̳� ??
			if( n_Type_SetCloth == nType )
			{
				//���� ����(����,����,�ǵ鷿,����) �� �������� ���� �ϸ� �������� ���ϰ� �Ѵ�.
				if( WearSlot[n_SlotIdx_Coat]->PcItem || WearSlot[n_SlotIdx_Pants]->PcItem || 
					WearSlot[n_SlotIdx_Glove]->PcItem || WearSlot[n_SlotIdx_Boots]->PcItem )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, 
					G_STRING_CHAR( ISD_SYSPR_WEAR_WITHOUT_OTHERWEAR ) );	
					return;
				}
			}
			else	//���õ� �������� ��Ʈ �������� �ƴҰ�� 
			{
				//���õ� �������� ���� Ÿ���� üũ 
				int nTdx = GetWearIndex( pslot->PcItem->Class );
				//���õ� �������� ����,����,�ǵ鷿,���� �ϰ��
				if( nTdx == n_SlotIdx_Coat || nTdx == n_SlotIdx_Pants || nTdx == n_SlotIdx_Glove ||
					nTdx == n_SlotIdx_Boots )
				{
					//���� ���Կ� �ִ� �������� ��Ʈ �������� ��� �������� ���ϰ��Ѵ�.
					if(  WearSlot[n_SlotIdx_Coat]->PcItem && 
						( n_Type_SetCloth == WearSlot[n_SlotIdx_Coat]->PcItem->ItemTable->theType ) )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, 
						G_STRING_CHAR( IDS_SYSPR_WEAR_WITHOUT_SETITEM ) );					
						return;
					}
				}
			}			
			
			//�������ۿ� �´� �����̸� ü����....
			if( ( pslot->PcItem->Class == cslot->Class ) && 
				( ( pslot->PcItem->ItemTable->theCitizen == PcParam->Citizen ) || ( pslot->PcItem->ItemTable->theCitizen == 2 ) ) )	// ���߿� �������� ���� ����� �����ϸ� ���ڴ�...���� 
			{
				// ����̳� Ȱ �� �μտ� ��� ��� �ִ� �������� üũ�ؼ� ��� ���� ���ѵξ�� �Ѵ�.
				if( ( cslot->SlotNum == 2 ) || ( cslot->SlotNum == 3 ) )
				{
					if( WearSlot[n_SlotIdx_Weapon]->PcItem != NULL ) //������ ���� �˻�

					{
						///-- ��հ�, ������, �������� ������� ��� ���� �������� �ƴ϶��...
						///-- ���Ͻ�Ų��..					
						
						if( ( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_Sword ) ||
							( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_TwohandSword ) ||
							( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_CrystalBall ) ||
							( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_Staff ) )
						{
							if( pslot->PcItem->ItemTable->theType == n_Type_Sword ||
								pslot->PcItem->ItemTable->theType == n_Type_TwohandSword ||
								pslot->PcItem->ItemTable->theType == n_Type_CrystalBall ||
								pslot->PcItem->ItemTable->theType == n_Type_Staff )
							{
							}
							else
							{
								SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP ) );
//								nRui->TextOutChat( _RT("SYSTEM_MSG : ���� ��� ������ �������� �������� ������ �� �����ϴ�."), D3DCOLOR_XRGB( 255, 255, 0 ) );
								return;
							}							
						}
					}
					
					///-- Ȱ å�� ������� ��� ���� �������� �ƴ϶��...
					///-- ���Ͻ�Ų��..					
					if( WearSlot[n_SlotIdx_Shield]->PcItem != NULL ) //�޼� ���� �˻�
					{
						if( ( WearSlot[n_SlotIdx_Shield]->PcItem->ItemTable->theType == n_Type_MagicBook ) ||
							( WearSlot[n_SlotIdx_Shield]->PcItem->ItemTable->theType == n_Type_Bow ) )
						{
							if( pslot->PcItem->ItemTable->theType == n_Type_MagicBook ||
								pslot->PcItem->ItemTable->theType == n_Type_Bow )
							{
							}
							else
							{
								SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP ) );
//								nRui->TextOutChat( _RT("SYSTEM_MSG : ���� ��� ������ �������� �������� ������ �� �����ϴ�."), D3DCOLOR_XRGB( 255, 255, 0 ) );
								return;
							}
						}
					}					
				}

				bool result = CheckWearPassiveSkill( pslot->PcItem->ItemTable );
				if( result == false )
				{					
					if( nRui->GetPcParamInfo()->Level < pslot->PcItem->ItemTable->theLimitLevel )						
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CONFIRM_ITEMLEV_CHARACTERLEV ) ) ;				
					}
					else
					{
						SSkillBaseInfo* skill_info = NULL;
						skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( pslot->PcItem->ItemTable->theNeedSkill );

						if( skill_info )
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_EQUIP ), skill_info->theSkillName );				
						}
					}
					
					return;
				}

				tmpPcItem = pslot->GetSlotItem();

				//��� ���� ��û
				//position = StartSlotNum + pslot->SlotNum;//
				position = cslot->SlotNum;//
				g_RockClient.Send_RpCsEquip( tmpPcItem->ItemUID, position );
			}
		}
		else if( ( pslot->Group == n_SGROUP_INVENTORY ) && 
			     ( pslot->PcItem->IType == n_ITYPE_SKIN_WEAR ) && 
			      g_UserStoreSellWnd.ISUserSellMode() == FALSE )
		{
			//�������ۿ� �´� �����̸� ü����....
			if(  pslot->PcItem->Class == cslot->Class  )
			{
				
				SItemBaseInfo * pItemBaseInfo = pslot->PcItem->ItemTable;
				if(pItemBaseInfo)
				{
					if( !(!pItemBaseInfo->theFileCode1 && !pItemBaseInfo->theFileCode2) )
					{
						int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();			
						DWORD dFileCode =  g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, 
							pslot->PcItem->ItemTable->theCode );
						if(0 == dFileCode)
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTSEX_WEAR ) ); 
							return;
						}	
					}
				}

				tmpPcItem = pslot->GetSlotItem();
				//��� ���� ��û
				//position = StartSlotNum + pslot->SlotNum;//
				position = cslot->SlotNum;//
				g_RockClient.Send_RpCsEquip( tmpPcItem->ItemUID, position );
			}
		}

	
		else if( ( pslot->Group == n_SGROUP_INVENTORY ) && 
			     ( pslot->PcItem->IType == n_ITYPE_RIDE ) && 
			      g_UserStoreSellWnd.ISUserSellMode() == FALSE )
		{

			//�������ۿ� �´� �����̸� ü����....
			if(  pslot->PcItem->Class == cslot->Class  )
			{
				if(g_Pc.GetPlayer()->m_State.bRide)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_CHANGE_RS_INRIDING ) );		
					return;
				}
				
				tmpPcItem = pslot->GetSlotItem();

				//��� ���� ��û
				//position = StartSlotNum + pslot->SlotNum;//
				position = cslot->SlotNum;//
				g_RockClient.Send_RpCsEquip( tmpPcItem->ItemUID, position );
			}
		}
		else if( ( pslot->Group == n_SGROUP_INVENTORY ) && 
			     ( pslot->PcItem->IType == n_ITYPE_RIDE_WEAR ) && 
			      g_UserStoreSellWnd.ISUserSellMode() == FALSE )
		{
			if( pslot->PcItem->Class == cslot->Class )
			{
				if( pslot->PcItem->ItemTable )
				{
					///-- ���õ� ���̵��������� ��Ʈ �������ϰ��
					int nType = pslot->PcItem->ItemTable->theType;
					if( n_Type_Riding_Full == nType )
					{
						//���̵� ����Կ� �������� ���� �ϸ� �������� ���ϰ� �Ѵ�.
						if( WearSlot[n_Slotidx_Horse_Top]->PcItem || WearSlot[n_Slotidx_Horse_Middle]->PcItem || 
							WearSlot[n_Slotidx_Horse_Bottom]->PcItem  )
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_REMOVE_RIDEITEM ) );  
							return;
						}
					}
					else
					{
						//���õ� �������� ���� Ÿ���� üũ
						int nTdx = GetWearIndex( pslot->PcItem->Class );
						//���õ� �������� ����,����,�ǵ鷿,���� �ϰ��
						if( nTdx == n_Slotidx_Horse_Top || nTdx == n_Slotidx_Horse_Middle 
							|| nTdx == n_Slotidx_Horse_Bottom )
						{	//�߰� ���Կ� �ִ� �������� ��Ʈ �������� ��� �������� ���ϰ��Ѵ�.
							if(  WearSlot[n_Slotidx_Horse_Middle]->PcItem && 
								( n_Type_Riding_Full == WearSlot[n_Slotidx_Horse_Middle]->PcItem->ItemTable->theType ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_REMOVE_RIDESETITEM ) );	  		
								return;
							}
						}
					}					
					
					if( nType == n_Type_Riding_Full )
					{
					   nType = n_Type_Riding_Saddle;	
					}

#ifdef RIDING_SYSTEM					
					if( cslot->Type == nType )
					{
						if( nRui->thePcInven.WearItem[n_WearSlot_Riding] == NULL )
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_RITEM_NORIDESLOT ) );		
							return;
						}

						DWORD nRidingCode = nRui->thePcInven.WearItem[n_WearSlot_Riding]->Code;
						SItemBaseInfo* item_Info = NULL;
						item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRidingCode );

						if( item_Info )
						{
							DWORD dNPCCode = item_Info->theReHP;
							SNPCBaseInfo* npc_Info = NULL;
							npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

							if( npc_Info )
							{
								///-- �ش� ���̵� �������� �����Ҽ� ���� ����̶��...
								if( pslot->PcItem->ItemTable->theUpgradeForm != npc_Info->theTargetRange )
								{
									SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYS_NOWRIDINGCANNOTEQUIPITEM ) );  
									return;
								}
							}				
						}

						tmpPcItem = pslot->GetSlotItem();

						position = cslot->SlotNum;			
						g_RockClient.Send_RpCsEquip( tmpPcItem->ItemUID, position );
					}
#endif
				}
			}
		}		
		else
		{
//			nRui->TextOutChat( _RT("SYSTEM_MSG : �ٸ� ��� or ��� �̿��� �������� �̵��Ұ�"), D3DCOLOR_XRGB( 255, 255, 0 ) );
		}
	}
	else
	{
		//Ŀ���� ������ ���̱�

		cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		
		nRui->SetCursorItem( cslot->PcItem );
		nRui->SetItemSelectedSlot( cslot );
	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::WearSlotDBClick( SEventMessage* EMsg )
{
	if( g_UserStoreSellWnd.ISUserSellMode() != FALSE )
		return;
	
	// ���� Ŭ���� �ϸ� �ڵ����� ����������?!!...
	// ����ִ� �������� �ڵ����� ��� �ű�� 
	// �κ�â�� �� �� ������ ����� �ȵǰ� ���� 
	
	CSlot* cslot = (CSlot*)Container->Get( EMsg->FocusWnd );
	
	if( cslot && cslot->Class == n_Class_Riding )
	{
		if( cslot->PcItem != NULL && g_Pc.GetPlayer()->m_State.bRide)
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_REMOVE_TS_INRIDING ) );		
			return;
		}
	}

	// ��� �����ϰ� �ִٸ� 
	if( cslot->PcItem != NULL )
	{
		for( int i = 0; i < c_MaxInvenItemNum  ; ++i )		
		{
			if( PcInven->InvenItem[i] == NULL )
			{
				g_RockClient.Send_RpCsTakeoff( cslot->PcItem->ItemUID, i );				
				return;
			}
		}				
	}

	return;
}

//-----------------------------------------------------------------------------
void CInventoryWnd::WearSlotDrag( SEventMessage* EMsg )
{
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );
	
	if( slot && slot->Class == n_Class_Riding )
	{
		if( slot->PcItem != NULL && g_Pc.GetPlayer()->m_State.bRide)
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_REMOVE_TS_INRIDING ) );		
			return;
		}
	}	
	
	if( ( slot->PcItem != NULL ) && ( EMsg->FocusWnd != (WNDID)EMsg->fParam ) )
	{
		//Ŀ���� ������ ���̱�

		if( nRui->GetCursorItem() == NULL )
		{
			nRui->SetCursorItem( slot->PcItem );
			nRui->SetItemSelectedSlot( slot );
		}
	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::WearSlotDrop( SEventMessage* EMsg )
{
	CSlot* cslot = (CSlot*)Container->Get( EMsg->FocusWnd );
	CSlot* tslot = (CSlot*)Container->Get( (WNDID)EMsg->fParam );

	int			position = 0;
	

	if( ((WNDID)EMsg->fParam != WID_None ) && ( tslot->GetWindowType() == n_wtSlot ) )
	{
		if( cslot == tslot )
		{
			nRui->ResetCursorItem();
		}
		else if( cslot->PcItem != NULL )
		{
			if( ( tslot->Group == n_SGROUP_INVENTORY ) && ( tslot->PcItem == NULL ) )
			{
				if( InvenState == n_INVEN_ITEM && g_UserStoreSellWnd.ISUserSellMode() == FALSE )
				{
					//������..�κ� â�̰� ���������쿡�� �̵�
					//��� ���� ��û
					position = StartSlotNum + tslot->SlotNum;
					g_RockClient.Send_RpCsTakeoff( cslot->PcItem->ItemUID, position );//
				}
				else
				{
					//n_INVEN_QUEST�� �̵� �Ұ�....
				}
			}
			else if( tslot->Group == n_SGROUP_STORE && g_UserStoreSellWnd.ISUserSellMode() == FALSE )
			{
				//���� �ȱ�
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SELL_IN_WEAR ) );				
				//StoreSellWnd.SetSellItemInfo( cslot->PcItem, n_WearTab );

			}
			// ���� ���� 
			else if( tslot->Group == n_SGROUP_USER_STORE_SELL )
			{
				g_UserStoreSellWnd.InsertItem( tslot , cslot );
			}
		}
		else
		{
			//cslot->PcItem == NULL �̸�...�ű� �������� ����.
		}
	}

	nRui->ResetCursorItem();
}

//-----------------------------------------------------------------------------
void CInventoryWnd::InvenBgSlotClick( SEventMessage* EMsg )
{
	CSlot*	cslot	= NULL;//���� ��� ����
	CSlot*	pslot	= NULL;//���õ�(picked) �κ� ����

	if( nRui->GetCursorItem() != NULL )
	{
		cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		pslot = nRui->GetItemSelectedSlot();
		if( pslot->PcItem == NULL )
		{
			return;
		}
				
		if( cslot->PcItem && (cslot->PcItem == pslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		//..................................................................................................
		// ���� ���� ��� 
		//..................................................................................................
		else if( pslot->Group == n_SGROUP_USER_STORE_BUY )
		{
			g_UserStoreSetBuyWnd.SetItemInfo( pslot->PcItem , g_UserStoreBuyWnd.m_PriceList[ pslot->SlotNum + ( g_UserStoreBuyWnd.m_TopSlotIndex * 2 ) ] );
			g_UserStoreSetBuyWnd.Open();
			nRui->ResetCursorItem();
		}
		else if( pslot->Group == n_SGROUP_STORE )
		{
			if( pslot->PcItem->Amount > 1)
			{
				g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_SELLSTORE, pslot->PcItem );
			}
			else
			{
				if( cslot->Group == n_SGROUP_INVENTORY )
				{
					StoreSellWnd.SetSellItemInfo( pslot->PcItem, n_InvenTab );
				}
				else if( cslot->Group == n_SGROUP_WEAR )
				{
					StoreSellWnd.SetSellItemInfo( pslot->PcItem, n_WearTab );
				}
			}

			nRui->ResetCursorItem();
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_BUY ) );
//			nRui->TextOutChat( _RT("SYSTEM_MSG : ������ ���Ը� �� �� �����ϴ�."), D3DCOLOR_XRGB( 255, 255, 0 ) );
		}
	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::InvenSlotFocus( SEventMessage* EMsg )
{
	static CSlot* cslot = NULL;
	
	cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
	if( ( InvenState == n_INVEN_ITEM ) && ( cslot->PcItem != NULL ) && ( nRui->GetCursorItem() == NULL ) )
	{
		
		nRui->theCursorState = n_CT_HAND;
	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::InvenSlotClick( SEventMessage* EMsg )
{
	int			idx		= 0;
	CSlot*		cslot	= NULL;//Ŭ���� �κ� ����
	CSlot*		pslot	= NULL;//���õ�(pick) ��� ����
	SPcItem*	tmpItem	= NULL;

	if( nRui->GetCursorItem() != NULL )
	{
		cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		pslot = nRui->GetItemSelectedSlot();

		m_SlotPosition = cslot->SlotNum + StartSlotNum;

		if( pslot->PcItem == NULL )
		{
			return;
		}

		if( cslot->PcItem && (cslot->PcItem == pslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		else
		{
			if( InvenState == n_INVEN_ITEM )
			{
				// ���� ���� ���
				if( pslot->Group == n_SGROUP_USER_STORE_BUY )
				{
					g_UserStoreSetBuyWnd.SetItemInfo( pslot->PcItem , g_UserStoreBuyWnd.m_PriceList[ pslot->SlotNum + ( g_UserStoreBuyWnd.m_TopSlotIndex * 2 ) ] );
					g_UserStoreSetBuyWnd.Open();
					nRui->ResetCursorItem();
				}
				else if( g_UserStoreSellWnd.ISUserSellMode() == FALSE )
				{	
					switch( pslot->Group )			// ���� Ŭ���� ������ ó�� �Ѵ�. 
					{
					case n_SGROUP_QUICK:			ClickQuickSlot();					break;//����â - ������
					case n_SGROUP_INVENTORY:		ClickInventory( cslot, pslot );		break;//����ǰâ - �κ��丮
					case n_SGROUP_WEAR:				ClickWear( cslot, pslot );			break;//����ǰâ - ����
						
					case n_SGROUP_EXCHANGE:			ClickExchange( cslot, pslot );		break;//��ȯâ - �� ��������
					case n_SGROUP_STORE:			ClickStore( pslot );				break;//����â
					case n_SGROUP_WAREHOUSE:		ClickBank( pslot );					break;//����â
#ifdef DECO_RENEWAL_GUILD_MJH
					case n_SGROUP_GUILDINVENTORY:	ClickGuild( pslot );				break;//���â��
#endif // DECO_RENEWAL_GUILD_MJH
					case n_SGROUP_TUNING:			ClickTuning();						break;//����â
					case n_SGROUP_SKILL:			ClickSkill();						break;//��ųâ
					case n_SGROUP_PAYITEM_WND:		ClickPayItemWnd( pslot );			break;//���� ������ ����Ʈ â 
					case n_SGROUP_PET_INVENTORY:    ClickPetInven(pslot , cslot );		break;//���κ� -> PC�κ� 


					}
				}
				else
				{
					if( pslot->Group == n_SGROUP_USER_STORE_SELL )
					{
						g_UserStoreSellWnd.DeleteItem( pslot );
					}
					else
					{
						nRui->ResetCursorItem();
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_USE ) );
					}
				}
			}
			else if( InvenState == n_INVEN_QUEST )
			{
				return;

				if( pslot->PcItem->Class == n_Class_Quest )
				{
					ClickInventory( cslot, pslot );
				}
				else
				{
//					nRui->TextOutChat( "SYSTEM_MSG : ����Ʈ�ǿ����� ����Ʈ �����۸� ���� �� �ֽ��ϴ�.", 0xffffff00 );
				}
			}
			else if( InvenState == n_INVEN_HORSE )
			{
				switch( pslot->Group )			// ���� Ŭ���� ������ ó�� �Ѵ�. 
				{
					case n_SGROUP_INVENTORY:		ClickInventory( cslot, pslot );		break;//����ǰâ - �κ��丮				
				}
			}
		}
	}
	else
	{
		//Ŀ���� ������ ���̱� �޼���.
		cslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );

		if( ( cslot->Group == n_SGROUP_INVENTORY ) && ( InvenState == n_INVEN_QUEST ) )
		{
			return;
		}
		
		if( ( cslot->PcItem != NULL ) && ( cslot->PcItem->IType == n_ITYPE_RIDE_WEAR ) && 
			( InvenState == n_INVEN_HORSE ) )
		{
			return;
		}

		nRui->SetCursorItem( cslot->PcItem );
		nRui->SetItemSelectedSlot( cslot );

		nRui->theCursorState = n_CT_ITEMDROP;

		//��� �������̸�... �ش� ��� ������ ���̶���Ʈ ���ش�.
		if( ( cslot->PcItem != NULL ) && 
			( cslot->PcItem->IType == n_ITYPE_WEAR || cslot->PcItem->IType == n_ITYPE_RIDE ) )
		{
			idx = GetWearIndex( cslot->PcItem->Class );
			if( idx == n_ICLASS_NONE )
				return;
			
			if( idx == n_SlotIdx_Accessory1 )
			{
				//�Ǽ��縮 2��...GetWearIndex�� ���� ù��° �ε����� �����Ѵ�.
				WearSlot[idx]->SetHighlight( true );
				WearSlot[idx+1]->SetHighlight( true );
			}
			else
			{
				WearSlot[idx]->SetHighlight( true );
			}
		}
		else if( ( cslot->PcItem != NULL ) && ( cslot->PcItem->IType == n_ITYPE_RIDE_WEAR ) )
		{
			if(cslot->PcItem->ItemTable)
			{
				idx = GetWearIndex( cslot->PcItem->Class, cslot->PcItem->ItemTable->theType );
				if( idx == n_ICLASS_NONE )
					return;				
				WearSlot[idx]->SetHighlight( true );			
			}
		}
		else if( (cslot->PcItem != NULL ) && (cslot->PcItem->IType == n_ITYPE_SKIN_WEAR ))
		{
			if(cslot->PcItem->ItemTable)
			{
				idx = GetWearIndex(cslot->PcItem->Class , cslot->PcItem->ItemTable->theType );
				if( idx == n_ICLASS_NONE)
					return;
				WearSlot[idx]->SetHighlight( true );
  
				SItemBaseInfo * pItemBaseInfo = cslot->PcItem->ItemTable;
				if(pItemBaseInfo)
				{
					if( !(!pItemBaseInfo->theFileCode1 && !pItemBaseInfo->theFileCode2) )
					{
						int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();			
						DWORD dFileCode =  g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, 
							cslot->PcItem->ItemTable->theCode );
						if(0 == dFileCode)
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTSEX_WEAR ) ); 
							return;
						}	
					}
				}

			}
		}



	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::InvenSlotDBClick( SEventMessage* EMsg )
{
	//������ ���
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );

	if( slot->PcItem )
	{
		if( g_UserStoreSellWnd.ISUserSellMode() != FALSE )
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_USE )  );						
			return;
		}
		
		///-- �������� ��ȯ �������ϰ�� 
		if( g_Pc_Manager.IsColonyItem(slot->PcItem->ItemTable) )
		{
			///-- �������� ��ȯ ������ ���
			if( !g_Pc_Manager.IsCanCallColonyItem() )
			{
				return;
			}			
			
			if( !g_ColMapPositionWnd->IsVisible )
			{
				UINT nCallType = slot->PcItem->ItemTable->theReHPSpeed;
				g_ColMapPositionWnd->Open();
				g_ColMapPositionWnd->SetColItem(slot->PcItem, nCallType);
			}
			
			return;
		}

		//���� üũ ��ƾ
		WORD Citizen = (WORD)(slot->PcItem->ItemTable->theCitizen);
		if( RAIN == Citizen|| MILLENA == Citizen)	//�������� ���� ���� �������̶��
		{
			if( Citizen != nRui->thePcParam.Citizen )		//�÷��̾� ���� �������� �ƴ϶��
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OTHER_NATION_ITEM )  );	
				return;
			}
		}
		
		if( (slot->PcItem->ItemTable->theReHP == n_Scroll_Expression) && 
			g_Pc.GetPlayer()->m_curt_event.type == SM_ACT )
		{
			LPACTION_DATA	pData = NULL;

			if(	( pData = g_RockClient.m_Action.GetData( slot->PcItem->ItemTable->theReSP ) ) == NULL )
				return;

			if( g_Pc.GetPlayer()->m_ani_index == pData->StartAni[0])
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EMOTION_NOT_USE ) );
				return;
			}

		}

		//�������� �ż��о�� �������� ���
		if(slot->PcItem->ItemTable && slot->PcItem->ItemTable->theClass == n_Class_FrontierPet )
		{
			if( nRui->GetPcInvenInfo()->FrontierInfo )
			{
				if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );									
				}
				else
				{
					g_StrInputWnd.Open( n_STRINPUT_FRONTIERPET_NAME, slot->PcItem->Code );
				}
			
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_IN_FRONTIER ) );
			}
			return;
		}
		
		
		///��ȫ ���� ... �ϴ� �̰ɷ� ���� ��ü ���� 
//
//		if(slot->PcItem->ItemTable && slot->PcItem->ItemTable->theCode == 6062)
//		{
//			//������ �̸����� â�� ��� .. 
//			g_PetCreateWnd.Open();
//			g_RockClient.Send_RpCsUse( slot->PcItem->ItemUID );		
//			return;
//		}


		///�������� �ް��� �������� ���
		
#ifdef MEGAPHONE_RENEW
		if(slot->PcItem->ItemTable && slot->PcItem->ItemTable->theType == n_Type_Scroll)
		{
			switch(slot->PcItem->ItemTable->theReHP)	
			{
				case n_Scroll_Channel_Shout:
					{
						g_ChatMainProc.m_ChatActPrintWnd.m_dUseShoutCode = slot->PcItem->ItemUID;
						g_ChatMainProc.m_ChatActPrintWnd.SetChatState(n_WholeChannelChat);
						g_ChatMainProc.SetOutState(COutState_Act);
						return;
					}
				case n_Scroll_AllChannel_Shout:
					{
						g_ChatMainProc.m_ChatActPrintWnd.m_dUseShoutCode = slot->PcItem->ItemUID;
						g_ChatMainProc.m_ChatActPrintWnd.SetChatState(n_WholeServerChat);
						g_ChatMainProc.SetOutState(COutState_Act);
						return;
					}
				case n_Scroll_Map_Shout:
					{
						g_ChatMainProc.m_ChatActPrintWnd.m_dUseShoutCode = slot->PcItem->ItemUID;
						g_ChatMainProc.m_ChatActPrintWnd.SetChatState(n_WholeAreaChat);
						g_ChatMainProc.SetOutState(COutState_Act);
						return;
					}
				
			}
		}
#else 
		if(slot->PcItem->ItemTable && (slot->PcItem->ItemTable->theCode == 11026 || 
			slot->PcItem->ItemTable->theCode == 14937 ) )
		{
			g_ChatMainProc.m_ChatActPrintWnd.m_dUseShoutCode = slot->PcItem->ItemUID;
			g_ChatMainProc.m_ChatActPrintWnd.SetChatState(n_WholeChannelChat);
			g_ChatMainProc.SetOutState(COutState_Act);
			return;
		}
		
		if(slot->PcItem->ItemTable && slot->PcItem->ItemTable->theCode == 11027)
		{
			g_ChatMainProc.m_ChatActPrintWnd.m_dUseShoutCode = slot->PcItem->ItemUID;
			g_ChatMainProc.m_ChatActPrintWnd.SetChatState(n_WholeServerChat);
			g_ChatMainProc.SetOutState(COutState_Act);
			return;
		}
		
		if(slot->PcItem->ItemTable && slot->PcItem->ItemTable->theCode == 11058 )
		{
			g_ChatMainProc.m_ChatActPrintWnd.m_dUseShoutCode = slot->PcItem->ItemUID;
			g_ChatMainProc.m_ChatActPrintWnd.SetChatState(n_WholeAreaChat);
			g_ChatMainProc.SetOutState(COutState_Act);
			return;
		}



#endif 
		
		if( slot->PcItem->IType == n_ITYPE_USE && slot->PcItem->ItemTable )		//�Ҹ� �������� ���
		{
			bool result = CheckUseItemPassiveSkill( slot->PcItem->ItemTable );
			
			if( result )
			{
				if(!MainMenuWnd.UseItem(slot))
				{
					return;
				}
			}
			else
			{				
				if( nRui->GetPcParamInfo()->Level < slot->PcItem->ItemTable->theLimitLevel )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CONFIRM_ITEMLEV_CHARACTERLEV ) ) ;				
				}
				else
				{					
					SSkillBaseInfo* skill_info = NULL;
					skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( slot->PcItem->ItemTable->theNeedSkill );

					if( skill_info )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_EQUIP ), skill_info->theSkillName );				
					}
				}
			}
		}
		///---------------------------------------------------------------------------
		///-- ���� ������ 
		else if( slot->PcItem->IType == n_ITYPE_WEAR && slot->PcItem->ItemTable )
		{			
			int idx = GetWearIndex( slot->PcItem->Class );
			bool result = CheckWearPassiveSkill( slot->PcItem->ItemTable );
			
			///-- ���� �Ұ�.
			if( result == false )
			{
			   if( nRui->GetPcParamInfo()->Level < slot->PcItem->ItemTable->theLimitLevel )
			   {
				   SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CONFIRM_ITEMLEV_CHARACTERLEV ) ) ;				
			   }
			   else
			   {
				   SSkillBaseInfo* skill_info = NULL;
				   skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( slot->PcItem->ItemTable->theNeedSkill );

				   if( skill_info )
				   {
					   SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_EQUIP ), skill_info->theSkillName );				
				   }
			   }
			   
			   return;			
			}
			
			SItemBaseInfo * pItemBaseInfo = slot->PcItem->ItemTable;
			if(pItemBaseInfo)
			{
				if( !(!pItemBaseInfo->theFileCode1 && !pItemBaseInfo->theFileCode2) )
				{
					int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();			
					DWORD dFileCode =  g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, 
													slot->PcItem->ItemTable->theCode );
					if(0 == dFileCode)
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTSEX_WEAR ) ); 
						return;
					}	
				}
			}
 
			///-- ���õ� �������� ��Ʈ �������� ���
			int nType = slot->PcItem->ItemTable->theType;

			if( n_Type_SetCloth == nType )
			{
				//���� ����(����,����,�ǵ鷿,����) �� �������� ���� �ϸ� �������� ���ϰ� �Ѵ�.
				if( WearSlot[n_SlotIdx_Coat]->PcItem || WearSlot[n_SlotIdx_Pants]->PcItem || 
					WearSlot[n_SlotIdx_Glove]->PcItem || WearSlot[n_SlotIdx_Boots]->PcItem )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, 
					G_STRING_CHAR( ISD_SYSPR_WEAR_WITHOUT_OTHERWEAR ) );	
					return;
				}

				m_IsSendSetItemEquip = TRUE;
				m_SendSetItmCheckTime = g_nowTime;
			}
			else	//���õ� �������� ��Ʈ �������� �ƴҰ��
			{
				if( m_IsSendSetItemEquip )
				{
					return;
				}				
				
				//���õ� �������� ���� Ÿ���� üũ 
				int nTdx = GetWearIndex( slot->PcItem->Class );
				//���õ� �������� ����,����,�ǵ鷿,���� �ϰ��
				if( nTdx == n_SlotIdx_Coat || nTdx == n_SlotIdx_Pants || nTdx == n_SlotIdx_Glove ||
					nTdx == n_SlotIdx_Boots )
				{	//���� ���Կ� �ִ� �������� ��Ʈ �������� ��� �������� ���ϰ��Ѵ�.
					if(  WearSlot[n_SlotIdx_Coat]->PcItem && 
						( n_Type_SetCloth == WearSlot[n_SlotIdx_Coat]->PcItem->ItemTable->theType ) )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, 
						G_STRING_CHAR( IDS_SYSPR_WEAR_WITHOUT_SETITEM ) );				
						return;
					}
				}
			}			
			
			if( WearSlot[idx]->PcItem )		//�ش� ���� ���� ���Կ� �������� �����Ұ��  	
			{
				//�ٲٷ��°� �׼����� 1 -> 1�ڸ��� �̹������ϰ� 2�ڸ��� ���������
				if( ( idx == n_SlotIdx_Accessory1 ) && 
					( WearSlot[n_SlotIdx_Accessory2]->PcItem == NULL ) 
				  )
				{
					g_RockClient.Send_RpCsEquip( slot->PcItem->ItemUID, WearSlot[n_SlotIdx_Accessory2]->SlotNum );
					return;
				}		

				// �ڵ� ��� �ٲ� n_SlotIdx_Weapon �Ǵ� n_SlotIdx_Shield�� ���
				// ������ ������ Ÿ�Կ� ���� Wear_Slot�� ��� ������ �ڵ� �ٲ� ^ ^V									
				if( ( WearSlot[idx]->SlotNum == 2 ) || ( WearSlot[idx]->SlotNum == 3 ) )
				{
					if( WearSlot[n_SlotIdx_Weapon]->PcItem != NULL )//������ ���� �˻�

					{
						if( ( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_Sword ) ||
							( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_TwohandSword ) ||
							( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_CrystalBall ) ||
							( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_Staff ) )
						{
							if( slot->PcItem->ItemTable->theType == n_Type_Sword ||
								slot->PcItem->ItemTable->theType == n_Type_TwohandSword ||
								slot->PcItem->ItemTable->theType == n_Type_CrystalBall ||
								slot->PcItem->ItemTable->theType == n_Type_Staff )
							{
							}
							else
							{
								SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP ) );
//								nRui->TextOutChat( _RT("SYSTEM_MSG : ���� ��� ������ �������� �������� ������ �� �����ϴ�."), D3DCOLOR_XRGB( 255, 255, 0 ) );
								return;
							}							
						}
					}
					else if( WearSlot[n_SlotIdx_Shield]->PcItem != NULL )//�޼� ���� �˻�

					{
						if( ( WearSlot[n_SlotIdx_Shield]->PcItem->ItemTable->theType == n_Type_MagicBook ) ||
							( WearSlot[n_SlotIdx_Shield]->PcItem->ItemTable->theType == n_Type_Bow ) )
						{
							if( slot->PcItem->ItemTable->theType == n_Type_MagicBook ||
								slot->PcItem->ItemTable->theType == n_Type_Bow )
							{
							}
							else
							{
								SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP ) );
//								nRui->TextOutChat( _RT("SYSTEM_MSG : ���� ��� ������ �������� �������� ������ �� �����ϴ�."), D3DCOLOR_XRGB( 255, 255, 0 ) );
								return;
							}
						}
					}					
				}

				if( idx == n_SlotIdx_Accessory1 )
				{
					static int nIndex = 0;					
					g_RockClient.Send_RpCsEquip( slot->PcItem->ItemUID, 
												 WearSlot[n_SlotIdx_Accessory1+nIndex]->SlotNum );					
					nIndex++;
					if(nIndex > 1)
					{
						nIndex = 0;
					}
					return;
				}

				g_RockClient.Send_RpCsEquip( slot->PcItem->ItemUID, WearSlot[idx]->SlotNum );
			}
			else	//�ش� ���� ���Կ�	�������� �������� �������
			{				
				// �ڵ� ���� 
				// ������ ������ Ÿ�Կ� ���� Wear_Slot�� ��� ���� ��쿡 �˾Ƽ� �ֱ� ^ ^V
				g_RockClient.Send_RpCsEquip( slot->PcItem->ItemUID, WearSlot[idx]->SlotNum );
			}						
		}
		else if  ( (slot->PcItem->IType == n_ITYPE_SKIN_WEAR) && slot->PcItem->ItemTable )
		{
			
			SItemBaseInfo * pItemBaseInfo = slot->PcItem->ItemTable;
			if(pItemBaseInfo)
			{
				if( !(!pItemBaseInfo->theFileCode1 && !pItemBaseInfo->theFileCode2) )
				{
					int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();			
					DWORD dFileCode =  g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, 
						slot->PcItem->ItemTable->theCode );
					if(0 == dFileCode)
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTSEX_WEAR ) ); 
						return;
					}	
				}
			}



			int idx = GetWearIndex( slot->PcItem->Class  , slot->PcItem->ItemTable->theType   );			
			g_RockClient.Send_RpCsEquip( slot->PcItem->ItemUID, WearSlot[idx]->SlotNum );
		}

		else if( ( slot->PcItem->IType == n_ITYPE_RIDE ) && slot->PcItem->ItemTable )
		{
			if(g_Pc.GetPlayer()->m_State.bRide)
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_CHANGE_RS_INRIDING ) );		
				return;
			}			
			
			int idx = GetWearIndex( slot->PcItem->Class );			
			g_RockClient.Send_RpCsEquip( slot->PcItem->ItemUID, WearSlot[idx]->SlotNum );
		}
		else if( ( slot->PcItem->IType == n_ITYPE_RIDE_WEAR ) && slot->PcItem->ItemTable)
		{		
			if(InvenState == n_INVEN_HORSE )	
			{
				bool bFlag = false ; 

				for( int i = 0; i < c_MaxInvenItemNum; ++i )		
				{
					if( PcInven->InvenItem[i] == NULL )
					{
						bFlag = true; 
						g_RockClient.Send_RpCsTakeoff( slot->PcItem->ItemUID, i );				
						return;
					}
				}
			
				if( !bFlag )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_GMMSG_INVENTORY_ENOUGH));	
					return;
				}
			}
			else if( InvenState == n_INVEN_ITEM )
			{	
				if( nRui->thePcInven.WearItem[n_WearSlot_Riding] == NULL )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_RITEM_NORIDESLOT ) );	
					return;
				}
				
				DWORD nRidingCode = nRui->thePcInven.WearItem[n_WearSlot_Riding]->Code;
				SItemBaseInfo* item_Info = NULL;
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRidingCode );

				if( item_Info )
				{
					DWORD dNPCCode = item_Info->theReHP;
					SNPCBaseInfo* npc_Info = NULL;
					npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

					if( npc_Info )
					{
						///-- �ش� ���̵� �������� �����Ҽ� ���� ����̶��...
						if( slot->PcItem->ItemTable->theUpgradeForm != npc_Info->theTargetRange )
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYS_NOWRIDINGCANNOTEQUIPITEM ) );  
							return;
						}
					}				
				}

				///-- ���õ� ���̵��������� ��Ʈ �������ϰ��
				int nType = slot->PcItem->ItemTable->theType;
				if( n_Type_Riding_Full == nType )
				{
					//���̵� ����Կ� �������� ���� �ϸ� �������� ���ϰ� �Ѵ�.
					if( WearSlot[n_Slotidx_Horse_Top]->PcItem || WearSlot[n_Slotidx_Horse_Middle]->PcItem || 
						WearSlot[n_Slotidx_Horse_Bottom]->PcItem  )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_REMOVE_RIDEITEM ) );  
						return;
					}
				}
				else
				{
					//���õ� �������� ���� Ÿ���� üũ
					int nTdx = GetWearIndex( slot->PcItem->Class );
					//���õ� �������� ����,����,�ǵ鷿,���� �ϰ��
					if( nTdx == n_Slotidx_Horse_Top || nTdx == n_Slotidx_Horse_Middle 
						|| nTdx == n_Slotidx_Horse_Bottom )
					{	//�߰� ���Կ� �ִ� �������� ��Ʈ �������� ��� �������� ���ϰ��Ѵ�.
						if(  WearSlot[n_Slotidx_Horse_Middle]->PcItem && 
							( n_Type_Riding_Full == WearSlot[n_Slotidx_Horse_Middle]->PcItem->ItemTable->theType ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_REMOVE_RIDESETITEM ) );	  		
							return;
						}
					}
				}

				int idx = GetWearIndex( slot->PcItem->Class, slot->PcItem->ItemTable->theType );			
				g_RockClient.Send_RpCsEquip( slot->PcItem->ItemUID, WearSlot[idx]->SlotNum );
			}
		}
	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::InvenSlotDrag( SEventMessage* EMsg )
{
	int idx = 0;
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );
	
	if( ( slot->Group == n_SGROUP_INVENTORY ) && ( InvenState == n_INVEN_QUEST ) )
	{
		return;
	}

	if( ( slot->PcItem != NULL ) && ( EMsg->FocusWnd != (WNDID)EMsg->fParam ) )
	{
		nRui->theCursorState = n_CT_ITEMDROP;

		//Ŀ���� ������ ���̱�

		if( nRui->GetCursorItem() == NULL )
		{
			nRui->SetCursorItem( slot->PcItem );
			nRui->SetItemSelectedSlot( slot );

			//��� �������̸�... �ش� ��� ������ ���̶���Ʈ ���ش�.
			if(  slot->PcItem != NULL && 
				( slot->PcItem->IType == n_ITYPE_WEAR  || slot->PcItem->IType == n_ITYPE_RIDE  ) )
			{
				idx = GetWearIndex( slot->PcItem->Class );
				if( idx == n_SlotIdx_Accessory1 )
				{
					//�Ǽ��縮 2��...GetWearIndex�� ���� ù��° �ε����� �����Ѵ�.
					WearSlot[idx]->SetHighlight( true );
					WearSlot[idx+1]->SetHighlight( true );
				}
				else
				{
					WearSlot[idx]->SetHighlight( true );
				}
			}
			else if( slot->PcItem != NULL && slot->PcItem->IType == n_ITYPE_RIDE_WEAR )
			{
				int a = 0;
			}
			else if( slot->PcItem != NULL && slot->PcItem->IType ==  n_ITYPE_SKIN_WEAR )
			{
				if(slot->PcItem->ItemTable)
				{
					idx = GetWearIndex(slot->PcItem->Class , slot->PcItem->ItemTable->theType );
					if( idx == n_ICLASS_NONE)
						return;
					WearSlot[idx]->SetHighlight( true );
				}
			}
		}
	}
}
void CInventoryWnd::SetSlotPosition(int SlotNum)
{
	m_SlotPosition  =   StartSlotNum + SlotNum;
}


//-----------------------------------------------------------------------------
void CInventoryWnd::InvenSlotDrop( SEventMessage* EMsg )
{
	CSlot* cslot = (CSlot*)Container->Get( EMsg->FocusWnd );
	CSlot* tslot = (CSlot*)Container->Get( (WNDID)EMsg->fParam );
	
	if( ((WNDID)EMsg->fParam != WID_None ) && ( tslot->GetWindowType() == n_wtSlot ) )
	{
		if( cslot == tslot )
		{
			nRui->ResetCursorItem();
		}
		else if( cslot->PcItem != NULL )
		{
			if(InvenState == n_INVEN_HORSE)			
			{
				switch( tslot->Group )
				{
					case n_SGROUP_WEAR:				DropWear( cslot, tslot );		break;//�κ��丮 - ����	
				}
			}
			else
			{		
				if( g_UserStoreSellWnd.ISUserSellMode() == FALSE )
				{
					switch( tslot->Group )
					{
					case n_SGROUP_QUICK:			DropQuickSlot( cslot, tslot );	break;//����â - ������
					case n_SGROUP_INVENTORY:		DropInventory( cslot, tslot );	break;//�κ��丮 - �κ��丮
					case n_SGROUP_WEAR:				DropWear( cslot, tslot );		break;//�κ��丮 - ����
						
					case n_SGROUP_EXCHANGE:			DropExchange( cslot, tslot );	break;//��ȯâ - �� ������
					case n_SGROUP_STORE:			DropStore( cslot );				break;//����â
					case n_SGROUP_WAREHOUSE:		DropBank( tslot );				break;//����â
#ifdef DECO_RENEWAL_GUILD_MJH
					case n_SGROUP_GUILDINVENTORY:	DropGuild( tslot );				break;//���â��
#endif // DECO_RENEWAL_GUILD_MJH
					case n_SGROUP_TUNING:			DropTuning();					break;//����â
					case n_SGROUP_SKILL:			DropSkill();					break;//��ųâ
					case n_SGROUP_PAYITEM_WND:		DropPayItemWnd( tslot );		break;//���� ������ ����Ʈ â 
					case n_SGROUP_PET_INVENTORY:	DropPetInventory(cslot, tslot); break;

					}
				}
				else
				{
					if( n_SGROUP_USER_STORE_SELL == tslot->Group )
					{
						g_UserStoreSellWnd.InsertItem( tslot , cslot );
					}
					else
					{
						nRui->ResetCursorItem();
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_USE )  );
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::ResetWearSlotHighlight()
{
	for( int i = 0; i < RP_MAX_EQUIP ; ++i )
	{
		WearSlot[i]->SetHighlight( false );
	}
}

//-----------------------------------------------------------------------------
int	CInventoryWnd::GetWearIndex( BYTE wearclass , BYTE wearType )
{
	switch( wearclass )
	{
		case n_Class_Helmet:		return( n_SlotIdx_Hair );//�Ӹ� WearSlot[4]
		case n_Class_Weapon:		return( n_SlotIdx_Weapon );//���� WearSlot[2]//������
		case n_Class_CoatArmor:		return( n_SlotIdx_Coat );//���� WearSlot[0]
		case n_Class_PantsArmor:		return( n_SlotIdx_Pants );//���� WearSlot[1]
		case n_Class_Gloves:		return( n_SlotIdx_Glove );//�尩 WearSlot[6]
		case n_Class_Shoes:		return( n_SlotIdx_Boots );//���� WearSlot[7]
		case n_Class_Serve:			return( n_SlotIdx_Sub );//���� WearSlot[5]
		case n_Class_Shield:		return( n_SlotIdx_Shield );//���� WearSlot[3]//�޼�
		case n_Class_Accessory:	return( n_SlotIdx_Accessory1 );//��ű� WearSlot[8], WearSlot[9]
																		//�Ǽ��縮 2���߿��� ù��° �ε����� �����Ѵ�.
		case n_Class_Riding:		return( n_Slotidx_Horse);
		case n_Class_RidingItem:
			{
				switch( wearType )
				{
					case	n_Type_Riding_Head:	return( n_Slotidx_Horse_Top );
					case	n_Type_Riding_Saddle: 
					case	n_Type_Riding_Full:	return( n_Slotidx_Horse_Middle );
					case	n_Type_Riding_Leg:  return( n_Slotidx_Horse_Bottom );					
					default	:	return( n_Slotidx_Horse_Top );
				}			
			}
			break;	

		case n_Class_Skin:
			{
				switch(wearType)
				{
					case n_Type_Skin_Suit: return(n_Slotidx_Skin_Suit);
					case n_Type_Skin_ChangeHair: return(n_Slotidx_Skin_Hair);
					case n_Type_Skin_KeepypHair: return(n_Slotidx_Skin_Hair);
					case n_Type_Skin_Wing: return(n_Slotidx_Skin_Wing);
				}
			}

	default:
		return( n_ICLASS_NONE );						
	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::ClickQuickSlot()
{
	//�����Կ��� �κ������� �̵��� ����.... �ܼ��� ��ũ�� ��������....
}

//���κ����� -> PC �κ����� �Ѿ� �°� .. 
//-------------------------------------------------------------------------------
void CInventoryWnd::ClickPetInven(CSlot* pslot,CSlot * tslot)
{


	//������� �� �꿡�� �κ� , �κ����� �� .. 
	//�ܼ��� �Ѿ�� �۾� .. 

//void Send_CsMoveItemPettoPC ( DWORD aItemId, BYTE aPetSlot, BYTE aItemPos, BYTE aItemNum )
//void Send_RpCsChangeSlot ( DWORD aItemUID1, DWORD aItemUID2, BYTE aSlot, BYTE aItemTab )
	
	if( pslot->PcItem )
	{
		if( pslot->PcItem->IType != n_ITYPE_NONE )
		{
			if( pslot->PcItem->ItemTable->theBundle != 1 && pslot->PcItem->Amount > 1 ) 
			{
				g_NumberInputWnd.Open( n_NUMBERINPUT_PETINVEN_INVENTORY, pslot->PcItem );			
				m_SrcSlot = pslot->SlotAddr;			
				m_TargetSlot = tslot;
					
			}
			else
			{
				g_RockClient.Send_CsMoveItemPettoPC(pslot->PcItem->ItemUID,g_PetInventoryWnd.m_CurSlotIndex , tslot->SlotNum, pslot->PcItem->Amount );					
						
			}	
		}	
		
	}

	
//	else
//	{
//		if( pslot->PcItem )
//		{
//			if( ( pslot->PcItem->ItemTable->theBundle > 1 ) &&
//				( pslot->PcItem->ItemTable->theCode == tslot->PcItem->ItemTable->theCode ) )
//			{
//				//	3. ��ġ��( �Ѵ� �����̰� ���� �������� ��� )
//				//   Combine��.... B -> A�� 
//				//���� �߰� 
//			}
//			else
//			{
//				//	2. ��ȯ( �̵� �ڸ��� �������� ������� )
//				g_RockClient.Send_CsMoveItemPettoPC(pslot->PcItem->ItemUID,g_PetInventoryWnd.m_CurSlotIndex , tslot->SlotNum, pslot->PcItem->Amount );
//			}
//		}
//	}
	
	nRui->ResetCursorItem();
	/*
	if( slot->PcItem->IType != n_ITYPE_NONE )
	{
		if( slot->PcItem->ItemTable->theBundle != 1 && slot->PcItem->Amount > 1 ) 
		{
			g_NumberInputWnd.Open( n_NUMBERINPUT_WAREHOUSE_INVENTORY, slot->PcItem );			
			
		}
		else
		{
			// ���κ����� �κ����� �̵� .. 
						
			//tslot->SetSlotPcItem( *(sslot->PcItem) );
			//�κ��� �ִ� Ÿ�� ���Ͽ� �ҽ� ���� �����۸� ���� 
			PcInven->InvenItem[tslot->SlotNum] = slot->PcItem;
			//��pcItem �� NULL���� �ش� .. ȭ�鿡 ���� �ֱ� ���ؼ� .. 
			
			PcInven->thePetData[PcInven->PetSlotNum]->InvenItem[slot->SlotNum] =  NULL ;
			
		

			nRui->ResetCursorItem();								
			g_PetInventoryWnd.UpdatePetPcItem();
			InventoryWnd.UpdateInvenSlotItem();
		}	
	}	
	*/



			
	//	nRui->ResetCursorItem();

}
//-----------------------------------------------------------------------------
void CInventoryWnd::ClickInventory( CSlot* cslot, CSlot* pslot ) //�κ��̵�
{
	int	position = 0;

	if( InvenState == n_INVEN_HORSE )
	{
		nRui->ResetCursorItem();
		return;
	}
	if(!nRui->m_KeyLock.Inventory.GetCheckTime(g_nowTime))
		return;

	//�κ��ȿ��� �̵���� 3����
	if( cslot->PcItem == NULL )
	{
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��	
		if(cslot->SlotNum < RP_MAX_INVENTORY)
		{
			position = StartSlotNum + cslot->SlotNum;
		}
		else
		{
			position = cslot->SlotNum +  PayStartSlotNum ;
		}
		//	1. �̵�( �󽽷��϶� )
#else		
		if( TabType == n_InvenTab )
		{
			position = StartSlotNum + cslot->SlotNum;
		}
		else
		{
			position = cslot->SlotNum;
		}
#endif //WIDE_PAY_INVENTORY		
		

		///////////////////////////////////////////////////////////////////////////////////////////////////
		if( pslot->PcItem )
		{
			g_RockClient.Send_RpCsChangeSlot( pslot->PcItem->ItemUID, 0, position, TabType );
		}
	}
	else
	{
		if( pslot->PcItem )
		{
			if( pslot->PcItem->ItemTable )
			{
				if( ( pslot->PcItem->ItemTable->theBundle > 1 ) &&
					( cslot->PcItem->ItemTable && 
					  pslot->PcItem->ItemTable->theCode == cslot->PcItem->ItemTable->theCode ) )
				{
					//	3. ��ġ��( �Ѵ� �����̰� ���� �������� ��� )
					//   Combine��.... B -> A�� 
					g_RockClient.Send_RpCsCombine( cslot->PcItem->ItemUID, pslot->PcItem->ItemUID, TabType );
				}
				else if( cslot->PcItem->ItemTable )
				{
					//	2. ��ȯ( �̵� �ڸ��� �������� ������� )
					g_RockClient.Send_RpCsChangeSlot( pslot->PcItem->ItemUID, cslot->PcItem->ItemUID, 255, TabType );
				}
			}
		}
	}
	
	nRui->ResetCursorItem();

	return;
}

//-----------------------------------------------------------------------------
void CInventoryWnd::ClickWear( CSlot* cslot, CSlot* pslot )
{
	SPcItem*	tmpItem = NULL;
	int			position = 0;

	if( cslot->PcItem == NULL )
	{
		tmpItem = pslot->GetSlotItem();

		//��� ���� ��û
		position = StartSlotNum + cslot->SlotNum;
		g_RockClient.Send_RpCsTakeoff( tmpItem->ItemUID, position );
	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::ClickExchange( CSlot* cslot, CSlot* pslot )
{	
//	if( pslot->PcItem->Class == n_Class_Use )
//	{
//		if( pslot->PcItem->Amount > 1 ) 
//		{
//			g_NumberInputWnd.Open( &g_ExchangeWnd, pslot->PcItem, n_InvenTab );
//		}
//	}

	return;
}

//-----------------------------------------------------------------------------
void CInventoryWnd::ClickStore( CSlot* slot )
{
	if( slot->PcItem == NULL )
		return;
	
	// �ż��� ��� ���� ó�� 
	if( slot->PcItem->Class == n_Class_FrontierPet )
	{
		if( nRui->GetPcInvenInfo()->FrontierInfo )
		{
			if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );									
			}
			else
			{
				if( nRui->GetPcInvenInfo()->FrontierInfo->theDues >= slot->PcItem->ItemTable->thePrice )
				{
					g_StrInputWnd.Open( n_STRINPUT_FRONTIERPET_NAME, slot->PcItem->Code );
				}
				else
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );									
				}	
			}
		}

		nRui->ResetCursorItem();

		return;
	}	

	//������ ���� â ����
	StoreBuyWnd.SetBuyStoreOwner( StoreWnd.OwnerUID );
	if( slot->PcItem->ItemTable->theBundle != 1 )
	{		
		g_NumberInputWnd.Open( n_NUMBERINPUT_BUYSTORE_INVENTORY, slot->PcItem );
	}
	else
	{
		if( nRui->thePcParam.Money >= slot->PcItem->ItemTable->thePrice )
		{
			StoreBuyWnd.SetBuyItemInfo( slot->PcItem );
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
//			nRui->TextOutChat( "���� �����ؼ� �������� ������ �� �����ϴ�.", 0xffff0000 );
		}
	}

	nRui->ResetCursorItem();
}

//-----------------------------------------------------------------------------
void CInventoryWnd::ClickBank( CSlot* slot )
{
	if( slot->PcItem == NULL )
	{
		return;
	}

	if( slot->PcItem->IType != n_ITYPE_NONE )
	{
		if( slot->PcItem->ItemTable->theBundle != 1 && slot->PcItem->Amount > 1 ) 
		{
			g_NumberInputWnd.Open( n_NUMBERINPUT_WAREHOUSE_INVENTORY, slot->PcItem );			
		}
		else
		{
			g_RockClient.Send_RpCsDraw( slot->PcItem->ItemUID, slot->PcItem->Amount );
		}

		nRui->ResetCursorItem();
	}

	return;
}
#ifdef DECO_RENEWAL_GUILD_MJH
//-----------------------------------------------------------------------------
void CInventoryWnd::ClickGuild( CSlot* slot )
{
	if( slot->PcItem == NULL )
	{
		return;
	}

	if( slot->PcItem->IType != n_ITYPE_NONE )
	{
		if( slot->PcItem->ItemTable->theBundle != 1 && slot->PcItem->Amount > 1 ) 
		{
			g_NumberInputWnd.Open( n_NUMBERINPUT_GUILDINVEN_INVENTORY, slot->PcItem );			
		}
		else
		{

			g_RockClient.Send_RpCsDrawFrontier( slot->PcItem->ItemUID, slot->PcItem->Amount );

		}

		nRui->ResetCursorItem();
	}

	return;
}
#endif // DECO_RENEWAL_GUILD_MJH




//-----------------------------------------------------------------------------
void CInventoryWnd::ClickTuning()
{
}

//-----------------------------------------------------------------------------
void CInventoryWnd::ClickSkill()
{
	//��ųâ���� �κ����� �̵��Ұ�
}


//-----------------------------------------------------------------------------
void CInventoryWnd::ClickPayItemWnd( CSlot* slot )
{
	if( slot->PcItem == NULL )
	{
		return;
	}
	g_RockClient.Send_CsGetPayItem( slot->PcItem->ItemUID );
	
	nRui->ResetCursorItem();

	return;
}

//-----------------------------------------------------------------------------
void CInventoryWnd::DropQuickSlot( CSlot* cslot, CSlot* tslot )
{
	if( cslot && cslot->PcItem->IType == n_ITYPE_USE )
	{
#ifdef PREMIUMZONE_KEY
		if( cslot->PcItem->Code == 21560 || cslot->PcItem->Code == 21561
			|| cslot->PcItem->Code == 21562 || cslot->PcItem->Code == 21563 )
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP_THERE ) );
			return;
		}
#endif // PREMIUMZONE_KEY
#ifdef ITEMLIMITFUNC_NOTQUICKSLOT			//������ �������
		WORD aItemLimitFunc = cslot->PcItem->ItemTable->theItemLimitFunc;
		if(CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotQuickSlot ))
		{
			nRui->ResetCursorItem();
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP_THERE ) );
			return;
		}
#endif

		//���� üũ ��ƾ
		WORD Citizen = (WORD)(cslot->PcItem->ItemTable->theCitizen);
		if( RAIN == Citizen|| MILLENA == Citizen )	//�������� ���� ���� �������̶��
		{
			if( Citizen != nRui->thePcParam.Citizen )		//�÷��̾� ���� �������� �ƴ϶��
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OTHER_NATION_ITEM )  );	
				return;
			}
		}		
		
		//�����Կ� �������� �ִٸ�...����...
		if( tslot->PcItem != NULL )
		{
			BYTE position = StartSlotNum + tslot->SlotNum;
//			PcInven->QuickSlot[position] = NULL;
			tslot->PcItem = NULL;
			
			//��ũ ����

			g_RockClient.Send_RpCsUnLinkQuickSlot( position );
		}
		
		//�������� �����������....��ũ....
		if( tslot->PcItem == NULL )
		{
			for( int i = 0; i < RP_MAX_QUICK_SLOT; ++i )
			{
				if( PcInven->QuickSlot[i] == cslot->PcItem )
				{
					g_RockClient.Send_RpCsUnLinkQuickSlot( i );
//					PcInven->QuickSlot[i] = NULL;
					MainMenuWnd.UpdateQuickSlotItem();
					break;
				}
			}
			
			int StartSlotNum = MainMenuWnd.GetStartSlotNum();
			
//			tslot->SetSlotItem( cslot->GetSlotItem() );
//			PcInven->QuickSlot[StartSlotNum + tslot->SlotNum] = cslot->GetSlotItem();
						
			//�κ����� ���������� ��ũ
			SRpdQuickSlot qslot;
			qslot.theID.theItemID	= cslot->GetSlotItem()->ItemUID;
			qslot.theType			= n_ITEM;
			qslot.thePosition = StartSlotNum + tslot->SlotNum;
			//qslot.thePosition		= StartSlotNum + tslot->SlotNum;
			g_RockClient.Send_RpCsLinkQuickSlot( qslot ); 

			nRui->ResetCursorItem();
		}		
	}
	else
	{
		nRui->ResetCursorItem();
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP_THERE ) );
		return;
	}

	return;
}

//=-------------------------------------------------------------------------------

void CInventoryWnd::DropPetInventory(CSlot* pslot , CSlot* tslot)
{
	if( pslot->PcItem == NULL  ||  InvenState == n_INVEN_QUEST)
	{
		return;
	}

	if( pslot->PcItem )
	{
		
		if( pslot->PcItem->IType != n_ITYPE_NONE )
		{
			if( pslot->PcItem->ItemTable->theBundle != 1 && pslot->PcItem->Amount > 1 ) 
			{
				g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_PETINVEN, pslot->PcItem );			
				g_PetInventoryWnd.SetSrcSlot(pslot);			
				g_PetInventoryWnd.SetTargetSlot(tslot);
				
			}
			else
			{
				g_RockClient.Send_CsMoveItemPCtoPet(pslot->PcItem->ItemUID,g_PetInventoryWnd.m_CurSlotIndex , tslot->SlotNum, pslot->PcItem->Amount );					
							
			}	
		}	
		
	}
	nRui->ResetCursorItem();
}

//-----------------------------------------------------------------------------
void CInventoryWnd::DropInventory( CSlot* cslot, CSlot* tslot )
{
	int	position = 0;
	
	if( InvenState == n_INVEN_QUEST )
	{
		return;
	}
	//�κ��ȿ��� �̵���� 3����
	if(!nRui->m_KeyLock.Inventory.GetCheckTime(g_nowTime))
		return;

	if( tslot->PcItem == NULL )
	{
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��
		//	1. �̵�( �󽽷��϶� )
		if(tslot->SlotNum < RP_MAX_INVENTORY)
		{
			position = StartSlotNum + tslot->SlotNum;
		}
		else
		{
			position = tslot->SlotNum +  PayStartSlotNum ;
		}
#else
		position = StartSlotNum + tslot->SlotNum;
#endif //WIDE_PAY_INVENTORY

		if( cslot->PcItem )
		{
			g_RockClient.Send_RpCsChangeSlot( cslot->PcItem->ItemUID, 0, position, TabType );
		}
	}
	else
	{
		if( cslot->PcItem )
		{
			if( ( cslot->PcItem->ItemTable->theBundle > 1 ) &&
				( cslot->PcItem->ItemTable->theCode == tslot->PcItem->ItemTable->theCode ) )
			{
				//	3. ��ġ��( �Ѵ� �����̰� ���� �������� ��� )
				//   Combine��.... B -> A�� 
				g_RockClient.Send_RpCsCombine( tslot->PcItem->ItemUID, cslot->PcItem->ItemUID, TabType );
			}
			else
			{
				//	2. ��ȯ( �̵� �ڸ��� �������� ������� )
				g_RockClient.Send_RpCsChangeSlot( cslot->PcItem->ItemUID, tslot->PcItem->ItemUID, 255, TabType );
			}
		}
	}

	nRui->ResetCursorItem();

	return;
}

//-----------------------------------------------------------------------------
void CInventoryWnd::DropWear( CSlot* cslot, CSlot* tslot )
{
	SPcItem*	tmpItem = NULL;
	int			position = 0;


	if( cslot->PcItem && cslot->PcItem->IType == n_ITYPE_WEAR )
	{
		//���� üũ ��ƾ
		WORD Citizen = (WORD)(cslot->PcItem->ItemTable->theCitizen);
		if( RAIN == Citizen|| MILLENA == Citizen )	//�������� ���� ���� �������̶��
		{
			if( Citizen != nRui->thePcParam.Citizen )		//�÷��̾� ���� �������� �ƴ϶��
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OTHER_NATION_ITEM )  );	
				return;
			}
		}		
		
		if( ( cslot->PcItem->Class == tslot->Class ) && 
			( ( cslot->PcItem->ItemTable->theCitizen == PcParam->Citizen ) || ( cslot->PcItem->ItemTable->theCitizen == 2 ) ) )
		{
			// ����̳� Ȱ �� �μտ� ��� ��� �ִ� �������� üũ�ؼ� ��� ���� ���ѵξ�� �Ѵ�.
			if( ( tslot->SlotNum == 2 ) || ( tslot->SlotNum == 3 ) )
			{
				if( WearSlot[n_SlotIdx_Weapon]->PcItem != NULL )//������ ���� �˻�
				{
					if( ( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_TwohandSword ) ||
						( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_CrystalBall ) ||
						( WearSlot[n_SlotIdx_Weapon]->PcItem->ItemTable->theType == n_Type_Staff ) )
					{
						if( cslot->PcItem->ItemTable->theType == n_Type_TwohandSword ||
							cslot->PcItem->ItemTable->theType == n_Type_CrystalBall ||
							cslot->PcItem->ItemTable->theType == n_Type_Staff )
						{
						}
						else
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP ) );
//							nRui->TextOutChat( _RT("SYSTEM_MSG : ���� ��� ������ �������� �������� ������ �� �����ϴ�."), D3DCOLOR_XRGB( 255, 255, 0 ) );
						}
						return;
					}
				}
				if( WearSlot[n_SlotIdx_Shield]->PcItem != NULL )//�޼� ���� �˻�
				{
					if( ( WearSlot[n_SlotIdx_Shield]->PcItem->ItemTable->theType == n_Type_MagicBook ) ||
						( WearSlot[n_SlotIdx_Shield]->PcItem->ItemTable->theType == n_Type_Bow ) )
					{
						if( cslot->PcItem->ItemTable->theType == n_Type_MagicBook ||
							cslot->PcItem->ItemTable->theType == n_Type_Bow )
						{
						}
						else
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EQUIP ) );
//							nRui->TextOutChat( _RT("SYSTEM_MSG : ���� ��� ������ �������� �������� ������ �� �����ϴ�."), D3DCOLOR_XRGB( 255, 255, 0 ) );
						}
						return;
					}
				}				
			}

			bool result = CheckWearPassiveSkill( cslot->PcItem->ItemTable );
			if( result == false )
			{				
				if( nRui->GetPcParamInfo()->Level < cslot->PcItem->ItemTable->theLimitLevel )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CONFIRM_ITEMLEV_CHARACTERLEV )) ;				
				}
				else
				{
					SSkillBaseInfo* skill_info = NULL;
					skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( cslot->PcItem->ItemTable->theNeedSkill );

					if( skill_info )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_EQUIP ), skill_info->theSkillName );				
					}
				}
				
				return;
			}

			SItemBaseInfo * pItemBaseInfo = cslot->PcItem->ItemTable;
			if(pItemBaseInfo)
			{
				if( !(!pItemBaseInfo->theFileCode1 && !pItemBaseInfo->theFileCode2) )
				{
					int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();			
					DWORD dFileCode =  g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, 
													cslot->PcItem->ItemTable->theCode );
					if(0 == dFileCode)
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTSEX_WEAR ) ); 
						return;
					}	
				}
			}

			///-- ���õ� �������� ��Ʈ �������� ���
			int nType = cslot->PcItem->ItemTable->theType;
			if( n_Type_SetCloth == nType )
			{
				//���� ����(����,����,�ǵ鷿,����) �� �������� ���� �ϸ� �������� ���ϰ� �Ѵ�.
				if( WearSlot[n_SlotIdx_Coat]->PcItem || WearSlot[n_SlotIdx_Pants]->PcItem || 
					WearSlot[n_SlotIdx_Glove]->PcItem || WearSlot[n_SlotIdx_Boots]->PcItem )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, 
					G_STRING_CHAR( ISD_SYSPR_WEAR_WITHOUT_OTHERWEAR ) );	
					return;
				}
			}
			else	//���õ� �������� ��Ʈ �������� �ƴҰ��
			{
				//���õ� �������� ���� Ÿ���� üũ 
				int nTdx = GetWearIndex( cslot->PcItem->Class );
				//���õ� �������� ����,����,�ǵ鷿,���� �ϰ��
				if( nTdx == n_SlotIdx_Coat || nTdx == n_SlotIdx_Pants || nTdx == n_SlotIdx_Glove ||
					nTdx == n_SlotIdx_Boots )
				{
					//���� ���Կ� �ִ� �������� ��Ʈ �������� ��� �������� ���ϰ��Ѵ�.
					if(  WearSlot[n_SlotIdx_Coat]->PcItem && 
						( n_Type_SetCloth == WearSlot[n_SlotIdx_Coat]->PcItem->ItemTable->theType ) )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, 
						G_STRING_CHAR( IDS_SYSPR_WEAR_WITHOUT_SETITEM ) );					
						return;
					}
				}
			}
			
			tmpItem = cslot->GetSlotItem();

			//��� ���� ��û
			position = tslot->SlotNum;
			g_RockClient.Send_RpCsEquip( tmpItem->ItemUID, position );
		}
	}
	else if( cslot->PcItem && cslot->PcItem->IType == n_ITYPE_RIDE )
	{
		if( cslot->PcItem->Class == tslot->Class )
		{		
			tmpItem = cslot->GetSlotItem();
			//��� ���� ��û
			position = tslot->SlotNum;
			g_RockClient.Send_RpCsEquip( tmpItem->ItemUID, position );
		}
	}
	else if( cslot->PcItem && cslot->PcItem->IType == n_ITYPE_RIDE_WEAR )
	{
		int a = 0;
	}
}

//-----------------------------------------------------------------------------
void CInventoryWnd::DropExchange( CSlot* cslot, CSlot* tslot )
{
//	if( cslot->PcItem->Class == n_Class_Use )
//	{
//		if( cslot->PcItem->Amount > 1 ) 
//		{
//			g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_EXCHANGE, cslot->PcItem );
//		}
//	}

	return;
}

//-----------------------------------------------------------------------------
void CInventoryWnd::DropStore( CSlot* slot )
{
	if( slot->PcItem == NULL )
	{
		return;
	}

	//�ٷ� �������� ��ȯ
	//�̵� ��ġ�� ���� ������ ��ü....

	//������ �Ǹ� â ����

	if( slot->PcItem->Amount > 1 )
	{
		g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_SELLSTORE, slot->PcItem );
	}
	else
	{
		if( slot->Group == n_SGROUP_INVENTORY )
		{
			StoreSellWnd.SetSellItemInfo( slot->PcItem, n_InvenTab );
		}
		else if( slot->Group == n_SGROUP_WEAR )
		{
			StoreSellWnd.SetSellItemInfo( slot->PcItem, n_WearTab );
		}
	}

	nRui->ResetCursorItem();
}

//-----------------------------------------------------------------------------
void CInventoryWnd::DropBank( CSlot* slot )
{
	if( slot->PcItem == NULL )
	{
		return;
	}

	if( slot->PcItem->IType != n_ITYPE_NONE )
	{
		if( slot->PcItem->ItemTable->theBundle != 1 && slot->PcItem->Amount > 1 ) 
		{
			g_NumberInputWnd.Open( n_NUMBERINPUT_WAREHOUSE_INVENTORY, slot->PcItem );			
		}
		else
		{
			g_RockClient.Send_RpCsDraw( slot->PcItem->ItemUID, slot->PcItem->Amount );
		}

		nRui->ResetCursorItem();
	}

	return;
}
#ifdef DECO_RENEWAL_GUILD_MJH
//-----------------------------------------------------------------------------
void CInventoryWnd::DropGuild( CSlot* slot )
{
	if( slot->PcItem == NULL )
	{
		return;
	}

	if( slot->PcItem->IType != n_ITYPE_NONE )
	{
		if( slot->PcItem->ItemTable->theBundle != 1 && slot->PcItem->Amount > 1 ) 
		{
			g_NumberInputWnd.Open( n_NUMBERINPUT_GUILDINVEN_INVENTORY, slot->PcItem );			
		}
		else
		{

			g_RockClient.Send_RpCsDrawFrontier( slot->PcItem->ItemUID, slot->PcItem->Amount );

			
		}

		nRui->ResetCursorItem();
	}

	return;
}
#endif // DECO_RENEWAL_GUILD_MJH
//-----------------------------------------------------------------------------
void CInventoryWnd::DropTuning()
{
}

//-----------------------------------------------------------------------------
void CInventoryWnd::DropSkill()
{
	//�κ����� ��ųâ���� �̵��Ұ�....
}

void CInventoryWnd::DropPayItemWnd( CSlot* slot )
{
	nRui->ResetCursorItem();
	
	return;
}
//
void CInventoryWnd::CloseNumberInputWnd( int aNumItem )
{
	if( !m_SrcSlot )
	{
		return;
	}

	if( m_SrcSlot->PcItem )
	{
		{
			if( m_SrcSlot->PcItem->Amount >= aNumItem )
			{
								
				g_RockClient.Send_CsMoveItemPettoPC(m_SrcSlot->PcItem->ItemUID,g_PetInventoryWnd.m_CurSlotIndex , m_TargetSlot->SlotNum, aNumItem );
				//m_TargetSlot->SetSlotPcItem( *(m_SrcSlot->PcItem) );				
				//m_TargetSlot->PcItem->Amount = aNumItem;
				//	if(PcInven->thePetData[m_CurSlotIndex] !=NULL )
				//	{
				//		PcInven->thePetData[m_CurSlotIndex]->InvenItem[m_TargetSlot->SlotNum] = m_SrcSlot->PcItem;
				//		PcInven->thePetData[m_CurSlotIndex]->InvenItem[m_TargetSlot->SlotNum]->Amount = aNumItem;
				//		PcInven->InvenItem[m_SrcSlot->SlotNum] = NULL;
				//		UpdatePetPcItem();
				//		InventoryWnd.UpdateInvenSlotItem();
				//	}
				
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_NUM ) );
			}
		}

	}




}


//-----------------------------------------------------------------------------
int CInventoryWnd::GetStartSlotNum()
{
	return( StartSlotNum );
}

//-----------------------------------------------------------------------------
int CInventoryWnd::GetBlankSlotNum()
{
	int i( 0 );
	for( i = 0; i < c_MaxInventoryItemDrawNum; ++i )
	{
		if( PcInven->InvenItem[StartSlotNum + i] == NULL )
		{
			//���� ȭ�鿡 ���̴� �󽽷��� ã�´�.
			return( StartSlotNum + i );
		}
	}

	//��ü ������ ������ �� ������ ã�´�.
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{
		if( PcInven->InvenItem[i] == NULL )
		{
			return( i );
		}
	}
	
	//�� ������ ������ ����.... -1
	return( -1 );
}

int CInventoryWnd::GetFirstSlotNum()
{
	for(int i =0 ; i < c_MaxInvenItemNum; ++i)
	{
		if(PcInven->InvenItem[i] == NULL)
		{
			return i;
		}
	}

	return -1;
}

///---------------------------------------------------------------------------
///-- CheckWearPassiveSkill
///---------------------------------------------------------------------------
///-- ��� ���� ������ Skill �� �������� �Ǵ��Ѵ�.
///-- ��� ������ �� �ִ� Level �ΰ� �Ǵ��Ѵ�.
///-- true  : ���� ���� 
///-- flase : ���� �Ұ�
bool CInventoryWnd::CheckWearPassiveSkill( SItemBaseInfo* aItemInfo )
{
	bool result = true;

	if( aItemInfo == NULL )
	{
		return false; 
	}

    //by simwoosung
	//�������� �з����� ��ȣ�� ������ ��ȣ�� �ʿ�� �Ѵٸ�

	if( theSkillCode_CitizenBless == aItemInfo->theNeedSkill )
		return true;	
	
	// ���� �����ۿ� ���� ������ �� �ִ� ��ų�� �������� �˻��Ѵ�. 
	// ����� 
	if( aItemInfo->theType == n_Type_Sword )		
	{
		if( theSkillCode_Sword < aItemInfo->theNeedSkill )
		{
			result = false;	
		}			
	}
	else if( aItemInfo->theType == n_Type_TwohandSword ) 
	{
		if( theSkillCode_TwohandSword < aItemInfo->theNeedSkill )
		{
			result = false;	
		}			
	}
	else if( aItemInfo->theType == n_Type_Bow ) 
	{
		if( theSkillCode_Bow < aItemInfo->theNeedSkill )
		{
			result = false;	
		}			
	}
	else if( aItemInfo->theType == n_Type_MagicBook ) 
	{
		if( theSkillCode_MagicBook < aItemInfo->theNeedSkill )
		{
			result = false;			
		}			
	}
	else if( aItemInfo->theType == n_Type_Staff ) 
	{
		if( theSkillCode_Staff < aItemInfo->theNeedSkill )
		{
			result = false;			
		}			
	}
	else if( aItemInfo->theType == n_Type_CrystalBall ) 
	{
		if( theSkillCode_CrystalBall < aItemInfo->theNeedSkill )
		{
			result = false;				
		}			
	}
	else if( aItemInfo->theType == n_Type_Shield ) 
	{
		if( theSkillCode_Shield < aItemInfo->theNeedSkill )
		{
			result = false;				
		}			
	}	
	// ���� ���� 
	else if( aItemInfo->theType == n_Type_ChangeHair || aItemInfo->theType == n_Type_KeepupHair ) 
	{
		if( aItemInfo->theNeedSkill )
		{
			int skill_max = ( ( aItemInfo->theNeedSkill / 10 ) * 10 ) + 10;			

			for( int i = 0; i < RP_MAX_USER_SKILL; ++i )
			{
				if( nRui->GetPcInvenInfo()->SkillItem[i] != NULL )
				{
					if( nRui->GetPcInvenInfo()->SkillItem[i]->Code >= aItemInfo->theNeedSkill && 
						nRui->GetPcInvenInfo()->SkillItem[i]->Code <= skill_max )									
					{
						return true;
					}
				}
			}
			
			result = false;				
		}			
	}
	else if( aItemInfo->theType >= n_Type_LightCoat && aItemInfo->theType <= n_Type_LightShoes ) 
	{
		if( theSkillCode_LightArmor < aItemInfo->theNeedSkill )
		{
			result = false;			
		}			
	}
	else if( aItemInfo->theType >= n_Type_MiddleCoat && aItemInfo->theType <= n_Type_MiddleShoes ) 
	{
		if( theSkillCode_MiddleArmor < aItemInfo->theNeedSkill )
		{
			result = false;			
		}			
	}
	else if( aItemInfo->theType >= n_Type_HeavyCoat && aItemInfo->theType <= n_Type_HeavyShoes ) 
	{
		if( theSkillCode_HeavyArmor < aItemInfo->theNeedSkill )
		{
			result = false;			
		}			
	}
	else if( aItemInfo->theType >= n_Type_MagicCoat && aItemInfo->theType <= n_Type_MagicShoes ) 
	{
		if( theSkillCode_MasicArmor < aItemInfo->theNeedSkill )
		{
			result = false;				
		}			
	}
	else if( aItemInfo->theType >= n_Type_PriestCoat && aItemInfo->theType <= n_Type_PriestShoes ) 
	{
		if( theSkillCode_PriestArmor < aItemInfo->theNeedSkill )
		{
			result = false;				
		}			
	}
	else if( aItemInfo->theType >= n_Type_SpiritCoat && aItemInfo->theType <= n_Type_SpiritShoes ) 
	{
		if( theSkillCode_SpiritArmor < aItemInfo->theNeedSkill )
		{
			result = false;	
		}			
	}

	if( nRui->GetPcParamInfo()->Level < aItemInfo->theLimitLevel )
	{
		result = false;			
	}

//	if( result == false )
//	{
//		SSkillBaseInfo* skill_info = NULL;
//		skill_info = g_Pc_Manager.GetSkillBaseInfoPtr( aItemInfo->theNeedSkill );

//		SystemPrint( n_DarkRed, n_SystemMsg, "%s�� �����ž� ������ �� �ֽ��ϴ�", skill_info->theSkillName );				
//	}

	return result;
}

bool CInventoryWnd::CheckUseItemPassiveSkill( SItemBaseInfo* aItemInfo )
{
	bool result = true;

	//by simwoosung
	//�������� �з����� ��ȣ�� ������ ��ȣ�� �ʿ�� �Ѵٸ�

	if( theSkillCode_CitizenBless == aItemInfo->theNeedSkill )
		return true;	

	// ���� �����ۿ� ���� ������ �� �ִ� ��ų�� �������� �˻��Ѵ�. 
	// �๰�� 
	// ���ٸ� ���̺��� ��� �ϴ� �ϵ��ڵ����� �����Ѵ�. �Ѥ�; �ϵ��ڵ��� ���� ���� �÷�...�� ��...�� 
	if( aItemInfo->theNeedSkill >= c_SkillCode_BasicCure_Min && aItemInfo->theNeedSkill <= c_SkillCode_BasicCure_Max )			// ���� ġ�� ��ų ���� 
	{
		if( theSkillCode_BasicCure < aItemInfo->theNeedSkill )
		{
			result = false;	
		}			
	}
	else if( aItemInfo->theNeedSkill >= c_SkillCode_MasicPharmacy_Min && aItemInfo->theNeedSkill <= c_SkillCode_MasicPharmacy_Max )		// ������ ���� ��ų ���� 
	{
		if( theSkillCode_MasicPharmacy < aItemInfo->theNeedSkill )
		{
			result = false;	
		}			
	}

	return result;
}

//----------------------------------------------------------------------------
bool CInventoryWnd::CompareSlot( CSlot* cslot, CSlot* tslot )
{
	if( ( cslot != NULL ) && ( tslot != NULL ) )
	{
		if( ( cslot->Group == tslot->Group ) &&
			( cslot->SlotNum == tslot->SlotNum ) &&
			( cslot->PcItem->ItemUID == tslot->PcItem->ItemUID ) &&
			( cslot->PcItem->Code == tslot->PcItem->Code ) )
		{
			return( true );
		}
	}

	return( false );
}

BOOL CInventoryWnd::IsEmptyWearSlot()
{
	//���� ����(���, ����,����,�ǵ鷿,����) �� �������� ���� �ϸ� �������� ���ϰ� �Ѵ�.
	if( WearSlot[n_SlotIdx_Hair]->PcItem || WearSlot[n_SlotIdx_Coat]->PcItem || 
		WearSlot[n_SlotIdx_Pants]->PcItem || WearSlot[n_SlotIdx_Glove]->PcItem || 
		WearSlot[n_SlotIdx_Boots]->PcItem 
		|| WearSlot[n_Slotidx_Skin_Suit]->PcItem  || WearSlot[n_Slotidx_Skin_Hair]->PcItem 
		|| WearSlot[n_Slotidx_Skin_Wing]->PcItem 

		)
	{
		return FALSE;
	}

	return TRUE;
}
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��

void CInventoryWnd::Open()
{
	
	bool IsWnd = !InventoryWnd.IsVisible;
	PayFrameWnd->Reposition( 750 , 400 );
	Statebtn->SetVisible(false);
	Statebtn1->SetVisible(false);
	Statebtn2->SetVisible(false);
	Statebtn3->SetVisible(false);
	Statebtn4->SetVisible(false);

	nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
	 
	switch( g_slotNum )
	{	
		case 0:
		break;
		case 1:
			Statebtn->SetVisible(true);
			nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
		break;
		case 2:
			Statebtn->SetVisible(true);
			Statebtn1->SetVisible(true);
			nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
		break;
		case 3:
			Statebtn->SetVisible(true);
			Statebtn1->SetVisible(true);
			Statebtn2->SetVisible(true);
			nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
		break;
		case 4:
			Statebtn->SetVisible(true);
			Statebtn1->SetVisible(true);
			Statebtn2->SetVisible(true);
			Statebtn3->SetVisible(true);
			nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
		break;
		case 5:
			Statebtn->SetVisible(true);
			Statebtn1->SetVisible(true);
			Statebtn2->SetVisible(true);
			Statebtn3->SetVisible(true);
			Statebtn4->SetVisible(true);
			nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)IsWnd, 0, 0, 0 );
		break;
	}
	
}
#endif //WIDE_PAY_INVENTORY

void CInventoryWnd::SetEldText(void* aEldText )
{
	RTCHAR *Str = (RTCHAR*)aEldText;
	
	RTCHAR  CurNumber[256]={0,};
	RTCHAR  NumberComma[256]={0,};
	
	int Count = 0 ; 
	
	int Len = Rstrlen(Str);
	
	int FirstPoint =Len%3  ;
	if( FirstPoint == 0 )
	{
		FirstPoint = 3;
	}
	int i = 0 , j = 0;
	
	ZeroMemory(&CurNumber , sizeof(CurNumber));
	
	for(i =0 , j=0 ; i < 256 ; i++ , j++)
	{
		//3�ڸ����̻� ������ . 
		if( Str[i] != NULL && Count == FirstPoint )
		{
			FirstPoint = 3; 
			Count = 0; 
			CurNumber[j++] = ',';
		}
		
		CurNumber[j] = Str[i];
		
		if( Count > 3 )	Count = 0 ; 
		
		Count++;
		
		if(Str[i] == NULL)
		{
			ZeroMemory(Str , i + 2);
			Rstrcpy(Str , CurNumber);
			break; 
		}
		
	}
}

void CInventoryWnd::SetEldText( char* Str, DWORD aEld )
{

	char  CurNumber[256]={0,};
	char  NumberComma[256]={0,};
	
	int Count = 0 ; 
	
	_ultoa(aEld , Str , 10);
	
	int Len = strlen(Str);
	
	int FirstPoint =Len%3  ;
	if( FirstPoint == 0 )
	{
		FirstPoint = 3;
	}
	int i = 0 , j = 0;
	
	ZeroMemory(&CurNumber , sizeof(CurNumber));
	
	for(i =0 , j=0 ; i < 256 ; i++ , j++)
	{
		//3�ڸ����̻� ������ . 
		if( Str[i] != NULL && Count == FirstPoint )
		{
			FirstPoint = 3; 
			Count = 0; 
			CurNumber[j++] = ',';
		}
		
		CurNumber[j] = Str[i];
		
		if( Count > 3 )	Count = 0 ; 
		
		Count++;
		
		if(Str[i] == NULL)
		{
			
			ZeroMemory(Str , i + 2);
#ifdef UNICODE_ROCK
			DXUtil_ConvertAnsiStringToWide(RWCHAR(Str) , CurNumber , -1) ;
#else  
			SAFE_STR_CPY(Str , CurNumber , Rstrlen(CurNumber) + 1  );
#endif
			
			break; 
		}
		
	}

	/*
	DWORD eld_1 = aEld % 1000;
	DWORD eld_1_000 = 0;
	DWORD eld_1_000_000 = 0;
	DWORD eld_1_000_000_000 = 0;	

	char eldstr_1[255] = {0,};
	char eldstr_1_000[255] = {0,};
	char eldstr_1_000_000[255] = {0,};
	char eldstr_1_000_000_000[255] = {0,};

 
	eld_1_000_000_000 = aEld / 1000000000; 
	eld_1_000_000 = ( aEld - ( eld_1_000_000_000 * 1000000000 ) ) / 1000000; 
	eld_1_000 = ( aEld - ( eld_1_000_000_000 * 1000000000 ) - ( eld_1_000_000 * 1000000 ) ) / 1000;	
	eld_1 = aEld - ( eld_1_000_000_000 * 1000000000 ) - ( eld_1_000_000 * 1000000 ) - ( eld_1_000 * 1000 );	


	if( eld_1 )
	{
		Rsprintf( RWCHAR(eldstr_1), _RT("%d"), eld_1 );										// 1,000,000,000,[000] 

		if( eld_1_000 || eld_1_000_000 || eld_1_000_000_000 )
		{
			int str_length = Rstrlen( RWCHAR(eldstr_1) );

			if( str_length == 1 )
			{
				Rsprintf( RWCHAR(eldstr_1), _RT("00%d"), eld_1 );								// 1,000,000,000,[000] 
			}
			else if( str_length == 2 )
			{
				Rsprintf( RWCHAR(eldstr_1), _RT("0%d"), eld_1 );								// 1,000,000,000,[000] 
			}
		}		
	}
	else
	{
		if( eld_1_000 || eld_1_000_000 || eld_1_000_000_000 )
		{
			Rsprintf( RWCHAR(eldstr_1), _RT("000") );											// 1,000,000,000,[000] 		
		}
		else
		{
			Rsprintf( RWCHAR(eldstr_1), _RT("0") );											// 1,000,000,000,[000] 		
		}
	}
	
	if( eld_1_000 )
	{
		Rsprintf( RWCHAR(eldstr_1_000), _RT("%d,"), eld_1_000);								// 1,000,000,[000],000 

		if( eld_1_000_000 || eld_1_000_000_000 )
		{
			int str_length = Rstrlen( RWCHAR(eldstr_1_000) );

			if( str_length == 2 )
			{				
				Rsprintf( RWCHAR(eldstr_1_000), _RT("00%d,"), eld_1_000 );					// 1,000,000,[000],000 
			}
			else if( str_length == 3 )
			{				
				Rsprintf( RWCHAR(eldstr_1_000), _RT("0%d,"), eld_1_000 );						// 1,000,000,[000],000 
			}
		}	
	}
	else
	{
		Rsprintf( RWCHAR(eldstr_1_000), _RT("000,") );										// 1,000,000,[000],000 
	}

	if( eld_1_000_000 )
	{
		Rsprintf( RWCHAR(eldstr_1_000_000), _RT("%d,"), eld_1_000_000);						// 1,000,[000],000,000 

		if( eld_1_000_000_000 )
		{
			int str_length = Rstrlen( RWCHAR(eldstr_1_000_000) );

			if( str_length == 2 )
			{					
				Rsprintf( RWCHAR(eldstr_1_000_000), _RT("00%d,"), eld_1_000_000 );			// 1,000,[000],000,000 
			}
			else if( str_length == 3 )
			{					
				Rsprintf( RWCHAR(eldstr_1_000_000), _RT("0%d,"), eld_1_000_000 );				// 1,000,[000],000,000 
			}
		}	
	}
	else
	{
		Rsprintf( RWCHAR(eldstr_1_000_000), _RT("000,") );									// 1,000,[000],000,000 
	}

	if( eld_1_000_000_000 )
	{
		Rsprintf( RWCHAR(eldstr_1_000_000_000), _RT("%d,"), eld_1_000_000_000 );				// 1,[000],000,000,000 
	}
	else
	{
		Rsprintf( RWCHAR(eldstr_1_000_000_000), _RT("000,") );								// 1,[000],000,000,000 
	}
	

	if( eld_1_000_000_000 )
	{
		SAFE_STR_CPY( aEldText, eldstr_1_000_000_000, 256 ); 
		SAFE_STR_CAT( aEldText, eldstr_1_000_000, 256 ); 
		SAFE_STR_CAT( aEldText, eldstr_1_000, 256 ); 
		SAFE_STR_CAT( aEldText, eldstr_1, 256 ); 
	}
	else if( eld_1_000_000 )
	{
		SAFE_STR_CPY( aEldText, eldstr_1_000_000, 256 ); 
		SAFE_STR_CAT( aEldText, eldstr_1_000, 256 ); 
		SAFE_STR_CAT( aEldText, eldstr_1, 256 );  
	}
	else if( eld_1_000 )
	{		
		SAFE_STR_CPY( aEldText, eldstr_1_000, 256 ); 
		SAFE_STR_CAT( aEldText, eldstr_1, 256 );  
	}
	else 
	{
		SAFE_STR_CPY( aEldText, eldstr_1, 256 );  
	} 
	*/

}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CMoneyThrowWnd::Init()
{
	m_FrameWnd = NULL;
	m_EldInput = NULL;
	m_UseInfo = NULL;

	m_InputState = n_ELDINPUT_INVENTORY_MONEYTHROW;
}

//-----------------------------------------------------------------------------
void CMoneyThrowWnd::Composition()
{
	CButton*		btn;
	CImageBox*		img;

	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect money_throw_wnd;
	money_throw_wnd.w	= 204;
	money_throw_wnd.h	= 132;
	money_throw_wnd.x	= center_x - ( money_throw_wnd.w / 2 );
	money_throw_wnd.y	= center_y - ( money_throw_wnd.h / 2 );
	
	///-- Frame Wnd	
	m_FrameWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_MoneyThrow_Wnd, WID_None, money_throw_wnd.x, money_throw_wnd.y, money_throw_wnd.w, money_throw_wnd.h, false );	
	m_FrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_FrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_FrameWnd->SetIconTID( TID_CTRL_ICON_STATUS );	
	m_FrameWnd->SetWndTileSizeLR( 40, 29 );
	m_FrameWnd->SetWndTileSizeTB( 31, 11 );
	
	m_FrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_ELDTHROW ) );
	m_FrameWnd->SetTitle( true );
	m_FrameWnd->SetWndProc( this );		
	m_FrameWnd->SetVisible( false );
	m_FrameWnd->SetZorder( n_zoTop );
	m_FrameWnd->Initialize();


	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MoneyThrow_Wnd, 1, 33, 202, 63, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MoneyThrow_Wnd, 1, 32, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MoneyThrow_Wnd, 1, 96, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	

	///-- Slot	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MoneyThrow_Wnd, 13, 64, 23, 23, false );    
	img->SetClientImage( TID_CTRL_BTN_ELD );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MoneyThrow_Wnd, 36, 64, 132, 23, false );     
	img->SetClientImage( TID_CTRL_IMG_ELDTEXT132 );
	

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
		
	m_UseInfo = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MoneyThrow_InfoText, WID_MoneyThrow_Wnd, 10, 35, 184, 22, false );
	m_UseInfo->SetFontR( n_fRsvFontWndStatic );
	m_UseInfo->SetText( G_STRING_CHAR( IDS_WND_INPUT_THROW_ELD ) );
	///m_UseInfo->SetBlank( 2, 0 );	

	m_EldInput = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_MoneyThrow_Edit, WID_MoneyThrow_Wnd, 40, 66, 124, 20, false );
	m_EldInput->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
#ifdef UNICODE_ROCK
	m_EldInput->SetTextSize( 20 );
#else 
	m_EldInput->SetTextSize( 16 );
#endif 
	
	m_EldInput->SetClientImage( TID_None );
	m_EldInput->SetClientColor( 0, 0, 0 );
	m_EldInput->SetAlignText( n_atLeft, n_atCenter );
	m_EldInput->SetBlank( 2, 0, 0, 0 );
	m_EldInput->SetEditStyle( esText );
	m_EldInput->SetText( _RT("") );
	m_EldInput->SetEditStyle( esOnlyNumeric );
	m_EldInput->SetReturnActionClear( false );
	m_EldInput->InitEditBox();
	m_EldInput->SetHID( HID_None );

#ifdef EDITBOX_COMMA
	m_EldInput->SetNumberComma(true);
#endif


	///-- Ȯ�� ��ư
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MoneyThrow_OkBtn, WID_MoneyThrow_Wnd, 29, 101, 58, 25, false );   
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );	

	///-- ��� ��ư
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MoneyThrow_CancelBtn, WID_MoneyThrow_Wnd, 117, 101, 58, 25, false );    
	btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	btn->SetFontR( n_fRsvFontWndButton );   
	btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );	
}

//-----------------------------------------------------------------------------
void CMoneyThrowWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CMoneyThrowWnd::Proc( SEventMessage* EMsg )
{
	switch( EMsg->FocusWnd )
	{
	case WID_MoneyThrow_Edit:
		if( EMsg->Notify == NM_EDT_RETURN_ACTION )
		{
			OkClose();
		}
		break;
	case WID_MoneyThrow_OkBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			OkClose();
		}
		break;	
	case WID_MoneyThrow_CancelBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			CancelClose();
		}
		break;
	}
}

//-----------------------------------------------------------------------------	
void CMoneyThrowWnd::Open( N_ELDINPUTSTATE aInputState )
{
	m_InputState = aInputState;

	SAFE_STR_CPY( gEldStr , _RT("") , MAX_SIZE_64 );

	nRui->SendEvent( WID_MoneyThrow_Edit, n_emSetText, (EPARAM)gEldStr, 255, 255, 255 );
	nRui->SendEvent( WID_MoneyThrow_Edit, n_emSetFocus, 0, 0, 0, 0 );

	switch( m_InputState )
	{
	case n_ELDINPUT_INVENTORY_MONEYTHROW:
		{
			m_UseInfo->SetText( G_STRING_CHAR( IDS_WND_INPUT_THROW_ELD ) );			
		}
		break;
	case n_ELDINPUT_INVENTORY_EXCHANGE:
		{
			m_UseInfo->SetText( G_STRING_CHAR( IDS_WND_INPUT_EXCH_ELD ) );	
		}
		break;
	case n_ELDINPUT_INVENTORY_WAREHOUSE:
		{
			m_UseInfo->SetText( G_STRING_CHAR( IDS_WND_INPUT_WARE_ELD ) );
		}
		break;
	case n_ELDINPUT_EXCHANGE_DRAWOUT:
		{
			m_UseInfo->SetText( G_STRING_CHAR( IDS_WND_INPUT_EXD_ELD ) );	
		}
		break;
	case n_ELDINPUT_WAREHOUSE_DRAWOUT:
		{
			m_UseInfo->SetText( G_STRING_CHAR( IDS_WND_INPUT_WAD_ELD ) );
		}
		break;
	case n_ELDINPUT_FRONTIER_PAYMENT:
		{
			// ���â���� ȸ�� �����Ѵ�. 
			m_UseInfo->SetText( G_STRING_CHAR( IDS_WND_INPUT_PAYMENT ) );
		}
		break;					
	case n_ELDINPUT_FRONTIER_ELDSHARE:
		{
			// ���â���� ���带 �й��Ѵ�. 
			m_UseInfo->SetText( G_STRING_CHAR( IDS_WND_INPUT_ELDSHARE ) );
		}
		break;	
	case n_ELDINPUT_FRONTIER_PRESENT:
		{
			m_UseInfo->SetText( _RT("�����Ͻ� �ݾ��� �Է����ּ���") );	
		}
		break;
	}

	nRui->SendEvent( WID_MoneyThrow_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SetModal( WID_MoneyThrow_Wnd );
	
	return;
}

//-----------------------------------------------------------------------------	
void CMoneyThrowWnd::OkClose()
{
	long eld( 0 );
	char eldstr[256] = {0,};


	nRui->SendEvent( WID_MoneyThrow_Edit, n_emGetText, (EPARAM)gEldStr, gEldBufLen, 0, 0 );
#ifdef EDITBOX_COMMA
	m_EldInput->GetDeleteComma(gEldStr);
#endif

	eld = Ratol( RWCHAR(gEldStr) );
	
	nRui->SendEvent( WID_MoneyThrow_Edit, n_emEditDisable, 0, 0, 0, 0 );

	switch( m_InputState )
	{
	case n_ELDINPUT_INVENTORY_MONEYTHROW:
		{				
			g_RockClient.Send_RpCsDropEld( (DWORD)eld );
		}
		break;
	case n_ELDINPUT_INVENTORY_EXCHANGE:
		{
			if( nRui->GetPcParamInfo()->Money >= (DWORD)eld )
			{
				g_ExchangeWnd.m_MineEld = (DWORD)eld;
//				itoa( eld, eldstr, 10 );
				InventoryWnd.SetEldText( eldstr, (DWORD)eld );
				g_ExchangeWnd.m_MineEldText->SetText( eldstr );				
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
			}
		}
		break;
	case n_ELDINPUT_INVENTORY_WAREHOUSE:
		{		
			if( nRui->GetPcParamInfo()->Money >= (DWORD)eld )
			{
				g_RockClient.Send_RpCsDepositEld( (DWORD)eld );
				
//				g_WarehouseWnd.m_Eld += (DWORD)eld;
//				InventoryWnd.SetEldText( eldstr, g_WarehouseWnd.m_Eld );
//				g_WarehouseWnd.m_EldText->SetText( eldstr );
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
			}
		}
		break;
	case n_ELDINPUT_EXCHANGE_DRAWOUT:
		{ 
			if( g_ExchangeWnd.m_MineEld >= (DWORD)eld )
			{
				g_ExchangeWnd.m_MineEld -= (DWORD)eld;
//				itoa( g_ExchangeWnd.m_MineEld, eldstr, 10 );
				InventoryWnd.SetEldText( eldstr, g_ExchangeWnd.m_MineEld );
				g_ExchangeWnd.m_MineEldText->SetText( eldstr );						
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
			}
		}
		break;
	case n_ELDINPUT_WAREHOUSE_DRAWOUT:
		{
			if( g_WarehouseWnd.m_Eld >= (DWORD)eld )
			{			
				g_RockClient.Send_RpCsDrawEld( (DWORD)eld );							
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
			}
		}
		break;
	case n_ELDINPUT_FRONTIER_PAYMENT:
		{
			if( nRui->GetPcParamInfo()->Money >= (DWORD)eld )
			{
#ifdef FRONTIER		
				g_RockClient.Send_CsPayDues( eld );
#endif 
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
			}
		}
		break;					
	case n_ELDINPUT_FRONTIER_ELDSHARE:
		{
			// ��� ���� ����� ���� �ʿ�!!! 
			if( nRui->GetPcParamInfo()->Money >= (DWORD)eld )
			{
#ifdef FRONTIER		
				g_RockClient.Send_CsMsngrDivideDues( eld );
#endif
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
			}
		}
		break;	
	case n_ELDINPUT_FRONTIER_PRESENT:
		{		
			if( nRui->GetPcParamInfo()->Money >= (DWORD)eld )
			{		
				g_ProposeJoinFrontier.m_Eld += (DWORD)eld;
				InventoryWnd.SetEldText( eldstr, g_ProposeJoinFrontier.m_Eld );
				g_ProposeJoinFrontier.m_EldText->SetText( eldstr );
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
			}
		}
		break;
	}

	nRui->SendEvent( WID_MoneyThrow_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	if( ChatBoxWnd.IsVisible == true )
	{
		nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetFocus, 0, 0, 0, 0 );
	}

	return;
}

//-----------------------------------------------------------------------------	
void CMoneyThrowWnd::CancelClose()
{	
	switch( m_InputState )
	{
	case n_ELDINPUT_INVENTORY_MONEYTHROW:
	case n_ELDINPUT_INVENTORY_EXCHANGE:
	case n_ELDINPUT_INVENTORY_WAREHOUSE:
	case n_ELDINPUT_EXCHANGE_DRAWOUT:
	case n_ELDINPUT_WAREHOUSE_DRAWOUT:		
	case n_ELDINPUT_FRONTIER_PAYMENT:
	case n_ELDINPUT_FRONTIER_ELDSHARE:
	case n_ELDINPUT_FRONTIER_PRESENT:
		{
		}
		break;
	}

	nRui->SendEvent( WID_MoneyThrow_Edit, n_emEditDisable, 0, 0, 0, 0 );
	nRui->SendEvent( WID_MoneyThrow_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	if( ChatBoxWnd.IsVisible == true )
	{
		nRui->SendEvent( WID_ChatBox_MessageEdit, n_emSetFocus, 0, 0, 0, 0 );
	}

	return;
}

void CMoneyThrowWnd::DeleteComma(char *Str)
{





}

//-----------------------------------------------------------------------------	








