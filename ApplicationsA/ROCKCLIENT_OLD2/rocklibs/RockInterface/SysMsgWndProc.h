#if !defined( __RUI_SYSMSG_WNDPROC_H__ )
#define __RUI_SYSMSG_WNDPROC_H__
//-----------------------------------------------------------------------------
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
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"

#include "SysMsgOutput.h"



class CSysMsgOutputWnd : public CWndProc
{
public:
	CSysMsgOutputWnd()
	{
		IsVisible	= false;
		SysMsgWnd	= NULL;
		SysMsgTxt	= NULL;
		Step		= 0;
	}
	~CSysMsgOutputWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	bool	IsVisible;
	int		Step;

	CFrameWnd*		SysMsgWnd;
	CSysMsgOutput*	SysMsgTxt;
	CSysMsgOutput*	AdminMsgTxt;
	CSysMsgOutput*	MapNameMsgTxt;

	void ReSetMsgTxt();
};


extern CSysMsgOutputWnd	SysMsgOutputWnd;


//-----------------------------------------------------------------------------
#endif  __RUI_SYSMSG_WNDPROC_H__

