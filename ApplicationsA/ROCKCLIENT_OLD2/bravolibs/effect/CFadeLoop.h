#ifndef __CFADELOOP_H
#define __CFADELOOP_H

#include <Windows.h>

typedef enum 
{
	FADE_START_WAIT	= 0 ,
	FADE_IN				,
	FADE_WAIT			,	
	FADE_OUT			,
	FADE_END_WAIT		,
	FADE_DESTROY		,
	FADE_MAX_STATE		

}FADE_STATE;


class CFadeLoop
{
public:
	 CFadeLoop();
	~CFadeLoop();


public:
	
	// 현 시간 , 첫 대기 시간 , 페이드인 시간 , 대기시간 , 페이드 아웃 시간 , 끝날때 시간  
	void    SetStart(DWORD time,DWORD sw_time,DWORD fin_time,DWORD fw_time,DWORD fout_time,DWORD ew_time);
	
	// 현 시간 , 0 ~ 1.0f 
	FADE_STATE  Loop(DWORD time,float *pRange = NULL );

	FADE_STATE  GetState(void)	{ return m_State; }
	float		GetRange(void)	{ return m_Range; }


private:

	float		m_Range;
	DWORD		m_OldTime;
	
	DWORD		m_WaitTime[ FADE_MAX_STATE - 1 ];

	FADE_STATE	m_State;
};

#endif

