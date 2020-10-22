#if !defined( __RUI_ESSENCE_WNDPROC_H__ )
#define __RUI_ESSENCE_WNDPROC_H__
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


class CEssenceWndProc : public CWndProc
{
public:
	CEssenceWndProc();
	~CEssenceWndProc(){}
	
	 

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
	
	CFrameWnd*		m_pFrameWnd;
private:
	void	ClearPreItem();
	BOOL	CheckItemList();
	void	ItemSlotClick( SEventMessage* EMsg );
	void	ItemSlotDBClick( SEventMessage* EMsg );

	CImageBox*		m_PreItemImg[4];

	CButton*		start_btn;	
	CButton*		cancel_btn;
	CImageBox*		ImgSlot[3];
	
	SPcItem			m_PrePcItem[2];	
	CSlot	*		m_PreSlot[2];

	CTextBox*		ChangeGold[3];
	CEditBox*		EditItemCreateNum;

	CTextBox*		m_MoneyText;

	int				m_MaxItemCount;

	bool mb_button_lock;		///-- ��ư Lock ���� ��ư ������ ���� ����
	int	ExChangeItemMaxCount;	// ��ȯ�� �������� �Ѱ��� 

};

extern	CEssenceWndProc	*g_EssenceWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ESSENCE_WNDPROC_H__