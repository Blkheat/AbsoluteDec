// NonPlayerInfoAddWnd.cpp: implementation of the CNonPlayerInfoAddWnd class.
//
//////////////////////////////////////////////////////////////////////

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

#include "NonPlayerInfoAddWnd.h"

#include "NonPlayerInfoWnd.h"
#include "InventoryWndProc.h"

#include "..\\..\\brother.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\Map.h"

#include "UiUtil.h"
#include "PartyInforWnd.h"

#include "ExchangeWndProc.h"
#include "MessageWndProc.h"
#include "MCommunityWnd.h"
#include "ChatMainProc.h"
#include "stringmanager.h"
#include "FrontierMainWndProc.h"
#include "ProposeJoinFrontier.h"



/*
#include "QuickSlotWndProc.h"
#include "MainMenuWndProc.h"
#include "ChatBoxWndProc.h"


#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"
*/

CNonPlayerInfoAddWnd NonPlayerInfoAddWnd;
CNonPlayerInfoAdd2Wnd g_NonPlayerInfoAdd2Wnd;

extern long g_nowTime;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNonPlayerInfoAddWnd::CNonPlayerInfoAddWnd()
{
	IsVisible = false;
}

CNonPlayerInfoAddWnd::~CNonPlayerInfoAddWnd()
{
	
}

//-----------------------------------------------------------------------------
void CNonPlayerInfoAddWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CNonPlayerInfoAddWnd::Composition()
{
	CFrameWnd*		fw;
//	CTextBox*		tb;	
//	CProgressBar*	pb;
	CButton*		btn;
//	CStateButton*	sbtn;
//	CEditBox*		eb;
//	CSpin*			spin;
	//CImageBox*		img;
//	CScrollBar*		sb;
	

	int center_x	= nRui->Desktop.Width / 2;
//	int center_y	= nRui->Desktop.Height / 2;
	
	//-------------------------------------------------------------------------
	//
	// 다른 유저 및 NPC 정보창 
	//
	//-------------------------------------------------------------------------
	SRect infownd;
	infownd.w	= 152;
	infownd.h	= 28;
	infownd.x	= center_x - ( 193 / 2 );		// NonPlayerInfoWnd Width
//	infownd.y	= center_y - ( infownd.h * 2 );

	infownd.x += (193 + 5); //infownd.x + 227; 
	infownd.y = 30; //36 + 2;		// + NonPlayerInfoWnd.Y
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_NonPlayerInfoAddWnd, WID_None, 
		infownd.x, infownd.y, infownd.w, infownd.h, false );
	fw->SetClientImage( TID_MAIN_MENU );
	fw->SetCaption( _RT("NonPlayerInfoAddBtn") );
	fw->SetWndProc( this );
	fw->SetTitle( false );
	fw->SetMove( false );
	fw->SetVisible( false );
	fw->SetZorder( n_zoTop );
	fw->Initialize();

	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NonPlayerInfoAdd_ChatBtn, WID_NonPlayerInfoAddWnd, 
		3, 2, 24, 24, false );
	btn->SetBtnImage( TID_MAIN_WHISP, TID_MAIN_WHISP_OVR, TID_MAIN_WHISP_CLK, TID_None );
	btn->SetHID( HD_MWHISPER );
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NonPlayerInfoAdd_PartyBtn, WID_NonPlayerInfoAddWnd, 
		28, 2, 24, 24,  false );
	btn->SetBtnImage( TID_MAIN_PARTY, TID_MAIN_PARTY_OVR, TID_MAIN_PARTY_CLK, TID_None );
	btn->SetHID( HD_MPARTY );
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NonPlayerInfoAdd_FriendBtn, WID_NonPlayerInfoAddWnd, 
		54, 2, 24, 24,  false );
	btn->SetBtnImage( TID_MAIN_FRIEND, TID_MAIN_FRIEND_OVR, TID_MAIN_FRIEND_CLK, TID_None );
	btn->SetHID( HD_MFRIEND );	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NonPlayerInfoAdd_TradeBtn, WID_NonPlayerInfoAddWnd, 
		79, 2, 24, 24,  false );
	btn->SetBtnImage( TID_MAIN_EXCH, TID_MAIN_EXCH_OVR, TID_MAIN_EXCH_CLK, TID_None );
	btn->SetHID( HD_MEXCHANGE );
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NonPlayerInfoAdd_ChaseBtn, WID_NonPlayerInfoAddWnd, 
		105, 2, 24, 24,  false );
	btn->SetBtnImage( TID_MAIN_FOLLOW, TID_MAIN_FOLLOW_OVR, TID_MAIN_FOLLOW_CLK, TID_None );
	btn->SetHID( HD_MCHASE );
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NonPlayerInfoAdd_AttackBtn, WID_NonPlayerInfoAddWnd, 
		130, 2, 24, 24, false );
	btn->SetBtnImage( TID_MAIN_ATTACK, TID_MAIN_ATTACK_OVR, TID_MAIN_ATTACK_CLK, TID_None );
	btn->SetHID( HD_MATTACK );
	
}

//-----------------------------------------------------------------------------
void CNonPlayerInfoAddWnd::Update()
{


}

//-----------------------------------------------------------------------------
void CNonPlayerInfoAddWnd::Proc( SEventMessage* EMsg )
{
	if( g_Pc.GetPlayer()->theMapType == n_FightZone )
	{
		return;
	}


	switch( EMsg->FocusWnd )
	{	
	// 각각의 컨트롤에 따라서 처리 	
//	case WID_NonPlayerInfo_StatICONImg1:			// 혹시 툴팁 처리???
//	case WID_NonPlayerInfo_StatICONImg2:
//	case WID_NonPlayerInfo_StatICONImg3:
//	case WID_NonPlayerInfo_StatICONImg4:
//	case WID_NonPlayerInfo_StatICONImg5:
//	case WID_NonPlayerInfo_StatICONImg6:
//		break;

	case WID_NonPlayerInfoAdd_ChatBtn:				// 귓속말 
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				g_NonPlayerInfoAdd2Wnd.Close();
				
				if( NonPlayerInfoWnd.thePickedNpcID != -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_CHAT_NOT_YET ) );
				}
				else if( NonPlayerInfoWnd.thePickedUserID != -1 )
				{

#ifdef NEW_PVP_DECO
					if(g_Pc.m_MainPC.char_info.theCitizen != g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.theCitizen )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOT_INVITATION2)); 				
						return;
					}
#endif

					char NameStr[256] = {0,};
					
					TCHAR *pDest = (TCHAR *)g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.thePCName;

					g_ChatMainProc.m_ChatActPrintWnd.SetWhisDest(pDest);
					g_ChatMainProc.m_ChatActPrintWnd.SetChatState(n_WhisperChat);
					g_ChatMainProc.SetOutState(COutState_Act);		
				}		
			}
		}
		break;
	case WID_NonPlayerInfoAdd_PartyBtn:				// 파티 신청 
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				
				if( NonPlayerInfoWnd.thePickedUserID != -1 )
				{
				
#ifdef NEW_PVP_DECO
					if(g_Pc.m_MainPC.char_info.theCitizen != g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.theCitizen )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOT_INVITATION3)); 				
						return;
					}
#endif
				}

				if(g_NonPlayerInfoAdd2Wnd.IsVisible)
				{
					g_NonPlayerInfoAdd2Wnd.Close();
				}
				else
				{
					g_NonPlayerInfoAdd2Wnd.Open();
				}				
			}
		}
		break;
	case WID_NonPlayerInfoAdd_FriendBtn:			// 친구 등록 
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				g_NonPlayerInfoAdd2Wnd.Close();
				
				if( NonPlayerInfoWnd.thePickedNpcID != -1 )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_FRIEND_NPC ) );	
				}
				else if( NonPlayerInfoWnd.thePickedUserID != -1 )
				{

					
#ifdef NEW_PVP_DECO
					
					if(g_Pc.m_MainPC.char_info.theCitizen != g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.theCitizen )
					{
						SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_NOT_INVITATION3)); 				
						return;
					}
#endif
					
					TCHAR *pPcName = (TCHAR *)g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.thePCName;
					
					if(g_MCommunityWnd.m_MFriendTabWnd.FindFriendData(pPcName))
					{
						MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_AREADY_FRIEND ), 1500, NULL);	
					}
					else
					{						
						g_RockClient.Send_CsMsngrMemberInsert(pPcName);
					}
				}
			}
		}
		break;
	case WID_NonPlayerInfoAdd_TradeBtn:				// 교환 
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				g_NonPlayerInfoAdd2Wnd.Close();
				
				if( NonPlayerInfoWnd.thePickedNpcID != -1 )
				{
//					InventoryWnd.FrameWnd->Reposition( 470, 245 );
//					nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
//					g_ExchangeWnd.Open();
//					nRui->SendEvent( WID_Exchange_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );													
//					nRui->SendEvent( WID_Warehouse_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );					
				}
				else if( NonPlayerInfoWnd.thePickedUserID != -1 )
				{	
					
#ifdef NEW_PVP_DECO
					#ifdef NOT_TALK
					if(g_Pc.m_MainPC.char_info.theCitizen != g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.theCitizen )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOT_INVITATION4)); 				
						return;
					}
					#endif
#endif
					
					
					if( !g_ExchangeWnd.m_bIsAccept )
					{						
						g_RockClient.Send_RpCsAskExchange( g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lUnique );
						g_ExchangeWnd.m_ExchangePcIndex = NonPlayerInfoWnd.thePickedUserID;
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_DO_EXCHANGE_ORDER ), g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.thePCName );						
					}

				}				
			}
		}
		break;
	case WID_NonPlayerInfoAdd_ChaseBtn:				// 따라가기  
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				g_NonPlayerInfoAdd2Wnd.Close();
				
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_PREPARE ) );
			}
		}
		break;
	case WID_NonPlayerInfoAdd_AttackBtn:			// 공격 
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				g_NonPlayerInfoAdd2Wnd.Close();
				
				// 부가효과 (강제타겟) 처리 
				if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke))
				{
					return;
				}			

				if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC )
				{
					return;
				}
				else if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
				{
					if( g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING || 
						g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_SPELLING )
					{
					}
					else
					{
						return;
					}
				}
				
				///-- Skill 을 사용할 때 CM_MOVE_ATTACK 금지
				if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY && g_Pc.GetPlayer()->m_curt_event.tbl_Idx == g_Map.m_nPickedNpc ||
					g_Pc.GetPlayer()->m_save_event.type == SM_MOVE_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc ||
					g_Pc.GetPlayer()->m_curt_event.type == SM_BOW_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc || 
					( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL &&
					g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_SITDOWN &&
					g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_KEEPING &&
					g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_STANDUP ) )	// 휴식 스킬 (유지)	
				{
					return;
				}				
				
				if( g_Pc.GetPlayer()->m_curt_event.type == SM_SLIDE_NPC ||				// 크리티컬에 맞아 밀려나거나 죽었을땐 이동 및 공격 불가 
					g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE ||
					g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN ||
					g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_STANDUP ||
					g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_SITDOWN ||
					g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_KEEPING ||
					g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_STANDUP    )
				{
					return;
				}
				
				if( NonPlayerInfoWnd.thePickedNpcID != -1 )
				{
					CWnd* Wnd = NULL;
					char strTemp[128] = {0,};

					// 선택한 NPC 가 신수일경우 공격하지 공격 하지 않음
					if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_State.nHolyState == nHolyState_Holy )
					{
						return;
					}

					// 선택한 NPC 가 펫일경우 공격하지 공격 하지 않음
					if( g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].m_State.nPetState == nPetState_Pet )
					{
						return;
					}

					/// 선택한 NPC가 같은국가의 유닛이면 공격하지 않음
					if(  IsSameNationUnit( (void *)(&g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID]) ) )
					{
						return;
					}

					int nRace = g_Pc.GetPlayer()->m_curt_event.race;
					int nIndex = g_Pc.GetPlayer()->m_curt_event.tbl_Idx;

					int nGapTime = c_PickSameAttack_DelayTime;

					if( nRace != g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].race || nIndex != NonPlayerInfoWnd.thePickedNpcID )
					{
						nGapTime = c_PickOtherAttack_DelayTime;
					}				

					// 콤보 아닌 공격은 짧은시간에 여러번 발생안됨 
					if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( g_RockClient.m_PickAttack_DelayTime , nGapTime ) , > , g_nowTime ) )
					{
						return;
					}
					
					if( g_Pc.GetPlayer()->m_curt_event.type == CM_MOVE_ATTACK &&
						g_Pc.GetPlayer()->m_curt_event.unique == g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lUnique &&
						g_Pc.GetPlayer()->m_curt_event.cell == g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lDestBlock )
					{
						return;
					}	

					EVENT_DATA_INFO pickNPC_event;
					pickNPC_event.type = CM_MOVE_ATTACK;
					pickNPC_event.motion = n_Attack9_Basic0;
					pickNPC_event.tbl_Idx = NonPlayerInfoWnd.thePickedNpcID;
					pickNPC_event.race	 = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].race;
					pickNPC_event.index	 = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].index;
					pickNPC_event.unique = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lUnique;
					pickNPC_event.x		 = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lDestBlock%(g_Map.m_cSize*2);
					pickNPC_event.y		 = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lDestBlock/(g_Map.m_cSize*2);
					pickNPC_event.vPos.x = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._41;
					pickNPC_event.vPos.y = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._42;
					pickNPC_event.vPos.z = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._43;
					pickNPC_event.cell = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lDestBlock; 
					g_Pc.GetPlayer()->PushQueue( pickNPC_event, TRUE );										
				}
				else if( NonPlayerInfoWnd.thePickedUserID != -1 )
				{
					
					int nRace = g_Pc.GetPlayer()->m_curt_event.race;
					int nIndex = g_Pc.GetPlayer()->m_curt_event.tbl_Idx;

					int nGapTime = c_PickSameAttack_DelayTime;

					if( nRace != g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.race || nIndex != NonPlayerInfoWnd.thePickedUserID )
					{
						nGapTime = c_PickOtherAttack_DelayTime;
					}				

					// 콤보 아닌 공격은 짧은시간에 여러번 발생안됨 
					if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( g_RockClient.m_PickAttack_DelayTime , nGapTime ) , > , g_nowTime ) )
					{
						return;
					}					
					
					if( g_Pc.GetPlayer()->m_curt_event.type == CM_MOVE_ATTACK &&
						g_Pc.GetPlayer()->m_curt_event.unique == g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lUnique &&
						g_Pc.GetPlayer()->m_curt_event.cell == g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lDestBlock )
					{
						///--PRINT_DLG_LOG( "현재 이동공격중이라 공격못함");
						return;
					}

					// PVP Zone 이 아닐때는 같은 종족끼리는 전투 불가 
					if( g_Pc.GetPlayer()->theMapType == n_FightZone )
					{
						if( g_Pc.m_MainPC.nFlag == g_Pc.m_PcTable[g_Pc.m_nPickedUser].nFlag )
						{
							return;
						}
					}
					else
					{
						if( !g_RockClient.CanAttackPlayer( &g_Pc.m_Pc[g_Pc.m_nPickedUser] ) )	
						{
							return;
						}
					}

					EVENT_DATA_INFO pickPC_event;
					pickPC_event.type	= CM_MOVE_ATTACK;
					pickPC_event.motion = n_Attack9_Basic0;
					pickPC_event.tbl_Idx = g_Pc.m_nPickedUser;
					pickPC_event.race	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.race;
					pickPC_event.index	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].nIndex;
					pickPC_event.unique	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lUnique;
					pickPC_event.x		 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock%(g_Map.m_cSize*2);
					pickPC_event.y		 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock/(g_Map.m_cSize*2);
					pickPC_event.vPos.x = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._41;
					pickPC_event.vPos.y = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._42;
					pickPC_event.vPos.z = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._43;
					pickPC_event.cell = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock;
					g_Pc.GetPlayer()->PushQueue( pickPC_event, TRUE );

//					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_PREPARE ) );					
				}
			}
		}
		break;
	default:
		break;
	}

	return;
}

//-----------------------------------------------------------------------------

CNonPlayerInfoAdd2Wnd::CNonPlayerInfoAdd2Wnd()
{
	m_pFrameWnd = NULL;
	m_pSPartyBtn = NULL;
	m_pSLeagueBtn = NULL;
	m_pSFrontierBtn = NULL;
	m_pCoIFrontierBtn = NULL;
}

CNonPlayerInfoAdd2Wnd::~CNonPlayerInfoAdd2Wnd()
{

}

void CNonPlayerInfoAdd2Wnd::Init()
{

}

void CNonPlayerInfoAdd2Wnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	
	SRect infownd;
	
#ifdef COLONY_UPDATE
	infownd.w	= 91 + 28;
#else
	infownd.w	= 91;
#endif	
	
	infownd.h	= 28;
	infownd.x	= center_x - ( 193 / 2 ) + (193 + 5) + 28;
	infownd.y = 30 + 28 + 2;

	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
		infownd.x, infownd.y, infownd.w, infownd.h, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_NORMAL_FRAME_L1 );	
	m_pFrameWnd->SetWndTileSizeLR( 4, 4 );
	m_pFrameWnd->SetWndTileSizeTB( 4, 4 );						
	m_pFrameWnd->SetWndProc( this );
    m_pFrameWnd->SetVisible( false );
    m_pFrameWnd->SetMove( false );
	m_pFrameWnd->SetZorder( n_zoTop );
    m_pFrameWnd->Initialize();

	m_pSPartyBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
					m_pFrameWnd->GetID(), 3, 2, 24, 24, false );
	m_pSPartyBtn->SetBtnImage( TID_SMAIN_PARTY, TID_SMAIN_PARTY_OVR, TID_SMAIN_PARTY_CLK, TID_None );
	m_pSPartyBtn->SetHID( HD_SPARTY );

	m_pSLeagueBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
					m_pFrameWnd->GetID(), 33, 2, 24, 24, false );
	m_pSLeagueBtn->SetBtnImage( TID_SMAIN_LEAGUE, TID_SMAIN_LEAGUE_OVR, TID_SMAIN_LEAGUE_CLK, TID_None );
	m_pSLeagueBtn->SetHID( HD_SLEAGUE );

	m_pSFrontierBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
					m_pFrameWnd->GetID(), 60, 2, 28, 24, false );
	m_pSFrontierBtn->SetBtnImage( TID_SMAIN_FRONTIER, TID_SMAIN_FRONTIER_OVR, TID_SMAIN_FRONTIER_CLK, TID_None );
	m_pSFrontierBtn->SetHID( HD_SFRONTIER );

#ifdef COLONY_UPDATE
	///-- 연합 프론티어 초대버튼
	m_pCoIFrontierBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
					m_pFrameWnd->GetID(), 92, 2, 24, 24, false );
	m_pCoIFrontierBtn->SetBtnImage( TID_SMAIN_PARTY, TID_SMAIN_PARTY_OVR, TID_SMAIN_PARTY_CLK, TID_None );
	m_pCoIFrontierBtn->SetHID( HD_SPARTY );
#endif
	
}

void CNonPlayerInfoAdd2Wnd::Update()
{
	if( !NonPlayerInfoAddWnd.IsVisible )
	{
		if( IsVisible )
		{
			Close();
		}
	}
}

void CNonPlayerInfoAdd2Wnd::Proc( SEventMessage* EMsg )
{
	///-- 윈도우 닫기 버튼
	if( EMsg->FocusWnd == m_pSPartyBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( NonPlayerInfoWnd.thePickedNpcID != -1 )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_PARTY_NPC ) );
			}
			else if( NonPlayerInfoWnd.thePickedUserID != -1 )
			{
				TCHAR *pDest = (TCHAR *)g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.thePCName;				
				g_ChatMainProc.m_ChatActPrintWnd.ExcuteInvitePatry(pDest);
			}
		}
	}

	if( EMsg->FocusWnd == m_pSLeagueBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( NonPlayerInfoWnd.thePickedNpcID != -1 )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NO_DEST ) );			
			}
			else if( NonPlayerInfoWnd.thePickedUserID != -1 )
			{
				//신수를 가지고있는지 체크
			/*
				if(_tcscmp(nRui->thePcParam.PCName, nRui->GetPcInvenInfo()->FrontierCaller) != 0 )
				{
					SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ONLY_HOLLCALLER) );		
					return;
				}		
			*/
				
				TCHAR *pDest = (TCHAR *)g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.thePCName;				
				g_ChatMainProc.m_ChatActPrintWnd.ExcuteInvitePatry(pDest, true);
			}	
		}
	}

	if( EMsg->FocusWnd == m_pSFrontierBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( NonPlayerInfoWnd.thePickedNpcID != -1 )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NO_DEST ) );				
			}
			else if( NonPlayerInfoWnd.thePickedUserID != -1 )
			{
				//1차적으로 프론티어에 소속되어있는지
				if( ( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL ) || 
					!( ( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID != 0 ) && 
			  ( ( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master ) || 
#ifdef DECO_RENEWAL_GUILD_MJH
				( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Assist ) ||
				( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major ) )
#else
			  ( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major ) )
#endif // DECO_RENEWAL_GUILD_MJH
					  ) ) 	
				{
					SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );				 
					return;
				}

				//현재 멤버과 꽉찼으면..
				// if(g_FrontierMemberListWnd.m_FrontierMemberInfoList.size() == m_MaxMemberNum)

				if( g_FrontierMainWnd.IsFullFrontierMember() )
				{
					SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERMAXMEMBER ) );
					return;
				}	
#ifdef DECO_RENEWAL_GUILD_MJH //가입제안
				g_ProposeJoinFrontier.OpenProposeWnd();
#else
				g_RockClient.Send_CsMsngrProposeJoinFrontier((DWORD)g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lUnique);			
#endif // DECO_RENEWAL_GUILD_MJH
			}	
		}
	}

#ifdef COLONY_UPDATE
	if( EMsg->FocusWnd == m_pCoIFrontierBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( NonPlayerInfoWnd.thePickedNpcID != -1 )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NO_DEST ) );				
			}
			else if( NonPlayerInfoWnd.thePickedUserID != -1 )
			{
				//1차적으로 프론티어에 소속되어있는지
				if( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL )
				{
					SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );				 
					return;
				}

				if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID == 0 )
				{
					SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );				 
					return;
				}

				if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade != n_FT_Master )
				{
					SystemPrint( n_DarkGreen ,n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );				 
					return;
				}	
				
				if(nRui->GetPcInvenInfo()->FrontierInfo)
				{
					if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
					{
						SystemPrint( n_DarkGreen ,n_SystemMsg, "예비 프론티어 간에는 연합프론티어를 만들수 없습니다. " );		///-- HAN_MSG
						return;
					}
				}
				
				if( g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.theStatus == n_Preliminary_Frontier )
				{
					SystemPrint( n_DarkGreen ,n_SystemMsg, "예비 프론티어 간에는 연합프론티어를 만들수 없습니다. " );		///-- HAN_MSG
					return;
				}

				g_RockClient.Send_CsMsngrReqCombine((DWORD)g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lUnique);
			}		
		}	
	}
#endif	
}

void CNonPlayerInfoAdd2Wnd::Open()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CNonPlayerInfoAdd2Wnd::Close()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}
