#include "RockPCH.h"
#include "RockClient.h"
///#include <windows.h>
///#include <d3d8.h>
///#include <d3dx8.h>

#include "landpatch.h"
#include "landscape.h"

#include "quadlist.h"
#include "MAP/field.h"
#include "map.h"
#include "CRenderManager.h"

#ifdef _DEBUG
int g_LoadPatchNum = 0;
#endif

extern int g_Count;

CPatch::CPatch()
{
	m_pd3dDevice	= NULL;
	m_pVB			= NULL;
	m_pIB			= NULL;
	m_pTileIB		= NULL;
	m_pTileVB		= NULL;

	m_fLODQuality = 1.0f;
	
	m_pIndices		= NULL;

	m_dwLastRenderTime = timeGetTime();
}

CPatch::~CPatch()
{	
}

HRESULT	CPatch::InvalidateDeviceObjects()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pTileIB);
	SAFE_RELEASE(m_pTileVB);
	return	S_OK;
}

HRESULT	CPatch::FinalCleanup()
{
	InvalidateDeviceObjects();
	DeleteDeviceObjects();
	return	S_OK;
}

HRESULT	CPatch::DeleteDeviceObjects()
{
	m_pd3dDevice = NULL;
	return	S_OK;
}

HRESULT	CPatch::RestoreDeviceObjects()
{
	HRESULT hr;

	if ( FAILED ( hr = g_RenderManager.CreateIndexBuffer(MAX_IDX/2*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB) ) )
	{
		Error("Create IB");
		return g_RockClient.DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT, "Error Create IB" );			
	}
	if ( FAILED ( hr = g_RenderManager.CreateVertexBuffer(MAX_VTX*sizeof(FVF_LOD), D3DUSAGE_WRITEONLY, D3DFVF_LOD, D3DPOOL_MANAGED, &m_pVB) ) )
	{
		Error("Create VB");
		return g_RockClient.DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT, "Error Create VB" );		
	}

	LPFVF_LOD pVertices;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0,0,(void**)&pVertices, 0) ) )
#else
	if ( FAILED ( m_pVB->Lock(0,0,(BYTE**)&pVertices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock VB");

	const float	uvoff	= 1.0f/(float)(CLandscape::m_nMapSize+1);

	m_fHeightMax = 0.0f;	
	m_fHeightMin = 1024.0f;

	for ( int y = 0; y < (PATCH_SIZE+1); ++y )
	{
		for ( int x = 0; x < (PATCH_SIZE+1); ++x )
		{
			pVertices[(y*(PATCH_SIZE+1))+x].p = D3DXVECTOR3( (float)(x+m_WorldX)*VERTEX_INTERVAL,
															 (float)(m_HeightMap[(y*CLandscape::m_nPadedMapSize)+x]),
															 (float)(y+m_WorldY)*VERTEX_INTERVAL);

			if( m_fHeightMax < pVertices[(y*(PATCH_SIZE+1))+x].p.y ) 
				m_fHeightMax = pVertices[(y*(PATCH_SIZE+1))+x].p.y;
			if( m_fHeightMin > pVertices[(y*(PATCH_SIZE+1))+x].p.y ) 
				m_fHeightMin = pVertices[(y*(PATCH_SIZE+1))+x].p.y;
		}
	}

	m_pVB->Unlock();
	return	S_OK;
}

HRESULT	CPatch::Error(const char* strError)
{
#ifdef	_DEBUG
	OutputDebugString( strError );
#endif
	return	S_OK;
}

void CPatch::SetLODQuality( int quality )
{
	switch( quality )
	{
	case 0:
	case 1:
			m_fLODQuality = 0.4f;	break;
	case 2:	m_fLODQuality = 0.6f;	break;
	case 3:	m_fLODQuality = 0.8f;	break;
	default : m_fLODQuality = 1.0f;	break;
	}
}

void	CPatch::Split(TriTreeNode *tri)
{
	// We are already split, no need to do it again.
	if (tri->LeftChild)
	{
		return;
	}

	g_Count++;

	// If this triangle is not in a proper diamond, force split our base neighbor
	if ( tri->BaseNeighbor && (tri->BaseNeighbor->BaseNeighbor != tri) )
	{
		Split(tri->BaseNeighbor);
	}

	// Create children and link into mesh
	tri->LeftChild  = CLandscape::AllocateTri();
	tri->RightChild = CLandscape::AllocateTri();

	// If creation failed, just exit.
	if ( !tri->LeftChild )
		return;

	// Fill in the information we can get from the parent (neighbor pointers)
	tri->LeftChild->BaseNeighbor  = tri->LeftNeighbor;
	tri->LeftChild->LeftNeighbor  = tri->RightChild;

	tri->RightChild->BaseNeighbor  = tri->RightNeighbor;
	tri->RightChild->RightNeighbor = tri->LeftChild;

	// Link our Left Neighbor to the new children
	if (tri->LeftNeighbor != NULL)
	{
		if (tri->LeftNeighbor->BaseNeighbor == tri)
			tri->LeftNeighbor->BaseNeighbor = tri->LeftChild;
		else if (tri->LeftNeighbor->LeftNeighbor == tri)
			tri->LeftNeighbor->LeftNeighbor = tri->LeftChild;
		else if (tri->LeftNeighbor->RightNeighbor == tri)
			tri->LeftNeighbor->RightNeighbor = tri->LeftChild;
		else
			;// Illegal Left Neighbor!
	}

	// Link our Right Neighbor to the new children
	if (tri->RightNeighbor != NULL)
	{
		if (tri->RightNeighbor->BaseNeighbor == tri)
			tri->RightNeighbor->BaseNeighbor = tri->RightChild;
		else if (tri->RightNeighbor->RightNeighbor == tri)
			tri->RightNeighbor->RightNeighbor = tri->RightChild;
		else if (tri->RightNeighbor->LeftNeighbor == tri)
			tri->RightNeighbor->LeftNeighbor = tri->RightChild;
		else
			;// Illegal Right Neighbor!
	}

	// Link our Base Neighbor to the new children
	if (tri->BaseNeighbor != NULL)
	{
		if ( tri->BaseNeighbor->LeftChild )
		{
			tri->BaseNeighbor->LeftChild->RightNeighbor = tri->RightChild;
			tri->BaseNeighbor->RightChild->LeftNeighbor = tri->LeftChild;
			tri->LeftChild->RightNeighbor = tri->BaseNeighbor->RightChild;
			tri->RightChild->LeftNeighbor = tri->BaseNeighbor->LeftChild;
		}
		else
		{
			Split( tri->BaseNeighbor);  // Base Neighbor (in a diamond with us) was not split yet, so do that now.
		}
	}
	else
	{
		// An edge triangle, trivial case.
		tri->LeftChild->RightNeighbor = NULL;
		tri->RightChild->LeftNeighbor = NULL;
	}
}

void	CPatch::RecursTessellate(TriTreeNode* tri, int leftX, int leftY, int rightX, int rightY, int apeX, int apeY, int node)
{
	int	centerX	= (leftX+rightX)>>1;
	int	centerY	= (leftY+rightY)>>1;

	bool isBorderTri = false;
	
	if( GetNearDegree() == 1 )		/// 근거리와 원거리 사이의 경계 patch 
	{
		isBorderTri = true;

		///-- 근거리와 원거리 사이의 경계 해당 삼각형이 근거리 타일링 영역 cell index min, max
		///-- 의 내에 들어오게되면... isBorderTri 가 true
		///-- 그렇지 않으면 isBorderTri 가 false
		do
		{
			if( ( leftX >= CLandscape::m_NearPatchWorldIdxMinX ) && ( leftX <= CLandscape::m_NearPatchWorldIdxMaxX ) && 
				( leftY >= CLandscape::m_NearPatchWorldIdxMinY ) && ( leftY <= CLandscape::m_NearPatchWorldIdxMaxY ) ) break;

			if( ( rightX >= CLandscape::m_NearPatchWorldIdxMinX ) && ( rightX <= CLandscape::m_NearPatchWorldIdxMaxX ) && 
				( rightY >= CLandscape::m_NearPatchWorldIdxMinY ) && ( rightY <= CLandscape::m_NearPatchWorldIdxMaxY ) ) break;

			if( ( apeX >= CLandscape::m_NearPatchWorldIdxMinX ) && ( apeX <= CLandscape::m_NearPatchWorldIdxMaxX ) && 
				( apeY >= CLandscape::m_NearPatchWorldIdxMinY ) && ( apeY <= CLandscape::m_NearPatchWorldIdxMaxY ) ) break;

			if( ( centerX >= CLandscape::m_NearPatchWorldIdxMinX ) && ( centerX <= CLandscape::m_NearPatchWorldIdxMaxX ) && 
				( centerY >= CLandscape::m_NearPatchWorldIdxMinY ) && ( centerY <= CLandscape::m_NearPatchWorldIdxMaxY ) ) break;

			isBorderTri = false;

		} while( isBorderTri );
	}

	///-- 경계 삼각형이면 더 세밀하게 분할
	if( isBorderTri ) 
	{
		Split(tri);

		if ( tri->LeftChild &&											// If this triangle was split, try to split it's children as well.
			((abs(leftX-rightX) >= 2 ) || (abs(leftY-rightY) >= 2 )) )	// Tessellate all the way down to one vertex per height field entry
		{
			RecursTessellate(tri->LeftChild, apeX, apeY, leftX, leftY, centerX, centerY, node<<1);
			RecursTessellate(tri->RightChild, rightX, rightY, apeX, apeY, centerX, centerY, 1+(node<<1));
		}
	}
	else 
	{
		float TriVariance;
		float distance;

		///-- 마지막 깊이노드가 아닐경우
		if( node < ( 1 << VARIANCE_DEPTH ) )
		{
			distance = 1.0f + ( ( abs( ( centerX * VERTEX_INTERVAL ) - m_EyeX ) +
								  abs( ( centerY * VERTEX_INTERVAL ) - m_EyeY ) ) );

			TriVariance	= ( (float)( m_CurrentVariance[node] * CLandscape::m_nMapSize * 2 ) / 
				          ( distance * 0.2f / VERTEX_INTERVAL ) ) * m_fLODQuality ;
		}

		///-- 해당노드를 위한 편차값을얻지 못한다면.. 분할을 통해 이것을 얻어야만한다.
		///-- 계속해서 최저레벨까지 내려가자.
		///-- 편차트리 아래 있지않다면 편차값을 테스트 해보자.
		if ( ( node >= ( 1 << VARIANCE_DEPTH ) ) ||				// IF we do not have variance info for this node, then we must have gotten here by splitting, so continue down to the lowest level.
			 ( TriVariance > CLandscape::m_fFrameVariance ) )	// OR if we are not below the variance tree, test for variance.
		{
			Split(tri);

			if ( tri->LeftChild &&											// If this triangle was split, try to split it's children as well.
				((abs(leftX-rightX) >= 4 ) || (abs(leftY-rightY) >= 4 )) )	// Tessellate all the way down to one vertex per height field entry
			{
				RecursTessellate(tri->LeftChild, apeX, apeY, leftX, leftY, centerX, centerY, node<<1);
				RecursTessellate(tri->RightChild, rightX, rightY, apeX, apeY, centerX, centerY, 1+(node<<1));
			}
		}
	}
}

void	CPatch::RecursRender(TriTreeNode* tri, int leftX, int leftY, int rightX, int rightY, int apexX, int apexY)
{
	if ( tri->LeftChild )
	{
		int	centerX	= (leftX+rightX)>>1;
		int	centerY	= (leftY+rightY)>>1;

		RecursRender(tri->LeftChild, apexX, apexY, leftX, leftY, centerX, centerY);
		RecursRender(tri->RightChild, rightX, rightY, apexX, apexY, centerX, centerY);
	}
	else
	{
#ifdef	_DEBUG
		++( CLandscape::m_nNumTrisRendered );
#endif

		m_pIndices[m_nNumIndices++]	= (unsigned short)(leftY*(PATCH_SIZE+1))+leftX;
		m_pIndices[m_nNumIndices++]	= (unsigned short)(rightY*(PATCH_SIZE+1))+rightX;
		m_pIndices[m_nNumIndices++]	= (unsigned short)(apexY*(PATCH_SIZE+1))+apexX;
	}
}

unsigned char	CPatch::RecursComputeVariance(	int leftX,  int leftY,  int leftZ,
												int rightX, int rightY, int rightZ,
												int apexX,  int apexY,  int apexZ,
												int node)
{
	//        /|\
	//      /  |  \
	//    /    |    \
	//  /      |      \
	//  ~~~~~~~*~~~~~~~  <-- Compute the X and Y coordinates of '*'
	//
	int	centerX	= ((leftX+rightX)) >>1;		// Compute X coordinate of center of Hypotenuse
	int	centerY	= ((leftY+rightY)) >>1;		// Compute Y coord...
	unsigned char	myVariance;

	// Get the height value at the middle of the Hypotenuse
	int	centerZ	= (int)( m_HeightMap[(centerY*CLandscape::m_nPadedMapSize)+centerX] );

	// Variance of this triangle is the actual height at it's hypotenuse midpoint minus the interpolated height.
	// Use values passed on the stack instead of re-accessing the Height Field.
	myVariance	= abs((int)centerZ-(((int)leftZ+(int)rightZ)>>1));

	// Since we're after speed and not perfect representations,
	//    only calculate variance down to an 8x8 block
	if ( (abs(leftX-rightX) >= 8 )||
		 (abs(leftY-rightY) >= 8 ) )
	{
		// Final Variance for this node is the max of it's own variance and that of it's children.
		myVariance	= MAX(myVariance, RecursComputeVariance(apexX,apexY,apexZ,leftX,leftY,leftZ,centerX,centerY,centerZ,node<<1));
		myVariance	= MAX(myVariance, RecursComputeVariance(rightX,rightY,rightZ,apexX,apexY,apexZ,centerX,centerY,centerZ,1+(node<<1)));
	}

	// Store the final variance for this node.  Note Variance is never zero.
	if ( node < (1<<VARIANCE_DEPTH) )
		m_CurrentVariance[node]	= 1+myVariance;

	return	myVariance;
}

#ifdef DIRECT_VERSION_9_MJH
void	CPatch::Create(LPDIRECT3DDEVICE9 pd3dDev, int heightX, int heightY, int worldX, int worldY, float* hMap, CCamera* pCamera)
#else
void	CPatch::Create(LPDIRECT3DDEVICE8 pd3dDev, int heightX, int heightY, int worldX, int worldY, float* hMap, CCamera* pCamera)
#endif // DIRECT_VERSION_9_MJH
{
	m_pd3dDevice = pd3dDev;
	m_pCamera = pCamera;

	///
	m_BaseLeft.RightNeighbor = m_BaseLeft.LeftNeighbor = m_BaseRight.RightNeighbor = m_BaseRight.LeftNeighbor =
	m_BaseLeft.LeftChild = m_BaseLeft.RightChild = m_BaseRight.LeftChild = m_BaseLeft.LeftChild = NULL;

	// Attach the two m_Base triangles together
	m_BaseLeft.BaseNeighbor = &m_BaseRight;
	m_BaseRight.BaseNeighbor = &m_BaseLeft;

	// Store Patch offsets for the world and heightmap.
	m_WorldX = worldX;
	m_WorldY = worldY;

	// Store pointer to first byte of the height data for this patch.
	m_HeightMap = &hMap[heightY * CLandscape::m_nPadedMapSize + heightX];

	// Initialize flags
	m_VarianceDirty = 1;
	m_isVisible = 0;
}

void	CPatch::Reset( int eyeX, int eyeY )
{
	// Assume patch is not visible.
	m_isVisible = 0;

	// Reset the important relationships
	m_BaseLeft.LeftChild = m_BaseLeft.RightChild = m_BaseRight.LeftChild = m_BaseLeft.LeftChild = NULL;

	// Attach the two m_Base triangles together
	m_BaseLeft.BaseNeighbor = &m_BaseRight;
	m_BaseRight.BaseNeighbor = &m_BaseLeft;

	// Clear the other relationships.
	m_BaseLeft.RightNeighbor = m_BaseLeft.LeftNeighbor = m_BaseRight.RightNeighbor = m_BaseRight.LeftNeighbor = NULL;

	///-- 시야 위치를 눈의 셀좌표로 변환한다.
	int eyePatchX = eyeX / VERTEX_INTERVAL / PATCH_SIZE;
	int eyePatchY = eyeY / VERTEX_INTERVAL / PATCH_SIZE;

	///-- 시야 위치가 패치의 외곽영역을 벗어났을때...
	if( eyePatchX < 0 || eyePatchX >= CLandscape::m_nNumPatches || 
		eyePatchY < 0 || eyePatchY >= CLandscape::m_nNumPatches )
	{
		SetNearDegree( 2 );		/// 원거리
	}
	else
	{
		int patchX = m_WorldX / PATCH_SIZE;
		int patchY = m_WorldY / PATCH_SIZE;

		int x = abs( eyePatchX - patchX );
		int y = abs( eyePatchY - patchY );
		
		if( x <= CLandscape::m_nLODQuality && y <= CLandscape::m_nLODQuality )
		{
			SetNearDegree( 0 );		/// 근거리 
		}
		else if( x <= ( CLandscape::m_nLODQuality + 1 ) && y <= ( CLandscape::m_nLODQuality + 1 ) )
		{
			SetNearDegree( 1 );		/// 경계 patch
		}
		else
		{
			SetNearDegree( 2 );		/// 원거리
		}
	}

	m_EyeX = eyeX;
	m_EyeY = eyeY;
}

void	CPatch::ComputeVariance()
{
	int		nPatchSize	= PATCH_SIZE;

	m_CurrentVariance	= m_VarianceLeft;
	RecursComputeVariance(	0, nPatchSize, (int)m_HeightMap[nPatchSize*CLandscape::m_nPadedMapSize],
							nPatchSize, 0, (int)m_HeightMap[nPatchSize],
							0,0, (int)m_HeightMap[0],
							1);
	m_CurrentVariance	= m_VarianceRight;
	RecursComputeVariance(	nPatchSize, 0, (int)m_HeightMap[nPatchSize],
							0, nPatchSize, (int)m_HeightMap[nPatchSize*CLandscape::m_nPadedMapSize],
							nPatchSize, nPatchSize, (int)m_HeightMap[(nPatchSize*CLandscape::m_nPadedMapSize)+nPatchSize],
							1);

	m_VarianceDirty	= 0;
}

void	CPatch::SetVisibility()
{
	float h = ( m_fHeightMax - m_fHeightMin ) * 0.5f;

	m_isVisible	= m_pCamera->CubeInFrustum( (float)((m_WorldX+PATCH_SIZE/2)*VERTEX_INTERVAL),
											 m_fHeightMin + h,
											 (float)((m_WorldY+PATCH_SIZE/2)*VERTEX_INTERVAL), 
											 (float)((PATCH_SIZE/2)*VERTEX_INTERVAL),
											 h,
											 (float)((PATCH_SIZE/2)*VERTEX_INTERVAL) );
}

void	CPatch::Tessellate()
{
	if( GetNearDegree() == 0 ) return;

	int	nPatchSize	= PATCH_SIZE;
	m_CurrentVariance	= m_VarianceLeft;
	RecursTessellate( &m_BaseLeft,
						m_WorldX, m_WorldY+nPatchSize,
						m_WorldX+nPatchSize, m_WorldY,
						m_WorldX, m_WorldY,
						1);
	m_CurrentVariance	= m_VarianceRight;
	RecursTessellate( &m_BaseRight,
						m_WorldX+nPatchSize, m_WorldY,
						m_WorldX, m_WorldY+nPatchSize,
						m_WorldX+nPatchSize, m_WorldY+nPatchSize,
						1);
}

void	CPatch::Update()
{
	if( GetNearDegree() == 0 ) return;

	int	nPatchSize	= PATCH_SIZE;
	m_nNumIndices	= 0;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0,0,(void**)&m_pIndices, 0) ) )
#else
	if ( FAILED ( m_pIB->Lock(0,0,(BYTE**)&m_pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		Error("Lock IB");

	RecursRender(&m_BaseLeft, 0, nPatchSize, nPatchSize, 0, 0, 0);
	RecursRender(&m_BaseRight, nPatchSize, 0, 0, nPatchSize, nPatchSize, nPatchSize);
	
	m_pIB->Unlock();
}

void	CPatch::Render()
{
	if( GetNearDegree() == 0 ) return;

	g_RenderManager.SetStreamSource(0, m_pVB, sizeof(FVF_LOD));
	
	g_RenderManager.SetFVF( D3DFVF_LOD );
	g_RenderManager.SetIndices(m_pIB, 0);
	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, (PATCH_SIZE+1)*(PATCH_SIZE+1), 0, m_nNumIndices/3);
}

///--------------------------------------------------------------
///	tile
///--------------------------------------------------------------

void	CPatch::CreateTileMap( WORD* pTexIdxList )
{
	/// 해당 patch의 텍스쳐 정보 시작점 
	m_pTexIdxList = &pTexIdxList[m_WorldY * CLandscape::m_nMapSize + m_WorldX];
}

const float	TILE_UV[14][4][2]	= 
{
	{{0.01f,0.01f},{0.99f,0.01f},{0.99f,0.99f},{0.01f,0.99f}},	//TT_LEFT		
	{{0.01f,0.99f},{0.01f,0.01f},{0.99f,0.01f},{0.99f,0.99f}},	//TT_TOP		
	{{0.99f,0.99f},{0.01f,0.99f},{0.01f,0.01f},{0.99f,0.01f}},	//TT_RIGHT	
	{{0.99f,0.01f},{0.99f,0.99f},{0.01f,0.99f},{0.01f,0.01f}},	//TT_BOTTOM	

	{{0.01f,0.99f},{0.01f,0.01f},{0.99f,0.01f},{0.99f,0.99f}},	//TT_TLEFT	
	{{0.99f,0.99f},{0.99f,0.01f},{0.01f,0.01f},{0.01f,0.99f}},	//TT_TRIGHT	
	{{0.01f,0.01f},{0.01f,0.99f},{0.99f,0.99f},{0.99f,0.01f}},	//TT_BLEFT	
	{{0.99f,0.01f},{0.99f,0.99f},{0.01f,0.99f},{0.01f,0.01f}},	//TT_BRIGHT	
	
	{{0.01f,0.99f},{0.01f,0.01f},{0.99f,0.01f},{0.99f,0.99f}},	//TT_TILEFT	
	{{0.99f,0.99f},{0.99f,0.01f},{0.01f,0.01f},{0.01f,0.99f}},	//TT_TIRIGHT	
	{{0.01f,0.01f},{0.01f,0.99f},{0.99f,0.99f},{0.99f,0.01f}},	//TT_BILEFT	
	{{0.99f,0.01f},{0.99f,0.99f},{0.01f,0.99f},{0.01f,0.01f}},	//TT_BIRIGHT	
	
	{{0.01f,0.99f},{0.01f,0.01f},{0.99f,0.01f},{0.99f,0.99f}},	//TT_CENTER
	{{0.01f,0.99f},{0.01f,0.01f},{0.99f,0.01f},{0.99f,0.99f}},	//TT_DEFAULT
};


HRESULT	CPatch::RestoreDeviceObjectsTileMap()
{

#ifdef _DEBUG
	++g_LoadPatchNum;
///	RLG3( "+++ [%d] RESTORE tile vb, ib. (%d, %d)\n", g_LoadPatchNum, m_WorldX/PATCH_SIZE, m_WorldY/PATCH_SIZE );
#endif

	ZeroMemory( &m_nTileRenderInfo, sizeof( STileRenderInfo )*1024 );

	/// 타일링은 텍스쳐 UV를 공유할 수 없으므로 TRIANGLELIST 로 그립니다.
	if ( FAILED ( g_RenderManager.CreateIndexBuffer( MAX_IDX * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
													 D3DPOOL_MANAGED, &m_pTileIB) ) )
	{
		return	Error("Create Tile IB");
	}

	if( FAILED ( g_RenderManager.CreateVertexBuffer( 4 * MAX_VTX * sizeof(FVF_TILE), D3DUSAGE_WRITEONLY, D3DFVF_TILE, 
													 D3DPOOL_MANAGED, &m_pTileVB) ) )
	{
		return Error("Create Tile VB");
	}

	LPFVF_TILE	pVertices;
	WORD*		pIndices;

	DWORD	dwVtxCount = 0;
	DWORD	dwIdxCount = 0;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pTileIB->Lock(0, 0, (void**)&pIndices, 0) ) )
#else
	if( FAILED ( m_pTileIB->Lock(0, 0, (BYTE**)&pIndices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock Tile VB");
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pTileVB->Lock(0, 0, (void**)&pVertices, 0) ) )
#else
	if( FAILED ( m_pTileVB->Lock(0, 0, (BYTE**)&pVertices, 0) ) )
#endif // DIRECT_VERSION_9_MJH
		return	Error("Lock Tile VB");

	const float	uvoffset = 1.0f / (float)( CLandscape::m_nMapSize + 1 );

	int x, y, i;

	/// sooree ter opt
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 pTileTex;
#else
	LPDIRECT3DTEXTURE8 pTileTex;
#endif // DIRECT_VERSION_9_MJH
	int nTileGroupOut;
	int nTileTypeIdxOut; 
	int nTileBlendSeqOut;
	int nSubTileOut;

	/// 타일 종류 추출 
	m_nTileRenderInfoNum = 0;

	for( y = 0; y < (PATCH_SIZE); ++y )
	{
		for( x = 0; x < (PATCH_SIZE); ++x )
		{
			/// sooree ter opt
			pTileTex = g_TileTexture.GetTileTexture( m_pTexIdxList[ ( y * CLandscape::m_nMapSize ) + x ], 
													 nTileGroupOut,
													 nTileTypeIdxOut,
													 nTileBlendSeqOut,
													 nSubTileOut );

			for( i = 0; i < m_nTileRenderInfoNum; ++i )
			{
				if( pTileTex == m_nTileRenderInfo[i].m_dpTileTex ) break;
			}

			if( i == m_nTileRenderInfoNum ) /// 새로 나온 타일 
			{
				m_nTileRenderInfo[m_nTileRenderInfoNum].m_dpTileTex = pTileTex;
				++m_nTileRenderInfoNum;
			}
		}
	}

	for( i = 0; i < m_nTileRenderInfoNum ; ++i )
	{
		for ( int y = 0; y < (PATCH_SIZE) ; ++y )
		{
			for ( int x = 0; x < (PATCH_SIZE) ; ++x )
			{
				/// sooree ter opt
				pTileTex = g_TileTexture.GetTileTexture( m_pTexIdxList[( y * CLandscape::m_nMapSize ) + x ], 
														 nTileGroupOut,
														 nTileTypeIdxOut,
														 nTileBlendSeqOut,
														 nSubTileOut );

				if( pTileTex != m_nTileRenderInfo[i].m_dpTileTex )
				{
					continue;
				}

				pVertices[dwVtxCount].p = D3DXVECTOR3(	(float)(x+m_WorldX)*VERTEX_INTERVAL,
														(float)(m_HeightMap[(y*CLandscape::m_nPadedMapSize)+x]),
														(float)(y+m_WorldY)*VERTEX_INTERVAL);
				pVertices[dwVtxCount].t2.x = TILE_UV[nTileTypeIdxOut][0][0] * 0.125f + nTileBlendSeqOut * 0.125f;
				pVertices[dwVtxCount].t2.y = TILE_UV[nTileTypeIdxOut][0][1] * 0.5f + nSubTileOut * 0.5f;
				++dwVtxCount;

				pVertices[dwVtxCount].p = D3DXVECTOR3(	(float)(x+m_WorldX)*VERTEX_INTERVAL,
														(float)(m_HeightMap[((y+1)*CLandscape::m_nPadedMapSize)+x]),
														(float)((y+1)+m_WorldY)*VERTEX_INTERVAL);
				pVertices[dwVtxCount].t2.x = TILE_UV[nTileTypeIdxOut][1][0] * 0.125f + nTileBlendSeqOut * 0.125f;
				pVertices[dwVtxCount].t2.y = TILE_UV[nTileTypeIdxOut][1][1] * 0.5f + nSubTileOut * 0.5f;
				++dwVtxCount;

				pVertices[dwVtxCount].p = D3DXVECTOR3(	(float)((x+1)+m_WorldX)*VERTEX_INTERVAL,
														(float)(m_HeightMap[((y+1)*CLandscape::m_nPadedMapSize)+(x+1)]),
														(float)((y+1)+m_WorldY)*VERTEX_INTERVAL);
				pVertices[dwVtxCount].t2.x = TILE_UV[nTileTypeIdxOut][2][0] * 0.125f + nTileBlendSeqOut * 0.125f;
				pVertices[dwVtxCount].t2.y = TILE_UV[nTileTypeIdxOut][2][1] * 0.5f + nSubTileOut * 0.5f;
				++dwVtxCount;

				pVertices[dwVtxCount].p = D3DXVECTOR3(	(float)((x+1)+m_WorldX)*VERTEX_INTERVAL,
														(float)(m_HeightMap[(y*CLandscape::m_nPadedMapSize)+(x+1)]),
														(float)(y+m_WorldY)*VERTEX_INTERVAL);
				pVertices[dwVtxCount].t2.x = TILE_UV[nTileTypeIdxOut][3][0] * 0.125f + nTileBlendSeqOut * 0.125f;
				pVertices[dwVtxCount].t2.y = TILE_UV[nTileTypeIdxOut][3][1] * 0.5f + nSubTileOut * 0.5f;
				++dwVtxCount;

				pIndices[dwIdxCount] = (WORD)dwVtxCount-4;
				++dwIdxCount;
				pIndices[dwIdxCount] = (WORD)dwVtxCount-3;
				++dwIdxCount;
				pIndices[dwIdxCount] = (WORD)dwVtxCount-2;
				++dwIdxCount;
				pIndices[dwIdxCount] = (WORD)dwVtxCount-2;
				++dwIdxCount;
				pIndices[dwIdxCount] = (WORD)dwVtxCount-1;
				++dwIdxCount;
				pIndices[dwIdxCount] = (WORD)dwVtxCount-4;
				++dwIdxCount;

				++m_nTileRenderInfo[i].nTileNum;
			}
		}
	}

	m_pTileVB->Unlock();
	m_pTileIB->Unlock();

	m_dwLastRenderTime = timeGetTime();

	return S_OK;
}

void	CPatch::RenderTileMap()
{
	/// 근거리와 원거리 사이의 경계 patch : Frustum 안에 들어와서 RenderTileMap() 이 호출된 것이지. 
	///	이경우 이동 경로일 가능성이 크므로 cache 개념으로 미리 로딩해 놓는다. 
	if( GetNearDegree() == 1 )
	{
		if( m_pTileVB == NULL )
		{
			/// Restore tile vb, ib
			RestoreDeviceObjectsTileMap();		
		}
		
		/// 경계 patch가 Frustum 안에 있다면 cache를 유지하도록 tick을 Update 해준다. 
		m_dwLastRenderTime = timeGetTime();
		return;
	}
	else if( GetNearDegree() != 0 )
	{
		return;
	}

	if( m_pTileVB == NULL )
	{
		/// Restore tile vb, ib
		if( S_OK != RestoreDeviceObjectsTileMap() )
		{
			return;
		}
	}	

	m_dwLastRenderTime = timeGetTime();

	g_RenderManager.SetStreamSource( 0, m_pTileVB, sizeof(FVF_TILE) );	
	g_RenderManager.SetFVF( D3DFVF_TILE );
	g_RenderManager.SetIndices( m_pTileIB, 0 );

	int	curIdx = 0;
	int curVtx = 0;	

	for( int i = 0; i < m_nTileRenderInfoNum; ++i )
	{		
		/// sooree ter opt 
#ifdef	_DEBUG
		if(  GetPrivateProfileInt( "Texture", "TileTex_Skip", 0, "./dev.ini") )
		{
			g_RenderManager.SetTexture( 0, NULL );
		}
		else
		{
			g_RenderManager.SetTexture( 0, m_nTileRenderInfo[i].m_dpTileTex );
		}
#else
		g_RenderManager.SetTexture( 0, m_nTileRenderInfo[i].m_dpTileTex );
#endif		
		
		
		
		g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
											curVtx, 
											m_nTileRenderInfo[i].nTileNum * 4, 
											curIdx, 
											m_nTileRenderInfo[i].nTileNum * 2 );

		curVtx += m_nTileRenderInfo[i].nTileNum * 4;
		curIdx += m_nTileRenderInfo[i].nTileNum * 6;

#ifdef	_DEBUG
		++( CLandscape::m_nNumTileRenderCall );
#endif
	}

}

void	CPatch::CacheTimeout( DWORD curTime )
{
	if( m_pTileVB )
	{
		if( curTime > m_dwLastRenderTime + CACHE_TIME_OUT )
		{
#ifdef _DEBUG
			--g_LoadPatchNum;
///			RLG3( "--- [%d] RELEASE tile vb, ib. (%d, %d)\n", g_LoadPatchNum, m_WorldX/PATCH_SIZE, m_WorldY/PATCH_SIZE );
#endif
			/// release tile vb, ib
			SAFE_RELEASE(m_pTileIB);
			SAFE_RELEASE(m_pTileVB);
		}
	}
}

