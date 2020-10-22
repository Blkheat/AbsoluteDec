///---------------------------------------------------------------------------
///-- CMapDataManager class

#if !defined(__UIDATA_MANAGER_H_INCLUDED__)
#define __UIDATA_MANAGER_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RockPCH.h"
#include "RockClient.h"
#include "defineUI_ID.h"
#include <vector>

using namespace std;

#pragma pack(push,1)

typedef struct SUiDataInfo //ui테이블 구조체
{
	DWORD	dIndex;
	DWORD	dPosition;
	DWORD	dCtrltype;
	DWORD	dIid;
	DWORD	dPid;
	DWORD	dUi_X,dUi_Y,dUi_W,dUi_H;

	BYTE	dAlpha;
	BYTE	dUi_Option;
	BYTE	dEnable;
	BYTE	dVisible;

	DWORD dImage;
	
	SUiDataInfo()
	{
	   memset(this, 0, sizeof(SUiDataInfo) );
	}

}UI_DATA,*LPUI_DATA;


#pragma pack(pop)

class CUiDataManager
{

public:	 
	 CUiDataManager();
	~CUiDataManager();


	BOOL LoadData();  //ui데이터 로드
	void ClearData(); //데이터 삭제

	
	LPUI_DATA CUiDataManager::GetData(int Num);

protected: 
	vector < SUiDataInfo > m_UiDataList; 

};

extern CUiDataManager g_UiDataManager;

#endif