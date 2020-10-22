#include "CRenderManager.h"
#include <process.h> //_beginthreadex, _endthreadex

CRITICAL_SECTION	g_cDeviceFlag;
CRenderManager	g_RenderManager;



CRenderManager::CRenderManager(): CRenderState() ,
								  CTextureState() ,
								  CRenderer()
{
	InitializeCriticalSection( &g_cDeviceFlag );
}

CRenderManager::~CRenderManager()
{
	DeleteCriticalSection( &g_cDeviceFlag );
}

void CRenderManager::Init(void)
{
	InitRenderState();
	InitTextureState();
	InitRenderer();
}
