#ifndef __MPOPUPMENUWND_H__
#define __MPOPUPMENUWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"

class CMPopupMenuWnd : public CWndProc
{
protected:	
	CFrameWnd *      m_pMPopupWnd;	

	CButton*		 m_pMPopTexBtn1;
	CButton*		 m_pMPopTexBtn2;
	CButton*		 m_pMPopTexBtn3;
	CButton*		 m_pMPopTexBtn4;

	TCHAR			 m_strParam[256];

public:
	CMPopupMenuWnd();
	~CMPopupMenuWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public: 
	CFrameWnd *		 GetPopUpWnd()	{ return m_pMPopupWnd; }
	bool			 CheckFocusWnd( UINT  FocusWnd );
	void			 SetParam( TCHAR * strName );
	void			 SetActiveMenu(bool bMenu1, bool bMenu2, bool bMenu3, bool bMenu4);	
};

//-----------------------------------------------------------------------------
#endif	__MPOPUPMENUWND_H__