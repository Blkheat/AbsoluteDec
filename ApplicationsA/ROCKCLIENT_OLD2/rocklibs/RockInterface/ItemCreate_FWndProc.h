#if !defined( __RUI_ITEMCREATE_F_WNDPROC_H__ )
#define __RUI_ITEMCREATE_F_WNDPROC_H__ 
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


class CItemCreate_F_WndProc : public CWndProc
{
public:
	CItemCreate_F_WndProc (){}
	~CItemCreate_F_WndProc (){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	///virtual void SetWheelValue( short _wheel );

	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	
	void CloseEnchantWnd();
	void OpenCreate_F_WndProc( const SBundleItem * const aDelitems, const int ai_exp, int ai_penalty );

	BOOL CheckEnchantItemList();
private:
	CSlot*			mcp_item_slot[6];
	SPcItem			mc_itemslot_item[6];
	CTextBox*		mcp_remodel_Desc[6];
	CImageBox*		mcp_icon_x[6];
	
};



extern	CItemCreate_F_WndProc 	g_ItemCreate_F_WndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMCREATE_F_WNDPROC_H__ 