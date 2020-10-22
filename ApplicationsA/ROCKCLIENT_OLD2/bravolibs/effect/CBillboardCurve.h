#ifndef __CBILLBOARDCURVE_H
#define __CBILLBOARDCURVE_H

#include <RockPCH.h>

#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH
#include <deque>
#include <vector>

typedef struct 
{

    D3DXVECTOR3 pos;
	float		u,v;			

}BILLBOARD_CURVE_VERTEX;

typedef struct 
{
	D3DXVECTOR3 pos;
	float v;
}BILLBOARD_CURVE_DATA;

#define D3DFVF_BILLBOARD_CURVE_VERTEX		(D3DFVF_XYZ|D3DFVF_TEX1)

#define BILLBOARD_CURVE_MAX_POINT_BUFFER		10
#define BILLBOARD_CURVE_MAX_COUNT				20
#define BILLBOARD_CURVE_SHOW_MIN_VERTEX_SIZE	2


class CBillboardCurve
{
public:
	 
	 CBillboardCurve();
	~CBillboardCurve();

public:

#ifdef DIRECT_VERSION_9_MJH
	int Render(LPDIRECT3DDEVICE9 pDevice);
	int InitDevice(LPDIRECT3DDEVICE9 pDevice);
#else
	int Render(LPDIRECT3DDEVICE8 pDevice);
	int InitDevice(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	int AddPoint(D3DXVECTOR3 vPos);
	//...............................................................................................................................
	// 넓이 , 카메라 방향 벡터
	//...............................................................................................................................
	int Update(float fWidth,D3DXVECTOR3 vEyeDir);
	
private:

	void				MakeBezierCurve(int num,float range);

	//..................................................................................................................................................
	// Bezier Curve
	//..................................................................................................................................................
	int					Factorial(int n);
	int					Coefficient(int n, int k);
	float				CalBezier(int k, int n, float u);

	BILLBOARD_CURVE_VERTEX m_TraceBuffer[ BILLBOARD_CURVE_MAX_COUNT * 2 ];	
	BILLBOARD_CURVE_DATA   m_CurveBuffer[ BILLBOARD_CURVE_MAX_COUNT     ];	

	std::deque	<D3DXVECTOR3>	m_vList;								// 빌보드 축 위치 리스트

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9      m_pIB;
#else
	LPDIRECT3DVERTEXBUFFER8		m_pVB;
	LPDIRECT3DINDEXBUFFER8      m_pIB;
#endif // DIRECT_VERSION_9_MJH
	
};

#endif