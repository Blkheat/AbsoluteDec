#ifndef __CMESSAGEBOXWND_H
#define __CMESSAGEBOXWND_H

#include "WndProc.h"

typedef enum MESSAGEBOX_TYPE
{
	MB_TYPE_NONE					 = 0 ,			
	MB_TYPE_TIMER						 ,
	MB_TYPE_OK					         ,
	MB_TYPE_CANCEL						 ,
	MB_TYPE_PROCESSCLOSE				 ,
};

class CMessageBoxWnd : public CWndProc
{
public:
	 
	 CMessageBoxWnd(){}
	~CMessageBoxWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	

	// 버튼 타입
	void	Open(void *pContext,MESSAGEBOX_TYPE Type,CWndProc *pParent, BOOL bProcessClose = FALSE);
	// 타이머 타입
	void    Open(void  *pContext,DWORD Time,CWndProc *pParent, BOOL bProcessClose = FALSE);
	// 수동으로 닫어야하는 타입
	void	Open(void *pContext);

	void	SetRurnURL(char * pstrReturnUrl);
	
	void    Close(void);
	void    SendParent(UINT Notify);
	
	BOOL	IsProcessClose()	{ return m_bProcessClose; }	

private:

	CFrameWnd*		m_MessageBoxWnd;
	CButton*		m_OKBtn;
	CButton*		m_CancelBtn;
	CTextBox*       m_ContextBox;

	CWndProc	   *m_pParentWndProc;	
	DWORD			m_LifeTime;
	DWORD			m_OldTime;

	CImageBox*		m_DialogImage;

	SRect			m_R;
	
	MESSAGEBOX_TYPE  m_Type;
	BOOL			 m_bProcessClose;
	BOOL			 m_OpenReturnUrl;
	
	char		     m_ReturnURL[100];
};	
extern	CMessageBoxWnd	MessageBoxWnd;

#endif