#if !defined( __RUI_SKILL_ELDA_MIX_WNDPROC_H__ )
#define __RUI_SKILL_ELDA_MIX_WNDPROC_H__
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

#pragma pack(push,1)
struct SEldaItemInfo2
{

	BYTE	Index;
	BYTE	EldaStone[4];
	DWORD	NeedMoney;

	SEldaItemInfo2()
	{
		Index = 0 ;
		EldaStone[0] = 0 ; 
		EldaStone[1] = 0 ; 
		EldaStone[2] = 0 ; 
		EldaStone[3] = 0 ; 
		NeedMoney = 0 ; 
	}
};
#pragma pack(pop)



class CItemSelectEldaMixWndProc : public CWndProc
{
public:
	CItemSelectEldaMixWndProc(){}
	~CItemSelectEldaMixWndProc(){}

	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	
	void Close();
	void Open();
	void Unlock_button(){mb_button_lock = false;};
	bool GetButtonLock(){return mb_button_lock;}
	BOOL SetAddPcItem(CSlot *Slot , int SlotNum , int AddItemNum ,BOOL flag = false);


	BOOL CreateLoadTable();
	

	
	void ClearPreItem();

		
	SPcItem			m_CurPcItem[15];
	CSlot*			m_CurSlot[15];
	int				m_TableEldaCode[11];

	int m_iTableCount ;

	SEldaItemInfo2*		m_EldaItemInfo2;

private:
	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	void EnchantText();

	CSlot*			RusultItem;

	CButton*		start_btn;	
	CButton*		cancel_btn;

	CFrameWnd*		m_pFrameWnd;
	


	bool mb_button_lock;		///-- 버튼 Lock 가공 버튼 여러번 누름 방지
	

	SBundleItem EldaStone[3];
};




extern	CItemSelectEldaMixWndProc	g_ItemSelectEldaMixWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMENCHANT_WNDPROC_H__