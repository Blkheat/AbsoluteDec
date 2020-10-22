#if !defined( __RUI_PAYITEM_WNDPROC_H__ )
#define __RUI_PAYITEM_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"



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


const int c_MaxPayItemDrawNum( 25 );


const int c_MaxPayPcItemNum( 250 );		



	
class CPayItemWnd : public CWndProc
{
public:
	CFrameWnd*			m_FrameWnd;
	CScrollBar*			m_pScrollBar;

	SPcItem				m_PayPcItem[c_MaxPayPcItemNum];


public:
	CPayItemWnd() { Init(); }
	~CPayItemWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void Open();
	void LogOff();	
	void ClosePayItemWnd();
//	void CloseNumberInputWnd( int aNumItem );
	void SetPayItemWndPcItem( SPayItem aItemList );
	void DeletePcItem( DWORD aPayItemID );
	void UpdatePayItemPcItem();
	
	void SetWheelValue( short _wheel );
	void SetSlotScroll( int linenum );

private:	
				
	CSlot*				m_ItemSlot[c_MaxPayItemDrawNum];
	
	int					m_StartSlotIndex;

	// 소모품 아이템 개수 입력시 필요 
//	CSlot*				m_SrcSlot;			
//	CSlot*				m_TargetSlot;

	CButton*			m_pCloseBtn;


	void PayItemSlotFocus( SEventMessage* EMsg );
	void PayItemSlotClick( SEventMessage* EMsg );
	void PayItemSlotDBClick( SEventMessage* EMsg );
	void PayItemSlotDrag( SEventMessage* EMsg );
	void PayItemSlotDrop( SEventMessage* EMsg );

	void ResetCursorPcItem();

};

extern	CPayItemWnd	g_PayItemWnd;



//-----------------------------------------------------------------------------
#endif  __RUI_PAYITEM_WNDPROC_H__
