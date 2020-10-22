#if !defined( __RUI_ITEMENCHANTR_WNDPROC_H__ )
#define __RUI_ITEMENCHANTR_WNDPROC_H__
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


class CItemEnchantRWndProc : public CWndProc
{
public:
	CItemEnchantRWndProc(){}
	~CItemEnchantRWndProc(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	void OpenEnchantR_SWnd( DWORD ai_ItemUID  , DWORD ai_eldaID , DWORD ai_exp, int ai_penalty );
	void OpenEnchantR_FWnd( DWORD ai_DelStoneID , DWORD ai_exp, int ai_penalty );

	void EnchantText( SPcItem * ap_Item );
	CSlot*			mcp_item_slot;
	SPcItem			mc_itemslot_item;
	CImageBox*		mcp_icon_x;
		
	CTextBox*		mcp_exp;
	CTextBox*		mcp_exp_penalty;
	CTextBox*		mcp_effect[3];
	CTextBox*		mcp_result_text;

};



extern	CItemEnchantRWndProc	g_ItemEnchant_RWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMENCHANTR_WNDPROC_H__