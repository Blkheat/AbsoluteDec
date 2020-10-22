
#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <memory.h>

// PC, NPC ���� �з�...��, ������, ����, ����Ʈ...
const int c_PcRace_User( 0 );				// �ٸ� ������ 
const int c_PcRace_Illusion( 1 );			// �н� ��ų�� ���� ������ ȯ���� 

const int c_NpcRace_Mob( -1 );				// ���� �� ���� �� 
const int c_NpcRace_Item( -2 );				// ������ 
const int c_NpcRace_Transpotation( -3 );	// ��� ���� 

const int c_NpcIndex_Item( 1 );				// ������!!!...


//-----------------------------------------------------------------------------
// Desc: ����
//-----------------------------------------------------------------------------
enum Skill_Race
{
	UNION = 0,
	DADE,
	JENI,
};


//-----------------------------------------------------------------------------
// Desc: ����
//-----------------------------------------------------------------------------
enum Skill_Job
{
	NOVICE = 0,		// ����
	KNIGHT,			// ���
	ARCHER,			// �ü�
	MAGICIAN,		// ������
	FAIRY,			// ��
	ELIADEN,		// �����Ƶ�
	ROIEL,			// ���̿�
};


//-----------------------------------------------------------------------------
// Desc: ����Ʈ
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
// Desc: ���
//-----------------------------------------------------------------------------
enum MODE_TYPE
{
	MODE_CHARGE = 0,	// 0 ����
	MODE_DEFENSE,		// 1 ���潺
	MODE_MOVE,			// 2 �ȱ�-�ٱ�
	MODE_PK,			// 3 PK
	MODE_ATTACK,		// 4 attack����
	MODE_DOWN,			// 5 �ɱ�-�Ͼ��
	MODE_OPERATOR,		// 6 ���
	MODE_PARTYOK = 10,	// 10 ��Ƽ �ź� 
	MODE_TRADEOK,		// 11 ��ȯ �ź�
	MODE_POSSESSIONOK,	// 12 ��ü �ź�
};


//-----------------------------------------------------------------------------
// Desc: ���⸵ũ
//-----------------------------------------------------------------------------
enum WEAPON_TAKE_INDEX
{
	TAKE_RHAND = 0,		// ������
	TAKE_LHAND,			// �޼�
	TAKE_ETC1,			// ��Ÿ
	TAKE_ETC2,			// ��Ÿ2
};


//-----------------------------------------------------------------------------
// Desc: ��Ÿ������
//-----------------------------------------------------------------------------
enum ETC_ITEM_INDEX
{
	ETC_NECTA = 1,		// ��Ÿ
	ETC_TAR,			// Ÿ��
	ETC_CHAIR,			// ����
};


//-----------------------------------------------------------------------------
// Desc: �ؽ��ļӼ�
//-----------------------------------------------------------------------------
enum DATA_ATTR
{
	DATA_DELETE0 = 0,	// �������� �ؽ���
	DATA_DELETE1,		// �������� �ؽ���
	DATA_NON_DELETE,	// �����Ұ� �ؽ���
};


//-----------------------------------------------------------------------------
// Desc: ����Ÿ��
//-----------------------------------------------------------------------------
enum SORT_TYPE
{
	SORT_NPC = 0,	// NPC
	SORT_PC,		// PC
	SORT_OBJ,		// OBJ
};


//-----------------------------------------------------------------------------
// Desc: ���÷���Ÿ��
//-----------------------------------------------------------------------------
enum DISPLAY_TYPE
{
	DISPLAY_NORMAL = 0,	// �Ϲ�
	DISPLAY_SELECT,		// ����â
	DISPLAY_MAKE,		// ����â
};


//-----------------------------------------------------------------------------
// Desc: ����Ÿ��
//-----------------------------------------------------------------------------
enum POTION_TYPE
{
	USE_HP_POTION = 0,	// ��������	0
	USE_MP_POTION,		// ��������	1
	USE_NECTA,			// ��Ÿ		2
	USE_TAR,			// Ÿ��		3
	USE_HORSE_FOOD,		// ����		4
};


//-----------------------------------------------------------------------------
// Desc: ��ų, ������ ��� 
//-----------------------------------------------------------------------------
#define LEARN_SKILL					253		// ��ų ��ﶧ��,�־ ���. �����ÿ��� ������ ���� �����ϰ��� �ϴ� ���� ��ȣ
#define LEARN_SKILL_SUC 			254		// ����
#define LEARN_SKILL_FAIL			255		// ����
#define CHANGE_CLASS_SUC			251		// ���� ����
#define CHANGE_CLASS_FAIL			252		// ���� ����

/*
// ��罺ų
#define		KNIGHT_SKILL_0				0	// ������ο�		
#define		KNIGHT_SKILL_1				1	// ����Ʈü��		
#define		KNIGHT_SKILL_2				2	// ������ο�		
#define		KNIGHT_SKILL_3				3	// ���潺�����͸�	
#define		KNIGHT_SKILL_4				4	// �Ŀ�������		
#define		KNIGHT_SKILL_5				5	// ü�ξ���		
#define		KNIGHT_SKILL_6				6	// ���̵�			
#define		KNIGHT_SKILL_7				7	// �������		
#define		KNIGHT_SKILL_8				8	// �Ŀ���			
#define		KNIGHT_SKILL_9				9	// ����			
#define		KNIGHT_SKILL_10				10	// ���̽��ַο�	
#define		KNIGHT_SKILL_11				11	// ���̾�ַο�	
#define		KNIGHT_SKILL_12				12	// �Ŀ���ο�		
#define		KNIGHT_SKILL_13				13	// �Ŀ���Ʈ����ũ	
#define		KNIGHT_SKILL_14				14	// ����Ŭ��		

// ����ų
#define		FAIRY_SKILL_100				100		// Ʈ��Ŭ				
#define		FAIRY_SKILL_102				102		// ������				
#define		FAIRY_SKILL_103				103		// ����Ʈ�ö��		
#define		FAIRY_SKILL_104				104		// ��Ʈ����	
#define		FAIRY_SKILL_105				105		// ����Ʈ				
#define		FAIRY_SKILL_106				106		// ����Ʈ		
#define		FAIRY_SKILL_107				107		// ����				
#define		FAIRY_SKILL_108				108		// ���̽���Ʈ		
#define		FAIRY_SKILL_109				109		// ���ͽ�Ʈ����ũ		
#define		FAIRY_SKILL_111				111		// �ͽ��÷���			
#define		FAIRY_SKILL_112				112		// �����÷���		
#define		FAIRY_SKILL_119				119		// ���̾		
#define		FAIRY_SKILL_126				126		// ���̽��Ƿ�			
*/
#define		HUMAN_SKILL_0				0		// ���̵�				X

#define		KNIGHT_SKILL_1				1		// ���潺 �����͸�		X
#define		KNIGHT_SKILL_2				2		// �Ŀ���ο�			X
#define		KNIGHT_SKILL_3				3		// �������
#define		KNIGHT_SKILL_4				4		// �Ŀ�������			X
#define		KNIGHT_SKILL_5				5		// Ȧ������

#define		ARCHER_SKILL_50				50		// �齺��				X
#define		ARCHER_SKILL_51				51		// �Ŀ���				X
#define		ARCHER_SKILL_52				52		// ���ڽ�				X
#define		ARCHER_SKILL_53				53		// ������̺�
#define		ARCHER_SKILL_54				54		// �ҿｺƮ��
#define		ARCHER_SKILL_55				54		// �Ŀ���Ʈ����ũ

#define		FAIRY_SKILL_100				100		// Ʈ��Ŭ	( ��ų�� ��� ���ϴ� �⺻ ��ų )	X
#define		FAIRY_SKILL_101				101		// ������				X

#define		ELLIADEN_SKILL_102			102		// ���̾				X
#define		ELLIADEN_SKILL_103			103		// ���̽��Ƿ�			X
#define		ELLIADEN_SKILL_104			104		// ����Ʈ
#define		ELLIADEN_SKILL_105			105		// ����
#define		ELLIADEN_SKILL_106			106		// ������

#define		ROIEL_SKILL_150				150		// ����Ʈ�׽���
#define		ROIEL_SKILL_151				151		// ����Ʈ�׺�Ʈ
#define		ROIEL_SKILL_152				152		// �����䷹�̼�
#define		ROIEL_SKILL_153				153		// ����
#define		ROIEL_SKILL_154				154		// Ʈ������
#define		ROIEL_SKILL_155				155		// ������
#define		ROIEL_SKILL_156				156		// ��Ʈ����

//-----------------------------------------------------------------------------
// Desc: ��Ʈ����
//-----------------------------------------------------------------------------
#define		HIGH_4BIT				240
#define		LOW_4BIT				15


//-----------------------------------------------------------------------------
// Desc: ���ڿ�����
//-----------------------------------------------------------------------------
#define		STR_LINE_64				64
#define		STR_LINE_128			128	
#define		STR_LINE_256			256
#define		BYTE_FAIL				255

#define		TAR_ITEM_INDEX			90				// Ÿ��������
#define		NECTA_ITEM_INDEX		91				// ��ŸŸ����
#define		CHAIR_ITEM_INDEX		92				// ���ھ�����
#define		TREE_ITEM_INDEX			97				// ����������
#define		TAR_NECTA_CANCEL		10				// Ÿ��, ��Ÿ��� �Ҷ� ���尪

//-----------------------------------------------------------------------------
// Desc: ��Ÿ...
//-----------------------------------------------------------------------------
#define		EPSILON					0.0002f			// �Ҽ��� ��
#define		ANGLE					0.01745f		// ȸ����. ����
#define		ANGLE_SPEED				4				// ȸ���ӵ�
#define		CLIENT_TILESIZE			8				// Ÿ�� ũ��
#define		LOD_RANGE				100				// LOD�Ÿ�(�⺻���� ���ؼ� ����)


//-----------------------------------------------------------------------------
// Desc: �����۹迭
//-----------------------------------------------------------------------------
#define		HEAD_INDEX				5				// �ִϸ��̼� ��忡�� �Ӹ� �ε���
#define		HAIR_INDEX				4				// �Ӹ�ī��
#define		FACE_INDEX				3				// ��


//-----------------------------------------------------------------------------
// Desc: �����۸�ũ ����
//-----------------------------------------------------------------------------
#define		WEAPON_READYLINK		49				// Į��ũ�Ǵº� 
#define		SHIELD_READYLINK		51				// ���и�ũ�Ǵº�
#define		LINK_ITEM_COUNT			4				// ��ũ�Ǵ� ����


//-----------------------------------------------------------------------------
// Desc: �ʺ�
//-----------------------------------------------------------------------------
#define		CLEAR_BLOCK				0				// CLEAR
#define		OBJ_BLOCK				1				// OBJ
#define		NPC_BLOCK				12				// NPC
#define		PC_BLOCK				13				// PC
#define		ITEM_BLOCK				14				// ITEM


//-----------------------------------------------------------------------------
// Desc: Į�˱�
//-----------------------------------------------------------------------------
#define		MAX_TRACE_COUNT			256				// ���� �ִ� Ŀ�������� : TOTAL_POINTS * TRACE_STEP
#define		TOTAL_POINTS			64				// �ִϸ��̼� �Ǵ��� : 30
#define		TRACE_STEP				4				// �ִϸ��̼� ������ �鰹��


//-----------------------------------------------------------------------------
// Desc: ������ �ε���
//-----------------------------------------------------------------------------
#define		ITEM_CAEM_TYPE			1				// ī�� ������ Ÿ��
#define		ITEM_POTION_TYPE		13				// ���� ������ Ÿ��
#define		ITEM_HORSE_PAPER		268476195		// ������

//-----------------------------------------------------------------------------
// Desc: ĳ���ͱ��� ������, ���ؽ�����
//-----------------------------------------------------------------------------
#define		MAX_PC_ITM				6				// pc�����۰���
#define		MAX_NPC_ITM				2				// npc�����۰���
#define		MAX_PC_VTX				1280			// pc���ؽ�����ũ��
#define		MAX_NPC_VTX				2560			// npc���ؽ�����ũ��


//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼� 
//-----------------------------------------------------------------------------
#define		NPC_ANI_INDEX_COUNT		30				// NPC �ִϸ��̼� ���� 
#define		PC_ANI_INDEX_COUNT		150				// PC  �ִϸ��̼� ���� 
#define		PCATTACK_SPEED			74.0f			// ���� ����
#define		ANI_TIME				84.0f			// �⺻ �ִϸ��̼�(0.084��)


//-----------------------------------------------------------------------------
// Desc: ������Ʈ �ε���
//-----------------------------------------------------------------------------
#define		CASTTLE_DOOR			202				// ����
#define		MOUSEPOINT_INDEX		500				// ���콺������


//-----------------------------------------------------------------------------
// Desc: PC �ε���
//-----------------------------------------------------------------------------
#define		UNION_MAN				997				// ���Ͽ� ����
#define		UNION_WOMAN				998				// ���Ͽ� ����
#define		UNION_FAIRY				999				// ���Ͽ� ����


//-----------------------------------------------------------------------------
// Desc: NPC �ε���
//-----------------------------------------------------------------------------
#define		NPC_RACE				-991
#define		LIVEITEM_RACE			-992
#define		LIVEITEM_INDEX			900				// ����Ʈ������
//#define		DESTROY_OBJECT_INDEX	911				// �ı�������Ʈ(�볪��)

//-----------------------------------------------------------------------------
// Desc: �� �ε���
//-----------------------------------------------------------------------------
#define		RED_HORSE				995				// ����
#define		WHITE_HORSE				919				// �鸶
#define		BLACK_HORSE				920				// �渶
#define		STID_INDEX				996				// ��ƼƮ(npc����)
#define		HORSE_RACE				-993				// ������(npc����)


//-----------------------------------------------------------------------------
// Desc: �ð�
//-----------------------------------------------------------------------------
#define		NPC_UNDERATTACK_TIME	 500.0f			// �¾��ִ� �ð� (0.5��)
#define		NPC_DIE_TIME			5000.0f			// �׾��ִ� �ð� (5��)
#define		NPC_DIE_BLENDTIME		1500.0f			// ����   �ð� (1.5��)
#define		NPC_RESPAWN_BLENDTIME	 500.0f			// ���������ð�(0.5��)
#define		FAIRYUNITE_TIME			3000.0f			// �� ��ü�ð�(3��)
#define		HEAD_ROTATE_TIME		 200.0f			// �Ӹ�ȸ���ϴ½ð�(0.2��)


//-----------------------------------------------------------------------------
// Desc: �Ÿ� ����
//-----------------------------------------------------------------------------
#define		XBLOCK_RANGE			32				// NPC ó���ϴ¹���
#define		MAX_ITEM_RANGE			1				// �����ۼ��� Ÿ�ϰŸ�
#define		MAX_RUN_RANGE			2				// �ȱ�, �ٱ� ���ðŸ�


//-----------------------------------------------------------------------------
// Desc: ����ó��
//-----------------------------------------------------------------------------
#define		ATTACKTYPE_A			0			// ����Ÿ��1
#define		ATTACKTYPE_B			1			// ����Ÿ��2
#define		ATTACKTYPE_C			2			// ����Ÿ��3

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