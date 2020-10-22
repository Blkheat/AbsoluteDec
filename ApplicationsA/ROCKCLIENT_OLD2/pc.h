#ifndef		_____PC_____
#define		_____PC_____

#include	"brother.h"
#include	<OBJ\\ObjBase.H>
#include	<OBJ\\Player.H>
#include	<OBJ\\NonPlayer.H>
#include	<Map\Field.h>
#include	"Map.H"
#include	"bravolibs\\effect\\CDamageEffect.h"		// ������ ����Ʈ ó�� �κ� by wxywxy
#include	"Bravolibs\\Obj\\SkillSystem.h"
#include	<list>

using namespace std;
 

#define		MAX_USER_INTABLE			128			// �������̺�
#define		MAX_ID_STRLEN				10			// ���̵�
#define		MAX_PASS_STRLEN				10			// ��й�ȣ
#define		MAX_CHARACTER_COUNT			3			// ĳ����

#define		MAX_NPC_SPPECH_COUNT		10			// NPC �� ����
#define		QUEST_MAX_COUNT				5			// ����Ʈ ����
#define		MAX_TEMP_STATE				16			// �ӽû�������

//-----------------------------------------------------------------------------
// Desc: ��������
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
// Desc: ����Ÿ��
//-----------------------------------------------------------------------------
enum	MERCHANT_TYPE
{
	MAGIC_MERCHANT = 3,		// ����	3
	WEAPON_MERCHANT,		// ����	4
	STORE_MERCHANT,			// â��	5
	GUILD_MASTER,			// ���	6
	HORSE_MERCHANT,			// ��	7
};


//-----------------------------------------------------------------------------
// Desc: ��ڿ� �ý��� ��ɾ� Ÿ��
//-----------------------------------------------------------------------------
enum OPERATOR_MSG_TYPE
{
	OPT_MSG_BASIC = 0,	// �⺻
	OPT_MSG_POS,		// �ʾ��̵�, X-Y ��ǥ
	OPT_MSG_COIN,		// ������
	OPT_MSG_TALK,		// ä�� on, off
};


//-----------------------------------------------------------------------------
// Desc: �˱�, �׸��ڵ��� ��Ÿ ����� �ؽ��ĸ���
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
	LPDIRECT3DTEXTURE9			pAfterImage1;	// �˱�(�Ϲ�)
	LPDIRECT3DTEXTURE9			pAfterImage2;	// �˱�(��)
	LPDIRECT3DTEXTURE9			pAfterImage3;	// �˱�(����)
	LPDIRECT3DTEXTURE9			pShadow;		// �׸���			
	LPDIRECT3DTEXTURE9			pImoticon;		// �̸�Ƽ��	
	LPDIRECT3DTEXTURE9			pQuest;			// ����Ʈâ
#else
	LPDIRECT3DTEXTURE8			pAfterImage1;	// �˱�(�Ϲ�)
	LPDIRECT3DTEXTURE8			pAfterImage2;	// �˱�(��)
	LPDIRECT3DTEXTURE8			pAfterImage3;	// �˱�(����)
	LPDIRECT3DTEXTURE8			pShadow;		// �׸���			
	LPDIRECT3DTEXTURE8			pImoticon;		// �̸�Ƽ��	
	LPDIRECT3DTEXTURE8			pQuest;			// ����Ʈâ
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
// Desc: ���ùڽ� 
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
// Desc: �׸��� ���̺�
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
// ���� ����
//-----------------------------------------------------------------------------
/*
enum	Character_Status
{
	CHR_ALIVE = 0,		// �Ϲ� ����
	CHR_REST,			// �޽� ����
	CHR_PASSIVE,		// �нú꽺ų�� �ɸ� ����
	CHR_MAGIC,			// �������� ����
	CHR_POISON,			// ���� �ɸ� ����
	CHR_STUN,			// ����
	CHR_DEATH,			// ���� ����
	CHR_WARP,			// �������� ����...�� ���¿����� ������� ��� �޼��� �ñ�...2-19
};
*/

//-----------------------------------------------------------------------------
// Desc: ���� ���̺�
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
// Desc: �̸�Ƽ��
//-----------------------------------------------------------------------------
struct IMOTICON_TABLE 
{
	int		index;		// �ε���.. ������ -1�̴�
	DWORD	unique;		// ����ũ
	DWORD	time;		// ������ �ð�
	int		type;		// �̸�Ƽ�� ����.. -1�̸� ���� 

	IMOTICON_TABLE()	{ Clear(); }
	void	Clear() { index = -1; unique = 0; time = 0; type = -1; }
};


//-----------------------------------------------------------------------------
// Desc: ����Ʈ����
//-----------------------------------------------------------------------------
#define		QUEST_TITLE			64		// �ѱ� 32
#define		QUEST_OFFER			32		// �ѱ� 16
#define		QUEST_STORY			512		// �ѱ� 256
#define		QUEST_OBJECT		128		// �ѱ� 64
#define		QUEST_GIFT			128		// �ѱ� 64

#define		QUEST_LONG_LINE		20		// 20��
#define		QUEST_MEDIUM_LINE	10		// 10��
#define		QUEST_SHORT_LINE	5		// 5��

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
	char		title[QUEST_TITLE];		// ����
	char		offer[QUEST_OFFER];		// ������	

	// ����
	BYTE		SubjectCurLine;
	BYTE		SubjectMaxLine;
	CHAR_LINE	Subject[QUEST_LONG_LINE];

	// ����
	BYTE		ObjectCurLine;
	BYTE		ObjectMaxLine;
	CHAR_LINE	Object[QUEST_SHORT_LINE];

	// ����
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
	QUEST_SCENARIO = 0,		// �ó�����
	QUEST_EVENT,			// �̺�Ʈ
	QUEST_MAKE,				// ����
	QUEST_BOARD,			// ����
	QUEST_TASK,				// �½�ũ
};

struct	QUEST_INFO
{	
	bool			bInit;		// �ʱ�ȭ
	QUEST_TABLE		table;		// ����Ʈ
	QUEST_TEXT		text;		// �ؽ�Ʈ	
	
	QUEST_INFO()	{ Clear(); }
	void	Clear()	{ bInit = false; }
	void	SetQuestData( int index );	
};

//-----------------------------------------------------------------------------
// Desc: ������(�̸�) 
//-----------------------------------------------------------------------------
#define	MAP_COUNT		10		// �ʰ���
#define	MAPID_LENGTH	20		// ���̸�����(�ѱ�10��)

struct	MAP_INFO
{
	CHAR_LINE	id[MAP_COUNT];	
};


struct Item_Info_Save;
union _uItemType;

//-----------------------------------------------------------------------------
// Desc: ����ó��
//-----------------------------------------------------------------------------
class		CBravoPC
{
/// sooree network
///-------------------------------------------------------------------------------------
///	Rock PC Data : ������ ������ ���� �κ��� �ϴ� ���ΰ� �츮 �����ͷ� ���� �޾ƿͼ� ó���Ѵ�. 
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
	// Desc: ��� ����
	//-----------------------------------------------------------------------------
	// �����������	
//	Char_Item		m_Equip[MAX_EQUIPED];		
//	Char_Item*		GetEquip()					{ return m_Equip; }	
	
	// ����, �����
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

	// ĳ���ͼӼ�����
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

	void		SM__CHANGE__ITEM( long unique, DWORD item_id, DWORD item_unique, DWORD item_pos, BYTE pos );	// ������ �ٲٱ�
	void		SM__TAKEOFF__ITEM( long unique, DWORD item_id, DWORD item_unique );
	void		SM__APPEAR( long unique, PCTABLE& pc );															// ���� ���� 

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
	// Desc: ����ó��
	//-----------------------------------------------------------------------------
	// ĳ���� ����
	/*
	void		SM__APPEAR( PCTABLE& pc, char szHorseName[12], DWORD type, bool bUnite, long unique, bool bAttackMode, 
							bool bCharge, bool bNecta, bool bTar, bool bPK, bool bWalk, bool bSword, bool bDown, BYTE status );

	void		SM__DISAPPEAR( long unique );																	// ĳ���� ����
	void		SM__MOVE__BROADCAST( PCTABLE& pc, DWORD Path, bool bUnite, long unique, bool bCurrent );		// ĳ���� �̵�
	void		SM__USER__DIE( long unique );																	// ĳ���� ���
	void		SM__ATTACK__NPC( long unique, DWORD npcunique, DWORD dmg, int motion, DWORD pos, bool critic );	// ���ݸ޼��� �ޱ�
	void		SM__CHR__DIR( DWORD unique, BYTE dir );
	void		SM__CHARACTER__INFO( SP_Character_Info* pInfo );

	//-----------------------------------------------------------------------------
	// Desc: ������
	//-----------------------------------------------------------------------------
	void		SM__MODE__CHANGE( long unique, BYTE type );
	void		CM__MODE__CHANGE( BYTE type );
	void		SM__USE__NECTA__TAR( long unique, BYTE kind, bool bFlag );	
	void		CM__USE__NECTA__TAR( BYTE type, bool bCancel = false );
	void		SM__CHANGE__GAGE( BYTE type, int cur );	
	void		SM__OPERATOR__MODE( BYTE err );

	//-----------------------------------------------------------------------------
	// Desc: ����Ʈ
	//-----------------------------------------------------------------------------
	void		SM__GET__QUEST( BYTE err, BYTE Class, BYTE index );				// ����Ʈ�ޱ�
	void		SM__SUCC__QUEST( BYTE Class, BYTE index );						// ����Ʈ��������
	void		SM__QUEST__CHECK( BYTE Class, BYTE Case, DWORD index );			// ��������Ʈ�˻��ϱ�

	//-----------------------------------------------------------------------------
	// ����, ������
	//-----------------------------------------------------------------------------
	void		SM__CHANGE__CLASS( BYTE job );
	void		SM__LEVEL__UP( long lUnique, int nLevel, int nPoint );
	
	//-----------------------------------------------------------------------------
	// ���� ��ȭ	
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
	// ������
	//-----------------------------------------------------------------------------
	void		SM__CHANGE__ITEM( long unique, DWORD item_id, DWORD item_unique, DWORD item_pos, BYTE pos );	// ������ �ٲٱ�
	void		SM__TAKEOFF__ITEM( long unique, DWORD item_id, DWORD item_unique );					// ������ ����		
	void		SM__DROP__ITEM( DWORD id, DWORD unique );											// ������ ����
	bool		SM__PICKUP__ITEM( DWORD id, DWORD unique, DWORD life );								// ������ ����
	void		SM__ATTACK__ITEM( DWORD id, DWORD unique, long user_unique, int motion, int dmg );	// ������ ����
	void		SM__DESTROY__ITEM( int index, DWORD unique );										// �ı�������Ʈ	
	void		SM__DELETE__EQUIP( DWORD unique, DWORD item_index, DWORD item_unique );				// ���������ۻ���
	void		SM__TRY__ITEMDEAL( long unique, DWORD item_index, DWORD item_unique );				// �������ְ�ޱ�
	void		SM__REPLY__ITEMDEAL( long unique, DWORD item_index, DWORD item_uniquem, BYTE byFlag,
									 BYTE item_pos, _uItemInfo iteminfo );		// �����۹ޱ��û
	// ȭ��
	void		SM__CONFIRM__ARROW( BYTE arrow );
	int			GetArrowCount();	

	//-----------------------------------------------------------------------------
	// ��
	//-----------------------------------------------------------------------------
	int			SM__HORSE__BUY( WORD error, long unique );														// ����� 
	void		SM__HORSE__SELL( WORD error, long sell_unique, long buy_unique );								// ���ȱ�
	void		SM__HORSE__DOWN( WORD error, long unique, DWORD pos, int horse_index, long horse_unique );		// �������ٰ� �ޱ�
	void		CM__HORSE__DOWN();																				// �������ٰ� ���
	void		SM__HORSE__RIDE( WORD error, long chr_unique, int index, long horse_unique, int pos = -1 );		// ��ź�ٰ� �ޱ�
	void		SM__HORSE__TIME( WORD error, long unique, int index );											// ���ױ�
	
	void		SM__TAMING__HORSE( WORD error, long unique, int index, DWORD horse_unique );		// �����ñ�
	void		SM__TREAT__CARROT( WORD error );													// ����ֱ�
	void		SM__HORSE__KEEPIN( WORD error, long unique );										// ���°��� �ֱ�
	void		SM__HORSE__KEEPOUT( WORD error, long unique, DWORD pos, DWORD type );				// ��ã��

	//-----------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------
	void		SM__REQUEST__PK( long unique );														// ���ݽ�û
	void		SM__REPLY__PK( long unique, bool bReply );											// ���ݽ�û ����
	void		SM__ATTACK__PC( long Attacker, long Defender, int motion, int dmg, bool critic );	// ���ݻ���
	void		SM__ARROW__GOING( int shooter_index, long shooter, int index, DWORD target );		// Ȱ���
	void		SM__USE__SKILL( BYTE Res, int skill, int caster_index, long caster, int target_index, long target );	// ��ų��� 
	void		SetSkillEffect( int skill, int caster_index, long caster, int target_index, long target, int skilllev = 0 );				// ��ųȿ��
	void		CM__USE__SKILL();												// ��ų���
	void		SM__BUFF__CHANGE( long unique, BYTE byBuff, bool bFlag );		// ����
	void		CM__USE__SKILL__PARTY( int menu, int num );

	//-----------------------------------------------------------------------------
	// ���ݴ�� ����
	//-----------------------------------------------------------------------------
	EVENT_DATA_INFO	GetAttackState( long lUnique );								// Ÿ������
	bool		GetPcAlive( EVENT_DATA_INFO& data );							// ������������
	void		SetUnderAttack( long unique );									// �������ݴ���

	//-----------------------------------------------------------------------------
	// ä��
	//-----------------------------------------------------------------------------
	bool		SM__NORMAL__CHAT( long unique, char* message );					// ä�� �޼��� �ޱ�
	void		CM__NORMAL__CHAT( char* message );								// ä�� �޼��� ��� 

	bool		CM__ADMIN__CHAT( char* message );								// ��� �޼���
	bool		UpdataOperatorMode( char* message );							// ��ڸ�� �����ΰ�?
	
	//-----------------------------------------------------------------------------
	// Desc: ������ ������
	//-----------------------------------------------------------------------------
	void		CM__WEATHER( int type );										// ����������	

/****************************************************************************************************
	* Display with Network Info
****************************************************************************************************/
protected:		
	//-----------------------------------------------------------------------------
	// Desc: �ڱ� ĳ����
	//-----------------------------------------------------------------------------
	Player		m_Player;		

public:
	//-----------------------------------------------------------------------------
	// �������� ���� ���
	//-----------------------------------------------------------------------------
	long		lReceiveBlock;
	long		GetReceiveBlock()	{ return lReceiveBlock; }
	int			GetReceiveX()		{ return lReceiveBlock%m_cSize; }
	int			GetReceiveY()		{ return lReceiveBlock/m_cSize; }
	//-----------------------------------------------------------------------------
	// Desc: �ٸ� ����
	//-----------------------------------------------------------------------------
	NonPlayer	m_Pc[MAX_USER_INTABLE];	
	int			m_PcTableCount;

	//-----------------------------------------------------------------------------
	// Desc: ���������� ����
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
	// Desc: ����ȭ�鿡�� ���
	//-----------------------------------------------------------------------------
	bool		m_bTempPlayer[MAX_CHARACTER_COUNT];
	int			m_TempIndex[MAX_CHARACTER_COUNT];
	Player		m_TempPlayer[MAX_CHARACTER_COUNT];	
	void		CreateTempPlayer();
	void		DeleteTempPlayer( bool bAll = true );
	void		DisplayTempPlayer();	

	//-----------------------------------------------------------------------------
	// Desc: ĳ���ͻ���ȭ�鿡�� ���
	//-----------------------------------------------------------------------------
	Player		m_MakePlayer;
	int			m_MakePlayerIndex;
	void		CreateMakePlayer( int index );
	void		DisplayMakePlayer();	
	void		ChangePlayerStyle( BYTE color, BYTE hair, BYTE face, long lUnique = -1 );	
	void		ChangePlayerStyle( UINT style, long lUnique = -1 );
	

	//-----------------------------------------------------------------------------
	// Desc: �̸�Ƽ��ó��
	//-----------------------------------------------------------------------------
	IMOTICON_TABLE	m_ImoticonTable[MAX_USER_INTABLE];
	void		SetImoticon( int index, DWORD unique, int type );
	void		DisplayImoticon();
	void		DelImoticon( int index, DWORD unique );

	//-----------------------------------------------------------------------------
	// Desc: ����Ʈ
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
	// Desc: ������
	//-----------------------------------------------------------------------------
	MAP_INFO	m_MapInfo;
	void		InitMapInfo();
	void		DisplayMapInfo( int map_id, BYTE type = 0 );
	char		m_cMapName[64];
	char*		GetMapName( int map_id );

	//-----------------------------------------------------------------------------
	// ������ ����
	//-----------------------------------------------------------------------------
	bool		GetHorseState( int index = MAX_USER_INTABLE );
	
	//-----------------------------------------------------------------------------
	// ��ü���� �ӽú���
	//-----------------------------------------------------------------------------
	long		m_TempUnite[MAX_TEMP_STATE];	// ����ũ
	STATE_INFO	m_TempState[MAX_TEMP_STATE];	// ĳ���ͻ���

	//-----------------------------------------------------------------------------
	// Desc: ��Ÿ...
	//-----------------------------------------------------------------------------
	TextureTable		m_TexTable;			// �ؽ��ĸ���
	SelectBox			m_Select;			// ĳ���ͼ��ùڽ�	
	EffectTexture*		m_pEffectTex;		// ����Ʈ �ؽ���
	ShadowTable			m_ShadowTable;		// �׸��� ���̺�	
	D3DXVECTOR3			m_vCameraMove;		// ī�޶� �̵�
	bool				m_bOperator;		// ��ڴ�?
	unsigned int		m_bChatType;		// ä������		

	CDamageEffect		m_DamageEffect;

public:
	//-----------------------------------------------------------------------------
	// Desc: �ʻ�����
	//-----------------------------------------------------------------------------
	int			m_cSize;										// �ʻ�����
	bool		Init(int size);									// �ʻ����� �ʱ�ȭ
	void		SetupSize(int size)	{ m_cSize = size; };		// �ʻ��������� 
	int			GetMapSize()		{ return m_cSize; }			// �� �������	
	
	//-----------------------------------------------------------------------------
	// Desc: ������ ����
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
	float		GetMapHeight( int x, int y )	{ return g_Map.Get__MapHeight( x, y ); }	// �ش� Ÿ�� ���̰�
	float		GetMapHeightF( float x, float y )	{ return g_Map.Get__MapHeightF( x, y ); }	// �ش� Ÿ�� ���̰�
	void		SetlBlock( int block )			{ m_MainPC.lBlock = block; }				// ������� ����
	long		GetlBolck()						{ return m_MainPC.lBlock; }					// ������� ����
	void		SetlDestBlock( int block )		{ m_MainPC.lDestBlock = block; }			// ������� ����
	long		GetlDestBolck()					{ return m_MainPC.lDestBlock; }				// ������� ����	

	D3DXMATRIX	GetPosTM( long unique );													// ��Ʈ���� ����
	D3DXMATRIX	GetPosEyeTM( long unique );
	D3DXVECTOR3	GetvPos( long unique );
	D3DXVECTOR3	GetvBonePos( long unique , int BoneNum );

	D3DXMATRIX	GetHandTM( int type, long unique );											// ������, �޼� ����
	D3DXMATRIX	GetHandRealTM( int type, long unique );

	int			GetBoneLinkTM(long Unique,int Index,int Num,D3DXMATRIX &mat);						// ����Ʈ ��½� �� ��Ʈ���� ���ϱ� ( NPC �� �߰� )

	D3DXVECTOR3	GetBoundMax( long unique, BYTE type = 0 );									// �ٿ�� �ڽ� �� ����	
	float		GetBoundSize( long unique );											// �ִ� ����.
	float		GetPcHeight( long unique );												// ĳ���� ���̰�
	bool		GetVisible( long unique );													// ���̴� �÷��� ����
	float		GetDirection( long unique );												// ����ũ�� ���� ��� 
	D3DXVECTOR3	GetPlayerSightDir( long unique, float SightLength, float DeltaRad );		// ���� ������ ���� ������ ��ǥ �Ÿ������� ��ǥ�� ���� 
	int			GetHorseRide( int index, long lBlock, long lDestBlock, float& rot1, float& rot2, int& angle );	

	D3DXVECTOR3	GetPcInnerPos( int index, long unique, float height = 0.5f );
	D3DXVECTOR3 GetPcCenterBoneInnerPos( int index, long unique, float height = 0.5f );
	D3DXMATRIX	GetPosBoneCenterTM( long unique );

public:	
	//-----------------------------------------------------------------------------
	// Desc: �ʱ�ȭ
	//-----------------------------------------------------------------------------
	CBravoPC();
	~CBravoPC();
	void		FinalCleanup();	
	void		CreateEffectData();
	
	void		CreateDamageEffect(void);				// Desc: ������ ����Ʈ �ε�

	//-----------------------------------------------------------------------------
	// Desc: ������ó��
	//-----------------------------------------------------------------------------	
	void		AniProcess();
	void		Display();
	void		EffectDisplay();	

	//-----------------------------------------------------------------------------
	// �׸���
	//-----------------------------------------------------------------------------
	void		ShadowDisplay();
	void		ShadowMake();			

	//-----------------------------------------------------------------------------
	// Desc: �̵�ó��
	//-----------------------------------------------------------------------------
	void		SetPlayerMove( DWORD pos );				// ĳ���� ��ġ��
	void		SetWarpMove( PCTABLE& pc );				// �������� ��� �ڱ� ���� �޾Ƽ� ���̺� ����		

	//-----------------------------------------------------------------------------
	// Desc: ���콺ó��
	//-----------------------------------------------------------------------------
	bool		SetMouse( bool bFindNpc );				// ���콺�Է¿� ���� ó��
	bool		SetTile();								// Ÿ�ϼ���
	bool		SetPC();								// ��������
	bool		SetNPC();								// NPC����
	bool		MerchantProcess( int merchant, int select = -1 );		// ����ó��
		
	//-----------------------------------------------------------------------------
	// Desc: ������ŷ
	//-----------------------------------------------------------------------------
#ifdef HHM_CHARACTER_SELECT_3D_PICK// ĳ���� ����â���� ��ư���� �ϴ� ĳ���� ������ 3D ��ŷ���� �ٲ�(2009/06/12)

	short		PickCharSelectUser();							// ĳ���ͼ���ȭ�� ĳ���� ��ŷ

#endif// #ifdef HHM_CHARACTER_SELECT_3D_PICK// ĳ���� ����â���� ��ư���� �ϴ� ĳ���� ������ 3D ��ŷ���� �ٲ�(2009/06/12)

	short		PickUser();								// ĳ������ ĳ���� ��ŷ
	short		m_nPickedUser;							// ��ŷ�� ĳ���� �ε���
	bool		UpdateUserTable();						// ���� ���̺� ����
	bool		UpdateUser( int who );					// who ���� ���̺� ����
	short		m_nUserCount;							// ���� ����


	//...............................................................................................................
	// AppearNPC �޼��� ������ �ż��̸� ó��
	//...............................................................................................................
	int			ISCheckHoly(SRpScAppearNPC *aMsg);
	//...............................................................................................................
	// DisAppearNPC �޼��� ������ �ż��̸� ó��
	//...............................................................................................................
	int			ISCheckDisappearHoly(SRpScDisappearNPC *aMsg);

	int			GetDanceMotion( PCTABLE	* pTable );
};

extern	CBravoPC		g_Pc;


#endif
