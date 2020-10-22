#include "RockPCH.h"

#include	"..\\quadlist.h"
#include	"base\\d3dbase.h"
#include	"field.h"
#include	"obj\\ObjMath.h"
#include	"..\\camera.h"
#include	"gtexture.h"

void	CField::SetupWaterfallTemp()
{
	m_WaterfallTemp.FinalCleanup();
	m_WaterfallTemp.Build(m_pDevice, g_WaterfallTexture.GetTexture(m_byCurWaterfallTexNum), g_WaterfallTexture.GetFallTexture(m_byCurWaterfallTexNum),m_byCurWaterfallTexNum);
}

void	CField::DisplayWaterfallTemp()
{
	if ( m_WaterfallTemp.Enable() )
	{
		m_WaterfallTemp.FrameMove();
		m_WaterfallTemp.Render();
	}
//	if ( m_WaterfallTemp.GetDevice() == NULL )
//		return;

//	m_WaterfallTemp.Display();
}

void	CField::UpdateWaterfallPosTemp()
{
	D3DXMATRIX	matPos	= 	m_WaterfallTemp.GetPosTM();

	matPos._41	= (m_dwPickedX * 8.0f) - 4.0f;
	matPos._42	= GetAttrHeight(m_dwPickedX, m_dwPickedY);
	matPos._43	= (m_dwPickedY * 8.0f) - 4.0f;

	m_WaterfallTemp.SetPosTM(matPos);
//	m_WaterfallTemp.SetupTransTM(m_vPickedPt.x, m_vPickedPt.y, m_vPickedPt.z);
}


bool	CField::AddWaterfall()
{
	if ( !m_WaterfallTemp.Enable() )
		return	false;

	short i = 0;
	for ( i = 0; i < MAX_WATERFALL; i ++ )
	{
		if ( !m_WaterfallList[i].Enable() )
			break;
	}

	m_WaterfallList[i].Build(m_pDevice, g_WaterfallTexture.GetTexture(m_byCurWaterfallTexNum), g_WaterfallTexture.GetFallTexture(m_byCurWaterfallTexNum),m_byCurWaterfallTexNum);
	m_WaterfallList[i].SetPosTM(m_WaterfallTemp.GetPosTM());
	m_WaterfallList[i].SetVisible(true);
	m_WaterfallTemp.FinalCleanup();
	m_nWaterfallCount	++;
/*	if ( m_WaterfallTemp.GetDevice() == NULL )
		return	false;

	for ( short i = 0; i < MAX_WATERFALL; i ++ )
	{
		if ( m_Waterfall[i].GetDevice() == NULL )
			break;
	}
	
	m_Waterfall[i].InitDevice(m_pDevice);
	m_Waterfall[i].Build();
	m_Waterfall[i].SetupPosTM(m_WaterfallTemp.GetPosTM());
	
	m_WaterfallTemp.FinalCleanup();

	D3DXVECTOR3		min	= D3DXVECTOR3(-16.0f, 0.0f, -16.0f);	//= m_Object[i].m_bound.m_min_vertex;
	D3DXVECTOR3		max	= D3DXVECTOR3( 16.0f, 64.0f, 16.0f);	//= m_Object[i].m_bound.m_max_vertex;

	D3DXVec3TransformCoord(&min, &min, &m_Waterfall[i].GetPosTM());
	D3DXVec3TransformCoord(&max, &max, &m_Waterfall[i].GetPosTM());

	m_Waterfall[i].m_bIsVisible	= g_Camera.CubeInFrustum(min.x, min.y, min.z,max.x-min.x,max.y-min.y,max.z-min.z);

	m_nWaterfallCount	++;
	*/
	return	true;
}

void	CField::DisplayWaterfall()
{
	for ( int i = 0; i < MAX_WATERFALL; i ++ )
	{
		if ( m_WaterfallList[i].Enable() && m_WaterfallList[i].IsVisible() )
		{
			m_WaterfallList[i].FrameMove();
			m_WaterfallList[i].Render();
		}
	}
/*	for ( short i = 0; i < MAX_WATERFALL; i ++ )
	{
		if ( m_Waterfall[i].GetDevice() && m_Waterfall[i].m_bIsVisible )
		{
			m_Waterfall[i].Display();
		}
	}
	*/
}

void	CField::WaterfallToTemp()
{
	if ( m_nPickedWaterfall == -1 )
		return;

	m_WaterfallTemp.FinalCleanup();
	m_WaterfallTemp.Build(m_pDevice, g_WaterfallTexture.GetTexture(m_WaterfallList[m_nPickedWaterfall].GetTexIndex()), g_WaterfallTexture.GetFallTexture(m_WaterfallList[m_nPickedWaterfall].GetTexIndex()),m_WaterfallList[m_nPickedWaterfall].GetTexIndex());
	m_WaterfallTemp.SetPosTM(m_WaterfallList[m_nPickedWaterfall].GetPosTM());
	m_WaterfallList[m_nPickedWaterfall].FinalCleanup();
	m_nWaterfallCount	--;

/*	if ( m_nPickedWaterfall == -1 )
		return;

	m_WaterfallTemp.FinalCleanup();
	m_WaterfallTemp.InitDevice(m_pDevice);
	m_WaterfallTemp.Build();
	m_WaterfallTemp.SetupPosTM(m_Waterfall[m_nPickedWaterfall].GetPosTM());
	m_Waterfall[m_nPickedWaterfall].FinalCleanup();
	m_nWaterfallCount	--;
	*/
}


void	CField::DelWaterfallTemp()
{
	m_WaterfallTemp.FinalCleanup();
	m_nPickedWaterfall	= -1;
//	m_WaterfallTemp.FinalCleanup();
}

HRESULT		CField::PickWaterfall(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight)
{
	if ( m_pDisplayMap == NULL || m_nWaterfallCount == 0 )
		return E_FAIL;

	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(hWnd, &ptCursor);
	ptCursor.x	+= 2;
	ptCursor.y	+= 10;

	D3DXVECTOR3	vPickRayDir;
	D3DXVECTOR3	vPickRayOrig;

	D3DXMATRIX		matProj;
	g_RenderManager.GetTransform(D3DTS_PROJECTION, &matProj);
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
	g_RenderManager.GetTransform(D3DTS_VIEW, &matView);
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

	D3DXVECTOR3		min;
	D3DXVECTOR3		max;

	for ( i = 0; i < MAX_WATERFALL; i ++ )
	{
		if ( m_WaterfallList[i].Enable() )
		{
			min	= D3DXVECTOR3(-25.0f, 0.0f, -25.0f);	//= m_Object[i].m_bound.m_min_vertex;
			max	= D3DXVECTOR3( 25.0f, 100.0f, 25.0f);	//= m_Object[i].m_bound.m_max_vertex;
			D3DXVec3TransformCoord(&min, &min, &m_WaterfallList[i].GetPosTM());
			D3DXVec3TransformCoord(&max, &max, &m_WaterfallList[i].GetPosTM());

			v1[0].x	= min.x;
			v1[0].y	= min.y;
			v1[0].z	= min.z;
			v1[1].x	= min.x;
			v1[1].y	= min.y;
			v1[1].z	= max.z;
			v1[2].x	= max.x;
			v1[2].y	= min.y;
			v1[2].z	= min.z;

			v2[0].x	= max.x;
			v2[0].y	= min.y;
			v2[0].z	= max.z;
			v2[1].x	= max.x;
			v2[1].y	= min.y;
			v2[1].z	= min.z;
			v2[2].x	= min.x;
			v2[2].y	= min.y;
			v2[2].z	= max.z;

			if (IntersectTriangle(vPickRayOrig, vPickRayDir, v1[0], v1[1], v1[2], &fDist, &fBary1, &fBary2) 
			||	IntersectTriangle(vPickRayOrig, vPickRayDir, v2[0], v2[1], v2[2], &fDist, &fBary1, &fBary2) )
			{
				m_nPickedWaterfall	= i;
				return S_OK;
			}
		}
	}
	m_nPickedWaterfall	= -1;
	return	E_FAIL;
}

bool	CField::UpdateWaterfall()
{
	dwWaterfallVisibleCount	= 0;
	for ( short i = 0; i < MAX_WATERFALL; i ++ )
	{
		if ( m_WaterfallList[i].Enable() )
		{			
			D3DXVECTOR3		min	= D3DXVECTOR3(-25.0f, 0.0f, -25.0f);	//= m_Object[i].m_bound.m_min_vertex;
			D3DXVECTOR3		max	= D3DXVECTOR3( 25.0f, 100.0f, 25.0f);	//= m_Object[i].m_bound.m_max_vertex;
			D3DXVec3TransformCoord(&min, &min, &m_WaterfallList[i].GetPosTM());
			D3DXVec3TransformCoord(&max, &max, &m_WaterfallList[i].GetPosTM());
			m_WaterfallList[i].SetVisible(g_Camera.CubeInFrustum(min.x, min.y, min.z,max.x-min.x,max.y-min.y,max.z-min.z));
			if ( m_WaterfallList[i].IsVisible() )
				dwWaterfallVisibleCount ++;
		}
	}
/*	dwWaterfallVisibleCount	= 0;

	for ( short i = 0; i < MAX_WATERFALL; i ++ )
	{
		if ( m_Waterfall[i].GetDevice() )
		{
			D3DXVECTOR3		min	= D3DXVECTOR3(-16.0f, 0.0f, -16.0f);	//= m_Object[i].m_bound.m_min_vertex;
			D3DXVECTOR3		max	= D3DXVECTOR3( 16.0f, 64.0f, 16.0f);	//= m_Object[i].m_bound.m_max_vertex;

			D3DXVec3TransformCoord(&min, &min, &m_Waterfall[i].GetPosTM());
			D3DXVec3TransformCoord(&max, &max, &m_Waterfall[i].GetPosTM());

			m_Waterfall[i].m_bIsVisible	= g_Camera.CubeInFrustum(min.x, min.y, min.z,max.x-min.x,max.y-min.y,max.z-min.z);
			if ( m_Waterfall[i].m_bIsVisible )
				dwWaterfallVisibleCount	++;
		}
	}
	return	true;
	*/
	return	true;
}

void	CField::CleanupWaterfall()
{
	if ( m_nWaterfallCount )
	{
		for ( int i = 0; i < MAX_WATERFALL; i ++ )
		{
			if ( m_WaterfallList[i].Enable() )
				m_WaterfallList[i].FinalCleanup();
		}
	}
	m_WaterfallTemp.FinalCleanup();
	m_nWaterfallCount	= 0;
	m_nPickedWaterfall	= -1;
}

