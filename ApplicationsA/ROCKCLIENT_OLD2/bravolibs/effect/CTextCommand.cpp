//..................................................................................................................
//
//	CTextCommand.cpp																			By wxywxy
//
//..................................................................................................................

#include "RockPCH.h"
#include "CTextCommand.h"

CTextCommand::CTextCommand()		
{
	m_fp = NULL;
	m_pFH = NULL;
	m_pFileSystem = NULL;
		
	memset( m_String , 0 , 1024 );
}

CTextCommand::~CTextCommand()
{
	m_ArgList.clear();

	Close();
}

int CTextCommand::Close(void)
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

FILE *CTextCommand::Open(char *FileName)
{
	Close();

	m_fp = fopen( FileName , "r+t" );

	if( m_fp == NULL)		
	{
		RLGS( "Error CTextCommand::Load = Open\n" );
	}

	return m_fp;
}


VFileHandle *CTextCommand::OpenVF(char *FileName)
{
	Close();
	
	m_pFH = g_VFEtc.OpenFile( FileName );
	
	if( m_pFH == NULL)		
	{
		RLGS( "Error CTextCommand::Load = Open\n" );
	}

	return m_pFH;
}

VFileHandle *CTextCommand::OpenCharVF(char *FileName)
{
	m_pFH = g_VFChrRoot.OpenFile( FileName );
	
	if( m_pFH == NULL)		
	{
		RLGS( "Error CTextCommand::Load = Open\n" );
	}

	return m_pFH;
}

int CTextCommand::CloseChar(void)
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

VFileHandle *CTextCommand::OpenVF(CVFileSystem * pFileSystem, char *FileName)
{
	if( NULL == pFileSystem )
	{
		return NULL;
	}
	
	m_pFH = pFileSystem->OpenFile( FileName );
	
	if( m_pFH == NULL)		
	{
		RLGS( "Error CTextCommand::Load = Open\n" );
	}

	return m_pFH;
}

int   CTextCommand::Close(CVFileSystem * pFileSystem)
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

char *CTextCommand::GetLine(void)
{
	m_ArgList.clear();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if( m_fp == NULL ) return NULL;

	memset( m_String , 0 , 1024 );
	
	if( fgets( m_String , 1024 , m_fp ) == NULL )
	{
		return NULL;
	}
	
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	if( l_nDevSkip )
	{
		int i;
		if( m_fp == NULL ) return NULL;
		
		memset( m_String , 0 , 1024 );
		
		if( fgets( m_String , 1024 , m_fp ) == NULL )
		{
			return NULL;
		}
		
		
		i = strlen( m_String );
		if( i > 0 )
		{
			if( m_String[i-1] == '\n' )
			{					
				m_String[i-1] = 0;				
			}
		}
	}
	else
	{
		if( m_pFH == NULL ) return NULL;
		
		memset( m_String , 0 , 1024 );
		
		if( m_pFH->Gets( m_String , 1024 ) == NULL )
		{
			return NULL;
		}
	}
	
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	if( strlen( m_String ) <= 0 ) return m_String;
	// 문자열 검색후 뺄수있는부분 빼기
	ExceptString();
	// 띄어쓰기 단위로 나누기
	MakeArgList();

	return m_String;
}

char *CTextCommand::GetArg(int x)
{
	if( m_ArgList.size() <= x )
		return NULL;

	return ( char * )m_ArgList[ x ].c_str();


	return NULL;
}

bool CTextCommand::EqualArgToString(int x,char *pStr)
{
	if( m_ArgList.size() <= x )
		return false;

	if( strcmp( m_ArgList[ x ].c_str() , pStr ) == 0 )
		return true;

	return false;
}

void CTextCommand::ExceptString(void)
{
	int  Count			 = 0;
	int  DestCount		 = 0;
	int  Len			 = strlen( m_String );
	char TempStr[ 1024 ] = "";

	memset( TempStr  , 0		, 1024 );
	memcpy( TempStr  , m_String , 1024 );
	memset( m_String , 0	    , 1024 );
	
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


int CTextCommand::MakeArgList(void)
{
	int Len = strlen( m_String );

	if( 0 >= Len )
		return false;
		
	char TempStr[ 1024 ] = "";
	int  Count = 0;

	char *pStr = m_String;

	for( int i = 0 ; i < Len ; i++ )
	{
		if( pStr[ Count ] == ' ' )
		{
			memset( TempStr , 0 , 1024 );
		
			memcpy( TempStr , pStr , Count );
		
			string str = TempStr;
			
			m_ArgList.push_back( str );
			
			pStr += ( Count + 1 );

			Count = 0;
		}
		else
			Count++;
	}

	memset( TempStr , 0 , 1024 );
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	memcpy( TempStr , pStr , Count - 1 );
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else                   /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	if( l_nDevSkip )
	{
		memcpy( TempStr , pStr , Count );
	}
	else
	{
		memcpy( TempStr , pStr , Count );
	}	
#endif	

	string str = TempStr;
			
	m_ArgList.push_back( str );
		

	return true;
}

int	CTextCommand::GetArgValue(int x,float &Value)
{
	if( m_ArgList.size() <= x ) return false;

	Value = ( float ) atof( ( char * )m_ArgList[ x ].c_str() );

	return TRUE;
}

int	CTextCommand::GetArgValue(int x,DWORD &Value)
{
	if( m_ArgList.size() <= x ) return false;

	Value = ( DWORD ) atoi( ( char * )m_ArgList[ x ].c_str() );

	return TRUE;
}

int	CTextCommand::GetArgValue(int x,int   &Value)
{
	if( m_ArgList.size() <= x ) return false;

	Value = ( int ) atoi( ( char * )m_ArgList[ x ].c_str() );

	return TRUE;
}

int	CTextCommand::GetArgValue(int x,BYTE  &Value)
{
	if( m_ArgList.size() <= x ) return false;

	Value = ( BYTE ) atoi( ( char * )m_ArgList[ x ].c_str() );

	return TRUE;
}

int CTextCommand::GetArgValue(int x,WORD  &Value)
{
	if( m_ArgList.size() <= x ) return false;

	Value = ( WORD ) atoi( ( char * )m_ArgList[ x ].c_str() );

	return TRUE;
}