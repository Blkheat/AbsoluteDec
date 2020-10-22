//----------------------------------------------------------------------------------------------------------------------
// 파일명 : ExchangeWndProc.h
// 용  도 : 유저간 거래
//----------------------------------------------------------------------------------------------------------------------
#if !defined( __RUI_EXCHANGE_WNDPROC_H__ )
#define __RUI_EXCHANGE_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "UpGauge.h"
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



const int c_MaxExchangeNum( 10 );


class CExchangeWnd : public CWndProc
{
public:
	BOOL				m_bIsExchangeEnable;
	BOOL				m_bIsExchangeLock;	

	BOOL				m_bIsAccept;

	BOOL				m_bIsYoursUpload;
	
	int					m_ExchangePcIndex;
	
	DWORD				m_MineEld;
	CTextBox*			m_MineEldText;
	
	DWORD				m_YoursEld;
	CTextBox*			m_YoursEldText;


	CExchangeWnd() { Init(); }
	~CExchangeWnd() {}


	virtual void Init();
	virtual void Composition();
	virtual void Update();	
	virtual void Proc( SEventMessage* EMsg );

	void Open();
	void LogOff();
	void CloseNumberInputWnd( int aNumItem );
	void ResetCursorPcItem();
	
	BOOL CheckExChangeList();
	BOOL CheckExChangeEld();

	void CloseExchangeWnd();
	void ResetMineItemSlot();

	void CheckWndUpdate();	

	void SetYoursItemSlot( DWORD aEld, BYTE aItemNum, URpdItem aItemList[] );

	SPcItem* GetMyItemPcItem(){return m_MinePcItem ;};
	SPcItem* GetYoursItemPcItem(){return m_YoursPcItem; };

private:
	CSlot*				m_MineItemSlot[c_MaxExchangeNum];
	CSlot*				m_YoursItemSlot[c_MaxExchangeNum];

	SPcItem				m_MinePcItem[c_MaxExchangeNum];
	SPcItem				m_YoursPcItem[c_MaxExchangeNum];
	
	CButton*			m_UploadBtn;
	CButton*			m_OKBtn;
	CButton*			m_CancelBtn;

	// 소모품 아이템 개수 입력시 필요 
	CSlot*				m_SrcSlot;			
	CSlot*				m_TargetSlot;
	

	void ExchangeSlotFocus( SEventMessage* EMsg );
	void ExchangeSlotClick( SEventMessage* EMsg );
	void ExchangeSlotDBClick( SEventMessage* EMsg );
	void ExchangeSlotDrag( SEventMessage* EMsg );
	void ExchangeSlotDrop( SEventMessage* EMsg );

	BOOL CheckSameMineItemSlot( SPcItem* pPcItem );
	
};



extern	CExchangeWnd		g_ExchangeWnd;



//-----------------------------------------------------------------------------
#endif  __RUI_EXCHANGE_WNDPROC_H__


