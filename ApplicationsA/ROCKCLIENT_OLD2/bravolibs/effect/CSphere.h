#ifndef __CSPHERE_H
#define __CSPHERE_H

#include <d3dx8math.h>
#include <d3d8.h>

typedef struct 
{
	
	D3DXVECTOR3 pos;
	float u;
	float v;

}SPHERE_VERTEX;

#define SPHERE_RING_COUNT			20
#define SPHERE_SEGMENT_COUNT		20
#define SPHERE_VERTEX_COUNT			( 2 * SPHERE_RING_COUNT * ( SPHERE_SEGMENT_COUNT + 1 ) )

#define D3DFVF_SPHERE_VERTEX		(D3DFVF_XYZ|D3DFVF_TEX1)



class CSphere
{
public:
	
	 CSphere();
	~CSphere();

public:

	int Render(void);

public:

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);

private:
	
	static  LPDIRECT3DVERTEXBUFFER8		m_pVB;
	static  LPDIRECT3DINDEXBUFFER8      m_pIB;

};

#endif

