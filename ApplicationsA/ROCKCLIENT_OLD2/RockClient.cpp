// 키보드 후킹 함수 쓰기 위해서...버젼 에러가 나서 ^ ^;
#ifndef _WIN32_WINNT
#define _WIN32_WINNT  0x0501      // WIN2K 500, XP 501 
#endif /* !_WIN32_WINNT */

#include "RockPCH.h"
#include "ole2.h"
#include	<NETWORK\\Protocol.h>

#include	<Base\\D3DBase.h>
#include	"quadlist.h"
#include	<Obj\\Object.h>
#include	<Map\\Field.h>
#include	"RockClient.h"
///--#define TEST_REALEASE_CLIENT 
            
#ifdef JAPAN_HPZERO_BUG_MODIFY
	bool g_bFindRHpZeroAliveMob = false;
#endif
	        
                 
#ifdef 	C_HACK_SHIELD	
	#include	"HShield.h"
	#pragma comment( lib, "HShield.lib" )
#endif

	#pragma comment(lib , "NPGameLib_65_MD.lib")// 게임가드 라이브러리

#include	"Camera.h"
#include	"MObject.h"
#include	"CharItem.h"
#include	"Map.h"
#include	"Pc.h"
#include	"Npc.h"
#include	<SOUND\\Sound.H>
#include	<Map\\Grass.h>
#include	<Map\\GTexture.h>
#include	<EFFECT\\Particle.h>
#include	<EFFECT\\EffectInsect.h> 
#include	"Effect.h"
#include	<Obj\\TextureManager.H>
#include	"global.h"
#include	"VFileSystem.h"	/// sooree pack 

#include	"profile\profile.h"
#include	"profile\custom_time.h"

#include    "RockInterface\\LoadingWndProc.h"
#include    "RockInterface\\OptionWndProc.h"

#include	"RockInterface\\MessageBox.h"
#include	"RockInterface\\MessageWndProc.h"
#include	"StringManager.h"
#include	"MapDataManager.h"
#ifdef UI_CONTROL_RENEWAL
#include	"UIDataManager.h"
#endif // UI_CONTROL_RENEWAL

#include	"RockInterface\\MessageWndProc.h"
#include    "CRenderManager.h"
#include	"bravolibs\\obj\\EffItemManager.h"
#include	"CLoadingBarThread.h"
#include    "CLogoDisplayThread.h"

#include    "RockInterface\\CUserStoreTitleWnd.h"
#include    "RockInterface\\CUserStoreSetWnd.h"
#include    "RockInterface\\CUserStoreSellWnd.h"
#include    "RockInterface\\CUserStoreBuyWnd.h"
#include    "RockInterface\\CUserStoreSetBuyWnd.h"
#include	"RockInterface\\InventoryWndProc.h"
#include    "RockInterface\\CWebShopWnd.h"
#include    "RockInterface\\EmblemTexManager.h"
#include	"RockInterface\\StoreWndProc.h"
#include	"RockInterface\\CUserStoreSellWnd.h"
#include	"RockInterface\\CUserStoreTitleWnd.h"
#include    "md5.h"
#include     <tlhelp32.h>
#include	 <shellapi.h>
#include	"RockInterface\\SystemMenuWndProc.h"
#include	"RockInterface\\ServerListWndProc.h"

#pragma warning(disable:4098)

bool g_bWinThread = false; 


// 봉인의 성지 보스몹 테스트

  

void WinThread(void *Chk)
{
	while(g_bWinThread)
		g_RockClient.Main();
}	

         
/// sooree pack 
CVFileSystem		g_VFChrRoot; 
CVFileSystem		g_VFChrItm;		/// 빈번한 ChangeDir()은 오버헤드가 심하다. 
CVFileSystem		g_VFChrMsh;		/// Npc는 로딩 구조상...아예 자주 쓰는 디렉토리별로 파일 시스템을 별도로 Open 하자. 
CVFileSystem		g_VFChrTex;
CVFileSystem		g_VFChrAni;

CVFileSystem		g_VFNpcRoot; 
CVFileSystem		g_VFNpcItm;		/// 빈번한 ChangeDir()은 오버헤드가 심하다. 
CVFileSystem		g_VFNpcMsh;		/// Npc는 로딩 구조상...아예 자주 쓰는 디렉토리별로 파일 시스템을 별도로 Open 하자. 
CVFileSystem		g_VFNpcTex;
CVFileSystem		g_VFNpcAni;
   
CVFileSystem		g_VFEffectRoot;
CVFileSystem		g_VFEffectData;
CVFileSystem		g_VFEffectStatic;
CVFileSystem		g_VFEffectTex;
CVFileSystem		g_VFEffectTexAni;

CVFileSystem		g_VFEnv;
CVFileSystem		g_VFEtc;
CVFileSystem		g_VFInterface;
CVFileSystem		g_VFSound;
CVFileSystem		g_VFMap;

CLogoDisplayThread g_CLogoDisplayThread;

extern CSpellEffectBaseData	g_SpellEffectBaseData;
#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

#include "GTimer.h"
   
#include "RockInterface\\InputDXDevice.h"
    
#include "RockInterface\\MiniMapWndProc.h"
#include "RockInterface\\MainMenuWndProc.h"
#include "RockInterface\\SkillWndProc.h"
#include "RockInterface\\SkillRegistWnd.h"
#include "RockInterface\\SkillOverWriteWnd.h"
#include "RockInterface\\CharSelectWndProc.h"
#include "RockInterface\\NonPlayerInfoWnd.h"
#include "RockInterface\\NumberInputWnd.h"
#include "RockInterface\\LoadingWndProc.h"
#include "RockInterface\\Rui.h"
#include "RockInterface\\CBillBoardUIRender.h"
#include "RockInterface\\PartyInforWnd.h"
#include "RockInterface\\CLogWnd.h"
#include "RockInterface\\UiUtil.h"
#include "MapDataManager.h"
#include "CItemTexLoadThread.h"
#include "CNetProcThread.h"
#include "..\\bravolibs\\obj\\texturemanager.h"

extern int g_Max_Effect_Display;
         
///////////////////////////////////////////////
/// realease에서는 버전 체크 해야합니다. 
 
#define		KOREA_GAME_VERSION			10144
#define		JAPAN_GAME_VERSION			10145
#define		JAPAN_GAME_VERSION_TEST		10000	
#define		INDONESIA_GAME_VERSION		10045
//Old #define		GLOBAL_GAME_VERSION		10010
#define		GLOBAL_GAME_VERSION		983
#define		CHINA_GAME_VERSION		10001
///-- #define		CHINA_GAME_VERSION		906
#define		TAIWAN_GAME_VERSION		10001
#define		HONGKONG_GAME_VERSION	970
   
    
//////////////////////////////////////////////////////////////////////////
// 런쳐버젼 비교한다.
//////////////////////////////////////////////////////////////////////////
#ifdef LAUNCHER_UPGREADE_VERSION
	#ifdef TEST_INSTALL_VERSION
		#define		LAUNCHER_VERSION	100	
	#else 
		#define		LAUNCHER_VERSION	900
	#endif
#endif               

int g_This_Client_Version = -( KOREA_GAME_VERSION + JAPAN_GAME_VERSION + INDONESIA_GAME_VERSION 
							   + GLOBAL_GAME_VERSION + CHINA_GAME_VERSION + TAIWAN_GAME_VERSION + HONGKONG_GAME_VERSION  );            
   
   
#define KEY_DISABLETASKMGR  "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
#define VAL_DISABLETASKMGR  "DisableTaskMgr"
          
///////////////////////////////////////////////
      
extern HANDLE	g_hTextrueUpdateThread;
extern unsigned int __stdcall TextrueUpdateThread( LPVOID pVoid );

HANDLE	g_hMousePosUpdateThread;
unsigned int __stdcall MousePosUpdateThread( LPVOID pVoid );

extern TEXTURE_MANAGER		g_TexManager;
#ifdef DIRECT_VERSION_9_MJH
extern LPDIRECT3DTEXTURE9	g_pCurTextureSky;
#else
extern LPDIRECT3DTEXTURE8	g_pCurTextureSky;
#endif // DIRECT_VERSION_9_MJH

extern HANDLE	g_hLoadingThread;
unsigned int __stdcall LoadingBarThread( void* arg );

/// WinMain() 보다 먼저 호출---------------------------------------------------
CRockClient			g_RockClient;
///----------------------------------------------------------------------------

BOOL	g_HoldGame = FALSE;
  
CBravoCamera		g_Camera;
CBravoObject		g_Object;
CBravoItem			g_Item;
CBravoMap			g_Map;
CBravoPC			g_Pc;
CGrass				g_Grass;
CGTexture			g_GTexture;
#ifdef DIRECT_VERSION_9_MJH
IDirect3DDevice9*	g_lpDevice;
#else
IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
CBravoWaterfallTexture	g_WaterfallTexture;
Texture				g_LogoTex;
Texture				g_EffTex1;
Texture				g_EffTex2;
Texture				g_EffTex3;
Texture				g_LoadingTex;
Sound_Manager		g_Sound;
Object_Manager		g_Obj_Manager;
EtcTable			g_EtcTable;
CGrassData 			g_GrassList[1024];
CInsect_Manager		g_Insect;
CBravoParticle		g_Particle;
CBillboardManager	g_Billboard;
CParticleSystem		g_ParticleSystem;
CAnimManager		g_Anim;
   
//-----------------------------------------------------------------------------
STICKYKEYS g_StartupStickyKeys = {sizeof(STICKYKEYS), 0};
TOGGLEKEYS g_StartupToggleKeys = {sizeof(TOGGLEKEYS), 0};
FILTERKEYS g_StartupFilterKeys = {sizeof(FILTERKEYS), 0};   

//-----------------------------------------------------------------------------

///-- Minimap TID table
//{
//	0,		0,		459,	460,	461,		// 0 ~ 4 
//	462,	463,	464,	465,	466,		// 5 ~ 9 
//	467,	468,	469,	895,	896,		// 10 ~ 14 
//	897,	897,	897,	897,	897,		// 15 ~ 19 
//	897,	897,	897							// 20 ~ 21 
//};

    
void		SetupMaxMin(D3DXVECTOR3* max, D3DXVECTOR3* min);

SPacketInfoPerSec	g_SPacketInfoPerSec;

bool			g_bShowWideMinimap	= false;
bool			g_bShowUserName		= false;

int				g_Shadow;	// 0 그림자 없음, 1 동그란그림자, 2실시간그림자
int				g_GrsVisible;
int				g_LODRange;
int				g_ViewLODTerrain;
BOOL			g_ISObjTexLoad = FALSE;		// 맵 오브젝트 텍스처 로딩 여부
BOOL			g_ISLandEffect = FALSE;		// 지형 이펙트 표시 여부
int				g_Version;			///-- Client 의 버전
BOOL			g_LogPrint = FALSE;

long			g_nowTime = timeGetTime();
DWORD			g_Last_Memory = 0;
bool			g_ISWeatherVisible = TRUE;
bool			g_IsHelpView = true;
bool			g_ISCharShowIDVisible = FALSE;
bool			g_IsFrameOptimize = false;
bool			g_IsEffectMinimize = false;
bool			g_IsSkEffSys = true;

int				g_nTextureDetail = 0;
int				g_IsEWindowsMode = 0;


int theHotKeyID_ALT_TAB  = 0;

HHOOK		g_hHook = NULL;		// hook handle

char		strSendMesseage[512] = "";


//모든창 최소화 
#ifdef	GM_CLIENT
	bool g_AllWnd = false;
#else	
	bool g_AllWnd = true;	
#endif

//define/////////////////////////////////////////
const int HACKSHIELD_MSG = 160;     

     
     
extern	int	GetMax( int a, int b );

#ifdef HHM_1600X1200_TEST// Radeon 9550 해상도 1600 X 1200 창모드에서 속도 저하(2009/05/15)
	
	bool g_1600x1200 = true;

#endif// #define HHM_1600X1200_TEST// Radeon 9550 해상도 1600 X 1200 창모드에서 속도 저하(2009/05/15)


/// sooree network
#ifndef	NETWORK_CLIENT
unsigned int __stdcall TextrueUpdateThread( LPVOID pVoid );
extern	TEXTURE_MANAGER		g_TexManager;
#endif
 
int CALLBACK AhnHS_Callback(long lCode, long lParamSize, void* pParam)
{
     
#ifdef 	C_HACK_SHIELD		
	
	TCHAR szMsg[512] = {0,};

	switch(lCode)
	{
		//Engine Callback
		case AHNHS_ENGINE_DETECT_GAME_HACK:
		{

#ifdef UNICODE_ROCK
			WCHAR wMsg[512] = {0,};
			DXUtil_ConvertAnsiStringToWide(wMsg , (TCHAR*)pParam , -1 );
#else
			char *wMsg = (char *)pParam;
#endif

			Rsprintf( RWCHAR(szMsg) , G_STRING_CHAR(HACK_DETECT_GAME_HACK), RWCHAR(wMsg) );
			g_RockClient.HackClose(szMsg ,CsDETECT_HACK_PROGRAM );
			break;
		}

		//일부 API가 이미 후킹되어 있는 상태
		//그러나 실제로는 이를 차단하고 있기 때문에 다른 후킹시도 프로그램은 동작하지 않습니다.
		//이 Callback은 단지 경고 내지는 정보제공 차원에서 제공되므로 게임을 종료할 필요가 없습니다.
		case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:
		{
			PACTAPCPARAM_DETECT_HOOKFUNCTION pHookFunction = (PACTAPCPARAM_DETECT_HOOKFUNCTION)pParam;
		
			sprintf( szMsg , "[HACKSHIELD] Already Hooked\n- szFunctionName : %s\n- szModuleName : %s\n", 
				pHookFunction->szFunctionName, pHookFunction->szModuleName);
			
			break;
		}

		//Speed 관련
		case AHNHS_ACTAPC_DETECT_SPEEDHACK:
//		case AHNHS_ACTAPC_DETECT_SPEEDHACK_APP:
		{
			Rsprintf( RWCHAR(szMsg), G_STRING_CHAR(HACK_DETECT_SPEEDHACK));
			g_RockClient.HackClose(szMsg ,CsDETECT_SPEEDHACK);
			break;
		}

		//디버깅 방지 
		case AHNHS_ACTAPC_DETECT_KDTRACE:	
		case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
		{
		
			Rsprintf( RWCHAR(szMsg), G_STRING_CHAR(HACK_DETECT_KDTRACE), lCode);
			g_RockClient.HackClose(szMsg, CsDETECT_KDTRACE);
			break;
		}
		
		case AHNHS_ACTAPC_DETECT_AUTOMACRO:
		{
			Rsprintf( RWCHAR(szMsg), G_STRING_CHAR(HACK_DETECT_AUTOMACRO), lCode);
			g_RockClient.HackClose(szMsg);
			break;
		}
  
		// 코드 패치 감지
		case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:
		{

#ifdef CHINA_VERSION
#else

#ifdef UNICODE_ROCK
			WCHAR wMsg[512] = {0,};
			DXUtil_ConvertAnsiStringToWide(wMsg , (char*)pParam);
#else
			char *wMsg = (char *)pParam;
#endif

			Rsprintf( RWCHAR(szMsg),G_STRING_CHAR(HACK_DETECT_ABNORMAL_MEMORY_ACCESS), wMsg );
			g_RockClient.HackClose(szMsg ,CsDETECT_MEMRORY_ACCESS );
#endif
			break;
		}
		//그외 해킹 방지 기능 이상 
		case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
		case AHNHS_ACTAPC_DETECT_DRIVERFAILED:		
		case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:
		case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
		case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
		{		
			Rsprintf( RWCHAR(szMsg), G_STRING_CHAR(HACK_DETECT_ABNORMAL), lCode);
			g_RockClient.HackClose(szMsg, CsDETECT_ETC );
			break;
		}
	}

#endif

	return 1;
}

/// nProtect callback 
//C_GAME_GURAD


BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
#ifdef C_GAME_GUARD
	RTCHAR g_szHackMsg[512] ={0,};

	bool bAppExit = FALSE ; 
    
	switch (dwMsg)
	{
	case NPGAMEMON_COMM_ERROR:
	case NPGAMEMON_COMM_CLOSE:
		bAppExit = true; 
		break;
	case NPGAMEMON_INIT_ERROR:
		Rwsprintf(g_szHackMsg, G_STRING_CHAR(IDS_MSG_NPGAME_INIT_ERROR ) , dwArg);
		bAppExit = true; 
		break;
	case NPGAMEMON_SPEEDHACK:
		Rwsprintf(g_szHackMsg, G_STRING_CHAR(IDS_MSG_NPGAME_SPEEDHACK) );
		bAppExit = true; 
		break;
	case NPGAMEMON_GAMEHACK_KILLED:
		Rwsprintf(g_szHackMsg, G_STRING_CHAR(IDS_MSG_NPGAME_GAMEHACK_KILLED), g_RockClient.pNpgl->GetInfo());
		bAppExit = true;
		break;
	case NPGAMEMON_GAMEHACK_DETECT:
		Rwsprintf(g_szHackMsg, G_STRING_CHAR(IDS_MSG_NPGAME_GAMEHACK_DETECT), g_RockClient.pNpgl->GetInfo());
		bAppExit = true;
		break;
	case NPGAMEMON_GAMEHACK_DOUBT:
		Rwsprintf(g_szHackMsg, G_STRING_CHAR(IDS_MSG_NPGAME_GAMEHACK_DOUBT) );
		break;
	case NPGAMEMON_GAMEHACK_REPORT:
		{
			DWORD dwHackInfoSize = 0;
			LPBYTE pHackInfo = NULL;
			pHackInfo = GetHackInfoFromGameMon(&dwHackInfoSize);
			// pHackInfo = npgl.GetHackInfo(&dwHackInfoSize); // C++ 일 경우.
			if (pHackInfo && dwHackInfoSize > 0)
			{
				// 아래 함수는 게임가드에서 제공하는 함수가 아닙니다.
				//ClientFilelog("GAMEHACK_REPORT Send Server string size %s " , dwHackInfoSize );
				g_RockClient.Send_CsGameGuard( 1 , pHackInfo , dwHackInfoSize); // 서버로 데이터 전송.
			}
		}
		return true;
	case NPGAMEMON_CHECK_CSAUTH2:

		memcpy(&g_RockClient.m_AuthData, (PVOID)dwArg, sizeof(GG_AUTH_DATA));
		// 서버로 인증 패킷을 전송. 이때 Callback 함수는 게임가드 프로세서가 호출하므로 동기화가 필수
		g_RockClient.Send_CsGameGuard( n_GG_Normal , (PBYTE)&g_RockClient.m_AuthData , sizeof(GG_AUTH_DATA) );
		return true;
  



	}

	if(bAppExit)
	{
		RMessageBox(NULL , g_szHackMsg , _RT("nProtect GameGuard") , MB_OK );
		ClientClose( g_RockClient.GetApp() , 1);
		
	}
#endif 
	return true; // 계속 진행
}



BOOL ThreadClosed()
{
	if( STILL_ACTIVE == g_CLogoDisplayThread.getExitCode() )
	{
		g_CLogoDisplayThread.Stop();	
	}

	if( STILL_ACTIVE == g_CLoadingBarThread.getExitCode() )
	{
		g_CLoadingBarThread.Stop();	
	}

	if( g_pCNetProcThread && ( STILL_ACTIVE == g_pCNetProcThread->getExitCode() ) )
	{
		g_pCNetProcThread->Stop();
	}
	
	g_Sound.m_BackSound.Stop();

	Sleep( 100 );

	return TRUE;

}

float	FtoDW(float f)	
{ 
	return *((DWORD*)&f); 
}

/// sooree network
BOOL	g_RunThreadFlag = FALSE;
unsigned int __stdcall TempDisplayThread( LPVOID pVoid )
{
	while( g_RunThreadFlag )
	{
		g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET, g_RockClient.m_SceneManager.GetBGColor() , 1.0f, 0 );
		if( SUCCEEDED( g_RockClient.BeginScene() ) )
		{
			g_RockClient.LogoDisplay( 255 );

			g_RockClient.EndScene();
		}
		g_RockClient.Present( NULL, NULL, NULL, NULL );

		Sleep( 1 );
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////
// 감마 조절 by sooree

///#define	OTP_GAMMA_ENABLE	/// 감마 조절 옵션 기능 넣을 경우 



float g_fGamma = 0.0f;
float g_fContrast;
//모든 창 최소화 

CRITICAL_SECTION	g_csLoadingFlag;

//#import "shell32.dll" raw_interfaces_only
//using namespace Shell32;

void CloseAllWnd()
{
	if( g_AllWnd == false)
		return;

#ifdef C_HACK_SHIELD
	g_RockClient.HS_PauseService();
#endif
	keybd_event(VK_LWIN, OemKeyScan(VK_LWIN), KEYEVENTF_EXTENDEDKEY, 0);
	keybd_event(VkKeyScan('D'), OemKeyScan('D'), 0, 0);
	keybd_event(VkKeyScan('D'), OemKeyScan('D'), KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LWIN, OemKeyScan(VK_LWIN), KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0);

	Sleep(2000);

#ifdef C_HACK_SHIELD
	g_RockClient.HS_ResumeService();
#endif
}

void Drv_SetGamma(HDC hDC, float gamma, int overbright, float contrast)
{    
	WORD ramp[256*3];
	float f = 0.0f;
	
	overbright= 10;
	contrast = 1.0f;
	
	for( int i = 0; i < 256; i++ )
	{
		f = (float)(255 * pow((float)i/256, 1/gamma));		
		f = f * contrast + overbright;
		if (f < 0) f = 0;
		if (f > 255) f = 255;
		ramp[i+0] = ramp[i+256] = ramp[i+512] = (WORD)min(65535, ((unsigned short)f)<<8);
	}
	   
	SetDeviceGammaRamp( ::GetDC(NULL), ramp );
}


int SortNpcIndexList( const void *arg1, const void *arg2 )
{
	sNpcIndexlist* a1 = ( sNpcIndexlist* ) arg1;
	sNpcIndexlist* a2 = ( sNpcIndexlist* ) arg2;
	
	// 0 same, > 0 arg1 win, < 0 arg2 win
	if( a1->Distance == a2->Distance )
	{
		return 0;
	}
	else if( a1->Distance > a2->Distance )
	{
		return 1;
	}
	else
	{
		return -1;
	}

	
	return 0;
}
#ifdef ADD_APPLYWIDE_MAP 
struct SortNpcIndexKeyList: public binary_function<sNpcIndexlist, sNpcIndexlist, bool> 
{ 
       bool operator()(const sNpcIndexlist &Key0,const sNpcIndexlist &Key1) const 
       { 
		   return ( Key0.Distance > Key1.Distance );
       } 
};
#endif


///////////////////////////////////////////////////////////////////////////////////
// 자동 버그 리포트 by sooree

#define	n_MaxBugLogSize	4096
#define	n_RpCsBugReport	0xffee
#define	n_RpCsDeBugReport 0xffef

#pragma pack(1)
typedef struct _SRpCsBugReport
{
	_SRpCsBugReport()
	{
		theSize = sizeof( _SRpCsBugReport );
		theProtocol = n_RpCsBugReport;
		ZeroMemory( theAccountName, sizeof( theAccountName ) );
		ZeroMemory( thePCName, sizeof( thePCName ) );
		ZeroMemory( &thePCSystemInfo, sizeof( thePCSystemInfo ) );
		theLogSize = 0;
		ZeroMemory( theLog, sizeof( theLog ) );
	}
	WORD			theSize;
	WORD			theProtocol;
	char			theAccountName[32];
	char			thePCName[32];
	PCSystemInfo	thePCSystemInfo;
	WORD			theLogSize;
	BYTE			theLog[n_MaxBugLogSize];
}SRpCsBugReport, * SRpCsBugReportPtr;
#pragma pack()

#pragma pack(1)
typedef struct _SRpCsDeBugReport
{
	_SRpCsDeBugReport()
	{
		theSize = sizeof( _SRpCsDeBugReport );
		theProtocol = n_RpCsDeBugReport;
		ZeroMemory( theAccountName, sizeof( theAccountName ) );
		ZeroMemory( thePCName, sizeof( thePCName ) );
		ZeroMemory( &thePCSystemInfo, sizeof( thePCSystemInfo ) );
		theLogSize = 0;
		ZeroMemory( theLog, sizeof( theLog ) );
	}
	WORD			theSize;
	WORD			theProtocol;
	char			theAccountName[32];
	char			thePCName[32];
	PCSystemInfo	thePCSystemInfo;
	WORD			theLogSize;
	BYTE			theLog[n_MaxBugLogSize];
}SRpCsDeBugReport, * SRpCsDeBugReportPtr;
#pragma pack()



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void CRockClient::DeleteEmblemFile()
{
	//Load emb File List 

	int Option = 0 ; 
	Option = GetPrivateProfileInt( "Client", "emblem", 0, "./setup.ini" );
	if(Option == 1 )
	{
		return;
	}
	
	const DWORD NOT_FILE = 0xFFFFFFFF;

	//디렉토리 생성
	TCHAR file_path[260]	= {0,};
	wsprintf( file_path, "%s\\data\\emb", g_RockClient.GetRootDir() );
	
	TCHAR setupFile[MAX_PATH];
	wsprintf( setupFile, ".\\%s", "setup.ini" );

	int Num[13]={ 8,10,11,13,14,15,17,19,23,29,36,38 };
	TCHAR FileName[256]= "";
	TCHAR StrTmp[16]="";
   
	//Create FileName
	for( int i =0 ; i < 13 ; ++i)
	{
		if(Num[i] == 0 )continue;

		wsprintf(FileName , "%s\\%d.emb" , file_path , Num[i]  );
		DWORD dAttr =  ::GetFileAttributes(FileName);
		if( NOT_FILE != dAttr )	//해당 파일이 있으면 .. 
		{
			unlink( FileName);
		}
	}
	
	wsprintf( StrTmp , "%d", 1 );
	WritePrivateProfileString( "Client", "Emblem", StrTmp , setupFile );

	return;

}

//    
LRESULT CRockClient::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static TCHAR str[40];
	static int count1( 0 );

	switch( uMsg )
    {
		case WM_SIZE:
			{
				switch(wParam)
				{
					case SIZE_MINIMIZED:
					{
					//	g_bWinThread = true; 
					//	_beginthread(WinThread , 0 , NULL);
						g_RockClient.Send_CsOnInactivated();
					}
					break;
					case SIZE_MAXIMIZED:
					{
					//	g_bWinThread = false; 
					//	_endthread();
					}
					break; 
					case SIZE_RESTORED:
					{
						g_RockClient.Send_CsOnRestoreOK();
					}
					break;
				}
			}
			break;	
		case WM_CREATE:
			{
			///-- PacketTranslog

#ifdef UNICODE_ROCK
//			nRui->LanguageChange(hWnd , wParam , lParam);
			count1 =0 ;
#endif 


#ifdef	PACKETLOG_CLIENT
			thePacketTransLogDlg.CreateDlg( m_hInstance, hWnd );
			thePacketTransLogDlg.Close();
#endif
			}
			break;

			/*
#ifdef	GM_CLIENT
	case WM_NCHITTEST:	/// GM Client에선 윈도우 위치 고정시키기 위해 
		{
			LRESULT ret = DefWindowProc( hWnd, uMsg, wParam, lParam );
			if( ret == HTCAPTION ) ret = HTCLIENT;
			return ret;
		}
#endif
		*/

		case WM_SETCURSOR:            
            {
		       if(nRui && nRui->m_IsSurpportHCursor)
			   {
				   if(m_IsDeviceCursor && m_pd3dDevice)
				   {
					   if( g_RockClient.m_SceneManager.GetCursorDisplay() )
					   {
						   g_RenderManager.ShowCursor( TRUE );
						   m_IsDeviceCursor = TRUE;
					   }
				   }
				   
				   if(!m_IsDeviceCursor && m_pd3dDevice)
				   {
					   if( g_RockClient.m_SceneManager.GetCursorDisplay() )
					   {
						   g_RenderManager.ShowCursor( TRUE );			
						   m_IsDeviceCursor = TRUE;
					   }
				   }
			   }		
			  
			  return TRUE;
			}
			break;
		case WM_SETFOCUS:
			if( nRui )
			{
				SetFocus( nRui->GetCurrentWndHandle());
				//sprintf( str, "WM_SETFOCUS : %d\n", count1++ );
				//OutputDebugString( str );
				//nRui->SendEvent( WID_DeskTop, n_emTextOutput, (EPARAM)str, 20, 140, D3DCOLOR_XRGB( 160, 200, 220 ) );
			}			
			if(nRui)
			{
				if(nRui->m_IsSurpportHCursor && !m_IsCursor)
				{
                   if( g_RockClient.m_SceneManager.GetCursorDisplay() )
				   {
						ShowCursor( TRUE );
						m_IsCursor = TRUE;
				   }
				}
			}
#ifdef CPU_SHARE_RATE
			m_bFocus = TRUE;
#endif	//	CPU_SHARE_RATE
			break;
			
		case WM_ACTIVATE:
			{
			///-- 비활성화 될 경우 바로 스핵 체크 방지 요청				
				if( ( LOWORD(wParam) == WA_INACTIVE && HIWORD(wParam) ) ||
					( LOWORD(wParam) == WA_INACTIVE && 
					 ( m_bFullScreen && m_EWindowsMode == MODE_NORMAL_FULLMODE ) ) )
				{
					//if( GetLanguage() != eLang_Korea )
					{
						if( !thePacketTransLogDlg.IsActivePDlg() )
						{
							Send_CsOnInactivated();						
							RLGS("---  Disable SHack Chk INACTIVATE" );
						}
					}	
				}
#ifdef CPU_SHARE_RATE
				else if( ( LOWORD(wParam) == WA_INACTIVE && HIWORD(wParam) ) ||
						( LOWORD(wParam) == WA_INACTIVE && 
						 ( !m_bFullScreen ) ) )
				{
					m_bFocus = FALSE;
				}
#endif	//	CPU_SHARE_RATE
				else if( LOWORD(wParam) == WA_ACTIVE )
				{
					//if( GetLanguage() != eLang_Korea )
					{
						if( !thePacketTransLogDlg.IsActivePDlg() )
						{
							Send_CsOnRestoreOK();					
							RLGS("---  Enable SHack Chk ACTIVATE" );
						}
					}
				}
					
				Inputg->Acquire();	
			}
			break;	

		case WM_ACTIVATEAPP:
			{

///--JAPAN_BUG_MODIFY
/*
BOOL bCheckClose = FALSE;

#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)

	#ifdef KOREA_VERSION
		#ifndef C_HACK_SHIELD	  
			bCheckClose = TRUE;
		#endif
	#endif

	#ifdef JAPAN_VERSION
		#if !defined(C_HACK_SHIELD) && !defined(TEST_NO_HACK_SHIELD)
			bCheckClose = TRUE;	
		#endif
	#endif

	#ifdef INDONESIA_VERSION
			bCheckClose = TRUE;	
	#endif
			
	#ifdef  TAIWAN_VERSION
			bCheckClose = FALSE;	
	#endif

	#ifdef GLOBAL_VERSION
			bCheckClose = FALSE;	
	#endif 

	#ifdef CHINA_VERSION
		#ifndef C_HACK_SHIELD
			bCheckClose = TRUE;
		#endif			
	#endif

#endif

#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)
				if( wParam == FALSE )
				{
					if( bCheckClose	)
					{
						SendMessage( g_RockClient.GetApp(), WM_CLOSE, NULL, 1 );						
					}
				}
#endif 	
*/
			}
			break;

		case WM_HOTKEY:
			{
		
				if( (int)wParam == theHotKeyID_ALT_TAB )
				{
				}
			}
			break;

		case WM_COMMAND:
			switch( HIWORD( wParam ) )
			{
			//Edit 컨트롤의 핸들을 체크하지 않고 바로 문자열이 변경되었다는 메세지를 받으면
			//그 때...포커스를 가지고 있는 Edit 컨트롤을 찾아서...메세지 보냄
#ifdef UNICODE_ROCK
			case EN_UPDATE:
				PostMessageW( nRui->GetCurrentWndHandle(), WM_USER + 45, NULL, NULL );
				break;
			case EN_CHANGE:
				//hedit = GetFocus();
				//SendMessage(hedit, WM_USER+44, NULL, NULL); 
//				WCHAR Test[256] = L"";
//				GetWindowTextW( nRui->GetCurrentWndHandle() , Test , 256);
				PostMessageW( nRui->GetCurrentWndHandle(), WM_USER + 44, NULL, NULL );
				break;
#else 
			case EN_UPDATE:
				PostMessage( nRui->GetCurrentWndHandle(), WM_USER + 45, NULL, NULL );
				break;
			case EN_CHANGE:
				//hedit = GetFocus();
				//SendMessage(hedit, WM_USER+44, NULL, NULL);   
				char Test[256] = "";
				GetWindowText( nRui->GetCurrentWndHandle() , Test , 256);
				PostMessage( nRui->GetCurrentWndHandle(), WM_USER + 44, NULL, NULL );
				break;
#endif 
			}
			break;

		case	WM_CLOSE:
		case    WM_DESTROY:
			{
				if( (uMsg == WM_CLOSE) && (g_RockClient.GetGameMode() == GMODE_NORMAL) )
				{
					if( lParam == 0 )
					{
						SystemMenuWnd.GameExitBtn();
						return S_OK;
					}
				}
				

#ifdef C_HACK_SHIELD
			HS_StopService();
			HS_UnInit();
#endif


			//	if( (strcmp(strOsVersion, "windows xp") == 0) /*&& (ml_language != eLang_Korea)*/ )
				{
					KillProcess("KeyRock.rok");
				}				
		
				if( g_RockClient.m_bGameEnd == false )
				{
					WinSockClose();	//// sooree network
					g_RockClient.m_bGameEnd = true;				

					ThreadClosed();

					/// sooree pack 
					g_VFChrRoot.CloseFileSystem();
					g_VFChrItm.CloseFileSystem();
					g_VFChrMsh.CloseFileSystem();
					g_VFChrTex.CloseFileSystem();
					g_VFChrAni.CloseFileSystem();
				 	
					g_VFNpcRoot.CloseFileSystem();
					g_VFNpcItm.CloseFileSystem();
					g_VFNpcMsh.CloseFileSystem();
					g_VFNpcTex.CloseFileSystem();
					g_VFNpcAni.CloseFileSystem();

					g_VFEffectRoot.CloseFileSystem();
					g_VFEffectData.CloseFileSystem();
					g_VFEffectStatic.CloseFileSystem();
					g_VFEffectTex.CloseFileSystem();
					g_VFEffectTexAni.CloseFileSystem();

					g_VFEnv.CloseFileSystem();
					g_VFEtc.CloseFileSystem();
					g_VFInterface.CloseFileSystem();
					//g_VFDynamic.CloseFileSystem();
					g_VFSound.CloseFileSystem();

					g_VFMap.CloseFileSystem();

#ifdef	GM_CLIENT
					theGMCmdDlg.DestroyDlg();
#endif
					thePacketTransLogDlg.DestroyDlg();

#ifdef TAIWAN_VERSION
					char tmpbuff[256] = {0,};
					GetPrivateProfileString( "URL", "EventPage", NULL ,tmpbuff, sizeof(tmpbuff), "./gameinfo.ini");
					if(tmpbuff != NULL)
					{
						ShellExecute(NULL, "open", tmpbuff , NULL, NULL, SW_SHOW);
					}
#endif 

				}
				break;
			}
			break;
    }

	Inputg->MsgProc( hWnd, uMsg, wParam, lParam );

    return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}

/// 자동 버그 리포트 by sooree
void CRockClient::OnCrash()
{
	
	if( theGameSocketPtr )	/// 접속 중이라면 로그아웃 보내고 
	{
		///-- 클라이언트 비정상 종료 패킷전송
		Send_RpCsPCLogout( n_CLIENT_SHUTDOWN );
	}

#if defined(LAUNCHER_UPGREADE_VERSION) ///--&& !defined(_DEBUG)

	int autoReport = GetRockProfileInt( "BUGREPORT", "AUTOREPORT", 1, "./setup.ini" );
	char ip[16];

	GetRockProfileString( "BUGREPORT", "IP", "222.106.111.170", ip, sizeof( ip ), "./setup.ini" );
	unsigned int port = GetRockProfileInt( "BUGREPORT", "PORT", 10298, "./setup.ini" );

#else	

	int autoReport = GetPrivateProfileInt( "BUGREPORT", "AUTOREPORT", 1, "./setup.ini" );
	char ip[16];

	GetPrivateProfileString( "BUGREPORT", "IP", "222.106.111.170", ip, sizeof( ip ), "./setup.ini" );
	unsigned int port = GetPrivateProfileInt( "BUGREPORT", "PORT", 10298, "./setup.ini" );

#endif //LAUNCHER_UPGREADE_VERSION	
	
	if( autoReport == 0 || ip[0] == '\0' || port == 0 )
	{
		remove( "RockCrash.rlg" );
		return;
	}

	SRpCsBugReport	buglogMsg;
	FILE* fp = fopen( "RockCrash.rlg", "rt" );
	if( fp == NULL ) return;

	int n = 0, ch;
	while( ( ch = fgetc( fp ) ) != EOF )
	{
		buglogMsg.theLog[n] = (BYTE)ch;
		++n;
		if( n >= n_MaxBugLogSize - 1 ) break;
	}
	fclose( fp );
	
	buglogMsg.theLog[n] = '\0';
	buglogMsg.theLogSize = n;
	SAFE_STR_CPY( buglogMsg.theAccountName, theAccountName, 32 );
	SAFE_STR_CPY( buglogMsg.thePCName, nRui->thePcParam.PCName, 32 );
	memcpy( &(buglogMsg.thePCSystemInfo), &g_PcSystemInfo, sizeof(PCSystemInfo) );
	buglogMsg.theSize -= (n_MaxBugLogSize - n );

	CNetConnectSocket	logSocket;
	if( logSocket.Connect( ip, port ) )
	{
		logSocket.Create( n_MaxBugLogSize*4, n_MaxBugLogSize*2, n_MaxBugLogSize );
		logSocket.SetSockTCPNoDelay(); 
		logSocket.SetSockBufRCV( n_MaxBugLogSize*2 );
		logSocket.SetSockBufSND( n_MaxBugLogSize*2 );

		logSocket.PushSendQ( &buglogMsg, buglogMsg.theSize );
		logSocket.NbSend();
		Sleep( 500 );
		logSocket.Close();
		remove( "RockCrash.rlg" );
	}
}

void CRockClient::SendDebugLog(char *clog, ... )
{
	va_list args;
		
	va_start( args, clog );
	vsprintf( strSendMesseage , clog, args );
	va_end( args );

#if defined(LAUNCHER_UPGREADE_VERSION) ///--&& !defined(_DEBUG)
	int autoReport = GetRockProfileInt( "BUGREPORT", "AUTOREPORT", 1, "./setup.ini" );
	char ip[16];
	GetRockProfileString( "BUGREPORT", "IP", "222.106.111.170", ip, sizeof( ip ), "./setup.ini" );
	unsigned int port = GetRockProfileInt( "BUGREPORT", "PORT", 10298, "./setup.ini" );
#else
	int autoReport = GetPrivateProfileInt( "BUGREPORT", "AUTOREPORT", 1, "./setup.ini" );
	char ip[16];
	GetPrivateProfileString( "BUGREPORT", "IP", "222.106.111.170", ip, sizeof( ip ), "./setup.ini" );
	unsigned int port = GetPrivateProfileInt( "BUGREPORT", "PORT", 10298, "./setup.ini" );
#endif

	if( autoReport == 0 || ip[0] == '\0' || port == 0 )
	{
		return;
	}

	SRpCsDeBugReport	buglogMsg;

	int n = 0;
	while( strSendMesseage[n] != '\0' )
	{
		buglogMsg.theLog[n] = (BYTE)strSendMesseage[n];
		++n;
		if( n >= n_MaxBugLogSize - 1 ) break;
	}
	
	buglogMsg.theLog[n] = '\0';
	buglogMsg.theLogSize = n;
	SAFE_STR_CPY( buglogMsg.theAccountName, theAccountName, 32 );
	SAFE_STR_CPY( buglogMsg.thePCName, nRui->thePcParam.PCName, 32 );
	memcpy( &(buglogMsg.thePCSystemInfo), &g_PcSystemInfo, sizeof(PCSystemInfo) );
	buglogMsg.theSize -= (n_MaxBugLogSize - n );

	CNetConnectSocket	logSocket;
	if( logSocket.Connect( ip, port ) )
	{
		logSocket.Create( n_MaxBugLogSize*4, n_MaxBugLogSize*2, n_MaxBugLogSize );
		logSocket.SetSockTCPNoDelay(); 
		logSocket.SetSockBufRCV( n_MaxBugLogSize*2 );
		logSocket.SetSockBufSND( n_MaxBugLogSize*2 );

		logSocket.PushSendQ( &buglogMsg, buglogMsg.theSize );
		logSocket.NbSend();
		Sleep( 200 );
		logSocket.Close();		
	}	
}



int	CRockClient::GetGameVersion()
{
	int nVersion = 0;

	nVersion = abs( g_Version );
	BOOL IsModefyVersion = FALSE;
	TCHAR szMsg[255] = "";
	#if defined(KOREA_VERSION)
	{
		nVersion -= ( JAPAN_GAME_VERSION + INDONESIA_GAME_VERSION + 
					  GLOBAL_GAME_VERSION + CHINA_GAME_VERSION  + TAIWAN_GAME_VERSION + HONGKONG_GAME_VERSION );
		if( KOREA_GAME_VERSION != nVersion )
		{
			IsModefyVersion = TRUE;
		}	
	}
	#elif defined(JAPAN_VERSION)
	{
		nVersion -= ( KOREA_GAME_VERSION + INDONESIA_GAME_VERSION + 
					  GLOBAL_GAME_VERSION + CHINA_GAME_VERSION + TAIWAN_GAME_VERSION + HONGKONG_GAME_VERSION);
		if( JAPAN_GAME_VERSION != nVersion )
		{
			IsModefyVersion = TRUE;
		}	
	}
	#elif defined(INDONESIA_VERSION)
	{
		nVersion -= ( KOREA_GAME_VERSION + JAPAN_GAME_VERSION + 
					  GLOBAL_GAME_VERSION + CHINA_GAME_VERSION + + TAIWAN_GAME_VERSION + HONGKONG_GAME_VERSION);
		if( INDONESIA_GAME_VERSION != nVersion )
		{
			IsModefyVersion = TRUE;
		}
	}
	#elif defined(GLOBAL_VERSION)
	{
		nVersion -= ( KOREA_GAME_VERSION + JAPAN_GAME_VERSION + 
					  INDONESIA_GAME_VERSION + CHINA_GAME_VERSION +TAIWAN_GAME_VERSION +HONGKONG_GAME_VERSION );
		if( GLOBAL_GAME_VERSION != nVersion )
		{
			IsModefyVersion = TRUE;
		}
	}					
	#elif defined(CHINA_VERSION)
	{
		nVersion -= ( KOREA_GAME_VERSION + JAPAN_GAME_VERSION + 
					  INDONESIA_GAME_VERSION + GLOBAL_GAME_VERSION + TAIWAN_GAME_VERSION +HONGKONG_GAME_VERSION);
		if( CHINA_GAME_VERSION != nVersion )
		{
			IsModefyVersion = TRUE;
		}
	}  
	#elif defined(TAIWAN_VERSION)
	{
		nVersion -= ( KOREA_GAME_VERSION + JAPAN_GAME_VERSION + 
					 INDONESIA_GAME_VERSION + CHINA_GAME_VERSION + GLOBAL_GAME_VERSION + HONGKONG_GAME_VERSION );
		if( TAIWAN_GAME_VERSION != nVersion )
		{
			IsModefyVersion = TRUE;
		}
	}
	
	#elif defined(HONGKONG_VERSION)
	{
		nVersion -= ( KOREA_GAME_VERSION + JAPAN_GAME_VERSION + 
			INDONESIA_GAME_VERSION + CHINA_GAME_VERSION + GLOBAL_GAME_VERSION + TAIWAN_GAME_VERSION );
		if( HONGKONG_GAME_VERSION != nVersion )
		{
			IsModefyVersion = TRUE;
		}
	}
	


	#else 
	{
		nVersion = 0;		
	}
	#endif

	if(IsModefyVersion)
	{
		Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_FILE_CHANGE_CRC_ERROR) , nVersion );
		SEND_DEBUG_LOG(szMsg);
		return 0;
	}

	return nVersion;	
}


CRockClient::CRockClient()
{	
	m_bConnect = false;

	m_IsLoadProcessAfter = FALSE;
	m_InitTime = 0;

	pNpgl = NULL ; 


	



#ifdef BOUND_BOX_DISPLAY
	
	m_TopCameraHeight = 5000.0f;

#endif

#ifdef DISPLAY_16BIT_SCREENSHOT_DOWN
	m_IsClickScreen = 0;
#else
	m_IsClickScreen = FALSE;
#endif // DISPLAY_16BIT_SCREENSHOT_DOWN
	
	memset(m_strTemp , 0 , MAX_SIZE_512);
	memset(strOsVersion , 0 , MAX_SIZE_512);
	
//	InitializeCriticalSection(&m_csFlag);
	m_nLoadingCount = 0;
	m_byCurCursor	= 0;
	m_byCurFrame	= 0;
	m_bGameEnd		= false;
	ml_language		= 0;

	//-----------------------------------------------------------------------------	
	// 게임경로 읽어오기
	//-----------------------------------------------------------------------------		
	ZeroMemory(m_szRootDir, sizeof(m_szRootDir));
	
	// 자기 자신 경로 알아오기(exe와 dll이 다름. 인터넷 검색 참고)
	GetModuleFileName( NULL, m_szRootDir, sizeof( m_szRootDir ) );
	int i = 0 ; 
	for(  i = ( strlen( m_szRootDir ) - 1 ); i >= 0; --i )
	{
		if( m_szRootDir[i] == '\\' || m_szRootDir[i] == '/' )
		{
			m_szRootDir[i] = '\0';
			break;
		}
	}
#ifdef UNICODE_ROCK
	
	GetModuleFileNameW( NULL, m_szRootDirW, sizeof( m_szRootDirW ) );
	for(  i = ( Rstrlen( m_szRootDirW ) - 1 ); i >= 0; --i )
	{
		if( m_szRootDirW[i] == _RT('\\') || m_szRootDirW[i] == _RT('/') )
		{
			m_szRootDirW[i] = _RT('\0');
			break;
		}
	}

#endif 



	if (i < 0)
	{
		i = 0;
		m_szRootDir[i] = '\0';
	}
	SetCurrentDirectory( m_szRootDir );

#ifdef CHINA_VERSION
	m_strWindowTitle    = _T("LnD Online");
#elif defined(KOREA_VERSION) 
	m_strWindowTitle    = _T("시공찬가");
#elif defined(HONGKONG_VERSION)
	m_strWindowTitle =  RCHAR(_RT("Seiya-Online"));
#else 
	m_strWindowTitle =  RCHAR(_RT("Deco-Online"));
#endif
    m_bUseDepthBuffer   = TRUE;
    m_dwMinDepthBits    = 24;
    m_dwMinStencilBits  = 8;
    m_bShowCursorWhenFullscreen = FALSE;

	int opt = GetPrivateProfileInt("Client", "Screen", 2, "./setup.ini");

/// 릴리스 버전이면 당분간 무조건 
///#ifndef	_DEBUG
///	opt = 2;
///#endif

//	switch( opt )
//	{
//		case 0 : m_dwCreationWidth = 640;	m_dwCreationHeight = 480;	break;
//		case 1 : m_dwCreationWidth = 800;	m_dwCreationHeight = 600;	break;
//		case 2 : m_dwCreationWidth = 1024;	m_dwCreationHeight = 768;	break;
//		default: m_dwCreationWidth = 800;	m_dwCreationHeight = 600;	break;
//	}

	m_dwCreationWidth = 1024;	
	m_dwCreationHeight = 768;	

#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/22)
	
	m_dwCreationWidth = GetPrivateProfileInt( "Client", "Width", 1024, "./setup.ini" );
	// m_dwCreationWidth = m_dwCreationWidth >= 1024 ? m_dwCreationWidth : 1024;
	
	m_dwCreationHeight = GetPrivateProfileInt( "Client", "Height", 768, "./setup.ini" );
	// m_dwCreationHeight = m_dwCreationHeight >= 768 ? m_dwCreationHeight : 768;
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT

	opt = GetPrivateProfileInt("Client", "ColorBit", 0, "./setup.ini");

	if( opt == 0 )					// 16Bit Mode
	{
		m_wPixelFormat = 16;
	}
	else if( opt == 1 )				// 32Bit Mode 
	{
		m_wPixelFormat = 32;
	}
	
#ifndef INDONESIA_VERSION
	// 16Bit 화면 많이 깨지는 현상이 생김. 수정전까지는 일단 32Bit 사용 
	m_wPixelFormat = 32;
#endif

	// bMode :  0 풀스크린 1 : 창모드 
	
	opt = GetPrivateProfileInt("Client", "Window", 0, "./setup.ini");
	g_IsEWindowsMode = opt;
	
#ifdef ROCK_WINDOWS_MODE

		//한국은 풀스크린 .. 
	#if defined(KOREA_VERSION) || defined(JAPAN_VERSION)
		m_EWindowsMode = MODE_NORMAL_FULLMODE;		
	#else
		//그외 윈도우 풀스크린..
		m_EWindowsMode = MODE_WINDOW_FULLMODE;
	#endif
	
#else

	///-- 한국, 중국 이외는 무조건 풀모드 
	g_IsEWindowsMode = 0;
	
#endif

	
	//3.16 이후 부터 SetUp.ini 로부터 읽지 않고 
	//RockClient.exe Version 을 서버에 보내주고 화면에 버젼을 표시한다.  
		
	//GetPrivateProfileInt("Updater", "Version", 0, "./setup.ini");
	g_Version =  g_This_Client_Version;//
	
//	By wxywxy
/// GM Client 는 무조건 창모드 
#ifdef	GM_CLIENT
	//g_IsEWindowsMode = 1;// hhm 0 이면 GM Client도 풀스크린	
	char buf[32] = "";
	wsprintf( buf, "%d", g_IsEWindowsMode );
	WritePrivateProfileString( "Client", "Window", buf , "./setup.ini" );
#endif

	///-- 풀모드 적용
	if( !g_IsEWindowsMode )
	{
		m_bFullScreen = TRUE;
	}	
     
	m_PickPcIdx  = -1;
	m_PickUnique = -1;

	m_pTextureCursor = NULL;
///	m_pSprCursor = NULL;

	m_MapChangeProgress = false;

	m_IsReConnect = FALSE;
	m_IsMapLoding = FALSE;

	m_bIsBeginScene = false;
	m_bIsPresent = false;
	
	m_IsDeviceCursor = FALSE; 
    m_IsCursor = FALSE;
	m_isCursorInit = false;
	m_IsMouseRotate = false;
	m_lastPos.x = 0;
	m_lastPos.y = 0;
	
	m_MouseControlType = n_MouseType_Normal;
	memset(&m_UserName,NULL,sizeof(m_UserName));
	
	m_bIsDrawFullScreenUI = false;

	m_PickAttack_DelayTime = timeGetTime();

	m_GameTypeInfo.theFrom = n_Normal;	
	
	m_dwNewTimeHack = NULL;
	m_bHackStart = false;
   
	   
	theAreaInfoNum =0 ;
#ifdef CHINA_VERSION
	theAreaInfoNum = GetPrivateProfileInt( "ServerInfo", "LastSelected", 0, "./setup.ini" );
#endif	
	
    SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
    SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
    SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);

	STICKYKEYS skOff = g_StartupStickyKeys;
	if( (skOff.dwFlags & SKF_STICKYKEYSON) == 0 )// Shift 고정키 해제
	{
		// Disable the hotkey and the confirmation
		skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
		skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;
		
		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
	}
	    
	TOGGLEKEYS tkOff = g_StartupToggleKeys;
	if( (tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0 )// 토글키 해제
	{
		// Disable the hotkey and the confirmation
		tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
		tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;
		
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
	}
	
	FILTERKEYS fkOff = g_StartupFilterKeys;
	if( (fkOff.dwFlags & FKF_FILTERKEYSON) == 0 )// 필터키 해제
	{
		// Disable the hotkey and the confirmation
		fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
		fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;
		
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
	}
	
	m_NpcIndexList.clear();

	InitializeCriticalSection( &g_csLoadingFlag );// 크리티컬 섹션 초기화
}

HRESULT CRockClient::InvalidateDeviceObjects()
{
	RLGS("InvalidateDeviceObjects() Start" );	

	g_TexManager.ClearTheCreateTexList();
	
	g_Map.InvalidateDeviceObject();
	g_Billboard.InvalidateDeviceObject();
	m_FixSizeBillboard.InvalidateDeviceObject();
	g_Particle.m_Glow.InvalidateDeviceObject();	
	g_Pc.InvalidateDeviceObject();
	g_Pc_Manager.m_CharImage.InvalidateDeviceObject();	

	LoadingBarWnd.InvalidateDeviceObjects();
	g_EmblemTexManager.Clear();
	g_Particle.m_EffectManager.InvalidateDeviceObject();
	g_Particle.m_ScreenBlur.InvalidateDeviceObject();				// 블러 테스트

	RLGS("InvalidateDeviceObjects() End" );

    return S_OK;
}

HRESULT CRockClient::RestoreDeviceObjects()	
{
	if( m_lpD3DDevice == NULL )
		return S_OK;
	
	RLGS("RestoreDeviceObjects() Start" );
	
	g_RenderManager.Init();

	//if( GetLanguage() != eLang_Korea )
	{
		g_RockClient.Send_CsOnInactivated();
		RLGS("---  Disable SHack Chk Restore" );
	}    
	
	if(nRui)
	{
		if(nRui->m_IsSurpportHCursor)
		{
			nRui->SetDeviceCursor(nRui->GetCursor()->img[0] , false);
		}
	}
    
	g_Map.RestoreDeviceObject(m_lpD3DDevice);
	g_Billboard.RestoreDeviceObject(m_lpD3DDevice);
	m_FixSizeBillboard.RestoreDeviceObject(m_lpD3DDevice);
	g_Particle.m_Glow.RestoreDeviceObject(m_pD3D, m_lpD3DDevice);	
	g_Pc.RestoreDeviceObject(m_lpD3DDevice);
	g_Pc_Manager.m_CharImage.RestoreDeviceObject(m_lpD3DDevice);
	g_Particle.m_EffectManager.RestoreDeviceObject();
	g_Particle.m_ScreenBlur.RestoreDeviceObject(m_pD3D, m_lpD3DDevice);			// 블러 테스트
	
	
	///-- RestoreDeviceObjects() 상태에서는 디바이스가  D3DERR_DEVICENOTRESET
	///-- 상태이기 때문에... 렌더링을 진행해서는 안된다.
	if(m_dwGameMode == GMODE_NORMAL)	
	{
		///-- 초상화의 렌더링은 몇초후에 이루어지기때문에 상관없음
		g_Pc_Manager.m_CharImage.Create( g_Pc.GetPlayer()->GetUnique() , 
		g_Pc.m_MainPC.char_info.race , m_lpD3DDevice, false );		
		g_Pc_Manager.m_CharImage.Create( g_Pc.GetPlayer()->GetUnique() , 
		g_Pc.m_MainPC.char_info.race , m_lpD3DDevice );
		
		g_Pc_Manager.m_CharImage.ReFreshPetImage();
		g_PartyInforWnd.RestorePMemberCharImg();
	}

	LoadingBarWnd.RestoreDeviceObjects();

	g_RenderManager.SetTransform(D3DTS_VIEW, &(g_Camera.m_matView));
	g_RenderManager.SetTransform(D3DTS_PROJECTION, &(g_Camera.m_matProj));
	g_EmblemTexManager.LoadEmblemFiles();
 
//	g_RockClient.Send_CsRestoreDeviceObjectsEnd();
	
	//if( GetLanguage() != eLang_Korea )
	{
		g_RockClient.Send_CsOnRestoreOK();
	}

	RLGS("RestoreDeviceObjects() End" );

	return S_OK;
}

HRESULT CRockClient::DeleteDeviceObjects()
{
	//_RELEASE(m_pTextureCursor);		
	//_RELEASE(m_pSprCursor);
	
	return S_OK;
}

CRockClient::~CRockClient()
{
//	DeleteCriticalSection(&m_csFlag);

	DeleteCriticalSection( &g_csLoadingFlag );
	
	ProfileDumpOutputToBuffer();

	SAFE_DELETE( InputDXDevice );
	SAFE_DELETE( Inputg );
	SAFE_DELETE( nRui );

    SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
    SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
    SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);

	if( theHotKeyID_ALT_TAB )
	{
		GlobalDeleteAtom( theHotKeyID_ALT_TAB );
	}

	m_lpD3DDevice = NULL;
	g_lpDevice = NULL;
	m_NpcIndexList.clear();


}


HRESULT CRockClient::BeginScene()
{
	if( m_lpD3DDevice == NULL || m_bIsPresent == true )
	{
		return S_FALSE;
	}
	
	if( m_bIsBeginScene == true )
	{
		EndScene();
	}

	HRESULT d3d_result;

	d3d_result = g_RenderManager.BeginScene();

	if( d3d_result == S_OK )
	{
		m_bIsBeginScene = true;
	}
	else
	{
		m_bIsBeginScene= false;
	}

	return d3d_result;
}

HRESULT CRockClient::EndScene()
{
	if( m_lpD3DDevice == NULL || m_bIsBeginScene == false )
	{
		return S_FALSE;
	}
	
	HRESULT d3d_result;

	d3d_result = g_RenderManager.EndScene();

	if( d3d_result == S_OK )
	{
		m_bIsBeginScene = false;
	}

	return d3d_result;
}

HRESULT CRockClient::Present( RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion )
{	
	if( m_lpD3DDevice == NULL || m_bIsPresent == true )
	{
		return S_FALSE;
	}
	
	m_bIsPresent = true;

	HRESULT d3d_result;

	d3d_result = g_RenderManager.Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );

	m_bIsPresent = false;

	return d3d_result;
}

BOOL	CRockClient::InitLanguege()
{
	TCHAR pathname[256]="";
	int l_nDevSkip = 0 ;
		
	wsprintf( pathname, "%s\\data\\interface.rvf", m_szRootDir );
	if( g_VFInterface.OpenFileSystem( pathname ) == FALSE ) return FALSE;

#ifdef _DEBUG
  
	l_nDevSkip = GetPrivateProfileInt( "Local", "Language", 0, "./setup.ini");

#else

	#if defined(KOREA_VERSION)
		l_nDevSkip = eLang_Korea;
	#elif defined(JAPAN_VERSION)
		l_nDevSkip = eLang_Japan;
	#elif defined(INDONESIA_VERSION)
		l_nDevSkip = eLang_Indonesia;
	#elif defined(GLOBAL_VERSION)
		l_nDevSkip = eLang_English;
	#elif defined(CHINA_VERSION)
		l_nDevSkip = eLang_China;
	#elif defined(TAIWAN_VERSION)
		l_nDevSkip = eLang_Taiwan;
	#elif defined(HONGKONG_VERSION )
		l_nDevSkip = eLang_HongKong;
	#else 
		l_nDevSkip = 0;
	#endif

#endif     
	switch( l_nDevSkip ) 
	{
	case eLang_Japan:		
		g_StringMgr.Create( eLang_Japan );
		ml_language = eLang_Japan;		
		break;
	case eLang_Indonesia:
		g_StringMgr.Create( eLang_Indonesia );
		ml_language = eLang_Indonesia;		
		break;
	case eLang_English:
		g_StringMgr.Create( eLang_English );	
		ml_language = eLang_English;		
		break;
	case eLang_China:
		g_StringMgr.Create( eLang_China );
		ml_language = eLang_China;		
		break;
	case eLang_Taiwan:
		g_StringMgr.Create( eLang_Taiwan );
		ml_language = eLang_Taiwan;		
		break; 
	case eLang_Singapore:
		g_StringMgr.Create(eLang_Singapore);
		ml_language = eLang_Singapore;
		break; 
	case eLang_HongKong:
		g_StringMgr.Create(eLang_HongKong);
		ml_language = eLang_HongKong;
		break; 
	default:
		g_StringMgr.Create( eLang_Korea );
		ml_language = 0;
		break;
	}

	return TRUE;
}




HRESULT		CRockClient::InitState()
{
	GetOsVersion();
	   
	m_lpD3DDevice	= m_pd3dDevice;
	m_lpD3D			= m_pD3D;
	srand((unsigned int)time(NULL));

	g_lpDevice		= m_lpD3DDevice;
	m_fPrevTime		= timeGetTime();
	m_bSaveInfo		= false;	
	m_bTakeoff		= false; 
	

#if defined( LOAD_VF )
	/// sooree pack 
	char pathname[MAX_PATH];

	sprintf( pathname, "%s\\data\\etc.rvf", m_szRootDir );
	if( g_VFEtc.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;
	//-----------------------------------------------------------------------------	
	// 회사로고 이미지 읽기
	//-----------------------------------------------------------------------------	
	//ProfileBegin("LogoLoad - ReadVF");

	g_WebShopWnd.m_WebDialog.PreLoad(m_hInstance,m_hWnd);	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	char filename[256];
	sprintf( filename, "%s\\etc\\rocksoft.jpg", m_szRootDir );
	g_LogoTex.Read( filename, n_Interface );	

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	g_LogoTex.ReadVF( &g_VFEtc, "rocksoft.jpg", n_Interface );
	SetLastError(0);

#ifdef APPLY_INTRO_ANI
	g_EffTex1.ReadVF( &g_VFEtc, "logo1.tga", n_Interface );
	g_EffTex2.ReadVF( &g_VFEtc, "logo2.tga", n_Interface );
	g_EffTex3.ReadVF( &g_VFEtc, "logo3.tga", n_Interface );
	g_LoadingTex.ReadVF( &g_VFEtc, "loading.tga", n_Interface );
#endif
	

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////	
	//ProfileBegin("BeginScene");

	////-- Rui를 여기서 생성해줘야 한다.
	nRui = new CRui( m_lpD3DDevice );

#ifdef APPLY_INTRO_ANI
	nRui->m_Logo2DEffect.m_uNowEffStep = n_LogoEffStart;
	nRui->m_Logo2DEffect.m_dPrevTime = timeGetTime();
#endif

	FILE  *Tempfp = NULL;
	char  FileName[ 1024 ] = "";
	wsprintf( FileName , "%s\\UNPACKSOUND.dat" , m_szRootDir );
	
	Tempfp = fopen( FileName , "r+t" );

	if( Tempfp == NULL )
	{
		sprintf( pathname, "%s\\data\\sound.rvf", m_szRootDir );
		if( g_VFSound.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;
	}
	else
	{
		fclose( Tempfp );
	}

	//ProfileBegin("g_Sound");
	g_Sound.Create( g_RockClient.GetApp() , m_szRootDir );	

	//ProfileEnd("g_Sound");

#ifdef APPLY_INTRO_ANI
	g_Sound.UpdateSound( 60 );
	g_Sound.UpdateSound( 61 );
	g_Sound.UpdateSound( 283 );
#endif
	
	g_CLogoDisplayThread.Start();	
	SetLastError(0);

//	g_LogoTex.Destory();
	
	//ProfileEnd("LogoLoad - ReadVF");

	sprintf( pathname, "%s\\data\\chr.rvf", m_szRootDir );
	if( g_VFChrRoot.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;
	if( g_VFChrItm.OpenFileSystem( pathname ) == FALSE ) return	E_FAIL;
	if( g_VFChrMsh.OpenFileSystem( pathname ) == FALSE ) return	E_FAIL;
	if( g_VFChrTex.OpenFileSystem( pathname ) == FALSE ) return	E_FAIL;
	if( g_VFChrAni.OpenFileSystem( pathname ) == FALSE ) return	E_FAIL;
	g_VFChrRoot.ChangeDir( "/" );
	g_VFChrItm.ChangeDir( "/Itm" );
	g_VFChrMsh.ChangeDir( "/Msh" );
	g_VFChrTex.ChangeDir( "/Tex" );
	g_VFChrAni.ChangeDir( "/Ani" );	
	
	sprintf( pathname, "%s\\data\\npc.rvf", m_szRootDir );
	if( g_VFNpcRoot.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;
	if( g_VFNpcItm.OpenFileSystem( pathname ) == FALSE ) return	E_FAIL;
	if( g_VFNpcMsh.OpenFileSystem( pathname ) == FALSE ) return	E_FAIL;
	if( g_VFNpcTex.OpenFileSystem( pathname ) == FALSE ) return	E_FAIL;
	if( g_VFNpcAni.OpenFileSystem( pathname ) == FALSE ) return	E_FAIL;
	g_VFNpcRoot.ChangeDir( "/" );
	g_VFNpcItm.ChangeDir( "/Itm" );
	g_VFNpcMsh.ChangeDir( "/Msh" );
	g_VFNpcTex.ChangeDir( "/Tex" );
	g_VFNpcAni.ChangeDir( "/Ani" );	
	
	// 이펙트 파일들 로드
	sprintf( pathname, "%s\\data\\effect.rvf", m_szRootDir );
	if( g_VFEffectRoot.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;
	if( g_VFEffectData.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;
	if( g_VFEffectStatic.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;
	if( g_VFEffectTex.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;
	if( g_VFEffectTexAni.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;
	g_VFEffectRoot.ChangeDir( "/" );
	g_VFEffectData.ChangeDir( "/Data" );
	g_VFEffectStatic.ChangeDir( "/Static" );
	g_VFEffectTex.ChangeDir( "/Tex" );
	g_VFEffectTexAni.ChangeDir( "/Tex/Ani" );
	
	sprintf( pathname, "%s\\data\\env.rvf", m_szRootDir );
	if( g_VFEnv.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;


//	sprintf( pathname, "%s\\data\\interface.rvf", m_szRootDir );
//	if( g_VFInterface.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;

	sprintf( pathname, "%s\\data\\dynamic.rvf", m_szRootDir );
	//if( g_VFDynamic.OpenFileSystem( pathname ) == FALSE ) return E_FAIL;

#endif	

	SetGameMode(GMODE_INITLOAD);
	
	//ProfileEnd("VF Setting");


	//-----------------------------------------------------------------------------	
	// 커서초기화
	//-----------------------------------------------------------------------------	
	//ProfileBegin("InitCursor");

	InitCursor();

	//ProfileEnd("InitCursor");

	//	g_RunThreadFlag = FALSE;
	//  Sleep(100);		
	
	//ProfileEnd("BeginScene");

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	m_dwCreationWidth = m_dwCreationWidth != m_d3dsdBackBuffer.Width ? m_d3dsdBackBuffer.Width : m_dwCreationWidth;
	m_dwCreationHeight = m_dwCreationHeight != m_d3dsdBackBuffer.Height ? m_d3dsdBackBuffer.Height : m_dwCreationHeight;
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	//-------------------------------------------------------------------------
	//ProfileBegin("Input");
	Inputg = new CInput( m_hInstance, m_hWnd, m_dwCreationWidth, m_dwCreationHeight );
	
	memset( theKey, 0, sizeof( SKeys ) * 256 );
	memset( &theMouse, 0, sizeof( SMouse ) );
	//ProfileEnd("Input");

	//by simwoosung
	g_GTimer.InitTimer();	
	    
#ifdef LAUNCHER_UPGREADE_VERSION
	LoadSetup();
#endif
	
#ifdef SHUTDOWN_TEST
	if( GetLastErorrCode() ) RMessageBox(NULL , _RT("InitState 03") , _RT("") , MB_OK);
#endif 	
	///-- 클라이언트 맵데이터 로딩
	g_MapDataManager.LoadData();
	///-- 클라이언트 관련 추가 데이터 로딩
	g_Pc_Manager.m_PlayerNPCDataManager.LoadData();
	g_Pc_Manager.m_SourceDataManager.LoadData();
	
#ifdef UI_CONTROL_RENEWAL
	g_UiDataManager.LoadData(); //ui 데이터 로딩
#endif //UI_CONTROL_RENEWAL
	 
	///--GM_Client
#ifdef	GM_CLIENT
	theGMCmdDlg.CreateDlg( m_hInstance, m_hWnd );// hhm 필요할때만 생성. 주석 처리하면 GM 다이얼로그 없다.
#endif	
				
	//ProfileBegin("RUI Create");	

	///---------------------------------------------------------------------------
	///-- Rui
	///---------------------------------------------------------------------------

	nRui->Set_Language( ml_language );
	
	//ProfileEnd("RUI Create");

	memset( m_PcInfo, 0, sizeof( SPcInfo ) * MAX_CHARACTER );
	nRui->SetPcInfo( m_PcInfo );//미니맵 생성 순서때문에.... CreateDesktop보다 앞에서 설정
	//미니맵에서 m_PcInfo정보를 얻기 위해서 미니맵 생성 전에 포인터 설정....
		

	//nRui 초기 설정과... 인터페이스 구성
	//ProfileBegin("CreateDesktoprui");

	nRui->CreateDesktop( m_hInstance, m_hWnd, m_dwCreationWidth, m_dwCreationHeight, 32 );
	
	//ProfileEnd("CreateDesktoprui");
	//ProfileBegin("SetSkillSystem");

	MainMenuWnd.SetSkillSystem( g_Pc.GetSkillSystem() );
	g_SkillWnd.SetSkillSystem( g_Pc.GetSkillSystem() );
//	SkillRegistWnd.SetSkillSystem( g_Pc.GetSkillSystem() );
//	SkillOverWriteWnd.SetSkillSystem( g_Pc.GetSkillSystem() );

	//(주의)로그인 이후에 값이...들어온다....
	m_PcParam = nRui->GetPcParamInfo();
	m_PcInven = nRui->GetPcInvenInfo();
	
	//ProfileEnd("SetSkillSystem");
	//-------------------------------------------------------------------------


	///-----------------------------------------------------------------------------	
	/// Load Options
	///-----------------------------------------------------------------------------	

	//ProfileBegin("Option");
	int	option;

	/// 그림자 
	g_Shadow = GetPrivateProfileInt( "Client", "Shadow", 2, "./setup.ini" );

	/// 뽀샤시 
	option = GetPrivateProfileInt( "Client", "Glow", 30 , "./setup.ini" );
	
	if( m_wPixelFormat == 16 )
	{
		option = 0;
	}
   
	g_Particle.m_GlowAlpha = option;

	/// 풀 밀도 
	g_GrsVisible = GetPrivateProfileInt( "Client", "Grass", 0, "./setup.ini" );

	/// 오브젝트 시야 거리 + 지형 디테일 
	g_LODRange = GetPrivateProfileInt( "Client", "ObjView", 2, "./setup.ini" );
	g_Obj_Manager.SetLodRange( g_LODRange );

	/// 원경 지형 그릴 것인가 말것인가? 
	g_ViewLODTerrain = GetPrivateProfileInt( "Client", "LODTerrain", 1, "./setup.ini" );

	/// BGM 플레이 여부 
	option = GetPrivateProfileInt( "Client", "BGM", 1, "./setup.ini" );
	g_Sound.SetMP3Enable( option );

	/// Effect Sound 플레이 여부 
	option = GetPrivateProfileInt( "Client", "EffectSound", 1, "./setup.ini" );
	g_Sound.SetWaveEnable( option );

	/// 맵 오브젝트 텍스처 로딩 유무
	g_ISObjTexLoad = GetPrivateProfileInt( "Client", "ObjTexLoad", 1, "./setup.ini" );

	/// 로그 프린트 여부
	g_LogPrint = GetPrivateProfileInt( "Client", "LogPrint", 0, "./setup.ini" );
	
	/// 지형이펙트 표시 여부
	g_ISLandEffect = GetPrivateProfileInt( "Client", "LandEffect", 1, "./setup.ini" );

	/// 이펙트 사운드 볼륨
	g_Sound.SetWaveVolume( GetPrivateProfileInt( "Client", "EffectSoundVol", 100, "./setup.ini" ) );

	/// 배경음악 볼륨	
	g_Sound.SetMP3Volume( GetPrivateProfileInt( "Client", "BGMVol", 100 , "./setup.ini" ) );	

	/// Weather Option
	option = GetPrivateProfileInt( "Client", "Weather", 1, "./setup.ini" );
	g_ISWeatherVisible = option;

	/// HelpView Option
	option = GetPrivateProfileInt( "Client", "HelpView", 1, "./setup.ini" );
	g_IsHelpView = option;

	/// 캐릭터 아이디 
	option = GetPrivateProfileInt( "Client", "CharShowID", 1, "./setup.ini" );
	g_ISCharShowIDVisible = option;

	///프레임 최적화
	option = GetPrivateProfileInt( "Client", "FrameOptimize", 0, "./setup.ini" );
	g_IsFrameOptimize = option;

	option = GetPrivateProfileInt( "Client", "EffectMinimize", 0, "./setup.ini" );
	g_IsEffectMinimize = option;
	if( g_IsEffectMinimize )
	{
		g_Max_Effect_Display = 250;		
	}
	else
	{
		g_Max_Effect_Display = 450;		
	}

	option = GetPrivateProfileInt( "Client", "TexDetail", 0, "./setup.ini" );
	g_nTextureDetail = option;

	///-- 이펙트 효력 메시지 출력여부
	option = GetPrivateProfileInt( "Client", "SkillEffSys", 1, "./setup.ini" );
	g_IsSkEffSys = option;

	///화면 밝기
	option = GetPrivateProfileInt( "Client", "Gamma", 0, "./setup.ini" );
	option = max( option, 0 );
	option = min( option, 8 );
	g_fGamma = option * 0.1f + 0.8f;
		
#ifdef	OTP_GAMMA_ENABLE
	////////////////////////////// 감마 조절 by sooree
	/// 감마 옵션 
	option = GetPrivateProfileInt( "Client", "Gamma", 5, "./setup.ini" );	/// 0.5 ~ 2.0 : 16단계. 기본 = 1.0 즉, 5 
	option = max( option, 5 );
	option = min( option, 15 );
	g_fGamma = option * 0.1f + 0.5f;	/// 0 -> 0.5f, 5 -> 1.0f, 10 -> 1.5, 15 -> 2.0f
	/// 콘트라스트 옵션 
	option = GetPrivateProfileInt( "Client", "Contrast", 5, "./setup.ini" );	/// 0.5 ~ 1.5 : 11단계. 기본 = 1.0 즉, 5 
	option = max( option, 5 );
	option = min( option, 10 );
	g_fContrast = option * 0.1f + 0.5f;	/// 0 -> 0.5f, 5 -> 1.0f, 10 -> 1.5f
	///-----------------------------------------------------------------------------	
#endif

	// 버젼읽어오기
	DWORD dwVersion = GetVersion();
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));

	//-----------------------------------------------------------------------------	
	// 2개변수가 모두 5라면 XP
	//-----------------------------------------------------------------------------	
	if( dwWindowsMajorVersion == 5 && dwWindowsMinorVersion == 5 )
		g_Sound.SetMP3Enable( FALSE );		// By wxywxy	

	theUserSelectExit = FALSE;		/// sooree network
	SetLoginStep( nNotConnect );	/// sooree network
	if( WinSockStart() == FALSE )	/// sooree network
	{
///		MessageBox( m_hWnd, "WinSock Intialize Failed!", "error", MB_OK|MB_ICONSTOP );
		return	E_FAIL;
	}
#ifdef _DEBUG
	DebugPrint( "!\r\n WinMain:게임시작" );	
#endif 


#ifdef SHUTDOWN_TEST
	if( GetLastErorrCode() ) RMessageBox(NULL , _RT("InitState 04") , _RT("") , MB_OK);
#endif 	
/******************************************************************************************
	* Direct3D Render State
******************************************************************************************/
	
/*
	//-----------------------------------------------------------------------------	
	// 커서초기화
	//-----------------------------------------------------------------------------	
	InitCursor();
	
	//-----------------------------------------------------------------------------	
	// 회사로고 이미지 읽기
	//-----------------------------------------------------------------------------	
	char filename[256];
	sprintf( filename, "%s\\etc\\rocksoft.jpg", m_szRootDir );
	g_LogoTex.Read( filename, n_Interface );
*/

//	g_RunThreadFlag = FALSE;
//	Sleep(100);	
	
	//ProfileEnd("Option");
	//-----------------------------------------------------------------------------	
	// 초기데이타생성	: sooree network
	//-----------------------------------------------------------------------------	
	//ProfileBegin("InitLoading");
	InitLoading();		/// 메뉴 텍스쳐, 사운드 로딩하기
	//ProfileEnd("InitLoading");
#ifdef SHUTDOWN_TEST
	if( GetLastErorrCode() ) RMessageBox(NULL , _RT("InitState a2") , _RT("") , MB_OK);
#endif 
	//ProfileBegin("SelectLoading");
	if( SelectLoading() == false )	/// 파티클, 구름.... 여러가지 데이타 로딩
	{
		//ProfileEnd("SelectLoading");
		ClientClose( g_RockClient.GetApp() , 1);
		return E_FAIL;
	}
	
	//ProfileEnd("SelectLoading");

//	MEMORYSTATUS ms;
//	GlobalMemoryStatus(&ms);
//	DWORD elapse_memory = 0;
//	elapse_memory = g_Last_Memory - ms.dwAvailPhys ; 
//	RLG1( "메모리 사용량 체크 :: elapse_memory = %d", elapse_memory );

	//ProfileBegin("thread");
	SetGameMode( GMODE_LOGINPAGE );

	g_TexManager.UpdateSoundCreate();

	/*FILE* fp = fopen( "__RockUpdater.exe", "r" );
	if( fp )
	{
		fclose( fp );
		remove( "Deco.exe" );
		rename( "__RockUpdater.exe", "Deco.exe" );
	}*/

#ifdef REMOVE_PDBFILE
	FILE* fp = NULL;
	fp = fopen( "RockClient.pdb", "r" );
	
	if( fp )
	{
		fclose( fp );
		remove( "RockClient.pdb" );
	}

#endif

	//ProfileEnd("thread");
	
	
//-----------------------------------------------------------------------------	
// 싱글모드 클라이언트 
//-----------------------------------------------------------------------------	
#ifndef	NETWORK_CLIENT
	//ProfileBegin("ETC");

	PCTABLE	pc;
	pc.Init();
	pc.lUnique			= 0;
	pc.nIndex			= 0;

	pc.lDestBlock		= 2080;
	pc.lBlock			= 2080;

	pc.lCurMap			= g_Map.m_NowMapID;
				
	pc.char_info.sex  = 0;
	pc.char_info.race = c_PcRace_User;
	pc.char_info.job  = 0;

	// 얼굴, 머리모양... 머리색깔
	pc.char_info.face = 0;
	pc.char_info.hair = 0;
	pc.char_info.color = 0;
	
	g_Pc.SetupMainPc(pc);
	g_Pc.SetupTempPc(g_Pc.m_MainPC);
	g_Map.SetupMapIdx(g_Map.m_NowMapID);

	//-----------------------------------------------------------------------------
	// 맵 체인지
	//-----------------------------------------------------------------------------
	//ProfileEnd("ETC");

	//ProfileBegin("MAP");	

	if( MapChange( g_Map.m_NowMapID ) == false )
	{
		//ProfileEnd("MAP");
		ClientClose( g_RockClient.GetApp() , 1);
		
		return E_FAIL;
	}
	//ProfileEnd("MAP");

	//ProfileBegin("ETC2");
	SetGameMode( GMODE_LOGINPAGE );

	// 캐릭터 애니메이션 기본셋팅
	g_Pc.GetPlayer()->AnimationPlay( n_Stand_Peace9 );
	g_Pc.GetPlayer()->m_SimTime = g_nowTime;
	//ProfileEnd("ETC2");

#endif
	
//	GlobalMemoryStatus(&ms);
//	elapse_memory = g_Last_Memory - ms.dwAvailPhys ; 
//	RLG1( "메모리 사용량 체크 :: elapse_memory = %d", elapse_memory );

	//ProfileEnd("< CRockClient::InitState >");
	
//	DebugPrint( "!\r\n CRockClient::InitState" );

	///--Sleep(100);	

	m_IsLoadProcessAfter = TRUE;
	m_InitTime = 0;

#ifdef APPLY_INTRO_ANI
	while( n_LogoEffEnd != nRui->m_Logo2DEffect.m_uNowEffStep )
	{
		Sleep(1000);	
	}
#endif
	
	
	return	S_OK;
}

int CRockClient::GetRockProfileInt( IN LPCSTR lpAppName, IN LPCSTR lpKeyName, IN INT nDefault , IN LPCSTR lpFileName )
{

	int Result = nDefault; 
#ifdef LAUNCHER_UPGREADE_VERSION	
	for(int i =0 ; i < m_iFileDataCount ; ++i)
	{
		if( (strcmp( m_FileDataInfo[i].AppName , lpAppName)  == 0) &&
			(strcmp( m_FileDataInfo[i].KeyName , lpKeyName)  == 0))
		{
			Result =  atoi(m_FileDataInfo[i].DataStr);
			break;
		}
	}
#endif
	return Result;
}


BOOL CRockClient::GetRockProfileString(IN LPCSTR lpAppName , IN LPCSTR lpKeyName, IN LPCSTR lpDefault, OUT LPSTR lpReturnedString, IN DWORD nSize  , IN LPCSTR lpFileName  )
{
	
#ifdef LAUNCHER_UPGREADE_VERSION
	//목차와
	//찾기
//	TCHAR lpAppName[256] ="[Updater]";
//	TCHAR lpkeyName[256] ="Version";
//	TCHAR lpReturnString[256] ="";
   
	//처음 setup.ini 읽어올때 .. 
	//GetRockProfileString (lpAppName , lpKeyName,  lpDefault, lpReturnedString, nSize ,  lpFileName )

	if( lpDefault == NULL)
	{
		strcpy( lpReturnedString ,  "\0" );
	}
	else
	{
		strcpy( lpReturnedString ,  lpDefault );
	}
	
	for(int i =0 ; i < m_iFileDataCount ; ++i)
	{
		if( (strcmp( m_FileDataInfo[i].AppName , lpAppName)  == 0) &&
			(strcmp( m_FileDataInfo[i].KeyName , lpKeyName)  == 0))
		{
			strcpy( lpReturnedString ,  m_FileDataInfo[i].DataStr);
			return TRUE;
		}
	}

#endif
return	 TRUE;
}

BOOL CRockClient::LoadSetup()
{
 
	TCHAR InfoFileName[256] = "serverlist.ini";

	TCHAR pathname[256]="";

	CVFileHandle *pFH;
   		
	pFH = g_VFEtc.OpenFile(InfoFileName);
	if( pFH == NULL)
	{
		return FALSE;
	}
 
	TCHAR 	string[512] = {0,};		// 문서 줄단위 저장 변수
	bool	bRead  = true;			// 문서 다 읽었는지 체크
	TCHAR	def_str[512] = {0,};
	int		len = 0;
	//int size =  pFH->GetSize();
	//BYTE* pData = new BYTE[size];
	//memcpy(pData , (BYTE *)pFH->GetData(), pFH->GetSize() );
	//delete [] pData;
	//pData = NULL;
	
	//새데이터를 얻어온후 
	//데이터 변경후 
	// 압축을 하여 
	// 다시 추가를 한다 .. 
	
	// 줄 단위로 읽기
	if( pFH->Gets( string, sizeof( string )) == 0 )
	{
		g_VFEtc.CloseFile( pFH );
		return FALSE; 
	}		
	
	TCHAR tempAppName[128]="";
	TCHAR tempKeyName[128]="";

	do 
	{
		// 주석이 있다면 다음 줄 읽음
		while( (string[0] == '/' && string[1] == '/') || string[0] == ';' )	
		{	
			// 더 이상 읽은게 없다면 루프 종료
			// 빈공백이 있으면 . 
			++m_iFileDataCount;
			if( pFH->Gets( string, sizeof( string ) ) == 0 )
			{
				//bRead = false;
				break;
			}
		}
		len  = strlen(string);
		
		memset( def_str , NULL , sizeof(def_str)) ;
		sscanf( string, _T("%s"), def_str );
		strcpy( &def_str[len] , "" );
		int index = 0 ; 
		
		if(string[0] == '[')
		{				
			memset(tempAppName , NULL, sizeof(tempAppName ));
			strncpy( tempAppName, def_str +1 , len - 2 );
		}
		
		if(  strchr(string , '=') != NULL)
		{
			index = strcspn(def_str  , "=");
			strncpy( m_FileDataInfo[m_iFileDataCount].KeyName , def_str ,  index );
			strncpy( m_FileDataInfo[m_iFileDataCount].DataStr , def_str+(index+1) , sizeof(m_FileDataInfo[m_iFileDataCount].DataStr)  );
		}

		strcpy( m_FileDataInfo[m_iFileDataCount].AppName , tempAppName );

		
		// 다음 줄 읽기, 읽은게 없다면 string은 0으로
		++m_iFileDataCount;
		
		if( m_iFileDataCount == 126 )
			break; 
		
		pFH->Gets( string, sizeof( string ));
		if( _tcsicmp( string, _T("[END]") ) == 0 )
		{
			bRead = false;	
			break; 
		}


	}  while( bRead != false ); // 더 이상 읽은게 없다면 루프 종료			

	g_VFEtc.CloseFile( pFH );

	return TRUE;


}

//-----------------------------------------------------------------------------
// 메뉴 텍스쳐, 사운드 로딩하기
//-----------------------------------------------------------------------------
void		CRockClient::InitLoading()
{
	//g_Sound.PlayMP3();

	m_FaceExp.Init();

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	g_Particle.m_Fade.Init( m_dwCreationWidth, m_dwCreationHeight );
	
#else
	
	g_Particle.m_Fade.Init( 1024 , 768 );
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	m_SlangFilter.LoadList( "slang.dat" );
	m_SlangFilter.LoadNameList( "namelist.dat" );
//	m_SlangFilter.LoadAdminNameList("AdminName.dat");

	g_Particle.m_SpotEffect.Init( g_lpDevice );
	m_Action.Load();	
	
	// 클래스 뷰 데이터 로딩
	m_ClassView.Load();
	
	// 카메라 에니메이션
	char TempStr[ 1024 ] = "";

	for( int i = 0 ; i < 15 ; ++i )
	{
		wsprintf( TempStr , "%d.cwk" , (i+1) );
		g_Camera.LoadWalk( TempStr );
	}


	g_Camera.LoadInvWalk();


	g_LogWnd.LoadData();
}

//-----------------------------------------------------------------------------
// 선택화면 로딩
//-----------------------------------------------------------------------------
bool CRockClient::SelectLoading()
{	
	//......................................................................................................
	// 렌더 스테이트 초기화 By wxywxy
	//......................................................................................................
	g_RenderManager.Init();

	//-----------------------------------------------------------------------------
	// 잡다한 텍스쳐
	//-----------------------------------------------------------------------------
	g_Pc.m_TexTable.Create();

	//ProfileBegin("< CRockClient::SelectLoading >");
	g_Map.SetDevice( m_lpD3DDevice );		
	g_Map.SetLight();
	///-- by simwoosung
	g_Map.CreateShadowList();

	//ProfileBegin("g_Item");
	if( g_Item.Create() == false )
	{
		return false;
	}
	//ProfileEnd("g_Item");
	
	//ProfileBegin("CreateEffect");
	g_Pc.CreateEffectData();
	//ProfileEnd("CreateEffect");

	//ProfileBegin("Damage");
	g_Pc.CreateDamageEffect();
	//ProfileEnd("Damage");

	//ProfileBegin("SpecialEff");
	m_FixSizeBillboard.Init( m_lpD3DDevice );

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	bool bGlow = true;
	if(m_d3dCaps.MaxTextureWidth < 1024 || m_d3dCaps.MaxTextureHeight < 1024)
		bGlow = false;

	g_OptionWnd.SwitchGlow(bGlow);// 게임내 옵션 뽀샤시 효과 온/오프

	g_Particle.m_Glow.Init( m_dwCreationWidth, m_dwCreationHeight , m_pD3D , m_lpD3DDevice, bGlow );	
	g_Particle.m_ScreenBlur.Init( m_dwCreationWidth, m_dwCreationHeight, m_pD3D , m_lpD3DDevice );	// 블러 테스트
	
#else
	
	g_Particle.m_Glow.Init( 1024 , 768 , m_pD3D , m_lpD3DDevice );	
	g_Particle.m_ScreenBlur.Init( 1024 , 768 , m_pD3D , m_lpD3DDevice );	// 블러 테스트
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

///	g_Particle.m_GlowAlpha = 0;	/// 옵션에서 로딩 
	D3DXCOLOR Color = D3DCOLOR_ARGB( g_Particle.m_GlowAlpha , 255 , 255 , 255 );
	g_Particle.m_Glow.SetColor( Color );
		
	g_SpellEffectBaseData.Load( m_szRootDir );
	g_Particle.m_ArrowDisplay.Init( m_lpD3DDevice );

	//ProfileEnd("SpecialEff");
	//.........................................................................
	// 빌보드 / 파티클 텍스처 로딩
	//.........................................................................

#if !defined( LOAD_VF )

	g_Particle.m_EffectTextureManager.AllLoadFromDirectory( m_szRootDir );
	
#else
	//ProfileBegin("AllEffect");
	g_Particle.m_EffectTextureManager.AllLoadFromVirtualFile( &g_VFEffectTex );
	//ProfileEnd("AllEffect");
#endif

	//ProfileBegin("m_ExpDisplay");
	g_Particle.m_ExpDisplay.InitDevice( g_lpDevice );
	//ProfileEnd("m_ExpDisplay");

	// 캐릭터 얼굴 초기화
	g_Pc_Manager.m_CharImage.Init( g_lpDevice );
	
	//.........................................................................
	// 파티클 관련 ( 오브젝트 , 데이터 ) 로딩
	//.........................................................................	
	//ProfileBegin("CreateEffectObj");
	g_Particle.CreateEffectObj(m_szRootDir);
	//ProfileEnd("CreateEffectObj");
	
	//ProfileBegin("g_ParticleCreate");
	g_Particle.Create(m_szRootDir);
	//ProfileEnd("g_ParticleCreate");

	//ProfileBegin("g_ParticleLoadTexture");
	g_Particle.m_Combo2DEffect.LoadTexture( g_lpDevice );
	//ProfileEnd("g_ParticleLoadTexture");
	
	//.........................................................................
	// 곤충 관련 ( 데이터 ) 로딩
	//.........................................................................
	//ProfileBegin("g_Insect");
	if( g_ISObjTexLoad )
	{
		g_Insect.Create_Insect( m_szRootDir , 1);
		g_Insect.Create_Insect( m_szRootDir , 2);
	}
	//ProfileEnd("g_Insect");
	
	//.........................................................................
	
	//ProfileBegin("g_Map");
	g_Map.Create( m_lpD3DDevice );	
	//ProfileEnd("g_Map");

	//ProfileBegin("g_Grass");
	if( g_ISObjTexLoad )
		g_Grass.Create(m_lpD3DDevice);	
	//ProfileEnd("g_Grass");

	//ProfileBegin("g_GTexture");
	if( g_ISObjTexLoad )
	{
		g_GTexture.Create(m_lpD3DDevice);
	}
	//ProfileEnd("g_GTexture");

	/// set file은 처음 뜰 때 한번만 로딩 : root\\map\\ 아래에 0.set, 1.set... 이 순서로...
	char buf[MAX_PATH];
	sprintf( buf, "%s\\map", m_szRootDir );
	
	//ProfileBegin("g_Obj_Manager");
	g_Obj_Manager.SetCreate( buf );
	//ProfileEnd("g_Obj_Manager");
	

	//ProfileBegin("m_SetEnv");
	g_Map.m_SetEnv.Load( m_szRootDir );
	//ProfileEnd("m_SetEnv");

	//ProfileBegin("MapClear_c_RockMap_Login_Field");
	MapClear( c_RockMap_Login_Field );
	//ProfileEnd( "MapClear_c_RockMap_Login_Field" );

	//ProfileBegin("MapCreate_c_RockMap_Login_Field");
	if( MapCreate( c_RockMap_Login_Field , 184, 328 ) == false )
	{		
		return false;
	}
	//ProfileEnd("MapCreate_c_RockMap_Login_Field");

	CharSelectWnd.StartCameraPosition();									// 카메라 워크관련 By wxywxy

#ifdef __EF_MANAGER_PROFILE	
	
		g_Particle.m_EffectTextureManager.WriteProfile();

#endif

		//ProfileEnd("< CRockClient::SelectLoading >");

		return true;
}


//-----------------------------------------------------------------------------
// Desc: 맵로딩
//-----------------------------------------------------------------------------
bool CRockClient::MapCreate( DWORD idx )
{
	if(idx == 0 && g_RockClient.GetLanguage() == eLang_Korea )
	{
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NIGHT );
	}
	else
	{
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NONE );
	}

	//ProfileBegin("< CRockClient::MapCreate >");
	if( g_Sound.m_BackSound.m_bPlay )
	{
		g_Sound.m_BackSound.Stop();
		g_Sound.m_BackSound.Destroy();
		g_Sound.m_GroundIndex = -1;
	}

	int current = timeGetTime();

	// NPC 대화초기화
	// 맵 읽기
	//ProfileBegin("MapLoading");
	if( g_Map.m_NowMapID != idx )
	{
/// sooree pack
#if !defined( LOAD_VF )
		g_Map.Load( idx );
#else
		if( g_Map.LoadVF( idx ) == false )
		{
			return false;
		}
#endif
		g_Pc.SetupSize( g_Map.GetSize() * 2 );				

		if( g_Pc_Manager.MapChangePC( g_RockClient.GetRootDir() ) == false )
		{
			return false;			
		}

		if( g_Pc_Manager.MapChangeNPC( g_RockClient.GetRootDir() ) == false )
		{
			return false;			
		}		
	}

#ifndef	NETWORK_CLIENT
	static bool initFlag = true;

	if( initFlag )
	{
		/// 맵 크기와 상관없이 캐릭터가 적당한 위치에 그려지도록 수정 
		g_Pc.m_TempPC.lBlock = (int)( g_Pc.m_cSize / 3 ) * g_Pc.m_cSize + (int)( g_Pc.m_cSize / 3 );
		initFlag = false;
	}
#endif
	
	//ProfileEnd("MapLoading");
	//ProfileBegin("MAPETC");

	g_Pc.m_TempPC.lDestBlock = g_Pc.m_TempPC.lBlock;
	g_Pc.SetWarpMove( g_Pc.GetTempPc() );
		
	int destx = g_Pc.GetDestX();
	int desty = g_Pc.GetDestY();
	
	g_Camera.Init(m_lpD3DDevice, destx, desty);
//	g_Camera.SetCameraPos( 3.1f , 0.5f , 120.0f );
	g_Camera.Update();

	g_Camera.m_bRotBySort = true;	

	g_Map.UpdateTerrain(destx, desty, true);
	

	// Picking값 초기화
	g_Map.SetupPickedX(0);
	g_Map.SetupPickedY(0);

	g_Sound.PlayMP3( false, (int)idx );

	LoadingBarWnd.SetPercent(94);
	
	//--------------------------------------------------------------------------------------------
	// Interface Initialize 
	
	MiniMapWnd.SetMiniMap( g_Map.GetSize(), G_MAP_MINIMAPID(g_Map.m_NowMapID) ); ///--g_Map.m_NowMapID + TID_MAP02_512 - 4 ) );//MAP02_512 -> 32
	MiniMapWnd.SetMapIndex( g_Map.m_NowMapID );
	MiniMapWnd.Map->m_bIsMapChange = TRUE;
	
	if( idx != c_RockMap_Login_Field ) // 로딩 맵이 아니면...표시 
	{
		
#ifdef ROCKCLIENT_DEV
		nRui->theViewPcInfo	= n_VIEW_ALL_PCID;
#else
		nRui->theViewPcInfo	= n_VIEW_PICK_PCID;	
#endif			
	}

	// 픽킹된 유저 및 몬스터 표시 
	m_PickPcIdx = -1;
	
	//ProfileEnd("MAPETC");
	//ProfileEnd("< CRockClient::MapCreate >");	

	g_Particle.m_WeatherDrop.Init();

	g_Pc_Manager.m_CharImage.ReFreshPetImage();

	return true;
}

//-----------------------------------------------------------------------------
// Desc: 맵로딩
//-----------------------------------------------------------------------------
bool CRockClient::MapCreate(DWORD idx,int x,int y)
{
	if(idx == 0 && g_RockClient.GetLanguage() == eLang_Korea )
	{
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NIGHT );
	}
	else
	{
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NONE );
	}
	
	//ProfileBegin("< CRockClient::MapCreate(DWORD idx,int x,int y) >");
	if( g_Sound.m_BackSound.m_bPlay )
	{
		g_Sound.m_BackSound.Stop();
		g_Sound.m_BackSound.Destroy();
		g_Sound.m_GroundIndex = -1;
	}

	int current = timeGetTime();

	// NPC 대화초기화
	// 맵 읽기
	//ProfileBegin("g_Map.LoadVF");
	if( g_Map.m_NowMapID != idx )
	{
/// sooree pack
#if !defined( LOAD_VF )
		g_Map.Load( idx );
#else
		if( g_Map.LoadVF( idx ) == false )
		{
			return false;
		}
#endif
		g_Pc.SetupSize( g_Map.GetSize() * 2 );		
		
		if( g_Pc_Manager.MapChangePC( g_RockClient.GetRootDir() ) == false )
		{
			return false;
		}
		if( g_Pc_Manager.MapChangeNPC( g_RockClient.GetRootDir() ) == false )
		{
			return false;
		}
	}

	g_Camera.Init(m_lpD3DDevice, x, y);
	g_Camera.Update();
	g_Camera.m_bRotBySort = true;	

	//ProfileEnd("g_Map.LoadVF");

	ProfileBegin("UpdateTerrain");
	g_Map.UpdateTerrain( x, y, true );
	ProfileEnd("UpdateTerrain");

	ProfileBegin("CMAPETC");
	// Picking값 초기화
	g_Map.SetupPickedX(0);
	g_Map.SetupPickedY(0);

	g_Sound.PlayMP3( false, (int)idx );

	//-------------------------------------------------------------------------
	MiniMapWnd.SetMiniMap( g_Map.GetSize(), G_MAP_MINIMAPID(g_Map.m_NowMapID) ); ///--  + TID_MAP02_512 - 4 ) );//MAP02_512 -> 32
	MiniMapWnd.SetMapIndex( g_Map.m_NowMapID );
	//-------------------------------------------------------------------------
	
	ProfileEnd("CMAPETC");	
	//ProfileEnd("< CRockClient::MapCreate(DWORD idx,int x,int y) >");

	g_Particle.m_WeatherDrop.Init();

	g_Pc_Manager.m_CharImage.ReFreshPetImage();

	return true;
}

void CRockClient::MapClear(DWORD idx)
{
	g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NONE );

	//ProfileBegin("< CRockClient::MapClear >");
	int current = timeGetTime();

	// NPC 대화초기화
	// 맵 읽기
	//ProfileBegin("mCleanup");
	if( g_Map.m_NowMapID != idx )
	{
		LoadingBarWnd.SetPercent(30);//%%
		g_Map.Cleanup();
		g_Map.CleanupObj();	
		g_Map.m_bObjSortInit = false;		// 거리에 따른 Alpha Obj 처리를 위해..		
		g_Map.CleanupWaterfall();
		g_Pc_Manager.DestroyPC( idx );		
		g_Pc_Manager.DestroyNPC();		
		///--PRINT_DLG_LOG("Map Cleanup() 시간 :%.3f", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );
		DebugPrint( "!\r\n Map Cleanup() 시간 :%.3f", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );
	}
	else
	{
		g_Map.CleanObjParticleAliveInfo();
	}
	
	//...........................................7...............................................................
	// 지역 이펙트 지우기 By wxywxy
	//..........................................................................................................
	g_Particle.m_EffectManager.DeleteAllLandEffect();
	//..........................................................................................................

	//ProfileEnd("mCleanup");	
	
	//ProfileBegin("ClearInit");
	// 유저 테이블, 캐릭터 삭제
	g_Pc.InitTable();
	g_Pc.FinalCleanup();
	
	// NPC 테이블, 캐릭터 삭제
	g_Map.CleanupNpc();
	g_Map.InitNpcTable();
	
	// 아이템 삭제
	m_IsMapLoding = TRUE;
	
	// 파티클 삭제
	if( !m_IsReConnect )
	{
		g_Particle.DeleteAll();
		g_Billboard.DelAllManager();		
	}

	m_IsMapLoding = FALSE;
	LoadingBarWnd.SetPercent(40);//%%

	// 카메라 위치, 맵업데이트
	if( g_Map.m_byMapType == MS_INDOOR )
	{
		g_Camera.SetCameraPos( 0.84f, 0.6f, 100.0f );
	}
//	else
//	{
//		g_Camera.SetCameraClear();
//	}
	// 배경음악변경
	g_Sound.AllStop();
	
	//ProfileEnd("ClearInit");	
	//ProfileEnd("< CRockClient::MapClear >");
}


//-----------------------------------------------------------------------------
// Desc: 맵로딩
//-----------------------------------------------------------------------------
bool CRockClient::MapChange(DWORD idx)
{
	if( idx && WEATHER_TYPE_NIGHT == g_Particle.m_WeatherDrop.GetDropType() )
	{
		g_Particle.m_WeatherDrop.m_WasBeforeNight = TRUE;
	}	
	
	char strBuf[512] = "";
	::GetAreaStrName(strBuf, idx);
	
	///--PRINT_DLG_LOG( "" );
	///--PRINT_DLG_LOG( "맵이동시작 %s", strBuf );
	
	//by simwoosung 마우스 커서 복구
	if(m_IsMouseRotate)
	{
		nRui->SetCursorType(n_CT_NORMAL);
		m_IsMouseRotate = false;
	}
	
	DWORD dRideNPCID = 0;

	if( g_Pc.GetPlayer()->m_State.bRide )
	{
		dRideNPCID = g_Map.m_NpcTable[g_Pc.GetPlayer()->m_State.TransIndex].lUnique;
		SetRideOff( MAX_USER_INTABLE );
	}	
	
	if(idx == 0 && g_RockClient.GetLanguage() == eLang_Korea )
	{
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NIGHT );
	}
	else
	{
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NONE );
	}

	//ProfileBegin("< CRockClient::MapChange >");
	int current = timeGetTime();
		
	//ProfileBegin("MapClear");
	MapClear(idx);
	//ProfileEnd("MapClear");

	RLG1( "MapCreate :: MapID = %d", idx );

	//by simwoosung - 파티 멤버 정보창의 전환
	if(g_PartyInforWnd.IsMemberParty())
	{
		list< SPARTYMEMBER_INFO * >::iterator i = g_PartyInforWnd.GetBeginMember();
		list< SPARTYMEMBER_INFO * >::iterator end = g_PartyInforWnd.GetEndMember();

		for( ; i != end; i++ )
		{
			SPARTYMEMBER_INFO *pMemberInfo = (*i);
		    if(pMemberInfo->m_dPCID == g_Pc.m_MainPC.lUnique)
			  continue;
	        
			pMemberInfo->m_bIsViewRange = FALSE;					
		}
	}

	//ProfileBegin("MapCreate");
	if( MapCreate(idx) == false )
	{
		MessageBoxA(NULL , "There is not information about this map" , "Error" , MB_OK  );
		LoadingBarWnd.SetPercent(98);
		//ProfileEnd("MapCreate");
		return false;
	}
	//ProfileEnd("MapCreate");

	LoadingBarWnd.SetPercent(98);

	g_Particle.m_WeatherDrop.Init();	

	// log
	DebugPrint( "!\r\n Total Map Change() 시간 :%.3f", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );
	///--PRINT_DLG_LOG("Total Map Change() 시간 :%.3f", SAFE_TIME_SUB( timeGetTime() , current ) / 1000.0f );

	//ProfileEnd("< CRockClient::MapChange >");

	if( dRideNPCID )
	{
		SetRideOn( MAX_USER_INTABLE );
	}
	
	g_Particle.m_WeatherDrop.m_WasBeforeNight = FALSE;

	return true;
}

//-----------------------------------------------------------------------------
// Desc: 맵로딩 쓰레드 펑션
//-----------------------------------------------------------------------------
unsigned int __stdcall  LoadingBarThread( void* arg )
{
	while( 1 ) 
	{	
		g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) , 
									D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 1000 , TRUE );
		while( 1 )
		{
			Sleep( 10 );
			
			if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) )
			{
				break;
			}

			EnterCriticalSection( &g_csLoadingFlag );			

			g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
								D3DCOLOR_XRGB( 0x55, 0x66, 0x77 ), 1.0f, 0);

			if( SUCCEEDED( g_RockClient.BeginScene() ) )
			{
				LoadingBarWnd.DrawScreenAfterImage();
				g_Particle.m_Fade.Process( g_lpDevice );
				g_RockClient.EndScene();						

				g_RockClient.Present( NULL, NULL, NULL, NULL );
			}			
			
			LeaveCriticalSection( &g_csLoadingFlag );			
		}
		
		g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 
									D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) , 1500 , TRUE );	

		while( 1 )
		{
			Sleep( 10 );
			
			EnterCriticalSection( &g_csLoadingFlag );			

			g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
							  D3DCOLOR_XRGB( 0x55, 0x66, 0x77 ), 1.0f, 0);
			
			nRui->UpdatePcData();
			nRui->RenderProc();

			if( g_Particle.m_Fade.GetColor() != D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) )
			{
				if( SUCCEEDED( g_RockClient.BeginScene() ) )
				{
					g_Particle.m_Fade.Process( g_lpDevice );
					g_RockClient.EndScene();					
				}
			}
			
			g_RockClient.Present( NULL, NULL, NULL, NULL );
			
			LeaveCriticalSection( &g_csLoadingFlag );		
			
			if( LoadingBarWnd.Percent == 99 )
			{
				g_Particle.m_Fade.Start( FADE_STATE_FADE , D3DCOLOR_ARGB( 0 , 0 , 0 , 0  ) , 
											D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 500 , TRUE );
				while( 1 )
				{
					Sleep( 10 );				
					
					EnterCriticalSection( &g_csLoadingFlag );					

					g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
										g_RockClient.m_SceneManager.GetBGColor() , 1.0f, 0);
					
					nRui->UpdatePcData();
					nRui->RenderProc();
					
					if( SUCCEEDED( g_RockClient.BeginScene() ) )
					{
						g_Particle.m_Fade.Process( g_lpDevice );
						g_RockClient.EndScene();						
					}

					g_RockClient.Present( NULL, NULL, NULL, NULL );
					
					if( g_Particle.m_Fade.GetColor() == D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) )
					{
						nRui->SendEvent( WID_LoadingWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

						NonPlayerInfoWnd.thePickedNpcID = -1;
						NonPlayerInfoWnd.thePickedUserID = -1;
						
						g_RockClient.m_MapChangeProgress = false;				
						LoadingBarWnd.IsChange	= true;
						LoadingBarWnd.SetPercent(100);						
					}
					
					LeaveCriticalSection( &g_csLoadingFlag );
					
					if(!g_RockClient.m_MapChangeProgress)
					{
						break;
					}
				}			

				break;
			}
		}

		break;
	}


	CloseHandle( g_hLoadingThread );

	return 0;
}


unsigned int __stdcall MousePosUpdateThread( LPVOID pVoid )
{
	CRITICAL_SECTION	csLoadingFlag;
	InitializeCriticalSection( &csLoadingFlag );

	for(;;)
	{	
		if( true == g_RockClient.m_bGameEnd )
			return -1;

		switch ( g_RockClient.GetGameMode() )
		{
			case GMODE_LOGINPAGE:
			case GMODE_SELECT:
			case GMODE_NORMAL:
			{
				EnterCriticalSection( &csLoadingFlag );	

				POINT		ptCursor;
				GetCursorPos( &ptCursor );
				::ScreenToClient( g_RockClient.GetApp(), &ptCursor );

				nRui->GetCursor()->pos.x = ptCursor.x;
				nRui->GetCursor()->pos.y = ptCursor.y;

				LeaveCriticalSection( &csLoadingFlag );				

				Sleep( 1 );		
			}
		}
	}

	DeleteCriticalSection( &csLoadingFlag );

	return 0;
}





//-----------------------------------------------------------------------------
// 메인처리루틴
//---------------------------------------------------------------------------
void CRockClient::CheckWarpzone()
{
	int		x = 0;
	int		y = 0;
	int		mapNum = 0;

	bool	isChange = false;
	long	destBlock;

	x = g_Pc.GetPlayer()->m_Mov.curt.x;
	y = g_Pc.GetPlayer()->m_Mov.curt.y;

	if( g_Map.m_NowMapID == c_RockMap_Millena_City )
	{
		if( x >= 50 && x <= 55 && y >= 205 && y <= 218 )
		{
			mapNum		= c_RockMap_Millena_Field1_TwinibleWood_Gate;
			isChange	= true;
			destBlock	= 149 + 63 * g_Pc.m_cSize;
		}
	}
	else if( g_Map.m_NowMapID == c_RockMap_Millena_Field1_TwinibleWood_Gate )
	{
		if( x >= 138 && x <= 148 && y >= 43 && y <= 49 )
		{
			mapNum		= c_RockMap_Millena_City;
			isChange	= true;
			destBlock	= 64 + 210 * g_Pc.m_cSize;
		}
	}

	if( isChange )
	{
		g_Map.m_NowMapID = mapNum;

		g_Pc.SetupTempPc( g_Pc.m_MainPC );
		g_Pc.m_TempPC.lBlock = destBlock;

		if( g_RockClient.MapChange( g_Map.m_NowMapID ) )
		{
			ClientClose( g_RockClient.GetApp() , 1);
			return;
		}
	}
}


/// old version의 password() 함수 : 게임 엔젤 유저용 
/// MySQL 4.1.0 이후 버전에서는 안먹는다. 
void MySQLPassword( const char* org, char* out )
{
	DWORD nr = 1345345333; 
	DWORD add = 7; 
	DWORD nr2 = 0x12345671; 

	int tmp;

	for( int i = 0; i < (int)strlen( org ) ; ++i ) 
	{ 
		if( org[i] == ' ' || org[i] == '\t' ) continue; 
		tmp = ( int )org[i]; 
		nr ^= ( ( ( nr & 63 ) + add ) * tmp ) + ( nr << 8 ); 
		nr2 += ( nr2 << 8 ) ^ nr; 
		add += tmp; 
	} 

	DWORD result1 = nr & ( (unsigned)( 1 << 31 ) - 1 ); // Don't use sign bit (str2int) 
	DWORD result2 = nr2 & ( (unsigned)( 1 << 31 ) - 1 ); 

	wsprintf( out, "%08x%08x", result1, result2); 
}

// Login Packet Send
BOOL	CRockClient::LoginProc( char* aUserName, char* aPassWord, UINT uFrom )
{
	static DWORD OldTime = timeGetTime();
	static BOOL  OneRun  = TRUE;
	
	char userName[128] = {0,};
	char passWord[128] = {0,};

	strcpy( userName, aUserName );
	strcpy( passWord, aPassWord );

	UINT from = uFrom;

	NLoginStep eStep = GetLoginStep();

	// 한번 보내고 60초 시간체크
#ifdef _DEBUG
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , OldTime ) , > , 1000 ) || OneRun )
#else
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , OldTime ) , > , 1000 ) || OneRun )
#endif
	{
		OldTime = timeGetTime();
		OneRun = FALSE;
	}
	else
	{
	
		eStep = GetLoginStep();
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NOW_CONNECT ) , 1000, NULL );
		return FALSE;
	}


	if( GetGameMode() != GMODE_LOGINPAGE )
		return FALSE;	
	
	if(eStep != nNotConnect )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NOW_CONNECT ) , 1000, NULL );
		return FALSE;	
	}

	/// 여기서 aUserName, aPassWord 에 이상이 있는지 없는지 체크 : 길이, 공백문자, 특수문자... 
	/// 이상 없다면 

	/// 원래 ip, port는 하드코딩으로 박아놔야 하지만 지금은 테스트 단계이므로...
	char ip[16] = {0, };
	short port=0;
	char str[128]="";
#if defined(LAUNCHER_UPGREADE_VERSION) ///--&& !defined(_DEBUG)
	#ifdef CHINA_VERSION
		wsprintf( str, "Server%d", g_RockClient.GetAreaInfoNum());
		GetRockProfileString( str, "IP", NULL,  ip, sizeof(  ip ), "./setup.ini" );
		port = GetRockProfileInt( str, "PORT", 0, "./setup.ini" );
	#else
		GetRockProfileString( "NET", "IP", NULL,  ip, sizeof(  ip ), "./setup.ini" );
		port = GetRockProfileInt( "NET", "PORT", 0, "./setup.ini" );
	#endif

#else

	#ifdef CHINA_VERSION
		wsprintf( str, "Server%d", g_RockClient.GetAreaInfoNum());
		GetPrivateProfileString( str, "IP", NULL,  ip, sizeof(  ip ), "./setup.ini" );
		port = GetPrivateProfileInt( str, "PORT", 0, "./setup.ini" );
	#else
		GetPrivateProfileString( "NET", "IP", "", ip, sizeof( ip ), "./setup.ini" );
		port = GetPrivateProfileInt( "NET", "PORT", 0, "./setup.ini" );

	#endif

#endif
	

#ifdef _DEBUG
	RLG2("ip %s port %d" , ip ,port );
#endif 


	if( ConnectToLoginServer( ip, port ) == FALSE )
	{
///		MessageBox( NULL, "ConnectToLoginServer fail", "error", MB_OK|MB_ICONSTOP );
		return FALSE;
	}

	memset( m_UserName , 0 , sizeof( m_UserName ) );
	memset( m_UserPWD  , 0 , sizeof( m_UserPWD  ) );
	memset( m_UserSysID , 0 , sizeof(m_UserSysID)) ; 


	wsprintf( m_UserName , "%s" , aUserName );
	wsprintf( m_UserPWD  , "%s" , aPassWord );


	Send_RpCsAccountLogin( userName, aPassWord, from );
	SetLoginStep( nAccountLogin );
	

	md5_encodeU( passWord , g_WebShopWnd.m_md5 );

	ServerListWnd.mb_server = false;  
/*
	int len    = strlen( g_WebShopWnd.m_md5 );
	int	SubNum = ( int ) ( 'a' - 'A' );

	for( int i = 0 ; i < len ; ++i )
	{
		if( g_WebShopWnd.m_md5[ i ] >= 'a' && g_WebShopWnd.m_md5[ i ] <= 'z'  )
		{
			g_WebShopWnd.m_md5[ i ] = g_WebShopWnd.m_md5[ i ] - SubNum;
		}
	}
*/    
	return TRUE;
}

Void CRockClient::SetLoginStep( NLoginStep aLoginStep )
{
	theLoginStep = aLoginStep;
}
  
BOOL	CRockClient::ReLoginProc(void)
{
	if( GetGameMode() != GMODE_LOGINPAGE ) return FALSE;	
	if( GetLoginStep() != nNotConnect ) return FALSE;	
  
	/// 여기서 aUserName, aPassWord 에 이상이 있는지 없는지 체크 : 길이, 공백문자, 특수문자... 
	/// 이상 없다면 

	/// 원래 ip, port는 하드코딩으로 박아놔야 하지만 지금은 테스트 단계이므로...
	char ip[16] = {0, };
	short port=0;
	char str[128]= {0,};
#if defined(LAUNCHER_UPGREADE_VERSION) ///--&& !defined(_DEBUG)
	#ifdef CHINA_VERSION
		wsprintf( str, "Server%d", g_RockClient.GetAreaInfoNum());
		GetRockProfileString( str, "IP", NULL,  ip, sizeof( ip ), "./setup.ini" );
		port = GetRockProfileInt( str, "PORT", 0, "./setup.ini" );
	#else
		GetRockProfileString( "NET", "IP", NULL,  ip, sizeof(  ip ), "./setup.ini" );
		port = GetRockProfileInt( "NET", "PORT", 0, "./setup.ini" );
	#endif
#else
	#ifdef CHINA_VERSION
		wsprintf( str, "Server%d", g_RockClient.GetAreaInfoNum());
		GetPrivateProfileString( str, "IP", NULL,  ip, sizeof( ip ), "./setup.ini" );
		port = GetPrivateProfileInt( str, "PORT", 0, "./setup.ini" );
	#else
		GetPrivateProfileString( "NET", "IP", "", ip, sizeof( ip ), "./setup.ini" );
		port = GetPrivateProfileInt( "NET", "PORT", 0, "./setup.ini" );
	#endif
#endif //LAUNCHER_UPGREADE_VERSION
	
	if( ConnectToLoginServer( ip, port ) == FALSE )
	{
///		MessageBox( NULL, "ConnectToLoginServer fail", "error", MB_OK|MB_ICONSTOP );
		return FALSE;
	}
   
	char userName[128];
	SAFE_STR_CPY( userName, m_UserName, 128 );
	
	UINT from = g_RockClient.m_GameTypeInfo.theFrom;

	Send_RpCsAccountLogin( userName, m_UserPWD, from );
	
	SetLoginStep( nAccountLogin );

	return TRUE;
}

//........................................................................................................
// 임시 스킬 테스트 ( CMD -> /생성이펙트 스킬코드 데미지맞는NPC개수)
//........................................................................................................
void CRockClient::CreateExampleEffect(DWORD Code,int Num)
{
	if( Num <= 0 )
		return;
	
	int AttackCount = Num;
	
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( Code ); 

	if( skill_Info == NULL )
		return;

	DWORD *m_pUniqueList = NULL;

	BOOL   ISKeepUpSkill[ 3 ] = { FALSE , };
	

	// 지속 여부
	for( int i = 0 ; i < 3 ; ++i )
	{
		if( skill_Info->theEffectInfo[ i ].theKeepupTime != 0 )
		{
			ISKeepUpSkill[ i ] = TRUE;
		}
	}
	
	int    m_InsertCount = 0;
	
	// 자기 한테 걸기
	if( skill_Info->theTarget == n_AT_SELF )
	{
		AttackCount = 1;
	
		 m_pUniqueList = SAFE_NEW_ARRAY( DWORD , AttackCount );

		 m_pUniqueList[ 0 ] = g_Pc.GetPlayer()->GetUnique();

		 m_InsertCount++;
	}
	else
	{
		m_pUniqueList = SAFE_NEW_ARRAY( DWORD , AttackCount );
		
		g_Map.m_SortTable.StartNPCPointer();

		int NPC_Index = -1;
	
		while( ( NPC_Index = g_Map.m_SortTable.GetNPC() ) != -1 )
		{
			if( m_InsertCount >= AttackCount )
				break;
			
			if( g_Map.m_NpcTable[NPC_Index].race == c_NpcRace_Item ||
				g_Map.m_Npc[NPC_Index].m_bIsVisible == false ||
				g_Map.m_Npc[NPC_Index].m_curt_event.type == SM_STATE_DIE )
			{
				continue;
			}
			
			m_pUniqueList[ m_InsertCount ] = g_Map.m_Npc[ NPC_Index ].GetUnique();
			
			m_InsertCount++;
			
		}	
	}	
	
	if( m_InsertCount <= 0 )
		return;
	
	SRpScSkill TestSkill;
	
	TestSkill.theResult    = n_Success;
	TestSkill.theSkillCode = Code;
	
	TestSkill.theHP = nRui->thePcParam.Stat1.theHP ;
	TestSkill.theSP = nRui->thePcParam.Stat1.theSP ;
	TestSkill.theMP = nRui->thePcParam.Stat1.theMP ;
	
	
	if( skill_Info->theTarget == n_AT_SELF )
	{
		TestSkill.theTargetType = n_PC;
		TestSkill.theTarget = m_pUniqueList[ 0 ];
	}
	else
	if( skill_Info->theActTarget == 4 )
	{
		D3DXVECTOR3 vPos;

		vPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
		vPos.y = g_Pc.GetPlayer()->GetPosTM()._42;
		vPos.z = g_Pc.GetPlayer()->GetPosTM()._43;

		int X = ( int ) ( vPos.x / (TILE_SIZE/2) );
		int Y = ( int ) ( vPos.z / (TILE_SIZE/2) );

		TestSkill.theTargetType = n_Cell;
		TestSkill.theTarget     = ( DWORD )( ( Y * g_Pc.m_cSize ) + X );
	}
	else
	{
		TestSkill.theTargetType = n_NPC;
		TestSkill.theTarget = m_pUniqueList[ 0 ];
	}
  
	  

	TestSkill.theTargetNum = m_InsertCount;
	
	for(int i = 0 ; i < m_InsertCount ; ++i )
	{
		TestSkill.theTargetList[ i ].theTargetID = m_pUniqueList[ i ];
		TestSkill.theTargetList[ i ].theDieFlag  = 0;
		TestSkill.theTargetList[ i ].theEffectMaxValue = 0;
		TestSkill.theTargetList[ i ].theEffectValue[ 0 ] = 0;
		TestSkill.theTargetList[ i ].theEffectValue[ 1 ] = 0;
		TestSkill.theTargetList[ i ].theEffectValue[ 2 ] = 0;
		TestSkill.theTargetList[ i ].thePushedCell = 0;
	
		if( skill_Info->theTarget == n_AT_SELF )
		{
			TestSkill.theTargetList[ i ].theTargetType = n_PC;
		}
		else
		{
			TestSkill.theTargetList[ i ].theTargetType = n_NPC;
		}

/*
		if( skill_Info->theActionKeepupTime == TRUE )
		{
			TestSkill.
		}
*/

		for( int j = 0 ; j < 3 ; ++j )
		{
			if( TRUE == ISKeepUpSkill[ j ] )
			{
				TestSkill.theTargetList[ i ].theKeepupFlag[ j ] = n_Success;
			}
			else
			{
				TestSkill.theTargetList[ i ].theKeepupFlag[ j ] = 0;
			}
		}
	}
	
	Proc_RpScSkill( &TestSkill );

	SAFE_DELETE_ARRAY( m_pUniqueList );
}

///---------------------------------------------------------------------------
///-- UserInputProc
///---------------------------------------------------------------------------


LRESULT		CRockClient::UserInputProc()
{
	


	static int OldPickX = -1;
	static int OldPickZ = -1;

	if( g_Pc.GetPlayer()->theMapType == n_FightZone && 
		g_Pc.GetPlayer()->theFightZoneReady == false)
	{
		return S_OK;
	}

	if( IsWaitingSvrMsg() ) 
	{
		return S_OK;	// 일반적으로 Cs 와 Sc가 쌍을 이루어 처리되는 경우 중간 입력 무시
	}

	if( !g_Pc.GetPlayer()->IsMyPlayerRender ) 
	{
		return S_OK;
	}
	
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_SLIDE_NPC ||				// 크리티컬에 맞아 밀려나거나 죽었을땐 이동 및 공격 불가 
		g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_SITDOWN ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_STANDUP ||
		
		g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_SITDOWN ||	// 개인상점시 일어설때 이동 공격 불가
		g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_STANDUP || 
		g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_KEEPING ||

		g_UserStoreSellWnd.ISUserSellMode() ||
		g_UserStoreBuyWnd.ISUserBuyMode()	
		) 			
	{
		return S_OK;
	} 
	
	if( !m_SceneManager.GetKeyInput() )
	{
		return S_OK;
	}

	CWnd* Wnd = NULL;
	char strTemp[128] = {0,};

	int i = 0;

	///-----------------------------------------------------------------------
	///-- 몬스터 자동 타겟킹

#ifdef NEXT_SELECT_TO_TAB


	static int indexCount  = 0 ; 
	static int dwTabKeyOldTime = g_nowTime;

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , dwTabKeyOldTime ) , > , 4000 ) )
	{
		m_NpcIndexList.clear();
		dwTabKeyOldTime = g_nowTime;
	}


	if( theKey[DIK_TAB].Action == n_baUp )
	{		

		dwTabKeyOldTime = g_nowTime;

		// 부가효과 (강제타겟) 처리 
		if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke))
		{
			return S_OK;
		}
		
		float min_dist = 1000.0f;
		int npc_index = -1;
		int pc_index = -1;

		//대전장 .. 
		if( g_Pc.GetPlayer()->theMapType == n_FightZone )
		{
			for( int i = 0; i < MAX_USER_INTABLE; i ++ )
			{
				if( g_Pc.m_PcTable[i].nFlag == g_Pc.m_MainPC.nFlag ||
					g_Pc.m_Pc[i].m_bIsVisible == false ||
					g_Pc.m_Pc[i].m_curt_event.type == SM_STATE_DIE ||
					g_Pc.m_Pc[i].m_CharAlpha.State == CHAR_NONE_DISPLAY )
				{
					continue;
				}
				
				if( g_Pc.m_PcTable[i].lUnique != -1 )				
				{
					if( g_Pc.m_Pc[i].m_fDistance < min_dist )
					{
						min_dist = g_Pc.m_Pc[i].m_fDistance;
						pc_index = i;
					}
				}
			}		

			if( pc_index == -1 )
			{
				nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				NonPlayerInfoWnd.thePickedUserID = -1;
				NonPlayerInfoWnd.thePickedNpcID = -1;
			}
			else
			{
				if( g_Pc.m_PcTable[ pc_index ].lUnique != -1 )
				{			
					if( NonPlayerInfoWnd.thePickedUserID != pc_index )
					{
						nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
						
						Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
						Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Pc.m_PcTable[ pc_index ].char_info.theLevel );
						Wnd->ReceiveEvent( n_emSetText, (EPARAM) strTemp, 240, 240, 200 );			
						
						Wnd = Container->Get( WID_NonPlayerInfo_NameText );
						Wnd->ReceiveEvent( n_emSetText, (EPARAM) g_Pc.m_PcTable[ pc_index ].char_info.thePCName, 240, 240, 200 );			
						
						NonPlayerInfoWnd.thePickedUserID = pc_index;
						NonPlayerInfoWnd.thePickedNpcID = -1;
						
						NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Pc.m_Pc[ pc_index ] );
					}
				}
			}
		}
		//일반 필드
		else
		{				
			for( int i = 0; i < MAX_USER_INTABLE; i ++ )
			{
								
				if( !CanAttackPlayer( &(g_Pc.m_Pc[i]) ) ||
					g_Pc.m_Pc[i].m_bIsVisible == false ||
					g_Pc.m_Pc[i].m_curt_event.type == SM_STATE_DIE ||
					g_Pc.m_Pc[i].m_CharAlpha.State == CHAR_NONE_DISPLAY )
				{
					continue;
				}
				
				if( g_Pc.m_PcTable[i].lUnique != -1 )				
				{

					if( g_Pc.m_Pc[i].m_fDistance < min_dist )
					{
						min_dist = g_Pc.m_Pc[i].m_fDistance;
						pc_index = i;
					}
				}
			}

		
			bool bFind = false;

			if( pc_index != -1 )
			{
				if( g_Pc.m_PcTable[ pc_index ].lUnique != -1 )
				{			
					bFind = true;
					if( NonPlayerInfoWnd.thePickedUserID != pc_index )
					{
						nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
						
						Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
						Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Pc.m_PcTable[ pc_index ].char_info.theLevel );
						Wnd->ReceiveEvent( n_emSetText, (EPARAM) strTemp, 240, 240, 200 );			
						
						Wnd = Container->Get( WID_NonPlayerInfo_NameText );
						Wnd->ReceiveEvent( n_emSetText, (EPARAM) g_Pc.m_PcTable[ pc_index ].char_info.thePCName, 240, 240, 200 );			
						
						NonPlayerInfoWnd.thePickedUserID = pc_index;
						NonPlayerInfoWnd.thePickedNpcID = -1;
						
						NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Pc.m_Pc[ pc_index ] );					
					}
				}
			}			
		

			
			for( i = 0; i < MAX_NPC_INTABLE; i ++ )
			{
					if( g_Map.m_NpcTable[i].race == c_NpcRace_Item ||
					g_Map.m_Npc[i].m_bIsVisible == false ||
					g_Map.m_Npc[i].m_curt_event.type == SM_STATE_DIE ||
					g_Map.m_Npc[i].m_State.nRideState == nRideState_Ride ||
					g_Map.m_Npc[i].m_State.nHolyState == nHolyState_Holy ||
					g_Map.m_Npc[i].m_State.nPetState == nPetState_Pet ||
					IsSameNationUnit( (void *)(&g_Map.m_Npc[i]) ) )
				{
					continue;
				}
				
				if( g_Map.m_NpcTable[i].lUnique != -1 )				
				{
					
					if( g_Map.m_Npc[i].m_fDistance < min_dist ) 
					{
						if(m_NpcIndexList.size() != 0  )
						{
							sNpcIndexlist tmpNpcIndexlist;
							
							tmpNpcIndexlist.Distance = g_Map.m_Npc[i].m_fDistance;
							tmpNpcIndexlist.index  = i ; 
							
							m_NpcIndexList.push_back(tmpNpcIndexlist);

						}
					}
				}
			}		

			if( m_NpcIndexList.size() != 0 )
			{
#ifdef ADD_APPLYWIDE_MAP
				sort( m_NpcIndexList.begin() , m_NpcIndexList.end() , SortNpcIndexKeyList() );
#else
				qsort( (void *) & m_NpcIndexList[0] , (size_t) m_NpcIndexList.size() , sizeof(sNpcIndexlist) , SortNpcIndexList );
#endif // ADD_APPLYWIDE_MAP
			}

			//스택에 들어 있는 사이즈보다 크면 .. 
			if( indexCount > m_NpcIndexList.size() || indexCount > 4 ) 
			{
				indexCount = 0; 
			}
			
			if(m_NpcIndexList.size() != 0 )
			{

				for(int i =0 ; i != m_NpcIndexList.size() ; ++i)
				{
					PRINT_DLG_LOG("pcIndex %d Dist: %f", m_NpcIndexList[i].index , m_NpcIndexList[i].Distance);
				}

				npc_index = m_NpcIndexList[indexCount].index;
			}
		
		
			
			if( npc_index != -1 )
			{
				bFind = true;
				if( NonPlayerInfoWnd.thePickedNpcID != npc_index )
				{
					nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
					
					Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
					Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Map.m_NpcTable[ npc_index ].theLevel );
					Wnd->ReceiveEvent(n_emSetText, (EPARAM)strTemp, 240, 240, 200 );
					
					Wnd = Container->Get( WID_NonPlayerInfo_NameText );
					Wnd->ReceiveEvent(n_emSetText, (EPARAM)g_Map.m_NpcTable[ npc_index ].szName, 240, 240, 200 );			
					
					NonPlayerInfoWnd.thePickedUserID = -1;
					NonPlayerInfoWnd.thePickedNpcID = npc_index;
					
					NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Map.m_Npc[ npc_index ] );						
				}
			}		

			indexCount++;



			if(bFind == false)
			{
				nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				NonPlayerInfoWnd.thePickedUserID = -1;
				NonPlayerInfoWnd.thePickedNpcID = -1;
			}
		}
	}
#else 


	if( theKey[DIK_TAB].Action == n_baUp )
	{		
		// 부가효과 (강제타겟) 처리 
		if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke))
		{
			return S_OK;
		}
		
		float min_dist = 1000.0f;
		int npc_index = -1;
		int pc_index = -1;
	
		//대전장 .. 
		if( g_Pc.GetPlayer()->theMapType == n_FightZone )
		{
			for( int i = 0; i < MAX_USER_INTABLE; i ++ )
			{
				if( g_Pc.m_PcTable[i].nFlag == g_Pc.m_MainPC.nFlag ||
					g_Pc.m_Pc[i].m_bIsVisible == false ||
					g_Pc.m_Pc[i].m_curt_event.type == SM_STATE_DIE ||
					g_Pc.m_Pc[i].m_CharAlpha.State == CHAR_NONE_DISPLAY )
				{
					continue;
				}
				
				if( g_Pc.m_PcTable[i].lUnique != -1 )				
				{
					if( g_Pc.m_Pc[i].m_fDistance < min_dist )
					{
						min_dist = g_Pc.m_Pc[i].m_fDistance;
						pc_index = i;
					}
				}
			}		

			if( pc_index == -1 )
			{
				nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				NonPlayerInfoWnd.thePickedUserID = -1;
				NonPlayerInfoWnd.thePickedNpcID = -1;
			}
			else
			{
				if( g_Pc.m_PcTable[ pc_index ].lUnique != -1 )
				{			
					if( NonPlayerInfoWnd.thePickedUserID != pc_index )
					{
						nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
						
						Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
						Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Pc.m_PcTable[ pc_index ].char_info.theLevel );
						Wnd->ReceiveEvent( n_emSetText, (EPARAM) strTemp, 240, 240, 200 );			
						
						Wnd = Container->Get( WID_NonPlayerInfo_NameText );
						Wnd->ReceiveEvent( n_emSetText, (EPARAM) g_Pc.m_PcTable[ pc_index ].char_info.thePCName, 240, 240, 200 );			
						
						NonPlayerInfoWnd.thePickedUserID = pc_index;
						NonPlayerInfoWnd.thePickedNpcID = -1;
						
						NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Pc.m_Pc[ pc_index ] );
					}
				}
			}
		}
		//일반 필드
		else
		{				
			for( int i = 0; i < MAX_USER_INTABLE; i ++ )
			{
								
				if( !CanAttackPlayer( &(g_Pc.m_Pc[i]) ) ||
					g_Pc.m_Pc[i].m_bIsVisible == false ||
					g_Pc.m_Pc[i].m_curt_event.type == SM_STATE_DIE ||
					g_Pc.m_Pc[i].m_CharAlpha.State == CHAR_NONE_DISPLAY )
				{
					continue;
				}
				
				if( g_Pc.m_PcTable[i].lUnique != -1 )				
				{

					if( g_Pc.m_Pc[i].m_fDistance < min_dist )
					{
						min_dist = g_Pc.m_Pc[i].m_fDistance;
						pc_index = i;
					}
				}
			}

		
			bool bFind = false;

			if( pc_index != -1 )
			{
				if( g_Pc.m_PcTable[ pc_index ].lUnique != -1 )
				{			
					bFind = true;
					if( NonPlayerInfoWnd.thePickedUserID != pc_index )
					{
						nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
						
						Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
						Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Pc.m_PcTable[ pc_index ].char_info.theLevel );
						Wnd->ReceiveEvent( n_emSetText, (EPARAM) strTemp, 240, 240, 200 );			
						
						Wnd = Container->Get( WID_NonPlayerInfo_NameText );
						Wnd->ReceiveEvent( n_emSetText, (EPARAM) g_Pc.m_PcTable[ pc_index ].char_info.thePCName, 240, 240, 200 );			
						
						NonPlayerInfoWnd.thePickedUserID = pc_index;
						NonPlayerInfoWnd.thePickedNpcID = -1;
						
						NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Pc.m_Pc[ pc_index ] );					
					}
				}
			}			
		
			for( i = 0; i < MAX_NPC_INTABLE; i ++ )
			{
					if( g_Map.m_NpcTable[i].race == c_NpcRace_Item ||
					g_Map.m_Npc[i].m_bIsVisible == false ||
					g_Map.m_Npc[i].m_curt_event.type == SM_STATE_DIE ||
					g_Map.m_Npc[i].m_State.nRideState == nRideState_Ride ||
					g_Map.m_Npc[i].m_State.nHolyState == nHolyState_Holy ||
					g_Map.m_Npc[i].m_State.nPetState == nPetState_Pet ||
					IsSameNationUnit( (void *)(&g_Map.m_Npc[i]) ) )
				{
					continue;
				}
				
				if( g_Map.m_NpcTable[i].lUnique != -1 )				
				{
					if( g_Map.m_Npc[i].m_fDistance < min_dist )
					{
						min_dist = g_Map.m_Npc[i].m_fDistance;
						npc_index = i;
					}
				}
			}		
			
			if( npc_index != -1 )
			{
				bFind = true;
				if( NonPlayerInfoWnd.thePickedNpcID != npc_index )
				{
					nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
					
					Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
					Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Map.m_NpcTable[ npc_index ].theLevel );
					Wnd->ReceiveEvent(n_emSetText, (EPARAM)strTemp, 240, 240, 200 );
					
					Wnd = Container->Get( WID_NonPlayerInfo_NameText );
					Wnd->ReceiveEvent(n_emSetText, (EPARAM)g_Map.m_NpcTable[ npc_index ].szName, 240, 240, 200 );			
					
					NonPlayerInfoWnd.thePickedUserID = -1;
					NonPlayerInfoWnd.thePickedNpcID = npc_index;
					
					NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Map.m_Npc[ npc_index ] );						
				}
			}		

			if(bFind == false)
			{
				nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
				NonPlayerInfoWnd.thePickedUserID = -1;
				NonPlayerInfoWnd.thePickedNpcID = -1;
			}
		}
	}

#endif



/*
	if(theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_TAB].Action == n_baUp )
	{		
		// 부가효과 (강제타겟) 처리		  
		if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeProvoke))
		{
			return S_OK;
		}
		
		float min_dist = 1000.0f;
		int npc_index = 0;
		int pc_index = 0;

		if( g_Pc.GetPlayer()->theMapType == n_FightZone )
		{
			for( int i = 0; i < MAX_USER_INTABLE; i ++ )
			{
				if( g_Pc.m_PcTable[i].nFlag != g_Pc.m_MainPC.nFlag ||
					g_Pc.m_Pc[i].m_bIsVisible == false ||
					g_Pc.m_Pc[i].m_curt_event.type == SM_STATE_DIE )
				{
					continue;
				}
				
				if( g_Pc.m_PcTable[i].lUnique != -1 )				
				{
					if( g_Pc.m_Pc[i].m_fDistance < min_dist )
					{
						min_dist = g_Pc.m_Pc[i].m_fDistance;
						pc_index = i;
					}
				}
			}		

			if( g_Pc.m_PcTable[ pc_index ].lUnique != -1 )
			{			
				if( NonPlayerInfoWnd.thePickedUserID != pc_index )
				{
					nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				

					Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
					Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Pc.m_PcTable[ pc_index ].char_info.theLevel );
					Wnd->ReceiveEvent( n_emSetText, (EPARAM) strTemp, 240, 240, 200 );			

					Wnd = Container->Get( WID_NonPlayerInfo_NameText );
					Wnd->ReceiveEvent( n_emSetText, (EPARAM) g_Pc.m_PcTable[ pc_index ].char_info.thePCName, 240, 240, 200 );			
					
					NonPlayerInfoWnd.thePickedUserID = pc_index;
					
					NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Pc.m_Pc[ pc_index ] );
				}
			}
		}		
	}	
*/
	// Use Down 
	static DWORD pre_send_cell = 0;
	static BOOL bLButtonDown = FALSE;
	static long down_time = timeGetTime();
	static long attack_time = timeGetTime();

	// Use Press 
	static DWORD pick_cell = 0;		
	static DWORD send_cell = 0;		
		

	// 클릭하면 PC, NPC 선택, 선택한 대상에 대한 정보창이 뜨고, 
	// 더블 클릭시에 공격 또는 단축키를 통한 공격 및 스킬 사용 ^ ^/ 으쌰~~~ (인터페이스 되면 작업 시작) 		
		if( (nRui->GetCursorItem() == NULL && theMouse.LButton == n_baDown) &&
//		(nRui->GetCursorItem() == NULL && theMouse.LButton == n_baUp) &&
		(g_RockClient.GetApp() ==  ::GetActiveWindow()) )  	// 캐릭터 이동 처리 
	{
		bLButtonDown = TRUE;
		down_time = timeGetTime();
		
		RLGS( "theMouse.LButton == n_baDown" );

		///--By simwoosung 다음 공격일때 다른대상을 클릭불가
		if( g_Pc.GetPlayer()->m_next_event.type == SM_ATTACK_NPC )
		{
			///--PRINT_DLG_LOG( "return g_Pc.GetPlayer()->m_next_event.type == SM_ATTACK_NPC" );
			return S_OK;
		}
		else if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
		{
			if( g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING || 
				g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_SPELLING )
			{
			}
			else
			{
				///--PRINT_DLG_LOG( "return g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL" );
				return S_OK;
			}
		}
		
		if( theKey[DIK_LSHIFT].Action == n_baPress && g_Map.PHF_PickDest(m_hWnd) == S_OK )
		{
			static long change_time = false;

			if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( change_time, 1000 ) ) )
			{
				//by dongs 이동중중일떄는 쉬프트 키 를 막는다 .. 
				if( g_Pc.GetPlayer()->m_curt_event.type != SM_MOVE_BROADCAST )
				{
					g_Pc.GetPlayer()->SetDirection( (float)g_Map.GetPickedX()*CLIENT_TILESIZE, (float)g_Map.GetPickedY()*CLIENT_TILESIZE );
					Send_RpCsDirection( g_Pc.GetPlayer()->m_Mov.now_Yrad );	
				}

				change_time = g_nowTime;
			}		
		}
		/// 지속성 지역 타겟 스킬 사용			
		else if( m_MouseControlType == n_MouseType_UseSkill )
		{
			///--PRINT_DLG_LOG( "return n_MouseType_UseSkill" );
			
			if( g_Map.PHF_PickDest(m_hWnd) == S_OK && g_Particle.m_EffectManager.GetLandMarkType())
			{				
				g_Pc.GetPlayer()->Illusion_CsSkill( ( g_Map.GetPickedX() + g_Map.GetPickedY() * g_Pc.m_cSize ), n_Cell, 
												g_Pc.theSkillSystem.theTargetLand_SkillCode );
				
				m_MouseControlType = n_MouseType_Normal;
				g_Pc.theSkillSystem.theTargetLand_SkillCode = 0;				
				g_Particle.m_EffectManager.SetLandMark( FALSE, 0.0f );
								
				return S_OK; 
			}			
		}
		else if( g_Map.PickNpc() != -1 )					// 몬스터 선택 
		{	
			NonPlayerInfoWnd.thePickedUserID = -1;

			///-- 선택한 것이 아이템이 아니라면..
			if( g_Map.m_NpcTable[g_Map.m_nPickedNpc].race != c_NpcRace_Item )
			{
				if( NonPlayerInfoWnd.thePickedNpcID != g_Map.m_nPickedNpc )
				{
					nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
					Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
					Rsprintf( RWCHAR(strTemp), _RT("%d") , g_Map.m_NpcTable[g_Map.m_nPickedNpc].theLevel );
					Wnd->ReceiveEvent(n_emSetText, (EPARAM)strTemp, 240, 240, 200 );			
					Wnd = Container->Get( WID_NonPlayerInfo_NameText );
					Wnd->ReceiveEvent(n_emSetText, (EPARAM)g_Map.m_NpcTable[g_Map.m_nPickedNpc].szName, 240, 240, 200 );			

					NonPlayerInfoWnd.thePickedNpcID = g_Map.m_nPickedNpc;
					NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Map.m_Npc[g_Map.m_nPickedNpc] );


				}
				else
				{	
					// 부가효과 (공격불가) 처리
					if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack))
					{
						return S_OK;
					}

					///-- Skill 을 사용할 때 CM_MOVE_ATTACK 금지
					if( 
						(g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY && g_Pc.GetPlayer()->m_curt_event.tbl_Idx == g_Map.m_nPickedNpc) ||
						(g_Pc.GetPlayer()->m_save_event.type == SM_MOVE_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc) ||
						(g_Pc.GetPlayer()->m_curt_event.type == SM_BOW_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc) || 
						( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL &&
						g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_SITDOWN &&
						g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_KEEPING &&
						g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_STANDUP ) )	// 휴식 스킬 (유지)	
					{
						return S_OK;
					}

					// 선택한 NPC 가 신수일경우 공격하지 공격 하지 않음
					if( g_Map.m_Npc[g_Map.m_nPickedNpc].m_State.nHolyState == nHolyState_Holy )
					{
						return S_OK;
					}

					//펫은 공격하지 못한다.
					if( g_Map.m_Npc[g_Map.m_nPickedNpc].m_State.nPetState == nPetState_Pet )
					{
						return S_OK;
					}

					///-- 같은 국가의 유닛은 공격하지 못한다.
					if( IsSameNationUnit( (void *)(&g_Map.m_Npc[g_Map.m_nPickedNpc]) ) )
					{
						return S_OK;
					}

					int nRace = g_Pc.GetPlayer()->m_curt_event.race;
					int nIndex = g_Pc.GetPlayer()->m_curt_event.tbl_Idx;

					int nGapTime = c_PickSameAttack_DelayTime;

					if( nRace != c_NpcRace_Mob || nIndex != g_Map.m_nPickedNpc )
					{
						nGapTime = c_PickOtherAttack_DelayTime;
					}				

					// 콤보 아닌 공격은 짧은시간에 여러번 발생안됨 
					if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_PickAttack_DelayTime , nGapTime ) , > , g_nowTime ) )
					{
						return S_OK;
					}

					if( g_Pc.GetPlayer()->m_curt_event.type == CM_MOVE_ATTACK &&
						g_Pc.GetPlayer()->m_curt_event.unique == g_Map.m_NpcTable[g_Map.m_nPickedNpc].lUnique &&
						g_Pc.GetPlayer()->m_curt_event.cell == g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock )
					{
						return S_OK;
					}

					EVENT_DATA_INFO pickNPC_event;
					pickNPC_event.type = CM_MOVE_ATTACK;
					pickNPC_event.motion = n_Attack9_Basic0;
					pickNPC_event.tbl_Idx = g_Map.m_nPickedNpc;
					pickNPC_event.race	 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].race;
					pickNPC_event.index	 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].index;
					pickNPC_event.unique = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lUnique;
					pickNPC_event.x		 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock%(g_Map.m_cSize*2);
					pickNPC_event.y		 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock/(g_Map.m_cSize*2);
					pickNPC_event.vPos.x = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._41;
					pickNPC_event.vPos.y = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._42;
					pickNPC_event.vPos.z = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._43;
					pickNPC_event.cell = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock;

					if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
						g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY )
					{
						g_Pc.GetPlayer()->theTAtSave_Event = pickNPC_event;
					}
					else
					{											
						g_Pc.GetPlayer()->PushQueue( pickNPC_event, TRUE );
						g_Pc.GetPlayer()->PetSetAttackQueue( pickNPC_event, false );
					}
				}
			}
			else	//아이템 선택
			{
				// 부가효과 (공격불가) 처리				
				if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack))
				{
					return S_OK;
				}

				if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY && g_Pc.GetPlayer()->m_curt_event.tbl_Idx == g_Map.m_nPickedNpc ||
					g_Pc.GetPlayer()->m_save_event.type == SM_MOVE_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc ||
					g_Pc.GetPlayer()->m_curt_event.type == SM_BOW_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc )
				{
					return S_OK;
				}

				int nRace = g_Pc.GetPlayer()->m_curt_event.race;
				int nIndex = g_Pc.GetPlayer()->m_curt_event.tbl_Idx;

				int nGapTime = c_PickSameAttack_DelayTime;

				if( nRace != c_NpcRace_Item || nIndex != g_Map.m_nPickedNpc )
				{
					nGapTime = c_PickOtherAttack_DelayTime;
				}				

				// 콤보 아닌 공격은 짧은시간에 여러번 발생안됨 
				if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_PickAttack_DelayTime , nGapTime ) , > , g_nowTime ) )
				{
					return S_OK;
				}

				if( g_Pc.GetPlayer()->m_curt_event.type == CM_MOVE_ATTACK &&
					g_Pc.GetPlayer()->m_curt_event.unique == g_Map.m_NpcTable[g_Map.m_nPickedNpc].lUnique &&
					g_Pc.GetPlayer()->m_curt_event.cell == g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock )
				{
					return S_OK;
				}

				EVENT_DATA_INFO pickNPC_event;
				pickNPC_event.type = CM_MOVE_ATTACK;
				pickNPC_event.motion = n_Attack9_Basic0;
				pickNPC_event.tbl_Idx = g_Map.m_nPickedNpc;
				pickNPC_event.race	 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].race;
				pickNPC_event.index	 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].index;
				pickNPC_event.unique = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lUnique;
				pickNPC_event.x		 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock%(g_Map.m_cSize*2);
				pickNPC_event.y		 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock/(g_Map.m_cSize*2);
				pickNPC_event.vPos.x = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._41;
				pickNPC_event.vPos.y = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._42;
				pickNPC_event.vPos.z = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._43;
				pickNPC_event.cell = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock;				

				if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
					g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY )
				{
					g_Pc.GetPlayer()->theTAtSave_Event = pickNPC_event;
				}
				else
				{											
					g_Pc.GetPlayer()->PushQueue( pickNPC_event, TRUE );
					g_Pc.GetPlayer()->PetSetAttackQueue( pickNPC_event, false );
				}
			}
		}
		else if( g_Pc.PickUser() != -1 )			// 캐릭터 선택 
		{	
			NonPlayerInfoWnd.thePickedNpcID = -1;
			
			if( g_Pc.m_nPickedUser < MAX_USER_INTABLE ) 
			{
				if( NonPlayerInfoWnd.thePickedUserID != g_Pc.m_nPickedUser )
				{
					nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
					Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
					Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.theLevel );
					Wnd->ReceiveEvent(n_emSetText, (EPARAM)strTemp, 240, 240, 200 );		
					Wnd = Container->Get( WID_NonPlayerInfo_NameText );
					Wnd->ReceiveEvent(n_emSetText, (EPARAM)g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.thePCName, 240, 240, 200 );				

					NonPlayerInfoWnd.thePickedUserID = g_Pc.m_nPickedUser;				
					NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Pc.m_Pc[g_Pc.m_nPickedUser] );

				}
				// 클베때는 PK 안되게 막는다 ^ ^;;
				else
				{						
					
					///-- 이부분은 개인 상점일 경우
					//.............................................................................................
					// 유저가 개인 상점 개설중에 있으면 사기창 열기 이미 열려 있으면 무시 By wxywxy
					//.............................................................................................
					if( g_Pc.m_Pc[g_Pc.m_nPickedUser].m_curt_event.status == SM_SELLSKILL_SITDOWN ||
						g_Pc.m_Pc[g_Pc.m_nPickedUser].m_curt_event.status == SM_SELLSKILL_KEEPING ||
						g_Pc.m_Pc[g_Pc.m_nPickedUser].m_curt_event.status == SM_SELLSKILL_STANDUP  	 )
					{
						if( g_UserStoreBuyWnd.ISUserBuyMode() )
						{
							return S_OK;
						}

						RLGS( "---- Open Buy Window ----" );
						EVENT_DATA_INFO pickPC_event;
						pickPC_event.type	= CM_MOVE_ATTACK;
						pickPC_event.motion = n_Attack9_Basic0;
						pickPC_event.tbl_Idx = g_Pc.m_nPickedUser;
						pickPC_event.race	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.race;
						pickPC_event.index	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].nIndex;
						pickPC_event.unique	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lUnique;
						pickPC_event.x		 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock%(g_Map.m_cSize*2);
						pickPC_event.y		 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock/(g_Map.m_cSize*2);
						pickPC_event.vPos.x = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._41;
						pickPC_event.vPos.y = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._42;
						pickPC_event.vPos.z = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._43;
						pickPC_event.cell = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock;
						g_Pc.GetPlayer()->PushQueue( pickPC_event, TRUE );
						g_Pc.GetPlayer()->PetSetAttackQueue( pickPC_event, false );

						attack_time = g_nowTime;

						return S_OK;
					}
					
					// 부가효과 (공격불가) 처리 
					if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack))
					{
						return S_OK;
					}

					if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY && g_Pc.GetPlayer()->m_curt_event.tbl_Idx == g_Pc.m_nPickedUser ||
						g_Pc.GetPlayer()->m_save_event.type == SM_MOVE_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Pc.m_nPickedUser ||
						g_Pc.GetPlayer()->m_curt_event.type == SM_BOW_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Pc.m_nPickedUser ) 
					{
						return S_OK;
					}

					int nRace = g_Pc.GetPlayer()->m_curt_event.race;
					int nIndex = g_Pc.GetPlayer()->m_curt_event.tbl_Idx;

					int nGapTime = c_PickSameAttack_DelayTime;

					if( nRace != g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.race  || nIndex != g_Pc.m_nPickedUser )
					{
						nGapTime = c_PickOtherAttack_DelayTime;
					}				

					// 콤보 아닌 공격은 짧은시간에 여러번 발생안됨 
					if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_PickAttack_DelayTime , nGapTime ) , > , g_nowTime ) )
					{
						return S_OK;
					}

					if( g_Pc.GetPlayer()->m_curt_event.type == CM_MOVE_ATTACK &&
						g_Pc.GetPlayer()->m_curt_event.unique == g_Pc.m_PcTable[g_Pc.m_nPickedUser].lUnique &&
						g_Pc.GetPlayer()->m_curt_event.cell == g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock )
					{
						return S_OK;
					}

					// 유료 근접 아이템을 든경우 때려 Boa요 by wxywxy
					if( g_Pc.GetPlayer()->m_ISApproachWeapon )
					{
						switch( g_Pc.m_Pc[g_Pc.m_nPickedUser].m_curt_event.status )
						{
							case SM_SELLSKILL_SITDOWN:
							case SM_SELLSKILL_KEEPING:
							case SM_SELLSKILL_STANDUP:
								return S_OK;
						}
						
					}
					// PVP Zone 이 아닐때는 같은 종족끼리는 전투 불가 
					else if( g_Pc.GetPlayer()->theMapType == n_FightZone )
					{
						if( g_Pc.m_MainPC.nFlag == g_Pc.m_PcTable[g_Pc.m_nPickedUser].nFlag )
						{
							return S_OK;
						}
					}
					else
					{
						if( !CanAttackPlayer( &g_Pc.m_Pc[g_Pc.m_nPickedUser] ) )
						{
							return S_OK;
						}
					}
					
					EVENT_DATA_INFO pickPC_event;
					pickPC_event.type	= CM_MOVE_ATTACK;
					pickPC_event.motion = n_Attack9_Basic0;
					pickPC_event.tbl_Idx = g_Pc.m_nPickedUser;
					pickPC_event.race	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.race;
					pickPC_event.index	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].nIndex;
					pickPC_event.unique	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lUnique;
					pickPC_event.x		 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock%(g_Map.m_cSize*2);
					pickPC_event.y		 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock/(g_Map.m_cSize*2);
					pickPC_event.vPos.x = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._41;
					pickPC_event.vPos.y = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._42;
					pickPC_event.vPos.z = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._43;
					pickPC_event.cell = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock;

					if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
						g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY )
					{
						g_Pc.GetPlayer()->theTAtSave_Event = pickPC_event;
					}
					else
					{											
						g_Pc.GetPlayer()->PushQueue( pickPC_event, TRUE );
						g_Pc.GetPlayer()->PetSetAttackQueue( pickPC_event, false );
					}				
				}

			}
		}	
		/*-------------------------------------------------------------------------
			캐릭터 이동관련 사용자 인풋 & 메세지 처리 정리 
		---------------------------------------------------------------------------
			<사용자 입력 처리> 
			- 정상 프로세스 : 피킹 -> theInputedDestCell에 저장 -> Cs_StartMove 전송 -> SetWaitingSvrMsg()
							Sc_StartMove 수신 -> theInputedDestCell로 이동 시작 -> theInputedDestCell 초기화 
			- 피킹에서 갈 수 있는 곳인지 아닌지, 길찾기 path가 유효한지 여부를 미리 검사 해야 한다. 
			- Cs_StartMove를 날리고 아직 응답을 받지 않은 상태에서 들어오는 마우스 입력은 무시 
			- 이동중 입력을 받은 경우 : 일단 그냥 정상 프로세스랑 똑같이 해보자. 많이 튀면 바꿔야지. -_-;

			<목적지 성격에 따른 처리>
			- 빈타일 -> 일반 이동 
			- PC -> 근처까지 이동 : PK 모드이면 공격 
			- NPC -> 근처까지 이동 : 기능성 있는 NPC이면 창 열기 
			- Monster -> 근처까지 이동 : 공격하기 
			- Item -> 근처까지 이동 : 줍기 

		--------------------------------------------------------------------------*/		
		///-- 지형 픽킹 루틴
		else if( theKey[DIK_LSHIFT].Action != n_baPress && g_Map.PHF_PickDest(m_hWnd) == S_OK )
		{
			// 부가효과 (이동불가) 처리 
			if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotMove))
			{
				///--PRINT_DLG_LOG( "return g_Pc.GetPlayer()->theAddEffect_State[n_TypeNotMove]" );
				return S_OK;
			}

			/// [To현배]
			/// 여기서 피킹된 셀이 갈 수 있는 곳인지 아닌지 판단 필요. 
			/// 길찾기도 한번 돌려봐서 목적지까지 유효한 path가 존재하는지도 확인 필요.
			/// 목적지 성격이 무엇인가에 따른 적절한 처리를 위한 준비도 필요... 
    
			//..................................................................................................
			// 피킹 부분 이펙트  By wxywxy
			//..................................................................................................
			if( g_Map.GetPickedX() != OldPickX ||
				g_Map.GetPickedY() != OldPickZ   )
			{
				
				g_Particle.DelEffSetCon( EF_PICK , 100  , -1 , -1 );

				float fHeight = g_Map.Get__MapHeight( g_Map.GetPickedX() , g_Map.GetPickedY() );
			
				float x = ( g_Map.GetPickedX() * CLIENT_TILESIZE ) + ( CLIENT_TILESIZE / 2 );
				float z = ( g_Map.GetPickedY() * CLIENT_TILESIZE ) + ( CLIENT_TILESIZE / 2 );
				
				D3DXVECTOR3 vPos( x , fHeight , z );

				if( 0 == g_Map.m_Block[g_Map.GetPickedX()][g_Map.GetPickedY()].Attr &&
					g_Map.IsNoGiantUnit( vPos ) )
				{
					g_Particle.SetEffSetCon( 0.0f , EF_PICK  , 100 , EFFECT_SET_LOOP_TIME , -1 , -1 , vPos );
				}
				else
				{
					g_Particle.SetEffSetCon( 0.0f , EF_PICK_X , 100 , 1.2f , -1 , -1 , vPos );
					return S_OK;
				}
			}

			DWORD dest_cell = 0;

			dest_cell = g_Map.GetPickedX() + ( g_Map.GetPickedY() * g_Pc.m_cSize );		

			if( pre_send_cell == dest_cell )
			{
				///--PRINT_DLG_LOG( "return pre_send_cell = %d == dest_cell = %d", pre_send_cell, dest_cell );
				return S_OK;
			}

			//..................................................................................................

			if( ( g_Map.m_pAttrMap[g_Map.GetPickedY()][g_Map.GetPickedX()].byAttr != MATTR_OBJ ) &&
				( g_Map.m_Block[g_Map.GetPickedX()][g_Map.GetPickedY()].GetClear() ) )
			{
				pre_send_cell = dest_cell;
				
				g_Pc.GetPlayer()->SetInputedDestCell( dest_cell );				
				
				// Master가 이동하면 Illusion도 이동한다. 
				g_Pc.GetPlayer()->Illusion_CsStartMove( dest_cell );				

				RLG2( "ID = %d, DestCell = %d", g_Pc.m_MainPC.lUnique, g_Pc.GetPlayer()->GetInputedDestCell() );

				g_Pc.GetPlayer()->m_save_event.Clear();		
				g_Pc.GetPlayer()->theAttack_Event.Clear();
				
				g_Pc.GetPlayer()->Pet_CsStartMove( dest_cell );

				if( g_Pc.GetPlayer()->m_curt_event.type == SM_BOW_ATTACK ||
					g_Pc.GetPlayer()->m_curt_event.type	== SM_BOW_READY   )
				{
					g_Pc.GetPlayer()->m_curt_event.Clear();
					
					g_ArcherBar.End();
				}
				else if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL &&
						 g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_SPELLING )
				{
					g_Pc.GetPlayer()->m_curt_event.Clear();
					g_Pc.GetPlayer()->m_SpellEffect.Clear();
					nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );					
				}							
			}
			else
			{  
				///--PRINT_DLG_LOG( "return 지형에 블록있음" );
			}
		} 		
	}
	else if( ( nRui->GetCursorItem() == NULL && theMouse.LButton == n_baPress ) &&
			 ( g_RockClient.GetApp() == ::GetActiveWindow() )	  	// 캐릭터 이동 처리 
		   ) 	
	{		
		static long move_time = timeGetTime();

		if( bLButtonDown )
		{
			if( SAFE_TIME_COMPARE( timeGetTime() , > , SAFE_TIME_ADD( down_time , 350 ) ) )
			{
				bLButtonDown = FALSE;
			}			
			return S_OK;
		}

		if( SAFE_TIME_COMPARE( timeGetTime() , > , SAFE_TIME_ADD( move_time , 150 ) ) )
		{
			move_time = timeGetTime();

			if( g_Pc.GetPlayer()->m_next_event.type == SM_ATTACK_NPC )
			{
				return S_OK;
			}
			else if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
			{
				if( g_Pc.GetPlayer()->m_curt_event.status == SM_RESTSKILL_KEEPING || 
					g_Pc.GetPlayer()->m_curt_event.status == SM_CHANT_SPELLING )
				{
				}
				else
				{
					return S_OK;
				}
			}
			
			if( g_Map.PickNpc() != -1 )					// 몬스터 선택 
			{	
			
				NonPlayerInfoWnd.thePickedUserID = -1;
				
				if( g_Map.m_NpcTable[g_Map.m_nPickedNpc].race != c_NpcRace_Item )
				{
					if( NonPlayerInfoWnd.thePickedNpcID != g_Map.m_nPickedNpc )
					{
						nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
						Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
						Rsprintf( RWCHAR(strTemp), _RT("%d") , g_Map.m_NpcTable[g_Map.m_nPickedNpc].theLevel );
						Wnd->ReceiveEvent(n_emSetText, (EPARAM)strTemp, 240, 240, 200 );			
						Wnd = Container->Get( WID_NonPlayerInfo_NameText );
						Wnd->ReceiveEvent(n_emSetText, (EPARAM)g_Map.m_NpcTable[g_Map.m_nPickedNpc].szName, 240, 240, 200 );			
						
						NonPlayerInfoWnd.thePickedNpcID = g_Map.m_nPickedNpc;
						NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Map.m_Npc[g_Map.m_nPickedNpc] );
					}
					else
					{	
						// 부가효과 (공격불가) 처리 
						if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack))
						{
							return S_OK;
						}
						///-- Skill 을 사용할 때 CM_MOVE_ATTACK 금지
						if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY && g_Pc.GetPlayer()->m_curt_event.tbl_Idx == g_Map.m_nPickedNpc ||
							g_Pc.GetPlayer()->m_save_event.type == SM_MOVE_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc ||
							g_Pc.GetPlayer()->m_curt_event.type == SM_BOW_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc || 
							( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL &&
							g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_SITDOWN &&
							g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_KEEPING &&
							g_Pc.GetPlayer()->m_curt_event.status != SM_RESTSKILL_STANDUP ) )	// 휴식 스킬 (유지)	
						{
							return S_OK;
						}
						
						// 선택한 NPC 가 신수일경우 공격하지 공격 하지 않음
						if( g_Map.m_Npc[g_Map.m_nPickedNpc].m_State.nHolyState == nHolyState_Holy )
						{
							return S_OK;
						}

						//펫은 공격하지 못한다.
						if( g_Map.m_Npc[g_Map.m_nPickedNpc].m_State.nPetState == nPetState_Pet )
						{
							return S_OK;
						}

						/// 선택한 NPC가 같은국가의 유닛이면 공격하지 않음
						if( IsSameNationUnit( (void *)(&g_Map.m_Npc[g_Map.m_nPickedNpc]) ) )
						{
							return S_OK;
						}

						int nRace = g_Pc.GetPlayer()->m_curt_event.race;
						int nIndex = g_Pc.GetPlayer()->m_curt_event.tbl_Idx;

						int nGapTime = c_PickSameAttack_DelayTime;

						if( nRace != c_NpcRace_Mob || nIndex != g_Map.m_nPickedNpc )
						{
							nGapTime = c_PickOtherAttack_DelayTime;
						}				

						// 콤보 아닌 공격은 짧은시간에 여러번 발생안됨 
						if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_PickAttack_DelayTime , nGapTime ) , > , g_nowTime ) )
						{
							return S_OK;
						}

						if( g_Pc.GetPlayer()->m_curt_event.type == CM_MOVE_ATTACK &&
							g_Pc.GetPlayer()->m_curt_event.unique == g_Map.m_NpcTable[g_Map.m_nPickedNpc].lUnique &&
							g_Pc.GetPlayer()->m_curt_event.cell == g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock )
						{
							return S_OK;
						}						
						
						EVENT_DATA_INFO pickNPC_event;
						pickNPC_event.type = CM_MOVE_ATTACK;
						pickNPC_event.motion = n_Attack9_Basic0;
						pickNPC_event.tbl_Idx = g_Map.m_nPickedNpc;
						pickNPC_event.race	 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].race;
						pickNPC_event.index	 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].index;
						pickNPC_event.unique = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lUnique;
						pickNPC_event.x		 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock%(g_Map.m_cSize*2);
						pickNPC_event.y		 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock/(g_Map.m_cSize*2);
						pickNPC_event.vPos.x = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._41;
						pickNPC_event.vPos.y = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._42;
						pickNPC_event.vPos.z = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._43;
						pickNPC_event.cell = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock; 

						if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
							g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY )
						{
							g_Pc.GetPlayer()->theTAtSave_Event = pickNPC_event;
						}
						else
						{											
							g_Pc.GetPlayer()->PushQueue( pickNPC_event, TRUE );
							g_Pc.GetPlayer()->PetSetAttackQueue( pickNPC_event, false );
						}							
					}
				}
				else
				{
					// 부가효과 (공격불가) 처리
					if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack))
					{
						return S_OK;
					}

					if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY && g_Pc.GetPlayer()->m_curt_event.tbl_Idx == g_Map.m_nPickedNpc ||
						g_Pc.GetPlayer()->m_save_event.type == SM_MOVE_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc ||
						g_Pc.GetPlayer()->m_curt_event.type == SM_BOW_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Map.m_nPickedNpc )
					{
						return S_OK;					
					}

					int nRace = g_Pc.GetPlayer()->m_curt_event.race;
					int nIndex = g_Pc.GetPlayer()->m_curt_event.tbl_Idx;

					int nGapTime = c_PickSameAttack_DelayTime;

					if( nRace != c_NpcRace_Item || nIndex != g_Map.m_nPickedNpc )
					{
						nGapTime = c_PickOtherAttack_DelayTime;
					}				

					// 콤보 아닌 공격은 짧은시간에 여러번 발생안됨 
					if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_PickAttack_DelayTime , nGapTime ) , > , g_nowTime ) )
					{
						return S_OK;
					}

					if( g_Pc.GetPlayer()->m_curt_event.type == CM_MOVE_ATTACK &&
						g_Pc.GetPlayer()->m_curt_event.unique == g_Map.m_NpcTable[g_Map.m_nPickedNpc].lUnique &&
						g_Pc.GetPlayer()->m_curt_event.cell == g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock )
					{
						return S_OK;
					}
					
					EVENT_DATA_INFO pickNPC_event;
					pickNPC_event.type = CM_MOVE_ATTACK;
					pickNPC_event.motion = n_Attack9_Basic0;
					pickNPC_event.tbl_Idx = g_Map.m_nPickedNpc;
					pickNPC_event.race	 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].race;
					pickNPC_event.index	 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].index;
					pickNPC_event.unique = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lUnique;
					pickNPC_event.x		 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock%(g_Map.m_cSize*2);
					pickNPC_event.y		 = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock/(g_Map.m_cSize*2);
					pickNPC_event.vPos.x = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._41;
					pickNPC_event.vPos.y = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._42;
					pickNPC_event.vPos.z = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._43;
					pickNPC_event.cell = g_Map.m_NpcTable[g_Map.m_nPickedNpc].lDestBlock; 

					if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
						g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY )
					{
						g_Pc.GetPlayer()->theTAtSave_Event = pickNPC_event;
					}
					else
					{											
						g_Pc.GetPlayer()->PushQueue( pickNPC_event, TRUE );
						g_Pc.GetPlayer()->PetSetAttackQueue( pickNPC_event, false );
					}
				}

				return S_OK;
			}
			else if( g_Pc.PickUser() != -1 )			// 캐릭터 선택 
			{	
							
				NonPlayerInfoWnd.thePickedNpcID = -1;
				
				if( g_Pc.m_nPickedUser < MAX_USER_INTABLE ) 
				{
					if( NonPlayerInfoWnd.thePickedUserID != g_Pc.m_nPickedUser )
					{
						nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
						Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
						Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.theLevel );
						Wnd->ReceiveEvent(n_emSetText, (EPARAM)strTemp, 240, 240, 200 );		
						Wnd = Container->Get( WID_NonPlayerInfo_NameText );
						Wnd->ReceiveEvent(n_emSetText, (EPARAM)g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.thePCName, 240, 240, 200 );				
						
						NonPlayerInfoWnd.thePickedUserID = g_Pc.m_nPickedUser;				
						NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Pc.m_Pc[g_Pc.m_nPickedUser] );
						
					}
					// 클베때는 PK 안되게 막는다 ^ ^;;
					else
					{						
						//.............................................................................................
						// 유저가 개인 상점 개설중에 있으면 사기창 열기 이미 열려 있으면 무시 By wxywxy
						//.............................................................................................
						if( g_Pc.m_Pc[g_Pc.m_nPickedUser].m_curt_event.status == SM_SELLSKILL_SITDOWN ||
							g_Pc.m_Pc[g_Pc.m_nPickedUser].m_curt_event.status == SM_SELLSKILL_KEEPING ||
							g_Pc.m_Pc[g_Pc.m_nPickedUser].m_curt_event.status == SM_SELLSKILL_STANDUP  	 )
						{
							if( g_UserStoreBuyWnd.ISUserBuyMode() )
							{
								return S_OK;
							}
							
							RLGS( "---- Open Buy Window ----" );
							//Send_CsInPCShop( g_Pc.m_Pc[g_Pc.m_nPickedUser].GetUnique() );
							EVENT_DATA_INFO pickPC_event;
							pickPC_event.type	= CM_MOVE_ATTACK;
							pickPC_event.motion = n_Attack9_Basic0;
							pickPC_event.tbl_Idx = g_Pc.m_nPickedUser;
							pickPC_event.race	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.race;
							pickPC_event.index	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].nIndex;
							pickPC_event.unique	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lUnique;
							pickPC_event.x		 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock%(g_Map.m_cSize*2);
							pickPC_event.y		 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock/(g_Map.m_cSize*2);
							pickPC_event.vPos.x = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._41;
							pickPC_event.vPos.y = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._42;
							pickPC_event.vPos.z = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._43;
							pickPC_event.cell = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock;
							g_Pc.GetPlayer()->PushQueue( pickPC_event, TRUE );
							g_Pc.GetPlayer()->PetSetAttackQueue( pickPC_event, false );

							attack_time = g_nowTime;
							
							return S_OK;
						}
						
						// 부가효과 (공격불가) 처리
						if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack))
						{
							return S_OK;
						}

						if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY && g_Pc.GetPlayer()->m_curt_event.tbl_Idx == g_Pc.m_nPickedUser ||
							g_Pc.GetPlayer()->m_save_event.type == SM_MOVE_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Pc.m_nPickedUser ||
							g_Pc.GetPlayer()->m_curt_event.type == SM_BOW_ATTACK && g_Pc.GetPlayer()->m_save_event.tbl_Idx == g_Pc.m_nPickedUser ) 
						{
							return S_OK;
						}
						
						int nRace = g_Pc.GetPlayer()->m_curt_event.race;
						int nIndex = g_Pc.GetPlayer()->m_curt_event.tbl_Idx;

						int nGapTime = c_PickSameAttack_DelayTime;

						if( nRace != g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.race || nIndex != g_Pc.m_nPickedUser )
						{
							nGapTime = c_PickOtherAttack_DelayTime;
						}				

						// 콤보 아닌 공격은 짧은시간에 여러번 발생안됨 
						if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_PickAttack_DelayTime , nGapTime ) , > , g_nowTime ) )
						{
							return S_OK;
						}

						if( g_Pc.GetPlayer()->m_curt_event.type == CM_MOVE_ATTACK &&
							g_Pc.GetPlayer()->m_curt_event.unique == g_Pc.m_PcTable[g_Pc.m_nPickedUser].lUnique && 
							g_Pc.GetPlayer()->m_curt_event.cell == g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock )
						{
							return S_OK;
						}

						// 유료 근접 아이템을 든경우 때려 Boa요 by wxywxy
						if( g_Pc.GetPlayer()->m_ISApproachWeapon )
						{
							switch( g_Pc.m_Pc[ g_Pc.m_nPickedUser].m_curt_event.status )
							{
								case SM_SELLSKILL_SITDOWN:
								case SM_SELLSKILL_KEEPING:
								case SM_SELLSKILL_STANDUP:
									return S_OK;
							}
						}
						// PVP Zone 이 아닐때는 같은 종족끼리는 전투 불가 
						else if( g_Pc.GetPlayer()->theMapType == n_FightZone )
						{
							if( g_Pc.m_MainPC.nFlag == g_Pc.m_PcTable[g_Pc.m_nPickedUser].nFlag )
							{
								return S_OK;
							}		
						}
						else
						{
							if( !CanAttackPlayer( &g_Pc.m_Pc[g_Pc.m_nPickedUser] ) )
							{
								return S_OK;
							}
						}
						
						EVENT_DATA_INFO pickPC_event;
						pickPC_event.type	= CM_MOVE_ATTACK;
						pickPC_event.motion = n_Attack9_Basic0;
						pickPC_event.tbl_Idx = g_Pc.m_nPickedUser;
						pickPC_event.race	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.race;
						pickPC_event.index	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].nIndex;
						pickPC_event.unique	 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lUnique;
						pickPC_event.x		 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock%(g_Map.m_cSize*2);
						pickPC_event.y		 = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock/(g_Map.m_cSize*2);
						pickPC_event.vPos.x = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._41;
						pickPC_event.vPos.y = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._42;
						pickPC_event.vPos.z = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._43;
						pickPC_event.cell = g_Pc.m_PcTable[g_Pc.m_nPickedUser].lDestBlock;

						if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
							g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY )
						{
							g_Pc.GetPlayer()->theTAtSave_Event = pickPC_event;
						}
						else	
						{											
							g_Pc.GetPlayer()->PushQueue( pickPC_event, TRUE );
							g_Pc.GetPlayer()->PetSetAttackQueue( pickPC_event, false );
						}
					}					
				}
      
				return S_OK;
			}
			else if( theKey[DIK_LSHIFT].Action != n_baPress && g_Map.PHF_PickDest( m_hWnd ) == S_OK )
			{
				// 부가효과 (이동불가) 처리 
				if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotMove) || g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotMove) )
				{
					return S_OK;
				}
				
				DWORD dest_cell = 0;	
				dest_cell = g_Map.GetPickedX() + ( g_Map.GetPickedY() * g_Pc.m_cSize );					
				
				if( pick_cell == dest_cell )
				{
					return S_OK;
				}
				
				if( g_Map.GetPickedX() != OldPickX ||
					g_Map.GetPickedY() != OldPickZ   )
				{
					g_Particle.DelEffSetCon( EF_PICK , 100  , -1 , -1 );
					
					float fHeight = g_Map.Get__MapHeight( g_Map.GetPickedX() , g_Map.GetPickedY() );
					
					float x = ( g_Map.GetPickedX() * CLIENT_TILESIZE ) + ( CLIENT_TILESIZE / 2 );
					float z = ( g_Map.GetPickedY() * CLIENT_TILESIZE ) + ( CLIENT_TILESIZE / 2 );
					
					D3DXVECTOR3 vPos( x , fHeight , z );
					
					if( 0 == g_Map.m_Block[g_Map.GetPickedX()][g_Map.GetPickedY()].Attr &&
						g_Map.IsNoGiantUnit(vPos) )
					{
						g_Particle.SetEffSetCon( 0.0f , EF_PICK  , 100 , EFFECT_SET_LOOP_TIME , -1 , -1 , vPos );
					}
					else
					{
						g_Particle.SetEffSetCon( 0.0f , EF_PICK_X , 100 , 1.2f , -1 , -1 , vPos );
						return S_OK;
					}
				}
				
				g_Map.SetMapData( CLEAR_BLOCK, MAX_USER_INTABLE, g_Pc.m_MainPC.lDestBlock, true );						
				g_Pc.SetTile();					

				// 이동하면 저장된 이동 공격을 취소 시킨다. 
				g_Pc.GetPlayer()->m_save_event.Clear();
				
				// 클라이언트에서 맘대로 움직이는 위치를 일단 모두 저장 
				pick_cell = dest_cell;
			}
		}
	}

	if( pick_cell )
	{
		// 일단 젤루 간단하게 움직이기만 하면 1셀이상 차이가 난다면 pick_cell 로 이동되어야 한다. 
		int pre_xpos = 0;
		int pre_ypos = 0;
		int curt_xpos = 0;
		int curt_ypos = 0;
		int real_range = 0; 
		int skip_range = 10; 
		

		// 서버에 보내진 위치와 현재 위치를 계산
		pre_xpos = send_cell % g_Pc.m_cSize;
		pre_ypos = send_cell / g_Pc.m_cSize;
		curt_xpos = g_Pc.GetPlayer()->m_Mov.curt.x;
		curt_ypos = g_Pc.GetPlayer()->m_Mov.curt.y;
		real_range = GetMax( abs( pre_xpos - curt_xpos ), abs( pre_ypos - curt_ypos ) );


		// 가야될 위치가 멀때 처리 
		if( real_range > 8 )
		{
			// 서버에 보내진 위치와 갈 위치를 계산 
			pre_xpos = send_cell % g_Pc.m_cSize;
			pre_ypos = send_cell / g_Pc.m_cSize;
			curt_xpos = pick_cell % g_Pc.m_cSize;
			curt_ypos = pick_cell / g_Pc.m_cSize;	
			skip_range = GetMax( abs( pre_xpos - curt_xpos ), abs( pre_ypos - curt_ypos ) );
		}

		// 서버에 보내진 위치와 현재 위치가 1셀이라도 차이가 있다면 보낸다. 
		if( real_range ) 
		{
			if( skip_range > 4 )
			{
				
				g_Pc.GetPlayer()->m_save_event.Clear();
				g_Pc.GetPlayer()->theAttack_Event.Clear();
				
				g_Pc.GetPlayer()->SetInputedDestCell( pick_cell );					
				g_Pc.GetPlayer()->Illusion_CsStartMove( pick_cell );

				g_Pc.GetPlayer()->Pet_CsStartMove( pick_cell );	
				send_cell = pick_cell;	
				pick_cell = 0;	
			}
		}
	}

	return S_OK;
}


int KeyValueList[] = { DIK_0, DIK_1, DIK_2, DIK_3, DIK_4, DIK_5, DIK_6, DIK_7, DIK_8, DIK_9 };

  
///---------------------------------------------------------------------------
///-- Main
///---------------------------------------------------------------------------


HRESULT		CRockClient::Main()
{
	BOOL bCloseGame = FALSE;
	BOOL bCheckFullGame = FALSE;

#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)
		#ifdef INDONESIA_VERSION		
			///-- 핵쉴드를 제공하지 않은 경우에 
			///-- 강제로 윈도우 모드로 전환시킬 경우
			bCheckFullGame = TRUE;
			if(m_bWindowed == TRUE)
			{
				bCloseGame = TRUE;
			}
		#endif	
	#endif   

/*
#if !defined(_DEBUG) && !defined(TEST_REALEASE_CLIENT)
	///-- 포커스가 다른윈도우에 잡히면...
	if( bCheckFullGame && g_RockClient.GetApp() != ::GetActiveWindow() )
	{
		SendMessage( g_RockClient.GetApp(), WM_CLOSE, NULL, 1 );
	}	
#endif		
*/		

	if(bCloseGame)
	{
		TCHAR szMsg[255] = "";
		Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_FILE_CHANGE_CRC_ERROR) , 1 );
		SEND_DEBUG_LOG(szMsg);
		Sleep(50);

		ClientClose( g_RockClient.GetApp() , 1);
	}
			
	//마우스가 안움직이게 되는경우를 위해 처리 - 임시코드
	if( m_pd3dDevice  )
	{
		POINT ptCursor; 
		GetCursorPos( &ptCursor ); 
		if( !m_d3dpp.Windowed && !m_bDeviceLost )
		{
			ScreenToClient( m_hWnd, &ptCursor );
			g_RenderManager.SetCursorPosition( ptCursor.x, ptCursor.y, 0 ); 	
		}
	}	
	 
	if( GetGameMode() == GMODE_INITLOAD )
		return S_OK;

	//////////////////////////////////////////////////////////////////////////
	//nProtect 실행중인지 체크한다 .. 
	//////////////////////////////////////////////////////////////////////////
#ifdef C_GAME_GUARD
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_dwNewTimeHack ) , >= , 10000 ) )
	{	

		m_dwNewTimeHack = g_nowTime;
		
		if( g_RockClient.pNpgl->Check() != NPGAMEMON_SUCCESS ) 
		{
			ClientClose( g_RockClient.GetApp() , 1);
		}
	}

#endif 
   
	//	g_nowTime = timeGetTime();	/// 현재시간저장
	//-------------------------------------------------------------------------

	Inputg->AcquireEvent( theKey, &theMouse );// DxInput을 이용해서 키/마우스 상태값 가져오기
	g_FocusWnd = nRui->IsFocus( theMouse.Aposition.x, theMouse.Aposition.y );

	static bool bIsMoveWnd = false;
	static bool bIsMoveCamera = false;

	
	if( (g_FocusWnd != WID_None) && 		
		(g_FocusWnd != WID_ChatActFrame) &&
		(g_FocusWnd != WID_ChatNonActFrame) &&
		(g_FocusWnd != WID_SystemPrintFrame) 

#ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)
		
		&& (GetGameMode() != GMODE_SELECT)
		
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 기존 선택된 해상도(2009/04/27)

		)
	{
		theMouse.LButton = n_baNone;

		if( bIsMoveCamera == false )
		{
			theMouse.RButton = n_baNone;
		}
		
		g_PressTime -= 100000;

		if( nRui->GetEMsg()->Mouse.Action == MA_LDRAG )
		{
			bIsMoveWnd = true;
		}
	}
	else
	{
		// 카메라 이동중 윈도우 메세지 처리 무시 
		if( theMouse.RButton == n_baPress )
		{
			bIsMoveCamera = true;
		}	
		else if( theMouse.RButton == n_baUp )
		{
			bIsMoveCamera = false;
		}	
	}


	// 윈도우 이동중 캐릭터 이동 불가 처리 
	if( bIsMoveWnd == true )
	{
		if( theMouse.LButton == n_baPress )
		{
			theMouse.LButton = n_baNone;		
		}
		else if( theMouse.LButton == n_baUp )
		{
			theMouse.LButton = n_baNone;		

			bIsMoveWnd = false;
		}
	}
	

	// UI창들에 대한 키/마우스 이벤트 처리
	N_EVENTRESULT EventResult = nRui->EventProc( theKey, &theMouse );

	///---------------------------------------------------------------------------
	///-- n_ERESULT_ITEM_DROP 
	///---------------------------------------------------------------------------
	///-- Item Drop
	if( EventResult == n_ERESULT_ITEM_DROP )
	{
		// 1. 아이템 정보 얻기
		//SPcItem* pc_item;
		//pc_item  = nRui->GetCursorItem();
		
		CSlot* slot = nRui->GetItemSelectedSlot();

		///-- Quick Slot
		if( slot->Group == n_SGROUP_QUICK )
		{
			//퀵슬롯 해제....
			MainMenuWnd.UnLinkQuickSlot( slot );
		}
		///-- Inventory
		else if( slot->Group == n_SGROUP_INVENTORY )
		{
			if( slot->PcItem )
			{
				
#ifdef ITEM_LIMIT_FUNC				
				BYTE aItemLimitFunc = 0;

				if( slot->PcItem->ItemTable )
				{
					aItemLimitFunc = slot->PcItem->ItemTable->theItemLimitFunc;					
				}		

				if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotDrop ) )
				{
					SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_DROPITEM ) );					
					nRui->ResetCursorItem();
					nRui->SetItemSelectedSlot( NULL );
					goto IDROP_END;
				}
#endif				
				if( g_UserStoreSellWnd.ISUserSellMode() != FALSE )
				{
					nRui->ResetCursorItem();
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_USE ) ); // 개인 상점중에는 사용하실수없습니다.
				}
				else
				///-- quest slot
				if( slot->PcItem->IType == n_ITYPE_QUEST )
				{
					if( slot->PcItem->ItemTable->theBundle != 1 )		// 1이 아니면 번들개수가 있는 아이템 
					{
						g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_Q_ITEMDROP, slot->PcItem );
					}			
					else
					{
						Send_RpCsDrop( slot->PcItem->ItemUID, 1, n_QuestTab );
					}
				}				
				///-- inven slot
				else
				{
					///-- 유료 아이템이라면
					WORD wCode = slot->PcItem->ItemTable->theCode;
					if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max) )
					{
					#ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824
						
						if(slot->PcItem->IsItemCode12180_12188_Above())// 파괴?
							g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );
						else
						{
							SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );						
							nRui->ResetCursorItem();
							goto IDROP_END;
						}
						
					#endif// #ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824
					}
					
					// 2. 만약...번들 아이템이라면..몇개 버릴껀지...확인 하고 버리기					
//					if( slot->PcItem->Class == n_ICLASS_USE )
					if( slot->PcItem->ItemTable->theBundle != 1 )		// 1이 아니면 번들개수가 있는 아이템 
					{
						g_NumberInputWnd.Open( n_NUMBERINPUT_INVENTORY_ITEMDROP, slot->PcItem );
					}
					///-- 단일 아이템. MessageBox 로 의사확인
					else
					{
						if(InventoryWnd.GetTabType() != n_RidingTab)
						{
							g_MessageBox.SetParam( slot->PcItem->ItemUID );
							
#ifdef ITEM_LIMIT_FUNC
							if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_DropDestory ) )
							{
								g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );								
							}
							else
							{
								
								BOOL IsDestory = FALSE;
								
								///-- 가공 장신구는 파괴만 가능하다.
								BYTE aClass = slot->PcItem->ItemTable->theClass;
								if( n_Class_Accessory == aClass )
								{
									if( slot->PcItem->WearTable.theEldaStone )
										IsDestory = TRUE; 
								}

							#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

								if(slot->PcItem->IsItemCode60000Above())
									IsDestory = TRUE; 

							#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

								if( IsDestory )
								{
									g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );
								}
								else
								{
									g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_MSGBOX_ASK_DROP ), 250, 100 );	
								}
							}

#else
						    g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_MSGBOX_ASK_DROP ), 250, 100 );
#endif							
										
							// 서버에 버리기 요청
							//Send_RpCsDrop( slot->PcItem->ItemUID, 1, n_InvenTab );
						}
						else
						{
							g_MessageBox.SetParam( slot->PcItem->ItemUID );
							g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_R_ITEMDROP, G_STRING_CHAR( IDS_SYSPR_REALLY_DELETE ), 250, 100 );		
						}
					}
				}
			}
		}
		else if( slot->Group == n_SGROUP_WEAR )
		{
			if( slot->PcItem )
			{

#ifdef ITEM_LIMIT_FUNC				
				BYTE aItemLimitFunc = 0;

				if( slot->PcItem->ItemTable )
				{
					aItemLimitFunc = slot->PcItem->ItemTable->theItemLimitFunc;				
				}

				if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_NotDrop ) )
				{
					SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSCANNOT_DROPITEM ) );					
					nRui->ResetCursorItem();
					nRui->SetItemSelectedSlot( NULL );
					goto IDROP_END;
				}
#endif		
				
				///-- 유료 아이템이라면
				WORD wCode = slot->PcItem->ItemTable->theCode;
				if( ( c_ItemCode_Cash_Min <= wCode ) && ( wCode <= c_ItemCode_Cash_Max) )
				{
					SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_NOTSUPPLY_PAYITEM ) );					
					nRui->ResetCursorItem();
					nRui->SetItemSelectedSlot( NULL );
					goto IDROP_END;
				}				
				
				if( g_UserStoreSellWnd.ISUserSellMode() != FALSE )
				{
					nRui->ResetCursorItem();
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_USERSTORE_USE ) );			// 개인 상점중에는 사용하실수없습니다.
				}
				else
				{
					///-- 착용 아이템일 경우에는 라이딩슬롯이 아닐경우에만 버리기가 가능하다.
					if(slot->Class != n_Class_Riding)
					{
						g_MessageBox.SetParam( slot->PcItem->ItemUID );
						
#ifdef ITEM_LIMIT_FUNC
						if( CHECK_ITEMFUNC( aItemLimitFunc, n_ItemFunc_DropDestory ) )
						{
							g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_W_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );	
								
						}
						else
						{
							BOOL IsDestory = FALSE;
								
							///-- 가공 장신구는 파괴만 가능하다.
							BYTE aClass = slot->PcItem->ItemTable->theClass;
							if( n_Class_Accessory == aClass )
							{
								if( slot->PcItem->WearTable.theEldaStone )
								{
									IsDestory = TRUE; 
								}
							}
							
							if( IsDestory )
							{
								g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_SYSWHOLEDESTROYITEM ), 300, 100 );
							}
							else
							{
								g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_ITEMDROP, G_STRING_CHAR( IDS_MSGBOX_ASK_DROP ), 250, 100 );	
							}							
							
							g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_W_ITEMDROP, G_STRING_CHAR( IDS_MSGBOX_ASK_DROP ), 250, 100 );	
						}

#else
						g_MessageBox.OpenMessageBox( n_MESSAGEBOX_INVENTORY_W_ITEMDROP, G_STRING_CHAR( IDS_MSGBOX_ASK_DROP ), 250, 100 );
#endif						
											
						// 서버에 버리기 요청
						//Send_RpCsDrop( slot->PcItem->ItemUID, 1, n_WearTab );
					}
					
					nRui->ResetCursorItem();
					nRui->SetItemSelectedSlot( NULL );
					goto IDROP_END;
				}
			}
		}

		if( g_UserStoreSellWnd.ISUserSellMode() != FALSE )
		{
			nRui->SetCursorItem( NULL );
			nRui->SetItemSelectedSlot( NULL );
		}
	}

IDROP_END:

/*	//-------------------------------------------------------------------------
	if( GetGameMode() == GMODE_NORMAL ) 
	{
		static TCHAR str[128] = _T( "" );

		sprintf( str, "< Message >" );
		nRui->TextOut( str, 30, 150, D3DCOLOR_XRGB( 120, 200, 120 ) );
		sprintf( str, "--------------------------------------------" );
		nRui->SendEvent( WID_DeskTop, n_emTextOutput, (EPARAM)str, 30, 168, D3DCOLOR_XRGB( 120, 200, 120 ) );
		sprintf( str, "@ " );
		nRui->TextOut( str, 30, 190, D3DCOLOR_XRGB( 200, 120, 120 ) );
		sprintf( str, "@ " );
		nRui->TextOut( str, 30, 208, D3DCOLOR_XRGB( 200, 120, 120 ) );
		if( theMouse.LButton == n_baNone )
		{
			sprintf( str, "Mouse LButton : n_baNone\n" );
			//OutputDebugString( str );
		}
		if( theMouse.LButton == n_baDown )
		{
			sprintf( str, "Mouse LButton : n_baDown\n" );
			//OutputDebugString( str );
		}
		if( theMouse.LButton == n_baPress )
		{
			sprintf( str, "Mouse LButton : n_baPress\n" );
			//OutputDebugString( str );
		}
		if( theMouse.LButton == n_baUp )
		{
			sprintf( str, "Mouse LButton : n_baUp\n" );
			//OutputDebugString( str );
		}
		if( theMouse.LButton == n_baDblClick )
		{
			sprintf( str, "Mouse LButton : n_baDblClick\n" );
			//OutputDebugString( str );
		}
		nRui->TextOut( str, 30, 224, D3DCOLOR_XRGB( 200, 120, 120 ) );
		
	}
*/
	//by simwoosung
	g_GTimer.UpdateFrameElapsedTime();
	//사운드 
	g_Sound.Update();
	//카메라 
	g_Camera.UpdateWalk( m_pd3dDevice );

#ifdef	NETWORK_CLIENT
	static int cnt = 0;
	///-----------------------------------------------------------
	/// network process
	///-----------------------------------------------------------
//	RLG1( "NetworkProc() Start : %d", ++cnt );	
	
	NetworkProc();	/// main loop에서 호출

	//by simwoosung
	//------------------------------------------------------------------	
	//	CsStartMove 하고 일정시간 동안 ScStartMove가 안들어오면 강제로
	//	SetWaitingSvrMsg 중지//
	//------------------------------------------------------------------
	
	if( IsWaitingSvrMsg() )
	{
		theScWatingCumulateTime += g_GTimer.GetFrameElapsedSecTime();

		if( theScWatingCumulateTime > theScStartMoveWatingTime )
		{
			RLGS( "**** ScStartMove Time Over - SetWaitingSvrMsg( FALSE ) ****" );
			theScWatingCumulateTime = 0.0f;
			theActiveCsStartMoveFlag = FALSE;
			SetWaitingSvrMsg( FALSE );
		}
	}

//	RLG1( "NetworkProc() End : %d", cnt );
	if( IsDisConnected() && !MessageBoxWnd.IsProcessClose() )
	{
		if( GetLoginStep() != nNoConnect )
		{
			Proc_OnSocketClose();			
			return( S_OK );
		}
	}
#endif

	g_nowTime = timeGetTime();	/// 현재시간저장
	
#ifdef	GM_CLIENT
	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_TAB].Action == n_baDown )
	{
		theGMCmdDlg.ActiveDlg();
	}
	/*
	if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_1].Action == n_baDown )
	{
		g_WebShopWnd.OpenURL( L"http://naver.com" , "");
	}
	
	if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_2].Action == n_baDown )
	{
		g_WebShopWnd.OpenURL( L"http://so.runup.com.hk/itemmall" , "");
	}
	*/


#endif	
	
	if( thePacketTransLogDlg.IsActivePDlg() && 
		theKey[DIK_LSHIFT].Action == n_baPress && theKey[DIK_F1].Action == n_baUp )
	{
		bool bIsShow = thePacketTransLogDlg.m_IsShow;
		bIsShow = !bIsShow;
		if(bIsShow)
		{
			thePacketTransLogDlg.Open();
		}
		else
		{
			thePacketTransLogDlg.Close();
		}				
	}
	//..........................................................................................................
	// 게임 루프
	//..........................................................................................................
	switch( GetGameMode() )
	{		
		//......................................................................................................
		// 로그인 모드 ( 로그인창 / 서버 리스트 )
		//......................................................................................................
		case GMODE_LOGINPAGE:
		{			
#ifdef ROCKCLIENT_DEV	
			//..................................................................................................
			// ESC 키 검사 ( = 종료 )
			//..................................................................................................
			if( theKey[DIK_ESCAPE].Action == n_baUp )
			{
				ClientClose( g_RockClient.GetApp() , 1);
				return S_OK;
			}
#endif

#ifdef FIRSTLOADING_IMAGEFIXING		// 첫 로딩화면 고정
			if(LoadingBarWnd.IsFirstLoading==false)
				LoadingBarWnd.IsFirstLoading=true;
#endif
			//..................................................................................................
			// 로그인 처리 루틴
			//..................................................................................................
//			if( FAILED( LoginStepProcess() ) )
//			{
//				return S_FALSE;
//			}
		}
		break;

		//......................................................................................................
		// 캐릭터 선택 모드 ( 캐릭터 선택 / 생성 / 삭제 )
		//......................................................................................................
		case GMODE_SELECT:
		{

#ifdef ROCKCLIENT_DEV	

		if( theKey[DIK_F8].Action == n_baUp )
		{
			///맵 렌더링 - > 와이어프레임 on/off
			g_Map.theWireframe = !g_Map.theWireframe;
		}
#endif
			if( FAILED( CharacterSelectStepProcess() ) )
			{
				return S_FALSE;
			}

		#ifdef HHM_CHARACTER_SELECT_3D_PICK// 캐릭터 선택창에서 버튼으로 하던 캐릭터 선택을 3D 픽킹으로 바꿈(2009/06/12)
			
			// 반드시 n_baUp으로 할것. n_baDown이면 NM_BTN_CLICK이벤트 발생전 창이 닫힘. 삭제가 안되는 문제 생김.
			// 버튼 이벤트 NM_BTN_CLICK은 마우스업일 때 발생.
			if(theMouse.LButton == n_baUp)// && !g_CharacterDelWnd.IsVisible)
			{
				int iCore = g_Pc.PickCharSelectUser();
				if(iCore >= 0)
					CharSelectWnd.SelectCharacterMove(iCore);
			}
			
		#endif// #ifdef HHM_CHARACTER_SELECT_3D_PICK// 캐릭터 선택창에서 버튼으로 하던 캐릭터 선택을 3D 픽킹으로 바꿈(2009/06/12)


			if( g_Map.m_bReady == true )
			{
		
				//-----------------------------------------------------------------------------
				// 다른 캐릭터
				//-----------------------------------------------------------------------------
				for( int i = 0; i < MAX_USER_INTABLE; ++i )
				{
					if( g_Pc.m_Pc[i].m_bIsLoaded == true )
					{
						// 큐에 들어있는 메세지저장
						g_Pc.m_Pc[i].QueueProcess();
						
						int nRes = g_Pc.m_Pc[i].AniProcess();
					}
				}
			}		

		}
		break;

		//......................................................................................................
		// 게임 초기화 모드
		//......................................................................................................
		case GMODE_WORLDINIT:
		{
#ifndef	NETWORK_CLIENT
			// 텍스쳐 검색시간 초기화//
			g_TexManager.InitTextrueTime();
			g_Pc_Manager.InitSortTable();
			
			// 네트워크, 게임상태 변경
			m_dwGameMode = GMODE_NORMAL;

			///--PRINT_DLG_LOG("GMODE_INIT:게임모드를 NORMAL로 변경");
			DebugPrint( "!\r\n GMODE_INIT:게임모드를 NORMAL로 변경" );	
#endif
		}
		break;

		//......................................................................................................
		// 게임 진행
		//......................................................................................................
		case GMODE_NORMAL:
		{
			if( LoadingBarWnd.IsProgress == false )
			{
				g_TexManager.CreateUseTex();
				g_TexManager.DeleteNotUsedTex();
				g_EffItemManager.DeleteNotUsedTex();
			
#ifdef	NETWORK_CLIENT
				if( S_OK != UserInputProc() )
				{
					return E_FAIL;
				}
#endif

				if( g_RockClient.m_bIsDrawFullScreenUI == false )
				{
					if( g_ISObjTexLoad )
						g_Grass.Update();

					g_Map.ProcessLightByTime();
					
					if( g_Map.m_bReady == true )
					{
						g_Pc.AniProcess();
						g_Map.AniProcessNpc();						
						g_Map.CreateShadow();	///그림자 만들기
					}		
				}

#ifndef	NETWORK_CLIENT
				CheckWarpzone();	/// 싱글모드 클라이언트 임시 맵 워프 
#endif
			}
		
		}
		break;
	}
  
	
	if( (theKey[DIK_F12].Action == n_baDown  && m_IsClickScreen == 0) )
		m_IsClickScreen = 1;

/* 키입력 테스트	
	if(GetGameMode() == GMODE_NORMAL && theKey[DIK_F11].Action == n_baDown)
	{
// 		CEFBaragosEff::ms_iEffCount++;
// 		if(CEFBaragosEff::ms_iEffCount > 20)
// 			CEFBaragosEff::ms_iEffCount = 0;
	}
		//g_Particle.m_bDisplay = g_Particle.m_bDisplay ? false : true;

	TCHAR str[128] = _T("");
	//sprintf( str, _T("g_Particle.m_bDisplay : %d\n"),  g_Particle.m_bDisplay);
	//sprintf( str, _T("CEFBaragosEff::ms_iEffCount : %d\n"),  CEFBaragosEff::ms_iEffCount);
	//nRui->TextOut( str, 30, 90, D3DCOLOR_XRGB( 120, 200, 120 ) );
*/

#ifdef HHM_1600X1200_TEST// Radeon 9550 해상도 1600 X 1200 창모드에서 속도 저하(2009/05/15)

	if(theKey[DIK_F11].Action == n_baDown)
		g_1600x1200 = g_1600x1200 ? false : true;

#endif HHM_1600X1200_TEST// Radeon 9550 해상도 1600 X 1200 창모드에서 속도 저하(2009/05/15)

	return S_OK;
}

//-----------------------------------------------------------------------------
// 마우스커서
//-----------------------------------------------------------------------------
static	long	g_lCursorPreCount	= 0;
static	int		g_lCursorAni		= 0;
void		CRockClient::DisplayCursor()
{
	
	POINT	pt;GetCursorPos(&pt);
	D3DXVECTOR2	v	= D3DXVECTOR2(pt.x-5.0f,pt.y-10.0f);
	D3DXVECTOR2	scale	= D3DXVECTOR2(1.0f, 1.0f);

	switch ( m_byCurCursor )
	{
	case	BC_NORMAL:
	case	BC_ATTACKENABLE:
		SetRect(&m_rcCursor, m_byCurPush*SIZE_CURSORTEX, m_byCurCursor*SIZE_CURSORTEX, (m_byCurPush+1)*SIZE_CURSORTEX, (m_byCurCursor+1)*SIZE_CURSORTEX);
		break;
	case	BC_MOVEDISABLE:
		SetRect(&m_rcCursor, 0, m_byCurCursor*SIZE_CURSORTEX,SIZE_CURSORTEX,(m_byCurCursor+1)*SIZE_CURSORTEX);
		break;
	case	BC_EVENT:

		m_byCurAni	= 0;
//		g_dwCursorPreCount = g_nowTime;
		SetRect(&m_rcCursor, m_byCurAni*SIZE_CURSORTEX, m_byCurCursor*SIZE_CURSORTEX, (m_byCurAni+1)*SIZE_CURSORTEX, (m_byCurCursor+1)*SIZE_CURSORTEX);
//		}
		break;
	case	BC_ITEM:
			SetRect(&m_rcCursor, 128+m_byCurPush*SIZE_CURSORTEX, 0, 128+(m_byCurPush+1)*SIZE_CURSORTEX, 64);
		break;
	case	BC_CAM:
			SetRect(&m_rcCursor, 2*SIZE_CURSORTEX, 2*SIZE_CURSORTEX, 3*SIZE_CURSORTEX, 3*SIZE_CURSORTEX);
		break;
	case	BC_PERSON:
		{
			if ( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( g_lCursorPreCount , 400 ) ) )
			{
				if( g_lCursorAni == 1 )
					g_lCursorAni = 0;
				else
					g_lCursorAni++;
				g_lCursorPreCount = g_nowTime;
			}
			SetRect(&m_rcCursor, (2+g_lCursorAni)*SIZE_CURSORTEX, 1*SIZE_CURSORTEX, (3+g_lCursorAni)*SIZE_CURSORTEX, 2*SIZE_CURSORTEX);
		}
		break;
	case	BC_HORSE:
			SetRect(&m_rcCursor, 1*SIZE_CURSORTEX, 2*SIZE_CURSORTEX, 2*SIZE_CURSORTEX, 3*SIZE_CURSORTEX);
		break;
	}
	int alpha = 255;

//	m_pSprCursor->Begin();
//	m_pSprCursor->Draw( m_pTextureCursor, 
//						&m_rcCursor, 
//						&scale, 
//						NULL, 
//						NULL, 
//						&v, 
//						D3DCOLOR_ARGB(alpha,255,255,255 ));
//	m_pSprCursor->End();
}

//-----------------------------------------------------------------------------
// Desc: 마우스이동
//-----------------------------------------------------------------------------
void		CRockClient::SetCursor()
{	
	bool bFind = false;
	switch( g_Pc.GetPlayer()->m_curt_event.type )
	{
	case SM_ATTACK_NPC:
		{
			bFind = true;
		}
		break;
	case SM_USE_SKILL:
		{
			bFind = true;
		}
		break;
	}
	switch( g_Pc.GetPlayer()->m_save_event.type )
	{
	case SM_ATTACK_NPC:
		{
			bFind = true;
		}
		break;
	case SM_USE_SKILL:
		{
			bFind = true;
		}
		break;
	}

	if( bFind == false )
	{
		if( g_Pc.m_nPickedUser != -1 && g_Pc.m_nPickedUser != MAX_USER_INTABLE )
		{
			g_Pc.m_Select.bPC	= true;
			g_Pc.m_Select.index = g_Pc.m_nPickedUser;
			return;
		}
		else if( g_Map.m_nPickedNpc != -1  )
		{
			g_Pc.m_Select.bPC	= false;
			g_Pc.m_Select.index = g_Map.m_nPickedNpc;
			return;
		}
		g_Pc.m_Select.Clear();
	}	
		
	/*
	//-----------------------------------------------------------------------------
	// 아무것도 선택하지 않았다면 리턴
	//-----------------------------------------------------------------------------
	if( m_CursorIndex == -1 && m_CursorUnique == -1 )
		return;

	//-----------------------------------------------------------------------------
	// 해당 캐릭터위치를 2D값으로 변경
	//-----------------------------------------------------------------------------
	D3DXVECTOR2 vPos;

	// PC
	if( m_CursorIndex == -1 )	
		vPos = g_ChatList.From3DTo2D( m_CursorUnique );		
	else
	{
		// 아이템
		if( m_bCursorItem == true )
			vPos = g_ChatList.From3DTo2D( (short)g_Map.FindNpc(m_CursorIndex,m_CursorUnique), true );		
		// NPC
		else
			vPos = g_ChatList.From3DTo2D( m_CursorUnique, m_CursorIndex );		
	}

	//-----------------------------------------------------------------------------
	// 마우스이동
	//-----------------------------------------------------------------------------
	SetCursorPos( (int)vPos.x, (int)vPos.y );
	*/
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT CRockClient::ConfirmDevice(D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT Format)
#else
HRESULT CRockClient::ConfirmDevice(D3DCAPS8* pCaps, DWORD dwBehavior, D3DFORMAT Format)
#endif // DIRECT_VERSION_9_MJH
{
/*	if( pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
        return S_OK;
    if( pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHA )
        return S_OK;
*/	
	return	S_OK;
}
 



//-----------------------------------------------------------------------------
int SortCharacterList( const void* arg1, const void* arg2 )
{
	SPcInfo* key		= (SPcInfo*)arg1;
    SPcInfo* element	= (SPcInfo*)arg2;
	
	/*
	// 캐릭이 존재하지 않아도....거리값이 초기화( 제일 먼 거리로.... )
	// 되어있기 때문에...예외 처리 필요없다.

	if( ( key->existence == false ) && ( element->existence == false ) )
	{
		return 1;
	}
	//*/

	return( element->distance - key->distance );// 왜...distance = 0xffffffff를 -1로 받아들이는가?
}

#ifdef ADD_APPLYWIDE_MAP
struct SortCharacterIndexKeyList: public binary_function<SPcInfo, SPcInfo, bool>  //MJHMAP
{ 
       bool operator()(const SPcInfo &Key0,const SPcInfo &Key1) const 
       { 
		   return ( Key1.distance - Key0.distance   );
       } 
}; 
#endif // ADD_APPLYWIDE_MAP


SPcInfo* CRockClient::FindNPCPcInfo(long unique)
{
	for ( int i = 0; i < MAX_CHARACTER; ++i )
	{			
		if( m_PcInfo[i].existence == false )
		{
			continue;
		}

		Character*	pick_chr = NULL;
		pick_chr = (Character*) m_PcInfo[i].pCharacter;

		if(pick_chr)
		{
			if(false == pick_chr->m_bPC)
			{
				if( unique == m_PcInfo[i].id )
				{
					return &m_PcInfo[i];
				}
			}
		}	
	}

	return NULL;
}

//-----------------------------------------------------------------------------
int	CRockClient::PickPc()
{
	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);
//	ptCursor.x	+= 2;
//	ptCursor.y	+= 10;

	D3DXVECTOR3	vPickRayDir;
	D3DXVECTOR3	vPickRayOrig;

	D3DXMATRIX		matProj;
	matProj	= g_Camera.m_matProj;

	GetCursorPos(&ptCursor);
//	ptCursor.x	+= 2;
//	ptCursor.y	+= 10;
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);

	D3DXVECTOR3	v;
	v.x	= (((2.0f*ptCursor.x)/g_RockClient.GetDisplayWidth())-1) / matProj._11;
	v.y	= -(((2.0f*ptCursor.y)/g_RockClient.GetDisplayHeight())-1) / matProj._22;
	v.z	= 1.0f;

	D3DXMATRIX		matView, m;
	matView	= g_Camera.m_matView;
	D3DXMatrixInverse(&m, NULL, &matView);

	vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;

	FLOAT	fBary1, fBary2;
	FLOAT	fDist;
	
	D3DXVECTOR3		v1[3];	// 1st face
	D3DXVECTOR3		v2[3];	// 2nd face
	D3DXMATRIX		matpos;
	D3DXMATRIX		matrot;
	D3DXVECTOR3		min, max;
	
	bool IsPickIndex = true;

	nRui->theNonPlayerInfoMiniBar.thePickedIndex = -1;

	for ( int i = 0; i < MAX_CHARACTER; ++i )
	{			
		if( m_PcInfo[i].existence == false )
		{
			continue;
		}

		Character*	pick_chr = NULL;
		pick_chr = (Character*) m_PcInfo[i].pCharacter;
		
		if( !pick_chr->m_bIsVisible )
		{
			continue;
		}

		if(pick_chr->m_State.nRideState == nRideState_Ride)
			continue;
		
		if(pick_chr->m_CharAlpha.State == CHAR_NONE_DISPLAY)
			continue;

		// 타겟팅 된 유저나 몬스터의 인덱스 값을 저장한다 
		if( IsPickIndex )
		{				
			if( pick_chr )
			{
				if( !g_Pc.GetPlayer()->IsMyPlayerRender && 
					pick_chr->m_nTableIndex == MAX_USER_INTABLE)
				{
					continue;
				}				
				
				if( NonPlayerInfoWnd.thePickedNpcID != -1 )
				{
					if( pick_chr->m_curt_event.type == SM_STATE_DIE )
					{
						nRui->theNonPlayerInfoMiniBar.thePickedIndex = -1;
					}
					else if( g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].code >= c_NonplayerCode_Min && 
							 g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].code < c_NonplayerCode_Max )
					{
						nRui->theNonPlayerInfoMiniBar.thePickedIndex = -1;
					}
					else if( m_PcInfo[i].ctype == enMONSTER && pick_chr->m_nTableIndex == NonPlayerInfoWnd.thePickedNpcID && pick_chr->m_bIsVisible )
					{
						nRui->theNonPlayerInfoMiniBar.thePickedIndex = i;
						IsPickIndex = false;
					}
				}
				else if( NonPlayerInfoWnd.thePickedUserID != -1 && NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE )
				{
					if( pick_chr->m_curt_event.type == SM_STATE_DIE )
					{
						nRui->theNonPlayerInfoMiniBar.thePickedIndex = -1;
					}
					else if( m_PcInfo[i].ctype == enNPC && pick_chr->m_nTableIndex == NonPlayerInfoWnd.thePickedUserID && pick_chr->m_bIsVisible )
					{
						nRui->theNonPlayerInfoMiniBar.thePickedIndex = i;
						IsPickIndex = false;
					} 
				}
				else
				{
					nRui->theNonPlayerInfoMiniBar.thePickedIndex = -1;
				}
			}
			else
			{
				nRui->theNonPlayerInfoMiniBar.thePickedIndex = -1;
			}
		}	

		D3DXMatrixIdentity( &matpos );		
		D3DXMatrixIdentity( &matrot );		

		matpos._41 = m_PcInfo[i].wpos.x;
		matpos._42 = m_PcInfo[i].wpos.y;
		matpos._43 = m_PcInfo[i].wpos.z;

		min = m_PcInfo[i].min;
		max = m_PcInfo[i].max;

		for( int j = 0; j < 6; j ++ )
		{
			switch( j )
			{
			// front side
			case 0:				
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;

				v2[0].x	= max.x;
				v2[0].y	= max.y;
				v2[0].z	= min.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= min.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break; 
			// right side
			case 1:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= max.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;

				v2[0].x	= max.x;
				v2[0].y	= max.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= max.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break;
			// back side
			case 2:		
				v1[0].x	= max.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;

				v2[0].x	= min.x;
				v2[0].y	= max.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= max.z;
				break;
			// left side
			case 3:		
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= max.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= min.z;
				v1[2].x	= min.x;
				v1[2].y	= max.y;
				v1[2].z	= max.z;

				v2[0].x	= min.x;
				v2[0].y	= max.y;
				v2[0].z	= min.z;
				v2[1].x	= min.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= min.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break;
			// top side
			case 4:					
				v1[0].x	= min.x;
				v1[0].y	= max.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= max.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= max.y;
				v1[2].z	= min.z;

				v2[0].x	= min.x;
				v2[0].y	= max.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= max.y;
				v2[1].z	= max.z;
				v2[2].x	= max.x;
				v2[2].y	= max.y;
				v2[2].z	= min.z;
				break;
			// bottom side
			case 5:						
				v1[0].x	= min.x;
				v1[0].y	= min.y;
				v1[0].z	= min.z;
				v1[1].x	= min.x;
				v1[1].y	= min.y;
				v1[1].z	= max.z;
				v1[2].x	= max.x;
				v1[2].y	= min.y;
				v1[2].z	= min.z;

				v2[0].x	= min.x;
				v2[0].y	= min.y;
				v2[0].z	= max.z;
				v2[1].x	= max.x;
				v2[1].y	= min.y;
				v2[1].z	= max.z;
				v2[2].x	= max.x;
				v2[2].y	= min.y;
				v2[2].z	= min.z;
				break; 
			}

			D3DXMatrixRotationYawPitchRoll( &matrot, pick_chr->m_Mov.now_Yrad, pick_chr->m_Mov.now_Xrad, 0.0f );
			D3DXVec3TransformCoord( &v1[0], &v1[0], &matrot );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matrot );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matrot );
			D3DXVec3TransformCoord( &v2[0], &v2[0], &matrot );
			D3DXVec3TransformCoord( &v2[1], &v2[1], &matrot );
			D3DXVec3TransformCoord( &v2[2], &v2[2], &matrot );

			D3DXVec3TransformCoord( &v1[0], &v1[0], &matpos );
			D3DXVec3TransformCoord( &v1[1], &v1[1], &matpos );
			D3DXVec3TransformCoord( &v1[2], &v1[2], &matpos );
			D3DXVec3TransformCoord( &v2[0], &v2[0], &matpos );
			D3DXVec3TransformCoord( &v2[1], &v2[1], &matpos );
			D3DXVec3TransformCoord( &v2[2], &v2[2], &matpos );

/*			if( pick_chr->m_CollisionBox.m_pDebugVB ) 
			{
				FVF_PD*	pVertices = NULL;
				int nVtxCnt = 0;
				
				if ( !FAILED ( pick_chr->m_CollisionBox.m_pDebugVB->Lock(0,0,(BYTE**)&pVertices, 0) ) )
				{			
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v1[0].x, v1[0].y, v1[0].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v1[1].x, v1[1].y, v1[1].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v1[1].x, v1[1].y, v1[1].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v1[2].x, v1[2].y, v1[2].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v1[2].x, v1[2].y, v1[2].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v1[0].x, v1[0].y, v1[0].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v2[0].x, v2[0].y, v2[0].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v2[1].x, v2[1].y, v2[1].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v2[1].x, v2[1].y, v2[1].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v2[2].x, v2[2].y, v2[2].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v2[2].x, v2[2].y, v2[2].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					pVertices[nVtxCnt].v		= D3DXVECTOR3( v2[0].x, v2[0].y, v2[0].z );
					pVertices[nVtxCnt++].color	= D3DCOLOR_XRGB( 0, 0, 255 );
					
					pick_chr->m_CollisionBox.m_pDebugVB->Unlock();				
	
					D3DXMATRIX	matWorld;
					D3DXMatrixIdentity( &matWorld );
					}
			}
*/
			
			if( g_Map.IntersectTriangle(vPickRayOrig, vPickRayDir, v1[0], v1[1], v1[2], &fDist, &fBary1, &fBary2) ||
				g_Map.IntersectTriangle(vPickRayOrig, vPickRayDir, v2[0], v2[1], v2[2], &fDist, &fBary1, &fBary2) )
			{
//				RLG2("<<CCW>>-=*****Pick Plane = %d, m_Mov.now_Yrad = %3.3f*****=-", j, pick_chr->m_Mov.now_Yrad);
				nRui->theNonPlayerInfoMiniBar.thePickedIndex = i;
				return i;
			}			
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
int CRockClient::GetPickPcIdx()
{
	return( m_PickPcIdx );
}
    
//-----------------------------------------------------------------------------
SPcInfo* CRockClient::GetPickPcInfo()
{
	if( m_PickPcIdx == -1 )
	{
		return NULL;
	}
	
	return( &m_PcInfo[m_PickPcIdx] );
}

//-----------------------------------------------------------------------------
void CRockClient::SetCharacterInfo()
{
//	int mouse_speed[3] = {0,};
//	// 마우스 감도 높이기 
//	mouse_speed[0] = 20;
//	mouse_speed[1] = 0;
//	mouse_speed[2] = 0;
//	SystemParametersInfo( SPI_SETMOUSESPEED, 0, mouse_speed, SPIF_SENDCHANGE );


	int				i = 0;
	int				count = 0;
	nRui->thePcinfoCount =0 ; 

	D3DXVECTOR3		tpos;
	D3DXVECTOR3		cpos;
	D3DXVECTOR3		dv;//카메라와 캐릭터의 사이의 벡터
	D3DXVECTOR3		vc;//다른 캐릭터의 플레이어 사이의 벡터
#ifdef DIRECT_VERSION_9_MJH
	D3DVIEWPORT9	viewport;
#else
	D3DVIEWPORT8	viewport;
#endif // DIRECT_VERSION_9_MJH
	D3DXMATRIX		matchar;
	D3DXMATRIX		matproj;
	D3DXMATRIX		matview;
	D3DXMATRIX		matworld;

	D3DXVECTOR3     vPlayerPos;
	
	memset( m_PcInfo, 0, sizeof( SPcInfo ) * MAX_CHARACTER );

	memset( &tpos, 0, sizeof(D3DXVECTOR3) );
	memset( &cpos, 0, sizeof(D3DXVECTOR3) );
	memset( &dv, 0, sizeof(D3DXVECTOR3) );
#ifdef DIRECT_VERSION_9_MJH
	memset( &viewport, 0, sizeof(D3DVIEWPORT9) );
#else
	memset( &viewport, 0, sizeof(D3DVIEWPORT8) );
#endif // DIRECT_VERSION_9_MJH

	//D3DCREATE_PUREDEVICE옵션을 쓸때는 Get관련 함수들은 쓸수 없다.
	D3DXMatrixIdentity( &matproj );
	D3DXMatrixIdentity( &matview );
	D3DXMatrixIdentity( &matworld );
	g_RenderManager.GetTransform( D3DTS_PROJECTION, &matproj );
	g_RenderManager.GetTransform( D3DTS_VIEW, &matview );
	g_RenderManager.GetViewport( &viewport );


	//-------------------------------------------------------------------------
	Player*	user_pc = g_Pc.GetPlayer();
	
	m_PcInfo[count].pCharacter = ( Character * )user_pc;

	matchar = user_pc->GetPosTM();
	
	vPlayerPos.x = matchar._41;
	vPlayerPos.y = matchar._42;
	vPlayerPos.z = matchar._43;

	cpos.x = matchar._41;
	cpos.y = matchar._42 + user_pc->m_fMaxHeight;// 머리위에 띄울려면 중심점에서....키 높이 만큼 더해서 계산하자....
	cpos.z = matchar._43;
	
	if(user_pc->m_State.bRide)
	{
		cpos.y += user_pc->m_fAddRideHeight;
	}

	dv.x = g_Camera.m_vEye.x - matchar._41;
	dv.y = g_Camera.m_vEye.y - matchar._42;
	dv.z = g_Camera.m_vEye.z - matchar._43;
	
	D3DXVec3Project( &tpos, &cpos, &viewport, &matproj, &matview, &matworld );

	m_PcInfo[count].existence	= true;
	m_PcInfo[count].distance	= abs( sqrtf( ( dv.x * dv.x ) + ( dv.y * dv.y ) + ( dv.z * dv.z ) ) );
	
	if(g_IsFrameOptimize)	
	{
		user_pc->SetCamDistance((int)( m_PcInfo[count].distance ));
	}
	else
	{
		user_pc->m_nDelVATime = 30;	
	}

	m_PcInfo[count].char_distance = 0;
	m_PcInfo[count].direction	= user_pc->m_byAngle;// 현재 바라보는 각
	m_PcInfo[count].angle		= (int)D3DXToDegree( user_pc->m_radian ) % 360;
	
	m_PcInfo[count].ctype		= enPLAYER;

	m_PcInfo[count].npc_code	= 0;
	m_PcInfo[count].id			= g_Pc.m_MainPC.lUnique;
	m_PcInfo[count].type_idx	= -1;

	m_PcInfo[count].btype		= user_pc->m_BType;
	m_PcInfo[count].color		= user_pc->m_Color;

	SPcDataParam* PcParam = nRui->GetPcParamInfo();
	SAFE_STR_CPY( m_PcInfo[count].name, PcParam->PCName, 32 );
	SAFE_STR_CPY( m_PcInfo[count].message, user_pc->m_Message, 128 );
	
    //by simwoosung
	SAFE_STR_CPY( m_PcInfo[count].partyname, PcParam->PartyName, 128 );
	

#ifdef CPK_SYSTEM
	
	///-- 자신이 카오인지 여부확인
	DWORD dPKPoint = PcParam->thePKPoint;
	if( ::GetPKStepFromPKPoint( dPKPoint ) )
	{
		m_PcInfo[count].IsChaosChar = TRUE;	
	}
	else
	{
		m_PcInfo[count].IsChaosChar = FALSE;	
	}

#endif

	
#ifdef FRONTIER
	
	if(nRui->GetPcInvenInfo()->FrontierInfo)
	{
		SAFE_STR_CPY( m_PcInfo[count].FrontierName, nRui->GetPcInvenInfo()->FrontierInfo->theFrontierName, RP_MAX_FRONTIER_NAME);
		m_PcInfo[count].Status = nRui->GetPcInvenInfo()->FrontierInfo->theStatus;
	}

	if(nRui->GetPcInvenInfo()->FrontierMyInfo)
	{
		SAFE_STR_CPY( m_PcInfo[count].FrontierNick, nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierNick, RP_MAX_FRONTIER_NICK );
		m_PcInfo[count].FrontierID = nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID;
	}
	
	//프론티어 아이디
	
#endif

//	m_PcInfo[count].min = g_Pc_Manager.GetBoundMinPos( user_pc );
//	m_PcInfo[count].max = g_Pc_Manager.GetBoundMaxPos( user_pc );
	m_PcInfo[count].min = g_Pc_Manager.GetMinBoundVec( user_pc );
	m_PcInfo[count].max = g_Pc_Manager.GetMaxBoundVec( user_pc );

	if(!user_pc->m_State.bRide)
	{
		m_PcInfo[count].wpos.x = matchar._41;
		m_PcInfo[count].wpos.y = matchar._42;	
		m_PcInfo[count].wpos.z = matchar._43;
	}
	else
	{
		m_PcInfo[count].wpos.x = user_pc->m_matRideChr._41;
		m_PcInfo[count].wpos.y = user_pc->m_matRideChr._42;	
		m_PcInfo[count].wpos.z = user_pc->m_matRideChr._43;
	}
	
	m_PcInfo[count].spos.x = tpos.x;
	m_PcInfo[count].spos.y = tpos.y;

	count++;

	//-------------------------------------------------------------------------
	NonPlayer* other_pc[MAX_USER_INTABLE];
	for( i = 0; i < MAX_USER_INTABLE; ++i )
	{
		other_pc[i] = &g_Pc.m_Pc[i];

		memset( &tpos, 0, sizeof(D3DXVECTOR3) );
		memset( &cpos, 0, sizeof(D3DXVECTOR3) );

		if( g_Pc.m_PcTable[i].lUnique != -1 || other_pc[i]->m_nTableIndex != -1 )
		{
			matchar = other_pc[i]->GetPosTM();
			cpos.x = matchar._41;
			cpos.y = matchar._42 + g_Pc.m_Pc[i].m_fMaxHeight; // 머리위에 띄울려면 중심점에서....키 높이 만큼 더해서 계산하자....
			cpos.z = matchar._43;

			if(other_pc[i]->m_State.bRide)
			{
				cpos.y += other_pc[i]->m_fAddRideHeight;
			}

			dv.x = g_Camera.m_vEye.x - matchar._41;
			dv.y = g_Camera.m_vEye.y - matchar._42;
			dv.z = g_Camera.m_vEye.z - matchar._43;

			vc.x =  ( matchar._41 - vPlayerPos.x );
			vc.y =  ( matchar._42 - vPlayerPos.y );
			vc.z =  ( matchar._43 - vPlayerPos.z );

			D3DXVec3Project( &tpos, &cpos, &viewport, &matproj, &matview, &matworld );

			m_PcInfo[count].existence	  = true;
			m_PcInfo[count].distance  	  = abs( sqrtf( ( dv.x * dv.x ) + ( dv.y * dv.y ) + ( dv.z * dv.z ) ) );

			if(g_IsFrameOptimize)	
			{
				other_pc[i]->SetCamDistance(m_PcInfo[count].distance);
			}
			else
			{
				other_pc[i]->m_nDelVATime = 30;
			}
			
			m_PcInfo[count].char_distance = abs( sqrtf( ( vc.x * vc.x ) + ( vc.y * vc.y ) + ( vc.z * vc.z ) ) );
			m_PcInfo[count].direction	= other_pc[i]->m_byAngle;// 현재 바라보는 각

			m_PcInfo[count].npc_code	= 0;
			m_PcInfo[count].id			= g_Pc.m_PcTable[i].lUnique;
			m_PcInfo[count].type_idx	= -1;

			m_PcInfo[count].ctype		= enNPC;

			m_PcInfo[count].btype		= other_pc[i]->m_BType;
			m_PcInfo[count].color		= other_pc[i]->m_Color;
			SAFE_STR_CPY( m_PcInfo[count].name, g_Pc.m_PcTable[i].char_info.thePCName, 32 );
			SAFE_STR_CPY( m_PcInfo[count].message, other_pc[i]->m_Message, 128 );
			
            //by simwoosung
			SAFE_STR_CPY( m_PcInfo[count].partyname, g_Pc.m_PcTable[i].char_info.thePartyName, 128 );

#ifdef CPK_SYSTEM
	
			///-- 해당 유저가 카오인지 여부확인
			DWORD dPKPoint = g_Pc.m_PcTable[i].char_info.thePKPoint; 
			if( ::GetPKStepFromPKPoint( dPKPoint ) )
			{
				m_PcInfo[count].IsChaosChar = TRUE;	
			}

#endif
			
#ifdef FRONTIER

		    m_PcInfo[count].FrontierID = g_Pc.m_PcTable[i].char_info.theFrontierID;
			//프론티어 아이디
			SAFE_STR_CPY( m_PcInfo[count].FrontierNick, g_Pc.m_PcTable[i].char_info.theFrontierNick, RP_MAX_FRONTIER_NICK );
			SAFE_STR_CPY( m_PcInfo[count].FrontierName, g_Pc.m_PcTable[i].char_info.theFrontierName, RP_MAX_FRONTIER_NAME );
			m_PcInfo[count].Status = g_Pc.m_PcTable[i].char_info.theStatus;

#endif
			if(!other_pc[i]->m_State.bRide)
			{
				m_PcInfo[count].wpos.x = matchar._41;
				m_PcInfo[count].wpos.y = matchar._42;	
				m_PcInfo[count].wpos.z = matchar._43;
			}
			else
			{
				m_PcInfo[count].wpos.x = other_pc[i]->m_matRideChr._41;
				m_PcInfo[count].wpos.y = other_pc[i]->m_matRideChr._42;	
				m_PcInfo[count].wpos.z = other_pc[i]->m_matRideChr._43;
			}			
			
			m_PcInfo[count].spos.x = tpos.x;
			m_PcInfo[count].spos.y = tpos.y;

//			m_PcInfo[count].min = g_Pc_Manager.GetBoundMinPos( other_pc[i] );
//			m_PcInfo[count].max = g_Pc_Manager.GetBoundMaxPos( other_pc[i] );
			m_PcInfo[count].min = g_Pc_Manager.GetMinBoundVec( other_pc[i] );
			m_PcInfo[count].max = g_Pc_Manager.GetMaxBoundVec( other_pc[i] );

			m_PcInfo[count].pCharacter = ( Character * )&g_Pc.m_Pc[i];
		}
		else
		{
			m_PcInfo[count].existence	  = false;
			m_PcInfo[count].distance      = 65535;// 왜...0xffffffff를 -1로 받아들이는가?
			m_PcInfo[count].char_distance = 65535;// 왜...0xffffffff를 -1로 받아들이는가?
			m_PcInfo[count].pCharacter    = NULL;
		}
		
		count++;
	}

	//-------------------------------------------------------------------------
	NonPlayer* monster_pc[MAX_NPC_INTABLE];
	for( i = 0; i < MAX_NPC_INTABLE; ++i )
	{
		monster_pc[i] = &g_Map.m_Npc[i];

		memset( &tpos, 0, sizeof(D3DXVECTOR3) );
		memset( &cpos, 0, sizeof(D3DXVECTOR3) );
	
		if( g_Map.m_NpcTable[i].lUnique != -1 || monster_pc[i]->m_nTableIndex != -1 )
		{
			matchar = monster_pc[i]->GetPosTM();
			cpos.x = matchar._41;
			cpos.y = matchar._42 + g_Map.m_Npc[i].m_fMaxHeight; // 머리위에 띄울려면 중심점에서....키 높이 만큼 더해서 계산하자....
			cpos.z = matchar._43;

			dv.x = g_Camera.m_vEye.x - matchar._41;
			dv.y = g_Camera.m_vEye.y - matchar._42;
			dv.z = g_Camera.m_vEye.z - matchar._43;

			vc.x =  ( matchar._41 - vPlayerPos.x );
			vc.y =  ( matchar._42 - vPlayerPos.y );
			vc.z =  ( matchar._43 - vPlayerPos.z );

			D3DXVec3Project( &tpos, &cpos, &viewport, &matproj, &matview, &matworld );

			m_PcInfo[count].existence	= true;
			m_PcInfo[count].distance	= abs( sqrtf( ( dv.x * dv.x ) + ( dv.y * dv.y ) + ( dv.z * dv.z ) ) );

			if(g_IsFrameOptimize)	
			{
				monster_pc[i]->SetCamDistance(m_PcInfo[count].distance);
			}
			else
			{
				monster_pc[i]->m_nDelVATime = 30;
			}		
			
			m_PcInfo[count].char_distance = abs( sqrtf( ( vc.x * vc.x ) + ( vc.y * vc.y ) + ( vc.z * vc.z ) ) );
			m_PcInfo[count].direction	= monster_pc[i]->m_byAngle;// 현재 바라보는 각

			m_PcInfo[count].npc_code	= g_Map.m_NpcTable[i].code;
			m_PcInfo[count].id			= g_Map.m_NpcTable[i].lUnique;
			m_PcInfo[count].type_idx	= g_Map.m_NpcTable[i].index;

			switch( g_Map.m_NpcTable[i].race )
			{
				case -1:
					
					m_PcInfo[count].ctype		= enMONSTER;
		
					break;
				case -2:
	
					m_PcInfo[count].ctype		= enItem;
					
					break;
			}

			m_PcInfo[count].race		= g_Map.m_NpcTable[i].race;
			m_PcInfo[count].btype		= monster_pc[i]->m_BType;
			m_PcInfo[count].color		= monster_pc[i]->m_Color;
			SAFE_STR_CPY( m_PcInfo[count].name, g_Map.m_NpcTable[i].szName, 32 );
			SAFE_STR_CPY( m_PcInfo[count].message, monster_pc[i]->m_Message, 128 );			

			SAFE_STR_CPY( m_PcInfo[count].FrontierName, g_Map.m_NpcTable[i].theFrontierName, RP_MAX_FRONTIER_NAME );
			m_PcInfo[count].FrontierID = g_Map.m_NpcTable[i].theFrontierID;	

			m_PcInfo[count].wpos.x = matchar._41;
			m_PcInfo[count].wpos.y = matchar._42;
			m_PcInfo[count].wpos.z = matchar._43;
			m_PcInfo[count].spos.x = tpos.x;
			m_PcInfo[count].spos.y = tpos.y;

			/*if( (g_Map.m_NpcTable[i].code >= c_ObjectNPCCode_Min && g_Map.m_NpcTable[i].code <= c_ObjectNPCCode_Max) ||
				 g_Map.m_NpcTable[i].theNpcSize >= 4 ||
				 g_Map.IsGiantUnit(&g_Map.m_Npc[i]) ) 
			{
				m_PcInfo[count].min = g_Pc_Manager.GetBoundMinPos( monster_pc[i] );
				m_PcInfo[count].max = g_Pc_Manager.GetBoundMaxPos( monster_pc[i] );
			}
			else
			{
				m_PcInfo[count].min = g_Pc_Manager.GetMinBoundVec( monster_pc[i] );
				m_PcInfo[count].max = g_Pc_Manager.GetMaxBoundVec( monster_pc[i] );
			}*/
			
			if( g_Map.m_Npc[i].m_State.nCharacterType != nCharacterTypePlayerNPC )
			{
				m_PcInfo[count].min = g_Pc_Manager.GetBoundMinPos( monster_pc[i] );
				m_PcInfo[count].max = g_Pc_Manager.GetBoundMaxPos( monster_pc[i] );
			}
			else
			{
				m_PcInfo[count].min = g_Pc_Manager.GetMinBoundVec( monster_pc[i] );
				m_PcInfo[count].max = g_Pc_Manager.GetMaxBoundVec( monster_pc[i] );
			}			

			m_PcInfo[count].pCharacter = ( Character * )&g_Map.m_Npc[i];
		}
		else
		{
			m_PcInfo[count].existence	  = false;
			m_PcInfo[count].distance	  = 65535;
			m_PcInfo[count].char_distance = 65535;
			m_PcInfo[count].pCharacter    = NULL;
		}
	
		count++;
	}

	nRui->thePcinfoCount  = count;
	
#ifdef ADD_APPLYWIDE_MAP
	sort( m_PcIndexList.begin(), m_PcIndexList.end() , SortCharacterIndexKeyList());
#else
	qsort( m_PcInfo, MAX_CHARACTER, sizeof( SPcInfo ), SortCharacterList );
#endif // ADD_APPLYWIDE_MAP
	
	//..........................................................................................................
	// 피킹시 표시 부분 By wxywxy
	//..........................................................................................................
	static int	newPickIdx = -1;

	
	newPickIdx = PickPc();
		
	//원반 효과.....
	if( m_PickPcIdx != newPickIdx )
	{
		if( m_PickPcIdx != -1 )
		{
//			g_Billboard.DelManager( EB_PICK , m_PickUnique , m_PickIndex );

			m_PickUnique = -1;
			m_PickPcIdx  = -1;
			m_PickIndex  = -1;
		}
		
		m_PickPcIdx  = newPickIdx;
		m_PickUnique = m_PcInfo[newPickIdx].id;
		m_PickIndex  = m_PcInfo[newPickIdx].type_idx;
/*
		이펙트
		if( newPickIdx != -1 )
			g_Billboard.SetManager( 0 , EB_PICK , m_PickUnique , m_PickIndex );
*/
	}
	
	if( m_PickPcIdx != -1 )
	{
		if( (( Character * )m_PcInfo[m_PickPcIdx].pCharacter)->GetEvent()->type == SM_STATE_DIE )
		{
//			g_Billboard.DelManager( EB_PICK , m_PickUnique , m_PickIndex );

			m_PickPcIdx  = -1;
			m_PickUnique = -1;
			m_PickIndex  = -1;
		}

	}

	nRui->SetPickPcIdx( m_PickPcIdx );
	//..........................................................................................................

/*
	for( int x = 0; x < MAX_CHARACTER; ++x )

	{
		if(	( m_PcInfo[x].existence == true ) &&
			( ( m_PcInfo[x].spos.x >= 0 ) && ( m_PcInfo[x].spos.x < 1024 ) ) &&
			( ( m_PcInfo[x].spos.y >= 0 ) && ( m_PcInfo[x].spos.y < 768 ) ) )
		{
			sprintf( str, "(%d)  %s  - d: %d\n",  x, m_PcInfo[x].name, m_PcInfo[x].distance );
			OutputDebugString( str );
		}
	}

	OutputDebugString( "\n" );
	//*/
}

//-----------------------------------------------------------------------------
void CRockClient::SetPlayerMessage( TCHAR* str )
{
	Player* pc = g_Pc.GetPlayer();

	pc->m_CID	= thePCID;
	pc->m_BType	= n_BT_NORMAL;
	pc->m_Color	= D3DCOLOR_XRGB( 255, 255, 255 );
	
	SAFE_STR_CPY( pc->m_Message, str, 128 );
	
	nRui->SetDlgBalloon( enPLAYER, pc->m_CID );
}

//-----------------------------------------------------------------------------
void CRockClient::SetNonPlayerMessage( DWORD pcid, TCHAR* str )
{
	NonPlayer* npc = g_Pc.GetUser( (long)pcid );
	
	if( npc != NULL )
	{
		int tbl_index = g_Pc.FindUser( (long)pcid );
		
		npc->m_CID		= pcid;
		npc->m_BType	= n_BT_NORMAL;
		npc->m_Color	= D3DCOLOR_XRGB( 255, 255, 255 );
		
		SAFE_STR_CPY( npc->m_Message, str, 128 );
	
		nRui->SetDlgBalloon( enNPC, npc->m_CID );
	}

	return;
}


///----------------------------------------------------------------------------
///-- 몹들의 대화창을 생성한다. 2005.03.04 / Lyul
///----------------------------------------------------------------------------
///-- 각 Type 에 맞는 대화를 찾아서 설정한다.
///-- '#'으로 설정되어 있다면 대화창을 띄우지 않는다.
void CRockClient::SetMonsterMessage( DWORD pcid, NMonsterChatType atype , int RandomRatio /* = 0  */)
{
	TCHAR* str;
    
	NonPlayer* npc = g_Map.GetMonster( (long)pcid ); //-- 해당 Monster	

	SNPCBaseInfo* npc_Info;		//-- Table
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[g_Map.FindNpc( pcid )].code ); 
	
	if( npc != NULL && npc_Info != NULL )
	{
		int tbl_index = g_Map.FindNpc( (long)pcid );
		int random_val = int(( (float)rand() / (float)RAND_MAX) *10.0f); //-- 0~9까지의 random 값

		if( RandomRatio != 0 )
		{
			random_val = RandomRatio;
		}

		npc->m_CID		= pcid;
		npc->m_BType	= n_BT_NORMAL;
		npc->m_Color	= D3DCOLOR_XRGB( 255, 255, 255 );

		///-- Type 에 맞게 설정
		switch( atype )
		{
		case n_Monster_Skill: ///-- 스킬 사용시
			str = npc_Info->theAttackDlg1;
			break;
		case n_Monster_Attack: //-- 공격할 때
			if( random_val > 6 )
			{
				str = npc_Info->theAttackDlg2;
			}
			else
			{
				return;
			}
			break;

		case n_Monster_Attacked: //-- 공격당할 때
			if( random_val > 6 )
			{
				str = npc_Info->theAttackDlg0;
			}
			else
			{
				return;
			}
			break;
			
		case n_Monster_Wait: //-- 기다릴 때
			if(random_val > 7 )
			{
				str = npc_Info->theWaitDlg;
			}
			else
			{
				return;
			}
			break;

		case n_Monster_Move: //-- 이동할 때 
			if(random_val > 7 )
			{
				str = npc_Info->theMoveDlg;
			}
			else
			{
				return;
			}
			break;

		case n_Monster_Rengen: //-- Regen 될 때
			str = npc_Info->theRegenDlg;
			break;

		case n_Monster_Die: //-- 죽을 때
			str = npc_Info->theDieDlg;
			break;
			
		}
		//-- 입력
		if( str[0] != '#')
		{
			SAFE_STR_CPY( npc->m_Message, str , 128 );		
			nRui->SetDlgBalloon( enMONSTER, npc->m_CID );
		}
	}	
	return;
}


//-----------------------------------------------------------------------------


//..............................................................................................................
// 로그인 / 서버 선택 처리 루틴
//..............................................................................................................
HRESULT	CRockClient::LoginStepProcess(void)
{
	switch( GetLoginStep() )
	{
		//......................................................................................................
		// 로그인창이 떠있는 상태 
		//......................................................................................................
		case nNotConnect:	
		{
			/// RUI : LoginWndProc() 에서 CRockClient::LoginProc()를 호출하도록 수정 
		}
		break;

		//......................................................................................................
		// 서버 리스트창이 떠있는 상태 
		//......................................................................................................
		case nServerSelectWait:
		{

		}
		break;
	}

	return S_OK;
}

//..............................................................................................................
// 캐릭터 선택 / 생성 처리 루틴
//..............................................................................................................
HRESULT	CRockClient::CharacterSelectStepProcess(void)
{
	switch( GetLoginStep() )
	{
		//......................................................................................................
		// 캐릭터 리스트가 보이는 상태. 사용자의 input wait
		//......................................................................................................
		case nCharacterSelectWait:
		{
		/*
		//..................................................................................................
		// ESC 키 검사 ( = 종료 )
		//..................................................................................................
		if( theKey[DIK_DELETE].Action == n_baUp )	
		{
		// 캐릭터 지우기 루틴 넣어 Boa 요 
		if( CharSelectWnd.GetState() == CSW_SELECT_EXIST )
		{
		Send_RpCsDeleteCharacter( CharSelectWnd.m_pSelectChar->theData.thePCName );
		}
		}
			*/
		}
		
		break;
	}
	
	return S_OK;
}

//..............................................................................................................
// 로그인 소켓 닫고 로그인 상태로 돌리기
//..............................................................................................................
void CRockClient::CloseLoginState(void)
{
	CleanUpSocket( theLoginSocketPtr );

	theLoginStep = nNotConnect;

	m_dwGameMode = GMODE_LOGINPAGE;
}



void CRockClient::LogOff()
{
	//. 게임서버소켓닫기( theGameSocketPtr = NULL; )
	//. 로그인소켓닫기( theLoginSocketPtr = NULL; )
	CleanUpSocket( theGameSocketPtr  );
	CleanUpSocket( theLoginSocketPtr );
	
	//. 게임모드변경 -> GMODE_LOGINPAGE
	theLoginStep = nNotConnect;
	m_dwGameMode = GMODE_LOGINPAGE;

	m_bConnect = false;

	if(g_Pc.GetPlayer()->m_State.bRide)
	{
		SetRideOff(MAX_USER_INTABLE);
	}

	//	주석 처리함 .. g_Pc.GetPlayer()->Clear();
	//by dongs g_Pc.GetPlayer()->Clear(); 완전히 안지워 진다 .. 이유는 추후 .;; 지금 라이닝 
	g_Pc.GetPlayer()->Destroy()	;

	m_bHackStart= false; 


	//by simwoosung 마우스 커서 복구
	if(m_IsMouseRotate)
	{
		nRui->SetCursorType(n_CT_NORMAL);
		m_IsMouseRotate = false;
	}	
	
	if( g_RockClient.GetLanguage() == eLang_Korea )
	{
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NIGHT );
	}
	else
	{
		g_Particle.m_WeatherDrop.SetDropType( WEATHER_TYPE_NONE );
	}

	//by simwoosung - 파티 멤버 정보창의 전환
	if(g_PartyInforWnd.IsMemberParty())
	{
		list< SPARTYMEMBER_INFO * >::iterator i = g_PartyInforWnd.GetBeginMember();
		list< SPARTYMEMBER_INFO * >::iterator end = g_PartyInforWnd.GetEndMember();

		for( ; i != end; i++ )
		{
			SPARTYMEMBER_INFO *pMemberInfo = (*i);
		    if(pMemberInfo->m_dPCID == g_Pc.m_MainPC.lUnique)
			  continue;
			pMemberInfo->m_bIsViewRange = FALSE;					
		}
	}

	g_Particle.m_WeatherDrop.m_WasBeforeNight = FALSE;
	
	//. 맵체인지 -> 0
	MapClear( 0 );
	
	if( MapCreate( 0 , 184, 328 ) == false )
	{	
		ClientClose( g_RockClient.GetApp() , 1);
		return;
	}
	
	g_Particle.m_WeatherDrop.Init();

	CharSelectWnd.StartCameraPosition();

	g_Particle.m_Fade.Start( FADE_STATE_LOOP , g_Particle.m_Fade.GetColor() , 
			D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) ,
			D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) ,
			1 , 2500 , FALSE );
				
	g_RockClient.m_LoginShutter.Reset();
}

void CRockClient::SelecttoServerList(void)
{
	//. 게임서버소켓닫기( theGameSocketPtr = NULL; )
	CleanUpSocket( theGameSocketPtr  );

	//. 게임 모드 변경.
	theLoginStep = nServerSelectWait;
	m_dwGameMode = GMODE_LOGINPAGE;

	//. 맵체인지 -> 0
	MapClear( 0 );

	if( MapCreate( 0 , CharSelectWnd.PosInfo[ CSW_SELECT_POS ].CelPos[ 0 ], CharSelectWnd.PosInfo[ CSW_SELECT_POS ].CelPos[ 1 ] ) == false )
	{
		ClientClose( g_RockClient.GetApp() , 1);
		return;
	}
}

//
void  CRockClient::CheckProtocol(void * aMsg, BOOL IsRecv)
{
	
#ifdef _DEBUG

	///-- 1인이 클라이언트 실행시 주고받는 패킷의 총 사이즈와 횟수를 초당 저장하는
	///-- 기능을 추가한 GM 클라이언트 작업
	
	SRpHeader * header = NULL;	
	header = static_cast< SRpHeaderPtr > ( aMsg );

	if( header == NULL )
	{
		return;
	}

#ifdef SAVE_PINFO_LOG_BUTTON
	
	if( g_SPacketInfoPerSec.IsSavePacketInfoPerSec )
	{
		if( IsRecv )
		{
			g_SPacketInfoPerSec.nRecvPacCount++;
			g_SPacketInfoPerSec.dRecvPacSize += (header->theSize);
		}
		else
		{
			g_SPacketInfoPerSec.nSendPacCount++;
			g_SPacketInfoPerSec.dSendPacSize += (header->theSize);
		}		
		
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , g_SPacketInfoPerSec.dPrePInfoSaveTime ) , >= , 1000 ) )
		{
			Filelog("\t%d\t%u\t%d\t%u", g_SPacketInfoPerSec.nRecvPacCount, g_SPacketInfoPerSec.dRecvPacSize,
										g_SPacketInfoPerSec.nSendPacCount, g_SPacketInfoPerSec.dSendPacSize );
			
			g_SPacketInfoPerSec.nRecvPacCount = 0;
			g_SPacketInfoPerSec.dRecvPacSize = 0;
			g_SPacketInfoPerSec.nSendPacCount = 0;
			g_SPacketInfoPerSec.dSendPacSize = 0;			
			g_SPacketInfoPerSec.dPrePInfoSaveTime = (DWORD)timeGetTime();
		}		
	}
	else
	{
		g_SPacketInfoPerSec.nRecvPacCount = 0;
		g_SPacketInfoPerSec.dRecvPacSize = 0;
		g_SPacketInfoPerSec.nSendPacCount = 0;
		g_SPacketInfoPerSec.dSendPacSize = 0;
		g_SPacketInfoPerSec.dPrePInfoSaveTime = (DWORD)timeGetTime();
	}
	
#endif
	
	thePacketTransLogDlg.PrintPacketTransInfo((void *)aMsg, IsRecv);

#endif

}

//.............................................................................................................
// 사교 동작 관련 ISKeeping = True -> 사전동작없이 바로 지속 액션으로
//.............................................................................................................
int CRockClient::SetAction(Character *pSrc,int MotionIndex,BOOL ISKeeping)
{
	if( pSrc == NULL )
		return FALSE;
	
	if( pSrc->m_bPC == FALSE )
		return FALSE;

	LPACTION_DATA	pData = NULL;

	if( ( pData = m_Action.GetData( MotionIndex ) ) == NULL )
		return FALSE;

	EVENT_DATA_INFO temp;


	temp.Clear();
	temp.type   = SM_ACT;

	
	if(pSrc->m_State.bRide)
	{
		temp.cell = pData->ViewRide ; //말이냐 ??
		temp.number = pData->NpcAni ; //NPc 애니 번호 
		temp.tbl_Idx  =  pSrc->m_State.TransIndex; // 말 테이블 정보인거다 .. 
	}

	if( ISKeeping == TRUE ) 
	{
		// 지속 액션이 아니면 무시하기
		if( pData->ISKeep == FALSE )
			return FALSE;
		temp.status = SM_ACT_KEEPING;
	}
	else
		temp.status = SM_ACT_START;
	
	temp.count  = pData->ISKeep;

	PCTABLE	*pTable = g_Pc.FindPcTable( pSrc );

	if( pTable == NULL )
		return FALSE;
	
	temp.weaponView[0] = pData->ViewRight;
	temp.weaponView[1] = pData->ViewLeft;

	int nMotion = -1;

	if(MotionIndex >= 23  && MotionIndex <= 31 )
	{
		temp.IsHideWing = TRUE;
	}

	///-- 댄스 모션일 경우
	if( MotionIndex == 23 )
	{
		
		nMotion = g_Pc.GetDanceMotion( pTable );
		if( nMotion == -1 )
		{
			return FALSE;
		}
	}

	if( pTable->char_info.theCitizen == MILLENA )
	{
		if( pTable->char_info.theGender == MALE )
		{
			if( ISKeeping == TRUE )
			{
				temp.motion = pData->KeepAni[ 0 ];
				///-- 댄스모션일 경우 
				if( MotionIndex == 23 )
				{
					temp.motion = nMotion;
				}
			}
			else
			{
				temp.motion = pData->StartAni[ 0 ];
				temp.index  = pData->KeepAni[ 0 ];
				///-- 댄스모션일 경우 
				if( MotionIndex == 23 )
				{
					temp.motion = nMotion;
					temp.index = nMotion;
				}
			}
		}
		else
		{
			if( ISKeeping == TRUE )
			{
				temp.motion = pData->KeepAni[ 1 ];
				///-- 댄스모션일 경우 
				if( MotionIndex == 23 )
				{
					temp.motion = nMotion;
				}
			}
			else
			{
				temp.motion = pData->StartAni[ 1 ];
				temp.index  = pData->KeepAni[ 1 ];
				///-- 댄스모션일 경우 
				if( MotionIndex == 23 )
				{
					temp.motion = nMotion;
					temp.index = nMotion;
				}
			}
		}
	}
	else
	{
		if( pTable->char_info.theGender == MALE )
		{
			if( ISKeeping == TRUE )
			{
				temp.motion = pData->KeepAni[ 2 ];
				///-- 댄스모션일 경우 
				if( MotionIndex == 23 )
				{
					temp.motion = nMotion;
				}
			}
			else
			{
				temp.motion = pData->StartAni[ 2 ];
				temp.index  = pData->KeepAni[ 2 ];
				///-- 댄스모션일 경우 
				if( MotionIndex == 23 )
				{
					temp.motion = nMotion;
					temp.index = nMotion;
				}
			}
		}
		else
		{
			if( ISKeeping == TRUE )
			{
				temp.motion = pData->KeepAni[ 3 ];
				///-- 댄스모션일 경우 
				if( MotionIndex == 23 )
				{
					temp.motion = nMotion;
				}
			}
			else
			{
				temp.motion = pData->StartAni[ 3 ];
				temp.index  = pData->KeepAni[ 3 ];
				///-- 댄스모션일 경우 
				if( MotionIndex == 23 )
				{
					temp.motion = nMotion;
					temp.index = nMotion;
				}
			}
		}
	}
	 
	
	if( g_Pc.GetPlayer()->GetUnique() == pSrc->GetUnique() )
	{
		if( ISKeeping == TRUE )
		{	
			g_Pc.GetPlayer()->AnimationPlayNoneBlend( temp.motion );
		}
		else
		{
			g_Pc.GetPlayer()->PushQueue( temp, TRUE );
		}


	}
	else
	{
		if( ISKeeping == TRUE )
		{
			((NonPlayer *)pSrc)->AnimationPlayNoneBlend( temp.motion );
		}
		else 
		{
			((NonPlayer *)pSrc)->PushQueue( temp, TRUE );
		//	if(pSrc->m_State.bRide)
		///	{
		//		g_Map.m_Npc[temp.tbl_Idx].AnimationPlay(n_NPC_Attack0);
		//	}

		}
		
	}

	return TRUE;
}

void CRockClient::SetRideOn( int nPcIndex )
{

#ifdef RIDING_SYSTEM
	
	Character *pUser = NULL;
	WORD  nRidingCode = -1;
	long  lBolck = 0;
	DWORD aItemUID = 0;
	
	if( nPcIndex == MAX_USER_INTABLE ) //자신이냐...
	{
		pUser = (Character *)g_Pc.GetPlayer();
		nRidingCode = nRui->thePcInven.WearItem[n_WearSlot_Riding]->Code;
		lBolck = g_Pc.GetlBolck();
		aItemUID = nRui->thePcInven.WearItem[n_WearSlot_Riding]->ItemUID;
	}	
	else
	{
		pUser = (Character *)(&g_Pc.m_Pc[nPcIndex]);
		nRidingCode = g_Pc.m_PcTable[nPcIndex].char_info.theWearItem[n_WearSlot_Riding].theCode;
		lBolck = g_Pc.m_PcTable[nPcIndex].lBlock;
		aItemUID = g_Pc.m_PcTable[nPcIndex].theRidingUID;
	}

	if( pUser == NULL )
	{
		return;
	}

	if( pUser->m_State.bRide )
	{
		return;
	}	

	SItemBaseInfo* item_Info = NULL;
	item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRidingCode );

	if( item_Info == NULL )
	{
		return;
	}

	if(n_Class_Riding != item_Info->theClass )
	{
		return;
	}
	
	int nNPCCode = item_Info->theReHP;	
	
	int npc_tbl_index = g_Map.FindNpc( aItemUID, c_NpcRace_Transpotation );
	
	if( npc_tbl_index == MAX_NPC_INTABLE )
	{
		npc_tbl_index = g_Map.SM__RIDE__APPEAR( nNPCCode, aItemUID, 
		lBolck, 100, 100, 0 , pUser->GetDirection() );
	}
	else
	{
		RLG1( "Same Riding Id %d", aItemUID);
		return;
	}

	pUser->m_State.bRide = true;
	pUser->m_State.nRideState = nRideState_Master;
	pUser->m_State.TransIndex = npc_tbl_index;	
			
	g_Map.m_Npc[npc_tbl_index].thePath[0] = lBolck;
	g_Map.m_Npc[npc_tbl_index].SetPriority( n_NONE , -1 );
	g_Map.m_Npc[npc_tbl_index].m_State.nRideState = nRideState_Ride;
	g_Map.m_Npc[npc_tbl_index].m_State.TransIndex =  nPcIndex;

	if( nPcIndex == MAX_USER_INTABLE ) //자신이냐...
	{
		g_Pc.GetPlayer()->SetWearItem();
		g_Pc.GetPlayer()->SetShadowSizeType(NSHADOW_SIZE_64);
	}	
	else
	{
		g_Pc.m_Pc[nPcIndex].SetWearItem();
		g_Pc.m_Pc[nPcIndex].SetShadowSizeType(NSHADOW_SIZE_64);
	}

	//타 플레이어가 Appear 될때 캐릭터 아이디를 올리기위한 임시코드 ㅠㅠ
	if( nPcIndex != MAX_USER_INTABLE )
	{
		pUser->AnimationPlayNoneBlend( n_Stand_Peace9 );		
		pUser->m_IsRenderImg = TRUE;
		pUser->AniRender();
		pUser->m_IsRenderImg = FALSE;
	}
	
	pUser->AnimationPlayNoneBlend( n_Ride_Wait );
	
	SNPCBaseInfo* npc_Info = NULL;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( nNPCCode );

	if( npc_Info )
	{
		///-- 말일 경우에만...
		if( npc_Info->theTargetRange == 0 )
		{
			g_Sound.Play( 304, pUser->GetPosTM()._41, pUser->GetPosTM()._42, pUser->GetPosTM()._43);
		}
		///-- 랩터...
		else if( npc_Info->theTargetRange == 1 )
		{
			g_Sound.Play( 341, pUser->GetPosTM()._41, pUser->GetPosTM()._42, pUser->GetPosTM()._43);
		}
		///-- 그로핀
		else if( npc_Info->theTargetRange == 2 )
		{
			g_Sound.Play( 343, pUser->GetPosTM()._41, pUser->GetPosTM()._42, pUser->GetPosTM()._43);
		}
	}

#endif

}

void CRockClient::SetRideOff(int nPcIndex)
{

#ifdef RIDING_SYSTEM
	
	Character *pUser = NULL;
	long lDestBlock = 0;

	if( nPcIndex == MAX_USER_INTABLE ) //자신이냐...
	{
		pUser = (Character *)g_Pc.GetPlayer();
		lDestBlock = g_Pc.m_MainPC.lDestBlock;
	}	
	else
	{
		pUser = (Character *)(&g_Pc.m_Pc[nPcIndex]);
		lDestBlock = g_Pc.m_PcTable[nPcIndex].lDestBlock;
	}	

	if( pUser == NULL )
	{
		return;
	}

	if( !pUser->m_State.bRide )
	{
		return;
	}

	
//	g_Sound.Play(304,	pUser->GetPosTM()._41,	pUser->GetPosTM()._42,	pUser->GetPosTM()._43);	

	g_Map.SetMapData( CLEAR_BLOCK, nPcIndex , lDestBlock , true );
	
	int nRideID = g_Map.m_NpcTable[pUser->m_State.TransIndex].lUnique; 
	
	pUser->m_State.bRide = false;
	pUser->m_State.nRideState = nRideState_Normal;
	pUser->m_State.TransIndex = -1;
	pUser->AnimationPlayNoneBlend( n_Stand_Peace9 );
	pUser->SetShadowSizeType(NSHADOW_SIZE_64);

	g_Map.SM__RIDE__DISAPPEAR( 0, nRideID );

	if( nPcIndex == MAX_USER_INTABLE ) //자신이냐...
	{
		g_Camera.Update();
		g_Camera.MovCamera();
	}

#endif
	
}

BOOL CRockClient::CanRideOnDest()
{
	///-- 상점창이 열려있는 경우 
	if( g_UserStoreSellWnd.IsVisible || g_UserStoreTitleWnd.IsVisible )
	{
		SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_CANNOT_RIDING_INSHOP ) );	
		return FALSE;
	}	
	
	BOOL IsVeloSkill = FALSE;
	BOOL IsHide = FALSE ; 
	
	for( int i = 0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if(nRui->thePcInven.KeepupSkill[i])
		{
			SSkillBaseInfo* skill_Info;
			skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( nRui->thePcInven.KeepupSkill[i] );
			
			if( nRui->thePcInven.KeepupSkill[i] >= 8004 &&
				nRui->thePcInven.KeepupSkill[i] <= 8005)
			{
				IsHide  =TRUE;
				break; 
			}
			
			if( skill_Info )
			{
				BYTE bType = skill_Info->theEffectInfo[0].theEffectType;
				if( bType == 13 || bType == 43 || bType == 73 )
				{
					IsVeloSkill = TRUE;
					break;
				}

				bType = skill_Info->theEffectInfo[1].theEffectType;
				if( bType == 13 || bType == 43 || bType == 73 )
				{
					IsVeloSkill = TRUE;
					break;
				}

				bType = skill_Info->theEffectInfo[2].theEffectType;
				if( bType == 13 || bType == 43 || bType == 73 )
				{
					IsVeloSkill = TRUE;
					break;
				}
			}
		}
	}
	

	if( IsHide  ) 
	{
		SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_HIDE_OR_SENSOR ) );	 
		return FALSE;
	}
	
	if(IsVeloSkill)
	{
		SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_CANNOT_RIDING_VELSKILL ) );	 
		return FALSE;
	}
	
	if( g_Pc.GetPlayer()->theMapType == n_FightZone )
	{
		SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( ISD_SYSPR_CANNOT_RIDING_INFROOM ) );
		return FALSE;
	}
	
	//은신 중에는 사용할수 없다.
	if( g_Pc.GetPlayer()->m_CharAlpha.GetState() == CHAR_HIDE )
	{
		SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_CANNOT_RIDING_INSKILL ) );	 
		return FALSE;
	}
	
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
	{
		SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_CANNOT_RIDING_INSKILL ) );	 
		return FALSE;
	}	 
	
	if(  g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL && 
		( g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_SITDOWN ||
		g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_KEEPING || 
		g_Pc.GetPlayer()->m_curt_event.status == SM_SELLSKILL_STANDUP ) )
	{
		SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_CANNOT_RIDING_INSHOP ) );	 
		return FALSE;
	}
	
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
	{
		SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_CANNOT_RIDING_INSKILL ) );	 
		return FALSE;
	}
	
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_MOVE_ATTACK || 
		g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
		g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_DELAY )
	{
		SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_CANNOT_RIDING_INFSTATE ) );	 
		return FALSE;
	}
	
	if(g_Pc.GetPlayer()->m_curt_event.type == SM_ACT &&  g_Pc.GetPlayer()->m_curt_event.type == SM_ACT_KEEPING || 
		g_Pc.GetPlayer()->m_curt_event.motion == n_Act_Sit0 || g_Pc.GetPlayer()->m_curt_event.motion == n_Act_Sit1 ||
		g_Pc.GetPlayer()->m_curt_event.motion == n_Act_Lie0 || g_Pc.GetPlayer()->m_curt_event.motion == n_Act_Lie1 )
	{
		SystemPrint( n_DarkRed, n_SystemMsg ,G_STRING_CHAR(IDS_SYSPR_EMOTION_NOT_USE) );	 
		return FALSE;
	}	
	
	
	return TRUE;
}

BOOL CRockClient::KillProcess(char * strExeName)
{
	char ExeName[1024] = "";
	wsprintf( ExeName , "%s" , strExeName );
	_strupr( ExeName );

	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	
	if ( (int)hSnapshot != -1 ) 
    { 
        PROCESSENTRY32 pe32 ; 
        pe32.dwSize=sizeof(PROCESSENTRY32); 
        BOOL bContinue ; 
        
		char strProcessName[ 1024 ] = ""; 
       
		if ( Process32First ( hSnapshot, &pe32 ) ) 
        { 
            do 
            { 
                wsprintf( strProcessName , "%s" , pe32.szExeFile );
				_strupr( strProcessName );               
                
				if( ( strstr(strProcessName, ExeName) != NULL ) ) 
                { 
                    HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, 0, pe32.th32ProcessID ); 
                    if( hProcess ) 
                    { 
                        DWORD       dwExitCode; 
                        GetExitCodeProcess( hProcess, &dwExitCode); 
                        TerminateProcess( hProcess, dwExitCode); 
                        CloseHandle(hProcess); 
                        CloseHandle( hSnapshot ); 
                        return TRUE; 
                    } 
                    return FALSE; 
                } 
                
				bContinue = Process32Next( hSnapshot, &pe32 ); 
            
			  }
			while ( bContinue );        
		} 
        
		CloseHandle( hSnapshot ); 
    }
	 
	return TRUE;
}

void CRockClient::GetOsVersion()
{
   OSVERSIONINFO osvi_;
   osvi_.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   GetVersionEx(&osvi_);

   switch(osvi_.dwPlatformId)
   {
      // windows 32 호환 : window 3.1 
      case VER_PLATFORM_WIN32s:
           sprintf(strOsVersion, "windows 3.1");
           break;
      // windows 32 비트 운영 체제 : win95, win98
      case VER_PLATFORM_WIN32_WINDOWS:
          switch(osvi_.dwMinorVersion)
          {
             case 0:
				 sprintf(strOsVersion, "windows 95");				 
                break;
             case 10:
                if(strcmp(osvi_.szCSDVersion , " a ") == 0 )
                   sprintf(strOsVersion, "windows 98 second edition");			
                else
                   sprintf(strOsVersion, "windows 98");
                break;
             case 90:
				   sprintf(strOsVersion, "windows me");           
                break;
             default:
				   sprintf(strOsVersion, "UnKnown windows 32");				
                break;
         }
         break;
      // windows 32 비트 nt 운영 체제 : windows nt, 
      case VER_PLATFORM_WIN32_NT:    
         switch(osvi_.dwMajorVersion)
         {
             // windows nt 3.51, windows nt 4.0
             case 3:
             case 4:
                 sprintf(strOsVersion, "windows nt %d.%d %s", 
					osvi_.dwMajorVersion , osvi_.dwMinorVersion, osvi_.szCSDVersion);
                break;
             case 5:
                if(osvi_.dwMinorVersion == 0)
                   sprintf(strOsVersion, "windows 2000");				
                else
                   sprintf(strOsVersion, "windows xp");			
                break;
             default:
                sprintf(strOsVersion, "UnKnown windows nt");		
                break;
         }

         break;
      default:
         sprintf(strOsVersion, "UnKnown windows");		 
         break;
   }
   
}

#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9 CRockClient::GetTexture(unsigned long nHashCode)
#else
LPDIRECT3DTEXTURE8 CRockClient::GetTexture(unsigned long nHashCode)
#endif // DIRECT_VERSION_9_MJH
{
	return g_Particle.m_EffectTextureManager.GetTexture(nHashCode);	
}

void CRockClient::BeginLoadingBarThread()
{
	m_MapChangeProgress = true;	
	LoadingBarWnd.Open();	
	
	g_TexManager.ClearTheCreateTexList();	
	
	if( STILL_ACTIVE == g_CLoadingBarThread.getExitCode() )
	{
		g_CLoadingBarThread.Stop();	
	}

	Sleep(100);

	//DWORD dwThreadID = 0;
	//g_hLoadingThread = (HANDLE)_beginthreadex( NULL, 0, LoadingBarThread, NULL, 0, (unsigned*)&dwThreadID );

	g_CLoadingBarThread.m_nNowStep = nLoadingNone;

	g_Particle.m_Fade.Start( FADE_STATE_NONE , D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) , 
							 D3DCOLOR_ARGB( 0 , 0 , 0 , 0 ) , 500 , FALSE );	
	
	if(!g_CLoadingBarThread.Start())
	{
		SEND_DEBUG_LOG( "맵이동 쓰레드 동작실패");
	}	
}

void CRockClient::LoadingBarSafeStopAfterProcess()
{
	///-- 로딩바 쓰레드가 종료하길 기다린다.
	while( STILL_ACTIVE == g_CLoadingBarThread.getExitCode() )
	{
		Sleep(100);
	}
	
	Sleep(100);

	if( g_CLoadingBarThread.m_nNowStep != nLoadingEnd )
	{	
		SEND_DEBUG_LOG("맵이동 로딩 쓰레드 동작실패");		
	}	

	g_CLoadingBarThread.m_nNowStep = nLoadingNone;

	NonPlayerInfoWnd.thePickedNpcID = -1;
	NonPlayerInfoWnd.thePickedUserID = -1;
							
	g_RockClient.m_MapChangeProgress = false;				
	LoadingBarWnd.IsChange	= true;

	LoadingBarWnd.Close();	///-- 로딩바를 닫는다.

	///--PRINT_DLG_LOG("맵 이동 종료");
}

void CRockClient::ResetNowTextureDetail()
{
	g_TexManager.DeleteAllChrAndNpcTex();
	g_EffItemManager.AllDeleteUsedTex();
	m_FaceExp.DeleteAllTexture();

	g_Pc.GetPlayer()->PreCharTextureCreate();

	if(m_dwGameMode == GMODE_NORMAL)	
	{
		g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
			g_Pc.m_MainPC.char_info.race , g_lpDevice, false );
		g_Pc_Manager.m_CharImage.Change( g_Pc.GetPlayer()->GetUnique() , 
			g_Pc.m_MainPC.char_info.race , g_lpDevice );
		
		g_Pc_Manager.m_CharImage.ReFreshPetImage();

		if(g_PartyInforWnd.IsMemberParty())
		{
			g_PartyInforWnd.ReFreshPMemberCharImg();
		}
	}
}

void CRockClient::CheckUseVideoMemory()
{

//#ifdef CHECK_FREE_VIDEO_MEMORY
	
	LPDIRECTDRAW7	lpDD = NULL;
	DDSCAPS2		ddsCaps2; 
	DWORD			dwTotal, dwFree;
	HRESULT			hr; 
	
	DWORD			dTotalVideoMemory = 0;
	DWORD			dFreeVideoMemory = 0;
	
	hr = DirectDrawCreateEx( NULL, (VOID**)&lpDD, IID_IDirectDraw7, NULL );
	if( SUCCEEDED( hr ) )
	{
		ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));
		ddsCaps2.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM; 
		hr = lpDD->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree); 
		if( SUCCEEDED( hr ) )
		{
			dTotalVideoMemory = dwTotal / ( 1024 * 1024 );
			dFreeVideoMemory = dwFree / ( 1024 * 1024 );
		}

		if(lpDD) lpDD->Release();
	}

	if(dFreeVideoMemory)
	{
		PRINT_DLG_LOG( "총비디오메모리 %u 사용가능 비디오메모리 %u", dTotalVideoMemory, dFreeVideoMemory );
		PRINT_DLG_LOG( "사용중인 비디오 메모리  %u", dTotalVideoMemory - dFreeVideoMemory );
	}

//#endif
  
}
//////////////////////////////////////////////////////////////////////////
//nProtect 초기화 
//////////////////////////////////////////////////////////////////////////
BOOL CRockClient::NP_Init()
{
#ifdef GAME_GUARD

#ifdef HHM_GAME_GUARD_UPDATE_20090908

	//ClientFilelog("HHM_GAME_GUARD_UPDATE_20090908 Start\n");

	//디렉토리 생성
	TCHAR file_path[256]	= {0,};
	wsprintf( file_path, "%s\\GameGuard", g_RockClient.GetRootDir() );
	DWORD dAttr =  ::GetFileAttributes(file_path);
	//ClientFilelog("%s\n", file_path);
	
	if( FILE_ATTRIBUTE_DIRECTORY != dAttr )	//해당 폴더가 존재하지 않으면...
		::CreateDirectory(file_path, NULL);	 //디렉토리를 생성한다.
	
	TCHAR NPFileStr[][30] = {// 핵쉴드 업데이트 시 파일명 입력
		"GameGuard.des"};
		
	TCHAR NPStr[256] = {0};
	BYTE byFileCount = sizeof(NPFileStr) / sizeof(NPFileStr[0]);
	//ClientFilelog("byFileCount : %d\n", byFileCount);

	for(int i = 0; i < byFileCount; ++i)
	{
		sprintf(NPStr, "%s\\%s", file_path, NPFileStr[i]);
		//ClientFilelog("%s\n", NPStr);

		if( CopyFile(NPFileStr[i],// 복사 대상 파일명
			NPStr,// 새로 만들어질 파일명
			false ) )// 새로 만들어질 파일이 이미 있을 경우의 처리를 지정하는데 이 인수가 false이면 기존 파일을 지워버리고
			// 새로운 파일을 복사한다. true이면 기존 파일이 있을 경우 이 함수는 복사를 하지 않고 에러 코드를 리턴
		{
			//TCHAR UnlinkStr[256] = {0};
			//sprintf(UnlinkStr, "%s\\%s", g_RockClient.GetRootDir(), NPFileStr[i]);
			//unlink(UnlinkStr);// 파일을 지운다
			//ClientFilelog("파일을 지운다 : %s\n", UnlinkStr);
		}
	}

	//ClientFilelog("HHM_GAME_GUARD_UPDATE_20090908 End\n");
		
#endif// #ifdef HHM_GAME_GUARD_UPDATE_20090908


	DWORD dwResult = pNpgl->Init();
	
	if(dwResult != NPGAMEMON_SUCCESS)
	{
		RTCHAR msg[256];
		//LPCSTR lpszMsg;
		RTCHAR *lpszMsg; 

		// ‘6. 주요에러코드’를 참조하여 상황에 맞는 메시지를 출력해줍니다.
		switch (dwResult)
		{
		case NPGAMEMON_ERROR_EXIST:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_EXIST);
			break;
		case NPGAMEMON_ERROR_GAME_EXIST:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_GAME_EXIST);
			break;
		case NPGAMEMON_ERROR_INIT:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_INIT) ;
			break;
		case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
		case NPGAMEMON_ERROR_NFOUND_GG:
		case NPGAMEMON_ERROR_AUTH_INI:
		case NPGAMEMON_ERROR_NFOUND_INI:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_AUTH_GAMEGUARD) ;
			break;
		case NPGAMEMON_ERROR_CRYPTOAPI:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_CRYPTOAPI) ;
			break;
		case NPGAMEMON_ERROR_EXECUTE:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_EXECUTE);
			break;
		case NPGAMEMON_ERROR_ILLEGAL_PRG:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_ILLEGAL_PRG) ;
			break;
		case NPGMUP_ERROR_ABORT:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_ABORT);
			break;
		case NPGMUP_ERROR_CONNECT:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_CONNECT);
			break;
		case NPGAMEMON_ERROR_GAMEGUARD:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_GAMEGUARD);
			break;
		case NPGMUP_ERROR_PARAM:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_PARAM);
			break;
		case NPGMUP_ERROR_INIT:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGMUP_ERROR_INIT);
			break;
		case NPGMUP_ERROR_DOWNCFG:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_DOWNCFG);
			break;
		case NPGMUP_ERROR_AUTH:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_AUTH);
			break;
		case NPGAMEMON_ERROR_NPSCAN:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_NPSCAN);
			break;
		case NPGG_ERROR_COLLISION:
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_COLLISION);
			break;
		default:
			// 적절한 종료 메시지 출력
			lpszMsg = G_STRING_CHAR(IDS_MSG_NPGAME_ERROR_DEFAULT);
			break;
		}
		
		Rwsprintf(msg, G_STRING_CHAR(IDS_MSG_NPGAME_GAMEGUARD_ERROR), dwResult);
		RMessageBox(NULL, lpszMsg, msg, MB_OK);
		// 게임에 맞게 종료 코드
		ClientClose( g_RockClient.GetApp() , 1);

		return FALSE;
	}

#endif 
	return TRUE;
}




//////////////////////////////////////////////////////////////////////////
//핵쉴드 초기화 함수
//////////////////////////////////////////////////////////////////////////
bool CRockClient::HackClose(TCHAR *szMsg , int flag)
{

	TCHAR szTitle[128] ={0,};
	Rsprintf(RWCHAR(szTitle) , _RT("%s") , G_STRING_CHAR(IDS_RockSoft) );
	
	// 게임 상태가 노말이 아니면 클라에서 처리 한다 . 
	if( GetGameMode() != GMODE_NORMAL && 
		GetGameMode() != GMODE_SELECT  ) 
	{
		MessageBoxWnd.Open( szMsg , 5000, NULL , true );
		return true;
	}
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	ClientFilelog("%s [%d]" , szMsg , flag );

	switch(flag)
	{
		//로그 아웃 시킨다 .. 
	case ScGUID_MAKEFAIL:
	case ScCRC_MAKEFAIL:
		
		g_Camera.Reset();
		CharSelectWnd.CloseAll();
		g_RockClient.m_IsReConnect = FALSE;
		CharSelectWnd.SetChangeSkyBox(true);
		g_Particle.DelEffSetCon( EF_CORE0 , 100 , -1 , -1 );	

		nRui->SendEvent( WID_CharSelect_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_FAIL_GAME ), 3000 , NULL );		
		theLoginStep = nNotConnect;
		m_dwGameMode = GMODE_LOGINPAGE;
		
		//로그인창 띄우기
		nRui->SendEvent( WID_Login_PasswordEdit, n_emSetText, (EPARAM)_T(""), 255, 255, 255 );
		nRui->SendEvent( WID_LoginWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );			
		nRui->SendEvent( WID_Login_UserNameEdit, n_emSetFocus, 0, 0, 0, 0 );
		
#ifdef LOGINPAGE_BGIMG_CHANGE
		nRui->EnableBGround( TRUE );
#endif

		return false;
			break;
		//exe 틀렸기 떄문에 이놈들 무조건 강제종료 .. 
	case ScCRCMSG:
	case ScGUIDMSG:
			SEND_DEBUG_LOG("%s" , szMsg);
			break; 
	case ScGUID_Error: 
	case ScCRC_Error: 					
			SEND_DEBUG_LOG("%s" , szMsg);
			break;
	case CsDETECT_HACK_PROGRAM: 	
			g_RockClient.Send_CsHackShield(CsDETECT_HACK_PROGRAM , (UCHAR *)szMsg);
			SEND_DEBUG_LOG("%s" , szMsg);
			break;
	case CsDETECT_SPEEDHACK: 	
			g_RockClient.Send_CsHackShield(CsDETECT_SPEEDHACK , (UCHAR *)szMsg);
			SEND_DEBUG_LOG("%s" , szMsg);
			break;
	case CsDETECT_KDTRACE: 	
			g_RockClient.Send_CsHackShield(CsDETECT_KDTRACE , (UCHAR *)szMsg);
			SEND_DEBUG_LOG("%s" , szMsg);
			break;
	case CsDETECT_AUTOMACRO: 	
			g_RockClient.Send_CsHackShield(CsDETECT_AUTOMACRO , (UCHAR *)szMsg);
			SEND_DEBUG_LOG("%s" , szMsg);
				
			break;
	case CsDETECT_MEMRORY_ACCESS:
			g_RockClient.Send_CsHackShield(CsDETECT_MEMRORY_ACCESS , (UCHAR *)szMsg);
			SEND_DEBUG_LOG("%s" , szMsg);
				
			break;
	case CsDETECT_ETC:
			g_RockClient.Send_CsHackShield(CsDETECT_ETC , (UCHAR *)szMsg);
			SEND_DEBUG_LOG("%s" , szMsg);
					
			break;
	default: break;
	}
	//게임 내에서 처리 한다 . 
	//MessageBoxWnd.Open( szMsg , 3000, NULL , true );
	//MessageBox(g_RockClient.GetApp(), szMsg, szTitle, MB_OK);
	//창바꿈

	//#ifdef SINGAPORE_VERSION	
	MessageBoxWnd.Open( szMsg , 5000, NULL , true );
	
	/*
	#else
			Sleep(2000);
			MessageBox(g_RockClient.GetApp() , szMsg, szTitle, MB_OK);
			SendMessage( g_RockClient.GetApp(), WM_CLOSE, NULL, 1 );
	#endif	
	*/	


return true;
}


void CRockClient::HS_PauseService()
{
#ifdef C_HACK_SHIELD
	_AhnHS_PauseService(AHNHS_CHKOPT_MESSAGEHOOK);
#endif
	
}

void CRockClient::HS_ResumeService()
{
#ifdef C_HACK_SHIELD
	_AhnHS_ResumeService(AHNHS_CHKOPT_MESSAGEHOOK);
#endif
}

// 핵쉴드 초기화
BOOL CRockClient::HS_Init()
{

#ifdef C_HACK_SHIELD	
	const DWORD NOT_FILE = 0xFFFFFFFF;

	//디렉토리 생성
	TCHAR file_path[256]	= {0,};
	wsprintf( file_path, "%s\\hshield", g_RockClient.GetRootDir() );
	DWORD dAttr =  ::GetFileAttributes(file_path);
	
	if( FILE_ATTRIBUTE_DIRECTORY != dAttr )	//해당 폴더가 존재하지 않으면...
	{
		if(!::CreateDirectory(file_path, NULL))	 //디렉토리를 생성한다.
		{
			//MessageBox( NULL, "Failed to create Folder","Rocksoft" , MB_ICONINFORMATION );
		}
	}

	TCHAR HSFileStr[][30] = {// 핵쉴드 업데이트 시 파일명 입력

 #ifdef HHM_HACK_SHIELD_UPDATE_20090703
 
		"3N.mhe",
		"AhnRpt.exe",
		"ahnrpt.ini",
		"BldInfo.ini",
		"EHSvc.dll",
		"HShield.dat",
		"HsLogMgr.exe",
		"psapi.dll",
		"v3pro32s.dll",
		"v3warpns.v3d",
		"v3warpds.v3d"};

#else

		"3N.mhe",
		"BldInfo.ini",
		"egrnap.dll" ,
		"egrnapx2.dll",
		"ehsvc.dll", 
		"HShield.dat",
		"psapi.dll" ,
		"v3pro32s.dll",
		"v3warpns.v3d",
		"v3warpds.v3d"};

#endif// #ifdef HHM_HACK_SHIELD_UPDATE_20090703
	
	TCHAR HsStr[256] = {0};
	BYTE byFileCount = sizeof(HSFileStr) / sizeof(HSFileStr[0]);
	for(int i = 0; i < byFileCount; ++i)
	{
		sprintf(HsStr, "%s\\%s", file_path, HSFileStr[i]);
		if( CopyFile(HSFileStr[i],// 복사 대상 파일명
			HsStr,// 새로 만들어질 파일명
			false ) )// 새로 만들어질 파일이 이미 있을 경우의 처리를 지정하는데 이 인수가 false이면 기존 파일을 지워버리고
					 // 새로운 파일을 복사한다. true이면 기존 파일이 있을 경우 이 함수는 복사를 하지 않고 에러 코드를 리턴
			unlink(HSFileStr[i]);// 파일을 지운다
	}

	int		nRet = 0;
	char	szFullFilePath[MAX_PATH] ="";
	TCHAR	szMsg[MAX_PATH];
	DWORD	dwOption = 0;
	
	GetModuleFileName( NULL, szFullFilePath, sizeof( szFullFilePath ) );
	int i = 0;
	for( i = ( strlen( szFullFilePath ) - 1 ); i >= 0; --i )
	{
		if( szFullFilePath[i] == '\\' || szFullFilePath[i] == '/' )
		{
			szFullFilePath[i] = '\0';
			break;
		}
	}

	if (i < 0)
	{
		i = 0;
		szFullFilePath[i] = '\0';
	}	

	// ① 핵쉴드 폴더의 EhSvc.dll 위치를 지정합니다.
	strcat( szFullFilePath, "\\HShield\\EhSvc.dll");
	
	// ② _AhnHS_Initialize 함수 호출에 쓰일 옵션 플래그를 정의합니다
  
		//	디버거시 
#ifdef _DEBUG	

	dwOption = NULL;
//   	dwOption =	AHNHS_ALLOW_SVCHOST_OPENPROCESS |
// 				AHNHS_ALLOW_LSASS_OPENPROCESS |
// 				AHNHS_ALLOW_CSRSS_OPENPROCESS |
// 				AHNHS_DONOT_TERMINATE_PROCESS |
// 				AHNHS_CHKOPT_HSMODULE_CHANGE ;
               
#else 
	    	   
		dwOption = 
				AHNHS_CHKOPT_SPEEDHACK|
				AHNHS_CHKOPT_READWRITEPROCESSMEMORY|
				AHNHS_CHKOPT_KDTRACER|
		//		AHNHS_CHKOPT_AUTOMOUSE|
		//		AHNHS_CHKOPT_MESSAGEHOOK| //기능키 까지 막음으로서 이옵션은 푼다 .. 후에 처리 .. 
				AHNHS_CHKOPT_PROCESSSCAN|
				AHNHS_CHKOPT_OPENPROCESS|

				AHNHS_ALLOW_SVCHOST_OPENPROCESS |
				AHNHS_ALLOW_LSASS_OPENPROCESS |
				AHNHS_ALLOW_CSRSS_OPENPROCESS |
				AHNHS_DONOT_TERMINATE_PROCESS |
				AHNHS_CHKOPT_HSMODULE_CHANGE |
				AHNHS_USE_LOG_FILE;
#endif

	         
	        
// ③ _AhnHS_Initialize 함수를 호출하여 핵쉴드 서비스를 초기화 합니다.
	char HackKey[256]="";
	int HackGameCode = 0 ; 

#if defined(KOREA_VERSION)
	SAFE_STR_CPY(HackKey,"05B3D626774F037D8ABFF85C", 256);
	HackGameCode = 9301;
///--  임시적으로 일본에 한국 핵쉴드를 적용한다.
#elif defined(TAIWAN_VERSION)
	SAFE_STR_CPY(HackKey,"05B3D626774F037D8ABFF85C", 256);
	HackGameCode = 9301;
#elif defined(JAPAN_VERSION)
	SAFE_STR_CPY(HackKey,"05B3D626774F037D8ABFF85C", 256);
	HackGameCode = 9301;
#elif defined(CHINA_VERSION)
	SAFE_STR_CPY(HackKey,"0C4AD169484C368159F562EF" ,256);
	HackGameCode = 5742;
#else
	SAFE_STR_CPY(HackKey, _RT("") , 256 );
#endif



	nRet =  _AhnHS_Initialize( szFullFilePath, 
							   AhnHS_Callback,		// 콜백함수
							   HackGameCode, 				// 게임코드
  							   HackKey, 	// 라이센스 키
					     	   dwOption,
							   AHNHS_SPEEDHACK_SENSING_RATIO_NORMAL );	



	// ④ _AhnHS_Initialize 함수의 리턴 값을 검사하여 에러 처리합니다.

	if ( (nRet != HS_ERR_OK) )
	{    
		switch( nRet )
		{
		case HS_ERR_COMPATIBILITY_MODE_RUNNING:
			
			Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_MODE_RUNNING), nRet );
			break;
		case HS_ERR_NEED_ADMIN_RIGHTS:
			Rsprintf(RWCHAR(szMsg),  G_STRING_CHAR(HAKC_NEED_ADMIN_RIGHT), nRet );	
			break;
		case HS_ERR_INVALID_FILES:
			Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_INVALID_FILES), nRet );
			break;
		case HS_ERR_INIT_DRV_FAILED:
			Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_INIT_DRV_FAILED), nRet );
			break;
		case HS_ERR_INVALID_LICENSE:
			Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_INVALID_LICENSE), nRet );
			break;
		case HS_ERR_DEBUGGER_DETECT:
			Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_DERBUGGER_DETECT), nRet );
			break;
		default:
			Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_DETECT_ABNORMAL), nRet );
			break;
		}
		
		RMessageBox( NULL, RWCHAR(szMsg), G_STRING_CHAR(IDS_RockSoft), MB_OK );
		
		
		return FALSE;
	}
    
#endif	

	return TRUE;
}

BOOL CRockClient::HS_StartService()
{
    
#ifdef C_HACK_SHIELD

	int		nRet = 0;
	TCHAR	szMsg[MAX_PATH];
	
	// ① _AhnHS_StartService 함수를 호출하여 핵쉴드 서비스를 시작합니다.
	nRet = _AhnHS_StartService();
	
	// ② _AhnHS_StartService 함수의 리턴 값을 검사하여 에러 처리합니다.
	if ( nRet != HS_ERR_OK )
	{
		switch ( nRet )
		{
		case HS_ERR_START_ENGINE_FAILED:
		case HS_ERR_DRV_FILE_CREATE_FAILED:
		case HS_ERR_REG_DRV_FILE_FAILED:
		case HS_ERR_START_DRV_FAILED:
		default:
			Rsprintf(RWCHAR(szMsg), G_STRING_CHAR(HACK_DETECT_ABNORMAL), nRet );
			break;
		}
		RMessageBox( NULL, RWCHAR(szMsg), G_STRING_CHAR(IDS_RockSoft), MB_OK );
	
		return FALSE;
	}

#endif

	return TRUE;
}


BOOL CRockClient::HS_StopService()
{

#ifdef C_HACK_SHIELD
	int	nRet = 0;
	// ① _AhnHS_StopService 함수를 호출하여 핵쉴드 서비스를 정지합니다.
	nRet = _AhnHS_StopService();
	
	if ( nRet != HS_ERR_OK ) 
	{
		return FALSE;
	}

#endif

	return TRUE;
}




BOOL CRockClient::HS_UnInit()
{

	g_VFInterface.CloseFileSystem();
#ifdef C_HACK_SHIELD
	int	nRet = 0;
	
	// ① _AhnHS_Uninitialize 함수를 호출하여 핵쉴드 서비스를 종료합니다.
	nRet = _AhnHS_Uninitialize();
	
	if ( nRet != HS_ERR_OK ) 
	{
		return FALSE;
	}
#endif
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//쉐우드 계정 생성 
//////////////////////////////////////////////////////////////////////////

BOOL CRockClient::HS_ShadowUserCreate()
{     
/*
#ifdef C_HACK_SHIELD
	DWORD dwRet = HSUSERUTIL_ERR_OK;

	dwRet = _AhnHsUserUtil_CreateUser ( );

	//폴더 권한주기 
	if( dwRet ==  HSUSERUTIL_ERR_NOT_ADMIN)
	{
		return TRUE;
	}
	if(dwRet != HSUSERUTIL_ERR_OK  )
	switch ( dwRet )
	{
		case HSUSERUTIL_ERR_NOT_NT:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_NOT_NT", MB_OK );
//			AfxMessageBox ( "HSUSERUTIL_ERR_NOT_NT" );
			break;
		case HSUSERUTIL_ERR_DELSHADOWACNT_FAIL:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_DELSHADOWACNT_FAIL", MB_OK );
			break;
        case HSUSERUTIL_ERR_DELHIDEIDINFO_FAIL:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_DELHIDEIDINFO_FAIL", MB_OK );
			break;
		case HSUSERUTIL_ERR_ADDSHADOWACNT_FAIL:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_ADDSHADOWACNT_FAIL", MB_OK );
			break;
		default:
	//		AfxMessageBox ( "HSUSERUTIL_ERR_UNKNOWN" );
			break;	
	}
#endif
*/
	return TRUE;
}


BOOL CRockClient::HS_FolderPermission()
{
/*
	// TODO: Add your control notification handler code here
#ifdef C_HACK_SHIELD
	TCHAR szbuf[MAX_PATH] = { 0, };
	TCHAR szMsg [MAX_PATH] = "";

	_stprintf(szbuf, _T("%s"), g_RockClient.GetRootDir() );

	dwRet = _AhnHsUserUtil_SetFolderPermission(szbuf); 

	if( dwRet == HSUSERUTIL_ERR_OK)
	{
		return TRUE;
	}
	
	szMsg

	switch ( dwRet )
	{
		case HSUSERUTIL_ERR_NOT_NT:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_NOT_NT", MB_OK );
			break;
		case HSUSERUTIL_ERR_SETFLDRPERMISSION_FAIL:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_SETFLDRPERMISSION_FAIL", MB_OK );
			break;
        case HSUSERUTIL_ERR_GETGROUPSID_FAIL:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_GETGROUPSID_FAIL", MB_OK );
			break;
		case HSUSERUTIL_ERR_GETSECINFO_FAIL:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_GETSECINFO_FAIL", MB_OK );
			break;
		case HSUSERUTIL_ERR_ADDNEWACE_FAIL:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_ADDNEWACE_FAIL", MB_OK );
			break;
		case HSUSERUTIL_ERR_ADDNEWDACL_FAIL:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_ADDNEWDACL_FAIL", MB_OK );
			break;
		case HSUSERUTIL_ERR_COPYOLDDACL_FAIL:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_COPYOLDDACL_FAIL", MB_OK );
			break;
		case HSUSERUTIL_ERR_ADDNEWACETONEWDACL_FAIL:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_ADDNEWACETONEWDACL_FAIL", MB_OK );
			break;
		case HSUSERUTIL_ERR_OK:
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_OK", MB_OK );
			break;
		default:
			_stprintf(szMsg , "ERROR CODE : %X \n", dwRet )
			MessageBox( NULL, szMsg, "HSUSERUTIL_ERR_OK", MB_OK );
						
			break;	
	}
#endif
*/
	return TRUE ;

}

bool CRockClient::IsHolySkill(const int Code)
{
	const int HolySkillMin = 5541 ; 
	const int HolySkillMax = 5600 ; 

	if( Code >= HolySkillMin && Code <= HolySkillMax )
	{
		return TRUE;
	}


	return FALSE;
}

BOOL CRockClient::CanAttackPlayer( Character * pCharacter )
{
	if( pCharacter && pCharacter->m_bPC && pCharacter->m_nTableIndex != MAX_USER_INTABLE )
	{		
		if( g_Pc.GetPlayer()->theMapType == n_FightZone )
		{
			if( g_Pc.m_PcTable[pCharacter->m_nTableIndex].nFlag == g_Pc.m_MainPC.nFlag )
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		else
		{
			///-- 같은 국가일 경우
			if( g_Pc.m_PcTable[pCharacter->m_nTableIndex].char_info.theCitizen == 
				g_Pc.m_MainPC.char_info.theCitizen )
			{
				
#ifdef CPK_SYSTEM			
				
				BYTE bPKMode = g_Pc.GetPlayer()->GetPKMOde();
				
				switch( bPKMode )
				{
					case n_PKOFFMode:
						{
							return FALSE;
						}
						break;
					case n_PKONMode:
					case n_APKMode:
						{
							///-- 마을일 경우에는 PK불가
							if( g_Map.m_uMapType == MAP_TYPE_TOWN )
							{
								return FALSE;
							}						
							
							///-- PK모드 일 경우에도 파티나 리그멤버일 경우에는 PK가 이루어지지 않는다.
							if( ::IsMyPartyAndLeagueMember( (DWORD)g_Pc.m_PcTable[pCharacter->m_nTableIndex].lUnique ) )
							{
								return FALSE;
							}						
							
							return TRUE;
						}
						break;
					case n_DPKMode:
						{
							if( g_Map.m_uMapType == MAP_TYPE_TOWN )
							{
								return FALSE;
							}						
							
							if( g_Pc.GetPlayer()->IsMyAttacker( (DWORD)g_Pc.m_PcTable[pCharacter->m_nTableIndex].lUnique ) )
							{
								return TRUE;
							}
							
							return FALSE;						
						}
						break;
				}
#endif
				return FALSE;

			}
			else
			{
				///-- 타국은 무조건 공격 가능
				return TRUE;
			}
		}
	}

	return FALSE;
}