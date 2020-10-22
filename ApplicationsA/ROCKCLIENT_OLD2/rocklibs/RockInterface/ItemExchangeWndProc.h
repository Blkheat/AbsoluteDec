#if !defined( __RUI_ITEMEXCHANGE_WNDPROC_H__ )
#define __RUI_ITEMEXCHANGE_WNDPROC_H__
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
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Slot.h"
#include "Wnd.h"


#include "WndProc.h"


class CItemExchangeWnd : public CWndProc
{
public:
	CItemExchangeWnd(){}
	~CItemExchangeWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

};



extern	CItemExchangeWnd	ItemExchangeWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMEXCHANGE_WNDPROC_H__