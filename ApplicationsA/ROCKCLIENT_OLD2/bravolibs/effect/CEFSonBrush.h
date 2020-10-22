//...............................................................................................................
// 
//	CEFSonBrush.h														 Son Brush
//
//...............................................................................................................
#ifndef __CEFSonBrush_H
#define __CEFSonBrush_H

#include "CEffectBase.h"
#include "CTail.h"

class CEFSonBrush : public CEffectBase
{
public:
	 
	         CEFSonBrush();
	virtual ~CEFSonBrush();

	virtual int      Update(float dtime);

	int	Render(float dtime);

public:

	virtual void	 Init(void);
	DWORD			 m_OldTime;

};

#endif


