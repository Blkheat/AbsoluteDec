// ListTime.h: interface for the ListTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTTIME_H__8F7F8BE2_A4A8_40B9_8EB8_533AAAA49F28__INCLUDED_)
#define AFX_LISTTIME_H__8F7F8BE2_A4A8_40B9_8EB8_533AAAA49F28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
	�ð��� �ƿ����� ���� ����ü 
	1���̻� ���� ��� 1 �� ������, 
	1�� �̳��� ��� 1�ð�������, 
	1�ð� �̳��� ��� 10�� ������ �ȳ��Ѵ�.
*/
struct MessageTime
{	
	DWORD ItemUID;	
	DWORD Time;		//�����Ѹ����ΰ�
	BOOL  bIsShow;	//�ʾ����� �ѹ� �ѷ���.. 

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

	//���� ������ �̰� ������ ���� �ʾƼ� ..�ڵ尪���� �˻� . 
	void	GetTimeEffectStr(DWORD Code , TCHAR *str);

	tm		GetTimeTm(DWORD dwTime);
	DWORD	GetTimePayItem(DWORD ItemUID);	
	DWORD	GetTimePayEffect(DWORD Code);

	int		GetSec();
	int		FirstShowItem(DWORD Time , TCHAR *ItemName);

	//�ð��� ������ �ý��� �޼����� ��� ǥ�� 

	void	DelMsgTime(DWORD ItemUID);		//�޼��� �ð� ��� .. 
	void	UpdateMessageTime();			// ��.. 
	void	WarningMessage(SPayTimeItem *PayItem);	//��� �޼��� ��ﶧ .. 
	void	InsertMessageTime(DWORD ItemUID , DWORD dwTime);	// ���� �߰��ɶ� .. 

};


extern ListTime g_ListTime;

#endif // !defined(AFX_LISTTIME_H__8F7F8BE2_A4A8_40B9_8EB8_533AAAA49F28__INCLUDED_)
