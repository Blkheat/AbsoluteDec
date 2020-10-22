#include "RockPCH.h"

#include	"..\\quadlist.h"
#include	<BASE\\d3dapp.h>
#include	"..\\camera.h"

#include	"Field.h"

/****************************************************************************************************
****************************************************************************************************
****************************************************************************************************
	* Geometry Edit
****************************************************************************************************
****************************************************************************************************
****************************************************************************************************/

/*
	* PickPoint
*/
HRESULT		CField::PickPoint(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight)
{
	if ( m_pDisplayMap == NULL )
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

	for ( DWORD y = sy; y < ey; y ++ )
	{
		for ( DWORD x = sx; x < ex; x ++ )
		{
			if ( m_pDisplayMap[y][x].bIsVisible )
			{
				v1[0].x	= x*TILE_SIZE - (TILE_SIZE/2);
				v1[0].y	= m_pDisplayMap[y][x].Pos[0].y;//(float)m_pHeightField[(y*(m_dwSize+1))+x] * HEIGHT_VALUE;
				v1[0].z	= y*TILE_SIZE - (TILE_SIZE/2);
				v1[1].x	= x*TILE_SIZE - (TILE_SIZE/2);
				v1[1].y	= m_pDisplayMap[y][x].Pos[1].y;//(float)m_pHeightField[(y+1)*(m_dwSize+1)+x] * HEIGHT_VALUE;
				v1[1].z	= (y+1)*TILE_SIZE - (TILE_SIZE/2);
				v1[2].x	= (x+1)*TILE_SIZE - (TILE_SIZE/2);
				v1[2].y	= m_pDisplayMap[y][x].Pos[3].y;//(float)m_pHeightField[(y*(m_dwSize+1))+x+1] * HEIGHT_VALUE;
				v1[2].z	= y*TILE_SIZE - (TILE_SIZE/2);

				v2[0].x	= (x+1)*TILE_SIZE - (TILE_SIZE/2);
				v2[0].y	= m_pDisplayMap[y][x].Pos[2].y;//(float)m_pHeightField[(y+1)*(m_dwSize+1)+x+1] * HEIGHT_VALUE;
				v2[0].z	= (y+1)*TILE_SIZE - (TILE_SIZE/2);
				v2[1].x	= (x+1)*TILE_SIZE - (TILE_SIZE/2);
				v2[1].y	= m_pDisplayMap[y][x].Pos[3].y;//(float)m_pHeightField[(y*(m_dwSize+1))+x+1] * HEIGHT_VALUE;
				v2[1].z	= y*TILE_SIZE - (TILE_SIZE/2);
				v2[2].x	= x*TILE_SIZE - (TILE_SIZE/2);
				v2[2].y	= m_pDisplayMap[y][x].Pos[1].y;//(float)m_pHeightField[(y+1)*(m_dwSize+1)+x] * HEIGHT_VALUE;
				v2[2].z	= (y+1)*TILE_SIZE - (TILE_SIZE/2);

				if (IntersectTriangle(vPickRayOrig, vPickRayDir, v1[0], v1[1], v1[2], &fDist, &fBary1, &fBary2) 
				||	IntersectTriangle(vPickRayOrig, vPickRayDir, v2[0], v2[1], v2[2], &fDist, &fBary1, &fBary2) )
				{
					m_vPickedPt.x	= x*TILE_SIZE;
					m_vPickedPt.y	= v1[0].y;
					m_vPickedPt.z	= y*TILE_SIZE;

//					m_dwPickedX	= x;
//					m_dwPickedY	= y;

					return S_OK;
				}
			}
		}
	}
	return S_OK;
}

void		CField::UpdateVtxHeight(DWORD x, DWORD y, float height, bool plane)
{
	if ( y < 0 && y < m_cSize+1 )
		return;
	if ( x < 0 && x < m_cSize+1 )
		return;
	if ( y >= m_cSize )
		return;
	if ( x >= m_cSize )
		return;

	if ( m_pDisplayMap[y][x].Pos[0].y <= 0.0f && height < 0.0f )
		return;

	if ( !plane )
	{
		m_pDisplayMap[y][x].Pos[0].y	+= height;
		if ( x-1 >= 0 && x-1 < m_cSize+1 )
			m_pDisplayMap[y][x-1].Pos[3].y	+= height;
		if ( y-1 >= 0 && y-1 < m_cSize+1 )
			m_pDisplayMap[y-1][x].Pos[1].y	+= height;
		if ( x-1 >= 0 && x-1 < m_cSize+1 && y-1 >= 0 && y-1 < m_cSize+1 )
			m_pDisplayMap[y-1][x-1].Pos[2].y	+= height;
	}
	if ( plane )
	{
		m_pDisplayMap[y][x].Pos[0].y	= height;
		if ( x-1 >= 0 && x-1 < m_cSize+1 )
			m_pDisplayMap[y][x-1].Pos[3].y	= height;
		if ( y-1 >= 0 && y-1 < m_cSize+1 )
			m_pDisplayMap[y-1][x].Pos[1].y	= height;
		if ( x-1 >= 0 && x-1 < m_cSize+1 && y-1 >= 0 && y-1 < m_cSize+1 )
			m_pDisplayMap[y-1][x-1].Pos[2].y	= height;
	}
}

void		CField::UpdateHeight(DWORD RANGE)
{
	float		height;
	if ( m_dwGEditDir == GED_UP )
		height	= 1.0f;
	else
		height	= -1.0f;

	/* update one vtx */
	if ( RANGE == 0 )
	{
		for ( DWORD y = sy; y < ey; y ++ )
		{
			for ( DWORD x = sx; x < ex; x ++ )
			{
				for ( DWORD i = 0; i < 4; i ++ )
				{
					if ( m_vPickedPt.x == m_pDisplayMap[y][x].Pos[i].x &&
						 m_vPickedPt.y == m_pDisplayMap[y][x].Pos[i].y &&
						 m_vPickedPt.z == m_pDisplayMap[y][x].Pos[i].z )
					{
						m_pDisplayMap[y][x].Pos[i].y += height;
						if ( y+1 < m_cSize )
							m_pDisplayMap[y+1][x].Pos[3].y += height;
						if ( y+1 < m_cSize && x+1 < m_cSize )
							m_pDisplayMap[y+1][x+1].Pos[0].y += height;
						if ( x+1 < m_cSize )
							m_pDisplayMap[y][x+1].Pos[1].y += height;
						return;
					}
				}
			}
		}
	}

	else if ( m_dwGEditType == GEV_PLANEONVOLUME )
	{
		DWORD		ptidx_x	= (DWORD)m_vPickedPt.x / (DWORD)TILE_SIZE;
		DWORD		ptidx_y = (DWORD)m_vPickedPt.z / (DWORD)TILE_SIZE;
		DWORD		sx, sy, ex, ey;
		
		// picked vtx
		UpdateVtxHeight(ptidx_x, ptidx_y, m_pDisplayMap[ptidx_y][ptidx_x].Pos[0].y, true);

		for ( DWORD i = 0; i < RANGE; i ++ )
		{
			sy	= ptidx_y - (i+1);
			ey	= ptidx_y + (i+1);
			sx	= ptidx_x - (i+1);
			for ( DWORD y = sy; y < ey+1; y ++ )
			{
				UpdateVtxHeight(sx, y, m_pDisplayMap[ptidx_y][ptidx_x].Pos[0].y, true);
			}
			sx	= ptidx_x - (i+1);
			ex	= ptidx_x + (i+1);
			sy	= ptidx_y + (i+1);
			for ( DWORD x = sx+1; x < ex+1; x ++ )
			{
				UpdateVtxHeight(x, sy, m_pDisplayMap[ptidx_y][ptidx_x].Pos[0].y, true);
			}
			sx	= ptidx_x - (i+1);
			ex	= ptidx_x + (i+1);
			sy	= ptidx_y - (i+1);
			for (DWORD x = sx+1; x < ex+1; x ++ )
			{
				UpdateVtxHeight(x, sy, m_pDisplayMap[ptidx_y][ptidx_x].Pos[0].y, true);
			}
			sy	= ptidx_y + (i+1);
			ey	= ptidx_y - (i+1);
			sx	= ptidx_x + (i+1);
			for (DWORD y = sy-1; y > ey; y -- )
			{
				UpdateVtxHeight(sx, y, m_pDisplayMap[ptidx_y][ptidx_x].Pos[0].y, true);
			}
		}
	}
	
	else if ( m_dwGEditType == GEV_VOLUME )
	{
		DWORD		ptidx_x	= (DWORD)m_vPickedPt.x / (DWORD)TILE_SIZE;
		DWORD		ptidx_y = (DWORD)m_vPickedPt.z / (DWORD)TILE_SIZE;
		DWORD		sx, sy, ex, ey;
		
		// picked vtx
		UpdateVtxHeight(ptidx_x, ptidx_y, RANGE*height);

		for ( DWORD i = 0; i < RANGE; i ++ )
		{
			sy	= ptidx_y - (i+1);
			ey	= ptidx_y + (i+1);
			sx	= ptidx_x - (i+1);
			for ( DWORD y = sy; y < ey+1; y ++ )
			{
				UpdateVtxHeight(sx, y, (RANGE-i)*height);
			}
			sx	= ptidx_x - (i+1);
			ex	= ptidx_x + (i+1);
			sy	= ptidx_y + (i+1);
			for ( DWORD x = sx+1; x < ex+1; x ++ )
			{
				UpdateVtxHeight(x, sy, (RANGE-i)*height);
			}
			sx	= ptidx_x - (i+1);
			ex	= ptidx_x + (i+1);
			sy	= ptidx_y - (i+1);
			for (DWORD x = sx+1; x < ex+1; x ++ )
			{
				UpdateVtxHeight(x, sy, (RANGE-i)*height);
			}
			sy	= ptidx_y + (i+1);
			ey	= ptidx_y - (i+1);
			sx	= ptidx_x + (i+1);
			for (DWORD y = sy-1; y > ey; y -- )
			{
				UpdateVtxHeight(sx, y, (RANGE-i)*height);
			}
		}
	}

	else if ( m_dwGEditType == GEV_PLANE )
	{
		DWORD		ptidx_x	= (DWORD)m_vPickedPt.x / (DWORD)TILE_SIZE;
		DWORD		ptidx_y = (DWORD)m_vPickedPt.z / (DWORD)TILE_SIZE;
		DWORD		sx, sy, ex, ey;
		
		// picked vtx
		UpdateVtxHeight(ptidx_x, ptidx_y, height, false);

		for ( DWORD i = 0; i < RANGE; i ++ )
		{
			sy	= ptidx_y - (i+1);
			ey	= ptidx_y + (i+1);
			sx	= ptidx_x - (i+1);
			for ( DWORD y = sy; y < ey+1; y ++ )
			{
				UpdateVtxHeight(sx, y, height, false);
			}
			sx	= ptidx_x - (i+1);
			ex	= ptidx_x + (i+1);
			sy	= ptidx_y + (i+1);
			for ( DWORD x = sx+1; x < ex+1; x ++ )
			{
				UpdateVtxHeight(x, sy, height, false);
			}
			sx	= ptidx_x - (i+1);
			ex	= ptidx_x + (i+1);
			sy	= ptidx_y - (i+1);
			for (DWORD x = sx+1; x < ex+1; x ++ )
			{
				UpdateVtxHeight(x, sy, height, false);
			}
			sy	= ptidx_y + (i+1);
			ey	= ptidx_y - (i+1);
			sx	= ptidx_x + (i+1);
			for (DWORD y = sy-1; y > ey; y -- )
			{
				UpdateVtxHeight(sx, y, height, false);
			}
		}
	}
	BuildNormal();
}

void		CField::MakeFaceNormal(int indexX,int indexY,int pos) //block index, pos
{
/*	int x,y;
	x = indexX;
	y = indexY;


	float result1[3],result2[3],result[3];
	float length;

	//소스만들고
	result1[0] = 	m_pDisplayMap[indexY][indexX].Pos[0].x - m_pDisplayMap[indexY][indexX].Pos[1].x;
	result1[1] = 	m_pDisplayMap[indexY][indexX].Pos[0].y - m_pDisplayMap[indexY][indexX].Pos[1].y;
	result1[2] = 	m_pDisplayMap[indexY][indexX].Pos[0].z - m_pDisplayMap[indexY][indexX].Pos[1].z;

	result2[0] = 	m_pDisplayMap[indexY][indexX].Pos[1].x - m_pDisplayMap[indexY][indexX].Pos[3].x;
	result2[1] = 	m_pDisplayMap[indexY][indexX].Pos[1].y - m_pDisplayMap[indexY][indexX].Pos[3].y;
	result2[2] = 	m_pDisplayMap[indexY][indexX].Pos[1].z - m_pDisplayMap[indexY][indexX].Pos[3].z;

	result[0] = result1[1] * result2[2] - result1[2] * result2[1];
	result[1] = result1[2] * result2[0] - result1[0] * result2[2];
	result[2] = result1[0] * result2[1] - result1[1] * result2[0];
	

	length = sqrtf((result[0]*result[0]) + 
				   (result[1]*result[1]) +
				   (result[2]*result[2]));

	if (length == 0.0f) length = 1.0f;

	result[0] /= length;
	result[1] /= length;
	result[2] /= length;

	m_pDisplayMap[y][x].FaceNormalPos[0].normal[0] = result[0];	//x	
	m_pDisplayMap[y][x].FaceNormalPos[0].normal[1] = result[1];	//y
	m_pDisplayMap[y][x].FaceNormalPos[0].normal[2] = result[2];	//z

	//두번째 소스만들고
	result1[0] = 	m_pDisplayMap[indexY][indexX].Pos[2].x - m_pDisplayMap[indexY][indexX].Pos[3].x;
	result1[1] = 	m_pDisplayMap[indexY][indexX].Pos[2].y - m_pDisplayMap[indexY][indexX].Pos[3].y;
	result1[2] = 	m_pDisplayMap[indexY][indexX].Pos[2].z - m_pDisplayMap[indexY][indexX].Pos[3].z;

	result2[0] = 	m_pDisplayMap[indexY][indexX].Pos[3].x - m_pDisplayMap[indexY][indexX].Pos[1].x;
	result2[1] = 	m_pDisplayMap[indexY][indexX].Pos[3].y - m_pDisplayMap[indexY][indexX].Pos[1].y;
	result2[2] = 	m_pDisplayMap[indexY][indexX].Pos[3].z - m_pDisplayMap[indexY][indexX].Pos[1].z;

	result[0] = result1[1] * result2[2] - result1[2] * result2[1];
	result[1] = result1[2] * result2[0] - result1[0] * result2[2];
	result[2] = result1[0] * result2[1] - result1[1] * result2[0];
	

	length = sqrtf((result[0]*result[0]) + 
				   (result[1]*result[1]) +
				   (result[2]*result[2]));

	if (length == 0.0f) length = 1.0f;

	result[0] /= length;
	result[1] /= length;
	result[2] /= length;


	m_pDisplayMap[y][x].FaceNormalPos[1].normal[0] = result[0];
	m_pDisplayMap[y][x].FaceNormalPos[1].normal[1] = result[1];
	m_pDisplayMap[y][x].FaceNormalPos[1].normal[2] = result[2];
*/
}
/*
0,0,-1,-1,-1,0
0,0,0,1,1,1
1,0,1,0,1,0

0,0,0,1,1,1
0,1,1,1,0,0
1,0,1,0,1,0


0,0,1,-1,-1,0
0,0,0,-1,-1,-1
0,1,0,1,0,1

0,0,0,-1,-1,-1
0,-1,-1,-1,0,0
0,0,1,1,0,1
*/
int arrIndexX[4][6] = { {0,0,0,-1,-1,-1} ,
					    {0,0,-1,-1,-1,0},
						{0,0,0,1,1,1},
						{0,0,1,1,1,0},
						};
int arrIndexY[4][6] = { {0,-1,-1,-1,0,0}, 
					    {0,0,0,1,1,1},
						{0,1,1,1,0,0},
						{0,0,0,-1,-1,-1},
						};

int arrIndexPos[4][6] = { 
						{0,1,0,1,0,1}, 
					    {1,0,1,0,1,0},
						{1,0,1,0,1,0},
						{0,1,0,1,0,1},
						};

void		CField::MakeVertexNormal(int indexX, int indexY, int pos) //block index, pos
{
/*	DWORD tempx = indexX,tempy = indexY;
	DWORD count = 0;
	m_pDisplayMap[indexY][indexX].Pos[pos].nx = 0;
	m_pDisplayMap[indexY][indexX].Pos[pos].ny = 0;
	m_pDisplayMap[indexY][indexX].Pos[pos].nz = 0;

	for (int i = 0 ; i < 6 ; i++)
	{
		tempx = indexX,tempy = indexY;
		tempx += arrIndexX[pos][i];
		tempy += arrIndexY[pos][i];

		if (tempx < m_cSize && tempx >= 0)
		{
			if (tempy < m_cSize && tempy >= 0)
			{
				m_pDisplayMap[indexY][indexX].Pos[pos].nx += 
									m_pDisplayMap[tempy][tempx].FaceNormalPos[arrIndexPos[pos][i]].normal[0];
				m_pDisplayMap[indexY][indexX].Pos[pos].ny += 
									m_pDisplayMap[tempy][tempx].FaceNormalPos[arrIndexPos[pos][i]].normal[1];
				m_pDisplayMap[indexY][indexX].Pos[pos].nz += 
									m_pDisplayMap[tempy][tempx].FaceNormalPos[arrIndexPos[pos][i]].normal[2];
				count++;
			}
		}
	}

	m_pDisplayMap[indexY][indexX].Pos[pos].nx /= count;
	m_pDisplayMap[indexY][indexX].Pos[pos].ny /= count; 
	m_pDisplayMap[indexY][indexX].Pos[pos].nz /= count;
*/
}

void		CField::BuildNormal()
{
/*	for ( DWORD y = sy; y < ey; y ++ )
	{
		for ( DWORD x = sx; x < ex; x ++ )
		{
			MakeFaceNormal(x, y, 0);
		}
	}

	for ( y = sy; y < ey; y ++ )
	{
		for ( DWORD x = sx; x < ex; x ++ )
		{
			MakeVertexNormal(x, y, 0);
			MakeVertexNormal(x, y, 1);
			MakeVertexNormal(x, y, 2);
			MakeVertexNormal(x, y, 3);
		}
	}
	*/
}

void		CField::BuildNormalFull()
{
/*	for ( DWORD y = 0; y < m_cSize; y ++ )
	{
		for ( DWORD x = 0; x < m_cSize; x ++ )
		{
			MakeFaceNormal(x, y, 0);
		}
	}

	for ( y = 0; y < m_cSize; y ++ )
	{
		for ( DWORD x = 0; x < m_cSize; x ++ )
		{
			MakeVertexNormal(x, y, 0);
			MakeVertexNormal(x, y, 1);
			MakeVertexNormal(x, y, 2);
			MakeVertexNormal(x, y, 3);
		}
	}
	*/
}

/****************************************************************************************************
****************************************************************************************************
****************************************************************************************************
	* enf of GEOMETRY EDIT
****************************************************************************************************
****************************************************************************************************
****************************************************************************************************/