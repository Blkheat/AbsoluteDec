#ifndef __CVANIMODEL_H
#define __CVANIMODEL_H

#include "RockPCH.h"
#include "..\\Obj\\Character.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

class CVAniModel
{
public:
	 
	 CVAniModel();
	~CVAniModel();

public:
	
	 int	Create(Model *pModel,DWORD Time);
	 int	Update(Model *pModel,DWORD Time, Character * m_pChar = NULL );
	 int	Draw(Model *pModel, UINT uFrame = 30);
	 
	 D3DXVECTOR3 GetVertex(int Num);
	 int		 GetVertexCount(void)	{ return m_VertexCount; }


protected:

	 void	DeleteAllMemory(void);

protected:
	
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	m_VB;
#else
	LPDIRECT3DVERTEXBUFFER8	m_VB;
#endif // DIRECT_VERSION_9_MJH
	 BOOL						m_Init;
	 int						m_NowFrame;
	 int						m_EndFrame;
	 DWORD						m_PreTime;
	 int						m_VertexCount;

protected:

	 D3DXVECTOR3			   *m_pVertex;
	 D3DXVECTOR3			   *m_pNormal;
	 
	 UINT					    m_uFrame;			
};

#endif
