
#ifndef __OBJECTVERTEX_H__
#define __OBJECTVERTEX_H__

#ifdef DIRECT_VERSION_9_MJH
#include <D3DX9.H>
#else
#include <D3DX8.H>
#endif // DIRECT_VERSION_9_MJH


//-----------------------------------------------------------------------------
// Desc: �� ���ؽ� ����ü
//-----------------------------------------------------------------------------
struct OBJECTVERTEX
{
	D3DXVECTOR3		Vertex;		// x, y, z	
	D3DXVECTOR3		Normal;		// �븻��    
    float			u, v;		// U, V �� 	
};

#define D3DFVF_OBJECTVERTEX ( D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)



#endif