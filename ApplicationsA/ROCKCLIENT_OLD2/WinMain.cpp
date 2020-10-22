//----------------------------------------------------------------------------------------------------------------------
// ��  ��   �� : WinMain.cpp
// ��       �� : �������� ���α׷��� �������� ������ main�� �ƴ� WinMain�̴�. ���� ��� �������� ���α׷��� WinMain
//               �������� ������ ����
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
int   g_nOverBright = 0;	/// �̰� �� �ǹ̰� ����...


// Global Function
void gOnCrash()
{
	g_RockClient.OnCrash();
}

// ��ó ������Ʈ(��ó�� ���� Ŭ���̾�Ʈ�� ������Ʈ �ϰ�, Ŭ���̾�Ʈ�� ��ó�� ������Ʈ�Ѵ�)
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

// ��ũ���� ���� Ȯ��
bool MakeScreenShotFolder()
{
	TCHAR file_path[MAX_PATH]	= {0,};
	wsprintf( file_path, "%s\\ScreenShot", g_RockClient.GetRootDir() );
	DWORD dAttr =  ::GetFileAttributes(file_path);// ���� ���� ����
	
	if( FILE_ATTRIBUTE_DIRECTORY == dAttr )	
		return true;

	//�ش� ������ �������� ������...
	if(!::CreateDirectory(file_path, NULL))	 //���丮�� �����Ѵ�.
	{
		// ������ �߻��� ������ �ִ� ��� API �Լ����� ���ϰ����� ���� ������ ������ �ش�.
		// �׷��� �Լ��� ���ϰ����δ� ���� �߻� ��Ǹ� �˷��� �� ��ü������ � �������� �� �� ����.
		DWORD dErr = ::GetLastError();// �߻��� ������ ���� Ȯ��
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


// Ŭ���̾�Ʈ �� �αױ�� �� ������ �����. - ������ ����� ���� Ȯ�ο�
bool MakeClientLogFolder()
{
	TCHAR file_path[MAX_PATH]	= {0,};
	wsprintf( file_path, "%s\\ClientLog", g_RockClient.GetRootDir() );
	DWORD dAttr =  ::GetFileAttributes(file_path);
	
	if( FILE_ATTRIBUTE_DIRECTORY == dAttr )
		return true;

	//�ش� ������ �������� ������...
	if(!::CreateDirectory(file_path, NULL))	 //���丮�� �����Ѵ�.
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

	//�޼����ڽ� ������ string.dat �ε� �Ѵ�..
	if(!g_RockClient.InitLanguege())
	{
		RMessageBox(NULL, _RT("String Error") , _RT("Error") ,  MB_ICONINFORMATION );
		g_VFInterface.CloseFileSystem();
		return -1;
	}	
	
	// ��ó ������Ʈ
	if(UpdateLauncher())
		return -1;
	


	///-- �Ʒ� �ڵ�� ���ؽ� �κ��� Ŭ���̾�Ʈ�� ���ÿ� ����Ǵ°� �����ϱ� ���� �ڵ��̴�.
	///-- ���� ������ �߻��Ѵٸ�.. Ŭ���̾�Ʈ �ΰ������ϴ� �ٸ� ����� ���� �ϴ��� �ǽ��غ��� �ҵ� �ϴ�.

#ifdef CHINA_VERSION
	hwnd =  FindWindow( NULL, _T( "LnD Online" ) );
#elif defined(HONGKONG_VERSION)
	hwnd =  NULL;//FindWindowW( NULL, _RT( "SeiyaOnline" ) );
#elif defined(KOREA_VERSION) 
	hwnd =  FindWindow( NULL, _T( "�ð�����" ) );
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
	
	// ��ũ���� ���� Ȯ��
	if(!MakeScreenShotFolder())
		return -1;

	// Ŭ���̾�Ʈ �� �αױ�� �� ������ �����.
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
	//by simwoosung - ����� ��� - ���α׷� ��������ÿ��� 
	//�޸� ������ �����â�� ����Ѵ�.
	//Shift-F5�δ� �޸� ���� ��� ��������
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	
	
	VERIFY( ROCKBASE_INITIALIZE() );
	R_RELEASE_DFMF();
	RLG_SET_OPTS( RLG_OPT_ODS );	/// n_Default = n_ODS | n_Manager ��. Log Manager�� �α� �ȳ��⵵��...


#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)
	RLG_DISABLE();
#endif

	
	SetExceptionHandler( gOnCrash );	/// �ڵ� ���� ����Ʈ 

	CreateNetPack();


	/*if(0)
	 goto RET_FALSE;*/

	///////////////////////////////////////////////////////////////
	/// release ��忡�� üũ : �ݵ�� ���ĸ� ���� ����Ǿ�� �մϴ�. 

	int ErrorCode = -1 ;
	
	
#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)

	//by simwoosung ���� 4���� �޴� �κ�....
	char strParam1[128] = {0,};
	char strParam2[128] = {0,};
	char strParam3[128] = {0,};
	char strParam4[128] = {0,};
	int nCode = 0;

	// �����̽��� �и��� ������ ���ڸ� ���� �޾Ƴ���.
	sscanf(pStr, "%s %s %s %s", strParam1, strParam2, strParam3, strParam4);	
	
	char strParam[128];
	char binParam[12]; 
	int VersionLen = 0; 

	DWORD	gameVersion;
	int TestVersion =0 ;

	VersionLen = strlen( strParam1 );

	//���� ���� üũ
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
			goto RET_FALSE;	/// ��ӵ� �Ķ���Ͱ� ��ġ���� ���� 
		}
		/// var1���� 80 = 8��. ���� 8�� �̳����� ��ȿ�� ���� 
	/*
		if( ml_language	!= eLang_Korea)
		{
			check = ( timeGetTime() / 100 ) % ( 1 << 16 ); 
			if( check > var1 + 200 )	
			{
				goto RET_FALSE; /// ���� �ð� üũ -> �ȸ´�! 
			}
		}
			*/			
		gameVersion = ver_major * 1000 + ver_minor * 10 + ver_bugfix;
   
// ���� 3���� ���� ������ ���ϴ�.
// ������ �ּ�ó�� �� 
		////////////////////////////////////////////////////////////////
		/// ���� ���� ���񽺿����� �ݵ�� üũ�ؾ� �մϴ�. : ���� �������� ���� �ϴ� �򰡸��� 
		// gameVersion ���Ŀ��� ���� ���°� setup.ini >  g_This_Client_Version Ŭ���̾�Ʈ ������ ����
		// Ŭ���̾�Ʈ ���� ���� �����κ��� ������ üũ�մϴ�.
		

	//	if( gameVersion > g_This_Client_Version )
	//	{
	//		char str[128];
	///		wsprintf( str, "Version error! This client version is %d.%02d.%d", g_This_Client_Version / 10000, ( g_This_Client_Version % 1000 ) / 10, g_This_Client_Version % 10 );
	//		MessageBox( NULL, str, "Deco", MB_ICONSTOP );
	///		goto RET_FALSE;
	//	}
	}
	///////////////////////////////////////////////////////////////
	//���� �κп� ����Ī ���� ó��(�ݸ���, ���ӿ���...)
	
	// 2��° �Ķ���Ͱ� NFrom ��
#ifdef CHINA_VERSION
	//�߱��� ���������� �Ѿ� �´�. 
	nCode = atoi(strParam2);
	g_RockClient.SetAreaInfoNum(nCode);

	//strParam3 IP strParam4 Port����



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
				strcpy(g_RockClient.m_GameTypeInfo.theUserID, strParam3);	//���� ������ 
				strcpy(g_RockClient.m_GameTypeInfo.theUserPass, strParam4);	//���� ������ 
			}
			else
			{
				g_RockClient.m_GameTypeInfo.theFrom = ( NFrom )nCode;
			}
		}
		break;
	}

#endif	//CHINA_VERSION

	//���Ĺ����� 
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


	//���� ����â ���� 
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
	

#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)	/// ���� ���� : ������ ������ by sooree
	/// ���� ������ ��� 
	GetDeviceGammaRamp( ::GetDC( NULL ), g_ramp );
	/// �ɼǿ� ���� ���� 
	Drv_SetGamma( ::GetDC( NULL ), g_fGamma, g_nOverBright, g_fContrast );

#endif
	
	// ���� ���� ����
	ret = g_RockClient.Run();

	VERIFY( ROCKBASE_SHUTDOWN() );
	
	if( g_hClientMutex != INVALID_HANDLE_VALUE )
	{
		CloseHandle( g_hClientMutex );
		g_hClientMutex = INVALID_HANDLE_VALUE;
	}
	
	CleanupNetPack();

#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)
	/// ������ �������� by sooree
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