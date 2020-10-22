///////////////////////////////////////////////////////////////////////////////
///
///		File		: Util.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "Util.h"


__int64 lnFrequency = 0;
static __int64		lnCounts;
static double		dbReturn;


//-----------------------------------------------------------------------------
void InitTimer()
{
	 if( !QueryPerformanceFrequency( (LARGE_INTEGER *)&lnFrequency ) )
	 {
		 MessageBox( NULL, _T("!타이머 초기화 실패"), _T("에러"), MB_OK );
	 }
}

//-----------------------------------------------------------------------------
double GetFrequencyTime()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&lnCounts );
	dbReturn = (double)lnCounts / lnFrequency;
	
	//int64 elapsed = endtime - BeginTime; 
	//double duringtime = (double)elapsed / (double)Frequency;

	return dbReturn;
}

//-----------------------------------------------------------------------------
double GetCountTime()
{
	return (double)lnCounts;
}

//-----------------------------------------------------------------------------
__int64 GetFrequency()
{
	return lnFrequency;
}

//-----------------------------------------------------------------------------