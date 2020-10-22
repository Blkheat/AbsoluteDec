#include "RockPCH.h"

#include	"..\\quadlist.h"
#include	<BASE\\d3dapp.h>
#include	"Field.h"
#include	"..\\Camera.h"
#include	"Grass.h"
#include	"GrassNormal.h"
#include	"base\\d3dutil.h"
#include "..\\..\\CRenderManager.h"

extern	CGrassData	g_GrassList[1024];

void		CField::SetupTempGrass(DWORD idx)
{
	m_nCurGrass	= idx;
	m_GrassTemp.m_pData		= &g_GrassList[idx];
}

HRESULT		CField::DisplayTempGrass()
{
//	m_GrassTemp.Display();

	return	S_OK;
}

void		CField::CleanupTempGrass()
{
	// new
	m_GrassTemp.m_pData	= NULL;
	m_GrassTemp.m_pAni	= NULL;
	m_GrassTemp.m_bIsVisible	= false;
	m_nPickedGrass	= -1;
	m_nCurGrass		= -1;
}

void		CField::UpdateTempGrassPos()
{
	m_GrassTemp.m_matPos._41	= m_dwPickedX*(TILE_SIZE/2);
	m_GrassTemp.m_matPos._42	= GetAttrHeight(m_dwPickedX, m_dwPickedY);
	m_GrassTemp.m_matPos._43	= m_dwPickedY*(TILE_SIZE/2);
}

void		CField::DelTempGrass()
{
	m_GrassTemp.m_pData	= NULL;
	m_GrassTemp.m_pAni	= NULL;
	m_GrassTemp.m_bIsVisible	= false;
	m_nPickedGrass	= -1;
	m_nCurGrass		= -1;
}

bool		CField::AddGrass()
{
	if ( m_nCurGrass == -1 )
		return	false;

	short i = 0;
	for (  i = 0; i < MAX_GRASS; i ++ )
	{
		if ( m_GrassList[i].m_pData == NULL )
			break;
	}
	m_GrassList[i].m_pData	= &g_GrassList[m_nCurGrass];
	m_GrassList[i].m_matPos	= m_GrassTemp.m_matPos;
	m_GrassList[i].m_bIsVisible	= true;
	m_GrassList[i].m_nGrassIdx	= m_nCurGrass;
	m_dwGrassCount	++;
	return	true;
}

bool		CField::UpdateGrass()
{
	dwGrassVisibleCount		= 0;

	for ( short i = 0; i < m_dwGrassCount; i ++ )
	{
		if ( m_GrassList[i].m_pData )
		{
			if ( true == ( m_GrassList[i].m_bIsVisible = g_Camera.CubeInFrustum(m_GrassList[i].m_matPos._41, m_GrassList[i].m_matPos._42, m_GrassList[i].m_matPos._43, 50, 50, 50) ) )
				dwGrassVisibleCount	++;
		}
	}

	return	true;
}

void		CField::DelGrass()
{
	if ( m_nPickedGrass == -1 )
		return;

	m_GrassList[m_nPickedGrass].m_pData	= NULL;
	m_GrassList[m_nPickedGrass].m_pAni	= NULL;
	m_GrassList[m_nPickedGrass].m_bIsVisible	= false;
	m_dwGrassCount	--;
}

void		CField::GrassToTemp()
{
	if ( m_nPickedGrass == -1 )
		return;

	m_GrassTemp.m_pData	= NULL;
	m_GrassTemp.m_pAni	= NULL;
	m_GrassTemp.m_bIsVisible	= false;
	SetupTempGrass(m_GrassList[m_nPickedGrass].m_nGrassIdx);
	m_GrassTemp.m_matPos		= m_GrassList[m_nPickedGrass].m_matPos;
	DelGrass();
}

HRESULT		CField::DisplayGrass()
{
	if ( m_dwGrassCount <= 0 )
		return	E_FAIL;

	g_RenderManager.SetZWriteEnable( FALSE );

	g_RenderManager.LightEnable(0, TRUE);
	g_RenderManager.LightEnable(1, FALSE);
	g_RenderManager.SetMaterial(&m_MaterialGrass);
	
	g_RenderManager.SetLighting( TRUE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetCullMode( D3DCULL_NONE );

	g_RenderManager.SetAlphaTestEnable( TRUE );
	g_RenderManager.SetAlphaRef( 0x000000af );
	g_RenderManager.SetAlphaFunc( D3DCMP_GREATEREQUAL );

	g_RenderManager.SetAlphaRef( 0 );
	g_RenderManager.SetAlphaTestEnable( FALSE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetZWriteEnable( TRUE );

	return	S_OK;
}

void		CField::CleanupGrass()
{
	for ( short i = 0; i < MAX_GRASS; i ++ )
	{
		if ( m_GrassList[i].m_pData != NULL )
		{
			m_GrassList[i].m_pData	= NULL;
			m_GrassList[i].m_pAni	= NULL;
			m_GrassList[i].m_bIsVisible	= false;
		}
	}
	m_GrassTemp.m_pData		= NULL;
	m_GrassTemp.m_pAni		= NULL;
	m_GrassTemp.m_bIsVisible	= false;
	m_dwGrassCount	= 0;
	m_nCurGrass		= -1;
}

HRESULT		CField::PickGrass(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight)
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
	g_RenderManager.GetTransform(D3DTS_PROJECTION, &matProj);
	
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

	for ( i = 0; i < MAX_GRASS; i ++ )
	{
		if ( m_GrassList[i].m_pData )
		{
			D3DXVECTOR3	min, max;
			min.x	= m_GrassList[i].m_matPos._41;
			min.y	= m_GrassList[i].m_matPos._42;
			min.z	= m_GrassList[i].m_matPos._43;
			max.x	= min.x + 8.0f;
			max.y	= min.y + 8.0f;
			max.z	= min.z	+ 8.0f;

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
				m_nPickedGrass	= i;
				return S_OK;
			}
		}
	}
	m_nPickedGrass	= -1;
	return	E_FAIL;
}