///////////////////////////////////////////////////////////////////////////////
///
///		File		: HelpStringTip.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "HelpStringTip.h"


//D3DCOLOR g_HSStrColor = D3DCOLOR_XRGB( 120, 110, 75 );
D3DCOLOR g_HSStrColor = D3DCOLOR_XRGB( 221, 225, 237 );
D3DCOLOR g_HSBKColor = D3DCOLOR_ARGB( 225, 57, 61, 72 );


//-----------------------------------------------------------------------------
CHelpTip::CHelpTip( SDesktop* DT )
{
	pDT				= DT;
	
	theHid			= HID_None;	
	theFontg		= NULL;
	//theFontg		= Fontman->GetFont( n_ftGulimChe, 12, n_fwMedium, n_fdNormal );
	
	memset( &theStrInfo, 0, sizeof( SStringInfo ) );
	memset( theText, 0, sizeof( TCHAR ) * 256 );
	
	theBlankLR		= 4;
	theBlankTB		= 4;
	
	theMaxWidth		= 240 + 11;			// 일본의 경우 글자에 따라 마지막 20글자에서 가로길이가 넘어서서 다음 빈공백이 생김. 그래서 여유로 + 11 !! 
	theFrameWidth	= 2;

	IsHid = FALSE;	 //헬프아이디로 구별여부
}

//-----------------------------------------------------------------------------
CHelpTip::~CHelpTip()
{
}

//-----------------------------------------------------------------------------
void CHelpTip::Initialize()
{
}

//-----------------------------------------------------------------------------
void CHelpTip::Set( HLPID Hid, SRect* wc_size, TCHAR* str, D3DCOLOR color )
{
		
	//id가 같다면..같은 툴팁
	if( (theHid == Hid) && IsHid)
	{
		theHid	= Hid;
		//그래도 위치 값은 계산해줘야...위치가 틀리지 않는다....
		CalcClientSize( wc_size );
		CalcFrameSize();
		return;
	}

	IsHid = TRUE;
	
	theHid	= Hid;
	
	// *문자열의 길이와 높이 구하기
	SAFE_STR_CPY( theText , str ,MAX_SIZE_256 );
	memset( &theStrInfo, 0, sizeof( SStringInfo ) );	
	
	if(theFontg)
		theFontg->GetStringInfo( theText, &theStrInfo, theMaxWidth, true );	
	
	// *길이 높이에 따른 클라이언트 크기 설정 & 프레임 크기 설정
	// *화면상의 위치 계산( 화면 밖으로 벗어나지 않게 보정 )
	CalcClientSize( wc_size );
	CalcFrameSize();	
	
}

void CHelpTip::SetString(SRect* wc_size, TCHAR* str, D3DCOLOR color )
{
	if( (Rstrcmp(theText, str) == 0) && !IsHid )
	{		
		//그래도 위치 값은 계산해줘야...위치가 틀리지 않는다....
		CalcClientSize( wc_size );
		CalcFrameSize();
		return;
	}

	IsHid = FALSE;
	
	// *문자열의 길이와 높이 구하기
	SAFE_STR_CPY( theText, str , MAX_SIZE_256 );
	memset( &theStrInfo, 0, sizeof( SStringInfo ) );	
	
	if(theFontg)
		theFontg->GetStringInfo( theText, &theStrInfo, theMaxWidth, true );	
	
	// *길이 높이에 따른 클라이언트 크기 설정 & 프레임 크기 설정
	// *화면상의 위치 계산( 화면 밖으로 벗어나지 않게 보정 )
	CalcClientSize( wc_size );
	CalcFrameSize();
}

void CHelpTip::SetUpPosString(SRect* wc_size, TCHAR* str, D3DCOLOR color )
{
	//id가 같다면..같은 툴팁
	if( (Rstrcmp(theText, str) == 0) && !IsHid )
	{		
		//그래도 위치 값은 계산해줘야...위치가 틀리지 않는다....
		CalcUpPosClientSize( wc_size );
		CalcFrameSize();
		return;
	}

	IsHid = FALSE;
	
	// *문자열의 길이와 높이 구하기
	SAFE_STR_CPY( theText, str , MAX_SIZE_256 );
	memset( &theStrInfo, 0, sizeof( SStringInfo ) );	
	
	if(theFontg)
		theFontg->GetStringInfo( theText, &theStrInfo, theMaxWidth, true );	
	
	// *길이 높이에 따른 클라이언트 크기 설정 & 프레임 크기 설정
	// *화면상의 위치 계산( 화면 밖으로 벗어나지 않게 보정 )
	CalcUpPosClientSize( wc_size );
	CalcFrameSize();	
}

//-----------------------------------------------------------------------------
void CHelpTip::CalcClientSize( SRect* wc_size )
{
	
	
	/*if( theStrInfo.Line == 1 )
	{
		theClient.w = theStrInfo.Width + ( theBlankLR * 2 );
		theClient.h = theStrInfo.Height + ( theBlankTB * 2 );
	}
	else
	{
		theClient.w = theMaxWidth + ( theBlankLR * 2 );
		theClient.h = theStrInfo.Height + ( theBlankTB * 2 );
	}*/

	theClient.w = theStrInfo.Width + ( theBlankLR * 2 );
	theClient.h = theStrInfo.Height + ( theBlankTB * 2 );
	
	int sc_x = pDT->Width / 2;
	int sc_y = pDT->Height / 2;
	
	int wc_x = wc_size->x;
	int wc_y = wc_size->y;
	int wc_w = wc_size->x + wc_size->w;
	int wc_h = wc_size->y + wc_size->h;
	
	//wndclient_center
	int wc_cnt = ( wc_size->w / 2 ) +  wc_size->x;
	
	
	if( wc_cnt <= sc_x )
	{
		//왼쪽
		if( ( wc_y - theClient.h ) > 0 )
		{
			theClient.x = wc_w;
			theClient.y = wc_y - theClient.h;
		}
		else
		{
			theClient.x = wc_w;
			theClient.y = wc_h;
		}
		
	}
	else if( wc_cnt > sc_x )
	{
		//오른쪽
		if( ( wc_y - theClient.h ) > 0 )
		{
			theClient.x = wc_x - theClient.w;
			theClient.y = wc_y - theClient.h;
		}
		else
		{
			theClient.x = wc_x - theClient.w;
			theClient.y = wc_h;
		}
	}
}

void CHelpTip::CalcUpPosClientSize( SRect* wc_size )
{
	if( theStrInfo.Line == 1 )
	{
		theClient.w = theStrInfo.Width + ( theBlankLR * 2 );
		theClient.h = theStrInfo.Height + ( theBlankTB * 2 );
	}
	else
	{
		theClient.w = theMaxWidth + ( theBlankLR * 2 );
		theClient.h = theStrInfo.Height + ( theBlankTB * 2 );
	}
	

	theClient.x  = wc_size->x;
	theClient.y  = (wc_size->y - theClient.h);	
}

//-----------------------------------------------------------------------------
void CHelpTip::CalcFrameSize()
{
	//가로 프레임 이미지는 괜찮은데..세로 프레임 이미지가 원본 크기보다 작으면 뭉개져서 나온다....
	//일단...크기를 줄여서 해결....원인은?
	
	//위
	theNorthFrm.x = theClient.x;
	theNorthFrm.y = theClient.y - theFrameWidth;
	theNorthFrm.w = theClient.w;
	theNorthFrm.h = theFrameWidth;
	//
	theNorthEastFrm.x = theClient.x + theClient.w;
	theNorthEastFrm.y = theClient.y - theFrameWidth;
	theNorthEastFrm.w = theFrameWidth;
	theNorthEastFrm.h = theFrameWidth;
	//오른쪽
	theEastFrm.x = theClient.x + theClient.w;
	theEastFrm.y = theClient.y;
	theEastFrm.w = theFrameWidth;
	theEastFrm.h = theClient.h;
	//
	theSouthEastFrm.x = theClient.x + theClient.w;
	theSouthEastFrm.y = theClient.y + theClient.h;
	theSouthEastFrm.w = theFrameWidth;
	theSouthEastFrm.h = theFrameWidth;
	//아래
	theSouthFrm.x = theClient.x;
	theSouthFrm.y = theClient.y + theClient.h;
	theSouthFrm.w = theClient.w;
	theSouthFrm.h = theFrameWidth;
	//
	theSouthWestFrm.x = theClient.x - theFrameWidth;
	theSouthWestFrm.y = theClient.y + theClient.h;
	theSouthWestFrm.w = theFrameWidth;
	theSouthWestFrm.h = theFrameWidth;
	//왼쪽
	theWestFrm.x = theClient.x - theFrameWidth;
	theWestFrm.y = theClient.y;
	theWestFrm.w = theFrameWidth;
	theWestFrm.h = theClient.h;
	//
	theNorthWestFrm.x = theClient.x - theFrameWidth;
	theNorthWestFrm.y = theClient.y - theFrameWidth;
	theNorthWestFrm.w = theFrameWidth;
	theNorthWestFrm.h = theFrameWidth;
}

//-----------------------------------------------------------------------------
void CHelpTip::RenderProc()
{
	//	Render->Draw( TID_CLIENT, theClient.x, theClient.y, theClient.w, theClient.h, D3DCOLOR_ARGB( 195, 113, 119, 97 ) );
	Render->Draw( TID_CLIENT, theClient.x, theClient.y, theClient.w, theClient.h, g_HSBKColor );	
	
	Render->Draw( TID_Tooltip2_Img, theNorthFrm.x, theNorthFrm.y, theNorthFrm.w, theNorthFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theNorthEastFrm.x, theNorthEastFrm.y, theNorthEastFrm.w, theNorthEastFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theEastFrm.x, theEastFrm.y, theEastFrm.w, theEastFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theSouthEastFrm.x, theSouthEastFrm.y, theSouthEastFrm.w, theSouthEastFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theSouthFrm.x, theSouthFrm.y, theSouthFrm.w, theSouthFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theSouthWestFrm.x, theSouthWestFrm.y, theSouthWestFrm.w, theSouthWestFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theWestFrm.x, theWestFrm.y, theWestFrm.w, theWestFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theNorthWestFrm.x, theNorthWestFrm.y, theNorthWestFrm.w, theNorthWestFrm.h, D3DCOLOR_ARGB( 225, 220, 220, 255 ) );
	
	
	int x = theClient.x + theBlankLR;
	int y = theClient.y + theBlankTB;
	int w = theClient.w - ( theBlankLR + theBlankLR );
	int h = theClient.h - ( theBlankTB + theBlankTB );
	if(theFontg)
		Render->DrawToolText( theFontg, theText, x, y, w, h, g_HSStrColor, true, 0 );
}

void CHelpTip::RenderProc( float fAlphaRatio, D3DCOLOR texColor )
{
	Render->Draw( TID_CLIENT, theClient.x, theClient.y, theClient.w, theClient.h, D3DCOLOR_ARGB( (int)(fAlphaRatio * 225), 57, 61, 72 ) );	
	
	Render->Draw( TID_Tooltip2_Img, theNorthFrm.x, theNorthFrm.y, theNorthFrm.w, theNorthFrm.h, D3DCOLOR_ARGB( (int)(fAlphaRatio * 225), 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theNorthEastFrm.x, theNorthEastFrm.y, theNorthEastFrm.w, theNorthEastFrm.h, D3DCOLOR_ARGB( (int)(fAlphaRatio * 225), 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theEastFrm.x, theEastFrm.y, theEastFrm.w, theEastFrm.h, D3DCOLOR_ARGB( (int)(fAlphaRatio * 225), 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theSouthEastFrm.x, theSouthEastFrm.y, theSouthEastFrm.w, theSouthEastFrm.h, D3DCOLOR_ARGB( (int)(fAlphaRatio * 225), 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theSouthFrm.x, theSouthFrm.y, theSouthFrm.w, theSouthFrm.h, D3DCOLOR_ARGB( (int)(fAlphaRatio * 225), 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theSouthWestFrm.x, theSouthWestFrm.y, theSouthWestFrm.w, theSouthWestFrm.h, D3DCOLOR_ARGB( (int)(fAlphaRatio * 225), 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theWestFrm.x, theWestFrm.y, theWestFrm.w, theWestFrm.h, D3DCOLOR_ARGB( (int)(fAlphaRatio * 225), 220, 220, 255 ) );
	Render->Draw( TID_Tooltip2_Img, theNorthWestFrm.x, theNorthWestFrm.y, theNorthWestFrm.w, theNorthWestFrm.h, D3DCOLOR_ARGB( (int)(fAlphaRatio * 225), 220, 220, 255 ) );
	
	
	int x = theClient.x + theBlankLR;
	int y = theClient.y + theBlankTB;
	int w = theClient.w - ( theBlankLR + theBlankLR );
	int h = theClient.h - ( theBlankTB + theBlankTB );
	
	D3DCOLOR TColor;
    TColor = (D3DCOLOR)( (0x00ffffff) & texColor );
	TColor = (D3DCOLOR)( ((((int)(fAlphaRatio * 255))&0xff)<<24) | TColor );

	if(theFontg)
		Render->DrawText( theFontg, theText, x, y, w, h, TColor, true, 0 );
}

//-----------------------------------------------------------------------------