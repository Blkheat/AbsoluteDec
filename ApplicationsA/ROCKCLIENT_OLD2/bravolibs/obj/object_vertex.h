
#ifndef __OBJECTVERTEX_H__
#define __OBJECTVERTEX_H__

#ifdef DIRECT_VERSION_9_MJH
#include <D3DX9.H>
#else
#include <D3DX8.H>
#endif // DIRECT_VERSION_9_MJH


//-----------------------------------------------------------------------------
// Desc: 모델 버텍스 구조체
//-----------------------------------------------------------------------------
struct OBJECTVERTEX
{
	D3DXVECTOR3		Vertex;		// x, y, z	
	D3DXVECTOR3		Normal;		// 노말값    
    float			u, v;		// U, V 값 	
};

#define D3DFVF_OBJECTVERTEX ( D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)



#endif