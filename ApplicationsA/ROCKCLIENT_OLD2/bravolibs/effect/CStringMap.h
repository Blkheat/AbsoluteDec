#ifndef __CStringMap_H
#define __CStringMap_H

#include "RockPCH.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

#pragma warning(disable:4786)

#include "CStringToCode.h"


template <class HASH_DATA>
class CStringMap
{
public:
	 
	 CStringMap() {};
	~CStringMap() { Clear(); };

	int		   Clear(void)
	{
		map <unsigned long,HASH_DATA *>::iterator i = m_HashTable.begin();

		for( ; i != m_HashTable.end() ; ++i )
		{
			SAFE_DELETE( i->second );
		}

		m_HashTable.clear();

		m_nList.clear();

		return true;
	}

	HASH_DATA *Find(char *pStr)
	{
		map <unsigned long,HASH_DATA *>::iterator pos;

		CStringToCode	HashString( pStr );

		pos = m_HashTable.find( HashString.GetHashCode() );

		if( pos != m_HashTable.end() )
		{
			return pos->second;
		}

		return NULL;
	}
	
	HASH_DATA *Find(unsigned long nHashCode)
	{
		map <unsigned long,HASH_DATA *>::iterator pos;
		
		pos = m_HashTable.find( nHashCode );

		if( pos != m_HashTable.end() )
		{
			return pos->second;
		}

		return NULL;
	}	

	int		   Erase(char *pStr)
	{
		map <unsigned long,HASH_DATA	*>::iterator pos;

		CStringToCode	HashString( pStr );

		pos = m_HashTable.find( HashString.GetHashCode() );

		if( pos != m_HashTable.end() )
		{
			SAFE_DELETE( pos->second );

			m_HashTable.erase( pos );

			return true;
		}
		
		vector < unsigned long >::iterator  vpos = find( m_nList.begin(), m_nList.end(), 
												   HashString.GetHashCode());
		m_nList.erase(vpos);
		
		return false;
	}

	int	Insert(char *pStr,HASH_DATA **pData)
	{
		if( NULL != Find( pStr ) )
			return false;

		CStringToCode	HashString( pStr );
		
		m_HashTable.insert( 
			map<unsigned long,HASH_DATA *>::value_type( HashString.GetHashCode() , (*pData) ) 
			);

		m_nList.push_back(HashString.GetHashCode());

		return true;
	}

public:
	
	vector < unsigned long >			m_nList;
	map <unsigned long,HASH_DATA *>		m_HashTable;

	vector < unsigned long > * GetnList()	{ return &m_nList; }
};

#endif
