///---------------------------------------------------------------------------
///-- CStringManager class
///--
///--	Author	:	Lyul	
///--	Date	:	2005.06.28
///--	Lastest	:	
///---------------------------------------------------------------------------
///-- String �� �����ϴ� Class

///-- Desc. : Client���� ���Ǵ� String �� MBCS �������� �����Ѵ�.
///--		: MBCS System Codepage �� �������̹Ƿ� �� Client Test �� Window ���� ������ �ʿ�� �Ѵ�.
///--		: Create �Լ� ȣ��� �ݵ�� Ư���� �����Ͽ� �����ؾ� �Ѵ�.
///--		: StringTable.xls ���� Export �� string_xxx.dat �� �ʿ��ϴ�.
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

	///-- GetString_Ptr MBCS ������ Char* �� Return �Ѵ�.
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
