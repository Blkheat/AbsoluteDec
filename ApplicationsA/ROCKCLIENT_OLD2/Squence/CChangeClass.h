#ifndef __CChangeClass_H
#define __CChangeClass_H

#include "CSquenceBase.h"
#include "..\\\bravolibs\\effect\\CRenderSurface.h"


typedef struct s_VERTEX3D
{
    D3DXVECTOR3 vPos;
    FLOAT       Tu,Tv;

	s_VERTEX3D()
	{
	}

}VERTEX3D;

typedef struct s_CrackTriangle
{
	
	VERTEX3D		   CrackVertex[3];
	D3DXVECTOR3		   vPos;

	BYTE			   RotType;		// 0 : x , 1 : y , 2 : z
	float			   RotAdd;		
	
	float			   AddX;		
	float			   AddY;

	float			   m_Angle;

	s_CrackTriangle()
	{
		vPos.x = vPos.y = vPos.z = 0.0f;
		m_Angle = 0.0f;
	}

}CRACKTRIANGLE,*LPCRACKTRIANGLE;


class CChangeClass : public CSquenceBase
{
public:
	 
			 CChangeClass();
    virtual ~CChangeClass();

	void DeleteAllMemory(void);

public:

	virtual int Squence00();
	virtual int Squence01();
	virtual int Squence02();
	virtual int Squence03();
	virtual int Squence04();
	virtual int Squence05();
	virtual int Squence06();
	virtual int Squence07();
	virtual int Squence08();
	virtual int Squence09();

private:

	void SetStartScene(void);
	void SetEndScene(void);

	void DisplayBG(void);
	
	void SetColor(D3DXCOLOR Value);

	void DrawCrack(void);

private:
	
	CAMERA_SAVE_VALUE	 m_SaveCamera;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9   m_pBGTexture;
	LPDIRECT3DTEXTURE9   m_pBGImageTexture;
#else
	LPDIRECT3DTEXTURE8   m_pBGTexture;
	LPDIRECT3DTEXTURE8   m_pBGImageTexture;
#endif // DIRECT_VERSION_9_MJH

	Model			    *m_pBGModel;

	D3DXMATRIX			 m_matPos;

	CRenderSurface		 m_SurfaceList[ 2 ];

	float				 m_fWidthRange;
	float				 m_fWidthRange1;
	float				 m_fThreeRange;
	float				 m_fTwoRange;

	void				 DrawPlayer(void);

	void DrawTexture( float x, float y, float w, float h, 
					  float tx, float ty, float tw, float th,
#ifdef DIRECT_VERSION_9_MJH
					  D3DCOLOR color , LPDIRECT3DTEXTURE9 pTex , LPDIRECT3DDEVICE9 pDevice );
#else
					  D3DCOLOR color , LPDIRECT3DTEXTURE8 pTex , LPDIRECT3DDEVICE8 pDevice );
#endif // DIRECT_VERSION_9_MJH

	LPCRACKTRIANGLE		m_pCrackList;
	int					m_CrackCount;
	
	void MakeCrackPlane(void);
	int  DrawPlayerFrame(Character *pChar,int AniIndex,int AniFrame);
	
	int					m_AniIndex[2];

	void	GetAniIndex(void);
	
	void	MakeBackImage(void);
	
	BOOL	m_ISMakeBackImage;	

public:

	virtual int SquenceCheck00();
	virtual int SquenceCheck01();
	virtual int SquenceCheck02();
	virtual int SquenceCheck03();
	virtual int SquenceCheck04();
	virtual int SquenceCheck05();
	virtual int SquenceCheck06();
	virtual int SquenceCheck07();
	virtual int SquenceCheck08();
	virtual int SquenceCheck09();

	virtual int SquenceFirstScene02();
	virtual int SquenceFirstScene01();
	virtual int SquenceFirstScene03();
	virtual int SquenceFirstScene04();
	virtual int SquenceFirstScene05();
	virtual int SquenceFirstScene07();
	virtual int SquenceFirstScene08();

	virtual int SquenceAniFrame02();
	
	float	m_fRotAngle;
};

#endif