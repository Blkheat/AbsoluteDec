#include "CompntCommon.h"

#include "LoadingWndProc.h"
#include "LoginWndProc.h"
#include "ServerListWndProc.h"
#include "SystemMenuWndProc.h"
#include "MainMenuWndProc.h"
#include "PlayerInfoWnd.h"
#include "PlayerInfoAddWnd.h"
#include "NonPlayerInfoWnd.h"
#include "NonPlayerInfoAddWnd.h"
#include "ChantHitBarWnd.h"
#include "CDelayBarWnd.h"
#include "InventoryWndProc.h"
#include "QuestProc.h"
#include "NpcChatWnd.h"
#include "CharStateWndProc.h"
#include "ProposeJoinFrontier.h"


#include "AbilityUpWndProc.h"
#include "AbilityUpConfirmWnd.h"
#include "SkillWndProc.h"
#include "SkillRegistWnd.h"
#include "SkillLevelUpWnd.h"
#include "SkillInfoWnd.h"
#include "SkillOverWriteWnd.h"
#include "MiniMapWndProc.h"
#include "ExchangeWndProc.h"
#include "SystemMenuWndProc.h"
#include "MessageWndProc.h"
#include "StoreWndProc.h"
#include "BankWndProc.h"
#include "CArcherBar.h"

#include "ServerListWndProc.h"
#include "MainMenuWndProc.h"
#include "ChatBoxWndProc.h"

#include "CharSelectWndProc.h"
#include "MessageWndProc.h"
#include "AliveWnd.h"

#include "NumberInputWnd.h"


#include "SkillLearnProc.h"
#include "SysMsgWndProc.h"


#include "CPAcceptYesNoWnd.h"
#include "UIMsgWnd.h"
#include "PartyInforWnd.h"
#include "PartyPopupWnd.h"

//아이템 
#include "WarehouseWndProc.h"
#include "GuildInventoryWndProc.h"
#include "ItemEnchantWndProc.h"
#include "ItemEnchantRWndProc.h"
#include "ItemExtractWndProc.h"
#include "ItemExtractRWndProc.h"
#include "ItemRemodelWndProc.h"
#include "ItemRemodel_FWndProc.h"
#include "ItemRemodel_SWndProc.h"
#include "ItemRepairWndProc.h"
#include "ItemCreateWndProc.h"
#include "ItemCreate_FWndProc.h"
#include "ItemCreate_SWndProc.h"
#include "ItemAccEnchant.h"
#include "ItemAccEnchantRWndProc.h"


#include "MCommunityWnd.h"
#include "MessengerMemoWnd.h"
#include "messageBox.h"
#include "OptionWndProc.h"
#include "BattleZoneWndProc.h"
#include "CLogWnd.h"

#include "ChatMainProc.h"
#include "SystemPrintWnd.h"

#include "CUserStoreSellWnd.h"
#include "CUserStoreTitleWnd.h"
#include "CUserStoreSetWnd.h"
#include "CUserStoreBuyWnd.h"
#include "CUserStoreSetBuyWnd.h"

#include "CSocialActWnd.h"
#include "CWebShopWnd.h"
#include "PayItemWndProc.h"

#include "FrontierCreateWnd.h"
#include "FrontierMainWndProc.h"

#include "LeagueInfoWnd.h"


#include "PetInfoWnd.h"
#include "PetInventoryWnd.h"
#include "PetMainWndProc.h"


#include "ItemEldaMixAWndProc.h"
#include "ItemEldaMixRWndProc.h"
#include "ItemSkillPotionWndProc.h"
#include "ItemSkillPotionRWndProc.h"
#include "ItemSelectPotionWndProc.h"




///-- 콜로니 관련
#include "CColRegionListWnd.h"
#include "CColRegionInfoWnd.h"
#include "CColItemPreViewWnd.h"
#include "CColMapPositionWnd.h"

#include "CGuideWnd.h"

#include "RadioBroadcast.h"
#include "AddEffectMsg.h"

#include "CAcryEnchantWnd.h"
#include "CAcryEnchantRWnd.h"

#include "CChangeEldaColorWnd.h"
#include "CChangeEldaColorRWnd.h"

#include "ItemEldaDust.h"
#include "ItemEldaDustRWndProc.h"


#include "EssenceWndProc.h"
#include "SpellStoneWndProc.h"
#include "StrengthStoneWndProc.h"

#include "CharCreateWndProc.h"
#include "CharDelWndProc.h"



extern long g_nowTime;

//-----------------------------------------------------------------------------
void CompntCommon()
{
	//-------------------------------------------------------------------------
	//
	//	로딩바
	//
	//-------------------------------------------------------------------------
	
	LoadingBarWnd.Composition();
	
	
	//-------------------------------------------------------------------------
	//
	//	로그인
	//
	//-------------------------------------------------------------------------
	LoginWnd.Composition();
	
	
	//-------------------------------------------------------------------------
	//
	//  서버 리스트
	//
	//-------------------------------------------------------------------------
	ServerListWnd.Composition();


	//-------------------------------------------------------------------------
	//
	//	시스템 메뉴
	//
	//-------------------------------------------------------------------------
	SystemMenuWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	메인메뉴
	//
	//-------------------------------------------------------------------------
	///MainMenuLVgageWnd.Composition();
	MainMenuWnd.Composition();
	g_main_buttons.Composition();
	g_Exp_wnd.Composition();
	g_endu_warning.Composition();
	
	//-------------------------------------------------------------------------
	//
	//	유저 정보
	//
	//-------------------------------------------------------------------------
	PlayerInfoWnd.Composition();
	PlayerInfoAddWnd.Composition();	
	
	//-------------------------------------------------------------------------
	//
	//	다른 유저 및 NPC 정보
	//
	//-------------------------------------------------------------------------
	NonPlayerInfoWnd.Composition();
	NonPlayerInfoAddWnd.Composition();
	g_NonPlayerInfoAdd2Wnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	HitBar Window
	//
	//-------------------------------------------------------------------------
	g_ChantHitBarWnd.Composition();

	//-------------------------------------------------------------------------- 
    //
    //  딜레이 바 윈도우
    //
    //-------------------------------------------------------------------------- 
    g_DelayBarWnd.Composition();
	
	//-------------------------------------------------------------------------
	//
	//	인벤토리
	//
	//-------------------------------------------------------------------------
	InventoryWnd.Composition();
	g_MoneyThrowWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	퀘스트 창
	//
	//-------------------------------------------------------------------------
	QuestWnd.Composition();		


	//-------------------------------------------------------------------------
	//
	//	NPC 대화 창
	//
	//-------------------------------------------------------------------------
	NpcChatWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	캐릭터 상태 윈도우
	//
	//-------------------------------------------------------------------------
	CharStateWnd.Composition();
	
	//-------------------------------------------------------------------------
	//
	//	능력업 처리 윈도우
	//
	//-------------------------------------------------------------------------
	AbilityUpWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	능력업 확인취소 윈도우
	//
	//-------------------------------------------------------------------------
	AbilityUpConfirmWnd.Composition();	
	
	//-------------------------------------------------------------------------
	//
	// 부활창 
	//
	//-------------------------------------------------------------------------
	g_AliveWnd.Composition();
	
	//-------------------------------------------------------------------------
	//
	// 부활창 - 유료 아이템 창 
	//
	//-------------------------------------------------------------------------
	g_AliveChargeWnd.Composition();
	

	//-------------------------------------------------------------------------
	//
	//	채팅창
	//
	//-------------------------------------------------------------------------
	ChatMessageWnd.Composition();
	ChatBoxWnd.Composition();
	ChatOutputWnd.Composition();
	
	//-------------------------------------------------------------------------
	//
	//	미니맵
	//
	//-------------------------------------------------------------------------
	MiniMapWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	교환창
	//
	//-------------------------------------------------------------------------
	g_ExchangeWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	스킬
	//
	//-------------------------------------------------------------------------
	g_SkillWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	단축키 변경 창 
	//
	//-------------------------------------------------------------------------
//	SkillRegistWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	스킬레벨업처리창
	//
	//-------------------------------------------------------------------------
	g_SkillLevelUpWnd.Composition();

	

	//-------------------------------------------------------------------------
	//
	//	스킬정보창
	//
	//-------------------------------------------------------------------------
//	SkillInfoWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	단축키 변경 확인창
	//
	//-------------------------------------------------------------------------
//	SkillOverWriteWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	상점 거래
	//
	//-------------------------------------------------------------------------
	StoreWnd.Composition();
	StoreBuyWnd.Composition();
	StoreSellWnd.Composition();
		
	//-------------------------------------------------------------------------
	//
	//	메세지 창
	//
	//-------------------------------------------------------------------------
	MessageBoxWnd.Composition();
	g_NumberInputWnd.Composition();
	g_StrInputWnd.Composition();
	///-- MessageBox
	g_MessageBox.Composition();

	// 시스템 메세지창
	SysMsgOutputWnd.Composition();

	

	//-------------------------------------------------------------------------
	//
	//	캐릭터 선택창
	//
	//-------------------------------------------------------------------------
	CharSelectWnd.Composition();
	g_CharCreateWndProc.Composition();
#ifdef DECO_RENEWAL_MJH
	g_CharCreateHairProc.Composition();
#endif // DECO_RENEWAL_MJH
	
	// 스킬 습득창
	SkillLearnWnd.Composition();

	// 수락여부창 - 현재는 파티초대에 대한 수락만을 고려
	//by Simwoosung
	g_AcceptYesNoWnd.Composition();

	//파티 정보창
	g_PartyInforWnd.Composition();
    
	//파티-팝업메뉴창
	g_PartyPopupWnd.Composition();
		
	//-------------------------------------------------------------------------
	// 창고창 
	//-------------------------------------------------------------------------
	g_WarehouseWnd.Composition();
	
	//-------------------------------------------------------------------------
	// 길드 창고창 
	//-------------------------------------------------------------------------
#ifdef DECO_RENEWAL_GUILD_MJH
	g_GuildInvenWnd.Composition();
#endif // DECO_RENEWAL_GUILD_MJH
	
	
	//-------------------------------------------------------------------------
	//
	// 아처 컨디션 게이지
	//
	//-------------------------------------------------------------------------
	g_ArcherBar.Composition();
	

	///-----------------------------------------------------------------------
	///-- 아이템 수리 윈도 
	///-----------------------------------------------------------------------
	g_ItemRepair_WndProc.Composition();

	///-----------------------------------------------------------------------
	///-- SubClass Elda Craft
	///-----------------------------------------------------------------------
	///-- 아이템 강화
	g_ItemEnchantWndProc.Composition();
	g_ItemEnchant_RWndProc.Composition();
	g_ItemAccEnchant.Composition();
	g_ItemAccEnchant_RWndProc.Composition();

	
	///-- 엘다 추출
//	g_ItemExtract_WndProc.Composition();
//	g_ItemExtract_R_WndProc.Composition();
	
	///-- 가공
//	g_ItemRemodel_F_WndProc.Composition();
//	g_ItemRemodel_S_WndProc.Composition();
//	g_ItemRemodel_WndProc.Composition();

	///-- 아이템 제작
	g_ItemCreate_WndProc.Composition();

	///-- 제조 결과 성공/실패
	g_ItemCreate_S_WndProc.Composition();
	g_ItemCreate_F_WndProc.Composition();


	//-------------------------------------------------------------------------
	// 메신저
	//-------------------------------------------------------------------------
	g_MCommunityWnd.Composition();
	//-------------------------------------------------------------------------
	// 메신저 - 메모창
	//-------------------------------------------------------------------------
	g_MessengerMemoWnd.Composition();

	//-------------------------------------------------------------------------
	// 채팅창
	//-------------------------------------------------------------------------
	g_ChatMainProc.Composition();	

	//-------------------------------------------------------------------------
	// 시스템 출력창
	//-------------------------------------------------------------------------
	g_SystemPrintWnd.Composition();	
	
	
	//by simwoosung - UI 메시지창 -- 이게 항상 맨밑으로
	g_UIMsgWnd.Composition();
	
	//-------------------------------------------------------------------------
	// 옵 션 창 By wxywxy
	//-------------------------------------------------------------------------
	g_OptionWnd.Composition();

	//-------------------------------------------------------------------------
	// PVP Battle Zone System 
	//-------------------------------------------------------------------------
	// 공식전, 비공식전 게임 선택 
	g_BZ_SelectZoneWnd.Composition();

	// 레벨별 등급 선택   
	g_BZ_SelectGradeWnd.Composition();

	// 대전장 리스트 
	g_BZ_RoomListWnd.Composition();

	// 방 만들기 
	g_BZ_CreateRoomWnd.Composition();
	
	// 대기방 
	g_BZ_WaitingRoomWnd.Composition();

	// 게임 결과창 
	g_BZ_GameResultWnd.Composition();
	//-------------------------------------------------------------------------	
	

	//-------------------------------------------------------------------------
	// 콤보 / 서브직업 기록창
	//-------------------------------------------------------------------------
	g_LogWnd.Composition();	
	

	//..........................................................................................................
	// 개인 상점
	//..........................................................................................................
	g_UserStoreTitleWnd.Composition();
	g_UserStoreSellWnd.Composition();	
	g_UserStoreSetWnd.Composition();	
	g_UserStoreBuyWnd.Composition();	
	g_UserStoreSetBuyWnd.Composition();
	
	// 사교 동작창 
	g_SocialActWnd.Composition();

	//..........................................................................................................
	// 웹 상점 By wxywxy
	//..........................................................................................................
	g_WebShopWnd.Composition();

	// 구매 아이템 리스트 창 
	g_PayItemWnd.Composition();	

	// 프론티어 생성창
	g_FrontierCreateWnd.Composition();
#ifdef DECO_RENEWAL_GUILD_MJH 
    g_FrontierCreateScrollWnd.Composition(); //길드 가입명
	g_FrontierMenuBtnWnd.Composition();
	g_ProposeJoinFrontier.Composition();
#endif //DECO_RENEWAL_GUILD_MJH
	
	// 프론티어 메인 정보창 
	g_FrontierMainWnd.Composition();

	// 프론티어 멤버 정보창 
	g_FrontierMemberListWnd.Composition();

	// 프론티어 멤버 정보 수정창 
	g_FrontierMemberEditWnd.Composition();


	g_FrontierAddPersons.Composition();

	
	// 리그 멤버 정보창
	g_LeagueInfoWnd.Composition();

	
	//펫상태 창 
	g_PetInfoWnd.Composition();	

	//"PetInventoryWnd.h"
	g_PetMainWndProc.Composition();

	//펫 이름 생성
	g_PetCreateWnd.Composition();

	//펫 인벤토리 
	g_PetInventoryWnd.Composition();
	
	//엘다스톤 

	#ifdef MIX_ELDASTONE
	g_ItemEldaMixAWndProc.Composition();
	#endif 
	

	g_ItemSkillPotionWndProc.Composition();
	g_ItemSelectPotionWndProc.Composition();
	g_ItemSkillPotionRWndProc.Composition();
	g_ItemEldaMixRWndProc.Composition();


	g_CharDelWndProc.Composition();
	
	///-- By simwoosung
	///-- 콜로니 관련 UI
	///-- 콜로니 리스트 UI
//	g_ColRegionListWnd.Composition();
	///-- 콜로니 관련정보 UI
//	g_ColRegionInfoWnd.Composition();
	///-- 콜로니 공성/수성병기 맵배치창
//	g_ColMapPositionWnd->Composition();	

	///-- 가이드창 - 모험백서
	g_pGuideWnd->Composition();

	
#ifdef C_SEALED_ELDASTONE_DUST
	g_ItemEldaDustWndProc.Composition();
	g_ItemEldaDust_RWndProc.Composition();
	
#endif 


	//라이오 방송 버튼 
	#ifdef CHINA_VERSION
	g_pRadioBroadcast->Composition();
	#endif
	//--메인 화면에 뛰우는 부가효과
	g_pAddEffectMsg->Composition();

	///-- 장신구 가공창
	g_pAcryEnchantWnd->Composition();
	///-- 장신구 가공 결과창
	g_pAcryEnchantRWnd->Composition();
	
	//엘다스톤 색상 변경창
	g_pChangeEldaColorWnd->Composition(); 
	//엘다스톤 색상 변경 결과창
	g_pChangeEldaColorRWnd->Composition();

#ifdef C_SUBJOB_RENEW
	g_EssenceWndProc->Composition();
	g_SpellStoneWndProc->Composition();
	g_StrengthStoneWndProc->Composition();
#endif 	


	return;
}

//-----------------------------------------------------------------------------