#ifndef __CHOLYMANAGER_H
#define __CHOLYMANAGER_H

#include <vector>

#define HOLY_LEVEL_MAX_COUNT	5
#define PET_LEVEL_MAX_COUNT		4

typedef struct s_HolyTable
{
	int	HolyBaseCode;
	
	int HolyCode     [ HOLY_LEVEL_MAX_COUNT ];
	int CreateEffect [ HOLY_LEVEL_MAX_COUNT ];
	int DeleteEffect [ HOLY_LEVEL_MAX_COUNT ];
	int LevelUpEffect[ HOLY_LEVEL_MAX_COUNT ];

	void operator=(const s_HolyTable TempData)
	{
		memcpy( this , &TempData , sizeof( s_HolyTable ) );
	}

}HOLYTABLE, *LPHOLYTABLE;

typedef struct s_PetTable
{

	int PetBaseCode;

	int PetCode		 [ PET_LEVEL_MAX_COUNT ];
	int CreateEffect [ PET_LEVEL_MAX_COUNT ];
	int DeleteEffect [ PET_LEVEL_MAX_COUNT ];
	int LevelUpEffect[ PET_LEVEL_MAX_COUNT ];
}PETTABLE, *LPPETTABLE;

class CHolyManager
{
public:
	 
	 CHolyManager();
	~CHolyManager();

public:

	int	LoadTable(void);
	
//신수 관련	
	int GetHolyCode(int BaseCode,int Level);
	int GetCreateEffect (int BaseCode,int Level);
	int GetDeleteEffect (int BaseCode,int Level);
	int GetLevelUpEffect(int BaseCode,int Level);

	int GetLevelToTableNum(int Level);
	
//Pet 관련	
	int GetPetCode(int PetCode,int Level);
	int GetPetCreateEffect (int PetCode,int Level);
	int GetPetDeleteEffect (int PetCode,int Level);
	int GetPetLevelUpEffect(int PetCode,int Level);

	int GetPetLevelToTableNum(int Level);
	
	std::vector<HOLYTABLE>	m_HolyTableList;
	std::vector<int>		m_HolyNPCCodeList;

	std::vector<PETTABLE>	m_PetTableList;
	std::vector<int>		m_PetNPCCodeList;	
};

#endif
