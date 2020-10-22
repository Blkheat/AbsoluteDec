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

	CFrameWnd*		m_pWholeFrame;							///-- ��ü ������		
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
	
	bool mb_button_lock;		///-- ��ư Lock ���� ��ư ������ ���� ����
	
	bool mb_craft_weapon;		///-- ���� / �Ƹ� ũ����Ʈ  �Ǵ�.
	int mi_skill_level;			///-- �ش� ��ų�� �ְ� ����	

	///----------------------------------------------------------------------------------
	/// ���� ������ ���� â
	///----------------------------------------------------------------------------------
	
	CButton*		mcp_chargeWindow_btn;			///-- ���� ������ â ��ư
	BOOL			m_IsViewchargeWindow;			///-- ���� ������ â �����ֱ� ���� 	
	CFrameWnd*		mpCharge_framewnd;				///-- ���� ������ â
	
	///-- ���� ������ ��� 
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