///////////////////////////////////////////////////////////////////////////////
///
///		File		: StateButton.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __STATEBUTTON_H__
#define __STATEBUTTON_H__
//-----------------------------------------------------------------------------
#include "Wnd.h"

#include "Button.h"

// PUSH BUTTON
class CStateButton : public CButton
{
public:
	CStateButton(){}
	CStateButton( SDesktop* DT );
	virtual ~CStateButton();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );
	virtual void RenderProc();
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );
	//-------------------------------------------------------------------------

	void InitStateButton( NCheckMark CheckMark, void* Caption );
	NCheckMark GetCheckMark();

	void SetChecked();
	void SetUnChecked();
	
	//by simwoosung
	void SetDirectCheck(bool bFlag);

protected:
	NCheckMark	CheckMark;

	int			NumList;
	WNDID*		Group;

	//by simwoosung
	bool		IsDirectCheck;		//직접 체크인 가능여부
	
	
	virtual void SetGroupWnd( int Count, WNDID* WndList );
	virtual void SetSelectedWnd();


	virtual void SetAdjustPosition( int PosX, int PosY );//OR

	virtual void OnMouseFocus( int Mx, int My );
	virtual void OnMouseLButtonDown( int Mx, int My );
	virtual void OnMouseLButtonPress( int Mx, int My );
	virtual void OnMouseLButtonUp( int Mx, int My );
	
};

//-----------------------------------------------------------------------------
#endif	__STATEBUTTON_H__