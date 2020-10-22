#ifndef	__ROCK_LANDSCAPE_INCLUDED__
#define	__ROCK_LANDSCAPE_INCLUDED__


#include "landpatch.h"
RB_USING
#include "RockResourceManager.h"

#include <set>
#include <map>
using namespace std;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

typedef map < int , CPatch* >				CPatchMap;
typedef map < int , CPatch* >::iterator		CPatchMap_i;
typedef map < int , CPatch* >::value_type	CPatchMap_vt;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

class	CLandscape
{
public:
	~CLandscape();
	CLandscape();
#ifdef DIRECT_VERSION_9_MJH
	IDirect3DDevice9*	m_pd3dDevice;
#else
	IDirect3DDevice8*	m_pd3dDevice;
#endif // DIRECT_VERSION_9_MJH
	CCamera *			m_pCamera;

	static TriTreeNode	*AllocateTri();
	static TriTreeNode	m_TriPool[MAX_TRIPOOL];
	static int			m_nNextTriNode;

	static int			m_nMapSize;
	static int			m_nPadedMapSize;
	static float		m_fFrameVariance;
	float*				m_HeightMap;

#ifdef	_DEBUG
	int					m_nRenderedPatches;
	static int			m_nNumTrisRendered;
	int					m_nNumTileTrisRendered;
	int					m_nNumTilePatches;
	static int			m_nNumTileRenderCall;
#endif

	static int			m_nDesiredTris;
	static int			m_nNumPatches;
	CPatch	**			m_Patches;

	WORD*				m_pTileIndexList;	/// tile patch를 실시간으로 restore 하기 위해 기억해 놓는다. 

	static int			m_nLODQuality;
	void				SetLODQuality( int quality = 2 );
	inline int			GetLODQuality()						{	return m_nLODQuality;			}

	static int			m_NearPatchWorldIdxMinX;
	static int			m_NearPatchWorldIdxMinY;
	static int			m_NearPatchWorldIdxMaxX;
	static int			m_NearPatchWorldIdxMaxY;

	bool				m_bAccessEnable;

	static void			SetNextTriNode(int nNextNode)	{ m_nNextTriNode = nNextNode; }
	static int			GetNextTriNode()				{ return m_nNextTriNode; }
	
	bool	LoadFileHF( const char* pszFileName );	
#ifdef DIRECT_VERSION_9_MJH
	void	CreateTerrain(	LPDIRECT3DDEVICE9 pd3dDev, CCamera* pCamera, int nDesiredTri,
#else
	void	CreateTerrain(	LPDIRECT3DDEVICE8 pd3dDev, CCamera* pCamera, int nDesiredTri,
#endif // DIRECT_VERSION_9_MJH 
							int nMapSize, short* pHeightList, WORD* pTexIdxList );	

#ifdef DIRECT_VERSION_9_MJH
	void	Create( LPDIRECT3DDEVICE9 pd3dDev, CCamera* pCamera, int nDesiredTri, WORD* pTexIdxList );	
#else
	void	Create( LPDIRECT3DDEVICE8 pd3dDev, CCamera* pCamera, int nDesiredTri, WORD* pTexIdxList );	
#endif // DIRECT_VERSION_9_MJH
	
	void	Reset( int eyeX, int eyeY );
	void	Tessellate();
	void	Render();
	void	Update();

	HRESULT	RestoreDeviceObjects();
	HRESULT	InvalidateDeviceObjects();
	HRESULT	DeleteDeviceObjects();
	HRESULT	FinalCleanup();	

	///--------------------------------------------------------------
	///	tile
	///--------------------------------------------------------------
	void	RenderTileMap();

	D3DXMATRIX		m_MatScaleTexture0;
	D3DXMATRIX		m_MatScaleTexture1;
	D3DXMATRIX		m_MatTex0;
	D3DXMATRIX		m_MatTex1;

	int				m_EyePatchX;
	int				m_EyePatchY;
	inline	void	GetEyePatch( int* outX, int* outY ) {	*outX = m_EyePatchX;	*outY = m_EyePatchY;	}
	
	///-- By simwoosung 1024 맵을 위해 
    ///-- 현재 캐릭터가 속해있는 Patch 인덱스와 그 인덱스를 중심으로 
	///-- 외곽 한계영역을 계산 그부분만 Update.. Render루틴을 적용시킨다.

	int				m_xCurPatch;
	int				m_zCurPatch;
	int				m_xSrtPatch;
	int				m_zSrtPatch;
	int				m_xEndPatch;
	int				m_zEndPatch;

	CRockResourceManager < CPatch >  m_CPatchPool;

	CPatchMap		m_PatchMap;

	int GetPatchID(int xInx,int zInx);
	int GetXPatchIndex(float fPos);	
	int GetZPatchIndex(float fPos);	
	CPatch* GetPatch(int xInx,int zInx);
};

#endif