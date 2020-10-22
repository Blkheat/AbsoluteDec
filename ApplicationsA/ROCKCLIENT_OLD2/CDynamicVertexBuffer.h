#pragma once
#include <RockPCH.h>

#ifdef DIRECT_VERSION_9_MJH
#include <d3d9.h>
#else
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH


class CDynamicVertexBuffer
{
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
#else
	LPDIRECT3DVERTEXBUFFER8 m_pVB;
#endif // DIRECT_VERSION_9_MJH
	DWORD					m_dwBase;
	DWORD					m_dwNextBase;
	DWORD					m_dwBatchBase;
	DWORD					m_dwBufferSize;
	DWORD					m_dwVertexSize;
	
	CDynamicVertexBuffer();
	void Release();
#ifdef DIRECT_VERSION_9_MJH
	int Create( LPDIRECT3DDEVICE9 device, int fvf, int fvfsize, int num, D3DPOOL pool );
#else
	int Create( LPDIRECT3DDEVICE8 device, int fvf, int fvfsize, int num, D3DPOOL pool );
#endif // DIRECT_VERSION_9_MJH
	
	//--------------------------------------------------------------------------------------------------------
	
	void *Lock( DWORD num, DWORD *lockedIndex );
	
	//--------------------------------------------------------------------------------------------------------
	
	void Unlock();
	
	//--------------------------------------------------------------------------------------------------------
	
	int LeftVertexCount();
	
	void ReStart();
	
	//--------------------------------------------------------------------------------------------------------
	
	void BeginBatch();
	
	int BatchVertexCount();
	
	void EndBatch();
	
	//--------------------------------------------------------------------------------------------------------
};

