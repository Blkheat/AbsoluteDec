//.......................................................................................................
// 
// Set Render Manager Header	 															By wxywxy
//
//.......................................................................................................

#ifndef __CRENDERMANAGER_H
#define __CRENDERMANAGER_H

#include <RockPCH.h>

#include "CRenderState.h"
#include "CTextureState.h"
#include "CRenderer.h"

class CRenderManager : public CRenderState , public CTextureState ,
					   public CRenderer
{
public:
	 
	 CRenderManager();
	~CRenderManager();

public:

	void Init(void);
};

extern CRenderManager	g_RenderManager;
extern CRITICAL_SECTION	g_cDeviceFlag;

#endif
