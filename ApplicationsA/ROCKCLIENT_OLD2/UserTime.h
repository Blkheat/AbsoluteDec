// UserTime.h: interface for the UserTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTIME_H__0CF45AC0_5CC3_40FD_BFD9_5F4D8C6C729E__INCLUDED_)
#define AFX_USERTIME_H__0CF45AC0_5CC3_40FD_BFD9_5F4D8C6C729E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>

/********************************************************************
	created:	2007/12/17
	created:	17:12:2007   10:16
	filename: 	E:\RockDev\RockClient\ApplicationsA\RockClient\UserTime.h
	file path:	E:\RockDev\RockClient\ApplicationsA\RockClient
	file base:	UserTime
	file ext:	h
	author:		hicom15
	
	purpose:	중국 18세미만 사용자 들을 대상으로 게임을 5시간이상을 못하게 
				하는 시스템으로 경고메세지를 띄운다 
*********************************************************************/

class UserTime  
{
public:
	UserTime();
	virtual ~UserTime();
private:
	int m_iCurShowTime;
	DWORD m_Time;
	BOOL m_Lock;
	BOOL m_Show;
	
	int m_Cumulative;
	int m_MilliSec;
	DWORD m_NowTime;
	DWORD m_CurTime;
	BOOL bShowTimeCheck[5];


public:
	void Init();
	void Update();
	void CheckTime();
	void SetTime(int dTime);
	void SetCount(int Count){m_iCurShowTime = Count;}
	void SetLock(BOOL Lock);
	void WarningMsg(int Num);

	int GetCount();
	DWORD GetTime();
	void GetLock();
	int GetSec();
};

extern UserTime g_UserTime;

#endif // !defined(AFX_USERTIME_H__0CF45AC0_5CC3_40FD_BFD9_5F4D8C6C729E__INCLUDED_)

