#ifndef __CEFPossessed_H
#define __CEFPossessed_H

#include "CEffectBase.h"

class CEFPossessed : public CEffectBase
{
public:
	 
	         CEFPossessed();
	virtual ~CEFPossessed();

public:

	virtual void	 Init(void);
	virtual int      Update(float dtime);

private:

	DWORD	m_OldTimer;								
	DWORD   m_RangeTimer;								// 지속 시간

	BOOL	m_ISAppear;
	BOOL	m_ISIng;
	int		m_AppearID;
};

#endif