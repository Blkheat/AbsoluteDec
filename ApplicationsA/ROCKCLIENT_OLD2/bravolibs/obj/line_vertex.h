
#ifndef __LINE_VERTEX_H__
#define __LINE_VERTEX_H__


#ifdef DIRECT_VERSION_9_MJH
#include <D3DX9.H>
#else
#include <D3DX8.H>
#endif // DIRECT_VERSION_9_MJH


//-----------------------------------------------------------------------------
// Desc: 라인  버텍스 구조체
//-----------------------------------------------------------------------------
struct LINE_VERTEX
{
	D3DXVECTOR3		Vertex;		// x, y, z	
	DWORD			color;		// 컬러값    
};

#define D3DFVF_LINE_VERTEX ( D3DFVF_XYZ|D3DFVF_DIFFUSE )


#endif