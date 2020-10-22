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

//������ 
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




///-- �ݷδ� ����
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
	//	�ε���
	//
	//-------------------------------------------------------------------------
	
	LoadingBarWnd.Composition();
	
	
	//-------------------------------------------------------------------------
	//
	//	�α���
	//
	//-------------------------------------------------------------------------
	LoginWnd.Composition();
	
	
	//-------------------------------------------------------------------------
	//
	//  ���� ����Ʈ
	//
	//-------------------------------------------------------------------------
	ServerListWnd.Composition();


	//-------------------------------------------------------------------------
	//
	//	�ý��� �޴�
	//
	//-------------------------------------------------------------------------
	SystemMenuWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	���θ޴�
	//
	//-------------------------------------------------------------------------
	///MainMenuLVgageWnd.Composition();
	MainMenuWnd.Composition();
	g_main_buttons.Composition();
	g_Exp_wnd.Composition();
	g_endu_warning.Composition();
	
	//-------------------------------------------------------------------------
	//
	//	���� ����
	//
	//-------------------------------------------------------------------------
	PlayerInfoWnd.Composition();
	PlayerInfoAddWnd.Composition();	
	
	//-------------------------------------------------------------------------
	//
	//	�ٸ� ���� �� NPC ����
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
    //  ������ �� ������
    //
    //-------------------------------------------------------------------------- 
    g_DelayBarWnd.Composition();
	
	//-------------------------------------------------------------------------
	//
	//	�κ��丮
	//
	//-------------------------------------------------------------------------
	InventoryWnd.Composition();
	g_MoneyThrowWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	����Ʈ â
	//
	//-------------------------------------------------------------------------
	QuestWnd.Composition();		


	//-------------------------------------------------------------------------
	//
	//	NPC ��ȭ â
	//
	//-------------------------------------------------------------------------
	NpcChatWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	ĳ���� ���� ������
	//
	//-------------------------------------------------------------------------
	CharStateWnd.Composition();
	
	//-------------------------------------------------------------------------
	//
	//	�ɷ¾� ó�� ������
	//
	//-------------------------------------------------------------------------
	AbilityUpWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	�ɷ¾� Ȯ����� ������
	//
	//-------------------------------------------------------------------------
	AbilityUpConfirmWnd.Composition();	
	
	//-------------------------------------------------------------------------
	//
	// ��Ȱâ 
	//
	//-------------------------------------------------------------------------
	g_AliveWnd.Composition();
	
	//-------------------------------------------------------------------------
	//
	// ��Ȱâ - ���� ������ â 
	//
	//-------------------------------------------------------------------------
	g_AliveChargeWnd.Composition();
	

	//-------------------------------------------------------------------------
	//
	//	ä��â
	//
	//-------------------------------------------------------------------------
	ChatMessageWnd.Composition();
	ChatBoxWnd.Composition();
	ChatOutputWnd.Composition();
	
	//-------------------------------------------------------------------------
	//
	//	�̴ϸ�
	//
	//-------------------------------------------------------------------------
	MiniMapWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	��ȯâ
	//
	//-------------------------------------------------------------------------
	g_ExchangeWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	��ų
	//
	//-------------------------------------------------------------------------
	g_SkillWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	����Ű ���� â 
	//
	//-------------------------------------------------------------------------
//	SkillRegistWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	��ų������ó��â
	//
	//-------------------------------------------------------------------------
	g_SkillLevelUpWnd.Composition();

	

	//-------------------------------------------------------------------------
	//
	//	��ų����â
	//
	//-------------------------------------------------------------------------
//	SkillInfoWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	����Ű ���� Ȯ��â
	//
	//-------------------------------------------------------------------------
//	SkillOverWriteWnd.Composition();

	//-------------------------------------------------------------------------
	//
	//	���� �ŷ�
	//
	//-------------------------------------------------------------------------
	StoreWnd.Composition();
	StoreBuyWnd.Composition();
	StoreSellWnd.Composition();
		
	//-------------------------------------------------------------------------
	//
	//	�޼��� â
	//
	//-------------------------------------------------------------------------
	MessageBoxWnd.Composition();
	g_NumberInputWnd.Composition();
	g_StrInputWnd.Composition();
	///-- MessageBox
	g_MessageBox.Composition();

	// �ý��� �޼���â
	SysMsgOutputWnd.Composition();

	

	//-------------------------------------------------------------------------
	//
	//	ĳ���� ����â
	//
	//-------------------------------------------------------------------------
	CharSelectWnd.Composition();
	g_CharCreateWndProc.Composition();
#ifdef DECO_RENEWAL_MJH
	g_CharCreateHairProc.Composition();
#endif // DECO_RENEWAL_MJH
	
	// ��ų ����â
	SkillLearnWnd.Composition();

	// ��������â - ����� ��Ƽ�ʴ뿡 ���� �������� ���
	//by Simwoosung
	g_AcceptYesNoWnd.Composition();

	//��Ƽ ����â
	g_PartyInforWnd.Composition();
    
	//��Ƽ-�˾��޴�â
	g_PartyPopupWnd.Composition();
		
	//-------------------------------------------------------------------------
	// â��â 
	//-------------------------------------------------------------------------
	g_WarehouseWnd.Composition();
	
	//-------------------------------------------------------------------------
	// ��� â��â 
	//-------------------------------------------------------------------------
#ifdef DECO_RENEWAL_GUILD_MJH
	g_GuildInvenWnd.Composition();
#endif // DECO_RENEWAL_GUILD_MJH
	
	
	//-------------------------------------------------------------------------
	//
	// ��ó ����� ������
	//
	//-------------------------------------------------------------------------
	g_ArcherBar.Composition();
	

	///-----------------------------------------------------------------------
	///-- ������ ���� ���� 
	///-----------------------------------------------------------------------
	g_ItemRepair_WndProc.Composition();

	///-----------------------------------------------------------------------
	///-- SubClass Elda Craft
	///-----------------------------------------------------------------------
	///-- ������ ��ȭ
	g_ItemEnchantWndProc.Composition();
	g_ItemEnchant_RWndProc.Composition();
	g_ItemAccEnchant.Composition();
	g_ItemAccEnchant_RWndProc.Composition();

	
	///-- ���� ����
//	g_ItemExtract_WndProc.Composition();
//	g_ItemExtract_R_WndProc.Composition();
	
	///-- ����
//	g_ItemRemodel_F_WndProc.Composition();
//	g_ItemRemodel_S_WndProc.Composition();
//	g_ItemRemodel_WndProc.Composition();

	///-- ������ ����
	g_ItemCreate_WndProc.Composition();

	///-- ���� ��� ����/����
	g_ItemCreate_S_WndProc.Composition();
	g_ItemCreate_F_WndProc.Composition();


	//-------------------------------------------------------------------------
	// �޽���
	//-------------------------------------------------------------------------
	g_MCommunityWnd.Composition();
	//-------------------------------------------------------------------------
	// �޽��� - �޸�â
	//-------------------------------------------------------------------------
	g_MessengerMemoWnd.Composition();

	//-------------------------------------------------------------------------
	// ä��â
	//-------------------------------------------------------------------------
	g_ChatMainProc.Composition();	

	//-------------------------------------------------------------------------
	// �ý��� ���â
	//-------------------------------------------------------------------------
	g_SystemPrintWnd.Composition();	
	
	
	//by simwoosung - UI �޽���â -- �̰� �׻� �ǹ�����
	g_UIMsgWnd.Composition();
	
	//-------------------------------------------------------------------------
	// �� �� â By wxywxy
	//-------------------------------------------------------------------------
	g_OptionWnd.Composition();

	//-------------------------------------------------------------------------
	// PVP Battle Zone System 
	//-------------------------------------------------------------------------
	// ������, ������� ���� ���� 
	g_BZ_SelectZoneWnd.Composition();

	// ������ ��� ����   
	g_BZ_SelectGradeWnd.Composition();

	// ������ ����Ʈ 
	g_BZ_RoomListWnd.Composition();

	// �� ����� 
	g_BZ_CreateRoomWnd.Composition();
	
	// ���� 
	g_BZ_WaitingRoomWnd.Composition();

	// ���� ���â 
	g_BZ_GameResultWnd.Composition();
	//-------------------------------------------------------------------------	
	

	//-------------------------------------------------------------------------
	// �޺� / �������� ���â
	//-------------------------------------------------------------------------
	g_LogWnd.Composition();	
	

	//..........................................................................................................
	// ���� ����
	//..........................................................................................................
	g_UserStoreTitleWnd.Composition();
	g_UserStoreSellWnd.Composition();	
	g_UserStoreSetWnd.Composition();	
	g_UserStoreBuyWnd.Composition();	
	g_UserStoreSetBuyWnd.Composition();
	
	// �米 ����â 
	g_SocialActWnd.Composition();

	//..........................................................................................................
	// �� ���� By wxywxy
	//..........................................................................................................
	g_WebShopWnd.Composition();

	// ���� ������ ����Ʈ â 
	g_PayItemWnd.Composition();	

	// ����Ƽ�� ����â
	g_FrontierCreateWnd.Composition();
#ifdef DECO_RENEWAL_GUILD_MJH 
    g_FrontierCreateScrollWnd.Composition(); //��� ���Ը�
	g_FrontierMenuBtnWnd.Composition();
	g_ProposeJoinFrontier.Composition();
#endif //DECO_RENEWAL_GUILD_MJH
	
	// ����Ƽ�� ���� ����â 
	g_FrontierMainWnd.Composition();

	// ����Ƽ�� ��� ����â 
	g_FrontierMemberListWnd.Composition();

	// ����Ƽ�� ��� ���� ����â 
	g_FrontierMemberEditWnd.Composition();


	g_FrontierAddPersons.Composition();

	
	// ���� ��� ����â
	g_LeagueInfoWnd.Composition();

	
	//����� â 
	g_PetInfoWnd.Composition();	

	//"PetInventoryWnd.h"
	g_PetMainWndProc.Composition();

	//�� �̸� ����
	g_PetCreateWnd.Composition();

	//�� �κ��丮 
	g_PetInventoryWnd.Composition();
	
	//���ٽ��� 

	#ifdef MIX_ELDASTONE
	g_ItemEldaMixAWndProc.Composition();
	#endif 
	

	g_ItemSkillPotionWndProc.Composition();
	g_ItemSelectPotionWndProc.Composition();
	g_ItemSkillPotionRWndProc.Composition();
	g_ItemEldaMixRWndProc.Composition();


	g_CharDelWndProc.Composition();
	
	///-- By simwoosung
	///-- �ݷδ� ���� UI
	///-- �ݷδ� ����Ʈ UI
//	g_ColRegionListWnd.Composition();
	///-- �ݷδ� �������� UI
//	g_ColRegionInfoWnd.Composition();
	///-- �ݷδ� ����/�������� �ʹ�ġâ
//	g_ColMapPositionWnd->Composition();	

	///-- ���̵�â - ����鼭
	g_pGuideWnd->Composition();

	
#ifdef C_SEALED_ELDASTONE_DUST
	g_ItemEldaDustWndProc.Composition();
	g_ItemEldaDust_RWndProc.Composition();
	
#endif 


	//���̿� ��� ��ư 
	#ifdef CHINA_VERSION
	g_pRadioBroadcast->Composition();
	#endif
	//--���� ȭ�鿡 �ٿ�� �ΰ�ȿ��
	g_pAddEffectMsg->Composition();

	///-- ��ű� ����â
	g_pAcryEnchantWnd->Composition();
	///-- ��ű� ���� ���â
	g_pAcryEnchantRWnd->Composition();
	
	//���ٽ��� ���� ����â
	g_pChangeEldaColorWnd->Composition(); 
	//���ٽ��� ���� ���� ���â
	g_pChangeEldaColorRWnd->Composition();

#ifdef C_SUBJOB_RENEW
	g_EssenceWndProc->Composition();
	g_SpellStoneWndProc->Composition();
	g_StrengthStoneWndProc->Composition();
#endif 	


	return;
}

//-----------------------------------------------------------------------------