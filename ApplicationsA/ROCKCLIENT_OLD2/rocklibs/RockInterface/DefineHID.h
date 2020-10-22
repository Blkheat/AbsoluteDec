#ifndef __DEFINE_HID_H__
#define __DEFINE_HID_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"


#define HID_None			-1
#define HID_ITEMTIP			1024
#define HID_ITEMPREVIEW		1025


struct SHelpTip
{
	TCHAR		Desc[256];
	D3DCOLOR	Color;

	SHelpTip()
	{
		SAFE_STR_CPY( Desc, _RT(""), 256 );
		Color = D3DCOLOR_XRGB( 255, 255, 255 );
	}
};


//-----------------------------------------------------------------------------
#endif  __DEFINE_HID_H__