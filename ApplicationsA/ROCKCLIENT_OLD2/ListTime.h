// ListTime.h: interface for the ListTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTTIME_H__8F7F8BE2_A4A8_40B9_8EB8_533AAAA49F28__INCLUDED_)
#define AFX_LISTTIME_H__8F7F8BE2_A4A8_40B9_8EB8_533AAAA49F28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
	시간제 아에템을 위한 구조체 
	1일이상 남은 경우 1 일 단위로, 
	1일 이내인 경우 1시간단위로, 
	1시간 이내인 경우 10분 단위로 안내한다.
*/
struct MessageTime
{	
	DWORD ItemUID;	
	DWORD Time;		//언제뿌릴것인가
	BOOL  bIsShow;	//않았으면 한번 뿌려라.. 

	MessageTime()
	{
		ItemUID = 0 ; 
		Time  = 0; 
		bIsShow = 0 ;
	}
};

class ListTime  
{
private:

	DWORD m_CurTime;
	DWORD m_NowTime;
	DWORD m_Cumulative;
	DWORD m_MilliSec;

	SPayTimeItem **PayTimeItemList;
	MessageTime m_PayMessageTime[RP_MAX_TIMELIMIT_PAY_CONTENTS];

public:
	ListTime();
	virtual ~ListTime();

	void Init();
	void Update();
	
	
	void	DeletePayItem(DWORD ItemUID);

	void	GetTimeRemainStr(DWORD ItemUID , TCHAR *str);

	//버프 아이템 이건 가지고 있지 않아서 ..코드값으로 검사 . 
	void	GetTimeEffectStr(DWORD Code , TCHAR *str);

	tm		GetTimeTm(DWORD dwTime);
	DWORD	GetTimePayItem(DWORD ItemUID);	
	DWORD	GetTimePayEffect(DWORD Code);

	int		GetSec();
	int		FirstShowItem(DWORD Time , TCHAR *ItemName);

	//시간제 아이템 시스템 메세지로 경고 표시 

	void	DelMsgTime(DWORD ItemUID);		//메세지 시간 지울떄 .. 
	void	UpdateMessageTime();			// 음.. 
	void	WarningMessage(SPayTimeItem *PayItem);	//경고 메세지 띄울때 .. 
	void	InsertMessageTime(DWORD ItemUID , DWORD dwTime);	// 새로 추가될때 .. 

};


extern ListTime g_ListTime;

#endif // !defined(AFX_LISTTIME_H__8F7F8BE2_A4A8_40B9_8EB8_533AAAA49F28__INCLUDED_)
