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
	
	theMaxWidth		= 240 + 11;			// �Ϻ��� ��� ���ڿ� ���� ������ 20���ڿ��� ���α��̰� �Ѿ�� ���� ������� ����. �׷��� ������ + 11 !! 
	theFrameWidth	= 2;

	IsHid = FALSE;	 //�������̵�� ��������
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
		
	//id�� ���ٸ�..���� ����
	if( (theHid == Hid) && IsHid)
	{
		theHid	= Hid;
		//�׷��� ��ġ ���� ��������...��ġ�� Ʋ���� �ʴ´�....
		CalcClientSize( wc_size );
		CalcFrameSize();
		return;
	}

	IsHid = TRUE;
	
	theHid	= Hid;
	
	// *���ڿ��� ���̿� ���� ���ϱ�
	SAFE_STR_CPY( theText , str ,MAX_SIZE_256 );
	memset( &theStrInfo, 0, sizeof( SStringInfo ) );	
	
	if(theFontg)
		theFontg->GetStringInfo( theText, &theStrInfo, theMaxWidth, true );	
	
	// *���� ���̿� ���� Ŭ���̾�Ʈ ũ�� ���� & ������ ũ�� ����
	// *ȭ����� ��ġ ���( ȭ�� ������ ����� �ʰ� ���� )
	CalcClientSize( wc_size );
	CalcFrameSize();	
	
}

void CHelpTip::SetString(SRect* wc_size, TCHAR* str, D3DCOLOR color )
{
	if( (Rstrcmp(theText, str) == 0) && !IsHid )
	{		
		//�׷��� ��ġ ���� ��������...��ġ�� Ʋ���� �ʴ´�....
		CalcClientSize( wc_size );
		CalcFrameSize();
		return;
	}

	IsHid = FALSE;
	
	// *���ڿ��� ���̿� ���� ���ϱ�
	SAFE_STR_CPY( theText, str , MAX_SIZE_256 );
	memset( &theStrInfo, 0, sizeof( SStringInfo ) );	
	
	if(theFontg)
		theFontg->GetStringInfo( theText, &theStrInfo, theMaxWidth, true );	
	
	// *���� ���̿� ���� Ŭ���̾�Ʈ ũ�� ���� & ������ ũ�� ����
	// *ȭ����� ��ġ ���( ȭ�� ������ ����� �ʰ� ���� )
	CalcClientSize( wc_size );
	CalcFrameSize();
}

void CHelpTip::SetUpPosString(SRect* wc_size, TCHAR* str, D3DCOLOR color )
{
	//id�� ���ٸ�..���� ����
	if( (Rstrcmp(theText, str) == 0) && !IsHid )
	{		
		//�׷��� ��ġ ���� ��������...��ġ�� Ʋ���� �ʴ´�....
		CalcUpPosClientSize( wc_size );
		CalcFrameSize();
		return;
	}

	IsHid = FALSE;
	
	// *���ڿ��� ���̿� ���� ���ϱ�
	SAFE_STR_CPY( theText, str , MAX_SIZE_256 );
	memset( &theStrInfo, 0, sizeof( SStringInfo ) );	
	
	if(theFontg)
		theFontg->GetStringInfo( theText, &theStrInfo, theMaxWidth, true );	
	
	// *���� ���̿� ���� Ŭ���̾�Ʈ ũ�� ���� & ������ ũ�� ����
	// *ȭ����� ��ġ ���( ȭ�� ������ ����� �ʰ� ���� )
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
		//����
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
		//������
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
	//���� ������ �̹����� ��������..���� ������ �̹����� ���� ũ�⺸�� ������ �������� ���´�....
	//�ϴ�...ũ�⸦ �ٿ��� �ذ�....������?
	
	//��
	theNorthFrm.x = theClient.x;
	theNorthFrm.y = theClient.y - theFrameWidth;
	theNorthFrm.w = theClient.w;
	theNorthFrm.h = theFrameWidth;
	//
	theNorthEastFrm.x = theClient.x + theClient.w;
	theNorthEastFrm.y = theClient.y - theFrameWidth;
	theNorthEastFrm.w = theFrameWidth;
	theNorthEastFrm.h = theFrameWidth;
	//������
	theEastFrm.x = theClient.x + theClient.w;
	theEastFrm.y = theClient.y;
	theEastFrm.w = theFrameWidth;
	theEastFrm.h = theClient.h;
	//
	theSouthEastFrm.x = theClient.x + theClient.w;
	theSouthEastFrm.y = theClient.y + theClient.h;
	theSouthEastFrm.w = theFrameWidth;
	theSouthEastFrm.h = theFrameWidth;
	//�Ʒ�
	theSouthFrm.x = theClient.x;
	theSouthFrm.y = theClient.y + theClient.h;
	theSouthFrm.w = theClient.w;
	theSouthFrm.h = theFrameWidth;
	//
	theSouthWestFrm.x = theClient.x - theFrameWidth;
	theSouthWestFrm.y = theClient.y + theClient.h;
	theSouthWestFrm.w = theFrameWidth;
	theSouthWestFrm.h = theFrameWidth;
	//����
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