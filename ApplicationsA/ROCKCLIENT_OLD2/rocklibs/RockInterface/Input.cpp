///////////////////////////////////////////////////////////////////////////////
///
///		File		: Input.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-02-18
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#define _WIN32_WINNT 0x500

#include "Input.h"
#include "InputDXDevice.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"


CInput*	Inputg = NULL;
extern BOOL	g_HoldGame;


const int c_DubleClick_Time( 300 );
bool g_IsLButtonDown = false;
DWORD g_PressTime = timeGetTime();



//-----------------------------------------------------------------------------
CInput::CInput( HINSTANCE hInst, HWND hWnd, int Cwidth, int Cheight )
{
	thehInst	= hInst;
	thehWnd		= hWnd;

	theCwidth	= Cwidth;
	theCheight	= Cheight;

	memset( theOldKeys, 0, sizeof( bool ) * 256 );
	memset( theCurrentKeys, 0, sizeof( bool ) * 256 );
	memset( theKeys, 0, sizeof( SKeys ) * 256 );
	memset( &theOldMouse, 0, sizeof( SMouse ) );
	memset( &theMouse, 0, sizeof( SMouse ) );

	theOldMouse.State		= n_msStop;
	theMouse.State			= n_msStop;
	

	POINT	Position;
	GetCursorPos( &Position );
	ScreenToClient( thehWnd, &Position );
	
	theOldMouse.Aposition.x	= Position.x;
	theOldMouse.Aposition.y	= Position.y;
	theMouse.Aposition.x	= Position.x;
	theMouse.Aposition.y	= Position.y;
	
	theIsMoveWheel			= false;
	theIsOldLeftButtonDown	= false;
	theIsOldRightButtonDown	= false;
	theIsLeftButtonDown		= false;
	theIsRightButtonDown	= false;
	

	InputDXDevice = SAFE_NEW( CInputDXDevice );	
	if( !InputDXDevice->Initialize( hInst, hWnd ) )
		MessageBox( NULL, "!D3DInput장치 설정 실패", "에러", MB_OK );
}

//-----------------------------------------------------------------------------
CInput::~CInput()
{
}

//-----------------------------------------------------------------------------
void CInput::MsgProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	int MousePositionX( 0 );
	int MousePositionY( 0 );
	
	
	switch( Msg )
	{
		case WM_MOUSEWHEEL:
			SetMouseWheel( HIWORD( wParam ) );
			break;
		case WM_RBUTTONDBLCLK:
			theRDblClick = true;
			break;

#ifdef TEST_GAME_NECK
		case WM_LBUTTONDBLCLK:
			{
				theLDblClick = true;				
			}	
			break;			
		case WM_RBUTTONUP:
			{
				if( g_RockClient.GetGameMode() == GMODE_NORMAL )
				{
					g_HoldGame =!g_HoldGame;
				}			
			}
			break;
#else
		case WM_LBUTTONDBLCLK:
			theLDblClick = true;
			break;
#endif


/*		case WM_MOUSEMOVE:
			MousePositionX = LOWORD( lParam );
			MousePositionY = HIWORD( lParam );
			SetMouseMove( MousePositionX, MousePositionY );
			break;

		case WM_LBUTTONDOWN:
			SetLeftButtonDown();
			if( GetCapture() == NULL )
			{
				SetCapture( hWnd );
			}
			break;

		case WM_LBUTTONUP:
			if( GetCapture() != NULL )
			{
				ReleaseCapture();
			}
			SetLeftButtonUp();
			break;

		case WM_MBUTTONDOWN:
			SetMiddleButtonDown();
			if( GetCapture() == NULL )
			{
				SetCapture( hWnd );
			}
			break;

		case WM_MBUTTONUP:
			if( GetCapture() != NULL )
			{
				ReleaseCapture();
			}
			SetMiddleButtonUp();
			break;

		case WM_RBUTTONDOWN:
			SetRightButtonDown();
			if( GetCapture() == NULL )
			{
				SetCapture( hWnd );
			}
			break;

		case WM_RBUTTONUP:
			if( GetCapture() != NULL )
			{
				ReleaseCapture();
			}
			SetRightButtonUp();
			break;

		case WM_KEYDOWN:
			SetKeyDown( wParam );
			break;

		case WM_KEYUP:
			SetKeyUp( wParam );
			break;

		case WM_SYSKEYDOWN:
			break;

		case WM_SYSKEYUP:
			break;
			//*/
	}
}

//-----------------------------------------------------------------------------
void CInput::AcquireEvent( SKeys* aKeyEvt, SMouse* aMouseEvt )
{
	InputDXDevice->GetKeyboardDevice()->Acquire();
	InputDXDevice->GetMouseDevice()->Acquire();

	SetKeyStateDInput();
	SetMouseStateDInput();

	if( theIsMoveWheel )
	{
		theMouse.State = n_msWheel;
	}

	memcpy( aKeyEvt, theKeys, sizeof( SKeys ) * 256 );
	memcpy( aMouseEvt, &theMouse, sizeof( SMouse ) );

	theIsMoveWheel		= false;
	theLDblClick		= false;
	theRDblClick		= false;
}

void CInput::Acquire()
{
	if( InputDXDevice )
	{
		if( InputDXDevice->GetKeyboardDevice() )
		{
			InputDXDevice->GetKeyboardDevice()->Acquire();
		}
		if( InputDXDevice->GetMouseDevice() )
		{
			InputDXDevice->GetMouseDevice()->Acquire();
		}
	}
	return;
}

//-----------------------------------------------------------------------------
void CInput::ResetAposition( int x, int y )
{
	theMouse.Aposition.x = x;
	theMouse.Aposition.y = y;
}

//-----------------------------------------------------------------------------
void CInput::SetKeyDown( UINT aKey )
{
	theCurrentKeys[aKey] = true;
}

//-----------------------------------------------------------------------------
void CInput::SetKeyUp( UINT aKey )
{
	theCurrentKeys[aKey] = false;
}

//-----------------------------------------------------------------------------
void CInput::SetLeftButtonDown()
{
	theIsLeftButtonDown = true;
}

//-----------------------------------------------------------------------------
void CInput::SetLeftButtonUp()
{
	theIsLeftButtonDown = false;
}

//-----------------------------------------------------------------------------
void CInput::SetMiddleButtonDown()
{
}

//-----------------------------------------------------------------------------
void CInput::SetMiddleButtonUp()
{
}

//-----------------------------------------------------------------------------
void CInput::SetRightButtonDown()
{
	theIsRightButtonDown = true;
}

//-----------------------------------------------------------------------------
void CInput::SetRightButtonUp()
{
	theIsRightButtonDown = false;
}

//-----------------------------------------------------------------------------
void CInput::SetMouseMove( int aMousePositionX, int aMousePositionY )
{
	theMouse.Aposition.x = aMousePositionX;
	theMouse.Aposition.y = aMousePositionY;
}

//-----------------------------------------------------------------------------
void CInput::SetMouseWheel( short aWheelValue )
{
	theMouse.WheelValue = aWheelValue;
	theIsMoveWheel = true;
}

//-----------------------------------------------------------------------------
void CInput::SetKeyStateWndMessage()
{
	NButtonAction	KeyAction( n_baNone );

	for( int i = 0; i < 256; ++i )
	{
		if( ( theOldKeys[i] == false ) && ( theCurrentKeys[i] == false ) )
		{
			KeyAction = n_baNone;
		}
		else if( ( theOldKeys[i] == false ) && ( theCurrentKeys[i] == true ) )
		{
			KeyAction = n_baDown;
		}
		else if( ( theOldKeys[i] == true ) && ( theCurrentKeys[i] == false ) )
		{
			KeyAction = n_baUp;
		}
		else if( ( theOldKeys[i] == true ) && ( theCurrentKeys[i] == true ) )
		{
			KeyAction = n_baPress;
		}

		theKeys[i].Action = KeyAction;
	}

	memcpy( theOldKeys, theCurrentKeys, sizeof( bool ) * 256 );
}

//-----------------------------------------------------------------------------
void CInput::SetMouseStateWndMessage()
{
	theMouse.State = n_msStop;
	theMouse.Rposition.x = 0;
	theMouse.Rposition.y = 0;

	if( ( theOldMouse.Aposition.x != theMouse.Aposition.x ) ||
		( theOldMouse.Aposition.y != theMouse.Aposition.y )		)
	{
		theMouse.Rposition.x = theMouse.Aposition.x - theOldMouse.Aposition.x;
		theMouse.Rposition.y = theMouse.Aposition.y - theOldMouse.Aposition.y;
		theMouse.State = n_msMove;
	}
	
	if( ( theIsOldLeftButtonDown == false ) && ( theIsLeftButtonDown == false ) )
	{
		theMouse.LButton = n_baNone;
	}
	if( ( theIsOldLeftButtonDown == false ) && ( theIsLeftButtonDown == true ) )
	{
		theMouse.LButton = n_baDown;
	}
	if( ( theIsOldLeftButtonDown == true ) && ( theIsLeftButtonDown == false ) )
	{
		theMouse.LButton = n_baUp;
	}
	if( ( theIsOldLeftButtonDown == true ) && ( theIsLeftButtonDown == true ) )
	{
		theMouse.LButton = n_baPress;
	}

	if( ( theIsOldRightButtonDown == false ) && ( theIsRightButtonDown == false ) )
	{
		theMouse.RButton = n_baNone;
	}
	if( ( theIsOldRightButtonDown == false ) && ( theIsRightButtonDown == true ) )
	{
		theMouse.RButton = n_baDown;
	}
	if( ( theIsOldRightButtonDown == true ) && ( theIsRightButtonDown == false ) )
	{
		theMouse.RButton = n_baUp;
	}
	if( ( theIsOldRightButtonDown == true ) && ( theIsRightButtonDown == true ) )
	{
		theMouse.RButton = n_baPress;
	}
	
	theIsOldLeftButtonDown	= theIsLeftButtonDown;
	theIsOldRightButtonDown	= theIsRightButtonDown;
	memcpy( &theOldMouse, &theMouse, sizeof( SMouse ) );
}

//-----------------------------------------------------------------------------
void CInput::SetKeyStateDInput()
{
	NButtonAction	KeyAction( n_baNone );
	char*	TempKeysBufPtr = NULL; //<- ( NULL ) 로 초기화를 하면...에러가 나는가?....흠흠

	TempKeysBufPtr = InputDXDevice->GetKeyboardState();
	if( TempKeysBufPtr == NULL )
	{
		return;
	}

	int i( 0 );
	for( i = 0; i < 256; ++i )
	{
		( TempKeysBufPtr[i] & 0x80 ) ? theCurrentKeys[i] = true : theCurrentKeys[i] = false;
	}

	for( i = 0; i < 256; ++i )
	{
		if( ( theOldKeys[i] == false ) && ( theCurrentKeys[i] == false ) )
		{
			KeyAction = n_baNone;
		}
		else if( ( theOldKeys[i] == false ) && ( theCurrentKeys[i] == true ) )
		{
			KeyAction = n_baDown;
		}
		else if( ( theOldKeys[i] == true ) && ( theCurrentKeys[i] == false ) )
		{
			KeyAction = n_baUp;
		}
		else if( ( theOldKeys[i] == true ) && ( theCurrentKeys[i] == true ) )
		{
			KeyAction = n_baPress;
		}

		theKeys[i].Action = KeyAction;
	}

	memcpy( theOldKeys, theCurrentKeys, sizeof( bool ) * 256 );
}

//-----------------------------------------------------------------------------
void CInput::SetMouseStateDInput()
{
	DIMOUSESTATE*	TempMouseBufPtr = NULL;
	TempMouseBufPtr = InputDXDevice->GetMouseState();

	if( TempMouseBufPtr == NULL )
	{
		return;
	}

	theMouse.State = n_msStop;
	
	POINT	Position;
	GetCursorPos( &Position );
	ScreenToClient( thehWnd, &Position );

	if( Position.x <= 0 )
	{
		Position.x = 0;
	}
	if( Position.x >= theCwidth )
	{
		Position.x = theCwidth;
	}
	if( Position.y <= 0 )
	{
		Position.y = 0;
	}
	if( Position.y >= theCheight )
	{
		Position.y = theCheight;
	}

	theMouse.Aposition.x = Position.x;
	theMouse.Aposition.y = Position.y; 
	theMouse.Rposition.x = theMouse.Aposition.x - theOldMouse.Aposition.x;
	theMouse.Rposition.y = theMouse.Aposition.y - theOldMouse.Aposition.y;
	
	( TempMouseBufPtr->rgbButtons[0] & 0x80 ) ? theIsLeftButtonDown = true : theIsLeftButtonDown = false;//LBUTTON
	( TempMouseBufPtr->rgbButtons[1] & 0x80 ) ? theIsRightButtonDown = true : theIsRightButtonDown = false;//RBUTTON


	if( ( theOldMouse.Aposition.x != theMouse.Aposition.x ) ||
		( theOldMouse.Aposition.y != theMouse.Aposition.y )		)
	{
		theMouse.State = n_msMove;
	}
	

	int nowtime = timeGetTime();

	if( ( theIsOldLeftButtonDown == false ) && ( theIsLeftButtonDown == false ) )
	{
		theMouse.LButton	= n_baNone;
		theMouse.uiLButton	= n_baNone;
				
//		if( nowtime >= c_DubleClick_Time + g_PressTime + 10000 )
//		{
//			theMouse.LButton = n_baNone;
///			g_IsLButtonDown = false;
//		}
//		else if( g_IsLButtonDown && ( nowtime >= c_DubleClick_Time + g_PressTime ) )
//		{
//			theMouse.LButton = n_baDown;		
//			g_IsLButtonDown = false;
//		}
	}
	if( ( theIsOldLeftButtonDown == false ) && ( theIsLeftButtonDown == true ) )
	{	
		theMouse.uiLButton	= n_baDown;

//		if( g_IsLButtonDown )		// 눌려진 상태 
//		{
//			if( nowtime < c_DubleClick_Time + g_PressTime )
//			{
//				theMouse.LButton = n_baDown; // n_baNone; // n_baDblClick;		// 더블 클릭은 무시!! 
//				g_IsLButtonDown = false;
//			}
//		}
//		else					// 눌려지지 않은 상태
//		{			
			theMouse.LButton = n_baDown;							// 더블클릭을 사용하면 나중에 판단하므로 주석!! 	
//			g_IsLButtonDown = true;
//			g_PressTime = nowtime;
//		}
	}
	if( ( theIsOldLeftButtonDown == true ) && ( theIsLeftButtonDown == false ) )
	{
		theMouse.LButton	= n_baUp;
		theMouse.uiLButton	= n_baUp;
	}
	if( ( theIsOldLeftButtonDown == true ) && ( theIsLeftButtonDown == true ) )
	{
		theMouse.LButton	= n_baPress;
		theMouse.uiLButton	= n_baPress;
	}
//	if( theLDblClick == true )
//	{
//		theMouse.LButton = n_baDblClick;
//	}


	if( ( theIsOldRightButtonDown == false ) && ( theIsRightButtonDown == false ) )
	{
		theMouse.RButton	= n_baNone;
		theMouse.uiRButton	= n_baNone;
	}
	if( ( theIsOldRightButtonDown == false ) && ( theIsRightButtonDown == true ) )
	{
		theMouse.RButton	= n_baDown;
		theMouse.uiRButton	= n_baDown;
	}
	if( ( theIsOldRightButtonDown == true ) && ( theIsRightButtonDown == false ) )
	{
		theMouse.RButton	= n_baUp;
		theMouse.uiRButton	= n_baUp;
	}
	if( ( theIsOldRightButtonDown == true ) && ( theIsRightButtonDown == true ) )
	{
		theMouse.RButton	= n_baPress;
		theMouse.uiRButton	= n_baPress;
	}
//	if( theRDblClick == true )
//	{
//		theMouse.RButton = n_baDblClick;
//	}
	
	
	theIsOldLeftButtonDown	= theIsLeftButtonDown;
	theIsOldRightButtonDown	= theIsRightButtonDown;

	memcpy( &theOldMouse, &theMouse, sizeof( SMouse ) );

	//by simwoosung
	/*if(TempMouseBufPtr->lZ != 0)
	{
		SetMouseWheel( TempMouseBufPtr->lZ );
	}*/	
}

//-----------------------------------------------------------------------------