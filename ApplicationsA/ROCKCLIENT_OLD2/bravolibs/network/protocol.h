//_________________________________________________________________________________
//
//	protocol.H
//
//	프로토콜들 정의
//
//
//_________________________________________________________________________________
//
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_


///------------------------------------------------------------------------------
///	sooree network : GNI 네트웍 소스 정리 - 이 부분은 떼내기 힘들어서 일단 그냥 두고 갑니다. 
///------------------------------------------------------------------------------
/*
#define SM_MOVE_BROADCAST		257		// 이동 메세지 헤더 (S->C)
#define SM_ATTACK_NPC			294		// 사람이 npc공격 방송
#define SM_NPC_RESPAWN			297			// 10-15
#define SM_NPC_DIE				298
#define SM_USER_DIE				312		// 유저 죽음
#define SM_ARROW_GOING			341		// 화살 날라가는거
#define SM_ATTACK_PC			345		// 유저 공격
#define SM_NPC_DISAPPEAR		375		// 유저가 나감에 속해 있는 NPC 사라짐
#define SM_USE_SKILL			377		// 사용 응답
#define SM_SLIDE_NPC			383		// stun 류 기술 맞으면 밀려나기

#define SM_CHANT_MODE				432		// 영창 콤보 모드 
#define CM_CHANT_MODE				432		// 영창 콤보 모드 

#define SM_ATTACK_DELAY				433		// 공격 대기 모드 
#define CM_ATTACK_DELAY				433		// 공격 대기 모드 
*/
typedef enum NEventType
{
	SM_MOVE_BROADCAST = 100,		// 이동 메세지 헤더 (S->C)
	SM_MOVE_ATTACK,					// 이동 공격 
	CM_MOVE_ATTACK,					// 서버에 방송하기 전의 이동 공격
	SM_ATTACK_NPC,					// 공격 모드 
	SM_ATTACK_DELAY,				// 공격 대기 모드 
	SM_USE_SKILL,					// 사용 응답
	SM_CHANT_READY,					// 영창 준비 
	SM_CHANT_SPELLING,				// 영창 콤보 단계 
	SM_SIMSIM,						// 심심이벤트(인사,노는행동)
	SM_UNDERATTACK,					// 공격당했음
	SM_SLIDE_NPC,					// stun 류 기술 맞으면 밀려나기
	SM_NPC_RESPAWN,					// 10-15
	SM_STATE_DIE,					// 유저 죽음
	SM_RESTSKILL_SITDOWN,			// 휴식 스킬 (앉기)
	SM_RESTSKILL_KEEPING,			// 휴식 스킬 (유지)
	SM_RESTSKILL_STANDUP,			// 휴식 스킬 (서기)
	SM_ITEM_DROPING,				// 아이템 떨어뜨리기 
	SM_ARROW_GOING,					// 화살 날라가는거
	SM_BOW_ATTACK,					// 화살 공격
	SM_BOW_READY,					// 화살 준비 동작
	SM_BOW_WAIT,					// 화살 대기 동작
	SM_SELF_DIE,					// 자살
	SM_MOVE_TURN,					// 제자리 돌기								// 카메라 워크관련 By wxywxy
	SM_ANI_QUEUE_PROCESS,			// 에니메이션 순차 처리하기					// 카메라 워크관련 By wxywxy
	SM_ACT,							// 행동 
	SM_ACT_START,					// 단발 행동
	SM_ACT_KEEPING,					// 지속 행동
	//.......................................................................................................
	// 개인 상점 By wxywxy
	//.......................................................................................................
	SM_SELLSKILL_SITDOWN,			// 개인 팔기 상점 앉기  status
	SM_SELLSKILL_KEEPING,			// 개인 팔기 상점 유지  status
	SM_SELLSKILL_STANDUP,			// 개인 팔기 상점 서기  status
	//.......................................................................................................
	SM_ANI_SQUENCE,					// 시퀀스 에니메이션 ( 동작 완료후 다음 시퀀스로 이동 ) By wxywxy
	//.......................................................................................................
	//	라이딩 By simwoosung	
	//.......................................................................................................
	SM_RIDING,

	///-- By simwoosung 일단 이부분이 필요성을 느껴서 만들어봄... 아직 전부는 미구현중
	SM_MOVE_SKILL,					// 이동 후 스킬
	CM_MOVE_SKILL,					// 서버에 방송하기 전의 이동 스킬 	
	
	SM_EVENT_MAXCOUNT
};

#define SIMSIM_TIME			30000.0f			// 심심해서 하는 행동(5초)


#define MAX_EQUIPED		10
#define MAX_INVENTORY1	145

struct Char_Info		// 보이는 정보들	
{
	/// sooree network
	///--------------------------------------------------------------------------
	///	우리 거에 맞춘 새로운 데이터 값들... 앞으로는 이걸로 코드를 재구성 
	///	struct _SRpdOtherPC에서 Cell 좌표만 뺀 구조 
	///-----------------------------------------------------------------------
	DWORD				thePCID;
	char				thePCName[RP_MAX_PC_NAME];
	BYTE				theLevel;			
	BYTE				theCitizen;									// RAIN, MILLENA
	BYTE				theGender;									// MALE, FEMALE
	DWORD				theFace;
	DWORD				theHair;
	DWORD				theHP;
	DWORD				theSMP;										// Citizen에 따라서 SP 또는 MP로 처리 
	DWORD				theMaxHP;									// 기본 값 + 유저 설정값 + Item + Skill
	DWORD				theMaxSMP;
	DWORD				theRealHP;
	BYTE				theWasteGage;
	SRpdCharWearItem	theWearItem[RP_MAX_EQUIP];					// 장신구1, 2 제외
	WORD				theKeepupSkill[RP_MAX_KEEPUP_SKILL];		// 지속성 스킬 코드 		
	BYTE				theMainClass;								// 메인 클래스	

	
	int					theFame;									// 캐릭터의 명성...
	
	///--				중국 관련 PK작업
	DWORD				thePKPoint;

	///--------------------------------------------------------
	///	이건 전에 쓰이던 데이터들...일단 에러땜시롱 걍 나둔다. 
	///--------------------------------------------------------
	char	ID[12];			// id

	int		race;			// 종족		0 : 유니언, 1 : 다른1, 2 : 다른2
	int		sex;			// 성별		0 : 남, 1 : 여
	int		job;			// 직업		0 : 전직전, 1 : 전사, 2 : 궁수, 3 : 매지션, 4 : 페어리....
	BYTE	hair;			// 머리모양
	BYTE	color;			// 머리색깔
	BYTE	face;			// 얼굴모양 

	DWORD		head;
	DWORD		body;
	DWORD		r_hand;
	DWORD		l_hand;
	DWORD		arm;
	DWORD		foot;

	int		effect[5];			// 걸려 있는 효과
	
	//by simwoosung  파티정보
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
#define ITEM_KEY_COUNT  3						//갯수 (먹을꺼등만 사용함)
#define ITEM_KEY_SEX    3						//성별
#define ITEM_KEY_RACE   2						//종족
#define ITEM_KEY_GRAV	2						//중요도
#define ITEM_KEY_CLASS	1						//무기종류 (칼,방패등)
#define ITEM_KEY_TYPE	1						//무기종류 (긴칼,짧은칼등)
#define ITEM_KEY_NUMBER	0						//아이템 번호 
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
///	이하 원래 GNI 네트웍 처리 관련 Define, Structure 들. 
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
// CM_어쩌구는 클라이언트에서 서버로 오는 메세지 헤더
// SM_어쩌구는 서버에서 클라이언트로 가는 메세지 헤더

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

#define SSM_PRIVATE_CHAT_REQUEST	12			// 서버간 귓말 패킷
#define SSM_PRIVATE_CHAT_REPLY		13
#define SSM_SHOUT_CHAT				14

#define SSM_WARP_REQUEST			15			// warp 요청
#define SSM_WARP_REPLY				16			// warp 응답

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
#define LCM_LOGIN_CHARACTER_DELETE  132  // 클에서 지운다고 날려줌 
#define LCM_LOGIN_LIVE_CHECK		136 // 클 살았다고 응답
//to client for login interface
#define LSM_LOGIN_ERROR 			128
#define LSM_GROUPSERV_INFO			129
#define LSM_CHARACTER_INFO			130
#define LSM_LOGIN_OK				131
#define LSM_LOGIN_CHARACTER_DELETE  133	// 서버에서 성공유무를 날려줌
#define LSM_LOGIN_VER_ERROR			135	// 버젼이 달라서 접속이 안된다.
#define LSM_LOGIN_LIVE_CHECK		136 // 클 살아 있는지 확인
#define LSM_CHARACTER_STORAGE		137	// 창고
#define LSM_HORSE_LOAD				138 // 말정보를 클에게 
#define MAX_HORSE_PER				11	// max horse per


//msg header for game play --------------------------------------------------------
#define CM_MOVE_BROADCAST			256		// 이동 메세지 헤더 (C->S)
#define SM_MOVE_BROADCAST			257		// 이동 메세지 헤더 (S->C)

#define SM_CONFIRM_POINT			258		// 모든 포인트 관련..레벨업 스탯 포인트는 기냥 지금과 같이..

// 257-259 남음

#define SM_APPEAR	  260
#define CM_DISCONNECT 261		// CM_DISAPPEAR 3 ??? 뭐징~
#define SM_DISAPPEAR  262		// 클라이언트에서 disconnect를 받고 서버에서는 disappear를 보낸다...

#define CM_LOGINZONE  263
#define SM_LOGINZONE	264

#define SM_CHECK_CONNECTION		265		// 접속 확인 메세지...6-26일 추가..

#define CM_NORMAL_CHAT	266		// 일반 채팅
#define SM_NORMAL_CHAT	266
#define CM_PRIVATE_CHAT	267		// 귓말
#define SM_PRIVATE_CHAT	267
#define CM_SHOUT_CHAT	268		// 전체 외치기
#define SM_SHOUT_CHAT	268
#define CM_PARTY_CHAT	269		// 파티 대화
#define SM_PARTY_CHAT	269
#define CM_GUILD_CHAT	270		// 길드 대화
#define SM_GUILD_CHAT	270
#define CM_TRADE		271		// 유저간 거래
#define SM_TRADE		271

#define CM_REQUIRE_INDEX	272		// 목록확인
#define SM_REQUIRE_INDEX	272

#define SM_NPC_USE_MAGIC	275	// 10-18 npc마법사용

#define	SM_NPC_APPEAR	276		// 8-8
#define SM_NPC_MOVE		277

#define SM_WARP			278		// 8-23..
#define CM_WARP			279

#define CM_TAKEOFF_ITEM	280		// 착용한거 벗기
#define SM_TAKEOFF_ITEM 281

#define CM_CHANGE_ITEM	282		// 착용한거 바꾸기 또는 착용하기
#define SM_CHANGE_ITEM	283

#define CM_DROP_ITEM	284		// 아이템 버리기
#define SM_DROP_ITEM	285
#define SM_ITEM_APPEAR	286		// 이동이나 버렸을시 아이템 보이기

#define CM_PICKUP_ITEM	287		// 아이템 줍기
#define SM_PICKUP_ITEM	288
#define SM_ITEM_DISAPPEAR	289	// 아이템 사라짐..줏음...

#define CM_BUY			290			// 상인클릭시
#define SM_BUY_OK		291			// 사기 성공
#define SM_BUY_ERROR	292			// 사기 실패...이번엔 인벤에 빈칸이 없을경우

#define CM_ATTACK_NPC		293		// 사람이 npc공격
#define SM_ATTACK_NPC		294		// 사람이 npc공격 방송

#define CM_WEATHER		295
#define SM_WEATHER		296

#define SM_NPC_RESPAWN	297			// 10-15

#define SM_NPC_DIE		298

#define CM_SELL			299
#define SM_SELL_OK		300
#define SM_SELL_ERROR	301

#define SM_NPC_ATTACK_PC	302		// npc가 유저 공격
#define CM_WARP_READY		303

#define SM_TIME_CHANGE		304		// 시간 변화

#define SM_ITEM_CHANGE		305
#define SM_ITEM_MOVE		306
#define SM_ITEM_EXTINCTION	307		// 시간 소멸 되는 아이템

#define CM_ATTACK_ITEM		308		// 아이템 공격
#define SM_ATTACK_ITEM		309		// 위에꺼 방송

#define	SM_NPC_SPEECH		310

#define	SM_USER_RECOVER		311		// 유저 회복............이넘두 현재 hp,mp로 바껴
#define SM_USER_DIE			312		// 유저 죽음
#define SM_LEVEL_UP			313		// 레벨업

#define CM_CHANGE_STAT		314		// 유저 스탯 변화
#define SM_CHANGE_STAT		315

#define	SM_CHANGE_EXP		316		// 경험치 증감..머 죽었을때는 -겠지...이넘두 바껴...현재 경험치로

#define SM_CONFIRM_CASH		317		// 현재 니가 가진 돈이얌
#define SM_CONFIRM_STAT		318

// 2-19 물약
#define	CM_USE_POTION		319
#define SM_USE_POTION		320

// 3-4파괴
#define SM_DESTROY_ITEM		321

// 3-8 합체
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

// 3-19.....10-13수정
#define CM_PARTY			332
#define SM_PARTY			332
#define SM_MEMBER_INFO		333		// 파티원 정보..나중엔 길드전 들어가면 길드 메세지까지도...
#define SM_MEMBER_RECOVER	334

// 12-3 상태 변화
#define SM_STATUS_CHANGE	335


// 3-21
#define SM_ARROW_GOING		341		// 화살 날라가는거
#define SM_MAGIC_GOING		342		// 마법 날라가는거

// 3-24
#define CM_ATTACK_PC		344		// 유저 공격
#define SM_ATTACK_PC		345		// 유저 공격

// 이거 다 버리자..
#define CM_REQUEST_PK		346		// 공격 하겠오
#define SM_REQUEST_PK		347		// 요청이야
#define	CM_REPLY_PK			348		// 요청받은넘이 하는 응답이야
#define SM_REPLY_PK			349		// 요청한넘한테 날려줄 꺼야

// 3-27	032
#define CM_HORSE_DOWN		350		// 말에서 내린다고 함 요청
#define SM_HORSE_DOWN		351		// 말에서 내린다고 함 응답
#define CM_HORSE_RIDE		354		// 말을 탄다고 함 요청
#define SM_HORSE_RIDE		355		// 말을 탄다고 함 응답




// 4-29
#define CM_CHANGE_CLASS		356		// 전직할래
#define SM_CHANGE_CLASS		357		// 그래..
// 5-1
#define SM_NPC_DAMAGE		358		// npc가 공격 받거나 상태 변화일때

// 4-2
#define SM_REQUEST_REVIVE	360		// 너 부활할래?
#define CM_REPLY_REVIVE		361		// 물어본거 대답
#define	SM_USER_REVIVE		362		// 누구 부활했다..

//363 364 empty

#define CM_HORSE_ATTR	    365     // 말의 속성을 바꾼다고 요청 
#define SM_HORSE_ATTR		366		// 말의 속성을 바꾼다고 응답 

#define CM_HORSE_KEEPIN		367		// 말을 마굿간에 맡기다. 요청 
#define SM_HORSE_KEEPIN		368		// 말을 마굿간에 맡기다. 응답

#define CM_HORSE_KEEPOUT	369		// 말을 마굿간에 뺀다 . 요청 
#define SM_HORSE_KEEPOUT	370		// 말을 마굿간에 뺀다 . 응답

// 4-8
#define	CM_TAMING_HORSE		371		// 야생마를 꼬시겠다
#define SM_TAMING_HORSE		372		// 야생마 꼬시기시도 결과
#define CM_TREAT_CARROT		373		// 야생마에게 당근 먹이기
#define SM_TREAT_CARROT		374		// 당근 먹인 결과..독이면 말 죽이고..

// 4 - 22
#define SM_NPC_DISAPPEAR	375		// 유저가 나감에 속해 있는 NPC 사라짐

#define CM_USE_SKILL		376		// 스킬 사용
#define SM_USE_SKILL		377		// 사용 응답

#define SM_LEARN_SKILL		378		// 스킬 배움

#define CM_INVEN_CHANGE		379		// 아이템을 인밴 위치에서 다른 위치로 바꿈 요청 
#define SM_INVEN_CHANGE		380		// 아이템을 인밴 위치에서 다른 위치로 바꿈 응답 

// 6-9
#define CM_ADMIN_CHAT		381		// 클라이언트에서 운영자가 날리는 채팅 메세지
#define SM_ADMIN_CHAT		382		// 운영자 메세지..클라이언트에서는 화면 가운데 뿌려라

// 6-10
#define SM_SLIDE_NPC		383		// stun 류 기술 맞으면 밀려나기
#define SM_SLIDE_PC			384		// 나중에 사람이 밀려날수도 있으니까

// 6-11
#define SM_CONFIRM_POTION	385		// 물약 갯수 확인

// 6-12
#define CM_NOW_TIME			386		// 클라이언트에서 1분에 한번씩 쏘는 패킷..1분마다 시간쏜다
#define CM_KILL_CLIENT		387		// 운영자가 쏘는거..해당 유저 접속 끊어라
#define SM_KILL_CLIENT		388		// 이거 받으면 받는 클라이언트 접속 끊어라

// 6-14
#define CM_CHECK_CONNECTION		389	// 접속 확인 메세지...6-26일 추가..

// 6-15
#define SM_NO_EXIST			390
#define CM_REQUIRE_DATA		391		// 없다

// 7-14 
#define CM_CHR_DIR			392		// 머리 돌아가는 방송 요청
#define SM_CHR_DIR			393		// 머리 돌아가는 방송 

// 7- 16
#define CM_NPC_ENERGY			394		// NPC현 HP% 요청 
#define SM_NPC_ENERGY			395		// NPC현 HP% 응답 

// 패킷을 아끼기위한 공통 #define 처리
#define CM_NPC_STATUS			396		// npc현 상태 요청
#define SM_NPC_STATUS			396		// npc현 상태 응답

//
#define CM_ANIMA_STATUS			397		// 아니마 아이템 요청(필요 없당)
#define SM_ANIMA_STATUS			397		// 아니마 아이템을 쏴줌 

#define CM_CHARACTER_STATUS			398		//   상태 
#define SM_CHARACTER_STATUS			398		//   상태

#define CM_GET_QUEST				399		//	 퀘스트 요청
#define SM_GET_QUEST				399		//   퀘스트 얻음 

#define CM_PUT_PIECE				400		// 퍼즐 조각 끼우기
#define SM_PUT_PIECE				400

#define CM_SUCC_QUEST				401		//	 퀘스트 성공
#define SM_SUCC_QUEST				401		//   퀘스트 성공 

#define CM_COMPLETE_PUZZLE			402		// 퍼즐 완료 아이템으로
#define SM_COMPLETE_PUZZLE			402

#define SM_USE_NECTA_TAR			403

#define CM_QUESTITEM_DEL			404      // 성공시 삭제 
#define SM_QUESTITEM_DEL			404

#define CM_QUEST_CHECK				405
#define SM_QUEST_CHECK				405 

#define SM_CHANGE_GAGE				406
#define CM_MODE_CHANGE				407
#define SM_MODE_CHANGE				407

#define CM_REGISTER_BOARD			408		// 인벤에서 책 우클릭시 쏘는거
#define SM_REGISTER_BOARD			408		// 등록 성공 여부

#define SM_ITEM_DELETE				409		// 지울꺼

#define SM_BUFF_CHANGE				410		// 버프

#define CM_STORAGE_CHANGE			413		// 창고인벤정리
#define SM_STORAGE_CHANGE			413

#define SM_ERROR_CODE				415		// 약속된 에러 코드로 가는 에러 메세지
#define SM_ERROR_MSG				416		// 문자열이 담긴 에러 메세지

#define SM_CONFIRM_ARROW			417     // 9-23 쏜 화살의 수를 날린다.

#define	CM_OPERATOR_MODE			418		// 9-23 운영자모드
#define	SM_OPERATOR_MODE			418		// 9-23 운영자모드

#define SM_CHARACTER_INFO			419		// 캐릭터 정보

#define CM_OPERATOR_CHAT			420		// 운영자에게
#define SM_OPERATOR_CHAT			420		// 운영자에게

#define CM_SKILL_UPGRADE			421		// 스킬 업그레이드
#define SM_SKILL_UPGRADE			421

#define SM_DELETE_EQUIP				422		// 어떤이유에서 없어짐 

#define CM_TRY_ITEMDEAL				423		// item 주고 받기 신청
#define SM_TRY_ITEMDEAL				423
#define CM_REPLY_ITEMDEAL			424		// item 주고 받기 응답.	
#define	SM_REPLY_ITEMDEAL			424

#define CM_SAVE_WARPZONE			425		// 
#define SM_SAVE_WARPZONE			425

#define CM_ITEM_UPGRADE				426		// 아이템 업그레이드 패킷
#define SM_ITEM_UPGRADE				426		// 아이템 업그레이드 패킷

#define CM_CONNECTED				510
#define CM_DISCONNECTED				511

#define CM_TELEPORTER				273		// 텔레포트 npc눌러서 할 텔레포트
#define SM_TELEPORTER				273

#define CM_HORSE_NAME				427
#define SM_HORSE_NAME				427

#define CM_HORSE_STATUS				428
#define SM_HORSE_STATUS				428

#define CM_HORSE_AWAY				429
#define SM_HORSE_AWAY				429

#define CM_NORMAL_LOGOUT			430		// 정상종료

#define CM_USER_INFO				431 // 정상종료



#define SM_CHANT_MODE				432		// 영창 콤보 모드 
#define CM_CHANT_MODE				432		// 영창 콤보 모드 

#define SM_ATTACK_DELAY				433		// 공격 대기 모드 
#define CM_ATTACK_DELAY				433		// 공격 대기 모드 




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

// db에서 받은게 UINT 맨위 한바이트는 냄겨놓자.. 종족[2] 성별[1] 직업[0] 번째 바이트이다
#define CHAR_SHIFT_KEY_HAIR	24		// 머리모양 머리색
#define CHAR_SHIFT_KEY_RACE	16
#define CHAR_SHIFT_KEY_SEX	8
#define CHAR_SHIFT_KEY_JOB	0
#define CHAR_KEY		255
#define CHAR_KEY_HAIR	15
#define CHAR_KEY_COLOR	240

struct Char_Info		// 보이는 정보들
{
	char	ID[CHARACTER_LENGTH];		// id

	int		race;			// 종족		0 : 유니언, 1 : 다른1, 2 : 다른2
	int		sex;			// 성별		0 : 남, 1 : 여
	int		job;			// 직업		0 : 전직전, 1 : 전사, 2 : 궁수, 3 : 매지션, 4 : 페어리....
	BYTE	hair;			// 머리모양
	BYTE	color;			// 머리색깔
	BYTE	face;			// 얼굴모양 

	DWORD		head;
	DWORD		body;
	DWORD		r_hand;
	DWORD		l_hand;
	DWORD		arm;
	DWORD		foot;

	int		effect[CHARACTER_MAX_EFFECT];			// 걸려 있는 효과
};
struct Char_Info_All: public Char_Info		// 안보이는 정보들까지 포함한 ...
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
	BYTE		m_byHorseInfo[4];		// 0:미사용, 1:몸색깔, 2:보호대, 3:안장
};

#define BYTE_HIGH4BIT	240
#define BYTE_LOW4BIT	15
#define ITEM_KEY_COUNT  3						//갯수 (먹을꺼등만 사용함)
#define ITEM_KEY_SEX    3						//성별
#define ITEM_KEY_RACE   2						//종족
#define ITEM_KEY_GRAV	2						//중요도
#define ITEM_KEY_CLASS	1						//무기종류 (칼,방패등)
#define ITEM_KEY_TYPE	1						//무기종류 (긴칼,짧은칼등)
#define ITEM_KEY_NUMBER	0						//아이템 번호 
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
	SUCCHANGE,			// 성공율 
	EVDCHANGE,			// 회피율
	PALCHANGE,			// physical attack level change
	PDLCHANGE,			// physical defense level change
	MALCHANGE,			// magical attack level change
	MDLCHANGE,			// magical defense level change
	SUCLCHANGE,			// 성공율 레벨별 
	EVDLCHANGE,			// 회피율 레벨별

	HPCHANGE,			// 체력가감
	MPCHANGE,			// 마력가감

	HPLCHANGE,			// 체력 레벨당 가감
	MPLCHANGE,			// 마력 레벨당 가감

	STRCHANGE,			// 힘
	DEXCHANGE,			// 민
	CONCHANGE,			// 콘
	WISCHANGE,			// 마

	NoneEffect = 255
};

union Effect_Value
{
	short	m_effectvalue;
	BYTE	m_byeffectvalue[2];
};

struct Item_Effect
{
	ItemEffect		m_effect_id;		// 어떤 효과...255는 효과 없음이다
	Effect_Value	m_effect;			// 효과 수치 [최소][최대]
};

// 아이템 구조체
struct Item_Index
{
	_uItemType		m_item_id;
	//Item_Effect		m_item_effect[3];	// 능력치...
	//int				m_item_delay;		// 딜레이
	int				m_item_range;		// 사정거리
	int				m_item_price;		// 기본 가격
	//int				m_item_maxup;		// 최대 강화 수치
	int				m_item_level;		// 착용 제한 레벨
	//int				m_item_str;			// 착용제한 힘...255는 제한 없다
	//int				m_item_dex;
	//int				m_item_con;
	//int				m_item_wis;
	//BYTE			m_item_set;			// 세트번호
	//BYTE			m_item_special;		// 특수 효과

	// 5 7 032 item 추가
	int				m_UseJob;			// 사용가능직업
	int				m_attr;				// 속성
	int				m_live;				// 수명 
	int				m_Attack_Ph;		// 물리공격력
	int				m_Defense_Ph;		// 물리방어력 
	int				m_Evade_Per;		// 회피율
	int				m_Attack_speed;		// 공격시피드
	int				m_Attack_Magic;		// 매직공격력
	int				m_Defense_Magic;	// 매직방어력

										// 사정거리 
										// 제한능력 레벨 

	int				m_Limit_Capability; // 제한능력 
	int				m_Limit_Capa_Value; // 제한능력치
	int				m_Recover;			// 회복
	int				m_Recover_Value;	// 회복량
	int				m_Attack_Ph_Up;		// 물리공격력 향상
	int				m_Defense_Ph_Up;	// 물리방어력 향상
	int				m_Attack_Magic_Up;	// 매직공격력 향상
	int				m_Defense_Magic_Up; // 매직방어력 향상
	int				m_Attack_Speed_Up;  // 공격스피드 향상
	int				m_Evade_Per_Up;		// 회피율 향상
	int				m_SetItem_No;		// Set아이템 번호
	int				m_PuzzleItem_No;	// 퍼즐아이템 번호
	int				m_bChange;			// 변형여부 
	int				m_Change_Per;		// 변형확률
	int				m_Item_Maxup;		// 강화단계
	int				m_bSkill;			// 스킬유무
	int				m_Skill_Kind;		// 스킬종류
	int				m_Serise_Attack;	// 연속공격판단
	int				m_Size;				// 크기

										// 가격 	
};

// 창고
#define MAX_STORAGE_TAB				4
#define STORAGE_X					8
#define STORAGE_Y					12
#define MAX_STORE_PER_TAB  ( STORAGE_X * STORAGE_Y + 1)

struct Char_info_Tab_Storage
{
	Item_Info_Save m_Storage[MAX_STORE_PER_TAB];
};

//말정보 클래스 

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
	int		m_nIndex;		// 스킬 인덱스
	BYTE	m_byLevel;		// 스킬 레벨
};


// 8 - 3 퀘스트 상태 정보 저장용
struct Sinario_Quest
{
	_Value4 m_SQIndex; // 여기서 하나를 가지고 온다. 1개   // (0) index (1) 나머지 한개

	_Value4 m_SQStatus; // 4개 
	
	DWORD	m_QuestTime;		// 퀘스트 지속시간
};

struct Board
{
	int		m_nMyBoard;				// 내가 맞추고 있는 판 번호
	DWORD	m_aryPiece[12];	// 짜맞춘 조각들
};

struct Char_Info_Save
{//모든 정보를 가지고 있다
	long m_CharUnique;
	BYTE m_byCharName[CHARACTER_LENGTH];

	UINT  m_Level;
	UINT  m_Exp;
	UINT  m_Race;
	UINT  m_Cash;
	UINT  m_Guild;

	_Value2 m_StaminaSpell;				// 체력... 마나
	_Value2 m_PowerAgility;				// 힘....	민첩
	_Value2 m_IntelligenceMagic;		// 콘...	마력..    4-14 콘->지구력
	_Value2	m_BraveFamous;				// 용기....	명성		4-14 
	_Value2	m_PossesionRes;				// 퍼제션.. 나중에 쓸 여분..		4-14 
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
	Skills	m_Skills[40];		// 앞에 10 : 기본스킬 뒤에 30개 전직 스킬...디비에 할때 풀어라

	// 6-11
	_Value4	m_Potions;					// 0 : hp물약  1 : mp물약

	// 6 -24 
	UINT  m_Style;				// 캐릭터의 모습  

	// 8 - 3 퀘스트 상태 저장 
	Sinario_Quest s_Quest;	
	
	WORD	m_Extacy;		// 엑스터스 저장 2
	Board   m_Board;		// 보드 아이템 저장용 52
	DWORD	m_DailogTime;	// 재갈시간
	BYTE	m_bySkillPoint;	// 남은 스킬 포인트
	BYTE	m_byWarpID;		// 마지막 저장맵
};

#define LOGIN_PROCESS_OK			0			//로긴성공
#define LOGIN_PROCESS_UNKNOWN		1			//알수없는 에러
#define LOGIN_PROCESS_INVALIDID		2			//아이디가 틀림
#define LOGIN_PROCESS_INVALIDPASS	3			//비번이 틀림
#define LOGIN_PROCESS_OCCUPIED_ACC 	4			//계정이 사용중
#define LOGIN_PROCESS_OCCUPIED_CHAR	5			//캐릭터가 사용중


#define LOGIN_PROCESS_GAMEDB_DEAD	6			//게임디비가 디져서 로긴을 수행할수 없음
#define LOGIN_PROCESS_LOGIN_DEAD	7			//로긴서버가  ""             ""
#define LOGIN_PROCESS_ACCOUNT_DEAD	8			//계정서버가  ""             ""
#define LOGIN_PROCESS_GAMEINFO_DEAD	9			//게임인포서버가 ""
#define LOGIN_PROCESS_ZONE_DEAD		10          //존서버가      ""
#define LOGIN_PROCESS_ACCDB_DEAD	11          //계정디비서버가      ""

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

// CP_어쩌구는 클라이언트에서 서버로 오는 패킷
// SP_어쩌구는 서버에서 클라이언트로 가는 패킷
// MP_어쩌구는 분산 서버에서 사용하는 서버간 통신 패킷
//CP messages-----------------------------------------------------------------------


struct CP_Move_Broadcast:public SIOCPPacket_DFOV				// 이동
{
	CP_Move_Broadcast() {m_Header.wID[PACKET_HEADER] = CM_MOVE_BROADCAST;}
	WORD	angle;		// 이동방향
	DWORD	next;		// 이동 타일값	
	int		floor;		// 이동 층
	DWORD	flag;		// 직선 타일값
};

struct CP_DisConnect:public SIOCPPacket				// 끊을때 사용
{
	CP_DisConnect () {m_Header.wID[PACKET_HEADER] = CM_DISCONNECT;}

};

//#define CM_LOGINZONE 
//client 가 zone서버에 접속후 처음 날려주는 메세지
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
	BYTE	m_byCommand;		// 0 신청, 1 아이템 등록, 2 아이템 제거, 3 돈 액수, 4 확인, 5 취소 6 신청 응답
	long	m_lTarget;		// 0 타겟 유니크, 1,2 아이템 유니크, 3 돈 액수, 4,5 때는 -1
	BYTE	m_byPos;		// 위치..1일때만 값을 넣어주고 사용..이외의 경우에는 사용 안함	6 일경우 1 하자 0 거절
};

struct CP_Require_Index : public SIOCPPacket
{
	CP_Require_Index() { m_Header.wID[PACKET_HEADER] = CM_REQUIRE_INDEX; }
	BYTE	m_byKind;		// 종류... 0:상점 1:텔레포터
	int	m_nIndex;		// npc index
};

struct CP_Warp:public SIOCPPacket
{
	CP_Warp() { m_Header.wID[PACKET_HEADER] = CM_WARP;}
	long m_lUnique;
};

struct CP_Takeoff_Item:public SIOCPPacket		// 아이템은 착용중이니까 착용배열에서만 검색
{
	CP_Takeoff_Item() { m_Header.wID[PACKET_HEADER] = CM_TAKEOFF_ITEM; }
	DWORD		m_itemindex;		// 벗는 아이템
	DWORD		m_pos;				// 벗기는 인벤토리 위치 
	DWORD		m_IOveritem;		// 벗기는 곳에 있는 아이템 인덱스 
	DWORD		m_iOverunique;		// 벗기는 곳에 있는 아이템 유니크 
	DWORD		m_itemUnique;		// 벗는 아이템 equip pos (반지용)
};

struct CP_Change_Item:public SIOCPPacket		// 바꾸는건 인벤토리에서 착용할 아이템검색
{
	CP_Change_Item() { m_Header.wID[PACKET_HEADER] = CM_CHANGE_ITEM; }
	DWORD		m_itemindex;		// 착용할 아이템인덱스
	DWORD		m_itemunique;		// 착용할 건 유니크로 구분도 해야해 12-16
	BYTE		m_pos;				// 착용위치
};

struct CP_Drop_Item:public SIOCPPacket			// 버리는건 착용이거나 인벤이니까 위치 받자
{
	CP_Drop_Item() { m_Header.wID[PACKET_HEADER] = CM_DROP_ITEM; }
	DWORD		m_itemindex;		// 버릴 아이템 인덱스
	DWORD		m_itemunique;		// 버릴것두 유니크로 구분   12-16
	bool	m_equip;
};

struct CP_Pickup_Item:public SIOCPPacket
{
	CP_Pickup_Item() { m_Header.wID[PACKET_HEADER] = CM_PICKUP_ITEM; }
	DWORD		m_itemidx;
	DWORD		m_itemunique;		// 줍는것두 유니크 넣어줘 12-16
	DWORD	m_tileno;
	int		m_floor;	

};

struct CP_Buy:public SIOCPPacket
{
	CP_Buy() { m_Header.wID[PACKET_HEADER] = CM_BUY; }
	DWORD		m_itemidx;		// 아이템 인덱스
	int			m_count;		// 갯수
	int			m_marketindex;		// 1-14추가
	
	DWORD		m_pos;				// 넣고자 하는 인벤토리 위치 6-9 추가
	DWORD		m_iOveritem;		// 겹쳐진 아이템의 인덱스
	DWORD		m_iOverunique;		// 겹쳐진 아이템의 유니크
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
	int		m_weather;		// 0:맑음(비그침) 1:비 2:번개 3:밤  4:낮  
};

struct CP_Sell:public SIOCPPacket
{
	CP_Sell() { m_Header.wID[PACKET_HEADER] = CM_SELL; }
	int		m_item_index;
	bool	m_equip;
// 나중에 item번호 체계 갖춰지면 그거에 따라 변경
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
	_Value2		m_strdex;		// 변화값(힘,민)
	_Value2		m_conwis;		// 변화값(콘,마)
};

struct CP_Use_Potion : public SIOCPPacket
{
	CP_Use_Potion() { m_Header.wID[ PACKET_HEADER ] = CM_USE_POTION; }
	BYTE		m_type;			// 물약인지 HP인지 판별 .....0:hp 1:mp 2:necta 3:tar
	// 나중에 물약 종류...체력..마력 또는 대중소...가리는거 추가
};

struct CP_Try_Unite : public SIOCPPacket
{
	CP_Try_Unite() { m_Header.wID[PACKET_HEADER] = CM_TRY_UNITE; }
	long	m_unique;		// 합체 시도 할 페어리 유니크
};

struct CP_Reply_Unite : public SIOCPPacket
{
	CP_Reply_Unite() { m_Header.wID[PACKET_HEADER] = CM_REPLY_UNITE; }
	bool	m_reply;		// 대답 : true : 합체, false : 안해 띠바야..
	long	m_unique;		// 시도하려고 했던넘
};

struct CP_Seperate_Unite : public SIOCPPacket
{
	CP_Seperate_Unite() { m_Header.wID[PACKET_HEADER] = CM_SEPERATE_UNIT; }		// 이거 날라오면 현재 합체한넘들한테
};

// 3-14
struct CP_Toggle_Run : public SIOCPPacket
{
	CP_Toggle_Run() { m_Header.wID[PACKET_HEADER] = CM_TOGGLE_RUN; }		// 이거 날라오면 걷기/달리기 플래그 뒤집어 준다
};

struct CP_Toggle_AttackMode : public SIOCPPacket
{
	CP_Toggle_AttackMode() { m_Header.wID[PACKET_HEADER] = CM_TOGGLE_ATTACKMODE; }		// 이거 날라오면 방어모드 on/off
};

// 3-19............10-13
struct CP_Party : public SIOCPPacket
{
	CP_Party() { m_Header.wID[PACKET_HEADER] = CM_PARTY; }
	BYTE	m_byCommand;	// 0 : 생성, 1 : 초대, 2 : 초대결과ok 3 : 초대결과no, 4 : 멤버 추가, 5 : 멤버 삭제, 6 : 파티 깨짐
	long	m_lUnique;		// command = 0 -> 안씀 : -1
							// command = 1 -> 초대할 캐릭 유니크
							// command = 2 -> 응답 받을 캐릭 유니크..신청한넘이 되겠지..
							// command = 3 -> 응답 받을 캐릭 유니크..신청한넘이 되겠지..
							// command = 4 -> 추가될 멤버 유니크
							// command = 5 -> 빠질 멤버 유니크
							// command = 6 -> 파티 깨지면 안씀 : -1..이거 받으면 파티정보 지워
};

// 3-24신일
struct CP_Attack_PC : public SIOCPPacket
{
	CP_Attack_PC() { m_Header.wID[PACKET_HEADER] = CM_ATTACK_PC; }
	long	m_lUnique;			// 타겟유저 유니크
	DWORD	m_dwTargetPos;		// xblock선택 때매... 검색쪽 바껴야 하는데...-ㅇ-
	int		m_nMotion;			// 공격 동작
};

struct CP_Request_PK : public SIOCPPacket
{
	CP_Request_PK() { m_Header.wID[PACKET_HEADER] = CM_REQUEST_PK; }
	long	m_lUnique;		// 타겟 유저 유니크
};

struct CP_Reply_PK : public SIOCPPacket
{
	CP_Reply_PK(){ m_Header.wID[PACKET_HEADER] = CM_REPLY_PK; }
	long	m_lUnique;			// 신청한넘
	bool	m_bReply;			// 대답
};

// 3 - 27 first 032
struct CP_Horse_Down : public SIOCPPacket							//말에서 내린다고 함 
{
	CP_Horse_Down() { m_Header.wID[PACKET_HEADER] = CM_HORSE_DOWN; }
	DWORD m_dwDownpos;
};

// 3 -28 032
struct CP_Horse_Ride : public SIOCPPacket							//말을 탄다고 함 
{
	CP_Horse_Ride() { m_Header.wID[PACKET_HEADER] = CM_HORSE_RIDE; }
	int		m_iHorseIndex;		// 나중에 필요 없을수도
	long	m_lUnique;			// 말 유니크
};

// 3-29

// 4-29
struct CP_Change_Class : public SIOCPPacket
{
	CP_Change_Class() { m_Header.wID[PACKET_HEADER] = CM_CHANGE_CLASS; }
	BYTE	m_byJob;		// 전직하고자 하는 직업...255는 없는거..
	BYTE	m_bySkill;		// 배우고자 하는 스킬
};

struct CP_Reply_Revive : public SIOCPPacket
{
	CP_Reply_Revive() { m_Header.wID[PACKET_HEADER] = CM_REPLY_REVIVE; }
	bool	m_bReply;		// true :  응		false : 아니
};

struct CP_Horse_Attr : public SIOCPPacket
{
	CP_Horse_Attr(){ m_Header.wID[PACKET_HEADER] = CM_HORSE_ATTR; }
	int m_iHorseColor;      //몸 설정 
	int m_iSeatColor;		//말 안장 설정
	int m_iHorseIndex;		//말 몸 정류
	int m_iSeatIndex;		//안장 종류
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
	DWORD	m_dwidx;		// 말 인덱스
	long	m_lUnique;		// 말 유니크
};

struct CP_Treat_Carrot : public SIOCPPacket
{
	CP_Treat_Carrot() { m_Header.wID[PACKET_HEADER] = CM_TREAT_CARROT; }
	DWORD	m_dwidx;		// 말 인덱스
	long	m_lUnique;		// 말 유니크
};

struct CP_Use_Skill : public SIOCPPacket
{
	CP_Use_Skill() { m_Header.wID[PACKET_HEADER] = CM_USE_SKILL; }
	bool	m_bSpecial;		// 전직기술인가?	true:특화기술 false:기본기술
	int		m_nSkill;		// 사용할 스킬
	int		m_nNPCIndex;	// 몹이라면 몹 인덱스...사람이면 -1
	long	m_lUnique;		// 타겟이 될 유니크..자신이면 -1, 사람이면 인덱스-1에 사람 유니크, 몹이면 몹 유니크
};

struct CP_Inven_Change : public SIOCPPacket
{
	CP_Inven_Change() { m_Header.wID[PACKET_HEADER] = CM_INVEN_CHANGE; }
	DWORD		m_itemidx;			// 바뀔 id 
	DWORD		m_itemunique;		// 바뀔 유니크 
	DWORD		m_ipos;				// 위치 
	DWORD		m_iOveritem;		// 겹쳐진 아이템의 인덱스
	DWORD		m_iOverunique;		// 겹쳐진 아이템의 유니크 
};

struct CP_Admin_Chat : public SIOCPPacket
{
	CP_Admin_Chat(){ m_Header.wID[PACKET_HEADER] = CM_ADMIN_CHAT; }
	char		m_strMsg[64];		// 메세지
};

struct CP_Now_Time : public SIOCPPacket
{
	CP_Now_Time() { m_Header.wID[PACKET_HEADER ] = CM_NOW_TIME; }
	DWORD		m_dwTime;
};

struct CP_Kill_Client : public SIOCPPacket
{
	CP_Kill_Client() { m_Header.wID[PACKET_HEADER] = CM_KILL_CLIENT; }
	// 나중에 생각하자..id로 할지 유니크로 할지
};

struct CP_Check_Connection : public SIOCPPacket
{
	CP_Check_Connection() { m_Header.wID[PACKET_HEADER] = CM_CHECK_CONNECTION; }
};

// 공통으로 쓰고
struct CP_Require_Data : public SIOCPPacket
{
	CP_Require_Data() { m_Header.wID[PACKET_HEADER] = CM_REQUIRE_DATA; }
	BYTE		m_byKind;		// 0 : 유저  1: npc   2: item
	DWORD		m_dwUnique;		// 유저라면 유니크 npc,item각각 유니크
	DWORD		m_dwIndex;		// npc,item index , 유저라면 0
};

// 7 - 14 얼굴 돌아가는 패킷 
struct CP_Chr_Dir : public SIOCPPacket
{
	CP_Chr_Dir() { m_Header.wID[PACKET_HEADER] = CM_CHR_DIR; }
	BYTE		m_byDir;  // 얼굴 각도 
};

// 7 - 16 npc HP 요청
struct CP_NPC_Energy : public SIOCPPacket
{
	CP_NPC_Energy() { m_Header.wID[PACKET_HEADER] = CM_NPC_ENERGY; }
	DWORD		m_dwIndex;  // 보고자 하는 npc인덱스
	DWORD		m_dwUnique;	// npc 유니크 
};

// 7 - 16 npc 상태 요청
struct CP_NPC_Status : public SIOCPPacket
{
	CP_NPC_Status() { m_Header.wID[PACKET_HEADER] = CM_NPC_STATUS; }
	DWORD		m_dwIndex;
	DWORD		m_dwUnique; 
};

// 8 - 1 quest요청
struct CP_Get_Quest : public SIOCPPacket
{
	CP_Get_Quest() { m_Header.wID[PACKET_HEADER] = CM_GET_QUEST; }
	BYTE		m_QuestClass;
	BYTE		m_QuestIndex;
};

struct CP_Put_Piece : public SIOCPPacket
{
	CP_Put_Piece() { m_Header.wID[PACKET_HEADER] = CM_PUT_PIECE; }
	DWORD		m_dwIndex;		// 끼울조각 인덱스
	DWORD		m_dwUnique;		// 끼울조각 유니크
	BYTE		m_byPosition;	// 끼울 위치
};

struct CP_Succ_Quest : public SIOCPPacket
{
	CP_Succ_Quest() { m_Header.wID[PACKET_HEADER] = CM_SUCC_QUEST; }
};

struct CP_Complete_Puzzle : public SIOCPPacket
{
	CP_Complete_Puzzle() { m_Header.wID[PACKET_HEADER] = CM_COMPLETE_PUZZLE; }
	DWORD	m_dwItem;		// 업그레이드할 아이템 유니크..어차피 유니크는 안겹침
	BYTE 	m_byNumber[5];		// 아이템 인덱스의 끝자리 번호..알파벳이니까..
	DWORD	m_dwUnique[6];		// 꽂은 아이템 유니크 + 보석 유니크
};

struct CP_Quest_Check : public SIOCPPacket
{
	CP_Quest_Check() { m_Header.wID[PACKET_HEADER] = CM_QUEST_CHECK; }
	BYTE m_QuestClass;		//	뭔지 알지 ^^ 0 이면 시나리오 1이면 event 2  제조 3 보드 
	BYTE m_Case;			// 이건 경우의수 0 npc die 1 pickup item 2 make item 3 meet npc 4는 pickup item drop
	// 5는  make item 중 drop
	DWORD m_Index;			// 
};

struct CP_Mode_Change : public SIOCPPacket
{
	CP_Mode_Change() { m_Header.wID[PACKET_HEADER] = CM_MODE_CHANGE; }
	BYTE	m_byType;		// 0 :charge 1:방어 모드 2:걷기 뛰기 3:pk ..기타 등등...전에 쓰던 토글 통합 가능
							// 4 : 칼뽑기
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
	BYTE	m_Class; 			// 메시지 종류 0 부터 11 까지	
	char 	m_CharID[12];		// 찾고자 하는 아이디 
	DWORD	m_ItemNpcTarget;	// NPC, ITEM
	byte 	m_Map;				// 맵 번호 
	int  	m_X;				// X
	int  	m_Y;				// Y 
	DWORD	m_CoinTime;			// 줄돈, 타임
	char 	m_AllChat[64];		// 운영자가 전체 대화시 쓴다.
};

struct CP_Operator_Chat : public SIOCPPacket
{
	CP_Operator_Chat() {m_Header.wID[PACKET_HEADER] = CM_OPERATOR_CHAT;}
	char m_chatmsg[64];
};

struct CP_Skill_Upgrade : public SIOCPPacket
{
	CP_Skill_Upgrade() { m_Header.wID[PACKET_HEADER] = CM_SKILL_UPGRADE; }
	BYTE	m_bySkill;	// 업할 스킬
};

// 주고받기 신청
struct CP_Try_Itemdeal : public SIOCPPacket
{
	CP_Try_Itemdeal() { m_Header.wID[PACKET_HEADER] = CM_TRY_ITEMDEAL; }
	long	m_rev_unique;		// 받는넘 
	DWORD	m_Itemindex;
	DWORD   m_Itemunique;	
};

// 주고받기 응답
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
	DWORD		m_itemidx;			// 바뀔 id	// 돈이면 0:넣고 1:빼고 
	DWORD		m_itemunique;		// 바뀔 유니크 // 돈 
	BYTE		m_Pos[4];			// 0. 기존 tab 1. 이동할 tab 2. 이동할 위치 3. command:0인풋1:아웃풋2:체인지3:돈4:창고시작
};

struct CP_Teleporter : public SIOCPPacket
{
	CP_Teleporter() { m_Header.wID[ PACKET_HEADER] = CM_TELEPORTER; }
	BYTE	m_byIndex;		// 인덱스 번호
	BYTE	m_bySort;		// 목록내 번호(순서)
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
struct SP_Move_Broadcast:public SIOCPPacket					// 이동
{
	SP_Move_Broadcast() {m_Header.wID[PACKET_HEADER] = SM_MOVE_BROADCAST;}
	DWORD	who;	// 누가...infoserver & db에서 가지고 있는 고유 캐릭터 id
	DWORD	start;	// 어디에서
	DWORD	dest;	// 어디로
	int		floor;	// 7-5
	bool	m_bUnite;
	long	m_lUnite_unique;
	// 3-15
//	bool	m_bAttackMode;		// 방어모드 on/off
	bool	m_bCurrent;			// 현재 걷기/뛰기

	//9 - 17 대각선 이동 땜시 추가 	
	DWORD   m_flag;				// 별이 알아서 
};

struct SP_Appear:public SIOCPPacket					// 클라이언트 접속시 사용
{
	SP_Appear() {m_Header.wID[PACKET_HEADER] = SM_APPEAR;}
	long		m_lUnique;
	DWORD		m_where;
	int			m_floor;
	Char_Info	char_info;
	// 합체 때매 3-11추가
	bool		m_bUnite;			// 합체 여부
	long		m_lUnite_unique;	// 합체한넘 유니크
	// 3-15
	bool		m_bAttackMode;		// 방어모드 on/off

	//3-31	
	char		m_HorseName[12];	// 말이름 방송 
	bool		m_bHorseRide;		// 말을 타고 있는지를
	DWORD		m_type;				// 4비트가 야생마인지?

	bool		m_bCharge;			// true : 차지모드 상태 false : 일반 상태
	bool		m_bNecta;			// true : 마시고 있는 상태 false : 일반 상태
	bool		m_bTar;				// true : 피고있다 false : 일반 상태
	bool		m_bPK;				// true : pk모드(아뒤 빨간색) false : pk모드 아님

	bool		m_bToggle;			// 걷기 모드 상태 
	bool		m_bSword;			// 칼위치
	bool		m_bSit;				// 앉아 있나??
	BYTE		m_byStatus;			// 현재 유저가 Login해서 쏜건지 : 0 , Xblock내에 들어와서 쏜건지 : 1
	BYTE		m_bySkillStatus;	// 현재 걸려있는 상태 변화 스킬..base.h참조
};

struct SP_DisAppear:public SIOCPPacket				// 클라이언트 끊겼을때 사용
{
	SP_DisAppear() {m_Header.wID[PACKET_HEADER] = SM_DISAPPEAR;}
	long	who;	// 없어질 넘...
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

struct SP_Check_Connection:public SIOCPPacket		// 6-26 추가
{
	SP_Check_Connection() { m_Header.wID[PACKET_HEADER] = SM_CHECK_CONNECTION; }
};

struct SP_Normal_Chat:public SIOCPPacket
{
	SP_Normal_Chat() {m_Header.wID[PACKET_HEADER] = SM_NORMAL_CHAT; ZeroMemory( m_chatmsg, 64 ); }
	long m_unique;		// 말한넘
	char m_chatmsg[64];		// 내용
};

struct SP_Private_Chat:public SIOCPPacket		// 서버 귓말 패킷
{
	SP_Private_Chat() { m_Header.wID[PACKET_HEADER] = SM_PRIVATE_CHAT;}
	char m_msg[64];			// 메세지
};

struct SP_NPC_Appear:public SIOCPPacket		// 이동중 범위에 나타나거나 소환됐을때 주변에 방송용
{
	SP_NPC_Appear() {m_Header.wID[PACKET_HEADER] = SM_NPC_APPEAR;}
	int		m_NPC_Index;
	int		m_floor;
	DWORD	m_pos;
	DWORD	m_npc_unique;
	DWORD	m_type;				// 0 말의 몸통 1, 몸통색깔  2. 안장 종류 3. 안장 색깔
	BYTE	m_npc_status;		// npc상태 
	BYTE	m_bySkillStatus;	// 현재 걸려있는 상태 변화 스킬..base.h참조
};

struct SP_NPC_Move:public SIOCPPacket
{
	SP_NPC_Move() { m_Header.wID[PACKET_HEADER] = SM_NPC_MOVE;}
	int	m_NPC_Index;
	DWORD m_npc_unique;
	DWORD m_cur;
	DWORD m_dest;
	int   m_floor;
	bool	m_bFight;		// 이동 모션..true면 뛰기..false면 걷기
};

struct SP_Shout_Chat:public SIOCPPacket			// 서버가 클라이언트에게 직접 쏘는거
{
	SP_Shout_Chat() { m_Header.wID[PACKET_HEADER] = SM_SHOUT_CHAT;}
	char m_msg[64];
};

struct SP_Party_Chat:public SIOCPPacket
{
	SP_Party_Chat() { m_Header.wID[PACKET_HEADER] = SM_PARTY_CHAT; }
	long	m_lUnique;		// 말한 멤버 유니크...이름,유니크는 처음에 받아서 가지거 있다
	char	m_msg[64];
};

struct SP_Trade:public SIOCPPacket
{
	SP_Trade() { m_Header.wID[PACKET_HEADER] = SM_TRADE; }
	BYTE	m_byCommand;		// 0 신청, 1 아이템 등록, 2 아이템 제거, 3 돈 액수, 4 확인, 5 취소 6 신청 응답 7 거래시작
	long	m_lUser;		// 0 타겟 유니크, 1,2 아이템 유니크, 3 돈 액수, 4,5 때는 -1...아이템 등록하거나 빼는 놈
	Item_Info_Save 	m_lTarget;	// 안에 유니크를 위와 동일한 기능, 유니온들은 아이템에만 관련되어 사용.
	BYTE	m_byPos;		// 위치..1일때만 값을 넣어주고 사용..이외의 경우에는 사용 안함 6 일경우 1 하자 0 거절
};

struct SP_Require_Index : public SIOCPPacket
{
	SP_Require_Index() { m_Header.wID[PACKET_HEADER] = SM_REQUIRE_INDEX; }
	BYTE	m_byKind;		// 종류... 0:상점 1:텔레포터
	BYTE	m_byIndex;		// 해당 목록에서의 번호
};



struct SP_Warp: public SIOCPPacket
{
	SP_Warp() { m_Header.wID[PACKET_HEADER] = SM_WARP; }
	int		m_Server;		// 접속하게 될 서버
	long	m_Unique;		// 워프하는 넘
	int		m_Map_ID;		// 워프할 맵번호
	DWORD	m_Pos;			// 워프한 맵에서 나타날 좌표
	int		m_floor;		// 층..항상 0
	bool	m_result;		// 결과
	// 3-11
	bool	m_bUnite;
	long	m_lUnite_unique;
};

struct SP_Takeoff_Item:public SIOCPPacket
{
	SP_Takeoff_Item() { m_Header.wID[PACKET_HEADER] = SM_TAKEOFF_ITEM; }
	long	m_lUnique;		// 벗길넘
	DWORD		m_itemindex;	// 벗은거
	DWORD		m_dwitemUnique;  // 벗는거 유니크
};

struct SP_Change_Item:public SIOCPPacket
{
	SP_Change_Item() { m_Header.wID[PACKET_HEADER] = SM_CHANGE_ITEM; }
	long	m_lUnique;
	DWORD		m_itemindex;	// 입을꺼
	DWORD		m_itemunique;	// 입는거 유니크
	DWORD		m_ipos;		// 위치
	BYTE		m_arraypos;   // 
};

struct SP_Drop_Item:public SIOCPPacket
{
	SP_Drop_Item() { m_Header.wID[PACKET_HEADER] = SM_DROP_ITEM; }
	DWORD		m_itemidx;		// 버린 아이템 인덱스...인벤에서 제거할것
	DWORD		m_itemunique;	// 버린 아이템 유니크
};

struct SP_Item_Appear:public SIOCPPacket
{
	SP_Item_Appear() { m_Header.wID[PACKET_HEADER] = SM_ITEM_APPEAR; }
	DWORD		m_itemidx;		// 땅에 표시될 그림 인덱스
	DWORD		m_itemunique;	// 유니크
	DWORD	m_pos;			// 위치
	int		m_floor;
	bool	m_changed;		// 변신상태
	DWORD	m_dwPos;
};

struct SP_Pickup_Item:public SIOCPPacket
{
	SP_Pickup_Item() { m_Header.wID[PACKET_HEADER] = SM_PICKUP_ITEM; }
	DWORD	m_item_index;
	DWORD	m_item_unique;
	bool	m_res;
	DWORD	m_pos;			// 인벤에 들어갈 위치
	_Value4 m_iteminfo;     // 아이템의 정보들
};

// 줍거나 충돌해서 없어지거나 맞아죽었을때는 이거
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
	DWORD	m_itemidx;		// 아이템 인덱스
	DWORD	m_itemunique;	// 산 아이템 유니크
	int		m_count;		// 갯수
	DWORD   m_pos;			// 아이템 위치
	_Value4 m_iteminfo;		// 아이템의 정보 life , upgrade , color
};

#define NOT_ENOUGH_MONEY	0
#define NOT_ENOUGH_INVEN	1
struct SP_Buy_Error:public SIOCPPacket
{
	SP_Buy_Error() { m_Header.wID[PACKET_HEADER] = SM_BUY_ERROR; }
//	char	m_msg[32];		// 이부분은 나중에 제거됨...구입후 error패킷 받으면 구입 실패..팔기도 마찮가지...
	BYTE	m_error;
};

struct SP_Attack_NPC:public SIOCPPacket
{
	SP_Attack_NPC() { m_Header.wID[PACKET_HEADER] = SM_ATTACK_NPC; }
	DWORD	m_tileno;		// 공격 위치
	DWORD	m_npcunique;	// 공격 당하는 npc유니크
	long	m_unique;		// 공격동작 취하는 pc유니크
	int		m_dmg;			// 공격력...0이면 회피한것
	int		m_motion;
	// 3-15
//	bool	m_bAttackMode;	// 공격 모드
	bool	m_bCritical;	// true : 크리티컬 false : 걍

};

struct SP_Weather:public SIOCPPacket
{
	SP_Weather() { m_Header.wID[PACKET_HEADER] = SM_WEATHER; }
	int		m_weather;		// base.h 에 define 참조
	int		m_weather2;		// 보조날씨
};

struct SP_NPC_Respawn:public SIOCPPacket
{
	SP_NPC_Respawn() { m_Header.wID[PACKET_HEADER] = SM_NPC_RESPAWN; }
	int		m_npc_index;		// 참조할 인덱스
	DWORD	m_npc_unique;		// 몬스터 유니크
	DWORD	m_tileno;			// 생성위치
	int		m_floor;			// 생성 층
	bool	m_speechflag;		// 말 할까 말까
};

struct SP_NPC_Die:public SIOCPPacket
{
	SP_NPC_Die() { m_Header.wID[PACKET_HEADER] = SM_NPC_DIE; }
	int		m_npc_index;		// 죽는 모션 틀어줄 인덱스
	DWORD	m_npc_unique;		// 죽는 넘 유니크
	bool	m_speechflag;		// 말 할까 말까
	long	m_lUnique;			// 공격 한넘
	bool	m_bType;			// true : 근거리 false : 원거리
};

struct SP_Sell_OK:public SIOCPPacket
{
	SP_Sell_OK() { m_Header.wID[PACKET_HEADER] = SM_SELL_OK; }
	DWORD		m_item_index;			// 제거해줄 아이템
// 나중에 item번호 체계 갖춰지면 그거에 따라 변경
	DWORD	m_item_unique;
};

struct SP_Sell_Error:public SIOCPPacket
{
	SP_Sell_Error() { m_Header.wID[PACKET_HEADER] = SM_SELL_ERROR; }
//	BYTE	m_error_code;			// 실패코드
};

struct SP_NPC_Attack_PC:public SIOCPPacket
{
	SP_NPC_Attack_PC() { m_Header.wID[PACKET_HEADER] = SM_NPC_ATTACK_PC; }
	int		m_index;	// npc index
	DWORD	m_npc;		// npc unique
	long	m_target;	// target user unique
	int		m_motion;	// 틀어줄 동작
	int		m_dmg;		// 데미지
	int		m_dialog;	// 대사
	bool	m_speechflag;		// 말 할까 말까

};

struct SP_Time_Change:public SIOCPPacket
{
	SP_Time_Change() { m_Header.wID[PACKET_HEADER] = SM_TIME_CHANGE; }
	short	m_time;		// 바뀔 시간
};

struct SP_Item_Change:public SIOCPPacket		// 변신하는 넘
{
	SP_Item_Change() { m_Header.wID[PACKET_HEADER] = SM_ITEM_CHANGE; }
	DWORD	m_itemindex;		// 인덱스
	DWORD	m_itemunique;		// 유니크
};

struct SP_Item_Move:public SIOCPPacket
{
	SP_Item_Move() { m_Header.wID[PACKET_HEADER] = SM_ITEM_MOVE; }
	DWORD	m_itemindex;		// 인덱스
	DWORD	m_itemunique;		// 유니크
	DWORD	m_tileno;			// 이동할 위치
	int		m_floor;			// 층
};

// 시간 되서 소멸 
struct SP_Item_Extinction:public SIOCPPacket
{
	SP_Item_Extinction() { m_Header.wID[PACKET_HEADER] = SM_ITEM_EXTINCTION; }
	DWORD	m_itemindex;	// 인덱스
	DWORD	m_itemunique;	// 아이템 유니크
};

struct SP_Attack_Item:public SIOCPPacket
{
	SP_Attack_Item() { m_Header.wID[PACKET_HEADER] = SM_ATTACK_ITEM; }
	long	m_userunique;		// 공격하는넘 유니크
	DWORD	m_itemindex;		// 공격당할 넘 인덱스
	DWORD	m_itemunique;		// 공격당할 넘 유니크
	int		m_motion;
	int		m_dmg;
	// 3-15
//	bool	m_bAttackMode;		// 필요 없을듯..왜냐하면 캐릭터들이 처음 받는 appear나 move중 appear에서 현재 모드를 알수 있다
};

// npc 말해
struct SP_NPC_Speech : public SIOCPPacket
{
	SP_NPC_Speech() { m_Header.wID[PACKET_HEADER] = SM_NPC_SPEECH; }
	int		m_npcindex;			// 인덱스
	DWORD	m_npcunique;		// 유니크
	BYTE	m_status;			// 대사할 상태
};

struct SP_User_Recover:public SIOCPPacket
{
	SP_User_Recover(){ m_Header.wID[ PACKET_HEADER] = SM_USER_RECOVER; }
	long		m_unique;		// 체력,마력 찰넘..나중에 파티가 되면 언넘인지 알아야 하니까
	_Value2		m_recover;		// 회복될 체력,마력...이 아니라..현재 너의 값이다
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
	bool		m_result;	// 결과
	int			m_point;
};

struct SP_Change_Exp : public SIOCPPacket
{
	SP_Change_Exp() { m_Header.wID[ PACKET_HEADER ] = SM_CHANGE_EXP; }
	BYTE		m_type;			// 0: user 1: horse
	DWORD		m_exp;			// 니 현재 경험치야
};

struct SP_Confirm_Cash : public SIOCPPacket
{
	SP_Confirm_Cash() { m_Header.wID[ PACKET_HEADER ] = SM_CONFIRM_CASH; }
	DWORD		m_money;		// 니가 가지고 있는 현재 돈이얌
};

struct CHR_STAT
{
	// 4-15
	_Value2 m_PowerAgility;				// 힘....	민첩
	_Value2 m_IntelligenceMagic;		// 콘...	마력..    4-14 콘->지구력
	_Value2	m_BraveFamous;				// 용기....	명성		4-14 
	_Value2	m_PossesionRes;				// 퍼제션.. 나중에 쓸 여분..		4-14 
	//
	int		m_att;			// 공격력
	int		m_sucrate;		// 성공율
	int		m_evaderate;	// 회피율
	int		m_mp;			// 마력
	int		m_magicac;		// 마법 방어력
	int		m_hp;			// 체력
	int		m_ac;			// 방어력
	int		m_magicatt;		// 매직 공격력
};

struct SP_Confirm_Stat : public SIOCPPacket
{
	SP_Confirm_Stat() { m_Header.wID[ PACKET_HEADER ] = SM_CONFIRM_STAT; }
	CHR_STAT	m_stat;
};

struct SP_Use_Potion : public SIOCPPacket
{
	// 나중에 해당 물약의 남은 갯수 표시를 위해 사용하는게 좋을듯
	SP_Use_Potion() { m_Header.wID[ PACKET_HEADER ] = SM_USE_POTION; }
	_Value2		m_recover;
};

struct SP_Destroy_Item : public SIOCPPacket
{
	SP_Destroy_Item() { m_Header.wID[ PACKET_HEADER ] = SM_DESTROY_ITEM; }
	int		m_index;
	DWORD	m_unique;
	long	m_lUnique;	// 부순넘
};

struct SP_Try_Unite : public SIOCPPacket
{
	SP_Try_Unite() { m_Header.wID[PACKET_HEADER] = SM_TRY_UNITE; }
	long	m_unique;		// 합체 시도하려는 넘의 유니크( 페어리에게 날릴 메세지임 )
};
struct SP_Reply_Unite : public SIOCPPacket
{
	SP_Reply_Unite() { m_Header.wID[PACKET_HEADER] = SM_REPLY_UNITE; }
	long	m_man_unique;		// 합체하는 넘 유니크. -1이면 실패다 이너마
	long	m_fairy_unique;		// 합체하는 넘 유니크. -1이면 실패다 이너마
};
struct SP_Seperate_Unite : public SIOCPPacket
{
	SP_Seperate_Unite() 
	{ m_Header.wID[PACKET_HEADER] = SM_SEPERATE_UNIT; m_human_unique = -1; m_fairy_unique = -1;}		// 이거 날려주면 각각 자기가 콘트롤을 한다..준비하고.자기 그려놓고..
	long	m_human_unique;		// 분리할 인간 유니크
	long	m_fairy_unique;		// 분리할 페어리 유니크
	DWORD	m_fairy_position;	// 분리한 페어리가 있을 자리
};		// 이거 받은 넘들은 저 유니크의 넘을 분리된 상태(즉 페이리를 지운다)로 그리고 페어리 appear받아서 그린다.

// 3-14
struct SP_Toggle_Run : public SIOCPPacket
{
	SP_Toggle_Run() { m_Header.wID[PACKET_HEADER] = SM_TOGGLE_RUN; }
	bool	m_bCurrent;		// false : 걷기, true : 달리기
};
struct SP_Toggle_AttackMode : public SIOCPPacket
{
	SP_Toggle_AttackMode() { m_Header.wID[PACKET_HEADER] = SM_TOGGLE_ATTACKMODE; }
	long	m_lUnique;		// 보여지는넘 유니크
	bool	m_bCurrent;		// false : 꺼져있다, true : 켜져있다..방어모드 상태
};


struct SP_Party : public SIOCPPacket
{
	SP_Party() { m_Header.wID[PACKET_HEADER] = SM_PARTY; }
	BYTE	m_byCommand;	// 0 : 생성, 1 : 초대, 2 : 초대결과ok 3 : 초대결과no, 4 : 멤버 추가, 5 : 멤버 삭제, 6 : 파티 깨짐
	long	m_lUnique;		// command = 0 -> 안씀 : -1
							// command = 1 -> 초대할 캐릭 유니크
							// command = 2 -> 응답 받을 캐릭 유니크..신청한넘이 되겠지..
							// command = 3 -> 응답 받을 캐릭 유니크..신청한넘이 되겠지..
							// command = 4 -> 추가될 멤버 유니크
							// command = 5 -> 빠질 멤버 유니크
							// command = 6 -> 파티 깨지면 안씀 : -1..이거 받으면 파티정보 지워
};
struct SP_Member_Info : public SIOCPPacket
{
	SP_Member_Info() { m_Header.wID[PACKET_HEADER] = SM_MEMBER_INFO; }
	char	m_Name[12];
	long	m_lUnique;				// 나중에 체력변화량 받는곳에서 unique가 다르면 파티원에서 유니크 찾아라
	BYTE	m_byJob;
	BYTE	m_byHP;					// 체력 % 현재피/만피
//	_Value2	m_HP;					// 0번은 max 1번은 cur
};
struct SP_Member_Recover : public SIOCPPacket
{
	SP_Member_Recover() { m_Header.wID[PACKET_HEADER] = SM_MEMBER_RECOVER; }
	long	m_lUnique;		// 변화하는넘
	BYTE	m_byHP;			// 체력 %
};

struct SP_Arrow_Going : public SIOCPPacket
{
	SP_Arrow_Going() { m_Header.wID[PACKET_HEADER] = SM_ARROW_GOING; }
	int		m_nShooterIndex;	// 쏘는넘 인덱스..사람이면 -1;
	long	m_lShooter;			// 쏘는넘
	int		m_nTargetIndex;		// 맞는넘인덱스...몹일땐 인덱스 사람이면 -1..유니크로만 찾아봐 
	DWORD	m_lTarget;			// 맞는넘
//	DWORD	m_dwStart;		// 시작점
//	DWORD	m_dwDest;		// 도착점
};

struct SP_Magic_Going : public SIOCPPacket
{
	SP_Magic_Going() { m_Header.wID[PACKET_HEADER] = SM_MAGIC_GOING; }
	DWORD	m_dwStart;		// 시작점
	DWORD	m_dwDest;		// 도착점
	int		m_nIndex;		// effect index
};



//3 - 27 032
struct SP_Horse_Down : public SIOCPPacket
{
	WORD m_wError;			//0이면 에러가 없다. 1이면 내릴장소가 없다.
							
	long m_lCharacter_Unique;  //말 에서 내린사람  사람 

	DWORD m_wDownSpace;		//내릴 장소 
	
	int m_iHorse_Index;		//말인덱스

	long m_lHorse_Unique;	//말의 유니크

	SP_Horse_Down(){m_Header.wID[PACKET_HEADER] = SM_HORSE_DOWN;}
};

// 3-24 신일
struct SP_Attack_PC : public SIOCPPacket
{
	SP_Attack_PC() { m_Header.wID[PACKET_HEADER] = SM_ATTACK_PC; }
	long	m_lAttacker;		// 공격자 유니크
	long	m_lDefender;		// 방어자 유니크
	int		m_nMotion;			// 공격 동작
	int		m_nDmg;				// 데미지

	bool	m_bCritical;
};

struct SP_Request_PK : public SIOCPPacket
{
	SP_Request_PK() { m_Header.wID[PACKET_HEADER] = SM_REQUEST_PK; }
	long	m_lUnique;			// 신청한넘
};

struct SP_Reply_PK : public SIOCPPacket
{
	SP_Reply_PK() { m_Header.wID[PACKET_HEADER] = SM_REPLY_PK; }
	long	m_lUnique;			// 타겟이 되는 넘의 유니크..yes면 쌍방이 공격 가능 no면 ..님이 거절했습니다. 
	bool	m_bReply;			// true : ok	false : no
};


// 3- 28 032
struct SP_Horse_Ride : public SIOCPPacket
{
	SP_Horse_Ride(){ m_Header.wID[PACKET_HEADER] = SM_HORSE_RIDE; }

	WORD m_wError;			//0이면 에러가 없다. 1이면 말이 없다는 거 2이면 자기말이 아닌데 탈려구 하는거 							
	long m_lCharacter_Unique;  //말을 탈려고 하는 사람
	int m_iHorseIndex;	//어떤말을 탈까..
	DWORD	m_dwUnique;		// 말 유니크
};


// 3-29

// 4-29
struct SP_Change_Class : public SIOCPPacket
{
	SP_Change_Class() { m_Header.wID[PACKET_HEADER] = SM_CHANGE_CLASS; }
	BYTE	m_byJob;		// 255는 실패..
};

struct SP_Request_Revive : public SIOCPPacket
{
	SP_Request_Revive() { m_Header.wID[PACKET_HEADER] = SM_REQUEST_REVIVE; }
	long	m_lUnique;		// 너 살려줄라는넘
};

// 5-1
struct SP_NPC_Damage : public SIOCPPacket
{
	SP_NPC_Damage() { m_Header.wID[PACKET_HEADER] = SM_NPC_DAMAGE; }
	int		m_nAttacker;	// 공격자 인덱스 사람이면 -1 나중에 몹이면 인덱스
	long	m_lUnique;		// 공격자 유니크
	int		m_nIndex;		// npc index
	DWORD	m_dwUnique;		// npc unique
	int		m_nDmg;			// 공격 당했으면 dmg... -1 : 상태 변화만인거..
	BYTE	m_byStatus;		// 상태변화이면.. 0 : 공격만 당한거	1:독걸림  2:스턴   
	BYTE	m_bySkill;		// 스킬 넘버
	bool	m_bCritical;
};


struct SP_User_Revive : public SIOCPPacket
{
	SP_User_Revive() { m_Header.wID[PACKET_HEADER] = SM_USER_REVIVE; }
	long	m_lUnique;		// 이 유니크인넘 살려줘
};

struct SP_Horse_Attr : public SIOCPPacket
{
	SP_Horse_Attr(){ m_Header.wID[PACKET_HEADER] = SM_HORSE_ATTR; }
	// 1 : 말이 없어서 속성을 설정 할수 없다.
	WORD	m_wError;			 							
	long	m_lCharacter_Unique;	// 말의 모습을 바꿀려고 하는 사람 
	int m_iHorseColor;      //몸 설정 
	int m_iSeatColor;		//말 안장 설정
	int m_iHorseIndex;		//말 몸 정류
	int m_iSeatIndex;		//안장 종류
};

struct SP_Horse_Keepin : public SIOCPPacket
{
	SP_Horse_Keepin(){ m_Header.wID[PACKET_HEADER] = SM_HORSE_KEEPIN; }
	long m_lCharacter_Unique;		// 말을 맡길려고 하는 사람 	
};

struct SP_Horse_Keepout : public SIOCPPacket
{
	SP_Horse_Keepout(){ m_Header.wID[PACKET_HEADER] = SM_HORSE_KEEPOUT; }
	
	DWORD		m_lCharacter_Unique;	// 말을 빼는 사람 	
	char		m_HorseName[12];
	DWORD		m_Type;
};

struct SP_Taming_Horse : public SIOCPPacket
{
	SP_Taming_Horse() { m_Header.wID[PACKET_HEADER] = SM_TAMING_HORSE; }
	// 1 : 말을 가지고 있는 상태다. 
	// 2 : 말을 마굿간에다 맡긴 상태다. 
	// 3 : 말의 꼬심을 실패 했다. 
	// 4 : 꼬심의 수가 꼬실수 있는 수를 초과 했다.
	WORD	m_wError;				
	long	m_lCharacter_Unique;	// 말을 얻는 사람 

	// Taming 성공한 말
	int		m_iIndex;				// 말의 인덱스 
	DWORD	m_dwHorseUnique;		// 말 유니크
};

struct SP_Treat_Carrot : public SIOCPPacket
{
	SP_Treat_Carrot() { m_Header.wID[PACKET_HEADER] = SM_TREAT_CARROT; }
	WORD m_wError;			// 0이면 말이 당근을 먹었다.(50% 깍기고)
							// 1이면 말이 독당근을 먹어서 죽었다.
							// 2이면 말을 가지고 있으므로 말을 꼬실수 없다.
};

struct SP_NPC_Disappear : public SIOCPPacket
{
	SP_NPC_Disappear(){  m_Header.wID[PACKET_HEADER] = SM_NPC_DISAPPEAR; }
	int		m_iIndex;			// 사라지는 NPC 인덱스
	DWORD	m_dwUnique;			// 사라지는 NPC 유니크
};

struct SP_Use_Skill : public SIOCPPacket
{
	SP_Use_Skill() { m_Header.wID[PACKET_HEADER] = SM_USE_SKILL; }
	BYTE	m_byRes;		// 스킬 사용 결과... 0:성공 1:타겟이 잘못됐다 2:사용능력치부족  3:사용가능 무기 틀림
	int		m_nSkill;		// 사용스킬
	long	m_lCaster;		// 시전자 유니크
	int		m_nCasterIndex;	// 시전자가 몹일수도 있으니까..
	long	m_lTarget;		// 타겟 유니크
	int		m_nTargetIndex;	// 타겟이 몹일수도 있으니까..
};

struct SP_Learn_Skill : public SIOCPPacket
{
	SP_Learn_Skill() { m_Header.wID[PACKET_HEADER] = SM_LEARN_SKILL; }
	int m_iSkillIndex;	//배운 스킬 인덱스
};

struct SP_Inven_Change : public SIOCPPacket
{
	SP_Inven_Change() { m_Header.wID[PACKET_HEADER] = SM_INVEN_CHANGE; }
	bool m_bValue;
};

struct SP_Admin_Chat : public SIOCPPacket
{
	SP_Admin_Chat(){ m_Header.wID[PACKET_HEADER] = SM_ADMIN_CHAT; }
	char		m_strMsg[64];		// 메세지
};

struct SP_Slide_NPC : public SIOCPPacket
{
	SP_Slide_NPC(){ m_Header.wID[PACKET_HEADER] = SM_SLIDE_NPC; }
	int			m_nIndex;			// npc index
	DWORD		m_dwUnique;			// npc unique
	DWORD		m_dwPos;			// 밀려날 위치
	BYTE		m_byDirection;		// 쳐다볼 방향
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
	// 이거 받음 접속 끊을때 날리는 패킷 쏴라
	SP_Kill_Client() { m_Header.wID[ PACKET_HEADER] = SM_KILL_CLIENT; }
};

struct SP_No_Exist : public SIOCPPacket
{
	SP_No_Exist() { m_Header.wID[PACKET_HEADER] = SM_NO_EXIST; }
	BYTE		m_byKind;		// 1: npc   2: item
	DWORD		m_dwUnique;		// 유저라면 유니크 npc,item각각 유니크
	DWORD		m_dwIndex;		// npc,item index , 유저라면 -1
};

struct SP_Chr_Dir : public SIOCPPacket
{
	SP_Chr_Dir() { m_Header.wID[PACKET_HEADER] = SM_CHR_DIR; }
	DWORD		m_dwUnique;		// 캐릭터 유니크 
	BYTE		m_byDir;		// 머리 방향 
};

struct SP_NPC_Energy : public SIOCPPacket
{
	SP_NPC_Energy() {  m_Header.wID[PACKET_HEADER] = SM_NPC_ENERGY; }
	int			m_dwIndex;		// 
	DWORD		m_dwUnique;		//
	byte		m_HPper;		//
};

// 7 - 16 npc 상태 요청  * -1이면 유저 상태 *
struct SP_NPC_Status : public SIOCPPacket
{
	SP_NPC_Status() { m_Header.wID[PACKET_HEADER] = SM_NPC_STATUS; }
	int 		m_dwIndex;  // -1이면 유저의 상태입니다.
	DWORD		m_dwUnique; 
	BYTE		m_byStatus;
};

// 7 - 28 아니마 아이템 상태 
struct SP_Anima_Status : public SIOCPPacket
{
	SP_Anima_Status() { m_Header.wID[PACKET_HEADER] = SM_ANIMA_STATUS; }
	DWORD		m_dwUnique; //  유저 유니크 
	int			m_nIndex;  //  npc 인덱스 
	DWORD		m_dwNPCUnique; //  npc 유니크
	BYTE		m_byStatus; //  상태
};

// 7 - 28 현재 유저 상의 상태의 변화를 쏴준다.
struct SP_Character_Status : public SIOCPPacket
{
	SP_Character_Status() { m_Header.wID[PACKET_HEADER] = SM_CHARACTER_STATUS; }
	DWORD		m_dwUnique; //  유저 유니크 
	BYTE		m_byStatus; //  상태
	WORD		m_Time;		//  걸리면 얼마나 걸린건지 지속시간
};

// 8 - 1 퀘스트 신청 응답.
struct SP_Get_Quest : public SIOCPPacket
{
	SP_Get_Quest() { m_Header.wID[PACKET_HEADER] = SM_GET_QUEST; }
	BYTE m_Err;  // 0이면 성공 1: 현재 퀘스트를 가지고 있다. 2: 직업이 안 맞는다.
	BYTE m_QuestClass;
	BYTE m_QuestIndex;	
};

struct SP_Put_Piece : public SIOCPPacket
{
	SP_Put_Piece() { m_Header.wID[PACKET_HEADER] = SM_PUT_PIECE; }
	BYTE	m_byResult;		// 0:성공 1:끼울자리에 다른거 있어 실패 2:넌 그거 없어
};

struct SP_Succ_Quest : public SIOCPPacket
{
	SP_Succ_Quest() { m_Header.wID[PACKET_HEADER] = SM_SUCC_QUEST; }
	BYTE	m_Quest;	// 다음 퀘스트 클래스		// 0이면 없당
	BYTE	m_NextIndex;	// 다음 퀘스트 인덱스		// 0이면 없당
};

struct SP_Complete_Puzzle : public SIOCPPacket
{
	SP_Complete_Puzzle() { m_Header.wID[PACKET_HEADER] = SM_COMPLETE_PUZZLE; }
	BYTE	m_byResult;		// 0:성공 아이템 생길꺼다 1:잘못 끼운 실패  2:아직 다 못꼈다
	DWORD	m_dwItem;		// 시도했던 아이템
	BYTE	m_byUpgrade;	// 업그레이드 수치
};

struct SP_Use_Necta_Tar : public SIOCPPacket
{
	SP_Use_Necta_Tar() { m_Header.wID[PACKET_HEADER] = SM_USE_NECTA_TAR; }
	long	m_lUnique;		// 모양 변경 될 넘
	BYTE	m_byKind;		// 0:넥타 1:타르
	bool	m_bFlag;		// true : 동작 시작 false : 끝
};

struct SP_Quest_Check : public SIOCPPacket
{
	SP_Quest_Check() { m_Header.wID[PACKET_HEADER] = SM_QUEST_CHECK; }
	BYTE m_QuestClass;		//	뭔지 알지 ^^ 0 이면 시나리오 1이면 event 2  제조 3 보드 
	BYTE m_Case;			// 이건 경우의수 0 npc die 1 pickup item 2 make item 3 meet npc 4는 pickup item drop
	// 5는  make item 중 drop
	DWORD m_Index;			// 
};

struct SP_Change_Gage : public SIOCPPacket
{
	SP_Change_Gage(){ m_Header.wID[PACKET_HEADER] = SM_CHANGE_GAGE; }
	BYTE	m_byType;		// 0 :charge 1:extacy
	int		m_byCurrent;	// 해당케이지의 현재값
};

struct SP_Mode_Change : public SIOCPPacket
{
	SP_Mode_Change(){ m_Header.wID[PACKET_HEADER] = SM_MODE_CHANGE; }
	long	m_lUnique;
	BYTE	m_byType;		// 0 :charge 1:방어 모드 2:걷기 뛰기 3:pk ..기타 등등...전에 쓰던 토글 통합 가능
							// 4 : 칼뽑기
};

struct SP_QuestItem_Del : public SIOCPPacket
{
	SP_QuestItem_Del(){ m_Header.wID[PACKET_HEADER] = SM_QUESTITEM_DEL; }
	DWORD	m_dwIndex;				// 이거 두개만 남겨라
	long	m_lUnique;
};

struct SP_Register_Board : public SIOCPPacket
{
	SP_Register_Board() { m_Header.wID[PACKET_HEADER] = SM_REGISTER_BOARD; }
	bool		m_bRes;		// true : 등록 성공
};

struct SP_Item_Delete : public SIOCPPacket		// 지울꺼
{
	SP_Item_Delete() { m_Header.wID[PACKET_HEADER] = SM_ITEM_DELETE; }
	DWORD	m_dwIndex;
	long	m_lUnique;
};

struct SP_Error_Code : public SIOCPPacket
{
	SP_Error_Code() { m_Header.wID[PACKET_HEADER] = SM_ERROR_CODE; }
	BYTE		m_byErr;			// 약속된 코드
};
struct SP_Error_Msg : public SIOCPPacket
{
	SP_Error_Msg() { m_Header.wID[PACKET_HEADER] = SM_ERROR_MSG; memset( m_ErrMsg, 64, sizeof(char) ); m_bTop = true; }
	bool		m_bTop;				// true 면 가운데...false면 채팅창...기본은 false
	char		m_ErrMsg[64];		// 해당 에러 문자열 -> 48->64증가
};

struct SP_Storage_Change : public SIOCPPacket
{
	SP_Storage_Change() { m_Header.wID[PACKET_HEADER] = SM_STORAGE_CHANGE; }
	DWORD		m_itemidx;			// 바뀔 id	// 돈이면 0:넣고 1:빼고 
	DWORD		m_itemunique;		// 바뀔 유니크 // 돈 
	BYTE		m_Pos[4];			// 0. 기존 tab 1. 이동할 tab 2. 이동할 위치 3. command:0인풋1:아웃풋2:체인지3:돈4:창고시작
};

struct SP_Confirm_Arrow : public SIOCPPacket			// 화살쏜수를 날린다.
{
	SP_Confirm_Arrow() { m_Header.wID[ PACKET_HEADER ] = SM_CONFIRM_ARROW; }
	BYTE		m_Arrow;		//  현재 화살을 쏜수야. 250이 되면 화살통은 없어진당.
};

struct SP_Operator_Mode : public SIOCPPacket			// 화살쏜수를 날린다.
{
	SP_Operator_Mode() { m_Header.wID[ PACKET_HEADER ] = SM_OPERATOR_MODE; }
	BYTE		m_ErrFlag;		//  1 . 운영자가 아니다. 2. etc 나중에 정함
};

struct SP_Character_Info : public SIOCPPacket
{
	SP_Character_Info() { m_Header.wID[ PACKET_HEADER ] = SM_CHARACTER_INFO; }

	int		m_level;			// 캐릭터 렙
	int		m_Chr_Str;			// 힘
	int		m_Chr_Dex;			// 민
	int		m_Chr_End;			// 지구.......콘->지구
	int		m_Chr_Wis;			// 지혜
	int		m_Chr_Bra;			// 용기.......
	int		m_Chr_Fam;			// 명성........
	int		m_Chr_Pos;			// 퍼제션.............

	int		m_Chr_Att;			// 물리 공격력
	int		m_Chr_AC;			// 물리 방어력
	int		m_Chr_Magic_Att;	// 마법 공격력
	int		m_Chr_Magic_AC;		// 마법 방어력.................
	int		m_Chr_Evade_Rate;	// 회피율
	char    CName[12];			// 이름 
};

struct SP_Skill_Upgrade : public SIOCPPacket
{
	SP_Skill_Upgrade() { m_Header.wID[ PACKET_HEADER ] = SM_SKILL_UPGRADE; }
	BYTE	m_bySkill;		// 업그레이드 한 스킬
	BYTE	m_byUpgrade;	// 그 스킬의 업그레이드 수치
};


struct SP_Confirm_Point : public SIOCPPacket
{
	SP_Confirm_Point() { m_Header.wID[PACKET_HEADER] = SM_CONFIRM_POINT; }
	BYTE	m_byType;		// 종...0:스킬포인트
	int		m_nPoint;		// 현재 가지고 있는 포인트양
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
	long	m_lUnique;		// 캐릭터 유니크
	BYTE	m_byBuff;		// 걸리거나 켜지는 버프 번호
	bool	m_bFlag;
};

// 주고받기 신청
struct SP_Try_Itemdeal : public SIOCPPacket
{
	SP_Try_Itemdeal() { m_Header.wID[PACKET_HEADER] = SM_TRY_ITEMDEAL; }
	long	m_send_unique;
	DWORD	m_Itemindex;
	DWORD   m_Itemunique;	
};

// 주고받기 응답
struct SP_Reply_Itemdeal : public SIOCPPacket
{
	SP_Reply_Itemdeal() { m_Header.wID[PACKET_HEADER] = SM_REPLY_ITEMDEAL; }
	long		m_unique;		//
	DWORD		m_Itemindex;
	DWORD		m_Itemunique;	
	BYTE		m_flag;				// 0이면 주는넘 1.이면 받는넘 
	BYTE		m_itempos;			// 아이템 위치 
	_uItemInfo	iteminfo;			// intem info

};

struct SP_Save_Warpzone : public SIOCPPacket
{
	SP_Save_Warpzone() { m_Header.wID[PACKET_HEADER] = SM_SAVE_WARPZONE; }
	BYTE   m_byErr;  // 맵번호
};

struct SP_Item_Upgrade : public SIOCPPacket
{
	SP_Item_Upgrade() { m_Header.wID[PACKET_HEADER] = SM_ITEM_UPGRADE; }
	Item_Info_Save m_UpgradeItem;
};

struct SP_NPC_Use_Magic : public SIOCPPacket
{
	SP_NPC_Use_Magic() { m_Header.wID[PACKET_HEADER] = SM_NPC_USE_MAGIC; }
	int		m_nIndex;		// 인덱스
	DWORD	m_dwUnique;		// 유니크
	BYTE	m_byMotion;		// 동작
};

// hsex = 255면 말이름이 중복 
// hsex = 254면 버그 
// 캐릭유니크가 0이면 마굿간에 있는거. 성별이 255면 없는 정보
struct Horse_Info_Load
{
	DWORD	charunique;		// 캐릭유니크
	char	hname[12];		// 이름
	BYTE	hsex;			// 성별 
	BYTE	hlevel;			// 래밸
	BYTE	hstress;		// 스트레스
	BYTE	hloyal;			// 충성도
	BYTE	hendurance;		// 지구력
	BYTE	helegance;		// 기품
	BYTE	hspirit;		// 발정기
	BYTE	hcolor;			// 색깔 
	BYTE	hhelmet;		// 핼맷 
	BYTE	hbody;			// 몸 색깔
	DWORD	hexp;			// 경험치
	DWORD	htime;			// 망아지 성장 시간
};

struct Horse_Info_Save : public Horse_Info_Load
{
	DWORD accunique;  // 계정유니크 
};

struct SP_Horse_Name : public SIOCPPacket
{
	SP_Horse_Name() { m_Header.wID[PACKET_HEADER] = SM_HORSE_NAME; }
	Horse_Info_Save horsedata;
	//BYTE hsex;		// 성별  255 : 이름이 같다 254 인밴에 빈자리가 없다. 	
	//BYTE hLevel;	// 말의 래밸
	//char m_HorseName[12];		// 이름 
	//BYTE m_Type;	//타입 0 .타고 1. 마굿간
};

// 12-3
struct SP_Status_Change : public SIOCPPacket
{
	SP_Status_Change() { m_Header.wID[PACKET_HEADER] = SM_STATUS_CHANGE; }
	int		m_nIndex;	// 유저라면 -1, npc라면 인덱스
	long	m_lUnique;	// 유니크..해당 유니크
	BYTE	m_byStatus;	// 해당 상태...base.h 참조

	
};

struct SP_Horse_Status : public SIOCPPacket
{
	SP_Horse_Status() { m_Header.wID[PACKET_HEADER] = SM_HORSE_STATUS; }	
	DWORD	m_CharUnique;
	DWORD	m_Status;	// 0칼라, 1헬멧, 2바디, 3?
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
	 DWORD byVer;  // 버전 채크 
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
	 DWORD m_dwCharUnique; // 지우고자 하는 캐릭터 유니크 
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

struct LSP_Character_Storage : public SIOCPPacket  // 클에게 창고 정보를 보냄 
{
	 LSP_Character_Storage() {m_Header.wID[PACKET_HEADER] = LSM_CHARACTER_STORAGE;}
	 Char_info_Tab_Storage	m_Storage;
	 DWORD					m_Cash;
};


struct LSP_Horse_Load : public SIOCPPacket  // 클에게 말 정보를 보냄 
{
	LSP_Horse_Load() {m_Header.wID[PACKET_HEADER] = LSM_HORSE_LOAD;}
	Horse_Info_Load		m_Horse;
	BYTE				m_count;
};

// MP messages for server com
//서버 간 접속시 사용하는 페킷
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



// info server -> zoneserver 로 client 의 로긴을 대기하라는 메세지
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

struct MP_Shout_Chat:public SIOCPPacket			// 유저 외치기시..모든 존서버루 날리는 패킷
{
	MP_Shout_Chat() { m_Header.wID[PACKET_HEADER] = SSM_SHOUT_CHAT; }
	char		m_from[16];
	char		m_msg[64];
};

struct MP_Warp_Request:public SIOCPPacket
{
	MP_Warp_Request() { m_Header.wID[PACKET_HEADER] = SSM_WARP_REQUEST; }
	long		m_lUnique;
	int			m_Map_ID;		// 워프할 맵번호
	int			m_WarpIndex;	// 워프한 맵에서 나타날 좌표는 해당맵의 index의 좌표
	int			m_floor;		// 층..항상 0
	Char_Info_All m_IncomingCharacter;
};

struct MP_Warp_Reply: public SIOCPPacket
{
	MP_Warp_Reply() { m_Header.wID[PACKET_HEADER] = SSM_WARP_REPLY; }
	long		m_lUnique;
	int			m_Map_ID;		// 워프할 맵번호
	DWORD		m_Pos;			// 워프한 맵에서 나타날 좌표
	int			m_floor;		// 층..항상 0
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
	BYTE   m_byWarpID;  // 지정 워프존
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
	BYTE	m_byResult;		// 0 : 성공		1 : 돈부족 실패		2 : 기타 실패(레벨제한이나 그런거 )		3 : 합체시 텔 못함
};

*/



#endif
