///---------------------------------------------------------------------------
///-- CStringManager class
///--
///--	Author	:	Lyul	
///--	Date	:	2005.06.28
///--	Lastest	:	
///---------------------------------------------------------------------------
///-- String 을 관리하는 Class

///-- Desc. : Client에서 사용되는 String 을 MBCS 형식으로 관리한다.
///--		: MBCS System Codepage 에 종속적이므로 각 Client Test 는 Window 설정 변경을 필요로 한다.
///--		: Create 함수 호출시 반드시 특정언어를 지정하여 생성해야 한다.
///--		: StringTable.xls 에서 Export 된 string_xxx.dat 가 필요하다.
///---------------------------------------------------------------------------


#if !defined(__STRING_MANAGER_H_INCLUDED__)
#define __STRING_MANAGER_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RockPCH.h"
#include "DefineSTR_ID.h"
#include "RockClient.h"

#ifdef UNICODE_ROCK
#define G_STRING_CHAR(a) (WCHAR *)(g_StringMgr.GetString_Ptr(a))

#else

#define G_STRING_CHAR(a) (char*)g_StringMgr.GetString_Ptr(a)
#define G_STRING_UCHAR(a) (unsigned char*)g_StringMgr.GetString_Ptr(a)
#endif
///---------------------------------------------------------------------------
///-- E_LANGUAGE
///---------------------------------------------------------------------------
enum E_LANGUAGE
{
	eLang_Korea = 0,
	eLang_Japan,
	eLang_Indonesia,
	eLang_English,
	eLang_China,
	eLang_Taiwan,
	eLang_Singapore,
	eLang_HongKong,
};
///---------------------------------------------------------------------------
///-- CStringManager  
///---------------------------------------------------------------------------
class CStringManager  
{
public:
	CStringManager();
	virtual ~CStringManager();

	///-- Create
	bool Create( E_LANGUAGE ae_language );

	///-- GetString_Ptr MBCS 형식의 Char* 를 Return 한다.
	const unsigned char* const GetString_Ptr( const unsigned long al_str_index ) const ; 

private:

	///-- _Clear
	void _clear();

	///-- _Load_String_File
	bool _Load_String_File();
	
	///-- _Load_String_VF
	bool _Load_String_VF();
	

	
	bool mb_created;
	E_LANGUAGE me_language;
	signed long ml_count;			///-- String Count
	BYTE **mpp8_string;				///-- Strnig Store

	char  m_strTempBuf[256];	
};

extern CStringManager g_StringMgr;

#endif 
