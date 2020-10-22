
#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "Rui.h"
#include "PartyInforWnd.h"

#include "..\\..\\GTimer.h"

#include	"..\\..\\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>

#include	"..\\..\\Pc.h"
#include "..\\..\\global.h"

#include "UiUtil.h"
#include "LimitLenTextBox.h"

CLimitLenTextBox::CLimitLenTextBox( SDesktop* DT ) : CButton(DT)
{
	SAFE_STR_CPY( m_LimitTxt , _RT("") , MAX_SIZE_512 );
	m_bEqualString = true;
}

CLimitLenTextBox::~CLimitLenTextBox()
{
	SAFE_DELETE_ARRAY( Text );
}

SEventMessage* CLimitLenTextBox::EventProc( SEventMessage* EMsg )
{
	switch( EMsg->Mouse.Action )
	{
	   case MA_LDRAG:
		   {
			 if(Pid != WID_None && Pwnd)
			 {
				if( n_wtFrameWnd == Pwnd->GetWindowType() )			   
				{				
  					((CFrameWnd *)Pwnd)->SetFrameWndFocusPos(fwfNoneFocus);
					return Pwnd->EventProc(EMsg);
				}				
			 }
		   }		   
		   break;
	}
	
	return CButton::EventProc(EMsg);
}

void CLimitLenTextBox::RenderProc()
{
	UpdateHandler();
	
	if( IsImgBtn == false )
	{
		if( pDT->CurFocusWnd != Iid )
		{
			BtnState = n_bsNormal; 
		}
		if( IsHold )
		{
			BtnState = n_bsHold; 
		}
	}
	
	Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	Render->Draw( BtnImg[BtnState], Bx, By, Bw, Bh, Ccolor );
	
	Render->DrawAlignText( theFontg, m_LimitTxt, Ax, Ay, Tx, Ty, Tw, Th, Tcolor );

}

void CLimitLenTextBox::SetLimitLength(int nLength)
{
	SAFE_STR_CPY( m_LimitTxt ,  Text , MAX_SIZE_512 );	
	::ConvertStrInLimitLength(RWCHAR(m_LimitTxt), nLength);

	if( Rstrcmp(m_LimitTxt, Text) == 0 )
	{
		m_bEqualString = true;
	}
	else
	{
		m_bEqualString = false;
	}
}

void CLimitLenTextBox::SetText( void* pStr )
{
	char*Str = RCHAR(pStr); 
	TextLength = Rstrlen( Str );
	if( TextLength == 0 )
	{
		Rstrcpy(RWCHAR(Text), _RT("") );
		return;
	}
#ifdef UNICODE_ROCK
	if( TextLimitLen <= (TextLength*2) )
	{
		SAFE_DELETE_ARRAY( Text );	
		
		TextLimitLen = (TextLength + 1)*2;
		Text = SAFE_NEW_ARRAY( TCHAR , TextLimitLen ); 		
	}
#else
	if( TextLimitLen <= TextLength )
	{
		SAFE_DELETE_ARRAY( Text );	
		
		TextLimitLen = TextLength + 1;
		Text = SAFE_NEW_ARRAY( TCHAR , TextLimitLen ); 		
	}
#endif
	
	Rstrcpy(RWCHAR(Text), RWCHAR(Str) );

	GetTextLine();
	theFontg->GetStringInfo( Text, &StrInfo );
	
	
	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;
	Tw = Cw - ( BlankRight + BlankLeft );
	Th = Ch - ( BlankBottom + BlankTop );
	
	Ax = GetAlignPosX();
	Ay = GetAlignPosY();

	Rstrcpy(RWCHAR(m_LimitTxt), RWCHAR(Text) );	
}