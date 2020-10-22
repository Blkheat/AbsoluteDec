#if !defined( __RUI_StrengthStone_WNDPROC_H__ )
#define __RUI_StrengthStone_WNDPROC_H__
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


class CStrengthStoneWndProc : public CWndProc
{
public:
	CStrengthStoneWndProc();
	~CStrengthStoneWndProc();
	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	

	void CloseEnchantWnd();
	void OpenEnchantWnd();
	void Unlock_button(){mb_button_lock = false;};

	BOOL CheckEnchantItemList();

	bool IsEnchantItemSubJob(CSlot * pSlot);
	bool IsEnchantEldaSubJob(CSlot * pSlot);

	CFrameWnd*		m_pWholeFrame;							///-- 전체 프레임		
private:
	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	void EnchantText();

	
	CFrameWnd*		mpC_framewnd;

	CSlot*			mcp_item_slot;
	CSlot*			mcp_SpellStone_slot;
//	CSlot*			mcp_ResultSlot;
	CImageBox*		mcp_ResultImg;

	CButton*		mcp_start_btn;	
	
	SPcItem			mc_itemslot_item;
	SPcItem			mc_SpellStone_item;
//	SPcItem			mc_Result_item;

	CTextBox*		mcp_Level;
	CTextBox*		mcp_Item;		
	CTextBox*		mcp_textResult;
	
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

	CButton*		m_CloseBtn;
	CButton*		m_StopBtn;

	CTextBox*		m_MoneyText;
public:
	void OpenCharge_Wnd();
	void CloseCharge_Wnd();

};



extern	CStrengthStoneWndProc	*g_StrengthStoneWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_StrengthStone_WNDPROC_H__