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
#include "ItemEldaMixRWndProc.h"
#include "stringmanager.h"
#include "ItemEldaMixAWndProc.h"
#include "SOUND\Sound.H"
#include "ItemGetButton.h"
#include "MainMenuWndProc.h"
#include "InventoryWndProc.h"
#include "StrengthStoneWndProc.h"
#include "NpcChatWnd.h"


CItemEldaMixRWndProc	g_ItemEldaMixRWndProc;
extern long	 g_nowTime;


CItemEldaMixRWndProc::CItemEldaMixRWndProc()
{
	bSuccess = false;
	m_bEndMixElda = false;
};


//-----------------------------------------------------------------------------
void CItemEldaMixRWndProc::Init()
{

}

//-----------------------------------------------------------------------------
void CItemEldaMixRWndProc::Composition()
{
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
	FrameWnd->SetIconTID( TID_CTRL_ICON_NONE );	
	FrameWnd->SetWndTileSizeLR( 40, 29 );
	FrameWnd->SetWndTileSizeTB( 31, 11 );

	FrameWnd->SetCaption( G_STRING_CHAR(IDS_WND_RESULT_WINDOWS) );	
	FrameWnd->SetTitle( true );
	FrameWnd->SetWndProc( this );
	FrameWnd->SetZorder(n_zoTop);
	FrameWnd->SetVisible( false );
	FrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(), 1, 111, 202, 72, false );
	img->SetClientImage( TID_CTRL_BACK );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(),  1, 110, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(), 1, 183, 202, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	
	///-- Slot

	m_FailImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(), 85, 52, 35, 35, false );  
	m_FailImg->SetClientImage( TID_CTRL_SLOT_SUNKEN );
	m_FailImg->SetVisible(false);
	///-- 원석 (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(),  52, 93, 100, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );
	tbox->SetText( G_STRING_CHAR(IDS_WND_RESULT_ITEM) );


	///-- 아이템 Slot
	mcp_item_slot = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, FrameWnd->GetID(), 85, 52, 35, 35, false );
	mcp_item_slot->PcItem = &mc_itemslot_item;		
	mcp_item_slot->SetSlotGroup( n_SGROUP_TUNING );
	mcp_item_slot->SetHID( HID_ITEMTIP );
	
	///-- 확인 버튼
	Ok_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, FrameWnd->GetID(), 74, 189, 58, 25, false );
	Ok_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	Ok_btn->SetFontR( n_fRsvFontWndButton );   
	Ok_btn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
	Ok_btn->SetBtnHold(true);
	///-- Exp
	mcp_exp = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 5, 111, 190, 14, false );
	mcp_exp->SetFontR( n_fRsvFontWndStatic );	

	///-- Penalty 
	mcp_exp_penalty = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 15, 124, 190, 14, false );
	mcp_exp_penalty->SetFontR( n_fRsvFontWndStatic );	
	
	///-- Effect
	mcp_effect[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 5, 139, 190, 14, false );
	mcp_effect[0]->SetFontR( n_fRsvFontWndStatic );		
	mcp_effect[0]->SetVisible(false);

	///-- Effect
	mcp_effect[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 5, 154, 190, 14, false );
	mcp_effect[1]->SetFontR( n_fRsvFontWndStatic );		
	mcp_effect[0]->SetVisible(true);
	///-- Effect
	mcp_effect[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 5, 169, 190, 14, false );
	mcp_effect[2]->SetFontR( n_fRsvFontWndStatic );	
	mcp_effect[2]->SetVisible(true);

	///-- image	
	mcp_icon_x = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, FrameWnd->GetID(), 89, 56, 27, 27, false );
	mcp_icon_x->SetClientImage( TID_C_WND_ELDA_SB_X );
	mcp_icon_x->SetVisible(false);
	///-- Result text
	mcp_result_text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, FrameWnd->GetID(), 50, 37, 104, 12, false );
	mcp_result_text->SetFontR( n_fRsvFontWndStatic );




	m_SpriteElda[0] = (CImageAni*)nRui->CreateWnd( n_wtImageAni, WID_None, FrameWnd->GetID(), -105 , -130 , 400, 400, false );
	m_SpriteElda[0]->SetClientImage( TID_ELDA_EFFECT_01 );
	m_SpriteElda[0]->SetInfoSprite(CImageAni::SIZE_DOWN , 11 , -1 , 0 , 1 );
	m_SpriteElda[0]->SetSize(400 , 4.0f);
	m_SpriteElda[0]->SetFocusCheck(false);
	m_SpriteElda[0]->SetPlay(false);
	m_SpriteElda[0]->SetZorder( n_zoTop );
//	m_SpriteElda[0]->SetPlay(1);
	
	m_SpriteElda[1] = (CImageAni*)nRui->CreateWnd( n_wtImageAni, WID_None, FrameWnd->GetID(), 100 , 70 , 0, 0, false );
	m_SpriteElda[1]->SetClientImage( TID_ELDA_EFFECT_BOMB_01 );
	m_SpriteElda[1]->SetInfoSprite(CImageAni::SIZE_UP , 6 , -1 , 0 , 1 );
	m_SpriteElda[1]->SetSize(300 , 15.0f);
	m_SpriteElda[1]->SetFocusCheck(false);
	m_SpriteElda[1]->SetClientColor(255,0,0);
	m_SpriteElda[1]->SetPlay(false);
	m_SpriteElda[1]->SetZorder( n_zoTop );
	
	



}
void CItemEldaMixRWndProc::Close()
{
	nRui->SendEvent( FrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}


//by sdh
//성공시 이펙트 초기화 시킨다. 
void CItemEldaMixRWndProc::EffectInit()
{


	m_SpriteElda[0]->SetPlay(true);
	m_SpriteElda[1]->SetPlay(false);

	bOneCount = false;
	m_bEndMixElda = false;

	mcp_item_slot->SetVisible(false);
	m_FailImg->SetVisible(false);
	mcp_icon_x->SetVisible(false);
	mcp_result_text->SetVisible(false);
	mcp_effect[0]->SetVisible(false);
	mcp_effect[1]->SetVisible(false);
	mcp_effect[2]->SetVisible(false);
	g_Sound.Play( 345 );

	
}

//effect if 
void CItemEldaMixRWndProc::EffectUpdate()
{

	if( (m_SpriteElda[0]->GetEndSprite() == true ) && (m_SpriteElda[0]->GetPlay() == true) )
	{
		m_SpriteElda[0]->SetPlay(false);
		m_SpriteElda[1]->SetPlay(true);

		g_Sound.Play( 346 );
	}

	if( m_SpriteElda[1]->GetEndSprite() == true  && (!bOneCount) )
	{
		bOneCount = true;
		OneTimer = g_nowTime;
	}
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , OneTimer ) , >= ,  300 )  && 
		(bOneCount) )
	{
		
		bOneCount = false;
		
		m_bEndMixElda =true; 

		if( bSuccess ) 
		{
			
			char tmpStr[256]={0,};
			SPcDataInven* PcInven =  nRui->GetPcInven();
			SPcItem*	tmpPcItem = NULL;
			g_RockClient.CreateItemPcItem(&tmpPcItem , m_MixElda);			
		
			SItemBaseInfo* item_Info = NULL;
			item_Info = g_Pc_Manager.GetItemBaseInfoPtr( m_MixElda.theItem.theCode );	
			if( item_Info == NULL)
			{
				SAFE_DELETE( tmpPcItem );
				return;
			}


			CItemTransButton *pItemTransButton = g_main_buttons.GetInVenButton();
			SItemTransInfo ItemTransInfo;
			ItemTransInfo.Is3DPos = false ;
			ItemTransInfo.interNpcId = 0;
			ItemTransInfo.isMoney = false;
			ItemTransInfo.isGet = true;
			ItemTransInfo.pcInfo = *tmpPcItem;			
			ItemTransInfo.isMonster = false;
			pItemTransButton->InsertItemTransInfo(ItemTransInfo);
						
		
			PcInven->InvenItem[m_MixElda.theItem.thePosition] = tmpPcItem;

			mcp_item_slot->SetSlotPcItem( *(PcInven->InvenItem[m_MixElda.theItem.thePosition]) );
			mcp_item_slot->PcItem->Amount = 1;
					
			Rsprintf(RWCHAR(tmpStr) , _RT("%s") ,RWCHAR( mcp_item_slot->PcItem->ItemTable->theName) );
			mcp_effect[0]->SetText(tmpStr);	
			mcp_effect[1]->SetText(G_STRING_CHAR(IDS_WND_GET_ITEM));
			
			switch(m_eItemMode)
			{
			case eEldaMix:
				//SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FAILMIXELDASTONE) );
				break; 
			case eStrengthStone:
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_SPELL_STONE_SUCC) );	//HAN_MSG
				break; 
			}

			InventoryWnd.UpdateInvenSlotItem();
			
			g_Sound.Play( 101 );
		}
		else
		{

			m_FailImg->SetVisible(true);
			mcp_icon_x->SetVisible(true);
			g_Sound.Play( 106 );
			switch(m_eItemMode)
			{
				case eEldaMix:
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FAILMIXELDASTONE) );
					break; 
				case eStrengthStone:
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_SPELL_STONE_FAIL) );
					break; 
			}
			
		}

		mcp_item_slot->SetVisible(true);
	
		mcp_result_text->SetVisible(true);
		mcp_effect[0]->SetVisible(true);
		mcp_effect[1]->SetVisible(true);
		mcp_effect[2]->SetVisible(true);

		m_SpriteElda[0]->SetPlay(false);
		m_SpriteElda[1]->SetPlay(false);

	}



}





///---------------------------------------------------------------------------
///-- OpenEnchantRWnd
///---------------------------------------------------------------------------
///-- 아이템 강화 성공시 Wnd
void CItemEldaMixRWndProc::OpenEnchantR_SWnd(URpdItem MixElda , eITEMMODE Mode )
{
	int i = 0;
	TCHAR tmpStr[128] = {0,};
	bSuccess = true;
	m_MixElda = MixElda;
	
	m_eItemMode = Mode; 
	 

	//자리를 찾는다 . 
	//그리고 이벤엔 집어 넣는다. 
	//서버에서 자리를 받아와야 하낟.. 지금음 테스팅 .. ㅠㅠ
	mcp_item_slot->PcItem->Clear();
	mcp_result_text->SetText( G_STRING_CHAR( IDS_SUCCESS ) );
	mcp_item_slot->SetVisible(true);
	mcp_icon_x->SetVisible( false );	
	FrameWnd->Reposition(521 , 384); 
	nRui->SendEvent( FrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
	EffectInit();
}

///---------------------------------------------------------------------------
///-- OpenEnchantR_FWnd
///---------------------------------------------------------------------------
///-- 아이템 강화 실패시 Wnd
void CItemEldaMixRWndProc::OpenEnchantR_FWnd(eITEMMODE Mode)
{
	int i = 0 ;
	TCHAR tmpStr[128] = {0,};
	
	m_eItemMode = Mode;
	
	bSuccess = false; 

	mcp_effect[0]->SetText(G_STRING_CHAR(IDS_WND_NEXT_CHANCE));
	mcp_effect[1]->SetText("");

	///-- Image
	mcp_result_text->SetText( G_STRING_CHAR( IDS_FAIL )); 
	mcp_item_slot->PcItem->Clear();
	mcp_item_slot->SetVisible(false);
	mcp_icon_x->SetVisible( true );	

	FrameWnd->Reposition(521 , 384); 
	nRui->SendEvent( FrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	EffectInit();

}


///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
void CItemEldaMixRWndProc::Update()
{
	EffectUpdate();
	
	if( !m_bEndMixElda ) 
	{
		Ok_btn->SetBtnHold(true);
	}
	else 
	{
		Ok_btn->SetBtnHold(false);
	}

}


///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CItemEldaMixRWndProc::Proc( SEventMessage* EMsg )
{

	if( EMsg->FocusWnd == Ok_btn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( FrameWnd->GetID() , n_emSetVisible , (EPARAM)false , 0 , 0 , 0 );
			//창다시 띄우기 
			if( !NpcChatWnd.IsVisible)
				return;

			switch(m_eItemMode)
			{
				case eEldaMix:
					g_ItemEldaMixAWndProc.Open(g_ItemEldaMixAWndProc.GetWindowsState());
					break; 
				case eStrengthStone:
					g_StrengthStoneWndProc->OpenEnchantWnd();
					break; 
			}

			
		}
	}
}


///---------------------------------------------------------------------------
///-- EnchantText
///---------------------------------------------------------------------------
void CItemEldaMixRWndProc::EnchantText( bool flag )
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