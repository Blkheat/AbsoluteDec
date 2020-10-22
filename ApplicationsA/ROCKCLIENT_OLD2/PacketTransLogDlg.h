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
	
	bool			m_IsPrintLog;					//�α���¿���
	int				m_nPrintType;
	
	HINSTANCE		m_hParentInstance;				//�θ� �ν��Ͻ�
	HWND			m_hParentWnd;					//�θ� �������� �ڵ�
    HWND			m_hMainDlg;						//���� Dialog �ڵ�
	
	
	HBRUSH			m_hMainDlgBgBrush;				//���� Dialog ��� �귯��	
    HFONT			m_hFont;						//��Ʈ
    RECT			m_MainDlgRect;					//���� Dialog ����
	
	HWND			m_hEditLog;                     //�α� ����Ʈ �ڵ�
	HWND			m_hDebLog;						//����� �α� ����Ʈ �ڵ�	
	
	HWND			m_hHiddenWnd;					//���� ������ �ڵ�

	HWND			m_hActiveBtn;					//�α���� ���۹�ư

	HWND			m_hPacketBtn;					//��Ŷ�α���� ���۹�ư
	HWND			m_hDlogBtn;						//����׷α���� ���۹�ư

	HWND			m_hLogSaveBtn;					//�α� �������� ��ư

	char *			m_pStrEdit;
	int				m_nMaxStrLength;				//���ڿ� �ִ����
	
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

