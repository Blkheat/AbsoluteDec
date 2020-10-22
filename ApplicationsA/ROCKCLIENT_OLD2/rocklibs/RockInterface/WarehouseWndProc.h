#if !defined( __RUI_WAREHOUSE_WNDPROC_H__ )
#define __RUI_WAREHOUSE_WNDPROC_H__
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
#include "StateButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Slot.h"
#include "Wnd.h"

#include "WndProc.h"


#include "..\\..\\bravolibs\\obj\\player_manager.h" 


const int c_MaxWarehouseItemDrawNum( 25 );


const int c_MaxWarehousePcItemNum( 125 );		// c_MaxWarehouseItemDrawNum * Box 5 



enum N_WAREHOUSESTATE
{
	n_WAREHOUSE_SHOP1 = 0,
	n_WAREHOUSE_SHOP2, 
	n_WAREHOUSE_SHOP3, 
	n_WAREHOUSE_SHOP4, 
	n_WAREHOUSE_SHOP5, 
};


	
class CWarehouseWnd : public CWndProc
{
public:
	CFrameWnd*			m_FrameWnd;

	DWORD				m_Eld;
	CTextBox*			m_EldText;

	SPcItem				m_WarehousePcItem[c_MaxWarehousePcItemNum];


public:
	CWarehouseWnd() { Init(); }
	~CWarehouseWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void Open();
	void LogOff();	
	void CloseWarehouseWnd();
	void CloseNumberInputWnd( int aNumItem );
	void SetWarehousePcItem( DWORD aEld, BYTE aItemNum, URpdItem aItemList[] );
	void UpdateWarehousePcItem();
	
private:	
	
	N_WAREHOUSESTATE	m_WarehouseState;
		
	CSlot*				m_ItemSlot[c_MaxWarehouseItemDrawNum];
	
	int					m_StartSlotIndex;

	// 소모품 아이템 개수 입력시 필요 
	CSlot*				m_SrcSlot;			
	CSlot*				m_TargetSlot;


	void SetWarehouseState( N_WAREHOUSESTATE state );
	
	void WarehouseSlotFocus( SEventMessage* EMsg );
	void WarehouseSlotClick( SEventMessage* EMsg );
	void WarehouseSlotDBClick( SEventMessage* EMsg );
	void WarehouseSlotDrag( SEventMessage* EMsg );
	void WarehouseSlotDrop( SEventMessage* EMsg );

	void ResetCursorPcItem();

};

extern	CWarehouseWnd	g_WarehouseWnd;


//-----------------------------------------------------------------------------
#endif  __RUI_WAREHOUSE_WNDPROC_H__
