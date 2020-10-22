//...............................................................................................................
// 
//	CEFScapmper.h														 аЗаж
//
//...............................................................................................................
#ifndef __CEFSCAPMPER_H
#define __CEFSCAPMPER_H

#include "CEffectBase.h"
#include "CTail.h"

class CEFScapmper : public CEffectBase
{
public:
	 
	         CEFScapmper();
	virtual ~CEFScapmper();

	virtual int      Update(float dtime);

	int	Render(float dtime);

public:

	virtual void	 Init(void);

private:

	std::deque <D3DXVECTOR3> m_FootList;
	
	DWORD m_OldTime;

};

#endif


