//----------------------------------------------------------------------------------------------------------------------
// 파일명 : CUserStoreSellWnd.h
// 용  도 : 개인상점 판매
//----------------------------------------------------------------------------------------------------------------------
#ifndef __CUSERSTORESELLWND_H
#define __CUSERSTORESELLWND_H

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ScrollBar.h"
#include "Slot.h"
#include "Wnd.h"
#include "WndProc.h"

#define USER_STORE_SELL_ITEM_SLOT_MAX_COUNT		8
#define USER_STORE_SELL_ITEM_SAVE_MAX_COUNT		50

class CUserStoreSellWnd : public CWndProc
{
public:
	 
	 CUserStoreSellWnd();
	~CUserStoreSellWnd();

public:

	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

public:

	void ResetItemList(void);
	void Open(void);
	void SetSlotLabel( int slot , int price , int count );
	void SetSlotVisible( int slot , BOOL Visible );

public:

	CFrameWnd	*m_pFrame;
	
	CScrollBar	*m_pVScroll;
	CButton		*m_pCloseBtn;
	CButton		*m_pStartBtn;
	CButton		*m_pSortBtn;
	
	CTextBox	*m_TextBox[USER_STORE_SELL_ITEM_SLOT_MAX_COUNT][4]; // 0 , 1 : 값 | 2 , 3 : 라벨
	CTextBox    *m_pStoreNameText;
	CTextBox    *m_pStoreContextText;

public:

	BOOL		m_UserStoreSellMode;

	BOOL		ISUserSellMode(void)		{ return m_UserStoreSellMode; }
	void		SetUserSellMode(BOOL Mode) { m_UserStoreSellMode = Mode; }
	BOOL		m_UserStoreSellOpenMode;

	//......................................................................................................
	// 슬롯 관련 이벤트 처리
	//......................................................................................................
	void		StoreSellSlotFocus(SEventMessage* EMsg);	
	void		StoreSellSlotClick(SEventMessage* EMsg);
	void		StoreSellSlotDBClick(SEventMessage* EMsg);
	void		StoreSellSlotDrag(SEventMessage* EMsg);
	void		StoreSellSlotDrop(SEventMessage* EMsg);	
	
	void		UpdateSlot(void);
	BOOL		ISFindItem(SPcItem *pItem);
	void		InsertItem(CSlot *cslot,CSlot *pslot);
	void		DeleteItem(CSlot *cslot);
	void		InsertSaveItem(int Price,int Count);
	
	void		UserStoreStart(void);
	void		SortSlot();
	void		Close(void);
	
	void		SetItemList(char *pContext,int Num,URpdItem *pItemList,DWORD m_pPriceList);

	void		SellInMyStore(DWORD PCID,DWORD Eld,DWORD ItemID,BYTE Count,int *pSubValue);

	void		ItemClear(void);

	int			GetItemCountInIventory(DWORD UID);
	
private:
	SPcItem		*m_SaveSrcItem;
	int			 m_SaveSlotNum;
	BOOL		 m_ISSellLockMode;

public:

	CSlot       *m_pItemSlotList[ USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ];
	SPcItem		 m_WarehousePcItem[ USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ];
	
	int			 m_TopSlotIndex;
	BOOL		 m_IsVisible;
	int			 m_PriceList[ USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ];

private:

	BOOL		 ISEmptyItem(void);

};

extern CUserStoreSellWnd g_UserStoreSellWnd;

#endif	