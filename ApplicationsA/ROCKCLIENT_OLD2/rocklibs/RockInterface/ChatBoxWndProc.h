#if !defined( __RUI_CHATBOX_WNDPROC_H__ )
#define __RUI_CHATBOX_WNDPROC_H__
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

#include "ChatOutput.h"



class CChatBoxWnd : public CWndProc
{
public:
	CChatBoxWnd()
	{
		StateImg	= NULL;
		ChatState	= n_NormarChat;
		SizeState	= 0;
		SAFE_STR_CPY( Receiver, _RT(""), 256 ); 
	}
	~CChatBoxWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	

	CImageBox*	StateImg;
	int			ChatState;
	int			SizeState;

	
	TCHAR		Receiver[256];

};




class CChatOutputWnd : public CWndProc
{
public:
	CChatOutputWnd()
	{
		IsVisible	= false;
		ChatWnd		= NULL;
		ChatTxt		= NULL;
		Fimg_up		= NULL;
	}
	~CChatOutputWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	bool IsVisible;


	CFrameWnd*		ChatWnd;
	CChatOutput*	ChatTxt;
	CImageBox*		Fimg_up;	
};




class CChatMessageWnd : public CWndProc
{
public:
	CChatMessageWnd()
	{
		IsVisible	= false;
		ChatWnd		= NULL;
		ChatTxt		= NULL;
		Step		= 0;
	}
	~CChatMessageWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	long	theOutputTime;
	bool	IsVisible;
	int		Step;

	CFrameWnd*		ChatWnd;
	CChatOutput*	ChatTxt;
};


extern CChatMessageWnd	ChatMessageWnd;
extern CChatBoxWnd		ChatBoxWnd;
extern CChatOutputWnd	ChatOutputWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_CHATBOX_WNDPROC_H__