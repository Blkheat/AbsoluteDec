///////////////////////////////////////////////////////////////////////////////////
///		File	: PickHeightField.h
///		Desc	: HeightField Picking Abstract Class
///
///		Author	: Cho Hong-Seob
///		Team	: Program - Client Team
///		Date	: 2004-04-09
///
///		Copyright (c) 2004, RockSoft Co., Ltd. ALL RIGHTS RESERVED
///////////////////////////////////////////////////////////////////////////////////

#if !defined(__PICKHEIGHTMAP_H_INCLUDED__)
#define __PICKHEIGHTMAP_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const float Default_Distance = 512.0f;	/// 50m �̳��� ��ŷ
const float Pick_Scale_Min = 4.0f;	/// 4 * 4 ũ���� ���� ���������� ��ŷ 

enum NPickMethod
{
	n_LogicalVertex,
	n_TerrainVertex,
	n_TerrainVertexWide,
};

class CPickHeightField 
{
public:
	CPickHeightField();
	virtual ~CPickHeightField();

	bool PHF_Pick( HWND aWND, D3DXMATRIX* aMatProj, D3DXMATRIX* aMatView, DWORD aWidth, DWORD aHeight,
			   float aCenterX, float aCenterZ, float* aPickX, float* aPickZ, float aDistance = Default_Distance ); 	/// �⺻ : �ݰ� 25m

	virtual float PHF_GetHeight( const float aX, const float aZ ) = 0;		/// pure virtual function
	virtual bool  PHF_IsValidPoint( const float aX, const float aZ ) = 0;	/// pure virtual function

	int	thePickMethod;	/// ��� �ܰ迡�� ��ŷ�� �����ߴ��� �ѹ� Ȯ���غ��� ���� ���� 

protected:
	void	PHF_Reset();
	void	PHF_SetPickedXZ( float aX, float aZ );
	bool	PHF_IsFailed();

	void	PHF_RecursivePick( float aCenterX, float aCenterZ, float aDistance );

	virtual void PHF_VertexComparePick(  float aCenterX, float aCenterZ, float aDistance );

	bool	PHF_IntersectTriangle( D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, 
							   float* t, float* u, float* v );

    D3DXVECTOR3 thePickRayDir;
    D3DXVECTOR3 thePickRayOrig;

	float	theLastCenterX;
	float	theLastCenterZ;
	float	theLastDistance;

	float	thePickX;
	float	thePickZ;

};

#endif
