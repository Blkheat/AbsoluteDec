//...........................................................................................................
// 
//	CObjectSort.cpp						소팅 관련 By wxywxy
//
//...........................................................................................................
#include "CNodeSort.h"

CNodeSort::CNodeSort()
{
	m_DataSize      = 0;
	m_MaxTableCount = 0;
}

CNodeSort::~CNodeSort()
{
}

int CNodeSort::Clear(void)
{
	for( int i = 0 ; i < m_DataSize ; ++i )
	{
		m_SortList[ i ].Clear();
	}

	m_SortNPCList.clear();
	m_SortPCList.clear();

	m_DataSize = 0;

	return TRUE;
}

int	CNodeSort::SetTableMaxCount(int Size)
{
	m_SortList.clear();
	m_SortNPCList.clear();
	m_SortPCList.clear();
	m_SortList.resize(Size);
	m_MaxTableCount = Size;
	m_DataSize      = 0;

	return TRUE;
}

int	CNodeSort::Insert(OBJECT_IDENTITY ID,DWORD Index,DWORD ChildIndex,BYTE ISBlend)
{
	int InsertNum = 0;
	
	for( int i = 0 ; i < m_MaxTableCount ; ++i )
	{
		if( m_SortList[ i ].ID == ID_NONE )
			break;
			
		InsertNum++;
	}

	m_SortList[ InsertNum ].ID			= ID;
	m_SortList[ InsertNum ].Index		= Index;
	m_SortList[ InsertNum ].ChildIndex = ChildIndex;
	m_SortList[ InsertNum ].ISBlend    = ISBlend;

	m_DataSize++;

	switch( ID )
	{
		case ID_NPC:
			m_SortNPCList.push_back( Index );
			break;
		case ID_PC:
			m_SortPCList.push_back( Index );
			break;
	}

	return TRUE;
}

int CNodeSort::Delete(OBJECT_IDENTITY ID,DWORD Index,DWORD ChildIndex)
{
	int Count = m_SortList.size();

	for( int i = 0  ; i < m_MaxTableCount ; ++i )
	{
		if( Count <= i )
			return FALSE;

		if( m_SortList[ i ].ID    == ID	   &&
			m_SortList[ i ].Index == Index &&
			m_SortList[ i ].ChildIndex == ChildIndex   )
		{
			m_SortList[ i ].Clear();

			m_DataSize--;

			if( ID == ID_NPC )
			{
				std::list	<int>::iterator	pi = m_SortNPCList.begin();

				for( ; pi != m_SortNPCList.end() ; ++pi )
				{
					if( Index == (*pi) )
					{
						m_SortNPCList.erase( pi );
		
						return TRUE;
					}
				}
			}
			else if( ID == ID_PC )
			{
				std::list	<int>::iterator	pi = m_SortPCList.begin();

				for( ; pi != m_SortPCList.end() ; ++pi )
				{
					if( Index == (*pi) )
					{
						m_SortPCList.erase( pi );
						
						return TRUE;
					}
				}
			}
			
			return TRUE;
		}
	}

	return FALSE;
}

int	CNodeSort::Sort(void)
{

	return TRUE;
}

NODE_SORT_DATA *CNodeSort::Get(int Index)
{
	return &m_SortList[ Index ];
}

int	CNodeSort::StartNPCPointer(void)
{
	m_SortNPCStart = m_SortNPCList.begin();

	return TRUE;
}

int	CNodeSort::StartPCPointer(void)
{	
	m_SortPCStart = m_SortPCList.begin();

	return TRUE;
}
	 
int	CNodeSort::GetNPC(void)
{
	if( m_SortNPCStart != m_SortNPCList.end() )
	{
		int Index = (*m_SortNPCStart);
	
		m_SortNPCStart++;

		return Index;
	}
	
	return -1;
}

int	CNodeSort::GetPC(void)
{
	if( m_SortPCStart != m_SortPCList.end() )
	{
		int Index = (*m_SortPCStart);

		m_SortPCStart++;

		return Index;
	}

	return -1;
}

