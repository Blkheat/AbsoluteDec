#include "RockPCH.h"

#include "effect.h"
#include "RockClient.h"
#include "CRenderManager.h"

#include "CLogoDisplayThread.h"

CLogoDisplayThread::CLogoDisplayThread()
{

}

CLogoDisplayThread::~CLogoDisplayThread()
{

}

unsigned CLogoDisplayThread::ThreadFunction()
{
	//while loop will execute till the stop-event
	//is signaled	

	while(isStopEventSignaled() == false) 
	{
		if(32 == g_RockClient.m_wPixelFormat)
		{
			g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB( 255, 255, 255 ) , 1.0f, 0);
		}
		else
		{
			g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 255, 255, 255 ) , 1.0f, 0);
		}		
		
		if( SUCCEEDED( g_RockClient.BeginScene() ) )
		{
			if( nRui )
			{
				nRui->m_Logo2DEffect.Update2DEffect();
			}
			
			g_RockClient.LogoDisplay( 255 );
			g_RockClient.EndScene();
			g_RockClient.Present( NULL, NULL, NULL, NULL );
		}	
		
		Sleep(30);
	}
	
	return 0; //return exit-code of thread
}

