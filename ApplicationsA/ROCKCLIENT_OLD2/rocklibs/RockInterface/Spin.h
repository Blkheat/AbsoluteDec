///////////////////////////////////////////////////////////////////////////////
///
///		File		: Spin.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __SPIN_H__
#define __SPIN_H__
//-----------------------------------------------------------------------------
#include "Wnd.h"

#include "Button.h"
#include "TextBox.h"


enum NSpinFocusPos
{
	n_spSpinClient,
	n_spUpBtnFocus,
	n_spDownBtnFocus,
};



class CSpin : public CTextBox
{
public:
	CSpin(){}
	CSpin( SDesktop* DT );
	virtual ~CSpin();

	//-------------------------------------------------------------------------
	virtual SEventMessage* EventProc( SEventMessage* EMsg );	
	virtual void RenderProc();

	virtual IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );
	virtual WNDID FocusCheck( int Mx, int My, bool IsChildCheck );
	//-------------------------------------------------------------------------

	void InitSpin( int Value, int Min, int Max, bool IsText, int al_bt_width =0 , int al_btheight =0);

	int GetValue();
	void SetValue( int Value );
	void SetValue( int Value, int Min, int Max );
	void SetValue( int Min, int Max );
	
	void SetUpHold( bool hold );
	void SetDownHold( bool hold );
	
	void SetUpBtnImage( TEXID Normal, TEXID Focus, TEXID Select, TEXID Hold );
	void SetDownBtnImage( TEXID Normal, TEXID Focus, TEXID Select, TEXID Hold );

protected:
	CButton*		theUpBtn;
	CButton*		theDownBtn;
	
	bool			theUpHold;
	bool			theDownHold;

	int				theValue;
	int				theMinValue;
	int				theMaxValue;
	bool			theIsText;

	NSpinFocusPos	theFocusPos;
	NSpinFocusPos	theFocusFixed;

	virtual void SetAdjustPosition( int PosX, int PosY );//OR
	virtual void OnMouseFocus( int Mx, int My );
	virtual void OnMouseLButtonDown( int Mx, int My );
	virtual void OnMouseLButtonPress( int Mx, int My );
	virtual void OnMouseLButtonUp( int Mx, int My );
};

//-----------------------------------------------------------------------------
#endif	__SPIN_H__