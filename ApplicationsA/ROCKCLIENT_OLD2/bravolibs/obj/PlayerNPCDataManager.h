#ifndef __PLAYERNPCDATAMANAGER_H
#define __PLAYERNPCDATAMANAGER_H

#pragma warning(disable:4786)
#include <windows.h>
#include <map>

using namespace std;

#pragma pack(push,1)
struct SPlayerNPCData
{
	DWORD	NPC_Code;
	BYTE	Citizen;								
	BYTE	Gender;
	BYTE	MainClass;

	DWORD   Hair_Code;
	DWORD   Face_Code;	
	DWORD	CoatArmor_Code;
	DWORD	PantsArmor_Code;
	DWORD	Gloves_Code;
	DWORD	Shoes_Code;
	DWORD	Helmet_Code;
	DWORD	Serve_Code;
	DWORD	LeftArm_Code;
	DWORD	RightArm_Code;

	SPlayerNPCData()
	{
		memset(this, 0, sizeof(SPlayerNPCData));
	}	
};
#pragma pack(pop)

typedef map < DWORD, SPlayerNPCData > PlayerNPCDataList;
typedef map < DWORD, SPlayerNPCData >::iterator PlayerNPCDataListPos;

class CPlayerNPCDataManager
{
public:	 
	 CPlayerNPCDataManager();
	~CPlayerNPCDataManager();

	BOOL LoadData(void);

	void ClearData();

	SPlayerNPCData FindPlayerNPCData(DWORD nCode);

protected:
	PlayerNPCDataList m_PlayerNPCDataList;
};


#endif