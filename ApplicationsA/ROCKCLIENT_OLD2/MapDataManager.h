///---------------------------------------------------------------------------
///-- CMapDataManager class

#if !defined(__MAPDATA_MANAGER_H_INCLUDED__)
#define __MAPDATA_MANAGER_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RockPCH.h"
#include "RockClient.h"
#include "defineMap_ID.h"
#include <vector>

using namespace std;

#define G_MAP_MINIMAPID(a) (DWORD)g_MapDataManager.GetMiniMapID(a)
#define G_MAP_STRID(a) (DWORD)g_MapDataManager.GetStrID(a)
#define G_MAP_NATION(a) (BYTE)g_MapDataManager.GetNation(a)
#define G_MAP_MAPTYPE(a) (BYTE)g_MapDataManager.GetMapType(a)

enum NMAP_NATION_TYPE
{
	MAP_MILENA_NATION = 0,
	MAP_RAIN_NATION,
	MAP_NEUTRALITY_NATION,
};

enum NMAP_TYPE
{
	MAP_TYPE_NORMAL = 0,
	MAP_TYPE_COLONY,
	MAP_TYPE_TOWN,
	MAP_TYPE_BATTLEROOM,
	MAP_TYPE_PVPNEUTFIELD,
};

#pragma pack(push,1)
struct SMapDtaInfo
{
	DWORD	dIndex;
	DWORD	dMiniId;
	DWORD	dStrId;
	
#ifdef NEW_CLIENT_MAPTYPE
	BYTE	bNation;
	BYTE	bMapType;
#endif
	
	SMapDtaInfo()
	{
	   memset(this, 0, sizeof(SMapDtaInfo) );
	}
};
#pragma pack(pop)

class CMapDataManager
{
public:	 
	 CMapDataManager();
	~CMapDataManager();

	BOOL LoadData();
	
	BOOL LoadDataMapBase();	//map.dat 로드한다... 인던정보를 알기 위해서 추가함..


	void ClearData();

	DWORD GetMiniMapID( DWORD al_map_index );
	DWORD GetStrID( DWORD al_map_index );
	BYTE  GetNation( DWORD al_map_index );
	BYTE  GetMapType( DWORD al_map_index );

	BOOL		GetInDunInfo(int iMapIndex) ;
	
protected:
	vector < SMapDtaInfo > m_MapDataList;  
	vector < SMapBaseInfo >m_MapBaseInfo; 
};

extern CMapDataManager g_MapDataManager;

#endif