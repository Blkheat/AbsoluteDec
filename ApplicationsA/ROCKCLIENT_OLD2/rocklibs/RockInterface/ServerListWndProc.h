#if !defined( __RUI_SERVERLIST_WNDPROC_H__ )
#define __RUI_SERVERLIST_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "WndProc.h"

#define SERVER_LIST_SELECT_BTN_COUNT	12


class CButton;
class CFrameWnd;
class CTextBox;
class CImageBox;
class CImageBox9;
class CScrollBar;
class CProgressBar;
class CServerListWnd : public CWndProc
{
public:
 	 
	 CServerListWnd() {}
	~CServerListWnd() {}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );


	void	Open_ServerList_Wnd( );
	void    SelectServerList(int Num);
	void    SetScroll(int Num);

	CButton*		m_ConnectBtn;
	CButton*		m_CancelBtn;

private:

	CFrameWnd*		m_ServerListWnd;
	CTextBox*		mcp_title;
	CTextBox*		mcp_sel_server;
	CImageBox*		mcp_channel_img;
	CImageBox*		mcp_sersel_img;
	CImageBox*		mcp_chasel_img;
	
	CImageBox9*		mcp_black_back9;
	CImageBox9*		mcp_black_server9;	
	

	CScrollBar*	    m_ScrollBtn_svr; ///-- 서버 스크롤 버튼 
	CScrollBar*	    m_ScrollBtn_chn; ///-- 채널 스크롤 버튼
	CButton*		m_BtnList[ SERVER_LIST_SELECT_BTN_COUNT ];		
	CTextBox*		m_Statetext[ SERVER_LIST_SELECT_BTN_COUNT ];		
	
	CButton*		m_BtnServerTab[SERVER_LIST_SELECT_BTN_COUNT];
	int				m_iServerTab;
	int				m_iCurIndex;

	CProgressBar*	m_ServerPB[ SERVER_LIST_SELECT_BTN_COUNT ];	
	
	int				m_TopNum;
	int				m_SelectNum;

		
	void			ConnectProcess(void);
	void			CompositionOld();
	void			CompositionNew();
	int StartX;
	

public:
	bool			mb_server;
	bool	m_bRunServerListName;
	void			CancelProcess(bool bFlag = false);
	void			GetServerName(char *ServerNameStr);

};



extern	CServerListWnd	ServerListWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_SERVERLIST_WNDPROC_H__