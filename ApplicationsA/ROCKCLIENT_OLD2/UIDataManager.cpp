
#include "UIDataManager.h"
#include "..\\VFileSystem.h"

CUiDataManager g_UiDataManager;

CUiDataManager::CUiDataManager()
{

}

CUiDataManager::~CUiDataManager()
{
	ClearData();
}

BOOL CUiDataManager::LoadData()
{

	ClearData();

	VFileHandle* pFH = NULL;

	pFH =  g_VFInterface.OpenFile( "ClientUi.dat" );

	if( NULL == pFH )
	{
		char lac_error[300] = "";
		sprintf(lac_error," Cannot Open File : ClientUi.dat " );
		
		MessageBox( NULL, lac_error, _T("!알림"), MB_OK );
		return FALSE;
	}

	///---------------------------------------------------------------------------
	///-- Read Data

	int nfile_size = pFH->GetSize();

	///-- 사이즈 정규성 Check
	if( nfile_size % sizeof( UI_DATA ) != 0 )
	{
		g_VFInterface.CloseFile( pFH );
		return FALSE;
	}
	
	///-- Set Count
	int n_count = nfile_size / sizeof( UI_DATA );

	for( int i = 0; i < n_count ; ++i )
	{
		UI_DATA UiDataInfo;	
		pFH->Read( ( void * )&UiDataInfo, sizeof( UI_DATA ));

		m_UiDataList.push_back(UiDataInfo);
	}

	g_VFInterface.CloseFile( pFH );

	return TRUE;
}

LPUI_DATA CUiDataManager::GetData(int Num)
{
	std::vector <UI_DATA>::iterator iter = m_UiDataList.begin();
	
	for( ; iter != m_UiDataList.end() ; ++iter )
	{
		if( iter->dIndex == ( short ) Num )
		{
			return &(*iter);
		}
	}

	return NULL;
}


void CUiDataManager::ClearData()
{
	m_UiDataList.clear();
}