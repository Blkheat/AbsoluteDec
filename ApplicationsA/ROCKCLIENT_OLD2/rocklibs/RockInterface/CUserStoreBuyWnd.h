#ifndef __CUSERSTOREBUYWND_H
#define __CUSERSTOREBUYWND_H

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ScrollBar.h"
#include "Slot.h"
#include "Wnd.h"
#include "WndProc.h"
#include "CUserStoreSellWnd.h"

class CUserStoreBuyWnd : public CWndProc
{
public:
	 
	 CUserStoreBuyWnd();
	~CUserStoreBuyWnd();

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

	void StoreBuySlotFocus(SEventMessage*  EMsg );	
	void StoreBuySlotClick(SEventMessage*  EMsg );	
	void StoreBuySlotDBClick(SEventMessage*  EMsg );
	void StoreBuySlotDrag(SEventMessage*  EMsg );	
	void StoreBuySlotDrop(SEventMessage*  EMsg );
	
	void UpdateSlot(void);
	void SortSlot(void);
	void ChangeItem(DWORD ItemID,BYTE Count);
	
public:

	void		SetShopPCID(DWORD ID,TCHAR *Name) 
	{ 
		m_ShopPCID = ID; 
		
		memcpy( m_Name , Name , sizeof( m_Name ) );
	}

	DWORD		m_ShopPCID;
	TCHAR		m_Name[32];					// 아이디...

	CFrameWnd	*m_pFrame;
	
	CScrollBar	*m_pVScroll;
	CButton		*m_pCloseBtn;
	CButton		*m_pStartBtn;
	CButton		*m_pSortBtn;
	
	CTextBox	*m_TextBox[USER_STORE_SELL_ITEM_SLOT_MAX_COUNT][4]; // 0 , 1 : 값 | 2 , 3 : 라벨
	CTextBox    *m_pStoreNameText;
	CTextBox    *m_pStoreContextText;

	BOOL		m_UserStoreBuyMode;

	BOOL		ISUserBuyMode(void)		{ return m_UserStoreBuyMode; }
	void		SetUserBuyMode(BOOL Mode) { m_UserStoreBuyMode = Mode; }
	void		Close(void);
	void		ItemClear(void);
	void		SetItemList(char *pName,char *pContext,int Num,URpdItem *pItemList,DWORD *m_pPriceList);

public:

	CSlot       *m_pItemSlotList[ USER_STORE_SELL_ITEM_SLOT_MAX_COUNT ];
	SPcItem		 m_WarehousePcItem[ USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ];
	int			 m_PriceList[ USER_STORE_SELL_ITEM_SAVE_MAX_COUNT ];
	
	int			 m_TopSlotIndex;
	BOOL		 m_IsVisible;
};

extern CUserStoreBuyWnd g_UserStoreBuyWnd;

#endif	