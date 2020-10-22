#include "RockPCH.h"

#include	"..\\quadlist.h"
#include	<BASE\\d3dapp.h>
#include	"Field.h"
#include	"..\\Camera.h"

#include "..\\..\\CRenderManager.h"

bool		CField::LoadAttr(attr* attr)
{
	D3DXMatrixIdentity(&m_matWorldAttr);

	DWORD i, y, x;
	/* CREATION ATTRIBUTE MAP */

	m_pAttrDisp	= SAFE_NEW_ARRAY( AttrDisp* , m_cSize*2 );	
	for ( i = 0; i < m_cSize*2; i ++ )
		m_pAttrDisp[i]	= SAFE_NEW_ARRAY( AttrDisp , m_cSize*2 );
	
	/* FILL ATTRIBUTE */
	for ( y = 0; y < m_cSize*2; y ++ )
	{
		for ( x = 0; x < m_cSize*2; x ++ )
		{
			m_pAttrMap[y][x].byAttr	= (BYTE)attr[(y*(m_cSize*2))+x].wAttr;
//			m_pAttrMap[y][x].dwType	= (WORD)attr[(y*(m_cSize*2))+x].wType;
			m_pAttrMap[y][x].nHeight	= (short)attr[(y*(m_cSize*2))+x].fHeight;
		}
	}

	for ( y = 0; y < m_cSize*2; y ++ )
	{
		for ( x = 0; x < m_cSize*2; x ++ )
		{
			m_pAttrDisp[y][x].bIsVisible	= false;
			m_pAttrDisp[y][x].dwTexIdx		= -1;

			m_pAttrDisp[y][x].Pos[0].x	= x * (TILE_SIZE/2);
			m_pAttrDisp[y][x].Pos[0].y	= 0.0f;
			m_pAttrDisp[y][x].Pos[0].z	= y * (TILE_SIZE/2);
//			m_pAttrDisp[y][x].Pos[0].tu	= ;
//			m_pAttrDisp[y][x].Pos[0].tv	= ;

			m_pAttrDisp[y][x].Pos[1].x	= x * (TILE_SIZE/2);
			m_pAttrDisp[y][x].Pos[1].y	= 0.0f;
			m_pAttrDisp[y][x].Pos[1].z	= (y+1) * (TILE_SIZE/2);
//			m_pAttrDisp[y][x].Pos[1].tu	= ;
//			m_pAttrDisp[y][x].Pos[1].tv	= ;

			m_pAttrDisp[y][x].Pos[2].x	= (x+1) * (TILE_SIZE/2);
			m_pAttrDisp[y][x].Pos[2].y	= 0.0f;
			m_pAttrDisp[y][x].Pos[2].z	= (y+1) * (TILE_SIZE/2);
//			m_pAttrDisp[y][x].Pos[2].tu	= ;
//			m_pAttrDisp[y][x].Pos[2].tv	= ;

			m_pAttrDisp[y][x].Pos[3].x	= (x+1) * (TILE_SIZE/2);
			m_pAttrDisp[y][x].Pos[3].y	= 0.0f;
			m_pAttrDisp[y][x].Pos[3].z	= y * (TILE_SIZE/2);
//			m_pAttrDisp[y][x].Pos[3].tu	= ;
//			m_pAttrDisp[y][x].Pos[3].tv	= ;

			SetupAttrUV(x, y, m_pAttrMap[y][x].byAttr);
		}
	}
	const int size	= 1600*6;

	if ( FAILED ( g_RenderManager.CreateIndexBuffer( size * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIBufferAttr ) ) )
	{
///		MessageBox(NULL, "Error - Create(IB ATTR)", NULL, MB_OK);
		return false;
	}
	if ( FAILED ( g_RenderManager.CreateVertexBuffer( size * sizeof(TERVTX), D3DUSAGE_WRITEONLY, D3DFVF_TERVTX, D3DPOOL_MANAGED, &m_pVBufferAttr ) ) )
	{
///		MessageBox(NULL, "Error - Create(VB ATTR)", NULL, MB_OK);
		return false;
	}

	return true;
}

bool		CField::UpdateAttr()
{
	if ( m_pAttrDisp == NULL )
		return false;

	DWORD	y, x;
	DWORD	sx2, sy2, ex2, ey2;

	sx2	= sx*2;
	sy2	= sy*2;
	ex2	= ex*2;
	ey2	= ey*2;

	if ( sx2 < 0 || sx2 >= m_cSize*2 )		sx2 = 0;
	if ( sy2 < 0 || sy2 >= m_cSize*2 )		sy2 = 0;
	if ( ex2 > m_cSize*2 )					ex2 = m_cSize*2;
	if ( ey2 > m_cSize*2 )					ey2 = m_cSize*2;

	dwAttrVisibleCount	= 0;
	/* VISIBLE CHECK */
	for ( y = sy2; y < ey2; y ++ )
	{
		for ( x = sx2; x < ex2; x ++ )
		{
			if ( true == (m_pAttrDisp[y][x].bIsVisible	= g_Camera.CubeInFrustum(m_pAttrDisp[y][x].Pos[0].x, m_pAttrDisp[y][x].Pos[0].y, m_pAttrDisp[y][x].Pos[0].z, TILE_SIZE/4)) )
				dwAttrVisibleCount	++;
		}
	}
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIBufferAttr->Lock(0, 0, (void**)&m_pIdxListAttr, 0 ) ) )
#else
	if ( FAILED ( m_pIBufferAttr->Lock(0, 0, (BYTE**)&m_pIdxListAttr, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock", "IB ATTR", MB_OK);
		return false;
	}
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVBufferAttr->Lock(0, 0, (void**)&m_pVtxListAttr, 0 ) ) )
#else
	if ( FAILED ( m_pVBufferAttr->Lock(0, 0, (BYTE**)&m_pVtxListAttr, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock", "VB ATTR", MB_OK);
		return false;
	}

	DWORD	dwIdxCount	= 0;
	DWORD	dwVtxCount	= 0;
	float	fCenter	= 0.0f;

	m_dwVtxCountAttr	= 0;
	m_dwIdxCountAttr	= 0;

	/* CALCULATE CENTER INTERPOLATION VALUE OF TILE */
	for ( y = sy2; y < ey2; y ++ )
	{
		for ( x = sx2; x < ex2; x ++ )
		{
			fCenter	= m_pDisplayMap[y/2][x/2].Pos[1].y > m_pDisplayMap[y/2][x/2].Pos[3].y ?
				m_pDisplayMap[y/2][x/2].Pos[3].y + (m_pDisplayMap[y/2][x/2].Pos[1].y-m_pDisplayMap[y/2][x/2].Pos[3].y)/2 :
				m_pDisplayMap[y/2][x/2].Pos[1].y + (m_pDisplayMap[y/2][x/2].Pos[3].y-m_pDisplayMap[y/2][x/2].Pos[1].y)/2;
			
			/* TOP RIGHT */
			if ( y%2 && x%2 )
			{
				m_pAttrDisp[y][x].Pos[0].y	= fCenter;
				m_pAttrDisp[y][x].Pos[1].y	= m_pDisplayMap[y/2][x/2].Pos[1].y > m_pDisplayMap[y/2][x/2].Pos[2].y ?
					m_pDisplayMap[y/2][x/2].Pos[2].y + (m_pDisplayMap[y/2][x/2].Pos[1].y-m_pDisplayMap[y/2][x/2].Pos[2].y)/2 :
					m_pDisplayMap[y/2][x/2].Pos[1].y + (m_pDisplayMap[y/2][x/2].Pos[2].y-m_pDisplayMap[y/2][x/2].Pos[1].y)/2;
				m_pAttrDisp[y][x].Pos[2].y	= m_pDisplayMap[y/2][x/2].Pos[2].y;
				m_pAttrDisp[y][x].Pos[3].y	= m_pDisplayMap[y/2][x/2].Pos[2].y > m_pDisplayMap[y/2][x/2].Pos[3].y ?
					m_pDisplayMap[y/2][x/2].Pos[3].y + (m_pDisplayMap[y/2][x/2].Pos[2].y-m_pDisplayMap[y/2][x/2].Pos[3].y)/2 :
					m_pDisplayMap[y/2][x/2].Pos[2].y + (m_pDisplayMap[y/2][x/2].Pos[3].y-m_pDisplayMap[y/2][x/2].Pos[2].y)/2;
			}
			/* TOP LEFT */
			else if ( y%2 && !(x%2) )
			{
				m_pAttrDisp[y][x].Pos[0].y	= m_pDisplayMap[y/2][x/2].Pos[0].y > m_pDisplayMap[y/2][x/2].Pos[1].y ?
					m_pDisplayMap[y/2][x/2].Pos[1].y + (m_pDisplayMap[y/2][x/2].Pos[0].y-m_pDisplayMap[y/2][x/2].Pos[1].y)/2 :
					m_pDisplayMap[y/2][x/2].Pos[0].y + (m_pDisplayMap[y/2][x/2].Pos[1].y-m_pDisplayMap[y/2][x/2].Pos[0].y)/2;
				m_pAttrDisp[y][x].Pos[1].y	= m_pDisplayMap[y/2][x/2].Pos[1].y;
				m_pAttrDisp[y][x].Pos[2].y	= m_pDisplayMap[y/2][x/2].Pos[1].y > m_pDisplayMap[y/2][x/2].Pos[2].y ?
					m_pDisplayMap[y/2][x/2].Pos[2].y + (m_pDisplayMap[y/2][x/2].Pos[1].y-m_pDisplayMap[y/2][x/2].Pos[2].y)/2 :
					m_pDisplayMap[y/2][x/2].Pos[1].y + (m_pDisplayMap[y/2][x/2].Pos[2].y-m_pDisplayMap[y/2][x/2].Pos[1].y)/2;
				m_pAttrDisp[y][x].Pos[3].y	= fCenter;
			}
			/* BOTTOM RIGHT */
			else if ( !(y%2) && x%2 )
			{
				m_pAttrDisp[y][x].Pos[0].y	= m_pDisplayMap[y/2][x/2].Pos[0].y > m_pDisplayMap[y/2][x/2].Pos[3].y ?
					m_pDisplayMap[y/2][x/2].Pos[3].y + (m_pDisplayMap[y/2][x/2].Pos[0].y-m_pDisplayMap[y/2][x/2].Pos[3].y)/2 :
					m_pDisplayMap[y/2][x/2].Pos[0].y + (m_pDisplayMap[y/2][x/2].Pos[3].y-m_pDisplayMap[y/2][x/2].Pos[0].y)/2;
				m_pAttrDisp[y][x].Pos[1].y	= fCenter;
				m_pAttrDisp[y][x].Pos[2].y	= m_pDisplayMap[y/2][x/2].Pos[2].y > m_pDisplayMap[y/2][x/2].Pos[3].y ?
					m_pDisplayMap[y/2][x/2].Pos[3].y + (m_pDisplayMap[y/2][x/2].Pos[2].y-m_pDisplayMap[y/2][x/2].Pos[3].y)/2 :
					m_pDisplayMap[y/2][x/2].Pos[2].y + (m_pDisplayMap[y/2][x/2].Pos[3].y-m_pDisplayMap[y/2][x/2].Pos[2].y)/2;
				m_pAttrDisp[y][x].Pos[3].y	= m_pDisplayMap[y/2][x/2].Pos[3].y;
			}
			/* BOTTOM LEFT */
			else if ( !(y%2) && !(x%2) )
			{
				m_pAttrDisp[y][x].Pos[0].y	= m_pDisplayMap[y/2][x/2].Pos[0].y;
				m_pAttrDisp[y][x].Pos[1].y	= m_pDisplayMap[y/2][x/2].Pos[0].y > m_pDisplayMap[y/2][x/2].Pos[1].y ?
					m_pDisplayMap[y/2][x/2].Pos[1].y + (m_pDisplayMap[y/2][x/2].Pos[0].y-m_pDisplayMap[y/2][x/2].Pos[1].y)/2 :
					m_pDisplayMap[y/2][x/2].Pos[0].y + (m_pDisplayMap[y/2][x/2].Pos[1].y-m_pDisplayMap[y/2][x/2].Pos[0].y)/2;
				m_pAttrDisp[y][x].Pos[2].y	= fCenter;
				m_pAttrDisp[y][x].Pos[3].y	= m_pDisplayMap[y/2][x/2].Pos[0].y > m_pDisplayMap[y/2][x/2].Pos[3].y ?
					m_pDisplayMap[y/2][x/2].Pos[3].y + (m_pDisplayMap[y/2][x/2].Pos[0].y-m_pDisplayMap[y/2][x/2].Pos[3].y)/2 :
					m_pDisplayMap[y/2][x/2].Pos[0].y + (m_pDisplayMap[y/2][x/2].Pos[3].y-m_pDisplayMap[y/2][x/2].Pos[0].y)/2;
			}
		}
	}

	for ( y = sy2; y < ey2; y ++ )
	{
		for ( x = sx2; x < ex2; x ++ )
		{
			if ( m_pAttrDisp[y][x].bIsVisible )
			{
				m_pVtxListAttr[dwVtxCount].x	= m_pAttrDisp[y][x].Pos[0].x;
				m_pVtxListAttr[dwVtxCount].y	= m_pAttrDisp[y][x].Pos[0].y;
				m_pVtxListAttr[dwVtxCount].z	= m_pAttrDisp[y][x].Pos[0].z;
				m_pVtxListAttr[dwVtxCount].tu	= m_pAttrDisp[y][x].Pos[0].tu;
				m_pVtxListAttr[dwVtxCount].tv	= m_pAttrDisp[y][x].Pos[0].tv;
				dwVtxCount	++;

				m_pVtxListAttr[dwVtxCount].x	= m_pAttrDisp[y][x].Pos[1].x;
				m_pVtxListAttr[dwVtxCount].y	= m_pAttrDisp[y][x].Pos[1].y;
				m_pVtxListAttr[dwVtxCount].z	= m_pAttrDisp[y][x].Pos[1].z;
				m_pVtxListAttr[dwVtxCount].tu	= m_pAttrDisp[y][x].Pos[1].tu;
				m_pVtxListAttr[dwVtxCount].tv	= m_pAttrDisp[y][x].Pos[1].tv;
				dwVtxCount	++;

				m_pVtxListAttr[dwVtxCount].x	= m_pAttrDisp[y][x].Pos[2].x;
				m_pVtxListAttr[dwVtxCount].y	= m_pAttrDisp[y][x].Pos[2].y;
				m_pVtxListAttr[dwVtxCount].z	= m_pAttrDisp[y][x].Pos[2].z;
				m_pVtxListAttr[dwVtxCount].tu	= m_pAttrDisp[y][x].Pos[2].tu;
				m_pVtxListAttr[dwVtxCount].tv	= m_pAttrDisp[y][x].Pos[2].tv;
				dwVtxCount	++;

				m_pVtxListAttr[dwVtxCount].x	= m_pAttrDisp[y][x].Pos[3].x;
				m_pVtxListAttr[dwVtxCount].y	= m_pAttrDisp[y][x].Pos[3].y;
				m_pVtxListAttr[dwVtxCount].z	= m_pAttrDisp[y][x].Pos[3].z;
				m_pVtxListAttr[dwVtxCount].tu	= m_pAttrDisp[y][x].Pos[3].tu;
				m_pVtxListAttr[dwVtxCount].tv	= m_pAttrDisp[y][x].Pos[3].tv;
				dwVtxCount	++;

				m_pIdxListAttr[dwIdxCount++]	= (WORD)dwVtxCount-4;
				m_pIdxListAttr[dwIdxCount++]	= (WORD)dwVtxCount-3;
				m_pIdxListAttr[dwIdxCount++]	= (WORD)dwVtxCount-1;
				m_pIdxListAttr[dwIdxCount++]	= (WORD)dwVtxCount-2;
				m_pIdxListAttr[dwIdxCount++]	= (WORD)dwVtxCount-1;
				m_pIdxListAttr[dwIdxCount++]	= (WORD)dwVtxCount-3;
			}
		}
	}

	m_dwVtxCountAttr	= dwVtxCount;
	m_dwIdxCountAttr	= dwIdxCount;

	m_pIBufferAttr->Unlock();
	m_pVBufferAttr->Unlock();

	return true;
}

HRESULT		CField::DisplayAttr()
{
	if ( m_pAttrDisp == NULL )
		return E_FAIL;

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorldAttr);
	g_RenderManager.SetStreamSource(0, m_pVBufferAttr, sizeof(ATTRVTX));
	g_RenderManager.SetFVF(D3DFVF_PT);
	g_RenderManager.SetIndices(m_pIBufferAttr, 0);

	g_RenderManager.SetTexture(0, m_pAttrTex);

	DWORD	curidx	= 0;
	DWORD	curvtx	= 0;

	DWORD	sx2	= sx*2;
	DWORD	sy2	= sy*2;
	DWORD	ex2	= ex*2;
	DWORD	ey2	= ey*2;

	if ( sx2 < 0 || sx2 >= m_cSize*2 )		sx2 = 0;
	if ( sy2 < 0 || sy2 >= m_cSize*2 )		sy2 = 0;
	if ( ex2 > m_cSize*2 )					ex2 = m_cSize*2;
	if ( ey2 > m_cSize*2 )					ey2 = m_cSize*2;

	dwAttrVisibleCount	= 0;

	/* VISIBLE CHECK */
	for ( DWORD	y = sy2; y < ey2; y ++ )
	{
		for ( DWORD x = sx2; x < ex2; x ++ )
		{
			if ( m_pAttrDisp[y][x].bIsVisible )
			{
				if ( m_pAttrMap[y][x].byAttr == MATTR_NONE )
					g_RenderManager.SetFillMode( D3DFILL_WIREFRAME );
				else
					g_RenderManager.SetFillMode( D3DFILL_SOLID );

				g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, curvtx, 4, curidx, 2);
				curidx	+= 6;
				curvtx	+= 4;
			}
		}
	}

	return	S_OK;
}

HRESULT		CField::PickAttr(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight)
{
	if ( m_pAttrDisp == NULL )
		return E_FAIL;

	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(hWnd, &ptCursor);
	ptCursor.x	+= 2;
	ptCursor.y	+= 10;

	D3DXVECTOR3	vPickRayDir;
	D3DXVECTOR3	vPickRayOrig;

	D3DXMATRIX		matProj;
	matProj	= g_Camera.m_matProj;
	
	GetCursorPos(&ptCursor);
	ptCursor.x	+= 2;
	ptCursor.y	+= 10;
	::ScreenToClient(hWnd, &ptCursor);

	D3DXVECTOR3	v;
	v.x	= (((2.0f*ptCursor.x)/dwScreenWidth)-1) / matProj._11;
	v.y	= -(((2.0f*ptCursor.y)/dwScreenHeight)-1) / matProj._22;
	v.z	= 1.0f;
	
	D3DXMATRIX		matView, m;
	matView	= g_Camera.m_matView;
	D3DXMatrixInverse(&m, NULL, &matView);

	vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;

	FLOAT	fBary1, fBary2;
	FLOAT	fDist;

	// check range == tile size ( for easy picking )
	D3DXVECTOR3		v1[3];	// 1st face
	D3DXVECTOR3		v2[3];	// 2nd face

	DWORD			i = 0;

	DWORD	sx2, sy2, ex2, ey2;

	sx2	= sx*2;
	sy2	= sy*2;
	ex2	= ex*2;
	ey2	= ey*2;

	if ( sx2 < 0 || sx2 >= m_cSize*2 )		sx2 = 0;
	if ( sy2 < 0 || sy2 >= m_cSize*2 )		sy2 = 0;
	if ( ex2 > m_cSize*2 )					ex2 = m_cSize*2;
	if ( ey2 > m_cSize*2 )					ey2 = m_cSize*2;

	for ( DWORD y = sy2; y < ey2; y ++ )
	{
		for ( DWORD x = sx2; x < ex2; x ++ )
		{
			if ( m_pAttrDisp[y][x].bIsVisible )
			{
				v1[0].x	= m_pAttrDisp[y][x].Pos[0].x;
				v1[0].y	= m_pAttrDisp[y][x].Pos[0].y;
				v1[0].z	= m_pAttrDisp[y][x].Pos[0].z;
				v1[1].x	= m_pAttrDisp[y][x].Pos[1].x;
				v1[1].y	= m_pAttrDisp[y][x].Pos[1].y;
				v1[1].z	= m_pAttrDisp[y][x].Pos[1].z;
				v1[2].x	= m_pAttrDisp[y][x].Pos[3].x;
				v1[2].y	= m_pAttrDisp[y][x].Pos[3].y;
				v1[2].z	= m_pAttrDisp[y][x].Pos[3].z;

				v2[0].x	= m_pAttrDisp[y][x].Pos[2].x;
				v2[0].y	= m_pAttrDisp[y][x].Pos[2].y;
				v2[0].z	= m_pAttrDisp[y][x].Pos[2].z;
				v2[1].x	= m_pAttrDisp[y][x].Pos[3].x;
				v2[1].y	= m_pAttrDisp[y][x].Pos[3].y;
				v2[1].z	= m_pAttrDisp[y][x].Pos[3].z;
				v2[2].x	= m_pAttrDisp[y][x].Pos[1].x;
				v2[2].y	= m_pAttrDisp[y][x].Pos[1].y;
				v2[2].z	= m_pAttrDisp[y][x].Pos[1].z;

				if (IntersectTriangle(vPickRayOrig, vPickRayDir, v1[0], v1[1], v1[2], &fDist, &fBary1, &fBary2) 
				||	IntersectTriangle(vPickRayOrig, vPickRayDir, v2[0], v2[1], v2[2], &fDist, &fBary1, &fBary2) )
				{
					m_dwPickedX	= x;
					m_dwPickedY	= y;

					return S_OK;
				}
			}
		}
	}
	return S_OK;
}

void	CField::SetupAttrUV()
{
	const float	xoffset	= 0.25f;
	const float yoffset	= 0.1f;
	DWORD	u, v;

	u	= m_dwCurAttr%4;
	v	= m_dwCurAttr/4;

	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[0].tu	= (float)u*xoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[0].tv	= (float)(v+1)*yoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[1].tu	= (float)u*xoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[1].tv	= (float)v*yoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[2].tu	= (float)(u+1)*xoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[2].tv	= (float)v*yoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[3].tu	= (float)(u+1)*xoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[3].tv	= (float)(v+1)*yoffset;
}

void	CField::SetupAttrUV(DWORD x, DWORD y, DWORD idx)
{
	const float	xoffset	= 0.25f;
	const float yoffset	= 0.1f;
	DWORD	u, v;

	u	= idx%4;
	v	= idx/4;

	m_pAttrDisp[y][x].Pos[0].tu	= (float)u*xoffset;
	m_pAttrDisp[y][x].Pos[0].tv	= (float)(v+1)*yoffset;
	m_pAttrDisp[y][x].Pos[1].tu	= (float)u*xoffset;
	m_pAttrDisp[y][x].Pos[1].tv	= (float)v*yoffset;
	m_pAttrDisp[y][x].Pos[2].tu	= (float)(u+1)*xoffset;
	m_pAttrDisp[y][x].Pos[2].tv	= (float)v*yoffset;
	m_pAttrDisp[y][x].Pos[3].tu	= (float)(u+1)*xoffset;
	m_pAttrDisp[y][x].Pos[3].tv	= (float)(v+1)*yoffset;
}

void	CField::SetupAttr()
{
	m_pAttrMap[m_dwPickedY][m_dwPickedX].byAttr	= (BYTE)m_dwCurAttr;
}

void	CField::SetupAttrNone()
{
	m_pAttrMap[m_dwPickedY][m_dwPickedX].byAttr	= MATTR_NONE;
	const float	xoffset	= 0.25f;
	const float yoffset	= 0.1f;
	DWORD	u = 0, v = 0;

	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[0].tu	= (float)u*xoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[0].tv	= (float)(v+1)*yoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[1].tu	= (float)u*xoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[1].tv	= (float)v*yoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[2].tu	= (float)(u+1)*xoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[2].tv	= (float)v*yoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[3].tu	= (float)(u+1)*xoffset;
	m_pAttrDisp[m_dwPickedY][m_dwPickedX].Pos[3].tv	= (float)(v+1)*yoffset;
}

float	CField::GetAttrHeight(DWORD x, DWORD y)
{
	if ( m_pDisplayMap == NULL )
		return	0.0f;

	if( x > m_cSize*2 || x < 0 || y > m_cSize*2 || y < 0 )
		return 0.0f;

	DWORD	dwDispX	= x/2;
	DWORD	dwDispY	= y/2;
	float	fFinal	= 0.0f;
	float	fVerticalCenter		= 0.0f;
	float	fHorizontalCenter	= 0.0f;
	float	fCenter				= 0.0f;
	float	fHeight[4]			= { m_pDisplayMap[dwDispY][dwDispX].Pos[0].y,
									m_pDisplayMap[dwDispY][dwDispX].Pos[1].y,
									m_pDisplayMap[dwDispY][dwDispX].Pos[2].y,
									m_pDisplayMap[dwDispY][dwDispX].Pos[3].y
	};

	/* CASE : 0 */// BOTTOM LEFT
	if ( (x%2)==0 && (y%2)==0 )
	{
		fVerticalCenter		= (fHeight[0] + fHeight[3])/2;
		fHorizontalCenter	= (fHeight[0] + fHeight[1])/2;
		fFinal				= (fVerticalCenter +fHorizontalCenter)/2;
		return	fFinal;
	}
	/* CASE : 1 */// TOP LEFT
	else if ( !(x%2) && (y%2) )
	{
/*		fVerticalCenter		= (fHeight[0] + fHeight[2])/2;
		fHorizontalCenter	= (fHeight[1] + fHeight[3])/2;
		fFinal				= (fVerticalCenter + fHorizontalCenter)/2;
		fFinal				= (fFinal + fHeight[2])/2;
		*/
		fVerticalCenter		= (fHeight[1] + fHeight[3])/2;
		fFinal				= (fVerticalCenter+fHeight[1])/2;
		return	fFinal;
	}
	/* CASE : 2 */// TOP RIGHT
	else if ( (x%2) && (y%2) )
	{
		fVerticalCenter		= (fHeight[1] + fHeight[2])/2;
		fHorizontalCenter	= (fHeight[2] + fHeight[3])/2;
		fFinal				= (fVerticalCenter + fHorizontalCenter)/2;
		return	fFinal;
	}
	/* CASE : 3 */// BOTTOM RIGHT
	else if ( (x%2) && !(y%2) )
	{
/*		fVerticalCenter		= (fHeight[0] + fHeight[2])/2;
		fHorizontalCenter	= (fHeight[1] + fHeight[3])/2;
		fFinal				= (fVerticalCenter + fHorizontalCenter)/2;
		fFinal				= (fFinal + fHeight[1])/2;
		*/
		fVerticalCenter		= (fHeight[1] + fHeight[3])/2;
		fFinal				= (fVerticalCenter + fHeight[3])/2;
		return	fFinal;
	}
	return	0.0f;
}


void		CField::CalculateAttrHeight()
{
	float	fHeight[2];

	for ( DWORD y = 0; y < m_cSize*2; y ++ )
	{
		for ( DWORD x = 0; x < m_cSize*2; x ++ )
		{
			fHeight[0]	= m_pAttrDisp[y][x].Pos[1].y;
			fHeight[1]	= m_pAttrDisp[y][x].Pos[3].y;

			m_pAttrMap[y][x].nHeight	= (fHeight[0] > fHeight[1]) ?
				fHeight[1] + ((fHeight[0]-fHeight[1])/2) : fHeight[0] + ((fHeight[1]-fHeight[0])/2);
		}
	}
}

/// sooree height ---------------------------------------------->
/// 논리적 cell 좌표로 높이 구하기(속성 높이 포함) 
float CField::Get__MapHeight( int cell_x, int cell_y )	
{
	float ht = Get__AttrHeight( cell_x, cell_y );

	if( ht > 0 )
	{
		return ht;
	}

	return( Get__MapHeightF( ( cell_x * 8.0f + 4.0f ), ( cell_y * 8.0f + 4.0f ) ) );
}

/// real-float 좌표로 지형 높이 구하기(속성 높이 포함) 
float	CField::Get__MapHeightF( float aX, float aZ )	
{
	float dx = aX / 16.0f;
	float dz = aZ / 16.0f; 
	int ix = (int)( dx );
	int iz = (int)( dz );
	if( ix >= 0 && ix < m_cSize - 1 && iz >= 0 && iz < m_cSize - 1 )
	{
		dx -= ix;
		dz -= iz;
 
		/// 지형 타일 버텍스 높이 : 속성 높이가 있다면 그것을 읽어온다. 
		float x_z, x1_z, x_z1, x1_z1;
		x_z = m_pAttrMap[iz << 1][ix << 1].nHeight; // Get__AttrHeight( ix << 1, iz << 1 );
		if( x_z == 0 ) 
		{
			x_z = m_pDisplayMap[iz][ix].Pos[0].y;
		}
		x1_z = m_pAttrMap[iz << 1][(ix + 1) << 1].nHeight; // Get__AttrHeight( (ix + 1) << 1, iz << 1 );
		if( x1_z == 0 ) 
		{
			x1_z = m_pDisplayMap[iz][ix + 1].Pos[0].y;
		}
		x_z1 = m_pAttrMap[(iz + 1) << 1][ix << 1].nHeight; // Get__AttrHeight( ix << 1, (iz + 1) << 1 );
		if( x_z1 == 0 ) 
		{
			x_z1 = m_pDisplayMap[iz + 1][ix].Pos[0].y;
		}
		x1_z1 = m_pAttrMap[(iz + 1) << 1][(ix + 1) << 1].nHeight; // Get__AttrHeight( (ix + 1) << 1, (iz + 1) << 1 );
		if( x1_z1 == 0 ) 
		{
			x1_z1 = m_pDisplayMap[iz + 1][ix + 1].Pos[0].y;
		}

		float height = dx + dz - 1.0f;
		if( height <= 0.0f )
		{
			height =	x1_z * dx + 
						x_z1 * dz - 
						x_z * height;
		}
		else
		{
			height =	x_z1 * ( 1.0f - dx ) + 
						x1_z * ( 1.0f - dz ) + 
						x1_z1 * height;
		}
		return( height );
	}

	return( 0.0f );
}

/// 로딩된 속성 높이만 읽어오기 
float	CField::Get__AttrHeight( int cell_x, int cell_y )	
{
	if ( cell_x > m_cSize*2 || cell_x < 0 )	return 0.0f;
	if ( cell_y > m_cSize*2 || cell_y < 0 )	return 0.0f;

	if( m_pAttrMap ) 
	{
		return	(float)m_pAttrMap[cell_y][cell_x].nHeight;
	}	

	return 0.0f;
}

/// real-float 좌표로 지형 높이만 구하기
float	CField::Get__TerrainHeightF( float aX, float aZ )
{
	float dx = aX / 16.0f;
	float dz = aZ / 16.0f; 
	int ix = (int)( dx );
	int iz = (int)( dz );
	if( ix >= 0 && ix < m_cSize - 1 && iz >= 0 && iz < m_cSize - 1 )
	{
		dx -= ix;
		dz -= iz;
 
		/// 지형 타일 버텍스 높이
		float x_z = m_pDisplayMap[iz][ix].Pos[0].y;
		float x1_z = m_pDisplayMap[iz][ix + 1].Pos[0].y;
		float x_z1 = m_pDisplayMap[iz + 1][ix].Pos[0].y;
		float x1_z1 = m_pDisplayMap[iz + 1][ix + 1].Pos[0].y;

		float height = dx + dz - 1.0f;
		if( height <= 0.0f )
		{
			height =	x1_z * dx + 
						x_z1 * dz - 
						x_z * height;
		}
		else
		{
			height =	x_z1 * ( 1.0f - dx ) + 
						x1_z * ( 1.0f - dz ) + 
						x1_z1 * height;
		}
		return( height );
	}

	return( 0.0f );
}

float	CField::Get__CharacterMoveHeight( float curPosX, float curPosY, int curCX, int curCY, int nextCX, int nextCY )
{
	
	//by simwoosung
	//---------------------curCX
	if( curCX < 0 )
	{
		curCX = 0;
	}

	if( ATTR_MAPSIZE <= curCX )
	{
		curCX = ATTR_MAPSIZE - 1;
	}
	
	//---------------------curCY
	if( curCY < 0 )
	{
		curCY = 0;
	}

	if( ATTR_MAPSIZE <= curCY )
	{
		curCY = ATTR_MAPSIZE - 1;
	}

	//---------------------nextCX
	if( nextCX < 0 )
	{
		nextCX = 0;
	}

	if( ATTR_MAPSIZE <= nextCX )
	{
		nextCX = ATTR_MAPSIZE - 1;
	}

	//---------------------nextCY
	if( nextCY < 0 )
	{
		nextCY = 0;
	}

	if( ATTR_MAPSIZE <= nextCY )
	{
		nextCY = ATTR_MAPSIZE - 1;
	}
	
	if( m_pAttrMap[curCY][curCX].nHeight == 0 && m_pAttrMap[nextCY][nextCX].nHeight == 0 )	
	{
		return( Get__TerrainHeightF( curPosX, curPosY ) );	/// 그냥 지형 높이 
	}
	else if( m_pAttrMap[curCY][curCX].nHeight > 0 && m_pAttrMap[nextCY][nextCX].nHeight > 0 )
	{
//		if( m_pAttrMap[curCY][curCX].nHeight == m_pAttrMap[nextCY][nextCX].nHeight && 
//			m_pAttrMap[curCY][curCX].nHeight == m_pAttrMap[nextCY * 2 - curCY][nextCX * 2 - curCX].nHeight ) 
//		{
//			return( m_pAttrMap[curCY][curCX].nHeight ); 
//		}
	}

	return( Get__MapHeightF( curPosX, curPosY ) );
}

/// <---------------------------------------------- sooree height 
