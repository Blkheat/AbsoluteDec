// CPAcceptYesNoWnd.h: interface for the CPAcceptYesNoWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACCEPTYESNOWND_H__0D6A231F_C86C_4A8E_BBD7_698CFB01B7FA__INCLUDED_)
#define AFX_PACCEPTYESNOWND_H__0D6A231F_C86C_4A8E_BBD7_698CFB01B7FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WndProc.h"


enum N_ACCEPTSTATE
{
	n_ACCEPT_PARTY = 0,
	n_ACCEPT_EXCHANGE
	
};


class CAcceptYesNoWnd : public CWndProc
{
public:
	N_ACCEPTSTATE		m_AcceptState;
	TCHAR				m_strLeaderName[256];         //파티 초대 리더 네임


	void Open( N_ACCEPTSTATE aAcceptState );	
	void SetLearderName( TCHAR* pName );
	void LogOff();

public:
	CAcceptYesNoWnd();
	virtual ~CAcceptYesNoWnd();

	virtual void Init();
    virtual void Composition();
    virtual void Update();
    virtual void Proc( SEventMessage* EMsg );


protected:	
    CTextBox*			m_pTextBox[3];
	CButton*			m_button[2];

};

extern CAcceptYesNoWnd g_AcceptYesNoWnd;


#endif // !defined(AFX_PACCEPTYESNOWND_H__0D6A231F_C86C_4A8E_BBD7_698CFB01B7FA__INCLUDED_)

