#ifndef __CUSERSTORETITLEWND_H
#define __CUSERSTORETITLEWND_H

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "Wnd.h"
#include "WndProc.h"

class CUserStoreTitleWnd : public CWndProc
{
public:
	 
	 CUserStoreTitleWnd();
	~CUserStoreTitleWnd();

public:

	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:

	void Open(void);
	void Close();

public:

	CFrameWnd	*m_pFrame;
	CButton		*m_pCloseBtn;
	CButton		*m_pXBtn;
	CButton		*m_pOKBtn;
	CTextBox	*m_pStoreNameLabel;
	CTextBox	*m_pStoreContextLabel;
	CTextBox	*m_pHelpLabel;
	CImageBox9  *m_pEditBackImage[2];
	CEditBox	*m_pStoreNameEditBox;
	CEditBox	*m_pStoreContextEditBox;

public:

	char		 m_StoreNameStr[ 41 ];
	char		 m_StoreContextStr[ 41 ];

	BOOL		 m_IsVisible;
};

extern CUserStoreTitleWnd g_UserStoreTitleWnd;

#endif	