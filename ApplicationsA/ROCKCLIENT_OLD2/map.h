#ifndef		_____MAP_____
#define		_____MAP_____

//#include	"ItemDrop.h"
#include	"SkyBox.h"
#include	"quadlist.h"
#include	"landscape.h"	/// sooree terrain
#include    "CCloud.h"
#include    "CSetEnv.h"
#include    "CCollision.h"
#include	"CNodeSort.h"
#include	"bravolibs\\map\\fileinfo.h"

#pragma warning(disable:4786)
#include <list>
#include <map>
using namespace std;

#define		MAX_DARK					1.5f;
#define		MAX_BRIGHT					1.5f;
#define		MAX_DROPPEDITEM_INFIELD		128
#define		MAX_COINT_COUNT				5


#ifdef APPLYWIDE_MAP
	#define		MAX_MAP_BLOCK				1024
#else
	#define		MAX_MAP_BLOCK				512
#endif

#define WATER_TRANS_TIMER		3000

struct  SNationOBJ
{
	WORD  dOBjNum;
	int   nNation;
	
	SNationOBJ()
	{
		memset(this, 0, sizeof(SNationOBJ));
	}
};

struct SPvPFieldInfo
{
	DWORD					theNPCID;
	DWORD					theNPCCode;
	DWORD					theNPCCell;
	BYTE					theOccupationNation;		//진지 점령한 나라

	SPvPFieldInfo()
	{
		memset(this, 0, sizeof(SPvPFieldInfo));
	}

	void operator = (const SPvPFieldInfo &TempValue)
	{
		theNPCID			= TempValue.theNPCID;
		theNPCCode			= TempValue.theNPCCode;
		theNPCCell			= TempValue.theNPCCell;
		theOccupationNation = TempValue.theOccupationNation;
	}
};

typedef map < WORD, WORD > EmbObjList;
typedef map < WORD, WORD >::iterator EmbObjListPos;

typedef map < WORD, SNationOBJ > NationOBJList;
typedef map < WORD, SNationOBJ >::iterator NationOBJListPos;

typedef list < SPvPFieldInfo * > PvPFieldInfoList;
typedef list < SPvPFieldInfo * >::iterator PvPFieldInfoListPos;

//-----------------------------------------------------------------------------
// Desc: 맵블럭
//-----------------------------------------------------------------------------
struct	MAP_BLOCK
{	
	char	Attr;		// 속성값
	int		Index;		// 인덱스
	long	Unique;		// 유니크

	MAP_BLOCK()		{ Init(); }
	~MAP_BLOCK()	{ Init(); }
	void	Init();
	void	SetData( char attr, int index, long unique );
	bool	GetClear();
	bool	FindTarget( int index, long unique );
}; 

struct		vtxpos
{
	float	x, y, z;
	DWORD	color;
};

#define		fvf_vtxpos	(D3DFVF_XYZ|D3DFVF_DIFFUSE)

enum		nodedir
{
	nodedir_bl	= 0,
	nodedir_br,
	nodedir_tl,
	nodedir_tr,
	nodedir_parent,
};

struct		node_obj
{
	node_idxlist	idxlist;
	node_idxlist	grslist;
	///-- 중심점 위치
	short			cx, cy;
	short			size;
	node_obj *		child[4];

	node_obj()
	{
		for ( short i = 0; i < 4; i ++ )
			child[i]	= NULL;
	}
};


enum Map_Property
{	
	normal = 0,
	plain,
	desert,
	snow_plain,
	high_plain,
	woods,
	cayon,
	rain,
	snow,
	wind,
	fog,
	indoor,
};

/****************************************************************************************************
	* dropped items in field
****************************************************************************************************/
typedef struct s_AppearItem
{
	DWORD		ItemID;
	DWORD		Cell;
	DWORD		NPCID;
	WORD		ItemCode;
} s_AppearItem;


/****************************************************************************************************
	* major field manager
****************************************************************************************************/

class			CGrassPoint
{
public:
	GRASS_POINT	*	m_Data;
	short			m_nCount;
	short			m_nCurrent;
	short			m_nCurrentEmpty;
	GRASS_POINT	*	m_pDataHeader;

	HRESULT		Add(const short& x, const short& y, const BYTE& range, const BYTE& density, const BYTE& idx);
	HRESULT		Add(GRASS_POINT* pData);
	HRESULT		Del();
	HRESULT		FinalCleanup();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT		DisplayCurrent(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT		DisplayCurrent(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	GRASS_POINT *	GetTailNode();
	
	CGrassPoint();
	~CGrassPoint();
};

class		CTileTexture
{
protected:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9		m_pd3dDevice;
#else
	LPDIRECT3DDEVICE8		m_pd3dDevice;
#endif // DIRECT_VERSION_9_MJH
///	IDirect3DTexture8 **	m_dpTextures;
///	IDirect3DTexture8 **	m_dpTextures2;
	int						m_nCount;

	void		CountTexture(const char* rootDir);
	HRESULT	Error(const TCHAR* strError)	{ MessageBox(NULL, strError, _T("TileTexture"), MB_OK|MB_ICONSTOP);return E_FAIL; }

public:
	/// sooree ter opt 
#ifdef DIRECT_VERSION_9_MJH
	IDirect3DTexture9 **	m_dpTileTex;
#else
	IDirect3DTexture8 **	m_dpTileTex;
#endif // DIRECT_VERSION_9_MJH
	int						m_nTileTexCount;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	GetTileTexture( int nTileTexIndexIn,
#else
	LPDIRECT3DTEXTURE8	GetTileTexture( int nTileTexIndexIn,
#endif // DIRECT_VERSION_9_MJH 
										int& nTileGroupOut, 
										int& nTileTypeIdxOut, 
										int& nTileAngleOut, 
										int& nSubTileOut );

public:
#ifdef DIRECT_VERSION_9_MJH
	IDirect3DTexture9*		m_dpBaseTextures;
	IDirect3DTexture9*		m_dpBaseTextureLight;	/// sooree terrain
#else
	IDirect3DTexture8*		m_dpBaseTextures;
	IDirect3DTexture8*		m_dpBaseTextureLight;	/// sooree terrain
#endif // DIRECT_VERSION_9_MJH

#ifdef DIRECT_VERSION_9_MJH
	HRESULT	InitDeviceObjects(LPDIRECT3DDEVICE9 pd3ddev);
#else
	HRESULT	InitDeviceObjects(LPDIRECT3DDEVICE8 pd3ddev);
#endif // DIRECT_VERSION_9_MJH
	HRESULT	LoadAllUnits(const char* rootDir);
	HRESULT ReSetShadowMap( const char* rootDir );
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	GetTextureSelection(const int& selTex, const unsigned char& bysel);
#else
	LPDIRECT3DTEXTURE8	GetTextureSelection(const int& selTex, const unsigned char& bysel);
#endif // DIRECT_VERSION_9_MJH
	HRESULT	FinalCleanup();
	int						GetCount()	{ return m_nCount; }

	CTileTexture();
	~CTileTexture();
};


#include "PickHeightField.h"	/// pick by sooree
#include "RockWater.h"			/// sooree water


struct	SColonyMapExInfo
{
	int				m_nMapIndex;		///-- 콜로니맵 타입인덱스	
	DWORD			m_dAttackFronId;	///-- 공성측 프론티어 아이디
	int				m_nAttackNation;	///-- 공성측 국가
	DWORD			m_dDeffenFronId;	///-- 수성측 프론티어 아이디
	int				m_nDeffenNation;	///-- 수성측 국가

	SColonyMapExInfo()
	{
		memset(this, 0, sizeof(SColonyMapExInfo));
	}
};	

class		CBravoMap : public CField, 
						public CPickHeightField	/// pick by sooree
{
public:
	bool					theWireframe;
	// kstar79
#ifdef DIRECT_VERSION_9_MJH
	void					SetDevice( LPDIRECT3DDEVICE9 pDevice ) { m_pDevice = pDevice; }
#else
	void					SetDevice( LPDIRECT3DDEVICE8 pDevice ) { m_pDevice = pDevice; }
#endif // DIRECT_VERSION_9_MJH
	void					SetLight( int type = 2 );

	D3DXMATRIX				m_matIdentity;

	TCHAR					theMapPath[256];/// ....sh

// 게임 오브젝트 소팅관련 By wxywxy
public:
	
	void					MakeObjectBoundBox(void);

	CNodeSort				m_SortTable;

	void					DisplaySortNode(void);

	void					SetMapObjectRenderState(void);
	void					EndMapObjectRenderState(void);
	
	void					SetCharacterObjectRenderState(void);
	void					EndCharacterObjectRenderState(void);	

	void					SetSwordTailRenderState(void);
	int						CheckPointInBox(D3DXVECTOR3 *pVec,D3DXPLANE *pPlane);	

	///-- By simwoosung
	void					SetShadowMakeRenderState();
	void					EndShadowMakeRenderState();
	void					SetShadowDisplayRenderState();
	void					EndShadowDisplayRenderState();

public:

	//...............................................................................................................
	// 이전에 업데이트 되었던 중심 좌표 얻기 By wxywxy
	//...............................................................................................................
	short					GetPreCX(void)		{ return thePreCX; }
	short					GetPreCY(void)		{ return thePreCY; }

protected:
	FILE_INFO				m_File;
	BRVF					m_NewFile;
	DWORD					m_dwCount;
	DWORD					m_dwIdx;
	char **					m_szNameList;
	DWORD					m_dwMapIdx;

	short					thePreCX;		// 여러번 로딩하지 말기 *^^*
	short					thePreCY;

public:
//	BYTE					m_byMapType;
	
	/// sooree water
	int						theWaterListNum;
	CRockWater*				theWaterList;

	CCollision				m_Collision;	// 충돌 관련 By wxywxy

	/// sooree terrain
	CLandscape				theTerrain;

	NPC_INFO2			   *m_pNpcList;
	int						m_NpcCount;

	CSkyBox				    m_SkyBox;
	CCloud					m_Cloud;
	CSetEnv					m_SetEnv;

	// 물 높이 구하기 By wxywxy
	float					GetWaterHeight(float x,float z);	

public:
	//-----------------------------------------------------------------------------
	// 맵블럭처리하는 함수 
	//-----------------------------------------------------------------------------
	MAP_BLOCK				m_Block[MAX_MAP_BLOCK][MAX_MAP_BLOCK];
	void					InitMapData();
	int						FindMapData( int x, int y );
	bool					SetMapData( int attr, int index, long lBlock,  bool bPC = false );
	bool					SetMapData( int attr, int index, int x, int y, bool bPC = false );
	void					MapDataProcess(  int attr, int index, int x, int y, bool bPC );

public:
	bool					m_bReady;		
	bool					m_bLightEffectDark;

	long					m_lPrevTime;
	long					m_lPrevTime2;

	DWORD					CountBrvf();
#ifdef DIRECT_VERSION_9_MJH
	bool					Create(LPDIRECT3DDEVICE9 pDevice);
#else
	bool					Create(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void					FinalCleanup();
	void					Cleanup();
	bool					Load(DWORD idx);
	bool					LoadVF(DWORD idx);	/// sooree pack
	bool					UpdateTerrain( int cx, int cy, bool bAlways = false );	  
	HRESULT					Display();
	void					DisplayTempMap();
	void					SetupMapIdx(DWORD idx)		{ m_dwMapIdx = idx; };
	DWORD					GetMapIdx()					{ return m_dwMapIdx; };
	DWORD					GetMapSize()				{ return m_cSize; };
	DWORD					GetWarp1();	


	///----------------------------------------
	/// pick by sooree
	///----------------------------------------
	float	PHF_GetHeight( const float aX, const float aZ );	/// real 좌표로 높이 얻기 
	float	PHF_GetHeight( const int aX, const int aZ );		/// cell index로 높이 얻기 
	bool	PHF_IsValidPoint( const float aX, const float aZ );
	bool	PHF_CellIntersectTriangle( int aX, int aZ );		/// cell index로 검사 
	void	PHF_VertexComparePick(  float aCenterX, float aCenterZ, float aDistance );
	HRESULT	PHF_PickDest( HWND hWnd );

	/// sooree height : field.h fieldattr.cpp 로 옮깁니다. 
///	float					Get__MapHeight( int x, int y );
///	float					Get__MapHeightF( float x, float y );	/// by sooree real-float pos

	void					CreateShadow();
	void					DisplayShadow();	
	void					CreateObjIndex();

	// test
	short					m_nEditLight;
	short					m_nEditMaterial;
	bool					UpdateObj();


	// water material
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9			m_MaterialWater;
#else
	D3DMATERIAL8			m_MaterialWater;
#endif // DIRECT_VERSION_9_MJH
//	void					DisplayWater();

/**********
	* STATIC NODE UPDATE WITH QUAD TREE
**********/

	node_obj *				m_pNodeObj;
	short					m_nDepth;

	bool					CreateObjNode();
	void					CleanupObjNode();
	void					RecursiveCleanupNode(node_obj** node, short depth);
	bool					RecursiveCreateNode(node_obj* node, short depth);
	BYTE					ObjInTree(const short& idx, const short& sidx, const short& cx, const short& cy);
	void					FindNode(const short& idx, const short& sidx, node_obj* node, short depth);
	void					RecursiveNodeVisible(node_obj* node, short depth);

	bool					UpdateObj2();

//	bool					UpdateWater();
//	bool					UpdateWaterEffect();
	bool					CheckObjBlending(const short& idx, const short& sidx);

	short					PickNpc();

	// pc on grass
	void					CheckPcOnGrass(node_obj* node, short depth);

	bool					UpdateWaterfall();

	// update light by time

	void					ProcessLightByTime();

	// debug

//	BYTE	*				m_byColorTemp;
//	void					CheckColor(const short& cx, const short& cy);
	
#ifdef DIRECT_VERSION_9_MJH
	IDirect3DVertexBuffer9*	m_pVBPos;
	IDirect3DIndexBuffer9*	m_pIBPos;
#else
	IDirect3DVertexBuffer8*	m_pVBPos;
	IDirect3DIndexBuffer8*	m_pIBPos;
#endif // DIRECT_VERSION_9_MJH
	bool					updateonter(const short& cx, const short& cy);
	short					m_nvtxinvb;
	short					m_nidxinib;
	void					displayonter();

	// grass
	CGrassPoint		m_GrassPoint;
	void			BuildGrassWithPoint(const short& idx, const short& listidx);
	HRESULT			DisplayGrass();
	
	//by simwoosung
	HRESULT			InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT			RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT			RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void			CreateShadowList();

	CWaveList		m_WaveList;

	// 텍츠처 에니 관련
	D3DXMATRIX		m_matWaterTrans;
	float			m_fWaterRange;
	DWORD			m_OldWaterTimer;
	
	///-- By simwoosung -- 거대 유닛관련
	bool			IsNoGiantUnit(D3DXVECTOR3 vNexPos);
	bool			IsNoGiantUnit(D3DXVECTOR3 vNowPos, D3DXVECTOR3 vNexPos, Character * pCharacter);
	BOOL			IsGiantUnit(Character *pCharacter);
	bool			IsNoBuildingUnit(D3DXVECTOR3 vOriPos, D3DXVECTOR3 vDir, float fLength);
	
	///-- By simwoosung -- 콜로니관련
	UINT			m_uMapType;			///-- 맵 타입

	SColonyMapExInfo  m_nExColInfo;	

	BOOL			ReSetMapIndex(int nIndex);
	void			LoadEmbObjAndNationObj();

	EmbObjList		m_EmbObjList;
	NationOBJList   m_NationObjList;

	BOOL			IsEmbObj(WORD nObjNum);
	SNationOBJ *	FineNationOBJPtr(WORD nObjNum);	

	///-- By simwoosung -- 전장 중립필드 관련	
	PvPFieldInfoList m_PvPFieldInfoList;

	void			ClearPvPFieldInfoList();

	CBravoMap();
	~CBravoMap();
	
public:
	std::vector<s_AppearItem> theAppearItemList;
	
	SHADOW_INFO		m_ShadowList[3];		
};

extern	CBravoMap		g_Map;



struct	FVF_LIGHTMAP
{
	float	x, y, z;
	DWORD	color;
	float	u1, v1;
	float	u2, v2;
};

#define	D3DFVF_LIGHTMAP	(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2)

class	CEffectMap
{
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		m_pTexture;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
#else
	LPDIRECT3DTEXTURE8		m_pTexture;
	LPDIRECT3DVERTEXBUFFER8	m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
#endif // DIRECT_VERSION_9_MJH

	D3DXMATRIX				m_matPos;
	D3DXMATRIX				m_matTexPos;
	DWORD					m_dwFVF;
	short					m_nVtxCount;
	short					m_nIdxCount;

#ifdef DIRECT_VERSION_9_MJH
	HRESULT					CreateEnvironment(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT					CreateEnvironment(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	HRESULT					Update(const D3DXVECTOR3& v);
	HRESULT					Update(const float& x, const float& y, const float& z);
	HRESULT					Update(const D3DXMATRIX& mat);
#ifdef DIRECT_VERSION_9_MJH
	HRESULT					Display(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT					Display(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	~CEffectMap();
	CEffectMap();
};

extern	CTileTexture	g_TileTexture;

#endif