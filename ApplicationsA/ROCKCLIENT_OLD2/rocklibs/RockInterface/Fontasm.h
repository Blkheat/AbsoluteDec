///////////////////////////////////////////////////////////////////////////////
///
///		File		: Fontasm.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-06
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __FONTASM_H__
#define __FONTASM_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9.h>
#else
#include <d3dx8.h>
#endif // DIRECT_VERSION_9_MJH

#include "Define.h"



#define		MAX_FONTNAME	32
#define		MAX_TTFONT		1024


#define		FONTSTR_KR "굴림체"
#define		FONTSTR_JP "굃굍 긕긘긞긏"
#define		FONTSTR_ALL "Arial"
#define		FONTSTR_CH "SIMHEI"




class CFontasm
{
public:
	CFontasm();
#ifdef DIRECT_VERSION_9_MJH
	CFontasm( HWND hWnd, LPDIRECT3DDEVICE9 d3dDevice );
#else
	CFontasm( HWND hWnd, LPDIRECT3DDEVICE8 d3dDevice );
#endif // DIRECT_VERSION_9_MJH
	~CFontasm();

	void EnumTTFontFamilies();								// TTFont 열거

	void SetTTFont( LOGFONT* TTFont );						// 폰트 열거중..TTFont List 설정
	void SetNumTTFont( int NumTTFont );						// TTFont 갯수 설정
	int GetNumTTFont();										// TTFont 갯수 얻기
	
	LOGFONT* GetTTFont( TCHAR* Name );						// 이름검색
	LOGFONT* GetTTFont( int Idx );							// 인덱스검색
	LOGFONT* GetTTFont( int Idx, NFontCharset Charset );	// 조건검색( 캐릭터셋으로 )
	
	bool IsTTFont( TCHAR* Name );
	NFontCharset GetTTFontCharset( TCHAR* Name );


	int Confirm( SFont** Fontasm, TCHAR* Str, FONTID* Buf );
	bool Setfontg_asm( NFontType FontType, int Size, NFontWeights Weight, NFontDecoration Deco, bool ab_smooth , int al_width );
	SFont* MakeFontg( TCHAR* Ch );

	int CreateUNICODE( SFont** Fontasm, TCHAR* Str, FONTID* Buf );	
	int CreateDBCS( SFont** apps_fontasm, const char* const apc_str, FONTID * const lps_font_id_buf );
	int DetectionDBCS( int* const li_count, const char* const apc_str );

	
private:
	void SetDevice( NFontType FontType );
	void ReleaseDevice();
	
	void DecorationNormal( TCHAR* Ch, SIZE* Size, SIZE* RISize );
	void DecorationShadow( TCHAR* Ch, SIZE* Size, SIZE* RISize );
	void DecorationOutline( TCHAR* Ch, SIZE* Size, SIZE* RISize );
	void DecorationUserColor( TCHAR* Ch, SIZE* Size, SIZE* RISize );
	
	int FlipBitmap( UCHAR* Img, int Bytes_per_line, int Height );
	int GetTextureSize( int Fontsize );
	
	

	TCHAR			thePrevName[MAX_FONTNAME];
	int				thePrevSize;
	int				theSmooth_Multi;
	int				theFontWidth;
	NFontWeights	thePrevWeight;
	NFontDecoration	thePrevDeco;
	NFontType		thePrevFont;
	bool			thePrevSmooth;

	int				theTextureSize;
	NFontCharset	theFontCharset;

	int				theNumTTFont;
	LOGFONT			theTTFont[MAX_TTFONT];
	

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9	theD3DDevice;
#else
	LPDIRECT3DDEVICE8	theD3DDevice;
#endif // DIRECT_VERSION_9_MJH

	HWND		thehWnd;
	HDC			thehDC;

	HDC			thehNomMemDC;
	HDC			thehShdMemDC;

	HBITMAP		thehNomBitmap;
	HBITMAP		thehNomOldBitmap;
	HBITMAP		thehShdBitmap;
	HBITMAP		thehShdOldBitmap;

	HFONT		thehFont;
	HFONT		thehNomOldFont;
	HFONT		thehShdOldFont;

	HBRUSH		thehBrush;
	HBRUSH		thehNomOldBrush;
	HBRUSH		thehShdOldBrush;

	LOGFONT		theLogfont;
	BITMAPINFO	theBmpInfo;

	UCHAR*		theRgbBuffer;
	UCHAR*		theFlipBuffer;
};


extern	CFontasm*		Fontasm;
//-----------------------------------------------------------------------------
#endif	__FONTASM_H__