#include "RockPCH.h"
/*
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "RockClient.h"*/
#include "CSlangFilter.h"
#include "RockClient.h"
#include "bravolibs\\effect\\CTextCommand.h"
#include "bravolibs\\effect\\CTextCommandW.h"

CSlangFilter::CSlangFilter()
{
    memset( m_TempStr , 0 , MAX_SLANG_STRING );
}

CSlangFilter::~CSlangFilter()
{
    DeleteAllMemory();
}

void CSlangFilter::DeleteAllMemory(void)
{
    DeleteSlangList();
    DeleteNameList();
}

void CSlangFilter::DeleteSlangList(void)
{
    std::vector <LPSLANG_DATA> ::iterator i = m_pSlangList.begin();

    for( ; i != m_pSlangList.end() ; ++i )
    {
        LPSLANG_DATA pData = (*i);
		SAFE_DELETE( pData );
		(*i) = NULL;
    }

    m_pSlangList.clear();
}

void CSlangFilter::DeleteNameList(void)
{
    std::vector <LPNAME_DATA> ::iterator i = m_pNameList.begin();

    for( ; i != m_pNameList.end() ; ++i )
    {
        LPNAME_DATA pData = (*i);
		SAFE_DELETE( pData );
		(*i) = NULL;
    }

    m_pNameList.clear();
}

void CSlangFilter::DeleteAdminNameList(void)
{
	std::vector <LPNAME_DATA> ::iterator i = m_pAdminNameList.begin();

	for( ; i != m_pAdminNameList.end(); ++i)
	{
		SAFE_DELETE((*i));
	}

	m_pAdminNameList.clear();

}

///---------------------------------------------------------------------------
///-- LoadList
///---------------------------------------------------------------------------
int CSlangFilter::LoadList(char *FileName)
{
    DeleteSlangList();

    RTCHAR CompareStr[ 1024 ] = _RT("");
    RTCHAR ReplaceStr[ 1024 ] = _RT("");

#ifdef UNICODE_ROCK
	CTextCommandW TextCmd;
#else
	CTextCommand TextCmd;
#endif
    


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	char Path[ 1024 ] = "";

	wsprintf( Path , "Etc\\%s" , FileName );

    if( TextCmd.Open( Path ) == NULL )
    {
        return false;
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
	
	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	if( l_nDevSkip )
	{
		char Path[ 1024 ] = "";		
		wsprintf( Path , "%s\\local_test\\%s" ,g_RockClient.GetRootDir(), FileName );		
		if( TextCmd.Open( Path ) == NULL )
		{
			::MessageBox(NULL,"Cannot Found slang.dat",0,0);
			return false;
		}		
	}
	else
	{
		if( TextCmd.OpenVF( FileName ) == NULL )
		{
			return false;
		}
	}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

    while( TextCmd.GetLine() != NULL )
    {
        memset( CompareStr , 0 , sizeof(RTCHAR) * 1024 );
        memset( ReplaceStr , 0 , sizeof(RTCHAR) * 1024 );

		if( TextCmd.ISArgEmpty() )
        {
            continue;
        }
#ifdef TEXFILTERING_TEST
		
        if( TextCmd.GetArgSize() == 1 )
        {
            continue;
        }
		
#else
		if( TextCmd.GetArgSize() != 1 )
        {
            continue;
        }
#endif //TEXFILTERING_TEST
        Rsprintf( CompareStr , _RT("%s") , TextCmd.GetArg( 0 ) );
		Rsprintf( ReplaceStr , _RT("%s") , CompareStr  );
		int ReplLen = Rstrlen(ReplaceStr);

		Rstrnset( ReplaceStr  , _RT('*') , ReplLen );
	
        LPSLANG_DATA pData = NULL;
        
		pData = SAFE_NEW( SLANG_DATA ); 

        pData->StrCompare = SAFE_NEW_ARRAY( RTCHAR , Rstrlen( CompareStr)+ 1);
		memset( pData->StrCompare , 0 , sizeof(RTCHAR) * Rstrlen( CompareStr) + 1);

        Rsprintf( pData->StrCompare , _RT("%s") , CompareStr );

		pData->StrReplace = SAFE_NEW_ARRAY( RTCHAR , Rstrlen( ReplaceStr) + 1 );	
		memset( pData->StrReplace , 0 , sizeof(RTCHAR) * Rstrlen( ReplaceStr ) + 1 );

        Rsprintf( RWCHAR(pData->StrReplace) , _RT("%s") , RWCHAR(ReplaceStr) );

        m_pSlangList.push_back( pData );
    }

    TextCmd.Close();
    
    return true;
}

int CSlangFilter::LoadNameList(char *FileName)
{
    DeleteNameList();

    RTCHAR  CompareStr[ 1024 ]= {0,};
	
	
#ifdef UNICODE_ROCK
	CTextCommandW TextCmd;
#else
	CTextCommand TextCmd;
#endif
    

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	char Path[ 1024 ] = _RT("");

	wsprintf( Path , "Etc\\%s" , FileName );

    if( TextCmd.Open( Path ) == NULL )
    {
        return false;
    }
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	if( l_nDevSkip )
	{
		char Path[ 1024 ] = "";	
		wsprintf( Path , "local_test\\%s" , FileName );
		if( TextCmd.Open( Path ) == NULL )
		{
			::MessageBox(NULL,"Cannot Found namelist.dat",0,0);
			return false;
		}	
	}
	else
	{
		if( TextCmd.OpenVF( FileName ) == NULL )
		{
			return false;
		}		
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
	
    while( TextCmd.GetLine() != NULL )
    {
        memset( CompareStr , 0 , sizeof(RTCHAR) * 1024 );

		if( TextCmd.ISArgEmpty() )
        {
            continue;
        }
#ifdef TEXFILTERING_TEST
        if( TextCmd.GetArgSize() == 1 )
        {
            continue;
        }
#else
		if( TextCmd.GetArgSize() != 1 )
        {
            continue;
        }
#endif //TEXFILTERING_TEST
        Rsprintf( CompareStr , _RT("%s") , TextCmd.GetArg( 0 ) );

        LPNAME_DATA pData = NULL;
		
		pData = SAFE_NEW( NAME_DATA );

        pData->StrCompare = SAFE_NEW_ARRAY( RTCHAR , Rstrlen(CompareStr)+1 );
		ZeroMemory(pData->StrCompare ,  sizeof(RTCHAR) * Rstrlen(CompareStr)+1 );

        Rsprintf( pData->StrCompare , _RT("%s") , CompareStr );

		int Len = Rstrlen( pData->StrCompare );
#ifdef UNICODE_ROCK
		
		for( int i = 0 ; i < Len ; ++i )
		{
			//----------------------------------------------------
			// 1 byte code
			//----------------------------------------------------
			if ( ( ( pData->StrCompare[ i ]>= 0x0041 ) && ( pData->StrCompare[ i ] <= 0x005A ) ) || 
					( ( pData->StrCompare[ i ]>= 0x0061 ) && ( pData->StrCompare[ i ]<= 0x007A) ) 
				) 
			{
				// english character 
				if( pData->StrCompare[ i ] >= L'A' && pData->StrCompare[ i ] <= L'Z' )
				{
					pData->StrCompare[ i ] -= (L'A' - L'a');
				}
			}
		}
			
#else 
		
		for( int i = 0 ; i < Len ; ++i )
		{
			//----------------------------------------------------
			// 1 byte code
			//----------------------------------------------------
			if (!(((unsigned char)pData->StrCompare[ i ]) & 0x80))
			{
				if ( ( ( pData->StrCompare[ i ]>= 0x41 ) && ( pData->StrCompare[ i ] <= 0x5A ) ) || 
					( ( pData->StrCompare[ i ]>= 0x61 ) && ( pData->StrCompare[ i ]<= 0x7A) ) 
					) 
				{
					// english character 
					if( pData->StrCompare[ i ] >= 'A' && pData->StrCompare[ i ] <= 'Z' )
					{
						pData->StrCompare[ i ] -= ('A' - 'a');
					}
				}
				else if ( ( pData->StrCompare[ i ] >= '0' ) && ( pData->StrCompare[ i ] <= '9' ) )
				{
					// numeric character
				}
				else if ( pData->StrCompare[ i ] == ' ' )
				{
					// 1 byte white space
				}
				else
				{
					// 1 byte special character
				}
			}
			
			//----------------------------------------------------
			// 2 byte code
			//----------------------------------------------------
			else
			{
				// broken lead byte
				if (i + 1 == Len ) return false;
				
				if 
					(
					(
					( (unsigned char)pData->StrCompare[i] >= 0xA4   ) && ( (unsigned char)pData->StrCompare[i] <= 0xA4 ) &&
					( (unsigned char)pData->StrCompare[i+1] >= 0xA1 ) && ( (unsigned char)pData->StrCompare[i+1] <= 0xFE )
					)
					||	
					( 
					( (unsigned char)pData->StrCompare[i] >= 0xB0 ) && ( (unsigned char)pData->StrCompare[i] <= 0xC8 ) &&
					( (unsigned char)pData->StrCompare[i+1] >= 0xA1 ) && ( (unsigned char)pData->StrCompare[i+1] <= 0xFE )
					)
					) 
				{ 
					i++; 
					// valid Korean character 
				} 
				else if ( ( (unsigned char)pData->StrCompare[i]>= 0xCA ) && ( (unsigned char)pData->StrCompare[i] <= 0xFD ) &&
					( (unsigned char)pData->StrCompare[i+1] >= 0xA1 ) && ( (unsigned char)pData->StrCompare[i+1] <= 0xFE)
					) 
				{ 
					i++; 
					// old - Chinese character
					
				} 
				else if ( ( (unsigned char)pData->StrCompare[i]>= 0x81 ) && ( (unsigned char)pData->StrCompare[i] <= 0x9f ) ||
					( (unsigned char)pData->StrCompare[i] >= 0xe0 ) && ( (unsigned char)pData->StrCompare[i] <= 0xfc )
					) 
				{ 
					i++; 
					// Japanese Character
				} 
				else
				{
					i++; 
					// 2 byte special character
				}
			}
			//.....................................................................................................
		}


#endif

        m_pNameList.push_back( pData );
    }

    TextCmd.Close();
    
    return true;
}

int CSlangFilter::LoadAdminNameList(char *FileName)
{
	DeleteAdminNameList();

    RTCHAR CompareStr[ 1024 ] = _RT("");

#ifdef UNICODE_ROCK
    CTextCommandW TextCmd;
#else
	CTextCommand TextCmd;
#endif


	if( TextCmd.OpenVF( FileName ) == NULL )
    {
        return false;
    }
	
    while( TextCmd.GetLine() != NULL )
    {
        memset( CompareStr , 0 , 1024 );

		if( TextCmd.ISArgEmpty() )
        {
            continue;
        }
#ifdef TEXFILTERING_TEST
        if( TextCmd.GetArgSize() == 1 )
        {
            continue;
        }
#else
		if( TextCmd.GetArgSize() != 1 )
        {
            continue;
        }
#endif //TEXFILTERING_TEST
        Rsprintf( RWCHAR(CompareStr) , _RT("%s") , RWCHAR(TextCmd.GetArg( 0 )) );

        LPNAME_DATA pData = NULL;

        pData = SAFE_NEW( NAME_DATA );

		pData->StrCompare = SAFE_NEW_ARRAY( RTCHAR , Rstrlen( CompareStr ) + 1)  ;

        memset( pData->StrCompare , 0 , sizeof(RTCHAR )  * Rstrlen( CompareStr ) + 1 );

        Rsprintf( RWCHAR(pData->StrCompare) , _RT("%s") , RWCHAR(CompareStr) );

        m_pAdminNameList.push_back( pData );
    
	}

    TextCmd.Close();

	return true;
}

bool CSlangFilter::IsCheckSymbol(void *pSrcStr , void *pDstStr)
{
	RTCHAR *SrcStr  = RWCHAR(pSrcStr);
	RTCHAR *DstStr  = RWCHAR(pDstStr);
	int SrcLen = Rstrlen(RWCHAR(SrcStr));
	int DstLen = Rstrlen(RWCHAR(DstStr));
	
	int Dinx = 0 ; 
	int Sinx = 0 ; 

	for( Sinx  =0 ; Sinx< SrcLen ; ++Sinx )
	{
		for(Dinx = 0 ; Dinx < DstLen ; ++Dinx)
		{
			if( SrcStr[Sinx] == DstStr[Dinx])
			{
				return true;				
			}
			
		}
		
	}
	return false; 

}

bool CSlangFilter::IsFilterGmName(void *Str)
{
#ifdef UNICODE_ROCK
	return IsFilterGmNameW( (WCHAR*)Str);
#else
	return IsFilterGmNameA( (char*)Str);
#endif

}

bool CSlangFilter::IsFilterGmNameA(char *Str)
{
	
	//..........................................................................
    // 필터링
    //..........................................................................
	int   i = 0;
    int len = strlen( Str );
	char GmName[128] = _T("GM");

	if(len > 1 )
	{
		len = 1;
	}

    while( i < len )
    {
        bool found = false;
        int left = 0;
        int Result;

		//.....................................................................................................
		// 수정 루틴
		//.....................................................................................................
		//std::vector <LPNAME_DATA>::iterator pi = m_pNameList.begin();
		
	//	for( ; pi != m_pNameList.end() ; ++pi )
     //   {
		
			if( i >= len )
				return false;

            Result = strncmp( GmName , &Str[ i ] , strlen( GmName ) );
            
			if( Result == 0 )
            {
                return true;
            }
     //   }
		
		//----------------------------------------------------
		// 1 byte code
		//----------------------------------------------------
		if (!(((unsigned char)m_TempStr[ i ]) & 0x80))
		{
			if ( ( ( m_TempStr[ i ]>= 0x41 ) && ( m_TempStr[ i ] <= 0x5A ) ) || 
				 ( ( m_TempStr[ i ]>= 0x61 ) && ( m_TempStr[ i ] <= 0x7A) ) 
				) 
			{
				// english character 
				i++;
			}
			else if ( ( m_TempStr[ i ] >= '0' ) && ( m_TempStr[ i ] <= '9' ) )
			{
				// numeric character
				i++;
			}
			else if ( m_TempStr[ i ] == ' ' )
			{
				// 1 byte white space
				i++;
			}
			else
			{
				// 1 byte special character
				i++;
			}
		}

		//----------------------------------------------------
		// 2 byte code
		//----------------------------------------------------
		else
		{
			// broken lead byte
			if (i + 1 == len) return false;

			if 
			(
			   (
			     ( (unsigned char)m_TempStr[i] >= 0xA4   ) && ( (unsigned char)m_TempStr[i] <= 0xA4 ) &&
			  	 ( (unsigned char)m_TempStr[i+1] >= 0xA1 ) && ( (unsigned char)m_TempStr[i+1] <= 0xFE )
			   )
			   ||	
			   ( 
			     ( (unsigned char)m_TempStr[i] >= 0xB0 ) && ( (unsigned char)m_TempStr[i] <= 0xC8 ) &&
			  	 ( (unsigned char)m_TempStr[i+1] >= 0xA1 ) && ( (unsigned char)m_TempStr[i+1] <= 0xFE )
			   )
			) 
			{ 
				i += 2; 
				// valid Korean character 
			} 
			else if ( ( (unsigned char)m_TempStr[i]>= 0xCA ) && ( (unsigned char)m_TempStr[i] <= 0xFD ) &&
					  ( (unsigned char)m_TempStr[i+1] >= 0xA1 ) && ( (unsigned char)m_TempStr[i+1] <= 0xFE)
					) 
			{ 
				i += 2; 
				// old - Chinese character

			} 
			else if ( ( (unsigned char)m_TempStr[i]>= 0x81 ) && ( (unsigned char)m_TempStr[i] <= 0x9f ) ||
					  ( (unsigned char)m_TempStr[i] >= 0xe0 ) && ( (unsigned char)m_TempStr[i] <= 0xfc )
					) 
			{ 
				i += 2; 
				// Japanese Character
			} 
			else
			{
				i += 2;
				// 2 byte special character
			}
		}
		//.....................................................................................................
    }

/*

	if(_tcscmp( Str, _T("") ) != 0 )
	{
		std::vector <LPNAME_DATA>::iterator pi = m_pAdminNameList.begin();
		
		for( ; pi != m_pAdminNameList.end() ; ++pi )
		{
			//해당 목록에 존재 하는가 .. 
			if ( _tcscmp( (*pi)->StrCompare , Str) == 0 ) 
			{
				
				return true; 
			}
		}
	}
	*/

	return false;
}


bool CSlangFilter::IsFilterGmNameW(WCHAR *Str)
{
	
	//..........................................................................
    // 필터링
    //..........................................................................
    int len = wcslen( Str );
	WCHAR GmName[5] = L"GM";
	if(len < 1 )
	{
		return false; 
	}
	int Result = 0 ; 
	Result = wcsncmp( GmName , Str , sizeof(WCHAR)*2);
	
	if( Result == 0 )
	{
		return true;
	}
    
	return false;
}


int CSlangFilter::Filter(void *Str)
{
	
#ifdef UNICODE_ROCK
	return FilterW(RWCHAR(Str));
#else
	return FilterA(RWCHAR(Str));
#endif

}

int CSlangFilter::FilterA(char *Str)
{
	
	char SaveStr[ MAX_SLANG_STRING ] = "";	

	m_ISSlangFound = FALSE;

    m_SpaceCount = 0;

    int len = strlen( Str );

    if( len <= 0 )
        return false;

    memset( m_TempStr , 0 , MAX_SLANG_STRING );
    memset( SaveStr   , 0 , MAX_SLANG_STRING );
	
    //..........................................................................
    // 공백문자 없애기
    //..........................................................................
    int Count = 0;
    int i;

    for( i = 0 ; i < len ; ++i )
    {
		//----------------------------------------------------
		// 1 byte code
		//----------------------------------------------------
		if (!(((unsigned char)Str[ i ]) & 0x80))
		{
			// 41 : A , 5A : Z
			// 61 : a , 7A : z
			if ( ( ( Str[ i ]>= 0x41 ) && ( Str[ i ] <= 0x5A ) ) || 
				 ( ( Str[ i ]>= 0x61 ) && ( Str[ i ] <= 0x7A) ) 
				) 
			{
				// english character 
				if( 90 >= Str[ i ] && Str[ i ] >= 65 )
				{
					SaveStr[ Count + m_SpaceCount ]   = Str[ i ];
					m_TempStr[ Count ] = ( Str[ i ] + 32 );
				}
				else
				{
					m_TempStr[ Count ] = Str[ i ];
				}

				Count++;
			}
			else if ( ( Str[ i ] >= '0' ) && ( Str[ i ] <= '9' ) )
			{
				// numeric character
				m_TempStr[ Count ] = Str[ i ];
				Count++;
			}
			else if ( Str[ i ] == ' ' )
			{
	           m_SpacePosition[ m_SpaceCount ] = ( i - m_SpaceCount );

		       m_SpaceCount++;
			}
			else
			{
				// 1 byte special character
				m_TempStr[ Count ] = Str[ i ];
				Count++;
			}
		}
		//----------------------------------------------------
		// 2 byte code
		//----------------------------------------------------
		else
		{
			// broken lead byte
			if (i + 1 == len) return false;

			if 
			(
			   (
			     ( (unsigned char)Str[i] >= 0xA4   ) && ( (unsigned char)Str[i] <= 0xA4 ) &&
			  	 ( (unsigned char)Str[i+1] >= 0xA1 ) && ( (unsigned char)Str[i+1] <= 0xFE )
			   )
			   ||	
			   ( 
			     ( (unsigned char)Str[i] >= 0xB0 ) && ( (unsigned char)Str[i] <= 0xC8 ) &&
			  	 ( (unsigned char)Str[i+1] >= 0xA1 ) && ( (unsigned char)Str[i+1] <= 0xFE )
			   )
			) 
			{ 
				m_TempStr[ Count   ] = Str[ i     ];
				m_TempStr[ Count+1 ] = Str[ i + 1 ];
				Count += 2; 
				i++;
				// valid Korean character 
			} 
			else if ( ( (unsigned char)Str[i]>= 0xCA ) && ( (unsigned char)Str[i] <= 0xFD ) &&
					  ( (unsigned char)Str[i+1] >= 0xA1 ) && ( (unsigned char)Str[i+1] <= 0xFE)
					) 
			{ 
				m_TempStr[ Count   ] = Str[ i     ];
				m_TempStr[ Count+1 ] = Str[ i + 1 ];
				Count += 2; 
				i++;
				// old - Chinese character

			} 
			else if ( ( (unsigned char)Str[i]>= 0x81 ) && ( (unsigned char)Str[i] <= 0x9f ) ||
					  ( (unsigned char)Str[i] >= 0xe0 ) && ( (unsigned char)Str[i] <= 0xfc )
					) 
			{ 
				m_TempStr[ Count   ] = Str[ i     ];
				m_TempStr[ Count+1 ] = Str[ i + 1 ];
				Count += 2; 
				i++;
				// Japanese Character
			} 
			else
			{
				
				//by dongs 필터링 수정 .. 
				if(Str[i] == -128 )
				{
					m_TempStr[ Count   ] = Str[ i     ];
					Count++;
				}
				else
				{
					m_TempStr[ Count   ] = Str[ i     ];
					m_TempStr[ Count+1 ] = Str[ i + 1 ];
					Count += 2;
					i++;
					// 2 byte special character

				}
			
			}
		}
		//.....................................................................................................
    }


    //..........................................................................
    // 필터링
    //..........................................................................
    i = 0;
    len = strlen( m_TempStr );

    while( i < len )
    {
        bool found = false;
        int left = 0;
        int right;
        int Result;

        right = m_pSlangList.size() - 1;

		//.....................................................................................................
		// 수정 루틴
		//.....................................................................................................
		std::vector <LPSLANG_DATA>::iterator pi = m_pSlangList.begin();
        
		for( ; pi != m_pSlangList.end() ; ++pi )
        {
			if( i >= len )
				return false;

			RTCHAR *TempStr = (RTCHAR*)m_TempStr;
			
            Result = Rstrncmp( (*pi)->StrCompare , &TempStr[ i ] , Rstrlen( (*pi)->StrCompare ) );
            
			if( Result == 0 )
            {
                memcpy( &m_TempStr[ i ] , (*pi)->StrReplace , sizeof(RTCHAR) * Rstrlen( (*pi)->StrReplace ) );

                m_ISSlangFound = TRUE;

				found = true;

                break;
            }
        }

        if( found )
        {
            i += Rstrlen( (*pi)->StrReplace );
        }
        else
        {
			//----------------------------------------------------
			// 1 byte code
			//----------------------------------------------------
			if (!(((unsigned char)m_TempStr[ i ]) & 0x80))
			{
				if ( ( ( m_TempStr[ i ]>= 0x41 ) && ( m_TempStr[ i ] <= 0x5A ) ) || 
					( ( m_TempStr[ i ]>= 0x61 ) && ( m_TempStr[ i ] <= 0x7A) ) 
					) 
				{
					// english character 
					i++;
				}
				else if ( ( m_TempStr[ i ] >= '0' ) && ( m_TempStr[ i ] <= '9' ) )
				{
					// numeric character
					i++;
				}
				else if ( m_TempStr[ i ] == ' ' )
				{
					// 1 byte white space
					i++;
				}
				else
				{
					// 1 byte special character
					i++;
				}
			}
			
			//----------------------------------------------------
			// 2 byte code
			//----------------------------------------------------
			else
			{
				// broken lead byte
				if (i + 1 == len) return i++;
				
				if 
					(
					(
					( (unsigned char)m_TempStr[i] >= 0xA4   ) && ( (unsigned char)m_TempStr[i] <= 0xA4 ) &&
					( (unsigned char)m_TempStr[i+1] >= 0xA1 ) && ( (unsigned char)m_TempStr[i+1] <= 0xFE )
					)
					||	
					( 
					( (unsigned char)m_TempStr[i] >= 0xB0 ) && ( (unsigned char)m_TempStr[i] <= 0xC8 ) &&
					( (unsigned char)m_TempStr[i+1] >= 0xA1 ) && ( (unsigned char)m_TempStr[i+1] <= 0xFE )
					)
					) 
				{ 
					i += 2; 
					// valid Korean character 
				} 
				else if ( ( (unsigned char)m_TempStr[i]>= 0xCA ) && ( (unsigned char)m_TempStr[i] <= 0xFD ) &&
					( (unsigned char)m_TempStr[i+1] >= 0xA1 ) && ( (unsigned char)m_TempStr[i+1] <= 0xFE)
					) 
				{ 
					i += 2; 
					// old - Chinese character					
				} 
				else if ( ( (unsigned char)m_TempStr[i]>= 0x81 ) && ( (unsigned char)m_TempStr[i] <= 0x9f ) ||
					  ( (unsigned char)m_TempStr[i] >= 0xe0 ) && ( (unsigned char)m_TempStr[i] <= 0xfc )
					) 
				{ 
					i += 2; 
					// Japanese Character
				} 
				else
				{

					//by dongs 필터링 수정 .. 
					if(m_TempStr[i] == -128 )
					{
						i++;
					}
					else
					{
						i += 2;
					}
					
					// 2 byte special character
				}
			}
			//.....................................................................................................
		}
	}

    len = strlen( m_TempStr );

    char Temp[ 1024 ] = "";

    memset( Temp , 0 , 1024 );

    int DestCount = 0;
    int SrcCount  = 0;

    for( i = 0 ; i < m_SpaceCount; i++ )
    {
        for( int j = SrcCount; j < m_SpacePosition[ i ] ; j++ )
        {
            Temp[ DestCount ] = m_TempStr[ j ];

            DestCount++;
        }

        Temp[ DestCount ] = ' ';
        DestCount++;
        SrcCount = m_SpacePosition[ i ];
	}

    for( i = SrcCount; i < len ; i++ )
    {
        Temp[ DestCount ] = m_TempStr[ i ];

        DestCount++;
    }

    Temp[ DestCount ] = '\0';

	Rsprintf( RWCHAR(m_TempStr) , _RT("%s") , RWCHAR(Temp) );

	int StrLen = strlen( m_TempStr );
	
	for( i = 0 ; i < StrLen ; ++i )
	{
		if( SaveStr[ i ] != 0 )
		{
			if( m_TempStr[ i ] != '*' )
			{
				m_TempStr[ i ] = SaveStr[ i ];
			}
		}

	}
    
    return true;
}

int CSlangFilter::FilterW(WCHAR *Str)
{
	WCHAR SaveStr[ MAX_SLANG_STRING ] = {0,};	

	m_ISSlangFound = FALSE;

    m_SpaceCount = 0;

    int len = wcslen( Str );

    if( len <= 0 )
        return false;

    memset( m_TempStr , 0 ,  MAX_SLANG_STRING );
    memset( SaveStr   , 0 ,  MAX_SLANG_STRING );
	
	WCHAR *wTempStr = reinterpret_cast<WCHAR*>(m_TempStr);


    //..........................................................................
    // 공백문자 없애기
    //..........................................................................
    int Count = 0;
    int i;

    for( i = 0 ; i < len ; ++i )
    {
		// 41 : A , 5A : Z
		// 61 : a , 7A : z
		if ( ( ( Str[ i ]>= 0x0041 ) && ( Str[ i ] <= 0x005A ) ) || 
			( ( Str[ i ]>= 0x0061 ) && ( Str[ i ] <= 0x007A) ) 
			) 
		{
			// english character 소문자 만들기
			if( 0x005A >= Str[ i ] && Str[ i ] >= 0x0041 )
			{
				SaveStr[ Count + m_SpaceCount ]   = Str[ i ];
				wTempStr[ Count ] = ( Str[ i ] + 32 );
			}
			else
			{
				wTempStr[ Count ] = Str[ i ];
			}
			
			Count++;
		}
		else if ( ( Str[ i ] >= '0' ) && ( Str[ i ] <= '9' ) )
		{
			// numeric character
			wTempStr[ Count ] = Str[ i ];
			Count++;
		}
		else if ( Str[ i ] == ' ' )
		{
	           m_SpacePosition[ m_SpaceCount ] = ( i - m_SpaceCount );
			   m_SpaceCount++;
		}
		else
		{
			// 1 byte special character
			wTempStr[ Count ] = Str[ i ];
			Count++;
		}
		//.....................................................................................................
    }


    //..........................................................................
    // 필터링
    //..........................................................................
    i = 0;
    len = wcslen( wTempStr);

    while( i < len )
    {
        bool found = false;
        int left = 0;
        int right;
        int Result;

        right = m_pSlangList.size() - 1;

		//.....................................................................................................
		// 수정 루틴
		//.....................................................................................................
		std::vector <LPSLANG_DATA>::iterator pi = m_pSlangList.begin();
        
		for( ; pi != m_pSlangList.end() ; ++pi )
        {
			if( i >= len )
				return false;
								
			WCHAR *wStrCompare = reinterpret_cast<WCHAR*>((*pi)->StrCompare);

            Result = wcsncmp(  wStrCompare , &wTempStr[ i ] , wcslen( wStrCompare ) );
            
			if( Result == 0 )
            {
                memcpy( &wTempStr[ i ] , RWCHAR((*pi)->StrReplace)  , sizeof(RTCHAR)*Rstrlen( RWCHAR((*pi)->StrReplace)   ) );

                m_ISSlangFound = TRUE;

				found = true;

                break;
            }
        }

        if( found )
        {
            i += wcslen( (WCHAR*)(*pi)->StrReplace );
        }
        else
        {
			i++;
		}	
		
	}

    len = wcslen( wTempStr );

    WCHAR Temp[512] = L"";

    memset( Temp , 0 , sizeof(WCHAR)*512 );

	int DestCount = 0;
    int SrcCount  = 0;

    for( i = 0 ; i < m_SpaceCount; i++ )
    {
        for( int j = SrcCount; j < m_SpacePosition[ i ] ; j++ )
        {
            Temp[ DestCount ] = wTempStr[ j ];

            DestCount++;
        }

        Temp[ DestCount ] = _RT(' ');
        DestCount++;
        SrcCount = m_SpacePosition[ i ];
	}

    for( i = SrcCount; i < len ; i++ )
    {
        Temp[ DestCount ] = wTempStr[ i ];

        DestCount++;
    }

    Temp[ DestCount ] = _RT('\0');

	Rsprintf( RWCHAR(m_TempStr) , _RT("%s") , RWCHAR(Temp) );

	int StrLen = wcslen( wTempStr );
	
	for( i = 0 ; i < StrLen ; ++i )
	{
		if( SaveStr[ i ] != 0 )
		{
			if( wTempStr[ i ] != _RT('*') )
			{
				wTempStr[ i ] = SaveStr[ i ];
			}
		}

	}
    
    return true;
}

int CSlangFilter::ISFindFilterName(void *Str)
{
#ifdef UNICODE_ROCK
	return ISFindFilterNameW( (WCHAR*)Str);
#else
	return ISFindFilterNameA( (char*)Str);
#endif
}

int CSlangFilter::ISFindFilterNameA(char *Str)
{
    m_SpaceCount = 0;

    int len = strlen( Str );

    if( len <= 0 )
        return true;

    memset( m_TempStr , 0 , MAX_SLANG_STRING );

    //..........................................................................
    // 공백문자 없애기

    //..........................................................................
    int Count = 0;
    int i;

    for( i = 0 ; i < len ; ++i )
    {
		//----------------------------------------------------
		// 1 byte code
		//----------------------------------------------------
		if (!(((unsigned char)Str[ i ]) & 0x80))
		{
			if ( ( ( Str[ i ]>= 0x41 ) && ( Str[ i ] <= 0x5A ) ) || 
				 ( ( Str[ i ]>= 0x61 ) && ( Str[ i ] <= 0x7A) ) 
				) 
			{
				// english character 
				if( 90 >= Str[ i ] && Str[ i ] >= 65 )
				{
					m_TempStr[ Count ] = ( Str[ i ] + 32 );
				}
				else
				{
					m_TempStr[ Count ] = Str[ i ];
				}

				Count++;
			}
			else if ( ( Str[ i ] >= '0' ) && ( Str[ i ] <= '9' ) )
			{
				// numeric character
				m_TempStr[ Count ] = Str[ i ];
				Count++;
			}
			else if ( Str[ i ] == ' ' )
			{
				return true;
				// 1 byte white space
				m_TempStr[ Count ] = Str[ i ];
				Count++;
			}
			else
			{
				// 1 byte special character
				m_TempStr[ Count ] = Str[ i ];
				Count++;
			}
		}

		//----------------------------------------------------
		// 2 byte code
		//----------------------------------------------------
		else
		{
			// broken lead byte
			if (i + 1 == len) return false;

			if 
			(
			   (
			     ( (unsigned char)Str[i] >= 0xA4   ) && ( (unsigned char)Str[i] <= 0xA4 ) &&
			  	 ( (unsigned char)Str[i+1] >= 0xA1 ) && ( (unsigned char)Str[i+1] <= 0xFE )
			   )
			   ||	
			   ( 
			     ( (unsigned char)Str[i] >= 0xB0 ) && ( (unsigned char)Str[i] <= 0xC8 ) &&
			  	 ( (unsigned char)Str[i+1] >= 0xA1 ) && ( (unsigned char)Str[i+1] <= 0xFE )
			   )
			) 
			{ 
				m_TempStr[ Count   ] = Str[ i     ];
				m_TempStr[ Count+1 ] = Str[ i + 1 ];
				Count += 2; 
				i++;
				// valid Korean character 
			} 
			else if ( ( (unsigned char)Str[i]>= 0xCA ) && ( (unsigned char)Str[i] <= 0xFD ) &&
					  ( (unsigned char)Str[i+1] >= 0xA1 ) && ( (unsigned char)Str[i+1] <= 0xFE)
					) 
			{ 
				m_TempStr[ Count   ] = Str[ i     ];
				m_TempStr[ Count+1 ] = Str[ i + 1 ];
				Count += 2; 
				i++;
				// old - Chinese character

			} 
			else if ( ( (unsigned char)Str[i]>= 0x81 ) && ( (unsigned char)Str[i] <= 0x9f ) ||
					  ( (unsigned char)Str[i] >= 0xe0 ) && ( (unsigned char)Str[i] <= 0xfc )
					) 
			{ 
				m_TempStr[ Count   ] = Str[ i     ];
				m_TempStr[ Count+1 ] = Str[ i + 1 ];
				Count += 2; 
				i++;
				// Japanese Character
			} 
			else
			{
				m_TempStr[ Count   ] = Str[ i     ];
				m_TempStr[ Count+1 ] = Str[ i + 1 ];
				Count += 2;
				i++;
				// 2 byte special character
			}
		}
		//.....................................................................................................
    }

    //..........................................................................
    // 필터링
    //..........................................................................
    i = 0;
    len = strlen( m_TempStr );

    while( i < len )
    {
        bool found = false;
        int left = 0;
        int Result;

		//.....................................................................................................
		// 수정 루틴
		//.....................................................................................................
		std::vector <LPNAME_DATA>::iterator pi = m_pNameList.begin();
        
		for( ; pi != m_pNameList.end() ; ++pi )
        {
			if( i >= len )
				return false;
			
			RTCHAR *m_Str = (RTCHAR*)m_TempStr;

            Result = Rstrncmp( (*pi)->StrCompare , &m_Str[ i ] , Rstrlen( (*pi)->StrCompare ) );
            
			if( Result == 0 )
            {
                return true;
            }
        }
		
		//----------------------------------------------------
		// 1 byte code
		//----------------------------------------------------
		if (!(((unsigned char)m_TempStr[ i ]) & 0x80))
		{
			if ( ( ( m_TempStr[ i ]>= 0x41 ) && ( m_TempStr[ i ] <= 0x5A ) ) || 
				 ( ( m_TempStr[ i ]>= 0x61 ) && ( m_TempStr[ i ] <= 0x7A) ) 
				) 
			{
				// english character 
				i++;
			}
			else if ( ( m_TempStr[ i ] >= '0' ) && ( m_TempStr[ i ] <= '9' ) )
			{
				// numeric character
				i++;
			}
			else if ( m_TempStr[ i ] == ' ' )
			{
				// 1 byte white space
				i++;
			}
			else
			{
				// 1 byte special character
				i++;
			}
		}

		//----------------------------------------------------
		// 2 byte code
		//----------------------------------------------------
		else
		{
			// broken lead byte
			if (i + 1 == len) return false;

			if 
			(
			   (
			     ( (unsigned char)m_TempStr[i] >= 0xA4   ) && ( (unsigned char)m_TempStr[i] <= 0xA4 ) &&
			  	 ( (unsigned char)m_TempStr[i+1] >= 0xA1 ) && ( (unsigned char)m_TempStr[i+1] <= 0xFE )
			   )
			   ||	
			   ( 
			     ( (unsigned char)m_TempStr[i] >= 0xB0 ) && ( (unsigned char)m_TempStr[i] <= 0xC8 ) &&
			  	 ( (unsigned char)m_TempStr[i+1] >= 0xA1 ) && ( (unsigned char)m_TempStr[i+1] <= 0xFE )
			   )
			) 
			{ 
				i += 2; 
				// valid Korean character 
			} 
			else if ( ( (unsigned char)m_TempStr[i]>= 0xCA ) && ( (unsigned char)m_TempStr[i] <= 0xFD ) &&
					  ( (unsigned char)m_TempStr[i+1] >= 0xA1 ) && ( (unsigned char)m_TempStr[i+1] <= 0xFE)
					) 
			{ 
				i += 2; 
				// old - Chinese character

			} 
			else if ( ( (unsigned char)m_TempStr[i]>= 0x81 ) && ( (unsigned char)m_TempStr[i] <= 0x9f ) ||
					  ( (unsigned char)m_TempStr[i] >= 0xe0 ) && ( (unsigned char)m_TempStr[i] <= 0xfc )
					) 
			{ 
				i += 2; 
				// Japanese Character
			} 
			else
			{
				i += 2;
				// 2 byte special character
			}
		}
		//.....................................................................................................
    }

    return false;
}


int CSlangFilter::ISFindFilterNameW(WCHAR *Str)
{
    m_SpaceCount = 0;

    int len = wcslen( Str );

    if( len <= 0 )
        return true;

    memset( m_TempStr , 0 , MAX_SLANG_STRING );
	WCHAR *wTempStr =  reinterpret_cast<WCHAR*>(m_TempStr);
	
    //..........................................................................
    int Count = 0;
    int i;

    for( i = 0 ; i < len ; ++i )
    {
		//----------------------------------------------------
		// 1 byte code
		//----------------------------------------------------
		if ( ( ( Str[ i ]>= 0x41 ) && ( Str[ i ] <= 0x5A ) ) || 
			( ( Str[ i ]>= 0x61 ) && ( Str[ i ] <= 0x7A) ) 
			) 
		{
			// english character 
			if( 90 >= Str[ i ] && Str[ i ] >= 65 )
			{
				wTempStr[ Count ] = ( Str[ i ] + 32 );
			}
			else
			{
				wTempStr[ Count ] = Str[ i ];
			}
			
			Count++;
		}
		else if ( ( Str[ i ] >= '0' ) && ( Str[ i ] <= '9' ) )
		{
			// numeric character
			wTempStr[ Count ] = Str[ i ];
			Count++;
		}
		else if ( Str[ i ] == ' ' )
		{
			return true;
			// 1 byte white space
			wTempStr[ Count ] = Str[ i ];
			Count++;
		}
		else
		{
			// 1 byte special character
			wTempStr[ Count ] = Str[ i ];
			Count++;
		}
		
    }

    //..........................................................................
    // 필터링
    //..........................................................................
    i = 0;
    len = wcslen( wTempStr );

    while( i < len )
    {
        bool found = false;
        int left = 0;
        int Result;

		//.....................................................................................................
		// 수정 루틴
		//.....................................................................................................
		std::vector <LPNAME_DATA>::iterator pi = m_pNameList.begin();
        
		for( ; pi != m_pNameList.end() ; ++pi )
        {
			if( i >= len )
				return false;

			WCHAR* wStrCompare = reinterpret_cast<WCHAR*>((*pi)->StrCompare);

            Result = wcsncmp( wStrCompare ,  &wTempStr[ i ] , wcslen( wStrCompare ) );
            
			if( Result == 0 )
            {
                return true;
            }
        }
		
		//----------------------------------------------------
		// 1 byte code
		//----------------------------------------------------
	
		if ( ( ( m_TempStr[ i ]>= 0x41 ) && ( m_TempStr[ i ] <= 0x5A ) ) || 
			( ( m_TempStr[ i ]>= 0x61 ) && ( m_TempStr[ i ] <= 0x7A) ) 
			) 
		{
			// english character 
			i++;
		}
		else if ( ( m_TempStr[ i ] >= '0' ) && ( m_TempStr[ i ] <= '9' ) )
		{
			// numeric character
			i++;
		}
		else if ( m_TempStr[ i ] == ' ' )
		{
			// 1 byte white space
			i++;
		}
		else
		{
			// 1 byte special character
			i++;
		}
	}

    return false;
}
bool  CSlangFilter::IsValidString(void * buffer)
{
#ifdef UNICODE_ROCK
	return IsValidStringW((WCHAR*) buffer);
#else
	return IsValidStringA((char*) buffer);	
#endif

}


bool  CSlangFilter::IsValidStringA(char * buffer)
{
	int len = strlen(buffer);

	for (int i = 0 ; i < len ; i++)
	{
		//----------------------------------------------------
		// 1 byte code - 아스키 코드일때
		//----------------------------------------------------
		if( !(((unsigned char)buffer[i]) & 0x80) )
		{
			if ( ( ( buffer[i] >= 0x41 ) && ( buffer[i] <= 0x5A ) ) || 
				 ( ( buffer[i] >= 0x61 ) && ( buffer[i] <= 0x7A) ) 
				) 
			{
				// english character 
			}
			else if ( ( buffer[i] >= '0' ) && ( buffer[i] <= '9' ) )
			{
				// numeric character
			}
			else if ( buffer[i] == ' ' )
			{
				// 1 byte white space
				return false;
			}
			else
			{
				// 1 byte special character
				return false;
			}
		}
#ifdef INDONESIA_CLIENT		
		else 
		{
			return false;
		}
#else
		//----------------------------------------------------
		// 2 byte code
		//----------------------------------------------------
		else
		{
			// broken lead byte
			if (i + 1 == len) return false;

			if 
			(
			   (
			     ( (unsigned char)buffer[i] >= 0xA4 ) && ( (unsigned char)buffer[i] <= 0xA4 ) &&
			  	 ( (unsigned char)buffer[i+1] >= 0xA1 ) && ( (unsigned char)buffer[i+1] <= 0xFE )
			   )
			   ||	
			   ( 
			     ( (unsigned char)buffer[i] >= 0xB0 ) && ( (unsigned char)buffer[i] <= 0xC8 ) &&
			  	 ( (unsigned char)buffer[i+1] >= 0xA1 ) && ( (unsigned char)buffer[i+1] <= 0xFE )
			   )
			) 
			{ 
				++i; 
				// valid Korean character 
			} 
			else if ( ( (unsigned char)buffer[i]>= 0xCA ) && ( (unsigned char)buffer[i] <= 0xFD ) &&
					  ( (unsigned char)buffer[i+1] >= 0xA1 ) && ( (unsigned char)buffer[i+1] <= 0xFE)
					) 
			{ 
			
			
				++i; 
				// old - Chinese character
			} 
			else if ( ( (unsigned char)buffer[i]>= 0x81 ) && ( (unsigned char)buffer[i] <= 0x9f ) ||
					  ( (unsigned char)buffer[i] >= 0xe0 ) && ( (unsigned char)buffer[i] <= 0xfc )
					) 
			{ 
				if (  ( (unsigned char)buffer[i] == 0x81 ) && ( (unsigned char)buffer[i+1] == 0x40 ) )		// blank char, Space Key
				{
					return false;
				}
//				if( (unsigned char)buffer[i] == 0x81 )		// Special Char
//				if( (unsigned char)buffer[i] != 0x82 )		// Special Char
//				{
//					return false;
//				}

				++i; 
				// Japanese Character
			} 
			else
			{
				// 2 byte special character
				return false;
			}
		}
#endif
	}


	return true;
}


bool  CSlangFilter::IsValidStringW(WCHAR* buffer)
{
	int len = wcslen(buffer);

	for (int i = 0 ; i < len ; i++)
	{
		//----------------------------------------------------
		// 1 byte code - 아스키 코드일때
		//----------------------------------------------------
		if( buffer[i] < 0x0100 )
		{
			if ( ( ( buffer[i] >= 0x0041 ) && ( buffer[i] <= 0x005A ) ) || 
				 ( ( buffer[i] >= 0x0061 ) && ( buffer[i] <= 0x007A) ) 
				) 
			{
				// english character 
			}
			else if ( ( buffer[i] >= '0' ) && ( buffer[i] <= '9' ) )
			{
				// numeric character
			}
			else if ( buffer[i] == ' ' )
			{
				// 1 byte white space
				return false;
			}
			else
			{
				// 1 byte special character
				return false;
			}
		}
		else 
		{
			if ( ( buffer[i] >= 0x3300 ) && ( buffer[i] <= 0x9FFF ) ) 
			{
				
			}
			else
			{
				return false; 
			}
		}
	}

	return true;
}


bool  CSlangFilter::ConvertToUpper(char * buffer)
{
	int len = strlen(buffer);

	for (int i = 0 ; i < len ; i++)
	{
		//----------------------------------------------------
		// 1 byte code
		//----------------------------------------------------
		if (!(((unsigned char)buffer[i]) & 0x80))
		{
			if ( ( ( buffer[i]>= 0x41 ) && ( buffer[i] <= 0x5A ) ) || 
				 ( ( buffer[i] >= 0x61 ) && ( buffer[i] <= 0x7A) ) 
				) 
			{
				// english character 

				// 소문자를 대문자로 변환
				if( 122 >= buffer[ i ] && buffer[ i ] >= 97 )
		        {
			        buffer[ i ] = ( buffer[ i ] - 32 );
				}
			}
			else if ( ( buffer[i] >= '0' ) && ( buffer[i] <= '9' ) )
			{
				// numeric character
			}
			else if ( buffer[i] == ' ' )
			{
				// 1 byte white space
			}
			else
			{
				// 1 byte special character
			}
		}

		//----------------------------------------------------
		// 2 byte code
		//----------------------------------------------------
		else
		{
			// broken lead byte
			if (i + 1 == len) 
			{
//				return false;
			}

			if 
			(
			   (
			     ( (unsigned char)buffer[i] >= 0xA4 ) && ( (unsigned char)buffer[i] <= 0xA4 ) &&
			  	 ( (unsigned char)buffer[i+1] >= 0xA1 ) && ( (unsigned char)buffer[i+1] <= 0xFE )
			   )
			   ||	
			   ( 
			     ( (unsigned char)buffer[i] >= 0xB0 ) && ( (unsigned char)buffer[i] <= 0xC8 ) &&
			  	 ( (unsigned char)buffer[i+1] >= 0xA1 ) && ( (unsigned char)buffer[i+1] <= 0xFE )
			   )
			) 
			{ 
				++i; 
				// valid Korean character 
			} 
			else if ( ( (unsigned char)buffer[i]>= 0xCA ) && ( (unsigned char)buffer[i] <= 0xFD ) &&
					  ( (unsigned char)buffer[i+1] >= 0xA1 ) && ( (unsigned char)buffer[i+1] <= 0xFE)
					) 
			{ 
				++i; 
				// old - Chinese character
			} 
			else if ( ( (unsigned char)buffer[i]>= 0x81 ) && ( (unsigned char)buffer[i] <= 0x9f ) ||
					  ( (unsigned char)buffer[i] >= 0xe0 ) && ( (unsigned char)buffer[i] <= 0xfc )
					) 
			{ 
				++i; 
				// Japanese Character
			} 
			else
			{
				// 2 byte special character
			}
		}
	}

	return true;
}
