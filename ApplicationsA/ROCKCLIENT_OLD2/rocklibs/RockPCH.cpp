////////////////////////////////////////////////////////////////////////////////
///
///		@file
///
///		@brief	Precompiled Header.
///
///		RochPCH.cpp : source file that includes just the standard includes
///		precompiled.pch will be the pre-compiled header
///		RochPCH.obj will contain the pre-compiled type information
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

#include <stdarg.h>
#include <time.h>
#include <stdio.h>

#include "RockPCH.h"
#include "RockClient.h"

// TODO: reference any additional headers you need in RockPCH.H
// and not in this file

extern BOOL g_LogPrint;

int g_Max_Effect_Display = 450;

// 1 은 기본죵료 0은 강제종료 입니다. 
extern void ClientClose(HWND hWnd ,  int Flag)
{
	if(hWnd == NULL)
	{
		SendMessage( g_RockClient.GetApp() , WM_CLOSE, NULL, NULL);
	}
	else 
	{
		SendMessage( hWnd , WM_CLOSE, NULL, Flag );
	}
	
}

void SAFE_STR_CPY_FUNC(void * str1, void * str2, int nSize)
{
	
#ifdef UNICODE_ROCK
	WCHAR* destStr = RWCHAR(str1);
	WCHAR* SrcStr = RWCHAR(str2);
	int wSize = nSize>>1;
	wcsncpy( destStr , SrcStr , wSize-1 );
	destStr[wSize-1] = '\0';
#else
	CHAR* destStr = RCHAR(str1);
	CHAR* SrcStr = RCHAR(str2);
	strncpy( destStr, SrcStr, nSize-1 );
	destStr[nSize-1] = '\0';
#endif
	
	
}

void SAFE_STR_CAT_FUNC(void * str1, void * str2, int nSize)
{
#ifdef UNICODE_ROCK
	WCHAR* destStr = RWCHAR(str1);
	WCHAR* SrcStr = RWCHAR(str2);
	int wSize = nSize>>1;
	int nLen = wSize - wcslen(destStr);	
	wcsncat( destStr, SrcStr , nLen - 1 );
	destStr[wSize-1] = '\0';
#else
	CHAR* destStr = RCHAR(str1);
	CHAR* SrcStr = RCHAR(str2);
	int nLen = nSize - Rstrlen(destStr);	
	strncat( destStr, SrcStr, nLen - 1 );
	destStr[nSize-1] = '\0';
#endif
}

void SAFE_SPRINTF_FUNC(char * str1, char * str2, int nSize)
{
	_snprintf( str1, nSize-1 , str2 );
	str1[nSize-1] = '\0';
}

void DebugPrint( const char* pFormat,... )
{
	static char logBuf[8192];
	va_list args;
	va_start( args, pFormat );
	vsprintf( logBuf, pFormat, args );
	va_end( args );
	
#ifdef _DEBUG
	OutputDebugString( logBuf );
#endif
	
}

// by simwoosung
//-- 이건 유저에게 배포해서 에러로그를 기록해서 보내게 해주는용
void ClientFilelog( char *buf, ... )
{
	char tmp_buf[1024], tmp_buf2[1024];
	va_list args;
	FILE *fp;
	
	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );
	
	fp = fopen( "ClientLog\\ClientErrorLog.txt" , "a" );
	
	if( !fp )
		return;
	
	va_start( args, buf );
	vsprintf( tmp_buf, buf, args );
	va_end( args );
	
	sprintf( tmp_buf2, "[%d/%02d/%02d %02d:%02d:%02d] %s \n", currTM->tm_year+1900, 
		currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec, tmp_buf );
	strcpy( tmp_buf, tmp_buf2);
	
	fprintf( fp, tmp_buf );
	
#ifdef _DEBUG 
	OutputDebugString( tmp_buf );
#endif
	
	fclose( fp );
	
}

void ClientFileLogW(WCHAR *buf , ...)
{

#ifdef UNICODE_ROCK
	WCHAR tmp_buf[1024] = {0,}, tmp_buf2[1024] ={0,};
	va_list args;
	FILE *fp;
	
	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );
	
	fp = _wfopen( _RT("ClientLog\\ClientErrorLog.txt") , _RT("a") );
	
	if( !fp )
		return;
	
	va_start( args, buf );
	vswprintf( tmp_buf, buf, args );
	va_end( args );
	
	Rsprintf( tmp_buf2, _RT("[%d/%02d/%02d %02d:%02d:%02d] %s \n" ) , currTM->tm_year+1900, 
		currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec, tmp_buf );
	Rstrcpy( tmp_buf, tmp_buf2);
	
	fwprintf( fp, tmp_buf );
	
	fclose( fp );
#endif 

}



// by simwoosung
// log 화면 출력 and 기록 함수
// va_list를 이용하여 sprintf등과 
// 같은 형식으로 사용 가능하다.
void Filelog( char *buf, ... )
{
	
#ifdef _DEBUG 	
	char tmp_buf[1024], tmp_buf2[1024];
	va_list args;
	FILE *fp;
	
	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );
	
	fp = fopen( "ClientLog.txt" , "a" );
	
	if( !fp )
		return;
	
	va_start( args, buf );
	vsprintf( tmp_buf, buf, args );
	va_end( args );
	
	sprintf( tmp_buf2, "[%d/%02d/%02d %02d:%02d:%02d] %s \n", currTM->tm_year+1900, 
		currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec, tmp_buf );
	strcpy( tmp_buf, tmp_buf2);
	
	fprintf( fp, tmp_buf );	
	fclose( fp );
#endif
	
}

void Filelog2( char *buf, ... )
{
	
#ifdef _DEBUG 		
	char tmp_buf[1024], tmp_buf2[1024];
	va_list args;
	FILE *fp;
	
	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );
	
	fp = fopen( "ClientLog2.txt" , "a" );
	
	if( !fp )
		return;
	
	va_start( args, buf );
	vsprintf( tmp_buf, buf, args );
	va_end( args );
	
	sprintf( tmp_buf2, "[%d/%02d/%02d %02d:%02d:%02d] %s \n", currTM->tm_year+1900, 
		currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec, tmp_buf );
	strcpy( tmp_buf, tmp_buf2);
	
	fprintf( fp, tmp_buf );
	OutputDebugString( tmp_buf );
	fclose( fp );
#endif 
	
}


void Filelog3( char *buf, ... )
{
	
#ifdef _DEBUG 		
	char tmp_buf[1024], tmp_buf2[1024];
	va_list args;
	FILE *fp;
	
	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );
	
	fp = fopen( "ClientLog3.txt" , "a" );
	
	if( !fp )
		return;
	
	va_start( args, buf );
	vsprintf( tmp_buf, buf, args );
	va_end( args );
	
	sprintf( tmp_buf2, "[%d/%02d/%02d %02d:%02d:%02d] %s \n", currTM->tm_year+1900, 
		currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec, tmp_buf );
	strcpy( tmp_buf, tmp_buf2);
	
	fprintf( fp, tmp_buf );
	OutputDebugString( tmp_buf );
	fclose( fp );
#endif 
	
}

//by simwoosung
char * GetStringToken(char *strIn, char ch, char *strOut, int iOutSize )
{
    char *strInTemp = strIn;
	int i = 0;
	
	while( (*strInTemp) != '\0' )
	{
		if( (*strInTemp)	== ch )
		{
			*strOut = '\0';
			strInTemp++;
			return strInTemp;	   
		}
		else
		{
			if( i < iOutSize )
			{
				*strOut = *strInTemp;
				strOut++;
				i++;
			}
		}	
		
		strInTemp++;
	}
	
	return NULL;
}

//by simwoosung
void StringTrim(void *strIn)
{
	RTCHAR *strInTemp = reinterpret_cast<RTCHAR*>(strIn);
	RTCHAR *strInTemp2 = strInTemp;
	RTCHAR *temp = NULL;
	
	int iLen = Rstrlen(RWCHAR(strIn));

	if( iLen <= 0 )
	{
		return;
	}
	
	BOOL bFindFlag = FALSE;
	
	do{		
		
		bFindFlag = FALSE;
		
		if( (*strInTemp) == _RT('\0') )
		{
			break;
		}
		
		if( (*strInTemp) == _RT(' ') )
		{
			bFindFlag = TRUE;	
		}
		
		if( (*strInTemp) == _RT('\t') )
		{
			bFindFlag = TRUE;		
		}
		
		if( bFindFlag )
		{
			temp = strInTemp;
			
			while( (*temp) != _RT('\0') )
			{
				*(temp) = *(temp + 1);
				temp++;
			}
			
			continue;
		}
		
		strInTemp++;
		
	}while( bFindFlag );
	
	strInTemp = strInTemp2;
	strInTemp = &(strInTemp[iLen - 1]);

	do{
		bFindFlag = FALSE;

		if( iLen == 0 )
		{
			break;
		}
		
		if( (*strInTemp) == _RT(' ') )
		{
			bFindFlag = TRUE;	
		}
		
		if( (*strInTemp) == _RT('\t') )
		{
			bFindFlag = TRUE;		
		}

		if( bFindFlag )
		{
			(*strInTemp) = _RT('\0');
		}

		strInTemp--;
		iLen--;	

	}while( bFindFlag );
	
}

//by simwoosung
void DeleteTokenInString(void *strIn, RTCHAR ch)
{
	RTCHAR *strInTemp = reinterpret_cast<RTCHAR*>(strIn);
	RTCHAR *temp = NULL;
	
	while( (*strInTemp) != _RT('\0') )
	{
		if( (*strInTemp)	== ch )
		{
			temp = strInTemp;
			while( (*temp) != _RT('\0') )
			{
				*(temp) = *(temp + 1);
				temp++;
			}
			continue;
		}
		
		strInTemp++;
	}
}

DWORD GetLastErorrCode() 
{ 
  // Retrieve the system error message for the last-error code
    LPVOID lpMsgBuf;
   // LPVOID lpDisplayBuf;
	RTCHAR StrMsg[1024]= {0,};

    DWORD dwLastError = GetLastError(); 
	
	if( dwLastError != 0)
	{
		RFormatMessage
			(   FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_IGNORE_INSERTS
			, NULL
			, dwLastError
			, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT )
			, (RTCHAR*)&lpMsgBuf
			, 0
			, NULL );
		
		// Display the error message and exit the process
		/*
		lpDisplayBuf 
        = (LPVOID)
		LocalAlloc(LMEM_ZEROINIT, ( lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40 ) *sizeof(TCHAR) ); 
		
		  
			StringCchPrintf( (LPTSTR) lpDisplayBuf
			, LocalSize(lpDisplayBuf) / sizeof(TCHAR)
			, _T("%s failed with error %d: %s")
			, lpszFunction
			, dwLastError
			, lpMsgBuf ); 
		*/
		
		//ClientFileLogW( L"%s" , lpMsgBuf );
		Rsprintf(StrMsg , _RT("%s Code %d") , (RTCHAR*)lpMsgBuf , dwLastError );

		RMessageBox(NULL, StrMsg, _RT("Error"), MB_OK); 
		
		LocalFree(lpMsgBuf);
    }
	
	return dwLastError;

  // ExitProcess(dwLastError); 
}
