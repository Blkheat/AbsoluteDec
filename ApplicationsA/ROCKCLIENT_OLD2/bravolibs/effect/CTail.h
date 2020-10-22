//.................................................................................................................
//
//	CTail.h																				By wxywxy
//
//.................................................................................................................
#ifndef __CTAIL_H
#define __CTAIL_H

#include <iostream>
#include <deque>
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

typedef struct 
{
    D3DXVECTOR3 pos;
	DWORD		color;
	float		u,v;			

}TAIL_VERTEX;

#define D3DFVF_TAIL_VERTEX			(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define TAIL_MAX_POINT_BUFFER		12
#define TRACE_MAX_COUNT				80
#define SHOW_MIN_VERTEX_SIZE		2

class CTail
{
public:

	 CTail();
	~CTail();

public:

#ifdef DIRECT_VERSION_9_MJH
	void    InitDevice(LPDIRECT3DDEVICE9 pDevice);
#else
	void    InitDevice(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void	Update(void);
#ifdef DIRECT_VERSION_9_MJH
	void	AddPoint(D3DXVECTOR3 &v0,D3DXVECTOR3 &v1,LPDIRECT3DDEVICE9 pDevice);
	void	Render(LPDIRECT3DDEVICE9 pDevice);
#else
	void	AddPoint(D3DXVECTOR3 &v0,D3DXVECTOR3 &v1,LPDIRECT3DDEVICE8 pDevice);
	void	Render(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH
	void    Clear(void);
	BOOL	ISDisplay(void)										  { return m_ISDisplay; }
	
	HRESULT	InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	int		GetSize()	{ return m_vTopList.size(); }

public:

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 GetTexture(void)							  { return m_pTexture; }
	void			   SetTexture(LPDIRECT3DTEXTURE9 *ppTexture)  { m_pTexture = *ppTexture; }
	LPDIRECT3DTEXTURE9			m_pTexture;
#else
	LPDIRECT3DTEXTURE8 GetTexture(void)							  { return m_pTexture; }
	void			   SetTexture(LPDIRECT3DTEXTURE8 *ppTexture)  { m_pTexture = *ppTexture; }
	LPDIRECT3DTEXTURE8			m_pTexture;
#endif // DIRECT_VERSION_9_MJH
	unsigned long				m_HashCode;

private:

	void				MakeBezierCurve(int num,float range);
	D3DXVECTOR3			PointOnCurve(D3DXVECTOR3 start, D3DXVECTOR3 control1, D3DXVECTOR3 control2, D3DXVECTOR3 end, float time);

	//..................................................................................................................................................
	// Bezier Curve
	//..................................................................................................................................................
	int					Factorial(int n);
	int					Coefficient(int n, int k);
	float				CalBezier(int k, int n, float u);	

private:

	std::deque	<D3DXVECTOR3>   m_vTopList;
	std::deque	<D3DXVECTOR3>   m_vBottomList;

	
	TAIL_VERTEX			   	    m_TraceBuffer[ TRACE_MAX_COUNT * 2 ];	

	BOOL						m_ISDisplay;

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9      m_pIB;
#else
	LPDIRECT3DVERTEXBUFFER8		m_pVB;
	LPDIRECT3DINDEXBUFFER8      m_pIB;
#endif // DIRECT_VERSION_9_MJH

	DWORD						m_OldTime;	

};

#endif