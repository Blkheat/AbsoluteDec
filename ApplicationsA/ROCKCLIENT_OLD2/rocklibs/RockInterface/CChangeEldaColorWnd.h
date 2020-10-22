#ifndef __CCHANGEELDACOLORWND_H__
#define __CCHANGEELDACOLORWND_H__
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

class CChangeEldaColorWnd : public CWndProc
{
public:
	CChangeEldaColorWnd();
	~CChangeEldaColorWnd();
	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	

	void Open();
	void Close();

	void SetPayPcItem( SPcItem * pPcItem );

	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );

	void CheckElda_ItemList();
	void ViewApplyEldaStoneList();
	void SetSlotItem(DWORD nItemCode , CSlot * pSlot);
	void ClearApplyEldaStoneList();

	void SetSelectColorTxt();

	void SetNonSelectColorElda(); 	

	CFrameWnd *			m_pFrameWnd;				///-- �ܰ� ������
	SPcItem				m_CPayItem;					

	CSlot *				m_pCEldaSlot;				///-- ������ ���� ����
	SPcItem				m_CEldaItem;

	CSlot *				m_pCREldaCheckSlot[10];		///-- ������ ���ϴ� ����Ÿ��
	SPcItem				m_CREldaCheckItem[10];
	CImageBox *			m_pCRSelectImg[10];			///-- ������ ���ϴ� ���� �����̹���

	CButton *			m_pCloseBtn;				///-- �ݱ� ��ư
	CButton	*			m_pOkBtn;					///-- Ȯ�� ��ư

	CTextBox *			m_pSelectColorTxt;			///-- ���� ���û��� 					

	DWORD				m_ItemID;
	int					m_nSelectIndex;

	BOOL				m_IsButtonLock;				///-- ��ư Lock ���� ��ư ������ ���� ����

	int					m_nMinLevel;
	int					m_nMaxLevel;

	int					m_nNowEldaLevel;
};

extern CChangeEldaColorWnd	* g_pChangeEldaColorWnd;

#endif