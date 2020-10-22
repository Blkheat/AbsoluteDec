//..................................................................................................................
//
//	CTextCommandW.cpp																			By wxywxy
//
//..................................................................................................................

#include "RockPCH.h"
#include "CTextCommandW.h"

CTextCommandW::CTextCommandW()		
{
	m_fp = NULL;
	m_pFH = NULL;
	m_pFileSystem = NULL;
		
	memset( m_String , 0 , sizeof(WCHAR)*1024 );
}

CTextCommandW::~CTextCommandW()
{
	m_ArgList.clear();

	Close();
}

int CTextCommandW::Close(void)
{
	if( m_fp != NULL ) 
	{
		fclose( m_fp );	
		m_fp = NULL;
	}

	if( m_pFH != NULL )
	{
		g_VFEtc.CloseFile( m_pFH );
		m_pFH = NULL;
	}

	return true;
}

FILE *CTextCommandW::Open(char *FileName)
{
	Close();

	m_fp = fopen( FileName , "r+t" );

	if( m_fp == NULL)		
	{
		RLGS( "Error CTextCommandW::Load = Open\n" );
	}

	return m_fp;
}


VFileHandle *CTextCommandW::OpenVF(char *FileName)
{
	Close();
	
	m_pFH = g_VFEtc.OpenFile( FileName );
	
	if( m_pFH == NULL)		
	{
		RLGS( "Error CTextCommandW::Load = Open\n" );
	}

	m_pFH->Seek(2 , SEEK_SET);
	
	return m_pFH;
}

VFileHandle *CTextCommandW::OpenCharVF(char *FileName)
{
	m_pFH = g_VFChrRoot.OpenFile( FileName );
	
	if( m_pFH == NULL)		
	{
		RLGS( "Error CTextCommandW::Load = Open\n" );
	}

	return m_pFH;
}

int CTextCommandW::CloseChar(void)
{
	if( m_fp != NULL ) 
	{
		fclose( m_fp );	
		m_fp = NULL;
	}

	if( m_pFH != NULL )
	{
		g_VFChrRoot.CloseFile( m_pFH );
		m_pFH = NULL;
	}

	return true;
}

VFileHandle *CTextCommandW::OpenVF(CVFileSystem * pFileSystem, char *FileName)
{
	if( NULL == pFileSystem )
	{
		return NULL;
	}
	
	m_pFH = pFileSystem->OpenFile( FileName );
	
	if( m_pFH == NULL)		
	{
		RLGS( "Error CTextCommandW::Load = Open\n" );
	}
	
	m_pFH->Seek(2 , SEEK_SET);

	return m_pFH;
}

int   CTextCommandW::Close(CVFileSystem * pFileSystem)
{
	if( NULL == pFileSystem )
	{
		return 0;
	}	
	
	if( m_fp != NULL ) 
	{
		fclose( m_fp );	
		m_fp = NULL;
	}

	if( m_pFH != NULL )
	{
		pFileSystem->CloseFile( m_pFH );
		m_pFH = NULL;
	}

	return 1;
}

WCHAR *CTextCommandW::GetLine(void)
{

	m_ArgList.clear();

	if( m_pFH == NULL ) return NULL;
		
	memset( m_String , 0 , sizeof(WCHAR) * 1024 );
	
	if( m_pFH->Getws( m_String , sizeof(m_String) ) == NULL )
	{
		return NULL;
	}
		
	if( wcslen( m_String ) <= 0 ) return m_String;
	// 문자열 검색후 뺄수있는부분 빼기
	ExceptString();
	// 띄어쓰기 단위로 나누기
	MakeArgList();

	return m_String;
}

WCHAR *CTextCommandW::GetArg(int x)
{
	if( m_ArgList.size() <= x )
		return NULL;

	return ( WCHAR * )m_ArgList[ x ].c_str();


	return NULL;
}

bool CTextCommandW::EqualArgToString(int x,WCHAR *pStr)
{
	if( m_ArgList.size() <= x )
		return false;

	if( wcscmp( m_ArgList[ x ].c_str() , pStr ) == 0 )
		return true;

	return false;
}

void CTextCommandW::ExceptString(void)
{
	int  Count			 = 0;
	int  DestCount		 = 0;
	int  Len			 = wcslen( m_String );
	WCHAR TempStr[ 1024 ] = L"";

	memset( TempStr , 0 , sizeof(WCHAR) * 1024 );
	memcpy( TempStr  , m_String , sizeof(WCHAR) *1024 );
	memset( m_String , 0 , sizeof(WCHAR) * 1024 );
	
	while( Count <= Len )
	{
		switch( TempStr[ Count ] )
		{
			case ' ':
			case 9  :
				
				if( DestCount != 0 )
				{
					if( m_String[ DestCount - 1 ] != ' ' )
						m_String[ DestCount++   ] = ' ';
				}
				
				Count++;

				break;
		
			case '/':
				
				if( TempStr[ Count + 1 ] == '/' )
				{
					if( DestCount != 0 )
						if( m_String[ DestCount - 1 ] == ' ' ) 
							m_String[ DestCount - 1 ] = 10;
			
					return;
				}
				else
					m_String[ DestCount++ ] = TempStr[ Count ];
				
				Count++;

				break;

			case 10:
				/*
				if( DestCount != 0 )
					if( m_String[ DestCount - 1 ] == ' ' ) 
						m_String[ DestCount - 1 ] = 0;
				*/
				
				m_String[ DestCount++ ] = TempStr[ Count++ ];

				return;

			default:
					m_String[ DestCount++ ] = TempStr[ Count++ ];
				
				break;
		}
	}
}


int CTextCommandW::MakeArgList(void)
{
	int Len = wcslen( m_String );

	if( 0 >= Len )
		return false;
		
	WCHAR TempStr[ 1024 ] = L"";
	int  Count = 0;

	WCHAR *pStr = m_String;

	for( int i = 0 ; i < Len ; i++ )
	{
		if( pStr[ Count ] == ' ' )
		{
			memset( TempStr , 0 , sizeof(WCHAR) * 1024 );
		
			memcpy( TempStr , pStr , sizeof(WCHAR)*Count );
		
			wstring str = TempStr;
			
			m_ArgList.push_back( str );
			
			pStr += ( Count + 1 );

			Count = 0;
		}
		else
			Count++;
	}

	memset( TempStr , 0 , sizeof(WCHAR) * 1024 );
	memcpy( TempStr , pStr , sizeof(WCHAR) *Count );



	wstring str = TempStr;
			
	m_ArgList.push_back( str );
		

	return true;
}

int	CTextCommandW::GetArgValue(int x,float &Value)
{
	if( m_ArgList.size() <= x ) return false;

	Value = ( float ) _wtoi( ( WCHAR * )m_ArgList[ x ].c_str() );

	return TRUE;
}

int	CTextCommandW::GetArgValue(int x,DWORD &Value)
{
	if( m_ArgList.size() <= x ) return false;

	Value = ( DWORD ) _wtoi( ( WCHAR * )m_ArgList[ x ].c_str() );

	return TRUE;
}

int	CTextCommandW::GetArgValue(int x,int   &Value)
{
	if( m_ArgList.size() <= x ) return false;

	Value = ( int ) _wtoi( ( WCHAR * )m_ArgList[ x ].c_str() );

	return TRUE;
}

int	CTextCommandW::GetArgValue(int x,BYTE  &Value)
{
	if( m_ArgList.size() <= x ) return false;

	Value = ( BYTE ) _wtoi( ( WCHAR * )m_ArgList[ x ].c_str() );

	return TRUE;
}

int CTextCommandW::GetArgValue(int x,WORD  &Value)
{
	if( m_ArgList.size() <= x ) return false;

	Value = ( WORD ) _wtoi( ( WCHAR * )m_ArgList[ x ].c_str() );

	return TRUE;
}