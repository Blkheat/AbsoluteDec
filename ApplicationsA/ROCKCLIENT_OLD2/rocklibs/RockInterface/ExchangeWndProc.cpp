#include "..\\RockPCH.h"


#include "Rui.h"


#include "..\\..\\brother.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\NetworkMgr.h"
#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\bravolibs\\Sound\\Sound.h"


#include "ExchangeWndProc.h"

#include "SystemMenuWndProc.h"
#include "CharStateWndProc.h"
#include "SkillWndProc.h"
#include "InventoryWndProc.h"
#include "QuestProc.h"

#include "NumberInputWnd.h"
#include "NpcChatWnd.h"
#include "WarehouseWndProc.h"
#include "GuildInventoryWndProc.h"
#include "StringManager.h"



CExchangeWnd		g_ExchangeWnd;

extern long			g_nowTime;


//-----------------------------------------------------------------------------
void CExchangeWnd::Init()
{
	m_bIsExchangeEnable = TRUE;
	m_bIsExchangeLock = FALSE;
	
	m_bIsAccept = FALSE;
	
	m_bIsYoursUpload = FALSE;
	
	m_ExchangePcIndex = 0;
	
	m_MineEld = 0;
	m_YoursEld = 0;
	
	return;
}


//-----------------------------------------------------------------------------
void CExchangeWnd::Composition()
{
	CFrameWnd*		fw		= NULL;
	CTextBox*		tbox;
	CButton*		btn		= NULL;
	CImageBox*		img		= NULL;
	int i,j;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	if(true)
		
#else
		
	if( nRui->GetResolution() == n_1024X768 )
			
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	{		
		fw = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_Exchange_Wnd, WID_None, 250, 245, 204, 290, false );
		fw->SetWndTexStyle( fwfTex_Style_Tile9 );
		fw->SetClientImage( TID_CTRL_WND_L1 );
		fw->SetIconTID( TID_CTRL_ICON_INVEN );	
		fw->SetWndTileSizeLR( 40, 29 );
		fw->SetWndTileSizeTB( 31, 11 );		
		
		fw->SetCaption( G_STRING_CHAR( IDS_WND_EXCHANGE )); 
		fw->SetTitle( true );
		fw->SetWndProc( this );	
		fw->SetVisible( false );
		fw->Initialize();
		
		///---------------------------------------------------------------------------	
		///-- Static
		///---------------------------------------------------------------------------		
		
		///-- Line
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 1, 69, 202, 79, false );
		img->SetClientImage( TID_CTRL_BACK );
		img->SetFocusCheck( false );
		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 1, 68, 202, 1, false );
		img ->SetClientImage( TID_CTRL_LINEDOT );
		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 1, 148, 202, 1, false );
		img ->SetClientImage( TID_CTRL_LINEDOT );
		
		///-- Line
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 1, 179, 202, 79, false );
		img->SetClientImage( TID_CTRL_BACK );
		img->SetFocusCheck( false );
		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 1, 178, 202, 1, false );
		img ->SetClientImage( TID_CTRL_LINEDOT );
		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 1, 258, 202, 1, false );
		img ->SetClientImage( TID_CTRL_LINEDOT );
		
		///-- Slot
		///-- 내 물품 (Static)
		tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Exchange_Wnd, 8, 46, 90, 17, false );
		tbox->SetFontR( n_fRsvFontWndStatic );
		tbox->SetText( G_STRING_CHAR( IDS_WND_EX_MY_ITEMS )); 
		
		///--  상대 물품(Static)
		tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_Exchange_Wnd, 8, 156, 90, 17, false );
		tbox->SetFontR( n_fRsvFontWndStatic );
		tbox->SetText( G_STRING_CHAR( IDS_WND_EX_OB_ITEMS ) ); 
		
		///-- Eld 입력바 내꺼
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 111, 43, 90, 23, false );      
		img ->SetClientImage( TID_CTRL_IMG_ELDTEXT90 );
		
		///-- Eld 입력바 down
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 111, 153, 90, 23, false );      
		img ->SetClientImage( TID_CTRL_IMG_ELDTEXT90 );
		
		///-- 상대 Eld 버튼( static )
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 88, 153, 23, 23, false );
		img ->SetClientImage( TID_CTRL_BTN_ELD );
		
		///-- 내 Slot Static
		for( i = 0 ; i < 2; ++i )
		{			
			for( j = 0 ; j < 5 ; ++j )
			{
				img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 6 + j*39, 71 + i*39, 35, 35, false );      
				img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );				
			}
		}
		
		///-- 상대 Slot Static
		for( i = 0 ; i < 2; ++i )
		{			
			for( j = 0 ; j < 5 ; ++j )
			{
				img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_Exchange_Wnd, 6 + j*39, 181 + i*39, 35, 35, false );      
				img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );				
			}
		}
		
		///---------------------------------------------------------------------------	
		///-- Control
		///---------------------------------------------------------------------------	
		///-- X	
		btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Exchange_CloseBtn, WID_Exchange_Wnd, 185, 6, 13, 13, false ); 
		btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
		
		///-- Elda button
		btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Exchange_MineEldBtn, WID_Exchange_Wnd, 88, 43, 23, 23, false );
		btn->SetBtnImage( TID_CTRL_BTN_ELD, TID_CTRL_BTN_ELD_OVR, TID_CTRL_BTN_ELD_CLK, TID_None );
		btn->SetHID( HID_None );
		
		
		///-- 올리기 버튼
		m_UploadBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Exchange_UploadBtn, WID_Exchange_Wnd, 4, 262, 72, 25, false );  
		m_UploadBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );		
		m_UploadBtn->SetFontR( n_fRsvFontWndButton ); 
		m_UploadBtn->SetText(G_STRING_CHAR( IDS_WND_BT_CONFIRM_EX )); 
		m_UploadBtn->SetHID( HID_None );
		
		///-- 승락
		m_OKBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Exchange_OKBtn, WID_Exchange_Wnd, 83, 262, 58, 25, false );   
		m_OKBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
		m_OKBtn->SetFontR( n_fRsvFontWndButton ); 
		m_OKBtn->SetText(G_STRING_CHAR( IDS_WND_COM_ACCEPT )); 		
		m_OKBtn->SetHID( HID_None );
		m_OKBtn->SetBtnHold( TRUE );		
		
		///-- 취소		
		m_CancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Exchange_CancelBtn, WID_Exchange_Wnd, 142, 262, 58, 25, false );   
		m_CancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
		m_CancelBtn->SetFontR( n_fRsvFontWndButton ); 
		m_CancelBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 		
		m_CancelBtn->SetHID( HID_None );
		
		
		///-- 내 엘드 텍스트
		m_MineEldText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Exchange_MineEldText, WID_Exchange_Wnd, 121, 45, 88, 21, false );
		m_MineEldText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_MineEldText->SetClientColor( 180, 175, 160 );
		m_MineEldText->SetClientImage( TID_None );
		m_MineEldText->SetAlignText( n_atLeft, n_atCenter );
		m_MineEldText->SetText( _RT("0") );		
		m_MineEldText->SetTextColor( 240, 240, 200 );
		m_MineEldText->SetBlank( 0, 0 );
		
		//---------------------------------------------------------------------
		m_MineItemSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_MineItemSlot0, WID_Exchange_Wnd, 6, 71, 35, 35, false );
		m_MineItemSlot[0]->SetSlotGroup( n_SGROUP_EXCHANGE );
		m_MineItemSlot[0]->SetSlotNum( 0 );
		m_MineItemSlot[0]->SetHID( HID_ITEMTIP );
		m_MineItemSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_MineItemSlot1, WID_Exchange_Wnd, 45, 71, 35, 35, false );
		m_MineItemSlot[1]->SetSlotGroup( n_SGROUP_EXCHANGE );
		m_MineItemSlot[1]->SetSlotNum( 1 );
		m_MineItemSlot[1]->SetHID( HID_ITEMTIP );
		m_MineItemSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_MineItemSlot2, WID_Exchange_Wnd, 84, 71, 35, 35, false );
		m_MineItemSlot[2]->SetSlotGroup( n_SGROUP_EXCHANGE );
		m_MineItemSlot[2]->SetSlotNum( 2 );
		m_MineItemSlot[2]->SetHID( HID_ITEMTIP );
		m_MineItemSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_MineItemSlot3, WID_Exchange_Wnd, 123, 71, 35, 35, false );
		m_MineItemSlot[3]->SetSlotGroup( n_SGROUP_EXCHANGE );
		m_MineItemSlot[3]->SetSlotNum( 3 );
		m_MineItemSlot[3]->SetHID( HID_ITEMTIP );
		m_MineItemSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_MineItemSlot4, WID_Exchange_Wnd, 162, 71, 35, 35, false );
		m_MineItemSlot[4]->SetSlotGroup( n_SGROUP_EXCHANGE );
		m_MineItemSlot[4]->SetSlotNum( 4 );
		m_MineItemSlot[4]->SetHID( HID_ITEMTIP );
		m_MineItemSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_MineItemSlot5, WID_Exchange_Wnd, 6, 110, 35, 35, false );
		m_MineItemSlot[5]->SetSlotGroup( n_SGROUP_EXCHANGE );
		m_MineItemSlot[5]->SetSlotNum( 5 );
		m_MineItemSlot[5]->SetHID( HID_ITEMTIP );
		m_MineItemSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_MineItemSlot6, WID_Exchange_Wnd, 45, 110, 35, 35, false );
		m_MineItemSlot[6]->SetSlotGroup( n_SGROUP_EXCHANGE );
		m_MineItemSlot[6]->SetSlotNum( 6 );
		m_MineItemSlot[6]->SetHID( HID_ITEMTIP );
		m_MineItemSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_MineItemSlot7, WID_Exchange_Wnd, 84, 110, 35, 35, false );
		m_MineItemSlot[7]->SetSlotGroup( n_SGROUP_EXCHANGE );
		m_MineItemSlot[7]->SetSlotNum( 7 );
		m_MineItemSlot[7]->SetHID( HID_ITEMTIP );
		m_MineItemSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_MineItemSlot8, WID_Exchange_Wnd, 123, 110, 35, 35, false );
		m_MineItemSlot[8]->SetSlotGroup( n_SGROUP_EXCHANGE );
		m_MineItemSlot[8]->SetSlotNum( 8 );
		m_MineItemSlot[8]->SetHID( HID_ITEMTIP );
		m_MineItemSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_MineItemSlot9, WID_Exchange_Wnd, 162, 110, 35, 35, false );
		m_MineItemSlot[9]->SetSlotGroup( n_SGROUP_EXCHANGE );
		m_MineItemSlot[9]->SetSlotNum( 9 );
		m_MineItemSlot[9]->SetHID( HID_ITEMTIP );
		
		
		m_YoursEldText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Exchange_YoursEldText, WID_Exchange_Wnd, 121, 155, 88, 21, false );
		m_YoursEldText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_YoursEldText->SetClientColor( 180, 175, 160 );
		m_YoursEldText->SetClientImage( TID_None );
		m_YoursEldText->SetAlignText( n_atLeft, n_atCenter );
		m_YoursEldText->SetText( _RT("0") );		
		m_YoursEldText->SetTextColor( 240, 240, 200 );
		m_YoursEldText->SetBlank( 0, 0 );
		
		//---------------------------------------------------------------------
		m_YoursItemSlot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_YoursItemSlot0, WID_Exchange_Wnd, 6, 181, 35, 35, false );
		m_YoursItemSlot[0]->SetSlotGroup( n_SGROUP_NONE );
		m_YoursItemSlot[0]->SetSlotNum( 0 );
		m_YoursItemSlot[0]->SetHID( HID_ITEMTIP );
		m_YoursItemSlot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_YoursItemSlot1, WID_Exchange_Wnd, 45, 181, 35, 35, false );
		m_YoursItemSlot[1]->SetSlotGroup( n_SGROUP_NONE );
		m_YoursItemSlot[1]->SetSlotNum( 1 );
		m_YoursItemSlot[1]->SetHID( HID_ITEMTIP );
		m_YoursItemSlot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_YoursItemSlot2, WID_Exchange_Wnd, 84, 181, 35, 35, false );
		m_YoursItemSlot[2]->SetSlotGroup( n_SGROUP_NONE );
		m_YoursItemSlot[2]->SetSlotNum( 2 );
		m_YoursItemSlot[2]->SetHID( HID_ITEMTIP );
		m_YoursItemSlot[3] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_YoursItemSlot3, WID_Exchange_Wnd, 123, 181, 35, 35, false );
		m_YoursItemSlot[3]->SetSlotGroup( n_SGROUP_NONE );
		m_YoursItemSlot[3]->SetSlotNum( 3 );
		m_YoursItemSlot[3]->SetHID( HID_ITEMTIP );
		m_YoursItemSlot[4] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_YoursItemSlot4, WID_Exchange_Wnd, 162, 181, 35, 35, false );
		m_YoursItemSlot[4]->SetSlotGroup( n_SGROUP_NONE );
		m_YoursItemSlot[4]->SetSlotNum( 4 );
		m_YoursItemSlot[4]->SetHID( HID_ITEMTIP );
		m_YoursItemSlot[5] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_YoursItemSlot5, WID_Exchange_Wnd, 6, 220, 35, 35, false );
		m_YoursItemSlot[5]->SetSlotGroup( n_SGROUP_NONE );
		m_YoursItemSlot[5]->SetSlotNum( 5 );
		m_YoursItemSlot[5]->SetHID( HID_ITEMTIP );
		m_YoursItemSlot[6] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_YoursItemSlot6, WID_Exchange_Wnd, 45, 220, 35, 35, false );
		m_YoursItemSlot[6]->SetSlotGroup( n_SGROUP_NONE );
		m_YoursItemSlot[6]->SetSlotNum( 6 );
		m_YoursItemSlot[6]->SetHID( HID_ITEMTIP );
		m_YoursItemSlot[7] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_YoursItemSlot7, WID_Exchange_Wnd, 84, 220, 35, 35, false );
		m_YoursItemSlot[7]->SetSlotGroup( n_SGROUP_NONE );
		m_YoursItemSlot[7]->SetSlotNum( 7 );
		m_YoursItemSlot[7]->SetHID( HID_ITEMTIP );
		m_YoursItemSlot[8] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_YoursItemSlot8, WID_Exchange_Wnd, 123, 220, 35, 35, false );
		m_YoursItemSlot[8]->SetSlotGroup( n_SGROUP_NONE );
		m_YoursItemSlot[8]->SetSlotNum( 8 );
		m_YoursItemSlot[8]->SetHID( HID_ITEMTIP );
		m_YoursItemSlot[9] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_Exchange_YoursItemSlot9, WID_Exchange_Wnd, 162, 220, 35, 35, false );
		m_YoursItemSlot[9]->SetSlotGroup( n_SGROUP_NONE );
		m_YoursItemSlot[9]->SetSlotNum( 9 );
		m_YoursItemSlot[9]->SetHID( HID_ITEMTIP );
		
		
		
		for( int i = 0; i < c_MaxExchangeNum; ++i )
		{
			m_MineItemSlot[i]->PcItem = &m_MinePcItem[i];
			m_YoursItemSlot[i]->PcItem = &m_YoursPcItem[i];
		}
	}
}

//-----------------------------------------------------------------------------
void CExchangeWnd::Update()
{ 
	if( IsVisible )
	{
		if( m_ExchangePcIndex )
		{
			// 교환 상대와의 거리를 측정해서 멀어지면 창을 닫고 교환 취소 한다.
			int nPosX = 0;
			int nPosY = 0;
			float distance = 0.0f;
			
			nPosX	= g_Pc.m_PcTable[m_ExchangePcIndex].lDestBlock%g_Pc.m_cSize;
			nPosY	= g_Pc.m_PcTable[m_ExchangePcIndex].lDestBlock/g_Pc.m_cSize;
			
			float sx = g_Pc.GetPlayer()->GetPosTM()._41;
			float sz = g_Pc.GetPlayer()->GetPosTM()._43;
			float dx = g_Pc.m_Pc[m_ExchangePcIndex].GetPosTM()._41;
			float dz = g_Pc.m_Pc[m_ExchangePcIndex].GetPosTM()._43;
			
			distance = DIST( sx, sz, dx, dz );
			
			float range = 240.0f;
			
			if( distance > range )
			{
				g_RockClient.Send_RpCsCancelExchange();			
				
				CloseExchangeWnd();	
			}
		}
	}
	
	return;
}

//-----------------------------------------------------------------------------
void CExchangeWnd::Proc( SEventMessage* EMsg )
{
	int i = 0;
	int	ItemNum = 0;
	
	_SExItem ItemList[c_MaxExchangeNum];	
	
	bool IsWnd = false;
	
	
	switch( EMsg->FocusWnd )
	{	
	case WID_Exchange_MineEldBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			if( m_bIsExchangeLock == FALSE )
			{
				g_MoneyThrowWnd.Open( n_ELDINPUT_EXCHANGE_DRAWOUT );
			}
		}
		break;
	case WID_Exchange_MineItemSlot0:
	case WID_Exchange_MineItemSlot1:
	case WID_Exchange_MineItemSlot2:
	case WID_Exchange_MineItemSlot3:
	case WID_Exchange_MineItemSlot4:
	case WID_Exchange_MineItemSlot5:
	case WID_Exchange_MineItemSlot6:
	case WID_Exchange_MineItemSlot7:
	case WID_Exchange_MineItemSlot8:
	case WID_Exchange_MineItemSlot9:
		{
			if( m_bIsExchangeLock == FALSE )
			{
				if( EMsg->Notify == NM_SLOT_FOCUS )			{	ExchangeSlotFocus( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_PRESS )	{	ExchangeSlotFocus( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_CLICK )	{	ExchangeSlotClick( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DBCLICK )	{	ExchangeSlotDBClick( EMsg );	}
				else if( EMsg->Notify == NM_SLOT_DRAG )		{	ExchangeSlotDrag( EMsg );		}
				else if( EMsg->Notify == NM_SLOT_DROP )		{	ExchangeSlotDrop( EMsg );		}
			}
		}
		break;
		
	case WID_Exchange_UploadBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			nRui->ResetCursorItem();
			
			if( CheckExChangeList() == FALSE && CheckExChangeEld() == FALSE )
			{			
				for( i = 0; i < c_MaxExchangeNum; ++i )
				{
					m_MineItemSlot[i]->theSlotState = n_Slot_Disable;		
					
					if( m_MineItemSlot[i]->PcItem->IType != n_ITYPE_NONE )
					{
						++ItemNum;
						ItemList[i].theItemUID = m_MineItemSlot[i]->PcItem->ItemUID;
						ItemList[i].theBundle = m_MineItemSlot[i]->PcItem->Amount;
					}
					else
					{
						ItemList[i].theItemUID = 0;
						ItemList[i].theBundle = 0;
					}
				}
				
				if( ItemNum >= 0 || m_MineEld >= 0 )
				{
					g_RockClient.Send_RpCsSuggestExItem( m_MineEld, ItemNum, ItemList );
					
					m_bIsExchangeLock = TRUE;
					m_UploadBtn->SetBtnHold( TRUE );
					m_OKBtn->SetBtnHold( FALSE );	
					
					ItemNum = 0;
					
					// 올리기를 한 후에는 인벤 슬롯, 퀵슬롯 모두 n_Slot_Disable 시켜놓는다
					// InventoryWnd 
					// MainMenuWnd 
				}
			}
		}
		break;
	case WID_Exchange_OKBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{	
			nRui->ResetCursorItem();
			
			if( m_bIsYoursUpload && ( m_UploadBtn->GetBtnState() == n_bsHold ) )
			{
				g_RockClient.Send_RpCsAgreeExchange();
				
				m_OKBtn->SetBtnHold( TRUE );					
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EX_CHECK_APPLY ) );
			}
		}
		break;
	case WID_Exchange_CancelBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{		
			nRui->ResetCursorItem();
			
			g_RockClient.Send_RpCsCancelExchange();
			
			CloseExchangeWnd();				
		}
		break;
	case WID_Exchange_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			nRui->ResetCursorItem();
			
			g_RockClient.Send_RpCsCancelExchange();			
			
			CloseExchangeWnd();
		}
		break;
	}
	return;
	
}



void CExchangeWnd::ExchangeSlotFocus( SEventMessage* EMsg )
{
	static CSlot* tslot = NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
	if( ( tslot->PcItem != NULL ) && ( nRui->GetCursorItem() == NULL ) )
	{
		if( tslot->PcItem->IType != n_ITYPE_NONE )
		{
			nRui->theCursorState = n_CT_HAND;
		}
	}
	
	return;
}

//-----------------------------------------------------------------------------
void CExchangeWnd::ExchangeSlotClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	// 클릭된 교환창 슬롯
	CSlot*		sslot	= NULL;	// 선택된(pick) 아이템 슬롯
	
	if( nRui->GetCursorItem() != NULL )// 마우스에 등록아이콘
	{
		tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		sslot = nRui->GetItemSelectedSlot();
		if( sslot->PcItem == NULL )
		{
			return;
		}
		
		if( tslot->PcItem && (tslot->PcItem == sslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
		else
		{				
			if( sslot->Group )
			{
				switch( sslot->Group )
				{
				case n_SGROUP_WEAR:
					{

#ifdef ITEM_LIMIT_FUNC
						BYTE aItemLimitFunc = 0;

						if( sslot->PcItem->ItemTable )
						{
							aItemLimitFunc = sslot->PcItem->ItemTable->theItemLimitFunc;
							
							///-- 가공 장신구는 교환 할수 없다.
							BYTE aClass = sslot->PcItem->ItemTable->theClass;
							if( n_Class_Accessory == aClass )
							{
								if( sslot->PcItem->WearTable.theEldaStone )
								{
									SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
									nRui->ResetCursorItem();								
									return;
								}
							}
						}

					#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625
						
						if(sslot->PcItem->IsItemCode60000Above())
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();
							return;
						}
						else
						{
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
								nRui->ResetCursorItem();				
								return;
							}
						}

					#else

						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}
						
					#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

					#ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

						if(sslot->PcItem->IsItemCode12180_12188_Above())
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();
							return;
						}
						else
						{
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
								nRui->ResetCursorItem();				
								return;
							}
						}
						
					#else
						
						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}

					#endif// #ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

#endif
						
						///-- 유료 아이템이라면
						WORD wCode = sslot->PcItem->ItemTable->theCode;
						if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max) )
						{

#ifdef C_DROP_WASTEPAYITEM

							if(sslot->PcItem->IType != n_ITYPE_USE )
							{
							#ifdef HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707
								
								if(sslot->PcItem->Class != n_Class_Use &&
									sslot->PcItem->Class != n_Class_FrontierPet &&
									sslot->PcItem->Class != n_Class_PersonalPet)
								{
									SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
									nRui->ResetCursorItem();						
									return;						
								}
								
							#else
								
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
								nRui->ResetCursorItem();						
								return;						
								
							#endif// #ifdef HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707
							}
#else

							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
							nRui->ResetCursorItem();						
							return;						

#endif
						}						
						
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EXCHANGE_WEAR ) );
						nRui->ResetCursorItem();
						
					}
					break;
				case n_SGROUP_INVENTORY:
					{							
#ifdef ITEM_LIMIT_FUNC
						BYTE aItemLimitFunc = 0;

						if( sslot->PcItem->ItemTable )
						{
							aItemLimitFunc = sslot->PcItem->ItemTable->theItemLimitFunc;

							///-- 가공 장신구는 교환 할수 없다.
							BYTE aClass = sslot->PcItem->ItemTable->theClass;
							if( n_Class_Accessory == aClass )
							{
								if( sslot->PcItem->WearTable.theEldaStone )
								{
									SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
									nRui->ResetCursorItem();									
									return;
								}
							}
						}

					#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625
						
						if(sslot->PcItem->IsItemCode60000Above())
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();
							return;
						}
						else
						{
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
								nRui->ResetCursorItem();				
								return;
							}
						}

					#else

						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}
						
					#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

					#ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

						if(sslot->PcItem->IsItemCode12180_12188_Above())
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();
							return;
						}
						else
						{
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
								nRui->ResetCursorItem();				
								return;
							}
						}
						
					#else
						
						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}

					#endif// #ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

#endif					
						///-- 유료 아이템이라면
						WORD wCode = sslot->PcItem->ItemTable->theCode;
						if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max) )
						{
#ifdef C_DROP_WASTEPAYITEM

							if(sslot->PcItem->IType != n_ITYPE_USE )
							{
							#ifdef HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707
								
								if(sslot->PcItem->Class != n_Class_Use &&
									sslot->PcItem->Class != n_Class_FrontierPet &&
									sslot->PcItem->Class != n_Class_PersonalPet)
								{
									SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
									nRui->ResetCursorItem();						
									return;						
								}
								
							#else
								
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
								nRui->ResetCursorItem();						
								return;						
								
							#endif// #ifdef HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707
							}
#else
							
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
							nRui->ResetCursorItem();
							return;

#endif
						}						
						
						if( CheckSameMineItemSlot( sslot->PcItem ) == FALSE )
						{							
							if( sslot->PcItem->ItemTable->theBundle != 1 && sslot->PcItem->Amount > 1 ) 								
							{
								g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_EXCHANGE, sslot->PcItem );
								m_SrcSlot = sslot->SlotAddr;			
								m_TargetSlot = tslot;
								nRui->ResetCursorItem();
							}
							else
							{						
								tslot->SetSlotPcItem( *(sslot->PcItem) );				
								nRui->ResetCursorItem();						
							}
						}
						else
						{
							nRui->ResetCursorItem();
						}
					}
					break;
				case n_SGROUP_EXCHANGE:
					{
						if( tslot->PcItem->IType == n_ITYPE_NONE )
						{	
							tslot->SetSlotPcItem( *(sslot->PcItem) );				
							ResetCursorPcItem();						
							sslot->SlotAddr->PcItem->Clear();	
							nRui->thePcItem = NULL;
							nRui->SetItemSelectedSlot( NULL );
						}
						else
						{
							SPcItem		pSrctmpItem;
							pSrctmpItem = *(sslot->PcItem);
							SPcItem		pTargettmpItem;
							pTargettmpItem = *(tslot->PcItem);
							
							tslot->SetSlotPcItem( pSrctmpItem );				
							ResetCursorPcItem();			
							sslot->SlotAddr->SetSlotPcItem( pTargettmpItem );							
							nRui->thePcItem = NULL;
							nRui->SetItemSelectedSlot( NULL );
						}
					}
					break;
				default:
					{
						nRui->ResetCursorItem();
					}
					break;
				}
			}			
		}
	}
	else
	{		
		//커서에 아이템 붙이기 메세지.
		tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		
		if( tslot->PcItem->IType != n_ITYPE_NONE )
		{
			nRui->SetCursorItem( tslot->PcItem );
			nRui->SetItemSelectedSlot( tslot );
			
			nRui->theCursorState = n_CT_ITEMDROP;	
		}
	}
	
	return;
}

//-----------------------------------------------------------------------------
void CExchangeWnd::ExchangeSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();
	
	
	if( tslot->PcItem->IType != n_ITYPE_NONE )
	{
		if( sslot->PcItem )
		{
			if( tslot->PcItem->ItemUID == sslot->PcItem->ItemUID )
			{
				nRui->ResetCursorItem();
			}
		}
		
		tslot->PcItem->Clear();
	}
	
	return;
}

//-----------------------------------------------------------------------------
void CExchangeWnd::ExchangeSlotDrag( SEventMessage* EMsg )
{
	int idx = 0;
	CSlot* slot = (CSlot*)Container->Get( EMsg->FocusWnd );
	
	if( ( slot->PcItem != NULL ) && ( EMsg->FocusWnd != (WNDID)EMsg->fParam ) && ( slot->PcItem->IType != n_ITYPE_NONE ) )
	{
		if( slot->PcItem->IType != n_ITYPE_NONE )
		{
			nRui->theCursorState = n_CT_ITEMDROP;
			
			//커서에 아이템 붙이기
			if( nRui->GetCursorItem() == NULL )
			{
				nRui->SetCursorItem( slot->PcItem );
				nRui->SetItemSelectedSlot( slot );
			}
		}
	}
	
	return;
}

//-----------------------------------------------------------------------------
void CExchangeWnd::ExchangeSlotDrop( SEventMessage* EMsg )
{
	CSlot* sslot = (CSlot*)Container->Get( EMsg->FocusWnd );		// 들고있는 Src Slot
	CSlot* tslot = (CSlot*)Container->Get( (WNDID)EMsg->fParam );	// 내려놓는 Target Slot
	
	if( ((WNDID)EMsg->fParam != WID_None ) && ( tslot->GetWindowType() == n_wtSlot ) )
	{
		if( sslot == tslot )
		{
			nRui->ResetCursorItem();
		}
		else if( ( sslot->PcItem != NULL ) && ( sslot->PcItem->IType != n_ITYPE_NONE ) ) 
		{
			if( sslot->Group )
			{
				switch( sslot->Group )
				{	
				case n_SGROUP_WEAR:
					{
											
#ifdef ITEM_LIMIT_FUNC
						BYTE aItemLimitFunc = 0;

						if( sslot->PcItem->ItemTable )
						{
							aItemLimitFunc = sslot->PcItem->ItemTable->theItemLimitFunc;

							///-- 가공 장신구는 교환 할수 없다.
							BYTE aClass = sslot->PcItem->ItemTable->theClass;
							if( n_Class_Accessory == aClass )
							{
								if( sslot->PcItem->WearTable.theEldaStone )
								{
									SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
									nRui->ResetCursorItem();									
									return;
								}
							}
						}

					#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625
						
						if(sslot->PcItem->IsItemCode60000Above())
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();
							return;
						}
						else
						{
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
								nRui->ResetCursorItem();				
								return;
							}
						}

					#else

						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}
						
					#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

					#ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

						if(sslot->PcItem->IsItemCode12180_12188_Above())
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();
							return;
						}
						else
						{
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
								nRui->ResetCursorItem();				
								return;
							}
						}
						
					#else
						
						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}

					#endif// #ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824



#endif					
						
						///-- 유료 아이템이라면
						WORD wCode = sslot->PcItem->ItemTable->theCode;
						if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max) )
						{

#ifdef  C_DROP_WASTEPAYITEM
							if(sslot->PcItem->IType != n_ITYPE_USE )
							{
							#ifdef HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707
								
								if(sslot->PcItem->Class != n_Class_Use &&
									sslot->PcItem->Class != n_Class_FrontierPet &&
									sslot->PcItem->Class != n_Class_PersonalPet)
								{
									SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
									nRui->ResetCursorItem();						
									return;						
								}
								
							#else
								
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
								nRui->ResetCursorItem();						
								return;						
								
							#endif// #ifdef HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707
							}
#else
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
							nRui->ResetCursorItem();						
							return;	
#endif
							
							
						}
						
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_EXCHANGE_WEAR ) );
						nRui->ResetCursorItem();
					}
					break;
				case n_SGROUP_INVENTORY:
					{							
						
#ifdef ITEM_LIMIT_FUNC
						BYTE aItemLimitFunc = 0;

						if( sslot->PcItem->ItemTable )
						{
							aItemLimitFunc = sslot->PcItem->ItemTable->theItemLimitFunc;

							///-- 가공 장신구는 교환 할수 없다.
							BYTE aClass = sslot->PcItem->ItemTable->theClass;
							if( n_Class_Accessory == aClass )
							{
								if( sslot->PcItem->WearTable.theEldaStone )
								{
									SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
									nRui->ResetCursorItem();									
									return;
								}
							}
						}						

					#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625
						
						if(sslot->PcItem->IsItemCode60000Above())
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );	
							nRui->ResetCursorItem();
							return;
						}
						else
						{
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
								nRui->ResetCursorItem();				
								return;
							}
						}

					#else

						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}
						
					#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

					#ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

						if(sslot->PcItem->IsItemCode12180_12188_Above())
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );	
							nRui->ResetCursorItem();
							return;
						}
						else
						{
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
							{
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
								nRui->ResetCursorItem();				
								return;
							}
						}
						
#else
						
						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotExchange ) )
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_EXCHANGEITEM ) );					
							nRui->ResetCursorItem();				
							return;
						}

					#endif// #ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

#endif				
						///-- 유료 아이템이라면
						WORD wCode = sslot->PcItem->ItemTable->theCode;
						if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max) )
						{
#ifdef  C_DROP_WASTEPAYITEM							
							if(sslot->PcItem->IType != n_ITYPE_USE )
							{
							#ifdef HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707
								
								if(sslot->PcItem->Class != n_Class_Use &&
									sslot->PcItem->Class != n_Class_FrontierPet &&
									sslot->PcItem->Class != n_Class_PersonalPet)
								{
									SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
									nRui->ResetCursorItem();						
									return;						
								}
								
							#else	
								
								SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
								nRui->ResetCursorItem();						
								return;						
								
							#endif// #ifdef HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707
							}
#else
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
							nRui->ResetCursorItem();
							return;
#endif
							
							
						}					
						
						if( CheckSameMineItemSlot( sslot->PcItem ) == FALSE )
						{
							if( sslot->PcItem->ItemTable->theBundle != 1 && sslot->PcItem->Amount > 1 ) 
							{
								g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_EXCHANGE, sslot->PcItem );								
								m_SrcSlot = sslot->SlotAddr;			
								m_TargetSlot = tslot;
								nRui->ResetCursorItem();
							}
							else
							{							
								tslot->SetSlotPcItem( *(sslot->PcItem) );				
								nRui->ResetCursorItem();						
							}
						}
						else
						{
							nRui->ResetCursorItem();
						}
					}
					break;
				case n_SGROUP_EXCHANGE:
					{
						if( tslot->Group == n_SGROUP_EXCHANGE )
						{
							if( tslot->PcItem->IType == n_ITYPE_NONE )
							{	
								tslot->SetSlotPcItem( *(sslot->PcItem) );											
								ResetCursorPcItem();							
								sslot->PcItem->Clear();		
								nRui->thePcItem = NULL;
								nRui->SetItemSelectedSlot( NULL );
							}
							else
							{
								SPcItem		pSrctmpItem;
								pSrctmpItem = *(sslot->PcItem);
								SPcItem		pTargettmpItem;
								pTargettmpItem = *(tslot->PcItem);
								
								tslot->SetSlotPcItem( pSrctmpItem );				
								ResetCursorPcItem();			
								sslot->SetSlotPcItem( pTargettmpItem );							
								nRui->thePcItem = NULL;
								nRui->SetItemSelectedSlot( NULL );								
							}
						}
					}
					break;
				default:
					{
						nRui->ResetCursorItem();
					}
					break;
				}
			}
		}
	}
	return;
}

BOOL CExchangeWnd::CheckSameMineItemSlot( SPcItem* pPcItem )
{
	for( int i = 0; i < c_MaxExchangeNum; ++i )
	{
		if( m_MineItemSlot[i]->PcItem->ItemUID == pPcItem->ItemUID )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

void CExchangeWnd::CloseNumberInputWnd( int aNumItem )
{
	if( !m_SrcSlot )
	{
		return;
	}
	
	if( m_SrcSlot->PcItem )
	{
		if( m_SrcSlot->PcItem->Amount >= aNumItem )
		{
			m_TargetSlot->SetSlotPcItem( *(m_SrcSlot->PcItem) );				
			m_TargetSlot->PcItem->Amount = aNumItem;
		}
		else
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_VOLUME ) );
		}
	}
	
	return;
}

void CExchangeWnd::Open()
{ 
	// 교환 수락시 다른 창들을 모두 닫는다. ( NPC 대화창, 보관창 )
	if( NpcChatWnd.IsVisible )
	{
		NpcChatWnd.CloseNpcChatWnd(); 
		
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_NPCCHAT_ON_EX ) );		
	}
	else if( g_WarehouseWnd.IsVisible )
	{
		nRui->SendEvent( WID_Warehouse_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_WAREHOUSE_ON_EX ) );
	}
#ifdef DECO_RENEWAL_GUILD_MJH
	else if( g_GuildInvenWnd.IsVisible )
	{
		nRui->SendEvent( WID_GuildInven_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_WAREHOUSE_ON_EX ) );
	}
#endif // DECO_RENEWAL_GUILD_MJH

	
	nRui->SendEvent( WID_Exchange_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );													
	
	m_MineEld = 0;
	m_YoursEld = 0;
	m_MineEldText->SetText( _RT("0") );	
	m_YoursEldText->SetText( _RT("0") );	
	
	return;
}

void CExchangeWnd::LogOff()
{ 
	if( m_bIsAccept )
	{
		g_RockClient.Send_RpCsCancelExchange();			
		CloseExchangeWnd();			
	}
	return;
}

void CExchangeWnd::ResetCursorPcItem()
{
	InventoryWnd.ResetWearSlotHighlight();									// 장비 슬롯의 하이라이트를 끈다....
	
	if( nRui->thePcItem != NULL )											// NULL이 들어오는건 아이템이 떨어지는것으로 간주한다...
	{
		if( nRui->thePcItem->IType != n_ITYPE_NONE )
		{
			if( nRui->thePcItem->IType == n_ITYPE_SKILL )
			{
				g_Sound.Play( 28 );											// 스킬 놓기 사운드
			}
			else
			{
				g_Sound.Play( nRui->thePcItem->ItemTable->theSoundFile );	// 떨어뜨릴때는 아이템 고유 사운드 출력
			}		
		}
	}
	return;
}

BOOL CExchangeWnd::CheckExChangeList()
{
	int i = 0;
	int j = 0;
	
	bool isFind = FALSE;
	
	for( i = 0; i < c_MaxExchangeNum; ++i )
	{
		if( m_MineItemSlot[i]->PcItem->IType == n_ITYPE_NONE )
		{
			continue;
		}
		
		isFind = FALSE;		
		
		for( j = 0; j < c_MaxInvenItemNum; ++j )
		{	
			if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
			{
				continue;
			}
			
			if( m_MineItemSlot[i]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
			{
				isFind = TRUE;
				break;
			}
		}
		
		if( isFind )
		{
			if( m_MineItemSlot[i]->PcItem->IType == n_ITYPE_USE )
			{
				// 실제 가지고 있는것 보다 많이 교환하려구 하면...교환창에 넣고 아이템 떨군게야...ㅡㅡ; 사기꾼!!! 
				if( m_MineItemSlot[i]->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
				{
					m_MineItemSlot[i]->PcItem->Clear();			
					return TRUE;
				}
			}			
		}
		else
		{
			m_MineItemSlot[i]->PcItem->Clear();					// 못찾으면 변경된것이니 지워라...
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CExchangeWnd::CheckExChangeEld()
{
	if( m_MineEld > nRui->GetPcParamInfo()->Money )
	{
		m_MineEld = 0;
		m_MineEldText->SetText( _RT("0") );
		return TRUE;
	}
	
	return FALSE;
}


void CExchangeWnd::CloseExchangeWnd()
{
	nRui->SendEvent( WID_Exchange_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장
	nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );//유료인벤확장
#endif
	
	g_MoneyThrowWnd.CancelClose();

	m_UploadBtn->SetBtnHold( FALSE );
	m_OKBtn->SetBtnHold( TRUE );				
	m_CancelBtn->SetBtnHold( FALSE );				
	
	m_bIsExchangeLock = FALSE;
	m_bIsAccept = FALSE;
	m_bIsYoursUpload = FALSE;
	
	for( int i = 0; i < c_MaxExchangeNum; ++i )
	{
		m_MineItemSlot[i]->theSlotState = n_Slot_Enable;		
		m_MineItemSlot[i]->PcItem->Clear();	
		m_YoursItemSlot[i]->PcItem->Clear();	
	}
	
	m_MineEld = 0;
	m_MineEldText->SetText( _RT("0") );
	
	m_YoursEld = 0;
	m_YoursEldText->SetText( _RT("0") );
	
	m_ExchangePcIndex = 0;
	
	return;
}


void CExchangeWnd::ResetMineItemSlot()
{
	for( int i = 0; i < c_MaxExchangeNum; ++i )
	{
		m_MineItemSlot[i]->theSlotState = n_Slot_Enable;		
		m_MineItemSlot[i]->PcItem->Clear();		
	}
	
	m_UploadBtn->SetBtnHold( FALSE );
	m_OKBtn->SetBtnHold( TRUE );				
	m_CancelBtn->SetBtnHold( FALSE );				
	
	m_bIsExchangeLock = FALSE;
	
	return;
}

void CExchangeWnd::CheckWndUpdate()
{
	if( m_bIsAccept )
	{
		g_RockClient.Send_RpCsCancelExchange();			
		CloseExchangeWnd();			
	}
	
	return;
}

void CExchangeWnd::SetYoursItemSlot( DWORD aEld, BYTE aItemNum, URpdItem aItemList[] )
{
	m_YoursEld = aEld;
	
	char EldStr[256];
	///itoa( aEld, EldStr, 10 );
	InventoryWnd.SetEldText( EldStr, (DWORD)aEld );
	m_YoursEldText->SetText( EldStr );
	
	for( int i = 0; i < aItemNum; ++i )
	{
		SPcItem pc_item;
		
		switch( aItemList[i].theItem.theType )
		{
		case n_Weapon:
		case n_Armor:
		case n_Accessory:
			{
				pc_item.IType		= n_ITYPE_WEAR;				
				pc_item.Class		= g_Pc_Manager.GetItemClsss( aItemList[i].theItem.theCode );
				pc_item.ItemUID		= aItemList[i].theItem.theItemUID;
				pc_item.Code		= aItemList[i].theItem.theCode;
				pc_item.Tid			= (TEXID)g_Pc_Manager.GetItemTID( aItemList[i].theItem.theCode );
				pc_item.Amount		= 1;
				pc_item.ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aItemList[i].theItem.theCode );
				memcpy( &(pc_item.WearTable), &(aItemList[i].theWearItem), sizeof( URpdItem ) );
				
				// 왼손,오른손....
				//---------------------------------------------------------------------
				if( ( pc_item.ItemTable->theType == n_Type_MagicBook ) || ( pc_item.ItemTable->theType == n_Type_Bow ) || ( pc_item.ItemTable->theType == n_Type_Shield ) )
				{
					pc_item.Class = n_Class_Shield;
				}
				else if( ( pc_item.ItemTable->theType == n_Type_Sword ) || ( pc_item.ItemTable->theType == n_Type_TwohandSword ) ||
					( pc_item.ItemTable->theType == n_Type_Staff ) || ( pc_item.ItemTable->theType == n_Type_CrystalBall ) )
				{
					pc_item.Class = n_Class_Weapon;
				}
				//---------------------------------------------------------------------				
			}
			break;
		case n_WasteItem:
			{
				if( g_Pc_Manager.GetItemClsss( aItemList[i].theItem.theCode ) == 9 )
				{
					pc_item.IType	= n_ITYPE_USE;//소모품
				}
				else if( g_Pc_Manager.GetItemClsss( aItemList[i].theItem.theCode ) == 10 )
				{
					pc_item.IType	= n_ITYPE_UNUSE;//비소모품
				}
				else if( g_Pc_Manager.GetItemClsss( aItemList[i].theItem.theCode ) == 11 || 
					g_Pc_Manager.GetItemClsss( aItemList[i].theItem.theCode ) == 14 )
				{
					pc_item.IType	= n_ITYPE_SPECIAL;
				}				
				
				pc_item.Class		= g_Pc_Manager.GetItemClsss( aItemList[i].theItem.theCode );
				pc_item.ItemUID		= aItemList[i].theItem.theItemUID;
				pc_item.Code		= aItemList[i].theItem.theCode;
				pc_item.Tid			= (TEXID)g_Pc_Manager.GetItemTID( aItemList[i].theItem.theCode );
				pc_item.Amount		= aItemList[i].theWasteItem.theBundle;
				pc_item.ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aItemList[i].theItem.theCode );
			}
			break;
		}		
		
		m_YoursPcItem[i] = pc_item;		
	}
	
	return;
}