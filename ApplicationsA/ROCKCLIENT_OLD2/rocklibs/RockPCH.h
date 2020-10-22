    ////////////////////////////////////////////////////////////////////////////////
///
///		@file
///
///		@brief	Precompiled Header.
///
///		Copyright (c) 2003, RockSoft Co., Ltd.
///		ALL RIGHTS RESERVED
///
///		RockEngine(R), Rock3D(R), RockServer(R) are registered trademarks of
///		RockSoft Co., Ltd.
///
///		Author	: Kim, ByungKee
///		Team	: Rock 3D team.
///		Date	: 2003-03-01
///
////////////////////////////////////////////////////////////////////////////////

#if !defined( __ROCK_PCH_H__ )
/// 해더 인클루드 정의
#define	__ROCK_PCH_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define DIRECT_VERSION_9_MJH // DX9 포팅
#define _USE_32BIT_TIME_T
//대만꼬는 여기서.. 유니코드 락을 푼다. 


#if defined( UNICODE ) && !defined( _UNICODE )
#	define _UNICODE
#endif
#if !defined( UNICODE ) && defined( _UNICODE )
#	define UNICODE
#endif

///-- Bounder Checker ( Dev Partner ) with STLport
///-- 이 Define 을 선언해 줘야만 Bounder Checker 사용시 STLport 에서 쓸 때 없는 Memory Leak 이 생기지 않는다.
/// #define _STLP_USE_NEWALLOC

enum EWINDOWS_MODE
{
	MODE_NORMAL_FULLMODE = 0 ,
	MODE_WINDOW_FULLMODE,
};


// Windows Header Files:
#include <winsock2.h>
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <windows.h>
#include <windowsx.h>

#include "RockWarningsOff.h"
#include <mmsystem.h>
#include "RockWarningsOn.h"

#include <commctrl.h>
#include <commdlg.h>

#ifndef	DIRECTINPUT_VERSION
	#define	DIRECTINPUT_VERSION	0x0800
#endif

#include <DInput.h>
#ifdef DIRECT_VERSION_9_MJH
#include <D3DX9.h>
#else
#include <D3DX8.h>
#endif // DIRECT_VERSION_9_MJH

// C RunTime Header Files
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <process.h>

//*
// C++ Header Files
#include "RockWarningsOff.h"
#	include <string>
#	include <vector>
#	include <deque>
#	include <set>
#	include <map>
#	include <algorithm>
#	include <functional>
#include "RockWarningsOn.h"
#include <dimm.h>

//#define USE_ROCKCRYPTO -> NetworkMgr.h 로 이동

#include "RockBase.h"
/// sooree network
#include "Protocols\\BaseDataStructures.h"


#include <GlobalDefine.h>
#include "HHMWorkDefine.h"


#ifdef TAIWAN_LOGIN_VERSION
	#define UNICODE_ROCK		//대만,홍콩 유니코드 시 적용시켜줘야 한다..	
#endif
#ifdef HONGKONG_LOGIN_VERSION
	#define UNICODE_ROCK		//대만,홍콩 유니코드 시 적용시켜줘야 한다..
#endif

#ifdef UNICODE
#	undef UNICODE
#endif 


void ClientClose(HWND hWnd = NULL ,  int Flag =NULL);

template< class T >
T * SAFE_NEW_FUNC(  T * tptr ) 
{
	if( tptr == NULL )
	{
		MessageBox( NULL, _T("Not enough memory.\nNot Enough Memory\n\nThis Program will now exit."), 
					_T("Rocksoft"), MB_OK );
		ClientClose();
	}
	
	return tptr;
}

template< class T >
T * SAFE_NEW_FUNC(  T * tptr, int size ) 
{
	if( tptr == NULL )
	{
		MessageBox( NULL, _T("Not enough memory.\nNot Enough Memory\n\nThis Program will now exit."), 
					_T("Rocksoft"), MB_OK );
		ClientClose();
	}	
	
	return tptr;
}

template< class T, class S >
DWORD  SAFE_TIME_SUB_FUNC( T time1, S time2 )
{
	DWORD dTime1 = (DWORD)time1;
	DWORD dTime2 = (DWORD)time2;
	
	DWORD dGabTime = 0;
	
	if( dTime2 > dTime1 )
	{
		return dGabTime;
	}

	dGabTime = (dTime1 - dTime2);

	return dGabTime;
}

template< class T, class S >
DWORD  SAFE_TIME_ADD_FUNC( T time1, S time2 )
{
	DWORD dTime1 = (DWORD)time1;
	DWORD dTime2 = (DWORD)time2;

	return ( dTime1 + dTime2 );
}

extern void SAFE_STR_CPY_FUNC(void * str1, void * str2, int nSize);
extern void SAFE_STR_CAT_FUNC(void * str1, void * str2, int nSize);
extern void SAFE_SPRINTF_FUNC(char * str1, char * str2, int nSize);

template< class T , class T2 >
void SAFE_STR_CPY_FUNC_TEMPLATE( T * str1, T2* str2, int nSize )
{
	SAFE_STR_CPY_FUNC( (void *)(str1) , (void *)(str2), nSize * sizeof(T) );
}

template< class T , class T2 >
void SAFE_STR_CAT_FUNC_TEMPLATE( T * str1, T2 * str2, int nSize )
{
	SAFE_STR_CAT_FUNC( (void *)(str1) , (void *)(str2), nSize * sizeof(T) );
}

template< class T, class T1 >
_inline size_t _Rstrcmp(T *s1 , T1* s2)
{
#ifdef UNICODE_ROCK
	return wcscmp(reinterpret_cast<wchar_t*>(s1) , reinterpret_cast<wchar_t*>(s2) );
#else
	return strcmp(reinterpret_cast<char*>(s1) , reinterpret_cast<char*>(s2));
#endif
}

template< class T, class T1 >
_inline void* _Rstrtok(T *s1 , T1* s2)
{
#ifdef UNICODE_ROCK
	return wcstok(reinterpret_cast<wchar_t*>(s1) , reinterpret_cast<wchar_t*>(s2) );
#else
	return strtok(reinterpret_cast<char*>(s1) , reinterpret_cast<char*>(s2));
#endif
}

template< class T, class T1 >
_inline void _Rstrcat(T *s1 , T1* s2)
{
#ifdef UNICODE_ROCK
	wcscat(reinterpret_cast<wchar_t*>(s1) , reinterpret_cast<wchar_t*>(s2) );
#else
	strcat(reinterpret_cast<char*>(s1) , reinterpret_cast<char*>(s2));
#endif
}

template< class T, class T1 >
_inline void _Rstrcpy(T *s1 , T1* s2)
{
#ifdef UNICODE_ROCK
	wcscpy(reinterpret_cast<wchar_t*>(s1) , reinterpret_cast<wchar_t*>(s2) );
#else
	strcpy(reinterpret_cast<char*>(s1) , reinterpret_cast<char*>(s2));
#endif
}

template< class T, class T1 >
_inline void _Rstrncat(T *s1 , T1* s2 ,size_t size )
{
#ifdef UNICODE_ROCK
	wcsncat(reinterpret_cast<wchar_t*>(s1) , reinterpret_cast<wchar_t*>(s2) , size  );
#else
	strncat(reinterpret_cast<char*>(s1) , reinterpret_cast<char*>(s2) , size );
#endif
}

template< class T, class T1 >
_inline void _Rstrncpy(T *s1 , T1* s2 , size_t size )
{
#ifdef UNICODE_ROCK
	wcsncpy(reinterpret_cast<wchar_t*>(s1) , reinterpret_cast<wchar_t*>(s2) , size  );
#else
	strncpy(reinterpret_cast<char*>(s1) , reinterpret_cast<char*>(s2) , size );
#endif
}


template< class T>
_inline size_t _Rstrlen(T *s1 )
{
#ifdef UNICODE_ROCK
	return wcslen(reinterpret_cast<wchar_t*>(s1));
#else
	return strlen(reinterpret_cast<char*>(s1) );
#endif
}



#ifdef UNICODE_ROCK
typedef wchar_t RTCHAR;
#define Rstrnset	wcsnset
#define Rsprintf	swprintf
#define Rwsprintf	wsprintfW
#define	Rstrlen		_Rstrlen
#define Rstrncpy	_Rstrncpy
#define	Rstrcpy		_Rstrcpy
#define	Rstrcat		_Rstrcat
#define	Rstrncat	_Rstrncat
#define	Rstrcmp		_Rstrcmp
#define Rstrncmp	wcsncmp
#define Rstrchr		wcschr
#define Rstrstr		wcsstr
#define Ritoa		_itow
#define Ratoi		_wtoi
#define Ratol		_wtol
#define Rstrtok		_Rstrtok
#define _ultoR		_ultow
#define Rsscanf		swscanf
#define	RMessageBox MessageBoxW
#define RFormatMessage FormatMessageW
//#define RCHAR		WCHAR
#define _RT(x)      L ## x
#define RWCHAR(x)	reinterpret_cast<wchar_t*>(x)
#define RCHAR(x)	reinterpret_cast<char*>(x)
#define RCASTCTOW(x)	reinterpret_cast<wchar_t*>(x)
#define RCASTWTOC(x)	reinterpret_cast<char*>(x)
#else
typedef char RTCHAR;
#define Rstrnset	strnset
#define Rsprintf	sprintf
#define Rwsprintf	wsprintfA
#define	Rstrlen		strlen
#define Rstrncpy	strncpy
#define	Rstrcpy		strcpy
#define	Rstrcat		strcat
#define	Rstrncat	strncat
#define	Rstrcmp		strcmp	
#define Rstrstr		strstr
#define Rstrchr		strchr
#define Ritoa		itoa
#define Ratoi		atoi
#define Ratol		atol
#define _ultoR		_ultoa
#define Rstrtok		strtok
#define Rsscanf		sscanf
#define Rstrncmp	strncmp
#define	RMessageBox MessageBoxA
#define RFormatMessage FormatMessageA
#define _RT(x)      x 
#define RWCHAR(x)	reinterpret_cast<char*>(x)
#define RCHAR(x)	reinterpret_cast<char*>(x)
#define RCASTCTOW(x)	reinterpret_cast<wchar_t*>(x)
#define RCASTWTOC(x)	reinterpret_cast<char*>(x)
#endif

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_NEW(p)  SAFE_NEW_FUNC( new p );
#define SAFE_NEW_ARRAY(p, size)  ( size ? SAFE_NEW_FUNC( new p[size] , size ) : NULL );
#define SAFE_TIME_SUB( time1 , time2 )  SAFE_TIME_SUB_FUNC( time1 , time2 )
#define SAFE_TIME_ADD( time1 , time2 )  SAFE_TIME_ADD_FUNC( time1,  time2 )
#define SAFE_TIME_COMPARE( time1 , Sign, time2 ) ( (DWORD)(time1) Sign (DWORD)(time2) )

#define SAFE_STR_CPY( str1 , str2, nSize )	SAFE_STR_CPY_FUNC_TEMPLATE( str1 , str2, nSize )
#define SAFE_STR_CAT( str1 , str2, nSize )	SAFE_STR_CAT_FUNC_TEMPLATE( str1 , str2, nSize )
#define SAFE_SPRINTF( str1 , str2, nSize )	SAFE_SPRINTF_FUNC( (char *)(str1) , (char *)(str2), nSize )


// Local Header Files
#define WORK_AROUND_SRCLINE_BUG
#define USE_BUGSLAYERUTIL
#include "BSU/BugslayerUtil.h"

#if !defined( _DEBUG ) && !defined( assert )
#	define assert( x )
#endif

RB_USING

// by dong
#define MAX_SIZE_32 32
#define MAX_SIZE_64 64
#define MAX_SIZE_128 128
#define MAX_SIZE_256 256
#define MAX_SIZE_512 512
#define MAX_SIZE_1024 1024 


#define	INVALID_DWORD_VALUE	0xFFFFFFFF
#define	INVALID_WORD_VALUE	0xFFFF
#define	INVALID_BYTE_VALUE	0xFF


#pragma comment( lib, "d3dx8.lib" )
#pragma comment( lib, "winmm.lib" )

#pragma warning( disable: 4018 4244 4800 4786)


///////////////////////////////////////////////////////////
/// 컴파일 옵션 : RockVirtualFileSystem으로 로딩 정의 
#define	LOAD_VF  
///////////////////////////////////////////////////////////

extern int g_Max_Effect_Display;

extern	void	DebugPrint( const char* pFormat,... );

//by simwoosung
void	ClientFilelog( char *buf, ... );
void	ClientFileLogW(WCHAR *buf , ...);
void	Filelog( char *buf, ... );
void	Filelog2( char *buf, ... );
void	Filelog3( char *buf, ... );

char * GetStringToken(char *strIn, char ch, char *strOut, int iOutSize );
void   DeleteTokenInString(void *strIn, RTCHAR ch);
void   StringTrim(void *strIn);
DWORD GetLastErorrCode() ;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//*/
#endif // __ROCK_PCH_H__
