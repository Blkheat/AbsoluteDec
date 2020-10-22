
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include	"CharacterAI.h"
#include	"ObjBase.H"
#include	"Define.H"
#include	<map\\d3drenderobj.h>
#include	<queue>
#include	<list>
using namespace	std;
#include	"findway.h"
#include    "..\\Effect\\CTail.h"
#include    "..\\Effect\\CSpellEffect.h"
#include    "CFaceExp.h"

const UINT c_MoveTime( 30 );
const UINT c_Pc_MoveSpeed( 180 );

const UINT c_Max_WearItemType( 7 );

const UINT c_Max_RideItemType( 3 );

const UINT c_MotionBlendFrame( 8 );
const UINT c_MotionBlendHalfFrame( 4 );

const UINT c_WeaponDelayTime( 2000 );				// �⺻ �����̰� 2000 

const int c_Max_Illusion_Num( 2 );



typedef enum n_QuestMarkType						// ����Ʈ �����ÿ� ����Ʈ ��ũ Ÿ��
{
	NONE_QUEST_MARK	   = -1 ,						// ����Ʈ ��ũ ����
	START_QUEST_MARK   =  0 ,						// ����Ʈ ���� ��ũ ( ����ǥ )
	REPORT_QUEST_MARK  =  1							// ����Ʈ ���� ��ũ ( ȭ��ǥ )

}QUEST_MARK_TYPE;


typedef enum NWearItemType
{
	n_Wear_Face= 0,
	n_Wear_CoatArmor,
	n_Wear_PantsArmor,
	n_Wear_Gloves,
	n_Wear_Shoes,
	n_Wear_Hair, 
	n_Wear_Serve

} NWearItemType;


/* �������� �����ִ� 10���� ��� Ÿ�� ���� ������ ���� 
typedef enum NWearItemSlotType 
{	
	n_WearSlot_CoatArmor = 0,
	n_WearSlot_PantsArmor,
	n_WearSlot_Weapon,
	n_WearSlot_Shield,
	n_WearSlot_Hair, 
	n_WearSlot_Serve,
	n_WearSlot_Gloves,
	n_WearSlot_Shoes,
	n_WearSlot_Accessory1,
	n_WearSlot_Accessory2

} NWearItemSlotType; */


typedef enum NCharaterType
{
	n_Millena_Man = 0,
	n_Millena_Woman,
	n_Rain_Man,
	n_Rain_Woman

} NCharaterType;


///-- By simwoosung 
///-- BaseDataStructure.h���� ���ǵǾ����� �ʱ⿡ �̰����� ��������
///-- ����
typedef enum NCharaterGender
{
	n_NoGender = 0,
	n_Male,
	n_FeMale,
	n_Boss,
	n_ColANPC,			///-- �ݷδ� ������ NPC
	n_ColDNPC,			///-- �ݷδ� ������ NPC
	n_4FootUnit,		///-- 4�� ���� ����
	n_Millena_Unit,		///-- �з��� ���� ����
	n_Rain_Unit,		///-- ���� ���� ����
	n_Neutral_Unit,		///-- �߸� ���� ����

} NCharaterGender;


//-----------------------------------------------------------------------------
// Desc: ĳ���� �̵� Ŭ����
//-----------------------------------------------------------------------------
struct TILE_INFO
{
	DWORD	x;
	DWORD	y;
	float	height;
};


struct MOVE_INFO
{
	D3DXVECTOR3	vStPos;			// ������ġ
	D3DXVECTOR3	vCVtPos;		// �浹���� ��ġ
	TILE_INFO	strt;			// ����Ÿ��
	TILE_INFO	curt;			// ����Ÿ��
	TILE_INFO	next;			// ����Ÿ��
	TILE_INFO	dest;			// ����Ÿ��
	TILE_INFO	path;			// ��ã��Ÿ��

	float		step_x;			// x
	float		step_y;			// y
	float		step_height;	// ����

	float		now_Yrad;		// ���� Y����	
	float		nex_Yrad;		// ���� Y����	
	float		add_Yrad;		// ���� Y����
	float		now_Xrad;		// ���� X����	
	float		nex_Xrad;		// ���� X����	
	float		add_Xrad;		// ���� X����

	bool		bYRotate;		// Yȸ��	
	bool		bXRotate;		// Xȸ��
	int			rad_count;		// ȸ��ī��Ʈ
	float		frad_remain;	// ���� �����ð�		
	
	MOVE_INFO()		{ Clear(); }
	~MOVE_INFO()	{}

	void	Clear();
	void	Init( int x, int y );
	bool	ValueUpdate();
	bool	FindValue( int x, int y, bool bNext = true );
};



//-----------------------------------------------------------------------------
// Desc: ť Ŭ����
//-----------------------------------------------------------------------------
struct EVENT_DATA_INFO
{
	DWORD			type;
	int				race;	
	DWORD			tbl_Idx;			// �� NPC ���̺� ��� 
	DWORD			index;
	long			unique;	
	long			time;
	DWORD			x;
	DWORD			y;
	DWORD			motion;	
	bool			loop;				// false�̸� �ױ�
	bool			kill;
	D3DXVECTOR3		vPos;	
	DWORD			damage;				// Ÿ�� ������ 
	BYTE			critical;			// ũ��Ƽ�ÿ� ���� ��� �� Ư��ȿ�� �߻� (Ŭ���̾�Ʈ���� �������� �����ϱ� �޺� Ÿ��(c_ComboZKey1)���� ��� 
	DWORD			cell;				// 0�� �ƴϸ� �ڷ� ���� �������� ��� , �� �ൿ���� �뵵 ����� 
	DWORD			count;				// �޺� ���� Ƚ���� ���� Ư��ȿ�� �߻� : UseSkill������ ��ų �ڵ�
	DWORD			code;				// �޺� ������ Ư��ȿ�� �߻�, ��ų �ڵ尪 
	BYTE			status;				// �޺� �ϼ� ���� 
	DWORD			skill_unique;		// ���� ���� ��ų�� ��ų ���̵�
	BYTE			number;				// ��ų ���ݽ� ���� ���� ��� �� NPC �� ����  , ������̺� �� NPC Ani ��ȣ 
	BYTE			weaponView[2];		// ���� ������ �����ֱ� ����
	BYTE			IsHideWing;			// ���������ֱ� ����
	BOOL			comcomple;			// �޺� �ϼ�����
	BOOL			IsActProc;			// �̹� ó���Ǿ����� ����
	long			lStartTime;			// �̺�Ʈ ���۽ð�

	///-- By simwoosung
	int				nADisErrCnt;		// ���ݽ� �Ÿ�üũ �κ� ���� ���� 	

	///--JAPAN_BUG_MODIFY
	DWORD			dKeepSkillCode;		// ���ӽ�ų�ڵ�
	
	void*			target_list;		// ���� ���ݽÿ� �������� ���� �Ͽ����� �ΰ�ȿ�� ó��	
	
	EVENT_DATA_INFO()	{ Init(); }
	~EVENT_DATA_INFO()	{ Clear(); }
	void	Init();
	void	Clear();

	void operator=(const EVENT_DATA_INFO & event);
	
};

//-----------------------------------------------------------------------------
// Desc: ���ؽ�����
//-----------------------------------------------------------------------------
struct	VTX_BUFFER_INFO
{
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		NormalVB;	// ���ؽ�����
#else
	LPDIRECT3DVERTEXBUFFER8		NormalVB;	// ���ؽ�����
#endif // DIRECT_VERSION_9_MJH	
	D3DXVECTOR3*				pVertex;	// ���ؽ���
	bool						bInit;		// �����ʱ�ȭ
	
	int							nSize;

	VTX_BUFFER_INFO()	{ Clear(); }
	~VTX_BUFFER_INFO()	{ Destroy(); }

	void	Clear();
	void	Destroy();	
	void	CreateBuffer( int _nVtxCount );
};

typedef enum NCharState
{
	nCharState_Normal = 0 ,				// ĳ���� �⺻
	nCharState_Master,					// �н��� ���� ����
	nCharState_Illusion,				// �н��� 
	nCharState_Detect_Illusion			// ���̴� �н��� 
	
} NCharState;

typedef enum NHolyState
{
	nHolyState_Normal = 0  ,			// NPC �⺻
	nHolyState_Master      ,			// �н��� ���� ����
	nHolyState_Holy  	   ,			// �ż�
} NHolyState;

typedef enum NRideState
{
	nRideState_Normal = 0	,			// ���̵� �⺻
	nRideState_Master		,			// ���̵� ����
 	nRideState_Ride			,			// ���̵� ���		
} NRideState;

typedef enum NPetState
{
	nPetState_Normal = 0	,			// �� �⺻	
	nPetState_Master		,			// �� ����	
	nPetState_Pet			,			// �� ���	
} NPetState;


///-- By simwoosung
///-- ĳ���� Ÿ��
typedef enum NCharacterType
{
	nCharacterTypeNormal = 0	,		/// �⺻ Ÿ��			
	nCharacterTypePlayerNPC		,		/// �÷��̾� ���ҽ��� ����ϴ� NPC
	nCharacterTypeBigMoster,			/// �Ŵ� ��
	nCharacterTypeBulding,				/// �Ŵ� �ǹ� ����		
} NCharacterType;

enum NWeaponLightType
{
	nWeaponLightNone = 0,
	nWeaponLightModulate2X,
	nWeaponLightModulate4X,
};

//-- By SDH
enum NActiveState
{
	nActiveTypeNormal =0 , 
	nActiveTypeMove  , 
	nActiveTypeSKill ,
};

extern int GetMax( int x, int xx );

//-----------------------------------------------------------------------------
// Desc: ���� �ڱ� ����
//-----------------------------------------------------------------------------
struct STATE_INFO
{	
	bool		bWalk;									// �Ȱ� �ִ��� �ٰ� �ִ���...������ ��� �ٱ�...�ȱ� ����...

	NRideState	nRideState;								// ���̵� ���°�
	bool		bRide;									// Ÿ�� �ִ��� �ƴ��� 
	int			TransIndex;								// �����Ͷ�� ��� ������ Index ��ۼ����̶�� Master Index

	NCharState	nCharState;								// ĳ���� ���°� 
	
	int			IllusionIndex[c_Max_Illusion_Num];		// �����Ͷ�� ȯ���� Index, ȯ���̶�� Mastrer Index
	
	NHolyState  nHolyState;								// �ż� ���� ���°�
	int			HolyIndex;								// �����Ͷ�� �ż� Index, �ż���� Mastrer Index

	WORD		BaseCode;								// �ż���� �⺻ NPC �ڵ� ��

	NPetState	nPetState;								// �� ���� ���°�
	int			PetIndex;								// �����Ͷ�� ���� Index ���̸� �������� Index
	
	NCharacterType nCharacterType;						// ĳ���� Ÿ�� 
	
	UINT		nWeaponLightType;						// ���� �߱� ������ Ÿ��
	
	NActiveState nActiveState; 


	STATE_INFO()		{ Clear(); }
	~STATE_INFO()		{ Clear(); }

	void	Clear()		
	{ 
		nActiveState = nActiveTypeNormal;
		nRideState = nRideState_Normal;
		bWalk = false; 
		bRide = false; 
		TransIndex = -1; 
		nCharState = nCharState_Normal; 
		
		for( int i = 0; i < c_Max_Illusion_Num; ++i )
		{
			IllusionIndex[i] = -1; 
		}

		nHolyState = nHolyState_Normal;
		HolyIndex  = -1;
		BaseCode   = 0;

		nPetState = nPetState_Normal;
		PetIndex  = -1;

		nCharacterType = nCharacterTypeNormal;
		nWeaponLightType = nWeaponLightNone;
	}

	void	Destroy()	{ Clear(); }	
};


//-----------------------------------------------------------------------------
// Desc: ���� ���� ����
//-----------------------------------------------------------------------------
struct USER_INFO
{
	ARMOR_INFO		ArmorInfo;			// ��������

	int				HeadCurFrame;		// �Ӹ�ȸ�� ����������
	int				HeadPreFrame;		// �Ӹ�ȸ�� ��������	
	long			HeadTime;			// �Ӹ�ȸ�� Ÿ��
	
	bool			bPkMode;			// PK���
	bool			bDown;				// �ɾҴ�?
	bool			bCharge;			// �������� 
	bool			bExtacy;			// �����ͽû���
	bool			bAttack;			// ���ݴ�����
	bool			bDefense;			// false:����	true:���潺	
	BYTE			EtcItem;			// 0:���� 1:��Ÿ 2:Ÿ��	3:����

	float			fEmitterScale;		// �߱�������

	//.........................................................................
	// �˱� ����
	//.........................................................................
	D3DXVECTOR3			vSwordTop; 						// Į max��
	D3DXVECTOR3			vSwordBottom;					// Į min��

	D3DXVECTOR3			vSwordAxis[2];					// �̵��� Į ��  ( 0 : Į �� , 1 : Į �ص� )

	USER_INFO()		{ Clear(); }
	~USER_INFO()	{ Clear(); }
	void Clear();
};

//-----------------------------------------------------------------------------
// Desc: �� ������ ����ü 
//-----------------------------------------------------------------------------

#define					FAIRYMOVRADIUS			4.0f		

struct	FairyMove
{
	BYTE				Type;					// ���� Ÿ��		0 ������ 1 �����϶� 2 �ɽ� 

	long				UniteUniq;				// �� ��ü���� ����ũ 
	bool				RightShoulderOn;		// ������ ����� ������ true

	D3DXVECTOR3			CenterPos;				// ĳ���� ��ǥ + �ٿ�� �ڽ� �ƽ� Y�� 
	bool				ClockRotate;			// ȸ�� ���� true�� �ð���� 
	float				Speed;					// Vec�� ���ǵ�
	float				Dir;					

	D3DXVECTOR3			AxisPos;				// ĳ���� �ٷ� �Ӹ� �� 

	D3DXVECTOR3			VitualPos;				// ������ �� �ȿ� ��� �ִ� ��ǥ 
	D3DXVECTOR3			VitualPosToCen;			// ������ ������ ���� �߾� ������ ���� 
	float				VitualPosRange;			// �̰��� �׻� ���� 	FAIRYMOVRADIUS �̴�.

	D3DXVECTOR3			Pos;					// �ӵ��� ������ ���ο� ��ǥ 
	D3DXVECTOR3			PosToCen;				// �� ��ǥ�� �߾ӱ����� ���� 
	float				PosRange;				// �� �Ÿ� 

	float				Life;					// ������ ���� �ð� 
	float				Height;					// ���� 

	D3DXVECTOR3			RealPos;				// RealPos = RealDis* (RealRange / Range) + CenterPos;
	float				RealRange;				// RealPos�� �Ÿ�

	D3DXVECTOR3			UnitVec;				// UnitVec.x	=	((2*ClockRotate)-1) * RealDis.z * sqrt( RealDis.x * RealDis.x + RealDis.z * RealDis.z )
												// UnitVec.z	=	-1 * ((2*ClockRotate)-1) * RealDis.x * sqrt( RealDis.x * RealDis.x + RealDis.z * RealDis.z )
	D3DXVECTOR3			Vec;					// Vec = UnitVec * Speed;


	D3DXMATRIX			Loc;					// ���� ������ ��Ʈ���� 
};

class CEffectBase;								// ����Ʈ ���̽� Ŭ���� by wxywxy

// ĳ���� ������ ���°�
typedef enum CHAR_ALPHA_STATE
{
	CHAR_NORMAL			= 0 ,					// ĳ���� �⺻
	CHAR_HIDE			= 1 ,					// ����� ( ���İ� ���� )
	CHAR_NONE_DISPLAY	= 2						// ǥ�� ���ϱ�
};

typedef struct s_CHAR_ALPHA
{
	CHAR_ALPHA_STATE	State;
	float				fAlpha;
	BOOL				bShow;	// ���� ��� ������ ��������� .. 
	

	void Clear(void)
	{
		State  = CHAR_NORMAL;
		fAlpha = 1.0f;
		bShow = 0 ; 
	}

	s_CHAR_ALPHA()
	{
		Clear();
	}

	CHAR_ALPHA_STATE GetState()
	{
		return State;
	}
		
	BOOL GetShow()
	{
		return bShow;
	}
	
	void SetShow(BOOL bshow) //���� ����߰� ��࿡ ���� �������� ��Ʈ �ȴ�. 
	{
		bShow = bshow;
	}
	
	void Set(CHAR_ALPHA_STATE state,float alpha)
	{
		State  = state;
		fAlpha = alpha;
	}

}CHAR_ALPHA;


//BY simwoosung - ĳ���� ���� �ǻ� ����
enum CHAR_WEAR_TYPE
{
	CHAR_WEAR_NORMAL = 0,
	CHAR_WEAR_SETITEM,
};

typedef struct s_QuestData
{
	int					Code;
	QUEST_MARK_TYPE		Type;

}QUESTDATA,*LPQUESTDATA;

//...............................................................................................................
// ŵ�� ��ų ����ȭ ���� üũ ������ By wxywxy
//...............................................................................................................
typedef struct s_KeepUpSkillChecker
{
public:

	WORD	theKeepupSkill[RP_MAX_KEEPUP_SKILL];

	s_KeepUpSkillChecker()
	{
		Clear();
	}

	~s_KeepUpSkillChecker()
	{
		Clear();
	}

	int SetCode(WORD Code);
	int DeleteCode(WORD Code);

	void Clear(void)
	{
		memset( theKeepupSkill , 0 , sizeof( WORD ) * RP_MAX_KEEPUP_SKILL );
	}

	int GetEmpty(void)
	{
		for( int i = 0 ; i < RP_MAX_KEEPUP_SKILL ; ++i )
		{
			if( theKeepupSkill[ i ] == 0 )
				return i;
		}

		return -1;
	}

	int FindCode(WORD Code)
	{
		for( int i = 0 ; i < RP_MAX_KEEPUP_SKILL ; ++i )
		{
			if( theKeepupSkill[ i ] == Code )
				return i;
		}
		
		return -1;
	}

}KEEPUP_SKILL_CHECKER,*LPKEPPUP_SKILL_CHECKER;


const float c_Stand_AniTime( 30.0f );
const float c_ReAction2_AniTime( 15.0f );

#define		HPREFRESH_TIME			2500

struct SNOWATTACKINFO
{
   DWORD  dAttackType;
   DWORD  dSkillCode;

   SNOWATTACKINFO()
   {
	  dAttackType = 0;
	  dSkillCode = 0;
   }
};

//-----------------------------------------------------------------------------
// Desc: ĳ���� Ŭ���� (���� Ŭ�������� ��Ÿ �ൿ�� ���� ó���� ���� �и����ѳ��´�)
//-----------------------------------------------------------------------------
class Character
{
public:
	bool			m_bPC;						// ����?
	int				m_nTableIndex;				// ���̺��ε���	
	BYTE			m_byAngle;					// ���� �ٶ󺸴� ��
	float			m_radian;				
	USER_INFO*		m_pUserInfo;				// �������뵥��Ÿ
	bool			m_bSoundPlay1;				// �����÷���1
	bool			m_bSoundPlay2;				// �����÷���2
	bool			m_ISAniProcess;				// ���ϸ��̼� ��� ����	
	
	long			m_lPreTime;					// HP ���� ������ ���Žð�
	long			m_lModifyHpTime;			// HP ����ȭ �������Žð�
	long			m_lLevelUpTime;				// ������ ������ ���Žð�
	
	BOOL			m_IsInitUpdate;				// �ʱ� ������Ʈ ����
	BOOL			m_IsNextApplyVA;			// ���� �ִϷ����� ��Ű�� ���뿩��
	BOOL			m_IsApplyVA;				// ��Ű�� ���뿩��
	long			m_nPrevVATime;				// ��Ű�� ����Ÿ��
	long			m_nDelVATime;				// ��Ű�� ����Ÿ��
	BOOL			m_IsRenderImg;				// �̹����� �׸����� ����
	int				m_nCamDistance;

	void			SetCamDistance(int nDis);
	
protected:
	BOOL			m_IsRealDie;				// ���� �׾��ִ��� ����
	BYTE			m_aAttackType;				// ������ ���� ����� Ÿ��
	DWORD			m_aAttackID;				// ������ ���� ����� ���̵�
	BOOL			m_IsApplyConAniSkill;		// ���� �ִϸ��̼� ��ų���� ����
	int				m_aConAniTime;				// ���� �ִϸ��̼� ����Ÿ��
	
	BOOL			m_IsSkillNoAni;				// ��ų�� �ߵ��Ǵµ� �ִϸ��̼��� �������

public:
	BOOL			IsRealDie()		{ return m_IsRealDie; }
	void			SetRealDie( BOOL bFlag, BYTE aAttackType = 0, DWORD aAttackID = 0 );
	BOOL			IsApplyConAniSkill()	{ return m_IsApplyConAniSkill; }
	void			SetApplyConAniSkill(BOOL bFlag);
	int				GetConAniTime()	{ return m_aConAniTime; }
	void			SetConAniTime(int nTime); 

	void			AddMatPosMatPos(D3DXMATRIX &matOut, D3DXMATRIX &mat1, D3DXMATRIX &mat2);

//...........................................................................................................
// �� ǥ��
//...........................................................................................................
public:
	DWORD					 m_FaceExpTimer;
	CFaceExpData			*m_pCurrentFaceExpData;	
	CFaceExpData::FACE_EXP	 m_NowFaceType;

	void ChangeFaceExp(CFaceExpData::FACE_EXP eType);
	void FaceExpProcess(void);

//...........................................................................................................
// �ΰ� ȿ�� By wxywxy
//...........................................................................................................
public:
	
	D3DXCOLOR		m_EffectColor;				// ����Ʈ �ɷ����� ����
	BOOL			m_ISEffectColor;			// ����Ʈ Į�� ����

//...........................................................................................................
// ������ �������
//...........................................................................................................
public:
	CHAR_WEAR_TYPE	m_Char_WearType;			//������ �������	

public:
	//by simwoosung
	D3DXVECTOR3	    m_MaxBoundVec;              //�ִ� �ٿ�� ��ġ         
	D3DXVECTOR3	    m_MinBoundVec; 	            //�ּ� �ٿ�� ��ġ

    BOOL			m_BIsBoundSet;				//�ٿ�� ������ ��������
	float			m_fMaxHeight;				//�ְ� �ٿ�� ����
	float			m_fMinHeight;				//���� �ٿ�� ����
	
	int				m_nAddNum;					//ä��â�� �� �÷��ֱ� ���� ���� �߰���
	
	float			m_fAddRideHeight;			//���̵��� ĳ���� ���̵� �� �÷��ֱ� ���� ����
	float			m_fAddHeight;				//���̵��� ĳ���͸� �� �÷��ֱ� ���� ����
	float			m_fAddForward;				//���̵��� ĳ������ġ ������ ����
//-----------------------------------------------------------------------------
// Desc: ����Ʈ ���� By wxywxy
//-----------------------------------------------------------------------------
public:
	
	std::list	<CEffectBase *>		m_EffectList;

public:
	
	int EffectCheck(void);							// ����Ʈ �����Ӻ��� üũ
	int EffectSortRender(void);						// ��Ʈ�� ĳ���� ������ ����Ʈ ���

	int PopEffectBase(CEffectBase **ppEffectBase);	// ����Ʈ ���� 
	int PushEffectBase(CEffectBase **ppEffectBase); // ����Ʈ �߰� 

//-----------------------------------------------------------------------------
// Desc: ����, �̺�Ʈ
//-----------------------------------------------------------------------------
public:
	DWORD			theInputedDestCell;	/// sooree network
	inline void		SetInputedDestCell( DWORD aDestCell )	{ theInputedDestCell = aDestCell;	};
	inline DWORD	GetInputedDestCell()					{ return( theInputedDestCell );		};


	STATE_INFO			m_State;					// �ڱ����
	MOVE_INFO			m_Mov;						// �̵� ����	

	EVENT_DATA_INFO		m_curt_event;				// ���� �̺�Ʈ
	EVENT_DATA_INFO		m_save_event;				// ���� �̺�Ʈ
	EVENT_DATA_INFO		m_next_event;				// ���� �̺�Ʈ 
	EVENT_DATA_INFO		m_temp_event;				// �ӽ� �̺�Ʈ

	long				m_Attack_DelayTime;			// ���� ������ �ð�				
	long				m_Weapon_DelayTime;			// ���⺰ ������ �ð� 
//-----------------------------------------------------------------------------
// Desc: �׸���
//-----------------------------------------------------------------------------
protected:	
	D3DXVECTOR3			m_vLight;					// ����Ʈ��ġ
	bool				m_bShadowDisplay;			// �׸��� ����������
	bool				m_bShadowMake;				// �׸��� ��������
	///-- by simwoosung
	UINT				m_uShadowSizeType;			// �׸��� ũ��Ÿ��
	int					m_nShadowSize;
	
public:
	inline void			SetShadowSizeType(UINT nType)	{ m_uShadowSizeType = nType; }

//-----------------------------------------------------------------------------
// Desc: �⺻����
//-----------------------------------------------------------------------------
public:	
	SHADOW_INFO			m_Shadow;							// �׸���	
	int					m_pItm_index[c_Max_WearItemType];	// itm �ε���(���, ��, ����, ����, �尩, ����, �������...�� 7��)
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_pMaterial[c_Max_WearItemType];	// ���͸���
#else
	D3DMATERIAL8		m_pMaterial[c_Max_WearItemType];	// ���͸���
#endif // DIRECT_VERSION_9_MJH	
	VTX_BUFFER_INFO		m_pVtxBuffer[c_Max_WearItemType];	// ���ؽ�����		
	D3DXVECTOR3			m_vScale;							// ������

	// �⺻ �ǻ� 
	int					m_Basic_DrawItm[c_Max_WearItemType];	// �����۰� �Բ� ���ÿ� �׷����� ���� (���, �� ��)
	int					m_Basic_Itm_index[c_Max_WearItemType];	// itm �ε���(���, ��, ����, ����, �尩, ����, �������...�� 7��)
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_Basic_Material[c_Max_WearItemType];	// ���͸���	
#else
	D3DMATERIAL8		m_Basic_Material[c_Max_WearItemType];	// ���͸���	
#endif // DIRECT_VERSION_9_MJH
	VTX_BUFFER_INFO		m_Basic_VtxBuffer[c_Max_WearItemType];	// ���ؽ�����

	// ���̵� ������
	int					m_Ride_Item_index[c_Max_RideItemType];	// ���̵� ������ �ε���
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_Ride_Material[c_Max_RideItemType];	// ���͸���
#else
	D3DMATERIAL8		m_Ride_Material[c_Max_RideItemType];	// ���͸���
#endif // DIRECT_VERSION_9_MJH	
	VTX_BUFFER_INFO		m_Ride_VtxBuffer[c_Max_RideItemType];	// ���ؽ�����
	
	
	CHAR_ALPHA			m_CharAlpha;							// ĳ���� ���İ� ����

//-----------------------------------------------------------------------------
// Desc: ���ؽ�, �ִϸ��̼�����
//-----------------------------------------------------------------------------
public:		
	int					m_ani_number;				// ���� �ִϸ��̼� ��ȣ
	int					m_pre_ani_number;			// ���� �ִϸ��̼� ��ȣ
	int					m_ani_index;				// ���� �ִϸ��̼� �ε���	
	int					m_ani_frame;				// �ִ� ������

	int					m_next_frame;
	int					m_curt_frame;
	
	///-- By simwoosung
	int					m_start_blend_frame;		// ���� ���� ������

	unsigned int		m_end_frame;				// ���� ���� ������			
	unsigned int		m_pre_end_frame;			// ���� ��ǥ ������			

	int					m_blend_frame;				// ��� ���� ������ 

	int					m_move_frame;				// �̵� ������ 
	float				m_move_middle_time;			// �̵� ���� ������
	
	float				m_move_percent;				// �̵��Ÿ� �ۼ�Ʈ 
	float				m_ani_percent;				// �ִϸ��̼� �ۼ�Ʈ 
	
	float				m_step_time;				// ����Ÿ��
	float				m_ani_time;					// �ִ� Ÿ��
	float				m_realani_time;				// ���� �ִ�Ÿ��
	float				m_middle_time;				// 1������ ~ 2������ ������ �߰��� ���� ( 0.0 ~ 1.0 ���� )	
	long				m_PreTime;					// ���� �������� �ð�
	long				m_PreMTime;					// �̵����� ������ �ð�
	long				m_SimTime;					// �ɽ��� �ð�	

public:
	CFindWay*			theFindWay;					// ��ã�� �˰��� 
	DWORD				thePath[6];					// NPC �н� �̵� 
	int					theCurtPath;				// ���� ���° �н�����...
	int					theDestPath;				// ������ �н� ��ȣ 
	D3DXVECTOR3			m_vDir;						// ĳ������ ���� ����
	
	BOOL				m_ISApproachWeapon;			// ���� ���� ��/��
	UINT				m_AttackMotion;				// ������ ���ݸ�� ��ȣ
	
	D3DXMATRIX			m_matRideChr;				// ���̵��� ĳ������ġ

//-----------------------------------------------------------------------------
// Desc: ������� ��Ʈ����
//-----------------------------------------------------------------------------
protected:						
	D3DXMATRIX			m_matPreChr;				// ĳ�������� ��ġ
	D3DXMATRIX			m_matChr;					// ĳ������ġ
	D3DXMATRIX			m_matRHand;					// ������
	D3DXMATRIX			m_matLHand;					// �޼�	
	D3DXMATRIX			m_matRHand2;				// ������2(Ÿ��,��Ÿ� ���)

	D3DXMATRIX			m_matORHand;					// ������
	D3DXMATRIX			m_matOLHand;					// �޼�	
	D3DXMATRIX			m_matORHand2;				// ������2(Ÿ��,��Ÿ� ���)

public:
	D3DXVECTOR3			m_maxBoundVec;				// �ٿ�� �ڽ�_Max
	D3DXVECTOR3			m_minBoundVec;				// �ٿ�� �ڽ�_Min

	D3DXMATRIX			m_matRealRHand;				// ���� ������
	D3DXMATRIX			m_matRealLHand;				// ���� �޼�
	D3DXMATRIX			m_matRealHair;				// ���� �Ӹ� ��
	D3DXMATRIX			m_matRealbreast;			// ���� ���� ��
	D3DXMATRIX			m_matRealCenter;			// ���� ���� ��
	D3DXMATRIX			m_matRealLFoot;				// ���� �޹� ��
	D3DXMATRIX			m_matRealRFoot;				// ���� ������ ��

	D3DXMATRIX			m_matRealLShoulder;			// ���� �� ��� 
	D3DXMATRIX			m_matRealRShoulder;			// ���� ���� ��� 

	D3DXMATRIX			m_matRealLElbow;			// ���� �� ��ġ
	D3DXMATRIX			m_matRealRElbow;			// ���� ���� ��ġ


	D3DXMATRIX			m_matRealLKnee;				//	���� ���� ���� 
	D3DXMATRIX			m_matRealRKnee;				// ���� ���� ���� 
	D3DXMATRIX			m_matRealLPelvis;			// ���� �� ��� 
	D3DXMATRIX			m_matRealRPelvis;			// ���� ���� ��� 


	D3DXMATRIX			m_matRealORHand;			// ���� ������
	D3DXMATRIX			m_matRealOLHand;			// ���� �޼�
	D3DXMATRIX			m_matRealOHair;				// ���� �Ӹ� ��
	D3DXMATRIX			m_matRealObreast;			// ���� ���� ��
	D3DXMATRIX			m_matRealOCenter;			// ���� ���� ��
	D3DXMATRIX			m_matRealOLFoot;			// ���� �޹� ��
	D3DXMATRIX			m_matRealORFoot;			// ���� ������ ��
	D3DXMATRIX			m_matRealOLShoulder;		// ���� �� ��� 
	D3DXMATRIX			m_matRealORShoulder;		// ���� ���� ��� 
	D3DXMATRIX			m_matRealOLElbow;			// ���� �� ��ġ
	D3DXMATRIX			m_matRealORElbow;			// ���� ���� ��ġ
	D3DXMATRIX			m_matRealOLKnee;			//	���� ���� ���� 
	D3DXMATRIX			m_matRealORKnee;			// ���� ���� ���� 
	D3DXMATRIX			m_matRealOLPelvis;			// ���� �� ��� 
	D3DXMATRIX			m_matRealORPelvis;			// ���� ���� ��� 



	D3DXMATRIX			m_matRideCenter;			// ���̵����� �� ���� 	

	D3DXMATRIX			m_matBoneCenter;			// 0�� ������ �������� ������ ��ǥ ( ����Ʈ���� �߽����� �������� )

	CSpellEffect		m_SpellEffect;				// ��â ����Ʈ

	int					GetWeaponType(void);

//-----------------------------------------------------------------------------
// Desc: �Լ�
//-----------------------------------------------------------------------------
public:
	void		SetItemBuffer( int index );					// ������ ���� ���Ҵ��ϱ�
	void		SetBasicItemBuffer( int _nIndex );
	void		SetRideItemBuffer( int _nIndex );

protected:	
	void		AlphaProcess();								// ���ĺ���
	void		SetHeightStep( bool _bInit = false );		// ���̰�����(��������)
	void		SetRotate( TILE_INFO	tPos );				// ȸ��������(ȸ������) 
	bool		SetMoveStep( TILE_INFO	tPos );				// ����������(��������)	
	void		GetMoveStep( int dest_x, int dest_y, int now_x, int now_y, float &x, float &y );	// ����������
			
	void		Move( float middle_time );										// �����̵�
	void		MoveRatio( float fRatio );										// ���� ���� �̵�
	void		MiddleMove( float middle_time );								// �����̵�
	///--JAPAN_BUG_MODIFY
	void		Rotate( float fRatio, int aAddFrame );		// �̵��� ĳ���� ���� �ִϸ��̼� 
	bool		AniSearch( unsigned int ani_number );		// �ִϸ��̼ǰ˻�

public:
	Character()		{ Init(); }

	~Character()	{ Free(); }		

	//-----------------------------------------------------------------------------
	// Desc : ���� �ʱ�ȭ
	//-----------------------------------------------------------------------------
	void		Init();
	void		Free();

	//-----------------------------------------------------------------------------
	// Desc : ������
	//-----------------------------------------------------------------------------	
	void		UpdateTexture();												// �ؽ��ľ�����Ʈ
	bool		AniRender(bool bFlag = false);									// ĳ���;ִϹ��ۼ���
	bool		BoneRender();
	bool		ViewModelPC(void);
	bool		ViewModelNPC(void);
	
	void		Display( BYTE _byType = 0, BYTE _bySort = 0, bool bFlag = false, BOOL IsRender = TRUE );					// ĳ����
	bool		DisplayLine( BYTE _byType = 0, BYTE _bySort = 0 );				// �ܰ���
	bool		DisplayMotionBlow();											// ��Ǻ�ξ�
	bool		DisplayAfterImage();											// Į�˱�
	void		DisplayEffect();												// ����Ʈ	
	void		DisplayBound( BYTE _byType = 0 );								// �ٿ��ڽ�
	//by simwoosung
	void		DisplayBoundBox();

	///-- By simwoosung -- �ܻ� ������
	bool		DisplayAfterImage(SAImageData & sIData);						// ĳ���� �ܻ�
	// ������ �ִϸ��̼�
	void		ItemAnimation(SAImageData & sIData, int type = -1, bool bChange = false, 
							  int next_frame = 0, int curt_frame = 0 );

	void		MeshAnimation( int mod );										// �޽� �ִϸ��̼�	
	D3DXMATRIX	HeadAnimation();												// �Ӹ� �ִϸ��̼�	
	
	void		RenderToAlpha(D3DXVECTOR3 vPos,float Alpha, 
							  BOOL IsRenderWeapon = TRUE, SAImageData * psIDat = NULL);
	
	void		RenderToAlphaEffect(D3DXVECTOR3 vPos,float Alpha, float Scale, D3DXCOLOR Color , 
									BOOL IsRenderWeapon = TRUE, SAImageData * psIDat = NULL);

	
	BOOL		PreTextureLoad(void);											// �̸� �ؽ�ó �б�

	int			GetBoneLinkTM(int Num, D3DXMATRIX &mat, BOOL IsModify = TRUE);							// ����Ʈ ��½� �� ��Ʈ���� ���ϱ� ( NPC �� �߰� )
	int			GetDirectBoneLinkTM(int Num,  D3DXMATRIX &mat );
	
	EVENT_DATA_INFO	 *GetEvent(void)	{return &m_curt_event;}

	// ������ �ִϸ��̼�
	void		ItemAnimation( int type = -1, bool bChange = false, int next_frame = 0, int curt_frame = 0 );
	bool		AniFrameProcess();												// �ִϸ��̼ǰŲٷ�?
	bool		UpdateFrame();													// �Ųٷξִϸ��̼�ó��
	int			GetNextFrame();													// ���������Ӱ� ����

	// ��� ������ ���� ���ʹϿ� ����
	void		blendQuaternion( D3DXQUATERNION* blend, float factor, D3DXQUATERNION src, D3DXQUATERNION dest );

	//-----------------------------------------------------------------------------
	// Desc : ĳ���� ��ġŸ�� �ֱ�, ����
	//-----------------------------------------------------------------------------	
	void		SetTilePos( int _nPosX, int _nPosY );					// ����Ÿ����ġ, ��Ʈ������
	void		SetPosTM( const D3DXMATRIX matWorld );					// �����Ʈ����
	bool		GetRealTM( float& _fPosX, float& _fPosY );				// Ÿ�ϼӼ��� ���� ��Ʈ����
	
	void		SetPosTM2(D3DXMATRIX mat) { m_matChr = mat; }
	D3DXMATRIX	GetPosTM();												// ��Ʈ���� ����
	D3DXMATRIX  GetBonCenterTM();
	D3DXMATRIX  GetPosEffTM();
	D3DXMATRIX  GetPosEyeTM();
	D3DXMATRIX	GetPosCenterBoneTM() { return m_matBoneCenter; }		// ��Ʈ���� ( 0 �� �� )

	D3DXMATRIX	GetRHandTM()	{ return m_matRHand; }					// ������ ����
	D3DXMATRIX	GetLHandTM()	{ return m_matLHand; }					// ������ ����
	
	D3DXMATRIX	GetBoneTM( DWORD _dwNode );								// node ��Ʈ���� ����
	D3DXMATRIX	GetBoneRealTM( DWORD _dwNode );							// Node ��Ʈ���� ���� By wxywxy

	void        CalLinkBone(int ani,int pre,int curt_frame,int next_frame,int MotionBlendHalfFrame,float factor,bool bMotionBlend);	// Desc: ��ũ ó�� �� ����ϱ�
	D3DXMATRIX  GetLinkBone(int ani,int pre,int curt_frame,int next_frame,int MotionBlendHalfFrame,float factor,bool bMotionBlend,int id, bool bRide = false);
	D3DXMATRIX  GetRideLinkBone(int ani,int pre,int curt_frame,int next_frame,int MotionBlendHalfFrame,float factor,bool bMotionBlend,int id);

	D3DXMATRIX	GetRealRHandTM()	{ return m_matRealRHand; }					// ������ ����
	D3DXMATRIX	GetRealLHandTM()	{ return m_matRealLHand; }					// ������ ����
	
	long        GetUnique(void);										// ����ũ ���
	int         GetIndex(void);											// �ε��� ���

	//-----------------------------------------------------------------------------
	// Desc : ���� -> Ŭ���̾�Ʈ �޼���	
	//-----------------------------------------------------------------------------		
/*	bool		SM__USE__SKILL( EVENT_DATA_INFO data );								// ��ų���
	bool		SM__ARROW__GOING( EVENT_DATA_INFO data );							// ȭ���� 
	void		SM__CHR__DIR( DWORD dir );										// �Ӹ�ȸ��
	int			SM__CHANGE__ITEM(  DWORD item_id, int item_unique = -1 );		// �������Ա�
	int			SM__TAKEOFF__ITEM( DWORD item_id, int item_unique = -1 );		// �����۹���
	void		SM__PICKUP__ITEM( DWORD item_id, DWORD unique, DWORD life );	// �������ݱ�
	void		SM__DROP__ITEM( DWORD item_id, DWORD unique  );					// �����۹�����
	void		SM__HORSE__DOWN( EVENT_DATA_INFO data );								// ��������
	bool		SM__HORSE__RIDE( EVENT_DATA_INFO data );								// ��Ÿ��	
	bool		SM__SLIDE__NPC( EVENT_DATA_INFO data );								// �з�����
	void		SM__REPLY__UNITE( bool bUnite, long unique);					// ��ü
	void		SM__SEPERATE__UNIT( DWORD pos );								// �и�	
	void		SM__MODE__CHANGE( BYTE type	);									// ��庯��
	void		SM__USE__NECTA__TAR( BYTE kind, bool bFlag );					// ��Ÿ, Ÿ������
*/		
	int			SM__CHANGE__ITEM(  DWORD item_id, int item_unique = -1 );		// �������Ա�
	int			SM__TAKEOFF__ITEM( DWORD item_id, int item_unique );			// �����۹���
	//-----------------------------------------------------------------------------
	// Desc : �ִϸ��̼������� ȣ���Լ�
	//-----------------------------------------------------------------------------	
	void		SoundPlay();											// ���� �÷���	
	int			AnimationPlay( int index );								// �ִϸ��̼��÷���			
	int			AnimationPlayNoneBlend( int index,int AniFrame=0);		// �ִϸ��̼��÷���(���� ����) By wxywxy
	int			SetWeaponMotionIndex( int index );						// ���⺰ ��� �缳��
	int			SetWeaponAttackMotionIndex( int index );				// ���⺰ ���� ��� �缳��
	int			SetWeaponReActionMotionIndex( int index );				// ���⺰ �´� ��� �缳�� 
	int			SetKeepUpAction( int index );							// ���⺰ �ߵ� ��� �缳��
	int			SetChantMotion( int aChant_State, int chant_grade );	// ���⺰ ��â ��� ���� 
	int			GetRunFrame();											// �ٱ� �����Ӹ���	
	float		GetAniTime();											// �̵���(�ȱ�, �ٱ�) �ִϸ��̼� ���� �ð���� 

	//-----------------------------------------------------------------------------
	// Desc : �ٿ��ڽ��� ����
	//-----------------------------------------------------------------------------			
	D3DXVECTOR3	GetBoundMinPos();
	D3DXVECTOR3	GetBoundMaxPos();
	D3DXVECTOR3	GetCBoundMaxPos();
	D3DXVECTOR3	GetCBoundMinPos();


	D3DXVECTOR3	GetBoundMaxVec(void);
	D3DXVECTOR3	GetBoundMinVec(void);
	
	//-----------------------------------------------------------------------------
	// Desc : ���� �̺�Ʈ ���� ����
	//-----------------------------------------------------------------------------	
	void		ClearEvent();												// �̺�Ʈ ����	
	int			GetAttackEvent( bool bSave = true );						// ���� ������?
	bool		GetStopEvent( bool bSave = true );							// ���� ����?

	//-----------------------------------------------------------------------------
	// Desc : ��庯��
	//-----------------------------------------------------------------------------
	void		SetState( STATE_INFO State );								// ���� ��������	
	int			UpdateReplyUnite();											// ��ü�ϴ� ����ó��	
	bool		SetSimSim();												// �ɽ��� �ൿ
		
	//-----------------------------------------------------------------------------
	// Desc : ����� �Լ���
	//-----------------------------------------------------------------------------	
	int			GetItmIndex( int _nIndex );			// �������ε���
	int			GetItmCount();						// ������ ���� 

	float		GetAttackTime();												// ���ݽð�
	D3DXVECTOR3	GetVertex( int index, int number );									// ���ؽ���	
	float		GetAttackTime( unsigned int motion );								// ���ݽð�	
	void		SetAngle( int angle ) { m_byAngle = (BYTE)angle; }					// ���� ���� 
	int			GetAngle( long pre, long current );									// �ٶ󺸴� ���� ���ϱ�	
	int			GetAngle( int prex, int prey, int curx, int cury );					// �ٶ󺸴� ���� ���ϱ�	
	void		SetDirection( float x, float z );									// �؈Թ�������ȸ���ϱ�
	void		SetDirection( float rot, bool bHorse = false );						// ���� ��������
	void		SetBreakEvent();													// ������ �̵���ġ�� �̵�
	void		SetAniTime( float time ) { m_ani_time = time; }						// �ִ�Ÿ�� ����	
	float		GetDirection()		{ return m_Mov.now_Yrad; }						// ����ĳ���� ȸ����	
	bool		SetMaterialColor( int index, int itm, int color );					// Į������
	void		ChangeHair( DWORD aItemCode );										// �Ӹ�ī��
	void		ChangeFace( DWORD aItemCode );										// ��	
	int			GetAttrIndex();														// �Ӽ��ε���	
	bool		GetRange( int range );												// �����ǰŸ�
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	GetBuffer( int index ) { return m_pVtxBuffer[index].NormalVB; }	// ���ؽ�����
#else
	LPDIRECT3DVERTEXBUFFER8	GetBuffer( int index ) { return m_pVtxBuffer[index].NormalVB; }	// ���ؽ�����
#endif // DIRECT_VERSION_9_MJH
	int			GetStatus();
	int			GetWearItemCode( int aPos );									

	BOOL		ISEmblemWear(WORD Code);

	//-----------------------------------------------------------------------------
	// Desc : ����ó��
	//-----------------------------------------------------------------------------
	void		SetUnderAttack();			// ��Ÿ�ݴ��ϴ� �ִϸ��̼�	
	bool		SetArrowCreate();			// ȭ�����


	//-----------------------------------------------------------------------------
	// Desc : Ŭ���� & �ε�������
	//-----------------------------------------------------------------------------				
	float		m_fDistance;				// �Ÿ���
	bool		m_bIsVisible;				// is object in frustum?
	bool		m_bIsLoaded;				// is obj loaded?
	
	//-----------------------------------------------------------------------------
	// Desc : ��������ó��
	//-----------------------------------------------------------------------------	
	FairyMove*	m_pFMove;

	void		InitFairyMove();									//����ü���� ����ũ(UniteUniq)�� �޾Ƽ� �� ������ ����ü ����
	void		OrderFairyMove( int type );							//���� �������� �����ϴ� �κ� 
	void		ProcessFairyMove();									//�� �������� ���������� ó�� 

	void		CalCenterPos( bool	RightShoulderOn );				//���� �������� ����� �Ǵ� ��ǥ ���
	void		CalAxisPos();										//ĳ���� �Ӹ��� ��ǥ ���  
	float		CalDistance( D3DXVECTOR3& v ){ return (float)sqrt((double)(v.x*v.x + v.y*v.y + v.z*v.z)); }			// ���ͱ��� �Ÿ� 
	float		CalDistance2( D3DXVECTOR3& v ){ return (float)sqrt((double)(v.x*v.x + v.z*v.z)); }			// ���ͱ��� �Ÿ� 
	float		CalAngle( D3DXVECTOR3& v );

	float		m_fPrevTime;
	int         m_nDust;

	CRenderBound	m_CollisionBox;

	//-----------------------------------------------------------------------------
	// Desc : Į �˱� ( By wxywxy )
	//-----------------------------------------------------------------------------	
	CTail		m_SwordTail;
	void		CalWeaponAxis(void);												// ���� �� ���

	
	////////////////////////////////////////////////////////////////////////////////////
	// �´� ��� ó��
	////////////////////////////////////////////////////////////////////////////////////
	bool		ReadyUnderAttackMotion( BOOL IsCritiCal );							// �´� ����� �� �� �ִ��� �Ǵ� (�̵����̰ų� �����߿��� ����)
	DWORD		SetUnderAttackMotion( DWORD aDamage );				// �������� ���� �´� ����� ����

	bool		SM__SLIDE__NPC( EVENT_DATA_INFO& data );			// �з�����

	////////////////////////////////////////////////////////////////////////////////////
	// ������ ��Ż
	////////////////////////////////////////////////////////////////////////////////////
	int			Item_Equip( DWORD aItemCode, int aEquipPos );		// ������ ����
	int			Item_TakeOff( int aEquipPos );						// ������ ���� 

	void		PreCharTextureCreate();								// ������ �ǽð� ������ ���� �ؽ����� ��� �̸� ����� ���´� 
	bool		DrawWeaponCheckStatus(bool bActCheck = true);		// �޽� ��ų�̳� �米������ ��� ���⸦ �׸��� �ʴ´� 

	////////////////////////////////////////////////////////////////////////////////////
	// ������ ��Ż
	////////////////////////////////////////////////////////////////////////////////////
	long		theVibrationTime;
	bool		theIsVibration;
	void		SetVibrationPos( D3DMATRIX* aMatWorld );

	////////////////////////////////////////////////////////////////////////////////////
	// NPC ���� ���ؼ� ����Ʈ ���� 
	////////////////////////////////////////////////////////////////////////////////////
	std::vector<QUESTDATA> thePreQuestCodeList;	
	QUEST_MARK_TYPE		   m_QuestMarkType;
	
	//QUEST_MARK_TYPE  m_QuestMarkType;								// ����Ʈ ��ũ Ÿ��
	
	
	BOOL	m_ISQuestEffectCreated;									// ����Ʈ ���� �߳�
	
	bool	theIsDrawWeapon;
	DWORD	theDrawWeaponFadeTime;
	bool	DrawWeaponFade();

	//-----------------------------------------------------------------------------
	// ��Ƽ ���� �޼��� ó�� 
	//-----------------------------------------------------------------------------
	bool		MultiAttackTargetMgr( EVENT_DATA_INFO& aEvent );


	//.........................................................................................................
	// ŵ�� ��ų ����ȭ ���� ������ By wxywxy
	//.........................................................................................................
	KEEPUP_SKILL_CHECKER	m_KeepUpChecker;

	//.........................................................................................................
	// NPC ����Ʈ ������ ���� by wxywxy
	//.........................................................................................................
public:
	
	int		m_NPCImpactFrame;						// NPC ����Ʈ ������ 
	bool	m_bNotImpact;
	
	//.........................................................................................................
	// ���� ���� By wxywy
	//.........................................................................................................
public:	
	
	// �������� ���� n_PC, n_Party , ���̵� ( ����ũ )
	void	SetPriority(BYTE thePriorityType,DWORD thePriorityID);	
private:

	BOOL	m_ISPriority;

protected:	
	CState   *m_pCurrentState; 

public:
	DWORD	dDestCell;
	bool	m_IsDefenceBack;		//����� �� - �����ڿ��� �������� ����
	bool	m_IsOwnerBack;			//�����ڿ��� ���ư���
	DWORD	dAttackDestCell;
	int		m_attack_type;
	DWORD   theAttackedID;
	D3DXVECTOR3	vDestPos;

	void	ChangeState(CState * pNewState);
	CState   * GetState()	 {	return m_pCurrentState; }

	int		m_TempIndex;
	SNOWATTACKINFO	m_NowAInfo;	

	BOOL	m_IsAllLTexture;		///-- �ؽ��İ� ���� �ε�Ǿ����� ����
	
	///-- ���� ����������κ� ������
	DWORD	FindFrontierID();
	void	ApplyIItemSlotAndFounded(int idx, BOOL * ISEmblemSlot, BOOL & ISEmblemWearFounded);
	
	void	GetXAndYRadian(float &fXRadian, float &fYRadian, 
									D3DXVECTOR3 vStartPos, D3DXVECTOR3 vDestPos);

};


#endif
