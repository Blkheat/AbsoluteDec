// ItemAccEnchant.h: interface for the CAccEnchant class.
// ��� �ʴ� ����
//////////////////////////////////////////////////////////////////////

#if !defined(__RUI_PROPOSEJOINFRONTIER_WNDPROC_H__)
#define __RUI_PROPOSEJOINFRONTIER_WNDPROC_H__

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



class CProposeJoinFrontier  : public CWndProc
{
public:
	CProposeJoinFrontier(){}
	~CProposeJoinFrontier(){}
	
	DWORD			m_Eld;
	CTextBox*		m_EldText;
	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	
	//void CloseEnchantWnd();
	void OpenProposeWnd();
	void Close();
	BOOL CheckProposeFrontierItemList();
	void CloseNumberInputWnd( int num_item );

	//bool IsEnchantItemSubJob(CSlot * pSlot);
	//bool IsEnchantEldaSubJob(CSlot * pSlot);

private:
	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	//void EnchantText();
	
	CSlot*			mcp_item_slot;
	CSlot*			temp_src_slot;
	CSlot*			temp_tag_slot;
	//CSlot*			mcp_production_slot;

				

	CButton*		mcp_start_btn;
	CButton*		m_Eldbtn;
	
	SPcItem			mc_itemslot_item;
	SPcItem			mc_production_slot;

	CTextBox*		mcp_effect[3];
	CTextBox*		mcp_Item;		
	
	//
	CButton*		CloseBtn;
	CButton*		StopBtn;


	//bool mb_button_lock;		///-- ��ư Lock ���� ��ư ������ ���� ����
	
	bool mb_craft_weapon;		///-- ���� / �Ƹ� ũ����Ʈ  �Ǵ�.
	int mi_skill_level;			///-- �ش� ��ų�� �ְ� ����
};


extern	CProposeJoinFrontier	g_ProposeJoinFrontier;

#endif __RUI_PROPOSEJOINFRONTIER_WNDPROC_H__
