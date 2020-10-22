#ifndef __CUSER_STORE_SET_WND_H
#define __CUSER_STORE_SET_WND_H

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "Wnd.h"
#include "WndProc.h"

class CUserStoreSetWnd : public CWndProc
{
public:
	 
	 CUserStoreSetWnd();
	~CUserStoreSetWnd();

public:

	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:

	void Open(void);
	void SetItemInfo( SPcItem* _pc_item , int Count );
	void Close(void);

public:

	CFrameWnd	*m_pFrame;
	CButton		*m_pCloseBtn;
	CButton		*m_pXBtn;
	CButton		*m_pOKBtn;

	CTextBox	*m_pPriceLabel;
	CTextBox	*m_pCountLabel;
	
	CImageBox9  *m_pEditBackImage[2];
	
	CEditBox	*m_pPriceEditBox;
	CEditBox	*m_pCountEditBox;
	CTextBox	*m_pCountEditLabel;

public:

	SPcItem*	 m_pPcItem;
	CImageBox*	 m_pIconImg;  

	int			 m_Price;
	int 		 m_Count;

	BOOL		 m_IsVisible;
};

extern CUserStoreSetWnd g_UserStoreSetWnd;

#endif	