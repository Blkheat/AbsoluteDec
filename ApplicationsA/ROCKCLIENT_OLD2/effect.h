#ifndef		_____BRAVO_PARTICLE_H_____
#define		_____BRAVO_PARTICLE_H_____

#pragma once

#include <vector>		// STL의 벡터를 쓰기 위해서
#include <list>
using namespace	std;		// STL을 쓰겠다고 선언

#include	<EFFECT\\EffectSystem.h>
#include	<EFFECT\\EffectInsect.h>
#include	<EFFECT\\Billboard.h>
#include	<EFFECT\\AnimSequence.h>
#include	<EFFECT\\Tails.h>
#include	<EFFECT\\cloak.h>
#include	"quadlist.h"
#include	<OBJ\\ObjBase.H>
#include	<OBJ\\Object.H>
#include    "bravolibs\effect\CDamageNum.h"
#include    "bravolibs\effect\CWeatherDrop.h"
#include    "bravolibs\effect\CEffectManager.h"				// 이펙트매니저 by wxywxy
#include	"bravolibs\effect\CCombo2DEffect.h"

#include    "bravolibs\effect\CFade.h"

#include <NETWORK\Protocol.h>
#include "..\\quadlist.h"
#include <BASE\\D3DBASE.H>
#include "..\\RockClient.h"
#include "..\\PC.h"
#include "..\\Camera.h"
#include "..\\Map.h"
#include "ObjBase.H"
#include "..\\bravolibs\\obj\\Character_Item.H"
#include "..\\bravolibs\\obj\\Player_Manager.H"
#include "..\\bravolibs\\obj\\Line_Vertex.H"
#include <stdio.h>
#include <assert.H>
#ifdef DIRECT_VERSION_9_MJH
#include "D3dx9math.h"
#else
#include "D3dx8math.h"
#endif // DIRECT_VERSION_9_MJH
#include "bravolibs\effect\CGlow.h"
#include "bravolibs\effect\CArrowDisplay.h"

#include "CIndexList.h"										// 인덱스 리스트 2004.8.24 By wxywxy 
#include "CSetEnv.h"
#include "bravolibs\effect\CEffectTextureManager.h"
#include "bravolibs\effect\CSpotEffect.h"
#include "bravolibs\effect\CExpDisplay.h"


RB_USING
#include "RockResourceManager.h"

#include "bravolibs\effect\CScreenBlur.h"									// 블러 테스트

#define		MAX_PARTICLE_SOURCE				100
#define		MAX_INFO						10

#define		EFFECT_SET_LOOP_TIME			99999.0f

#define SPHERE_RING_COUNT			30
#define SPHERE_SEGMENT_COUNT		30
#define SPHERE_VERTEX_COUNT			( 2 * SPHERE_RING_COUNT * ( SPHERE_SEGMENT_COUNT + 1 ) )

// 이펙트 셋 위치 처리 방식
typedef enum EFFECTSET_LINK_TYPE
{
	ESLT_NONE				=  -1		,		// 이펙트셋 정의 안된거
	ESLT_CHARPOS			=	0		,		// 캐릭터 위치로 처리
	ESLT_CHARWHOLELINK		=   1		,		// 캐릭터 매트릭스에 완전링크
	ESLT_POS				= 100		,       // 저장되있는 위치로 처리
	ESLT_SRC_RAD			= 120       ,       // 소스쪽 방향 + 180도 방향으로 처리해 Boa요
	ESLT_CPOS_ICAMERA       = 130		,		// 캐릭터 위치 카메라 역행렬방향 - 빌보드 형태	
};


enum		BRAVOPARTICLE_TYPE
{
	PARTICLETYPE_RAIN_L,
	PARTICLETYPE_RAIN_BOUND,
	PARTICLETYPE_RAIN_M,
	PARTICLETYPE_RAIN_H,
	PARTICLETYPE_DIRT,
	PARTICLETYPE_SMOKE,
};

struct		EffectObjDelay
{
	bool			Use;
	float			Life;
	int				Index;
	D3DXMATRIX		MatPos;
	D3DXMATRIX		PcUniqMat;
	bool			Child;

	int				m_iPcUniq;
	int				m_iPcIdx;

	int				m_iEffSetCon;

	void			Init()
	{
		Use	= false;
		Life = -1.0f;
		Index = -1;
		D3DXMatrixIdentity(&MatPos);
		D3DXMatrixIdentity(&PcUniqMat);
		Child	= false;
		m_iPcUniq	= -1;
		m_iPcIdx	= -1;
		m_iEffSetCon	= -1;
	}
};


struct		EffectDelay
{
	// 타입
	int				Type;

	//딜레이할 시간.
	float			Life;

	//딜레이할 정보.
	int				Number;
	long			Uniq;
	int				Index;
	int				LinkIndex;
	D3DXVECTOR3		Loc;
	D3DXVECTOR3		vAttackSrcDir;

	Character *		m_pAttackUnit;
	BOOL			m_IsDirAttatack;
	BOOL			m_IsGUnitLinkBoneAndSizeFix;
	BYTE			m_aDirectLinkBone;

	int				m_iEffSetCon;

	void			Init()
	{
		Type		 = -1;
		Life		 = 0.0f;
		Number		 = -1;
		Uniq		 = -1;
		Index		 = -1;
		LinkIndex	 = -1;
		m_iEffSetCon = -1;
		Loc			 = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		vAttackSrcDir  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

		m_pAttackUnit = NULL;
		m_IsDirAttatack = FALSE;
		m_IsGUnitLinkBoneAndSizeFix = FALSE;

		m_aDirectLinkBone = 0;
	}

};


struct		EffectStop
{
	// 타입
	int				Type;
	//딜레이할 시간.
	float			Life;

	//딜레이할 정보.
	int				Number;
	long			Uniq;
	int				Index;
	int				LinkIndex;
	D3DXVECTOR3		Loc;

	int				m_iEffSetCon;

	void			Init()
	{
		Type = -1;
		Life = 0.0f;
		Number = -1;
		Uniq = -1;
		Index	= -1;
		LinkIndex	= -1;
		Loc = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		m_iEffSetCon	= -1;
	}
};



class	CEffSetControl
{
public:
	int			m_iSetType;


public:

	float		m_fDelayTime;

	int			m_iType;

	float		m_fLife;		// 0.7f이 맥스

	long		m_lTargetUniq;
	int			m_iTargetIndex;

	D3DXVECTOR3	m_vPos;
	D3DXVECTOR3	m_vVec;

	float		m_fRad;

	bool		m_bMake;
	int			m_iChildMake;

	int			m_iTailNum;
	int			m_iTailType;

	float		m_fThetaHoriz;
	float		m_fThetaVert;
	
	int			m_nHeight;
	D3DXVECTOR3	m_vDir;

	void		Init();
	
	~CEffSetControl();

	Character		*m_pSrc;
	Character		*m_pAttackUnit;		//공격 유닛
	BOOL			 m_IsDirAttatack;
	BOOL			 m_IsGUnitLinkBoneAndSizeFix;
	BYTE			 m_aDirectLinkBone;
	float			 m_fAddSize;

	e_SCENE_MODE	 m_SceneMode;
	bool			m_ConSkill;			//지속성 스킬인지 여부
};

//-----------------------------------------------------------------------------
// Desc: 파티클 정보
//-----------------------------------------------------------------------------
struct ParInfo
{
	int				index;		// 파티클 인덱스
	float			delayTime;	// 딜레이 타임
	D3DXVECTOR3		vPos;		// 파티클 위치
	BYTE			BoneLink;	// 본 링크 번호

	ParInfo()
	{
		index		= -1;
		delayTime	= 0.0f;
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		BoneLink = 0;
	}

	void Init()
	{
		index		= -1;
		delayTime	= 0.0f;
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		BoneLink = 0;
	}
};
//-----------------------------------------------------------------------------
// Desc: 빌보드 정보
//-----------------------------------------------------------------------------
struct BillInfo
{
	int				index;		// 빌보드 인덱스
	float			delayTime;	// 딜레이 타임
	D3DXVECTOR3		vPos;		// 빌보드 위치
	BYTE			BoneLink;	// 본 링크 번호

	BillInfo()
	{
		index		= -1;
		delayTime	= 0.0f;
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		BoneLink = 0;
	}

	void Init()
	{
		index		= -1;
		delayTime	= 0.0f;
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		BoneLink = 0;
	}
};
//-----------------------------------------------------------------------------
// Desc: 오브젝트 정보
//-----------------------------------------------------------------------------
struct ObjInfo
{
	int				index;		// 빌보드 인덱스
	float			delayTime;	// 딜레이 타임
	D3DXVECTOR3		vPos;		// 빌보드 위치
	BYTE			BoneLink;	// 본 링크 번호

	void Init()
	{
		index		= -1;
		delayTime	= 0.0f;
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		BoneLink = 0;
	}
};
//-----------------------------------------------------------------------------
// Desc: 엔피씨 파티클 정보
//-----------------------------------------------------------------------------
struct	npceffect
{
	int				index;

	int				righthand;
	int				lefthand;
	int				neck;

	int				skill1;
	int				skill2;

	void	Init()
	{
		index		= -1;

		righthand	= -1;
		lefthand	= -1;
		neck		= -1;

		skill1		= -1;
		skill2		= -1;
	}
};

class		CEffectSet
{
public:
	int					m_iMyType;
	int					m_iDrawParCount;
	int					m_iDrawBillCount;
	int					m_iDrawObjCount;
	ParInfo				m_ParticleInfo[ MAX_INFO ];
	BillInfo			m_BillboardInfo[ MAX_INFO ];
	ObjInfo				m_ObjInfo[ MAX_INFO ];

	void				LoadEffectSet(char *FileName);

	~CEffectSet();
};

class		CBravoParticle
{
public:

	Character			*m_pAttackUnit;						// 공격유닛
	BOOL				m_IsDirAttatack;					// 방향성 공격여부
	BOOL				m_IsGUnitLinkBoneAndSizeFix;		// 거대유닛 링크본과 사이즈 고정여부
	BYTE				m_aDirectLinkBone;					
	float				m_fAddSize;							// 크기 설정		

	CTailsManager		m_pTails;					// 이펙트 꼬리 부분 By wxywxy

	bool				m_bEffectDisplay;

public:
	CEffSetControl		m_pEffSetCon[512];
	
	void				EffSetConAllInit(BOOL ISEnd = FALSE);
	void				SetEffSetCon( float delaytime, int settype, int type, float life, 
									  long tuniq, int tidx, D3DXVECTOR3 pos= D3DXVECTOR3( 0.0f, 0.0f, 0.0f), 
									  float rad = 0.0f, int tail = -1, int nHeight = 0 );									 	
	
	void				DelEffSetCon( int settype, int type, long tuniq, int tidx, BOOL IsNowDel = FALSE, BOOL IsAllDelete = TRUE );
	void				DelEffSetCon( long tuniq, int tidx );
	void				DelEffSetCon_All( int settype, int type );
	void				EffSetConProcess( float fTimeDelta );
	
public:
	
	int					m_iEffectSet_Data_Count;
	
public:
	
	int					m_iParticle_Data_Count;
	CCombo2DEffect		m_Combo2DEffect;
	CGlow				m_Glow;
	int					m_GlowAlpha;
	void				Glow(void);
	
	CScreenBlur			m_ScreenBlur;						// 블러 테스트
	BOOL				m_IsRenderScreenBlur;

	void				ScreenBlur();

public:
	
	D3DXMATRIX			m_matIdentity;
	D3DXMATRIX			m_matInverseCamera;

// kstar79
public:
	Model		m_Mod[128];
	Texture		m_Tex[128];

	int			m_effectobjcount;
	Object		m_EffectObj[128];

	void		CreateEffectObj( char* file_pos );
	void		DisplayEffectObj( float TimeD );
	void		AddEffectObj( int index, int tuniq, int tidx, D3DXVECTOR3 pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
								float rad = 0.0f, int effsetcon = -1 );
	
	void		DelEffectObj( int index, int child );

	EffectDelay		EffectDelay[128];
	void			AddEffectDelay( int type, float life, int number, long uniq,
									int index, int linkindex, D3DXVECTOR3& loc, int effsetcon = -1 );
	void			DelEffectDelay( int num );
	void			EffectDelayProcess( float time );
	void			DelEffSetCon( int SetNum );

	EffectStop		EffectStop[128];
	void			AddEffectStop( int type, float life, int number, long uniq,
									int index, int linkindex, D3DXVECTOR3& loc, int effsetcon = -1 );
	void			DelEffectStop( int num );
	void			EffectStopProcess( float time );

	EffectObjDelay	EffectObjDelay[128];
	void			AddEffectObjDelay( float life, int index, D3DXMATRIX mat, D3DXMATRIX pcuniqmat,
										bool child, int pcuniq = -1, int pcindex = -1, int effsetcon = -1 );
	void			AddEffectObjDelay( float life, int index, int tuniq, int tidx, D3DXVECTOR3 pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
										float rad = 0.0f, int effsetcon = -1 );
	void			DelEffectObjDelay( int num );
	void			EffectObjDelayProcess( float time );

	void			Setup_EffectSet(int i);							// 이펙트 셋 초기 설정 By wxywxy
	void			Process_EffectSet(int i,float fTimeDelta);		// 이펙트 처리		   By wxywxy
	void			Stop_EffectSet(int i);							// 이펙트 종료		   By wxywxy
	
	Character		*GetCharacter(long Unique,int nIndex);
	

public:
	
	CEffectTextureManager		m_EffectTextureManager;
	CExpDisplay					m_ExpDisplay;

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pDotVB;
	LPDIRECT3DVERTEXBUFFER9		m_pColorVB;
	LPDIRECT3DVERTEXBUFFER9		m_pSpaceVB;
	LPDIRECT3DINDEXBUFFER9		m_pSpaceIB;

	LPDIRECT3DVERTEXBUFFER9		m_pParticleVB;
	LPDIRECT3DINDEXBUFFER9		m_pParticleIB;
#else
	LPDIRECT3DVERTEXBUFFER8		m_pDotVB;
	LPDIRECT3DVERTEXBUFFER8		m_pColorVB;
	LPDIRECT3DVERTEXBUFFER8		m_pSpaceVB;
	LPDIRECT3DINDEXBUFFER8		m_pSpaceIB;

	LPDIRECT3DVERTEXBUFFER8		m_pParticleVB;
	LPDIRECT3DINDEXBUFFER8		m_pParticleIB;
#endif // DIRECT_VERSION_9_MJH
	int							m_ParticleLockMaxCount;
	VERTEX_XYZ_DIFFUSE_TEX1		*m_pParticleMemVB;
	int							m_ParticleLockLimitCount;


	float		m_fTimeD;
	long		m_lPrevTime;
	bool		m_bDisplay;
	
	CDamageNum			m_DamageNum;				// 데미지 정도 표시 By wxywxy
	CEffectManager      m_EffectManager;			// 이펙트 매니저
	
	CInsect_Manager		m_IstManager;

	
	//...............................................................................................................
	// 파티클 처리 부분
	//...............................................................................................................
	std::list < CParticleSystem * >	m_ParticleSystemList;

	CRockResourceManager < CParticleSystem >  m_ParticleSystemPool;	

	//...............................................................................................................

	CArrowDisplay       m_ArrowDisplay;

	CFade				m_Fade;
	CSpotEffect			m_SpotEffect;

	D3DXMATRIX		m_MovingPos;
	HRESULT			Create(char* dir);

	//해당 파티클을 찾기 위한 함수
	HRESULT			Display();

	HRESULT			RestoreDeviceObjects();
	void			UpdateManager( FLOAT fTimeDelta );
	void			UpdateManager( CParticleSystem* pParticleSystem, FLOAT fTimeDelta );
	void			AddParticleSystem(CParticleSystem* pParticleSystem);

	void			DisplayUpdateNotApplyDis( int number, D3DXVECTOR3& v );
	void			DisplayUpdate( int number , long Uniq );				// type 1	
	void			DisplayUpdate( int number, D3DXVECTOR3& v, int effsetcon = -1, int fSize = 1.0f );				// type 2
	void			DisplayUpdate( int number, long Uniq, D3DXVECTOR3& v );	// type 3
	void			DisplayUpdate( int number, long Uniq, int LinkLoc );	// type 4	( LinkLoc : 0 오른손 1 왼손 )

	void			DisplayUpdate( int number, long Uniq, int index, int LinkLoc, D3DXVECTOR3& v, bool bFlag, 
								   int effsetcon = -1, D3DXVECTOR3 vAttackSrcDir = D3DXVECTOR3(0.0f,0.0f,0.0f), Character *	pAttackUnit = NULL,
								   BOOL	IsDirAttatack = FALSE, BOOL IsGUnitLinkBoneAndSizeFix = FALSE,
								   BYTE aDirectLinkBone = 0 );  // type 5
	void			DisplayUpdate( int number, long Uniq, int index, int linkLoc, bool link );
	void		    DisplayUpdate( int number, long Uniq, int index, int linkLoc, D3DXVECTOR3 v, int effsetcon,
								   D3DXVECTOR3 vAttackSrcDir = D3DXVECTOR3(0.0f,0.0f,0.0f), Character *	pAttackUnit = NULL,
								   BOOL	IsDirAttatack = FALSE, BOOL IsGUnitLinkBoneAndSizeFix = FALSE, 
								   BYTE aDirectLinkBone = 0 );	// type 6 ( 업데이트시 각도 , 위치 정해저있는것들 )

	void			StopDisplay( int number );
	void			StopDisplay( int number, long Uniq );
	void			StopDisplay( int number, D3DXVECTOR3& loc, int effsetcon = -1, BOOL IsNowDel = FALSE );
	void			StopDisplay( int number, long Uniq, int LinkLoc );
	void			StopDisplay( int number, long Uniq, int index, int LinkLoc, int effsetcon = -1, BOOL IsNowDel = FALSE );
	void			StopDisplay( long Uniq, int index );

	void			DeleteAll(BOOL ISEnd = FALSE);

	void			RainControl( float time );

	CWeatherDrop	m_WeatherDrop;


	//벡터를 방향으로 변환 시켜 주는 함수. 
	float			VectorToDir(D3DXVECTOR3& v);

	float			Distance3D( D3DXVECTOR3 &v ){ return (float)sqrt((double)(v.x*v.x + v.y*v.y + v.z*v.z)); }

	int				m_iAtFirst;

public:

	vector <npceffect> m_NpcEffectList;
	
	bool			Read_Npc_Effect( const char* file_pos );
	void			Make_Npc_Effect( int index, long unique );

public:
	void			AddDropItemEffect( int index, long pos );

	CBravoParticle();
	~CBravoParticle();


// 이펙트 로딩관리 ( By wxywxy )
private:

	int				LoadPaticle(char *pPath);						// 파티클 로딩 관련(.nb)
	int				LoadEffectSet(void);							// 이펙트 셋 로딩 관련

public:

	CIndexList<CParticleData>		m_ParticleData;	// 파티클 데이터 리스트 2004.8.24 By wxywxy(STL map이라고 생각하면 됨)
	CIndexList<CEffectSet>			m_EffectSet;	// 이펙트 셋 데이터 리스트


//................................................................................................................
// 카메라 흔들림 관련 ( by wxywxy )
//................................................................................................................
public:

	 void   RockCamera(DWORD Timer);
	 void	RockCheck(void);

private:

	DWORD	m_CameraOldTimer;
	DWORD   m_CameraRockTimer;
		
	BOOL	m_ISCameraRock;

	FLOAT	m_fOldCameraYaw;
	FLOAT	m_fOldCameraPitch;

};

extern	CBravoParticle	g_Particle;

// Particle system types
enum ParticleSystemType
{
	PS_Manual		= 0,
	PS_Hit
};

////////////////////////////////////////////////////////////////////////////
class		CBillboardManager
{
public:
	int							m_iBillboard_Data_Count;

public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pVB[eBDType_Count];
	LPDIRECT3DINDEXBUFFER9		m_pIB[eBDType_Count];
#else
	LPDIRECT3DVERTEXBUFFER8		m_pVB[eBDType_Count];
	LPDIRECT3DINDEXBUFFER8		m_pIB[eBDType_Count];
#endif // DIRECT_VERSION_9_MJH

	WORD**						m_pIndices;
	VERTEX_XYZ_DIFFUSE_TEX1**	m_pVertices;
	VERTEX_XYZ_DIFFUSE_TEX1**	m_pDefaultVertices;
	VERTEX_XYZ_DIFFUSE_TEX1**	m_pDefaultCopyVertices;


	int							m_pIndicesCount[eBDType_Count];
	int							m_pVerticesCount[eBDType_Count];

	CIndexList<CBillboardData>  m_BillboardData;			// 빌보드 리스트 읽기 2004.8.24 By wxywxy 
	list< CBillboard >			m_pBillboard;

	char m_strDir[256];

public:
	HRESULT		InitManager(char* dir, BOOL bRestore);

	// 엔피씨 용 
	void		SetManager( float delaytime, int type, long uniq, int index, int effsetcon = -1 );
	// 오브젝트 용 
	void		SetManager( float delaytime, int type, D3DXVECTOR3& loc, int effsetcon = -1, int nHeight = 0 );
	// 오브젝트 용 ( 기본 위치값 설정해야지 )
	void		SetManager( float delaytime, int type, long uniq, int index,
							D3DXVECTOR3 &loc,int Link = 0,int effsetcon = -1  );

	void		DisplayManager( float timed );

	void		DelManager( int type, long uniq, int index, int effsetcon = -1 );
	void		DelManager( int type, D3DXVECTOR3& loc, int effsetcon = -1 );
	void		DelManager( long uniq, int index );

	void		DelAllManager(BOOL ISEnd = FALSE);

	void		DisplayManager( CBillboard &Billboard,float timed );

	
	CBillboardManager();
	~CBillboardManager();

	int			LoadBillboard(char *pPath);// 빌보드 데이터 리스트 읽기 2004.8.24 By wxywxy(.eb)
	int			LoadExtraModel(char *pPath);

	//By simwoosung
	HRESULT		InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT		RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

//................................................................................................................
// 빌보드 모양 맹글어 Boa 요
//................................................................................................................
public:

	// 0. 실드
	int				MakeShield(void);
	// 1. 세로 방향 면
	int				MakeVerticalPlane(void);
	// 2. 가로 방향 면
	int				MakeHorizonPlane(void);
	// 3. 세로 방향 직사각형
	int				MakeVerticalLongPlane(void);
	// 4. 가로 방향 직사각형
	int				MakeHorizonLongPlane(void);
	// 5. 실린더
	int				MakeCylinder(void);
	// 6. 실린더 위가 큰거
	int				MakeCylinderUp(void);
	// 7. 반구	
	int				MakeHemisphere(void);
	// 8. 실린더 아래가 큰거
	int				MakeCylinderDown(void);
	// 9. 구
	int				MakeSphere(void);
	// 10. 도넛 ( UV 개별적 )
	int				MakeDonut(void);
	// 11. 도넛 ( UV 전체 )
	int				MakeDonutUVOne(void);
	// 12. 스팟 
	int				MakeSpot(void);
	// 13. 스팟 ( 뒤로 굽은거 )
	int				MakeSpot1(void);
	// 14. 스팟 ( 뒤로 굽은거 )
	int             MakeSpot2(void);
	// 15. 칼흔
	int				MakeSword(void);
	// 16. 크로스 평면
	int             MakeCrossPlane(void);
	// 17. 실린더 각진거 6각
	int				MakeCylinder6(void);
	// 18. 실린더 각진거 8각
	int				MakeCylinder8(void);

	CIndexList	<Model*>		m_pExtraModelList;

	void			SetDefaultVectex();
	VERTEX_XYZ_DIFFUSE_TEX1*	GetDefaultVectexByType(int iType);

};

extern	CBillboardManager	g_Billboard;


class		CAnimManager
{
public:

	CIndexList<CAnimSequence *> m_AniData;
	
	HRESULT		InitManager(char* dir);

public:
	
	 CAnimManager() {};
	~CAnimManager() 
	{
		m_AniData.ClearPointer();
	};
};

extern	CAnimManager	g_Anim;

#endif
