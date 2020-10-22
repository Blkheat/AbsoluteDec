///////////////////////////////////////////////////////////////////////////////
///
///		File		: ChatOutput.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "..\\RockPCH.h"
#include "ChatOutput.h"



//-----------------------------------------------------------------------------
CChatOutput::CChatOutput( SDesktop* DT ) : CWnd( DT )
{
	theFontg	= Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );

//	for( int i = 0; i < MAX_CHATITEM; ++i )
//	{
//		theChatItem[i] = NULL;
//	}

	memset( &theChatItem, 0, sizeof(SChatItem)*MAX_CHATITEM );
	
	theCurLine		= 0;
	theNumLine		= 0;
	theLimitLine	= 0;
	theOutputLine	= 1;
	theMaxLine		= 0;
}

//-----------------------------------------------------------------------------
CChatOutput::~CChatOutput()
{
}

//-----------------------------------------------------------------------------
void CChatOutput::RenderProc()
{
//	Render->Draw( Cimg, Cx, Cy, Csize[2], Csize[3], D3DCOLOR_ARGB( 189, 53, 42, 18 ) ); 
	Render->Draw( Cimg, Cx, Cy, Csize[2], Csize[3], D3DCOLOR_ARGB( 189, 74, 81, 94 ) ); 


	int x = Cx + 4;
	int y = Ch - 12;
	int line = theCurLine;

	
	for( int i = 0; i < theOutputLine; ++i )
	{
//		if( ( line >= 0 ) && ( theChatItem[line] != NULL ) )
		if( ( line >= 0 ) && ( line <= MAX_CHATITEM ) && ( theChatItem[line].Color != 0 ) )
		{
			Render->DrawText( theFontg, theChatItem[line].Text, x, y - ( i * 12 ), theChatItem[line].Color );
			line--;
		}
	}
}

//-----------------------------------------------------------------------------
void CChatOutput::SetChatItem( void* pstr, D3DCOLOR color )
{
	char *str = (char*)pstr;
	
	int len = Rstrlen( str );
	if( len != 0 )
	{
	
#ifdef UNICODE_ROCK
		wchar_t tstr[64];
		wchar_t	cset[] = L"\n";
		wchar_t*p = NULL;

		SAFE_STR_CPY( tstr, str, 128 );
		p = (wchar_t*)Rstrtok( tstr, cset );
		AddChatItem( RCHAR(p), color );
		
		while( ( p = wcstok( NULL , cset ) ) )
		{
			AddChatItem( RCHAR(p), color );
		}

#else
		TCHAR	tstr[128] = {0,};
		char	cset[] = _T("\n");
		char*	p = NULL;

		SAFE_STR_CPY( tstr, str, 128 );
		p = Rstrtok( tstr, cset );
		AddChatItem( RCHAR(p), color );

		
		while( ( p = Rstrtok( NULL , cset ) ) )
		{
			AddChatItem( p, color );
		}
#endif
	
		
	}

	//라인 맞추기.... 원래 여기서 하면 안되지만
	//스크롤바와의 연동이 없어졌으므로...기냥 씀....
	SetLineInit();
}

//-----------------------------------------------------------------------------
void CChatOutput::AddChatItem( TCHAR* str, D3DCOLOR color )
{
	SChatItem Item;

	SAFE_STR_CPY( Item.Text, str, 256 );
	Item.Color= color;

	if( theLimitLine >= MAX_CHATITEM )
	{
		SAFE_STR_CPY( theChatItem[0].Text, _RT(""), 256 );
		theChatItem[0].Color = 0;

		for( int i = 0; i < (MAX_CHATITEM - 1); ++i )
		{
			theChatItem[i] = theChatItem[i + 1];
		}

		theChatItem[theLimitLine - 1] = Item;
	}
	else
	{
		theChatItem[theLimitLine] = Item;
		theLimitLine++;
		theNumLine = theLimitLine - 1;
	}
}

//-----------------------------------------------------------------------------
void CChatOutput::SetOutputLevel( int level )
{
	switch( level )
	{
	case 0://1단계
		theOutputLine = 4;
		break;
	case 1://2단계
		theOutputLine = 8;
		break;
	case 2://3단계
		theOutputLine = 16;
		break;
	default:
		theOutputLine = 8;
	}
	
	ResetSize();
}

//-----------------------------------------------------------------------------
void CChatOutput::SetOutputStep( int step )
{
	theOutputLine	= step;

	if( (theMaxLine != 0 ) && ( theOutputLine >= theMaxLine ) )
	{
		theOutputLine = theMaxLine;
	}

	ResetSize();
}

//-----------------------------------------------------------------------------
void CChatOutput::InitChatOutput()
{
	ResetSize();
}

//-----------------------------------------------------------------------------
void CChatOutput::ResetSize()
{
	CWnd* Pwnd = GetParentHandle();

	SRect pCsize;
	Pwnd->GetClientSize( &pCsize );

	int tmpWidth	= pCsize.x + pCsize.w;
	int tmpHeight	= pCsize.y + pCsize.h;

	Cx = pCsize.x;
	Cy = tmpHeight - ( theOutputLine * 12 );
	Cw = tmpWidth;
	Ch = tmpHeight;

	Csize[2] = pCsize.w;
	Csize[3] = Ch - Cy;

	if( Iid == WID_ChatOutputWnd )
	{
		//pCsize.x = Cx;
		pCsize.y = Cy - 12;//upframe 뚜께....
		pCsize.w = pCsize.w;
		pCsize.h = Csize[3] + 12;//Cy값을 2만큼 올렸기 때문에....다시 내린다.
	}
	else
	{
		//pCsize.x = Cx;
		pCsize.y = Cy;
		pCsize.w = pCsize.w;
		pCsize.h = Csize[3];
	}
	
	Pwnd->ResetSize( &pCsize );
}

//-----------------------------------------------------------------------------
void CChatOutput::SetLineInit()
{
	theCurLine = theNumLine;
}

//-----------------------------------------------------------------------------
void CChatOutput::SetCurLine( int curline )
{
	theCurLine = curline;
}

//-----------------------------------------------------------------------------
int CChatOutput::GetNumLine()
{
	return( theNumLine );
}

//-----------------------------------------------------------------------------
int CChatOutput::GetCurLine()
{
	return( theCurLine );
}

//-----------------------------------------------------------------------------
void CChatOutput::SetMaxLine( int max_line )
{
	theMaxLine = max_line;
}

//-----------------------------------------------------------------------------

void CChatOutput::ResetChatOutput()
{
	for( int i = 0; i < MAX_CHATITEM; ++i )
	{
		SAFE_STR_CPY( theChatItem[i].Text, _RT(""), 256 );
		theChatItem[i].Color = 0;
	}

//	memset( &theChatItem, 0, sizeof(SChatItem)*MAX_CHATITEM );

	return;
}
