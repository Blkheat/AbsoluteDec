#if !defined( __RUI_BANK_WNDPROC_H__ )
#define __RUI_BANK_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "WndProc.h"


class CBankWnd : public CWndProc
{
public:
	CBankWnd(){}
	~CBankWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

};


extern	CBankWnd	BankWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_BANK_WNDPROC_H__