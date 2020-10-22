// AliveWnd.cpp: implementation of the CAliveWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"
 
#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "StateButton.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"


#include "AliveWnd.h"
#include "LoadingWndProc.h"
#include "stringmanager.h"



CAliveWnd	g_AliveWnd;
CAliveChargeWnd g_AliveChargeWnd;


///---------------------------------------------------------------------------
///-- Constructor
///---------------------------------------------------------------------------
CAliveWnd::CAliveWnd()
{
	///--JAPAN_BUG_MODIFY
	m_pMainFrame = NULL;
	Init();
}
///---------------------------------------------------------------------------
///-- Destructror
///---------------------------------------------------------------------------
CAliveWnd::~CAliveWnd()
{
}
//-----------------------------------------------------------------------------
void CAliveWnd::Init()
{
	m_AliveType = n_AnswerType_None;	
}

//-----------------------------------------------------------------------------
void CAliveWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	
//	CTextBox*		tb;	
//	CProgressBar*	pb;
	CTextBox*		tbox;
//	CStateButton*	sbtn;
//	CEditBox*		eb;
//	CSpin*			spin;
	CImageBox*		img;
//	CScrollBar*		sb;
	

	SRect alivewnd;
	alivewnd.w	= 178;
	alivewnd.h	= 100;
	alivewnd.x	= center_x - ( alivewnd.w / 2 );
	alivewnd.y	= center_y - ( alivewnd.h / 2 );

	///--JAPAN_BUG_MODIFY
	m_pMainFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_AliveWnd, WID_None, alivewnd.x, alivewnd.y, alivewnd.w, alivewnd.h, false );
	m_pMainFrame->SetClientImage( TID_None );	
	m_pMainFrame->SetWndProc( this );	
	m_pMainFrame->SetVisible( false );
	m_pMainFrame->SetMove( true );
	m_pMainFrame->SetZorder( n_zoTop );
	m_pMainFrame->Initialize();
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_Alive_AskImg, WID_AliveWnd, 0, 0, 178, 32, false );
	img->SetClientImage( TID_Alive_Wnd );	

	///-- 원석 (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AliveWnd, 0, 8, 178, 17, false );
	tbox->SetFontR( n_fRsvFontTitle );
	tbox->SetAlignText( n_atCenter, n_atCenter);	 
	tbox->SetText( G_STRING_CHAR( IDS_WND_DIE ) );
	
	// WID_Alive_OkBtn
//	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Alive_OkBtn, WID_AliveWnd, 2, 40, 174, 23, false );		
//	btn->SetBtnImage( TID_Alive_OkBtn_Normal, TID_Alive_OKBtn_Focus, TID_Alive_OkBtn_Select, TID_None );
//	btn->SetFontR( n_fRsvFontWndButton );
//	btn->SetTextColor( 255, 255, 255 );
//	btn->SetText( G_STRING_CHAR( IDS_WND_DIE_BT_TOWN ) );
//	btn->SetText( G_STRING_CHAR( IDS_WND_DIE_OBSERVER );			// 게임을 관전한다. 
//	btn->SetText( G_STRING_CHAR( IDS_WND_DIE_ROOMOUT );				// 대기실로 나간다.  
	
	int i = 0;

	for( i = 0; i < c_Max_AliveAnswerBtn_Num; ++i )
	{
		m_AliveAnwserBtn[i].answer_type = n_AnswerType_None;

		m_AliveAnwserBtn[i].select_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_AliveWnd, 2, 40 + ( i * 23 ) + ( i * 5 ) , 174, 23, false );		
		m_AliveAnwserBtn[i].select_btn->SetBtnImage( TID_Alive_OkBtn_Normal, TID_Alive_OKBtn_Focus, TID_Alive_OkBtn_Select, TID_None );
		m_AliveAnwserBtn[i].select_btn->SetFontR( n_fRsvFontWndButton );
		m_AliveAnwserBtn[i].select_btn->SetTextColor( 255, 255, 255 );
		m_AliveAnwserBtn[i].select_btn->SetText( _RT( "" ) );
		m_AliveAnwserBtn[i].select_btn->SetText( G_STRING_CHAR( IDS_WND_DIE_BT_TOWN ) );
		m_AliveAnwserBtn[i].select_btn->SetVisible( false );
	}
	
	//Open(n_AnswerWndType_Alive);

	return;	
}


//-----------------------------------------------------------------------------
void CAliveWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CAliveWnd::Proc( SEventMessage* EMsg )
{
//	switch( EMsg->FocusWnd )
//	{
//	case WID_Alive_OkBtn:
//		if( EMsg->Notify == NM_BTN_CLICK )
//		{
//			nRui->SendEvent( WID_AliveWnd,
//			n_emSetVisible, (EPARAM)false, 0, 0, 0 );
//			g_RockClient.Send_RpCsAlive();
//			LoadingBarWnd.m_IsMakeImg = true;
//		}
//		break;
//	} 

	if( m_AliveAnwserBtn[0].select_btn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ClickAnswerBtn( m_AliveAnwserBtn[0].answer_type );
		}
	}
	else if( m_AliveAnwserBtn[1].select_btn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ClickAnswerBtn( m_AliveAnwserBtn[1].answer_type );
		}
	}
	else if( m_AliveAnwserBtn[2].select_btn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ClickAnswerBtn( m_AliveAnwserBtn[2].answer_type );
		}
	}
	else if( m_AliveAnwserBtn[3].select_btn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ClickAnswerBtn( m_AliveAnwserBtn[3].answer_type );
		}
	}
	else if( m_AliveAnwserBtn[4].select_btn->GetID() == EMsg->FocusWnd )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			ClickAnswerBtn( m_AliveAnwserBtn[4].answer_type );
		}
	}

	return;
}

//-----------------------------------------------------------------------------

void CAliveWnd::Open( NAliveAnswerWndType aAliveAnswerWndType )
{
	nRui->SendEvent( WID_AliveWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
	int i = 0;

	switch( aAliveAnswerWndType )	
	{
	case n_AnswerWndType_Alive:
		{
			m_AliveAnwserBtn[0].answer_type = n_AnswerType_Alive;			
			m_AliveAnwserBtn[0].select_btn->SetText( G_STRING_CHAR( IDS_WND_DIE_BT_TOWN ) );			
			m_AliveAnwserBtn[0].select_btn->SetVisible( true );

			m_AliveAnwserBtn[1].answer_type = n_AnswerType_UseChargeitem;			
			m_AliveAnwserBtn[1].select_btn->SetText( G_STRING_CHAR( IDS_USE_CHARGE_ITEM ) );		
			m_AliveAnwserBtn[1].select_btn->SetVisible( true );
			
#ifdef C_PAY_SYSTEM
			for( i = 2; i < c_Max_AliveAnswerBtn_Num; ++i )
			{
				m_AliveAnwserBtn[i].answer_type = n_AnswerType_None;			
				m_AliveAnwserBtn[i].select_btn->SetText( _RT( "" ) );
				m_AliveAnwserBtn[i].select_btn->SetVisible( false );
			}
#else
			for( i = 1; i < c_Max_AliveAnswerBtn_Num; ++i )
			{
				m_AliveAnwserBtn[i].answer_type = n_AnswerType_None;			
				m_AliveAnwserBtn[i].select_btn->SetText( _RT( "" ) );
				m_AliveAnwserBtn[i].select_btn->SetVisible( false );
			}
#endif
		}
		break;
	case n_AnswerWndType_Observer_RoomOut:
		{
			m_AliveAnwserBtn[0].answer_type = n_AnswerType_Observer;			
			m_AliveAnwserBtn[0].select_btn->SetText( G_STRING_CHAR( IDS_WND_DIE_OBSERVER ) );			
			m_AliveAnwserBtn[0].select_btn->SetVisible( true );

			m_AliveAnwserBtn[1].answer_type = n_AnswerType_RoomOut;			
			m_AliveAnwserBtn[1].select_btn->SetText( G_STRING_CHAR( IDS_WND_DIE_ROOMOUT ) );			
			m_AliveAnwserBtn[1].select_btn->SetVisible( true );

			for( i = 2; i < c_Max_AliveAnswerBtn_Num; ++i )
			{
				m_AliveAnwserBtn[i].answer_type = n_AnswerType_None;			
				m_AliveAnwserBtn[i].select_btn->SetText( _RT( "" ) );
				m_AliveAnwserBtn[i].select_btn->SetVisible( false );
			}
		}
		break;
	default:
		break;
	}	

	return;
}


void CAliveWnd::ClickAnswerBtn( NAliveAnswerType aAliveAnswerType )
{
	m_AliveType = aAliveAnswerType;

	switch( aAliveAnswerType )	
	{
	case n_AnswerType_None:
		{	
			nRui->SendEvent( WID_AliveWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );			
		}
		break;
	case n_AnswerType_Alive:
		{	
			g_RockClient.Send_RpCsAlive(0);
			LoadingBarWnd.m_IsMakeImg = true;
		}
		break;
	case n_AnswerType_UseChargeitem:
		{
			if( !g_AliveChargeWnd.OpenWnd() )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOITEM_ALIVE  ) );	
			}			
		}
		break;
	case n_AnswerType_Observer:
		{			
			nRui->SendEvent( WID_AliveWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
		break;
	case n_AnswerType_RoomOut:
		{			
			g_RockClient.Send_CsOutFightRoom( g_Pc.m_MainPC.lUnique );
			nRui->SendEvent( WID_AliveWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
		break;
	default:
		break;
	}	
	
	return;
}

CAliveChargeWnd::CAliveChargeWnd()
{
	Init();
}

CAliveChargeWnd::~CAliveChargeWnd()
{

}

void CAliveChargeWnd::Init()
{

}

void CAliveChargeWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CTextBox*		tbox = NULL;
	CImageBox*		img = NULL;

	SRect ChargeWnd;
	ChargeWnd.w	= 166;
	ChargeWnd.h	= 108;
	ChargeWnd.x	= center_x - ( ChargeWnd.w / 2 );
	ChargeWnd.y	= center_y -  50 - ( ChargeWnd.h / 2 );

	///-- 유료 아이템 등록 윈도우
	m_pFramwWnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
		ChargeWnd.x, ChargeWnd.y, ChargeWnd.w, ChargeWnd.h, false );
	m_pFramwWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFramwWnd->SetClientImage( TID_CTRL_WND_NL1 );
	m_pFramwWnd->SetWndTileSizeLR( 5, 5 );
	m_pFramwWnd->SetWndTileSizeTB( 5, 5 );	
	m_pFramwWnd->SetTitle( false );
	m_pFramwWnd->SetWndProc( this );
	m_pFramwWnd->SetVisible( false );
	m_pFramwWnd->SetZorder( n_zoTop );
	m_pFramwWnd->SetMove( false );
	m_pFramwWnd->Initialize();

	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFramwWnd->GetID(), 
		26, 13, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFramwWnd->GetID(), 
		66, 13, 35, 35, false );      
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFramwWnd->GetID(), 
		106, 13, 35, 35, false );     
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );

	m_pChargeitem_slot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  m_pFramwWnd->GetID(), 26, 13, 35, 35, false );
	m_pChargeitem_slot[0]->PcItem = &(m_Chargeslot_item[0]);	
	m_pChargeitem_slot[0]->SetHID( HID_ITEMTIP );
	m_pChargeitem_slot[0]->SetButtonSlot(true);

	m_pChargeitem_slot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  m_pFramwWnd->GetID(), 66, 13, 35, 35, false );
	m_pChargeitem_slot[1]->PcItem = &(m_Chargeslot_item[1]);	
	m_pChargeitem_slot[1]->SetHID( HID_ITEMTIP );
	m_pChargeitem_slot[1]->SetButtonSlot(true);

	m_pChargeitem_slot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
							  m_pFramwWnd->GetID(), 106, 13, 35, 35, false );
	m_pChargeitem_slot[2]->PcItem = &(m_Chargeslot_item[2]);	
	m_pChargeitem_slot[2]->SetHID( HID_ITEMTIP );
	m_pChargeitem_slot[2]->SetButtonSlot(true);

	///-- 유료 아이템 (Static)
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFramwWnd->GetID(), 
			43, 51, 90, 17, false );
	tbox->SetFontR( n_fRsvFontWndStatic );	
	tbox->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );
	
	m_pClose_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFramwWnd->GetID(), 
									57, 73, 58, 25, false );
	m_pClose_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );			
	m_pClose_btn->SetFontR( n_fRsvFontWndButton ); 
	m_pClose_btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );	
}

void CAliveChargeWnd::Update()
{

}

void CAliveChargeWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == m_pClose_btn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			CloseWnd();		
		}
	}
	
	for(int i = 0; i < 3 ; i++)
	{	
		if( EMsg->FocusWnd == m_pChargeitem_slot[i]->GetID() )
		{
			if( EMsg->Notify == NM_SLOT_CLICK )
			{
				DWORD dPayItem = m_Chargeslot_item[i].ItemUID;
				if( 0 != dPayItem )
				{
					CloseWnd();
					g_RockClient.Send_RpCsAlive(dPayItem);
					LoadingBarWnd.m_IsMakeImg = true;
				}
			}
		}
	}
}

bool CAliveChargeWnd::OpenWnd()
{
	bool bFindFlag = false;
	int i, j;
	i = j = 0;

	static int nScrollCode[3] = { n_Scroll_AliveNoPenalty,
								  n_Scroll_AliveExpPenalty,
								  n_Scroll_AliveItemPenalty	};

	bool  nItemFindFlag[3] = {false, false, false}; 	

	///--- 인벤창에서 스크롤 아이템중 부활관련 아이템을 찾아낸다.
	///-- 전체 인벤토리 Access
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{
		if( nRui->GetPcInvenInfo()->InvenItem[i] == NULL )
		{
			continue;
		}

		if( n_Type_Scroll != nRui->GetPcInvenInfo()->InvenItem[i]->ItemTable->theType )
		{
			continue;
		}

		for( j = 0 ; j < 3 ; j++ )
		{
			if( !nItemFindFlag[j] &&
				(nScrollCode[j] == nRui->GetPcInvenInfo()->InvenItem[i]->ItemTable->theReHP) )
			{
				nItemFindFlag[j] = true;
				bFindFlag = true;

				m_pChargeitem_slot[j]->SetSlotPcItem( *(nRui->GetPcInvenInfo()->InvenItem[i]) );
				m_pChargeitem_slot[j]->PcItem->Amount = 1;
			}		
		}	
	}	
	
	if(bFindFlag)
	{
		nRui->SendEvent( m_pFramwWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SetModal( m_pFramwWnd->GetID() );
	}	
	
	return bFindFlag;
}

void CAliveChargeWnd::CloseWnd()
{
	nRui->SendEvent( m_pFramwWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	for(int i = 0 ; i < 3 ; i++ )
	{
		m_Chargeslot_item[i].Clear();
	}
}


