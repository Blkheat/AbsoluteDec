#if !defined( __RUI_ELDADUST_WNDPROC_H__ )
#define __RUI_ELDADUST_WNDPROC_H__
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

#include "ItemSelectPotionWndProc.h"
#include "WndProc.h"


class CItemEldaDustWndProc : public CWndProc
{
public:
	CItemEldaDustWndProc();
	~CItemEldaDustWndProc();
	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	
	void Close();
	void Open();
	void LogOff();
	void Unlock_button(){mb_button_lock = false;};
	BOOL CheckEnchantItemList();


	CFrameWnd*		m_FrameWnd;
	

private:
	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );

	int CheckItem(int iCode , int iBundle );
	void PrevSetPcItem(CSlot *Slot , int Code , int ItemNum , bool SlotEnable);

	

	CSlot*			m_EldaDustSlot;
	CSlot*			m_ProimaSlot;
	CSlot*			m_PrevProimaSlot;
	CButton*		m_StartBtn;	
	CButton*		m_StopBtn;
	CButton*		m_ExitBtn;
	
	SPcItem			m_EldaDust_item;
	SPcItem			m_Proima_item;	
	SPcItem			m_PrevProimaItem;

	
	CTextBox*		m_NeedEldTxt;
	CTextBox*		mcp_Item;		
	

	int m_iTableCount;
	

	int CurIndexInfo ; //데아터 위치를 기억한다 .  

	vector <SSkillPotionTable>	   m_ItemData;

	bool mb_button_lock;		///-- 버튼 Lock 가공 버튼 여러번 누름 방지
	

};


extern	CItemEldaDustWndProc	g_ItemEldaDustWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ELDADUST_WNDPROC_H__