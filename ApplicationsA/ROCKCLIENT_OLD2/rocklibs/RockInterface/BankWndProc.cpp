#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "BankWndProc.h"


CBankWnd	BankWnd;


//-----------------------------------------------------------------------------
void CBankWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CBankWnd::Composition()
{
}

//-----------------------------------------------------------------------------
void CBankWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CBankWnd::Proc( SEventMessage* EMsg )
{
	/*
	switch( EMsg->FocusWnd )
	{
	case WID_System_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_SystemMenuWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			gVisibleSystemWnd = false;
		}
		break;
	}
	//*/
}

//-----------------------------------------------------------------------------
void CBankWnd::SetWheelValue( short _wheel )
{
}

//-----------------------------------------------------------------------------
