///////////////////////////////////////////////////////////////////////////////
///
///		File		: fontman.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-06
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "Fontman.h"
#include "Fontasm.h"
#include "..\\..\\RockClient.h"


CFontman*		Fontman = NULL;



//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
CFontman::CFontman( HWND hWnd, LPDIRECT3DDEVICE9 d3dDevice )
#else
CFontman::CFontman( HWND hWnd, LPDIRECT3DDEVICE8 d3dDevice )
#endif // DIRECT_VERSION_9_MJH
{
	theNumFontg = 0;
	memset( theFontg, NULL, sizeof( CFontg* ) * MAX_FONTG );

	Fontasm = SAFE_NEW( CFontasm( hWnd, d3dDevice ) );	
	Fontasm->EnumTTFontFamilies();
}

//-----------------------------------------------------------------------------
CFontman::~CFontman()
{
	delete( Fontasm );

	for( int i = 0; i < theNumFontg; ++i )
	{
		if( theFontg[i] != NULL )
		{
			delete( theFontg[i] );
			theFontg[i] = NULL;
		}
	}

	theNumFontg = 0;
}

//-----------------------------------------------------------------------------
CFontg* CFontman::GetFont( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco, bool ab_smooth , int al_width )
{

	switch(g_RockClient.GetLanguage())
	{
	case eLang_Korea:
	case eLang_Japan:
	case eLang_China:
	case eLang_Taiwan:
	case eLang_HongKong:
		break;
	default:
#ifdef APPLY_ENGLISH_ORIGIN_SIZE
#else		
		Size = Size - 2;
#endif

	    break;
	}

	for( int i = 0; i < theNumFontg; ++i )
	{
		if( ( theFontg[i] != NULL ) && ( theFontg[i]->Existence( FontType, Size, Weight, Deco, ab_smooth, al_width ) ) )
		{
			theFontg[i]->AddRef();
			return( theFontg[i] );
		}
	}
	
	if( theNumFontg <= MAX_FONTG )
	{
		theFontg[theNumFontg] = SAFE_NEW( CFontg( FontType, Size, Weight, Deco, ab_smooth, al_width) );		
		theFontg[theNumFontg]->AddRef();
		theNumFontg++;

		return( theFontg[theNumFontg-1] );//리턴해야될 값은 n-1 ( theNumFontg++ 때문에 )
	}

	// 동시 지원하는 폰트의 갯수를 넘어쓸때.... 에러 메세지 출력
	return( NULL );
}

//-----------------------------------------------------------------------------
int CFontman::GetNumTTFont()// TTFont 갯수
{
	return( Fontasm->GetNumTTFont() );
}

//-----------------------------------------------------------------------------
LOGFONT* CFontman::GetTTFont( TCHAR* Name )// 이름검색
{
	return( Fontasm->GetTTFont( Name ) );
}

//-----------------------------------------------------------------------------
LOGFONT* CFontman::GetTTFont( int Idx )// 인덱스검색
{
	return( Fontasm->GetTTFont( Idx ) );
}

//-----------------------------------------------------------------------------
LOGFONT* CFontman::GetTTFont( int Idx, NFontCharset Charset )//조건검색( 캐릭터셋으로 )
{
	return( Fontasm->GetTTFont( Idx, Charset ) );
}

//-----------------------------------------------------------------------------