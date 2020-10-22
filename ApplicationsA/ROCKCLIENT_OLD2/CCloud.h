#ifndef __CCLOUD_H
#define __CCLOUD_H

#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

#define MAX_CLOUD0_TIMER	100000
#define MAX_CLOUD1_TIMER	 50000

// ³· -> È²È¥ -> ¹ã -> »õº®
typedef enum
{

	CLOUD_DAYLIGHT = 0 ,
	CLOUD_DUSK			,
	CLOUD_NIGH			,
	CLOUD_DAWN			,
	CLOUD_CYCLE_MAX

}CLOUD_CYCLE;


typedef struct 
{
    D3DXVECTOR3 vPos;

	float		tu;
	float		tv;			

	float		tu1;
	float		tv1;			

}CLOUD_VERTEX;

#define D3DFVF_CLOUD_VERTEX		(D3DFVF_XYZ|D3DFVF_TEX2)

class CCloud
{
public:
	 
	 CCloud();
	~CCloud();
#ifdef DIRECT_VERSION_9_MJH
	BOOL	LoadTexture(LPDIRECT3DDEVICE9 pDevice,int Index,int CycleType);
	BOOL	Display(LPDIRECT3DDEVICE9 pDevice);
	BOOL	InitDevice(LPDIRECT3DDEVICE9 pDevice,float Loop);
#else
	BOOL	LoadTexture(LPDIRECT3DDEVICE8 pDevice,int Index,int CycleType);
	BOOL	Display(LPDIRECT3DDEVICE8 pDevice);
	BOOL	InitDevice(LPDIRECT3DDEVICE8 pDevice,float Loop);
#endif // DIRECT_VERSION_9_MJH

	void	DeleteAllMemory(void);
	void    DeleteAllVertex(void);
	
	HRESULT	InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice);
#else
	HRESULT RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

public:

	void	EnableCloud(BOOL Value)			{ m_IsCloudDisplay = Value; }
	void    SetMatrix(D3DXMATRIX &mat)		{ m_matWorld = mat; }
	
private:
	
	BOOL					m_IsCloudDisplay;
	
	D3DXMATRIX				m_matWorld;

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
#else
	LPDIRECT3DVERTEXBUFFER8	m_pVB;
	LPDIRECT3DINDEXBUFFER8	m_pIB;
#endif // DIRECT_VERSION_9_MJH

public:

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		m_pTextureList[ 3 ];
#else
	LPDIRECT3DTEXTURE8		m_pTextureList[ 3 ];
#endif // DIRECT_VERSION_9_MJH

private:

	void					DeleteAllTexture(void);

	float					m_CloudMove0;
	float					m_CloudMove1;

	float					m_Loop;

	DWORD					m_OldTimer0;
	DWORD					m_OldTimer1;

	float					m_Move0;
	float					m_Move1;
};


#endif