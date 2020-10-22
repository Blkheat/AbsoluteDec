//...........................................................................................................
// 
//	CNodeSort.h								소팅 관련 By wxywxy
//
//...........................................................................................................
#ifndef CNODESORT_H
#define CNODESORT_H
#include <d3dx9math.h>
#include <d3d9.h>
#include <vector>
#include <list>

typedef enum 
{
	ID_NPC	   =	0	,		// NPC
	ID_PC				,		// PC
	ID_OBJ				,		// OBJ
	ID_PLAYER			,		// PLAYER
	ID_NONE	   = 255			// NONE

}OBJECT_IDENTITY;

typedef struct s_NodeSortData
{
	OBJECT_IDENTITY		ID;
	
	DWORD				Index;
	DWORD				ChildIndex;
	float               distance;
	BYTE				ISBlend;

	s_NodeSortData()
	{
		ID			= ID_NONE;
		Index		= 0;
		ChildIndex	= 0;
		distance    = 0;
		ISBlend     = FALSE;
	}

	void Clear(void)
	{
		ID			= ID_NONE;
		Index		= 0;
		ChildIndex	= 0;
		distance    = 0;
		ISBlend     = FALSE;
	}


}NODE_SORT_DATA;

class CNodeSort
{
public:
	
	 CNodeSort();
	~CNodeSort();

public:

	 int	m_MaxTableCount;
	 int    m_DataSize;

	 std::vector	<NODE_SORT_DATA>	m_SortList;
	 std::list	    <int>				m_SortNPCList;
	 std::list	    <int>				m_SortPCList;

	 NODE_SORT_DATA						m_StaticData;

public:

	 int    Clear(void);

	 int	SetTableMaxCount(int Size);
	 int    GetTableMaxCount(void)			{ return m_MaxTableCount; }
	
	 int    GetCount(void)					{ return m_DataSize;	  }
	 
	 int	Insert(OBJECT_IDENTITY ID,DWORD Index,DWORD ChildIndex,BYTE ISBlend);
	 int	Delete(OBJECT_IDENTITY ID,DWORD Index,DWORD ChildIndex);

	 int	Sort(void);

	 NODE_SORT_DATA *Get(int Index);
	
	 int	StartNPCPointer(void);	
	 int	StartPCPointer(void);	
	 
	 int	GetNPC(void);
	 int	GetPC(void);
	
	 std::list	    <int>::iterator m_SortNPCStart;
	 std::list	    <int>::iterator m_SortPCStart;
};

#endif