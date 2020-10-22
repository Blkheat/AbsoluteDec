///////////////////////////////////////////////////////////////////////////////
///
///		File		: SysMsgOutput.cpp
///		Desc		:
///
///		Author		: Niceguy
///		Team		: Program - Client Team
///		Date		: 2005-01-06
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "..\\..\\RockClient.h"
#include "..\\..\\brother.h"

#include "SysMsgOutput.h"




extern	long g_nowTime;


//-----------------------------------------------------------------------------
CSysMsgOutput::CSysMsgOutput( SDesktop* DT ) : CWnd( DT )
{
	memset( &theSystemMsg, 0, sizeof(SSystemMsg)*MAX_SYSTEMMSG );

	theCurLine		= 0;
	
	theLimitLine	= 0;
	theOutputLine	= 0;
	theMaxLine		= 0;

	theMsgType		= n_SystemMsg;	
}

//-----------------------------------------------------------------------------
CSysMsgOutput::~CSysMsgOutput()
{
}

void CSysMsgOutput::SetSysMsgType( NSysMsgType type )
{
	theMsgType = type;	

	switch( theMsgType )
	{
	case n_SystemMsg:
		{
			theFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwMedium, n_fdShadow );			
		}
		break;
	case n_AdminMsg:
		{
			theFontg = Fontman->GetFont( n_ftGulimChe, 13, n_fwBold, n_fdShadow );
		}
		break;
	case n_MapNameMsg:
		{
			theFontg = Fontman->GetFont( n_ftDotum, 22, n_fwBold, n_fdOutline );
		}
		break;
	default:
		{
			theFontg = Fontman->GetFont( n_ftGulimChe, 13, n_fwMedium, n_fdShadow );
		}
		break;
	}
	return;
}

//-----------------------------------------------------------------------------
void CSysMsgOutput::RenderProc()
{
	switch( theMsgType )
	{
	case n_SystemMsg:
		{			
			if( theOutputLine && SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( theSystemMsg[theOutputLine-1].Time , 60000 ) ) )
			{
				theSystemMsg[theOutputLine-1].Time = g_nowTime;
				SetOutputStep( theOutputLine - 1 );						
			}
			int x = Cx + 4;
			int y = Ch + (theOutputLine * 15);
			
			for( int i = 0; i < theOutputLine; ++i )
			{	
				int length = Rstrlen( RWCHAR(theSystemMsg[i].Text) );
				Render->DrawText( theFontg, theSystemMsg[i].Text, 512 - ((length * 6) / 2), y - ( i * 15 ), theSystemMsg[i].Color );						
			}
		}
		break;
	case n_AdminMsg:
		{
			if( theOutputLine && SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( theSystemMsg[theOutputLine-1].Time , 100000 ) ) )
			{
				theSystemMsg[theOutputLine-1].Time = g_nowTime;
				SetOutputStep( theOutputLine - 1 );						
			}

			for( int i = 0; i < theOutputLine; ++i )
			{	
				int length = Rstrlen( RWCHAR(theSystemMsg[i].Text) );
				Render->DrawText( theFontg, theSystemMsg[i].Text, 512 - ((length * 6) / 2), Ch - 3, theSystemMsg[i].Color );						
			}
		}
		break;
	case n_MapNameMsg:
		{
			int nIndex = theOutputLine - 1;
			if(nIndex == -1)
				nIndex = 0;
			
			D3DCOLOR msg_color = theSystemMsg[nIndex].Color;

			if( theOutputLine )
			{
				if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( theSystemMsg[theOutputLine-1].Time , 7000 ) ) )
				{
					theSystemMsg[theOutputLine-1].Time = g_nowTime;
					SetOutputStep( theOutputLine - 1 );						
				}			
				else if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( theSystemMsg[theOutputLine-1].Time , 6000 ) ) )
				{
					long time = SAFE_TIME_SUB( g_nowTime , SAFE_TIME_ADD( theSystemMsg[theOutputLine-1].Time , 6000 ) );
					float msg_alpha = 1.0f - (time / 1000.0f);
					
					DWORD color = theSystemMsg[theOutputLine-1].Color;

					int colorA, colorR, colorG, colorB;
					colorA = ( color >> 24 );
					colorR = ( ( color >> 16) & 0xff ); 
					colorG = ( ( color >> 8 ) & 0xff ); 
					colorB = ( color & 0xff ); 

					msg_color = D3DCOLOR_ARGB( (BYTE)(msg_alpha * 255), colorR, colorG, colorB );
				}
			}
			
			int y = Ch + 60;
			
			for( int i = 0; i < theOutputLine; ++i )
			{	
				int length = Rstrlen( RWCHAR(theSystemMsg[i].Text) );
				Render->DrawText( theFontg, theSystemMsg[i].Text, 512 - ((length * 16) / 2), y, msg_color );						
			}
		}
		break;
	default:
		break;
	}	
	return;
}

//-----------------------------------------------------------------------------
void CSysMsgOutput::SetSystemMsg( TCHAR* str, D3DCOLOR color )
{	
	int len = Rstrlen( str );
	if( len != 0 )
	{
		
#ifdef UNICODE_ROCK
		wchar_t tstr[128] = _RT("");
		wchar_t cset[] = _RT("\n");
		wchar_t *p = NULL;
		
		wcscpy( tstr, RWCHAR(str) );
		p = wcstok( tstr, cset );
		AddSystemMsg( RCHAR(p), color );
		
		while( ( p =wcstok( NULL, cset ) ) )
		{
			AddSystemMsg( RCHAR(p), color );
		}
#else
		
		char tstr[128] = {0,};
		char cset[] = "\n";
		char *	p = NULL;
		
		strcpy( tstr, str );
		p = strtok( tstr, cset );
		AddSystemMsg( p, color );
		
		while( ( p =Rstrtok( NULL, cset ) ) )
		{
			AddSystemMsg( p, color );
		}
	
#endif

	}
	return;
}

//-----------------------------------------------------------------------------
void CSysMsgOutput::AddSystemMsg( TCHAR* str, D3DCOLOR color )
{
	SSystemMsg Item;

	switch( theMsgType )
	{
	case n_SystemMsg:
		{
			Rstrcpy( Item.Text, str );
			Item.Color= color;
			
			for( int i = MAX_SYSTEMMSG - 1; i > 0; --i )
			{			
				theSystemMsg[i] = theSystemMsg[i - 1];
			}
			theSystemMsg[0] = Item;
			theSystemMsg[0].Time = g_nowTime;
			
			SetOutputStep( theOutputLine + 1 );
		}
		break;
	case n_AdminMsg:
		{
			Rstrcpy( Item.Text, str );
			Item.Color= color;
			
			theSystemMsg[0] = Item;
			theSystemMsg[0].Time = g_nowTime;

			SetOutputStep( 1 );
		}
		break;
	case n_MapNameMsg:
		{
			Rstrcpy( Item.Text, str );
			Item.Color= color;
			
			theSystemMsg[0] = Item;
			theSystemMsg[0].Time = g_nowTime;

			SetOutputStep( 1 );
		}
		break;
	
	default:
		break;
	}

	return;
}


//-----------------------------------------------------------------------------
void CSysMsgOutput::SetOutputStep( int step )
{
	theOutputLine	= step;
	
	if( (theMaxLine != 0 ) && ( theOutputLine >= theMaxLine ) )
	{
		theOutputLine = theMaxLine;
	}
	
	ResetSize();

	return;
}


//-----------------------------------------------------------------------------
void CSysMsgOutput::ResetSize()
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
void CSysMsgOutput::SetCurLine( int curline )
{
	theCurLine = curline;
}

//-----------------------------------------------------------------------------
int CSysMsgOutput::GetCurLine()
{
	return( theCurLine );
}

//-----------------------------------------------------------------------------
void CSysMsgOutput::SetMaxLine( int max_line )
{
	theMaxLine = max_line;
}

//-----------------------------------------------------------------------------

void CSysMsgOutput::ResetSysMsgOutput()
{
//	for( int i = 0; i < MAX_SYSTEMMSG; ++i )
//	{
//		_tcscpy( theSystemMsg[i].Text, _T("") );
//		theSystemMsg[i].Color = 0;
//	}

	theOutputLine = 0;

	return;
}

