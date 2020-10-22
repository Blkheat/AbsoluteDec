#include "RockPCH.h"

#include "CItemTexLoadThread.h"
#include "RockClient.h"
#include "bravolibs\\obj\\texturemanager.h"
#include "bravolibs\\obj\\player_manager.h"
#include "bravolibs\\obj\\Object_Manager.H"

CItemTexLoadThread g_CItemTexLoadThread;

CItemTexLoadThread::CItemTexLoadThread()
{

}

CItemTexLoadThread::~CItemTexLoadThread()
{

}


unsigned CItemTexLoadThread::ThreadFunction()
{
	//while loop will execute till the stop-event
	//is signaled
	while(isStopEventSignaled() == false) 
	{
		int nSize = 0;

		EnterCriticalSection( &g_TexManager.m_csTextureFlag );
		nSize = g_TexManager.theCreateTex.size();
		LeaveCriticalSection( &g_TexManager.m_csTextureFlag );	
		
		if( nSize == 0 )
		{
			::SetEvent(m_hStopEvent);
		}
		else
		{
			bool bFlag = false;
			
			EnterCriticalSection( &g_TexManager.m_csTextureFlag );
			std::list <DATA_CREATE_ATTR *>::iterator i = g_TexManager.theCreateTex.begin();
			if(i != g_TexManager.theCreateTex.end())
			{
				bFlag = true;
			}
			
			if(bFlag)
			{
				Texture* pTex = NULL;
				
				DATA_CREATE_ATTR * pData = (*i);
				
				if(pData)
				{
					switch( pData->byType )
					{
					case 0:				// NPC
					case 1:				// CHR
						pTex = g_Pc_Manager.m_pTex[pData->Index];
						break;
					case 2:				// OBJ
						pTex = &g_Obj_Manager.m_Tex[pData->Index];
						break;
					default:
						continue;				
						break;
					}

					int nType = pData->byType;

					if( pTex )
					{				
						///PRINT_DLG_LOG( "pData->byType = %d, pData->Index = %d", pData->byType, pData->Index );
						pTex->UpdateTexture( pData->byType, pData->Index );
					}
					
					SAFE_DELETE(pData);
					(*i) = NULL;
				}
							
				g_TexManager.theCreateTex.erase( i );											
			}		
			LeaveCriticalSection( &g_TexManager.m_csTextureFlag );
		}		
		
		::Sleep(100);
	}
	
	return 0; //return exit-code of thread
}

bool CItemTexLoadThread::Start()
{
	//check if thread is already running
	if (isRunning()) 
		return true;

	if (m_hStopEvent == NULL)
		return false;

	if (m_hThread != NULL)
		::CloseHandle(m_hThread);
	
	m_hThread = (HANDLE)_beginthreadex(NULL,
										0,
										&ThreadStarter,
										this ,
										0,
										&m_ThreadID);

	/*m_hThread = (HANDLE)_beginthreadex(NULL,
										0,
										&ThreadStarter,
										this ,
										CREATE_SUSPENDED,
										&m_ThreadID);*/
	if (m_hThread == 0) 
	{
		//failure
		m_ThreadID = -1;
		return false;
	}

	///SetThreadPriority(m_hThread, THREAD_PRIORITY_BELOW_NORMAL);
	///ResumeThread(m_hThread);

	return true;
}
