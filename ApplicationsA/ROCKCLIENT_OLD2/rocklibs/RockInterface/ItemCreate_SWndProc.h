#if !defined( __RUI_ITEMCREATE_S_WNDPROC_H__ )
#define __RUI_ITEMCREATE_S_WNDPROC_H__ 
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


class CItemCreate_S_WndProc : public CWndProc
{
public:
	CItemCreate_S_WndProc(){}
	~CItemCreate_S_WndProc(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	

	void OpenCreate_S_WndProc(bool bResult, const int ai_ItemUID, 
							  const DWORD ai_exp, int ai_penalty );

private:

	CFrameWnd*		mcp_fw;
	CSlot*			mcp_item_slot;
	SPcItem			mc_itemslot_item;
	
	CTextBox*		mcp_remodel_Name;
	CTextBox*		mcp_remodel_Desc;
	CTextBox*		mcp_exp_text;
	CTextBox*		mcp_exp_penalty;
	CImageBox*		mcp_icon_x;
};



extern	CItemCreate_S_WndProc	g_ItemCreate_S_WndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMCREATE_S_WNDPROC_H__ 
