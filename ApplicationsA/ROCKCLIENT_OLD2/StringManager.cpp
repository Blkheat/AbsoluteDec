// StringManager.cpp: implementation of the CStringManager class.
//
//////////////////////////////////////////////////////////////////////

#include "StringManager.h"
#include "..\\VFileSystem.h" 



///-- Instance 
CStringManager g_StringMgr;

#define __Macro_CHECK_NOT_CREATE assert( !mb_created );if( mb_created )return false;
#define __Macro_CHECK_CREATE assert( mb_created );if( !mb_created )return false;

///---------------------------------------------------------------------------
///-- Construction/Destruction
///---------------------------------------------------------------------------
CStringManager::CStringManager()
{
	mpp8_string = NULL;
	_clear();

	memset( m_strTempBuf , 0 , sizeof(char) * 256 );
	
}

CStringManager::~CStringManager()
{
	signed long i;

	if( mpp8_string )
	{		
		for( i = 0 ; i < ml_count ; ++i )
		{
			SAFE_DELETE_ARRAY(mpp8_string[i]);
		}
		
		SAFE_DELETE_ARRAY(mpp8_string);
	}

}

///---------------------------------------------------------------------------
///-- _clear
///---------------------------------------------------------------------------
void CStringManager::_clear()
{
	mb_created = false;
	me_language = eLang_Korea;
}
///---------------------------------------------------------------------------
///-- Create
///---------------------------------------------------------------------------
bool CStringManager::Create( E_LANGUAGE ae_language )
{
	__Macro_CHECK_NOT_CREATE
	
	bool lb_ret_value;
	me_language = ae_language;
	
	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	if( l_nDevSkip )
	{
		lb_ret_value = _Load_String_File();
	}
	else
	{
		lb_ret_value = _Load_String_VF();
	}
	
	if( lb_ret_value )
	{
		mb_created = true;
	}

	return lb_ret_value;
}
///---------------------------------------------------------------------------
///-- GetString_Ptr
///---------------------------------------------------------------------------
const unsigned char* const CStringManager::GetString_Ptr( const unsigned long al_str_index ) const 
{
	__Macro_CHECK_CREATE
#ifdef _DEBUG
//	assert( al_str_index < ml_count );
#endif
	
	if( al_str_index >= ml_count )
	{
		//유저들은 튕기면 안됀다 .. 게임은 죽으믄 안돼 OTL....
		return ( const unsigned char* const )m_strTempBuf;	

#ifdef _DEBUG
		::MessageBox( NULL,"String Data Error", 0, 0 );
#endif
	}
	
	return ( const unsigned char* const )mpp8_string[al_str_index];	
}
///---------------------------------------------------------------------------
///-- _Load_String
///---------------------------------------------------------------------------
bool CStringManager::_Load_String_File()
{
/*
	signed long i;
	signed long ll_index;
	signed long ll_len;
	char lac_filename[100]={0,};

	FILE* fp;	
	///---------------------------------------------------------------------------
	///-- Open String Data
	switch( me_language ) 
	{
	default:
	case eLang_Korea: 
		sprintf( lac_filename, "local_test/str_korea.dat" );		
		break;
	case eLang_Japan:
		sprintf( lac_filename, "local_test/str_japan.dat" );		
		break;
	case eLang_Indonesia:
		sprintf( lac_filename, "local_test/str_indonesia.dat" );		
		break;
	case eLang_China:
		sprintf( lac_filename, "local_test/str_china.dat" );		
		break;
	case eLang_Taiwan:
		sprintf( lac_filename, "local_test/str_taiwan.dat" );		
		break;
	}
	
	fp = _tfopen( lac_filename, _T("rb") ); ///pFH =  g_VFInterface.OpenFile( lac_filename );
	if( fp == NULL )
	{
		char lac_error[300];
		sprintf(lac_error," Cannot Open File : %s ", lac_filename );
		
		MessageBox( NULL, lac_error, _T("!알림"), MB_OK );
		return false;
	}	

	///---------------------------------------------------------------------------
	///-- Read Data	
	///-- Read string count
	fread( ( void * )&ml_count, sizeof( ml_count ), 1, fp);	
	mpp8_string = SAFE_NEW_ARRAY( BYTE* , ml_count );

	///-- Read Strings
	for( i = 0 ; i < ml_count; ++i )
	{
		///-- index check
		fread( ( void * )&ll_index, sizeof( ll_index ), 1 ,fp );		
		if( i != ll_index )
		{
			MessageBox( NULL, _T("! Error : Invalid string Index"), _T("Error"), MB_OK );
			return false;
		}
		///-- Length
		fread( ( void * )&ll_len, sizeof( ll_index ), 1 ,fp );		
		mpp8_string[i] = SAFE_NEW_ARRAY( BYTE , ll_len+1 );
		///-- String
		fread( ( void * )mpp8_string[i], ll_len, 1 ,fp );		
	}
	fclose( fp );
*/

	return true;
}
///---------------------------------------------------------------------------
///-- _Load_String_VF
///---------------------------------------------------------------------------
bool CStringManager::_Load_String_VF()
{
	VFileHandle* pFH;	
	signed long i;
	signed long ll_index;
	signed long ll_len;
	char lac_filename[100]={0,};
	
	///---------------------------------------------------------------------------
	///-- Open String Data
	switch( me_language ) 
	{
		default:
		case eLang_Korea: 
			sprintf( lac_filename, "str_korea.dat" );		
			break;
		case eLang_Japan:
			sprintf( lac_filename, "str_japan.dat" );		
			break;
		case eLang_Indonesia:
			sprintf( lac_filename, "str_indonesia.dat" );
			break; 
		case eLang_English:
			sprintf( lac_filename, "str_English.dat" );
			break;
		case eLang_China:
			sprintf( lac_filename, "str_Chinese.dat" );		
			break;
		case eLang_Taiwan:
			sprintf( lac_filename, "str_taiwan.dat" );	
			break;
		case eLang_Singapore:
			sprintf( lac_filename, "str_English.dat");
			break; 
		case eLang_HongKong:
			sprintf( lac_filename, "str_Hongkong.dat");
			break; 	

	}

	pFH =  g_VFInterface.OpenFile( lac_filename );
	if( pFH == NULL ) 
	{
		TCHAR lac_error[300];
		sprintf(lac_error, _T("Cannot Open File : %s "), lac_filename );
		
		MessageBox( NULL, lac_error, _T("!알림"), MB_OK );
		return false;
	}
	
	///---------------------------------------------------------------------------
	///-- Read Data
	
	///-- Read string count
	pFH->Read( ( void * )&ml_count, sizeof( ml_count ));
	mpp8_string = SAFE_NEW_ARRAY( BYTE* , ml_count );	
	
	///-- Read Strings
	for( i = 0 ; i < ml_count; ++i )
	{
		///-- index check
		pFH->Read( ( void * )&ll_index, sizeof( ll_index ));
		if( i != ll_index )
		{
			MessageBox( NULL, _T("! Error : Invalid string Index"), _T("Error"), MB_OK );
			return false;
		}
		///-- Length
		pFH->Read( ( void * )&ll_len, sizeof( ll_index ));
		
		///-- String
#ifdef UNICODE_ROCK
		mpp8_string[i] = SAFE_NEW_ARRAY( BYTE , ll_len+1 );	
		ZeroMemory(mpp8_string[i] , sizeof(BYTE) *ll_len+1 );
		pFH->Read( ( void * )mpp8_string[i], ll_len+1 );
#else
		mpp8_string[i] = SAFE_NEW_ARRAY( BYTE , ll_len+1 );	

		pFH->Read( ( void * )mpp8_string[i], ll_len );
#endif	
	}
	
	g_VFInterface.CloseFile( pFH );
	
	return true;
}