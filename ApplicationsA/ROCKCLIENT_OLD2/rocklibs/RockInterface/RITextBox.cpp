#include "..\\RockPCH.h"
#include "RITextBox.h"

#include "..\\..\\bravolibs\\obj\\EtcStruct.H"



CRITextBox::CRITextBox( SDesktop* DT ) : CTextBox(DT)
{
	m_IsMarkUp = FALSE;
}

CRITextBox::~CRITextBox()
{
	SAFE_DELETE_ARRAY( Text ); 
}

void CRITextBox::SetText( void* pStr )
{
	CHAR* Str = (CHAR*)(pStr);

	TextLength = Rstrlen( RWCHAR(Str));
	if( TextLength == 0 )
	{
		Rstrcpy( RWCHAR(Text), _RT("") );
		return;
	}

#ifdef UNICODE_ROCK
	if( TextLimitLen <= TextLength*2 )
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

	Rstrcpy( RWCHAR(Text), RWCHAR(Str) );

	GetTextLine();
	theFontg->GetRIStringInfo( Text, &StrInfo );
	
	
	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;
	Tw = Cw - ( BlankRight + BlankLeft );
	Th = Ch - ( BlankBottom + BlankTop );
	
	Ax = GetAlignPosX();
	Ay = GetAlignPosY();
}

void CRITextBox::RenderProc()
{
	UpdateHandler();
	
	Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );

	D3DCOLOR BTColor = D3DCOLOR_ARGB( Rcolor[A] , 0 , 0 , 0 );
	
	Render->RIDrawAlignText( theFontg, Text, Ax, Ay, Tx, Ty, Tw, Th, Tcolor, m_IsMarkUp );
	//Render->DrawAlignText( theFontg, Text, Ax, Ay, Tx, Ty, Tw, Th, Tcolor );
}