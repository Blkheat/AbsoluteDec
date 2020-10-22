
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

const UINT c_WeaponDelayTime( 2000 );				// 기본 딜레이가 2000 

const int c_Max_Illusion_Num( 2 );



typedef enum n_QuestMarkType						// 퀘스트 있을시에 퀘스트 마크 타입
{
	NONE_QUEST_MARK	   = -1 ,						// 퀘스트 마크 없음
	START_QUEST_MARK   =  0 ,						// 퀘스트 시작 마크 ( 느낌표 )
	REPORT_QUEST_MARK  =  1							// 퀘스트 보고 마크 ( 화살표 )

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


/* 서버에서 보내주는 10개의 장비 타입 슬롯 포지션 정의 
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
///-- BaseDataStructure.h에서 정의되어있지 않기에 이곳에서 열겨형을
///-- 정의
typedef enum NCharaterGender
{
	n_NoGender = 0,
	n_Male,
	n_FeMale,
	n_Boss,
	n_ColANPC,			///-- 콜로니 공성측 NPC
	n_ColDNPC,			///-- 콜로니 수성측 NPC
	n_4FootUnit,		///-- 4족 보행 유닛
	n_Millena_Unit,		///-- 밀레나 관련 유닛
	n_Rain_Unit,		///-- 레인 관련 유닛
	n_Neutral_Unit,		///-- 중립 관련 유닛

} NCharaterGender;


//-----------------------------------------------------------------------------
// Desc: 캐릭터 이동 클래스
//-----------------------------------------------------------------------------
struct TILE_INFO
{
	DWORD	x;
	DWORD	y;
	float	height;
};


struct MOVE_INFO
{
	D3DXVECTOR3	vStPos;			// 시작위치
	D3DXVECTOR3	vCVtPos;		// 충돌가상 위치
	TILE_INFO	strt;			// 시작타일
	TILE_INFO	curt;			// 현재타일
	TILE_INFO	next;			// 다음타일
	TILE_INFO	dest;			// 도착타일
	TILE_INFO	path;			// 길찾기타일

	float		step_x;			// x
	float		step_y;			// y
	float		step_height;	// 높이

	float		now_Yrad;		// 현재 Y라디안	
	float		nex_Yrad;		// 도착 Y라디안	
	float		add_Yrad;		// 증가 Y라디안
	float		now_Xrad;		// 현재 X라디안	
	float		nex_Xrad;		// 도착 X라디안	
	float		add_Xrad;		// 증가 X라디안

	bool		bYRotate;		// Y회전	
	bool		bXRotate;		// X회전
	int			rad_count;		// 회전카운트
	float		frad_remain;	// 남은 누적시간		
	
	MOVE_INFO()		{ Clear(); }
	~MOVE_INFO()	{}

	void	Clear();
	void	Init( int x, int y );
	bool	ValueUpdate();
	bool	FindValue( int x, int y, bool bNext = true );
};



//-----------------------------------------------------------------------------
// Desc: 큐 클래스
//-----------------------------------------------------------------------------
struct EVENT_DATA_INFO
{
	DWORD			type;
	int				race;	
	DWORD			tbl_Idx;			// 말 NPC 테이블 사용 
	DWORD			index;
	long			unique;	
	long			time;
	DWORD			x;
	DWORD			y;
	DWORD			motion;	
	bool			loop;				// false이면 죽기
	bool			kill;
	D3DXVECTOR3		vPos;	
	DWORD			damage;				// 타격 데미지 
	BYTE			critical;			// 크리티컬에 따라 모션 및 특수효과 발생 (클라이언트에서 판정할일 없으니까 콤보 타입(c_ComboZKey1)으로 사용 
	DWORD			cell;				// 0이 아니면 뒤로 나가 떨어지는 경우 , 말 행동여부 용도 사용함 
	DWORD			count;				// 콤보 성공 횟수에 따라 특수효과 발생 : UseSkill에서는 스킬 코드
	DWORD			code;				// 콤보 성공시 특수효과 발생, 스킬 코드값 
	BYTE			status;				// 콤보 완성 여부 
	DWORD			skill_unique;		// 광역 지속 스킬시 스킬 아이디
	BYTE			number;				// 스킬 공격시 공격 당한 사람 및 NPC 총 개수  , 모션테이블 상에 NPC Ani 번호 
	BYTE			weaponView[2];		// 종속 아이템 보여주기 여부
	BYTE			IsHideWing;			// 날개보여주기 여부
	BOOL			comcomple;			// 콤보 완성여부
	BOOL			IsActProc;			// 이미 처리되었는지 여부
	long			lStartTime;			// 이벤트 시작시간

	///-- By simwoosung
	int				nADisErrCnt;		// 공격시 거리체크 부분 에러 개수 	

	///--JAPAN_BUG_MODIFY
	DWORD			dKeepSkillCode;		// 지속스킬코드
	
	void*			target_list;		// 지역 공격시에 여러명에게 공격 하였을때 부가효과 처리	
	
	EVENT_DATA_INFO()	{ Init(); }
	~EVENT_DATA_INFO()	{ Clear(); }
	void	Init();
	void	Clear();

	void operator=(const EVENT_DATA_INFO & event);
	
};

//-----------------------------------------------------------------------------
// Desc: 버텍스버퍼
//-----------------------------------------------------------------------------
struct	VTX_BUFFER_INFO
{
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		NormalVB;	// 버텍스버퍼
#else
	LPDIRECT3DVERTEXBUFFER8		NormalVB;	// 버텍스버퍼
#endif // DIRECT_VERSION_9_MJH	
	D3DXVECTOR3*				pVertex;	// 버텍스값
	bool						bInit;		// 버퍼초기화
	
	int							nSize;

	VTX_BUFFER_INFO()	{ Clear(); }
	~VTX_BUFFER_INFO()	{ Destroy(); }

	void	Clear();
	void	Destroy();	
	void	CreateBuffer( int _nVtxCount );
};

typedef enum NCharState
{
	nCharState_Normal = 0 ,				// 캐릭터 기본
	nCharState_Master,					// 분신을 가진 주인
	nCharState_Illusion,				// 분신임 
	nCharState_Detect_Illusion			// 보이는 분신임 
	
} NCharState;

typedef enum NHolyState
{
	nHolyState_Normal = 0  ,			// NPC 기본
	nHolyState_Master      ,			// 분신을 가진 주인
	nHolyState_Holy  	   ,			// 신수
} NHolyState;

typedef enum NRideState
{
	nRideState_Normal = 0	,			// 라이딩 기본
	nRideState_Master		,			// 라이딩 주인
 	nRideState_Ride			,			// 라이딩 대상		
} NRideState;

typedef enum NPetState
{
	nPetState_Normal = 0	,			// 펫 기본	
	nPetState_Master		,			// 펫 주인	
	nPetState_Pet			,			// 펫 대상	
} NPetState;


///-- By simwoosung
///-- 캐릭터 타입
typedef enum NCharacterType
{
	nCharacterTypeNormal = 0	,		/// 기본 타입			
	nCharacterTypePlayerNPC		,		/// 플레이어 리소스를 사용하는 NPC
	nCharacterTypeBigMoster,			/// 거대 몹
	nCharacterTypeBulding,				/// 거대 건물 유닛		
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
// Desc: 현재 자기 상태
//-----------------------------------------------------------------------------
struct STATE_INFO
{	
	bool		bWalk;									// 걷고 있는지 뛰고 있는지...지금은 계속 뛰기...걷기 없음...

	NRideState	nRideState;								// 라이딩 상태값
	bool		bRide;									// 타고 있는지 아닌지 
	int			TransIndex;								// 마스터라면 운송 수단의 Index 운송수단이라면 Master Index

	NCharState	nCharState;								// 캐릭터 상태값 
	
	int			IllusionIndex[c_Max_Illusion_Num];		// 마스터라면 환영의 Index, 환영이라면 Mastrer Index
	
	NHolyState  nHolyState;								// 신수 관련 상태값
	int			HolyIndex;								// 마스터라면 신수 Index, 신수라면 Mastrer Index

	WORD		BaseCode;								// 신수라면 기본 NPC 코드 값

	NPetState	nPetState;								// 펫 관련 상태값
	int			PetIndex;								// 마스터라면 펫의 Index 펫이면 마스터의 Index
	
	NCharacterType nCharacterType;						// 캐릭터 타입 
	
	UINT		nWeaponLightType;						// 무기 발광 렌더링 타입
	
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
// Desc: 유저 전용 변수
//-----------------------------------------------------------------------------
struct USER_INFO
{
	ARMOR_INFO		ArmorInfo;			// 무기정보

	int				HeadCurFrame;		// 머리회전 현재프레임
	int				HeadPreFrame;		// 머리회전 이전프레	
	long			HeadTime;			// 머리회전 타임
	
	bool			bPkMode;			// PK모드
	bool			bDown;				// 앉았니?
	bool			bCharge;			// 차지상태 
	bool			bExtacy;			// 엑스터시상태
	bool			bAttack;			// 공격대기상태
	bool			bDefense;			// false:정상	true:디펜스	
	BYTE			EtcItem;			// 0:없다 1:넥타 2:타르	3:의자

	float			fEmitterScale;		// 발광스케일

	//.........................................................................
	// 검기 관련
	//.........................................................................
	D3DXVECTOR3			vSwordTop; 						// 칼 max값
	D3DXVECTOR3			vSwordBottom;					// 칼 min값

	D3DXVECTOR3			vSwordAxis[2];					// 이동된 칼 축  ( 0 : 칼 끝 , 1 : 칼 밑둥 )

	USER_INFO()		{ Clear(); }
	~USER_INFO()	{ Clear(); }
	void Clear();
};

//-----------------------------------------------------------------------------
// Desc: 페어리 움직임 구조체 
//-----------------------------------------------------------------------------

#define					FAIRYMOVRADIUS			4.0f		

struct	FairyMove
{
	BYTE				Type;					// 현재 타입		0 가만히 1 움직일때 2 심심 

	long				UniteUniq;				// 피 합체자의 유니크 
	bool				RightShoulderOn;		// 오른쪽 어깨에 있으면 true

	D3DXVECTOR3			CenterPos;				// 캐릭터 좌표 + 바운드 박스 맥스 Y값 
	bool				ClockRotate;			// 회전 방향 true가 시계방향 
	float				Speed;					// Vec의 스피드
	float				Dir;					

	D3DXVECTOR3			AxisPos;				// 캐릭터 바로 머리 위 

	D3DXVECTOR3			VitualPos;				// 가상의 원 안에 들어 있는 좌표 
	D3DXVECTOR3			VitualPosToCen;			// 가상의 원으로 부터 중앙 까지의 벡터 
	float				VitualPosRange;			// 이값은 항상 같은 	FAIRYMOVRADIUS 이다.

	D3DXVECTOR3			Pos;					// 속도가 더해진 새로운 좌표 
	D3DXVECTOR3			PosToCen;				// 위 좌표와 중앙까지의 벡터 
	float				PosRange;				// 그 거리 

	float				Life;					// 생성후 현재 시간 
	float				Height;					// 높이 

	D3DXVECTOR3			RealPos;				// RealPos = RealDis* (RealRange / Range) + CenterPos;
	float				RealRange;				// RealPos의 거리

	D3DXVECTOR3			UnitVec;				// UnitVec.x	=	((2*ClockRotate)-1) * RealDis.z * sqrt( RealDis.x * RealDis.x + RealDis.z * RealDis.z )
												// UnitVec.z	=	-1 * ((2*ClockRotate)-1) * RealDis.x * sqrt( RealDis.x * RealDis.x + RealDis.z * RealDis.z )
	D3DXVECTOR3			Vec;					// Vec = UnitVec * Speed;


	D3DXMATRIX			Loc;					// 최종 생성할 매트릭스 
};

class CEffectBase;								// 이펙트 베이스 클래스 by wxywxy

// 캐릭터 렌더링 상태값
typedef enum CHAR_ALPHA_STATE
{
	CHAR_NORMAL			= 0 ,					// 캐릭터 기본
	CHAR_HIDE			= 1 ,					// 숨기기 ( 알파값 적용 )
	CHAR_NONE_DISPLAY	= 2						// 표시 안하기
};

typedef struct s_CHAR_ALPHA
{
	CHAR_ALPHA_STATE	State;
	float				fAlpha;
	BOOL				bShow;	// 은신 비약 감지를 사용했을시 .. 
	

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
	
	void SetShow(BOOL bshow) //은신 사용했고 비약에 감지 당했을때 세트 된다. 
	{
		bShow = bshow;
	}
	
	void Set(CHAR_ALPHA_STATE state,float alpha)
	{
		State  = state;
		fAlpha = alpha;
	}

}CHAR_ALPHA;


//BY simwoosung - 캐릭터 착용 의상 상태
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
// 킵업 스킬 동기화 관련 체크 데이터 By wxywxy
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
// Desc: 캐릭터 클래스 (상태 클래스에서 기타 행동에 대한 처리를 따로 분리시켜놓는다)
//-----------------------------------------------------------------------------
class Character
{
public:
	bool			m_bPC;						// 유저?
	int				m_nTableIndex;				// 테이블인덱스	
	BYTE			m_byAngle;					// 현재 바라보는 각
	float			m_radian;				
	USER_INFO*		m_pUserInfo;				// 유저전용데이타
	bool			m_bSoundPlay1;				// 사운드플레이1
	bool			m_bSoundPlay2;				// 사운드플레이2
	bool			m_ISAniProcess;				// 에니메이션 계산 여부	
	
	long			m_lPreTime;					// HP 정보 마지막 갱신시간
	long			m_lModifyHpTime;			// HP 동기화 최종갱신시간
	long			m_lLevelUpTime;				// 레벨업 마지막 갱신시간
	
	BOOL			m_IsInitUpdate;				// 초기 업데이트 여부
	BOOL			m_IsNextApplyVA;			// 다음 애니렌더시 스키닝 적용여부
	BOOL			m_IsApplyVA;				// 스키닝 적용여부
	long			m_nPrevVATime;				// 스키닝 이전타임
	long			m_nDelVATime;				// 스키닝 간격타임
	BOOL			m_IsRenderImg;				// 이미지를 그리는지 여부
	int				m_nCamDistance;

	void			SetCamDistance(int nDis);
	
protected:
	BOOL			m_IsRealDie;				// 실제 죽어있는지 여부
	BYTE			m_aAttackType;				// 실제로 죽인 대상의 타입
	DWORD			m_aAttackID;				// 실제로 죽인 대상의 아이디
	BOOL			m_IsApplyConAniSkill;		// 연속 애니메이션 스킬적용 여부
	int				m_aConAniTime;				// 연속 애니메이션 적용타임
	
	BOOL			m_IsSkillNoAni;				// 스킬은 발동되는데 애니메이션이 없을경우

public:
	BOOL			IsRealDie()		{ return m_IsRealDie; }
	void			SetRealDie( BOOL bFlag, BYTE aAttackType = 0, DWORD aAttackID = 0 );
	BOOL			IsApplyConAniSkill()	{ return m_IsApplyConAniSkill; }
	void			SetApplyConAniSkill(BOOL bFlag);
	int				GetConAniTime()	{ return m_aConAniTime; }
	void			SetConAniTime(int nTime); 

	void			AddMatPosMatPos(D3DXMATRIX &matOut, D3DXMATRIX &mat1, D3DXMATRIX &mat2);

//...........................................................................................................
// 얼굴 표정
//...........................................................................................................
public:
	DWORD					 m_FaceExpTimer;
	CFaceExpData			*m_pCurrentFaceExpData;	
	CFaceExpData::FACE_EXP	 m_NowFaceType;

	void ChangeFaceExp(CFaceExpData::FACE_EXP eType);
	void FaceExpProcess(void);

//...........................................................................................................
// 부가 효과 By wxywxy
//...........................................................................................................
public:
	
	D3DXCOLOR		m_EffectColor;				// 이펙트 걸렸을때 색상값
	BOOL			m_ISEffectColor;			// 이펙트 칼라값 적용

//...........................................................................................................
// 아이템 착용상태
//...........................................................................................................
public:
	CHAR_WEAR_TYPE	m_Char_WearType;			//아이템 착용상태	

public:
	//by simwoosung
	D3DXVECTOR3	    m_MaxBoundVec;              //최대 바운드 위치         
	D3DXVECTOR3	    m_MinBoundVec; 	            //최소 바운드 위치

    BOOL			m_BIsBoundSet;				//바운드 높이의 설정여부
	float			m_fMaxHeight;				//최고 바운드 높이
	float			m_fMinHeight;				//최저 바운드 높이
	
	int				m_nAddNum;					//채팅창을 더 올려주기 위해 쓰는 추가수
	
	float			m_fAddRideHeight;			//라이딩시 캐릭터 아이디를 더 올려주기 위한 높이
	float			m_fAddHeight;				//라이딩시 캐릭터를 더 올려주기 위한 높이
	float			m_fAddForward;				//라이딩시 캐릭터위치 조정용 숫자
//-----------------------------------------------------------------------------
// Desc: 이펙트 관련 By wxywxy
//-----------------------------------------------------------------------------
public:
	
	std::list	<CEffectBase *>		m_EffectList;

public:
	
	int EffectCheck(void);							// 이펙트 프레임별로 체크
	int EffectSortRender(void);						// 소트된 캐릭터 찍을때 이펙트 찍기

	int PopEffectBase(CEffectBase **ppEffectBase);	// 이펙트 삭제 
	int PushEffectBase(CEffectBase **ppEffectBase); // 이펙트 추가 

//-----------------------------------------------------------------------------
// Desc: 상태, 이벤트
//-----------------------------------------------------------------------------
public:
	DWORD			theInputedDestCell;	/// sooree network
	inline void		SetInputedDestCell( DWORD aDestCell )	{ theInputedDestCell = aDestCell;	};
	inline DWORD	GetInputedDestCell()					{ return( theInputedDestCell );		};


	STATE_INFO			m_State;					// 자기상태
	MOVE_INFO			m_Mov;						// 이동 정보	

	EVENT_DATA_INFO		m_curt_event;				// 현재 이벤트
	EVENT_DATA_INFO		m_save_event;				// 저장 이벤트
	EVENT_DATA_INFO		m_next_event;				// 다음 이벤트 
	EVENT_DATA_INFO		m_temp_event;				// 임시 이벤트

	long				m_Attack_DelayTime;			// 공격 딜레이 시간				
	long				m_Weapon_DelayTime;			// 무기별 딜레이 시간 
//-----------------------------------------------------------------------------
// Desc: 그림자
//-----------------------------------------------------------------------------
protected:	
	D3DXVECTOR3			m_vLight;					// 라이트위치
	bool				m_bShadowDisplay;			// 그림자 렌더링여부
	bool				m_bShadowMake;				// 그림자 생성여부
	///-- by simwoosung
	UINT				m_uShadowSizeType;			// 그림자 크기타입
	int					m_nShadowSize;
	
public:
	inline void			SetShadowSizeType(UINT nType)	{ m_uShadowSizeType = nType; }

//-----------------------------------------------------------------------------
// Desc: 기본정보
//-----------------------------------------------------------------------------
public:	
	SHADOW_INFO			m_Shadow;							// 그림자	
	int					m_pItm_index[c_Max_WearItemType];	// itm 인덱스(헤어, 얼굴, 상의, 하의, 장갑, 부츠, 서브장비...총 7개)
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_pMaterial[c_Max_WearItemType];	// 메터리얼
#else
	D3DMATERIAL8		m_pMaterial[c_Max_WearItemType];	// 메터리얼
#endif // DIRECT_VERSION_9_MJH	
	VTX_BUFFER_INFO		m_pVtxBuffer[c_Max_WearItemType];	// 버텍스버퍼		
	D3DXVECTOR3			m_vScale;							// 스케일

	// 기본 의상 
	int					m_Basic_DrawItm[c_Max_WearItemType];	// 아이템과 함께 동시에 그려줄지 결정 (헤어, 얼굴 등)
	int					m_Basic_Itm_index[c_Max_WearItemType];	// itm 인덱스(헤어, 얼굴, 상의, 하의, 장갑, 부츠, 서브장비...총 7개)
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_Basic_Material[c_Max_WearItemType];	// 메터리얼	
#else
	D3DMATERIAL8		m_Basic_Material[c_Max_WearItemType];	// 메터리얼	
#endif // DIRECT_VERSION_9_MJH
	VTX_BUFFER_INFO		m_Basic_VtxBuffer[c_Max_WearItemType];	// 버텍스버퍼

	// 라이딩 아이템
	int					m_Ride_Item_index[c_Max_RideItemType];	// 라이딩 아이템 인덱스
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9		m_Ride_Material[c_Max_RideItemType];	// 메터리얼
#else
	D3DMATERIAL8		m_Ride_Material[c_Max_RideItemType];	// 메터리얼
#endif // DIRECT_VERSION_9_MJH	
	VTX_BUFFER_INFO		m_Ride_VtxBuffer[c_Max_RideItemType];	// 버텍스버퍼
	
	
	CHAR_ALPHA			m_CharAlpha;							// 캐릭터 알파값 세팅

//-----------------------------------------------------------------------------
// Desc: 버텍스, 애니메이션정보
//-----------------------------------------------------------------------------
public:		
	int					m_ani_number;				// 현재 애니메이션 번호
	int					m_pre_ani_number;			// 이전 애니메이션 번호
	int					m_ani_index;				// 현재 애니메이션 인덱스	
	int					m_ani_frame;				// 애니 프레임

	int					m_next_frame;
	int					m_curt_frame;
	
	///-- By simwoosung
	int					m_start_blend_frame;		// 시작 블렌딩 프레임

	unsigned int		m_end_frame;				// 현재 종료 프레임			
	unsigned int		m_pre_end_frame;			// 이전 종표 프레임			

	int					m_blend_frame;				// 모션 블렌딩 프레임 

	int					m_move_frame;				// 이동 프레임 
	float				m_move_middle_time;			// 이동 보간 프레임
	
	float				m_move_percent;				// 이동거리 퍼센트 
	float				m_ani_percent;				// 애니메이션 퍼센트 
	
	float				m_step_time;				// 보폭타임
	float				m_ani_time;					// 애니 타임
	float				m_realani_time;				// 실제 애니타임
	float				m_middle_time;				// 1프레임 ~ 2프레임 사이의 중간값 저장 ( 0.0 ~ 1.0 사이 )	
	long				m_PreTime;					// 이전 렌더링한 시간
	long				m_PreMTime;					// 이동이전 렌더링 시간
	long				m_SimTime;					// 심심한 시간	

public:
	CFindWay*			theFindWay;					// 길찾기 알고리즘 
	DWORD				thePath[6];					// NPC 패스 이동 
	int					theCurtPath;				// 현재 몇번째 패스인지...
	int					theDestPath;				// 목적지 패스 번호 
	D3DXVECTOR3			m_vDir;						// 캐릭터의 방향 벡터
	
	BOOL				m_ISApproachWeapon;			// 근접 무기 유/무
	UINT				m_AttackMotion;				// 아이템 공격모션 번호
	
	D3DXMATRIX			m_matRideChr;				// 라이딩시 캐릭터위치

//-----------------------------------------------------------------------------
// Desc: 월드행렬 매트릭스
//-----------------------------------------------------------------------------
protected:						
	D3DXMATRIX			m_matPreChr;				// 캐릭터이전 위치
	D3DXMATRIX			m_matChr;					// 캐릭터위치
	D3DXMATRIX			m_matRHand;					// 오른손
	D3DXMATRIX			m_matLHand;					// 왼손	
	D3DXMATRIX			m_matRHand2;				// 오른손2(타르,넥타등에 사용)

	D3DXMATRIX			m_matORHand;					// 오른손
	D3DXMATRIX			m_matOLHand;					// 왼손	
	D3DXMATRIX			m_matORHand2;				// 오른손2(타르,넥타등에 사용)

public:
	D3DXVECTOR3			m_maxBoundVec;				// 바운딩 박스_Max
	D3DXVECTOR3			m_minBoundVec;				// 바운딩 박스_Min

	D3DXMATRIX			m_matRealRHand;				// 실제 오른손
	D3DXMATRIX			m_matRealLHand;				// 실제 왼손
	D3DXMATRIX			m_matRealHair;				// 실제 머리 본
	D3DXMATRIX			m_matRealbreast;			// 실제 가슴 본
	D3DXMATRIX			m_matRealCenter;			// 실제 낭심 본
	D3DXMATRIX			m_matRealLFoot;				// 실제 왼발 본
	D3DXMATRIX			m_matRealRFoot;				// 실제 오른발 본

	D3DXMATRIX			m_matRealLShoulder;			// 실제 왼 어꺠 
	D3DXMATRIX			m_matRealRShoulder;			// 실제 오른 어깨 

	D3DXMATRIX			m_matRealLElbow;			// 실제 왼 꿈치
	D3DXMATRIX			m_matRealRElbow;			// 실제 오른 꿈치


	D3DXMATRIX			m_matRealLKnee;				//	실제 왼쪽 무릎 
	D3DXMATRIX			m_matRealRKnee;				// 실제 오른 무릎 
	D3DXMATRIX			m_matRealLPelvis;			// 실제 왼 골반 
	D3DXMATRIX			m_matRealRPelvis;			// 실제 오른 골반 


	D3DXMATRIX			m_matRealORHand;			// 실제 오른손
	D3DXMATRIX			m_matRealOLHand;			// 실제 왼손
	D3DXMATRIX			m_matRealOHair;				// 실제 머리 본
	D3DXMATRIX			m_matRealObreast;			// 실제 가슴 본
	D3DXMATRIX			m_matRealOCenter;			// 실제 낭심 본
	D3DXMATRIX			m_matRealOLFoot;			// 실제 왼발 본
	D3DXMATRIX			m_matRealORFoot;			// 실제 오른발 본
	D3DXMATRIX			m_matRealOLShoulder;		// 실제 왼 어꺠 
	D3DXMATRIX			m_matRealORShoulder;		// 실제 오른 어깨 
	D3DXMATRIX			m_matRealOLElbow;			// 실제 왼 꿈치
	D3DXMATRIX			m_matRealORElbow;			// 실제 오른 꿈치
	D3DXMATRIX			m_matRealOLKnee;			//	실제 왼쪽 무릎 
	D3DXMATRIX			m_matRealORKnee;			// 실제 오른 무릎 
	D3DXMATRIX			m_matRealOLPelvis;			// 실제 왼 골반 
	D3DXMATRIX			m_matRealORPelvis;			// 실제 오른 골반 



	D3DXMATRIX			m_matRideCenter;			// 라이딩에서 쓸 가슴 	

	D3DXMATRIX			m_matBoneCenter;			// 0번 본에서 수직으로 내려온 좌표 ( 이펙트에서 중심으로 쓸꺼에용 )

	CSpellEffect		m_SpellEffect;				// 영창 이펙트

	int					GetWeaponType(void);

//-----------------------------------------------------------------------------
// Desc: 함수
//-----------------------------------------------------------------------------
public:
	void		SetItemBuffer( int index );					// 아이템 버퍼 재할당하기
	void		SetBasicItemBuffer( int _nIndex );
	void		SetRideItemBuffer( int _nIndex );

protected:	
	void		AlphaProcess();								// 알파블렌딩
	void		SetHeightStep( bool _bInit = false );		// 높이값생성(높이저장)
	void		SetRotate( TILE_INFO	tPos );				// 회전값생성(회전저장) 
	bool		SetMoveStep( TILE_INFO	tPos );				// 보폭값생성(보폭저장)	
	void		GetMoveStep( int dest_x, int dest_y, int now_x, int now_y, float &x, float &y );	// 보폭값리턴
			
	void		Move( float middle_time );										// 정상이동
	void		MoveRatio( float fRatio );										// 비율 보간 이동
	void		MiddleMove( float middle_time );								// 보간이동
	///--JAPAN_BUG_MODIFY
	void		Rotate( float fRatio, int aAddFrame );		// 이동시 캐릭터 방향 애니메이션 
	bool		AniSearch( unsigned int ani_number );		// 애니메이션검사

public:
	Character()		{ Init(); }

	~Character()	{ Free(); }		

	//-----------------------------------------------------------------------------
	// Desc : 변수 초기화
	//-----------------------------------------------------------------------------
	void		Init();
	void		Free();

	//-----------------------------------------------------------------------------
	// Desc : 렌더링
	//-----------------------------------------------------------------------------	
	void		UpdateTexture();												// 텍스쳐업데이트
	bool		AniRender(bool bFlag = false);									// 캐릭터애니버퍼셋팅
	bool		BoneRender();
	bool		ViewModelPC(void);
	bool		ViewModelNPC(void);
	
	void		Display( BYTE _byType = 0, BYTE _bySort = 0, bool bFlag = false, BOOL IsRender = TRUE );					// 캐릭터
	bool		DisplayLine( BYTE _byType = 0, BYTE _bySort = 0 );				// 외각선
	bool		DisplayMotionBlow();											// 모션블로어
	bool		DisplayAfterImage();											// 칼검기
	void		DisplayEffect();												// 이펙트	
	void		DisplayBound( BYTE _byType = 0 );								// 바운드박스
	//by simwoosung
	void		DisplayBoundBox();

	///-- By simwoosung -- 잔상 렌더링
	bool		DisplayAfterImage(SAImageData & sIData);						// 캐릭터 잔상
	// 아이템 애니메이션
	void		ItemAnimation(SAImageData & sIData, int type = -1, bool bChange = false, 
							  int next_frame = 0, int curt_frame = 0 );

	void		MeshAnimation( int mod );										// 메쉬 애니메이션	
	D3DXMATRIX	HeadAnimation();												// 머리 애니메이션	
	
	void		RenderToAlpha(D3DXVECTOR3 vPos,float Alpha, 
							  BOOL IsRenderWeapon = TRUE, SAImageData * psIDat = NULL);
	
	void		RenderToAlphaEffect(D3DXVECTOR3 vPos,float Alpha, float Scale, D3DXCOLOR Color , 
									BOOL IsRenderWeapon = TRUE, SAImageData * psIDat = NULL);

	
	BOOL		PreTextureLoad(void);											// 미리 텍스처 읽기

	int			GetBoneLinkTM(int Num, D3DXMATRIX &mat, BOOL IsModify = TRUE);							// 이펙트 출력시 본 메트릭스 구하기 ( NPC 도 추가 )
	int			GetDirectBoneLinkTM(int Num,  D3DXMATRIX &mat );
	
	EVENT_DATA_INFO	 *GetEvent(void)	{return &m_curt_event;}

	// 아이템 애니메이션
	void		ItemAnimation( int type = -1, bool bChange = false, int next_frame = 0, int curt_frame = 0 );
	bool		AniFrameProcess();												// 애니메이션거꾸로?
	bool		UpdateFrame();													// 거꾸로애니메이션처리
	int			GetNextFrame();													// 다음프레임값 리턴

	// 모션 블렌딩을 위한 쿼터니온 블렌딩
	void		blendQuaternion( D3DXQUATERNION* blend, float factor, D3DXQUATERNION src, D3DXQUATERNION dest );

	//-----------------------------------------------------------------------------
	// Desc : 캐릭터 위치타일 넣기, 리턴
	//-----------------------------------------------------------------------------	
	void		SetTilePos( int _nPosX, int _nPosY );					// 현재타일위치, 메트릭스값
	void		SetPosTM( const D3DXMATRIX matWorld );					// 현재메트릭스
	bool		GetRealTM( float& _fPosX, float& _fPosY );				// 타일속성에 따른 메트릭스
	
	void		SetPosTM2(D3DXMATRIX mat) { m_matChr = mat; }
	D3DXMATRIX	GetPosTM();												// 메트릭스 리턴
	D3DXMATRIX  GetBonCenterTM();
	D3DXMATRIX  GetPosEffTM();
	D3DXMATRIX  GetPosEyeTM();
	D3DXMATRIX	GetPosCenterBoneTM() { return m_matBoneCenter; }		// 메트릭스 ( 0 번 본 )

	D3DXMATRIX	GetRHandTM()	{ return m_matRHand; }					// 오른손 리턴
	D3DXMATRIX	GetLHandTM()	{ return m_matLHand; }					// 오른손 리턴
	
	D3DXMATRIX	GetBoneTM( DWORD _dwNode );								// node 메트릭스 리턴
	D3DXMATRIX	GetBoneRealTM( DWORD _dwNode );							// Node 메트릭스 리턴 By wxywxy

	void        CalLinkBone(int ani,int pre,int curt_frame,int next_frame,int MotionBlendHalfFrame,float factor,bool bMotionBlend);	// Desc: 링크 처리 본 계산하기
	D3DXMATRIX  GetLinkBone(int ani,int pre,int curt_frame,int next_frame,int MotionBlendHalfFrame,float factor,bool bMotionBlend,int id, bool bRide = false);
	D3DXMATRIX  GetRideLinkBone(int ani,int pre,int curt_frame,int next_frame,int MotionBlendHalfFrame,float factor,bool bMotionBlend,int id);

	D3DXMATRIX	GetRealRHandTM()	{ return m_matRealRHand; }					// 오른손 리턴
	D3DXMATRIX	GetRealLHandTM()	{ return m_matRealLHand; }					// 오른손 리턴
	
	long        GetUnique(void);										// 유니크 얻기
	int         GetIndex(void);											// 인덱스 얻기

	//-----------------------------------------------------------------------------
	// Desc : 서버 -> 클라이언트 메세지	
	//-----------------------------------------------------------------------------		
/*	bool		SM__USE__SKILL( EVENT_DATA_INFO data );								// 스킬사용
	bool		SM__ARROW__GOING( EVENT_DATA_INFO data );							// 화살사용 
	void		SM__CHR__DIR( DWORD dir );										// 머리회전
	int			SM__CHANGE__ITEM(  DWORD item_id, int item_unique = -1 );		// 아이템입기
	int			SM__TAKEOFF__ITEM( DWORD item_id, int item_unique = -1 );		// 아이템벗기
	void		SM__PICKUP__ITEM( DWORD item_id, DWORD unique, DWORD life );	// 아이템줍기
	void		SM__DROP__ITEM( DWORD item_id, DWORD unique  );					// 아이템버리기
	void		SM__HORSE__DOWN( EVENT_DATA_INFO data );								// 말내리기
	bool		SM__HORSE__RIDE( EVENT_DATA_INFO data );								// 말타기	
	bool		SM__SLIDE__NPC( EVENT_DATA_INFO data );								// 밀려나기
	void		SM__REPLY__UNITE( bool bUnite, long unique);					// 합체
	void		SM__SEPERATE__UNIT( DWORD pos );								// 분리	
	void		SM__MODE__CHANGE( BYTE type	);									// 모드변경
	void		SM__USE__NECTA__TAR( BYTE kind, bool bFlag );					// 넥타, 타르섭취
*/		
	int			SM__CHANGE__ITEM(  DWORD item_id, int item_unique = -1 );		// 아이템입기
	int			SM__TAKEOFF__ITEM( DWORD item_id, int item_unique );			// 아이템벗기
	//-----------------------------------------------------------------------------
	// Desc : 애니메이션종류별 호출함수
	//-----------------------------------------------------------------------------	
	void		SoundPlay();											// 사운드 플레이	
	int			AnimationPlay( int index );								// 애니메이션플레이			
	int			AnimationPlayNoneBlend( int index,int AniFrame=0);		// 애니메이션플레이(블렌딩 없이) By wxywxy
	int			SetWeaponMotionIndex( int index );						// 무기별 모션 재설정
	int			SetWeaponAttackMotionIndex( int index );				// 무기별 공격 모션 재설정
	int			SetWeaponReActionMotionIndex( int index );				// 무기별 맞는 모션 재설정 
	int			SetKeepUpAction( int index );							// 무기별 발동 모션 재설정
	int			SetChantMotion( int aChant_State, int chant_grade );	// 무기별 영창 모션 설정 
	int			GetRunFrame();											// 뛰기 프레임리턴	
	float		GetAniTime();											// 이동시(걷기, 뛰기) 애니메이션 갱신 시간얻기 

	//-----------------------------------------------------------------------------
	// Desc : 바운드박스값 리턴
	//-----------------------------------------------------------------------------			
	D3DXVECTOR3	GetBoundMinPos();
	D3DXVECTOR3	GetBoundMaxPos();
	D3DXVECTOR3	GetCBoundMaxPos();
	D3DXVECTOR3	GetCBoundMinPos();


	D3DXVECTOR3	GetBoundMaxVec(void);
	D3DXVECTOR3	GetBoundMinVec(void);
	
	//-----------------------------------------------------------------------------
	// Desc : 현재 이벤트 관련 리턴
	//-----------------------------------------------------------------------------	
	void		ClearEvent();												// 이벤트 종료	
	int			GetAttackEvent( bool bSave = true );						// 현재 전투중?
	bool		GetStopEvent( bool bSave = true );							// 현재 멈춤?

	//-----------------------------------------------------------------------------
	// Desc : 모드변경
	//-----------------------------------------------------------------------------
	void		SetState( STATE_INFO State );								// 현재 상태저장	
	int			UpdateReplyUnite();											// 합체하는 동안처리	
	bool		SetSimSim();												// 심심한 행동
		
	//-----------------------------------------------------------------------------
	// Desc : 잡다한 함수들
	//-----------------------------------------------------------------------------	
	int			GetItmIndex( int _nIndex );			// 아이템인덱스
	int			GetItmCount();						// 아이템 갯수 

	float		GetAttackTime();												// 공격시간
	D3DXVECTOR3	GetVertex( int index, int number );									// 버텍스값	
	float		GetAttackTime( unsigned int motion );								// 공격시간	
	void		SetAngle( int angle ) { m_byAngle = (BYTE)angle; }					// 방향 저장 
	int			GetAngle( long pre, long current );									// 바라보는 방향 구하기	
	int			GetAngle( int prex, int prey, int curx, int cury );					// 바라보는 방향 구하기	
	void		SetDirection( float x, float z );									// 해댱방향으로회전하기
	void		SetDirection( float rot, bool bHorse = false );						// 현재 방향저장
	void		SetBreakEvent();													// 마지막 이동위치로 이동
	void		SetAniTime( float time ) { m_ani_time = time; }						// 애니타임 저장	
	float		GetDirection()		{ return m_Mov.now_Yrad; }						// 현재캐릭터 회전값	
	bool		SetMaterialColor( int index, int itm, int color );					// 칼라저장
	void		ChangeHair( DWORD aItemCode );										// 머리카락
	void		ChangeFace( DWORD aItemCode );										// 얼굴	
	int			GetAttrIndex();														// 속성인덱스	
	bool		GetRange( int range );												// 나와의거리
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	GetBuffer( int index ) { return m_pVtxBuffer[index].NormalVB; }	// 버텍스버퍼
#else
	LPDIRECT3DVERTEXBUFFER8	GetBuffer( int index ) { return m_pVtxBuffer[index].NormalVB; }	// 버텍스버퍼
#endif // DIRECT_VERSION_9_MJH
	int			GetStatus();
	int			GetWearItemCode( int aPos );									

	BOOL		ISEmblemWear(WORD Code);

	//-----------------------------------------------------------------------------
	// Desc : 공격처리
	//-----------------------------------------------------------------------------
	void		SetUnderAttack();			// 피타격당하는 애니메이션	
	bool		SetArrowCreate();			// 화살생성


	//-----------------------------------------------------------------------------
	// Desc : 클리핑 & 인덱스저장
	//-----------------------------------------------------------------------------				
	float		m_fDistance;				// 거리값
	bool		m_bIsVisible;				// is object in frustum?
	bool		m_bIsLoaded;				// is obj loaded?
	
	//-----------------------------------------------------------------------------
	// Desc : 페어리움직임처리
	//-----------------------------------------------------------------------------	
	FairyMove*	m_pFMove;

	void		InitFairyMove();									//피합체자의 유니크(UniteUniq)를 받아서 페어리 움직임 구조체 설정
	void		OrderFairyMove( int type );							//페어리의 움직임을 제어하는 부분 
	void		ProcessFairyMove();									//페어리 움직임을 직접적으로 처리 

	void		CalCenterPos( bool	RightShoulderOn );				//페어리의 움직임의 기반이 되는 좌표 계산
	void		CalAxisPos();										//캐릭터 머리위 좌표 계산  
	float		CalDistance( D3DXVECTOR3& v ){ return (float)sqrt((double)(v.x*v.x + v.y*v.y + v.z*v.z)); }			// 벡터까지 거리 
	float		CalDistance2( D3DXVECTOR3& v ){ return (float)sqrt((double)(v.x*v.x + v.z*v.z)); }			// 벡터까지 거리 
	float		CalAngle( D3DXVECTOR3& v );

	float		m_fPrevTime;
	int         m_nDust;

	CRenderBound	m_CollisionBox;

	//-----------------------------------------------------------------------------
	// Desc : 칼 검기 ( By wxywxy )
	//-----------------------------------------------------------------------------	
	CTail		m_SwordTail;
	void		CalWeaponAxis(void);												// 무기 축 계산

	
	////////////////////////////////////////////////////////////////////////////////////
	// 맞는 모션 처리
	////////////////////////////////////////////////////////////////////////////////////
	bool		ReadyUnderAttackMotion( BOOL IsCritiCal );							// 맞는 모션을 할 수 있는지 판단 (이동중이거나 공격중에는 무시)
	DWORD		SetUnderAttackMotion( DWORD aDamage );				// 데미지에 따라 맞는 모션을 설정

	bool		SM__SLIDE__NPC( EVENT_DATA_INFO& data );			// 밀려나기

	////////////////////////////////////////////////////////////////////////////////////
	// 아이템 착탈
	////////////////////////////////////////////////////////////////////////////////////
	int			Item_Equip( DWORD aItemCode, int aEquipPos );		// 아이템 착용
	int			Item_TakeOff( int aEquipPos );						// 아이템 벗기 

	void		PreCharTextureCreate();								// 아이템 실시간 관리로 없는 텍스쳐일 경우 미리 만들어 놓는다 
	bool		DrawWeaponCheckStatus(bool bActCheck = true);		// 휴식 스킬이나 사교동작일 경우 무기를 그리지 않는다 

	////////////////////////////////////////////////////////////////////////////////////
	// 아이템 착탈
	////////////////////////////////////////////////////////////////////////////////////
	long		theVibrationTime;
	bool		theIsVibration;
	void		SetVibrationPos( D3DMATRIX* aMatWorld );

	////////////////////////////////////////////////////////////////////////////////////
	// NPC 들을 위해서 퀘스트 저장 
	////////////////////////////////////////////////////////////////////////////////////
	std::vector<QUESTDATA> thePreQuestCodeList;	
	QUEST_MARK_TYPE		   m_QuestMarkType;
	
	//QUEST_MARK_TYPE  m_QuestMarkType;								// 퀘스트 마크 타입
	
	
	BOOL	m_ISQuestEffectCreated;									// 퀘스트 생성 했나
	
	bool	theIsDrawWeapon;
	DWORD	theDrawWeaponFadeTime;
	bool	DrawWeaponFade();

	//-----------------------------------------------------------------------------
	// 멀티 공격 메세지 처리 
	//-----------------------------------------------------------------------------
	bool		MultiAttackTargetMgr( EVENT_DATA_INFO& aEvent );


	//.........................................................................................................
	// 킵업 스킬 동기화 관련 데이터 By wxywxy
	//.........................................................................................................
	KEEPUP_SKILL_CHECKER	m_KeepUpChecker;

	//.........................................................................................................
	// NPC 임팩트 프레임 관련 by wxywxy
	//.........................................................................................................
public:
	
	int		m_NPCImpactFrame;						// NPC 임팩트 프레임 
	bool	m_bNotImpact;
	
	//.........................................................................................................
	// 선점 관련 By wxywy
	//.........................................................................................................
public:	
	
	// 선점권자 정보 n_PC, n_Party , 아이디 ( 유니크 )
	void	SetPriority(BYTE thePriorityType,DWORD thePriorityID);	
private:

	BOOL	m_ISPriority;

protected:	
	CState   *m_pCurrentState; 

public:
	DWORD	dDestCell;
	bool	m_IsDefenceBack;		//방어형 펫 - 소유자에게 뒤쪽으로 가기
	bool	m_IsOwnerBack;			//소유자에게 돌아가기
	DWORD	dAttackDestCell;
	int		m_attack_type;
	DWORD   theAttackedID;
	D3DXVECTOR3	vDestPos;

	void	ChangeState(CState * pNewState);
	CState   * GetState()	 {	return m_pCurrentState; }

	int		m_TempIndex;
	SNOWATTACKINFO	m_NowAInfo;	

	BOOL	m_IsAllLTexture;		///-- 텍스쳐가 전부 로드되었는지 여부
	
	///-- 엠블렌 아이템적용부분 리뉴얼
	DWORD	FindFrontierID();
	void	ApplyIItemSlotAndFounded(int idx, BOOL * ISEmblemSlot, BOOL & ISEmblemWearFounded);
	
	void	GetXAndYRadian(float &fXRadian, float &fYRadian, 
									D3DXVECTOR3 vStartPos, D3DXVECTOR3 vDestPos);

};


#endif
