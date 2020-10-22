#ifndef __CACRYENCAHNTWND_H__
#define __CACRYENCAHNTWND_H__
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

enum NAddAttriType
{
	nAddAttriNone = 0,		///-- ���� ���� �ΰ��Ӽ� ����
	nAddAttriParalyRes,		///-- �ΰ��Ӽ� - ��������
	nAddAttriDispelRes,		///-- �ΰ��Ӽ� - ���� ����
};

class CAcryEnchantWnd : public CWndProc
{
public:
	CAcryEnchantWnd();
	~CAcryEnchantWnd();
	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	UINT GetAddAttriType();
	void SetAddAttriType( UINT uAttriType );

	void Open();
	void Close();

	void RePosition( int nXPos, int nYPos );

	BOOL CheckAEnchant_ItemList();

	CFrameWnd * m_pAFrameWnd;

private:	

	CSlot *		m_pAccessarySlot;		///-- ��ű� ����
	CSlot *		m_pEnScrollSlot;		///-- �ֹ��� ����

	CComboBox * m_pAddtypeComBox;		///-- �ΰ��Ӽ� Ÿ��
	CTextBox * m_pNowAccAddtype;		///-- ���� ��ű� ����� �ΰ��Ӽ�
	CTextBox * m_pNeedScrollNum;		///-- �ʿ� �ֹ��� �ؽ�Ʈ

	SPcItem	 m_AccessaryItem;
	SPcItem	 m_EnScrollItem;
	
	CButton * m_pCloseBtn;				///-- �ݱ� ��ư
	CButton	* m_pStartBtn;				///-- �������� ��ư
	CButton * m_pCancleBtn;				///-- ��ҹ�ư

	BOOL	  m_IsButtonLock;			///-- ��ư Lock ���� ��ư ������ ���� ����

	UINT	  m_nNowAddAttriType;

	int		  m_nNeedScrollNum;			///-- �ʿ� �ֹ��� ����	
	
	char	  m_strTempString[256];
	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );

	BOOL SetAEnchantText();
};

extern CAcryEnchantWnd	* g_pAcryEnchantWnd;

#endif