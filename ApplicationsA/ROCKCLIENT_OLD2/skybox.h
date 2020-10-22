//...........................................................................................................
//
//	CSkyBox.h
//
//...........................................................................................................

#ifndef __CSKYBOX_H
#define __CSKYBOX_H

typedef enum
{

	SKYBOX_RIGHT = 0 ,
	SKYBOX_LEFT		 ,
	SKYBOX_TOP		 ,
	SKYBOX_DOWN		 ,
	SKYBOX_FRONT	 ,
	SKYBOX_BACK		 ,
	SKYBOX_DIR_MAX

}SKYBOX_DIR;

// ³· -> È²È¥ -> ¹ã -> »õº®
typedef enum
{

	SKYBOX_DAYLIGHT = 0 ,
	SKYBOX_DUSK			,
	SKYBOX_NIGH			,
	SKYBOX_DAWN			,
	SKYBOX_CYCLE_MAX

}SKYBOX_CYCLE;


typedef struct 
{
    float       x;
    float       y;
    float       z;
	float		tu;
	float		tv;			

}SKYBOX_VERTEX;

#define D3DFVF_SKYBOX_VERTEX		(D3DFVF_XYZ|D3DFVF_TEX1)


class CSkyBox
{
public:

	 CSkyBox();
	~CSkyBox();
	
#ifdef DIRECT_VERSION_9_MJH
	BOOL	LoadTexture(LPDIRECT3DDEVICE9 pDevice,int Index,int CycleType);
	BOOL	Display(LPDIRECT3DDEVICE9 pDevice);
	BOOL	InitDevice(LPDIRECT3DDEVICE9 pDevice,float fXSize,float fYSize,float fZSize);
#else
	BOOL	LoadTexture(LPDIRECT3DDEVICE8 pDevice,int Index,int CycleType);
	BOOL	Display(LPDIRECT3DDEVICE8 pDevice);
	BOOL	InitDevice(LPDIRECT3DDEVICE8 pDevice,float fXSize,float fYSize,float fZSize);
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

	void	EnableSkyBox(BOOL Value)			{ m_IsSkyBoxDisplay = Value; }
	void    SetMatrix(D3DXMATRIX &mat)			{ m_matWorld = mat; }
	void    SetPos(float x,float y,float z);
	
private:
	
	BOOL					m_IsSkyBoxDisplay;
	
	D3DXMATRIX				m_matWorld;

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
#else
	LPDIRECT3DVERTEXBUFFER8	m_pVB;
#endif // DIRECT_VERSION_9_MJH

public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		m_pTextureList[ SKYBOX_DIR_MAX ];
#else
	LPDIRECT3DTEXTURE8		m_pTextureList[ SKYBOX_DIR_MAX ];
#endif // DIRECT_VERSION_9_MJH

private:

	void					DeleteAllTexture(void);

	float					m_fZSize;
	float					m_fYSize;
	float					m_fXSize;

};


#endif