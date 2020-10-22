// ItemAccEnchant.h: interface for the CAccEnchant class.
// �Ǽ����� ��æƮ ... 
// by Dongs
//////////////////////////////////////////////////////////////////////

#if !defined(__RUI_ITEMACCENCHANT_WNDPROC_H__)
#define __RUI_ITEMACCENCHANT_WNDPROC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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



class CItemAccEnchant  : public CWndProc
{
public:
	CItemAccEnchant(){}
	~CItemAccEnchant(){}

	
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

private:
	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	void EnchantText();

	CSlot*			mcp_item_slot;
	CSlot*			mcp_production_slot;

				

	CButton*		mcp_start_btn;	
	
	SPcItem			mc_itemslot_item;
	SPcItem			mc_production_slot;

	CTextBox*		mcp_effect[3];
	CTextBox*		mcp_Item;		
	
	//
	CButton*		CloseBtn;
	CButton*		StopBtn;



	bool mb_button_lock;		///-- ��ư Lock ���� ��ư ������ ���� ����
	
	bool mb_craft_weapon;		///-- ���� / �Ƹ� ũ����Ʈ  �Ǵ�.
	int mi_skill_level;			///-- �ش� ��ų�� �ְ� ����
};


extern	CItemAccEnchant	g_ItemAccEnchant;

#endif __RUI_ITEMACCENCHANT_WNDPROC_H__
