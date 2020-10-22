#if !defined( __RUI_SKILLPORTION_WNDPROC_H__ )
#define __RUI_SKILLPORTION_WNDPROC_H__
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


class CItemSkillPotionWndProc : public CWndProc
{
public:
	CItemSkillPotionWndProc();
	~CItemSkillPotionWndProc(){}
	
	 

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	
	
	void Close();
	void Open();

	void Unlock_button(){mb_button_lock = false;};
	bool GetButtonLock(){return mb_button_lock;}
	
	BOOL SetClickItem(SPcItem* PcItem);

	//�������� �̸� �����ش�.
	void ClearPreItem();

	SPcItem			m_CurPcItem[MAKESKILLPOTIONNUM];
	SPcItem			m_PrePcItem[4];
	CSlot*			m_CurSlot[MAKESKILLPOTIONNUM];
	CSlot*			m_PreSlot[4];
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

	bool mb_button_lock;		///-- ��ư Lock ���� ��ư ������ ���� ����
	SBundleItem Potion[MAKESKILLPOTIONNUM];	
};




extern	CItemSkillPotionWndProc	g_ItemSkillPotionWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMENCHANT_WNDPROC_H__