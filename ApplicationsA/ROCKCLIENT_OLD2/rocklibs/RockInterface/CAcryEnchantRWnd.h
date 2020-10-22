#ifndef __CACRYENCAHNTRWND_H__
#define __CACRYENCAHNTRWND_H__
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


class CAcryEnchantRWnd : public CWndProc
{
public:
	CAcryEnchantRWnd();
	~CAcryEnchantRWnd();
	
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
	
	CFrameWnd * m_pARFrameWnd;

	CSlot *	m_pAccessarySlot;
	SPcItem	 m_AccessaryItem;	
	
	CTextBox * m_pResultTxt;		///-- ��� ����
	CTextBox * m_pAddAttriTxt;		///-- �ΰ� �Ӽ� ����

	CButton * m_pCloseBtn;			///-- �ݱ� ��ư
	CButton	* m_pConfirmBtn;		///-- Ȯ�� ��ư

	CImageBox*	m_pIConX;			///-- ������ ����� ���� �̹���
};

extern CAcryEnchantRWnd	* g_pAcryEnchantRWnd;

#endif