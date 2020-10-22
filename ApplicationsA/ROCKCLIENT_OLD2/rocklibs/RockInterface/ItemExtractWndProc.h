#if !defined( __RUI_ITEMEXTRACT_WNDPROC_H__ )
#define __RUI_ITEMEXTRACT_WNDPROC_H__
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


class CItemExtract_WndProc : public CWndProc
{
public:
	CItemExtract_WndProc(){}
	~CItemExtract_WndProc(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	///virtual void SetWheelValue( short _wheel );
	void OpenExtract_Wnd();
	void CloseExtract_Wnd();
	BOOL CheckExtract_ItemList();
	void Unlock_button(){mb_button_lock = false;};
	
private:
	CSlot*			mcp_item_slot;
	CSlot*			mcp_elda_slot;
	
	SPcItem			mc_itemslot_item;
	SPcItem			mc_eldaslot_item;

	CButton*		mcp_extract_btn;	
	CTextBox*		mcp_extract_Desc;
	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	
	bool mb_button_lock;		///-- 버튼 Lock 가공 버튼 여러번 누름 방지
	

};



extern	CItemExtract_WndProc	g_ItemExtract_WndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMEXTRACT_WNDPROC_H__