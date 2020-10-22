// NetworkMgr.h: interface for the CNetworkMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__NETWORKMGR_H_INCLUDED__)
#define __NETWORKMGR_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PacketCrypto.h"

#include "Net\\RockNetConnectSocket.h"
#include "Protocols\\FrameworkProtocols.h"
#include "Protocols\\GameProtocols.h"


#define	MAX_PACKET_SIZE	4096

enum	NConnectState
{
	nNoConnect,
	nInLoginServer,
	nInGameServer
};

class CNetworkMgr  
{
public:
	CNetworkMgr();
	virtual ~CNetworkMgr();

	BOOL		WinSockStart();
	void		WinSockClose();

	inline void	SetWaitingSvrMsg( BOOL aFlag = TRUE )	{	theWaitingSvrMsgFlag = aFlag;	}
	inline BOOL IsWaitingSvrMsg()						{	return( theWaitingSvrMsgFlag );	}

	inline int	GetConnectState()	{	return theConnectState;	}
	BOOL		ConnectToLoginServer( char* aLoginServerIP, int aLoginServerPort );
	BOOL		ConnectToGameServer( int aListIndex, char* aGameServerName );
	BOOL		ConnectToGameServer( int aListIndex );
	BOOL		SelectCharacter( int aListIndex, char* aCharacterName );
	BOOL		SelectCharacter( int aListIndex );

	/// �̰� ���� Loop���� �� frame ���� ȣ���մϴ�.
	void		NetworkProc();
	inline BOOL	IsDisConnected() {	return( ( theConnectState == nNoConnect )? TRUE:FALSE );	}

	void		CleanUpSocketAll();

	int			SendMsg2LS( void* aMsgPtr, int aSize );
	int			SendMsg2GS( void* aMsgPtr, int aSize );

	//-----------------------------------------------------------
	// Framework Related
	//-----------------------------------------------------------
	void		Send_RpCsKeepAliveCheck( SRpScKeepAliveCheck* aMsg );
	void		Send_RpCsAccountLogin( char* aAccountName, char* aPasswd, UINT uForm );
	///void		Send_RpCsSelectWorld( const BYTE a8_world_idx );
	void		Send_RpCsGameLogin();
	void		Send_RpCsSelectCharacter( char* aPCName );
	void		Send_RpCsPCLogin();
	void		Send_RpCsPCLogout( int aLogoutType );
#ifdef DECO_RENEWAL_MJH
	void		Send_RpCsCreateCharacter(int SelectNum,DWORD Class ,char *Name,BYTE Citizen,BYTE Gender,DWORD Hair,DWORD Face);
#else
	void		Send_RpCsCreateCharacter(int SelectNum,char *Name,BYTE Citizen,BYTE Gender,DWORD Hair,DWORD Face);
#endif // DECO_RENEWAL_MJH
	void        Send_RpCsDeleteCharacter(char *Name);
	
	//-----------------------------------------------------------
	// Other Game Protocols
	//-----------------------------------------------------------
	void		Send_RpCsStartMove( DWORD aMoveID, DWORD aDestiCell, BYTE aMoveSpeed );
	void		Send_RpCsMove( DWORD aMoveID, DWORD aCell, float aAngle );
	///--JAPAN_BUG_MODIFY
	void		Send_RpCsStop( DWORD aCell, float aAngle, DWORD dPCID = 0 );	
	void		Send_RpCsRequestWarp( int aGateIndex );
	void		Send_RpCsReadyWarp();	
	void		Send_RpCsAlive(DWORD thePayItem);
	void		Send_RpCsHotKey( BYTE aKeyType, WORD aSkillCode );
	void		Send_RpCsReqCharInfo( DWORD aChrID, BYTE aChrType );	
	void		Send_RpCsDirection( float aAngle );

	//-----------------------------------------------------------
	// Attack or Skill Protocols
	//-----------------------------------------------------------
	void		Send_RpCsAttack( DWORD aAttackID, DWORD aAttackedID, BYTE aAttackedType, WORD aMotion, BYTE aComboType, BYTE aComboCount, BYTE aComboStatus, BYTE aComboSeq = 0 );
	void		Send_RpCsSkill( DWORD aSkillID, DWORD aTarget, BYTE aTargetType, WORD aSkillCode );// ��ų ��� ��û
	void		Send_RpCsSpelling( BYTE aChantCount );
	void		Send_RpCsKeepUpSkillEnd( WORD aSkillCode );
	void		Send_RpCsGetSkill( WORD aSkillCode );	
	void		Send_CsSkillUp( DWORD aSkillCode ); 

		
	//-----------------------------------------------------------
	// Chatting Protocols
	//-----------------------------------------------------------
	void		Send_RpCsSay( TCHAR* aSayMessage );
	void		Send_RpCsWhisper( TCHAR* aReceiver, TCHAR* aSayMessage );
	
	//--------------------------------------------------
	// Item Protocols
	//--------------------------------------------------
	void		Send_RpCsPickup( DWORD aItemUID );
	void		Send_RpCsDrop( DWORD aItemUID, BYTE aBundle, BYTE aItemTab );
	void		Send_RpCsDropEld( DWORD aEld );
	void		Send_RpCsUse( DWORD aItemUID, BYTE aColonyPos = 0 );
	void		Send_RpCsEquip( DWORD aItemUID, BYTE aPosition );
	void		Send_RpCsTakeoff( DWORD aItemUID, BYTE aPosition );
	void		Send_RpCsBuy( DWORD aNPCID, WORD aItemCode, BYTE aBundle, BYTE aPosition );
	void		Send_RpCsSell( DWORD aNPCID, DWORD aItemUID, BYTE aBundle, BYTE aItemTab );
	void		Send_RpCsCombine( DWORD aItemUID1, DWORD aItemUID2, BYTE aItemTab );
	void		Send_RpCsChangeSlot( DWORD aItemUID1, DWORD aItemUID2, BYTE aSlot, BYTE aItemTab );
	void		Send_RpCsLinkQuickSlot( SRpdQuickSlot aQslot );
	void		Send_RpCsUnLinkQuickSlot( BYTE aPosition );


	//--------------------------------------------------
	// Exp, LevelUp Protocols
	//--------------------------------------------------
	void		Send_RpCsPoint( BYTE aPower, BYTE aVital, BYTE aSympathy, BYTE aInt, BYTE aStamina, BYTE aDex );


	//--------------------------------------------------
	// Npc Chatting, Script, Quest Protoclos
	//--------------------------------------------------
	void		Send_RpCsScriptStart( DWORD NPCID, WORD ScriptCode );
	void		Send_RpCsScriptEnd( WORD aNpcCode );
	void		Send_RpCsAnswerScript( BYTE aAnswer );


	//--------------------------------------------------
	// Quest
	//--------------------------------------------------
	void		Send_RpCsGiveupQuest( WORD aQuestCode );

#ifdef TAIWAN30MINEVENT
	//--------------------------------------------------
	// Event
	//--------------------------------------------------
	void        Send_SRpCsTaiwan30minEvent();
	void        Send_SRpCsTaiwan30minEventStart();
#endif //TAIWAN30MINEVENT
	
	//--------------------------------------------------
	// Archer
	//--------------------------------------------------
	void		Send_RpCsArcherReady(DWORD aTarget,BYTE aTargetType);
	
	//--------------------------------------------------
	// Delay Reason //by simwoosung
	//--------------------------------------------------	
    void		Send_RpCs10SecDelay( BYTE BDelayReason );

	//--------------------------------------------------
	// Party Protocols                   //by simwoosung
	//--------------------------------------------------
    void		Send_RpCsPartyAskJoin( TCHAR * aPCName, BYTE PartyType );
	void		Send_RpCsPartyAnsJoin( TCHAR * aLeaderName, BYTE aAnswer);
	void		Send_RpCsPartyLeave( TCHAR * aPCName );
	void		Send_RpCsPartyChangeName( TCHAR * aPartyName);
	void		Send_RpCsPartyChat(TCHAR *aString);
	void		Send_RpCsChangeLeader(TCHAR * aGiveLeader);
	void		Send_RpCsPartyItemMgr(BYTE aHowRoot);
	void		Send_RpCsPartyGiveGage(DWORD aPCID, BYTE aGage);

	//-----------------------------------------------------------
	// Exchange Protocols	
	//-----------------------------------------------------------
	void		Send_RpCsExchange( BYTE aIsOpen ); 
	void		Send_RpCsAskExchange( DWORD aPCID ); 
	void		Send_RpCsAnsExchange( BYTE aAnswer ); 
	void		Send_RpCsSuggestExItem( DWORD aEld, BYTE aItemNum, _SExItem aItemList[10] );
	void		Send_RpCsAgreeExchange();
	void		Send_RpCsCancelExchange(); 

	//-----------------------------------------------------------
	// Warehouse Protocols	
	//-----------------------------------------------------------
	void		Send_RpCsCloseBank();	
	void		Send_RpCsDeposit( DWORD aItemID, BYTE aBundle, BYTE aBankSlot );
	void		Send_RpCsDraw( DWORD aBankItemID, BYTE aBundle );
	void		Send_RpCsDepositEld( DWORD aEld );
	void		Send_RpCsDrawEld( DWORD aEld );
	void		Send_RpCsWHChangeSlot( DWORD aBankItemID, BYTE aSlot );
	
	//-----------------------------------------------------------
	// GuildInventory Protocols	
	//-----------------------------------------------------------
#ifdef DECO_RENEWAL_GUILD_MJH
	void        Send_RpCsCloseFrontierBank( TCHAR* aPcName );
	void		Send_RpCsDepositFrontier( DWORD aItemID, BYTE aBundle, BYTE aBankSlot );
	void		Send_RpCsDrawFrontier( DWORD aBankItemID, BYTE aBundle );
	void		Send_RpCsChangeFrontierSlot( DWORD aBankItemID, BYTE aSlot );
#endif // DECO_RENEWAL_GUILD_MJH 	
	
	void		Send_RpCsGMCmd( DWORD aCommand, int* aNParamList, char* aSParam );
	virtual void SetPlayerMessage( TCHAR* str );
	virtual void SetNonPlayerMessage( DWORD pcid, TCHAR* str );


	///-----------------------------------------------------------------------
	///-- Item ���� 
	///-----------------------------------------------------------------------
	void		Send_CsSetStone( DWORD aItemID, DWORD aEldaID, DWORD aPayItem[3] );
	void		Send_CsEldacraftGame( DWORD aMiss100, DWORD aMiss50, DWORD aCombo, DWORD aScore, BYTE aEnd, SEldacraftGameCell *theCellList );
	void		Send_CsMakeItem( SBundleItem atheItem[4], BYTE aB_item_type );


	///-- Repair
	void		Send_CsRequstRepair( SRpCsRequstRepair * aMsg );
	
	///-----------------------------------------------------------------------
	///-- community Protocols 
	///-----------------------------------------------------------------------
	void		Send_CsMsngrMemberInsert( TCHAR * aPCName );
	void		Send_CsMsngrMemberInsertAns( bool bFlag, TCHAR * aPCName );	
	void		Send_CsMsngrMemberDelete( TCHAR * aPCName );
	void		Send_CsMsngrMemberBlock( BYTE bProckType, TCHAR * aPCName );
	void		Send_CsMsngrChangeOpt( SRpCsMsngrChangeOpt * aMsg );
	void		Send_CsMsngrSay( TCHAR * aPCName, TCHAR* aSayMessage );
	void		Send_CsMsngrChatCreateRoom();
	void		Send_CsMsngrChatExitRoom();
	void		Send_CsMsngrChatInviteAsk( TCHAR * aPCName );	
	void		Send_CsMsngrChatInviteAns( bool YesNoFlag, TCHAR * aFromName, TCHAR * aOwnerName);
	void		Send_CsMsngrChatSay( TCHAR* aSayMessage );

	///-----------------------------------------------------------------------
	///-- �米 ���� ����
	///-----------------------------------------------------------------------
	void		Send_CsExpression(DWORD aPCID,BYTE Index);

	//-------------------------------------------------------------------------
	// PVP �ý��� 
	//-------------------------------------------------------------------------
	void		Send_CsReadyFightSystem();
	void		Send_CsSelectFightGameType( BYTE aGameType );
//	void		Send_CsSelectFightGrade( BYTE aGradeType );
	void		Send_CsMakeFightRoom( _SFightRoomInfo aRoomInfo );
	void		Send_CsJoinFightRoom( WORD aRoomID, void* aPassWord, BYTE theWatcher = 0 );
	void		Send_CsOutFightRoom( DWORD aKickID );
	void		Send_CsChangeFightInfo( BYTE aMapCode, BYTE aTimeOut, BYTE aItemFlag );
	void		Send_CsSelectTeam( BYTE aSelectTeam );
	void		Send_CsFightReady( BYTE aFlag );
	void		Send_CsFightStart(); 
	void		Send_CsFightReadyWarp();
	void		Send_CsOutWaitingRoom();

	//.........................................................................
	// ���� ���� 
	//.........................................................................
	void		Send_CsOpenPCShopWindow();				// ���� ���� ��û	
	void		Send_CsOpenPCShop();					// ���� ���� ����
	void		Send_CsClosePCShop();					// ���� ���� �ݱ�
	void		Send_CsInPCShop(DWORD ID);				// ���� ���� ����
	void		Send_CsBuyInPCShop(DWORD ShopPCID,DWORD ItemUID,BYTE Count); // ���� ���� ���

	//------------------------------------------------------------------------------------
	// Commercial Item ���� 
	//------------------------------------------------------------------------------------
	void		Send_CsOpenPayShop();
	void		Send_CsGetPayItem( DWORD aPayItemID );
	void		Send_CsMakeItem( SBundleItem aBundleItem[6], BYTE aB_item_type, DWORD aPayItem[3] );
	

	//------------------------------------------------------------------------------------
	// Frontier ���� 
	//------------------------------------------------------------------------------------
#ifdef DECO_RENEWAL_GUILD_MJH
	void		Send_CsReqMakeFrontier( TCHAR* aFrontierName );	
	void        Send_CsChangeFrontierNotice( TCHAR* aFrontierNotice , DWORD aFrontierID );
	void        Send_CsChangeFrontierMaster( TCHAR* aNewMaster );
	void        Send_CsChangeFrontierMemberIntro( TCHAR* aMemberIntro );
	void		Send_CsMsngrProposeJoinFrontier( DWORD aReceiverID , DWORD aEld , _SExItem aItem );
#else 
	void		Send_CsReqMakeFrontier( TCHAR* aFrontierName, TCHAR* aMemberName0, TCHAR* aMemberName1, TCHAR* aMemberName2 );	
	void		Send_CsMsngrProposeJoinFrontier( DWORD aReceiverID );
#endif // DECO_RENEWAL_GUILD_MJH 
	void		Send_CsMsngrAnswerJoinFrontier( BYTE aAnswer ); 
	void		Send_CsMsngrLeaveFrontier( TCHAR* aPCName );
	void		Send_CsMsngrFrontierMemberInfo( TCHAR* aPCName, TCHAR* aNickName, BYTE aGrade ); 
#ifdef DECO_RENEWAL_GUILD_MJH
	void		Send_CsMsngrUploadEmblem( BYTE aEmblem );
#else
	void		Send_CsMsngrUploadEmblem( WORD aEmblemSize, char* aEmblem );
#endif // DECO_RENEWAL_GUILD_MJH
	void		Send_CsMsngrEmblem( DWORD aFrontierID );
	void		Send_CsPayDues( DWORD aDues );
	void		Send_CsMsngrDivideDues( DWORD aDues ); 
	void		Send_CsMsngrFrontierSay( TCHAR* aStr );
	void		Send_CsCreateFrontierPet( WORD aItemCode, TCHAR* aPetName );
	void		Send_CsMsngrCallFrontierPet();
	void		Send_CsMsngrCancelCallFrontierPet();
	void		Send_CsMsngrAddFrontierMember(TCHAR* aPCName , DWORD aFrontierID);

	//------------------------------------------------------------------------------------
	//	���� Frontier ����
	//------------------------------------------------------------------------------------
	void		Send_CsMsngrReqCombine(DWORD aProposedPCID);
	void		Send_CsMsngrAnsCombine(DWORD aProposerPCID, BYTE aAnswer);
	void		Send_CsMsngrSeperateF();
	
	//------------------------------------------------------------------------------------
	//	���� Corporation ����
	//------------------------------------------------------------------------------------
#ifdef DECO_RENEWAL_CORPORATION_MJH
	void        Send_CsMsngrCorpJoin( BYTE aCorps);                      //���� ����
	void        Send_CsMsngrCorpLeave();                                 //���� Ż��
#endif // DECO_RENEWAL_CORPORATION_MJH
	
	//------------------------------------------------------------------------------------
	// �ް��� ��������ۿ�
	//------------------------------------------------------------------------------------
	void		Send_CsShout(DWORD theItemID, TCHAR *theMessage);
	
	//------------------------------------------------------------------------------------
	// Pet ����
	//------------------------------------------------------------------------------------
	void		Send_CsMakePetName( TCHAR * aPetName , BYTE CancelFlag );
	void		Send_CsMoveItemPCtoPet( DWORD aItemId, BYTE aPetSlot, BYTE aItemPos, BYTE aItemNum );
	void		Send_CsMoveItemPettoPC( DWORD aItemId, BYTE aPetSlot, BYTE aItemPos, BYTE aItemNum );
	void		Send_CsUsePetItem( BYTE aPetSlot, DWORD aItemID, TCHAR * aPetName = NULL );
	void		Send_CsCallPet( BYTE aPetSlot );
	void		Send_CsCancelCallPet();
	void		Send_CsPetStartMove( DWORD aPetID, DWORD aDestiCell );
	void		Send_CsPetStop( DWORD aPetID, DWORD aCell );
	void		Send_CsPetAttack( DWORD aPetID, DWORD aAttackedID, BYTE aAttackedType );
	void		Send_CsPetSkill( DWORD aPetID, DWORD aTarget, BYTE aTargetType, WORD aSkillCode );

	//------------------------------------------------------------------------------------
	// ���̵� �ý���
	//------------------------------------------------------------------------------------
	void		Send_CsSetHorse(DWORD aItemID);
	void		Send_CsRideOn();
	void		Send_CsRideOff();
	void		Send_CsHorseWear(DWORD aItemID);
	void		Send_CsHorseTakeOff(DWORD aItemID, BYTE aItemPos);
	
	//-------------------------------------------------------------------------------------
	//��ȯ, ��� ����
	//------------------------------------------------------------------------------------
	void		Send_CsUseMovePayItem(DWORD	aItemID, TCHAR* aPCName);
	void		Send_CsAnsMove(TCHAR* aPCName, BYTE aAnswer);		
	

	//-------------------------------------------------------------------------------------
	//�Ӹ� ������ 
	//---------------------------------------------------------------------------
	void		Send_CsDenyWhisper(BYTE aFlag);

	//------------------------------------------------------------------------------------
	//	Colony ����
	//------------------------------------------------------------------------------------
	void		Send_CsReqColonyInfo(WORD aColonyCode);
	void		Send_CsDeclareWar(WORD aColonyCode);
	void		Send_CsCancelWar(WORD aColonyCode);
	
	
	void		Send_CsSRpCsMixEldaStone(SRpCsMixEldaStone &TempMsg);
	void		Send_CsMakeSkillPotion(SBundleItem SKillPotion[MAKESKILLPOTIONNUM] , DWORD CreatePotion );


	//------------------------------------------------------------------------------------
	//	�߱� PK ����
	//------------------------------------------------------------------------------------
	void		Send_CsPKMode(BYTE bPKMode);
	
	//------------------------------------------------------------------------------------
	//  ���� ������ ����
	//------------------------------------------------------------------------------------
	void		Send_CsChangeName( DWORD aItemID, char * aNewName );
	
	//------------------------------------------------------------------------------------
	//  ��ű� ���� �ֹ��� ����
	//------------------------------------------------------------------------------------
	void		Send_CsMakeAddAccessory( DWORD aAddAccUID, BYTE aAddType, DWORD aAddScrollUID );
	
	//------------------------------------------------------------------------------------
	//  ���ٽ��� ���� ����
	//------------------------------------------------------------------------------------
	void		Send_CsChangeEldaStone( DWORD aEldaStoneID, DWORD aPayItemID, WORD aSelectedEldaStoneCode );

	///-- Wnd �ּ�ȭ�� ���� Speed Hack Off ��û �� ����
	void		Send_CsOnRestoreOK();
	void		Send_CsOnInactivated();

	BOOL		GetActiveCsStartMoveFlag()		{ return theActiveCsStartMoveFlag; }

	// �ٽ��� 
	void Send_CsHackShield(BYTE Flag , unsigned char *Msg);

	// nProtect 
	void Send_CsGameGuard(DWORD Flag , PBYTE Msg , int size);
				

	//���ٽ��� ���� ��ȯ
	void Send_SRpCsMakeEldastone(int theEldastone ,  SBundleItem  EldastonePowder , SBundleItem   Proima );
	

	//���� ���� ������ 
	//��ȭ �ֹ��� ���
	void Send_SRpCsUseStrengthstone(DWORD theStrengthstoneID , DWORD theStrengthItemID , DWORD thePayItem[3]);
	
	//���� ����  ����
	void Send_SRpCsMakeEssence(int	theEssenceCode  , int theCount);
		
	// ��ȭ �ֹ��� ����
	void Send_SRpCsMakeStrengthstone(int theStrengthstone , SBundleItem	theStrengthstoneMaterial[5] );

		
protected:
	BOOL		CreateAndConnect( CNetConnectSocket *& aSocketPtrRef, char* aIP, int aPort );
	void		CleanUpSocket( CNetConnectSocket *& aSocketPtrRef );

	void		MsgProcLS( CNetConnectSocket * aSocketPtr );
	void		MsgProcGS( CNetConnectSocket * aSocketPtr );

	void		OnLoginSocketClose();
	void		OnGameSocketClose();

	//-----------------------------------------------------------
	// Framework ���� �������ݸ� CNetworkMgr���� �ٷ�� 
	// ���� ���������� ��ӹ��� Ŭ�������� 
	//-----------------------------------------------------------
	virtual	void	Proc_OnSocketClose() {};
	virtual	void	Proc_RpScProtocols( void* aMsg ) {}; /// game protocols procedure
	virtual	void	Proc_RpScErrorFramework( SRpScErrorFramework* aMsg );
	//virtual	void	Proc_RpScWorldList( SRpScWorldList* aMsg );
	//virtual void	Proc_RpScChannelList( SRpScChannelList * aMsg );
	virtual	void	Proc_RpScCharacterList( SRpScCharacterList* aMsg );
	virtual	bool	Proc_RpScPrepareLogin( SRpScPrepareLogin* aMsg );
	virtual	void	Proc_RpScPCLogin( SRpScPCLogin* aMsg );
	virtual	void	Proc_RpScServerSetList( SRpScServerSetList* aMsg );
	virtual	void	Proc_RpScChangeAccount( SRpScChangeAccount* aMsg );
	virtual bool	Proc_RpScHackShield(SRpScHackShield * aMsg);
	virtual bool	Proc_RpScGameGuard(SRpScGameGuard * aMsg);

	//�������� üũ �����Լ� by simwoosung
	virtual void    CheckProtocol(void * aMsg, BOOL IsRecv)  {}

	virtual int GetGameVersion() {return 0;}

	BOOL					theActiveCsStartMoveFlag;		//CsStartMove ���ۿ���
	float					theScStartMoveWatingTime;		//ScWaiting time
	float					theScWatingCumulateTime;		//����Ÿ��

	BOOL					theWaitingSvrMsgFlag;

	char					theLoginServerIP[16];
	int						theLoginServerPort;
	char					theAccountName[65];
	char					theChangeAccountName[65];

	DWORD					thePCID;	
	int						theServerSetNum;		
	SServerSetData			theServerSetData[64];	/// �ϴ� �ִ� 64����� ���� 
	int						theAreaInfoNum;
	//int						theWorldNum;		
	//int						theChannelNum;	
	//SWorldData			theWorldData[64];	///-- ������ �ϴ� �ִ� 64����� ���� 
	//SChannelData		theChannelData[64];	///-- ä��  64�� �ִ�
	int						theCharacterNum;	

	SRpdCharacterDataWithCore	theCharacterList[5];

	int						    theSelectCharIndex;

	/// sooree net remodel 
	CNetPackMgr				theNetPack2Login;
	CNetPackMgr				theNetPack2Game;
	BYTE					theNetPackBuf[4096];	
	int						NetToLogin( void* aMsgPtr, int aSize );
	int						NetToGame( void* aMsgPtr, int aSize );

	NConnectState			theConnectState;
	CNetConnectSocket*		theLoginSocketPtr;
	CNetConnectSocket*		theGameSocketPtr;
	
	TCHAR					theWhisperMessage[MAX_SIZE_256];
	
};

#endif
