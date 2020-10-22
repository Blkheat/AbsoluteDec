#ifndef __CFACE_EXP_H
#define __CFACE_EXP_H

#include "RockPCH.h"
#include <vector>

class CFaceExpData
{
public:
	 
	 CFaceExpData();
	~CFaceExpData();

public:

	enum FACE_EXP 
	{ 
		FACE_EXP_NORMAL       = -2 , 
		FACE_EXP_TWINKLE_WAIT = -1 ,
		FACE_EXP_TWINKLE      =  0 ,
		FACE_EXP_MAXCOUNT
	};

public:

	DWORD				GetDisplayTime(FACE_EXP eType);
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	GetTexture(FACE_EXP eType);
#else
	LPDIRECT3DTEXTURE8	GetTexture(FACE_EXP eType);
#endif // DIRECT_VERSION_9_MJH

public:
	
	DWORD			    m_FaceCode;
	DWORD			    m_TwinkleWaitMin;
	DWORD			    m_TwinkleWaitMax;
	std::string		    m_TextureName[ FACE_EXP_MAXCOUNT ];
	DWORD			    m_DisplayTime[ FACE_EXP_MAXCOUNT ];

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9  m_pTextureList[ FACE_EXP_MAXCOUNT ];
#else
	LPDIRECT3DTEXTURE8  m_pTextureList[ FACE_EXP_MAXCOUNT ];
#endif // DIRECT_VERSION_9_MJH

public:
	
	void				DeleteAllMemory(void);


public:

	void operator=(const CFaceExpData TempData)
	{
		m_FaceCode		 = TempData.m_FaceCode;
		m_TwinkleWaitMin = TempData.m_TwinkleWaitMin;
		m_TwinkleWaitMax = TempData.m_TwinkleWaitMax;
		
		for( int i = 0 ; i < FACE_EXP_MAXCOUNT ; ++i )
		{
			m_TextureName [ i ] = TempData.m_TextureName [ i ];
			m_pTextureList[ i ] = TempData.m_pTextureList[ i ];
			m_DisplayTime [ i ] = TempData.m_DisplayTime [ i ];
		}
	}

};


class CFaceExp
{
public:

	 CFaceExp();
	~CFaceExp();

public:
	
	int					Init(void);
	void				DeleteAllMemory(void);
	void			    DeleteAllTexture();

	CFaceExpData		*GetFaceExpInfo(DWORD FaceCode);

public:

	std::vector <CFaceExpData *>	m_FaceExpList;
};

#endif