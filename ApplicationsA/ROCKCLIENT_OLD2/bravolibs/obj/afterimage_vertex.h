
#ifndef __AFTERIMAGE_VERTEX_H__
#define __AFTERIMAGE_VERTEX_H__

#ifdef DIRECT_VERSION_9_MJH
#include <D3DX9.H>
#else
#include <D3DX8.H>
#endif // DIRECT_VERSION_9_MJH

//-----------------------------------------------------------------------------
// Desc: �˱�(�ܻ�) ���ؽ� ����ü
//-----------------------------------------------------------------------------

struct AFTERIMAGE_VERTEX
{
	float	x, y, z;
	DWORD	color;	  
    float	u, v;
};

#define D3DFVF_AFTERIMAGE_VERTEX ( D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1 )


#endif