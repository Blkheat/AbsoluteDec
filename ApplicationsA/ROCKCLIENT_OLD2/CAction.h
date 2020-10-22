//...................................................................................................................
//
// CAction.h															By wxywxy
// 사교동작
//...................................................................................................................
#ifndef __CATCTION_H
#define __CATCTION_H

#include <windows.h>
#include <vector>

#define CACTION_DATA_FILE		"setaction.dat"

#pragma pack(push,1)
typedef struct s_ActionData
{
	
	short Index;
	short StartAni[4];		
	short KeepAni[4];
	BYTE  ISKeep;
	short KeyWord[3];
	BYTE  ViewRight;
	BYTE  ViewLeft;
	BYTE  ViewRide;
	BYTE  NpcAni;

}ACTION_DATA,*LPACTION_DATA;
#pragma pack(pop)

class CAction
{
public:
	 
	 CAction();
	~CAction();

public:

	int				Load(void);
	LPACTION_DATA	GetData(int Num);
	LPACTION_DATA	CheckKeyWord(char *pStr);
	BOOL			ISKeep(int Num);

private:

	std::vector <ACTION_DATA>	m_List;

};

#endif