#ifndef __CCHANGEELDACOLORRWND_H__
#define __CCHANGEELDACOLORRWND_H__
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

class CChangeEldaColorRWnd : public CWndProc
{
public:
	CChangeEldaColorRWnd();
	~CChangeEldaColorRWnd();
	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void Open(BOOL IsSuccess);
	void Close();
	
	void SetPcItem( SPcItem * pPcItem );
	void RePosition( int nXPos, int nYPos );

private:
	
	BOOL	 m_IsSuccess;
	
	CFrameWnd * m_pCRFrameWnd;

	CSlot *	m_ChangeEldaSlot;
	SPcItem	 m_ChangeEldaItem;	
	
	CTextBox * m_pResultTxt;		///-- ��� ����	

	CButton * m_pCloseBtn;			///-- �ݱ� ��ư
	CButton	* m_pConfirmBtn;		///-- Ȯ�� ��ư

	CImageBox*	m_pIConX;			///-- ������ ����� ���� �̹���
};

extern CChangeEldaColorRWnd	* g_pChangeEldaColorRWnd;

#endif