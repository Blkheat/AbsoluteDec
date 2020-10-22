///////////////////////////////////////////////////////////////////////////////
///
///		File		: InputDXDevice.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-02-18
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef	__INPUTDXDEVICE_H__
#define	__INPUTDXDEVICE_H__
///----------------------------------------------------------------------------

//#include "RockPCH.h"
#include <windows.h>
#include <dinput.h>


#define KEYDOWN(state,KeyID) (state[KeyID] & 0x80)
#define KEYDOWN_2KEYINPUT(state, KeyID1, KeyID2) ( (state[KeyID1]&0x80) && (state[KeyID2]&0x80) )

#define MOUSE_LBUTTON(State) (State.rgbButtons[0] & 0x80)
#define MOUSE_RBUTTON(State) (State.rgbButtons[1] & 0x80)

#define KeyState(buffer, x) ( ( buffer[x] & 0x80 ) ? true : false )


class CInputDXDevice
{
public:
	CInputDXDevice();
	~CInputDXDevice();

	bool Initialize( HINSTANCE hInst, HWND hWnd );
	void Release();

	char* GetKeyboardState();
	DIMOUSESTATE* GetMouseState();
	
	IDirectInputDevice8* GetKeyboardDevice() { return theDIKeyboardDevice;}
	IDirectInputDevice8* GetMouseDevice() { return theDIMouseDevice;}		

private:
	
	IDirectInput8*			theDInput;
	IDirectInputDevice8*	theDIKeyboardDevice;
	IDirectInputDevice8*	theDIMouseDevice;

	char					theKeyStateBuffer[256];
	DIMOUSESTATE			theMouseStateBuffer;

	bool ReadDevice( IDirectInputDevice8* aDInputDevice, void* DataBuffer, int BufferSize );

};

extern CInputDXDevice* InputDXDevice;

///----------------------------------------------------------------------------
#endif	__INPUTDXDEVICE_H__