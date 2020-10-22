
#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <memory.h>

// PC, NPC 종족 분류...몹, 아이템, 상인, 퀘스트...
const int c_PcRace_User( 0 );				// 다른 유저들 
const int c_PcRace_Illusion( 1 );			// 분신 스킬에 사용시 생성된 환영들 

const int c_NpcRace_Mob( -1 );				// 몬스터 및 상인 등 
const int c_NpcRace_Item( -2 );				// 아이템 
const int c_NpcRace_Transpotation( -3 );	// 운송 수단 

const int c_NpcIndex_Item( 1 );				// 보따리!!!...


//-----------------------------------------------------------------------------
// Desc: 종족
//-----------------------------------------------------------------------------
enum Skill_Race
{
	UNION = 0,
	DADE,
	JENI,
};


//-----------------------------------------------------------------------------
// Desc: 직업
//-----------------------------------------------------------------------------
enum Skill_Job
{
	NOVICE = 0,		// 무직
	KNIGHT,			// 기사
	ARCHER,			// 궁수
	MAGICIAN,		// 마법사
	FAIRY,			// 페어리
	ELIADEN,		// 엘리아덴
	ROIEL,			// 로이엘
};


//-----------------------------------------------------------------------------
// Desc: 퀘스트
//-----------------------------------------------------------------------------
enum QUEST_CHECK_TYPE
{
	QUEST_NPC_DIE = 0,
	QUEST_PICK_ITEM,
	QUEST_MAKE_ITEM,
	QUEST_MEET_NPC,
	QUEST_DROP_ITEM,
	QUEST_DROP_MAKEITEM,
};


//-----------------------------------------------------------------------------
// Desc: 모드
//-----------------------------------------------------------------------------
enum MODE_TYPE
{
	MODE_CHARGE = 0,	// 0 차지
	MODE_DEFENSE,		// 1 디펜스
	MODE_MOVE,			// 2 걷기-뛰기
	MODE_PK,			// 3 PK
	MODE_ATTACK,		// 4 attack상태
	MODE_DOWN,			// 5 앉기-일어나기
	MODE_OPERATOR,		// 6 운영자
	MODE_PARTYOK = 10,	// 10 파티 거부 
	MODE_TRADEOK,		// 11 교환 거부
	MODE_POSSESSIONOK,	// 12 합체 거부
};


//-----------------------------------------------------------------------------
// Desc: 무기링크
//-----------------------------------------------------------------------------
enum WEAPON_TAKE_INDEX
{
	TAKE_RHAND = 0,		// 오른손
	TAKE_LHAND,			// 왼손
	TAKE_ETC1,			// 기타
	TAKE_ETC2,			// 기타2
};


//-----------------------------------------------------------------------------
// Desc: 기타아이템
//-----------------------------------------------------------------------------
enum ETC_ITEM_INDEX
{
	ETC_NECTA = 1,		// 넥타
	ETC_TAR,			// 타르
	ETC_CHAIR,			// 의자
};


//-----------------------------------------------------------------------------
// Desc: 텍스쳐속성
//-----------------------------------------------------------------------------
enum DATA_ATTR
{
	DATA_DELETE0 = 0,	// 삭제가능 텍스쳐
	DATA_DELETE1,		// 삭제가능 텍스쳐
	DATA_NON_DELETE,	// 삭제불가 텍스쳐
};


//-----------------------------------------------------------------------------
// Desc: 소팅타입
//-----------------------------------------------------------------------------
enum SORT_TYPE
{
	SORT_NPC = 0,	// NPC
	SORT_PC,		// PC
	SORT_OBJ,		// OBJ
};


//-----------------------------------------------------------------------------
// Desc: 디스플레이타입
//-----------------------------------------------------------------------------
enum DISPLAY_TYPE
{
	DISPLAY_NORMAL = 0,	// 일반
	DISPLAY_SELECT,		// 선택창
	DISPLAY_MAKE,		// 생성창
};


//-----------------------------------------------------------------------------
// Desc: 포션타입
//-----------------------------------------------------------------------------
enum POTION_TYPE
{
	USE_HP_POTION = 0,	// 힐링포션	0
	USE_MP_POTION,		// 마나포션	1
	USE_NECTA,			// 넥타		2
	USE_TAR,			// 타르		3
	USE_HORSE_FOOD,		// 건초		4
};


//-----------------------------------------------------------------------------
// Desc: 스킬, 전직시 사용 
//-----------------------------------------------------------------------------
#define LEARN_SKILL					253		// 스킬 배울때는,넣어서 쏜다. 전직시에는 기존과 같이 전직하고자 하는 직업 번호
#define LEARN_SKILL_SUC 			254		// 성공
#define LEARN_SKILL_FAIL			255		// 실패
#define CHANGE_CLASS_SUC			251		// 전직 성공
#define CHANGE_CLASS_FAIL			252		// 전직 실패

/*
// 기사스킬
#define		KNIGHT_SKILL_0				0	// 데스블로우		
#define		KNIGHT_SKILL_1				1	// 레스트체어		
#define		KNIGHT_SKILL_2				2	// 슬랩블로우		
#define		KNIGHT_SKILL_3				3	// 디펜스마스터리	
#define		KNIGHT_SKILL_4				4	// 파워슬래쉬		
#define		KNIGHT_SKILL_5				5	// 체인어택		
#define		KNIGHT_SKILL_6				6	// 라이딩			
#define		KNIGHT_SKILL_7				7	// 레드아이		
#define		KNIGHT_SKILL_8				8	// 파워샷			
#define		KNIGHT_SKILL_9				9	// 더블샷			
#define		KNIGHT_SKILL_10				10	// 아이스애로우	
#define		KNIGHT_SKILL_11				11	// 파이어애로우	
#define		KNIGHT_SKILL_12				12	// 파워블로우		
#define		KNIGHT_SKILL_13				13	// 파워스트라이크	
#define		KNIGHT_SKILL_14				14	// 더블클랭		

// 페어리스킬
#define		FAIRY_SKILL_100				100		// 트윈클				
#define		FAIRY_SKILL_102				102		// 슬러프				
#define		FAIRY_SKILL_103				103		// 레스트플라워		
#define		FAIRY_SKILL_104				104		// 스트렝스	
#define		FAIRY_SKILL_105				105		// 서펜트				
#define		FAIRY_SKILL_106				106		// 블래스트		
#define		FAIRY_SKILL_107				107		// 블래스				
#define		FAIRY_SKILL_108				108		// 아이스볼트		
#define		FAIRY_SKILL_109				109		// 워터스트라이크		
#define		FAIRY_SKILL_111				111		// 익스플로젼			
#define		FAIRY_SKILL_112				112		// 버닝플레어		
#define		FAIRY_SKILL_119				119		// 파이어볼		
#define		FAIRY_SKILL_126				126		// 아이스피럴			
*/
#define		HUMAN_SKILL_0				0		// 라이딩				X

#define		KNIGHT_SKILL_1				1		// 디펜스 마스터리		X
#define		KNIGHT_SKILL_2				2		// 파워블로우			X
#define		KNIGHT_SKILL_3				3		// 쉴드어택
#define		KNIGHT_SKILL_4				4		// 파워슬래쉬			X
#define		KNIGHT_SKILL_5				5		// 홀리어택

#define		ARCHER_SKILL_50				50		// 백스텝				X
#define		ARCHER_SKILL_51				51		// 파워샷				X
#define		ARCHER_SKILL_52				52		// 스텔스				X
#define		ARCHER_SKILL_53				53		// 에어웨이브
#define		ARCHER_SKILL_54				54		// 소울스트링
#define		ARCHER_SKILL_55				54		// 파워스트라이크

#define		FAIRY_SKILL_100				100		// 트윈클	( 스킬로 사용 안하는 기본 스킬 )	X
#define		FAIRY_SKILL_101				101		// 슬러프				X

#define		ELLIADEN_SKILL_102			102		// 파이어볼				X
#define		ELLIADEN_SKILL_103			103		// 아이스피럴			X
#define		ELLIADEN_SKILL_104			104		// 서펜트
#define		ELLIADEN_SKILL_105			105		// 블래스
#define		ELLIADEN_SKILL_106			106		// 스윈들

#define		ROIEL_SKILL_150				150		// 라이트닝쉴드
#define		ROIEL_SKILL_151				151		// 라이트닝볼트
#define		ROIEL_SKILL_152				152		// 리스토레이션
#define		ROIEL_SKILL_153				153		// 슬립
#define		ROIEL_SKILL_154				154		// 트랜스폼
#define		ROIEL_SKILL_155				155		// 차밍페어리
#define		ROIEL_SKILL_156				156		// 스트랭스

//-----------------------------------------------------------------------------
// Desc: 비트연산
//-----------------------------------------------------------------------------
#define		HIGH_4BIT				240
#define		LOW_4BIT				15


//-----------------------------------------------------------------------------
// Desc: 문자열길이
//-----------------------------------------------------------------------------
#define		STR_LINE_64				64
#define		STR_LINE_128			128	
#define		STR_LINE_256			256
#define		BYTE_FAIL				255

#define		TAR_ITEM_INDEX			90				// 타르아이템
#define		NECTA_ITEM_INDEX		91				// 넥타타이템
#define		CHAIR_ITEM_INDEX		92				// 의자아이템
#define		TREE_ITEM_INDEX			97				// 나무아이템
#define		TAR_NECTA_CANCEL		10				// 타르, 넥타취소 할때 저장값

//-----------------------------------------------------------------------------
// Desc: 기타...
//-----------------------------------------------------------------------------
#define		EPSILON					0.0002f			// 소수점 비교
#define		ANGLE					0.01745f		// 회전각. 라디안
#define		ANGLE_SPEED				4				// 회전속도
#define		CLIENT_TILESIZE			8				// 타일 크기
#define		LOD_RANGE				100				// LOD거리(기본값에 더해서 쓴다)


//-----------------------------------------------------------------------------
// Desc: 아이템배열
//-----------------------------------------------------------------------------
#define		HEAD_INDEX				5				// 애니메이션 노드에서 머리 인덱스
#define		HAIR_INDEX				4				// 머리카락
#define		FACE_INDEX				3				// 얼굴


//-----------------------------------------------------------------------------
// Desc: 아이템링크 정보
//-----------------------------------------------------------------------------
#define		WEAPON_READYLINK		49				// 칼링크되는본 
#define		SHIELD_READYLINK		51				// 방패링크되는본
#define		LINK_ITEM_COUNT			4				// 링크되는 갯수


//-----------------------------------------------------------------------------
// Desc: 맵블럭
//-----------------------------------------------------------------------------
#define		CLEAR_BLOCK				0				// CLEAR
#define		OBJ_BLOCK				1				// OBJ
#define		NPC_BLOCK				12				// NPC
#define		PC_BLOCK				13				// PC
#define		ITEM_BLOCK				14				// ITEM


//-----------------------------------------------------------------------------
// Desc: 칼검기
//-----------------------------------------------------------------------------
#define		MAX_TRACE_COUNT			256				// 실제 최대 커브점갯수 : TOTAL_POINTS * TRACE_STEP
#define		TOTAL_POINTS			64				// 애니메이션 되는점 : 30
#define		TRACE_STEP				4				// 애니메이션 사이의 면갯수


//-----------------------------------------------------------------------------
// Desc: 아이템 인덱스
//-----------------------------------------------------------------------------
#define		ITEM_CAEM_TYPE			1				// 카엠 아이템 타입
#define		ITEM_POTION_TYPE		13				// 포션 아이템 타입
#define		ITEM_HORSE_PAPER		268476195		// 말증서

//-----------------------------------------------------------------------------
// Desc: 캐릭터구성 아이템, 버텍스갯수
//-----------------------------------------------------------------------------
#define		MAX_PC_ITM				6				// pc아이템갯수
#define		MAX_NPC_ITM				2				// npc아이템갯수
#define		MAX_PC_VTX				1280			// pc버텍스버퍼크기
#define		MAX_NPC_VTX				2560			// npc버텍스버퍼크기


//-----------------------------------------------------------------------------
// Desc: 애니메이션 
//-----------------------------------------------------------------------------
#define		NPC_ANI_INDEX_COUNT		30				// NPC 애니메이션 갯수 
#define		PC_ANI_INDEX_COUNT		150				// PC  애니메이션 갯수 
#define		PCATTACK_SPEED			74.0f			// 유저 공격
#define		ANI_TIME				84.0f			// 기본 애니메이션(0.084초)


//-----------------------------------------------------------------------------
// Desc: 오브젝트 인덱스
//-----------------------------------------------------------------------------
#define		CASTTLE_DOOR			202				// 성문
#define		MOUSEPOINT_INDEX		500				// 마우스포인터


//-----------------------------------------------------------------------------
// Desc: PC 인덱스
//-----------------------------------------------------------------------------
#define		UNION_MAN				997				// 유니온 남자
#define		UNION_WOMAN				998				// 유니온 여자
#define		UNION_FAIRY				999				// 유니온 요정


//-----------------------------------------------------------------------------
// Desc: NPC 인덱스
//-----------------------------------------------------------------------------
#define		NPC_RACE				-991
#define		LIVEITEM_RACE			-992
#define		LIVEITEM_INDEX			900				// 라이트아이템
//#define		DESTROY_OBJECT_INDEX	911				// 파괴오브젝트(통나무)

//-----------------------------------------------------------------------------
// Desc: 말 인덱스
//-----------------------------------------------------------------------------
#define		RED_HORSE				995				// 적마
#define		WHITE_HORSE				919				// 백마
#define		BLACK_HORSE				920				// 흑마
#define		STID_INDEX				996				// 스티트(npc종족)
#define		HORSE_RACE				-993				// 말구분(npc종족)


//-----------------------------------------------------------------------------
// Desc: 시간
//-----------------------------------------------------------------------------
#define		NPC_UNDERATTACK_TIME	 500.0f			// 맞아있는 시간 (0.5초)
#define		NPC_DIE_TIME			5000.0f			// 죽어있는 시간 (5초)
#define		NPC_DIE_BLENDTIME		1500.0f			// 블렌딩   시간 (1.5초)
#define		NPC_RESPAWN_BLENDTIME	 500.0f			// 생성블렌딩시간(0.5초)
#define		FAIRYUNITE_TIME			3000.0f			// 페어리 합체시간(3초)
#define		HEAD_ROTATE_TIME		 200.0f			// 머리회전하는시간(0.2초)


//-----------------------------------------------------------------------------
// Desc: 거리 지정
//-----------------------------------------------------------------------------
#define		XBLOCK_RANGE			32				// NPC 처리하는범위
#define		MAX_ITEM_RANGE			1				// 아이템선택 타일거리
#define		MAX_RUN_RANGE			2				// 걷기, 뛰기 선택거리


//-----------------------------------------------------------------------------
// Desc: 공격처리
//-----------------------------------------------------------------------------
#define		ATTACKTYPE_A			0			// 공격타입1
#define		ATTACKTYPE_B			1			// 공격타입2
#define		ATTACKTYPE_C			2			// 공격타입3

struct SAImageData
{
	int n_end_frame;	
	int n_blend_frame;
	int n_start_blend_frame;
	int n_ani_frame;
	int n_ani_number;
	int n_pre_end_frame;
	int n_pre_ani_number;
	float f_middle_time;

	float fAlpha;

	bool IsAfterWImage;

	struct SfvPos
	{
		float x;
		float y;
		float z;
	};

	struct SfDiffuse
	{
		float r;
		float g;
		float b;
	};

	SfvPos sPos;
	SfDiffuse sDiffuse;

	SAImageData()
	{
		memset(this, 0, sizeof(SAImageData));
	}

	SAImageData(const SAImageData & temp)
	{
		memset(this, 0, sizeof(SAImageData));
		
		n_end_frame = temp.n_end_frame;		
		n_blend_frame = temp.n_blend_frame;
		n_start_blend_frame = temp.n_start_blend_frame;
		n_ani_frame = temp.n_ani_frame;
		n_ani_number = temp.n_ani_number;
		n_pre_end_frame = temp.n_pre_end_frame;
		n_pre_ani_number = temp.n_pre_ani_number;
		
		f_middle_time = temp.f_middle_time;

		fAlpha = temp.fAlpha;

		IsAfterWImage = temp.IsAfterWImage;
		
		sPos.x = temp.sPos.x;
		sPos.y = temp.sPos.y;
		sPos.z = temp.sPos.z;

		sDiffuse.r = temp.sDiffuse.r;
		sDiffuse.g = temp.sDiffuse.g;
		sDiffuse.b = temp.sDiffuse.b;
	}

	void operator=(const SAImageData & temp)
	{
		n_end_frame = temp.n_end_frame;
		n_blend_frame = temp.n_blend_frame;
		n_start_blend_frame = temp.n_start_blend_frame;
		n_ani_frame = temp.n_ani_frame;
		n_ani_number = temp.n_ani_number;
		n_pre_end_frame = temp.n_pre_end_frame;
		n_pre_ani_number = temp.n_pre_ani_number; 

		f_middle_time = temp.f_middle_time;

		fAlpha = temp.fAlpha;

		IsAfterWImage = temp.IsAfterWImage;
		
		sPos.x = temp.sPos.x;
		sPos.y = temp.sPos.y;
		sPos.z = temp.sPos.z;

		sDiffuse.r = temp.sDiffuse.r;
		sDiffuse.g = temp.sDiffuse.g;
		sDiffuse.b = temp.sDiffuse.b;
	}	
};

#endif