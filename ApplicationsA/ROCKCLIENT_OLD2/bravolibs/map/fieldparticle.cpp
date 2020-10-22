#include "RockPCH.h"

#include	"..\\quadlist.h"
#include	"base\\d3dapp.h"
#include	"obj\\objmath.h"
#include	"field.h"
#include	"..\\camera.h"



bool	CField::CreateParticle(short idx)
{
	return	true;
}

HRESULT	CField::DisplayParticle()
{
	/*
	for ( short i = 0; i < MAX_PARTICLE; i ++ )
	{
		if ( m_Particle[i].m_bIsReady && m_Particle[i].m_bIsVisible )
		{
			m_Particle[i].Update(0.02f);
			m_Particle[i].Render();
		}
	}
	*/
	return	S_OK;
}

bool	CField::CreateParticleTemp(short idx)
{
	m_ParticleTemp.InvalidateDeviceObjects();
//	m_ParticleTemp.Create("", 0);
	m_ParticleTemp.m_bIsSuppressed	= false;
	return	true;
}

HRESULT	CField::DisplayParticleTemp()
{
	if ( !m_ParticleTemp.m_bIsReady )
		return	E_FAIL;
	m_ParticleTemp.Update(0.02f);
	m_ParticleTemp.Render();
	return	S_OK;
}

void	CField::UpdateParticlePosTemp()
{
	m_ParticleTemp.m_vMoveLocation	= m_vPickedPt;
}

bool	CField::AddParticle()
{
/*
	if ( !m_ParticleTemp.m_bIsReady )
		return	false;

	for ( short i = 0; i < MAX_PARTICLE; i ++ )
	{
		if ( !m_Particle[i].m_bIsReady )
			break;
	}
	m_Particle[i].InvalidateDeviceObjects();
//	m_Particle[i].Create("", m_ParticleTemp.m_nIdx);
	m_Particle[i].m_bIsSuppressed	= false;
	m_Particle[i].m_vMoveLocation	= m_ParticleTemp.m_vMoveLocation;
	m_Particle[i].m_fRotTheta	= m_ParticleTemp.m_fRotTheta;

	m_ParticleTemp.InvalidateDeviceObjects();

	m_nParticleCount	++;

	D3DXVECTOR3		min	= D3DXVECTOR3(-8.0f, 0.0f, -8.0f);
	D3DXVECTOR3		max	= D3DXVECTOR3( 8.0f, 0.0f, 8.0f);

	D3DXMATRIX	m;
	D3DXMatrixTranslation(&m, m_Particle[i].m_vMoveLocation.x, m_Particle[i].m_vMoveLocation.y, m_Particle[i].m_vMoveLocation.z);
	D3DXVec3TransformCoord(&min, &min, &m);
	D3DXVec3TransformCoord(&max, &max, &m);

	m_Particle[i].m_bIsVisible	= g_Camera.CubeInFrustum(min.x,min.y,min.z,16.0f, 16.0f, 16.0f);
*/
	return	true;
}

void	CField::ParticleToTemp()
{
/*	
	if ( m_nPickedParticle == -1 )
		return;

	m_ParticleTemp.InvalidateDeviceObjects();
//	m_ParticleTemp.Create(NULL, m_Particle[m_nPickedParticle].m_nIdx);
	m_ParticleTemp.m_bIsSuppressed	= false;
	m_ParticleTemp.m_vMoveLocation	= m_Particle[m_nPickedParticle].m_vMoveLocation;
	m_ParticleTemp.m_fRotTheta	= m_Particle[m_nPickedParticle].m_fRotTheta;

	m_Particle[m_nPickedParticle].InvalidateDeviceObjects();

	m_nParticleCount	--;
*/	
}

void	CField::DelParticleTemp()
{
	m_ParticleTemp.InvalidateDeviceObjects();
}

HRESULT		CField::PickParticle(HWND hWnd, DWORD dwScreenWidth, DWORD dwScreenHeight)
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
/*
	FLOAT	fBary1, fBary2;
	FLOAT	fDist;

	// check range == tile size ( for easy picking )
	D3DXVECTOR3		v1[3];	// 1st face
	D3DXVECTOR3		v2[3];	// 2nd face

	DWORD			i = 0;

	for ( i = 0; i < MAX_PARTICLE; i ++ )
	{
		if ( m_Particle[i].m_bIsReady )
		{
			D3DXVECTOR3		min	= D3DXVECTOR3(-8.0f, 0.0f, -8.0f);	//= m_Object[i].m_bound.m_min_vertex;
			D3DXVECTOR3		max	= D3DXVECTOR3( 8.0f, 0.0f, 8.0f);	//= m_Object[i].m_bound.m_max_vertex;

			D3DXMATRIX	m;
			D3DXMatrixTranslation(&m, m_Particle[i].m_vMoveLocation.x, m_Particle[i].m_vMoveLocation.y, m_Particle[i].m_vMoveLocation.z);
			D3DXVec3TransformCoord(&min, &min, &m);
			D3DXVec3TransformCoord(&max, &max, &m);

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
				m_nPickedParticle	= i;
				return S_OK;
			}
		}
	}
	
	m_nPickedParticle	= -1;
*/
	return	E_FAIL;
}

bool	CField::UpdateParticle()
{
	dwParticleVisibleCount	= 0;
/*
	for ( short i = 0; i < MAX_PARTICLE; i ++ )
	{
		if ( m_Particle[i].m_bIsReady )
		{
			D3DXVECTOR3		min	= D3DXVECTOR3(-8.0f, 0.0f, -8.0f);
			D3DXVECTOR3		max	= D3DXVECTOR3( 8.0f, 0.0f, 8.0f);

			D3DXMATRIX	m;
			D3DXMatrixTranslation(&m, m_Particle[i].m_vPotision.x, m_Particle[i].m_vPotision.y, m_Particle[i].m_vPotision.z);
			D3DXVec3TransformCoord(&min, &min, &m);
			D3DXVec3TransformCoord(&max, &max, &m);

			m_Particle[i].m_bIsVisible	= g_Camera.CubeInFrustum(min.x,min.y,min.z,16.0f, 16.0f, 16.0f);
			if ( m_Particle[i].m_bIsVisible == true )
				dwParticleVisibleCount	++;
		}
	}
	*/
	return	true;
}

void	CField::CleanupParticle()
{
	/*
	for ( short i = 0; i < MAX_PARTICLE; i ++ )
	{
		if ( m_Particle[i].m_bIsReady )
			m_Particle[i].InvalidateDeviceObjects();
	}
	m_ParticleTemp.InvalidateDeviceObjects();
	m_nParticleCount	= 0;
	m_nPickedParticle	= -1;
	*/
}