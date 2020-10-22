//.............................................................................................................
//
// CNPCImpactTable.h										NPC 타입별 임펙트 프레임 테이블
//
//.............................................................................................................
#pragma warning(disable:4786)

#include "RockPCH.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "VFileSystem.h"
#include "..\\Effect\\CTextCommand.h"

typedef struct s_ImpactTableData
{

	int		frame[10];

}IMPACTTABLE_DATA;

using namespace std;

class CNPCImpactTable
{
public:
	 
	 CNPCImpactTable();
	~CNPCImpactTable();

public:

	int Load(void);
	int GetFrame(int index,int frame);

	map <int,IMPACTTABLE_DATA>		m_NpcImpactMap;

};