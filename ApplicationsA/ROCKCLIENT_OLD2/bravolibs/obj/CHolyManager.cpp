#pragma warning(disable:4786)

#include "RockPCH.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "VFileSystem.h"
#include "CHolyManager.h"


CHolyManager::CHolyManager()
{
}

CHolyManager::~CHolyManager()
{
}

int	CHolyManager::LoadTable(void)
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FILE *fp = NULL;

	fp = fopen( "npc\\holy.dat" , "r+b" );

	if( NULL == fp )
		return FALSE;

	while( TRUE )
	{
		HOLYTABLE TempTable;

		if( fread( &TempTable, sizeof(HOLYTABLE) , fp ) <= 0 )
		{
			break;
		}
		
		for( int x = 0 ; x < HOLY_LEVEL_MAX_COUNT ; x++ )
		{
			m_HolyNPCCodeList.push_back( TempTable.HolyCode[ x ] );
		}
		
		m_HolyTableList.push_back( TempTable );
	}

	fclose(fp);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else

	VFileHandle* pFH = g_VFNpcRoot.OpenFile( "Holy.dat" );
	
	if( pFH == NULL ) return false;
	
	int Table_Num = pFH->GetSize() / sizeof( HOLYTABLE );

	HOLYTABLE TempTable;
	
	for( int i = 0; i < Table_Num; ++i )
	{
		if( pFH->Read( &TempTable, sizeof(HOLYTABLE) ) <= 0 )
		{
			break;
		}
		
		for( int x = 0 ; x < HOLY_LEVEL_MAX_COUNT ; x++ )
		{
			m_HolyNPCCodeList.push_back( TempTable.HolyCode[ x ] );
		}
		
		m_HolyTableList.push_back( TempTable );
	}

	g_VFNpcRoot.CloseFile( pFH );
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

	return TRUE;
}

int CHolyManager::GetHolyCode(int BaseCode,int Level)
{
	std::vector<HOLYTABLE>::iterator i = m_HolyTableList.begin();

	for( ; i != m_HolyTableList.end() ; ++i )
	{
		if( (i)->HolyBaseCode == BaseCode )
		{
			return i->HolyCode[ GetLevelToTableNum( Level ) ];
		}
	}

	return -1;
}

int CHolyManager::GetCreateEffect (int BaseCode,int Level)
{
	std::vector<HOLYTABLE>::iterator i = m_HolyTableList.begin();

	for( ; i != m_HolyTableList.end() ; ++i )
	{
		if( (i)->HolyBaseCode == BaseCode )
		{
			return i->CreateEffect[ GetLevelToTableNum( Level ) ];
		}
	}

	return -1;
}

int CHolyManager::GetDeleteEffect (int BaseCode,int Level)
{
	std::vector<HOLYTABLE>::iterator i = m_HolyTableList.begin();

	for( ; i != m_HolyTableList.end() ; ++i )
	{
		if( (i)->HolyBaseCode == BaseCode )
		{
			return i->DeleteEffect[ GetLevelToTableNum( Level ) ];
		}
	}

	return -1;
}

int CHolyManager::GetLevelUpEffect(int BaseCode,int Level)
{
	std::vector<HOLYTABLE>::iterator i = m_HolyTableList.begin();

	for( ; i != m_HolyTableList.end() ; ++i )
	{
		if( (i)->HolyBaseCode == BaseCode )
		{
			return i->LevelUpEffect[ GetLevelToTableNum( Level ) ];
		}
	}

	return -1;
}

int CHolyManager::GetLevelToTableNum(int Level)
{
	int TableNum = 4;

	switch( Level )
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			TableNum = 0;
			break;

		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			TableNum = 1;
			break;

		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			TableNum = 2;
			break;

		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
			TableNum = 3;
			break;

		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
			TableNum = 4;
			break;
	}

	return TableNum;
}

int CHolyManager::GetPetCode(int PetCode,int Level)
{
	return -1;
}

int CHolyManager::GetPetCreateEffect(int PetCode,int Level)
{
	std::vector<PETTABLE>::iterator i = m_PetTableList.begin();

	for( ; i != m_PetTableList.end() ; ++i )
	{
		for( int j = 0 ; j < PET_LEVEL_MAX_COUNT ; j++)
		{
			if( PetCode == i->PetCode[ j ] )
			{
				return i->CreateEffect[ j ];
			}
		}
	}
	
	return -1;
}

int CHolyManager::GetPetDeleteEffect(int PetCode,int Level)
{
	std::vector<PETTABLE>::iterator i = m_PetTableList.begin();

	for( ; i != m_PetTableList.end() ; ++i )
	{
		for( int j = 0 ; j < PET_LEVEL_MAX_COUNT ; j++)
		{
			if( PetCode == i->PetCode[ j ] )
			{
				return i->DeleteEffect[ j ];
			}
		}
	}
	
	return -1;
}

int CHolyManager::GetPetLevelUpEffect(int PetCode,int Level)
{
	std::vector<PETTABLE>::iterator i = m_PetTableList.begin();

	for( ; i != m_PetTableList.end() ; ++i )
	{
		for( int j = 0 ; j < PET_LEVEL_MAX_COUNT ; j++)
		{
			if( PetCode == i->PetCode[ j ] )
			{
				return i->LevelUpEffect[ j ];
			}
		}
	}	
	
	return -1;
}

int CHolyManager::GetPetLevelToTableNum(int Level)
{
	int TableNum = 3;

	return TableNum;
}