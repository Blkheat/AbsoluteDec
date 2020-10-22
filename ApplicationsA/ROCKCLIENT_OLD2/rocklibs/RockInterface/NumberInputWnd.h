#if !defined( __RUI_NUMBER_INPUT_WNDPROC_H__ )
#define __RUI_NUMBER_INPUT_WNDPROC_H__
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





enum N_NUMBER_INPUT_STATE
{
	n_NUMBERINPUT_INVENTORY_ITEMDROP = 0,				// 인벤토리에서 아이템 버리기 
	n_NUMBERINPUT_INVENTORY_Q_ITEMDROP,					// 인벤토리 퀘스트 탭에서 버리기
	n_NUMBERINPUT_INVENTORY_SELLSTORE,					// 인벤토리에서 상점에 아이템 팔때
	n_NUMBERINPUT_BUYSTORE_INVENTORY,					// 상점에서 인벤토리로 아이템 살때 
	n_NUMBERINPUT_INVENTORY_EXCHANGE,					// 인벤토리에서 교환창으로 이동 				
	n_NUMBERINPUT_INVENTORY_WAREHOUSE,					// 인벤토리에서 창고창으로 이동
	n_NUMBERINPUT_WAREHOUSE_INVENTORY, 					// 창고창에서 인벤토리로 이동 
	n_NUMBERINPUT_INVENTORY_TUNING,						// 인벤토리에서 개조창으로 이동
	n_NUMBERINPUT_INVENTORY_CREATE,						// 인벤토리에서 무기제조창으로 이동
	n_NUMBERINPUT_INVENTORY_PETINVEN,				//인벤토리에서 펫인벤으로 
	n_NUMBERINPUT_PETINVEN_INVENTORY,					//펫인벤에서 인벤토리 .. 
	n_NUMBERINPUT_INVENTORY_GUILDINVEN,                 // 인벤토리에서 길드 보관함으로
	n_NUMBERINPUT_GUILDINVEN_INVENTORY,                 // 길드 보관함에서 인벤토리로
	n_NUMBERINPUT_INVENTORY_PROPOSEFRON,
	
};

enum N_STR_INPUT_STATE
{
	n_STRINPUT_BZ_PASSWORD = 0,							// 배틀존에서 비방 들어갈때 패스워드 입력 
	n_STRINPUT_FRONTIERPET_NAME,						// 신수 분양 받을때 이름 입력 
};

enum N_NUMBER_INPUT_PROC
{
	n_ProcNone,
	n_ProcItemDrop,
};

///---------------------------------------------------------------------------
///-- CNumberInputWnd
///---------------------------------------------------------------------------
///-- 숫자 입력 에 사용된다.
class CNumberInputWnd : public CWndProc
{
public:
	CFrameWnd*				m_FrameWnd;
	CEditBox*				m_EditBox;

//	CWndProc*				m_LinkWnd;
//	BYTE					m_ItemTab;

//	N_NUMBER_INPUT_PROC		m_NIProc;

	SPcItem*				m_PcItem;
	CTextBox*				msgtbox;

	N_NUMBER_INPUT_STATE	m_InputState;
	
public:
	CNumberInputWnd(){}
	~CNumberInputWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	

	void Open( N_NUMBER_INPUT_STATE aInputState, SPcItem* pPcItem );	
//	void Open( CWndProc* _link_wnd, SPcItem* _pc_item, BYTE _item_tab );
//	void Open( N_NUMBER_INPUT_PROC _niproc, SPcItem* _pc_item );

	void OkClose();
	void CancelClose();
};


///---------------------------------------------------------------------------
///-- CNumberInputWnd
///---------------------------------------------------------------------------
///-- 패스워드 입력 에 사용된다.
class CStrInputWnd : public CWndProc
{
public:
	CFrameWnd*				m_FrameWnd;
	CEditBox*				m_EditBox;
	CTextBox*				msgtbox;
	CButton *				button[2];

	N_STR_INPUT_STATE		m_InputState;

	DWORD					m_Arg1;

public:
	CStrInputWnd(){}
	~CStrInputWnd(){}
	
	virtual void Composition();	
	virtual void Proc( SEventMessage* EMsg );

	void Open( N_STR_INPUT_STATE aInputState, DWORD aArg1 );
	void OkClose();
	void CancelClose();
};

extern	CStrInputWnd		g_StrInputWnd;
extern  CNumberInputWnd		g_NumberInputWnd;

//-----------------------------------------------------------------------------
#endif  __RUI_NUMBER_INPUT_WNDPROC_H__