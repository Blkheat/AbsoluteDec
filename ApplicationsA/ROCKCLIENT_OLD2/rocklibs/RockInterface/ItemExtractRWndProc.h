#if !defined( __RUI_ITEMEXTRACT_R_WNDPROC_H__ )
#define __RUI_ITEMEXTRACT_R_WNDPROC_H__
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


class CItemExtract_R_WndProc : public CWndProc
{
public:
	CItemExtract_R_WndProc(){}
	~CItemExtract_R_WndProc(){}

	virtual void Init();
	virtual void Composition();	
	virtual void Proc( SEventMessage* EMsg );	
	
	void OpenExtract_R_Wnd( const int ai_ItemUID );
private:
	
	CSlot*			mcp_elda_slot;
	SPcItem			mc_eldaslot_item;
	CTextBox*		mcp_extract_Desc;
};



extern	CItemExtract_R_WndProc	g_ItemExtract_R_WndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMEXTRACT_R_WNDPROC_H__