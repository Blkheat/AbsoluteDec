#if !defined( __RUI_ITEMREMODEL_S_WNDPROC_H__ )
#define __RUI_ITEMREMODEL_S_WNDPROC_H__
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


class CItemRemodel_S_WndProc : public CWndProc
{
public:
	CItemRemodel_S_WndProc(){}
	~CItemRemodel_S_WndProc(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	

	void OpenRemodel_S_WndProc( const int ai_ItemUID );

private:

	CSlot*			mcp_item_slot;
	SPcItem			mc_itemslot_item;
	CTextBox*		mcp_remodel_Desc;
};



extern	CItemRemodel_S_WndProc	g_ItemRemodel_S_WndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMREMODEL_S_WNDPROC_H__
