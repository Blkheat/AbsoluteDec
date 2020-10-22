//................................................................................................................
//
//  Àº½Å
//
//................................................................................................................
#ifndef __CEFHIDE_H
#define __CEFHIDE_H

#include "CEffectBase.h"
#include "CFloatSequence.h"

class CEFHide: public CEffectBase
{
public:
	 
	         CEFHide();
	virtual ~CEFHide();

public:

	virtual void	 Init(void);
			int		 Update(float dtime);

public:

	BOOL			m_ISFade;
	CFloatSequence	m_AlphaSequence;
	DWORD			m_OldTimer;
};


#endif