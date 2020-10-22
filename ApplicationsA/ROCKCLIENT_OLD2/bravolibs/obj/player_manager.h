
#ifndef _Player_Manager_H_
#define _Player_Manager_H_

#pragma warning(disable:4786)
#include <map>
#include <vector>

using namespace	std;

#include "RockPCH.h"


#include	"ObjBase.H"
#include	"Model.H"
#include	"Texture.H"
#include	"Animation.H"
#include	"Character_Item.H"
#include	"EtcStruct.H"
#include	"Define.H"
#include	"CMantleItem.h"								// 망토 by wxywxy
#include	"..\\bravolibs\\effect\\CFlyWeaponManager.h"   // by wxywy

#include	"..\\..\\RockLibs\\RockInterface\\Define.h"
#include	"..\\..\\RockLibs\\RockInterface\\CBillBoardUIRender.h"

#include	"VFileSystem.h"	/// sooree pack 
#include    "CCharImage.h"
#include    "CNPCImpactTable.h"
#include    "CHolyManager.h"
#include	"PlayerNPCDataManager.h"
#include	"SourceDataManager.h"

///#define DYNAMIC_RESOURCE_LOADING

// mod 갯수
#define		CHR_MOD_COUNT		180
#define		NPC_MOD_COUNT		45

// tex 갯수
#define		CHR_TEX_COUNT		214
#define		NPC_TEX_COUNT		58

// ani 갯수
#define		CHR_ANI_COUNT		69
#define		NPC_ANI_COUNT		255

// itm 갯수
#define		CHR_ITM_COUNT		421
#define		NPC_ITM_COUNT		66

// NPC
#define		NPCNAME_SIZE		16
#define		SPEECH_SIZE			60
#define		NPC_SPEECHCOUNT		12

// 화살테이블갯수
#define		ARROW_COUNT			32

// 유저, NPC 속성갯수
#define		NPC_TYPE_COUNT		81
#define		CHR_TYPE_COUNT		5

// Attr Num
const WORD	c_CharType_Count( 4 );		// 캐릭터 종족 별(밀레나, 레인) 성별(남, 여) 현재 총 4가지 
const WORD	c_NpcType_Count( 250 );		// 한 맵에 들어가는 최대 NPC 종류 50 가지 ( 신수 20 개 기본으로 입력 By wxywxy )

// Mod Num
const WORD	c_Chr_Mod_Count( 2000 );		// 2004. 4. 13 현재 100 개 
const WORD	c_Npc_Mod_Count( 500 );		

// Tex Num
const WORD	c_Chr_Tex_Count( 1000 );
const WORD	c_Npc_Tex_Count( 500 );		// 아직은 한 msh 당 한개의 텍스쳐 사용

// Ani Num
const WORD	c_Chr_Ani_Count( 2000 );		// 실제 애니 총 개수 - 캐릭 한 타입당 실제 300 개 정도씩...4 타입...200 * 4
const WORD	c_Npc_Ani_Count( 3000 );	// 한 NPC 당 최대 25개의 모션을 가짐 50 * 30 

// Itm Num
const WORD	c_Chr_Itm_Count( 3000 );
const WORD	c_Npc_Itm_Count( 500 );

// Quest Num
const WORD  c_Max_Quest_Count( 2000 );			// 퀘스트 카운트
const WORD  c_Max_PreQuestInfo_Count( 1000 );			// 퀘스트 시그널 카운트

const WORD	c_PC_AniIndex_Count( 500 );
const WORD	c_Npc_AniIndex_Count( 30 );			// Rock_Npc Total_Ani_Type 개수 

const WORD	c_Max_NPC_Num( 3000 );				// NPC 최대 개수 
const WORD	c_Max_NPCCode_Num( 8000 );			// NPC Code 최대 개수 

const WORD	c_Max_Item_Num( 15000 );			// ITEM 생성 최대 개수 
const WORD	c_Max_ItemCode_Num( 65535 );		// Item Code 최대 갯수

const WORD  c_Max_ItemAddEffect_Num( 1000 );		// 아이템  부가효과 타입의 개수
 

const WORD	c_Max_Skill_Num( 3000 );			// SKILL 최대 개수 
const WORD	c_Max_SkillCode_Num( 10000 );		// SKILL Code 최대 개수


const WORD	c_Max_Stat_Num( 1000 );				// Stat 최대 개수 
const WORD	c_Max_MainClass_Num( 200 );			// 

const WORD	c_Max_ScriptStr_Num( 30000 );		// Script String 최대 개수 

const WORD	c_Max_AddEffect_Num( 100 );			// 부가 효과 타입의 개수 
// 인코 시스템 
const WORD c_Max_FightMap_Num( 20 );			// 인코 시스템 사용 맵 개수 
const WORD c_Max_GameMode_Num( 5 );				// 인코 시스템 게임 모드 개수
 

//-----------------------------------------------------------------------------
// Desc: 알파소팅 테이블
//
// 1. 소팅타겟의 정보를 저장하는 구조체
// 2. 배열값과 위치값을 이용하며 해당 구조체함수를 이용하여서 모든 처리를 하도록 한다
// 3. 소팅타겟으로 받는 구조체에 알맞게 처리되도록 범용적으로 제작하는 것이 목적
//    
// Copyright (c) 2003.12.22 By KStaR79
//-----------------------------------------------------------------------------
struct CHR_SORT_TABLE
{	
	BYTE	byType;		// 타입
	short	Index;		// 인덱스
	
	CHR_SORT_TABLE()	{ Clear(); }
	void	Clear()		{ byType = 255; Index = 0; }
};

///-----------------------------------------------------------------------------
/// Desc : 각 Map 별로 쓰여지는 NPC 들을 나열하고 있다
/// author : niceguy 
/// date : 2004. 04. 12
///-----------------------------------------------------------------------------

///By simwoosung - NPC는 chr파일단위로 PC는 itm파일 단위로 데이타 관리
typedef struct SNPC_Set
{
	WORD		total_npc_set_num;
	WORD		total_npc_num;
	WORD		total_itm_num;
	WORD		total_msh_num;
	WORD		total_tex_num;
	WORD		total_ani_num;
//	WORD		index[c_NpcType_Count];
	WORD		npc_code[c_NpcType_Count];
	int			file_name[c_NpcType_Count];
//	char		npc_name[c_NpcType_Count];
	
	void	Clear()
	{
		total_npc_set_num = 0;
		total_npc_num = 0;
		total_itm_num = 0;
		total_msh_num = 0;
		total_tex_num = 0;
		total_ani_num = 0;

		memset(npc_code, 0, sizeof(WORD)*c_NpcType_Count);
		memset(file_name, 0, sizeof(int)*c_NpcType_Count);
	}

	BOOL IsExistFileName(int nfile_name)
	{
		for(int i = 0 ; i < total_npc_num ; i++)
		{
			if( file_name[i] == nfile_name )
			{
				return TRUE;
			}

			if(!file_name[i])
			{
				return FALSE;
			}
		}

		return FALSE;
	}

} SNPC_Set;

///-- By simwoosung
struct SPCItemSet
{
	map < DWORD, DWORD > mItmRFileCodelist;

	void	Clear()
	{
		mItmRFileCodelist.clear();
	}

	BOOL IsExistFileName(DWORD dfile_name)
	{
		map < DWORD, DWORD >::iterator fPos = mItmRFileCodelist.find(dfile_name);
		if(fPos == mItmRFileCodelist.end())
		{
			return FALSE;	
		}

		return TRUE;	
	}

	void InsertFileName(DWORD dfile_name)
	{	
		mItmRFileCodelist.insert(map <DWORD , DWORD>::value_type(dfile_name, dfile_name));
	}	
};

//-----------------------------------------------------------------------------
// Desc: NPC 속성
//
// NPC속성과 관련된 정보값을 저장하고 있다
//
// Copyright (c) 2003.12.29 By KStaR79
//-----------------------------------------------------------------------------
struct NPC_SSPEECH
{
	char	speech[SPEECH_SIZE];		// 대사(생성-이동-대기-소멸-전투순)
	NPC_SSPEECH() { memset(&speech, 0, sizeof(speech)); }
};

struct NPC_ATTR
{
	WORD		id;							// 인덱스
	char		name[NPCNAME_SIZE];			// 이름
	BYTE		attack;						// 공격 가능한지
	BYTE		respawn;					// 생성방식
	BYTE		die;						// 소멸방식
	BYTE		set;						// 셋트번호
	BYTE		block;						// 블럭타입

//	vector <WORD>	SoundList;				// 피 타격 사운드 리스트 By wxywxy	

	NPC_SSPEECH	Speech[NPC_SPEECHCOUNT];	// 대사

	NPC_ATTR()  
	{ 
		attack = 0; respawn = 0; die = 0; set = 0; block = 0;
		memset( &name, 0, sizeof(name) ); 
	}

};


//-----------------------------------------------------------------------------
// Desc: brv_chr 파일정보
//
// 파일정보(brv_chr)를 가지고 있는 구조체.
// 각각의 캐릭터가 해당 구조체를 링크하여 정보값을 사용하도록 한다
//
// Copyright (c) 2003.12.28 By KStaR79
//-----------------------------------------------------------------------------
struct CHR_FILE_INFO
{
	float			fAniTime;		// 애니타임(사용X)
	float			fStep;			// 이동보폭(사용X)
	D3DXVECTOR3		vScale;			// 스케일
	ALPHA_INFO		AlphaInfo;		// 알파정보(사용X)
	BOUND_INFO		BoundInfo;		// 바운드박스
	ANI_INFO*		pAniInfo;		// 애니메이션정보(차후에는 사용하지 않아야 한다)
	int				nItmCount;		// 아이템갯수
	int*			pItmIndex;		// 아이템인덱스
	ARMOR_INFO		ArmorInfo;		// 무기링크정보(Only Npc Use)
	unsigned int	m_Index;		// 파일인덱스
	D3DXVECTOR3		minBoundVec;	// 최소 
	D3DXVECTOR3		maxBoundVec;	// 최대
	BOUND_INFO		CollBoundInfo;	// 충돌바운드박스
	

	CHR_FILE_INFO()		{ Clear(); }
	~CHR_FILE_INFO()	{ Destroy(); }

	void	Clear();		// 초기화
	void	Destroy();		// 파괴자

	bool	FileRead( char* szFilePath, BYTE byType );	// brv_chr 파일읽기
	bool	FileReadVF( CVFileSystem* pVF, char* szFilePath, BYTE byType );	/// sooree pack

	void	ReSetBoundBox(D3DXVECTOR3 & min, D3DXVECTOR3 & max);
};


//-----------------------------------------------------------------------------
// Desc: 유저 속성
//-----------------------------------------------------------------------------
struct	PC_ATTR
{
	BYTE	center_node;				// 중심 본
	BYTE	r_hand_node;				// 오른손 본
	BYTE	l_hand_node;				// 왼손 본
	BYTE	breast_node;				// 가슴 본
	BYTE	hair_node;					// 머리 본
	BYTE	r_foot_node;				// 오른발 본
	BYTE	l_foot_node;				// 왼발 본
	BYTE	l_shoulder_node;			// 왼어깨 
	BYTE	r_shoulder_node;			// 오른어깨
	BYTE	l_elbow_node;				// 왼 꿈치 
	BYTE	r_elbow_node;				// 오른 꿈치
	BYTE	l_knee_node;				// 왼 무릎 
	BYTE	r_knee_node;				// 오른 무릎 
	BYTE	l_pelvis_node;				// 왼 골반 
	BYTE	r_pelvis_node;				// 오른 골반 
	
	
	
	DWORD	hair;		
	DWORD	face;
	DWORD	body;
	DWORD	pants; 
	DWORD	hand;
	DWORD	foot;
	DWORD	wing;

	PC_ATTR()	
	{ 
		r_hand_node = 0; 
		l_hand_node = 0; 
		hair = 0; 
		face = 0; 
		body = 0; 
		pants = 0;
		hand = 0; 
		foot = 0; 
		wing = 0;
	}
};

//-----------------------------------------------------------------------------
// Desc: 유저 추가 사운드정보
//
// 1.유저캐릭터 애니메이션에서 연결되는 사운드가 2가지로 추가되었다
// 2.구조체에 추가되는 사운드인덱스와 키값을 저장하여서 사용하도록 한다
//
// Copyright (c) 2003.11.13 By KStaR79
//-----------------------------------------------------------------------------
struct PC_ADD_SOUND_INFO
{
	int		sound_number;
	int		sound_key;

	PC_ADD_SOUND_INFO()		{ sound_number = -1; sound_key = 1; }
	~PC_ADD_SOUND_INFO()	{}
};


//-----------------------------------------------------------------------------
// Desc: 화살에 붙어 그려지는 효과
//	
// 1.Etc 폴더안에 있는 arrowline(msh.tex)파일을 이용해서 효과를 그려준다
// 2.거리에 따라서 Z축으로 스케일을 늘려주어서 길어지게 보이도록 한다
// 3.발광옵션으로 이펙트처럼 보이도록 한다.
// 
// Copyright (c) 2003.10.25 By KStaR79
//-----------------------------------------------------------------------------
struct	ARROW_LINE
{
	Model		Mod;
	Texture		Tex;
};


typedef struct sTest_ItemChangeID
{
	DWORD			right_hand[100];
	DWORD			left_hand[100];
	DWORD			coat_armor[100];
	DWORD			pants_armor[100];
	DWORD			gloves[100];
	DWORD			shoes[100];
	DWORD			face[100];
	DWORD			hair[100];

	WORD			cnt_right_hand;
	WORD			cnt_left_hand;
	WORD			cnt_coat_armor;
	WORD			cnt_pants_armor;
	WORD			cnt_gloves;
	WORD			cnt_shoes;
	WORD			cnt_face;
	WORD			cnt_hair;

} sTest_ItemChangeID;



struct STableInfo
{
	BYTE	NumStore;
};


typedef enum NDisplayItemIndex
{
	n_Pack = 0,
	n_Eld,
	n_NormalBox,
	n_GoodBox,
	n_QuestBox

} NDisplayItemIndex;

const WORD c_MaxDropItemIndex( 5 );


typedef struct SAutoHelpTipMsg
{	
	long		theAutoHelpTipTime;	
	int			theAutoHelpTipCurIdx;
	
	int			theAutoHelpTipStrNum_Millena;	
	char		theAutoHelpTipAdminMsg_Millena[255][255];

	int			theAutoHelpTipStrNum_Rain;	
	char		theAutoHelpTipAdminMsg_Rain[255][255];

	int			theAutoHelpTipStrNum_PVPChat;
	char		theAutoHelpTipAdminMsg_PVPChat[255][255];

} SAutoHelpTipMsg;


typedef enum NNationType
{
	n_Nation_Millena = 0,
	n_Nation_Rain,
	n_Nation_Common = 9,

} NNationType;

///-- 콜로니 호출타입
typedef enum NColCallType
{
	n_CoPosition = 1,		//진지
	n_CoAttackArams,		//공성병기
	n_CoBuilding,			//성벽/성문
	n_CoGuardTower,			//가드타워

} NColCallType;


typedef struct SScriptString_Client
{	
	DWORD	theStringIndex;
	WORD	theStringLen;
	char*	theString;	

	SScriptString_Client()
	{
		theStringIndex = 0;
		theStringLen = 0;
		theString = NULL;	
	}
	
} SScriptString_Client;

template< class T >
BOOL DELETE_ALL_PTR_MEMBER_MAP(  T & Datalist ) 
{
	T::iterator ir = Datalist.begin();

	for( ; ir != Datalist.end(); ++ir )		
	{
		SAFE_DELETE((*ir).second);		
	}

	Datalist.clear();
	
	return TRUE;
}

#ifdef DYNAMIC_RESOURCE_LOADING

typedef map < DWORD, CHR_FILE_INFO * >::iterator ChrFileInfoPos;
typedef map < DWORD, CHR_FILE_INFO * >::value_type ChrFileInfoValue;

typedef map < DWORD, Model * >::iterator ModelPos;
typedef map < DWORD, Model * >::value_type ModelValue;

typedef map < DWORD, Texture * >::iterator TexturePos;
typedef map < DWORD, Texture * >::value_type TextureValue;

typedef map < DWORD, Character_Item * >::iterator CharItemPos;
typedef map < DWORD, Character_Item * >::value_type CharItemValue;

typedef map < DWORD, Bone * >::iterator BonePos;
typedef map < DWORD, Bone * >::value_type BoneValue;

#endif

//-----------------------------------------------------------------------------
// Desc: Player_Manager( Item_Model, Item_Texture, Item File )
//-----------------------------------------------------------------------------
class Player_Manager
{		
public:	
	// 캐릭터별 아이템 테스팅 4가지 타입 (밀레나 남-녀, 레인 남-녀)
	sTest_ItemChangeID		test_item_changeID[4];	

	SNPCBaseInfo			theNPCInfoTable[c_Max_NPC_Num];
	WORD					theNPCCodeTable[c_Max_NPCCode_Num];
	DWORD					theNPCFileCodeTable[c_Max_NPC_Num];
		
	SItemBaseInfo			theItemInfoTable[c_Max_Item_Num];
	WORD					theItemCodeTable[c_Max_ItemCode_Num];
	DWORD					theItemFileCodeTable[2][c_Max_Item_Num];

	SSkillBaseInfo			theSkillInfoTable[c_Max_Skill_Num];
	WORD					theSkillCodeTable[c_Max_SkillCode_Num];		// 스킬 코드를 가지고 테이블에 바로 접근하기 위해서 미리 계산

	SItemAddEffectInfo		theItemAddEffectInfoTable[c_Max_ItemAddEffect_Num];

	SLevelBaseInfo			theStatInfoTable[c_Max_Stat_Num];	
	WORD					theMainClassTable[c_Max_MainClass_Num];
	SSubLevelBaseInfo		theSubStatInfoTable[c_Max_Stat_Num];
	///WORD					theSubClassTable[c_Max_Stat_Num];


	SQuestInfo				theQuestInfoTable[c_Max_Quest_Count];
	SPreQuestInfo			thePreQuestInfoTable[c_Max_PreQuestInfo_Count];
	
	SScriptString_Client	theScriptStrTable[c_Max_ScriptStr_Num];
	
	SSkillLessonInfo		theSkillLessonInfoTable[c_Max_Skill_Num];
	
	SAddEffectInfo			theAddEffectInfoTable[c_Max_AddEffect_Num]; 

	SFightMapInfo			theFightMapInfoTable[ c_Max_FightMap_Num ];

	STableInfo				theTableInfo;
	SStoreList				theStoreList; 


	//-----------------------------------------------------------------------------
	// 파일갯수
	//-----------------------------------------------------------------------------
	WORD					m_mod_count;
	WORD					m_tex_count;
	WORD					m_ani_count;
	WORD					m_itm_count;

	Model*					m_pMod[c_Chr_Mod_Count+c_Npc_Mod_Count];
	Texture*				m_pTex[c_Chr_Tex_Count+c_Npc_Tex_Count];		
	Character_Item*			m_pItm[c_Chr_Itm_Count+c_Npc_Itm_Count];
	Bone*					m_pAni[c_Chr_Ani_Count+c_Npc_Ani_Count];	

	//-----------------------------------------------------------------------------
	// 캐릭터 파일정보
	//-----------------------------------------------------------------------------
	CHR_FILE_INFO*			m_pPcFileInfo;
	Model*					theChrMod;
	Texture*				theChrTex;
	Character_Item*			theChrItm;
	Bone*					theChrAni;	

#ifdef DYNAMIC_RESOURCE_LOADING

	map < DWORD, CHR_FILE_INFO * >		m_pNpcFileInfo;
	map < DWORD, Model * >				m_NpcMod;
	map < DWORD, Texture * >			m_NpcTex;
	map < Character_Item * >			m_NpcItm;
	map < Bone * >						m_NpcAni;

#else	


	CHR_FILE_INFO*			m_pNpcFileInfo;
	Model*					theNpcMod;
	Texture*				theNpcTex;
	Character_Item*			theNpcItm;
	Bone*					theNpcAni;

#endif

	//-----------------------------------------------------------------------------
	// 유저, NPC 속성
	//-----------------------------------------------------------------------------
	PC_ATTR					m_PcAttr[c_CharType_Count*2];
//	NPC_ATTR				m_NpcAttr[c_NpcType_Count];
	

	//-----------------------------------------------------------------------------
	// 애니메이션 정보	
	//-----------------------------------------------------------------------------
	ANI_INFO				m_PcAni[c_CharType_Count][c_PC_AniIndex_Count];
	ANI_INFO_ACTION			m_PcAni_Act[c_CharType_Count][c_PC_AniIndex_Count];	
	PC_ADD_SOUND_INFO		m_PcAddSound[c_CharType_Count][c_PC_AniIndex_Count];

	//-----------------------------------------------------------------------------
	// 망토 관련 By wxywxy
	//-----------------------------------------------------------------------------
	//CMantleItem			m_Mantle;													

	//-----------------------------------------------------------------------------
	// 맵별 NPC 정보	
	//-----------------------------------------------------------------------------
	SNPC_Set				the_sNpc_SetData;
	SPCItemSet				m_PCItemSetData;

	//-----------------------------------------------------------------------------
	// 화살
	//-----------------------------------------------------------------------------
	CFlyWeaponManager		m_FlyWeaponList;			// By 날아다니는 무기 wxywxy
	ARROW_LINE				m_ArrowLine;
	
	//-----------------------------------------------------------------------------
	// 머리본
	//-----------------------------------------------------------------------------
	Bone					m_HeadAni[c_CharType_Count];

	//-----------------------------------------------------------------------------
	// 소팅테이블
	//-----------------------------------------------------------------------------
	long					m_lPreSortTime;
	//CHR_SORT_TABLE*			m_pSortTable;


	// 게임 분위기를 위해서 수동 조절...
#ifdef DIRECT_VERSION_9_MJH
	D3DLIGHT9				theCharacter_Light;
	D3DLIGHT9				theObject_Light;
	D3DLIGHT9				theTerrain_Light;
	D3DLIGHT9				theSky_Light;
	D3DMATERIAL9			theCharacter_Material;
	D3DMATERIAL9			theObject_Material;
	D3DMATERIAL9			theTerrain_Material;
	D3DMATERIAL9			theSky_Material;
#else
	D3DLIGHT8				theCharacter_Light;
	D3DLIGHT8				theObject_Light;
	D3DLIGHT8				theTerrain_Light;
	D3DLIGHT8				theSky_Light;
	D3DMATERIAL8			theCharacter_Material;
	D3DMATERIAL8			theObject_Material;
	D3DMATERIAL8			theTerrain_Material;
	D3DMATERIAL8			theSky_Material;
#endif // DIRECT_VERSION_9_MJH
	float					theFog_Start;
	float					theFog_End;
	D3DVECTOR				theFog_Color;

	bool					ReadLightSet( char* file_pos );
	bool					ReadLightSetVF();
	
	SAutoHelpTipMsg			theAutoHelpTipMsg;

	CCharImage				m_CharImage;
	
	char*					theGameModeDesc[c_Max_GameMode_Num];			// 게임 모드별 설명 

public:
	
	 Player_Manager()
	 { 
		 Clear();	 
	 }

	~Player_Manager()		{ Destroy(); }

	//-----------------------------------------------------------------------------
	// 초기화
	//-----------------------------------------------------------------------------
	void	Clear();
	void	Destroy();
	void	DestroyPC( int aMapIndex );
	void	DestroyNPC();

	//-----------------------------------------------------------------------------
	// 오브젝트에 관련된 파일 메모리에 할당(모델,텍스쳐,애니메이션)
	//-----------------------------------------------------------------------------
	bool	Create( char* file_pos );
	bool	CreatePC( char* file_pos );	
	bool 	CreateNPC( char* file_pos );
	bool	MapChangePC( char* file_pos );
	bool	MapChangeNPC( char* file_pos );
	
	int		GetFileCount( char* file_pos, char* file_ext );

	//-----------------------------------------------------------------------------
	// 소팅처리함수
	//-----------------------------------------------------------------------------
	void	SetSortInfo( BYTE _byType, int _nIndex );
	void	ClearSortInfo( BYTE _byType, int _nIndex );
	void	UpdateSortTable();
	void	InitSortTable();

	//-----------------------------------------------------------------------------
	// 화살처리
	//-----------------------------------------------------------------------------	
	void	CreateArrow( bool PC, int table_index, BYTE link, D3DXVECTOR3 Curt, D3DXVECTOR3 Dest);
	void	DisplayArrow();	
	void	ClearArrow( bool bPC, int table_index );
	D3DXMATRIX NodeAnimaion( int num, int node, int local, int frame, float time );
	
	void	CreateArrowLine( char* file_pos );
	void	DisplayLineArrow();
	
	//-----------------------------------------------------------------------------
	// 속성파일읽기s
	//-----------------------------------------------------------------------------
	bool	Set__ItemAttr( const char* file_pos );	
	bool	Set__ItemAttrVF();		
	void	Set__ItemChangeID();
	bool	Set__PcAttr( const char* file_pos );
	bool	Set__PcAttrVF();
	bool	Set__PcAnimationList( const char* file_pos, int index );
	bool	Set__PcAnimationListVF( const char* file_pos, int index );
	bool	Set__TextureAttr( const char* file_pos );

//	bool	Set__NpcAttr( const char* file_pos, char* filename );

	bool	Set__NpcSet( const char* file_pos, int curMapIndex );
	bool	Set__NpcSetVF( int curMapIndex );
	void	ReSet__NpcSetVF( int curMapIndex );

	//-----------------------------------------------------------------------------
	// 공격아이템
	//-----------------------------------------------------------------------------
	bool	GetAttackItem();
	int		GetAttackItemIndex( Character * pChar, int table_index );
	int		GetEqualItemIndex( int itm );

	//-----------------------------------------------------------------------------
	// 바운드박스 
	//-----------------------------------------------------------------------------
	D3DXVECTOR3	GetBoundMinPos( LPVOID _pChr );
	D3DXVECTOR3	GetBoundMaxPos( LPVOID _pChr );

	//-----------------------------------------------------------------------------
	// 충돌 바운드박스 
	//-----------------------------------------------------------------------------
	D3DXVECTOR3	GetCBoundMinPos( LPVOID _pChr );
	D3DXVECTOR3	GetCBoundMaxPos( LPVOID _pChr );

	D3DXVECTOR3	GetMinBoundVec( LPVOID _pChr );
	D3DXVECTOR3	GetMaxBoundVec( LPVOID _pChr );
	void SetMinBoundVec( LPVOID _pChr, D3DXVECTOR3 aNewBoundPos );
	void SetMaxBoundVec( LPVOID _pChr, D3DXVECTOR3 aNewBoundPos );

	//-----------------------------------------------------------------------------
	// 인덱스(아이템, 텍스쳐, 모델)
	//-----------------------------------------------------------------------------		
	int				GetItmIndex( LPVOID _pChr, int _nIndex );
	int				GetItmCount( LPVOID _pChr );
	int				GetTexIndex( LPVOID _pChr, int _nIndex );
	int				GetModIndex( LPVOID _pChr, int _nIndex );
	int				GetBasicTexIndex( LPVOID _pChr, int _nIndex );
	int				GetBasicModIndex( LPVOID _pChr, int _nIndex );
	int				GetRideTexIndex( LPVOID _pChr, int _nIndex );
	int				GetRideModIndex( LPVOID _pChr, int _nIndex );
	int				GetInitItmIndex( LPVOID _pChr, int _nIndex );
	bool			GetEmitterItm( LPVOID _pChr, int _nIndex );
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9	GetItmMaterial( LPVOID _pChr, int _nIndex );
#else
	D3DMATERIAL8	GetItmMaterial( LPVOID _pChr, int _nIndex );
#endif // DIRECT_VERSION_9_MJH
	int				GetArmorIndex( LPVOID _pChr, DWORD _dwNode );
	
	///-- By simwoosung
	DWORD			GetRealItemFileCode( LPVOID _pChr, int _nIndex );
	BOOL		    IsEmblemItem( LPVOID _pChr, DWORD dRealFileIndex );
	BOOL			IsNoApplyAlphaTestItem( LPVOID _pChr, DWORD dRealFileIndex );

	//-----------------------------------------------------------------------------
	// 애니메이션
	//-----------------------------------------------------------------------------
	D3DXMATRIX		GetAniLocalTM( LPVOID _pChr, int _nNumber, int _nNode );
	D3DXQUATERNION	GetAniQuaternion( int _nNumber, int _nNode, int _nFrame );
	D3DXVECTOR3		GetAniPos( int _nNumber, int _nNode, int _nFrame );
	int				GetSoundNumber( LPVOID _pChr, int _nAniIndex );
	int				GetSoundkey( LPVOID _pChr, int _nAniIndex );
	int				GetAniNumber( LPVOID _pChr, int _nAniIndex );
	int				GetEndFrame( LPVOID _pChr, int _nAniIndex );
	int				GetEndFrame( int _Num )	{ return m_pAni[_Num]->m_ani_frame; }
	int				GetVtxCount( LPVOID _pChr, int _nItmIndex, BYTE _byType = 0 );
	int				GetBasicVtxCount( LPVOID _pChr, int _nItmIndex, BYTE _byType = 0 );
	int				GetRideVtxCount( LPVOID _pChr, int _nItmIndex, BYTE _byType = 0 );
	D3DXVECTOR3		GetChrScale( LPVOID _pChr );
	

	//-----------------------------------------------------------------------------
	// 속성값 리턴
	//-----------------------------------------------------------------------------
	int		GetItemID( DWORD item_id );
//	int		GetItemType( DWORD item_id );
	BYTE	GetNpcBlockType( int _nIndex );
	//int		GetItemSound( int index );
	
	D3DXVECTOR3	GetVertex( int index, int number );	
	D3DXVECTOR3	GetLinkVertex( int index, int number );	

	DWORD	SetItemID( BYTE aSex, BYTE aRace, BYTE aGrav, BYTE aClass, BYTE aType, BYTE aNumber);

	/////////////////////////////////////////////////////////////////////////////////
	/// ActionSystem
	/////////////////////////////////////////////////////////////////////////////////
	int		GetRangeStartFrame( LPVOID _pChr, int _nAniIndex );
	int		GetRangeEndFrame( LPVOID _pChr, int _nAniIndex );
	int		GetImpactFrame( LPVOID _pChr, int _nAniIndex );
	
	//--------------------------------------------------------------------------------------
	/////////////////////////////////////////////////////////////////////////////////
	// Table Info Input System ( NPC, ITEM, SKILL... )
	/////////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------------------------
	bool	LoadNPCTable( char* file_pos );
	bool	LoadItemTable( char* file_pos );
	bool	LoadSkillTable( char* file_pos );
	bool	LoadStatTable( char* file_pos );
	bool	LoadStoreTable( char* file_pos );
	bool	LoadQuestTable( char* file_pos );
	bool	LoadQuestSignalTable( char* file_pos );
	bool	LoadTableInfo( char* file_pos );	
	bool	LoadSkillLessonTable( char* file_pos );
	bool	LoadAddEffectInfoTable( char* file_pos );
	bool	LoadItemAddEffectInfoTable( char* file_pos );
	bool	LoadSubStatTable( char* file_pos );
	bool	LoadFightMapInfoTable( char* file_pos );

	bool	LoadNPCTableVF();
	bool	LoadItemTableVF();
	bool	LoadSkillTableVF();
	bool	LoadStatTableVF();
	bool	LoadSubStatTableVF();
	bool	LoadStoreTableVF();
	bool	LoadQuestTableVF();
	bool	LoadQuestSignalTableVF();
	bool	LoadTableInfoVF();
	bool	LoadSkillLessonTableVF();
	bool	LoadAddEffectInfoTableVF();
	bool	LoadItemAddEffectInfoTableVF();
	bool	LoadFightMapInfoTableVF();
	bool	LoadEtcFileVF();

	//--------------------------------------------------------------------------------------
	// NPC
	//--------------------------------------------------------------------------------------
	void	SetNpcFileCode( int aChrIndex, DWORD aFileName ); // Chr 파일을 만들때 NPC Table 의 각각의 파일명을 비교해 실제 인덱스 값을 저장한다 
	SNPCBaseInfo* GetNPCBaseInfoPtr( DWORD aNPCCode );
	SNPCBaseInfo* GetNPCBaseInfo(int _nIndex);
	DWORD GetNPCRealFileCode( DWORD aNPCCode );
	//--------------------------------------------------------------------------------------
	// SKILL
	//--------------------------------------------------------------------------------------
	//....sh
	SSkillBaseInfo* GetSkillBaseInfoPtr( DWORD aSkillCode );
	WORD GetSkillIconTID( DWORD aSkillCode );
	WORD GetSkillAttriIconTID( DWORD aSkillCode );
	WORD GetSkillContinualIconTID( DWORD _skillcode );//지연스킬 아이콘	theAddEffectIcon
	WORD GetNextSkillLevel( DWORD _skillcode );

	//--------------------------------------------------------------------------------------
	// ITEM 
	//--------------------------------------------------------------------------------------
	void	SetItemFileCode12( int aItmIndex, DWORD aFileName );
	//....sh
#ifdef DECO_RENEWAL_GUILD_MJH
	SItemBaseInfo*	GetItemBaseInfoIndexPtr( BYTE ItemIndex );	
#endif // DECO_RENEWAL_GUILD_MJH
	SItemBaseInfo*	GetItemBaseInfoPtr( DWORD ItemCode );
	DWORD GetItemInfo_FileCode12( int ChrAttrIndex, DWORD ItemCode );
	DWORD GetItemRealFileCode( int ChrAttrIndex, DWORD ItemCode );
	WORD GetItemTID( DWORD ItemCode );
	BYTE GetItemClsss( DWORD ItemCode );
	BYTE GetItemType( DWORD ItemCode );
	
	SItemAddEffectInfo * GetItemAddEffectInfo( BYTE dAddCode );
	
	//--------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------
	SLevelBaseInfo*		GetStatBaseInfo( BYTE _main_class );
	int	GetStatSkillNeedPoint( BYTE _main_class, int _level );
	DWORD	GetStatNeedExp( BYTE _main_class, int _level );
	DWORD	GetStatNeedSubExp( BYTE _main_class, int _level );

	LONGLONG GetStatNeedExp64( BYTE _main_class, int _level );
	
	//--------------------------------------------------------------------------------------
	// Store Shop
	//--------------------------------------------------------------------------------------
	SStoreList*			GetStoreList();
	SStoreShop*			GetStoreShop( WORD OwnerCode );
	N_DEALINGITEM		GetDealingItem( SItemBaseInfo * pItemBaseInfo , int OwnerCode );
	BOOL				IsColonyItem( SItemBaseInfo * pItemBaseInfo );
	N_DEALINGITEM		IsSealHolyGroundNPC(int NpcCode);
	//--------------------------------------------------------------------------------------
	// Quest Shop
	//--------------------------------------------------------------------------------------
	SQuestInfo*			GetQuestInfoTable();
	int					GetQuestType( WORD _quest_code );//1:일반, 2:스토리, 3:전직
	SQuestInfo*			GetQuestFlagInfo( WORD _quest_code, WORD _flag );
	
	// Script String
	bool ReadScriptStrTable( const char* file_pos );
	bool ReadScriptStrTableVF();

	void ConvertNameString( void* aOrigin, void* aResult );

	bool ReadAutoHelpTip( char* file_pos );
	bool ReadAutoHelpTipVF();
	bool ReadAutoHelpTipVFW();
	/////////////////////////////////////////////////////////////////////////////////////////
	// 맵에 따라서 캐릭터 로딩 
	/////////////////////////////////////////////////////////////////////////////////////////
	int					theNationType;

	int 				GetMapNationType( int aMapIndex );					// 맵 인덱스에 따라서 국가 설정 
	bool				CheckLoadAniList_LoginFeild( int aFileName );		// 맨 처음 로딩이나 로긴맵에서는 선택 애니만 로딩 


	bool		ReadGameModeDesc( char* file_pos );
	bool		ReadGameModeDescVF();
	bool		ReadGameModeDescVFW();

	// 근접 유료 무기 사용시 쓸수 있는 스킬 체크
	BOOL		ISUseSkillOnApproachWeapon(int Code);
	
	///-- simwoosung - Set파일에 없는 인덱스가 들어올때 추가루틴
	BOOL AddNpcCode( int nNpcCode );
	void SetNpcDataArray();
	
	bool IsFindFileData( DWORD index );
	
	///-- 공성병기를 사용할수 있는 상태인지를 체크
	BOOL IsCanCallColonyItem();

public:
	
	CNPCImpactTable		m_NPCImpactTable;
	CHolyManager		m_HolyManager;
	CPlayerNPCDataManager	m_PlayerNPCDataManager;
	CSourceDataManager		m_SourceDataManager;						
};		

extern Player_Manager	g_Pc_Manager;

#endif