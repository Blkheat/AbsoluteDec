#ifndef __CEFSPEACIALITEM_H
#define __CEFSPEACIALITEM_H

#include "CEffectBase.h"

#include <deque>
using namespace std;

#pragma pack(push,1)
struct SSpeItmAimage
{
	DWORD dSetTime;
	float fScale;
	float fMaterialA;
	
	SSpeItmAimage()
	{
		dSetTime = 0;
		fScale = 0.0f;
		fMaterialA = 0.0f;
	}

	void operator=( SSpeItmAimage & r )
	{
		dSetTime = r.dSetTime;
		fScale = r.fScale;
		fMaterialA = r.fMaterialA;	
	}
};
#pragma pack(pop)

class CEFSpeacialItem : public CEffectBase
{
public:	
	         CEFSpeacialItem();
	virtual ~CEFSpeacialItem();

public:

	virtual void	 Init(void);	
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int		 SortRender(float dtime);

	virtual void	 SetDestroy(BOOL ISDamageAct = FALSE, BOOL ISBasicAttack = FALSE);
	
private:

	DWORD			 m_dInitTime;
	DWORD			 m_dPrevTime;	
	
	CParticleSystem	 m_ParticleLink[5];
	CParticleSystem	 m_AddParLink[4];

	deque < SSpeItmAimage > m_AImageList;
	
	CParticleData *  m_pData[2];
	
	BOOL			 m_IsInit;
	BOOL			 m_IsOneRender;
};

#endif