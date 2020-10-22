#include "CCollision.h"

#define ALMOST360 (0.99999 * (2.0 * D3DX_PI))


//...............................................................................................................
// Vector Betweeen Radian
//...............................................................................................................
float Vect3BetweenRadian(D3DXVECTOR3 &v0,D3DXVECTOR3 &v1)
{
	D3DXVECTOR3 vN0,vN1;
	
	D3DXVec3Normalize( &vN0 , &v0 );
	D3DXVec3Normalize( &vN1 , &v1 );

	double dot = (double)D3DXVec3Dot( &vN0 , &vN1 );
	
	double rad = acos( dot );

	if( _isnan( rad ) )
		return 0.0f;

	return (float) rad;
}


CCollision::CCollision()
{
}

CCollision::~CCollision()
{
}


//.....................................................................................................................
// 삼각형 반직선 충돌 테스트
//.....................................................................................................................
BOOL CCollision::ISFaceCollision(D3DXVECTOR3 &vFace0,D3DXVECTOR3 &vFace1,D3DXVECTOR3 &vFace2,
							     D3DXVECTOR3 &vStartLine,D3DXVECTOR3 &vEndLine,D3DXVECTOR3 vCollision)
{

	D3DXPLANE Plane;

	D3DXPlaneFromPoints( &Plane , &vFace0 , &vFace1 , &vFace2 );
	
	if( FALSE == ISIntersectPlane( vStartLine , vEndLine , Plane ) )
		return FALSE;

	D3DXPlaneIntersectLine( &vCollision , &Plane , &vStartLine , &vEndLine );

	if( FALSE == ISInsideFace( vFace0 , vFace1 , vFace2 , vCollision ) )
		return FALSE;

/*
	D3DXPLANE Plane;

	D3DXPlaneFromPoints( &Plane , &vFace0 , &vFace1 , &vFace2 );
	
	if( TRUE == ISIntersectPlane( vStartLine , vEndLine , Plane ) )
	{
		D3DXPlaneIntersectLine( &vCollision , &Plane , &vStartLine , &vEndLine );

		if( TRUE == ISInsideFace( vFace0 , vFace1 , vFace2 , vCollision ) )
			return TRUE;
	}

	D3DXPlaneFromPoints( &Plane , &vFace2 , &vFace1 , &vFace0 );

	if( TRUE == ISIntersectPlane( vStartLine , vEndLine , Plane ) )
	{
		D3DXPlaneIntersectLine( &vCollision , &Plane , &vStartLine , &vEndLine );

		if( TRUE == ISInsideFace( vFace0 , vFace1 , vFace2 , vCollision ) )
			return TRUE;
	}
*/	

	return TRUE;
}

//.....................................................................................................................
// 반직선이 평면에 걸치남? 평면의 공통으로 아래 또는 위쪽에 있을때는 안걸침
//.....................................................................................................................
BOOL CCollision::ISIntersectPlane(D3DXVECTOR3 &v0,D3DXVECTOR3 &v1,D3DXPLANE &Plane)
{
	float d1,d2;

	// 점이 평면의 아래쪽인지 위쪽인지 검사
	d1 = D3DXPlaneDotCoord( &Plane , &v0 );
	d2 = D3DXPlaneDotCoord( &Plane , &v1 );

	// 서로 다른 평면일때 (-) 값이 나옴
	if( ( d1 * d2 ) >=0 ) 
		return FALSE;

	return TRUE;
}

//.....................................................................................................................
// 점이 삼각형 내에 있남?
//.....................................................................................................................
BOOL CCollision::ISInsideFace(D3DXVECTOR3 &v0,D3DXVECTOR3 &v1,D3DXVECTOR3 &v2,D3DXVECTOR3 &vPos)
{
	D3DXVECTOR3	vA,vB;
	double      Randian = 0.0f;

	vA = v0 - vPos;	
	vB = v1 - vPos;
	Randian += Vect3BetweenRadian(vA, vB);	

	vA = v2 - vPos;
	Randian += Vect3BetweenRadian(vB, vA);	

	vB = v0 - vPos;
	Randian += Vect3BetweenRadian(vA, vB);
	
	double dAMost360 = (double)ALMOST360;

	if( (dAMost360 - Randian)  <=  0.01 ) 
	{
		return TRUE;		
	}

	return FALSE;
}
