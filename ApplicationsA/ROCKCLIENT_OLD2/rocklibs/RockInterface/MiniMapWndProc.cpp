#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

//#include "pc.h"

#include "UiUtil.h"
#include "MiniMapWndProc.h"



CMiniMapWnd	MiniMapWnd;
CMiniMapButton g_Minimap_menu;


extern bool	gVisibleMapWnd;


///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CMiniMapButton::Composition()
{	
	CompositionNew();	
}

void CMiniMapButton::CompositionOld()
{
	map_menuwnd.w	= 100;
	map_menuwnd.h	= 20;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	map_menuwnd.x = nRui->Desktop.Width - map_menuwnd.w;
	map_menuwnd.y = 5;
	
#else
	
	if( nRui->GetResolution() == n_800X600 )
	{
		map_menuwnd.x	= 800 - map_menuwnd.w;
		map_menuwnd.y	= 5;
	}
	else if( nRui->GetResolution() == n_1024X768 )
	{
		map_menuwnd.x	= 1024 - map_menuwnd.w;
		map_menuwnd.y	= 5;
	}	
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MINIMAP_MENU, WID_None, map_menuwnd.x, map_menuwnd.y, map_menuwnd.w, map_menuwnd.h, false );
	fw->SetClientImage( TID_None );	
	fw->SetWndProc( this );
	fw->SetTitle( false );	
	fw->SetMove( false );
	fw->SetVisible( false );
	fw->SetZorder( n_zoTop );
	fw->SetFocusCheck( true );//포커스 체크 없음....
	fw->Initialize();
	
	
	map_btn[0]= (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_MINIMAP_MENU	, 68, 3, 14, 14, false ); 
	map_btn[0]->SetBtnImage( TID_MAIN_MAPS_BT, TID_MAIN_MAPS_BT_OVR, TID_MAIN_MAPS_BT_CLK, TID_None );
	map_btn[0]->SetZorder( n_zoTop );
	map_btn[0]->SetVisible( false );

	map_btn[1]= (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_MINIMAP_MENU	, 68, 3, 14, 14, false ); 
	map_btn[1]->SetBtnImage( TID_MAIN_MAPL_BT, TID_MAIN_MAPL_BT_OVR, TID_MAIN_MAPL_BT_CLK, TID_None );
	map_btn[1]->SetZorder( n_zoTop );
	map_btn[1]->SetVisible( true );

	map_btn[2]= (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_MINIMAP_MENU	, 82, 3, 14, 14, false ); 
	map_btn[2]->SetBtnImage( TID_MAIN_MAPF_BT, TID_MAIN_MAPF_BT_OVR, TID_MAIN_MAPF_BT_CLK, TID_None );
	map_btn[2]->SetZorder( n_zoTop );
	map_btn[2]->SetVisible( true );
}

void CMiniMapButton::CompositionNew()
{
	map_menuwnd.w	= 100;
	map_menuwnd.h	= 20;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	map_menuwnd.x	= nRui->Desktop.Width - map_menuwnd.w;
	map_menuwnd.y	= 7;
	
#else
	
	if( nRui->GetResolution() == n_800X600 )
	{
		map_menuwnd.x	= 800 - map_menuwnd.w;
		map_menuwnd.y	= 7;
	}
	else if( nRui->GetResolution() == n_1024X768 )
	{
		map_menuwnd.x	= 1024 - map_menuwnd.w;
		map_menuwnd.y	= 7;
	}	
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MINIMAP_MENU, WID_None, map_menuwnd.x, map_menuwnd.y, map_menuwnd.w, map_menuwnd.h, false );
	fw->SetClientImage( TID_None );	
	fw->SetWndProc( this );
	fw->SetTitle( false );	
	fw->SetMove( false );
	fw->SetVisible( false );
	fw->SetZorder( n_zoTop );
	fw->SetFocusCheck( true );//포커스 체크 없음....
	fw->Initialize();
	
	
	map_btn[0]= (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_MINIMAP_MENU	, 68, 3, 14, 14, false ); 
	map_btn[0]->SetBtnImage( TID_MAIN_MAPS_BT, TID_MAIN_MAPS_BT_OVR, TID_MAIN_MAPS_BT_CLK, TID_None );
	map_btn[0]->SetZorder( n_zoTop );
	map_btn[0]->SetVisible( false );

	map_btn[1]= (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_MINIMAP_MENU	, 68, 3, 14, 14, false ); 
	map_btn[1]->SetBtnImage( TID_MAIN_MAPL_BT, TID_MAIN_MAPL_BT_OVR, TID_MAIN_MAPL_BT_CLK, TID_None );
	map_btn[1]->SetZorder( n_zoTop );
	map_btn[1]->SetVisible( true );

	map_btn[2]= (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_MINIMAP_MENU	, 82, 3, 14, 14, false ); 
	map_btn[2]->SetBtnImage( TID_MAIN_MAPF_BT, TID_MAIN_MAPF_BT_OVR, TID_MAIN_MAPF_BT_CLK, TID_None );
	map_btn[2]->SetZorder( n_zoTop );
	map_btn[2]->SetVisible( true );
}

///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CMiniMapButton::Proc( SEventMessage* EMsg )
{
	if( EMsg->FocusWnd == map_btn[0]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			MiniMapWnd.SetMapInfoText( 128 );
		}		
	}
	if( EMsg->FocusWnd == map_btn[1]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			MiniMapWnd.SetMapInfoText( 256 );			
		}		
	}
	if( EMsg->FocusWnd == map_btn[2]->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			bool IsWnd = !(MiniMapWnd.IsRenderLEntMap());
			MiniMapWnd.SetRenderLEntMap(IsWnd);				
		}		
	}
}
///---------------------------------------------------------------------------
///-- LogOff
///---------------------------------------------------------------------------
void CMiniMapButton::LogOff()
{


}
	
CMiniMapWnd::CMiniMapWnd()
{
	IsRenLEntMap = false;
	LocalEntMap = NULL;
	iMapType =0 ; 
}

CMiniMapWnd::~CMiniMapWnd()
{
//	AllClearNPCInfo();
}

BOOL CMiniMapWnd::IsMapChange(bool bMiniMize)
{

	if( bMiniMize == TRUE) 
	{
		if( iMapType >= 1 || IsRenderLEntMap() )
		{
			if( IsRenderLEntMap()) 
			{
				SetRenderLEntMap(false);
			}

			SetMapInfoText( 128 );
			iMapType  =0 ; 
			return TRUE;
		}
		else 
		{
			return FALSE;
		}
		
	}
	else 
	{
		if( ++iMapType  ) 
		{
			if( iMapType > 2)
			{
				iMapType  = 0;
			}
		}
		if( iMapType == 0 )
		{
			if( MiniMapWnd.IsRenderLEntMap() )					
			{
				MiniMapWnd.SetRenderLEntMap(false);
			}
			
			MiniMapWnd.SetMapInfoText( 128 );
		}
		if( iMapType == 1 )
		{				
			if( MiniMapWnd.IsRenderLEntMap() )					
			{
				MiniMapWnd.SetRenderLEntMap(false);
			}
			MiniMapWnd.SetMapInfoText( 256 );
		}
		if( iMapType == 2 )
		{		
			if( !MiniMapWnd.IsRenderLEntMap() )					
			{
				MiniMapWnd.SetRenderLEntMap( true );
			}				
		}
	}
	
	return TRUE;

}

//-----------------------------------------------------------------------------
void CMiniMapWnd::Init()
{
	PcInfo = nRui->GetPcInfo();
}

//-----------------------------------------------------------------------------
void CMiniMapWnd::Composition()
{
	CompositionNew();	
}

void CMiniMapWnd::CompositionOld()
{

}

void CMiniMapWnd::CompositionNew()
{
	CFrameWnd*		fw;
	
	SRect mapwnd;
	mapwnd.w	= 256;
	mapwnd.h	= 400;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	mapwnd.x	= nRui->Desktop.Width - mapwnd.w;
	mapwnd.y	= 7;
	
#else
	
	if( nRui->GetResolution() == n_800X600 )
	{
		mapwnd.x	= 800 - mapwnd.w;
		mapwnd.y	= 7;
	}
	else if( nRui->GetResolution() == n_1024X768 )
	{
		mapwnd.x	= 1024 - mapwnd.w;
		mapwnd.y	= 7;
	}	
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MiniMapWnd, WID_None, mapwnd.x, mapwnd.y, mapwnd.w, mapwnd.h, false );
	fw->SetClientImage( TID_CLIENT );
	fw->SetCaption( _RT("MiniMap Window") );
	fw->SetWndProc( this );
	fw->SetTitle( false );	
	fw->SetMove( false );
	fw->SetVisible( false );
	fw->SetZorder( n_zoBottom );
	fw->SetFocusCheck( true );//포커스 체크 없음....
	fw->Initialize();

	Map = (CMiniMap*)nRui->CreateWnd( n_wtMiniMap, WID_MiniMapViewer, WID_MiniMapWnd, 0, 0, 0, 0, false );

	InfoText128[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MiniMap_InfoText128_Name, WID_MiniMapWnd, 139, 142, 107, 18, false );
	InfoText128[0]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	InfoText128[0]->SetClientImage( TID_None );
	InfoText128[0]->SetAlignText( n_atCenter, n_atCenter );
	InfoText128[0]->SetText( _RT("") );
	InfoText128[0]->SetTextColor( 0, 0, 0 );
	InfoText128[0]->SetVisible( true  );
	
	InfoText128[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MiniMap_InfoText128_Coord, WID_MiniMapWnd, 154, 142, 73, 18, false );
	InfoText128[1]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	InfoText128[1]->SetClientImage( TID_None );
	InfoText128[1]->SetAlignText( n_atCenter, n_atCenter );
	InfoText128[1]->SetText( _RT("") );
	InfoText128[1]->SetTextColor( 0, 0, 0 );
	InfoText128[1]->SetVisible( false );

	InfoText128[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MiniMapWnd, 139 ,  2  , 107, 18, false );
	InfoText128[2]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	InfoText128[2]->SetClientImage( TID_None );
	InfoText128[2]->SetAlignText( n_atLeft, n_atCenter );
	InfoText128[2]->SetText( _RT("") );
	InfoText128[2]->SetTextColor( 0, 0, 0 );
	InfoText128[2]->SetVisible( true );

	InfoText256[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MiniMap_InfoText256_Name, WID_MiniMapWnd, 25, 237, 114, 22, false );
	InfoText256[0]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	InfoText256[0]->SetClientImage( TID_None );
	InfoText256[0]->SetAlignText( n_atCenter, n_atCenter );
	InfoText256[0]->SetText( _RT("") );
	InfoText256[0]->SetTextColor( 0, 0, 0 );
	InfoText256[0]->SetVisible( false );

	InfoText256[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_MiniMap_InfoText256_Coord, WID_MiniMapWnd, 152, 237, 75, 22, false );
	InfoText256[1]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	InfoText256[1]->SetClientImage( TID_None );
	InfoText256[1]->SetAlignText( n_atCenter, n_atCenter );
	InfoText256[1]->SetText( _RT("") );
	InfoText256[1]->SetTextColor( 0, 0, 0 );
	InfoText256[1]->SetVisible( false );

	InfoText256[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MiniMapWnd, 0 , 2 , 256, 18 , false );
	InfoText256[2]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	InfoText256[2]->SetClientImage( TID_None );
	InfoText256[2]->SetAlignText( n_atCenter, n_atCenter );
	InfoText256[2]->SetText( _RT("") );
	InfoText256[2]->SetTextColor( 0, 0, 0 );
	InfoText256[2]->SetVisible( false );
	
	Map->SetPcInfo( nRui->thePcInfo );
	Map->SetMapWnd( 128, TID_MAIN_MAP_S );

	LocalEntMap = (CLocalEntireMap *)nRui->CreateWnd( n_wtLocalEntireMap, WID_LocalEntireMap, WID_None, 0, 0, 0, 0, false );
	LocalEntMap->SetVisible( false );
	LocalEntMap->SetRefMiniMap(Map);
	LocalEntMap->SetMapAlpha(120);
	LocalEntMap->SetPcInfo( nRui->thePcInfo );
	LocalEntMap->SetFocusCheck(false);
	LocalEntMap->SetZorder( n_zoBottom );

	g_Minimap_menu.Composition();
}

///---------------------------------------------------------------------------
///-- SetVisible
///---------------------------------------------------------------------------
void CMiniMapWnd::SetVisible( bool Visible )
{
	IsVisible = Visible;
	g_Minimap_menu.map_btn[0]->SetVisible( Visible );
	g_Minimap_menu.map_btn[1]->SetVisible( Visible );
	g_Minimap_menu.map_btn[2]->SetVisible( Visible );
}

//-----------------------------------------------------------------------------
void CMiniMapWnd::Update()
{
}

///---------------------------------------------------------------------------
///-- Proc
///---------------------------------------------------------------------------
void CMiniMapWnd::Proc( SEventMessage* EMsg )
{

	/*
	switch( EMsg->FocusWnd )
	{
	case WID_Skill_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_MiniMapWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			gVisibleMapWnd = false;
		}
		break;
	}
	//*/
}

//-----------------------------------------------------------------------------
void CMiniMapWnd::SetMiniMap( int size, TEXID image )
{
	if( image != TID_None )
	{
		///-- Mini map 으로 초기화		
		///SetRenderLEntMap( false );
		g_Minimap_menu.fw->SetVisible( true );

		if( Map->GetMapWndSize()  == 128 )
		{	g_Minimap_menu.map_btn[0]->SetVisible( false );
			g_Minimap_menu.map_btn[1]->SetVisible( true );
			g_Minimap_menu.map_btn[2]->SetVisible( true );
		}
		else if( Map->GetMapWndSize()  == 256 )// 256
		{			
			g_Minimap_menu.map_btn[0]->SetVisible( true );
			g_Minimap_menu.map_btn[1]->SetVisible( false );
			g_Minimap_menu.map_btn[2]->SetVisible( true );	
		}
	}
	
	Map->SetMiniMap( size, image );
	//SetMapInfoText( size );
}

//-----------------------------------------------------------------------------
void CMiniMapWnd::SetPcMapCoord( int x, int y )
{
	static char map_coord[40] = "";
	Rsprintf( RWCHAR(map_coord), _RT("X=%d, Z=%d"), x, y );
	
	//유저캐릭 좌표 갱신
	InfoText128[1]->SetText( map_coord );
	InfoText256[1]->SetText( map_coord );
}
//-----------------------------------------------------------------------------
void CMiniMapWnd::SetMapIndex( int map_id )
{
	TCHAR map_name[256] = {0,};
	::GetAreaStrName(map_name, map_id);

	
	InfoText128[0]->SetText( map_name );		
	InfoText128[0]->SetLimitLength(17);	
	InfoText256[0]->SetText( map_name );	

	char StrTmp[256]= "";
	Rsprintf( RWCHAR(StrTmp ), _RT("%s %d") , G_STRING_CHAR(IDS_WND_CHANAL) , nRui->m_ChannelIndex+1 );
	InfoText128[2]->SetText(StrTmp);
	InfoText256[2]->SetText(StrTmp);
	LocalEntMap->ResetActiveMap();
}
//-----------------------------------------------------------------------------
void CMiniMapWnd::SetMapInfoText( int size )
{
	if( IsRenLEntMap )
		return;

	if( size == 128 )
	{		
		InfoText128[0]->SetVisible( true);
		InfoText128[1]->SetVisible( false );
		InfoText128[2]->SetVisible( true );
		InfoText256[0]->SetVisible( false );
		InfoText256[1]->SetVisible( false );
		InfoText256[2]->SetVisible( false );
		g_Minimap_menu.map_btn[0]->SetVisible( false );
		g_Minimap_menu.map_btn[1]->SetVisible( true );
		g_Minimap_menu.map_btn[2]->SetVisible( true );
		MiniMapWnd.Map->SetMapWnd( 128, TID_MAIN_MAP_S );
	}
	else if( size == 256 )// 256
	{
		InfoText128[0]->SetVisible( false );
		InfoText128[1]->SetVisible( false );
		InfoText128[2]->SetVisible( false );
		InfoText256[0]->SetVisible( true );
		InfoText256[1]->SetVisible( true );
		InfoText256[2]->SetVisible( true );

		g_Minimap_menu.map_btn[0]->SetVisible( true );
		g_Minimap_menu.map_btn[1]->SetVisible( false );
		g_Minimap_menu.map_btn[2]->SetVisible( true );
		MiniMapWnd.Map->SetMapWnd( 256, TID_MAIN_MAP_L );
	}
}

void  CMiniMapWnd::AllClearNPCInfo()		//맵연관 NPC정보를 전부삭제
{
	if(Map)
	{
	   	list< SMinimapNPCInfo * >::iterator i = Map->m_NPCInfoList.begin();
		
		for( ; i != Map->m_NPCInfoList.end() ; ++i )
		{
			SMinimapNPCInfo * pNPCinfo = (*i);
			SAFE_DELETE(pNPCinfo);
			(*i) = NULL;
		}

		Map->m_NPCInfoList.clear();
	}
}

//맵연관 NPC정보를 추가한다.
void  CMiniMapWnd::InsertNPCInfo(SMinimapNPCInfo *pNPCInfo)			
{
	if(Map)
	{
		Map->m_NPCInfoList.insert(Map->m_NPCInfoList.end(), pNPCInfo);
	}
	else
	{
		SAFE_DELETE(pNPCInfo);		
	}
}

//NPC와 연관된 퀘스트정보를 설정한다.
void  CMiniMapWnd::SetNPCQuestInfo(SMinimapNPCInfo *pNPCInfo)
{
	static int nGroupId[RP_MAX_QUEST_FLAG];

	for( int i = 0; i < RP_MAX_QUEST_FLAG; ++i )
	{
		if(nRui->thePcInven.QuestFlag[i])
		{
			for( int j = 0; j < c_Max_PreQuestInfo_Count ; ++j )
			{
				if( nRui->thePcInven.QuestFlag[i]->theQuestCode == 
					g_Pc_Manager.thePreQuestInfoTable[j].theQuestCode )
				{
					nGroupId[i] = g_Pc_Manager.thePreQuestInfoTable[j].theQuestGCode;
					break;
				}
			}
		}
		else
		{
			nGroupId[i] = 0;
		}
	}	
	
	for(int i = 0; i < c_Max_PreQuestInfo_Count ; ++i )
	{
		int quest_code  = g_Pc_Manager.thePreQuestInfoTable[i].theQuestCode;
		int npc_code    = g_Pc_Manager.thePreQuestInfoTable[i].theNPCCode;
		int npc_code2   = g_Pc_Manager.thePreQuestInfoTable[i].theAfterNPCCode;
		int need_level  = g_Pc_Manager.thePreQuestInfoTable[i].theNeedLevel;
		int FindNPCType = -1;	
		int ISDisplay   = g_Pc_Manager.thePreQuestInfoTable[i].theDisplayNPCEffect;
		int QuestGCode  = g_Pc_Manager.thePreQuestInfoTable[i].theQuestGCode;

		if( quest_code == 0 )
		{
			break;
		}

		if( npc_code == pNPCInfo->nNpcIndex )		// 일단 해당 NPC꺼냐??!!...
		{
			FindNPCType = 0;
		}

		if( npc_code2 == pNPCInfo->nNpcIndex )		// 보고 NPC 이냐?
		{
			if(0 == FindNPCType)					// 시작 NPC와 보고 NPC가 같으냐?
			{
				FindNPCType = 2;
			}
			else
			{
				FindNPCType = 1;
			}				
		}	

		if( FindNPCType == -1 )
		{
			continue;
		}

		if( need_level > nRui->thePcParam.Level )		// 그리고 내가 할 수 있는거냐??!!..
		{
			continue;
		}

		bool IsInsertQuest = true;

		for( int j = 0; j < RP_MAX_QUEST_FLAG; ++j )
		{
			if( nGroupId[j] == 0 )
			{
				// 보고 NPC 꺼면 아직 시자고 안했으니까 제외
				if( FindNPCType == 1 || ISDisplay == FALSE )
				{
					IsInsertQuest = false;
					break;
				}
				
				if( pNPCInfo->nQuestMarkType != REPORT_QUEST_MARK )
				{
					pNPCInfo->nQuestMarkType = START_QUEST_MARK;
				}
				break;
			}
			// 플래그 값이 있는 것일때
			else if( nGroupId[j] == QuestGCode )
			{
				//같은 그룹이지만 퀘스트 코드가 틀리면...
				if(nRui->thePcInven.QuestFlag[j]->theQuestCode != quest_code )
				{
					IsInsertQuest = false;
					break;
				}				
				
				// 종결 퀘스트
				if( nRui->thePcInven.QuestFlag[j]->theFlag == 60 )
				{
					IsInsertQuest = false;
					break;
				}
				// 진행중일때
				else
				if( nRui->thePcInven.QuestFlag[j]->theFlag != 60 && FindNPCType != 0 )
				{
					pNPCInfo->nQuestMarkType = REPORT_QUEST_MARK;
					break;
				}
				// 그외에는 제외
				else
				{
					IsInsertQuest = false;
					break;
				}
			}		
		}
		if( IsInsertQuest )
		{
			// 플레이어가 수행 해야 할 퀘스트를 알려주기 위해서...^^;;
			pNPCInfo->PreQuestCodeList.push_back( quest_code );
		}
	}
}

SMinimapNPCInfo * CMiniMapWnd::DeleteQuestCode( WORD quest_code,int index )
{
	if(Map)
	{
	   	list< SMinimapNPCInfo * >::iterator i = Map->m_NPCInfoList.begin();
		
		for( ; i != Map->m_NPCInfoList.end() ; ++i )
		{
			SMinimapNPCInfo * pNPCinfo = (*i);
			
			if(pNPCinfo->nNpcIndex == index)
			{
				std::vector<int>::iterator ir;		
				ir = pNPCinfo->PreQuestCodeList.begin();
				for( ; ir != pNPCinfo->PreQuestCodeList.end() ; ++ir )
				{
					if( (*ir) == quest_code )
					{
						pNPCinfo->PreQuestCodeList.erase( ir );
						return pNPCinfo;						
					}
				}
				
				return NULL;
			}		
		}		
	}

	return NULL;
}

SMinimapNPCInfo * CMiniMapWnd::FindQuestNPC( int quest_code, int index )
{
	if(Map)
	{
		list< SMinimapNPCInfo * >::iterator i = Map->m_NPCInfoList.begin();
		
		for( ; i != Map->m_NPCInfoList.end() ; ++i )
		{
			SMinimapNPCInfo * pNPCinfo = (*i);
			
			if(pNPCinfo->nNpcIndex == index)
			{
				std::vector<int>::iterator ir;		
				ir = pNPCinfo->PreQuestCodeList.begin();
				for( ; ir != pNPCinfo->PreQuestCodeList.end() ; ++ir )
				{
					if( (*ir) == quest_code )
					{
						return pNPCinfo;						
					}
				}				
				
				pNPCinfo->PreQuestCodeList.push_back(quest_code);
				return pNPCinfo;
			}	
		}	
	}
	
	return NULL;
}

//지역 전체맵 렌더링 여부
///---------------------------------------------------------------------------
///-- SetRenderLEntMap
///---------------------------------------------------------------------------
void CMiniMapWnd::SetRenderLEntMap(bool bFlag)
{
	if( LocalEntMap->m_MapState == LEntState_Nomal )
	{
		IsRenLEntMap = bFlag;	
		LocalEntMap->SetActiveMap(IsRenLEntMap);
		
		///-- 전체 화면 맵
		if( IsRenLEntMap )
		{
			g_Minimap_menu.map_btn[0]->SetVisible( false );
			g_Minimap_menu.map_btn[1]->SetVisible( false );
			///g_Minimap_menu.fw->SetWndRepositionR( 670, 127 );
		}
		///-- 미니맵
		else
		{
			if( Map->GetMapWndSize()  == 128 )
			{				
				g_Minimap_menu.map_btn[0]->SetVisible( false );
				g_Minimap_menu.map_btn[1]->SetVisible( true );			
				///g_Minimap_menu.fw->SetWndRepositionR( g_Minimap_menu.map_menuwnd.x, g_Minimap_menu.map_menuwnd.y );
			}
			else if( Map->GetMapWndSize()  == 256 )// 256
			{			
				g_Minimap_menu.fw->SetWndRepositionR( g_Minimap_menu.map_menuwnd.x, g_Minimap_menu.map_menuwnd.y );
				g_Minimap_menu.map_btn[0]->SetVisible( true );
				g_Minimap_menu.map_btn[1]->SetVisible( false );			
			}
		}
	}
}

void CMiniMapWnd::LogOff()
{
	IsRenLEntMap = false;
	LocalEntMap->SetVisible(false);
	
	g_Minimap_menu.map_btn[0]->SetVisible( false );
	g_Minimap_menu.map_btn[1]->SetVisible( false );
	g_Minimap_menu.map_btn[2]->SetVisible( false );
	g_Minimap_menu.fw->SetVisible( true);
}
