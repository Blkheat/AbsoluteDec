//.......................................................................................................
// 
// Set Light Header	 															By wxywxy
//
//.......................................................................................................

#ifndef __CLIGHTSTATE_H
#define __CLIGHTSTATE_H

#include "RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3d9.h>
#else
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

class CLightState
{
public:
	 
	 CLightState();
	~CLightState();

public:

	void InitLightState(void);
};

#endif
