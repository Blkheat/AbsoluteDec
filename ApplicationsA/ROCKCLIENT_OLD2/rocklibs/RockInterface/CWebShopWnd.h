// 유료아이템 창
#ifndef __WEBSHOPWND_H__
#define __WEBSHOPWND_H__
//-----------------------------------------------------------------------------

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "StateButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Slot.h"
#include "Wnd.h"
#include "WndProc.h"
#include "..\\..\\CWebDialog.h"
#include "..\\..\\md5.h"

class CWebShopWnd : public CWndProc
{

public:
	 
	 CWebShopWnd();
	~CWebShopWnd();

public:

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	
	int  OpenURL(wchar_t *addr, TCHAR *post);
	int  Close(void);
	void Present(void);
	void SetWebMode(int mode);
	void LogOff();

public:

	CFrameWnd*		m_pCullFrame;
	CFrameWnd*		m_pFrame;
	CButton*		m_pCloseBtn;

	char			m_md5[ 33 ];

public:

	CWebDialog		m_WebDialog;

private:

	RECT			m_rcTop;	   
	RECT			m_rcBottom;
	RECT			m_rcLeft;    
	RECT			m_rcRight;   

	SRect			m_WndRect;
	
	//by simwoosung
	bool			m_bFlag;	
	//by dongs
	int				m_iWebMode;  // 0 : 아이템몰 , 1: 게시판 
	int				m_iWebBuildMode;

	


};


extern	CWebShopWnd	g_WebShopWnd;

//-----------------------------------------------------------------------------
#endif	__WEBSHOPWND_H__