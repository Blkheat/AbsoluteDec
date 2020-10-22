#ifndef __SYSTEMPRINTWND_H__
#define __SYSTEMPRINTWND_H__
//-----------------------------------------------------------------------------

#include "TextOutputBase.h"
#include "WndProc.h"

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
#include "Wnd.h"
#include "CTextOutList.h"

class CSystemPrintWnd : public CWndProc
{
protected:
	SSystemTxtData		  m_TempSystemData;
	SSystemTxtData		  m_SystemTxtData[MAX_SYSTEMDATA_NUM + 1];
	int					  m_nInsertIndex;	
	
	SPoint2				  m_nLeftDownPos;		//기준이 되는 왼쪽아래위치
	CFrameWnd *			  m_pSystemBgWnd;
	CSystemOutList *	  m_pSystemOutList;
	CWnd*				  m_pSystemScrollBarBg;	
	CScrollBar*			  m_pSystemScrollBar;

	TCHAR				  m_strTempBuf1[1024];
	TCHAR				  m_strTempBuf2[512];
	
	bool				  m_bEnableWeel;
	BOOL				  m_bFocusSysWnd;
	BOOL				  m_bViewScroll;	
	float				  m_fCumTime;		//시간누적값


public:
	CSystemPrintWnd();
	virtual ~CSystemPrintWnd();

	virtual void Init();
    virtual void Composition();
    virtual void Update();
    virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );
	bool SetWheelValue(SMouse & theMouse);
	
	void UpdateSystemTxtData();
	
	void ResetSizeFrame();
	void InsertSystemMsg(void *strMsg, UINT uSysColor);
	void InsertSystemData();	
	void SetListScroll( int _linenum );

	void LogOff();
	
	CFrameWnd* GetFrameWnd()  { return m_pSystemBgWnd;	}
	void ResetScroll();
};

extern CSystemPrintWnd g_SystemPrintWnd;

//-----------------------------------------------------------------------------
#endif	__SYSTEMPRINTWND_H__