#if !defined( __RUI_SKILL_POTION_R_WNDPROC_H__ )
#define __RUI_SKILL_POTION_R_WNDPROC_H__
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


class CItemSkillPotionRWndProc : public CWndProc
{
public:
	CItemSkillPotionRWndProc(){}
	~CItemSkillPotionRWndProc(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	//void OpenEnchantR_SWnd( const int ItemUID );
	void OpenEnchantR_SWnd( const int ItemUID  , const int bundle = 0 );
	void OpenEnchantRCode_SWnd( const int Code  , const int bundle = 0 );
	void OpenEnchantR_FWnd();

	void EnchantText( bool flag );
	CSlot*			mcp_item_slot;
	SPcItem			mc_itemslot_item;
	CImageBox*		mcp_icon_x;
		
	CTextBox*		mcp_exp;
	CTextBox*		mcp_exp_penalty;
	CTextBox*		mcp_effect[2];
	CTextBox*		mcp_result_text;

	CFrameWnd* FrameWnd;

	CButton *Exit_btn;
	CButton *Ok_btn;



};



extern	CItemSkillPotionRWndProc	g_ItemSkillPotionRWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_SKILL_POTION_R_WNDPROC_H__