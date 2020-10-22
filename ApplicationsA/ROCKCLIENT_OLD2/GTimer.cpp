
#include <Windows.h>
#include "GTimer.h"

CGTimer g_GTimer;

CGTimer::CGTimer()
{
   m_llLastElapsedTime = 0;
   m_llBaseTime = 0;   
   
   
   m_dLastElapsedTime = 0;          //최근 경과시간
   m_dBaseTime = 0;                 //베이스 시간

   m_fFrameElapsedSecTime = 0;
}

CGTimer::~CGTimer()
{
   
}

bool CGTimer::InitTimer()
{
	framesPerSecond = b_framesPerSecond = 0;
	lastTime = b_startTime = b_lastTime = 0.0;
	
    if (!QueryPerformanceFrequency(&m_ticksPerSecond))
	{
		// system doesn't support hi-res timer
		m_dLastElapsedTime = timeGetTime() * 0.001f;    
		isGoodTimer = false;
	}
	else
	{
		QueryPerformanceCounter(&m_startTime);
		isGoodTimer = true;
		m_llLastElapsedTime = m_startTime.QuadPart;
	}
	
	return	true;
}

void  CGTimer::UpdateFrameElapsedTime()
{
	if(isGoodTimer)
	{
		LARGE_INTEGER currentTime;
		
		QueryPerformanceCounter(&currentTime);		

		// reset the timer		
		float seconds =  ((float)currentTime.QuadPart - (float)m_llLastElapsedTime) / 
						  (float)m_ticksPerSecond.QuadPart;
				
		m_llLastElapsedTime = currentTime.QuadPart;    	
	
		m_fFrameElapsedSecTime = seconds;
	}
	else
	{
		double  b_currentTime;
	
		b_currentTime = timeGetTime() * 0.001f;

		double	b_seconds = b_currentTime - m_dLastElapsedTime;

		m_dLastElapsedTime = b_currentTime;	     

		m_fFrameElapsedSecTime = (float)b_seconds;
	}
}

float CGTimer::GetFrameElapsedSecTime()
{
	return m_fFrameElapsedSecTime;
}