///////////////////////////////////////////////////////////////////////////////////
///		File	: RockClientNet.cpp
///		Desc	: CRockClient:: NetworkProc Functions...
///
///		Author	: Cho Hong-Seob
///		Team	: Program - Client Team
///		Date	: 2004-05-18
///
///		Copyright (c) 2004, RockSoft Co., Ltd. ALL RIGHTS RESERVED
///////////////////////////////////////////////////////////////////////////////////

#include "RockPCH.h" 
    
#include	"quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>
#include	"RockClient.h"
#include	"Map.h"
#include	"Pc.h"
#include	"Npc.h"
#include	<Obj\\TextureManager.H>
 
#include    "effect.h"
 
#include	"GTimer.h"
#include	"DelayActList.h"
#include    "MapDataManager.h"
#include    "CLoadingBarThread.h"

#include	"RockInterface\\UiUtil.h"
#include	"RockInterface\\Rui.h"
#include	"RockInterface\\ChatOutput.h"
#include	"RockInterface\\ScrollBar.h"

#include	"RockInterface\\MainMenuWndProc.h"
#include	"RockInterface\\InventoryWndProc.h"
#include	"RockInterface\\StoreWndProc.h"
#include	"RockInterface\\ChatBoxWndProc.h"

#include	"RockInterface\\ServerListWndProc.h"
#include	"RockInterface\\CharSelectWndProc.h"

#include	"RockInterface\\PlayerInfoWnd.h"
#include	"RockInterface\\NonPlayerInfoWnd.h"
#include	"RockInterface\\NpcChatWnd.h"
#include    "RockInterface\\MessageWndProc.h"
#include    "RockInterface\\SkillWndProc.h"
#include    "RockInterface\\LoadingWndProc.h"
#include	"RockInterface\\QuestProc.h"
#include    "RockInterface\\CharStateWndProc.h"
#include    "RockInterface\\SkillLearnProc.h"
#include    "RockInterface\\CSlotDelayProc.h"

#include    "RockInterface\\CPAcceptYesNoWnd.h"
#include	"RockInterface\\UIMsgWnd.h"
#include	"RockInterface\\PartyInforWnd.h"

#include    "RockInterface\\SysMsgWndProc.h"
#include    "bravolibs\effect\CEffectManager.h"
#include    "RockInterface\\CArcherBar.h"
#include    "RockInterface\\ExchangeWndProc.h"
#include    "RockInterface\\WarehouseWndProc.h"
#include	"RockInterface\ItemEnchantWndProc.h"
#include	"RockInterface\ItemEnchantRWndProc.h"
#include	"RockInterface\ItemExtractWndProc.h"
#include	"RockInterface\ItemExtractRWndProc.h"
#include	"RockInterface\ItemRemodelWndProc.h"
#include	"RockInterface\ItemRemodel_FWndProc.h"
#include	"RockInterface\ItemRemodel_SWndProc.h"
#include	"RockInterface\ItemRepairWndProc.h"	
#include	"RockInterface\ItemCreateWndProc.h"
#include	"RockInterface\ItemCreate_FWndProc.h"
#include	"RockInterface\ItemCreate_SWndProc.h"
#include	"RockInterface\MCommunityWnd.h"
#include	"RockInterface\Announcement.h"
#include	"RockInterface\CLogWnd.h"
#include	"RockInterface\ChatMainProc.h"
#include	"RockInterface\SystemPrintWnd.h"
#include	"RockInterface\BattleZoneWndProc.h"
#include	"RockInterface\AliveWnd.h"
#include	"RockInterface\CUserStoreSellWnd.h"
#include	"RockInterface\CUserStoreTitleWnd.h"
#include	"RockInterface\CUserStoreBuyWnd.h"
#include	"RockInterface\CUserStoreSetBuyWnd.h"
#include	"RockInterface\PayItemWndProc.h"
#include	"RockInterface\FrontierCreateWnd.h"
#include	"RockInterface\FrontierMainWndProc.h"
#include	"RockInterface\NonPlayerInfoAddWnd.h"
#include	"RockInterface\MessageBox.h"
#include	"RockInterface\LeagueInfoWnd.h"
#include	"RockInterface\SkillLevelUpWnd.h"
#include	"RockInterface\OptionWndProc.h"

#include "RockInterface\PetMainWndProc.h"
#include "RockInterface\PetInventoryWnd.h"
#include "RockInterface\PetInfoWnd.h"
#include "RockInterface\GuildInventoryWndProc.h"

#include	"RockInterface\EmblemTexManager.h"

#include	<SOUND\\Sound.H>

#include	"profile\profile.h"
#include	"profile\custom_time.h"
#include	"stringmanager.h"
#include	"MapDataManager.h"

#include "Squence\\CThreeWayDisplay.h"
#include "Squence\\CRotDisplay.h"
#include "Squence\\CChangeClass.h"


#include "RockInterface\ItemSelectEldaMixWndProc.h"
#include "RockInterface\ItemSelectPotionWndProc.h"
#include "RockInterface\ItemSkillPotionRWndProc.h"
#include "RockInterface\CDelayBarWnd.h"

#include "RockInterface\\CColRegionListWnd.h"
#include "RockInterface\\CColRegionInfoWnd.h"

#include "RockInterface\\CGuideWnd.h"

#include "RockInterface\\CAcryEnchantWnd.h"
#include "RockInterface\\CAcryEnchantRWnd.h"
#include "RockInterface\\ItemEldaMixAWndProc.h"
#include "RockInterface\\ItemEldaMixRWndProc.h"
#include "HShield.h"
#include "UserTime.h"
#include "ListTime.h"

#include "RockInterface\\CChangeEldaColorWnd.h"
#include "RockInterface\\CChangeEldaColorRWnd.h"

#include "RockInterface\\ItemEldaDustRWndProc.h"
#include "RockInterface\\ItemEldaDust.h"

#include "RockInterface\\CDelayBarWnd.h"

#include "RockInterface\\StrengthStoneWndProc.h"

/// sooree network
HANDLE	g_hTextrueUpdateThread = INVALID_HANDLE_VALUE;
unsigned int __stdcall TextrueUpdateThread( LPVOID pVoid );

HANDLE	g_hLoadingThread = INVALID_HANDLE_VALUE;
extern unsigned int __stdcall LoadingBarThread( void* arg );

extern	TEXTURE_MANAGER		g_TexManager;
extern  Sound_Manager		g_Sound;
extern	long				g_nowTime;

TCHAR g_strMessage[512]= {0,};
TCHAR g_strMessage2[512]={0,};

char	g_FrontierName[RP_MAX_FRONTIER_NAME];
char	g_FrontierPetName[RP_MAX_FRONTIER_PET_NAME];

void	CRockClient::Proc_OnSocketClose()
{
	if( ( GetLoginStep() == nInGamePlay || GetLoginStep() == nCharacterSelectWait ) 
		&& theUserSelectExit == FALSE && !MessageBoxWnd.IsProcessClose() )
	{
		if(GetLoginStep() == nInGamePlay)		
		{
			///-- SEND_DEBUG_LOG("서버에서 강제종료");
			///-- MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_INVALID_DISCONNECT ) , 1000 , NULL, TRUE );
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_INVALID_DISCONNECT ) , MB_TYPE_OK, NULL, TRUE );
		}
		else
		{
			ClientClose( g_RockClient.GetApp() , 1);
		}
	}
	
	if( GetLoginStep() != nNotConnect )
	{
		if( GetLoginStep() == nAccountLogin )
		{
			ServerListWnd.CancelProcess();
		}
		else if(GetLoginStep() == nServerSelectWait)
		{
			ServerListWnd.CancelProcess(true);
		}
		else if( theUserSelectExit != FALSE )
		{
			ClientClose( g_RockClient.GetApp() , 1);
		}
	}
}

// game protocols procedure       
void	CRockClient::Proc_RpScProtocols( void* aMsg )
{
	SRpHeader*	header = ( SRpHeader* ) aMsg; 		
 	 
	switch( header->theProtocol )
	{
	case n_RpScHackShield:
		Proc_RpScHackShield((SRpScHackShield*) aMsg); break; 
	
	case n_RpScGameGuard:
		Proc_RpScGameGuard( (SRpScGameGuard*) aMsg ) ; break; 
	
	case n_RpScAppearPC:
		Proc_RpScAppearPC( ( SRpScAppearPC* ) aMsg );			break;
	
	case n_RpScDisappearPC:
		Proc_RpScDisappearPC( ( SRpScDisappearPC* ) aMsg );		break;
		
	case n_RpScStartMove:
		Proc_RpScStartMove( ( SRpScStartMove* ) aMsg );			break;
		
	case n_RpScSeeStartMove:
		Proc_RpScSeeStartMove( ( SRpScSeeStartMove* ) aMsg );	break;
		
	case n_RpScStop:
		Proc_RpScStop( ( SRpScStop* ) aMsg );					break;
		
	case n_RpScAttack:
		Proc_RpScAttack( ( SRpScAttack* ) aMsg );				break;
		
	case n_RpScAttacked:
		Proc_RpScAttacked( ( SRpScAttacked* ) aMsg );			break;
		
	case n_RpScSeeAttack:
		Proc_RpScSeeAttack( ( SRpScSeeAttack* ) aMsg );			break;
		
	case n_RpScAppearNPC:
		Proc_RpScAppearNPC( ( SRpScAppearNPC* ) aMsg );			break;
		
	case n_RpScDisappearNPC:
		Proc_RpScDisappearNPC( ( SRpScDisappearNPC* ) aMsg );	break;
		
	case n_RpScNPCMove:
		Proc_RpScNPCMove( ( SRpScNPCMove* ) aMsg );				break;
		
	case n_RpScSay://SRpScSay
		Proc_RpScSay( ( SRpScSay* ) aMsg );						break;	
		
	case n_RpScWhisper://SRpScWhisper
		Proc_RpScWhisper( ( SRpScWhisper* ) aMsg );				break;
		
	case n_RpScRecvWhisper://SRpScRecvWhisper
		Proc_RpScRecvWhisper( ( SRpScRecvWhisper* ) aMsg );		break;
		
	case n_RpScAnswerWarp :
		Proc_RpScAnswerWarp( ( SRpScAnswerWarp* ) aMsg );		break;									
		
	case n_RpScSpelling:
		Proc_RpScSpelling( ( SRpScSpelling* ) aMsg );			break;									
		
	case n_RpScSeeSpelling:
		Proc_RpScSeeSpelling( ( SRpScSeeSpelling* ) aMsg );			break;
	
	case n_RpScSkill :
		Proc_RpScSkill( ( SRpScSkill* ) aMsg );		break;									
		
	case n_RpScSeeSkill :// 몹 스킬 발동
		Proc_RpScSeeSkill( ( SRpScSeeSkill* ) aMsg );		break;
		//.........................................................................
		// 스킬 관련 By wxywxy	
		//.........................................................................		
		// 스킬 걸린 유저들에게 보냄
	case n_RpScSeeAuraSkill:
		Proc_RpScSeeAuroraSkill( ( SRpScSeeAuraSkillPtr ) aMsg );	break;
		
		// 스킬 걸린 당사자에게 보냄 ( 플레이어 아니면 무시 )
	case n_RpScKeepupSkillStart:
		Proc_RpScKeepupSkillStart( ( SRpScKeepupSkillStartPtr ) aMsg ); break;
		//.........................................................................		
		//////////// Item Protocol ////////////////////////////////////////////
	case n_RpScPickup:
		Proc_RpScPickup( ( SRpScPickup* ) aMsg );					break;
		
	case n_RpScPickupEld:
		Proc_RpScPickupEld( ( SRpScPickupEld* ) aMsg );				break;
		
	case n_RpScDrop:
		Proc_RpScDrop( ( SRpScDrop* ) aMsg );						break;
		
	case n_RpScDropEld:
		Proc_RpScDropEld( ( SRpScDropEld* ) aMsg );					break;
		
	case n_RpScUse:	
		Proc_RpScUse( ( SRpScUse* ) aMsg );							break;
		
	case n_RpScSeeUse:
		Proc_RpScSeeUse( ( SRpScSeeUse* ) aMsg );					break;
		
	case n_RpScEquip:
		Proc_RpScEquip( ( SRpScEquip* ) aMsg );						break;
		
	case n_RpScChangeEquip:
		Proc_RpScChangeEquip( ( SRpScChangeEquip* ) aMsg );			break;
		
	case n_RpScSeeEquip:
		Proc_RpScSeeEquip( ( SRpScSeeEquip* ) aMsg );				break;
		
	case n_RpScTakeoff:
		Proc_RpScTakeoff( ( SRpScTakeoff* ) aMsg );					break;
		
	case n_RpScSeeTakeoff:
		Proc_RpScSeeTakeoff( ( SRpScSeeTakeoff* ) aMsg );			break;
		
	case n_RpScBuy:	
		Proc_RpScBuy( ( SRpScBuy* ) aMsg );							break;
		
	case n_RpScSell:
		Proc_RpScSell( ( SRpScSell* ) aMsg );						break;
		
	case n_RpScCombine:	
		Proc_RpScCombine( ( SRpScCombine* ) aMsg );					break;
		
	case n_RpScChangeSlot:
		Proc_RpScChangeSlot( ( SRpScChangeSlot* ) aMsg );			break;
		
	case n_RpScLinkQuickSlot:
		Proc_RpScLinkQuickSlot( ( SRpScLinkQuickSlot* ) aMsg );		break;
		
	case n_RpScUnlinkQuickSlot:
		Proc_RpScUnlinkQuickSlot( ( SRpScUnlinkQuickSlot* ) aMsg );	break;
		
	case n_RpScAppearItem:
		Proc_RpScAppearItem( ( SRpScAppearItem* ) aMsg );	break;
		
	case n_RpScDisappearItem:
		Proc_RpScDisappearItem( ( SRpScDisappearItem* ) aMsg );	break;
		
	case n_RpScAlive:
		Proc_RpScAlive( ( SRpScAlive* ) aMsg );	break;
		
	case n_RpScSeeAliveOnSpot:
		Proc_RpScSeeAliveOnSpot( ( SRpScSeeAliveOnSpot* ) aMsg );	break;
		
	case n_RpScGetExp:
		Proc_RpScGetExp( ( SRpScGetExp* ) aMsg );	break;
		
	case n_RpScLevelup:
		Proc_RpScLevelup( ( SRpScLevelup* ) aMsg );	break;
		
	case n_RpScSeeLevelup:
		Proc_RpScSeeLevelup( ( SRpScSeeLevelup* ) aMsg );	break;
		
	case n_RpScGetSkillPoint:
		Proc_RpScGetSkillPoint( ( SRpScGetSkillPoint* ) aMsg );	break;
		
	case n_RpScPoint:  
		Proc_RpScPoint( ( SRpScPoint* ) aMsg );	break;
		
	case n_RpScScriptStart:
		Proc_RpScScriptStart( ( SRpScScriptStart* ) aMsg );	break;
		
	case n_RpScDialogScript:
		Proc_RpScDialogScript( ( SRpScDialogScript* ) aMsg );	break;
		
	case n_RpScAskScript:
		Proc_RpScAskScript( ( SRpScAskScript* ) aMsg );	break;
		
	case n_RpScItemShopInfo:
		Proc_RpScItemShopInfo( ( SRpScItemShopInfo* ) aMsg );	break;
		
	case n_RpScCreateCharacter:			
		Proc_RpScCreateCharacter( ( SRpScCreateCharacter* ) aMsg ); break;
		
	case n_RpScDeleteCharacter:			
		Proc_RpScDeleteCharacter( ( SRpScDeleteCharacter* ) aMsg ); break;
		
	case n_RpScStatus:			
		Proc_RpScStatus( ( SRpScStatus* ) aMsg ); break;
		
	case n_RpScSeeKeepupSkillEnd:			
		Proc_RpScSeeKeepupSkillEnd( ( SRpScSeeKeepupSkillEnd* ) aMsg ); break;
		
	case n_RpScSeeKeepupSkillPreEnd:			
		Proc_RpScSeeKeepupSkillPreEnd( ( SRpScSeeKeepupSkillPreEnd* ) aMsg ); break;
		
	case n_RpScChangeMainClass:
		Proc_RpScChangeMainClass( ( SRpScChangeMainClass* ) aMsg ); break;
		
	case n_RpScChangeSubClass:
		Proc_RpScChangeSubClass( ( SRpScChangeSubClass* ) aMsg ); break;
		
	case n_RpScNPCStat:
		Proc_RpScNPCStat( ( SRpScNPCStat* ) aMsg ); break;
		
	case n_RpScSeeOtherStatus:
		Proc_RpScSeeOtherStatus( ( SRpScSeeOtherStatus* ) aMsg ); break;
		
	case n_RpScRecoveryStat:
		Proc_RpScRecoveryStat( ( SRpScRecoveryStat* ) aMsg ); break;
		
	case n_RpScGetSkill:
		Proc_RpScGetSkill( ( SRpScGetSkill* ) aMsg ); break;
		
	case n_RpScTakeSkill:
		Proc_RpScTakeSkill( ( SRpScTakeSkill* ) aMsg ); break;
		
	case n_RpScStatus2:
		Proc_RpScStatus2( ( SRpScStatus2* ) aMsg ); break;

	case n_RpScChangeAvatarSkin:
		Proc_RpScChangeAvatarSkin( (SRpScChangeAvatarSkin*) aMsg) ; break; 
		
	case n_RpScQuestWindow:
		Proc_RpScQuestWindow( ( SRpScQuestWindow* ) aMsg ); break;
		
	case n_RpScQuestStatus:
		Proc_RpScQuestStatus( ( SRpScQuestStatus* ) aMsg ); break;
		
	case n_RpScGMCmd:
		Proc_RpScGMCmd( ( SRpScGMCmd* ) aMsg ); break;
		
	case n_RpScGMCmdErr:
		Proc_RpScGMCmdErr( ( SRpScGMCmdErr* ) aMsg ); break;
		
	case n_RpScAnnouncement:
		Proc_RpScAnnouncement( ( SRpScAnnouncement* ) aMsg ); break;
		
	case n_RpScPvPAnnouncement:
		Proc_ScPvPAnnouncement( (SRpScPvPAnnouncement* )aMsg); break; 

	case n_RpScUpdateGage:
		Proc_RpScUpdateGage( ( SRpScUpdateGage* ) aMsg ); break;
		
		//.....................................................................................................
		// 아처 작업

		//.....................................................................................................
	case n_RpScArcherReady:
		Proc_RpScArcherReady( ( SRpScArcherReady *) aMsg ); break;
		
	case n_RpScSeeArcherReady:
		Proc_RpScSeeArcherReady( (SRpScSeeArcherReady *)aMsg); break;
		
	case n_RpScQuestCount:
		Proc_RpScQuestCount( (SRpScQuestCount *)aMsg); break;
		
		//--------------------------------------------------------------------//
		////////////////////////// Party Protocol //////////////////////////////
        //--------------------------------------------------------------------//
	case n_RpScPartyAskJoin:
		Proc_RpScPartyAskJoin( (SRpScPartyAskJoin *)aMsg); break;  
		
	case n_RpScPartyAnsJoin:
		Proc_RpScPartyAnsJoin( (SRpScPartyAnsJoin *)aMsg); break;
		
	case n_RpScPartyCreate:
		Proc_RpScPartyCreate( (SRpScPartyCreate *)aMsg );  break;
		
	case n_RpScPartyJoin:
		Proc_RpScPartyJoin( (SRpScPartyJoin *)aMsg );  break;
		
	case n_RpScPartyInfo:
		Proc_RpScPartyInfo( (SRpScPartyInfo *)aMsg );  break;
		
	case n_RpScPartyMemberInfo:
		Proc_RpScPartyMemberInfo( (SRpScPartyMemberInfo *)aMsg );  break;
		
	case n_RpScPartyChangeName:
		Proc_RpScPartyChangeName( (SRpScPartyChangeName *)aMsg );  break;
		
	case n_RpScPartyChangeLeader:
		Proc_RpScPartyChangeLeader( (SRpScPartyChangeLeader *)aMsg );  break;
		
      
	case n_RpScPartyLeave:
		Proc_RpScPartyLeave( (SRpScPartyLeave *)aMsg );    break;
		
	case n_RpScPartyTarget:
		Proc_RpScPartyTarget( (SRpScPartyTarget *)aMsg );  break;
		
	case n_RpScPartyGiveGage:
		Proc_RpScPartyGiveGage( (SRpScPartyGiveGage *)aMsg );  break;
		
	case n_RpScPartyUpdateGage:
		Proc_RpScPartyUpdateGage( (SRpScPartyUpdateGage *) aMsg ); break;
		
	case n_RpScPartyChat:
		Proc_RpScPartyChat( (SRpScPartyChat *)aMsg );    break;
		
	case n_RpScPartyItemMgr:
		Proc_RpScPartyItemMgr( (SRpScPartyItemMgr *)aMsg );   break;
		
	case n_RpScPartyGetItem:
		Proc_RpScPartyGetItem( (SRpScPartyGetItem *)aMsg );   break;
		
	case n_RpScPartyGetEld:
		Proc_RpScPartyGetEld( (SRpScPartyGetEld  *)aMsg );    break;
        
	case n_RpScPartyExpMgr:
		Proc_RpScPartyExpMgr( (SRpScPartyExpMgr *) aMsg );   break;
		
	case n_RpScPartyPosition:
		Proc_RpScPartyPosition( (SRpScPartyPosition *) aMsg );   break;
		
		//.....................................................................................................
		// NPC 선점 관련
		//.....................................................................................................
	case n_RpScNPCPriority:
		Proc_RpScNPCPriority( ( SRpScNPCPriority * ) aMsg ); break;
		
	case n_RpScDirection:
		Proc_RpScDirection( ( SRpScDirection * ) aMsg );	break;
		
		//========================================================================================================
		// Exchange Protocol 
		//========================================================================================================
	case n_RpScAskExchange:
		Proc_RpScAskExchange( ( SRpScAskExchange * ) aMsg );	break;
		
	case n_RpScAnsExchange:
		Proc_RpScAnsExchange( ( SRpScAnsExchange * ) aMsg );	break;
		
	case n_RpScSuggestResult:
		Proc_RpScSuggestResult( ( SRpScSuggestResult * ) aMsg );	break;
		
	case n_RpScSuggestExItem:
		Proc_RpScSuggestExItem( ( SRpScSuggestExItem * ) aMsg );	break;
		
	case n_RpScResultExchange:
		Proc_RpScResultExchange( ( SRpScResultExchange * ) aMsg );	break;
		
		//========================================================================================================
		// Warehouse Protocol 
		//========================================================================================================
	case n_RpScOpenBank:
		Proc_RpScOpenBank( ( SRpScOpenBank * ) aMsg );	break;
		
	case n_RpScDeposit:
		Proc_RpScDeposit( ( SRpScDeposit * ) aMsg );	break;
		
	case n_RpScDraw:
		Proc_RpScDraw( ( SRpScDraw * ) aMsg );	break;
		
	case n_RpScDepositEld:
		Proc_RpScDepositEld( ( SRpScDepositEld * ) aMsg );	break;
		
	case n_RpScDrawEld:
		Proc_RpScDrawEld( ( SRpScDrawEld * ) aMsg );	break;
		
	case n_RpScWHChangeSlot:
		Proc_RpScWHChangeSlot( ( SRpScWHChangeSlot * ) aMsg );	break;

#ifdef DECO_RENEWAL_GUILD_MJH
		//========================================================================================================
		// GuildInventory Protocol 
		//========================================================================================================
	case n_RpScFrontierBankOpen:
		Proc_RpScOpenFrontierBank( ( SRpScFrontierBankOpen * ) aMsg );	break;
		
	case n_RpScDepositFrontierBank:
		Proc_RpScDepositFrontier( ( SRpScDepositFrontierBank * ) aMsg );	break;
		
	case n_RpScDrawFrontierBank:
		Proc_RpScDrawFrontier( ( SRpScDrawFrontierBank * ) aMsg );	break;
						
	case n_RpScChangeSlotFrontierBank:
		Proc_RpScChangeSlotFrontier( ( SRpScChangeSlotFrontierBank * ) aMsg );	break;
#endif //DECO_RENEWAL_GUILD_MJH		

		///-----------------------------------------------------------------------
		///-- Item 개조 
		///-----------------------------------------------------------------------
	case n_RpScEldacraft:///-- Skill 사용 시
		Proc_RpScEldacraft  ( ( SRpScEldacraft * ) aMsg ); break;
		
	case n_RpScSetStone:///-- Game 실행 시
		Proc_RpScSetStone( ( SRpScSetStone * ) aMsg );break;
		
	case n_RpScEldacraftGame:///-- Game 종료 시
		Proc_RpScEldacraftGame( ( SRpScEldacraftGame * ) aMsg);break;
		
	case n_RpScUpdateMaxStat: 
		Proc_RpScUpdateMaxStat( ( SRpScUpdateMaxStat * ) aMsg);break;
		
		// 날씨 관련
	case n_RpScWeather:
		Proc_RpScWeather( ( SRpScWeatherPtr ) aMsg ); break;
		
	case n_RpScSubLevelup:
		Proc_ScSubLevelup( ( SRpScSubLevelup *) aMsg );break;
		
		///-- 수리 Window Open
	case n_RpScOpenRepairSkill:
		Proc_RpScOpenRepairWnd( ( SRpScOpenRepairSkillPtr ) aMsg);break;
		
		///-- 수리 결과

	case n_RpScReplyRepair:
		Proc_RpScReplyRepair( ( SRpScReplyRepair * )aMsg );	break;
		
		///-- 내구도 변화
	case n_RpScUpdateDurabilityItem:
		Proc_RpScUpdateDurabilityItem( ( SRpScUpdateDurabilityItem *) aMsg); break;
		
		///-- Make Item Minigame 시작
	case n_RpScMakeItem:
		Proc_RpScMakeItem( ( SRpScMakeItem *) aMsg);break;
		
		///-- Make Item Minigame 끝
	case n_RpScMakeItemResult:
		Proc_ScMakeItemResult( ( SRpScMakeItemResult *) aMsg ); break;
		
	case n_RpScSkillUp:
		Proc_ScSkillUp( ( SRpScSkillUp *) aMsg ); break;
		
	case n_RpScAppearSkill:
		Proc_RpScAppearSkill( ( SRpScAppearSkill *) aMsg ); break;
		
	case n_RpScDisappearSkill:
		Proc_RpScDisappearSkill( ( SRpScDisappearSkill *) aMsg ); break;
		
		///-----------------------------------------------------------------------
		///-- 커뮤니티 프로토콜 - 친구 , 차단 , 메신져 채팅 by simwoosung 
		///-----------------------------------------------------------------------
	case n_RpScMsngrError:
		Proc_ScMsngrError( ( SRpScMsngrError *) aMsg ); break;
		
	case n_RpScMsngrMemberState:
		Proc_ScMsngrMemberState( ( SRpScMsngrMemberState *) aMsg ); break;
		
	case n_RpScMsngrMemberInsertAsk:
		Proc_ScMsngrMemberInsertAsk( ( SRpScMsngrMemberInsertAsk *) aMsg ); break;
		
	case n_RpScMsngrMemberDelete:
		Proc_ScMsngrMemberDelete( ( SRpScMsngrMemberDelete *) aMsg ); break;
		
	case n_RpScMsngrMemberBlock:
		Proc_ScMsngrMemberBlock( ( SRpScMsngrMemberBlock *) aMsg ); break;
		
	case n_RpScMsngrChangeOpt:
		Proc_ScMsngrChangeOpt( ( SRpScMsngrChangeOpt *) aMsg ); break;
		
	case n_RpScMsngrSay:
		Proc_ScMsngrSay( ( SRpScMsngrSay *) aMsg ); break;
		
	case n_RpScMsngrChatSystemMsg:
		Proc_ScMsngrChatSystemMsg( ( SRpScMsngrChatSystemMsg *) aMsg ); break;
		
	case n_RpScMsngrChatMemberState:
		Proc_ScMsngrChatMemberState( ( SRpScMsngrChatMemberState *) aMsg ); break;
		
	case n_RpScMsngrChatCreateRoom:
		Proc_ScMsngrChatCreateRoom( ( SRpScMsngrChatCreateRoom *) aMsg ); break;
		
	case n_RpScMsngrChatDestroyRoom:
		Proc_ScMsngrChatDestroyRoom( ( SRpScMsngrChatDestroyRoom *) aMsg ); break;
		
	case n_RpScMsngrChatInviteAsk:
		Proc_ScMsngrChatInviteAsk( ( SRpScMsngrChatInviteAsk *) aMsg ); break;
		
	case n_RpScMsngrChatJoinRoom:
		Proc_ScMsngrChatJoinRoom( ( SRpScMsngrChatJoinRoom *) aMsg ); break;
		
	case n_RpScMsngrChatSay:
		Proc_ScMsngrChatSay( ( SRpScMsngrChatSay *) aMsg ); break;		
		
	case n_RpScDenyMessage:
		Proc_RpScDenyMessage( (SRpScDenyMessage*) aMsg); break; 
		
	case n_RpScUpdateFame:
		Proc_ScUpdateFame( ( SRpScUpdateFame *) aMsg ); break;		
		
	case n_RpScEldUpgrade:
		Proc_ScEldUpgrade( ( SRpScEldUpgrade *) aMsg ); break;
		
		//.........................................................................................................
		// 사교 동작
		//.........................................................................................................
	case n_RpScExpression:
		Proc_ScExpression( ( SRpScExpression * ) aMsg ); break;
		
		
		//.........................................................................................................
		// 콤보 로그 데이터 갱신
		//.........................................................................................................
	case n_RpScSaveLog:
		Proc_ScSaveLog( ( SRpScSaveLog * ) aMsg ); break;
		
	case n_RpScFightRoomList:
		Proc_ScFightRoomList( ( SRpScFightRoomList* ) aMsg );			break;
		
	case n_RpScNewWaitingFighter:
		Proc_ScNewWaitingFighter( ( SRpScNewWaitingFighter* ) aMsg );	break;
		
	case n_RpScDelWaitingFighter:
		Proc_ScDelWaitingFighter( ( SRpScDelWaitingFighter* ) aMsg );	break;
		
	case n_RpScMakeFightRoom:
		Proc_ScMakeFightRoom( ( SRpScMakeFightRoom* ) aMsg );			break;
		
	case n_RpScSeeMakeFightRoom:
		Proc_ScSeeMakeFightRoom( ( SRpScSeeMakeFightRoom* ) aMsg );		break;
		
	case n_RpScSeeDeleteFightRoom:
		Proc_ScSeeDeleteFightRoom( ( SRpScSeeDeleteFightRoom* ) aMsg );	break;
		
	case n_RpScUpdateFightRoom:
		Proc_ScUpdateFightRoom( ( SRpScUpdateFightRoom* ) aMsg );		break;
		
	case n_RpScJoinFightRoom:
		Proc_ScJoinFightRoom( ( SRpScJoinFightRoom* ) aMsg );			break;
		
	case n_RpScJoinedFighter:
		Proc_ScJoinedFighter( ( SRpScJoinedFighter* ) aMsg );			break;
		
	case n_RpScOutFightRoom:
		Proc_ScOutFightRoom( ( SRpScOutFightRoom* ) aMsg );				break;
		
	case n_RpScChangeRoomMaster:
		Proc_ScChangeRoomMaster( ( SRpScChangeRoomMaster* ) aMsg );		break;
		
	case n_RpScChangeFightInfo:
		Proc_ScChangeFightInfo( ( SRpScChangeFightInfo* ) aMsg );		break;
		
	case n_RpScSelectTeam:
		Proc_ScSelectTeam( ( SRpScSelectTeam* ) aMsg );					break;
		
	case n_RpScFightReady:
		Proc_ScFightReady( ( SRpScFightReady* ) aMsg );					break;
		
	case n_RpScFightStart:
		Proc_ScFightStart( ( SRpScFightStart* ) aMsg );					break;
		
	case n_RpScFightGo:
		Proc_ScScFightGo( ( SRpScFightGo* ) aMsg );						break;
		
	case n_RpScFightResult:
		Proc_ScFightResult( ( SRpScFightResult* ) aMsg );				break;
		
	case n_RpScFightBackRoom:
		Proc_ScFightBackRoom( ( SRpScFightBackRoom* ) aMsg );			break;
		
	case n_RpScUpdateContribution:
		Proc_ScUpdateContribution( ( SRpScUpdateContribution* ) aMsg );	break;
		
	case n_RpScErrorGame:
		Proc_ScErrorGame( ( SRpScErrorGame* ) aMsg );					break;
		
	case n_RpNPCInfo:
		Proc_ScNPCInfo( ( RpNPCInfo * ) aMsg );							break;
		
		//......................................................................................................
		// 상점 관련 By wxywxy
		//......................................................................................................
		
		// 상점 열기

	case n_RpScOpenPCShopWindow:
		Proc_ScOpenPCShopWindow((SRpScOpenPCShopWindow *)aMsg );		break; 
		
		// 상점 열기

	case n_RpScOpenPCShop:
		Proc_ScOpenPCShop((SRpScOpenPCShop *)aMsg);						break;
		
		// 다른 사람 상점 열기

	case n_RpScSeeOpenPCShop:
		Proc_ScSeeOpenPCShop((SRpScSeeOpenPCShop *)aMsg);					break;
		
		// 개인 상점 닫기 ( 다른 유저 )
	case n_RpScSeeClosePCShop:
		Proc_ScSeeClosePCShop((SRpScSeeClosePCShop *)aMsg);				break;
		
		// 개인 상점 보기	
	case n_RpScInPCShop:
		Proc_ScInPCShop((SRpScInPCShop *)aMsg);							break;
		// 상점에서 물건 사기

	case n_RpScBuyInPCShop:
		Proc_ScBuyInPCShop((SRpScBuyInPCShop *)aMsg);						break;
		// 자기 상점 물건 변경	
	case n_RpScBuyInMyShop:
		Proc_ScBuyInMyShop((SRpScBuyInMyShop *)aMsg);					break;
		
	case n_RpScFightHistory:
		Proc_ScFightHistory( ( SRpScFightHistory * ) aMsg );					break;
		
		// Commercial Item 관련 
	case n_RpScOpenPayShop:
		Proc_ScOpenPayShop( ( SRpScOpenPayShop * ) aMsg );					break;
		
	case n_RpScGetPayItem:
		Proc_ScGetPayItem( ( SRpScGetPayItem * ) aMsg );					break;
		
	case n_RpScUsePayItem:
		Proc_ScUsePayItem( ( SRpScUsePayItem * ) aMsg );					break;
		
	case n_RpScCheckPayTime:
		Proc_ScCheckPayTime(( SRpScCheckPayTime *)aMsg);					break;

	case n_RpScEndPayItem:
 		Proc_ScEndPayItem( ( SRpScEndPayItem * ) aMsg );					break;
		
	case n_RpScSeeChangeFace:
		Proc_ScSeeChangeFace( ( SRpScSeeChangeFace * ) aMsg );					break;
		
	case n_RpScSeeChangeHair:
		Proc_ScSeeChangeHair( ( SRpScSeeChangeHair * ) aMsg );					break;
		
	case n_RpScSeePayEffect:
		Proc_ScSeePayEffect( ( SRpScSeePayEffect * ) aMsg );					break;
		
	case n_RpScUpdateLuck:
		Proc_ScUpdateLuck( ( SRpScUpdateLuck * ) aMsg );					break;
		
	case n_RpScAnsMakeFrontier:
		Proc_RpScAnsMakeFrontier( ( SRpScAnsMakeFrontier * ) aMsg );						break;
		
	case n_RpScMsngrAuthorizeFrontier:
		Proc_RpScMsngrAuthorizeFrontier( ( SRpScMsngrAuthorizeFrontier * ) aMsg );			break;
		
	case n_RpScMsngrProposeJoinFrontier:
		Proc_RpScMsngrProposeJoinFrontier( ( SRpScMsngrProposeJoinFrontier * ) aMsg );		break;
		
	case n_RpScMsngrAnswerJoinFrontier:
		Proc_RpScMsngrAnswerJoinFrontier( ( SRpScMsngrAnswerJoinFrontier * ) aMsg );		break;
		
	case n_RpScMsngrJoinFrontier:
		Proc_RpScMsngrJoinFrontier( ( SRpScMsngrJoinFrontier * ) aMsg );					break;
		
	case n_RpScMsngrSeeJoinFrontier:
		Proc_RpScMsngrSeeJoinFrontier( ( SRpScMsngrSeeJoinFrontier * ) aMsg );				break;
		
	case n_RpScSeeFrontier:
		Proc_RpScSeeFrontier( ( SRpScSeeFrontier * ) aMsg );						break;
		
	case n_RpScMsngrLeaveFrontier:
		Proc_RpScMsngrLeaveFrontier( ( SRpScMsngrLeaveFrontier * ) aMsg );					break;
		
	case n_RpScMsngrFrontierInfo:
		Proc_RpScMsngrFrontierInfo( ( SRpScMsngrFrontierInfo * ) aMsg );					break;
		
	case n_RpScMsngrFrontierMemberInfo:
		Proc_RpScMsngrFrontierMemberInfo( ( SRpScMsngrFrontierMemberInfo * ) aMsg );		break;
		
	case n_RpScMsngrEmblem:
		Proc_RpScMsngrEmblem( ( SRpScMsngrEmblem * ) aMsg );								break;
#ifdef DECO_RENEWAL_GUILD_MJH
	case n_RpScChangeFrontierNotice:
		Proc_RpScChangeFrontierNotice( (SRpScChangeFrontierNotice * ) aMsg );					break;
#endif // DECO_RENEWAL_GUILD_MJH
		
	case n_RpScPayDues:
		Proc_RpScPayDues( ( SRpScPayDues * ) aMsg );										break;
		
	case n_RpScDivideDues:
		Proc_RpScDivideDues( ( SRpScDivideDues * ) aMsg );						break;
		
	case n_RpScMsngrFrontierSay:
		Proc_RpScMsngrFrontierSay( ( SRpScMsngrFrontierSay * ) aMsg );						break;
		
	case n_RpScMsngrCreateFrontierPet:
		Proc_RpScMsngrCreateFrontierPet( ( SRpScMsngrCreateFrontierPet * ) aMsg );			break;
		
	case n_RpScMsngrDeleteFrontierPet:
		Proc_RpScMsngrDeleteFrontierPet( ( SRpScMsngrDeleteFrontierPet * ) aMsg );			break;
		
	case n_RpScMsngrFrontierPetStatus:
		Proc_RpScMsngrFrontierPetStatus( ( SRpScMsngrFrontierPetStatus * ) aMsg );			break;
		
	case n_RpScMsngrFrontierPetExp:
		Proc_RpScMsngrFrontierPetExp( ( SRpScMsngrFrontierPetExp * ) aMsg );				break;
		
	case n_RpScMsngrFrontierPetLevelup:
		Proc_RpScMsngrFrontierPetLevelup( ( SRpScMsngrFrontierPetLevelup * ) aMsg );		break;
		
	case n_RpScMsngrCallFrontierPet:
		Proc_RpScMsngrCallFrontierPet( ( SRpScMsngrCallFrontierPet * ) aMsg );				break;
		
	case n_RpScMsngrCancelCallFrontierPet:
		Proc_RpScMsngrCancelCallFrontierPet( ( SRpScMsngrCancelCallFrontierPet * ) aMsg );		break;
		
	case n_RpScMsngrUpdateFrontierFame:
		Proc_RpScMsngrUpdateFrontierFame( ( SRpScMsngrUpdateFrontierFame* ) aMsg );		break;
		
	case n_RpScMsngrBuyFrontierItem:
		Proc_RpScMsngrBuyFrontierItem( ( SRpScMsngrBuyFrontierItem* ) aMsg );		break;

	case n_RpScMsngrAddFrontierMember:
		Proc_RpScMsngrAddFrontierMember( (SRpScMsngrAddFrontierMember*) aMsg) ;		break;
	
		///-- 연합 Frontier 관련
	case n_RpScMsngrReqCombine:
		Proc_RpScMsngrReqCombine( (SRpScMsngrReqCombine *) aMsg);		break;
		
	case n_RpScMsngrAnsCombine:
		Proc_RpScMsngrAnsCombine( (SRpScMsngrAnsCombine *) aMsg);		break;
		
	case n_RpScMsngrCombine:
		Proc_RpScMsngrCombine( (SRpScMsngrCombine *) aMsg);		break;
		
	case n_RpScMsngrSeperateF:
		Proc_RpScMsngrSeperateF( (SRpScMsngrSeperateF *) aMsg);	break;
		
	case n_RpScShout:
		Proc_RpScShout( (SRpScShout *) aMsg );	break;
		
		// Pet 관련		
	case n_RpScMakePetName:
		Proc_RpScMakePetName( (SRpScMakePetName *) aMsg );		break;
		
	case n_RpScAddPet:	
		Proc_RpScAddPet( (SRpScAddPet *) aMsg );		break;
		
	case n_RpScPetStatus:
		Proc_RpScPetStatus( (SRpScPetStatus *) aMsg );		break;
		
	case n_RpScPetLevelUP:
		Proc_RpScPetLevelUP( (SRpScPetLevelUP *) aMsg );		break;
		
	case n_RpScPetStat:
		Proc_RpScPetStat( (SRpScPetStat *) aMsg );		break;
		
	case n_RpScPetDie:
		Proc_RpScPetDie( (SRpScPetDie *) aMsg );		break;
		
	case n_RpScDeletePet:
		Proc_RpScDeletePet( (SRpScDeletePet *) aMsg );		break;
		
	case n_RpScMoveItemPCtoPet:
		Proc_RpScMoveItemPCtoPet( (SRpScMoveItemPCtoPet *) aMsg );		break;
		
	case n_RpScMoveItemPettoPC:
		Proc_RpScMoveItemPettoPC( (SRpScMoveItemPettoPC *) aMsg );		break;
		
	case n_RpScUsePetItem:
		Proc_RpScUsePetItem( (SRpScUsePetItem *) aMsg );		break;
		
	case n_RpScCallPet:
		Proc_RpScCallPet( (SRpScCallPet *) aMsg );		break;
		
	case n_RpScCancelCallPet:
		Proc_RpScCancelCallPet( (SRpScCancelCallPet *) aMsg );	break;
		
	case n_RpScAppearPet:
		Proc_RpScAppearPet( (SRpScAppearPet *) aMsg );		break;
		
	case n_RpScDisappearPet:
		Proc_RpScDisappearPet( (SRpScDisappearPet *) aMsg );		break;
		
	case n_RpScPetStartMove:
		Proc_RpScPetStartMove( (SRpScPetStartMove *) aMsg );		break;
		
	case n_RpScPetStop:
		Proc_RpScPetStop( (SRpScPetStop *) aMsg );		break;
		
	case n_RpScPetAttack:
		Proc_RpScPetAttack( (SRpScPetAttack *) aMsg );		break;
		
	case n_RpScPetAlive:
		Proc_RpScPetAlive((SRpScPetAlive *) aMsg);		break;
		
	case n_RpScUsePetPayItem:
		Proc_RpSRpScUsePetPayItem( (SRpScUsePetPayItem*) aMsg); break;
		
	case n_RpScEndPetPayItem:
		Proc_RpSRpScEndPetPayItem((SRpScEndPetPayItem*) aMsg); break;
		
	case n_RpScPetCheckIn:
		Proc_RpSRpScPetCheckIn((SRpScPetCheckIn*) aMsg); break;
		
	case n_RpScPetCheckOut:
		Proc_RpSRpScPetCheckOut((SRpScPetCheckOut*) aMsg); break;
		
	case n_RpScRideOn:
		Proc_RpScRideOn( (SRpScRideOn *) aMsg );		break;
		
	case n_RpScRideOff:
		Proc_RpScRideOff( (SRpScRideOff *) aMsg );		break;
		
	case n_RpScTotemScore:
		Proc_ScTotemScore( (SRpScTotemScore*) aMsg);	break;
		
	case n_RpScAskMove:
		Proc_RpScAskMove( (SRpScAskMove *) aMsg );	 break;
		
	case n_RpScIndunTime:
		Proc_RpScIndunTime((SRpScIndunTime *) aMsg ); break; 
		
	case n_RpScMakeSkillPotion:
		Proc_RpScMakeSkillPotion((SRpScMakeSkillPotion *) aMsg); break;
		
	case n_RpScMixEldaStone:
		Proc_RpScMixEldaStone((SRpScMixEldaStone*) aMsg); break; 
		
		///-- 콜로니 관련
	case n_RpScColonyList:
		Proc_RpScColonyList((SRpScColonyList *) aMsg);	break;
		
	case n_RpScAnsColonyInfo:
		Proc_RpScAnsColonyInfo((SRpScAnsColonyInfo *) aMsg);	break;
		
	case n_RpScDeclareWar:
		Proc_RpScDeclareWar((SRpScDeclareWar *) aMsg);	break;
		
	case n_RpScCancelWar:
		Proc_RpScCancelWar((SRpScCancelWar *) aMsg);  break;
		
	case n_RpScWarAnnounce:
		Proc_RpScWarAnnounce((SRpScWarAnnounce *) aMsg);	 break;
		
	case n_RpScColonyInfo:
		Proc_RpScColonyInfo((SRpScColonyInfo *) aMsg);	break;
		
	case n_RpScPlaytime:
		Proc_SRpScPlaytime((SRpScPlaytime *) aMsg); break;

	case n_RpScDeletedPKAttackUser:
		Proc_RpScDeletedPKAttackUser((SRpScDeletedPKAttackUser *) aMsg); break;
	
	case n_RpScPKAttack:
		Proc_RpScPKAttack((SRpScPKAttack *) aMsg); break;

	case n_RpScUpdatePKPoint:
		Proc_RpScUpdatePKPoint((SRpScUpdatePKPoint *) aMsg); break;
	
	case n_RpScChangePetName:
		Proc_RpScChangePetName((SRpScChangePetName *)aMsg); break;

	case n_RpScChangeName:
		Proc_RpScChangeName((SRpScChangeName *)aMsg); break;
	
	case n_RpScChangeGender:
		Proc_RpScChangeGender((SRpScChangeGender *)aMsg); break;

	case n_RpScChangeFrontierPCName:
		Proc_RpScChangeFrontierPCName((SRpScChangeFrontierPCName *) aMsg);	break;
	
	case n_RpScCheckSoldierTime:
		Proc_RpScCheckSoldierTime((SRpScCheckSoldierTime *) aMsg); break;
		
	case n_RpScNPCAttackedMsg:
		Proc_RpScNPCAttackedMsg((SRpScNPCAttackedMsg *) aMsg); break;

	case n_RpScNPCLevelUP:
		Proc_RpScNPCLevelUP((SRpScNPCLevelUP *) aMsg); break;
	
	case n_RpScEnterPvPField:
		Proc_RpScEnterPvPField((SRpScEnterPvPField *) aMsg); break;
	
	case n_RpScUpdatePvPField:
		Proc_RpScUpdatePvPField((SRpScUpdatePvPField *) aMsg); break;

	case n_RpScMakeAddAccessory:
		Proc_RpScMakeAddAccessory((SRpScMakeAddAccessory *) aMsg); break;
		
	case n_RpScOccupationTime:
		Proc_RpScOccupationTime((SRpScOccupationTime *) aMsg); break;
	
	case n_RpScChangeEldaStone:
		Proc_RpScChangeEldaStone((SRpScChangeEldaStone *) aMsg); break;

	case n_RpScMakeEldastone:
		Proc_RpScMakeEldastone( (SRpScMakeEldastone*) aMsg ); break; 


	case n_RpScMakeStrengthstone:
		Proc_SRpScMakeStrengthstone( (SRpScMakeStrengthstone*) aMsg); break; 

	case n_RpScMakeEssence:
		Proc_SRpScMakeEssence( (SRpScMakeEssence*)aMsg); break; 

	case n_RpScUseStrengthstone:
		Proc_SRpScUseStrengthstone( (SRpScUseStrengthstone *) aMsg ); break; 
#ifdef WIDE_PAY_INVENTORY //유료인벤확장	
	case n_RpScPayInven:
		Proc_SRpScPayInven( (SRpScPayInven *) aMsg ); break;  //유료인벤확장

	case n_RpScPayInvenAdd:
		Proc_SRpScPayInvenAdd( (SRpScPayInvenAdd *) aMsg ); break; 
#endif //WIDE_PAY_INVENTORY

#ifdef TAIWAN30MINEVENT
	case n_RpScTaiwan30minEvent:
		Proc_SRpScTaiwan30minEvent( (SRpScTaiwan30minEvent *) aMsg ); break;  //대만 출석이벤

	case n_RpScTaiwan30minEventStart:
		Proc_RpScTaiwan30minEventStart( (SRpScTaiwan30minEventStart *) aMsg ); break; 
#endif //TAIWAN30MINEVENT
		 
	default:
		{			
			////SEND_DEBUG_LOG( "GAME PACKET ERROR : %x", header->theProtocol );
			RLG1( "PACKET ERROR : %x", header->theProtocol );
			header->theSize = sizeof( SRpHeader );	
		}
		break;
	}
	
	//	static DWORD protocol = 0;
	//	protocol = header->theProtocol;	
}
 
//---------------------------------------------------------------------------------------------
// Framework Related
//---------------------------------------------------------------------------------------------
void	CRockClient::Proc_RpScErrorFramework( SRpScErrorFramework* aMsg )
{
	char buf[1024] = {0,};
	char FirstStr[256]={0,};
	char SecondStr[256]={0,};


#ifdef UNICODE_ROCK
	if(aMsg->theErrorStr[0] == '\0')
	{
		ZeroMemory(aMsg->theErrorStr , sizeof(aMsg->theErrorStr));
	}
#endif

#ifdef GLOBAL_LOGIN_VERSION

	if( aMsg->theReturnURL[0] != '\0' )
	{
		MessageBoxWnd.SetRurnURL(aMsg->theReturnURL);
	}

#endif
   
	switch( aMsg->theErrorCode )
	{
	case n_Already_Login:		/// 이미 로그인 중입니다
		{		
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_RELOGIN ) , 3000 , NULL );
//			RLGS( G_STRING_CHAR( IDS_MSGBOX_RELOGIN ));
		
			break; 
		}	
		break;		
	case n_NotExistAccount:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_MSGBOX_INVALID_ID ) , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
//			RLGS( G_STRING_CHAR( IDS_MSGBOX_INVALID_ID ));
			OnLoginSocketClose();			
			
		}
		break;
	case n_WrongPasswd:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_MSGBOX_INVALID_PASS ) , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
//			RLGS( G_STRING_CHAR( IDS_MSGBOX_INVALID_PASS ));
		}
		break;
	case n_NO_GameServer:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_MSGBOX_NOTSERVER ) , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
//			RLGS( G_STRING_CHAR( IDS_MSGBOX_NOTSERVER ));
		}
		break;
	case n_UserTypeProblem:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_MSGBOX_NOT_TESTER ) , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
//			RLGS( G_STRING_CHAR( IDS_MSGBOX_NOT_TESTER ));
		}
		break;
	case n_NotOpenTime:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_MSGBOX_TEST_TIME ) , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
//			RLGS( G_STRING_CHAR( IDS_MSGBOX_TEST_TIME ));
		}
		break;
	case n_BlockedUser:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_MSGBOX_BLOCK_ACCOUNT ) , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
//			RLGS( G_STRING_CHAR( IDS_MSGBOX_BLOCK_ACCOUNT ));
		}
		break;
	case n_Wrong_Version:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_MSGBOX_INVALID_VERSION ) , RWCHAR(aMsg->theErrorStr) );
			MessageBoxWnd.Open( buf , 3000, NULL );						
//			RLGS( G_STRING_CHAR( IDS_MSGBOX_INVALID_VERSION ));
		}
		break;
	case n_Less_Grade:
		{
			Rsprintf( RWCHAR(buf), _RT("본 게임은 12세이상만 접속할 수 있습니다.") , RWCHAR(aMsg->theErrorStr ));	
			MessageBoxWnd.Open( buf , 3000, NULL );						
			RLGS( buf );
		}
		break;
	case n_WrongGMtype:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_WRONG_GM_TYPE ) , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
		}
		break;
	case n_Joymax_131079:
		{
			// joymax.com 회원가입 여부 확인
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_n_Joymax_131079 ) , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
		}
		break;
	case n_Joymax_131081:
		{
			//데이타 이관  여부 확인 
			Rsprintf( RWCHAR(buf), _RT("Game use is only available to users who agree to the 'Deco Onlie Use Agreement' \
				This can be done by clicking the 'Deco Online Use Agreement' button located at Deco Online homepage ") , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
		}
		break;
	case n_Joymax_131084:
		{
			// 블록상태 확인 //Nation block 
			Rsprintf( RWCHAR(buf), _RT("ErrorMessage:%s") , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
		}
		break;
	case n_Joymax_131080:
		{
			// 블록상태 확인 // IP Block
			Rsprintf( RWCHAR(buf), _RT("ErrorMessage:%s") , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
		}
		break;
	case n_Joymax_131078:
		{
			// 블록상태 확인 // User Block
			Rsprintf( RWCHAR(buf), _RT("ErrorMessage:%s") , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
		}
		break;
	case n_Joymax_131077:
		{
			//암호 오류
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_n_Joymax_131077 ) , RWCHAR(aMsg->theErrorStr ));
			MessageBoxWnd.Open( buf , 3000, NULL );						
		}
		break;
	case n_Runup_PhoneCertify:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_RUNUP_PHONECERTIFY ) );
			MessageBoxWnd.Open( buf , 3000, NULL );					
		}
		break; 
#ifdef TAIWAN_LOGIN_VERSION
	case n_FailCheckAccount:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_FAILCHECKACCOUNT ) );
			MessageBoxWnd.Open( buf , 3000, NULL );	
		}
		break;
	case n_UnalisError_1:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_UNALISEFFOR_1 ) );
			MessageBoxWnd.Open( buf , 3000, NULL );	
		}
		break;
	case n_UnalisError_2:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_UNALISEFFOR_2 ) );
			MessageBoxWnd.Open( buf , 3000, NULL );	
		}
		break;
	case n_UnalisError_3:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_UNALISEFFOR_3 ) );
			MessageBoxWnd.Open( buf , 3000, NULL );	
		}
		break;
	case n_UnalisError_4:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_UNALISEFFOR_4 ) );
			MessageBoxWnd.Open( buf , 3000, NULL );	
			
		}
		break;
	case n_UnalisError_5:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_UNALISEFFOR_5 ) );
			MessageBoxWnd.Open( buf , 3000, NULL );	
		}
		break;
	case n_UnalisError_6:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_UNALISEFFOR_6 ) );
			MessageBoxWnd.Open( buf , 3000, NULL );	
		}
		break;
	case n_UnalisError_8:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_UNALISEFFOR_8 ) );
			MessageBoxWnd.Open( buf , 3000, NULL );	
		}
		break;
	case n_UnalisError_9:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_UNALISEFFOR_9 ) );
			MessageBoxWnd.Open( buf , 3000, NULL );	
		}
		break;
	case n_UnalisError_10:
		{
			Rsprintf( RWCHAR(buf), G_STRING_CHAR( IDS_SYSPR_UNALISEFFOR_10 ) );
			MessageBoxWnd.Open( buf , 3000, NULL );	
		}
		break;
#endif
	default:
		{						
			MessageBoxWnd.Open( aMsg->theErrorStr , 3000, NULL );						
			Rsprintf( RWCHAR(buf), _RT("!S2C_ErrorFramework : code=%d str=%s"), aMsg->theErrorCode, RWCHAR(aMsg->theErrorStr) );
			RLGS( buf );
		}
		break;		
	}
	OnLoginSocketClose();
}


///---------------------------------------------------------------------------
///-- Proc_RpScServerSetList
///---------------------------------------------------------------------------
void	CRockClient::Proc_RpScServerSetList( SRpScServerSetList* aMsg )
{
	CNetworkMgr::Proc_RpScServerSetList( aMsg );	/// 필수!!
	
	SetLoginStep( nServerSelectWait );
	ServerListWnd.Open_ServerList_Wnd( );	///-- 서버 선택창 열기
   
}

///---------------------------------------------------------------------------
///-- Proc_RpScCharacterList
///---------------------------------------------------------------------------
void	CRockClient::Proc_RpScCharacterList( SRpScCharacterList* aMsg )
{

	CNetworkMgr::Proc_RpScCharacterList( aMsg );	/// 필수!!

	SetGameMode( GMODE_SELECT );					/// 클라이언트 게임모드도 변경 
	SetLoginStep( nCharacterSelectWait );
	
	CharSelectWnd.StartUp();						// 캐릭터 선택창 창 열기

}

bool CRockClient::Proc_RpScPrepareLogin( SRpScPrepareLogin* aMsg )
{
	ProfileBegin("< CRockClient::Proc_RpScPrepareLogin >");	
	    
	if( aMsg->theResultCode != n_Success )
	{

		g_Camera.Reset();
		CharSelectWnd.CloseAll();
		g_RockClient.m_IsReConnect = FALSE;
		CharSelectWnd.SetChangeSkyBox(true);
							
		g_Particle.DelEffSetCon( EF_CORE0 , 100 , -1 , -1 );	


		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_FAIL_GAME ), 3000 , NULL );		
		theLoginStep = nNotConnect;
		m_dwGameMode = GMODE_LOGINPAGE;
		
		//로그인창 띄우기

		nRui->SendEvent( WID_Login_PasswordEdit, n_emSetText, (EPARAM)_RT(""), 255, 255, 255 );
		nRui->SendEvent( WID_LoginWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );			
		nRui->SendEvent( WID_Login_UserNameEdit, n_emSetFocus, 0, 0, 0, 0 );
		#ifdef LOGINPAGE_BGIMG_CHANGE		
		nRui->EnableBGround( TRUE );
		#endif

		return false;
	}
	
	SetGameMode( GMODE_WORLDINIT );
	SetLoginStep( nAllPrepareWait );
	
	///-- Dynamic texture 해제
	RLGS( "!-------------------------------------------" );
	RLGS( "!-- Login 과정 Resource 제거" );
	Resource->DestroyDynamic( TID_LOGIN_BACK );	
	Resource->DestroyDynamic( TID_Login_Wnd );
	Resource->DestroyDynamic( TID_PreView_Image );
	Resource->DestroyDynamic( TID_Server_List_Select_Svr );
	Resource->DestroyDynamic( TID_CharSel_Name_Img );
	Resource->DestroyDynamic( TID_Shutter_Up );
	Resource->DestroyDynamic( TID_CharCreate_Dlg );	
	Resource->DestroyDynamic( TID_Help_Image );
	
	/// 모든 준비 끝났다고 서버에 알려줌 
	Send_RpCsPCLogin();
	SetLoginStep( nAllPrepareOK );
	
	ProfileEnd("< CRockClient::Proc_RpScPrepareLogin >");
	ProfileDumpOutputToBuffer();
	
	return true;
}

// 게임서버 접속시 선택한 캐릭터의 정보(속성, 스탯, 아이템 등)
void	CRockClient::Proc_RpScPCLogin( SRpScPCLogin* aMsg )
{
	RLG3( "Map=%d Cell=(%d, %d)", aMsg->thePCInfo.theMap, aMsg->thePCInfo.theCell % g_Pc.m_cSize, aMsg->thePCInfo.theCell / g_Pc.m_cSize );
	
	g_Pc.GetPlayer()->m_Mov.Clear();
	
	if( aMsg->thePCInfo.theMap < c_RockMap_Login_Field || aMsg->thePCInfo.theMap > MAX_MAP_NUM )	/// 일단 하드코딩으로 예외처리 
	{
		char LogStr[256] = {0,};
		Rsprintf( RWCHAR(LogStr), _RT("!잘못된 맵 정보를 가지고 있습니다. MapNum = %d"), theCharacterList[theSelectCharIndex].theData.theMap );		
		RMessageBox(NULL, RWCHAR(LogStr), _RT("Error"), MB_OK);		
		ClientClose( g_RockClient.GetApp() , 1);
	}	
	
	//================================================================================================================
	//================================================================================================================	
	/// 캐릭터 설정 : 자세한 외양 정보는 서버에서 메시지 받아 처리하고 일단 초기화 된 형태로 설정 
	//================================================================================================================
	PCTABLE	pc;
	pc.lUnique = thePCID; 
	pc.nIndex = g_Pc.GetAttrIndex( theCharacterList[theSelectCharIndex].theData.theCitizen, theCharacterList[theSelectCharIndex].theData.theGender );
	pc.lCurMap = aMsg->thePCInfo.theMap;
	
	pc.lBlock = aMsg->thePCInfo.theCell;
	
	if( (int)( pc.lBlock / g_Pc.m_cSize ) >= g_Pc.m_cSize || (int)( pc.lBlock / g_Pc.m_cSize ) <= 0 )
	{
		/// 일단 하드코딩으로 예외처리 
		pc.lBlock = (int)( g_Pc.m_cSize / 3 ) * g_Pc.m_cSize + (int)( g_Pc.m_cSize / 3 );
	}
	
	pc.lDestBlock = pc.lBlock;
	
	g_Pc.SetupSize( g_Map.GetSize() * 2 );		
	
	g_Pc.SetupMainPc( pc );
	g_Pc.SetupTempPc( pc );							/// MapChange()에서 TempPC가 사용되므로 반드시 설정해주어야 함. 
	g_Map.SetupMapIdx( pc.lCurMap );
	
	
	LoadingBarWnd.m_IsMakeImg = true;
	LoadingBarWnd.MakeScreenAfterImage();	
	//================================================================================================================
	// 서버에서 준 정보로 실제 맵 이동 
	//================================================================================================================
	LoadingBarWnd.Open();	
	
	//by simwoosung - 위치변경
	
	// 전송받은 캐릭터 데이터를 저장한다.
	nRui->SetMyPcData( &( aMsg->thePCInfo ), ( aMsg->theMsgHeader.theSize - sizeof( aMsg->theMsgHeader ) ) );
	
#ifdef APPLY_GUIDE
	g_pGuideWnd->RefreshGuideDataList();
#endif

	BeginLoadingBarThread();	
	
	ProfileEnd("Presetting");
	/// 맵 체인지 : 일단 맵 체인지 시켜놓고 서버에서 메시지 받아 추가 캐릭터 정보 업데이트 
	ProfileBegin("MapChange");
	if( MapChange( pc.lCurMap ) == false )
	{
		ProfileEnd("MapChange");
		//쓰레드 강제 종료
		g_CLoadingBarThread.Stop();
		ClientClose( g_RockClient.GetApp() , 1);
		return;
	}	
	ProfileEnd("MapChange");
	
	/// 잡다한 텍스쳐
	ProfileBegin("m_TexTable");
	g_Pc.m_TexTable.Create();	
	ProfileEnd("m_TexTable");
	
	/// 텍스쳐 검색시간 초기화
	ProfileBegin("InitTextrueTime");
	g_TexManager.InitTextrueTime();
	ProfileEnd( "InitTextrueTime" );
	
	ProfileBegin( "InitSortTable" );
	g_Pc_Manager.InitSortTable();
	ProfileEnd( "InitSortTable" );
	
	//================================================================================================================
	
	
	/// [To현배] : theCitizen과 theGender를 확인하여 자기 캐릭 모델 데이터를 찾아 그려야 합니다.  
	/// 보이는 데이터들 : 그림 그리기 위한 데이터 
	g_Pc.m_MainPC.char_info.thePCID = thePCID;
	memcpy( g_Pc.m_MainPC.char_info.thePCName, aMsg->thePCInfo.thePCName, sizeof(g_Pc.m_MainPC.char_info.thePCName) ); 
	g_Pc.m_MainPC.char_info.theCitizen = aMsg->thePCInfo.theCitizen;
	g_Pc.m_MainPC.char_info.theGender = aMsg->thePCInfo.theGender;
	g_Pc.m_MainPC.char_info.theFace = aMsg->thePCInfo.theFace;
	g_Pc.m_MainPC.char_info.theHair = aMsg->thePCInfo.theHair;
	g_Pc.m_MainPC.char_info.race    = c_PcRace_User;
	
	g_Pc.m_MainPC.char_info.theHP = aMsg->thePCInfo.theStat1.theHP;
	g_Pc.m_MainPC.char_info.theMaxHP = aMsg->thePCInfo.theMaxHP;
	g_Pc.m_MainPC.char_info.theRealHP = aMsg->thePCInfo.theStat1.theHP;
	g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
	
	int chrAttrIndex = g_Pc.GetAttrIndex( aMsg->thePCInfo.theCitizen, aMsg->thePCInfo.theGender );
	int chrClassType = nRui->thePcParam.MainClass;
	g_Pc.m_MainPC.char_info.theMainClass = nRui->thePcParam.MainClass;
	
	if( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman )
	{
		g_Pc.m_MainPC.char_info.theSMP = aMsg->thePCInfo.theStat1.theSP;
		g_Pc.m_MainPC.char_info.theMaxSMP = aMsg->thePCInfo.theMaxSP;
	}
	else if( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman )
	{
		g_Pc.m_MainPC.char_info.theSMP = aMsg->thePCInfo.theStat1.theMP;
		g_Pc.m_MainPC.char_info.theMaxSMP = aMsg->thePCInfo.theMaxMP;
	}	
	
	
	if(	(	// 밀레나 남-녀, 직업은 원더러, 소드맨, 나이트, 머시너리, 슬레이어  
		( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || 
		chrClassType == Swordman || chrClassType == Knight || chrClassType == Mercenary || chrClassType == Slayer ) 
		//		) ||
		//		(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
		//		( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
		//		( chrClassType == Shaman || chrClassType == Psyche ) 
		)
		)
	{
		nRui->thePcParam.GageType = n_ComboGage;
	}
	else if(	// 밀레나 남-녀, 직업은 원더러, 아처, 소울 브레이커  
		( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || 
		chrClassType == Archer || chrClassType == Sniper ) 
		) 			
	{
		nRui->thePcParam.GageType = n_ConditionGage;
	}
	else if(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
		( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
		( chrClassType == Stranger ||
		chrClassType == Magician || chrClassType == Prominas || chrClassType == Priest || chrClassType == Holy_Avenger || 
		chrClassType == Shaman || chrClassType == Psyche )  		 
		)
	{
		nRui->thePcParam.GageType = n_ProimaGage;
	}	
	
	g_Pc.GetPlayer()->SetWearItem();
	
	CharStateWnd.SetCitizen( aMsg->thePCInfo.theCitizen );
	
	//	g_SkillWnd.Init();
	
	g_LogWnd.InitComboTextList();
	g_LogWnd.SetComboHotKey( g_Pc.theSkillSystem.theShortCut_SkillCode[2] );
	g_LogWnd.ComboControl();
	
	/// 모든게 끝났으니 게임 모드 변경!! 
	SetLoginStep( nInGamePlay );
	SetGameMode( GMODE_NORMAL );
	
	// 캐릭터 애니메이션 기본셋팅
	g_Pc.GetPlayer()->AnimationPlay( n_Stand_Peace9 );
	g_Pc.GetPlayer()->m_SimTime = g_nowTime;
	
	// 캐릭터가 죽어있는 상태...
	if( aMsg->thePCInfo.theStat1.theHP == 0 )
	{
		g_Pc.GetPlayer()->AnimationPlay( n_Die9 );
		g_Pc.GetPlayer()->m_curt_event.time = SAFE_TIME_ADD( g_nowTime , NPC_DIE_TIME );
		g_Pc.GetPlayer()->m_curt_event.type = SM_STATE_DIE;
		g_Pc.GetPlayer()->m_curt_event.status = SM_STATE_DIE;
		g_Pc.GetPlayer()->m_ani_frame = g_Pc.GetPlayer()->m_end_frame;
		g_Pc.GetPlayer()->m_blend_frame = c_MotionBlendFrame;
		
		for( int i = 0; i < c_Max_WearItemType; ++i )
		{
			g_Pc.GetPlayer()->m_pMaterial[i].Diffuse.a = 0.5f;
		}
		
		g_AliveWnd.Open( n_AnswerWndType_Alive );
	}
	
	// 캐릭터 로딩 끝내고 서버 메세지를 받을 준비 다 되었어요!!
	g_Pc.GetPlayer()->CM__READY__WARP( g_nowTime, n_NormalZone );	
	
	// 공격속도 aMsg->thePCInfo.theStat2.theAttackSpeed;
	g_Pc.GetPlayer()->m_Weapon_DelayTime = aMsg->thePCInfo.theStat2.theAttackSpeed * 100;		
	
	// 이동속도 
	g_Pc.GetPlayer()->m_ani_percent = ( (float)aMsg->thePCInfo.theStat2.theMoveSpeed / c_Pc_MoveSpeed );					
	g_Pc.GetPlayer()->m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.GetPlayer()->m_ani_percent );					
	
	// 캐릭터 선택 후 캐릭터 이미지 만들기(젤 처음 만듬)
	g_Pc_Manager.m_CharImage.Create( g_Pc.GetPlayer()->GetUnique() , 
		g_Pc.m_MainPC.char_info.race , g_lpDevice, false );
	g_Pc_Manager.m_CharImage.Create( g_Pc.GetPlayer()->GetUnique() , 
		g_Pc.m_MainPC.char_info.race , g_lpDevice );
	
	///-- 로딩바가 종료할때까지 기다려 안전적인 처리
	LoadingBarSafeStopAfterProcess();
	
	return;
}

//
void	CRockClient::Proc_RpScAppearPC( SRpScAppearPC* aMsg )
{
	// Illusion 처리. 모션이나 이펙트 후에 생성되야 하므로 딜레이 후 처리. 
	if( aMsg->theMasterID )
	{
		SDelayAppearPC* pDelayAppearPC = NULL;
		pDelayAppearPC = SAFE_NEW( SDelayAppearPC );		
		
		pDelayAppearPC->DelayTime = SAFE_TIME_ADD( g_nowTime , 1250 );
		pDelayAppearPC->AppearPC = *( aMsg );
		g_DelayActList.m_DelayAppearPCList.push_back( pDelayAppearPC );
		return;
	}
	
	RLG4( "ScAppearPC : [%u] %s (%d, %d)", aMsg->theAppearPC.thePCID, aMsg->theAppearPC.thePCName,
		aMsg->theAppearPC.theCell % g_Pc.m_cSize, aMsg->theAppearPC.theCell / g_Pc.m_cSize );
	
	if( g_Pc.m_MainPC.lUnique == aMsg->theAppearPC.thePCID )
	{
		RLGS( " Already Player Loading!!!" );
		return;
	}
	
	NonPlayer* pc = g_Pc.GetUser( (long)aMsg->theAppearPC.thePCID );
	if( pc )
	{
		RLGS( " Already Exist PC : ScAppearPC" );
		return;
	}
	
	/// 보이는 데이터들 : 그림 그리기 위한 데이터 
	PCTABLE addUser;
	addUser.Init();
	addUser.char_info.thePCID = aMsg->theAppearPC.thePCID;
	memcpy( addUser.char_info.thePCName, aMsg->theAppearPC.thePCName, sizeof(addUser.char_info.thePCName) ); 
	addUser.char_info.theLevel = aMsg->theAppearPC.theLevel;
	addUser.char_info.theCitizen = aMsg->theAppearPC.theCitizen;
	addUser.char_info.theGender = aMsg->theAppearPC.theGender;
	addUser.char_info.theFace = aMsg->theAppearPC.theFace;
	addUser.char_info.theHair = aMsg->theAppearPC.theHair;
	addUser.char_info.theMainClass = aMsg->theAppearPC.theMainClass;
	
	// 다른 유저 체력 표시 
	addUser.char_info.theHP = aMsg->theAppearPC.theHP;
	addUser.char_info.theSMP = aMsg->theAppearPC.theSMP;
	
	addUser.char_info.theMaxHP = aMsg->theAppearPC.theMaxHP;
	addUser.char_info.theMaxSMP = aMsg->theAppearPC.theMaxSMP;	
	addUser.char_info.theRealHP = aMsg->theAppearPC.theHP;
	addUser.char_info.theFame  = aMsg->theAppearPC.theFame;
	
	//파티이름 추가 - by simwoosung - 차후 서버에서 넘어오는 정보로 채움

    memset( addUser.char_info.thePartyName, NULL, sizeof(addUser.char_info.thePartyName));
    addUser.char_info.thePartyID = 0; 
	
	int pHeaderSize = sizeof(aMsg->theAppearPC) + sizeof(aMsg->theMsgHeader) + 
					  sizeof(WORD) + sizeof(DWORD) + sizeof(DWORD);

#ifdef HOLY_SKILLICON_MODIFY
	pHeaderSize += sizeof(BYTE);
#endif

	BYTE * pBuf  = (BYTE*)aMsg;
	int p = pHeaderSize; 	
	
#ifdef FRONTIER
	
#ifndef  NEW_PVP_DECO
	
	memcpy( addUser.char_info.theFrontierName, aMsg->theFrontierName, sizeof(aMsg->theFrontierName));
	memcpy( addUser.char_info.theFrontierNick, aMsg->theFrontierNick, sizeof(aMsg->theFrontierNick));
	addUser.char_info.theStatus = aMsg->theFrontierStatus;
	addUser.char_info.theFrontierID = aMsg->theFrontierID;
	
	//프론티어 엠블렌의 버전을 확인한다.
	if(aMsg->theFrontierID)
	{
		if(aMsg->theEmblemVer)
		{
			g_EmblemTexManager.ConFirmEmblemVersion( aMsg->theFrontierID, aMsg->theEmblemVer );
		}
	}
	
#else
	
	if(aMsg->theFrontierFlag == true)
	{
		int EmblemVer = 0 ; 
		//샵이 켜져 있으면 점프해서 읽어온다 .. 
		if(aMsg->theShopFlag == true)
		{
			p += sizeof(aMsg->theShopName);
		}

		memcpy(&addUser.char_info.theFrontierID , &pBuf[p] , sizeof(aMsg->theFrontierID));
		p+= sizeof(DWORD);
		memcpy(addUser.char_info.theFrontierName , &pBuf[p], sizeof(aMsg->theFrontierName));
		p+= sizeof(aMsg->theFrontierName);
		memcpy(&addUser.char_info.theStatus , &pBuf[p] , sizeof(aMsg->theFrontierStatus));
		p+= sizeof(aMsg->theFrontierStatus);
		memcpy(&EmblemVer , &pBuf[p], sizeof(aMsg->theEmblemVer));
		p+= sizeof(aMsg->theEmblemVer);
		memcpy( addUser.char_info.theFrontierNick, &pBuf[p], sizeof(aMsg->theFrontierNick));
		
#ifdef HOLY_SKILLICON_MODIFY
		addUser.char_info.theHolyLevel =  aMsg->theFrontierPetLevel ; 
#endif
		//프론티어 엠블렌의 버전을 확인한다.
		if(addUser.char_info.theFrontierID)
		{
			if(EmblemVer)
			{
				g_EmblemTexManager.ConFirmEmblemVersion( addUser.char_info.theFrontierID, EmblemVer );
			}
		}		
	}
	
#endif // NEW_PVP_DECO			
	
#endif

#ifdef PK_SYSTEM

	addUser.char_info.thePKPoint = aMsg->theAppearPC.thePKPoint;
	if(aMsg->theAppearPC.thePKPoint)
	{
		PRINT_DLG_LOG( "나타난 유저 PK포인트 %u", aMsg->theAppearPC.thePKPoint );
	}

#endif
	
	int i = 0;
	
	for( i = 0; i < RP_MAX_VISIBLE_EQUIP; ++i )
	{
		addUser.char_info.theWearItem[i] = aMsg->theAppearPC.theWearItem[i];
	}
	
	addUser.nIndex = g_Pc.GetAttrIndex( aMsg->theAppearPC.theCitizen, aMsg->theAppearPC.theGender );
	//	addUser.nIndex = rand() % 4;	/// 임시. 데모. 
	addUser.lCurMap = g_Pc.GetMainPC().lCurMap;
	addUser.lBlock = addUser.lDestBlock = aMsg->theAppearPC.theCell;
	
	if( g_Pc.GetPlayer()->theMapType == n_FightZone )
	{
		for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
		{
			if( g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ i ].FighterInfo.thePCID == aMsg->theAppearPC.thePCID )
			{
				addUser.nFlag = g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ i ].FighterTeam;
				break;
			}			
		}
	}	
	
	bool IsRestSkill = false;
	
	for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( aMsg->theAppearPC.theKeepupSkill[i] )
		{
			addUser.char_info.theKeepupSkill[i] = aMsg->theAppearPC.theKeepupSkill[i];
			
			// 호흡 조절이나 명상 스킬 모션 처리 
			if( (addUser.char_info.theKeepupSkill[i] >= c_SkillCode_BreathControl_Min && addUser.char_info.theKeepupSkill[i] <= c_SkillCode_BreathControl_Max) || 
				(addUser.char_info.theKeepupSkill[i] >= c_SkillCode_Meditation_Min && addUser.char_info.theKeepupSkill[i] <= c_SkillCode_Meditation_Max) )
			{
				IsRestSkill = true;
			}
		}
	}
	
	int pc_index = g_Pc.AppearPC( aMsg->theAppearPC.thePCID, addUser );
	if( pc_index == MAX_USER_INTABLE )
		return;
	
	//by simwoosung - 파티 멤버 정보창의 전환
	if(g_PartyInforWnd.IsMemberParty())
	{
		SPARTYMEMBER_INFO *pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theAppearPC.thePCID);
		if(pMemberInfo)
		{
			//멤버의 생김새를 알지 못할 경우

			if(!(pMemberInfo->m_bIsSee))
			{
				if( g_PartyInforWnd.MakePCharImage(aMsg->theAppearPC.thePCID) )
				{
					pMemberInfo->m_bIsSee = TRUE;
				}
			}
			else
			{
				g_PartyInforWnd.ChangePCharImage(aMsg->theAppearPC.thePCID);			
			}
			
			pMemberInfo->m_bIsViewRange = TRUE;
		}
	}
	
	if( IsRestSkill )
	{
		EVENT_DATA_INFO rest_keepup_event;
		rest_keepup_event.type = SM_USE_SKILL;
		rest_keepup_event.status = SM_RESTSKILL_KEEPING;
		rest_keepup_event.motion = n_Skill_RestKepping;			
		g_Pc.m_Pc[pc_index].PushQueue( rest_keepup_event, TRUE );		
		
		g_Particle.m_EffectManager.SetRestSkill( ( Character * ) &g_Pc.m_Pc[pc_index] );
	}
	
	//.................................................................................................
	// 지속성 스킬이 있다면 이펙트 달아주기 By wxywxy
	//.................................................................................................
	for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( aMsg->theAppearPC.theKeepupSkill[i] )
		{
			// 호흡 조절이나 명상 스킬 모션 처리 
			if( (aMsg->theAppearPC.theKeepupSkill[i] >= c_SkillCode_BreathControl_Min && aMsg->theAppearPC.theKeepupSkill[i] <= c_SkillCode_BreathControl_Max) || 
				(aMsg->theAppearPC.theKeepupSkill[i] >= c_SkillCode_Meditation_Min && aMsg->theAppearPC.theKeepupSkill[i] <= c_SkillCode_Meditation_Max) )
			{
			}
			else
			{
				//.........................................................................................................
				// 지속성 스킬이 있다면 이펙트 달아주기

				//.........................................................................................................
				///--JAPAN_BUG_MODIFY
				g_Pc.m_Pc[pc_index].m_curt_event.dKeepSkillCode = aMsg->theAppearPC.theKeepupSkill[i];
				g_Particle.m_EffectManager.CreateKeepupSkill( aMsg->theAppearPC.theKeepupSkill[i] , &g_Pc.m_Pc[pc_index] , NULL , FALSE );
			}
		}
	}

	///--JAPAN_BUG_MODIFY
	g_Pc.m_Pc[pc_index].m_curt_event.dKeepSkillCode = 0;
	
	// 공격속도도 보내주는데...현재 쓰이지 않음...(theAttackSpeed)
	// 다른 유저의 이동속도 (theMoveSpeed);	
	g_Pc.m_Pc[pc_index].m_Weapon_DelayTime = aMsg->theAppearPC.theAttackSpeed * 100;		
	g_Pc.m_Pc[pc_index].m_ani_percent = ( (float)aMsg->theAppearPC.theMoveSpeed / c_Pc_MoveSpeed );					
	g_Pc.m_Pc[pc_index].m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.m_Pc[pc_index].m_ani_percent );
	
	
	//이펙트 테스트 
	/*
	g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) EF_ROCK_SHIELD4 , ( Character * ) &g_Pc.m_Pc[pc_index] , NULL , NULL );
//	g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) EF_ICE_SHIELD4, ( Character * ) &g_Pc.m_Pc[pc_index] , NULL , NULL );
	g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) EF_REST_BREATH, ( Character * ) &g_Pc.m_Pc[pc_index] , NULL , NULL );
	g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) EF_DEPENCE_SHILED_ED , ( Character * ) &g_Pc.m_Pc[pc_index] , NULL , NULL );
	g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) EF_LIVE_MIND1, ( Character * ) &g_Pc.m_Pc[pc_index] , NULL , NULL );
	g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) EF_CHARGE, ( Character * ) &g_Pc.m_Pc[pc_index] , NULL , NULL );
	g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) EF_EGNEETION_ED0, ( Character * ) &g_Pc.m_Pc[pc_index] , NULL , NULL );	
	*/
	
	if( aMsg->theAppearPC.theMoveSpeed > 30000 )
	{
	//	ClientFilelog("ScAppearPC UnUsual theMoveSpeed %u", aMsg->theAppearPC.theMoveSpeed);
	}
	
	if( !IsRestSkill )
	{
		g_Pc.m_Pc[pc_index].m_curt_event.Clear();		
	}
	
	if( aMsg->theShopFlag == TRUE )
	{
		EVENT_DATA_INFO UserStoreEvent;
		
		UserStoreEvent.type   = SM_USE_SKILL;
		UserStoreEvent.status = SM_SELLSKILL_KEEPING;
		UserStoreEvent.motion = n_Skill_RestKepping;			
		
		g_Pc.m_Pc[pc_index].PushQueue( UserStoreEvent, TRUE );			
#ifndef NEW_PVP_DECO
		Rsprintf(RWCHAR(m_strTemp), _RT("%s"), RWCHAR(aMsg->theShopName));						
#else
		p = pHeaderSize ; 
		memcpy(m_strTemp,&pBuf[p], sizeof(aMsg->theShopName));
#endif
		
		SetNonPlayerMessage( g_Pc.m_Pc[pc_index].GetUnique() , m_strTemp );
	}
	
#ifdef RIDING_SYSTEM
	
	if( aMsg->theRidingFlag )
	{
		g_Pc.m_PcTable[ pc_index ].theRidingUID = aMsg->theRidingUID;		
		SetRideOn( pc_index );
	}
	
#endif

#ifdef PK_SYSTEM

	if( g_Pc.GetPlayer()->IsMyAttacker( aMsg->theAppearPC.thePCID ) )
	{
		g_Particle.m_EffectManager.CreateEffect( EF_BEFORE_ATTACKER_MARK , 
								   ( Character * )&g_Pc.m_Pc[ pc_index ] , NULL , NULL );
		///-- 여기서 선제공격자 표시를 넣어준다.
	}

#endif
	
	int table_index = 0;
	
	// 각각 상황 및 타입에 따라서 이펙트 처리...
	switch( aMsg->theAppearType )
	{
	case n_Login:			// 로긴한 유저 처리 
	case n_Warp:			// 맵 이동된 유저 처리 
		{
			// 워프시 이펙트
			g_Particle.SetEffSetCon( 0.0f , EF_WARP_APPEAR , 0 , 3.3f , aMsg->theAppearPC.thePCID , -1 );
			g_Sound.Play( 104 , g_Pc.m_Pc[pc_index].GetPosTM()._41 , g_Pc.m_Pc[pc_index].GetPosTM()._42 , g_Pc.m_Pc[pc_index].GetPosTM()._43 );
		}
		break;
	case n_Alive:			// 다시 부활한 유저 처리 
		{
			// Proc_RpScSeeAliveOnSpot 에서 처리...
		}
		break;
	case n_Die:				// 죽어있는 상태의 유저 처리 
		{
			g_Pc.m_Pc[pc_index].AnimationPlay( n_Die9 );
			g_Pc.m_Pc[pc_index].m_curt_event.time = SAFE_TIME_ADD( g_nowTime , NPC_DIE_TIME );
			g_Pc.m_Pc[pc_index].m_curt_event.type = SM_STATE_DIE;
			g_Pc.m_Pc[pc_index].m_curt_event.status = SM_STATE_DIE;
			g_Pc.m_Pc[pc_index].m_ani_frame = g_Pc.m_Pc[pc_index].m_end_frame;
			g_Pc.m_Pc[pc_index].m_blend_frame = c_MotionBlendFrame;
			g_Pc.m_Pc[pc_index].SetRealDie(TRUE);
			
			for( int i = 0; i < c_Max_WearItemType; ++i )
			{
				g_Pc.m_Pc[pc_index].m_pMaterial[i].Diffuse.a = 0.5f;
			}
		}
		break;
	case n_Appear:			// 시야안에 들어오게 된 유저 처리 
		
		if( ( aMsg->theShopFlag == FALSE ) && !IsRestSkill && ( aMsg->theRidingFlag == FALSE ) )
		{
			SetAction( ( Character *)&g_Pc.m_Pc[pc_index] , aMsg->theExpression , FALSE );
		}
		
		break;
	default:
		break;
	}
	
	return;
}


void	CRockClient::DelayProc_RpScAppearPC( SRpScAppearPC* aMsg )
{
	RLG1( "DelayProc_RpScAppearPC : [%d]", aMsg->theAppearPC.thePCID );
	
	if( g_Pc.m_MainPC.lUnique == aMsg->theAppearPC.thePCID )
	{
		RLGS( " Already Player Loading!!!" );
		return;
	}
	
	NonPlayer* pc = g_Pc.GetUser( (long)aMsg->theAppearPC.thePCID );
	if( pc )
	{
		RLGS( " Already Exist PC : ScAppearPC" );
		return;
	}
	
	/// 보이는 데이터들 : 그림 그리기 위한 데이터 
	PCTABLE addUser;
	addUser.Init();
	addUser.char_info.thePCID = aMsg->theAppearPC.thePCID;
	memcpy( addUser.char_info.thePCName, aMsg->theAppearPC.thePCName, sizeof(addUser.char_info.thePCName) ); 
	addUser.char_info.theLevel = aMsg->theAppearPC.theLevel;
	addUser.char_info.theCitizen = aMsg->theAppearPC.theCitizen;
	addUser.char_info.theGender = aMsg->theAppearPC.theGender;
	addUser.char_info.theFace = aMsg->theAppearPC.theFace;
	addUser.char_info.theHair = aMsg->theAppearPC.theHair;
	addUser.char_info.theMainClass = aMsg->theAppearPC.theMainClass;
	
	// 다른 유저 체력 표시 
	addUser.char_info.theHP = aMsg->theAppearPC.theHP;
	addUser.char_info.theSMP = aMsg->theAppearPC.theSMP;
	addUser.char_info.theMaxHP = aMsg->theAppearPC.theMaxHP;
	addUser.char_info.theMaxSMP = aMsg->theAppearPC.theMaxSMP;	
	addUser.char_info.theRealHP = aMsg->theAppearPC.theHP;
	
	addUser.char_info.theFame = aMsg->theAppearPC.theFame;
	
	
	///-- 마스터 id를 가진 캐릭터를 찾음
	if(aMsg->theMasterID)
	{
		if(aMsg->theMasterID == g_Pc.m_MainPC.lUnique)
		{
			SPcDataParam* PcParam = nRui->GetPcParamInfo();
			memcpy(addUser.char_info.thePartyName , PcParam->PartyName , sizeof(PcParam->PartyName));
			
#ifdef FRONTIER
			
			if(nRui->GetPcInvenInfo()->FrontierInfo)
			{
				memcpy(addUser.char_info.theFrontierName , nRui->GetPcInvenInfo()->FrontierInfo->theFrontierName , RP_MAX_FRONTIER_NAME);
				addUser.char_info.theStatus = nRui->GetPcInvenInfo()->FrontierInfo->theStatus;
			}
			
			if(nRui->GetPcInvenInfo()->FrontierMyInfo)
			{
				memcpy(addUser.char_info.theFrontierNick , nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierNick , RP_MAX_FRONTIER_NICK );
				addUser.char_info.theFrontierID = nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID;
			}	
			
#endif
		}
		else
		{
			PCTABLE *pPcTable = g_Pc.FindPcTable((long)aMsg->theMasterID);
			if(pPcTable)
			{
				memcpy( addUser.char_info.thePartyName,  pPcTable->char_info.thePartyName, 512 );
#ifdef FRONTIER
				
				memcpy( addUser.char_info.theFrontierName, pPcTable->char_info.theFrontierName, RP_MAX_FRONTIER_NAME );
				addUser.char_info.theStatus = pPcTable->char_info.theStatus;
				memcpy( addUser.char_info.theFrontierNick, pPcTable->char_info.theFrontierNick, RP_MAX_FRONTIER_NICK );
				addUser.char_info.theFrontierID  = pPcTable->char_info.theFrontierID;
#ifdef DECO_RENEWAL_GUILD_MJH
				addUser.char_info.theFrontierLevel = pPcTable->char_info.theFrontierLevel;
				addUser.char_info.theCorps = pPcTable->char_info.theCorps;
				addUser.char_info.theFrontierFame = pPcTable->char_info.theFrontierFame;
#endif // DECO_RENEWAL_GUILD_MJH				
#endif			
			}
		}
	}
	
	int i = 0;
	
	if( aMsg->theMasterID )
	{
		int master_index = g_Pc.FindUser( aMsg->theMasterID );
		
		if( g_Pc.m_MainPC.lUnique == aMsg->theMasterID )
		{
			//보이는 기준으로 루프를 돌아라
			for(int i =0 ; i < RP_MAX_EQUIP ; i++  )
			{
				if( nRui->GetPcInvenInfo()->WearItem[i])
				{		
					int iCode = nRui->GetPcInvenInfo()->WearItem[i]->WearTable.theCode;
					int iEldaStone = nRui->GetPcInvenInfo()->WearItem[i]->WearTable.theEldaStone;

					addUser.char_info.theWearItem[i].theCode = iCode;
					addUser.char_info.theWearItem[i].theEldaStone = iEldaStone;
					
					switch(i)
					{
					case n_WearSlot_SKIN_Suit: 
						addUser.char_info.theWearItem[n_WearSlot_CoatArmor].theCode = iCode;
						addUser.char_info.theWearItem[n_WearSlot_CoatArmor].theEldaStone = iEldaStone;
						break; 
					case n_WearSlot_SKIN_Helmet: 
						addUser.char_info.theWearItem[n_WearSlot_Helmet].theCode = iCode;
						addUser.char_info.theWearItem[n_WearSlot_Helmet].theEldaStone = iEldaStone;
						break;
					case n_WearSlot_SKIN_Serve: 
						addUser.char_info.theWearItem[n_WearSlot_Serve].theCode = iCode;
						addUser.char_info.theWearItem[n_WearSlot_Serve].theEldaStone = iEldaStone;
						break; 
					}

				}
				
			}
		}
		else
		{
			
			memcpy(addUser.char_info.theWearItem, 
				g_Pc.m_PcTable[master_index].char_info.theWearItem, sizeof(SRpdCharWearItem)*RP_MAX_EQUIP );
			
		}
	}
	


	addUser.nIndex = g_Pc.GetAttrIndex( aMsg->theAppearPC.theCitizen, aMsg->theAppearPC.theGender );
	//	addUser.nIndex = rand() % 4;	/// 임시. 데모. 
	addUser.lCurMap = g_Pc.GetMainPC().lCurMap;
	addUser.lBlock = addUser.lDestBlock = aMsg->theAppearPC.theCell;
	
	if( g_Pc.GetPlayer()->theMapType == n_FightZone )
	{
		for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
		{
			if( g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ i ].FighterInfo.thePCID == aMsg->theAppearPC.thePCID )
			{
				addUser.nFlag = g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ i ].FighterTeam;
				break;
			}			
		}
	}	
	
	bool IsRestSkill = false;
	
	for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( aMsg->theAppearPC.theKeepupSkill[i] )
		{
			addUser.char_info.theKeepupSkill[i] = aMsg->theAppearPC.theKeepupSkill[i];
			
			// 호흡 조절이나 명상 스킬 모션 처리 
			if( (addUser.char_info.theKeepupSkill[i] >= c_SkillCode_BreathControl_Min && addUser.char_info.theKeepupSkill[i] <= c_SkillCode_BreathControl_Max) || 
				(addUser.char_info.theKeepupSkill[i] >= c_SkillCode_Meditation_Min && addUser.char_info.theKeepupSkill[i] <= c_SkillCode_Meditation_Max) )
			{
				IsRestSkill = true;
			}
		}
	}
		
	int pc_index = g_Pc.AppearPC( aMsg->theAppearPC.thePCID, addUser );
	if( pc_index == MAX_USER_INTABLE )
		return;
	
	// Illusion 처리 
	if( aMsg->theMasterID )
	{
		int idx = 0;
		
		g_Pc.m_Pc[pc_index].m_State.nCharState = nCharState_Illusion;
		
		int master_index = g_Pc.FindUser( aMsg->theMasterID );
		



		if( g_Pc.m_MainPC.lUnique == aMsg->theMasterID )
		{			
	
			g_Pc.GetPlayer()->m_State.nCharState = nCharState_Master;
	
			// 플레이어 자신의 분신을 알려준다. 
			for( idx = 0; idx < c_Max_Illusion_Num; ++idx )
			{
				if( g_Pc.GetPlayer()->m_State.IllusionIndex[idx] == -1 )
				{
					g_Pc.GetPlayer()->m_State.IllusionIndex[idx] = pc_index;
					break;
				}
			}			
		}
		else
		{
			if( master_index != MAX_USER_INTABLE )
			{


				g_Pc.m_Pc[master_index].m_State.nCharState = nCharState_Master;
				// 마스터에게 자신의 분신을 알려준다. 
				for( idx = 0; idx < c_Max_Illusion_Num; ++idx )
				{
					if( g_Pc.m_Pc[master_index].m_State.IllusionIndex[idx] == -1 )
					{
						g_Pc.m_Pc[master_index].m_State.IllusionIndex[idx] = pc_index;
						break;
					}
				}
			}
		}
		
		// 분신일 경우에는 마스터의 인덱트를 알려준다. 같은 국가의 유저의 분신은 구별할 수 있게 표현. 
		g_Pc.m_Pc[pc_index].m_State.IllusionIndex[0] = master_index;
		
		// 자신의 주인이 플레이어이거나 주인이 플레이어와 같은 파티원일 경우에만 분신을 구별 할 수 있다. 
		if( ( master_index == MAX_USER_INTABLE ) ||  
			( ::IsMyPartyAndLeagueMember( g_Pc.m_PcTable[master_index].lUnique ) )
			)
		{			
			g_Pc.m_Pc[pc_index].m_State.nCharState = nCharState_Detect_Illusion;
		}
		else
		{
			g_Pc.m_Pc[pc_index].m_State.nCharState = nCharState_Illusion;
		}
		
		if( g_Pc.GetPlayer()->theMapType == n_FightZone )
		{
			for( i = 0; i < c_Max_WaitingRoom_UserNum; ++i )
			{
				if( g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ i ].FighterInfo.thePCID == aMsg->theMasterID )
				{
					g_Pc.m_PcTable[pc_index].nFlag = g_BZ_WaitingRoomWnd.m_WaitingUserInfo[ i ].FighterTeam;
					break;
				}			
			}
		}	
	}
	
	
	//by simwoosung - 파티 멤버 정보창의 전환
	if(g_PartyInforWnd.IsMemberParty())
	{
		SPARTYMEMBER_INFO *pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theAppearPC.thePCID);
		if(pMemberInfo)
		{
			//멤버의 생김새를 알지 못할 경우

			if(!(pMemberInfo->m_bIsSee))
			{
				if( g_PartyInforWnd.MakePCharImage(aMsg->theAppearPC.thePCID) )
				{
					pMemberInfo->m_bIsSee = TRUE;
				}
			}
			else
			{
				g_PartyInforWnd.ChangePCharImage(aMsg->theAppearPC.thePCID);			
			}
			
			pMemberInfo->m_bIsViewRange = TRUE;
		}
	}
	
	if( IsRestSkill )
	{
		EVENT_DATA_INFO rest_keepup_event;
		rest_keepup_event.type = SM_USE_SKILL;
		rest_keepup_event.status = SM_RESTSKILL_KEEPING;
		rest_keepup_event.motion = n_Skill_RestKepping;			
		g_Pc.m_Pc[pc_index].PushQueue( rest_keepup_event, TRUE );		
		
		g_Particle.m_EffectManager.SetRestSkill( ( Character * ) &g_Pc.m_Pc[pc_index] );
	}
	
	//.................................................................................................
	// 지속성 스킬이 있다면 이펙트 달아주기 By wxywxy
	//.................................................................................................
	for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( aMsg->theAppearPC.theKeepupSkill[i] )
		{
			// 호흡 조절이나 명상 스킬 모션 처리 
			if( (aMsg->theAppearPC.theKeepupSkill[i] >= c_SkillCode_BreathControl_Min && aMsg->theAppearPC.theKeepupSkill[i] <= c_SkillCode_BreathControl_Max) || 
				(aMsg->theAppearPC.theKeepupSkill[i] >= c_SkillCode_Meditation_Min && aMsg->theAppearPC.theKeepupSkill[i] <= c_SkillCode_Meditation_Max) )
			{
			}
			else
			{
				//.........................................................................................................
				// 지속성 스킬이 있다면 이펙트 달아주기

				//.........................................................................................................
				///--JAPAN_BUG_MODIFY
				g_Pc.m_Pc[pc_index].m_curt_event.dKeepSkillCode = aMsg->theAppearPC.theKeepupSkill[i];
				g_Particle.m_EffectManager.CreateKeepupSkill( aMsg->theAppearPC.theKeepupSkill[i] , &g_Pc.m_Pc[pc_index] , NULL , FALSE );
			}
		}
	}

	///--JAPAN_BUG_MODIFY
	g_Pc.m_Pc[pc_index].m_curt_event.dKeepSkillCode = 0;
	
	// 공격속도도 보내주는데...현재 쓰이지 않음...(theAttackSpeed)
	// 다른 유저의 이동속도 (theMoveSpeed);	
	g_Pc.m_Pc[pc_index].m_Weapon_DelayTime = aMsg->theAppearPC.theAttackSpeed * 100;		
	g_Pc.m_Pc[pc_index].m_ani_percent = ( (float)aMsg->theAppearPC.theMoveSpeed / c_Pc_MoveSpeed );					
	g_Pc.m_Pc[pc_index].m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.m_Pc[pc_index].m_ani_percent );					
	
	g_Pc.m_Pc[pc_index].m_curt_event.Clear();
	
	int table_index = 0;



	// 각각 상황 및 타입에 따라서 이펙트 처리...
	switch( aMsg->theAppearType )
	{
	case n_Login:			// 로긴한 유저 처리 
	case n_Warp:			// 맵 이동된 유저 처리 
		{
			// 워프시 이펙트
			g_Particle.SetEffSetCon( 0.0f , EF_WARP_APPEAR , 0 , 3.3f , aMsg->theAppearPC.thePCID , -1 );
			g_Sound.Play( 104 , g_Pc.m_Pc[pc_index].GetPosTM()._41 , g_Pc.m_Pc[pc_index].GetPosTM()._42 , g_Pc.m_Pc[pc_index].GetPosTM()._43 );
		}
		break;
	case n_Alive:			// 다시 부활한 유저 처리 
		{
			// Proc_RpScSeeAliveOnSpot 에서 처리...
		}
		break;
	case n_Die:				// 죽어있는 상태의 유저 처리 
		{
			g_Pc.m_Pc[pc_index].AnimationPlay( n_Die9 );
			g_Pc.m_Pc[pc_index].m_curt_event.time = SAFE_TIME_ADD( g_nowTime , NPC_DIE_TIME );
			g_Pc.m_Pc[pc_index].m_curt_event.type = SM_STATE_DIE;
			g_Pc.m_Pc[pc_index].m_curt_event.status = SM_STATE_DIE;
			g_Pc.m_Pc[pc_index].m_ani_frame = g_Pc.m_Pc[pc_index].m_end_frame;
			g_Pc.m_Pc[pc_index].m_blend_frame = c_MotionBlendFrame;
			g_Pc.m_Pc[pc_index].SetRealDie(TRUE);
			
			for( int i = 0; i < c_Max_WearItemType; ++i )
			{
				g_Pc.m_Pc[pc_index].m_pMaterial[i].Diffuse.a = 0.5f;
			}		
		}
		break;
	case n_Appear:			// 시야안에 들어오게 된 유저 처리 
		break;
	default:
		break;
	}
	
	return;
}


void	CRockClient::Proc_RpScDisappearPC( SRpScDisappearPC* aMsg )
{
	
	RLG1( "ScDisappearPC : [%u]", aMsg->theDisappearPCID );
	
	int table_index = g_Pc.FindUser( aMsg->theDisappearPCID );
	if( table_index == MAX_USER_INTABLE )
	{
		return;
	}
	else
	{
		// AppearPC 처리 안되었을때 DisappearPC 가 먼저 오면 지워준다. 
		list< SDelayAppearPC * >::iterator ir = g_DelayActList.m_DelayAppearPCList.begin();			
		
		for( ; ir != g_DelayActList.m_DelayAppearPCList.end(); ++ir )
		{			
			if( ( *ir )->AppearPC.theAppearPC.thePCID == aMsg->theDisappearPCID )
			{
				SDelayAppearPC *pData = (*ir);
				SAFE_DELETE( pData );
				(*ir) = NULL;
				
				g_DelayActList.m_DelayAppearPCList.erase( ir++ );
			}				
		}		
	}
	
	//by simwoosung
	//현재 자신의 캐릭터가 파이트존에 있을경우에
	if(g_Pc.GetPlayer()->theMapType == n_FightZone)		 
	{
		if(g_Pc.GetPlayer()->IsMyPlayerRender)
		{	//자신의 캐릭터가 죽어있을경우

			if(g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE)
			{
				if(table_index == g_Pc.GetPlayer()->m_nViewRefDestIndex)
				{
					PCTABLE	*pPcTable = NULL;
					pPcTable = g_Pc.GetNextPcTable(g_Pc.GetPlayer()->m_nViewRefDestIndex);
					if(pPcTable)
					{
						g_Camera.m_unique = pPcTable->lUnique;
					}
					else
					{
						g_Camera.m_unique = -1;
					}
					g_Camera.Update();
					g_Camera.MovCamera();
					g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
				}
			}
		}
		else
		{
			if(table_index == g_Pc.GetPlayer()->m_nViewRefDestIndex)
			{
				PCTABLE	*pPcTable = NULL;
				pPcTable = g_Pc.GetNextPcTable(g_Pc.GetPlayer()->m_nViewRefDestIndex);
				if(pPcTable)
				{
					g_Camera.m_unique = pPcTable->lUnique;
				}
				else
				{
					pPcTable = g_Pc.GetNextPcTable(g_Pc.GetPlayer()->m_nViewRefDestIndex);
					g_Camera.m_unique = pPcTable->lUnique;
				}
				g_Camera.Update();
				g_Camera.MovCamera();
				g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
			}	
		}
	}	
	
	// 분신 죽거나 사라질 경우 마스터에게 알려준다. 
	if( g_Pc.m_Pc[table_index].m_State.nCharState == nCharState_Illusion || 
		g_Pc.m_Pc[table_index].m_State.nCharState == nCharState_Detect_Illusion )
	{
		int idx = 0;
		int master_index = g_Pc.m_Pc[table_index].m_State.IllusionIndex[0];
		
		if( master_index == MAX_USER_INTABLE )
		{		
			g_Pc.GetPlayer()->m_State.nCharState = nCharState_Normal;
			
			for( idx = 0; idx < c_Max_Illusion_Num; ++idx )
			{
				if( g_Pc.GetPlayer()->m_State.IllusionIndex[idx] == table_index )
				{
					g_Pc.GetPlayer()->m_State.IllusionIndex[idx] = -1;
					break;
				}
			}
		}
		else
		{
			g_Pc.m_Pc[master_index].m_State.nCharState = nCharState_Normal;
			
			for( idx = 0; idx < c_Max_Illusion_Num; ++idx )
			{
				if( g_Pc.m_Pc[master_index].m_State.IllusionIndex[idx] == table_index )
				{
					g_Pc.m_Pc[master_index].m_State.IllusionIndex[idx] = -1;
					break;
				}
			}
		}		
	}
	
	D3DXVECTOR3 vPos = g_Pc.GetPcInnerPos( -1 , aMsg->theDisappearPCID , 0.5f );
	
	switch( aMsg->theDisappearType )
	{
	case n_LogoutnWarp:
		
		// 워프시 이펙트 ( 로그아웃 / 워프만 보여줌 )
		g_Particle.SetEffSetCon( 0.0f , EF_WARP_DISAPPEAR , 100 , 3.1f , -1 , -1 , vPos );
		g_Sound.Play( 105 , g_Pc.m_Pc[ table_index ].GetPosTM()._41 , 
			g_Pc.m_Pc[ table_index ].GetPosTM()._42 ,
			g_Pc.m_Pc[ table_index ].GetPosTM()._43  );
		break;
	}
	
#ifdef RIDING_SYSTEM
	
	if( g_Pc.m_Pc[ table_index ].m_State.bRide )
	{
		SetRideOff( table_index );
	}
	
#endif
	
	g_Map.SetMapData( CLEAR_BLOCK, table_index, g_Pc.m_PcTable[table_index].lDestBlock, true );
	
	g_Pc.DisappearPC( aMsg->theDisappearPCID );
	
	//by simwoosung - 파티 멤버 정보창의 전환
	if(g_PartyInforWnd.IsMemberParty())
	{
		SPARTYMEMBER_INFO *pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theDisappearPCID);
		if(pMemberInfo)
		{
			pMemberInfo->m_bIsViewRange = FALSE;
		}

		pMemberInfo = g_LeagueInfoWnd.FindLeagueMember(aMsg->theDisappearPCID);
		
		if(pMemberInfo)
		{
			pMemberInfo->m_bIsViewRange = FALSE;
		}
	}
	
	if( NonPlayerInfoWnd.thePickedUserID == table_index )
	{
		NonPlayerInfoWnd.thePickedUserID = -1;
		nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		if(g_NonPlayerInfoAdd2Wnd.IsVisible)
		{
			g_NonPlayerInfoAdd2Wnd.Close();
		}
		
		NonPlayerInfoWnd.SetPickCharacter( NULL );
	}			
	
	if(g_Pc.GetPlayer()->theMapType == n_FightZone)
	{
		g_Pc.GetPlayer()->m_curt_event.Clear();
		g_Pc.m_Pc[table_index].m_curt_event.Clear();
	}

	return;
}

void	CRockClient::Proc_RpScStartMove( SRpScStartMove* aMsg )
{
	//	RLG5( "-=***** ScStartMove : result=%d Dest = %d (%u, %u) Speed=%d", aMsg->theResult, aMsg->theDestiCell , aMsg->theDestiCell % g_Pc.m_cSize, aMsg->theDestiCell / g_Pc.m_cSize, aMsg->theMoveSpeed );
	
	if( aMsg->theResult == n_Success )	/// 갈 수 있다! 
	{
		g_Pc.GetPlayer()->m_ani_percent = ( (float)aMsg->theMoveSpeed / c_Pc_MoveSpeed );					
		g_Pc.GetPlayer()->m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.GetPlayer()->m_ani_percent );					
		
		//현재 라이딩 중이라면

		if(g_Pc.GetPlayer()->m_State.bRide)
		{
			//현재 캐릭터의 속도에 따라 라이딩 대상의 모션을 결정			
			g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].m_ani_percent = 
				( (float)aMsg->theMoveSpeed / c_Pc_MoveSpeed );
			g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].m_move_percent = 
				(float)aMsg->theMoveSpeed / ( (float)aMsg->theMoveSpeed * g_Pc.GetPlayer()->m_ani_percent );
		}		
		
		if( g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN || 
			g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING )
		{
			EVENT_DATA_INFO standup_event;
			standup_event.type = SM_USE_SKILL;
			standup_event.status = SM_RESTSKILL_STANDUP;
			standup_event.motion = n_Skill_RestStandup;			
			g_Pc.GetPlayer()->PushQueue( standup_event, TRUE );
			
			if( g_Pc.GetPlayer()->theAttack_Event.type == SM_MOVE_ATTACK )
			{
				g_Pc.GetPlayer()->m_next_event = g_Pc.GetPlayer()->theAttack_Event;
			}
			else
			{
				g_Pc.GetPlayer()->m_next_event.cell = aMsg->theDestiCell;
			}			
		}
		else
		{
			if( g_Pc.GetPlayer()->theAttack_Event.type == SM_MOVE_ATTACK )
			{	
				//				SystemPrint( n_DarkRed, n_SystemMsg, "==SM_MOVE_ATTACK==" );
				g_Pc.GetPlayer()->PushQueue( g_Pc.GetPlayer()->theAttack_Event, TRUE );				
			}
			else
			{
				g_Map.SetMapData( CLEAR_BLOCK, MAX_USER_INTABLE, g_Pc.m_MainPC.lDestBlock, true );
				//			DebugPrint( "\r\n CLEAR_BLOCK g_Pc.m_MainPC.lDestBlock = %u", g_Pc.m_MainPC.lDestBlock );
				
				/// 예약된 위치와 응답이 같은 경우에만 처리 
				if( g_Pc.GetPlayer()->GetInputedDestCell() == aMsg->theDestiCell )
				{
					g_Map.SetupPickedX( aMsg->theDestiCell % g_Pc.m_cSize );
					g_Map.SetupPickedY( aMsg->theDestiCell / g_Pc.m_cSize );
					//RLG1("aMsg->theDestiCell = %d", aMsg->theDestiCell);
					
					//					SystemPrint( n_DarkRed, n_SystemMsg, "==Proc_RpScStartMove==" );
					g_Pc.SetTile();					
				}
				else
				{
					//					RLG1("g_Pc.GetPlayer()->GetInputedDestCell() = %d", 
					//						  g_Pc.GetPlayer()->GetInputedDestCell());
					///					RLG1("aMsg->theDestiCell = %d", aMsg->theDestiCell);				
				}
				
				g_Pc.GetPlayer()->SetInputedDestCell( INVALID_DWORD_VALUE );			
				
				// 이동하면 저장된 이동 공격을 취소 시킨다. 
				g_Pc.GetPlayer()->m_save_event.Clear();
			}
		}
	}
	
	//by simwoosung
	theActiveCsStartMoveFlag = FALSE;	
	theScWatingCumulateTime = 0.0f;	
	
	//	RLGS( "theActiveCsStartMoveFlag = FALSE" );				
	
	SetWaitingSvrMsg( FALSE );
	
	return;
}

void	CRockClient::Proc_RpScSeeStartMove( SRpScSeeStartMove* aMsg )
{
#ifdef _DEBUG
	RLG6( "ScSeeStartMove : [%u] (%d, %d)->(%d, %d) Speed=%d", aMsg->thePCID, 
		aMsg->theStartCell % g_Pc.m_cSize, aMsg->theStartCell / g_Pc.m_cSize, 
		aMsg->theDestiCell % g_Pc.m_cSize, aMsg->theDestiCell / g_Pc.m_cSize, 
		aMsg->theMoveSpeed );
#endif 

	NonPlayer* pc = g_Pc.GetUser( (long)aMsg->thePCID );
	int tbl_index = g_Pc.FindUser( (long)aMsg->thePCID );
	if( tbl_index == MAX_USER_INTABLE )
		return;
	
	if( pc )
	{
		if( pc->m_bIsLoaded == true )
		{
			g_Pc.m_Pc[ tbl_index ].m_ani_percent = ( (float)aMsg->theMoveSpeed / c_Pc_MoveSpeed );					
			g_Pc.m_Pc[ tbl_index ].m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.m_Pc[ tbl_index ].m_ani_percent );					
			
			if( aMsg->theMoveSpeed > 30000 )
			{
//				ClientFilelog("SeeStartMove UnUsual theMoveSpeed %u", aMsg->theMoveSpeed );
			}			
			
			if(g_Pc.m_Pc[ tbl_index ].m_State.bRide)
			{
				g_Map.m_Npc[g_Pc.m_Pc[ tbl_index ].m_State.TransIndex].m_ani_percent = 
					( (float)aMsg->theMoveSpeed / c_Pc_MoveSpeed );
				g_Map.m_Npc[g_Pc.m_Pc[ tbl_index ].m_State.TransIndex].m_move_percent = 
					(float)aMsg->theMoveSpeed / ( (float)aMsg->theMoveSpeed * g_Pc.GetPlayer()->m_ani_percent );
			}			
			
			if( pc->m_curt_event.status == SM_RESTSKILL_SITDOWN || 
				pc->m_curt_event.status == SM_RESTSKILL_KEEPING )
			{
				EVENT_DATA_INFO standup_event;
				standup_event.type = SM_USE_SKILL;
				standup_event.status = SM_RESTSKILL_STANDUP;
				standup_event.motion = n_Skill_RestStandup;			
				pc->PushQueue( standup_event, TRUE );
				pc->m_next_event.cell = aMsg->theDestiCell;		
			}
			else
			{
				g_Map.SetMapData( CLEAR_BLOCK, tbl_index, g_Pc.m_PcTable[tbl_index].lDestBlock, true );
				//			DebugPrint( "\r\n CLEAR_BLOCK g_Pc.m_PcTable[tbl_index].lDestBlock = %u", g_Pc.m_PcTable[tbl_index].lDestBlock);
				
				EVENT_DATA_INFO	data;
				data.x = aMsg->theDestiCell % g_Pc.m_cSize;
				data.y = aMsg->theDestiCell / g_Pc.m_cSize;
				data.type	= SM_MOVE_BROADCAST;
				
				pc->PushQueue( data );
				
				g_Pc.m_PcTable[tbl_index].lDestBlock = aMsg->theDestiCell;
				//			DebugPrint( "\r\n Proc_RpScSeeStartMove data.x = %d, data.y = %d", data.x, data.y );
			}
		}
	}
}

void	CRockClient::Proc_RpScStop( SRpScStop* aMsg )
{
	//	RLG2( "ScStop : PCID = %u, cell = %u", aMsg->thePCID, aMsg->theCell );
	
	if( aMsg->thePCID == g_Pc.m_MainPC.lUnique )
	{	
		if( g_Pc.m_MainPC.lDestBlock != aMsg->theCell )
		{
			//			RLG1( "Proc_RpScStop : aMsg->theAngle = %3.3f", aMsg->theAngle );			
			
			g_Pc.m_MainPC.lDestBlock = aMsg->theCell;
			g_Pc.GetPlayer()->SetDirection( aMsg->theAngle, false );		
			g_Pc.GetPlayer()->SetTilePos( g_Pc.m_MainPC.lDestBlock%(g_Map.m_cSize*2),
				g_Pc.m_MainPC.lDestBlock/(g_Map.m_cSize*2) );		
			g_Camera.Update();
		}		
	}
	else
	{
		int stopIdx = g_Pc.FindUser(aMsg->thePCID);		
		if( stopIdx == MAX_USER_INTABLE )
			return;
		
		if( g_Pc.m_PcTable[stopIdx].lDestBlock != aMsg->theCell )
		{
			g_Pc.m_PcTable[stopIdx].lDestBlock = aMsg->theCell;
			g_Pc.m_Pc[stopIdx].SetDirection( aMsg->theAngle, false );		
			g_Pc.m_Pc[stopIdx].SetTilePos( g_Pc.m_PcTable[stopIdx].lDestBlock%(g_Map.m_cSize*2),
				g_Pc.m_PcTable[stopIdx].lDestBlock/(g_Map.m_cSize*2) );		
		}
	}
	
	
	//	if( aMsg->theStopType == n_Teleport )
	//	{
	// 텔레포트 이동 이펙트 추가 
	//		g_Particle.m_EffectManager.UseItemEffect( g_Pc.GetPlayer()->GetUnique(), 6005 );
	//	}
	
	return;	
}


bool CRockClient::GetSkillProtocolResult( int aResult )
{
/*
enum NSkillError
{
n_NOT_YOUR_SKILL = 2,	// 소유하고 있는 스킬이 아님
n_NOT_SKILL_ITEM  = 3 ,		// 필요아이템 부재

n_NOT_SKILL_CONDITION = 4 ,	// 필요상태 불만족
n_NOT_ENOUGH_GAGE =5 ,		// 프로이마 부족
n_NO_TARGET_USER = 6,		// 공격대상유저가 없는 경우(ID가 잘못되거나, 로그아웃하거나 스킬테이블에 정의된 대상과 다른 경우 )
n_CANNOT_USE_SKILL = 7 ,		// 스킬을 사용할 수 없는 상태 ( delay time 등 )
n_TOO_LONG_DISTANCE = 8,	// 공격대상거리가 너무 먼 상태
n_NOT_PROIMA_ITEM = 9,
n_WRONG_SKILLCODE =10,
n_HAVENOT_KEEPUP_SKILL = 11,
n_WRONG_INFO =12,
n_KEEPUP_SKILL = 13,			// 지속성 스킬 적용중 (같은스킬이 이미 적용중)
n_CANNOT_FIND_TARGET = 14,
n_DIED_TARGET = 15,
n_SHORT_SP = 16,
n_WRONG_WEAPON =-17,			// 요청한 공격 장비가 아님..
n_WARP_TARGET = 18 ,			// 타켓이 이동중.
n_SHORT_MP = 19,
n_POSSESSION= 20,			// 빙의상태에서는 -> 사용불가.
n_TOGGLEMODE = 21,			// 토글 스킬 처리 (스킬 해제)
n_OVER_KEEPUPSKILL = 22,		// 지속스킬 마법은 최대 10개까지 가능 .. 현재 10개이므로 더이상 시작 불가 
n_NOT_ENOUGH_SKILLPOINT= 23,	
n_NOT_EXIST_BASESKILL= 24,	// 스킬업에 필요한 사전 스킬 없음.
n_ALREADY_MASTERSKILL= 25,	// 이미 마스터 스킬인 경우 더이상 스킬렙업을 할 수 없다. 
n_ATTACKTIME =26,			// 은신은 마지막 공격후 30초가 지나야 합니다.
n_NOT_ENOUGH_STAT =27,		// 필요 HP, SP, MP 부족 
n_ERROR_PRESKILL= 28,		// 필요스킬 조건 불만족 
n_NOTEXIST_COMBOSEQ= 29,	// 콤보 시퀀스가 존재하지 않는다 
n_CANNOT_INFIGHTING,		// 전투중에 시전 할 수 없는 스킬 
n_OVER_MAX_POSSESSION,		// 최대 빙의수를 초과하였습니다. 
};
	*/
	
	if( aResult != n_Success )
	{		
		switch( aResult )
		{
		case n_CANNOT_FIND_TARGET:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_DONTFINDTARGET ) );
			}
			break;
		case n_DIED_TARGET:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_DEAD_TARGET ) );				
			}
			break;
		case n_SHORT_SP:
		case n_SHORT_MP:
		case n_NOT_ENOUGH_STAT:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SHORTAGE_ANUM ) );
			}
			break;
		case n_DIE_STATUS:
			{
				//				SystemPrint( n_DarkRed, n_SystemMsg, "공격 가능 상태가 아닙니다" );
			}
			break;
		case n_HAVENOT_PASSIVE_SKILL:
			{
				//				SystemPrint( n_DarkRed, n_SystemMsg, "콤보스킬이 없습니다");
			}
			break;
			//		case n_MAXCOMBO_OVER:
			//			{
			//				SystemPrint( n_DarkRed, n_SystemMsg, "콤보스킬이 없습니다");
			//			}
			//			break;
		case n_Fail :
			{
				//				SystemPrint( n_DarkRed, n_SystemMsg, "최대 콤보가 오버 되었습니다" );
			}
			break;
		case n_NOT_YOUR_SKILL:
			{
				//				SystemPrint( n_DarkRed, n_SystemMsg, "소유하신 스킬이 아닙니다" );
			}
			break;
		case n_NOT_SKILL_ITEM:			
			{	
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_EQUIP_FOR_SKILL ) );
			}
			break;
		case n_NOT_SKILL_CONDITION:
			{
				//				SystemPrint( n_DarkRed, n_SystemMsg, "스킬 사용할 수 있는 수치가 부족합니다" );
			}
			break;
		case n_NOT_ENOUGH_GAGE:			
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_GAUGE_FOR_SKILL ) );
			}
			break;
		case n_NO_TARGET_USER:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INVALID_TARGET ) );
			}
			break;
		case n_CANNOT_USE_SKILL:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SKILL_NOT_YET ) );
			}
			break;
		case n_TOO_LONG_DISTANCE:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SKILL_DISTANCE ) );
			}
			break;
		case n_NOT_PROIMA_ITEM:
		case n_WRONG_SKILLCODE:
			{
				//				SystemPrint( n_DarkRed, n_SystemMsg, "존재하지 않는 스킬입니다" );
			}
			break;
		case n_HAVENOT_KEEPUP_SKILL:
		case n_WRONG_INFO:
			{
				//				SystemPrint( n_DarkRed, n_SystemMsg, "잘못된 스킬 정보입니다" );
			}
			break;
		case n_KEEPUP_SKILL:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SKILL_DUPL ) );
			}
			break;
		case n_POSSESSION:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SKILL_BE_POSSE ) );
			}
			break;	
			//		case n_WRONG_WEAPON: 		// 요청한 공격 장비가 아님..
			//		case n_WARP_TARGET:			// 타켓이 이동중.
			//			break;
		case n_TOGGLEMODE:			// 토글 스킬 처리 (스킬 해제)
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANCEL_SKILL ) );
			}
			break;
			//		case n_OVER_KEEPUPSKILL:	// 지속스킬 마법은 최대 10개까지 가능 .. 현재 10개이므로 더이상 시작 불가 
			//			break;
		case n_ATTACKTIME:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ATTACK_TIME ) );
			}
			break;
		case n_ERROR_PRESKILL:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ERROR_PRESKILL ) );
			}
			break;		
		case n_NOTEXIST_COMBOSEQ:			// 콤보 시퀀스가 존재하지 않는다 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTEXIST_COMBOSEQ ) );
			}
			break;	
		case n_OVER_MAX_POSSESSION:			// 지속 빙의 개수를 초과하였습니다. 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVER_MAX_POSSESSION ) );
			}
			break;	
		case n_EXIST_OBSTACLE:			// 목표물과의 사이에 방해물이 존재합니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OBSTACLE ) );
			}
			break;	
		case n_InWarpSafeTime:	// 맵이동 후 안전보장 중입니다. 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_IN_WARP_SAFETIME) );
			}
			break;
		case n_CANNOT_OVERAP:		// 중복해서 사용할 수 없습니다. 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_CANNOT_OVERLAP));
			}
			break;	
		case n_Knight_Possession_Count:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYS_KNIGHT_POSS_COUNT));
			}
			break;
		default:
			break;
		}
		
		return false;
	}
	
	return true;
}

///----------------------------------------------------------------------------
///-- Player Attack NPC or Monster
///----------------------------------------------------------------------------
///-- Player 공격 Message 2005.03.08 / Lyul
///-- Player가 공격하는 대상이 Monster 일 경우 '맞음 Message' 출력
///-- Player가 공격하는 Monster 가 죽었을 경우 '소멸 Message' 출력

void	CRockClient::Proc_RpScAttack( SRpScAttack* aMsg )
{
	int attackedIdx = 0;	
	
	if( GetSkillProtocolResult( aMsg->theResult ) == false )
	{

		//콤보 공격 실패시 초기화 
		g_Pc.GetPlayer()->theActionSystem.InitComboEnable();
		g_Pc.GetPlayer()->theCombo_State = n_Combo_Skill_Enable;		//콤보완성시 초기화
		nRui->m_Combo2DTimingEffect.SetComboState( STATE_COMBO_NONE );
		
#ifdef ROCKCLIENT_DEV		
		SystemPrint( n_DarkRed, n_SystemMsg, "ScAttack Error Result : %d", aMsg->theResult );
		RLGS("ScAttack Error");
#endif
		
	
		
		// 거리가 멀어서 공격 취소가 되었을 경우 클라이언트에서 알아서 재공격 설정
		if( aMsg->theResult== n_TOO_LONG_DISTANCE )
		{
			EVENT_DATA_INFO attack_event;
			attack_event.type	= CM_MOVE_ATTACK;
			attack_event.motion	= n_Attack9_Basic0;
			
			if( aMsg->theAttackedType == n_PC )
			{
				attackedIdx = g_Pc.FindUser(aMsg->theAttackedID);
				
				if( attackedIdx == MAX_USER_INTABLE )
				{
					return;
				}
				
				attack_event.tbl_Idx	= attackedIdx;
				attack_event.race		= g_Pc.m_PcTable[ attackedIdx ].char_info.race;
				attack_event.index		= g_Pc.m_PcTable[ attackedIdx ].nIndex;
				attack_event.unique		= g_Pc.m_PcTable[ attackedIdx ].lUnique;
				attack_event.x			= g_Pc.m_PcTable[ attackedIdx ].lDestBlock%(g_Map.m_cSize*2);
				attack_event.y			= g_Pc.m_PcTable[ attackedIdx ].lDestBlock/(g_Map.m_cSize*2);
				attack_event.vPos.x		= g_Pc.m_Pc[ attackedIdx ].GetPosTM()._41;
				attack_event.vPos.y		= g_Pc.m_Pc[ attackedIdx ].GetPosTM()._42;
				attack_event.vPos.z		= g_Pc.m_Pc[ attackedIdx ].GetPosTM()._43;
				attack_event.cell		= g_Pc.m_PcTable[ attackedIdx ].lDestBlock;
				
				///-- 공격시 서버와 클라이언트 거리상 에러 체크값 증가
				attack_event.nADisErrCnt = g_Pc.GetPlayer()->m_save_event.nADisErrCnt;
				attack_event.nADisErrCnt++;
		
				///--PRINT_DLG_LOG( "Move Attack Dis Error Count %d", attack_event.nADisErrCnt );

				g_Pc.GetPlayer()->PushQueue( attack_event );				
			}
			else
			{
				attackedIdx = g_Map.FindNpc(aMsg->theAttackedID);
				
				if( attackedIdx == MAX_NPC_INTABLE )
				{
					return;
				}
				
				attack_event.tbl_Idx	=  attackedIdx ;
				attack_event.race		= g_Map.m_NpcTable[ attackedIdx ].race;
				attack_event.index		= g_Map.m_NpcTable[ attackedIdx ].index;
				attack_event.unique		= g_Map.m_NpcTable[ attackedIdx ].lUnique;
				attack_event.x			= g_Map.m_NpcTable[ attackedIdx ].lDestBlock%(g_Map.m_cSize*2);
				attack_event.y			= g_Map.m_NpcTable[ attackedIdx ].lDestBlock/(g_Map.m_cSize*2);
				attack_event.vPos.x		= g_Map.m_Npc[ attackedIdx ].GetPosTM()._41;
				attack_event.vPos.y		= g_Map.m_Npc[ attackedIdx ].GetPosTM()._42;
				attack_event.vPos.z		= g_Map.m_Npc[ attackedIdx ].GetPosTM()._43;
				attack_event.cell		= g_Map.m_NpcTable[ attackedIdx ].lDestBlock;
				
				attack_event.nADisErrCnt = g_Pc.GetPlayer()->m_save_event.nADisErrCnt;
				attack_event.nADisErrCnt++;

				g_Pc.GetPlayer()->PushQueue( attack_event );				
			}
		}
		else if( aMsg->theResult== n_DIED_TARGET )
		{
			static auto die_ID = 0;
			if( die_ID != aMsg->theAttackedID )
			{					
				die_ID = aMsg->theAttackedID;
			}
			else
			{
				//				SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			}
		}		
		else 
		{
			int b = 0 ;

		}
		return;
	}

	///-- 공격시 서버와 클라이언트 거리상 에러 체크값 0로 초기화
	g_Pc.GetPlayer()->m_save_event.nADisErrCnt = 0;	
	
	if(  aMsg->theResult == n_Success )
	{
		
		///-------------------------------------------------------------------------
		///-- Monster ChattingBox	
		///-- 2005.03.07 / Lyul / 맞았을때
		//	RLGS("-- Monster Chatting Box : Player Attacked Monster!");
		// by dongs 수정 대사가 너무 많이 나오는 관계로 ... 랜덤으로 출력하라 .. 
		float rd = (float)rand()/ (float)RAND_MAX;
		if( rd  > nRui->m_BugMop.fMopAttacked )
		{
			SetMonsterMessage( aMsg->theAttackedID, n_Monster_Attacked );
		}		
		
		///-------------------------------------------------------------------------		
		// 자기 자신의 플레이는 클라이언트에서 모두 판단 처리...서버에 방송만 한다. 
		// 이곳에서의 메세지는 타격 데미지 판단 이펙트 처리 등에만 사용 
		EVENT_DATA_INFO attack_event;
		attack_event.type	= SM_ATTACK_NPC;
		attack_event.motion = aMsg->theMotion;
		
		nRui->thePcParam.Stat1.theMP = aMsg->theSMP;
		nRui->thePcParam.Stat1.theSP = aMsg->theSMP;
		
		//by simwoosung- 콤보게이지로 통일
		nRui->thePcParam.Stat1.theGage = aMsg->theGage;
		g_Pc.GetPlayer()->theActionSystem.SetComboGauge();		
		
		// 플레이어한테 맞는다 (PC, NPC)
		//	EVENT_DATA_INFO underattack_event;
		//	underattack_event.type = SM_UNDERATTACK;
		
		// 플레이어가 때린다...
		if( aMsg->theAttackedType == n_PC )
		{
			if( g_Pc.m_MainPC.lUnique == aMsg->theAttackedID )			// 플레이어가 맞는 것은 처리 By wxywxy
			{
				SRpScAttacked AttackedMsg;
				
				AttackedMsg.theAttackID     = g_Pc.m_MainPC.lUnique;
				AttackedMsg.theAttackedCell = aMsg->theAttackedCell; // g_Pc.GetPlayer()->m_curt_event.cell;
				AttackedMsg.theAttackedHP   = aMsg->theAttackedHP;
				AttackedMsg.theMotion       = aMsg->theMotion;
				AttackedMsg.theAttackType   = n_PC;
				AttackedMsg.theCritical     = aMsg->theCritical;
				AttackedMsg.theComboCount   = aMsg->theComboCount;
				AttackedMsg.theComboStatus  = aMsg->theComboStatus; 
				AttackedMsg.theGage		    = aMsg->theGage;
				AttackedMsg.theDieFlag      = aMsg->theDieFlag;
				AttackedMsg.theComboSkill	= aMsg->theComboSkill;
				
				Proc_RpScAttacked( &AttackedMsg );
				
				RLGS("Process Player_Attacked Event Msg In Proc_RpScAttack!!!");
				return;
			}
			
			// unique 를 가지고 테이블 위치를 알아낸다.
			attackedIdx = g_Pc.FindUser(aMsg->theAttackedID);
			
			if( attackedIdx == MAX_USER_INTABLE )
				return;
			
			attack_event.tbl_Idx = attackedIdx;
			attack_event.race	= g_Pc.m_PcTable[attackedIdx].char_info.race;
			attack_event.index	= g_Pc.m_PcTable[attackedIdx].nIndex;
			attack_event.unique = g_Pc.m_PcTable[attackedIdx].lUnique;
			attack_event.x		= g_Pc.m_PcTable[attackedIdx].lDestBlock%(g_Map.m_cSize*2);
			attack_event.y		= g_Pc.m_PcTable[attackedIdx].lDestBlock/(g_Map.m_cSize*2);
			attack_event.vPos.x = g_Pc.m_Pc[attackedIdx].GetPosTM()._41;
			attack_event.vPos.y = g_Pc.m_Pc[attackedIdx].GetPosTM()._42;
			attack_event.vPos.z = g_Pc.m_Pc[attackedIdx].GetPosTM()._43;
			//		attack_event.damage = g_Pc.m_PcTable[attackedIdx].char_info.theHP - aMsg->theAttackedHP; // 현재 HP 에서 예전 HP 를 빼고 난 수치가 데미지로 저장한 다음 처리 		
			int damage = g_Pc.m_PcTable[attackedIdx].char_info.theRealHP - aMsg->theAttackedHP; // 안타깝지만 일단 임시로...^ ^;;; 헐...
			if( damage < 0 ) 
			{ 
				attack_event.damage = 0; 
			}
			else 
			{
				attack_event.damage = damage; 
			}
			
			g_Pc.m_PcTable[attackedIdx].char_info.theRealHP = aMsg->theAttackedHP;
			g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
			
			attack_event.critical = aMsg->theCritical;
			//		attack_event.cell = aMsg->theAttackedCell;		// 0이 아니면 뒤로 나가 떨어지는 경우

			attack_event.count = aMsg->theComboCount;		// 콤보 성공 횟수에 따라 특수효과 발생 
			attack_event.kill = aMsg->theDieFlag;		// 죽어 있으면 한번만 애니메이션
			
			if( aMsg->theComboStatus == n_ComboComplete )
			{
				attack_event.comcomple = TRUE;
			}
			
	//		RLG3("SCATTACK theRealHP = %d, theHP = %d, theMaxHP = %d", g_Pc.m_PcTable[attackedIdx].char_info.theRealHP, g_Pc.m_PcTable[attackedIdx].char_info.theHP, g_Pc.m_PcTable[attackedIdx].char_info.theMaxHP );
			
			if( attack_event.kill )
			{			
				attack_event.damage = aMsg->theAttackedHP;		
				g_Pc.m_PcTable[attackedIdx].char_info.theRealHP = 0;
				g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
				g_Pc.m_Pc[attackedIdx].SetRealDie(TRUE);
				
				//			DebugPrint("-=***ScAttack PC attack_event.kill = %d***=-\n", attack_event.kill);
			}
			
			if( attack_event.kill && g_Pc.GetPlayer()->IsRealDie() )
			{
				attack_event.damage = aMsg->theAttackedHP;
				
				///--JAPAN_BUG_MODIFY
				EVENT_DATA_INFO underattack_event;
				underattack_event.type = SM_STATE_DIE;
				underattack_event.loop = false;					
				underattack_event.motion = n_Die9;
				g_Pc.m_Pc[attackedIdx].PushQueue( underattack_event, TRUE );
				
				g_Pc.m_PcTable[attackedIdx].char_info.theHP = 0;
				g_Pc.m_PcTable[attackedIdx].char_info.theRealHP = 0;				
				g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
				g_Pc.m_Pc[attackedIdx].SetRealDie(TRUE);
				
				return;
			}			
		
			///-- By simwoosung
			///-- 콤보완성 추가스킬을 시전해보아요
			if( aMsg->theComboSkill && aMsg->theComboSkill != 4801 )
			{
				///-- 이전 콤보대상인지를 체크 한다.
				///-- if( nRui->m_Combo2DTimingEffect.m_nRace == c_PcRace_User && 
				///--     nRui->m_Combo2DTimingEffect.m_lDestID == g_Pc.m_PcTable[attackedIdx].lUnique  )
				///-- 이 부분은 좀 테스트가 필요할듯...
				Send_RpCsSkill( g_Pc.m_MainPC.lUnique, g_Pc.m_PcTable[attackedIdx].lUnique, 
					n_PC, aMsg->theComboSkill );				
			}			
		}
		else if( aMsg->theAttackedType == n_NPC )
		{
			attackedIdx = g_Map.FindNpc(aMsg->theAttackedID);
			
			if( attackedIdx == MAX_NPC_INTABLE )
				return;
			
			attack_event.tbl_Idx = attackedIdx;
			attack_event.race	= g_Map.m_NpcTable[attackedIdx].race;
			attack_event.index	= g_Map.m_NpcTable[attackedIdx].index;
			attack_event.unique	= g_Map.m_NpcTable[attackedIdx].lUnique;
			attack_event.x		= g_Map.m_NpcTable[attackedIdx].lDestBlock%(g_Map.m_cSize*2);
			attack_event.y		= g_Map.m_NpcTable[attackedIdx].lDestBlock/(g_Map.m_cSize*2);
			attack_event.vPos.x = g_Map.m_Npc[attackedIdx].GetPosTM()._41;
			attack_event.vPos.y = g_Map.m_Npc[attackedIdx].GetPosTM()._42;
			attack_event.vPos.z = g_Map.m_Npc[attackedIdx].GetPosTM()._43;
			//		attack_event.damage = g_Map.m_NpcTable[attackedIdx].theHP - aMsg->theAttackedHP; // 현재 HP 에서 예전 HP 를 빼고 난 수치가 데미지로 저장한 다음 처리 		
			int damage = g_Map.m_NpcTable[attackedIdx].theRealHP - aMsg->theAttackedHP; 
			if( damage < 0 ) 
			{ 
				attack_event.damage = 0;
			}
			else 
			{ 
				attack_event.damage = damage; 
			}
			
			//dongs 피버그 추가 
			if( g_Map.m_NpcTable[attackedIdx].theHP > g_Map.m_NpcTable[attackedIdx].theMaxHP && (g_Map.m_NpcTable[attackedIdx].theRealHP > 0) )
			{
				g_Map.m_NpcTable[attackedIdx].theHP = g_Map.m_NpcTable[attackedIdx].theRealHP ;
				g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;				
			}
			
			//		if( aMsg->theAttackedHP == 0 && (g_Map.m_Npc[attackedIdx].m_curt_event.type != SM_STATE_DIE))
			//		{				
			//			nRui->m_BugMop.Number = attackedIdx ; 
			//			nRui->m_BugMop.PosX = attack_event.x ;
			//			nRui->m_BugMop.PosY = attack_event.y ;
			//		}		
			
			//		RLG5("ScAttack NPC_ID = %d NPCHP = %d, RaalHP = %d, AttackedHP = %d, Demage = %d", aMsg->theAttackedID, g_Map.m_NpcTable[attackedIdx].theHP, g_Map.m_NpcTable[attackedIdx].theRealHP, aMsg->theAttackedHP, attack_event.damage );
			
			g_Map.m_NpcTable[attackedIdx].theRealHP = aMsg->theAttackedHP;
			g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;		
			
			attack_event.critical = aMsg->theCritical;
			//		attack_event.cell = aMsg->theAttackedCell;		
			attack_event.count = aMsg->theComboCount;	
			attack_event.kill = aMsg->theDieFlag;		// 죽어 있으면 한번만 애니메이션
			
			if( aMsg->theComboStatus == n_ComboComplete )
			{
				attack_event.comcomple = TRUE;
			}	

			if( attack_event.kill )
			{
				attack_event.damage = aMsg->theAttackedHP;			
				
				g_Map.m_NpcTable[attackedIdx].theRealHP = 0;
				g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;
				g_Map.m_Npc[attackedIdx].SetRealDie(TRUE);				
				///-------------------------------------------------------------------------
				///-- Monster ChattingBox	
				///-- 2005.03.07 / Lyul / 죽었을 때		
				//			RLGS("-- Monster Chatting Box : Monster Died by Player!");
				
				if( ( (float)rand() / (float)RAND_MAX) > nRui->m_BugMop.fMopDie  )
				{
					SetMonsterMessage( aMsg->theAttackedID, n_Monster_Die );			
				}				
				
				///-------------------------------------------------------------------------
				
				RLG1( "-=***ScAttack NPC attack_event.kill = %d***=-\n", attack_event.kill );
			}
			
			if( attack_event.kill && g_Pc.GetPlayer()->IsRealDie() )
			{
				attack_event.damage = aMsg->theAttackedHP;

				///--JAPAN_BUG_MODIFY
				EVENT_DATA_INFO underattack_event;
				underattack_event.type = SM_STATE_DIE;
				underattack_event.loop = false;					
				
				if( g_Map.m_Npc[attackedIdx].m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					underattack_event.motion = n_Die9;
				}
				else
				{
					underattack_event.motion = n_NPC_Die0;
				}
				
				g_Map.m_NpcTable[attackedIdx].theHP = 0;
				g_Map.m_NpcTable[attackedIdx].theRealHP = 0;
				g_Map.m_Npc[attackedIdx].PushQueue( underattack_event, TRUE );
				g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;
				g_Map.m_Npc[attackedIdx].SetRealDie(TRUE);
				
				return;
			}			

			///-- By simwoosung
			///-- 콤보완성 추가스킬을 시전해보아요
			if( aMsg->theComboSkill && aMsg->theComboSkill != 4801 )
			{
				///-- 이전 콤보대상인지를 체크 한다.
				///-- if( nRui->m_Combo2DTimingEffect.m_nRace == c_NpcRace_Mob && 
				///--     nRui->m_Combo2DTimingEffect.m_lDestID == g_Map.m_NpcTable[attackedIdx].lUnique  )
				///-- 이 부분은 좀 테스트가 필요할듯...
				Send_RpCsSkill( g_Pc.m_MainPC.lUnique, g_Map.m_NpcTable[attackedIdx].lUnique, 
					n_NPC, aMsg->theComboSkill );
			}			
		}
		
		if( aMsg->theAttackedNum ) 
		{
			attack_event.number = aMsg->theAttackedNum;
			attack_event.target_list = SAFE_NEW_ARRAY( _SAttacked, aMsg->theAttackedNum );			
			
			memcpy( attack_event.target_list, aMsg->theAttackedList, sizeof(SAttacked) * aMsg->theAttackedNum );		
			
			g_Pc.GetSkillSystem()->SetMultiAttackDamageValue( attack_event , n_PC , g_Pc.m_MainPC.lUnique );
		}		
 
		if( g_Pc.GetPlayer()->GetWeaponType() == 1 )
		{
			attack_event.motion = n_Attack10_BowReady0;
		}	
	
		g_Pc.GetPlayer()->PushQueue( attack_event );
		
	}
	return;
}


///----------------------------------------------------------------------------
///-- Palyer Attacked by NPC or Monster
///----------------------------------------------------------------------------
///-- Player 공격당하는  Message 2005.03.08 / Lyul
///-- Player를 공격하는 대상이 Monster 일 경우 '공격 Message' 출력

void	CRockClient::Proc_RpScAttacked( SRpScAttacked* aMsg )
{
	///-------------------------------------------------------------------------
	///-- Monster ChattingBox	
	///-- 2005.03.07 / Lyul / 공격할 때	
	//	RLGS("-- Monster Chatting Box : Monster Attacked Player!");
	if( ( (float)rand() / (float)RAND_MAX) > nRui->m_BugMop.fMopAttack )
	{
		SetMonsterMessage( aMsg->theAttackID, n_Monster_Attack );
	}
	
	///-------------------------------------------------------------------------	
	
	EVENT_DATA_INFO attack_event;
	attack_event.type	= SM_ATTACK_NPC;
	attack_event.motion = aMsg->theMotion;
	attack_event.tbl_Idx = MAX_USER_INTABLE;
	attack_event.race	= g_Pc.m_MainPC.char_info.race;
	attack_event.index	= g_Pc.m_MainPC.nIndex;
	attack_event.unique = g_Pc.m_MainPC.lUnique;
	attack_event.x		= g_Pc.m_MainPC.lDestBlock%(g_Map.m_cSize*2);
	attack_event.y		= g_Pc.m_MainPC.lDestBlock/(g_Map.m_cSize*2);
	attack_event.vPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
	attack_event.vPos.y = g_Pc.GetPlayer()->GetPosTM()._42;
	attack_event.vPos.z = g_Pc.GetPlayer()->GetPosTM()._43;
	
	//	attack_event.damage = nRui->thePcParam.Stat1.theHP - aMsg->theAttackedHP; // 현재 HP 에서 예전 HP 를 빼고 난 수치가 데미지로 저장한 다음 처리 		
	int damage = g_Pc.m_MainPC.char_info.theRealHP - aMsg->theAttackedHP;
	if( damage < 0 )
	{ 
		attack_event.damage = 0; 
	}	
	else 
	{ 
		attack_event.damage = damage;
	}
	
//	PRINT_DLG_LOG("ScAttacked AttackID = %d , Die %d PlayerHP = %d, RealHP = %d, AttackedHP = %d, Demage = %d", 
//		aMsg->theAttackID, aMsg->theDieFlag , nRui->thePcParam.Stat1.theHP, g_Pc.m_MainPC.char_info.theRealHP, aMsg->theAttackedHP, attack_event.damage );
	
	g_Pc.m_MainPC.char_info.theRealHP = aMsg->theAttackedHP;	
	
	//PRINT_DLG_LOG("나 그냥 맞음 들어옴 theRealHP - %d 대미지 - %d 때린놈 아이디 - %d", 
	//g_Pc.m_MainPC.char_info.theRealHP, damage, aMsg->theAttackID );
	
	g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
	
	attack_event.critical = aMsg->theCritical;
	//	attack_event.cell = aMsg->theAttackedCell;		
	attack_event.count = aMsg->theComboCount;	
	attack_event.kill = aMsg->theDieFlag;		// 죽어 있으면 한번만 애니메이션

	// 콤보 시퀀스 완성시에 추가 효과 스킬 적용 - 이동불가만 적용 
	if( aMsg->theComboSkill == 4801 )
	{
		attack_event.code = aMsg->theComboSkill;
	}
	
	if( aMsg->theAttackedNum ) 
	{
		attack_event.number = aMsg->theAttackedNum;
		attack_event.target_list = SAFE_NEW_ARRAY( SAttacked , aMsg->theAttackedNum );
		
		memcpy( attack_event.target_list, aMsg->theAttackedList, sizeof(SAttacked) * aMsg->theAttackedNum );
		
		g_Pc.GetSkillSystem()->SetMultiAttackDamageValue( attack_event , aMsg->theAttackType , aMsg->theAttackID );
	}
	
	int attackIdx = 0;

	if( attack_event.kill )
	{
		///-- JAPAN_BUG_MODIFY
		///-- 내가 날 공격하지 않는 경우에만..;;
		if( !(g_Pc.GetPlayer()->IsRealDie()) && 
			g_Pc.m_MainPC.lUnique != aMsg->theAttackID )
		{
			nRui->thePcParam.Stat1.theHP = 0;
			g_Pc.m_MainPC.char_info.theHP = 0;
			g_Pc.m_MainPC.char_info.theRealHP = 0;
			g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			g_Pc.GetPlayer()->SetRealDie(TRUE, aMsg->theAttackType, aMsg->theAttackID);		
		}
	}
	
	///-- 때리는 주체가 플레이어일 경우

	if( g_Pc.m_MainPC.lUnique == aMsg->theAttackID && aMsg->theAttackType == n_PC )
	{		
		///-- 죽지않아야 데미지 처리를 함
		if( !(g_Pc.GetPlayer()->IsRealDie()) )
		{
			if( attack_event.kill )
			{
				///-- JAPAN_BUG_MODIFY
				///-- 내가 날 공격하고 바로 죽을시에는
				EVENT_DATA_INFO underattack_event;
				underattack_event.type = SM_STATE_DIE;
				underattack_event.loop = false;
				
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_DIE_COUNTER ) );
				
				underattack_event.motion = n_Die9;
				g_Pc.GetPlayer()->PushQueue( underattack_event );			
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
				g_Pc.GetPlayer()->SetRealDie(TRUE, n_PC, g_Pc.m_MainPC.lUnique);
				nRui->thePcParam.Stat1.theHP = 0;
				g_Pc.m_MainPC.char_info.theHP = 0;
				g_Pc.m_MainPC.char_info.theRealHP = 0;
			}
			
			return;
		}
		
		g_Pc.GetPlayer()->PushQueue(attack_event);	
	}
	else
	{
		///-- 때리는 주체가 일반유저일 경우

		if( aMsg->theAttackType == n_PC )
		{
			// unique 를 가지고 테이블 위치를 알아낸다.
			attackIdx = g_Pc.FindUser(aMsg->theAttackID);
			
			///-- 공격주체가 현재 존재하지 않을경우

			if( attackIdx == MAX_USER_INTABLE )
			{
				///-- by simwoosung 공격 주체가 없을시에도 죽음, 데미지처리는 해야 한다.
				SetProcessAttackEvent( attack_event, TRUE, FALSE, aMsg->theAttackType, aMsg->theAttackID );			
				
				g_RockClient.Send_RpCsReqCharInfo( aMsg->theAttackID, aMsg->theAttackType );
				return;
			}
			
			///-- 공격주체가 현재 죽어있다면....
			///--JAPAN_BUG_MODIFY
			if( g_Pc.m_Pc[attackIdx].IsRealDie() )
			{
				///-- by simwoosung 공격 주체가 죽을시에도 죽음, 데미지처리는 해야 한다.
				SetProcessAttackEvent( attack_event, TRUE, FALSE, aMsg->theAttackType, aMsg->theAttackID );				
				
				return;
			}
			
			g_Pc.m_Pc[attackIdx].m_Weapon_DelayTime = aMsg->theAttackSpeed * 100;
			
			///-- 활을 무기로 가지고 있을 경우 
			if( g_Pc.m_Pc[attackIdx].GetWeaponType() == 1 )
			{
				attack_event.motion = n_Attack10_BowReady0;
			}
			
			//if( g_Pc.m_Pc[attackIdx].m_bIsVisible == true && g_Pc.m_Pc[attackIdx].m_bIsLoaded == true )
			{
				///-- simwoosung 이곳으로 큐를 처리하는 루틴을 따로 빼서 적용

				g_Pc.m_Pc[attackIdx].PushQueue( attack_event );
			}
		}
		else if( aMsg->theAttackType == n_NPC )	///-- 공격주체가 몬스터일경우

		{
			attackIdx = g_Map.FindNpc(aMsg->theAttackID);		
			
			if( attackIdx == MAX_NPC_INTABLE )	///-- 공격주체가 존재하지 않을경우

			{
				///-- by simwoosung 공격 주체가 없을시에도 죽음, 데미지처리는 해야 한다.
				SetProcessAttackEvent( attack_event, TRUE, FALSE, aMsg->theAttackType, aMsg->theAttackID );
				
				g_RockClient.Send_RpCsReqCharInfo( aMsg->theAttackID, aMsg->theAttackType );
				return;
			}
			
			///--JAPAN_BUG_MODIFY
			if( g_Map.m_Npc[attackIdx].IsRealDie() )
			{
				///-- by simwoosung 공격 주체가 죽을시에도 죽음, 데미지처리는 해야 한다.
				SetProcessAttackEvent( attack_event, TRUE, FALSE, aMsg->theAttackType, aMsg->theAttackID );
				
				return;
			}

			//거대몹 크리리 티컬 .. 처리 .. 
			if( g_Map.m_Npc[attackIdx].m_State.nCharacterType == nCharacterTypeNormal   
				||g_Map.m_Npc[attackIdx].m_State.nCharacterType == nCharacterTypeBigMoster )
			{
				if( attack_event.critical )
				{
					attack_event.motion = n_NPC_Attack1;		
				}
				else
				{
					attack_event.motion = n_NPC_Attack0;		
				}
			}
			else if( g_Map.m_Npc[attackIdx].m_State.nCharacterType == nCharacterTypePlayerNPC )
			{
				///-- 공격 클래스가 아쳐나 스나이퍼 라면

				if( g_Map.m_NpcTable[attackIdx].theExPcInfo.theMainClass == Archer ||
					g_Map.m_NpcTable[attackIdx].theExPcInfo.theMainClass == Sniper )
				{
					attack_event.motion = n_Attack10_BowReady0;				
				}
				else
				{
					if( attack_event.critical )
					{
						attack_event.motion = n_Attack9_Basic4;		
					}
					else
					{
						attack_event.motion = n_Attack9_Basic0;		
					}
				}					
			}
			
			g_Map.m_Npc[attackIdx].m_Weapon_DelayTime = aMsg->theAttackSpeed * 100;
			
			//if( g_Map.m_Npc[attackIdx].m_bIsVisible == true && g_Map.m_Npc[attackIdx].m_bIsLoaded == true )
			{
				g_Map.m_Npc[attackIdx].PushQueue(attack_event);		
			}			
		} 
	}
	
	return;
}
///----------------------------------------------------------------------------
///-- NPC or Monster Attack NPC or Monster
///----------------------------------------------------------------------------
///-- 주변의 NPC/Monster 공격 Message 2005.03.08 / Lyul
///-- 공격 주체가 Monster : Monster '공격 Message'
///-- 공격 대상이 Monster : Monster '맞음 Message'
void	CRockClient::Proc_RpScSeeAttack( SRpScSeeAttack* aMsg )
{ 
	//	RLGS("ScSeeAttack");
	//	RLG7( "ScSeeAttack : attackID = %u, attackedID = %u, damage = %u, attackType = %d, attackedType = %d, motion = %d, critical = %d", 
	//	aMsg->theAttackID, aMsg->theAttackedID, aMsg->theDamage, 
	//	aMsg->theAttackType, aMsg->theAttackedType, aMsg->theMotion, aMsg->theCritical );
	
	
	EVENT_DATA_INFO attack_event;
	attack_event.type	= SM_ATTACK_NPC;
	attack_event.motion = aMsg->theMotion;	
	
	EVENT_DATA_INFO underattack_event;
	underattack_event.type = SM_UNDERATTACK;
	
	int attackIdx = 0;
	int attackedIdx = 0; 
	
	//쓰레기 모션값이 들어올경우

	if( n_Animation_MaxCount < attack_event.motion )
		attack_event.motion = n_Attack9_Basic4;	
	
	// 콤보 시퀀스 완성시에 추가 효과 스킬 적용 - 이동불가 만 적용 
	if( aMsg->theComboSkill == 4801 )
	{
		attack_event.code = aMsg->theComboSkill;
	}
	
	if( aMsg->theAttackedNum && 
		!( aMsg->theAttackedType == n_PC && g_Pc.m_MainPC.lUnique == aMsg->theAttackedID ) ) 
	{
		///-- By simwoosung
		///-- 받아치기 루틴수정 플레이어가 맞는 상황이 아닐경우에만..
		///-- 멀티 대미지 처리를 해준다.
		attack_event.number = aMsg->theAttackedNum;
		attack_event.target_list = SAFE_NEW_ARRAY( SAttacked , aMsg->theAttackedNum );		
	
		memcpy( attack_event.target_list, aMsg->theAttackedList, sizeof(SAttacked) * aMsg->theAttackedNum );		
		g_Pc.GetSkillSystem()->SetMultiAttackDamageValue( attack_event, aMsg->theAttackType, aMsg->theAttackID );
	}
	
	///-- 플레이어가 맞을 때
	if( aMsg->theAttackedType == n_PC )  ///-- 유저가 공격당할때..
	{
		///--------------------------------------------------------------------
		///-- Monster ChattingBox	
		///-- 2005.03.07 / Lyul / 공격당할 때	
		//		RLGS("-- Monster Chatting Box : Monster Attacked OtherUser!");
		if( ( (float)rand() / (float)RAND_MAX) > nRui->m_BugMop.fMopAttack )
		{
			SetMonsterMessage( aMsg->theAttackID, n_Monster_Attack );
		}	
		
		///--------------------------------------------------------------------
		
		///--- 공격당한 유저가 플레이어 자신일때	
		if( g_Pc.m_MainPC.lUnique == aMsg->theAttackedID )			
		{
			SRpScAttacked AttackedMsg;
			
			AttackedMsg.theAttackID     = aMsg->theAttackID;
			AttackedMsg.theAttackedCell = aMsg->theAttackedCell;
			AttackedMsg.theAttackedHP   = aMsg->theAttackedHP;
			AttackedMsg.theMotion       = aMsg->theMotion;
			AttackedMsg.theAttackType   = aMsg->theAttackType;
			AttackedMsg.theCritical     = aMsg->theCritical;
			AttackedMsg.theComboCount   = aMsg->theComboCount;
			AttackedMsg.theComboStatus  = aMsg->theComboStatus; 
			AttackedMsg.theGage			= aMsg->theGage;
			AttackedMsg.theDieFlag      = aMsg->theDieFlag;
			AttackedMsg.theComboSkill	= aMsg->theComboSkill;
			AttackedMsg.theAttackedNum	= aMsg->theAttackedNum;
			AttackedMsg.theAttackSpeed	= aMsg->theAttackSpeed;
			
			if(aMsg->theDieFlag)
			{
				int a = 0;
			}
			
			if( aMsg->theAttackedNum )
			{
				memcpy( AttackedMsg.theAttackedList, aMsg->theAttackedList, sizeof(SAttacked) * aMsg->theAttackedNum );
			}
			
			Proc_RpScAttacked( &AttackedMsg );
			return;
		}		
		
		attackedIdx = g_Pc.FindUser(aMsg->theAttackedID);
		if( attackedIdx == MAX_USER_INTABLE )
		{
			///-- 직접적인 대상은 없지만.. 공격대상이 다수존재할 경우를 위해 루틴처리 필요			
			SetProcessAttackEvent( attack_event, FALSE, FALSE,  aMsg->theAttackType, aMsg->theAttackID );
			
			RLGS("!ScSeeAttack 맞을 놈이 없다니 누굴 때려야 하나...ㅡㅡ; 에공...");
			return;
		}
		
		attack_event.tbl_Idx = attackedIdx;
		attack_event.race	= g_Pc.m_PcTable[attackedIdx].char_info.race;
		attack_event.index	= g_Pc.m_PcTable[attackedIdx].nIndex;
		attack_event.unique = g_Pc.m_PcTable[attackedIdx].lUnique;
		attack_event.x		= g_Pc.m_PcTable[attackedIdx].lDestBlock%(g_Map.m_cSize*2);
		attack_event.y		= g_Pc.m_PcTable[attackedIdx].lDestBlock/(g_Map.m_cSize*2);
		attack_event.vPos.x = g_Pc.m_Pc[attackedIdx].GetPosTM()._41;
		attack_event.vPos.y = g_Pc.m_Pc[attackedIdx].GetPosTM()._42;
		attack_event.vPos.z = g_Pc.m_Pc[attackedIdx].GetPosTM()._43;
		//		attack_event.damage = g_Pc.m_PcTable[attackedIdx].char_info.theHP - aMsg->theAttackedHP; // 현재 HP 에서 예전 HP 를 빼고 난 수치가 데미지로 저장한 다음 처리 		
		int damage = g_Pc.m_PcTable[attackedIdx].char_info.theRealHP - aMsg->theAttackedHP; 
		if( damage < 0 ) { attack_event.damage = 0; }	else { attack_event.damage = damage; }
		g_Pc.m_PcTable[attackedIdx].char_info.theRealHP = aMsg->theAttackedHP;
		g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
		
		attack_event.critical = aMsg->theCritical;		
		attack_event.count = aMsg->theComboCount;	
		attack_event.kill = aMsg->theDieFlag;		// 죽어 있으면 한번만 애니메이션
		
		if( attack_event.kill )
		{
			attack_event.damage = aMsg->theAttackedHP;
			g_Pc.m_PcTable[attackedIdx].char_info.theRealHP = 0;
			g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
			g_Pc.m_Pc[attackedIdx].SetRealDie(TRUE);
		}		
	}
	///-- NPC가 맞을 때
	else if( aMsg->theAttackedType == n_NPC )
	{
		
		///--------------------------------------------------------------------
		///-- Monster ChattingBox	
		///-- 2005.03.07 / Lyul / 공격할 때	
		RLGS("-- Monster Chatting Box : OtherUser Attacked Monster!");
		
		///-------------------------------------------------------------------------
		///-- Monster ChattingBox	
		///-- 2005.03.07 / Lyul
		if( ( (float)rand() / (float)RAND_MAX) > nRui->m_BugMop.fMopAttacked )
		{
			SetMonsterMessage( aMsg->theAttackedID, n_Monster_Attacked );
		}
		
		///--------------------------------------------------------------------
		
		attackedIdx = g_Map.FindNpc(aMsg->theAttackedID);
		if( attackedIdx == MAX_NPC_INTABLE )
		{
			///-- 직접적인 대상은 없지만.. 공격대상이 다수존재할 경우를 위해 루틴처리 필요			
			SetProcessAttackEvent( attack_event, FALSE, FALSE,  aMsg->theAttackType, aMsg->theAttackID );
			
			RLGS("!ScSeeAttack 맞을 놈이 없다니 누굴 때려야 하나...ㅡㅡ; 에공...");
			return;
		}
		
		attack_event.tbl_Idx = attackedIdx;
		attack_event.race	= g_Map.m_NpcTable[attackedIdx].race;
		attack_event.index	= g_Map.m_NpcTable[attackedIdx].index;
		attack_event.unique	= g_Map.m_NpcTable[attackedIdx].lUnique;
		attack_event.x		= g_Map.m_NpcTable[attackedIdx].lDestBlock%(g_Map.m_cSize*2);
		attack_event.y		= g_Map.m_NpcTable[attackedIdx].lDestBlock/(g_Map.m_cSize*2);
		attack_event.vPos.x = g_Map.m_Npc[attackedIdx].GetPosTM()._41;
		attack_event.vPos.y = g_Map.m_Npc[attackedIdx].GetPosTM()._42;
		attack_event.vPos.z = g_Map.m_Npc[attackedIdx].GetPosTM()._43;		
		//		attack_event.damage = g_Map.m_NpcTable[att2ackedIdx].theHP - aMsg->theAttackedHP; // 현재 HP 에서 예전 HP 를 빼고 난 수치가 데미지로 저장한 다음 처리 		
		int damage = g_Map.m_NpcTable[attackedIdx].theRealHP - aMsg->theAttackedHP; 
		if( damage < 0 ) { attack_event.damage = 0; }	else { attack_event.damage = damage; }
		
		g_Map.m_NpcTable[attackedIdx].theRealHP = aMsg->theAttackedHP;
		g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;
		
		attack_event.critical = aMsg->theCritical;
		//		attack_event.cell = aMsg->theAttackedCell;		
		attack_event.count = aMsg->theComboCount;	
		attack_event.kill = aMsg->theDieFlag;		// 죽어 있으면 한번만 애니메이션	
		
		if( attack_event.kill )
		{
			attack_event.damage = aMsg->theAttackedHP;
			g_Map.m_NpcTable[attackedIdx].theRealHP = 0;
			g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;
			g_Map.m_Npc[attackedIdx].SetRealDie(TRUE);
			
			///-------------------------------------------------------------------------
			///-- Monster ChattingBox	
			///-- 2005.03.07 / Lyul / 죽었을 때			
			RLGS("-- Monster Chatting Box : Monster Died by OtherUser!");
			if( ( (float)rand() / (float)RAND_MAX) > nRui->m_BugMop.fMopDie  )
			{
				SetMonsterMessage( aMsg->theAttackedID, n_Monster_Die );			
			}
			///-------------------------------------------------------------------------			
			DebugPrint("-=***ScSeeAttack NPC attack_event.kill = %d***=-\n", attack_event.kill);
		}
	}
	
	///-- 플레이어가 공격할 경우

	if( aMsg->theAttackType == n_PC )
	{						
		// unique 를 가지고 테이블 위치를 알아낸다.
		attackIdx = g_Pc.FindUser(aMsg->theAttackID);		
		
		///-- 공격 주체가 존재하지 않을 경우

		if( attackIdx == MAX_USER_INTABLE )
		{
			
#ifdef ROCKCLIENT_DEV				
			RLG2( "!SeeAttack [%d]이 없는데 [%d]를 때리란다", aMsg->theAttackID, aMsg->theAttackedID );			
#endif
			
			if( aMsg->theAttackedType == n_PC )
			{
				SetProcessAttackEvent( attack_event, FALSE, TRUE, aMsg->theAttackType,
					aMsg->theAttackID, aMsg->theAttackedType, attackedIdx );
				
				g_RockClient.Send_RpCsReqCharInfo( aMsg->theAttackID, aMsg->theAttackType );
				
				return;
			}
			else if( aMsg->theAttackedType == n_NPC )
			{
				SetProcessAttackEvent( attack_event, FALSE, TRUE, aMsg->theAttackType,
					aMsg->theAttackID, aMsg->theAttackedType, attackedIdx );
				
				g_RockClient.Send_RpCsReqCharInfo( aMsg->theAttackID, aMsg->theAttackType );
				
				return;
			}			
		}
		
		///-- 공격주체가 이미 죽어있을 경우

		///--JAPAN_BUG_MODIFY
		if( g_Pc.m_Pc[attackIdx].IsRealDie() )
		{
#ifdef ROCKCLIENT_DEV	
			RLG2( "!SeeAttack [%d]는 죽었는데 [%d]를 때리란다", aMsg->theAttackID, aMsg->theAttackedID );			
#endif
			
			if( aMsg->theAttackedType == n_PC )
			{
				SetProcessAttackEvent( attack_event, FALSE, TRUE, aMsg->theAttackType,
					aMsg->theAttackID, aMsg->theAttackedType, attackedIdx );
				
				return;
			}
			else if( aMsg->theAttackedType == n_NPC )
			{
				SetProcessAttackEvent( attack_event, FALSE, TRUE, aMsg->theAttackType,
					aMsg->theAttackID, aMsg->theAttackedType, attackedIdx );
				
				return;
			}
		}
		
		g_Pc.m_Pc[attackIdx].m_Weapon_DelayTime = aMsg->theAttackSpeed * 100;

		///-- 활을 무기로 가지고 있을 경우 
		if( g_Pc.m_Pc[attackIdx].GetWeaponType() == 1 )
		{
			attack_event.motion = n_Attack10_BowReady0;
		}
		
		g_Pc.m_Pc[attackIdx].PushQueue(attack_event);
		
	}
	//공격자가 .. NPC 경우에 .. 
	else if( aMsg->theAttackType == n_NPC )
	{
		attackIdx = g_Map.FindNpc(aMsg->theAttackID);
		
		///-- 공격 주체가 존재하지 않을 경우

		if( attackIdx == MAX_NPC_INTABLE )
		{
#ifdef ROCKCLIENT_DEV				
			RLG2( "!SeeAttack [%d]이 없는데 [%d]를 때리란다", aMsg->theAttackID, aMsg->theAttackedID );			
#endif
			
			if( aMsg->theAttackedType == n_PC )
			{
				SetProcessAttackEvent( attack_event, FALSE, TRUE, aMsg->theAttackType,
					aMsg->theAttackID, aMsg->theAttackedType, attackedIdx );
				
				g_RockClient.Send_RpCsReqCharInfo( aMsg->theAttackID, aMsg->theAttackType );
				return;
			}
			else if( aMsg->theAttackedType == n_NPC )
			{
				SetProcessAttackEvent( attack_event, FALSE, TRUE, aMsg->theAttackType,
					aMsg->theAttackID, aMsg->theAttackedType, attackedIdx );
				
				g_RockClient.Send_RpCsReqCharInfo( aMsg->theAttackID, aMsg->theAttackType );
				return;
			}
			
		}
		
		///-- 공격 주체가 이미 죽어있을경우

		///--JAPAN_BUG_MODIFY
		if( g_Map.m_Npc[attackIdx].IsRealDie() )
		{
#ifdef ROCKCLIENT_DEV	
			RLG2( "!SeeAttack [%d]는 죽었는데 [%d]를 때리란다", aMsg->theAttackID, aMsg->theAttackedID );			
#endif			
			
			if( aMsg->theAttackedType == n_PC )
			{
				SetProcessAttackEvent( attack_event, FALSE, TRUE, aMsg->theAttackType,
					aMsg->theAttackID, aMsg->theAttackedType, attackedIdx );
				
				return;
			}
			else if( aMsg->theAttackedType == n_NPC )
			{
				SetProcessAttackEvent( attack_event, FALSE, TRUE, aMsg->theAttackType,
					aMsg->theAttackID, aMsg->theAttackedType, attackedIdx );
				
				return;
			}
		}
		
		if( g_Map.m_Npc[attackIdx].m_State.nCharacterType == nCharacterTypeNormal )
		{
			if( attack_event.critical )
			{
				attack_event.motion = n_NPC_Attack1;		
			}
			else
			{
				attack_event.motion = n_NPC_Attack0;		
			}
		}
		else if( g_Map.m_Npc[attackIdx].m_State.nCharacterType == nCharacterTypePlayerNPC )
		{
			///-- 공격 클래스가 아쳐나 스나이퍼 라면

			if( g_Map.m_NpcTable[attackIdx].theExPcInfo.theMainClass == Archer ||
				g_Map.m_NpcTable[attackIdx].theExPcInfo.theMainClass == Sniper )
			{
				attack_event.motion = n_Attack10_BowReady0;				
			}
			else
			{
				if( attack_event.critical )
				{
					attack_event.motion = n_Attack9_Basic4;		
				}
				else
				{
					attack_event.motion = n_Attack9_Basic0;		
				}	
			}
		}
		
		g_Map.m_Npc[attackIdx].m_Weapon_DelayTime = aMsg->theAttackSpeed * 100;
		
		g_Map.m_Npc[attackIdx].PushQueue(attack_event);
		
	}
	
	return;
}

void	CRockClient::Proc_RpScAppearNPC( SRpScAppearNPC* aMsg )
{
	RLG4( "****ScAppearNPC : npcID = [%u], npcType = %u, cellIndex = %u, HP = %d",
		aMsg->theNPCID, aMsg->theNPCType, aMsg->theCellIndex, aMsg->theHP );
	
	//	DWORD				theNPCID;
	//	WORD				theNPCType;
	//	DWORD				theCellIndex;	// CellIndex
	//	DWORD				theHP;
	//	DWORD				theSP;
	//	BYTE				theAttackSpeed;
	//	WORD				theMoveSpeed;
	//	WORD				theKeepupSkill[RP_MAX_KEEPUP_SKILL];
	//	BYTE				thePriorityType;	// 선점권자 정보 n_PC, n_Party
	//	DWORD				thePriorityID;	
	
	// 신수 인지 체크
	if( g_Pc.ISCheckHoly( aMsg ) )
	{
		return;
	}
	
	/// 유저형 NPC인지 체크
	if( ISCheckPlayerNPC( aMsg ) )
	{
		return;
	}

	BYTE * pBuff = NULL;
	pBuff = (BYTE *)aMsg;

	int nPackSize = 0;

#ifdef PACKET_RESIZE

	nPackSize += sizeof( aMsg->theMsgHeader ) + sizeof( DWORD ) + sizeof( WORD ) + sizeof( DWORD ) + sizeof( DWORD );
	nPackSize += sizeof( BYTE ) + sizeof( WORD ) + sizeof( BYTE );

	memset( g_FrontierName, 0, sizeof( g_FrontierName ) );
	memset( g_FrontierPetName, 0, sizeof( g_FrontierPetName ) );

	DWORD aNPCID	 = aMsg->theNPCID;
	WORD  aNPCType	 = aMsg->theNPCType;	
	DWORD aCellIndex = aMsg->theCellIndex;
	DWORD aHP		 = aMsg->theHP;
	DWORD aSP = 0;
	BYTE  aAttackSpeed = aMsg->theAttackSpeed;
	WORD  aMoveSpeed =   aMsg->theMoveSpeed;

	SNPCBaseInfo* npc_Info = NULL;
	
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aNPCType ); 

	if( npc_Info == NULL )
		return;

	aSP = npc_Info->theMaxSP;	

	float fAngle = 0.0f;
	BYTE  aPriorityType = n_NONEIDType;
	DWORD aPriorityID = 0;	
	DWORD aFrontierID = 0;	

	if( aMsg->theFrontierFlag && !aMsg->thePriorityFlag )
	{
		memcpy( &aPriorityType, &pBuff[nPackSize], sizeof( BYTE ) );
		nPackSize += sizeof(BYTE);
		memcpy( &aPriorityID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);
		memcpy( &fAngle, &pBuff[nPackSize], sizeof( float ) );
		nPackSize += sizeof(float);	
		memcpy( &aFrontierID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);	
		memcpy( g_FrontierName, &pBuff[nPackSize], RP_MAX_FRONTIER_NAME );		
		nPackSize += RP_MAX_FRONTIER_NAME;
		memcpy( g_FrontierPetName, &pBuff[nPackSize], RP_MAX_FRONTIER_PET_NAME );		
		nPackSize += RP_MAX_FRONTIER_PET_NAME;	
	}

	if( aMsg->thePriorityFlag && !aMsg->theFrontierFlag )
	{
		memcpy( &aPriorityType, &pBuff[nPackSize], sizeof( BYTE ) );
		nPackSize += sizeof(BYTE);
		memcpy( &aPriorityID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);
		memcpy( &fAngle, &pBuff[nPackSize], sizeof( float ) );
		nPackSize += sizeof(float);
	}

	int nKeepUpSkillNum = aMsg->theKeepupSkillNum;
	WORD * pKeepUpSkillList = NULL;

	if( nKeepUpSkillNum )
	{
		pKeepUpSkillList = SAFE_NEW_ARRAY( WORD , nKeepUpSkillNum );
		memset( pKeepUpSkillList, 0 , sizeof( WORD ) * nKeepUpSkillNum );
		memcpy( pKeepUpSkillList, &pBuff[nPackSize], sizeof( WORD ) * nKeepUpSkillNum );
		nPackSize += ( sizeof( WORD ) * nKeepUpSkillNum );	
	}	

	int npc_tbl_index = g_Map.FindNpc( (long)aNPCID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{		
		npc_tbl_index = g_Map.SM__NPC__APPEAR( aNPCType, aNPCID, aCellIndex, 
											   aHP, aSP, 0 , fAngle );
		
		if( npc_tbl_index == -1 )
		{
			SAFE_DELETE_ARRAY( pKeepUpSkillList );
			return;
		}		
	
		g_Map.m_Npc[npc_tbl_index].thePath[0] = aCellIndex;
		
		for( int i = 0; i < nKeepUpSkillNum ; ++i )
		{
			if( pKeepUpSkillList[i] )
			{
				///--JAPAN_BUG_MODIFY
				g_Map.m_NpcTable[npc_tbl_index].theKeepupSkill[i] = pKeepUpSkillList[i];				
				// 지속성 스킬 이펙트 추가 [상준]
				g_Particle.m_EffectManager.CreateKeepupSkill( 
					pKeepUpSkillList[i] , &g_Map.m_Npc[npc_tbl_index] , NULL , FALSE );
			}
		}
		
		// 선점 처리 By wxywxy
		g_Map.m_Npc[npc_tbl_index].SetPriority( aPriorityType , aPriorityID );
		  
		//theAttackSpeed; theMoveSpeed;	
		g_Map.m_Npc[npc_tbl_index].m_Weapon_DelayTime = aAttackSpeed * 100; 
		
		// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
		SNPCBaseInfo*	npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[npc_tbl_index].code ); 

		if( npc_Info->theMoveSpeed )
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = ( (float)aMoveSpeed / npc_Info->theMoveSpeed );		
			g_Map.m_Npc[npc_tbl_index].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[npc_tbl_index].m_ani_percent );
		}
		else
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = 1.0f;
			g_Map.m_Npc[npc_tbl_index].m_move_percent = 1.0f;
		}	

		///-------------------------------------------------------------------------
		///-- Monster ChattingBox	
		///-- 2005.03.07 / Lyul / 리젠
		//RLGS("-- Monster Chatting Box : Regen Monster!");
		SetMonsterMessage( aNPCID, n_Monster_Rengen );
		///-------------------------------------------------------------------------
	}
	else
	{
		//지금 해당 NPC가 존재하고 있다면;;;
		NonPlayer *pNonPlaer = g_Map.GetMonster( (int)aNPCID );
		if(pNonPlaer)
		{
			///-- JAPAN_BUG_MODIFY 
			///-- 해당 NPC가 죽어있으면;; 여기서는 죽는 애니메이션이 들어가기전에..
			///-- 리젠이 되어버릴수 있기 때문에...;;; 이런처리를 함 
			if( pNonPlaer->IsRealDie() )
			{
				//맵에서 삭제
				g_Map.SM__NPC__DISAPPEAR( 0, aNPCID );			
				//다시 AppearNPC 호출

				Proc_RpScAppearNPC( aMsg );
				RLG1( " Update Monster(Disappear - Appear) %d", aNPCID );
				SAFE_DELETE_ARRAY( pKeepUpSkillList );
				return;				
			}
			else
			{
				if( g_Map.m_NpcTable[npc_tbl_index].lDestBlock != aCellIndex )
				{
					if( g_Map.m_NpcTable[npc_tbl_index].theRealHP != aHP )
					{
//						ClientFilelog( "Not Same HP Before = %u Now = %u", 
//									g_Map.m_NpcTable[npc_tbl_index].theRealHP, aHP );
					}
				}
			}		
			
			// 정보 갱신 
			g_Map.m_NpcTable[npc_tbl_index].lBlock = aCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].lDestBlock = aCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].theHP = aHP;
			g_Map.m_NpcTable[npc_tbl_index].theRealHP = aHP;
			g_Map.m_NpcTable[npc_tbl_index].theSMP = aSP;
			
			if( g_Map.m_NpcTable[npc_tbl_index].lDestBlock != aCellIndex )
			{
				g_Map.m_Npc[npc_tbl_index].SetDirection( D3DXToRadian( fAngle ) );
			}				
		}		
		
		RLG1( " Already Exist NPC ID - %d : ScAppearNPC", aNPCID );
		SAFE_DELETE_ARRAY( pKeepUpSkillList );
		return;		
	}
	
	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
	//......................................................................................................
	npc_tbl_index = g_Map.FindNpc( (long)aNPCID );			// 위에서 새로 만들수 있어서 다시 검색
	
	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		
		SNPCBaseInfo* npc_Info = NULL;
		
		NPCTABLE *pTable = g_Map.GetNpcTable( (long)aNPCID );
		
		if( NULL != pTable )
		{
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
			
			if( NULL != npc_Info )
			{
				if( npc_Info->theRightEff != -1 )
				{
					g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff , 
						( Character * ) &g_Map.m_Npc[npc_tbl_index] , NULL , NULL );
				}
			}
		}
	}

	SAFE_DELETE_ARRAY( pKeepUpSkillList );	
	return;

#else
	
	int npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		
		//		g_Map.SM__NPC__APPEAR( index, aMsg->theNPCID, aMsg->theCellIndex, 0, 0, 0 ); // index는 몬스터 타입을 말하는 듯...
		//		g_Map.SM__NPC__APPEAR( aMsg->theNPCType, aMsg->theNPCID, aMsg->theCellIndex, 0, 0, 0 );
		npc_tbl_index = g_Map.SM__NPC__APPEAR( aMsg->theNPCType, aMsg->theNPCID, aMsg->theCellIndex, aMsg->theHP, aMsg->theSP, 0 , aMsg->theAngle );
		
		if( npc_tbl_index == -1 )
		{
			return;
		}
		
		//		npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID );
		g_Map.m_Npc[npc_tbl_index].thePath[0] = aMsg->theCellIndex;
		
		//RLG1( "ScAppearNPC : npc_tbl_index = [%d]", npc_tbl_index );
		
		for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			if( aMsg->theKeepupSkill[i] )
			{
				///--JAPAN_BUG_MODIFY
				g_Map.m_NpcTable[npc_tbl_index].theKeepupSkill[i] = aMsg->theKeepupSkill[i];				
				// 지속성 스킬 이펙트 추가 [상준]
				g_Particle.m_EffectManager.CreateKeepupSkill( aMsg->theKeepupSkill[i] , &g_Map.m_Npc[npc_tbl_index] , NULL , FALSE );
			}
		}
		
		// 선점 처리 By wxywxy
		g_Map.m_Npc[npc_tbl_index].SetPriority( aMsg->thePriorityType , aMsg->thePriorityID );
		  
		//theAttackSpeed; theMoveSpeed;	
		g_Map.m_Npc[npc_tbl_index].m_Weapon_DelayTime = aMsg->theAttackSpeed * 100; 
		
		// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
		SNPCBaseInfo*	npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[npc_tbl_index].code ); 

		if( npc_Info->theMoveSpeed )
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = ( (float)aMsg->theMoveSpeed / npc_Info->theMoveSpeed );		
			g_Map.m_Npc[npc_tbl_index].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[npc_tbl_index].m_ani_percent );
		}
		else
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = 1.0f;
			g_Map.m_Npc[npc_tbl_index].m_move_percent = 1.0f;
		}	

		///-------------------------------------------------------------------------
		///-- Monster ChattingBox	
		///-- 2005.03.07 / Lyul / 리젠
		//RLGS("-- Monster Chatting Box : Regen Monster!");
		SetMonsterMessage( aMsg->theNPCID, n_Monster_Rengen );
		///-------------------------------------------------------------------------
	}
	else
	{
		//지금 해당 NPC가 존재하고 있다면;;;
		NonPlayer *pNonPlaer = g_Map.GetMonster( (int)aMsg->theNPCID );
		if(pNonPlaer)
		{
			///-- JAPAN_BUG_MODIFY 
			///-- 해당 NPC가 죽어있으면;; 여기서는 죽는 애니메이션이 들어가기전에..
			///-- 리젠이 되어버릴수 있기 때문에...;;; 이런처리를 함 
			if( pNonPlaer->IsRealDie() )
			{
				//맵에서 삭제
				g_Map.SM__NPC__DISAPPEAR( 0, aMsg->theNPCID );			
				//다시 AppearNPC 호출

				Proc_RpScAppearNPC( aMsg );
				RLG1( " Update Monster(Disappear - Appear) %d", aMsg->theNPCID );
				return;				
			}
			else
			{
				///-- ClientFilelog( "Not Dead DisAppear And Appear NPC" );
				
				if( g_Map.m_NpcTable[npc_tbl_index].lDestBlock != aMsg->theCellIndex )
				{
					if( g_Map.m_NpcTable[npc_tbl_index].theRealHP != aMsg->theHP )
					{
//						ClientFilelog( "Not Same HP Before = %u Now = %u", 
//									g_Map.m_NpcTable[npc_tbl_index].theRealHP, aMsg->theHP );
					}
				}
			}		
			


			// 정보 갱신 
			g_Map.m_NpcTable[npc_tbl_index].lBlock = aMsg->theCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].lDestBlock = aMsg->theCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].theHP = aMsg->theHP;
			g_Map.m_NpcTable[npc_tbl_index].theRealHP = aMsg->theHP;
			g_Map.m_NpcTable[npc_tbl_index].theSMP = aMsg->theSP;
			
			if( g_Map.m_NpcTable[npc_tbl_index].lDestBlock != aMsg->theCellIndex )
			{
				g_Map.m_Npc[npc_tbl_index].SetDirection( D3DXToRadian( aMsg->theAngle ) );
			}				
		}		
		
		RLG1( " Already Exist NPC ID - %d : ScAppearNPC", aMsg->theNPCID );
		return;		
	}
	
	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
	//......................................................................................................
	npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID );			// 위에서 새로 만들수 있어서 다시 검색

	
	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		
		SNPCBaseInfo* npc_Info = NULL;
		
		NPCTABLE *pTable = g_Map.GetNpcTable( (long)aMsg->theNPCID );
		
		if( NULL != pTable )
		{
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
			
			if( NULL != npc_Info )
			{
				if( npc_Info->theRightEff != -1 )
				{
					g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff , ( Character * ) &g_Map.m_Npc[npc_tbl_index] , NULL , NULL );
				}
			}
		}
	}
	
	return;

#endif

}

BOOL	CRockClient::ISCheckPlayerNPC( SRpScAppearNPC* aMsg )
{
	BYTE * pBuff = NULL;
	pBuff = (BYTE *)aMsg;

	int nPackSize = 0;
	
#ifdef PACKET_RESIZE	
	
	nPackSize += sizeof( aMsg->theMsgHeader ) + sizeof( DWORD ) + sizeof( WORD ) + sizeof( DWORD ) + sizeof( DWORD );
	nPackSize += sizeof( BYTE ) + sizeof( WORD ) + sizeof( BYTE );

	memset( g_FrontierName, 0, sizeof( g_FrontierName ) );
	memset( g_FrontierPetName, 0, sizeof( g_FrontierPetName ) );

	DWORD aNPCID	 = aMsg->theNPCID;
	WORD  aNPCType	 = aMsg->theNPCType;	
	DWORD aCellIndex = aMsg->theCellIndex;
	DWORD aHP		 = aMsg->theHP;
	DWORD aSP = 0;
	BYTE  aAttackSpeed = aMsg->theAttackSpeed;
	WORD  aMoveSpeed =   aMsg->theMoveSpeed;

	SNPCBaseInfo* npc_Info = NULL;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aNPCType ); 
	
	if( npc_Info == NULL )
		return FALSE;

	aSP = npc_Info->theMaxSP;
	
	if( npc_Info->theRace != NPC_RACE_REALHUMAN )
		return FALSE;	

	float fAngle = 0.0f;
	BYTE  aPriorityType = n_NONEIDType;
	DWORD aPriorityID = 0;	
	DWORD aFrontierID = 0;	

	if( aMsg->theFrontierFlag && !aMsg->thePriorityFlag )
	{
		memcpy( &aPriorityType, &pBuff[nPackSize], sizeof( BYTE ) );
		nPackSize += sizeof(BYTE);
		memcpy( &aPriorityID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);
		memcpy( &fAngle, &pBuff[nPackSize], sizeof( float ) );
		nPackSize += sizeof(float);	
		memcpy( &aFrontierID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);	
		memcpy( g_FrontierName, &pBuff[nPackSize], RP_MAX_FRONTIER_NAME );		
		nPackSize += RP_MAX_FRONTIER_NAME;
		memcpy( g_FrontierPetName, &pBuff[nPackSize], RP_MAX_FRONTIER_PET_NAME );		
		nPackSize += RP_MAX_FRONTIER_PET_NAME;	
	}

	if( aMsg->thePriorityFlag && !aMsg->theFrontierFlag )
	{
		memcpy( &aPriorityType, &pBuff[nPackSize], sizeof( BYTE ) );
		nPackSize += sizeof(BYTE);
		memcpy( &aPriorityID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);
		memcpy( &fAngle, &pBuff[nPackSize], sizeof( float ) );
		nPackSize += sizeof(float);
	}

	int nKeepUpSkillNum = aMsg->theKeepupSkillNum;
	WORD * pKeepUpSkillList = NULL;

	if( nKeepUpSkillNum )
	{
		pKeepUpSkillList = SAFE_NEW_ARRAY( WORD , nKeepUpSkillNum );
		memset( pKeepUpSkillList, 0 , sizeof( WORD ) * nKeepUpSkillNum );
		memcpy( pKeepUpSkillList, &pBuff[nPackSize], sizeof( WORD ) * nKeepUpSkillNum );
		nPackSize += ( sizeof( WORD ) * nKeepUpSkillNum );	
	}
	
	SPlayerNPCData PNPCData = g_Pc_Manager.m_PlayerNPCDataManager.FindPlayerNPCData(aNPCType);
	if( aNPCType != PNPCData.NPC_Code )
	{
		SAFE_DELETE_ARRAY( pKeepUpSkillList );
		return TRUE;
	}
	
	int npc_tbl_index = g_Map.FindNpc( (long)aNPCID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		npc_tbl_index = g_Map.SM__PLAYER_NPC__APPEAR( aNPCType, aNPCID, 
			aCellIndex, aHP, aSP , 0 , fAngle );
		
		if( -1 == npc_tbl_index )
		{
			SAFE_DELETE_ARRAY( pKeepUpSkillList );
			return TRUE;
		}
		
		g_Map.m_Npc[npc_tbl_index].thePath[0] = aCellIndex;	
		
		for( int i = 0; i < nKeepUpSkillNum ; ++i )
		{
			if( pKeepUpSkillList[i] )
			{
				g_Map.m_NpcTable[npc_tbl_index].theKeepupSkill[i] = pKeepUpSkillList[i];
				
				///--JAPAN_BUG_MODIFY
				g_Map.m_Npc[npc_tbl_index].m_curt_event.dKeepSkillCode = pKeepUpSkillList[i];
				// 지속성 스킬 이펙트 추가 [상준]
				g_Particle.m_EffectManager.CreateKeepupSkill( pKeepUpSkillList[i] , 
					&g_Map.m_Npc[npc_tbl_index] , NULL , FALSE );
			}
		}

		///--JAPAN_BUG_MODIFY
		g_Map.m_Npc[npc_tbl_index].m_curt_event.dKeepSkillCode = 0;
		
		// 선점 처리 By wxywxy
		g_Map.m_Npc[npc_tbl_index].SetPriority( aPriorityType , aPriorityID );
		
		//theAttackSpeed; theMoveSpeed;	
		g_Map.m_Npc[npc_tbl_index].m_Weapon_DelayTime = aAttackSpeed * 100; 
		
		// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
		SNPCBaseInfo*	npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[npc_tbl_index].code ); 
		
		if( npc_Info->theMoveSpeed )
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = ( (float)aMoveSpeed / npc_Info->theMoveSpeed );					
			g_Map.m_Npc[npc_tbl_index].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[npc_tbl_index].m_ani_percent );
		}
		else
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = 1.0f;
			g_Map.m_Npc[npc_tbl_index].m_move_percent = 1.0f;
		}
		///-------------------------------------------------------------------------
		///-- Monster ChattingBox	
		///-- 2005.03.07 / Lyul / 리젠
		//RLGS("-- Monster Chatting Box : Regen Monster!");
		SetMonsterMessage( aNPCID, n_Monster_Rengen );
		///-------------------------------------------------------------------------
	}
	else
	{
		//지금 해당 NPC가 존재하고 있다면;;;
		NonPlayer *pNonPlaer = g_Map.GetMonster( (int)aNPCID );
		if(pNonPlaer)
		{
			///--JAPAN_BUG_MODIFY 
			///-- 해당 NPC가 죽어있으면;; 여기서는 죽는 애니메이션이 들어가기전에..
			///-- 리젠이 되어버릴수 있기 때문에...;;; 이런처리를 함 
			if( pNonPlaer->IsRealDie() )
			{
				//맵에서 삭제
				g_Map.SM__NPC__DISAPPEAR( 0, aNPCID );			
				//다시 AppearNPC 호출

				Proc_RpScAppearNPC( aMsg );
				RLG1( " Update Monster(Disappear - Appear) %d", aNPCID );
				return TRUE;				
			}			
			
			// 정보 갱신 
			g_Map.m_NpcTable[npc_tbl_index].lBlock = aCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].lDestBlock = aCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].theHP = aHP;
			g_Map.m_NpcTable[npc_tbl_index].theRealHP = aHP;
			g_Map.m_NpcTable[npc_tbl_index].theSMP = aSP;
			
			if( g_Map.m_NpcTable[npc_tbl_index].lDestBlock != aCellIndex )
			{
				g_Map.m_Npc[npc_tbl_index].SetDirection( D3DXToRadian( fAngle ) );
			}			
		}		
		
		RLG1( " Already Exist NPC ID - %d : ScAppearNPC", aNPCID );

		SAFE_DELETE_ARRAY( pKeepUpSkillList );
		
		return TRUE;		
	}
	
	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
	//......................................................................................................
	npc_tbl_index = g_Map.FindNpc( (long)aNPCID );			// 위에서 새로 만들수 있어서 다시 검색	
	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		SNPCBaseInfo* npc_Info = NULL;		
		NPCTABLE *pTable = g_Map.GetNpcTable( (long)aNPCID );
		
		if( NULL != pTable )
		{
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
			
			if( NULL != npc_Info )
			{
				if( npc_Info->theRightEff != -1 )
				{
					g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff ,
						( Character * ) &g_Map.m_Npc[npc_tbl_index] , NULL , NULL );
				}
			}
		}
	}	

	SAFE_DELETE_ARRAY( pKeepUpSkillList );
	
	return TRUE;

#else	
	
	SNPCBaseInfo* npc_Info;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aMsg->theNPCType ); 
	
	if( npc_Info == NULL )
		return FALSE;
	
	if( npc_Info->theRace != NPC_RACE_REALHUMAN )
		return FALSE;
	
	
	SPlayerNPCData PNPCData = g_Pc_Manager.m_PlayerNPCDataManager.FindPlayerNPCData(aMsg->theNPCType);
	if( aMsg->theNPCType != PNPCData.NPC_Code )
	{
		return TRUE;
	}
	
	int npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		npc_tbl_index = g_Map.SM__PLAYER_NPC__APPEAR( aMsg->theNPCType, aMsg->theNPCID, 
			aMsg->theCellIndex, aMsg->theHP, aMsg->theSP, 0 , aMsg->theAngle );
		
		if( -1 == npc_tbl_index )
		{
			return TRUE;
		}
		
		g_Map.m_Npc[npc_tbl_index].thePath[0] = aMsg->theCellIndex;	
		
		for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			if( aMsg->theKeepupSkill[i] )
			{
				g_Map.m_NpcTable[npc_tbl_index].theKeepupSkill[i] = aMsg->theKeepupSkill[i];
				
				///--JAPAN_BUG_MODIFY
				g_Map.m_Npc[npc_tbl_index].m_curt_event.dKeepSkillCode = aMsg->theKeepupSkill[i];
				// 지속성 스킬 이펙트 추가 [상준]
				g_Particle.m_EffectManager.CreateKeepupSkill( aMsg->theKeepupSkill[i] , 
					&g_Map.m_Npc[npc_tbl_index] , NULL , FALSE );
			}
		}

		///--JAPAN_BUG_MODIFY
		g_Map.m_Npc[npc_tbl_index].m_curt_event.dKeepSkillCode = 0;
		
		// 선점 처리 By wxywxy
		g_Map.m_Npc[npc_tbl_index].SetPriority( aMsg->thePriorityType , aMsg->thePriorityID );
		
		//theAttackSpeed; theMoveSpeed;	
		g_Map.m_Npc[npc_tbl_index].m_Weapon_DelayTime = aMsg->theAttackSpeed * 100; 
		
		// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
		SNPCBaseInfo*	npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[npc_tbl_index].code ); 
		
		if( npc_Info->theMoveSpeed )
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = ( (float)aMsg->theMoveSpeed / npc_Info->theMoveSpeed );					
			g_Map.m_Npc[npc_tbl_index].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[npc_tbl_index].m_ani_percent );
		}
		else
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = 1.0f;
			g_Map.m_Npc[npc_tbl_index].m_move_percent = 1.0f;
		}
		///-------------------------------------------------------------------------
		///-- Monster ChattingBox	
		///-- 2005.03.07 / Lyul / 리젠
		//RLGS("-- Monster Chatting Box : Regen Monster!");
		SetMonsterMessage( aMsg->theNPCID, n_Monster_Rengen );
		///-------------------------------------------------------------------------
	}
	else
	{
		//지금 해당 NPC가 존재하고 있다면;;;
		NonPlayer *pNonPlaer = g_Map.GetMonster( (int)aMsg->theNPCID );
		if(pNonPlaer)
		{
			///--JAPAN_BUG_MODIFY 
			///-- 해당 NPC가 죽어있으면;; 여기서는 죽는 애니메이션이 들어가기전에..
			///-- 리젠이 되어버릴수 있기 때문에...;;; 이런처리를 함 
			if( pNonPlaer->IsRealDie() )
			{
				//맵에서 삭제
				g_Map.SM__NPC__DISAPPEAR( 0, aMsg->theNPCID );			
				//다시 AppearNPC 호출

				Proc_RpScAppearNPC( aMsg );
				RLG1( " Update Monster(Disappear - Appear) %d", aMsg->theNPCID );
				return TRUE;				
			}			
			
			// 정보 갱신 
			g_Map.m_NpcTable[npc_tbl_index].lBlock = aMsg->theCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].lDestBlock = aMsg->theCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].theHP = aMsg->theHP;
			g_Map.m_NpcTable[npc_tbl_index].theRealHP = aMsg->theHP;
			g_Map.m_NpcTable[npc_tbl_index].theSMP = aMsg->theSP;
			
			if( g_Map.m_NpcTable[npc_tbl_index].lDestBlock != aMsg->theCellIndex )
			{
				g_Map.m_Npc[npc_tbl_index].SetDirection( D3DXToRadian( aMsg->theAngle ) );
			}			
		}		
		
		RLG1( " Already Exist NPC ID - %d : ScAppearNPC", aMsg->theNPCID );
		return TRUE;		
	}
	
	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
	//......................................................................................................
	npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID );			// 위에서 새로 만들수 있어서 다시 검색	
	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		SNPCBaseInfo* npc_Info = NULL;		
		NPCTABLE *pTable = g_Map.GetNpcTable( (long)aMsg->theNPCID );
		
		if( NULL != pTable )
		{
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
			
			if( NULL != npc_Info )
			{
				if( npc_Info->theRightEff != -1 )
				{
					g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff ,
						( Character * ) &g_Map.m_Npc[npc_tbl_index] , NULL , NULL );
				}
			}
		}
	}	
	
	return TRUE;

#endif

}

void	CRockClient::Proc_RpScDisappearNPC( SRpScDisappearNPC* aMsg )
{
	//	RLG1( "*****ScDisappearNPC : [%u]", aMsg->theNPCID );
	
	int npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		if( g_Pc.ISCheckDisappearHoly( aMsg ) )
			return;
		else
		{
			RLG1( " Nothing NPC ID - %d : SRpScNPCMove", aMsg->theNPCID );
			
			return;
		}
	}	

	SNPCBaseInfo* npc_Info = NULL ; 

	NPCTABLE *pTable = g_Map.GetNpcTable( (long)aMsg->theNPCID );
	
	if( NULL != pTable )
	{
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
		
		if( NULL != npc_Info )
		{
			if( npc_Info->theRightEff != -1 )
			{
				// 게임상에서 완전히 사라질때 나오는 이펙트 .. 
				g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theNeckEff , ( Character * ) &g_Map.m_Npc[npc_tbl_index] , NULL , NULL );
			}
		}
	}

	//	g_Map.SM__NPC__DISAPPEAR( g_Map.m_NpcTable[npc_tbl_index].index, aMsg->theNPCID );
	if( g_Map.m_Npc[npc_tbl_index].m_State.nHolyState == nHolyState_Holy )
	{
		// 소환 해제 되는 신수이면 처리하기 서버 메세지 플래그 추가 By wxywxy
		if( aMsg->theCancelCallFlag )
		{
			int index = g_Pc_Manager.m_HolyManager.GetDeleteEffect( g_Map.m_Npc[ npc_tbl_index ].m_State.BaseCode ,
				g_Map.m_NpcTable[ npc_tbl_index ].theLevel );
			
			D3DXVECTOR3	vPos;
			
			vPos = D3DXVECTOR3( g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._41 , 
				g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._42 + 15.0f, 
				g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._43 );
			
			
			g_Particle.SetEffSetCon( 0.0f , index , ESLT_POS , 5.1f , -1 , -1 , vPos );
			
			g_Sound.Play( 295 , vPos.x , vPos.y , vPos.z );
		}
		
		g_Map.SM__HOLY__DISAPPEAR( 0, aMsg->theNPCID );
		
	}
	else
	{
		g_Map.SM__NPC__DISAPPEAR( 0, aMsg->theNPCID );
	}
	
	
	if( NonPlayerInfoWnd.thePickedNpcID == npc_tbl_index )
	{
		NonPlayerInfoWnd.thePickedNpcID = -1;
		nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		
		if(g_NonPlayerInfoAdd2Wnd.IsVisible)
		{
			g_NonPlayerInfoAdd2Wnd.Close();
		}
	}					
	
	return;
}

///----------------------------------------------------------------------------
///-- Monster Move Message
///----------------------------------------------------------------------------
///-- Monster가 이동할 때 오는 Message
///-- 임시구현 : 정확한 확률 계산 미정.
///--          : 
void	CRockClient::Proc_RpScNPCMove( SRpScNPCMove* aMsg )
{
	//	RLG3( "*****ScNPCMove : ID = %d, distance = %d, thePath[theDistance] = %u", 
	//			aMsg->theNPCID, aMsg->theDistance, aMsg->thePath[aMsg->theDistance-1]);
	
	int npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		//		RLG1( " Nothing NPC ID - %d: SRpScNPCMove", aMsg->theNPCID );
		return;
	}	
	
	if( g_Map.m_NpcTable[ npc_tbl_index ].code >= c_NotMoveNPCCode_Min && 
		g_Map.m_NpcTable[ npc_tbl_index ].code <= c_NotMoveNPCCode_Max   )
	{
		return;
	}
	
	///-------------------------------------------------------------------------
	///-- Monster ChattingBox	
	///-- 2005.03.07 / Lyul
#ifdef C_SUBJOB_RENEW
	
	if( !(g_Map.m_NpcTable[ npc_tbl_index ].code > 5000 && g_Map.m_NpcTable[ npc_tbl_index ].code < 5500 ) ) 
	{
		if( ( (float)rand() / (float)RAND_MAX ) > nRui->m_BugMop.fMopWait )
		{
			///-- 달림
			if( aMsg->theMoveType == 0 )
			{
				SetMonsterMessage( aMsg->theNPCID, n_Monster_Move);
			}
			///-- 느린 이동
			else
			{
				SetMonsterMessage( aMsg->theNPCID, n_Monster_Wait);
			}
		}
	}

#else 
	if( ( (float)rand() / (float)RAND_MAX ) > nRui->m_BugMop.fMopWait )
	{
		///-- 달림
		if( aMsg->theMoveType == 0 )
		{
			SetMonsterMessage( aMsg->theNPCID, n_Monster_Move);
		}
		///-- 느린 이동
		else
		{
			SetMonsterMessage( aMsg->theNPCID, n_Monster_Wait);
		}
	}
#endif 

	///-------------------------------------------------------------------------
	
	int idx = 0;

	for( idx = 0; idx < 6; ++idx )
	{
		g_Map.m_Npc[npc_tbl_index].thePath[idx] = aMsg->thePath[idx];
		/*
		if(g_Map.m_nPickedNpc != -1 )
		{
			
			if( g_Map.m_Npc[g_Map.m_nPickedNpc].thePath[0] != 0 )
			{
				int x = g_Map.m_Npc[g_Map.m_nPickedNpc].thePath[0] % g_Pc.m_cSize;
				int y = g_Map.m_Npc[g_Map.m_nPickedNpc].thePath[0] / g_Pc.m_cSize;
				PRINT_DLG_LOG("Pick mob pos X:%d Z:%d " ,x, y);
			}
		}
		*/
	}
	
	g_Map.SetMapData( CLEAR_BLOCK, npc_tbl_index, g_Map.m_NpcTable[npc_tbl_index].lDestBlock, false );
	
	//	int curtPath = g_Map.m_Npc[npc_tbl_index].theCurtPath;
	g_Map.m_Npc[npc_tbl_index].theDestPath = aMsg->theDistance - 1;
	g_Map.m_Npc[npc_tbl_index].theCurtPath = 0;
	
	EVENT_DATA_INFO	data;	
	//	data.x = g_Map.m_Npc[npc_tbl_index].thePath[curtPath] % g_Pc.m_cSize;
	//	data.y = g_Map.m_Npc[npc_tbl_index].thePath[curtPath] / g_Pc.m_cSize;
	data.x = g_Map.m_Npc[npc_tbl_index].thePath[0] % g_Pc.m_cSize;
	data.y = g_Map.m_Npc[npc_tbl_index].thePath[0] / g_Pc.m_cSize;
	data.type = SM_MOVE_BROADCAST;
	
	g_Map.m_Npc[npc_tbl_index].m_PreMTime = g_nowTime;
	
	// 임시 이동 테스트 ( 다른 유저와 NPC )
	if( g_Map.m_Npc[npc_tbl_index].m_bIsLoaded == true )
		g_Map.m_Npc[npc_tbl_index].PushQueue( data );
	
	//	g_Map.m_NpcTable[npc_tbl_index].lDestBlock = (data.y*g_Pc.m_cSize)+data.x;
	//	g_Map.SetMapData( NPC_BLOCK, npc_tbl_index, g_Map.m_NpcTable[npc_tbl_index].lDestBlock, true );
	
	g_Map.m_Npc[npc_tbl_index].m_State.bWalk = aMsg->theMoveType;	
	
	//	if(g_Map.m_NpcTable[npc_tbl_index].theHP == 0 && g_Map.m_NpcTable[npc_tbl_index].theRealHP == 0   )
	//	{
	//		g_Map.m_NpcTable[npc_tbl_index].theHP  = g_Map.m_NpcTable[npc_tbl_index].theMaxHP;
	//		g_Map.m_NpcTable[npc_tbl_index].theRealHP = g_Map.m_NpcTable[npc_tbl_index].theMaxHP;
	//	}
	//	
	
	//dongtest
	//	ZeroMemory(&nRui->m_BugMop, sizeof(nRui->m_BugMop));	
	
	return;
}


void	CRockClient::Proc_RpScAnswerWarp( SRpScAnswerWarp* aMsg )
{
	//	RLG5( "\nScAnswerWarp : theWarpCell = %u, Cell=(%d, %d), theWarpMap = %u, theResult = %d", 
	//			aMsg->theWarpCell, aMsg->theWarpCell%g_Pc.m_cSize, aMsg->theWarpCell/g_Pc.m_cSize, aMsg->theWarpMap, aMsg->theResult );
	
	if( aMsg->theResult == n_Success )			// 맵 이동 가능 
	{
		LoadingBarWnd.m_IsMakeImg = true;
		LoadingBarWnd.Set_warpmap_id( aMsg->theWarpMap );
		LoadingBarWnd.m_pMsgScAnswerWarp = SAFE_NEW( SRpScAnswerWarp );		
		*(LoadingBarWnd.m_pMsgScAnswerWarp) = *(aMsg);
#ifdef DECO_RENEWAL_GUILD_MJH

	std::vector < SFMemberInfo > :: iterator ir;							
			
	ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();
	
	for( ; ir != g_FrontierMemberListWnd.m_FrontierMemberInfoList.end() ; ++ir )
	{
		if( Rstrcmp( (*ir).Name, nRui->thePcParam.PCName ) == 0 )
		{
			(*ir).TheMap= aMsg->theWarpMap;
			break;
		}
	}	
	g_FrontierMainWnd.RefreshFrontierMyInfo();
	g_FrontierMainWnd.RefreshFrontierMyFrontier();
	g_FrontierMemberListWnd.RefreshMemberInfoList();

#endif // DECO_RENEWAL_GUILD_MJH
	}
	else
	{
		RLGS("!맵 이동 불가능");
	}	
	return;
}

void	CRockClient::DelayProc_RpScAnswerWarp( SRpScAnswerWarp* aMsg )
{	
	if( aMsg->theResult == n_Success )			// 맵 이동 가능 
	{	
		
				
		m_bIsDrawFullScreenUI = false;
		
		g_Pc.GetPlayer()->IsMyPlayerRender = TRUE;	
		
		g_BZ_RoomListWnd.Close();		
		g_BZ_WaitingRoomWnd.Close();
		
		BeginLoadingBarThread();
		
		g_Pc.SetupTempPc( g_Pc.m_MainPC );
		g_Pc.m_TempPC.lCurMap = aMsg->theWarpMap;
		g_Pc.m_TempPC.lBlock = aMsg->theWarpCell;
		g_Pc.m_TempPC.lDestBlock = aMsg->theWarpCell;
		
		if( g_RockClient.MapChange( aMsg->theWarpMap ) == false )
		{
			if( STILL_ACTIVE == g_CLoadingBarThread.getExitCode() )
			{
				g_CLoadingBarThread.Stop();	
			}			
			SEND_DEBUG_LOG("맵 데이타 오류");
			// 맵 로딩후엔 다 쓴 메세지 없애기 
			SAFE_DELETE( LoadingBarWnd.m_pMsgScAnswerWarp );
			return;
		}
		
		g_Pc.GetPlayer()->SetWearItem();
		
		if( aMsg->theFlag == n_FightZone )		// n_FightZone PVP 존, n_NormalZone	일반 존 
		{	
			g_Pc.GetPlayer()->CM__READY__WARP( g_nowTime, n_FightZone );
			g_Pc.GetPlayer()->theFightZoneReady = false;
			if(g_BZ_WaitingRoomWnd.m_ZoneViewType == n_BattleZone_NormalZone )
			{
				g_Pc.GetPlayer()->IsMyPlayerRender = TRUE;
			}
			else
			{
				g_Pc.GetPlayer()->IsMyPlayerRender = FALSE;
			}
		}
		else
		{
			g_BZ_RoomListWnd.Close();	
			g_BZ_WaitingRoomWnd.Close();
			g_BZ_GameResultWnd.Close();
			
			g_Pc.GetPlayer()->CM__READY__WARP( g_nowTime, n_NormalZone );		 				
		}
		
		// 다른 유저 및 몬스터 표시창 
		if( NonPlayerInfoWnd.IsVisible )	
		{ 
			nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
		
		if( NpcChatWnd.IsVisible )
		{
			NpcChatWnd.CloseNpcChatWnd( true );			
		}
		
		// 맵 로딩후엔 다 쓴 메세지 없애기 
		SAFE_DELETE( LoadingBarWnd.m_pMsgScAnswerWarp );
		




		///-- 로딩바가 종료할때까지 기다려 안전적인 처리
		LoadingBarSafeStopAfterProcess();
#ifdef BATTLEMAP_OPTIMIZE
		switch(aMsg->theWarpMap)
		{
		case c_RockMap_Argore_Heights:
		case c_RockMap_Ground_of_Curse62:
		case c_RockMap_Hell_of_Silence:
		case c_RockMap_Before_Hell_of_Fire:
		case c_RockMap_Gate_for_Hell_of_Fire:
		case c_RockMap_Gate_for_Hell_of_Silence:
		case c_RockMap_Before_Hell_of_Silence:
		case c_RockMap_Hell_of_Fire:
		case c_RockMap_Gate_for_Hell_of_Pain:
		case c_RockMap_Befor_Hell_of_Pain:
		case c_RockMap_Hell_of_Pain:
		case c_RockMap_Gate_for_Hell_of_Seal:
		case c_RockMap_Before_Hell_of_Seal:
		case c_RockMap_Hell_of_Seal:
		case c_RockMap_Gate_for_Hell_of_Ruin:
		case c_RockMap_Before_Hell_of_Ruin:
		case c_RockMap_Hell_of_Ruin:
			g_OptionWnd.CompulsionFrm();
			g_OptionWnd.SetCompulsion(true);
		default:
			if( g_OptionWnd.GetCompulsion() == TRUE )
			{
				g_OptionWnd.RollBackCompulsionFrm();
				g_OptionWnd.SetCompulsion(false);
			}
		    break;
		}
#endif 

	}
	else
	{
		RLGS("!맵 이동 불가능");
		// 맵 로딩후엔 다 쓴 메세지 없애기 
		SAFE_DELETE( LoadingBarWnd.m_pMsgScAnswerWarp );
	}	
	return;
}


void	CRockClient::Proc_RpScSpelling( SRpScSpelling* aMsg )
{
	//	RLGS( "ScSpelling" );
	map < DWORD, long >::iterator iPos = 
		g_Pc.GetSkillSystem()->UseSkillList.find((DWORD)aMsg->theCode);	
	if(iPos != g_Pc.GetSkillSystem()->UseSkillList.end())
	{
		(*iPos).second = g_nowTime;		
	}
	
	EVENT_DATA_INFO chant_event;
	chant_event.type = SM_USE_SKILL;		
	chant_event.status = SM_CHANT_READY;
	chant_event.count = aMsg->theCode;		
	
	chant_event.motion = g_Pc.GetPlayer()->SetChantMotion( SM_CHANT_READY, 0 );		// 처음 영창 시작이므로...^ ^;	

	g_Pc.GetPlayer()->PushQueue( chant_event );	
	
	return;
}


void CRockClient::Proc_RpScSeeSpelling( SRpScSeeSpelling* aMsg )
{
	//	RLG2( "ScSeeSpelling : PcID = %d, ComboCount = %d", aMsg->thePCID , aMsg->theComboCount );
#ifdef UPDATE_NPC_SPELLING	
	if( aMsg->theSpellingPCType == n_PC ) 
	{
		int spellingIdx = g_Pc.FindUser( aMsg->thePCID );
		if( spellingIdx == MAX_USER_INTABLE )
			return;
		
	
		EVENT_DATA_INFO chant_event;
		
		chant_event.type   = SM_USE_SKILL;		
		chant_event.status = SM_CHANT_READY;
		chant_event.count  = aMsg->theCode;
		chant_event.motion = g_Pc.m_Pc[spellingIdx].SetChantMotion( SM_CHANT_READY, aMsg->theSpellingCount );		// 처음 영창 시작이므로...^ ^;
		
		g_Pc.m_Pc[spellingIdx].PushQueue(chant_event);	
	}
	else if( aMsg->theSpellingPCType == n_NPC || aMsg->theSpellingPCType == n_PET )
	{
		SetMonsterMessage( aMsg->thePCID , n_Monster_Skill , 10 );	
	}

#else
				
		int spellingIdx = g_Pc.FindUser( aMsg->thePCID );
		if( spellingIdx == MAX_USER_INTABLE )
			return;
		
		EVENT_DATA_INFO chant_event;
		
		chant_event.type   = SM_USE_SKILL;		
		chant_event.status = SM_CHANT_READY;
		chant_event.count  = aMsg->theCode;
		chant_event.motion = g_Pc.m_Pc[spellingIdx].SetChantMotion( SM_CHANT_READY, aMsg->theSpellingCount );		// 처음 영창 시작이므로...^ ^;
		
		g_Pc.m_Pc[spellingIdx].PushQueue(chant_event);	
#endif		 

	return;
}

void CRockClient::Proc_RpScArcherReady(SRpScArcherReady *aMsg)
{
	//	RLGS("Proc_RpScArcherReady\n");
	switch( aMsg->theResult )
	{
		// 공격대상유저가 없는 경우(ID가 잘못되거나, 로그아웃하거나 스킬테이블에 정의된 대상과 다른 경우 )
	case n_NO_TARGET_USER:
		
		// 공격대상거리가 너무 먼 상태
	case n_TOO_LONG_DISTANCE:
		//..................................................................................................
		// 재 공격
		//..................................................................................................
		{
			RLGS("..................................................................................................");
			RLG2( "n_TOO_LONG_DISTANCE ArcherReady %d , %d ", g_Pc.GetPlayer()->m_save_event.tbl_Idx ,
				g_Pc.GetPlayer()->m_save_event.race   );
			RLGS("..................................................................................................");
			
			EVENT_DATA_INFO attack_event;
			
			attack_event.type    = CM_MOVE_ATTACK;
			attack_event.motion  = n_Attack9_Basic0;
			attack_event.tbl_Idx = g_Pc.GetPlayer()->m_save_event.tbl_Idx;
			
			//...............................................................................................
			// NPC	
			//...............................................................................................
			if( g_Pc.GetPlayer()->m_save_event.race < 0 )
			{
				attack_event.race	= g_Map.m_NpcTable[attack_event.tbl_Idx].race;
				attack_event.index	= g_Map.m_NpcTable[attack_event.tbl_Idx].index;
				attack_event.unique	= g_Map.m_NpcTable[attack_event.tbl_Idx].lUnique;
				attack_event.x		= g_Map.m_NpcTable[attack_event.tbl_Idx].lDestBlock%(g_Map.m_cSize*2);
				attack_event.y		= g_Map.m_NpcTable[attack_event.tbl_Idx].lDestBlock/(g_Map.m_cSize*2);
				attack_event.vPos.x	= g_Map.m_Npc[attack_event.tbl_Idx].GetPosTM()._41;
				attack_event.vPos.y	= g_Map.m_Npc[attack_event.tbl_Idx].GetPosTM()._42;
				attack_event.vPos.z	= g_Map.m_Npc[attack_event.tbl_Idx].GetPosTM()._43;
				attack_event.cell	= g_Map.m_NpcTable[attack_event.tbl_Idx].lDestBlock; 
				g_Pc.GetPlayer()->PushQueue( attack_event, TRUE );
			}
			//...............................................................................................
			// PC	
			//...............................................................................................
			else
			{
				attack_event.race	= g_Pc.m_PcTable[attack_event.tbl_Idx].char_info.race;
				attack_event.index	= g_Pc.m_PcTable[attack_event.tbl_Idx].nIndex;
				attack_event.unique	= g_Pc.m_PcTable[attack_event.tbl_Idx].lUnique;
				attack_event.x		= g_Pc.m_PcTable[attack_event.tbl_Idx].lDestBlock%(g_Map.m_cSize*2);
				attack_event.y		= g_Pc.m_PcTable[attack_event.tbl_Idx].lDestBlock/(g_Map.m_cSize*2);
				attack_event.vPos.x	= g_Pc.m_Pc[attack_event.tbl_Idx].GetPosTM()._41;
				attack_event.vPos.y	= g_Pc.m_Pc[attack_event.tbl_Idx].GetPosTM()._42;
				attack_event.vPos.z	= g_Pc.m_Pc[attack_event.tbl_Idx].GetPosTM()._43;
				attack_event.cell	= g_Pc.m_PcTable[attack_event.tbl_Idx].lDestBlock;
				g_Pc.GetPlayer()->PushQueue( attack_event, TRUE );
			}
			
		}
		return;
		
		// 패시브 스킬이 없다.
	case n_HAVENOT_KEEPUP_SKILL:
		RLGS( "n_HAVENOT_KEEPUP_SKILL ArcherReady\n");
		return;
		
		// 아처 무기가 아님..
	case n_WRONG_WEAPON:
		RLGS( "n_WRONG_WEAPON ArcherReady\n");
		return;
		
		// 타겟 사망
	case n_DIED_TARGET:
		RLGS( "n_DIED_TARGET ArcherReady\n");
		return;
		
		// 타켓이 이동중.
	case n_WARP_TARGET:
		RLGS( "n_WARP_TARGET ArcherReady\n");
		return;
		
	case n_Fail:
		
		RLGS( "Fail ArcherReady\n");
		
		return;
		
	case n_Success:
		{
			//			RLGS("Proc_RpScArcherReady->n_Success");
			EVENT_DATA_INFO ready_event;
			
			ready_event = g_Pc.GetPlayer()->m_curt_event;
			
			ready_event.type   = SM_BOW_ATTACK;		
			ready_event.status = SM_BOW_READY;
			ready_event.count  = 0;
			ready_event.motion = n_Attack10_BowReady0;
			
			g_Pc.GetPlayer()->PushQueue( ready_event, TRUE );
			
			int Level = g_Pc.GetPlayer()->theActionSystem.GetSkillLevel( g_Pc.GetPlayer()->theActionSystem.theComboSkillCode1 );
			
			///-- 아쳐바를 뛰우는 루틴
			g_ArcherBar.Start( Level );		
		}
		
		return;
	}
	
}

void CRockClient::Proc_RpScSeeArcherReady(SRpScSeeArcherReady *aMsg)
{
	EVENT_DATA_INFO ready_event;
	
	int ArcherIndex = g_Pc.FindUser( aMsg->thePCID );
	
	if( ArcherIndex == MAX_USER_INTABLE )
	{
		RLGS("!Proc_SRpScSeeArcherReady Archer 읍~다");
		
		return;		
	}	
	
	ready_event.Clear();
	
	ready_event.type   = SM_BOW_ATTACK;		
	ready_event.status = SM_BOW_READY;
	ready_event.count  = 0;
	ready_event.motion = n_Attack10_BowReady0;
	
	int Index = 0;
	// PC 공격
	if( aMsg->theTargetType == n_PC )
	{
		// 플레이어 공격
		if( g_Pc.m_MainPC.lUnique == aMsg->theTarget )
		{
			// Proc_RpScArcherReady 서 처리
			return;
		}
		// 다른 유저 공격
		else
		{
			Index = g_Pc.FindUser( aMsg->theTarget );
			
			// 유저 못찾아요

			if( Index == MAX_USER_INTABLE )
			{
				RLGS("!Proc_SRpScSeeArcherReady n_PC 읍~다");
				
				return;
			}
			
			ready_event.tbl_Idx = Index;
			ready_event.race	= g_Pc.m_PcTable[Index].char_info.race;
			ready_event.index	= g_Pc.m_PcTable[Index].nIndex;
			ready_event.unique  = g_Pc.m_PcTable[Index].lUnique;
			ready_event.x		= g_Pc.m_PcTable[Index].lDestBlock%(g_Map.m_cSize*2);
			ready_event.y		= g_Pc.m_PcTable[Index].lDestBlock/(g_Map.m_cSize*2);
			ready_event.vPos.x  = g_Pc.m_Pc[Index].GetPosTM()._41;
			ready_event.vPos.y  = g_Pc.m_Pc[Index].GetPosTM()._42;
			ready_event.vPos.z  = g_Pc.m_Pc[Index].GetPosTM()._43;
		}
		
	}
	// NPC 공격
	else
	{
		Index = g_Map.FindNpc( aMsg->theTarget );
		
		if( Index == MAX_NPC_INTABLE )
		{
			RLGS("!Proc_SRpScSeeArcherReady NPC 읍~다");
			
			return;
		}
		
		ready_event.tbl_Idx = Index;
		ready_event.race	= g_Map.m_NpcTable[Index].race;
		ready_event.index	= g_Map.m_NpcTable[Index].index;
		ready_event.unique  = g_Map.m_NpcTable[Index].lUnique;
		ready_event.x	    = g_Map.m_NpcTable[Index].lDestBlock%(g_Map.m_cSize*2);
		ready_event.y	    = g_Map.m_NpcTable[Index].lDestBlock/(g_Map.m_cSize*2);
		ready_event.vPos.x  = g_Map.m_Npc[Index].GetPosTM()._41;
		ready_event.vPos.y  = g_Map.m_Npc[Index].GetPosTM()._42;
		ready_event.vPos.z  = g_Map.m_Npc[Index].GetPosTM()._43;		
	}
	
	g_Pc.m_Pc[ArcherIndex].SetDirection( ready_event.vPos.x, ready_event.vPos.z );
	
	g_Pc.m_Pc[ArcherIndex].PushQueue( ready_event, TRUE );
}

// 스킬 사용 결과값
void	CRockClient::Proc_RpScSkill( SRpScSkill* aMsg )
{
	// 
	if( aMsg->theSkillCode == 4521 )
	{
#ifdef ROCKCLIENT_DEV		
		SystemPrint( n_DarkRed, n_SystemMsg, "Return Skill code : %d", aMsg->theSkillCode );
#endif
		return;
	}
	
	//by simwoosung - 스펠 스킬코드 초기화	
	g_SlotDelayProc.SetSpellSkillCode( 0 );

	if( GetSkillProtocolResult( aMsg->theResult ) == false )
	{

#ifdef ROCKCLIENT_DEV		
		SystemPrint( n_DarkRed, n_SystemMsg, _RT("!ScSkill Error Result : %d"), aMsg->theResult );
		RLG1( "errorResult = %d", aMsg->theResult );	
#endif
		

		g_Pc.GetSkillSystem()->theIsUsedSkill = FALSE;
		
		map < DWORD, long >::iterator iPos = 
			g_Pc.GetSkillSystem()->UseSkillList.find((DWORD)aMsg->theSkillCode);	
		if(iPos != g_Pc.GetSkillSystem()->UseSkillList.end())
		{
			g_Pc.GetSkillSystem()->UseSkillList.erase(iPos);
		}
		
		///--JAPAN_BUG_MODIFY
		if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
		{
			if( g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_READY ||
				g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_SPELLING )
			{
				g_Pc.GetPlayer()->m_curt_event.Clear();
			}
		}
		
		return;
	}	
	
	int skilledIdx = 0;
	
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr(aMsg->theSkillCode); 
	if( skill_Info == NULL )
		return;	
	
	EVENT_DATA_INFO skill_event;
	skill_event.type	= SM_USE_SKILL;	
	
	///-- 휴식 스킬 적용시에는...
	if( aMsg->theSkillCode >= c_SkillCode_BreathControl_Min && aMsg->theSkillCode <= c_SkillCode_BreathControl_Max ||				// 호흡 조절 
		aMsg->theSkillCode >= c_SkillCode_Meditation_Min && aMsg->theSkillCode <= c_SkillCode_Meditation_Max )				// 명상 스킬 
	{
		skill_event.status  = SM_RESTSKILL_SITDOWN;	
	}
	else
	{
		skill_event.status  = SM_USE_SKILL;	
	}
#ifdef NPC_MOVE_TO_SKILL
	if( skill_Info->theAddEffect == n_AddEff_Rush )
	{
		skill_event.type = SM_MOVE_SKILL;
		skill_event.status = SM_MOVE_SKILL;
		g_Pc.GetPlayer()->m_State.nActiveState = nActiveTypeMove;

	}
#endif 

	skill_event.motion	= skill_Info->theActionMotion; 
	skill_event.count	= aMsg->theSkillCode;
	
	if( aMsg->theHP && g_Pc.m_MainPC.char_info.theRealHP != aMsg->theHP )
	{
		int change_hp = aMsg->theHP - g_Pc.m_MainPC.char_info.theRealHP;		
		g_Pc.m_MainPC.char_info.theRealHP = aMsg->theHP;
		g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
		///-- 사용된 스킬이 체력을 깍는 스킬이라면
		
		if( skill_Info->theWasteHP )
		{
			nRui->thePcParam.Stat1.theHP += change_hp;											
		}			
	}		
	
	nRui->thePcParam.Stat1.theSP = aMsg->theSP;
	nRui->thePcParam.Stat1.theMP = aMsg->theMP;
	
	nRui->thePcParam.Stat1.theGage = aMsg->theWasteGage;
	g_Pc.GetPlayer()->theActionSystem.SetComboGauge();
	
	if( aMsg->theTargetType == n_Cell )
	{
		skill_event.race	= n_Cell;
		skill_event.cell	= aMsg->theTarget;
		skill_event.x		= aMsg->theTarget % g_Pc.m_cSize;
		skill_event.y		= aMsg->theTarget / g_Pc.m_cSize;
		skill_event.vPos.x = skill_event.x * (TILE_SIZE/2);
		skill_event.vPos.y = g_Pc.GetMapHeight((int)skill_event.x, (int)skill_event.y );
		skill_event.vPos.z = skill_event.y * (TILE_SIZE/2);
	}
	else
	{			
		if( aMsg->theTargetType == n_PC )
		{
			// 자기 자신한테 사용하는 스킬일 경우 모션 없이 수치 및 이펙트 효과 적용 
			if( g_Pc.m_MainPC.lUnique == aMsg->theTarget )
			{
				skill_event.tbl_Idx = MAX_USER_INTABLE;
				skill_event.race	= g_Pc.m_MainPC.char_info.race;
				skill_event.index	= g_Pc.m_MainPC.nIndex;
				skill_event.unique = g_Pc.m_MainPC.lUnique;
				skill_event.x		= g_Pc.m_MainPC.lDestBlock%(g_Map.m_cSize*2);
				skill_event.y		= g_Pc.m_MainPC.lDestBlock/(g_Map.m_cSize*2);
				skill_event.vPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
				skill_event.vPos.y = g_Pc.GetPlayer()->GetPosTM()._42;
				skill_event.vPos.z = g_Pc.GetPlayer()->GetPosTM()._43;
			}
			else
			{
				skilledIdx = g_Pc.FindUser(aMsg->theTarget);
				
				///--JAPAN_BUG_MODIFY
				if( skilledIdx == MAX_USER_INTABLE )
					return;
				
				skill_event.tbl_Idx = skilledIdx;
				skill_event.race	= c_PcRace_User;
				skill_event.index	= g_Pc.m_PcTable[skilledIdx].nIndex;
				skill_event.unique	= g_Pc.m_PcTable[skilledIdx].lUnique;
				skill_event.x		= g_Pc.m_PcTable[skilledIdx].lDestBlock%(g_Map.m_cSize*2);
				skill_event.y		= g_Pc.m_PcTable[skilledIdx].lDestBlock/(g_Map.m_cSize*2);
				skill_event.vPos.x = g_Pc.m_Pc[skilledIdx].GetPosTM()._41;
				skill_event.vPos.y = g_Pc.m_Pc[skilledIdx].GetPosTM()._42;
				skill_event.vPos.z = g_Pc.m_Pc[skilledIdx].GetPosTM()._43;
			}
		}
		else if( aMsg->theTargetType == n_NPC )
		{
			skilledIdx = g_Map.FindNpc(aMsg->theTarget);
			
			if( skilledIdx == MAX_NPC_INTABLE )
				return;
			
			skill_event.tbl_Idx = skilledIdx;
			skill_event.race	= c_NpcRace_Mob;
			//			skill_event.race	= g_Map.m_NpcTable[skilledIdx].race;
			skill_event.index	= g_Map.m_NpcTable[skilledIdx].index;
			skill_event.unique	= g_Map.m_NpcTable[skilledIdx].lUnique;
			skill_event.x		= g_Map.m_NpcTable[skilledIdx].lDestBlock%(g_Map.m_cSize*2);
			skill_event.y		= g_Map.m_NpcTable[skilledIdx].lDestBlock/(g_Map.m_cSize*2);
			skill_event.vPos.x = g_Map.m_Npc[skilledIdx].GetPosTM()._41;
			skill_event.vPos.y = g_Map.m_Npc[skilledIdx].GetPosTM()._42;
			skill_event.vPos.z = g_Map.m_Npc[skilledIdx].GetPosTM()._43;
			// 임시 스킬 테스트
			// g_Particle.m_EffectManager.Create( EF_FIREBALL , g_Pc.GetPlayer() , &g_Map.m_Npc[skilledIdx] );
		} 
	}
	
	// 광역 스킬의 경우 스킬의 Unique ID 를 가지고 각각 컨트롤 한다. 
	if( aMsg->theSkillinMapCode )
	{
		skill_event.skill_unique = aMsg->theSkillinMapCode;
		g_Particle.m_EffectManager.PushReadyLandEffect( aMsg->theSkillinMapCode );
	}
	
	// 실제적인 스킬 부과효과 수치에 따라 상태및 데미지등을 적용시킨다 
	if( aMsg->theTargetNum )
	{
		skill_event.number = aMsg->theTargetNum;
		skill_event.target_list = SAFE_NEW_ARRAY( _STargetInfo, aMsg->theTargetNum );		
		
		//		DebugPrint("skill_event.target_list = %p\n", skill_event.target_list);
		memcpy( skill_event.target_list, aMsg->theTargetList, sizeof(_STargetInfo)*aMsg->theTargetNum );
		//		_STargetInfo* tg_info = (_STargetInfo*)skill_event.target_list;			// 확인을 위해...사용...
		
		int num = 0; 
		_STargetInfo* tg_Info = NULL;
		
		tg_Info = (_STargetInfo*) skill_event.target_list;
		
		BOOL	InsertKeepUpSkill = FALSE;
		
		for( num = 0; num < skill_event.number; ++num )
		{	
			
			//by dongs 몬스터 가 죽기 직전에 소리를 질러보아요 .. 
			if( (tg_Info[ num ].theTargetType == n_NPC) && tg_Info[num].theDieFlag ) 
			{	
				//	if(((float)rand() / (float)RAND_MAX) > nRui->m_BugMop.fMopDie )
				{
					SetMonsterMessage(tg_Info[num].theTargetID , n_Monster_Die);
				}					
			}				
			
			g_Pc.theSkillSystem.SetDamageValue( aMsg->theSkillCode, &tg_Info[num] );				
			//................................................................................................
			//
			//................................................................................................
			InsertKeepUpSkill = FALSE;
			
			if( tg_Info[num].theAddEfectFlag && skill_Info->theAddEffectTime != 0 )
			{		
				InsertKeepUpSkill = TRUE;
			}
			else 
			{
				for( int i = 0 ; i < 3 ; ++i )
				{
					if( skill_Info->theEffectInfo[i].theEffectType == n_ZERO )
					{
						continue;
					}
					
					if( skill_Info->theEffectInfo[i].theKeepupTime == 0 )
						continue;
					
					if( tg_Info[num].theKeepupFlag[ i ] == 0 )
						continue;
					
					InsertKeepUpSkill = TRUE;
				}
			}
			
			if( InsertKeepUpSkill == TRUE )
			{
				if( tg_Info[ num ].theTargetType == n_PC )
				{
					if( g_Pc.m_MainPC.lUnique == tg_Info[ num ].theTargetID )
					{
						g_Pc.GetPlayer()->m_KeepUpChecker.SetCode( aMsg->theSkillCode );
					}
					else
					{
						int Index = g_Pc.FindUser( tg_Info[ num ].theTargetID );
						///--JAPAN_BUG_MODIFY
						if( Index == MAX_USER_INTABLE )
							continue;
						
						g_Pc.m_Pc[ Index ].m_KeepUpChecker.SetCode( aMsg->theSkillCode );
					}
				}
				else if( tg_Info[ num ].theTargetType == n_NPC )
				{
					int Index = g_Map.FindNpc( tg_Info[ num ].theTargetID );
					
					if( Index == MAX_NPC_INTABLE )
						continue;
					
					g_Map.m_Npc[ Index ].m_KeepUpChecker.SetCode( aMsg->theSkillCode );
				}
			}				
			//................................................................................................
		}			
		
	}
	
	///-- 영창 모션중이냐...
	BOOL IsSpellMotion = FALSE;
	
	///--현재 모션이 스펠시전 모션일 경우에는 모션을 초기화시켜 준다.
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
	{
		if( g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_READY ||
			g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_SPELLING )
		{
			IsSpellMotion = TRUE;
		}
	}	
	
	if( ( g_Pc.GetPlayer()->m_curt_event.type == SM_MOVE_BROADCAST ) &&
		( skill_Info->theSkillType2 == n_GoodSupport &&				
		skill_Info->theActTarget == n_AT_SELF && 
		skill_Info->theToggleFlag == 0 ) )
	{
		///-- By simwoosung
		///-- 스킬사용시 이전모든 액션을 처리해버리면...
		///-- 공격스킬 사용후 - 버프를 걸어 공격 애니메이션 처리를 무시해버리는 경우가
		///-- 있을수있어 악용될 가능성이 높다.
		///-- SetUnderAttackEventMgr 함수는 현재 들어온 이벤트관련부분만 직접 처리하는 
		///-- 함수이므로 AllActQueue() 함수를 호출하지 않는게 좋을듯하다.
		///-- 이후 문제가 생길 가능성을 대비해 일단 주석처리만 한다.			
		///-- g_Pc.GetPlayer()->AllActQueue();
		if( IsSpellMotion )
		{
			g_Pc.GetPlayer()->PushQueue( skill_event );
		}
		else
		{
			g_Pc.GetPlayer()->SetUnderAttackEventMgr( skill_event );
		}
	}
	//이벤트 스킬은 한번에뿌려준다 . - 이쪽은 보니까 비약스킬을 가지고 한거 같은데... 
	else if( skill_Info->theSkillType2 == n_GoodSupport &&				
			     skill_Info->theActTarget == n_AT_SELF && skill_Info->theToggleFlag == 0  &&
				 skill_Info->theActionMotion == 0  && 
				 ( 8000 <= skill_Info->theSkillCode && 8100 >= skill_Info->theSkillCode ) )
	{
		///-- g_Pc.GetPlayer()->AllActQueue();
		if( IsSpellMotion )
		{
			g_Pc.GetPlayer()->PushQueue( skill_event );
		}
		else
		{
			g_Pc.GetPlayer()->SetUnderAttackEventMgr( skill_event );
		}
	}
	///-- 자기자신에게 거는 좋은 버프스킬중 모션이 0인것은 바로 실행시켜준다.
	///-- 예: 장착 아이템을 통해 생기는 버프
	else if( skill_Info->theSkillType2 == n_GoodSupport &&				
			     skill_Info->theActTarget == n_AT_SELF && 
				 skill_Info->theTarget == n_T_SELF && 
				 skill_Info->theToggleFlag == 0  &&
				 skill_Info->theNeedSpelling == 0 &&
				 skill_Info->theActionMotion == 0 )
	{
		///-- 이부분에 AllActQueue()를 넣어버리면 
		///-- 장착 아이템을 통한 버프를 이용해 현재 적용중인 모든 스킬을 일괄실행 
		///-- 시켜버리게되기 때문에 안된다.			
		if( IsSpellMotion )
		{
			g_Pc.GetPlayer()->PushQueue( skill_event );
		}
		else
		{
			g_Pc.GetPlayer()->SetUnderAttackEventMgr( skill_event );
		}
	}
	else
		g_Pc.GetPlayer()->PushQueue( skill_event );		
	
	//스킬 지연 시간 표시
	MainMenuWnd.SetSkillDelayTime( aMsg->theSkillCode );	
	
	/*
	RLGS("Player Skill...................................................................................End");
	*/
	return;
}


void	CRockClient::Proc_RpScSeeSkill( SRpScSeeSkill* aMsg )
{

//	n_Summoner
	if( aMsg->theSkillCode == 4521 )
	{
#ifdef ROCKCLIENT_DEV		
		SystemPrint( n_DarkRed, n_SystemMsg, "Return Skill code : %d", aMsg->theSkillCode );
#endif
		return;
	}	
/*
	if( aMsg->theSkillCode ==8723 || aMsg->theSkillCode ==8730)
	{
		SystemPrint( n_DarkRed, n_SystemMsg, "Return Skill code : %d", aMsg->theSkillCode );
	}
*/
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr(aMsg->theSkillCode); 

	
#ifdef _DEBUG
	PRINT_DLG_LOG("NPC Use SKILL %d  [%s]" , aMsg->theSkillCode , skill_Info->theSkillName);
#endif 
	
	if( skill_Info == NULL )
		return;

	EVENT_DATA_INFO skill_event;

	
	skill_event.type = SM_USE_SKILL;
	
	if( (aMsg->theSkillCode >= c_SkillCode_BreathControl_Min && aMsg->theSkillCode <= c_SkillCode_BreathControl_Max) ||				// 호흡 조절 
		(aMsg->theSkillCode >= c_SkillCode_Meditation_Min && aMsg->theSkillCode <= c_SkillCode_Meditation_Max) )				// 명상 스킬 
	{
		skill_event.status  = SM_RESTSKILL_SITDOWN;	
	}
	else
	{
		skill_event.status  = SM_USE_SKILL;	
	}
	
#ifdef NPC_MOVE_TO_SKILL
	//부가 효과가 중 돌진 부가 효과를 가지고 있으면
	if( skill_Info->theAddEffect == n_AddEff_Rush  || 
		skill_Info->theAddEffect == 38 )
	{
		skill_event.type = SM_MOVE_SKILL;
		skill_event.status = SM_MOVE_SKILL;
	}
#endif 


	skill_event.motion	= skill_Info->theActionMotion;
	skill_event.count	= aMsg->theSkillCode;	
	skill_event.critical = aMsg->theCritical;	
	
	int skillIdx = 0;
	int skilledIdx = 0;
	
	// 광역 스킬 공격에 대한 데미지 받는 유저들 따로 처리 
	if( aMsg->theTargetType == n_SKILL )
	{
		// aMsg->theTarget;			// 스킬 UID 
		// aMsg->theSkillinMapCode; // 스킬 UID 
		
		int damage = 0;
		int realHP = 0;
		Character *pAttackChr = NULL;
		
		///-- 이곳에서는 실질적인 HP갱신을 하지않음
		if( aMsg->theAttackType == n_PC )
		{			
			///-- 플레이어가 스킬을 시전할 경우

			if( g_Pc.m_MainPC.lUnique == aMsg->theAttackID )
			{
				pAttackChr = g_Pc.GetPlayer();		
			}
			else
			{
				///-- 일반 유저가 스킬을 시전할 경우

				skillIdx = g_Pc.FindUser( aMsg->theAttackID );
				
				if( skillIdx != MAX_USER_INTABLE )
				{			
					pAttackChr = &g_Pc.m_Pc[skillIdx];	
				}				
			}
		}///-- NPC나 Pet이 스킬을 시전할 경우

		else if( aMsg->theAttackType == n_NPC || aMsg->theAttackType == n_PET )
		{
			skillIdx = g_Map.FindNpc( aMsg->theAttackID );		
			
			if( skillIdx != MAX_NPC_INTABLE )
			{
				pAttackChr = &g_Map.m_Npc[skillIdx];	
			}
		}
			
		
		int i = 0;
		int targetIdx = 0;
		int skillcase = 0;
		BYTE effect_type = 0;
		int effect_value = 0;
		Character *pTargetChr = NULL;
		
		
		for( int tgNum = 0; tgNum < aMsg->theTargetNum; ++tgNum )
		{
			_STargetInfo* tg_Info = (_STargetInfo*) aMsg->theTargetList;
			pTargetChr = NULL;
			
			//나 죽을때 ? 소리 
			//by dongs 죽을소리내어 죽는다 .. 100%로 소리를 낸다. 
			
			if( (aMsg->theAttackType == n_PC) && (g_Pc.m_MainPC.lUnique == aMsg->theAttackID) 
				&& tg_Info[tgNum].theDieFlag )
			{
				SetMonsterMessage(tg_Info[tgNum].theTargetID, n_Monster_Die );
			}
			
			if( tg_Info )
			{
				for( i = 0; i < 3; ++i )
				{
					effect_value = tg_Info[tgNum].theEffectValue[i];
					effect_type = skill_Info->theEffectInfo[i].theEffectType;
					
					skillcase = effect_type / 30;
					if( skillcase == 1 )
					{
						effect_type -= 30;
					}
					else if( skillcase == 2 )
					{
						effect_type -= 60;
					}		
					
					switch(effect_type)
					{
					case n_HPAdd:
						//					case n_VitalAdd:
						//					case n_AttackPowerAdd:
						//					case n_MagicPowerAdd:
					case n_AttackBlowAdd:
					case n_MagicBlowAdd:
						{
							if( tg_Info[tgNum].theTargetType == n_PC )
							{
								if( g_Pc.m_MainPC.lUnique == tg_Info[tgNum].theTargetID )
								{
									pTargetChr = g_Pc.GetPlayer();									
									realHP = g_Pc.m_MainPC.char_info.theRealHP;
								}
								else
								{
									///--JAPAN_BUG_MODIFY
									targetIdx = g_Pc.FindUser( tg_Info[tgNum].theTargetID );
									if( targetIdx == MAX_USER_INTABLE )
										break;
						
									pTargetChr = &g_Pc.m_Pc[targetIdx];
									realHP = g_Pc.m_PcTable[targetIdx].char_info.theRealHP;
								}
							}
							else if( tg_Info[tgNum].theTargetType == n_NPC )
							{
								targetIdx = g_Map.FindNpc( tg_Info[tgNum].theTargetID );		
								if( targetIdx == MAX_NPC_INTABLE )
									break;
								
								pTargetChr = &g_Map.m_Npc[targetIdx];
								realHP = g_Map.m_NpcTable[targetIdx].theRealHP;
							}
							
							if( tg_Info[tgNum].theDieFlag )
							{
								damage = effect_value; 
							}
							else
							{
								damage = realHP - effect_value; 
							}
						}
						break;
					}				
				}
				
				if( pTargetChr )
				{
					
					// 데미지 표시 및 이펙트, 죽거나 맞는 이벤트 처리 
					if( aMsg->theTarget )
					{	// 광역 스킬 데미지 처리 ( + 맞을 때 이펙트 처리 )
						g_Particle.m_EffectManager.DamageLandEffect( pTargetChr, damage, aMsg->theSkillCode , ( BYTE )tg_Info[tgNum].theDieFlag );
					}
					else
					{	// 지속적인 데미지 처리 ( 맞는 이펙트 처리 X )
						g_Particle.m_EffectManager.DamageSkillEffect( pTargetChr, damage, aMsg->theSkillCode , ( BYTE )tg_Info[tgNum].theDieFlag );
					}
					
					g_Pc.theSkillSystem.SetDamageValue( aMsg->theSkillCode, &tg_Info[tgNum] );					
					g_Pc.theSkillSystem.UseSkillTargetInfoMgr( pAttackChr, tg_Info[tgNum], aMsg->theSkillCode, FALSE );
					g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( pAttackChr, tg_Info[tgNum], aMsg->theSkillCode, FALSE );
		
				}				
			}
		}					
		
		return;			
	}
	else if( aMsg->theTargetType == n_Cell )
	{
		skill_event.race	= n_Cell;
		skill_event.cell	= aMsg->theTarget;
		skill_event.x		= aMsg->theTarget % g_Pc.m_cSize;
		skill_event.y		= aMsg->theTarget / g_Pc.m_cSize;
		skill_event.vPos.x = skill_event.x * (TILE_SIZE/2);
		skill_event.vPos.y = g_Pc.GetMapHeight((int)skill_event.x, (int)skill_event.y );
		skill_event.vPos.z = skill_event.y * (TILE_SIZE/2);		
	}
	else if( aMsg->theTargetType == n_PC )	///-- 타겟 대상이 유저일 경우
	{
		if( 0 == aMsg->theTargetNum )
		{
			///--PRINT_DLG_LOG("타겟타입 n_PC 스킬 타겟수 0가 들어옴");
			return;
		}	
		
		if( g_Pc.m_MainPC.lUnique == aMsg->theTarget )	///-- 타겟 대상이 자신일 경우
		{
			skill_event.tbl_Idx = MAX_USER_INTABLE;
			skill_event.race	= g_Pc.m_MainPC.char_info.race;
			skill_event.index	= g_Pc.m_MainPC.nIndex;
			skill_event.unique = g_Pc.m_MainPC.lUnique;
#ifdef NPC_MOVE_TO_SKILL
			if(skill_event.type ==  SM_MOVE_SKILL )
			{
				int x = aMsg->theMP%(g_Map.m_cSize*2);
				int y = aMsg->theMP/(g_Map.m_cSize*2);
				skill_event.x		= aMsg->theMP%(g_Map.m_cSize*2);
				skill_event.y		= aMsg->theMP/(g_Map.m_cSize*2);
				skill_event.vPos.x = skill_event.x * (TILE_SIZE/2);
				skill_event.vPos.y = g_Pc.GetMapHeight((int)skill_event.x, (int)skill_event.y );
				skill_event.vPos.z = skill_event.y * (TILE_SIZE/2);
			}
			else 
#endif 
			{
				skill_event.x		= g_Pc.m_MainPC.lDestBlock%(g_Map.m_cSize*2);
				skill_event.y		= g_Pc.m_MainPC.lDestBlock/(g_Map.m_cSize*2);
				skill_event.vPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
				skill_event.vPos.y = g_Pc.GetPlayer()->GetPosTM()._42;
				skill_event.vPos.z = g_Pc.GetPlayer()->GetPosTM()._43;
			}

		}
		else
		{
			skilledIdx = g_Pc.FindUser(aMsg->theTarget);	///-- 타겟 대상이 유저일 경우	
			
			///--JAPAN_BUG_MODIFY
			if( skilledIdx == MAX_USER_INTABLE )
				return;
			
			skill_event.tbl_Idx = skilledIdx;
			skill_event.race	= c_PcRace_User;
			skill_event.index	= g_Pc.m_PcTable[skilledIdx].nIndex;
			skill_event.unique	= g_Pc.m_PcTable[skilledIdx].lUnique;
#ifdef NPC_MOVE_TO_SKILL
			if(skill_event.type ==  SM_MOVE_SKILL )
			{
				skill_event.x		= aMsg->theMP%(g_Map.m_cSize*2);
				skill_event.y		= aMsg->theMP/(g_Map.m_cSize*2);
				skill_event.vPos.x = skill_event.x * (TILE_SIZE/2);
				skill_event.vPos.y = g_Pc.GetMapHeight((int)skill_event.x, (int)skill_event.y );
				skill_event.vPos.z = skill_event.y * (TILE_SIZE/2);
			}
			else 
#endif 
			{
				skill_event.x		= g_Pc.m_PcTable[skilledIdx].lDestBlock%(g_Map.m_cSize*2);
				skill_event.y		= g_Pc.m_PcTable[skilledIdx].lDestBlock/(g_Map.m_cSize*2);
				skill_event.vPos.x = g_Pc.m_Pc[skilledIdx].GetPosTM()._41;
				skill_event.vPos.y = g_Pc.m_Pc[skilledIdx].GetPosTM()._42;
				skill_event.vPos.z = g_Pc.m_Pc[skilledIdx].GetPosTM()._43;
			}
		}
	}
	else if( aMsg->theTargetType == n_NPC )		///-- 타겟 대상이 몬스터일 경우
	{
		if( 0 == aMsg->theTargetNum )
		{
			///--PRINT_DLG_LOG("타겟타입 n_NPC 스킬 타겟수 0가 들어옴");
			return;
		}	
		
		skilledIdx = g_Map.FindNpc(aMsg->theTarget);
		
		if( skilledIdx == MAX_NPC_INTABLE )
			return;
		
		skill_event.tbl_Idx = skilledIdx;
		skill_event.race	= c_NpcRace_Mob;
		//			skill_event.race	= g_Map.m_NpcTable[skilledIdx].race;
		skill_event.index	= g_Map.m_NpcTable[skilledIdx].index;
		skill_event.unique	= g_Map.m_NpcTable[skilledIdx].lUnique;
		skill_event.x		= g_Map.m_NpcTable[skilledIdx].lDestBlock%(g_Map.m_cSize*2);
		skill_event.y		= g_Map.m_NpcTable[skilledIdx].lDestBlock/(g_Map.m_cSize*2);
		skill_event.vPos.x = g_Map.m_Npc[skilledIdx].GetPosTM()._41;
		skill_event.vPos.y = g_Map.m_Npc[skilledIdx].GetPosTM()._42;
		skill_event.vPos.z = g_Map.m_Npc[skilledIdx].GetPosTM()._43;
		
		// 임시 스킬 테스트
		//g_Particle.m_EffectManager.Create( EF_FIREBALL , g_Pc.GetPlayer() , &g_Map.m_Npc[skilledIdx] );
	}
	
	// 광역 스킬의 경우 스킬의 Unique ID 를 가지고 각각 컨트롤 한다. 
	if( aMsg->theSkillinMapCode )
	{
		skill_event.skill_unique = aMsg->theSkillinMapCode;
		g_Particle.m_EffectManager.PushReadyLandEffect( aMsg->theSkillinMapCode );
	}
	
	Character *pDest = NULL;
	///-- 바로 실행해야 하는 여부확인
	BOOL	IsNowAct = FALSE;
	BOOL	IsNonNPC = FALSE;
	
	// 실제적인 스킬 부과효과 수치에 따라 상태및 데미지등을 적용시킨다 
	
	if( aMsg->theTargetNum )	///-- 공격 대상이 다수가 존재할 경우
	{
		skill_event.number = aMsg->theTargetNum;
		skill_event.target_list = SAFE_NEW_ARRAY( _STargetInfo , aMsg->theTargetNum );		
		
		memcpy( skill_event.target_list, aMsg->theTargetList, sizeof(_STargetInfo)*aMsg->theTargetNum );		
		
		int num = 0; 
		_STargetInfo* tg_Info = NULL;
		
		tg_Info = (_STargetInfo*) skill_event.target_list;
		
		BOOL	InsertKeepUpSkill = FALSE;
		
		if( aMsg->theAttackType == n_PC )
		{
			skillIdx = g_Pc.FindUser( aMsg->theAttackID );		
			///--JAPAN_BUG_MODIFY
			if( skillIdx == MAX_USER_INTABLE )
			{
				IsNowAct = TRUE;
			}
		}
		else if( aMsg->theAttackType == n_NPC || aMsg->theAttackType == n_PET )
		{
			skillIdx = g_Map.FindNpc( aMsg->theAttackID );		
			if( skillIdx == MAX_NPC_INTABLE )
			{
				IsNowAct = TRUE;
			}
			
			if(IsNonPlayerCode(g_Map.m_NpcTable[skillIdx].code))
			{
				IsNowAct = TRUE;
				IsNonNPC = TRUE;
			}

			//몬서터 변신 ~~ 
			if( skill_Info->theAddEffect == n_AddEff_Transformer && 
				tg_Info[0].theAddEfectFlag && aMsg->theAttackID == tg_Info[0].theTargetID )
			{
				SNPCBaseInfo* npc_Info = NULL;
				
				NPCTABLE *pTable = g_Map.GetNpcTable(aMsg->theAttackID);

				D3DXVECTOR3 vec; 
				vec.x = g_Map.m_Npc[skillIdx].GetPosTM()._41;
				vec.y = g_Map.m_Npc[skillIdx].GetPosTM()._42 + g_Map.m_Npc[skillIdx].GetBoundMaxPos().y * 0.5f ;
				vec.z = g_Map.m_Npc[skillIdx].GetPosTM()._43;
				
				g_Particle.m_fAddSize = 1; 
				g_Particle.SetEffSetCon( 0.0f , 1175 , ESLT_POS , 5.0f , -1 , -1  , vec , 0.0f );

				if( NULL != pTable )
				{
					g_Map.SM__NPC__CHANGE(aMsg->theAttackID , pTable->code+1 );
				}

				return;
			}

		}
				
		for( num = 0; num < skill_event.number; ++num )
		{	
			
			g_Pc.theSkillSystem.SetDamageValue( aMsg->theSkillCode, &tg_Info[num], aMsg->theAttackType, aMsg->theAttackID );
			
			//................................................................................................
			//
			//................................................................................................
			InsertKeepUpSkill = FALSE;
			
			if( tg_Info[num].theAddEfectFlag && skill_Info->theAddEffectTime != 0 )
			{		
				InsertKeepUpSkill = TRUE;
			}
			else 
			{
				for( int i = 0 ; i < 3 ; ++i )
				{
					if( skill_Info->theEffectInfo[i].theEffectType == n_ZERO )
					{
						continue;
					}
					
					if( skill_Info->theEffectInfo[i].theKeepupTime == 0 )
						continue;
					
					if( tg_Info[num].theKeepupFlag[ i ] == 0 )
						continue;
					
					InsertKeepUpSkill = TRUE;
				}
			}
			
			if( InsertKeepUpSkill == TRUE )
			{
				if( tg_Info[ num ].theTargetType == n_PC )
				{
					if( g_Pc.m_MainPC.lUnique == tg_Info[ num ].theTargetID )
					{
						g_Pc.GetPlayer()->m_KeepUpChecker.SetCode( aMsg->theSkillCode );
					}
					else
					{
						///--JAPAN_BUG_MODIFY
						int Index = g_Pc.FindUser( tg_Info[ num ].theTargetID );						
						if( Index == MAX_USER_INTABLE )
							continue;
						
						g_Pc.m_Pc[ Index ].m_KeepUpChecker.SetCode( aMsg->theSkillCode );
					}
				}
				else if( tg_Info[ num ].theTargetType == n_NPC )
				{
					int Index = g_Map.FindNpc( tg_Info[ num ].theTargetID );
					
					if( Index == MAX_NPC_INTABLE )
						continue;
#ifdef HOLY_SKILLICON_MODIFY
					g_Map.m_NpcTable[ Index ].theHolyLevel = g_Map.m_NpcTable[ Index ].theLevel; 
#endif
					g_Map.m_Npc[ Index ].m_KeepUpChecker.SetCode( aMsg->theSkillCode );					
				}
			}			
			
			if( IsNowAct == TRUE )
			{
				if( tg_Info[ num ].theTargetType == n_PC )
				{
					if( g_Pc.m_MainPC.lUnique == tg_Info[ num ].theTargetID )
					{
						pDest = (Character *)g_Pc.GetPlayer();
						if( tg_Info[ num ].theDieFlag )
						{
							EVENT_DATA_INFO underattack_event;
							underattack_event.type = SM_STATE_DIE;
							underattack_event.loop = false;					
							underattack_event.motion = n_Die9;
							g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );							
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
							g_Pc.GetPlayer()->SetRealDie(TRUE, aMsg->theAttackType, aMsg->theAttackID);

							nRui->thePcParam.Stat1.theHP = 0;
							g_Pc.m_MainPC.char_info.theHP = 0;
							g_Pc.m_MainPC.char_info.theRealHP = 0;
						}							
						
						g_Pc.theSkillSystem.UseSkillTargetInfoMgr( NULL , tg_Info[num] , 
							skill_event.count, TRUE );					
						g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( NULL, 
							tg_Info[num] , skill_event.count, TRUE );
					}
					else
					{
						int Index = g_Pc.FindUser( tg_Info[ num ].theTargetID );						
						///--JAPAN_BUG_MODIFY
						if( Index == MAX_USER_INTABLE )
							continue;
						
						pDest = (Character *)&g_Pc.m_Pc[Index];
						if( tg_Info[ num ].theDieFlag )
						{
							EVENT_DATA_INFO underattack_event;
							underattack_event.type = SM_STATE_DIE;
							underattack_event.loop = false;					
							underattack_event.motion = n_Die9;
							g_Pc.m_Pc[Index].PushQueue( underattack_event, TRUE );
							g_Pc.m_Pc[Index].m_lPreTime = g_nowTime;
							g_Pc.m_Pc[Index].SetRealDie(TRUE);

							g_Pc.m_PcTable[Index].char_info.theHP = 0;
							g_Pc.m_PcTable[Index].char_info.theRealHP = 0;
						}	
						
						g_Pc.theSkillSystem.UseSkillTargetInfoMgr( NULL , tg_Info[num] , 
							skill_event.count, TRUE );					
						g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( NULL , 
							tg_Info[num] , skill_event.count, TRUE );
					}
					
					if(IsNonNPC && g_RockClient.GetLanguage() == eLang_Korea)
					{						
						SNPCBaseInfo* npc_Info;		//-- Table
						npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[skillIdx].code );
											
						if( npc_Info == NULL || skill_Info == NULL )
						{
							return;
						}
						
						if(pDest == g_Pc.GetPlayer())
						{
							SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_GIVE_UP_ORDER) , 
								npc_Info->theName,  skill_Info->theSkillName);		
						}
					}
				}	
				else if( tg_Info[ num ].theTargetType == n_NPC )
				{
					int Index = g_Map.FindNpc( tg_Info[ num ].theTargetID );					
					if( Index == MAX_NPC_INTABLE )
						continue;
					
					pDest = (Character *)&g_Map.m_Npc[Index];
					if( tg_Info[ num ].theDieFlag )
					{
						EVENT_DATA_INFO underattack_event;
						underattack_event.type = SM_STATE_DIE;
						underattack_event.loop = false;					
						
						if( g_Map.m_Npc[Index].m_State.nCharacterType == nCharacterTypePlayerNPC )
						{
							underattack_event.motion = n_Die9;
						}
						else
						{
							underattack_event.motion = n_NPC_Die0;
						}
						
						g_Map.m_Npc[Index].PushQueue( underattack_event, TRUE );
						g_Map.m_Npc[Index].m_lPreTime = g_nowTime;
						g_Map.m_Npc[Index].SetRealDie(TRUE);

						g_Map.m_NpcTable[Index].theHP = 0;
						g_Map.m_NpcTable[Index].theRealHP = 0;
					}						
					
					g_Pc.theSkillSystem.UseSkillTargetInfoMgr( NULL , tg_Info[num] , 
						skill_event.count, TRUE );					
					g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( NULL , 
						tg_Info[num] , skill_event.count, TRUE );	
					
				}	
				
			}		
			//................................................................................................
		}
	}
	
	if( IsNowAct )
	{
		return;
	}
	//스킬 사용 주체자 -유져-
	if( aMsg->theAttackType == n_PC )	

	{						
		// unique 를 가지고 테이블 위치를 알아낸다.
		skillIdx = g_Pc.FindUser( aMsg->theAttackID );		
		///--JAPAN_BUG_MODIFY
		if( skillIdx == MAX_USER_INTABLE )
		{
			return;
		}
		
		if( g_Pc.m_Pc[skillIdx].m_bIsLoaded == true )
		{
			if( g_Pc.m_PcTable[skillIdx].char_info.theRealHP != aMsg->theHP )
			{
				int change_hp = aMsg->theHP - g_Pc.m_PcTable[skillIdx].char_info.theRealHP;	
				g_Pc.m_PcTable[skillIdx].char_info.theRealHP = aMsg->theHP;
				g_Pc.m_Pc[skillIdx].m_lPreTime = g_nowTime;
				///-- 사용된 스킬이 체력을 깍는 스킬이라면

				if( skill_Info->theWasteHP )
				{
					g_Pc.m_PcTable[skillIdx].char_info.theHP += change_hp;
				}
			}
			
			if( g_Pc.m_PcTable[skillIdx].nIndex == n_Millena_Man || g_Pc.m_PcTable[skillIdx].nIndex == n_Millena_Woman )
			{
				g_Pc.m_PcTable[skillIdx].char_info.theSMP = aMsg->theSP;
			}
			else if( g_Pc.m_PcTable[skillIdx].nIndex == n_Rain_Man || g_Pc.m_PcTable[skillIdx].nIndex == n_Rain_Woman )
			{
				g_Pc.m_PcTable[skillIdx].char_info.theSMP = aMsg->theMP;				
			}
			
			g_Pc.m_PcTable[skillIdx].char_info.theWasteGage = aMsg->theWasteGage;			
			
#ifdef NPC_MOVE_TO_SKILL
			if(skill_event.type == SM_MOVE_SKILL)
			{
				g_Pc.m_Pc[skillIdx].m_State.nActiveState = nActiveTypeMove; 
			}
#endif 
			///-- 영창 모션중이냐...
			BOOL IsSpellMotion = FALSE;

			///--현재 모션이 스펠시전 모션일 경우에는 모션을 초기화시켜 준다.
			if( g_Pc.m_Pc[skillIdx].m_curt_event.type == SM_USE_SKILL )
			{
				if( g_Pc.m_Pc[skillIdx].m_curt_event.status == SM_CHANT_READY ||
					g_Pc.m_Pc[skillIdx].m_curt_event.status == SM_CHANT_SPELLING )
				{
					IsSpellMotion = TRUE;
				}
			}	
			
			if( ( g_Pc.m_Pc[skillIdx].m_curt_event.type == SM_MOVE_BROADCAST ) &&
				( skill_Info->theSkillType2 == n_GoodSupport &&				
				  skill_Info->theActTarget == n_AT_SELF && 
				  skill_Info->theToggleFlag == 0 ) )
			{
				///-- By simwoosung
				///-- 스킬사용시 이전모든 액션을 처리해버리면...
				///-- 공격스킬 사용후 - 버프를 걸어 공격 애니메이션 처리를 무시해버리는 경우가
				///-- 있을수있어 악용될 가능성이 높다.
				///-- SetUnderAttackEventMgr 함수는 현재 들어온 이벤트관련부분만 직접 처리하는 
				///-- 함수이므로 AllActQueue() 함수를 호출하지 않는게 좋을듯하다.
				///-- 이후 문제가 생길 가능성을 대비해 일단 주석처리만 한다.
				///--g_Pc.m_Pc[skillIdx].AllActQueue();
				if( IsSpellMotion )
				{
					g_Pc.m_Pc[skillIdx].PushQueue( skill_event );
				}
				else
				{
					g_Pc.m_Pc[skillIdx].SetUnderAttackEventMgr( skill_event );
				}				
			}
			///-- 이쪽은 보니까 비약스킬을 가지고 한거 같은데... 
			else if( skill_Info->theSkillType2 == n_GoodSupport &&				
				     skill_Info->theActTarget == n_AT_SELF && skill_Info->theToggleFlag == 0  &&
				     skill_Info->theActionMotion == 0  && 
				    (8000 <= skill_Info->theSkillCode && 8100 >= skill_Info->theSkillCode ) )
			{
				///--g_Pc.m_Pc[skillIdx].AllActQueue();
				if( IsSpellMotion )
				{
					g_Pc.m_Pc[skillIdx].PushQueue( skill_event );
				}
				else
				{
					g_Pc.m_Pc[skillIdx].SetUnderAttackEventMgr( skill_event );
				}
			}
			///-- 자기자신에게 거는 좋은 버프스킬중 모션이 0인것은 바로 실행시켜준다.
			///-- 예: 장착 아이템을 통해 생기는 버프
			else if( skill_Info->theSkillType2 == n_GoodSupport &&				
					 skill_Info->theActTarget == n_AT_SELF && 
					 skill_Info->theTarget == n_T_SELF && 
					 skill_Info->theToggleFlag == 0  &&
					 skill_Info->theNeedSpelling == 0 &&
					 skill_Info->theActionMotion == 0 )
			{
				///-- 이부분에 AllActQueue()를 넣어버리면 
				///-- 장착 아이템을 통한 버프를 이용해 현재 적용중인 모든 스킬을 일괄실행 
				///-- 시켜버리게되기 때문에 안된다.
				if( IsSpellMotion )
				{
					g_Pc.m_Pc[skillIdx].PushQueue( skill_event );
				}
				else
				{
					g_Pc.m_Pc[skillIdx].SetUnderAttackEventMgr( skill_event );
				}
			}
			else
			{	
				
				g_Pc.m_Pc[skillIdx].PushQueue( skill_event );						
			}
		}
	}
	// 스킬 주체자가 NPC나 Pet 일 경우
	else if( aMsg->theAttackType == n_NPC || aMsg->theAttackType == n_PET )
	{
		skillIdx = g_Map.FindNpc( aMsg->theAttackID );		
		if( skillIdx == MAX_NPC_INTABLE )
		{
			return;
		}
		
		if( skill_event.race == n_Cell )
		{
			if( skill_event.cell == g_Map.m_NpcTable[skillIdx].lDestBlock )
			{
				D3DXVECTOR3	vPos = D3DXVECTOR3( g_Map.m_Npc[skillIdx].GetPosTM()._41,
												0.0f,
												g_Map.m_Npc[skillIdx].GetPosTM()._43 );

				skill_event.vPos = vPos + ( 20.0f * g_Map.m_Npc[skillIdx].m_vDir );
			}
		}
		
#ifdef NPC_MOVE_TO_SKILL
		if(skill_event.type == SM_MOVE_SKILL)
		{
			g_Map.m_Npc[skillIdx].m_State.nActiveState = nActiveTypeMove; 
		}
#endif 

		if( !skill_Info->theNeedSpelling && ( ( (float)rand() / (float)RAND_MAX ) > nRui->m_BugMop.fMopSkill) )
		{			
			SetMonsterMessage( aMsg->theAttackID, n_Monster_Skill );	
		}
		// 여기서 처리해 버리면 Damage 표시 방식일때 애니메이션 처리뒤 
		// HP에서 Damage를 빼주기 때문에 두번 빼주는 듯한 결과 발생...일단 막는것으로 처리 T,.T 
		if( g_Map.m_NpcTable[skillIdx].theRealHP != aMsg->theHP )
		{
			//대화형 NPC 죽지 않게 처리하고 에너지를 못깍게 강제적올 HP MAX 값을 넣어준다 . 
			if( g_Map.m_NpcTable[skillIdx].code >= 5000 && g_Map.m_NpcTable[skillIdx].code< 5500 ) 
			{
				int change_hp = aMsg->theHP - g_Map.m_NpcTable[skillIdx].theRealHP;
				g_Map.m_NpcTable[skillIdx].theRealHP = g_Map.m_NpcTable[skillIdx].theMaxHP;
				g_Map.m_Npc[skillIdx].m_lPreTime = g_nowTime;
				
				///-- 사용된 스킬이 체력을 깍는 스킬이라면
				if( skill_Info->theWasteHP )
				{
					g_Map.m_NpcTable[skillIdx].theHP = g_Map.m_NpcTable[skillIdx].theMaxHP;
				}

			}
			else 
			{
				int change_hp = aMsg->theHP - g_Map.m_NpcTable[skillIdx].theRealHP;
				g_Map.m_NpcTable[skillIdx].theRealHP = aMsg->theHP;
				g_Map.m_Npc[skillIdx].m_lPreTime = g_nowTime;
				
				///-- 사용된 스킬이 체력을 깍는 스킬이라면
				
				if( skill_Info->theWasteHP )
				{
					g_Map.m_NpcTable[skillIdx].theHP += change_hp;
				}
			}

		}
		
		g_Map.m_NpcTable[skillIdx].theSMP = aMsg->theSP;
		
		//현재 이동중이고 들어온 타입이 보조계 마법이고 자신이게 쏘는것이라면 바로 적용 하라 .. 
		if( ( g_Map.m_Npc[skillIdx].m_curt_event.type == SM_MOVE_BROADCAST ) &&
			( skill_Info->theSkillType2 == n_GoodSupport &&				
			  skill_Info->theActTarget == n_AT_SELF && 
			  skill_Info->theToggleFlag == 0 ) )
		{
			///--g_Map.m_Npc[skillIdx].AllActQueue();
			g_Map.m_Npc[skillIdx].SetUnderAttackEventMgr( skill_event );
		}
		else
		{
			if( g_Map.m_Npc[skillIdx].m_State.nCharacterType == nCharacterTypePlayerNPC )
			{	
				///--PRINT_DLG_LOG("유저형 NPC 스킬 들어옴 - 스킬 코드 %d 대상수 %d", skill_Info->theSkillCode, skill_event.number );
				
				if(skill_Info->theNeedSpelling)
				{
					skill_event.status = SM_CHANT_READY;
					skill_event.motion = g_Map.m_Npc[skillIdx].SetChantMotion( SM_CHANT_READY,0 );
				}
			}
		
			g_Map.m_Npc[skillIdx].PushQueue( skill_event );			
		}
	} 
	
	return;
} 


void CRockClient::Proc_RpScAppearSkill( SRpScAppearSkill* aMsg )
{
	//	DWORD		theSkillID;
	//	DWORD		theCell;
	//	WORD		theSkillCode;
	
	g_Particle.m_EffectManager.AddLandEffect( aMsg->theSkillCode, aMsg->theSkillID, 
		aMsg->theCell % (g_Map.m_cSize * 2), 
		aMsg->theCell / (g_Map.m_cSize * 2) );
	
	return;
}


void CRockClient::Proc_RpScDisappearSkill( SRpScDisappearSkill* aMsg )
{
	//	DWORD		theSkillID;
	
	g_Particle.m_EffectManager.Delete( aMsg->theSkillID );
	
	return;
}


void CRockClient::Proc_RpScSeeKeepupSkillEnd( SRpScSeeKeepupSkillEnd* aMsg )
{
	RLGS( "Proc_RpScSeeKeepupSkillEnd()" );
	
	//	DWORD			theID;			// ID
	//	BYTE			theType:1;		// n_PC, n_NPC;
	//	BYTE			theEffectType:7;// 1~30 : 지속효과, 31 ~ : 지속부가효과(부가효과 + 30)
	//	WORD			theEffectValue;	// 복구되는 효력 값(부가효과일 경우는 미정)
	//	WORD			theEffectMaxValue; // theEffectMaxValue
	//	WORD			theSkillCode;	// 스킬코드 USHRT_MAX : 적용된 모든 스킬 해제 theEffectType 무시
	//	BYTE			theEndFlag;		// n_Success(해당 지속스킬 종료)
	
	// 부가효과  theEffectValue, theEffectMaxValue만 사용 안함
	//	DWORD			theID;			// ID
	//	BYTE			theType:1;		// n_PC, n_NPC;
	//	BYTE			theEffectType:7;// 1~30 : 지속효과, 31 ~ : 지속부가효과(부가효과 + 30)
	//	theEffectType // 51 부터는 부가효과 코드로 사용 
	//	WORD			theSkillCode;	// 스킬코드 USHRT_MAX : 적용된 모든 스킬 해제 theEffectType 무시
	//	BYTE			theEndFlag;		// n_Success(해당 지속스킬 종료) 
	
	
	BOOL IsDeleteSkillIcon = FALSE;
	BOOL IsDeleteKeepupSkill = FALSE;
	BOOL IsDeleteKeepupSkill_AddEffect = FALSE;
	
	
	if( aMsg->theEffectType > 50 )		// 부가효과 처리 (50번 이상은 부가효과로 처리한다)
	{
		g_Pc.theSkillSystem.UseSkillAddEffectValueMgr( NULL, FALSE, aMsg->theID, aMsg->theType, aMsg->theSkillCode );
		
		IsDeleteKeepupSkill_AddEffect = TRUE;
	}
	else // 일반 효과 처리 
	{		
		DWORD l_EffectValue = 0;
		l_EffectValue = aMsg->theEffectValue;
		
		g_Pc.theSkillSystem.SetDamageValue( aMsg->theID, aMsg->theType, aMsg->theSkillCode, aMsg->theEffectType, l_EffectValue );
		g_Pc.theSkillSystem.UseSkillEffectValueMgr( aMsg->theID, aMsg->theType, aMsg->theSkillCode, 
													aMsg->theEffectType, l_EffectValue, aMsg->theEffectMaxValue, FALSE );
	}
	
	WORD i = 0;
	WORD j = 0;
	
	if( aMsg->theEndFlag == n_Success )		
	{
		RLGS( "***** aMsg->theEndFlag == n_Success *****" );
		///--PRINT_DLG_LOG("** aMsg->theEndFlag == n_Success ** SkillCode = %u", aMsg->theSkillCode );
		
		IsDeleteSkillIcon = TRUE;
		IsDeleteKeepupSkill = TRUE;
		IsDeleteKeepupSkill_AddEffect = TRUE;
	}	
	else if( aMsg->theEndFlag == n_EffectEndFlag )				// 효과 3가지가 모두 종료되고 부가효과만 남은 상태 
	{ 
		IsDeleteKeepupSkill = TRUE;  
	}
	
	// <false, 전체 종료, 효과 3개> 죵료 세가지로 처리 (스킬 아이콘 + 효과 3개 종료 경구엔 클라이언트 알아서 예외처리)
	//	if( aMsg->theEndFlag == n_Skill_End_Icon )					// 스킬 아이콘만 종료 ( 종료할 효과 없음 )
	//	{
	//		IsDeleteSkillIcon = TRUE;
	//	}
	//	else if( aMsg->theEndFlag == n_Skill_End_Icon_Effect )		// 스킬 아이콘 + 효과 3개 종료 
	//	{
	//		IsDeleteSkillIcon = TRUE;
	//		IsDeleteKeepupSkill = TRUE;
	//	}
	//	else if( aMsg->theEndFlag == n_Effect_End )					// 스킬 효과 3개만 종료 
	//	{ 
	//		IsDeleteKeepupSkill = TRUE;  
	//	}

	SSkillBaseInfo* skill_Info = NULL;
	
	// 지속스킬이 모두 종료 되었을때 날아오며 TRUE 일때 이펙트를 지워준다 			
	if( IsDeleteSkillIcon || IsDeleteKeepupSkill || IsDeleteKeepupSkill_AddEffect)
	{	
		WORD tableIndex = 0;
		
		if( aMsg->theType == n_PC )
		{
			if( g_Pc.m_MainPC.lUnique == aMsg->theID )
			{
				if( IsDeleteSkillIcon )
					g_Pc.GetPlayer()->m_KeepUpChecker.DeleteCode( aMsg->theSkillCode );
				
				for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
				{
					if( nRui->thePcInven.KeepupSkill[i] == aMsg->theSkillCode )
					{
						if( IsDeleteSkillIcon )
						{
							skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aMsg->theSkillCode );
							
							if( skill_Info  )
							{
								Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR( IDS_SYSPR_DISKEEPSKILL ) , RWCHAR(skill_Info->theSkillName ) );	///--HAN_MSG
								SystemPrint( n_LightYellow, n_SystemMsg, g_strMessage ); 
							}

							WORD aSkillCode = aMsg->theSkillCode;
							nRui->thePcInven.KeepupSkill[i] = 0;
							PlayerInfoWnd.DeleteKeepUpSkillCode( i, aSkillCode );						
							PlayerInfoWnd.SortKeepupSkill();	
						}
						
						if( IsDeleteKeepupSkill )
						{
							//.............................................................................................................
							// 지속성 스킬 이펙트 지우기

							//.............................................................................................................
							g_Particle.m_EffectManager.DeleteKeepupSkill( aMsg->theSkillCode , g_Pc.GetPlayer() );
						}
						
						if( IsDeleteKeepupSkill_AddEffect )
						{
							g_Particle.m_EffectManager.DeleteAddEffectEnd( aMsg->theSkillCode , g_Pc.GetPlayer() );
						}
						break;
					}
				}
			}
			else
			{
				tableIndex = g_Pc.FindUser( aMsg->theID );
				if( tableIndex == MAX_USER_INTABLE )
					return;
				
				if( IsDeleteSkillIcon )
					g_Pc.m_Pc[ tableIndex ].m_KeepUpChecker.DeleteCode( aMsg->theSkillCode );
				
				for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
				{
					if( g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[i] == aMsg->theSkillCode )
					{
						if( IsDeleteSkillIcon )
						{
							g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[i] = 0;
							
							int	skill_index = 0;
							WORD tmpKeepUpSkill[RP_MAX_KEEPUP_SKILL] = {0,};
							
							for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
							{
								if( g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[j] != 0 )
								{
									tmpKeepUpSkill[skill_index] = g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[j];			
									++skill_index;									
									g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[j] = 0;			
								}		
							}
							
							for( j = 0; j < skill_index; ++j )
							{
								g_Pc.m_PcTable[tableIndex].char_info.theKeepupSkill[j] = tmpKeepUpSkill[j];
							}		
							
							///-- By simwoosung -- 아이템 관련스킬 아이콘 때문에 
							///-- 지속스킬로 인해 스킬이 사라질때 스킬아이콘이 안사리지는경우가있다.
							///-- 해서 이코드를 넣어준다.
							NonPlayerInfoWnd.DeleteAllKeepUpSkill();
						}						
						
						if( IsDeleteKeepupSkill )
						{
							//.............................................................................................................
							// 지속성 스킬 이펙트 지우기

							//.............................................................................................................
							g_Particle.m_EffectManager.DeleteKeepupSkill( aMsg->theSkillCode , &g_Pc.m_Pc[ tableIndex ] );
						}
						
						if( IsDeleteKeepupSkill_AddEffect )
						{
							g_Particle.m_EffectManager.DeleteAddEffectEnd( aMsg->theSkillCode , &g_Pc.m_Pc[ tableIndex ] );
						}
						
						break;
					}
				}
			}
			
			
		}
		else if( aMsg->theType == n_NPC )
		{
			tableIndex = g_Map.FindNpc( aMsg->theID );
			if( tableIndex == MAX_NPC_INTABLE )
				return;
			
			if( IsDeleteSkillIcon )
				g_Map.m_Npc[ tableIndex ].m_KeepUpChecker.DeleteCode( aMsg->theSkillCode );
			
			for( i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
			{
				if( g_Map.m_NpcTable[tableIndex].theKeepupSkill[i] == aMsg->theSkillCode )
				{
					if( IsDeleteSkillIcon )
					{
						g_Map.m_NpcTable[tableIndex].theKeepupSkill[i] = 0;
						
						int	skill_index = 0;
						WORD tmpKeepUpSkill[RP_MAX_KEEPUP_SKILL] = {0,};
						
						for( j = 0; j < RP_MAX_KEEPUP_SKILL; ++j )
						{
							if( g_Map.m_NpcTable[tableIndex].theKeepupSkill[j] != 0 )
							{
								tmpKeepUpSkill[skill_index] = g_Map.m_NpcTable[tableIndex].theKeepupSkill[j];			
								++skill_index;
								
								g_Map.m_NpcTable[tableIndex].theKeepupSkill[j] = 0;			
							}		
						}
						
						for( j = 0; j < skill_index; ++j )
						{
							g_Map.m_NpcTable[tableIndex].theKeepupSkill[j] = tmpKeepUpSkill[j];
						}

						///-- By simwoosung -- 아이템 관련스킬 아이콘 때문에 
						///-- 지속스킬로 인해 스킬이 사라질때 스킬아이콘이 안사리지는경우가있다.
						///-- 해서 이코드를 넣어준다.
						NonPlayerInfoWnd.DeleteAllKeepUpSkill();						
					}
					
					if( IsDeleteKeepupSkill )
					{
						//.............................................................................................................
						// 지속성 스킬 이펙트 지우기

						//.............................................................................................................
						g_Particle.m_EffectManager.DeleteKeepupSkill( aMsg->theSkillCode , &g_Map.m_Npc[ tableIndex ] );
					}
					
					if( IsDeleteKeepupSkill_AddEffect )
					{
						g_Particle.m_EffectManager.DeleteAddEffectEnd( aMsg->theSkillCode , &g_Map.m_Npc[ tableIndex ] );
					}
					
					break;
				}
			}
			// 지속성 스킬 이펙트 삭제 [상준]
			
		}
	}
	
	return;
}


void CRockClient::Proc_RpScSeeKeepupSkillPreEnd( SRpScSeeKeepupSkillPreEnd* aMsg )
{
	//	DWORD			theID;			// ID
	//	BYTE			theType:1;		// n_PC, n_NPC;
	//	BYTE			theEffectType:7;// 1~30 : 지속효과, 31 ~ : 지속부가효과(부가효과 + 30)
	//	WORD			theSkillCode;	// 스킬코드 USHRT_MAX : 적용된 모든 스킬 해제 theEffectType 무시
	
	// 이 스킬의 효력은 10초후에 사라집니다 
	
	return;
}


//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScSay( SRpScSay* aMsg )
{
	
	char theMessage[RP_MAX_CHAT_SIZE];
	memset(theMessage, 0, sizeof(char)*RP_MAX_CHAT_SIZE);
	memcpy(theMessage, aMsg->theMessage, aMsg->theMessageSize);

	int a = 0;
	
	//상대국 대화 차단 .. 	
#ifdef NOT_TALK
	///-- 자기 자신과 국가가 다를시에는 이상한 깨진글자가 나오게 한다.	
		if( aMsg->theSenderID != g_Pc.GetPlayer()->GetUnique() && 
			nRui->thePcParam.Citizen != aMsg->theSenderCitizen )
		{  
			//아이다가 Gm 이라는 단어가 포함 되면 대화 다 볼수있게 한다. . 
#ifndef _DEBUG //g_Pc.m_PcTable[ table_index ].char_info.thePCName
			if(!m_SlangFilter.IsFilterGmName(aMsg->theSender) )
			{
				::ConvertOtherConLanguage(RWCHAR(theMessage) );
			}
#endif	//_DEBUG
			
		}
#endif	// NOT_TALK
		
		//관전자도 그리를 보이게 하라 .. 
		if(!g_RockClient.m_bIsDrawFullScreenUI)
		{
			if( g_Pc.GetPlayer()->IsMyPlayerRender )		
			{
				int table_index = g_Pc.FindUser( aMsg->theSenderID );
				if( table_index == MAX_USER_INTABLE )
				{
					//자기 자신의 말이면 
					if( aMsg->theSenderID == g_Pc.GetPlayer()->GetUnique() )
					{
						

				  		g_ChatMainProc.InsertChatContent(n_NormarChat, nRui->thePcParam.PCName,
													 theMessage );					
				
						if( g_Pc.GetPlayer()->m_curt_event.status != SM_SELLSKILL_SITDOWN && 
							g_Pc.GetPlayer()->m_curt_event.status != SM_SELLSKILL_KEEPING &&
							g_Pc.GetPlayer()->m_curt_event.status != SM_SELLSKILL_STANDUP &&
							g_Pc.GetPlayer()->IsMyPlayerRender )
						{
							SetPlayerMessage( theMessage );
						}
		

						return;
					}
					else
					{	
						//PVP 맵
						if( g_Map.m_NowMapID == c_RockMap_Millena_Rain_PVP_Zone || 
							g_Map.m_NowMapID == c_RockMap_Millena_Rain_Space33 || 
							g_Map.m_NowMapID == c_RockMap_Millena_Rain_FlagPVP34 )
						{
							
						}
						else 
						{
							return ; 		
						}						
					}					
				}
				else
				{					
					if( g_Pc.m_Pc[ table_index ].m_curt_event.status != SM_SELLSKILL_SITDOWN &&
						g_Pc.m_Pc[ table_index ].m_curt_event.status != SM_SELLSKILL_KEEPING &&
						g_Pc.m_Pc[ table_index ].m_curt_event.status != SM_SELLSKILL_STANDUP    )
					{
						SetNonPlayerMessage( aMsg->theSenderID, theMessage );
					}					
				}				
			}

			g_ChatMainProc.InsertChatContent(n_NormarChat, aMsg->theSender, theMessage);
		}
		else
		{
			if(g_BZ_RoomListWnd.IsVisible)
			{
				g_BZ_RoomListWnd.InsertBZNomalChatMsg(aMsg->theSender, theMessage);
			}
			else if(g_BZ_WaitingRoomWnd.IsVisible)
			{
				g_BZ_WaitingRoomWnd.InsertBZNomalChatMsg(aMsg->theSender, theMessage);
			}
		}

}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScWhisper( SRpScWhisper* aMsg )
{
	CChatOutput* optext			= (CChatOutput*)Container->Get( WID_ChatOutput_Text );
	TCHAR WhisperMessage[256]	= {0,};
	
	if( g_MCommunityWnd.m_MSettingTabWnd.GetIsWhisperBlock() )
	{
		return;
	}

	switch( aMsg->theResult )
	{
	case n_Success://성공

		{

			// 일반 유져들만 검사한다. 

#ifdef WHISPER_GM

			if( aMsg->theUsertype == n_Default)
			{
				if(g_MCommunityWnd.m_MBlockTabWnd.ExistBlockName(aMsg->theReceiver) )
				{
					break;
				}
			}
#else
			if(g_MCommunityWnd.m_MBlockTabWnd.ExistBlockName(aMsg->theReceiver) )
			{
				break;
			}

#endif		
			if(!g_RockClient.m_bIsDrawFullScreenUI)
			{
				g_ChatMainProc.InsertChatContent(n_WhisperChat, aMsg->theReceiver, 
					theWhisperMessage, true);
			}
			else
			{
				if(g_BZ_RoomListWnd.IsVisible)
				{
					g_BZ_RoomListWnd.InsertBZWhisperChatMsg(aMsg->theReceiver, theWhisperMessage,
						true);
				}
				else if(g_BZ_WaitingRoomWnd.IsVisible)
				{
					g_BZ_WaitingRoomWnd.InsertBZWhisperChatMsg(aMsg->theReceiver, theWhisperMessage,
						true);
				}
			}
		}
		break;
	case n_Fail://실패
		Rsprintf( RWCHAR(WhisperMessage), G_STRING_CHAR( IDS_SYSPR_WHISP_FAIL ), RWCHAR(aMsg->theReceiver) );
		break;
	case n_ERR_RECEIVER_NOT_LOGIN://상대방이 로그인 하지 않았을때....
		Rsprintf( RWCHAR(WhisperMessage), G_STRING_CHAR( IDS_SYSPR_WHISP_NOT_LOGIN ), RWCHAR(aMsg->theReceiver) );
		break;
	case n_ERR_RECEIVER_DENY_MSG://상대방이 귓말 거부상태일때
		Rsprintf( RWCHAR(WhisperMessage), G_STRING_CHAR( IDS_SYSPR_WHISP_REFUSED ), RWCHAR(aMsg->theReceiver) );
		break;
	case n_ERR_RECEIVER_ID_NOT_EXIST://ID가 존재하지 않을때
		Rsprintf( RWCHAR(WhisperMessage), G_STRING_CHAR( IDS_SYSPR_WHISP_NOT_EXIST ), RWCHAR(aMsg->theReceiver) );
		break;
	case n_ERR_RECEIVER_MSG_OVER:// 메시지 사이즈가 너무 큰 경우

		Rsprintf( RWCHAR(WhisperMessage), G_STRING_CHAR( IDS_SYSPR_WHISP_OVERFLOW ), RWCHAR(aMsg->theReceiver) );
		break; 
	case n_ERR_IS_NOT_SAME_CITIZEN:
		Rsprintf( RWCHAR(WhisperMessage),G_STRING_CHAR(IDS_SYSPR_SAME_CITIZEN) , RWCHAR(aMsg->theReceiver) );
		
		break;
	}
	
	
	if( (aMsg->theResult != n_Success) && (Rstrcmp(WhisperMessage, _RT("") ) != 0) )
	{
		if(!g_RockClient.m_bIsDrawFullScreenUI)
		{
			SystemPrint( n_DarkGreen ,n_SystemMsg, WhisperMessage);	
		}
		else
		{
			if(g_BZ_RoomListWnd.IsVisible)
			{
				g_BZ_RoomListWnd.InsertBZHelpChatMsg(WhisperMessage);
			}
			else if(g_BZ_WaitingRoomWnd.IsVisible)
			{
				g_BZ_WaitingRoomWnd.InsertBZHelpChatMsg(WhisperMessage);
			}
		}		
	}
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScRecvWhisper( SRpScRecvWhisper* aMsg )
{
	char theMessage[RP_MAX_CHAT_SIZE] = {0,};
	memcpy(theMessage, aMsg->theMessage, aMsg->theMessageSize);
	
	// 일반 유져들만 검사한다. 
#ifdef WHISPER_GM

	if( aMsg->theUsertype == n_Default)
	{
		if( g_MCommunityWnd.m_MSettingTabWnd.GetIsWhisperBlock() )
		{
			g_RockClient.Send_RpCsWhisper(RCHAR(aMsg->theSender),RCHAR(G_STRING_CHAR(IDS_WND_ALL_INTERCEPTION_WHISPER)));
			return;
		}
		else 
		if(g_MCommunityWnd.m_MBlockTabWnd.ExistBlockName(aMsg->theSender))
		{
			g_RockClient.Send_RpCsWhisper(RCHAR(aMsg->theSender),RCHAR(G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER)));
			return;
		}
	}
	
#else
	
	if(g_MCommunityWnd.m_MBlockTabWnd.ExistBlockName(aMsg->theSender))
	{
		g_RockClient.Send_RpCsWhisper(aMsg->theSender,G_STRING_CHAR(IDS_SYSPR_INTERCEPTION_NOT_WHISPER));
		return;
	}

#endif	
	
	if(!g_RockClient.m_bIsDrawFullScreenUI)
	{
		g_ChatMainProc.InsertChatContent(n_WhisperChat, aMsg->theSender, 
			theMessage, false);
	}
	else
	{
		if(g_BZ_RoomListWnd.IsVisible)
		{
			g_BZ_RoomListWnd.InsertBZWhisperChatMsg(aMsg->theSender, theMessage,
				false);
		}
		else if(g_BZ_WaitingRoomWnd.IsVisible)
		{
			g_BZ_WaitingRoomWnd.InsertBZWhisperChatMsg(aMsg->theSender, theMessage,
				false);
		}
	}
}

//============================================================================================================
//============================================================================================================
// 아이템 시스템 관련 프로토콜 
//============================================================================================================
//============================================================================================================
bool CRockClient::GetItemProtocolResult( int aResult )
{
/*
enum NItemERROR
{
n_NOT_EXIST_ITEM		= 2,
n_NOT_MY_ITEM			= 3,	// 없는 아이템
n_FULL_INVENTORY		= 4,	// 인벤토리 부족
n_WRONG_POSITION		= 5,	// 장착위치 에러
n_SHORT_MONEY			= 6,	// 돈 부족
n_SHORT_BUNDLE			= 7,
n_CANNOT_SELL			= 8,	// 팔수 없는 아이템 
n_CANNOT_EQUIP			= 9,	// 장착불가능 아이템 
n_CANNOT_USE			= 10,
n_NOT_SAME_CODE			= 11,
n_CANNOT_COMBINE		= 12,
n_SHOUD_TAKEOFF_LEFT	= 13,	// 왼손아이템 제거 요망
n_SHOUD_TAKEOFF_RIGHT	= 14,	// 오른손아이템 제거 요망
n_CITIZEN_ERROR			= 15,	// 착용가능한 종족이 아니다.
//n_HAVENOT_PASSIVE_SKILL	= 16	// 패시브 스킬
n_HAVENOT_PASSIVE_SKILL	= 18,
n_MAXCOMBO_OVER			= 19,
n_NOT_EXIST_RECEIVER	= 20,	// 주기에서 상대가 존재하지 않는 경우.. (이럴수가.. )
n_TOO_LONG_DIST_GIVE	= 21,	// 줄때 상대와의 거리는 15cell 이내여야 한다. 
n_NOT_YOUR_OWNITEM		= 22,	// 아이템 소유권 에러 
n_NOT_ALREADY_POS		= 23,
n_ERROR_ITEMDELAYTIME	= 24,	// 아이템 딜레이 타임 
n_NOT_EXIST_BANKITEM	= 25,
n_OVER_BANKSLOT			= 26,
n_FULL_BANK				= 27,	// 보관함이 가득찼습니다.
n_NOT_EMPTY_SLOT		= 28,	// 빈 슬롯이 아닙니다.

  // 엘다크래프트 
  n_LOW_SKILL_GRADE		= 29,	// 스킬 등급이 낮습니다.
  n_CANNOT_SETSTONE		= 30,	// 아이템 개조를 할 수 없는 아이템입니다. (번들아이템)
  n_NoMatch_RemodelStoneInfo	= 31,		// 엘다스톤 개조에 필요한 첨가물의 정보가 일치하지 않습니다.
  n_HAVENOT_LIMIT_LEVEL		= 32, // 착용할수있는 레벨이 아니다..
  n_NOT_EXIST_ITEM_ATTABLE	= 33,
  n_NEED_SCROLLLOG		= 34,		// 스크롤로그가 필요합니다.
  n_ALREADYOPENSCROLL		= 35,		// 이미 열린 스크롤입니다.
  n_NOTMATCHSEQUENCE		= 36,		// 스크롤을 적용할 수 없습니다.
  n_NOTMATCHSUBCLASS		= 37,		// 서브직업이 스크롤과 맞지 않습니다.
  n_CANNOT_USE_INFIGHTING = 38,		// 대전장에서는 사용할 수 없는 아이템입니다. 
  
	// 개인상점
	n_CANNOT_OPENSHOP_MAP	= 39,		// 개인상점을 열 수 없는 장소입니다.
	n_ISNOT_ARMORSTONE		= 40,		// 방어구에 가공이 불가능한 원석입니다.
	n_ISNOT_WEAPONSTONE		= 41,		// 무기에 가공이 불가능한 원석입니다.
	n_PAYITEM_LIMIT			= 42,		// 유료아이템 제약조건 
	
	  n_CHECKING_PAYITEM		= 43,		// 유료아이템 점검중입니다. 잠시후 다시 시도하여 주십시오. 	
	  };
	*/	
	if( aResult != n_Success )
	{		
		switch( aResult )
		{
		case n_NOT_EXIST_ITEM:		// 존재하지 않는 아이템입니다 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_FIND_ITEMINFO ) );
			}
			break;
		case n_NOT_MY_ITEM:			// 없는 아이템
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_FIND_ITEM ) );
			}
			break;
		case n_FULL_INVENTORY:		// 인벤토리 부족
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_FULL_INVEN ) );
			}
			break;
		case n_WRONG_POSITION:		// 장착위치 에러
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_WEAR_THERE ) );
			}
			break;
		case n_SHORT_MONEY:			// 돈 부족
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
			}
			break;
		case n_SHORT_BUNDLE:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CHECK_NUM ) );
			}
			break;
		case n_CANNOT_SELL:			// 팔수 없는 아이템 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_SELL_ITEM ) );
			}
			break;
		case n_CANNOT_EQUIP:		// 장착불가능 아이템
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_WEAR_ITEM ) );
			}
			break;
		case n_CANNOT_USE:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ) );
			}
			break;
		case n_NOT_SAME_CODE:
			break;
		case n_CANNOT_COMBINE:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT ) );
			}
			break;
		case n_SHOUD_TAKEOFF_LEFT:	// 왼손아이템 제거 요망
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_TAKE_OFF_LEFT_HAND ) );
			}
			break;
		case n_SHOUD_TAKEOFF_RIGHT: // 오른손아이템 제거 요망
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_TAKE_OFF_RIGHT_HAND ) );
			}
			break;
		case n_CITIZEN_ERROR:		// 착용가능한 종족이 아니다.	
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OTHER_NATION_ITEM ) );
			}
			break;
		case n_HAVENOT_PASSIVE_SKILL:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_SKILL ) );				
			}
			break;
		case n_MAXCOMBO_OVER:
			break;
		case n_NOT_EXIST_RECEIVER:	// 주기에서 상대가 존재하지 않는 경우.. (이럴수가.. )			
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_EXIST_OBJECT ) );
			}
			break;
		case n_TOO_LONG_DIST_GIVE:	// 줄때 상대와의 거리는 15cell 이내여야 한다. 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVER_DISTANCE ) );
			}
			break;
		case n_NOT_YOUR_OWNITEM: // 아이템 소유권 에러 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );
			}
			break;
		case n_NOT_ALREADY_POS: 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_BLANK_SLOT ) );
			}
			break;
		case n_ERROR_ITEMDELAYTIME:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_USE_BY_COOLTIME ) );
			}
			break;
		case n_NOT_EXIST_BANKITEM:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_FIND_KEEPING_ITEM ) );
			}
			break;
		case n_OVER_BANKSLOT:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_KEEPING ) );
			}
			break;
		case n_FULL_BANK:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_KEEPING ) );
			}
			break;
		case n_NOT_EMPTY_SLOT:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_BLANK_SLOT ) );
			}
			break;
		case n_LOW_SKILL_GRADE:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LOW_SKILL_GRADE ) );
			}
			break;
		case n_CANNOT_SETSTONE:					// 개조 가능한 아이템이 아닙니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_SETSTONE ) );
			}
			break;
		case n_HAVENOT_LIMIT_LEVEL:				// 사용할 수 있는 레벨이 아닙니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_HAVENOT_LIMIT_LEVEL ) );
			}
			break;		
		case n_NEED_SCROLLLOG:				// 스크롤로그가 필요합니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_SCROLLLOG ) );
			}
			break;
		case n_ALREADYOPENSCROLL:			// 이미 열린 스크롤입니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ALREADYOPENSCROLL ) );
			}
			break;
		case n_NOTMATCHSEQUENCE:			// 스크롤을 적용할 수 없습니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTMATCHSEQUENCE ) );
			}
			break;
		case n_NOTMATCHSUBCLASS:			// 서브직업이 스크롤과 맞지 않습니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTMATCHSUBCLASS ) );
			}
			break;
		case n_ISNOT_ARMORSTONE:			// 방어구에 가공이 불가능한 원석입니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_ARMOR ) );
			}
			break;
		case n_ISNOT_WEAPONSTONE:			// 무기에 가공이 불가능한 원석입니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ELDA_NOTAPP_IN_WEAPON ) );
			}
			break;	
		case n_PAYITEM_LIMIT:				// 유료아이템은 지원하지 않는 기능입니다. 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );
			}
			break;		
		case n_CHECKING_PAYITEM:				// 유료아이템 점검중입니다. 잠시후 다시 시도하여 주십시오. 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CHECKING_PAYITEM ) );
			}
			break;		
		case n_CANNOT_USE_OPTION:			//"아이템을 사용할 수 없는 대전옵션입니다." 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTUSE_ITEM_WAR )); 
				
			}
			break;
		case n_HAVENOT_LIMIT_FAME:
			{													// "명성치가 부족합니다. "
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTENOUGH_FAME ) );
			}
			break;
		case n_OverPetSlot:						
			{														//  최대 펫의 수를 초과하였습니다.
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MAXIMUM_PET_EXCESS ));
			}
			break;
		case n_NotPetItem:
			{																	//"팻의 아이템만 팻의 인벤에 적용 가능. "
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ONLY_ITEMPET_IN_PETINVEN ));
			}
			break;
		case n_NotMatchPet:
			{																						//펫의 종류에 부합하지 않는 아이템 입니다.
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_MATCH_PET));			
			}
			break;
		case n_OverRidingNum:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERRIDINGNUM ));
			}
			break;
		case n_NotRidingItem:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_RIDING_ITEM ));
			}
			break;
		case n_MustBeNoItem:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MUST_BE_NOTITEM ));
			}
			break;
		case n_NeedRepairRiding:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_REPAIR_RIDING ));
			}
			break;
		case n_Is_AttackTime:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ATTACK_RIDETIME ));
			}
			break;
		case n_Takeoff_AllRidingItem:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ALLREMOVE_RITEM ));
			}
			break;
			
		case n_NotLoginedUser:
			{	
				//로그인 하지 않은 유져 입니다 .. 
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTLOGINEDUSER ));
				
			}
			break;
		case n_CannotUseforMove:
			{
				//"대상이 소환이나 출두를 할 수 없는 곳에 위치하고 있습니다. "
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOTUSERFORMOVE ));
				
			}
			break;
		case n_NeedUpFirstClass:
			{
				//"1차 전직을 해야 합니다."
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEEDUPFIRSTCLASS ));
				
			}
			break;
		case n_NeedUpSecondClass:
			{
				//"1차 전직을 해야 합니다."
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEEDUPSECONDCLASS ));
				
			}
		case n_FailSetStone:
			{
				// 인챈트를 실패하였습니다.
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FAIL_SET_STONE));
			}
		case n_NeedEmptyServeSlot:
			{
				// 날개장착창이 비어있어야만 합니다. 
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NEED_EMPTY_SERVE_SLOT));
			}
		case n_ErrorWarEvent:
			{
				// 깃발은 교환이나 인벤에 보관할 수 없습니다. 
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ERROR_WAR_EVENT));
			}
		case n_NoMatch_MakeSkillPotion:
			{
				//포션 혼합이 맞지 않습니다.
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOMATCH_MAKESKILLPOTION)); 
			}			
			break;
		case n_Short_DrawMoney:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_SHORT_DRAWMOENY) ); 
			}
			break;
		case n_Error_CallPet:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSERROR_CALLPET) ); 
			}
			break;
		case n_Error_ArmorTakeON:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSERROR_ARMORTAKEON) ); 
			}
			break;
		case n_NotCreateSoldier_LevelLimit:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSCANNOTCALLSOLINTIHSLEVEL) );	
			}
			break;
		case n_NotCreateSoldier_Already:
			{
				SystemPrint(n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSNOWSOLINSLOT) );  
			}
			break;
		case n_Limit_AddAccessory:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_ADDACCCANNOTCUMMUNI) ); 
			}
			break;
			////-- 이 부분에다 기타 다른 에러메시지를 뿌려준다.
		case n_Not_AddScroll:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_HAVENOTSCROLL) );	
			}
			break;
		case n_Short_AddScroll:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_HAVENOTENOUGHSCROLL) ); 			
			}
			break;
		case n_Not_AddAccessory:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_HAVENOTACCESSARY) ); 
			}
			break;
		case n_Over_MaxLevel:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_MAXLEVELIS20) ); 
			}
			break;	
		case n_Difference_AddAccessoryType:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_NOTTYPEMAKEACCESARY) ); 
			}
			break;
		case n_NotCreateSoldier_Occupation:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_NOTSOLDIER_NOTOCCU) ); 
			}
			break;
		case n_NotMove_PayItem:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_NOTMOVE_PAYITEM) ); 
			}
			break;
		case n_OnlyOneTimeQuest:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_ONLYONETIMEQUEST) ); 
			}
			break;
		case n_JustOneADayQuest:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_JUSTONEADAYQUEST) ); 
			}
			break;
		case n_NotMatchMainClass:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_CANNOTBYUANDUSEJOP) ); 
			}
			break;
		case n_OverapedItemEffect:
			{
				SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_OVERRAPEDITEMEFFECT) ); 
			}
			break;	
		case n_NotExistFrontierPet:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTEXIST_FRONTIERPET ) );
			}
			break;
		case n_NotJoinedFrontier :
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_IN_FRONTIER ) );
			}
			break;
		case n_DiedFrontierPet:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_HOLLY_ISDIE) );
			}
			break;
		case n_MajorAuthority:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MAJORAUTHORITY ) );
			}
			break;
		case n_NeedCallFrontierPet:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_HOLLY_MUSTCALL) );	
			}
			break;
		case n_ITEMERR_Need_SameGrade_Eldastone:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_Need_SameGrade_Eldastone) );	
			}
			break;
		case n_ITEMERR_Check_MixEldastone_Condition:
			{
				SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_Check_MixEldastone_Condition));	
			}
			break;
		case n_ITEMERR_FAILMIXELDASTONE:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FAILMIXELDASTONE));
			}
			break;
		case n_ITEMERR_MIXELDA_SETSEQUENCELV:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_MIXELDA_SETSEQUENCELV));
			}
			break;
		case n_ITEMERR_Q_OVERAPED_QUESTITEM:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_Q_OVERAPED_QUESTIME));
			}
			break; 
		case n_ITEMERR_Need_SameGrade_Insurance:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NEED_SAMEGRADE_INSURANCE));
			}
			break; 
		case n_PETERR_Not_Usable_PetType:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOT_USABLE_PETTYPE));
			}
			break;
		case n_ITEMERR_Check_Changable_StoneGrade:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_CHANGABLE_STONEGRADE));
			}
			break;
		case n_NoRiding_InIndun:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NORIDE_INDUN));
			}
			break; 
		case n_OverMaxMakeCount:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_OVERMAX_MAKECOUNT)); 
			}
			break; 
		case n_NotMatchEventItem:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOTMATCH_EVENTITEM)); 
			}
			break;
		case n_ErrEnterAlgor_LevelLimit:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTENTERALGOR_LEVELLIMIT ) );
			}
			break;
		case n_ErrEnterGoldo_LevelLimit:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTENTERGOLDO_LEVELLIMIT ) );
			}
			break;
		case n_ErrUseMovePayItem_Nation:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_USEMOVEPAYITEM_NATION  ) );
			}
			break;
		case n_ErrUsePayInvenAddItem:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_USEPAYINVEN_ADDITEM  ) );
			}
			break;
		case n_ErrOverTimePetSlot:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERTIME_PETSLOT  ) );
			}
			break;
		default:
			break;
		}
		
		
		return false;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScPickup( SRpScPickup* aMsg )
{
	//	RLGS( "**** PROC_SC -> PICKUP ****" );
	///--PRINT_DLG_LOG("ScPickup code %d UID %d Bundle %d " , aMsg->theItemData.theWasteItem.theCode , aMsg->theItemData.theWasteItem.theItemUID , aMsg->theItemData.theWasteItem.theBundle);

	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		SPcItem*	tmpPcItem = NULL;
		
		// SPcItem객체생성
		tmpPcItem = SAFE_NEW( SPcItem );	
		
		BOOL IsInsertItem = FALSE;
		
		//6만번대 이상은 테이블에 없는 정보이므로 강제로 해줘야 한다. 
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->theItemData.theItem.theCode );
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) ) 
		{
			tmpPcItem->IType	= n_ITYPE_WEAR;
			tmpPcItem->Amount	= 1;			
			
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItemData), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			tmpPcItem->IType	= n_ITYPE_USE;
			tmpPcItem->Amount	= aMsg->theItemData.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			tmpPcItem->IType	= n_ITYPE_UNUSE;
			tmpPcItem->Amount	= aMsg->theItemData.theWasteItem.theBundle;
		}
		//else if( ItemType == 12 ) 얼굴
		else if( ItemType == n_Class_Quest )
		{
			tmpPcItem->IType	= n_ITYPE_QUEST;
			tmpPcItem->Amount	= aMsg->theItemData.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
		{
			tmpPcItem->IType	= n_ITYPE_SPECIAL; 
			tmpPcItem->Amount	= aMsg->theItemData.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_PersonalPet )
		{
			tmpPcItem->IType	= n_ITYPE_PET_USE; 
			tmpPcItem->Amount	= aMsg->theItemData.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			tmpPcItem->IType	= n_ITYPE_RIDE; 
			tmpPcItem->Amount	= 1;
			//말에 말굽내구도가 있기 땜시 .. 장비테이블에 설정해준다 .. 
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItemData), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{
			tmpPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			tmpPcItem->Amount	= 1;			
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItemData), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{
			tmpPcItem->IType = n_ITYPE_SKIN_WEAR;
			tmpPcItem->Amount	= 1;
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItemData), sizeof( URpdItem ) );
		}

		tmpPcItem->Class		= ItemType;
		tmpPcItem->ItemUID		= aMsg->theItemData.theItem.theItemUID;
		tmpPcItem->Code			= aMsg->theItemData.theItem.theCode;
		tmpPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theItemData.theItem.theCode );
		tmpPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItemData.theItem.theCode );
		
		if( tmpPcItem->ItemTable == NULL )
		{
			SAFE_DELETE( tmpPcItem );						
			return;
		}
		// 왼손,오른손....
		//---------------------------------------------------------------------
		if( ( tmpPcItem->ItemTable->theType == n_Type_MagicBook ) || ( tmpPcItem->ItemTable->theType == n_Type_Bow ) || 
			( tmpPcItem->ItemTable->theType == n_Type_Shield ) )
		{
			tmpPcItem->Class = n_Class_Shield;
		}
		else if( ( tmpPcItem->ItemTable->theType == n_Type_Sword ) || ( tmpPcItem->ItemTable->theType == n_Type_TwohandSword ) ||
			( tmpPcItem->ItemTable->theType == n_Type_Staff ) || ( tmpPcItem->ItemTable->theType == n_Type_CrystalBall ) )
		{
			tmpPcItem->Class = n_Class_Weapon;
		}
		//---------------------------------------------------------------------
		
		SItemBaseInfo* item_Info = NULL;
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItemData.theItem.theCode );	
		if( item_Info == NULL)
		{
			SAFE_DELETE( tmpPcItem );
			return;
		}
		
		//메모리 해제해야할 주소를 걸러내기 위한 변수 선언 by simwoosung
		SPcItem*	ptmpReleasePcItem = NULL;
		
		CItemTransButton *pItemTransButton = g_main_buttons.GetInVenButton();
		SItemTransInfo ItemTransInfo;
		ItemTransInfo.Is3DPos = true;
		ItemTransInfo.interNpcId = aMsg->theProcessNPC;
		ItemTransInfo.isMoney = false;
		ItemTransInfo.isGet = true;
		ItemTransInfo.pcInfo = *tmpPcItem;			
		
		NonPlayer *pNonPlayer = g_Map.GetMonster(ItemTransInfo.interNpcId);
		NPCTABLE  *pNPCTable  = g_Map.GetNpcTable(ItemTransInfo.interNpcId);
		if( pNonPlayer && pNPCTable )
		{
			WORD NPCCode = pNPCTable->code;
			if(	IsNonPlayerCode(NPCCode) )
			{
				ItemTransInfo.isMonster = false;					
			}
			else
			{
				ItemTransInfo.isMonster = true;
				ItemTransInfo.upPosition = D3DXVECTOR3(pNonPlayer->GetPosTM()._41,
					pNonPlayer->GetPosTM()._42 
					+ pNonPlayer->m_fMaxHeight,
					pNonPlayer->GetPosTM()._43);
			}			
			
			pItemTransButton->InsertItemTransInfo(ItemTransInfo);
		}
		else
		{
			ItemTransInfo.isMonster = false;
			pItemTransButton->InsertItemTransInfo(ItemTransInfo);
		}	
		
		if( tmpPcItem->IType == n_ITYPE_QUEST )
		{
			int nPreCount = 0;
			WORD nPreItmCode = 0;

			if( m_PcInven->InvenQuest[aMsg->theItemData.theItem.thePosition] )
			{
				nPreCount = m_PcInven->InvenQuest[aMsg->theItemData.theItem.thePosition]->Amount;
				nPreItmCode = m_PcInven->InvenQuest[aMsg->theItemData.theItem.thePosition]->Code;

				///-- 같은 아이템이 아닐경우
				if( nPreItmCode != tmpPcItem->Code )
				{
					nPreCount = 0;
				}
			}
			
			SAFE_DELETE( m_PcInven->InvenQuest[aMsg->theItemData.theItem.thePosition] );			
			m_PcInven->InvenQuest[aMsg->theItemData.theItem.thePosition] = tmpPcItem;
			IsInsertItem = TRUE;
			
			if( item_Info )
			{
				SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_ITEM ), 
				item_Info->theName, tmpPcItem->Amount - nPreCount );
			}
		}
		else if(tmpPcItem->IType == n_ITYPE_RIDE)
		{
			if( item_Info )
			{
				SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_RIDING_IN_TAB) , 
					item_Info->theName );
			}
			
			SAFE_DELETE( m_PcInven->InvenHorse[aMsg->theItemData.theItem.thePosition] );
			m_PcInven->InvenHorse[aMsg->theItemData.theItem.thePosition] = tmpPcItem;
			IsInsertItem = TRUE;
		}
		else
		{	
			if( item_Info )
			{
				if( m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition] && m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition]->Amount >= 1 )
				{
					SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_ITEM ), 
						item_Info->theName, tmpPcItem->Amount - m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition]->Amount );
					
					//현재 딜레이가 적용되는 아이템이라면
					if(m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition]->IsDelay)
					{
						tmpPcItem->IsDelay = true;
						tmpPcItem->DelayTime = m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition]->DelayTime;
						tmpPcItem->ProgressTime = m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition]->ProgressTime;
						tmpPcItem->CumulativeTime = m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition]->CumulativeTime;				   				
					}			
				}
				else
				{
					////-- 현재 아이템에 관련된 딜레이가 적용되었는지 체크해서 넣어준다.					
					if( tmpPcItem->IType == n_ITYPE_USE && tmpPcItem->ItemTable )
					{
						 int iDelayGroup = tmpPcItem->ItemTable->theDelayGroup;
						 if( !(nRui->enableDelayTime[iDelayGroup] ) )						
						 {
							tmpPcItem->IsDelay = true;						
						 }
					}					
					
					SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_ITEM ), 
						item_Info->theName, tmpPcItem->Amount );
				}
			}		
			
			for( int i = 0; i < RP_MAX_QUICK_SLOT; ++i )
			{
				if( ( m_PcInven->QuickSlot[i] != NULL ) && ( m_PcInven->QuickSlot[i]->IType == n_ITYPE_USE ) &&
					( m_PcInven->QuickSlot[i]->ItemUID == tmpPcItem->ItemUID ) )
				{
					m_PcInven->QuickSlot[i] = tmpPcItem;
					IsInsertItem = TRUE;
					break;
				}
			}
			
			SAFE_DELETE( m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition] );
			m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition] = tmpPcItem;
			IsInsertItem = TRUE;			
		}
		  
		for( int j = 0 ; j < RP_MAX_QUEST_FLAG; ++j )
		{
			//순서대로 등록되니 널이면 튕~
			if(m_PcInven->QuestFlag[j] == NULL) 	
			{
				break;
			}
			
			for(int  num = 0; num < RP_MAX_QUEST_COUNT; ++num )
			{
				if( m_PcInven->QuestCount[num] == NULL )
				{
					break;
				}
				
				//플래그의 퀘스트 코드와 , 퀘스트목록에 코드와 비교 
				if( m_PcInven->QuestFlag[j]->theQuestCode == m_PcInven->QuestCount[num]->theQuestCode && 
					m_PcInven->QuestFlag[j]->theDisplay == n_OPEN )
				{
					// 퀘스트 조건아이템과  현재 들어온 아이템 비교 
					if( m_PcInven->QuestCount[num]->theType == n_ITEM  && ( m_PcInven->QuestCount[num]->theCode ==  tmpPcItem->Code ) )
					{
						QuestWnd.SetSQuestCount(m_PcInven->QuestCount[num]);
						SystemPrint( n_LightOrange, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_Q_GET ), 
							item_Info->theName, m_PcInven->QuestCount[num]->theCount, 
							m_PcInven->QuestCount[num]->theTotalCount);	
					}				
				}
			}
		}	
		//아이템을 처음 생성시... 시간갱신 

#ifdef C_TIME_SUIT_UPDATE
		if(tmpPcItem->ItemTable->theRare == 255 && tmpPcItem->ItemTable->theMaxEndurance == 0 )
		{
			DWORD dwTime = 0 ; 
			for( int j = 0; j < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++j)
			{
				if(m_PcInven->PayTimeItemList[j] == NULL)
				{
					//두곳 장착과 인벤 아이템 들어가 있는것. . 
					m_PcInven->PayTimeItemList[j] = SAFE_NEW( SPayTimeItem );

					m_PcInven->PayTimeItemList[j]->theItemCode = tmpPcItem->Code;
					m_PcInven->PayTimeItemList[j]->theItemID = tmpPcItem->ItemUID;
					
					DWORD AllUseTime = item_Info->theReGage * 1;							// Sec
					AllUseTime += item_Info->theReMPSpeed * 1 * 60;					// Min
					AllUseTime += item_Info->theReSPSpeed * 1 * 60 * 60;			// Hour
					AllUseTime += item_Info->theReHPSpeed * 1 * 60 * 60 * 24;	// Day 
					m_PcInven->PayTimeItemList[j]->theTime = AllUseTime;
					g_ListTime.InsertMessageTime(m_PcInven->PayTimeItemList[j]->theItemID , AllUseTime);
					char ItemStr[128] ={0,};
					Rstrcpy(ItemStr ,tmpPcItem->ItemTable->theName );
					g_ListTime.FirstShowItem(AllUseTime , tmpPcItem->ItemTable->theName ); 
					break;
				}
			}	
		}
#endif

		QuestWnd.SetQuestField();
		QuestWnd.SetRenewalQuestInfo();
		InventoryWnd.UpdateInvenSlotItem();
		MainMenuWnd.UpdateQuickSlotItem();
		
		if( FALSE == IsInsertItem )
		{
			SAFE_DELETE( tmpPcItem );		
		}
	}
	
	// 일단 "아이템 정보를 찾을 수 없습니다." 라는 메세지들은 서버에서 아이템이 없는 것이므로 클라이언트도 지워주기. 
	if( aMsg->theResult == n_NOT_EXIST_ITEM )
	{
		SRpScDisappearItem	DisappearItemMsg;
		DisappearItemMsg.theItemID = aMsg->theItemData.theItem.theItemUID;
		g_RockClient.Proc_RpScDisappearItem( &DisappearItemMsg );
	}
	
	return;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScPickupEld( SRpScPickupEld* aMsg )
{
	RLGS( "**** PROC_SC -> PICKUP ELD ****" );
	
	int eld = aMsg->theEld - m_PcParam->Money;
	
	if( eld > 0 )
	{
		CItemTransButton *pItemTransButton = g_main_buttons.GetInVenButton();
		SItemTransInfo ItemTransInfo;
		ItemTransInfo.Is3DPos = true;
		ItemTransInfo.interNpcId = aMsg->theProcessNPC;
		ItemTransInfo.isMoney = true;
		ItemTransInfo.isGet = true;		
		memset(&(ItemTransInfo.pcInfo), 0 , sizeof(SPcItem) );			
		
		NonPlayer *pNonPlayer = g_Map.GetMonster(ItemTransInfo.interNpcId);
		NPCTABLE  *pNPCTable  = g_Map.GetNpcTable(ItemTransInfo.interNpcId);
		
		if( pNonPlayer && pNPCTable )
		{
			WORD NPCCode = pNPCTable->code;
			if(	IsNonPlayerCode(NPCCode) )
			{
				ItemTransInfo.isMonster = false;
			}			
			else
			{
				ItemTransInfo.isMonster = true;
				ItemTransInfo.upPosition = D3DXVECTOR3(pNonPlayer->GetPosTM()._41,
					pNonPlayer->GetPosTM()._42
					+ pNonPlayer->m_fMaxHeight,
					pNonPlayer->GetPosTM()._43);		
			}			
			pItemTransButton->InsertItemTransInfo(ItemTransInfo);
		}
		else
		{
			ItemTransInfo.isMonster = false;
			pItemTransButton->InsertItemTransInfo(ItemTransInfo);
		}
		
		SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_ELD ), aMsg->theEld - m_PcParam->Money );
	}
	
	m_PcParam->Money = aMsg->theEld;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScStatus2( SRpScStatus2* aMsg )
{
	RLGS( "**** PROC_SC -> STATUS2 ****" );
	
	//아이템 효과 적용....
	memcpy( &m_PcParam->Stat2, &aMsg->theStat2, sizeof( SRpdSTAT2 ) );
}

void	CRockClient::Proc_RpScChangeAvatarSkin(SRpScChangeAvatarSkin* aMsg)
{
	
	//DWORD			theBeforeItemCode;	// 변경전 아이템 (기존에 착용하고 있던 아이템)
	//DWORD			theAfterItemCode;		// 변경후 아이템 (이전에 SRpScPickup으로 보낸 아이템의 ID)
	SItemBaseInfo* Item_Info1 = NULL; 
	Item_Info1 = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theBeforeItemCode  );
	SItemBaseInfo* Item_Info2 = NULL ;
	Item_Info2 = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theAfterItemCode  );
	
	SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CHANGE_AVATA ),Item_Info1->theName ,  Item_Info2->theName  );
}
//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScDrop( SRpScDrop* aMsg )
{
	RLGS( "**** PROC_SC -> DROP ****" );
	///--PRINT_DLG_LOG("**** PROC_SC -> DROP ****");
	//aMsg->theItemID;
	//aMsg->theBundle;
	//aMsg->theResult;
	// 항상...Drop은 InvenSlot에 있는 Item을 Drop한다...
	int i = 0;
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		SItemBaseInfo* item_Info = NULL;		
		
		// 드롭이나 픽킹시 정보를 매번  갱신해 준다 .. 		
		
		switch( aMsg->theItemTab )
		{
		case n_WearTab:
			for( i = 0; i < RP_MAX_EQUIP; ++i )
			{
				if( ( m_PcInven->WearItem[i] != NULL ) && ( m_PcInven->WearItem[i]->ItemUID == aMsg->theItemID ) )
				{	
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( m_PcInven->WearItem[i]->Code );
					
					//커서에 있는 Item 지우기....
					nRui->ResetCursorItem();					
					
					//캐릭터에 실제 적용....
					g_Pc.GetPlayer()->Item_TakeOff( i );
					
					
					if( i == n_WearSlot_SKIN_Serve  && nRui->thePcInven.WearItem[ n_WearSlot_Serve] ) 
					{
						g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[ n_WearSlot_Serve]->Code , g_Pc.GetPlayer() );
					}

					//dongs 퀘스트 정보  추가  일반아이템 퀘스트 로  들어가서 .. 처리 해죠야 함 ... 
					// 성공시 플그래스로 메모리에 삭제를 안시키니깐 있냐 없냐 를 체크 
					for(int j=0 ; j < RP_MAX_QUEST_FLAG; ++j)
					{
						//순서대로 등록되니 널이면 튕~
						if(m_PcInven->QuestFlag[j] == NULL) 	
						{
							break;
						}
			
						for(int  num = 0; num < RP_MAX_QUEST_COUNT; ++num )
						{
							if( m_PcInven->QuestCount[num] == NULL)
							{
								break;
							}
							//플래그의 퀘스트 코드와 , 퀘스트목록에 코드와 비교 
							if(m_PcInven->QuestFlag[j]->theQuestCode ==  m_PcInven->QuestCount[num]->theQuestCode && 
								m_PcInven->QuestFlag[j]->theDisplay == n_OPEN)
							{
								
								// 퀘스트 조건아이템  과  현재 들어온 아이템 비교 
								if( ( m_PcInven->QuestCount[num]->theCode ==  m_PcInven->WearItem[i]->Code ))
								{
									m_PcInven->QuestCount[num]->theCount = 0;
									
									//인벤 검사 있냐 없냐 ? 
									for(int k =0 ; k < c_MaxInvenItemNum; ++k)
									{
										if(m_PcInven->InvenItem[k] == NULL ) continue;
										
										if(m_PcInven->QuestCount[num]->theCode  == m_PcInven->InvenItem[k]->Code )
										{											
											m_PcInven->QuestCount[num]->theCount +=  m_PcInven->InvenItem[k]->Amount;       
										}
										
									}		
									
									//장비 인벤 검사 
									for(int j=0; j < RP_MAX_EQUIP; ++j )
									{
										if(m_PcInven->WearItem[j]==NULL) continue;
										
										if(m_PcInven->QuestCount[num]->theCode == m_PcInven->WearItem[j]->Code )
										{
											m_PcInven->QuestCount[num]->theCount +=  m_PcInven->WearItem[j]->Amount;    
										}
									}
									
									
									if( m_PcInven->QuestCount[num]->theCount >  0 )
									{
										//아이템 갯수가 기본은 1이고 1개 이상이 면 따로처리 ??
										
										m_PcInven->QuestCount[num]->theCount -= m_PcInven->WearItem[i]->Amount;
										
										SystemPrint( n_LightOrange, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_Q_GET ), item_Info->theName, m_PcInven->QuestCount[num]->theCount, m_PcInven->QuestCount[num]->theTotalCount);		
										break;	
									}
								}	
								
							}
							
						}
						
					}					
					
					delete( m_PcInven->WearItem[i] );
					m_PcInven->WearItem[i] = NULL;



					break;
				}
			}
			QuestWnd.SetRenewalQuestInfo();
			InventoryWnd.UpdateWearSlotItem();
			break;
		case n_InvenTab:
			for( i = 0; i < c_MaxInvenItemNum; ++i )
			{
				if( ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theItemID ) )
				{
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( m_PcInven->InvenItem[i]->Code );
					//dongs 퀘스트 정보  추가  일반아이템 퀘스트 로  들어가서 .. 처리 해죠야 함 ... 
					
					for(int j=0 ; j < RP_MAX_QUEST_FLAG; ++j)
					{
						//순서대로 등록되니 널이면 튕~
						if(m_PcInven->QuestFlag[j] == NULL) 	
						{
							break;
						}
						
						for(int  num = 0; num < RP_MAX_QUEST_COUNT; ++num )
						{
							if( m_PcInven->QuestCount[num])
							{
								//플래그의 퀘스트 코드와 , 퀘스트목록에 코드와 비교 
								if(m_PcInven->QuestFlag[j]->theQuestCode ==  m_PcInven->QuestCount[num]->theQuestCode && 
									m_PcInven->QuestFlag[j]->theDisplay == n_OPEN)
								{
									
									// 퀘스트 조건아이템  과  현재 들어온 아이템 비교 
									if( ( m_PcInven->QuestCount[num]->theCode ==  m_PcInven->InvenItem[i]->Code ))
									{
										
										m_PcInven->QuestCount[num]->theCount = 0;
										
										//인벤 검사 있냐 없냐 ? 
										for(int k =0 ; k < c_MaxInvenItemNum; ++k)
										{
											if(m_PcInven->InvenItem[k] == NULL ) continue;
											
											if(m_PcInven->QuestCount[num]->theCode  == m_PcInven->InvenItem[k]->Code )
											{
												m_PcInven->QuestCount[num]->theCount +=  m_PcInven->InvenItem[k]->Amount;       
											}
											
										}	
										
										if( m_PcInven->QuestCount[num]->theCount >  0  )
										{
											//인벤에서는 한개를 
											if(m_PcInven->InvenItem[i]->IType ==  n_ITYPE_SPECIAL)
											{
												m_PcInven->QuestCount[num]->theCount  = aMsg->theBundle;
											}
											else
											{
												m_PcInven->QuestCount[num]->theCount -= m_PcInven->InvenItem[i]->Amount;
											}											
											
											SystemPrint( n_LightOrange, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_Q_GET ), item_Info->theName, m_PcInven->QuestCount[num]->theCount, m_PcInven->QuestCount[num]->theTotalCount);		
											
											//	return;
											break;	
										}
									}	
									
								}
								
							}
							else 
							{
								break;
							}
							
						}
						
					}
					
					//커서에 있는 Item 지우기....
					nRui->ResetCursorItem();
					
					m_PcInven->InvenItem[i]->Amount = aMsg->theBundle;					

					//소모품일때...번들값이 0이면 삭제....다른 아이템들은 Drop일때..기냥 삭제...					
					//					if( m_PcInven->InvenItem[i]->IType == n_ITYPE_USE ) 
					if( m_PcInven->InvenItem[i]->ItemTable->theBundle != 1 )		// 1이 아니면 번들개수가 있는 아이템 
					{
						if( m_PcInven->InvenItem[i]->Amount == 0 )
						{
							//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
							for( int j = 0; j < RP_MAX_QUICK_SLOT; ++j )
							{
								if( m_PcInven->InvenItem[i] == m_PcInven->QuickSlot[j] )
								{
									m_PcInven->QuickSlot[j] = NULL;
									break;
								}
							}
							
							delete( m_PcInven->InvenItem[i] );
							m_PcInven->InvenItem[i] = NULL;
						}
					}
					else
					{
						delete( m_PcInven->InvenItem[i] );
						m_PcInven->InvenItem[i] = NULL;
					}
					
					//퀘스트 창에 적용 
					QuestWnd.SetRenewalQuestInfo();	
					//슬롯에 적용....
					
					MainMenuWnd.UpdateQuickSlotItem();
					InventoryWnd.UpdateInvenSlotItem();	
					return;
				}
			}
			break;
			
		case n_QuestTab:
			for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
			{
				if( ( m_PcInven->InvenQuest[i] != NULL ) && ( m_PcInven->InvenQuest[i]->ItemUID == aMsg->theItemID ) )
				{
					m_PcInven->InvenQuest[i]->Amount = aMsg->theBundle;
					
					if( m_PcInven->InvenQuest[i]->Amount == 0 )
					{
						delete( m_PcInven->InvenQuest[i] );
						m_PcInven->InvenQuest[i] = NULL;
					}
					
					//커서에 있는 Item 지우기....
					nRui->ResetCursorItem();
					
					//슬롯에 적용....
					InventoryWnd.UpdateInvenSlotItem();					
					
					return;
				}
			}
			break;
		case n_RidingTab:
			{
				for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
				{
					if( ( m_PcInven->InvenHorse[i] != NULL ) && 
						( m_PcInven->InvenHorse[i]->ItemUID == aMsg->theItemID ) )
					{
						
						//커서에 있는 Item 지우기....
						nRui->ResetCursorItem();
						
						delete( m_PcInven->InvenHorse[i] );
						m_PcInven->InvenHorse[i] = NULL;
						
						//슬롯에 적용....
						InventoryWnd.UpdateInvenSlotItem();						
						
						return;
					}						
				}
			}
			break;
		case n_PayItemTab:
			{

				for( i = 0; i < RP_MAX_EQUIP; ++i )
				{
					if( ( m_PcInven->WearItem[i] != NULL ) && ( m_PcInven->WearItem[i]->ItemUID == aMsg->theItemID ) )
					{	
						item_Info = g_Pc_Manager.GetItemBaseInfoPtr( m_PcInven->WearItem[i]->Code );
						
						//커서에 있는 Item 지우기....
						nRui->ResetCursorItem();					
						//캐릭터에 실제 적용....

						if( i == n_WearSlot_SKIN_Serve  && nRui->thePcInven.WearItem[ n_WearSlot_Serve] ) 
						{
							g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[ n_WearSlot_Serve]->Code , g_Pc.GetPlayer() );
						}

						g_Pc.GetPlayer()->Item_TakeOff( i );
					
						//dongs 퀘스트 정보  추가  일반아이템 퀘스트 로  들어가서 .. 처리 해죠야 함 ... 
						// 성공시 플그래스로 메모리에 삭제를 안시키니깐 있냐 없냐 를 체크 
						for(int j=0 ; j < RP_MAX_QUEST_FLAG; ++j)
						{
							//순서대로 등록되니 널이면 튕~
							if(m_PcInven->QuestFlag[j] == NULL) 	
							{
								break;
							}
							
							for(int  num = 0; num < RP_MAX_QUEST_COUNT; ++num )
							{
								if( m_PcInven->QuestCount[num] == NULL)
								{
									break;
								}
								//플래그의 퀘스트 코드와 , 퀘스트목록에 코드와 비교 
								if(m_PcInven->QuestFlag[j]->theQuestCode ==  m_PcInven->QuestCount[num]->theQuestCode && 
									m_PcInven->QuestFlag[j]->theDisplay == n_OPEN)
								{
									
									// 퀘스트 조건아이템  과  현재 들어온 아이템 비교 
									if( ( m_PcInven->QuestCount[num]->theCode ==  m_PcInven->WearItem[i]->Code ))
									{
										m_PcInven->QuestCount[num]->theCount = 0;
										
										//인벤 검사 있냐 없냐 ? 
										for(int k =0 ; k < c_MaxInvenItemNum; ++k)
										{
											if(m_PcInven->InvenItem[k] == NULL ) continue;
											
											if(m_PcInven->QuestCount[num]->theCode  == m_PcInven->InvenItem[k]->Code )
											{											
												m_PcInven->QuestCount[num]->theCount +=  m_PcInven->InvenItem[k]->Amount;       
											}
											
										}		
										
										//장비 인벤 검사 
										for(int j=0; j < RP_MAX_EQUIP; ++j )
										{
											if(m_PcInven->WearItem[j]==NULL) continue;
											
											if(m_PcInven->QuestCount[num]->theCode == m_PcInven->WearItem[j]->Code )
											{
												m_PcInven->QuestCount[num]->theCount +=  m_PcInven->WearItem[j]->Amount;    
											}
										}
										
										
										if( m_PcInven->QuestCount[num]->theCount >  0 )
										{
											//아이템 갯수가 기본은 1이고 1개 이상이 면 따로처리 ??
											
											m_PcInven->QuestCount[num]->theCount -= m_PcInven->WearItem[i]->Amount;
											
											SystemPrint( n_LightOrange, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_Q_GET ), item_Info->theName, m_PcInven->QuestCount[num]->theCount, m_PcInven->QuestCount[num]->theTotalCount);		
											break;	
										}
									}	
									
								}
								
							}
							
						}					
						
						
						delete( m_PcInven->WearItem[i] );
						m_PcInven->WearItem[i] = NULL;
					}
				}
			}
			break;

		}		
		
		if( aMsg->theProcess == n_PaySystemProcess )
		{
			if( item_Info )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_DELETE_PAYITEM_LIFEZERO ), 
							 item_Info->theName );
			}
		}

		if( aMsg->theProcess == n_PKGeneralItemProcess )
		{
			if( item_Info )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSITEM_DROP_CAODEATH ),	
							 item_Info->theName );
			}
		}

		if( aMsg->theProcess == n_PKChargeItemProcess )
		{
			if( item_Info )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSITEM_DESTROY_CAODEATH ),    
							 item_Info->theName );
			}
		}
		
	}
	return;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScDropEld( SRpScDropEld* aMsg )
{
	RLGS( "**** PROC_SC -> DROP ELD ****" );
	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		m_PcParam->Money = aMsg->theEld;
	}
	return;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScUse( SRpScUse* aMsg )
{
	RLGS( "**** PROC_SC -> USE ****" );
	
	///--JAPAN_BUG_MODIFY
	nRui->ResetCursorItem();
	
	//소모품만 사용 가능 
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		for( int i = 0; i < c_MaxInvenItemNum; ++i )
		{
			if( ( m_PcInven->InvenItem[i] != NULL ) && (m_PcInven->InvenItem[i]->ItemUID == aMsg->theItemID ) )
			{ 
				nRui->theItemDalayTime[m_PcInven->InvenItem[i]->ItemTable->theDelayGroup] = g_nowTime;
				
				//...........................................................................................
				// 아이템 사용시 이펙트 효과 By wxywxy
				//...........................................................................................
				g_Particle.m_EffectManager.UseItemEffect( g_Pc.GetPlayer()->GetUnique() , m_PcInven->InvenItem[i]->Code );
				//...........................................................................................
				
				//...........................................................................................
				// 소모성 아이템 딜레이효과 적용 By simwoosung
				//...........................................................................................
				if(!LoadingBarWnd.IsVisible)  //귀환류의 아이템이 아니면

					g_SlotDelayProc.SetUseDelayTime(FALSE, m_PcInven->InvenItem, m_PcInven->InvenItem[i]->Code,
					c_MaxInvenItemNum);
				else
					g_SlotDelayProc.SetUseDelayTime(TRUE, m_PcInven->InvenItem, m_PcInven->InvenItem[i]->Code,
					c_MaxInvenItemNum);				
				
				//커서에 있는 Item 지우기....
				nRui->ResetCursorItem();
				
				//아이템 효과 적용....
				//				memcpy( &m_PcParam->Stat1, &aMsg->theStat, sizeof( SRpdSTAT1 ) );
				
				//사용하고 남은 번들.... 0이면 삭제...
				m_PcInven->InvenItem[i]->Amount = aMsg->theBundle;
				bool bChangeItme = false;
				//아 추후에 함수로 빼야 된당. . 
				if( m_PcInven->InvenItem[i]->Amount == 0 )
				{
					//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
					for( int j = 0; j < RP_MAX_QUICK_SLOT; ++j )
					{
						//이 소비 아이템이  퀵슬롯 에 등록 된것이 있느냐 ? 
						if( m_PcInven->QuickSlot[j] && m_PcInven->InvenItem[i] == m_PcInven->QuickSlot[j] )
						{
							//인벤에서 검사 한다 .. 
							for(int k =0 ; k < c_MaxInvenItemNum; ++k )
							{
								//같은 코드의 아이템이 있는가 
								if( m_PcInven->InvenItem[k] != NULL && 
									( (m_PcInven->InvenItem[k]->Code == m_PcInven->InvenItem[i]->Code ) &&
									m_PcInven->InvenItem[k]->Amount > 0) )
								{
									
									//퀵슬롯에 현재 선택된 아이템이 등록이 되있는가 ? ? 
									bool bflag = true;  
									for( int cnt = 0; cnt < RP_MAX_QUICK_SLOT; ++cnt )
									{
										//				
										//현재 선택된 아이템이 퀵슬롯에 있으면 통과 시킨다. 
										if( m_PcInven->QuickSlot[cnt] && 
											( m_PcInven->InvenItem[k]->ItemUID == m_PcInven->QuickSlot[cnt]->ItemUID ) ) 
										{
											bflag = false;  
										}
									}
									
									if(bflag)									
									{
										
										SRpdQuickSlot	Qslot;
										Qslot.theID.theItemID	= m_PcInven->InvenItem[k]->ItemUID;
										Qslot.theType			= n_ITEM;
										Qslot.thePosition		= j ;
										g_RockClient.Send_RpCsLinkQuickSlot( Qslot );
										//m_PcInven->QuickSlot[j] = m_PcInven->InvenItem[k];
										break;
										
									}
									else 
									{
										continue;
									}
									
								}
								
							}
							
							m_PcInven->QuickSlot[j] = NULL;
							break;
						}
					}
					delete( m_PcInven->InvenItem[i] );
					m_PcInven->InvenItem[i] = NULL;
				}
				
				//슬롯에 적용....
				MainMenuWnd.UpdateQuickSlotItem();
				InventoryWnd.UpdateInvenSlotItem();	
				break;
			}			
		}
	}
	
	return;
	
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScSeeUse( SRpScSeeUse* aMsg )
{
	//나중에 처리....
	aMsg->thePCID;// 아이템을 사용하는 유저 아이디 
	aMsg->theItemCode;// 아이템 사용에 따른 효과처리를 위한 아이템 코드

	
	RLGS( "**** PROC_SC -> SEE_USE ****" );
	
	
	//...........................................................................................
	// 아이템 사용시 이펙트 효과 By wxywxy
	//...........................................................................................
	g_Particle.m_EffectManager.UseItemEffect( aMsg->thePCID , aMsg->theItemCode );
	//...........................................................................................
	
	/*
	int tbl_index = g_Pc.FindUser( (long)aMsg->thePCID );
	
	  if( tbl_index == MAX_USER_INTABLE )
	  {
	  g_Pc.m_Pc[tbl_index].Item_Equip( aMsg->theItem.theCode, aMsg->thePosition );
	  }
	  else
	  {
	  //예외 처리
	  RLGS( "**** SC_FAILED -> SEE_USE ****" );
	  }
	//*/
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScEquip( SRpScEquip* aMsg )
{
	RLGS( "**** PROC_SC -> EQUIP ****" );
	
	int  position = aMsg->thePosition;
	
	// 장비 장착.... 인벤 -> 장비창
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		bool bFlag = false;
		
#ifdef RIDING_SYSTEM		
		if( position == n_WearSlot_Riding )
		{
			bFlag = true;
		}
		
		if( position == n_WearSlot_RidingItem1 || position == n_WearSlot_RidingItem2 ||
			position == n_WearSlot_RidingItem3 )
		{
			if(aMsg->theRidingFlag)
			{
				bFlag = true;
			}
			else
			{
				bFlag = false;
			}
		}
		
		if( true == bFlag  )	
		{	
			for( int i = 0; i < RP_MAX_QUEST_ITEM; ++i )
			{
				if( ( m_PcInven->InvenHorse[i] != NULL ) && ( m_PcInven->InvenHorse[i]->ItemUID == aMsg->theItemID ) )
				{
					if( NULL != m_PcInven->WearItem[position] )
					{
						g_Particle.m_EffectManager.DeleteItemLinkEffect( position , ( Character * ) g_Pc.GetPlayer() );
					}
					
					//아이템 효과 적용....
					memcpy( &m_PcParam->Stat2, &aMsg->theStat, sizeof( SRpdSTAT2 ) );
					
					//장비 장착

					m_PcInven->WearItem[position] = m_PcInven->InvenHorse[i];
					m_PcInven->InvenHorse[i] = NULL;
					
					//슬롯 재설정
					InventoryWnd.UpdateInvenSlotItem();
					InventoryWnd.UpdateWearSlotItem();
					
					//커서에 있는 Item 지우기....
					nRui->ResetCursorItem();
					
					//실제 적용....
					g_Pc.GetPlayer()->Item_Equip( m_PcInven->WearItem[position]->Code, position );
					
					break;
				}
			}		
		}
#endif		
		
		if( false == bFlag )
		{		
			for( int i = 0; i < c_MaxInvenItemNum; ++i )
			{
				if( ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theItemID ) )
				{
					if( NULL != m_PcInven->WearItem[position] )
					{
						g_Particle.m_EffectManager.DeleteItemLinkEffect( position , ( Character * ) g_Pc.GetPlayer() );
					}
					
					//아이템 효과 적용....
					memcpy( &m_PcParam->Stat2, &aMsg->theStat, sizeof( SRpdSTAT2 ) );
					
					//장비 장착

					m_PcInven->WearItem[position] = m_PcInven->InvenItem[i];
					m_PcInven->InvenItem[i] = NULL;
					
					//슬롯 재설정
					InventoryWnd.UpdateInvenSlotItem();
					InventoryWnd.UpdateWearSlotItem();
					
					//커서에 있는 Item 지우기....
					nRui->ResetCursorItem();
					
					//실제 적용....
					g_Pc.GetPlayer()->Item_Equip( m_PcInven->WearItem[position]->Code, position );
					
					g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
						g_Pc.m_MainPC.char_info.race , g_lpDevice, false );				
					g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
						g_Pc.m_MainPC.char_info.race , g_lpDevice );
						
					break;
				}
			}
		}		
		
		nRui->thePcParam.MaxHP = aMsg->theMaxHP;
		nRui->thePcParam.MaxMP = aMsg->theMaxMP;
		nRui->thePcParam.MaxSP = aMsg->theMaxSP;
		
		nRui->thePcParam.Power = aMsg->thePower;
		nRui->thePcParam.Vital = aMsg->theVital;
		nRui->thePcParam.Sympathy = aMsg->theSympathy;
		nRui->thePcParam.Intel = aMsg->theInt;		
		nRui->thePcParam.Stamina = aMsg->theStamina;		
		nRui->thePcParam.Dex = aMsg->theDex;		
	}
	return;	
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScChangeEquip( SRpScChangeEquip* aMsg )
{
	RLGS( "**** PROC_SC -> CHANGE_EQUIP ****" );
	
	// 장비 장착.... 인벤 -> 장비창
	
	//aMsg->thePreItemID; //기존 WearSlot에 있던...아이템
	//aMsg->theNewItemID; //새로 WearSlot으로 갈 아이템 
	//aMsg->thePrePosition; // WearSlot에 있던 Item이 들어갈 Inven Position
	//aMsg->theNewPosition; // InvenSlot에 있던 Item이 들어갈 Wear Position
	//aMsg->theResult;// n_NOT_MY_ITEM:없는 아이템, n_CANNOT_EQUIP:장착불가능 아이템, n_WRONG_POSITION:위치에러
	//aMsg->theStat;// ( SRpdSTAT2 )
	int  pre_position = aMsg->thePrePosition;
	int  new_position = aMsg->theNewPosition;
	///--PRINT_DLG_LOG("ScChangeEquip thePreItemID %d theNewItemID %d" ,aMsg->thePreItemID ,aMsg->theNewItemID );
	SPcItem* tmpPcItem = NULL;
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		//아이템 효과 적용....
		memcpy( &m_PcParam->Stat2, &aMsg->theStat, sizeof( SRpdSTAT2 ) );
		

		if(new_position  == n_WearSlot_Serve )	
		{
			g_Particle.m_EffectManager.DeleteItemLinkEffect( n_WearSlot_SKIN_Serve , ( Character * ) g_Pc.GetPlayer() );
		}
		
		g_Particle.m_EffectManager.DeleteItemLinkEffect( new_position , ( Character * ) g_Pc.GetPlayer() );
		
		bool bFlag = false;
#ifdef RIDING_SYSTEM
		if( new_position == n_WearSlot_Riding )
		{
			bFlag = true;						
		}
#endif		
		if(bFlag)
		{
			SPcItem *pTempItem = NULL;
			
			for( int i = 0; i < RP_MAX_EQUIP ; ++i )
			{
				if( ( m_PcInven->WearItem[i] != NULL ) && 
					( m_PcInven->WearItem[i]->ItemUID == aMsg->thePreItemID ) )
				{
					if(m_PcInven->InvenHorse[pre_position])
					{
						///-- 말 아이템 슬롯이 서로 바뀌는 순간 오류발생
						///-- 예.. 5번이 기본슬롯인 흑마가 10번인 갈색말 슬롯으로 옮겨질 경우

						///-- 재접하면 다시 나타남
					}
					
					pTempItem = m_PcInven->InvenHorse[pre_position];
					m_PcInven->InvenHorse[pre_position] = m_PcInven->WearItem[i];
					m_PcInven->WearItem[i] = NULL;			
					break;
				}		
			}
			
			if(pTempItem)
			{
				m_PcInven->WearItem[new_position] = pTempItem;
			}
			else
			{
				for(short i = 0; i < RP_MAX_QUEST_ITEM ; ++i )
				{
					if( ( m_PcInven->InvenHorse[i] != NULL ) && 
						( m_PcInven->InvenHorse[i]->ItemUID == aMsg->theNewItemID ) )
					{
						m_PcInven->WearItem[new_position] = m_PcInven->InvenHorse[i];
						m_PcInven->InvenHorse[i] = NULL;			
						break;
					}		
				}
			}
		}
		else
		{
			SPcItem *pTempItem = NULL; 
			
			for( int i = 0; i < RP_MAX_EQUIP ; ++i )
			{
				if( ( m_PcInven->WearItem[i] != NULL ) && 
					( m_PcInven->WearItem[i]->ItemUID == aMsg->thePreItemID ) )
				{
					pTempItem = m_PcInven->InvenItem[pre_position];
					m_PcInven->InvenItem[pre_position] = m_PcInven->WearItem[i];
					m_PcInven->WearItem[i] = NULL;			
					break;
				}		
			}
			
			if(pTempItem)
			{
				m_PcInven->WearItem[new_position] = pTempItem;
			}
			else
			{				
				for(short i = 0; i < c_MaxInvenItemNum ; ++i )
				{
					if( ( m_PcInven->InvenItem[i] != NULL ) && 
						( m_PcInven->InvenItem[i]->ItemUID == aMsg->theNewItemID ) )
					{
						m_PcInven->WearItem[new_position] = m_PcInven->InvenItem[i];
						m_PcInven->InvenItem[i] = NULL;			
						break;
					}		
				}
			}
		}
		
		//슬롯 재설정....
		InventoryWnd.UpdateInvenSlotItem();
		InventoryWnd.UpdateWearSlotItem();
		
		//커서에 있는 Item 지우기....
		nRui->ResetCursorItem();

		//교체될 슬롯이 기본날개 인벤 이면 
		if( (new_position == n_WearSlot_Serve) && m_PcInven->WearItem[n_WearSlot_SKIN_Serve] )
		{
			g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[ n_WearSlot_SKIN_Serve]->Code , g_Pc.GetPlayer() );
		}

		g_Pc.GetPlayer()->Item_Equip( m_PcInven->WearItem[new_position]->Code, new_position );
		
		g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
			g_Pc.m_MainPC.char_info.race , g_lpDevice, false );
		g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
			g_Pc.m_MainPC.char_info.race , g_lpDevice );		
		
		nRui->thePcParam.MaxHP = aMsg->theMaxHP;
		nRui->thePcParam.MaxMP = aMsg->theMaxMP;
		nRui->thePcParam.MaxSP = aMsg->theMaxSP;
		nRui->thePcParam.Power = aMsg->thePower;
		nRui->thePcParam.Vital = aMsg->theVital;
		nRui->thePcParam.Sympathy = aMsg->theSympathy;
		nRui->thePcParam.Intel = aMsg->theInt;		
		nRui->thePcParam.Stamina = aMsg->theStamina;		
		nRui->thePcParam.Dex = aMsg->theDex;	
		
		nRui->Refresh_ItemHelpTip();
	}
	return;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScSeeEquip( SRpScSeeEquip* aMsg )
{
	RLGS( "**** PROC_SC -> SEE_EQUIP ****" );
	
	int tbl_index = g_Pc.FindUser( (long)aMsg->thePCID );
	
	if( tbl_index != MAX_USER_INTABLE )
	{

		//상대 스킨정보는 장착기본슬롯으로 들어간다. 
	//	int iPosion = aMsg->thePosition;

		switch(aMsg->thePosition)
		{
		case n_WearSlot_SKIN_Suit:	aMsg->thePosition = n_WearSlot_CoatArmor;break;
		case n_WearSlot_SKIN_Helmet:aMsg->thePosition = n_WearSlot_Helmet; break;
		case n_WearSlot_SKIN_Serve:	aMsg->thePosition = n_WearSlot_Serve; break;
		}

		
		g_Particle.m_EffectManager.DeleteItemLinkEffect( aMsg->thePosition , ( Character *) &g_Pc.m_Pc[tbl_index] );
		
		//.....................................................................................................
		// 바뀐 아이템을 넣어주기 By wxywxy
		//.....................................................................................................

		g_Pc.m_PcTable[tbl_index].char_info.theWearItem[aMsg->thePosition].theCode      = aMsg->theItem.theCode;
		g_Pc.m_PcTable[tbl_index].char_info.theWearItem[aMsg->thePosition].theEldaStone = aMsg->theItem.theEldaStone;
		//.....................................................................................................
		
		g_Pc.m_Pc[tbl_index].Item_Equip( aMsg->theItem.theCode, aMsg->thePosition );
		
		//by simwoosung - 파티 멤버 이미지의 갱신
		if(g_PartyInforWnd.IsMemberParty())
		{
			SPARTYMEMBER_INFO *pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->thePCID);
			if(pMemberInfo && pMemberInfo->m_bIsViewRange && pMemberInfo->m_bIsSee)
			{
				g_PartyInforWnd.ChangePCharImage(aMsg->thePCID);			
			}
		}
	}
	else
	{
		//예외 처리
		RLGS( "**** SC_FAILED -> SEE_EQUIP ****" );
	}
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScTakeoff( SRpScTakeoff* aMsg )
{
	RLGS( "**** PROC_SC -> TAKEOFF ****" );
	///--PRINT_DLG_LOG("ScTakeoff theItemID %d thePosition %d",aMsg->theItemID , aMsg->thePosition);
	int  position = aMsg->thePosition;
	int i = 0;
	bool bWearItem = false;
	//장비 벗기...( 장비창 -> 인벤으로 )
	if( GetItemProtocolResult( aMsg->theResult ) )
	{

		//장비 벗기...( 장비창 -> 인벤으로 )
		if(!bWearItem)
		{
			for( int i = 0; i < RP_MAX_EQUIP; ++i )
			{
				if( ( m_PcInven->WearItem[i] != NULL ) && ( m_PcInven->WearItem[i]->ItemUID == aMsg->theItemID ) )
				{

					bWearItem = true;  
					//아이템 효과 적용....
					memcpy( &m_PcParam->Stat2, &aMsg->theStat, sizeof( SRpdSTAT2 ) );
					
					//실제 적용....
					g_Pc.GetPlayer()->Item_TakeOff( i );

					bool bFlag = false;
					
#ifdef RIDING_SYSTEM				
					if( i == n_WearSlot_Riding )
					{
						bFlag = true;				
					}
					
					if( i == n_WearSlot_RidingItem1 || i == n_WearSlot_RidingItem2 ||
						i == n_WearSlot_RidingItem3 )
					{
						if(aMsg->theRidingFlag)
						{
							bFlag = true;
						}
						else
						{
							bFlag = false;
						}
					}
#endif					
					if(bFlag == false)
					{
						//장비 벗기				
						m_PcInven->InvenItem[position] = m_PcInven->WearItem[i];
						m_PcInven->WearItem[i] = NULL;
					}
					else
					{
						m_PcInven->InvenHorse[position] = m_PcInven->WearItem[i];
						m_PcInven->WearItem[i] = NULL;
					}

					//아이템 벗기 
					if( i == n_WearSlot_SKIN_Serve  && nRui->thePcInven.WearItem[ n_WearSlot_Serve] ) 
					{
						g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[ n_WearSlot_Serve]->Code , g_Pc.GetPlayer() );
					}

					//슬롯 재설정....
					InventoryWnd.UpdateInvenSlotItem();
					InventoryWnd.UpdateWearSlotItem();
					
					//커서에 있는 Item 지우기....
					nRui->ResetCursorItem();				
					
					g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
						g_Pc.m_MainPC.char_info.race , g_lpDevice, false );
					g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
						g_Pc.m_MainPC.char_info.race , g_lpDevice );
					
					break;
				}
			}
		}
		
		nRui->thePcParam.MaxHP = aMsg->theMaxHP;
		nRui->thePcParam.MaxMP = aMsg->theMaxMP;
		nRui->thePcParam.MaxSP = aMsg->theMaxSP;
		nRui->thePcParam.Power = aMsg->thePower;
		nRui->thePcParam.Vital = aMsg->theVital;
		nRui->thePcParam.Sympathy = aMsg->theSympathy;
		nRui->thePcParam.Intel = aMsg->theInt;		
		nRui->thePcParam.Stamina = aMsg->theStamina;		
		nRui->thePcParam.Dex = aMsg->theDex;	


#ifdef RIDING_SYSTEM
	if(!bWearItem)
		for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
		{
			if( ( m_PcInven->InvenHorse[i] != NULL ) && ( m_PcInven->InvenHorse[i]->ItemUID == aMsg->theItemID ) )
			{
				
				m_PcInven->InvenItem[position]  = m_PcInven->InvenHorse[i];
				m_PcInven->InvenHorse[i] = NULL;
				
				//슬롯 재설정
				InventoryWnd.UpdateInvenSlotItem();
				InventoryWnd.UpdateWearSlotItem();
				
				//커서에 있는 Item 지우기....
				nRui->ResetCursorItem();
				
				return;
			}
		}		
#endif

		


	}
	return;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScSeeTakeoff( SRpScSeeTakeoff* aMsg )
{
	RLGS( "**** PROC_SC -> SEE_TAKEOFF ****" );
	
	int tbl_index = g_Pc.FindUser( (long)aMsg->thePCID );
	
	if( tbl_index != MAX_USER_INTABLE )
	{

		switch(aMsg->thePosition)
		{
			case n_WearSlot_SKIN_Suit:	aMsg->thePosition = n_WearSlot_CoatArmor;break;
			case n_WearSlot_SKIN_Helmet:aMsg->thePosition = n_WearSlot_Helmet; break;
			case n_WearSlot_SKIN_Serve:	aMsg->thePosition = n_WearSlot_Serve; break;
		}

		g_Pc.m_Pc[tbl_index].Item_TakeOff( aMsg->thePosition );
		
		//.....................................................................................................
		// 바뀐 아이템을 넣어주기 By wxywxy
		//.....................................................................................................
		g_Pc.m_PcTable[tbl_index].char_info.theWearItem[aMsg->thePosition].theCode      = 0;
		g_Pc.m_PcTable[tbl_index].char_info.theWearItem[aMsg->thePosition].theEldaStone = 0;
		//.....................................................................................................
		
		//by simwoosung - 파티 멤버 이미지의 갱신
		if(g_PartyInforWnd.IsMemberParty())
		{
			SPARTYMEMBER_INFO *pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->thePCID);
			if(pMemberInfo && pMemberInfo->m_bIsViewRange && pMemberInfo->m_bIsSee)
			{
				g_PartyInforWnd.ChangePCharImage(aMsg->thePCID);			
			}
		}		
	}
	else
	{
		//예외 처리
		RLGS( "**** SC_FAILED -> SEE_TAKEOFF ****" );
	}
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScBuy( SRpScBuy* aMsg )
{
	RLGS( "**** PROC_SC -> BUY ****" );
	
	SPcItem*	tmpPcItem = NULL;
	
	// 상점과의 거래....살때( 상점 -> 인벤 )
	//aMsg->theMoney;// 구매후 남은 금액
	//aMsg->theResult;	// n_FULL_INVENTORY, n_SHORT_MONEY
	//aMsg->theItemData;// ( URpdItem )
	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		//커서에 있는 Item 지우기....
		nRui->ResetCursorItem();
		
		//아이템 효과 적용....
		m_PcParam->Money = aMsg->theMoney;// 구매후 금액
		
		// SPcItem객체생성
		tmpPcItem = SAFE_NEW( SPcItem );
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->theItemData.theItem.theCode );
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
		{
			tmpPcItem->IType		= n_ITYPE_WEAR;
			tmpPcItem->Amount		= 1;
			
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItemData), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			tmpPcItem->IType	= n_ITYPE_USE;//소모품
			tmpPcItem->Amount	= aMsg->theItemData.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			tmpPcItem->IType	= n_ITYPE_UNUSE;//비소모품
			tmpPcItem->Amount	= aMsg->theItemData.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
		{
			tmpPcItem->IType	= n_ITYPE_SPECIAL; 
			tmpPcItem->Amount	= aMsg->theItemData.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_PersonalPet )
		{
			tmpPcItem->IType	= n_ITYPE_PET_USE; 
			tmpPcItem->Amount	= aMsg->theItemData.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			tmpPcItem->IType	= n_ITYPE_RIDE; 
			tmpPcItem->Amount	= 1;
			
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItemData), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{
			tmpPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			tmpPcItem->Amount	= 1;
			
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItemData), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{
			tmpPcItem->IType = n_ITYPE_SKIN_WEAR;
			tmpPcItem->Amount	= 1;
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItemData), sizeof( URpdItem ) );
		}
		tmpPcItem->Class		= ItemType;
		tmpPcItem->ItemUID		= aMsg->theItemData.theItem.theItemUID;
		tmpPcItem->Code			= aMsg->theItemData.theItem.theCode;
		tmpPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theItemData.theItem.theCode );
		tmpPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItemData.theItem.theCode );
		
		if( tmpPcItem->ItemTable == NULL )
		{
			SAFE_DELETE( tmpPcItem );	
			return;
		}

		////-- 현재 아이템에 관련된 딜레이가 적용되었는지 체크해서 넣어준다.					
		if( tmpPcItem->IType == n_ITYPE_USE && tmpPcItem->ItemTable )
		{
			 int iDelayGroup = tmpPcItem->ItemTable->theDelayGroup;
			 if( !(nRui->enableDelayTime[iDelayGroup] ) )						
			 {
				tmpPcItem->IsDelay = true;						
			 }
		}		
		
		// 왼손,오른손....
		//---------------------------------------------------------------------
		if( ( tmpPcItem->ItemTable->theType == n_Type_MagicBook ) || ( tmpPcItem->ItemTable->theType == n_Type_Bow ) || ( tmpPcItem->ItemTable->theType == n_Type_Shield ) )
		{
			tmpPcItem->Class = n_Class_Shield;
		}
		else if( ( tmpPcItem->ItemTable->theType == n_Type_Sword ) || ( tmpPcItem->ItemTable->theType == n_Type_TwohandSword ) ||
			( tmpPcItem->ItemTable->theType == n_Type_Staff ) || ( tmpPcItem->ItemTable->theType == n_Type_CrystalBall ) )
		{
			tmpPcItem->Class = n_Class_Weapon;
		}
		//---------------------------------------------------------------------		
		
		for( int i = 0; i < RP_MAX_QUICK_SLOT; ++i )
		{
			if( ( m_PcInven->QuickSlot[i] != NULL ) && ( m_PcInven->QuickSlot[i]->ItemUID == tmpPcItem->ItemUID ) )
			{
				m_PcInven->QuickSlot[i] = tmpPcItem;			
				break;
			}
		}
		
		SAFE_DELETE( m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition] );		
		m_PcInven->InvenItem[aMsg->theItemData.theItem.thePosition] = tmpPcItem;
		

		//슬롯 재설정....
		MainMenuWnd.UpdateQuickSlotItem();		
		InventoryWnd.UpdateInvenSlotItem();
	}
	return;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScSell( SRpScSell* aMsg )
{
	RLGS( "**** PROC_SC -> SELL ****" );
	
	// 상점과의 거래....팔때( 인벤 -> 상점  )
	int i = 0;
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		switch( aMsg->theItemTab )
		{
		case n_WearTab:
			for( i = 0; i < RP_MAX_EQUIP; ++i )
			{
				if( ( m_PcInven->WearItem[i] != NULL ) && ( m_PcInven->WearItem[i]->ItemUID == aMsg->theItemID ) )
				{

#ifdef C_TIME_SUIT_UPDATE
					if(m_PcInven->WearItem[i]->ItemTable->theRare == 255 && 
						m_PcInven->WearItem[i]->ItemTable->theMaxEndurance == 0 )
					{			
						g_ListTime.DeletePayItem(aMsg->theItemID);
					}				
#endif


					delete( m_PcInven->WearItem[i] );
					m_PcInven->WearItem[i] = NULL;
					
					//아이템 효과 적용....
					m_PcParam->Money = aMsg->theMoney;// 판매후 금액
					
					//캐릭터에 실제 적용....
					g_Pc.GetPlayer()->Item_TakeOff( i );
					break;
				}
			}
			
			InventoryWnd.UpdateWearSlotItem();
			break;
		case n_InvenTab:
			for( i = 0; i < c_MaxInvenItemNum; ++i )
			{
				if( ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theItemID ) )
				{


#ifdef C_TIME_SUIT_UPDATE
					if(m_PcInven->InvenItem[i]->ItemTable->theRare == 255 && 
						m_PcInven->InvenItem[i]->ItemTable->theMaxEndurance == 0 )
					{			
						g_ListTime.DeletePayItem(aMsg->theItemID);
					}				
#endif

					//커서에 있는 Item 지우기....
					nRui->ResetCursorItem();
					
					//아이템 효과 적용....
					m_PcParam->Money = aMsg->theMoney;// 판매후 금액			
					
					//판매후 남은 번들 수. 0 인 경우 아이템 삭제.
					m_PcInven->InvenItem[i]->Amount = aMsg->theBundle;
					if( m_PcInven->InvenItem[i]->Amount == 0 )
					{
						//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
						for( int j = 0; j < RP_MAX_QUICK_SLOT; ++j )
						{
							if( m_PcInven->InvenItem[i] == m_PcInven->QuickSlot[j] )
							{
								m_PcInven->QuickSlot[j] = NULL;
								break;
							}
						}
						
						delete( m_PcInven->InvenItem[i] );
						m_PcInven->InvenItem[i] = NULL;
					}
					
					//슬롯에 적용....
					MainMenuWnd.UpdateQuickSlotItem();
					InventoryWnd.UpdateInvenSlotItem();	
					break;
				}
			}
			break;
		case n_QuestTab:
			break;
		}		
	}
	return;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScCombine( SRpScCombine* aMsg )
{
	RLGS( "**** PROC_SC -> COMBINE ****" );
	
	// 인벤내에서....합치기...
	int Item1Position = 0;
	int Item2Position = 0;
	int i = 0;


	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		switch( aMsg->theItemTab )
		{
		case n_WearTab:
			break;
		case n_InvenTab:
			for( i = 0; i < c_MaxInvenItemNum; ++i )
			{
				if(  ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theItemID1 ) ){	Item1Position = i;	}
				if(  ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theItemID2 ) ){	Item2Position = i;	}
			}
			
			m_PcInven->InvenItem[Item1Position]->Amount = aMsg->theBundle1;
			m_PcInven->InvenItem[Item2Position]->Amount = aMsg->theBundle2;
			
			//커서에 있는 Item 지우기....
			nRui->ResetCursorItem();
			
			//합치고 남은 번들.... 0이면 삭제...
			if( m_PcInven->InvenItem[Item2Position]->Amount == 0 )
			{
				//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
				for( int j = 0; j < RP_MAX_QUICK_SLOT; ++j )
				{
					if( m_PcInven->InvenItem[Item2Position] == m_PcInven->QuickSlot[j] )
					{
						m_PcInven->QuickSlot[j] = NULL;
						break;
					}
				}
				
				delete( m_PcInven->InvenItem[Item2Position] );
				m_PcInven->InvenItem[Item2Position] = NULL;
			}
			
			//슬롯에 적용....
			MainMenuWnd.UpdateQuickSlotItem();
			InventoryWnd.UpdateInvenSlotItem();	
			break;
		case n_QuestTab:
			for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
			{
				if(  ( m_PcInven->InvenQuest[i] != NULL ) && ( m_PcInven->InvenQuest[i]->ItemUID == aMsg->theItemID1 ) ){	Item1Position = i;	}
				if(  ( m_PcInven->InvenQuest[i] != NULL ) && ( m_PcInven->InvenQuest[i]->ItemUID == aMsg->theItemID2 ) ){	Item2Position = i;	}
			}
			
			m_PcInven->InvenQuest[Item1Position]->Amount = aMsg->theBundle1;
			m_PcInven->InvenQuest[Item2Position]->Amount = aMsg->theBundle2;
			
			//합치고 남은 번들.... 0이면 삭제...
			if( m_PcInven->InvenQuest[Item2Position]->Amount == 0 )
			{
				delete( m_PcInven->InvenQuest[Item2Position] );
				m_PcInven->InvenQuest[Item2Position] = NULL;
			}
			
			//슬롯에 적용....
			InventoryWnd.UpdateInvenSlotItem();	
			//커서에 있는 Item 지우기....
			nRui->ResetCursorItem();
			break;
		}
	}
	nRui->m_KeyLock.Inventory.SetUnLock();
	return;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScChangeSlot( SRpScChangeSlot* aMsg )
{
	RLGS( "**** PROC_SC -> CHANGE_SLOT ****" );
	///--PRINT_DLG_LOG("ScChangeSlot theItemID %d theItemID2 %d theSlot %d" ,aMsg->theItemID ,  aMsg->theItemID2 , aMsg->theSlot  );
	// 인벤내에서....자리 이동
	//aMsg->theItemID;
	//aMsg->theItemID2;
	//aMsg->theSlot;
	//aMsg->theResult;
	
	int			position = 0;
	SPcItem*	tmpPcItem = NULL;
	int			i = 0;

	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		switch( aMsg->theItemTab )
		{
		case n_WearTab:
			break;
		case n_InvenTab:
			for( i = 0; i < c_MaxInvenItemNum; ++i )
			{
				if( ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theItemID ) )
				{
					if( aMsg->theItemID2 == 0 )//이동....
					{
						position = aMsg->theSlot;
						m_PcInven->InvenItem[position] = m_PcInven->InvenItem[i];
						m_PcInven->InvenItem[i] = NULL;
					}
					if( aMsg->theSlot == 255 )//스왑....
					{
						for( int j = 0; j < c_MaxInvenItemNum; ++j )
						{
							if( ( m_PcInven->InvenItem[j] != NULL ) && ( m_PcInven->InvenItem[j]->ItemUID == aMsg->theItemID2 ) )
							{
								tmpPcItem = m_PcInven->InvenItem[i];
								m_PcInven->InvenItem[i] = m_PcInven->InvenItem[j];
								m_PcInven->InvenItem[j] = tmpPcItem;
								break;
							}
						}
					}
					
					//슬롯 재설정
					InventoryWnd.UpdateInvenSlotItem();
					InventoryWnd.UpdateWearSlotItem();
					
					//커서에 있는 Item 지우기....
					nRui->ResetCursorItem();
					break;	
				}
			}
			break;
		case n_QuestTab:
			for( i = 0; i < RP_MAX_QUEST_ITEM; ++i )
			{
				if( ( m_PcInven->InvenQuest[i] != NULL ) && ( m_PcInven->InvenQuest[i]->ItemUID == aMsg->theItemID ) )
				{
					if( aMsg->theItemID2 == 0 )//이동....
					{
						position = aMsg->theSlot;
						m_PcInven->InvenQuest[position] = m_PcInven->InvenQuest[i];
						m_PcInven->InvenQuest[i] = NULL;
					}
					if( aMsg->theSlot == 255 )//스왑....
					{
						for( int j = 0; j < RP_MAX_QUEST_ITEM; ++j )
						{
							if( ( m_PcInven->InvenQuest[j] != NULL ) && ( m_PcInven->InvenQuest[j]->ItemUID == aMsg->theItemID2 ) )
							{
								tmpPcItem = m_PcInven->InvenQuest[i];
								m_PcInven->InvenQuest[i] = m_PcInven->InvenQuest[j];
								m_PcInven->InvenQuest[j] = tmpPcItem;
								break;
							}
						}
					}
					
					//슬롯 재설정
					InventoryWnd.UpdateInvenSlotItem();
					
					//커서에 있는 Item 지우기....
					nRui->ResetCursorItem();
					break;	
				}
			}
			break;
		}
	}

	nRui->m_KeyLock.Inventory.SetUnLock();
	return;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScLinkQuickSlot( SRpScLinkQuickSlot* aMsg )
{
	//	SRpdQuickSlot	theQuickSlot1;
	//	SRpdQuickSlot	theQuickSlot2;		// theQuickSlot2의 ID가 0이면 퀵슬롯 추가, ID 있으면 ChangeQuickSlot !!!
	//	BYTE			theResult;
	
	//union _QuickSlotID
	//{
	//	DWORD			theItemID;
	//	WORD			theSkillID;
	//};
	//	_QuickSlotID	theID;				// ItemID, SkillCode
	//	BYTE			theType:2;			// Skill(3), Item(2)
	//	BYTE			thePosition:6;		// Slot Pos 50개
	//	BYTE			theResult;
	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		if( aMsg->theQuickSlot2.theType == 0 )
		{
			int			position = 0;
			SPcItem*	tmpPcItem = NULL;
			int			i = 0;			
			
			///-- Skill
			if( aMsg->theQuickSlot1.theType == n_SKILL )
			{
				SPcItem* link_pcitem = NULL;
				
				// 스킬 아이디로 PcItem 찾는다. 
				for( i = 0; i < RP_MAX_USER_SKILL; ++i )
				{
					if( ( m_PcInven->SkillItem[i] != NULL ) && ( m_PcInven->SkillItem[i]->Code == aMsg->theQuickSlot1.theID.theSkillID ) )
					{
						tmpPcItem = m_PcInven->SkillItem[i];
						break;
					}
				}
				
				m_PcInven->QuickSlot[ aMsg->theQuickSlot1.thePosition ] = tmpPcItem;
				
				//	position = aMsg->theQuickSlot1.thePosition % 10;
				//	g_Pc.GetSkillSystem()->theQuickSlot_SkillCode[ position ] = aMsg->theQuickSlot1.theID.theSkillID;
				
			}
			else if( aMsg->theQuickSlot1.theType == n_ITEM  )
			{
				for( i = 0; i < c_MaxInvenItemNum; ++i )
				{
					if( ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theQuickSlot1.theID.theItemID ) )
					{
						tmpPcItem = m_PcInven->InvenItem[i];
						break;
					}
				}
				
				m_PcInven->QuickSlot[ aMsg->theQuickSlot1.thePosition ] = tmpPcItem;
			}
			
			MainMenuWnd.UpdateQuickSlotItem();
			MainMenuWnd.QuickSlotLineChange();
			
		}
		else			// 퀵슬롯과 퀵슬롯의 ChangeSlot 
		{
			
			int			position = 0;
			SPcItem*	tmpPcItem = NULL;
			int			i = 0;			
			
			if( aMsg->theQuickSlot1.theType == n_SKILL )
			{
				SPcItem * link_pcitem = NULL;
				
				// 스킬 아이디로 PcItem 찾는다. 
				for( i = 0; i < RP_MAX_USER_SKILL; ++i )
				{
					if( ( m_PcInven->SkillItem[i] != NULL ) && 
						( m_PcInven->SkillItem[i]->Code == aMsg->theQuickSlot1.theID.theSkillID ) )
					{
						tmpPcItem = m_PcInven->SkillItem[i];
						break;
					}
				}
				
				m_PcInven->QuickSlot[ aMsg->theQuickSlot1.thePosition ] = tmpPcItem;
				
				//	position = aMsg->theQuickSlot1.thePosition % 10;
				//	g_Pc.GetSkillSystem()->theQuickSlot_SkillCode[ position ] = aMsg->theQuickSlot1.theID.theSkillID;
				
			}
			else if( aMsg->theQuickSlot1.theType == n_ITEM  )
			{
				for( i = 0; i < c_MaxInvenItemNum; ++i )
				{
					if( ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theQuickSlot1.theID.theItemID ) )
					{
						tmpPcItem = m_PcInven->InvenItem[i];
						break;
					}
				}
				
				m_PcInven->QuickSlot[ aMsg->theQuickSlot1.thePosition ] = tmpPcItem;
			}
			
			if( aMsg->theQuickSlot2.theType == n_SKILL )
			{
				SPcItem* link_pcitem = NULL;
				
				// 스킬 아이디로 PcItem 찾는다. 
				for( i = 0; i < RP_MAX_USER_SKILL; ++i )
				{
					if( ( m_PcInven->SkillItem[i] != NULL ) && 
						( m_PcInven->SkillItem[i]->Code == aMsg->theQuickSlot2.theID.theSkillID ) )
					{
						tmpPcItem = m_PcInven->SkillItem[i];
						break;
					}
				}
				
				m_PcInven->QuickSlot[ aMsg->theQuickSlot2.thePosition ] = tmpPcItem;
				
				//position = aMsg->theQuickSlot2.thePosition % 10;
				
				// dongs//////////////////////////////////////////////////////////////////////////
				// 데코시스템에서는 theQuickSlot_SkillCode[10] 화면에 보이는 퀙슬롯 만 갱신해 준다 .. 
				// theQuickSlot2 정보는 화면에 안보이는것이기 때문에 패스~~ 
				//g_Pc.GetSkillSystem()->theQuickSlot_SkillCode[ position ] = aMsg->theQuickSlot2.theID.theSkillID;
				
			}
			else if( aMsg->theQuickSlot2.theType == n_ITEM  )
			{
				for( i = 0; i < c_MaxInvenItemNum; ++i )
				{
					if( ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theQuickSlot2.theID.theItemID ) )
					{
						tmpPcItem = m_PcInven->InvenItem[i];
						break;
					}
				}
				
				m_PcInven->QuickSlot[ aMsg->theQuickSlot2.thePosition ] = tmpPcItem;
			}			
			
			MainMenuWnd.UpdateQuickSlotItem();
			MainMenuWnd.QuickSlotLineChange();
		}
	}
	
	nRui->m_KeyLock.QuickSlotKey.SetUnLock();
	//nRui->m_KeyLock.QuickSlotKey.bflag = false;
	return;
}

//-----------------------------------------------------------------------------
void	CRockClient::Proc_RpScUnlinkQuickSlot( SRpScUnlinkQuickSlot* aMsg )
{
	//	BYTE			thePosition;
	//	BYTE			theResult;
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		m_PcInven->QuickSlot[ aMsg->thePosition ] = NULL;
		
		MainMenuWnd.UpdateQuickSlotItem();
	}
	
	return;
}

//-----------------------------------------------------------------------------
void CRockClient::Proc_RpScAppearItem( SRpScAppearItem* aMsg )
{
	//	RLGS( "**** PROC_SC -> APPEAR_ITEM ****" );
	
	//	RLG3( "****ScAppearItem : ItemID = [%u] Cell = %u ItemCode = %u",
	//		aMsg->theItemID, aMsg->theCell, aMsg->theItemCode );
	///--PRINT_DLG_LOG("ScAppearItem theItemID %d theCell %d aMsg->mCode %d theNPCID %d " , aMsg->theItemID , aMsg->theCell , aMsg->theItemCode ,aMsg->theNPCID );

	static int nCount = 0;
	
	int npc_tbl_index = g_Map.FindNpc( (long)aMsg->theItemID, c_NpcRace_Item );
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		int npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID, c_NpcRace_Item );
		if( npc_tbl_index == MAX_NPC_INTABLE )
		{
			g_Map.SM__ITEM__APPEAR( aMsg->theItemID, aMsg->theCell, aMsg->theItemCode );		
		}
		else
		{
			s_AppearItem AppearItemList;
			AppearItemList.ItemID = aMsg->theItemID;
			AppearItemList.Cell = aMsg->theCell;
			AppearItemList.ItemCode = aMsg->theItemCode;
			AppearItemList.NPCID = aMsg->theNPCID;
			g_Map.theAppearItemList.push_back( AppearItemList );
		}
	}
	else
	{
		RLGS( " Already Exist ITEM : ScAppearITEM" );
		return;		
	}
	
	return;
}

//-----------------------------------------------------------------------------
void CRockClient::Proc_RpScDisappearItem( SRpScDisappearItem* aMsg )
{
	//	RLGS( "**** PROC_SC -> DISAPPEAR_ITEM ****" );
	
	//	RLG1( "*****ScDisappearItem : [%u]", aMsg->theItemID );
	///--PRINT_DLG_LOG("ScDisappearItem ItemID %d", aMsg->theItemID);
	int npc_tbl_index = g_Map.FindNpc( (long)aMsg->theItemID, c_NpcRace_Item );
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		//		RLGS( " Nothing ITEM : ScDisappearItem" );
		
		// 혹시 아직 생성되지 않는 아이템을 지워라고 할지도 모르니까...검색해서 있으면 지워준다 
		int idx = 0;
		std::vector<s_AppearItem>::iterator ir;							
		ir = g_Map.theAppearItemList.begin();
		for( ; ir != g_Map.theAppearItemList.end() ; ++ir )
		{
			if( (*ir).NPCID == aMsg->theItemID )
			{
				g_Map.theAppearItemList.erase( ir );
				break;
			}
		}					
		return;
	}
	
	g_Map.SM__ITEM__DISAPPEAR( aMsg->theItemID );
	
	if( NonPlayerInfoWnd.thePickedNpcID == npc_tbl_index )
	{
		NonPlayerInfoWnd.thePickedNpcID = -1;
		nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		
		if(g_NonPlayerInfoAdd2Wnd.IsVisible)
		{
			g_NonPlayerInfoAdd2Wnd.Close();
		}
	}		
	
	return;
}
   
///---------------------------------------------------------------------------
///-- Proc_RpScAlive
///---------------------------------------------------------------------------
void CRockClient::Proc_RpScAlive( SRpScAlive* aMsg )
{
	RLGS( "*****Proc_RpScAlive*****" );
	
	int i = 0;
	
	///-----------------------------------------------------------------------
	///-- 아이템 제거 
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{	
		if( m_PcInven->InvenItem[i] == NULL )
		{
			continue;
		}
		
		if ( m_PcInven->InvenItem[i] && 
			m_PcInven->InvenItem[i]->ItemUID == aMsg->thePayItem ) 
		{
			///-- Amount 변경

			m_PcInven->InvenItem[i]->Amount -= 1;
			///-- 아이템 완전 삭제
			if( m_PcInven->InvenItem[i]->Amount < 1 )
			{	
				delete( m_PcInven->InvenItem[i] );
				m_PcInven->InvenItem[i] = NULL;
				break;
			}		
		}
	}
	
	InventoryWnd.UpdateInvenSlotItem();
	
	g_Pc.GetPlayer()->m_curt_event.Clear();
	g_Pc.GetPlayer()->AnimationPlay( n_Stand_Peace9 );
	
	for( i = 0; i < c_Max_WearItemType; ++i )
	{
		g_Pc.GetPlayer()->m_pMaterial[i].Diffuse.a = 1.0f;
	}
	
	g_Pc.m_MainPC.char_info.theRealHP = aMsg->theHP;
	g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
	g_Pc.GetPlayer()->SetRealDie(FALSE);
	
	nRui->thePcParam.Stat1.theHP = aMsg->theHP;
	nRui->thePcParam.Stat1.theMP = aMsg->theSMP;
	nRui->thePcParam.Stat1.theSP = aMsg->theSMP;
	nRui->SendEvent( WID_AliveWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	
	nRui->thePcParam.Stat1.theGage = 0;
	g_Pc.GetPlayer()->theActionSystem.SetComboGauge();
	nRui->thePcParam.Stat1.theGage = 0;
	g_Pc.GetPlayer()->theActionSystem.SetProimaGauge();
	nRui->thePcParam.Stat1.theGage = 0;
	g_Pc.GetPlayer()->theActionSystem.SetConditionGauge();
	
	SItemBaseInfo* item_Info = NULL;
	
	if( nRui->thePcInven.WearItem[n_WearSlot_Shield] )
	{
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code );
		
		if( item_Info && item_Info->theType != n_Type_Shield ) 
		{
			//			g_Pc.GetPlayer()->m_Weapon_DelayTime = c_WeaponDelayTime - (item_Info->theAttackSpeed * 100);		
			g_Pc.GetPlayer()->m_Weapon_DelayTime = nRui->GetPcParamInfo()->Stat2.theAttackSpeed * 100;		
		}
	}
	else if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
	{		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
		
		if( item_Info )
		{
			//			g_Pc.GetPlayer()->m_Weapon_DelayTime = c_WeaponDelayTime - (item_Info->theAttackSpeed * 100);	
			g_Pc.GetPlayer()->m_Weapon_DelayTime = nRui->GetPcParamInfo()->Stat2.theAttackSpeed * 100;		
		}
	}
	else
	{
		g_Pc.GetPlayer()->m_Weapon_DelayTime = c_WeaponDelayTime;
	}
	
	g_Pc.GetPlayer()->m_ani_percent = 1.0f;	
	g_Pc.GetPlayer()->m_move_percent = 1.0f;		
	
	if( g_Map.m_NowMapID == aMsg->theMap )
	{
		if( g_Pc.m_MainPC.lDestBlock == aMsg->theCell )
		{	
			g_Pc.GetPlayer()->SetTilePos( (int)(aMsg->theCell%g_Pc.m_cSize), (int)(aMsg->theCell/g_Pc.m_cSize) );				
			g_Camera.m_unique = -1;
			g_Pc.GetPlayer()->m_nViewRefDestIndex = -1;
			g_Camera.Update();
			g_Map.UpdateTerrain( (int)(aMsg->theCell%g_Pc.m_cSize), (int)(aMsg->theCell/g_Pc.m_cSize) );	
		}
		else
		{
			// 유저 및 NPC 전부 disappear 시킨다...
			// g_Pc.InitTable();
			// g_Pc.FinalCleanup();			
			// g_Map.CleanupNpc();
			
			g_Pc.GetPlayer()->SetTilePos( (int)(aMsg->theCell%g_Pc.m_cSize), (int)(aMsg->theCell/g_Pc.m_cSize) );				
			g_Camera.m_unique = -1;	
			g_Pc.GetPlayer()->m_nViewRefDestIndex = -1;
			g_Camera.Update();
			g_Map.UpdateTerrain( (int)(aMsg->theCell%g_Pc.m_cSize), (int)(aMsg->theCell/g_Pc.m_cSize) );	
		}
	}
	else		// 전부 지우고 다시 생성
	{
		// 워프시 이펙트
		// 자기꺼는 안보여주기 ( 맵 로딩시 정지 상태 )
		LoadingBarWnd.Open();
		
		BeginLoadingBarThread();
		
		g_Pc.SetupTempPc( g_Pc.m_MainPC );
		g_Pc.m_TempPC.lCurMap = aMsg->theMap;
		g_Pc.m_TempPC.lBlock = aMsg->theCell;
		g_Pc.m_TempPC.lDestBlock = aMsg->theCell;
		
		//		g_Map.m_NowMapID = aMsg->theMap;
		if( g_RockClient.MapChange( aMsg->theMap ) == false )
		{
			ClientClose( g_RockClient.GetApp() , 1);
			return;
		}

		///-- 로딩바가 종료할때까지 기다려 안전적인 처리
		LoadingBarSafeStopAfterProcess();
	}  
	
	if( g_Pc.GetPlayer()->theMapType == n_FightZone )
	{		
		g_Pc.GetPlayer()->CM__READY__WARP( g_nowTime, n_FightZone );		 			
	}
	else
	{
		g_Pc.GetPlayer()->CM__READY__WARP( g_nowTime, n_NormalZone );		 				
	}	
	
	// 다른 유저 및 몬스터 표시창 
	if( NonPlayerInfoWnd.IsVisible )	
	{ 
		NonPlayerInfoWnd.thePickedUserID = -1;
		NonPlayerInfoWnd.thePickedNpcID = -1;
		nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
	
    //스킬 및 아이템 딜레이 초기화 
    g_SlotDelayProc.InitiallizeUseDelayTime();
	g_SlotDelayProc.InitiallizeSkillDelayTime();
	
	// 부활 효과 이펙트
	g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_REVIVE , 0 , 2.6f , g_Pc.GetMainPCUniqe() , -1 );
	g_Sound.Play( 24 , NULL );
	
	g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
		g_Pc.m_MainPC.char_info.race , g_lpDevice, false );
	g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
		g_Pc.m_MainPC.char_info.race , g_lpDevice );
	
	g_Pc.GetPlayer()->SetWearItem();
	
//	g_Particle.m_EffectManager.CreateWeaponEnChantEffect( ( Character * ) g_Pc.GetPlayer() , 255 );	
	
	//세트 플레이어만 .. 아이템인지 체크한다 .. 
	SItemBaseInfo* pSItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( g_Pc.m_MainPC.char_info.theWearItem[n_WearSlot_CoatArmor].theCode);
	
	if(pSItemBaseInfo && (n_Type_SetCloth == pSItemBaseInfo->theType))
	{
		g_Particle.m_EffectManager.CreateDefenceEnChantEffect( (Character * )g_Pc.GetPlayer()  , 255 );
	}
	
	g_AliveWnd.m_AliveType = n_AnswerType_None;
	
	int idx = 0;
	
	for( idx = 0; idx < RP_MAX_KEEPUP_SKILL; ++idx )
	{
		if( nRui->thePcInven.KeepupSkill[idx] )			
		{
			WORD aSkillCode = nRui->thePcInven.KeepupSkill[idx];
			nRui->thePcInven.KeepupSkill[idx] = 0;
			PlayerInfoWnd.DeleteKeepUpSkillCode( idx, aSkillCode );
		}
	}
	
	g_Pc.GetPlayer()->m_KeepUpChecker.Clear();
	
	///-- 부과효과 초기화
	for( idx = 0; idx < MAX_ADDEFFECT_NUM; ++idx )
	{
		g_Pc.GetPlayer()->theAddEffect_State[idx] = 0;
	}
	
	return;
}

void CRockClient::Proc_RpScSeeAliveOnSpot( SRpScSeeAliveOnSpot* aMsg )
{
	RLGS( "*****Proc_RpScSeeAliveOnSpot*****" );
	
	int aliveIdx = 0;
	
	aliveIdx = g_Pc.FindUser(aMsg->thePCID);
	
	if( aliveIdx == MAX_USER_INTABLE )
		return;
	
	g_Pc.m_Pc[aliveIdx].ClearEvent();
	
	for( int i = 0; i < c_Max_WearItemType; ++i )
	{
		g_Pc.m_Pc[aliveIdx].m_pMaterial[i].Diffuse.a = 1.0f;
	}
	
	g_Pc.m_PcTable[aliveIdx].char_info.theHP = aMsg->theHP;
	g_Pc.m_PcTable[aliveIdx].char_info.theSMP = aMsg->theSMP;
	g_Pc.m_PcTable[aliveIdx].char_info.theRealHP = aMsg->theHP;
	g_Pc.m_Pc[aliveIdx].m_lPreTime = g_nowTime;
	g_Pc.m_Pc[aliveIdx].SetRealDie(FALSE);	
	
	// 부활 효과 이펙트
	g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_REVIVE , 0 , 2.6f , aMsg->thePCID , -1 );
	g_Sound.Play( 24 , g_Pc.m_Pc[aliveIdx].GetPosTM()._41 , g_Pc.m_Pc[aliveIdx].GetPosTM()._42 , g_Pc.m_Pc[aliveIdx].GetPosTM()._43  );
	
	g_Particle.m_EffectManager.CreateWeaponEnChantEffect( ( Character * ) &g_Pc.m_Pc[aliveIdx] , 255 );
	
	
	//슈트착용한 사람만 .. .. 아이템인지 체크한다 .. 
	SItemBaseInfo* pSItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( g_Pc.m_PcTable[aliveIdx].char_info.theWearItem[n_WearSlot_CoatArmor].theCode);
	if(pSItemBaseInfo && (n_Type_SetCloth == pSItemBaseInfo->theType))
	{
		g_Particle.m_EffectManager.CreateDefenceEnChantEffect( ( Character * ) &g_Pc.m_Pc[aliveIdx] , 255 );
	}	
	
	//상대의 갑옷 .. 판단하여 세트 아이템이고 .. 엘다가 몇인지 구별한다 . .	
	
	DWORD pos = g_Pc.m_PcTable[aliveIdx].lDestBlock;
	g_Pc.m_Pc[aliveIdx].SetTilePos( (int)(pos%g_Pc.m_cSize), (int)(pos/g_Pc.m_cSize) );
	
	return;
}


void CRockClient::Proc_RpScGetExp( SRpScGetExp* aMsg )
{
	int exp = aMsg->theExp - nRui->thePcParam.Exp;
	
	if( exp > 0 )
	{
		SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
		if((g_PartyInforWnd.IsMemberParty()) && (aMsg->theFrom == n_PARTY))
		{
			Rsprintf(RWCHAR(g_strMessage) ,G_STRING_CHAR( IDS_SYSPR_GET_EXP_PARTY ),
				exp, pPartyInfo->m_ClassBonus, 
				pPartyInfo->m_MemberNumBonus,
				pPartyInfo->m_LevelPenalty);
			
			g_SystemPrintWnd.InsertSystemMsg((TCHAR *)g_strMessage, (UINT)n_LightBlue);
		}
		else
		{
            SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_EXP ), exp );
		}
		
#ifdef CPK_SYSTEM			
			
			int nDeRatio = ::GetDecreaseEXPRatioFromPKPoint( nRui->GetPcParam()->thePKPoint );
			if(nDeRatio)
			{
				Rsprintf( RWCHAR(g_strMessage) , G_STRING_CHAR( IDS_SYSCAOADDPANELTY ), nDeRatio );			
				g_SystemPrintWnd.InsertSystemMsg((TCHAR *)g_strMessage, (UINT)n_LightBlue);
			}
#endif		
		nRui->thePcParam.Exp = aMsg->theExp;
		
		// 경험치 표시 부분
		D3DXVECTOR3 vPos;
		
		vPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
		vPos.z = g_Pc.GetPlayer()->GetPosTM()._43;
		vPos.y = g_Pc.GetPlayer()->GetPosTM()._42 + ( g_Pc.GetPlayer()->GetBoundMaxVec().y - g_Pc.GetPlayer()->GetBoundMinVec().y ) * 1.5f;
		
		g_Particle.m_ExpDisplay.Add( exp , vPos );
	}
	else if( exp < 0 )
	{
		if( aMsg->theFrom == 255 )			// 죽었을때 패널티...
		{
			SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SUBTRACT_EXP ), abs( exp ) );	// IDS_SYSPR_SUBTRACT_EXP 사망패널티로 경험치가 %d가 감소되었습니다.
		}
		
		nRui->thePcParam.Exp = aMsg->theExp;
	}
	
	return;
}
  
void CRockClient::Proc_RpScLevelup( SRpScLevelup* aMsg )
{
#ifdef DECO_RENEWAL_GUILD_MJH

	std::vector < SFMemberInfo > :: iterator ir;							
			
	ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();
	
	for( ; ir != g_FrontierMemberListWnd.m_FrontierMemberInfoList.end() ; ++ir )
	{
		if( Rstrcmp( (*ir).Name, nRui->thePcParam.PCName ) == 0 )
		{
			(*ir).Level= aMsg->theLevel;
			break;
		}
	}	
	g_FrontierMainWnd.RefreshFrontierMyInfo();
	g_FrontierMainWnd.RefreshFrontierMyFrontier();
	g_FrontierMemberListWnd.RefreshMemberInfoList();

#endif // DECO_RENEWAL_GUILD_MJH
	SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEVELUP ) );
	
	nRui->thePcParam.Level = aMsg->theLevel;
	nRui->thePcParam.MaxHP = aMsg->theMaxHP;
	nRui->thePcParam.MaxSP = aMsg->theMaxSP;
	nRui->thePcParam.MaxMP = aMsg->theMaxMP;	

	nRui->thePcParam.Stat1.theHP = aMsg->theMaxHP;	
	g_Pc.m_MainPC.char_info.theRealHP = aMsg->theMaxHP;
	g_Pc.GetPlayer()->m_lLevelUpTime = timeGetTime();
	
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	
	if( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman )
	{
		nRui->thePcParam.Stat1.theSP = aMsg->theMaxSP;
		g_Pc.m_MainPC.char_info.theSMP = aMsg->theMaxSP;
	}
	else if( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman )
	{
		nRui->thePcParam.Stat1.theMP = aMsg->theMaxMP;
		g_Pc.m_MainPC.char_info.theSMP = aMsg->theMaxSP;
	}
	
	nRui->thePcParam.Power = aMsg->thePower;
	nRui->thePcParam.Vital = aMsg->theVital;
	nRui->thePcParam.Sympathy = aMsg->theSympathy;
	nRui->thePcParam.Intel = aMsg->theInt;		
	nRui->thePcParam.Stamina = aMsg->theStamina;		
	nRui->thePcParam.Dex = aMsg->theDex;		
	nRui->thePcParam.Exp = aMsg->theExp;
	
	int point = aMsg->thePoint - nRui->thePcParam.Point;
	if( point )
	{
		SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_ABPOINT ), point );
	}
	
	nRui->thePcParam.Point = aMsg->thePoint;			// 남은 능력치 업 포인트 (스킬 레벨 업 포인트는 따로 관리한다 SkillPoint1, SkillPoint2)
	nRui->thePcParam.Stat2 = aMsg->theStat2;
	
	point = aMsg->theSkillPoint - nRui->thePcParam.SkillPoint;	
	if( point )
	{
		SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_SKPOINT ), point );
	}
	
	nRui->thePcParam.SkillPoint = aMsg->theSkillPoint;
	nRui->thePcParam.TotalSkillPoint = aMsg->theTotalSkillPoint;	
	//nRui->SendEvent( WID_AbilityUpWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
	
	// 레벨업 이펙트
	g_Particle.SetEffSetCon( 0.0f , EF_LEVEL_UP , 0 , 3.1f , g_Pc.GetMainPCUniqe() , -1 );
	g_Sound.Play( 101 , NULL );
	
	// 레벨업 하면 스킬업 할 수 있는지 검사 한다. 
	g_SkillWnd.UpdateSkillSlotItem();

#ifdef APPLY_GUIDE
	g_pGuideWnd->PrintNowLevelGuide();
#endif

	g_Pc_Manager.ReSet__NpcSetVF( g_Map.m_NowMapID );
	g_Map.ReSetNpcQuestMark();
	
	return;
}


void CRockClient::Proc_RpScSeeLevelup( SRpScSeeLevelup * aMsg )
{
	int levelupIdx = 0;	
	
	SPARTYMEMBER_INFO *pMemberInfo = NULL;   
    pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->thePCID);
	if(pMemberInfo)
	{
		pMemberInfo->m_bLevel = aMsg->theLevel;   
		pMemberInfo = NULL;
	}

	pMemberInfo = g_LeagueInfoWnd.FindLeagueMember(aMsg->thePCID);
	if(pMemberInfo)
	{
		pMemberInfo->m_bLevel = aMsg->theLevel;   
		pMemberInfo = NULL;
	}

	// unique 를 가지고 테이블 위치를 알아낸다.
	levelupIdx = g_Pc.FindUser(aMsg->thePCID);
	
	if( levelupIdx == MAX_USER_INTABLE )
		return;

	g_Pc.m_PcTable[levelupIdx].char_info.theHP = aMsg->theMaxHP;				
	g_Pc.m_PcTable[levelupIdx].char_info.theLevel = aMsg->theLevel;
	g_Pc.m_PcTable[levelupIdx].char_info.theMaxHP = aMsg->theMaxHP;
	g_Pc.m_PcTable[levelupIdx].char_info.theMaxSMP = aMsg->theMaxSMP;	
	g_Pc.m_PcTable[levelupIdx].char_info.theSMP = aMsg->theMaxSMP;
	g_Pc.m_PcTable[levelupIdx].char_info.theRealHP = aMsg->theMaxHP;
	g_Pc.m_Pc[levelupIdx].m_lLevelUpTime = timeGetTime();
	
	// 레벨업 이펙트
	g_Particle.SetEffSetCon( 0.0f , EF_LEVEL_UP , 0 , 3.1f , aMsg->thePCID , -1 );
	g_Sound.Play( 101 , g_Pc.m_Pc[ levelupIdx ].GetPosTM()._41 ,
					   g_Pc.m_Pc[ levelupIdx ].GetPosTM()._42 ,
					   g_Pc.m_Pc[ levelupIdx ].GetPosTM()._43   );	

	g_Pc_Manager.ReSet__NpcSetVF( g_Map.m_NowMapID );
	g_Map.ReSetNpcQuestMark();
	
	return;
}

void CRockClient::Proc_RpScGetSkillPoint( SRpScGetSkillPoint* aMsg )
{
	WORD point = aMsg->theSkillPoint - nRui->thePcParam.SkillPoint;
	if( point )
	{
		SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_SKPOINT ), point );
	}
	
	nRui->thePcParam.SkillPoint = aMsg->theSkillPoint;
	nRui->thePcParam.TotalSkillPoint = aMsg->theTotalSkillPoint;
	
	return;
}

void CRockClient::Proc_RpScPoint( SRpScPoint* aMsg )
{
	if( aMsg->theResult != n_Success )
	{
		RLG1("Proc_RpScPoint : Error Result = %d", aMsg->theResult );
		
		switch( aMsg->theResult )
		{
		case n_NOT_ENOUGH_POINT:
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SHORTAGE_POINT ) );
			break;
		case n_WRONG_POINT:
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_FIAL_POINT_SET ) );
			break;			
		default:
			break;
		}		
		return;
	}
	
	nRui->thePcParam.MaxHP = aMsg->theMaxHP; 
	nRui->thePcParam.MaxSP = aMsg->theMaxSP; 
	nRui->thePcParam.MaxMP = aMsg->theMaxMP; 
	
	nRui->thePcParam.Power = aMsg->thePower;
	nRui->thePcParam.Vital = aMsg->theVital;
	nRui->thePcParam.Sympathy = aMsg->theSympathy;
	nRui->thePcParam.Intel = aMsg->theInt;		
	nRui->thePcParam.Stamina = aMsg->theStamina;		
	nRui->thePcParam.Dex = aMsg->theDex;		
	
	nRui->thePcParam.Stat2 = aMsg->theStat;
	
	nRui->thePcParam.Point = aMsg->thePoint;			// 남은 능력치 업 포인트 (스킬 레벨 업 포인트는 따로 관리한다 SkillPoint1, SkillPoint2)
	
	return;
}




void CRockClient::Proc_RpScScriptStart( SRpScScriptStart* aMsg )
{
	
#ifdef NEW_PVP_DECO
	if( aMsg->theResult == n_ERR_IS_NOT_SAME_CITIZEN)
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_SAME_CITIZEN)); 
		return;
	}
#endif	
	
	
	if( aMsg->theResult != n_Success )
	{
		return;
	}
	
	NpcChatWnd.Open();

	if(NpcChatWnd.theNpcCode == 0 )
	{
		NpcChatWnd.bPayScriptStart = true; 
		NpcChatWnd.theNpcCode = aMsg->theNPCCode;
	}
	

	return;
}


void CRockClient::Proc_RpScDialogScript( SRpScDialogScript* aMsg )
{
	//	BYTE			theCommand;			// say, picture, sound, openItemShopWindow
	//	WORD			theCode;			// SAY : NextSay가 있으면 TRUE , PICTURE 등은 실행할 Code 값 
	//	char			theString[216];		// dialog, pic_filename, sound_filename 
	
	// NEW_CODE
	//	BYTE			theCommand;			// say, picture, sound, openItemShopWindow
	//	WORD			theCode;			// SAY : NextSay가 있으면 TRUE , PICTURE 등은 실행할 Code 값 
	//	DWORD			theStringIndex;			// ULONG_MAX : string이 없거나, Say의 경우 theString으로 대체하는 경우 
	//	char			theString[256];			// 가변사이즈

	
	// 대화 특수효과 필요??
	// 페이드 인, 아웃으로 대화 내용 보여주기? 아님 타자기 효과로?
	// 나중에 효과주기 업데이트 필요...
	
	
	switch( aMsg->theCommand )
	{
	case n_CMD_SAY:
		{
			// 일단은 Say 대화 밖에 없으므로...테스트...
			// 받은 스트링을 화면에 맞게 줄바꿈하여 화면에 그려준다 
			// 굴림 11, 최대 가로 18글자, 세로 6줄의 대화 내용...
			if( aMsg->theStringIndex == ULONG_MAX )
			{
				NpcChatWnd.SetSayString( aMsg->theString );	
			}
			else
			{
				//int Strlen = Rstrlen(g_Pc_Manager.theScriptStrTable[aMsg->theStringIndex].theString);
				char cvrt_str[4048] = {0,};
				
				if( g_Pc_Manager.theScriptStrTable[aMsg->theStringIndex].theString != NULL)
				{
					g_Pc_Manager.ConvertNameString( g_Pc_Manager.theScriptStrTable[aMsg->theStringIndex].theString, cvrt_str );
					::ConvertTemKeyRestKey( RWCHAR(cvrt_str) );
					NpcChatWnd.SetSayString( cvrt_str );	
				}
				else 
				{
					NpcChatWnd.SetSayString( _RT("Errors in the Script") );	
				}

			}
			
			if( aMsg->theCode )			// 다음 대화 있으면...다음 버튼 활성화 
			{
				nRui->SendEvent( WID_NpcChat_Ask_NextBtn, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
			}
			else
			{
				nRui->SendEvent( WID_NpcChat_Ask_NextBtn, n_emSetVisible, (EPARAM)false, 0, 0, 0 );				
			}
		}
		break;
	case n_CMD_PICTURE:
		{
			//			NpcChatWnd.theNpcPictureImg->SetClientImage( TID_Flora );	
#ifdef C_SEALED_NPC_CHAT_RENEW
		#ifdef C_NPCCHAT_RENEWAL
			NpcChatWnd.theNpcPictureImg->SetClientImage( aMsg->theCode );					
		#endif
#else 
			NpcChatWnd.theNpcPictureImg->SetClientImage( aMsg->theCode );					
#endif 
		}
		break;
	case n_CMD_PLAY_BG:
		{
		}
		break;
	case n_CMD_STOP_BG:
		{
		}
		break; 
	case n_CMD_OPEN_ITEMSHOP:
		{
			//상점, 인벤 띄우기....위치처리....
			SRect size;
		
			//			NpcChatWnd.FrameWnd->Reposition( 198, 108 );
			StoreWnd.FrameWnd->Reposition( 184, 394 );
			StoreWnd.FrameWnd->GetClientSize( &size );
			size.x += size.w;
			size.y;
			
			InventoryWnd.FrameWnd->Reposition( size.x, size.y );
			
			nRui->SendEvent( WID_StoreWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
			InventoryWnd.Open();
#else
			nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
			nRui->SendEvent( WID_Warehouse_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			
			///StoreWnd.FrameWnd->SetMove( false );
			///InventoryWnd.FrameWnd->SetMove( false );
		}
		break; 
	case n_CMD_CLOSE_ITEMSHOP:
		{
			StoreWnd.FrameWnd->SetMove( true );
			InventoryWnd.FrameWnd->SetMove( true );
			
			nRui->SendEvent( WID_StoreWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장
			nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );//유료인벤확장
#endif
			g_ItemSelectPotionWndProc.Close();
			
		}
		break;
	case n_OPEN_SKILLLESSON:
		{
			SkillLearnWnd.Open();
			SkillLearnWnd.SetClassSkillLearnInfo();
		}
		break;		
	case n_CLOSE_SKILLLESSON:
		{
			nRui->SendEvent( WID_SkillLearn_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		}
		break;	
	case n_CMD_OPEN_FIGHTSYSTEM:
		{
			g_BZ_SelectZoneWnd.Open( aMsg->theCode, aMsg->theStringIndex );		// 혼잡도 표시 			
		}
		break;
	case n_CMD_EXIT: 
		{
			NpcChatWnd.CloseNpcChatWnd( false );
			
			StoreWnd.FrameWnd->SetMove( true );
			InventoryWnd.FrameWnd->SetMove( true );
		}
		break;
	case n_CMD_OPEN_REQ_FRONTIER:
		{
			g_FrontierCreateWnd.Open();	
			break;
			
		}
	case n_CMD_OPEN_POTIONMAKER:
		{
			g_ItemSelectPotionWndProc.Open( n_SkillPotion );
			break;
		}
	case n_CMD_OPEN_GAMBLE:
		{
		//	g_ItemSelectEldaMixWndProc.Open();
			break;
		}		
		break;
	case n_CMD_OPEN_ADDACCESSORY:
		{
			int nXPos = 0;
			int nYPos = 0;
			int center_x	= nRui->Desktop.Width / 2;
			int center_y	= nRui->Desktop.Height / 2;
			
			if( g_pAcryEnchantWnd )
			{
				SRect size;
				
				nXPos = 223;
				nYPos = 349;
				
				g_pAcryEnchantWnd->RePosition( nXPos, nYPos );	
				g_pAcryEnchantWnd->m_pAFrameWnd->GetClientSize( &size );
				size.x += (size.w + 15);

				InventoryWnd.FrameWnd->Reposition( size.x, size.y );		
				
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
				InventoryWnd.Open();
#else
				nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif		
				nRui->SendEvent( WID_Warehouse_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

				g_pAcryEnchantWnd->Open();							
			}

			if( g_pAcryEnchantRWnd )
			{
				nXPos = center_x - ( 204 / 2 );
				nYPos = 349;
				
				g_pAcryEnchantRWnd->RePosition( nXPos, nYPos );
				g_pAcryEnchantRWnd->Close();
			}
		}
		break;
	case n_CMD_MIXWEAPONELDASTONE:
		g_ItemEldaMixAWndProc.Open(0);
		break;

	case n_CMD_MIXARMORELDASTONE:
		g_ItemEldaMixAWndProc.Open(1);
		break;

	case n_CMD_MAKE_ELDASTONE:
		
		g_ItemEldaDustWndProc.m_FrameWnd->Reposition(519 , 282) ;
		InventoryWnd.FrameWnd->Reposition( 153, 232 );		
		
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
		InventoryWnd.Open();
#else
		nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif		
		g_ItemEldaDustWndProc.Open();
		break;
	case n_CMD_RELEASE_RUINED_01: 
		g_ItemSelectPotionWndProc.Open( n_Ruined_Interpid_Item  );
		break; 
	case n_CMD_RELEASE_RUINED_02: 
		g_ItemSelectPotionWndProc.Open( n_Ruined_Wrath_item );
		break; 
	case n_CMD_RELEASE_SEALED_01: 
		g_ItemSelectPotionWndProc.Open( n_Sealed_Interpid_Item );
		break; 
	case n_CMD_RELEASE_SEALED_02: 
		g_ItemSelectPotionWndProc.Open( n_Sealed_Wrath_item);
		break; 
	case n_CMD_MAKE_ESSENCE:
		g_ItemSelectPotionWndProc.Open(n_Make_Essence);
		break; 
	case n_CMD_MAKE_MILLENASTRENGTHSTONE:
			g_ItemSelectPotionWndProc.Open(n_Make_M_Strengthstone);			
		break; 
	case n_CMD_MAKE_RAINSTRENGTHSTONE:
			g_ItemSelectPotionWndProc.Open(n_Make_R_Strengthstone);			
		break; 
	case n_CMD_MIX_STRENGTHSTONE:
		{
			int center_x	= nRui->Desktop.Width / 2;
			int center_y	= nRui->Desktop.Height / 2;
			SRect size;
			
			g_StrengthStoneWndProc->m_pWholeFrame->Reposition(center_x-200 , center_y );
			g_StrengthStoneWndProc->m_pWholeFrame->GetClientSize( &size );
			size.x += (size.w + 15);
			InventoryWnd.FrameWnd->Reposition( size.x, size.y );		
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
			InventoryWnd.Open();
#else
			nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
			g_StrengthStoneWndProc->OpenEnchantWnd();
			g_ItemEnchantWndProc.CloseEnchantWnd();
		}
		break;
	case n_CMD_SET_ELDASTONE:
		{

			int center_x	= nRui->Desktop.Width / 2;
			int center_y	= nRui->Desktop.Height / 2;
			SRect size;
				
			g_ItemEnchantWndProc.m_pWholeFrame->Reposition(center_x-200 , center_y-100 );
			g_ItemEnchantWndProc.m_pWholeFrame->GetClientSize( &size );
			size.x += (size.w + 15);
			InventoryWnd.FrameWnd->Reposition( size.x, size.y );		
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
			InventoryWnd.Open();
#else
			nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif	
			g_ItemEnchantWndProc.OpenEnchantWnd();
			g_StrengthStoneWndProc->CloseEnchantWnd();
		}
		break;
#ifdef DECO_RENEWAL_CORPORATION_MJH
	case n_CMD_OPEN_FRONTIERBANK:
		{
			g_UIMsgWnd.SetMsgType( MSG_CORPORATION_JOIN_1 );
			nRui->SendEvent( WID_UIMsgWndAdd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		}
		break;
#endif // DECO_RENEWAL_CORPORATION_MJH
	default:
		break;
	}
	
	/*
	enum NScriptCommand
	{
	n_CMD_WAIT_KEY		= 10,
	n_CMD_PICTURE		= 11,
	n_CMD_PLAY_BG		= 12,
	n_CMD_STOP_BG		= 13,
	n_CMD_PLAY_EFFECT	= 14,
	n_CMD_STOP_EFFECT	= 15,
	
	  //	n_CMD_SELECT_CALL	= 19,
	  //	n_CMD_SELECT		= 20,		// Select, Say 다음 무조건 PauseScript 
	  n_CMD_SAY			= 21,		// -> 결과값이 들어오면 ContinueScript
	  n_CMD_SET_PCFLAG	= 22,
	  n_CMD_PLUS_PC_HP	= 23,
	  
		n_CMD_GIVE_ITEM		= 30,
		n_CMD_TAKE_ITEM		= 31,
		n_CMD_OPEN_ITEMSHOP	= 32,
		n_CMD_CLOSE_ITEMSHOP	= 33,
		n_CMD_DROP_ITEM		= 34,
		
		  n_CMD_GIVE_SKILL	= 40,
		  n_CMD_TAKE_SKILL	= 41,
		  n_CMD_SKILLLIST		= 42,
		  
			n_CMD_QUEST_START	= 50,	
			n_CMD_QUEST_END		= 51,
			n_CMD_EXIT			= 0xa6
			
			  };
			  */
			  return;
}


void CRockClient::Proc_RpScAskScript( SRpScAskScript* aMsg )
{
	//	BYTE			theStringNum;
	//	char			theString[5][50];	// size 50 String이 최대 5개 : 갯수에 따른 가변사이즈 
	//										// 순서대로 Index는 1부터 순차적으로 시작 
	
	// NEW_CODE
	// DWORD			theStringIndex;			// [TAB]으로 각 항목이 구분되어있다.
	/*	if( aMsg->theStringNum <= 0 || aMsg->theStringNum > 5 )
	{
	RLGS("Nothing StringNum...");
	return;
	}
	
	  
	*/
	char AskBtnString[10][100];
	ZeroMemory(&AskBtnString , sizeof(AskBtnString));
		
	int Num = 0;
	RTCHAR *pfindstr = NULL;
	RTCHAR splitter[] = _RT("\t");
	
	RTCHAR cvrt_str[4048]={0,};

	g_Pc_Manager.ConvertNameString( g_Pc_Manager.theScriptStrTable[aMsg->theStringIndex].theString, cvrt_str );

	int iSelectMaxNum = 5; 
#ifdef C_NPCCHAT_RENEWAL
	iSelectMaxNum = 6;
#else 
	iSelectMaxNum = 10;
#endif 




#ifdef UNICODE_ROCK
	pfindstr = wcstok( cvrt_str, splitter );
#else
	pfindstr = Rstrtok( cvrt_str, splitter );
#endif

	if( pfindstr == NULL )
	{
		return;
	}
	
	//SAFE_STR_CPY( AskBtnString[Num], pfindstr, 100 );
	memcpy( AskBtnString[Num], pfindstr, 100 );
	++Num;
	
	// TAB의 갯수를 읽어 낸다 
	while( pfindstr != NULL && Num < iSelectMaxNum )
	{
#ifdef UNICODE_ROCK
		pfindstr = wcstok( NULL, splitter );
#else
		pfindstr = strtok( NULL, splitter );
#endif
		if( pfindstr )
		{
			//SAFE_STR_CPY( AskBtnString[Num], pfindstr, 100 );				
			memcpy( AskBtnString[Num], pfindstr, 100 );				
			++Num;
		}
	};
	
	int i = 0;
	
	for( i = 0; i < Num; ++i )
	{
		nRui->SendEvent( WID_NpcChat_AnswerBtn1 + i, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
		
		NpcChatWnd.SetAnswerBtnString( WID_NpcChat_AnswerBtn1 + i, AskBtnString[i] );
	}
	for( ; i < iSelectMaxNum; ++i )
	{
		nRui->SendEvent( WID_NpcChat_AnswerBtn1 + i, n_emSetVisible, (EPARAM)false, 0, 0, 0 );				
	}
	
	
	return;
}



void CRockClient::Proc_RpScItemShopInfo( SRpScItemShopInfo* aMsg )
{
	int item_info_size	= aMsg->theMsgHeader.theSize - sizeof( aMsg->theMsgHeader ) - sizeof( aMsg->theVersion );
	int num_item		= item_info_size / 3; //sizeof( _ShopItemInfo );	
	
	SStoreShop* shop = NULL;
	shop = g_Pc_Manager.GetStoreShop( aMsg->theVersion );//aMsg->theVersion -> Owner code
	//shop->Owner = aMsg->theVersion;
	//SStoreShop* shop =g_Pc_Manager.GetStoreShop( g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].code );
	
	if( shop == NULL )
	{
		return;
	}
	 
	int i = 0;
	int j = 0;
	
	for( i = 0; i < num_item; ++i )
	{
		for( j = 0; j < shop->NumItem; ++j )
		{
			if( aMsg->theItemInfo[i].theItemCode == shop->Item[j].Code )
			{
				shop->Item[j].Amount = aMsg->theItemInfo[i].theNum;
				break;
			}
		}
	}
	
	// 상점의 할인률 적용 
	StoreWnd.theDCRate = aMsg->theDCRate;
	
	//상점 입점....
	StoreWnd.SetShopInfo( shop );
	StoreWnd.OwnerUID = 0; 

	//상점 오너의 UID 셋팅
	for( i = 0; i < MAX_CHARACTER; ++i )
	{
		if( m_PcInfo[i].npc_code == aMsg->theVersion )//OwnerCode
		{
			StoreWnd.OwnerUID = m_PcInfo[i].id;
			break;
		}
	}
	
	return;
}


void CRockClient::Proc_RpScCreateCharacter(SRpScCreateCharacter* aMsg )
{
	switch( aMsg->theResultCode )
	{
	case 	n_Wrong_Core:
		
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_INVALID_CORE ) , 2000 , NULL );
		
		return;
		
	case 	n_ALREADY_CHARACTER_CORE:
		
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_RECREATE_CORE ) , 2000 , NULL );
		
		return;
		
	case	n_NOT_EXIST_CORE:
		
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NOT_EXIST_CORE ) , 2000 , NULL );
		
		return;
		
	case	n_FAIL_CREATE_CHARACTER:
		
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_FAIL_CREATE_CH ) , 2000 , NULL );
		
		return;
		
	case	n_ALREADY_CHARACTER_NAME:
		
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_AREADY_USE_NAME ), 2000 , NULL );
		
		return;
	}
	
	CharSelectWnd.CreateOK();
	
}

void CRockClient::Proc_RpScDeleteCharacter(SRpScDeleteCharacter* aMsg)
{
	if( aMsg->theResultCode == n_Fail )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_FAIL_DEL_CH ) , 2000 , NULL );
		
		return;
	}
	else if( aMsg->theResultCode == n_CANNOT_DELETE_CHARACTER_PROMOTION )	
	{
		MessageBoxWnd.Open( G_STRING_CHAR( DIS_SYSPR_CANNOT_DELETE_CHARACTER_PROMOTION ) , 2000 , NULL );
		return;
	}
	
	
	CharSelectWnd.DeleteOK();
}


/*

  typedef struct _SRpScDeleteCancel
  {
  _SRpScDeleteCancel()
  {
		theMsgHeader.theSize = sizeof( _SRpScDeleteCancel );
		theMsgHeader.theProtocolType = 0;
		theMsgHeader.theProtocol = n_RpScDeleteCancel;
		theMsgHeader.thePCID = 0;
		
		  }
		  
			SRpHeader	theMsgHeader;
			int			theResultCode;			/// 캐릭터 삭제취소 성공 여부
			}SRpScDeleteCancel, * SRpScDeleteCancelPtr;
			
*/


void CRockClient::Proc_RpScStatus( SRpScStatus* aMsg )
{
	DWORD theHP = aMsg->theHP;
	
	if( nRui->thePcParam.MaxHP < theHP )
	{
		theHP = nRui->thePcParam.MaxHP;
	}	
	
	//파이트 존에서 는 모든 상태 리셋이다 .. 
	if(g_Pc.GetPlayer()->theMapType == n_FightZone )
	{
		g_Pc.GetPlayer()->SetRealDie(FALSE);
	}
		
	if( theHP && g_Pc.m_MainPC.char_info.theRealHP != theHP )
	{
		///--JAPAN_BUG_MODIFY
		if( !(g_Pc.GetPlayer()->IsRealDie()) )
		{
			int change_hp = theHP - g_Pc.m_MainPC.char_info.theRealHP;
			g_Pc.m_MainPC.char_info.theRealHP = theHP;
			
			nRui->thePcParam.Stat1.theHP += change_hp;	
			
			g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
		}
	}

	if(!aMsg->theMP)
	{
		int a = 0;
	}
	
	nRui->thePcParam.Stat1.theSP = aMsg->theSP;
	nRui->thePcParam.Stat1.theMP = aMsg->theMP;	
	
	return;
}

void CRockClient::Proc_RpScSeeOtherStatus( SRpScSeeOtherStatus* aMsg )
{
	
	DWORD statusIdx = g_Pc.FindUser( aMsg->thePCID );
	if( statusIdx == MAX_USER_INTABLE )
		return;
	
	DWORD theHP = aMsg->theHP;
	
	if( g_Pc.m_PcTable[statusIdx].char_info.theMaxHP < theHP )
	{
		theHP = g_Pc.m_PcTable[statusIdx].char_info.theMaxHP;
	}
	
	if( g_Pc.m_PcTable[statusIdx].char_info.theRealHP != theHP )
	{
		///--JAPAN_BUG_MODIFY
		if( !(g_Pc.m_Pc[statusIdx].IsRealDie()) )
		{
			int change_hp = theHP - g_Pc.m_PcTable[statusIdx].char_info.theRealHP;
			g_Pc.m_PcTable[statusIdx].char_info.theRealHP = theHP;			
			g_Pc.m_PcTable[statusIdx].char_info.theHP += change_hp;			
			g_Pc.m_Pc[statusIdx].m_lPreTime = g_nowTime;			
		}		
	}
	
	int chrAttrIndex = g_Pc.m_Pc[statusIdx].GetAttrIndex();
	if( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman )
	{
		g_Pc.m_PcTable[statusIdx].char_info.theSMP = aMsg->theSP;		
	}
	else if( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman )
	{
		g_Pc.m_PcTable[statusIdx].char_info.theSMP = aMsg->theMP;		
	}
	
	return;
}


void CRockClient::Proc_RpScNPCStat( SRpScNPCStat* aMsg )
{
	WORD statIdx = g_Map.FindNpc( aMsg->theNPCID );
	if( statIdx == MAX_NPC_INTABLE )
		return;
	
	DWORD theHP = aMsg->theHP;
	
	if( g_Map.m_NpcTable[statIdx].theMaxHP < theHP )
	{
		theHP = g_Map.m_NpcTable[statIdx].theMaxHP;
	}
	
	if( g_Map.m_NpcTable[statIdx].theRealHP != theHP )
	{
		///--JAPAN_BUG_MODIFY
		if( !(g_Map.m_Npc[statIdx].IsRealDie()) )
		{
			int change_hp = theHP - g_Map.m_NpcTable[statIdx].theRealHP;
			g_Map.m_NpcTable[statIdx].theRealHP = theHP;
			g_Map.m_Npc[statIdx].m_lPreTime = g_nowTime;
			
			g_Map.m_NpcTable[statIdx].theHP += change_hp;			
		}
	}
	
	g_Map.m_NpcTable[statIdx].theSMP = aMsg->theSP;			// 일단은 몬스터 SMP 표시는 보류 
	
	return;
}


///---------------------------------------------------------------------------
///-- Proc_RpScChangeMainClass
///---------------------------------------------------------------------------
///-- Main Class 변경

void CRockClient::Proc_RpScChangeMainClass( SRpScChangeMainClass* aMsg )
{	
	
	//현재 말을 타고 있으면 강제로 해제 시킨다 .. 
	if( g_Pc.GetPlayer()->m_State.bRide)
	{
		Send_CsRideOff();
	}
	
	nRui->thePcParam.MainClass = aMsg->theMainClass;
	g_Pc.m_MainPC.char_info.theMainClass = nRui->thePcParam.MainClass;
	
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;
	
	
	if(	(	// 밀레나 남-녀, 직업은 원더러, 소드맨, 나이트, 머시너리, 슬레이어  
		( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || 
		chrClassType == Swordman || chrClassType == Knight || chrClassType == Mercenary || chrClassType == Slayer ) 
		//		) ||
		//		(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
		//		( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
		//		( chrClassType == Shaman || chrClassType == Psyche ) 
		)
		)
	{
		nRui->thePcParam.GageType = n_ComboGage;
	}
	else if(	// 밀레나 남-녀, 직업은 원더러, 아처, 소울 브레이커  
		( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || 
		chrClassType == Archer || chrClassType == Sniper ) 
		) 			
	{
		nRui->thePcParam.GageType = n_ConditionGage;
	}
	else if(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
		( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
		( chrClassType == Stranger ||
		chrClassType == Magician || chrClassType == Prominas || chrClassType == Priest || chrClassType == Holy_Avenger || 
		chrClassType == Shaman || chrClassType == Psyche )  		 
		)
	{
		nRui->thePcParam.GageType = n_ProimaGage;
	}	
	
	//#ifdef ROCKCLIENT_DEV		
	//......................................................................................................
	// 전직시 연출 By wxywxy
	//......................................................................................................
	CChangeClass *pTempData = SAFE_NEW( CChangeClass );
				
	m_SquenceManager.Start( pTempData );
	
	//#endif
	
	g_LogWnd.SetEnableCombo(chrClassType);
#ifdef APPLY_GUIDE
	g_pGuideWnd->RefreshGuideDataList();
#endif
	
	return;
}

///---------------------------------------------------------------------------
///-- Proc_RpScChangeSubClass
///---------------------------------------------------------------------------
///-- Sub Class 변경

void CRockClient::Proc_RpScChangeSubClass( SRpScChangeSubClass* aMsg )
{
	nRui->thePcParam.SubClass = aMsg->theSubClass;	
	nRui->thePcParam.SubLevel = aMsg->theSubLevel;	
	return;
}



void CRockClient::Proc_RpScRecoveryStat( SRpScRecoveryStat* aMsg )
{
	//	WORD		theHP;				// 현재 HP, Max HP
	//	WORD		theMP;				// 현재 MP, Max MP
	//	WORD		theSP;				// 현재 SP, Max SP
	
	//	BYTE		thePower;			// 파워

	//	BYTE		theDefense;			// 방어

	//	BYTE		theSympathy;		// 교감
	//	BYTE		theResist;			// 저항
	//	BYTE		theFocus;			// 집중
	//	BYTE		theReaction;		// 반응	
	
	//	SRpdSTAT2	theStat2;
	
	//	g_Pc.m_MainPC.char_info.theMaxHP = aMsg->theHP;	
	
	//	int chr_index = g_Pc.GetPlayer()->GetAttrIndex();
	//	if( chr_index == n_Millena_Man || chr_index == n_Millena_Woman )
	//		g_Pc.m_MainPC.char_info.theMaxSMP = aMsg->theSP;			
	//	else if( chr_index == n_Rain_Man || chr_index == n_Rain_Woman )
	//		g_Pc.m_MainPC.char_info.theMaxSMP = aMsg->theMP;			
	
	//	nRui->thePcParam.MaxHP = aMsg->theHP;	
	
	
	nRui->thePcParam.MaxHP = aMsg->theMaxHP;
	nRui->thePcParam.MaxMP = aMsg->theMaxMP;
	nRui->thePcParam.MaxSP = aMsg->theMaxSP;
	
	nRui->thePcParam.Power = aMsg->thePower;
	nRui->thePcParam.Vital = aMsg->theVital;
	nRui->thePcParam.Sympathy = aMsg->theSympathy;
	nRui->thePcParam.Intel = aMsg->theInt;		
	nRui->thePcParam.Stamina = aMsg->theStamina;		
	nRui->thePcParam.Dex = aMsg->theDex;		
	
	nRui->thePcParam.Stat2 = aMsg->theStat2;
	
	return;
}


void CRockClient::Proc_RpScGetSkill( SRpScGetSkill* aMsg )
{
	if( aMsg->theResult != n_Success )
	{
		switch( aMsg->theResult )
		{
		case n_Fail:
			break;
		case n_NoMatch_Class:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OTHER_CLASS_SKILL ) );
			}
			break;
		case n_ShortLevel:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SHORTAGE_LEVEL ) );
			}
			break;
		case n_No_NeedSkill:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARN_NEED_SKILL ) );
			}
			break;
		case n_ShortSkillCost:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NEED_LEARNING_COST));	
			}
			break;
		
		default:
			break;
		}
		return;
	}
	
	int	LevelCount = 0;

#ifdef SEALED_TEMPLE_200811
	nRui->thePcParam.SkillPoint =  aMsg->theSkillPoint;
#endif 
	LevelCount = g_Pc.GetPlayer()->theActionSystem.GetComboLevel();
	
	//스킬창에 스킬 추가
	SPcItem* pcitem = NULL;
	
	for( int i = 0; i < RP_MAX_USER_SKILL; ++i )
	{
		if( m_PcInven->SkillItem[i] == NULL )
		{
			// SPcItem객체생성
			pcitem = SAFE_NEW( SPcItem );
	
			// SPcItem객체에 SkillItem값 설정
			pcitem->IType		= n_ITYPE_SKILL;
			pcitem->Class		= n_ICLASS_NONE;
			pcitem->ItemUID		= 0;
			pcitem->Code		= aMsg->theSkillCode;
			pcitem->Tid			= (TEXID)g_Pc_Manager.GetSkillIconTID( aMsg->theSkillCode );
			pcitem->TidEx		= (TEXID)g_Pc_Manager.GetSkillAttriIconTID( aMsg->theSkillCode );
			pcitem->Amount		= 1;
			pcitem->SkillTable	= g_Pc_Manager.GetSkillBaseInfoPtr( aMsg->theSkillCode );
			pcitem->Nation		= g_Pc.m_MainPC.char_info.theCitizen;
			
			m_PcInven->SkillItem[i] = pcitem;
			
			nRui->UpdateMaxGage(aMsg->theSkillCode, true);	
			
			break;
		}
	}
	
	//Skill창 업데이트
	g_SkillWnd.UpdateSkillSlotItem();
	g_SkillWnd.m_InsertHighlight = TRUE;
	
	SSkillBaseInfo* skill_Info = NULL;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aMsg->theSkillCode );	
	if( skill_Info )
	{
		SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARNED_SKILL ), skill_Info->theSkillName );
	}
	
	SkillLearnWnd.SetClassSkillLearnInfo();
	
	if( LevelCount != g_Pc.GetPlayer()->theActionSystem.GetComboLevel() ) 
	{
		g_LogWnd.InitComboTextList();		
		g_LogWnd.SetComboHotKey( g_Pc.theSkillSystem.theShortCut_SkillCode[2] );
	}
	
	///-- 아이템 강화스킬레벨 다시 설정
	g_ItemEnchantWndProc.SetSkillLevel();
}

void CRockClient::Proc_RpScTakeSkill( SRpScTakeSkill* aMsg )
{
	//스킬창에서 스킬 삭제.	
	///--JAPAN_BUG_MODIFY	
	if(g_SkillLevelUpWnd.IsVisible)
	{
		nRui->SendEvent( WID_SkillLevelUpWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}

	nRui->ResetCursorItem();
	
	SPcItem * delSkillItem = NULL;
	
	for( int i = 0; i < RP_MAX_USER_SKILL; ++i )
	{
		if( m_PcInven->SkillItem[i] )
		{
			if( m_PcInven->SkillItem[i]->Code == aMsg->theSkillCode )
			{
				delSkillItem = m_PcInven->SkillItem[i];
				delete( m_PcInven->SkillItem[i] );
				m_PcInven->SkillItem[i] = NULL;
				
				nRui->UpdateMaxGage(aMsg->theSkillCode, false);
				
				break;
			}
		}
	}
	
	//퀵슬롯에서도 삭제한다.
	//추후엔 퀵슬롯상에선 갱신루틴으로 하는게 낳을듯..
	if( delSkillItem != NULL )
	{
		for( int i = 0; i < RP_MAX_QUICK_SLOT; ++i )
		{
			if( delSkillItem == m_PcInven->QuickSlot[i] )
			{
				m_PcInven->QuickSlot[i] = NULL;	
			}
		}
	}
	
	//단축키 제거
	/*if( g_SkillWnd.m_pShortCutSlot_Ctrl->PcItem && 
	g_SkillWnd.m_pShortCutSlot_Ctrl->PcItem->Code == aMsg->theSkillCode )
	{
	g_SkillWnd.m_pShortCutSlot_Ctrl->PcItem = NULL;
	g_Pc.GetSkillSystem()->theShortCut_SkillCode[0] = 0;
	g_RockClient.Send_RpCsHotKey( 0, 0 );
	}
	else if( g_SkillWnd.m_pShortCutSlot_Shift->PcItem &&  
	g_SkillWnd.m_pShortCutSlot_Shift->PcItem->Code == aMsg->theSkillCode )
	{
	g_SkillWnd.m_pShortCutSlot_Shift->PcItem = NULL;
	g_Pc.GetSkillSystem()->theShortCut_SkillCode[1] = 0;
	g_RockClient.Send_RpCsHotKey( 1, 0 );
}*/
	
	
	//Skill창 업데이트
	g_SkillWnd.UpdateSkillSlotItem();
	//메인메뉴 퀵슬롯 갱신
	MainMenuWnd.UpdateQuickSlotItem();
	
}


void CRockClient::Proc_RpScQuestWindow( SRpScQuestWindow* aMsg )
{
/*
SRpHeader		theMsgHeader;
WORD			theQuestCode;
BYTE			theStatus;			// 퀘스트 창 열기 : OPEN, 퀘스트 창 닫기 : CLOSE(이 경우 QuestCode 값 무시 )
	//*/
	QuestWnd.SetQuestWindow( aMsg->theQuestCode, aMsg->theStatus );
}

void CRockClient::Proc_RpScQuestStatus( SRpScQuestStatus* aMsg )
{
/*
SRpHeader		theMsgHeader;
WORD			theQuestCode;
WORD			theFlag;
BYTE			theDisplay;			// OPEN : 퀘스트 보여주기, CLOSE : 퀘스트 감추기 
DWORD			theTime;			// 타임제한 퀘스트의 경우 ms단위로 시간을 전송한다. 
	//*/
	
	///-- By simwoosung -- 이미 이곳에서 InsertHighlight의 여부를 판단하고 동작시킨다.
	QuestWnd.SetQuestStatus( aMsg->theQuestCode, aMsg->theFlag, aMsg->theDisplay ,  aMsg->theTime);
	///--QuestWnd.SetInsertHighlight( true );
	
	g_Pc_Manager.ReSet__NpcSetVF( g_Map.m_NowMapID );
	g_Map.ReSetNpcQuestMark();
	
	return;
}

void CRockClient::Proc_ScNPCInfo(RpNPCInfo *aMsg)
{
	thePacketTransLogDlg.InsertListString( "-------------- NPC Info --------------" );
	thePacketTransLogDlg.InsertListString( "ID(%d),GroupRT(%d)" , aMsg->theNPCID , aMsg->theGroupRegenTime );
	thePacketTransLogDlg.InsertListString( "State(%d),NotMove(%d),NotAttack(%d),Provoke(%d)" , 
		aMsg->theState , aMsg->theNotMove , aMsg->theNotAttack , aMsg->theProvoke );
	
	thePacketTransLogDlg.InsertListString( "GameState(%d),Mode(%d),GameNotMove(%d),GameNotAttack(%d),theGameProvoke(%d)" , 
		aMsg->theGameState , aMsg->theMode , aMsg->theGameNotMove , aMsg->theGameNotAttack , aMsg->theGameProvoke );
	
}



void CRockClient::Proc_RpScGMCmd( SRpScGMCmd* aMsg )
{
	theGMCmdDlg.OnScGMCmd( aMsg->theCommand, aMsg->theNParam, aMsg->theSParam );
}

void CRockClient::Proc_RpScGMCmdErr( SRpScGMCmdErr* aMsg )
{
	theGMCmdDlg.OnScGMCmdErr( aMsg->theCommand, aMsg->theErrorCode );
}

void CRockClient::Proc_RpScAnnouncement( SRpScAnnouncement* aMsg )
{
	g_Announcement.InsertAnnouncement(aMsg->theAnnouncement);
	g_ChatMainProc.InsertChatContent(n_AnnounChat, _RT(""), aMsg->theAnnouncement);
	return;
}

void CRockClient::Proc_ScPvPAnnouncement( SRpScPvPAnnouncement* aMsg )
{
	//메세지 는 가변사이즈 
	char strtmp[128] ={0,};

	if( aMsg->theAttackCitizen == RAIN ) 
	{
		Rsprintf(RWCHAR(strtmp ),  G_STRING_CHAR(IDS_NOTICE_RAIN_ATTACK ) , RWCHAR(aMsg->theAttackPCName) , RWCHAR(aMsg->theDefensePCName) );
	}
	else
	{
		Rsprintf(RWCHAR(strtmp ),  G_STRING_CHAR(IDS_NOTICE_MILLENA_ATTACK ) , RWCHAR(aMsg->theAttackPCName ), RWCHAR(aMsg->theDefensePCName) );
	}

	//[밀레나]송 이 [레인]김 을 죽였습니다.
	g_Announcement.InsertAnnouncement(strtmp , false );
	//g_ChatMainProc.InsertChatContent(n_AnnounChat, {0,}, aMsg->theAnnouncement);
	return;
}

void CRockClient::Proc_RpScUpdateGage( SRpScUpdateGage* aMsg )
{
	nRui->thePcParam.Stat1.theGage = aMsg->theGage;
	g_Pc.GetPlayer()->theActionSystem.SetComboGauge();
	
	return;
}

void CRockClient::Proc_RpScQuestCount( SRpScQuestCount* aMsg )
{
	if( aMsg->theType == n_NPC ) 
	{
		SNPCBaseInfo* npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aMsg->theCode ); 
		
		if( npc_Info )
		{
			SystemPrint( n_LightOrange, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_Q_KILL ), npc_Info->theName, aMsg->theCount, aMsg->theTotalCount );
#ifdef QUEST_COUTLIST_RENEWAL
		QuestWnd.SetCompleteItemTextNew( aMsg );			
#else 
		QuestWnd.SetCompleteItemText( aMsg );		
#endif 
	
		}
	}
	// dongs 주석 처리 .. 
	// 이제 아이템 부분만 클라이 언트에서 처리 .. 부하를 막기위해 ㅡㅡ??
	//	else if( aMsg->theType == n_ITEM ) 
	//	{
	//		SItemBaseInfo* item_Info = NULL;
	//		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theCode );	
	//
	//		if( item_Info )
	//		{
	//			SystemPrint( n_LightOrange, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_Q_GET ), item_Info->theName, aMsg->theCount, aMsg->theTotalCount );		
	//
	//			QuestWnd.SetCompleteItemText( aMsg );		
	//		}		
	//	}
	
	return;
}

//....................Party Protocols....//By Simwoosung//.............................//

void CRockClient::Proc_RpScPartyAskJoin( SRpScPartyAskJoin * aMsg)
{	

	g_UIMsgWnd.SetParam( aMsg->theLeaderName );

	if( g_MCommunityWnd.m_MSettingTabWnd.GetIsPartyCall())
	{
		//상대방에 거부 메세지를 보낸다. 
		g_RockClient.Send_RpCsPartyAnsJoin(aMsg->theLeaderName, false);
		return;
	}

	if(n_PARTY == aMsg->thePartyType)	
	{
		g_UIMsgWnd.SetMsgType( MSG_PARTYCALL );
	}
	else
	{
		g_UIMsgWnd.SetMsgType( MSG_LEAGUECALL );
	}	
	
	nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );	
	
	return;	
}

void CRockClient::Proc_RpScPartyAnsJoin( SRpScPartyAnsJoin * aMsg)
{
    TCHAR tstrPAnsMessage[256] = {0,};
	
	if(aMsg->theAnswer)
	{
		switch(aMsg->theAnswer)
		{
		case n_Party_AlreadyPartyMember: 
			Rsprintf(RWCHAR( tstrPAnsMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_OTHERMEMBER ), RWCHAR(aMsg->thePCName ));		
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrPAnsMessage);							          
			break;
		case n_Party_LogoutPC:
			Rsprintf(RWCHAR( tstrPAnsMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_LOGOUT ), RWCHAR(aMsg->thePCName ));				
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrPAnsMessage);	          
			break;
		case n_Party_NotLoginedPC:
			Rsprintf(RWCHAR( tstrPAnsMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_NOT_LOGIN ), RWCHAR(aMsg->thePCName) );				
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrPAnsMessage);	          
			break;
		case n_Party_NotSameCitizen:
			Rsprintf(RWCHAR( tstrPAnsMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_OTHERNATION ));				
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrPAnsMessage);          
			break;
		case n_Party_NotExistFrontierPet:
			Rsprintf(RWCHAR( tstrPAnsMessage), G_STRING_CHAR( IDS_SYSPR_NOTEXIST_FRONTIERPET ));				
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrPAnsMessage);  
			break;
		}
	}
	else
	{
		Rsprintf(RWCHAR( tstrPAnsMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_REFUSED ), RWCHAR(aMsg->thePCName) );		
		SystemPrint(n_DarkGreen ,n_SystemMsg, tstrPAnsMessage);      	
	}
}

void CRockClient::Proc_RpScPartyCreate( SRpScPartyCreate * aMsg )
{
	int a = 0;
}

void CRockClient::Proc_RpScPartyJoin( SRpScPartyJoin * aMsg )
{
	//같은 아이디면 리턴
	if( g_Pc.m_MainPC.lUnique == aMsg->theMemberID )
		return;
	
	TCHAR tstrMessage[256] = {0,};
	SPARTYMEMBER_INFO *pMemberInfo = NULL;
	SPARTYMEMBER_INFO *pMemberInfo2 = NULL;
	
	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
	
	if( Rstrcmp( aMsg->theMemberName, _RT("") ) == 0 )
		return;
	
	//같은 파티내에 소속되어 있으면

	if( pPartyInfo->m_dPartyID == aMsg->thePartyID )
	{
		
		if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
		{			
			//로그오프된 멤버의 갱신 - 로그인처리		
			pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theMemberName);
		}
		else
		{
			pMemberInfo = g_LeagueInfoWnd.FindLeagueMember(aMsg->theMemberName);
			pMemberInfo2 = g_PartyInforWnd.FindPartyMember(aMsg->theMemberName);
		}
		
		if( pMemberInfo )
		{
			pMemberInfo->m_dPCID = aMsg->theMemberID;
			
			if(pMemberInfo2)
			{
				pMemberInfo2->m_dPCID = aMsg->theMemberID;
			}
			
			if( pMemberInfo->m_bIsLogOut == TRUE )
			{
				Rsprintf( RWCHAR(tstrMessage) , G_STRING_CHAR( IDS_SYSPR_PARTY_LOGIN ), RWCHAR(aMsg->theMemberName ) );
				SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
				pMemberInfo->m_bIsLogOut = FALSE;				
			}

			///--JAPAN_BUG_MODIFY
			if( pMemberInfo2 && pMemberInfo2->m_bIsLogOut == TRUE )
			{
				pMemberInfo2->m_bIsLogOut = FALSE;				
			}
			
			long lunique = (long)aMsg->thePartyID;			
			PCTABLE *pPcTable = g_Pc.FindPcTable(lunique);		//현재 가시영역내에 존재한다면..
			if(pPcTable)
			{			
				if( g_PartyInforWnd.MakePCharImage(aMsg->thePartyID) )
				{
					pMemberInfo->m_bIsViewRange = TRUE;
					pMemberInfo->m_bIsSee = TRUE;
				}
			}
			else
			{
				pMemberInfo->m_bIsViewRange = FALSE;
				pMemberInfo->m_bRecon = 1;
			}
			
			goto PJOIN_END;
		}
		
		if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
		{			
			//파티에 소속된 아이디가 들어올때 리턴
			pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theMemberID);
		}
		else
		{
			pMemberInfo =  g_LeagueInfoWnd.FindLeagueMember(aMsg->theMemberID);
		}		
		
		if(pMemberInfo)
		{
			goto PJOIN_END;
		}
		
		if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
		{		
			//새로운 멤버의 추가
			pMemberInfo = g_PartyInforWnd.InsertPartyMember( aMsg->theMemberID );
		}
		else
		{
			pMemberInfo = g_LeagueInfoWnd.InsertLeagueMember( aMsg->theMemberID );
		}				
		
		g_Particle.m_EffectManager.CheckPartiHide( aMsg->theMemberID );
		g_Pc.GetPlayer()->DetectPartyIllusion( aMsg->theMemberID, TRUE );			
		
		if(pMemberInfo)
		{
			pMemberInfo->m_bWasMember = FALSE;
			goto PJOIN_END;
		}		
	}
	
PJOIN_END:	
	
	if(LEAGUE_PARTY == g_PartyInforWnd.GetPartyType())
	{	
		g_LeagueInfoWnd.ReSetPartyMember(true);
	}	
	
	//자신의 파티 멤버이든 다른 파티멤버이든 파티이름을 등록한다.
	long lunique = aMsg->theMemberID;
	PCTABLE *pPcTable = g_Pc.FindPcTable(lunique);			 
	if(pPcTable)
	{
		memcpy(pPcTable->char_info.thePartyName , aMsg->thePartyName , sizeof(aMsg->thePartyName));
		//SAFE_STR_CPY( (pPcTable->char_info.thePartyName), aMsg->thePartyName, 512 );
		pPcTable->char_info.thePartyID = aMsg->thePartyID;
	}
}

void CRockClient::Proc_RpScPartyInfo( SRpScPartyInfo * aMsg )
{
	if(g_PartyInforWnd.IsVisible) //파티 윈도우가 보인다면 - 파티에 가입해있다면;;
	{
		return;
	}	
	
	/// 리그인지 파티인지 여부를 설정
	if(aMsg->thePartyType == n_LEAGUE)
	{
		g_PartyInforWnd.SetPartyType(LEAGUE_PARTY);	
	}
	else
	{
		g_PartyInforWnd.SetPartyType(NORMAL_PARTY);	
	}
	
	g_PartyInforWnd.SetVisible(TRUE);
	SPcDataParam* PcParam = nRui->GetPcParamInfo();
	memcpy(PcParam->PartyName ,  aMsg->thePartyName , sizeof(aMsg->thePartyName) );
    	
	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
	pPartyInfo->m_dPartyID = aMsg->thePartyID;
	memcpy( pPartyInfo->m_strPartyName, aMsg->thePartyName, sizeof(aMsg->thePartyName));

	pPartyInfo->m_dLeaderID = aMsg->theLeaderID;
	pPartyInfo->m_bItemMgr = aMsg->theItemMgr;
	
	g_PartyInforWnd.UpdatePartyWnd();
	
	SPARTYMEMBER_INFO *pMemberInfo = NULL;
	
	if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
	{
		for(int i = 0; i < aMsg->theMemberNum ; i++)
		{
			if( aMsg->theMemberIDList[i] != 0 )
			{
				pMemberInfo = g_PartyInforWnd.InsertPartyMember( aMsg->theMemberIDList[i] );
				g_Particle.m_EffectManager.CheckPartiHide( aMsg->theMemberIDList[i] );
				g_Pc.GetPlayer()->DetectPartyIllusion( aMsg->theMemberIDList[i], TRUE );
				
				if(pMemberInfo)
				{
					pMemberInfo->m_bWasMember = TRUE;
					pMemberInfo->m_bIsViewRange = FALSE;
					pMemberInfo->m_bIsLogOut = FALSE;
					pMemberInfo->m_bIsSee = FALSE;
				}
			}	    
		}
	}
	else
	{
		for(int i = 0; i < aMsg->theMemberNum ; i++)
		{
			if( aMsg->theMemberIDList[i] != 0 )
			{
				pMemberInfo = g_LeagueInfoWnd.InsertLeagueMember(aMsg->theMemberIDList[i]);
				g_Particle.m_EffectManager.CheckPartiHide( aMsg->theMemberIDList[i] );
				g_Pc.GetPlayer()->DetectPartyIllusion( aMsg->theMemberIDList[i], TRUE );				
				if(pMemberInfo)
				{
					pMemberInfo->m_bWasMember = TRUE;
					pMemberInfo->m_bIsViewRange = FALSE;
					pMemberInfo->m_bIsLogOut = FALSE;
					pMemberInfo->m_bIsSee = FALSE;
				}
			}	    
		}
	}
	
	TCHAR tstrMessage[256] = {0,};
	if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
	{
		if( g_Pc.m_MainPC.lUnique != pPartyInfo->m_dLeaderID )
		{
			Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_JOIN ), RWCHAR( tstrMessage) );
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
		}
		else
		{
			Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_CREATED ), RWCHAR( tstrMessage) );
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
		}
	}
	else
	{
		if( g_Pc.m_MainPC.lUnique != pPartyInfo->m_dLeaderID )
		{
			Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_LEAGUE_JOIN ), RWCHAR( tstrMessage) );		
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
		}
		else
		{
			Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_LEAGUE_CREATED ) , RWCHAR( tstrMessage) );		
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
		}
	}
}

void CRockClient::Proc_RpScPartyMemberInfo( SRpScPartyMemberInfo * aMsg )
{
	//같은 아이디면 리턴
	if( g_Pc.m_MainPC.lUnique == aMsg->thePCID )
		return;
	
	if(!g_PartyInforWnd.IsMemberParty()) //파티 멤버가 아니라면

		return;
	
	SPARTYMEMBER_INFO *pMemberInfo = NULL; 
	
    //로그 오프된 파티의 멤버가 들어오는곳

	if( aMsg->thePCID == 1 )
	{
		if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
		{		
			pMemberInfo = g_PartyInforWnd.InsertPartyMember( aMsg->thePCID );
		}
		else
		{
			pMemberInfo = g_LeagueInfoWnd.InsertLeagueMember(aMsg->thePCID);
		}		
		
		g_Particle.m_EffectManager.CheckPartiHide( aMsg->thePCID );
		g_Pc.GetPlayer()->DetectPartyIllusion( aMsg->thePCID, TRUE );
		
		if( pMemberInfo )
		{
			//SAFE_STR_CPY( pMemberInfo->m_strPCName, aMsg->thePCName, RP_MAX_PC_NAME );
			memcpy( pMemberInfo->m_strPCName, aMsg->thePCName, RP_MAX_PC_NAME );
			pMemberInfo->m_dCitizen = aMsg->theCitizen;
			pMemberInfo->m_dGender = aMsg->theGender;
			pMemberInfo->m_dFace = aMsg->theFace;
			pMemberInfo->m_dHair = aMsg->theHair;
			pMemberInfo->m_bMainClass = aMsg->theMainClass;
			pMemberInfo->m_bFrame = aMsg->theFame;
			pMemberInfo->m_bLevel = aMsg->theLevel;
			pMemberInfo->m_bGage = aMsg->theGage; 
			pMemberInfo->m_bMaxGage = aMsg->theMaxGage;
			pMemberInfo->m_wMap = aMsg->theMap;
			pMemberInfo->m_dCell = aMsg->theCell;			
			pMemberInfo->m_bWasMember = TRUE;
			
			pMemberInfo->m_bIsViewRange = FALSE;
			pMemberInfo->m_bIsLogOut = TRUE;
			pMemberInfo->m_bIsSee = FALSE;
		}
		
		if(LEAGUE_PARTY == g_PartyInforWnd.GetPartyType())
		{	
			g_LeagueInfoWnd.ReSetPartyMember(false);
		}
		
		return;
	}
	
	if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
	{		
		pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->thePCID);
	}
	else
	{
		pMemberInfo = g_LeagueInfoWnd.FindLeagueMember(aMsg->thePCID);
	}	
	
	if(pMemberInfo)
	{
		//SAFE_STR_CPY( pMemberInfo->m_strPCName, aMsg->thePCName, RP_MAX_PC_NAME );
		memcpy( pMemberInfo->m_strPCName, aMsg->thePCName, RP_MAX_PC_NAME );
		pMemberInfo->m_dCitizen = aMsg->theCitizen;
		pMemberInfo->m_dGender = aMsg->theGender;
		pMemberInfo->m_dFace = aMsg->theFace;
		pMemberInfo->m_dHair = aMsg->theHair;
		pMemberInfo->m_bMainClass = aMsg->theMainClass;
		pMemberInfo->m_bFrame = aMsg->theFame;
		pMemberInfo->m_bLevel = aMsg->theLevel;
		pMemberInfo->m_bGage = aMsg->theGage;
		pMemberInfo->m_bMaxGage = aMsg->theMaxGage;
		pMemberInfo->m_wMap = aMsg->theMap;
		pMemberInfo->m_dCell = aMsg->theCell;
		
		if(pMemberInfo->m_bWasMember == FALSE)
		{
			pMemberInfo->m_bWasMember = TRUE;
			TCHAR tstrMessage[256] = {0,};
			
			if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
			{		
				Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_JOIN_MEMBER ), RWCHAR(aMsg->thePCName ));
			}
			else
			{
				Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_LEAGUE_JOIN_MEMBER ), RWCHAR(aMsg->thePCName) );
			}						
			
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);			 
		}
		
		long lunique = (long)aMsg->thePCID;
		//현재 가시영역에 존재하는지 여부를 찾음
		PCTABLE *pPcTable = g_Pc.FindPcTable(lunique);
		if( pPcTable )
		{
			pMemberInfo->m_bIsViewRange = TRUE;			
			if( g_PartyInforWnd.MakePCharImage(aMsg->thePCID) )
			{
				pMemberInfo->m_bIsSee = TRUE;
			}
		}
		else
		{
			pMemberInfo->m_bIsViewRange = FALSE; 
		}
		pMemberInfo->m_bIsLogOut = FALSE;
	}
	
	if(LEAGUE_PARTY == g_PartyInforWnd.GetPartyType())
	{	
		g_LeagueInfoWnd.ReSetPartyMember(true);
	}	
}

void CRockClient::Proc_RpScPartyChangeName( SRpScPartyChangeName *aMsg )
{
	
	TCHAR tstrMessage[256] = {0,};	
	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
	
	if(pPartyInfo->m_dPartyID == aMsg->thePartyID)
	{
		SPcDataParam* PcParam = nRui->GetPcParamInfo();
		
		if( Rstrcmp(RWCHAR(pPartyInfo->m_strPartyName), RWCHAR(aMsg->thePartyName) ) != 0 )
		{
			ZeroMemory(pPartyInfo->m_strPartyName , 256 );
			ZeroMemory(PcParam->PartyName , 256 );
			
			memcpy(pPartyInfo->m_strPartyName , aMsg->thePartyName , sizeof(aMsg->thePartyName) );
			memcpy( PcParam->PartyName , aMsg->thePartyName, sizeof(aMsg->thePartyName) );
			
			if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
			{		
				Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_RENAME ), RWCHAR(aMsg->thePartyName) );
			}
			else
			{
				Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_LEAGUE_RENAME ), RWCHAR(aMsg->thePartyName ));	
			}				
			
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
			
			g_PartyInforWnd.UpdatePartyWnd();
		}
	}	
	
	NonPlayer* other_pc[MAX_USER_INTABLE];
	for(int i = 0; i < MAX_USER_INTABLE; ++i )
	{	
		other_pc[i] = &g_Pc.m_Pc[i];	
		
		if( g_Pc.m_PcTable[i].lUnique != -1 || other_pc[i]->m_nTableIndex != -1 )
		{
			if(g_Pc.m_PcTable[i].char_info.thePartyID == aMsg->thePartyID) 
			{
				memset(g_Pc.m_PcTable[i].char_info.thePartyName , NULL , 512 );
				memcpy(g_Pc.m_PcTable[i].char_info.thePartyName , aMsg->thePartyName , 21  );
			}
		}
	}
}

void CRockClient::Proc_RpScPartyChangeLeader( SRpScPartyChangeLeader * aMsg )
{
	TCHAR tstrMessage[256] = {0,};
	   
	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
	if(g_PartyInforWnd.IsMemberParty())
	{
		pPartyInfo->m_dLeaderID = aMsg->theNewLeaderID;
		
		SPARTYMEMBER_INFO *pMemberInfo = NULL; 
		pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theNewLeaderID);
		if(pMemberInfo)
		{
			Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_CH_MASTER ),	
				RWCHAR(pMemberInfo->m_strPCName ));		
			SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
		
		}

		if( LEAGUE_PARTY == g_PartyInforWnd.GetPartyType())
		{
			g_LeagueInfoWnd.SetLeagueMemberField();
		}

		
		g_PartyInforWnd.UpdatePartyWnd();
	}
}

void CRockClient::Proc_RpScPartyLeave( SRpScPartyLeave *aMsg )
{
	TCHAR tstrMessage[256] = {0,};
	SPARTYMEMBER_INFO *pMemberInfo = NULL;
	
	SPcDataParam* PcParam = nRui->GetPcParamInfo();
	
	UINT uPType = g_PartyInforWnd.GetPartyType();
	
	//나간 사람이 자신이라면

	if(Rstrcmp(PcParam->PCName, aMsg->thePCName) == 0)
	{
		
		if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
		{	
			list< SPARTYMEMBER_INFO * >::iterator i = g_PartyInforWnd.GetBeginMember();
			list< SPARTYMEMBER_INFO * >::iterator end = g_PartyInforWnd.GetEndMember();
			
			for( ; i != end; i++ )
			{
				SPARTYMEMBER_INFO *pMemberInfo = (*i);
				
				g_Particle.m_EffectManager.CheckPartiDeleteHide(pMemberInfo->m_dPCID );			
				g_Pc.GetPlayer()->DetectPartyIllusion( pMemberInfo->m_dPCID, FALSE );
			}
		}
		else
		{
			int nLeagueMemSize = g_LeagueInfoWnd.GetLeagueMemSize();
			SPARTYMEMBER_INFO **pMemList = g_LeagueInfoWnd.GetLeagueMemList();
			
			for( int i = 0 ; i < nLeagueMemSize ; i++ )
			{
				if(pMemList[i])
				{
					g_Particle.m_EffectManager.CheckPartiDeleteHide( pMemList[i]->m_dPCID );			
					g_Pc.GetPlayer()->DetectPartyIllusion( pMemList[i]->m_dPCID , FALSE );
				}
			}
			
			g_LeagueInfoWnd.AllDelete();
			g_LeagueInfoWnd.Close();			
		}	
		
		
		g_PartyInforWnd.ClearAllInfo();
		g_PartyInforWnd.SetVisible(FALSE);		
		
		memset(PcParam->PartyName , NULL , MAX_SIZE_256 );
				
		if(NORMAL_PARTY == uPType)
		{		
			if(aMsg->theLeaveFlag == n_Party_Leave)
			{
				Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_OUT ) );	
				SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
			}	  
			
			if(aMsg->theLeaveFlag == n_Party_Leaved)
			{
				Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_KICKED ) );
				SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
			}	 
			
			if(aMsg->theLeaveFlag == n_Party_Disjoint)
			{
				Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_DESTROY ) );
				SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
			}	  
		}
		else
		{
			if(aMsg->theLeaveFlag == n_Party_Leave)
			{
				Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_LEAGUE_OUT ) );			
				SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
			}	  
			
			if(aMsg->theLeaveFlag == n_Party_Leaved)
			{
				Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_KICKED ) );
				SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
			}	 
			
			if(aMsg->theLeaveFlag == n_Party_Disjoint)
			{
				Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_LEAGUE_DESTROY ) );		
				SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
			}	  
		}		
		
		
		if(aMsg->theLeaveFlag == n_Party_ValidLogout)
		{
			//자신이 로그아웃중? - 일단 리턴
			return; 
		}		
		
	}
	else  //다른 사람이 나간상황 이라면

	{
		if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
		{		
			pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->thePCName);
		}
		else
		{
			pMemberInfo = g_LeagueInfoWnd.FindLeagueMember(aMsg->thePCName);
		}
		
		if(pMemberInfo)
		{
			if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
			{		
				if(aMsg->theLeaveFlag == n_Party_Leave)
				{
					Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_OUT_MEMBER ),
						RWCHAR(aMsg->thePCName) );
					SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
				}		
				
				if(aMsg->theLeaveFlag == n_Party_Leaved)
				{
					if(pMemberInfo->m_bIsLogOut)
					{
						Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_OFF_MEMBER ),
							RWCHAR(aMsg->thePCName) ); 					
					}
					else
					{
						Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_KICKED_MEM ), 
							RWCHAR(aMsg->thePCName)  ); 
					}								
					SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
				}	  
			}
			else
			{
				if(aMsg->theLeaveFlag == n_Party_Leave)
				{
					Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_LEAGUE_OUT_MEMBER ),		
						RWCHAR(aMsg->thePCName) );
					SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
				}		
				
				if(aMsg->theLeaveFlag == n_Party_Leaved)
				{
					if(pMemberInfo->m_bIsLogOut)
					{
						Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_OFF_MEMBER ),
							RWCHAR(aMsg->thePCName) ); 					
					}
					else
					{
						Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_KICKED_MEM ), 
							RWCHAR(aMsg->thePCName) ); 
					}								
					SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
				}
			}
			
			if(aMsg->theLeaveFlag == n_Party_ValidLogout)
			{
				if(pMemberInfo->m_bIsLogOut == FALSE)
				{
					Rsprintf(RWCHAR( tstrMessage), G_STRING_CHAR( IDS_SYSPR_PARTY_LOGOUT_MEM ),
						RWCHAR(aMsg->thePCName) );		
					SystemPrint(n_DarkGreen ,n_SystemMsg, tstrMessage);
					
					pMemberInfo->m_bIsSee = FALSE;
					pMemberInfo->m_bIsViewRange = FALSE;
					pMemberInfo->m_bIsLogOut = TRUE;
					g_PartyInforWnd.DeletePCharImage(pMemberInfo->m_dPCID);
				}
				//10분후에도 로그아웃이면 내보내고 그렇지 않으면 내버려둠
				if(LEAGUE_PARTY == g_PartyInforWnd.GetPartyType())
				{	
					g_LeagueInfoWnd.ReSetPartyMember(true);
				}	
				return;
			}
			
			g_Particle.m_EffectManager.CheckPartiDeleteHide(pMemberInfo->m_dPCID);			
			g_Pc.GetPlayer()->DetectPartyIllusion( pMemberInfo->m_dPCID, FALSE );
			
			if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
			{			
				g_PartyInforWnd.DeletePartyMember(pMemberInfo->m_dPCID);
			}
			else
			{
				g_PartyInforWnd.DeletePartyMember(pMemberInfo->m_dPCID);
				g_LeagueInfoWnd.DeleteLeagueMember(pMemberInfo->m_dPCID);
				g_LeagueInfoWnd.ReSetPartyMember(true);
			}
		}
	}
	
	//자신의 파티 멤버이든 다른 파티멤버이든 파티를 해제한다.	
	PCTABLE *pPcTable = g_Pc.FindPcTable(aMsg->thePCName);			 
	if(pPcTable)
	{
		memset(pPcTable->char_info.thePartyName , NULL , MAX_SIZE_512 );
		pPcTable->char_info.thePartyID = 0;
	}
}

void CRockClient::Proc_RpScPartyTarget( SRpScPartyTarget * aMsg )
{
	//이쪽은 차후 작업

	int a = 0;
}

void CRockClient::Proc_RpScPartyUpdateGage( SRpScPartyUpdateGage * aMsg )
{
	if(!g_PartyInforWnd.IsMemberParty())
		return;	
	
	if(g_Pc.m_MainPC.lUnique == aMsg->theMemberID)
	{
		nRui->thePcParam.Stat1.theGage = aMsg->theGage;		     
		g_Pc.GetPlayer()->theActionSystem.SetComboGauge();
		return;
	}
	
	SPARTYMEMBER_INFO *pMemberInfo = NULL;   
	pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theMemberID);	
	if(pMemberInfo)
	{
		pMemberInfo->m_bGage = aMsg->theGage;
		pMemberInfo->m_bMaxGage = aMsg->theMaxGage;
		pMemberInfo = NULL;
	}

	pMemberInfo = g_LeagueInfoWnd.FindLeagueMember(aMsg->theMemberID);	
	if(pMemberInfo)
	{
		pMemberInfo->m_bGage = aMsg->theGage;
		pMemberInfo->m_bMaxGage = aMsg->theMaxGage;
	}
}

void CRockClient::Proc_RpScPartyGiveGage( SRpScPartyGiveGage * aMsg )
{
	if(!g_PartyInforWnd.IsMemberParty())
		return;
	
	//게이지를 전송한 사람이 자신이고 상대방이 오버게이지 일경우

	if( (g_Pc.m_MainPC.lUnique == aMsg->theGiverID) && (aMsg->theResult == n_Party_OverGage))
	{
		SystemPrint( n_LightYellow, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GAUGE_TRNS_CANT_FL ));
		return;
	}
	
	//게이지를 전송한 사람이 자신이고 상대방이 오버게이지 일경우

	if( (g_Pc.m_MainPC.lUnique == aMsg->theGiverID) && (aMsg->theResult == n_Party_OverBounding))
	{
		SystemPrint( n_LightYellow, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GAUGE_TRNS_FAR ));
		return;
	}
	
	//성공하지 않으면 일단 리턴
	if( aMsg->theResult != n_Success )
	{
		SystemPrint( n_LightYellow, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GAUGE_TRNS_CANT ));
		return;
	}
	
	//...........................................................................................................
	// 게이지 전송이펙트 By wxywxy
	//...........................................................................................................
	Character *pSrc  = NULL;
	Character *pDest = NULL;
	
	if( g_Pc.m_MainPC.lUnique == aMsg->theGiverID )
	{
		pSrc = ( Character * ) g_Pc.GetPlayer();
	}
	else
	{
		int skilledIdx = g_Pc.FindUser( aMsg->theGiverID );
		
		if( skilledIdx == MAX_USER_INTABLE )
		{
			pSrc = NULL;
		}
		else
		{
			pSrc = ( Character * ) &g_Pc.m_Pc[skilledIdx];
		}
	}
	
	if( g_Pc.m_MainPC.lUnique == aMsg->theReceiverID )
	{
		pDest = ( Character * ) g_Pc.GetPlayer();
	}
	else
	{
		int skilledIdx = g_Pc.FindUser( aMsg->theReceiverID );
		
		if( skilledIdx == MAX_USER_INTABLE )
		{
			pDest = NULL;
		}
		else
		{
			pDest = ( Character * ) &g_Pc.m_Pc[skilledIdx];
		}
	}
	
	if( pSrc != NULL && pDest != NULL )
	{
		g_Particle.m_EffectManager.Create( EF_GAGE_TRANS , pSrc , pDest );
	}
	
	//...........................................................................................................	
	
	
	int iGage = aMsg->theGage;
	SPARTYMEMBER_INFO *pMemberInfo = NULL;
	
	//게이지를 받은 사람이 자신이라면

	if(g_Pc.m_MainPC.lUnique == aMsg->theReceiverID)
	{
		pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theGiverID);
		if(pMemberInfo)
		{		
			SystemPrint( n_LightYellow, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_GAUGE_PARTY ),
				pMemberInfo->m_strPCName, iGage );			
		}		
	}
	else
	{
		//보낸 사람이 자신이라면

		if(g_Pc.m_MainPC.lUnique == aMsg->theGiverID)	
		{
			pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theReceiverID);
			if(pMemberInfo)
			{
				SystemPrint( n_LightYellow, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GIVE_GAUAGE_PARTY ),
					pMemberInfo->m_strPCName , iGage);			
			}			
			return;
		}
		
		TCHAR tstrGiver[256] = {0,};
		pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theGiverID);
		if(pMemberInfo)
		{
			memcpy(tstrGiver , pMemberInfo->m_strPCName , sizeof(pMemberInfo->m_strPCName));
		}
		
		pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theReceiverID);
		if(pMemberInfo)
		{
			SystemPrint( n_LightYellow, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_VIEW_GAUAGE_PARTY ),
				tstrGiver, pMemberInfo->m_strPCName, iGage );	
		}		
	}
}

void CRockClient::Proc_RpScPartyChat( SRpScPartyChat * aMsg )
{
    if(!g_PartyInforWnd.IsMemberParty())
		return;
	
	char theMessage[RP_MAX_CHAT_SIZE] = "";
	memset(theMessage, 0, sizeof(theMessage));
	memcpy(theMessage, aMsg->theString, aMsg->theStringSize);
	
	g_ChatMainProc.InsertChatContent(n_PartyChat, aMsg->thePCName, theMessage);
}

void CRockClient::Proc_RpScPartyItemMgr( SRpScPartyItemMgr * aMsg )
{
	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
	
	if(!g_PartyInforWnd.IsMemberParty())
		return;
	
	pPartyInfo->m_bItemMgr = aMsg->theItemMgr;
	g_PartyInforWnd.UpdatePartyWnd();
	
	TCHAR tstring[256]	= {0,};
	TCHAR tMessage[256]	= {0,};
	switch(aMsg->theItemMgr)
	{
	case n_PartyItem_Free: SAFE_STR_CPY(tstring, G_STRING_CHAR( IDS_SYSPR_PT_RANDOM_GET ), 256 );
		break;
	case n_PartyItem_Seq:  SAFE_STR_CPY(tstring, G_STRING_CHAR( IDS_SYSPR_PT_RULE_GET ), 256 );
		break;		
	}	
	
	Rsprintf( RWCHAR(tMessage) ,  G_STRING_CHAR( IDS_SYSPR_PT_GET_CHANGE ), RWCHAR(tstring) );
	SystemPrint(n_DarkGreen ,n_SystemMsg, tMessage);
}

void CRockClient::Proc_RpScPartyGetItem( SRpScPartyGetItem * aMsg )
{
    if(!g_PartyInforWnd.IsMemberParty())
		return;
	
	TCHAR tMessage[256]	= {0,};
	
	SItemBaseInfo* item_Info;
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItemCode ); 
	if( item_Info == NULL )
		return;
	
	Rsprintf( RWCHAR(tMessage), G_STRING_CHAR( IDS_SYSPR_PT_GET_MEMBER ), RWCHAR(aMsg->thePCName), RWCHAR(item_Info->theName ));
	
	SystemPrint( n_LightOrange, n_SystemMsg, tMessage );
}

void CRockClient::Proc_RpScPartyGetEld( SRpScPartyGetEld  * aMsg )
{
	if(!g_PartyInforWnd.IsMemberParty())
		return;
	
	TCHAR tMessage[256]	= {0,};
	
	int eld = aMsg->theEld - m_PcParam->Money;
	
	if(eld > 0)
	{		
		CItemTransButton *pItemTransButton = g_main_buttons.GetInVenButton();
		SItemTransInfo ItemTransInfo;
		ItemTransInfo.Is3DPos = true;
		ItemTransInfo.interNpcId = 0;
		ItemTransInfo.isMoney = true;
		ItemTransInfo.isGet = true;		
		memset(&(ItemTransInfo.pcInfo), 0 , sizeof(SPcItem) );
		
		ItemTransInfo.isMonster = false;
		pItemTransButton->InsertItemTransInfo(ItemTransInfo);		
		
		if(NORMAL_PARTY == g_PartyInforWnd.GetPartyType())
		{		
			Rsprintf( RWCHAR(tMessage), G_STRING_CHAR( IDS_SYSPR_PT_GET_ELD ), eld  );
		}
		else
		{
			Rsprintf( RWCHAR(tMessage), G_STRING_CHAR( IDS_SYSPR_LG_GET_ELD ), eld  );			
		}		
		
		SystemPrint( n_LightOrange, n_SystemMsg, tMessage );
	}
	
	m_PcParam->Money = aMsg->theEld;   
}

void CRockClient::Proc_RpScPartyExpMgr( SRpScPartyExpMgr * aMsg )
{
    if(!g_PartyInforWnd.IsMemberParty())
		return;
	
	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
	
	pPartyInfo->m_ClassBonus  = aMsg->theClassBonus;
	pPartyInfo->m_MemberNumBonus = aMsg->theMemberNumBonus;
	pPartyInfo->m_LevelPenalty = aMsg->theLevelPenalty;	
}


void CRockClient::Proc_RpScPartyPosition( SRpScPartyPosition * aMsg )
{
	
	RLGS( "**** PROC_SC -> PartyPosition ****" );
	
	//아이디가 자기자신이면 return;
	if(g_Pc.m_MainPC.lUnique == aMsg->theMemberID)
		return;
	
	SPARTYMEMBER_INFO *pMemberInfo = NULL;
	
	pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->theMemberID);
	if(pMemberInfo)
	{
		pMemberInfo->m_wMap = aMsg->theMap;
		pMemberInfo->m_dCell = aMsg->theCell;
		pMemberInfo = NULL;
	}

	pMemberInfo = g_LeagueInfoWnd.FindLeagueMember(aMsg->theMemberID);	
	if(pMemberInfo)
	{
		pMemberInfo->m_wMap = aMsg->theMap;
		pMemberInfo->m_dCell = aMsg->theCell;
	}
}

//....................................................................
// NPC 선점
//....................................................................
void CRockClient::Proc_RpScNPCPriority( SRpScNPCPriority * aMsg )
{
	int npc_tbl_index = g_Map.FindNpc( (long) aMsg->theNPCID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		DebugPrint( "!NPC 읍~다~~::CRockClient::Proc_RpScNPCPriority\n");
		
		return;
	}
	
	g_Map.m_Npc[npc_tbl_index].SetPriority( aMsg->thePriorityType , aMsg->thePriorityID );
}


//.....................................................................................//
void CRockClient::Proc_RpScDirection( SRpScDirection *aMsg )
{
	int table_index = g_Pc.FindUser( aMsg->thePCID );
	if( table_index == MAX_USER_INTABLE )
		return;
	
	g_Pc.m_Pc[table_index].SetDirection( aMsg->theAngle, false );
	
	return;	
}


//============================================================================================================
// 교환 시스템 관련 프로토콜 
//============================================================================================================
bool CRockClient::GetExchangeResult( int aResult )
{
	if( aResult != n_Success )
	{		
		switch( aResult )
		{
		case n_EXCH_NOTLOGINID:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_CONNECTED_USER ) );
			}
			break;
		case n_EXCH_DENY_ENV:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EX_REFUSED_ALL ) );
			}
			break;
		case n_EXCH_DENY:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EX_REFUSE ) );
			}
			break;
		case n_EXCH_ALREADYEXCH:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EX_AREADY_DOING ) );
			}
			break;
		case n_EXCH_DIEUSER:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EX_DEAD_TARGET ) );
			}
			break;
		case n_EXCH_LONGDISTANCE:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EX_DISTANCE ) );
			}
			break;
		case n_EXCH_CANCEL:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EX_CANCEL ) );
			}
			break;
		case n_EXCH_OVERINVEN:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EX_INVENFULL_USER ) );
			}
			break;	
		case n_EXCH_OVERELD:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_ENOUGH_ELD ) );
			}
			break;	
		case n_EXCH_ERRORITEM:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EX_NOTEXIST_ITEM ) );
			}
			break;	
		default:
			break;
		}
		
		return false;
	}
	
	return true;
}

// B에게 교환 요청에 대한 전달 메시지

void CRockClient::Proc_RpScAskExchange( SRpScAskExchange *aMsg )
{
	//	aMsg->thePCID; 		// 교환을 신청한 ID

	if( g_MCommunityWnd.m_MSettingTabWnd.GetIsExChange())
	{
		g_RockClient.Send_RpCsAnsExchange( n_Fail );
		return;
	}

	int table_index = g_Pc.FindUser( aMsg->thePCID );
	if( table_index == MAX_USER_INTABLE )
		return;
	
	g_AcceptYesNoWnd.SetLearderName( g_Pc.m_PcTable[table_index].char_info.thePCName );
	g_AcceptYesNoWnd.Open( n_ACCEPT_EXCHANGE );
	
	return;
	
}

// A의 요청에 대한 결과( B의 응답 포함 ) 메시지

void CRockClient::Proc_RpScAnsExchange( SRpScAnsExchange *aMsg )
{
	if( GetExchangeResult( aMsg->theResult ) )
	{
		// 거래 승락 
		InventoryWnd.FrameWnd->Reposition( 470, 245 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
		InventoryWnd.Open();
#else
		nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
		g_ExchangeWnd.Open();
		g_ExchangeWnd.m_bIsAccept = TRUE;
		//		nRui->SendEvent( WID_Exchange_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
	}
	
	return;
	
}

// 올리기 한 결과 전송
void CRockClient::Proc_RpScSuggestResult( SRpScSuggestResult *aMsg )
{
	if( GetExchangeResult( aMsg->theResult ) )
	{		
		// 거래 리스트를 올린 후에는 기능 제한 (교환창, 인벤토리창, 공격, 스킬 등)		
	}
	else
	{
		g_ExchangeWnd.ResetMineItemSlot();
	}
	
	return;
	
}

// 상대방이 올린 아이템의 리스트 전송
void CRockClient::Proc_RpScSuggestExItem( SRpScSuggestExItem *aMsg )
{
	//	DWORD		theEld;
	//	BYTE		theItemNum;		// 교환제안하는 아이템 갯수(가변사이즈 계산용)
	//	URpdItem	theItemList[10];	// 가변사이즈

	
	g_ExchangeWnd.SetYoursItemSlot( aMsg->theEld, aMsg->theItemNum, aMsg->theItemList );
	
	g_ExchangeWnd.m_bIsYoursUpload = TRUE;
	
	SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EX_CHECK_O_ITEMS ) );
	
	return;
}

// 교환승락 버튼 클릭에 대한 응답 또는 교환 취소에 대한 전달 메시지

void CRockClient::Proc_RpScResultExchange( SRpScResultExchange *aMsg )
{	
	//struct _SExItem
	//{
	//	DWORD	theItemUID;
	//	BYTE	theBundle;
	//};
	
	//	BYTE		theResult;		// 성공, 실패(슬롯부족, Eld부족, 거래취소 등)
	
	if( GetExchangeResult( aMsg->theResult ) )
	{
		// (성공외의 경우 아래정보는 전송되지 않는다.)
		//	DWORD		theEld;					// 교환 후 최종 Eld
		
		//	BYTE		theDelItemNum;			// 아이템 갯수(가변사이즈 계산용)
		//	BYTE		theAddItemNum;
		
		//	_SExItem	theDelItemList[10];		// theBundle 개수가 0이면 삭제 
		//	URpdItem	theAddItemList[10];
		
		m_PcParam->Money = aMsg->theEld;
		
		
		BYTE* pBuf = (BYTE*) &(aMsg->theDelItemList);
		
		_SExItem DelItemList[10];
		memcpy( DelItemList, &pBuf[0], sizeof( _SExItem ) * aMsg->theDelItemNum );
		
		int idx_buf = sizeof( _SExItem ) * aMsg->theDelItemNum;
		
		URpdItem AddItemList[10];
		memcpy( AddItemList, &pBuf[idx_buf], sizeof( URpdItem ) * aMsg->theAddItemNum );
		
		
		int i = 0;
		int j = 0;
		
		for( i = 0; i < aMsg->theDelItemNum; ++i )
		{
			for( j = 0; j < c_MaxInvenItemNum; ++j )
			{	
				if( m_PcInven->InvenItem[j] == NULL )
				{
					continue;
				}
				
				if( DelItemList[i].theItemUID == m_PcInven->InvenItem[j]->ItemUID )
				{
					//판매후 남은 번들 수. 0 인 경우 아이템 삭제.
					m_PcInven->InvenItem[j]->Amount = DelItemList[i].theBundle;
					
					if( m_PcInven->InvenItem[j]->Amount == 0 )
					{
						//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
						for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
						{
							if( m_PcInven->InvenItem[j] == m_PcInven->QuickSlot[idx] )
							{
								m_PcInven->QuickSlot[idx] = NULL;
								break;
							}
						}
						
						SAFE_DELETE( m_PcInven->InvenItem[j] );						
					}					
					//슬롯에 적용....
					MainMenuWnd.UpdateQuickSlotItem();
					InventoryWnd.UpdateInvenSlotItem();	
				}
			}			
		}
		
		//	BYTE		theAddItemNum;
		//	URpdItem	theAddItemList[10];
		
		for( i = 0; i < aMsg->theAddItemNum; ++i )
		{
			SPcItem* addPcItem = NULL;
			addPcItem = SAFE_NEW( SPcItem );				
			
			WORD ItemType = g_Pc_Manager.GetItemClsss( AddItemList[i].theItem.theCode );
			if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
			{
				addPcItem->IType		= n_ITYPE_WEAR;
				addPcItem->Amount		= 1;
				
				memcpy( &(addPcItem->WearTable), &AddItemList[i], sizeof( URpdItem ) );
			}
			else if( ItemType == n_Class_Use )
			{
				addPcItem->IType	= n_ITYPE_USE;//소모품
				addPcItem->Amount	= AddItemList[i].theWasteItem.theBundle;
			}
			else if( ItemType == n_Class_Useless )
			{
				addPcItem->IType	= n_ITYPE_UNUSE;//비소모품
				addPcItem->Amount	= AddItemList[i].theWasteItem.theBundle;
			}
			else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
				|| ItemType == n_Class_FrontierPet ||  
				ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
			{
				addPcItem->IType	= n_ITYPE_SPECIAL; 
				addPcItem->Amount	= AddItemList[i].theWasteItem.theBundle;
			}		
			else if( ItemType == n_Class_PersonalPet )
			{
				addPcItem->IType	= n_ITYPE_PET_USE; 
				addPcItem->Amount	= AddItemList[i].theWasteItem.theBundle;
			}
			else if( ItemType == n_Class_Riding )
			{
				addPcItem->IType	= n_ITYPE_RIDE; 
				addPcItem->Amount	= 1;
				
				memcpy( &(addPcItem->WearTable), &AddItemList[i], sizeof( URpdItem ) );
			}
			else if( ItemType == n_Class_RidingItem )
			{
				addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
				addPcItem->Amount	= 1;
				
				memcpy( &(addPcItem->WearTable), &AddItemList[i], sizeof( URpdItem ) );
			}
			else if( ItemType == n_Class_Skin )
			{
				addPcItem->IType = n_ITYPE_SKIN_WEAR;
				addPcItem->Amount	= 1;
				memcpy( &(addPcItem->WearTable), &AddItemList[i] , sizeof( URpdItem ) );
			}
			
			addPcItem->Class		= ItemType;
			addPcItem->ItemUID		= AddItemList[i].theItem.theItemUID;
			addPcItem->Code			= AddItemList[i].theItem.theCode;
			addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( AddItemList[i].theItem.theCode );
			addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( AddItemList[i].theItem.theCode );
			
			if( addPcItem->ItemTable == NULL ) 
			{
				SAFE_DELETE( addPcItem );
				return;
			}

			////-- 현재 아이템에 관련된 딜레이가 적용되었는지 체크해서 넣어준다.					
			if( addPcItem->IType == n_ITYPE_USE && addPcItem->ItemTable )
			{
				 int iDelayGroup = addPcItem->ItemTable->theDelayGroup;
				 if( !(nRui->enableDelayTime[iDelayGroup] ) )						
				 {
					addPcItem->IsDelay = true;						
				 }
			}	
			
			// 왼손,오른손....
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
			
			m_PcInven->InvenItem[AddItemList[i].theItem.thePosition] = addPcItem;		
			
			for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
			{
				if( ( m_PcInven->QuickSlot[idx] != NULL ) && ( m_PcInven->QuickSlot[idx]->ItemUID == addPcItem->ItemUID ) )
				{
					m_PcInven->QuickSlot[idx] = addPcItem;				
					break;
				}
			}
			
			//슬롯 재설정....
			MainMenuWnd.UpdateQuickSlotItem();		
			InventoryWnd.UpdateInvenSlotItem();
		}	
		
		//dongs//////////////////////////////////////////////////////////////////////////
		// 퀘스트 정보  조건을 가지고 인벤을 다시한번 검사 한다 .. 
		// 	m_PcInven->QuestCount[num]->theCount 값을 템프로 저장 시키고 값이 바뀌면 바뀐 정보를 출력해 주고 
		//정보를 갱신한다 .. 
		//필요변수 SQuestCount* TempQuestCount [RP_MAX_QUEST_COUNT]
		//////////////////////////////////////////////////////////////////////////
		int TemptheCount [RP_MAX_QUEST_COUNT];
		memset(&TemptheCount,NULL,sizeof(int)*RP_MAX_QUEST_COUNT);
		
		int fcnt = 0;
		int k =0 ;
		int num =0 ;
		WORD 	ItemType = 0;
		
		//인벤 과 퀘스트 정보 갱신 ..
		for( fcnt=0 ; fcnt< RP_MAX_QUEST_FLAG; ++fcnt)
		{
			if(m_PcInven->QuestFlag[fcnt] == NULL) 	
			{
				break;
			}
			if(m_PcInven->QuestFlag[fcnt]->theDisplay == n_OPEN)
			{
				for( num = 0; num < RP_MAX_QUEST_COUNT; ++num )
				{
					if( m_PcInven->QuestCount[num] == NULL )
					{
						break;
					}
					ItemType = g_Pc_Manager.GetItemClsss( m_PcInven->QuestCount[num]->theCode);
					if(ItemType ==  n_Class_Quest)
					{	
						continue;
					}
					//플래그의 퀘스트 코드와 , 퀘스트목록에 코드와 비교 
					if(m_PcInven->QuestFlag[fcnt]->theQuestCode ==  m_PcInven->QuestCount[num]->theQuestCode)
					{
						
						
						
						if(m_PcInven->QuestCount[num]->theType == n_ITEM)	
						{
							TemptheCount[num] = 0;
							//인벤 검사 있냐 없냐 ? 
							for( k =0 ; k < c_MaxInvenItemNum; ++k)
							{
								if(m_PcInven->InvenItem[k] == NULL ) continue;
								
								if(m_PcInven->InvenItem[k]->Code == m_PcInven->QuestCount[num]->theCode )
								{
									
									TemptheCount[num] +=  m_PcInven->InvenItem[k]->Amount;       
								}
							}
						}
					}
				}
			}
		}
		
		// 템프퀘스트 내용와 저장된 내용과 비교하여 .. 값이 다르면 시스템챗에 정보 출력 ㅋㅋ 
		for(fcnt=0 ; fcnt< RP_MAX_QUEST_FLAG; ++fcnt)
		{
			if(m_PcInven->QuestFlag[fcnt] == NULL) 	
			{
				break;
			}
			if(m_PcInven->QuestFlag[fcnt]->theDisplay == n_OPEN)
			{
				for( num = 0; num < RP_MAX_QUEST_COUNT; ++num )
				{
					if( m_PcInven->QuestCount[num] == NULL )
					{
						break;
					}
					ItemType = g_Pc_Manager.GetItemClsss( m_PcInven->QuestCount[num]->theCode);
					if(ItemType ==  n_Class_Quest)
					{	
						continue;
					}
					//플래그의 퀘스트 코드와 , 퀘스트목록에 코드와 비교 
					if(m_PcInven->QuestFlag[fcnt]->theQuestCode ==  m_PcInven->QuestCount[num]->theQuestCode)
					{
						if(m_PcInven->QuestCount[num]->theType == n_ITEM)	
						{
							if(m_PcInven->QuestCount[num]->theCount  != TemptheCount[num])
							{
								m_PcInven->QuestCount[num]->theCount  = TemptheCount[num];
								
								SItemBaseInfo* item_Info = NULL;
								item_Info = g_Pc_Manager.GetItemBaseInfoPtr(m_PcInven->QuestCount[num]->theCode);	
								
								SystemPrint( n_LightOrange, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_Q_GET ), item_Info->theName, m_PcInven->QuestCount[num]->theCount, m_PcInven->QuestCount[num]->theTotalCount);		
								
							}
						}
					}
				}
			}
		}
		
		
		QuestWnd.SetRenewalQuestInfo();
	}
	
	
	g_ExchangeWnd.CloseExchangeWnd();
	
	return;
}


//============================================================================================================
//============================================================================================================
// 보관 시스템 관련 프로토콜 
//============================================================================================================
//============================================================================================================
// 보관리스트 열기

void CRockClient::Proc_RpScOpenBank( SRpScOpenBank *aMsg )
{
	//	BYTE			theLoadFlag;					// n_Success (첫오픈, 아래 아이템 정보 포함), n_Fail(재오픈, 아래 정보 제외)
	//	DWORD			theEld;							// 보관된 Eld
	//	BYTE			theItemNum;						// 보관된 아이템 갯수 (가변길이 계산시 참조)
	//	URpdItem		theItemList[RP_MAX_BANKITEM];	// 보관된 아이템리스트, 아이템의 UID는 슬롯번호와 같다. (기존 아이템들의 UID와 구분 필요)
	
	if( aMsg->theLoadFlag == n_Success )
	{
		BYTE* pBuf = (BYTE*) &(aMsg->theItemList);
		
		URpdItem BankItemList[RP_MAX_BANKITEM];		
		
		if( aMsg->theItemNum >= 1 )
		{
			memcpy( BankItemList, &pBuf[0], sizeof( URpdItem ) * aMsg->theItemNum );
		}

		g_WarehouseWnd.SetWarehousePcItem( aMsg->theEld, aMsg->theItemNum, BankItemList );		
	}
	
	g_WarehouseWnd.Open();
	
	return;
}

void CRockClient::Proc_RpScDeposit( SRpScDeposit *aMsg )
{
	//	DWORD			theItemID;
	//	BYTE			theBundle;			// 맡기고 남은 Bundle, 소모품 & 0 인 경우 삭제 
	//	URpdItem		theBankItem;		// 보관함에 들어간 아이템의 정보 
	//	BYTE			theResult;			// n_NOT_MY_ITEM, 등 
	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		int i = 0;
		
		// 인벤토리 처리 
		for( i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}
			
			if( aMsg->theItemID == m_PcInven->InvenItem[i]->ItemUID )
			{
				//판매후 남은 번들 수. 0 인 경우 아이템 삭제.
				m_PcInven->InvenItem[i]->Amount = aMsg->theBundle;
				
				if( m_PcInven->InvenItem[i]->Amount == 0 )
				{
					//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
					for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
					{
						if( m_PcInven->InvenItem[i] == m_PcInven->QuickSlot[idx] )
						{
							m_PcInven->QuickSlot[idx] = NULL;
							break;
						}
					}
					
					delete( m_PcInven->InvenItem[i] );
					m_PcInven->InvenItem[i] = NULL;
				}					
				
				//슬롯에 적용....
				MainMenuWnd.UpdateQuickSlotItem();
				InventoryWnd.UpdateInvenSlotItem();	
				
				break;
			}
		}	
		
		
		// 보관창 처리 
		SPcItem* addPcItem = NULL;
		
		addPcItem = &g_WarehouseWnd.m_WarehousePcItem[aMsg->theBankItem.theItem.thePosition];
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->theBankItem.theItem.theCode );
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
		{
			addPcItem->IType		= n_ITYPE_WEAR;
			addPcItem->Amount		= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theBankItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			addPcItem->IType	= n_ITYPE_USE;//소모품
			addPcItem->Amount	= aMsg->theBankItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			addPcItem->IType	= n_ITYPE_UNUSE;//비소모품
			addPcItem->Amount	= aMsg->theBankItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
			{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= aMsg->theBankItem.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_PersonalPet )
		{
			addPcItem->IType	= n_ITYPE_PET_USE; 
			addPcItem->Amount	= aMsg->theBankItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			addPcItem->IType	= n_ITYPE_RIDE; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theBankItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{
			addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theBankItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{
			addPcItem->IType = n_ITYPE_SKIN_WEAR;
			addPcItem->Amount	= 1;
			memcpy( &(addPcItem->WearTable), &(aMsg->theBankItem) , sizeof( URpdItem ) );
		}
		
		
		addPcItem->Class		= ItemType;
		addPcItem->ItemUID		= aMsg->theBankItem.theItem.theItemUID;
		addPcItem->Code			= aMsg->theBankItem.theItem.theCode;
		addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theBankItem.theItem.theCode );
		addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theBankItem.theItem.theCode );
		
		if( addPcItem->ItemTable == NULL)
		{
			return;
		}
		// 왼손,오른손....
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
		
		g_WarehouseWnd.UpdateWarehousePcItem();
	}
	
	return;
}

void CRockClient::Proc_RpScDraw( SRpScDraw *aMsg )
{
	//	BYTE			theResult;
	//	URpdItem		theItem;
	//	DWORD			theBankItemID;		// 보관함에서 처리된 아이템 ID
	//	BYTE			theBankBundle;		// 보관함에 남은 번들 수

	

#ifdef C_KEEPING_CHARGE
	if(aMsg->theResult == n_Short_DrawMoney )
	{
		SystemPrint( n_DarkRed , n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_SHORT_DRAWMOENY) , aMsg->theDrawCost ); 
		return;
	}
#endif	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		
#ifdef C_KEEPING_CHARGE
		char eldstr[256] = {0,};
		DWORD Eld = g_WarehouseWnd.m_Eld - aMsg->theDrawCost;
		g_WarehouseWnd.m_Eld = Eld;
		InventoryWnd.SetEldText( eldstr, g_WarehouseWnd.m_Eld );
		g_WarehouseWnd.m_EldText->SetText( eldstr );
		SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FEE_EXPENSES) , aMsg->theDrawCost );
#endif		
		
		int i = 0;
		
		for( i = 0; i < c_MaxWarehousePcItemNum; ++i )
		{
			if( g_WarehouseWnd.m_WarehousePcItem[i].IType == n_ITYPE_NONE )
			{
				continue;
			}
			
			if( aMsg->theBankItemID == g_WarehouseWnd.m_WarehousePcItem[i].ItemUID )
			{
				//판매후 남은 번들 수. 0 인 경우 아이템 삭제.
				g_WarehouseWnd.m_WarehousePcItem[i].Amount = aMsg->theBankBundle;
				
				if( g_WarehouseWnd.m_WarehousePcItem[i].Amount == 0 )
				{
					g_WarehouseWnd.m_WarehousePcItem[i].Clear();
				}		
				
				break;
			}
		}
		
		
		SPcItem* addPcItem = NULL;
		addPcItem = SAFE_NEW( SPcItem );
		
		BOOL IsInsertItem = FALSE;
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->theItem.theItem.theCode );
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
		{
			addPcItem->IType		= n_ITYPE_WEAR;
			addPcItem->Amount		= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			addPcItem->IType	= n_ITYPE_USE;//소모품
			addPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			addPcItem->IType	= n_ITYPE_UNUSE;//비소모품
			addPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
		{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone ||
			ItemType == n_Class_FrontierPet )
		{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_PersonalPet )
		{
			addPcItem->IType	= n_ITYPE_PET_USE; 
			addPcItem->Amount	=aMsg->theItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			addPcItem->IType	= n_ITYPE_RIDE; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{
			addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{
			addPcItem->IType = n_ITYPE_SKIN_WEAR;
			addPcItem->Amount	= 1;
			memcpy( &(addPcItem->WearTable), &(aMsg->theItem) , sizeof( URpdItem ) );
		}
		
		addPcItem->Class		= ItemType;
		addPcItem->ItemUID		= aMsg->theItem.theItem.theItemUID;
		addPcItem->Code			= aMsg->theItem.theItem.theCode;
		addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theItem.theItem.theCode );
		addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItem.theItem.theCode );
		
		if( addPcItem->ItemTable == NULL )
		{
			SAFE_DELETE( addPcItem );	
			return;
		}

		////-- 현재 아이템에 관련된 딜레이가 적용되었는지 체크해서 넣어준다.					
		if( addPcItem->IType == n_ITYPE_USE && addPcItem->ItemTable )
		{
			 int iDelayGroup = addPcItem->ItemTable->theDelayGroup;
			 if( !(nRui->enableDelayTime[iDelayGroup] ) )						
			 {
				addPcItem->IsDelay = true;						
			 }
		}	
		
		// 왼손,오른손....
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
		
		SAFE_DELETE( m_PcInven->InvenItem[aMsg->theItem.theItem.thePosition] );		
		m_PcInven->InvenItem[aMsg->theItem.theItem.thePosition] = addPcItem;
		
		IsInsertItem = TRUE;
		
		for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
		{
			if( ( m_PcInven->QuickSlot[idx] != NULL ) && ( m_PcInven->QuickSlot[idx]->ItemUID == addPcItem->ItemUID ) )
			{
				m_PcInven->QuickSlot[idx] = addPcItem;
				IsInsertItem = TRUE;
				break;
			}
		}
		
		if(FALSE == IsInsertItem)
		{
			SAFE_DELETE( addPcItem );
		}
		
		//슬롯 재설정....
		MainMenuWnd.UpdateQuickSlotItem();		
		InventoryWnd.UpdateInvenSlotItem();
	}
	
	return;
}

void CRockClient::Proc_RpScDepositEld( SRpScDepositEld *aMsg )
{
	//	DWORD			theEld;					// 맡기고 남은 금액
	//	DWORD			theBankEld;				// 은행에 맡겨진 총 금액
	//	BYTE			theResult;
	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		char eldstr[256] = {0,};
		
		g_WarehouseWnd.m_Eld = aMsg->theBankEld;
		InventoryWnd.SetEldText( eldstr, g_WarehouseWnd.m_Eld );
		g_WarehouseWnd.m_EldText->SetText( eldstr );
		
		nRui->GetPcParamInfo()->Money = aMsg->theEld;
	}
	
	return;
}

void CRockClient::Proc_RpScDrawEld( SRpScDrawEld *aMsg )
{
	//	DWORD			theEld;					// 찾은 후 소지하고 있는 총 Eld
	//	DWORD			theBankEld;				// 찾은 후 은행에 보관된 총 Eld
	//	BYTE			theResult;
	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		char eldstr[256] = {0,};
		
		g_WarehouseWnd.m_Eld = aMsg->theBankEld;
		InventoryWnd.SetEldText( eldstr, g_WarehouseWnd.m_Eld );
		g_WarehouseWnd.m_EldText->SetText( eldstr );
		
		nRui->GetPcParamInfo()->Money = aMsg->theEld;
	}
	
	return;
}

void CRockClient::Proc_RpScWHChangeSlot( SRpScWHChangeSlot *aMsg )
{
	//	DWORD			theBankItemID1;		// 옮긴 보관 src 아이템 ID( 0일 경우 빈슬롯이 된다.)
	//	BYTE			theSlot1;			// theBankItemID1 src 가 위치한 슬롯
	//	DWORD			theBankItemID2;		// 옮긴 보관 dest 아이템 ID( 0일 경우 빈슬롯이 된다.)
	//	BYTE			theSlot2;			// theBankItemID2 dest 가 위치한 슬롯
	//	BYTE			theResult;
	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		SPcItem srcPcItem; 
		SPcItem destPcItem; 
		
		int i = 0;
		int find_cnt = 0;
		
		
		if( aMsg->theBankItemID1 == 0 )
		{
			++find_cnt;
		}
		if( aMsg->theBankItemID2 == 0 )
		{
			++find_cnt;
		}
		
		for( i = 0; i < c_MaxWarehousePcItemNum; ++i )
		{
			if( g_WarehouseWnd.m_WarehousePcItem[i].IType == n_ITYPE_NONE )
			{
				continue;
			}
			
			if( aMsg->theBankItemID1 == g_WarehouseWnd.m_WarehousePcItem[i].ItemUID )
			{
				srcPcItem = g_WarehouseWnd.m_WarehousePcItem[i];
				++find_cnt;
			}
			else if( aMsg->theBankItemID2 == g_WarehouseWnd.m_WarehousePcItem[i].ItemUID )
			{
				destPcItem = g_WarehouseWnd.m_WarehousePcItem[i];
				++find_cnt;
			}
			
			if( find_cnt == 2 )
			{				
				break;
			}
		}
		
		g_WarehouseWnd.m_WarehousePcItem[aMsg->theSlot1] = srcPcItem;
		g_WarehouseWnd.m_WarehousePcItem[aMsg->theSlot2] = destPcItem;
	}

	nRui->m_KeyLock.Warehouse.SetUnLock();
	return;
}

#ifdef DECO_RENEWAL_GUILD_MJH

//============================================================================================================
//============================================================================================================
// 길드창고 시스템 관련 프로토콜 
//============================================================================================================
//============================================================================================================
// 보관리스트 열기 

void CRockClient::Proc_RpScOpenFrontierBank( SRpScFrontierBankOpen *aMsg )
{
	//	BYTE			theLoadFlag;					// n_Success (첫오픈, 아래 아이템 정보 포함), n_Fail(재오픈, 아래 정보 제외)
	//	DWORD			theEld;							// 보관된 Eld
	//	BYTE			theItemNum;						// 보관된 아이템 갯수 (가변길이 계산시 참조)
	//	URpdItem		theItemList[RP_MAX_BANKITEM];	// 보관된 아이템리스트, 아이템의 UID는 슬롯번호와 같다. (기존 아이템들의 UID와 구분 필요)
	
	g_GuildInvenWnd.ResetFrontierBank();
	BYTE* pBuf = (BYTE*) &(aMsg->theItemList);
	
	URpdItem GuildBankItemList[50];		
	 
	if( aMsg->theItemNum >= 1 )
	{
		memcpy( GuildBankItemList, &pBuf[0], sizeof( URpdItem ) * aMsg->theItemNum );
	}

	g_GuildInvenWnd.SetGuildInvenPcItem( aMsg->theItemNum, GuildBankItemList );		
	g_GuildInvenWnd.Open();
	
	return;
}

void CRockClient::Proc_RpScDepositFrontier( SRpScDepositFrontierBank *aMsg )
{
	//	DWORD			theItemID;
	//	BYTE			theBundle;			// 맡기고 남은 Bundle, 소모품 & 0 인 경우 삭제 
	//	URpdItem		theBankItem;		// 보관함에 들어간 아이템의 정보 
	//	BYTE			theResult;			// n_NOT_MY_ITEM, 등 
	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		int i = 0;
		
		// 인벤토리 처리 
		for( i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}
			
			if( aMsg->theItemID == m_PcInven->InvenItem[i]->ItemUID )
			{
				
				//판매후 남은 번들 수. 0 인 경우 아이템 삭제.
				m_PcInven->InvenItem[i]->Amount -= aMsg->theBundle;
				
				if( m_PcInven->InvenItem[i]->Amount == 0 )
				{
					//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
					for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
					{
						if( m_PcInven->InvenItem[i] == m_PcInven->QuickSlot[idx] )
						{
							m_PcInven->QuickSlot[idx] = NULL;
							break;
						}
					}
					
					delete( m_PcInven->InvenItem[i] );
					m_PcInven->InvenItem[i] = NULL;
				}					
				
				//슬롯에 적용....
				MainMenuWnd.UpdateQuickSlotItem();
				InventoryWnd.UpdateInvenSlotItem();	
				
				break;
			}
		}	
		
	
		// 보관창 처리 
		SPcItem* addPcItem = NULL;
		
		addPcItem = &g_GuildInvenWnd.m_GuildInvenPcItem[aMsg->theDepositItem.theItem.thePosition];
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->theDepositItem.theItem.theCode );
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
		{
			addPcItem->IType		= n_ITYPE_WEAR;
			addPcItem->Amount		= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theDepositItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			addPcItem->IType	= n_ITYPE_USE;//소모품
			addPcItem->Amount	= aMsg->theDepositItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			addPcItem->IType	= n_ITYPE_UNUSE;//비소모품
			addPcItem->Amount	= aMsg->theDepositItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
			{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= aMsg->theDepositItem.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_PersonalPet )
		{
			addPcItem->IType	= n_ITYPE_PET_USE; 
			addPcItem->Amount	= aMsg->theDepositItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			addPcItem->IType	= n_ITYPE_RIDE; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theDepositItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{
			addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theDepositItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{
			addPcItem->IType = n_ITYPE_SKIN_WEAR;
			addPcItem->Amount	= 1;
			memcpy( &(addPcItem->WearTable), &(aMsg->theDepositItem) , sizeof( URpdItem ) );
		}
		
		
		addPcItem->Class		= ItemType;
		addPcItem->ItemUID		= aMsg->theDepositItem.theItem.theItemUID;
		addPcItem->Code			= aMsg->theDepositItem.theItem.theCode;
		addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theDepositItem.theItem.theCode );
		addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theDepositItem.theItem.theCode );
		
		if( addPcItem->ItemTable == NULL)
		{
			return;
		}
		// 왼손,오른손....
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
		
		g_GuildInvenWnd.UpdateGuildInvenPcItem();
		
	}
	
	return;
}

void CRockClient::Proc_RpScDrawFrontier( SRpScDrawFrontierBank *aMsg )
{
	//	BYTE			theResult;
	//	URpdItem		theItem;
	//	DWORD			theBankItemID;		// 보관함에서 처리된 아이템 ID
	//	BYTE			theBankBundle;		// 보관함에 남은 번들 수

	
/*
#ifdef C_KEEPING_CHARGE
	if(aMsg->theResult == n_Short_DrawMoney )
	{
		SystemPrint( n_DarkRed , n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_SHORT_DRAWMOENY) , aMsg->theDrawCost ); 
		return;
	}
#endif	
*/	
/*		
#ifdef C_KEEPING_CHARGE
		char eldstr[256] = {0,};
		DWORD Eld = g_WarehouseWnd.m_Eld - aMsg->theDrawCost;
		g_WarehouseWnd.m_Eld = Eld;
		InventoryWnd.SetEldText( eldstr, g_WarehouseWnd.m_Eld );
		g_WarehouseWnd.m_EldText->SetText( eldstr );
		SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FEE_EXPENSES) , aMsg->theDrawCost );
#endif		
*/

	if( GetItemProtocolResult( aMsg->theResult ) )
	{
	
		int i = 0;
		
		for( i = 0; i < c_MaxGuildInvenPcItemNum; ++i )
		{
			if( g_GuildInvenWnd.m_GuildInvenPcItem[i].IType == n_ITYPE_NONE )
			{
				continue;
			}
			
			if( aMsg->theBankItemID == g_GuildInvenWnd.m_GuildInvenPcItem[i].ItemUID )
			{
				//판매후 남은 번들 수. 0 인 경우 아이템 삭제.
				g_GuildInvenWnd.m_GuildInvenPcItem[i].Amount -= aMsg->theBankBundle;
				
				if( g_GuildInvenWnd.m_GuildInvenPcItem[i].Amount == 0 )
				{
					g_GuildInvenWnd.m_GuildInvenPcItem[i].Clear();
				}		
				
				break;
			}
		}
		
		
		SPcItem* addPcItem = NULL;
		addPcItem = SAFE_NEW( SPcItem );
		
		BOOL IsInsertItem = FALSE;
		
		if(aMsg->theDrawItem.theItem.theCode == 0 ) // 길드창고에서 내가찾지않는 경우는 그려주지않는다
		{
			return;
		}
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->theDrawItem.theItem.theCode );
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
		{
			addPcItem->IType		= n_ITYPE_WEAR;
			addPcItem->Amount		= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theDrawItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			addPcItem->IType	= n_ITYPE_USE;//소모품
			addPcItem->Amount	= aMsg->theDrawItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			addPcItem->IType	= n_ITYPE_UNUSE;//비소모품
			addPcItem->Amount	= aMsg->theDrawItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
		{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= aMsg->theDrawItem.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone ||
			ItemType == n_Class_FrontierPet )
		{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= aMsg->theDrawItem.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_PersonalPet )
		{
			addPcItem->IType	= n_ITYPE_PET_USE; 
			addPcItem->Amount	=aMsg->theDrawItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			addPcItem->IType	= n_ITYPE_RIDE; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theDrawItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{
			addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->theDrawItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{
			addPcItem->IType = n_ITYPE_SKIN_WEAR;
			addPcItem->Amount	= 1;
			memcpy( &(addPcItem->WearTable), &(aMsg->theDrawItem) , sizeof( URpdItem ) );
		}
		
		addPcItem->Class		= ItemType;
		addPcItem->ItemUID		= aMsg->theDrawItem.theItem.theItemUID;
		addPcItem->Code			= aMsg->theDrawItem.theItem.theCode;
		addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theDrawItem.theItem.theCode );
		addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theDrawItem.theItem.theCode );
		
		if( addPcItem->ItemTable == NULL )
		{
			SAFE_DELETE( addPcItem );	
			return;
		}

		////-- 현재 아이템에 관련된 딜레이가 적용되었는지 체크해서 넣어준다.					
		if( addPcItem->IType == n_ITYPE_USE && addPcItem->ItemTable )
		{
			 int iDelayGroup = addPcItem->ItemTable->theDelayGroup;
			 if( !(nRui->enableDelayTime[iDelayGroup] ) )						
			 {
				addPcItem->IsDelay = true;						
			 }
		}	
		
		// 왼손,오른손....
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
		
		SAFE_DELETE( m_PcInven->InvenItem[aMsg->theDrawItem.theItem.thePosition] );		
		m_PcInven->InvenItem[aMsg->theDrawItem.theItem.thePosition] = addPcItem;
		
		IsInsertItem = TRUE;
		
		for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
		{
			if( ( m_PcInven->QuickSlot[idx] != NULL ) && ( m_PcInven->QuickSlot[idx]->ItemUID == addPcItem->ItemUID ) )
			{
				m_PcInven->QuickSlot[idx] = addPcItem;
				IsInsertItem = TRUE;
				break;
			}
		}
		
		if(FALSE == IsInsertItem)
		{
			SAFE_DELETE( addPcItem );
		}
		
		//슬롯 재설정....
		MainMenuWnd.UpdateQuickSlotItem();		
		InventoryWnd.UpdateInvenSlotItem();
	}
	
	return;
}

void CRockClient::Proc_RpScChangeSlotFrontier( SRpScChangeSlotFrontierBank *aMsg )
{
	//	DWORD			theBankItemID1;		// 옮긴 보관 src 아이템 ID( 0일 경우 빈슬롯이 된다.)
	//	BYTE			theSlot1;			// theBankItemID1 src 가 위치한 슬롯
	//	DWORD			theBankItemID2;		// 옮긴 보관 dest 아이템 ID( 0일 경우 빈슬롯이 된다.)
	//	BYTE			theSlot2;			// theBankItemID2 dest 가 위치한 슬롯
	//	BYTE			theResult;
	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		SPcItem srcPcItem; 
		SPcItem destPcItem; 
		
		int i = 0;
		int find_cnt = 0;
		
		
		if( aMsg->theBankItemID1 == 0 )
		{
			++find_cnt;
		}
		if( aMsg->theBankItemID2 == 0 )
		{
			++find_cnt;
		}
		
		for( i = 0; i < c_MaxGuildInvenPcItemNum; ++i )
		{
			if( g_GuildInvenWnd.m_GuildInvenPcItem[i].IType == n_ITYPE_NONE )
			{
				continue;
			}
			
			if( aMsg->theBankItemID1 == g_GuildInvenWnd.m_GuildInvenPcItem[i].ItemUID )
			{
				srcPcItem = g_GuildInvenWnd.m_GuildInvenPcItem[i];
				++find_cnt;
			}
			else if( aMsg->theBankItemID2 == g_GuildInvenWnd.m_GuildInvenPcItem[i].ItemUID )
			{
				destPcItem = g_GuildInvenWnd.m_GuildInvenPcItem[i];
				++find_cnt;
			}
			
			if( find_cnt == 2 )
			{				
				break;
			}
		}
		
		g_GuildInvenWnd.m_GuildInvenPcItem[aMsg->theSlot1] = srcPcItem;
		g_GuildInvenWnd.m_GuildInvenPcItem[aMsg->theSlot2] = destPcItem;
	}

	nRui->m_KeyLock.GuildInven.SetUnLock();
	g_GuildInvenWnd.UpdateGuildInvenPcItem();

	return;
}

#endif //DECO_RENEWAL_GUILD_MJH

///---------------------------------------------------------------------------
///-- Proc_RpScIEWndOpen
///---------------------------------------------------------------------------
///-- Item 개조 Skill 결과 : 창을 띄운다.
void CRockClient::Proc_RpScEldacraft( SRpScEldacraft *aMsg)
{
	switch( NCraftType ( aMsg->theType ) )
	{
	case n_SetStone: ///-- 개조
		nRui->SendEvent( WID_SUB_ENCHANT_RESULT, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		g_ItemEnchantWndProc.OpenEnchantWnd();
		break;
	case n_ExtractStone: ///-- 추출

		nRui->SendEvent( WID_SUB_EXTRACT_RESULT, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		g_ItemExtract_WndProc.OpenExtract_Wnd();
		break;
	case n_RemodelStone: ///-- 강화
		nRui->SendEvent( WID_SUB_REMODEL_FA, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( WID_SUB_REMODEL_SUC, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		g_ItemRemodel_WndProc.OpenRemodel_RWnd();
		break;
	case n_MakeWeapon:
	case n_MakeArmor:
		nRui->SendEvent( WID_ITEMCREATEF_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( WID_ITEMCREATES_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );			
		g_ItemCreate_WndProc.OpenCreate_Wnd( NCraftType ( aMsg->theType ) , g_Pc.m_MainPC.char_info.theCitizen );
		break;
		
	default:
		break;
	}
	
	
}
///---------------------------------------------------------------------------
///-- Proc_RpScSetStone
///---------------------------------------------------------------------------
///-- Item 개조 창에서 Game 시작 버튼을 누른 결과

///-- mini game 을 시작한다.
void CRockClient::Proc_RpScSetStone( SRpScSetStone *aMsg )
{	
	
	if( aMsg->theResult == n_FailSetStone )
	{
		///--엘다스톤과 아이템 전부다 사라진다.
		g_ItemEnchantWndProc.CloseEnchantWnd();		
		
		///-----------------------------------------------------------------------
		///-- 전체 인벤토리 Access
		for(int i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}

			///-- 아이템 소실 명성템만 삭제를 한다 . 
			if( m_PcInven->InvenItem[i]->ItemUID == aMsg->theItem.theItem.theItemUID ) 
			{
				SAFE_DELETE( m_PcInven->InvenItem[i] );				
			}

#ifdef PAYITEM_6			
			///-- 각 유료 아이템 별
			for( int j = 0 ; j < 3 ; ++j )
			{
				if ( m_PcInven->InvenItem[i] && 
					 m_PcInven->InvenItem[i]->ItemUID == aMsg->thePayItem[j].theItemID ) 
				{
					///-- Amount 변경
					m_PcInven->InvenItem[i]->Amount = aMsg->thePayItem[j].theBundle;
					
					///-- 아이템 완전 삭제
					if( m_PcInven->InvenItem[i]->Amount < 1 )
					{	
						SAFE_DELETE( m_PcInven->InvenItem[i] );					
					}
				}			
			}
#endif

		}
		
		///-----------------------------------------------------------------------
		///-- 전체 인벤토리 Access
		for( short i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}

			///-- 엘다스톤 삭제
			if ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theDelStoneID ) 
			{
				///-- 아이템 완전 삭제
				if( m_PcInven->InvenItem[i]->Amount <=1 )
				{	
					SAFE_DELETE( m_PcInven->InvenItem[i] );				
				}
				///-- 번들 개수 줄임
				else
				{
					--( m_PcInven->InvenItem[i]->Amount );
				}
			}
		}
		
		///-- 인벤 체크 
		InventoryWnd.UpdateInvenSlotItem();
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ELDASTONE_ENCHANT_FAILE));
		
		return;
	}	
	
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		g_ItemEnchantWndProc.CloseEnchantWnd();
		
		///-----------------------------------------------------------------------
		///-- 전체 인벤토리 Access
		for(int i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}
			///-- 강화 아이템 변경

			if( m_PcInven->InvenItem[i]->ItemUID == aMsg->theItem.theItem.theItemUID) 
			{
				memcpy( &(m_PcInven->InvenItem[i]->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );				
			}
			
#ifdef PAYITEM_6			
			///-- 각 유료 아이템 별
			for( int j = 0 ; j < 3 ; ++j )
			{
				if ( m_PcInven->InvenItem[i] && 
					 m_PcInven->InvenItem[i]->ItemUID == aMsg->thePayItem[j].theItemID ) 
				{
					///-- Amount 변경
					m_PcInven->InvenItem[i]->Amount = aMsg->thePayItem[j].theBundle;
					
					///-- 아이템 완전 삭제
					if( m_PcInven->InvenItem[i]->Amount < 1 )
					{	
						SAFE_DELETE( m_PcInven->InvenItem[i] );					
					}
				}			
			}
#endif
		}
		
		///-----------------------------------------------------------------------
		///-- 전체 인벤토리 Access
		for(short i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}
			///-- 엘다스톤 삭제
			if ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theDelStoneID ) 
			{
				///-- 아이템 완전 삭제
				if( m_PcInven->InvenItem[i]->Amount <=1 )
				{	
					delete( m_PcInven->InvenItem[i] );
					m_PcInven->InvenItem[i] = NULL;
				}
				///-- 번들 개수 줄임
				else
				{
					-- ( m_PcInven->InvenItem[i]->Amount );
				}
			}
		}
		
		///-- 인벤 체크 
		InventoryWnd.UpdateInvenSlotItem();
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ELDASTONE_ENCHANT_SUCCESS));
	}
	else
	{
		g_ItemEnchantWndProc.Unlock_button();
	}
}


///---------------------------------------------------------------------------
///-- Proc_RpScMakeItem
///---------------------------------------------------------------------------	
///-- Make Item Minigame 시작
void CRockClient::Proc_RpScMakeItem( SRpScMakeItem * aMsg)
{	
	//// 아이템 업그레이드 관련 
	//	BYTE			theResult;
	//	long			theMapIndex;
	//#ifdef C_PAY_SYSTEM
	//	BYTE			theGameTime;	// 기본 100% 
	//#endif
	//	SBundleItem		theDelItem[6];		// 삭제할 아이템 (유료아이템으로 소멸이 방지된 경우 리스트에서 제외됨)
	// SRpScMakeItem
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		///-----------------------------------------------------------------------
		///-- 아이템 제거 
		for(int i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}
			
			///-- 각 Slot 별
			for(int j = 0 ; j < 6 ; ++j )
			{
				///-- 잡탬 검색

				if ( m_PcInven->InvenItem[i] && 
					m_PcInven->InvenItem[i]->ItemUID == aMsg->theDelItem[j].theItemID ) 
				{
					///-- Amount 변경

					m_PcInven->InvenItem[i]->Amount = aMsg->theDelItem[j].theBundle;
					
					///-- 아이템 완전 삭제
					if( m_PcInven->InvenItem[i]->Amount < 1 )
					{	
						delete( m_PcInven->InvenItem[i] );
						m_PcInven->InvenItem[i] = NULL;
					}
				}			
			}
			
			///-- 각 유료 아이템 별
			for(short j = 0 ; j < 3 ; ++j )
			{
				///-- 잡탬 검색

				if ( m_PcInven->InvenItem[i] && 
					m_PcInven->InvenItem[i]->ItemUID == aMsg->thePayItem[j].theItemID ) 
				{
					///-- Amount 변경

					m_PcInven->InvenItem[i]->Amount = aMsg->thePayItem[j].theBundle;
					
					///-- 아이템 완전 삭제
					if( m_PcInven->InvenItem[i]->Amount < 1 )
					{	
						delete( m_PcInven->InvenItem[i] );
						m_PcInven->InvenItem[i] = NULL;
					}
				}			
			}
		}
		
		///-- 인벤 체크 
		InventoryWnd.UpdateInvenSlotItem();		
		
		nRui->SendEvent( WID_ITEMCREATE_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
		
		nRui->SetCursorType(n_CT_NORMAL);
		
		if( g_ItemCreate_WndProc.Get_Craft_Weapon() )
		{
			nRui->MiniGameStartUp( 1, aMsg->theMapIndex, aMsg->theGameTime );			
		}
		else
		{
			nRui->MiniGameStartUp( 2, aMsg->theMapIndex, aMsg->theGameTime );			
		}
	}
	g_ItemCreate_WndProc.Unlock_button();
}
///---------------------------------------------------------------------------
///-- Proc_ScMakeItemResult
///---------------------------------------------------------------------------
///-- Make Item Minigame 끝
void CRockClient::Proc_ScMakeItemResult( SRpScMakeItemResult *aMsg )
{
	DWORD li_subexp;	
	SPcItem*	tmpPcItem = NULL;
	
	///-- 성공

	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		///-- Sub 경험치 반영
		li_subexp = aMsg->theAddExp;
				
		nRui->thePcParam.SubExp = aMsg->theSubExp;	
		

		int NeedSubExp	= g_Pc_Manager.GetStatNeedSubExp( nRui->thePcParam.SubClass, nRui->thePcParam.SubLevel );
		//Max 경험치... 
		if(nRui->thePcParam.SubExp >= NeedSubExp)
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_SUBJOB_MESSAGE1));
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_SUBJOB_MESSAGE2));
		}

		///-- Item 생성
		if( aMsg->theItem.theItem.theItemUID != 0 && aMsg->theItem.theWasteItem.theBundle !=0 )
		{
			///-- 추출된 Elda Stone 
			tmpPcItem = SAFE_NEW( SPcItem );
	
			tmpPcItem->Class	= g_Pc_Manager.GetItemClsss( aMsg->theItem.theItem.theCode );
			tmpPcItem->ItemUID	= aMsg->theItem.theItem.theItemUID;
			tmpPcItem->IType = n_ITYPE_WEAR;	///-- Elda Stone
			tmpPcItem->Amount = 1;
			tmpPcItem->Code			= aMsg->theItem.theItem.theCode;
			tmpPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theItem.theItem.theCode );				
			tmpPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItem.theItem.theCode );
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItem.theWearItem), sizeof( SRpdWearItemBase ) );
			
			if( tmpPcItem->ItemTable == NULL )
			{
				SAFE_DELETE( tmpPcItem );
				return;
			}
			
			tmpPcItem->WearTable.theEndurance = tmpPcItem->ItemTable->theMaxEndurance;			

			SAFE_DELETE( m_PcInven->InvenItem[aMsg->theItem.theItem.thePosition] );
			m_PcInven->InvenItem[aMsg->theItem.theItem.thePosition] = tmpPcItem;		
			
			///-- 제조 성공 창
			g_ItemCreate_S_WndProc.OpenCreate_S_WndProc(true, aMsg->theItem.theItem.theItemUID, 
				li_subexp, (int)aMsg->thePenalty );
		}		
	}
	///-----------------------------------------------------------------------
	///-- 제조 실패
	else if( aMsg->theResult == 0 )
	{	
		///-- Sub 경험치 반영
		li_subexp = aMsg->theSubExp - nRui->thePcParam.SubExp;
		nRui->thePcParam.SubExp = aMsg->theSubExp;
			
		int NeedSubExp	= g_Pc_Manager.GetStatNeedSubExp( nRui->thePcParam.SubClass, nRui->thePcParam.SubLevel );
		//Max 경험치... 
		if(nRui->thePcParam.SubExp >= NeedSubExp)
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_SUBJOB_MESSAGE1));
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_SUBJOB_MESSAGE2));
		}


		g_ItemCreate_WndProc.Unlock_button();
		g_ItemCreate_S_WndProc.OpenCreate_S_WndProc(false, 0, li_subexp, (int)aMsg->thePenalty );
		
		///-- 제조 실패 창
		//g_ItemCreate_F_WndProc.OpenCreate_F_WndProc( aMsg->theDelItem, li_subexp, (int)aMsg->thePenalty );
	}	
	///-----------------------------------------------------------------------
	///-- 제조 불가
	else 
	{
		///-- 인벤 체크 
		InventoryWnd.UpdateInvenSlotItem();
		return;
	}
	
	///-----------------------------------------------------------------------
	///-- 아이템 제거 
	for(int i = 0; i < c_MaxInvenItemNum; ++i )
	{	
		if( m_PcInven->InvenItem[i] == NULL )
		{
			continue;
		}
		
		///-- 각 Slot 별
		for(int j = 0 ; j < 6 ; ++j )
		{
			///-- 잡탬 검색

			if ( m_PcInven->InvenItem[i] && 
				m_PcInven->InvenItem[i]->ItemUID == aMsg->theDelItem[j].theItemID ) 
			{
				///-- Amount 변경

				m_PcInven->InvenItem[i]->Amount = aMsg->theDelItem[j].theBundle;
				
				///-- 아이템 완전 삭제
				if( m_PcInven->InvenItem[i]->Amount < 1 )
				{	
					SAFE_DELETE( m_PcInven->InvenItem[i] );					
				}
			}			
		}								
	}
	
	InventoryWnd.UpdateInvenSlotItem();	
}

///---------------------------------------------------------------------------
///-- Proc_RpScEldacraftGame
///---------------------------------------------------------------------------
///-- Game 결과 전송
///-- 성공시 아이템 변경

void CRockClient::Proc_RpScEldacraftGame( SRpScEldacraftGame *aMsg )
{	
	int li_subexp;
	int i;
	///-- 성공

	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		///-----------------------------------------------------------------------
		///-- 전체 인벤토리 Access
		for( i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}
			///-- 강화 아이템 변경

			if( m_PcInven->InvenItem[i]->ItemUID == aMsg->theItem.theItem.theItemUID) 
			{
				memcpy( &(m_PcInven->InvenItem[i]->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );				
			}
		}
		
		///-- Sub 경험치 반영
		li_subexp = aMsg->theSubExp - nRui->thePcParam.SubExp;
		nRui->thePcParam.SubExp = aMsg->theSubExp;
		
		///-- 결과창
		g_ItemEnchant_RWndProc.OpenEnchantR_SWnd( aMsg->theItem.theItem.theItemUID, aMsg->theDelStoneID, li_subexp, (int)aMsg->thePenalty );
	}
	///-- 실패
	else
	{
		///-- Sub 경험치 반영
		li_subexp = aMsg->theSubExp - nRui->thePcParam.SubExp;
		nRui->thePcParam.SubExp = aMsg->theSubExp;
		g_ItemEnchant_RWndProc.OpenEnchantR_FWnd( aMsg->theDelStoneID, li_subexp, (int)aMsg->thePenalty );
		
	}
	
	
	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{	
		if( m_PcInven->InvenItem[i] == NULL )
		{
			continue;
		}
		///-- 엘다스톤 삭제
		if ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theDelStoneID ) 
		{
			///-- 아이템 완전 삭제
			if( m_PcInven->InvenItem[i]->Amount <=1 )
			{	
				delete( m_PcInven->InvenItem[i] );
				m_PcInven->InvenItem[i] = NULL;
			}
			///-- 번들 개수 줄임
			else
			{
				-- ( m_PcInven->InvenItem[i]->Amount );
			}
		}
	}
	
	///-- 인벤 체크 
	InventoryWnd.UpdateInvenSlotItem();
}

//.........................................................................
// Proc_RpScSeeAuroraSkill
//.........................................................................		
void CRockClient::Proc_RpScSeeAuroraSkill(SRpScSeeAuraSkillPtr aMsg)
{
	int HolyLevel = 0 ; 


#ifdef HOLY_SKILLICON_MODIFY

/*
	int size = sizeof(_SObjectID);
	BYTE *p = (BYTE *)aMsg->theAddTargetList;
	memcpy(&HolyLevel , p + (aMsg->theAddTargetNum * size) ,  sizeof(BYTE) );
	memcpy( p , &aMsg->theAddTargetList , sizeof(size)* aMsg->theAddTargetNum );
*/
	
	HolyLevel = aMsg->theFrontierPetLevel;
#endif

	g_Particle.m_EffectManager.CreateAuroraEffect(
		aMsg->theSkillCode ,
		aMsg->theAddTargetNum,
		aMsg->theAddTargetList , 
		HolyLevel );
}

//.........................................................................
// Proc_RpScKeepupSkillStart
//.........................................................................		
void CRockClient::Proc_RpScKeepupSkillStart(SRpScKeepupSkillStartPtr aMsg)
{
	
	g_Pc.GetPlayer()->m_KeepUpChecker.SetCode( aMsg->theSkillCode );
	
	g_Particle.m_EffectManager.CreateKeepupSkillStart(
		( DWORD ) aMsg->theSkillCode   ,
		aMsg->theEffectType  , 
		aMsg->theEffectValue ,
		aMsg->theEffectMaxValue );	
}


// 본인과 가시범위내 유저들의 체력등 최대치 정보 수정 
void CRockClient::Proc_RpScUpdateMaxStat( SRpScUpdateMaxStat *aMsg)
{
	//	DWORD			thePCID;
	//	WORD			theMaxHP;		// 기본 값 + 유저 설정값 + Item + Skill
	//	WORD			theMaxSP;
	//	WORD			theMaxMP;
	
	
	if( g_Pc.m_MainPC.lUnique == aMsg->thePCID )
	{
		nRui->thePcParam.MaxHP = aMsg->theMaxHP; 
		nRui->thePcParam.MaxSP = aMsg->theMaxSP; 
		nRui->thePcParam.MaxMP = aMsg->theMaxMP; 
	}
	else
	{
		int User_Idx = g_Pc.FindUser( aMsg->thePCID );
		
		if( User_Idx == MAX_USER_INTABLE )
			return;
		
		g_Pc.m_PcTable[User_Idx].char_info.theMaxHP = aMsg->theMaxHP; 
		
		int chrAttrIndex = g_Pc.m_Pc[User_Idx].GetAttrIndex();		
		
		if( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman )
		{
			g_Pc.m_PcTable[User_Idx].char_info.theMaxSMP = aMsg->theMaxSP; 
		}
		else if( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman )
		{
			
			g_Pc.m_PcTable[User_Idx].char_info.theMaxSMP = aMsg->theMaxMP; 
		}		
	}
	
	return;
}

///-----------------------------------------------------------------------
///-- 날씨 관련 By wxywxy
///-----------------------------------------------------------------------
void CRockClient::Proc_RpScWeather(SRpScWeatherPtr aMsg)
{
	switch( aMsg->theWeather )
	{
		// 맑음
	case n_Clean:
		
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NONE );
		
		break;
		
		// 비

	case n_Rain:
		
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_RAIN );
		
		break;
		
		// 눈 
	case n_Snow:
		
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_SNOW );
		
		break;
		
	case n_Sakura:
		
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_LEAF );
		
		break;
		
	case n_ColorPaper:
		
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_PAPER );
		
		break;
	case n_Night:
		
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NIGHT );
		
		break;
	}	
}
///---------------------------------------------------------------------------
///-- Proc_ScSubLevelup
///---------------------------------------------------------------------------
void CRockClient::Proc_ScSubLevelup( SRpScSubLevelup * aMsg)
{
	
	nRui->thePcParam.SubExp = aMsg->theSubExp;
	nRui->thePcParam.SubLevel = aMsg->theSubLevel;	


	SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SUBJOB_LEVELUP ) );
	SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SUBJOB_NEXT_QUEST ) );



	return;
}


void CRockClient::Proc_ScSkillUp( SRpScSkillUp* aMsg )
{
	//	WORD			theBeforeSkillCode;		// 해당 스킬
	//	WORD			theAfterSKillCode;		// 레벨업된 스킬 코드

	//	WORD			theSkillPoint;
	//	BYTE			theResult;				// n_Success, n_Fail, n_NOT_ENOUGH_POINT, n_NOT_EXIST_PRESKILL
	
	if( aMsg->theResult != n_Success )
	{
		switch( aMsg->theResult )
		{
		case n_Fail:
			break;
		case n_NOT_ENOUGH_POINT:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SH_POINT_FOR_LEVUP ) );
			}
			break;
		case n_ALREADY_MASTERSKILL:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ALREADY_MAX_LEVEL ) );				
			}
			break;
			
		default:
			break;
		}
		
		return;
	}
	
	nRui->GetPcParamInfo()->SkillPoint = aMsg->theSkillPoint;
	
	SSkillBaseInfo* skill_Info = NULL;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aMsg->theBeforeSkillCode );
	if( skill_Info == NULL )
		return;	
	
	SSkillBaseInfo* upskill_Info = NULL;
	upskill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aMsg->theAfterSKillCode );
	if( upskill_Info == NULL )
		return;
	
	// 액티브 스킬은 하나 더 추가한다. 
	if( skill_Info->theSkillType1 == n_ActiveSkill )
	{
		//스킬창에 스킬 추가
		SPcItem* pcitem = NULL;
		
		for( int i = 0; i < RP_MAX_USER_SKILL; ++i )
		{
			if( m_PcInven->SkillItem[i] == NULL )
			{
				// SPcItem객체생성
				pcitem = SAFE_NEW( SPcItem );
				
				// SPcItem객체에 SkillItem값 설정
				pcitem->IType		= n_ITYPE_SKILL;
				pcitem->Class		= n_ICLASS_NONE;
				pcitem->ItemUID		= 0;
				pcitem->Code		= aMsg->theAfterSKillCode;
				pcitem->Tid			= (TEXID)g_Pc_Manager.GetSkillIconTID( aMsg->theAfterSKillCode );
				pcitem->TidEx		= (TEXID)g_Pc_Manager.GetSkillAttriIconTID( aMsg->theAfterSKillCode );
				pcitem->Amount		= 1;
				pcitem->SkillTable	= g_Pc_Manager.GetSkillBaseInfoPtr( aMsg->theAfterSKillCode );
				pcitem->Nation		= g_Pc.m_MainPC.char_info.theCitizen;

				m_PcInven->SkillItem[i] = pcitem;
				
				nRui->UpdateMaxGage( aMsg->theAfterSKillCode, true );	
				
				break;
			}
		}
		
		//Skill창 업데이트
		g_SkillWnd.UpdateSkillSlotItem();
		
		SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARNED_SKILL ), upskill_Info->theSkillName );
	}
	// 패시브 스킬은 전에 스킬을 레벨업 된 스킬로 변환한다. 
	else if( skill_Info->theSkillType1 == n_PasiveSkill )
	{
		SPcItem * upSkillItem = NULL;
		
		for( int i = 0; i < RP_MAX_USER_SKILL; ++i )
		{
			if( m_PcInven->SkillItem[i] )
			{
				if( m_PcInven->SkillItem[i]->Code == aMsg->theBeforeSkillCode )
				{
					m_PcInven->SkillItem[i]->IType		= n_ITYPE_SKILL;
					m_PcInven->SkillItem[i]->Class		= n_ICLASS_NONE;
					m_PcInven->SkillItem[i]->ItemUID	= 0;
					m_PcInven->SkillItem[i]->Code		= aMsg->theAfterSKillCode;
					m_PcInven->SkillItem[i]->Tid		= (TEXID)g_Pc_Manager.GetSkillIconTID( aMsg->theAfterSKillCode );
					m_PcInven->SkillItem[i]->TidEx		= (TEXID)g_Pc_Manager.GetSkillAttriIconTID( aMsg->theAfterSKillCode );
					m_PcInven->SkillItem[i]->Amount		= 1;
					m_PcInven->SkillItem[i]->Nation		= g_Pc.m_MainPC.char_info.theCitizen;

					m_PcInven->SkillItem[i]->SkillTable	= g_Pc_Manager.GetSkillBaseInfoPtr( aMsg->theAfterSKillCode );
					
					nRui->UpdateMaxGage( aMsg->theAfterSKillCode, true );	
					
					break;
				}
			}
		}
		
		//Skill창 업데이트
		g_SkillWnd.UpdateSkillSlotItem();
		//메인메뉴 퀵슬롯 갱신
		MainMenuWnd.UpdateQuickSlotItem();
		
		SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_LEARNED_SKILL ), upskill_Info->theSkillName );
	}
	
	SkillLearnWnd.SetClassSkillLearnInfo();
	

	return;
}

///---------------------------------------------------------------------------
///-- Proc_RpScOpenRepairWnd
///---------------------------------------------------------------------------
///-- ultralyul
void CRockClient::Proc_RpScOpenRepairWnd( SRpScOpenRepairSkill *aMsg )
{
	if( aMsg->theRepairType == n_ByRepairSkill )
	{
		g_ItemRepair_WndProc.OpenRepair_Wnd( false );
	}
	else if( aMsg->theRepairType == n_ByRepairNPC)
	{
		//상점, 인벤 띄우기....위치처리....
		SRect size;
		//			NpcChatWnd.FrameWnd->Reposition( 198, 108 );
		g_ItemRepair_WndProc.FrameWnd->Reposition( 184, 394 );
		g_ItemRepair_WndProc.FrameWnd->GetClientSize( &size );
		size.x += size.w;
		size.y;
		
		InventoryWnd.FrameWnd->Reposition( size.x, size.y );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
		InventoryWnd.Open();
#else
		nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
		g_ItemRepair_WndProc.OpenRepair_Wnd( true );
	}
}

///---------------------------------------------------------------------------
///-- Proc_RpScUpdateDurabilityItem
///---------------------------------------------------------------------------
void CRockClient::Proc_RpScUpdateDurabilityItem( SRpScUpdateDurabilityItem *aMsg)
{
	int j,i;
	
	for( j = 0 ; j < aMsg->theCount ; ++j )
	{
		///-- 해당 아이템 찾음 
		for( i = 0 ; i < RP_MAX_EQUIP; ++i )
		{
			if( ( m_PcInven->WearItem[i] != NULL ) && ( m_PcInven->WearItem[i]->ItemUID == aMsg->theRePairItemArray[j].theItemID ) )
			{
				m_PcInven->WearItem[i]->WearTable.theEndurance = aMsg->theRePairItemArray[j].theEndurance;
				g_ItemRepair_WndProc.Refresh( aMsg->theRePairItemArray[j].theItemID );
			}
		}
		for( i = 0 ; i < c_MaxInvenItemNum; ++i )
		{
			///-- 해당 아이템 찾음 
			if( ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theRePairItemArray[j].theItemID ) )
			{
				m_PcInven->InvenItem[i]->WearTable.theEndurance = aMsg->theRePairItemArray[j].theEndurance;
				g_ItemRepair_WndProc.Refresh( aMsg->theRePairItemArray[j].theItemID );
			}
			
		}
	}
	nRui->Refresh_ItemHelpTip();
	
}

///---------------------------------------------------------------------------
///-- Proc_RpScReplyRepair
///---------------------------------------------------------------------------
void CRockClient::Proc_RpScReplyRepair( SRpScReplyRepair *aMsg )
{
	
	if( GetItemProtocolResult( aMsg->theErrorCode ) )
	{
		nRui->thePcParam.Money = aMsg->theEld;
		g_ItemRepair_WndProc.Clear();
		
	}

	nRui->SendEvent( WID_InventoryWnd , n_emSetVisible, (EPARAM)false , 0, 0, 0 );
	nRui->SendEvent( WID_REPAIR_WND , n_emSetVisible, (EPARAM)false , 0, 0, 0 );

	g_ItemRepair_WndProc.Unlock_button();
}

///-----------------------------------------------------------------------
///-- community Protocols - 친구 , 차단 , 메신져 채팅 by simwoosung 
///-----------------------------------------------------------------------
void CRockClient::Proc_ScMsngrError( SRpScMsngrError * aMsg )
{
	RLG2( "SRpScMsngrError  %d   %s", aMsg->theErrorCode, aMsg->theStr );
	TCHAR tMessage[256]	= {0,};
	
	switch(aMsg->theErrorCode)
	{
	case n_MsngrESystemError:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_SERVER_ERROR ) );	
			MessageBoxWnd.Open( tMessage, 1500, NULL);	
		}
		break;
	case n_MsngrERegistOverflow:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_OVER_MAXUSER ) );	
			MessageBoxWnd.Open( tMessage, 1500, NULL);
		}
		break;
	case n_MsngrENotConnected:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_NOTCONNECTED ), RWCHAR(aMsg->theStr) );	
			MessageBoxWnd.Open(tMessage, 1500, NULL);
		}
		break;
	case n_MsngrEYouBlocked:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_BLOCK_BY ), RWCHAR(aMsg->theStr) );	
			MessageBoxWnd.Open(tMessage, 1500, NULL);	
		}
		break;
	case n_MsngrEInMyBlockList:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_IN_BLOCK_LIST ), RWCHAR(aMsg->theStr) );  
			MessageBoxWnd.Open(tMessage, 1500, NULL);	
		}
		break;		
	case n_MsngrEAlreadyRegistered:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_ALREADY_FRIEND ), RWCHAR(aMsg->theStr) );
			MessageBoxWnd.Open(tMessage, 1500, NULL);	
		}
		break;
	case n_MsngrEAlreadyBlocked:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_ALREADY_BLOCK ), RWCHAR(aMsg->theStr) );	
			MessageBoxWnd.Open(tMessage, 1500, NULL);	
		}
		break;
	case n_MsngrEChangeOptFail:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_FAIL_OPTION )); 
			MessageBoxWnd.Open(tMessage, 1500, NULL);	
		}
		break;
	case n_MsngrESendSayFail:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_FAIL_SEND_MSG ));	
			MessageBoxWnd.Open(tMessage, 1500, NULL);	
		}
		break;
	case n_MsngrEChatRoomJoinFail:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_FIAL_JOIN_ROOM ) );	
			MessageBoxWnd.Open(tMessage, 1500, NULL);	
		}
		break;
	case n_MsngrEDenyInsertMember:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_REFUSE_FRIEND ), RWCHAR(aMsg->theStr) );
			MessageBoxWnd.Open(tMessage, 1500, NULL);				
		}
		break;
	case n_MsngrNotSameCitizen:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_SYSPR_NOT_INVITATION3 ));
			MessageBoxWnd.Open(tMessage, 1500, NULL);				
		}
		break;
	}
}

void CRockClient::Proc_ScMsngrMemberState( SRpScMsngrMemberState * aMsg )
{
	RLG3( "SRpScMsngrMemberState  %d  %s  %d", aMsg->theProcType, aMsg->thePCName, aMsg->theState);
	RLG4( "%d  %d  %d  %d", aMsg->theChannel, aMsg->theMap, aMsg->theLevel, aMsg->theJob );

#ifdef DECO_RENEWAL_GUILD_MJH

	std::vector < SFMemberInfo > :: iterator ir;							
			
	ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();
	
	for( ; ir != g_FrontierMemberListWnd.m_FrontierMemberInfoList.end() ; ++ir )
	{
		if( Rstrcmp( (*ir).Name, aMsg->thePCName ) == 0 )
		{
			(*ir).Level= aMsg->theLevel;
			(*ir).TheMap= aMsg->theMap;
			break;
		}
	}	
	g_FrontierMainWnd.RefreshFrontierMyInfo();
	g_FrontierMainWnd.RefreshFrontierMyFrontier();
	g_FrontierMemberListWnd.RefreshMemberInfoList();

#endif // DECO_RENEWAL_GUILD_MJH
	
	SFriendData *pFriendData = NULL;
	UINT uFriendState = 0;
	
	switch(aMsg->theProcType)
	{
	case n_MsngrInsert:
		{
#ifdef FRONTIER
			if(aMsg->theFrontierFlag)
			{
				if(g_MCommunityWnd.m_MGuildTabWnd.FindFriendData(aMsg->thePCName))
					return;
			}
			else
			{
				if(g_MCommunityWnd.m_MFriendTabWnd.FindFriendData(aMsg->thePCName))
					return;
			}
#else
			if(g_MCommunityWnd.m_MFriendTabWnd.FindFriendData(aMsg->thePCName))
				return;
#endif			
			pFriendData = SAFE_NEW( SFriendData );			
			memset(pFriendData->strFName , NULL , sizeof(pFriendData->strFName)) ;
			memcpy(pFriendData->strFName, aMsg->thePCName, sizeof(aMsg->thePCName));								
		}
		break;
	case n_MsngrUpdate:
		{
#ifdef FRONTIER
			if(aMsg->theFrontierFlag)
			{
				pFriendData = g_MCommunityWnd.m_MGuildTabWnd.FindFriendData(aMsg->thePCName);
			}
			else
			{
				pFriendData = g_MCommunityWnd.m_MFriendTabWnd.FindFriendData(aMsg->thePCName);
			}
#else
			pFriendData = g_MCommunityWnd.m_MFriendTabWnd.FindFriendData(aMsg->thePCName);
#endif		
			
			
			if(pFriendData)
			{
				
				if( (n_MsngrStateOffline == pFriendData->uFriendState) &&  //친구가 오프라인 상태에서
					(n_MsngrStateOffline != aMsg->theState ) 
#ifdef NEW_PVP_DECO
					&& (g_Pc.m_MainPC.char_info.theCitizen == aMsg->theCitizen)
#endif	
					
					)			   //오프라인이 아닌상태로 전환되면	
				{
					SystemPrint( n_LightYellow, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CONNECT_FRIEND ), 
						aMsg->thePCName);
				}
			}
		}
		break;	
	}
	
	if(pFriendData == NULL)
		return;
	
	pFriendData->uFriendState = (UINT)(aMsg->theState);
	
	if(aMsg->theMap == -1)
	{
		pFriendData->bOpenArea = false;
		pFriendData->uiMapID = 0;	
	}
	else
	{
		pFriendData->bOpenArea = true;
		pFriendData->uiMapID = (UINT)(aMsg->theMap);
		pFriendData->nChanelNum = (int)(aMsg->theChannel);
	}
	
	if(aMsg->theLevel == -1)
	{
		pFriendData->bOpenLevel = false;
		pFriendData->iLevel = 0;	
	}
	else
	{
		pFriendData->bOpenLevel = true;
		pFriendData->iLevel = (int)(aMsg->theLevel);		
	}
	
	if(aMsg->theJob == -1)
	{
		pFriendData->bOpenJob = false;
		pFriendData->uiJob = 0;
	}
	else
	{
		pFriendData->bOpenJob = true;
		pFriendData->uiJob = (UINT)(aMsg->theJob);
	}
	
#ifdef NEW_PVP_DECO
	pFriendData->bCitizen = aMsg->theCitizen;
#endif	
	
	if(aMsg->theProcType == n_MsngrInsert)
	{
#ifdef FRONTIER
		if(aMsg->theFrontierFlag)
		{
			g_MCommunityWnd.m_MGuildTabWnd.InsertFriendData(pFriendData, false);
		}
		else
		{
			g_MCommunityWnd.m_MFriendTabWnd.InsertFriendData(pFriendData, false);
		}
#else
		g_MCommunityWnd.m_MFriendTabWnd.InsertFriendData(pFriendData, false);
#endif		
	}
	
#ifdef FRONTIER
	if(aMsg->theFrontierFlag)
	{
		g_MCommunityWnd.m_MGuildTabWnd.SortFriendData();
		g_MCommunityWnd.m_MGuildTabWnd.SetFriendField();
	}
	else
	{
		g_MCommunityWnd.m_MFriendTabWnd.SortFriendData();
		g_MCommunityWnd.m_MFriendTabWnd.SetFriendField();
	}
#else
	g_MCommunityWnd.m_MFriendTabWnd.SortFriendData();
	g_MCommunityWnd.m_MFriendTabWnd.SetFriendField();
#endif		
}

void CRockClient::Proc_ScMsngrMemberInsertAsk( SRpScMsngrMemberInsertAsk * aMsg )
{
	RLG1( "SRpScMsngrMemberInsertAsk  %s", aMsg->thePCName );
	
	g_UIMsgWnd.SetParam( aMsg->thePCName );
	g_UIMsgWnd.SetMsgType( MSG_MFRIENDINVITE );
	nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CRockClient::Proc_ScMsngrMemberDelete( SRpScMsngrMemberDelete * aMsg )
{
	RLG1( "SRpScMsngrMemberDelete  %s", aMsg->thePCName );
#ifdef FRONTIER
	if(aMsg->theFrontierFlag)
	{
		g_MCommunityWnd.m_MGuildTabWnd.DeleteFriendData(aMsg->thePCName, true);
	}
	else
	{
		g_MCommunityWnd.m_MFriendTabWnd.DeleteFriendData(aMsg->thePCName, true);
	}
#else
	g_MCommunityWnd.m_MFriendTabWnd.DeleteFriendData(aMsg->thePCName, true);
#endif		
}

void CRockClient::Proc_ScMsngrMemberBlock( SRpScMsngrMemberBlock * aMsg )
{
	RLG2( "SRpScMsngrMemberBlock  %d  %s", aMsg->theProcType, aMsg->thePCName );
	
	switch(aMsg->theProcType)
	{
	case n_MsngrInsert:
		{
			if(g_MCommunityWnd.m_MBlockTabWnd.ExistBlockName(aMsg->thePCName))
				return;
			
			SBlockData * pBlockData = SAFE_NEW( SBlockData );		
		
			SAFE_STR_CPY(pBlockData->strBDestName, aMsg->thePCName, 512);
			g_MCommunityWnd.m_MBlockTabWnd.InsertBlockData(pBlockData, true);
		}
		break;
	case n_MsngrDelete:
		{
			g_MCommunityWnd.m_MBlockTabWnd.DeleteBlockData(aMsg->thePCName, true);
		}
		break;
	}
}

void CRockClient::Proc_ScMsngrChangeOpt( SRpScMsngrChangeOpt * aMsg )
{
	RLG1( "SRpScMsngrChangeOpt  %d", aMsg->theOptState );   
	RLG3( "%d  %d  %d", aMsg->theOptMapFlag, aMsg->theOptLevelFlag, aMsg->theOptJobFlag );
	
	g_MCommunityWnd.m_MSettingTabWnd.SetMessenState((UINT)(aMsg->theOptState), false);
	g_MCommunityWnd.m_MSettingTabWnd.SetSelectLocal((BOOL)(aMsg->theOptMapFlag), false);
	g_MCommunityWnd.m_MSettingTabWnd.SetSelectLevel((BOOL)(aMsg->theOptLevelFlag), false);
	g_MCommunityWnd.m_MSettingTabWnd.SetSelectJob((BOOL)(aMsg->theOptJobFlag), false);
	
	g_MCommunityWnd.m_MSettingTabWnd.ResetSettingData();
	g_MCommunityWnd.SetMessenState((UINT)(aMsg->theOptState));
}

void CRockClient::Proc_ScMsngrSay( SRpScMsngrSay * aMsg )
{
	RLG2( "SRpScMsngrSay  %s  %s", aMsg->theFromPCName, aMsg->theStr );
	
	g_MCommunityWnd.InsertOneChatContent(aMsg->theFromPCName, aMsg->theStr);
}

void CRockClient::Proc_ScMsngrChatSystemMsg( SRpScMsngrChatSystemMsg * aMsg )
{
	RLG2( "SRpScMsngrChatSystemMsg  %d  %s", aMsg->theCode, aMsg->thePCName );
	
	TCHAR tMessage[256]	= {0,};
	
	switch(aMsg->theCode)
	{
	case n_MsngrChatAskOverflow:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_CANT_MORE_INVITE ));
			MessageBoxWnd.Open( tMessage, 1500, NULL);
		}
		break;
	case n_MsngrChatAlreadyJoinSame:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_ALREADY_SAMEROOM ), RWCHAR(aMsg->thePCName) );	
			MessageBoxWnd.Open( tMessage, 1500, NULL);
		}
		break;
	case n_MsngrChatAlreadyJoinOther:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_ALREADY_OTHERROOM ), RWCHAR(aMsg->thePCName) );
			MessageBoxWnd.Open( tMessage, 1500, NULL);
		}
		break;
	case n_MsngrChatDenyJoin:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_REFUSE_JOINROOM ), RWCHAR(aMsg->thePCName) );
			MessageBoxWnd.Open( tMessage, 1500, NULL);
		}
		break;
	case n_MsngrChatJoin:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_JOINROOM ), RWCHAR(aMsg->thePCName) );
			g_MCommunityWnd.InsertMultiChatContent(true, 0 , 215, 5, 
				NULL, tMessage);
		}
		break;
	case n_MsngrChatLeave:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_OUTROOM ), RWCHAR(aMsg->thePCName));
			g_MCommunityWnd.InsertMultiChatContent(true, 0 , 215, 5, 
				NULL, tMessage);
		}
		break;
	case n_MsngrChatSayFail:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_CANT_SEND_CHAT ) );	
			MessageBoxWnd.Open( tMessage, 1500, NULL);
		}
		break;			
	}
}

void CRockClient::Proc_ScMsngrChatMemberState( SRpScMsngrChatMemberState * aMsg )
{
	RLG2( "SRpScMsngrChatMemberState  %d  %s", aMsg->theProcType, aMsg->thePCName );
	SPcDataParam* PcParam = nRui->GetPcParamInfo();
	
	if(!g_MCommunityWnd.m_MessenMultiChatWnd.IsVisible)
	{
		return;
	}
	
	switch(aMsg->theProcType)
	{
	case n_MsngrInsert:
		{
			if(	Rstrcmp(RWCHAR(aMsg->thePCName), RWCHAR(PcParam->PCName)) != 0)			
			{
				g_MCommunityWnd.m_MessenMultiChatWnd.InsertChatMemName(aMsg->thePCName, true);
			}
		}
		break;
	case n_MsngrDelete:
		{
			g_MCommunityWnd.m_MessenMultiChatWnd.DeleteChatMemName(aMsg->thePCName, true);
		}
		break;
	}
}

void CRockClient::Proc_ScMsngrChatCreateRoom( SRpScMsngrChatCreateRoom * aMsg )
{
	RLG1( "SRpScMsngrChatCreateRoom  %d", aMsg->theResult );
	SPcDataParam* PcParam = nRui->GetPcParamInfo();
	TCHAR tMessage[256]	= {0,};
	
	if(g_MCommunityWnd.m_MessenMultiChatWnd.IsVisible)
	{
		g_MCommunityWnd.m_MessenMultiChatWnd.Init();
		g_MCommunityWnd.m_MessenMultiChatWnd.SetChatMasterName(PcParam->PCName);
		return;
	}
	
	switch(aMsg->theResult)
	{
	case 0:
		{
			g_MCommunityWnd.m_MessenMultiChatWnd.SetVisible(true);				
			g_MCommunityWnd.m_MessenMultiChatWnd.SetChatMasterName(PcParam->PCName);
		}
		break;
	case 1:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_SERVER_ERROR ) );
			MessageBoxWnd.Open( tMessage, 1500, NULL);
		}
		break;
	case 2:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_ALREADY_CREROOM ) );
			MessageBoxWnd.Open( tMessage, 1500, NULL);
		}
		break;
	case 3:
		{
			Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_ALREADY_JOINROOM ) );
			MessageBoxWnd.Open( tMessage, 1500, NULL);
		}
		break;
	}
}

void CRockClient::Proc_ScMsngrChatDestroyRoom( SRpScMsngrChatDestroyRoom * aMsg )
{
	RLG1( "SRpScMsngrChatDestroyRoom  %s", aMsg->theOwnerPCName );
	TCHAR tMessage[256]	= {0,};
	
	if(!g_MCommunityWnd.m_MessenMultiChatWnd.IsVisible)
	{
		return;
	}
	
	Rsprintf(RWCHAR(tMessage), G_STRING_CHAR( IDS_MSGBOX_REMOVE_ROOM ), RWCHAR(aMsg->theOwnerPCName) );	 
	MessageBoxWnd.Open( tMessage, 2000, NULL);
	g_MCommunityWnd.m_MessenMultiChatWnd.SetVisible(false);																						 
	
	/*g_MCommunityWnd.InsertMultiChatContent(true, 0 , 215, 5, NULL, tMessage);
	g_MCommunityWnd.m_MessenMultiChatWnd.SetActiveWnd(false);*/
}

void CRockClient::Proc_ScMsngrChatInviteAsk( SRpScMsngrChatInviteAsk * aMsg )
{
	RLG2( "SRpScMsngrChatInviteAsk  %s  %s", aMsg->theFromPCName, aMsg->theOwnerPCName );
	
	g_UIMsgWnd.SetParam( aMsg->theFromPCName );
	g_UIMsgWnd.SetSecondParam( aMsg->theOwnerPCName );
	
	g_UIMsgWnd.SetMsgType( MSG_MCHATINVITE );
	nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
}

void CRockClient::Proc_ScMsngrChatJoinRoom( SRpScMsngrChatJoinRoom * aMsg )
{
	int i = 0;
	SPcDataParam* PcParam = nRui->GetPcParamInfo();
	RLG1( "SRpScMsngrChatJoinRoom  %s  %s", aMsg->theOwnerPCName);
	for( i = 0 ; i < 6 ; i++ )
	{
		if(aMsg->theJoinedMemberPCName[i])
		{
			RLG1( "%s", aMsg->theJoinedMemberPCName[i]);
		}
	}
	
	if(g_MCommunityWnd.m_MessenMultiChatWnd.IsVisible)
	{
		return;
	}
	
	g_MCommunityWnd.m_MessenMultiChatWnd.SetVisible(true);
	g_MCommunityWnd.m_MessenMultiChatWnd.SetChatMasterName(aMsg->theOwnerPCName);
	
	for( i = 0 ; i < 6 ; i++ )
	{
		if(aMsg->theJoinedMemberPCName[i] && (Rstrcmp(aMsg->theJoinedMemberPCName[i], _RT("")) != 0) &&
			(Rstrcmp(aMsg->theJoinedMemberPCName[i], PcParam->PCName) != 0)	)
		{
			g_MCommunityWnd.m_MessenMultiChatWnd.InsertChatMemName(aMsg->theJoinedMemberPCName[i], true);
		}
	}
}

void CRockClient::Proc_ScMsngrChatSay( SRpScMsngrChatSay * aMsg )
{
	RLG2( "SRpScMsngrChatSay  %s  %s", aMsg->theFromPCName, aMsg->theStr );
	
	g_MCommunityWnd.m_MessenMultiChatWnd.InsertChatContent(aMsg->theFromPCName, aMsg->theStr);
}

void CRockClient::Proc_ScUpdateFame( SRpScUpdateFame * aMsg )
{
	//	DWORD			theFame; 
	
	
	
	
#ifndef NEW_PVP_DECO
	
	int fame = aMsg->theFame - nRui->GetPcParamInfo()->Fame;
	
	nRui->GetPcParamInfo()->Fame = aMsg->theFame;
	
	SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_FAME ), fame );
	
#else
	
	if( g_Pc.m_MainPC.lUnique == aMsg->thePCID)
	{
		g_Pc.m_MainPC.char_info.theFame =  aMsg->theFame;
		
		int fame = aMsg->theFame - nRui->GetPcParamInfo()->Fame;
		
		nRui->GetPcParamInfo()->Fame = aMsg->theFame;
		
		
		if( fame >= 0 )
		{
			SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_FAME ), fame );
		}
		else 
		{
			SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_FAME_WASTE ), fame );
		}
		
		
		return;
	}
	else 
	{		
		int tbl_index = g_Pc.FindUser( (long)aMsg->thePCID );
		
		if( tbl_index != MAX_USER_INTABLE )
		{
			
			//int fame = aMsg->theFame - g_Pc.m_PcTable[tbl_index].char_info.theFame ;
			
			g_Pc.m_PcTable[tbl_index].char_info.theFame = aMsg->theFame;
			
			//SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_FAME ), fame );
			
		}
		
	}
	
	
	
	
	
#endif
	
	
	
	
	return;
}
///---------------------------------------------------------------------------
///-- Proc_ScEldUpgrade
///---------------------------------------------------------------------------
void CRockClient::Proc_ScEldUpgrade( SRpScEldUpgrade * aMsg )
{
	nRui->thePcParam.Money = aMsg->theEld;
}


///---------------------------------------------------------------------------
///-- Proc_ScExpression
///---------------------------------------------------------------------------
void CRockClient::Proc_ScExpression(SRpScExpression *aMsg)
{
	if( g_Pc.GetPlayer()->GetUnique() == aMsg->thePCID )
	{
		SetAction( ( Character *) g_Pc.GetPlayer() , aMsg->theExpression , FALSE );
	}
	else
	{
		int Index = g_Pc.FindUser( aMsg->thePCID );
		
		if( Index == MAX_USER_INTABLE )
		{
			RLGS("Not Found Express Other Player\n");
			
			return;
		}
		
		SetAction( ( Character *) &g_Pc.m_Pc[ Index ] , aMsg->theExpression , FALSE );
	}
}

///---------------------------------------------------------------------------
///-- Proc_ScSaveLog
///---------------------------------------------------------------------------
void CRockClient::Proc_ScSaveLog(SRpScSaveLog *aMsg)
{
	int nLogFlag = aMsg->theSaveLogFlag;
	
	g_LogWnd.SetSavedLog( nLogFlag , aMsg->theFlag );
	g_LogWnd.InitComboTextList();
	
	if( (0 <= nLogFlag) && ( nLogFlag < 21 ) )
	{
		COMBO_LOG_DATA *pData = g_LogWnd.GetComboLogData( nLogFlag );
		
		if( NULL != pData && aMsg->theFlag )
		{
			SystemPrint( n_LightYellow, n_SystemMsg,
				G_STRING_CHAR( IDS_RECORD_COMBOLOG ), 
				(TCHAR *)(pData->StrSequence.c_str()) );	
		}	
		
		if( aMsg->theFlag )
		{
			int nComTotCount = g_LogWnd.GetComboTotCount( nLogFlag );
			int nSkillLevel = g_Pc.GetPlayer()->theActionSystem.GetEnableSkillLevel();
			
			TCHAR strString[512] = {0,};
			TCHAR strSkill[512] = {0,};
			
			if( nComTotCount > (nSkillLevel + 3) )
			{				
				nRui->m_Combo2DTimingEffect.GetNeedSkill(strSkill);
				
				if( strcmp(strSkill,"") !=0 )		
				{
					int nNeedLevel = nComTotCount - 3;
					Rsprintf( RWCHAR(strString), _RT("%s-[%s %d]"), G_STRING_CHAR( IDS_SYSPR_SHORT_COMBOLEVEL ), strSkill, nNeedLevel);
				}
				else
				{
					Rsprintf( RWCHAR(strString),_RT("%s"), G_STRING_CHAR( IDS_SYSPR_SHORT_COMBOLEVEL ));
				}
				
				MessageBoxWnd.Open( strString , 1000 , NULL );		
				return;
			}
			
			g_RockClient.Send_RpCsHotKey( 2 , ( WORD ) nLogFlag );
			g_LogWnd.SetComboHotKey( nLogFlag );		
		}
	}
	
	return;
}

// 대전장 정보 전송
void CRockClient::Proc_ScFightRoomList( SRpScFightRoomList* aMsg )
{
	//	struct _SFightRoom
	//	{
	//		DWORD	theRoomID;
	//		char	theName[40];
	//		WORD	theLock:1;			// 잠긴 방 여부 
	//		WORD	theGameType:6;		// 게임타입 
	//		WORD	theNum:4;			// 참가한 인원

	//		WORD	theMaxNum:4;		// 게임인원

	//		WORD	theStatus:1;				// 상태(준비, 게임)
	//	};
	
	//	WORD			theRoomNum;
	//	_SFightRoom		theRoomList[1000];
	//	WORD			theWaitingUserNum;
	//	char			theWaitingUserList[1000][RP_MAX_PC_NAME];

	nRui->m_bIsShowUI=true;

	if( g_RockClient.m_bIsDrawFullScreenUI == false )
	{
		LoadingBarWnd.m_IsMakeImg = true;
		LoadingBarWnd.m_pMsgScFightRoomList = SAFE_NEW( SRpScFightRoomList );	
		
		if( LoadingBarWnd.m_pMsgScFightRoomList )
		{
			//			*(LoadingBarWnd.m_pMsgScFightRoomList) = *(aMsg);
			//			memcpy( &LoadingBarWnd.m_pMsgScFightRoomList, &aMsg, sizeof( SRpScFightRoomList ) );
			
			LoadingBarWnd.m_pMsgScFightRoomList->theRoomNum =  aMsg->theRoomNum;
			LoadingBarWnd.m_pMsgScFightRoomList->theWaitingUserNum =  aMsg->theWaitingUserNum;
			
			int i = 0;
			
			BYTE* pBuf = (BYTE*) &( aMsg->theRoomList );
			
			if( aMsg->theRoomNum )
			{			
				SRpScFightRoomList::_SFightRoom*	pRoomList = NULL;					// struct 안에 정의된 _SFightRoom 는 쓸수 없음...Error...재생성
				pRoomList = SAFE_NEW_ARRAY( SRpScFightRoomList::_SFightRoom, aMsg->theRoomNum );				
				
				memcpy( pRoomList, &pBuf[0], sizeof( SRpScFightRoomList::_SFightRoom ) * aMsg->theRoomNum );
				
				for( i = 0; i < aMsg->theRoomNum; ++i )
				{					
					//					g_BZ_RoomListWnd.m_BZ_RoomInfoList.push_back( pRoomList[i] );
					LoadingBarWnd.m_pMsgScFightRoomList->theRoomList[i] = pRoomList[i];
				}		
				
				SAFE_DELETE_ARRAY( pRoomList );
			}
			
			if( aMsg->theWaitingUserNum )
			{
				int idx_buf = sizeof( SRpScFightRoomList::_SFightRoom ) * aMsg->theRoomNum;
				
				SBZ_UserList* pUserList = NULL;
				pUserList = SAFE_NEW_ARRAY( SBZ_UserList, aMsg->theWaitingUserNum );			
				
				memcpy( pUserList, &pBuf[idx_buf], sizeof( SBZ_UserList ) * aMsg->theWaitingUserNum );
				
				for( i = 0; i < aMsg->theWaitingUserNum; ++i )
				{
					//					g_BZ_RoomListWnd.m_BZ_UserNameList.push_back( pUserList[i] );
					Rsprintf( RWCHAR(LoadingBarWnd.m_pMsgScFightRoomList->theWaitingUserList[i]), _RT("%s"), RWCHAR(pUserList[i].UserName) );
				}
				
				SAFE_DELETE_ARRAY( pUserList );
			}
			
			
			
		}
		
		return;
	}
	else
	{
		g_BZ_RoomListWnd.Open();
		
		g_BZ_SelectZoneWnd.m_BZ_WndType = n_BattleZone_RoomListWnd;
		
		g_BZ_RoomListWnd.m_BZ_RoomInfoList.clear();
		g_BZ_RoomListWnd.m_BZ_UserNameList.clear();
		
		int i = 0;
		
		BYTE* pBuf = (BYTE*) &( aMsg->theRoomList );
		
		if( aMsg->theRoomNum )
		{			
			SRpScFightRoomList::_SFightRoom*	pRoomList = NULL;					// struct 안에 정의된 _SFightRoom 는 쓸수 없음...Error...재생성
			pRoomList = SAFE_NEW_ARRAY( SRpScFightRoomList::_SFightRoom , aMsg->theRoomNum );			
			
			memcpy( pRoomList, &pBuf[0], sizeof( SRpScFightRoomList::_SFightRoom ) * aMsg->theRoomNum );
			
			for( i = 0; i < aMsg->theRoomNum; ++i )
			{
				g_BZ_RoomListWnd.m_BZ_RoomInfoList.push_back( pRoomList[i] );
			}		
			
			SAFE_DELETE_ARRAY( pRoomList );
		}
		
		if( aMsg->theWaitingUserNum )
		{
			int idx_buf = sizeof( SRpScFightRoomList::_SFightRoom ) * aMsg->theRoomNum;
			
			SBZ_UserList* pUserList = NULL;
			pUserList = SAFE_NEW_ARRAY( SBZ_UserList, aMsg->theWaitingUserNum );		
			
			memcpy( pUserList, &pBuf[idx_buf], sizeof( SBZ_UserList ) * aMsg->theWaitingUserNum );
			
			for( i = 0; i < aMsg->theWaitingUserNum; ++i )
			{
				g_BZ_RoomListWnd.m_BZ_UserNameList.push_back( pUserList[i] );
			}
			
			SAFE_DELETE_ARRAY( pUserList );
		}
		
		g_BZ_RoomListWnd.SetRefreshRoomList();
		g_BZ_RoomListWnd.SetRefreshUserList();
		
		g_RockClient.Send_CsReadyFightSystem();
	}
	
	return;
}

void CRockClient::DelayProc_RpScFightRoomList( SRpScFightRoomList* aMsg )
{
	BeginLoadingBarThread();
	
	g_Pc.SetupTempPc( g_Pc.m_MainPC );
	g_Pc.m_TempPC.lCurMap = c_RockMap_Millena_Rain_PVP_Zone;
	g_Pc.m_TempPC.lBlock = g_Pc.m_MainPC.lDestBlock;
	g_Pc.m_TempPC.lDestBlock = g_Pc.m_MainPC.lDestBlock;
	
	LoadingBarWnd.Set_warpmap_id( c_RockMap_Millena_Rain_PVP_Zone );
	g_RockClient.MapChange( c_RockMap_Millena_Rain_PVP_Zone );
	
	g_Pc.GetPlayer()->SetWearItem();		
	
	if( NonPlayerInfoWnd.IsVisible )	
	{ 
		nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
	
	if( NpcChatWnd.IsVisible )
	{
		NpcChatWnd.CloseNpcChatWnd( true );
	}
	
	g_BZ_RoomListWnd.Open();
	
	g_BZ_SelectZoneWnd.m_BZ_WndType = n_BattleZone_RoomListWnd;
	
	g_BZ_RoomListWnd.m_BZ_RoomInfoList.clear();
	g_BZ_RoomListWnd.m_BZ_UserNameList.clear();
	
	int i = 0;
	
	//	BYTE* pBuf = (BYTE*) &( aMsg->theRoomList );
	
	if( aMsg->theRoomNum )
	{			
		for( i = 0; i < aMsg->theRoomNum; ++i )
		{
			//			g_BZ_RoomListWnd.m_BZ_RoomInfoList.push_back( pRoomList[i] );
			g_BZ_RoomListWnd.m_BZ_RoomInfoList.push_back( LoadingBarWnd.m_pMsgScFightRoomList->theRoomList[i] );
		}		
		
		//		SAFE_DELETE_ARRAY( pRoomList );
	}
	
	if( aMsg->theWaitingUserNum )
	{
		for( i = 0; i < aMsg->theWaitingUserNum; ++i )
		{
			//			g_BZ_RoomListWnd.m_BZ_UserNameList.push_back( pUserList[i] );
			
			SBZ_UserList UserList;
			Rsprintf( RWCHAR(UserList.UserName), _RT("%s"), RWCHAR(LoadingBarWnd.m_pMsgScFightRoomList->theWaitingUserList[i]) );
			
			g_BZ_RoomListWnd.m_BZ_UserNameList.push_back( UserList );
		}
		
		//		SAFE_DELETE_ARRAY( pUserList );
	}
	
	g_BZ_RoomListWnd.SetRefreshRoomList();
	g_BZ_RoomListWnd.SetRefreshUserList();
	
	// 맵 로딩후엔 다 쓴 메세지 없애기 
	SAFE_DELETE( LoadingBarWnd.m_pMsgScFightRoomList );		
	
	//	g_Pc.GetPlayer()->CM__READY__WARP( g_nowTime, n_FightZone );
	
	///-- 로딩바가 종료할때까지 기다려 안전적인 처리
	LoadingBarSafeStopAfterProcess();
	
	g_RockClient.Send_CsReadyFightSystem();
	
	return;
}

// 기존 대기자들에게 대기실 유저 추가 전송
void CRockClient::Proc_ScNewWaitingFighter( SRpScNewWaitingFighter* aMsg )
{
	//	char		thePCName[RP_MAX_PC_NAME];
	
	
	SBZ_UserList  SUserList;
	
	Rsprintf( RWCHAR(SUserList.UserName), _RT("%s"), RWCHAR(aMsg->thePCName) );
	
	g_BZ_RoomListWnd.m_BZ_UserNameList.push_back( SUserList );
	
	g_BZ_RoomListWnd.SetRefreshUserList();
	
	return;
}

// 대기자들에게 대기실 유저 삭제 전송
void CRockClient::Proc_ScDelWaitingFighter( SRpScDelWaitingFighter* aMsg )
{
	//	char		thePCName[RP_MAX_PC_NAME];
	
	
	std::vector < SBZ_UserList > :: iterator ir;							
	
	ir = g_BZ_RoomListWnd.m_BZ_UserNameList.begin();
	
	for( ; ir != g_BZ_RoomListWnd.m_BZ_UserNameList.end() ; ++ir )
	{
		if( Rstrcmp( RWCHAR((*ir).UserName), RWCHAR(aMsg->thePCName) ) == 0 )
		{
			g_BZ_RoomListWnd.m_BZ_UserNameList.erase( ir );
			break;
		}
	}	
	
	g_BZ_RoomListWnd.SetRefreshUserList();
				
	return;
}

// 방만들기 결과 전송
void CRockClient::Proc_ScMakeFightRoom( SRpScMakeFightRoom* aMsg )
{
	//	BYTE		theResult;
	//	DWORD		theRoomID;
	
	if( aMsg->theResult != n_Success )
	{
		return;
	}
	
	g_BZ_CreateRoomWnd.Close();
	
	nRui->SetModal( WID_None );
	
	g_BZ_SelectZoneWnd.m_BZ_WndType = n_BattleZone_WaitingRoomWnd;				
	
	g_BZ_WaitingRoomWnd.m_WaitingRoomID = aMsg->theRoomID;
	g_BZ_WaitingRoomWnd.m_MasterID = g_Pc.m_MainPC.lUnique;
	g_BZ_WaitingRoomWnd.m_bIsMaster = true;
	
#ifdef JUST_WATCH	
	
	g_BZ_WaitingRoomWnd.m_ZoneViewType = n_BattleZone_NormalZone;	
	
#endif
	
	g_BZ_WaitingRoomWnd.Open( true );
	g_BZ_WaitingRoomWnd.SetInitUserSlot();
	
	return;
}

// 대기실의 유저들에게 새로 만든 방 정보 전송
void CRockClient::Proc_ScSeeMakeFightRoom( SRpScSeeMakeFightRoom* aMsg )
{
	//	SRpScFightRoomList::_SFightRoom		theRoomInfo;
	
	
	SRpScFightRoomList::_SFightRoom		room_list;
	
	room_list.theGameType = aMsg->theRoomInfo.theGameType;
	room_list.theRoomID = aMsg->theRoomInfo.theRoomID;
	Rsprintf( RWCHAR(room_list.theName), _RT("%s"), RWCHAR(aMsg->theRoomInfo.theName) );
	room_list.theLock = aMsg->theRoomInfo.theLock;
	room_list.theGameType = aMsg->theRoomInfo.theGameType;
	room_list.theNum = aMsg->theRoomInfo.theNum;
	room_list.theMaxNum = aMsg->theRoomInfo.theMaxNum;
	room_list.theStatus = aMsg->theRoomInfo.theStatus;
	
	g_BZ_RoomListWnd.m_BZ_RoomInfoList.push_back( room_list );
	
	if( room_list.theStatus == 0 )
	{
		g_BZ_RoomListWnd.m_BZ_RoomInfoList_Waiting.push_back( room_list );
	}
	
	g_BZ_RoomListWnd.SetRefreshRoomList();
	
	return;
}

// 대기실의 유저들에게 지워진 방 정보 전송
void CRockClient::Proc_ScSeeDeleteFightRoom( SRpScSeeDeleteFightRoom* aMsg )
{
	//	DWORD			theRoomID;
	
	std::vector < SRpScFightRoomList::_SFightRoom > :: iterator ir;							
	
	ir = g_BZ_RoomListWnd.m_BZ_RoomInfoList.begin();
	
	for( ; ir != g_BZ_RoomListWnd.m_BZ_RoomInfoList.end() ; ++ir )
	{
		if( (*ir).theRoomID == aMsg->theRoomID ) 
		{
			g_BZ_RoomListWnd.m_BZ_RoomInfoList.erase( ir );
			break;
		}
	}	
	
	ir = g_BZ_RoomListWnd.m_BZ_RoomInfoList_Waiting.begin();
	
	for( ; ir != g_BZ_RoomListWnd.m_BZ_RoomInfoList_Waiting.end() ; ++ir )
	{
		if( (*ir).theRoomID == aMsg->theRoomID ) 
		{
			g_BZ_RoomListWnd.m_BZ_RoomInfoList_Waiting.erase( ir );
			break;
		}
	}	
	
	g_BZ_RoomListWnd.SetRefreshRoomList();
				
	return;
}


// 대전장의 정보 변경시 전송 ( 대전자수, 상태 ) 
void CRockClient::Proc_ScUpdateFightRoom( SRpScUpdateFightRoom* aMsg )
{
	//	WORD		theRoomID; 
	//	BYTE		theNum:4;				// 참가한 인원

	//	BYTE		Status:2;				// 상태(준비, 게임)
	//	char		theName[40];		// 방이름 	
	
	std::vector < SRpScFightRoomList::_SFightRoom > :: iterator ir;							
	
	ir = g_BZ_RoomListWnd.m_BZ_RoomInfoList.begin();
	
	for( ; ir != g_BZ_RoomListWnd.m_BZ_RoomInfoList.end() ; ++ir )
	{
		if( (*ir).theRoomID == aMsg->theRoomID ) 
		{
			(*ir).theNum = aMsg->theNum;
			(*ir).theStatus = aMsg->Status;			
			SAFE_STR_CPY( (*ir).theName, aMsg->theName, 40 );
			break;
		}
	}	
	
	ir = g_BZ_RoomListWnd.m_BZ_RoomInfoList_Waiting.begin();
	
	for( ; ir != g_BZ_RoomListWnd.m_BZ_RoomInfoList_Waiting.end() ; ++ir )
	{
		if( (*ir).theRoomID == aMsg->theRoomID ) 
		{
			(*ir).theNum = aMsg->theNum;
			(*ir).theStatus = aMsg->Status;			
			SAFE_STR_CPY( (*ir).theName, aMsg->theName, 40 );
			break;
		}
	}	
	
	g_BZ_RoomListWnd.SetRefreshRoomList();
				
	return;
}

// 참가하기 결과, 대전장의 상세정보 및 방주인정보 전송
void CRockClient::Proc_ScJoinFightRoom( SRpScJoinFightRoom* aMsg )
{
	//	BYTE			theResult;
	//	SFightRoomInfo	theInfo;
	//	DWORD			theRoomMasterID;
	
	if( aMsg->theResult != n_Success )
	{
		switch( aMsg->theResult )
		{
		case n_FightError_Password:
			{
				g_BZ_RoomListWnd.InsertBZHelpChatMsg( G_STRING_CHAR( IDS_GAME_INCORRECT_PASS ) );				
			}
			break;
		case n_FightError_NeedEld:
			{
				g_BZ_RoomListWnd.InsertBZHelpChatMsg( G_STRING_CHAR( IDS_SYSPR_FIGHTZONE_NEED_ELD ) );				
			}
			break;
		case n_NeedTicket:
			{
				g_BZ_RoomListWnd.InsertBZHelpChatMsg( G_STRING_CHAR( IDS_SYSPR_NEED_TICKET ) );
			}
			break;			
		}		
		return;
	}
	
	g_BZ_WaitingRoomWnd.m_RoomInfo = aMsg->theInfo;
	g_BZ_WaitingRoomWnd.m_MasterID = aMsg->theRoomMasterID;
	
#ifdef JUST_WATCH	
	if(aMsg->theWatcher)
	{
		g_BZ_WaitingRoomWnd.m_ZoneViewType = n_BattleZone_ViewZone;
	}
	else
	{
		g_BZ_WaitingRoomWnd.m_ZoneViewType = n_BattleZone_NormalZone;	
	}
#endif
	
	g_BZ_WaitingRoomWnd.Open( true );
	g_BZ_WaitingRoomWnd.SetInitUserSlot();
	
	return;
}

// 이미 참가된 유저들에게 새 참가자의 정보 전송. 이미 참가된 유저의 정보를 새 참가자에게 전송
void CRockClient::Proc_ScJoinedFighter( SRpScJoinedFighter* aMsg )
{
	//	BYTE			theNum;
	//	SRpdOtherPC		theFighterInfo[12];
	//	BYTE			theTeam[12];			// theFighterInfo의 순서의 팀정보 
	
	
	BYTE* pBuf = (BYTE*) &( aMsg->theTeam );
	
	BYTE* pTeamInfo = NULL;
	pTeamInfo = SAFE_NEW_ARRAY( BYTE, aMsg->theNum );
	
	memcpy( pTeamInfo, &pBuf[ 0 ], sizeof( BYTE ) * aMsg->theNum );
	
	int idx_buf = sizeof( BYTE ) * aMsg->theNum;
	
	SRpdOtherPC*	pPcInfo = NULL;					
	pPcInfo = SAFE_NEW_ARRAY( SRpdOtherPC, aMsg->theNum );		
	
	memcpy( pPcInfo, &pBuf[ idx_buf ], sizeof( SRpdOtherPC ) * aMsg->theNum );
	
	idx_buf += ( sizeof( SRpdOtherPC ) * aMsg->theNum );
	
	BYTE*	pPcState = NULL;					
	pPcState = SAFE_NEW_ARRAY( BYTE, aMsg->theNum );		
	
	memcpy( pPcState, &pBuf[ idx_buf ], sizeof( BYTE ) * aMsg->theNum );
	
	
	int i = 0;
	
	for( i = 0; i < aMsg->theNum; ++i )
	{		
		if( pPcState[ i ] == 1 )
		{
			g_BZ_WaitingRoomWnd.JoinedUserListSlot( pPcInfo[ i ], pTeamInfo[ i ], n_BZ_State_Ready );
		}
		else
		{
			g_BZ_WaitingRoomWnd.JoinedUserListSlot( pPcInfo[ i ], pTeamInfo[ i ], n_BZ_State_Wait );
		}
	}
	
	SAFE_DELETE_ARRAY( pTeamInfo ); 
	SAFE_DELETE_ARRAY( pPcInfo ); 
	
	return;
}

// 대전방 맴버들에게 전송(나가는 본인 포함)
void CRockClient::Proc_ScOutFightRoom( SRpScOutFightRoom* aMsg )
{
	//	DWORD		thePCID;
	
	if( g_Pc.m_MainPC.lUnique == aMsg->thePCID )
	{	
		// Proc_ScFightRoomList 전송 받는다. 
		
		return;
	}
	else
	{
		g_BZ_WaitingRoomWnd.OutUserListSlot( aMsg->thePCID );
	}
	
	return;
}

void CRockClient::Proc_ScChangeRoomMaster( SRpScChangeRoomMaster* aMsg )
{
	//	DWORD		theMasterID;
	
	
	g_BZ_WaitingRoomWnd.SetChangeRoomMaster( aMsg->theMasterID );
	
	return;
}

// 대전방 맴버들에게 전송 ( 대전방 정보 변경 ) 
void CRockClient::Proc_ScChangeFightInfo( SRpScChangeFightInfo* aMsg )
{
	//	BYTE		theMapCode;
	//	BYTE		theTimeOut;
	//	BYTE		theItemFlag;
	//	char		theName[40];		// 방이름 	
	
	g_BZ_WaitingRoomWnd.SetChangeRoomInfo( aMsg->theMapCode, aMsg->theTimeOut, aMsg->theItemFlag, aMsg->theName );
	
	return;
}

// 대전방 맴버들에게 팀변경 전송 
void CRockClient::Proc_ScSelectTeam( SRpScSelectTeam* aMsg )
{
	//	DWORD		thePCID;
	//	BYTE		theTeam;
	
	g_BZ_WaitingRoomWnd.SetChangeSelectTeam( aMsg->thePCID, aMsg->theTeam );
	
	return;
}

// 게임 준비 또는 취소 버튼 
void CRockClient::Proc_ScFightReady( SRpScFightReady* aMsg )
{
	//	DWORD		thePCID;	
	//	BYTE		theFlag;			// n_Success : 준비   n_Fail : 취소 
	
	
	g_BZ_WaitingRoomWnd.SetChangeFightReady( aMsg->thePCID, aMsg->theFlag );
	
	return;
}

// 대전방 게임 시작 
void CRockClient::Proc_ScFightStart( SRpScFightStart* aMsg )
{
	//	BYTE		theFlag;		// n_Success, NFightGameError 
	
	
	if( aMsg->theFlag != n_Success )
	{
		//		switch( aMsg->theFlag )
		//		{
		//		default:
		//			break;
		//		}
		return;
	}
	
	return;
}


// 실제 게임 시작 ( 맵 이동후 전체 로딩이 다 되었을때 시작!!! ) 
void CRockClient::Proc_ScScFightGo( SRpScFightGo* aMsg )
{
	// 이 프로토콜이 날아오기 전까지는 전부 대기 상태  
	//	char temp[128] = {0,};
	//	sprintf( temp, "대전 시작!!" );	
	//	g_Announcement.InsertAnnouncement( temp );
	
	nRui->m_BZoneIn2DEffect.SetReadyEffect( N2DEffect_Trans );
	nRui->m_BZoneIn2DEffect.SetStartEffect( N2DEffect_Trans );
	
	g_Pc.GetPlayer()->theFightZoneReady = true;
	
	if(!g_Pc.GetPlayer()->IsMyPlayerRender)
	{
		PCTABLE	*pPcTable = NULL;					
		pPcTable = g_Pc.GetNextPcTable(g_Pc.GetPlayer()->m_nViewRefDestIndex);
		
		if( pPcTable )
		{
			g_Camera.m_unique = pPcTable->lUnique;
		}
		else
		{
			pPcTable = g_Pc.GetNextPcTable(g_Pc.GetPlayer()->m_nViewRefDestIndex);
			g_Camera.m_unique = pPcTable->lUnique;
		}
		
		g_Camera.Update();
		g_Camera.MovCamera();
		g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
	}
	
	//아이템 딜레이 누적루틴 초기화
	g_SlotDelayProc.InitiallizeUseDelayTime();
	g_SlotDelayProc.InitiallizeSkillDelayTime();
	
	g_Particle.m_EffectManager.DeleteNow( ( Character * ) g_Pc.GetPlayer() );
	
	g_Particle.DelEffSetCon( g_Pc.m_MainPC.lUnique, g_Pc.GetPlayer()->GetIndex() );
	g_Particle.StopDisplay(  g_Pc.m_MainPC.lUnique , g_Pc.GetPlayer()->GetIndex() );
	g_Billboard.DelManager(  g_Pc.m_MainPC.lUnique , g_Pc.GetPlayer()->GetIndex() ); 
	
	return;
}

// 게임 결과 전송
void CRockClient::Proc_ScFightResult( SRpScFightResult* aMsg )
{
	//	DWORD		theWinTeam;			// 0 : 패, 1 : 승, 2 : 무승부, 서바이벌시 : 우승자 Unique ID 
	//	DWORD		theFame;			// 0 : 보상없음. 
	//	DWORD		theContribution;	// 0 : 보상없음. 
	//	DWORD		theExp;				// 0 : 보상없음. 
	//	DWORD		theEld;				// 0 : 보상없음. 
	//	WORD		theItemCode;		// 0 : 보상아이템 없음. 
	//	BYTE		theItemNum;
	
	
	// 게임 결과에 따른 UI or 이펙트 작업 
	//	char temp[128] = {0,};
	
	//	sprintf( temp, "^ ^)b : %d팀이 우승하셨습니다!!!", aMsg->theWinTeam + 1 );	
	//	g_Announcement.InsertAnnouncement( temp );
	
	//	g_Particle.m_EffectManager.DeleteAllKeepupSkillOnPlayer();
	
	DWORD Fame = 0;
	DWORD Contribution = 0;
	DWORD Exp = 0;				
	DWORD Eld = 0;	
	
	Fame = aMsg->theFame; // - nRui->GetPcParamInfo()->Fame;
	Contribution = aMsg->theContribution; // - nRui->GetPcParamInfo()->Contribution;
	Exp = aMsg->theExp; // - nRui->GetPcParamInfo()->Exp;
	Eld = aMsg->theEld; // - nRui->GetPcParamInfo()->Money;	
	
	//	nRui->GetPcParamInfo()->Fame = aMsg->theFame;
	//	nRui->GetPcParamInfo()->Contribution = aMsg->theContribution;
	//	nRui->GetPcParamInfo()->Exp = aMsg->theExp;
	//	nRui->GetPcParamInfo()->Money = aMsg->theEld; 	
	
	if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_Survival )
	{
		if( aMsg->theWinTeam == g_Pc.m_MainPC.lUnique ) 
		{
			g_BZ_GameResultWnd.Open( 1, Fame, Contribution, Exp, Eld, aMsg->theItemCode, aMsg->theItemNum );
		}
		else
		{
			g_BZ_GameResultWnd.Open( 0, Fame, Contribution, Exp, Eld, aMsg->theItemCode, aMsg->theItemNum );
		}	
	}
	else
	{
		if( aMsg->theWinTeam == g_Pc.m_MainPC.nFlag ) //	if( aMsg->theWinTeam == g_BZ_WaitingRoomWnd.m_WaitingUserInfo[m_SelfSlotIdx].FighterTeam )
		{
			g_BZ_GameResultWnd.Open( 1, Fame, Contribution, Exp, Eld, aMsg->theItemCode, aMsg->theItemNum );
		}
		else if( aMsg->theWinTeam == 2 )			// Draw 
		{
			g_BZ_GameResultWnd.Open( 2, Fame, Contribution, Exp, Eld, aMsg->theItemCode, aMsg->theItemNum );
		}
		else
		{
			g_BZ_GameResultWnd.Open( 0, Fame, Contribution, Exp, Eld, aMsg->theItemCode, aMsg->theItemNum );
		}	
	}
	
	return;
}


void CRockClient::Proc_ScFightBackRoom( SRpScFightBackRoom* aMsg )
{

	nRui->m_bIsShowUI=true;


	//스킬 및 아이템 딜레이 초기화 
    g_SlotDelayProc.InitiallizeUseDelayTime();
	g_SlotDelayProc.InitiallizeSkillDelayTime();
	
	//대기방에서 아무 행동도 하지 않는다 .. gg 
	//g_Pc.GetPlayer()->m_curt_event.type = 0;
	
	
	g_RockClient.m_bIsDrawFullScreenUI = true;
	
	g_BZ_WaitingRoomWnd.Open( false );
	
	g_Pc.GetPlayer()->m_nViewRefDestIndex = -1;
	g_Camera.m_unique = -1;
	g_Camera.Update();
	g_Camera.MovCamera();
	g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
	
	return;
}


// 국가 공헌도 전송 
void CRockClient::Proc_ScUpdateContribution( SRpScUpdateContribution* aMsg )
{
	//	DWORD			theContribution;
	
	nRui->GetPcParamInfo()->Contribution = aMsg->theContribution;
	
	return;
}


void CRockClient::Proc_ScFightHistory( SRpScFightHistory* aMsg )
{
	//	DWORD		theLoseFCount;				// 패 
	//	DWORD		theWinFCount;				// 승
	//	DWORD		theTotalFCount;				// 전적

	//	BYTE		theWFightCountPerDay;		// 일일 경기수 
	
	// 하루 동안 보상 받을 수 있는 남은 경기수 = 80 - theWFightCountPerDay
	// 무승부 게임 = theTotalFCount - theWinFCount - theLoseFCount;
	
#ifndef NEW_PVP_DECO
	g_BZ_SelectZoneWnd.m_BZ_FightRecord.TotalCnt = aMsg->theTotalFCount;
	g_BZ_SelectZoneWnd.m_BZ_FightRecord.WinCnt = aMsg->theWinFCount;
	g_BZ_SelectZoneWnd.m_BZ_FightRecord.LoseCnt = aMsg->theLoseFCount;
	g_BZ_SelectZoneWnd.m_BZ_FightRecord.DrawCnt = aMsg->theTotalFCount - aMsg->theWinFCount - aMsg->theLoseFCount;
	g_BZ_SelectZoneWnd.m_BZ_FightRecord.FightCntPerDay = aMsg->theWFightCountPerDay;
	
	char str_temp[255] = {0,};
	
	//	sprintf( str_temp, "총전적 : 100승 / 100무 / 100패" );		// IDS_SYSPR_FIGHTRECORD	총전적 : %d승 / %d무 / %d패
	Rsprintf( RWCHAR(str_temp), G_STRING_CHAR( IDS_SYSPR_FIGHTRECORD ), g_BZ_SelectZoneWnd.m_BZ_FightRecord.WinCnt,
		g_BZ_SelectZoneWnd.m_BZ_FightRecord.DrawCnt,
		g_BZ_SelectZoneWnd.m_BZ_FightRecord.LoseCnt  );		
	g_BZ_WaitingRoomWnd.InsertBZHelpChatMsg( str_temp );		
	
	//	sprintf( str_temp, "당일 경기수 : 50 / 80" );				// IDS_SYSPR_FIGHTCNT		당일 경기수 : %d / %d 
	Rsprintf( RWCHAR(str_temp), G_STRING_CHAR( IDS_SYSPR_FIGHTCNT ), g_BZ_SelectZoneWnd.m_BZ_FightRecord.FightCntPerDay, 80 );	
	g_BZ_WaitingRoomWnd.InsertBZHelpChatMsg( str_temp );		
	
	
	if( g_BZ_WaitingRoomWnd.m_RoomInfo.theGameType == n_Survival )
	{
		g_BZ_WaitingRoomWnd.InsertBZHelpChatMsg( G_STRING_CHAR( IDS_SYSPR_GAMEDESC1_SURVIVAL ) );		
		g_BZ_WaitingRoomWnd.InsertBZHelpChatMsg( G_STRING_CHAR( IDS_SYSPR_GAMEDESC2_SURVIVAL ) );		
		//		sprintf( str_temp, "서바이벌 모드는 자신의 레벨에 두배에 해당하는 대전료를 지급하고 승리시 대전자들의 모든 대전료를 차지하게 됩니다." );	
		//		g_BZ_WaitingRoomWnd.InsertBZHelpChatMsg( str_temp );				
	}
#else
	nRui->thePcParam.WinFCount = aMsg->theWinFCount;
	nRui->thePcParam.TotalFCount = aMsg->theTotalFCount;
#endif
	
	return;
}


// 일반적으로는 ErrorCode에 따른 스트링을 출력해주시면 되는데요

// 등록된 ErrorCode가 없을때는.. ErrorStr을 출력해주시면 됩니다.
void CRockClient::Proc_ScErrorGame( SRpScErrorGame* aMsg )
{
	//	int			theErrorCode;
	//	char		theErrorStr[RP_MAX_ERROR_STRING];	// 가변 사이즈 전송 
	static char TempStr[RP_MAX_ERROR_STRING] = {0,};
	memset( TempStr, 0, RP_MAX_ERROR_STRING );

	int pHeaderSize = sizeof(aMsg->theMsgHeader) + 4;
	BYTE*pBuf  = (BYTE*)aMsg;
	int p = pHeaderSize; 
	memcpy( TempStr,  &pBuf[p] , (aMsg->theMsgHeader.theSize - pHeaderSize)  );
	
	
	switch( aMsg->theErrorCode )
	{
	case 0:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, "%s", TempStr );		
		}
		break;
	case n_OverTotalSkillPoint:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MAXSKILLPOINTOVER ) );		// 최대 스킬 포인트를 초과하였습니다.
		}
		break;
	case n_IndomitableEffect:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INDOMITABLE_EFFECT ) );		// 불굴의 의지로 부활하였습니다.
		}
		break;
	case n_NeedClassUp:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_CLASSUP ) );				// 레벨업을 하시려면 전직하셔야 합니다.
		}
		break;
	case n_FAIL_PCLOGIN:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_FAIL_PCLOGIN ) );				// 로그인 실패
		}
		break;
	case n_NeedCountForWarp:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_COUNTFORWARP ), TempStr );		// 이 맵은 최소 %s명의 파티원이 있어야 합니다.
		}
		break;
	case n_NotFightTime:
		{
			if( g_Pc.GetPlayer()->theMapType == n_FightZone )
			{
				if( g_BZ_RoomListWnd.IsVisible )
				{
					g_BZ_RoomListWnd.InsertBZHelpChatMsg( G_STRING_CHAR( IDS_SYSPR_NOTUSE_FIGHTZONE ) );
				}
				//				else if( g_BZ_WaitingRoomWnd.IsVisible )
				//				{
				//					g_BZ_WaitingRoomWnd.InsertBZHelpChatMsg( G_STRING_CHAR( IDS_SYSPR_NOTUSE_FIGHTZONE ) );
				//				}
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTUSE_FIGHTZONE ) );		// 대전장 사용시간이 아닙니다.
			}
		}
		break;
	case n_Limit_FightLevel:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTUSE_FIGHTZONE_NEEDLEVEL ) );		// 대전장은 레벨 21부터 입장이 가능합니다.
		}
		break;
	case n_OverFightCountPerDay:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MAX_PVPNUM_80 ) );		//-- 하루 PvP 제한횟수는 80입니다.
		}
		break;
	case n_NeedSameFrontierForWarp:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_IN_ONLYSAME_FRONTIER ) );		
		}
		break;
	case n_OverOpenSlot:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANOPEN_5CSLOT_MAX ) );	
		}
		break;
		//추가 ...
	case n_NeedSlotEmpty:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEEDSLOTEMPTY ) );	
		}
		break;
	case n_NotEnoughCP:
		{
			if(m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum])
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTENOUGHCP ),m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->Name );	
			}
		}
		break;
	case n_NeedCheckOutEld:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEEDCHECKOUTELD ) );	
		}
		break;
	case n_NotCheckInPet:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTCHECKINPET ) );	
			
		}
		break;
	case n_OverLimitPCCount:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_OVERPC_COUNT) );
		}
		break;
	case n_Is_AttackTime2:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ATTACKTIME2));
		}
		break;
	case n_NeedKey_Map53:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NEEDKEY_ARMIS));
		}
		break;
	case n_NeedKey_Map56:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NEEDKEY_MAP56));
		}
	case n_Full_Inven:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_GMMSG_INVENTORY_ENOUGH));
		}
		break;
	///--JAPAN_BUG_MODIFY	
	case n_Riding_CannotCallFrontierPet:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_CANNOTCALLFPETINRIDING) );
		}
		break;
	case n_OverPvPCount:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_CANNOTPVPOVERTEMNUM_DAY) );	
		}
		break;
	case n_NotPKSameParty:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_CANNOTPKSAMEPARTY) );
		}
		break;
	case n_NotPKVillage:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_CANNOTPKINTOWN) );	
		}
		break;
	case n_NotPKOtherCitizen:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_CANNOTPKOTHERCONTRY) );	
		}
		break;
	case n_CloseItemShop_ChaoUser:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_CANNOTSHOPFROMCAOTREE) );	
		}
		break;
	case n_NotPKONMode:
	case n_NotPKOFFMode:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_CANNOTPKDEFORATTMODE) );	
		}
		break;
	case n_PKPenalty_LevelLimit:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_CANNOTDESPKPOINTOVERTEN) );	
		}
		break;
	case n_Duplicate_CharacName:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSDUPLICATE_CHANAME) );	
		}
		break;
	case n_NotMaster:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSERRORNOTFRONMASTER) );	
		}
		break;
	case n_Duplicate_FrontierName:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSDUPLICATE_FRONNAME) );	
		}
		break;
	case n_NotEnter_PvPFiled:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSNOTCHANNELENTERINPVEP) );	
		}
		break;
	case n_Error_StopSay:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_STOP_SAY) );	
		}
		break;
	case n_NotEnterAlgor_LevelLimit:
		{
			SystemPrint( n_DarkRed, n_SystemMsg,  G_STRING_CHAR(IDS_SYSPR_CANNOTEN_ALGOR) );	
		}
		break;
	case n_NotEnterGoldo_LevelLimit:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_CANNOTEN_GOLDO) );	
		}
		break;
	case n_NotEnterPvPField_Occupation:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_OCCUEDNATION_CANNOTEN) );		
		}
		break;
	case n_CallPet_NotChangeFrontierName:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_CALLPET_NOTFRONTIERNAME) );		
		}
		break;
	case n_NotEnterPvPField_FieldTicket:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NEEDADMISSIONFORPVPFIELD) );
		}
		break;
	case n_NotPvPFieldOpenTime:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOTPVPFIELDOPENTIME) );
		}
		break;
	case n_ComebackHome:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_COMEBACKHOME) );
		}
		break;
	case n_NotPvPFieldChannel:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOTPVPFIELDCHANNEL) );
		}
		break;
	case n_NotEnterPvPInden:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTENTERPVPINDEN) );
		}
		break;
		
	case n_ReEquipOwnSkillItem:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_EQUIPOWNSKILLITEM ) );
		}			
		break;
	case n_Quest_TimeOver:
		{
			//%s 퀘스트는 시간이 만료되어 자동 종료되었습니다. : %s 는 퀘스트명
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_QUEST_TIMEOVER ) , TempStr );
		}
		break;
	case n_NeedKey:
		{
			//이 맵에 들어가기 위해서는 %s 아이템이 필요합니다.
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEED_KEY ) , TempStr );
		}
		break;
	case n_IndunLimit:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_INDUN_LIMIT ) );
		}
		break; 
	case n_DoubleUpgrade_Err:
		{
			int Code = atoi(TempStr); 
			SItemBaseInfo* Item_Info = NULL ; 
			Item_Info = g_Pc_Manager.GetItemBaseInfoPtr( Code);
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SCROLL_DOUBLEUPGRADE_ERR ) , Item_Info->theName  );//Item_Info->theName
		}
		break;
	case n_PayMovePremiumZone_Err:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PAYMOVEPREMIUMZONE_ERR ) );
		}
		break; 
	case n_PayMovePremiumZone_Err2:
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PAYMOVEPREMIUMZONE_ERR2 ) );
		}
		break; 
	default:
		break;
	}	return;
}
    
  

//..........................................................................................................
// 개인 상점 관련
//..........................................................................................................
void CRockClient::Proc_ScOpenPCShopWindow(SRpScOpenPCShopWindow *aMsg )
{
	switch( aMsg->theResult )
	{
	case n_CANNOT_OPENSHOP_MAP:
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_NOPLACE ) );		// 개인 상점을 열수없는 장소입니다.
		
		// 상점 중단후 다시 상점스킬을 보냈을때 실패하면 팔기창 닫기

		if( g_UserStoreSellWnd.IsVisible )
		{
			g_UserStoreSellWnd.Close();
		}
		
		break;
		
		// 개인 상점창 열기

	case n_Success:
		
		// 상점 중단후 다시 상점스킬을 보냈을때 성공시
		if( g_UserStoreSellWnd.IsVisible )
		{
			POINT pt;
			SRect RStore;
			SRect RInven;
			
			g_UserStoreSellWnd.m_pFrame->GetClientSize( &RStore );
			InventoryWnd.FrameWnd->GetClientSize( &RInven );
			
			pt.x = ( 1024 - ( RStore.w + RInven.w ) ) / 2;
			pt.y = ( 768 -  RStore.h ) / 2;
			
			
			g_UserStoreSellWnd.m_pFrame->Reposition( pt.x , pt.y );
			
			pt.x += RStore.w;
			
			InventoryWnd.FrameWnd->Reposition( pt.x , pt.y );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
			InventoryWnd.Open();
#else
			nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
			
		}
		else if( !g_UserStoreSellWnd.ISUserSellMode() )
		{
			g_UserStoreTitleWnd.Open();
			
			g_UserStoreSellWnd.SetUserSellMode( TRUE );
		}
		
		
		break;
	}
}


// 상점 열기

void CRockClient::Proc_ScOpenPCShop(SRpScOpenPCShop *aMsg)
{
	switch( aMsg->theResult )
	{
	case n_Success:
		{
			EVENT_DATA_INFO event_store;
			
			event_store.type   = SM_USE_SKILL;
			event_store.status = SM_SELLSKILL_SITDOWN;
			event_store.motion = 0;			
			
			g_Pc.GetPlayer()->PushQueue( event_store, TRUE );
			
			char TempStr[ 256 ] = "";
			
			g_UserStoreSellWnd.m_pStoreNameText->GetText( TempStr , 41 );
			
			
			Rsprintf(RWCHAR(m_strTemp), _RT("%s"), RWCHAR(TempStr));										
			SetPlayerMessage( m_strTemp );
			
			g_UserStoreSellWnd.m_UserStoreSellOpenMode = TRUE;
		}
		break;
	}
}

// 다른 사람 상점 열기

void CRockClient::Proc_ScSeeOpenPCShop(SRpScSeeOpenPCShop *aMsg)
{
	int Index = 0;
	
	Index = g_Pc.FindUser(aMsg->thePCID);
				
	if( Index == MAX_USER_INTABLE )
		return;
	
	EVENT_DATA_INFO event_store;
				
	event_store.type   = SM_USE_SKILL;
	event_store.status = SM_SELLSKILL_SITDOWN;
	event_store.motion = 0;			
				
	g_Pc.m_Pc[ Index ].PushQueue( event_store, TRUE );	
	
	Rsprintf(RWCHAR(m_strTemp), _RT("%s"), RWCHAR(aMsg->theShopName));											
	SetNonPlayerMessage( aMsg->thePCID , m_strTemp );
	
}

// 개인 상점 닫기

void CRockClient::Proc_ScSeeClosePCShop(SRpScSeeClosePCShop *aMsg)
{
	// 자기 상점 물건 다팔렸을때
	if( aMsg->thePCID == g_Pc.GetPlayer()->GetUnique() )
	{
		EVENT_DATA_INFO event_store;
		
		event_store.type   = SM_USE_SKILL;
		event_store.status = SM_SELLSKILL_STANDUP;
		event_store.motion = 0;			
		
		
		g_UserStoreSellWnd.Close();
		
		g_Pc.GetPlayer()->PushQueue( event_store, TRUE );
		nRui->DeleteDlgBalloon( enPLAYER , g_Pc.GetPlayer()->GetUnique() );
	}
	// 다른 사람 상점 닫기

	else
	{
		int Index = 0;
		
		Index = g_Pc.FindUser(aMsg->thePCID);
		
		if( Index == MAX_USER_INTABLE )
			return;
		
		EVENT_DATA_INFO event_store;
		
		event_store.type   = SM_USE_SKILL;
		event_store.status = SM_SELLSKILL_STANDUP;
		event_store.motion = 0;			
		
		g_Pc.m_Pc[ Index ].PushQueue( event_store, TRUE );
		nRui->DeleteDlgBalloon( enNPC  , aMsg->thePCID );
		
		if( g_UserStoreBuyWnd.m_ShopPCID == aMsg->thePCID )
		{
			g_UserStoreBuyWnd.Close();
			g_UserStoreSetBuyWnd.Close();
		}
	}
	
}

// 개인 상점 보기

void CRockClient::Proc_ScInPCShop(SRpScInPCShop *aMsg)
{
	DWORD *pPriceList = NULL;
	
	pPriceList = ( DWORD * )( ( ( char *)&aMsg->theShopItemList ) + ( sizeof( URpdItem ) * aMsg->theShopItemNum ) );
	
	g_UserStoreBuyWnd.SetItemList( aMsg->theShopName,
		aMsg->theShopDesc,
		aMsg->theShopItemNum,
		aMsg->theShopItemList,
		pPriceList );
	
	g_UserStoreBuyWnd.UpdateSlot();
	g_UserStoreBuyWnd.Open();
	
	POINT pt;
	SRect RStore;
	SRect RInven;
	
	g_UserStoreBuyWnd.m_pFrame->GetClientSize( &RStore );
	InventoryWnd.FrameWnd->GetClientSize( &RInven );
	
	pt.x = ( 1024 - ( RStore.w + RInven.w ) ) / 2;
	pt.y = ( 768 -  RStore.h ) / 2;
	
	
	g_UserStoreBuyWnd.m_pFrame->Reposition( pt.x , pt.y );
	
	pt.x += RStore.w;
	
	InventoryWnd.FrameWnd->Reposition( pt.x , pt.y );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장			
	InventoryWnd.Open();
#else
	nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#endif
}

// 상점에서 물건 사기

void CRockClient::Proc_ScBuyInPCShop(SRpScBuyInPCShop *aMsg)
{
	SPcItem*	tmpPcItem = NULL;
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		//커서에 있는 Item 지우기....
		nRui->ResetCursorItem();
		
		//아이템 효과 적용....
		m_PcParam->Money = aMsg->theEld;
		
		// SPcItem객체생성
		tmpPcItem = SAFE_NEW( SPcItem );
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->theItem.theItem.theCode );
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
		{
			tmpPcItem->IType		= n_ITYPE_WEAR;
			tmpPcItem->Amount		= 1;
			
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			tmpPcItem->IType	= n_ITYPE_USE;//소모품
			tmpPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			tmpPcItem->IType	= n_ITYPE_UNUSE;//비소모품
			tmpPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
		{
			tmpPcItem->IType	= n_ITYPE_SPECIAL; 
			tmpPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_PersonalPet )
		{
			tmpPcItem->IType	= n_ITYPE_PET_USE; 
			tmpPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			tmpPcItem->IType	= n_ITYPE_RIDE; 
			tmpPcItem->Amount	= 1;
			
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{
			tmpPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			tmpPcItem->Amount	= 1;
			
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );
		}		
		else if( ItemType == n_Class_Skin )
		{
			tmpPcItem->IType	= n_ITYPE_SKIN_WEAR;
			tmpPcItem->Amount	= 1;
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theItem) , sizeof( URpdItem ) );
		}


		tmpPcItem->Class		= ItemType;
		tmpPcItem->ItemUID		= aMsg->theItem.theItem.theItemUID;
		tmpPcItem->Code			= aMsg->theItem.theItem.theCode;
		tmpPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theItem.theItem.theCode );
		tmpPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItem.theItem.theCode );
		
		if( tmpPcItem->ItemTable == NULL )
		{
			SAFE_DELETE( tmpPcItem );	
			return;
		}

		////-- 현재 아이템에 관련된 딜레이가 적용되었는지 체크해서 넣어준다.					
		if( tmpPcItem->IType == n_ITYPE_USE && tmpPcItem->ItemTable )
		{
			 int iDelayGroup = tmpPcItem->ItemTable->theDelayGroup;
			 if( !(nRui->enableDelayTime[iDelayGroup] ) )						
			 {
				tmpPcItem->IsDelay = true;						
			 }
		}		
		
		// 왼손,오른손....
		//---------------------------------------------------------------------
		if( ( tmpPcItem->ItemTable->theType == n_Type_MagicBook ) || ( tmpPcItem->ItemTable->theType == n_Type_Bow ) || ( tmpPcItem->ItemTable->theType == n_Type_Shield ) )
		{
			tmpPcItem->Class = n_Class_Shield;
		}
		else if( ( tmpPcItem->ItemTable->theType == n_Type_Sword ) || ( tmpPcItem->ItemTable->theType == n_Type_TwohandSword ) ||
			( tmpPcItem->ItemTable->theType == n_Type_Staff ) || ( tmpPcItem->ItemTable->theType == n_Type_CrystalBall ) )
		{
			tmpPcItem->Class = n_Class_Weapon;
		}
		//---------------------------------------------------------------------	

		SAFE_DELETE( m_PcInven->InvenItem[aMsg->theItem.theItem.thePosition] );		
		m_PcInven->InvenItem[aMsg->theItem.theItem.thePosition] = tmpPcItem;		
		
		for( int i = 0; i < RP_MAX_QUICK_SLOT; ++i )
		{
			if( ( m_PcInven->QuickSlot[i] != NULL ) && ( m_PcInven->QuickSlot[i]->ItemUID == tmpPcItem->ItemUID ) )
			{
				m_PcInven->QuickSlot[i] = tmpPcItem;				
				break;
			}
		}
		
		//슬롯 재설정....
		MainMenuWnd.UpdateQuickSlotItem();		
		InventoryWnd.UpdateInvenSlotItem();
	}
	
	// 열고 있는 상점의 아이템 변경 수정
	{
		g_UserStoreBuyWnd.ChangeItem( aMsg->theShopItemUID , aMsg->theShopBundle );
	}
	return;
}

// 자기 상점 물건 변경

void CRockClient::Proc_ScBuyInMyShop (SRpScBuyInMyShop *aMsg)
{
	DWORD SellValue = aMsg->theEld - m_PcParam->Money;
	
	m_PcParam->Money = aMsg->theEld;
	
	// 이전 번들갯수와 차이값
	int SubValue = 0;
	
	g_UserStoreSellWnd.SellInMyStore( aMsg->theCustomerID , SellValue , aMsg->theShopItemUID ,
		aMsg->theShopBundle , &SubValue  );
	
	
	// 인벤토리 처리
	int i = 0;
	
	for( i = 0; i < RP_MAX_EQUIP; ++i )
	{
		if( ( m_PcInven->WearItem[i] != NULL ) && ( m_PcInven->WearItem[i]->ItemUID == aMsg->theShopItemUID ) )
		{
			delete( m_PcInven->WearItem[i] );
			m_PcInven->WearItem[i] = NULL;
			
			//캐릭터에 실제 적용....
			g_Pc.GetPlayer()->Item_TakeOff( i );
			
			InventoryWnd.UpdateWearSlotItem();
			
			return;
		}
	}
	
	for( i = 0; i < c_MaxInvenItemNum; ++i )
	{
		if( ( m_PcInven->InvenItem[i] != NULL ) && ( m_PcInven->InvenItem[i]->ItemUID == aMsg->theShopItemUID ) )
		{
			//커서에 있는 Item 지우기....
			nRui->ResetCursorItem();
			
			//판매후 남은 번들 수. 0 인 경우 아이템 삭제.
			m_PcInven->InvenItem[i]->Amount = m_PcInven->InvenItem[i]->Amount - SubValue;
			
			if( m_PcInven->InvenItem[i]->Amount == 0 )
			{
				//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
				for( int j = 0; j < RP_MAX_QUICK_SLOT; ++j )
				{
					if( m_PcInven->InvenItem[i] == m_PcInven->QuickSlot[j] )
					{
						m_PcInven->QuickSlot[j] = NULL;
						break;
					}
				}
				
				delete( m_PcInven->InvenItem[i] );
				m_PcInven->InvenItem[i] = NULL;
			}
			
			//슬롯에 적용....
			MainMenuWnd.UpdateQuickSlotItem();
			InventoryWnd.UpdateInvenSlotItem();	
			
			return;
		}
	}
}


// 구매한 유료아이템 리스트 열기 
void CRockClient::Proc_ScOpenPayShop( SRpScOpenPayShop *aMsg )
{
	//	typedef struct _SPayItem
	//	{
	//		DWORD	thePayItemID;
	//		WORD	thePayItemCode;
	//	} SPayItem;
	
	//	BYTE			theResult;			// n_Success, n_Fail
	//	SPayItem		thePayItem;			// 각각 아이템별로 정보를 전송한다. 
	
	if( aMsg->theResult == n_Success )
	{
		g_PayItemWnd.SetPayItemWndPcItem( aMsg->thePayItem );		
	}
	
	return;
}


// 구매한 유료아이템 인벤으로 옮기기 
void CRockClient::Proc_ScGetPayItem( SRpScGetPayItem *aMsg )
{	
	//	BYTE			theResult;		// NItemError 참조
	//	DWORD			thePayItemID;	// PayItemList중에서 제거될 아이템 
	//	URpdItem		theItem;		// 인벤에 옮겨진 아이템의 정보 
	
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		// PayItemWnd 에서 아이템 삭제 처리 
		g_PayItemWnd.DeletePcItem( aMsg->thePayItemID );	
	}
	
	return;
}


// 유료아이템 사용표시를 위한 미니아이콘 출력용

void CRockClient::Proc_ScUsePayItem( SRpScUsePayItem *aMsg )
{
	//typedef struct _SPayEffect
	//{
	//	WORD				theItemCode;	// 구매한 유료아이템의 코드

	//	DWORD				theTime;		// 남은 지속 시간
	//}SPayEffect, * SPayEffectPtr;
	
	//	SPayEffect	theInfo;
	//	DWORD			theEffectValue;			// 사용후 변경 EffectValue
	//	DWORD			theMaxEffectValue;
	
	
	SItemBaseInfo* item_Info = NULL;
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theInfo.theItemCode );	
	
	if( item_Info )
	{
		if( item_Info->theReHPSpeed || item_Info->theReSPSpeed || item_Info->theReMPSpeed || item_Info->theReGage )
		{
			for( int i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
			{
				if( nRui->GetPcInvenInfo()->PayEffect[i] == NULL )
				{
					// 아이템 사용 효과에 관한 표시 처리...어떻게??? 						
					// item_Info->theReHP; // 효과 타입 
					// item_Info->theReSP; // 효과 값 
					// item_Info->theReMP; // 효과 지속 시간, 0일 경우 지속 아이템이 아님 
					
					// WORD type = aInfo->theReHP;
					// WORD param1 = aInfo->theReSP;
					// WORD param2 = aInfo->theReMP;
					// BYTE day = aInfo->theReHPSpeed;
					// BYTE hour = aInfo->theReSPSpeed;
					// BYTE min = aInfo->theReMPSpeed;
					// BYTE sec = aInfo->theReGage;

#ifdef KEEPUP_PAYITEM_RENEWAL
						aMsg->theInfo.theTime = aMsg->theInfo.theTime;
#else
						aMsg->theInfo.theTime = aMsg->theInfo.theTime * 1000;
#endif 
					nRui->GetPcInvenInfo()->PayEffect[i] = SAFE_NEW( SPayEffect );					
					
					nRui->GetPcInvenInfo()->PayEffect[i]->theItemCode = aMsg->theInfo.theItemCode;
					nRui->GetPcInvenInfo()->PayEffect[i]->theTime = aMsg->theInfo.theTime;
					
					PlayerInfoWnd.SetKeepUpItemCode( i, aMsg->theInfo );
					
					break;
				}
			}
		}
		
		g_Pc.theSkillSystem.UseItemEffectValueMgr( g_Pc.m_MainPC.lUnique, n_PC, aMsg->theInfo.theItemCode, item_Info->theReHP, aMsg->theEffectValue, aMsg->theMaxEffectValue, FALSE );
	}
	
	/*	static int ls_PayItemID = 1;
	static int ls_PayItemCode = 11101;
	
	  ++ls_PayItemID;
	  ++ls_PayItemCode;
	  
		if( ls_PayItemCode > 11123 )
		{
		ls_PayItemCode = 11101;
		}
		
		  SRpScOpenPayShop tempMsg;
		  tempMsg.theResult = n_Success;
		  tempMsg.thePayItem.thePayItemID = ls_PayItemID;
		  tempMsg.thePayItem.thePayItemCode = ls_PayItemCode;
		  
			g_RockClient.Proc_ScOpenPayShop( &tempMsg );
	*/
	return;
}

void CRockClient::Proc_ScCheckPayTime( SRpScCheckPayTime *aMsg)
{
	/*
	BYTE			theEffectNum;
	SPayEffect		thePayEffectList[RP_MAX_PAY_CONTENTS];
	*/
	 
#ifdef C_TIME_SUIT_UPDATE
	int p = sizeof(aMsg->theMsgHeader) + sizeof(BYTE)+ sizeof(WORD);
	BYTE*pBuf  = (BYTE*)aMsg;

	SPayEffect		PayEffect[RP_MAX_PAY_CONTENTS];
	SPayTimeItem	PayTimeItem[RP_MAX_TIMELIMIT_PAY_CONTENTS];

	memset( PayEffect , 0 , sizeof(SPayEffect) * 30 );
	memset( PayTimeItem, 0 , sizeof(SPayTimeItem) * RP_MAX_TIMELIMIT_PAY_CONTENTS);

	if(aMsg->theEffectNum != 0)
	{
		memcpy( PayEffect ,  &pBuf[p] , sizeof(SPayEffect)*  aMsg->theEffectNum	);
	}
	p += sizeof(SPayEffect) * aMsg->theEffectNum;

	if( aMsg->thePayItemTimeNum != 0 )
	{
		memcpy( PayTimeItem ,  &pBuf[p] , sizeof(SPayTimeItem)*  aMsg->thePayItemTimeNum  );
	}

#else

	int size = aMsg->theEffectNum ; 
	SPayEffect PayEffect[RP_MAX_PAY_CONTENTS] ;
	memset( PayEffect , 0 , sizeof(SPayEffect) * 30 );
	memcpy( PayEffect , aMsg->thePayEffectList , size * sizeof(SPayEffect) );

#endif

	for( int i = 0 ; i < RP_MAX_PAY_CONTENTS ; ++i)
	{
		if(PayEffect[i].theItemCode != 0 )
		{
			for( int j = 0; j < RP_MAX_PAY_CONTENTS; ++j )
			{			
				if( nRui->GetPcInvenInfo()->PayEffect[j] != NULL)
				{
					if( nRui->GetPcInvenInfo()->PayEffect[j]->theItemCode == PayEffect[i].theItemCode )
					{
						//시간코드 를 추가한다. 
#ifdef KEEPUP_PAYITEM_RENEWAL
						nRui->GetPcInvenInfo()->PayEffect[j]->theTime = PayEffect[i].theTime;
#else
						PayEffect[i].theTime = PayEffect[i].theTime * 1000;						
						nRui->GetPcInvenInfo()->PayEffect[j]->theTime = PayEffect[i].theTime;
#endif

						PlayerInfoWnd.SetCheckPayTime(j , PayEffect[i] );				
										
					}
				}
			}
		}
	}

#ifdef C_TIME_SUIT_UPDATE	
	for(short i  = 0 ; i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i)
	{
		if(PayTimeItem[i].theItemCode != 0)
		{
			for(int j  = 0 ; j < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++j)
			{
				if(nRui->GetPcInvenInfo()->PayTimeItemList[j] != NULL)
				{
					if(nRui->GetPcInvenInfo()->PayTimeItemList[j]->theItemID == PayTimeItem[i].theItemID &&
					nRui->GetPcInvenInfo()->PayTimeItemList[j]->theItemCode == PayTimeItem[i].theItemCode	)
					{
						//이것이 1년 짜리두 있기 땜시 밀리세컨드는 뺀당... 
						nRui->GetPcInvenInfo()->PayTimeItemList[j]->theTime = PayTimeItem[i].theTime;
					}
				}
			}
		}
	}
#endif 

}


void CRockClient::Proc_ScEndPayItem( SRpScEndPayItem *aMsg )
{
	// 유료아이템 사용시간 종료 
	//	WORD			theItemCode;
	//	WORD			theEffectValue;			// 종료후 변경 EffectValue
	//	WORD			theMaxEffectValue;
	
	 
	SItemBaseInfo* item_Info = NULL;
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItemCode );	
	
#ifdef C_TIME_SUIT_UPDATE

	//시간제한이 있는 아이템으로 간주한다 	
	if(item_Info && item_Info->theRare == 255 && item_Info->theMaxEndurance == 0 )
	{

		if( nRui->thePcInven.WearItem[n_WearSlot_Riding] != NULL && 
		 	nRui->thePcInven.WearItem[n_WearSlot_Riding]->ItemUID == aMsg->theItemID &&
			g_Pc.GetPlayer()->m_State.bRide )
		{
			Send_CsRideOff();
		}

		g_ListTime.DeletePayItem(aMsg->theItemID);
		SRpScDrop Msg;
		Msg.theItemID = aMsg->theItemID;
		Msg.theItemTab = aMsg->theItemTab;
		Msg.theBundle = 0 ; 
		Msg.theProcess = n_BaseProcess;
		Msg.theProcessNPC = 0;
		Msg.theResult = 1; //성공 	
		Proc_RpScDrop( &Msg );
		//한번더 삭제한다. 
		InventoryWnd.FindDeleteItem(aMsg->theItemID);

		SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_OVER_TIME), item_Info->theName );	
#ifdef PREMIUMZONE_KEY
		if( item_Info->theCode == 21560 || item_Info->theCode == 21561 
			|| item_Info->theCode == 21562 || item_Info->theCode == 21563)
		{
			//g_DelayBarWnd.Clear();
			g_Pc.GetPlayer()->m_curt_event.Clear();
			g_Pc.GetPlayer()->m_SpellEffect.Clear();				
			nRui->SendEvent( WID_ChantHitBar_Wnd , n_emSetVisible, (EPARAM)false, 0 , 0 , 0 );
			g_Pc.GetPlayer()->m_curt_event.loop = false;	
			g_Pc.GetSkillSystem()->theUseSpellSkill = FALSE;
		}

#endif //PREMIUMZONE_KEY
		
	}
	else
#endif

	if( item_Info )
	{
		if( item_Info->theReHPSpeed || item_Info->theReSPSpeed || item_Info->theReMPSpeed || item_Info->theReGage) 
		{
			for( int i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
			{
				if( nRui->GetPcInvenInfo()->PayEffect[i] )
				{
					if( nRui->GetPcInvenInfo()->PayEffect[i]->theItemCode == aMsg->theItemCode )
					{
						PlayerInfoWnd.DeleteKeepUpItemCode( i );				
						
						SAFE_DELETE( nRui->GetPcInvenInfo()->PayEffect[i] );
						
						PlayerInfoWnd.SortKeepupItem();
						
						break;
					}
				}
			}
		}
		
		g_Pc.theSkillSystem.UseItemEffectValueMgr( g_Pc.m_MainPC.lUnique, n_PC, aMsg->theItemCode, item_Info->theReHP, aMsg->theEffectValue, aMsg->theMaxEffectValue, FALSE );
	}
	
	/*	static int ls_PayItemID = 1;
	static int ls_ItemID = 9991;
	static int ls_ItemCode = 11101;
	
	  ++ls_PayItemID;
	  ++ls_ItemID;
	  ++ls_ItemCode;
	  
		if( ls_ItemCode > 11123 )
		{
		ls_ItemCode = 11101;
		}
		
		  SRpScGetPayItem tempMsg;
		  tempMsg.theResult = n_Success;
		  tempMsg.thePayItemID = ls_PayItemID;
		  tempMsg.theItem.theItem.theItemUID = ls_ItemID;
		  tempMsg.theItem.theItem.theCode = ls_ItemCode;
		  tempMsg.theItem.theWasteItem.theBundle = 1;
		  tempMsg.theItem.theItem.thePosition = 0;	
		  
			g_RockClient.Proc_ScGetPayItem( &tempMsg );
	*/
	return;
}


void CRockClient::Proc_ScSeeChangeFace( SRpScSeeChangeFace *aMsg )
{
	/// 유료아이템으로 얼굴 및 헤어 바꾸기 
	// 본인 포함 가시범위 유저들에게 전송 
	//	DWORD			thePCID;
	//	WORD			theFaceCode;
	
	
	if( aMsg->thePCID == g_Pc.m_MainPC.lUnique )
	{
		g_Pc.GetPlayer()->ChangeFace( aMsg->theFaceCode );
		
		g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
			g_Pc.m_MainPC.char_info.race , g_lpDevice, false );				
		g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
			g_Pc.m_MainPC.char_info.race , g_lpDevice );
	}
	else
	{
		int userIdx = g_Pc.FindUser( aMsg->thePCID );
		
		if( userIdx == MAX_USER_INTABLE )
			return;
		
		g_Pc.m_Pc[ userIdx ].ChangeFace( aMsg->theFaceCode );

		//by simwoosung - 파티 멤버 이미지의 갱신
		if(g_PartyInforWnd.IsMemberParty())
		{
			SPARTYMEMBER_INFO *pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->thePCID);
			if(pMemberInfo && pMemberInfo->m_bIsViewRange && pMemberInfo->m_bIsSee)
			{
				g_PartyInforWnd.ChangePCharImage(aMsg->thePCID);			
			}
		}
	}
	
	return;
}

void CRockClient::Proc_ScSeeChangeHair( SRpScSeeChangeHair *aMsg )
{
	//	DWORD			thePCID;
	//	WORD			theHairCode;
	
	
	if( aMsg->thePCID == g_Pc.m_MainPC.lUnique )
	{
		g_Pc.GetPlayer()->ChangeHair( aMsg->theHairCode ); 
		
		g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
			g_Pc.m_MainPC.char_info.race , g_lpDevice, false );				
		g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
			g_Pc.m_MainPC.char_info.race , g_lpDevice );
	}
	else
	{
		int userIdx = g_Pc.FindUser( aMsg->thePCID );
		
		if( userIdx == MAX_USER_INTABLE )
			return;	
		
		g_Pc.m_Pc[ userIdx ].ChangeHair( aMsg->theHairCode );

		//by simwoosung - 파티 멤버 이미지의 갱신
		if(g_PartyInforWnd.IsMemberParty())
		{
			SPARTYMEMBER_INFO *pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->thePCID);
			if(pMemberInfo && pMemberInfo->m_bIsViewRange && pMemberInfo->m_bIsSee)
			{
				g_PartyInforWnd.ChangePCharImage(aMsg->thePCID);			
			}
		}
	}
	
	return;
}


void CRockClient::Proc_ScSeePayEffect( SRpScSeePayEffect *aMsg )
{
	//	DWORD			thePCID;	
	//	WORD			thePayItemCode;
	
	if( g_Pc.GetPlayer()->GetUnique() == aMsg->thePCID )
	{
		return;
	}	
	//...........................................................................................
	// 아이템 사용시 이펙트 효과 By wxywxy
	//...........................................................................................
	g_Particle.m_EffectManager.UseItemEffect( aMsg->thePCID , aMsg->thePayItemCode );
	//...........................................................................................
	
	return;
}

void CRockClient::Proc_ScUpdateLuck( SRpScUpdateLuck *aMsg )
{
	//	char			theLuck;
	
	nRui->GetPcParamInfo()->Luck = aMsg->theLuck;
	
	return;
}

//................................................................................................................
// 신수 관련 테스트 By wxywxy
//................................................................................................................
void	CRockClient::Proc_RpScAppearHoly( SRpScAppearNPC* aMsg )
{

	BYTE * pBuff = NULL;
	pBuff = (BYTE *)aMsg;

	int nPackSize = 0;
	
#ifdef PACKET_RESIZE
	
	nPackSize += sizeof( aMsg->theMsgHeader ) + sizeof( DWORD ) + sizeof( WORD ) + sizeof( DWORD ) + sizeof( DWORD );
	nPackSize += sizeof( BYTE ) + sizeof( WORD ) + sizeof( BYTE );

	memset( g_FrontierName, 0, sizeof( g_FrontierName ) );
	memset( g_FrontierPetName, 0, sizeof( g_FrontierPetName ) );

	DWORD aNPCID	 = aMsg->theNPCID;
	WORD  aNPCType	 = aMsg->theNPCType;	
	DWORD aCellIndex = aMsg->theCellIndex;
	DWORD aHP		 = aMsg->theHP;
	DWORD aSP = 0;
	BYTE  aAttackSpeed = aMsg->theAttackSpeed;
	WORD  aMoveSpeed =   aMsg->theMoveSpeed;

	SNPCBaseInfo* npc_Info = NULL;
	
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aNPCType ); 

	if( npc_Info == NULL )
		return;

	aSP = npc_Info->theMaxSP;	

	float fAngle = 0.0f;
	BYTE  aPriorityType = n_NONEIDType;
	DWORD aPriorityID = 0;	
	DWORD aFrontierID = 0;	

	if( aMsg->theFrontierFlag && !aMsg->thePriorityFlag )
	{
		memcpy( &aPriorityType, &pBuff[nPackSize], sizeof( BYTE ) );
		nPackSize += sizeof(BYTE);
		memcpy( &aPriorityID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);
		memcpy( &fAngle, &pBuff[nPackSize], sizeof( float ) );
		nPackSize += sizeof(float);	
		memcpy( &aFrontierID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);	
		memcpy( g_FrontierName, &pBuff[nPackSize], RP_MAX_FRONTIER_NAME );		
		nPackSize += RP_MAX_FRONTIER_NAME;
		memcpy( g_FrontierPetName, &pBuff[nPackSize], RP_MAX_FRONTIER_PET_NAME );		
		nPackSize += RP_MAX_FRONTIER_PET_NAME;	
	}

	if( aMsg->thePriorityFlag && !aMsg->theFrontierFlag )
	{
		memcpy( &aPriorityType, &pBuff[nPackSize], sizeof( BYTE ) );
		nPackSize += sizeof(BYTE);
		memcpy( &aPriorityID, &pBuff[nPackSize], sizeof( DWORD ) );
		nPackSize += sizeof(DWORD);
		memcpy( &fAngle, &pBuff[nPackSize], sizeof( float ) );
		nPackSize += sizeof(float);
	}

	int nKeepUpSkillNum = aMsg->theKeepupSkillNum;
	WORD * pKeepUpSkillList = NULL;

	if( nKeepUpSkillNum )
	{
		pKeepUpSkillList = SAFE_NEW_ARRAY( WORD , nKeepUpSkillNum );
		memset( pKeepUpSkillList, 0 , sizeof( WORD ) * nKeepUpSkillNum );
		memcpy( pKeepUpSkillList, &pBuff[nPackSize], sizeof( WORD ) * nKeepUpSkillNum );
		nPackSize += ( sizeof( WORD ) * nKeepUpSkillNum );	
	}	
	
	RLG4( "****Proc_RpScAppearHoly : npcID = [%u], npcType = %u, cellIndex = %u, HP = %d",
	aNPCID, aNPCType, aCellIndex, aHP );
	
	int	 npc_tbl_index = g_Map.FindNpc( (long)aNPCID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		npc_tbl_index = g_Map.SM__HOLY__APPEAR( aNPCType, aNPCID, aCellIndex, 
												aHP, aSP, 0 , fAngle , aPriorityType );
		
		if( npc_tbl_index == -1 )
		{
			SAFE_DELETE_ARRAY( pKeepUpSkillList );
			return;
		}
		
		g_Map.m_Npc[npc_tbl_index].thePath[0] = aCellIndex;
		
		for( int i = 0; i < nKeepUpSkillNum ; ++i )
		{
			if( pKeepUpSkillList[i] )
			{
				g_Map.m_NpcTable[npc_tbl_index].theKeepupSkill[i] = pKeepUpSkillList[i];
				
				///--JAPAN_BUG_MODIFY
				g_Map.m_Npc[npc_tbl_index].m_curt_event.dKeepSkillCode = pKeepUpSkillList[i];
				// 지속성 스킬 이펙트 추가 [상준]
				g_Particle.m_EffectManager.CreateKeepupSkill( pKeepUpSkillList[i] , &g_Map.m_Npc[npc_tbl_index] , NULL , FALSE );
			}
		}

		///--JAPAN_BUG_MODIFY
		g_Map.m_Npc[npc_tbl_index].m_curt_event.dKeepSkillCode = 0;
		
		// 선점 처리 By wxywxy
		g_Map.m_Npc[npc_tbl_index].SetPriority( n_NONE , -1 );
		
		//theAttackSpeed; theMoveSpeed;	
		g_Map.m_Npc[npc_tbl_index].m_Weapon_DelayTime = aAttackSpeed * 100; 
		
		// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
		SNPCBaseInfo*	npc_Info = NULL;
		
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[npc_tbl_index].code ); 
		if( npc_Info->theMoveSpeed )
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = ( (float)aMoveSpeed / npc_Info->theMoveSpeed );					
			g_Map.m_Npc[npc_tbl_index].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[npc_tbl_index].m_ani_percent );
		}
		else
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = 1.0f;
			g_Map.m_Npc[npc_tbl_index].m_move_percent = 1.0f;
		}
		
		g_Map.m_Npc[npc_tbl_index].m_State.nHolyState = nHolyState_Holy;
		g_Map.m_Npc[npc_tbl_index].m_State.HolyIndex  = MAX_USER_INTABLE;
		//.......................................................................................................
	}
	else
	{
		//지금 해당 NPC가 존재하고 있다면;;;
		NonPlayer *pNonPlaer = g_Map.GetMonster( (int)aNPCID );
		
		if(pNonPlaer)
		{
			///--JAPAN_BUG_MODIFY 
			///-- 해당 NPC가 죽어있으면;; 여기서는 죽는 애니메이션이 들어가기전에..
			///-- 리젠이 되어버릴수 있기 때문에...;;; 이런처리를 함 
			if( pNonPlaer->IsRealDie() )
			{
				//맵에서 삭제
				g_Map.SM__HOLY__DISAPPEAR( 0, aNPCID );			
				//다시 AppearNPC 호출

				Proc_RpScAppearNPC( aMsg );
				RLG1( " Update Monster(Disappear - Appear) %d", aNPCID );
				SAFE_DELETE_ARRAY( pKeepUpSkillList );
				return;				
			}	
			
			//.......................................................................................................
			
			// 정보 갱신 
			g_Map.m_NpcTable[npc_tbl_index].lBlock = aCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].lDestBlock = aCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].theHP = aHP;
			g_Map.m_NpcTable[npc_tbl_index].theRealHP = aHP;
			g_Map.m_NpcTable[npc_tbl_index].theSMP = aSP;
			g_Map.m_NpcTable[npc_tbl_index].theLevel = aPriorityType;
			
			if( g_Map.m_NpcTable[npc_tbl_index].lDestBlock != aCellIndex )
			{
				g_Map.m_Npc[npc_tbl_index].SetDirection( D3DXToRadian( fAngle ) );
			}
			
			g_Map.m_Npc[npc_tbl_index].m_State.nHolyState = nHolyState_Holy;
			g_Map.m_Npc[npc_tbl_index].m_State.HolyIndex  = MAX_USER_INTABLE;			
			//.......................................................................................................
					
		}		

		SAFE_DELETE_ARRAY( pKeepUpSkillList );
		return;		
	}
	
	if( nRui->GetPcInvenInfo()->FrontierMyInfo )
	{
		if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID == aFrontierID )
		{
			nRui->GetPcInvenInfo()->FrontierPetID = aNPCID;
		}
	}
	
	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
	//......................................................................................................
	npc_tbl_index = g_Map.FindNpc( (long)aNPCID );			// 위에서 새로 만들수 있어서 다시 검색
	
	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		
		SNPCBaseInfo* npc_Info = NULL;
		
		NPCTABLE *pTable = g_Map.GetNpcTable( (long)aNPCID );
		
		if( NULL != pTable )
		{
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
			
			if( NULL != npc_Info )
			{
				if( npc_Info->theRightEff != -1 )
				{
					g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff , ( Character * ) &g_Map.m_Npc[npc_tbl_index] , NULL , NULL );
				}
			}
		}
	}
	
	Rsprintf( RWCHAR(g_Map.m_NpcTable[ npc_tbl_index ].szName), _RT("%s"), RWCHAR(g_FrontierPetName) );
	Rsprintf( RWCHAR(g_Map.m_NpcTable[ npc_tbl_index ].theFrontierName), _RT("%s"), RWCHAR(g_FrontierName) );
	g_Map.m_NpcTable[ npc_tbl_index ].theFrontierID = aFrontierID;
	
	//	aMsg->theMoveSpeed = 200;
	
	npc_Info = NULL;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[ npc_tbl_index ].code ); 
	
	if( NULL != npc_Info )
	{
		g_Map.m_Npc[npc_tbl_index].m_ani_percent = ( (float)aMoveSpeed / npc_Info->theMoveSpeed );					
		g_Map.m_Npc[npc_tbl_index].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[npc_tbl_index].m_ani_percent );
	}

	SAFE_DELETE_ARRAY( pKeepUpSkillList );
	
	return;

#else	
	
	RLG4( "****Proc_RpScAppearHoly : npcID = [%u], npcType = %u, cellIndex = %u, HP = %d",
		aMsg->theNPCID, aMsg->theNPCType, aMsg->theCellIndex, aMsg->theHP );
	
	int	 npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		npc_tbl_index = g_Map.SM__HOLY__APPEAR( aMsg->theNPCType, aMsg->theNPCID, aMsg->theCellIndex, 
												aMsg->theHP, aMsg->theSP, 0 , aMsg->theAngle , aMsg->thePriorityType );
		
		if( npc_tbl_index == -1 )
			return;
		
		g_Map.m_Npc[npc_tbl_index].thePath[0] = aMsg->theCellIndex;
		
		for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
		{
			if( aMsg->theKeepupSkill[i] )
			{
				g_Map.m_NpcTable[npc_tbl_index].theKeepupSkill[i] = aMsg->theKeepupSkill[i];
				
				///--JAPAN_BUG_MODIFY
				g_Map.m_Npc[npc_tbl_index].m_curt_event.dKeepSkillCode = aMsg->theKeepupSkill[i];
				// 지속성 스킬 이펙트 추가 [상준]
				g_Particle.m_EffectManager.CreateKeepupSkill( aMsg->theKeepupSkill[i] , &g_Map.m_Npc[npc_tbl_index] , NULL , FALSE );
			}
		}

		///--JAPAN_BUG_MODIFY
		g_Map.m_Npc[npc_tbl_index].m_curt_event.dKeepSkillCode = 0;
		
		// 선점 처리 By wxywxy
		g_Map.m_Npc[npc_tbl_index].SetPriority( n_NONE , -1 );
		
		//theAttackSpeed; theMoveSpeed;	
		g_Map.m_Npc[npc_tbl_index].m_Weapon_DelayTime = aMsg->theAttackSpeed * 100; 
		
		// 몹 또는 상인 NPC 들에게 적용되는 스킬일 경우...
		SNPCBaseInfo*	npc_Info = NULL;
		
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[npc_tbl_index].code ); 
		if( npc_Info->theMoveSpeed )
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = ( (float)aMsg->theMoveSpeed / npc_Info->theMoveSpeed );					
			g_Map.m_Npc[npc_tbl_index].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[npc_tbl_index].m_ani_percent );
		}
		else
		{
			g_Map.m_Npc[npc_tbl_index].m_ani_percent = 1.0f;
			g_Map.m_Npc[npc_tbl_index].m_move_percent = 1.0f;
		}
		
		g_Map.m_Npc[npc_tbl_index].m_State.nHolyState = nHolyState_Holy;
		g_Map.m_Npc[npc_tbl_index].m_State.HolyIndex  = MAX_USER_INTABLE;
		//.......................................................................................................
	}
	else
	{
		//지금 해당 NPC가 존재하고 있다면;;;
		NonPlayer *pNonPlaer = g_Map.GetMonster( (int)aMsg->theNPCID );
		
		if(pNonPlaer)
		{
			///--JAPAN_BUG_MODIFY 
			///-- 해당 NPC가 죽어있으면;; 여기서는 죽는 애니메이션이 들어가기전에..
			///-- 리젠이 되어버릴수 있기 때문에...;;; 이런처리를 함 
			if( pNonPlaer->IsRealDie() )
			{
				//맵에서 삭제
				g_Map.SM__HOLY__DISAPPEAR( 0, aMsg->theNPCID );			
				//다시 AppearNPC 호출

				Proc_RpScAppearNPC( aMsg );
				RLG1( " Update Monster(Disappear - Appear) %d", aMsg->theNPCID );
				return;				
			}	
			
			//.......................................................................................................
			
			// 정보 갱신 
			g_Map.m_NpcTable[npc_tbl_index].lBlock = aMsg->theCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].lDestBlock = aMsg->theCellIndex;
			g_Map.m_NpcTable[npc_tbl_index].theHP = aMsg->theHP;
			g_Map.m_NpcTable[npc_tbl_index].theRealHP = aMsg->theHP;
			g_Map.m_NpcTable[npc_tbl_index].theSMP = aMsg->theSP;
			g_Map.m_NpcTable[npc_tbl_index].theLevel = aMsg->thePriorityType;
			
			if( g_Map.m_NpcTable[npc_tbl_index].lDestBlock != aMsg->theCellIndex )
			{
				g_Map.m_Npc[npc_tbl_index].SetDirection( D3DXToRadian( aMsg->theAngle ) );
			}
			
			g_Map.m_Npc[npc_tbl_index].m_State.nHolyState = nHolyState_Holy;
			g_Map.m_Npc[npc_tbl_index].m_State.HolyIndex  = MAX_USER_INTABLE;
			
			//.......................................................................................................
					
		}		
		
		return;		
	}
	
	if( nRui->GetPcInvenInfo()->FrontierMyInfo )
	{
		if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID == aMsg->theFrontierID )
		{
			nRui->GetPcInvenInfo()->FrontierPetID = aMsg->theNPCID;
		}
	}
	
	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
	//......................................................................................................
	npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID );			// 위에서 새로 만들수 있어서 다시 검색

	
	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		
		SNPCBaseInfo* npc_Info = NULL;
		
		NPCTABLE *pTable = g_Map.GetNpcTable( (long)aMsg->theNPCID );
		
		if( NULL != pTable )
		{
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
			
			if( NULL != npc_Info )
			{
				if( npc_Info->theRightEff != -1 )
				{
					g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff , ( Character * ) &g_Map.m_Npc[npc_tbl_index] , NULL , NULL );
				}
			}
		}
	}
	
	Rsprintf( RWCHAR(g_Map.m_NpcTable[ npc_tbl_index ].szName), _RT("%s"), RWCHAR(aMsg->theFrontierPetName) );
	Rsprintf( RWCHAR(g_Map.m_NpcTable[ npc_tbl_index ].theFrontierName), _RT("%s"), RWCHAR(aMsg->theFrontierName) );
	g_Map.m_NpcTable[ npc_tbl_index ].theFrontierID = aMsg->theFrontierID;
	
	//	aMsg->theMoveSpeed = 200;
	
	SNPCBaseInfo*	npc_Info = NULL;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[ npc_tbl_index ].code ); 
	
	if( NULL != npc_Info )
	{
		g_Map.m_Npc[npc_tbl_index].m_ani_percent = ( (float)aMsg->theMoveSpeed / npc_Info->theMoveSpeed );					
		g_Map.m_Npc[npc_tbl_index].m_move_percent = npc_Info->theMoveSpeed / ( npc_Info->theMoveSpeed * g_Map.m_Npc[npc_tbl_index].m_ani_percent );
	}
	
	return;

#endif
}


void	CRockClient::Proc_RpScDisappearHoly( SRpScDisappearNPC* aMsg )
{
	//	RLG1( "*****ScDisappearNPC : [%u]", aMsg->theNPCID );
	if( nRui->GetPcInvenInfo()->FrontierMyInfo )
	{
		if( nRui->GetPcInvenInfo()->FrontierPetID == aMsg->theNPCID )
		{
			nRui->GetPcInvenInfo()->FrontierPetID = 0;
		}
	}
	
	int npc_tbl_index = g_Map.FindNpc( (long)aMsg->theNPCID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		RLG1( " Nothing NPC ID - %d : SRpScNPCMove", aMsg->theNPCID );
		return;
	}
	
	//.......................................................................................................
	// 신수 상관 관계 설정
	//.......................................................................................................
	if( g_Map.m_Npc[npc_tbl_index].m_State.HolyIndex == MAX_USER_INTABLE )
	{
		g_Pc.GetPlayer()->m_State.nHolyState          = nHolyState_Normal;
		g_Pc.GetPlayer()->m_State.HolyIndex           = -1;
	}
	else
	{
		g_Pc.m_Pc[g_Map.m_Npc[npc_tbl_index].m_State.HolyIndex].m_State.nHolyState = nHolyState_Normal;
		g_Pc.m_Pc[g_Map.m_Npc[npc_tbl_index].m_State.HolyIndex].m_State.HolyIndex  = -1;
	}
	//.......................................................................................................
	
	
	//	g_Map.SM__NPC__DISAPPEAR( g_Map.m_NpcTable[npc_tbl_index].index, aMsg->theNPCID );
	g_Map.SM__HOLY__DISAPPEAR( 0, aMsg->theNPCID );
	
	
	if( NonPlayerInfoWnd.thePickedNpcID == npc_tbl_index )
	{
		NonPlayerInfoWnd.thePickedNpcID = -1;
		nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		
		if(g_NonPlayerInfoAdd2Wnd.IsVisible)
		{
			g_NonPlayerInfoAdd2Wnd.Close();
		}
	}					
	
	return;
}

bool CRockClient::GetPetProtocolResult(int aResult)
{
	if(aResult != n_Success)
	{		
		switch(aResult) 
		{
			
		case n_IsEmptyPetSlot:	//펫이 존재하지 않습니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_PET_ISEMPTYSLOT) );
			}
			break;
		case n_CannotCallPet:				// 소환 에 실패 하였습니다 다시 시도해 주세요 . 
			{			
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_PET_CANNOT_CALLPET) );
			}
			break;
		}
		
		return false;
	}
	return true;
	
	
}


bool CRockClient::GetFrontierProtocolResult( int aResult )
{
	
	if( aResult != n_Success )
	{				
		switch( aResult )
		{
		case n_Fail:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_REFUSE_INVITE_FRONTIER ) );
			}
			break;		
		case n_Error_NeedMinMember:					// 최소 3명이상의 멤버가 필요합니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEEDMINMEMBER ) );
			}
			break;
		case n_Error_MakeFrontier_Class:			// 마스터는 2차 전직 이상의 직업이 필요합니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MAKEFRONTIER_CLASS ) );
			}
			break;
		case n_Error_MakeFrontier_FindMember:		// NPC 주변에 멤버가 모여야 합니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MAKEFRONITER_FINDMEMBER ) );
			}
			break;
		case n_Error_MakeFrontier_Eld:				// 창단시 필요한 엘드가 부족합니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MAKEFRONTIER_ELD ) );
			}
			break;
		case n_Error_MakeFrontier_OtherFT:			// 이미 다른 프론티어의 맴버입니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MAKEFRONTIER_OTHERFT ) );
			}
			break;
		case n_Error_Proposed:						// 다른 프론티어의 제안받고 있는 중입니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PROPOSED ) );
			}
			break;
		case n_Error_OverMaxMember:					// 더 이상 멤버를 받을 수 없습니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERMAXMEMBER ) );
			}
			break;
		case n_Error_MasterAuthority:				// 마스터의 권한입니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MASTERAUTHORITY ) );
			}
			break;
		case n_Error_MajorAuthority:				// 메이저 이상급의 권한입니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MAJORAUTHORITY ) );
			}
			break;
		case n_Error_MemberAuthority:				// 멤버 이상급의 권한입니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_MEMBERAUTHORITY ) );
			}
			break;
		case n_Error_NotEnoughFrontierEld:			// 프론티어 엘드가 부족합니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTENOUGH_FRONTIERELD ) );
			}
			break;
		case n_Error_NotExistFrontierPet:			// 신수가 존재하지 않습니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTEXIST_FRONTIERPET ) );
			}
			break;
		case n_Error_AlreadyCalled:					// 신수가 이미 다른 맴버에 의해 호출되어 있습니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ALREADYCALLED ) );
			}
			break;
		case n_Error_NotMyFrontierMember:			// 같은 포론티어의 맴버가 아닌다. 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTMYFRONTIERMEMBER ) );
			}
			break;
		case n_Error_ShortFrontierEld:				// 프론티어 돈이 부족합니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTENOUGH_FRONTIERELD ) );
			}
			break;
		case n_Error_Need_10Member:					// 최소 10명의 멤버가 필요합니다.
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NEEDMIN10MEMBER ) );
			}
			break;
		case n_Error_Frontier_Citizen:				// 다른 국가는 같은 프론티어가 될 수 없습니다. 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_FAILOTHERCITIZEN ) );
			}
			break;
		case n_Error_Frontier_AlreadyHavePet:		// 이미 신수를 분양 받았습니다. 
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ALREADYHAVEPET ) );
			}
			break;

		case n_Error_FrontierMemberFull:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_FRONTIER_MEMBERFULL) );
			}
			break;

		case n_Error_AlreadyCombined:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, "상대 프론티어는 이미 다른 프론티어와 연합되어있습니다." );	///-- HAN_MSG
			}
			break;
		case n_Error_AlreadyCombine:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, "이미 다른 프론티어와 연합되어있습니다." );	///-- HAN_MSG
			}
			break;
		case n_Error_NotJoinedFrontier:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, "프론티어 가입자가 아닙니다." );	///-- HAN_MSG
			}
			break;
		case n_Error_NotMaster:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, "마스터가 아닙니다." );			///-- HAN_MSG
			}
			break;
		case n_Error_AnotherProcessing:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, "다른 작업중입니다. 잠시후에 다시 시도해주십시오." );			///-- HAN_MSG
			}
			break;
		case n_Error_NoCombinedFrontier:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, "연합된 프론티어가 없습니다." );	///-- HAN_MSG
			}
			break;
		case n_Error_DiedFrontierPet:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_HOLLY_ISDIE) );	///-- HAN_MSG
			}
			break;
		case n_Error_NeedCallFrontierPet:
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_HOLLY_MUSTCALL) );	///-- HAN_MSG
			}
			break;
		}
		return false;
	}	
	return true;
}

// 창단 신청에 대한 결과 
void CRockClient::Proc_RpScAnsMakeFrontier( SRpScAnsMakeFrontier* aMsg )
{
	//	BYTE				theResult;		// NFrontierError 참조 
	if( GetFrontierProtocolResult( aMsg->theResult ) )
	{
	}
}


// 프론티어 정식 인증 : 프론티어 맴버 및 그들의 가시범위 유저들에게 방송 
void CRockClient::Proc_RpScMsngrAuthorizeFrontier( SRpScMsngrAuthorizeFrontier* aMsg )
{
	/*
	enum NFrontierStatus
	{
	n_Preliminary_Frontier,		// 예비

	n_Authorized_Frontier,		// 공식 
	};
	*/
	if( GetFrontierProtocolResult( aMsg->theResult ) )
	{
		nRui->GetPcInvenInfo()->FrontierInfo->theStatus = n_Authorized_Frontier;
		nRui->GetPcInvenInfo()->FrontierInfo->theDues = aMsg->theFrontierDues;
		
		g_FrontierMainWnd.RefreshFrontierMyInfo();
		g_FrontierMainWnd.RefreshFrontierMyFrontier();
		g_FrontierMemberListWnd.RefreshMemberInfoList();
		
		char strTemp[255] = {0,};
		Rsprintf( RWCHAR(strTemp), _RT("%s##%s"), G_STRING_CHAR( IDS_MSGBOX_DESC_CONGRATULATIONS ), G_STRING_CHAR( IDS_MSGBOX_DESC_APPROVALFRONTIER ) );
		g_MessageBox.OpenMessageBox( n_MESSAGEBOX_NONE, strTemp, 250, 130, G_STRING_CHAR( IDS_WND_APPROVAL ), n_MB_OK );
	}
}


// 가입제안 받은 것을 클라이언트에 알려줌 
void CRockClient::Proc_RpScMsngrProposeJoinFrontier( SRpScMsngrProposeJoinFrontier* aMsg )
{
	//	DWORD		theProposerID;		// 제안하는 유저(Master or Major)
	

	///-- 가시범위 내에 존재하는지 여부 확인
	PCTABLE *pPcTable = g_Pc.FindPcTable(aMsg->theProposerID);
	if(!pPcTable)
	{
		return;
	}	

#ifdef DECO_RENEWAL_GUILD_MJH
	SPcItem* addPcItem = NULL;
		
	addPcItem = &g_UIMsgWnd.m_pPcItem;

	if( aMsg->theItem.theItem.theCode = 0 )
	{
		return;
	} 

	WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->theItem.theItem.theCode );
	if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
	{
		addPcItem->IType		= n_ITYPE_WEAR;
		addPcItem->Amount		= 1;
		
		memcpy( &(addPcItem->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );
	}
	else if( ItemType == n_Class_Use )
	{
		addPcItem->IType	= n_ITYPE_USE;//소모품
		addPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
	}
	else if( ItemType == n_Class_Useless )
	{
		addPcItem->IType	= n_ITYPE_UNUSE;//비소모품
		addPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
	}
	else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
		|| ItemType == n_Class_FrontierPet ||  
		ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
		{
		addPcItem->IType	= n_ITYPE_SPECIAL; 
		addPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
	}		
	else if( ItemType == n_Class_PersonalPet )
	{
		addPcItem->IType	= n_ITYPE_PET_USE; 
		addPcItem->Amount	= aMsg->theItem.theWasteItem.theBundle;
	}
	else if( ItemType == n_Class_Riding )
	{
		addPcItem->IType	= n_ITYPE_RIDE; 
		addPcItem->Amount	= 1;
		
		memcpy( &(addPcItem->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );
	}
	else if( ItemType == n_Class_RidingItem )
	{
		addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
		addPcItem->Amount	= 1;
		
		memcpy( &(addPcItem->WearTable), &(aMsg->theItem), sizeof( URpdItem ) );
	}
	else if( ItemType == n_Class_Skin )
	{
		addPcItem->IType = n_ITYPE_SKIN_WEAR;
		addPcItem->Amount	= 1;
		memcpy( &(addPcItem->WearTable), &(aMsg->theItem) , sizeof( URpdItem ) );
	}
		
	addPcItem->Class		= ItemType;
	addPcItem->ItemUID		= aMsg->theItem.theItem.theItemUID;
	addPcItem->Code			= aMsg->theItem.theItem.theCode;
	addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theItem.theItem.theCode );
	addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItem.theItem.theCode );

	if( addPcItem->ItemTable == NULL)
	{
		return;
	}
	// 왼손,오른손....
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
	g_UIMsgWnd.UpdateItemSlot();

#endif // DECO_RENEWAL_GUILD_MJH

	g_UIMsgWnd.SetParam( pPcTable->char_info.thePCName );
	g_UIMsgWnd.SetMsgType( MSG_FRONTIERINVITE );
	nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
}

// 가입제안에 대한 답변을 제안자에게 전송 
void CRockClient::Proc_RpScMsngrAnswerJoinFrontier( SRpScMsngrAnswerJoinFrontier* aMsg )
{
	//	BYTE		theAnswer;			// n_Success(승낙), n_Fail(거부), 그외 N_FrontierError 참조 
	
	if( GetFrontierProtocolResult( aMsg->theAnswer ) )
	{
	
	}	
}

// 가입한 프론티어의 정보 
void CRockClient::Proc_RpScMsngrJoinFrontier( SRpScMsngrJoinFrontier* aMsg )
{
	//	DWORD				theFrontierID;
	//	SRpFrontierInfo		theFrontierInfo;				// 프론티어 정보
	//	char				theCaller[RP_MAX_PC_NAME];		// 신수 소환자 
	//	SRpFrontierPetInfo	theFrontierPetInfo;				// 신수 정보 
	
	
	if( nRui->GetPcInvenInfo()->FrontierMyInfo )
	{
		nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID = aMsg->theFrontierID;
	}
#ifdef DECO_RENEWAL_GUILD_MJH
		g_FrontierCreateScrollWnd.m_FrontierLimitTime = aMsg->theLimitTime;
		//nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade = aMsg->theFrontierInfo.
#endif // DECO_RENEWAL_GUILD_MJH	
	//프론티어 엠블렌의 버전을 확인한다.
	if(aMsg->theFrontierID)
	{
		if(aMsg->theEmblemVer)
		{
			g_EmblemTexManager.ConFirmEmblemVersion( aMsg->theFrontierID, aMsg->theEmblemVer );
		}
	}
	memcpy(nRui->GetPcInvenInfo()->FrontierCaller, aMsg->theCaller , sizeof(aMsg->theCaller));
		
	nRui->GetPcInvenInfo()->FrontierInfo = SAFE_NEW( SRpFrontierInfo );	
	
	*( nRui->GetPcInvenInfo()->FrontierInfo ) = aMsg->theFrontierInfo;
	//	memcpy( nRui->GetPcInvenInfo()->FrontierInfo, aMsg->theFrontierInfo, sizeof( SRpFrontierInfo ) );		
	
	if( aMsg->theFrontierPetInfo.thePetCode )
	{
		nRui->GetPcInvenInfo()->FrontierPetInfo = SAFE_NEW( SRpFrontierPetInfo );		
		*( nRui->GetPcInvenInfo()->FrontierPetInfo ) = aMsg->theFrontierPetInfo;
		BYTE Level = nRui->GetPcInvenInfo()->FrontierPetInfo->thePetLevel ; 
		DWORD PetMaxExp = ((Level+1)*(Level+1)) * 75;
		nRui->GetPcInvenInfo()->FrontierPetMaxExp = PetMaxExp ; 

		if( nRui->GetPcInvenInfo()->FrontierPetInfo->thePetHP == 0  )
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_THEHOLLY_ISDIE) , 
						 nRui->GetPcInvenInfo()->FrontierPetInfo->thePetName ); 
		}

		//memcpy( nRui->GetPcInvenInfo()->FrontierPetInfo, aMsg->theFrontierPetInfo, sizeof( SRpFrontierPetInfo ) );		
	}
	 
	g_FrontierMainWnd.RefreshFrontierMyFrontier();
	g_FrontierMainWnd.RefreshFrontierMyFrontierPet();

	
	
#ifdef COLONY_UPDATE
	if(nRui->GetPcInvenInfo()->FrontierInfo->theCombinedFID)
	{
		SAFE_DELETE( nRui->GetPcInvenInfo()->pComBineFrontierInfo );	
		
		nRui->GetPcInvenInfo()->pComBineFrontierInfo = SAFE_NEW( SComBineFrontierInfo );	
		
		nRui->GetPcInvenInfo()->pComBineFrontierInfo->dCombinedFID = nRui->GetPcInvenInfo()->FrontierInfo->theCombinedFID;
		
		memcpy( nRui->GetPcInvenInfo()->pComBineFrontierInfo->strCombinedFName, aMsg->theCombinedFName , RP_MAX_FRONTIER_NAME );
		memcpy( nRui->GetPcInvenInfo()->pComBineFrontierInfo->strCombinedFMasterName, aMsg->theCombinedFMasterName , RP_MAX_PC_NAME );
		nRui->GetPcInvenInfo()->pComBineFrontierInfo->aCombinedFMemberNum = aMsg->theCombinedFMemberNum;
		nRui->GetPcInvenInfo()->pComBineFrontierInfo->aCombinedFFame = aMsg->theCombinedFFame;
		
		g_FrontierMainWnd.RefreshFrontierCombineFrontier();
	}
#endif
}

// 가입된 맴버 정보들을 한명씩 전송 (전체 보기)
void CRockClient::Proc_RpScMsngrSeeJoinFrontier( SRpScMsngrSeeJoinFrontier* aMsg )
{

	if( nRui->GetPcInvenInfo()->FrontierMyInfo )
	{
		if( Rstrcmp( RWCHAR(nRui->GetPcParamInfo()->PCName) , RWCHAR(aMsg->thePCName) ) == 0 )
		{			
			SAFE_STR_CPY( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierNick, aMsg->theInfo.theNick , RP_MAX_FRONTIER_NICK );
			nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade = aMsg->theInfo.theGrade;
			nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierDues = aMsg->theInfo.theDues;
		}
	}
	
	g_FrontierMainWnd.RefreshMyFrontierGrade( aMsg->theInfo.theGrade, aMsg->thePCName );
	
	if( nRui->GetPcInvenInfo()->FrontierInfo )
	{
		nRui->GetPcInvenInfo()->FrontierInfo->theFame = aMsg->theFrontierFame;
	}
	
	// Find Frontier Member in list 
	std::vector < SFMemberInfo > :: iterator ir;							
	
	ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();
	static int scount = 0 ; 
	scount++;

	int count = scount;


	for( ; ir != g_FrontierMemberListWnd.m_FrontierMemberInfoList.end() ; ++ir )
	{
		if( Rstrcmp( RWCHAR((*ir).Name) , RWCHAR(aMsg->thePCName) ) == 0 )
		{					
			memset((*ir).Name, NULL , 32  );
			memset((*ir).NickName , NULL , 32);
			
			memcpy( (*ir).Name, aMsg->thePCName, 32 );
			memcpy( (*ir).NickName, aMsg->theInfo.theNick , sizeof(aMsg->theInfo.theNick) );
			
			if( (*ir).Grade != aMsg->theInfo.theGrade )
			{
				g_FrontierMainWnd.RefreshMyFrontierGrade( aMsg->theInfo.theGrade, aMsg->thePCName );				
			}
			
			(*ir).Grade = aMsg->theInfo.theGrade;
			(*ir).AccumulationFee = aMsg->theInfo.theDues;
			(*ir).Fame = aMsg->theInfo.theFame;
#ifdef DECO_RENEWAL_GUILD_MJH
			memset((*ir).theIntroduce , NULL , 30); 
			memcpy( (*ir).theIntroduce, aMsg->theInfo.theIntroduce, 30 );
			//(*ir).Level = nRui->GetPcParam()->Level; //케릭터 레벨정보
			//(*ir).TheMap = nRui->GetPcParam()->Map;  //케릭터 맵정보
#endif // DECO_RENEWAL_GUILD_MJH		
			g_FrontierMainWnd.RefreshFrontierMyInfo();
			g_FrontierMainWnd.RefreshFrontierMyFrontier();
			g_FrontierMemberListWnd.RefreshMemberInfoList();

			return;
		}
	}	
	
	SFMemberInfo FMemberInfo; 
	
	SAFE_STR_CPY( FMemberInfo.Name, aMsg->thePCName , 32 );
	SAFE_STR_CPY( FMemberInfo.NickName, aMsg->theInfo.theNick , 32 );
	
	FMemberInfo.Grade = aMsg->theInfo.theGrade;
	FMemberInfo.AccumulationFee = aMsg->theInfo.theDues;
	FMemberInfo.Fame = aMsg->theInfo.theFame;
#ifdef DECO_RENEWAL_GUILD_MJH
	SAFE_STR_CPY( FMemberInfo.theIntroduce, aMsg->theInfo.theIntroduce , 30 );
	FMemberInfo.Level = nRui->GetPcParam()->Level; //케릭터 레벨정보
	FMemberInfo.TheMap = nRui->GetPcParam()->Map;   //케릭터 맵정보
#endif // DECO_RENEWAL_GUILD_MJH

	g_FrontierMemberListWnd.m_FrontierMemberInfoList.push_back( FMemberInfo );
	
	g_FrontierMainWnd.RefreshFrontierMyInfo();
	g_FrontierMainWnd.RefreshFrontierMyFrontier();
	g_FrontierMemberListWnd.RefreshMemberInfoList();
}

// 가시범위내의 모든 유저들에게 유저의 변경된 프론티어 정보 전송 
// 창단, 가입, 정보 변경시 이용 
void CRockClient::Proc_RpScSeeFrontier( SRpScSeeFrontier* aMsg )
{

	long lunique = aMsg->thePCID;
	PCTABLE *pPcTable = g_Pc.FindPcTable(lunique);			 
	if(pPcTable)
	{
		SAFE_STR_CPY((pPcTable->char_info.theFrontierName), aMsg->theFrontierName, RP_MAX_FRONTIER_NAME);
		SAFE_STR_CPY((pPcTable->char_info.theFrontierNick), aMsg->theFrontierNick, RP_MAX_FRONTIER_NICK);
		pPcTable->char_info.theFrontierID = aMsg->theFrontierID;
		pPcTable->char_info.theStatus = aMsg->theFrontierStatus;
#ifdef DECO_RENEWAL_GUILD_MJH
		pPcTable->char_info.theFrontierLevel = aMsg->theFrontierLevel;
		pPcTable->char_info.theFrontierFame = aMsg->theFame;
		pPcTable->char_info.theCorps = aMsg->theCorps;	
#endif // DECO_RENEWAL_GUILD_MJH
	}
	
	//프론티어 엠블렌의 버전을 확인한다.
	if(aMsg->theFrontierID)
	{
		if(aMsg->theEmblemVer)
		{
			g_EmblemTexManager.ConFirmEmblemVersion( aMsg->theFrontierID, aMsg->theEmblemVer );
		}
	}
}

// 탈퇴 처리에 대한 응답 (본인 및 모든 프론티어 맴버들에게 전송) 
void CRockClient::Proc_RpScMsngrLeaveFrontier( SRpScMsngrLeaveFrontier* aMsg )
{
	//	char		thePCName[RP_MAX_PC_NAME];
	//	BYTE		theResult;			// NFrontierError 참조 ( 성공에 포함 : n_Leave_Frontier, n_Leaved_Frontier, n_Break_Frontier )
	
	if( aMsg->theResult == n_Success ||
		aMsg->theResult == n_Leave_Frontier ||
		aMsg->theResult == n_Leaved_Frontier ||
		aMsg->theResult == n_Break_Frontier )
	{
#ifdef UPDATE_FAMEINFO
	nRui->GetPcInvenInfo()->FrontierInfo->theFame  = aMsg->theFame;
#endif 


		// 프론티어 정보에 관련된 모든 정보 지움?!! 
		if( Rstrcmp( nRui->GetPcParamInfo()->PCName, aMsg->thePCName ) == 0 )
		{
			if( nRui->GetPcInvenInfo()->FrontierMyInfo != NULL )
			{
				memset( nRui->GetPcInvenInfo()->FrontierMyInfo, 0, sizeof( SFrontier_MyInfo ) );
			}
			
			SAFE_DELETE( nRui->GetPcInvenInfo()->FrontierInfo );			
			nRui->GetPcInvenInfo()->FrontierCaller[0] = '\0';			
			SAFE_DELETE( nRui->GetPcInvenInfo()->FrontierPetInfo );
			SAFE_DELETE( nRui->GetPcInvenInfo()->pComBineFrontierInfo );
			
			g_FrontierMainWnd.ClearFrontierMainWnd();
			
			g_FrontierMemberListWnd.ClearFrontierMemberListWnd();
			g_FrontierMemberListWnd.Close();
			
			g_FrontierMemberEditWnd.Close();
		}
		else
		{
			// Find Frontier Member in list 
			std::vector < SFMemberInfo > :: iterator ir;							
			
			ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();
			
			for( ; ir != g_FrontierMemberListWnd.m_FrontierMemberInfoList.end() ; ++ir )
			{
				if( Rstrcmp( (*ir).Name, aMsg->thePCName ) == 0 )
				{
					g_FrontierMainWnd.RefreshMyFrontierGrade( n_FT_Member, aMsg->thePCName );				
					
					g_FrontierMemberListWnd.m_FrontierMemberInfoList.erase( ir++ );
					
					if( Rstrcmp( RWCHAR(g_FrontierMemberEditWnd.m_MemberInfo.Name), RWCHAR(aMsg->thePCName) ) == 0 )
					{
						g_FrontierMemberEditWnd.Close();
					}
					break;
				}
			}	
			
			g_FrontierMainWnd.RefreshFrontierMyFrontier();
			g_FrontierMemberListWnd.RefreshMemberInfoList();			
		}
			


		//해당 유저의 프론티어 정보를 초기화	
		PCTABLE *pPcTable = g_Pc.FindPcTable(aMsg->thePCName);			 
		if(pPcTable)
		{
			memset(pPcTable->char_info.theFrontierName , NULL ,RP_MAX_FRONTIER_NAME );
			memset(pPcTable->char_info.theFrontierNick , NULL ,RP_MAX_FRONTIER_NICK );
			pPcTable->char_info.theFrontierID = 0;
#ifdef DECO_RENEWAL_GUILD_MJH
			pPcTable->char_info.theFrontierLevel = 0;
			pPcTable->char_info.theFrontierFame = 0;
			pPcTable->char_info.theCorps = 0;	
#endif // DECO_RENEWAL_GUILD_MJH
		}
	}
	else
	{
		GetFrontierProtocolResult( aMsg->theResult );
	}
}

// 프론티어 정보 변경 (보유엘드, 명성치 정보 수정)
void CRockClient::Proc_RpScMsngrFrontierInfo( SRpScMsngrFrontierInfo* aMsg )
{
	//	BYTE		theTotalMember;			// 전체 회원수
	//	BYTE		theLoginedMember;		// 로그인한 회원수 
	//	DWORD		theDues;				// 보유엘드
	//	DWORD		theFame;				// 명성치
	
	nRui->GetPcInvenInfo()->FrontierInfo->theDues = aMsg->theDues;
	nRui->GetPcInvenInfo()->FrontierInfo->theFame = aMsg->theFame;
	
	g_FrontierMainWnd.m_MaxMemberNum = aMsg->theTotalMember;
	
	g_FrontierMainWnd.RefreshFrontierMyFrontier();
}


// 본인 포함 프론티어 맴버들의 정보 변경시 전송
void CRockClient::Proc_RpScMsngrFrontierMemberInfo( SRpScMsngrFrontierMemberInfo* aMsg )
{

	if( nRui->GetPcInvenInfo()->FrontierMyInfo )
	{
		if( Rstrcmp( RWCHAR(nRui->GetPcParamInfo()->PCName) , RWCHAR(aMsg->thePCName) ) == 0 )
		{			
			SAFE_STR_CPY( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierNick, aMsg->theInfo.theNick , RP_MAX_FRONTIER_NICK );
			nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade = aMsg->theInfo.theGrade;
			nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierDues = aMsg->theInfo.theDues;
		}
	}
	
	g_FrontierMainWnd.RefreshMyFrontierGrade( aMsg->theInfo.theGrade, aMsg->thePCName );				
	
	// Find Frontier Member in list 
	std::vector < SFMemberInfo > :: iterator ir;							
	
	ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();
	
	for( ; ir != g_FrontierMemberListWnd.m_FrontierMemberInfoList.end() ; ++ir )
	{
		if( Rstrcmp( RWCHAR((*ir).Name), RWCHAR(aMsg->thePCName) ) == 0 )
		{
			SAFE_STR_CPY( (*ir).Name, aMsg->thePCName , 32 );
			SAFE_STR_CPY( (*ir).NickName, aMsg->theInfo.theNick , 32 );
			
			(*ir).Grade = aMsg->theInfo.theGrade;
			(*ir).AccumulationFee = aMsg->theInfo.theDues;
			(*ir).Fame = aMsg->theInfo.theFame;
			
			break;
		}
	}	
	
	g_FrontierMainWnd.RefreshFrontierMyInfo();
	g_FrontierMainWnd.RefreshFrontierMyFrontier();
	g_FrontierMemberListWnd.RefreshMemberInfoList();
}


// 요청한 프론티어의 엠블렘 정보 갱신 
void CRockClient::Proc_RpScMsngrEmblem( SRpScMsngrEmblem* aMsg )
{
	int a = 0;	
#ifdef DECO_RENEWAL_GUILD_MJH
	char cEmblem[30] = {0,};
	SItemBaseInfo* item_Info;
	item_Info = g_Pc_Manager.GetItemBaseInfoIndexPtr( aMsg->theVersion );

	//sprintf( cEmblem, "%s\\data\\etc.rvf\\%d.jpg", g_RockClient.GetRootDirW() ,item_Info->theFileCode1 );
	sprintf( cEmblem, "%d.jpg",item_Info->theFileCode1 );

	g_EmblemTexManager.UpdateEmblenVersion(aMsg->theFrontierID, aMsg->theVersion ,cEmblem );
#else	
	g_EmblemTexManager.UpdateEmblenVersion(aMsg->theFrontierID, aMsg->theVersion, 
		aMsg->theEmblemSize, aMsg->theEmblem);
#endif // DECO_RENEWAL_GUILD_MJH
	
}

// 납부처리에 대한 결과 
void CRockClient::Proc_RpScPayDues( SRpScPayDues* aMsg )
{
	std::vector < SFMemberInfo > :: iterator ir;							
	
	ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();
	
	for( ; ir != g_FrontierMemberListWnd.m_FrontierMemberInfoList.end() ; ++ir )
	{
		if( Rstrcmp( RWCHAR((*ir).Name), RWCHAR(aMsg->thePCName) ) == 0 )
		{
			(*ir).AccumulationFee = aMsg->theFrontierMemberDues;
			
			break;
		}
	}	
	
	if( nRui->GetPcInvenInfo()->FrontierMyInfo )
	{
		if( Rstrcmp( RWCHAR(nRui->GetPcParamInfo()->PCName), RWCHAR(aMsg->thePCName)  ) == 0 )
		{	
			nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierDues = aMsg->theFrontierMemberDues;
		}
	}
	
	if( nRui->GetPcInvenInfo()->FrontierInfo )
	{
		nRui->GetPcInvenInfo()->FrontierInfo->theDues = aMsg->theFrontierDues;
		
		if( Rstrcmp( RWCHAR(nRui->GetPcParamInfo()->PCName), RWCHAR(aMsg->thePCName) ) == 0 )
		{	
			nRui->GetPcParamInfo()->Money = aMsg->theEld;
		}
	}
	
	g_FrontierMainWnd.RefreshFrontierMyInfo();
	g_FrontierMainWnd.RefreshFrontierMyFrontier();
	g_FrontierMemberListWnd.RefreshMemberInfoList();
}

// 분배된 프론티어 엘드에 대한 정보
void CRockClient::Proc_RpScDivideDues( SRpScDivideDues* aMsg )
{
	// 분배 받았을때 창 표시 (UI 필요)
	char strTemp[128] = {0,};
	
	//#define      IDS_MSGBOX_DESC_DIVIDEELD				 637      ///-- 분배 프론티어 엘드

	//#define      IDS_MSGBOX_DESC_DIVIDEELD_AFTERELD		 637      ///-- 남은 프론티어 엘드

	//#define      IDS_MSGBOX_DESC_DIVIDEELD_MYELD 			 637      ///-- 지급 받은 엘드

	
	//	sprintf( strTemp, " %s ## => %s ## %s ## => %s ## %s ## => %s", "분배 프론티어 엘드", "1234567890",
	//																	"남은 프론티어 엘드", "1234567890",
	//																	"지급 받은 엘드", "1234567890" );
	//	sprintf( strTemp, " %s ## => %s ## %s ## => %s ## %s ## => %s", G_STRING_CHAR( IDS_MSGBOX_DESC_DIVIDEELD ), "1234567890",
	//																	G_STRING_CHAR( IDS_MSGBOX_DESC_DIVIDEELD_AFTERELD ), "1234567890",
	//																	G_STRING_CHAR( IDS_MSGBOX_DESC_DIVIDEELD_MYELD ), "1234567890" );
	
	//	g_MessageBox.OpenMessageBox( n_MESSAGEBOX_NONE, strTemp, 250, 180, "엘드 분배", n_MB_OK );
	
	//#define      IDS_SYSPR_DIVIDEELD_MYELD			 		 637      ///-- 프론티어 마스터에게서 %d 엘드를 분배받았습니다.
	
	int MyDivideEld = aMsg->theEld - nRui->GetPcParamInfo()->Money;
	
	//	SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_DIVIDEELD_MYELD ), MyDivideEld );
	SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_GET_ELD_FROM_F_MASTER ), MyDivideEld );	
	
	nRui->GetPcInvenInfo()->FrontierInfo->theDues = aMsg->theFrontierDues;
	nRui->GetPcParamInfo()->Money = aMsg->theEld;
	
	g_FrontierMainWnd.RefreshFrontierMyInfo();
}

#ifdef DECO_RENEWAL_GUILD_MJH

void CRockClient::Proc_RpScChangeFrontierNotice( SRpScChangeFrontierNotice* aMsg )
{
	//공지사항 변경 
	//nRui->GetPcInvenInfo()->FrontierInfo->theNotice = aMsg->theNewNotice;
	SAFE_STR_CPY( nRui->GetPcInvenInfo()->FrontierInfo->theNotice, aMsg->theNewNotice , 50 );

	g_FrontierMainWnd.RefreshFrontierMyFrontier();

}

#endif // DECO_RENEWAL_GUILD_MJH

void CRockClient::Proc_RpScMsngrFrontierSay( SRpScMsngrFrontierSay* aMsg )
{
	///프론티어에 소속되어 있지않으면
	if( nRui->GetPcInvenInfo()->FrontierMyInfo &&
		nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID == 0 )
	{
		return;
	}
	
	char theMessage[RP_MAX_CHAT_SIZE] = {0,};
	
	memcpy(theMessage, aMsg->theStr, aMsg->theStrSize);
	
	g_ChatMainProc.InsertChatContent(n_GuildChat, aMsg->theFromPCName, theMessage);
}

// 신수 관리 
void CRockClient::Proc_RpScMsngrCreateFrontierPet( SRpScMsngrCreateFrontierPet* aMsg )
{
	if( GetFrontierProtocolResult( aMsg->theResult ) )
	{
		SAFE_DELETE( nRui->GetPcInvenInfo()->FrontierPetInfo );
		
		nRui->GetPcInvenInfo()->FrontierPetInfo = SAFE_NEW( SRpFrontierPetInfo );		

		*( nRui->GetPcInvenInfo()->FrontierPetInfo ) = aMsg->thePetInfo;

		BYTE Level = nRui->GetPcInvenInfo()->FrontierPetInfo->thePetLevel ; 
		DWORD PetMaxExp = ((Level+1)*(Level+1)) * 75;
		nRui->GetPcInvenInfo()->FrontierPetMaxExp = PetMaxExp ; 

		if( nRui->GetPcInvenInfo()->FrontierInfo )
		{
			nRui->GetPcInvenInfo()->FrontierInfo->theDues = aMsg->theDues;
			
			g_FrontierMainWnd.RefreshFrontierMyFrontier();
		}	
		
		g_FrontierMainWnd.RefreshFrontierMyFrontierPet();
	}
} 

void CRockClient::Proc_RpScMsngrDeleteFrontierPet( SRpScMsngrDeleteFrontierPet* aMsg )
{
	nRui->GetPcInvenInfo()->FrontierCaller[0] = '\0';
	
	SAFE_DELETE( nRui->GetPcInvenInfo()->FrontierPetInfo );
	
	g_FrontierMainWnd.RefreshFrontierMyFrontierPet();
}


// 신수의 체력 정보 갱신
void CRockClient::Proc_RpScMsngrFrontierPetStatus( SRpScMsngrFrontierPetStatus* aMsg )
{
	//	WORD				theHP;
	
	if( nRui->GetPcInvenInfo()->FrontierPetInfo )
	{
		nRui->GetPcInvenInfo()->FrontierPetInfo->thePetHP = aMsg->theHP;

		if( nRui->GetPcInvenInfo()->FrontierPetInfo->thePetHP == 0  )
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_THEHOLLY_ISDIE) , 
						 nRui->GetPcInvenInfo()->FrontierPetInfo->thePetName ); 
		}
	}
	
	if( nRui->GetPcInvenInfo()->FrontierMyInfo )
	{
		int		npc_tbl_index = g_Map.FindNpc( nRui->GetPcInvenInfo()->FrontierPetID );
		
		if( npc_tbl_index != MAX_NPC_INTABLE )
		{
			g_Map.m_NpcTable[npc_tbl_index].theHP		= aMsg->theHP;
			g_Map.m_NpcTable[npc_tbl_index].theRealHP	= aMsg->theHP;
		}
	}
	
	g_FrontierMainWnd.RefreshFrontierMyFrontierPet();
}


void CRockClient::Proc_RpScMsngrFrontierPetExp( SRpScMsngrFrontierPetExp* aMsg )
{
	//	DWORD				theExp;
	//	DWORD				theFrontierFame;	// 신수의 경험치로 인한 프론티어의 명성치 갱신 
	
	if( nRui->GetPcInvenInfo()->FrontierInfo )
	{
		nRui->GetPcInvenInfo()->FrontierInfo->theFame = aMsg->theFrontierFame;
	}
	
	if( nRui->GetPcInvenInfo()->FrontierPetInfo )
	{
		nRui->GetPcInvenInfo()->FrontierPetInfo->thePetExp = aMsg->theExp;
	}
	
	g_FrontierMainWnd.RefreshFrontierMyFrontier();	
	g_FrontierMainWnd.RefreshFrontierMyFrontierPet();
}

void CRockClient::Proc_RpScMsngrFrontierPetLevelup( SRpScMsngrFrontierPetLevelup* aMsg )
{
	//	WORD				theLevel;
	//	DWORD				theExp;	
	// thePetID <- 프로토콜 바뀌면 바꾸어 Boa 요	
	///-- 이 부분은 브로드 캐스팅 관련 신수의 외향을 바꾸고.. 정보를 갱신.. 
	if( aMsg->thePetID )
	{
		g_Map.SM__HOLY__LEVELUP( aMsg->thePetID , aMsg->theLevel );
		
		if( nRui->GetPcInvenInfo()->FrontierMyInfo && nRui->GetPcInvenInfo()->FrontierPetInfo )
		{
			if( aMsg->thePetID == nRui->GetPcInvenInfo()->FrontierPetID )
			{
				nRui->GetPcInvenInfo()->FrontierPetInfo->thePetLevel = aMsg->theLevel;
				nRui->GetPcInvenInfo()->FrontierPetInfo->thePetExp = aMsg->theExp;
				DWORD PetMaxExp = ((aMsg->theLevel+1)*(aMsg->theLevel+1))*75;
				nRui->GetPcInvenInfo()->FrontierPetMaxExp = PetMaxExp; 
			}
		}
	}  
	///-- 이부분은 실제 신수를 소유하고있는 유저들에게만 보내준다.
	///-- 신수의 정보만을 갱신 한다.
	else
	{
		nRui->GetPcInvenInfo()->FrontierPetInfo->thePetLevel = aMsg->theLevel;
		nRui->GetPcInvenInfo()->FrontierPetInfo->thePetExp = aMsg->theExp;
		DWORD PetMaxExp = ((aMsg->theLevel+1)*(aMsg->theLevel+1))*75;
		nRui->GetPcInvenInfo()->FrontierPetMaxExp = PetMaxExp; 	
	}
	
	g_FrontierMainWnd.RefreshFrontierMyFrontierPet();
}

void CRockClient::Proc_RpScMsngrCallFrontierPet( SRpScMsngrCallFrontierPet* aMsg )
{
	//	BYTE				theResult;		// NFrontierError 참조 	
	if( GetFrontierProtocolResult( aMsg->theResult ) )
	{
	
		SAFE_STR_CPY( nRui->GetPcInvenInfo()->FrontierCaller, aMsg->theCallerName , RP_MAX_PC_NAME );		
		g_FrontierMainWnd.RefreshFrontierMyFrontierPet();

	}
}

void CRockClient::Proc_RpScMsngrCancelCallFrontierPet( SRpScMsngrCancelCallFrontierPet* aMsg )
{

	nRui->GetPcInvenInfo()->FrontierCaller[0] = '\0';	
	g_FrontierMainWnd.RefreshFrontierMyFrontierPet();

}


void CRockClient::Proc_RpScMsngrBuyFrontierItem( SRpScMsngrBuyFrontierItem* aMsg )
{
	if( GetFrontierProtocolResult( aMsg->theResult ) )
	{		
		nRui->GetPcInvenInfo()->FrontierInfo->theDues = aMsg->theFrontierDues;
		
		SItemBaseInfo* item_Info;
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItemCode ); 
		
		if( item_Info )
		{
			SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_BUYITEM ), aMsg->thePCName, item_Info->theName, aMsg->theNum );			// "%s님이 %s를 %d개 구입하셨습니다."
		}
		
		SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_FRONTIERELD ), aMsg->theFrontierDues );		// "남은 프론티어 엘드 : %d"
	}
}

void CRockClient::Proc_RpScMsngrUpdateFrontierFame( SRpScMsngrUpdateFrontierFame* aMsg )
{
	std::vector < SFMemberInfo > :: iterator ir;							
	
	ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();
	
	for( ; ir != g_FrontierMemberListWnd.m_FrontierMemberInfoList.end() ; ++ir )
	{
		if( Rstrcmp( RWCHAR((*ir).Name), RWCHAR(aMsg->thePCName) ) == 0 )
		{
			(*ir).Fame = aMsg->theFame;
			
			break;
		}
	}	
	
	if( nRui->GetPcInvenInfo()->FrontierInfo )
	{
		nRui->GetPcInvenInfo()->FrontierInfo->theFame = aMsg->theFrontierFame;		
	}
	
	g_FrontierMainWnd.RefreshFrontierMyFrontier();
	g_FrontierMemberListWnd.RefreshMemberInfoList();
}

void CRockClient::Proc_RpScMsngrAddFrontierMember( SRpScMsngrAddFrontierMember* aMsg) 
{
	if( GetFrontierProtocolResult (aMsg->theResult))
	{
		g_FrontierMainWnd.m_MaxMemberNum  = MAX(aMsg->theFrontierMemberNum  , 30 );
		nRui->GetPcInvenInfo()->FrontierInfo->theDues  = aMsg->theEld;
		g_FrontierMainWnd.RefreshFrontierMyFrontier();
	}	
}
	

void CRockClient::Proc_RpScShout( SRpScShout* aMsg )
{
	char theMessage[RP_MAX_CHAT_SIZE] = "";
	memset(theMessage, 0, sizeof(theMessage));

	SItemBaseInfo* item_Info;
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItemCode ); 
	
	if( item_Info == NULL ) 
	{
		return;
	}

	memcpy(theMessage, aMsg->theMessage, aMsg->theMessageSize);

	if( (n_Scroll_Channel_Shout == item_Info->theReHP) && (n_Type_Scroll== item_Info->theType ) )
	{
		g_ChatMainProc.InsertChatContent( n_WholeChannelChat, aMsg->theSender, theMessage);
	}	
	else if( (n_Scroll_AllChannel_Shout == item_Info->theReHP) && (n_Type_Scroll== item_Info->theType ) )
	{
		g_ChatMainProc.InsertChatContent( n_WholeServerChat, aMsg->theSender, theMessage);
	}	
	else if( (n_Scroll_Map_Shout == item_Info->theReHP) && (n_Type_Scroll== item_Info->theType ) )
	{
		g_ChatMainProc.InsertChatContent( n_WholeAreaChat, aMsg->theSender, theMessage);
	}	

}

// 연합 Frontier 관련
void CRockClient::Proc_RpScMsngrReqCombine( SRpScMsngrReqCombine * aMsg)
{
	///-- 가시범위 내에 존재하는지 여부 확인
	PCTABLE *pPcTable = g_Pc.FindPcTable(aMsg->theProposerPCID);
	if(!pPcTable)
	{
		return;
	}	
	
	g_UIMsgWnd.SetParam( pPcTable->char_info.thePCName );
	g_UIMsgWnd.SetParamID( aMsg->theProposerPCID );
	g_UIMsgWnd.SetMsgType( MSG_INCOMBINE_FRONTIER );
	nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CRockClient::Proc_RpScMsngrAnsCombine( SRpScMsngrAnsCombine * aMsg)
{
	TCHAR strTemp[256] = {0,};
	
	if(aMsg->theAnswer)
	{
		GetFrontierProtocolResult( aMsg->theAnswer );
	}
	else
	{
		///-- 가시범위 내에 존재하는지 여부 확인
		PCTABLE *pPcTable = g_Pc.FindPcTable(aMsg->theProposedPCID);
		if(!pPcTable)
		{
			return;
		}
		
		Rsprintf( RWCHAR(strTemp), _RT("%s님이 연합프론티어 초대를 거절하셨습니다."), RWCHAR(pPcTable->char_info.thePCName) );	 ///-- HAN_NAME	
		SystemPrint(n_DarkGreen ,n_SystemMsg, strTemp);   
	}	
}

// 연합프로티어 정보 : 모든 멤버들에게 전송 
void CRockClient::Proc_RpScMsngrCombine( SRpScMsngrCombine * aMsg)
{
	TCHAR strTemp[256] = {0,};
	
	SAFE_DELETE( nRui->GetPcInvenInfo()->pComBineFrontierInfo );	
	nRui->GetPcInvenInfo()->pComBineFrontierInfo = SAFE_NEW( SComBineFrontierInfo );	
	
#ifdef COLONY_UPDATE
	nRui->GetPcInvenInfo()->FrontierInfo->theCombinedFID = aMsg->theCombinedFID;
#endif
	
	if( nRui->GetPcInvenInfo()->pComBineFrontierInfo )
	{		
		nRui->GetPcInvenInfo()->pComBineFrontierInfo->dCombinedFID = aMsg->theCombinedFID;
		SAFE_STR_CPY( nRui->GetPcInvenInfo()->pComBineFrontierInfo->strCombinedFName, aMsg->theCombinedFName , RP_MAX_FRONTIER_NAME );
		SAFE_STR_CPY( nRui->GetPcInvenInfo()->pComBineFrontierInfo->strCombinedFMasterName, aMsg->theCombinedFMasterName , RP_MAX_PC_NAME );
		nRui->GetPcInvenInfo()->pComBineFrontierInfo->aCombinedFMemberNum = aMsg->theCombinedFMemberNum;
		nRui->GetPcInvenInfo()->pComBineFrontierInfo->aCombinedFFame = aMsg->theCombinedFFame;
		
		g_FrontierMainWnd.RefreshFrontierCombineFrontier();
		Rsprintf( RWCHAR(strTemp), _RT("%s 프론티어와 연합을 맺었습니다."), RWCHAR(aMsg->theCombinedFName) );	 ///-- HAN_NAME	
		SystemPrint(n_DarkGreen ,n_SystemMsg, strTemp); 
	}
}

// 연합이 해제되었음을 양측 프론티어 멤버들에게 모두 전송
void CRockClient::Proc_RpScMsngrSeperateF( SRpScMsngrSeperateF * aMsg)
{
	TCHAR strTemp[256] = {0,};
	TCHAR strName[256] = {0,};
	
	if( !GetFrontierProtocolResult( aMsg->theResult ) )
	{
		return;
	}	
	
	if( nRui->GetPcInvenInfo()->FrontierMyInfo &&
		nRui->GetPcInvenInfo()->FrontierInfo && 
		nRui->GetPcInvenInfo()->pComBineFrontierInfo )
	{
		if(aMsg->theSeperateFID == nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID )
		{
			SAFE_STR_CPY(strName, nRui->GetPcInvenInfo()->FrontierInfo->theFrontierName , 256 );
		}
		else
		{
			SAFE_STR_CPY(strName, nRui->GetPcInvenInfo()->pComBineFrontierInfo->strCombinedFName , 256 );
		}
		
		SAFE_DELETE( nRui->GetPcInvenInfo()->pComBineFrontierInfo );
		g_FrontierMainWnd.RefreshFrontierCombineFrontier();
		Rsprintf( RWCHAR(strTemp), _RT("%s 프론티어 요청으로 연합이 해제되었습니다."), RWCHAR(strName) );	 ///-- HAN_NAME
		SystemPrint(n_DarkGreen ,n_SystemMsg, strTemp); 
	}	
}

// 펫 관련
//서버에서 보내준다 펫을 만드 세용 ~
void CRockClient::Proc_RpScMakePetName( SRpScMakePetName * aMsg )
{
	for( int i =0 ;  i < 3 ; ++i)
	{
		//그래도 확인 ... 
		if(m_PcInven->thePetData[i] == NULL)
		{	
			g_PetCreateWnd.Open();
			break;
		}
	}	
}

// 펫 지속 유료 아이템 사용시 
void CRockClient::Proc_RpSRpScUsePetPayItem(SRpScUsePetPayItem * aMsg)
{
	
	//	BYTE			thePetSlot;
	//	SPayEffect		theInfo;
	//  DWORD			theEffectValue;			// 사용후 변경 EffectValue
	
	SItemBaseInfo* item_Info = NULL;
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theInfo.theItemCode );	
	
	if( item_Info )
	{
		if( item_Info->theReHPSpeed || item_Info->theReSPSpeed || item_Info->theReMPSpeed || item_Info->theReGage )
		{
			for( int i = 0; i < RP_MAX_PET_USEDITEM; ++i )
			{
				if( nRui->GetPcInvenInfo()->thePetData[aMsg->thePetSlot]->PayEffect[i] == NULL )
				{
			
					aMsg->theInfo.theTime = aMsg->theInfo.theTime * 1000;
					m_PcInven->thePetData[aMsg->thePetSlot]->PayEffect[i] = SAFE_NEW( SPayEffect );						
					m_PcInven->thePetData[aMsg->thePetSlot]->PayEffect[i]->theItemCode = aMsg->theInfo.theItemCode;
					m_PcInven->thePetData[aMsg->thePetSlot]->PayEffect[i]->theTime = aMsg->theInfo.theTime;
					
					g_PetMainWndProc.SetKeepUpItemCode(i , aMsg->theInfo ,	aMsg->thePetSlot);
					
					
					break;
				}
			}
		}
		
	}	
	
}


// 지속 유료 아이템 끝날때 호출됨 
void CRockClient::Proc_RpSRpScEndPetPayItem(SRpScEndPetPayItem * aMsg)
{
	//BYTE			thePetSlot;
	//WORD			theItemCode;
	//DWORD			theEffectValue;		
	
	SItemBaseInfo* item_Info = NULL;
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theItemCode );	
	
	if( item_Info )
	{
		if( item_Info->theReHPSpeed || item_Info->theReSPSpeed || item_Info->theReMPSpeed || item_Info->theReGage )
		{
			for( int i = 0; i < RP_MAX_PET_USEDITEM; ++i )
			{
				if(  nRui->GetPcInvenInfo()->thePetData[aMsg->thePetSlot]->PayEffect[i])
				{
					if(  nRui->GetPcInvenInfo()->thePetData[aMsg->thePetSlot]->PayEffect[i]->theItemCode == aMsg->theItemCode )
					{
						
						
						SItemBaseInfo* item_Info;
						item_Info = g_Pc_Manager.GetItemBaseInfoPtr(aMsg->theItemCode ); 
						if( item_Info == NULL )
							return;
						
						switch(	item_Info->theReHP)
						{
						case n_Scroll_Pet_Attack:
							m_PcInven->thePetData[aMsg->thePetSlot]->Ability = aMsg->theEffectValue;
							break;
						case n_Scroll_Pet_AttackSpeed:
							{
								m_PcInven->thePetData[aMsg->thePetSlot]->ProbAbility = aMsg->theEffectValue;
								
								if( g_PetInfoWnd.m_CurPetNum == aMsg->thePetSlot && m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum] &&
									m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->PetType == 0 )	//현재 소환중인 펫인 경우

								{
									if(g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master)
									{
										long nDelayTime = 	( 80 - m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->ProbAbility ) * 100;
										g_Map.m_Npc[g_Pc.GetPlayer()->m_State.PetIndex].m_Weapon_DelayTime = nDelayTime;
									}
								}
								
							}
							break;
						case n_Scroll_Pet_DecDamage:
							m_PcInven->thePetData[aMsg->thePetSlot]->Ability = aMsg->theEffectValue;
							break;
						case n_Scroll_Pet_DecDamageRate:
							m_PcInven->thePetData[aMsg->thePetSlot]->ProbAbility = aMsg->theEffectValue;
							break;
							
						default:
							break;
						}
						
						g_PetMainWndProc.DeleteKeepUpItemCode( i , aMsg->thePetSlot);				
						
						SAFE_DELETE( nRui->GetPcInvenInfo()->thePetData[aMsg->thePetSlot]->PayEffect[i] );
						
						g_PetMainWndProc.SortKeepupItem(aMsg->thePetSlot);
						
						break;
					}
				}
			}
		}		
	}	
}

// 펫을 보관소에 맡긴다 
void CRockClient::Proc_RpSRpScPetCheckIn( SRpScPetCheckIn * aMsg)
{
	//	BYTE				thePetSlot;
	//	time_t				theCheckInTime;
	if(m_PcInven->thePetData[aMsg ->thePetSlot] !=NULL  )
	{
		
		m_PcInven->thePetData[aMsg ->thePetSlot]->CheckInTime = aMsg->theCheckInTime;
		
		//보관 됐다 .. 
		m_PcInven->thePetData[aMsg ->thePetSlot]->PetState = n_Pet_Custody ;
		//서버에 한번 보내준다 . 
		if ( m_PcInven->thePetData[aMsg->thePetSlot]->IsVisible ) 
		{
			g_RockClient.Send_CsCancelCallPet();		
		}

		g_PetMainWndProc.PetUpdateState();
		g_PetMainWndProc.Update();
		
		SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PET_CHECKIN),   
			m_PcInven->thePetData[aMsg ->thePetSlot]->Name );		
		
	}
	
}
//펫을 보관소에서 찾아옴 
void CRockClient::Proc_RpSRpScPetCheckOut( SRpScPetCheckOut * aMsg)
{
	//	BYTE				thePetSlot;
	
	if(m_PcInven->thePetData[aMsg ->thePetSlot] !=NULL  )
	{
		//죽다 
		if(m_PcInven->thePetData[aMsg ->thePetSlot]->DieTime != NULL  )
		{
			m_PcInven->thePetData[aMsg ->thePetSlot]->PetState = n_Pet_Die;			
		}
		//살다 
		else 
		{
			m_PcInven->thePetData[aMsg ->thePetSlot]->PetState = n_Pet_Live;
		}		
		
		g_PetMainWndProc.Update();
		
		SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PET_CHECKOUT), //"%s 를 찾았습니다." ,
			m_PcInven->thePetData[aMsg ->thePetSlot]->Name );		
		
	}	
}

void CRockClient::Proc_RpScAddPet( SRpScAddPet * aMsg )
{
	
	int i =0 ;
	int ItemInx = 0 ; 
	int Level = aMsg->thePetInfo.thePetDBInfo.theLevel;
	long MaxExp = (Level+1)*(Level+1) * 10 ;
	int thePetSlot  =  aMsg->thePetInfo.thePetSlot; 
	
	RLGS( "------------------- Proc_RpScAddPet ------------------- \n");

	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}
	
	if( aMsg->theResult == n_Success || aMsg->theResult == 255)
	{
		if( m_PcInven->thePetData[thePetSlot] == NULL )
		{
			SNPCBaseInfo* npc_info = NULL;		
			npc_info = g_Pc_Manager.GetNPCBaseInfoPtr(aMsg->thePetInfo.thePetDBInfo.theCode);
			
			int nPetCode = aMsg->thePetInfo.thePetDBInfo.theCode;
			g_Pc_Manager.m_CharImage.Create( nPetCode, c_NpcRace_Mob , g_lpDevice, false );
			//	g_Pc_Manager.m_CharImage.Create( nPetCode, c_NpcRace_Mob , g_lpDevice, true );	
			
			m_PcInven->thePetData[thePetSlot] = SAFE_NEW( SPetData );			
			
			m_PcInven->thePetData[thePetSlot]->PetSlot =			thePetSlot;
			m_PcInven->thePetData[thePetSlot]->Level =			aMsg->thePetInfo.thePetDBInfo.theLevel;
			m_PcInven->thePetData[thePetSlot]->Ability =			aMsg->thePetInfo.theStat1;
			m_PcInven->thePetData[thePetSlot]->ProbAbility =		aMsg->thePetInfo.theStat2;
			
			m_PcInven->thePetData[thePetSlot]->HP =			aMsg->thePetInfo.thePetDBInfo.theHP;
			m_PcInven->thePetData[thePetSlot]->MaxHP =		npc_info->theMaxHP ;
			
			m_PcInven->thePetData[thePetSlot]->CP =			aMsg->thePetInfo.thePetDBInfo.theCP;
			m_PcInven->thePetData[thePetSlot]->MaxCP		=	npc_info->theMaxSP;
			
			m_PcInven->thePetData[thePetSlot]->Exp =		aMsg->thePetInfo.thePetDBInfo.theExp;
			m_PcInven->thePetData[thePetSlot]->PetID	=	aMsg->thePetID;			
			
			
			//현재 아직 공식이 없다 .. 그래서 랜덤으로 
			m_PcInven->thePetData[thePetSlot]->MaxExp	= MaxExp;
			
			m_PcInven->thePetData[thePetSlot]->DieTime =		aMsg->thePetInfo.thePetDBInfo.theDieTime;
			m_PcInven->thePetData[thePetSlot]->UpdateTime = aMsg->thePetInfo.thePetDBInfo.theUpdateTime;
			m_PcInven->thePetData[thePetSlot]->CheckInPetShop =  aMsg->thePetInfo.thePetDBInfo.theCheckInPetShop;
			
			
			m_PcInven->thePetData[thePetSlot]->MaxInvenNum = aMsg->thePetInfo.thePetDBInfo.theItemMaxNum;
			
			memcpy(m_PcInven->thePetData[thePetSlot]->Name, aMsg->thePetInfo.thePetDBInfo.theName , sizeof( aMsg->thePetInfo.thePetDBInfo.theName) ) ;
			
			///-- 용병은 이름을 NPC테이블에서 받아온다.
			if( thePetSlot == MERCE_SLOT_NUM )
			{
				SNPCBaseInfo*	npc_Info = NULL;
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aMsg->thePetInfo.thePetDBInfo.theCode );
				
				if( npc_Info )
				{
					memcpy( m_PcInven->thePetData[thePetSlot]->Name, npc_Info->theName , sizeof( npc_Info->theName ) ) ;
				}
			}			
			 
			m_PcInven->thePetData[thePetSlot]->NpcCode =		aMsg->thePetInfo.thePetDBInfo.theCode;			
			m_PcInven->thePetData[thePetSlot]->ItemNum = aMsg->thePetInfo.thePetDBInfo.theItemNum;			
			m_PcInven->thePetData[thePetSlot]->DieTime =		aMsg->thePetInfo.thePetDBInfo.theDieTime;
			m_PcInven->thePetData[thePetSlot]->CheckInTime = aMsg->thePetInfo.thePetDBInfo.theCheckInTime;
			//살아 있다		
#ifdef PET_TIME_TEST
			if(m_PcInven->thePetData[thePetSlot]->DieTime == NULL 
				|| (m_PcInven->thePetData[thePetSlot]->NpcCode > 6190 && m_PcInven->thePetData[thePetSlot]->NpcCode < 6197 ) )
#else 
			if(m_PcInven->thePetData[thePetSlot]->DieTime == NULL )
#endif // PET_TIME_TEST
			{
				m_PcInven->thePetData[thePetSlot]->PetState = n_Pet_Live ;
			}
			//죽음 
			else 
			{
				m_PcInven->thePetData[thePetSlot]->PetState = n_Pet_Die;
				m_PcInven->thePetData[thePetSlot]->HP = 0 ; 
			}

			//보관 
			if(m_PcInven->thePetData[thePetSlot]->CheckInTime != NULL)
			{	
				m_PcInven->thePetData[thePetSlot]->PetState = n_Pet_Custody;
			}
			
			//공격 
			if(	npc_info->theAttackType == PET_AI_ATTACK )
			{
				m_PcInven->thePetData[thePetSlot]->PetType = 0;			
			}
			//방어형 
			else if(npc_info->theAttackType == PET_AI_REDUCEDAMAGE)
			{
				m_PcInven->thePetData[thePetSlot]->PetType = 1;			
			}
			
			g_PetMainWndProc.Update();			
			
			//	g_PetMainWndProc.m_PetText[thePetSlot].ReCallBtn->SetEnable(true);
			//	g_PetMainWndProc.m_PetText[thePetSlot].InvenBtn->SetEnable(true)	;
			
			g_PetMainWndProc.m_PetText[thePetSlot].ReCallBtn->SetVisible(true);
			g_PetMainWndProc.m_PetText[thePetSlot].CallOffBtn->SetVisible(false);
			g_PetMainWndProc.m_PetText[thePetSlot].InvenBtn->SetVisible(true);

			if( thePetSlot == MERCE_SLOT_NUM )
			{
				g_PetMainWndProc.m_PetText[thePetSlot].InvenBtn->SetVisible(false);
				m_PcInven->thePetData[thePetSlot]->theLifeTime = aMsg->theLifeTime * 1000;
				m_PcInven->thePetData[thePetSlot]->theStartTime = timeGetTime();

			}
			/********************************************************************
			- 아이템 설정 
			*********************************************************************/
			
			for(ItemInx = 0 ; ItemInx<  aMsg->thePetInfo.thePetDBInfo.theItemMaxNum;  ++ItemInx )
			{
				if( aMsg->thePetInfo.thePetDBInfo.theItem[ItemInx].theItem.theCode != NULL  )
				{
					
					SPcItem* addPcItem = NULL;
					addPcItem = SAFE_NEW( SPcItem );
					
					WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->thePetInfo.thePetDBInfo.theItem[ItemInx].theItem.theCode );
					
					//여긴 펫전용 아이템이 불러옴 .. ㅋㅋ 
					if( ItemType == n_Class_PersonalPet )
					{
						addPcItem->IType	= n_ITYPE_PET_USE; 
						addPcItem->Amount	= aMsg->thePetInfo.thePetDBInfo.theItem[ItemInx].theWasteItem.theBundle;
					}		
					else 
					{
#ifdef _DEBUG
						RLGS("펫전용 아이템에 이상한 아이템이 들어오고 있어요 확인해주세요 ");
#endif
					}
					
					addPcItem->Class		= ItemType;
					addPcItem->ItemUID		= aMsg->thePetInfo.thePetDBInfo.theItem[ItemInx].theItem.theItemUID;
					addPcItem->Code			= aMsg->thePetInfo.thePetDBInfo.theItem[ItemInx].theItem.theCode;
					addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->thePetInfo.thePetDBInfo.theItem[ItemInx].theItem.theCode );
					addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->thePetInfo.thePetDBInfo.theItem[ItemInx].theItem.theCode );
					
					if( addPcItem->ItemTable == NULL ) 
					{
						SAFE_DELETE( addPcItem );
						return;
					}
					
					SAFE_DELETE( m_PcInven->thePetData[thePetSlot]->InvenItem[aMsg->thePetInfo.thePetDBInfo.theItem[ItemInx].theItem.thePosition] );			
					m_PcInven->thePetData[thePetSlot]->InvenItem[aMsg->thePetInfo.thePetDBInfo.theItem[ItemInx].theItem.thePosition] = addPcItem;
					
				}
				
			}	
			/********************************************************************
			- 유료 지속 아이템 
			*********************************************************************/			
			
			SItemBaseInfo* item_Info = NULL;
			for( int i = 0; i < RP_MAX_PET_USEDITEM; ++i )
			{
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->thePetInfo.thePetDBInfo.thePayEffect[i].theItemCode );	
				
				if( item_Info )
				{
					if( item_Info->theReHPSpeed || item_Info->theReSPSpeed || item_Info->theReMPSpeed || item_Info->theReGage )
					{
						
						if( m_PcInven->thePetData[thePetSlot]->PayEffect[i] == NULL )
						{
#ifdef KEEPUP_PAYITEM_RENEWAL
							aMsg->thePetInfo.thePetDBInfo.thePayEffect[i].theTime = aMsg->thePetInfo.thePetDBInfo.thePayEffect[i].theTime * 1000;
#else
							aMsg->thePetInfo.thePetDBInfo.thePayEffect[i].theTime = aMsg->thePetInfo.thePetDBInfo.thePayEffect[i].theTime * 1000;
#endif
								
							m_PcInven->thePetData[thePetSlot]->PayEffect[i] = SAFE_NEW( SPayEffect );					
							
							m_PcInven->thePetData[thePetSlot]->PayEffect[i]->theItemCode = aMsg->thePetInfo.thePetDBInfo.thePayEffect[i].theItemCode;
							m_PcInven->thePetData[thePetSlot]->PayEffect[i]->theTime = aMsg->thePetInfo.thePetDBInfo.thePayEffect[i].theTime;
							
							g_PetMainWndProc.SetKeepUpItemCode(i , *m_PcInven->thePetData[thePetSlot]->PayEffect[i], thePetSlot);							
						}
						
					}
				}
			}

			if(aMsg->theResult == n_Success)
			{	
				//펫을 맹글면 펫정보창을 띄어 주는 센스~~
				g_PetMainWndProc.Open();
				
				if( thePetSlot == MERCE_SLOT_NUM )
				{
					SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYS_ADD_SOLDIER ), 
					m_PcInven->thePetData[thePetSlot]->Name );		
				}
				else
				{
					SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_ADD_PET ), 
					m_PcInven->thePetData[thePetSlot]->Name );
				}
			}			
		}
	}	
}

//NPC 정보 갱신 
void CRockClient::Proc_RpScPetStatus( SRpScPetStatus * aMsg )
{
	int Percent = 0; 
	
	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}
	
	if( m_PcInven->thePetData[aMsg->thePetSlot] != NULL )
	{
		m_PcInven->thePetData[aMsg->thePetSlot]->HP = 	aMsg->theHP;
		m_PcInven->thePetData[aMsg->thePetSlot]->CP =	aMsg->theCP;
		m_PcInven->thePetData[aMsg->thePetSlot]->Exp =  aMsg->theExp;		
		
		Percent = ( (float)m_PcInven->thePetData[aMsg->thePetSlot]->HP / 
					 m_PcInven->thePetData[aMsg->thePetSlot]->MaxHP ) * 100;
		
		if( ( Percent > 0  ) &&  ( Percent< 10 ) )
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOTENOUGHHP ), 
					     m_PcInven->thePetData[aMsg->thePetSlot]->Name );		
		}
	
	    int npc_tbl_index = g_Map.FindNpc(m_PcInven->thePetData[aMsg->thePetSlot]->PetID);
	  
		if( npc_tbl_index != MAX_NPC_INTABLE )
		{
		   g_Map.m_NpcTable[npc_tbl_index].theLevel =  m_PcInven->thePetData[aMsg->thePetSlot]->Level;
		   g_Map.m_NpcTable[npc_tbl_index].theHP	=  m_PcInven->thePetData[aMsg->thePetSlot]->HP;
		   g_Map.m_NpcTable[npc_tbl_index].theMaxHP =  m_PcInven->thePetData[aMsg->thePetSlot]->MaxHP;
		   g_Map.m_NpcTable[npc_tbl_index].theSMP	=  m_PcInven->thePetData[aMsg->thePetSlot]->CP;					
		}		
	}
	
	g_PetMainWndProc.Update();
}

void CRockClient::Proc_RpScPetLevelUP( SRpScPetLevelUP * aMsg )
{
	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}	
	
	int nPrevPetCode = -1;
	int	npc_tbl_index = g_Map.FindNpc( aMsg->thePetID );
	
	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		nPrevPetCode = g_Map.m_NpcTable[npc_tbl_index].code;
	}	
	
	g_Map.SM__PET__LEVELUP( aMsg->thePetID , aMsg->thePetCode, aMsg->theLevel );
	g_Pc_Manager.m_CharImage.Create( aMsg->thePetCode, c_NpcRace_Mob , g_lpDevice, false );
	g_Pc_Manager.m_CharImage.Create( aMsg->thePetCode, c_NpcRace_Mob , g_lpDevice, true );
	
	D3DXVECTOR3	vPos;
	
	vPos = D3DXVECTOR3( g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._41 , 
		g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._42 + 6.0f, 
		g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._43 );
	
	g_Sound.Play( 302 , vPos.x , vPos.y , vPos.z );	
	
	if(  m_PcInven->thePetData[aMsg->thePetSlot ]  && (m_PcInven->thePetData[aMsg->thePetSlot ]->PetID == aMsg->thePetID))
	{
		long MaxExp = (aMsg->theLevel+1)*(aMsg->theLevel+1) * 10;
		
		SNPCBaseInfo* npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aMsg->thePetCode );	
		
		m_PcInven->thePetData[aMsg->thePetSlot]->Level = aMsg->theLevel;
		m_PcInven->thePetData[aMsg->thePetSlot]->Exp = aMsg->theExp;
		m_PcInven->thePetData[aMsg->thePetSlot]->MaxExp = MaxExp;
		
		m_PcInven->thePetData[aMsg->thePetSlot]->MaxCP = npc_Info->theMaxSP;
		m_PcInven->thePetData[aMsg->thePetSlot]->MaxHP = npc_Info->theMaxHP;
		
		m_PcInven->thePetData[aMsg->thePetSlot]->PetID = aMsg->thePetID;
		m_PcInven->thePetData[aMsg->thePetSlot]->NpcCode = aMsg->thePetCode;
	
		//펫의 최대 HP과 최대 MP 
		if(	npc_Info->theAttackType == PET_AI_ATTACK )
		{
			m_PcInven->thePetData[aMsg->thePetSlot]->PetType = 0 ;			
		}
		//방어형 
		else if(npc_Info->theAttackType == PET_AI_REDUCEDAMAGE)
		{
			m_PcInven->thePetData[aMsg->thePetSlot]->PetType = 1 ;			
		}

		int npc_tbl_index = g_Map.FindNpc(m_PcInven->thePetData[aMsg->thePetSlot]->PetID);
		if( npc_tbl_index != MAX_NPC_INTABLE)
		{
			g_Map.m_NpcTable[npc_tbl_index].theLevel = m_PcInven->thePetData[aMsg->thePetSlot]->Level;
			g_Map.m_NpcTable[npc_tbl_index].theHP = m_PcInven->thePetData[aMsg->thePetSlot]->HP;
			g_Map.m_NpcTable[npc_tbl_index].theMaxHP	= m_PcInven->thePetData[aMsg->thePetSlot]->MaxHP;
			g_Map.m_NpcTable[npc_tbl_index].theSMP	= m_PcInven->thePetData[aMsg->thePetSlot]->CP;					
		}		
		
		bool bFind = false;
		for( int i =0 ; i < MAX_PET_SLOT_NUM ; ++i )
		{
			if( m_PcInven->thePetData[i] !=NULL)
			{
				if(nPrevPetCode == m_PcInven->thePetData[i]->NpcCode)
				{
					bFind = true;
				}
			}
		}
		
		if(false == bFind)
		{
			g_Pc_Manager.m_CharImage.Delete( nPrevPetCode, c_NpcRace_Mob, false);
			g_Pc_Manager.m_CharImage.Delete( nPrevPetCode, c_NpcRace_Mob, true);
		}
		
		g_PetMainWndProc.Update();		
	}	
	
}

void CRockClient::Proc_RpScPetStat( SRpScPetStat * aMsg )
{
	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}	
	
	if( m_PcInven->thePetData[aMsg->thePetSlot ] != NULL )
	{
		m_PcInven->thePetData[aMsg->thePetSlot]->Ability = aMsg->theStat1;
		m_PcInven->thePetData[aMsg->thePetSlot]->ProbAbility = aMsg->theStat2;				
		g_PetMainWndProc.Update();
		
		if( g_PetInfoWnd.m_CurPetNum == aMsg->thePetSlot && m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum] &&
			m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->PetType == 0 )	//현재 소환중인 펫인 경우

		{
			if(g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master)
			{
				long nDelayTime = 
					( 80 - m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->ProbAbility ) * 100;
				g_Map.m_Npc[g_Pc.GetPlayer()->m_State.PetIndex].m_Weapon_DelayTime = nDelayTime;
			}
		}
	}	
}
//부활 시키다 
void CRockClient::Proc_RpScPetAlive(SRpScPetAlive * aMsg)
{
	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}	
	
	if(m_PcInven->thePetData[ aMsg->thePetSlot] !=NULL)
	{
		m_PcInven->thePetData[aMsg->thePetSlot]->HP= aMsg->theHP;
		
		m_PcInven->thePetData[aMsg->thePetSlot]->PetState	= n_Pet_Live;		
		
		m_PcInven->thePetData[aMsg->thePetSlot]->DieTime = 0 ; 

		g_PetMainWndProc.Update();			
		
		SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PET_RIVIVAL),   //"%s 가 부활 했습니다." 
			m_PcInven->thePetData[aMsg->thePetSlot]->Name );		
	}	
}

void CRockClient::Proc_RpScPetDie( SRpScPetDie * aMsg )
{
	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}
	
	if( m_PcInven->thePetData[aMsg->thePetSlot] != NULL )
	{
		m_PcInven->thePetData[aMsg->thePetSlot]->DieTime = aMsg->theDieTime;
		//죽으면 
		m_PcInven->thePetData[aMsg->thePetSlot]->PetState = n_Pet_Die;
		
		g_PetMainWndProc.Update();
		
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PET_DIE) ,   //"%s 가 사망했습니다"
			m_PcInven->thePetData[aMsg->thePetSlot]->Name );		
		
	}
	
}

//펫이 죽으면 인젠에서 제거한다 .. 
//죽은자리가 나면 아래펫이 있으면 자리 이동을 한다 .. 

void CRockClient::Proc_RpScDeletePet( SRpScDeletePet * aMsg )
{
	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}	
	
	if(m_PcInven->thePetData[aMsg->thePetSlot] != NULL)
	{
		//	g_Pc_Manager.m_CharImage.Delete( m_PcInven->thePetData[aMsg->thePetSlot]->NpcCode, c_NpcRace_Mob, false);
		//	g_Pc_Manager.m_CharImage.Delete(  m_PcInven->thePetData[aMsg->thePetSlot]->NpcCode, c_NpcRace_Mob, true);
		
		SAFE_DELETE(m_PcInven->thePetData[aMsg->thePetSlot]);		
		g_PetMainWndProc.ClearPetMainWnd();		
	}	
}

void CRockClient::Proc_RpScMoveItemPCtoPet( SRpScMoveItemPCtoPet * aMsg )
{
	
	RLGS( "------------------- Proc_RpScMoveItemPCtoPet ------------------- \n");
	
	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}
	
	if(GetItemProtocolResult (aMsg->theResult) ) 
	{		
		int i =0 ; 	
		
		SPcItem *tmpPcItem = NULL;
		nRui->ResetCursorItem();
		DWORD	PetPos = aMsg->thePetItem.theWasteItem.thePosition;
		DWORD	PcPos = aMsg->thePCItem.theWasteItem.thePosition;		
		
		// 인벤토리 에서 같은아이템을 찾아 내어 삭제한다 ... 
		for( i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}
			
			if( aMsg->thePCItem.theItem.theItemUID == m_PcInven->InvenItem[i]->ItemUID )
			{
				m_PcInven->InvenItem[i]->Amount = aMsg->thePCItem.theWasteItem.theBundle;
				
				if( m_PcInven->InvenItem[i]->Amount == 0 )
				{
					SAFE_DELETE( m_PcInven->InvenItem[i] );					
				}					
				
				//슬롯에 적용 
				InventoryWnd.UpdateInvenSlotItem();	
				break;
			}
		}	
		
		SPcItem* addPcItem = NULL;
		addPcItem = SAFE_NEW( SPcItem );
		
		WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->thePetItem.theItem.theCode );
		
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
		{
			addPcItem->IType		= n_ITYPE_WEAR;
			addPcItem->Amount		= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->thePetItem ), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			addPcItem->IType	= n_ITYPE_USE;//소모품
			addPcItem->Amount	= aMsg->thePetItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			addPcItem->IType	= n_ITYPE_UNUSE;//비소모품
			addPcItem->Amount	= aMsg->thePetItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
		{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= aMsg->thePetItem.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_PersonalPet )
		{
			addPcItem->IType	= n_ITYPE_PET_USE; 
			addPcItem->Amount	= aMsg->thePetItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			addPcItem->IType	= n_ITYPE_RIDE; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->thePetItem ), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{
			addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->thePetItem ), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{
			addPcItem->IType = n_ITYPE_SKIN_WEAR;
			addPcItem->Amount	= 1;
			memcpy( &(addPcItem->WearTable), &(aMsg->thePetItem) , sizeof( URpdItem ) );
		}

		addPcItem->Class		= ItemType;
		addPcItem->ItemUID		= aMsg->thePetItem.theItem.theItemUID;
		addPcItem->Code			= aMsg->thePetItem.theItem.theCode;
		addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->thePetItem.theItem.theCode );
		addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->thePetItem.theItem.theCode );
		
		if( addPcItem->ItemTable == NULL )
		{
			SAFE_DELETE( addPcItem );
			return;
		}

		////-- 현재 아이템에 관련된 딜레이가 적용되었는지 체크해서 넣어준다.					
		if( addPcItem->IType == n_ITYPE_USE && addPcItem->ItemTable )
		{
			 int iDelayGroup = addPcItem->ItemTable->theDelayGroup;
			 if( !(nRui->enableDelayTime[iDelayGroup] ) )						
			 {
				addPcItem->IsDelay = true;						
			 }
		}	
		
		// 왼손,오른손....
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
		

		SAFE_DELETE( m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[PetPos] );		
		m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[PetPos] = addPcItem;	
		
		g_PetInventoryWnd.UpdatePetPcItem();
		InventoryWnd.UpdateInvenSlotItem();			
	}	
}

void CRockClient::Proc_RpScMoveItemPettoPC( SRpScMoveItemPettoPC * aMsg )
{
	
	RLGS( "------------------- Proc_RpScMoveItemPettoPC ------------------- \n");
	
	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}	
	
	if(GetItemProtocolResult (aMsg->theResult) ) 
	{	
		int i =0 ; 
		SPcItem *tmpPcItem = NULL;
		
		DWORD	PetPos = aMsg->thePetItem.theWasteItem.thePosition;
		DWORD	PcPos = aMsg->thePCItem.theWasteItem.thePosition;
		
		// 펫인벤토리 에서 같은아이템을 찾아 내어 삭제한다 ... 
		for( i = 0; i < m_PcInven->thePetData[aMsg->thePetSlot]->MaxInvenNum ; ++i )
		{	
			
			if( m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i] == NULL )
			{
				continue;
			}
			
			if( aMsg->thePetItem.theItem.theItemUID == m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i]->ItemUID )
			{
				
				m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i]->Amount = aMsg->thePetItem.theWasteItem.theBundle;
				
				if( m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i]->Amount == 0 )
				{
					SAFE_DELETE( m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i] );					
				}					
				
				//슬롯에 적용 
				g_PetInventoryWnd.UpdatePetPcItem();
				
				break;
			}
		}	
	
		SPcItem* addPcItem = NULL;
		addPcItem = SAFE_NEW( SPcItem );

		WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->thePCItem.theItem.theCode );
		
		if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) )
		{
			addPcItem->IType		= n_ITYPE_WEAR;
			addPcItem->Amount		= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->thePCItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Use )
		{
			addPcItem->IType	= n_ITYPE_USE;//소모품
			addPcItem->Amount	= aMsg->thePCItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Useless )
		{
			addPcItem->IType	= n_ITYPE_UNUSE;//비소모품
			addPcItem->Amount	= aMsg->thePCItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
			|| ItemType == n_Class_FrontierPet ||  
			ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
		{
			addPcItem->IType	= n_ITYPE_SPECIAL; 
			addPcItem->Amount	= aMsg->thePCItem.theWasteItem.theBundle;
		}		
		else if( ItemType == n_Class_PersonalPet )
		{
			addPcItem->IType	= n_ITYPE_PET_USE; 
			addPcItem->Amount	= aMsg->thePCItem.theWasteItem.theBundle;
		}
		else if( ItemType == n_Class_Riding )
		{
			addPcItem->IType	= n_ITYPE_RIDE; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->thePCItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_RidingItem )
		{
			addPcItem->IType	= n_ITYPE_RIDE_WEAR; 
			addPcItem->Amount	= 1;
			
			memcpy( &(addPcItem->WearTable), &(aMsg->thePCItem), sizeof( URpdItem ) );
		}
		else if( ItemType == n_Class_Skin )
		{
			addPcItem->IType = n_ITYPE_SKIN_WEAR;
			addPcItem->Amount	= 1;
			memcpy( &(addPcItem->WearTable), &(aMsg->thePCItem) , sizeof( URpdItem ) );
		}
		
		addPcItem->Class		= ItemType;
		addPcItem->ItemUID		= aMsg->thePCItem.theItem.theItemUID;
		addPcItem->Code			= aMsg->thePCItem.theItem.theCode;
		addPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->thePCItem.theItem.theCode );
		addPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->thePCItem.theItem.theCode );
		
		if( addPcItem->ItemTable == NULL )
		{
			SAFE_DELETE( addPcItem );
			return;
		}

		////-- 현재 아이템에 관련된 딜레이가 적용되었는지 체크해서 넣어준다.					
		if( addPcItem->IType == n_ITYPE_USE && addPcItem->ItemTable )
		{
			 int iDelayGroup = addPcItem->ItemTable->theDelayGroup;
			 if( !(nRui->enableDelayTime[iDelayGroup] ) )						
			 {
				addPcItem->IsDelay = true;						
			 }
		}		
		
		// 왼손,오른손....
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

		SAFE_DELETE( m_PcInven->InvenItem[PcPos] );		
		m_PcInven->InvenItem[PcPos]  = addPcItem;		  
		
		g_PetInventoryWnd.UpdatePetPcItem();
		InventoryWnd.UpdateInvenSlotItem();	
	}
	
}

void CRockClient::Proc_RpScUsePetItem( SRpScUsePetItem * aMsg )
{

	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}	
	
	//소모품만 사용 가능 
	if( GetItemProtocolResult( aMsg->theResult ) )
	{
		for( int i = 0; i < RP_MAX_PET_ITEM; ++i )
		{
			if( ( m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i] != NULL ) )
			{ 
				if(m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i]->ItemUID == aMsg->theItemID )
				{
					
					nRui->theItemDalayTime[m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i]->ItemTable->theDelayGroup] = g_nowTime;
					
					//...........................................................................................
					// 아이템 사용시 이펙트 효과 By wxywxy
					//...........................................................................................
					g_Particle.m_EffectManager.UseItemEffect( g_Pc.GetPlayer()->GetUnique() , m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i]->Code );
					//...........................................................................................
					
					
					SItemBaseInfo* item_Info;
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr(aMsg->theInfo.theItemCode ); 
					if( item_Info == NULL )
						return;
					
					SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_USEPETITEM ), m_PcInven->thePetData[aMsg->thePetSlot]->Name,	item_Info->theName );
					
					switch(	item_Info->theReHP)
					{
					case n_Scroll_Pet_HP:
						m_PcInven->thePetData[aMsg->thePetSlot]->HP  = aMsg->theEffectValue; 
						break;
					case n_Scroll_Pet_CP:
						m_PcInven->thePetData[aMsg->thePetSlot]->CP = aMsg->theEffectValue;
						break;
					case n_Scroll_Pet_EXP:
						m_PcInven->thePetData[aMsg->thePetSlot]->Exp = aMsg->theEffectValue;
						break;
					case n_Scroll_Pet_ExpBooster:
						break;
					case n_Scroll_Pet_Attack:
						m_PcInven->thePetData[aMsg->thePetSlot]->Ability = aMsg->theEffectValue;
						break;
					case n_Scroll_Pet_AttackSpeed:
						{
							m_PcInven->thePetData[aMsg->thePetSlot]->ProbAbility = aMsg->theEffectValue;
							
							if( g_PetInfoWnd.m_CurPetNum == aMsg->thePetSlot && m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum] &&
								m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->PetType == 0 )	//현재 소환중인 펫인 경우

							{
								if(g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master)
								{
									long nDelayTime = 
										( 80 - m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->ProbAbility ) * 100;
									g_Map.m_Npc[g_Pc.GetPlayer()->m_State.PetIndex].m_Weapon_DelayTime = nDelayTime;
								}
							}
							
						}
						break;
					case n_Scroll_Pet_DecDamage:
						m_PcInven->thePetData[aMsg->thePetSlot]->Ability = aMsg->theEffectValue;
						break;
					case n_Scroll_Pet_DecDamageRate:
						m_PcInven->thePetData[aMsg->thePetSlot]->ProbAbility = aMsg->theEffectValue;
						break;
					case n_Scroll_Pet_Alive:
						break;
					default:
						break;
					}
						
					//사용하고 남은 번들.... 0이면 삭제...
					m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i]->Amount = aMsg->theBundle;
					if( m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i]->Amount == 0 )
					{
						//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
						delete( m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i] );
						m_PcInven->thePetData[aMsg->thePetSlot]->InvenItem[i] = NULL;
					}
					
					g_PetInventoryWnd.UpdatePetPcItem();
					g_PetMainWndProc.Update();
					break;
				}
				
			}			
		}
	}
	
	
}


//소환 
void CRockClient::Proc_RpScCallPet( SRpScCallPet * aMsg )
{
	//aMsg->theResult
	RLGS("-------------Proc_RpScCallPet------------------");
	///--PRINT_DLG_LOG("Proc_RpScCallPet");
	
	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}
	
	if(	GetPetProtocolResult(aMsg->theResult))
	{
		if(m_PcInven->thePetData[aMsg->thePetSlot ] != NULL)
		{
			if(m_PcInven->thePetData[aMsg->thePetSlot]->IsVisible == false) 
			{
				m_PcInven->thePetData[aMsg->thePetSlot]->IsVisible = true;
				
				//펫 정보창에 아이콘 바꿈 
				g_PetMainWndProc.m_PetText[aMsg->thePetSlot].ReCallBtn->SetVisible(false);
				g_PetMainWndProc.m_PetText[aMsg->thePetSlot].CallOffBtn->SetVisible(true);				
				
				int nNpcCode = m_PcInven->thePetData[aMsg->thePetSlot]->NpcCode;
				g_PetInfoWnd.m_pChaImgBox->SetRenderCharImgProperty(nNpcCode, c_NpcRace_Mob, 
					CHARIMAGE_64SIZE, false, true);
				
				g_PetInfoWnd.m_CurPetNum = aMsg->thePetSlot;
				g_PetInfoWnd.Open(aMsg->thePetSlot);				
			}			
		}		
	}
	
}
//소환 해제 ..
void CRockClient::Proc_RpScCancelCallPet( SRpScCancelCallPet * aMsg )
{
	///--PRINT_DLG_LOG("Proc_RpScCancelCallPet");
	RLGS("-------------Proc_RpScCancelCallPet------------------");
	
	///-- 펫 슬롯이 최대 슬롯수를 넘어갈때
	if( aMsg->thePetSlot >= MAX_PET_SLOT_NUM )
	{
		return;
	}
	
	if(aMsg->theResult == n_Success)
	{
		if(m_PcInven->thePetData[aMsg->thePetSlot] != NULL)
		{
			if(m_PcInven->thePetData[aMsg->thePetSlot]->IsVisible == true) 
			{
				m_PcInven->thePetData[aMsg->thePetSlot]->IsVisible = false ;
				
				//펫 정보창에 아이콘 바꿈 
				g_PetMainWndProc.m_PetText[aMsg->thePetSlot].ReCallBtn->SetVisible(true );
				g_PetMainWndProc.m_PetText[aMsg->thePetSlot].CallOffBtn->SetVisible(false);
				
				g_PetInfoWnd.m_CurPetNum = aMsg->thePetSlot;
				g_PetInfoWnd.Close();

				if( g_PetInventoryWnd.IsVisible )
				{
					g_PetInventoryWnd.Close();
				}				
			}
		}
		
	}
	
}

void CRockClient::Proc_RpScAppearPet( SRpScAppearPet * aMsg )
{
	//여기서 소환과 이동을 분리한다.
	SNPCBaseInfo* npc_Info;
	bool bFlag = true;
	
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aMsg->theCode );
	RLG1( "-----------PetName %s ---------------- \n" , aMsg->thePetName );
	
	if( npc_Info == NULL )
		return;
	
	list< SDelayAppearPet * >::iterator i = g_DelayActList.m_DelayAppearPetList.begin();
	
	for( ; i != g_DelayActList.m_DelayAppearPetList.end() ; ++i )
	{			
		if( ( *i )->AppearPet.thePetID == aMsg->thePetID )
		{
			///-- RLG1( "-------- Being Pet List %d---------- \n" , aMsg->thePetID );
			return;			
		}				
	}
	
	int	 npc_tbl_index = g_Map.FindNpc( (long)aMsg->thePetID );
	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		///-- RLG1( "-------- Being Pet List %d---------- \n" , aMsg->thePetID );
		return;	
	}
	
	if(g_Pc.m_MainPC.lUnique != aMsg->thePCID)
	{
		NonPlayer* pc = g_Pc.GetUser( (long)aMsg->thePCID );
		if( NULL == pc )
		{
			RLG1( "-------- Not Being Master List %d---------- \n" , aMsg->thePCID );
			bFlag = false;
		}
	}
	
	list< SDelayDisappearPet * >::iterator ii = g_DelayActList.m_DelayDisappearPetList.begin();
	for( ; ii != g_DelayActList.m_DelayDisappearPetList.end() ; ++ii )
	{			
		if( ( *ii ) && ( *ii )->DisappearPet.thePetID == aMsg->thePetID )
		{
			SDelayDisappearPet *pData = (*ii); 
			SAFE_DELETE( pData );
			(*ii) = NULL;
			
			RLG1( "-------- Delete Pet List %d---------- \n" , aMsg->thePetID );
			g_DelayActList.m_DelayDisappearPetList.erase( ii++ );
		}				
	}
	
	//...........................................................................................................
	// 펫 처리
	//...........................................................................................................
	if( aMsg->theCallFlag ) // 소환시
	{
		SDelayAppearPet* pDelayAppearPet = NULL;
		pDelayAppearPet = SAFE_NEW( SDelayAppearPet );				
		pDelayAppearPet->DelayTime = SAFE_TIME_ADD( g_nowTime , 1000 );
		pDelayAppearPet->AppearPet = *( aMsg );
		g_DelayActList.m_DelayAppearPetList.push_back( pDelayAppearPet );
		
		if(bFlag)
		{
			int pX = (int)(aMsg->theCell%g_Pc.m_cSize);
			int pZ = (int)(aMsg->theCell/g_Pc.m_cSize);
			
			float fX = pX * (TILE_SIZE/2) + (TILE_SIZE/4);
			float fZ = pZ * (TILE_SIZE/2) + (TILE_SIZE/4);
			float fY = g_Map.Get__MapHeightF( fX , fZ ) + 6.0f;
			
			g_Particle.SetEffSetCon( 0.0f , EF_PET_CREATE , ESLT_POS , 8.1f , -1 , -1 , 
				D3DXVECTOR3( fX , fY , fZ  ) );
			
			g_Sound.Play( 298 , fX , fY , fZ );
		}
		
		RLG1( "------------------- Insert Pet List %d ------------------- \n" , aMsg->thePetID );
	}
	else
	{
		DelayProc_RpScAppearPet( aMsg );
	}
	
}

void CRockClient::DelayProc_RpScAppearPet( SRpScAppearPet* aMsg )
{
	RLGS( " **** DelayProc_RpScAppearPet **** " );	
	///-- Pet Master의 처리
	Character *pMaster = NULL;
	int nMasterIndex = MAX_USER_INTABLE;
	if(aMsg->thePCID == g_Pc.m_MainPC.lUnique)
	{
		g_Pc.GetPlayer()->m_State.nPetState = nPetState_Master;
		pMaster = g_Pc.GetPlayer();
		nMasterIndex = MAX_USER_INTABLE;
	}
	else
	{
		NonPlayer* pc = g_Pc.GetUser( (long)aMsg->thePCID );
		if(pc)
		{
			pc->m_State.nPetState = nPetState_Master;
			pMaster = (Character *)pc;
			nMasterIndex = pc->m_nTableIndex;
		}	
	}
	
	if(!pMaster)
	{
		return;
	}		
	
	//-- Pet의 처리
	int		npc_tbl_index = g_Map.FindNpc( (long)aMsg->thePetID );	
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		float fAngle = 0.0f;
		if(pMaster)
		{
			fAngle = pMaster->GetDirection();
		}
		
		SNPCBaseInfo* npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aMsg->theCode );
		
		if( npc_Info == NULL )
		{
			return;
		}

		if( npc_Info->theRace == NPC_RACE_REALHUMAN )
		{
			
			SPlayerNPCData PNPCData = g_Pc_Manager.m_PlayerNPCDataManager.FindPlayerNPCData(aMsg->theCode);
			if( aMsg->theCode != PNPCData.NPC_Code )
			{
				return;
			}
			
			npc_tbl_index = g_Map.SM__PLAYER_NPC__APPEAR( aMsg->theCode, aMsg->thePetID, 
			aMsg->theCell, aMsg->theHP, aMsg->theCP, 0 , fAngle, aMsg->theLevel );
		}
		else
		{
			npc_tbl_index = g_Map.SM__PET__APPEAR( aMsg->theCode, aMsg->thePetID, aMsg->theCell, 
				aMsg->theHP, aMsg->theCP, 0 ,aMsg->theLevel , fAngle );
		}
		
		if( npc_tbl_index == -1 )
		{
			pMaster->m_State.nPetState = nPetState_Normal;
			return;
		}
		
		// 선점 처리 By wxywxy
		g_Map.m_Npc[npc_tbl_index].SetPriority( n_NONE , -1 );		
		
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( aMsg->theCode );	
		
		long nDelayTime = npc_Info->theAttackSpeed * 100;
		
		if( nMasterIndex == MAX_USER_INTABLE && m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum] && 
			m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->PetType == 0 )
		{
			nDelayTime = ( 80 - m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->ProbAbility ) * 100;			
		}		
		
		g_Map.m_Npc[npc_tbl_index].m_Weapon_DelayTime = nDelayTime; 		
		
		g_Map.m_Npc[npc_tbl_index].m_ani_percent = 1.0f;					
		g_Map.m_Npc[npc_tbl_index].m_move_percent = 1.0f;	
		g_Map.m_Npc[npc_tbl_index].m_State.nPetState = nPetState_Pet;
		g_Map.m_Npc[npc_tbl_index].m_State.PetIndex  = nMasterIndex;		
		
		if(pMaster)
		{
			pMaster->m_State.PetIndex = npc_tbl_index;
		}
		//.......................................................................................................
	}
	else
	{
		
		//지금 해당 NPC가 존재하고 있다면;;;
		NonPlayer *pNonPlaer = g_Map.GetMonster( (int)aMsg->thePetID );
		
		if(pNonPlaer)
		{
			///--JAPAN_BUG_MODIFY 
			///-- 해당 NPC가 죽어있으면;; 여기서는 죽는 애니메이션이 들어가기전에..
			///-- 리젠이 되어버릴수 있기 때문에...;;; 이런처리를 함 
			if( pNonPlaer->IsRealDie() )
			{
				//맵에서 삭제
				g_Map.SM__NPC__DISAPPEAR( 0, aMsg->thePetID );			
				//다시 AppearNPC 호출

				DelayProc_RpScAppearPet( aMsg );			
				return;				
			}			
			
			// 정보 갱신 
			g_Map.m_NpcTable[npc_tbl_index].lBlock = aMsg->theCell;
			g_Map.m_NpcTable[npc_tbl_index].lDestBlock = aMsg->theCell;
			g_Map.m_NpcTable[npc_tbl_index].theHP = aMsg->theHP;
			g_Map.m_NpcTable[npc_tbl_index].theSMP = aMsg->theCP;		
			g_Map.m_NpcTable[npc_tbl_index].theLevel = aMsg->theLevel;
			
			
			if( g_Map.m_NpcTable[npc_tbl_index].lDestBlock != aMsg->theCell )
			{
				float fAngle = 0.0f;
				if(pMaster)
				{
					fAngle = pMaster->GetDirection();
				}
				g_Map.m_Npc[npc_tbl_index].SetDirection( D3DXToRadian( fAngle ) );
			}
			
			g_Map.m_Npc[npc_tbl_index].m_State.nPetState = nPetState_Pet;
			g_Map.m_Npc[npc_tbl_index].m_State.PetIndex  = nMasterIndex;
			
			if(pMaster)
			{
				pMaster->m_State.PetIndex = npc_tbl_index;
			}				
			//.......................................................................................................
		}		
		
		return;		
	}	
	
	
	if(pMaster)
	{
		if( pMaster->m_CharAlpha.GetState() == CHAR_HIDE )
		{
			g_Map.m_Npc[npc_tbl_index].m_CharAlpha.Set( CHAR_HIDE, 0.3f );
		}
		else if( pMaster->m_CharAlpha.GetState() == CHAR_NONE_DISPLAY )
		{
			g_Map.m_Npc[npc_tbl_index].m_CharAlpha.Set( CHAR_NONE_DISPLAY, 0.0f );
		}
	}	
	
	//......................................................................................................
	// NPC 생성시 이펙트 달고 나오기 By wxywxy
	//......................................................................................................
	npc_tbl_index = g_Map.FindNpc( (long)aMsg->thePetID );			// 위에서 새로 만들수 있어서 다시 검색

	
	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		SNPCBaseInfo* npc_Info = NULL;
		
		NPCTABLE *pTable = g_Map.GetNpcTable( (long)aMsg->thePetID );
		
		if( NULL != pTable )
		{
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
			
			if( NULL != npc_Info )
			{
				if( npc_Info->theRightEff != -1 )
				{
					g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theRightEff , ( Character * ) &g_Map.m_Npc[npc_tbl_index] , NULL , NULL );
				}
			}
		}
	}
	// 정보 갱신 

	SNPCBaseInfo*	npc_Info = NULL;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[ npc_tbl_index ].code ); 
	
	if( Rstrcmp( RWCHAR(aMsg->thePetName), _RT("") ) == 0 )
	{
		Rsprintf( RWCHAR(g_Map.m_NpcTable[ npc_tbl_index ].szName), _RT("%s"), RWCHAR(npc_Info->theName) );
	}
	else
	{
		Rsprintf( RWCHAR(g_Map.m_NpcTable[ npc_tbl_index ].szName), _RT("%s"), RWCHAR(aMsg->thePetName) );	
	}
	
	if( NULL != npc_Info )
	{
		g_Map.m_Npc[npc_tbl_index].m_ani_percent = 1.0f;					
		g_Map.m_Npc[npc_tbl_index].m_move_percent = 1.0f;
	}
}

void CRockClient::Proc_RpScDisappearPet( SRpScDisappearPet * aMsg )
{
	///-- Pet Master의 처리
	Character *pMaster = NULL;
	if(aMsg->thePCID == g_Pc.m_MainPC.lUnique)
	{
		g_Pc.GetPlayer()->m_State.nPetState = nPetState_Normal;
		g_Pc.GetPlayer()->m_State.PetIndex = -1;		
	}
	else
	{
		NonPlayer* pc = g_Pc.GetUser( (long)aMsg->thePCID );
		if(pc)
		{
			pc->m_State.nPetState = nPetState_Normal;
			pc->m_State.PetIndex = -1;			
		}	
	}
	
	//-- Pet의 처리	
	int npc_tbl_index = g_Map.FindNpc( (long)aMsg->thePetID );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		list< SDelayAppearPet * >::iterator i = g_DelayActList.m_DelayAppearPetList.begin();
		
		for( ; i != g_DelayActList.m_DelayAppearPetList.end() ; ++i )
		{			
			if( ( *i ) && ( *i )->AppearPet.thePetID == aMsg->thePetID )
			{
				SDelayAppearPet *pData = (*i);
				SAFE_DELETE( pData );
				(*i) = NULL;
					
				g_DelayActList.m_DelayAppearPetList.erase( i++ );
				
				RLG1( "------------------- Delete Pet List %d------------------- \n" , aMsg->thePetID );
				
				return;
			}				
		}		
		
		///-- RLGS( " Nothing NPC ID " );	
		return;
	}
	// 소환 해제 되는 펫이면 처리하기 
	if( aMsg->theCancelCallFlag )
	{
		D3DXVECTOR3	vPos;
		
		vPos = D3DXVECTOR3( g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._41 , 
			g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._42 + 6.0f, 
			g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._43 );
		
		g_Particle.SetEffSetCon( 0.0f , EF_PET_DESTROY , ESLT_POS , 5.1f , -1 , -1 , vPos );
		g_Sound.Play( 299 , vPos.x , vPos.y , vPos.z );
		
		
		SDelayDisappearPet* pDelayDisappearPet = NULL;
		pDelayDisappearPet = SAFE_NEW( SDelayDisappearPet );	
			
		pDelayDisappearPet->DelayTime = SAFE_TIME_ADD( g_nowTime , 1000 );
		pDelayDisappearPet->DisappearPet = *( aMsg );
		g_DelayActList.m_DelayDisappearPetList.push_back( pDelayDisappearPet );
		
		RLG1( "------------------- Delete Pet List %d------------------- \n" , aMsg->thePetID );
	}
	else
	{
		g_Map.SM__NPC__DISAPPEAR( 0, aMsg->thePetID );
	}	
	
	if( NonPlayerInfoWnd.thePickedNpcID == npc_tbl_index )
	{
		NonPlayerInfoWnd.thePickedNpcID = -1;
		nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		
		if(g_NonPlayerInfoAdd2Wnd.IsVisible)
		{
			g_NonPlayerInfoAdd2Wnd.Close();
		}
	}					
	
	return;
}

void CRockClient::DelayProc_RpScDisAppearPet( SRpScDisappearPet * aMsg )
{
	g_Map.SM__NPC__DISAPPEAR( 0, aMsg->thePetID );
}

void CRockClient::Proc_RpScPetStartMove( SRpScPetStartMove * aMsg )
{
	NonPlayer* pc = g_Map.GetMonster( (int)aMsg->thePetID );
	int tbl_index = g_Map.FindNpc( (long)aMsg->thePetID );
	///--JAPAN_BUG_MODIFY
	if( tbl_index == MAX_NPC_INTABLE )
		return;	
	
	if( aMsg->theResult == n_Success )	/// 갈 수 있다! 
	{
		g_Map.SetMapData( CLEAR_BLOCK, tbl_index, g_Map.m_NpcTable[tbl_index].lDestBlock, false );
		
		g_Map.m_Npc[tbl_index].m_ani_percent = ( (float)aMsg->theMoveSpeed / c_Pc_MoveSpeed );
		g_Map.m_Npc[tbl_index].m_move_percent = 
			c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.m_Pc[ tbl_index ].m_ani_percent );
		
		RLG1( "------ Proc_RpScPetStartMove m_ani_percent %f ----- \n" 
			, g_Map.m_Npc[tbl_index].m_ani_percent );		
		
		///-- 내 펫과 다른플레이어의 펫을 분리처리
		/// 내 펫이면

		if( g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master && 
			tbl_index == g_Pc.GetPlayer()->m_State.PetIndex )
		{
			
			if( g_Map.m_Npc[tbl_index].theAttack_Event.type == SM_MOVE_ATTACK )
			{
				if( g_Map.m_Npc[tbl_index].m_bIsLoaded == true )
				{
					g_Map.m_Npc[tbl_index].PushQueue( g_Map.m_Npc[tbl_index].theAttack_Event, TRUE );					
				}
			}
			else
			{
				EVENT_DATA_INFO	data;
				data.x =  aMsg->theDestiCell % g_Pc.m_cSize;
				data.y =  aMsg->theDestiCell / g_Pc.m_cSize;
				data.type = SM_MOVE_BROADCAST;	
				
				// 임시 이동 테스트 ( 다른 유저와 NPC )
				if( g_Map.m_Npc[tbl_index].m_bIsLoaded == true )
					g_Map.m_Npc[tbl_index].PushQueue( data );
			}		
		}
		else
		{
			EVENT_DATA_INFO	data;
			data.x =  aMsg->theDestiCell % g_Pc.m_cSize;
			data.y =  aMsg->theDestiCell / g_Pc.m_cSize;
			data.type = SM_MOVE_BROADCAST;	
			
			// 임시 이동 테스트 ( 다른 유저와 NPC )
			if( g_Map.m_Npc[tbl_index].m_bIsLoaded == true )
				g_Map.m_Npc[tbl_index].PushQueue( data );	
		}
		
		g_Map.m_NpcTable[tbl_index].lDestBlock = aMsg->theDestiCell;		
		g_Map.m_Npc[tbl_index].m_State.bWalk = false;
	}
}

void CRockClient::Proc_RpScPetStop( SRpScPetStop * aMsg )
{
	
	int stopIdx = g_Map.FindNpc( (long)aMsg->thePetID );		
	///--JAPAN_BUG_MODIFY
	if( stopIdx == MAX_NPC_INTABLE )
		return;
	
	if( g_Map.m_NpcTable[stopIdx].lDestBlock != aMsg->theCell )
	{
		g_Map.SetMapData( CLEAR_BLOCK, stopIdx, g_Map.m_NpcTable[stopIdx].lDestBlock, false );		
		g_Map.m_NpcTable[stopIdx].lDestBlock = aMsg->theCell;		
		g_Map.m_Npc[stopIdx].SetTilePos( g_Map.m_NpcTable[stopIdx].lDestBlock%(g_Map.m_cSize*2),
			g_Map.m_NpcTable[stopIdx].lDestBlock/(g_Map.m_cSize*2) );		
	}
}

void CRockClient::Proc_RpScPetAttack( SRpScPetAttack * aMsg )
{
	EVENT_DATA_INFO attack_event;
	attack_event.type = SM_ATTACK_NPC;
	
	EVENT_DATA_INFO underattack_event;
	underattack_event.type = SM_UNDERATTACK;
	
	int attackIdx = 0;
	int attackedIdx = 0; 
	int npc_tbl_index = 0;

	if( aMsg->theResult != n_Success )
	{
		// 거리가 멀어서 공격 취소가 되었을 경우 클라이언트에서 알아서 재공격 설정
		if( aMsg->theResult== n_TOO_LONG_DISTANCE )
		{
			//내 펫이냐...
			if( g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master )
			{
				if( g_Map.m_NpcTable[g_Pc.GetPlayer()->m_State.PetIndex].lUnique == aMsg->thePetID )
				{
					EVENT_DATA_INFO attack_event;
					attack_event.type	= CM_MOVE_ATTACK;
					
					if( aMsg->theAttackedType == n_PC )
					{
						attackedIdx = g_Pc.FindUser(aMsg->theAttackedID);
						
						if( attackedIdx == MAX_USER_INTABLE )
						{
							return;
						}
						
						attack_event.tbl_Idx	= attackedIdx;
						attack_event.race		= g_Pc.m_PcTable[ attackedIdx ].char_info.race;
						attack_event.index		= g_Pc.m_PcTable[ attackedIdx ].nIndex;
						attack_event.unique		= g_Pc.m_PcTable[ attackedIdx ].lUnique;
						attack_event.x			= g_Pc.m_PcTable[ attackedIdx ].lDestBlock%(g_Map.m_cSize*2);
						attack_event.y			= g_Pc.m_PcTable[ attackedIdx ].lDestBlock/(g_Map.m_cSize*2);
						attack_event.vPos.x		= g_Pc.m_Pc[ attackedIdx ].GetPosTM()._41;
						attack_event.vPos.y		= g_Pc.m_Pc[ attackedIdx ].GetPosTM()._42;
						attack_event.vPos.z		= g_Pc.m_Pc[ attackedIdx ].GetPosTM()._43;
						attack_event.cell		= g_Pc.m_PcTable[ attackedIdx ].lDestBlock;
						
						///-- 공격시 서버와 클라이언트 거리상 에러 체크값 증가
						attack_event.nADisErrCnt = g_Pc.GetPlayer()->m_save_event.nADisErrCnt;
						attack_event.nADisErrCnt++;					

						///--PRINT_DLG_LOG("ScPetAttack - Too Long");
						g_Pc.GetPlayer()->PetSetAttackQueue( attack_event );				
					}
					else
					{
						attackedIdx = g_Map.FindNpc(aMsg->theAttackedID);
						
						if( attackedIdx == MAX_NPC_INTABLE )
						{
							return;
						}
						
						attack_event.tbl_Idx	=  attackedIdx ;
						attack_event.race		= g_Map.m_NpcTable[ attackedIdx ].race;
						attack_event.index		= g_Map.m_NpcTable[ attackedIdx ].index;
						attack_event.unique		= g_Map.m_NpcTable[ attackedIdx ].lUnique;
						attack_event.x			= g_Map.m_NpcTable[ attackedIdx ].lDestBlock%(g_Map.m_cSize*2);
						attack_event.y			= g_Map.m_NpcTable[ attackedIdx ].lDestBlock/(g_Map.m_cSize*2);
						attack_event.vPos.x		= g_Map.m_Npc[ attackedIdx ].GetPosTM()._41;
						attack_event.vPos.y		= g_Map.m_Npc[ attackedIdx ].GetPosTM()._42;
						attack_event.vPos.z		= g_Map.m_Npc[ attackedIdx ].GetPosTM()._43;
						attack_event.cell		= g_Map.m_NpcTable[ attackedIdx ].lDestBlock;
						
						attack_event.nADisErrCnt = g_Pc.GetPlayer()->m_save_event.nADisErrCnt;
						attack_event.nADisErrCnt++;

						g_Pc.GetPlayer()->PetSetAttackQueue( attack_event );					
					}
				}
				return;	
			}
		}
		return;	
	}

	//내 펫이냐...
	if( g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master )
	{
		if( g_Map.m_NpcTable[g_Pc.GetPlayer()->m_State.PetIndex].lUnique == aMsg->thePetID )
		{
			///-- 공격시 서버와 클라이언트 거리상 에러 체크값 0로 초기화
			g_Map.m_Npc[g_Pc.GetPlayer()->m_State.PetIndex].m_save_event.nADisErrCnt = 0;
			
			if( m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum] && (aMsg->theCP <= m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->MaxCP))
			{
				m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->CP = aMsg->theCP;
				
				npc_tbl_index = g_Map.FindNpc(aMsg->thePetID);
				g_Map.m_NpcTable[npc_tbl_index].theSMP = aMsg->theCP;
				
				if( aMsg->theCP == 0 )
				{
					SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOTENOUGHCP ), m_PcInven->thePetData[g_PetInfoWnd.m_CurPetNum]->Name);		
				}
			}
		}	
	}
	//상대방 펫이냐 ? 
	else 
	{		
		npc_tbl_index = g_Map.FindNpc(aMsg->thePetID);
		///--JAPAN_BUG_MODIFY
		if( npc_tbl_index != MAX_NPC_INTABLE )
		{
			g_Map.m_NpcTable[npc_tbl_index].theSMP = aMsg->theCP;			
		}		
	}

	// 콤보 시퀀스 완성시에 추가 효과 스킬 적용 
	
	///-- 플레이어가 맞을 때
	if( aMsg->theAttackedType == n_PC )
	{
		if( g_Pc.m_MainPC.lUnique == aMsg->theAttackedID )	//내가 맞을때
		{
			attack_event.type	= SM_ATTACK_NPC;
			attack_event.tbl_Idx = MAX_USER_INTABLE;
			attack_event.race	= g_Pc.m_MainPC.char_info.race;
			attack_event.index	= g_Pc.m_MainPC.nIndex;
			attack_event.unique = g_Pc.m_MainPC.lUnique;
			attack_event.x		= g_Pc.m_MainPC.lDestBlock%(g_Map.m_cSize*2);
			attack_event.y		= g_Pc.m_MainPC.lDestBlock/(g_Map.m_cSize*2);
			attack_event.vPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
			attack_event.vPos.y = g_Pc.GetPlayer()->GetPosTM()._42;
			attack_event.vPos.z = g_Pc.GetPlayer()->GetPosTM()._43;		
			
			//내 펫이 날때리면 - 에너지 흡수

			attackIdx = g_Map.FindNpc(aMsg->thePetID);
			int damage = 0;
			
			///-- 내 펫이 날 때리는 것이라면

			if( g_Map.m_Npc[attackIdx].m_State.PetIndex == MAX_USER_INTABLE )
			{	
				damage = aMsg->theAttackedHP;
								
				if( damage == 0 ) 
				{ 
					attack_event.damage = 0; 
				}	
				else
				{ 
					attack_event.damage = damage;

					if( aMsg->theCritical == 1 ) 
					{
						char strtmp[256]="";
						char strtmpAdd[256]="";
						Rsprintf(RWCHAR(strtmp),G_STRING_CHAR(IDS_SYSPR_ABSORPTION_DAMAGE),damage);
						//sprintf(strtmpAdd ,);
						SAFE_STR_CAT(strtmp ,G_STRING_CHAR(IDS_SYSPR_PVP_DEFENSEPET_MAX80) , 255 );
						SystemPrint( n_DarkGreen, n_SystemMsg, strtmp );
					}
					else
					{
						SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ABSORPTION_DAMAGE), damage );
					}
				}
			}
			else
			{
				damage = g_Pc.m_MainPC.char_info.theRealHP - aMsg->theAttackedHP;
				g_Pc.m_MainPC.char_info.theRealHP = aMsg->theAttackedHP;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
				if( damage < 0 ) 
				{ 
					attack_event.damage = 0; 
				}	
				else
				{ 
					attack_event.damage = damage;
					///--PRINT_DLG_LOG( "펫데미지 %d", damage );
				}
				attack_event.critical = aMsg->theCritical;	
				attack_event.kill = aMsg->theDieFlag;		
				
				if( attack_event.kill )
				{
					attack_event.damage = aMsg->theAttackedHP;									
					g_Pc.m_MainPC.char_info.theRealHP = 0;
					g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
					g_Pc.GetPlayer()->SetRealDie(TRUE, n_NPC, aMsg->thePetID);
				}
			}	
			
		}
		else	///-- 일반 유저가 맞을때	
		{
			attackedIdx = g_Pc.FindUser(aMsg->theAttackedID);
			if( attackedIdx == MAX_USER_INTABLE )
			{
				return;
			}
			
			attack_event.tbl_Idx = attackedIdx;
			attack_event.race	= g_Pc.m_PcTable[attackedIdx].char_info.race;
			attack_event.index	= g_Pc.m_PcTable[attackedIdx].nIndex;
			attack_event.unique = g_Pc.m_PcTable[attackedIdx].lUnique;
			attack_event.x		= g_Pc.m_PcTable[attackedIdx].lDestBlock%(g_Map.m_cSize*2);
			attack_event.y		= g_Pc.m_PcTable[attackedIdx].lDestBlock/(g_Map.m_cSize*2);
			attack_event.vPos.x = g_Pc.m_Pc[attackedIdx].GetPosTM()._41;
			attack_event.vPos.y = g_Pc.m_Pc[attackedIdx].GetPosTM()._42;
			attack_event.vPos.z = g_Pc.m_Pc[attackedIdx].GetPosTM()._43;
			
			bool bFlag = true;
			
			if(g_Pc.m_Pc[attackedIdx].m_State.nPetState == nPetState_Master)
			{
				if( g_Map.m_NpcTable[g_Pc.m_Pc[attackedIdx].m_State.PetIndex].lUnique == aMsg->thePetID )
				{
					bFlag  = false;
				}	
			}
			
			if(bFlag)
			{
				int damage = g_Pc.m_PcTable[attackedIdx].char_info.theRealHP - aMsg->theAttackedHP; 
				g_Pc.m_PcTable[attackedIdx].char_info.theRealHP = aMsg->theAttackedHP;
				
				if( damage < 0 ) 
				{ 
					attack_event.damage = 0; 
				}	
				else
				{ 
					attack_event.damage = damage;					
				}				
				
				g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
				
				attack_event.critical = aMsg->theCritical;		
				attack_event.kill = aMsg->theDieFlag;		// 죽어 있으면 한번만 애니메이션
				
				if( g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master )
				{
					//내 펫이면

					if( aMsg->thePetID == g_Map.m_NpcTable[g_Pc.GetPlayer()->m_State.PetIndex].lUnique )
					{
						if( (damage > 0 )  && !attack_event.kill)
						{
							if( g_PetInfoWnd.m_CurPetNum == MERCE_SLOT_NUM )							
							{
								SystemPrint( n_DarkGreen, n_SystemMsg, 
								 G_STRING_CHAR( IDS_SOLDIER_ATTACK_DAMAGE ), damage );		
							}
							else
							{
								SystemPrint( n_DarkGreen, n_SystemMsg, 
								 G_STRING_CHAR( IDS_PET_ATTACK_DAMAGE ), damage );
							}
						}
					}				
				}
				
				if( attack_event.kill )
				{
					attack_event.damage = aMsg->theAttackedHP;
					g_Pc.m_PcTable[attackedIdx].char_info.theRealHP = 0;
					g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
					g_Pc.m_Pc[attackedIdx].SetRealDie(TRUE);
				}
			}
		}
	}
	///-- NPC가 맞을 때
	else if( aMsg->theAttackedType == n_NPC )
	{
		attackedIdx = g_Map.FindNpc(aMsg->theAttackedID);
		if( attackedIdx == MAX_NPC_INTABLE )
		{	
			return;
		}
		
		attack_event.tbl_Idx = attackedIdx;
		attack_event.race	= g_Map.m_NpcTable[attackedIdx].race;
		attack_event.index	= g_Map.m_NpcTable[attackedIdx].index;
		attack_event.unique	= g_Map.m_NpcTable[attackedIdx].lUnique;
		attack_event.x		= g_Map.m_NpcTable[attackedIdx].lDestBlock%(g_Map.m_cSize*2);
		attack_event.y		= g_Map.m_NpcTable[attackedIdx].lDestBlock/(g_Map.m_cSize*2);
		attack_event.vPos.x = g_Map.m_Npc[attackedIdx].GetPosTM()._41;
		attack_event.vPos.y = g_Map.m_Npc[attackedIdx].GetPosTM()._42;
		attack_event.vPos.z = g_Map.m_Npc[attackedIdx].GetPosTM()._43;		
		
		int damage = g_Map.m_NpcTable[attackedIdx].theRealHP - aMsg->theAttackedHP;	
		if( damage < 0 ) 
		{ 
			attack_event.damage = 0; 
		}	
		else 
		{ 
			attack_event.damage = damage; 			
		}
		
		g_Map.m_NpcTable[attackedIdx].theRealHP = aMsg->theAttackedHP;
		g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;
		
		attack_event.critical = aMsg->theCritical;		
		attack_event.kill = aMsg->theDieFlag;		// 죽어 있으면 한번만 애니메이션
		
		if( g_Pc.GetPlayer()->m_State.nPetState == nPetState_Master )
		{
			//내 펫이면

			if( aMsg->thePetID == g_Map.m_NpcTable[g_Pc.GetPlayer()->m_State.PetIndex].lUnique )
			{
				if( (damage > 0 ) && !attack_event.kill )
				{
					if( g_PetInfoWnd.m_CurPetNum == MERCE_SLOT_NUM )							
					{
						SystemPrint( n_DarkGreen, n_SystemMsg, 
						 G_STRING_CHAR( IDS_SOLDIER_ATTACK_DAMAGE ), damage );		///-- HAN_MSG
					}
					else
					{
						SystemPrint( n_DarkGreen, n_SystemMsg, 
						 G_STRING_CHAR( IDS_PET_ATTACK_DAMAGE ), damage );
					}
				}
			}				
		}
		
		if( attack_event.kill )
		{
			attack_event.damage = aMsg->theAttackedHP;
			g_Map.m_NpcTable[attackedIdx].theRealHP = 0;
			g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;
			g_Map.m_Npc[attackedIdx].SetRealDie(TRUE);
			
			///-------------------------------------------------------------------------
			///-- Monster ChattingBox	
			///-- 2005.03.07 / Lyul / 죽었을 때			
			RLGS("-- Monster Chatting Box : Monster Died by OtherUser!");
			//	if( ( (float)rand() / (float)RAND_MAX) > nRui->m_BugMop.fMopDie  )
			{
				SetMonsterMessage( aMsg->theAttackedID, n_Monster_Die );			
			}
			///-------------------------------------------------------------------------	
		}		
		
	}
	attackIdx = g_Map.FindNpc(aMsg->thePetID);
	///-- 공격 주체 펫이 존재하지 않을 경우

	if( attackIdx == MAX_NPC_INTABLE )
	{
		if( aMsg->theAttackedType == n_PC )
		{
			SetProcessAttackEvent( attack_event, FALSE, TRUE, n_NPC,
					aMsg->thePetID, aMsg->theAttackedType, attackedIdx );
			
			g_RockClient.Send_RpCsReqCharInfo( aMsg->thePetID, n_NPC );
			return;
		}
		else if( aMsg->theAttackedType == n_NPC )
		{
			SetProcessAttackEvent( attack_event, FALSE, TRUE, n_NPC,
					aMsg->thePetID, aMsg->theAttackedType, attackedIdx );
			
			g_RockClient.Send_RpCsReqCharInfo( aMsg->thePetID, n_NPC );
			return;
		}
		
	}		
	///-- 공격 주체 펫이 이미 죽어있을 경우
	///--JAPAN_BUG_MODIFY	
	if( g_Map.m_Npc[attackIdx].IsRealDie() )
	{
		if( aMsg->theAttackedType == n_PC )
		{
			SetProcessAttackEvent( attack_event, FALSE, TRUE, n_NPC,
					aMsg->thePetID, aMsg->theAttackedType, attackedIdx );
			
			return;
		}
		else if( aMsg->theAttackedType == n_NPC )
		{
			SetProcessAttackEvent( attack_event, FALSE, TRUE, n_NPC,
					aMsg->thePetID, aMsg->theAttackedType, attackedIdx );
			
			return;
		}
	}
	
	if( attack_event.critical )
	{
		attack_event.motion = n_NPC_Attack1;		
	}
	else
	{
		attack_event.motion = n_NPC_Attack0;		
	}	
	
	if( g_Map.m_Npc[attackIdx].m_State.nCharacterType == nCharacterTypeNormal )
	{
		if( attack_event.critical )
		{
			attack_event.motion = n_NPC_Attack1;		
		}
		else
		{
			attack_event.motion = n_NPC_Attack0;		
		}
	}
	else if( g_Map.m_Npc[attackIdx].m_State.nCharacterType == nCharacterTypePlayerNPC )
	{
		///-- 공격 클래스가 아쳐나 스나이퍼 라면

		if( g_Map.m_NpcTable[attackIdx].theExPcInfo.theMainClass == Archer ||
			g_Map.m_NpcTable[attackIdx].theExPcInfo.theMainClass == Sniper )
		{
			attack_event.motion = n_Attack10_BowReady0;				
		}
		else
		{
			if( attack_event.critical )
			{
				attack_event.motion = n_Attack9_Basic4;		
			}
			else
			{
				attack_event.motion = n_Attack9_Basic0;		
			}	
		}
	}
	
	///--PRINT_DLG_LOG("ScPetAttack - PushQueue");
	g_Map.m_Npc[attackIdx].PushQueue(attack_event);	
}

void CRockClient::Proc_RpScRideOn( SRpScRideOn * aMsg )
{
#ifdef RIDING_SYSTEM
	
	if( aMsg->thePCID == g_Pc.m_MainPC.lUnique )
	{
		if( GetItemProtocolResult(aMsg->theResult)	)
		{
			g_Particle.SetEffSetCon( 0.0f , EF_RIDE_ON , ESLT_CHARPOS , 5.1f , g_Pc.GetPlayer()->GetUnique(),
				g_Pc.GetPlayer()->GetIndex() );
			
			//라이딩 할시 이펙트 보여주기
			
			SetRideOn( MAX_USER_INTABLE );
			
			g_DelayBarWnd.Close();		
		}
		else 
		{
			g_DelayBarWnd.Close();	
		}
/*
		if(aMsg->theResult == n_Success)
		{
	
		}
		else
		{
		
			SystemPrint( n_DarkRed, n_SystemMsg,  G_STRING_CHAR(IDS_SYSPR_NOT_USE_HORSE) ) ;
		}
*/
		
	}
	else
	{
		int nPcIndex = g_Pc.FindUser(aMsg->thePCID);		
		if( MAX_USER_INTABLE != nPcIndex )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_RIDE_ON , ESLT_CHARPOS , 5.1f , g_Pc.m_Pc[nPcIndex].GetUnique(),
				g_Pc.m_Pc[nPcIndex].GetIndex() );
			
			g_Pc.m_PcTable[nPcIndex].theRidingUID = aMsg->theRidingUID;			
			
			SetRideOn( nPcIndex );		
		}
	}	
	
#endif
	
}

void CRockClient::Proc_RpScRideOff( SRpScRideOff * aMsg )
{
#ifdef RIDING_SYSTEM
	if( aMsg->thePCID == g_Pc.m_MainPC.lUnique )
	{
		g_Particle.SetEffSetCon( 0.0f , EF_RIDE_OFF , ESLT_CHARPOS , 5.1f , g_Pc.GetPlayer()->GetUnique(),
			g_Pc.GetPlayer()->GetIndex() );		
		//라이딩 종료 할시 이펙트 보여주기

		SetRideOff( MAX_USER_INTABLE);
		
		//	g_Pc.m_MainPC.lDestBlock = g_Pc.m_MainPC.lBlock;
		
		g_RockClient.Send_RpCsStop( g_Pc.m_MainPC.lBlock, g_Pc.GetPlayer()->m_Mov.now_Yrad);
		//	g_RockClient.Send_RpCsMove( g_Pc.m_MainPC.lUnique, g_Pc.m_MainPC.lBlock, m_Mov.now_Yrad );
	}
	else
	{
		int nPcIndex = g_Pc.FindUser(aMsg->thePCID);
		if( MAX_USER_INTABLE != nPcIndex )
		{
			g_Particle.SetEffSetCon( 0.0f , EF_RIDE_OFF , ESLT_CHARPOS , 5.1f , g_Pc.m_Pc[nPcIndex].GetUnique() ,
				g_Pc.m_Pc[nPcIndex].GetIndex() );
			SetRideOff( nPcIndex );
			
		}
	}
#endif
	
}

void CRockClient::Proc_ScTotemScore( SRpScTotemScore * aMsg)
{
	//BYTE				theMillena;
	//BYTE				theRain;
	//한번이라두 정보가 날라오면 보여줘라 .. 
	if(!nRui->GetPcInvenInfo()->TotemScore.bShow)
	{
		nRui->GetPcInvenInfo()->TotemScore.bShow = true; 
	}
	nRui->GetPcInvenInfo()->TotemScore.Millena = aMsg->theMillena;
	nRui->GetPcInvenInfo()->TotemScore.Rain = aMsg->theRain;
}

void CRockClient::Proc_RpScAskMove( SRpScAskMove * aMsg )
{
	g_UIMsgWnd.SetParam( aMsg->thePCName );
	g_UIMsgWnd.SetMsgType(MSG_CALL_ASK);
	nRui->SendEvent( WID_UIMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}


///-- 공격 주체가 사라지거나 죽어있어서 애니메이션 처리가 불가능할때 직접적인 대미지를 처리하는 루틴
void CRockClient::SetProcessAttackEvent( EVENT_DATA_INFO & data_event, 
									     BOOL IsPlayer , BOOL IsAttackedDest, 
										 int nAttackType, DWORD dAttackID,
										 int nAttackedType, int nAtedIndex )
{
	///-- 플레이어 대상 관련 공격일때
	if(IsPlayer)
	{
		if(data_event.kill)
		{
			EVENT_DATA_INFO underattack_event;
			underattack_event.type = SM_STATE_DIE;
			underattack_event.loop = false;
			
			///--SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_DIE_COUNTER ) );
			underattack_event.motion = n_Die9;
			g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
			nRui->thePcParam.Stat1.theHP = 0;
			g_Pc.m_MainPC.char_info.theHP = 0;
			g_Pc.m_MainPC.char_info.theRealHP = 0;
			g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			g_Pc.GetPlayer()->SetRealDie(TRUE, nAttackType, dAttackID);
		}
		else
		{
			if( data_event.damage && 
				nRui->thePcParam.Stat1.theHP == g_Pc.m_MainPC.char_info.theRealHP )
			{
				return;
			}
			
			///--JAPAN_BUG_MODIFY
			///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
			if( !(g_Pc.GetPlayer()->IsRealDie()) )
			{
				nRui->thePcParam.Stat1.theHP -= data_event.damage;
				g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
			}
		}
	}
	///-- 플레이어 이외 대상을 공격할때
	else if(IsAttackedDest)
	{
		if( nAttackedType == n_PC )
		{
			if( data_event.kill )
			{
				EVENT_DATA_INFO underattack_event;
				underattack_event.type = SM_STATE_DIE;
				underattack_event.loop = false;
				
				underattack_event.motion = n_Die9;
				g_Pc.m_Pc[nAtedIndex].PushQueue(underattack_event, TRUE);	
				g_Pc.m_PcTable[nAtedIndex].char_info.theHP = 0;
				g_Pc.m_PcTable[nAtedIndex].char_info.theRealHP = 0;
				g_Pc.m_Pc[nAtedIndex].m_lPreTime = g_nowTime;
				g_Pc.m_Pc[nAtedIndex].SetRealDie(TRUE);
			}
			else
			{
				if( data_event.damage && 
					g_Pc.m_PcTable[nAtedIndex].char_info.theHP 
					== g_Pc.m_PcTable[nAtedIndex].char_info.theRealHP )
				{
					return;
				}				
				
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
				if( !(g_Pc.m_Pc[nAtedIndex].IsRealDie()) )
				{
					g_Pc.m_PcTable[nAtedIndex].char_info.theHP -= data_event.damage;
					g_Pc.m_Pc[nAtedIndex].m_lPreTime = g_nowTime;
				}	
			}
		}
		else if(nAttackedType == n_NPC)
		{
			if( data_event.kill )
			{
				EVENT_DATA_INFO underattack_event;
				underattack_event.type = SM_STATE_DIE;
				underattack_event.loop = false;
				
				if( g_Map.m_Npc[nAtedIndex].m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					underattack_event.motion = n_Die9;
				}
				else
				{
					underattack_event.motion = n_NPC_Die0 + rand()%2;
				}		
				
				g_Map.m_Npc[nAtedIndex].PushQueue(underattack_event, TRUE);	
				g_Map.m_NpcTable[nAtedIndex].theHP = 0;
				g_Map.m_NpcTable[nAtedIndex].theRealHP = 0;
				g_Map.m_Npc[nAtedIndex].m_lPreTime = g_nowTime;
				g_Map.m_Npc[nAtedIndex].SetRealDie(TRUE);
			}
			else
			{
				if( data_event.damage && 
					g_Map.m_NpcTable[nAtedIndex].theHP 
					== g_Map.m_NpcTable[nAtedIndex].theRealHP )
				{
					return;
				}
				
				///--JAPAN_BUG_MODIFY
				///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
				if( !(g_Map.m_Npc[nAtedIndex].IsRealDie()) )
				{
					g_Map.m_NpcTable[nAtedIndex].theHP -= data_event.damage;
					g_Map.m_Npc[nAtedIndex].m_lPreTime = g_nowTime;
				}
			}
		}
	}
	
	///-- 다수의 대상을 공격할때
	if( data_event.number )
	{
		int tgNum = 0;
		int damage = 0; 
		int attackedIdx = 0;
		SAttacked* tg_Info = (SAttacked*) data_event.target_list;
		
		if( tg_Info )
		{
			for( tgNum = 0; tgNum < data_event.number; ++tgNum )
			{
				if( tg_Info[tgNum].theAttackedType == n_PC )
				{
					if( tg_Info[tgNum].theAttackedID == g_Pc.m_MainPC.lUnique )				
					{
						if( tg_Info[tgNum].theDieFlag )
						{
							EVENT_DATA_INFO underattack_event;
							underattack_event.type = SM_STATE_DIE;
							underattack_event.loop = false;
							underattack_event.motion = n_Die9;
							g_Pc.GetPlayer()->PushQueue( underattack_event, TRUE );
							nRui->thePcParam.Stat1.theHP = 0;
							g_Pc.m_MainPC.char_info.theHP = 0;
							g_Pc.m_MainPC.char_info.theRealHP = 0;
							g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
							g_Pc.GetPlayer()->SetRealDie(TRUE, nAttackType, dAttackID);
						}
						else
						{
							if( tg_Info[tgNum].theAttackedHP && 
								nRui->thePcParam.Stat1.theHP == g_Pc.m_MainPC.char_info.theRealHP )
							{
								continue;
							}							
							
							///--JAPAN_BUG_MODIFY
							///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
							if( !(g_Pc.GetPlayer()->IsRealDie()) )
							{
								nRui->thePcParam.Stat1.theHP -= tg_Info[tgNum].theAttackedHP;
								g_Pc.GetPlayer()->m_lPreTime = g_nowTime;
							}
						}
					}				
					else
					{
						attackedIdx = g_Pc.FindUser( tg_Info[tgNum].theAttackedID );		
						if( attackedIdx != MAX_USER_INTABLE )
						{
							if( tg_Info[tgNum].theDieFlag )
							{
								EVENT_DATA_INFO underattack_event;
								underattack_event.type = SM_STATE_DIE;
								underattack_event.loop = false;
								underattack_event.motion = n_Die9;
								g_Pc.m_Pc[attackedIdx].PushQueue( underattack_event, TRUE );
								g_Pc.m_PcTable[attackedIdx].char_info.theHP = 0;
								g_Pc.m_PcTable[attackedIdx].char_info.theRealHP = 0;
								g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
								g_Pc.m_Pc[attackedIdx].SetRealDie(TRUE);
							}
							else
							{
								if( tg_Info[tgNum].theAttackedHP && 
									g_Pc.m_PcTable[attackedIdx].char_info.theHP 
									== g_Pc.m_PcTable[attackedIdx].char_info.theRealHP )
								{
									continue;
								}								
								
								///--JAPAN_BUG_MODIFY
								///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
								if( !(g_Pc.m_Pc[attackedIdx].IsRealDie()) )
								{
									g_Pc.m_PcTable[attackedIdx].char_info.theHP -= tg_Info[tgNum].theAttackedHP;
									g_Pc.m_Pc[attackedIdx].m_lPreTime = g_nowTime;
								}
							}
						}
					}				
				}
				else if( tg_Info[tgNum].theAttackedType == n_NPC )
				{
					attackedIdx = g_Map.FindNpc( tg_Info[tgNum].theAttackedID );							
					
					if( attackedIdx != MAX_NPC_INTABLE )
					{			
						if( tg_Info[tgNum].theDieFlag )
						{
							EVENT_DATA_INFO underattack_event;
							underattack_event.type = SM_STATE_DIE;
							underattack_event.loop = false;
							
							if( g_Map.m_Npc[attackedIdx].m_State.nCharacterType == nCharacterTypePlayerNPC )
							{
								underattack_event.motion = n_Die9;
							}
							else
							{							
								underattack_event.motion = n_NPC_Die0;
							}
							g_Map.m_Npc[attackedIdx].PushQueue( underattack_event, TRUE );
							g_Map.m_NpcTable[attackedIdx].theHP = 0;
							g_Map.m_NpcTable[attackedIdx].theRealHP = 0;
							g_Map.m_Npc[attackedIdx].SetRealDie(TRUE);
							g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;
						}
						else
						{
							if( tg_Info[tgNum].theAttackedHP && 
								g_Map.m_NpcTable[attackedIdx].theHP 
								== g_Map.m_NpcTable[attackedIdx].theRealHP )
							{
								continue;
							}	
							
							///--JAPAN_BUG_MODIFY
							///-- by simwoosung 현재 죽어있지 않는경우에만 적용시킨다.
							if( !(g_Map.m_Npc[attackedIdx].IsRealDie()) )
							{
								g_Map.m_NpcTable[attackedIdx].theHP -= tg_Info[tgNum].theAttackedHP;
								g_Map.m_Npc[attackedIdx].m_lPreTime = g_nowTime;
							}
						}
					}
				}		
			}		
		}	
	}	
}


void CRockClient::Proc_RpScDenyMessage( SRpScDenyMessage * aMsg)
{
	if( aMsg->theResult == n_Success)
	{
		SystemPrint( n_LightBlue, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_AURICULR_BLOCK));		
	}
	
}

void CRockClient::Proc_RpScIndunTime(SRpScIndunTime * aMsg)
{
	
	//카운트가 값이 들어오면 그때부터 뿌려줌 .. 
	PlayerInfoWnd.InitFameInDun();
	nRui->GetPcInvenInfo()->InDunData.iStartCount =  aMsg->theRemainedTime; 
	nRui->GetPcInvenInfo()->InDunData.dwStartTime = g_nowTime;

}

void CRockClient::Proc_RpScMakeSkillPotion(SRpScMakeSkillPotion * aMsg)
{

	int i =0 , j = 0 ; 
	if( aMsg->theResult == n_FailSetStone )
	{
		for( i = 0; i < MAKESKILLPOTIONNUM ; ++i )
		{
			for( j = 0; j < c_MaxInvenItemNum; ++j )
			{	
				if( m_PcInven->InvenItem[j] == NULL )
				{
					continue;
				}
				if( aMsg->theDeletePotionElement[i].theItemID == m_PcInven->InvenItem[j]->ItemUID )
				{
					//판매후 남은 번들 수. 0 인 경우 아이템 삭제.
					m_PcInven->InvenItem[j]->Amount = aMsg->theDeletePotionElement[i].theBundle;
					
					if( m_PcInven->InvenItem[j]->Amount == 0 )
					{
						//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
						for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
						{
							if( m_PcInven->InvenItem[j] == m_PcInven->QuickSlot[idx] )
							{
								m_PcInven->QuickSlot[idx] = NULL;
								break;
							}
						}
						
						delete( m_PcInven->InvenItem[j] );
						m_PcInven->InvenItem[j] = NULL;
					}					
					//슬롯에 적용....
					MainMenuWnd.UpdateQuickSlotItem();
					InventoryWnd.UpdateInvenSlotItem();	
				}
			}			
		}
		m_PcParam->Money = aMsg->thePCMoney;
		InventoryWnd.UpdateInvenSlotItem();		
		
		g_ItemSkillPotionRWndProc.OpenEnchantR_FWnd();
		return;
	}
	else 
		if( GetItemProtocolResult(aMsg->theResult)) 
		{
			int Pos =0 ; 

			for( j = 0; j < c_MaxInvenItemNum; ++j )
			{	
				Pos = -1;

				if( m_PcInven->InvenItem[j] == NULL )
				{
					continue;
				}
				
				if(aMsg->theDeletePotionElement[0].theItemID == m_PcInven->InvenItem[j]->ItemUID) Pos= 0 ;
				else if(aMsg->theDeletePotionElement[1].theItemID == m_PcInven->InvenItem[j]->ItemUID) Pos= 1 ; 
				else if(aMsg->theDeletePotionElement[2].theItemID == m_PcInven->InvenItem[j]->ItemUID) Pos= 2 ;

				if( Pos != -1)
				{
					m_PcInven->InvenItem[j]->Amount = aMsg->theDeletePotionElement[Pos].theBundle;

					if( m_PcInven->InvenItem[j]->Amount == 0 )
					{
						//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
						for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
						{
							if( m_PcInven->InvenItem[j] == m_PcInven->QuickSlot[idx] )
							{
								m_PcInven->QuickSlot[idx] = NULL;
								break;
							}
						}
					
						delete( m_PcInven->InvenItem[j] );
						m_PcInven->InvenItem[j] = NULL;
					}					
				}
			}			
			
			//인벤에서 삭제 시켜라 .. 
			// 총 3개 ? 아이템 ... ㅋㅋ 
			///-- 인벤 체크 
			SPcItem*	tmpPcItem = NULL;
			CreateItemPcItem(&tmpPcItem , aMsg->theSkillPotion );
						
			CItemTransButton *pItemTransButton = g_main_buttons.GetInVenButton();
			SItemTransInfo ItemTransInfo;
			ItemTransInfo.Is3DPos = false ;
			ItemTransInfo.interNpcId = 0;
			ItemTransInfo.isMoney = false;
			ItemTransInfo.isGet = true;
			ItemTransInfo.pcInfo = *tmpPcItem;			
			ItemTransInfo.isMonster = false;
			pItemTransButton->InsertItemTransInfo(ItemTransInfo);
						
			//자리를 찾는다 . 
			//그리고 이벤엔 집어 넣는다. 
			//서버에서 자리를 받아와야 하낟.. 지금음 테스팅 .. ㅠㅠ
			m_PcInven->InvenItem[aMsg->theSkillPotion.theItem.thePosition] = tmpPcItem;

			m_PcParam->Money = aMsg->thePCMoney;
			
			//슬롯에 적용....
			MainMenuWnd.UpdateQuickSlotItem();
			InventoryWnd.UpdateInvenSlotItem();	
			
			g_ItemSkillPotionRWndProc.OpenEnchantR_SWnd(aMsg->theSkillPotion.theItem.theItemUID);
		}
}

//Colony 관련
// 콜로니의 리스트 (맵번호만)
void CRockClient::Proc_RpScColonyList(SRpScColonyList * aMsg)
{
	g_ColRegionListWnd.ClearColAreaDataList();
	g_ColRegionListWnd.m_nLinkColonyCount = aMsg->theColonyNum;
	
	for(int i = 0 ; i < g_ColRegionListWnd.m_nLinkColonyCount ; i++)
	{
		g_ColRegionListWnd.m_sCAreaData[i].aColonyCode = aMsg->theColonyCode[i];
	}
}

// 상세한 정보 포함. 한번 받은 것은 저장하고 다시 요청하지 않기를
void CRockClient::Proc_RpScAnsColonyInfo(SRpScAnsColonyInfo * aMsg)
{
	for(int i = 0 ; i < g_ColRegionListWnd.m_nLinkColonyCount ; i++)
	{
		if( g_ColRegionListWnd.m_sCAreaData[i].aColonyCode == aMsg->theColonyCode )
		{
			if(g_ColRegionListWnd.m_sCAreaData[i].pColAreaDetailInfo == NULL)
			{
				g_ColRegionListWnd.m_sCAreaData[i].pColAreaDetailInfo = SAFE_NEW( SColAreaDetailInfo );			
			}
			
			g_ColRegionListWnd.m_sCAreaData[i].pColAreaDetailInfo->aColonyCode = aMsg->theColonyCode;
			SAFE_STR_CPY( g_ColRegionListWnd.m_sCAreaData[i].pColAreaDetailInfo->strFrontierName, aMsg->theFrontierName , RP_MAX_COMBINED_FRONTIERNAME );
			SAFE_STR_CPY( g_ColRegionListWnd.m_sCAreaData[i].pColAreaDetailInfo->strMasterPCName, aMsg->theMasterPCName , RP_MAX_PC_NAME );
			g_ColRegionListWnd.m_sCAreaData[i].pColAreaDetailInfo->aMemberNum = aMsg->theMemberNum;
			g_ColRegionListWnd.m_sCAreaData[i].pColAreaDetailInfo->aWarTime = aMsg->theWarTime;
			SAFE_STR_CPY( g_ColRegionListWnd.m_sCAreaData[i].pColAreaDetailInfo->strAttackFName, aMsg->theAttackFName , RP_MAX_COMBINED_FRONTIERNAME );
			
			break;
		}			
	}
	
	if( g_ColRegionInfoWnd.IsVisible && 
		g_ColRegionInfoWnd.m_pLinkColAreaData && 
		g_ColRegionInfoWnd.m_pLinkColAreaData->pColAreaDetailInfo )
	{
		g_ColRegionInfoWnd.RefreshColRegionInfo();
	}	
}

// 선전포고 결과

void CRockClient::Proc_RpScDeclareWar(SRpScDeclareWar * aMsg)
{
	TCHAR sTrMessage[256] = {0,};
	TCHAR sTrMapName[256] = {0,};
	TCHAR sTrMapDate[256] = {0,};
	
	::GetAreaStrName(sTrMapName, (UINT)(aMsg->theColonyCode));
	::ConvertTimeToString(aMsg->theWarTime, sTrMapDate);
	
	Rsprintf(RWCHAR(sTrMessage), _RT("%s콜로니에서 %s 선전포고 되었습니다."), RWCHAR(sTrMapName), RWCHAR(sTrMapDate));		///-- HAN_MSG
	g_ChatMainProc.InsertChatContent(n_AnnounChat, _RT(""), sTrMessage);		
}

// 선전포고 취소 결과 
void CRockClient::Proc_RpScCancelWar(SRpScCancelWar * aMsg)
{  
	TCHAR sTrMessage[256] = {0,};
	TCHAR sTrMapName[256] = {0,};
	
	::GetAreaStrName(sTrMapName, (UINT)(aMsg->theColonyCode));
	Rsprintf(RWCHAR(sTrMessage), _RT("%s콜로니에서 선전포고 취소되었습니다."), RWCHAR(sTrMapName));			///-- HAN_MSG
	g_ChatMainProc.InsertChatContent(n_AnnounChat, _RT(""), sTrMessage);	
}

// 선전포고에 대한 안내방송
void CRockClient::Proc_RpScWarAnnounce(SRpScWarAnnounce * aMsg)
{
	memset(g_strMessage , NULL , MAX_SIZE_512 );

	TCHAR sTrMapName[256] = {0,};
	TCHAR sTrMapDate[256] = {0,};
	
	::GetAreaStrName(sTrMapName, (UINT)(aMsg->theColonyCode));
	::ConvertTimeToString(aMsg->theWarTime, sTrMapDate);
	
	Rsprintf( RWCHAR(g_strMessage), _RT("%s콜로니의 %s 프론티어에게 %s 프론티어가 %s 선전포고 하였습니다."), RWCHAR(sTrMapName), RWCHAR(aMsg->theOwnerFrontierName), 
		RWCHAR(aMsg->theDeclareWarFrontierName), sTrMapDate);		///-- HAN_MSG
	
	g_ChatMainProc.InsertChatContent(n_AnnounChat, _RT(""), g_strMessage);		
}

// 콜로니맵에 입장시 상세 정보 전송. (성벽구성 등)
void CRockClient::Proc_RpScColonyInfo(SRpScColonyInfo * aMsg)
{
	if( G_MAP_MAPTYPE(aMsg->theColonyCode) == MAP_TYPE_COLONY )
	{
		g_Map.m_nExColInfo.m_nMapIndex = (int)aMsg->theBuildType;
	}
}

void CRockClient::Proc_RpScMixEldaStone(SRpScMixEldaStone* aMsg)
{
	
	/*

  BYTE			theResult;								// n_Success, n_Fail
  URpdItem		theMixEldaStone;						// 성공시 생성된 엘다스톤
  DWORD			theEld;									// 혼합수수료 지불후 남은 잔금 
  SBundleItem		theEldaStone[MIX_ELDA_ITEM_NUM];		// 처리후 남은 엘다스톤 정보 
	SBundleItem		thePayItem[RP_MAX_MIXELDA_PAY];			// 처리후 관련 유료아이템 정보

  n_ITEMERR_Need_SameGrade_Eldastone, 
	n_ITEMERR_Check_MixEldastone_Condition, 
	n_ITEMERR_FAILMIXELDASTONE
	n_ITEMERR_MIXELDA_SETSEQUENCELV
	*/
#ifdef MIX_ELDASTONE
	if( aMsg->theResult == n_ITEMERR_FAILMIXELDASTONE  )
	{
		///-- 전체 인벤토리 Access
		for(int i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}
		
			///-- 각 유료 아이템 별
			for( int j = 0 ; j < RP_MAX_MIXELDA_PAY ; ++j )
			{
				if ( m_PcInven->InvenItem[i] && 
					m_PcInven->InvenItem[i]->ItemUID == aMsg->thePayItem[j].theItemID ) 
				{
					///-- Amount 변경
					m_PcInven->InvenItem[i]->Amount = aMsg->thePayItem[j].theBundle;
					
					///-- 아이템 완전 삭제
					if( m_PcInven->InvenItem[i]->Amount < 1 )
					{	
						SAFE_DELETE( m_PcInven->InvenItem[i] );
						m_PcInven->InvenItem[i] = NULL;
						continue;
					}
				}			
			}

			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}


			///-- 엘다스톤 삭제
			if (
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[0].theItemID )||
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[1].theItemID  )||
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[2].theItemID  )||
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[3].theItemID  )||
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[4].theItemID  )||
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[5].theItemID  )
				) 
			{
				///-- 아이템 완전 삭제
				if( m_PcInven->InvenItem[i]->Amount <=1 )
				{	
					delete( m_PcInven->InvenItem[i] );
					m_PcInven->InvenItem[i] = NULL;
				}
				///-- 번들 개수 줄임
				else
				{
					--( m_PcInven->InvenItem[i]->Amount );
				}
			}
		}			
	

		m_PcParam->Money = aMsg->theEld;
		InventoryWnd.UpdateInvenSlotItem();		
		g_ItemEldaMixRWndProc.FrameWnd->SetCaption(G_STRING_CHAR(IDS_WND_ELDASTONE_MIX)); 
		g_ItemEldaMixRWndProc.OpenEnchantR_FWnd(CItemEldaMixRWndProc::eEldaMix);
		
		return;
	}
	else if( GetItemProtocolResult(aMsg->theResult)) 
	{
		
		///-- 전체 인벤토리 Access
		for(int i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}
				
			///-- 각 유료 아이템 별
			for( int j = 0 ; j < RP_MAX_MIXELDA_PAY ; ++j )
			{
				if ( m_PcInven->InvenItem[i] && 
					m_PcInven->InvenItem[i]->ItemUID == aMsg->thePayItem[j].theItemID ) 
				{
					///-- Amount 변경
					m_PcInven->InvenItem[i]->Amount = aMsg->thePayItem[j].theBundle;
					
					///-- 아이템 완전 삭제
					if( m_PcInven->InvenItem[i]->Amount < 1 )
					{	
						SAFE_DELETE( m_PcInven->InvenItem[i] );	
						m_PcInven->InvenItem[i] = NULL;
					}
				}			
			}

			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}


			///-- 엘다스톤 삭제
			if (
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[0].theItemID )||
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[1].theItemID  )||
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[2].theItemID  )||
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[3].theItemID  )||
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[4].theItemID  )||
				(m_PcInven->InvenItem[i]->ItemUID == aMsg->theEldaStone[5].theItemID  )
				) 
			{
				///-- 아이템 완전 삭제
				if( m_PcInven->InvenItem[i]->Amount <=1 )
				{	
					delete( m_PcInven->InvenItem[i] );
					m_PcInven->InvenItem[i] = NULL;
				}
				///-- 번들 개수 줄임
				else
				{
					--( m_PcInven->InvenItem[i]->Amount );
				}
			}
		}			
				
		m_PcParam->Money =  aMsg->theEld ;
		InventoryWnd.UpdateInvenSlotItem();
		
		g_ItemEldaMixRWndProc.FrameWnd->SetCaption(G_STRING_CHAR(IDS_WND_ELDASTONE_MIX)); 
		g_ItemEldaMixRWndProc.OpenEnchantR_SWnd(aMsg->theMixEldaStone , CItemEldaMixRWndProc::eEldaMix );			
		
	}	

#endif
		
}

//중국 PK 관련부분 작업

void CRockClient::Proc_RpScDeletedPKAttackUser(SRpScDeletedPKAttackUser *aMsg)
{
	if( aMsg->theAttackUserPCID ) 
	{
		if( g_Pc.GetPlayer()->DeleteMyAttackerList( aMsg->theAttackUserPCID ) )
		{
			int nPcIndex = g_Pc.FindUser(aMsg->theAttackUserPCID);		
			if( MAX_USER_INTABLE != nPcIndex )
			{
				SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_END_BEFOREATTACK) , g_Pc.m_PcTable[nPcIndex].char_info.thePCName ); 
				///-- 여기서 선제공격자 표시를 지워준다.
				g_Particle.m_EffectManager.Delete( EF_BEFORE_ATTACKER_MARK , 
										   ( Character * )&g_Pc.m_Pc[nPcIndex] , NULL );
			}
		}
	}
}

void CRockClient::Proc_RpScPKAttack(SRpScPKAttack *aMsg)
{
	//-- 나만....

	BYTE bPKFlag = g_Pc.GetPlayer()->GetPKMOde();
	
	if( bPKFlag != aMsg->thePKFlag)
	{
		switch(aMsg->thePKFlag)
		{
			case n_PKOFFMode:
			{ 
				SystemPrint( n_LightYellow, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_PKMODE_NORMAL) ); 
			}
			break;
		case n_PKONMode:
			{
				SystemPrint( n_LightYellow, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_PKMODE_PK) ); 				
			}
			break;
		case n_DPKMode:
			{
				SystemPrint( n_LightYellow, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_PKMODE_DEFENCE) ); 
			}
			break;
		case n_APKMode:
			{
				SystemPrint( n_LightYellow, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_PKMODE_ATTACK) ); 
			}
			break;
		}
	}

	g_Pc.GetPlayer()->SetPKMode(aMsg->thePKFlag);

	int nBeforePkPoint = nRui->GetPcParam()->thePKPoint;
	int nNowPkPoint = aMsg->thePKPoint;

	int nDelPKPoint = nNowPkPoint - nBeforePkPoint;
	if( nDelPKPoint > 0 )
	{
		Rsprintf( RWCHAR(g_strMessage ), G_STRING_CHAR(IDS_SYSPR_ADDPKPOINT), nDelPKPoint );		
		SystemPrint( n_DarkRed, n_SystemMsg , g_strMessage );
	}
	else if( nDelPKPoint < 0 )
	{
		nDelPKPoint = abs(nDelPKPoint);
		Rsprintf( RWCHAR(g_strMessage) , G_STRING_CHAR(IDS_SYSPR_SUBPKPOINT), nDelPKPoint );	
		SystemPrint( n_DarkRed, n_SystemMsg , g_strMessage );
	}

	BOOL IsBeforeCao = FALSE;
	if( ::GetPKStepFromPKPoint( nBeforePkPoint ) )
	{
		IsBeforeCao = TRUE;
	}

	BOOL IsAfterCao = FALSE;
	if( ::GetPKStepFromPKPoint( nNowPkPoint ) )
	{
		IsAfterCao = TRUE;
	}

	if( IsBeforeCao != IsAfterCao )
	{
		if( IsBeforeCao )
		{
			Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR(IDS_SYSPR_DELCAOMARK) );		
			SystemPrint( n_DarkRed, n_SystemMsg , g_strMessage );
		}
		else
		{
			Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR(IDS_SYSPR_ADDCAOMARK) );		
			SystemPrint( n_DarkRed, n_SystemMsg , g_strMessage );
		}
	}		
	
	nRui->GetPcParam()->thePKPoint = aMsg->thePKPoint;

	if( aMsg->theAttackUserPCID ) 
	{
		if( g_Pc.GetPlayer()->InsertMyAttackerList(aMsg->theAttackUserPCID) )
		{
			int nPcIndex = g_Pc.FindUser(aMsg->theAttackUserPCID);		
			if( MAX_USER_INTABLE != nPcIndex )
			{
				SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR(IDS_SYSPR_START_BEFOREATTACK), g_Pc.m_PcTable[nPcIndex].char_info.thePCName ); 
				g_Particle.m_EffectManager.CreateEffect( EF_BEFORE_ATTACKER_MARK , 
										   ( Character * )&g_Pc.m_Pc[nPcIndex] , NULL , NULL );
				///-- 여기서 선제공격자 표시를 넣어준다.
			}
		}
	}
}

void CRockClient::Proc_RpScUpdatePKPoint(SRpScUpdatePKPoint *aMsg)
{
	///-- 나랑 근처유저 다보내줌..
	if( g_Pc.m_MainPC.lUnique == aMsg->thePCID )
	{
		int nBeforePkPoint = nRui->GetPcParam()->thePKPoint;
		int nNowPkPoint = aMsg->thePKPoint;

		int nDelPKPoint = nNowPkPoint - nBeforePkPoint;
		if( nDelPKPoint > 0 )
		{
			Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR(IDS_SYSPR_ADDPKPOINT), nDelPKPoint );		
			SystemPrint( n_DarkRed, n_SystemMsg , g_strMessage );
		}
		else if( nDelPKPoint < 0 )
		{
			nDelPKPoint = abs(nDelPKPoint);
			Rsprintf(RWCHAR( g_strMessage), G_STRING_CHAR(IDS_SYSPR_SUBPKPOINT), nDelPKPoint );		
			SystemPrint( n_DarkRed, n_SystemMsg , g_strMessage );
		}

		BOOL IsBeforeCao = FALSE;
		if( ::GetPKStepFromPKPoint( nBeforePkPoint ) )
		{
			IsBeforeCao = TRUE;
		}

		BOOL IsAfterCao = FALSE;
		if( ::GetPKStepFromPKPoint( nNowPkPoint ) )
		{
			IsAfterCao = TRUE;
		}

		if( IsBeforeCao != IsAfterCao )
		{
			if( IsBeforeCao )
			{
				Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR(IDS_SYSPR_DELCAOMARK) );		
				SystemPrint( n_DarkRed, n_SystemMsg , g_strMessage );
			}
			else
			{
				Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR(IDS_SYSPR_ADDCAOMARK) );		
				SystemPrint( n_DarkRed, n_SystemMsg , g_strMessage );
			}
		}		
		
		nRui->GetPcParam()->thePKPoint = aMsg->thePKPoint;
	}
	else
	{
		int nPcIndex = g_Pc.FindUser(aMsg->thePCID);		
		if( MAX_USER_INTABLE != nPcIndex )
		{
			g_Pc.m_PcTable[nPcIndex].char_info.thePKPoint = aMsg->thePKPoint;			
		}
	}
}

void CRockClient::Proc_RpScChangePetName(SRpScChangePetName *aMsg)
{
	if( m_PcInven->thePetData[ aMsg->thePetSlot ] != NULL )
	{
		memset(m_PcInven->thePetData[aMsg->thePetSlot]->Name , NULL , 64 );
		memcpy( m_PcInven->thePetData[aMsg->thePetSlot]->Name, aMsg->thePetName , RP_MAX_PET_NAME );	
		Rsprintf(RWCHAR( g_strMessage) , G_STRING_CHAR(IDS_SYSCHANGE_PETNAME), RWCHAR(aMsg->thePetName) );	
		SystemPrint( n_DarkGreen, n_SystemMsg , g_strMessage );
		g_PetMainWndProc.Update();
	}
}

void CRockClient::Proc_RpScChangeName( SRpScChangeName * aMsg )
{
	///-- 플레이어 이름이 바뀔경우

	if( aMsg->theType == n_ChangePCName )
	{
		///-- 내이름이 바뀐 경우라면

		if( g_Pc.m_MainPC.lUnique == aMsg->theID )
		{
			// Find Frontier Member in list 
			std::vector < SFMemberInfo > :: iterator ir;							
			
			ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();
			
			for( ; ir != g_FrontierMemberListWnd.m_FrontierMemberInfoList.end() ; ++ir )
			{
				if( Rstrcmp( RWCHAR((*ir).Name), RWCHAR(g_Pc.m_MainPC.char_info.thePCName) ) == 0 )
				{
					SAFE_STR_CPY( (*ir).Name, aMsg->theNewName , 32 );			
					break;
				}
			}

			///-- 프론티어 마스터/메이져 이름이 변경되었는지 확인
			g_FrontierMainWnd.CheckChangeMasterMagorName( g_Pc.m_MainPC.char_info.thePCName, aMsg->theNewName );
			
			g_FrontierMainWnd.RefreshFrontierMyInfo();
			g_FrontierMainWnd.RefreshFrontierMyFrontier();
			g_FrontierMemberListWnd.RefreshMemberInfoList();

			SAFE_STR_CPY( g_Pc.m_MainPC.char_info.thePCName, aMsg->theNewName, RP_MAX_PC_NAME );
			SAFE_STR_CPY( nRui->thePcParam.PCName , aMsg->theNewName, RP_MAX_PC_NAME  );
			Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR(IDS_SYSCHANGE_PCNAME) , RWCHAR(aMsg->theNewName));			
			SystemPrint( n_DarkGreen, n_SystemMsg , g_strMessage );

		}
		else
		{
			NonPlayer* other_pc[ MAX_USER_INTABLE ];
			for(int i = 0; i < MAX_USER_INTABLE; ++i )
			{	
				other_pc[i] = &g_Pc.m_Pc[i];	
				
				if( g_Pc.m_PcTable[i].lUnique != -1 || other_pc[i]->m_nTableIndex != -1 )
				{
					if( g_Pc.m_PcTable[i].lUnique == aMsg->theID ) 
					{
						SAFE_STR_CPY( g_Pc.m_PcTable[i].char_info.thePCName, aMsg->theNewName, RP_MAX_PC_NAME );
						break;
					}
				}
			}
		}	
	}	
	///-- 프론티어 이름이 바뀔경우

	else if( aMsg->theType == n_ChangeFrontierName_MySelf )
	{
		if( nRui->GetPcInvenInfo()->FrontierMyInfo )
		{
			if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID == aMsg->theID )
			{
				if( nRui->GetPcInvenInfo()->FrontierInfo )	
				{
					Rsprintf( RWCHAR(g_strMessage) , G_STRING_CHAR(IDS_SYSCHANGE_FRONNAME) , RWCHAR(aMsg->theNewName) );	
					SystemPrint( n_DarkGreen, n_SystemMsg , g_strMessage );
					SAFE_STR_CPY( nRui->GetPcInvenInfo()->FrontierInfo->theFrontierName, aMsg->theNewName , RP_MAX_FRONTIER_NAME );
				
					g_FrontierMainWnd.RefreshFrontierMyInfo();
					g_FrontierMainWnd.RefreshFrontierMyFrontier();				
				}
			}
		}					
	}
	else if( aMsg->theType == n_ChangeFrontierName_OtherSelf )
	{
		NonPlayer* other_pc[ MAX_USER_INTABLE ];
		for(int i = 0; i < MAX_USER_INTABLE; ++i )
		{	
			other_pc[i] = &g_Pc.m_Pc[i];	
				
			if( g_Pc.m_PcTable[i].lUnique != -1 || other_pc[i]->m_nTableIndex != -1 )
			{
				if( g_Pc.m_PcTable[i].char_info.theFrontierID == aMsg->theID ) 
				{
					memset(g_Pc.m_PcTable[i].char_info.theFrontierName , NULL , sizeof(g_Pc.m_PcTable[i].char_info.theFrontierName));
					memcpy(g_Pc.m_PcTable[i].char_info.theFrontierName , aMsg->theNewName , sizeof(aMsg->theNewName));
				}
			}
		}
	}
}

void CRockClient::Proc_RpScChangeGender(SRpScChangeGender * aMsg)
{
	///-- 내성별이 바뀐 경우라면

	if( g_Pc.m_MainPC.lUnique == aMsg->thePCID )
	{
		nRui->thePcParam.Gender = aMsg->theGender;
		g_Pc.m_MainPC.char_info.theGender = nRui->thePcParam.Gender;
		g_Pc.m_MainPC.nIndex = g_Pc.GetAttrIndex( g_Pc.m_MainPC.char_info.theCitizen, g_Pc.m_MainPC.char_info.theGender );
		g_Pc.m_TempPC.nIndex = g_Pc.m_MainPC.nIndex;

		g_Pc.m_MainPC.char_info.theFace = aMsg->theFaceCode;
		g_Pc.m_MainPC.char_info.theHair = aMsg->theHairCode;

		g_Pc.GetPlayer()->ReSetWearItem();
		g_Pc.GetPlayer()->AnimationPlayNoneBlend( n_Stand_Peace9 );
		g_Pc.GetPlayer()->AniRender();

		g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
			g_Pc.m_MainPC.char_info.race , g_lpDevice, false );				
		g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
			g_Pc.m_MainPC.char_info.race , g_lpDevice );

		nRui->ReSetTIDImage();
		
		if( aMsg->theGender == MALE )
		{
			SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSCHANGE_GENDERMALE) ); 
		}
		else
		{
			SystemPrint( n_DarkGreen, n_SystemMsg , G_STRING_CHAR(IDS_SYSCHANGE_GENDERFEMALE) ); 
		}
	}
	else
	{
		NonPlayer* other_pc[ MAX_USER_INTABLE ];
		for(int i = 0; i < MAX_USER_INTABLE; ++i )
		{	
			other_pc[i] = &g_Pc.m_Pc[i];	
			
			if( g_Pc.m_PcTable[i].lUnique != -1 || other_pc[i]->m_nTableIndex != -1 )
			{
				if( g_Pc.m_PcTable[i].lUnique == aMsg->thePCID ) 
				{
					g_Pc.m_PcTable[i].char_info.theGender = aMsg->theGender;
					g_Pc.m_PcTable[i].nIndex = g_Pc.GetAttrIndex( g_Pc.m_PcTable[i].char_info.theCitizen, g_Pc.m_PcTable[i].char_info.theGender );
					
					g_Pc.m_PcTable[i].char_info.theFace = aMsg->theFaceCode;
					g_Pc.m_PcTable[i].char_info.theHair = aMsg->theHairCode;

					g_Pc.m_Pc[i].ReSetWearItem();
					g_Pc.m_Pc[i].AnimationPlayNoneBlend( n_Stand_Peace9 );
					g_Pc.m_Pc[i].AniRender();

					//by simwoosung - 파티 멤버 이미지의 갱신
					if(g_PartyInforWnd.IsMemberParty())
					{
						SPARTYMEMBER_INFO *pMemberInfo = g_PartyInforWnd.FindPartyMember(aMsg->thePCID);
						if(pMemberInfo && pMemberInfo->m_bIsViewRange && pMemberInfo->m_bIsSee)
						{
							g_PartyInforWnd.ChangePCharImage(aMsg->thePCID);			
						}
					}
					
					break;
				}
			}
		}		
	}
}

void CRockClient::Proc_RpScChangeFrontierPCName(SRpScChangeFrontierPCName * aMsg)
{
	// Find Frontier Member in list 
	std::vector < SFMemberInfo > :: iterator ir;							
	
	ir = g_FrontierMemberListWnd.m_FrontierMemberInfoList.begin();
	
	for( ; ir != g_FrontierMemberListWnd.m_FrontierMemberInfoList.end() ; ++ir )
	{
		if( Rstrcmp( RWCHAR((*ir).Name), RWCHAR(aMsg->theOldPCName) ) == 0 )
		{
			SAFE_STR_CPY( (*ir).Name, aMsg->theNewPCName , 32 );			
			break;
		}
	}

	///-- 프론티어 마스터/메이져 이름이 변경되었는지 확인
	g_FrontierMainWnd.CheckChangeMasterMagorName( aMsg->theOldPCName, aMsg->theNewPCName );
	
	g_FrontierMainWnd.RefreshFrontierMyInfo();
	g_FrontierMainWnd.RefreshFrontierMyFrontier();
	g_FrontierMemberListWnd.RefreshMemberInfoList();

	SFriendData *pFriendData = NULL;
	pFriendData = g_MCommunityWnd.m_MGuildTabWnd.FindFriendData(aMsg->theOldPCName);

	if(pFriendData)
	{
		SAFE_STR_CPY(pFriendData->strFName, aMsg->theNewPCName, 512);
		
		g_MCommunityWnd.m_MGuildTabWnd.SortFriendData();
		g_MCommunityWnd.m_MGuildTabWnd.SetFriendField();
	}
}

void CRockClient::Proc_RpScCheckSoldierTime(SRpScCheckSoldierTime * aMsg)
{
	DWORD aSlotNumber = aMsg->theSlotNumber;
	
	if( aSlotNumber != MERCE_SLOT_NUM )
	{
		return;
	}

	if( m_PcInven->thePetData[aSlotNumber] )
	{
		m_PcInven->thePetData[aSlotNumber]->theLifeTime = aMsg->theRemainTime * 1000;
		m_PcInven->thePetData[aSlotNumber]->theStartTime = timeGetTime();
	}
}

///--- PVP 중립 전장필드에서 어느 상징물이 공격받고있는지 여부에 대한 프로토콜  
void CRockClient::Proc_RpScNPCAttackedMsg(SRpScNPCAttackedMsg * aMsg)
{
	if( aMsg->theResult )
	{
		WORD aMapCode = aMsg->theMapCode;	
		BYTE aOccNation = aMsg->theOccupationNation;

		switch(aOccNation)
		{
			case RAIN:
				{
					Rsprintf( RWCHAR(g_strMessage),  G_STRING_CHAR( IDS_FIELD_OCCUPYED_BY ), 
											G_STRING_CHAR( G_MAP_STRID(aMapCode) ), G_STRING_CHAR( IDS_RAIN ) );	
					SystemPrint( n_LightGreen, n_SystemMsg , g_strMessage );			
				}
				break;
			case MILLENA:
				{
					Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR( IDS_FIELD_OCCUPYED_BY ), 
											G_STRING_CHAR( G_MAP_STRID(aMapCode) ), G_STRING_CHAR( IDS_MILLENA ) );	
					SystemPrint( n_LightGreen, n_SystemMsg , g_strMessage );	
				}
				break;
		}
	
	}
	else
	{
		///-- 전장 필드 바깥쪽 유저의 경우
		if( aMsg->theFiledFlag )
		{
			WORD aMapCode = aMsg->theMapCode;
			Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR( IDS_FIELD_ATTACKED_BY ), G_STRING_CHAR( G_MAP_STRID(aMapCode) ) );	
			SystemPrint( n_LightGreen, n_SystemMsg , g_strMessage ); 
		}
		///-- 전장 필드 안쪽 유저의 경우
		else
		{
			DWORD aNPCID = aMsg->theNPCID;
			PvPFieldInfoListPos aPos = g_Map.m_PvPFieldInfoList.begin();

			for( ; aPos != g_Map.m_PvPFieldInfoList.end(); ++aPos )
			{
				SPvPFieldInfo * pPvPFieldInfo = (SPvPFieldInfo *)(*aPos);
				if( pPvPFieldInfo->theNPCID == aNPCID )
				{
					SNPCBaseInfo* npc_Info = NULL;				
					npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pPvPFieldInfo->theNPCCode );
					
					if( npc_Info )
					{
						Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR( IDS_SYMBOL_ATTACKED_BY ), RWCHAR(npc_Info->theName) );	
						SystemPrint( n_LightYellow, n_SystemMsg , g_strMessage ); 
					}

					break;
				}
			}
		}
	}
}

void CRockClient::Proc_RpScNPCLevelUP(SRpScNPCLevelUP * aMsg)
{

	int nPrevNpcCode = -1;
	int	npc_tbl_index = g_Map.FindNpc( aMsg->theNPCID );
	
	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		nPrevNpcCode = g_Map.m_NpcTable[npc_tbl_index].code;
	}
	
	if( nPrevNpcCode == aMsg->theNPCCode )
	{
		return;
	}

	DWORD aNPCID = aMsg->theNPCID;
	PvPFieldInfoListPos aPos = g_Map.m_PvPFieldInfoList.begin();

	for( ; aPos != g_Map.m_PvPFieldInfoList.end(); ++aPos )
	{
		SPvPFieldInfo * pPvPFieldInfo = (SPvPFieldInfo *)(*aPos);
		if( pPvPFieldInfo->theNPCID == aNPCID )
		{
			pPvPFieldInfo->theNPCCode = aMsg->theNPCCode;
			break;
		}
	}

	g_Map.SM__NPC__LEVELUP( aMsg->theNPCID, aMsg->theNPCCode );

	D3DXVECTOR3	vPos;
	
	vPos = D3DXVECTOR3( g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._41 , 
		g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._42 + 6.0f, 
		g_Map.m_Npc[ npc_tbl_index ].GetPosTM()._43 );
	
	g_Sound.Play( 302 , vPos.x , vPos.y , vPos.z );

	if( npc_tbl_index != MAX_NPC_INTABLE )
	{
		g_Map.m_NpcTable[npc_tbl_index].theHP = aMsg->theHP;
		g_Map.m_NpcTable[npc_tbl_index].theRealHP = aMsg->theHP;						
	}
}

void CRockClient::Proc_RpScEnterPvPField(SRpScEnterPvPField * aMsg)
{
	int nInfoCount = aMsg->theInfoNum;
	
	g_Map.ClearPvPFieldInfoList();

	for( int i = 0 ; i < nInfoCount ; i++ )
	{
		SPvPFieldInfo * pSPvPFInfo  = SAFE_NEW( SPvPFieldInfo );
		pSPvPFInfo->theNPCID				= aMsg->theEnterPvPFieldInfo[i].theNPCID;
		pSPvPFInfo->theNPCCode			= aMsg->theEnterPvPFieldInfo[i].theNPCCode;
		pSPvPFInfo->theNPCCell			= aMsg->theEnterPvPFieldInfo[i].theNPCCell;
		pSPvPFInfo->theOccupationNation	= aMsg->theEnterPvPFieldInfo[i].theOccupationNation;
		
		g_Map.m_PvPFieldInfoList.push_back( pSPvPFInfo );
	}
}

void CRockClient::Proc_RpScUpdatePvPField(SRpScUpdatePvPField * aMsg)
{
	///-- 현재 내가 속해있는 맵과 같으냐 정보
	if( g_Map.m_NowMapID != aMsg->theMapCode )
	{
		return;
	}	
	
	DWORD aNPCCell = aMsg->theNPCCell;
	PvPFieldInfoListPos aPos = g_Map.m_PvPFieldInfoList.begin();

	for( ; aPos != g_Map.m_PvPFieldInfoList.end(); ++aPos )
	{
		SPvPFieldInfo * pPvPFieldInfo = (SPvPFieldInfo *)(*aPos);
		if( pPvPFieldInfo->theNPCCell == aNPCCell )
		{
			pPvPFieldInfo->theNPCID = aMsg->theNPCID;
			pPvPFieldInfo->theNPCCode = aMsg->theNPCCode;			
			
			BYTE aPrevNation = pPvPFieldInfo->theOccupationNation;
			BYTE aNowNation = aMsg->theOccupationNation;		

			if( aPrevNation != aNowNation )
			{
				SNPCBaseInfo* npc_Info = NULL;				
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pPvPFieldInfo->theNPCCode );
				if( npc_Info ) 
				{
					switch(aNowNation)
					{
						case RAIN:
							{
								Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR( IDS_SYMBOL_OCCUPYED_BY ), 
										 RWCHAR(npc_Info->theName), G_STRING_CHAR( IDS_RAIN ) );
								SystemPrint( n_LightYellow, n_SystemMsg , g_strMessage ); 
							}
							break;
						case MILLENA:
							{
								Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR( IDS_SYMBOL_OCCUPYED_BY ), 
										 RWCHAR(npc_Info->theName), G_STRING_CHAR( IDS_MILLENA ) );
								SystemPrint( n_LightYellow, n_SystemMsg , g_strMessage ); 
							}
							break;
					}
				}
			}			
			
			pPvPFieldInfo->theOccupationNation = aMsg->theOccupationNation;			
			break;
		}
	}
}

void CRockClient::Proc_RpScMakeAddAccessory(SRpScMakeAddAccessory * aMsg)
{
	static SPcItem tempStaticPcItem;	
	
	if( g_pAcryEnchantWnd && g_pAcryEnchantWnd->IsVisible )
	{
		g_pAcryEnchantWnd->Close();
	}

	if( aMsg->theResult > 1 )
	{
		if( !GetItemProtocolResult( aMsg->theResult ) )
		{
			return;
		}
	}
	
	switch( aMsg->theResult )
	{
		///-- 실패
		case 0:	
		///-- 성공
		case 1:
			{
				for(int i = 0; i < c_MaxInvenItemNum; ++i )
				{	
					if( m_PcInven->InvenItem[i] == NULL )
					{
						continue;
					}

					if ( m_PcInven->InvenItem[i] && 
						 m_PcInven->InvenItem[i]->ItemUID == aMsg->theAddAccessory.theWearItem.theItemUID ) 
					{
						///-- 성공
						if( aMsg->theResult == 1 )
						{
							memcpy( &(m_PcInven->InvenItem[i]->WearTable), &(aMsg->theAddAccessory.theWearItem) , sizeof(SRpdWearItemBase) );
						}
						
						memcpy( &tempStaticPcItem, m_PcInven->InvenItem[i], sizeof(SPcItem) );

						///-- 실패
						if( aMsg->theResult == 0 )
						{
							delete( m_PcInven->InvenItem[i] );
							m_PcInven->InvenItem[i] = NULL;
						}

						break;
					}
				}

				for(short i = 0; i < c_MaxInvenItemNum; ++i )
				{	
					if( m_PcInven->InvenItem[i] == NULL )
					{
						continue;
					}

					if ( m_PcInven->InvenItem[i] && 
						 m_PcInven->InvenItem[i]->ItemUID == aMsg->theAddScroll.theItemID ) 
					{
						//남은 번들 수. 0 인 경우 아이템 삭제.
						m_PcInven->InvenItem[i]->Amount = aMsg->theAddScroll.theBundle;
						if( m_PcInven->InvenItem[i]->Amount == 0 )
						{
							//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
							for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
							{
								if( m_PcInven->InvenItem[i] == m_PcInven->QuickSlot[idx] )
								{
									m_PcInven->QuickSlot[idx] = NULL;
									break;
								}
							}
	
							delete( m_PcInven->InvenItem[i] );
							m_PcInven->InvenItem[i] = NULL;							
						}
						break;
					}
				}

				if( g_pAcryEnchantRWnd )
				{
					///-- 성공
					if( aMsg->theResult == 1 )
					{
						g_pAcryEnchantRWnd->Open( TRUE );
					}
					else
					{
						g_pAcryEnchantRWnd->Open( FALSE );
					}
					
					g_pAcryEnchantRWnd->SetPcItem( &tempStaticPcItem );
				}

				//슬롯에 적용....
				MainMenuWnd.UpdateQuickSlotItem();
				InventoryWnd.UpdateInvenSlotItem();	
			}
			break;	
	}
}

void CRockClient::Proc_RpScChangeEldaStone(SRpScChangeEldaStone * aMsg)
{
	SPcItem tempPcItem;

	if( aMsg->theResult > 1 )
	{
		if( !GetItemProtocolResult( aMsg->theResult ) )
		{
			return;
		}
	}

	int i = 0;

	switch( aMsg->theResult )
	{
		///-- 실패
		case 0:	
		///-- 성공
		case 1:
			{
				///-- 변경전 엘다스톤 정보
				for( i = 0; i < c_MaxInvenItemNum; ++i )
				{	
					if( m_PcInven->InvenItem[i] == NULL )
					{
						continue;
					}

					if ( m_PcInven->InvenItem[i] && 
						 m_PcInven->InvenItem[i]->ItemUID == aMsg->theBeforeEldaStone.theWearItem.theItemUID ) 
					{
						///-- 실패
						if( aMsg->theResult == 0 )
						{
							memcpy( &tempPcItem, m_PcInven->InvenItem[i], sizeof(SPcItem) );
						}

						//남은 번들 수. 0 인 경우 아이템 삭제.
						m_PcInven->InvenItem[i]->Amount = aMsg->theBeforeEldaStone.theWasteItem.theBundle;
						if( m_PcInven->InvenItem[i]->Amount == 0 )
						{
							//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
							for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
							{
								if( m_PcInven->InvenItem[i] == m_PcInven->QuickSlot[idx] )
								{
									m_PcInven->QuickSlot[idx] = NULL;
									break;
								}
							}
	
							delete( m_PcInven->InvenItem[i] );
							m_PcInven->InvenItem[i] = NULL;							
						}
						break;
					}

				}

				///-- 성공시
				if( aMsg->theResult == 1 )
				{
					///-- 변경된 엘다스톤 정보
					SPcItem*	tmpPcItem = NULL;				
					tmpPcItem = SAFE_NEW( SPcItem );
					WORD ItemType = g_Pc_Manager.GetItemClsss( aMsg->theAfterEldaStone.theWasteItem.theCode );
					if( ItemType != n_Class_Eldastone )
					{
						SAFE_DELETE( tmpPcItem );
						return;
					}	

					tmpPcItem->IType	= n_ITYPE_SPECIAL; 
					tmpPcItem->Amount	= aMsg->theAfterEldaStone.theWasteItem.theBundle;

					tmpPcItem->Class		= ItemType;
					tmpPcItem->ItemUID		= aMsg->theAfterEldaStone.theWasteItem.theItemUID;
					tmpPcItem->Code			= aMsg->theAfterEldaStone.theWasteItem.theCode;
					tmpPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theAfterEldaStone.theWasteItem.theCode );
					tmpPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theAfterEldaStone.theWasteItem.theCode );

					if( tmpPcItem->ItemTable == NULL )
					{
						SAFE_DELETE( tmpPcItem );						
						return;
					}

					SItemBaseInfo* item_Info = NULL;
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theAfterEldaStone.theWasteItem.theCode );	

					SAFE_DELETE( m_PcInven->InvenItem[aMsg->theAfterEldaStone.theWasteItem.thePosition] );
					m_PcInven->InvenItem[aMsg->theAfterEldaStone.theWasteItem.thePosition] = tmpPcItem;	
					
					memcpy( &tempPcItem, tmpPcItem , sizeof(SPcItem) );
				}

				///-- 사용한 유료아이템의 정보			
				for( i = 0; i < c_MaxInvenItemNum; ++i )
				{	
					if( m_PcInven->InvenItem[i] == NULL )
					{
						continue;
					}

					if ( m_PcInven->InvenItem[i] && 
						 m_PcInven->InvenItem[i]->ItemUID == aMsg->thePayItem.theWearItem.theItemUID ) 
					{
						//남은 번들 수. 0 인 경우 아이템 삭제.
						m_PcInven->InvenItem[i]->Amount = aMsg->thePayItem.theWasteItem.theBundle;
						if( m_PcInven->InvenItem[i]->Amount == 0 )
						{
							//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
							for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
							{
								if( m_PcInven->InvenItem[i] == m_PcInven->QuickSlot[idx] )
								{
									m_PcInven->QuickSlot[idx] = NULL;
									break;
								}
							}
	
							delete( m_PcInven->InvenItem[i] );
							m_PcInven->InvenItem[i] = NULL;							
						}
						break;
					}

				}

				if( g_pChangeEldaColorRWnd )
				{
					///-- 성공
					if( aMsg->theResult == 1 )
					{
						g_pChangeEldaColorRWnd->Open( TRUE );
					}
					else
					{
						g_pChangeEldaColorRWnd->Open( FALSE );
					}
					
					g_pChangeEldaColorRWnd->SetPcItem( &tempPcItem );
				}

				//슬롯에 적용....
				MainMenuWnd.UpdateQuickSlotItem();
				InventoryWnd.UpdateInvenSlotItem();	
			}
			break;
	}

}

//남은 점령시간 안내관련
void CRockClient::Proc_RpScOccupationTime(SRpScOccupationTime * aMsg)
{
	if( aMsg->theField )
	{
		DWORD dRemainTime = aMsg->theRemainTime * 1000;

		::GetRemainTimeStr( g_strMessage2 , dRemainTime );
		Rsprintf( RWCHAR(g_strMessage), G_STRING_CHAR( IDS_SYSPR_REMAIN_OCUUTIME ), 
							   G_STRING_CHAR( G_MAP_STRID(aMsg->theField) ) , RWCHAR(g_strMessage2) );
		SystemPrint( n_LightGreen, n_SystemMsg , g_strMessage );	
	}
}

bool CRockClient::Proc_RpScGameGuard(SRpScGameGuard * aMsg)
{

	if( aMsg->theResult == n_GG_Normal)
	{
#ifdef C_GAME_GUARD
		memcpy(&m_AuthData, &aMsg->theGameGuardMsg , sizeof(GG_AUTH_DATA));
		pNpgl->Auth2(&m_AuthData);
#else
		BYTE Msg[6] = {0,};
		DWORD Result = -1;
		memcpy(&Msg , &Result , sizeof(DWORD));
		g_RockClient.Send_CsGameGuard( -1 , Msg , sizeof(Msg)); // 서버로 데이터 전송.
#endif 
	}

	return TRUE;
}

bool CRockClient::Proc_RpScHackShield(SRpScHackShield* aMsg)
{

	UCHAR uMsg[340] = {0,};
	DWORD Ret = ERROR_SUCCESS ; 
	TCHAR szMsg[255] = {0,};
	bool bModeHackShied = false; 
	char Msg[256] = {0,};
	UCHAR Strtmp[340] = {0,};
//	sprintf(Msg , "ScHackShield %s " , aMsg->theMsg );
//	MessageBoxWnd.Open(Msg  , 3000, NULL , false);

	switch(aMsg->theFlag)
	{
	case ScGUIDMSG: //GUID 검색 로그인힌 한번만 검색한다 
		// 340 byte 변경 가변사이즈로 계산한다. 
		//memset(&uMsg , aMsg->theMsg , sizeof(aMsg->theMsg));

#if defined(GM_CLIENT) || defined( GLOBAL_VERSION ) || defined( TEST_NO_HACK_SHIELD )
			//GM클라이언트 체크 루틴..
			//여기에 .. uMsg 보내줘야 함. . 
			//////////////////////////////////////////////////////////////////////////
			g_RockClient.Send_CsHackShield(CsGM_CLIENTMSG , uMsg);
			return TRUE;
#else	 
			memcpy( Strtmp , aMsg->theMsg , SIZEOF_GUIDREQMSG );

#ifdef C_HACK_SHIELD
			Ret = _AhnHS_MakeGuidAckMsg( Strtmp , uMsg);
#endif 
			if( Ret != ERROR_SUCCESS)
			{
				Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_SHILED_GUID_ERROR) , Ret);
				HackClose(szMsg,ScGUIDMSG);
			}
			
			g_RockClient.Send_CsHackShield(CsGUIDMSG , uMsg);
			//ZeroMemory(uMsg , sizeof(uMsg));
			//g_RockClient.Send_CsHackShield(CsGM_CLIENTMSG , uMsg);
		
#endif			
		break;
	case ScCRCMSG:	//CRC  게임내에 주기적으로 호출된다 . 

#if defined(GM_CLIENT) || defined( GLOBAL_VERSION ) || defined( TEST_NO_HACK_SHIELD )
		
			g_RockClient.Send_CsHackShield(CsGM_CLIENTMSG , uMsg);
			return TRUE;

#else

			memcpy( Strtmp , aMsg->theMsg , 160 );

#ifdef C_HACK_SHIELD
			Ret = _AhnHS_MakeAckMsg( Strtmp  , uMsg );
#endif 		

			if( Ret != ERROR_SUCCESS)
			{
				Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_SHILED_CRC_ERROR) , Ret);
				HackClose(szMsg , ScCRCMSG );
			}
			g_RockClient.Send_CsHackShield(CsCRCMSG , uMsg);

#endif			
		break;
	case ScGUID_MAKEFAIL:
		Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_SHILED_GUID_FAIL));
		if(!HackClose(szMsg , ScGUID_MAKEFAIL))
			return false;
		break;
	case ScCRC_MAKEFAIL:
		Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_SHILED_CRC_FAIL));
		if(! HackClose(szMsg , ScCRC_MAKEFAIL ))
			return false;
		break;
	case ScGUID_Error:  //exe 버젼이 틀린경우 
		Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_FILE_CHANGE_GUID_ERROR) ,aMsg->theErrorCode );
		HackClose(szMsg, ScGUID_Error);
		break;
	case ScCRC_Error:  //exe 버젼이 틀린경우 
		Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_FILE_CHANGE_CRC_ERROR) ,aMsg->theErrorCode );
		HackClose(szMsg, ScCRC_Error);
		break;
	
	default: break; 

	}



return TRUE;

}
void CRockClient::Proc_SRpScPlaytime(SRpScPlaytime * aMsg)
{

	/*
	NORMAL		  = 0,	//정상..
	NORMAL_CHECK  = 1,	//1시간 마다..
	ABNORMAL      = 2,	//3시간 되면..
	SOME_ABNORMAL = 3,  //3시간 이상

	MANY_ABNORMAL = 4,	//5시간 이상 
	int				thePlaytime;		
	BYTE			thePlayAbleFlag;	// 0 : 정상, 1: 3시간 이상, 2 : 5시간 이상 
	*/
	g_UserTime.SetLock(true);

	switch(aMsg->thePlayAbleFlag) 
	{
	case NORMAL:  
			g_UserTime.SetTime(aMsg->thePlaytime);
			break;
	case NORMAL_CHECK: 			
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PLAY_TIME_NORMAL ) );
			break;
	case ABNORMAL: 		
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PLAY_TIME_ABNORMAL ) );
			break;
	case SOME_ABNORMAL: 		
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PLAY_TIME_SOME_ABNORMAL ) );
			break;
	case MANY_ABNORMAL: 			
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PLAY_TIME_MANY_ABNORMAL ) );
			break;
	default:
			break;


	}


}

void CRockClient::Proc_RpScMakeEldastone( SRpScMakeEldastone* aMsg)
{
/*
	SRpHeader			theMsgHeader;
	BYTE				theResult;			// NItemError 참조 
	URpdItem			theEldastone;		// 생성된 엘다스톤 
	URpdItem			theEldastonePowder;	// 엘다스톤 생성후 파우더의 잔존 정보 
	URpdItem			theProima;			// 엘다스톤 생성후 프로이마의 잔존 정보 
	DWORD				theEld;				// 생성후 잔존 엘드 정보 
*/

	int i =0 , j = 0 ; 

	if( GetItemProtocolResult(aMsg->theResult)) 
	{
		int Pos =0 ; 
		
		for( j = 0; j < c_MaxInvenItemNum; ++j )
		{	
			Pos = -1;
			
			if( m_PcInven->InvenItem[j] == NULL )
			{
				continue;
			}
			
			if(aMsg->theEldastonePowder.theItem.theItemUID == m_PcInven->InvenItem[j]->ItemUID) Pos= 0 ; 
			else if(aMsg->theProima.theItem.theItemUID  == m_PcInven->InvenItem[j]->ItemUID) Pos= 1 ; 
						
			if( Pos != -1)
			{
				if(Pos == 0 )
				{
					m_PcInven->InvenItem[j]->Amount = aMsg->theEldastonePowder.theWasteItem.theBundle;
				}
				else 
				{
					m_PcInven->InvenItem[j]->Amount = aMsg->theProima.theWasteItem.theBundle;
				}
								
				if( m_PcInven->InvenItem[j]->Amount == 0 )
				{
					//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
					for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
					{
						if( m_PcInven->InvenItem[j] == m_PcInven->QuickSlot[idx] )
						{
							m_PcInven->QuickSlot[idx] = NULL;
							break;
						}
					}
					
					delete( m_PcInven->InvenItem[j] );
					m_PcInven->InvenItem[j] = NULL;
				}					
			}
		}			
		
		//인벤에서 삭제 시켜라 .. 
		// 총 3개 ? 아이템 ... ㅋㅋ 
		///-- 인벤 체크 
		SPcItem*	tmpPcItem = NULL;
		CreateItemPcItem(&tmpPcItem , aMsg->theEldastone  );
		
		CItemTransButton *pItemTransButton = g_main_buttons.GetInVenButton();
		SItemTransInfo ItemTransInfo;
		ItemTransInfo.Is3DPos = false ;
		ItemTransInfo.interNpcId = 0;
		ItemTransInfo.isMoney = false;
		ItemTransInfo.isGet = true;
		ItemTransInfo.pcInfo = *tmpPcItem;			
		ItemTransInfo.isMonster = false;
		pItemTransButton->InsertItemTransInfo(ItemTransInfo);
		
		//자리를 찾는다 . 
		//그리고 이벤엔 집어 넣는다. 
		//서버에서 자리를 받아와야 하낟.. 지금음 테스팅 .. ㅠㅠ
		m_PcInven->InvenItem[aMsg->theEldastone.theItem.thePosition] = tmpPcItem;
		
		m_PcParam->Money = aMsg->theEld;
		
		g_ItemSkillPotionRWndProc.FrameWnd->SetCaption(G_STRING_CHAR(IDS_WND_RESULT_WINDOWS)); 
		g_ItemSkillPotionRWndProc.OpenEnchantR_SWnd(aMsg->theEldastone.theItem.theItemUID);
		
		MainMenuWnd.UpdateQuickSlotItem();
		InventoryWnd.UpdateInvenSlotItem();	
	}

		

/*
	else 
	{
		int Pos =0 ; 
		
		for( j = 0; j < c_MaxInvenItemNum; ++j )
		{	
			Pos = -1;
			
			if( m_PcInven->InvenItem[j] == NULL )
			{
				continue;
			}
			
			if(aMsg->theEldastonePowder.theItem.theItemUID == m_PcInven->InvenItem[j]->ItemUID) Pos= 0 ; 
			else if(aMsg->theProima.theItem.theItemUID  == m_PcInven->InvenItem[j]->ItemUID) Pos= 1 ; 
			
			if( Pos != -1)
			{
				if(Pos == 0 )
				{
					m_PcInven->InvenItem[j]->Amount = aMsg->theEldastonePowder.theWasteItem.theBundle;
				}
				else 
				{
					m_PcInven->InvenItem[j]->Amount = aMsg->theProima.theWasteItem.theBundle;
				}
				
				if( m_PcInven->InvenItem[j]->Amount == 0 )
				{
					//소모품은 퀵슬롯 연결되어 있을때....퀵슬롯에서도...삭제 되어야 한다.
					for( int idx = 0; idx < RP_MAX_QUICK_SLOT; ++idx )
					{
						if( m_PcInven->InvenItem[j] == m_PcInven->QuickSlot[idx] )
						{
							m_PcInven->QuickSlot[idx] = NULL;
							break;
						}
					}
					
					delete( m_PcInven->InvenItem[j] );
					m_PcInven->InvenItem[j] = NULL;
				}					
			}
		}			

		m_PcParam->Money = aMsg->theEld;
		
		g_ItemSkillPotionRWndProc.FrameWnd->SetCaption("결과"); 
		g_ItemSkillPotionRWndProc.OpenEnchantR_FWnd();
		
	}
	*/
	//슬롯에 적용....
		
	
}


void CRockClient::Proc_SRpScMakeStrengthstone( SRpScMakeStrengthstone * aMsg)
{
/*
	SRpHeader			theMsgHeader;
	BYTE				theResult;						// NItemError 참조 
	URpdItem			theStrengthstone;				// 생성된 강화 주문석 코드
	URpdItem			theStrengthstoneMaterial[5];	// 재료 잔존 정보 
	DWORD				theEld;							// 생성후 잔존 엘드 정보 
*/

	if( GetItemProtocolResult(aMsg->theResult)) 
	{
		///-----------------------------------------------------------------------
		///-- 아이템 제거 
		for(int i = 0; i < c_MaxInvenItemNum; ++i )
		{	
			if( m_PcInven->InvenItem[i] == NULL )
			{
				continue;
			}
			
			///-- 각 Slot 별
			for(int j = 0 ; j < 5; ++j )
			{
				///-- 잡탬 검색
				
				if ( m_PcInven->InvenItem[i] && 
					m_PcInven->InvenItem[i]->ItemUID == aMsg->theStrengthstoneMaterial[j].theWasteItem.theItemUID  ) 
				{
					///-- Amount 변경
					
					m_PcInven->InvenItem[i]->Amount = aMsg->theStrengthstoneMaterial[j].theWasteItem.theBundle ;
					
					///-- 아이템 완전 삭제
					if( m_PcInven->InvenItem[i]->Amount < 1 )
					{	
						SAFE_DELETE( m_PcInven->InvenItem[i] );					
					}
				}			
			}								
		}
	
		SPcItem*	tmpPcItem = NULL;
		CreateItemPcItem(&tmpPcItem , aMsg->theStrengthstone );
		
		CItemTransButton *pItemTransButton = g_main_buttons.GetInVenButton();
		SItemTransInfo ItemTransInfo;
		ItemTransInfo.Is3DPos = false ;
		ItemTransInfo.interNpcId = 0;
		ItemTransInfo.isMoney = false;
		ItemTransInfo.isGet = true;
		ItemTransInfo.pcInfo = *tmpPcItem;			
		ItemTransInfo.isMonster = false;
		pItemTransButton->InsertItemTransInfo(ItemTransInfo);
		
		m_PcParam->Money = aMsg->theEld;
		
		//슬롯에 적용....
		MainMenuWnd.UpdateQuickSlotItem();
		InventoryWnd.UpdateInvenSlotItem();	
		
		g_ItemSkillPotionRWndProc.OpenEnchantR_SWnd(aMsg->theStrengthstone.theItem.theItemUID);
	}

}

void CRockClient::Proc_SRpScMakeEssence(SRpScMakeEssence * aMsg)
{
	if( GetItemProtocolResult(aMsg->theResult)) 
	{

		//창띄움 성공시
		//어던 아이템을 획득 하였습니다. 
		g_ItemSkillPotionRWndProc.OpenEnchantRCode_SWnd(aMsg->theEssence.theItemID , aMsg->theEssence.theBundle  );
		return;
	}
}

void CRockClient::Proc_SRpScUseStrengthstone(SRpScUseStrengthstone * aMsg )
{
	/*
	SRpHeader			theMsgHeader;
	BYTE				theResult;			// NItemError 참조
	URpdItem			theStrengthstone;	// 남은 강화 주문석
	WORD				theBeforeStrengthItem;	// 강화 전 아이템
	URpdItem			theStrengthItem;	// 강화된 아이템
	DWORD				theEld;				// 생성후 잔존 엘드 정보
	#ifdef PAYITEM_6
	SBundleItem				thePayItem[3];			// 인챈트용 유료아이템 ID
	 */
	if( GetItemProtocolResult(aMsg->theResult)) 
	{
		if( aMsg->theStrengthItem.theItem.theItemUID != 0 && aMsg->theStrengthItem.theWasteItem.theBundle !=0 )
		{
			/*			
			///-- 추출된 Elda Stone 
			SPcItem * tmpPcItem; 
			tmpPcItem = SAFE_NEW( SPcItem );
			
			tmpPcItem->Class	= g_Pc_Manager.GetItemClsss( aMsg->theStrengthItem.theItem.theCode );
			tmpPcItem->ItemUID	= aMsg->theStrengthItem.theItem.theItemUID;
			tmpPcItem->IType = n_ITYPE_WEAR;	///-- Elda Stone
			tmpPcItem->Amount = 1;
			tmpPcItem->Code			= aMsg->theStrengthItem.theItem.theCode;
			tmpPcItem->Tid			= (TEXID)g_Pc_Manager.GetItemTID( aMsg->theStrengthItem.theItem.theCode );				
			tmpPcItem->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aMsg->theStrengthItem.theItem.theCode );
			memcpy( &(tmpPcItem->WearTable), &(aMsg->theStrengthItem.theWearItem), sizeof( SRpdWearItemBase ) );
			
			if( tmpPcItem->ItemTable == NULL )
			{
				SAFE_DELETE( tmpPcItem );
				return;
			}
			
			tmpPcItem->WearTable.theEndurance = tmpPcItem->ItemTable->theMaxEndurance;			
			
			SAFE_DELETE( m_PcInven->InvenItem[aMsg->theStrengthItem.theItem.thePosition] );
			m_PcInven->InvenItem[aMsg->theStrengthItem.theItem.thePosition] = tmpPcItem;		
		*/
//			g_ItemSkillPotionRWndProc.OpenEnchantR_SWnd(m_PcInven->InvenItem[aMsg->theStrengthItem.theItem.thePosition]->ItemUID);
			
			g_ItemEldaMixRWndProc.FrameWnd->SetCaption(G_STRING_CHAR(IDS_WND_RESULT_WINDOWS)); 
			g_ItemEldaMixRWndProc.OpenEnchantR_SWnd(aMsg->theStrengthItem  , CItemEldaMixRWndProc::eStrengthStone );
					
		}		

	}
	else if(aMsg->theResult == n_Fail)
	{
		g_ItemEldaMixRWndProc.OpenEnchantR_FWnd(CItemEldaMixRWndProc::eStrengthStone );
	}
	else 
	{
		return;
	}
	
	///-----------------------------------------------------------------------
	///-- 아이템 제거 
	for(int i = 0; i < c_MaxInvenItemNum; ++i )
	{	
		if( m_PcInven->InvenItem[i] == NULL )
		{
			continue;
		}
		
		///-- 주문석 아이템 삭제 
		if ( m_PcInven->InvenItem[i] && 
			m_PcInven->InvenItem[i]->ItemUID == aMsg->theStrengthstone.theWasteItem.theItemUID ) 
		{
			///-- Amount 변경
			
			m_PcInven->InvenItem[i]->Amount = aMsg->theStrengthstone.theWasteItem.theBundle;
			
			///-- 아이템 완전 삭제
			if( m_PcInven->InvenItem[i]->Amount < 1 )
			{	
				SAFE_DELETE( m_PcInven->InvenItem[i] );					
			}
		}			
		
		///-- 강화이전 아이템 삭제 
		if ( m_PcInven->InvenItem[i] && 
			m_PcInven->InvenItem[i]->ItemUID == aMsg->theBeforeStrengthItem ) 
		{
			///-- Amount 변경
			
			--m_PcInven->InvenItem[i]->Amount;
			
			///-- 아이템 완전 삭제
			if( m_PcInven->InvenItem[i]->Amount < 1 )
			{	
				SAFE_DELETE( m_PcInven->InvenItem[i] );					
			}
		}
		
		for( int j = 0 ; j < 3 ; ++j)
		{
			if ( m_PcInven->InvenItem[i] && 
				m_PcInven->InvenItem[i]->ItemUID == aMsg->thePayItem[j].theItemID ) 
			{
				m_PcInven->InvenItem[i]->Amount = aMsg->thePayItem[j].theBundle;
				
				///-- 아이템 완전 삭제
				if( m_PcInven->InvenItem[i]->Amount < 1 )
				{	
					SAFE_DELETE( m_PcInven->InvenItem[i] );					
				}
			}
			 
		}


		

	}	
	
	InventoryWnd.UpdateInvenSlotItem();	


	
}



void CRockClient::CreateItemPcItem(SPcItem** tmpPcItem , URpdItem URpdItemInfo )
{
	// SPcItem객체생성

	(*tmpPcItem) = SAFE_NEW( SPcItem );
	
	WORD ItemType = g_Pc_Manager.GetItemClsss( URpdItemInfo.theItem.theCode );
	if( ( ItemType >= n_Class_Helmet ) && ( ItemType <= n_Class_Shield ) ) 
	{
		(*tmpPcItem)->IType	= n_ITYPE_WEAR;
		(*tmpPcItem)->Amount	= 1;			
		
		memcpy( &((*tmpPcItem)->WearTable), &(URpdItemInfo), sizeof( URpdItem ) );
	}
	else if( ItemType == n_Class_Use )
	{
		(*tmpPcItem)->IType	= n_ITYPE_USE;
		(*tmpPcItem)->Amount	= URpdItemInfo.theWasteItem.theBundle;
	}
	else if( ItemType == n_Class_Useless )
	{
		(*tmpPcItem)->IType	= n_ITYPE_UNUSE;
		(*tmpPcItem)->Amount	= URpdItemInfo.theWasteItem.theBundle;
	}
	//else if( ItemType == 12 ) 얼굴
	else if( ItemType == n_Class_Quest )
	{
		(*tmpPcItem)->IType	= n_ITYPE_QUEST;
		(*tmpPcItem)->Amount	= URpdItemInfo.theWasteItem.theBundle;
	}
	else if( ItemType == n_Class_Special || ItemType == n_Class_Eldastone 
		|| ItemType == n_Class_FrontierPet ||  
		ItemType == n_Class_Weapon_StrengthStone ||	ItemType == n_Class_Armor_StrengthStone)
	{
		(*tmpPcItem)->IType	= n_ITYPE_SPECIAL; 
		(*tmpPcItem)->Amount	= URpdItemInfo.theWasteItem.theBundle;
	}		
	else if( ItemType == n_Class_PersonalPet )
	{
		(*tmpPcItem)->IType	= n_ITYPE_PET_USE; 
		(*tmpPcItem)->Amount	= URpdItemInfo.theWasteItem.theBundle;
	}
	else if( ItemType == n_Class_Riding )
	{
		(*tmpPcItem)->IType	= n_ITYPE_RIDE; 
		(*tmpPcItem)->Amount	= 1;
		//말에 말굽내구도가 있기 땜시 .. 장비테이블에 설정해준다 .. 
		memcpy( &((*tmpPcItem)->WearTable), &(URpdItemInfo), sizeof( URpdItem ) );
	}
	else if( ItemType == n_Class_RidingItem )
	{
		(*tmpPcItem)->IType	= n_ITYPE_RIDE_WEAR; 
		(*tmpPcItem)->Amount	= 1;			
		memcpy( &((*tmpPcItem)->WearTable), &(URpdItemInfo), sizeof( URpdItem ) );
	}
	else if( ItemType == n_Class_Skin )
	{
		(*tmpPcItem)->IType = n_ITYPE_SKIN_WEAR;
		(*tmpPcItem)->Amount	= 1;
		memcpy( &((*tmpPcItem)->WearTable), &(URpdItemInfo) , sizeof( URpdItem ) );
	}


	(*tmpPcItem)->Class		= ItemType;
	(*tmpPcItem)->ItemUID		= URpdItemInfo.theItem.theItemUID;
	(*tmpPcItem)->Code			= URpdItemInfo.theItem.theCode;
	(*tmpPcItem)->Tid			= (TEXID)g_Pc_Manager.GetItemTID( URpdItemInfo.theItem.theCode );
	(*tmpPcItem)->ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( URpdItemInfo.theItem.theCode );
	
	if( (*tmpPcItem)->ItemTable == NULL )
	{
		SAFE_DELETE( (*tmpPcItem) );
		return;
	}
	
	////-- 현재 아이템에 관련된 딜레이가 적용되었는지 체크해서 넣어준다.					
	if( (*tmpPcItem)->IType == n_ITYPE_USE && (*tmpPcItem)->ItemTable )
	{
		 int iDelayGroup = (*tmpPcItem)->ItemTable->theDelayGroup;
		 if( !(nRui->enableDelayTime[iDelayGroup] ) )						
		 {
			(*tmpPcItem)->IsDelay = true;						
		 }
	}		

	// 왼손,오른손....
	//---------------------------------------------------------------------
	if( ( (*tmpPcItem)->ItemTable->theType == n_Type_MagicBook ) || ( (*tmpPcItem)->ItemTable->theType == n_Type_Bow ) || 
		( (*tmpPcItem)->ItemTable->theType == n_Type_Shield ) )
	{
		(*tmpPcItem)->Class = n_Class_Shield;
	}
	else if( ( (*tmpPcItem)->ItemTable->theType == n_Type_Sword ) || ( (*tmpPcItem)->ItemTable->theType == n_Type_TwohandSword ) ||
		( (*tmpPcItem)->ItemTable->theType == n_Type_Staff ) || ( (*tmpPcItem)->ItemTable->theType == n_Type_CrystalBall ) )
	{
		(*tmpPcItem)->Class = n_Class_Weapon;
	}

	//퀵슬롯에 등록된 아이템이 있냐 ? 같이 삭제해 줘라 ?? 

	for( int i = 0; i < RP_MAX_QUICK_SLOT; ++i )
	{
		if( ( m_PcInven->QuickSlot[i] != NULL ) && ( m_PcInven->QuickSlot[i]->ItemUID == (*tmpPcItem)->ItemUID ) )
		{
			m_PcInven->QuickSlot[i] = (*tmpPcItem);	
	
			break;
		}
	}
	
	SAFE_DELETE( m_PcInven->InvenItem[URpdItemInfo.theItem.thePosition] );	
	//인벤에 기억한다.
	m_PcInven->InvenItem[URpdItemInfo.theItem.thePosition] = (*tmpPcItem);
	
	MainMenuWnd.UpdateQuickSlotItem();
	InventoryWnd.UpdateInvenSlotItem();

}
#ifdef WIDE_PAY_INVENTORY //유료인벤확장
void CRockClient::Proc_SRpScPayInven( SRpScPayInven * aMsg )
{
	InventoryWnd.GetInvenSlot(aMsg->thePayInvenSlotNum);  // 유료인벤확장 아이템 갯수 받아오기
	

	BYTE* pBuf = (BYTE*) &(aMsg->thePayInvenList);
	
	URpdItem PayItemList[RP_MAX_PAYINVENTORY];	//유료인벤 아이템 리스트	
	
	if( aMsg->theItemNum >= 1 )
	{
		memcpy( PayItemList, &pBuf[0], sizeof( URpdItem ) * aMsg->theItemNum );
	}

	InventoryWnd.SetPayInvenPcItem( aMsg->theItemNum, PayItemList ); //서버에서받은 리스트 클라로 보내기		

	
	//g_PayInvenWnd.Open(true);
	
	return;

}

void CRockClient::Proc_SRpScPayInvenAdd( _SRpScPayInvenAdd * aMsg )
{
	InventoryWnd.GetInvenSlot(aMsg->thePayInvenSlotNum); // 유료인벤확장 아이템 갯수 받아오기
	
	if(aMsg->theResult == 1)
	{
		SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_USEPAYINVEN_ADDITEM_MSG  ) );
	}
	
}
#endif //WIDE_PAY_INVENTORY

#ifdef TAIWAN30MINEVENT
void CRockClient::Proc_SRpScTaiwan30minEvent( SRpScTaiwan30minEvent * aMsg )
{
	g_main_buttons.GetSignalEnd( aMsg->theResult );
}
void CRockClient::Proc_RpScTaiwan30minEventStart( SRpScTaiwan30minEventStart * aMsg )
{
	g_main_buttons.GetSignalStart( aMsg->result);
}
#endif //TAIWAN30MINEVENT