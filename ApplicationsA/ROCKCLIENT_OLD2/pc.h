#ifndef		_____PC_____
#define		_____PC_____

#include	"brother.h"
#include	<OBJ\\ObjBase.H>
#include	<OBJ\\Player.H>
#include	<OBJ\\NonPlayer.H>
#include	<Map\Field.h>
#include	"Map.H"
#include	"bravolibs\\effect\\CDamageEffect.h"		// 데미지 이펙트 처리 부분 by wxywxy
#include	"Bravolibs\\Obj\\SkillSystem.h"
#include	<list>

using namespace std;
 

#define		MAX_USER_INTABLE			128			// 유저테이블
#define		MAX_ID_STRLEN				10			// 아이디
#define		MAX_PASS_STRLEN				10			// 비밀번호
#define		MAX_CHARACTER_COUNT			3			// 캐릭터

#define		MAX_NPC_SPPECH_COUNT		10			// NPC 말 종류
#define		QUEST_MAX_COUNT				5			// 퀘스트 종류
#define		MAX_TEMP_STATE				16			// 임시상태저장

//-----------------------------------------------------------------------------
// Desc: 유저상태
//-----------------------------------------------------------------------------
#define		NONE_SKILL		0	// 0
#define		STELTH			1	// 1
#define		AIRWAVE			2	// 2
#define		DIEACTION		3	// 3
#define		SLEEP			4	// 4
#define		TRANSFORM		5	// 5
#define		STUN			6	// 6
/*
enum CHR_STATUS
{
	NONE_SKILL	= 0,	// 0
	STELTH,				// 1
	AIRWAVE,			// 2
	DIEACTION,			// 3
	SLEEP,				// 4
	TRANSFORM,			// 5
	STUN,				// 6
};
*/
	

//-----------------------------------------------------------------------------
// Desc: 상인타입
//-----------------------------------------------------------------------------
enum	MERCHANT_TYPE
{
	MAGIC_MERCHANT = 3,		// 마법	3
	WEAPON_MERCHANT,		// 무기	4
	STORE_MERCHANT,			// 창고	5
	GUILD_MASTER,			// 길드	6
	HORSE_MERCHANT,			// 말	7
};


//-----------------------------------------------------------------------------
// Desc: 운영자용 시스템 명령어 타입
//-----------------------------------------------------------------------------
enum OPERATOR_MSG_TYPE
{
	OPT_MSG_BASIC = 0,	// 기본
	OPT_MSG_POS,		// 맵아이디, X-Y 좌표
	OPT_MSG_COIN,		// 돈설정
	OPT_MSG_TALK,		// 채팅 on, off
};


//-----------------------------------------------------------------------------
// Desc: 검기, 그림자등의 기타 잡다한 텍스쳐모음
//-----------------------------------------------------------------------------
struct	EffectTexture
{
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	pTex;
#else
	LPDIRECT3DTEXTURE8	pTex;
#endif // DIRECT_VERSION_9_MJH

	EffectTexture()		{ Clear(); }
	~EffectTexture()	{ Destroy(); }
	void	Clear() { pTex = NULL; }
	void	Destroy()
	{
		SAFE_RELEASE( pTex );	
	}
};

struct	TextureTable
{			
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9			pAfterImage1;	// 검기(일반)
	LPDIRECT3DTEXTURE9			pAfterImage2;	// 검기(불)
	LPDIRECT3DTEXTURE9			pAfterImage3;	// 검기(얼음)
	LPDIRECT3DTEXTURE9			pShadow;		// 그림자			
	LPDIRECT3DTEXTURE9			pImoticon;		// 이모티콘	
	LPDIRECT3DTEXTURE9			pQuest;			// 퀘스트창
#else
	LPDIRECT3DTEXTURE8			pAfterImage1;	// 검기(일반)
	LPDIRECT3DTEXTURE8			pAfterImage2;	// 검기(불)
	LPDIRECT3DTEXTURE8			pAfterImage3;	// 검기(얼음)
	LPDIRECT3DTEXTURE8			pShadow;		// 그림자			
	LPDIRECT3DTEXTURE8			pImoticon;		// 이모티콘	
	LPDIRECT3DTEXTURE8			pQuest;			// 퀘스트창
#endif // DIRECT_VERSION_9_MJH
	
	void	Clear()
	{						
		pAfterImage1	= NULL;
		pAfterImage3	= NULL;
		pAfterImage2	= NULL;				
		pShadow			= NULL;
		pImoticon		= NULL;
		pQuest			= NULL;
	}
	void	Destroy()
	{		
		SAFE_RELEASE( pAfterImage1 ); 	
		SAFE_RELEASE( pAfterImage2 ); 	
		SAFE_RELEASE( pAfterImage3 ); 							
		SAFE_RELEASE( pShadow ); 	
		SAFE_RELEASE( pImoticon ); 
		SAFE_RELEASE( pQuest ); 	
		Clear();
	}
	void	Create();	
};


//-----------------------------------------------------------------------------
// Desc: 선택박스 
//-----------------------------------------------------------------------------
struct	SelectBox
{	
	bool		bPC;
	bool		bDisplay;
	int			index;
	BYTE		CurrentHP;
	
	void	Clear() 
	{ 		
		bPC			= false;		
		index		= -1;		
		bDisplay	= false;
		CurrentHP	= 100;
	}
	void	Destroy() 
	{ 		
		Clear();
	}			
};


//-----------------------------------------------------------------------------
// Desc: 그림자 테이블
//-----------------------------------------------------------------------------
struct	ShadowTable
{
	D3DXMATRIX	matTiny;
	D3DXMATRIX	matSmall;
	D3DXMATRIX	matMiddle;
	D3DXMATRIX	matBig;
	D3DXMATRIX	matLarge;	
	
	void	Clear();
};


//-----------------------------------------------------------------------------
// 유저 상태
//-----------------------------------------------------------------------------
/*
enum	Character_Status
{
	CHR_ALIVE = 0,		// 일반 상태
	CHR_REST,			// 휴식 상태
	CHR_PASSIVE,		// 패시브스킬이 걸린 상태
	CHR_MAGIC,			// 보조마법 상태
	CHR_POISON,			// 독에 걸린 상태
	CHR_STUN,			// 스턴
	CHR_DEATH,			// 죽은 상태
	CHR_WARP,			// 워프중인 상태...이 상태에서는 날라오는 모든 메세지 씹기...2-19
};
*/

//-----------------------------------------------------------------------------
// Desc: 유저 테이블
//-----------------------------------------------------------------------------
struct		PCTABLE
{	
	int				nIndex;
	long			lUnique;	
	long			lBlock;	
	long			lDestBlock;	
	long			lCurMap;
	BYTE			nStatus;	
	BYTE			nCurrentFloor;
	bool			bRender;
	long			lPreTime;
	Char_Info		char_info;
	int				nFlag;
	DWORD		    theRidingUID;

	PCTABLE()		{ Init(); }

	void		Init()
	{
		nIndex			= -1;
		lUnique			= -1;
		lBlock			= -1;		
		lDestBlock		= -1;
		nStatus			= 0;		
		nCurrentFloor	= 0;
		bRender			= true;
		nFlag			= 0;
		ZeroMemory( &char_info, sizeof( Char_Info ) );
		theRidingUID    =  0;
	}
};


//-----------------------------------------------------------------------------
// Desc: 이모티콘
//-----------------------------------------------------------------------------
struct IMOTICON_TABLE 
{
	int		index;		// 인덱스.. 유저는 -1이다
	DWORD	unique;		// 유니크
	DWORD	time;		// 끝나는 시간
	int		type;		// 이모티콘 종류.. -1이면 없다 

	IMOTICON_TABLE()	{ Clear(); }
	void	Clear() { index = -1; unique = 0; time = 0; type = -1; }
};


//-----------------------------------------------------------------------------
// Desc: 퀘스트정보
//-----------------------------------------------------------------------------
#define		QUEST_TITLE			64		// 한글 32
#define		QUEST_OFFER			32		// 한글 16
#define		QUEST_STORY			512		// 한글 256
#define		QUEST_OBJECT		128		// 한글 64
#define		QUEST_GIFT			128		// 한글 64

#define		QUEST_LONG_LINE		20		// 20줄
#define		QUEST_MEDIUM_LINE	10		// 10줄
#define		QUEST_SHORT_LINE	5		// 5줄

struct	CHAR_LINE
{
	char*	buff;	
	
	CHAR_LINE()		{ buff = NULL; }
	~CHAR_LINE()	{ Destroy(); }	
	void	Create( int size )
	{
		buff = SAFE_NEW_ARRAY( char , size );		
		memset( buff, 0, sizeof(buff) ); 
	}
	void	Destroy()
	{
		SAFE_DELETE_ARRAY( buff );
	}
};

struct	QUEST_TEXT
{
	char		title[QUEST_TITLE];		// 제목
	char		offer[QUEST_OFFER];		// 제공자	

	// 내용
	BYTE		SubjectCurLine;
	BYTE		SubjectMaxLine;
	CHAR_LINE	Subject[QUEST_LONG_LINE];

	// 목적
	BYTE		ObjectCurLine;
	BYTE		ObjectMaxLine;
	CHAR_LINE	Object[QUEST_SHORT_LINE];

	// 보상
	BYTE		GiftCurLine;
	BYTE		GiftMaxLine;
	CHAR_LINE	Gift[QUEST_SHORT_LINE];	
};



struct TEMP_TEXT
{
	char	title[QUEST_TITLE];
	char	offer[QUEST_OFFER];		
	char	story[QUEST_STORY];
	char	object[QUEST_OBJECT];
	char	gift[QUEST_GIFT];	
};

enum	QUEST_CLASS
{
	QUEST_SCENARIO = 0,		// 시나리오
	QUEST_EVENT,			// 이벤트
	QUEST_MAKE,				// 제조
	QUEST_BOARD,			// 보드
	QUEST_TASK,				// 태스크
};

struct	QUEST_INFO
{	
	bool			bInit;		// 초기화
	QUEST_TABLE		table;		// 퀘스트
	QUEST_TEXT		text;		// 텍스트	
	
	QUEST_INFO()	{ Clear(); }
	void	Clear()	{ bInit = false; }
	void	SetQuestData( int index );	
};

//-----------------------------------------------------------------------------
// Desc: 맵정보(이름) 
//-----------------------------------------------------------------------------
#define	MAP_COUNT		10		// 맵갯수
#define	MAPID_LENGTH	20		// 맵이름길이(한글10자)

struct	MAP_INFO
{
	CHAR_LINE	id[MAP_COUNT];	
};


struct Item_Info_Save;
union _uItemType;

//-----------------------------------------------------------------------------
// Desc: 유저처리
//-----------------------------------------------------------------------------
class		CBravoPC
{
/// sooree network
///-------------------------------------------------------------------------------------
///	Rock PC Data : 기존의 데이터 관리 부분은 일단 관두고 우리 데이터로 새로 받아와서 처리한다. 
///-------------------------------------------------------------------------------------
public:
//	SRpdPCData	theMyPCData;

//	inline	SRpdPCData*	GetMyPCData()							{	return &theMyPCData;		}
//	inline	void		SetMyPCData( SRpdPCData* aSRpdPCData )	{	theMyPCData = *aSRpdPCData;	}

	CSkillSystem		theSkillSystem;
	CSkillSystem*		GetSkillSystem()	{	return( &theSkillSystem );		}

/****************************************************************************************************
	* Network Process & Update
****************************************************************************************************/
public:
	char		m_szID[MAX_ID_STRLEN];
	char		m_szPass[MAX_PASS_STRLEN];
	
	PCTABLE		m_MainPC;						/* Main PC */
	PCTABLE		m_TempPC;
	PCTABLE		m_PcTable[MAX_USER_INTABLE];	/* Around Main PC */
	
	//-----------------------------------------------------------------------------
	// Desc: 장비 정보
	//-----------------------------------------------------------------------------
	// 현재장착장비	
//	Char_Item		m_Equip[MAX_EQUIPED];		
//	Char_Item*		GetEquip()					{ return m_Equip; }	
	
	// 갑옷, 무기등
//	Char_Item		m_Tool_Inventory1[MAX_INVENTORY1];		
//	Char_Item*		GetToolInventory1()			{ return m_Tool_Inventory1; }

public:
	/******	COMMON ******/
	PCTABLE		GetTempPc()						{ return m_TempPC; };
	void		SetupTempPc(PCTABLE &pc)		{ m_TempPC	= pc; };
	void		InitPC(PCTABLE& pc);

	/******	MAIN PC ******/
	PCTABLE		GetMainPC()						{ return m_MainPC; };
	void		SetupUnique(long id)			{ m_MainPC.lUnique = id; }
	long		GetMainPCUniqe()				{ return m_MainPC.lUnique; }
	void		SetupMainPc(PCTABLE& pc);
	void		SetupMainPcFloor(int floor)		{ m_MainPC.nCurrentFloor = (BYTE)floor; }
	void		CreateChracterItem( Item_Info_Save* pInventory1, Item_Info_Save* pEquip );
	bool		FindChracterItem( int Index, DWORD ItemIdx );

	// 캐릭터속성저장
	int			SetCharacterAttr( PCTABLE _Table );
	int			SetCharacterAttr( int _nJob, int _nRace, int _nSex );

	/******	PCs In MAIN PC TABLE ******/
	void		InitTable();
	int			FindUser(long unique);
	void		CleanupPcTable();	
	long		GetPcPos(long unique);
	char*		GetPcName(long unique);
	char*		GetPcName(int index);
	long		GetNameToUnique(char* szName);
	int			GetPcSex(long unique);	

	void		SM__CHANGE__ITEM( long unique, DWORD item_id, DWORD item_unique, DWORD item_pos, BYTE pos );	// 아이템 바꾸기
	void		SM__TAKEOFF__ITEM( long unique, DWORD item_id, DWORD item_unique );
	void		SM__APPEAR( long unique, PCTABLE& pc );															// 유저 생성 

	int			AppearPC( long unique, PCTABLE& pc );	/// sooree network
	int 		DisappearPC( DWORD aPCID );	/// sooree network
	
	//by simwoosung
	HRESULT		InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT		RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	//-----------------------------------------------------------------------------
	// Desc: 유저처리
	//-----------------------------------------------------------------------------
	// 캐릭터 생성
	/*
	void		SM__APPEAR( PCTABLE& pc, char szHorseName[12], DWORD type, bool bUnite, long unique, bool bAttackMode, 
							bool bCharge, bool bNecta, bool bTar, bool bPK, bool bWalk, bool bSword, bool bDown, BYTE status );

	void		SM__DISAPPEAR( long unique );																	// 캐릭터 삭제
	void		SM__MOVE__BROADCAST( PCTABLE& pc, DWORD Path, bool bUnite, long unique, bool bCurrent );		// 캐릭터 이동
	void		SM__USER__DIE( long unique );																	// 캐릭터 사망
	void		SM__ATTACK__NPC( long unique, DWORD npcunique, DWORD dmg, int motion, DWORD pos, bool critic );	// 공격메세지 받기
	void		SM__CHR__DIR( DWORD unique, BYTE dir );
	void		SM__CHARACTER__INFO( SP_Character_Info* pInfo );

	//-----------------------------------------------------------------------------
	// Desc: 모드수정
	//-----------------------------------------------------------------------------
	void		SM__MODE__CHANGE( long unique, BYTE type );
	void		CM__MODE__CHANGE( BYTE type );
	void		SM__USE__NECTA__TAR( long unique, BYTE kind, bool bFlag );	
	void		CM__USE__NECTA__TAR( BYTE type, bool bCancel = false );
	void		SM__CHANGE__GAGE( BYTE type, int cur );	
	void		SM__OPERATOR__MODE( BYTE err );

	//-----------------------------------------------------------------------------
	// Desc: 퀘스트
	//-----------------------------------------------------------------------------
	void		SM__GET__QUEST( BYTE err, BYTE Class, BYTE index );				// 퀘스트받기
	void		SM__SUCC__QUEST( BYTE Class, BYTE index );						// 퀘스트성공여부
	void		SM__QUEST__CHECK( BYTE Class, BYTE Case, DWORD index );			// 현재퀘스트검사하기

	//-----------------------------------------------------------------------------
	// 전직, 레벨업
	//-----------------------------------------------------------------------------
	void		SM__CHANGE__CLASS( BYTE job );
	void		SM__LEVEL__UP( long lUnique, int nLevel, int nPoint );
	
	//-----------------------------------------------------------------------------
	// 상태 변화	
	//-----------------------------------------------------------------------------
	void		SM__TRY__UNITE( long unique );
	void		CM__TRY__UNITE();
	void		SM__REPLY__UNITE( long human_unique, long fairy_unique );
	void		CM__SEPERATE__UNIT();
	void		SM__SEPERATE__UNIT( long human_unique, long fairy_unique, DWORD pos );	
	void		CM__TOGGLE__RUN( bool bWalk );	
	void		SM__CHARACTER__STATUS( DWORD unique, BYTE status, WORD time );	
	void		SM__STATUS__CHANGE( int Index, long lUnique, BYTE Status );
	
	//-----------------------------------------------------------------------------
	// 아이템
	//-----------------------------------------------------------------------------
	void		SM__CHANGE__ITEM( long unique, DWORD item_id, DWORD item_unique, DWORD item_pos, BYTE pos );	// 아이템 바꾸기
	void		SM__TAKEOFF__ITEM( long unique, DWORD item_id, DWORD item_unique );					// 아이템 벗기		
	void		SM__DROP__ITEM( DWORD id, DWORD unique );											// 아이템 버려
	bool		SM__PICKUP__ITEM( DWORD id, DWORD unique, DWORD life );								// 아이템 집기
	void		SM__ATTACK__ITEM( DWORD id, DWORD unique, long user_unique, int motion, int dmg );	// 아이템 공격
	void		SM__DESTROY__ITEM( int index, DWORD unique );										// 파괴오브젝트	
	void		SM__DELETE__EQUIP( DWORD unique, DWORD item_index, DWORD item_unique );				// 장착아이템삭제
	void		SM__TRY__ITEMDEAL( long unique, DWORD item_index, DWORD item_unique );				// 아이템주고받기
	void		SM__REPLY__ITEMDEAL( long unique, DWORD item_index, DWORD item_uniquem, BYTE byFlag,
									 BYTE item_pos, _uItemInfo iteminfo );		// 아이템받기요청
	// 화살
	void		SM__CONFIRM__ARROW( BYTE arrow );
	int			GetArrowCount();	

	//-----------------------------------------------------------------------------
	// 말
	//-----------------------------------------------------------------------------
	int			SM__HORSE__BUY( WORD error, long unique );														// 말사기 
	void		SM__HORSE__SELL( WORD error, long sell_unique, long buy_unique );								// 말팔기
	void		SM__HORSE__DOWN( WORD error, long unique, DWORD pos, int horse_index, long horse_unique );		// 말내린다고 받기
	void		CM__HORSE__DOWN();																				// 말내린다고 쏘기
	void		SM__HORSE__RIDE( WORD error, long chr_unique, int index, long horse_unique, int pos = -1 );		// 말탄다고 받기
	void		SM__HORSE__TIME( WORD error, long unique, int index );											// 말죽기
	
	void		SM__TAMING__HORSE( WORD error, long unique, int index, DWORD horse_unique );		// 말꼬시기
	void		SM__TREAT__CARROT( WORD error );													// 당근주기
	void		SM__HORSE__KEEPIN( WORD error, long unique );										// 마굿간에 넣기
	void		SM__HORSE__KEEPOUT( WORD error, long unique, DWORD pos, DWORD type );				// 말찾기

	//-----------------------------------------------------------------------------
	// 공격
	//-----------------------------------------------------------------------------
	void		SM__REQUEST__PK( long unique );														// 공격신청
	void		SM__REPLY__PK( long unique, bool bReply );											// 공격신청 응답
	void		SM__ATTACK__PC( long Attacker, long Defender, int motion, int dmg, bool critic );	// 공격상태
	void		SM__ARROW__GOING( int shooter_index, long shooter, int index, DWORD target );		// 활쏘기
	void		SM__USE__SKILL( BYTE Res, int skill, int caster_index, long caster, int target_index, long target );	// 스킬사용 
	void		SetSkillEffect( int skill, int caster_index, long caster, int target_index, long target, int skilllev = 0 );				// 스킬효과
	void		CM__USE__SKILL();												// 스킬쏘기
	void		SM__BUFF__CHANGE( long unique, BYTE byBuff, bool bFlag );		// 버프
	void		CM__USE__SKILL__PARTY( int menu, int num );

	//-----------------------------------------------------------------------------
	// 공격대상 갱신
	//-----------------------------------------------------------------------------
	EVENT_DATA_INFO	GetAttackState( long lUnique );								// 타겟정보
	bool		GetPcAlive( EVENT_DATA_INFO& data );							// 유저정보갱신
	void		SetUnderAttack( long unique );									// 유저공격당함

	//-----------------------------------------------------------------------------
	// 채팅
	//-----------------------------------------------------------------------------
	bool		SM__NORMAL__CHAT( long unique, char* message );					// 채팅 메세지 받기
	void		CM__NORMAL__CHAT( char* message );								// 채팅 메세지 쏘기 

	bool		CM__ADMIN__CHAT( char* message );								// 운영자 메세지
	bool		UpdataOperatorMode( char* message );							// 운영자모드 변경인가?
	
	//-----------------------------------------------------------------------------
	// Desc: 서버에 보내기
	//-----------------------------------------------------------------------------
	void		CM__WEATHER( int type );										// 날씨보내기	

/****************************************************************************************************
	* Display with Network Info
****************************************************************************************************/
protected:		
	//-----------------------------------------------------------------------------
	// Desc: 자기 캐릭터
	//-----------------------------------------------------------------------------
	Player		m_Player;		

public:
	//-----------------------------------------------------------------------------
	// 서버에서 받은 블락
	//-----------------------------------------------------------------------------
	long		lReceiveBlock;
	long		GetReceiveBlock()	{ return lReceiveBlock; }
	int			GetReceiveX()		{ return lReceiveBlock%m_cSize; }
	int			GetReceiveY()		{ return lReceiveBlock/m_cSize; }
	//-----------------------------------------------------------------------------
	// Desc: 다른 유저
	//-----------------------------------------------------------------------------
	NonPlayer	m_Pc[MAX_USER_INTABLE];	
	int			m_PcTableCount;

	//-----------------------------------------------------------------------------
	// Desc: 유저포인터 리턴
	//-----------------------------------------------------------------------------
	PCTABLE	   *GetNextPcTable(int & nindex);			// By simwoosung
	PCTABLE	   *FindPcTable(Character *pSrc);			// By wxywxy
	PCTABLE	   *FindPcTable(long unique);               // By simwoosung
	PCTABLE	   *FindPcTable(TCHAR * strName);	
	
	NonPlayer*	GetUser(const short& idx) { return &m_Pc[idx]; }
	NonPlayer*	GetUser(long unique);
	Player*		GetPlayer() { return &m_Player; }
	
	int			GetAttrIndex(BYTE aCitizen,BYTE aGender);

public:
	//-----------------------------------------------------------------------------
	// Desc: 선택화면에서 사용
	//-----------------------------------------------------------------------------
	bool		m_bTempPlayer[MAX_CHARACTER_COUNT];
	int			m_TempIndex[MAX_CHARACTER_COUNT];
	Player		m_TempPlayer[MAX_CHARACTER_COUNT];	
	void		CreateTempPlayer();
	void		DeleteTempPlayer( bool bAll = true );
	void		DisplayTempPlayer();	

	//-----------------------------------------------------------------------------
	// Desc: 캐릭터생성화면에서 사용
	//-----------------------------------------------------------------------------
	Player		m_MakePlayer;
	int			m_MakePlayerIndex;
	void		CreateMakePlayer( int index );
	void		DisplayMakePlayer();	
	void		ChangePlayerStyle( BYTE color, BYTE hair, BYTE face, long lUnique = -1 );	
	void		ChangePlayerStyle( UINT style, long lUnique = -1 );
	

	//-----------------------------------------------------------------------------
	// Desc: 이모티콘처리
	//-----------------------------------------------------------------------------
	IMOTICON_TABLE	m_ImoticonTable[MAX_USER_INTABLE];
	void		SetImoticon( int index, DWORD unique, int type );
	void		DisplayImoticon();
	void		DelImoticon( int index, DWORD unique );

	//-----------------------------------------------------------------------------
	// Desc: 퀘스트
	//-----------------------------------------------------------------------------
	bool		m_DisplayQuest;
	int			m_CurrentQuest;
	QUEST_INFO	m_QuestInfo[QUEST_MAX_COUNT];
	void		ViewQuest();
	void		DisplayQuest();
	void		UpdateQuest( int index, BYTE type );
	void		SetQuestData( int Class, int index );
	void		DelQuestData( int Class );	

	//-----------------------------------------------------------------------------
	// Desc: 맵정보
	//-----------------------------------------------------------------------------
	MAP_INFO	m_MapInfo;
	void		InitMapInfo();
	void		DisplayMapInfo( int map_id, BYTE type = 0 );
	char		m_cMapName[64];
	char*		GetMapName( int map_id );

	//-----------------------------------------------------------------------------
	// 말상태 리턴
	//-----------------------------------------------------------------------------
	bool		GetHorseState( int index = MAX_USER_INTABLE );
	
	//-----------------------------------------------------------------------------
	// 합체상태 임시변수
	//-----------------------------------------------------------------------------
	long		m_TempUnite[MAX_TEMP_STATE];	// 유니크
	STATE_INFO	m_TempState[MAX_TEMP_STATE];	// 캐릭터상태

	//-----------------------------------------------------------------------------
	// Desc: 기타...
	//-----------------------------------------------------------------------------
	TextureTable		m_TexTable;			// 텍스쳐모음
	SelectBox			m_Select;			// 캐릭터선택박스	
	EffectTexture*		m_pEffectTex;		// 이펙트 텍스쳐
	ShadowTable			m_ShadowTable;		// 그림자 테이블	
	D3DXVECTOR3			m_vCameraMove;		// 카메라 이동
	bool				m_bOperator;		// 운영자니?
	unsigned int		m_bChatType;		// 채팅종류		

	CDamageEffect		m_DamageEffect;

public:
	//-----------------------------------------------------------------------------
	// Desc: 맵사이즈
	//-----------------------------------------------------------------------------
	int			m_cSize;										// 맵사이즈
	bool		Init(int size);									// 맵사이즈 초기화
	void		SetupSize(int size)	{ m_cSize = size; };		// 맵사이즈저장 
	int			GetMapSize()		{ return m_cSize; }			// 맵 사이즈리턴	
	
	//-----------------------------------------------------------------------------
	// Desc: 정보값 리턴
	//-----------------------------------------------------------------------------	
	int			GetX()							{ return m_MainPC.lBlock%m_cSize; };	
	int			GetY()							{ return m_MainPC.lBlock/m_cSize; };	
	int			GetDestX( long unique );
	int			GetDestY( long unique );
	int			GetDestX();						
	int			GetDestY();						
	long		GetDestBlock(int x, int y)		{ return (y*m_cSize)+x; };
	void		SetupDestBlock(int x, int y)	{ m_MainPC.lDestBlock = (y*m_cSize)+x; };
	void		SetupDestBlock(long unique, int x, int y);			
	void		SetupPcFloor(int floor)			{ m_MainPC.nCurrentFloor = (BYTE)floor; }	
	float		GetMapHeight( int x, int y )	{ return g_Map.Get__MapHeight( x, y ); }	// 해당 타일 높이값
	float		GetMapHeightF( float x, float y )	{ return g_Map.Get__MapHeightF( x, y ); }	// 해당 타일 높이값
	void		SetlBlock( int block )			{ m_MainPC.lBlock = block; }				// 현재블럭값 저장
	long		GetlBolck()						{ return m_MainPC.lBlock; }					// 현재블럭값 리턴
	void		SetlDestBlock( int block )		{ m_MainPC.lDestBlock = block; }			// 현재블럭값 저장
	long		GetlDestBolck()					{ return m_MainPC.lDestBlock; }				// 현재블럭값 리턴	

	D3DXMATRIX	GetPosTM( long unique );													// 메트릭스 리턴
	D3DXMATRIX	GetPosEyeTM( long unique );
	D3DXVECTOR3	GetvPos( long unique );
	D3DXVECTOR3	GetvBonePos( long unique , int BoneNum );

	D3DXMATRIX	GetHandTM( int type, long unique );											// 오른손, 왼손 리턴
	D3DXMATRIX	GetHandRealTM( int type, long unique );

	int			GetBoneLinkTM(long Unique,int Index,int Num,D3DXMATRIX &mat);						// 이펙트 출력시 본 메트릭스 구하기 ( NPC 도 추가 )

	D3DXVECTOR3	GetBoundMax( long unique, BYTE type = 0 );									// 바운드 박스 값 리턴	
	float		GetBoundSize( long unique );											// 최대 길이.
	float		GetPcHeight( long unique );												// 캐릭터 높이값
	bool		GetVisible( long unique );													// 보이는 플래그 리턴
	float		GetDirection( long unique );												// 유니크로 방향 얻기 
	D3DXVECTOR3	GetPlayerSightDir( long unique, float SightLength, float DeltaRad );		// 보는 방향을 집어 넣으면 좌표 거리에따른 좌표를 리턴 
	int			GetHorseRide( int index, long lBlock, long lDestBlock, float& rot1, float& rot2, int& angle );	

	D3DXVECTOR3	GetPcInnerPos( int index, long unique, float height = 0.5f );
	D3DXVECTOR3 GetPcCenterBoneInnerPos( int index, long unique, float height = 0.5f );
	D3DXMATRIX	GetPosBoneCenterTM( long unique );

public:	
	//-----------------------------------------------------------------------------
	// Desc: 초기화
	//-----------------------------------------------------------------------------
	CBravoPC();
	~CBravoPC();
	void		FinalCleanup();	
	void		CreateEffectData();
	
	void		CreateDamageEffect(void);				// Desc: 데미지 이펙트 로딩

	//-----------------------------------------------------------------------------
	// Desc: 렌더링처리
	//-----------------------------------------------------------------------------	
	void		AniProcess();
	void		Display();
	void		EffectDisplay();	

	//-----------------------------------------------------------------------------
	// 그림자
	//-----------------------------------------------------------------------------
	void		ShadowDisplay();
	void		ShadowMake();			

	//-----------------------------------------------------------------------------
	// Desc: 이동처리
	//-----------------------------------------------------------------------------
	void		SetPlayerMove( DWORD pos );				// 캐릭터 위치값
	void		SetWarpMove( PCTABLE& pc );				// 워프했을 경우 자기 정보 받아서 테이블에 저장		

	//-----------------------------------------------------------------------------
	// Desc: 마우스처리
	//-----------------------------------------------------------------------------
	bool		SetMouse( bool bFindNpc );				// 마우스입력에 위한 처리
	bool		SetTile();								// 타일선택
	bool		SetPC();								// 유저선택
	bool		SetNPC();								// NPC선택
	bool		MerchantProcess( int merchant, int select = -1 );		// 상인처리
		
	//-----------------------------------------------------------------------------
	// Desc: 유저픽킹
	//-----------------------------------------------------------------------------
#ifdef HHM_CHARACTER_SELECT_3D_PICK// 캐릭터 선택창에서 버튼으로 하던 캐릭터 선택을 3D 픽킹으로 바꿈(2009/06/12)

	short		PickCharSelectUser();							// 캐릭터선택화면 캐릭터 픽킹

#endif// #ifdef HHM_CHARACTER_SELECT_3D_PICK// 캐릭터 선택창에서 버튼으로 하던 캐릭터 선택을 3D 픽킹으로 바꿈(2009/06/12)

	short		PickUser();								// 캐릭터중 캐릭터 픽킹
	short		m_nPickedUser;							// 픽킹한 캐릭터 인덱스
	bool		UpdateUserTable();						// 유저 테이블 갱신
	bool		UpdateUser( int who );					// who 유저 테이블 갱신
	short		m_nUserCount;							// 유저 갯수


	//...............................................................................................................
	// AppearNPC 메세지 받은후 신수이면 처리
	//...............................................................................................................
	int			ISCheckHoly(SRpScAppearNPC *aMsg);
	//...............................................................................................................
	// DisAppearNPC 메세지 받은후 신수이면 처리
	//...............................................................................................................
	int			ISCheckDisappearHoly(SRpScDisappearNPC *aMsg);

	int			GetDanceMotion( PCTABLE	* pTable );
};

extern	CBravoPC		g_Pc;


#endif
