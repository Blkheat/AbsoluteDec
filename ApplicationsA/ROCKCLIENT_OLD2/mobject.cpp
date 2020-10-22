#include "RockPCH.h"

#include	<base\\d3dbase.h>
#include	<obj\\object_manager.h>
#include	"MObject.h"
#include	"RockClient.h"
#include	<stdio.h>
#include	"profile\profile.h"
#include	"profile\custom_time.h"

CBravoObject::CBravoObject()
{
	m_dwObjCount = 0;
}

CBravoObject::~CBravoObject()
{
	FinalCleanup();
} 

bool   CBravoObject::Create( char* ObjPath )
{
	//ProfileBegin( "< CBravoObject::Create >" );
	DebugPrint("\r\n CBravoObject::Create start");

	char	path[128];
	ZeroMemory(path, sizeof(char)*128);
	
	strcpy(path, ObjPath);
	strcat(path, "\\Obj");
	
	//ProfileBegin( "g_Obj_Manager.ClearObj" );
	g_Obj_Manager.ClearObj();//....sh
	//ProfileEnd( "g_Obj_Manager.ClearObj" );

	//ProfileBegin( "g_Obj_Manager.Create" );

	///-- �̰����� /obj/data �� ����ִ� ��� msh, dds ������ �о�´�.
	if( g_Obj_Manager.Create( path ) == false )
	{
		return false;
	}
	//ProfileEnd( "g_Obj_Manager.Create" );
	
#if !defined( LOAD_VF )
	
	m_dwObjCount	= g_Obj_Manager.GetFileCount(path,"\\*.obj");	/// by sooree
	m_szObjNameList	= SAFE_NEW_ARRAY( char* , m_dwObjCount );	
		
	ZeroMemory(m_szObjNameList, sizeof(char*)*m_dwObjCount);

	DWORD count = 0;
	for( DWORD i = 0; count < m_dwObjCount; i++ )
	{				
		m_szObjNameList[count]	= SAFE_NEW_ARRAY( char , 256 );	
		ZeroMemory(m_szObjNameList[count], sizeof(char)*256);
		sprintf( m_szObjNameList[count], "%s\\obj\\%08d.obj", g_RockClient.GetCurrentMapPath(), count );	/// by sooree
		++count;
	}	
#else

	/// sooree pack
/// Object_Manager::GetFileCountVF(...) ����! 
///		1) ȣ������ �ݵ�� �ش���丮�� VF.ChangeDir(...) �̸� ���־�� �Ѵ�. 
///		2) file_ext�� "obj", "msh" �� ó�� Ȯ���� �����ڸ� �ѱ�� 

	
	///-- obj ������ ��� ������ �����´�.
	g_VFMap.ChangeDir( "/obj" );
	m_dwObjCount = g_Obj_Manager.GetFileCountVF( &g_VFMap, "obj" );
	m_szObjNameList	= SAFE_NEW_ARRAY( char* , m_dwObjCount );	
	
	if( m_szObjNameList == NULL )
	{
		MessageBox( NULL , "Create Object Name List...1" , "Error" , MB_OK );

		return FALSE;
	}
	ZeroMemory(m_szObjNameList, sizeof(char*)*m_dwObjCount);	

	DWORD count = 0;
	//ProfileBegin( "m_szObjNameList<for>" );

	///-- obj ������ �̸��� ����Ѵ�.
	for( DWORD i = 0; count < m_dwObjCount; i++ )
	{				
		m_szObjNameList[count]	= SAFE_NEW_ARRAY( char , 256 );		
		
		if( m_szObjNameList[count] == NULL )
		{
			MessageBox( NULL , "Create Object Name List...2" , "Error" , MB_OK );

			return FALSE;
		}

		ZeroMemory(m_szObjNameList[count], sizeof(char)*256);
		sprintf( m_szObjNameList[count], "%08d.obj", count );	/// by sooree
		++count;
	}

	//ProfileEnd( "m_szObjNameList<for>" );
#endif

	
	//ProfileEnd( "< CBravoObject::Create >" );	
	DebugPrint("\r\n CBravoObject::Create end");
	return true;
}

void		CBravoObject::FinalCleanup()
{
	for ( DWORD i = 0; i < m_dwObjCount; i ++ )
	{
		SAFE_DELETE_ARRAY( m_szObjNameList[i] );  
	}

	SAFE_DELETE_ARRAY( m_szObjNameList );
}

DWORD		CBravoObject::GetIndexByName(char* name)
{
	for ( DWORD i = 0; i < m_dwObjCount; i ++ )
	{
		if ( strcmp(name, m_szObjNameList[i]) == 0 )
		{
			return i;
		}
	}

	return 0;
}