//----------------------------------------------------------------------------------------------------------------------
// ��  ��   �� : MCommunityWnd.h
// ��       �� : ģ��â(FŰ)
//----------------------------------------------------------------------------------------------------------------------
#ifndef __MESSENGERWND_H__
#define __MESSENGERWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"

#include "MFriendTabWnd.h"
#include "MGuildTabWnd.h"
#include "MBlockTabWnd.h"
#include "MSettingTabWnd.h"

#include "MMsgWnd.h"
#include "MPopupMenuWnd.h"

#include "MessenOneChatWnd.h"
#include "MessenNotifyWnd.h"

#include "MessenMultiChatWnd.h"

#define MAX_MESSEN_ONECHATWND_NUM		32
#define MAX_MESSEN_NOTIFYWND_NUM		8
#define MAX_MESSEN_NOTIFYDESTNAME_NUM	32						

enum N_CumunityState
{
	n_MS_None = 0,
	n_MS_Friend,
	n_MS_Guild,
	n_MS_Block,	
	n_MS_Setting	
};

class CStateButton;
class CMCommunityWnd : public CWndProc
{
protected:
	UINT				m_uiMState;			//�޽��� ����
	UINT			    m_uiTempMState;     //�޽��� �����ӽú��� 

	CFrameWnd *			m_pFrameWnd;		//Messenger Frame
	
	CTextBox *			m_pSelfStateTxt;	//�ڱ� ����ǥ�� - �¶���,�Ļ���,�޽���,��������		

	CButton	*			m_pCloseBtn;		//Ŭ���� ��ư
	CStateButton *		m_pStateBtn[4];		//�� ���¹�ư	
	CScrollBar *		m_pScrollBar;		//��ũ�ѹ�
		
	CButton		*		m_pAddBtn;			//�߰���ư
	CButton		*		m_pSubBtn;			//������ư

	int					m_nChatWndXpos;		//ä�������� ����X��ġ
	int					m_nChatWndYPos;		//ä�������� ����Y��ġ
	
	int					m_nChatWndOriXpos;	//ä�������� ����X��ġ
	int					m_nChatWndOriYPos;	//ä�������� ����Y��ġ

public:
	CMCommunityWnd();
	~CMCommunityWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

public:
	CMFriendTabWnd		m_MFriendTabWnd;
	CMGuildTabWnd		m_MGuildTabWnd;
	CMBlockTabWnd		m_MBlockTabWnd;
	CMSettingTabWnd		m_MSettingTabWnd;
	CMMsgWnd			m_MMsgWnd;
	CMPopupMenuWnd		m_MPopupMenuWnd;

	CMessenOneChatWnd	m_MessenOneChatWnd[MAX_MESSEN_ONECHATWND_NUM];
	CMessenNotifyWnd	m_MessenNotifyRefWnd;	
	
	POINT				m_MessenNotifyWndPos[MAX_MESSEN_NOTIFYWND_NUM + 1];		
	CMessenNotifyWnd	m_MessenNotifyWnd[MAX_MESSEN_NOTIFYWND_NUM];
	int					m_nViewNotifyWndNum;
	
	TCHAR*				m_MessenNotifyDestName[MAX_MESSEN_NOTIFYDESTNAME_NUM + 1];
	int					m_nDestNameNum;

	CMessenMultiChatWnd m_MessenMultiChatWnd;

public:
	void SetMessengerState( UINT uiState , bool reDraw);
	void SetMessengerListScroll( int _linenum );

	void AddBtnProc();
	void SubBtnProc();

	void OpenMessenOneChatWnd(TCHAR * strParam);
	void GetMessenOneChatWndPos(POINT & pos);
	
	CMessenOneChatWnd * FindMessenOneChatWnd(void * strParam);
	void ActionInsertMessenNotifyWnd();

	int	 GetViewNotifyWndNum();
	void SetViewNotifyWndNum(int nNum);
	int  GetDestNameNum();
	void SetDestNameNum(int nNum);

	void SetMessenState(UINT uMessenState);
	void SetVisible(bool bFlag);
	bool SetMessenChatWndWheelValue(SMouse & theMouse);
	
	void InsertOneChatContent(TCHAR * pStrName, TCHAR * pStrContent);
	void InsertMultiChatContent(bool IsSystem, UINT uRed, UINT uGreen, UINT uBlue, 
								TCHAR * pStrName, TCHAR * pStrContent);

	void LogOff();
};

extern CMCommunityWnd g_MCommunityWnd;

//-----------------------------------------------------------------------------
#endif	__MESSENGERWND_H__