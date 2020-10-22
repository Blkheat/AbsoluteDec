
#include "MapDataManager.h"
#include "..\\VFileSystem.h"

CMapDataManager g_MapDataManager;

CMapDataManager::CMapDataManager()
{

}

CMapDataManager::~CMapDataManager()
{
	ClearData();
}

DWORD CMapDataManager::GetMiniMapID( DWORD al_map_index )
{
	int nSize = m_MapDataList.size();
	
	assert( nSize > 0  );

	if( al_map_index > nSize )
	{
		return 0;
	}
	
	if( al_map_index >= nSize )
	{
		::MessageBox( NULL,"MapInfo Data Error", 0, 0 );
	}

	return ( DWORD )m_MapDataList[al_map_index].dMiniId;
}

DWORD CMapDataManager::GetStrID( DWORD al_map_index )
{
	int nSize = m_MapDataList.size();
	
	assert( nSize > 0  );	

	if( al_map_index > nSize )
	{
		return 0;
	}
	
	if( al_map_index >= nSize )
	{
		::MessageBox( NULL,"MapInfo Data Error", 0, 0 );
	}
	
	return ( DWORD )m_MapDataList[al_map_index].dStrId;
}

BYTE  CMapDataManager::GetNation( DWORD al_map_index )
{
	BYTE bNation = MAP_NEUTRALITY_NATION;

	int i=0; 
	
	for( i=0 ;i < m_MapDataList.size() ; ++i )
	{
		if( m_MapBaseInfo[i].theMapIndex == al_map_index)
		{
			return m_MapBaseInfo[i].theCitizen; 
		}
	}
	
	/*
	int nSize = m_MapDataList.size();
	
	assert( nSize > 0  );
	
	if( al_map_index > nSize )
	{
		return 0;
	}
	
	if( al_map_index >= nSize )
	{
		::MessageBox( NULL,"MapInfo Data Error", 0, 0 );
	}

	BYTE bNation = MAP_NEUTRALITY_NATION;

#ifdef NEW_CLIENT_MAPTYPE	
	bNation = ( BYTE )m_MapDataList[al_map_index].bNation;
#endif
	*/


	return bNation;
}

BYTE  CMapDataManager::GetMapType( DWORD al_map_index )
{
	int nSize = m_MapDataList.size();
	
	assert( nSize > 0  );
	assert( al_map_index < nSize );
	
	if( al_map_index >= nSize )
	{
		::MessageBox( NULL,"MapInfo Data Error", 0, 0 );
	}

	BYTE bMapType = MAP_TYPE_NORMAL;

#ifdef NEW_CLIENT_MAPTYPE	
	bMapType = ( BYTE )m_MapDataList[al_map_index].bMapType;
#endif

	return bMapType;
}

BOOL CMapDataManager::LoadData()
{

#ifdef CUR_INDUN_CHECK
	LoadDataMapBase();
#endif 


	ClearData();

	VFileHandle* pFH = NULL;

	pFH =  g_VFChrRoot.OpenFile( "ClientMap.dat" );

	if( NULL == pFH )
	{
		char lac_error[300] = "";
		sprintf(lac_error," Cannot Open File : ClientMap.dat " );
		
		MessageBox( NULL, lac_error, _T("!알림"), MB_OK );
		return FALSE;
	}

	///---------------------------------------------------------------------------
	///-- Read Data

	int nfile_size = pFH->GetSize();

	///-- 사이즈 정규성 Check
	if( nfile_size % sizeof( SMapDtaInfo ) != 0 )
	{
		g_VFChrRoot.CloseFile( pFH );
		return FALSE;
	}
	///-- Set Count
	int n_count = nfile_size / sizeof( SMapDtaInfo );

	for( int i = 0; i < n_count ; ++i )
	{
		SMapDtaInfo MapDataInfo;	
		pFH->Read( ( void * )&MapDataInfo, sizeof( SMapDtaInfo ));

		if( i != MapDataInfo.dIndex )
		{
			g_VFChrRoot.CloseFile( pFH );
			MessageBox( NULL, _T("! Error : Invalid Map Index"), _T("Error"), MB_OK );
			return FALSE;
		}

		m_MapDataList.push_back(MapDataInfo);
	}

	g_VFChrRoot.CloseFile( pFH );

	return TRUE;
}

BOOL CMapDataManager::LoadDataMapBase()
{
	ClearData();
	
	VFileHandle* pFH = NULL;
	pFH =  g_VFChrRoot.OpenFile( "Map.dat" );
	
	if( NULL == pFH )
	{
		char lac_error[300] = "";
		sprintf(lac_error," Cannot Open File : Map.dat " );
		
		MessageBox( NULL, lac_error, _T("!알림"), MB_OK );
		return FALSE;
	}
	
	///---------------------------------------------------------------------------
	///-- Read Data
	
	int nfile_size = pFH->GetSize();
	///-- 사이즈 정규성 Check
	if( nfile_size % sizeof( SMapBaseInfo ) != 0 )
	{
		g_VFChrRoot.CloseFile( pFH );
		return FALSE;
	}
	///-- Set Count
	int n_count = nfile_size / sizeof( SMapBaseInfo );
	
	for( int i = 0; i < n_count ; ++i )
	{
		SMapBaseInfo MapDataInfo;	
	
		if( pFH->Read( ( void * )&MapDataInfo, sizeof( SMapBaseInfo )  ) == FALSE )
		{
			break; 
		}
		
		m_MapBaseInfo.push_back(MapDataInfo);
	}
	
	g_VFChrRoot.CloseFile( pFH );
	
	return TRUE;
}

BOOL CMapDataManager::GetInDunInfo(int iMapIndex)
{
	int i=0; 
	
	for( i=0 ;i < m_MapDataList.size() ; ++i )
	{
		if( m_MapBaseInfo[i].theMapIndex == iMapIndex)
		{
			return (BOOL)m_MapBaseInfo[i].theIncoFlag; 
		}
	}

	return FALSE;
}


void CMapDataManager::ClearData()
{
	m_MapDataList.clear();
	m_MapBaseInfo.clear();
}