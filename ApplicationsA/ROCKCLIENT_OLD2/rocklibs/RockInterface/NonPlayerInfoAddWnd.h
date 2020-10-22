// NonPlayerInfoAddWnd.h: interface for the CNonPlayerInfoAddWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NONPLAYERINFOADDWND_H__C2C5A9D5_3143_4D22_8B57_6BDD4AE9694D__INCLUDED_)
#define AFX_NONPLAYERINFOADDWND_H__C2C5A9D5_3143_4D22_8B57_6BDD4AE9694D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "WndProc.h"

class CNonPlayerInfoAddWnd : public CWndProc  
{
public:
	CNonPlayerInfoAddWnd();
	virtual ~CNonPlayerInfoAddWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );	

};

class CNonPlayerInfoAdd2Wnd : public CWndProc  
{
protected:
	CFrameWnd * m_pFrameWnd;
	CButton *	m_pSPartyBtn;
	CButton *   m_pSLeagueBtn;
	CButton *	m_pSFrontierBtn;
	CButton *   m_pCoIFrontierBtn;

public:
	CNonPlayerInfoAdd2Wnd();
	virtual ~CNonPlayerInfoAdd2Wnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void Open();
	void Close();
};

extern CNonPlayerInfoAddWnd NonPlayerInfoAddWnd;
extern CNonPlayerInfoAdd2Wnd g_NonPlayerInfoAdd2Wnd;

#endif // !defined(AFX_NONPLAYERINFOADDWND_H__C2C5A9D5_3143_4D22_8B57_6BDD4AE9694D__INCLUDED_)
