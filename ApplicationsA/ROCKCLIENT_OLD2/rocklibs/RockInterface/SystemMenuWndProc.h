#if !defined( __RUI_SYSTEMMENU_WNDPROC_H__ )
#define __RUI_SYSTEMMENU_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "WndProc.h"


class CSystemMenuWnd : public CWndProc
{
	CTextBox *textbox;

	CFrameWnd* m_pFramwWnd;
	
	CButton * m_pGoOutBtn;
	CButton * m_pLogOffBtn;		
	CButton * m_pGameExitBtn;
	CButton * m_pScreenShotBtn;
	CButton * m_pOptionBtn;
	CButton * m_CloseBtn;

public:
	CSystemMenuWnd(){}	
	~CSystemMenuWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void Open(BOOL bIsNormal);
	void Close();
		
	void GameExitBtn();
	void GameLogOff();	

};


extern	CSystemMenuWnd	SystemMenuWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_SYSTEMMENU_WNDPROC_H__
