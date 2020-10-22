///////////////////////////////////////////////////////////////////////////////
///
///		File		: RadioButton.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __RADIOBUTTON_H__
#define __RADIOBUTTON_H__
//-----------------------------------------------------------------------------
#include "Button.h"

class CRadioButton : public CButton
{
public:
	CRadioButton(){}
	CRadioButton( SDesktop* DT );
	virtual ~CRadioButton();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );
	virtual void RenderProc();
	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );
	//-------------------------------------------------------------------------

	void InitRadioButton( NCheckMark CheckMark, void* Caption );
	NCheckMark GetCheckMark();
	
	//by simwoosung
	void SetDirectCheck(bool bFlag);


protected:
	NCheckMark	CheckMark;

	int			NumList;
	WNDID*		Group;

	//by simwoosung
	bool		IsDirectCheck;		//직접 체크인 가능여부

	void SetChecked();
	void SetUnChecked();
	
	virtual void SetGroupWnd( int Count, WNDID* WndList );
	virtual void SetSelectedWnd();


	virtual void SetAdjustPosition( int PosX, int PosY );//OR

	virtual void OnMouseFocus( int Mx, int My );
	virtual void OnMouseLButtonDown( int Mx, int My );
	virtual void OnMouseLButtonPress( int Mx, int My );
	virtual void OnMouseLButtonUp( int Mx, int My );
};

//-----------------------------------------------------------------------------
#endif  __RADIOBUTTON_H__
