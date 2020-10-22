#ifndef __CEFEldaWeaponItem_H
#define __CEFEldaWeaponItem_H

#include "CEffectBase.h"
#include "CEFSpeacialItem.h"

#include <deque>
using namespace std;

class CEFEldaWeaponItem : public CEffectBase
{
public:	
	CEFEldaWeaponItem();
	virtual ~CEFEldaWeaponItem();
	
public:
	
	virtual void	 Init(void);	
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int		 SortRender(float dtime);
	
	virtual void	 SetDestroy(BOOL ISDamageAct = FALSE, BOOL ISBasicAttack = FALSE);

	
	BYTE							 m_Level;
	int								 m_WeaponType;
	int								 m_Count;

	float tx,ty,tz,tx2,ty2,tz2;
private:
	
	DWORD			 m_dInitTime;
	DWORD			 m_dPrevTime;	
	
	CParticleSystem	 m_ParticleLink[5];
	CParticleSystem	 m_AddParLink[4];
	
	deque < SSpeItmAimage > m_AImageList;
	
	CParticleData *  m_pData[2];
	
	BOOL			 m_IsInit;
	BOOL			 m_IsOneRender;
	
	float m_fScaleSizeStart ; 
	float m_fScaleSizeEnd  ;


};

#endif
