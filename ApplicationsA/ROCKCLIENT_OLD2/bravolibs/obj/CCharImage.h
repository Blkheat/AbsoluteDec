#ifndef __CCHARIMAGE_H
#define __CCHARIMAGE_H

#include "RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include "d3d9.h"
#include "d3dx9math.h"
#else
#include "d3d8.h"
#include "d3dx8math.h"
#endif // DIRECT_VERSION_9_MJH


#include <vector>
#include <list>

using namespace	std;

typedef enum n_CharImageType
{

	CHARIMAGE_32SIZE = 0 ,
	CHARIMAGE_64SIZE     ,

}NCHAR_IMAGE_TYPE;

typedef struct s_CharImageData
{
	int					unique;
	int					race;
	bool				isBackImg;				//검은 배경을 그리는지의 여부
	
	//by simwoosung	
	static	float       m_fReNewTime;
	bool				m_IsRender;
	bool				m_IsOneTimeRender;
	float				m_fCumulativeTime;		//텍스쳐가 생성된후 일정시간후에 재갱신-시간 누적

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	pCharImageTex;
	LPDIRECT3DTEXTURE9	pCharImageTexMem;
	LPDIRECT3DSURFACE9	pCharImageSurface;		// 비디오 서피스 
	LPDIRECT3DSURFACE9	pCharImageSurfaceMem;	// 메모리 서피스
#else
	LPDIRECT3DTEXTURE8	pCharImageTex;
	LPDIRECT3DTEXTURE8	pCharImageTexMem;
	LPDIRECT3DSURFACE8	pCharImageSurface;		// 비디오 서피스 
	LPDIRECT3DSURFACE8	pCharImageSurfaceMem;	// 메모리 서피스
#endif // DIRECT_VERSION_9_MJH 
	
	void *				m_pCharacter;	

	s_CharImageData()
	{
		pCharImageTex     = NULL;
		pCharImageSurface = NULL;
		pCharImageSurfaceMem = NULL ;
		pCharImageTexMem = NULL;


		unique            = -1;
		race			  = -1;
		isBackImg		  = true;

		m_IsRender		  = false;
		m_IsOneTimeRender = false;
		m_fCumulativeTime = 0.0f;
		m_pCharacter = NULL;
	}

	~s_CharImageData()
	{
		InvalidateDeviceObject();
		
		SAFE_DELETE(m_pCharacter);
	}
	
	void InvalidateDeviceObject()
	{
		if( NULL != pCharImageTex )
		{
			pCharImageTex->Release();
			pCharImageTex = NULL;
		}

		if( NULL != pCharImageSurface )
		{
			pCharImageSurface->Release();
			pCharImageSurface = NULL;
		}	
		
		if( NULL != pCharImageSurfaceMem)
		{
			pCharImageSurfaceMem->Release();
			pCharImageSurfaceMem = NULL;
		}

		if( NULL != pCharImageTexMem)	
		{
			pCharImageTexMem->Release();
			pCharImageTexMem = NULL;
		}
	}

}CHAR_IMAGE_DATA,*LPCHAR_IMAGE_DATA;

typedef struct s_VERTEX_2D_CHARIMG
{
    D3DXVECTOR4			pos;
    D3DCOLOR			color;
    FLOAT				tu0, tv0;

	s_VERTEX_2D_CHARIMG()
	{
		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;
		pos.w = 1.0f;

		color = D3DCOLOR_ARGB( 0 , 255 , 255 , 255 );
	}

}VERTEX_2D_CHARIMG;

#define D3DFVF_VERTEX_2D_CHARIMG ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 )

class CCharImage
{
public:
	 
	 CCharImage();
	~CCharImage();

public:

#ifdef DIRECT_VERSION_9_MJH
	int		Init(LPDIRECT3DDEVICE9 pDevice);
#else
	int		Init(LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	void	DeleteAll(void);

#ifdef DIRECT_VERSION_9_MJH
	int		Create(int unique,int race,LPDIRECT3DDEVICE9 pDevice, bool isBackImg = true);
	int		Change(int unique,int race,LPDIRECT3DDEVICE9 pDevice, bool isBackImg = true);
#else
	int		Create(int unique,int race,LPDIRECT3DDEVICE8 pDevice, bool isBackImg = true);
	int		Change(int unique,int race,LPDIRECT3DDEVICE8 pDevice, bool isBackImg = true);
#endif // DIRECT_VERSION_9_MJH
	int     Delete(int unique,int race, bool isBackImg = true);
#ifdef DIRECT_VERSION_9_MJH
	int		Render(int x,int y,int unique,int race,NCHAR_IMAGE_TYPE type,LPDIRECT3DDEVICE9 pDevice,
#else
	int		Render(int x,int y,int unique,int race,NCHAR_IMAGE_TYPE type,LPDIRECT3DDEVICE8 pDevice,
#endif // DIRECT_VERSION_9_MJH 
				   bool isBackImg = true);
	LPCHAR_IMAGE_DATA *Find(int unique,int race, bool isBackImg = true);

	//by simwoosung
	int		Render(int x,int y,int unique,int race,NCHAR_IMAGE_TYPE type, 
#ifdef DIRECT_VERSION_9_MJH
		LPDIRECT3DTEXTURE9 texmask,LPDIRECT3DDEVICE9 pDevice,  bool isBackImg = true , bool alive  = true);
#else
		LPDIRECT3DTEXTURE8 texmask,LPDIRECT3DDEVICE8 pDevice,  bool isBackImg = true , bool alive  = true);
#endif // DIRECT_VERSION_9_MJH

	//by simwoosung
	HRESULT	InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	HRESULT RestoreDeviceObject(LPDIRECT3DDEVICE9 pDevice);
	LPDIRECT3DTEXTURE9	GetPartyCharMask()	{ return m_PartyCharMask; }
#else
	HRESULT RestoreDeviceObject(LPDIRECT3DDEVICE8 pDevice);
	LPDIRECT3DTEXTURE8	GetPartyCharMask()	{ return m_PartyCharMask; }
#endif // DIRECT_VERSION_9_MJH
	
	void ReFreshPetImage();

private:

#ifdef DIRECT_VERSION_9_MJH
	int		MakeImage(LPCHAR_IMAGE_DATA *ppData,LPDIRECT3DDEVICE9 pDevice, bool bFlag);	
#else
	int		MakeImage(LPCHAR_IMAGE_DATA *ppData,LPDIRECT3DDEVICE8 pDevice, bool bFlag);	
#endif // DIRECT_VERSION_9_MJH

	list<LPCHAR_IMAGE_DATA>	m_List;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9				m_MaskTex[2];
	LPDIRECT3DTEXTURE9				m_CharMask;
	LPDIRECT3DTEXTURE9				m_PartyCharMask;
#else
	LPDIRECT3DTEXTURE8				m_MaskTex[2];
	LPDIRECT3DTEXTURE8				m_CharMask;
	LPDIRECT3DTEXTURE8				m_PartyCharMask;
#endif // DIRECT_VERSION_9_MJH

	LPD3DXRENDERTOSURFACE			m_pRenderToSurface;	

	void DrawTexture( float x, float y, float w, float h, 
					  float tx, float ty, float tw, float th,
#ifdef DIRECT_VERSION_9_MJH
					  D3DCOLOR color , LPDIRECT3DDEVICE9 pDevice );
#else
					  D3DCOLOR color , LPDIRECT3DDEVICE8 pDevice );
#endif // DIRECT_VERSION_9_MJH

	D3DXMATRIX						m_matView;
	D3DXMATRIX						m_matWorld;

};

#endif
