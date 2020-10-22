#if !defined( __RUI_PARTYPOPUP_WND_H__ )
#define __RUI_PARTYPOPUP_WND_H__
//-----------------------------------------------------------------------------

#include "PartyMemberWnd.h"
#include "WndProc.h"
#include "LimitLenTextBox.h"

class CPartyPopupWnd : public CWndProc
{
protected:
	SPARTYMEMBER_INFO * m_pMemberInfo;			 	//파티 멤버 정보
	
	CFrameWnd *			m_pPartyPopupWnd;
	CLimitLenTextBox*	m_pPartyPopName;	

	CButton*		 m_pPartyPopTexBtn1;
	CButton*		 m_pPartyPopTexBtn2;
	CButton*		 m_pPartyPopTexBtn3;
	CButton*		 m_pPartyPopTexBtn4;

	TCHAR			 m_tmpStr[128];

public:
	CPartyPopupWnd();
	~CPartyPopupWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	CFrameWnd *	     GetPartyPopupWnd()	{ return m_pPartyPopupWnd; }
	BOOL			 CheckFocusWnd( UINT  FocusWnd );
	
	void	SetActiveMemnu(bool bMenu1, bool bMenu2, bool bMenu3, bool bMenu4);
	void	SetMemberInfo( SPARTYMEMBER_INFO * pMemberInfo );
	void    LogOff();

protected:
	void    UpdatePartyPopupInfo();
};

extern CPartyPopupWnd g_PartyPopupWnd;

//-----------------------------------------------------------------------------
#endif  __RUI_PARTYPOPUP_WND_H__