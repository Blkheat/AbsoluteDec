// GMCmdDlg.cpp: implementation of the CGMCmdDlg class.
//////////////////////////////////////////////////////////////////////

#include "RockPCH.h"
#include "GMCmdDlg.h"
#include "RockClient.h"

#include "RockInterface\\NonPlayerInfoWnd.h"

#include "quadlist.h"
#include "bravolibs\\network\\Protocol.h"

#include "RockClient.h"
#include "PC.h"
#include "Map.h"
#include	<SOUND\\Sound.H>

extern Sound_Manager		g_Sound;
extern SPacketInfoPerSec	g_SPacketInfoPerSec;
extern 

#include "stringmanager.h"

#define	DLG_CLASS_NAME		"GM Command"


#define	DLG_HEIGHT			150

enum
{
	IDC_EDIT_CMD = 0x00c1, 
	IDC_LIST_LOG = 0x00c2, 
	IDC_SAVE_BTN = 0x00C3,		
};


#define	GM_CMD_ERR_NUM	16

char	g_GmCmdErrStr[GM_CMD_ERR_NUM][255] = 
{
	0, 
/*	G_STRING_CHAR( IDS_GMMSG_NOT_GMCMD ),					// "그런 명령어가 없습니다.",					
	G_STRING_CHAR( IDS_GMMSG_NOT_GMCMD_POWER ),				// "명령 실행 권한이 없습니다.",				
	G_STRING_CHAR( IDS_GMMSG_ERROR_CMD_PARAM ),				// "명령 파라미터에 오류가 있습니다.(범용)",	
	G_STRING_CHAR( IDS_GMMSG_NOT_LOGIN_USER ),				// "대상 캐릭터가 접속중이지 않습니다.",		
	G_STRING_CHAR( IDS_GMMSG_ERROR_MAPINDEX ),				// "맵 번호가 잘못되었습니다.",				
	G_STRING_CHAR( IDS_GMMSG_ERROR_POS ),					// "지정된 좌표가 잘못되었습니다.",		
	G_STRING_CHAR( IDS_GMMSG_ERROR_SKILLCODE ),				// "스킬 코드가 잘못 되었습니다.",		
	G_STRING_CHAR( IDS_GMMSG_ALREADY_GETSKILL ),			// "이미 배운 스킬입니다.",				
	G_STRING_CHAR( IDS_GMMSG_NOT_GETSKILL ),				// "배우지 않은 스킬입니다.",			
	G_STRING_CHAR( IDS_GMMSG_ERROR_ITEMCODE ),				// "아이템 코드가 잘못 되었습니다.",	
	G_STRING_CHAR( IDS_GMMSG_INVENTORY_ENOUGH ),			// "인벤토리에 공간이 부족합니다.",		
	G_STRING_CHAR( IDS_GMMSG_ERROR_CREATEITEM ),			// "아이템 생성 실패(범용)",			
	G_STRING_CHAR( IDS_GMMSG_ERROR_SAMELEVEL ),				// "바꿀레벨이 현재 레벨과 같습니다.",			
	G_STRING_CHAR( IDS_GMMSG_ERROR_OVERMAXEXP ),			// "경험치값이 최대값 범위를 벗어납니다.",		
	G_STRING_CHAR( IDS_GMMSG_NOT_GETQUEST ),				// "퀘스트 정보가 없습니다.",					
	G_STRING_CHAR( IDS_GMMSG_CHANGE_CLASS_LEVEL10 ),		// "레벨 10에서만 직업을 변경할 수 있습니다."	
*/
};

///////////////////////////////  command  ///////////////////////////////////////
/*
enum GMCmd
{
	n_Summons				= 1001,	// 소환
	n_Appearance			= 1002,	// 출두
	n_MovePC				= 1003,	// 이동
	n_MoveGM				= 1004,	// 이동 ( GM )
	n_ReturnPC				= 1005, // 귀환
	n_ExitPC				= 1006, // 나가
	n_ExitMapPC				= 1007, // 맵나가
	n_StopMove				= 1008, // 이동금지
	n_StopMoveMap			= 1009, // 맵이동 금지
	n_StopMoveCancel		= 1010, // 이동금지 해제
	n_StopMoveMapCancel		= 1011, // 맵이동 금지 해제

	n_Announcement			= 2001, // 공지
	n_AnnouncementMap		= 2002, // 맵공지
	n_ExitAllPC				= 2003, // 다나가
	n_ServerDown			= 2004, // 서버다운.
	n_MillenaAnnouncement	= 2005, // 밀레나 공지.......
	n_RainAnnouncement		= 2006, // 레인 공지.......
	n_ExitGhostPC			= 2007, // 유령 유저 삭제.......
	n_ReloadScript			= 2008,	// 스크립트 로딩 
	
	n_ConnectCount			= 3001,	// 유저 카운트
	n_ConnectMapCount		= 3002,	// 맵 유저 카운트
	n_MillenaConnectCount	= 3003, // 밀레나 유저 카운트.......
	n_RainConnectCount		= 3004, // 레인 유저 카운트.......

	n_CreateSkill			= 4001,	// 스킬 생성
	n_DeleteSkill			= 4002,	// 스킬 삭제
	n_CreateItem			= 4003,	// 아이템 생성
	n_InitPoint				= 4004,	// 포인트 초기화
	n_LevelUp				= 4005,	// 레벨업
	n_LevelChange			= 4006,	// 레벨 변경
	n_ChangeExp				= 4007,	// 경험치 변경
	n_FreeGage				= 4008, // 게이지 해제.......
	n_GetQuestFlag			= 4009, // 퀘스트 플래그 검색
	n_SetQuestFlag			= 4010, // 퀘스트 플래그 갱신
	n_ChangeClass			= 4011,	// 전직
	n_ChangeSubClass		= 4012, // 서브직업 전직 
	n_CreateItemwithStone	= 4013, // 아이템에 엘다스톤 추가하기
	n_MakeNPC				= 4014,	// NPC 생성 
	n_ChangeEndurance		= 4015,	// 착용한 아이템 내구도 변경 
	n_InsertSavedLog		= 4016,	// 로그 저장 
	n_RemoveSavedLog		= 4017,	// 로그 저장 
	n_DelQuest				= 4018,	// 퀘스트 삭제 
	n_FightTime				= 4019,	// 대전 시간 관리 
	n_DeleteItem			= 4020,	// 아이템 삭제 (퀘템관리)
	n_ReloadFrontier		= 4021, // 프론티어 
	n_SetEventOP			= 4022, // 이벤트 진행자 셋팅  
	n_SetRoomName			= 4023, // 이벤트 대전방 이름 변경

	n_SearchPC				= 5001,	// 
	n_WatchPC				= 5002, // 감시
	n_CharacterInfo			= 5003, // 캐릭터 정보.......
	n_AccountInfo			= 5004, // 계정 정보.......

	n_RequestChangeWheather = 5005, // 날씨 변경
	n_RequestDyePlayer		= 5006, // Player 자살 
	n_RequestAddEld			= 5007, // eld Added
	n_RequestNPCInfo		= 5008, // 특정 NPC의 정보를 알아본다. 
	n_EventExp				= 5009,	// 이벤트용 경험치 획득률 변경 /EventExp (증가율)
	n_EventItemDrop			= 5010,	// 이벤트용 아이템드랍률 조절  /EventItemDrop (증가율)

	n_Reanimate				= 6001,	// 소생
*/

SCmdInfo *g_CmdInfo = NULL;


///---------------------------------------------------------------------------
///-- _Create_GM_data
///---------------------------------------------------------------------------
bool CGMCmdDlg::_Create_GM_data()
{	
/*
	signed long i;
 	signed long ll_index = 0;
	signed long ll_len;
	char lac_filename[100]={0,};
	int ll_count;
	int ll_array_idx = 0;
	
	FILE* fp = NULL;	

	sprintf( g_GmCmdErrStr[0], G_STRING_CHAR( IDS_GMMSG_NOT_GMCMD ) );
	sprintf( g_GmCmdErrStr[1], G_STRING_CHAR( IDS_GMMSG_NOT_GMCMD_POWER ) );
	sprintf( g_GmCmdErrStr[2], G_STRING_CHAR( IDS_GMMSG_ERROR_CMD_PARAM ) );
	sprintf( g_GmCmdErrStr[3], G_STRING_CHAR( IDS_GMMSG_NOT_LOGIN_USER ) );
	sprintf( g_GmCmdErrStr[4], G_STRING_CHAR( IDS_GMMSG_ERROR_MAPINDEX ) );
	sprintf( g_GmCmdErrStr[5], G_STRING_CHAR( IDS_GMMSG_ERROR_POS ) );
	sprintf( g_GmCmdErrStr[6], G_STRING_CHAR( IDS_GMMSG_ERROR_SKILLCODE ) );
	sprintf( g_GmCmdErrStr[7], G_STRING_CHAR( IDS_GMMSG_ALREADY_GETSKILL ) );
	sprintf( g_GmCmdErrStr[8], G_STRING_CHAR( IDS_GMMSG_NOT_GETSKILL ) );
	sprintf( g_GmCmdErrStr[9], G_STRING_CHAR( IDS_GMMSG_ERROR_ITEMCODE ) );
	sprintf( g_GmCmdErrStr[10], G_STRING_CHAR( IDS_GMMSG_INVENTORY_ENOUGH ) );
	sprintf( g_GmCmdErrStr[11], G_STRING_CHAR( IDS_GMMSG_ERROR_CREATEITEM ) );
	sprintf( g_GmCmdErrStr[12], G_STRING_CHAR( IDS_GMMSG_ERROR_SAMELEVEL ) );
	sprintf( g_GmCmdErrStr[13], G_STRING_CHAR( IDS_GMMSG_ERROR_OVERMAXEXP ) );
	sprintf( g_GmCmdErrStr[14], G_STRING_CHAR( IDS_GMMSG_NOT_GETQUEST ) );
	sprintf( g_GmCmdErrStr[15], G_STRING_CHAR( IDS_GMMSG_CHANGE_CLASS_LEVEL10 ) );

	///---------------------------------------------------------------------------
	///-- Open String Data
	switch( g_RockClient.GetLanguage() ) 
	{
	default:
	case eLang_Korea: 
		sprintf( lac_filename, "local_test\\gm_korea.dat" );		
		break;
	case eLang_Japan:
		sprintf( lac_filename, "local_test\\gm_japan.dat" );		
		break;
	case eLang_Indonesia:
		sprintf( lac_filename, "local_test\\gm_indonesia.dat" );		
		break;
	case eLang_China:
		sprintf( lac_filename, "local_test\\gm_china.dat" );		
		break;
	case eLang_Taiwan:
		sprintf( lac_filename, "local_test\\gm_taiwan.dat" );		
		break;
	case eLang_English:
		sprintf( lac_filename, "local_test\\gm_English.dat" );		
		break;
	case eLang_Singapore:
		sprintf( lac_filename, "local_test\\gm_English.dat" );		
		break;
	
		
	}
	
	fp = _tfopen( lac_filename, _T("rb") ); ///pFH =  g_VFInterface.OpenFile( lac_filename );
	if( fp == NULL ) 
	{
		char lac_error[300];
		sprintf(lac_error," Cannot Open File : %s ", lac_filename );
		
		MessageBox( NULL, lac_error, G_STRING_CHAR( IDS_WND_ALARM ), MB_OK );
		return false;
	}
	
	///---------------------------------------------------------------------------
	///-- Read Data
	
	///-- Read string count
	fread( ( void * )&ll_count, sizeof( ll_count ), 1, fp);
	g_CmdInfo = SAFE_NEW_ARRAY( SCmdInfo, ll_count );
	
	///-- Read Strings
	for( i = 0 ; i < ll_count; ++i )
	{
		///-- code
		fread(  ( void * )&g_CmdInfo[i].theCmdCode, sizeof( int ), 1, fp );
		
		///-- nparam
		fread(  ( void * )&g_CmdInfo[i].theNParamNum, sizeof( int ), 1, fp );

		///-- sparam
		fread(  ( void * )&g_CmdInfo[i].theSParamNum, sizeof( int ), 1, fp );
		
		///-- Length
		fread(  ( void * )&ll_len, sizeof( ll_index ), 1, fp );
				
		///-- String
		fread( ( void * )&g_CmdInfo[i].theCmdStr, ll_len, 1, fp );
	}
	fclose( fp );
*/	
	return true;

}
///---------------------------------------------------------------------------
///-- _Create_GM_data_VF
///---------------------------------------------------------------------------
bool CGMCmdDlg::_Create_GM_data_VF()
{
	
#ifdef	GM_CLIENT

	VFileHandle* pFH;	
	signed long i;
 	signed long ll_index = 0;
	signed long ll_len;
	char lac_filename[100]={0,};
	int ll_count;
	int ll_array_idx = 0;
	
	Rsprintf( RWCHAR(g_GmCmdErrStr[0]), G_STRING_CHAR( IDS_GMMSG_NOT_GMCMD ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[1]), G_STRING_CHAR( IDS_GMMSG_NOT_GMCMD_POWER ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[2]), G_STRING_CHAR( IDS_GMMSG_ERROR_CMD_PARAM ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[3]), G_STRING_CHAR( IDS_GMMSG_NOT_LOGIN_USER ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[4]), G_STRING_CHAR( IDS_GMMSG_ERROR_MAPINDEX ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[5]), G_STRING_CHAR( IDS_GMMSG_ERROR_POS ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[6]), G_STRING_CHAR( IDS_GMMSG_ERROR_SKILLCODE ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[7]), G_STRING_CHAR( IDS_GMMSG_ALREADY_GETSKILL ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[8]), G_STRING_CHAR( IDS_GMMSG_NOT_GETSKILL ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[9]), G_STRING_CHAR( IDS_GMMSG_ERROR_ITEMCODE ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[10]), G_STRING_CHAR( IDS_GMMSG_INVENTORY_ENOUGH ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[11]), G_STRING_CHAR( IDS_GMMSG_ERROR_CREATEITEM ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[12]), G_STRING_CHAR( IDS_GMMSG_ERROR_SAMELEVEL ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[13]), G_STRING_CHAR( IDS_GMMSG_ERROR_OVERMAXEXP ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[14]), G_STRING_CHAR( IDS_GMMSG_NOT_GETQUEST ) );
	Rsprintf( RWCHAR(g_GmCmdErrStr[15]), G_STRING_CHAR( IDS_GMMSG_CHANGE_CLASS_LEVEL10 ) );
	
	///---------------------------------------------------------------------------
	///-- Open String Data
	switch( g_RockClient.GetLanguage() ) 
	{
	default:
	case eLang_Korea: 
		sprintf( lac_filename, "gm_korea.dat" );		
		break;
	case eLang_Japan:
		sprintf( lac_filename, "gm_japan.dat" );		
		break;
	case eLang_Indonesia:
		sprintf( lac_filename, "gm_indonesia.dat" );		
		break;
	case eLang_English:
		sprintf( lac_filename, "gm_English.dat" );		
		break; 
	case eLang_China:
		sprintf( lac_filename, "gm_china.dat" );		
		break;
	case eLang_Taiwan:
		sprintf( lac_filename, "gm_taiwan.dat" );		
		break;
	case eLang_Singapore:
		sprintf( lac_filename, "gm_english.dat" );		
		break;
	case eLang_HongKong:
		sprintf( lac_filename, "gm_Hongkong.dat" );		
		break;
		

	} 
	
	pFH =  g_VFInterface.OpenFile( lac_filename );
//	pFH =  g_VFEtc.OpenFile( lac_filename );
	if( pFH == NULL ) 
	{
		char lac_error[300];
		sprintf(lac_error," Cannot Open File : %s ", lac_filename );
		MessageBox( NULL, lac_error, "Error", MB_OK );
		return false;
	}
	
	///---------------------------------------------------------------------------
	///-- Read Data
	
	///-- Read string count
	pFH->Read( ( void * )&ll_count, sizeof( ll_count ));
	g_CmdInfo = SAFE_NEW_ARRAY( SCmdInfo, ll_count );
	ZeroMemory(g_CmdInfo , sizeof(SCmdInfo) * ll_count);
		
	///-- Read Strings
	for( i = 0 ; i < ll_count; ++i )
	{
		///-- code
		pFH->Read( ( void * )&g_CmdInfo[i].theCmdCode, sizeof( int ));
		
		///-- nparam
		pFH->Read( ( void * )&g_CmdInfo[i].theNParamNum, sizeof( int ));

		///-- sparam
		pFH->Read( ( void * )&g_CmdInfo[i].theSParamNum, sizeof( int ));
		
		///-- Length
		pFH->Read( ( void * )&ll_len, sizeof( ll_index ));
				
		///-- String
#ifdef UNICODE_ROCK
		WCHAR Str[32]={0,};
		memset( g_CmdInfo[i].theCmdStr , NULL , sizeof(g_CmdInfo[i].theCmdStr) );
		pFH->Read( ( void * )&g_CmdInfo[i].theCmdStr , ll_len);
		DXUtil_ConvertAnsiStringToWide(Str ,g_CmdInfo[i].theCmdStr , -1);
		Rstrcpy(g_CmdInfo[i].theCmdStr ,Str);
#else
		pFH->Read( ( void * )&g_CmdInfo[i].theCmdStr , ll_len);
#endif
	
	}

	g_VFInterface.CloseFile( pFH );

#endif
	
	return true;

}
///---------------------------------------------------------------------------
///-- ViewHelp
///---------------------------------------------------------------------------
///-- 2005.04.04 / Lyul
///-- /? parameter 가 붙을 경우 해당 명령어의 Help 를 보여준다.
bool CGMCmdDlg::ViewHelp( TCHAR *atp_Str )
{
	/*
	int i = -1;	

	while( g_CmdInfo[++i].theCmdStr[0] != '\0' )
	{
		///-- 명령어 찾음
		if( lstrcmp( g_CmdInfo[i].theCmdStr, atp_Str ) == 0 )
		{	
			SendMessage( theListLog, LB_INSERTSTRING, 0, (LPARAM)_T( "-----------------------------------------" ) );
			ReadHelpFromFile( i );			
			SendMessage( theListLog, LB_INSERTSTRING, 0, (LPARAM)_T( "-----------------------------------------" ) );
			return true;
		}
	}
	*/
	return false;
}
///---------------------------------------------------------------------------
///-- ReadHelpFromFile
///---------------------------------------------------------------------------
///-- 2005.04.04 / Lyul
///-- Help 파일로 부터 해당 코드의 모든 Help 를 불러온다.
void CGMCmdDlg::ReadHelpFromFile( int ai_code )
{
	FILE* fp;
	TCHAR lta_buf[1024];
	TCHAR tmp_str[10];
	int li_code;
	bool	bRead  = true;			// 문서 다 읽었는지 체크		
	
	fp = _tfopen( _T("gm_help.txt"), _T("r") );
	
	///-- Open File
	if( fp == NULL )
	{		
		SendMessage( theListLog, LB_INSERTSTRING, 0, (LPARAM)_T( "GM_Help File Not Found." ) );
		return ;
	}

	///-- Line Process
	if( _fgetts( lta_buf, 1024, fp ) == 0 )
	{
		fclose( fp );
		return ;  
	}		
	do 
	{
		
		///-- Code 를 읽어온다.			
		_stscanf( lta_buf, _T("%s"), tmp_str );	
		
		///-- 처리 영역
		if( _tcsicmp( tmp_str, _T("{") ) == 0 )
		{
			if( _fgetts( lta_buf, 1024, fp ) == 0)
			{
				bRead = false;
				break;
			}

			///-- Code 를 읽어온다.			
			_stscanf( lta_buf, _T("%d"), &li_code );	
			do
			{
				if( _fgetts( lta_buf, 1024, fp ) == 0 )
				{
					bRead = false;
					break;
				}
				
				_stscanf( lta_buf, _T("%s"), tmp_str );
				
				if( _tcsicmp( tmp_str, _T("}") ) != 0 )
				{	
					///-- Add Text
					if( li_code == ai_code )
					{
						int len = Rstrlen( lta_buf );
						for(int i = 0; i <len ; i++)
						{
							if( lta_buf[i] == 0x0a || lta_buf[i] == 0x09) lta_buf[i] = 0x20;
						}
						SendMessage( theListLog, LB_INSERTSTRING, 0, (LPARAM)_T( lta_buf ) );
					}
						
				}
				
			}while( _tcsicmp( tmp_str, _T("}") ) != 0 );
			
		}
		
		// 다음 줄 읽기, 읽은게 없다면 string은 0으로									
		if( _fgetts( lta_buf, 1024, fp ) == 0 )
		{
			bRead = false;			
		}
		
	}  while( bRead != false ); // 더 이상 읽은게 없다면 루프 종료			
	
	fclose( fp );

}

SCmdInfo*	GetCmdInfo( char* aCmdStr )
{
	int i = 0;

	while( Rstrcmp(g_CmdInfo[i].theCmdStr ,  _RT("")) != 0)
	{
		if( Rstrcmp( g_CmdInfo[i].theCmdStr, RWCHAR(aCmdStr) ) == 0 )
		{
			return( &( g_CmdInfo[i] ) );
		}
		i++;
	}
	return NULL;
}

SCmdInfo*	GetCmdInfo( DWORD aCmdCode )
{
	int i = -1;
	while( Rstrcmp(g_CmdInfo[++i].theCmdStr ,  _RT("")) != 0)
	{
		if( g_CmdInfo[i].theCmdCode == aCmdCode )
		{
			return( &( g_CmdInfo[i] ) );
		}
	}
	return NULL;
}





///////////////////////////////  global  ///////////////////////////////////////

static CGMCmdDlg* g_CGMCmdDlg = NULL;

LRESULT CALLBACK	GMCmdDlgProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	return( g_CGMCmdDlg->DlgMsgProc( hWnd, iMessage, wParam, lParam ) );
}

WNDPROC OldEditProc;
LRESULT CALLBACK EditSubProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	
#ifdef	GM_CLIENT	
	
	if( hWnd == g_CGMCmdDlg->GetEditCmdHandle() )
	{
		switch (iMessage) 
		{
		case WM_KEYDOWN:
			switch( wParam) 
			{
			case VK_RETURN	: g_CGMCmdDlg->OnEditEnterKey();	break;
			case VK_UP		: g_CGMCmdDlg->OnEditUPKey();		break;
			}
			break;
		}
	}

#endif

	return CallWindowProc(OldEditProc,hWnd,iMessage,wParam,lParam);
}

///////////////////////////////////////////////////////////////////////////////



CGMCmdDlg::CGMCmdDlg()
{
	g_CGMCmdDlg = this;
	
	theMainDlgHandle = NULL;
	theParentWndHandle = NULL;
	theMainDlgBgBrush = NULL;
	theFont = NULL;
	
	theBufStrCmdOnly[0] = '\0';
	theBufStrCmdParam[0] = '\0';
	theBufCmdInfo = NULL;
	theBufMode = 0;
}

CGMCmdDlg::~CGMCmdDlg()
{
	SAFE_DELETE_ARRAY( g_CmdInfo );
	DestroyDlg();
}

void	CGMCmdDlg::CreateDlg( HINSTANCE hParentInst, HWND hParentWnd )
{
	if( theMainDlgHandle )
	{
		ShowWindow( theMainDlgHandle, SW_SHOW);
		return;
	}
	
	theParentWndHandle = hParentWnd;
	theParentInst = hParentInst;
	theMainDlgBgBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	RECT rect;
	GetWindowRect( hParentWnd, &rect );
	//	SetRect( &rect, 0, 0, 1024, 768 );
	theMainDlgRect.left = rect.left;	theMainDlgRect.right = rect.right;
	theMainDlgRect.top = rect.bottom;	theMainDlgRect.bottom = rect.bottom + DLG_HEIGHT;

#ifdef UNICODE_ROCK


	WNDCLASSW wndClass = {	0, GMCmdDlgProc, 0, 0, hParentInst,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor( NULL, IDC_ARROW ),
		theMainDlgBgBrush,
		NULL, L"GM Command"};
	RegisterClassW(&wndClass);
	
	theMainDlgHandle = CreateWindow( DLG_CLASS_NAME, DLG_CLASS_NAME, 
		WS_OVERLAPPED | WS_CAPTION,
		rect.left, rect.bottom, ( rect.right - rect.left ), DLG_HEIGHT,
		NULL, (HMENU)NULL, hParentInst, NULL);


#else 




	WNDCLASS wndClass = {	0, GMCmdDlgProc, 0, 0, hParentInst,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor( NULL, IDC_ARROW ),
		theMainDlgBgBrush,
		NULL, DLG_CLASS_NAME };
	RegisterClass(&wndClass);
	
	theMainDlgHandle = CreateWindow( DLG_CLASS_NAME, DLG_CLASS_NAME, 
		WS_OVERLAPPED | WS_CAPTION,
		rect.left, rect.bottom, ( rect.right - rect.left ), DLG_HEIGHT,
		NULL, (HMENU)NULL, hParentInst, NULL);

#endif 

	int l_nDevSkip = GetPrivateProfileInt( "Local", "TestFile", 0, "./setup.ini");
	if( l_nDevSkip )
	{
		_Create_GM_data();
	}
	else
	{
		_Create_GM_data_VF();
	}
	
	ShowWindow( theMainDlgHandle, SW_SHOW);
}

void	CGMCmdDlg::DestroyDlg()
{
	if( theMainDlgHandle )
	{
		DestroyWindow( theMainDlgHandle );
		theMainDlgHandle = NULL;
	}
	
	if( theMainDlgBgBrush )
	{
		DeleteObject( theMainDlgBgBrush );
		theMainDlgBgBrush = NULL;
	}
	
	if( theFont )
	{
		DeleteObject( theFont );
		theFont = NULL;
	}
}

void	CGMCmdDlg::ActiveDlg()
{
	SetActiveWindow( theMainDlgHandle );
}

LRESULT CGMCmdDlg::DlgMsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	
	switch(iMessage) 
	{
	case WM_CREATE:
	
#ifdef UNICODE_ROCK
		theEditHidden = CreateWindowW( L"edit", NULL, WS_CHILD | WS_BORDER,
			20, 10, 400, 20, hWnd, (HMENU)0, theParentInst, NULL);
		OldEditProc=(WNDPROC)SetClassLongW(theEditHidden,GCL_WNDPROC,(LONG)EditSubProc);
		
		theEditCmd = CreateWindowW( L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			20, 10, 480, 20, hWnd, (HMENU)IDC_EDIT_CMD, theParentInst, NULL );

		theListLog	= CreateWindowW( L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY,
			520, 10, 480, 110, hWnd, (HMENU)IDC_LIST_LOG, theParentInst, NULL );

		
#else
		
		theEditHidden = CreateWindow( "edit", NULL, WS_CHILD | WS_BORDER,
			20, 10, 400, 20, hWnd, (HMENU)0, theParentInst, NULL);
		OldEditProc=(WNDPROC)SetClassLong(theEditHidden,GCL_WNDPROC,(LONG)EditSubProc);
		
		theEditCmd = CreateWindow( "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			20, 10, 480, 20, hWnd, (HMENU)IDC_EDIT_CMD, theParentInst, NULL );
      
		theListLog	= CreateWindow( "listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY,
			520, 10, 480, 110, hWnd, (HMENU)IDC_LIST_LOG, theParentInst, NULL );


#endif
	  
		
#ifdef SAVE_PINFO_LOG_BUTTON				
		m_hLogSaveBtn = CreateWindow( "button", _T("Save Not PInfoLog"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						20, 40, 150, 35, hWnd, (HMENU)IDC_SAVE_BTN, theParentInst, NULL);			
#endif

#ifdef UNICODE_ROCK
		SendMessageW( theEditCmd, EM_LIMITTEXT, (WPARAM)42, (LPARAM)0 );
#else 
		SendMessage( theEditCmd, EM_LIMITTEXT, (WPARAM)80, (LPARAM)0 );
#endif 
	

		switch( g_RockClient.GetLanguage() ) 
		{
		default:
		case eLang_Korea: 
			theFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "굴림" );
			break;
		case eLang_Japan:
			theFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "굃굍 긕긘긞긏" );		
			break;
		case eLang_Indonesia:
			theFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, FONTSTR_ALL );		
			break;
		case eLang_English:
			theFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, FONTSTR_ALL );		
			break;
		case eLang_China:
			theFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "SIMSUN" );		
			break;
		case eLang_Taiwan:
			theFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "mingliu" );		
			break;
		case eLang_HongKong:
			theFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "mingliu" );		
			break; 
		case eLang_Singapore:
			theFont = CreateFont( 12, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, FONTSTR_ALL );		
	
		}
		
		SendMessage( theEditCmd, WM_SETFONT, (WPARAM)theFont, (LPARAM)MAKELPARAM(TRUE, 0) );
		SendMessage( theListLog, WM_SETFONT, (WPARAM)theFont, (LPARAM)MAKELPARAM(TRUE, 0) );
		return 0;
		
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
		
	case WM_COMMAND:
		{
			
#ifdef SAVE_PINFO_LOG_BUTTON			
			if( LOWORD(wParam) == IDC_SAVE_BTN )
			{
				BOOL IsSavePacketInfoPerSec = g_SPacketInfoPerSec.IsSavePacketInfoPerSec;
				IsSavePacketInfoPerSec = !IsSavePacketInfoPerSec;
				g_SPacketInfoPerSec.IsSavePacketInfoPerSec = IsSavePacketInfoPerSec;

				if( IsSavePacketInfoPerSec )
				{
					SetWindowText( m_hLogSaveBtn, _T( "Save PInfoLog" ) );
				}
				else
				{
					SetWindowText( m_hLogSaveBtn, _T( "Save Not PInfoLog" ) );			
				}			
			}			
			else 
#endif
			if( LOWORD( wParam ) == IDC_LIST_LOG && HIWORD( wParam ) == LBN_DBLCLK )
			{
				char str[128];
#ifdef UNICODE_ROCK
				SendMessageW( theListLog, LB_GETTEXT, 
					( WPARAM )SendMessageW( theListLog, LB_GETCURSEL, 0, 0 ), 
					( LPARAM )str );
				
				if( RWCHAR(str)[0] != '\0' )
				{
					
					SetWindowTextW( theEditCmd, RWCHAR(str) );
					SetFocus( theEditCmd );
				}
#else 
				SendMessage( theListLog, LB_GETTEXT, 
					( WPARAM )SendMessage( theListLog, LB_GETCURSEL, 0, 0 ), 
					( LPARAM )str );
				
				if( str[0] != '\0' )
				{
					
					SetWindowText( theEditCmd, str );
					SetFocus( theEditCmd );
				}
#endif 
			}
		}
		return 0;

	case WM_SETFOCUS:
		
		SetFocus( theEditCmd );
		return 0;
		
	case WM_KEYDOWN:
		if((int)wParam == VK_TAB)
		{
			if( GetAsyncKeyState( VK_CONTROL ) < 0 )
			{
				SetActiveWindow( theParentWndHandle );
			}
		}
		break;
		
	case WM_NCHITTEST:
		{
			LRESULT ret = DefWindowProc( hWnd, iMessage, wParam, lParam );
			if( ret == HTCAPTION ) ret = HTCLIENT;
			return ret;
		}
		
	case WM_CLOSE:
	case WM_DESTROY:
		//SetClassLong(theEditCmd,GCL_WNDPROC,(LONG)OldEditProc);
		//by simwoosung
		SetClassLong(theEditHidden,GCL_WNDPROC,(LONG)OldEditProc);
		//		PostQuitMessage(0);
		return 0;
	}
#ifdef UNICODE_ROCK
	return( DefWindowProcW( hWnd, iMessage, wParam, lParam ) );	
#else 
	return( DefWindowProc( hWnd, iMessage, wParam, lParam ) );	
#endif 
	

}

BOOL	CGMCmdDlg::CommandSend( char* aCmd, char* aCmdParam )
{

	if( g_RockClient.GetGameMode() != GMODE_NORMAL ) return FALSE;
	
#ifdef	GM_CLIENT	
	
	int	nParam[4] = { -1, -1, -1, -1 };
	RTCHAR sParam[64] = { 0, };
	
	/// 정수 파라미터와 문자열 파라미터가 모두 필요한 경우 
	if( theBufCmdInfo->theNParamNum && theBufCmdInfo->theSParamNum )
	{
		char str[96]= {0,};
		char nParamStr[96] = {0,};

		SAFE_STR_CPY( str, aCmdParam, 96 );
		
		RTCHAR* p = Rstrchr( RWCHAR(str), _RT(' ') );
		
		if( p == NULL ) return FALSE;

		*p = _RT('\0');

		Rstrcpy( sParam, RWCHAR(str));			/// 문자열 파라미터를 먼저 걸러내고 
		Rstrcpy( RWCHAR(nParamStr), p + 1);	/// 숫자열 파리미터만 따로 스트링으로 만든 후 파싱 
	
		int ret;
		switch( theBufCmdInfo->theNParamNum )
		{
		case 1: ret = Rsscanf( RWCHAR(nParamStr), _RT("%d"), &nParam[0] );											break;
		case 2: ret = Rsscanf( RWCHAR(nParamStr), _RT("%d %d"), &nParam[0], &nParam[1] );								break;
		case 3: ret = Rsscanf( RWCHAR(nParamStr), _RT("%d %d %d"), &nParam[0], &nParam[1], &nParam[2] );				break;
		case 4: ret = Rsscanf( RWCHAR(nParamStr), _RT("%d %d %d %d"), &nParam[0], &nParam[1], &nParam[2], &nParam[3] );	break;
		}
		if( ret <= 0 || ret > theBufCmdInfo->theNParamNum ) return FALSE;
		if( sParam[0] == _RT('\0') ) return FALSE;
#ifdef UNICODE_ROCK
		sParam[(RP_MAX_PC_NAME>>1) - 1] = _RT('\0');	/// 이 경우 문자열 파라미터는 모두 PC Name이다(아닌 경우가 있다면 별도 처리 필요)
#else
		sParam[RP_MAX_PC_NAME - 1] = '\0';	/// 이 경우 문자열 파라미터는 모두 PC Name이다(아닌 경우가 있다면 별도 처리 필요)
#endif

	}
	
	/// 정수 파라미터만 필요한 경우 
	else if( theBufCmdInfo->theNParamNum )
	{
		int ret;
		switch( theBufCmdInfo->theNParamNum )
		{
		case 1: ret = Rsscanf( RWCHAR(aCmdParam), _RT("%d"), &nParam[0] );											break;
		case 2: ret = Rsscanf( RWCHAR(aCmdParam), _RT("%d %d"), &nParam[0], &nParam[1] );								break;
		case 3: ret = Rsscanf( RWCHAR(aCmdParam), _RT("%d %d %d"), &nParam[0], &nParam[1], &nParam[2] );				break;
		case 4: ret = Rsscanf( RWCHAR(aCmdParam), _RT("%d %d %d %d"), &nParam[0], &nParam[1], &nParam[2], &nParam[3] );	break;
		}
			
		if(theBufCmdInfo->theCmdCode == 4026)
		{
			nParam[1] = nParam[0];
			int Unique = GetPickNonPlayerID();
			if( Unique !=  -1) 
			{
				nParam[0] = Unique;
				
			}
			else 
			{
				return FALSE;
			}
			
		}
        
		if( ret <= 0 || ret > theBufCmdInfo->theNParamNum ) return FALSE;
	}
	/// 문자열 파라미터만 필요한 경우 
	else if( theBufCmdInfo->theSParamNum )
	{
		int length = RP_MAX_PC_NAME;	/// 대부분 문자열 파라미터는 PC Name
		/// 공지의 경우는 채팅 길이 
		if( theBufCmdInfo->theCmdCode == 2001 || theBufCmdInfo->theCmdCode == 2002  ||
			theBufCmdInfo->theCmdCode == 2005 || theBufCmdInfo->theCmdCode == 2006 )
		{
			length = 64;
		}
		/// 길이를 안전하게 걸러서 전송 
#ifdef UNICODE_ROCK
		Rstrncpy( sParam , RWCHAR(aCmdParam), 64 );
		sParam[63] = _RT('\0');
#else
		strncpy( sParam, aCmdParam, length - 1 );
		sParam[length - 1] = '\0';
#endif
		// 소생명령어의 경우
		if(theBufCmdInfo->theCmdCode == 6001)
		{
			if( sParam[0] == _RT('\0'))
			{
				if(!GetNonPlayerInfo(RCHAR(sParam)))
					return TRUE;
			}
		}

		if( sParam[0] == _RT('\0' )) return FALSE;
	}	
	
	switch( theBufCmdInfo->theCmdCode )
	{
		case 1:
			g_RockClient.CreateExampleEffect( ( DWORD ) nParam[0] , nParam[1] );
			return TRUE;

		case 2:
			g_Sound.Play( nParam[0] , NULL );
			return TRUE;
	
		//.........................................................................................................
		// 선택되어 있는 NPC 정보를 출력한다
		//.........................................................................................................
		case n_RequestNPCInfo:
			
			NonPlayerInfoWnd.thePickedUserID = -1;

			if( g_Map.m_NpcTable[g_Map.m_nPickedNpc].race == c_NpcRace_Mob )
			{
				nParam[0] = g_Map.m_Npc[g_Map.m_nPickedNpc].GetUnique();
			}
			else
			{
				return TRUE;
			}
	}

	char log[256]={0,};
	Rsprintf( RWCHAR(log), _RT("[Send_RpCsGMCmd]cmd=(%d) sparam=(%s) nparam=%d, %d, %d, %d"), 
		theBufCmdInfo->theCmdCode, RWCHAR(sParam), nParam[0], nParam[1], nParam[2], nParam[3] );
	
#ifdef UNICODE_ROCK

	SendMessageW( theListLog, LB_INSERTSTRING, 0, (LPARAM)log );

#else
	
	SendMessage( theListLog, LB_INSERTSTRING, 0, (LPARAM)log );

#endif
	

	g_RockClient.Send_RpCsGMCmd( theBufCmdInfo->theCmdCode, nParam, RCHAR(sParam) );
#endif
	
	return TRUE;
}

///---------------------------------------------------------------------------
///-- OnEditEnterKey
///---------------------------------------------------------------------------
void	CGMCmdDlg::OnEditEnterKey()
{

#ifdef	GM_CLIENT	
	
	char str[96] = {0,} , strCmd[96] = {0,} , strParam[96] = {0,};
#ifdef UNICODE_ROCK
	GetWindowTextW( theEditCmd, RWCHAR(str), sizeof( str ) );
#else
	GetWindowText( theEditCmd, str, sizeof( str ) );
#endif
	int len  = 0 ; 

	SAFE_STR_CPY( strCmd, str, 96 );
	RTCHAR* p = Rstrchr( RWCHAR(strCmd), _RT(' ') );
	if( p )
	{
		*p = _RT('\0');
		
		len = Rstrlen(p+1);

		SAFE_STR_CPY( strParam, p + 1, 96 );
	}
	
	///-- 명령어 체크
	theBufCmdInfo = GetCmdInfo( strCmd );
	if( theBufCmdInfo == NULL )
	{
		
		if( Rstrcmp(RWCHAR(strCmd) , _RT("/rock1" ) )== 0 ) 
		{
			 g_RockClient.m_EtcInfo.DevMode = 1;
			SetWindowText( theEditCmd, "" );
			return;
		}
		
		if( Rstrcmp(RWCHAR(strCmd) , _RT("/rock2") ) == 0 ) 
		{
			g_RockClient.m_EtcInfo.DevMode = 2;
			SetWindowText( theEditCmd, "" );
			return;
		}
		
		if( Rstrcmp(RWCHAR(strCmd) , _RT("/msg") ) == 0 ) 
		{
			SetWindowText( theEditCmd, "" );
			g_RockClient.m_EtcInfo.DevMode = 3;
			return;
		}
		RMessageBox( theMainDlgHandle, G_STRING_CHAR( IDS_GMMSG_NOT_GMCMD ), G_STRING_CHAR( IDS_WND_ALARM ), MB_OK|MB_ICONINFORMATION );
	
		return;
	}

	///-- HELP Display
	/*
	if( _tcsstr( str, _T( "/?" )) )
	{	
		if( ViewHelp( strCmd ) ) return;
	}
	*/
	
	/// 여기서 스트링 파싱해서 소켓에 명령어 전송 
	if( CommandSend( strCmd, strParam ) != TRUE )
	{
		RMessageBox( theMainDlgHandle, G_STRING_CHAR( IDS_GMMSG_ERROR_CMD_PARAM ), G_STRING_CHAR( IDS_WND_ALARM ), MB_OK|MB_ICONINFORMATION );
		return;
	}
	
	memcpy(theBufStrCmdOnly ,strCmd , sizeof(theBufStrCmdOnly) );
	memcpy(theBufStrCmdParam ,strParam , sizeof(theBufStrCmdParam) );
	//SAFE_STR_CPY( theBufStrCmdOnly, strCmd, 32 );
	//SAFE_STR_CPY( theBufStrCmdParam, strParam, 96 );
	theBufMode = 0;
	 
#ifdef UNICODE_ROCK
	SetWindowTextW( theEditCmd, L"" );
	SendMessageW( theListLog, LB_INSERTSTRING, 0, (LPARAM)str );
#else 
	SetWindowText( theEditCmd, "" );
	SendMessage( theListLog, LB_INSERTSTRING, 0, (LPARAM)str );
#endif 

	
#endif

}

void	CGMCmdDlg::OnEditUPKey()
{
	theBufMode = ++theBufMode % 3;
	switch( theBufMode )
	{
	case 0: SetWindowText( theEditCmd, "" );				break;
	case 1: SetWindowText( theEditCmd, theBufStrCmdOnly );	break;
	case 2: 
		{
			RTCHAR str[96] = {0,};
			Rsprintf( str, _RT("%s %s"), RWCHAR(theBufStrCmdOnly), RWCHAR(theBufStrCmdParam) );
#ifdef UNICODE_ROCK
			SetWindowTextW( theEditCmd, str );				break;
#else
			SetWindowText( theEditCmd, str );				break;
#endif	
		}
	}
}

void	CGMCmdDlg::OnScGMCmd( DWORD aCommand, int* aNParamList, char* aSParam )
{
	char  str[128] = {0,} ;
	char  Citizen[64] = {0,};
	char  Class[64] = {0,};
	char  Connect[64] = {0,};

	switch( aCommand )
	{
	case n_ConnectCount : 
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_GMMSG_CONNECTCNT ), aNParamList[0] );		// "RET[/몇명] : 현재 접속자수는 (%d)명입니다", 
		break;
	case n_ConnectMapCount : 
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_GMMSG_CONNECTCNT_MAP ), aNParamList[0] );			// "RET[/맵몇명] : 현재 해당 맵의 접속자수는 (%d)명입니다", 
		break;
	case n_RainConnectCount: 
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_GMMSG_CONNECTCNT_RAIN ), aNParamList[0] );       // "RET[/몇명레인] : 현재 레인의 접속자수는 (%d)명입니다"          
		break;
    case n_MillenaConnectCount : 
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_GMMSG_CONNECTCNT_MILLENA ), aNParamList[0] );		// "RET[/몇명밀레나] : 현재 밀레나의 접속자수는 (%d)명입니다"
		break;				
	case n_SearchPC : 
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_GMMSG_SEARCHPC_MAP_POS ), aNParamList[0], aNParamList[1], aNParamList[2] );		// "RET[/어디] : 현재 해당 캐릭터의 위치는 맵 (%d)에 좌표 (%d, %d)입니다"
		break;
    case n_CharacterInfo : 
		GetCitizen( aNParamList[1] , Citizen);
		GetClass( aNParamList[2], Class);
		GetConnect(aNParamList[3], Connect);
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_GMMSG_GETPCINFO_CHARNAME ), aNParamList[0], Citizen, Class, Connect, RWCHAR(aSParam) );			// "RET[/무엇] : 현재 해당 캐릭터의 정보는 레벨(%d) 국가(%s) 직업(%s) 접속여부(%s) 계정명(%s)입니다", 
		break; 
    case n_AccountInfo : 
		GetCitizen( aNParamList[1] , Citizen);
		GetClass( aNParamList[2], Class);
		GetConnect(aNParamList[3], Connect);
		Rsprintf( RWCHAR(str), G_STRING_CHAR( IDS_GMMSG_GETPCINFO_ACCOUNT ), aNParamList[0], Citizen, Class, Connect);		// "RET[/무엇] : 현재 해당 캐릭터의 정보는 레벨(%d) 국가(%s) 직업(%s) 접속여부(%s)입니다", 
		break;	
		
	default :
		{
			aSParam[63] = '\0';
			aSParam[62] = '\0';

			SCmdInfo* cmdInfo = GetCmdInfo( aCommand );
			if( cmdInfo )
				Rsprintf( RWCHAR(str), _RT("RET[%s] : NParam=(%d, %d, %d, %d) SParam=(%s)"), cmdInfo->theCmdStr, aNParamList[0], aNParamList[1], aNParamList[2], aNParamList[3], RWCHAR(aSParam) );
			else
				Rsprintf( RWCHAR(str), _RT("RET[/unknown:%u] : NParam=(%d, %d, %d, %d) SParam=(%s)"), aCommand, aNParamList[0], aNParamList[1], aNParamList[2], aNParamList[3], RWCHAR(aSParam) );
		}
		break;
	}
#ifdef UNICODE_ROCK
		SendMessageW( theListLog, LB_INSERTSTRING, 0, (LPARAM)str );
#else
		SendMessage( theListLog, LB_INSERTSTRING, 0, (LPARAM)str );
#endif

}

void	CGMCmdDlg::OnScGMCmdErr( DWORD aCommand, int aErrorCode )
{
	if(aErrorCode <= 0)
		return;
	
	RTCHAR str[128] = {0,};
	SCmdInfo* cmdInfo = GetCmdInfo( aCommand );
	
	if( aErrorCode >= 0 && aErrorCode <= GM_CMD_ERR_NUM )
	{
		if( cmdInfo )
			Rsprintf( str, _RT("ERR[%s] : %s"), cmdInfo->theCmdStr, g_GmCmdErrStr[aErrorCode-1] );
		else
			Rsprintf( str, _RT("ERR[/unknown:%u] : %s"), aCommand, g_GmCmdErrStr[aErrorCode-1] );
	}
	else
	{
		if( cmdInfo )
			Rsprintf( str, _RT("ERR[%s] : Not Define Error(%d)"), RWCHAR(cmdInfo->theCmdStr), (aErrorCode-1) );
		else
			Rsprintf( str, _RT("ERR[/unknown:%u] : Not Define Error(%d)"), aCommand, (aErrorCode-1) );
	}
#ifdef UNICODE_ROCK
	SendMessageW( theListLog, LB_INSERTSTRING, 0, (LPARAM)str );
#else
	SendMessage( theListLog, LB_INSERTSTRING, 0, (LPARAM)str );
#endif
	
}


int CGMCmdDlg::GetPickNonPlayerID()
{
	int UniqueID = -1 ; 

	if(g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lUnique != -1 ) 
	{
		UniqueID = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lUnique;
	}
	else 
	{
		RMessageBox( theMainDlgHandle, G_STRING_CHAR( IDS_SYSPR_INVALID_TARGET ), G_STRING_CHAR( IDS_WND_ALARM ), MB_OK|MB_ICONINFORMATION );
	}

	return UniqueID;
}

//by simwoosung
BOOL CGMCmdDlg::GetNonPlayerInfo(char *pSParam)
{
	//NPC 창이 활성화 되어 있을 경우
	if(NonPlayerInfoWnd.IsVisible)
	{
		//유저 캐릭터가 아니면
		if( NonPlayerInfoWnd.thePickedNpcID != -1 && NonPlayerInfoWnd.thePickedNpcID == MAX_USER_INTABLE )
		{
			RMessageBox( theMainDlgHandle, G_STRING_CHAR( IDS_SYSPR_INVALID_TARGET ), G_STRING_CHAR( IDS_WND_ALARM ), MB_OK|MB_ICONINFORMATION );
			return FALSE;
		}
		else
		{
			//SAFE_STR_CPY( pSParam, g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.thePCName, 64 );
			memcpy(pSParam , g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.thePCName , RP_MAX_PC_NAME );
			//memcpy( pSParam, g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.thePCName, 64 );
		}
	}
	else
	{
		RMessageBox( theMainDlgHandle, G_STRING_CHAR( IDS_WND_SEL_CHAR ), G_STRING_CHAR( IDS_WND_ALARM ), MB_OK|MB_ICONINFORMATION );		  
		return FALSE;
	}	
	
    return TRUE;
}

void CGMCmdDlg::GetCitizen(int iCitizen , char * strCitizen)
{
	switch( iCitizen )
	{
		case RAIN:       SAFE_STR_CPY(strCitizen, G_STRING_CHAR( IDS_RAIN ), 64 );        break;  
		case MILLENA:    SAFE_STR_CPY(strCitizen, G_STRING_CHAR( IDS_MILLENA ), 64 );      break;
	}
}

void CGMCmdDlg::GetClass(int iClass , char * strClass)
{
	GetClassText( iClass, strClass );

	return;
}

void CGMCmdDlg::GetConnect(int iConnect ,char * strConnect)
{
	
	if(iConnect)
		SAFE_STR_CPY(strConnect, _RT("O"), 64);
	else
		SAFE_STR_CPY(strConnect, _RT("X"), 64);
}


