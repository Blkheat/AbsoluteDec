#ifndef __FRONTIERCREATEWND_H__
#define __FRONTIERCREATEWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"

#ifdef DECO_RENEWAL_GUILD_MJH
#include "FrontierMainWndProc.h"

#define CREATE_MEMBER_MAX_COUNT				10

#endif // #ifdef DECO_RENEWAL_GUILD_MJH

class CFrontierCreateWnd : public CWndProc
{
protected:
	CFrameWnd*		m_pFrameWnd;
	
	CEditBox*		m_pPNameEdit;			///-- ����Ƽ�� �̸�
	CEditBox*		m_pPCMemeberEdit[3];	///-- â�� ��� 3��
	
	CButton*		m_pCloseBtn;			///-- �ݱ� ��ư
	CButton*		m_pConfirmBtn;			///-- Ȯ�� ��ư
	CButton*		m_pCancleBtn;			///-- ��� ��ư

public:
	CFrontierCreateWnd();
	~CFrontierCreateWnd();

	virtual void Init();
	virtual void Composition();	
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	
	BOOL ConfirmInputContent();
	void Open();
	void Close();
};
#ifdef DECO_RENEWAL_GUILD_MJH

class CFrontierCreateScrollWnd : public CWndProc
{
protected:
	DWORD m_CurTime;
	DWORD m_NowTime;
	DWORD m_Cumulative;
	DWORD m_MilliSec;
	

	CFrameWnd*		m_pFrameWnd;
	CTextBox*	    m_pNamebox[10];         ///-- ��� �ο� �ؽ�Ʈ
	CButton*		m_pCloseBtn;			///-- �ݱ� ��ư
	CTextBox*	    m_pCreateTime;          ///-- â�� ���� �ð�

	
	
public:
	CFrontierCreateScrollWnd();
	~CFrontierCreateScrollWnd();

	virtual void Init();
	virtual void Composition();	
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:

	time_t          m_FrontierLimitTime; //��� �������ѽð� 

	void Open();
	void Close();
	void FrontierMemberInfoList();
	void FrontierLimitTime();
	int  GetSec();

};

extern CFrontierCreateScrollWnd g_FrontierCreateScrollWnd;

#endif // DECO_RENEWAL_GUILD_MJH

extern CFrontierCreateWnd g_FrontierCreateWnd;

//-----------------------------------------------------------------------------
#endif	__FRONTIERCREATEWND_H__