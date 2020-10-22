#ifndef __CUSER_STORE_SETBUY_WND_H
#define __CUSER_STORE_SETBUY_WND_H

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "Wnd.h"
#include "WndProc.h"
#include "CUserStoreBuyWnd.h"

class CUserStoreSetBuyWnd : public CWndProc
{
public:
	 
	 CUserStoreSetBuyWnd();
	~CUserStoreSetBuyWnd();

public:

	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:

	void Open(void);
	void Close(void);
	void SetItemInfo( SPcItem* _pc_item , int Price );
	void CalTotal(void);

public:

	CFrameWnd	*m_pFrame;
	CButton		*m_pXBtn;
	CButton		*m_pOKBtn;
	CButton		*m_pCloseBtn;

	CTextBox	*m_pPriceLabel;
	CTextBox	*m_pCountLabel;
	
	CImageBox9  *m_pEditBackImage[2];
	
	CTextBox	*m_pPriceEditBox;
	CEditBox	*m_pCountEditBox;

	CTextBox	*m_pTotalEditBox;
	CTextBox	*m_pTotalLabel;

public:

	SPcItem*	 m_pPcItem;
	CImageBox*	 m_pIconImg;  

	int			 m_Price;
	int 		 m_Count;

	BOOL		 m_IsVisible;
};

extern CUserStoreSetBuyWnd g_UserStoreSetBuyWnd;

#endif	