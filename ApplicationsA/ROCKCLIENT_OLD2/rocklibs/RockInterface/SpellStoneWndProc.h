#if !defined( __RUI_SpellStoneWndProc_WNDPROC_H__ )
#define __RUI_SpellStoneWndProc_WNDPROC_H__
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

const int MAKESPELLSTONEMAX = 5;


class CSpellStoneWndProc : public CWndProc
{
public:
	CSpellStoneWndProc();
	~CSpellStoneWndProc(){}
	
	 

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	
	void Close();
	void Open();

	void Unlock_button(){mb_button_lock = false;};
	bool GetButtonLock(){return mb_button_lock;}
	
	BOOL SetClickItem(SPcItem* PcItem);

	//아이템을 미리 보여준다.
	void ClearPreItem();

	SPcItem			m_CurPcItem[MAKESPELLSTONEMAX];
	SPcItem			m_PrePcItem[MAKESPELLSTONEMAX+1];
	CSlot*			m_CurSlot[MAKESPELLSTONEMAX];
	CSlot*			m_PreSlot[MAKESPELLSTONEMAX+1];
	CFrameWnd*		m_pFrameWnd;
private:
	BOOL CheckItemList();
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );
	void CheckItemAdd(SPcItem* PrePcItem ,SPcItem* CurPcItem,SPcItem* SrcPcItem , SPcItem *TarGetPcItem);

	CImageBox*		m_PreItemImg[4];

	CButton*		start_btn;	
	CButton*		cancel_btn;

	CTextBox*		m_MoneyText;

	bool mb_button_lock;		///-- 버튼 Lock 가공 버튼 여러번 누름 방지
	SBundleItem Potion[MAKESPELLSTONEMAX];	
};




extern	CSpellStoneWndProc*	g_SpellStoneWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_SpellStoneWndProc_WNDPROC_H__