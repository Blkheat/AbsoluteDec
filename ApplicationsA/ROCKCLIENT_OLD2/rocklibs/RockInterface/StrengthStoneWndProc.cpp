
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
#include "StrengthStoneWndProc.h"
#include "StringManager.h"

//�ֹ��� ���� ������ �����ϱ�
DWORD g_Cost[10] = {  10000 ,	20000 ,	50000 , 80000 , 150000 , 300000 , 600000 , 1000000 , 2000000 , 3000000 };

CStrengthStoneWndProc	*g_StrengthStoneWndProc;

CStrengthStoneWndProc::CStrengthStoneWndProc()
{
	m_IsViewchargeWindow = FALSE;

	m_pWholeFrame = NULL;				
	mpC_framewnd = NULL;
}

CStrengthStoneWndProc::~CStrengthStoneWndProc()
{

}

//-----------------------------------------------------------------------------
void CStrengthStoneWndProc::Init()
{
	mcp_Level = NULL;
	mcp_Item = NULL;
}


///---------------------------------------------------------------------------
///-- OpenEnchantWnd
///---------------------------------------------------------------------------
///-- ���� â�� ����
///-- Slot �� �ʱ�ȭ �Ѵ�.
///-- 
void CStrengthStoneWndProc::OpenEnchantWnd()
{
	TCHAR tmpStr[128] = {0,};
	
	memset( m_dPayItem, NULL, sizeof( m_dPayItem ) );
	
	nRui->SendEvent( m_pWholeFrame->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	   
	Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT(""));
	     
	mcp_Level->SetText( tmpStr );
	mcp_textResult->SetText(tmpStr);
		
	mcp_item_slot->PcItem->Clear();
	mcp_SpellStone_slot->PcItem->Clear();
	mcp_ResultImg->SetClientImage(TID_None);

	mcp_start_btn->SetBtnHold( true );
	mb_button_lock = false;		

	for( int i = 0 ; i < 3 ; ++i )
	{
		mcp_Chargeitem_slot[i]->PcItem->Clear();
	}

}
///---------------------------------------------------------------------------
///-- CloseEnchantWnd
///---------------------------------------------------------------------------
///-- ���� â�� �ݴ´�.
///-- �� Resource �� �����Ѵ�. ( Dynamic Texture_M �� �����Ǿ�� �� ) 
void CStrengthStoneWndProc::CloseEnchantWnd()
{
	nRui->SendEvent( m_pWholeFrame->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	///TID_ELD_WND_UpGrade ����

}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------

void CStrengthStoneWndProc::Composition()
{
	
	CFrameWnd*		fw = NULL;
	CImageBox*		img = NULL;
	CButton*		btn = NULL;
	CTextBox*		tbox =NULL;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect enchantwnd;


	int AddHeight = 36 ; 

	enchantwnd.w	= 254;
	enchantwnd.h	= 235 + AddHeight ;
	enchantwnd.x	= center_x - ( enchantwnd.w / 2 );
	enchantwnd.y	= center_y - ( enchantwnd.h / 2 );
	///-- Frame Wnd
	m_pWholeFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
									  enchantwnd.x, enchantwnd.y, enchantwnd.w, enchantwnd.h, false );	
	m_pWholeFrame->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pWholeFrame->SetClientImage( TID_CTRL_WND_L1 );
	m_pWholeFrame->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pWholeFrame->SetWndTileSizeLR( 40, 29 );
	m_pWholeFrame->SetWndTileSizeTB( 31, 11 );

	m_pWholeFrame->SetCaption( G_STRING_CHAR(IDS_WND_MAKE_STRONG_STONE) );	
	m_pWholeFrame->SetTitle( true );	
	m_pWholeFrame->SetWndProc( this );
	m_pWholeFrame->SetVisible( false );
	m_pWholeFrame->Initialize();

	///-- Frame Wnd
	mpC_framewnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None , m_pWholeFrame->GetID(), 
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


	

	///-- Result img 
	sTempRect.x	= (enchantwnd.w/2)- (35/2);
	sTempRect.y	= 140;
	sTempRect.w	= 35;
	sTempRect.h	= 35;
	
	mcp_ResultImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpC_framewnd->GetID(), 
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );            
	mcp_ResultImg->SetClientImage( TID_None );





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
	tbox->SetText( G_STRING_CHAR(IDS_WND_SPELL_STONE));	

	sTempRect.x	= 70;
	sTempRect.y	= 95 ;
	sTempRect.w	= 110;
	sTempRect.h	= 17;
	
	///-- �ռ�ȿ��  ( Static Text )
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR(IDS_ESSENCE_CREATE_ITEM) ); 
	
	m_MoneyText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 1,  95+18 , enchantwnd.w-30 , 17 , false );
	m_MoneyText->SetFontR( n_fRsvFontWndStatic );	
	m_MoneyText->SetAlignText( n_atRight , n_atCenter );
	m_MoneyText->SetText( G_STRING_CHAR(IDS_WND_EXPENSE) );
	
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- ���� ��ư
	sTempRect.x	= 235;
	sTempRect.y	= 6;
	sTempRect.w	= 13;
	sTempRect.h	= 13;

	///-- �ݱ�
	m_CloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, mpC_framewnd->GetID(), 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	m_CloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	sTempRect.x	= 56;
	sTempRect.y	= 171 + AddHeight ;
	sTempRect.w	= 58;
	sTempRect.h	= 25;

	mcp_start_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, mpC_framewnd->GetID(), 
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
	m_StopBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, mpC_framewnd->GetID(), 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	m_StopBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );	
	m_StopBtn->SetFontR( n_fRsvFontWndButton ); 
	m_StopBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 

	sTempRect.x	= 58;
	sTempRect.y	= 56;
	sTempRect.w	= 35;
	sTempRect.h	= 35;

		
	///-- ������ Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, mpC_framewnd->GetID(), 
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );	
	mcp_item_slot->SetHID(HID_ITEMTIP);
	sTempRect.x	= 157;
	sTempRect.y	= 56;
	sTempRect.w	= 35;
	sTempRect.h	= 35;

	///-- Elda Slot
	mcp_SpellStone_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, mpC_framewnd->GetID(), 
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	mcp_SpellStone_slot->SetHID(HID_ITEMTIP);
/*
	sTempRect.x	= (enchantwnd.w/2)- (35/2);
	sTempRect.y	= 140;
	sTempRect.w	= 35;
	sTempRect.h	= 35;

	mcp_ResultSlot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, mpC_framewnd->GetID(), 
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	
	mcp_ResultSlot->SetSlotGroup( n_SGROUP_TUNING );
*/
	sTempRect.x	= (enchantwnd.w/2)- (35/2);
	sTempRect.y	= 140+36;
	sTempRect.w	= 35;
	sTempRect.h	= 35;

	mcp_textResult = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	mcp_textResult->SetFontR( n_fRsvFontWndStatic );
	mcp_textResult->SetText( _RT("") ); 
	

	

	///-- None Item ����
	mcp_item_slot->PcItem = &mc_itemslot_item;		
	mcp_SpellStone_slot->PcItem = &mc_SpellStone_item;

	
	sTempRect.x	= 115;
	sTempRect.y	= 67;
	sTempRect.w	= 20;
	sTempRect.h	= 14;

	///-- Skill Level
	mcp_Level = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, mpC_framewnd->GetID(), 
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
	mpCharge_framewnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None, m_pWholeFrame->GetID() , 
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


void CStrengthStoneWndProc::OpenCharge_Wnd()
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

void CStrengthStoneWndProc::CloseCharge_Wnd()
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
void CStrengthStoneWndProc::Update()
{
	static TCHAR tmpStr[128] = {0,};	

	///mcp_SpellStone_slot->PcItem->ItemTable->
	if( mcp_item_slot->PcItem->IType != n_ITYPE_NONE && 
		mcp_SpellStone_slot->PcItem->IType != n_ITYPE_NONE )
	{	
		///-- enchant Text
		///-- Level
	}
	else if(mcp_item_slot->PcItem->IType != n_ITYPE_NONE )
	{
		int Grade = mcp_item_slot->PcItem->ItemTable->theGrade/100; 
		
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), Grade  );
		mcp_Level->SetText( tmpStr );
		
		Rsprintf( RWCHAR(tmpStr), _RT("%s : %d"), G_STRING_CHAR(IDS_WND_EXPENSE)  , g_Cost[Grade-1] );
		m_MoneyText->SetText(tmpStr );

	}
	else
	{
		//-- Effect 1
		Rsprintf( RWCHAR(tmpStr), _RT("%s"), _RT("") );
	
		mcp_Level->SetText( tmpStr );
		m_MoneyText->SetText(tmpStr )	;
		mcp_ResultImg->SetClientImage(TID_None);
	}

	CheckEnchantItemList();
}


///---------------------------------------------------------------------------
///-- CheckItemList - Inven ���濡 ���� ��þƮ â�� ����
///---------------------------------------------------------------------------
BOOL CStrengthStoneWndProc::CheckEnchantItemList()
{
	int i = 0;
	int j = 0;
	
	BOOL lb_isFind_item = FALSE;
	BOOL lb_isFind_elda = FALSE;	
	BOOL lb_isFind_Chargeitem[3] = { FALSE, FALSE, FALSE };	
	
	if( !mcp_item_slot->PcItem && !mcp_SpellStone_slot->PcItem &&
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
		if( mcp_SpellStone_slot->PcItem && 
			mcp_SpellStone_slot->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
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
		mcp_SpellStone_slot->PcItem->Clear();
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
		mcp_SpellStone_slot->PcItem->IType != n_ITYPE_NONE )
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
void CStrengthStoneWndProc::Proc( SEventMessage* EMsg )
{
	if( m_CloseBtn->GetID() == EMsg->FocusWnd || 
		m_StopBtn->GetID() == EMsg->FocusWnd	)
	{
		if( EMsg->Notify == NM_BTN_CLICK )	
		{
			nRui->SendEvent( m_pWholeFrame->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			return;
		}
	}


	if( mcp_item_slot->GetID() == EMsg->FocusWnd || 
		mcp_SpellStone_slot->GetID() == EMsg->FocusWnd )
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
			mcp_SpellStone_slot->PcItem->IType != n_ITYPE_NONE )
		{
			mcp_start_btn->SetBtnHold( false );				
		}
		else
		{
			mcp_start_btn->SetBtnHold( true );
		}
	}


	if( mcp_start_btn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK && !mb_button_lock )
		{	
			if( mc_SpellStone_item.IType != n_ITYPE_NONE &&
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
			
			mb_button_lock = true;			
			g_RockClient.Send_SRpCsUseStrengthstone(mcp_SpellStone_slot->PcItem->ItemUID , 
				mcp_item_slot->PcItem->ItemUID , m_dPayItem  );
			CloseEnchantWnd();
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
void CStrengthStoneWndProc::ItemSlotDBClick( SEventMessage* EMsg )
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
void CStrengthStoneWndProc::ItemSlotClick( SEventMessage* EMsg )
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
				///-- ���� üũ 
				///-- By simwoosung �� �κп��� ���� ũ����Ʈ�� 
				///-- �Ƹ� ũ����Ʈ ���� ���� ������ Ŭ���� ���� ��ƾ �߰�
				
				bool bIsEnchantItemSubJob = IsEnchantItemSubJob(srcslot);
				
				//���� �������� ��ȭ �ȵǰ�..
				if( srcslot->PcItem->ItemTable->theType == n_Type_Scroll)
				{
					bIsEnchantItemSubJob = false; 
				}

				if(!bIsEnchantItemSubJob)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOTUPGRADE_THEITEM) );
					return;
				}
				
				///-- Copy item
				targetslot->SetSlotPcItem( *(srcslot->PcItem) );
				///-- Del Cursor
				//targetslot->PcItem = srcslot->PcItem;
				
			
				//�̸� ������ �����ֱ� 
				
		//		mcp_ResultSlot->SetSlotPcItem( *srcslot->PcItem );
				
				mcp_ResultImg->SetClientImage( srcslot->PcItem->Tid );
				SItemBaseInfo* item_Info = NULL;
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( mcp_item_slot->PcItem->ItemTable->theUpgradeItem );	
				mcp_textResult->SetText(item_Info->theName);

				nRui->ResetCursorItem();	

			}
			else if ( srcslot->Group == n_SGROUP_WEAR)
			{				
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_ENCHANT_WEAR ) );			
			}
		}
		///-------------------------------------------------------------------
		///-- elda Slot
		else if( targetslot == mcp_SpellStone_slot )
		{
		
			if(mcp_item_slot->PcItem->IType == n_ITYPE_NONE)
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FIRSTITEM_WEAPONS_OR_ARMOR) );	
				return;
			}
			else if( g_Pc.m_MainPC.char_info.theCitizen != srcslot->PcItem->ItemTable->theCitizen ) 
			{
			//	SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ANOTHER_NATION_ITEM) );	
				return;
			}
			else if( (srcslot->PcItem->ItemTable->theGrade/100) != (mcp_item_slot->PcItem->ItemTable->theGrade/100) )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FIXTURES_LEVEL_AND_LEVEL_DIFF) );	
				return;
			}

			if( mb_craft_weapon )
			{	
				if( srcslot->PcItem->Class == n_Class_Weapon_StrengthStone  )
				{
					targetslot->SetSlotPcItem( *(srcslot->PcItem) );	
					targetslot->PcItem->Amount = 1;
					//targetslot->PcItem = srcslot->PcItem;
					///-- Del Cursor
					nRui->ResetCursorItem();
				}
				else 
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PUTON_WEPON_STONE) );
				}

			}
			else 
			{
				if( srcslot->PcItem->Class == n_Class_Armor_StrengthStone )
				{
					///-- Copy item
					targetslot->SetSlotPcItem( *(srcslot->PcItem) );	
					targetslot->PcItem->Amount = 1;
					//targetslot->PcItem = srcslot->PcItem;
					///-- Del Cursor
					nRui->ResetCursorItem();
				}
				else 
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PUTON_ARMOR_STONE));
				}
			}
			
		}
		//-- ���� ������ Slot
		else if( targetslot == mcp_Chargeitem_slot[0] || targetslot == mcp_Chargeitem_slot[1] || 
				 targetslot == mcp_Chargeitem_slot[2] )
		{
			if( srcslot->Group == n_SGROUP_INVENTORY )
			{
				if(mcp_item_slot->PcItem->IType == n_ITYPE_NONE)
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FIRSTITEM_WEAPONS_OR_ARMOR) );	
					return;
				}

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
				if( ( n_Scroll_MGItemBonus != nScrollType ) && 
					( n_Scroll_MGLevelBonus != nScrollType ) && 
					(n_Scroll_DoubleUpgrade != nScrollType ) &&
					( n_Scroll_CriticalDie != nScrollType )	)
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
				
				//��ŵ ������ �����ܰ谡 ���� �ϴ��� üũ�ؼ� ������ �ѱ�� . 
				if( nScrollType == n_Scroll_DoubleUpgrade )
				{
					int UpGradeItemCode = mcp_item_slot->PcItem->ItemTable->theUpgradeItem ; 
					
					SItemBaseInfo* item_Info = NULL;
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( UpGradeItemCode );	
					
					if( item_Info->theUpgradeItem == 0 )
					{
						SystemPrint( n_DarkRed, n_SystemMsg,  G_STRING_CHAR(IDS_SYSPR_SCROLL_DOUBLEUPGRADE_ERR) , srcslot->PcItem->ItemTable->theName );	//HAN_MSG 
						return;
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

bool CStrengthStoneWndProc::IsEnchantItemSubJob(CSlot * pSlot)
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
	
	if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotUpgradeSetStome ) )
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
		/*
		if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max ) )
		{
			n_make_type = pSlot->PcItem->ItemTable->theType;		
			bIsMakeItemSubJob = true;

			//mcp_Item->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );
			goto S_END;
		}
		*/
		///--JAPAN_BUG_MODIFY
		///-- �� �̻� ��ȭ�Ұ�
		if(!(pSlot->PcItem->ItemTable->theUpgradeItem))
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
		mb_craft_weapon = false ; 

		mcp_Item->SetText( G_STRING_CHAR(IDS_CHARGE_ITEM) );
	
		goto S_END;
	}

	
	if( nRui->GetPcParamInfo()->Citizen == MILLENA )
	{
		switch(pSlot->PcItem->ItemTable->theType)
		{
		case n_Type_Sword:				///-- ��
		case n_Type_TwohandSword:	   ///-- ���
		case n_Type_Bow:				///-- Ȱ
			{
				bIsMakeItemSubJob = true;
				mb_craft_weapon = true ;
			}
			break;
		case n_Type_LightCoat:	   ///-- �з��� �Ƹ� �����
		case n_Type_LightPants:	
		case n_Type_LightGlove:	 
		case n_Type_LightShoes:
		case n_Type_MiddleCoat:    ///-- �з��� �Ƹ� �����
		case n_Type_MiddlePants:	
		case n_Type_MiddleGlove:	 
		case n_Type_MiddleShoes:
		case n_Type_HeavyCoat:    ///-- �з��� �Ƹ� ������
		case n_Type_HeavyPants:	
		case n_Type_HeavyGlove:	 
		case n_Type_HeavyShoes:
		case n_Type_Shield:		  ///-- ����	
		case n_Type_KeepupHair:
		case n_Type_ChangeHair:
		case n_Type_SetCloth: 
			{	
				mb_craft_weapon = false;
				bIsMakeItemSubJob = true;
			}
			break;
			
		}	
		
	}
	else
	{
		switch(pSlot->PcItem->ItemTable->theType)
		{
		case n_Type_MagicBook:			///-- ������
		case n_Type_Staff:				///-- ������
		case n_Type_CrystalBall:	   ///-- ������
			{
				mb_craft_weapon = true; 
				bIsMakeItemSubJob = true;
			}
			break;
		case n_Type_MagicCoat:    ///-- ���� ������
		case n_Type_MagicPants:	
		case n_Type_MagicGlove:	 
		case n_Type_MagicShoes:
		case n_Type_PriestCoat:    ///-- ���� ������
		case n_Type_PriestPants:	
		case n_Type_PriestGlove:	 
		case n_Type_PriestShoes:
		case n_Type_SpiritCoat:    ///-- ���� ���ź�
		case n_Type_SpiritPants:	
		case n_Type_SpiritGlove:	 
		case n_Type_SpiritShoes:
		case n_Type_KeepupHair:
		case n_Type_ChangeHair:
		case n_Type_SetCloth: 
			{	
				mb_craft_weapon = false; 
				bIsMakeItemSubJob = true;
			}
			break;
		}
		
	}
	
S_END:
	
	return bIsMakeItemSubJob;
}

bool CStrengthStoneWndProc::IsEnchantEldaSubJob(CSlot * pSlot)
{

	return TRUE;
}

///---------------------------------------------------------------------------
///-- EnchantText
///---------------------------------------------------------------------------
void CStrengthStoneWndProc::EnchantText()
{
			
				
}

