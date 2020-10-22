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
	nAddAttriNone = 0,		///-- 현재 적용 부가속성 없음
	nAddAttriParalyRes,		///-- 부가속성 - 마비저항
	nAddAttriDispelRes,		///-- 부가속성 - 디스펠 저항
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

	CSlot *		m_pAccessarySlot;		///-- 장신구 슬롯
	CSlot *		m_pEnScrollSlot;		///-- 주문서 슬롯

	CComboBox * m_pAddtypeComBox;		///-- 부가속성 타입
	CTextBox * m_pNowAccAddtype;		///-- 현재 장신구 적용된 부가속성
	CTextBox * m_pNeedScrollNum;		///-- 필요 주문서 텍스트

	SPcItem	 m_AccessaryItem;
	SPcItem	 m_EnScrollItem;
	
	CButton * m_pCloseBtn;				///-- 닫기 버튼
	CButton	* m_pStartBtn;				///-- 가공시작 버튼
	CButton * m_pCancleBtn;				///-- 취소버튼

	BOOL	  m_IsButtonLock;			///-- 버튼 Lock 가공 버튼 여러번 누름 방지

	UINT	  m_nNowAddAttriType;

	int		  m_nNeedScrollNum;			///-- 필요 주문서 개수	
	
	char	  m_strTempString[256];
	
	void ItemSlotClick( SEventMessage* EMsg );
	void ItemSlotDBClick( SEventMessage* EMsg );

	BOOL SetAEnchantText();
};

extern CAcryEnchantWnd	* g_pAcryEnchantWnd;

#endif