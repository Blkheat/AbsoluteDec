#include "RockPCH.h"

#include	"..\\quadlist.h"
#include	"base\\d3dapp.h"
#include	"Field.h"


#define		MULTITX_WIDTH		8
#define		MULTITX_HEIGHT		6
#define		MULTITX_DIR			12
#define		MULTITX_MAXIDX		24
#define		MULTITX_DEFAULT		18
#define		MULTITX_CENTER		5

static const unsigned int	MultiTx[MULTITX_DIR][2]	=
{
	/* LEFT */		{4,16},
	/* TOP */		{1,2},
	/* RIGHT */		{7,19},
	/* BOTTOM */	{21,22},
	/* T-LEFT */	{0,0},
	/* T-RIGHT */	{3,3},
	/* B-LEFT */	{20,20},
	/* B-RIGHT */	{23,23},
	/* TI-LEFT */	{8,14},
	/* TI-RIGHT */	{11,13},
	/* BI-LEFT */	{12,10},
	/* BI-RIGHT */	{15,9}
};

/*
void		CField::SetupDefaultMultiTexIndex()
{
	const float		xOffset	= 1.0f / MULTITX_WIDTH;
	const float		yOffset	= 1.0f / MULTITX_HEIGHT;

	float			u, v;

	for ( DWORD y = 0; y < m_cSize; y ++ )
	{
		for ( DWORD x = 0; x < m_cSize; x ++ )
		{
//			m_pDisplayMap[y][x].dwMultiIdx	= MULTITX_DEFAULT;
//			u	= m_pDisplayMap[y][x].dwMultiIdx % 4;
//			v	= m_pDisplayMap[y][x].dwMultiIdx / 4;

			m_pDisplayMap[y][x].Pos[0].tu2	= (float)u*xOffset;
			m_pDisplayMap[y][x].Pos[0].tv2	= (float)(v+1)*yOffset;
			m_pDisplayMap[y][x].Pos[1].tu2	= (float)u*xOffset;
			m_pDisplayMap[y][x].Pos[1].tv2	= (float)v*yOffset;
			m_pDisplayMap[y][x].Pos[2].tu2	= (float)(u+1)*xOffset;
			m_pDisplayMap[y][x].Pos[2].tv2	= (float)v*yOffset;
			m_pDisplayMap[y][x].Pos[3].tu2	= (float)(u+1)*xOffset;
			m_pDisplayMap[y][x].Pos[3].tv2	= (float)(v+1)*yOffset;
		}
	}
}

void	CField::SetupMultiUV(DWORD x, DWORD y, DWORD idx)
{
	const float		xOffset	= 1.0f / MULTITX_WIDTH;
	const float		yOffset	= 1.0f / MULTITX_HEIGHT;
	const float		Offset	= xOffset * (idx*4);
	float			u, v;

//	u	= (m_pDisplayMap[y][x].dwMultiIdx - (MULTITX_MAXIDX*idx))%4;
//	v	= (m_pDisplayMap[y][x].dwMultiIdx - (MULTITX_MAXIDX*idx))/4;

	m_pDisplayMap[y][x].Pos[0].tu2	= (float)u*xOffset + Offset;
	m_pDisplayMap[y][x].Pos[0].tv2	= (float)(v+1)*yOffset;
	m_pDisplayMap[y][x].Pos[1].tu2	= (float)u*xOffset + Offset;
	m_pDisplayMap[y][x].Pos[1].tv2	= (float)v*yOffset;
	m_pDisplayMap[y][x].Pos[2].tu2	= (float)(u+1)*xOffset + Offset;
	m_pDisplayMap[y][x].Pos[2].tv2	= (float)v*yOffset;
	m_pDisplayMap[y][x].Pos[3].tu2	= (float)(u+1)*xOffset + Offset;
	m_pDisplayMap[y][x].Pos[3].tv2	= (float)(v+1)*yOffset;
}

void	CField::SetupMultiUV(DWORD x, DWORD y)
{
	const float		xOffset	= 1.0f / MULTITX_WIDTH;
	const float		yOffset	= 1.0f / MULTITX_HEIGHT;
//	DWORD			idx		= m_pDisplayMap[y][x].dwMultiIdx / MULTITX_MAXIDX;
	const float		Offset	= xOffset * ( idx*4 );

	float			u, v;

//	u	= (m_pDisplayMap[y][x].dwMultiIdx - (MULTITX_MAXIDX*idx))%4;
//	v	= (m_pDisplayMap[y][x].dwMultiIdx - (MULTITX_MAXIDX*idx))/4;

	m_pDisplayMap[y][x].Pos[0].tu2	= (float)u*xOffset + Offset;
	m_pDisplayMap[y][x].Pos[0].tv2	= (float)(v+1)*yOffset;
	m_pDisplayMap[y][x].Pos[1].tu2	= (float)u*xOffset + Offset;
	m_pDisplayMap[y][x].Pos[1].tv2	= (float)v*yOffset;
	m_pDisplayMap[y][x].Pos[2].tu2	= (float)(u+1)*xOffset + Offset;
	m_pDisplayMap[y][x].Pos[2].tv2	= (float)v*yOffset;
	m_pDisplayMap[y][x].Pos[3].tu2	= (float)(u+1)*xOffset + Offset;
	m_pDisplayMap[y][x].Pos[3].tv2	= (float)(v+1)*yOffset;
}


void	CField::SetupMultiIdx(DWORD type, DWORD idx, DWORD x, DWORD y)
{
	switch ( type )
	{
	case	TIDX_TOPLEFT:
//		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_TOPLEFT][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_TOPRIGHT:
//		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_TOPRIGHT][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_BOTLEFT:
		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_BOTLEFT][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_BOTRIGHT:
		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_BOTRIGHT][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_INV_TOPLEFT:
		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_INV_TOPLEFT][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_INV_TOPRIGHT:
		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_INV_TOPRIGHT][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_INV_BOTLEFT:
		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_INV_BOTLEFT][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_INV_BOTRIGHT:
		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_INV_BOTRIGHT][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_TOP:
		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_TOP][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_LEFT:
		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_LEFT][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_RIGHT:
		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_RIGHT][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	case	TIDX_BOTTOM:
		m_pDisplayMap[y][x].dwMultiIdx	= MultiTx[TIDX_BOTTOM][rand()%2]+(idx*MULTITX_MAXIDX);
		break;
	}
	SetupMultiUV(x,y,idx);
}

DWORD	CField::GetMultiIdxType(DWORD idx, DWORD x, DWORD y)
{
	DWORD	Top, Left, Right, Bottom;
	DWORD	TopLeft, TopRight, BotLeft, BotRight;
	DWORD	Center;

	Top			= CheckMultiTop(idx, x, y);
	Left		= CheckMultiLeft(idx, x, y);
	Right		= CheckMultiRight(idx, x, y);
	Bottom		= CheckMultiBottom(idx, x, y);

	TopLeft		= CheckMultiTopLeft(idx, x, y);
	TopRight	= CheckMultiTopRight(idx, x, y);
	BotLeft		= CheckMultiBotLeft(idx, x, y);
	BotRight	= CheckMultiBotRight(idx, x, y);

	Center		= CheckMultiCenter(idx, x, y);

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


DWORD	CField::CheckMultiLeft(DWORD idx, DWORD x, DWORD y)
{
	if ( m_pDisplayMap[y][x-1].dwMultiIdx == MULTITX_DEFAULT )
		return	TIDX_EMPTY;

	if ( m_pDisplayMap[y][x-1].dwMultiIdx == MULTITX_CENTER+(idx*MULTITX_MAXIDX) )
		return	TIDX_CENTER;

	for ( BYTE i = 0; i < MULTITX_DIR; i ++ )
	{
		for ( BYTE j = 0; j < 2; j ++ )
		if ( m_pDisplayMap[y][x-1].dwMultiIdx == MultiTx[i][j]+(idx*MULTITX_MAXIDX) )
			return i;
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckMultiTop(DWORD idx, DWORD x, DWORD y)
{
	if ( m_pDisplayMap[y+1][x].dwMultiIdx == MULTITX_DEFAULT )
		return TIDX_EMPTY;

	if ( m_pDisplayMap[y+1][x].dwMultiIdx == MULTITX_CENTER+(idx*MULTITX_MAXIDX) )
		return TIDX_CENTER;
	
	for ( BYTE i = 0; i < MULTITX_DIR; i ++ )
	{
		for ( BYTE j = 0; j < 2; j ++ )
		if ( m_pDisplayMap[y+1][x].dwMultiIdx == MultiTx[i][j]+(idx*MULTITX_MAXIDX) )
			return i;
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckMultiRight(DWORD idx, DWORD x, DWORD y)
{
	if ( m_pDisplayMap[y][x+1].dwMultiIdx == MULTITX_DEFAULT )
		return TIDX_EMPTY;

	if ( m_pDisplayMap[y][x+1].dwMultiIdx == MULTITX_CENTER+(idx*MULTITX_MAXIDX) )
		return TIDX_CENTER;
	
	for ( BYTE i = 0; i < MULTITX_DIR; i ++ )
	{
		for ( BYTE j = 0; j < 2; j ++ )
		if ( m_pDisplayMap[y][x+1].dwMultiIdx == MultiTx[i][j]+(idx*MULTITX_MAXIDX) )
			return i;
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckMultiBottom(DWORD idx, DWORD x, DWORD y)
{
	if ( m_pDisplayMap[y-1][x].dwMultiIdx == MULTITX_DEFAULT )
		return TIDX_EMPTY;

	if ( m_pDisplayMap[y-1][x].dwMultiIdx == MULTITX_CENTER+(idx*MULTITX_MAXIDX) )
		return TIDX_CENTER;
	
	for ( BYTE i = 0; i < MULTITX_DIR; i ++ )
	{
		for ( BYTE j = 0; j < 2; j ++ )
		if ( m_pDisplayMap[y-1][x].dwMultiIdx == MultiTx[i][j]+(idx*MULTITX_MAXIDX) )
			return i;
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckMultiTopLeft(DWORD idx, DWORD x, DWORD y)
{
	if ( m_pDisplayMap[y+1][x-1].dwMultiIdx == MULTITX_DEFAULT )
		return TIDX_EMPTY;

	if ( m_pDisplayMap[y+1][x-1].dwMultiIdx == MULTITX_CENTER+(idx*MULTITX_MAXIDX) )
		return TIDX_CENTER;
	
	for ( BYTE i = 0; i < MULTITX_DIR; i ++ )
	{
		for ( BYTE j = 0; j < 2; j ++ )
		if ( m_pDisplayMap[y+1][x-1].dwMultiIdx == MultiTx[i][j]+(idx*MULTITX_MAXIDX) )
			return i;
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckMultiTopRight(DWORD idx, DWORD x, DWORD y )
{
	if ( m_pDisplayMap[y+1][x+1].dwMultiIdx == MULTITX_DEFAULT )
		return TIDX_EMPTY;

	if ( m_pDisplayMap[y+1][x+1].dwMultiIdx == MULTITX_CENTER+(idx*MULTITX_MAXIDX) )
		return TIDX_CENTER;
	
	for ( BYTE i = 0; i < MULTITX_DIR; i ++ )
	{
		for ( BYTE j = 0; j < 2; j ++ )
		if ( m_pDisplayMap[y+1][x+1].dwMultiIdx == MultiTx[i][j]+(idx*MULTITX_MAXIDX) )
			return i;
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckMultiBotLeft(DWORD idx, DWORD x, DWORD y)
{
	if ( m_pDisplayMap[y-1][x-1].dwMultiIdx == MULTITX_DEFAULT )
		return TIDX_EMPTY;

	if ( m_pDisplayMap[y-1][x-1].dwMultiIdx == MULTITX_CENTER+(idx*MULTITX_MAXIDX) )
		return TIDX_CENTER;
	
	for ( BYTE i = 0; i < MULTITX_DIR; i ++ )
	{
		for ( BYTE j = 0; j < 2; j ++ )
		if ( m_pDisplayMap[y-1][x-1].dwMultiIdx == MultiTx[i][j]+(idx*MULTITX_MAXIDX) )
			return i;
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckMultiBotRight(DWORD idx, DWORD x, DWORD y)
{
	if ( m_pDisplayMap[y-1][x+1].dwMultiIdx == MULTITX_DEFAULT )
		return TIDX_EMPTY;

	if ( m_pDisplayMap[y-1][x+1].dwMultiIdx == MULTITX_CENTER+(idx*MULTITX_MAXIDX) )
		return TIDX_CENTER;
	
	for ( BYTE i = 0; i < MULTITX_DIR; i ++ )
	{
		for ( BYTE j = 0; j < 2; j ++ )
		if ( m_pDisplayMap[y-1][x+1].dwMultiIdx == MultiTx[i][j]+(idx*MULTITX_MAXIDX) )
			return i;
	}

	return TIDX_EMPTY;
}

DWORD	CField::CheckMultiCenter(DWORD idx, DWORD x, DWORD y)
{
	if ( m_pDisplayMap[y][x].dwMultiIdx == MULTITX_DEFAULT )
		return TIDX_EMPTY;

	if ( m_pDisplayMap[y][x].dwMultiIdx == MULTITX_CENTER+(idx*MULTITX_MAXIDX) )
		return TIDX_CENTER;
	
	for ( BYTE i = 0; i < MULTITX_DIR; i ++ )
	{
		for ( BYTE j = 0; j < 2; j ++ )
		if ( m_pDisplayMap[y][x].dwMultiIdx == MultiTx[i][j]+(idx*MULTITX_MAXIDX) )
			return i;
	}

	return TIDX_EMPTY;
}

DWORD	MultiDir[8][2]	= {
	{-1,0},
	{0,1},
	{1,0},
	{0,-1},
	{-1,1},
	{1,1},
	{-1,-1},
	{1,-1}
};


void	CField::SetupMultiLibIdx()
{
	DWORD	x	= m_dwPickedX;
	DWORD	y	= m_dwPickedY;
	DWORD	ele	= m_dwSelectedMultiEle;

	if ( x <= 1 || y <= 1 || x >= m_cSize-2 || y >= m_cSize-2 )
		return;

	DWORD	srcEle = 0, srcType = 0;

	m_pDisplayMap[y][x].dwMultiIdx	= MULTITX_CENTER + (ele*MULTITX_MAXIDX);
	SetupMultiUV(x,y,ele);

	for ( DWORD i = 0; i < 8; i ++ )
	{
		x	= m_dwPickedX + MultiDir[7-i][0];
		y	= m_dwPickedY + MultiDir[7-i][1];

		SetupMultiIdx(GetMultiIdxType(ele,x,y),ele,x,y);
	}

}

void	CField::SetupMultiIdxWithoutLib()
{
	if ( m_pDisplayMap )
	{
		m_pDisplayMap[m_dwPickedY][m_dwPickedX].dwMultiIdx	= m_dwPickedMultiIdx;
		SetupMultiUV(m_dwPickedX, m_dwPickedY, m_dwSelectedMultiEle);
	}
}
*/