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
#include	<vector>		// STL�� ���͸� ���� ���ؼ�
using namespace	std;		// STL�� ���ڴٰ� ����

#define		MAX_NPC_INTABLE		256
#define		MAX_OBJ				128
#define		MAX_BILOBJ			16

#define		MAX_GRASS			1024
#define		TILE_SIZE			16

#define		D3DFVF_PNTD		(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2)
#define		D3DFVF_PT		(D3DFVF_XYZ|D3DFVF_TEX1)

#define		DISPLAY_RANGE	18	/// by sooree ������ Ÿ�ϸ� ���� ( ���� + 2 )

enum	MAPSTATS
{
	MS_FIELD	= 0,
	MS_INDOOR,
};


//-----------------------------------------------------------------------------
// Desc: Į������
//
// Copyright (c) 2001.11.28 By KStaR79
//-----------------------------------------------------------------------------
struct	Color_Table
{
	int		r, g, b;	
};

//-----------------------------------------------------------------------------
// Desc: ����Ÿ�� ���̺�
//
// Copyright (c) 2001.11.28 By KStaR79
//-----------------------------------------------------------------------------
#define	LIGHTTALBE_COUNT	64

struct	LIGHTTILE_TABLE
{
	int			Display;	// �׸���? 0�ȱ׸��� 1�׸���(ȸ��X) 2�׸���(ȸ��0)	
	D3DXMATRIX	matPos;		// ������ġ��	
	float		fRot;		// ȸ����
	int			PreTime;	// �ð�
	int			Index;		// -1:PC
	long		lUnique;	// ����ũ
		
	LIGHTTILE_TABLE()	{ Clear(); }
	void	Clear()		{ Display = -2; Index = -1; lUnique = -1; }
};


//-----------------------------------------------------------------------------
// Desc: ������Ʈ ���� ���̺�
//
// ������Ʈ ���ý� ���Ǵ� �������� �����ϴ� ����ü
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
// Desc: �� ���̺�
//
// 1. ��(��Ƽ��)�� ���� �������� �����ϴ� ����ü
// 2. NPCTABLE�� �ߺ��Ǿ ���Ǹ�, NPCTABLE���� �ش� �����̺��� �ε����� �����ϰ� �ִ�
//
// Copyright (c) 2001.11.28 By KStaR79
//-----------------------------------------------------------------------------
struct		HORSE_TABLE
{
	char	szName[10];		// �̸�(�ѱ�5��)
	bool	bMale;			// ����, ����
	BYTE	byLevel;		// ����
};
*/


//-----------------------------------------------------------------------------
// Desc: NPC(����, ���ε�) ���̺�
//
// Copyright (c) 2001.11.28 By KStaR79
//-----------------------------------------------------------------------------
// ����
enum NPC_STATUS
{
	WAIT = 0,			// ���		0
	MOVE,				// �̵���	1
	FIGHT,				// �ο���	2
	DEATH,				// ������	3
	RUN,				// ������	4
	POISON,				// �ߵ�		5
	STUN,				// ����		6
};

///-- By simwoosung
///-- NPC�� �÷��̾� ���ҽ��� ����ϱ� ���� �ʿ��� �߰� ���̺�
struct	 NPC_EX_PCTABLE
{
	BYTE				theCitizen;									// RAIN, MILLENA
	BYTE				theGender;									// MALE, FEMALE
	DWORD				theFace;
	DWORD				theHair;
	BYTE				theMainClass;								// ���� Ŭ����

	WORD				theWearItem[RP_MAX_EQUIP];					// ��ű�1, 2 ����
};

struct		NPCTABLE
{
	int				code;										// g_Pc_Manager.theNPCInfoTable.theCode 
	int				index;										// g_Pc_Manager.m_NpcFileInfo[index]
	long			lUnique;									// ����ũ
	long			lBlock;										// ����Ÿ��
	long			lDestBlock;									// ����Ÿ��
	BYTE			nStatus;									// ����
	int				race;										// -1 ��, -2 ���̺������, -3 ��
//	BYTE			nCurrentFloor;								// �� 
//	DWORD			type;										// Ÿ��(�����)
	BYTE			theLevel;									// ���� 
	char			szName[31];									// �̸�
	DWORD			theHP;										// ü�� 
	DWORD			theSMP;										// ���׹̳� 
	DWORD			theMaxHP;									// �ִ� ü��ġ 
	DWORD			theRealHP;									// �������� �޴� �������̽� ������ ���� HP
	WORD			theKeepupSkill[RP_MAX_KEEPUP_SKILL];		// ���Ӽ� ��ų �ڵ�
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
// �����Ӽ�
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
	MATYPE_LAND	= 0,				// ��
	MATYPE_GRASS,					// Ǯ
	MATYPE_WATER,					// ��
	MATYPE_STONE,					// ��
	MATYPE_WOOD,					// ����
	MATYPE_SNOW,					// ��
	MATYPE_SAND						// ��
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
	list <Character * > m_GiantUnitList;	///-- �Ŵ�����, ����, ��������Ʈ  
	list <Character * > m_GiantBilbList;  ///-- �Ŵ���� ����Ʈ

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
	
	///���콺 Ŀ�� Ŭ���̾�Ʈ ��ǥ
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
	/// ������ Load() �޽��� ����....
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

	/// sooree height : map.h map.cpp�� �ִ� �͵��� field.h fieldattr.cpp �� �ŰܿԽ��ϴ�. 
	float		Get__MapHeight( int cell_x, int cell_y );	/// ���� cell ��ǥ�� ���� ���ϱ�(�Ӽ� ���� ����) 
	float		Get__MapHeightF( float x, float y );		/// real-float ��ǥ�� ���� ���� ���ϱ�(�Ӽ� ���� ����) 
	float		Get__AttrHeight( int cell_x, int cell_y );	/// �ε��� �Ӽ� ���̸� �о���� 
	float		Get__TerrainHeightF( float x, float y );	/// real-float ��ǥ�� ���� ���̸� ���ϱ�
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
	
	int			m_ObjCount;// ��� ������Ʈ �� �ε����� Ʋ�� �͸� ������ ����
	int			m_AllObjCount;// �ʵ���Ÿ���� �о�� ��� ������Ʈ �Ѱ���

public:	
	//-----------------------------------------------------------------------------
	// Desc: ������Ʈ/TempObj(ĳ���ͼ���ȭ�鿡�����)/m_PointObj���콺�����ڸ�
	//-----------------------------------------------------------------------------	
	Object		m_Object[MAX_OBJ];
	Object		m_TempObj;	
	Object		m_PointObj;

	//-----------------------------------------------------------------------------
	// Desc: ���ļ���
	//-----------------------------------------------------------------------------		
	//OBJ_SORT_TABLE*	m_pAlphaTable;
	int				m_SortCount;
	float			m_fSortTime;
	bool			m_bObjSortInit;
	int				m_PreMapID;
	int				m_NowMapID;// ���� �� ���̵�

	//-----------------------------------------------------------------------------
	// Desc: Ÿ�ϻ��� ���ϰ��ϱ�
	//-----------------------------------------------------------------------------
	float			m_fColorValue;			// Į����հ�
	float			m_fPreTime;				// ��ɵ��� �ð� 
	float			m_fEndTime;				// ���ѻ� ���ӵǴ� �ð�
	float			m_fChangeTime;			// �����ϰ� �ϴ� ���ð�
	bool			m_bChangeColor;			// �� ���ϰ� �� ���� �÷���	
	DWORD			m_vertex_color;			// ����	
	Color_Table		m_ChangeColor;			// ���һ�	
	short			m_change_x;				// ���� ���ϴ� ���� ��ġ
	short			m_change_y;				// ���� ���ϴ� ���� ��ġ

	void			SetTileColor( short x, short y, short r, short g, short b, float fChangeTime, float fEndTime );
	void			UpdateTileColor();
	DWORD			SetVertexColor( DWORD color );

public:
	//-----------------------------------------------------------------------------
	// Desc: ������
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
	// Desc: NPC ��������
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
	// Desc: NPC �ൿó��
	//-----------------------------------------------------------------------------
	void		NpcUnderAttack( int index, long unique );		// NPC ���ݴ���

	//-----------------------------------------------------------------------------
	// Desc: �����
	//-----------------------------------------------------------------------------	
	int			FindHorse( int index, long unique );		
	bool		FindHorse( int index );
	int			SM__HORSE__BUY( int index, long unique, DWORD pos, DWORD type );
	void		SM__HORSE__SELL( int index, bool bBlock = false );
	void		SM__HORSE__TIME( int index, int state );
	int			SM__TAMING__HORSE( int index, long unique );

	//-----------------------------------------------------------------------------
	// Desc: �������� �ޱ�
	//-----------------------------------------------------------------------------	
//	int			SM__NPC__APPEAR( int index, DWORD unique, DWORD pos, int floor, DWORD type, BYTE status = 0 );	// NPC ����
	int			SM__NPC__APPEAR( int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle = 0.0f );

	void		SM__NPC__DISAPPEAR( int index, DWORD unique );													// NPC ����

	int			SM__ITEM__APPEAR( DWORD aUnique, DWORD aCell, WORD aItemCode );	// NPC ����
	void		SM__ITEM__DISAPPEAR( DWORD aUnique );												// NPC ����	
	
	
	void		SM__NPC__MOVE( int index, DWORD cur, DWORD move, int floor, DWORD unique, bool bFight );		// NPC �̵�
	EVENT_DATA_INFO	GetAttackState( int index, DWORD unique );													// ���� ���ã��
	void		SM__NPC__DIE( int index, DWORD npc_unique, bool m_bType );										// NPC �ױ�	
	void		SM__NPC__RESPAWN( int index, DWORD pos, int floor, DWORD unique );								// NPC ź��
	void		SM__NPC__ATTACK__PC( int index, DWORD unique, int motion, int dmg, long target );				// NPC -> PC ����
	void		SM__NPC__DAMAGE( int caster, long caster_unique, int target, DWORD target_unique, int dmg, BYTE status, bool critic, int skillidx );	// NPC ��ų���� ������
	void		SM__SLIDE__NPC( int index, DWORD unique, DWORD pos );											// NPC �з�����
	void		SM__NPC__ENERGY( int index, DWORD unique, BYTE hp );
	void		SM__NPC__STATUS( int index, DWORD unique, BYTE status );
	void		SM__NPC__USE__MAGIC( int index, DWORD unique, BYTE byMotion );

	void		SM__ITEM__CHANGE(DWORD index, DWORD unique, long lBlock, bool bAni);	// ������ ����
	void		SM__ITEM__MOVE(DWORD index, DWORD unique, int floor, DWORD move );		// ������ �̵�
	void		SM__ITEM__EXTINCTION( DWORD index, DWORD unique, bool bAni );			// ������ �ױ�
	void		SM__DESTROY__ITEM( int index, DWORD unique );							// ������ �ı�
	void		SM__ANIMA__STATUS( DWORD pc_unique, DWORD npc_index, DWORD npc_unique, BYTE Status );	// ������ ���� 


	//................................................................................................................
	// �ż� ���� By wxywxy	
	//................................................................................................................
	int			SM__HOLY__APPEAR( int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle , BYTE Level );
	void		SM__HOLY__DISAPPEAR( int index, DWORD unique );
	void		SM__HOLY__LEVELUP( DWORD Unique, WORD Level );

	//-------------------------------------------------------------------------------------------
	// ���̵� ���� By simwoosung
	//-------------------------------------------------------------------------------------------
	int			SM__RIDE__APPEAR(int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle = 0.0f);
	void		SM__RIDE__DISAPPEAR( int index, DWORD unique );	

	//-------------------------------------------------------------------------------------------
	// �� ���� By simwoosung
	//-------------------------------------------------------------------------------------------
	int			SM__PET__APPEAR(int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , BYTE PetLevel ,float fAngle = 0.0f);
	void		SM__PET__LEVELUP( DWORD Unique, WORD nNewPetCode, BYTE aLevel );

	//-------------------------------------------------------------------------------------------
	// ������ NPC���� By simwoosung
	//-------------------------------------------------------------------------------------------
	int			SM__PLAYER_NPC__APPEAR(int index, DWORD unique, DWORD pos, DWORD hp, DWORD sp, BYTE status , float fAngle = 0.0f, BYTE PetLevel = 0 );
	
	//-------------------------------------------------------------------------------------------
	// NPC ���� ������
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
