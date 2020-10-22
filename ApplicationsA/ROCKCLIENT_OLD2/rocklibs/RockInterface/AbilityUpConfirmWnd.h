#if !defined( __RUI_ABILITYUPCONFIRM_WNDPROC_H__ )
#define __RUI_ABILITYUPCONFIRM_WNDPROC_H__
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
#include "Spin.h"
#include "Wnd.h"

#include "WndProc.h"



class CAbilityUpConfirmWnd : public CWndProc
{
public:
	CAbilityUpConfirmWnd(){}
	~CAbilityUpConfirmWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void InitTextBox();

	CTextBox*	PowerTxt;
	CTextBox*	VitalTxt;
	CTextBox*	StaminaTxt;
	CTextBox*	DexTxt;
	CTextBox*	SympathyTxt;
	CTextBox*	IntTxt;

	CTextBox*	DescTxt;
};


extern	CAbilityUpConfirmWnd	AbilityUpConfirmWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_ABILITYUPCONFIRM_WNDPROC_H__