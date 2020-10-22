       #ifndef			_____FIELD_H_____
#define			_____FIELD_H_____

#include	<OBJ\\Object.h>
#include	"FileInfo.h"
#include	<OBJ\\NonPlayer.h>
#include	"GrassNormal.h"
#include	"planewatereffect.h"
#include	"water.h"
#include	"waterfall.h"
#include	"effect\\effectsystem.h"

#define		MAX_WATERFALL	1
#define		MAX_PARTICLE	1024

#pragma warning(disable:4786)
#include	<list>
#include	<map>
#include	<vector>		// STL의 벡터를 쓰기 위해서
using namespace	std;		// STL을 쓰겠다고 선언

#define		MAX_NPC_INTABLE		256
#define		MAX_OBJ				128
#define		MAX_BILOBJ			16

#define		MAX_GRASS			1024
#define		TILE_SIZE			16

#define		D3DFVF_PNTD		(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2)
#define		D3DFVF_PT		(D3DFVF_XYZ|D3DFVF_TEX1)

#define		DISPLAY_RANGE	18	/// by sooree 디테일 타일링 범위 ( 범위 + 2 )

enum	MAPSTATS
{
	MS_FIELD	= 0,
	MS_INDOOR,
};


//-----------------------------------------------------------------------------
// Desc: 칼라저장
//
// Copyright (c) 2001.11.28 By KStaR79
//-----------------------------------------------------------------------------
struct	Color_Table
{
	int		r, g, b;	
};

//-----------------------------------------------------------------------------
// Desc: 광원타일 테이블
//
// Copyright (c) 2001.11.28 By KStaR79
//-----------------------------------------------------------------------------
#define	LIGHTTALBE_COUNT	64

struct	LIGHTTILE_TABLE
{
	int			Display;	// 그리니? 0안그린다 1그린다(회전X) 2그린다(회전0)	
	D3DXMATRIX	matPos;		// 현재위치값	
	float		fRot;		// 회전값
	int			PreTime;	// 시간
	int			Index;		// -1:PC
	long		lUnique;	// 유니크
		
	LIGHTTILE_TABLE()	{ Clear(); }
	void	Clear()		{ Display = -2; Index = -1; lUnique = -1; }
};


//-----------------------------------------------------------------------------
// Desc: 오브젝트 소팅 테이블
//
// 오브젝트 소팅시 사용되는 정보값을 저장하는 구조체
//
// Copyright (c) 2001.11.28 By KStaR79
//-----------------------------------------------------------------------------
#define	SAMEOBJ_COUNT	512

struct	OBJ_SORT_TABLE
{
	DWORD	Index;	
	DWORD	Child;
	//float	x;
	//float	z;
};

struct	OBJ_INFO_TABLE
{
	int		Index;	
	int		ChildCount;
	int		Same[SAMEOBJ_COUNT];

	OBJ_INFO_TABLE()	
	{
		Index  = -1;		
		ChildCount  = 0;		
		memset( &Same, -1, sizeof(Same) );
	}
};

/*
//-----------------------------------------------------------------------------
// Desc: 말 테이블
//
// 1. 말(스티드)에 대한 정보값을 저장하는 구조체
// 2. NPCTABLE과 중복되어서 사용되며, NPCTABLE에서 해당 말테이블의 인덱스를 저장하고 있다
//
// Copyright (c) 2001.11.28 By KStaR79
//-----------------------------------------------------------------------------
struct		HORSE_TABLE
{
	char	szName[10];		// 이름(한글5자)
	bool	bMale;			// 수컷, 암컷
	BYTE	byLevel;		// 레벨
};
*/


//-----------------------------------------------------------------------------
// Desc: NPC(몬스터, 상인등) 테이블
//
// Copyright (c) 2001.11.28 By KStaR79
//-----------------------------------------------------------------------------
// 상태
enum NPC_STATUS
{
	WAIT = 0,			// 대기		0
	MOVE,				// 이동중	1
	FIGHT,				// 싸움중	2
	DEATH,				// 뒤졌네	3
	RUN,				// 도망중	4
	POISON,				// 중독		5
	STUN,				// 스턴		6
};

///-- By simwoosung
///-- NPC가 플레이어 리소스를 사용하기 위해 필요한 추가 테이블
struct	 NPC_EX_PCTABLE
{
	BYTE				theCitizen;									// RAIN, MILLENA
	BYTE				theGender;									// MALE, FEMALE
	DWORD				theFace;
	DWORD				theHair;
	BYTE				theMainClass;								// 메인 클래스

	WORD				theWearItem[RP_MAX_EQUIP];					// 장신구1, 2 제외
};

struct		NPCTABLE
{
	int				code;										// g_Pc_Manager.theNPCInfoTable.theCode 
	int				index;										// g_Pc_Manager.m_NpcFileInfo[index]
	long			lUnique;									// 유니크
	long			lBlock;										// 이전타일
	long			lDestBlock;									// 현재타일
	BYTE			nStatus;									// 상태
	int				race;										// -1 몹, -2 라이브아이템, -3 말
//	BYTE			nCurrentFloor;								// 층 
//	DWORD			type;										// 타입(색깔등)
	BYTE			theLevel;									// 레벨 
	char			szName[31];									// 이름
	DWORD			theHP;										// 체력 
	DWORD			theSMP;										// 스테미너 
	DWORD			theMaxHP;									// 최대 체력치 
	DWORD			theRealHP;									// 서버에서 받는 인터페이스 적용전 실제 HP
	WORD			theKeepupSkill[RP_MAX_KEEPUP_SKILL];		// 지속성 스킬 코드
	DWORD			theFrontierID;
	char			theFrontierName[RP_MAX_FRONTIER_NAME];
	int				theNpcSize;
	int				theHolyLevel;
	

	NPC_EX_PCTABLE	theExPcInfo;
	
	void			Init()
	{
		code		= -1;
		index		= -1;
		lUnique		= -1;
		lBlock		= -1;
		lDestBlock	= -1;
		nStatus		= 0;
		race		= c_NpcRace_Mob;
//		type		= 0;
//		nCurrentFloor = 0;	
		theLevel	= 0;
		memset( &szName, 0, sizeof(szName) );
		theHP		= 0;
		theSMP		= 0;
		theMaxHP	= 0;
		theRealHP	= 0;
		memset( &theKeepupSkill, 0, sizeof(theKeepupSkill) );
		theFrontierID = 0;
		memset( &theFrontierName, 0, sizeof(theFrontierName) );

		theNpcSize = 1;

		memset( &theExPcInfo, 0, sizeof(NPC_EX_PCTABLE) );
		theHolyLevel = 0 ; 
	}
	NPCTABLE()
	{
		Init();
	}
};


//-----------------------------------------------------------------------------
// 워프속성
//-----------------------------------------------------------------------------
struct		WarpInfo
{
	int		nDestWarpIdx;
	int		nDestMapID;
	WarpInfo()
	{
		nDestWarpIdx	= -1;
		nDestMapID		= -1;
	}
};

enum		MAP_FLOOR
{
	MF_1	= 0,
	MF_2,
	MF_3,
};

enum			TEX_IDX
{
	TIDX_LEFT	= 0,
	TIDX_TOP,
	TIDX_RIGHT,
	TIDX_BOTTOM,
	TIDX_TOPLEFT,
	TIDX_TOPRIGHT,
	TIDX_BOTLEFT,
	TIDX_BOTRIGHT,
	TIDX_INV_TOPLEFT,
	TIDX_INV_TOPRIGHT,
	TIDX_INV_BOTLEFT,
	TIDX_INV_BOTRIGHT,
	TIDX_EMPTY,
	TIDX_CENTER,
	TIDX_SETTED,
};

enum			OBJROT
{
	OR_X	= 0,
	OR_Y,
	OR_Z,
};

/*
	* GEOM EDIT VOLUME
*/
enum			GEOMEDIT_VOLUME
{
	GEV_PLANEONVOLUME	= 0,
	GEV_PLANE,
	GEV_VOLUME,
};

enum			GEOMEDIT_DIR
{
	GED_UP	= 0,
	GED_DOWN,
};

/*
	* Terrain Vertex Info
*/
struct			TERVTX
{
	float		x, y, z;
	float		tu, tv;
//	float		tu2, tv2;
};

#define	D3DFVF_TERVTX		(D3DFVF_XYZ|D3DFVF_TEX1)

/*
	* Attr Vertex
*/
struct			ATTRVTX
{
	float		x, y, z;
	float		tu, tv;
};
/*
	* 3D Field Info
*/
typedef struct _MAPFACENORMAL
{
	float normal[3];
}MAPFACENORMAL;


struct	TileInfo
{
	bool			bIsVisible;
	WORD			nTexIndex;
	short			vidx;
	unsigned char	bySelTex;
	TERVTX			Pos[4];
};


/*
	* Attr Info
*/


enum			MAPATTRTYPE
{
	MATYPE_LAND	= 0,				// 흙
	MATYPE_GRASS,					// 풀
	MATYPE_WATER,					// 물
	MATYPE_STONE,					// 돌
	MATYPE_WOOD,					// 나무
	MATYPE_SNOW,					// 눈
	MATYPE_SAND						// 모래
};

enum			MAPATTR
{
	MATTR_NONE	= 0,
	MATTR_OBJ,
	MATTR_WARP,
	MATTR_2F,
	MATTR_WARP2,
	MATTR_WARP3,
	MATTR_WARP4,
	MATTR_WARP5,
	MATTR_WARP6,
	MATTR_WARP7,
	MATTR_WARP8,
	MATTR_WARP9,
	MATTR_PC,
	MATTR_NPC,
	MATTR_ITEM,
};

enum			MAPTYPE
{
	MTYPE_LAND	= 0,
	MTYPE_WATER,
	MTYPE_MOUNTAIN,
};



enum			ATTRTYPE
{
	AT_GRASS	= 0,
	AT_SOIL,
	AT_CLAY,
	AT_STONE
};

struct			AttrMap
{
//	DWORD		dwAttr;
//	DWORD		dwType;
//	float		fHeight;
	BYTE		byAttr;
	BYTE		byType;
	short		nHeight;	

	AttrMap()
	{
	   memset( this, 0, sizeof(AttrMap) );
	}
};

/*
	* Attr Display Info
*/
struct			AttrDisp
{
	bool		bIsVisible;
	DWORD		dwTexIdx;
	ATTRVTX		Pos[4];
};

enum		WATER_IDX
{
	WIDX_LEFT	= 0,
	WIDX_TOP,
	WIDX_RIGHT,
	WIDX_BOTTOM,
	WIDX_TOPLEFT,
	WIDX_TOPRIGHT,
	WIDX_BOTLEFT,
	WIDX_BOTRIGHT,
	WIDX_INV_TOPLEFT,
	WIDX_INV_TOPRIGHT,
	WIDX_INV_BOTLEFT,
	WIDX_INV_BOTRIGHT,
	WIDX_CENTER,
	WIDX_EMPTY,
};

struct	FVF_BRAVOCLOUD
{
	float	x, y, z;
	DWORD	color;
	float	u, v;
};


enum	RUNSDIR
{
	RD_LEFTUP	= 0,
	RD_UP,
	RD_RIGHTUP,
	RD_LEFT,
	RD_NONE,
	RD_RIGHT,
	RD_LEFTDOWN,
	RD_DOWN,
	RD_RIGHTDOWN,
};


enum	BRAVOSKY_ORDER
{
	BSKY_FRONT	= 0,
	BSKY_RIGHT,
	BSKY_REAR,
	BSKY_LEFT,
	BSKY_BOTTOM,
	BSKY_TOP
};

#ifdef APPLYWIDE_MAP
	#define ATTR_MAPSIZE 1024
#else
	#define ATTR_MAPSIZE 512
#endif

class	CBravoSky
{
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
#else
	LPDIRECT3DVERTEXBUFFER8	m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
#endif // DIRECT_VERSION_9_MJH
	D3DXMATRIX				m_matWorld;

	short					m_nSize;
	short					m_nHighest;
	short					m_nLowest;
	short					m_nVtxCount;
	short					m_nIdxCount;

	void	FinalCleanup();
#ifdef DIRECT_VERSION_9_MJH
	bool	Create(LPDIRECT3DDEVICE9 pDevice, const short& size);
#else
	bool	Create(LPDIRECT3DDEVICE8 pDevice, const short& size);
#endif // DIRECT_VERSION_9_MJH
	bool	Update();
	bool	UpdateColor();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT	Display(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT	Display(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	CBravoSky();
	~CBravoSky();
};

class	CBravoCloud
{
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
#else
	LPDIRECT3DVERTEXBUFFER8	m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
#endif // DIRECT_VERSION_9_MJH
	D3DXMATRIX				m_matWorld;

	void	FinalCleanup();
#ifdef DIRECT_VERSION_9_MJH
	bool	Create(LPDIRECT3DDEVICE9 pDevice, const short& size);
#else
	bool	Create(LPDIRECT3DDEVICE8 pDevice, const short& size);
#endif // DIRECT_VERSION_9_MJH
	bool	Update();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT	Display(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT	Display(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	short	m_nCX, m_nCY;

	short	m_nVtxCount;
	short	m_nIdxCount;

	short	m_nBaseHeight;
	short	m_nCloudCount;
	short	m_nCloudSize;
	short	m_nRadius;
	BYTE	m_byDepth;

	CBravoCloud();
	~CBravoCloud();
};

/*
	* Map Info
*/
class			CField
{
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9			m_pTextureWater;
	LPDIRECT3DTEXTURE9			m_pTextureWaterfall;
	LPDIRECT3DTEXTURE9			m_pTextureMulti;
	LPDIRECT3DTEXTURE9			m_pTextureFlow;
	LPDIRECT3DTEXTURE9			m_pTexture;
#else
	LPDIRECT3DTEXTURE8			m_pTextureWater;
	LPDIRECT3DTEXTURE8			m_pTextureWaterfall;
	LPDIRECT3DTEXTURE8			m_pTextureMulti;
	LPDIRECT3DTEXTURE8			m_pTextureFlow;
	LPDIRECT3DTEXTURE8			m_pTexture;
#endif // DIRECT_VERSION_9_MJH


protected:
#ifdef DIRECT_VERSION_9_MJH
	/* Attr Texture */
	LPDIRECT3DTEXTURE9			m_pAttrTex;
	/* Terrain */
	LPDIRECT3DDEVICE9			m_pDevice;
	LPDIRECT3DVERTEXBUFFER9		m_pVBuffer;
	LPDIRECT3DINDEXBUFFER9		m_pIBuffer;
#else
	/* Attr Texture */
	LPDIRECT3DTEXTURE8			m_pAttrTex;
	/* Terrain */
	LPDIRECT3DDEVICE8			m_pDevice;
	LPDIRECT3DVERTEXBUFFER8		m_pVBuffer;
	LPDIRECT3DINDEXBUFFER8		m_pIBuffer;
#endif // DIRECT_VERSION_9_MJH
	TERVTX *					m_pVtxList;
	WORD *						m_pIdxList;
	D3DXMATRIX					m_matWorld;
	DWORD						m_dwVtxCount;
	DWORD						m_dwIdxCount;
	/* Attr Map */
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pVBufferAttr;
	LPDIRECT3DINDEXBUFFER9		m_pIBufferAttr;
#else
	LPDIRECT3DVERTEXBUFFER8		m_pVBufferAttr;
	LPDIRECT3DINDEXBUFFER8		m_pIBufferAttr;
#endif // DIRECT_VERSION_9_MJH
	ATTRVTX *					m_pVtxListAttr;
	WORD *						m_pIdxListAttr;
	D3DXMATRIX					m_matWorldAttr;
	DWORD						m_dwVtxCountAttr;
	DWORD						m_dwIdxCountAttr;

public:
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9				m_Material;
	D3DMATERIAL9				m_GridMaterial;
	D3DMATERIAL9				m_AttrMaterial;
	// adding grass material - 2003/04/16
	D3DMATERIAL9				m_MaterialGrass;
	D3DLIGHT9					m_Light;
#else
	D3DMATERIAL8				m_Material;
	D3DMATERIAL8				m_GridMaterial;
	D3DMATERIAL8				m_AttrMaterial;
	// adding grass material - 2003/04/16
	D3DMATERIAL8				m_MaterialGrass;
	D3DLIGHT8					m_Light;
#endif // DIRECT_VERSION_9_MJH

	BYTE						m_byMapType;
	// npcs in indoor
	D3DXMATRIX					m_matNpcPos[32];

	///-- By simwoosung
	list <Character * > m_GiantUnitList;	///-- 거대유닛, 몬스터, 빌딩리스트  
	list <Character * > m_GiantBilbList;  ///-- 거대빌딩 리스트

/**************************************************
	* MAJOR
**************************************************/
protected:
	WarpInfo	m_WarpInfo[9];

public:
	void SetupWarpInfo(DWORD index, int destid, int destidx)
	{
		m_WarpInfo[index].nDestWarpIdx	= destidx;
		m_WarpInfo[index].nDestMapID	= destid;
	}

	WarpInfo	GetWarpInfo(DWORD idx)	{ return m_WarpInfo[idx]; };

/**************************************************
	* MAJOR
**************************************************/
protected:
public:
	DWORD		m_cSize;
	//TileInfo **	m_pDisplayMap;
	
#ifdef APPLYWIDE_MAP	
	TileInfo		m_pDisplayMap[512][512];
#else
	TileInfo		m_pDisplayMap[256][256];
#endif
	/* UPDATE RANGE CHECK */
public:
	//DWORD		sx, sy, ex, ey;
	short		sx, sy, ex, ey;
	DWORD		m_dwHighestFloor;
	
	///마우스 커서 클라이언트 좌표
	int		thescreenX;
	int		theScreenY;

public:
#ifdef DIRECT_VERSION_9_MJH
	virtual	bool		Create(LPDIRECT3DDEVICE9 pDevice);
#else
	virtual	bool		Create(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	virtual	bool		Init();
	virtual	void		Cleanup();
	virtual	void		FinalCleanup();
	
	void  CleanObjParticleAliveInfo();

public:
	/// 맵툴의 Load() 메쏘드와 같음....
	bool		LoadTerrainNew(short* pHeightList, WORD* pTexIdxList, COLOR_RGB* pColorList);
	///
	bool		BuildTerrain();

	virtual	bool		UpdateTerrain(DWORD cx, DWORD cy);
	///
	virtual	HRESULT		Display();



//	void		DisplayShadowTile(const short& px, const short& py);
	void 		DisplayShadowTile( const short& px, const short& py, int aBlockSize );
	void		DisplayShadowTile2(const short& px, const short& py);
	void		DisplayShadowTile(const D3DXMATRIX& mat, const D3DXVECTOR3& min, const D3DXVECTOR3& max);
	void		DisplayOnTile(float px,float pz,float xsize,float zsize);
	void		DisplayOnTile2(float px,float pz,float xsize,float zsize);

	LIGHTTILE_TABLE	m_LightTable[LIGHTTALBE_COUNT];	
	void		ClearLightTile( int Index, long lUnique );
	void		SetLightTile( D3DXMATRIX mat, int btDisplay = 1, int Index = -1, long lUnique = -1 );
	void		UpdateLightTile();
	void		DisplayLightTile(const D3DXMATRIX& mat, int type = 0, float fRot = 0.0f );	

	BOOL		IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v);
	BOOL		IntersectTriangle2(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v, BOOL* fb );

public:
	DWORD		GetSize()					{ return m_cSize; }
	void		SetupSize(DWORD size)		{ m_cSize = size; }

/**************************************************
	* ATTR
**************************************************/
public:
	AttrMap		m_pAttrMap[ATTR_MAPSIZE][ATTR_MAPSIZE];

protected:
	AttrDisp **	m_pAttrDisp;
	DWORD		m_dwCurAttr;
	DWORD		m_dwEditableFloor;

// - Attr 2nd Floor
public:
	DWORD		m_dwCurFloor;

public:
	void		CalculateAttrHeight();

//	DWORD		GetEditableFloor()					{ return m_dwEditableFloor; };
//	void		SetupEditableFloor(MAP_FLOOR mf)	{ m_dwEditableFloor = mf; };

//	void		SetupCurAttr(DWORD attr)			{ m_dwCurAttr = attr; };
	void		SetupAttrNone();
	DWORD		GetCurAttr()						{ return m_dwCurAttr; };
	//DWORD		GetPickedAttr()						{ if ( m_pAttrMap == NULL )return 0;return m_pAttrMap[m_dwPickedY][m_dwPickedX].dwAttr; };
	BYTE		GetPickedAttr()						{ if ( m_pAttrMap == NULL )return 0;return m_pAttrMap[m_dwPickedY][m_dwPickedX].byAttr; };
	
	BYTE		GetAttrType(int Y,int X)			
	{ 
		if( m_pAttrMap == NULL )return 0;
		
		if( X < 0 || X >= ( m_cSize * 2 ) ||
			Y < 0 || Y >= ( m_cSize * 2 )   )
			return 0;
			
		return m_pAttrMap[Y][X].byType; 
	};

	
	void		SetupAttr();
//	bool		BuildAttr();
	bool		LoadAttr(attr* attr);
	bool		UpdateAttr();
	void		SetupAttrUV(DWORD x, DWORD y, DWORD idx);
	void		SetupAttrUV();
	HRESULT		DisplayAttr();
	HRESULT		PickAttr(HWND hWNd, DWORD dwScreenWidth, DWORD dwScreenHeight);
	float		GetAttrHeight(DWORD x, DWORD y);

	/// sooree height : map.h map.cpp에 있던 것들을 field.h fieldattr.cpp 로 옮겨왔습니다. 
	float		Get__MapHeight( int cell_x, int cell_y );	/// 논리적 cell 좌표로 높이 구하기(속성 높이 포함) 
	float		Get__MapHeightF( float x, float y );		/// real-float 좌표로 지형 높이 구하기(속성 높이 포함) 
	float		Get__AttrHeight( int cell_x, int cell_y );	/// 로딩된 속성 높이만 읽어오기 
	float		Get__TerrainHeightF( float x, float y );	/// real-float 좌표로 지형 높이만 구하기
	float		Get__CharacterMoveHeight( float curPosX, float curPosY, int curCX, int curCY, int nextCX, int nextCY );

/**************************************************
	* OBJ SET
**************************************************/
protected:
	//DWORD		m_dwObjCount;
	int			m_nCurObj;
	int			m_nPickedObj;
	DWORD		m_dwObjRotDir;
	DWORD		m_dwObjPosDir;
	
	int			m_ObjCount;// 배경 오브젝트 중 인덱스가 틀린 것만 생성한 개수
	int			m_AllObjCount;// 맵데이타에서 읽어온 배경 오브젝트 총개수

public:	
	//-----------------------------------------------------------------------------
	// Desc: 오브젝트/TempObj(캐릭터선택화면에서사용)/m_PointObj마우스찍은자리
	//-----------------------------------------------------------------------------	
	Object		m_Object[MAX_OBJ];
	Object		m_TempObj;	
	Object		m_PointObj;

	//-----------------------------------------------------------------------------
	// Desc: 알파소팅
	//-----------------------------------------------------------------------------		
	//OBJ_SORT_TABLE*	m_pAlphaTable;
	int				m_SortCount;
	float			m_fSortTime;
	bool			m_bObjSortInit;
	int				m_PreMapID;
	int				m_NowMapID;// 현재 맵 아이디

	//-----------------------------------------------------------------------------
	// Desc: 타일색깔 변하게하기
	//-----------------------------------------------------------------------------
	float			m_fColorValue;			// 칼라평균값
	float			m_fPreTime;				// 명령들어온 시간 
	float			m_fEndTime;				// 변한색 지속되는 시간
	float			m_fChangeTime;			// 색변하게 하는 지시간
	bool			m_bChangeColor;			// 색 변하게 할 건지 플래그	
	DWORD			m_vertex_color;			// 색값	
	Color_Table		m_ChangeColor;			// 변할색	
	short			m_change_x;				// 색이 변하는 곳의 위치
	short			m_change_y;				// 색이 변하는 곳의 위치

	void			SetTileColor( short x, short y, short r, short g, short b, float fChangeTime, float fEndTime );
	void			UpdateTileColor();
	DWORD			SetVertexColor( DWORD color );

public:
	//-----------------------------------------------------------------------------
	// Desc: 빌보드
	//-----------------------------------------------------------------------------	
	int			m_BilObjCount;
	Object		m_BilObj[MAX_BILOBJ];
	void		CreateBilObj();	
	//void		DisplayBilObj( int index, D3DMATERIAL8 material, D3DXMATRIX matWorld, float scale );
	void		DisplayBilObj( int lod, int child, Object* pObj );

public:
	HRESULT		PickObject(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight);
	void		ObjToTemp();
	void		DelTempObj();
	void		CleanupObj();
	int			GetCurObj()		{ return m_nCurObj; };
	int			GetPickedObj()	{ return m_nPickedObj; };
	DWORD		GetObjCount()	{ return m_ObjCount; };
	virtual	bool		UpdateObj();	
	HRESULT		DisplayObj();
	void		CreateObjSortTable();	
	void		EffectDisplayUpdate();
	
	bool		m_bShadowMakeObj;
	bool		m_bShadowDisplayObj;
	void		ShadowMakeObj();
	void		ShadowDisplayObj();

/**************************************************
	* NPC SET
**************************************************/
public:
	DWORD		m_dwNpcCount;
	int			m_nCurNpc;
	int			m_nPickedNpc;

public:	
	NonPlayer	m_Npc[MAX_NPC_INTABLE];
	NonPlayer	m_TempNpc;

	int			GetCurNpc()		{ return m_nCurNpc; };
	int			GetPickedNpc()	{ return m_nPickedNpc; };
	DWORD		GetNpcCount()	{ return m_dwNpcCount; };
	HRESULT		PickNpc(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight);
	
	bool		UpdateNpc();
	bool		UpdateNpc( int number );
	HRESULT		DisplayNpc();
	void		AniProcessNpc();
	void		ShadowDisplayNpc();
	void		ShadowMakeNpc();
	void		CleanupNpc();
	
	//-----------------------------------------------------------------------------
	// Desc: NPC 정보리턴
	//-----------------------------------------------------------------------------
	NPCTABLE	m_NpcTable[MAX_NPC_INTABLE];	
	
	void		InitNpcTable();
	int			FindNpc( long unique, int race = c_NpcRace_Mob );	
	int			GetEmptyTable();
	int			GetNpcArrayIndex( int index, long unique );
	NonPlayer  *GetMonster(int Unique);

	NPCTABLE   *GetNpcTable( NonPlayer * pNonPlayer  );				// By simwoosung
	NPCTABLE   *GetNpcTable( int unique );							// By wxywxy
	
	long		GetNpcPos( int index, int unique );			
	int			GetNpcIndex( DWORD pos );
	bool		GetNpcAlive( EVENT_DATA_INFO& data );
	bool		GetNpcAlive( int index );
	bool		GetNpcAttackEnable( int index );
	D3DXVECTOR3	GetNpcBoundMax( int index, long unique );
	float		GetNpcBoundSize( int index, long unique );
	D3DXVECTOR3	GetNpcBoundMax(const short& idx)	{ return m_Npc[idx].GetBoundMaxPos(); };
	D3DXVECTOR3	GetNpcBoundMin(const short& idx)	{ return m_Npc[idx].GetBoundMinPos(); };
	D3DXMATRIX  GetNpcMatrix( int index, long unique );
	D3DXMATRIX  GetNpcCenterBoneMatrix( int index, long unique );

	D3DXMATRIX  GetNpcMatrix( int index ) { return m_Npc[ index ].GetPosTM(); }
	bool		GetNpcIsDraw( int index, long unique );
	bool		GetNpcVisible( int index, long unique );

	//-----------------------------------------------------------------------------
	// Desc: NPC 행동처리
	//-----------------------------------------------------------------------------
	void		NpcUnderAttack( int index, long unique );		// NPC 공격당함

	//-----------------------------------------------------------------------------
	// Desc: 말사기
	//-----------------------------------------------------------------------------	
	int			FindHorse( int index, long unique );		
	bool		FindHorse( int index );
	int			SM__HORSE__BUY( int index, long unique, DWORD pos, DWORD type );
	void		SM__HORSE__SELL( int index, bool bBlock = false );
	void		SM__HORSE__TIME( int index, int state );
	int			SM__TAMING__HORSE( int index, long unique );

	//-----------------------------------------------------------------------------
	// Desc: 서버에서 받기
	//-----------------------------------------------------------------------------	
//	int			SM__NPC__APPEAR( int index, DWORD unique, DWORD pos, int floor, DWORD type, BYTE status = 0 );	// NPC 생성
	int			SM__NPC__APPEAR( int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle = 0.0f );

	void		SM__NPC__DISAPPEAR( int index, DWORD unique );													// NPC 삭제

	int			SM__ITEM__APPEAR( DWORD aUnique, DWORD aCell, WORD aItemCode );	// NPC 생성
	void		SM__ITEM__DISAPPEAR( DWORD aUnique );												// NPC 삭제	
	
	
	void		SM__NPC__MOVE( int index, DWORD cur, DWORD move, int floor, DWORD unique, bool bFight );		// NPC 이동
	EVENT_DATA_INFO	GetAttackState( int index, DWORD unique );													// 공격 대상찾기
	void		SM__NPC__DIE( int index, DWORD npc_unique, bool m_bType );										// NPC 죽기	
	void		SM__NPC__RESPAWN( int index, DWORD pos, int floor, DWORD unique );								// NPC 탄생
	void		SM__NPC__ATTACK__PC( int index, DWORD unique, int motion, int dmg, long target );				// NPC -> PC 공격
	void		SM__NPC__DAMAGE( int caster, long caster_unique, int target, DWORD target_unique, int dmg, BYTE status, bool critic, int skillidx );	// NPC 스킬맞은 데미지
	void		SM__SLIDE__NPC( int index, DWORD unique, DWORD pos );											// NPC 밀려나기
	void		SM__NPC__ENERGY( int index, DWORD unique, BYTE hp );
	void		SM__NPC__STATUS( int index, DWORD unique, BYTE status );
	void		SM__NPC__USE__MAGIC( int index, DWORD unique, BYTE byMotion );

	void		SM__ITEM__CHANGE(DWORD index, DWORD unique, long lBlock, bool bAni);	// 아이템 변신
	void		SM__ITEM__MOVE(DWORD index, DWORD unique, int floor, DWORD move );		// 아이템 이동
	void		SM__ITEM__EXTINCTION( DWORD index, DWORD unique, bool bAni );			// 아이템 죽기
	void		SM__DESTROY__ITEM( int index, DWORD unique );							// 아이템 파괴
	void		SM__ANIMA__STATUS( DWORD pc_unique, DWORD npc_index, DWORD npc_unique, BYTE Status );	// 아이템 상태 


	//................................................................................................................
	// 신수 관련 By wxywxy	
	//................................................................................................................
	int			SM__HOLY__APPEAR( int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle , BYTE Level );
	void		SM__HOLY__DISAPPEAR( int index, DWORD unique );
	void		SM__HOLY__LEVELUP( DWORD Unique, WORD Level );

	//-------------------------------------------------------------------------------------------
	// 라이딩 관련 By simwoosung
	//-------------------------------------------------------------------------------------------
	int			SM__RIDE__APPEAR(int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle = 0.0f);
	void		SM__RIDE__DISAPPEAR( int index, DWORD unique );	

	//-------------------------------------------------------------------------------------------
	// 펫 관련 By simwoosung
	//-------------------------------------------------------------------------------------------
	int			SM__PET__APPEAR(int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , BYTE PetLevel ,float fAngle = 0.0f);
	void		SM__PET__LEVELUP( DWORD Unique, WORD nNewPetCode, BYTE aLevel );

	//-------------------------------------------------------------------------------------------
	// 유저형 NPC관련 By simwoosung
	//-------------------------------------------------------------------------------------------
	int			SM__PLAYER_NPC__APPEAR(int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle = 0.0f, BYTE PetLevel = 0 );
	
	//-------------------------------------------------------------------------------------------
	// NPC 관련 레벨업
	//-------------------------------------------------------------------------------------------
	void		SM__NPC__LEVELUP( DWORD aNPCID, DWORD aNPCCode );
	void		SM__NPC__CHANGE( DWORD aNPCID, DWORD aNPCCode );
	
	

	//by simwoosung
	HRESULT		ReSetNpcQuestMark(); 
	void		UpdateNPCQuestMark(int nNpcCode, int nNpcIndex);
	
/**************************************************
	* GRASS EDIT
**************************************************/	
protected:
	DWORD		m_dwGrassCount;
	int			m_nCurGrass;
	int			m_nPickedGrass;

public:
	DWORD		GetGrassCount()		{ return m_dwGrassCount; };
	int			GetPickedGrass()	{ return m_nPickedGrass; };
	int			GetCurGrass()		{ return m_nCurGrass; };

	CGrassList		m_GrassList[MAX_GRASS];
	CGrassList		m_GrassTemp;

	HRESULT			PickGrass(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight);

	void			SetupTempGrass(DWORD idx);
	HRESULT			DisplayTempGrass();
	void			CleanupTempGrass();
	void			DelTempGrass();
	void			UpdateTempGrassPos();
	void			GrassToTemp();
	bool			AddGrass();
	void			DelGrass();
	bool			UpdateGrass();
	virtual	HRESULT	DisplayGrass();
	void			CleanupGrass();

/**************************************************
	* GEOMETRY EDIT
**************************************************/
protected:
	DWORD		m_dwGEditDir;
	DWORD		m_dwGEditType;

public:
	HRESULT		PickPoint(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight);
	D3DXVECTOR3	m_vPickedPt;
	void		UpdateHeight(DWORD RANGE);
	void		UpdateVtxHeight(DWORD x, DWORD y, float height = 1.0f, bool plane = false);
	void		MakeFaceNormal(int indexX, int indexY, int pos);
	void		MakeVertexNormal(int indexX, int indexY, int pos);
	void		BuildNormal();
	void		BuildNormalFull();
	void		SetupGEditDir(GEOMEDIT_DIR dir)					{ m_dwGEditDir = dir; }
	void		SetupGEditType(GEOMEDIT_VOLUME type)			{ m_dwGEditType = type; }
	DWORD		GetGEditDir()									{ return m_dwGEditDir; }
	DWORD		GetGEditType()									{ return m_dwGEditType; }

/**************************************************
	* TILE MAPPING
**************************************************/
public:
	
	float		m_RealPickedX;			
	float		m_RealPickedY;


protected:
	DWORD		m_dwPickedX;
	DWORD		m_dwPickedY;
	
	DWORD		m_dwSelectedEle;
	DWORD		m_dwSelectedMultiEle;
	BYTE		m_bySelTexture;

	bool		m_bLibMapping;
	DWORD		m_dwPickedIdx;
	DWORD		m_dwPickedMultiIdx;
	bool		m_bMultiLibMapping;

public:

	DWORD		GetPickedX()		{ return m_dwPickedX; }
	DWORD		GetPickedY()		{ return m_dwPickedY; }
	void		SetupPickedX(DWORD x)	{ m_dwPickedX = x; };
	void		SetupPickedY(DWORD y)	{ m_dwPickedY = y; };
	DWORD		GetSelectedEle()	{ return m_dwSelectedEle; }
	void		SetupSelectedEle(DWORD ele)	{ m_dwSelectedEle = ele; }

	HRESULT		PickTile(HWND hWNd, DWORD dwScreenWidth, DWORD dwScreenHeight);
	void		SetupLibIndex();
	void		SetupUV();
	void		SetupUV(DWORD x, DWORD y);
	void		SetupUV(DWORD x, DWORD y, DWORD idx);
	void		SetupDefaultTexIndex();
	DWORD		CheckLeft(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckTop(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckRight(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckBottom(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckTopLeft(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckTopRight(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckBotLeft(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckBotRight(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckCenter(DWORD idx, DWORD x, DWORD y);
	DWORD		GetIdxType(DWORD idx, DWORD x, DWORD y);
	void		SetupIdx(DWORD type, DWORD idx, DWORD x, DWORD y);
	bool		GetLibMapping()		{ return m_bLibMapping; };
	void		SetupLibMapping(bool b)	{ m_bLibMapping	= b; };
	void		SetupIdxWithoutLib();
	DWORD		GetPickedIdx()		{ return m_dwPickedIdx; };
	void		SetupPickedIdx(DWORD idx)	{ m_dwPickedIdx = idx; };

	///
	void		SetupNewUV(const short& x, const short& y);

	/* Multi texturing */
/*	void		SetupDefaultMultiTexIndex();
	DWORD		CheckMultiLeft(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckMultiTop(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckMultiRight(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckMultiBottom(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckMultiTopLeft(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckMultiTopRight(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckMultiBotLeft(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckMultiBotRight(DWORD idx, DWORD x, DWORD y);
	DWORD		CheckMultiCenter(DWORD idx, DWORD x, DWORD y);
	void		SetupMultiUV(DWORD x, DWORD y, DWORD idx);
	void		SetupMultiUV(DWORD x, DWORD y);
	void		SetupMultiIdx(DWORD type, DWORD idx, DWORD x, DWORD y);
	DWORD		GetMultiIdxType(DWORD idx, DWORD x, DWORD y);
	void		SetupMultiLibIdx();
	void		SetupSelectedMultiEle(DWORD ele)		{ m_dwSelectedMultiEle = ele; };
	DWORD		GetSelectedMultiEle()					{ return m_dwSelectedMultiEle; };
	void		SetupMultiIdxWithoutLib();
	void		SetupPickedMultiIdx(DWORD idx)			{ m_dwPickedMultiIdx	= idx; };
	void		SetupMultiLibMapping(bool b)			{ m_bMultiLibMapping	= b; };
	bool		GetMultiLibMapping()					{ return m_bMultiLibMapping; };
*/

	BYTE					m_byRunsDir;
	BYTE					m_byCurWaterfallTexNum;
	BYTE					m_byCurWaterTexNum;

	//CBravoEffectiveWater	m_BravoEffectiveWater;
	BOOL					m_bShowWater;
	static float			m_fBaseHeightWater;
	static BYTE				m_byHighestWaterPlus;

/*CLOUD*/
	BOOL					m_bShowCloud;
	BYTE					m_byCurCloudTexNum;
	CBravoCloud				m_BravoCloud;

/*SKY*/
public:
//	LPDIRECT3DTEXTURE8		m_pTextureSky;
	BYTE					m_byCurSkyTexNum;
	BOOL					m_bShowSky;
//	CBravoSky				m_BravoSky;



/**************************************************
	* WATERFALL
**************************************************/
public:
	CBravoWaterfall		m_WaterfallTemp;
	CBravoWaterfall		m_WaterfallList[MAX_WATERFALL];

	short				m_nWaterfallCount;
	short				m_nPickedWaterfall;

	HRESULT				PickWaterfall(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight);
public:
	// temporary for adding
	void				SetupWaterfallTemp();
	void				DisplayWaterfallTemp();
	void				DelWaterfallTemp();
	void				UpdateWaterfallPosTemp();
	// stored to field
	bool				AddWaterfall();
	void				DisplayWaterfall();
	virtual	bool		UpdateWaterfall();

	// swtich 
	void				WaterfallToTemp();
	void				CleanupWaterfall();


/**************************************************
	* PARTICLE
**************************************************/
public:
	HRESULT				PickParticle(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight);
	short				m_nParticleCount;
//	CParticleSystem		m_Particle[MAX_PARTICLE];
	CParticleSystem		m_ParticleTemp;
	short				m_nPickedParticle;
	
public:
	bool				CreateParticle(short idx);
	bool				CreateParticleTemp(short idx);
	HRESULT				DisplayParticle();
	HRESULT				DisplayParticleTemp();
	void				UpdateParticlePosTemp();
	bool				AddParticle();
	void				ParticleToTemp();
	void				DelParticleTemp();
	bool				UpdateParticle();	
	void				CleanupParticle();

/**************************************************
	* FILE
**************************************************/
public:
	BYTE		GetTextureIdx()		{ return m_bySelTexture; }
//	void		SetupTextureIdx(DWORD selected)	{ m_bySelTexture = selected; }
	void		StoreHeight(float *height);
	void		StoreTexIdx(WORD *texidx);
	void		StoreObj(OBJ_INFO *obj);
	void		StoreAttr(attr* attr);
	void		StoreBlock(Block *block);
	void		StoreWarp(FILEWARP *warp);
	void		StoreNpc(NPC_INFO *npc);


/**************************************************
	* DEBUGING
**************************************************/
public:
	DWORD			dwTileVisibleCount;
	DWORD			dwAttrVisibleCount;
	DWORD			dwObjVisibleCount;
	DWORD			dwNpcVisibleCount;
	DWORD			dwGrassVisibleCount;
	DWORD			dwWaterVisibleCount;
	DWORD			dwParticleVisibleCount;
	DWORD			dwWaterfallVisibleCount;

public:
	CField();
	virtual	~CField();
};

#endif
