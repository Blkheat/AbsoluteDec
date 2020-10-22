#include "RockPCH.h"
///#include <windows.h>
///#include <d3d8.h>
///#include <d3dx8.h>
///#include <stdio.h>

#include "landscape.h"

#include "quadlist.h"
#include "MAP/field.h"
#include "map.h"
#include "CRenderManager.h"
#include "profile\\profile.h"

TriTreeNode	CLandscape::m_TriPool[MAX_TRIPOOL];
int			CLandscape::m_nNextTriNode;
int			CLandscape::m_nMapSize = 0;
int			CLandscape::m_nPadedMapSize = 0;
float		CLandscape::m_fFrameVariance;

#ifdef	_DEBUG
int			CLandscape::m_nNumTrisRendered;
int			CLandscape::m_nNumTileRenderCall;
#endif

int			CLandscape::m_nDesiredTris;
int			CLandscape::m_nNumPatches;
int			CLandscape::m_nLODQuality = 3;

int			CLandscape::m_NearPatchWorldIdxMinX;
int			CLandscape::m_NearPatchWorldIdxMinY;
int			CLandscape::m_NearPatchWorldIdxMaxX;
int			CLandscape::m_NearPatchWorldIdxMaxY;

int g_Count = 0;

CLandscape::CLandscape()
{
	m_nNextTriNode		= 0;
	m_fFrameVariance	= 50;

#ifdef	_DEBUG
	m_nRenderedPatches = 0;
	m_nNumTrisRendered = 0;
	m_nNumTileTrisRendered = 0;
	m_nNumTilePatches = 0;
	m_nNumTileRenderCall = 0;
#endif

	m_nDesiredTris		= 100;
	m_bAccessEnable		= false;

	m_Patches	= NULL;
	m_HeightMap = NULL;

	m_pTileIndexList = NULL;

	m_xCurPatch = 0;
	m_zCurPatch = 0;
	m_xSrtPatch = 0;
	m_zSrtPatch = 0;
	m_xEndPatch = 0;
	m_zEndPatch = 0;

#ifdef APPLYWIDE_MAP

	m_CPatchPool.Reserve( 81 );

#endif

}

CLandscape::~CLandscape()
{
	FinalCleanup();
}

HRESULT	CLandscape::RestoreDeviceObjects()
{

	CPatch * pPatch = NULL;

#ifdef APPLYWIDE_MAP	
	
	for( CPatchMap_i it = m_PatchMap.begin() ; it != m_PatchMap.end() ; ++it )
	{
		(*it).second->RestoreDeviceObjects();		
	}

#else

	for ( int y = 0; y < m_nNumPatches; ++y )
	{
		for ( int x = 0; x < m_nNumPatches; ++x )
		{
			pPatch	= &(m_Patches[y][x]);			
			pPatch->RestoreDeviceObjects();
///			m_Patches[y][x].RestoreDeviceObjectsTileMap();	/// Render에서 실시간으로 Restore한다. 
		}
	}

#endif

	return	S_OK;
}

HRESULT	CLandscape::InvalidateDeviceObjects()
{
	CPatch * pPatch = NULL;

#ifdef APPLYWIDE_MAP	
	
	for( CPatchMap_i it = m_PatchMap.begin() ; it != m_PatchMap.end() ; ++it )
	{
		(*it).second->InvalidateDeviceObjects();		
	}
		
#else	
	
	for ( int y = 0; y < m_nNumPatches; ++y )
	{
		for ( int x = 0; x < m_nNumPatches; ++x )
		{
			pPatch	= &(m_Patches[y][x]);			
			pPatch->InvalidateDeviceObjects();
		}
	}

#endif

	return	S_OK;
}

HRESULT	CLandscape::DeleteDeviceObjects()
{
	CPatch * pPatch = NULL;

#ifdef APPLYWIDE_MAP

	for( CPatchMap_i it = m_PatchMap.begin() ; it != m_PatchMap.end() ; ++it )
	{
		(*it).second->DeleteDeviceObjects();		
	}

#else	
	
	for ( int y = 0; y < m_nNumPatches; ++y )
	{
		for ( int x = 0; x < m_nNumPatches; ++x )
		{
			pPatch	= &(m_Patches[y][x]);
			pPatch->DeleteDeviceObjects();
		}
	}

#endif

	return	S_OK;
}

HRESULT	CLandscape::FinalCleanup()
{
	
#ifdef APPLYWIDE_MAP
		
	CPatch	*	pPatch = NULL;
	
	for( CPatchMap_i it = m_PatchMap.begin() ; it != m_PatchMap.end() ; ++it )
	{
		pPatch = (CPatch *)( (*it).second );
		pPatch->FinalCleanup();
		///SAFE_DELETE( pPatch );
		m_CPatchPool.DeleteMember( pPatch );
	}

	m_PatchMap.clear();

#else	
	
	if( m_Patches )
	{
		for ( int y = 0; y < m_nNumPatches; ++y )
		{
			for ( int x = 0; x < m_nNumPatches; ++x )
			{
				m_Patches[y][x].FinalCleanup();
			}
		}

		for (int y = 0; y < m_nNumPatches; ++y )
		{
			SAFE_DELETE_ARRAY(m_Patches[y]);
		}

		SAFE_DELETE_ARRAY(m_Patches);
	}

#endif

	if( m_HeightMap )
	{
		SAFE_DELETE_ARRAY(m_HeightMap);
	}

	if( m_pTileIndexList )
	{
		SAFE_DELETE_ARRAY(m_pTileIndexList);
	}

	return	S_OK;
}

void	CLandscape::SetLODQuality( int quality )
{	
	m_nLODQuality = quality + 1;	

#ifdef APPLYWIDE_MAP
	
	for( CPatchMap_i it = m_PatchMap.begin() ; it != m_PatchMap.end() ; ++it )
	{
		(*it).second->SetLODQuality( m_nLODQuality );
	}

#else

	if( m_Patches )
	{
		for ( int y = 0 ; y < m_nNumPatches; ++y )
		{
			for ( int x = 0; x < m_nNumPatches; ++x )
			{
				( m_Patches[y][x] ).SetLODQuality( m_nLODQuality );
			}
		}
	}

#endif
}

TriTreeNode	*CLandscape::AllocateTri()
{
	TriTreeNode	*pTri;	

	if ( m_nNextTriNode >= MAX_TRIPOOL )
	{
		return	NULL;
	}

	pTri	= &(m_TriPool[m_nNextTriNode++]);
	pTri->LeftChild = pTri->RightChild	= NULL;

	return	pTri;
}

#define ADJUST_SCALE_Y		0.055f
#define	HF_HEADER_STRING	"binary_heightfield:shorts"

bool	CLandscape::LoadFileHF( const char* pszFileName )
{
	float headerInfos[5];

	FILE* fp = fopen( pszFileName, "rb" );
	if ( fp == NULL )
	{
		return( false );
	}

	char headerString[512];
	fread( headerString, sizeof( char ), 512, fp );
	if( lstrcmp( headerString, HF_HEADER_STRING ) != 0 )
	{
		return( false );
	}

	fscanf( fp, "%f %f %f %f %f",	&( headerInfos[0] ),		/// width 
									&( headerInfos[1] ),		/// height
									&( headerInfos[2] ),		/// x 0
									&( headerInfos[3] ),		/// z 0
									&( headerInfos[4] ) );		/// somthing... /// 32767.00000

	if( ( int ) headerInfos[0] != ( int ) headerInfos[1] )
	{
		return( false );
	}

	m_nMapSize = ( int ) headerInfos[0];

	/// 지원하는 맵 크기인지 체크 
	if( m_nMapSize != 64 && m_nMapSize != 128 && m_nMapSize != 256 && m_nMapSize != 512 && m_nMapSize != 1024 )
	{
		fclose( fp );
		return( false );
	}

	// Optimization:  Add an extra row and column
	m_nPadedMapSize = m_nMapSize + 1;

	if( m_HeightMap != NULL ) SAFE_DELETE_ARRAY(m_HeightMap);
	m_HeightMap = SAFE_NEW_ARRAY( float, m_nPadedMapSize * m_nPadedMapSize );	
	
	ZeroMemory( m_HeightMap, sizeof( float ) * ( m_nPadedMapSize * m_nPadedMapSize )  );

	fseek( fp, 1, SEEK_CUR );

	short hfdata;
	int z = 0;
	for( z = 0 ; z < m_nMapSize ; ++z )
	{
		int x = 0;
		for( x = 0 ; x < m_nMapSize ; ++x )
		{
			fread( &hfdata, sizeof( short ), 1, fp );
			m_HeightMap[ z * m_nPadedMapSize + x] = ( float ) hfdata * ADJUST_SCALE_Y;
		}
		/// fill the extra column
		m_HeightMap[z * m_nPadedMapSize + x] = m_HeightMap[z * m_nPadedMapSize + ( x - 1 )];	
	}
	/// fill the extra raw
	memcpy( &( m_HeightMap[z * m_nPadedMapSize] ), 
			&( m_HeightMap[(z - 1) * m_nPadedMapSize] ),
			sizeof( float ) * m_nPadedMapSize );

	fclose( fp );

	return( true );
}
#ifdef DIRECT_VERSION_9_MJH
void	CLandscape::CreateTerrain(	LPDIRECT3DDEVICE9 pd3dDev, CCamera* pCamera, int nDesiredTri,
#else
void	CLandscape::CreateTerrain(	LPDIRECT3DDEVICE8 pd3dDev, CCamera* pCamera, int nDesiredTri,
#endif // DIRECT_VERSION_9_MJH 
									int nMapSize, short* pHeightList, WORD* pTexIdxList )
{
	m_pd3dDevice = pd3dDev;
	m_pCamera = pCamera;
	m_nMapSize = nMapSize;
	m_nPadedMapSize = m_nMapSize + 1;

	if( m_HeightMap != NULL ) SAFE_DELETE_ARRAY(m_HeightMap);
	m_HeightMap = SAFE_NEW_ARRAY( float , m_nPadedMapSize * m_nPadedMapSize );	
	
	ZeroMemory( m_HeightMap, sizeof( float ) * ( m_nPadedMapSize * m_nPadedMapSize )  );

	for( int i = 0 ; i < m_nPadedMapSize * m_nPadedMapSize ; ++i )
	{
		m_HeightMap[i] = ( float ) pHeightList[i];
	}

	/// 텍스처 UV 하드웨어 가속을 위한 텍스쳐 매트릭스 설정 
	ZeroMemory( &m_MatScaleTexture0, sizeof( m_MatScaleTexture0 ) );
	m_MatScaleTexture0._11 = 1.0f/ ( VERTEX_INTERVAL * CLandscape::m_nMapSize );
	m_MatScaleTexture0._32 = -1.0f/ ( VERTEX_INTERVAL * CLandscape::m_nMapSize ); /// TGA flip 때문에 -1 

	ZeroMemory( &m_MatScaleTexture1, sizeof( m_MatScaleTexture1 ) );
	m_MatScaleTexture1._11 = 1.0f/ VERTEX_INTERVAL;
	m_MatScaleTexture1._32 = 1.0f/ VERTEX_INTERVAL;

	/// tile patch 실시간 restore를 위해 기억 
	if( m_pTileIndexList != NULL ) SAFE_DELETE_ARRAY(m_pTileIndexList);
	m_pTileIndexList = SAFE_NEW_ARRAY( WORD , nMapSize*nMapSize );
	
	memcpy( m_pTileIndexList, pTexIdxList, sizeof(WORD)*nMapSize*nMapSize );

	///-- CPatch의 생성 구성 초기화 
	Create( pd3dDev, pCamera, nDesiredTri, m_pTileIndexList );
}

#ifdef DIRECT_VERSION_9_MJH
void	CLandscape::Create(LPDIRECT3DDEVICE9 pd3dDev, CBravoCamera* pCamera, int nDesiredTri, WORD* pTexIdxList)
#else
void	CLandscape::Create(LPDIRECT3DDEVICE8 pd3dDev, CBravoCamera* pCamera, int nDesiredTri, WORD* pTexIdxList)
#endif // DIRECT_VERSION_9_MJH
{
	CPatch	*pPatch = NULL;

	m_nNumPatches	= m_nMapSize/PATCH_SIZE;
	m_nDesiredTris	= nDesiredTri;
	///-- 1024 되면.. m_nNumPatches<-- 이 값은 16이 된다.

#ifndef APPLYWIDE_MAP
	
	m_Patches = SAFE_NEW_ARRAY( CPatch* , m_nNumPatches );	
	
	for ( int i = 0; i < m_nNumPatches; ++i )
	{
		m_Patches[i] = SAFE_NEW_ARRAY( CPatch , m_nNumPatches );
	}

	for ( int y = 0 ; y < m_nNumPatches; ++y )
	{
		for ( int x = 0; x < m_nNumPatches; ++x )
		{
			pPatch	= &m_Patches[y][x];
			pPatch->Create(pd3dDev, x*PATCH_SIZE, y*PATCH_SIZE, x*PATCH_SIZE, y*PATCH_SIZE, m_HeightMap, pCamera);
			pPatch->ComputeVariance();
			pPatch->RestoreDeviceObjects();

			pPatch->CreateTileMap( pTexIdxList );
			/// pPatch->RestoreDeviceObjectsTileMap();	/// Render에서 실시간으로 Restore 
		}
	}

#endif

	m_bAccessEnable	= true;
}

void	CLandscape::Reset( int eyeX, int eyeY )
{
	///-- LandScape 가 생성되었는지의 여부
	if ( !m_bAccessEnable )
		return;

	CPatch	*	pPatch = NULL;
	int			nNumPatches	= m_nNumPatches;

	SetNextTriNode(0);

#ifdef _DEBUG
	m_nNumTrisRendered = 0;
	m_nNumTileTrisRendered = 0;
	m_nNumTilePatches = 0;
	m_nRenderedPatches = 0;
#endif	

#ifdef APPLYWIDE_MAP
	
	///-- 여기 거리가 2000.0f일때는 괘안은데... 3000.0f일대는 스택오버플로어가 일어남;;;;
	m_xCurPatch = GetXPatchIndex( (float)eyeX );
	m_zCurPatch = GetZPatchIndex( (float)eyeY );
	m_xSrtPatch = max(0,m_xCurPatch-GetXPatchIndex(4500.0f));
	m_zSrtPatch = max(0,m_zCurPatch-GetZPatchIndex(4500.0f));
	m_xEndPatch = min(m_nNumPatches-1,m_xCurPatch+GetXPatchIndex(4500.0f));
	m_zEndPatch = min(m_nNumPatches-1,m_zCurPatch+GetZPatchIndex(4500.0f));

	//CPatchMap_i mit;
	set < int > PatchSet;

	//for( mit = m_PatchMap.begin(); mit != m_PatchMap.end(); ++mit ) 
	//{
	//	PatchSet.insert( (*mit).first );
	//}

	int iPatchID = 0, x = 0, y = 0;
	
	for( y = m_zSrtPatch ; y <= m_zEndPatch ; ++y )
	{
		for( x = m_xSrtPatch ; x <= m_xEndPatch ; ++x )
		{
			iPatchID = GetPatchID( x , y );

			if( PatchSet.find( iPatchID ) != PatchSet.end() )
			{
				PatchSet.erase( iPatchID );
			}
			else
			{
				///pPatch = SAFE_NEW( CPatch );
				pPatch = m_CPatchPool.NewMember();

				if( pPatch )
				{
					pPatch->Create( m_pd3dDevice, x*PATCH_SIZE, y*PATCH_SIZE, x*PATCH_SIZE, y*PATCH_SIZE, 
									m_HeightMap, m_pCamera );
					pPatch->ComputeVariance();
					pPatch->RestoreDeviceObjects();
					pPatch->CreateTileMap( m_pTileIndexList );
					m_PatchMap.insert( CPatchMap_vt( iPatchID, pPatch ) );
				}
			}
		}
	}

	for( set < int >::iterator sit = PatchSet.begin() ; sit != PatchSet.end() ; ++sit )
	{
		///SAFE_DELETE( ( *m_PatchMap.find(*sit) ).second );
		pPatch = ( *m_PatchMap.find(*sit) ).second;		
		pPatch->FinalCleanup();
		m_CPatchPool.DeleteMember( pPatch );
		///SAFE_DELETE( pPatch );	
		m_PatchMap.erase( *sit );
	}

	CPatch * pNeighbor = NULL;

	for( y = m_zSrtPatch ; y <= m_zEndPatch ; y++ )
	{
		for( x = m_xSrtPatch ; x <= m_xEndPatch ; x++ )
		{
			pPatch = GetPatch( x , y );
			
			if(!pPatch) continue;
		
			pPatch->Reset( eyeX, eyeY );
			pPatch->SetVisibility();
			
			if( pPatch->IsDirty() )
				pPatch->ComputeVariance();

			if( pPatch->IsVisibile() )
			{
				pNeighbor = GetPatch( x - 1 , y );
				pPatch->GetBaseLeft()->LeftNeighbor = (!pNeighbor)? NULL : pNeighbor->GetBaseRight();

				pNeighbor = GetPatch( x + 1 , y );
				pPatch->GetBaseRight()->LeftNeighbor = (!pNeighbor)? NULL : pNeighbor->GetBaseLeft();

				pNeighbor = GetPatch( x , y - 1 );
				pPatch->GetBaseLeft()->RightNeighbor = (!pNeighbor)? NULL : pNeighbor->GetBaseRight();

				pNeighbor = GetPatch( x , y + 1 );
				pPatch->GetBaseRight()->RightNeighbor = (!pNeighbor)? NULL : pNeighbor->GetBaseLeft();
			}		
		}	
	}
	
#else	
	
	for( int y = 0;  y < nNumPatches; ++y )
	{
		for( int x = 0; x < nNumPatches; ++x )
		{			
			
			pPatch	= &(m_Patches[y][x]);
			
			pPatch->Reset( eyeX, eyeY );
			pPatch->SetVisibility();
			
			if( pPatch->IsDirty() )
				pPatch->ComputeVariance();

			if( pPatch->IsVisibile() )
			{

#ifdef _DEBUG
				++m_nRenderedPatches;

				///-- 근거리 일때
				if( pPatch->GetNearDegree() == 0 )
				{
					/// 보이는 patch 중에 m_NearDegree == 0 이면 타일링 하는 영역 
					m_nNumTileTrisRendered += PATCH_SIZE * PATCH_SIZE * 2;	
					///-- 타일링 하는 패치의 갯수 상승
					++m_nNumTilePatches;
				}
#endif
				if ( x > 0 )
				{
					pPatch->GetBaseLeft()->LeftNeighbor = m_Patches[y][x-1].GetBaseRight();
				}
				else
				{
					pPatch->GetBaseLeft()->LeftNeighbor = NULL;		// Link to bordering Landscape here..
				}

				if ( x < (nNumPatches-1) )
				{
					pPatch->GetBaseRight()->LeftNeighbor = m_Patches[y][x+1].GetBaseLeft();
				}
				else
				{
					pPatch->GetBaseRight()->LeftNeighbor = NULL;		// Link to bordering Landscape here..
				}

				if ( y > 0 )
				{
					pPatch->GetBaseLeft()->RightNeighbor = m_Patches[y-1][x].GetBaseRight();
				}
				else
				{
					pPatch->GetBaseLeft()->RightNeighbor = NULL;		// Link to bordering Landscape here..
				}

				if ( y < (nNumPatches-1) )
				{
					pPatch->GetBaseRight()->RightNeighbor = m_Patches[y+1][x].GetBaseLeft();
				}
				else
				{
					pPatch->GetBaseRight()->RightNeighbor = NULL;	// Link to bordering Landscape here..
				}
			}	
		}
	}

#endif

	/// 근거리 타일링 영역 cell index min, max 구하기 
	m_EyePatchX = eyeX / VERTEX_INTERVAL / PATCH_SIZE;
	m_EyePatchY = eyeY / VERTEX_INTERVAL / PATCH_SIZE;

	///-- 눈의 위치가 구성패치 내에 들어와 있으면...
	if( m_EyePatchX >= 0 && m_EyePatchX < m_nNumPatches && m_EyePatchY >= 0 && m_EyePatchY < m_nNumPatches )
	{
		///-- 해당 눈이 위치한 패치를 포함.. 
		///-- 그 외 이웃 패치부분에 걸친 최소 최대 셀 Index min. max를 구한다.  

#ifdef APPLYWIDE_MAP
		pPatch = GetPatch( m_EyePatchX , m_EyePatchY );
#else		
		pPatch = &m_Patches[m_EyePatchY][m_EyePatchX];
#endif

		if( pPatch )
		{
			m_NearPatchWorldIdxMinX = pPatch->GetWorldX() - GetLODQuality() * PATCH_SIZE;
			m_NearPatchWorldIdxMinY = pPatch->GetWorldY() - GetLODQuality() * PATCH_SIZE;
			m_NearPatchWorldIdxMaxX = pPatch->GetWorldX() + ( GetLODQuality() + 1 ) * PATCH_SIZE;
			m_NearPatchWorldIdxMaxY = pPatch->GetWorldY() + ( GetLODQuality() + 1 ) * PATCH_SIZE;
			
			if( m_NearPatchWorldIdxMinX < 0 ) m_NearPatchWorldIdxMinX = 0;
			if( m_NearPatchWorldIdxMinY < 0 ) m_NearPatchWorldIdxMinY = 0;

#ifdef APPLYWIDE_MAP

			if( m_NearPatchWorldIdxMaxX > 511 ) m_NearPatchWorldIdxMaxX = 511;
			if( m_NearPatchWorldIdxMaxY > 511 ) m_NearPatchWorldIdxMaxY = 511;

#else

			if( m_NearPatchWorldIdxMaxX > 255 ) m_NearPatchWorldIdxMaxX = 255;
			if( m_NearPatchWorldIdxMaxY > 255 ) m_NearPatchWorldIdxMaxY = 255;

#endif
		
		}

	}
}

void	CLandscape::Tessellate()
{
	if ( !m_bAccessEnable )
		return;
	int	nNumPatches	= m_nNumPatches;
	
	g_Count = 0;

#ifdef APPLYWIDE_MAP	
	
	for( CPatchMap_i it = m_PatchMap.begin() ; it != m_PatchMap.end() ; ++it )
	{
		if( (*it).second->IsVisibile() )
		{
			(*it).second->Tessellate();
		}
	}

#else	
	
	for ( int y = 0; y < nNumPatches; ++y )
	{
		for ( int x = 0; x < nNumPatches; ++x )
		{			
			if ( m_Patches[y][x].IsVisibile() )
			{
				m_Patches[y][x].Tessellate();
			}
		}
	}

#endif

}

void	CLandscape::Render()
{
	if ( !m_bAccessEnable )
		return;

	D3DXMATRIX matPos;
	D3DXMatrixIdentity(&matPos);
	g_RenderManager.SetTransform(D3DTS_WORLD, &matPos);

	g_RenderManager.SetTexture( 0, g_TileTexture.m_dpBaseTextures );

	/// 타일링 : 0번->통맵, 1번->라이트맵 
	g_RenderManager.SetTexture( 1, g_TileTexture.m_dpBaseTextureLight );

	/// 텍스처 UV 하드웨어 가속 
	D3DXMATRIX matView, matViewInv;
	matView = m_pCamera->GetView();
	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	D3DXMatrixMultiply(&m_MatTex0, &matViewInv, &m_MatScaleTexture0);
	D3DXMatrixMultiply(&m_MatTex1, &matViewInv, &m_MatScaleTexture0);	/// !!! 

	g_RenderManager.SetTextureCoordIndex( 0 , D3DTSS_TCI_CAMERASPACEPOSITION );	
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT2 );

	g_RenderManager.SetTransform(D3DTS_TEXTURE0, &m_MatTex0); 

	g_RenderManager.SetTextureCoordIndex( 1 , D3DTSS_TCI_CAMERASPACEPOSITION );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_COUNT2 );

	g_RenderManager.SetTransform(D3DTS_TEXTURE1, &m_MatTex1); 

#ifdef APPLYWIDE_MAP
	
	for( CPatchMap_i it = m_PatchMap.begin() ; it != m_PatchMap.end() ; ++it )
	{
		if( (*it).second->IsVisibile() )
		{
			(*it).second->Render();
		}
	}
	
#else	
	
	for ( int y = 0;  y < m_nNumPatches; ++y )
	{
		for ( int x = 0; x < m_nNumPatches; ++x )
		{
			if ( m_Patches[y][x].IsVisibile() )
			{
				m_Patches[y][x].Render();
			}
		}
	}

#endif
	
	///-- 우리가 요구된 삼각형갯수에서 벗어났는지 체크하자 
	///-- 더 적절한 값으로 프레임 편차를 조절한다.
	if ( GetNextTriNode() != m_nDesiredTris )
		m_fFrameVariance += ((float)GetNextTriNode() - (float)m_nDesiredTris) / (float)m_nDesiredTris;

	if ( m_fFrameVariance < 0 ) m_fFrameVariance = 0;
	if ( m_fFrameVariance > 300 ) m_fFrameVariance = 300;
}

void	CLandscape::Update()
{
	if ( !m_bAccessEnable )
		return;

#ifdef APPLYWIDE_MAP
	
	for( CPatchMap_i it = m_PatchMap.begin() ; it != m_PatchMap.end() ; ++it )
	{
		if( (*it).second->IsVisibile() )
		{
			(*it).second->Update();
		}
	}

#else	
	
	for ( int y = 0; y < m_nNumPatches; ++y )
	{
		for ( int x = 0; x < m_nNumPatches; ++x )
		{
			if ( m_Patches[y][x].IsVisibile() )
			{
				m_Patches[y][x].Update();
			}
		}
	}

#endif
}

///--------------------------------------------------------------
///	tile
///--------------------------------------------------------------

void	CLandscape::RenderTileMap()
{
	D3DXMATRIX matPos;
	D3DXMatrixIdentity(&matPos);
	g_RenderManager.SetTransform(D3DTS_WORLD, &matPos);

	/// 타일방식 : 0번 텍스쳐는 타일링에서 설정 
	g_RenderManager.SetTextureCoordIndex( 0 , 0 );
	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );

	/// 1번 텍스쳐는 라이트맵 
	g_RenderManager.SetTexture( 1, g_TileTexture.m_dpBaseTextureLight );

	/// 라이트맵 텍스처 UV 하드웨어 가속 
	D3DXMATRIX matView, matViewInv;
	matView = m_pCamera->GetView();
	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	D3DXMatrixMultiply(&m_MatTex1, &matViewInv, &m_MatScaleTexture0);	/// !!! 

	g_RenderManager.SetTextureCoordIndex( 1 , D3DTSS_TCI_CAMERASPACEPOSITION );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_COUNT2 );

	g_RenderManager.SetTransform( D3DTS_TEXTURE1, &m_MatTex1 ); 


#ifdef _DEBUG
	m_nNumTileRenderCall = 0;
#endif

	DWORD tick = timeGetTime();
	
	ProfileBegin( "RenderTileMap()" );

	int num = 0;

#ifdef APPLYWIDE_MAP
	
	for( CPatchMap_i it = m_PatchMap.begin() ; it != m_PatchMap.end() ; ++it )
	{
		if( (*it).second->IsVisibile() )
		{
			(*it).second->RenderTileMap();
			num++;
		}
		else
		{
			(*it).second->CacheTimeout( tick );
		}
	}
	
#else
	
	for ( int y = 0; y < m_nNumPatches; ++y )
	{
		for ( int x = 0; x < m_nNumPatches; ++x )
		{

			if ( m_Patches[y][x].IsVisibile() )
			{
				m_Patches[y][x].RenderTileMap();
				num++;
			}
			else
			{
				/// view frustum 안에 없는 녀석들은 vertex buffer cache free 
				m_Patches[y][x].CacheTimeout( tick );
			}
		}
	}

#endif
	
	ProfileEnd( "RenderTileMap()" );

	g_RenderManager.SetTextureCoordIndex( 1 , 1 );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );
	
	
	D3DXMatrixIdentity( &m_MatTex1 );
	g_RenderManager.SetTransform( D3DTS_TEXTURE1 , &m_MatTex1);
}

CPatch* CLandscape::GetPatch(int xInx,int zInx)		
{
	CPatchMap_i it = m_PatchMap.find(GetPatchID(xInx,zInx));
	if(it != m_PatchMap.end()) return (*it).second;

	return NULL;
}

int CLandscape::GetPatchID(int xInx,int zInx)
{
	return xInx*10000+zInx;
}

int CLandscape::GetXPatchIndex(float fPos)
{
	return max(0 , min( m_nNumPatches - 1 , (int)(fPos/( PATCH_SIZE*VERTEX_INTERVAL ) )));
}

int CLandscape::GetZPatchIndex(float fPos)
{
	return max(0 , min( m_nNumPatches - 1 , (int)(fPos/( PATCH_SIZE*VERTEX_INTERVAL ) )));
}
