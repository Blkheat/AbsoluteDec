//----------------------------------------------------------------------------------------------------------------------
// 파  일   명 : WinMain.cpp
// 용       도 : 윈도우즈 프로그램의 시작점은 도스의 main이 아닌 WinMain이다. 따라서 모든 윈도우즈 프로그램은 WinMain
//               에서부터 실행을 시작
//----------------------------------------------------------------------------------------------------------------------
#ifndef WIN_MAIN_CPP
#define WIN_MAIN_CPP


#include "RockClient.h"
#include "profile/profile.h"
#include <Ole2.h>
#include "HShield.h"

// crypto 
extern void	CreateNetPack();
extern void	CleanupNetPack();


// RockClient
extern void CloseAllWnd();
extern int CALLBACK AhnHS_Callback(long lCode, long lParamSize, void* pParam);
extern float g_fGamma;
extern float g_fContrast;


// Global Value
HANDLE	g_hClientMutex = INVALID_HANDLE_VALUE;
#define	CLIENT_AVOID_DUPLICATION_MUTEX		"ClientAvoidDuplicationMutexOnProjectORock"
unsigned short g_ramp[3*256];
int   g_nOverBright = 0;	/// 이건 별 의미가 없네...


// Global Function
void gOnCrash()
{
	g_RockClient.OnCrash();
}

// 런처 업데이트(런처는 게임 클라이언트를 업데이트 하고, 클라이언트는 런처를 업데이트한다)
bool UpdateLauncher()
{
	FILE* fp = fopen( "_Updater.exe", "r" );
	if( fp )
	{
		fclose( fp );
		Sleep( 1500 );
		
# ifdef CHINA_VERSION		
		unlink( "Deco.exe" );
		unlink( "LnD.exe" );
		rename( "_Updater.exe", "LnD.exe" );
# elif defined (HONGKONG_VERSION)
		unlink( "Seiya.exe" );
		rename( "_Updater.exe", "Seiya.exe" );
#else 
		unlink( "Deco.exe" );
		rename( "_Updater.exe", "Deco.exe" );
#endif
		RMessageBox( NULL, G_STRING_CHAR(IDS_WND_RESTART_GAME), G_STRING_CHAR(IDS_RockSoft), MB_ICONINFORMATION );
		
		All_Delete_g_Variable_Ptr();
		return true;
	}

	return false;
}

// 스크린샷 폴더 확인
bool MakeScreenShotFolder()
{
	TCHAR file_path[MAX_PATH]	= {0,};
	wsprintf( file_path, "%s\\ScreenShot", g_RockClient.GetRootDir() );
	DWORD dAttr =  ::GetFileAttributes(file_path);// 파일 정보 조사
	
	if( FILE_ATTRIBUTE_DIRECTORY == dAttr )	
		return true;

	//해당 폴더가 존재하지 않으면...
	if(!::CreateDirectory(file_path, NULL))	 //디렉토리를 생성한다.
	{
		// 에러가 발생할 소지가 있는 모든 API 함수들은 리턴값으로 에러 유무를 리턴해 준다.
		// 그러나 함수의 리턴값으로는 에러 발생 사실만 알려줄 뿐 구체적으로 어떤 에러인지 알 수 없다.
		DWORD dErr = ::GetLastError();// 발생한 에러의 종류 확인
		if( dErr != ERROR_ALREADY_EXISTS )	
		{
			RMessageBox( NULL, _RT("Failed to create Screen Folder"), G_STRING_CHAR(IDS_RockSoft), MB_ICONINFORMATION );						
			return false;
		}
		else
			SetFileAttributes( file_path, FILE_ATTRIBUTE_DIRECTORY );
	}

	return true;
}


// 클라이언트 용 로그기록 용 폴더를 만든다. - 재현이 어려운 버그 확인용
bool MakeClientLogFolder()
{
	TCHAR file_path[MAX_PATH]	= {0,};
	wsprintf( file_path, "%s\\ClientLog", g_RockClient.GetRootDir() );
	DWORD dAttr =  ::GetFileAttributes(file_path);
	
	if( FILE_ATTRIBUTE_DIRECTORY == dAttr )
		return true;

	//해당 폴더가 존재하지 않으면...
	if(!::CreateDirectory(file_path, NULL))	 //디렉토리를 생성한다.
	{
		DWORD dErr = GetLastError();
		if( dErr != ERROR_ALREADY_EXISTS )	
		{
			RMessageBox( NULL, _RT("Failed to create ClientLog Folder"), G_STRING_CHAR(IDS_RockSoft) , 
				MB_ICONINFORMATION );
			return false;
		}
		else
			SetFileAttributes( file_path, FILE_ATTRIBUTE_DIRECTORY );	
	}

	return true;
}



INT	WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR pStr, INT)
{	
	ProfileInit();
	
	HRESULT ret;
	HWND hwnd = NULL;

	//메세지박스 때문에 string.dat 로딩 한다..
	if(!g_RockClient.InitLanguege())
	{
		RMessageBox(NULL, _RT("String Error") , _RT("Error") ,  MB_ICONINFORMATION );
		g_VFInterface.CloseFileSystem();
		return -1;
	}	
	
	// 런처 업데이트
	if(UpdateLauncher())
		return -1;
	


	///-- 아래 핸들과 뮤텍스 부분은 클라이언트가 동시에 실행되는걸 방지하기 위한 코드이다.
	///-- 이후 문제가 발생한다면.. 클라이언트 두개실행하는 다른 방법이 존재 하는지 의심해봐야 할듯 하다.

#ifdef CHINA_VERSION
	hwnd =  FindWindow( NULL, _T( "LnD Online" ) );
#elif defined(HONGKONG_VERSION)
	hwnd =  NULL;//FindWindowW( NULL, _RT( "SeiyaOnline" ) );
#elif defined(KOREA_VERSION) 
	hwnd =  FindWindow( NULL, _T( "시공찬가" ) );
#else 

	#ifdef UNICODE_ROCK
		hwnd =  FindWindowW( NULL, _RT("Deco-Online"));
	#else 
		hwnd =  FindWindow( NULL, _RT("Deco-Online"));
	#endif 
	
#endif

#ifndef _DEBUG
	RTCHAR ErrStr[256]={0,};
	if( hwnd != NULL )
	{
		Rsprintf(ErrStr , _RT("%s - H1 %d") , G_STRING_CHAR(IDS_WND_RUN_CLIENT) , hwnd );
		RMessageBox( NULL,  ErrStr  , G_STRING_CHAR(IDS_RockSoft) , MB_ICONSTOP );		
		return -1;
	}

	g_hClientMutex = ::CreateMutex( NULL, FALSE, CLIENT_AVOID_DUPLICATION_MUTEX );
	HANDLE hMutexHandle = g_hClientMutex;
	if( hMutexHandle )
	{
		if( ::GetLastError() == ERROR_ALREADY_EXISTS )
		{
			RMessageBox( NULL, G_STRING_CHAR(IDS_WND_RUN_CLIENT) , G_STRING_CHAR(IDS_RockSoft) , MB_ICONSTOP );		
			return -1;
		}
	}
#endif 

	All_New_g_Variable_Ptr();
	
	// 스크린샷 폴더 확인
	if(!MakeScreenShotFolder())
		return -1;

	// 클라이언트 용 로그기록 용 폴더를 만든다.
	if(!MakeClientLogFolder())
		return -1;
	
#ifdef C_HACK_SHIELD

	if(!g_RockClient.HS_Init())
	{
		g_RockClient.HS_UnInit() ;
		All_Delete_g_Variable_Ptr();
		
		return -1;
	}
   	
	if ( !g_RockClient.HS_StartService() )
	{
		g_RockClient.HS_StopService();
		g_RockClient.HS_UnInit();
				
		All_Delete_g_Variable_Ptr();

		return -1;
	}
	
	_AhnHS_SaveFuncAddress(1, AhnHS_Callback );

#endif



#ifdef C_GAME_GUARD

	g_RockClient.pNpgl = new CNPGameLib("SeiyaOnlineHK");
	
	if (!g_RockClient.NP_Init())
	{		
		All_Delete_g_Variable_Ptr();
		return -1;
	}

#endif 

#ifdef	_DEBUG
	//by simwoosung - 디버그 모드 - 프로그램 정상종료시에만 
	//메모리 누수를 디버그창에 출력한다.
	//Shift-F5로는 메모리 누스 출력 하지않음
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	
	
	VERIFY( ROCKBASE_INITIALIZE() );
	R_RELEASE_DFMF();
	RLG_SET_OPTS( RLG_OPT_ODS );	/// n_Default = n_ODS | n_Manager 임. Log Manager에 로그 안남기도록...


#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)
	RLG_DISABLE();
#endif

	
	SetExceptionHandler( gOnCrash );	/// 자동 버그 리포트 

	CreateNetPack();


	/*if(0)
	 goto RET_FALSE;*/

	///////////////////////////////////////////////////////////////
	/// release 모드에서 체크 : 반드시 런쳐를 통해 실행되어야 합니다. 

	int ErrorCode = -1 ;
	
	
#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)

	//by simwoosung 인자 4개를 받는 부분....
	char strParam1[128] = {0,};
	char strParam2[128] = {0,};
	char strParam3[128] = {0,};
	char strParam4[128] = {0,};
	int nCode = 0;

	// 스페이스로 분리된 각각의 인자를 따로 받아낸다.
	sscanf(pStr, "%s %s %s %s", strParam1, strParam2, strParam3, strParam4);	
	
	char strParam[128];
	char binParam[12]; 
	int VersionLen = 0; 

	DWORD	gameVersion;
	int TestVersion =0 ;

	VersionLen = strlen( strParam1 );

	//게임 버전 체크
	if( VersionLen != ( 12 * 3 ) )
	{
		ErrorCode = VersionLen ;
		goto RET_FALSE;
	}

	{
		char temp[32];
		int	n;
		strncpy( strParam, strParam1, 128 );
		for( int i = 0; i < 12; ++i )
		{
			temp[0] = strParam[ i * 3 ];
			temp[1] = strParam[ i * 3 + 1 ];
			temp[2] = strParam[ i * 3 + 2 ];
			temp[3] = '\0';
			n = atoi( temp );
			binParam[i] = (char)n;
		}

		CRockCrypto crypto; 
		BYTE key0[] = { 0x53, 0x5F, 0xC9, 0x8D, 0x6B, 0x08, 0x10, 0xF8, 0xEB, 0xEA, 0x33, 0x90 }; 
		BYTE key1[] = { 0xCE, 0x5D, 0xAA, 0x90, 0x43, 0x2D, 0xA1, 0x0C, 0x41, 0xE9, 0x89, 0xB3, 0x5E, 0xC8, 0x01, 0xD1 };
		crypto.SetKey( (const char *)key0, (const char *)key1, NULL, NULL );
		crypto.Decrypt( binParam, sizeof( binParam ) );

		DWORD	param;
		BYTE	var0;
		WORD	var1, var2/*, check*/;
		BYTE	ver_major, ver_minor, ver_bugfix; 

		int pos = 0;
		memcpy( &var0, &(binParam[pos]), sizeof( var0 ) );				pos += sizeof( var0 );
		memcpy( &ver_bugfix, &(binParam[pos]), sizeof( ver_bugfix ) );	pos += sizeof( ver_bugfix );
		memcpy( &ver_major, &(binParam[pos]), sizeof( ver_major ) );	pos += sizeof( ver_major );
		memcpy( &var1, &(binParam[pos]), sizeof( var1 ) );				pos += sizeof( var1 );
		memcpy( &param, &(binParam[pos]), sizeof( param ) );			pos += sizeof( param );
		memcpy( &var2, &(binParam[pos]), sizeof( var2 ) );				pos += sizeof( var2 );
		memcpy( &ver_minor, &(binParam[pos]), sizeof( ver_minor ) );

		if( param != 3034980102 ) 
		{	
			ErrorCode = 2;
			goto RET_FALSE;	/// 약속된 파라미터가 일치하지 않음 
		}
		/// var1에서 80 = 8초. 오차 8초 이내에만 유효로 인정 
	/*
		if( ml_language	!= eLang_Korea)
		{
			check = ( timeGetTime() / 100 ) % ( 1 << 16 ); 
			if( check > var1 + 200 )	
			{
				goto RET_FALSE; /// 실행 시간 체크 -> 안맞다! 
			}
		}
			*/			
		gameVersion = ver_major * 1000 + ver_minor * 10 + ver_bugfix;
   
// 이제 3개국 버젼 관리에 들어갑니다.
// 디파인 주석처리 함 
		////////////////////////////////////////////////////////////////
		/// 실제 게임 서비스에서는 반드시 체크해야 합니다. : 내부 개발중일 때는 일단 띵가먹자 
		// gameVersion 런쳐에서 가져 오는값 setup.ini >  g_This_Client_Version 클라이언트 설정된 버젼
		// 클라이언트 버젼 관리 서버로부터 버전을 체크합니다.
		

	//	if( gameVersion > g_This_Client_Version )
	//	{
	//		char str[128];
	///		wsprintf( str, "Version error! This client version is %d.%02d.%d", g_This_Client_Version / 10000, ( g_This_Client_Version % 1000 ) / 10, g_This_Client_Version % 10 );
	//		MessageBox( NULL, str, "Deco", MB_ICONSTOP );
	///		goto RET_FALSE;
	//	}
	}
	///////////////////////////////////////////////////////////////
	//이쪽 부분에 웹런칭 관련 처리(넷마블, 게임엔젤...)
	
	// 2번째 파라미터가 NFrom 값
#ifdef CHINA_VERSION
	//중국은 구역정보가 넘어 온다. 
	nCode = atoi(strParam2);
	g_RockClient.SetAreaInfoNum(nCode);

	//strParam3 IP strParam4 Port정보



	//g_RockClient.m_GameTypeInfo.theFrom = ( NFrom )nCode;
#else
	
	nCode = atoi(strParam2);
	switch(nCode)
	{
		
	case 1: // n_JapanNetMarble & n_Kr_PublicWeb
	case 2:	// n_Kr_GameAngel
	case 3:	// n_Kr_JJangFile
	case 4:	// n_Kr_Chollian
	case 5:	// n_Kr_Netimo
	case 6:	// n_Kr_Pigwing
	case 7: // n_Kr_MyClub
	case 8: // n_Kr_DreamLine
	case 9: // n_Kr_SantaGame
	case 10: //	n_Kr_LuluGame
		{
			if( g_RockClient.GetLanguage() != eLang_Korea )
			{
				g_RockClient.m_GameTypeInfo.theFrom = ( NFrom )nCode;
				strcpy(g_RockClient.m_GameTypeInfo.theUserID, strParam3);	//현재 사용안함 
				strcpy(g_RockClient.m_GameTypeInfo.theUserPass, strParam4);	//현재 사용안함 
			}
			else
			{
				g_RockClient.m_GameTypeInfo.theFrom = ( NFrom )nCode;
			}
		}
		break;
	}

#endif	//CHINA_VERSION

	//런쳐버젼비교 
/*
	#ifdef LAUNCHER_UPGREADE_VERSION
		{		
			int LauncherVersion = atoi(strParam3);
			
			if(LAUNCHER_VERSION != LauncherVersion )
			{
				//MessageBox( NULL, "Launcher version is not match.", "Error", MB_ICONINFORMATION );
				RMessageBox( NULL, G_STRING_CHAR(IDS_WND_NOT_LAUNCHER_VERSION) , G_STRING_CHAR(IDS_RockSoft), MB_ICONINFORMATION );
				return -1;
			}
		}
	#endif
*/

#endif	//_DEBUG
	 
	ShowCursor( TRUE );

	if(OleInitialize(NULL) != S_OK)
	{

#ifdef C_HACK_SHIELD
	g_RockClient.HS_StopService();
	g_RockClient.HS_UnInit();
#endif

		All_Delete_g_Variable_Ptr();
		return FALSE;
	}	

#if !defined(TEST_REALEASE_CLIENT)
	CloseAllWnd();	
#endif


	//데코 게임창 생성 
	g_RockClient.Create( hInstance );

	if( nRui && !nRui->m_IsSurpportHCursor)
	{
		SetFocus(g_RockClient.GetApp());
		ShowCursor( FALSE );
		ShowCursor( FALSE );
		g_RockClient.m_IsCursor = FALSE;
		g_RockClient.m_isCursorInit = true;
	}

	
#ifdef C_GAME_GUARD
	g_RockClient.pNpgl->SetHwnd(g_RockClient.GetApp());
#endif 
	

#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)	/// 감마 조절 : 릴리즈 버전만 by sooree
	/// 원래 감마값 기억 
	GetDeviceGammaRamp( ::GetDC( NULL ), g_ramp );
	/// 옵션에 따른 설정 
	Drv_SetGamma( ::GetDC( NULL ), g_fGamma, g_nOverBright, g_fContrast );

#endif
	
	// 게임 루프 시작
	ret = g_RockClient.Run();

	VERIFY( ROCKBASE_SHUTDOWN() );
	
	if( g_hClientMutex != INVALID_HANDLE_VALUE )
	{
		CloseHandle( g_hClientMutex );
		g_hClientMutex = INVALID_HANDLE_VALUE;
	}
	
	CleanupNetPack();

#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)
	/// 감마값 돌려놓기 by sooree
	SetDeviceGammaRamp( ::GetDC( NULL ) , g_ramp );
#endif
	
	OleUninitialize();

#ifdef C_HACK_SHIELD
	g_RockClient.HS_StopService();
	g_RockClient.HS_UnInit();
#endif

	All_Delete_g_Variable_Ptr();

	return ret;

#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)
	
RET_FALSE:
	g_VFInterface.CloseFileSystem();
	
#ifdef C_HACK_SHIELD
	g_RockClient.HS_StopService();
	g_RockClient.HS_UnInit();
#endif
	
	RTCHAR ErrorMsg[256]= {0,};
	Rsprintf( RWCHAR(ErrorMsg) , _RT(" %s ErrorCode =%d") ,G_STRING_CHAR(IDS_WND_RUN_DECO)  , ErrorCode );
	RMessageBox( NULL, RWCHAR(ErrorMsg)  , G_STRING_CHAR(IDS_RockSoft), MB_ICONSTOP );
	
	VERIFY( ROCKBASE_SHUTDOWN() );

	if( g_hClientMutex != INVALID_HANDLE_VALUE )
	{
		CloseHandle( g_hClientMutex );
		g_hClientMutex = INVALID_HANDLE_VALUE;
	}
    	
	CleanupNetPack();

	All_Delete_g_Variable_Ptr();	

	return -1;

#endif
  
}


#endif// #ifndef WIN_MAIN_CPP