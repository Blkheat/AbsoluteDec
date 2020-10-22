
#include "..\\RockPCH.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"
#include "Rui.h"

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Spin.h"
#include "StateButton.h"
#include "ChatOutput.h"
#include "Slot.h"

#include "PlayerInfoAddWnd.h"

#include "PlayerInfoWnd.h"

#include "..\\..\\brother.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\Map.h"

#include "PartyInforWnd.h"
#include "InventoryWndProc.h"
#include "CUserStoreTitleWnd.h"
#include "CUserStoreSellWnd.h"
#include "CSocialActWnd.h"
#include "stringmanager.h"
#include "MapDataManager.h"

#include "PetMainWndProc.h"



CPlayerInfoAddWnd PlayerInfoAddWnd;

CPlayerInfoAddWnd::CPlayerInfoAddWnd()
{
	m_pFrameWnd = NULL;
	m_pMyShopBtn = NULL;
	m_pSoActBtn = NULL;
	m_pPVPBtn = NULL;
	m_pPetInfoBtn = NULL;

	m_IsMySellShop = false;

	m_pPKModeBtn = NULL;
}

CPlayerInfoAddWnd::~CPlayerInfoAddWnd()
{
	
}

void CPlayerInfoAddWnd::Init()
{
	
}

void CPlayerInfoAddWnd::Composition()
{

	SRect sFrameRect;
	SRect sPetInfoBtnRect;
	SRect sPKModeBtnRect;

#ifdef PET_SYSTEM 

	sFrameRect.x = 275;
	sFrameRect.y = 30;
	sFrameRect.w = 109;
	sFrameRect.h = 28;

	#ifdef CPK_SYSTEM

		sFrameRect.x = 275;
		sFrameRect.y = 30;
		sFrameRect.w = 135;
		sFrameRect.h = 28;

	#endif	

#else

	sFrameRect.x = 275;
	sFrameRect.y = 30;
	sFrameRect.w = 86;
	sFrameRect.h = 28;

	#ifdef CPK_SYSTEM

		sFrameRect.x = 275;
		sFrameRect.y = 30;
		sFrameRect.w = 109;
		sFrameRect.h = 28;

	#endif

#endif

	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
		sFrameRect.x, sFrameRect.y, sFrameRect.w, sFrameRect.h, false );
	m_pFrameWnd->SetClientImage( TID_icon_boxbarR );
	m_pFrameWnd->SetCaption( _RT("") );
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetTitle( false );
	m_pFrameWnd->SetMove( false );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->SetZorder( n_zoTop );
	m_pFrameWnd->Initialize();	

#ifdef PET_SYSTEM
	
	sPetInfoBtnRect.x = 81;
	sPetInfoBtnRect.y = 3;
	sPetInfoBtnRect.w = 24;
	sPetInfoBtnRect.h = 22;
	
	m_pPetInfoBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 
											   sPetInfoBtnRect.x, sPetInfoBtnRect.y, 
											   sPetInfoBtnRect.w, sPetInfoBtnRect.h, false );
	m_pPetInfoBtn->SetBtnImage( TID_icon_pet, TID_icon_pet_bright, 
		TID_icon_pet_dark, TID_None );
	m_pPetInfoBtn->SetHID(HID_PET_INFO_BTN);

#endif

	
#ifdef	CPK_SYSTEM
	
	sPKModeBtnRect.x = 81;
	sPKModeBtnRect.y = 3;
	sPKModeBtnRect.w = 24;
	sPKModeBtnRect.h = 22;

	#ifdef PET_SYSTEM

		sPKModeBtnRect.x = 107;
		sPKModeBtnRect.y = 3;
		sPKModeBtnRect.w = 24;
		sPKModeBtnRect.h = 22;

	#endif

	m_pPKModeBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
					m_pFrameWnd->GetID(),  sPKModeBtnRect.x , sPKModeBtnRect.y, 
					sPKModeBtnRect.w , sPKModeBtnRect.h , false );
	m_pPKModeBtn->SetBtnImage( TID_icon_PKMode, TID_icon_PKMode_bright, 
							   TID_icon_PKMode_dark, TID_None );	

#endif

	m_pMyShopBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
	m_pFrameWnd->GetID(),  3, 3, 24, 22, false );
	m_pMyShopBtn->SetBtnImage( TID_icon_store, TID_icon_store_bright, 
		TID_icon_store_dark, TID_None );
	m_pMyShopBtn->SetHID(HD_MMY_SHOP);
	
	m_pSoActBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
		m_pFrameWnd->GetID(),  29, 3, 24, 22, false );
	m_pSoActBtn->SetBtnImage( TID_icon_emotion, TID_icon_emotion_bright, 
		TID_icon_emotion_dark, TID_None );
	m_pSoActBtn->SetHID(HD_MSO_ACT);
	
	m_pPVPBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
		m_pFrameWnd->GetID(),  55, 3, 24, 22, false );
	m_pPVPBtn->SetBtnImage( TID_icon_worldwar, TID_icon_worldwar_bright, 
		TID_icon_worldwar_dark, TID_None );
	m_pPVPBtn->SetHID(HD_PVPZONE);

	return;
}

void CPlayerInfoAddWnd::Update()
{
	
}

void CPlayerInfoAddWnd::Proc( SEventMessage* EMsg )
{
	if( g_Pc.GetPlayer()->theMapType == n_FightZone )
	{
		return;
	}	
	
	//개인 상점
	if( EMsg->FocusWnd == m_pMyShopBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			//SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_PREPARE ) );

			if( g_SocialActWnd.IsVisible )
			{
				g_SocialActWnd.Close();
			}
			
			//라이딩 중이면 사용할수 없다
			if(g_Pc.GetPlayer()->m_State.bRide)
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_THEFUNCTION_INRIDING ) );										
				return;
			}			
			
			if( (nRui->thePcParam.MainClass == Wanderer) || 
				(nRui->thePcParam.MainClass == Stranger) )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, 
							G_STRING_CHAR( IDS_USERSTROE_NOT_BEFORE_FIRSTJOP ) );		
				return;
			}

			
			if( g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN ||
				g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING ||
				g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_STANDUP   )
			{
				return;
			}			
			
			if( !g_UserStoreSellWnd.ISUserSellMode() )
			{
				g_RockClient.Send_CsOpenPCShopWindow();					//개인상점 요청
			}
			else if( g_UserStoreSellWnd.m_UserStoreSellOpenMode )		//개인상점 닫기
			{
				g_RockClient.Send_CsClosePCShop();
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_USE ) );
			}
		}
	}	
	
	//사교동작
	if( EMsg->FocusWnd == m_pSoActBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			switch( g_Pc.GetPlayer()->m_curt_event.type )
			{
			case NULL:
			case SM_SIMSIM:
			case SM_ACT:
			case SM_RIDING:
				break;
				
			default:
				SystemPrint( n_DarkRed, n_SystemMsg, 
							 //G_STRING_CHAR( IDS_USERSTORE_USE ) ); // 개인 상점중에는 사용하실수없습니다.
						    G_STRING_CHAR( IDS_COMMUNITY_NOT_IN_NOW_STATE ));			
				return;
			}
			
			if( g_UserStoreSellWnd.IsVisible )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_USE ) ); // 개인 상점중에는 사용하실수없습니다.
				return;
			}
			if( g_SocialActWnd.IsVisible )
			{
				g_SocialActWnd.Close();
			}
			else
			{
				g_SocialActWnd.Open();
			}
		}
	}

	if( EMsg->FocusWnd == m_pPVPBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			if( g_UserStoreSellWnd.IsVisible )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_USE ) ); // 개인 상점중에는 사용하실수없습니다.
				return;
			}
#ifdef CUR_INDUN_CHECK
			if( g_MapDataManager.GetInDunInfo(g_Map.m_NowMapID) ) 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INDUN_LIMIT ) ); // 개인 상점중에는 사용하실수없습니다.
				return; 
			}
#endif 
			nRui->SendEvent( WID_BZ_SELECTZONE_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );			
		}
	}

#ifdef PET_SYSTEM
	if( EMsg->FocusWnd == m_pPetInfoBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{		

#ifdef SINGAPORE_BLOCK_PET
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_COMING_SOON_PET ) );
#else
		
			if(g_PetMainWndProc.IsVisible)			
			{
				nRui->SendEvent( WID_PETMAIN_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );			
			}
			else
			{
				nRui->SendEvent( WID_PETMAIN_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );			
			}	
#endif			
		}
	}
#endif

#ifdef CPK_SYSTEM
	if( EMsg->FocusWnd == m_pPKModeBtn->GetID() )
	{	
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			///-- PK 모드 관련 동작 수행
			g_Pc.GetPlayer()->ExcutePKModeAction();
		}
	}
#endif

	return;
}

UINT CPlayerInfoAddWnd::GetFrameWndID()
{
	if(m_pFrameWnd)
	{
		return m_pFrameWnd->GetID();
	}
	
	return 0;
}

void  CPlayerInfoAddWnd::Open()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, 
		(EPARAM)TRUE, 0, 0, 0 );
	
}

void  CPlayerInfoAddWnd::Close()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, 
		(EPARAM)FALSE, 0, 0, 0 );
	
	if( g_SocialActWnd.IsVisible )
	{
		g_SocialActWnd.Close();
	}
}
