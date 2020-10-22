#ifndef __CDROPEFFECT_H
#define __CDROPEFFECT_H

#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH
#include <list>

typedef enum DROPEFFECT_STATE
{
	DROPST_DELAY	,				// ´ë±â 
	DROPST_DROP		,				// ¶³¾îÁú¶§
	DROPST_END		,				// ³¡
	DROPST_DESTROY					// Á×À»¶§
};

typedef struct s_DropEffectData
{
	
	D3DXVECTOR3			vStartPos;
	D3DXVECTOR3			vEndPos;
	D3DXVECTOR3			vPos;
	DROPEFFECT_STATE	State;
	DWORD				OldTime;
	DWORD				WaitTime;
	DWORD				DropTime;
	float				fYadian;

	s_DropEffectData()
	{
		memset(this, 0, sizeof(s_DropEffectData));
	}

}DROP_EFFECT_DATA;

class CDropEffect
{
public:
	 
	 CDropEffect();
	~CDropEffect();

public:

	 int			   Init(float CenterX,float CenterZ,float Height,float Angle,DWORD DropMinTime,DWORD DropMaxTime,int Count,float Radius,DWORD nowTime,DWORD RunningTime,BOOL Loop = FALSE );
	 void			   Update(DWORD Time);

	 void			   StartPosition(void);
	 DROP_EFFECT_DATA *GetDropData(void);

	 int			   GetDataSize(void)	{ return m_List.size(); }

	 D3DXVECTOR3	   GetCenterPos(void)	{ return m_vCenter; }
	 void			   SetIsLoop(BOOL bFlag) { m_ISLoop = bFlag; }

	 //by simwoosung
	 int			   InitBack(D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, float fMinHei,float fMaxHei, 
					   DWORD dMinTime,DWORD dMaxTime,int Count,float fSRadius,
					   float fERadius,DWORD nowTime,DWORD RunningTime,BOOL Loop );
	 void			   UpdateBack(DWORD Time);

private:

	std::list	<DROP_EFFECT_DATA>	m_List;

	BOOL							m_ISStart;	

	std::list	<DROP_EFFECT_DATA>::iterator m_pPointer;

	D3DXVECTOR3						m_vDir;
	D3DXVECTOR3						m_vCenter;
	BOOL							m_ISLoop;
	float							m_Radius;
	float							m_Height;
	float							m_CenterX;
	float							m_CenterZ;

	float							m_fSRadius;
	float							m_fMinHei;
	float							m_fMaxHei;
	D3DXVECTOR3						m_vStart;
	D3DXVECTOR3						m_vEnd;
};

#endif