#if !defined( __RUI_ITEMELDAMIXR_WNDPROC_H__ )
#define __RUI_ITEMELDAMIX_WNDPROC_H__
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

#include "ImageAni.h"
#include "WndProc.h"


class CItemEldaMixRWndProc : public CWndProc
{
public:
	CItemEldaMixRWndProc();
	~CItemEldaMixRWndProc(){}

	enum eITEMMODE{ eEldaMix , eStrengthStone	};

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void OpenEnchantR_SWnd(URpdItem MixElda , eITEMMODE Mode );
	void OpenEnchantR_FWnd(eITEMMODE Mode);
	void Close();


	void EnchantText( bool flag );

	void EffectInit();
	void EffectUpdate();

	CSlot*			mcp_item_slot;
	SPcItem			mc_itemslot_item;
	CImageBox*		mcp_icon_x;
		
	CTextBox*		mcp_exp;
	CTextBox*		mcp_exp_penalty;
	CTextBox*		mcp_effect[2];
	CTextBox*		mcp_result_text;

	eITEMMODE m_eItemMode; 
	
	CFrameWnd* FrameWnd;

	CButton *Exit_btn;
	CButton *Ok_btn;

	CImageAni *m_SpriteElda[3] ; 
	DWORD OneTimer;
	bool	bOneCount;
	CImageBox* m_FailImg;
	CImageBox* m_FailImg2;	
	bool	bSuccess;
	bool	m_bEndMixElda;	// 성공이나 실패시 이펙트가 다 돌았냐 ?? 

	URpdItem m_MixElda;


};



extern	CItemEldaMixRWndProc	g_ItemEldaMixRWndProc;
//-----------------------------------------------------------------------------
#endif  __RUI_ITEMELDAMIX_WNDPROC_H__