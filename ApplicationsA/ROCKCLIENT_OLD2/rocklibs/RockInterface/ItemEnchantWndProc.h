#if !defined( __RUI_ITEMENCHANT_WNDPROC_H__ )
#define __RUI_ITEMENCHANT_WNDPROC_H__
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


class CItemEnchantWndProc : public CWndProc
{
public:
	CItemEnchantWndProc();
	~CItemEnchantWndProc();
	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	
	void ComposionOld();
	void ComposionNew();
	


	void CloseEnchantWnd();
	void OpenEnchantWnd();
	void Unlock_button(){mb_button_lock = false;};
	void SetSkillLevel();

	BOOL CheckEnchantItemList();

	bool IsEnchantItemSubJob(CSlot * pSlot);
	bool IsEnchantEldaSubJob(CSlot * pSlot);
	bool IsEnchantItemSubJobNew(CSlot * pSlot);
	bool IsEnchantEldaSubJobNew(CSlot * pSlot);

	CFrameWnd*		m_pWholeFrame;							///-- 전체 프레임			
private:
	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	void EnchantText();


	CFrameWnd*		mpC_framewnd;

	CSlot*			mcp_item_slot;
	CSlot*			mcp_elda_slot;
	CButton*		mcp_start_btn;	
	
	SPcItem			mc_itemslot_item;
	SPcItem			mc_eldaslot_item;

	CTextBox*		mcp_Level;
	CTextBox*		mcp_effect[7];
	CTextBox*		mcp_Item;		

	
	bool mb_button_lock;		///-- 버튼 Lock 가공 버튼 여러번 누름 방지
	
	bool mb_craft_weapon;		///-- 웨폰 / 아머 크래프트  판단.
	int mi_skill_level;			///-- 해당 스킬의 최고 레벨	

	///----------------------------------------------------------------------------------
	/// 유료 아이템 관련 창
	///----------------------------------------------------------------------------------
	
	CButton*		mcp_chargeWindow_btn;			///-- 유료 윈도우 창 버튼
	BOOL			m_IsViewchargeWindow;			///-- 유료 아이템 창 보여주기 여부 	
	CFrameWnd*		mpCharge_framewnd;				///-- 유료 윈도우 창
	
	///-- 유료 아이템 등록 
	SPcItem			mc_Chargeslot_item[3];
	CSlot*			mcp_Chargeitem_slot[3];

	DWORD	m_dPayItem[3];

public:
	void OpenCharge_Wnd();
	void CloseCharge_Wnd();

};



extern	CItemEnchantWndProc	g_ItemEnchantWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMENCHANT_WNDPROC_H__