//...............................................................................................................
// 
//	CEFFireWork.h														 ºÒ²É
//
//...............................................................................................................
#ifndef __CEFFireWork_H
#define __CEFFireWork_H

#include "CEffectBase.h"
#include "CEffectModel.h"

#include "list"

typedef enum 
{

	FIREWORK_WAIT = 0 ,
	FIREWORK_SHOT ,
	FIREWORK_BOOM,
	FIREWORK_HEART_BOOM,
	FIREWORK_BIG_BOOM1,
	FIREWORK_BIG_BOOM2,
	FIREWORK_SMALL_BOOM1,
	FIREWORK_SMALL_BOOM2,

}FIREWORK_STATE;

struct SpotData
{
	int				count;
	long			preTime;
	D3DXVECTOR3		pos;
	D3DXVECTOR3		dir;
};

class CEFFireWork : public CEffectBase
{
public:
	 
	         CEFFireWork();
	virtual ~CEFFireWork();

public:

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);

public:
	
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);

	virtual void	 Init(void);

	void Boom_Heart_FireFlower();

private:

	static CEffectModel	 m_BaseModel;
	list  <SpotData>     m_Spotlist;

//	CParticleSystem	 m_FireParticle;

	int				 m_TailParticleNum;
	int				 m_SpotParticleNum;
	int				 m_Color;

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vStartPos;
	D3DXVECTOR3		 m_vDestPos;
	D3DXVECTOR3		 m_vDistance;
	D3DXVECTOR3		 m_vPos;

	int				 m_Count;
	FIREWORK_STATE   m_State;

	D3DXVECTOR3 *	 m_pvStartPosList;
	D3DXVECTOR3	*	 m_pvEndPosList;
};

#endif
	

