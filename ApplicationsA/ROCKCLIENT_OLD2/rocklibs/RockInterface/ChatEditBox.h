#ifndef __CHATEDITBOX_H__
#define __CHATEDITBOX_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#include "EditBox.h"

class CChatEditBox : public CEditBox
{
protected:
	int			n_LimitLen;
public:
	CChatEditBox() {}; 
	CChatEditBox( SDesktop* DT );
	virtual ~CChatEditBox();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );//OR
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	//------------------------------------------------------------------------

	virtual LRESULT CALLBACK EditSubProc( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam );
	virtual LRESULT CALLBACK EditSubProc_JP( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam );
	virtual LRESULT CALLBACK EditSubProc_IME( HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam );
	
	virtual void DrawText();
	virtual void DrawCaret();

	virtual void GetWindowEditCaretInfo();
	virtual void CreateWindowEditBox();

	void SetLimitLen(int nLen);
	int  GetVisibleIdx();
};

//-----------------------------------------------------------------------------
#endif	__CHATEDITBOX_H__