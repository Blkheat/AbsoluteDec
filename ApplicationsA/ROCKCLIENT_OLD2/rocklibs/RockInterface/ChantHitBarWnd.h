// ChantHitBarWnd.h: interface for the CChantHitBarWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANTHITBARWND_H__01223248_365A_4A2E_97BA_BD220F9971C5__INCLUDED_)
#define AFX_CHANTHITBARWND_H__01223248_365A_4A2E_97BA_BD220F9971C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "WndProc.h"

class CChantHitBarWnd : public CWndProc  
{
public:
	CChantHitBarWnd();
	virtual ~CChantHitBarWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

};

extern CChantHitBarWnd g_ChantHitBarWnd;


#endif // !defined(AFX_CHANTHITBARWND_H__01223248_365A_4A2E_97BA_BD220F9971C5__INCLUDED_)
