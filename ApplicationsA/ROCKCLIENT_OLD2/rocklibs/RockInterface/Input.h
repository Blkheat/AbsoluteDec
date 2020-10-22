///////////////////////////////////////////////////////////////////////////////
///
///		File		: Input.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-02-18
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef	__INPUT_H__
#define	__INPUT_H__
///----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#include <dinput.h>

#include "Define.h"







class CInput
{
public:
	CInput(){};
	CInput( HINSTANCE hInst, HWND hWnd, int Cwidth, int Cheight );
	~CInput();

	void MsgProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
	void AcquireEvent( SKeys* aKeyEvt, SMouse* aMouseEvt );
	void Acquire();
	void ResetAposition( int x, int y );

private:
	void SetKeyDown( UINT aKey );
	void SetKeyUp( UINT aKey );
	void SetLeftButtonDown();
	void SetLeftButtonUp();
	void SetMiddleButtonDown();
	void SetMiddleButtonUp();
	void SetRightButtonDown();
	void SetRightButtonUp();
	void SetMouseMove( int aMousePositionX, int aMousePositionY );
	void SetMouseWheel( short aMoveWheelValue );

	//-------------------------------------------------------------------------
	// 지금은 시간이 없어서리...단순 처리...-.-;...
	//-------------------------------------------------------------------------
	void SetKeyStateWndMessage();
	void SetMouseStateWndMessage();

	void SetKeyStateDInput();
	void SetMouseStateDInput();
	//-------------------------------------------------------------------------

	bool	theOldKeys[256];
	bool	theCurrentKeys[256];
	SKeys	theKeys[256];

	bool	theIsMoveWheel;
	bool	theIsOldLeftButtonDown;
	bool	theIsOldRightButtonDown;
	bool	theIsLeftButtonDown;
	bool	theIsRightButtonDown;

	SMouse	theOldMouse;
	SMouse	theMouse;

private:
	HINSTANCE	thehInst;
	HWND		thehWnd;

	int			theCwidth;
	int			theCheight;
	
	bool		theLDblClick;
	bool		theRDblClick;
};


extern DWORD g_PressTime;
extern CInput*	Inputg;
//----------------------------------------------------------------------------
#endif	__INPUT_H__