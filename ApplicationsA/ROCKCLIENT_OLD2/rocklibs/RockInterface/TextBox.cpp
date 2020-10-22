///////////////////////////////////////////////////////////////////////////////
///
///		File		: TextBox.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////

#include "TextBox.h"
#include "UiUtil.h"

#include "..\\..\\StringManager.h"
#include "..\\..\\RockClient.h"



extern long g_nowTime;

//-----------------------------------------------------------------------------
CTextBox::CTextBox( SDesktop* DT ) : CWnd( DT )
{
	TextLimitLen	= 64;
	TextLength		= 0;
	NumLine			= 0;
	
	Text = NULL;
	Text = SAFE_NEW_ARRAY( TCHAR , TextLimitLen+1 );
	m_AllocTextLen = TextLimitLen+1;

	if( Text != NULL )
	{
		// _tcscpy( Text, _RT("") );
		memset( Text , 0 , sizeof( TCHAR ) * ( TextLimitLen + 1 ) );
	}
	else
	{
		assert(0);
	}

	Tx = 0;
	Ty = 0;
	Tw = 0;
	Th = 0;

	Ax = 0;
	Ay = 0;

	DecorationType = n_fdNormal;

	StrInfo.Width = 0;
	StrInfo.Height = 0;
	StrInfo.NumStr = 0;

	theFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	//Font->GetStringInfo( Text, DecorationType, &StrInfo );

	Rtcolor[0] = 255;
	Rtcolor[1] = 255;
	Rtcolor[2] = 255;
	Rtcolor[3] = 255;
	Tcolor = D3DCOLOR_ARGB( Rtcolor[0], Rtcolor[1], Rtcolor[2], Rtcolor[3] );

	BlankLeft	= 0;
	BlankRight	= 0;
	BlankTop	= 0;
	BlankBottom	= 0;

	AlignX		= n_atLeft;
	AlignY		= n_atTop;
	
	isBlankLine		= false;
	isCenterLine	= false;
	isMultiLine		= false;
	m_LineInterval	= 0;
	isIntegralText	= true;
	isImageNum = false;

	StartLine		= 0; 

	m_DrawTextTime = 0;

	IsScaleTrans = false;

	aFontResv = -1;

	m_IsMultiLineAsign = false;
	
	memset(TextLine, 0, sizeof(TCHAR *) * 10);
}

//-----------------------------------------------------------------------------
CTextBox::~CTextBox()
{
	SAFE_DELETE_ARRAY( Text );

	for(int i = 0 ; i < 10 ; i++)
	{
		SAFE_DELETE_ARRAY(TextLine[i]);
	}
}

void CTextBox::ResetSize( SRect* size )
{
	CWnd::ResetSize( size );
}

//-----------------------------------------------------------------------------
void CTextBox::RenderProc()
{
	UpdateHandler();

	//Render->DrawState();	
	
	if( isFitImg )
	{
		// 이미지 전체를 정해진 사이즈에 맞게 그리기 
		Render->DrawFitImg( Cimg, Cx, Cy, Cw, Ch, Ccolor );
	}
	else
	{
		// TID 텍스쳐 이미지를 정해진 사이즈에 맞게 그리기 
		if( IsScaleTrans )
		{
			Render->ScretchDraw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
		}
		else
		{
			Render->Draw( Cimg, Cx, Cy, Cw, Ch, Ccolor );
		}
	}

	static int x = 0;
	if( isImageNum )
	{
		Render->DrawImageNUM( Text, Ax, Ay, Tcolor );
	}
	else if( isMultiLine )
	{
		if( m_DrawTextTime )
		{
			int StepCnt =  SAFE_TIME_SUB( g_nowTime, m_DrawStartTime ) / m_DrawTextTime;
		
			bool bDrawStep = true;

			bDrawStep = Render->DrawText_StepCnt( theFontg, Text, Tx, Ty, Tw, Th, Tcolor, 
								isIntegralText, StartLine, StepCnt, m_LineInterval );

			if( bDrawStep == false )
			{
				m_DrawTextTime = 0;
			}
		}
		else 
		{
			if(m_IsMultiLineAsign)
			{
				DrawMutiAlignText();
			}
			else
			{
				Render->DrawText( theFontg, Text, Tx, Ty, Tw, Th, Tcolor, 
							isIntegralText, StartLine, m_LineInterval );
			}
		}
	}
	else
	{
		Render->DrawAlignText( theFontg, Text, Ax, Ay, Tx, Ty, Tw, Th, Tcolor );
	}
}

void CTextBox::DrawMutiAlignText()
{
	static SStringInfo	tempStrInfo;

	int nPosX = Tx;
	int nPosY = Ty;	
	
	for(int i = 0; i < m_MultiLineCount ; i++)
	{
		theFontg->GetStringInfo( TextLine[i], &tempStrInfo );
		nPosX = GetAlignPosX(tempStrInfo);
		int nAddH = Render->DrawAlignText( theFontg, TextLine[i], nPosX, nPosY, Tx, Ty, Tw, Th, Tcolor );
		
		nPosY += (nAddH + m_LineInterval);
	}
}

#ifdef HHM_TEXTBOX_ENABLE// 활성화에 따라 텍스트 색 변경. 비활성색(128, 128, 128) (2009/05/13)

void CTextBox::SetEnable(bool bEnable)
{
	CWnd::SetEnable(bEnable);

	if(!isEnable)
		SetTextColor(128, 128, 128);
}

#endif// #define HHM_TEXTBOX_ENABLE// 활성화에 따라 텍스트 색 변경. 비활성색(128, 128, 128) (2009/05/13)

//-----------------------------------------------------------------------------
SEventMessage* CTextBox::EventProc( SEventMessage* EMsg )
{
	if(!isEnable)
	  return EMsg;	
	
	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;	
	
	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
		OnMouseFocus( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_FOCUS;
		break;
	case MA_LDOWN:
		OnMouseLButtonDown( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_DOWN;
		break;
	case MA_LPRESS:
		OnMouseLButtonPress( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_PRESS;
		break;
	case MA_RUP:
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_RCLICK;
		break;	
	case MA_LUP:
		OnMouseLButtonUp( Mx, My );
		EMsg->FocusWnd	= Iid;
		EMsg->Notify	= NM_BTN_CLICK;
		break;
	}
	
	return( EMsg );
}

//-----------------------------------------------------------------------------
IRESULT CTextBox::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	int Iresult( 0 );
	Iresult = CWnd::ReceiveEvent( Event, fParam, sParam, xParam, exParam );

	switch( Event )
	{
	case n_emSetText:
		SetText( (TCHAR*)fParam );
		SetTextColor( (UCHAR)sParam, (UCHAR)xParam, (UCHAR)exParam );
		break;
	case n_emGetText:
		GetText( (TCHAR*)fParam, (int)sParam );
		break;
	case n_emSetTextAlpha:
		SetTextAlpha( (UCHAR)fParam );
		break;
	case n_emSetDecorationFont:
		SetFontDecoration( (NFontDecoration)fParam );
		break;
	case n_emMultiLine:
		SetMultiLine( (bool)fParam );
		GetTextLine();
		break;
	case n_emTextIntegral:
		SetIntegralText( (bool)fParam );
		break;
	case n_emGetTextLine:
		Iresult = NumLine;
		break;
	}

	return( Iresult );
}

///---------------------------------------------------------------------------
///-- SetFontg
///---------------------------------------------------------------------------
void CTextBox::SetFontg( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco )
{	
	// n_fdNormal, n_fdShadow, n_fdOutline
	theFontg = Fontman->GetFont( FontType, Size, Weight, Deco );	
	
	theFontg->GetStringInfo( _RT(""), &StrInfo );
}
///---------------------------------------------------------------------------
///-- SetFontg5
///---------------------------------------------------------------------------
void CTextBox::SetFontg5( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco , bool ab_smooth , int al_width )
{	
	// n_fdNormal, n_fdShadow, n_fdOutline
	theFontg = Fontman->GetFont( FontType, Size, Weight, Deco , ab_smooth , al_width );	
	
	theFontg->GetStringInfo( _RT(""), &StrInfo );
}

///---------------------------------------------------------------------------
///-- SetFontR
///---------------------------------------------------------------------------
///-- 각 상황에 사용될 Font 의 속성을 미리 정의해 두고 예약 Enum 으로 호출
///-- 중복 및 버그 방지
void CTextBox::SetFontR( NFontResv ae_fonttype )
{
//	int l_nDevSkip = GetPrivateProfileInt( "Local", "Language", 0, "./setup.ini");
	
	aFontResv = ae_fonttype;
	
	switch( ae_fonttype )
	{
		///-- Title Text
	case n_fRsvFontTitle:

		if( g_RockClient.GetLanguage() == eLang_Japan)
		{
			theFontg = Fontman->GetFont( n_ftDotum, 11, n_fwBold, n_fdNormal, true ); ///-- japan
		}
		else if( g_RockClient.GetLanguage() == eLang_Korea )
		{
			theFontg = Fontman->GetFont( n_ftDotum, 11, n_fwNormal, n_fdNormal, true ); ///-- japan
		}
		else 
		{
			theFontg = Fontman->GetFont( n_ftDotum, 12, n_fwNormal, n_fdNormal, true ); ///-- korea
		}		
		SetTextColor( 70, 70, 70 );	
		SetClientImage( TID_None );	
		SetAlignText( n_atLeft, n_atCenter );		
		theFontg->GetStringInfo( _RT(""), &StrInfo );
		SetFocusCheck( false );
		break;
		///-- Button Text
	case n_fRsvFontWndButton:

		if( g_RockClient.GetLanguage() == eLang_Japan )
		{
			theFontg = Fontman->GetFont( n_ftDotum, 10, n_fwBold, n_fdNormal, true );
		}
		else
		{
			theFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		}
		SetClientImage( TID_None );	
		SetAlignText( n_atCenter, n_atCenter );
		SetTextColor( 30, 30, 30 );
		theFontg->GetStringInfo( _RT(""), &StrInfo );
		
		break;
		///-- Wnd 바탕에 적혀있는 Static Text
	case n_fRsvFontWndStatic:
		theFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		SetClientImage( TID_None );	
		SetAlignText( n_atCenter, n_atCenter );
		SetTextColor( 100, 100, 100);
		theFontg->GetStringInfo( _RT(""), &StrInfo );		
		SetFocusCheck( false );
		break;		
		///-- Wnd 바탕에 적혀있는 Static Text
	case n_fRsvFontWndStatic2:
		theFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		SetClientImage( TID_None );	
		SetAlignText( n_atCenter, n_atCenter );
		SetTextColor( 255, 255, 255);
		theFontg->GetStringInfo( _RT(""), &StrInfo );		
		SetFocusCheck( false );
		break;
		///-- NPC Chat 
	case n_fRsvFontNPCChat:
		theFontg = Fontman->GetFont( n_ftDotum, 12, n_fwNormal, n_fdNormal, true );
		SetClientImage( TID_None );	
		SetAlignText( n_atLeft, n_atTop );
		SetTextColor( 15, 75, 120 );
		theFontg->GetStringInfo( _RT(""), &StrInfo );
		break;
	case n_fRsvFontNPCChat2:
		theFontg = Fontman->GetFont( n_ftDotum, 10, n_fwNormal, n_fdNormal, true );
		SetClientImage( TID_None );	
		SetAlignText( n_atLeft, n_atTop );
		SetTextColor( 15, 25, 70 );
		theFontg->GetStringInfo( _RT(""), &StrInfo );
		break;
		///-- 공지
	case n_fRsvNotice:
		theFontg = Fontman->GetFont( n_ftDotum, 20, n_fwBold, n_fdOutline, true, 5 );
		SetClientImage( TID_None );	
		SetAlignText( n_atCenter, n_atCenter );
		SetTextColor( 255, 255, 255);
		theFontg->GetStringInfo( _RT(""), &StrInfo );
		break;
		///-- MessageBox 에 사용되는 Font
	case n_fRsvMessageBox:
		theFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		SetClientImage( TID_None );	
		SetAlignText( n_atCenter, n_atCenter );
		SetTextColor( 70, 70, 150);		
		theFontg->GetStringInfo( _RT(""), &StrInfo );		
		SetFocusCheck( false );
		break;
		///-- Image Font
	case n_fRsvImageNum:		
		theFontg = Fontman->GetFont( n_ftDotum, 11, n_fwBold, n_fdNormal, true );
		SetClientImage( TID_None );		
		SetAlignText( n_atRight, n_atCenter );
		SetTextColor( 0, 0, 0);				
		SetImageNum( true );
		break;
		
	}
	
}

void CTextBox::SetText( void* pStr )
{
	char *Str = (char *)pStr;

	TextLength = Rstrlen( RWCHAR(Str) );
	if( TextLength == 0 )
	{
		Rstrcpy( RWCHAR(Text), _RT("") );
		return;
	}
#ifdef UNICODE_ROCK
	if( TextLimitLen <= (TextLength*2) )
	{
		SAFE_DELETE_ARRAY( Text );		
		TextLimitLen = (TextLength + 1)*2;
		Text = SAFE_NEW_ARRAY( TCHAR , TextLimitLen );
		m_AllocTextLen = TextLimitLen;
	}

#else
	if( TextLimitLen <= (TextLength) )
	{
		SAFE_DELETE_ARRAY( Text );		
		TextLimitLen = TextLength + 1;
		
		Text = SAFE_NEW_ARRAY( TCHAR , TextLimitLen );
		m_AllocTextLen = TextLimitLen;
	}

#endif

	Rstrcpy( RWCHAR(Text), RWCHAR(Str) );

	GetTextLine();
	theFontg->GetStringInfo( Text, &StrInfo );
	
	if( isImageNum )
	{
		StrInfo.Height = 5;
		StrInfo.Width =	TextLength * 6;		
	}
	
	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;
	Tw = Cw - ( BlankRight + BlankLeft );
	Th = Ch - ( BlankBottom + BlankTop );
	
	Ax = GetAlignPosX();
	Ay = GetAlignPosY();

	m_MultiLineCount = 0;

	for(int i = 0 ; i < 10 ; i++)
	{
		SAFE_DELETE_ARRAY(TextLine[i]);
	}

	if(m_IsMultiLineAsign) ///-- 문자열을 개행으로 분리한다.
	{
		int nLineCount = 0;
		int nColCount = 0;		
		
		TextLine[nLineCount] = SAFE_NEW_ARRAY( TCHAR , TextLimitLen );		
		memset( TextLine[nLineCount], 0, sizeof(TCHAR)*TextLimitLen);
		
		char *pStr = (char *)Text;
		int nStrLen = Rstrlen(RWCHAR(pStr));

		for(int i = 0 ; i < nStrLen ; i++)
		{
			//더블 샵 개행
			bool IsDoubleShap = false;
			if( (i < nStrLen - 1) && ('#' == pStr[i]) && ('#' == pStr[i + 1]) )
			{
				IsDoubleShap = true;
			}

			if(!IsDoubleShap)
			{
				TextLine[nLineCount][nColCount] = pStr[i];
				nColCount++;			
			}
			else
			{
				nLineCount++;
				nColCount = 0;
				i += 1;
				TextLine[nLineCount] = SAFE_NEW_ARRAY( TCHAR , TextLimitLen );				
				memset(TextLine[nLineCount], 0, sizeof(TCHAR)*TextLimitLen);
			}	
		}

	   m_MultiLineCount	= nLineCount + 1;
	}
}

//-----------------------------------------------------------------------------
void CTextBox::GetText( TCHAR* Str, int MaxLen )
{
	int Len =  Rstrlen( Text );
	if( MaxLen <=  Len )
	{
		return;
	}
	Rstrcpy( Str, Text);
}
///---------------------------------------------------------------------------
///-- SetLimitLength
///---------------------------------------------------------------------------
void CTextBox::SetLimitLength(const int nLength)
{		
	::ConvertStrInLimitLength( RWCHAR(Text), nLength);
}
//-----------------------------------------------------------------------------
void CTextBox::SetFontDecoration( NFontDecoration DecorationType )
{
	this->DecorationType = DecorationType;
}

//-----------------------------------------------------------------------------
void CTextBox::SetAdjustPosition( int PosX, int PosY )//OR
{
 	// 원 루틴

	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;

	Tw = Cw - ( BlankRight + BlankLeft );
	Th = Ch - ( BlankBottom + BlankTop );

	Ax = GetAlignPosX();
	Ay = GetAlignPosY();
}

//-----------------------------------------------------------------------------
void CTextBox::SetBlank( int BlankLR, int BlankTB )
{
	BlankLeft	= BlankLR;
	BlankRight	= BlankLR;
	BlankTop	= BlankTB;
	BlankBottom	= BlankTB;

	SetAdjustPosition( 0, 0 );
}

//-----------------------------------------------------------------------------
void CTextBox::SetBlank( int BlankL, int BlankR, int BlankT, int BlankB )
{
	BlankLeft	= BlankL;
	BlankRight	= BlankR;
	BlankTop	= BlankT;
	BlankBottom	= BlankB;

	SetAdjustPosition( 0, 0 );
}

//-----------------------------------------------------------------------------
void CTextBox::SetBlankLine( bool BlankLine )
{
	isBlankLine = BlankLine;
}

//-----------------------------------------------------------------------------
void CTextBox::SetCenterLine( bool CenterLine )
{
	isCenterLine = CenterLine;
}

//-----------------------------------------------------------------------------
void CTextBox::SetTextColor( UCHAR Red, UCHAR Green, UCHAR Blue )
{
	Rtcolor[R] = Red;
	Rtcolor[G] = Green;
	Rtcolor[B] = Blue;
	Tcolor = D3DCOLOR_ARGB( Rtcolor[A], Rtcolor[R], Rtcolor[G], Rtcolor[B] );
}

//-----------------------------------------------------------------------------
void CTextBox::SetTextAlpha( UCHAR Alpha )
{
	Rtcolor[A] = Alpha;
	Tcolor = D3DCOLOR_ARGB( Rtcolor[A], Rtcolor[R], Rtcolor[G], Rtcolor[B] );
}

//-----------------------------------------------------------------------------
void CTextBox::SetStartLine( int line )
{
	StartLine = line;
}

//-----------------------------------------------------------------------------
void CTextBox::SetMultiLine( bool MultiLine )
{
	isMultiLine = MultiLine;
}

void CTextBox::SetLineInterval( int LineInterval )
{
	m_LineInterval = LineInterval;
}

///---------------------------------------------------------------------------
///-- SetImageNum
///---------------------------------------------------------------------------
void CTextBox::SetImageNum( bool ab_imgnum)
{
	isImageNum = ab_imgnum;
}
//-----------------------------------------------------------------------------
void CTextBox::SetIntegralText( bool Integral )
{
	isIntegralText = Integral;
}

//-----------------------------------------------------------------------------
void CTextBox::SetAlignText( NAlignText AlignX, NAlignText AlignY )
{
	this->AlignX = AlignX;
	this->AlignY = AlignY;
}

//-----------------------------------------------------------------------------
void CTextBox::SetTextSize( int LimitLen )
{
	if( Text )
	{
		SAFE_DELETE_ARRAY( Text );		

		Text = SAFE_NEW_ARRAY( TCHAR , LimitLen );
		m_AllocTextLen = LimitLen;
		Rstrcpy( RWCHAR(Text), _RT("") );
	}
}

//-----------------------------------------------------------------------------
int CTextBox::GetTextLine()
{
	if( TextLength == 0 ){	return( 0 );	}
	if( !isMultiLine ){	return( 1 );	}
	
	int PosX = Tx;
	int PosY = Ty;
	int Width = Tx + Tw;
	//int Height = Ty + Th;
	
	static FONTID IdxBuf[MAX_INDEX_BUFFER];
	int NumStr( 0 );
	memset( IdxBuf, 0, sizeof(FONTID) * MAX_INDEX_BUFFER );
	SFont* RFont = NULL;
	
	NumStr = theFontg->Confirm( Text, IdxBuf );
	
	for( int i = 0; i < NumStr; ++i )
	{
		RFont = theFontg->GetFontasm( IdxBuf[i] );
		
		if( RFont != NULL )
		{
			PosX = PosX + RFont->w;

			//-----------------------------------------------------------------
			//Integral -> 문자열을 폭과 높이에 맞춘다
			if( ( isIntegralText && (PosX > Width) ) || (IdxBuf[i] == 10) )
			{
				PosX = Tx;
				PosY = PosY + RFont->h;

				RFont->vt[0].x = (float)PosX - 0.5f;
				RFont->vt[0].y = (float)PosY - 0.5f;
				RFont->vt[1].x = RFont->vt[0].x + RFont->w;
				RFont->vt[1].y = RFont->vt[0].y;
				RFont->vt[2].x = RFont->vt[0].x + RFont->w;
				RFont->vt[2].y = RFont->vt[0].y + RFont->h;
				RFont->vt[3].x = RFont->vt[0].x;
				RFont->vt[3].y = RFont->vt[0].y + RFont->h;

				if( IdxBuf[i] != 10 ){	PosX = PosX + RFont->w;	}

				NumLine++;
			}
		}
	}

	return( NumLine );
}

//-----------------------------------------------------------------------------
int CTextBox::GetAlignPosX()
{
	int PosX( 0 );
	float PosBoxCenterX( 0.0f );
	float PosFontCenterX( 0.0f );

	if( ( TextLength == 0 ) || ( StrInfo.Width >= Tw ) )
	{	
		PosX = Tx;
	}
	
    switch( AlignX )
    {
    case n_atLeft:
            PosX = Tx;
            break;

    case n_atCenter:
            PosBoxCenterX = (float)( Tw / 2 );
            PosFontCenterX = (float)( StrInfo.Width / 2 );

            PosX = (int)( PosBoxCenterX - PosFontCenterX );
			PosX += Tx;
            break;

    case n_atRight:
            PosX = Tw - StrInfo.Width;
			PosX += Tx;
            break;
    }

    return( PosX );
}

int CTextBox::GetAlignPosX(SStringInfo & tempStrInfo)
{
	int PosX( 0 );
	float PosBoxCenterX( 0.0f );
	float PosFontCenterX( 0.0f );

	if( ( TextLength == 0 ) || ( tempStrInfo.Width >= Tw ) )
	{	
		PosX = Tx;
	}
	
    switch( AlignX )
    {
    case n_atLeft:
            PosX = Tx;
            break;

    case n_atCenter:
            PosBoxCenterX = (float)( Tw / 2 );
            PosFontCenterX = (float)( tempStrInfo.Width / 2 );

            PosX = (int)( PosBoxCenterX - PosFontCenterX );
			PosX += Tx;
            break;

    case n_atRight:
            PosX = Tw - tempStrInfo.Width;
			PosX += Tx;
            break;
    }

    return( PosX );
}

//-----------------------------------------------------------------------------
int CTextBox::GetAlignPosY()
{
	int PosY( 0 );
	float PosBoxCenterY( 0.0f );
	float PosFontCenterY( 0.0f );

	if( ( TextLength == 0 ) || ( StrInfo.Height >= Th ) )
	{	
		PosY = Ty;
	}

	switch( AlignY )
    {
    case n_atTop:
            PosY = Ty;
            break;

    case n_atCenter:
            PosBoxCenterY = (float)( Th / 2 );
            PosFontCenterY = (float)( StrInfo.Height / 2 );

            PosY = (int)( PosBoxCenterY - PosFontCenterY );
			PosY += Ty;
            break;

    case n_atBottom:
			PosY = Th - StrInfo.Height;
			PosY += Ty;
            break;
    }

    return( PosY );
}

//-----------------------------------------------------------------------------

void CTextBox::SetDrawTextTime( long aDrawTextTime )
{
	m_DrawStartTime = g_nowTime;
	m_DrawTextTime = aDrawTextTime; 

	return;
}


void CTextBox::SetScaleTrans( bool bFlag )
{
	IsScaleTrans = bFlag;
}

void CTextBox::SetMultiLineAsign(bool bFlag)
{
	m_IsMultiLineAsign = bFlag;
}
