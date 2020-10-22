#ifndef	__ROCK_LANDPATCH_INCLUDED__
#define	__ROCK_LANDPATCH_INCLUDED__


#include "Camera.h"


//-----------------------------------------------------------------------------
// Miscellaneous helper functions : D3DxUtil.h
//-----------------------------------------------------------------------------

#define VARIANCE_DEPTH	(9)
#define	MAX_TRIPOOL		50000

#define SQR(x) ((x) * (x))
#define MAX(a,b) ((a < b) ? (b) : (a))
#define	PATCH_SIZE	32
#define	MAX_VTX		(PATCH_SIZE+1)*(PATCH_SIZE+1)
#define	MAX_IDX		10000
#define	VERTEX_INTERVAL	16	/// polygon scale

#define	CACHE_TIME_OUT	4*1000	/// 이 patch의 TileMap이 최근 렌더링 이후 이 시간이 경과하면 release 

///////////////////////////////////////////////////////////////////////////////

typedef	struct	_FVF_LOD
{
	D3DXVECTOR3	p;	/// LOD 그려줄 때는 텍스쳐 UV를 하드웨어 가속으로 처리 
}FVF_LOD, *LPFVF_LOD, **DLPFVF_LOD;

#define	D3DFVF_LOD	(D3DFVF_XYZ)

typedef	struct	_FVF_TILE
{
	D3DXVECTOR3	p;
	D3DXVECTOR2	t2;	/// I3D 엔진에서 t2 잡는 로직이 좀 복잡해서 호환성 유지를 위해 그냥 동일한 방식으로 갑니다.

}FVF_TILE, *LPFVF_TILE, **DLPFVF_TILE;

#define	D3DFVF_TILE	(D3DFVF_XYZ|D3DFVF_TEX1)


///////////////////////////////////////////////////////////////////////////////

struct	TriTreeNode
{
	TriTreeNode *LeftChild;
	TriTreeNode *RightChild;
	TriTreeNode *BaseNeighbor;
	TriTreeNode *LeftNeighbor;
	TriTreeNode *RightNeighbor;
};

class	CPatch 
{
public:
	CPatch();
	~CPatch();

protected:
#ifdef DIRECT_VERSION_9_MJH
	IDirect3DDevice9*			m_pd3dDevice;
	IDirect3DIndexBuffer9*		m_pIB;
	IDirect3DVertexBuffer9*		m_pVB;
#else
	IDirect3DDevice8*			m_pd3dDevice;
	IDirect3DIndexBuffer8*		m_pIB;
	IDirect3DVertexBuffer8*		m_pVB;
#endif // DIRECT_VERSION_9_MJH

public:
	HRESULT	RestoreDeviceObjects();
	HRESULT	InvalidateDeviceObjects();
	HRESULT	DeleteDeviceObjects();
	HRESULT	FinalCleanup();	
	HRESULT	Error(const char* strError);
	
	inline	int	GetWorldX()	{	return m_WorldX;	}
	inline	int	GetWorldY()	{	return m_WorldY;	}

	void	SetLODQuality( int quality = 2 );
	float	m_fLODQuality;

	inline	void SetNearDegree( int nearDegree )	{	m_NearDegree = nearDegree;	}
	inline	int  GetNearDegree()					{	return m_NearDegree;		}

	void		CacheTimeout( DWORD curTime );
	DWORD		m_dwLastRenderTime;

protected:
	CCamera *	m_pCamera;
	int			m_EyeX;
	int			m_EyeY;

	float*		m_HeightMap;
	int			m_WorldX, m_WorldY;										// World coordinate offset of this patch.
	int			m_NearDegree;	/// 0 : 근거리, 1 : 경계 patch, 2 : 원거리 

	float		m_fHeightMax;	
	float		m_fHeightMin;

	unsigned char	m_VarianceLeft[ 1<<(VARIANCE_DEPTH)];			// Left variance tree
	unsigned char	m_VarianceRight[1<<(VARIANCE_DEPTH)];			// Right variance tree

	unsigned char*	m_CurrentVariance;							// Which varience we are currently using. [Only valid during the Tessellate and ComputeVariance passes]
	unsigned char	m_VarianceDirty;								// Does the Varience Tree need to be recalculated for this Patch?
	bool			m_isVisible;									// Is this patch visible in the current frame?

	TriTreeNode		m_BaseLeft;										// Left base triangle tree node
	TriTreeNode		m_BaseRight;									// Right base triangle tree node
	int				m_nNumIndices;
	unsigned short*	m_pIndices;

public:
	// Some encapsulation functions & extras
	inline TriTreeNode *GetBaseLeft()  { return &m_BaseLeft; }
	inline TriTreeNode *GetBaseRight() { return &m_BaseRight; }
	inline char IsDirty()     { return m_VarianceDirty; }
	inline int  IsVisibile( ) { return m_isVisible; }

	void SetVisibility();

	// The static half of the Patch Class
#ifdef DIRECT_VERSION_9_MJH
	void Create( LPDIRECT3DDEVICE9 pd3dDev, int heightX, int heightY, int worldX, int worldY, float *hMap, CCamera* pCamera);
#else
	void Create( LPDIRECT3DDEVICE8 pd3dDev, int heightX, int heightY, int worldX, int worldY, float *hMap, CCamera* pCamera);
#endif // DIRECT_VERSION_9_MJH
	void Reset( int eyeX, int eyeY );
	void Tessellate();
	void Update();
	void Render();
	void ComputeVariance();

	// The recursive half of the Patch Class
	void			Split( TriTreeNode *tri);
	void			RecursTessellate( TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY, int node );
	void			RecursRender( TriTreeNode *tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY );
	unsigned char	RecursComputeVariance(	int leftX,  int leftY,  int leftZ,
											int rightX, int rightY, int rightZ,
											int apexX,  int apexY,  int apexZ,
											int node);

	///--------------------------------------------------------------
	///	tile
	///--------------------------------------------------------------

	struct STileRenderInfo
	{
		WORD nTileNum;
#ifdef DIRECT_VERSION_9_MJH
		IDirect3DTexture9* m_dpTileTex;	/// sooree ter opt
#else
		IDirect3DTexture8* m_dpTileTex;	/// sooree ter opt
#endif // DIRECT_VERSION_9_MJH
	};
	STileRenderInfo			m_nTileRenderInfo[1024];
	int						m_nTileRenderInfoNum;

	WORD*					m_pTexIdxList;
#ifdef DIRECT_VERSION_9_MJH
	IDirect3DIndexBuffer9*	m_pTileIB;
	IDirect3DVertexBuffer9*	m_pTileVB;
#else
	IDirect3DIndexBuffer8*	m_pTileIB;
	IDirect3DVertexBuffer8*	m_pTileVB;
#endif // DIRECT_VERSION_9_MJH

	void	CreateTileMap( WORD* pTexIdxList );
	HRESULT	RestoreDeviceObjectsTileMap();
	void	RenderTileMap();
};

#endif