#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"




#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\bravolibs\\obj\\player.h"
//#include "..\\..\\Pc.h"

#include "SysMsgWndProc.h"


CSysMsgOutputWnd	SysMsgOutputWnd;




//-----------------------------------------------------------------------------
void CSysMsgOutputWnd::Init()
{
}

//-----------------------------------------------------------------------------
void CSysMsgOutputWnd::Composition()
{
	//-------------------------------------------------------------------------
	//
	//	채팅출력창
	//
	//-------------------------------------------------------------------------
	SysMsgWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_SysMsgOutputWnd, WID_None, 360, 92, 665, 6, false );
	SysMsgWnd->SetClientImage( TID_None );
	SysMsgWnd->SetCaption( _RT("SystemMessage Window") );
	SysMsgWnd->SetWndProc( this );
	SysMsgWnd->SetTitle( false );
	SysMsgWnd->SetVisible( true );
	SysMsgWnd->SetMove( false );
	SysMsgWnd->SetFocusCheck( false );	

	SysMsgWnd->SetZorder( n_zoBottom );
	SysMsgWnd->Initialize();

	SysMsgTxt = (CSysMsgOutput*)nRui->CreateWnd( n_wtSysMsgOutput, WID_SysMsgOutput_Text, WID_SysMsgOutputWnd, 0, 0, 20, 20, false );	
	SysMsgTxt->SetClientImage( TID_None );
	SysMsgTxt->SetSysMsgType( n_SystemMsg );
	SysMsgTxt->SetOutputStep( 0 );	
	SysMsgTxt->SetMaxLine( 3 );
	SysMsgTxt->SetVisible( true );

	AdminMsgTxt = (CSysMsgOutput*)nRui->CreateWnd( n_wtSysMsgOutput, WID_AdminMsgOutput_Text, WID_SysMsgOutputWnd, 0, 0, 20, 20, false );	
	AdminMsgTxt->SetClientImage( TID_None );
	AdminMsgTxt->SetSysMsgType( n_AdminMsg );
	AdminMsgTxt->SetOutputStep( 0 );		
	AdminMsgTxt->SetMaxLine( 1 );
	AdminMsgTxt->SetVisible( true );

	MapNameMsgTxt = (CSysMsgOutput*)nRui->CreateWnd( n_wtSysMsgOutput, WID_MapNameMsgOutput_Text, WID_SysMsgOutputWnd, 0, 0, 20, 20, false );	
	MapNameMsgTxt->SetClientImage( TID_None );
	MapNameMsgTxt->SetSysMsgType( n_MapNameMsg );
	MapNameMsgTxt->SetOutputStep( 0 );		
	MapNameMsgTxt->SetMaxLine( 1 );
	MapNameMsgTxt->SetVisible( true );


	return;
}

//-----------------------------------------------------------------------------
void CSysMsgOutputWnd::Update()
{
}

//-----------------------------------------------------------------------------
void CSysMsgOutputWnd::Proc( SEventMessage* EMsg )
{
}

void CSysMsgOutputWnd::ReSetMsgTxt()
{
	SysMsgTxt->ResetSysMsgOutput();		
	AdminMsgTxt->ResetSysMsgOutput();		
	MapNameMsgTxt->ResetSysMsgOutput();		

	return;
}





//-----------------------------------------------------------------------------



