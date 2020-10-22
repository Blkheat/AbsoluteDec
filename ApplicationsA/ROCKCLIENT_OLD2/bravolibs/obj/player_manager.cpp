#include "RockPCH.h"
#include "VFileSystem.h"
#include "MapDataManager.h"

#include	<NETWORK\Protocol.h>
#include	<Base\\D3DBase.h>
#include	"..\\quadlist.h"
#include	"..\\RockClient.h"
#include	"..\\Pc.H"
#include	"..\\Camera.h"
#include	"Player_Manager.H"
#include	"ObjMath.H"
#include	"Line_Vertex.H"
#include	<assert.H>
#include	<stdio.h>
#include	<sys/stat.h> 

#include    "Effect\\CTextCommand.h"						// NPC�Ӽ������߰� By wxywxy
#include    "..\\Effect.h"
#include	"profile\profile.h"
#include	"profile\custom_time.h"

#include	"RockInterface\\UiUtil.h"
#include	"RockInterface\\MiniMapWndProc.h"
#include	"RockInterface\\BattleZoneWndProc.h"
#include	"RockInterFace\\ItemEldaMixAWndProc.h"
#include	"RockInterFace\\ItemSelectPotionWndProc.h"

#ifdef DIRECT_VERSION_9_MJH
extern		LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern		LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern		long				g_nowTime;

#define		CHR_SORT_TIME			5000.0f			// ���ýð�(5��)

int			gncount = 0;


Player_Manager	g_Pc_Manager;
D3DXVECTOR3 g_vZeroVec = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 g_vOneVec = D3DXVECTOR3(1.0f,1.0f,1.0f);

//-----------------------------------------------------------------------------
// Desc: Object_ManagerŬ���� �ʱ�ȭ, �ı���
//-----------------------------------------------------------------------------
void Player_Manager::Clear()
{
	memset( m_pMod, 0, sizeof(Model*         ) * (c_Chr_Mod_Count+c_Npc_Mod_Count) );
	memset( m_pTex, 0, sizeof(Texture*       ) * (c_Chr_Tex_Count+c_Npc_Tex_Count) );
	memset( m_pAni, 0, sizeof(Bone*          ) * (c_Chr_Ani_Count+c_Npc_Ani_Count) );
	memset( m_pItm, 0, sizeof(Character_Item*) * (c_Chr_Itm_Count+c_Npc_Itm_Count) );
	
	//	m_pSortTable	= NULL;	
	
	theChrMod		= NULL;
	theChrTex		= NULL;
	theChrAni		= NULL;
	theChrItm		= NULL;	
	m_pPcFileInfo	= NULL;
		
#ifndef DYNAMIC_RESOURCE_LOADING		
	theNpcMod		= NULL;
	theNpcTex		= NULL;
	theNpcAni		= NULL;
	theNpcItm		= NULL;	
	m_pNpcFileInfo	= NULL;
#endif	

	m_mod_count = 0;
	m_tex_count = 0;
	m_ani_count = 0;
	m_itm_count = 0;
	m_lPreSortTime	= g_nowTime;
	
	D3DUtil_InitLight( theCharacter_Light, D3DLIGHT_DIRECTIONAL, 100.0f, -100.0f, 100.0f );	
	D3DUtil_InitLight( theObject_Light, D3DLIGHT_DIRECTIONAL, 100.0f, -100.0f, 100.0f );
	D3DUtil_InitLight( theTerrain_Light, D3DLIGHT_DIRECTIONAL, 100.0f, -100.0f, 100.0f );
	D3DUtil_InitLight( theSky_Light, D3DLIGHT_DIRECTIONAL, 100.0f, -100.0f, 100.0f );
	
	D3DUtil_InitMaterial( theCharacter_Material, 1.0f, 1.0f, 1.0f, 1.0f);
	D3DUtil_InitMaterial( theObject_Material, 1.0f, 1.0f, 1.0f, 1.0f);
	D3DUtil_InitMaterial( theTerrain_Material, 1.0f, 1.0f, 1.0f, 1.0f);
	D3DUtil_InitMaterial( theSky_Material, 1.0f, 1.0f, 1.0f, 1.0f);
	theFog_Start = 0.0f;
	theFog_End = 0.0f;
	
	memset( &theAutoHelpTipMsg, 0, sizeof(SAutoHelpTipMsg) );
	
	theNationType = n_Nation_Common; 
	
	memset( &theGameModeDesc, 0, sizeof(char*) * c_Max_GameMode_Num );
	
	return;
}


void Player_Manager::Destroy()
{
	SAFE_DELETE_ARRAY(m_pPcFileInfo);
	SAFE_DELETE_ARRAY(theChrMod);	
	SAFE_DELETE_ARRAY(theChrTex);
	SAFE_DELETE_ARRAY(theChrAni);
	SAFE_DELETE_ARRAY(theChrItm);
	
#ifdef DYNAMIC_RESOURCE_LOADING
	
	DELETE_ALL_PTR_MEMBER_MAP(m_pNpcFileInfo);
	DELETE_ALL_PTR_MEMBER_MAP(m_NpcMod);
	DELETE_ALL_PTR_MEMBER_MAP(m_NpcTex);	
	DELETE_ALL_PTR_MEMBER_MAP(m_NpcAni);	
	DELETE_ALL_PTR_MEMBER_MAP(m_NpcItm);
	
#else
	
	SAFE_DELETE_ARRAY(m_pNpcFileInfo);
	SAFE_DELETE_ARRAY(theNpcMod);
	SAFE_DELETE_ARRAY(theNpcTex);
	SAFE_DELETE_ARRAY(theNpcAni);
	SAFE_DELETE_ARRAY(theNpcItm);
	
#endif
	
	int i = 0;
	
	for( i = 0; i < c_Max_ScriptStr_Num; ++i )
	{
		if( theScriptStrTable[i].theString )
		{
			SAFE_DELETE_ARRAY(theScriptStrTable[i].theString);	
		}
		else
		{
			break;
		}
	}
	
	for( i = 0; i < c_Max_GameMode_Num; ++i )
	{
		SAFE_DELETE_ARRAY( theGameModeDesc[i] );	
	}
	
	//Clear();
}

void Player_Manager::DestroyPC( int aMapIndex )
{	
	for( int i = 0; i < MAX_USER_INTABLE; ++i )
	{
		if( g_Pc.m_Pc[i].m_bIsLoaded )
		{
			g_Particle.StopDisplay( g_Pc.m_PcTable[i].lUnique, g_Pc.m_Pc[i].GetIndex() );
			g_Billboard.DelManager( g_Pc.m_PcTable[i].lUnique, g_Pc.m_Pc[i].GetIndex() );
			g_Particle.m_EffectManager.Delete( ( Character * ) &g_Pc.m_Pc[i] );
			g_Particle.DelEffSetCon( g_Pc.m_Pc[i].GetUnique() , g_Pc.m_Pc[i].GetIndex() );
			
			g_Pc.m_PcTable[i].Init();	
			g_Pc.m_Pc[i].m_bIsLoaded = false;
			g_Pc.m_Pc[i].Destroy();
			
			g_Pc.m_Pc[i].m_bIsLoaded = false;
			g_Pc.m_Pc[i].m_bIsVisible = false;				
		}
	}
	
	int nation_type = GetMapNationType( aMapIndex ); 
	if( theNationType == nation_type )
		return;
	
	SAFE_DELETE_ARRAY(m_pPcFileInfo);	
	SAFE_DELETE_ARRAY(theChrMod);	
	SAFE_DELETE_ARRAY(theChrTex);
	SAFE_DELETE_ARRAY(theChrAni);
	SAFE_DELETE_ARRAY(theChrItm);
}

void Player_Manager::DestroyNPC()
{
	for( int i = 0; i < MAX_NPC_INTABLE; ++i )
	{			
		if( g_Map.m_Npc[i].m_bIsLoaded )
		{
			// �ش� Ÿ���� ����Ʈ����
			g_Particle.StopDisplay( g_Map.m_NpcTable[i].lUnique, g_Map.m_Npc[i].GetIndex() );
			g_Billboard.DelManager( g_Map.m_NpcTable[i].lUnique, g_Map.m_Npc[i].GetIndex() );
			//			g_ChatList.DelUserBalloon( unique, index );
			
			// ����Ʈ ����Ʈ ����� By wxywxy
			g_Particle.m_EffectManager.Delete( ( Character * ) &g_Map.m_Npc[i] );
			g_Particle.DelEffSetCon( g_Map.m_Npc[i].GetUnique() , g_Map.m_Npc[i].GetIndex() );
			
			//			std::vector<int>::iterator ir = g_Map.m_Npc[i].thePreQuestCodeList.begin();
			//			for( ; ir != g_Map.m_Npc[who].thePreQuestCodeList.end() ; ++ir )
			//			{
			g_Map.m_Npc[i].thePreQuestCodeList.clear();
			g_Map.m_Npc[i].m_QuestMarkType = NONE_QUEST_MARK;
			//			}				
			
			// NPC���̺����
			g_Map.m_NpcTable[i].Init();
			g_Map.m_Npc[i].Destroy();
			
			g_Map.m_Npc[i].m_bIsLoaded = false;
			g_Map.m_Npc[i].m_bIsVisible = false;		
		}
	}
	
	g_Map.m_dwNpcCount = 0;
	
#ifdef DYNAMIC_RESOURCE_LOADING	
	
	DELETE_ALL_PTR_MEMBER_MAP(m_pNpcFileInfo);	
	DELETE_ALL_PTR_MEMBER_MAP(m_NpcMod);		
	DELETE_ALL_PTR_MEMBER_MAP(m_NpcTex);	
	DELETE_ALL_PTR_MEMBER_MAP(m_NpcAni);	
	DELETE_ALL_PTR_MEMBER_MAP(m_NpcItm);
	
#else
	
	SAFE_DELETE_ARRAY(m_pNpcFileInfo);	
	SAFE_DELETE_ARRAY(theNpcMod);
	SAFE_DELETE_ARRAY(theNpcTex);
	SAFE_DELETE_ARRAY(theNpcAni);
	SAFE_DELETE_ARRAY(theNpcItm);
	
#endif
}

bool Player_Manager::MapChangePC( char* file_pos )
{
	int i = 0;
	int nation_type = GetMapNationType( g_Map.m_NowMapID ); 
	if( theNationType == nation_type )
	{
		if( g_Map.m_NowMapID == c_RockMap_Login_Field ) 
			theNationType = -1;
		
		return true;
	}
	else
		theNationType = nation_type;
	
	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	
	///-- theNationType�� ���� n_Nation_Common�� ����
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LoadItemTable( file_pos );
	//	LoadSkillTable( file_pos );
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	if( l_nDevSkip  )
	{
		LoadItemTable( file_pos );
	}
	else
	{
		LoadItemTableVF();
		//		LoadSkillTableVF();
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	///--PRINT_DLG_LOG( "Start CreatePC()" );
	
	if( CreatePC(file_pos) == false )
	{
		return false;
	}
	
	///SAFE_DELETE_ARRAY(m_pPcFileInfo);
	
	///--PRINT_DLG_LOG( "End CreatePC()" );
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	WIN32_FIND_DATA fileFindData;
	HANDLE hFileFind;
	BOOL bFind;
	char	file_path[256]	= {0,};
	
	char buf[256] = {0,};
	char* c;
	int temp;
	
	//-----------------------------------------------------------------------------
	// ĳ�������������б�
	// NPC�� ���� ��Ʈ������ ���� ���������� �ϵ��� ���� �����Ѵ�
	//-----------------------------------------------------------------------------
	m_pPcFileInfo = SAFE_NEW_ARRAY( CHR_FILE_INFO , c_CharType_Count*2 );	
	bFind = TRUE;
	temp = 0;
	
	// PC
	sprintf( file_path, "%s\\Chr\\*.chr", file_pos );
	hFileFind = FindFirstFile( file_path, &fileFindData);
	while( ( hFileFind != INVALID_HANDLE_VALUE ) && bFind ) 
	{
		if( !(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			char race_type[2] = {0,};
			race_type[0] = fileFindData.cFileName[1];
			int race = atoi( race_type );
			if( ( theNationType == n_Nation_Common ) || ( theNationType == race ) || ( n_Nation_Common == race ) )
			{
				sprintf( file_path, "%s\\Chr\\%s", file_pos, fileFindData.cFileName );
				if( m_pPcFileInfo[temp].FileRead( file_path, SORT_PC ) == true )
				{		
					strcpy( buf, fileFindData.cFileName);
					c = strchr( buf, '.' );	/// ���ϸ��� Ȯ���� ���� ���� �κи� �߷����ϴ�. 
					*c = NULL;
					m_pPcFileInfo[temp].m_Index = atoi( buf ) ;				
					temp++;		
				}
				else
				{
					return false;
				}
			}
		}
		bFind = FindNextFile( hFileFind, &fileFindData );
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	char buf[256] = {0,};
	int temp;
	
	VFileHandleList* pFHL;
	VFileHandleList_i it;
	
	
	//-----------------------------------------------------------------------------
	// ĳ������������(brv_chr)�б�
	// NPC�� ���� ��Ʈ������ ���� ���������� �ϵ��� ���� �����Ѵ�
	//-----------------------------------------------------------------------------
	//ProfileBegin( "g_VFChrRoot" );
	m_pPcFileInfo = SAFE_NEW_ARRAY( CHR_FILE_INFO , c_CharType_Count*2 );	
	temp = 0;	
	pFHL = g_VFChrRoot.GetFileHandleList();
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			GetFileExt( (*it)->GetFileContext()->strName, buf );
			if( lstrcmp( buf, "chr" ) == 0 )	/// Ȯ���ڰ� chr ������ �Ѹ� �д´� 
			{
				char race_type[2] = {0,};
				race_type[0] = (*it)->GetFileContext()->strName[1];
				int race = atoi( race_type );
				if( ( theNationType == n_Nation_Common ) || ( theNationType == race ) || ( n_Nation_Common == race ) )
				{
					if( m_pPcFileInfo[temp].FileReadVF( &g_VFChrRoot, (*it)->GetFileContext()->strName, SORT_PC ) )
					{	
						GetFileNameOnly( (*it)->GetFileContext()->strName, buf );
						m_pPcFileInfo[temp].m_Index = atoi( buf ) ;						
					}
					else
					{
						return false;
					}
				}
				++temp;
			}
		}
	}
	//ProfileEnd( "g_VFChrRoot" );
	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	for( i = 0; i < c_Chr_Mod_Count; ++i )
	{
		if( i < m_mod_count )
		{
			m_pMod[i] = &theChrMod[i];
		}
		else
		{
			m_pMod[i] = NULL;
		}
	}
	
	for( i = 0; i < c_Chr_Tex_Count; ++i )
	{
		if( i < m_tex_count )
		{
			m_pTex[i] = &theChrTex[i];
		}
		else
		{
			m_pTex[i] = NULL;
		}
	}
	
	for( i = 0; i < c_Chr_Ani_Count; ++i )
	{
		if( i < m_ani_count )
		{
			m_pAni[i] = &theChrAni[i];
		}
		else
		{
			m_pAni[i] = NULL;
		}
	}
	
	for( i = 0; i < c_Chr_Itm_Count; ++i )
	{
		if( i < m_itm_count )
		{
			m_pItm[i] = &theChrItm[i];
		}
		else
		{
			m_pItm[i] = NULL;
		}
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// Desc: �� Warp �� NPC �ٽ� ���� �ϱ� 
//-----------------------------------------------------------------------------
bool Player_Manager::MapChangeNPC( char* file_pos )
{
	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LoadNPCTable( file_pos );
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	if( l_nDevSkip )
	{
		LoadNPCTable( file_pos );
	}
	else
	{
		LoadNPCTableVF();
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	if( CreateNPC(file_pos) == false )
	{
		return false;
	}
	
	SetNpcDataArray();
	
	return true;
}

void Player_Manager::SetNpcDataArray()
{
	int	i = 0;
	int	cnt = 0;
	
	//	for( i = c_Chr_Mod_Count, cnt = 0; i < c_Chr_Mod_Count + the_sNpc_SetData.total_npc_num; ++i, ++cnt )
	for( i = c_Chr_Mod_Count, cnt = 0; i < c_Chr_Mod_Count + c_Npc_Mod_Count; ++i, ++cnt )
	{
		if( i < c_Chr_Mod_Count + the_sNpc_SetData.total_msh_num )
		{
			
#ifdef DYNAMIC_RESOURCE_LOADING
			///--m_pMod[i] = (Model *)theNpcMod[cnt];
			
#else			
			m_pMod[i] = &theNpcMod[cnt];
#endif
			
		}
		else
		{
			m_pMod[i] = NULL;
		}
	}
	//	for( i = c_Chr_Tex_Count, cnt = 0; i < c_Chr_Tex_Count + the_sNpc_SetData.total_npc_num; ++i, ++cnt )
	for( i = c_Chr_Tex_Count, cnt = 0; i < c_Chr_Tex_Count + c_Npc_Tex_Count; ++i, ++cnt )
	{
		if( i < c_Chr_Tex_Count + the_sNpc_SetData.total_tex_num )
		{
#ifdef DYNAMIC_RESOURCE_LOADING			
			///--m_pTex[i] = (Texture *)theNpcTex[cnt];
#else
			m_pTex[i] = &theNpcTex[cnt];
#endif
		}
		else
		{
			m_pTex[i] = NULL;
		}
	}
	//	for( i = c_Chr_Ani_Count, cnt = 0; i < c_Chr_Ani_Count + the_sNpc_SetData.total_npc_num * c_Npc_AniIndex_Count; ++i, ++cnt )
	for( i = c_Chr_Ani_Count, cnt = 0; i < c_Chr_Ani_Count + c_Npc_Ani_Count; ++i, ++cnt )
	{
		if( i < c_Chr_Ani_Count + the_sNpc_SetData.total_npc_num * c_Npc_AniIndex_Count )
		{
#ifdef DYNAMIC_RESOURCE_LOADING				
			///--m_pAni[i] = (Bone *)theNpcAni[cnt];
#else
			m_pAni[i] = &theNpcAni[cnt];
#endif
		}
		else
		{
			m_pAni[i] = NULL;
		}
	}
	//	for( i = c_Chr_Itm_Count, cnt = 0; i < c_Chr_Itm_Count + the_sNpc_SetData.total_npc_num; ++i, ++cnt )
	for( i = c_Chr_Itm_Count, cnt = 0; i < c_Chr_Itm_Count + c_Npc_Itm_Count; ++i, ++cnt )
	{
		if( i < c_Chr_Itm_Count + the_sNpc_SetData.total_itm_num )
		{
#ifdef DYNAMIC_RESOURCE_LOADING			
			///--m_pItm[i] = (Character_Item *)theNpcItm[cnt];
#else
			m_pItm[i] = &theNpcItm[cnt];
#endif
		}
		else
		{
			m_pItm[i] = NULL;
		}
	}	
	
}

//-----------------------------------------------------------------------------
// Desc: ������Ʈ�� ���õ� ���� �޸𸮿� �Ҵ�(��,�ؽ���,�ִϸ��̼�)
//-----------------------------------------------------------------------------
bool Player_Manager::Create( char* file_pos )
{
	//ProfileBegin( "Player_Manager::Create" );
	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	LoadItemTable( file_pos );
	LoadNPCTable( file_pos );
	LoadSkillTable( file_pos );
	LoadStatTable( file_pos );
	LoadSubStatTable( file_pos );
	
	LoadTableInfo( file_pos );
	LoadStoreTable( file_pos );
	LoadQuestTable( file_pos );
	LoadQuestSignalTable( file_pos );	
	ReadScriptStrTable( file_pos );
	LoadSkillLessonTable( file_pos );
	LoadAddEffectInfoTable( file_pos );
	LoadItemAddEffectInfoTable( file_pos );	
	
	ReadLightSet( file_pos );
	
	LoadFightMapInfoTable( file_pos );
	ReadGameModeDesc( file_pos );	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	if( l_nDevSkip )
	{
		LoadItemTable( file_pos );
		LoadNPCTable( file_pos );
		LoadSkillTable( file_pos );
		ReadScriptStrTable( file_pos );
		LoadQuestTable( file_pos );
		ReadAutoHelpTip( file_pos );
	}
	else
	{		
		//ProfileBegin( "LoadItemTableVF" );
		LoadItemTableVF();
		//ProfileEnd( "LoadItemTableVF" );
		
		//ProfileBegin( "LoadNPCTableVF" );
		LoadNPCTableVF();
		//ProfileEnd( "LoadNPCTableVF" );
		
		//ProfileBegin( "LoadSkillTableVF" );
		LoadSkillTableVF();	
		//ProfileEnd( "LoadSkillTableVF" );
		
		//ProfileBegin( "ReadScriptStrTableVF" );
		ReadScriptStrTableVF();
		//ProfileEnd( "ReadScriptStrTableVF" );		
		
		//ProfileBegin( "LoadQuestTableVF" );
		LoadQuestTableVF();
		//ProfileEnd( "LoadQuestTableVF" );
		 
		
		//ProfileBegin( "ReadAutoHelpTipVF" );
#ifdef UNICODE_ROCK
		ReadAutoHelpTipVFW();
#else
		ReadAutoHelpTipVF();
#endif
	
		//ProfileEnd( "ReadAutoHelpTipVF" );		
	}	
	
	//ProfileBegin( "LoadStatTableVF" );
	LoadStatTableVF();
	LoadSubStatTableVF();
	
	//ProfileEnd( "LoadStatTableVF" );
	
	//ProfileBegin( "LoadTableInfoVF" );
	LoadTableInfoVF();
	//ProfileEnd( "LoadTableInfoVF" );
	
	//ProfileBegin( "LoadStoreTableVF" );
	LoadStoreTableVF();
	//ProfileEnd( "LoadStoreTableVF" );
	
	
	//ProfileBegin( "LoadQuestSignalTableVF" );
	LoadQuestSignalTableVF();		
	//ProfileEnd( "LoadQuestSignalTableVF" );
	
	
	LoadSkillLessonTableVF(); ///-- Open ���� �߰�
	
	//ProfileBegin( "LoadAddEffectInfoTableVF" );
	LoadAddEffectInfoTableVF();
	//ProfileEnd( "LoadAddEffectInfoTableVF" );
	
	LoadItemAddEffectInfoTableVF();
	
	//ProfileBegin( "ReadLightSetVF" );
	ReadLightSetVF();
	//ProfileEnd( "ReadLightSetVF" );
	
	//ProfileBegin( "LoadFightMapInfoTableVF" );
	LoadFightMapInfoTableVF();
	//ProfileEnd( "LoadFightMapInfoTableVF" );
	
	//ProfileBegin( "ReadGameModeDescVF" );
#ifdef UNICODE_ROCK
	ReadGameModeDescVFW();
#else
	ReadGameModeDescVF();
#endif
	//ProfileEnd( "ReadGameModeDescVF" );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ProfileBegin( "m_NPCImpactTable.Load" );
	m_NPCImpactTable.Load();
	//ProfileEnd( "m_NPCImpactTable.Load" );
	
	//ProfileBegin( "m_HolyManger.LoadTable" );
	g_Pc_Manager.m_HolyManager.LoadTable();
	//ProfileEnd( "m_HolyManger.LoadTable" );
	
	LoadEtcFileVF();




	//ProfileBegin( "CreatePC(file_pos)" );
	if( CreatePC(file_pos) == false )
	{
		//ProfileEnd( "CreatePC(file_pos)" );
		return false;
	}
	//ProfileEnd( "CreatePC(file_pos)" );
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	WIN32_FIND_DATA fileFindData;
	HANDLE hFileFind;
	BOOL bFind;
	
	char buf[256] = {0,};
	char* c;
	int temp;
	int i;
	
	//-----------------------------------------------------------------------------
	// ĳ�������������б�
	// NPC�� ���� ��Ʈ������ ���� ���������� �ϵ��� ���� �����Ѵ�
	//-----------------------------------------------------------------------------
	m_pPcFileInfo = SAFE_NEW_ARRAY( CHR_FILE_INFO , c_CharType_Count*2 );	
	bFind = TRUE;
	temp = 0;
	
	// PC
	sprintf( file_path, "%s\\Chr\\*.chr", file_pos );
	hFileFind = FindFirstFile( file_path, &fileFindData);
	while( ( hFileFind != INVALID_HANDLE_VALUE ) && bFind ) 
	{
		if( !(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			sprintf( file_path, "%s\\Chr\\%s", file_pos, fileFindData.cFileName );
			if( m_pPcFileInfo[temp].FileRead( file_path, SORT_PC ) == true )
			{		
				strcpy( buf, fileFindData.cFileName);
				c = strchr( buf, '.' );	/// ���ϸ��� Ȯ���� ���� ���� �κи� �߷����ϴ�. 
				*c = NULL;
				m_pPcFileInfo[temp].m_Index = atoi( buf ) ;				
				temp++;					
			}
			else
			{
				return false;
			}
		}
		bFind = FindNextFile( hFileFind, &fileFindData );
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	char buf[256] = {0,};
	int temp;
	int i;
	
	VFileHandleList* pFHL;
	VFileHandleList_i it;
	
	
	//-----------------------------------------------------------------------------
	// ĳ������������(brv_chr)�б�
	// NPC�� ���� ��Ʈ������ ���� ���������� �ϵ��� ���� �����Ѵ�
	//-----------------------------------------------------------------------------
	//ProfileBegin( "g_VFChrRoot" );
	m_pPcFileInfo = SAFE_NEW_ARRAY( CHR_FILE_INFO , c_CharType_Count*2 );	
	temp = 0;	
	pFHL = g_VFChrRoot.GetFileHandleList();
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			GetFileExt( (*it)->GetFileContext()->strName, buf );
			if( lstrcmp( buf, "chr" ) == 0 )	/// Ȯ���ڰ� chr ������ �Ѹ� �д´� 
			{
				if( m_pPcFileInfo[temp].FileReadVF( &g_VFChrRoot, (*it)->GetFileContext()->strName, SORT_PC ) )
				{	
					GetFileNameOnly( (*it)->GetFileContext()->strName, buf );
					m_pPcFileInfo[temp].m_Index = atoi( buf ) ;
					++temp;
				}
				else
				{
					return false;
				}
			}
		}
	}
	//ProfileEnd( "g_VFChrRoot" );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	//-----------------------------------------------------------------------------
	// ȭ�쿡 �׷����� ���� ����
	//-----------------------------------------------------------------------------
	//	ProfileBegin( "CreateArrowLine" );
	//	CreateArrowLine( file_pos );
	//	ProfileEnd( "CreateArrowLine" );	

	//-----------------------------------------------------------------------------
	// �׸��ڹ��ۻ���
	//-----------------------------------------------------------------------------	
	
	// ���� �ؽ�ó �ε� by wxywxy
	//m_Mantle.CreateTexture();
	
	//	CreateNPC(file_pos);
	
	int	cnt = 0;
	
	for( i = 0; i < m_mod_count; ++i )
	{
		m_pMod[i] = &theChrMod[i];
	}
	for( i = c_Chr_Mod_Count, cnt = 0; i < c_Chr_Mod_Count + the_sNpc_SetData.total_npc_num; ++i, ++cnt )
	{
#ifdef DYNAMIC_RESOURCE_LOADING
	    ///--m_pMod[i] = (Model *)theNpcMod[cnt];
#else
		m_pMod[i] = &theNpcMod[cnt];
#endif
	}
	
	for( i = 0; i < m_tex_count; ++i )
	{
		m_pTex[i] = &theChrTex[i];
	}
	for( i = c_Chr_Tex_Count, cnt = 0; i < c_Chr_Tex_Count + the_sNpc_SetData.total_npc_num; ++i, ++cnt )
	{
#ifdef DYNAMIC_RESOURCE_LOADING	
		///--m_pTex[i] = (Texture *)theNpcTex[cnt];
#else
		m_pTex[i] = &theNpcTex[cnt];
#endif
	}
	
	for( i = 0; i < m_ani_count; ++i )
	{
		m_pAni[i] = &theChrAni[i];
	}
	for( i = c_Chr_Ani_Count, cnt = 0; i < c_Chr_Ani_Count + the_sNpc_SetData.total_npc_num * c_Npc_AniIndex_Count; ++i, ++cnt )
	{
#ifdef DYNAMIC_RESOURCE_LOADING			
		///--m_pAni[i] =  (Bone *)theNpcAni[cnt];
#else
		m_pAni[i] = &theNpcAni[cnt];
#endif
	}
	
	for( i = 0; i < m_itm_count; ++i )
	{
		m_pItm[i] = &theChrItm[i];
	}
	for( i = c_Chr_Itm_Count, cnt = 0; i < c_Chr_Itm_Count + the_sNpc_SetData.total_npc_num; ++i, ++cnt )
	{
#ifdef DYNAMIC_RESOURCE_LOADING
		///--m_pItm[i] = (Character_Item *)theNpcItm[cnt];
#else
		m_pItm[i] = &theNpcItm[cnt];
#endif
	}
	
	//ProfileEnd( "Player_Manager::Create" );
	
	return true;
}


bool Player_Manager::CreatePC( char* file_pos )
{
	///-- ���Ⱑ �ε��Ҷ� �ð��� ���� ���� �ɸ��� ��ƾ....;;;
	///--PRINT_DLG_LOG( "Player_Manager::CreatePC");
	
	//ProfileBegin( "< Player_Manager::CreatePC >" );
	int		all_count		= 0;
	char	file_path[256]	= {0,};			
	
	DWORD	tick = timeGetTime();
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	WIN32_FIND_DATA fileFindData;
	HANDLE hFileFind;
	BOOL bFind;
	
	char buf[256] = {0,};
	char* c;
	int temp;
	int i;
	
	//---------------------------------------------------------------
	// �� ���� �޸𸮿� �ø��� by niceguy
	//---------------------------------------------------------------
	theChrMod = SAFE_NEW_ARRAY( Model , c_Chr_Mod_Count );	
	memset(theChrMod, 0, sizeof(Model) * c_Chr_Mod_Count);
	bFind = TRUE;
	temp = 0;
	
	sprintf( file_path, "%s\\Chr\\Msh\\*.msh", file_pos );
	hFileFind = FindFirstFile( file_path, &fileFindData);
	while( ( hFileFind != INVALID_HANDLE_VALUE ) && bFind ) 
	{
		if( !(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			char race_type[2] = {0,};
			race_type[0] = fileFindData.cFileName[1];
			int race = atoi( race_type );
			if( ( theNationType == n_Nation_Common ) || ( theNationType == race ) || ( n_Nation_Common == race ) )
			{
				sprintf( file_path, "%s\\Chr\\Msh\\%s", file_pos, fileFindData.cFileName );
				if( theChrMod[temp].Read( file_path, true ) == true )
				{		
					sstrcpy( buf, fileFindData.cFileName);
					c = strchr( buf, '.' );	/// ���ϸ��� Ȯ���� ���� ���� �κи� �߷����ϴ�. 
					*c = NULL;
					theChrMod[temp].m_Index = atoi( buf );				
					theChrMod[temp].m_race = race;				
					temp++;
				}
				else
				{
					return false;
				}
			}
		}
		bFind = FindNextFile( hFileFind, &fileFindData );
	}
	
	m_mod_count = temp;
	
	if( m_mod_count >= c_Chr_Mod_Count )
	{
		MessageBox( NULL, "m_mod_count >= c_Chr_Mod_Count", "Error", MB_OK );
	}
	
	//---------------------------------------------------------------
	// �ؽ��� ���� �޸𸮿� �ø��� by niceguy
	//---------------------------------------------------------------
	theChrTex = SAFE_NEW_ARRAY( Texture , c_Chr_Tex_Count );	
	memset(theChrTex, 0, sizeof(Texture) * c_Chr_Tex_Count);
	bFind = TRUE;
	temp = 0;
	
	//	sprintf( file_path, "%s\\Chr\\Tex\\*.tex", file_pos );
	sprintf( file_path, "%s\\Chr\\Tex\\*.dds", file_pos );
	hFileFind = FindFirstFile( file_path, &fileFindData);
	while( ( hFileFind != INVALID_HANDLE_VALUE ) && bFind ) 
	{
		if( !(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			sprintf( file_path, "%s\\Chr\\Tex\\%s", file_pos, fileFindData.cFileName );
			//			if( theChrTex[temp].Read( file_path, n_Texture ) == true )
			{		
				strcpy( buf, fileFindData.cFileName);
				c = strchr( buf, '.' );	/// ���ϸ��� Ȯ���� ���� ���� �κи� �߷����ϴ�. 
				*c = NULL;
				theChrTex[temp].m_Index = atoi( buf ) ;	
				//				theChrTex[temp].m_byAttr = DATA_NON_DELETE;
				theChrTex[temp].m_byAttr = DATA_DELETE0;
				temp++;
			}
			//			else
			//			{
			//				return false;
			//			}
		}
		bFind = FindNextFile( hFileFind, &fileFindData );
	}
	m_tex_count = temp;
	
	if( m_tex_count >= c_Chr_Tex_Count )
	{
		MessageBox( NULL, "m_tex_count >= c_Chr_Tex_Count", "Error", MB_OK );
	}
	
	// �ؽ��� �Ӽ����� �б�
	//	Set__TextureAttr( file_pos );
	
	//---------------------------------------------------------------
	// �ִϸ��̼� ���� �޸𸮿� �ø��� by niceguy
	//---------------------------------------------------------------
	theChrAni = SAFE_NEW_ARRAY( Bone , c_Chr_Ani_Count );	
	memset(theChrAni, 0, sizeof(Bone) * c_Chr_Ani_Count);
	bFind = TRUE;
	temp = 0;
	
	bool lb_IsLoading = true;
	int li_FileIndex = 0;
	
	// PC
	sprintf( file_path, "%s\\Chr\\Ani\\*.ani", file_pos );
	hFileFind = FindFirstFile( file_path, &fileFindData);
	while( ( hFileFind != INVALID_HANDLE_VALUE ) && bFind ) 
	{
		if( !(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			char race_type[2] = {0,};
			race_type[0] = fileFindData.cFileName[1];
			int race = atoi( race_type );
			if( ( theNationType == n_Nation_Common ) || ( theNationType == race ) || ( n_Nation_Common == race ) )
			{
				sprintf( file_path, "%s\\Chr\\Ani\\%s", file_pos, fileFindData.cFileName );
				
				strcpy( buf, fileFindData.cFileName);
				li_FileIndex = atoi( buf );	
				
				// ù �ε��̳� �α��� �ʿ����� ���� �ε� 
				if( g_RockClient.GetGameMode() == GMODE_INITLOAD || g_Map.m_NowMapID == c_RockMap_Login_Field )
				{
					lb_IsLoading = CheckLoadAniList_LoginFeild( li_FileIndex );					
				}
				
				if( lb_IsLoading )
				{
					if( theChrAni[temp].Read( file_path ) == true )
					{		
						c = strchr( buf, '.' );	/// ���ϸ��� Ȯ���� ���� ���� �κи� �߷����ϴ�. 
						*c = NULL;
						theChrAni[temp].m_Index = li_FileIndex; // atoi( buf );				
						theChrAni[temp].m_race = race;
						temp++;
					} 
					else
					{
						return false;
					}
				}
			}
		}
		bFind = FindNextFile( hFileFind, &fileFindData );
	}
	m_ani_count = temp;
	
	if( m_ani_count >= c_Chr_Ani_Count )
	{
		MessageBox( NULL, "m_ani_count >= c_Chr_Ani_Count", "Error", MB_OK );
	}
	
	//---------------------------------------------------------------
	// ������ ���� �޸𸮿� �ø��� by niceguy
	//---------------------------------------------------------------
	theChrItm = SAFE_NEW_ARRAY( Character_Item , c_Chr_Itm_Count );
	memset(theChrItm, 0, sizeof(Character_Item) * c_Chr_Itm_Count);
	bFind = TRUE;
	temp = 0;
	
	// PC
	sprintf( file_path, "%s\\Chr\\Itm\\*.itm", file_pos );
	hFileFind = FindFirstFile( file_path, &fileFindData);
	while( ( hFileFind != INVALID_HANDLE_VALUE ) && bFind ) 
	{
		if( !(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			char race_type[2] = {0,};
			race_type[0] = fileFindData.cFileName[1];
			int race = atoi( race_type );
			if( ( theNationType == n_Nation_Common ) || ( theNationType == race ) || ( n_Nation_Common == race ) )
			{
				sprintf( file_path, "%s\\Chr\\Itm\\%s", file_pos, fileFindData.cFileName );
				if( theChrItm[temp].File__Open( file_path ) == true )
				{
					theChrItm[temp].CreateVrxBuf(SORT_PC, true);
					strcpy( buf, fileFindData.cFileName);
					c = strchr( buf, '.' );	/// ���ϸ��� Ȯ���� ���� ���� �κи� �߷����ϴ�. 
					*c = NULL;
					theChrItm[temp].m_Index = atoi( buf ) ;			
					theChrItm[temp].m_race = race;			
					SetItemFileCode12( temp, theChrItm[temp].m_Index );
					temp++;
				}
				else
				{
					return false;
				}
			}
		}
		bFind = FindNextFile( hFileFind, &fileFindData );
	}
	m_itm_count = temp;
	
	if( m_itm_count >= c_Chr_Itm_Count )
	{
		MessageBox( NULL, "m_itm_count >= c_Chr_Itm_Count", "Error", MB_OK );
	}
	
	// ������ �Ӽ� �б�
	Set__ItemAttr( file_pos );
	///-- �� ��ƾ�� ������� ����
	///-Set__ItemChangeID();	
	
	//-----------------------------------------------------------------------------
	// ���� �Ӽ� �б�
	//-----------------------------------------------------------------------------
	Set__PcAttr( file_pos );
	
	//-----------------------------------------------------------------------------
	// ���� �ִϸ��̼� �б�
	//-----------------------------------------------------------------------------
	for( i = 0; i < c_CharType_Count; ++i )
	{
		sprintf( file_path, "%s\\chr\\ANI_%d.dat", file_pos, i );
		Set__PcAnimationList( file_path, i );
	}	
	
	FindClose( hFileFind );
	//////////////////////////////////////////////////////////////////////////////////////////////////
#else ////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	///			read from virtual filesystem	/// sooree pack
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	char buf[256] = {0,};
	int temp;
	int i;
	
	VFileHandleList* pFHL;
	VFileHandleList_i it;
	
	//---------------------------------------------------------------
	// �� ���� �޸𸮿� �ø��� by niceguy
	//---------------------------------------------------------------
	//ProfileBegin( "g_VFChrMsh" );
	theChrMod = SAFE_NEW_ARRAY( Model , c_Chr_Mod_Count );	
	memset(theChrMod, 0, sizeof(Model) * c_Chr_Mod_Count);
	temp = 0;
	//	g_VFChrMsh.ChangeDir( "/Msh" );
	pFHL = g_VFChrMsh.GetFileHandleList();
	
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			char race_type[2] = {0,};
			race_type[0] = (*it)->GetFileContext()->strName[1];
			int race = atoi( race_type );
			
			if( ( theNationType == n_Nation_Common ) || ( theNationType == race ) || ( n_Nation_Common == race ) )
			{				
				///-- �޽����� ���� �ε�
				if( theChrMod[temp].ReadVF( &g_VFChrMsh, (*it)->GetFileContext()->strName, true ) )
				{	
					GetFileNameOnly( (*it)->GetFileContext()->strName, buf );
					theChrMod[temp].m_Index = atoi( buf );				
					theChrMod[temp].m_race = race;
					
					++temp;
				}
				else
				{
					return false;
				}
			}
		}
	}
	
	m_mod_count = temp;
	
	//ProfileEnd( "g_VFChrMsh" );
	
	if( m_mod_count >= c_Chr_Mod_Count )
	{
		MessageBox( NULL, "m_mod_count >= c_Chr_Mod_Count", "Error", MB_OK );
	}	
	
	//---------------------------------------------------------------
	// �ؽ��� ���� �޸𸮿� �ø��� by niceguy
	//---------------------------------------------------------------
	//ProfileBegin( "g_VFChrTex" );
	theChrTex = SAFE_NEW_ARRAY( Texture , c_Chr_Tex_Count );	
	memset(theChrTex, 0, sizeof(Texture) * c_Chr_Tex_Count);
	temp = 0;
	//	g_VFChrTex.ChangeDir( "/Tex" );
	pFHL = g_VFChrTex.GetFileHandleList();
	
	//-- DevSkip_Lyul
	int l_nDevSkip = GetPrivateProfileInt( "Texture", "Character_Skip", 0, "./dev.ini");
	
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			//			if( theChrTex[temp].ReadVF( &g_VFChrTex, (*it)->GetFileContext()->strName, n_Texture, l_nDevSkip ) == true )
			{	
				GetFileNameOnly( (*it)->GetFileContext()->strName, buf );
				theChrTex[temp].m_Index = atoi( buf ) ;	
				//				theChrTex[temp].m_byAttr = DATA_NON_DELETE;
				theChrTex[temp].m_byAttr = DATA_DELETE0;
				++temp;
			}
			//			else
			//			{
			//				return false;
			//			}
		}
	}
	m_tex_count = temp;
	
	// �ؽ��� �Ӽ����� �б�
	//	Set__TextureAttr( file_pos );
	
	//ProfileEnd( "g_VFChrTex" );
	
	if( m_tex_count >= c_Chr_Tex_Count )
	{
		MessageBox( NULL, "m_tex_count >= c_Chr_Tex_Count", "Error", MB_OK );
	}
	
	//---------------------------------------------------------------
	// �ִϸ��̼� ���� �޸𸮿� �ø��� by niceguy
	//---------------------------------------------------------------
	//ProfileBegin( "g_VFChrAni" );	
	theChrAni = SAFE_NEW_ARRAY( Bone , c_Chr_Ani_Count );	
	memset(theChrAni, 0, sizeof(Bone) * c_Chr_Ani_Count);
	
	//	g_VFChrAni.ChangeDir( "/Ani" );
	
	bool lb_IsLoading = true;
	int li_FileIndex = 0;
	
	temp = 0;
	
	pFHL = g_VFChrAni.GetFileHandleList();
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			char race_type[2] = {0,};
			race_type[0] = (*it)->GetFileContext()->strName[1];
			int race = atoi( race_type );
			
			// ������ �ε� 
			if( ( theNationType == n_Nation_Common ) || ( theNationType == race ) || ( n_Nation_Common == race ) )
			{
				GetFileNameOnly( (*it)->GetFileContext()->strName, buf );
				li_FileIndex = atoi( buf );	
				
				// ù �ε��̳� �α��� �ʿ����� ���� �ε� 
				if( g_RockClient.GetGameMode() == GMODE_INITLOAD || g_Map.m_NowMapID == c_RockMap_Login_Field )
				{
					lb_IsLoading = CheckLoadAniList_LoginFeild( li_FileIndex );					
				}
				
				if( lb_IsLoading )
				{
					if( theChrAni[temp].ReadVF( &g_VFChrAni, (*it)->GetFileContext()->strName ) )
					{	
						//						GetFileNameOnly( (*it)->GetFileContext()->strName, buf );
						theChrAni[temp].m_Index = li_FileIndex; //atoi( buf );	
						theChrAni[temp].m_race = race;
						++temp;						
					}
					else
					{
						return false;
					}
				}
			}
		}
	}
	m_ani_count = temp;
	
	//ProfileEnd( "g_VFChrAni" );
	
	if( m_ani_count >= c_Chr_Ani_Count )
	{
		MessageBox( NULL, "m_ani_count >= c_Chr_Ani_Count", "Error", MB_OK );
	}	
	
	//---------------------------------------------------------------
	// ������ ���� �޸𸮿� �ø��� by niceguy
	//---------------------------------------------------------------
	//ProfileBegin( "g_VFChrItm" );
	theChrItm = SAFE_NEW_ARRAY( Character_Item , c_Chr_Itm_Count );	
	memset(theChrItm, 0, sizeof(Character_Item) * c_Chr_Itm_Count);
	temp = 0;
	//	g_VFChrItm.ChangeDir( "/Itm" );
	pFHL = g_VFChrItm.GetFileHandleList();
	for( it = pFHL->begin(); it != pFHL->end(); ++it )
	{
		if( (*it)->GetFileContext()->iType == FT_FILE )
		{
			char race_type[2] = {0,};
			race_type[0] = (*it)->GetFileContext()->strName[1];
			int race = atoi( race_type );
			if( ( theNationType == n_Nation_Common ) || ( theNationType == race ) || ( n_Nation_Common == race ) )
			{
				if( theChrItm[temp].File__OpenVF( &g_VFChrItm, (*it)->GetFileContext()->strName ) )
				{	
					char *cName = (*it)->GetFileContext()->strName;				
					
					theChrItm[temp].CreateVrxBuf( SORT_PC, true );
					GetFileNameOnly( (*it)->GetFileContext()->strName, buf );
					theChrItm[temp].m_Index = atoi( buf );			
					theChrItm[temp].m_race = race;			
					SetItemFileCode12( temp, theChrItm[temp].m_Index );
					++temp;
				}
				else
				{
					return false;
				}
			}
		}
	}
	m_itm_count = temp;
	//ProfileEnd( "g_VFChrItm" );
	
	if( m_itm_count >= c_Chr_Itm_Count )
	{
		MessageBox( NULL, "m_itm_count >= c_Chr_Itm_Count", "Error", MB_OK );
	}
	
	//	g_VFChrRoot.ChangeDir( "/" );
	
	//ProfileBegin( "Set__ItemAttrVF" );
	Set__ItemAttrVF();
	//ProfileEnd( "Set__ItemAttrVF" );
	
	///-- �̷�ƾ�� ������� ����
	///--ProfileBegin( "Set__ItemChangeID" );
	///--Set__ItemChangeID();
	///--ProfileEnd( "Set__ItemChangeID" );
	
	//ProfileBegin( "Set__PcAttrVF" );
	Set__PcAttrVF();
	//ProfileEnd( "Set__PcAttrVF" );
	
	//ProfileBegin( "Set__PcAnimationListVF(for)" );
	for( i = 0; i < c_CharType_Count; ++i )
	{
		sprintf( file_path, "ANI_%d.dat", i );
		Set__PcAnimationListVF( file_path, i );
	}	
	//ProfileEnd( "Set__PcAnimationListVF(for)" );	
	
	///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ProfileEnd( "< Player_Manager::CreatePC >" );
	
	///--PRINT_DLG_LOG( "!!!!!!!!!!!!!!!!!!!!! char load : %u !!!!!!!!!!!!!!!!!", SAFE_TIME_SUB( timeGetTime() , tick ) );
	RLG1( "!!!!!!!!!!!!!!!!!!!!! char load : %u !!!!!!!!!!!!!!!!!", SAFE_TIME_SUB( timeGetTime() , tick ) );
	
	return true;
}



//-----------------------------------------------------------------------------
// Desc: ������Ʈ�� ���õ� ���� �޸𸮿� �Ҵ�(��,�ؽ���,�ִϸ��̼�)
//-----------------------------------------------------------------------------
bool Player_Manager::CreateNPC( char* file_pos )
{
	char		file_path[256]	= {0,};			
	int			i = 0;	
	int			idx = 0;	
	int			itm_cnt = 0;
	int			msh_cnt = 0;
	int			tex_cnt = 0;
	int			ani_cnt = 0;	
	
	
	DWORD	tick = timeGetTime();
	
	///////////////////////////////////////////////////////////////////////////////////////
	// �� ��ȣ(curMapIndex)�� �����Ͽ� NPC_(n).set �� �о���δ�.
	// the_sNpc_SetData �� �����Ͽ� �ʿ��� ���̴� ���ҽ��� ��� �ε��Ѵ�.
	//- NPC_(n).set
	//--- chr	
	//------ itm
	//--------- msh, tex 
	//------ ani  	
	//--- other attr
	///////////////////////////////////////////////////////////////////////////////////////
	
#if !defined( LOAD_VF )
	
	if( Set__NpcSet( file_pos, g_Map.m_NowMapID ) == false )
	{
		CloseWnd_Error( "!NPC_%d.set ������ ã�� �� �����ϴ�.", g_Map.m_NowMapID );			
		return false;
	}
	
	m_pNpcFileInfo = SAFE_NEW_ARRAY( CHR_FILE_INFO , c_NpcType_Count );	
	theNpcItm = SAFE_NEW_ARRAY( Character_Item , c_NpcType_Count );	
	theNpcMod = SAFE_NEW_ARRAY( Model , c_NpcType_Count );	
	theNpcTex = SAFE_NEW_ARRAY( Texture , c_NpcType_Count );	
	theNpcAni = SAFE_NEW_ARRAY( Bone , c_NpcType_Count * c_Npc_AniIndex_Count );		
	
	memset( m_pNpcFileInfo, 0, sizeof(CHR_FILE_INFO)*c_NpcType_Count );
	memset( theNpcItm, 0, sizeof(Character_Item)*c_NpcType_Count );	
	memset( theNpcMod, 0, sizeof(Model)*c_NpcType_Count );
	memset( theNpcTex, 0, sizeof(Texture)*c_NpcType_Count );
	memset( theNpcAni, 0, sizeof(Bone)*c_NpcType_Count*c_Npc_AniIndex_Count );
	
	for( i = 0; i < the_sNpc_SetData.total_npc_num; ++i )
	{
		sprintf( file_path, "%s\\Npc\\%d.chr", file_pos, the_sNpc_SetData.file_name[i] );
		if( m_pNpcFileInfo[i].FileRead( file_path, SORT_NPC ) == true )
		{
			m_pNpcFileInfo[i].m_Index = the_sNpc_SetData.file_name[i];				
			
			SetNpcFileCode( i, m_pNpcFileInfo[i].m_Index );	
			
			for( idx = 0; idx < m_pNpcFileInfo[i].nItmCount; ++idx )
			{
				sprintf( file_path, "%s\\Npc\\Itm\\%d.itm", file_pos, m_pNpcFileInfo[i].pItmIndex[idx] );
				
				if( theNpcItm[itm_cnt].File__Open( file_path ) == true )
				{					
					if( theNpcItm[itm_cnt].m_mod_index != -1 )
					{
						sprintf( file_path, "%s\\Npc\\Msh\\%d.msh", file_pos, theNpcItm[itm_cnt].m_mod_index );	
						if( theNpcMod[msh_cnt].Read( file_path, true ) == true ) 
						{
							theNpcMod[msh_cnt].m_Index = theNpcItm[itm_cnt].m_mod_index;			
							theNpcItm[itm_cnt].m_mod_index = msh_cnt + c_Chr_Mod_Count;
							++msh_cnt;
						}	
						else
						{
							return false;
						}
						
						//						sprintf( file_path, "%s\\Npc\\Tex\\%d.tex", file_pos, theNpcItm[itm_cnt].m_tex_index );	
						sprintf( file_path, "%s\\Npc\\Tex\\%d.dds", file_pos, theNpcItm[itm_cnt].m_tex_index );	
						//						if( theNpcTex[tex_cnt].Read( file_path, n_Texture ) == true ) 
						{
							theNpcTex[tex_cnt].m_Index = theNpcItm[itm_cnt].m_tex_index;			
							theNpcTex[tex_cnt].m_byAttr = DATA_DELETE0;			
							//							theNpcTex[tex_cnt].m_byAttr = DATA_NON_DELETE;
							theNpcItm[itm_cnt].m_tex_index = tex_cnt + c_Chr_Tex_Count;
							++tex_cnt;
						}	
						//						else
						//						{
						//							return false;
						//						}
					}
					
					theNpcItm[itm_cnt].CreateVrxBuf(SORT_NPC, true);
					theNpcItm[itm_cnt].m_Index = m_pNpcFileInfo[i].pItmIndex[idx];				
					m_pNpcFileInfo[i].pItmIndex[idx] = itm_cnt + c_Chr_Itm_Count;					
					++itm_cnt;
				}
				else
				{
					return false;
				}
			}
			
			for( idx = 0; idx < c_Npc_AniIndex_Count; ++idx )
			{
				if( m_pNpcFileInfo[i].pAniInfo[idx].ani_number != -1 )
				{
					sprintf( file_path, "%s\\Npc\\Ani\\%d.ani", file_pos, m_pNpcFileInfo[i].pAniInfo[idx].ani_number );	
					if( theNpcAni[ani_cnt].Read( file_path ) == true )
					{
						theNpcAni[ani_cnt].m_Index = m_pNpcFileInfo[i].pAniInfo[idx].ani_number;				
						m_pNpcFileInfo[i].pAniInfo[idx].ani_number = ani_cnt + c_Chr_Ani_Count;
						++ani_cnt;
					}
					else
					{
						return false;
					}					
				}
			}			
		}
		else
		{
			return false;
		}
	}
	
	the_sNpc_SetData.total_itm_num = itm_cnt;
	the_sNpc_SetData.total_msh_num = msh_cnt;
	the_sNpc_SetData.total_tex_num = tex_cnt;
	the_sNpc_SetData.total_ani_num = ani_cnt;
	
	//-----------------------------------------------------------------------------
	// NPC �Ӽ�, ��� , ����Ʈ ( ./NPC/NPC_SET.dat ) By wxywxy
	//-----------------------------------------------------------------------------
	//	Set__NpcAttr( file_pos, "\\Npc\\NPC_Set.Dat" );		
	
#else
	////////////////////////////////////////////////////////////////////////////////
	///			read from virtual filesystem	/// sooree pack
	////////////////////////////////////////////////////////////////////////////////
	
	if( Set__NpcSetVF( g_Map.m_NowMapID ) == false )
	{
		CloseWnd_Error( "!NPC_%d.set ������ ã�� �� �����ϴ�.", g_Map.m_NowMapID );			
		return false;
	}
	
	//-- DevSkip_Lyul
	int l_nDevSkip = GetPrivateProfileInt( "Texture", "NPC_Skip", 0, "./dev.ini");
	
#ifdef DYNAMIC_RESOURCE_LOADING
	
	for( i = 0; i < the_sNpc_SetData.total_npc_num; ++i )
	{
		sprintf( file_path, "%d.chr", the_sNpc_SetData.file_name[i] );	
		CHR_FILE_INFO * pNpcFileInfo = SAFE_NEW( CHR_FILE_INFO );		
		
		if( pNpcFileInfo->FileReadVF( &g_VFNpcRoot, file_path, SORT_NPC ) == true )
		{
			m_pNpcFileInfo.push_back( pNpcFileInfo );			
			pNpcFileInfo->m_Index = the_sNpc_SetData.file_name[i];
			SetNpcFileCode( i, pNpcFileInfo->m_Index );
			
			for( idx = 0; idx < pNpcFileInfo->nItmCount; ++idx )
			{
				sprintf( file_path, "%d.itm", pNpcFileInfo->pItmIndex[idx] );			
				Character_Item * pNpcItem = SAFE_NEW( Character_Item );				
				
				if( pNpcItem->File__OpenVF( &g_VFNpcItm, file_path ) == true )
				{
					theNpcItm.push_back( pNpcItem );
					
					if( pNpcItem->m_mod_index != -1 )
					{
						sprintf( file_path, "%d.msh", pNpcItem->m_mod_index );							
						Model * pNpcMod = SAFE_NEW( Model );						
						
						if( pNpcMod->ReadVF( &g_VFNpcMsh, file_path, true ) == true ) 
						{
							theNpcMod.push_back(pNpcMod);
							
							pNpcMod->m_Index = pNpcItem->m_mod_index;			
							pNpcItem->m_mod_index = msh_cnt + c_Chr_Mod_Count;
							++msh_cnt;
						}	
						else
						{
							SAFE_DELETE(pNpcMod);
							return false;
						}
						
						sprintf( file_path, "%d.dds", pNpcItem->m_tex_index );						
						Texture * pNpcTex = SAFE_NEW( Texture );						
						theNpcTex.push_back(pNpcTex);
						
						pNpcTex->m_Index = pNpcItem->m_tex_index;			
						pNpcTex->m_byAttr = DATA_DELETE0;			
						pNpcItem->m_tex_index = tex_cnt + c_Chr_Tex_Count;
						++tex_cnt;					
					}
					
					pNpcItem->CreateVrxBuf(SORT_NPC, true);
					pNpcItem->m_Index = pNpcFileInfo->pItmIndex[idx];				
					pNpcFileInfo->pItmIndex[idx] = itm_cnt + c_Chr_Itm_Count;					
					++itm_cnt;
					
				}
				else
				{
					SAFE_DELETE(pNpcItem);
					return false;
				}			
			}
			
			for( idx = 0; idx < c_Npc_AniIndex_Count; ++idx )
			{
				if( pNpcFileInfo->pAniInfo[idx].ani_number != -1 )
				{
					sprintf( file_path, "%d.ani", pNpcFileInfo->pAniInfo[idx].ani_number );	
					
					Bone * pNpcAni = SAFE_NEW( Bone );
					
					if( pNpcAni->ReadVF( &g_VFNpcAni, file_path ) == true )
					{
						theNpcAni.push_back( pNpcAni );
						
						pNpcAni->m_Index = pNpcFileInfo->pAniInfo[idx].ani_number;				
						pNpcFileInfo->pAniInfo[idx].ani_number = ani_cnt + c_Chr_Ani_Count;
						++ani_cnt;
					}
					else
					{
						SAFE_DELETE(pNpcAni);
						return false;
					}					
				}
			}
		}
		else
		{
			SAFE_DELETE(pNpcFileInfo);
			return false;
		}	
	}
	
#else
	
	m_pNpcFileInfo = SAFE_NEW_ARRAY( CHR_FILE_INFO , c_NpcType_Count );	
	theNpcItm = SAFE_NEW_ARRAY( Character_Item , c_NpcType_Count );		
	theNpcMod = SAFE_NEW_ARRAY( Model , c_NpcType_Count );
	theNpcTex = SAFE_NEW_ARRAY( Texture , c_NpcType_Count );	
	theNpcAni = SAFE_NEW_ARRAY( Bone , c_NpcType_Count * c_Npc_AniIndex_Count );	
	
	memset( m_pNpcFileInfo, 0, sizeof(CHR_FILE_INFO)*c_NpcType_Count );
	memset( theNpcItm, 0, sizeof(Character_Item)*c_NpcType_Count );	
	memset( theNpcMod, 0, sizeof(Model)*c_NpcType_Count );
	memset( theNpcTex, 0, sizeof(Texture)*c_NpcType_Count );
	memset( theNpcAni, 0, sizeof(Bone)*c_NpcType_Count*c_Npc_AniIndex_Count );	
	
	for( i = 0; i < the_sNpc_SetData.total_npc_num; ++i )
	{
		sprintf( file_path, "%d.chr", the_sNpc_SetData.file_name[i] );
		
		if( m_pNpcFileInfo[i].FileReadVF( &g_VFNpcRoot, file_path, SORT_NPC ) == true )
		{
			m_pNpcFileInfo[i].m_Index = the_sNpc_SetData.file_name[i];				
			
			SetNpcFileCode( i, m_pNpcFileInfo[i].m_Index );	
			
			for( idx = 0; idx < m_pNpcFileInfo[i].nItmCount; ++idx )
			{
				sprintf( file_path, "%d.itm", m_pNpcFileInfo[i].pItmIndex[idx] );
				
				if( theNpcItm[itm_cnt].File__OpenVF( &g_VFNpcItm, file_path ) == true )
				{					
					if( theNpcItm[itm_cnt].m_mod_index != -1 )
					{
						sprintf( file_path, "%d.msh", theNpcItm[itm_cnt].m_mod_index );	
						if( theNpcMod[msh_cnt].ReadVF( &g_VFNpcMsh, file_path, true ) == true ) 
						{
							theNpcMod[msh_cnt].m_Index = theNpcItm[itm_cnt].m_mod_index;			
							theNpcItm[itm_cnt].m_mod_index = msh_cnt + c_Chr_Mod_Count;
							++msh_cnt;
						}	
						else
						{
							return false;
						}
						
						sprintf( file_path, "%d.dds", theNpcItm[itm_cnt].m_tex_index );						
						
						theNpcTex[tex_cnt].m_Index = theNpcItm[itm_cnt].m_tex_index;			
						theNpcTex[tex_cnt].m_byAttr = DATA_DELETE0;			
						theNpcItm[itm_cnt].m_tex_index = tex_cnt + c_Chr_Tex_Count;
						++tex_cnt;					
					}
					
					theNpcItm[itm_cnt].CreateVrxBuf(SORT_NPC, true);
					theNpcItm[itm_cnt].m_Index = m_pNpcFileInfo[i].pItmIndex[idx];				
					m_pNpcFileInfo[i].pItmIndex[idx] = itm_cnt + c_Chr_Itm_Count;					
					++itm_cnt;
				}
				else
				{
					return false;
				}
			}
			
			for( idx = 0; idx < c_Npc_AniIndex_Count; ++idx )
			{
				if( m_pNpcFileInfo[i].pAniInfo[idx].ani_number != -1 )
				{
					sprintf( file_path, "%d.ani", m_pNpcFileInfo[i].pAniInfo[idx].ani_number );	
					if( theNpcAni[ani_cnt].ReadVF( &g_VFNpcAni, file_path ) == true )
					{
						theNpcAni[ani_cnt].m_Index = m_pNpcFileInfo[i].pAniInfo[idx].ani_number;				
						m_pNpcFileInfo[i].pAniInfo[idx].ani_number = ani_cnt + c_Chr_Ani_Count;
						++ani_cnt;
					}
					else
					{
						return false;
					}
					
				}
			}			
		}
		else
		{
			return false;
		}
	}
	
#endif
	
	the_sNpc_SetData.total_itm_num = itm_cnt;
	the_sNpc_SetData.total_msh_num = msh_cnt;
	the_sNpc_SetData.total_tex_num = tex_cnt;
	the_sNpc_SetData.total_ani_num = ani_cnt;
	
	//-----------------------------------------------------------------------------
	// NPC �Ӽ�, ��� , ����Ʈ ( ./NPC/NPC_SET.dat ) By wxywxy
	//-----------------------------------------------------------------------------
	//	Set__NpcAttr( file_pos, "\\Npc\\NPC_Set.Dat" );		
	
	////////////////////////////////////////////////////////////////////////////////
#endif
	
	///--PRINT_DLG_LOG( "!!!!!!!!!!!!!!!!!!!!! NPC load : %u !!!!!!!!!!!!!!!!!", SAFE_TIME_SUB( timeGetTime() , tick ) );
	RLG1( "!!!!!!!!!!!!!!!!!!!!! NPC load : %u !!!!!!!!!!!!!!!!!", SAFE_TIME_SUB( timeGetTime() , tick ) );
	
	return true;
}


int Player_Manager::GetFileCount( char* file_pos, char* file_ext )
{
	int count	= 0;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// �������� �ִ� ��� �����	
	char file_path[256] = {0,};
	strcpy( file_path, file_pos );
	strcat( file_path, file_ext);	
	
	// ���� ã��	
	WIN32_FIND_DATA	fild_data;
	HANDLE hFindFile = FindFirstFile( file_path, &fild_data );
	
	int Find	= -1;
	while( Find != 0 )
	{			
		Find = FindNextFile( hFindFile, &fild_data );
		count++;
	}					
	FindClose( hFindFile );
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else ///////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return count;
}



//-----------------------------------------------------------------------------
// Desc: ������ �Ӽ� �б�
//-----------------------------------------------------------------------------
bool Player_Manager::Set__ItemAttr( const char* file_pos )
{	
	char	file_path[256] = {0,};
	strcpy( file_path, file_pos );	
	strcat( file_path, "\\Chr\\ITEM_ATTR.dat"); 
	
	FILE*	file;
	file = fopen( file_path, "rt" );
	if( file == NULL )
		return false;
	
	assert( file != NULL && "!�����ۼӼ� �����б� ����" );
	{
		char* token;				// ��ū
		char string [256];			// ���� �ٴ��� ���� ����
		bool bRead  = true;			// ���� �� �о����� üũ
		bool bLine  = true;			// �� �о����� üũ
		int	 line	= 0;			// �ټ�
		
		char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
		
		// �� ������ �б�
		if( fgets( string, 256, file ) == NULL )				
			return false;				
		do 
		{					
			// �ּ��� �ִٸ� ���� �� ����
			while( string[0] == '/' && string[1] == '/' )	
			{				
				// �� �̻� ������ ���ٸ� ���� ����
				if( fgets( string, 256 , file ) == NULL )		
				{
					bRead = false;
					break;
				}
			}
			
			// ������ �ε���
			token = strtok( string, splitter );
			
			if( token == NULL )			
				break;		
			
			// ��ȣ		
			int m_number = (int)atoi(token);
			int i;
			bool isOpen = false;
			
			//			for( i = 0; i < CHR_ITM_COUNT + NPC_ITM_COUNT; i++ )
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( theChrItm[i].m_Index == m_number )
				{
					m_number = i;		
					isOpen = true;
					break;
				}				
			}
			
			if( isOpen )
			{				
				// ������ ����
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_sex = (BYTE)atoi(token);
				
				// ������ ����
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_race = (BYTE)atoi(token);
				
				// ������ �߿䵵
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_grav = (BYTE)atoi(token);
				
				// ������ Ŭ����
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_class = (BYTE)atoi(token);
				
				// ������ Ÿ��
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_type = (BYTE)atoi(token);
				
				// ������ �Ϸù�ȣ
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_number = (BYTE)atoi(token);
				
				// ��������Ÿ�
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_range = (BYTE)atoi(token);
				
				// �˱��ؽ���
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_tex = (BYTE)atoi(token);			
				
				// ���� ���� x
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisBottom.x = (float)atof(token);			
				
				// ���� ���� y
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisBottom.y = (float)atof(token);			
				
				// ���� ���� z
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisBottom.z = (float)atof(token);			
				
				// ���� ���� x
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisTop.x = (float)atof(token);			
				
				// ���� ���� y
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisTop.y = (float)atof(token);			
				
				// ���� ���� z
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisTop.z = (float)atof(token);			
				
				// �˱� ���ؽ���ȣ
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_min = (BYTE)atoi(token);	
				
				// ������ �����ȣ
				token = strtok( NULL, splitter );			
				theChrItm[m_number].m_AttackAniNum = (BYTE)atoi(token);
			}
			
			// ���� �� �б�, ������ ���ٸ� string�� 0����									
			if( fgets( string, 256 , file ) == NULL )		
				bRead = false;			
		} 
		while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			
	}
	
	fclose( file );
	return true;
}


//-----------------------------------------------------------------------------
// Desc: ������ �Ӽ� �б�
//-----------------------------------------------------------------------------
bool Player_Manager::Set__ItemAttrVF()
{	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( "ITEM_ATTR.dat" );
	if( pFH == NULL ) return false;
	assert( pFH != NULL && "!�����ۼӼ� �����б� ����" );
	{
		char* token;				// ��ū
		char string [256];			// ���� �ٴ��� ���� ����
		bool bRead  = true;			// ���� �� �о����� üũ
		bool bLine  = true;			// �� �о����� üũ
		int	 line	= 0;			// �ټ�
		
		char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
		
		// �� ������ �б�
		if( pFH->Gets( string, sizeof( string ) ) == NULL )								
		{
			g_VFChrRoot.CloseFile( pFH );
			return false; 
		}
		do 
		{					
			// �ּ��� �ִٸ� ���� �� ����
			while( string[0] == '/' && string[1] == '/' )	
			{				
				// �� �̻� ������ ���ٸ� ���� ����
				if( pFH->Gets( string, sizeof( string ) ) == NULL )		
				{
					bRead = false;
					break;
				}
			}
			
			// ������ �ε���
			token = strtok( string, splitter );
			
			if( token == NULL )			
				break;		
			
			// ��ȣ		
			int m_number = (int)atoi(token);
			int i;
			bool isOpen = false;
			
					
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( theChrItm[i].m_Index == m_number )
				{
					m_number = i; 
					isOpen = true;
					break;
				}				
			}			
			
			if( isOpen )
			{				
				// ������ ����
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_sex = (BYTE)atoi(token);
				
				// ������ ����
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_race = (BYTE)atoi(token);
				
				// ������ �߿䵵
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_grav = (BYTE)atoi(token);
				
				// ������ Ŭ����
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_class = (BYTE)atoi(token);
				
				// ������ Ÿ��
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_type = (BYTE)atoi(token);
				
				// ������ �Ϸù�ȣ
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_number = (BYTE)atoi(token);
				
				// ��������Ÿ�
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_range = (BYTE)atoi(token);
				
				// �˱��ؽ���
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_tex = (BYTE)atoi(token);			
				
				// ���� ���� x
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisBottom.x = (float)atof(token);			
				
				// ���� ���� y
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisBottom.y = (float)atof(token);			
				
				// ���� ���� z
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisBottom.z = (float)atof(token);			
				
				// ���� ���� x
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisTop.x = (float)atof(token);			
				
				// ���� ���� y
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisTop.y = (float)atof(token);			
				
				// ���� ���� z
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_AxisTop.z = (float)atof(token);			
				
				// �˱� ���ؽ���ȣ
				token = strtok( NULL, splitter );
				theChrItm[m_number].m_min = (BYTE)atoi(token);	
				
				// ������ �����ȣ
				token = strtok( NULL, splitter );			
				theChrItm[m_number].m_AttackAniNum = (BYTE)atoi(token);
			}
			
			// ���� �� �б�, ������ ���ٸ� string�� 0����									
			if( pFH->Gets( string, sizeof( string ) ) == NULL )		
				bRead = false;			
		} 
		while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			
	}
	g_VFChrRoot.CloseFile( pFH );
	return true;
}


void Player_Manager::Set__ItemChangeID()
{	
	memset( test_item_changeID, 0, sizeof(sTest_ItemChangeID)*4 );
	
	int num = 0;
	int common = 1;
	int i = 0;
	int add_type = 0;
	
	for( num = 0; num < m_itm_count; ++num )
	{
		int	chr_type = 0;
		DWORD itemID = SetItemID( theChrItm[num].m_sex, 
			theChrItm[num].m_race, 
			theChrItm[num].m_grav, 
			theChrItm[num].m_class, 
			theChrItm[num].m_type, 
			theChrItm[num].m_number);
		
		
		if( theChrItm[num].m_race == 0 && theChrItm[num].m_sex == 0 )
		{
			chr_type = 0;
		}
		else if( theChrItm[num].m_race == 0 && theChrItm[num].m_sex == 1 )
		{
			chr_type = 1;
		}
		else if( theChrItm[num].m_race == 1 && theChrItm[num].m_sex == 0 )
		{
			chr_type = 2;
		}
		else if( theChrItm[num].m_race == 1 && theChrItm[num].m_sex == 1 )
		{
			chr_type = 3;
		}
		
		if( theChrItm[num].m_sex == 9 && theChrItm[num].m_race != 9)
		{
			if( theChrItm[num].m_race == 0 )
			{
				chr_type = 0;
			}
			else if( theChrItm[num].m_race == 1 )
			{
				chr_type = 2;
			}			
			add_type = 1;
			common = 2;			// ��-�� ���� 			
		}
		
		if( theChrItm[num].m_race == 9 && theChrItm[num].m_sex != 9 )
		{
			if( theChrItm[num].m_sex == 0 )
			{
				chr_type = 0;
			}
			else if( theChrItm[num].m_sex == 1 )
			{
				chr_type = 1;
			}			
			add_type = 2;
			common = 2;			// ��-�� ���� 			
		}
		
		
		for( i = 0; i < common; ++i )
		{
			chr_type = chr_type + (i * add_type);
			
			if( theChrItm[num].m_grav == 0 )	// ���� ������ 
			{
				if( theChrItm[num].m_type == 0 ) // ��ݽ� 
				{
					test_item_changeID[chr_type].coat_armor[test_item_changeID[chr_type].cnt_coat_armor] = itemID;
					++test_item_changeID[chr_type].cnt_coat_armor;
				}
				else if( theChrItm[num].m_type == 1 ) // �Ϲݽ� 
				{
					test_item_changeID[chr_type].pants_armor[test_item_changeID[chr_type].cnt_pants_armor] = itemID;
					++test_item_changeID[chr_type].cnt_pants_armor;
				}
				else if( theChrItm[num].m_type == 2 ) // �尩 
				{
					test_item_changeID[chr_type].gloves[test_item_changeID[chr_type].cnt_gloves] = itemID;
					++test_item_changeID[chr_type].cnt_gloves;
				}
				else if( theChrItm[num].m_type == 3 ) // ����  
				{
					test_item_changeID[chr_type].shoes[test_item_changeID[chr_type].cnt_shoes] = itemID;
					++test_item_changeID[chr_type].cnt_shoes;
				}
				else if( theChrItm[num].m_type == 7 ) // �� 
				{
					test_item_changeID[chr_type].face[test_item_changeID[chr_type].cnt_face] = itemID;
					++test_item_changeID[chr_type].cnt_face;
				}
				else if( theChrItm[num].m_type == 8 ) // ���  
				{
					test_item_changeID[chr_type].hair[test_item_changeID[chr_type].cnt_hair] = itemID;
					++test_item_changeID[chr_type].cnt_hair;
				}
			}
			else if( theChrItm[num].m_grav == 1 )	// ����
			{
				if( theChrItm[num].m_type == 0 ||	// ���, �Ѽհ�, ������, ����� 
					theChrItm[num].m_type == 2 ||				
					theChrItm[num].m_type == 6 ||
					theChrItm[num].m_type == 7 )	// ������ ����
				{
					test_item_changeID[chr_type].right_hand[test_item_changeID[chr_type].cnt_right_hand] = itemID;
					++test_item_changeID[chr_type].cnt_right_hand;
				}
				else if ( theChrItm[num].m_type == 1 ||		// Ȱ, ����, ������
					theChrItm[num].m_type == 3 ||
					theChrItm[num].m_type == 5 )		// �޼� ����
				{
					test_item_changeID[chr_type].left_hand[test_item_changeID[chr_type].cnt_left_hand] = itemID;
					++test_item_changeID[chr_type].cnt_left_hand;
				}
			}
		}
	}
	return;
}




//-----------------------------------------------------------------------------
// Desc: ���� �Ӽ� �б�
//-----------------------------------------------------------------------------
bool	Player_Manager::Set__PcAttr( const char* file_pos )
{
	char	file_path[256] = {0,};
	strcpy( file_path, file_pos );
	strcat( file_path, "\\CHR\\ATTR.dat" ); 
	
	FILE*	file;
	file = fopen( file_path, "rt" );
	assert( file != NULL && "!�����Ӽ� �����б� ����" );
	{
		char* token;				// ��ū
		char string [256];		// ���� �ٴ��� ���� ����
		bool bRead  = true;			// ���� �� �о����� üũ		
		int	 number	= 0;			// �ټ�		
		char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
		int i;
		
		// �� ������ �б�
		if( fgets( string, 256, file ) == NULL )				
			return false;				
		do 
		{								
			// �ּ��� �ִٸ� ���� �� ����
			while( string[0] == '/' && string[1] == '/' )	
			{				
				// �� �̻� ������ ���ٸ� ���� ����
				if( fgets( string, 256 , file ) == NULL )		
				{
					bRead = false;
					break;
				}
			}
			
			// ���� �̸�
			token = strtok( string, splitter );	
			
			if( token == NULL )			
				break;	
			
			
			// �߽� ���
			token = strtok( NULL, splitter );
			m_PcAttr[number].center_node = (unsigned int)atoi(token);
			
			// ������ ���
			token = strtok( NULL, splitter );
			m_PcAttr[number].r_hand_node = (unsigned int)atoi(token);
			
			// �޼� ��� 
			token = strtok( NULL, splitter );
			m_PcAttr[number].l_hand_node = (unsigned int)atoi(token);
			
			// ���� ��� 
			token = strtok( NULL, splitter );
			m_PcAttr[number].breast_node = (unsigned int)atoi(token);
			
			// �Ӹ� ��� 
			token = strtok( NULL, splitter );
			m_PcAttr[number].hair_node = (unsigned int)atoi(token);
			
			// �޹� ��� 
			token = strtok( NULL, splitter );
			m_PcAttr[number].l_foot_node = (unsigned int)atoi(token);
			
			// ������ ��� 
			token = strtok( NULL, splitter );
			m_PcAttr[number].r_foot_node = (unsigned int)atoi(token);
			
			// �Ӹ�
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].hair = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].hair = (unsigned int)atoi(token);
			
			// ��
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].face = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].face = (unsigned int)atoi(token);
			
			// ��ݽ� 
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].body = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].body = (unsigned int)atoi(token);
			
			// �Ϲݽ�
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].pants = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].hand = (unsigned int)atoi(token);
			
			// ��
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].hand = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].foot = (unsigned int)atoi(token);
			
			// ��
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].foot = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].wing = (unsigned int)atoi(token);			
			number++;
			
			// ���� �� �б�, ������ ���ٸ� string�� 0����									
			if( fgets( string, 256 , file ) == NULL )		
				bRead = false;			
		} 
		while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			
	}
	
	fclose( file );  
	return true;
}



//-----------------------------------------------------------------------------
// Desc: ���� �Ӽ� �б�
//-----------------------------------------------------------------------------
bool	Player_Manager::Set__PcAttrVF()
{
	VFileHandle* pFH = g_VFChrRoot.OpenFile( "ATTR.dat" );
	if( pFH == NULL ) return false;
	assert( pFH != NULL && "!�����Ӽ� �����б� ����" );
	{
		char* token;				// ��ū
		char string [256];		// ���� �ٴ��� ���� ����
		bool bRead  = true;			// ���� �� �о����� üũ		
		int	 number	= 0;			// �ټ�		
		char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
		int i;
		
		// �� ������ �б�
		if( pFH->Gets( string, sizeof( string ) ) == NULL )
		{
			g_VFChrRoot.CloseFile( pFH );
			return false; 
		}
		do 
		{								
			// �ּ��� �ִٸ� ���� �� ����
			while( string[0] == '/' && string[1] == '/' )	
			{				
				// �� �̻� ������ ���ٸ� ���� ����
				if( pFH->Gets( string, sizeof( string ) ) == NULL )
				{
					bRead = false;
					break;
				}
			}
			
			// ���� �̸�
			token = strtok( string, splitter );	
			
			if( token == NULL )			
				break;	
			
			
			// �߽� ���
			token = strtok( NULL, splitter );
			m_PcAttr[number].center_node = (unsigned int)atoi(token);
			
			// ������ ���
			token = strtok( NULL, splitter );
			m_PcAttr[number].r_hand_node = (unsigned int)atoi(token);
			
			// �޼� ��� 
			token = strtok( NULL, splitter );
			m_PcAttr[number].l_hand_node = (unsigned int)atoi(token);
			
			
#ifdef DEFENSE_EFFECT
			//�� ��� 
			token = strtok ( NULL , splitter );
			m_PcAttr[number].l_shoulder_node = (unsigned int)atoi(token);
			
			//���� ���
			token = strtok (NULL , splitter );
			m_PcAttr[number].r_shoulder_node = (unsigned int)atoi(token);
			
			//�� ��ġ
			token = strtok ( NULL , splitter ) ; 
			m_PcAttr[number].l_elbow_node  = ( unsigned int)atoi(token);
			
			//���� ��ġ
			token = strtok (NULL, splitter) ;
			m_PcAttr[number].r_elbow_node = (unsigned int )atoi(token);
			
			//�� ���� 
			token = strtok (NULL , splitter);
			m_PcAttr[number].l_knee_node = (unsigned int)atoi(token);
			
			//���� ���� 
			token = strtok(NULL, splitter);
			m_PcAttr[number].r_knee_node = (unsigned int)atoi(token);
			
			// �� ��� 
			token = strtok(NULL, splitter) ;
			m_PcAttr[number].l_pelvis_node = (unsigned int)atoi(token);
			
			//���� ��� 
			token = strtok(NULL, splitter) ; 
			m_PcAttr[number].r_pelvis_node = (unsigned int)atoi(token);
#endif
			
			// ���� ��� 
			token = strtok( NULL, splitter );
			m_PcAttr[number].breast_node = (unsigned int)atoi(token);
			
			// �Ӹ� ��� 
			token = strtok( NULL, splitter );
			m_PcAttr[number].hair_node = (unsigned int)atoi(token);
			
			// �޹� ��� 
			token = strtok( NULL, splitter );
			m_PcAttr[number].l_foot_node = (unsigned int)atoi(token);
			
			// ������ ��� 
			token = strtok( NULL, splitter );
			m_PcAttr[number].r_foot_node = (unsigned int)atoi(token);
			
			
			// �Ӹ�
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].hair = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].hair = (unsigned int)atoi(token);
			
			// ��
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].face = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].face = (unsigned int)atoi(token);
			
			// ��ݽ� 
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].body = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].body = (unsigned int)atoi(token);
			
			// �Ϲݽ�
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].pants = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].hand = (unsigned int)atoi(token);
			
			// ��
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].hand = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].foot = (unsigned int)atoi(token);
			
			// ��
			token = strtok( NULL, splitter );
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == (unsigned int)atoi(token) )
				{
					m_PcAttr[number].foot = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					break;
				}
			}
			//m_PcAttr[number].wing = (unsigned int)atoi(token);			
			number++;
			
			// ���� �� �б�, ������ ���ٸ� string�� 0����									
			if( pFH->Gets( string, sizeof( string ) ) == NULL )
				bRead = false;					
		} 
		while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			
	}
	
	g_VFChrRoot.CloseFile( pFH );
	
	return true;
}


//-----------------------------------------------------------------------------
// Desc: NPC �Ӽ��ֱ�
//-----------------------------------------------------------------------------
bool Player_Manager::Set__NpcSet( const char* file_pos, int curMapIndex )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos );
	sprintf( file_name, "\\Npc\\NPC_%d.set", curMapIndex);
	strcat( file_path, file_name);		
	
	FILE*	file;
	file = fopen( file_path, "rt" );
	if( file == NULL )
		return false;	
	
	char string [256];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ		
	int	 number	= 0;			// ������ ��ȣ		
	char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
	
	int npcCode = 0;
	int cellPosX = 0;
	int cellPosY = 0;
	
	the_sNpc_SetData.Clear();
	
	//by simwoosung
	MiniMapWnd.AllClearNPCInfo();		//�̴ϸ� -�ʻ� ���� NPC���� ����
	
	// �� ������ �б�
	if( fgets( string, 256, file ) == NULL )				
		return false;				
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( fgets( string, 256 , file ) == NULL )		
			{
				bRead = false;
				break;
			}
		}		
		
		sscanf(string, "%d %d %d", &npcCode, &cellPosX, &cellPosY);
		//the_sNpc_SetData.npc_code[number] = npcCode;
		
		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( the_sNpc_SetData.npc_code[number] ); 
		//the_sNpc_SetData.file_name[number] = npc_Info->theFileCode;
		
		//by simwoosung
		WORD nCode = npcCode;
		if( IsNonPlayerCode(nCode) )
		{
			SMinimapNPCInfo * pNPCInfo = SAFE_NEW( SMinimapNPCInfo );			
			pNPCInfo->nNpcIndex = npcCode;
			Rstrcpy( pNPCInfo->strNpcName , npc_Info->theName);
			pNPCInfo->nCellPosX = cellPosX;
			pNPCInfo->nCellPosY = cellPosY;
			pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
			MiniMapWnd.SetNPCQuestInfo(pNPCInfo);
			MiniMapWnd.InsertNPCInfo(pNPCInfo);			
		}
		
		// ������ȣ��...
		//++number;								
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( fgets( string, 256 , file ) == NULL )		
			bRead = false;			
		
		// �ʿ� ���� NPC �� �� ���� 
		//the_sNpc_SetData.total_npc_num = number;
	} 
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	fclose( file );
	
	//...........................................................................................................
	
	// Drop �� �������� �з��Ͽ� ������, ����, ���� ����, ���� ����, ����Ʈ ���ڷ� ����� �����ش� 
	// n_Pack	
	the_sNpc_SetData.file_name[number] = 19903000;		// ������ ������..."19903000.chr"
	the_sNpc_SetData.total_npc_num = ++number;
	
	// n_Eld
	the_sNpc_SetData.file_name[number] = 19903001;		
	the_sNpc_SetData.total_npc_num = ++number;
	
	// n_NormalBox
	the_sNpc_SetData.file_name[number] = 19903002;		
	the_sNpc_SetData.total_npc_num = ++number;
	
	// n_GoodBox
	the_sNpc_SetData.file_name[number] = 19903003;		
	the_sNpc_SetData.total_npc_num = ++number;
	
	// n_QuestBox
	the_sNpc_SetData.file_name[number] = 19903004;		
	the_sNpc_SetData.total_npc_num = ++number;
	
	the_sNpc_SetData.total_npc_set_num = the_sNpc_SetData.total_npc_num;
	
	return true;
}

//-----------------------------------------------------------------------------
// Desc: NPC �Ӽ��ֱ�
//-----------------------------------------------------------------------------
bool Player_Manager::Set__NpcSetVF( int curMapIndex )
{
	char	file_name[256] = {0,};
	
	char string [256];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ		
	int	 number	= 0;			// ������ ��ȣ		
	char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
	
	int npcCode = 0;
	int cellPosX = 0;
	int cellPosY = 0;

	sprintf( file_name, "NPC_%d.set", curMapIndex);
	
	VFileHandle* pFH = g_VFNpcRoot.OpenFile( file_name );
	
	///-- �� ������ �������� ������... �� ���� ��� �׸��̱�������..
	///-- ����Ʈ�� ���ؼ� NPC��ġ�� �����Ҷ��� �ʿ��ϴ�.
	if( pFH == NULL ) 
	{
		the_sNpc_SetData.Clear();	
		//by simwoosung
		MiniMapWnd.AllClearNPCInfo();		//�̴ϸ� -�ʻ� ���� NPC���� ����		
		goto LOADITEM_NPC;
	}
	
	the_sNpc_SetData.Clear();	
	//by simwoosung
	MiniMapWnd.AllClearNPCInfo();		//�̴ϸ� -�ʻ� ���� NPC���� ����
	
	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == NULL )
	{
		g_VFNpcRoot.CloseFile( pFH );
		return false; 
	}			
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == NULL )
			{
				bRead = false;
				break;
			}
		}
		
		sscanf(string, "%d %d %d", &npcCode, &cellPosX, &cellPosY);		
		
		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( npcCode ); 		
		
		//by simwoosung
		WORD nCode = npcCode;
		if( IsNonPlayerCode(nCode) )
		{
			SMinimapNPCInfo * pNPCInfo = SAFE_NEW( SMinimapNPCInfo );			
			pNPCInfo->nNpcIndex = npcCode;
			Rstrcpy( pNPCInfo->strNpcName , npc_Info->theName);
			pNPCInfo->nCellPosX = cellPosX;
			pNPCInfo->nCellPosY = cellPosY;
			pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
			MiniMapWnd.SetNPCQuestInfo(pNPCInfo);
			MiniMapWnd.InsertNPCInfo(pNPCInfo);			
		}								
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == NULL )
			bRead = false;
	} 
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	
	g_VFNpcRoot.CloseFile( pFH );
	
LOADITEM_NPC:
	
	//...........................................................................................................
	
	// Drop �� �������� �з��Ͽ� ������, ����, ���� ����, ���� ����, ����Ʈ ���ڷ� ����� �����ش� 
	// n_Pack	
	the_sNpc_SetData.file_name[number] = 19903000;		// ������ ������..."19903000.chr"
	the_sNpc_SetData.total_npc_num = ++number;
	
	// n_Eld
	the_sNpc_SetData.file_name[number] = 19903001;		
	the_sNpc_SetData.total_npc_num = ++number;
	
	// n_NormalBox
	the_sNpc_SetData.file_name[number] = 19903002;		
	the_sNpc_SetData.total_npc_num = ++number;
	
	// n_GoodBox
	the_sNpc_SetData.file_name[number] = 19903003;		
	the_sNpc_SetData.total_npc_num = ++number;
	
	// n_QuestBox
	the_sNpc_SetData.file_name[number] = 19903004;		
	the_sNpc_SetData.total_npc_num = ++number;
	
	the_sNpc_SetData.total_npc_set_num = the_sNpc_SetData.total_npc_num;
	
	return true;
}

void Player_Manager::ReSet__NpcSetVF( int curMapIndex )
{
	char	file_name[256] = {0,};
	
	char string [256];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ		
	int	 number	= 0;			// ������ ��ȣ		
	char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
	
	int npcCode = 0;
	int cellPosX = 0;
	int cellPosY = 0;

	sprintf( file_name, "NPC_%d.set", curMapIndex);
	
	VFileHandle* pFH = g_VFNpcRoot.OpenFile( file_name );

	///-- �� ������ �������� ������... �� ���� ��� �׸��̱�������..
	///-- ����Ʈ�� ���ؼ� NPC��ġ�� �����Ҷ��� �ʿ��ϴ�.
	if( pFH == NULL ) 
	{
		return;
	}	

	//by simwoosung
	MiniMapWnd.AllClearNPCInfo();		//�̴ϸ� -�ʻ� ���� NPC���� ����
	
	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == NULL )
	{
		g_VFNpcRoot.CloseFile( pFH );
		return; 
	}			
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == NULL )
			{
				bRead = false;
				break;
			}
		}
		
		sscanf(string, "%d %d %d", &npcCode, &cellPosX, &cellPosY);		
		
		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( npcCode ); 		
		
		//by simwoosung
		WORD nCode = npcCode;
		if( IsNonPlayerCode(nCode) )
		{
			SMinimapNPCInfo * pNPCInfo = SAFE_NEW( SMinimapNPCInfo );			
			pNPCInfo->nNpcIndex = npcCode;
			Rstrcpy( pNPCInfo->strNpcName , npc_Info->theName);
			pNPCInfo->nCellPosX = cellPosX;
			pNPCInfo->nCellPosY = cellPosY;
			pNPCInfo->nQuestMarkType = NONE_QUEST_MARK;
			MiniMapWnd.SetNPCQuestInfo(pNPCInfo);
			MiniMapWnd.InsertNPCInfo(pNPCInfo);			
		}								
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == NULL )
			bRead = false;
	} 
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	

	g_VFNpcRoot.CloseFile( pFH );

}

//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼� ����Ʈ ( ���� �κ� �߰� by wxywxy )
//-----------------------------------------------------------------------------
bool Player_Manager::Set__PcAnimationList( const char* file_pos, int index )
{
	FILE*	file;
	file = fopen( file_pos, "rt" );
	if( file == NULL )
		return false;
	
	char* token;				// ��ū
	char string [512];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ			
	char splitter[] = "\n\t";	// ������ : ĳ��������, �� 
	int  number = 0;
	int  MantleNum;
	
	// �� ������ �б�
	if( fgets( string, 512, file ) == NULL )				
		return false;				
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( fgets( string, 512 , file ) == NULL )		
			{
				bRead = false;
				break;
			}
		}
		
		// PC �ִϸ��̼� �����̸�
		if( NULL == (token = strtok( string, splitter )) )	
			break;
		
		int i;
		int ani_num;
		
		ani_num = (int)atoi(token);			
		
		//		for( i = 0; i < CHR_ANI_COUNT + NPC_ANI_COUNT; i++ )
		for( i = 0; i < c_Chr_Ani_Count; i++ )			
		{					
			if( g_Pc_Manager.theChrAni[i].m_Index == ani_num )
			{
				m_PcAni[index][number].ani_number = i; 
				break;
			}
		}
		
		// PC ���� �����̸�
		token = strtok( NULL, splitter );
		m_PcAni[index][number].sound_number = (int)atoi(token);
		
		// PC ����Ű
		token = strtok( NULL, splitter );
		m_PcAni[index][number].sound_key = (int)atoi(token);
		
		// �߰��Ǵ� ���� ���ϰ� Ű���� �����Ѵ�
		if( NULL != (token = strtok( NULL, splitter )) )
			m_PcAddSound[index][number].sound_number = (int)atoi(token);
		if( NULL != (token = strtok( NULL, splitter )) )
			m_PcAddSound[index][number].sound_key = (int)atoi(token);
		
		// Impact_Frame 
		token = strtok( NULL, splitter );
		m_PcAni_Act[index][number].Impact_frame = (int)atoi(token);
		
		if( m_PcAni_Act[index][number].Impact_frame < c_MotionBlendFrame )	
		{
			m_PcAni_Act[index][number].Impact_frame = c_MotionBlendFrame;
		}
		
		// �޺� Ÿ�̹� ���� ���� ������ 
		token = strtok( NULL, splitter );
		m_PcAni_Act[index][number].range_startframe = (int)atoi(token);
		
		// �޺� Ÿ�̹� ���� ���� ������ 
		token = strtok( NULL, splitter );
		m_PcAni_Act[index][number].range_endframe = (int)atoi(token);
		
		// ���� ���ؽ� ���� �޽� �̸�
		token = strtok( NULL, splitter );
		MantleNum = (int)atoi(token);
		
		// m_Mantle.LoadMSH( index , number , MantleNum );
		
		// �����迭
		number++;
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( fgets( string, 512 , file ) == NULL )		
			bRead = false;			
	} 
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	
	// ���ϴݱ�
	fclose( file );	
	return true;
}


//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼� ����Ʈ ( ���� �κ� �߰� by wxywxy )
//-----------------------------------------------------------------------------
bool Player_Manager::Set__PcAnimationListVF( const char* file_pos, int index )
{
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_pos );
	if( pFH == NULL ) return false;
	
	char* token;				// ��ū
	char string [512];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ			
	char splitter[] = "\n\t";	// ������ : ĳ��������, �� 
	int  number = 0;
	//	int  MantleNum;
	
	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == NULL )
	{
		g_VFChrRoot.CloseFile( pFH );
		return false; 
	}				
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == NULL )
			{
				bRead = false;
				break;
			}
		}
		
		// PC �ִϸ��̼� �����̸�
		if( NULL == (token = strtok( string, splitter )) )	
			break;
		
		int i;
		int ani_num;
		
		ani_num = (int)atoi(token);			
		/*
		if( 41907416 == ani_num )	
		{
		__asm int 3
		}
		*/
		
		//		for( i = 0; i < CHR_ANI_COUNT + NPC_ANI_COUNT; i++ )
		for( i = 0; i < c_Chr_Ani_Count; i++ )			
		{					
			if( g_Pc_Manager.theChrAni[i].m_Index == ani_num )
			{
				m_PcAni[index][number].ani_number = i; 
				break;
			}
		}
		
		// PC ���� �����̸�
		token = strtok( NULL, splitter );
		m_PcAni[index][number].sound_number = (int)atoi(token);
		
		// PC ����Ű
		token = strtok( NULL, splitter );
		m_PcAni[index][number].sound_key = (int)atoi(token);
		
		// �߰��Ǵ� ���� ���ϰ� Ű���� �����Ѵ�
		token = strtok( NULL, splitter );
		m_PcAddSound[index][number].sound_number = (int)atoi(token);
		
		token = strtok( NULL, splitter );
		m_PcAddSound[index][number].sound_key = (int)atoi(token);
		
		// Impact_Frame 
		token = strtok( NULL, splitter );
		m_PcAni_Act[index][number].Impact_frame = (int)atoi(token);
		
		if( m_PcAni_Act[index][number].Impact_frame < c_MotionBlendFrame )	
		{
			m_PcAni_Act[index][number].Impact_frame = c_MotionBlendFrame;
		}
		
		// �޺� Ÿ�̹� ���� ���� ������ 
		token = strtok( NULL, splitter );
		m_PcAni_Act[index][number].range_startframe = (int)atoi(token);
		
		// �޺� Ÿ�̹� ���� ���� ������ 
		token = strtok( NULL, splitter );
		m_PcAni_Act[index][number].range_endframe = (int)atoi(token);
		
		/*		
		// ���� ���ؽ� ���� �޽� �̸�
		token = strtok( NULL, splitter );
		MantleNum = (int)atoi(token);
		*/
		// m_Mantle.LoadMSH( index , number , MantleNum );
		
		// �����迭
		number++;
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == NULL )
		{
			bRead = false;
			break;
		}
	} 
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	
	// ���ϴݱ�
	g_VFChrRoot.CloseFile( pFH );
	
	return true;
}


//-----------------------------------------------------------------------------
// Desc: �ؽ��� �Ӽ� ���� �б�
//-----------------------------------------------------------------------------
bool Player_Manager::Set__TextureAttr( const char* file_pos )
{
	// ���Ϲ��ڿ������
	char	file_path[256] = {0,};
	strcpy( file_path, file_pos );
	strcat( file_path, "\\Chr\\TexAttr.dat" );
	
	// �����б�
	FILE*	file;
	file = fopen( file_path, "rt" );
	if( file == NULL )
		return false;
	
	char*	token;					// ��ū
	char	string[512];			// ���� �ٴ��� ���� ����				
	char	splitter[] = "\n\t";	// ������ : ĳ��������, ��	
	bool	bRead	= true;			// ���� �� �о����� üũ
	bool	bNPC	= false;
	
	// �� ������ �б�
	if( fgets( string, 512, file ) == NULL )				
		return false;				
	do 
	{			
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// NPC �Ӽ����̴�?
			token = strtok( string, splitter );
			if( strcmp( "//NPC", token ) == 0 )			
				bNPC = true;							
			
			// �� �̻� ������ ���ٸ� ���� ����
			if( fgets( string, 512 , file ) == NULL )		
			{
				bRead = false;
				break;
			}
		}
		
		// �ؽ��� �ε���
		token = strtok( string, splitter );
		if( token == NULL )
			break;				
		int number = (int)atoi( token );
		if( bNPC == true )
			number += c_Chr_Tex_Count;
		
		// �ؽ��� �ۼ���
		token = strtok( NULL, splitter );
		theChrTex[number].m_byAttr = (BYTE)atoi( token );
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( fgets( string, 512 , file ) == NULL )		
			bRead = false;			
	} 
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	
	// ���ϴݱ�
	fclose( file );
	return true;
}


///=======================================================================================
/// SetItemID( BYTE aSex, BYTE aRace, BYTE aGrav, BYTE aClass, BYTE aType, BYTE aNumber);
/// Return : tempitemID.m_dwItemType;
/// Desc : ������ �Ķ���͸� ������ ������ ������ �ε����� �Ҵ��Ѵ�. 
///=======================================================================================
DWORD Player_Manager::SetItemID( BYTE aSex, BYTE aRace, BYTE aGrav, BYTE aClass, BYTE aType, BYTE aNumber)
{
	_uItemType tempitemID;
	tempitemID.m_dwItemType = 0;
	
	tempitemID.m_byItemType[ITEM_KEY_NUMBER] = aNumber;
	tempitemID.m_byItemType[ITEM_KEY_TYPE] = aType;
	tempitemID.m_byItemType[ITEM_KEY_CLASS] += aClass << 4;
	tempitemID.m_byItemType[ITEM_KEY_GRAV] = aGrav;
	tempitemID.m_byItemType[ITEM_KEY_RACE] += aRace << 4;
	tempitemID.m_byItemType[ITEM_KEY_SEX] = aSex ^ 16;
	
	return tempitemID.m_dwItemType;
	
	/// sooree network
	///	return 0;
}

//-----------------------------------------------------------------------------
// Desc: �ش� ������ �迭�ε��� ����
//-----------------------------------------------------------------------------
int	Player_Manager::GetItemID( DWORD item_id )
{
	_uItemType tempitem;
	tempitem.m_dwItemType = item_id;
	BYTE item_sex		= tempitem.m_byItemType[ITEM_KEY_SEX]	& ITEM_KEY_SEX_BIT;
	BYTE item_race		= (tempitem.m_byItemType[ITEM_KEY_RACE]	& ITEM_KEY_RACE_BIT) >> 4;
	BYTE item_grav		= tempitem.m_byItemType[ITEM_KEY_GRAV]	& ITEM_KEY_GRAV_BIT;
	BYTE item_class		= tempitem.m_byItemType[ITEM_KEY_CLASS] >> 4;
	BYTE item_type		= tempitem.m_byItemType[ITEM_KEY_TYPE]  & ITEM_KEY_TYPE_BIT;
	BYTE item_number	= tempitem.m_byItemType[ITEM_KEY_NUMBER];
	///=======================================================================================
	/// SetItemID( BYTE aSex, BYTE aRace, BYTE aGrav, BYTE aClass, BYTE aType, BYTE aNumber);
	/// Return : tempitemID.m_dwItemType;
	/// Desc : ������ �Ķ���͸� ������ ������ ������ �ε����� �Ҵ��Ѵ�. 
	///=======================================================================================
	_uItemType tempitemID;
	tempitemID.m_dwItemType = 0;
	tempitemID.m_byItemType[ITEM_KEY_NUMBER] = item_number;
	tempitemID.m_byItemType[ITEM_KEY_TYPE] = item_type;
	tempitemID.m_byItemType[ITEM_KEY_CLASS] += item_class << 4;
	tempitemID.m_byItemType[ITEM_KEY_GRAV] = item_grav;
	tempitemID.m_byItemType[ITEM_KEY_RACE] += item_race << 4;
	tempitemID.m_byItemType[ITEM_KEY_SEX] = item_sex ^ 16;
	
	BYTE test_item_sex		= tempitemID.m_byItemType[ITEM_KEY_SEX]	& ITEM_KEY_SEX_BIT;
	BYTE test_item_race		= (tempitemID.m_byItemType[ITEM_KEY_RACE]	& ITEM_KEY_RACE_BIT) >> 4;
	BYTE test_item_grav		= tempitemID.m_byItemType[ITEM_KEY_GRAV]	& ITEM_KEY_GRAV_BIT;
	BYTE test_item_class	= tempitemID.m_byItemType[ITEM_KEY_CLASS] >> 4;
	BYTE test_item_type		= tempitemID.m_byItemType[ITEM_KEY_TYPE]  & ITEM_KEY_TYPE_BIT;
	BYTE test_item_number	= tempitemID.m_byItemType[ITEM_KEY_NUMBER];
	
	for( int i = 0; i < (int)m_itm_count; ++i )
	{
		if( m_pItm[i]->m_sex	== item_sex		&& m_pItm[i]->m_race == item_race && m_pItm[i]->m_grav	== item_grav &&
			m_pItm[i]->m_class	== item_class	&& m_pItm[i]->m_type == item_type && m_pItm[i]->m_number	== item_number )
			return i;
	}
	
	return -1;
}


//-----------------------------------------------------------------------------
// Desc: ������ Ÿ�� ����
//-----------------------------------------------------------------------------
//int	Player_Manager::GetItemType( DWORD item_id )
//{
/*	_uItemType tempitem;
tempitem.m_dwItemType = item_id;	
int item_type = tempitem.m_byItemType[ITEM_KEY_TYPE]  & ITEM_KEY_TYPE_BIT;
return item_type;
*/
/// sooree network
//	return 0;
//}


//-----------------------------------------------------------------------------
// Desc: ���ܰ����� �ƾ����̴�?
//-----------------------------------------------------------------------------
bool Player_Manager::GetAttackItem()
{
	if( g_Pc.GetPlayer()->m_pUserInfo != NULL )
	{
		if( g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0] == -1 &&
			g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1] == -1 )
			return false;
		
		int RHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0];
		int LHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1];
		
		// �Ѽչ���
		if( RHand_index != -1 )
		{
			if( g_Pc_Manager.m_pItm[RHand_index]->m_class == 0 )
				return true;
		}
		
		// ��չ���
		if( LHand_index != -1 )
		{
			if( g_Pc_Manager.m_pItm[LHand_index]->m_class == 1 )
				return true;
		}
	}
	return false;
}

int	Player_Manager::GetAttackItemIndex( Character * pChar , int table_index )
{
	int Res = -1;
	if( pChar->m_bPC == true || 
		( !pChar->m_bPC && pChar->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		//-----------------------------------------------------------------------------
		// �ڱ�ĳ����
		//-----------------------------------------------------------------------------
		int RHand_index = -1;
		int LHand_index = -1;
		if( pChar->m_bPC && table_index == MAX_USER_INTABLE )
		{
			if( g_Pc.GetPlayer()->m_pUserInfo != NULL )
			{
				if( g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0] == -1 &&
					g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1] == -1 )
					return Res;
				
				RHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0];
				LHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1];
			}
		}
		//-----------------------------------------------------------------------------
		// �ٸ� ����
		//-----------------------------------------------------------------------------
		else
		{
			if( pChar->m_bPC )
			{
				if( g_Pc.m_Pc[table_index].m_pUserInfo != NULL )
				{
					if( g_Pc.m_Pc[table_index].m_pUserInfo->ArmorInfo.index[0] == -1 &&
						g_Pc.m_Pc[table_index].m_pUserInfo->ArmorInfo.index[1] == -1 )
						return Res;			
					RHand_index = g_Pc.m_Pc[table_index].m_pUserInfo->ArmorInfo.index[0];
					LHand_index = g_Pc.m_Pc[table_index].m_pUserInfo->ArmorInfo.index[1];
				}
			}
			else if(!pChar->m_bPC && pChar->m_State.nCharacterType == nCharacterTypePlayerNPC)
			{
				if( g_Map.m_Npc[table_index].m_pUserInfo != NULL )
				{
					if( g_Map.m_Npc[table_index].m_pUserInfo->ArmorInfo.index[0] == -1 &&
						g_Map.m_Npc[table_index].m_pUserInfo->ArmorInfo.index[1] == -1 )
						return Res;			
					RHand_index = g_Map.m_Npc[table_index].m_pUserInfo->ArmorInfo.index[0];
					LHand_index = g_Map.m_Npc[table_index].m_pUserInfo->ArmorInfo.index[1];				
				}
			}
		}
		
		if( RHand_index != -1 )
		{
			switch( g_Pc_Manager.m_pItm[RHand_index]->m_type )
			{
			case 0:		// ��� 
			case 2:		// �Ѽհ� 
			case 6:		// ������ 
			case 7:		// �����
				{
					return RHand_index;
				}
				break;
			}
		}
		else if( LHand_index != -1 )
		{
			switch( g_Pc_Manager.m_pItm[LHand_index]->m_type )
			{
			case 1:			// Ȱ
			case 3:			// ����
			case 5:			// ������ 
				{
					return LHand_index;
				}
				break;
			}
		}
	}
	return Res;
}


//-----------------------------------------------------------------------------
// Desc: �ش� �����۰� ���� ������ã��
// (�տ� ��� Ȱ�� � ���� �ִ� Ȱ)
//-----------------------------------------------------------------------------
int Player_Manager::GetEqualItemIndex( int _nItmIndex )
{
	BYTE bySex		= m_pItm[_nItmIndex]->m_sex;
	BYTE byRace		= m_pItm[_nItmIndex]->m_race;
	BYTE byRrav		= m_pItm[_nItmIndex]->m_grav;
	BYTE byClass	= m_pItm[_nItmIndex]->m_class;
	BYTE byType		= m_pItm[_nItmIndex]->m_type;
	BYTE byNumber	= m_pItm[_nItmIndex]->m_number;
	
	for( int i = 0; i < (int)m_itm_count; ++i )
	{		
		if( i != _nItmIndex )
		{
			if( m_pItm[i]->m_sex	 == bySex	&& m_pItm[i]->m_race	== byRace &&
				m_pItm[i]->m_grav == byRrav		&& m_pItm[i]->m_class	== byClass &&
				m_pItm[i]->m_type == byType		&& m_pItm[i]->m_number	== byNumber )
			{
				return i;
			}
		}
	}
	return -1;
}

/*
//-----------------------------------------------------------------------------
// Desc: ������ ���� ����
//-----------------------------------------------------------------------------
int	Player_Manager::GetItemSound( int user )
{
int itm_index = GetAttackItemIndex( true, user );
if( itm_index == -1 )
return -1;	
return m_pItm[itm_index]->m_sound;
}
*/

D3DXVECTOR3 Player_Manager::GetVertex( int index, int number )
{
	//return m_pMod[m_pItm[index+m_itm_count].m_mod_index+m_mod_count].m_pVertex[number].m_vertex;
	return m_pMod[m_pItm[index]->m_mod_index]->m_pVertex[number].m_vertex;
}


D3DXVECTOR3	Player_Manager::GetLinkVertex( int index, int number )
{
	return m_pMod[m_pItm[index]->m_mod_index]->m_pVertex[number].m_vertex;
}


//-----------------------------------------------------------------------------
// Desc: NPC�Ӽ� ��Ÿ��
//-----------------------------------------------------------------------------
BYTE Player_Manager::GetNpcBlockType( int _nIndex )
{ 
	// ���̺����������?
	//	if( LIVEITEM_RACE == g_Map.m_NpcTable[_nIndex].race )
	//		return m_NpcAttr[LIVEITEM_INDEX].block;
	// �Ϲ�NPC
	//	return m_NpcAttr[.index].block;
	
	//	return theNPCInfoTable[g_Map.m_NpcTable[_nIndex].code].theBlockType;	 
	//	return theNPCInfoTable[theNPCCodeTable[g_Map.m_NpcTable[_nIndex].code]].theBlockType;	 
	return theNPCInfoTable[theNPCCodeTable[g_Map.m_NpcTable[_nIndex].code]].theSize;	 
}



//-----------------------------------------------------------------------------
// Desc: ���ư��� ȭ�쿡 �׷����� Line Buffer
//-----------------------------------------------------------------------------
void	Player_Manager::CreateArrowLine( char* file_pos )
{
/*	// ������ �б�
char szFilePath[256];

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  #if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
	sprintf( szFilePath, "%s\\Etc\\ArrowLine.msh", file_pos );	
	m_ArrowLine.Mod.Read( szFilePath, true );
	
	  // �ؽ��� �б�
	  sprintf( szFilePath, "%s\\Etc\\ArrowLine.tex", file_pos );	
	  m_ArrowLine.Tex.Read( szFilePath, true );
	  
		///////////////////////////////////////////////////////////////////////////////////////////////////	
		#else  ////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////
		
		  m_ArrowLine.Mod.ReadVF( g_VFEtc, "ArrowLine.msh", true );
		  m_ArrowLine.Tex.ReadVF( g_VFEtc, "ArrowLine.tex", true );
		  
			///////////////////////////////////////////////////////////////////////////////////////////////////	
			#endif ////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////
	*/
	return;
}


//-----------------------------------------------------------------------------
// Desc: ȭ�����
//-----------------------------------------------------------------------------
void Player_Manager::CreateArrow( bool PC, int table_index, BYTE link, D3DXVECTOR3 Curt, D3DXVECTOR3 Dest )
{
	m_FlyWeaponList.Create( PC , table_index , link , Curt , Dest , ARROW_WEAPON_TYPE ); // By wxywxy
}


//-----------------------------------------------------------------------------
// Desc: ȭ��׸���
//-----------------------------------------------------------------------------
void Player_Manager::DisplayArrow()
{
	m_FlyWeaponList.Update();
	m_FlyWeaponList.Render();
}


void Player_Manager::DisplayLineArrow()
{
	
}


//-----------------------------------------------------------------------------
// Desc: ȭ������ϱ�
//-----------------------------------------------------------------------------
void Player_Manager::ClearArrow( bool PC, int table_index )
{
	m_FlyWeaponList.Clear( PC , table_index );
}


//-----------------------------------------------------------------------------
// Desc: ������ �ִϸ��̼ǰ� ����
//-----------------------------------------------------------------------------
D3DXMATRIX Player_Manager::NodeAnimaion( int num, int node, int local, int frame, float time )
{
	D3DXMATRIX	matTemp;
	
	// ���� ������
	int next_frame = frame+1;
	if( g_Pc_Manager.m_pAni[num]->m_ani_frame >= next_frame )
		next_frame = frame;
	
	// ���ʹϿ� -> ��Ʈ���� ( ���� ȸ���� �˾Ƴ��� )
	D3DXQUATERNION	qtRot;
	D3DXQuaternionSlerp( &qtRot, 
		&g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[frame].m_qtRot,
		&g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[next_frame].m_qtRot,
		time );
	D3DXMatrixRotationQuaternion( &matTemp, &qtRot );	
	D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.m_pAni[local]->m_pNode[node].m_matLocal, &matTemp );
	
	// ��ġ��	
	matTemp._41 = (( g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[next_frame].m_vPos.x -
		g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[frame].m_vPos.x )*time )+
		g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[frame].m_vPos.x;			 
	matTemp._42 = (( g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[next_frame].m_vPos.y -
		g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[frame].m_vPos.y )*time )+
		g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[frame].m_vPos.y;
	matTemp._43 = (( g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[next_frame].m_vPos.z -
		g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[frame].m_vPos.z )*time )+
		g_Pc_Manager.m_pAni[num]->m_pNode[node].m_pAnimation[frame].m_vPos.z;
	return matTemp;
}


//-----------------------------------------------------------------------------
// Desc: ���ļ��� ���̺� �迭������, �ʱ�ȭ
//
// Copyright (c) 2003.12.26 By KStaR79
//-----------------------------------------------------------------------------
void	Player_Manager::SetSortInfo( BYTE _byType, int _nIndex )
{
	//if( g_RockClient.GetGameMode() != GMODE_NORMAL )
	//	return;
	/*
	if( m_pSortTable == NULL )
	return;
	
	  for( int i = 0; i < MAX_USER_INTABLE+MAX_NPC_INTABLE; ++i )
	  {
	  if( m_pSortTable[i].byType == 255 )
	  {
	  m_pSortTable[i].byType	= _byType;
	  m_pSortTable[i].Index	= _nIndex;
	  return;
	  }
	  }
	*/
}

void	Player_Manager::ClearSortInfo( BYTE _byType, int _nIndex )
{
	//if( g_RockClient.GetGameMode() != GMODE_NORMAL )
	//	return;
	/*
	if( m_pSortTable == NULL )
	return;
	
	  for( int i = 0; i < MAX_USER_INTABLE+MAX_NPC_INTABLE; ++i )
	  {
	  if( m_pSortTable[i].byType == _byType && m_pSortTable[i].Index == _nIndex )
	  {
	  m_pSortTable[i].Clear();
	  return;
	  }
	  }
	*/
}


//-----------------------------------------------------------------------------
// Desc: ���̺����
//
// ���ڷ� ���� 2���� ��ġ���� ���Ͽ��� �����Ѵ�
//
// Copyright (c) 2003.12.26 By KStaR79
//-----------------------------------------------------------------------------
int	ChrSortProcess( const VOID* arg1, const VOID* arg2 )
{	
	//-----------------------------------------------------------------------------
	// �����͹ޱ�
	//-----------------------------------------------------------------------------
    CHR_SORT_TABLE* p1 = (CHR_SORT_TABLE*)arg1;
    CHR_SORT_TABLE* p2 = (CHR_SORT_TABLE*)arg2;
	
	//-----------------------------------------------------------------------------
	// log
	//-----------------------------------------------------------------------------
	//	#ifdef GAMETYPE_TEST
	//	{
	//		if( p1 == NULL )
	//			MessageBox( NULL, "PcSortProcess : p1 Pointer NULL", "", MB_OK );
	//		if( p2 == NULL )
	//			MessageBox( NULL, "PcSortProcess : p2 Pointer NULL", "", MB_OK );
	//	}
	//	#endif
	
	//-----------------------------------------------------------------------------
	// ���̺� ���� ���� ���
	//-----------------------------------------------------------------------------
	if( p1->byType == 255 )
		return +1;
	else if( p2->byType == 255 )
		return -1;
	
	//-----------------------------------------------------------------------------
	// p1 ��ġ��
	//-----------------------------------------------------------------------------
	float _fX1, _fZ1, _fX2, _fZ2;
	switch( p1->byType )
	{
	case SORT_PC:	// PC
		{
			if( g_Pc.GetUser((short)p1->Index)->m_bIsVisible == false )
				return +1;			
			_fX1	= g_Pc.GetUser((short)p1->Index)->GetPosTM()._41;
			_fZ1	= g_Pc.GetUser((short)p1->Index)->GetPosTM()._43;			
		}
		break;
	case SORT_NPC:	// NPC
		{
			if( g_Map.m_Npc[p1->Index].m_bIsVisible == false )
				return +1;			
			_fX1	= g_Map.m_Npc[p1->Index].GetPosTM()._41;
			_fZ1	= g_Map.m_Npc[p1->Index].GetPosTM()._43;			
		}
		break;
	default:		
		//		#ifdef GAMETYPE_TEST		
		//			MessageBox( NULL, "Chr Sort Error1", "Error", MB_OK );		
		//		#endif
		break;
	}
	
	//-----------------------------------------------------------------------------
	// p2 ��ġ��
	//-----------------------------------------------------------------------------
	switch( p2->byType )
	{
	case SORT_PC:	// PC
		{			
			if( g_Pc.GetUser((short)p2->Index)->m_bIsVisible == false ) 
				return -1;			
			_fX2	= g_Pc.GetUser((short)p2->Index)->GetPosTM()._41;
			_fZ2	= g_Pc.GetUser((short)p2->Index)->GetPosTM()._43;
		}
		break;
	case SORT_NPC:	// NPC
		{	if( g_Map.m_Npc[p2->Index].m_bIsVisible == false )
			return -1;		
		_fX2	= g_Map.m_Npc[p2->Index].GetPosTM()._41;
		_fZ2	= g_Map.m_Npc[p2->Index].GetPosTM()._43;
		}
		break;
	default:		
		//		#ifdef GAMETYPE_TEST		
		//			MessageBox( NULL, "Chr Sort Error1", "Error", MB_OK );		
		//		#endif
		break;
	}
	
	//-----------------------------------------------------------------------------
	// ��ġ���� ��	
	//-----------------------------------------------------------------------------
	float _fD1	= (_fX1*g_Camera.GetAlphaSortRot().x) + (_fZ1*g_Camera.GetAlphaSortRot().z);
	float _fD2	= (_fX2*g_Camera.GetAlphaSortRot().x) + (_fZ2*g_Camera.GetAlphaSortRot().z);
	if( _fD1 < _fD2 )
		return -1;
	return +1;
}


//-----------------------------------------------------------------------------
// Desc: �������̺� ������Ʈ
//
// �����ð��� ����������(CHR_SORT_TIME) ���̺��� �����Ѵ�
//
// Copyright (c) 2003.12.26 By KStaR79
//-----------------------------------------------------------------------------
void	Player_Manager::UpdateSortTable()
{

}

//-----------------------------------------------------------------------------
// Desc: �������̺� �ʱ�ȭ
//
// �������̺��� �ʱ�ȭ�Ѵ�. �ڱⰡ Ÿ�� �ִ� ���� �ϸ� �ȵȴ�(�߿�!!!)
//
// Copyright (c) 2003.12.26 By KStaR79
//-----------------------------------------------------------------------------
void	Player_Manager::InitSortTable()
{
/*
if( m_pSortTable == NULL )
return;

  // ����ð�����
  m_lPreSortTime = g_nowTime;
  
	// �������̺� ����
	for( int i = 0; i < MAX_USER_INTABLE+MAX_NPC_INTABLE; ++i )
	{
	m_pSortTable[i].Clear();
	}
	*/
	return;
}






//-----------------------------------------------------------------------------
// Desc: �ٿ��ڽ��� ����
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
D3DXVECTOR3		Player_Manager::GetBoundMaxPos( LPVOID _pChr )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return g_vZeroVec;
	if( m_pPcFileInfo == NULL )
		return g_vZeroVec;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return g_vZeroVec;
	}
	
	if( pChr->m_bPC == true || 
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		return m_pPcFileInfo[pChr->GetAttrIndex()].BoundInfo.m_max_vertex;		
	}
	else
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			RLG1( "-=*****Error Report*****=- pChr->GetAttrIndex() = %d", pChr->GetAttrIndex() );
			return g_vOneVec;
		}

#ifdef DYNAMIC_RESOURCE_LOADING
		return ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->BoundInfo.m_max_vertex;	
#else	
		return m_pNpcFileInfo[pChr->GetAttrIndex()].BoundInfo.m_max_vertex;
#endif	
	}
	
	return g_vZeroVec;
}


D3DXVECTOR3		Player_Manager::GetBoundMinPos( LPVOID _pChr )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return g_vZeroVec;
	if( m_pPcFileInfo == NULL )
		return g_vZeroVec;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return g_vZeroVec;
	}
	
	if(  pChr->m_bPC == true || 
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )	
	{
		return m_pPcFileInfo[pChr->GetAttrIndex()].BoundInfo.m_min_vertex;
	}
	else
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			RLG1( "-=*****Error Report*****=- pChr->GetAttrIndex() = %d", pChr->GetAttrIndex() );
			return g_vOneVec;
		}

#ifdef DYNAMIC_RESOURCE_LOADING			
		return ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->BoundInfo.m_min_vertex;
#else
		return m_pNpcFileInfo[pChr->GetAttrIndex()].BoundInfo.m_min_vertex;
#endif
	}
	
	return g_vZeroVec;
}

D3DXVECTOR3		Player_Manager::GetCBoundMinPos( LPVOID _pChr )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return g_vZeroVec;
	if( m_pPcFileInfo == NULL )
		return g_vZeroVec;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return g_vZeroVec;
	}
	
	if( pChr->m_bPC == true || 
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		return m_pPcFileInfo[pChr->GetAttrIndex()].CollBoundInfo.m_min_vertex;		
	}
	else
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			RLG1( "-=*****Error Report*****=- pChr->GetAttrIndex() = %d", pChr->GetAttrIndex() );
			return g_vOneVec;
		}
		
#ifdef DYNAMIC_RESOURCE_LOADING
		return ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->CollBoundInfo.m_min_vertex;	
#else
		return m_pNpcFileInfo[pChr->GetAttrIndex()].CollBoundInfo.m_min_vertex;
#endif

	}
	
	return g_vZeroVec;
}

D3DXVECTOR3		Player_Manager::GetCBoundMaxPos( LPVOID _pChr )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return g_vZeroVec;
	if( m_pPcFileInfo == NULL )
		return g_vZeroVec;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return g_vZeroVec;
	}
	
	if(  pChr->m_bPC == true || 
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )	
	{
		return m_pPcFileInfo[pChr->GetAttrIndex()].CollBoundInfo.m_max_vertex;
	}
	else
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			RLG1( "-=*****Error Report*****=- pChr->GetAttrIndex() = %d", pChr->GetAttrIndex() );
			return g_vOneVec;
		}
#ifdef DYNAMIC_RESOURCE_LOADING	
		return ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->CollBoundInfo.m_max_vertex;
#else		
		return m_pNpcFileInfo[pChr->GetAttrIndex()].CollBoundInfo.m_max_vertex;
#endif
	}
	
	return g_vZeroVec;
}

D3DXVECTOR3		Player_Manager::GetMaxBoundVec( LPVOID _pChr )
{
	
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return g_vZeroVec;
	if( m_pPcFileInfo == NULL )
		return g_vZeroVec;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return g_vZeroVec;
	}
	
	if( pChr->m_bPC == true )	
		return pChr->m_maxBoundVec;
	else
		return pChr->m_maxBoundVec;
	
	return g_vZeroVec;
}


D3DXVECTOR3		Player_Manager::GetMinBoundVec( LPVOID _pChr )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return g_vZeroVec;
	if( m_pPcFileInfo == NULL )
		return g_vZeroVec;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return g_vZeroVec;
	}
	
	if( pChr->m_bPC == true )	
		return pChr->m_minBoundVec;
	else
		return pChr->m_minBoundVec;
	
	return g_vZeroVec;
}

void Player_Manager::SetMaxBoundVec( LPVOID _pChr, D3DXVECTOR3 aNewBoundPos )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return;
	if( m_pPcFileInfo == NULL )
		return;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return;
	}
	
	if( pChr->m_bPC == true )			
		pChr->m_maxBoundVec = aNewBoundPos;		
	else
		pChr->m_maxBoundVec = aNewBoundPos;
	
	return;
}


void Player_Manager::SetMinBoundVec( LPVOID _pChr, D3DXVECTOR3 aNewBoundPos )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return;
	if( m_pPcFileInfo == NULL )
		return;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return;
	}
	
	if( pChr->m_bPC == true )			
		pChr->m_minBoundVec = aNewBoundPos;		
	else
		pChr->m_minBoundVec = aNewBoundPos;
	
	return;
}



//-----------------------------------------------------------------------------
// Desc: ���������� �ʱ�ȭ
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
int		Player_Manager::GetInitItmIndex( LPVOID _pChr, int _nIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return 0;
	
	if( m_pPcFileInfo == NULL )
		return 0;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return 0;
	}
	
	
	if(  pChr->m_bPC == true || 
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		//by simwoosung
		if(m_pPcFileInfo[pChr->GetAttrIndex()].pItmIndex == NULL)
			return 0;
		
		return m_pPcFileInfo[pChr->GetAttrIndex()].pItmIndex[_nIndex];
	}
	else
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			RLG1( "-=*****Error Report*****=- pChr->GetAttrIndex() = %d", pChr->GetAttrIndex() );
			return 0;
		}
		
		//by simwoosung
#ifdef DYNAMIC_RESOURCE_LOADING
		if(((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->pItmIndex == NULL)
#else		
		if(m_pNpcFileInfo[pChr->GetAttrIndex()].pItmIndex == NULL)
#endif	
		{
			return 0;
		}
		
#ifdef DYNAMIC_RESOURCE_LOADING
		return ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->pItmIndex[_nIndex];
#else		
		return m_pNpcFileInfo[pChr->GetAttrIndex()].pItmIndex[_nIndex];
#endif

	}
	
	return 1;
}


//-----------------------------------------------------------------------------
// Desc: �������ε��� ����
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
int		Player_Manager::GetItmIndex( LPVOID _pChr, int _nIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return 0;
	if( m_pPcFileInfo == NULL )
		return 0;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return 0;
	}
	
	
	return pChr->m_pItm_index[_nIndex];	
}


//-----------------------------------------------------------------------------
// Desc: �����۰��� ����
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
int		Player_Manager::GetItmCount( LPVOID _pChr )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return 0;
	if( m_pPcFileInfo == NULL )
		return 0;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return 0;
	}
	
	if(  pChr->m_bPC == true || 
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{		
		//		return m_pPcFileInfo[pChr->GetAttrIndex()].nItmCount;
		return c_Max_WearItemType;
	}
	else
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			RLG1( "-=*****Error Report*****=- pChr->GetAttrIndex() = %d", pChr->GetAttrIndex() );
			return 0;
		}
#ifdef DYNAMIC_RESOURCE_LOADING
		return ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->nItmCount;
#else
		return m_pNpcFileInfo[pChr->GetAttrIndex()].nItmCount;
#endif
	}
}


//-----------------------------------------------------------------------------
// Desc: �ؽ����ε��� ����
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
int		Player_Manager::GetTexIndex( LPVOID _pChr, int _nIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return -1;
	}
	
	if( pChr->m_pItm_index[_nIndex] == -1 )
	{
		return -1;
	}
	else if( pChr->m_pItm_index[_nIndex] >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
	{
		return -1;
	}
	
	if( pChr->m_bPC == true )
	{
		if( m_pItm[pChr->m_pItm_index[_nIndex]] )
		{
			return m_pItm[pChr->m_pItm_index[_nIndex]]->m_tex_index;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( m_pItm[pChr->m_pItm_index[_nIndex]] )
		{
			return m_pItm[pChr->m_pItm_index[_nIndex]]->m_tex_index;	
		}
		else
		{
			return -1;
		}
	}
	
	return -1;
}

int Player_Manager::GetBasicTexIndex( LPVOID _pChr, int _nIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return -1;
	}
	
	if( pChr->m_Basic_Itm_index[_nIndex] == -1 )
	{
		return -1;
	}
	else if( pChr->m_Basic_Itm_index[_nIndex] >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
	{
		return -1;
	}
	
	if( m_pItm[pChr->m_Basic_Itm_index[_nIndex]] )
	{
		return m_pItm[pChr->m_Basic_Itm_index[_nIndex]]->m_tex_index;
	}
	else
	{
		return -1;
	}
	
	return -1;
}

int	Player_Manager::GetRideTexIndex( LPVOID _pChr, int _nIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return -1;
	}
	
	if( pChr->m_Ride_Item_index[_nIndex] == -1 )
	{
		return -1;
	}
	else if( pChr->m_Ride_Item_index[_nIndex] >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
	{
		return -1;
	}
	
	if( m_pItm[pChr->m_Ride_Item_index[_nIndex]] )
	{
		return m_pItm[pChr->m_Ride_Item_index[_nIndex]]->m_tex_index;
	}
	else
	{
		return -1;
	}
	
	return -1;
}

//-----------------------------------------------------------------------------
// Desc: ���ε��� ����
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
int		Player_Manager::GetModIndex( LPVOID _pChr, int _nIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return -1;
	}
	
	if( pChr->m_pItm_index[_nIndex] == -1 )
	{
		return -1;
	}
	
	if( pChr->m_pItm_index[_nIndex] >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
	{
		return -1;
	}
	if( m_pItm[pChr->m_pItm_index[_nIndex]] == NULL )
	{
		return -1;
	}
	
	if( pChr->m_bPC == true )
		return m_pItm[pChr->m_pItm_index[_nIndex]]->m_mod_index;
	
	return m_pItm[pChr->m_pItm_index[_nIndex]]->m_mod_index;
}

DWORD    Player_Manager::GetRealItemFileCode( LPVOID _pChr, int _nIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return 0;
	if( m_pPcFileInfo == NULL )
		return 0;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return 0;
	}

	if( pChr->m_pItm_index[_nIndex] == -1 )
	{
		return 0;
	}
	
	if( pChr->m_pItm_index[_nIndex] >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
	{
		return 0;
	}
	if( m_pItm[pChr->m_pItm_index[_nIndex]] == NULL )
	{
		return 0;
	}
	
	if( pChr->m_bPC == true )
		return (DWORD)(m_pItm[pChr->m_pItm_index[_nIndex]]->m_Index);
	
	return (DWORD)(m_pItm[pChr->m_pItm_index[_nIndex]]->m_Index);	
}

BOOL Player_Manager::IsEmblemItem(LPVOID _pChr, DWORD dRealFileIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
	{
		return FALSE;
	}
	
	if( pChr->m_bPC || 
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		if(m_SourceDataManager.FindChrEmblemItemCodePtr(dRealFileIndex))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if(m_SourceDataManager.FindNpcEmblemItemCodePtr(dRealFileIndex))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	return FALSE;
}

BOOL Player_Manager::IsNoApplyAlphaTestItem(LPVOID _pChr, DWORD dRealFileIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
	{
		return FALSE;
	}
	
	if( pChr->m_bPC || 
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		if( dRealFileIndex == 19905542 ||
			dRealFileIndex == 19915542 )
		{
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
	
	return FALSE;
}


int Player_Manager::GetBasicModIndex( LPVOID _pChr, int _nIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return -1;
	}
	
	if( pChr->m_Basic_Itm_index[_nIndex] >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
	{
		return -1;
	}
	if( m_pItm[pChr->m_Basic_Itm_index[_nIndex]] == NULL )
	{
		return -1;
	}
	
	return m_pItm[pChr->m_Basic_Itm_index[_nIndex]]->m_mod_index;
}

int Player_Manager::GetRideModIndex( LPVOID _pChr, int _nIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return -1;
	}
	
	if( pChr->m_Ride_Item_index[_nIndex] >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
	{
		return -1;
	}
	if( m_pItm[pChr->m_Ride_Item_index[_nIndex]] == NULL )
	{
		return -1;
	}
	
	return m_pItm[pChr->m_Ride_Item_index[_nIndex]]->m_mod_index;
}


//-----------------------------------------------------------------------------
// Desc: ���ؽ����� ����
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
int				Player_Manager::GetVtxCount( LPVOID _pChr, int _nIndex, BYTE _byType )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return 0;
	if( m_pPcFileInfo == NULL )
		return 0;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return 0;
	}
	
	int nItmIndex;
	if(  pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		switch( _byType )
		{
		case 0:
			{			
				// ���� �������̸� By wxywxy
				if( pChr->m_pItm_index[ _nIndex ] == - 1 )
					return 0;
				
				nItmIndex = pChr->m_pItm_index[_nIndex];				
			}
			break;
		case 1:
			{
				if( pChr->m_pUserInfo )
				{
					nItmIndex = pChr->m_pUserInfo->ArmorInfo.index[_nIndex];
					
					if( nItmIndex == -1 )
						return 0;
				}
				else
				{
					return 0;
				}
			}
			break;
		}
		
		if( nItmIndex >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
		{
			return 0;
		}
		
		if( m_pItm[nItmIndex] && m_pMod[m_pItm[nItmIndex]->m_mod_index] )
		{
			return m_pMod[m_pItm[nItmIndex]->m_mod_index]->m_header.m_num_vertex; 
		}
		else
		{
			return 0;
		}		
	}
	
	nItmIndex = pChr->m_pItm_index[_nIndex];//+m_itm_count;

	if( pChr->m_pItm_index[_nIndex] == -1 )
	{
		return 0;
	}

	return m_pMod[m_pItm[nItmIndex]->m_mod_index]->m_header.m_num_vertex;
}

int Player_Manager::GetBasicVtxCount( LPVOID _pChr, int _nIndex, BYTE _byType )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return 0;
	if( m_pPcFileInfo == NULL )
		return 0;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return 0;
	}
	
	int nItmIndex;
	if(  pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		switch( _byType )
		{
		case 0:
			nItmIndex = pChr->m_Basic_Itm_index[_nIndex];
			break;
		case 1:			
			{
				if( pChr->m_pUserInfo )
				{
					nItmIndex = pChr->m_pUserInfo->ArmorInfo.index[_nIndex];
				}
				else
				{
					return 0;
				}
			}
			break;
		}
		
		if( nItmIndex == -1 )
		{
			return 0;
		}
		
		if( nItmIndex >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
		{
			return 0;
		}
		
		if( m_pItm[nItmIndex] && m_pMod[m_pItm[nItmIndex]->m_mod_index] )
		{
			return m_pMod[m_pItm[nItmIndex]->m_mod_index]->m_header.m_num_vertex; 
		}
		else
		{
			return 0;
		}		
		
		return m_pMod[m_pItm[nItmIndex]->m_mod_index]->m_header.m_num_vertex; 
	}
	
	nItmIndex = pChr->m_Basic_Itm_index[_nIndex];//+m_itm_count;

	return m_pMod[m_pItm[nItmIndex]->m_mod_index]->m_header.m_num_vertex;
}

int Player_Manager::GetRideVtxCount( LPVOID _pChr, int _nIndex, BYTE _byType )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return 0;
	if( m_pPcFileInfo == NULL )
		return 0;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return 0;
	}
	
	int nItmIndex;
	
	if( nRideState_Ride == pChr->m_State.nRideState )
	{
		nItmIndex = pChr->m_Ride_Item_index[_nIndex];
		
		if( nItmIndex >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
		{
			return 0;
		}
		
		if( m_pItm[nItmIndex] && m_pMod[m_pItm[nItmIndex]->m_mod_index] )
		{
			return m_pMod[m_pItm[nItmIndex]->m_mod_index]->m_header.m_num_vertex; 
		}
		else
		{
			return 0;
		}		
		
		return m_pMod[m_pItm[nItmIndex]->m_mod_index]->m_header.m_num_vertex; 
	}
	
	return 0;
}

//-----------------------------------------------------------------------------
// Desc: �����۸��͸��� ����
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
D3DMATERIAL9	Player_Manager::GetItmMaterial( LPVOID _pChr, int _nIndex )
#else
D3DMATERIAL8	Player_Manager::GetItmMaterial( LPVOID _pChr, int _nIndex )
#endif // DIRECT_VERSION_9_MJH
{
	Character*	pChr = (Character*)_pChr;
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 nmaterial = m_pItm[0]->m_Material;
#else
	D3DMATERIAL8 nmaterial = m_pItm[0]->m_Material;
#endif // DIRECT_VERSION_9_MJH

	if( pChr == NULL )
	{
		return nmaterial;
	}
	if( m_pPcFileInfo == NULL )
	{
		return nmaterial;
	}
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return nmaterial;
	}
	
	int nItmIndex = 0;
	
	if( pChr->m_bPC == true )
	{
		nItmIndex = pChr->m_pItm_index[_nIndex];
	}
	else
	{
		nItmIndex = pChr->m_pItm_index[_nIndex];
	}

	if( -1 == nItmIndex )
	{
		return nmaterial;
	}	

	return m_pItm[nItmIndex]->m_Material;
}


//-----------------------------------------------------------------------------
// Desc: �߱�ȿ������������ �˻�
//
// Copyright (c) 2004.1.9 By KStaR79
//-----------------------------------------------------------------------------
bool			Player_Manager::GetEmitterItm( LPVOID _pChr, int _nIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return false;
	if( m_pPcFileInfo == NULL )
		return false;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return false;
	}
	
	int nItmIndex;
	if( true == pChr->m_bPC )	
	{
		nItmIndex = pChr->m_pItm_index[_nIndex];
	}
	else	
	{
		nItmIndex = pChr->m_pItm_index[_nIndex];//+m_itm_count;
	}

	if( pChr->m_pItm_index[_nIndex] == -1 )
	{
		return false;
	}

	if( 255 != m_pItm[nItmIndex]->m_number )
	{
		switch( m_pItm[nItmIndex]->m_class )
		{
		case 2:	// ����
			return true;
		case 4:	// ���
			return true;
		case 5:	// ����
			return true;
		case 6:	// �۷���
			return true;
		}
	}
	return false;
}


int Player_Manager::GetArmorIndex( LPVOID _pChr, DWORD _dwNode )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return 0;
	if( m_pPcFileInfo == NULL )
		return 0;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return false;
	}
	
	if( pChr->m_bPC == false && pChr->m_State.nCharacterType != nCharacterTypePlayerNPC )
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			RLG1( "-=*****Error Report*****=- pChr->GetAttrIndex() = %d", pChr->GetAttrIndex() );
			return 0;
		}
	}
	
	//-----------------------------------------------------------------------------
	// ��忡 ���� ��ũ�Ǵ� ��
	//-----------------------------------------------------------------------------
	switch( _dwNode )
	{
	case 0:		// ������
		{
			if(  pChr->m_bPC == true || 
				( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
			{
				_dwNode = pChr->m_pUserInfo->ArmorInfo.RHand;
			}
			else
			{			
#ifdef DYNAMIC_RESOURCE_LOADING
				_dwNode = ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->ArmorInfo.RHand;	
#else
				_dwNode = m_pNpcFileInfo[pChr->GetAttrIndex()].ArmorInfo.RHand;		
#endif			
			}
		}
		break;
	case 1:		// �޼�
		{
			if(  pChr->m_bPC == true ||
				( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
			{
				_dwNode = pChr->m_pUserInfo->ArmorInfo.LHand;			
			}
			else
			{
#ifdef DYNAMIC_RESOURCE_LOADING
				_dwNode = ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->ArmorInfo.LHand;
#else
				_dwNode = m_pNpcFileInfo[pChr->GetAttrIndex()].ArmorInfo.LHand;
#endif
			}
		}
		break;
	case 2:		// ��Ÿ
		{
			if(  pChr->m_bPC == true || 
				( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
			{
				_dwNode = pChr->m_pUserInfo->ArmorInfo.Temp1;
			}
			else
			{
#ifdef DYNAMIC_RESOURCE_LOADING
				_dwNode = ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->ArmorInfo.Temp1;
#else
				_dwNode = m_pNpcFileInfo[pChr->GetAttrIndex()].ArmorInfo.Temp1;
#endif
			}
		}
		break;
	case 3:		// ��Ÿ
		{
			if(  pChr->m_bPC == true ||
				( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
			{
				_dwNode = pChr->m_pUserInfo->ArmorInfo.Temp2;
			}
			else
			{
#ifdef DYNAMIC_RESOURCE_LOADING
				_dwNode = ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->ArmorInfo.Temp2;
#else
				_dwNode = m_pNpcFileInfo[pChr->GetAttrIndex()].ArmorInfo.Temp2;
#endif
			}
		}
		break;
		// ���ʾ��
	case 4:
		_dwNode = 13;
		break;
	case 5:
		_dwNode = 37;
		break;
	case 6:
		_dwNode = 7;
		break;
	}
	return _dwNode;
}

//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼�
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
D3DXMATRIX		Player_Manager::GetAniLocalTM( LPVOID _pChr, int _nNumber, int _nNode )
{
#ifdef DIRECT_VERSION_9_MJH
	Character*	pChr = (Character*)_pChr;
	if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{}
#else
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return NULL;
	if( m_pPcFileInfo == NULL )
		return NULL;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return NULL;
	}
#endif // DIRECT_VERSION_9_MJH

	int nAniNum;
	if( pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		nAniNum = m_PcAni[pChr->GetAttrIndex()][_nNumber].ani_number;
	}
	else
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			RLG1( "-=*****Error Report*****=- pChr->GetAttrIndex() = %d", pChr->GetAttrIndex() );
			nAniNum = 0;
		}
		else
		{
#ifdef DYNAMIC_RESOURCE_LOADING
			nAniNum = ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->pAniInfo[_nNumber].ani_number;
#else			
			nAniNum = m_pNpcFileInfo[pChr->GetAttrIndex()].pAniInfo[_nNumber].ani_number;//+m_ani_count;
#endif
		}
	}
	return m_pAni[nAniNum]->m_pNode[_nNode].m_matLocal;
}


D3DXQUATERNION	Player_Manager::GetAniQuaternion( int _nNumber, int _nNode, int _nFrame )
{
	return m_pAni[_nNumber]->m_pNode[_nNode].m_pAnimation[_nFrame].m_qtRot;
}


D3DXVECTOR3		Player_Manager::GetAniPos( int _nNumber, int _nNode, int _nFrame )
{
	return m_pAni[_nNumber]->m_pNode[_nNode].m_pAnimation[_nFrame].m_vPos;
}


int				Player_Manager::GetSoundkey( LPVOID _pChr, int _nAniIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return -1;
	}
	
	if(  pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )		
	{
		return m_PcAni[pChr->GetAttrIndex()][_nAniIndex].sound_key;
	}
	else
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			RLG1( "-=*****Error Report*****=- pChr->GetAttrIndex() = %d", pChr->GetAttrIndex() );
			return -1;
		}

#ifdef DYNAMIC_RESOURCE_LOADING
		return ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->pAniInfo[_nAniIndex].sound_key;	
#else		
		return m_pNpcFileInfo[pChr->GetAttrIndex()].pAniInfo[_nAniIndex].sound_key;
#endif
	}
}


int				Player_Manager::GetAniNumber( LPVOID _pChr, int _nAniIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return -1;
	}
	
	int ani_num = 0;
	
	if(  pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		ani_num = m_PcAni[pChr->GetAttrIndex()][_nAniIndex].ani_number;
	}
	else
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			return -1;
		}
		
		///-- �迭 üũ
		if( _nAniIndex > (c_Npc_AniIndex_Count - 1) )
		{
			int nAttr = pChr->GetAttrIndex();			
#ifdef ROCKCLIENT_DEV
			ClientFilelog("NPC Name = %s Ani Index= %d", g_Map.m_NpcTable[nAttr].szName, _nAniIndex);
#endif
			return -1;
		}		
		
#ifdef DYNAMIC_RESOURCE_LOADING
		if( ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->pAniInfo[_nAniIndex].ani_number == -1 )
#else		
		if( m_pNpcFileInfo[pChr->GetAttrIndex()].pAniInfo[_nAniIndex].ani_number == -1 )
#endif	
		{
			ani_num = -1;
		}
		else
		{
#ifdef DYNAMIC_RESOURCE_LOADING
			ani_num = ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->pAniInfo[_nAniIndex].ani_number;
#else
			ani_num = m_pNpcFileInfo[pChr->GetAttrIndex()].pAniInfo[_nAniIndex].ani_number;//+m_ani_count-1;
#endif	
		}
	}	
	
	if( ani_num >= c_Chr_Ani_Count+c_Npc_Ani_Count )
	{
		RLG1( "Error!!! Nothing Animation FileName : %d", ani_num );
		
		BYTE bType = pChr->m_bPC;	
		int nAttr = pChr->GetAttrIndex();
		char error[256];
		sprintf( error, "Error!!! Nothing Animation FileName : %d", ani_num );
		ClientFilelog("bNPC = %d Attr = %d Ani Index= %d", bType, nAttr, _nAniIndex);
		MessageBox( g_RockClient.GetApp(), error, "Error", NULL );
	}
	else if( ani_num == -1 )
	{
		//		char error[256];
		
		//		if( pChr->m_bPC )
		//		{		
		//			sprintf( error, "Error!!! Nothing Animation Index!!! pChr->GetAttrIndex() = %d, _nAniIndex = %d", pChr->GetAttrIndex(), _nAniIndex );			
		//			RLGS( error );			
		//		}
		//		else
		//		{
		//			sprintf( error, "Error!!! Nothing Animation Link!!! NPC Code = %d, _nAniIndex = %d", g_Map.m_NpcTable[pChr->m_nTableIndex].code, _nAniIndex );			
		//			RLGS( error );			
		//		}
		
		//		MessageBox( g_RockClient.GetApp(), error, "Error", NULL );
	}
	
	return ani_num;
}


int				Player_Manager::GetEndFrame( LPVOID _pChr, int _nAniIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return -1;
	}
	
	int nAniNum;
	if( pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )			
	{
		nAniNum = m_PcAni[pChr->GetAttrIndex()][_nAniIndex].ani_number;
	}
	else
	{
		if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		{
			RLG1( "-=*****Error Report*****=- pChr->GetAttrIndex() = %d", pChr->GetAttrIndex() );
			nAniNum = 0;
		}
		
#ifdef DYNAMIC_RESOURCE_LOADING
		nAniNum = ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->pAniInfo[_nAniIndex].ani_number;
#else
		nAniNum = m_pNpcFileInfo[pChr->GetAttrIndex()].pAniInfo[_nAniIndex].ani_number;//+m_ani_count;
#endif
	}
	
	if( nAniNum == -1 )
	{
		char error[256];
		
		if( pChr->m_bPC ||
			( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
		{		
			sprintf( error, "Error!!! Nothing Animation Index!!! pChr->GetAttrIndex() = %d, _nAniIndex = %d", pChr->GetAttrIndex(), _nAniIndex );			
			RLGS( error );			
		}
		else
		{
			sprintf( error, "Error!!! Nothing Animation Link!!! NPC Code = %d, _nAniIndex = %d", g_Map.m_NpcTable[pChr->m_nTableIndex].code, _nAniIndex );			
			RLGS( error );			
		}
		
		//		MessageBox( g_RockClient.GetApp(), error, "Error", NULL );
		
		return 30;			// �ӽ÷� �뷫 30������ ����...^ ^*
	}
	else
	{
		if( nAniNum >= c_Chr_Ani_Count+c_Npc_Ani_Count )
		{
			RLG1( "Error!!! Nothing Animation FileName : %d", nAniNum );
			
			char error[256];
			sprintf( error, "Error!!! Nothing Animation FileName : %d", nAniNum );
			MessageBox( g_RockClient.GetApp(), error, "Error", NULL );
		}
		
		if( m_pAni[nAniNum] )
		{
			return m_pAni[nAniNum]->m_ani_frame;
		}
		else
		{
			return 30;
		}
	}	
}



int Player_Manager::GetImpactFrame( LPVOID _pChr, int _nAniIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	
	if( pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )			
	{
		int nImpactFrame = m_PcAni_Act[pChr->GetAttrIndex()][_nAniIndex].Impact_frame;
		int nEndFrame = GetEndFrame( _pChr, _nAniIndex );
		
		if( nImpactFrame + c_MotionBlendHalfFrame > nEndFrame )
		{
			nImpactFrame = nEndFrame - c_MotionBlendHalfFrame;
		}		
		return nImpactFrame;	
	}
	else
	{
		DWORD dFileCode = g_Pc_Manager.GetNPCRealFileCode( g_Map.m_NpcTable[ pChr->m_nTableIndex ].code );	
		int nNPCImpactFrame = g_Pc_Manager.m_NPCImpactTable.GetFrame( dFileCode , 2 );	
		
		return nNPCImpactFrame;
	}
	
	return -1;
}

int Player_Manager::GetRangeStartFrame( LPVOID _pChr, int _nAniIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	
	if(  pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )			
	{
		return m_PcAni_Act[pChr->GetAttrIndex()][_nAniIndex].range_startframe;
	}
	
	return -1;
}

int Player_Manager::GetRangeEndFrame( LPVOID _pChr, int _nAniIndex )
{
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return -1;
	if( m_pPcFileInfo == NULL )
		return -1;
	
	if(  pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )			
	{
		return m_PcAni_Act[pChr->GetAttrIndex()][_nAniIndex].range_endframe;
	}
	
	return -1;
}


//-----------------------------------------------------------------------------
// Desc: ����
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
int				Player_Manager::GetSoundNumber( LPVOID _pChr, int _nAniIndex )
{
	Character*	pChr = (Character*)_pChr;
	
	if( pChr == NULL )
		return -1;
	
	if( m_pPcFileInfo == NULL )
		return -1;
	
	if( 
#ifdef DYNAMIC_RESOURCE_LOADING
		m_pNpcFileInfo.size() == 0	
#else
		m_pNpcFileInfo == NULL
#endif			
	  )
	{
		return -1;
	}	
	
	int AttrIndex = pChr->GetAttrIndex();
	
	if( -1 == AttrIndex )
		return -1;	
	
	if( pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )		
	{
		return m_PcAni[ AttrIndex ][_nAniIndex].sound_number;
	}
	
#ifdef DYNAMIC_RESOURCE_LOADING	
	return ((CHR_FILE_INFO*)m_pNpcFileInfo[ AttrIndex ])->pAniInfo[_nAniIndex].sound_number;	
#else
	return m_pNpcFileInfo[ AttrIndex ].pAniInfo[_nAniIndex].sound_number;
#endif
}


//-----------------------------------------------------------------------------
// Desc: ĳ���ͽ�����
//
// Copyright (c) 2003.12.30 By KStaR79
//-----------------------------------------------------------------------------
D3DXVECTOR3		Player_Manager::GetChrScale( LPVOID _pChr )
{
	D3DXVECTOR3		scale_vec = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	
	Character*	pChr = (Character*)_pChr;
	if( pChr == NULL )
		return scale_vec;
	if( m_pPcFileInfo == NULL )
		return scale_vec;
	else if( 
#ifdef DYNAMIC_RESOURCE_LOADING
			m_pNpcFileInfo.size() == 0	
#else
			m_pNpcFileInfo == NULL
#endif			
		   )
	{
		return scale_vec;
	}
	
	if( pChr->GetAttrIndex() >= the_sNpc_SetData.total_npc_num )
		return scale_vec;
	
	if(  pChr->m_bPC == true ||
		( pChr->m_bPC == false && pChr->m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		return m_pPcFileInfo[pChr->GetAttrIndex()].vScale;
	}

#ifdef DYNAMIC_RESOURCE_LOADING
	return ((CHR_FILE_INFO *)m_pNpcFileInfo[pChr->GetAttrIndex()])->vScale;
#else
	return m_pNpcFileInfo[pChr->GetAttrIndex()].vScale;
#endif
}


//-----------------------------------------------------------------------------
// Desc: CHR ���� ����ü �ʱ�ȭ, �ı���
//
// �ʱ�ȭ, �ı���(pAniInfo, pItmIndex ������. �޸��Ҵ� üũ ���ؾ� �Ѵ�)
//
// Copyright (c) 2003.12.29 By KStaR79
//-----------------------------------------------------------------------------
void	CHR_FILE_INFO::Clear()
{
	fAniTime	= 0.0f;
	fStep		= 0.0f;
	vScale		= D3DXVECTOR3( 1.0f, 1.0f, 1.0f );	
	pAniInfo	= NULL;
	nItmCount	= 0;
	pItmIndex	= NULL;
	ArmorInfo.Clear();
	
	minBoundVec	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	
	maxBoundVec = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	
}

void	CHR_FILE_INFO::Destroy()
{
	
	
	// �ִϸ��̼�
	SAFE_DELETE_ARRAY( pAniInfo );	
	// �������ε���
	SAFE_DELETE_ARRAY( pItmIndex );
	
	// �ʱ�ȭ
	Clear();
}


//-----------------------------------------------------------------------------
// Desc: CHR �����б�
//
// brv_chr������ �д´�. PC�� NPCŸ�Կ� ���� �����ؼ� �о���Ѵ�
//
// Copyright (c) 2003.12.29 By KStaR79
//-----------------------------------------------------------------------------
bool CHR_FILE_INFO::FileRead( char* szFilePath, BYTE byType )
{
	// �����о �޸��Ҵ�
	FILE*	file;
	if( NULL == (file = fopen(szFilePath, "rb")) )
	{
		CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", szFilePath );			
		return false;
	}
	
	// �⺻����
	fread( &fAniTime,		sizeof(float),			1,	file );
	fread( &fStep,			sizeof(float),			1,	file );	
	fread( &vScale,			sizeof(D3DXVECTOR3),	1,	file );
	fread( &AlphaInfo,		sizeof(ALPHA_INFO),		1,	file );
	fread( &BoundInfo,		sizeof(BOUND_INFO),		1,	file );
	
	float fTemp = BoundInfo.m_min_vertex.z;
	BoundInfo.m_min_vertex.z = BoundInfo.m_max_vertex.z;
	BoundInfo.m_max_vertex.z = fTemp;
	
	// �ִϸ��̼�
	switch( byType )
	{
	case SORT_NPC:	// NPC
		{
			pAniInfo = SAFE_NEW_ARRAY( ANI_INFO , c_Npc_AniIndex_Count );			
			fread( pAniInfo, sizeof(ANI_INFO), c_Npc_AniIndex_Count, file );				
		}
		break;
	case SORT_PC:	// PC
		{
			ANI_INFO	TempAniInfo[100];
			fread( TempAniInfo, sizeof(ANI_INFO), 100, file );				
		}
		break;
	}
	
	// ������
	fread( &nItmCount, sizeof(int), 1, file );
	pItmIndex = SAFE_NEW_ARRAY( int , nItmCount );	
	fread( pItmIndex, sizeof(int), nItmCount, file );
	
	// ���⸵ũ����(Only Npc Use)
	if( byType == SORT_NPC )
		fread( &ArmorInfo, sizeof(ARMOR_INFO), 1, file );
	
	// ���������ʹݱ�
	fclose( file );
	
	int i;
	int j;
	int *pTempitmIndex;
	
	bool IsFind_Itm = false;
	
	if( byType == SORT_PC ) 
	{
		pTempitmIndex = SAFE_NEW_ARRAY( int , nItmCount );		
		
		for( j = 0; j < nItmCount; j++ )
		{				
			IsFind_Itm = false;
			
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == pItmIndex[j] )
				{
					pItmIndex[j] = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					IsFind_Itm = true;
					
					if( g_Pc_Manager.theChrItm[i].m_type == 0 )			// ��ݽ�
					{
						pTempitmIndex[n_Wear_CoatArmor] = i;				
					}
					else if( g_Pc_Manager.theChrItm[i].m_type == 1 )		// �Ϲݽ�
					{
						pTempitmIndex[n_Wear_PantsArmor] = i;				
					}
					else if( g_Pc_Manager.theChrItm[i].m_type == 2 )		// �尩
					{
						pTempitmIndex[n_Wear_Gloves] = i;				
					}
					else if( g_Pc_Manager.theChrItm[i].m_type == 3 )		// ����
					{
						pTempitmIndex[n_Wear_Shoes] = i;				
					}
					else if( g_Pc_Manager.theChrItm[i].m_type == 7 )		// ��
					{
						pTempitmIndex[n_Wear_Face] = i;				
					}
					else if( g_Pc_Manager.theChrItm[i].m_type == 8 )		// ���
					{
						pTempitmIndex[n_Wear_Hair] = i;				
					}				
					break;
				}
			}
			
			if( IsFind_Itm == false )
			{
				CloseWnd_Error( "!%d.Itm ������ ã�� �� �����ϴ�.", pItmIndex[j] );				
				return false;
			}
		}
		
		for( i = 0; i < nItmCount; i++ )
		{
			pItmIndex[i] = pTempitmIndex[i]; 
		}
	}
	/*	else if( byType == SORT_NPC )
	{
	for( j = 0; j < nItmCount; j++ )
	{					
	for( i = 0; i < CHR_ITM_COUNT + NPC_ITM_COUNT; i++ )
	{					
				if( g_Pc_Manager.m_pItm[i].m_Index == pItmIndex[j] )
				{
				pItmIndex[j] = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
				}
				}
				}		
				} 
				
				  // �ִϸ��̼�
				  switch( byType )
				  {
				  case SORT_NPC:	// NPC
				  {
				  for( j = 0; j < NPC_ANI_INDEX_COUNT; j++ ) 
				  {				
				  for( i = 0; i < CHR_ANI_COUNT + NPC_ANI_COUNT; i++ )
				  {					
				  if( g_Pc_Manager.m_pAni[i].m_Index == pAniInfo[j].ani_number )
				  {
				  pAniInfo[j].ani_number = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
				  break;
				  }
				  }
				  }
				  }
				  break;
	} */
	
	return true;
}

//-----------------------------------------------------------------------------
// Desc: CHR �����б�
// brv_chr������ �д´�. PC�� NPCŸ�Կ� ���� �����ؼ� �о���Ѵ�
//-----------------------------------------------------------------------------
bool CHR_FILE_INFO::FileReadVF( CVFileSystem* pVF, char* szFilePath, BYTE byType )	/// sooree pack
{
	// ���������� ����
	VFileHandle* pFH = pVF->OpenFile( szFilePath );
	if( pFH == NULL ) 
	{
		CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", szFilePath );			
		return false;
	}
	
	// �⺻����
	pFH->Read( &fAniTime,		sizeof(float) );
	pFH->Read( &fStep,			sizeof(float) );	
	pFH->Read( &vScale,			sizeof(D3DXVECTOR3) );
	pFH->Read( &AlphaInfo,		sizeof(ALPHA_INFO) );
	pFH->Read( &BoundInfo,		sizeof(BOUND_INFO) );
	
	ReSetBoundBox(BoundInfo.m_min_vertex, BoundInfo.m_max_vertex);	
	
	// �ִϸ��̼�
	switch( byType )
	{
	case SORT_NPC:	// NPC
		{
			pAniInfo = SAFE_NEW_ARRAY( ANI_INFO , c_Npc_AniIndex_Count );			
			pFH->Read( pAniInfo, sizeof(ANI_INFO) * c_Npc_AniIndex_Count );
		}
		break;
	case SORT_PC:	// PC
		{
			ANI_INFO	TempAniInfo[100];
			pFH->Read( TempAniInfo, sizeof(ANI_INFO) * 100 );				
		}
		break;
	}
	
	// ������
	pFH->Read( &nItmCount, sizeof(int) );
	pItmIndex = SAFE_NEW_ARRAY( int , nItmCount );	
	
	pFH->Read( pItmIndex, sizeof(int) * nItmCount );
	
	D3DXVECTOR3 vTemp;
	
	// ���⸵ũ����(Only Npc Use)
	if( byType == SORT_NPC )
	{
		pFH->Read( ArmorInfo.index, sizeof(int) * 4 );
		pFH->Read( &ArmorInfo.RHand, sizeof(int) );
		pFH->Read( &ArmorInfo.LHand, sizeof(int) );		
		pFH->Read( &ArmorInfo.Temp1, sizeof(int) );
		pFH->Read( &ArmorInfo.Temp2, sizeof(int) );
		pFH->Read( &vTemp, sizeof(D3DXVECTOR3) );	
	}
	
	if( byType == SORT_NPC )
	{
		///-- ���� �߰��� ���� �ҷ�����	
		if( pFH->GetFsize() != pFH->GetFCurPos() )
		{
			pFH->Read( &CollBoundInfo.m_min_vertex,	sizeof(D3DXVECTOR3) );
			pFH->Read( &CollBoundInfo.m_max_vertex,	sizeof(D3DXVECTOR3) );
			
			ReSetBoundBox(CollBoundInfo.m_min_vertex, CollBoundInfo.m_max_vertex);	
		}
	}
	
	// ���� ������ �ݱ�
	pVF->CloseFile( pFH );
	
	int i;
	int j;
	int *pTempitmIndex = NULL;
	
	bool IsFind_Itm = false;
	
	if( byType == SORT_PC ) 
	{
		pTempitmIndex = SAFE_NEW_ARRAY( int , nItmCount );		
		memset( pTempitmIndex, 0, sizeof(int) * nItmCount );
		
		for( j = 0; j < nItmCount; j++ )
		{		
			IsFind_Itm = false;
			
			for( i = 0; i < c_Chr_Itm_Count; i++ )
			{					
				if( g_Pc_Manager.theChrItm[i].m_Index == pItmIndex[j] )
				{
					pItmIndex[j] = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
					IsFind_Itm = true;
					
					if( g_Pc_Manager.theChrItm[i].m_type == 0 )			// ��ݽ�
					{
						pTempitmIndex[n_Wear_CoatArmor] = i;				
					}
					else if( g_Pc_Manager.theChrItm[i].m_type == 1 )		// �Ϲݽ�
					{
						pTempitmIndex[n_Wear_PantsArmor] = i;				
					}
					else if( g_Pc_Manager.theChrItm[i].m_type == 2 )		// �尩
					{
						pTempitmIndex[n_Wear_Gloves] = i;				
					}
					else if( g_Pc_Manager.theChrItm[i].m_type == 3 )		// ����
					{
						pTempitmIndex[n_Wear_Shoes] = i;				
					}
					else if( g_Pc_Manager.theChrItm[i].m_type == 7 )		// ��
					{
						pTempitmIndex[n_Wear_Face] = i;				
					}
					else if( g_Pc_Manager.theChrItm[i].m_type == 8 )		// ���
					{
						pTempitmIndex[n_Wear_Hair] = i;				
					}				
					else if( g_Pc_Manager.theChrItm[i].m_type == 9 )		// ����
					{
						pTempitmIndex[n_Wear_Serve] = i;				
					}				
					
					break;
				}
			}
			
			if( IsFind_Itm == false )
			{
				CloseWnd_Error( "!%d.Itm ������ ã�� �� �����ϴ�.", pItmIndex[j] );				
				return false;
			}
		}
		
		for( i = 0; i < nItmCount; i++ )
		{
			pItmIndex[i] = pTempitmIndex[i];
		}
		
		SAFE_DELETE_ARRAY( pTempitmIndex );
		
	}
	
	return true;
}

void CHR_FILE_INFO::ReSetBoundBox(D3DXVECTOR3 & min, D3DXVECTOR3 & max)
{
	float fTemp = 0.0f;
	
	if(min.x > max.x)
	{
		fTemp = min.x;
		min.x = max.x;
		max.x = fTemp;
	}
	
	if(min.y > max.y)
	{
		fTemp = min.y;
		min.y = max.y;
		max.y = fTemp;
	}
	
	if(min.z > max.z)
	{
		fTemp = min.z;
		min.z = max.z;
		max.z = fTemp;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// "Light.Set" Read Method
// Ŭ���̾�Ʈ ����Ʈ ���� ������ ����...�׷��� �� �밡��!!!  
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
bool Player_Manager::ReadLightSet( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos);
	sprintf( file_name, "\\Env\\light.set");
	strcat( file_path, file_name);		
	
	FILE*	file;
	file = fopen( file_path, "rt" );
	if( file == NULL )
		return false;
	
	char* token;				// ��ū
	char string [256];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ			
	char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
	int	string_cnt = 1;
	
	
	// �� ������ �б�
	if( fgets( string, 256, file ) == NULL )				
		return false;				
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( fgets( string, 256 , file ) == NULL )		
			{
				bRead = false;
				break;
			}
		}
		
		// �ε��� 
		token = strtok( string, splitter );
		if( token == NULL )			
			break;	
		
		
		switch( string_cnt )
		{
		case 1:
			theCharacter_Light.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Light.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Light.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Light.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Light.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Light.Ambient.b = atof(token);
			break;
		case 2:
			theObject_Light.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theObject_Light.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theObject_Light.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theObject_Light.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theObject_Light.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theObject_Light.Ambient.b = atof(token);
			break;
		case 3:
			theTerrain_Light.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Light.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Light.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Light.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Light.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Light.Ambient.b = atof(token);
			break;
		case 4:
			theCharacter_Material.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Diffuse.a = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Ambient.b = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Ambient.a = atof(token);
			break;
		case 5:
			theObject_Material.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Diffuse.a = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Ambient.b = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Ambient.a = atof(token);
			break;
		case 6:
			theTerrain_Material.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Diffuse.a = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Ambient.b = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Ambient.a = atof(token);
			break;
		case 7:
			theFog_Start = atof(token);
			token = strtok( NULL, splitter );
			theFog_End = atof(token);
			token = strtok( NULL, splitter );
			theFog_Color.x = atof(token);
			token = strtok( NULL, splitter );
			theFog_Color.y = atof(token);
			token = strtok( NULL, splitter );
			theFog_Color.z = atof(token);			
			break;
		default:
			break;
		}
		
		++string_cnt;
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( fgets( string, 256 , file ) == NULL )		
			bRead = false;			
	}
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	fclose( file );	
	
	return true;
}


bool Player_Manager::ReadLightSetVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "light.set" );	
	
	VFileHandle* pFH = g_VFEnv.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
	char* token;				// ��ū
	char string [256];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ			
	char splitter[] = " \n\t";	// ������ : /, ĳ��������, �� 
	int	string_cnt = 1;
	
	
	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == NULL )
	{
		g_VFEnv.CloseFile( pFH );
		return false; 
	}	
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == NULL )
			{
				bRead = false;
				break;
			}
		}
		
		// �ε��� 
		token = strtok( string, splitter );
		if( token == NULL )			
			break;	
		
		
		switch( string_cnt )
		{
		case 1:
			theCharacter_Light.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Light.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Light.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Light.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Light.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Light.Ambient.b = atof(token);
			break;
		case 2:
			theObject_Light.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theObject_Light.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theObject_Light.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theObject_Light.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theObject_Light.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theObject_Light.Ambient.b = atof(token);
			break;
		case 3:
			theTerrain_Light.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Light.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Light.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Light.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Light.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Light.Ambient.b = atof(token);
			break;
		case 4:
			theCharacter_Material.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Diffuse.a = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Ambient.b = atof(token);
			token = strtok( NULL, splitter );
			theCharacter_Material.Ambient.a = atof(token);
			break;
		case 5:
			theObject_Material.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Diffuse.a = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Ambient.b = atof(token);
			token = strtok( NULL, splitter );
			theObject_Material.Ambient.a = atof(token);
			break;
		case 6:
			theTerrain_Material.Diffuse.r = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Diffuse.g = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Diffuse.b = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Diffuse.a = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Ambient.r = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Ambient.g = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Ambient.b = atof(token);
			token = strtok( NULL, splitter );
			theTerrain_Material.Ambient.a = atof(token);
			break;
		case 7:
			theFog_Start = atof(token);
			token = strtok( NULL, splitter );
			theFog_End = atof(token);
			token = strtok( NULL, splitter );
			theFog_Color.x = atof(token);
			token = strtok( NULL, splitter );
			theFog_Color.y = atof(token);
			token = strtok( NULL, splitter );
			theFog_Color.z = atof(token);			
			break;
		default:
			break;
		}
		
		++string_cnt;
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == NULL )
		{
			bRead = false;
			break;
		}		
	}
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	
	// ���ϴݱ�
	g_VFEnv.CloseFile( pFH );
	
	theCharacter_Light.Specular.r = 1.0f;
	theCharacter_Light.Specular.g = 1.0f;
	theCharacter_Light.Specular.b = 1.0f;
	theCharacter_Light.Specular.a = 1.0f;
	
	theCharacter_Light.Range = 1000;
	theCharacter_Light.Falloff = 0;
	theCharacter_Light.Attenuation0 = 1;
	theCharacter_Light.Attenuation1 = 0;
	theCharacter_Light.Attenuation2 = 0;
	
	theSky_Light.Diffuse.r = 1.0f;
	theSky_Light.Diffuse.g = 1.0f;
	theSky_Light.Diffuse.b = 1.0f;	
	
	theSky_Light.Ambient = theSky_Light.Diffuse;
	
	theSky_Material.Diffuse.r = 1.0f;
	theSky_Material.Diffuse.g = 1.0f;
	theSky_Material.Diffuse.b = 1.0f;
	
	theSky_Material.Ambient = theSky_Material.Diffuse;
	
	return true;
}



bool Player_Manager::LoadItemTable( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	int i =0  , j = 0 ; 
	strcpy( file_path, file_pos);
	//sprintf( file_name, "\\Chr\\item.dat");
	sprintf( file_name, "\\local_test\\item.dat");
	strcat( file_path, file_name);
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		::MessageBox(NULL,"Cannot Found item.dat",0,0);
		return false;
	}		
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SItemBaseInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SItemBaseInfo );
	
	if( Table_Num >= c_Max_Item_Num )
	{
		MessageBox( NULL, "<Item.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( theItemInfoTable, 0, sizeof( SItemBaseInfo )*c_Max_Item_Num);
	memset( theItemCodeTable, 0, sizeof( WORD ) * c_Max_ItemCode_Num);
	memset( theItemFileCodeTable, 0, sizeof( DWORD ) * 2 * c_Max_Item_Num );


	for( i = 0; i < c_Max_Item_Num; ++i )
	{
		if( fread( &theItemInfoTable[i], sizeof( SItemBaseInfo ), 1, file ) <= 0 )
		{
			break;
		}
	
		theItemCodeTable[theItemInfoTable[i].theCode] = i;
		theItemFileCodeTable[0][i] = theItemInfoTable[i].theFileCode1;
		theItemFileCodeTable[1][i] = theItemInfoTable[i].theFileCode2;
	}
	
	fclose( file );
	
	return true;
}

bool Player_Manager::LoadItemTableVF()
{
	char	file_name[256] = {0,};
	int i=0 , j =0;
	sprintf( file_name, "item.dat");
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
	int iFHSize = pFH->GetSize();
	int Table_Num = iFHSize / sizeof( SItemBaseInfo );
	int size = sizeof( SItemBaseInfo );
	
	if( Table_Num >= c_Max_Item_Num )
	{
		MessageBox( NULL, "<Item.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
		
	if( (Table_Num * size) != pFH->GetSize() )
	{
		SItemBaseInfo k;
		int iSize = sizeof(k.theVital);// #define MIX_ELDA_MODIFY2 �϶� iSize�� 2���� Ȯ��

		MessageBox( NULL, "<Item.dat> Loading Data Error", "Error", MB_OK );		
		return false;
	}

	memset( theItemInfoTable, 0, sizeof( SItemBaseInfo )*c_Max_Item_Num);
	memset( theItemCodeTable, 0, sizeof( WORD ) * c_Max_ItemCode_Num);
	memset( theItemFileCodeTable, 0, sizeof( DWORD ) * 2 * c_Max_Item_Num );
	
		
#ifdef MIX_ELDASTONE
	std::vector<int> m_CodeList;

	m_CodeList.clear();

	int IndexCode = 0;
	const int BosConstart	= 60010;
	const int BadiConstart	= 61010;
	const int Cotrastart	= 62010;
	const int Katrastart	= 63010;

	for( i = 0 ; i < 200 ; i+=10 )
	{
		m_CodeList.push_back(BosConstart + i );
		m_CodeList.push_back(BadiConstart + i );
		m_CodeList.push_back(Cotrastart + i );
		m_CodeList.push_back(Katrastart + i );
	}		

#endif


	for( i = 0; i < c_Max_Item_Num; ++i )
	{
		//		if( fread( &theItemInfoTable[i], sizeof( SItemBaseInfo ), 1, file ) <= 0 )
		if( pFH->Read( &theItemInfoTable[i], sizeof(SItemBaseInfo) ) <= 0 )
		{
			break;
		}
		
		theItemCodeTable[theItemInfoTable[i].theCode] = i;
		theItemFileCodeTable[0][i] = theItemInfoTable[i].theFileCode1;
		theItemFileCodeTable[1][i] = theItemInfoTable[i].theFileCode2;
		
#ifdef MIX_ELDASTONE
		int CopyCnt =0 ; 
	
		if( theItemInfoTable[i].theCode > 60000 )
		{
			for(int Next= 0 ; Next < m_CodeList.size() ; ++Next )
			{
				if( m_CodeList[Next]  == theItemInfoTable[i].theCode )
				{
					for( j = 1 ; j <10; j++)
					{
						int Count = i + j ; 
						//�ε��� : �⺻ �ռ������� ������ �����Ѵ�. 
						//���̺���� �ڵ� ���հ�� �����ۿ� ���ٽ��� �׻��迭 ���������� �ִ´�.
						// theAttackSuccSnd3 ���ݻ��� ������ ���� �ڵ带 �ִ´�. 
						theItemInfoTable[Count] = theItemInfoTable[i];

						//memcpy( theItemInfoTable[Count] , theItemInfoTable[i]  , sizeof(SItemBaseInfo) ) ;
						//�ڵ� �ϳ� ������ �ڵ�� �������� 
						theItemInfoTable[Count].theCode = theItemInfoTable[i].theCode+j;
						 
						theItemCodeTable[theItemInfoTable[Count].theCode] = i+j;
						theItemFileCodeTable[0][Count] = theItemInfoTable[i].theFileCode1;
						theItemFileCodeTable[1][Count] = theItemInfoTable[i].theFileCode2;
						
						int ColorIndex = theItemInfoTable[Count].theCode%theItemInfoTable[i].theCode;
						int ColorCode = 0 ; 
						int iGrade = (theItemInfoTable[Count].theGrade/100); 
						
						if(theItemInfoTable[Count].theCode < 62000)
						{
							switch(ColorIndex) 
							{   
								//������ 
							case 1:     
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_RED );
								}
								break; 
							case 2: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_ORANGE );
								}
								break;
							case 3: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_GREEN );
								}
								break;
							case 4: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_NAVY );
								}
								break;
							case 5: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_SILVER );
								}
								break;
							case 6: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_YELLOW_GREEN );
								}
								break;
							case 7: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_VIOLETE );
								}
								break;
							default:
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_RAW );
								}
								break;	
							}
						
						}	
						else 
						{
							switch(ColorIndex) 
							{   
							
							case 1:     
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_BLUE );
								}
								break; 
							case 2: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_YELLOW );
								}
								break;
							case 3: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_GREEN );
								}
								break;
							case 4: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_PURPLE );
								}
								break;
							case 5: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_SILVER );
								}
								break;
							case 6: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_YELLOW_GREEN );
								}
								break;
							case 7: 
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_VIOLETE );
								}
								break;
							default:
								{
									ColorCode  = g_ItemEldaMixAWndProc.GetItemCode( iGrade , CItemEldaMixAWndProc::ELDA_RAW );
								}
								break;	
								
							}
						}

						SItemBaseInfo* Item_Info =  NULL;
												
						Item_Info  = GetItemBaseInfoPtr( ColorCode ) ;
					
						theItemInfoTable[Count].theMinAttackBonus +=	Item_Info->theMinAttackBonus ;
						theItemInfoTable[Count].theMaxAttackBonus +=	Item_Info->theMaxAttackBonus ;
						theItemInfoTable[Count].theMinMagicBonus +=		Item_Info->theMinMagicBonus ;
						theItemInfoTable[Count].theMaxMagicBonus +=		Item_Info->theMaxMagicBonus ;
					
						theItemInfoTable[Count].thePower +=				Item_Info->thePower;
						theItemInfoTable[Count].theVital +=				Item_Info->theVital;
						theItemInfoTable[Count].theSympathy +=			Item_Info->theSympathy;
						theItemInfoTable[Count].theInt +=				Item_Info->theInt ;
						theItemInfoTable[Count].theStamina +=			Item_Info->theStamina ;
						theItemInfoTable[Count].theDex +=				Item_Info->theDex ;
						theItemInfoTable[Count].theAttackDef +=			Item_Info->theAttackDef ;
						theItemInfoTable[Count].theMagicRes +=			Item_Info->theMagicRes;
						theItemInfoTable[Count].theAttackSuccSnd3 =		ColorCode;
					}
					i+= j;
				}
			}
		}
		
#endif

	}
	
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� ��
	
	return true;
}

bool Player_Manager::LoadEtcFileVF()
{

	g_ItemSelectPotionWndProc.CreateLoadTable();
	  

	return true; 
}


bool Player_Manager::LoadNPCTable( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos);
	//sprintf( file_name, "\\Npc\\npc.dat");
	sprintf( file_name, "\\local_test\\npc.dat");
	strcat( file_path, file_name);
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		::MessageBox(NULL,"Cannot Found npc.dat",0,0);
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SNPCBaseInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SNPCBaseInfo );
	
	if( Table_Num >= c_Max_NPC_Num )
	{
		MessageBox( NULL, "<Npc.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( theNPCInfoTable, 0, sizeof( SNPCBaseInfo )*c_Max_NPC_Num);
	memset( theNPCCodeTable, 0, sizeof( WORD ) * c_Max_NPCCode_Num);
	
	g_Particle.m_NpcEffectList.clear();
	
	for( Int i = 0; i < c_Max_NPC_Num; ++i )
	{
		if( fread( &( theNPCInfoTable[i] ), sizeof( SNPCBaseInfo ), 1, file ) <= 0 )
		{
			break;
		}
		
		theNPCCodeTable[theNPCInfoTable[i].theCode] = i;		
		
		npceffect TempNpcEffect;
		
		TempNpcEffect.Init();
		TempNpcEffect.index     = i;
		TempNpcEffect.righthand = theNPCInfoTable[i].theRightEff;
		TempNpcEffect.lefthand  = theNPCInfoTable[i].theLefeEff;
		TempNpcEffect.neck      = theNPCInfoTable[i].theNeckEff;
		TempNpcEffect.skill1    = theNPCInfoTable[i].theSkill1Eff;
		TempNpcEffect.skill2    = theNPCInfoTable[i].theSkill2Eff;
		
		g_Particle.m_NpcEffectList.push_back( TempNpcEffect );
	}
	
	fclose( file );
	
	return true; 
}


bool Player_Manager::LoadNPCTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "npc.dat");
	
	VFileHandle* pFH = g_VFNpcRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
	int size = sizeof(SNPCBaseInfo);



	int Table_Num = pFH->GetSize() / sizeof( SNPCBaseInfo );
	
	if( Table_Num >= c_Max_NPC_Num )
	{
		MessageBox( NULL, "<Npc.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}


	
	if( (Table_Num * size) != pFH->GetSize() ) 
	{
		MessageBox( NULL, "<Npc.dat> File not Size", "Error", MB_OK );		
	}



	memset( theNPCInfoTable, 0, sizeof( SNPCBaseInfo )*c_Max_NPC_Num);
	memset( theNPCCodeTable, 0, sizeof( WORD ) * c_Max_NPCCode_Num);
	memset( theNPCFileCodeTable, 0, sizeof( DWORD ) * c_Max_NPC_Num);
	
	g_Particle.m_NpcEffectList.clear();
	
	for( Int i = 0; i < c_Max_NPC_Num; ++i )
	{
		//		if( fread( &( theNPCInfoTable[i] ), sizeof( SNPCBaseInfo ), 1, file ) <= 0 )
		if( pFH->Read( &theNPCInfoTable[i], sizeof( SNPCBaseInfo ) ) <= 0 )
		{
			break;
		}
		
		theNPCCodeTable[theNPCInfoTable[i].theCode] = i;
		theNPCFileCodeTable[i] = theNPCInfoTable[i].theFileCode;
		
		npceffect TempNpcEffect;
		
		TempNpcEffect.Init();
		TempNpcEffect.index     = i;
		TempNpcEffect.righthand = theNPCInfoTable[i].theRightEff;
		TempNpcEffect.lefthand  = theNPCInfoTable[i].theLefeEff;
		TempNpcEffect.neck      = theNPCInfoTable[i].theNeckEff;
		TempNpcEffect.skill1    = theNPCInfoTable[i].theSkill1Eff;
		TempNpcEffect.skill2    = theNPCInfoTable[i].theSkill2Eff;
		
		g_Particle.m_NpcEffectList.push_back( TempNpcEffect );
	}
	
	g_VFNpcRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 
	
	return true;
}

bool Player_Manager::LoadSkillTable( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos);
	//sprintf( file_name, "\\Chr\\skill.dat");
	sprintf( file_name, "\\local_test\\skill.dat");
	strcat( file_path, file_name);
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		::MessageBox(NULL,"Cannot Found skill.dat",0,0);
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SSkillBaseInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SSkillBaseInfo );
	
	if( Table_Num >= c_Max_Skill_Num )
	{
		MessageBox( NULL, "<Skill.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	
	memset( theSkillInfoTable, 0, sizeof( SSkillBaseInfo ) * c_Max_Skill_Num);
	memset( theSkillCodeTable, 0, sizeof( WORD ) * c_Max_SkillCode_Num);
	
	for( Int i = 0; i < c_Max_Skill_Num; ++i )
	{
		if( fread( &( theSkillInfoTable[i] ), sizeof( SSkillBaseInfo ), 1, file ) <= 0 )
		{
			break;
		}		
		theSkillCodeTable[theSkillInfoTable[i].theSkillCode] = i;
	}
	fclose( file );
	
	return true;
}

bool Player_Manager::LoadSkillTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "\\Chr\\skill.dat");
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
	int Table_Num = pFH->GetSize() / sizeof( SSkillBaseInfo );
	
	if( Table_Num >= c_Max_Skill_Num )
	{
		MessageBox( NULL, "<Skill.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( theSkillInfoTable, 0, sizeof( SSkillBaseInfo ) * c_Max_Skill_Num);
	memset( theSkillCodeTable, 0, sizeof( WORD ) * c_Max_SkillCode_Num);
	
	for( Int i = 0; i < c_Max_Skill_Num; ++i )
	{
		//		if( fread( &( theSkillInfoTable[i] ), sizeof( SSkillBaseInfo ), 1, file ) <= 0 )
		if( pFH->Read( &theSkillInfoTable[i], sizeof( SSkillBaseInfo ) ) <= 0 )
		{
			break;
		}		
		theSkillCodeTable[theSkillInfoTable[i].theSkillCode] = i;
	}
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 
	
	return true;
}

bool Player_Manager::LoadStatTable( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos);
	sprintf( file_name, "\\Chr\\level.dat");
	strcat( file_path, file_name);
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SLevelBaseInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SLevelBaseInfo );
	
	if( Table_Num >= c_Max_Stat_Num )
	{
		MessageBox( NULL, "<Level.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( theStatInfoTable, 0, sizeof( SLevelBaseInfo ) * c_Max_Stat_Num );
	//memset( theMainClassTable, 0, sizeof( DWORD ) * c_Max_MainClass_Num ); <---------- ���� 
	memset( theMainClassTable, 0, sizeof( WORD ) * c_Max_MainClass_Num );
	
	
	for( Int i = 0; i < c_Max_Stat_Num; ++i )
	{
		if( fread( &( theStatInfoTable[i] ), sizeof( SLevelBaseInfo ), 1, file ) <= 0 )
		{
			break;
		}
		theMainClassTable[theStatInfoTable[i].theMainClass] = i;
	}		
	fclose( file );
	
	return true;
}


bool Player_Manager::LoadStatTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "level.dat");
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
#ifdef EXP_INT64

	
	int Table_Num = pFH->GetSize() / sizeof( SLevelBaseInfoLoad );
	
	if( Table_Num >= c_Max_Stat_Num )
	{
		MessageBox( NULL, "<Level.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	int filesize  = Table_Num *  sizeof( SLevelBaseInfoLoad );
	if( filesize != pFH->GetSize())
	{
		MessageBox( NULL, "<Level.dat> Loading Max Over", "Error", MB_OK );		
		return false;
	}
	
	
	
	memset( theStatInfoTable, 0, sizeof( SLevelBaseInfo ) * c_Max_Stat_Num );
	//memset( theMainClassTable, 0, sizeof( DWORD ) * c_Max_MainClass_Num ); <---------- ���� 
	memset( theMainClassTable, 0, sizeof( WORD ) * c_Max_MainClass_Num );
	
	SLevelBaseInfoLoad BaseInfoLoad;
	
	for( Int i = 0; i < c_Max_Stat_Num; ++i )
	{
		memset(&BaseInfoLoad , NULL , sizeof(BaseInfoLoad));
		
		if( pFH->Read( &BaseInfoLoad, sizeof( SLevelBaseInfoLoad ) ) <= 0 )
		{
			break;
		}
		//theDex ���� �޸� ī�� 
		memcpy(&theStatInfoTable[i] , &BaseInfoLoad  , sizeof(BYTE) * 20  );
		
		theStatInfoTable[i].thePoint = BaseInfoLoad.thePoint;
		theStatInfoTable[i].theSkillPoint = BaseInfoLoad.thePoint;
		theStatInfoTable[i].theMaxSkillPoint = BaseInfoLoad.theMaxSkillPoint;
		theStatInfoTable[i].theExp =  (LONGLONG)(4290000000 * (LONGLONG)BaseInfoLoad.theInt64X) + (LONGLONG)BaseInfoLoad.thePlusValue;
			
		theMainClassTable[theStatInfoTable[i].theMainClass] = i;
	}		
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 


#else 
	
	int Table_Num = pFH->GetSize() / sizeof( SLevelBaseInfo );
	
	if( Table_Num >= c_Max_Stat_Num )
	{
		MessageBox( NULL, "<Level.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( theStatInfoTable, 0, sizeof( SLevelBaseInfo ) * c_Max_Stat_Num );
	//memset( theMainClassTable, 0, sizeof( DWORD ) * c_Max_MainClass_Num ); <---------- ���� 
	memset( theMainClassTable, 0, sizeof( WORD ) * c_Max_MainClass_Num );
	
	
	for( Int i = 0; i < c_Max_Stat_Num; ++i )
	{
		if( pFH->Read( &theStatInfoTable[i], sizeof( SLevelBaseInfo ) ) <= 0 )
		{
			break;
		}
		theMainClassTable[theStatInfoTable[i].theMainClass] = i;
	}		
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 


#endif 


	return true;
}

///---------------------------------------------------------------------------
///-- LoadSubStatTable
///---------------------------------------------------------------------------
///-- Sub Class ���� Data �� Read �Ѵ�.
bool Player_Manager::LoadSubStatTable( char* file_pos )
{	
	FILE*	file;
	char	file_name[256] = {0,};	
	char	file_path[256] = {0,};	
	
	strcpy( file_path, file_pos);
	sprintf( file_name, "\\Chr\\sublevel.dat");
	strcat( file_path, file_name);
	
	
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SSubLevelBaseInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SSubLevelBaseInfo );
	
	if( Table_Num >= c_Max_Stat_Num )
	{
		MessageBox( NULL, "<Sublevel.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( theSubStatInfoTable, 0, sizeof( SSubLevelBaseInfo ) * c_Max_Stat_Num );	
	//memset( theSubClassTable, 0, sizeof( WORD ) * c_Max_MainClass_Num );
	
	
	for( Int i = 0; i < c_Max_Stat_Num; ++i )
	{
		if( fread( &( theSubStatInfoTable[i] ), sizeof( SSubLevelBaseInfo ), 1, file ) <= 0 )
		{
			break;
		}
	}		
	fclose( file );
	
	return true;
}

///---------------------------------------------------------------------------
///-- LoadSubStatTableVF
///---------------------------------------------------------------------------
///-- Sub Class ���� Data �� Read �Ѵ�.
bool Player_Manager::LoadSubStatTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "sublevel.dat");
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
	int Table_Num = pFH->GetSize() / sizeof( SSubLevelBaseInfo );
	
	if( Table_Num >= c_Max_Stat_Num )
	{
		MessageBox( NULL, "<Sublevel.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( theSubStatInfoTable, 0, sizeof( SSubLevelBaseInfo ) * c_Max_Stat_Num );	
	//memset( theSubClassTable, 0, sizeof( WORD ) * c_Max_MainClass_Num );
	
	
	for( Int i = 0; i < c_Max_Stat_Num; ++i )
	{
		if( pFH->Read( &theSubStatInfoTable[i], sizeof( SSubLevelBaseInfo ) ) <= 0 )
		{
			break;
		}
	}		
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 
	
	return true;
}

/*
typedef struct _SItemShopInfo
{
struct _SListInfo
{
WORD	theItemCode;
WORD	theMaxNum;
};
WORD		theNPCCode;
BYTE		theListNum;
_SListInfo	theListInfo[50];
}SItemShopInfo, * SItemShopInfoPtr;

  MAX_NUM ( FileSize() / sizeof( SItemShopInfo ) )
  //--------------------------------------------------------
  struct SStoreItem
  {
  WORD	Code;
  WORD	Amount;//0�̸� ���� ���
  
	BYTE	DealingItemType;
	SItemBaseInfo*	Table;
	};
	
	  struct SStoreShop
	  {
	  WORD	OwnerCode;//NPC Code
	  DWORD	OwnerUID;
	  int		NumItem;
	  SStoreItem Item[MAX_STORE_SERVICE_ITEM];
	  };
	  
		struct SStoreList
		{
		int NumShop;
		SStoreShop Shop[MAX_STORE_SHOP];
		};
*/

bool Player_Manager::LoadStoreTable( char* file_pos )
{
	/*
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos);
	sprintf( file_name, "\\Chr\\ItemShop.dat");
	strcat( file_path, file_name);
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SItemShopInfo ) ) != 0 )
	{
		return( false );
	}
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		return false;
	}
	
	int Shop_Num = file_buf.st_size / sizeof( SItemShopInfo );
	theTableInfo.NumStore = Shop_Num;
	
	memset( &theStoreList, 0, sizeof( SStoreList ) );
	
	theStoreList.NumShop = theTableInfo.NumStore;
	
	SItemShopInfo itemshop_info;
	
	for( int i = 0; i < Shop_Num; ++i )
	{
		if( fread( &itemshop_info, sizeof( SItemShopInfo ), 1, file ) <= 0 )
		{
			break;
		}
		
		theStoreList.Shop[i].OwnerCode = itemshop_info.theNPCCode;
		theStoreList.Shop[i].NumItem = itemshop_info.theListNum;
		
		memset( theStoreList.Shop[i].Item, 0, sizeof( theStoreList.Shop[i].Item ) );
		
		for( int j=0; j < itemshop_info.theListNum; ++j )
		{		
			theStoreList.Shop[i].Item[j].Code = itemshop_info.theListInfo[j].theItemCode;
			theStoreList.Shop[i].Item[j].Amount = itemshop_info.theListInfo[j].theMaxNum;
			theStoreList.Shop[i].Item[j].Table = GetItemBaseInfoPtr( theStoreList.Shop[i].Item[j].Code );
			if( theStoreList.Shop[i].Item[j].Table )
			{
 				theStoreList.Shop[i].Item[j].DealingItemType = GetDealingItem( theStoreList.Shop[i].Item[j].Table  , theStoreList.Shop[i].OwnerCode );
			}
		}
	}
	
	fclose( file );
	*/

	return true;
}


bool Player_Manager::LoadStoreTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "ItemShop.dat");
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;	
	
	int Shop_Num = pFH->GetSize() / sizeof( SItemShopInfo );
	
	int size = sizeof(SItemShopInfo);

	if( (Shop_Num * sizeof(SItemShopInfo)) != pFH->GetSize() )
	{
		RMessageBox(NULL, _RT("Load File Fail ItemShop") , G_STRING_CHAR(IDS_RockSoft ) , MB_OK);
	}

	theTableInfo.NumStore = Shop_Num;
	
	memset( &theStoreList, 0, sizeof( SStoreList ) );
	
	theStoreList.NumShop = theTableInfo.NumStore;
	
	SItemShopInfo itemshop_info;
	
	for( int i = 0; i < Shop_Num; ++i )
	{
		if( pFH->Read( &itemshop_info, sizeof( SItemShopInfo ) ) <= 0 )
		{
			break;
		}
			
		theStoreList.Shop[i].OwnerCode = itemshop_info.theNPCCode;
		theStoreList.Shop[i].NumItem = itemshop_info.theListNum;
			
		memset( theStoreList.Shop[i].Item, 0, sizeof( theStoreList.Shop[i].Item ) );
		
		for( int j=0; j < itemshop_info.theListNum; ++j )
		{		
			theStoreList.Shop[i].Item[j].Code = itemshop_info.theListInfo[j].theItemCode;
			theStoreList.Shop[i].Item[j].Amount = itemshop_info.theListInfo[j].theMaxNum;
			theStoreList.Shop[i].Item[j].Table = GetItemBaseInfoPtr( theStoreList.Shop[i].Item[j].Code );
			if( theStoreList.Shop[i].Item[j].Table )
			{
				theStoreList.Shop[i].Item[j].DealingItemType = GetDealingItem( theStoreList.Shop[i].Item[j].Table  , theStoreList.Shop[i].OwnerCode );
			}
		}
	}
	
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 
	
	return true;
}

bool Player_Manager::LoadQuestTable( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos );
	//sprintf( file_name, "\\Chr\\Quest.dat");
	sprintf( file_name, "\\local_test\\Quest.dat");
	strcat( file_path, file_name);
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		::MessageBox(NULL,"Cannot Found Quest.dat",0,0);		
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SQuestInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SQuestInfo );
	
	if( Table_Num >= c_Max_Quest_Count )
	{
		MessageBox( NULL, "<Quest.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	
	//SQuestInfo theQuestInfoTable[c_Max_Quest_Count];
	memset( &theQuestInfoTable, 0, sizeof( SQuestInfo ) * c_Max_Quest_Count );
	
	for( int i = 0; i < c_Max_Quest_Count; ++i )
	{
		if( fread( &theQuestInfoTable[i], sizeof( SQuestInfo ), 1, file ) <= 0 )
		{
			break;
		}
	}
	
	fclose( file );
	
	return true;
}

bool Player_Manager::LoadQuestTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "Quest.dat");	
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) 
		return false;
	
	int iSize = pFH->GetSize();
	int iSizeof = sizeof( SQuestInfo );
	int Table_Num = iSize / iSizeof;
	
	if( ( iSize % iSizeof ) != 0 )
	{
		MessageBox( NULL, "<Quest.dat> Size Error", "Error", MB_OK );		
		return false;
	}
	

	if( Table_Num >= c_Max_Quest_Count )
	{
		MessageBox( NULL, "<Quest.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	//SQuestInfo theQuestInfoTable[c_Max_Quest_Count];
	memset( &theQuestInfoTable, 0, sizeof( SQuestInfo ) * c_Max_Quest_Count );
	
	for( int i = 0; i < c_Max_Quest_Count; ++i )
	{
		if( pFH->Read( &theQuestInfoTable[i], sizeof( SQuestInfo ) ) <= 0 )
		{
			break;
		}
	}
	
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 
	
	return true;
}

/*
typedef struct _SPreQuestInfo
{
WORD	theQuestCode;
WORD	theNPCCode;
BYTE	theNeedLevel;
}SPreQuestInfo, * SPreQuestInfoPtr;

  typedef struct _SQuestFlag
  {
  WORD	theQuestCode;	// code 
  BYTE	theFlag;		// 0 �� �ƴϸ� ������ �ƴϸ� ���� �Ϸ� 
  BYTE	theDisplay;		// �������� ����Ʈ�� ǥ���Ͽ� �ش� 
  }SQuestFlag, * SQuestFlagPtr;
*/


// character_display() ���� �÷��̾� 


bool Player_Manager::LoadQuestSignalTable( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos);
	sprintf( file_name, "\\Chr\\QuestSignal.dat");
	strcat( file_path, file_name);
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SPreQuestInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SPreQuestInfo );
	
	if( Table_Num >= c_Max_PreQuestInfo_Count )
	{
		MessageBox( NULL, "<QuestSignal.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	
	memset( &thePreQuestInfoTable, 0, sizeof( SPreQuestInfo ) * c_Max_PreQuestInfo_Count );
	
	for( int i = 0; i < c_Max_PreQuestInfo_Count; ++i )
	{
		if( fread( &thePreQuestInfoTable[i], sizeof( SPreQuestInfo ), 1, file ) <= 0 )
		{
			break;
		}
	}
	
	fclose( file );
	
	return true;
}

bool Player_Manager::LoadQuestSignalTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "QuestSignal.dat");	
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;	
	
	int Table_Num = pFH->GetSize() / sizeof( SPreQuestInfo );
	
	if( Table_Num >= c_Max_PreQuestInfo_Count )
	{
		MessageBox( NULL, "<QuestSignal.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( &thePreQuestInfoTable, 0, sizeof( SPreQuestInfo ) * c_Max_PreQuestInfo_Count );
	
	for( int i = 0; i < c_Max_PreQuestInfo_Count; ++i )
	{
		if( pFH->Read( &thePreQuestInfoTable[i], sizeof( SPreQuestInfo ) ) <= 0 )
		{
			break;
		}
	}
	
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 
	
	return true;
}


bool Player_Manager::LoadTableInfo( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos );
	sprintf( file_name, "\\Chr\\table.ini");
	strcat( file_path, file_name);
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		return false;
	}
	
	char tmp_str[256] = "";
	char tmp_line[256] = "";
	
	while( !feof( file ) )
	{
		_fgetts( tmp_line, 256, file );
		_stscanf( tmp_line, _T("%s %d"), tmp_str, &theTableInfo.NumStore );
	}
	
	
	fclose( file );
	
	return true;
}


bool Player_Manager::LoadTableInfoVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "table.ini");
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
	char tmp_str[256] = "";
	char tmp_line[256] = "";
	
	char*	token;				// ��ū
	char	string [512];			// ���� �ٴ��� ���� ����
	bool	bRead  = true;			// ���� �� �о����� üũ		
	char	splitter[] = " \n\t";	// ������ : ĳ��������, �� 
	
	
	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == 0 )
	{
		g_VFChrRoot.CloseFile( pFH );
		return false; 
	}		
	do 
	{
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{	
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				bRead = false;
				break;
			}
		}
		
		// ���� �б�
		token = strtok( string, splitter );
		if( token == NULL )			
			break;
		
		strcpy( tmp_str, token );									// "MaxShop"
		
		token = strtok( NULL, splitter ); 
		theTableInfo.NumStore = (int)atoi(token);			// Number
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == 0 )
			bRead = false;			
	}  while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����			
	
	g_VFChrRoot.CloseFile( pFH );
	
	return true;
}


void Player_Manager::SetNpcFileCode( int aChrIndex, DWORD aFileName )
{
	for( int idx = 0; idx < c_Max_NPC_Num; ++idx )
	{		
		if( theNPCInfoTable[idx].theFileCode == aFileName )		
		{
			theNPCInfoTable[idx].theFileCode = aChrIndex;			
			///-- �ٸ��ڵ忡 ���� ���ϳ����� ���� ����..;;;
		}
	}
	
	return; 
}

void Player_Manager::SetItemFileCode12( int aItmIndex, DWORD aFileName )
{	
	// 	theFileCode1 (��), theFileCode2 (��), theFileCode3 (�κ��丮_TID ���) 
	for( int idx = 0; idx < c_Max_Item_Num; ++idx )
	{		
		//		if( theItemInfoTable[idx].theClass == n_Class_Serve )		// ����� ���̺� ���� �״�� �����ؾ� �Ѵ� 
		//			continue;
		
		if( theItemInfoTable[idx].theFileCode1 == aFileName )		
		{
			theItemInfoTable[idx].theFileCode1 = aItmIndex;
			
			if( theItemInfoTable[idx].theFileCode2 == aFileName )		
			{
				theItemInfoTable[idx].theFileCode2 = aItmIndex;
			}		
			//			break;
		}
		if( theItemInfoTable[idx].theFileCode2 == aFileName )		
		{
			theItemInfoTable[idx].theFileCode2 = aItmIndex;
			//			break;
		}		
	}
	
	return; 
}


//-----------------------------------------------------------------------------
SNPCBaseInfo* Player_Manager::GetNPCBaseInfoPtr( DWORD aNPCCode )
{
	if( aNPCCode > c_Max_NPCCode_Num )
		return NULL;
	
	return( &theNPCInfoTable[theNPCCodeTable[aNPCCode]] );
}

//-----------------------------------------------------------------------------
SNPCBaseInfo* Player_Manager::GetNPCBaseInfo(int _nIndex)
{
	return (&theNPCInfoTable[theNPCCodeTable[g_Map.m_NpcTable[_nIndex].code]]);	 
}

DWORD Player_Manager::GetNPCRealFileCode( DWORD aNPCCode )
{
	if( aNPCCode > c_Max_NPCCode_Num )
		return 0;
	
	return( theNPCFileCodeTable[theNPCCodeTable[aNPCCode]] );
}

//-----------------------------------------------------------------------------
SSkillBaseInfo* Player_Manager::GetSkillBaseInfoPtr( DWORD aSkillCode )
{
	if( aSkillCode > c_Max_SkillCode_Num )
		return NULL;
	
	return( &theSkillInfoTable[theSkillCodeTable[aSkillCode]] );
}

//-----------------------------------------------------------------------------
WORD Player_Manager::GetSkillIconTID( DWORD aSkillCode )
{
	return( theSkillInfoTable[theSkillCodeTable[aSkillCode]].theSkillIcon );
}

//-----------------------------------------------------------------------------
WORD Player_Manager::GetSkillAttriIconTID( DWORD aSkillCode )
{
	if( theSkillCodeTable[aSkillCode] == 0 )
	{
		return TID_None;
	}
	
	return( theSkillInfoTable[theSkillCodeTable[aSkillCode]].theAttriIcon ); 
}

//-----------------------------------------------------------------------------
WORD Player_Manager::GetSkillContinualIconTID( DWORD _skillcode )//������ų ������	theAddEffectIcon
{
	if( theSkillCodeTable[_skillcode] == 0 )
	{
		return TID_None;
	}
	
	//	return( theSkillInfoTable[theSkillCodeTable[_skillcode]].theAddEffectIcon ); 
	// �ΰ�ȿ�� ������ -> Ȯ���� �ٲٴ� ����� �ش� ��ų ������ TID ��ȣ�� 10000 �� ���� ������ �����ؼ� ��� 
	return( theSkillInfoTable[theSkillCodeTable[_skillcode]].theSkillIcon ); 	
	//	return( theSkillInfoTable[theSkillCodeTable[_skillcode]].theMiniIcon ); 		
}

//-----------------------------------------------------------------------------
WORD Player_Manager::GetNextSkillLevel( DWORD _skillcode )
{
	WORD master = 0xffff;
	
	// return 0xffff�̸� ������....
	if( ( _skillcode % 10 ) == 0 )
	{
		return( master );
	}
	else if( ( _skillcode + 1 ) == 0 )
	{
		return( master );
	}
	else
	{
		return( _skillcode );
	}
}

//-----------------------------------------------------------------------------
SItemBaseInfo* Player_Manager::GetItemBaseInfoPtr( DWORD ItemCode )
{
	if( ItemCode > c_Max_ItemCode_Num )
		return NULL;
	
	if( ItemCode > 1 && theItemCodeTable[ItemCode] == 0 )
	{
		return &theItemInfoTable[theItemCodeTable[1]];
	}
		
	return( &theItemInfoTable[theItemCodeTable[ItemCode]] );
}
#ifdef DECO_RENEWAL_GUILD_MJH
// ��幮�� �ε����� ���ؼ� ���������� ����
SItemBaseInfo* Player_Manager::GetItemBaseInfoIndexPtr( BYTE ItemIndex )
{
	for (int i = 20000 ; i < c_Max_ItemCode_Num ; i++ )
	{
		if (theItemInfoTable[theItemCodeTable[i]].theSoundFile == ItemIndex)
		{
			return( &theItemInfoTable[theItemCodeTable[i]] ); 
		}		
	}
	return( &theItemInfoTable[theItemCodeTable[1]] ); 
}
#endif // DECO_RENEWAL_GUILD_MJH
SItemAddEffectInfo * Player_Manager::GetItemAddEffectInfo( BYTE dAddCode )
{
	if( dAddCode > c_Max_ItemAddEffect_Num )
		return NULL;
	
	return &theItemAddEffectInfoTable[dAddCode - 1];
}

//-----------------------------------------------------------------------------
DWORD Player_Manager::GetItemInfo_FileCode12( int ChrAttrIndex, DWORD ItemCode )
{
	if( ItemCode == 0 )
		return 0;
	
	if( ItemCode > 1 && theItemCodeTable[ItemCode] == 0 )
		return 0;
	
	DWORD Itm_Index = 0;	
	
	if( ChrAttrIndex == n_Millena_Man || ChrAttrIndex == n_Rain_Man )
	{
		Itm_Index = theItemInfoTable[theItemCodeTable[ItemCode]].theFileCode1;
	}
	else if( ChrAttrIndex == n_Millena_Woman || ChrAttrIndex == n_Rain_Woman )
	{
		Itm_Index = theItemInfoTable[theItemCodeTable[ItemCode]].theFileCode2;
	}
	
	if( Itm_Index >= ( c_Chr_Itm_Count + c_Npc_Itm_Count ) )
	{
		CloseWnd_Error( "!%d.itm ������ ã�� �� �����ϴ�. Code:%d", Itm_Index, ItemCode );					
		return 0;
	}
	
	return Itm_Index;
}

DWORD Player_Manager::GetItemRealFileCode( int ChrAttrIndex, DWORD ItemCode )
{
	if( ItemCode == 0 )
		return 0;
	
	if( ItemCode > 1 && theItemCodeTable[ItemCode] == 0 )
		return 0;
	
	DWORD dFileCode = 0;	
	
	if( ChrAttrIndex == n_Millena_Man || ChrAttrIndex == n_Rain_Man )
	{
		dFileCode = theItemFileCodeTable[0][theItemCodeTable[ItemCode]];
	}
	else if( ChrAttrIndex == n_Millena_Woman || ChrAttrIndex == n_Rain_Woman )
	{
		dFileCode = theItemFileCodeTable[1][theItemCodeTable[ItemCode]];
	}
	
	return dFileCode;
}

//-----------------------------------------------------------------------------
WORD Player_Manager::GetItemTID( DWORD ItemCode )
{
	if( ItemCode > 1 && theItemCodeTable[ItemCode] == 0 )
		return( theItemInfoTable[theItemCodeTable[1]].theFileCode3 );
	
	if( nRui->thePcParam.Gender == MALE )
	{
		return( theItemInfoTable[theItemCodeTable[ItemCode]].theFileCode3 );
	}	
	else if( nRui->thePcParam.Gender == FEMALE )
	{
		return( theItemInfoTable[theItemCodeTable[ItemCode]].theFileCode4 );
	}
	
	return 0;
}

//-----------------------------------------------------------------------------
BYTE Player_Manager::GetItemClsss( DWORD ItemCode )
{
	if( ItemCode > 1 && theItemCodeTable[ItemCode] == 0 )
		return 0;
	
	return( theItemInfoTable[theItemCodeTable[ItemCode]].theClass );
}

BYTE Player_Manager::GetItemType( DWORD ItemCode )
{
	if( ItemCode > 1 && theItemCodeTable[ItemCode] == 0 )
		return 0;
	
	return( theItemInfoTable[theItemCodeTable[ItemCode]].theType );
}

//-----------------------------------------------------------------------------
SLevelBaseInfo*	Player_Manager::GetStatBaseInfo( BYTE _main_class )
{
	return( theStatInfoTable );
}

//-----------------------------------------------------------------------------
int	Player_Manager::GetStatSkillNeedPoint( BYTE _main_class, int _level )
{
	for( int i = 0; i < c_Max_Stat_Num; ++i )
	{
		if( theStatInfoTable[i].theMainClass == _main_class )
		{
			for( int j = 0; j < 10; ++j )
			{
				if( theStatInfoTable[i+j].theLevel == _level )
				{
					return( theStatInfoTable[i+j].theSkillPoint );
				}
			}
		}
	}
	return( -1 );
}

LONGLONG  Player_Manager::GetStatNeedExp64( BYTE _main_class, int _level )
{
	for( int i = 0; i < c_Max_Stat_Num; ++i )
	{
		if( theStatInfoTable[i].theMainClass == _main_class )
		{
			for( int j = 0; j < 10; ++j )
			{
				if( theStatInfoTable[i+j].theLevel == _level )
				{
					return( theStatInfoTable[i+j].theExp );
				}
			}
		}
	}
	return( -1 );
}


//-----------------------------------------------------------------------------
DWORD  Player_Manager::GetStatNeedExp( BYTE _main_class, int _level )
{
	for( int i = 0; i < c_Max_Stat_Num; ++i )
	{
		if( theStatInfoTable[i].theMainClass == _main_class )
		{
			for( int j = 0; j < 10; ++j )
			{
				if( theStatInfoTable[i+j].theLevel == _level )
				{
					return( theStatInfoTable[i+j].theExp );
				}
			}
		}
	}
	return( -1 );
}
///---------------------------------------------------------------------------
///-- GetStatNeedSubExp
///---------------------------------------------------------------------------
DWORD  Player_Manager::GetStatNeedSubExp( BYTE a8_sub_class, int ai_level )
{
	for( int i = 0; i < c_Max_Stat_Num; ++i )
	{
		if( theSubStatInfoTable[i].theSubClass == a8_sub_class )
		{
			for( int j = 0; j < 10; ++j )
			{
				if( theSubStatInfoTable[i+j].theLevel == ai_level )
				{
					return( theSubStatInfoTable[i+j].theExp );
				}
			}
		}
	}
	return( -1 );
	
}
//-----------------------------------------------------------------------------
SStoreList*	Player_Manager::GetStoreList()
{
	return( &theStoreList );
}

//-----------------------------------------------------------------------------
SStoreShop*	Player_Manager::GetStoreShop( WORD OwnerCode )
{
	for( int i = 0; i < theStoreList.NumShop; ++i )
	{
		if( theStoreList.Shop[i].OwnerCode == OwnerCode )
		{
			return( &theStoreList.Shop[i] );
		}
	}
	
	return( NULL );
}

//���� �ϵ��ڵ��� �Ⱦ� ����... 
//���μ����� NPC�� �ϵ��ڵ� �Ǿ��ִ�. 

N_DEALINGITEM Player_Manager::IsSealHolyGroundNPC(int NpcCode )
{
	switch(NpcCode)
	{
		case 7079:
		case 7081:
		case 7083:
		case 7084:
		case 7085:
		case 7086:
		case 7087:
		case 7088:
			return n_STORE_ETC_DESTORY;

		case 7080:
		case 7082:
		case 7089:
		case 7090:
		case 7091:
		case 7092:
		case 7093:
		case 7094:
			return n_STORE_ETC_SEALED;
		
		default: 
			return n_STORE_NONE;
	}

	return n_STORE_NONE;
}


//-----------------------------------------------------------------------------
N_DEALINGITEM Player_Manager::GetDealingItem(SItemBaseInfo * pItemBaseInfo , int OwnerCode )
{
	//n_STORE_NONE,   //
	//n_STORE_WEAPON,  // ����( ����, ���� ) = ����, ����
	//n_STORE_ARMOR,  // ��( ����, ����, �尩, ���� ) = ����, ����, �尩, ����
	//n_STORE_SUNDRIES,  // ��ȭ( �������, ��ű� ) = �Ӹ�, �������, ��ű�
	//n_STORE_USE,   // �Ҹ�ǰ( �Ҹ�ǰ, ��Ҹ�ǰ ) = �Ҹ�ǰ, ��Ҹ�ǰ
	
	/// < �� 1 �з� > 
	/*
	enum NItemClass
	{
	n_Class_Helmet = 0,			// 0 �Ӹ�
	n_Class_Weapon,				// 1 ����
	n_Class_CoatArmor,			// 2 ������� - ����
	n_Class_PantsArmor,			// 3 ������� - ����
	n_Class_Gloves,				// 4 ������� - �尩
	n_Class_Shoes,				// 5 ������� - ����
	n_Class_Serve,				// 6 �������(����, ����, ��)
	n_Class_Accessory,			// 7 ��ű�
	n_Class_Shield,				// 8 ����
	n_Class_Use,				// 9 �Ҹ�ǰ
	n_Class_Useless,			// 10 ��Ҹ�ǰ
	n_Class_Special,			// 11 Ư�������� 
	n_Class_Face,				// 12 �� 
	n_Class_Quest,				// 13 ����Ʈ ������ 
	n_Class_Eldastone			// 14 ���ٽ���
	}; 
	*/
	//���μ��� NPC �� ���� �����Ѵ�. 
	N_DEALINGITEM n_StoreStage = IsSealHolyGroundNPC(OwnerCode) ;
	
	if( n_StoreStage  != n_STORE_NONE) 
	{
		return n_StoreStage;
	}

	if( IsColonyItem(pItemBaseInfo) )
	{
		WORD _CallType = pItemBaseInfo->theReHPSpeed;
		switch( _CallType )
		{
		case n_CoPosition:
			{
				return( n_STORE_POSITION ); 
			}
			break;
		case n_CoAttackArams:
			{
				return( n_STORE_ATTCKARMS ); 
			}
			break;
		case n_CoBuilding:
			{
				return( n_STORE_BULDING ); 
			}
			break;
		case n_CoGuardTower:
			{
				return( n_STORE_GTOWER ); 
			}
			break;
		}
	}
	
	BYTE _class = pItemBaseInfo->theClass;
	
	switch( _class )
	{
	case n_Class_Weapon:		// 1 ����
	case n_Class_Shield:		// 8 ����
		{
			return( n_STORE_WEAPON );
		}
	case n_Class_CoatArmor:		// 2 ������� - ����
	case n_Class_PantsArmor:	// 3 ������� - ����
	case n_Class_Gloves:		// 4 ������� - �尩
	case n_Class_Shoes:			// 5 ������� - ����
		{
			return( n_STORE_ARMOR );
		}
	case n_Class_Helmet:		// 0 �Ӹ�
	case n_Class_Serve:			// 6 �������(����, ����, ��)
	case n_Class_Accessory:		// 7 ��ű�
	case n_Class_Face:			// 12 �� 			
		{
			return( n_STORE_SUNDRIES );
		}
	case n_Class_Use:			// 9 �Ҹ�ǰ
	case n_Class_Useless:		// 10 ��Ҹ�ǰ
	case n_Class_Special:		// 11 Ư�������� 
	case n_Class_Quest:			// 13 ����Ʈ ������ 
	case n_Class_Eldastone:		// 14 ���ٽ���
	case n_Class_FrontierPet:	// 15 �ż� �о�� 
	case n_Class_Weapon_StrengthStone:
	case n_Class_Armor_StrengthStone:
		{
			return( n_STORE_USE );
		}
	}
	
	return( n_STORE_NONE );
}

BOOL Player_Manager::IsColonyItem( SItemBaseInfo * pItemBaseInfo )
{

//������ ������ �� �浹�� �� �Ͻ������� �ּ��� ġ�ڽ��ϴ�. 
/*
	if( pItemBaseInfo == NULL )
	{
		return FALSE;
	}	
	
	BYTE _class = pItemBaseInfo->theClass;
	BYTE _Type =  pItemBaseInfo->theType; 
	
	if( _class == n_Class_Use && _Type == n_Type_Scroll )
	{
		WORD  wTexID = pItemBaseInfo->theReMP;		
		if( wTexID )
		{
			WORD _CallType = pItemBaseInfo->theReHPSpeed;
			if( _CallType )
			{
				return TRUE;
			}
		}
	}	
*/

	return FALSE;
}

BOOL Player_Manager::IsCanCallColonyItem()
{
	///-- �� ����Ƽ�� �����Ͱ� �ƴ϶�� ���Ұ�	
	
	///-- ���� ���� �ݷδ� ���� �ƴϸ� ���Ұ�
	
	///-- ���� �������� ���� �ð��� �ƴ϶�� ���Ұ�	
	
	return TRUE;
}

//-----------------------------------------------------------------------------
SQuestInfo*	Player_Manager::GetQuestInfoTable()
{
	return( theQuestInfoTable );
}

//-----------------------------------------------------------------------------
int	Player_Manager::GetQuestType( WORD _quest_code )
{
	//1:�Ϲ�, 2:���丮, 3:����
	for( int i = 0; i < c_Max_Quest_Count; ++i )
	{
		if( theQuestInfoTable[i].theCode == _quest_code )
		{
			return( theQuestInfoTable[i].theQOpenType );
		}
	}
	return( 0 );
}

//-----------------------------------------------------------------------------
SQuestInfo*	Player_Manager::GetQuestFlagInfo( WORD _quest_code, WORD _flag )
{
	for( int i = 0; i < c_Max_Quest_Count; ++i )
	{
		if( ( theQuestInfoTable[i].theCode == _quest_code ) && ( theQuestInfoTable[i].theFlag == _flag ) )
		{
			return( &theQuestInfoTable[i] );
		}
	}
	
	return( NULL );
}

//-----------------------------------------------------------------------------
bool Player_Manager::ReadScriptStrTable( const char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos );
	//sprintf( file_name, "\\Chr\\ScriptStr.csd");
	sprintf( file_name, "\\local_test\\ScriptStr.csd");
	strcat( file_path, file_name);
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		::MessageBox(NULL,"Cannot Found ScriptStr.csd",0,0);
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	//	if( ( file_buf.st_size % sizeof( SScriptString ) ) != 0 )
	//	{
	//		return( false );
	//	}
//	SScriptString a;
/*
	
	int Table_Num = file_buf.st_size / sizeof( SScriptString );
	
	if( Table_Num >= c_Max_ScriptStr_Num )
	{
		MessageBox( NULL, "<ScriptStr.csd> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
*/

	//	memset( &theScriptStrTable, 0, sizeof(SScriptString)*c_Max_ScriptStr_Num );
	
	for( int i = 0; i < c_Max_ScriptStr_Num; ++i )
	{
		if( fread( &theScriptStrTable[i].theStringIndex, sizeof(DWORD), 1, file ) <= 0 )
		{
			break;
		}
		
		if( fread( &theScriptStrTable[i].theStringLen, sizeof(BYTE), 1, file ) <= 0 )
		{		
			break;
		}
		else
		{
			theScriptStrTable[i].theString = SAFE_NEW_ARRAY( char , theScriptStrTable[i].theStringLen + 1 );			
		}
		
		if( fread( theScriptStrTable[i].theString, theScriptStrTable[i].theStringLen, 1, file ) <= 0 )
		{
			break;
		}
		theScriptStrTable[i].theString[theScriptStrTable[i].theStringLen] = NULL;
	}
	
	fclose( file );
	
	return true;
}


bool Player_Manager::ReadScriptStrTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "ScriptStr.csd");	
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;	

	/*
	int Table_Num = pFH->GetSize() / sizeof( SScriptString );
	
	if( Table_Num >= c_Max_ScriptStr_Num )
	{
		MessageBox( NULL, "<ScriptStr.csd> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	*/


	//	memset( theScriptStrTable, 0, sizeof(SScriptString)*c_Max_ScriptStr_Num );
	
	for( int i = 0; i < c_Max_ScriptStr_Num; ++i )
	{
		if( pFH->Read( &theScriptStrTable[i].theStringIndex, sizeof(DWORD) ) <= 0 )
		{
			break;
		}

#ifdef NPC_CHAT_EXTENSION

		if( pFH->Read( &theScriptStrTable[i].theStringLen, sizeof(WORD) ) <= 0 ) 
		{		
			break;
		}
#else 
		if( pFH->Read( &theScriptStrTable[i].theStringLen, sizeof(BYTE) ) <= 0 ) 
		{			
			break;
		}
#endif 
		else 
		{
			theScriptStrTable[i].theString = SAFE_NEW_ARRAY( char , theScriptStrTable[i].theStringLen + 2 );			
			ZeroMemory(theScriptStrTable[i].theString , sizeof(char) * theScriptStrTable[i].theStringLen + 2 );
		}
		

		if( pFH->Read( theScriptStrTable[i].theString, theScriptStrTable[i].theStringLen ) <= 0 )
		{
			break;
		}
		
		theScriptStrTable[i].theString[theScriptStrTable[i].theStringLen+1 ] = NULL; 
	}
	
	g_VFChrRoot.CloseFile( pFH );
	
	return true;
}


void Player_Manager::ConvertNameString( void* paOrigin, void* paResult )
{
#ifdef UNICODE_ROCK
	WCHAR *result =NULL;
	WCHAR *aOrigin = (WCHAR*)(paOrigin);
	WCHAR *aResult = (WCHAR*)(paResult);
#else
	char *result =NULL;
	char *aOrigin = (char*)(paOrigin);
	char *aResult = (char*)(paResult);
#endif



#ifdef NPC_CHAT_EXTENSION
	const int MaxStr = 2000; 
#else 
	const int MaxStr = 256; 
#endif 


#ifdef NPC_CHAT_EXTENSION

	
	//aResult[0] = '\0';


	RTCHAR rtResult[4048] = {0,};
	SAFE_STR_CPY( rtResult , aOrigin , MaxStr );

//goCHECK_USERNMAE:
	
	bool bUserNameProcess = false; 
	
	while( (result = Rstrstr( rtResult , _RT("%UserName%"))) != NULL )
	{
		Rstrcpy( aResult , _RT(""));
		int pos = result - rtResult  + 1;
		
		Rstrncat( aResult, rtResult, pos-1 );
		SAFE_STR_CAT( aResult, nRui->thePcParam.PCName, MaxStr );
		SAFE_STR_CAT( aResult, rtResult+pos+Rstrlen(_RT("%UserName%"))-1, MaxStr );
		
		//if( Rstrstr( aResult , _RT("%UserName%")) != NULL )
		//{
		SAFE_STR_CPY( rtResult , aResult  , MaxStr ); 
//		/}

		bUserNameProcess =true; 
	}

	if(bUserNameProcess )
		return;
	



	


#else 

	//aResult[0] = '\0';
	Rstrcpy( aResult , _RT(""));
	result = Rstrstr( aOrigin , _RT("%UserName%") );
	
	if(result != NULL)
	{
		int pos = result - aOrigin  + 1;
		
		Rstrncat( aResult, aOrigin, pos-1 );
		SAFE_STR_CAT( aResult, nRui->thePcParam.PCName, MaxStr );
		SAFE_STR_CAT( aResult, aOrigin+pos+Rstrlen(_RT("%UserName%"))-1, MaxStr );
		return;
	}
	
	

#endif 

	
	char TempPetStateStr[128]={0,};
	
	char temp1[MaxStr]= {0,};
	char temp2[MaxStr]= {0,};
	
	
	//ó�� ���� �˻��Ѵ�
	result = Rstrstr( aOrigin, _RT("%PET0_NAME%") );
	
	if(result != NULL)
	{
		int pos = result - aOrigin + 1;
		
		Rstrncat( RWCHAR(temp1), aOrigin, pos-1 );
		if(nRui->thePcInven.thePetData[0])
		{
			switch(nRui->thePcInven.thePetData[0]->PetState	)
			{
			case n_Pet_Die:
				Rsprintf(RWCHAR(TempPetStateStr), _RT("%s %s"), RWCHAR(nRui->thePcInven.thePetData[0]->Name) , G_STRING_CHAR(IDS_PET_DIE_WND));break;
			case n_Pet_Custody:
				Rsprintf(RWCHAR(TempPetStateStr), _RT("%s %s"), RWCHAR(nRui->thePcInven.thePetData[0]->Name) , G_STRING_CHAR(IDS_PET_CUSTODY_WND));break;
			default:
				Rsprintf(RWCHAR(TempPetStateStr), _RT("%s %s"), RWCHAR(nRui->thePcInven.thePetData[0]->Name) , _RT(""));break;
			}
			
			SAFE_STR_CAT( temp1, TempPetStateStr, MaxStr );
			
		}
		else 
		{
			SAFE_STR_CAT( temp1, _RT("NO PET"), MaxStr );
		}

		SAFE_STR_CAT( temp1, aOrigin+pos+Rstrlen(_RT("%PET0_NAME%"))-1, MaxStr );		
	}
	
	Rstrcpy(aResult,RWCHAR(temp1));
	
	result = Rstrstr( RWCHAR(temp1), _RT("%PET1_NAME%") );
	
	if(result != NULL)
	{
		int pos = result - RWCHAR(temp1 )+ 1;
		
		Rstrncat( RWCHAR(temp2), RWCHAR(temp1), pos-1 );
		if(nRui->thePcInven.thePetData[1])
		{
			switch(nRui->thePcInven.thePetData[1]->PetState	)
			{
			case n_Pet_Die:
				Rsprintf(RWCHAR(TempPetStateStr), _RT("%s %s"), RWCHAR(nRui->thePcInven.thePetData[1]->Name) , G_STRING_CHAR(IDS_PET_DIE_WND));break;
			case n_Pet_Custody:
				Rsprintf(RWCHAR(TempPetStateStr), _RT("%s %s"), RWCHAR(nRui->thePcInven.thePetData[1]->Name) , G_STRING_CHAR(IDS_PET_CUSTODY_WND));break;
			default:
				Rsprintf(RWCHAR(TempPetStateStr), _RT("%s %s"), RWCHAR(nRui->thePcInven.thePetData[1]->Name) , _RT(""));break;
			}
			SAFE_STR_CAT( temp2, TempPetStateStr, MaxStr );
			
		}
		else 
		{
			SAFE_STR_CAT( temp2, _RT("NO PET"), MaxStr );
		}
		
		SAFE_STR_CAT( temp2, RWCHAR(temp1)+pos+Rstrlen(_RT("%PET1_NAME%"))-1, MaxStr );
		
	}
	
	memset(temp1, 0 , sizeof(temp1));
	Rstrcpy(aResult,RWCHAR(temp2));
	
	result = Rstrstr( RWCHAR(temp2), _RT("%PET2_NAME%") );
	
	if(result != NULL)
	{
		int pos = result - RWCHAR(temp2) + 1;
		
		Rstrncat( RWCHAR(temp1), RWCHAR(temp2), pos-1 );
		if(nRui->thePcInven.thePetData[2])
		{
			switch(nRui->thePcInven.thePetData[2]->PetState	) {
			case n_Pet_Die:
				Rsprintf(RWCHAR(TempPetStateStr), _RT("%s %s"), RWCHAR(nRui->thePcInven.thePetData[2]->Name) , G_STRING_CHAR(IDS_PET_DIE_WND));break;
			case n_Pet_Custody:
				Rsprintf(RWCHAR(TempPetStateStr), _RT("%s %s"), RWCHAR(nRui->thePcInven.thePetData[2]->Name) , G_STRING_CHAR(IDS_PET_CUSTODY_WND));break;
			default:
				Rsprintf(RWCHAR(TempPetStateStr), _RT("%s %s"), RWCHAR(nRui->thePcInven.thePetData[2]->Name) , _RT(""));break;
			}
			SAFE_STR_CAT( temp1, TempPetStateStr, MaxStr );
			
		}
		else 
		{
			SAFE_STR_CAT( temp1, _RT("NO PET"), MaxStr );
		}
		
		SAFE_STR_CAT( temp1, RWCHAR(temp2)+pos+Rstrlen(_RT("%PET2_NAME%"))-1, MaxStr );
		
	}

	Rstrcpy(aResult,RWCHAR(temp1));	
	
	if( result == NULL )
	{	
		Rstrcpy( aResult, aOrigin);		
	}	
	
	return;
}

bool Player_Manager::LoadSkillLessonTable( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos);
	sprintf( file_name, "\\Chr\\SkillLesson.dat");
	strcat( file_path, file_name);		
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SSkillLessonInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SSkillLessonInfo );
	
	if( Table_Num >= c_Max_Skill_Num )
	{
		MessageBox( NULL, "<SkillLesson.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( &theSkillLessonInfoTable, 0, sizeof( SSkillLessonInfo ) * c_Max_Skill_Num );
	
	for( int i = 0; i < c_Max_Skill_Num; ++i )
	{
		if( fread( &theSkillLessonInfoTable[i], sizeof( SSkillLessonInfo ), 1, file ) <= 0 )
		{
			break;
		}
	}
	
	fclose( file );
	
	return true;
}

bool Player_Manager::LoadSkillLessonTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "SkillLesson.dat");	
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;	
	
	int Table_Num = pFH->GetSize() / sizeof( SSkillLessonInfo );
	
	int size = sizeof(SSkillLessonInfo);

	if( Table_Num >= c_Max_Skill_Num )
	{
		MessageBox( NULL, "<SkillLesson.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( &theSkillLessonInfoTable, 0, sizeof( SSkillLessonInfo ) * c_Max_Skill_Num );
	
	for( int i = 0; i < c_Max_Skill_Num; ++i )
	{
		if( pFH->Read( &theSkillLessonInfoTable[i], sizeof( SSkillLessonInfo ) ) <= 0 )
		{
			break;
		}
	}
	
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 
	
	return true;
}



bool Player_Manager::ReadAutoHelpTip( char* file_pos )
{
	
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos);
	sprintf( file_name, "\\local_test\\AutoHelpTip.dat");
	//sprintf( file_name, "\\Etc\\AutoHelpTip.dat");
	strcat( file_path, file_name);		
	
	FILE*	file;
	file = fopen( file_path, "rt" );
	if( file == NULL )
	{
		::MessageBox(NULL,"Cannot Found AutoHelpTip.dat",0,0);
		return false;
	}
	
	
	char* token;					// ��ū
	char string [256];				// ���� �ٴ��� ���� ����
	bool bRead  = true;				// ���� �� �о����� üũ			
	char splitter[] = "\t\n";		// ������ : ĳ��������
	int MsgType = 0;
	
	
	theAutoHelpTipMsg.theAutoHelpTipCurIdx = 0;
	theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena = 0;
	theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain = 0;
	theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat = 0;
	
	
	// �� ������ �б�
	if( fgets( string, 256, file ) == NULL )				
		return false;				
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( fgets( string, 256 , file ) == NULL )		
			{
				bRead = false;
				break;
			}
		}
		
		// �ε��� 
		token = strtok( string, splitter );
		if( token == NULL )			
			break;	
		
		MsgType = atoi(token);	
		
		token = strtok( NULL, splitter );
		
		if( MsgType == 0 )			// �з��� ����
		{
			sprintf( theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Millena[theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena], "%s", token );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena++;
		}
		else if( MsgType == 1 )		// ���� ����
		{
			sprintf( theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Rain[theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain], "%s", token );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain++;
		}
		else if( MsgType == 2 )		// ���� ����
		{
			sprintf( theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Millena[theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena], "%s", token );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena++;
			
			sprintf( theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Rain[theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain], "%s", token );			
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain++;
		}
		else if( MsgType == 3 )		// PVP ä�� ����
		{
			sprintf( theAutoHelpTipMsg.theAutoHelpTipAdminMsg_PVPChat[theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat], "%s", token );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat++;
		}
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( fgets( string, 256 , file ) == NULL )		
			bRead = false;			
	}
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	fclose( file );	
	
	theAutoHelpTipMsg.theAutoHelpTipTime = g_nowTime;
	
	return true;
}


bool Player_Manager::ReadAutoHelpTipVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "AutoHelpTip.dat" );	
	
	VFileHandle* pFH = g_VFEtc.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
	char* token;				// ��ū
	char string [256];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ			
	char splitter[] = "\n\t";		// ������ : /, ĳ��������, �� 
	int	string_cnt = 1;
	int MsgType = 0;	
	
	
	theAutoHelpTipMsg.theAutoHelpTipCurIdx = 0;
	theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena = 0;
	theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain = 0;
	theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat = 0;
	
	
	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == NULL )
	{
		g_VFEtc.CloseFile( pFH );
		return false; 
	}	
	do 
	{		
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == NULL )		
			{
				bRead = false;
				break;
			}
		}
		
		// �ε��� 
		token = strtok( string, splitter );
		if( token == NULL )			
			break;	
		
		MsgType = atoi(token);	
		
		token = strtok( NULL, splitter );
		
		if( MsgType == 0 )			// �з��� ����
		{
			sprintf( theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Millena[theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena], "%s", token );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena++;
		}
		else if( MsgType == 1 )		// ���� ����
		{
			sprintf( theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Rain[theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain], "%s", token );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain++;
		}
		else if( MsgType == 2 )		// ���� ����
		{
			sprintf( theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Millena[theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena], "%s", token );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena++;
			
			sprintf( theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Rain[theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain], "%s", token );			
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain++;
		}
		else if( MsgType == 3 )		// PVP ä�� ����
		{
			sprintf( theAutoHelpTipMsg.theAutoHelpTipAdminMsg_PVPChat[theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat], "%s", token );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat++;
		}
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == NULL )
		{
			bRead = false;
			break;
		}	
	} while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	
	g_VFEtc.CloseFile( pFH );
	
	theAutoHelpTipMsg.theAutoHelpTipTime = g_nowTime;
	
	return true;
}

bool Player_Manager::ReadAutoHelpTipVFW()
{
	char	file_name[256] = {0,};
	sprintf( file_name, "AutoHelpTip.dat" );	
	
	VFileHandle* pFH = g_VFEtc.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
	WCHAR* token;				// ��ū
	WCHAR string [256];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ			
	WCHAR splitter[] = L"\r\n\t";		// ������ : /, ĳ��������, �� 
	int	string_cnt = 1;
	int MsgType = 0;	
	
	
	theAutoHelpTipMsg.theAutoHelpTipCurIdx = 0;
	theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena = 0;
	theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain = 0;
	theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat = 0;

	pFH->Seek(2 , SEEK_SET);
	
	// �� ������ �б�
	if( pFH->Getws( string, sizeof( string ) ) == NULL )
	{
		g_VFEtc.CloseFile( pFH );
		return false; 
	}	
	do 
	{		
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Getws( string, sizeof( string ) ) == NULL )		
			{
				bRead = false;
				break;
			}
		}
		
		// �ε��� 
		token = wcstok( string, splitter );
		if( token == NULL )			
			break;	
		
		MsgType = _wtol(token);	
		
		token = wcstok( NULL, splitter );
		
		if( MsgType == 0 )			// �з��� ����
		{
			Rsprintf( RWCHAR(theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Millena[theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena]), _RT("%s"), RWCHAR(token) );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena++;
		}
		else if( MsgType == 1 )		// ���� ����
		{
			Rsprintf( RWCHAR(theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Rain[theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain]), _RT("%s"), RWCHAR(token) );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain++;
		}
		else if( MsgType == 2 )		// ���� ����
		{
			Rsprintf( RWCHAR(theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Millena[theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena]), _RT("%s"), RWCHAR(token) );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Millena++;
			
			Rsprintf( RWCHAR(theAutoHelpTipMsg.theAutoHelpTipAdminMsg_Rain[theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain]), _RT("%s"), RWCHAR(token) );			
			theAutoHelpTipMsg.theAutoHelpTipStrNum_Rain++;
		}
		else if( MsgType == 3 )		// PVP ä�� ����
		{
			Rsprintf( RWCHAR(theAutoHelpTipMsg.theAutoHelpTipAdminMsg_PVPChat[theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat]), _RT("%s"), RWCHAR(token) );
			theAutoHelpTipMsg.theAutoHelpTipStrNum_PVPChat++;
		}
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Getws( string, sizeof( string ) ) == NULL )
		{
			bRead = false;
			break;
		}	
	} while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	
	g_VFEtc.CloseFile( pFH );
	
	theAutoHelpTipMsg.theAutoHelpTipTime = g_nowTime;
	
	return true;
}

bool Player_Manager::LoadItemAddEffectInfoTable( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos );
	sprintf( file_name, "\\Chr\\ItemAddEff.dat");
	strcat( file_path, file_name);		
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SItemAddEffectInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SItemAddEffectInfo );
	
	if( Table_Num >= c_Max_ItemAddEffect_Num )
	{
		MessageBox( NULL, "<ItemAddEff.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	
	memset( &theItemAddEffectInfoTable, 0, sizeof( SItemAddEffectInfo ) * c_Max_ItemAddEffect_Num );
	
	for( int i = 0; i < c_Max_ItemAddEffect_Num; ++i )
	{
		if( fread( &theItemAddEffectInfoTable[i], sizeof( SItemAddEffectInfo ), 1, file ) <= 0 )
		{
			break;
		}
	}
	
	fclose( file );
	
	return true;
}

bool Player_Manager::LoadItemAddEffectInfoTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "ItemAddEff.dat");	
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;	
	
	int Table_Num = pFH->GetSize() / sizeof( SItemAddEffectInfo );
	
	if( Table_Num >= c_Max_ItemAddEffect_Num )
	{
		MessageBox( NULL, "<ItemAddEff.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( &theItemAddEffectInfoTable, 0, sizeof( SItemAddEffectInfo ) * c_Max_ItemAddEffect_Num );
	
	for( int i = 0; i < c_Max_ItemAddEffect_Num; ++i )
	{
		if( pFH->Read( &theItemAddEffectInfoTable[i], sizeof( SItemAddEffectInfo ) ) <= 0 )
		{
			break;
		}
	}
	
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 
	
	return true;
}


bool Player_Manager::LoadAddEffectInfoTable( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos);
	sprintf( file_name, "\\Chr\\AddEffect.dat");
	strcat( file_path, file_name );		
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SAddEffectInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SAddEffectInfo );
	
	if( Table_Num >= c_Max_AddEffect_Num )
	{
		MessageBox( NULL, "<AddEffect.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	
	memset( &theAddEffectInfoTable, 0, sizeof( SAddEffectInfo ) * c_Max_AddEffect_Num );
	
	for( int i = 0; i < c_Max_AddEffect_Num; ++i )
	{
		if( fread( &theAddEffectInfoTable[i], sizeof( SAddEffectInfo ), 1, file ) <= 0 )
		{
			break;
		}
	}
	
	fclose( file );
	
	return true;
}

bool Player_Manager::LoadAddEffectInfoTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "AddEffect.dat");	
	
	VFileHandle* pFH = g_VFChrRoot.OpenFile( file_name );
	if( pFH == NULL ) return false;	
	
	int Table_Num = pFH->GetSize() / sizeof( SAddEffectInfo );
	
	if( Table_Num >= c_Max_AddEffect_Num )
	{
		MessageBox( NULL, "<AddEffect.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( &theAddEffectInfoTable, 0, sizeof( SAddEffectInfo ) * c_Max_AddEffect_Num );
	
	for( int i = 0; i < c_Max_AddEffect_Num; ++i )
	{
		if( pFH->Read( &theAddEffectInfoTable[i], sizeof( SAddEffectInfo ) ) <= 0 )
		{
			break;
		}
	}
	
	g_VFChrRoot.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 
	
	return true;
}

int Player_Manager::GetMapNationType( int aMapIndex )
{
	int nation_type = n_Nation_Common;
	
	
	/*switch( aMapIndex )
	{
	case c_RockMap_Millena_City:
	case c_RockMap_Millena_Field1_TwinibleWood_Gate:
	case c_RockMap_Millena_Field2_MemoryOfPlain:
	case c_RockMap_Millena_Academy:
	case c_RockMap_Millena_Dongeon1_TrajaOfSwamp: 
	case c_RockMap_Millena_Field3_WildWindOfWasteLand:
	case c_RockMap_Millena_Field4_Iinduf_BoundaryArea:	
	case c_RockMap_Millena_Field5_TearOfBreaker:
	case c_RockMap_Millena_Field6_WoodOfFate:
	case c_RockMap_Millena_Dongeon2_WoodOfHesitate:
	case c_RockMap_Millena_ForgottenSea:
	case c_RockMap_Millena_SpiralMountain:
	case c_RockMap_Millena_Arena27:
	case c_RockMap_Millena_Academy29:
	case c_RockMap_Millena_Arena31:
	case c_RockMap_Millena_Haku_Town35:
	case c_RockMap_Millena_Dongeon1_CaveOfExile37:	
	{
	nation_type = n_Nation_Millena;
	}
	break;
	case c_RockMap_Rain_City:
	case c_RockMap_Rain_Academy:
	case c_RockMap_Rain_Field1_GoldenOfHill:
	case c_RockMap_Rain_Field2_GaiaOfVally:
	case c_RockMap_Rain_Field3_SnowField:
	case c_RockMap_Rain_Field4_Ilisian_RuinArea:				
	case c_RockMap_Rain_Field5_PoolOfKabala:
	case c_RockMap_Rain_Dongeon1_CaveOfStart:	
	case c_RockMap_Rain_Field6_HillOfFate:
	case c_RockMap_Rain_Dongeon2_SwampOfPhilog:
	case c_RockMap_Rain_FrozenLake:
	case c_RockMap_Rain_SpiralMountain:
	case c_RockMap_Rain_Arena28:
	case c_RockMap_Rain_Academy30:
	case c_RockMap_Rain_Arena32:			
	case c_RockMap_Rain_Jede_Town36:
	case c_RockMap_Rain_Dongeon1_MazeOfArmis38:	
	{
	nation_type = n_Nation_Rain;
	}
	break;	
	default:
	{
	nation_type = n_Nation_Common;
	}
	break;
}*/
	
	return nation_type;
}

bool Player_Manager::CheckLoadAniList_LoginFeild( int aFileName )
{
	// ����...�ε� ������ ������ �ִ�...�׳� �ϵ��ڵ�����...������...T,.T Ū!! 
	// �� ã�Ƽ� �־���� �ϴٴ�...�밡��...�ٲ�� ���д�...T,.T;; 
	
	// �⺻���, �޸���, ĳ���� ����â ����ڼ�, �̸����� 
	
	// �̸����� �ִϸ��̼� ���� �ε� 
	// Ŭ������ȣ	����(����:0,�з���:1)	����(��:0,��:1)   �ִ� �ε��� 
	/*
	2	1	0		126	132	223	
	6	1	0		258	141	143	
	8	1	0		255	84	225	
	
	  3	1	1		239	240	50 	
	  7	1	1		256			
	  9	1	1		257			
	  
		5	0	0		253			
		12	0	0		256			
		13	0	0		217	308		
		
		  4	0	1		252			
		  11	0	1		255			
		  10	0	1		254			
	*/
	
	UINT lui_FileName[100] = { 
		// �з��� ����
		49909999, 
			40900100, 40901100, 40902100, 40903100, 40909100, 
			40900402, 40901400, 40902402, 40903402, 40909402, 
			40902311, 40902322, 40902411, 40903601, 40903301, 
			40903303, 40900306, 40900304, 40900411, 
			40908100, 40908200, 40908400, 40908402,
			// �з��� ����
			49919999, 
			40910100, 40911100, 40912100, 40913100, 40919100, 
			40910402, 40911400, 40912402, 40913402, 40919402, 
			40911411, 40911412, 40911400, 40912701, 40911701, 
			40918100, 40918200, 40918400, 40918402,											
			// ���� ����
			49909999, 
			41905100, 41906100, 41907100, 41909100, 
			41905402, 41906402, 41907400, 41909402,
			41907601, 41906701, 41907335, 41907605, 
			41908100, 41908200, 41908400, 41908402,
			// ���� ����
			49919999, 
			41915100, 41916100, 41917100, 41919100, 
			41915402, 41916402, 41917400, 41919402,
			41915701, 41916701, 41915702, 
			41918100, 41918200, 41918400, 41918402,
			0, 			
	};
	
	for( int idx = 0; idx < 100; ++idx )
	{
		if( lui_FileName[idx] == 0 )
		{
			break;
		}
		
		if( lui_FileName[idx] == aFileName )
		{
			return true;
		}
	}
	
	return false;
}


bool Player_Manager::LoadFightMapInfoTable( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos);
	sprintf( file_name, "\\Etc\\FightMap.dat");
	strcat( file_path, file_name );		
	
	FILE*	file;
	file = fopen( file_path, "rb" );
	if( file == NULL )
	{
		return false;
	}
	
	struct _stat file_buf;
	int result = _stat( file_path, &file_buf );
	
	if( result != 0 )
	{
		return( false );
	}
	if( ( file_buf.st_size % sizeof( SFightMapInfo ) ) != 0 )
	{
		return( false );
	}
	
	int Table_Num = file_buf.st_size / sizeof( SFightMapInfo );
	
	if( Table_Num >= c_Max_FightMap_Num )
	{
		MessageBox( NULL, "<FightMap.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( &theFightMapInfoTable, 0, sizeof( SFightMapInfo ) * c_Max_FightMap_Num );
	
	for( int i = 0; i < c_Max_FightMap_Num; ++i )
	{
		if( fread( &theFightMapInfoTable[i], sizeof( SFightMapInfo ), 1, file ) <= 0 )
		{
			break;
		}
	}
	
	fclose( file );
	
	// �� ���� ����  
	g_BZ_WaitingRoomWnd.SetFightMapInfo();
	
	return true;
}

bool Player_Manager::LoadFightMapInfoTableVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "FightMap.dat");	
	
	VFileHandle* pFH = g_VFEtc.OpenFile( file_name );
	if( pFH == NULL ) return false;	
	
	int Table_Num = pFH->GetSize() / sizeof( SFightMapInfo );
	
	if( Table_Num >= c_Max_FightMap_Num )
	{
		MessageBox( NULL, "<FightMap.dat> Loading Max Over", "Error", MB_OK );		
		
		return false;
	}
	
	memset( &theFightMapInfoTable, 0, sizeof( SFightMapInfo ) * c_Max_FightMap_Num );
	
	for( int i = 0; i < c_Max_FightMap_Num; ++i )
	{
		if( pFH->Read( &theFightMapInfoTable[i], sizeof( SFightMapInfo ) ) <= 0 )
		{
			break;
		}
	}
	
	g_VFEtc.CloseFile( pFH );	/// �ݵ�� �ݾƾ� �� 
	
	// �� ���� ����  
	g_BZ_WaitingRoomWnd.SetFightMapInfo();
	
	/*
	typedef struct _SFightMapInfo
	{
	struct _SFightPos
	{
	WORD	theXCell;
	WORD	theZCell;
	};
	struct _SNPCPos
	{
	WORD	theNPCCode;
	WORD	theXCell;
	WORD	theZCell;
	};
	struct _SRewardItem
	{
	WORD	theItemCode;
	BYTE	theRate;
	};
	WORD			theMapCode;
	BYTE			theGameType;
	_SFightPos		thePosition[12];	// 0-5 : 1�� , 6-11 : 2�� 
	_SNPCPos		theNPCPosition[12];	// 0-5 : 1�� , 6-11 : 2�� 
	_SRewardItem	theRewardItem[6];
	} SFightMapInfo, * SFightMapInfoPtr;
	*/
	return true;
}


bool Player_Manager::ReadGameModeDesc( char* file_pos )
{
	char	file_path[256] = {0,};
	char	file_name[256] = {0,};
	
	strcpy( file_path, file_pos );
	sprintf( file_name, "\\Etc\\GameModeDesc.dat");
	strcat( file_path, file_name);		
	
	FILE*	file;
	file = fopen( file_path, "rt" );
	if( file == NULL )
		return false;
	
	char* token;					// ��ū
	char string [256];				// ���� �ٴ��� ���� ����
	bool bRead  = true;				// ���� �� �о����� üũ			
	char splitter[] = "\t\n";		// ������ : ĳ��������
	int	str_length = 0;
	int GameType = 0;	
	
	
	// �� ������ �б�
	if( fgets( string, 256, file ) == NULL )				
		return false;				
	do 
	{	
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( fgets( string, 256 , file ) == NULL )		
			{
				bRead = false;
				break;
			}
		}
		
		// �ε��� 
		token = strtok( string, splitter );
		if( token == NULL )			
			break;	
		
		// 0 : ������ġ, 1 : ���� ��ġ, 2 : ��� ����, 3 : ���� ���� 
		GameType = atoi(token);	
		if( GameType >= c_Max_GameMode_Num )
		{
			break;
		}
		
		token = strtok( NULL, splitter );
		str_length = strlen( token );
		
		if( str_length )
		{
			theGameModeDesc[GameType] = SAFE_NEW_ARRAY( char , str_length );			
			
			if( theGameModeDesc[GameType] )
			{
				sprintf( theGameModeDesc[GameType], "%s", token );			
			}
		}
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( fgets( string, 256 , file ) == NULL )		
			bRead = false;			
	}
	while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	fclose( file );	
	
	return true;
}

bool Player_Manager::ReadGameModeDescVFW()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "GameModeDesc.dat" );	
	
	VFileHandle* pFH = g_VFEtc.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
	WCHAR* token;				// ��ū
	WCHAR string [256] = {0,};			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ			
	WCHAR splitter[] = L"\r\n\t";		// ������ : /, ĳ��������, �� 
	int	str_length = 0;
	int GameType = 0;	
	
	pFH->Seek(2 , SEEK_SET );
	
	
	// �� ������ �б�
	if( pFH->Getws( string, sizeof( string ) ) == NULL )
	{
		g_VFEtc.CloseFile( pFH );
		return false; 
	}	
	do 
	{		
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Getws( string, sizeof( string ) ) == NULL )		
			{
				bRead = false;
				break;
			}
		}
		
		// �ε��� 
		token = wcstok( string, splitter );
		if( token == NULL )			
			break;	
		
		// 0 : ������ġ, 1 : ���� ��ġ, 2 : ��� ����, 3 : ���� ���� 
		GameType = _wtoi(token);						
		if( GameType >= c_Max_GameMode_Num )
		{
			break;
		}
		
		token = wcstok( NULL, splitter );
		str_length = (wcslen( token )+1)*2;
		
		if( str_length )
		{
			if( RWCHAR(theGameModeDesc[GameType]) == NULL )
			{
				theGameModeDesc[GameType] = SAFE_NEW_ARRAY( char , str_length );			
			}
			
			if( theGameModeDesc[GameType] )
			{
				swprintf( reinterpret_cast<WCHAR*>(theGameModeDesc[GameType]), L"%s", token );			
			}
		}
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Getws( string, sizeof( string ) ) == NULL )
		{
			bRead = false;
			break;
		}	
	} while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	
	g_VFEtc.CloseFile( pFH );
	
	return true;
}


bool Player_Manager::ReadGameModeDescVF()
{
	char	file_name[256] = {0,};
	
	sprintf( file_name, "GameModeDesc.dat" );	
	
	VFileHandle* pFH = g_VFEtc.OpenFile( file_name );
	if( pFH == NULL ) return false;
	
	char* token;				// ��ū
	char string [256];			// ���� �ٴ��� ���� ����
	bool bRead  = true;			// ���� �� �о����� üũ			
	char splitter[] = "\n\t";		// ������ : /, ĳ��������, �� 
	int	str_length = 0;
	int GameType = 0;	
	
	
	// �� ������ �б�
	if( pFH->Gets( string, sizeof( string ) ) == NULL )
	{
		g_VFEtc.CloseFile( pFH );
		return false; 
	}	
	do 
	{		
		// �ּ��� �ִٸ� ���� �� ����
		while( string[0] == '/' && string[1] == '/' )	
		{				
			// �� �̻� ������ ���ٸ� ���� ����
			if( pFH->Gets( string, sizeof( string ) ) == NULL )		
			{
				bRead = false;
				break;
			}
		}
		
		// �ε��� 
		token = strtok( string, splitter );
		if( token == NULL )			
			break;	
		
		// 0 : ������ġ, 1 : ���� ��ġ, 2 : ��� ����, 3 : ���� ���� 
		GameType = atoi(token);						
		if( GameType >= c_Max_GameMode_Num )
		{
			break;
		}
		
		token = strtok( NULL, splitter );
		str_length = strlen( token );
		
		if( str_length )
		{
			if( theGameModeDesc[GameType] == NULL )
			{
				theGameModeDesc[GameType] = SAFE_NEW_ARRAY( char , str_length + 1 );			
			}
			
			if( theGameModeDesc[GameType] )
			{
				sprintf( theGameModeDesc[GameType], "%s", token );			
			}
		}
		
		// ���� �� �б�, ������ ���ٸ� string�� 0����									
		if( pFH->Gets( string, sizeof( string ) ) == NULL )
		{
			bRead = false;
			break;
		}	
	} while( bRead != false ); // �� �̻� ������ ���ٸ� ���� ����	
	
	g_VFEtc.CloseFile( pFH );
	
	return true;
}


BOOL Player_Manager::ISUseSkillOnApproachWeapon(int Code)
{
	if( Code >= c_SkillCode_BreathControl_Min && c_SkillCode_BreathControl_Max >= Code )
	{
		return TRUE;
	}
	else if( Code >= c_SkillCode_Meditation_Min && c_SkillCode_Meditation_Max >= Code )
	{
		return TRUE;
	}
	else 
	{
		switch( Code )
		{
		case c_SkillCode_UserSellStore:
		case c_SkillCode_Basic_PickUp:
		case c_SkillCode_BasicAttack_Millena:
		case c_SkillCode_BasicAttack_Rain:
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL Player_Manager::AddNpcCode( int nNpcCode )
{
	char		file_path[256]	= {0,};			
	int			i = 0;	
	int			idx = 0;	
	int			itm_cnt = 0;
	int			msh_cnt = 0;
	int			tex_cnt = 0;
	int			ani_cnt = 0;	
	
	int number = the_sNpc_SetData.total_npc_num;
	
	if( number >= c_NpcType_Count )
	{
		return FALSE;
	}
	
	SNPCBaseInfo* npc_Info;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( nNpcCode );
	
	if( NULL == npc_Info )
	{
		return FALSE;
	}
	
	the_sNpc_SetData.file_name[number] = GetNPCRealFileCode( nNpcCode );
	the_sNpc_SetData.npc_code[number]  = nNpcCode;
	
	itm_cnt = the_sNpc_SetData.total_itm_num;
	msh_cnt = the_sNpc_SetData.total_msh_num;
	tex_cnt = the_sNpc_SetData.total_tex_num;
	ani_cnt = the_sNpc_SetData.total_ani_num;
	
	///--PRINT_DLG_LOG( "���ο� NPC" );
	
#ifdef DYNAMIC_RESOURCE_LOADING
	
	sprintf( file_path, "%d.chr", the_sNpc_SetData.file_name[number] );	  
	CHR_FILE_INFO * pNpcFileInfo = SAFE_NEW( CHR_FILE_INFO );	
	
	if( pNpcFileInfo->FileReadVF( &g_VFNpcRoot, file_path, SORT_NPC ) == true )
	{
		m_pNpcFileInfo.push_back( pNpcFileInfo );
		pNpcFileInfo->m_Index = the_sNpc_SetData.file_name[number];
		SetNpcFileCode( number, pNpcFileInfo->m_Index );
		
		for( idx = 0; idx < pNpcFileInfo->nItmCount; ++idx )
		{
			sprintf( file_path, "%d.itm", pNpcFileInfo->pItmIndex[idx] );			
			Character_Item * pNpcItem = SAFE_NEW( Character_Item );			
			
			if( pNpcItem->File__OpenVF( &g_VFNpcItm, file_path ) == true )
			{
				theNpcItm.push_back( pNpcItem );
				
				if( pNpcItem->m_mod_index != -1 )
				{
					sprintf( file_path, "%d.msh", pNpcItem->m_mod_index );							
					Model * pNpcMod = SAFE_NEW( Model );					
					
					if( pNpcMod->ReadVF( &g_VFNpcMsh, file_path, true ) == true ) 
					{
						theNpcMod.push_back(pNpcMod);
						
						pNpcMod->m_Index = pNpcItem->m_mod_index;			
						pNpcItem->m_mod_index = msh_cnt + c_Chr_Mod_Count;
						++msh_cnt;
					}	
					else
					{
						SAFE_DELETE(pNpcMod);
						return false;
					}
					
					sprintf( file_path, "%d.dds", pNpcItem->m_tex_index );						
					Texture * pNpcTex = SAFE_NEW( Texture );					
					theNpcTex.push_back(pNpcTex);
					
					pNpcTex->m_Index = pNpcItem->m_tex_index;			
					pNpcTex->m_byAttr = DATA_DELETE0;			
					pNpcItem->m_tex_index = tex_cnt + c_Chr_Tex_Count;
					++tex_cnt;					
				}
				
				pNpcItem->CreateVrxBuf(SORT_NPC, true);
				pNpcItem->m_Index = pNpcFileInfo->pItmIndex[idx];				
				pNpcFileInfo->pItmIndex[idx] = itm_cnt + c_Chr_Itm_Count;					
				++itm_cnt;
				
			}
			else
			{
				SAFE_DELETE(pNpcItem);
				return false;
			}			
		}	 
		
		for( idx = 0; idx < c_Npc_AniIndex_Count; ++idx )
		{
			if( pNpcFileInfo->pAniInfo[idx].ani_number != -1 )
			{
				sprintf( file_path, "%d.ani", pNpcFileInfo->pAniInfo[idx].ani_number );	
				
				Bone * pNpcAni = SAFE_NEW( Bone );				
				
				if( pNpcAni->ReadVF( &g_VFNpcAni, file_path ) == true )
				{
					theNpcAni.push_back( pNpcAni );
					
					pNpcAni->m_Index = pNpcFileInfo->pAniInfo[idx].ani_number;				
					pNpcFileInfo->pAniInfo[idx].ani_number = ani_cnt + c_Chr_Ani_Count;
					++ani_cnt;
				}
				else
				{
					SAFE_DELETE(pNpcAni);
					return false;
				}					
			}
		}
		
	}
	else
	{
		SAFE_DELETE(pNpcFileInfo);
		return FALSE;	
	}
	
#else
	
	sprintf( file_path, "%d.chr", the_sNpc_SetData.file_name[number] );	  
	
	if( m_pNpcFileInfo[number].FileReadVF( &g_VFNpcRoot, file_path, SORT_NPC ) == true )
	{
		m_pNpcFileInfo[number].m_Index = the_sNpc_SetData.file_name[number];				
		
		SetNpcFileCode( number, m_pNpcFileInfo[number].m_Index );	
		
		for( idx = 0; idx < m_pNpcFileInfo[number].nItmCount; ++idx )
		{
			sprintf( file_path, "%d.itm", m_pNpcFileInfo[number].pItmIndex[idx] );
			
			if( theNpcItm[itm_cnt].File__OpenVF( &g_VFNpcItm, file_path ) == true )
			{					
				if( theNpcItm[itm_cnt].m_mod_index != -1 )
				{
					sprintf( file_path, "%d.msh", theNpcItm[itm_cnt].m_mod_index );	
					if( theNpcMod[msh_cnt].ReadVF( &g_VFNpcMsh, file_path, true ) == true ) 
					{
						theNpcMod[msh_cnt].m_Index = theNpcItm[itm_cnt].m_mod_index;			
						theNpcItm[itm_cnt].m_mod_index = msh_cnt + c_Chr_Mod_Count;
						++msh_cnt;
					}	
					else
					{
						return FALSE;
					}
					
					sprintf( file_path, "%d.dds", theNpcItm[itm_cnt].m_tex_index );						
					{
						theNpcTex[tex_cnt].m_Index = theNpcItm[itm_cnt].m_tex_index;			
						theNpcTex[tex_cnt].m_byAttr = DATA_DELETE0;					
						theNpcItm[itm_cnt].m_tex_index = tex_cnt + c_Chr_Tex_Count;
						++tex_cnt;
					}					  
				}
				
				theNpcItm[itm_cnt].CreateVrxBuf(SORT_NPC, true);
				theNpcItm[itm_cnt].m_Index = m_pNpcFileInfo[number].pItmIndex[idx];				
				m_pNpcFileInfo[number].pItmIndex[idx] = itm_cnt + c_Chr_Itm_Count;					
				++itm_cnt;
			}
			else
			{
				return FALSE;
			}
		}
		
		for( idx = 0; idx < c_Npc_AniIndex_Count; ++idx )
		{
			if( m_pNpcFileInfo[number].pAniInfo[idx].ani_number != -1 )
			{
				sprintf( file_path, "%d.ani", m_pNpcFileInfo[number].pAniInfo[idx].ani_number );	
				if( theNpcAni[ani_cnt].ReadVF( &g_VFNpcAni, file_path ) == true )
				{
					theNpcAni[ani_cnt].m_Index = m_pNpcFileInfo[number].pAniInfo[idx].ani_number;				
					m_pNpcFileInfo[number].pAniInfo[idx].ani_number = ani_cnt + c_Chr_Ani_Count;
					++ani_cnt;
				}
				else
				{
					return FALSE;
				}				  
			}
		}			
	}
	else
	{
		return FALSE;
	}
	
#endif
	
	number++;
	the_sNpc_SetData.total_npc_num = number;
	
	the_sNpc_SetData.total_itm_num = itm_cnt;
	the_sNpc_SetData.total_msh_num = msh_cnt;
	the_sNpc_SetData.total_tex_num = tex_cnt;
	the_sNpc_SetData.total_ani_num = ani_cnt;
	
	SetNpcDataArray();
	
	return TRUE;
}

bool Player_Manager::IsFindFileData( DWORD index )
{
	///-- NPC �ڵ�� Ʋ���� �����ڵ尡 ���� ���ҽ��� ó����ƾ�� �ʿ��ҵ�..
	if( the_sNpc_SetData.IsExistFileName( GetNPCRealFileCode( index ) ) )
	{
		return true;
	}
	
	return false;
}

