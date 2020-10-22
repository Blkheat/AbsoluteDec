#if !defined( __RUI_GUILDINVEN_WNDPROC_H__ )
#define __RUI_GUILDINVEN_WNDPROC_H__
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


const int c_MaxGuildInvenItemDrawNum( 25 );


const int c_MaxGuildInvenPcItemNum( 50 );		// c_MaxGuildInvenItemDrawNum * Box 2 



enum N_GUILDINVENSTATE
{
	n_GUILDINVEN_SHOP1 = 0,
	n_GUILDINVEN_SHOP2, 
};

#ifdef DECO_RENEWAL_GUILD_MJH
	
class CGuildInvenWnd : public CWndProc
{
public:
	CFrameWnd*			m_FrameWnd;
	SPcItem				m_GuildInvenPcItem[c_MaxGuildInvenPcItemNum];

public:
	CGuildInvenWnd() { Init(); }
	~CGuildInvenWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void Open();
	void LogOff();	 
	void ResetFrontierBank();
	void CloseGuildInvenWnd();
	void CloseNumberInputWnd( int aNumItem );
	void SetGuildInvenPcItem( BYTE aItemNum, URpdItem aItemList[] );
	void UpdateGuildInvenPcItem();
	
private:	
	
	N_GUILDINVENSTATE	m_GuildInvenState;
		
	CSlot*				m_ItemSlot[c_MaxGuildInvenItemDrawNum];
	
	int					m_StartSlotIndex;

	// 소모품 아이템 개수 입력시 필요 
	CSlot*				m_SrcSlot;			
	CSlot*				m_TargetSlot;


	void SetGuildInvenState( N_GUILDINVENSTATE state );
	
	void GuildInvenSlotFocus( SEventMessage* EMsg );
	void GuildInvenSlotClick( SEventMessage* EMsg );
	void GuildInvenSlotDBClick( SEventMessage* EMsg );
	void GuildInvenSlotDrag( SEventMessage* EMsg );
	void GuildInvenSlotDrop( SEventMessage* EMsg );

	void ResetCursorPcItem();

};

extern	CGuildInvenWnd	g_GuildInvenWnd;

#endif // DECO_RENEWAL_GUILD_MJH
//-----------------------------------------------------------------------------
#endif  __RUI_GUILDINVEN_WNDPROC_H__
