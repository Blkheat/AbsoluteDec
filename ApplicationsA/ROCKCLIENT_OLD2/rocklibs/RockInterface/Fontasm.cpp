     ///////////////////////////////////////////////////////////////////////////////
///
///		File		: Fontasm.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-06
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////

#include "..\\..\\StringManager.h"
#include "..\\..\\RockClient.h"

#include "Fontasm.h"


CFontasm*		Fontasm;


//-----------------------------------------------------------------------------
int CALLBACK EnumTTFontCallBack( ENUMLOGFONT FAR *lpelf, NEWTEXTMETRIC FAR *lpntm, int FontType, LPARAM lParam )
{
	static int TTFontCount( 0 );
	
	lpntm;
	lParam;
	FontType;//TRUETYPE_FONTTYPE�� �޴´�.


	if( FontType != TRUETYPE_FONTTYPE )
	{
		return( TRUE );
	}
	if( TTFontCount < MAX_TTFONT )
	{
		Fontasm->SetTTFont( &lpelf->elfLogFont );
		
		TTFontCount++;
		Fontasm->SetNumTTFont( TTFontCount );

		return( TRUE );
	}

	return( FALSE );//FALSE == �˻� ���
}

//-----------------------------------------------------------------------------
CFontasm::CFontasm()
{
}

//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
CFontasm::CFontasm( HWND hWnd, LPDIRECT3DDEVICE9 d3dDevice )
#else
CFontasm::CFontasm( HWND hWnd, LPDIRECT3DDEVICE8 d3dDevice )
#endif // DIRECT_VERSION_9_MJH

{
	///-- Global
	theD3DDevice		= d3dDevice;
	thehWnd				= hWnd;

	///-- Common Object
	thehDC				= NULL;
	thehFont			= NULL;	
	thehBrush			= NULL;

	///-- DC
	thehNomMemDC		= NULL;
	thehShdMemDC		= NULL;
	thehNomBitmap		= NULL;	
	thehShdBitmap		= NULL;

	///-- Bitmap
	thehNomOldBitmap	= NULL;	
	thehShdOldBitmap	= NULL;

	///-- Font
	thehNomOldFont		= NULL;
	thehShdOldFont		= NULL;

	///-- Brush
	thehNomOldBrush		= NULL;
	thehShdOldBrush		= NULL;
	
	
	memset( &theBmpInfo, 0, sizeof( BITMAPINFO ) );

	theRgbBuffer		= NULL;
	theFlipBuffer		= NULL;

	theNumTTFont		= 0;
	memset( &theTTFont[0], 0, sizeof( LOGFONT ) * MAX_TTFONT );


	///-- Create Memory DC
	thehDC				= GetDC( thehWnd );
	thehNomMemDC			= CreateCompatibleDC( thehDC );
	thehShdMemDC			= CreateCompatibleDC( thehDC );
	ReleaseDC( thehWnd , thehDC );

	SetMapMode( thehNomMemDC, MM_TEXT );
	SetMapMode( thehShdMemDC, MM_TEXT );

	//EnumTTFontFamilies(); // ���⼭�� ����....

	strcpy( thePrevName, "System" );
	thePrevSize			= 24;	
	thePrevWeight		= n_fwNormal;
	thePrevDeco			= n_fdNormal;
	thePrevFont			= n_ftGulimChe;
	thePrevSmooth		= false;
	
	theTextureSize		= GetTextureSize( thePrevSize );
	theFontCharset		= n_fcHangul; //n_fcAnsi; // GetTTFontCharset( thePrevName ); -> EnumTTFontFamilies();ȣ�����̶� ���� ����.
	theSmooth_Multi = 1;
	theFontWidth = 0;

	//ReleaseDevice(); ó�� ���۽� release�� �ʿ����.
	SetDevice( n_ftGulimChe );
}

//-----------------------------------------------------------------------------
CFontasm::~CFontasm()
{
	ReleaseDevice();

	DeleteDC( thehNomMemDC );
	DeleteDC( thehShdMemDC );
//  ReleaseDC( thehWnd, thehDC );
}

//-----------------------------------------------------------------------------
void CFontasm::EnumTTFontFamilies()// TTFont ����
{
	//HDC hDC = GetDC( thehWnd );
	thehDC				= GetDC( thehWnd );

	EnumFontFamilies( thehDC, NULL, (FONTENUMPROC)EnumTTFontCallBack, (LPARAM)NULL );

	ReleaseDC( thehWnd , thehDC );
	
	//ReleaseDC( thehWnd, hDC );
}

//-----------------------------------------------------------------------------
void CFontasm::SetTTFont( LOGFONT* TTFont )
{
	memcpy( &theTTFont[theNumTTFont], TTFont, sizeof( LOGFONT ) );
}

//-----------------------------------------------------------------------------
void CFontasm::SetNumTTFont( int NumTTFont )// TTFont ���� ����
{
	theNumTTFont = NumTTFont;
}

//-----------------------------------------------------------------------------
int CFontasm::GetNumTTFont()// TTFont ����

{
	return( theNumTTFont );
}

//-----------------------------------------------------------------------------
LOGFONT* CFontasm::GetTTFont( TCHAR* Name )// �̸��˻�

{
	for( int i = 0; i < theNumTTFont; ++i )
	{
		if( _tcsicmp( theTTFont[i].lfFaceName, Name ) == 0 )
		{
			return( &theTTFont[i] );
		}
	}

	return( NULL );
}

//-----------------------------------------------------------------------------
LOGFONT* CFontasm::GetTTFont( int Idx )// �ε����˻�

{
	if( ( Idx < 0 ) && ( Idx >= theNumTTFont ) )
	{
		return( NULL );
	}

	return( &theTTFont[Idx] );
}

//-----------------------------------------------------------------------------
LOGFONT* CFontasm::GetTTFont( int Idx, NFontCharset Charset )//���ǰ˻�( ĳ���ͼ����� )
{
	if( ( Idx < 0 ) && ( Idx >= theNumTTFont ) )
	{
		return( NULL );
	}

	if( theTTFont[Idx].lfCharSet == Charset )
	{
		return( &theTTFont[Idx] );
	}

	return( NULL );
}

//-----------------------------------------------------------------------------
int CFontasm::Confirm( SFont** Fontasm, TCHAR* Str, FONTID* Buf )
{
	int Idx( 0 );
	int Len( 0 );
	int NumStr( 0 );
	

    if( Str == NULL )
	{
		return( 0 );
	}

	Len = Rstrlen( Str );

#ifdef UNICODE_ROCK
    
	Len = wcslen( RWCHAR(Str) );
	WCHAR*pwStr = RWCHAR(Str);
	for( int i = 0; i < Len; ++i )
    {
		Idx = pwStr[i];
    
		if( Fontasm[Idx] == NULL )
        {
			return( -1 );
		}

		Buf[NumStr] = (FONTID)Idx;
		NumStr++;
	}
#else
	
	for( int i = 0; i < Len; ++i )
    {
		Idx = DetectionDBCS( &i, Str );
        
        if( Fontasm[Idx] == NULL )
        {
			return( -1 );
		}

		Buf[NumStr] = (FONTID)Idx;
		NumStr++;
	}

#endif

	return( NumStr );
}


///---------------------------------------------------------------------------
///-- Setfontg
///---------------------------------------------------------------------------
///-- ���������� ������ Font �� �����Ѵ�.
bool CFontasm::Setfontg_asm( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco, bool ab_smooth, int al_width )
{
	TCHAR* Name = NULL;
//	int l_nDevSkip = GetPrivateProfileInt( "Local", "Language", 0, "./setup.ini");
	
	///-- font ����
	thePrevFont	= FontType;
	
	switch( thePrevFont )	
	{
	case n_ftGulimChe:
		{
			if( g_RockClient.GetLanguage() == eLang_Korea )  
			{
				//Name = _T( "���� ���" );
				Name = _T( "����ü" );
#ifdef APPLY_BOLD_FONT				
				Weight = n_fwBold;
#endif
				//Name = _T( "SECPTB033" );
				//Name = _T( "SECPTL033" );
				//Name = _T( FONTSTR_ALL ); 
			}
			else if( g_RockClient.GetLanguage() == eLang_Japan )  
			{
				Name = _T( "�l�r �S�V�b�N" );  
			}
			else if( g_RockClient.GetLanguage() == eLang_China )
			{
				Name = _T( "SIMSUN" );
			}	
			else if (g_RockClient.GetLanguage() == eLang_Taiwan )
			{
				Name = _T( "mingliu");
			}
			else if (g_RockClient.GetLanguage() == eLang_HongKong )
			{
				Name = _T( "mingliu");
			}
			else
			{
				Name = _T( FONTSTR_ALL );			
			}
		}
		break;
	case n_ftDotum:
		{
			if( g_RockClient.GetLanguage() == eLang_Korea )  
			{
				Name = _T( "HY�︪��M" );
#ifdef APPLY_BOLD_FONT				
				Weight = n_fwBold;
#endif
			}
			else if( g_RockClient.GetLanguage() == eLang_Japan ) 
			{
				Name = _T( "�l�r ����" ); 
			}
			else if( g_RockClient.GetLanguage() == eLang_China )
			{
				Name = _T( "SIMSUN" ); 
			}	
			else if(g_RockClient.GetLanguage() == eLang_Taiwan  ) 
			{
				Name = _T( "mingliu" ); 
			}
			else if(g_RockClient.GetLanguage() == eLang_HongKong  ) 
			{
				Name = _T( "mingliu" ); 
			}
			else
			{
				Name = _T( FONTSTR_ALL ); 
			}
		}
		break;
	default:
		{
			if( g_RockClient.GetLanguage() == eLang_Korea )  
			{
				//Name = _T( "���� ���" );
				Name = _T( "����ü" );
#ifdef APPLY_BOLD_FONT				
				Weight = n_fwBold;
#endif
				//Name = _T( "SECPTB033" );
				//Name = _T( "SECPTL033" ); 
				//Name = _T( FONTSTR_ALL );
			}
			else if( g_RockClient.GetLanguage() == eLang_Japan )
			{				
				Name = _T( "�l�r �S�V�b�N" );  
			}
			else if( g_RockClient.GetLanguage() == eLang_China )
			{
				Name = _T( "SIMSUN" ); 
			}	
			else if( g_RockClient.GetLanguage() == eLang_Taiwan )
			{
				Name = _T( "mingliu" ); 
			}
			else if( g_RockClient.GetLanguage() == eLang_HongKong )
			{
				Name = _T( "mingliu" ); 
			}
			else
			{
				Name = _T( FONTSTR_ALL ); 
			}
		}
		break;
	}
	  
	if( ( _tcsicmp( thePrevName, Name ) == 0 ) && ( thePrevSize == Size ) && ( thePrevWeight == Weight ) 
		 && ( thePrevDeco == Deco ) && ( thePrevSmooth == ab_smooth ) )
	{
		return( false ); // ���� ���� ���°� ���ٸ� �׳�..����
	}
	//if( IsTTFont( Name ) )// TTFont�� �ƴϸ� ����
	//{
	//	return( false );
	//}
	 
	strcpy( thePrevName, Name );
	thePrevSize		= Size;
	thePrevWeight	= Weight;
	thePrevDeco		= Deco;
	theFontWidth	= al_width;
	thePrevSmooth	= ab_smooth;
	
	if( ab_smooth )
	{
		theSmooth_Multi = 2;
	}
	else
	{
		theSmooth_Multi = 1;
	}


	theTextureSize	= GetTextureSize( Size );
	theFontCharset	= GetTTFontCharset( thePrevName );

	ReleaseDevice();
	SetDevice( thePrevFont );

	return( true );
}


///---------------------------------------------------------------------------
///-- SetDevice
///---------------------------------------------------------------------------
///-- Texture �� ����� ���� font, bmp �� �غ��Ѵ�.
void CFontasm::SetDevice( NFontType FontType )
{
	thehDC				= GetDC( thehWnd );

	///-----------------------------------------------------------------------
	///-- logfont����ü ä���

	
	FillMemory( &theLogfont, sizeof(LOGFONT), 0 );

	strcpy( theLogfont.lfFaceName, thePrevName);			//font_name);//Ÿ�����̽�
//	theLogfont.lfHeight		= -MulDiv(thePrevSize, GetDeviceCaps(thehDC, LOGPIXELSY), 72); // 0; //thePrevSize;				//font_size;     // * GetDeviceCaps(hDC, LOGPIXELSY) / 72; //size = 30;
//	theLogfont.lfHeight			= thePrevSize;				//font_size;     // * GetDeviceCaps(hDC, LOGPIXELSY) / 72; //size = 30;
	theLogfont.lfUnderline		= FALSE;						//���� ��Ÿ��
	theLogfont.lfItalic			= FALSE;						//    "
	theLogfont.lfStrikeOut		= FALSE;						//    "
//	theLogfont.lfQuality		= PROOF_QUALITY;				//����� ��� ǰ���� ���� >������ ��Ʈ���� ���

//	theLogfont.lfQuality		= ANTIALIASED_QUALITY;				

#if defined (ONLY_ENGLISH_SET) 
	theLogfont.lfCharSet		= ANSI_CHARSET;			//HANGUL_CHARSET; // ANSI_CHARSET;	//���ڼ�
#else
	theLogfont.lfCharSet		= DEFAULT_CHARSET;			//HANGUL_CHARSET; // ANSI_CHARSET;	//���ڼ�
#endif

	theLogfont.lfWeight			= thePrevWeight;			//FW_BOLD;//FW_MEDIUM;//FW_NORMAL;	//���� ���� 0�̸� ����Ʈ
	theLogfont.lfOutPrecision	= OUT_OUTLINE_PRECIS;			//OUT_OUTLINE_PRECIS;		//OUT_STROKE_PRECIS;			//��� ��Ȯ��
	theLogfont.lfClipPrecision	= CLIP_CHARACTER_PRECIS;		//���� �Ϻκ��� Ŭ���� ������ ������� ó�� ���
	theLogfont.lfWidth			= theFontWidth;

	//theLogfont.lfWidth          �ַ� 0�� ��� > ������ ���� �������̶� �� �ǹ̰� ����. 
	//theLogfont.lfPitchAndFamily ��Ʈ�� ��ġ�� �йи��� ����  "��ġ|�йи�"�������� OR�����ڷ� ���� ����
	//theLogfont.lfEscapement     ��ġ�� x����� ������ 1/10�� ������ ����
	//theLogfont.lfQuality        ���� ������ ���� > win98������ lf.lfEscapement���� ������ ���� ����	
	
	

	///-----------------------------------------------------------------------
	///-- Font �� ���� ���μ���

	int nFontSize = thePrevSize;

	switch( FontType )	
	{
		///-- ����ü : �⺻
	case n_ftGulimChe:
		{			
			nFontSize -= 3;				
			theLogfont.lfQuality		= PROOF_QUALITY;			
		}
		break;
		
		///-- �︪��ü : ������ ��ȭ 32 / 64	
		///-- !!!! Antialiased �� size 14 ���� �����Ǹ� ���ϴ� ���õȴ�.
	case n_ftDotum:
		{				
			///-- Size
			if( nFontSize >= 12)
			{
				theTextureSize = 32;
			}			
			if( nFontSize >= 23 )
			{
				theTextureSize = 64;
			}
			
			theLogfont.lfQuality		= ANTIALIASED_QUALITY;
		}
		break;
	default:
		{
			nFontSize -= 3;				
			theLogfont.lfQuality		= PROOF_QUALITY;				
		}
		break;
	}

	theLogfont.lfHeight	= -MulDiv( nFontSize, GetDeviceCaps(thehDC, LOGPIXELSY), 72); 
	
	theTextureSize *= theSmooth_Multi;
	theLogfont.lfWidth *= theSmooth_Multi;
	theLogfont.lfHeight *= theSmooth_Multi;	
	//-------------------------------------------------------------------------
	/*if( g_RockClient.GetLanguage() == eLang_Korea )
	{
		thehFont = CreateFont( theLogfont.lfHeight, theLogfont.lfWidth,
							   theLogfont.lfEscapement, theLogfont.lfOrientation,
							   theLogfont.lfWeight, theLogfont.lfItalic,
							   theLogfont.lfUnderline, theLogfont.lfStrikeOut,
							   theLogfont.lfCharSet, theLogfont.lfOutPrecision,
							   theLogfont.lfClipPrecision, theLogfont.lfQuality,
							   theLogfont.lfPitchAndFamily, theLogfont.lfFaceName );
	}
	else*/
	{
		thehFont		= CreateFontIndirect( &theLogfont );
	}
	
	//thehOldFont		= 
		
	thehNomOldFont = (HFONT)SelectObject( thehNomMemDC, thehFont );		
	thehShdOldFont = (HFONT)SelectObject( thehShdMemDC, thehFont );		
	//theLogfont.lfHeight /= theSmooth_Multi;

	///-------------------------------------------------------------------------
	///-- Normal Bitmap
	thehNomBitmap		= CreateCompatibleBitmap( thehDC, theTextureSize, theTextureSize );
	thehNomOldBitmap	= (HBITMAP)SelectObject( thehNomMemDC, thehNomBitmap );

	thehShdBitmap		= CreateCompatibleBitmap( thehDC, theTextureSize, theTextureSize );
	thehShdOldBitmap	= (HBITMAP)SelectObject( thehShdMemDC, thehShdBitmap );

	//-------------------------------------------------------------------------
	///-- Shadow Bitmap
	thehBrush		= CreateSolidBrush( RGB( 0, 0, 0 ) );
	thehNomOldBrush	= (HBRUSH)SelectObject( thehNomMemDC, thehBrush ); 
	thehShdOldBrush	= (HBRUSH)SelectObject( thehShdMemDC, thehBrush ); 

	//-------------------------------------------------------------------------
	// fill .bmp - structures
	//-------------------------------------------------------------------------	
	theBmpInfo.bmiHeader.biSize          = sizeof( theBmpInfo.bmiHeader );
	theBmpInfo.bmiHeader.biWidth         = theTextureSize;
	theBmpInfo.bmiHeader.biHeight        = theTextureSize;
	theBmpInfo.bmiHeader.biPlanes        = 1;
	theBmpInfo.bmiHeader.biBitCount      = 24;
	theBmpInfo.bmiHeader.biCompression   = BI_RGB;
	theBmpInfo.bmiHeader.biSizeImage     = theTextureSize * theTextureSize * 3; 
	theBmpInfo.bmiHeader.biXPelsPerMeter = 0;
	theBmpInfo.bmiHeader.biYPelsPerMeter = 0;
	theBmpInfo.bmiHeader.biClrUsed       = 0;
	theBmpInfo.bmiHeader.biClrImportant  = 0;


	theRgbBuffer = (UCHAR*)malloc( theTextureSize * theTextureSize * 3 ); 
	theFlipBuffer = (UCHAR*)malloc( theTextureSize * theTextureSize * 2 );

	if( theFlipBuffer == NULL )
	{
		theFlipBuffer = (UCHAR*)malloc( theTextureSize * theTextureSize * 2 );
		void *a = VirtualAlloc(theFlipBuffer , theTextureSize * theTextureSize * 2 , MEM_COMMIT|MEM_RESERVE ,PAGE_READWRITE);
		DWORD dwLastError = GetLastError();

		if(theFlipBuffer ==NULL )
		{
			theFlipBuffer = new UCHAR[theTextureSize * theTextureSize * 2];
		}
		
		
		
	}

	ReleaseDC( thehWnd , thehDC );
}

//-----------------------------------------------------------------------------
void CFontasm::ReleaseDevice()
{
	if( theFlipBuffer != NULL )
	{
		free( theFlipBuffer );
	}
	if( theRgbBuffer != NULL )
	{
		free( theRgbBuffer );
	}
	

	SelectObject( thehNomMemDC, thehNomOldBitmap );
	SelectObject( thehNomMemDC, thehNomOldBrush );
	SelectObject( thehNomMemDC, thehNomOldFont );

	SelectObject( thehShdMemDC, thehShdOldBitmap );
	SelectObject( thehShdMemDC, thehShdOldBrush );
	SelectObject( thehShdMemDC, thehShdOldFont );

	DeleteObject( thehFont );
	DeleteObject( thehBrush );
	DeleteObject( thehShdBitmap );
	DeleteObject( thehNomBitmap );
}

///---------------------------------------------------------------------------
///-- MakeFontg
///---------------------------------------------------------------------------
///-- ���ں��� Texture �� �����Ѵ�.

///-- 2005.04.14 / Lyul
///-- Color ��ȯ ��� ����

SFont* CFontasm::MakeFontg( TCHAR* Ch )
{
	SIZE Size;		//Decoration Font
	Size.cx = 0;
	Size.cy = 0;
	
	SIZE RISize;		//���� ���ڻ�����	//by simwoosung

	//-------------------------------------------------------------------------
	Rectangle( thehNomMemDC, 0, 0, theTextureSize, theTextureSize );
	Rectangle( thehShdMemDC, 0, 0, theTextureSize, theTextureSize );
	
	switch( thePrevDeco )
	{
	case n_fdNormal:
		DecorationNormal( Ch, &Size, &RISize );
		break;
	case n_fdShadow:
		DecorationShadow( Ch, &Size, &RISize );
		break;
	case n_fdOutline:
		DecorationOutline( Ch, &Size, &RISize );
		break;
	case n_fdUserColor:
		DecorationUserColor(Ch,  &Size, &RISize); 
		break;
	default: // n_fdNormal
		DecorationNormal( Ch, &Size, &RISize );
		break;
	}
	
		
	//��Ʈ ��ü ����
	//-------------------------------------------------------------------------

	SFont* tmpFont = SAFE_NEW( SFont );	
	if( tmpFont == NULL )
	{
//		MessageBox( NULL, _T("��Ʈ ��ü ���� ���� - SFont"), _T("�˸�"), MB_OK );
		return( NULL );
	}

	tmpFont->nTexWidth = theTextureSize;
	tmpFont->nTexHeight = theTextureSize;

	///-- D3DFMT_A4R4G4B4
	if( FAILED( g_RenderManager.CreateTexture( theTextureSize, theTextureSize, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &tmpFont->pTexture ) ) )
	{
		SAFE_DELETE(tmpFont);		
//	    MessageBox( NULL, _T("��Ʈ �ؽ��� ���� ���� - CreateTexture"), _T("�˸�"), MB_OK );
		return( NULL );
	}

	//��Ʈ�� ���İ� �߰�
	BYTE* tmpFontData;//( 0 )
	WORD* TexData;//texel....
		
	//���ۿ��� ��Ʈ �ؽ��ķ� ����

	D3DLOCKED_RECT FontTextureRect;
	tmpFont->pTexture->LockRect( 0, &FontTextureRect, NULL, 0 );

	tmpFontData = (BYTE*)FontTextureRect.pBits;	

	///-----------------------------------------------------------------------
	///-- Shadow , Outline
	///-----------------------------------------------------------------------
	if( thePrevDeco == n_fdShadow || thePrevDeco == n_fdOutline )
	{
		///-- Get RGBData
		GetDIBits( thehShdMemDC, thehShdBitmap, 0, theTextureSize, theRgbBuffer, &theBmpInfo, DIB_RGB_COLORS );
		
		//-------------------------------------------------------------------------
		///-- Color ��ȯ A8R8G8B8 -> D3DFMT_A4R4G4B4
		for( int i = 0; i < theTextureSize; ++i )//height
		{
			TexData = (WORD*)( &tmpFontData[ i * FontTextureRect.Pitch ] );
			
			for( int j = 0; j < theTextureSize; ++j )//width
			{
				int r,g,b,a;
				
				r = theRgbBuffer[ ( i * theTextureSize * 3 ) + ( j * 3 ) + 0]  ;
				g = theRgbBuffer[ ( i * theTextureSize * 3 ) + ( j * 3 ) + 1 ] ;
				b = theRgbBuffer[ ( i * theTextureSize * 3 ) + ( j * 3 ) + 2 ] ;
				a = ( r+g+b )* 5 / 255;///a = ( r+g+b )* 15 / 255 / 3;
				
				///-- Black �� Alpha
				TexData[j] = (a << 12) | 0x0444;
			}
		}

	}
	
	if( thePrevDeco == n_fdUserColor )
	{
		///-- Get RGBData
		GetDIBits( thehShdMemDC, thehShdBitmap, 0, theTextureSize, theRgbBuffer, &theBmpInfo, DIB_RGB_COLORS );
		
		//-------------------------------------------------------------------------
		///-- Color ��ȯ A8R8G8B8 -> D3DFMT_A4R4G4B4
		for( int i = 0; i < theTextureSize; ++i )//height
		{
			TexData = (WORD*)( &tmpFontData[ i * FontTextureRect.Pitch ] );
			
			for( int j = 0; j < theTextureSize; ++j )//width
			{
				int r,g,b,a;
				
				r = theRgbBuffer[ ( i * theTextureSize * 3 ) + ( j * 3 ) + 0]  ;
				g = theRgbBuffer[ ( i * theTextureSize * 3 ) + ( j * 3 ) + 1 ] ;
				b = theRgbBuffer[ ( i * theTextureSize * 3 ) + ( j * 3 ) + 2 ] ;
				a = ( r+g+b )* 5 / 255;///a = ( r+g+b )* 15 / 255 / 3;
				
				///-- Black �� Alpha
				if( r!=0 ||	g!=0 || b!=0)
				{
					TexData[j] = (a << 12) | 0x0fff;
				}
				else
				{
					TexData[j] = (a << 12) | 0x0fff;
				}
				
			}
		}

	}
	
	///-----------------------------------------------------------------------
	///-- Normal
	///-----------------------------------------------------------------------
	///-- Color ��ȯ A8R8G8B8 -> D3DFMT_A4R4G4B4
	///-- Color �� 0�� ��츸 

	///-- Get RGBData
    GetDIBits( thehNomMemDC, thehNomBitmap, 0, theTextureSize, theRgbBuffer, &theBmpInfo, DIB_RGB_COLORS );
	
	
	//-------------------------------------------------------------------------	
    for( int i = 0; i < theTextureSize; ++i )//height
	{
		TexData = (WORD*)( &tmpFontData[ i * FontTextureRect.Pitch ] );

		for( int j = 0; j < theTextureSize; ++j )//width
		{
			int r,g,b,a;
	
			r = theRgbBuffer[ ( i * theTextureSize * 3 ) + ( j * 3 ) + 0]  * 15 / 255;
			g = theRgbBuffer[ ( i * theTextureSize * 3 ) + ( j * 3 ) + 1 ] * 15 / 255;
			b = theRgbBuffer[ ( i * theTextureSize * 3 ) + ( j * 3 ) + 2 ] * 15 / 255;
			a = ( r+g+b ) / 3;
			
			///-- Color Mapping
			if( r!=0 && g!=0 && b!=0)
			{				
				///-- White �� Alpha
						///-- White �� Alpha
				if(thePrevDeco == n_fdUserColor)
				{
					TexData[j] = (a << 12) | 0x0f00;
				}
				else
				{
					TexData[j] = (a << 12) | 0x0fff;
				}
				
			}	
			else
			{
				///-- Normal �ϰ�� �̳��� ���ߵȴ�.
				if( thePrevDeco == n_fdNormal )
				{					
					TexData[j] = 0x0fff;//(a << 12) | 0x0fff;
				}
			}
		}
	}

	//���⼭...Texture�� ��Ʈ �̹����� ������ �ش�....(bmp����������..�������� ���±� ������..�ٽ� �����´�.)
	FlipBitmap( (UCHAR*)tmpFontData, FontTextureRect.Pitch, theTextureSize );//height
	
	tmpFont->pTexture->UnlockRect( 0 );
	
	//��Ʈ ���� ����
	//-------------------------------------------------------------------------
	tmpFont->color = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	
	tmpFont->x = 0;
	tmpFont->y = 0;
	///-- Stretch
	tmpFont->w = Size.cx / theSmooth_Multi;// + size_supplement (��Ʈ��Ŀ� ���� ������ ����)
	tmpFont->h = Size.cy / theSmooth_Multi;// + size_supplement

	//���� ���ڸ��� �����ʺ� - by simwoosung
	tmpFont->Riw = RISize.cx;
	tmpFont->Rih = RISize.cy;

	tmpFont->vt[0].x = (float)tmpFont->x;
	tmpFont->vt[0].y = (float)tmpFont->y;
	tmpFont->vt[0].z = 0.0f;
	tmpFont->vt[0].w = 1.0f;
	tmpFont->vt[0].diffuse = tmpFont->color;

	tmpFont->vt[1].x = tmpFont->vt[0].x + tmpFont->w;
	tmpFont->vt[1].y = tmpFont->vt[0].y;
	tmpFont->vt[1].z = 0.0f;
	tmpFont->vt[1].w = 1.0f;
	tmpFont->vt[1].diffuse = tmpFont->color;

	tmpFont->vt[2].x = tmpFont->vt[0].x + tmpFont->w;
	tmpFont->vt[2].y = tmpFont->vt[0].y + tmpFont->h;
	tmpFont->vt[2].z = 0.0f;
	tmpFont->vt[2].w = 1.0f;
	tmpFont->vt[2].diffuse = tmpFont->color;

	tmpFont->vt[3].x = tmpFont->vt[0].x;
	tmpFont->vt[3].y = tmpFont->vt[0].y + tmpFont->h;
	tmpFont->vt[3].z = 0.0f;
	tmpFont->vt[3].w = 1.0f;
	tmpFont->vt[3].diffuse = tmpFont->color;

	//��ġ�� ũ�⸦ �̿��ؼ� Image�� UV��ǥ�� ����Ѵ�.
	//-------------------------------------------------------------------------
	tmpFont->vt[0].tu = tmpFont->vt[0].x * theSmooth_Multi/ theTextureSize;	//width
	tmpFont->vt[0].tv = tmpFont->vt[0].y * theSmooth_Multi/ theTextureSize;	//height

	tmpFont->vt[1].tu = tmpFont->vt[1].x * theSmooth_Multi/ theTextureSize;	//width
	tmpFont->vt[1].tv = tmpFont->vt[1].y * theSmooth_Multi/ theTextureSize;	//heigth

	tmpFont->vt[2].tu = tmpFont->vt[2].x * theSmooth_Multi/ theTextureSize;	//width
	tmpFont->vt[2].tv = tmpFont->vt[2].y * theSmooth_Multi/ theTextureSize;	//height

	tmpFont->vt[3].tu = tmpFont->vt[3].x * theSmooth_Multi/ theTextureSize;	//width
	tmpFont->vt[3].tv = tmpFont->vt[3].y * theSmooth_Multi/ theTextureSize;	//height
	
	return( tmpFont );
}


///---------------------------------------------------------------------------
///-- CreateDBCS
///---------------------------------------------------------------------------
///-- DBCS �� String �� Texture �� �����.
int CFontasm::CreateDBCS( SFont** apps_fontasm, const char* const apc_str, FONTID * const lps_font_id_buf )
{
	int li_idx( 0 );
	int li_len( 0 );
	int li_num_of_str( 0 );
	
    if( apc_str == NULL )
	{
		return( -1 );
	}

    li_len = strlen( apc_str );
    
	///-- String �� �� ���ڿ� ���Ͽ� Access
	for( int i = 0; i < li_len; ++i )
    {
		li_idx = DetectionDBCS( &i, apc_str );
        
        if( apps_fontasm[li_idx] == NULL )
        {
			apps_fontasm[li_idx] = MakeFontg( (TCHAR*)&li_idx );
		}

		lps_font_id_buf [li_num_of_str] = (FONTID)li_idx;
		li_num_of_str++;
	}

	return( li_num_of_str );
}

//-----------------------------------------------------------------------------
int CFontasm::CreateUNICODE( SFont** Fontasm, TCHAR* pStr, FONTID* Buf )
{
	int Idx( 0 );
	int Len( 0 );
	int NumStr( 0 );
	
    if( pStr == NULL )
	{
		return( -1 );
	}

    Len = Rstrlen( RWCHAR(pStr) );
    RTCHAR *Str = RWCHAR(pStr);

	for( int i = 0; i < Len; ++i )
    {
		Idx = Str[i];

		if( Fontasm[Idx] == NULL )
        {
			Fontasm[Idx] = MakeFontg( (char*)&Idx );
		}

		Buf[NumStr] = (FONTID)Idx;
		NumStr++;
	}

	return( NumStr );
}

//-----------------------------------------------------------------------------
int CFontasm::DetectionDBCS( int* const li_count, const char* const apc_str )
{
	static char Db[2] = "";
	static char Ch;
	int *Idx = NULL;

    //------------------------------------------------------------
    //ù����Ʈ�� 0x80�̻��̸� DBCS....
    //------------------------------------------------------------
    if( IsDBCSLeadByteEx( CP_ACP, apc_str[*li_count] ) )
    {	//DBCS
	    Db[0] = apc_str[*li_count];  
	    Db[1] = apc_str[(*li_count)+1];      

		if( strlen( Db ) )
		{
			Idx = (int*)Db;
		}
	    (*li_count)++;
    }
    else
    {   //ASCII
	    Ch    = apc_str[(*li_count)];
	    Db[0] = Ch;
	    Db[1] = 0;

		if( strlen( Db ) )
		{
			Idx = (int*)Db;
		}
    }
    
	if( Idx )
	{
		return( *Idx );
	}

    return 0;
}


///---------------------------------------------------------------------------
///-- DecorationNormal
///---------------------------------------------------------------------------
///-- 2005.04.15 / Lyul / ����
///-- NomMemDC �� ���
void CFontasm::DecorationNormal( TCHAR* Ch, SIZE* Size, SIZE* RISize )
{
	int StrLen = Rstrlen( RWCHAR(Ch) );
	SetBkMode( thehNomMemDC, TRANSPARENT );
	SetTextColor( thehNomMemDC, RGB( 255, 255, 255 ) );
	SetBkColor( thehNomMemDC, RGB( 0, 0, 0 ) );

#ifdef UNICODE_ROCK
	TextOutW( thehNomMemDC, 0, 0, RCASTCTOW(Ch), StrLen );
	GetTextExtentPoint32W( thehNomMemDC, RCASTCTOW(Ch), StrLen, Size );
#else
	TextOut( thehNomMemDC, 0, 0, Ch, StrLen );
	GetTextExtentPoint32( thehNomMemDC, Ch, StrLen, Size );
#endif
	///-- ���ڿ� ũ�� �˾ƺ���

	RISize->cx = Size->cx;
	RISize->cy = Size->cy;
}


///---------------------------------------------------------------------------
///-- DecorationShadow
///---------------------------------------------------------------------------
///-- 2005.04.15 / Lyul / ����

///-- Shadow �� ���� ���ڸ� ����Ѵ�.
///-- ����ũ�⿡ ���� Shadow ��ħ�� �����ϰ�

///-- ShdMemDC �� Shadow ���
void CFontasm::DecorationShadow( TCHAR* Ch, SIZE* Size, SIZE* RISize )
{	
	int StrLen = Rstrlen( RWCHAR(Ch) );
	int li_shd_dist;

	///-- Normal	
	SetBkMode( thehNomMemDC, TRANSPARENT );
	SetBkColor( thehNomMemDC, RGB( 0, 0, 0 ) );
	Rectangle( thehNomMemDC, 0, 0, theTextureSize, theTextureSize );
	SetTextColor( thehNomMemDC, RGB( 255, 255, 255 ) );
#ifdef UNICODE_ROCK
	TextOutW( thehNomMemDC, 0, 0, RWCHAR(Ch), StrLen );
	//���ڿ� ũ�� �˾ƺ���
	GetTextExtentPoint32W( thehNomMemDC, RWCHAR(Ch), StrLen, Size );
#else 
	TextOut( thehNomMemDC, 0, 0, Ch, StrLen );
	//���ڿ� ũ�� �˾ƺ���
	GetTextExtentPoint32( thehNomMemDC, Ch, StrLen, Size );
#endif
	
	RISize->cx = Size->cx;
	RISize->cy = Size->cy;

	///-- �׸��� Dist
	if( thePrevFont == n_ftGulimChe )
	{
		li_shd_dist = 1;
	}
	else
	{
		li_shd_dist = 1 + Size->cy / 24;
		///-- li_shd_dist = 1 + Size->cy / 12 ;		///-- ����ü�� ��� �ʹ� ŭ
	}	

	///-- Shadow
	SetBkMode( thehShdMemDC, TRANSPARENT );
	SetBkColor( thehShdMemDC, RGB( 0, 0, 0 ) );
	SetTextColor( thehShdMemDC, RGB( 255, 255, 255 ) );
#ifdef UNICODE_ROCK
	TextOutW( thehShdMemDC, li_shd_dist, li_shd_dist, RWCHAR(Ch), StrLen );
#else
	TextOut( thehShdMemDC, li_shd_dist, li_shd_dist, Ch, StrLen );	
#endif
	Size->cx += li_shd_dist; 
	Size->cy += li_shd_dist;
}
void CFontasm::DecorationUserColor( TCHAR* Ch, SIZE* Size, SIZE* RISize )
{

	int i( 0 );
	int j( 0 );
	int Len = Rstrlen( RWCHAR(Ch) );
	int li_shd_dist;

#ifdef UNICODE_ROCK
	TextOutW( thehNomMemDC, i, j, RWCHAR(Ch), Len );
	///-- ���ڿ� ũ�� �˾ƺ���
	GetTextExtentPoint32W( thehNomMemDC, RWCHAR(Ch), Len, Size );
#else
	TextOut( thehNomMemDC, i, j, Ch, Len );
	///-- ���ڿ� ũ�� �˾ƺ���
	GetTextExtentPoint32( thehNomMemDC, Ch, Len, Size );
#endif
	RISize->cx = Size->cx;
	RISize->cy = Size->cy;

	Rectangle( thehNomMemDC, 0, 0, theTextureSize, theTextureSize );
	
	///-- �׸��� Dist
	///-- �׸��� Dist
	if( thePrevFont == n_ftGulimChe )
	{
		li_shd_dist = 1;
	}
	else
	{	
		li_shd_dist = 1 + Size->cy / 24;
		///-- li_shd_dist = 1 + Size->cy / 16 ;    ����ü�� ��� �ʹ� ŭ
	}

	///-- Outline
	SetBkMode( thehShdMemDC, TRANSPARENT );
    SetBkColor( thehShdMemDC, RGB( 0, 0, 0 ) );
	SetTextColor( thehShdMemDC, RGB( 255, 255, 255 ) );

    for( i=0; i<=2; i++ )
	{
		for( j=0; j<=2; j++ )
		{
			//if( ( i == 1 ) && ( j == 1 ) )
			//{
			//	break;
			//}
#ifdef UNICODE_ROCK
			TextOutW( thehShdMemDC, i * li_shd_dist, j * li_shd_dist, RWCHAR(Ch), Len );
#else
			TextOut( thehShdMemDC, i * li_shd_dist, j * li_shd_dist, Ch, Len );
#endif
	
		}
	}
	

	///-- Normal
	SetBkMode( thehNomMemDC, TRANSPARENT );
    SetBkColor( thehNomMemDC, RGB( 0, 0, 0 ) );	
	SetTextColor( thehNomMemDC, RGB( 255, 255, 255 ) );
#ifdef UNICODE_ROCK
	TextOutW( thehNomMemDC, li_shd_dist, li_shd_dist, RWCHAR(Ch), Len );
#else
	TextOut( thehNomMemDC, li_shd_dist, li_shd_dist, Ch, Len );
#endif
	
	Size->cx += li_shd_dist * 2; 
	Size->cy += li_shd_dist * 2;

}


///---------------------------------------------------------------------------
///-- DecorationOutline
///---------------------------------------------------------------------------
///-- 2005.04.15 / Lyul / ����

///-- �ܰ��� �� ���� ���ڸ� ����Ѵ�.
///-- ����ũ�⿡ ���� Shadow ��ħ�� �����ϰ�

///-- ShdMemDC �� Shadow ���
void CFontasm::DecorationOutline( TCHAR* Ch, SIZE* Size, SIZE* RISize )
{	
	int i( 0 );
	int j( 0 );
	int Len = Rstrlen( RWCHAR(Ch) );
	int li_shd_dist;

#ifdef UNICODE_ROCK
	TextOutW( thehNomMemDC, i, j, RWCHAR(Ch), Len );
	///-- ���ڿ� ũ�� �˾ƺ���
	GetTextExtentPoint32W( thehNomMemDC, RWCHAR(Ch), Len, Size );
#else
	TextOut( thehNomMemDC, i, j, Ch, Len );
	///-- ���ڿ� ũ�� �˾ƺ���
	GetTextExtentPoint32( thehNomMemDC, Ch, Len, Size );
#endif	

	RISize->cx = Size->cx;
	RISize->cy = Size->cy;

	Rectangle( thehNomMemDC, 0, 0, theTextureSize, theTextureSize );
	
	///-- �׸��� Dist
	///-- �׸��� Dist
	if( thePrevFont == n_ftGulimChe )
	{
		li_shd_dist = 1;
	}
	else
	{	
		li_shd_dist = 1 + Size->cy / 24;
		///-- li_shd_dist = 1 + Size->cy / 16 ;    ����ü�� ��� �ʹ� ŭ
	}

	///-- Outline
	SetBkMode( thehShdMemDC, TRANSPARENT );
    SetBkColor( thehShdMemDC, RGB( 0, 0, 0 ) );
	SetTextColor( thehShdMemDC, RGB( 255, 255, 255 ) );

    for( i=0; i<=2; i++ )
	{
		for( j=0; j<=2; j++ )
		{
			//if( ( i == 1 ) && ( j == 1 ) )
			//{
			//	break;
			//}
#ifdef UNICODE_ROCK
			TextOutW( thehShdMemDC, i * li_shd_dist, j * li_shd_dist, RWCHAR(Ch), Len );
#else
			TextOut( thehShdMemDC, i * li_shd_dist, j * li_shd_dist, Ch, Len );
#endif
		}
	}
	

	///-- Normal
	SetBkMode( thehNomMemDC, TRANSPARENT );
    SetBkColor( thehNomMemDC, RGB( 0, 0, 0 ) );	
	SetTextColor( thehNomMemDC, RGB( 255, 255, 255 ) );
#ifdef UNICODE_ROCK
	TextOutW( thehNomMemDC, li_shd_dist, li_shd_dist, RWCHAR(Ch), Len );
#else
	TextOut( thehNomMemDC, li_shd_dist, li_shd_dist, Ch, Len );
#endif
	
	Size->cx += li_shd_dist * 2; 
	Size->cy += li_shd_dist * 2;
}

//-----------------------------------------------------------------------------
int CFontasm::FlipBitmap( UCHAR* Img, int Bytes_per_line, int Height )
{
	// this function is used to flip bottom-up .BMP images
	int Idx;     // looping index

	// copy image to work area
	memcpy( theFlipBuffer, Img, Bytes_per_line * Height );

	// flip vertically
	for( Idx = 0; Idx < Height; ++Idx )
	{
		memcpy( &Img[( ( Height - 1 ) - Idx) * Bytes_per_line],
				&theFlipBuffer[Idx * Bytes_per_line],
				Bytes_per_line );
	}
	
	return( 1 );// return success
}

//-----------------------------------------------------------------------------
bool CFontasm::IsTTFont( TCHAR* Name )
{
	for( int i = 0; i < theNumTTFont; ++i )
	{
		if( _tcsicmp( theTTFont[i].lfFaceName, Name ) == 0 )
		{
			return( true );
		}
	}

	return( false );
}

///---------------------------------------------------------------------------
///-- GetTTFontCharset
///---------------------------------------------------------------------------
///-- 
NFontCharset CFontasm::GetTTFontCharset( TCHAR* Name )
{
	int Idx( 0 );
	for( Idx = 0; Idx < theNumTTFont; ++Idx )
	{		
		if( _tcsicmp( theTTFont[Idx].lfFaceName, Name ) == 0 )
		{			
			return( (NFontCharset)theTTFont[Idx].lfCharSet );
		}
	}
	
	return( n_fcEct );
}

//-----------------------------------------------------------------------------
int CFontasm::GetTextureSize( int Fontsize )
{
	bool IsNotFind( true );	// ũ�⸦ ã�Ҵ°�?
	int TextureSize( 16 );	// �⺻ �ؽ��� ũ�� ( 16 )
	
	do
	{
		if( Fontsize > ( TextureSize - thePrevDeco ) )
		{
			TextureSize = TextureSize << 1;	// 16 32 64 128 256 ....ũ�⸦ ã���� ����...����Ʈ
		}
		else
		{
			IsNotFind = false;	// ã������...
		}
		
	}while( IsNotFind );
	
	return( TextureSize );
}

//-----------------------------------------------------------------------------