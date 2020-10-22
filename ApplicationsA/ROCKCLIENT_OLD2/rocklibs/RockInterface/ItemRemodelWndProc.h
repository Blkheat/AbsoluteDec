#if !defined( __RUI_ITEMREMODEL_WNDPROC_H__ )
#define __RUI_ITEMREMODEL_WNDPROC_H__ 
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


class CItemRemodel_WndProc : public CWndProc
{
public:
	CItemRemodel_WndProc();  
	~CItemRemodel_WndProc(); 

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
   
	void OpenRemodel_RWnd();
	void CheckRemodelEnable();
	BOOL LoadRemodelTable();
	BOOL CheckRemodel_ItemList();

	
	void CloseNumberInputWnd( int aNumItem );
	void Unlock_button(){mb_button_lock = false;};
	
	int	 GetTableCount(void)		{ return mi_table_count; }	// 테이블 개수 리턴 By wxywxy

	
private:
	
	void GetProbability();
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );

	SPcItem			mc_itemslot_item[3];
	SPcItem			mc_eldaslot_item;

	CSlot*			mcp_item_slot[3];
	CSlot*			mcp_elda_slot;
	CSlot*			mcp_temp_num_src_slot;
	CSlot*			mcp_temp_num_tga_slot;
	CTextBox*		mcp_remodel_item;
	CTextBox*		mcp_remodel_Prob;
	
	CButton*		mcp_remodel_btn;	

	
	int mi_skill_level;
	bool mb_loadRemodelTable;
	int mi_table_count;
	bool mb_button_lock;		///-- 버튼 Lock 가공 버튼 여러번 누름 방지
};



extern	CItemRemodel_WndProc	g_ItemRemodel_WndProc;
extern SRemodelStone * g_RemodelTable;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMREMODEL_WNDPROC_H__ 