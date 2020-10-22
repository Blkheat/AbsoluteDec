#if !defined( __RUI_ITEMREPAIR_WNDPROC_H__ )
#define __RUI_ITEMREPAIR_WNDPROC_H__
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


// 유료아이템중 고칠수 없는 아이템 범위 (대신 내구도가 달아도 공격력 및 방어력은 떨어지지 않는다.) 
const int c_NotRepair_PayItemCode_Min( 12301 );
const int c_NotRepair_PayItemCode_Max( 12600 );


class CItemRepair_WndProc : public CWndProc
{
public:
	CItemRepair_WndProc(){}
	~CItemRepair_WndProc(){}

	virtual void Init();
	virtual void Composition();	
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );
	void SetSlotScroll( int linenum );
	void OpenRepair_Wnd( const bool ab_npc_repair );
	void CloseExtract_Wnd();
	void Select_All();
	void Gen_Cost();
	BOOL CheckRepair_ItemList();
	void Unlock_button(){mb_button_lock = false;};
	void Refresh( DWORD ad_uid );
	void Clear();

public:
	CFrameWnd*		FrameWnd;
	
		
private:
	int				ml_cost;
	int				ml_count;
	CSlot*			mpc_slots[4][5];	
	SPcItem			mpc_items[14][5];


	//CComboBox*		m_pMapComboBox;
	CScrollBar*		mpc_scrollbar;
	CTextBox*		mcp_cost_text;

	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	bool			mb_npc_repair;		///-- true : NPC 수리 false: skill / skill 은 수리비가 50%
	int mi_skill_level;
	int mi_skill_max_code;
	bool mb_button_lock;		///-- 버튼 Lock 수리 버튼 여러번 누름 방지
	
	
	

};



extern	CItemRepair_WndProc	g_ItemRepair_WndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMREPAIR_WNDPROC_H__