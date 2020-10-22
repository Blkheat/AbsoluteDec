#ifndef __CARROWLINELIST_H__
#define __CARROWLINELIST_H__
//-----------------------------------------------------------------------------

#include "Wnd.h"

struct ARROWVERTEX
{
    D3DXVECTOR3 vPos;
	DWORD		diffuse;    

	ARROWVERTEX()
	{
		vPos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		diffuse = D3DCOLOR_ARGB( 255, 255, 0, 0 );
	}
};

#define MAX_POINT 30

#define D3DFVF_ARROWVERTEX ( D3DFVF_XYZ|D3DFVF_DIFFUSE )

class CArrowLineList : public CWnd
{
protected:
	int				m_nPointSize;
	int				m_nPointIndex;

	POINT			m_PointList[MAX_POINT];
	D3DXVECTOR3		m_VertexList[MAX_POINT];

	int				m_nPolyIndex;
	int				m_nPolyCount;	
	ARROWVERTEX		m_LinePolygon[MAX_POINT * 6];
	ARROWVERTEX		m_ArrowPolygon[3];

public:	
	CArrowLineList() {}
	CArrowLineList( SDesktop* DT );	 
	virtual ~CArrowLineList();
	virtual void RenderProc();
	
	void  Initialize();	
	void  AddPoint(POINT nPoint);
	void  ConVertPointTo3DVetex();
	void  MakePolygonList();
};

#endif