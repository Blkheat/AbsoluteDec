#ifndef __MGUILDTABWND_H__
#define __MGUILDTABWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "MFriendTabWnd.h"

class CMGuildTabWnd : public CMFriendTabWnd
{
protected:
	CFrameWnd	* m_pMGuildTabWnd;
	CTextBox	* m_pMGTextBox1;	

public:
	CMGuildTabWnd();
	virtual ~CMGuildTabWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
};

//-----------------------------------------------------------------------------
#endif	__MGUILDTABWND_H__