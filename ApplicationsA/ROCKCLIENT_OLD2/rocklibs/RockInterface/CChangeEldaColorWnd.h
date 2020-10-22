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

	CFrameWnd *			m_pFrameWnd;				///-- 외곽 윈도우
	SPcItem				m_CPayItem;					

	CSlot *				m_pCEldaSlot;				///-- 변경할 엘다 슬롯
	SPcItem				m_CEldaItem;

	CSlot *				m_pCREldaCheckSlot[10];		///-- 변경을 원하는 엘다타입
	SPcItem				m_CREldaCheckItem[10];
	CImageBox *			m_pCRSelectImg[10];			///-- 변경을 원하는 엘다 선택이미지

	CButton *			m_pCloseBtn;				///-- 닫기 버튼
	CButton	*			m_pOkBtn;					///-- 확인 버튼

	CTextBox *			m_pSelectColorTxt;			///-- 현재 선택색상 					

	DWORD				m_ItemID;
	int					m_nSelectIndex;

	BOOL				m_IsButtonLock;				///-- 버튼 Lock 가공 버튼 여러번 누름 방지

	int					m_nMinLevel;
	int					m_nMaxLevel;

	int					m_nNowEldaLevel;
};

extern CChangeEldaColorWnd	* g_pChangeEldaColorWnd;

#endif