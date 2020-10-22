#include "CClassView.h"
#include <algorithm>
#include "VFileSystem.h"
#include "..\\effect\\CTextCommand.h"
#include "..\\effect\\CTextCommandW.h"
#include "rockclient.h"

#define CLASSVIEW_FILENAME	"classview.dat"


CClassView::CClassView()
{
}

CClassView::~CClassView()
{
}

int CClassView::Load(void)
{
	// std::map<BYTE,CLASS_VIEW_DATA>		m_ClassMap;
#ifdef UNICODE_ROCK
	CTextCommandW	CText;
#else
	CTextCommand	CText;
#endif
	

	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	char FileName[ 1024 ] = "";
		
	wsprintf( FileName , "%s\\Etc\\%s" , g_RockClient.GetRootDir() , CLASSVIEW_FILENAME );

	if( CText.Open( FileName ) == NULL )
	{
		return FALSE;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	
	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	if( l_nDevSkip )
	{
		char FileName[ 1024 ] = "";		
		wsprintf( FileName , "%s\\local_test\\%s", g_RockClient.GetRootDir() , CLASSVIEW_FILENAME );		
		if( CText.Open( FileName ) == NULL )
		{
			::MessageBox(NULL,"Cannot Found classview.dat",0,0);
			return FALSE;
		}		
	}
	else
	{
		if( CText.OpenVF( CLASSVIEW_FILENAME ) == NULL )
		{
			return FALSE;
		}
	}
	
	
	
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


	while( CText.GetLine() != NULL )
	{
		if( CText.ISArgEmpty() ) continue;

		if( CText.EqualArgToString( 0 , _RT("HELP") ))
		{
			BYTE  Num;

			CText.GetArgValue( 1 , Num );
			
			// 클래스 데이터가 있는지 검사
			CLASS_VIEW_HELP	TempHelp;

			while( CText.GetLine() != NULL )
			{				
				if( CText.EqualArgToString( 0 , _RT("}") ) )	
				{
					m_ClassHelpMap.insert( 
					std::map<BYTE,CLASS_VIEW_HELP>::value_type( Num , TempHelp ) );

					break;
				}
				else if( CText.EqualArgToString( 0 , _RT("{") ) )
				{
					continue;
				}
				else
				{
#ifdef UNICODE_ROCK
					wstring TempStr;
#else
					string TempStr;
#endif	
						
					void * pStr = (void *)CText.GetString();				
					::StringTrim(pStr);
					TempStr = CText.GetString();

					TempHelp.StrHelp.push_back( TempStr );
				}
			}
		}
		else
		{
			BYTE ClassNum;
			
			CLASS_VIEW_DATA	TempData;

			CText.GetArgValue( 0  , ClassNum );
			CText.GetArgValue( 1  , TempData.Race );
			CText.GetArgValue( 2  , TempData.Sex  );
			CText.GetArgValue( 3  ,	TempData.Hair );
			CText.GetArgValue( 4  , TempData.Face );
			CText.GetArgValue( 5  , TempData.Helmet);
			CText.GetArgValue( 6  , TempData.Coat );
			CText.GetArgValue( 7  , TempData.Pants );
			CText.GetArgValue( 8  , TempData.Gloves );
			CText.GetArgValue( 9  , TempData.Shoes );
			CText.GetArgValue( 10  , TempData.Armor );
			CText.GetArgValue( 11 , TempData.Shiled );
			CText.GetArgValue( 12 , TempData.AniCount );

			int AniNum;
			int i = 13;

			for( ; i < ( 13 + TempData.AniCount ) ; ++i )
			{
				CText.GetArgValue( i , AniNum );
				
				TempData.AniList.push_back( AniNum );
			}

			m_ClassMap.insert( 
			std::map<BYTE,CLASS_VIEW_DATA>::value_type( ClassNum , TempData ) );
		
		}

	}

	CText.Close();

	return TRUE;
}


CLASS_VIEW_DATA *CClassView::GetData(BYTE class_num)
{
	std::map<BYTE,CLASS_VIEW_DATA>::iterator pos;

	pos = m_ClassMap.find( class_num );

	if( pos != m_ClassMap.end() )
	{
		return &pos->second;
	}

	return NULL;
}

CLASS_VIEW_HELP *CClassView::GetHelpData(BYTE class_num)
{
	std::map<BYTE,CLASS_VIEW_HELP>::iterator pos;

	pos = m_ClassHelpMap.find( class_num );

	if( pos != m_ClassHelpMap.end() )
	{
		return &pos->second;
	}

	return NULL;
}
