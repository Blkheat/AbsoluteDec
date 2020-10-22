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
//#include "pc.h"
#include "..\\..\\Effect.h"
#include "..\\..\\CItemTexLoadThread.h"
#include "..\\..\\bravolibs\\obj\\TextureManager.H"


#include "ChatBoxWndProc.h"
#include "SysMsgWndProc.h"
#include "SystemMenuWndProc.h"
#include "ChatOutput.h"
#include "SysMsgOutput.h"
#include "QuestProc.h"
#include "MainMenuWndProc.h"
#include "PlayerInfoWnd.h"
#include "PlayerInfoAddWnd.h"
#include "NonPlayerInfoWnd.h"
#include "ExchangeWndProc.h"
#include "WarehouseWndProc.h"
#include "GuildInventoryWndProc.h"
#include "CLogWnd.h"

#include "CDelayBarWnd.h"
#include "CSlotDelayProc.h"
#include "CPAcceptYesNoWnd.h"

#include "UIMsgWnd.h"
#include "PartyInforWnd.h"
#include "PartyPopupWnd.h"

#include "MiniMapWndProc.h"
#include "NpcChatWnd.h"
#include "MCommunityWnd.h"
#include "OptionWndProc.h"
#include "ChatMainProc.h"
#include "SystemPrintWnd.h"
#include "Announcement.h"
#include "CUserStoreBuyWnd.h"
#include "CUserStoreSellWnd.h"
#include "stringmanager.h"
#include "NonPlayerInfoAddWnd.h"
#include "PayItemWndProc.h"
#include "LeagueInfoWnd.h"
#include "FrontierMainWndProc.h"

#include "PetMainWndProc.h"
#include "PetInfoWnd.h"

#include "CColRegionListWnd.h"
#include "CColRegionInfoWnd.h"
#include "CColItemPreViewWnd.h"
#include "CColMapPositionWnd.h"

#include "CAcryEnchantWnd.h"
#include "CAcryEnchantRWnd.h"

#include "CChangeEldaColorWnd.h"
#include "CChangeEldaColorRWnd.h"
#include "ItemEldaDust.h"
#include "CWebShopWnd.h"

#include "EssenceWndProc.h"
#include "ItemEnchantWndProc.h"
#include "SpellStoneWndProc.h"
#include "StrengthStoneWndProc.h"


CSystemMenuWnd	SystemMenuWnd;

//-----------------------------------------------------------------------------
void CSystemMenuWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CSystemMenuWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CImageBox*		img;


	SRect systemwnd;
	systemwnd.w	= 175;
	systemwnd.h	= 172;
	systemwnd.x	= center_x - ( systemwnd.w / 2 );
	systemwnd.y	= center_y - ( systemwnd.h / 2 );
 
	
	m_pFramwWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SystemMenuWnd, WID_None, systemwnd.x, systemwnd.y, systemwnd.w, systemwnd.h, false );	
	m_pFramwWnd->SetClientImage( TID_None);		
	m_pFramwWnd->SetTitle( false );
	m_pFramwWnd->SetZorder( n_zoTop );
	m_pFramwWnd->SetWndProc( this );			
	m_pFramwWnd->SetVisible( false );
	m_pFramwWnd->Initialize();

		
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_SystemMenuWnd, 0, 0, 178, 32, false );
	img->SetClientImage( TID_Alive_Wnd );
	img->SetFocusCheck( false );
	
	textbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_SystemMenuWnd, 0, 8, 178, 17, false );
	textbox->SetFontR( n_fRsvFontTitle );
	textbox->SetAlignText( n_atCenter, n_atCenter );
	textbox->SetText( G_STRING_CHAR( IDS_WND_SYSTEM )  );	

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------
	
	//나가기
	m_pGoOutBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_SystemMenuWnd, 2,  154, 174, 23, false );       
	m_pGoOutBtn->SetClientImage( TID_None );
	m_pGoOutBtn->SetBtnImage( TID_Alive_OkBtn_Normal, TID_Alive_OKBtn_Focus, TID_Alive_OkBtn_Select, TID_None );
	m_pGoOutBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pGoOutBtn->SetTextColor( 200, 200, 200);
	m_pGoOutBtn->SetText( G_STRING_CHAR( IDS_WND_GOOUT ) );
	m_pGoOutBtn->SetVisible(false);
	m_pGoOutBtn->SetHID( HD_GOOUT );

	//로그 오프
	m_pLogOffBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_System_LogOffBtn, WID_SystemMenuWnd, 2,  34, 174, 23, false );       
	m_pLogOffBtn->SetClientImage( TID_None );
	m_pLogOffBtn->SetBtnImage( TID_Alive_OkBtn_Normal, TID_Alive_OKBtn_Focus, TID_Alive_OkBtn_Select, TID_None );
	m_pLogOffBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pLogOffBtn->SetTextColor( 200, 200, 200);
	m_pLogOffBtn->SetText( G_STRING_CHAR( IDS_WND_LOGOFF ) ); 
	m_pLogOffBtn->SetHID( HD_LOGOFF );

	//게임 종료
	m_pGameExitBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_System_GameExitBtn, WID_SystemMenuWnd, 2,  58, 174, 23, false );       
	m_pGameExitBtn->SetClientImage( TID_None );
	m_pGameExitBtn->SetBtnImage( TID_Alive_OkBtn_Normal, TID_Alive_OKBtn_Focus, TID_Alive_OkBtn_Select, TID_None );
	m_pGameExitBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pGameExitBtn->SetTextColor( 200, 200, 200);
	m_pGameExitBtn->SetText( G_STRING_CHAR( IDS_WND_GAME_EXIT ));
	m_pGameExitBtn->SetHID( HD_QUIT );

	//스크린 샷
	m_pScreenShotBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_System_ScreenShotBtn, WID_SystemMenuWnd, 2,  82, 174, 23, false );    
	m_pScreenShotBtn->SetClientImage( TID_None );
	m_pScreenShotBtn->SetBtnImage( TID_Alive_OkBtn_Normal, TID_Alive_OKBtn_Focus, TID_Alive_OkBtn_Select, TID_None );
	m_pScreenShotBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pScreenShotBtn->SetTextColor( 200, 200, 200);
	m_pScreenShotBtn->SetText( G_STRING_CHAR( IDS_WND_SCREENSHOT) ); 
	m_pScreenShotBtn->SetHID( HD_SCREEN_SHOT );

	//옵션
	m_pOptionBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_System_OptionBtn, WID_SystemMenuWnd, 2, 106, 174, 23, false ); 
	m_pOptionBtn->SetClientImage( TID_None );
	m_pOptionBtn->SetBtnImage( TID_Alive_OkBtn_Normal, TID_Alive_OKBtn_Focus, TID_Alive_OkBtn_Select, TID_None );
	m_pOptionBtn->SetAlignText( n_atCenter, n_atCenter );
	m_pOptionBtn->SetTextColor( 200, 200, 200);
	m_pOptionBtn->SetText( G_STRING_CHAR( IDS_WND_OPTION2 ) ); 
	m_pOptionBtn->SetHID( HD_OPTION );

	///-- X
	m_CloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_System_CloseBtn, WID_SystemMenuWnd, 2, 130, 174, 23, false );   
	m_CloseBtn->SetClientImage( TID_None );
	m_CloseBtn->SetBtnImage( TID_Alive_OkBtn_Normal, TID_Alive_OKBtn_Focus, TID_Alive_OkBtn_Select, TID_None );
	m_CloseBtn->SetAlignText( n_atCenter, n_atCenter );
	m_CloseBtn->SetTextColor( 200, 200, 200);
	m_CloseBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 
	m_CloseBtn->SetHID( HD_CLOSE_WINDOWS );
}

//-----------------------------------------------------------------------------
void CSystemMenuWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CSystemMenuWnd::Proc( SEventMessage* EMsg )
{
	CWnd* Wnd = NULL;

	if( EMsg->FocusWnd == m_pGoOutBtn->GetID())
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsOutFightRoom( g_Pc.m_MainPC.lUnique );
			Close();
		}
	}
	
	switch( EMsg->FocusWnd )
	{
	case WID_System_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
		break;
	case WID_System_LogOffBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			//By simwoosung
			if( g_DelayBarWnd.IsVisible && (g_DelayBarWnd.GetDelay_Type()  != DELAY_LOG_OFF))
			{
				g_DelayBarWnd.Clear();
			}
			
			if(g_DelayBarWnd.IsStart())
				return;				
			
		bool Is_Debug = false;	
#ifdef	_DEBUG
		 Is_Debug = true;  
#endif				
			if(!g_DelayBarWnd.IsActive() && !Is_Debug)			
			{				
				g_RockClient.Send_RpCs10SecDelay(n_DelayLogout);
				nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				BOOL bFlag = FALSE;
				Player*	user_pc = g_Pc.GetPlayer();

				///--JAPAN_BUG_MODIFY
				if(user_pc->IsRealDie())
				{
                   bFlag = TRUE;    
				}				
				
				g_DelayBarWnd.ResetDelay(DELAY_LOG_OFF, 0, bFlag);                
			}
			else
			{     				
				GameLogOff();
#ifdef RETURN_SERVERLIST_MJH
				g_Particle.DelEffSetCon( EF_CORE0 , 100 , -1 , -1 );				
				g_RockClient.SelecttoServerList();

				g_RockClient.CloseLoginState();
				g_Particle.m_Fade.Start( FADE_STATE_LOOP , g_Particle.m_Fade.GetColor() , 
										   D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) ,
										   D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) ,
											1 , 2500 , FALSE );
				g_RockClient.LogOff();
				if( g_RockClient.ReLoginProc( ) )
				{
				
				nRui->SendEvent( WID_CharSelect_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

				g_RockClient.m_IsReConnect = TRUE;
			//	m_ISChangeSkyBox = TRUE;

				g_Camera.SetStartFrame( 0 , g_lpDevice );
				g_RockClient.m_LoginShutter.Reset();

				}
#endif //RETURN_SERVERLIST_MJH
			}

		}
		break;
	case WID_System_GameExitBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			GameExitBtn();
		}
		break;
	case WID_System_ScreenShotBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.ScreenCapture();
		}
		break;
	case WID_System_OptionBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_OptionWnd.Open();;
		}
		break;
	case WID_System_CommonWordsBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
		}
		break;
	default:
		break;
	}	
}

void CSystemMenuWnd::GameLogOff()
{
	g_TexManager.ClearTheCreateTexList();				
	g_Pc.GetPlayer()->theMapType = n_NormalZone;
	
	if( NpcChatWnd.IsVisible ) 
	{
		NpcChatWnd.CloseNpcChatWnd(); 
	}
	else
	{
		NpcChatWnd.CloseNpcChatWnd( false ); 
	}

	//소켓 끊기, 맵체인지
	g_RockClient.Send_RpCsPCLogout( n_GAME_LOGOUT );	/// 이러면 서버에서 알아서 소켓을 끊습니다. 
	g_RockClient.LogOff();
	
	g_Particle.m_WeatherDrop.Reset();
	g_Particle.DeleteAll(TRUE);
	g_Billboard.DelAllManager(TRUE);
	g_Particle.EffSetConAllInit(TRUE);
	g_Particle.m_EffectManager.AllClear();
	
	//캐릭터 아이디 안나오게....설정
	nRui->theViewPcInfo = n_VIEW_NONE_PCID;
	
	g_Pc.GetPlayer()->m_ISApproachWeapon = FALSE;
	g_Pc.GetPlayer()->m_AttackMotion     = 0;

	//로그인창 외에 다른창들은 다 닫기
	//-----------------------------------------------------------------
	SystemMenuWnd.Close();
	nRui->SendEvent( WID_MainMenuLv_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_MainMenu_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_MainExp_Wnd	, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_MainButtons_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );					
	nRui->SendEvent( WID_MainMenu_QuestWndBtn, n_emSetClientImage , (EPARAM)TID_None, 0, 0, 0 );
	nRui->SendEvent( WID_MAIN_ENDU_WARN	, n_emSetVisible, (EPARAM)false, 0, 0, 0 );				
	nRui->SendEvent( WID_PlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_AddEffectMsgFrame , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_TOTEMSCORE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_OptWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

	g_NonPlayerInfoAdd2Wnd.Close();				
	
	MiniMapWnd.LogOff();
	nRui->SendEvent( WID_MiniMapWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

	//채팅창 로그오프
	g_ChatMainProc.LogOff();
	g_SystemPrintWnd.LogOff();
	
	nRui->SendEvent( WID_ComboGauge_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	
	nRui->SendEvent( WID_AbilityUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_AbilityUpConfirmWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_AliveWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	
	nRui->SendEvent( WID_ServerListWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_StatWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장
	nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );//유료인벤확장
#endif	
#ifdef TAIWAN30MINEVENT	
	nRui->SendEvent( WID_MainEventButtons_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );//대만 출석 이벤트
#endif //TAIWAN30MINEVENT

	nRui->SendEvent( WID_SkillWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
//				nRui->SendEvent( WID_SkillRegistWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
//				nRui->SendEvent( WID_SkillInfoWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
//				nRui->SendEvent( WID_SkillOverWriteWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_Quest_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );			
	
	nRui->SendEvent( g_LogWnd.m_pFrame->GetID(), n_emSetVisible, (EPARAM)FALSE, 0, 0, 0 );

	QuestWnd.SetInsertHighlight( false );			
	
	nRui->SendEvent( WID_StoreWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_ItemBuyWnd, n_emSetVisible, false, 0, 0, 0 );
	nRui->SendEvent( WID_ItemSellWnd, n_emSetVisible, false, 0, 0, 0 );
	nRui->SetModal( WID_None );
	//SendEvent( WID_BankWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	//SendEvent( WID_WithdrawalWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	//SendEvent( WID_DepositWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_NumberInput_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_MoneyThrow_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_MessageWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_CharSelect_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	PlayerInfoWnd.DeleteAllKeepUpSkillCode();
	NonPlayerInfoWnd.DeleteAllKeepUpSkill();
	nRui->SendEvent( WID_SUB_ENCHANT, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_ITEMCREATE_WND ,n_emSetVisible, (EPARAM)false, 0, 0, 0 ); 
	//-----------------------------------------------------------------						
	
	// 말풍선 초기화
	nRui->InitDlgBalloon();				

	SysMsgOutputWnd.ReSetMsgTxt();
	
	//아이템 딜레이 누적루틴 초기화
	g_SlotDelayProc.InitiallizeUseDelayTime();
	g_SlotDelayProc.InitiallizeSkillDelayTime();
	
	nRui->LogOff();
	//PcParam, PcInven의 전 캐릭터 데이타 초기화
	
	//펫 
	g_PetMainWndProc.LogOff();

	nRui->InitMyPcData();
	//아래 두개는 이곳에 위치해야함..;
	MainMenuWnd.LogOff();
	g_main_buttons.LogOff();
	PlayerInfoWnd.LogOff();	
	PlayerInfoAddWnd.Close();
	g_LeagueInfoWnd.LogOff();

	g_AcceptYesNoWnd.LogOff();				     				

	//일단 파티멤버이고 로그오프를 누를시에는
	//파티 정보창을 감춘다. By simwoosung //차후 재접시에 처리..
	//이건 게임종료시엔 어떻게 처리가 되나.
	g_PartyInforWnd.LogOff();
	g_UIMsgWnd.LogOff();
	g_PartyPopupWnd.LogOff();
	

	nRui->SendEvent( WID_Exchange_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_Warehouse_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

	g_ExchangeWnd.LogOff();
	g_WarehouseWnd.LogOff();
#ifdef DECO_RENEWAL_GUILD_MJH
	g_GuildInvenWnd.LogOff();
#endif // DECO_RENEWAL_GUILD_MJH
	

	//커뮤니티 윈도우 로그오프
	g_MCommunityWnd.LogOff();
	g_Announcement.LogOff();
	
	g_LogWnd.ClearLog();
	g_UserStoreBuyWnd.Close();
	g_UserStoreSellWnd.Close();
	
	g_PayItemWnd.LogOff();
	g_FrontierMainWnd.LogOff();
#ifdef DECO_RENEWAL_MJH
	nRui->SendEvent( WID_FrontierCreateScroll, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_FRONTIER_BTN_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#endif // DECO_RENEWAL_MJH

	///-- 콜로니 관련
//	g_ColRegionListWnd.LogOff();
//	g_ColRegionInfoWnd.LogOff();			
//	g_ColMapPositionWnd->LogOff();				

	if( g_pAcryEnchantWnd )
	{
		g_pAcryEnchantWnd->Close();
	}

	if(  g_pAcryEnchantRWnd )
	{
		g_pAcryEnchantRWnd->Close();
	}

	//엘다스톤 색상 변경창
	if( g_pChangeEldaColorWnd ) 
	{ 
		g_pChangeEldaColorWnd->Close();
	} 
	//엘다스톤 색상 변경 결과창
	if( g_pChangeEldaColorRWnd ) 
	{ 
		g_pChangeEldaColorRWnd->Close(); 
	}
	   



	g_ItemEldaDustWndProc.Close();

	g_WebShopWnd.LogOff();


	g_Pc.GetPlayer()->m_nViewRefDestIndex = -1;
	g_Camera.m_unique = -1;
	g_Camera.m_bRotBySort = true;
	
	//로그인 창 뛰우기 맨 마지막에
#ifdef RETURN_SERVERLIST_MJH
#else
	nRui->SendEvent( WID_Login_UserNameEdit, n_emSetText, (EPARAM)_T(""), 255, 255, 255 );
	nRui->SendEvent( WID_Login_PasswordEdit, n_emSetText, (EPARAM)_T(""), 255, 255, 255 );			
#endif // RETURN_SERVERLIST_MJH
	
	//로그인창 띄우기 - 일반 로그인일 경우에만
	if(n_Normal != g_RockClient.m_GameTypeInfo.theFrom)
	{
		#ifdef LOGINPAGE_BGIMG_CHANGE
			nRui->EnableBGround( TRUE );
			nRui->SendEvent( WID_LoginWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
			nRui->SendEvent( WID_Login_UserNameEdit, n_emSetFocus, 0, 0, 0, 0 );
		#else
			nRui->EnableBGround( FALSE );			
		#endif
	}
	else
	{
#ifdef RETURN_SERVERLIST_MJH
#else
		nRui->SendEvent( WID_LoginWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
		nRui->SendEvent( WID_Login_UserNameEdit, n_emSetFocus, 0, 0, 0, 0 );
#endif // RETURN_SERVERLIST_MJH
	}

	g_RockClient.m_SquenceManager.DeleteAllMemory();	
}

void CSystemMenuWnd::GameExitBtn()
{
	   
	
	//			if( g_Pc.GetPlayer()->theMapType == n_FightZone )
	//			{
	//				g_RockClient.Send_CsOutWaitingRoom();
	//				return;
	//			}
	if(g_DelayBarWnd.IsStart())
		return;	
	
	bool Is_Debug = false;	
#ifdef	_DEBUG
	Is_Debug = true;
#endif				
	if(!g_DelayBarWnd.IsActive() && !Is_Debug)		
	{
		g_RockClient.Send_RpCs10SecDelay(n_DelayLogout);
		nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		BOOL bFlag = FALSE;
		Player*	user_pc = g_Pc.GetPlayer();
		
		///--JAPAN_BUG_MODIFY
		if(user_pc->IsRealDie())
		{
			bFlag = TRUE;    
		}

		g_DelayBarWnd.ResetDelay(DELAY_GAME_OUT, 0, bFlag);                
	}
	else
	{                 
		g_TexManager.ClearTheCreateTexList();
		
		g_RockClient.Send_RpCsPCLogout( n_GAME_LOGOUT );	/// 이러면 서버에서 알아서 소켓을 끊습니다. 
		g_Particle.DeleteAll(TRUE);
		g_Billboard.DelAllManager(TRUE);
		g_Particle.EffSetConAllInit(TRUE);
		g_RockClient.m_SquenceManager.DeleteAllMemory();
		
		//by simwoosung
		//일단 파티 초대후 로그오프나 게임종료를 누를시에는
		//파티 거부로 처리
		g_AcceptYesNoWnd.LogOff();
		g_PartyInforWnd.LogOff();
		g_UIMsgWnd.LogOff();
		g_PartyPopupWnd.LogOff();
		
#ifdef	NETWORK_CLIENT			
		if( g_RockClient.GetConnectState() == nInGameServer )	/// sooree network 
		{
			if( g_RockClient.theUserSelectExit == FALSE )
			{
				g_RockClient.theUserSelectExit = TRUE;
				
				g_RockClient.Send_RpCsPCLogout( 1 );	/// 이러면 서버에서 알아서 소켓을 끊습니다. 
			}
		}
		Sleep( 1000 ); /// 1초간 기다렸다가 클라언트도 죽이자. 
#endif
		SendMessage( nRui->Desktop.hWnd, WM_CLOSE, NULL, 1 );
	}

}

void CSystemMenuWnd::Open(BOOL bIsNormal)
{
	if(bIsNormal)
	{
		m_pGoOutBtn->SetVisible(false);
		m_pLogOffBtn->SetWndRepositionR(2, 34);
		m_pGameExitBtn->SetWndRepositionR(2, 58);
		m_pScreenShotBtn->SetWndRepositionR(2, 82);
		m_pOptionBtn->SetWndRepositionR(2, 106);
		m_CloseBtn->SetWndRepositionR(2, 130);
		
		/*if( n_JapanNetMarble == g_RockClient.m_GameTypeInfo.theFrom || 
			  n_Kr_PublicWeb == g_RockClient.m_GameTypeInfo.theFrom || 
			  n_Kr_GameAngel == g_RockClient.m_GameTypeInfo.theFrom )
		{
			m_pLogOffBtn->SetVisible(false);
			m_pGameExitBtn->SetWndRepositionR(2, 34);
			m_pScreenShotBtn->SetWndRepositionR(2, 58);
			m_pOptionBtn->SetWndRepositionR(2, 82);
			m_CloseBtn->SetWndRepositionR(2, 106);			
		}*/
	}
	else
	{
		m_pGoOutBtn->SetVisible(true);
		m_pGoOutBtn->SetWndRepositionR(2,34);
		m_pLogOffBtn->SetWndRepositionR(2, 58);
		m_pGameExitBtn->SetWndRepositionR(2, 82);
		m_pScreenShotBtn->SetWndRepositionR(2, 106);
		m_pOptionBtn->SetWndRepositionR(2, 130);
		m_CloseBtn->SetWndRepositionR(2, 154);

		/*if( n_JapanNetMarble == g_RockClient.m_GameTypeInfo.theFrom || 
			  n_Kr_PublicWeb == g_RockClient.m_GameTypeInfo.theFrom || 
			  n_Kr_GameAngel == g_RockClient.m_GameTypeInfo.theFrom )
		{
			m_pLogOffBtn->SetVisible(false);
			m_pGameExitBtn->SetWndRepositionR(2, 58);
			m_pScreenShotBtn->SetWndRepositionR(2, 82);
			m_pOptionBtn->SetWndRepositionR(2, 106);
			m_CloseBtn->SetWndRepositionR(2, 130);			
		}*/
	}

	nRui->SendEvent( WID_SystemMenuWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CSystemMenuWnd::Close()
{
	nRui->SendEvent( WID_SystemMenuWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

//-----------------------------------------------------------------------------