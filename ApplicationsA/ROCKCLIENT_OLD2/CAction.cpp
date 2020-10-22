#include "RockPCH.h"
#include "CAction.h"
#include <algorithm>
#include "VFileSystem.h"
#include "StringManager.h"

CAction::CAction()
{
}

CAction::~CAction()
{
}

int	CAction::Load(void)
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	char FileName[ 1024 ] = "";
		
	wsprintf( FileName , "%s\\%s" , g_Rocklient.m_szRootDir , CACTION_DATA_FILE );
		
	FILE *fp = NULL;
		
	fp = fopen( FileName , "r+b" );

	if( NULL == fp )
		return FALSE;
		
	while( 1 )
	{
		ACTION_DATA TempData;
	
		if( FALSE == fread( &TempData , sizeof( ACTION_DATA ) , 1 , fp ) )
			break;
		
		m_List.push_back( TempData );
	}

	fclose( fp );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	char FileName[ 1024 ] = "";
		
	wsprintf( FileName , "%s" , CACTION_DATA_FILE );

	VFileHandle* pFH = g_VFChrRoot.OpenFile( FileName );		

	if( pFH == NULL )
		return FALSE;
		
	while( 1 )
	{
		ACTION_DATA TempData;
	
		if( pFH->Read( &TempData , sizeof( ACTION_DATA ) ) == FALSE )
			break;	

		m_List.push_back( TempData );
	}

	g_VFChrRoot.CloseFile( pFH );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

LPACTION_DATA CAction::GetData(int Num)
{
	std::vector <ACTION_DATA>::iterator i = m_List.begin();
	
	for( ; i != m_List.end() ; ++i )
	{
		if( i->Index == ( short ) Num )
		{
			return  &(*i);
		}
	}

	return NULL;
}

BOOL CAction::ISKeep(int Num)
{
	if( Num >= m_List.size() )
		return FALSE;

	return ( ( BOOL ) m_List[ Num ].ISKeep );
}

LPACTION_DATA CAction::CheckKeyWord(char *pStr)
{
	if( Rstrlen( pStr ) < 2 )
		return NULL;
	
#ifdef UNICODE_ROCK
	wchar_t *wpStr = RWCHAR(pStr);
	wchar_t *wpStr2 =  (wchar_t*)pStr[1];
	wchar_t *pCompareStr = &wpStr[ 1 ];

#else
	char *pCompareStr = &pStr[ 1 ];
#endif 
	

	std::vector <ACTION_DATA>::iterator i = m_List.begin();
	
	for( ; i != m_List.end() ; ++i )
	{
		for( int n = 0 ; n < 3 ; ++n )
		{
			if( i->KeyWord[ n ] != -1 )
			{
				if( Rstrcmp( RWCHAR(pCompareStr) , G_STRING_CHAR( i->KeyWord[ n ] ) ) == 0 )
				{
					return  &(*i);
				}
			}
		}
	}

	return NULL;
}
