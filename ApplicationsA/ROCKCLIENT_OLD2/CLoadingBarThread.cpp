#include "RockPCH.h"

#include "effect.h"
#include "CLoadingBarThread.h"
#include "RockClient.h"

#include "rocklibs\\RockInterface\\LoadingWndProc.h"
#include "rocklibs\\RockInterface\\NonPlayerInfoWnd.h"

CLoadingBarThread g_CLoadingBarThread;

CLoadingBarThread::CLoadingBarThread()
{
	m_nNowStep = nLoadingNone;
}

CLoadingBarThread::~CLoadingBarThread()
{

}


unsigned CLoadingBarThread::ThreadFunction()
{
	//while loop will execute till the stop-event
	//is signaled	

	m_nNowStep = nLoadingStart;

	while(isStopEventSignaled() == false) 
	{
		if( m_nNowStep == nLoadingEnd )
		{
			::SetEvent(m_hStopEvent);			
		}
		else
		{
			g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) , 
										D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 500 , TRUE );
			m_nNowStep = nLoadingSFadeIn;

			while( 1 )
			{
				if( g_Particle.m_Fade.GetState() == FADE_STATE_CONTINUE )
				{
					break;
				}				

				g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
									D3DCOLOR_XRGB( 0x55, 0x66, 0x77 ), 1.0f, 0);

				LoadingBarWnd.Update();				
				g_Particle.m_Fade.Process( g_lpDevice );
				
				if( SUCCEEDED( g_RockClient.BeginScene() ) )
				{
					LoadingBarWnd.DrawScreenAfterImage();
					g_Particle.m_Fade.Render();
					g_RockClient.EndScene();					
				}

				g_RockClient.Present( NULL, NULL, NULL, NULL );
				
				Sleep( 10 );
			}
		
			g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 
									D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) , 500 , FALSE );
			m_nNowStep = nLoadingSFadeOut;

			while( 1 )
			{
				g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
							  D3DCOLOR_XRGB( 0x55, 0x66, 0x77 ), 1.0f, 0);

				nRui->UpdatePcData();
				nRui->RenderProc();

				g_Particle.m_Fade.Process( g_lpDevice );
				
				if( g_Particle.m_Fade.GetState() != FADE_STATE_NONE )
				{
					if( SUCCEEDED( g_RockClient.BeginScene() ) )
					{
						g_Particle.m_Fade.Render();
						g_RockClient.EndScene();					
					}
				}

				g_RockClient.Present( NULL, NULL, NULL, NULL );

				if( ( g_Particle.m_Fade.GetState() == FADE_STATE_NONE ) && LoadingBarWnd.Percent == 100 )
				{
					m_nNowStep = nLoadingEnd;
					g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
										g_RockClient.m_SceneManager.GetBGColor() , 1.0f, 0);

					nRui->UpdatePcData();
					nRui->RenderProc();

					g_RockClient.Present( NULL, NULL, NULL, NULL );

					/*while( 1 )
					{
						if( g_Particle.m_Fade.GetState() == FADE_STATE_CONTINUE )
						{
							m_nNowStep = nLoadingEnd;							
						}						
						g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
										g_RockClient.m_SceneManager.GetBGColor() , 1.0f, 0);

						nRui->UpdatePcData();
						nRui->RenderProc();

						g_Particle.m_Fade.Process( g_lpDevice );
					
						if( SUCCEEDED( g_RockClient.BeginScene() ) )
						{
							g_Particle.m_Fade.Render();
							g_RockClient.EndScene();						
						}

						g_RockClient.Present( NULL, NULL, NULL, NULL );					
						
						Sleep( 10 );
					}*/

					if( m_nNowStep == nLoadingEnd )
					{
						break;
					}					
				}
				
				Sleep( 10 );					
			}		
		}
		
		Sleep(10);
	}
	
	return 0; //return exit-code of thread
}
