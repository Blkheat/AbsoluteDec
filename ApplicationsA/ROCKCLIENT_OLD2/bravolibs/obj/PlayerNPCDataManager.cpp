#include "etcstruct.h"
#include "PlayerNPCDataManager.h"

#include "..\\..\\VFileSystem.h"

CPlayerNPCDataManager::CPlayerNPCDataManager()
{
	ClearData();
}

CPlayerNPCDataManager::~CPlayerNPCDataManager()
{
	ClearData();
}

BOOL CPlayerNPCDataManager::LoadData(void)
{
	ClearData();

	char FileName[ 1024 ] = "";		
	wsprintf( FileName , "%s" , "PlayerNPC.dat" );

	VFileHandle* pFH = g_VFNpcRoot.OpenFile( FileName );
	VFileHandle* pTFH = NULL;

	if( pFH == NULL ) 
	{
		return FALSE;
	}

	int nfile_size = pFH->GetSize();

	///-- 사이즈 정규성 Check
	if( nfile_size % sizeof( SPlayerNPCData ) != 0 )
	{
		g_VFNpcRoot.CloseFile( pFH );
		return FALSE;
	}
	///-- Set Count
	int n_count = nfile_size / sizeof( SPlayerNPCData );

	for( int i = 0; i < n_count ; ++i )
	{
		SPlayerNPCData PNPCData;	
		pFH->Read( ( void * )&PNPCData, sizeof( SPlayerNPCData ));

		DWORD nNpcCode = PNPCData.NPC_Code;
		m_PlayerNPCDataList.insert(map <DWORD , SPlayerNPCData>::value_type(nNpcCode, PNPCData));	
	}	

	g_VFNpcRoot.CloseFile( pFH );		
	
	return TRUE;
}

void CPlayerNPCDataManager::ClearData()
{
	m_PlayerNPCDataList.clear();
}

SPlayerNPCData CPlayerNPCDataManager::FindPlayerNPCData(DWORD nCode)
{
	SPlayerNPCData PNPCData;
	
	PlayerNPCDataListPos iPos = m_PlayerNPCDataList.find(nCode);
	
	if(iPos == m_PlayerNPCDataList.end())
	{
		return PNPCData;	
	}
	
	PNPCData = (*iPos).second;	
	
	return PNPCData;
}