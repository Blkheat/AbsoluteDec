#include "RockPCH.h"

#include	<base\\d3dbase.h>
#include	<OBJ\\ObjBase.H>
#include	"CharItem.h"
#include	"RockClient.h"
#include	<stdio.h>

#include	"profile\profile.h"
#include	"profile\custom_time.h"

CBravoItem::CBravoItem()
{
}

CBravoItem::~CBravoItem()
{
	FinalCleanup();
}

bool		CBravoItem::Create()
{
	//ProfileBegin("< CBravoItem::Create >");
	// 캐릭터 데이타 로딩
	char	chr_path[ 128 ] = {0,};
	strcpy(chr_path, g_RockClient.GetRootDir());
	
	if( g_Pc_Manager.Create(chr_path) == false )
	{
		//ProfileEnd("< CBravoItem::Create >");
	
		ClientClose( g_RockClient.GetApp() , 1);
		return false;
	}

	//ProfileEnd("< CBravoItem::Create >");
	return true;
}

void		CBravoItem::FinalCleanup()
{
}

