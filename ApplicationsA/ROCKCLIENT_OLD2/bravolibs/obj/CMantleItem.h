#if !defined(AFX_CMANTLEITEM__INCLUDED_)
#define AFX_CMANTLEITEM__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RockPCH.h"
#include "Model.H"

#define MAX_MANTLE_TYPE_TEXTURE		5

class CMantleItem
{
public:
	 
	 CMantleItem();
	~CMantleItem();

	void Render(int race,int AniIndex,D3DXMATRIX matParent);
	void RenderShadow(int race,int AniIndex);

	// 종족 , 에니메이션 인덱스 , 망토 타입 , 현제 에니 값, 중간값 , 부모 메트릭스		
	void Update(int race,int AniIndex,int Type,int ani_frame,float middle_time);

	int  LoadMSH(int race,int AniIndex,int Name);
	
	void SetType(int type)							{ m_MantleType = type; }
	int  GetType(void)								{ return m_MantleType; }

	int  CreateTexture(void);

	Model *GetModel(int race,int AniIndex)			{ return &m_ModelList[ race ][ AniIndex ]; }

private:
	
	 Model		m_ModelList[4][200];
	 
	 int		m_MantleType;														// 망토 타입 = 텍스처
#ifdef DIRECT_VERSION_9_MJH
	 LPDIRECT3DTEXTURE9		m_pTexture[ MAX_MANTLE_TYPE_TEXTURE ];
#else
	 LPDIRECT3DTEXTURE8		m_pTexture[ MAX_MANTLE_TYPE_TEXTURE ];
#endif // DIRECT_VERSION_9_MJH		

};

#endif

