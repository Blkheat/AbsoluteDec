#include "RockPCH.h"
#include "CStringToCode.h"


CStringToCode::CStringToCode(const char *pStr)
{
	m_pString  = NULL;
	m_HashCode = 0;

	SetString( pStr );
}

CStringToCode::~CStringToCode()
{
	SAFE_DELETE_ARRAY( m_pString );
}

CStringToCode &CStringToCode::operator=(const CStringToCode &HashString)
{
	if( NULL != HashString.m_pString )
	{
		SetString( HashString.m_pString );
	}
	
	return *this;
}

bool CStringToCode::operator<(const CStringToCode &HashString) const
{
	if( m_HashCode != HashString.m_HashCode )
	{
		return ( m_HashCode < HashString.m_HashCode );
	}
	
	return ( strcmp( m_pString , HashString.m_pString ) < 0 );
}


int CStringToCode::SetString(const char *pStr)
{
	
	SAFE_DELETE_ARRAY( m_pString );

	int Len = strlen( pStr );

	if( Len <= 0 ) return false;

	m_pString = SAFE_NEW_ARRAY( char , Len + 1 );	

	memset( m_pString , 0 , Len + 1 );
	
	for( int i = 0 ; i < Len ; ++i )
	{
		if( pStr[ i ] <= 122 && pStr[ i ] >= 97 )
		{
			m_pString[ i ] = pStr[ i ] - 32;
		}
		else
		{
			m_pString[ i ] = pStr[ i ];
		}
	}

	m_HashCode = MakeHashCode( m_pString );

	return true;
}

unsigned long CStringToCode::MakeHashCode(const char *pStr)
{
	unsigned long ch;
	unsigned long result = 5381;

	int len = strlen( pStr );
	
	for( int i = 0 ; i < len ; ++i )
	{
		ch     = ( unsigned long ) pStr[ i ];
		result = ( ( result << 5 ) + result ) + ch;	// hash * 33 + ch;
	}

	return result;
}