//................................................................................................................
//
//  연속 애니 스킬
//
//................................................................................................................
#ifndef __CEFCONANISKILL_H
#define __CEFCONANISKILL_H

#include  "..\\obj\\Define.H"
#include "CEffectBase.h"

class CEFConAniSkill: public CEffectBase
{
public:
	 
	         CEFConAniSkill();
	virtual ~CEFConAniSkill();

public:

	virtual int		 CheckFrame(void);
	virtual void	 Init(void);
	virtual int		 Update(float dtime);
	virtual int		 Render(float dtime);
	int      SetAfterEffect(UINT uAttackId,BOOL IsLast);
			
	DWORD	m_dPrevTime;
	BOOL	m_IsRAImage;	///-- 잔상여부
	BOOL	m_IsRWAImage;	///-- 무기 잔상여부
	
	int	 m_nAniStep;
	int  m_nNowAniIndex;

	void * m_pConSkillData;
	
	std::deque <SAImageData> m_AImageList;

public:
};

#endif