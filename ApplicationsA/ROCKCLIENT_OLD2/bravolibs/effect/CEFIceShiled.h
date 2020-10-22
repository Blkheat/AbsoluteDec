//...............................................................................................................
// 
//	CEFIceShiled.h														 아이스 실드 ( 지속성 )
//
//...............................................................................................................
#ifndef __CEFIceShiled_H
#define __CEFIceShiled_H

#include "CEffectBase.h"

class CEFIceShiled : public CEffectBase
{
public:
	 
	         CEFIceShiled();
	virtual ~CEFIceShiled();

public:

	virtual void	 Init(void);
	virtual int      Update(float dtime);

};

#endif


