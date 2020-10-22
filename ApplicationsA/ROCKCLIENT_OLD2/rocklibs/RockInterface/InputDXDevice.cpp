///////////////////////////////////////////////////////////////////////////////
///
///		File		: InputDXDevice.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-02-18
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "..\\RockPCH.h"
#include "InputDXDevice.h"


CInputDXDevice* InputDXDevice;


//-----------------------------------------------------------------------------
CInputDXDevice::CInputDXDevice()
{
	theDInput			= NULL;
	theDIKeyboardDevice	= NULL;
	theDIMouseDevice	= NULL;

	memset( theKeyStateBuffer, 0, sizeof( char ) * 256 );
	memset( &theMouseStateBuffer, 0, sizeof( DIMOUSESTATE ) );
}

//-----------------------------------------------------------------------------
CInputDXDevice::~CInputDXDevice()
{
}

//-----------------------------------------------------------------------------
bool CInputDXDevice::Initialize( HINSTANCE hInst, HWND hWnd )
{
	//HRESULT hr;
	//GetModuleHandle( NULL )

	if( FAILED( DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&theDInput, NULL ) ) )
	{
		return FALSE;
	}
	
	// Create the device object
	if( FAILED( theDInput->CreateDevice( GUID_SysKeyboard, &theDIKeyboardDevice, NULL ) ) )
	{
		return FALSE;
	}
	// Set the data format
	if( FAILED( theDIKeyboardDevice->SetDataFormat( &c_dfDIKeyboard ) ) ) 
	{
		theDIKeyboardDevice->Release();
		return FALSE;
	}
	// Set the cooperative mode
	if( FAILED( theDIKeyboardDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) )
	{
		theDIKeyboardDevice->Release();
		return FALSE;
	}
	// Acquire the device for use
	theDIKeyboardDevice->Acquire();
//	if( FAILED( theDIKeyboardDevice->Acquire() ) )
//	{
//		theDIKeyboardDevice->Release();
//		return FALSE;
//	}
	
	// Create the device object
	if( FAILED( theDInput->CreateDevice( GUID_SysMouse, &theDIMouseDevice, NULL ) ) )
	{
		return FALSE;
	}
	// Set the data format
	if( FAILED( theDIMouseDevice->SetDataFormat( &c_dfDIMouse ) ) )
	{
		theDIMouseDevice->Release();
		return FALSE;
	}
	// Set the cooperative mode
	if( FAILED( theDIMouseDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) )
	{
		theDIMouseDevice->Release();
		return FALSE;
	}
	// Acquire the device for use
	theDIMouseDevice->Acquire();
//	if( FAILED( theDIMouseDevice->Acquire() ) )
//	{
//		theDIMouseDevice->Release(); 
//		return FALSE;
///	}

	return TRUE;
}

//-----------------------------------------------------------------------------
void CInputDXDevice::Release()
{
	if( theDIMouseDevice )
	{
		theDIMouseDevice->Unacquire();
		theDIMouseDevice->Release();
		theDIMouseDevice = NULL;
	}

	if( theDIKeyboardDevice )
	{
		theDIKeyboardDevice->Unacquire();
		theDIKeyboardDevice->Release();
		theDIKeyboardDevice = NULL;
	}

	if( theDInput )
	{
		theDInput->Release();
		theDInput = NULL;
	}
}

//-----------------------------------------------------------------------------
char* CInputDXDevice::GetKeyboardState()
{
	if( !ReadDevice( theDIKeyboardDevice, (void*)theKeyStateBuffer, 256 ) )
	{
		return NULL;
	}
	
	return theKeyStateBuffer;
}

//-----------------------------------------------------------------------------
DIMOUSESTATE* CInputDXDevice::GetMouseState()
{
	if( !ReadDevice( theDIMouseDevice, (void*)&theMouseStateBuffer, sizeof(DIMOUSESTATE) ) )
	{
		return NULL;
	}

	return &theMouseStateBuffer;
}

//-----------------------------------------------------------------------------
bool CInputDXDevice::ReadDevice( IDirectInputDevice8* aDInputDevice, void* aDataBuffer, int aBufferSize )
{
	HRESULT hr;

	while(1)
	{
		// Poll device
		//aDInputDevice->Poll();

		// Read in state
		hr = aDInputDevice->Poll();
		if( SUCCEEDED( hr = aDInputDevice->GetDeviceState( aBufferSize, (LPVOID)aDataBuffer ) ) )
		{
			break;
		}
		

		// Return on an unknown error  
		if( hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED )
		{
			return FALSE;
		}
		

		// Reacquire and try again
		if( FAILED( aDInputDevice->Acquire() ) )
		{
			return FALSE;
		}
		
	}

	// Return a success
	return TRUE;
}