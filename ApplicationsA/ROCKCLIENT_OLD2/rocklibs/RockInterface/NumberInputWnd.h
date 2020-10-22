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
	n_NUMBERINPUT_INVENTORY_ITEMDROP = 0,				// �κ��丮���� ������ ������ 
	n_NUMBERINPUT_INVENTORY_Q_ITEMDROP,					// �κ��丮 ����Ʈ �ǿ��� ������
	n_NUMBERINPUT_INVENTORY_SELLSTORE,					// �κ��丮���� ������ ������ �ȶ�
	n_NUMBERINPUT_BUYSTORE_INVENTORY,					// �������� �κ��丮�� ������ �춧 
	n_NUMBERINPUT_INVENTORY_EXCHANGE,					// �κ��丮���� ��ȯâ���� �̵� 				
	n_NUMBERINPUT_INVENTORY_WAREHOUSE,					// �κ��丮���� â��â���� �̵�
	n_NUMBERINPUT_WAREHOUSE_INVENTORY, 					// â��â���� �κ��丮�� �̵� 
	n_NUMBERINPUT_INVENTORY_TUNING,						// �κ��丮���� ����â���� �̵�
	n_NUMBERINPUT_INVENTORY_CREATE,						// �κ��丮���� ��������â���� �̵�
	n_NUMBERINPUT_INVENTORY_PETINVEN,				//�κ��丮���� ���κ����� 
	n_NUMBERINPUT_PETINVEN_INVENTORY,					//���κ����� �κ��丮 .. 
	n_NUMBERINPUT_INVENTORY_GUILDINVEN,                 // �κ��丮���� ��� ����������
	n_NUMBERINPUT_GUILDINVEN_INVENTORY,                 // ��� �����Կ��� �κ��丮��
	n_NUMBERINPUT_INVENTORY_PROPOSEFRON,
	
};

enum N_STR_INPUT_STATE
{
	n_STRINPUT_BZ_PASSWORD = 0,							// ��Ʋ������ ��� ���� �н����� �Է� 
	n_STRINPUT_FRONTIERPET_NAME,						// �ż� �о� ������ �̸� �Է� 
};

enum N_NUMBER_INPUT_PROC
{
	n_ProcNone,
	n_ProcItemDrop,
};

///---------------------------------------------------------------------------
///-- CNumberInputWnd
///---------------------------------------------------------------------------
///-- ���� �Է� �� ���ȴ�.
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
///-- �н����� �Է� �� ���ȴ�.
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