//_________________________________________________________________________________
//
//	protocol.H
//
//	�������ݵ� ����
//
//
//_________________________________________________________________________________
//
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_


///------------------------------------------------------------------------------
///	sooree network : GNI ��Ʈ�� �ҽ� ���� - �� �κ��� ������ ���� �ϴ� �׳� �ΰ� ���ϴ�. 
///------------------------------------------------------------------------------
/*
#define SM_MOVE_BROADCAST		257		// �̵� �޼��� ��� (S->C)
#define SM_ATTACK_NPC			294		// ����� npc���� ���
#define SM_NPC_RESPAWN			297			// 10-15
#define SM_NPC_DIE				298
#define SM_USER_DIE				312		// ���� ����
#define SM_ARROW_GOING			341		// ȭ�� ���󰡴°�
#define SM_ATTACK_PC			345		// ���� ����
#define SM_NPC_DISAPPEAR		375		// ������ ������ ���� �ִ� NPC �����
#define SM_USE_SKILL			377		// ��� ����
#define SM_SLIDE_NPC			383		// stun �� ��� ������ �з�����

#define SM_CHANT_MODE				432		// ��â �޺� ��� 
#define CM_CHANT_MODE				432		// ��â �޺� ��� 

#define SM_ATTACK_DELAY				433		// ���� ��� ��� 
#define CM_ATTACK_DELAY				433		// ���� ��� ��� 
*/
typedef enum NEventType
{
	SM_MOVE_BROADCAST = 100,		// �̵� �޼��� ��� (S->C)
	SM_MOVE_ATTACK,					// �̵� ���� 
	CM_MOVE_ATTACK,					// ������ ����ϱ� ���� �̵� ����
	SM_ATTACK_NPC,					// ���� ��� 
	SM_ATTACK_DELAY,				// ���� ��� ��� 
	SM_USE_SKILL,					// ��� ����
	SM_CHANT_READY,					// ��â �غ� 
	SM_CHANT_SPELLING,				// ��â �޺� �ܰ� 
	SM_SIMSIM,						// �ɽ��̺�Ʈ(�λ�,����ൿ)
	SM_UNDERATTACK,					// ���ݴ�����
	SM_SLIDE_NPC,					// stun �� ��� ������ �з�����
	SM_NPC_RESPAWN,					// 10-15
	SM_STATE_DIE,					// ���� ����
	SM_RESTSKILL_SITDOWN,			// �޽� ��ų (�ɱ�)
	SM_RESTSKILL_KEEPING,			// �޽� ��ų (����)
	SM_RESTSKILL_STANDUP,			// �޽� ��ų (����)
	SM_ITEM_DROPING,				// ������ ����߸��� 
	SM_ARROW_GOING,					// ȭ�� ���󰡴°�
	SM_BOW_ATTACK,					// ȭ�� ����
	SM_BOW_READY,					// ȭ�� �غ� ����
	SM_BOW_WAIT,					// ȭ�� ��� ����
	SM_SELF_DIE,					// �ڻ�
	SM_MOVE_TURN,					// ���ڸ� ����								// ī�޶� ��ũ���� By wxywxy
	SM_ANI_QUEUE_PROCESS,			// ���ϸ��̼� ���� ó���ϱ�					// ī�޶� ��ũ���� By wxywxy
	SM_ACT,							// �ൿ 
	SM_ACT_START,					// �ܹ� �ൿ
	SM_ACT_KEEPING,					// ���� �ൿ
	//.......................................................................................................
	// ���� ���� By wxywxy
	//.......................................................................................................
	SM_SELLSKILL_SITDOWN,			// ���� �ȱ� ���� �ɱ�  status
	SM_SELLSKILL_KEEPING,			// ���� �ȱ� ���� ����  status
	SM_SELLSKILL_STANDUP,			// ���� �ȱ� ���� ����  status
	//.......................................................................................................
	SM_ANI_SQUENCE,					// ������ ���ϸ��̼� ( ���� �Ϸ��� ���� �������� �̵� ) By wxywxy
	//.......................................................................................................
	//	���̵� By simwoosung	
	//.......................................................................................................
	SM_RIDING,

	///-- By simwoosung �ϴ� �̺κ��� �ʿ伺�� ������ ����... ���� ���δ� �̱�����
	SM_MOVE_SKILL,					// �̵� �� ��ų
	CM_MOVE_SKILL,					// ������ ����ϱ� ���� �̵� ��ų 	
	
	SM_EVENT_MAXCOUNT
};

#define SIMSIM_TIME			30000.0f			// �ɽ��ؼ� �ϴ� �ൿ(5��)


#define MAX_EQUIPED		10
#define MAX_INVENTORY1	145

struct Char_Info		// ���̴� ������	
{
	/// sooree network
	///--------------------------------------------------------------------------
	///	�츮 �ſ� ���� ���ο� ������ ����... �����δ� �̰ɷ� �ڵ带 �籸�� 
	///	struct _SRpdOtherPC���� Cell ��ǥ�� �� ���� 
	///-----------------------------------------------------------------------
	DWORD				thePCID;
	char				thePCName[RP_MAX_PC_NAME];
	BYTE				theLevel;			
	BYTE				theCitizen;									// RAIN, MILLENA
	BYTE				theGender;									// MALE, FEMALE
	DWORD				theFace;
	DWORD				theHair;
	DWORD				theHP;
	DWORD				theSMP;										// Citizen�� ���� SP �Ǵ� MP�� ó�� 
	DWORD				theMaxHP;									// �⺻ �� + ���� ������ + Item + Skill
	DWORD				theMaxSMP;
	DWORD				theRealHP;
	BYTE				theWasteGage;
	SRpdCharWearItem	theWearItem[RP_MAX_EQUIP];					// ��ű�1, 2 ����
	WORD				theKeepupSkill[RP_MAX_KEEPUP_SKILL];		// ���Ӽ� ��ų �ڵ� 		
	BYTE				theMainClass;								// ���� Ŭ����	

	
	int					theFame;									// ĳ������ ��...
	
	///--				�߱� ���� PK�۾�
	DWORD				thePKPoint;

	///--------------------------------------------------------
	///	�̰� ���� ���̴� �����͵�...�ϴ� �������÷� �� ���д�. 
	///--------------------------------------------------------
	char	ID[12];			// id

	int		race;			// ����		0 : ���Ͼ�, 1 : �ٸ�1, 2 : �ٸ�2
	int		sex;			// ����		0 : ��, 1 : ��
	int		job;			// ����		0 : ������, 1 : ����, 2 : �ü�, 3 : ������, 4 : ��....
	BYTE	hair;			// �Ӹ����
	BYTE	color;			// �Ӹ�����
	BYTE	face;			// �󱼸�� 

	DWORD		head;
	DWORD		body;
	DWORD		r_hand;
	DWORD		l_hand;
	DWORD		arm;
	DWORD		foot;

	int		effect[5];			// �ɷ� �ִ� ȿ��
	
	//by simwoosung  ��Ƽ����
	char				 thePartyName[512];
	DWORD                thePartyID;

	DWORD				 theFrontierID;
	char				 theFrontierNick[RP_MAX_FRONTIER_NICK];
	char				 theFrontierName[RP_MAX_FRONTIER_NAME];
	BYTE		         theFrontierLevel;
	BYTE	             theCorps;
	DWORD		         theFrontierFame;
	
	BYTE				 theHolyLevel;	 				

	BYTE				 theStatus;
	
	Char_Info()
	{
		thePartyID = 0;
		ZeroMemory(thePartyName , MAX_SIZE_512);
		theHolyLevel = 0;
		theFrontierID = 0;
		ZeroMemory(theFrontierNick, RP_MAX_FRONTIER_NICK);
		ZeroMemory(theFrontierName, RP_MAX_FRONTIER_NAME);

		thePKPoint = 0;
	}
};


#define BYTE_HIGH4BIT	240
#define BYTE_LOW4BIT	15
#define ITEM_KEY_COUNT  3						//���� (������� �����)
#define ITEM_KEY_SEX    3						//����
#define ITEM_KEY_RACE   2						//����
#define ITEM_KEY_GRAV	2						//�߿䵵
#define ITEM_KEY_CLASS	1						//�������� (Į,���е�)
#define ITEM_KEY_TYPE	1						//�������� (��Į,ª��Į��)
#define ITEM_KEY_NUMBER	0						//������ ��ȣ 
#define ITEM_KEY_COUNT_BIT	 BYTE_HIGH4BIT
#define ITEM_KEY_SEX_BIT	 BYTE_LOW4BIT
#define ITEM_KEY_RACE_BIT	 BYTE_HIGH4BIT
#define ITEM_KEY_GRAV_BIT	 BYTE_LOW4BIT
#define ITEM_KEY_CLASS_BIT	 BYTE_HIGH4BIT
#define ITEM_KEY_TYPE_BIT	 BYTE_LOW4BIT

union _uItemType
{
	DWORD m_dwItemType;			//item id
	BYTE  m_byItemType[4];
};

///------------------------------------------------------------------------------
///	���� ���� GNI ��Ʈ�� ó�� ���� Define, Structure ��. 
///------------------------------------------------------------------------------

/*

const UINT c_Max_Equiped( 10 );

typedef enum NEquipType
{
	n_Equip_Helmet = 0,
	n_Equip_Coat_Armor,
	n_Equip_Pants_Armor,
	n_Equip_Gloves,
	n_Equip_Shoes,
	n_Equip_Rhand,
	n_Equip_Lhand,
	n_Equip_Serve,
	n_Equip_Accessorily1,
	n_Equip_Accessorily2
//n_Equip_Necklace
//n_Equip_Bracelet
//n_Equip_Ring
};




//

//_________________________________________________________________________________
//
// define message header number
//_________________________________________________________________________________
//
// CM_��¼���� Ŭ���̾�Ʈ���� ������ ���� �޼��� ���
// SM_��¼���� �������� Ŭ���̾�Ʈ�� ���� �޼��� ���

//0~ 128   reserved for server comm
//128~159  reserved for login server
//160~181  reserved for AccountDB Middleware
//182~255  reserved for GameDBMiddleware
//256~512  for game play
//msg header for server com----------------------------------------------------------
#define SSM_SERVER_READY_DONTUSE	1			//IOCPnet reserved .. do not use 
#define SSM_SERVER_READY_DONTUSE2	2			//IOCPnet reserved .. do not use 
#define SSM_SERVER_READY_DONTUSE3	3			//IOCPnet reserved .. do not use 
#define SSM_USERINCOMING_STANDBY	10
#define SSM_USERINCOMING_READY		11

#define SSM_PRIVATE_CHAT_REQUEST	12			// ������ �Ӹ� ��Ŷ
#define SSM_PRIVATE_CHAT_REPLY		13
#define SSM_SHOUT_CHAT				14

#define SSM_WARP_REQUEST			15			// warp ��û
#define SSM_WARP_REPLY				16			// warp ����

#define SSM_GAMEINFO_LOGOUT				17
#define SSM_GAMEINFO_SAVE				18
#define SSM_GAMEINFO_ERROR				19
#define SSM_GAMEINFO_LOGIN				20
#define SSM_GAMEINFO_PRODUCE_ITEM		 21
#define SSM_GAMEINFO_PRODUCE_ITEM_RESULT 22
#define SSM_GAMEINFO_MAKE_CHARACTER      23
#define SSM_GAMEINFO_DELETE_CHARACTER    24

//account info server interface
#define SSM_ACCOUNTINFO_CHECK		32
#define SSM_ACCOUNTINFO_ERROR		33
#define SSM_ACCOUNTINFO_LOGOUT		34


//account db middleware interface
#define SSM_ACCOUNTDB_CHECK				64
#define SSM_ACCOUNTDB_CHECK_RESULT		65
#define SSM_ACCOUNTDB_SETTLE_REQUEST	66
#define SSM_ACCOUNTDB_SETTLE_RESULT		67


//game db middleware interface
#define SSM_GAMEDB_CHARACTER_INFO			96		
#define SSM_GAMEDB_CHARACTER_INFOLIST		97
#define SSM_GAMEDB_CHARACTER_SAVE			98
#define SSM_GAMEDB_CHARACTER_SINGLEINFO		99
#define SSM_GAMEDB_ERROR					100
#define SSM_GAMEDB_PRODUCE_ITEM				101
#define SSM_GAMEDB_CHARACTER_MAKE			102
#define SSM_GAMEDB_CHARACTER_DELETE			103


//msg header for login server--------------------------------------------------------
//from client for login interface
#define LCM_ACCOUNT_CHECK			128
#define LCM_GROUPSERV_SELECT		129
#define LCM_CHRACTER_SELECT			130
#define LCM_LOGIN_CHARACTER_MAKE    131
#define LCM_LOGIN_CHARACTER_DELETE  132  // Ŭ���� ����ٰ� ������ 
#define LCM_LOGIN_LIVE_CHECK		136 // Ŭ ��Ҵٰ� ����
//to client for login interface
#define LSM_LOGIN_ERROR 			128
#define LSM_GROUPSERV_INFO			129
#define LSM_CHARACTER_INFO			130
#define LSM_LOGIN_OK				131
#define LSM_LOGIN_CHARACTER_DELETE  133	// �������� ���������� ������
#define LSM_LOGIN_VER_ERROR			135	// ������ �޶� ������ �ȵȴ�.
#define LSM_LOGIN_LIVE_CHECK		136 // Ŭ ��� �ִ��� Ȯ��
#define LSM_CHARACTER_STORAGE		137	// â��
#define LSM_HORSE_LOAD				138 // �������� Ŭ���� 
#define MAX_HORSE_PER				11	// max horse per


//msg header for game play --------------------------------------------------------
#define CM_MOVE_BROADCAST			256		// �̵� �޼��� ��� (C->S)
#define SM_MOVE_BROADCAST			257		// �̵� �޼��� ��� (S->C)

#define SM_CONFIRM_POINT			258		// ��� ����Ʈ ����..������ ���� ����Ʈ�� ��� ���ݰ� ����..

// 257-259 ����

#define SM_APPEAR	  260
#define CM_DISCONNECT 261		// CM_DISAPPEAR 3 ??? ��¡~
#define SM_DISAPPEAR  262		// Ŭ���̾�Ʈ���� disconnect�� �ް� ���������� disappear�� ������...

#define CM_LOGINZONE  263
#define SM_LOGINZONE	264

#define SM_CHECK_CONNECTION		265		// ���� Ȯ�� �޼���...6-26�� �߰�..

#define CM_NORMAL_CHAT	266		// �Ϲ� ä��
#define SM_NORMAL_CHAT	266
#define CM_PRIVATE_CHAT	267		// �Ӹ�
#define SM_PRIVATE_CHAT	267
#define CM_SHOUT_CHAT	268		// ��ü ��ġ��
#define SM_SHOUT_CHAT	268
#define CM_PARTY_CHAT	269		// ��Ƽ ��ȭ
#define SM_PARTY_CHAT	269
#define CM_GUILD_CHAT	270		// ��� ��ȭ
#define SM_GUILD_CHAT	270
#define CM_TRADE		271		// ������ �ŷ�
#define SM_TRADE		271

#define CM_REQUIRE_INDEX	272		// ���Ȯ��
#define SM_REQUIRE_INDEX	272

#define SM_NPC_USE_MAGIC	275	// 10-18 npc�������

#define	SM_NPC_APPEAR	276		// 8-8
#define SM_NPC_MOVE		277

#define SM_WARP			278		// 8-23..
#define CM_WARP			279

#define CM_TAKEOFF_ITEM	280		// �����Ѱ� ����
#define SM_TAKEOFF_ITEM 281

#define CM_CHANGE_ITEM	282		// �����Ѱ� �ٲٱ� �Ǵ� �����ϱ�
#define SM_CHANGE_ITEM	283

#define CM_DROP_ITEM	284		// ������ ������
#define SM_DROP_ITEM	285
#define SM_ITEM_APPEAR	286		// �̵��̳� �������� ������ ���̱�

#define CM_PICKUP_ITEM	287		// ������ �ݱ�
#define SM_PICKUP_ITEM	288
#define SM_ITEM_DISAPPEAR	289	// ������ �����..����...

#define CM_BUY			290			// ����Ŭ����
#define SM_BUY_OK		291			// ��� ����
#define SM_BUY_ERROR	292			// ��� ����...�̹��� �κ��� ��ĭ�� �������

#define CM_ATTACK_NPC		293		// ����� npc����
#define SM_ATTACK_NPC		294		// ����� npc���� ���

#define CM_WEATHER		295
#define SM_WEATHER		296

#define SM_NPC_RESPAWN	297			// 10-15

#define SM_NPC_DIE		298

#define CM_SELL			299
#define SM_SELL_OK		300
#define SM_SELL_ERROR	301

#define SM_NPC_ATTACK_PC	302		// npc�� ���� ����
#define CM_WARP_READY		303

#define SM_TIME_CHANGE		304		// �ð� ��ȭ

#define SM_ITEM_CHANGE		305
#define SM_ITEM_MOVE		306
#define SM_ITEM_EXTINCTION	307		// �ð� �Ҹ� �Ǵ� ������

#define CM_ATTACK_ITEM		308		// ������ ����
#define SM_ATTACK_ITEM		309		// ������ ���

#define	SM_NPC_SPEECH		310

#define	SM_USER_RECOVER		311		// ���� ȸ��............�̳ѵ� ���� hp,mp�� �ٲ�
#define SM_USER_DIE			312		// ���� ����
#define SM_LEVEL_UP			313		// ������

#define CM_CHANGE_STAT		314		// ���� ���� ��ȭ
#define SM_CHANGE_STAT		315

#define	SM_CHANGE_EXP		316		// ����ġ ����..�� �׾������� -����...�̳ѵ� �ٲ�...���� ����ġ��

#define SM_CONFIRM_CASH		317		// ���� �ϰ� ���� ���̾�
#define SM_CONFIRM_STAT		318

// 2-19 ����
#define	CM_USE_POTION		319
#define SM_USE_POTION		320

// 3-4�ı�
#define SM_DESTROY_ITEM		321

// 3-8 ��ü
#define CM_TRY_UNITE		322
#define SM_TRY_UNITE		323
#define CM_REPLY_UNITE		324
#define SM_REPLY_UNITE		325
#define CM_SEPERATE_UNIT	326
#define SM_SEPERATE_UNIT	327

// 3-14
#define CM_TOGGLE_RUN		328
#define SM_TOGGLE_RUN		329
#define CM_TOGGLE_ATTACKMODE	330
#define SM_TOGGLE_ATTACKMODE	331

// 3-19.....10-13����
#define CM_PARTY			332
#define SM_PARTY			332
#define SM_MEMBER_INFO		333		// ��Ƽ�� ����..���߿� ����� ���� ��� �޼���������...
#define SM_MEMBER_RECOVER	334

// 12-3 ���� ��ȭ
#define SM_STATUS_CHANGE	335


// 3-21
#define SM_ARROW_GOING		341		// ȭ�� ���󰡴°�
#define SM_MAGIC_GOING		342		// ���� ���󰡴°�

// 3-24
#define CM_ATTACK_PC		344		// ���� ����
#define SM_ATTACK_PC		345		// ���� ����

// �̰� �� ������..
#define CM_REQUEST_PK		346		// ���� �ϰڿ�
#define SM_REQUEST_PK		347		// ��û�̾�
#define	CM_REPLY_PK			348		// ��û�������� �ϴ� �����̾�
#define SM_REPLY_PK			349		// ��û�ѳ����� ������ ����

// 3-27	032
#define CM_HORSE_DOWN		350		// ������ �����ٰ� �� ��û
#define SM_HORSE_DOWN		351		// ������ �����ٰ� �� ����
#define CM_HORSE_RIDE		354		// ���� ź�ٰ� �� ��û
#define SM_HORSE_RIDE		355		// ���� ź�ٰ� �� ����




// 4-29
#define CM_CHANGE_CLASS		356		// �����ҷ�
#define SM_CHANGE_CLASS		357		// �׷�..
// 5-1
#define SM_NPC_DAMAGE		358		// npc�� ���� �ްų� ���� ��ȭ�϶�

// 4-2
#define SM_REQUEST_REVIVE	360		// �� ��Ȱ�ҷ�?
#define CM_REPLY_REVIVE		361		// ����� ���
#define	SM_USER_REVIVE		362		// ���� ��Ȱ�ߴ�..

//363 364 empty

#define CM_HORSE_ATTR	    365     // ���� �Ӽ��� �ٲ۴ٰ� ��û 
#define SM_HORSE_ATTR		366		// ���� �Ӽ��� �ٲ۴ٰ� ���� 

#define CM_HORSE_KEEPIN		367		// ���� ���°��� �ñ��. ��û 
#define SM_HORSE_KEEPIN		368		// ���� ���°��� �ñ��. ����

#define CM_HORSE_KEEPOUT	369		// ���� ���°��� ���� . ��û 
#define SM_HORSE_KEEPOUT	370		// ���� ���°��� ���� . ����

// 4-8
#define	CM_TAMING_HORSE		371		// �߻����� ���ðڴ�
#define SM_TAMING_HORSE		372		// �߻��� ���ñ�õ� ���
#define CM_TREAT_CARROT		373		// �߻������� ��� ���̱�
#define SM_TREAT_CARROT		374		// ��� ���� ���..���̸� �� ���̰�..

// 4 - 22
#define SM_NPC_DISAPPEAR	375		// ������ ������ ���� �ִ� NPC �����

#define CM_USE_SKILL		376		// ��ų ���
#define SM_USE_SKILL		377		// ��� ����

#define SM_LEARN_SKILL		378		// ��ų ���

#define CM_INVEN_CHANGE		379		// �������� �ι� ��ġ���� �ٸ� ��ġ�� �ٲ� ��û 
#define SM_INVEN_CHANGE		380		// �������� �ι� ��ġ���� �ٸ� ��ġ�� �ٲ� ���� 

// 6-9
#define CM_ADMIN_CHAT		381		// Ŭ���̾�Ʈ���� ��ڰ� ������ ä�� �޼���
#define SM_ADMIN_CHAT		382		// ��� �޼���..Ŭ���̾�Ʈ������ ȭ�� ��� �ѷ���

// 6-10
#define SM_SLIDE_NPC		383		// stun �� ��� ������ �з�����
#define SM_SLIDE_PC			384		// ���߿� ����� �з������� �����ϱ�

// 6-11
#define SM_CONFIRM_POTION	385		// ���� ���� Ȯ��

// 6-12
#define CM_NOW_TIME			386		// Ŭ���̾�Ʈ���� 1�п� �ѹ��� ��� ��Ŷ..1�и��� �ð����
#define CM_KILL_CLIENT		387		// ��ڰ� ��°�..�ش� ���� ���� �����
#define SM_KILL_CLIENT		388		// �̰� ������ �޴� Ŭ���̾�Ʈ ���� �����

// 6-14
#define CM_CHECK_CONNECTION		389	// ���� Ȯ�� �޼���...6-26�� �߰�..

// 6-15
#define SM_NO_EXIST			390
#define CM_REQUIRE_DATA		391		// ����

// 7-14 
#define CM_CHR_DIR			392		// �Ӹ� ���ư��� ��� ��û
#define SM_CHR_DIR			393		// �Ӹ� ���ư��� ��� 

// 7- 16
#define CM_NPC_ENERGY			394		// NPC�� HP% ��û 
#define SM_NPC_ENERGY			395		// NPC�� HP% ���� 

// ��Ŷ�� �Ƴ������� ���� #define ó��
#define CM_NPC_STATUS			396		// npc�� ���� ��û
#define SM_NPC_STATUS			396		// npc�� ���� ����

//
#define CM_ANIMA_STATUS			397		// �ƴϸ� ������ ��û(�ʿ� ����)
#define SM_ANIMA_STATUS			397		// �ƴϸ� �������� ���� 

#define CM_CHARACTER_STATUS			398		//   ���� 
#define SM_CHARACTER_STATUS			398		//   ����

#define CM_GET_QUEST				399		//	 ����Ʈ ��û
#define SM_GET_QUEST				399		//   ����Ʈ ���� 

#define CM_PUT_PIECE				400		// ���� ���� �����
#define SM_PUT_PIECE				400

#define CM_SUCC_QUEST				401		//	 ����Ʈ ����
#define SM_SUCC_QUEST				401		//   ����Ʈ ���� 

#define CM_COMPLETE_PUZZLE			402		// ���� �Ϸ� ����������
#define SM_COMPLETE_PUZZLE			402

#define SM_USE_NECTA_TAR			403

#define CM_QUESTITEM_DEL			404      // ������ ���� 
#define SM_QUESTITEM_DEL			404

#define CM_QUEST_CHECK				405
#define SM_QUEST_CHECK				405 

#define SM_CHANGE_GAGE				406
#define CM_MODE_CHANGE				407
#define SM_MODE_CHANGE				407

#define CM_REGISTER_BOARD			408		// �κ����� å ��Ŭ���� ��°�
#define SM_REGISTER_BOARD			408		// ��� ���� ����

#define SM_ITEM_DELETE				409		// ���ﲨ

#define SM_BUFF_CHANGE				410		// ����

#define CM_STORAGE_CHANGE			413		// â���κ�����
#define SM_STORAGE_CHANGE			413

#define SM_ERROR_CODE				415		// ��ӵ� ���� �ڵ�� ���� ���� �޼���
#define SM_ERROR_MSG				416		// ���ڿ��� ��� ���� �޼���

#define SM_CONFIRM_ARROW			417     // 9-23 �� ȭ���� ���� ������.

#define	CM_OPERATOR_MODE			418		// 9-23 ��ڸ��
#define	SM_OPERATOR_MODE			418		// 9-23 ��ڸ��

#define SM_CHARACTER_INFO			419		// ĳ���� ����

#define CM_OPERATOR_CHAT			420		// ��ڿ���
#define SM_OPERATOR_CHAT			420		// ��ڿ���

#define CM_SKILL_UPGRADE			421		// ��ų ���׷��̵�
#define SM_SKILL_UPGRADE			421

#define SM_DELETE_EQUIP				422		// ��������� ������ 

#define CM_TRY_ITEMDEAL				423		// item �ְ� �ޱ� ��û
#define SM_TRY_ITEMDEAL				423
#define CM_REPLY_ITEMDEAL			424		// item �ְ� �ޱ� ����.	
#define	SM_REPLY_ITEMDEAL			424

#define CM_SAVE_WARPZONE			425		// 
#define SM_SAVE_WARPZONE			425

#define CM_ITEM_UPGRADE				426		// ������ ���׷��̵� ��Ŷ
#define SM_ITEM_UPGRADE				426		// ������ ���׷��̵� ��Ŷ

#define CM_CONNECTED				510
#define CM_DISCONNECTED				511

#define CM_TELEPORTER				273		// �ڷ���Ʈ npc������ �� �ڷ���Ʈ
#define SM_TELEPORTER				273

#define CM_HORSE_NAME				427
#define SM_HORSE_NAME				427

#define CM_HORSE_STATUS				428
#define SM_HORSE_STATUS				428

#define CM_HORSE_AWAY				429
#define SM_HORSE_AWAY				429

#define CM_NORMAL_LOGOUT			430		// ��������

#define CM_USER_INFO				431 // ��������



#define SM_CHANT_MODE				432		// ��â �޺� ��� 
#define CM_CHANT_MODE				432		// ��â �޺� ��� 

#define SM_ATTACK_DELAY				433		// ���� ��� ��� 
#define CM_ATTACK_DELAY				433		// ���� ��� ��� 




//defines for general
#define IPV6_LENGTH					24
#define ACCOUNT_LENGTH				12		//account name length
#define CHARACTER_LENGTH			12		//game character name length
#define CHARACTER_MAX_PER_GROUPSERV 3		//max characters per group server for one user
#define CHARACTER_MAX_EFFECT		5
//_________________________________________________________________________________
//
// define game structures
//_________________________________________________________________________________
//
#define KEY_MAP 10000000

#define	REMAIN_POINT_KEY	8
#define	LEVEL_KEY			255

// db���� ������ UINT ���� �ѹ���Ʈ�� ���ܳ���.. ����[2] ����[1] ����[0] ��° ����Ʈ�̴�
#define CHAR_SHIFT_KEY_HAIR	24		// �Ӹ���� �Ӹ���
#define CHAR_SHIFT_KEY_RACE	16
#define CHAR_SHIFT_KEY_SEX	8
#define CHAR_SHIFT_KEY_JOB	0
#define CHAR_KEY		255
#define CHAR_KEY_HAIR	15
#define CHAR_KEY_COLOR	240

struct Char_Info		// ���̴� ������
{
	char	ID[CHARACTER_LENGTH];		// id

	int		race;			// ����		0 : ���Ͼ�, 1 : �ٸ�1, 2 : �ٸ�2
	int		sex;			// ����		0 : ��, 1 : ��
	int		job;			// ����		0 : ������, 1 : ����, 2 : �ü�, 3 : ������, 4 : ��....
	BYTE	hair;			// �Ӹ����
	BYTE	color;			// �Ӹ�����
	BYTE	face;			// �󱼸�� 

	DWORD		head;
	DWORD		body;
	DWORD		r_hand;
	DWORD		l_hand;
	DWORD		arm;
	DWORD		foot;

	int		effect[CHARACTER_MAX_EFFECT];			// �ɷ� �ִ� ȿ��
};
struct Char_Info_All: public Char_Info		// �Ⱥ��̴� ��������� ������ ...
{
	DWORD		accessory1;
	DWORD		accessory2;
};

#define	HORSE_BODY_COLOR	1
#define	HORSE_BODY_COVER	2
#define HORSE_SADDLE_COLOR	3
union	_uHorseInfo
{
	DWORD		m_dwHorseInfo;
	BYTE		m_byHorseInfo[4];		// 0:�̻��, 1:������, 2:��ȣ��, 3:����
};

#define BYTE_HIGH4BIT	240
#define BYTE_LOW4BIT	15
#define ITEM_KEY_COUNT  3						//���� (������� �����)
#define ITEM_KEY_SEX    3						//����
#define ITEM_KEY_RACE   2						//����
#define ITEM_KEY_GRAV	2						//�߿䵵
#define ITEM_KEY_CLASS	1						//�������� (Į,���е�)
#define ITEM_KEY_TYPE	1						//�������� (��Į,ª��Į��)
#define ITEM_KEY_NUMBER	0						//������ ��ȣ 
#define ITEM_KEY_COUNT_BIT	 BYTE_HIGH4BIT
#define ITEM_KEY_SEX_BIT	 BYTE_LOW4BIT
#define ITEM_KEY_RACE_BIT	 BYTE_HIGH4BIT
#define ITEM_KEY_GRAV_BIT	 BYTE_LOW4BIT
#define ITEM_KEY_CLASS_BIT	 BYTE_HIGH4BIT
#define ITEM_KEY_TYPE_BIT	 BYTE_LOW4BIT



union _uItemType{
	DWORD m_dwItemType;			//item id
	BYTE  m_byItemType[4];
};

//item info
#define ITEM_KEY_LIFE		0	
#define ITEM_KEY_UPGRADE	1	
#define ITEM_KEY_COLOR		2	
#define ITEM_KEY_LOC		3
union _uItemInfo{
	DWORD  m_dwItemInfo;
	BYTE   m_byItemInfo[4];
};

struct Item_Info_Save
{
	DWORD	m_dwItemUnique;
	_uItemType m_uItemType;
	_uItemInfo m_uItemInfo;
//	_uItemInfo m_uItemInfo[2];
};



enum ItemEffect
{
	PACHANGE = 0,		// physical attack 
	PDCHANGE,			// physical defense
	MACHANGE,			// magical attack
	MDCHANGE,			// magical defense
	SUCCHANGE,			// ������ 
	EVDCHANGE,			// ȸ����
	PALCHANGE,			// physical attack level change
	PDLCHANGE,			// physical defense level change
	MALCHANGE,			// magical attack level change
	MDLCHANGE,			// magical defense level change
	SUCLCHANGE,			// ������ ������ 
	EVDLCHANGE,			// ȸ���� ������

	HPCHANGE,			// ü�°���
	MPCHANGE,			// ���°���

	HPLCHANGE,			// ü�� ������ ����
	MPLCHANGE,			// ���� ������ ����

	STRCHANGE,			// ��
	DEXCHANGE,			// ��
	CONCHANGE,			// ��
	WISCHANGE,			// ��

	NoneEffect = 255
};

union Effect_Value
{
	short	m_effectvalue;
	BYTE	m_byeffectvalue[2];
};

struct Item_Effect
{
	ItemEffect		m_effect_id;		// � ȿ��...255�� ȿ�� �����̴�
	Effect_Value	m_effect;			// ȿ�� ��ġ [�ּ�][�ִ�]
};

// ������ ����ü
struct Item_Index
{
	_uItemType		m_item_id;
	//Item_Effect		m_item_effect[3];	// �ɷ�ġ...
	//int				m_item_delay;		// ������
	int				m_item_range;		// �����Ÿ�
	int				m_item_price;		// �⺻ ����
	//int				m_item_maxup;		// �ִ� ��ȭ ��ġ
	int				m_item_level;		// ���� ���� ����
	//int				m_item_str;			// �������� ��...255�� ���� ����
	//int				m_item_dex;
	//int				m_item_con;
	//int				m_item_wis;
	//BYTE			m_item_set;			// ��Ʈ��ȣ
	//BYTE			m_item_special;		// Ư�� ȿ��

	// 5 7 032 item �߰�
	int				m_UseJob;			// ��밡������
	int				m_attr;				// �Ӽ�
	int				m_live;				// ���� 
	int				m_Attack_Ph;		// �������ݷ�
	int				m_Defense_Ph;		// �������� 
	int				m_Evade_Per;		// ȸ����
	int				m_Attack_speed;		// ���ݽ��ǵ�
	int				m_Attack_Magic;		// �������ݷ�
	int				m_Defense_Magic;	// ��������

										// �����Ÿ� 
										// ���Ѵɷ� ���� 

	int				m_Limit_Capability; // ���Ѵɷ� 
	int				m_Limit_Capa_Value; // ���Ѵɷ�ġ
	int				m_Recover;			// ȸ��
	int				m_Recover_Value;	// ȸ����
	int				m_Attack_Ph_Up;		// �������ݷ� ���
	int				m_Defense_Ph_Up;	// �������� ���
	int				m_Attack_Magic_Up;	// �������ݷ� ���
	int				m_Defense_Magic_Up; // �������� ���
	int				m_Attack_Speed_Up;  // ���ݽ��ǵ� ���
	int				m_Evade_Per_Up;		// ȸ���� ���
	int				m_SetItem_No;		// Set������ ��ȣ
	int				m_PuzzleItem_No;	// ��������� ��ȣ
	int				m_bChange;			// �������� 
	int				m_Change_Per;		// ����Ȯ��
	int				m_Item_Maxup;		// ��ȭ�ܰ�
	int				m_bSkill;			// ��ų����
	int				m_Skill_Kind;		// ��ų����
	int				m_Serise_Attack;	// ���Ӱ����Ǵ�
	int				m_Size;				// ũ��

										// ���� 	
};

// â��
#define MAX_STORAGE_TAB				4
#define STORAGE_X					8
#define STORAGE_Y					12
#define MAX_STORE_PER_TAB  ( STORAGE_X * STORAGE_Y + 1)

struct Char_info_Tab_Storage
{
	Item_Info_Save m_Storage[MAX_STORE_PER_TAB];
};

//������ Ŭ���� 

class CHorseData
{
public:
	int m_iPrimary;
	int m_iAbility;
	int m_iExperience;
};

#define MAX_EQUIPED		10
#define MAX_INVENTORY1	145
//#define MAX_INVENTORY2  18

#define EQUIP_HELMET		0
#define EQUIP_ARMOR			1
#define EQUIP_RHAND			2
#define EQUIP_LHAND			3
#define EQUIP_GLOVES		4
#define EQUIP_SHOES			5
#define EQUIP_NECKLACE		6
#define EQUIP_BRACELET		7
#define EQUIP_RING1			8
#define EQUIP_RING2			9

#define INVEN_TABSTART1		0
#define INVEN_TABSTART2		18


#define VALUE_WORD1		0
#define VALUE_WORD2		1

#define VALUE_BYTE1 0
#define VALUE_BYTE2	1
#define VALUE_BYTE3	2
#define VALUE_BYTE4	3

union _Value2
{
	DWORD m_dwValue;
	WORD  m_wValue[VALUE_WORD2+1];
};

union _Value4
{
	DWORD m_dwValue;
	BYTE  m_byValue[VALUE_BYTE4+1];
};

#define INTRO_MAX 64

// 5-1
struct Skills
{
	int		m_nIndex;		// ��ų �ε���
	BYTE	m_byLevel;		// ��ų ����
};


// 8 - 3 ����Ʈ ���� ���� �����
struct Sinario_Quest
{
	_Value4 m_SQIndex; // ���⼭ �ϳ��� ������ �´�. 1��   // (0) index (1) ������ �Ѱ�

	_Value4 m_SQStatus; // 4�� 
	
	DWORD	m_QuestTime;		// ����Ʈ ���ӽð�
};

struct Board
{
	int		m_nMyBoard;				// ���� ���߰� �ִ� �� ��ȣ
	DWORD	m_aryPiece[12];	// ¥���� ������
};

struct Char_Info_Save
{//��� ������ ������ �ִ�
	long m_CharUnique;
	BYTE m_byCharName[CHARACTER_LENGTH];

	UINT  m_Level;
	UINT  m_Exp;
	UINT  m_Race;
	UINT  m_Cash;
	UINT  m_Guild;

	_Value2 m_StaminaSpell;				// ü��... ����
	_Value2 m_PowerAgility;				// ��....	��ø
	_Value2 m_IntelligenceMagic;		// ��...	����..    4-14 ��->������
	_Value2	m_BraveFamous;				// ���....	��		4-14 
	_Value2	m_PossesionRes;				// ������.. ���߿� �� ����..		4-14 
	_Value4 m_SubAbility;

	LONGLONG m_Conflict;
	ULONG m_LogoutPos;
	BYTE  m_byIntro[INTRO_MAX];

	//items
	Item_Info_Save m_EquipArray[MAX_EQUIPED];
	Item_Info_Save m_Inventory [MAX_INVENTORY1]; //tools
	//_uItemType	   m_Inventory2[MAX_INVENTORY2]; //foods , etc	

//	CHorseData horsedata; 

	// 5-1
	Skills	m_Skills[40];		// �տ� 10 : �⺻��ų �ڿ� 30�� ���� ��ų...��� �Ҷ� Ǯ���

	// 6-11
	_Value4	m_Potions;					// 0 : hp����  1 : mp����

	// 6 -24 
	UINT  m_Style;				// ĳ������ ���  

	// 8 - 3 ����Ʈ ���� ���� 
	Sinario_Quest s_Quest;	
	
	WORD	m_Extacy;		// �����ͽ� ���� 2
	Board   m_Board;		// ���� ������ ����� 52
	DWORD	m_DailogTime;	// �簥�ð�
	BYTE	m_bySkillPoint;	// ���� ��ų ����Ʈ
	BYTE	m_byWarpID;		// ������ �����
};

#define LOGIN_PROCESS_OK			0			//�α伺��
#define LOGIN_PROCESS_UNKNOWN		1			//�˼����� ����
#define LOGIN_PROCESS_INVALIDID		2			//���̵� Ʋ��
#define LOGIN_PROCESS_INVALIDPASS	3			//����� Ʋ��
#define LOGIN_PROCESS_OCCUPIED_ACC 	4			//������ �����
#define LOGIN_PROCESS_OCCUPIED_CHAR	5			//ĳ���Ͱ� �����


#define LOGIN_PROCESS_GAMEDB_DEAD	6			//���ӵ�� ������ �α��� �����Ҽ� ����
#define LOGIN_PROCESS_LOGIN_DEAD	7			//�α伭����  ""             ""
#define LOGIN_PROCESS_ACCOUNT_DEAD	8			//����������  ""             ""
#define LOGIN_PROCESS_GAMEINFO_DEAD	9			//�������������� ""
#define LOGIN_PROCESS_ZONE_DEAD		10          //��������      ""
#define LOGIN_PROCESS_ACCDB_DEAD	11          //������񼭹���      ""

#define LOGIN_PROCESS_ZONE_ERROR	12
#define LOGIN_PROCESS_ACCINFO_ERROR 13

#define LOGIN_PROCESS_MAKE_FAIL		14
#define LOGIN_PROCESS_MAKE_SUCCESS	15

#define GAMEDB_CHARSAVE_SUCCESS				128	
#define GAMEDB_CHARSAVE_FAIL				129
#define GAMEDB_CHARSAVE_UNKNOWN_ERROR		130
#define GAMEDB_CHARSAVE_PRODUCEITEM_OK		131
#define GAMEDB_CHARSAVE_PRODUCEITEM_FAIL	132
#define GAMEDB_CHARLOAD_SUCCESS				133
#define GAMEDB_CHARLOAD_FAIL				134
#define GAMEDB_CHARLOAD_SINGLE_SUCCESS		135
#define GAMEDB_CHARLOAD_SINGLE_FAIL			136
#define GAMEDB_CHARMAKE_FAIL				137
#define GAMEDB_CHARMAKE_SUCCESS				138
#define GAMEDB_CHARMAKE_OVERLAP				139
#define GAMEDB_CHARDEL_FAIL					140
#define GAMEDB_CHARDEL_SUCCESS				141



#define ACCDB_ACCID_NOTAVALIBLE				170
#define ACCDB_ACCPASS_INVALID				171
#define ACCDB_ACC_CHECKFAIL					172
#define ACCDB_ACC_CHECK_OK					173

#define ACCINFO_OCCUPYPROC_OK				200


//_________________________________________________________________________________
//
// define packet structures
//_________________________________________________________________________________

// CP_��¼���� Ŭ���̾�Ʈ���� ������ ���� ��Ŷ
// SP_��¼���� �������� Ŭ���̾�Ʈ�� ���� ��Ŷ
// MP_��¼���� �л� �������� ����ϴ� ������ ��� ��Ŷ
//CP messages-----------------------------------------------------------------------


struct CP_Move_Broadcast:public SIOCPPacket_DFOV				// �̵�
{
	CP_Move_Broadcast() {m_Header.wID[PACKET_HEADER] = CM_MOVE_BROADCAST;}
	WORD	angle;		// �̵�����
	DWORD	next;		// �̵� Ÿ�ϰ�	
	int		floor;		// �̵� ��
	DWORD	flag;		// ���� Ÿ�ϰ�
};

struct CP_DisConnect:public SIOCPPacket				// ������ ���
{
	CP_DisConnect () {m_Header.wID[PACKET_HEADER] = CM_DISCONNECT;}

};

//#define CM_LOGINZONE 
//client �� zone������ ������ ó�� �����ִ� �޼���
struct CP_LoginZone : public SIOCPPacket
{
	CP_LoginZone () {m_Header.wID[PACKET_HEADER] = CM_LOGINZONE;}
	long m_lUnique;
};

// #define CM_NORMAL_CHAT	267
struct CP_Normal_Chat : public SIOCPPacket
{
	CP_Normal_Chat() {m_Header.wID[PACKET_HEADER] = CM_NORMAL_CHAT;}
	char m_chatmsg[64];
};

struct CP_Private_Chat:public SIOCPPacket
{
	CP_Private_Chat() { m_Header.wID[PACKET_HEADER] = CM_PRIVATE_CHAT;}
	char m_to[16];
	char m_msg[64];
};

struct CP_Shout_Chat:public SIOCPPacket
{
	CP_Shout_Chat() { m_Header.wID[PACKET_HEADER] = CM_SHOUT_CHAT;}
	char m_msg[64];
};

struct CP_Party_Chat:public SIOCPPacket
{
	CP_Party_Chat() { m_Header.wID[PACKET_HEADER] = CM_PARTY_CHAT; }
	char m_msg[64];
};

struct CP_Trade:public SIOCPPacket
{
	CP_Trade() { m_Header.wID[PACKET_HEADER] = CM_TRADE; }
	BYTE	m_byCommand;		// 0 ��û, 1 ������ ���, 2 ������ ����, 3 �� �׼�, 4 Ȯ��, 5 ��� 6 ��û ����
	long	m_lTarget;		// 0 Ÿ�� ����ũ, 1,2 ������ ����ũ, 3 �� �׼�, 4,5 ���� -1
	BYTE	m_byPos;		// ��ġ..1�϶��� ���� �־��ְ� ���..�̿��� ��쿡�� ��� ����	6 �ϰ�� 1 ���� 0 ����
};

struct CP_Require_Index : public SIOCPPacket
{
	CP_Require_Index() { m_Header.wID[PACKET_HEADER] = CM_REQUIRE_INDEX; }
	BYTE	m_byKind;		// ����... 0:���� 1:�ڷ�����
	int	m_nIndex;		// npc index
};

struct CP_Warp:public SIOCPPacket
{
	CP_Warp() { m_Header.wID[PACKET_HEADER] = CM_WARP;}
	long m_lUnique;
};

struct CP_Takeoff_Item:public SIOCPPacket		// �������� �������̴ϱ� ����迭������ �˻�
{
	CP_Takeoff_Item() { m_Header.wID[PACKET_HEADER] = CM_TAKEOFF_ITEM; }
	DWORD		m_itemindex;		// ���� ������
	DWORD		m_pos;				// ����� �κ��丮 ��ġ 
	DWORD		m_IOveritem;		// ����� ���� �ִ� ������ �ε��� 
	DWORD		m_iOverunique;		// ����� ���� �ִ� ������ ����ũ 
	DWORD		m_itemUnique;		// ���� ������ equip pos (������)
};

struct CP_Change_Item:public SIOCPPacket		// �ٲٴ°� �κ��丮���� ������ �����۰˻�
{
	CP_Change_Item() { m_Header.wID[PACKET_HEADER] = CM_CHANGE_ITEM; }
	DWORD		m_itemindex;		// ������ �������ε���
	DWORD		m_itemunique;		// ������ �� ����ũ�� ���е� �ؾ��� 12-16
	BYTE		m_pos;				// ������ġ
};

struct CP_Drop_Item:public SIOCPPacket			// �����°� �����̰ų� �κ��̴ϱ� ��ġ ����
{
	CP_Drop_Item() { m_Header.wID[PACKET_HEADER] = CM_DROP_ITEM; }
	DWORD		m_itemindex;		// ���� ������ �ε���
	DWORD		m_itemunique;		// �����͵� ����ũ�� ����   12-16
	bool	m_equip;
};

struct CP_Pickup_Item:public SIOCPPacket
{
	CP_Pickup_Item() { m_Header.wID[PACKET_HEADER] = CM_PICKUP_ITEM; }
	DWORD		m_itemidx;
	DWORD		m_itemunique;		// �ݴ°͵� ����ũ �־��� 12-16
	DWORD	m_tileno;
	int		m_floor;	

};

struct CP_Buy:public SIOCPPacket
{
	CP_Buy() { m_Header.wID[PACKET_HEADER] = CM_BUY; }
	DWORD		m_itemidx;		// ������ �ε���
	int			m_count;		// ����
	int			m_marketindex;		// 1-14�߰�
	
	DWORD		m_pos;				// �ְ��� �ϴ� �κ��丮 ��ġ 6-9 �߰�
	DWORD		m_iOveritem;		// ������ �������� �ε���
	DWORD		m_iOverunique;		// ������ �������� ����ũ
};

struct CP_Attack_NPC:public SIOCPPacket
{
	CP_Attack_NPC() { m_Header.wID[PACKET_HEADER] = CM_ATTACK_NPC; }
	int		m_index;
	DWORD	m_tileno;
	DWORD	m_npc_unique;
	int		m_motion;
};

struct CP_Weather:public SIOCPPacket
{
	CP_Weather() { m_Header.wID[PACKET_HEADER] = CM_WEATHER; }
	int		m_weather;		// 0:����(���ħ) 1:�� 2:���� 3:��  4:��  
};

struct CP_Sell:public SIOCPPacket
{
	CP_Sell() { m_Header.wID[PACKET_HEADER] = CM_SELL; }
	int		m_item_index;
	bool	m_equip;
// ���߿� item��ȣ ü�� �������� �װſ� ���� ����
	DWORD	m_item_unique;
};

struct CP_Warp_Ready:public SIOCPPacket
{
	CP_Warp_Ready() { m_Header.wID[PACKET_HEADER] = CM_WARP_READY; }
};

struct CP_Attack_Item:public SIOCPPacket
{
	CP_Attack_Item() { m_Header.wID[PACKET_HEADER] = CM_ATTACK_ITEM; }
	DWORD	m_targetpos;
	DWORD	m_itemindex;
	DWORD	m_itemunique;
	int		m_motion;
};

struct CP_Change_Stat : public SIOCPPacket
{
	CP_Change_Stat() { m_Header.wID[ PACKET_HEADER ] = CM_CHANGE_STAT; }
	_Value2		m_strdex;		// ��ȭ��(��,��)
	_Value2		m_conwis;		// ��ȭ��(��,��)
};

struct CP_Use_Potion : public SIOCPPacket
{
	CP_Use_Potion() { m_Header.wID[ PACKET_HEADER ] = CM_USE_POTION; }
	BYTE		m_type;			// �������� HP���� �Ǻ� .....0:hp 1:mp 2:necta 3:tar
	// ���߿� ���� ����...ü��..���� �Ǵ� ���߼�...�����°� �߰�
};

struct CP_Try_Unite : public SIOCPPacket
{
	CP_Try_Unite() { m_Header.wID[PACKET_HEADER] = CM_TRY_UNITE; }
	long	m_unique;		// ��ü �õ� �� �� ����ũ
};

struct CP_Reply_Unite : public SIOCPPacket
{
	CP_Reply_Unite() { m_Header.wID[PACKET_HEADER] = CM_REPLY_UNITE; }
	bool	m_reply;		// ��� : true : ��ü, false : ���� ��پ�..
	long	m_unique;		// �õ��Ϸ��� �ߴ���
};

struct CP_Seperate_Unite : public SIOCPPacket
{
	CP_Seperate_Unite() { m_Header.wID[PACKET_HEADER] = CM_SEPERATE_UNIT; }		// �̰� ������� ���� ��ü�ѳѵ�����
};

// 3-14
struct CP_Toggle_Run : public SIOCPPacket
{
	CP_Toggle_Run() { m_Header.wID[PACKET_HEADER] = CM_TOGGLE_RUN; }		// �̰� ������� �ȱ�/�޸��� �÷��� ������ �ش�
};

struct CP_Toggle_AttackMode : public SIOCPPacket
{
	CP_Toggle_AttackMode() { m_Header.wID[PACKET_HEADER] = CM_TOGGLE_ATTACKMODE; }		// �̰� ������� ����� on/off
};

// 3-19............10-13
struct CP_Party : public SIOCPPacket
{
	CP_Party() { m_Header.wID[PACKET_HEADER] = CM_PARTY; }
	BYTE	m_byCommand;	// 0 : ����, 1 : �ʴ�, 2 : �ʴ���ok 3 : �ʴ���no, 4 : ��� �߰�, 5 : ��� ����, 6 : ��Ƽ ����
	long	m_lUnique;		// command = 0 -> �Ⱦ� : -1
							// command = 1 -> �ʴ��� ĳ�� ����ũ
							// command = 2 -> ���� ���� ĳ�� ����ũ..��û�ѳ��� �ǰ���..
							// command = 3 -> ���� ���� ĳ�� ����ũ..��û�ѳ��� �ǰ���..
							// command = 4 -> �߰��� ��� ����ũ
							// command = 5 -> ���� ��� ����ũ
							// command = 6 -> ��Ƽ ������ �Ⱦ� : -1..�̰� ������ ��Ƽ���� ����
};

// 3-24����
struct CP_Attack_PC : public SIOCPPacket
{
	CP_Attack_PC() { m_Header.wID[PACKET_HEADER] = CM_ATTACK_PC; }
	long	m_lUnique;			// Ÿ������ ����ũ
	DWORD	m_dwTargetPos;		// xblock���� ����... �˻��� �ٲ��� �ϴµ�...-��-
	int		m_nMotion;			// ���� ����
};

struct CP_Request_PK : public SIOCPPacket
{
	CP_Request_PK() { m_Header.wID[PACKET_HEADER] = CM_REQUEST_PK; }
	long	m_lUnique;		// Ÿ�� ���� ����ũ
};

struct CP_Reply_PK : public SIOCPPacket
{
	CP_Reply_PK(){ m_Header.wID[PACKET_HEADER] = CM_REPLY_PK; }
	long	m_lUnique;			// ��û�ѳ�
	bool	m_bReply;			// ���
};

// 3 - 27 first 032
struct CP_Horse_Down : public SIOCPPacket							//������ �����ٰ� �� 
{
	CP_Horse_Down() { m_Header.wID[PACKET_HEADER] = CM_HORSE_DOWN; }
	DWORD m_dwDownpos;
};

// 3 -28 032
struct CP_Horse_Ride : public SIOCPPacket							//���� ź�ٰ� �� 
{
	CP_Horse_Ride() { m_Header.wID[PACKET_HEADER] = CM_HORSE_RIDE; }
	int		m_iHorseIndex;		// ���߿� �ʿ� ��������
	long	m_lUnique;			// �� ����ũ
};

// 3-29

// 4-29
struct CP_Change_Class : public SIOCPPacket
{
	CP_Change_Class() { m_Header.wID[PACKET_HEADER] = CM_CHANGE_CLASS; }
	BYTE	m_byJob;		// �����ϰ��� �ϴ� ����...255�� ���°�..
	BYTE	m_bySkill;		// ������ �ϴ� ��ų
};

struct CP_Reply_Revive : public SIOCPPacket
{
	CP_Reply_Revive() { m_Header.wID[PACKET_HEADER] = CM_REPLY_REVIVE; }
	bool	m_bReply;		// true :  ��		false : �ƴ�
};

struct CP_Horse_Attr : public SIOCPPacket
{
	CP_Horse_Attr(){ m_Header.wID[PACKET_HEADER] = CM_HORSE_ATTR; }
	int m_iHorseColor;      //�� ���� 
	int m_iSeatColor;		//�� ���� ����
	int m_iHorseIndex;		//�� �� ����
	int m_iSeatIndex;		//���� ����
};

struct CP_Horse_Keepin : public SIOCPPacket
{
	CP_Horse_Keepin(){ m_Header.wID[PACKET_HEADER] = CM_HORSE_KEEPIN; }
};


struct CP_Horse_Keepout : public SIOCPPacket
{
	CP_Horse_Keepout(){ m_Header.wID[PACKET_HEADER] = CM_HORSE_KEEPOUT; }
	char    m_horsename[12];
	//int m_iHorsehouse;
};

// 4-8
struct CP_Taming_Horse : public SIOCPPacket
{
	CP_Taming_Horse() { m_Header.wID[PACKET_HEADER] = CM_TAMING_HORSE; }
	DWORD	m_dwidx;		// �� �ε���
	long	m_lUnique;		// �� ����ũ
};

struct CP_Treat_Carrot : public SIOCPPacket
{
	CP_Treat_Carrot() { m_Header.wID[PACKET_HEADER] = CM_TREAT_CARROT; }
	DWORD	m_dwidx;		// �� �ε���
	long	m_lUnique;		// �� ����ũ
};

struct CP_Use_Skill : public SIOCPPacket
{
	CP_Use_Skill() { m_Header.wID[PACKET_HEADER] = CM_USE_SKILL; }
	bool	m_bSpecial;		// ��������ΰ�?	true:Ưȭ��� false:�⺻���
	int		m_nSkill;		// ����� ��ų
	int		m_nNPCIndex;	// ���̶�� �� �ε���...����̸� -1
	long	m_lUnique;		// Ÿ���� �� ����ũ..�ڽ��̸� -1, ����̸� �ε���-1�� ��� ����ũ, ���̸� �� ����ũ
};

struct CP_Inven_Change : public SIOCPPacket
{
	CP_Inven_Change() { m_Header.wID[PACKET_HEADER] = CM_INVEN_CHANGE; }
	DWORD		m_itemidx;			// �ٲ� id 
	DWORD		m_itemunique;		// �ٲ� ����ũ 
	DWORD		m_ipos;				// ��ġ 
	DWORD		m_iOveritem;		// ������ �������� �ε���
	DWORD		m_iOverunique;		// ������ �������� ����ũ 
};

struct CP_Admin_Chat : public SIOCPPacket
{
	CP_Admin_Chat(){ m_Header.wID[PACKET_HEADER] = CM_ADMIN_CHAT; }
	char		m_strMsg[64];		// �޼���
};

struct CP_Now_Time : public SIOCPPacket
{
	CP_Now_Time() { m_Header.wID[PACKET_HEADER ] = CM_NOW_TIME; }
	DWORD		m_dwTime;
};

struct CP_Kill_Client : public SIOCPPacket
{
	CP_Kill_Client() { m_Header.wID[PACKET_HEADER] = CM_KILL_CLIENT; }
	// ���߿� ��������..id�� ���� ����ũ�� ����
};

struct CP_Check_Connection : public SIOCPPacket
{
	CP_Check_Connection() { m_Header.wID[PACKET_HEADER] = CM_CHECK_CONNECTION; }
};

// �������� ����
struct CP_Require_Data : public SIOCPPacket
{
	CP_Require_Data() { m_Header.wID[PACKET_HEADER] = CM_REQUIRE_DATA; }
	BYTE		m_byKind;		// 0 : ����  1: npc   2: item
	DWORD		m_dwUnique;		// ������� ����ũ npc,item���� ����ũ
	DWORD		m_dwIndex;		// npc,item index , ������� 0
};

// 7 - 14 �� ���ư��� ��Ŷ 
struct CP_Chr_Dir : public SIOCPPacket
{
	CP_Chr_Dir() { m_Header.wID[PACKET_HEADER] = CM_CHR_DIR; }
	BYTE		m_byDir;  // �� ���� 
};

// 7 - 16 npc HP ��û
struct CP_NPC_Energy : public SIOCPPacket
{
	CP_NPC_Energy() { m_Header.wID[PACKET_HEADER] = CM_NPC_ENERGY; }
	DWORD		m_dwIndex;  // ������ �ϴ� npc�ε���
	DWORD		m_dwUnique;	// npc ����ũ 
};

// 7 - 16 npc ���� ��û
struct CP_NPC_Status : public SIOCPPacket
{
	CP_NPC_Status() { m_Header.wID[PACKET_HEADER] = CM_NPC_STATUS; }
	DWORD		m_dwIndex;
	DWORD		m_dwUnique; 
};

// 8 - 1 quest��û
struct CP_Get_Quest : public SIOCPPacket
{
	CP_Get_Quest() { m_Header.wID[PACKET_HEADER] = CM_GET_QUEST; }
	BYTE		m_QuestClass;
	BYTE		m_QuestIndex;
};

struct CP_Put_Piece : public SIOCPPacket
{
	CP_Put_Piece() { m_Header.wID[PACKET_HEADER] = CM_PUT_PIECE; }
	DWORD		m_dwIndex;		// �������� �ε���
	DWORD		m_dwUnique;		// �������� ����ũ
	BYTE		m_byPosition;	// ���� ��ġ
};

struct CP_Succ_Quest : public SIOCPPacket
{
	CP_Succ_Quest() { m_Header.wID[PACKET_HEADER] = CM_SUCC_QUEST; }
};

struct CP_Complete_Puzzle : public SIOCPPacket
{
	CP_Complete_Puzzle() { m_Header.wID[PACKET_HEADER] = CM_COMPLETE_PUZZLE; }
	DWORD	m_dwItem;		// ���׷��̵��� ������ ����ũ..������ ����ũ�� �Ȱ�ħ
	BYTE 	m_byNumber[5];		// ������ �ε����� ���ڸ� ��ȣ..���ĺ��̴ϱ�..
	DWORD	m_dwUnique[6];		// ���� ������ ����ũ + ���� ����ũ
};

struct CP_Quest_Check : public SIOCPPacket
{
	CP_Quest_Check() { m_Header.wID[PACKET_HEADER] = CM_QUEST_CHECK; }
	BYTE m_QuestClass;		//	���� ���� ^^ 0 �̸� �ó����� 1�̸� event 2  ���� 3 ���� 
	BYTE m_Case;			// �̰� ����Ǽ� 0 npc die 1 pickup item 2 make item 3 meet npc 4�� pickup item drop
	// 5��  make item �� drop
	DWORD m_Index;			// 
};

struct CP_Mode_Change : public SIOCPPacket
{
	CP_Mode_Change() { m_Header.wID[PACKET_HEADER] = CM_MODE_CHANGE; }
	BYTE	m_byType;		// 0 :charge 1:��� ��� 2:�ȱ� �ٱ� 3:pk ..��Ÿ ���...���� ���� ��� ���� ����
							// 4 : Į�̱�
};

struct CP_Register_Board : public SIOCPPacket
{
	CP_Register_Board(){ m_Header.wID[PACKET_HEADER] = CM_REGISTER_BOARD; }
	DWORD	m_dwIndex;
	DWORD	m_dwUnique;
};

struct CP_Operator_Mode : public SIOCPPacket
{
	CP_Operator_Mode(){ m_Header.wID[PACKET_HEADER] = CM_OPERATOR_MODE; }	
	BYTE	m_Class; 			// �޽��� ���� 0 ���� 11 ����	
	char 	m_CharID[12];		// ã���� �ϴ� ���̵� 
	DWORD	m_ItemNpcTarget;	// NPC, ITEM
	byte 	m_Map;				// �� ��ȣ 
	int  	m_X;				// X
	int  	m_Y;				// Y 
	DWORD	m_CoinTime;			// �ٵ�, Ÿ��
	char 	m_AllChat[64];		// ��ڰ� ��ü ��ȭ�� ����.
};

struct CP_Operator_Chat : public SIOCPPacket
{
	CP_Operator_Chat() {m_Header.wID[PACKET_HEADER] = CM_OPERATOR_CHAT;}
	char m_chatmsg[64];
};

struct CP_Skill_Upgrade : public SIOCPPacket
{
	CP_Skill_Upgrade() { m_Header.wID[PACKET_HEADER] = CM_SKILL_UPGRADE; }
	BYTE	m_bySkill;	// ���� ��ų
};

// �ְ�ޱ� ��û
struct CP_Try_Itemdeal : public SIOCPPacket
{
	CP_Try_Itemdeal() { m_Header.wID[PACKET_HEADER] = CM_TRY_ITEMDEAL; }
	long	m_rev_unique;		// �޴³� 
	DWORD	m_Itemindex;
	DWORD   m_Itemunique;	
};

// �ְ�ޱ� ����
struct CP_Reply_Itemdeal : public SIOCPPacket
{
	CP_Reply_Itemdeal() { m_Header.wID[PACKET_HEADER] = CM_REPLY_ITEMDEAL; }
	long	m_send_unique;		//
	DWORD	m_Itemindex;
	DWORD   m_Itemunique;	
};

struct CP_Storage_Change : public SIOCPPacket
{
	CP_Storage_Change(){ m_Header.wID[PACKET_HEADER] = CM_STORAGE_CHANGE; }
	DWORD		m_itemidx;			// �ٲ� id	// ���̸� 0:�ְ� 1:���� 
	DWORD		m_itemunique;		// �ٲ� ����ũ // �� 
	BYTE		m_Pos[4];			// 0. ���� tab 1. �̵��� tab 2. �̵��� ��ġ 3. command:0��ǲ1:�ƿ�ǲ2:ü����3:��4:â�����
};

struct CP_Teleporter : public SIOCPPacket
{
	CP_Teleporter() { m_Header.wID[ PACKET_HEADER] = CM_TELEPORTER; }
	BYTE	m_byIndex;		// �ε��� ��ȣ
	BYTE	m_bySort;		// ��ϳ� ��ȣ(����)
};

struct CP_Horse_Name : public SIOCPPacket
{
	CP_Horse_Name() { m_Header.wID[PACKET_HEADER] = CM_HORSE_NAME; }
	char m_HorseName[12];
	BYTE m_Type;
};

struct CP_Horse_Away : public SIOCPPacket
{
	CP_Horse_Away() { m_Header.wID[PACKET_HEADER] = CM_HORSE_AWAY; }
};

struct CP_Normal_Logout : public SIOCPPacket
{
	CP_Normal_Logout() { m_Header.wID[ PACKET_HEADER ] = CM_NORMAL_LOGOUT; }
};

struct CP_User_Info : public SIOCPPacket
{
	CP_User_Info() { m_Header.wID[ PACKET_HEADER ] = CM_USER_INFO; }
	char			CardName[128];
	LARGE_INTEGER	DriverVersion;
	DWORD			OSVersion;
	char			DXVersion[32];
};


// SP messages-for game play----------------------------------------------------------------
struct SP_Move_Broadcast:public SIOCPPacket					// �̵�
{
	SP_Move_Broadcast() {m_Header.wID[PACKET_HEADER] = SM_MOVE_BROADCAST;}
	DWORD	who;	// ����...infoserver & db���� ������ �ִ� ���� ĳ���� id
	DWORD	start;	// ��𿡼�
	DWORD	dest;	// ����
	int		floor;	// 7-5
	bool	m_bUnite;
	long	m_lUnite_unique;
	// 3-15
//	bool	m_bAttackMode;		// ����� on/off
	bool	m_bCurrent;			// ���� �ȱ�/�ٱ�

	//9 - 17 �밢�� �̵� ���� �߰� 	
	DWORD   m_flag;				// ���� �˾Ƽ� 
};

struct SP_Appear:public SIOCPPacket					// Ŭ���̾�Ʈ ���ӽ� ���
{
	SP_Appear() {m_Header.wID[PACKET_HEADER] = SM_APPEAR;}
	long		m_lUnique;
	DWORD		m_where;
	int			m_floor;
	Char_Info	char_info;
	// ��ü ���� 3-11�߰�
	bool		m_bUnite;			// ��ü ����
	long		m_lUnite_unique;	// ��ü�ѳ� ����ũ
	// 3-15
	bool		m_bAttackMode;		// ����� on/off

	//3-31	
	char		m_HorseName[12];	// ���̸� ��� 
	bool		m_bHorseRide;		// ���� Ÿ�� �ִ�����
	DWORD		m_type;				// 4��Ʈ�� �߻�������?

	bool		m_bCharge;			// true : ������� ���� false : �Ϲ� ����
	bool		m_bNecta;			// true : ���ð� �ִ� ���� false : �Ϲ� ����
	bool		m_bTar;				// true : �ǰ��ִ� false : �Ϲ� ����
	bool		m_bPK;				// true : pk���(�Ƶ� ������) false : pk��� �ƴ�

	bool		m_bToggle;			// �ȱ� ��� ���� 
	bool		m_bSword;			// Į��ġ
	bool		m_bSit;				// �ɾ� �ֳ�??
	BYTE		m_byStatus;			// ���� ������ Login�ؼ� ����� : 0 , Xblock���� ���ͼ� ����� : 1
	BYTE		m_bySkillStatus;	// ���� �ɷ��ִ� ���� ��ȭ ��ų..base.h����
};

struct SP_DisAppear:public SIOCPPacket				// Ŭ���̾�Ʈ �������� ���
{
	SP_DisAppear() {m_Header.wID[PACKET_HEADER] = SM_DISAPPEAR;}
	long	who;	// ������ ��...
	// 3-11
	bool	m_bUnite;
	long	m_lUnite_unique;
};

struct SP_LogInZone : public SIOCPPacket
{
	SP_LogInZone() {m_Header.wID[PACKET_HEADER] = SM_LOGINZONE;}	
//	Char_Info_All	m_User_Info;
	DWORD m_dwResult;
};

struct SP_Check_Connection:public SIOCPPacket		// 6-26 �߰�
{
	SP_Check_Connection() { m_Header.wID[PACKET_HEADER] = SM_CHECK_CONNECTION; }
};

struct SP_Normal_Chat:public SIOCPPacket
{
	SP_Normal_Chat() {m_Header.wID[PACKET_HEADER] = SM_NORMAL_CHAT; ZeroMemory( m_chatmsg, 64 ); }
	long m_unique;		// ���ѳ�
	char m_chatmsg[64];		// ����
};

struct SP_Private_Chat:public SIOCPPacket		// ���� �Ӹ� ��Ŷ
{
	SP_Private_Chat() { m_Header.wID[PACKET_HEADER] = SM_PRIVATE_CHAT;}
	char m_msg[64];			// �޼���
};

struct SP_NPC_Appear:public SIOCPPacket		// �̵��� ������ ��Ÿ���ų� ��ȯ������ �ֺ��� ��ۿ�
{
	SP_NPC_Appear() {m_Header.wID[PACKET_HEADER] = SM_NPC_APPEAR;}
	int		m_NPC_Index;
	int		m_floor;
	DWORD	m_pos;
	DWORD	m_npc_unique;
	DWORD	m_type;				// 0 ���� ���� 1, �������  2. ���� ���� 3. ���� ����
	BYTE	m_npc_status;		// npc���� 
	BYTE	m_bySkillStatus;	// ���� �ɷ��ִ� ���� ��ȭ ��ų..base.h����
};

struct SP_NPC_Move:public SIOCPPacket
{
	SP_NPC_Move() { m_Header.wID[PACKET_HEADER] = SM_NPC_MOVE;}
	int	m_NPC_Index;
	DWORD m_npc_unique;
	DWORD m_cur;
	DWORD m_dest;
	int   m_floor;
	bool	m_bFight;		// �̵� ���..true�� �ٱ�..false�� �ȱ�
};

struct SP_Shout_Chat:public SIOCPPacket			// ������ Ŭ���̾�Ʈ���� ���� ��°�
{
	SP_Shout_Chat() { m_Header.wID[PACKET_HEADER] = SM_SHOUT_CHAT;}
	char m_msg[64];
};

struct SP_Party_Chat:public SIOCPPacket
{
	SP_Party_Chat() { m_Header.wID[PACKET_HEADER] = SM_PARTY_CHAT; }
	long	m_lUnique;		// ���� ��� ����ũ...�̸�,����ũ�� ó���� �޾Ƽ� ������ �ִ�
	char	m_msg[64];
};

struct SP_Trade:public SIOCPPacket
{
	SP_Trade() { m_Header.wID[PACKET_HEADER] = SM_TRADE; }
	BYTE	m_byCommand;		// 0 ��û, 1 ������ ���, 2 ������ ����, 3 �� �׼�, 4 Ȯ��, 5 ��� 6 ��û ���� 7 �ŷ�����
	long	m_lUser;		// 0 Ÿ�� ����ũ, 1,2 ������ ����ũ, 3 �� �׼�, 4,5 ���� -1...������ ����ϰų� ���� ��
	Item_Info_Save 	m_lTarget;	// �ȿ� ����ũ�� ���� ������ ���, ���Ͽµ��� �����ۿ��� ���õǾ� ���.
	BYTE	m_byPos;		// ��ġ..1�϶��� ���� �־��ְ� ���..�̿��� ��쿡�� ��� ���� 6 �ϰ�� 1 ���� 0 ����
};

struct SP_Require_Index : public SIOCPPacket
{
	SP_Require_Index() { m_Header.wID[PACKET_HEADER] = SM_REQUIRE_INDEX; }
	BYTE	m_byKind;		// ����... 0:���� 1:�ڷ�����
	BYTE	m_byIndex;		// �ش� ��Ͽ����� ��ȣ
};



struct SP_Warp: public SIOCPPacket
{
	SP_Warp() { m_Header.wID[PACKET_HEADER] = SM_WARP; }
	int		m_Server;		// �����ϰ� �� ����
	long	m_Unique;		// �����ϴ� ��
	int		m_Map_ID;		// ������ �ʹ�ȣ
	DWORD	m_Pos;			// ������ �ʿ��� ��Ÿ�� ��ǥ
	int		m_floor;		// ��..�׻� 0
	bool	m_result;		// ���
	// 3-11
	bool	m_bUnite;
	long	m_lUnite_unique;
};

struct SP_Takeoff_Item:public SIOCPPacket
{
	SP_Takeoff_Item() { m_Header.wID[PACKET_HEADER] = SM_TAKEOFF_ITEM; }
	long	m_lUnique;		// �����
	DWORD		m_itemindex;	// ������
	DWORD		m_dwitemUnique;  // ���°� ����ũ
};

struct SP_Change_Item:public SIOCPPacket
{
	SP_Change_Item() { m_Header.wID[PACKET_HEADER] = SM_CHANGE_ITEM; }
	long	m_lUnique;
	DWORD		m_itemindex;	// ������
	DWORD		m_itemunique;	// �Դ°� ����ũ
	DWORD		m_ipos;		// ��ġ
	BYTE		m_arraypos;   // 
};

struct SP_Drop_Item:public SIOCPPacket
{
	SP_Drop_Item() { m_Header.wID[PACKET_HEADER] = SM_DROP_ITEM; }
	DWORD		m_itemidx;		// ���� ������ �ε���...�κ����� �����Ұ�
	DWORD		m_itemunique;	// ���� ������ ����ũ
};

struct SP_Item_Appear:public SIOCPPacket
{
	SP_Item_Appear() { m_Header.wID[PACKET_HEADER] = SM_ITEM_APPEAR; }
	DWORD		m_itemidx;		// ���� ǥ�õ� �׸� �ε���
	DWORD		m_itemunique;	// ����ũ
	DWORD	m_pos;			// ��ġ
	int		m_floor;
	bool	m_changed;		// ���Ż���
	DWORD	m_dwPos;
};

struct SP_Pickup_Item:public SIOCPPacket
{
	SP_Pickup_Item() { m_Header.wID[PACKET_HEADER] = SM_PICKUP_ITEM; }
	DWORD	m_item_index;
	DWORD	m_item_unique;
	bool	m_res;
	DWORD	m_pos;			// �κ��� �� ��ġ
	_Value4 m_iteminfo;     // �������� ������
};

// �ݰų� �浹�ؼ� �������ų� �¾��׾������� �̰�
struct SP_Item_Disappear:public SIOCPPacket
{
	SP_Item_Disappear() { m_Header.wID[PACKET_HEADER] = SM_ITEM_DISAPPEAR; }
	DWORD	m_itemidx;
	DWORD	m_itemunique;
	DWORD	m_tileno;
	int		m_floor;
};

struct SP_Buy_OK:public SIOCPPacket
{
	SP_Buy_OK() { m_Header.wID[PACKET_HEADER] = SM_BUY_OK; }
	DWORD	m_itemidx;		// ������ �ε���
	DWORD	m_itemunique;	// �� ������ ����ũ
	int		m_count;		// ����
	DWORD   m_pos;			// ������ ��ġ
	_Value4 m_iteminfo;		// �������� ���� life , upgrade , color
};

#define NOT_ENOUGH_MONEY	0
#define NOT_ENOUGH_INVEN	1
struct SP_Buy_Error:public SIOCPPacket
{
	SP_Buy_Error() { m_Header.wID[PACKET_HEADER] = SM_BUY_ERROR; }
//	char	m_msg[32];		// �̺κ��� ���߿� ���ŵ�...������ error��Ŷ ������ ���� ����..�ȱ⵵ ��������...
	BYTE	m_error;
};

struct SP_Attack_NPC:public SIOCPPacket
{
	SP_Attack_NPC() { m_Header.wID[PACKET_HEADER] = SM_ATTACK_NPC; }
	DWORD	m_tileno;		// ���� ��ġ
	DWORD	m_npcunique;	// ���� ���ϴ� npc����ũ
	long	m_unique;		// ���ݵ��� ���ϴ� pc����ũ
	int		m_dmg;			// ���ݷ�...0�̸� ȸ���Ѱ�
	int		m_motion;
	// 3-15
//	bool	m_bAttackMode;	// ���� ���
	bool	m_bCritical;	// true : ũ��Ƽ�� false : ��

};

struct SP_Weather:public SIOCPPacket
{
	SP_Weather() { m_Header.wID[PACKET_HEADER] = SM_WEATHER; }
	int		m_weather;		// base.h �� define ����
	int		m_weather2;		// ��������
};

struct SP_NPC_Respawn:public SIOCPPacket
{
	SP_NPC_Respawn() { m_Header.wID[PACKET_HEADER] = SM_NPC_RESPAWN; }
	int		m_npc_index;		// ������ �ε���
	DWORD	m_npc_unique;		// ���� ����ũ
	DWORD	m_tileno;			// ������ġ
	int		m_floor;			// ���� ��
	bool	m_speechflag;		// �� �ұ� ����
};

struct SP_NPC_Die:public SIOCPPacket
{
	SP_NPC_Die() { m_Header.wID[PACKET_HEADER] = SM_NPC_DIE; }
	int		m_npc_index;		// �״� ��� Ʋ���� �ε���
	DWORD	m_npc_unique;		// �״� �� ����ũ
	bool	m_speechflag;		// �� �ұ� ����
	long	m_lUnique;			// ���� �ѳ�
	bool	m_bType;			// true : �ٰŸ� false : ���Ÿ�
};

struct SP_Sell_OK:public SIOCPPacket
{
	SP_Sell_OK() { m_Header.wID[PACKET_HEADER] = SM_SELL_OK; }
	DWORD		m_item_index;			// �������� ������
// ���߿� item��ȣ ü�� �������� �װſ� ���� ����
	DWORD	m_item_unique;
};

struct SP_Sell_Error:public SIOCPPacket
{
	SP_Sell_Error() { m_Header.wID[PACKET_HEADER] = SM_SELL_ERROR; }
//	BYTE	m_error_code;			// �����ڵ�
};

struct SP_NPC_Attack_PC:public SIOCPPacket
{
	SP_NPC_Attack_PC() { m_Header.wID[PACKET_HEADER] = SM_NPC_ATTACK_PC; }
	int		m_index;	// npc index
	DWORD	m_npc;		// npc unique
	long	m_target;	// target user unique
	int		m_motion;	// Ʋ���� ����
	int		m_dmg;		// ������
	int		m_dialog;	// ���
	bool	m_speechflag;		// �� �ұ� ����

};

struct SP_Time_Change:public SIOCPPacket
{
	SP_Time_Change() { m_Header.wID[PACKET_HEADER] = SM_TIME_CHANGE; }
	short	m_time;		// �ٲ� �ð�
};

struct SP_Item_Change:public SIOCPPacket		// �����ϴ� ��
{
	SP_Item_Change() { m_Header.wID[PACKET_HEADER] = SM_ITEM_CHANGE; }
	DWORD	m_itemindex;		// �ε���
	DWORD	m_itemunique;		// ����ũ
};

struct SP_Item_Move:public SIOCPPacket
{
	SP_Item_Move() { m_Header.wID[PACKET_HEADER] = SM_ITEM_MOVE; }
	DWORD	m_itemindex;		// �ε���
	DWORD	m_itemunique;		// ����ũ
	DWORD	m_tileno;			// �̵��� ��ġ
	int		m_floor;			// ��
};

// �ð� �Ǽ� �Ҹ� 
struct SP_Item_Extinction:public SIOCPPacket
{
	SP_Item_Extinction() { m_Header.wID[PACKET_HEADER] = SM_ITEM_EXTINCTION; }
	DWORD	m_itemindex;	// �ε���
	DWORD	m_itemunique;	// ������ ����ũ
};

struct SP_Attack_Item:public SIOCPPacket
{
	SP_Attack_Item() { m_Header.wID[PACKET_HEADER] = SM_ATTACK_ITEM; }
	long	m_userunique;		// �����ϴ³� ����ũ
	DWORD	m_itemindex;		// ���ݴ��� �� �ε���
	DWORD	m_itemunique;		// ���ݴ��� �� ����ũ
	int		m_motion;
	int		m_dmg;
	// 3-15
//	bool	m_bAttackMode;		// �ʿ� ������..�ֳ��ϸ� ĳ���͵��� ó�� �޴� appear�� move�� appear���� ���� ��带 �˼� �ִ�
};

// npc ����
struct SP_NPC_Speech : public SIOCPPacket
{
	SP_NPC_Speech() { m_Header.wID[PACKET_HEADER] = SM_NPC_SPEECH; }
	int		m_npcindex;			// �ε���
	DWORD	m_npcunique;		// ����ũ
	BYTE	m_status;			// ����� ����
};

struct SP_User_Recover:public SIOCPPacket
{
	SP_User_Recover(){ m_Header.wID[ PACKET_HEADER] = SM_USER_RECOVER; }
	long		m_unique;		// ü��,���� ����..���߿� ��Ƽ�� �Ǹ� ������� �˾ƾ� �ϴϱ�
	_Value2		m_recover;		// ȸ���� ü��,����...�� �ƴ϶�..���� ���� ���̴�
};

struct SP_User_Die : public SIOCPPacket
{
	SP_User_Die() { m_Header.wID[ PACKET_HEADER ] = SM_USER_DIE; }
	long		m_userunique;
};

struct SP_Level_Up : public SIOCPPacket
{
	SP_Level_Up() { m_Header.wID[ PACKET_HEADER ] = SM_LEVEL_UP; }
	long		m_userunique;
	int			m_level;
	int			m_point;
	BYTE		m_type;   // 0 user 1 horse
};

struct SP_Change_Stat : public SIOCPPacket
{
	SP_Change_Stat() { m_Header.wID[ PACKET_HEADER] = SM_CHANGE_STAT; }
	BYTE		m_which;
	bool		m_result;	// ���
	int			m_point;
};

struct SP_Change_Exp : public SIOCPPacket
{
	SP_Change_Exp() { m_Header.wID[ PACKET_HEADER ] = SM_CHANGE_EXP; }
	BYTE		m_type;			// 0: user 1: horse
	DWORD		m_exp;			// �� ���� ����ġ��
};

struct SP_Confirm_Cash : public SIOCPPacket
{
	SP_Confirm_Cash() { m_Header.wID[ PACKET_HEADER ] = SM_CONFIRM_CASH; }
	DWORD		m_money;		// �ϰ� ������ �ִ� ���� ���̾�
};

struct CHR_STAT
{
	// 4-15
	_Value2 m_PowerAgility;				// ��....	��ø
	_Value2 m_IntelligenceMagic;		// ��...	����..    4-14 ��->������
	_Value2	m_BraveFamous;				// ���....	��		4-14 
	_Value2	m_PossesionRes;				// ������.. ���߿� �� ����..		4-14 
	//
	int		m_att;			// ���ݷ�
	int		m_sucrate;		// ������
	int		m_evaderate;	// ȸ����
	int		m_mp;			// ����
	int		m_magicac;		// ���� ����
	int		m_hp;			// ü��
	int		m_ac;			// ����
	int		m_magicatt;		// ���� ���ݷ�
};

struct SP_Confirm_Stat : public SIOCPPacket
{
	SP_Confirm_Stat() { m_Header.wID[ PACKET_HEADER ] = SM_CONFIRM_STAT; }
	CHR_STAT	m_stat;
};

struct SP_Use_Potion : public SIOCPPacket
{
	// ���߿� �ش� ������ ���� ���� ǥ�ø� ���� ����ϴ°� ������
	SP_Use_Potion() { m_Header.wID[ PACKET_HEADER ] = SM_USE_POTION; }
	_Value2		m_recover;
};

struct SP_Destroy_Item : public SIOCPPacket
{
	SP_Destroy_Item() { m_Header.wID[ PACKET_HEADER ] = SM_DESTROY_ITEM; }
	int		m_index;
	DWORD	m_unique;
	long	m_lUnique;	// �μ���
};

struct SP_Try_Unite : public SIOCPPacket
{
	SP_Try_Unite() { m_Header.wID[PACKET_HEADER] = SM_TRY_UNITE; }
	long	m_unique;		// ��ü �õ��Ϸ��� ���� ����ũ( ������ ���� �޼����� )
};
struct SP_Reply_Unite : public SIOCPPacket
{
	SP_Reply_Unite() { m_Header.wID[PACKET_HEADER] = SM_REPLY_UNITE; }
	long	m_man_unique;		// ��ü�ϴ� �� ����ũ. -1�̸� ���д� �̳ʸ�
	long	m_fairy_unique;		// ��ü�ϴ� �� ����ũ. -1�̸� ���д� �̳ʸ�
};
struct SP_Seperate_Unite : public SIOCPPacket
{
	SP_Seperate_Unite() 
	{ m_Header.wID[PACKET_HEADER] = SM_SEPERATE_UNIT; m_human_unique = -1; m_fairy_unique = -1;}		// �̰� �����ָ� ���� �ڱⰡ ��Ʈ���� �Ѵ�..�غ��ϰ�.�ڱ� �׷�����..
	long	m_human_unique;		// �и��� �ΰ� ����ũ
	long	m_fairy_unique;		// �и��� �� ����ũ
	DWORD	m_fairy_position;	// �и��� ���� ���� �ڸ�
};		// �̰� ���� �ѵ��� �� ����ũ�� ���� �и��� ����(�� ���̸��� �����)�� �׸��� �� appear�޾Ƽ� �׸���.

// 3-14
struct SP_Toggle_Run : public SIOCPPacket
{
	SP_Toggle_Run() { m_Header.wID[PACKET_HEADER] = SM_TOGGLE_RUN; }
	bool	m_bCurrent;		// false : �ȱ�, true : �޸���
};
struct SP_Toggle_AttackMode : public SIOCPPacket
{
	SP_Toggle_AttackMode() { m_Header.wID[PACKET_HEADER] = SM_TOGGLE_ATTACKMODE; }
	long	m_lUnique;		// �������³� ����ũ
	bool	m_bCurrent;		// false : �����ִ�, true : �����ִ�..����� ����
};


struct SP_Party : public SIOCPPacket
{
	SP_Party() { m_Header.wID[PACKET_HEADER] = SM_PARTY; }
	BYTE	m_byCommand;	// 0 : ����, 1 : �ʴ�, 2 : �ʴ���ok 3 : �ʴ���no, 4 : ��� �߰�, 5 : ��� ����, 6 : ��Ƽ ����
	long	m_lUnique;		// command = 0 -> �Ⱦ� : -1
							// command = 1 -> �ʴ��� ĳ�� ����ũ
							// command = 2 -> ���� ���� ĳ�� ����ũ..��û�ѳ��� �ǰ���..
							// command = 3 -> ���� ���� ĳ�� ����ũ..��û�ѳ��� �ǰ���..
							// command = 4 -> �߰��� ��� ����ũ
							// command = 5 -> ���� ��� ����ũ
							// command = 6 -> ��Ƽ ������ �Ⱦ� : -1..�̰� ������ ��Ƽ���� ����
};
struct SP_Member_Info : public SIOCPPacket
{
	SP_Member_Info() { m_Header.wID[PACKET_HEADER] = SM_MEMBER_INFO; }
	char	m_Name[12];
	long	m_lUnique;				// ���߿� ü�º�ȭ�� �޴°����� unique�� �ٸ��� ��Ƽ������ ����ũ ã�ƶ�
	BYTE	m_byJob;
	BYTE	m_byHP;					// ü�� % ������/����
//	_Value2	m_HP;					// 0���� max 1���� cur
};
struct SP_Member_Recover : public SIOCPPacket
{
	SP_Member_Recover() { m_Header.wID[PACKET_HEADER] = SM_MEMBER_RECOVER; }
	long	m_lUnique;		// ��ȭ�ϴ³�
	BYTE	m_byHP;			// ü�� %
};

struct SP_Arrow_Going : public SIOCPPacket
{
	SP_Arrow_Going() { m_Header.wID[PACKET_HEADER] = SM_ARROW_GOING; }
	int		m_nShooterIndex;	// ��³� �ε���..����̸� -1;
	long	m_lShooter;			// ��³�
	int		m_nTargetIndex;		// �´³��ε���...���϶� �ε��� ����̸� -1..����ũ�θ� ã�ƺ� 
	DWORD	m_lTarget;			// �´³�
//	DWORD	m_dwStart;		// ������
//	DWORD	m_dwDest;		// ������
};

struct SP_Magic_Going : public SIOCPPacket
{
	SP_Magic_Going() { m_Header.wID[PACKET_HEADER] = SM_MAGIC_GOING; }
	DWORD	m_dwStart;		// ������
	DWORD	m_dwDest;		// ������
	int		m_nIndex;		// effect index
};



//3 - 27 032
struct SP_Horse_Down : public SIOCPPacket
{
	WORD m_wError;			//0�̸� ������ ����. 1�̸� ������Ұ� ����.
							
	long m_lCharacter_Unique;  //�� ���� �������  ��� 

	DWORD m_wDownSpace;		//���� ��� 
	
	int m_iHorse_Index;		//���ε���

	long m_lHorse_Unique;	//���� ����ũ

	SP_Horse_Down(){m_Header.wID[PACKET_HEADER] = SM_HORSE_DOWN;}
};

// 3-24 ����
struct SP_Attack_PC : public SIOCPPacket
{
	SP_Attack_PC() { m_Header.wID[PACKET_HEADER] = SM_ATTACK_PC; }
	long	m_lAttacker;		// ������ ����ũ
	long	m_lDefender;		// ����� ����ũ
	int		m_nMotion;			// ���� ����
	int		m_nDmg;				// ������

	bool	m_bCritical;
};

struct SP_Request_PK : public SIOCPPacket
{
	SP_Request_PK() { m_Header.wID[PACKET_HEADER] = SM_REQUEST_PK; }
	long	m_lUnique;			// ��û�ѳ�
};

struct SP_Reply_PK : public SIOCPPacket
{
	SP_Reply_PK() { m_Header.wID[PACKET_HEADER] = SM_REPLY_PK; }
	long	m_lUnique;			// Ÿ���� �Ǵ� ���� ����ũ..yes�� �ֹ��� ���� ���� no�� ..���� �����߽��ϴ�. 
	bool	m_bReply;			// true : ok	false : no
};


// 3- 28 032
struct SP_Horse_Ride : public SIOCPPacket
{
	SP_Horse_Ride(){ m_Header.wID[PACKET_HEADER] = SM_HORSE_RIDE; }

	WORD m_wError;			//0�̸� ������ ����. 1�̸� ���� ���ٴ� �� 2�̸� �ڱ⸻�� �ƴѵ� Ż���� �ϴ°� 							
	long m_lCharacter_Unique;  //���� Ż���� �ϴ� ���
	int m_iHorseIndex;	//����� Ż��..
	DWORD	m_dwUnique;		// �� ����ũ
};


// 3-29

// 4-29
struct SP_Change_Class : public SIOCPPacket
{
	SP_Change_Class() { m_Header.wID[PACKET_HEADER] = SM_CHANGE_CLASS; }
	BYTE	m_byJob;		// 255�� ����..
};

struct SP_Request_Revive : public SIOCPPacket
{
	SP_Request_Revive() { m_Header.wID[PACKET_HEADER] = SM_REQUEST_REVIVE; }
	long	m_lUnique;		// �� ����ٶ�³�
};

// 5-1
struct SP_NPC_Damage : public SIOCPPacket
{
	SP_NPC_Damage() { m_Header.wID[PACKET_HEADER] = SM_NPC_DAMAGE; }
	int		m_nAttacker;	// ������ �ε��� ����̸� -1 ���߿� ���̸� �ε���
	long	m_lUnique;		// ������ ����ũ
	int		m_nIndex;		// npc index
	DWORD	m_dwUnique;		// npc unique
	int		m_nDmg;			// ���� �������� dmg... -1 : ���� ��ȭ���ΰ�..
	BYTE	m_byStatus;		// ���º�ȭ�̸�.. 0 : ���ݸ� ���Ѱ�	1:���ɸ�  2:����   
	BYTE	m_bySkill;		// ��ų �ѹ�
	bool	m_bCritical;
};


struct SP_User_Revive : public SIOCPPacket
{
	SP_User_Revive() { m_Header.wID[PACKET_HEADER] = SM_USER_REVIVE; }
	long	m_lUnique;		// �� ����ũ�γ� �����
};

struct SP_Horse_Attr : public SIOCPPacket
{
	SP_Horse_Attr(){ m_Header.wID[PACKET_HEADER] = SM_HORSE_ATTR; }
	// 1 : ���� ��� �Ӽ��� ���� �Ҽ� ����.
	WORD	m_wError;			 							
	long	m_lCharacter_Unique;	// ���� ����� �ٲܷ��� �ϴ� ��� 
	int m_iHorseColor;      //�� ���� 
	int m_iSeatColor;		//�� ���� ����
	int m_iHorseIndex;		//�� �� ����
	int m_iSeatIndex;		//���� ����
};

struct SP_Horse_Keepin : public SIOCPPacket
{
	SP_Horse_Keepin(){ m_Header.wID[PACKET_HEADER] = SM_HORSE_KEEPIN; }
	long m_lCharacter_Unique;		// ���� �ñ���� �ϴ� ��� 	
};

struct SP_Horse_Keepout : public SIOCPPacket
{
	SP_Horse_Keepout(){ m_Header.wID[PACKET_HEADER] = SM_HORSE_KEEPOUT; }
	
	DWORD		m_lCharacter_Unique;	// ���� ���� ��� 	
	char		m_HorseName[12];
	DWORD		m_Type;
};

struct SP_Taming_Horse : public SIOCPPacket
{
	SP_Taming_Horse() { m_Header.wID[PACKET_HEADER] = SM_TAMING_HORSE; }
	// 1 : ���� ������ �ִ� ���´�. 
	// 2 : ���� ���°����� �ñ� ���´�. 
	// 3 : ���� ������ ���� �ߴ�. 
	// 4 : ������ ���� ���Ǽ� �ִ� ���� �ʰ� �ߴ�.
	WORD	m_wError;				
	long	m_lCharacter_Unique;	// ���� ��� ��� 

	// Taming ������ ��
	int		m_iIndex;				// ���� �ε��� 
	DWORD	m_dwHorseUnique;		// �� ����ũ
};

struct SP_Treat_Carrot : public SIOCPPacket
{
	SP_Treat_Carrot() { m_Header.wID[PACKET_HEADER] = SM_TREAT_CARROT; }
	WORD m_wError;			// 0�̸� ���� ����� �Ծ���.(50% ����)
							// 1�̸� ���� ������� �Ծ �׾���.
							// 2�̸� ���� ������ �����Ƿ� ���� ���Ǽ� ����.
};

struct SP_NPC_Disappear : public SIOCPPacket
{
	SP_NPC_Disappear(){  m_Header.wID[PACKET_HEADER] = SM_NPC_DISAPPEAR; }
	int		m_iIndex;			// ������� NPC �ε���
	DWORD	m_dwUnique;			// ������� NPC ����ũ
};

struct SP_Use_Skill : public SIOCPPacket
{
	SP_Use_Skill() { m_Header.wID[PACKET_HEADER] = SM_USE_SKILL; }
	BYTE	m_byRes;		// ��ų ��� ���... 0:���� 1:Ÿ���� �߸��ƴ� 2:���ɷ�ġ����  3:��밡�� ���� Ʋ��
	int		m_nSkill;		// ��뽺ų
	long	m_lCaster;		// ������ ����ũ
	int		m_nCasterIndex;	// �����ڰ� ���ϼ��� �����ϱ�..
	long	m_lTarget;		// Ÿ�� ����ũ
	int		m_nTargetIndex;	// Ÿ���� ���ϼ��� �����ϱ�..
};

struct SP_Learn_Skill : public SIOCPPacket
{
	SP_Learn_Skill() { m_Header.wID[PACKET_HEADER] = SM_LEARN_SKILL; }
	int m_iSkillIndex;	//��� ��ų �ε���
};

struct SP_Inven_Change : public SIOCPPacket
{
	SP_Inven_Change() { m_Header.wID[PACKET_HEADER] = SM_INVEN_CHANGE; }
	bool m_bValue;
};

struct SP_Admin_Chat : public SIOCPPacket
{
	SP_Admin_Chat(){ m_Header.wID[PACKET_HEADER] = SM_ADMIN_CHAT; }
	char		m_strMsg[64];		// �޼���
};

struct SP_Slide_NPC : public SIOCPPacket
{
	SP_Slide_NPC(){ m_Header.wID[PACKET_HEADER] = SM_SLIDE_NPC; }
	int			m_nIndex;			// npc index
	DWORD		m_dwUnique;			// npc unique
	DWORD		m_dwPos;			// �з��� ��ġ
	BYTE		m_byDirection;		// �Ĵٺ� ����
};

struct SP_Slide_PC : public SIOCPPacket
{
	SP_Slide_PC(){ m_Header.wID[PACKET_HEADER] = SM_SLIDE_PC; }
};

struct SP_Confirm_Potion : public SIOCPPacket
{
	SP_Confirm_Potion(){ m_Header.wID[PACKET_HEADER] = SM_CONFIRM_POTION; }
	_Value4	  m_Potions;		// 0:hp	1:mp
};

struct SP_Kill_Client : public SIOCPPacket
{
	// �̰� ���� ���� ������ ������ ��Ŷ ����
	SP_Kill_Client() { m_Header.wID[ PACKET_HEADER] = SM_KILL_CLIENT; }
};

struct SP_No_Exist : public SIOCPPacket
{
	SP_No_Exist() { m_Header.wID[PACKET_HEADER] = SM_NO_EXIST; }
	BYTE		m_byKind;		// 1: npc   2: item
	DWORD		m_dwUnique;		// ������� ����ũ npc,item���� ����ũ
	DWORD		m_dwIndex;		// npc,item index , ������� -1
};

struct SP_Chr_Dir : public SIOCPPacket
{
	SP_Chr_Dir() { m_Header.wID[PACKET_HEADER] = SM_CHR_DIR; }
	DWORD		m_dwUnique;		// ĳ���� ����ũ 
	BYTE		m_byDir;		// �Ӹ� ���� 
};

struct SP_NPC_Energy : public SIOCPPacket
{
	SP_NPC_Energy() {  m_Header.wID[PACKET_HEADER] = SM_NPC_ENERGY; }
	int			m_dwIndex;		// 
	DWORD		m_dwUnique;		//
	byte		m_HPper;		//
};

// 7 - 16 npc ���� ��û  * -1�̸� ���� ���� *
struct SP_NPC_Status : public SIOCPPacket
{
	SP_NPC_Status() { m_Header.wID[PACKET_HEADER] = SM_NPC_STATUS; }
	int 		m_dwIndex;  // -1�̸� ������ �����Դϴ�.
	DWORD		m_dwUnique; 
	BYTE		m_byStatus;
};

// 7 - 28 �ƴϸ� ������ ���� 
struct SP_Anima_Status : public SIOCPPacket
{
	SP_Anima_Status() { m_Header.wID[PACKET_HEADER] = SM_ANIMA_STATUS; }
	DWORD		m_dwUnique; //  ���� ����ũ 
	int			m_nIndex;  //  npc �ε��� 
	DWORD		m_dwNPCUnique; //  npc ����ũ
	BYTE		m_byStatus; //  ����
};

// 7 - 28 ���� ���� ���� ������ ��ȭ�� ���ش�.
struct SP_Character_Status : public SIOCPPacket
{
	SP_Character_Status() { m_Header.wID[PACKET_HEADER] = SM_CHARACTER_STATUS; }
	DWORD		m_dwUnique; //  ���� ����ũ 
	BYTE		m_byStatus; //  ����
	WORD		m_Time;		//  �ɸ��� �󸶳� �ɸ����� ���ӽð�
};

// 8 - 1 ����Ʈ ��û ����.
struct SP_Get_Quest : public SIOCPPacket
{
	SP_Get_Quest() { m_Header.wID[PACKET_HEADER] = SM_GET_QUEST; }
	BYTE m_Err;  // 0�̸� ���� 1: ���� ����Ʈ�� ������ �ִ�. 2: ������ �� �´´�.
	BYTE m_QuestClass;
	BYTE m_QuestIndex;	
};

struct SP_Put_Piece : public SIOCPPacket
{
	SP_Put_Piece() { m_Header.wID[PACKET_HEADER] = SM_PUT_PIECE; }
	BYTE	m_byResult;		// 0:���� 1:�����ڸ��� �ٸ��� �־� ���� 2:�� �װ� ����
};

struct SP_Succ_Quest : public SIOCPPacket
{
	SP_Succ_Quest() { m_Header.wID[PACKET_HEADER] = SM_SUCC_QUEST; }
	BYTE	m_Quest;	// ���� ����Ʈ Ŭ����		// 0�̸� ����
	BYTE	m_NextIndex;	// ���� ����Ʈ �ε���		// 0�̸� ����
};

struct SP_Complete_Puzzle : public SIOCPPacket
{
	SP_Complete_Puzzle() { m_Header.wID[PACKET_HEADER] = SM_COMPLETE_PUZZLE; }
	BYTE	m_byResult;		// 0:���� ������ ���沨�� 1:�߸� ���� ����  2:���� �� ������
	DWORD	m_dwItem;		// �õ��ߴ� ������
	BYTE	m_byUpgrade;	// ���׷��̵� ��ġ
};

struct SP_Use_Necta_Tar : public SIOCPPacket
{
	SP_Use_Necta_Tar() { m_Header.wID[PACKET_HEADER] = SM_USE_NECTA_TAR; }
	long	m_lUnique;		// ��� ���� �� ��
	BYTE	m_byKind;		// 0:��Ÿ 1:Ÿ��
	bool	m_bFlag;		// true : ���� ���� false : ��
};

struct SP_Quest_Check : public SIOCPPacket
{
	SP_Quest_Check() { m_Header.wID[PACKET_HEADER] = SM_QUEST_CHECK; }
	BYTE m_QuestClass;		//	���� ���� ^^ 0 �̸� �ó����� 1�̸� event 2  ���� 3 ���� 
	BYTE m_Case;			// �̰� ����Ǽ� 0 npc die 1 pickup item 2 make item 3 meet npc 4�� pickup item drop
	// 5��  make item �� drop
	DWORD m_Index;			// 
};

struct SP_Change_Gage : public SIOCPPacket
{
	SP_Change_Gage(){ m_Header.wID[PACKET_HEADER] = SM_CHANGE_GAGE; }
	BYTE	m_byType;		// 0 :charge 1:extacy
	int		m_byCurrent;	// �ش��������� ���簪
};

struct SP_Mode_Change : public SIOCPPacket
{
	SP_Mode_Change(){ m_Header.wID[PACKET_HEADER] = SM_MODE_CHANGE; }
	long	m_lUnique;
	BYTE	m_byType;		// 0 :charge 1:��� ��� 2:�ȱ� �ٱ� 3:pk ..��Ÿ ���...���� ���� ��� ���� ����
							// 4 : Į�̱�
};

struct SP_QuestItem_Del : public SIOCPPacket
{
	SP_QuestItem_Del(){ m_Header.wID[PACKET_HEADER] = SM_QUESTITEM_DEL; }
	DWORD	m_dwIndex;				// �̰� �ΰ��� ���ܶ�
	long	m_lUnique;
};

struct SP_Register_Board : public SIOCPPacket
{
	SP_Register_Board() { m_Header.wID[PACKET_HEADER] = SM_REGISTER_BOARD; }
	bool		m_bRes;		// true : ��� ����
};

struct SP_Item_Delete : public SIOCPPacket		// ���ﲨ
{
	SP_Item_Delete() { m_Header.wID[PACKET_HEADER] = SM_ITEM_DELETE; }
	DWORD	m_dwIndex;
	long	m_lUnique;
};

struct SP_Error_Code : public SIOCPPacket
{
	SP_Error_Code() { m_Header.wID[PACKET_HEADER] = SM_ERROR_CODE; }
	BYTE		m_byErr;			// ��ӵ� �ڵ�
};
struct SP_Error_Msg : public SIOCPPacket
{
	SP_Error_Msg() { m_Header.wID[PACKET_HEADER] = SM_ERROR_MSG; memset( m_ErrMsg, 64, sizeof(char) ); m_bTop = true; }
	bool		m_bTop;				// true �� ���...false�� ä��â...�⺻�� false
	char		m_ErrMsg[64];		// �ش� ���� ���ڿ� -> 48->64����
};

struct SP_Storage_Change : public SIOCPPacket
{
	SP_Storage_Change() { m_Header.wID[PACKET_HEADER] = SM_STORAGE_CHANGE; }
	DWORD		m_itemidx;			// �ٲ� id	// ���̸� 0:�ְ� 1:���� 
	DWORD		m_itemunique;		// �ٲ� ����ũ // �� 
	BYTE		m_Pos[4];			// 0. ���� tab 1. �̵��� tab 2. �̵��� ��ġ 3. command:0��ǲ1:�ƿ�ǲ2:ü����3:��4:â�����
};

struct SP_Confirm_Arrow : public SIOCPPacket			// ȭ������ ������.
{
	SP_Confirm_Arrow() { m_Header.wID[ PACKET_HEADER ] = SM_CONFIRM_ARROW; }
	BYTE		m_Arrow;		//  ���� ȭ���� �����. 250�� �Ǹ� ȭ������ ��������.
};

struct SP_Operator_Mode : public SIOCPPacket			// ȭ������ ������.
{
	SP_Operator_Mode() { m_Header.wID[ PACKET_HEADER ] = SM_OPERATOR_MODE; }
	BYTE		m_ErrFlag;		//  1 . ��ڰ� �ƴϴ�. 2. etc ���߿� ����
};

struct SP_Character_Info : public SIOCPPacket
{
	SP_Character_Info() { m_Header.wID[ PACKET_HEADER ] = SM_CHARACTER_INFO; }

	int		m_level;			// ĳ���� ��
	int		m_Chr_Str;			// ��
	int		m_Chr_Dex;			// ��
	int		m_Chr_End;			// ����.......��->����
	int		m_Chr_Wis;			// ����
	int		m_Chr_Bra;			// ���.......
	int		m_Chr_Fam;			// ��........
	int		m_Chr_Pos;			// ������.............

	int		m_Chr_Att;			// ���� ���ݷ�
	int		m_Chr_AC;			// ���� ����
	int		m_Chr_Magic_Att;	// ���� ���ݷ�
	int		m_Chr_Magic_AC;		// ���� ����.................
	int		m_Chr_Evade_Rate;	// ȸ����
	char    CName[12];			// �̸� 
};

struct SP_Skill_Upgrade : public SIOCPPacket
{
	SP_Skill_Upgrade() { m_Header.wID[ PACKET_HEADER ] = SM_SKILL_UPGRADE; }
	BYTE	m_bySkill;		// ���׷��̵� �� ��ų
	BYTE	m_byUpgrade;	// �� ��ų�� ���׷��̵� ��ġ
};


struct SP_Confirm_Point : public SIOCPPacket
{
	SP_Confirm_Point() { m_Header.wID[PACKET_HEADER] = SM_CONFIRM_POINT; }
	BYTE	m_byType;		// ��...0:��ų����Ʈ
	int		m_nPoint;		// ���� ������ �ִ� ����Ʈ��
};

struct SP_Delete_Equip : public SIOCPPacket
{
	SP_Delete_Equip() { m_Header.wID[PACKET_HEADER] = SM_DELETE_EQUIP; }
	DWORD	m_Unique;
	DWORD	m_itemIndex;
	DWORD	m_itemUnique;
};

struct SP_Buff_Change : public SIOCPPacket
{
	SP_Buff_Change() { m_Header.wID[PACKET_HEADER] = SM_BUFF_CHANGE; };
	long	m_lUnique;		// ĳ���� ����ũ
	BYTE	m_byBuff;		// �ɸ��ų� ������ ���� ��ȣ
	bool	m_bFlag;
};

// �ְ�ޱ� ��û
struct SP_Try_Itemdeal : public SIOCPPacket
{
	SP_Try_Itemdeal() { m_Header.wID[PACKET_HEADER] = SM_TRY_ITEMDEAL; }
	long	m_send_unique;
	DWORD	m_Itemindex;
	DWORD   m_Itemunique;	
};

// �ְ�ޱ� ����
struct SP_Reply_Itemdeal : public SIOCPPacket
{
	SP_Reply_Itemdeal() { m_Header.wID[PACKET_HEADER] = SM_REPLY_ITEMDEAL; }
	long		m_unique;		//
	DWORD		m_Itemindex;
	DWORD		m_Itemunique;	
	BYTE		m_flag;				// 0�̸� �ִ³� 1.�̸� �޴³� 
	BYTE		m_itempos;			// ������ ��ġ 
	_uItemInfo	iteminfo;			// intem info

};

struct SP_Save_Warpzone : public SIOCPPacket
{
	SP_Save_Warpzone() { m_Header.wID[PACKET_HEADER] = SM_SAVE_WARPZONE; }
	BYTE   m_byErr;  // �ʹ�ȣ
};

struct SP_Item_Upgrade : public SIOCPPacket
{
	SP_Item_Upgrade() { m_Header.wID[PACKET_HEADER] = SM_ITEM_UPGRADE; }
	Item_Info_Save m_UpgradeItem;
};

struct SP_NPC_Use_Magic : public SIOCPPacket
{
	SP_NPC_Use_Magic() { m_Header.wID[PACKET_HEADER] = SM_NPC_USE_MAGIC; }
	int		m_nIndex;		// �ε���
	DWORD	m_dwUnique;		// ����ũ
	BYTE	m_byMotion;		// ����
};

// hsex = 255�� ���̸��� �ߺ� 
// hsex = 254�� ���� 
// ĳ������ũ�� 0�̸� ���°��� �ִ°�. ������ 255�� ���� ����
struct Horse_Info_Load
{
	DWORD	charunique;		// ĳ������ũ
	char	hname[12];		// �̸�
	BYTE	hsex;			// ���� 
	BYTE	hlevel;			// ����
	BYTE	hstress;		// ��Ʈ����
	BYTE	hloyal;			// �漺��
	BYTE	hendurance;		// ������
	BYTE	helegance;		// ��ǰ
	BYTE	hspirit;		// ������
	BYTE	hcolor;			// ���� 
	BYTE	hhelmet;		// �۸� 
	BYTE	hbody;			// �� ����
	DWORD	hexp;			// ����ġ
	DWORD	htime;			// ������ ���� �ð�
};

struct Horse_Info_Save : public Horse_Info_Load
{
	DWORD accunique;  // ��������ũ 
};

struct SP_Horse_Name : public SIOCPPacket
{
	SP_Horse_Name() { m_Header.wID[PACKET_HEADER] = SM_HORSE_NAME; }
	Horse_Info_Save horsedata;
	//BYTE hsex;		// ����  255 : �̸��� ���� 254 �ι꿡 ���ڸ��� ����. 	
	//BYTE hLevel;	// ���� ����
	//char m_HorseName[12];		// �̸� 
	//BYTE m_Type;	//Ÿ�� 0 .Ÿ�� 1. ���°�
};

// 12-3
struct SP_Status_Change : public SIOCPPacket
{
	SP_Status_Change() { m_Header.wID[PACKET_HEADER] = SM_STATUS_CHANGE; }
	int		m_nIndex;	// ������� -1, npc��� �ε���
	long	m_lUnique;	// ����ũ..�ش� ����ũ
	BYTE	m_byStatus;	// �ش� ����...base.h ����

	
};

struct SP_Horse_Status : public SIOCPPacket
{
	SP_Horse_Status() { m_Header.wID[PACKET_HEADER] = SM_HORSE_STATUS; }	
	DWORD	m_CharUnique;
	DWORD	m_Status;	// 0Į��, 1���, 2�ٵ�, 3?
	char	m_Name[12];
};

struct SP_Horse_Away : public SIOCPPacket
{
	SP_Horse_Away() { m_Header.wID[PACKET_HEADER] = SM_HORSE_AWAY; }	
	DWORD	m_CharUnique;
};



// SP messages-for login server----------------------------------------------------------------
struct LCP_Account_Check : public SIOCPPacket
{
	 LCP_Account_Check() {m_Header.wID[PACKET_HEADER] = LCM_ACCOUNT_CHECK;}
	 BYTE byAccID	[ACCOUNT_LENGTH];
	 BYTE byAccPass	[ACCOUNT_LENGTH];
	 DWORD byVer;  // ���� äũ 
};
struct LCP_GroupServ_Select : public SIOCPPacket
{
	 LCP_GroupServ_Select() {m_Header.wID[PACKET_HEADER] = LCM_GROUPSERV_SELECT;}
	 WORD m_wServerNumber;
};
struct LCP_Character_Select : public SIOCPPacket
{
	 LCP_Character_Select() {m_Header.wID[PACKET_HEADER] = LCM_CHRACTER_SELECT;}
	 DWORD m_dwCharUnique;
};
struct LCP_Login_CharacterMake : public SIOCPPacket
{
	 LCP_Login_CharacterMake() {m_Header.wID[PACKET_HEADER] = LCM_LOGIN_CHARACTER_MAKE;}
	 Char_Info_Save m_CharInfo;
};

struct LCP_Login_CharacterDelete : public SIOCPPacket
{
	 LCP_Login_CharacterDelete() {m_Header.wID[PACKET_HEADER] = LCM_LOGIN_CHARACTER_DELETE;}
	 DWORD m_dwCharUnique; // ������� �ϴ� ĳ���� ����ũ 
};

struct LCP_Login_Live_Check : public SIOCPPacket
{
	 LCP_Login_Live_Check() {m_Header.wID[PACKET_HEADER] = LCM_LOGIN_LIVE_CHECK;}
};


//login server -> client
struct LSP_Character_Info : public SIOCPPacket
{
	 LSP_Character_Info() {m_Header.wID[PACKET_HEADER] = LSM_CHARACTER_INFO;}
	 DWORD			m_dwCharInfoCount;
	 Char_Info_Save	m_UserInfo;
};
struct LSP_GroupServ_Info : public SIOCPPacket
{
	 LSP_GroupServ_Info() {m_Header.wID[PACKET_HEADER] = LSM_GROUPSERV_INFO;}

	 WORD m_wServerCount;
	 BYTE m_szServerName[3][40];
};
#define LOGINSERVER_GAMEINFO_ERROR 100
#define LOGINSERVER_GAMEDB_ERROR   101
struct LSP_Login_Error : public SIOCPPacket
{
	 LSP_Login_Error() {m_Header.wID[PACKET_HEADER] = LSM_LOGIN_ERROR;}
	 int  m_iCode;
	 BYTE m_byErrStr[128];
};
struct LSP_Login_OK : public SIOCPPacket
{
	 LSP_Login_OK() {m_Header.wID[PACKET_HEADER] = LSM_LOGIN_OK;}
	 char m_szServer[16];
	 long m_lPort;
	 long m_lUnique;
};
struct LSP_Login_Live_Check : public SIOCPPacket
{
	 LSP_Login_Live_Check() {m_Header.wID[PACKET_HEADER] = LSM_LOGIN_LIVE_CHECK;}
};

struct LSP_Character_Storage : public SIOCPPacket  // Ŭ���� â�� ������ ���� 
{
	 LSP_Character_Storage() {m_Header.wID[PACKET_HEADER] = LSM_CHARACTER_STORAGE;}
	 Char_info_Tab_Storage	m_Storage;
	 DWORD					m_Cash;
};


struct LSP_Horse_Load : public SIOCPPacket  // Ŭ���� �� ������ ���� 
{
	LSP_Horse_Load() {m_Header.wID[PACKET_HEADER] = LSM_HORSE_LOAD;}
	Horse_Info_Load		m_Horse;
	BYTE				m_count;
};

// MP messages for server com
//���� �� ���ӽ� ����ϴ� ��Ŷ
//IOCPnet reserved .. do not use 
struct MP_ServerReady_dont_use : public SIOCPPacket
{
	MP_ServerReady_dont_use() {m_Header.wID[PACKET_HEADER] = SSM_SERVER_READY_DONTUSE;}	
	long		  m_lValue;
};
struct MP_ServerReady_dont_use2 : public SIOCPPacket
{
	MP_ServerReady_dont_use2() {m_Header.wID[PACKET_HEADER] = SSM_SERVER_READY_DONTUSE2;}	
	long		  m_lValue;
};
struct MP_ServerReady_dont_use3 : public SIOCPPacket
{
	MP_ServerReady_dont_use3() {m_Header.wID[PACKET_HEADER] = SSM_SERVER_READY_DONTUSE3;}	
	long		  m_lValue;
};



// info server -> zoneserver �� client �� �α��� ����϶�� �޼���
struct MP_UserIncoming_Standby : public SIOCPPacket
{
	MP_UserIncoming_Standby() {m_Header.wID[PACKET_HEADER] = SSM_USERINCOMING_STANDBY;}	
	Char_Info_Save m_UserInfo;
	long		   m_lAccUnique;
};

#define READY_SUCCESS	0
#define READY_FAIL		1
struct MP_UserIncoming_Ready : public SIOCPPacket
{
	MP_UserIncoming_Ready() {m_Header.wID[PACKET_HEADER] = SSM_USERINCOMING_READY;}	
	long		  m_lUnique;			//charunique
	DWORD         m_lAccUnique;	
	short		  m_nSuccess;

};

struct MP_Private_Chat_Request:public SIOCPPacket
{
	MP_Private_Chat_Request() { m_Header.wID[PACKET_HEADER] = SSM_PRIVATE_CHAT_REQUEST; }
	char		m_to[16];
	char		m_from[16];
	char		m_msg[64];
};

struct MP_Private_Chat_Reply:public SIOCPPacket
{
	MP_Private_Chat_Reply() { m_Header.wID[PACKET_HEADER] = SSM_PRIVATE_CHAT_REPLY; }
	long		m_lUnique;
	char		m_from[16];
	char		m_msg[64];
};

struct MP_Shout_Chat:public SIOCPPacket			// ���� ��ġ���..��� �������� ������ ��Ŷ
{
	MP_Shout_Chat() { m_Header.wID[PACKET_HEADER] = SSM_SHOUT_CHAT; }
	char		m_from[16];
	char		m_msg[64];
};

struct MP_Warp_Request:public SIOCPPacket
{
	MP_Warp_Request() { m_Header.wID[PACKET_HEADER] = SSM_WARP_REQUEST; }
	long		m_lUnique;
	int			m_Map_ID;		// ������ �ʹ�ȣ
	int			m_WarpIndex;	// ������ �ʿ��� ��Ÿ�� ��ǥ�� �ش���� index�� ��ǥ
	int			m_floor;		// ��..�׻� 0
	Char_Info_All m_IncomingCharacter;
};

struct MP_Warp_Reply: public SIOCPPacket
{
	MP_Warp_Reply() { m_Header.wID[PACKET_HEADER] = SSM_WARP_REPLY; }
	long		m_lUnique;
	int			m_Map_ID;		// ������ �ʹ�ȣ
	DWORD		m_Pos;			// ������ �ʿ��� ��Ÿ�� ��ǥ
	int			m_floor;		// ��..�׻� 0
	bool		m_result;
};

//game infoserver interface------------------------------------------------
struct MP_GameInfo_Logout : public SIOCPPacket
{
	MP_GameInfo_Logout() {m_Header.wID[PACKET_HEADER] = SSM_GAMEINFO_LOGOUT;}
	DWORD		   m_AccUnique;
	Char_Info_Save m_CharInfo;
};
struct MP_GameInfo_Save : public SIOCPPacket
{
	MP_GameInfo_Save() {m_Header.wID[PACKET_HEADER] = SSM_GAMEINFO_SAVE;}
	Char_Info_Save m_CharInfo;
};
struct MP_GameInfo_Login : public SIOCPPacket
{
	MP_GameInfo_Login() {m_Header.wID[PACKET_HEADER] = SSM_GAMEINFO_LOGIN;}
	DWORD m_dwCharUnique;
	DWORD m_dwAccUnique;
};
#define GAMEINFO_LOGINFAIL 0
#define GAMEINFO_LOGINOK   1	
struct MP_GameInfo_Error : public SIOCPPacket
{
	MP_GameInfo_Error() {m_Header.wID[PACKET_HEADER] = SSM_GAMEINFO_ERROR;}
	BYTE m_byErrorType;
	DWORD m_dwAccUnique;
	DWORD m_dwCharUnique;
};

struct MP_GameInfo_ProduceItem : public SIOCPPacket
{
	MP_GameInfo_ProduceItem() {m_Header.wID[PACKET_HEADER] = SSM_GAMEINFO_PRODUCE_ITEM;}
	DWORD			m_dwRequestFrom;	//request from who ? (server number)
};
struct MP_GameInfo_ProduceItem_Result : public SIOCPPacket
{
	MP_GameInfo_ProduceItem_Result() {m_Header.wID[PACKET_HEADER] = SSM_GAMEINFO_PRODUCE_ITEM_RESULT;}
	DWORD			m_dwStart;	
};

struct MP_GameInfo_Make_Character : public SIOCPPacket
{
		MP_GameInfo_Make_Character() {m_Header.wID[PACKET_HEADER] = SSM_GAMEINFO_MAKE_CHARACTER;}
		DWORD m_AccUnique;
		Char_Info_Save m_Info;
};

struct MP_GameInfo_Delete_Character : public SIOCPPacket
{
		MP_GameInfo_Delete_Character() {m_Header.wID[PACKET_HEADER] = SSM_GAMEINFO_DELETE_CHARACTER;}
		DWORD m_dwCharUnique;
		DWORD m_dwAccUnique;
};


//account InfoServer interface****************************************************
#define LOGIN_TYPE_GAME 0
#define LOGIN_TYPE_WEB  1
struct MP_AccountInfo_Check : public SIOCPPacket
{
	MP_AccountInfo_Check() {m_Header.wID[PACKET_HEADER] = SSM_ACCOUNTINFO_CHECK;}

	DWORD m_dwLoginProcessID;
	BYTE  m_byAccID		[ACCOUNT_LENGTH];
	BYTE  m_byAccPass	[ACCOUNT_LENGTH];
	BYTE  m_byIP		[25];
	BYTE  m_byLoginType;
};

//error type
#define INFO_PROCESS_ERROR			0
#define INFO_INVALID_ID				1
#define INFO_INVALID_PASS			2
#define INFO_INVALID_OCCUPIED		3
#define INFO_INVALID_PROCESSOK		4
#define INFO_INVALID_GROUPCOUNTOVER	5
#define INFO_INVALID_SETTLEOVER		6
#define INFO_ACCESS_FAIL    		7

#define INFO_ERROR_NOVALUE			0
#define INFO_ERROR_SLOTFAIL			1
#define INFO_ERROR_DBDEAD			2
struct MP_AccountInfo_Error : public SIOCPPacket
{
	MP_AccountInfo_Error() {m_Header.wID[PACKET_HEADER] = SSM_ACCOUNTINFO_ERROR;}
	DWORD m_dwLoginProcessID;
	DWORD m_dwErrorValue;
	BYTE  m_byErrorType;

};
struct MP_AccountInfo_Logout : public SIOCPPacket
{
	MP_AccountInfo_Logout() {m_Header.wID[PACKET_HEADER] = SSM_ACCOUNTINFO_LOGOUT;}
	DWORD m_dwAccUnique;
};



//account DB Middle ware interface****************************************************
struct MP_AccountDB_Check : public SIOCPPacket
{
	MP_AccountDB_Check() {m_Header.wID[PACKET_HEADER] = SSM_ACCOUNTDB_CHECK;}
	DWORD	m_dwLoginProcessID;
	BYTE	byAccID		[ACCOUNT_LENGTH];
	BYTE	byAccPass	[ACCOUNT_LENGTH];
};


//return errors
#define CHECK_INVALID_OK	0
#define CHECK_INVALID_ID	1
#define CHECK_INVALID_PASS	2
struct MP_AccountDB_Check_Result : public SIOCPPacket
{
	MP_AccountDB_Check_Result() {m_Header.wID[PACKET_HEADER] = SSM_ACCOUNTDB_CHECK_RESULT;}
	
	BYTE	m_byResult;				//type result (check above)
	DWORD	m_dwLoginProcessID;
	DWORD	m_dwAccountUnique;				//the value of the result
};


struct MP_AccountDB_Settle_Request : public SIOCPPacket
{
	MP_AccountDB_Settle_Request() {m_Header.wID[PACKET_HEADER] = SSM_ACCOUNTDB_SETTLE_REQUEST;}
	DWORD   m_dwRequest;			//request number (AccUnique)
	DWORD   m_dwAccUnique;
	char	m_szIP[IPV6_LENGTH];
};

#define SETTLE_PRIVATE 0
#define SETTLE_GROUP   1
struct MP_AccountDB_Settle_Result : public SIOCPPacket
{
	MP_AccountDB_Settle_Result() {m_Header.wID[PACKET_HEADER] = SSM_ACCOUNTDB_SETTLE_RESULT;}
	DWORD	m_dwRequest;	
	//_SettleInfo m_SettleInfo[2];	//private, group
};

//game db middle ware interface ***********************************************************
struct MP_Character_Info : public SIOCPPacket
{
	MP_Character_Info() {m_Header.wID[PACKET_HEADER] = SSM_GAMEDB_CHARACTER_INFO;}
	DWORD   m_dwAccUnique;
	DWORD   m_dwCharUnique;
	BOOL    m_bSingOption;
};
struct MP_Character_Single_Info : public SIOCPPacket
{
	MP_Character_Single_Info() {m_Header.wID[PACKET_HEADER] = SSM_GAMEDB_CHARACTER_SINGLEINFO;}
	DWORD   m_dwAccUnique;
	DWORD   m_dwCharacterCount;
	Char_Info_Save m_UserInfo;
};
struct MP_Character_InfoList : public SIOCPPacket
{
	MP_Character_InfoList() {m_Header.wID[PACKET_HEADER] = SSM_GAMEDB_CHARACTER_INFOLIST;}
	DWORD			m_dwAccUnique;
	Char_Info_Save	m_UserInfo;
};
struct MP_Character_Save : public SIOCPPacket
{
	MP_Character_Save() {m_Header.wID[PACKET_HEADER] = SSM_GAMEDB_CHARACTER_SAVE;}
	DWORD			m_dwRequestNumber;
	DWORD			m_dwAccUnique;
	Char_Info_Save	m_UserInfo;
};


struct MP_GameDB_Error : public SIOCPPacket
{
	MP_GameDB_Error() {m_Header.wID[PACKET_HEADER] = SSM_GAMEDB_ERROR;}
	DWORD			m_dwRequestNumber;
	USHORT			m_nErrorCode;
};
struct MP_GameDB_CharacterMake : public SIOCPPacket
{
	MP_GameDB_CharacterMake() {m_Header.wID[PACKET_HEADER] = SSM_GAMEDB_CHARACTER_MAKE;}
	Char_Info_Save  m_Info;
	DWORD			m_dwAccountNumber;
};

struct MP_GameDB_CharacterDelete : public SIOCPPacket
{
	MP_GameDB_CharacterDelete() {m_Header.wID[PACKET_HEADER] = SSM_GAMEDB_CHARACTER_DELETE;}
	DWORD   m_dwAccUnique;
	DWORD   m_dwCharUnique;
};

struct CP_Save_Warpzone : public SIOCPPacket
{
	CP_Save_Warpzone() { m_Header.wID[PACKET_HEADER] = CM_SAVE_WARPZONE; }
	BYTE   m_byWarpID;  // ���� ������
};

struct CP_Item_Upgrade : public SIOCPPacket
{
	CP_Item_Upgrade() { m_Header.wID[PACKET_HEADER] = CM_ITEM_UPGRADE; }
	_uItemType	m_ItemIndex;
	DWORD		m_ItemUnique;
	BYTE		m_JewelIndex[5];
	DWORD		m_JewelUnique[5];	
};

struct SP_Teleporter : public SIOCPPacket
{
	SP_Teleporter() { m_Header.wID[PACKET_HEADER] = SM_TELEPORTER; }
	BYTE	m_byResult;		// 0 : ����		1 : ������ ����		2 : ��Ÿ ����(���������̳� �׷��� )		3 : ��ü�� �� ����
};

*/



#endif
