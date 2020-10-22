#include "CFadeLoop.h"

CFadeLoop::CFadeLoop()
{
	m_State     = FADE_START_WAIT;
	m_Range     = 0.0f;
	m_OldTime   = 0;
}

CFadeLoop::~CFadeLoop()
{
}


// 현 시간 , 첫 대기 시간 , 페이드인 시간 , 대기시간 , 페이드 아웃 시간 , 끝날때 시간  
void CFadeLoop::SetStart(DWORD time,DWORD sw_time,DWORD fin_time,DWORD fw_time,DWORD fout_time,DWORD ew_time)
{
	m_OldTime = time;

	m_WaitTime[ 0 ] = sw_time;
	m_WaitTime[ 1 ] = fin_time;
	m_WaitTime[ 2 ] = fw_time;
	m_WaitTime[ 3 ] = fout_time;
	m_WaitTime[ 4 ] = ew_time;
}

	
// 현 시간 , 0 ~ 1.0f 
FADE_STATE CFadeLoop::Loop(DWORD time,float *pRange)
{
	if( FADE_DESTROY == m_State )
	{
		return FADE_DESTROY;

		if( NULL != pRange )
			*pRange = 0.0f;
	}

	if( ( time - m_OldTime ) >= m_WaitTime[ m_State ] )
	{
		m_OldTime = time;

		int TempState = ( int ) m_State;

		TempState++;

		m_State = ( FADE_STATE ) TempState;
	}
	
	if( m_WaitTime[ m_State ] == 0 )
	{
		m_Range = 0.0f;
	}
	else
	{
		if( m_State == FADE_OUT )
		m_Range = 1.0f - ( ( float ) ( time - m_OldTime ) / ( float ) m_WaitTime[ m_State ] );
		else
		m_Range = ( float ) ( time - m_OldTime ) / ( float ) m_WaitTime[ m_State ];
	}


	if( pRange != NULL )
		*pRange = m_Range;

	return m_State;
}

