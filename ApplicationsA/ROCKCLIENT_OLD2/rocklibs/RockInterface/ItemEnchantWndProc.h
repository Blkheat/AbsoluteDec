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

	CFrameWnd*		m_pWholeFrame;							///-- ��ü ������			
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

public:
	void OpenCharge_Wnd();
	void CloseCharge_Wnd();

};



extern	CItemEnchantWndProc	g_ItemEnchantWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMENCHANT_WNDPROC_H__