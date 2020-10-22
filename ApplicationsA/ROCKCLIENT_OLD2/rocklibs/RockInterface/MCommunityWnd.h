//----------------------------------------------------------------------------------------------------------------------
// 파  일   명 : MCommunityWnd.h
// 용       도 : 친구창(F키)
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
	UINT				m_uiMState;			//메신저 상태
	UINT			    m_uiTempMState;     //메신져 상태임시보관 

	CFrameWnd *			m_pFrameWnd;		//Messenger Frame
	
	CTextBox *			m_pSelfStateTxt;	//자기 상태표시 - 온라인,식사중,휴식중,오프라인		

	CButton	*			m_pCloseBtn;		//클로즈 버튼
	CStateButton *		m_pStateBtn[4];		//각 상태버튼	
	CScrollBar *		m_pScrollBar;		//스크롤바
		
	CButton		*		m_pAddBtn;			//추가버튼
	CButton		*		m_pSubBtn;			//삭제버튼

	int					m_nChatWndXpos;		//채팅윈도우 오픈X위치
	int					m_nChatWndYPos;		//채팅윈도우 오픈Y위치
	
	int					m_nChatWndOriXpos;	//채팅윈도우 원래X위치
	int					m_nChatWndOriYPos;	//채팅윈도우 원래Y위치

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