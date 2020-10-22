///////////////////////////////////////////////////////////////////////////////
///
///		File		: CheckButton.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __CHECKBUTTON_H__
#define __CHECKBUTTON_H__
//-----------------------------------------------------------------------------
#include "Button.h"

class CCheckButton : public CButton
{
public:
	CCheckButton(){}
	CCheckButton( SDesktop* DT );
	virtual ~CCheckButton();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );//OR
	virtual void RenderProc();//OR
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );//OR
	//-------------------------------------------------------------------------

	void InitCheckButton( NCheckMark CheckMark, TCHAR* Caption );
	NCheckMark GetCheckMark();
	
	//by simwoosung
	void SetDirectCheck(bool bFlag);

protected:
	NCheckMark	CheckMark;
	
	//by simwoosung
	bool		IsDirectCheck;		//직접 체크인 가능여부	

	void SetChecked();
	void SetUnChecked();


	virtual void SetAdjustPosition( int PosX, int PosY );//OR

	virtual void OnMouseFocus( int Mx, int My );//OR
	virtual void OnMouseLButtonDown( int Mx, int My );//OR
	virtual void OnMouseLButtonPress( int Mx, int My );//OR
	virtual void OnMouseLButtonUp( int Mx, int My );//OR
};

//-----------------------------------------------------------------------------
#endif  __CHECKBUTTON_H__
