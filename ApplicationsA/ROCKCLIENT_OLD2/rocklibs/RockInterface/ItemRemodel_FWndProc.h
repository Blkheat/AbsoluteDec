#if !defined( __RUI_ITEMRESULT_F_WNDPROC_H__ )
#define __RUI_ITEMRESULT_F_WNDPROC_H__
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


class CItemResult_F_WndProc : public CWndProc
{
public:
	CItemResult_F_WndProc (){}
	~CItemResult_F_WndProc (){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	///virtual void SetWheelValue( short _wheel );

	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	
	void CloseEnchantWnd();
	void OpenRemodel_F_WndProc( const SBundleItem * const aDelitems );

	BOOL CheckEnchantItemList();
private:
	CSlot*			mcp_item_slot[3];
	SPcItem			mc_itemslot_item[3];
	CTextBox*		mcp_remodel_Desc[3];
	CImageBox*		mcp_icon_x[3];

	
};



extern	CItemResult_F_WndProc 	g_ItemRemodel_F_WndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMRESULT_F_WNDPROC_H__