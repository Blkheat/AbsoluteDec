#include "RockPCH.h"

#include	"..\\quadlist.h"
#include	<BASE\\D3DBase.h>
#include	"Field.h"
#include	<..\\Camera.h>


/****************************************************************************************************
****************************************************************************************************
****************************************************************************************************
	* MAPPING
****************************************************************************************************
****************************************************************************************************
****************************************************************************************************/
HRESULT		CField::PickTile(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight)
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
				v1[0].x	= m_pDisplayMap[y][x].Pos[0].x;
				v1[0].y	= m_pDisplayMap[y][x].Pos[0].y;
				v1[0].z	= m_pDisplayMap[y][x].Pos[0].z;
				v1[1].x	= m_pDisplayMap[y][x].Pos[1].x;
				v1[1].y	= m_pDisplayMap[y][x].Pos[1].y;
				v1[1].z	= m_pDisplayMap[y][x].Pos[1].z;
				v1[2].x	= m_pDisplayMap[y][x].Pos[3].x;
				v1[2].y	= m_pDisplayMap[y][x].Pos[3].y;
				v1[2].z	= m_pDisplayMap[y][x].Pos[3].z;

				v2[0].x	= m_pDisplayMap[y][x].Pos[2].x;
				v2[0].y	= m_pDisplayMap[y][x].Pos[2].y;
				v2[0].z	= m_pDisplayMap[y][x].Pos[2].z;
				v2[1].x	= m_pDisplayMap[y][x].Pos[3].x;
				v2[1].y	= m_pDisplayMap[y][x].Pos[3].y;
				v2[1].z	= m_pDisplayMap[y][x].Pos[3].z;
				v2[2].x	= m_pDisplayMap[y][x].Pos[1].x;
				v2[2].y	= m_pDisplayMap[y][x].Pos[1].y;
				v2[2].z	= m_pDisplayMap[y][x].Pos[1].z;

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

/*

	* TXIDX VALUE

	* LEFT, TOP, RIGHT, BOTTOM, T-LEFT, T-RIGHT, B-LEFT, B-RIGHT, TI-LEFT, TI-RIGHT, BI-LEFT, BI-RIGHT 

*/
//#define			TXWIDTH			16
#define			TXWIDTH			50
#define			TXDIR			12
#define			TXELE			4
#define			TXKIND			2
#define			TXKIND_CENTER	4
#define			TXKIND_CENTER2	8
#define			TXKIND_DEFAULT	4
#define			TXMAXIDX		40


static const unsigned int	TXDefault[TXKIND_DEFAULT]		= 
{26,27,30,31};

static const unsigned int	TXCenter[TXKIND_CENTER]	=
{5,6,17,18};

static const unsigned int	TXCenter2[TXKIND_CENTER2]	= 
{5,6,17,18,26,27,30,31};

static const unsigned int	TXBasic[TXDIR][TXKIND]	=
{
	/* LEFT */		{4,16},
	/* TOP */		{1,2},
	/* RIGHT */		{7,19},
	/* BOTTOM */	{21,22},
	/* T-LEFT */	{0,24},
	/* T-RIGHT */	{3,25},
	/* B-LEFT */	{20,28},
	/* B-RIGHT */	{23,29},
	/* TI-LEFT */	{8,14},
	/* TI-RIGHT */	{11,13},
	/* BI-LEFT */	{10,12},
	/* BI-RIGHT */	{9,15}
};



/*********************************************************************************

							/ CHECK MAPPING IDX /

*********************************************************************************/

DWORD	CField::CheckLeft(DWORD idx, DWORD x, DWORD y)
{
	// check default
	DWORD i = 0;
	for ( i = 0; i < TXKIND_DEFAULT; i ++ )
	{
		if ( m_pDisplayMap[y][x-1].nTexIndex == TXDefault[i] )
			return TIDX_EMPTY;
	}

	// check center
	for ( i = 0; i < TXKIND_CENTER; i ++ )
	{
		if ( m_pDisplayMap[y][x-1].nTexIndex == TXCenter[i]+(idx*TXMAXIDX) )
			return TIDX_CENTER;
	}

	// 
	for ( i = 0; i < TXDIR; i ++ )
	{
		for ( DWORD j = 0; j < TXKIND; j ++ )
		{
			if ( m_pDisplayMap[y][x-1].nTexIndex == TXBasic[i][j]+(idx*TXMAXIDX) )
				return i;
		}
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckTop(DWORD idx, DWORD x, DWORD y)
{
	DWORD i = 0;
	for (  i = 0; i < TXKIND_DEFAULT; i ++ )
	{
		if ( m_pDisplayMap[y+1][x].nTexIndex == TXDefault[i] )
			return TIDX_EMPTY;
	}

	for ( i = 0; i < TXKIND_CENTER; i ++ )
	{
		if ( m_pDisplayMap[y+1][x].nTexIndex == TXCenter[i]+(idx*TXMAXIDX) )
			return TIDX_CENTER;
	}

	//
	for ( i = 0; i < TXDIR; i ++ )
	{
		for ( DWORD j = 0; j < TXKIND; j ++ )
		{
			if ( m_pDisplayMap[y+1][x].nTexIndex == TXBasic[i][j]+(idx*TXMAXIDX) )
				return i;
		}
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckRight(DWORD idx, DWORD x, DWORD y)
{
	DWORD i = 0;
	for (  i = 0; i < TXKIND_DEFAULT; i ++ )
	{
		if ( m_pDisplayMap[y][x+1].nTexIndex == TXDefault[i] )
			return TIDX_EMPTY;
	}

	for ( i = 0; i < TXKIND_CENTER; i ++ )
	{
		if ( m_pDisplayMap[y][x+1].nTexIndex == TXCenter[i]+(idx*TXMAXIDX) )
			return TIDX_CENTER;
	}

	//
	for ( i = 0; i < TXDIR; i ++ )
	{
		for ( DWORD j = 0; j < TXKIND; j ++ )
		{
			if ( m_pDisplayMap[y][x+1].nTexIndex == TXBasic[i][j]+(idx*TXMAXIDX) )
				return i;
		}
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckBottom(DWORD idx, DWORD x, DWORD y)
{
	DWORD i = 0;
	for ( i = 0; i < TXKIND_DEFAULT; i ++ )
	{
		if ( m_pDisplayMap[y-1][x].nTexIndex == TXDefault[i] )
			return TIDX_EMPTY;
	}

	for ( i = 0; i < TXKIND_CENTER; i ++ )
	{
		if ( m_pDisplayMap[y-1][x].nTexIndex == TXCenter[i]+(idx*TXMAXIDX) )
			return TIDX_CENTER;
	}

	//
	for ( i = 0; i < TXDIR; i ++ )
	{
		for ( DWORD j = 0; j < TXKIND; j ++ )
		{
			if ( m_pDisplayMap[y-1][x].nTexIndex == TXBasic[i][j]+(idx*TXMAXIDX) )
				return i;
		}
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckTopLeft(DWORD idx, DWORD x, DWORD y)
{
	DWORD i = 0;
	for ( i = 0; i < TXKIND_DEFAULT; i ++ )
	{
		if ( m_pDisplayMap[y+1][x-1].nTexIndex == TXDefault[i] )
			return TIDX_EMPTY;
	}

	for ( i = 0; i < TXKIND_CENTER; i ++ )
	{
		if ( m_pDisplayMap[y+1][x-1].nTexIndex == TXCenter[i]+(idx*TXMAXIDX) )
			return TIDX_CENTER;
	}

	//
	for ( i = 0; i < TXDIR; i ++ )
	{
		for ( DWORD j = 0; j < TXKIND; j ++ )
		{
			if ( m_pDisplayMap[y+1][x-1].nTexIndex == TXBasic[i][j]+(idx*TXMAXIDX) )
				return i;
		}
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckTopRight(DWORD idx, DWORD x, DWORD y )
{
	DWORD i = 0;
	for ( i = 0; i < TXKIND_DEFAULT; i ++ )
	{
		if ( m_pDisplayMap[y+1][x+1].nTexIndex == TXDefault[i] )
			return TIDX_EMPTY;
	}

	for ( i = 0; i < TXKIND_CENTER; i ++ )
	{
		if ( m_pDisplayMap[y+1][x+1].nTexIndex == TXCenter[i]+(idx*TXMAXIDX) )
			return TIDX_CENTER;
	}

	//
	for ( i = 0; i < TXDIR; i ++ )
	{
		for ( DWORD j = 0; j < TXKIND; j ++ )
		{
			if ( m_pDisplayMap[y+1][x+1].nTexIndex == TXBasic[i][j]+(idx*TXMAXIDX) )
				return i;
		}
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckBotLeft(DWORD idx, DWORD x, DWORD y)
{
	DWORD i = 0;
	for ( i = 0; i < TXKIND_DEFAULT; i ++ )
	{
		if ( m_pDisplayMap[y-1][x-1].nTexIndex == TXDefault[i] )
			return TIDX_EMPTY;
	}

	for ( i = 0; i < TXKIND_CENTER; i ++ )
	{
		if ( m_pDisplayMap[y-1][x-1].nTexIndex == TXCenter[i]+(idx*TXMAXIDX) )
			return TIDX_CENTER;
	}

	//
	for ( i = 0; i < TXDIR; i ++ )
	{
		for ( DWORD j = 0; j < TXKIND; j ++ )
		{
			if ( m_pDisplayMap[y-1][x-1].nTexIndex == TXBasic[i][j]+(idx*TXMAXIDX) )
				return i;
		}
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckBotRight(DWORD idx, DWORD x, DWORD y)
{
	DWORD i = 0;
	for ( i = 0; i < TXKIND_DEFAULT; i ++ )
	{
		if ( m_pDisplayMap[y-1][x+1].nTexIndex == TXDefault[i] )
			return TIDX_EMPTY;
	}

	for ( i = 0; i < TXKIND_CENTER; i ++ )
	{
		if ( m_pDisplayMap[y-1][x+1].nTexIndex == TXCenter[i]+(idx*TXMAXIDX) )
			return TIDX_CENTER;
	}

	//
	for ( i = 0; i < TXDIR; i ++ )
	{
		for ( DWORD j = 0; j < TXKIND; j ++ )
		{
			if ( m_pDisplayMap[y-1][x+1].nTexIndex == TXBasic[i][j]+(idx*TXMAXIDX) )
				return i;
		}
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckCenter(DWORD idx, DWORD x, DWORD y)
{
	DWORD i = 0;
	for ( i = 0; i < TXKIND_DEFAULT; i ++ )
	{
		if ( m_pDisplayMap[y][x].nTexIndex == TXDefault[i] )
			return	TIDX_EMPTY;
	}

	for ( i = 0; i < TXKIND_CENTER; i ++ )
	{
		if ( m_pDisplayMap[y][x].nTexIndex == TXCenter[i]+(idx*TXMAXIDX) )
			return TIDX_CENTER;
	}

	//
	for ( i = 0; i < TXDIR; i ++ )
	{
		for ( DWORD j = 0; j < TXKIND; j ++ )
		{
			if ( m_pDisplayMap[y][x].nTexIndex == TXBasic[i][j]+(idx*TXMAXIDX) )
				return i;
		}
	}

	return TIDX_EMPTY;
}
/*********************************************************************************

							/ end of CHECK MAPPING IDX /

*********************************************************************************/
DWORD	CField::GetIdxType(DWORD idx, DWORD x, DWORD y)
{
	DWORD	Top, Left, Right, Bottom;
	DWORD	TopLeft, TopRight, BotLeft, BotRight;
	DWORD	Center;

	Top			= CheckTop(idx, x, y);
	Left		= CheckLeft(idx, x, y);
	Right		= CheckRight(idx, x, y);
	Bottom		= CheckBottom(idx, x, y);

	TopLeft		= CheckTopLeft(idx, x, y);
	TopRight	= CheckTopRight(idx, x, y);
	BotLeft		= CheckBotLeft(idx, x, y);
	BotRight	= CheckBotRight(idx, x, y);

	Center		= CheckCenter(idx, x, y);

	// check top-left
	if ( (Center != TIDX_CENTER) && (Top == TIDX_EMPTY) && (Left == TIDX_EMPTY) && (BotRight == TIDX_CENTER) )
		return TIDX_TOPLEFT;
	else if ( (Center != TIDX_CENTER) && (Bottom == TIDX_CENTER) && (Right == TIDX_CENTER) )
		return TIDX_INV_TOPLEFT;
	// check top-right
	if ( (Center != TIDX_CENTER) && (Top == TIDX_EMPTY) && (Right == TIDX_EMPTY) && (BotLeft == TIDX_CENTER) )
		return TIDX_TOPRIGHT;
	else if ( (Center != TIDX_CENTER) && (Bottom == TIDX_CENTER) && (Left == TIDX_CENTER) )
		return TIDX_INV_TOPRIGHT;
	// check bottom-left
	if ( (Center != TIDX_CENTER) && (Bottom == TIDX_EMPTY) && (Left == TIDX_EMPTY) && (TopRight == TIDX_CENTER) )
		return TIDX_BOTLEFT;
	else if ( (Center != TIDX_CENTER) && (Top == TIDX_CENTER) && (Right == TIDX_CENTER) )
		return TIDX_INV_BOTLEFT;
	// check bottom-right
	if ( (Center != TIDX_CENTER) && (Bottom == TIDX_EMPTY) && (Right == TIDX_EMPTY) && (TopLeft == TIDX_CENTER) )
		return TIDX_BOTRIGHT;
	else if ( (Center != TIDX_CENTER) && (Top == TIDX_CENTER) && (Left == TIDX_CENTER) )
		return TIDX_INV_BOTRIGHT;
	// check left
	if ( (Center != TIDX_CENTER) && (Left == TIDX_EMPTY) && (Right == TIDX_CENTER) )
		return TIDX_LEFT;
	// check right
	if ( (Center != TIDX_CENTER) && (Right == TIDX_EMPTY) && (Left == TIDX_CENTER) )
		return TIDX_RIGHT;
	// check top
	if ( (Center != TIDX_CENTER) && (Top == TIDX_EMPTY) && (Bottom == TIDX_CENTER) )
		return TIDX_TOP;
	// check bottom
	if ( (Center != TIDX_CENTER) && (Bottom == TIDX_EMPTY) && (Top == TIDX_CENTER) )
		return TIDX_BOTTOM;

	return TIDX_CENTER;
}


void	CField::SetupIdx(DWORD type, DWORD idx, DWORD x, DWORD y)
{
	switch ( type )
	{
	case	TIDX_TOPLEFT:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_TOPLEFT][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_TOPRIGHT:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_TOPRIGHT][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_BOTLEFT:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_BOTLEFT][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_BOTRIGHT:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_BOTRIGHT][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_INV_TOPLEFT:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_INV_TOPLEFT][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_INV_TOPRIGHT:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_INV_TOPRIGHT][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_INV_BOTLEFT:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_INV_BOTLEFT][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_INV_BOTRIGHT:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_INV_BOTRIGHT][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_TOP:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_TOP][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_LEFT:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_LEFT][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_RIGHT:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_RIGHT][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	case	TIDX_BOTTOM:
		m_pDisplayMap[y][x].nTexIndex	= TXBasic[TIDX_BOTTOM][rand()%TXKIND]+(idx*TXMAXIDX);
		break;
	}
	SetupUV(x,y,idx);
}

/*********************************************************************************

							/ Major Mapping Function /

*********************************************************************************/
DWORD	TXBasicDir[8][2]	= {
	{-1,0},
	{0,1},
	{1,0},
	{0,-1},
	{-1,1},
	{1,1},
	{-1,-1},
	{1,-1}
};

DWORD	TXBasicDir4x[4][2]	= {
	{0,0},
	{1,0},
	{0,-1},
	{1,-1},
};


void	CField::SetupLibIndex()
{
	DWORD	x	= m_dwPickedX;
	DWORD	y	= m_dwPickedY;
	DWORD	ele	= m_dwSelectedEle;

	if ( x <= 1 || y <= 1 || x >= m_cSize-2 || y >= m_cSize-2 )
		return;

	DWORD	srcEle = 0, srcType = 0;

	/* Final Setup */

	m_pDisplayMap[y][x].nTexIndex	= TXCenter2[rand()%TXKIND_CENTER2]+(ele*TXMAXIDX);

	SetupUV(x,y,ele);
//	m_pRaderView->UpdateColor(x, y);

	for ( DWORD i = 0; i < 8; i ++ )
	{
		x	= m_dwPickedX + TXBasicDir[7-i][0];
		y	= m_dwPickedY + TXBasicDir[7-i][1];

		SetupIdx(GetIdxType(ele,x,y),ele,x,y);
	}

}
/*********************************************************************************

						/ end of Major Mapping Function /

*********************************************************************************/

void	CField::SetupUV(DWORD x, DWORD y, DWORD idx)
{
	const float	xoffset	= 1.0f/TXWIDTH;
	const float yoffset	= 0.1f;
	float offset	= xoffset * (idx*4) + ((idx*xoffset)+(xoffset/2));

	float	u, v;

	u	= (m_pDisplayMap[y][x].nTexIndex - (TXMAXIDX*idx))%4;
	v	= (m_pDisplayMap[y][x].nTexIndex - (TXMAXIDX*idx))/4;


	m_pDisplayMap[y][x].Pos[0].tu	= u*xoffset + offset;
	m_pDisplayMap[y][x].Pos[0].tv	= (v+1)*yoffset;
	m_pDisplayMap[y][x].Pos[1].tu	= u*xoffset + offset;
	m_pDisplayMap[y][x].Pos[1].tv	= v*yoffset;
	m_pDisplayMap[y][x].Pos[2].tu	= (u+1)*xoffset + offset;
	m_pDisplayMap[y][x].Pos[2].tv	= v*yoffset;
	m_pDisplayMap[y][x].Pos[3].tu	= (u+1)*xoffset + offset;
	m_pDisplayMap[y][x].Pos[3].tv	= (v+1)*yoffset ;
}

void	CField::SetupUV(DWORD x, DWORD y)
{
	const float	xoffset	= 1.0f/TXWIDTH;
	const float yoffset	= 0.1f;
	float offset	= 0.0f;
	DWORD	idx;
	float	u, v;

	idx	= m_pDisplayMap[y][x].nTexIndex / TXMAXIDX;
	u	= (m_pDisplayMap[y][x].nTexIndex - (TXMAXIDX*idx))%4;
	v	= (m_pDisplayMap[y][x].nTexIndex - (TXMAXIDX*idx))/4;
	offset	= xoffset * (idx*4) + ((idx*xoffset)+(xoffset/2));

	m_pDisplayMap[y][x].Pos[0].tu	= u*xoffset + offset;
	m_pDisplayMap[y][x].Pos[0].tv	= (v+1)*yoffset;
	m_pDisplayMap[y][x].Pos[1].tu	= u*xoffset + offset;
	m_pDisplayMap[y][x].Pos[1].tv	= v*yoffset;
	m_pDisplayMap[y][x].Pos[2].tu	= (u+1)*xoffset + offset;
	m_pDisplayMap[y][x].Pos[2].tv	= v*yoffset;
	m_pDisplayMap[y][x].Pos[3].tu	= (u+1)*xoffset + offset;
	m_pDisplayMap[y][x].Pos[3].tv	= (v+1)*yoffset;
}
/*
void	CField::SetupUV()
{
	const float	xoffset	= 1.0f/TXWIDTH;
	const float yoffset	= 0.1f;
	float	u, v;
	DWORD	idx;
	float	offset;

	for ( DWORD y = 0; y < m_cSize; y ++ )
	{
		for ( DWORD x = 0; x < m_cSize; x ++ )
		{
			u	= m_pDisplayMap[y][x].nTexIndex%TXWIDTH;
			v	= m_pDisplayMap[y][x].nTexIndex/TXWIDTH;
			idx	= m_pDisplayMap[y][x].nTexIndex / TXMAXIDX;
			offset	= 

			m_pDisplayMap[y][x].Pos[0].tu	= u*xoffset;
			m_pDisplayMap[y][x].Pos[0].tv	= (v+1)*yoffset;
			m_pDisplayMap[y][x].Pos[1].tu	= u*xoffset;
			m_pDisplayMap[y][x].Pos[1].tv	= v*yoffset;
			m_pDisplayMap[y][x].Pos[2].tu	= (u+1)*xoffset;
			m_pDisplayMap[y][x].Pos[2].tv	= v*yoffset;
			m_pDisplayMap[y][x].Pos[3].tu	= (u+1)*xoffset;
			m_pDisplayMap[y][x].Pos[3].tv	= (v+1)*yoffset;
		}
	}
}
*/
void	CField::SetupDefaultTexIndex()
{
	const float	xoffset	= 1.0f/TXWIDTH;
	const float yoffset	= 0.1f;
	float	u, v;

	for ( DWORD y = 0; y < m_cSize; y ++ )
	{
		for ( DWORD x = 0; x < m_cSize; x ++ )
		{
			m_pDisplayMap[y][x].nTexIndex	= TXDefault[rand()%TXKIND_DEFAULT];
			u	= m_pDisplayMap[y][x].nTexIndex%4;
			v	= m_pDisplayMap[y][x].nTexIndex/4;

			m_pDisplayMap[y][x].Pos[0].tu	= u*xoffset + (xoffset/2);
			m_pDisplayMap[y][x].Pos[0].tv	= (v+1)*yoffset;
			m_pDisplayMap[y][x].Pos[1].tu	= u*xoffset + (xoffset/2);
			m_pDisplayMap[y][x].Pos[1].tv	= v*yoffset;
			m_pDisplayMap[y][x].Pos[2].tu	= (u+1)*xoffset + (xoffset/2);
			m_pDisplayMap[y][x].Pos[2].tv	= v*yoffset;
			m_pDisplayMap[y][x].Pos[3].tu	= (u+1)*xoffset + (xoffset/2);
			m_pDisplayMap[y][x].Pos[3].tv	= (v+1)*yoffset;
		}
	}
}

void	CField::SetupIdxWithoutLib()
{
	if ( m_pDisplayMap )
	{
		m_pDisplayMap[m_dwPickedY][m_dwPickedX].nTexIndex	= m_dwPickedIdx;
		SetupUV(m_dwPickedX, m_dwPickedY, m_dwSelectedEle);
	}
}

/****************************************************************************************************
****************************************************************************************************
****************************************************************************************************
	* end of MAPPING
****************************************************************************************************
****************************************************************************************************
****************************************************************************************************/

float	TILEDUV[14][4][2]	= 
{
	{{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f}},	//TT_LEFT		
	{{0.0f,1.0f},{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f}},	//TT_TOP		
	{{1.0f,1.0f},{0.0f,1.0f},{0.0f,0.0f},{1.0f,0.0f}},	//TT_RIGHT	
	{{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f},{0.0f,0.0f}},	//TT_BOTTOM	

	{{0.0f,1.0f},{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f}},	//TT_TLEFT	
	{{1.0f,1.0f},{1.0f,0.0f},{0.0f,0.0f},{0.0f,1.0f}},	//TT_TRIGHT	
	{{0.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f},{1.0f,0.0f}},	//TT_BLEFT	
	{{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f},{0.0f,0.0f}},	//TT_BRIGHT	
	
	{{0.0f,1.0f},{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f}},	//TT_TILEFT	
	{{1.0f,1.0f},{1.0f,0.0f},{0.0f,0.0f},{0.0f,1.0f}},	//TT_TIRIGHT	
	{{0.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f},{1.0f,0.0f}},	//TT_BILEFT	
	{{1.0f,0.0f},{1.0f,1.0f},{0.0f,1.0f},{0.0f,0.0f}},	//TT_BIRIGHT	
	
	{{0.0f,1.0f},{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f}},	//TT_CENTER
	{{0.0f,1.0f},{0.0f,0.0f},{1.0f,0.0f},{1.0f,1.0f}},	//TT_DEFAULT
};

void	CField::SetupNewUV(const short& x, const short& y)
{
	
/*	WORD	nidx	= 0;
	WORD	tmpidx	= 0;
	nidx	= (m_pDisplayMap[y][x].nTexIndex%10000)%14;
	tmpidx	= rand()%4;

	for ( unsigned char i = 0; i < 4; i ++ )
	{
		if ( nidx == 12 || nidx == 13 )
		{
			m_pDisplayMap[y][x].Pos[i].tu2	= TILEDUV[8+tmpidx][i][0];
			m_pDisplayMap[y][x].Pos[i].tv2	= TILEDUV[8+tmpidx][i][1];
		}
		else
		{
			m_pDisplayMap[y][x].Pos[i].tu2	= TILEDUV[nidx][i][0];
			m_pDisplayMap[y][x].Pos[i].tv2	= TILEDUV[nidx][i][1];
		}
	}
*/

}