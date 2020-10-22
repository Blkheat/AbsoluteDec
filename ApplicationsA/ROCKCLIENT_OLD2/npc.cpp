#include "RockPCH.h"

#include	<NETWORK\\Protocol.h>
#include	<base\\d3dbase.h>
#include	<obj\\Player_manager.h>
#include	"quadlist.h"
#include	"Pc.h"
//#include	"Network.h"
#include	"RockClient.h"
#include	"Npc.h"
#include	<stdio.h>
//#include	"Menu_Interface.h"

unsigned char		g_SellList	= 0;
//TELEPORT_NC			g_NCTeleport[MAX_TELEPORT_NC];
static		int		g_nTel	= -1;

CBravoNpc::CBravoNpc()
{
}

CBravoNpc::~CBravoNpc()
{
	FinalCleanup();
}

bool		CBravoNpc::Create()
{
	char	path[128]; 
	ZeroMemory(path, sizeof(char)*128);
	strcpy(path, g_RockClient.GetRootDir());
	strcat(path, "\\Npc");
	m_dwCount	= g_Pc_Manager.GetFileCount(path,"\\*.brv_chr");
	m_szNameList	= SAFE_NEW_ARRAY( char* , m_dwCount );

	for ( DWORD i = 0; i < m_dwCount; i ++ )
	{
		m_szNameList[i]	= SAFE_NEW_ARRAY( char , 128 );		
		ZeroMemory(m_szNameList[i], sizeof(char)*128);
		strcpy(m_szNameList[i], g_RockClient.GetRootDir());
		ZeroMemory(path, sizeof(char)*128);
		sprintf(path, "\\Npc\\%04d.brv_chr", i);
		strcat(m_szNameList[i], path);
	}

	return true;
}

void		CBravoNpc::FinalCleanup()
{
	for ( DWORD i = 0; i < m_dwCount; i ++ )
	{
		_DELETE_ARRAY( m_szNameList[i] );
	}

	_DELETE_ARRAY( m_szNameList );
}

DWORD		CBravoNpc::GetIndexByName(char* name)
{
	for ( DWORD i = 0; i < m_dwCount; i ++ )
	{
		if ( strcmp(name, m_szNameList[i]) == 0 )
			return i;
	}
	return 0;
}

