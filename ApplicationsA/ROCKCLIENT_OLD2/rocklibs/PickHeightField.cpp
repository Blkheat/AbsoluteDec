// PickHeightField.cpp: implementation of the CPickHeightMap class.

#include "RockPCH.h"
#include "PickHeightField.h"

CPickHeightField::CPickHeightField()
{

}

CPickHeightField::~CPickHeightField()
{

}

void	CPickHeightField::PHF_Reset()
{
	thePickRayDir.x = thePickRayDir.y = thePickRayDir.z = 0.0f;
	thePickRayOrig.x = thePickRayOrig.y = thePickRayOrig.z = 0.0f;

	thePickX = -1.0f;
	thePickZ = -1.0f;
}

void	CPickHeightField::PHF_SetPickedXZ( float aX, float aZ )
{
	thePickX = aX;
	thePickZ = aZ;
}

bool	CPickHeightField::PHF_IsFailed()
{
	if( thePickX == -1 || thePickZ == -1 ) return true;

	return false;
}

bool	CPickHeightField::PHF_Pick( HWND aWND, D3DXMATRIX* aMatProj, D3DXMATRIX* aMatView, DWORD aWidth, DWORD aHeight,
									float aCenterX, float aCenterZ, float* aPickX, float* aPickZ, float aDistance )
{
	PHF_Reset();

	POINT ptCursor;

    // Get the pick ray from the mouse position
//	if( GetCapture() ) /// 잡히지가 않네... 막아도 상관 없습니다. 
	{
		GetCursorPos( &ptCursor );
		ScreenToClient( aWND, &ptCursor );
	}

    // Compute the vector of the pick ray in screen space
    D3DXVECTOR3 v;
    v.x =  ( ( ( 2.0f * ptCursor.x ) / aWidth  ) - 1 ) / aMatProj->_11;
    v.y = -( ( ( 2.0f * ptCursor.y ) / aHeight ) - 1 ) / aMatProj->_22;
    v.z =  1.0f;

    // Get the inverse view matrix
    D3DXMATRIX m;
    D3DXMatrixInverse( &m, NULL, aMatView );

    // Transform the screen space pick ray into 3D space
    thePickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    thePickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    thePickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    thePickRayOrig.x = m._41;
    thePickRayOrig.y = m._42;
    thePickRayOrig.z = m._43;
 
/*
	thePickMethod = n_LogicalVertex;
	/// logical vertex compare picking : recursive function 
	PHF_RecursivePick( aCenterX, aCenterZ, aDistance );

	if( PHF_IsFailed() )
	{
		/// 최초, 가장 넓은 범위에서도 잡히지 않았다. : 너무 멀리 찍은 경우 
		if( theLastDistance == aDistance )
		{
			*aPickX = 0.0f;
			*aPickZ = 0.0f;
			return false;
		}

		thePickMethod = n_TerrainVertex;
		/// RecursivePick에서 좁혀놓은 범위 안에서 Terrain Vertex 비교 
		PHF_VertexComparePick( theLastCenterX, theLastCenterZ, theLastDistance );
		
		if( PHF_IsFailed() )
		{
			thePickMethod = n_TerrainVertexWide;
			/// 이도저도 안되면 무식하게 범위내 Terrain Vertex 모두 비교 
			PHF_VertexComparePick( aCenterX, aCenterZ, aDistance );
			
			if( PHF_IsFailed() )
			{
				*aPickX = 0.0f;
				*aPickZ = 0.0f;
				return false;
			}
		}
	}
*/
	/// 정확도를 높이기 위해... 버텍스 비교만 작동시킵니다. - 추후 logical vertex compare picking을 optimize 계획 
	thePickMethod = n_TerrainVertexWide;
	/// 이도저도 안되면 무식하게 범위내 Terrain Vertex 모두 비교 
	PHF_VertexComparePick( aCenterX, aCenterZ, aDistance );
	
	if( PHF_IsFailed() )
	{
		*aPickX = 0.0f;
		*aPickZ = 0.0f;
		return false;
	}

	*aPickX = thePickX;
	*aPickZ = thePickZ;

	return true;
}

void	CPickHeightField::PHF_RecursivePick( float aCenterX, float aCenterZ, float aDistance )
{
//	z
//	|[0]	[1]		[2]
//	+-------+-------+
//	|	1	|	2	|
//	|		|		|
//	|[3]	|[4]	|[5]
//	+-------+-------+
//	|	3	|	4	|
//	|		|		|
//	|[6]	|[7]	|[8]
//	+-------+-------+-- x

	// section 4 
	// vertex 9

	D3DXVECTOR3 v[9];

	v[0].x = aCenterX - aDistance;
	v[0].z = aCenterZ + aDistance;

	v[1].x = aCenterX;
	v[1].z = aCenterZ + aDistance;

	v[2].x = aCenterX + aDistance;
	v[2].z = aCenterZ + aDistance;

	v[3].x = aCenterX - aDistance;
	v[3].z = aCenterZ;

	v[4].x = aCenterX;
	v[4].z = aCenterZ;

	v[5].x = aCenterX + aDistance;
	v[5].z = aCenterZ;

	v[6].x = aCenterX - aDistance;
	v[6].z = aCenterZ - aDistance;

	v[7].x = aCenterX;
	v[7].z = aCenterZ - aDistance;

	v[8].x = aCenterX + aDistance;
	v[8].z = aCenterZ - aDistance;

	for( int i = 0; i < 9; ++i )
	{
		v[i].y = PHF_GetHeight( v[i].x, v[i].z );
	}

	theLastCenterX = aCenterX;
	theLastCenterZ = aCenterZ;
	theLastDistance = aDistance;

	float halfDist = aDistance * 0.5f + 0.5f;
	float fBary1, fBary2;
	float fDist;

	bool ret0, ret1;
	float pointX, pointZ;


	// 1
	if( PHF_IsFailed() == false ) return;	/// 이미 성공. 재귀 함수 탈출 
	if( ( ret0 = PHF_IntersectTriangle( v[0], v[1], v[3], &fDist, &fBary1, &fBary2 ) ) == true || 
		( ret1 = PHF_IntersectTriangle( v[1], v[4], v[3], &fDist, &fBary1, &fBary2 ) ) == true )
	{
		if( aDistance <= Pick_Scale_Min )
		{
			if( ret0 )
			{
				pointX = v[0].x + fBary1 * ( v[1].x - v[0].x ) + fBary2 * ( v[3].x - v[0].x );
				pointZ = v[0].z + fBary1 * ( v[1].z - v[0].z ) + fBary2 * ( v[3].z - v[0].z );
				if( PHF_IsValidPoint( pointX, pointZ ) )
				{
					PHF_SetPickedXZ( pointX, pointZ );
				}
			}
			else
			{
				pointX = v[1].x + fBary1 * ( v[4].x - v[1].x ) + fBary2 * ( v[3].x - v[1].x );
				pointZ = v[1].z + fBary1 * ( v[4].z - v[1].z ) + fBary2 * ( v[3].z - v[1].z );
				if( PHF_IsValidPoint( pointX, pointZ ) )
				{
				PHF_SetPickedXZ( pointX, pointZ );
				}
			}
			return;
		}

		PHF_RecursivePick( ( aCenterX - halfDist ), ( aCenterZ + halfDist ), halfDist );
	}

	// 2
	if( PHF_IsFailed() == false ) return;	/// 이미 성공. 재귀 함수 탈출 
	if( ( ret0 = PHF_IntersectTriangle( v[1], v[2], v[4], &fDist, &fBary1, &fBary2 ) ) == true || 
		( ret1 = PHF_IntersectTriangle( v[2], v[5], v[4], &fDist, &fBary1, &fBary2 ) ) == true )
	{
		if( aDistance <= Pick_Scale_Min )
		{
			if( ret0 )
			{
				pointX = v[1].x + fBary1 * ( v[2].x - v[1].x ) + fBary2 * ( v[4].x - v[1].x );
				pointZ = v[1].z + fBary1 * ( v[2].z - v[1].z ) + fBary2 * ( v[4].z - v[1].z );
				if( PHF_IsValidPoint( pointX, pointZ ) )
				{
					PHF_SetPickedXZ( pointX, pointZ );
				}
			}
			else
			{
				pointX = v[2].x + fBary1 * ( v[5].x - v[2].x ) + fBary2 * ( v[4].x - v[2].x );
				pointZ = v[2].z + fBary1 * ( v[5].z - v[2].z ) + fBary2 * ( v[4].z - v[2].z );
				if( PHF_IsValidPoint( pointX, pointZ ) )
				{
					PHF_SetPickedXZ( pointX, pointZ );
				}
			}
			return;
		}

		PHF_RecursivePick( ( aCenterX + halfDist ), ( aCenterZ + halfDist ), halfDist );
	}

	// 3
	if( PHF_IsFailed() == false ) return;	/// 이미 성공. 재귀 함수 탈출 
	if( ( ret0 = PHF_IntersectTriangle( v[3], v[4], v[6], &fDist, &fBary1, &fBary2 ) ) == true || 
		( ret1 = PHF_IntersectTriangle( v[4], v[7], v[6], &fDist, &fBary1, &fBary2 ) ) == true )
	{
		if( aDistance <= Pick_Scale_Min )
		{
			if( ret0 )
			{
				pointX = v[3].x + fBary1 * ( v[4].x - v[3].x ) + fBary2 * ( v[6].x - v[3].x );
				pointZ = v[3].z + fBary1 * ( v[4].z - v[3].z ) + fBary2 * ( v[6].z - v[3].z );
				if( PHF_IsValidPoint( pointX, pointZ ) )
				{
					PHF_SetPickedXZ( pointX, pointZ );
				}
			}
			else
			{
				pointX = v[4].x + fBary1 * ( v[7].x - v[4].x ) + fBary2 * ( v[6].x - v[4].x );
				pointZ = v[4].z + fBary1 * ( v[7].z - v[4].z ) + fBary2 * ( v[6].z - v[4].z );
				if( PHF_IsValidPoint( pointX, pointZ ) )
				{
					PHF_SetPickedXZ( pointX, pointZ );
				}
			}
			return;
		}

		PHF_RecursivePick( ( aCenterX - halfDist ), ( aCenterZ - halfDist ), halfDist );
	}
	
	// 4
	if( PHF_IsFailed() == false ) return;	/// 이미 성공. 재귀 함수 탈출 
	if( ( ret0 = PHF_IntersectTriangle( v[4], v[5], v[7], &fDist, &fBary1, &fBary2 ) ) == true || 
		( ret1 = PHF_IntersectTriangle( v[5], v[8], v[7], &fDist, &fBary1, &fBary2 ) ) == true )
	{
		if( aDistance <= Pick_Scale_Min )
		{
			if( ret0 )
			{
				pointX = v[4].x + fBary1 * ( v[5].x - v[4].x ) + fBary2 * ( v[7].x - v[4].x );
				pointZ = v[4].z + fBary1 * ( v[5].z - v[4].z ) + fBary2 * ( v[7].z - v[4].z );
				if( PHF_IsValidPoint( pointX, pointZ ) )
				{
					PHF_SetPickedXZ( pointX, pointZ );
				}
			}
			else
			{
				pointX = v[5].x + fBary1 * ( v[8].x - v[5].x ) + fBary2 * ( v[7].x - v[5].x );
				pointZ = v[5].z + fBary1 * ( v[8].z - v[5].z ) + fBary2 * ( v[7].z - v[5].z );
				if( PHF_IsValidPoint( pointX, pointZ ) )
				{
					PHF_SetPickedXZ( pointX, pointZ );
				}
			}
			return;
		}

		PHF_RecursivePick( ( aCenterX + halfDist ), ( aCenterZ - halfDist ), halfDist );
	}

	return;
}

void CPickHeightField::PHF_VertexComparePick(  float aCenterX, float aCenterZ, float aDistance )
{
	/// HeightField 클래스에서 구현되어야 함. 
	/// 지형 폴리곤들을 일일히 비교 
}

bool	CPickHeightField::PHF_IntersectTriangle( D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
											 float* t, float* u, float* v )
{
  // Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &thePickRayDir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = thePickRayOrig - v0;
    }
    else
    {
//        tvec = v0 - thePickRayOrig;	/// skip back-side face
//        det = -det;
    }

    if( det < 0.0001f )
        return FALSE;

    // Calculate U parameter and test bounds
    *u = D3DXVec3Dot( &tvec, &pvec );
    if( *u < 0.0f || *u > det )
        return FALSE;

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    *v = D3DXVec3Dot( &thePickRayDir, &qvec );
    if( *v < 0.0f || *u + *v > det )
        return FALSE;

    // Calculate t, scale parameters, ray intersects triangle
    *t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

	return true;
}

