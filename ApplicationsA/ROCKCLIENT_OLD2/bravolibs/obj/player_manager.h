
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
#include	"CMantleItem.h"								// ���� by wxywxy
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

// mod ����
#define		CHR_MOD_COUNT		180
#define		NPC_MOD_COUNT		45

// tex ����
#define		CHR_TEX_COUNT		214
#define		NPC_TEX_COUNT		58

// ani ����
#define		CHR_ANI_COUNT		69
#define		NPC_ANI_COUNT		255

// itm ����
#define		CHR_ITM_COUNT		421
#define		NPC_ITM_COUNT		66

// NPC
#define		NPCNAME_SIZE		16
#define		SPEECH_SIZE			60
#define		NPC_SPEECHCOUNT		12

// ȭ�����̺���
#define		ARROW_COUNT			32

// ����, NPC �Ӽ�����
#define		NPC_TYPE_COUNT		81
#define		CHR_TYPE_COUNT		5

// Attr Num
const WORD	c_CharType_Count( 4 );		// ĳ���� ���� ��(�з���, ����) ����(��, ��) ���� �� 4���� 
const WORD	c_NpcType_Count( 250 );		// �� �ʿ� ���� �ִ� NPC ���� 50 ���� ( �ż� 20 �� �⺻���� �Է� By wxywxy )

// Mod Num
const WORD	c_Chr_Mod_Count( 2000 );		// 2004. 4. 13 ���� 100 �� 
const WORD	c_Npc_Mod_Count( 500 );		

// Tex Num
const WORD	c_Chr_Tex_Count( 1000 );
const WORD	c_Npc_Tex_Count( 500 );		// ������ �� msh �� �Ѱ��� �ؽ��� ���

// Ani Num
const WORD	c_Chr_Ani_Count( 2000 );		// ���� �ִ� �� ���� - ĳ�� �� Ÿ�Դ� ���� 300 �� ������...4 Ÿ��...200 * 4
const WORD	c_Npc_Ani_Count( 3000 );	// �� NPC �� �ִ� 25���� ����� ���� 50 * 30 

// Itm Num
const WORD	c_Chr_Itm_Count( 3000 );
const WORD	c_Npc_Itm_Count( 500 );

// Quest Num
const WORD  c_Max_Quest_Count( 2000 );			// ����Ʈ ī��Ʈ
const WORD  c_Max_PreQuestInfo_Count( 1000 );			// ����Ʈ �ñ׳� ī��Ʈ

const WORD	c_PC_AniIndex_Count( 500 );
const WORD	c_Npc_AniIndex_Count( 30 );			// Rock_Npc Total_Ani_Type ���� 

const WORD	c_Max_NPC_Num( 3000 );				// NPC �ִ� ���� 
const WORD	c_Max_NPCCode_Num( 8000 );			// NPC Code �ִ� ���� 

const WORD	c_Max_Item_Num( 15000 );			// ITEM ���� �ִ� ���� 
const WORD	c_Max_ItemCode_Num( 65535 );		// Item Code �ִ� ����

const WORD  c_Max_ItemAddEffect_Num( 1000 );		// ������  �ΰ�ȿ�� Ÿ���� ����
 

const WORD	c_Max_Skill_Num( 3000 );			// SKILL �ִ� ���� 
const WORD	c_Max_SkillCode_Num( 10000 );		// SKILL Code �ִ� ����


const WORD	c_Max_Stat_Num( 1000 );				// Stat �ִ� ���� 
const WORD	c_Max_MainClass_Num( 200 );			// 

const WORD	c_Max_ScriptStr_Num( 30000 );		// Script String �ִ� ���� 

const WORD	c_Max_AddEffect_Num( 100 );			// �ΰ� ȿ�� Ÿ���� ���� 
// ���� �ý��� 
const WORD c_Max_FightMap_Num( 20 );			// ���� �ý��� ��� �� ���� 
const WORD c_Max_GameMode_Num( 5 );				// ���� �ý��� ���� ��� ����
 

//-----------------------------------------------------------------------------
// Desc: ���ļ��� ���̺�
//
// 1. ����Ÿ���� ������ �����ϴ� ����ü
// 2. �迭���� ��ġ���� �̿��ϸ� �ش� ����ü�Լ��� �̿��Ͽ��� ��� ó���� �ϵ��� �Ѵ�
// 3. ����Ÿ������ �޴� ����ü�� �˸°� ó���ǵ��� ���������� �����ϴ� ���� ����
//    
// Copyright (c) 2003.12.22 By KStaR79
//-----------------------------------------------------------------------------
struct CHR_SORT_TABLE
{	
	BYTE	byType;		// Ÿ��
	short	Index;		// �ε���
	
	CHR_SORT_TABLE()	{ Clear(); }
	void	Clear()		{ byType = 255; Index = 0; }
};

///-----------------------------------------------------------------------------
/// Desc : �� Map ���� �������� NPC ���� �����ϰ� �ִ�
/// author : niceguy 
/// date : 2004. 04. 12
///-----------------------------------------------------------------------------

///By simwoosung - NPC�� chr���ϴ����� PC�� itm���� ������ ����Ÿ ����
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
// Desc: NPC �Ӽ�
//
// NPC�Ӽ��� ���õ� �������� �����ϰ� �ִ�
//
// Copyright (c) 2003.12.29 By KStaR79
//-----------------------------------------------------------------------------
struct NPC_SSPEECH
{
	char	speech[SPEECH_SIZE];		// ���(����-�̵�-���-�Ҹ�-������)
	NPC_SSPEECH() { memset(&speech, 0, sizeof(speech)); }
};

struct NPC_ATTR
{
	WORD		id;							// �ε���
	char		name[NPCNAME_SIZE];			// �̸�
	BYTE		attack;						// ���� ��������
	BYTE		respawn;					// �������
	BYTE		die;						// �Ҹ���
	BYTE		set;						// ��Ʈ��ȣ
	BYTE		block;						// ��Ÿ��

//	vector <WORD>	SoundList;				// �� Ÿ�� ���� ����Ʈ By wxywxy	

	NPC_SSPEECH	Speech[NPC_SPEECHCOUNT];	// ���

	NPC_ATTR()  
	{ 
		attack = 0; respawn = 0; die = 0; set = 0; block = 0;
		memset( &name, 0, sizeof(name) ); 
	}

};


//-----------------------------------------------------------------------------
// Desc: brv_chr ��������
//
// ��������(brv_chr)�� ������ �ִ� ����ü.
// ������ ĳ���Ͱ� �ش� ����ü�� ��ũ�Ͽ� �������� ����ϵ��� �Ѵ�
//
// Copyright (c) 2003.12.28 By KStaR79
//-----------------------------------------------------------------------------
struct CHR_FILE_INFO
{
	float			fAniTime;		// �ִ�Ÿ��(���X)
	float			fStep;			// �̵�����(���X)
	D3DXVECTOR3		vScale;			// ������
	ALPHA_INFO		AlphaInfo;		// ��������(���X)
	BOUND_INFO		BoundInfo;		// �ٿ��ڽ�
	ANI_INFO*		pAniInfo;		// �ִϸ��̼�����(���Ŀ��� ������� �ʾƾ� �Ѵ�)
	int				nItmCount;		// �����۰���
	int*			pItmIndex;		// �������ε���
	ARMOR_INFO		ArmorInfo;		// ���⸵ũ����(Only Npc Use)
	unsigned int	m_Index;		// �����ε���
	D3DXVECTOR3		minBoundVec;	// �ּ� 
	D3DXVECTOR3		maxBoundVec;	// �ִ�
	BOUND_INFO		CollBoundInfo;	// �浹�ٿ��ڽ�
	

	CHR_FILE_INFO()		{ Clear(); }
	~CHR_FILE_INFO()	{ Destroy(); }

	void	Clear();		// �ʱ�ȭ
	void	Destroy();		// �ı���

	bool	FileRead( char* szFilePath, BYTE byType );	// brv_chr �����б�
	bool	FileReadVF( CVFileSystem* pVF, char* szFilePath, BYTE byType );	/// sooree pack

	void	ReSetBoundBox(D3DXVECTOR3 & min, D3DXVECTOR3 & max);
};


//-----------------------------------------------------------------------------
// Desc: ���� �Ӽ�
//-----------------------------------------------------------------------------
struct	PC_ATTR
{
	BYTE	center_node;				// �߽� ��
	BYTE	r_hand_node;				// ������ ��
	BYTE	l_hand_node;				// �޼� ��
	BYTE	breast_node;				// ���� ��
	BYTE	hair_node;					// �Ӹ� ��
	BYTE	r_foot_node;				// ������ ��
	BYTE	l_foot_node;				// �޹� ��
	BYTE	l_shoulder_node;			// �޾�� 
	BYTE	r_shoulder_node;			// �������
	BYTE	l_elbow_node;				// �� ��ġ 
	BYTE	r_elbow_node;				// ���� ��ġ
	BYTE	l_knee_node;				// �� ���� 
	BYTE	r_knee_node;				// ���� ���� 
	BYTE	l_pelvis_node;				// �� ��� 
	BYTE	r_pelvis_node;				// ���� ��� 
	
	
	
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
// Desc: ���� �߰� ��������
//
// 1.����ĳ���� �ִϸ��̼ǿ��� ����Ǵ� ���尡 2������ �߰��Ǿ���
// 2.����ü�� �߰��Ǵ� �����ε����� Ű���� �����Ͽ��� ����ϵ��� �Ѵ�
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
// Desc: ȭ�쿡 �پ� �׷����� ȿ��
//	
// 1.Etc �����ȿ� �ִ� arrowline(msh.tex)������ �̿��ؼ� ȿ���� �׷��ش�
// 2.�Ÿ��� ���� Z������ �������� �÷��־ ������� ���̵��� �Ѵ�
// 3.�߱��ɼ����� ����Ʈó�� ���̵��� �Ѵ�.
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

///-- �ݷδ� ȣ��Ÿ��
typedef enum NColCallType
{
	n_CoPosition = 1,		//����
	n_CoAttackArams,		//��������
	n_CoBuilding,			//����/����
	n_CoGuardTower,			//����Ÿ��

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
	// ĳ���ͺ� ������ �׽��� 4���� Ÿ�� (�з��� ��-��, ���� ��-��)
	sTest_ItemChangeID		test_item_changeID[4];	

	SNPCBaseInfo			theNPCInfoTable[c_Max_NPC_Num];
	WORD					theNPCCodeTable[c_Max_NPCCode_Num];
	DWORD					theNPCFileCodeTable[c_Max_NPC_Num];
		
	SItemBaseInfo			theItemInfoTable[c_Max_Item_Num];
	WORD					theItemCodeTable[c_Max_ItemCode_Num];
	DWORD					theItemFileCodeTable[2][c_Max_Item_Num];

	SSkillBaseInfo			theSkillInfoTable[c_Max_Skill_Num];
	WORD					theSkillCodeTable[c_Max_SkillCode_Num];		// ��ų �ڵ带 ������ ���̺� �ٷ� �����ϱ� ���ؼ� �̸� ���

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
	// ���ϰ���
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
	// ĳ���� ��������
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
	// ����, NPC �Ӽ�
	//-----------------------------------------------------------------------------
	PC_ATTR					m_PcAttr[c_CharType_Count*2];
//	NPC_ATTR				m_NpcAttr[c_NpcType_Count];
	

	//-----------------------------------------------------------------------------
	// �ִϸ��̼� ����	
	//-----------------------------------------------------------------------------
	ANI_INFO				m_PcAni[c_CharType_Count][c_PC_AniIndex_Count];
	ANI_INFO_ACTION			m_PcAni_Act[c_CharType_Count][c_PC_AniIndex_Count];	
	PC_ADD_SOUND_INFO		m_PcAddSound[c_CharType_Count][c_PC_AniIndex_Count];

	//-----------------------------------------------------------------------------
	// ���� ���� By wxywxy
	//-----------------------------------------------------------------------------
	//CMantleItem			m_Mantle;													

	//-----------------------------------------------------------------------------
	// �ʺ� NPC ����	
	//-----------------------------------------------------------------------------
	SNPC_Set				the_sNpc_SetData;
	SPCItemSet				m_PCItemSetData;

	//-----------------------------------------------------------------------------
	// ȭ��
	//-----------------------------------------------------------------------------
	CFlyWeaponManager		m_FlyWeaponList;			// By ���ƴٴϴ� ���� wxywxy
	ARROW_LINE				m_ArrowLine;
	
	//-----------------------------------------------------------------------------
	// �Ӹ���
	//-----------------------------------------------------------------------------
	Bone					m_HeadAni[c_CharType_Count];

	//-----------------------------------------------------------------------------
	// �������̺�
	//-----------------------------------------------------------------------------
	long					m_lPreSortTime;
	//CHR_SORT_TABLE*			m_pSortTable;


	// ���� �����⸦ ���ؼ� ���� ����...
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
	
	char*					theGameModeDesc[c_Max_GameMode_Num];			// ���� ��庰 ���� 

public:
	
	 Player_Manager()
	 { 
		 Clear();	 
	 }

	~Player_Manager()		{ Destroy(); }

	//-----------------------------------------------------------------------------
	// �ʱ�ȭ
	//-----------------------------------------------------------------------------
	void	Clear();
	void	Destroy();
	void	DestroyPC( int aMapIndex );
	void	DestroyNPC();

	//-----------------------------------------------------------------------------
	// ������Ʈ�� ���õ� ���� �޸𸮿� �Ҵ�(��,�ؽ���,�ִϸ��̼�)
	//-----------------------------------------------------------------------------
	bool	Create( char* file_pos );
	bool	CreatePC( char* file_pos );	
	bool 	CreateNPC( char* file_pos );
	bool	MapChangePC( char* file_pos );
	bool	MapChangeNPC( char* file_pos );
	
	int		GetFileCount( char* file_pos, char* file_ext );

	//-----------------------------------------------------------------------------
	// ����ó���Լ�
	//-----------------------------------------------------------------------------
	void	SetSortInfo( BYTE _byType, int _nIndex );
	void	ClearSortInfo( BYTE _byType, int _nIndex );
	void	UpdateSortTable();
	void	InitSortTable();

	//-----------------------------------------------------------------------------
	// ȭ��ó��
	//-----------------------------------------------------------------------------	
	void	CreateArrow( bool PC, int table_index, BYTE link, D3DXVECTOR3 Curt, D3DXVECTOR3 Dest);
	void	DisplayArrow();	
	void	ClearArrow( bool bPC, int table_index );
	D3DXMATRIX NodeAnimaion( int num, int node, int local, int frame, float time );
	
	void	CreateArrowLine( char* file_pos );
	void	DisplayLineArrow();
	
	//-----------------------------------------------------------------------------
	// �Ӽ������б�s
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
	// ���ݾ�����
	//-----------------------------------------------------------------------------
	bool	GetAttackItem();
	int		GetAttackItemIndex( Character * pChar, int table_index );
	int		GetEqualItemIndex( int itm );

	//-----------------------------------------------------------------------------
	// �ٿ��ڽ� 
	//-----------------------------------------------------------------------------
	D3DXVECTOR3	GetBoundMinPos( LPVOID _pChr );
	D3DXVECTOR3	GetBoundMaxPos( LPVOID _pChr );

	//-----------------------------------------------------------------------------
	// �浹 �ٿ��ڽ� 
	//-----------------------------------------------------------------------------
	D3DXVECTOR3	GetCBoundMinPos( LPVOID _pChr );
	D3DXVECTOR3	GetCBoundMaxPos( LPVOID _pChr );

	D3DXVECTOR3	GetMinBoundVec( LPVOID _pChr );
	D3DXVECTOR3	GetMaxBoundVec( LPVOID _pChr );
	void SetMinBoundVec( LPVOID _pChr, D3DXVECTOR3 aNewBoundPos );
	void SetMaxBoundVec( LPVOID _pChr, D3DXVECTOR3 aNewBoundPos );

	//-----------------------------------------------------------------------------
	// �ε���(������, �ؽ���, ��)
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
	// �ִϸ��̼�
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
	// �Ӽ��� ����
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
	void	SetNpcFileCode( int aChrIndex, DWORD aFileName ); // Chr ������ ���鶧 NPC Table �� ������ ���ϸ��� ���� ���� �ε��� ���� �����Ѵ� 
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
	WORD GetSkillContinualIconTID( DWORD _skillcode );//������ų ������	theAddEffectIcon
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
	int					GetQuestType( WORD _quest_code );//1:�Ϲ�, 2:���丮, 3:����
	SQuestInfo*			GetQuestFlagInfo( WORD _quest_code, WORD _flag );
	
	// Script String
	bool ReadScriptStrTable( const char* file_pos );
	bool ReadScriptStrTableVF();

	void ConvertNameString( void* aOrigin, void* aResult );

	bool ReadAutoHelpTip( char* file_pos );
	bool ReadAutoHelpTipVF();
	bool ReadAutoHelpTipVFW();
	/////////////////////////////////////////////////////////////////////////////////////////
	// �ʿ� ���� ĳ���� �ε� 
	/////////////////////////////////////////////////////////////////////////////////////////
	int					theNationType;

	int 				GetMapNationType( int aMapIndex );					// �� �ε����� ���� ���� ���� 
	bool				CheckLoadAniList_LoginFeild( int aFileName );		// �� ó�� �ε��̳� �α�ʿ����� ���� �ִϸ� �ε� 


	bool		ReadGameModeDesc( char* file_pos );
	bool		ReadGameModeDescVF();
	bool		ReadGameModeDescVFW();

	// ���� ���� ���� ���� ���� �ִ� ��ų üũ
	BOOL		ISUseSkillOnApproachWeapon(int Code);
	
	///-- simwoosung - Set���Ͽ� ���� �ε����� ���ö� �߰���ƾ
	BOOL AddNpcCode( int nNpcCode );
	void SetNpcDataArray();
	
	bool IsFindFileData( DWORD index );
	
	///-- �������⸦ ����Ҽ� �ִ� ���������� üũ
	BOOL IsCanCallColonyItem();

public:
	
	CNPCImpactTable		m_NPCImpactTable;
	CHolyManager		m_HolyManager;
	CPlayerNPCDataManager	m_PlayerNPCDataManager;
	CSourceDataManager		m_SourceDataManager;						
};		

extern Player_Manager	g_Pc_Manager;

#endif