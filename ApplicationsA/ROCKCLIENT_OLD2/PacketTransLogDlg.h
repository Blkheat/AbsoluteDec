// PacketTransLogDlg.h: 
//
//////////////////////////////////////////////////////////////////////

#if !defined(__PACKETTRANSLOGDLG_H_INCLUDED__)
#define __PACKETTRANSLOGDLG_H_INCLUDED__

#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	DLG_PACTLOG_NAME		"Packet Trans Log"
#define	DLG_PACTLOG_WIDTH		 600

#define IDC_PACTLOG_LIST_LOG	 0x0001
#define IDC_PACTLOG_ACT_BTN		 0x0002
#define IDC_PACTLOG_DLIST_LOG	 0x0003
#define	IDC_PACTLOG_PACT_BTN	 0x0004
#define IDC_PACTLOG_DACT_BTN	 0x0005
#define IDC_PACTLOG_SAVE_BTN	 0x0006			

#ifdef _DEBUG
#define	PACKETLOG_CLIENT 
#endif

#define PRINT_DLG_LOG g_RockClient.thePacketTransLogDlg.PrintDebugLog
#define SAVE_DLG_LOG g_RockClient.thePacketTransLogDlg.SaveLogFile

struct PACKETTYPE {
    
	int  iPaketType;  
	char strPacketName[256];
};

enum NLPType
{
	NPacketPrint = 0,
	NDebugPrint,
};

class CPacketTransLogDlg
{
private:
    
	CRITICAL_SECTION	csDebugFlag;
	
	bool			m_IsPrintLog;					//로그출력여부
	int				m_nPrintType;
	
	HINSTANCE		m_hParentInstance;				//부모 인스턴스
	HWND			m_hParentWnd;					//부모 윈도우의 핸들
    HWND			m_hMainDlg;						//메인 Dialog 핸들
	
	
	HBRUSH			m_hMainDlgBgBrush;				//메인 Dialog 배경 브러쉬	
    HFONT			m_hFont;						//폰트
    RECT			m_MainDlgRect;					//메인 Dialog 영역
	
	HWND			m_hEditLog;                     //로그 에디트 핸들
	HWND			m_hDebLog;						//디버그 로그 에디트 핸들	
	
	HWND			m_hHiddenWnd;					//숨김 윈도우 핸들

	HWND			m_hActiveBtn;					//로그출력 동작버튼

	HWND			m_hPacketBtn;					//패킷로그출력 동작버튼
	HWND			m_hDlogBtn;						//디버그로그출력 동작버튼

	HWND			m_hLogSaveBtn;					//로그 파일저장 버튼

	char *			m_pStrEdit;
	int				m_nMaxStrLength;				//문자열 최대길이
	
public:
	bool			m_IsShow;
	
	CPacketTransLogDlg();
	virtual ~CPacketTransLogDlg();
	
	void	CreateDlg( HINSTANCE hParentInst, HWND hParentWnd );
	void	DestroyDlg();

	void	ActiveDlg();
	LRESULT DlgMsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	HWND	GetLogHandle()	{ return m_hEditLog; }
	
	void    InsertListString( char *buf, ... );
	void    PrintPacketTransInfo(void * pPacket, BOOL IsRecv);
	
    void    PrintHighStoCPacketInfo(void * pPacket,
		char * pstrTimeInfo, char * pstrDirInfo);
	void    PrintStoCPacketInfo(void * pPacket,
		char * pstrTimeInfo, char * pstrDirInfo);
	void	PrintCtoSPacketInfo(void * pPacket,
		char * pstrTimeInfo, char * pstrDirInfo);
	
	void	Open();
	void	Close();

	void	PrintDebugLog(char *clog, ... );
	void	SaveLogFile();
	
	BOOL	IsActivePDlg()			
	{ 
		if(m_hMainDlg)
		{
			return TRUE;
		}
		
		return FALSE; 
	}
};

#endif

