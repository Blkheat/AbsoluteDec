#include "RockPCH.h" 

#include	"quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>
#include	"RockClient.h"
#include	"Map.h"
#include	"Pc.h"
#include	"Npc.h"
#include	<EFFECT\\Particle.h>
#include	"Effect.h"
#include	<SOUND\Sound.H>
#include	"Camera.H"
#include	"MapDataManager.h"
#include	"CLoadingBarThread.h"
#include	"CNetProcThread.h"
#include	"GTimer.h"

///
#include "profile\\profile.h"
#include "global.h"
#include "RockInterface\\Rui.h"
#include "RockInterface\\ServerListWndProc.h"
#include "RockInterface\\CharSelectWndProc.h"
#include "RockInterface\\MiniMapWndProc.h"
#include "RockInterface\\LoadingWndProc.h"
#include "RockInterface\\NonPlayerInfoWnd.h"
#include "RockInterface\\PlayerInfoWnd.h"
#include "RockInterface\\MessageBox.h"
#include "RockInterface\\MessageWndProc.h"

#include "RockInterface\\PartyInforWnd.h"
#include "RockInterface\\MCommunityWnd.h"
#include "RockInterface\\OptionWndProc.h"
#include "RockInterface\\ChatMainProc.h"
#include "RockInterface\\SystemPrintWnd.h"
#include "RockInterface\\CLogWnd.h"
#include "RockInterface\BattleZoneWndProc.h"
#include "RockInterface\MainMenuWndProc.h"
#include "bravolibs\effect\CEFAddEffectBase.h"
#include "RockInterface\\CUserStoreSellWnd.h"
#include "RockInterface\\CUserStoreBuyWnd.h"
#include "RockInterface\\CWebShopWnd.h"
#include "RockInterface\\PayItemWndProc.h"
#include "RockInterface\\FrontierCreateWnd.h"
#include "RockInterface\\FrontierMainWndProc.h"
#include "RockInterface\\UIMsgWnd.h"
#include "RockInterface\\CDelayBarWnd.h"
#include "RockInterface\\CColMapPositionWnd.h"
#include "RockInterface\\CAcryEnchantWnd.h"
#include "RockInterface\\CAcryEnchantRWnd.h"

#include "Squence\\CThreeWayDisplay.h"
#include "Squence\\CRotDisplay.h"
#include "Squence\\CChangeClass.h"

 
#include "RockInterface\ItemSelectEldaMixWndProc.h"
#include "RockInterface\ItemSelectPotionWndProc.h"

#include "RockInterface\\CGuideWnd.h"
#include "RockInterface\\Announcement.h"
#include "RockInterface\\RadioBroadcast.h"
#include "RockInterface\\AddEffectMsg.h"
#include "CLogoDisplayThread.h"
#include "RockInterface\\InventoryWndProc.h"
#include "RockInterface\\ItemEldaMixAWndProc.h"
#include "RockInterface\\ItemEldaMixRWndProc.h"

#include "RockInterface\\CChangeEldaColorWnd.h"
#include "RockInterface\\CChangeEldaColorRWnd.h"
#include "Rockinterface\\NpcChatWnd.h"

#include "RockInterface\\EssenceWndProc.h"
#include "RockInterface\\SpellStoneWndProc.h"


#include "RockInterface\\StrengthStoneWndProc.h"
#include "RockInterface\\ItemEnchantWndProc.h"
extern Texture				g_LogoTex;
extern Texture				g_EffTex1;
extern Texture				g_EffTex2;
extern Texture				g_EffTex3;
extern Texture				g_LoadingTex;

extern CLogoDisplayThread	g_CLogoDisplayThread;


extern unsigned int __stdcall LoadingBarThread( void* arg );

extern HWND	g_hEditWnd;
int			g_chr_face  = 0;
int			g_chr_count = 0;
int			g_obj_face = 0;
int			g_chr_anicount = 0;

bool		g_bShowDebugStr	= false;		
bool		g_bBlockDebug	= false;
bool		g_bShowCollisionBox	= false;

extern		int		g_nobjupdated;
extern		int		g_grsupdated;
extern		short	lodpatchvisible;


bool	AniDemo = false;	/// 임시. 데모 
int Ani_Type = 0;
static int changeKeyA = n_Attack0_Basic0;
static int changeKeyZ = n_Attack0_KeyZ0;
static int changeKeyX = n_Attack0_KeyX0;
static int changeKeyC = n_Attack0_KeyC0;

extern		long	g_nowTime;
extern		int		g_Shadow;
extern		int     g_ViewLODTerrain;
extern		int		g_LODRange;
extern		LONG	g_EffectSoundVol;
extern		BOOL	g_LogPrint;
extern		int		g_nTextureDetail;

bool	bViewRealHP = false;

#ifdef HHM_1600X1200_TEST// Radeon 9550 해상도 1600 X 1200 창모드에서 속도 저하(2009/05/15)

	extern bool g_1600x1200;

#endif// #define HHM_1600X1200_TEST// Radeon 9550 해상도 1600 X 1200 창모드에서 속도 저하(2009/05/15)


/// sooree
void write_JPEG_file( char* filename, int quality, int image_width, int image_height, BYTE* image_buffer );

void All_New_g_Variable_Ptr()
{
	///-- g_RockClient를 전역 포인터로 하니까 디바이스 리셋이 안됨..;;
	///-- 이유 불명
	
	///-- UI관련 전역변수 포인터
	g_ColMapPositionWnd = SAFE_NEW( CColMapPositionWnd );

	///-- 가이드창 - 모험백서
	g_pGuideWnd = SAFE_NEW( CGuideWnd );
	
	///-- 네트워크 루틴 임시 쓰레드 클래스
	g_pCNetProcThread = SAFE_NEW( CNetProcThread );

	//라이오 방송
	g_pRadioBroadcast = SAFE_NEW(CRadioBroadcast);
	
	//부가효과 메세지 
	g_pAddEffectMsg		= SAFE_NEW(AddEffectMsg);

	//장신구 가공창
	g_pAcryEnchantWnd = SAFE_NEW( CAcryEnchantWnd );
	//장신구 가공 결과창
	g_pAcryEnchantRWnd = SAFE_NEW( CAcryEnchantRWnd );	

	//엘다스톤 색상 변경창
	g_pChangeEldaColorWnd = SAFE_NEW( CChangeEldaColorWnd );

	//엘다스톤 색상 변경 결과창
	g_pChangeEldaColorRWnd = SAFE_NEW( CChangeEldaColorRWnd );
#ifdef C_SUBJOB_RENEW	

	g_EssenceWndProc = SAFE_NEW( CEssenceWndProc );

	g_SpellStoneWndProc	= SAFE_NEW( CSpellStoneWndProc );
	g_StrengthStoneWndProc = SAFE_NEW(CStrengthStoneWndProc);

#endif 
}

void All_Delete_g_Variable_Ptr()
{
	///-- UI관련 전역변수 포인터
	SAFE_DELETE( g_ColMapPositionWnd );

	///-- 가이드창 - 모험백서 
	SAFE_DELETE( g_pGuideWnd );

	///-- 네트워크 루틴 임시 쓰레드 클래스
	SAFE_DELETE( g_pCNetProcThread );
	
	// 라디오 방송 
	SAFE_DELETE( g_pRadioBroadcast);
	
	// 부가효과  메세지 
	SAFE_DELETE( g_pAddEffectMsg );

	//장신구 가공창
	SAFE_DELETE( g_pAcryEnchantWnd );
	//장신구 가공 결과창
	SAFE_DELETE( g_pAcryEnchantRWnd );

	//엘다스톤 색상 변경창
	SAFE_DELETE( g_pChangeEldaColorWnd );
	//엘다스톤 색상 변경 결과창
	SAFE_DELETE( g_pChangeEldaColorRWnd );
   
#ifdef C_SUBJOB_RENEW

	SAFE_DELETE( g_EssenceWndProc );
	SAFE_DELETE( g_SpellStoneWndProc );
	SAFE_DELETE( g_StrengthStoneWndProc );

#endif 



#ifdef C_GAME_GUARD
	SAFE_DELETE(g_RockClient.pNpgl);
#endif 

}

// 스크린샷
#ifdef DIRECT_VERSION_9_MJH
HRESULT CRockClient::ScreenGrab(LPDIRECT3DDEVICE9 pDev, char *fileName , char *OutPutFileName )
#else
HRESULT CRockClient::ScreenGrab(LPDIRECT3DDEVICE8 pDev, char *fileName , char *OutPutFileName )
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hr;
	
	// get display dimensions
	// this will be the dimensions of the front buffer
	D3DDISPLAYMODE mode;
#ifdef DIRECT_VERSION_9_MJH
	if (FAILED(hr=pDev->GetDisplayMode(0 , &mode)))
#else
	if (FAILED(hr=pDev->GetDisplayMode(&mode)))
#endif // DIRECT_VERSION_9_MJH
	return hr;
		
	
	// create the image surface to store the front buffer image
	// note that call to GetFrontBuffer will always convert format to A8R8G8B8
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSURFACE9 surf;
#else
	LPDIRECT3DSURFACE8 surf;
#endif // DIRECT_VERSION_9_MJH

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/12)
#ifdef DIRECT_VERSION_9_MJH
	if (FAILED(hr=pDev->CreateOffscreenPlainSurface(m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH ,&surf , NULL )))
#else
	if (FAILED(hr=pDev->CreateImageSurface(m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height, D3DFMT_A8R8G8B8, &surf)))
#endif // DIRECT_VERSION_9_MJH

#else
#ifdef DIRECT_VERSION_9_MJH
	if (FAILED(hr=pDev->CreateOffscreenPlainSurface(1024, 768, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH ,&surf, NULL )))
#else
	if (FAILED(hr=pDev->CreateImageSurface(1024, 768, D3DFMT_A8R8G8B8, &surf)))
#endif // DIRECT_VERSION_9_MJH


#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/12)
		return hr;
	
	
	// read the front buffer into the image surface
//	if (FAILED(hr=pDev->GetFrontBuffer(surf))) 
#ifdef DIRECT_VERSION_9_MJH
	if(FAILED(hr = pDev->GetBackBuffer(0 ,0, D3DBACKBUFFER_TYPE_MONO , &surf )))
#else
	if(FAILED(hr = pDev->GetBackBuffer(0 ,D3DBACKBUFFER_TYPE_MONO , &surf )))
#endif // DIRECT_VERSION_9_MJH
	{
		surf->Release();
		return hr;
	}
	
	// BMP로 저장 
	// write the entire surface to the requested file
/*
	if( m_EWindowsMode == MODE_WINDOW_FULLMODE ) 
	{
		hr=D3DXSaveSurfaceToFile(fileName, D3DXIFF_BMP, surf, NULL, NULL);
	}
	else
	{
		RECT rect;
		GetWindowRect( g_RockClient.GetApp(), &rect );
		hr=D3DXSaveSurfaceToFile(fileName, D3DXIFF_BMP, surf, NULL, &rect);
	}	
*/
	/// JPG로 저장 

	D3DLOCKED_RECT lockRect;    
    if( SUCCEEDED( hr=surf->LockRect( &lockRect, NULL, D3DLOCK_READONLY ) ) )
    {
		RECT rcWnd;
		GetClientRect(m_hWnd, &rcWnd);
		ClientToScreen(m_hWnd, ((LPPOINT)&rcWnd));
		ClientToScreen(m_hWnd, ((LPPOINT)&rcWnd)+1);
		//lockRect.pBits=(DWORD*)0x24090000;
		//lockRect.Pitch=4096;
		
        DWORD* pdwBase=(DWORD*)lockRect.pBits;
		UINT uPitch=lockRect.Pitch / sizeof(DWORD);

	#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/12)

		UINT uWidth= m_d3dsdBackBuffer.Width; //rcWnd.right-rcWnd.left;
        UINT uHeight= m_d3dsdBackBuffer.Height;//rcWnd.bottom-rcWnd.top;

	#else

        UINT uWidth= 1024; //rcWnd.right-rcWnd.left;
        UINT uHeight= 768;//rcWnd.bottomv             -rcWnd.top;

	#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/12)
		
		//BYTE* pSrc = (BYTE*)( pdwBase+rcWnd.top*uPitch+rcWnd.left );
		BYTE* pSrc = (BYTE*)( pdwBase+0*uPitch+0 );
		BYTE* pSrc24 = NULL;
		pSrc24 = SAFE_NEW_ARRAY( BYTE , ( uWidth * 3 ) * uHeight );		
		
		if( pSrc24 == NULL )
		{
			surf->UnlockRect(); 
			surf->Release();
			return S_FALSE;
		}
		
		int idx = 0;
		
		for( int h = 0; h < uHeight; ++h )
		{
			for( int w = 0; w < uWidth; ++w )	/// BGRA => RGB로 변환 
			{
				pSrc24[3 * uWidth * h + 3 * w    ] = pSrc[lockRect.Pitch * h + 4 * w + 2];	/// R
				pSrc24[3 * uWidth * h + 3 * w + 1] = pSrc[lockRect.Pitch * h + 4 * w + 1];	/// G
				pSrc24[3 * uWidth * h + 3 * w + 2] = pSrc[lockRect.Pitch * h + 4 * w];		/// B
			}
		}		
	
		write_JPEG_file( fileName, 90, uWidth, uHeight, pSrc24 );		
		
		SAFE_DELETE_ARRAY(pSrc24);
		surf->UnlockRect(); 

		SystemPrint( n_LightGreen, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_SAVE_SCREENSHOT ), OutPutFileName );
	}      
	

	// release the image surface
	surf->Release();
	
	// return status of save operation to caller
	return hr;
}

HRESULT CRockClient::ScreenCapture()
{
	
	///--JAPAN_BUG_MODIFY
	char strfilename[512] = {0,};
	char strtempstr[512] = {0,};
	char strOutPutFileName[512] = {0,};

	FILE* fp;

	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );	
#ifdef UNICODE_ROCK

	wsprintfW( RWCHAR(strtempstr), _RT("%d%02d%02d%02d%02d%02d"), currTM->tm_year+1900, 
		currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec);
	
	wsprintfW( RWCHAR(strfilename) , _RT("%s\\ScreenShot\\scr%s.jpg"), g_RockClient.GetRootDirW(), RWCHAR(strtempstr) ); 
	wsprintfW( RWCHAR(strOutPutFileName) , _RT("scr%s.jpg" ) , RWCHAR(strtempstr) ); 
	
	fp = _wfopen( RWCHAR(strfilename) , _RT("r") );	

#else 

	wsprintf( strtempstr, "%d%02d%02d%02d%02d%02d", currTM->tm_year+1900, 
		currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec);
	
	wsprintf( strfilename, "%s\\ScreenShot\\scr%s.jpg", g_RockClient.GetRootDir(), strtempstr ); 
	wsprintf( strOutPutFileName , "scr%s.jpg", strtempstr ); 
	
	fp = fopen( strfilename , "r" );	
#endif
	


	if( fp == NULL )
	{

		//wsprintf( strtempstr , "%s\\ScreenShot", g_RockClient.GetRootDir() );

		//DWORD dAttr =  ::GetFileAttributes(tempstr);
		
		//if( FILE_ATTRIBUTE_DIRECTORY != dAttr )	//해당 폴더가 존재하지 않으면...
		//{
		//	return( ScreenGrab( m_lpD3DDevice, OutPutFileName , OutPutFileName ) );
		//}
		//else
		//{
			return( ScreenGrab( m_lpD3DDevice, strfilename  , strOutPutFileName ) );
		//}

	}
	else
	{
		
		fclose( fp );

#ifdef UNICODE_ROCK
		wsprintfW( RWCHAR(strfilename), _RT("%s\\ScreenShot\\scr%s%s.jpg" ) , g_RockClient.GetRootDirW(), RWCHAR(strtempstr), _RT("_") );
		fp = _wfopen( RWCHAR(strfilename) , _RT("r") );
#else 
		wsprintf( strfilename, "%s\\ScreenShot\\scr%s%s.jpg", g_RockClient.GetRootDir(), strtempstr, "_" );
		fp = fopen( strfilename , "r" );
#endif 

		if( fp == NULL )
		{
			return( ScreenGrab( m_lpD3DDevice, strfilename ,  strOutPutFileName ) );
		}
	}

	fclose( fp );	
	
	return E_FAIL;
}


//-----------------------------------------------------------------------------
// 로고 디스플레이
//-----------------------------------------------------------------------------
void		CRockClient::LogoDisplay(int nAlpha)
{	
	if( nRui )
	{
		nRui->RenderLogo(nAlpha);
	}
}

//..............................................................................................................
// 로그인 페이지 디스플레이
//..............................................................................................................
void CRockClient::LoginPageDisplay(void)
{
	//	nRui->TextOut( _T("[ Login Page ]"), 410, 5, D3DCOLOR_ARGB( 255, 250, 60, 40 ) );
	
	static bool initLoginPage = true;
	static char ip[16] = {0, };
	static short port=0;
	static char accountID[20] = {0, };
	
	if( initLoginPage ) 
	{
		GetPrivateProfileString( "NET", "IP", "", ip, sizeof( ip ), "./net.ini" );
		port = GetPrivateProfileInt( "NET", "PORT", 0, "./net.ini" );
		initLoginPage = false;
	}
	
	//	TCHAR buf[1024];
	
	switch( GetLoginStep() )
	{
		/// 로그인창이 떠있는 상태 
	case nNotConnect:
		{
		
				
			#ifdef LOGINPAGE_BGIMG_CHANGE
				nRui->EnableBGround( TRUE );
			#else
				nRui->EnableBGround( FALSE );
			#endif
			g_Map.Display();
			
			m_LoginShutter.Update();			
			
			//넷마블 접속 자동 접속기능
			if( n_JapanNetMarble == m_GameTypeInfo.theFrom && 
				g_RockClient.GetLanguage() != eLang_Korea )
				//				n_Kr_PublicWeb == m_GameTypeInfo.theFrom ||
				//				n_Kr_GameAngel == m_GameTypeInfo.theFrom )
			{
				if( !m_bConnect && LoginProc( m_GameTypeInfo.theUserID, m_GameTypeInfo.theUserPass, m_GameTypeInfo.theFrom ) )
				{
					nRui->SendEvent( WID_LoginWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );					
					m_bConnect = true;
				}
			}
		}
		break;
		
		/// 서버에 로그인 메시지 보내고 응답을 기다리는 중 
	case nAccountLogin:
		{
			//			wsprintf( buf, "IP : %s   Port:%d", ip, port );
			//			nRui->TextOut( buf, 420, 30, D3DCOLOR_ARGB( 255, 200, 220, 255 ) );
			//			nRui->TextOut( _T("Waiting server message..."), 410, 70, D3DCOLOR_ARGB( 255, 240, 140, 60 ) );
			
			//Map.Display();
			if( m_IsReConnect )
			{
				g_Map.Display();
			#ifdef LOGINPAGE_BGIMG_CHANGE
				nRui->EnableBGround( TRUE );
			#else
				nRui->EnableBGround( FALSE );
			#endif
				
				//m_LoginShutter.Update();
				
			}
			else
			{
			#ifdef LOGINPAGE_BGIMG_CHANGE
				nRui->EnableBGround( TRUE );
			#else
				nRui->EnableBGround( FALSE );
			#endif
				
				g_Map.Display();
				
				m_LoginShutter.Update();
			}
			
		}
		break;
		
		/// 서버 리스트창이 떠있는 상태 
	case nServerSelectWait:
		{
			#ifdef LOGINPAGE_BGIMG_CHANGE
				nRui->EnableBGround( TRUE );
			#else
				nRui->EnableBGround( FALSE );
			#endif
			
			g_Map.Display();
			m_LoginShutter.Update();
		}
		break;
		
		/// 서버 리스트를 고르고 서버 응답을 기다리는 중 
	case nServerSelectOK:
		{
			//			for( int i = 0; i < theServerSetNum; ++i ) 
			//			{
			//				wsprintf( buf, "<%d> SERVER Name:%s IP:%s Port:%d",	i,	theServerSetData[i].theName, 
			//																		theServerSetData[i].theIP,
			//																		theServerSetData[i].thePort );
			//				nRui->TextOut( buf, 420, 25 * i + 30, D3DCOLOR_ARGB( 255, 200, 220, 255 ) );
			//			}
			
			//			nRui->TextOut( _T("Waiting server message..."), 410, 25 * i + 30 + 10, D3DCOLOR_ARGB( 255, 240, 140, 60 ) );
			#ifdef LOGINPAGE_BGIMG_CHANGE
				nRui->EnableBGround( TRUE );
			#else
				nRui->EnableBGround( FALSE );
			#endif
			
			g_Map.Display();
			m_LoginShutter.Update();
		}
		break;
	}
}

//..............................................................................................................
// 캐릭터 선택 화면 
//..............................................................................................................
void CRockClient::CharacSelectPageDisplay(void)
{
	//	nRui->TextOut( _T("[ Character Select Page ]"), 410, 5, D3DCOLOR_ARGB( 255, 250, 60, 40 ) );
	
	//	TCHAR buf[1024];
	
	g_Map.Display();// hhm 주석 처리하면 UI를 제외한 모든 것이 안그려진다.
	m_LoginShutter.Update();
	
	switch( GetLoginStep() )
	{
		/// 캐릭터 리스트가 보이는 상태. 사용자의 input wait
	case nCharacterSelectWait:
		{
			//			for( int i = 0; i < theCharacterNum; ++i ) 
			//			{
			//				wsprintf( buf, "<%d> Name:%s Nation:%s Gender:%s", i, 
			//								theCharacterList[i].theData.thePCName, 
			//								( theCharacterList[i].theData.theCitizen == 1 )? "RAIN":"MILLENA", 
			//								( theCharacterList[i].theData.theGender == 1 )? "MALE":"FEMALE" );
			//
			//				nRui->TextOut( buf, 420, 25 * i + 30, D3DCOLOR_ARGB( 255, 200, 220, 255 ) );
			//			}
			
			//			nRui->TextOut( _T("Input the Number you want to select..."), 410, 25 * i + 30 + 10, D3DCOLOR_ARGB( 255, 240, 140, 60 ) );
		}
		break;
		
		/// 선택된 캐릭터를 서버에 전송. 메시지 기다리는 중
	case nCharacterSelectOK:
		{
			//			for( int i = 0; i < theCharacterNum; ++i ) 
			//			{
			//				wsprintf( buf, "<%d> Name:%s Nation:%s Gender:%s", i, 
			//								  theCharacterList[i].theData.thePCName, 
			//								( theCharacterList[i].theData.theCitizen == 1 )? "RAIN":"MILLENA", 
			//								( theCharacterList[i].theData.theGender == 1 )? "MALE":"FEMALE" );
			//
			//				nRui->TextOut( buf, 420, 25 * i + 30, D3DCOLOR_ARGB( 255, 200, 220, 255 ) );
			//			}
			//
			//			nRui->TextOut( _T("Waiting server message..."), 410, 25 * i + 30 + 10, D3DCOLOR_ARGB( 255, 240, 140, 60 ) );
		}
		break;
		
	}
}

//..............................................................................................................
// 캐릭터 생성 화면 
//..............................................................................................................
void CRockClient::CharacCreatePageDisplay(void)
{
	nRui->TextOut( _RT("[ Character Create Page ]"), 410, 5, D3DCOLOR_ARGB( 255, 250, 60, 40 ) );
	
	g_Map.Display();
}



//-----------------------------------------------------------------------------
int CRockClient::MouseProcess()
{
	
	static long delayTime = g_nowTime;
	static POINT pLastPos = {0 , 0};	
	
	if( theMouse.RButton == n_baDown )
	{
		GetCursorPos( &m_lastPos );		
		pLastPos = m_lastPos;
		
		ScreenToClient( m_hWnd, &pLastPos );		
	}
	else if( theMouse.RButton == n_baPress )
	{		
		POINT curPos;
		GetCursorPos( &curPos );
		ScreenToClient( m_hWnd, &curPos );
		
		POINT varPos;		
		
		varPos.x = pLastPos.x - curPos.x;
		varPos.y = pLastPos.y - curPos.y;
	//	RLG2("pLastPosX %d pLastPosY %d" , pLastPos.x , pLastPos.y );
	//	RLG1("varposX %d" , varPos.x);
	//	RLG1("varposY %d" , varPos.y);
		if( varPos.x != 0 || varPos.y != 0 )
		{
			if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( delayTime , 60 ) , < , g_nowTime ) )
			{
				if(!m_IsMouseRotate)
				{
					if(nRui->GetCursorItem())
					{
						nRui->ResetCursorItem();
					}
					
					nRui->SetCursorType(n_CT_NONE);
#ifdef DIRECT_VERSION_9_MJH
					m_SceneManager.SetCursorDisplay( FALSE );
#endif // DIRECT_VERSION_9_MJH
					m_IsMouseRotate = true;
				}
				
				ProfileBegin("< theMouse.RButton => g_Map.UpdateTerrain() >");	

				g_Camera.m_fCameraYaw += ( 0.005f * varPos.x );
				g_Camera.m_fCameraPitch -= ( 0.005f * varPos.y );
				g_Camera.Update();
				g_Camera.m_bRotBySort = true;
				g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );
				ProfileEnd("< theMouse.RButton => g_Map.UpdateTerrain() >");	
				
				delayTime = g_nowTime;				
				
				SetCursorPos(m_lastPos.x, m_lastPos.y);						
			}			
		}
		
	}
	else if(theMouse.RButton == n_baUp)
	{
		if(m_IsMouseRotate)
		{
			nRui->SetCursorType(n_CT_NORMAL);
			m_IsMouseRotate = false;
		}
	}
	
	///--------------------------------------------------------------------
	//포커스가 있는 윈도우가...스크롤 컨트롤을 가지고 있다면....거기로..WheelValue를 보낸다.
	///--------------------------------------------------------------------
	
	CFrameWnd* fwnd	= (CFrameWnd*)nRui->Chain.GetNormalWndFocusHandle();
	m_WndID		= fwnd->GetID();
	m_WndHandle	= fwnd;
	
	POINT curtMousePos;
	GetCursorPos( &curtMousePos );
	ScreenToClient( m_hWnd, &curtMousePos );
	
	if( g_MCommunityWnd.SetMessenChatWndWheelValue(theMouse) )
	{	
		int a = 0;
	}
	else if(g_ChatMainProc.m_ChatActPrintWnd.SetWheelValue(theMouse))
	{	
		int a = 0;
	}
	else if( ( m_WndID == WID_ServerListWnd ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	else if( ( m_WndID == WID_InventoryWnd ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	else if( ( m_WndID == WID_Quest_Wnd ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	else if( ( m_WndID == WID_StoreWnd ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	else if( ( m_WndID == WID_SkillWnd ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	else if( ( m_WndID == WID_Exchange_Wnd ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	else if( ( m_WndID == WID_SkillLearn_Wnd ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	} 
	else if( ( m_WndID == WID_REPAIR_WND ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	else if( ( m_WndID == WID_MCommunityBgUI ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	else if( ( m_WndID == g_LogWnd.GetFrameID() ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}	
	else if( ( m_WndID == WID_BZ_ROOMLIST_WND ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	else if( ( m_WndID == WID_BZ_WAITINGROOM_WND ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	// 개인 상점 팔기 창
	else if( ( m_WndID == g_UserStoreSellWnd.m_pFrame->GetID() ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	// 개인 상점 사기 창
	else if( ( m_WndID == g_UserStoreBuyWnd.m_pFrame->GetID() ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	// 아이템 구매 리스트 창 
	else if( ( m_WndID == g_PayItemWnd.m_FrameWnd->GetID() ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	// 프론티어 회원 리스트 창 
	else if( ( m_WndID == g_FrontierMemberListWnd.m_pFrameWnd->GetID() ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	///-- 가이드창 - 모험백서
	else if( ( m_WndID == g_pGuideWnd->m_pGuideFrameWnd->GetID() ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	///-- 가이드창 - 모험백서
	else if( ( m_WndID == g_ItemSelectPotionWndProc.m_pFrameWnd->GetID()) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}
	else if( ( m_WndID == WID_NpcChatWnd ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
		m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
	{
		if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
		}
	}



	///-- NPC 대화창 



	else
	{
		//Z-order Bottom에있는 윈도우 스크롤 처리 by - simwoosung
		if(g_ChatMainProc.m_ChatNonActPrintWnd.SetWheelValue(theMouse))
		{		
			int a = 0;
		}
		else if(g_SystemPrintWnd.SetWheelValue(theMouse))
		{
			int a = 0;
		}		
		///--------------------------------------------------------------------
		///		마우스로 카메라 조작 : by sooree
		///			휠 : 줌인/아웃 
		///			오른쪽 버튼 누른 상태로 좌-우/상-하 이동 : 좌우상하 회전 
		//--------------------------------------------------------------------
		else if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
		{
			///--JAPAN_BUG_MODIFY
			///-- 휠부분은 DirectInput을 사용한게 아니라...
			///-- 윈도우 마우스 메시지를 이용하기 때문에...  
			///-- 아마 mouse maker:arvel mouse name: MES13-SV <-- 기종의 
			///-- 풀 클라이언트 방식에는 제대로 동작되지 않는거 같다.
			///-- 해서 이곳에서는 따로 시간을 적용하지 않고 
			///-- 일단은 사용하지 않는게 낳을듯하다.
			
			if( theMouse.WheelValue < 0 )
			{
				g_Camera.m_fCameraRadius -= ( g_Camera.m_fCameraRadius / ( INIT_RADIUS * 0.1f ) );
			}
			else
			{
				g_Camera.m_fCameraRadius += ( g_Camera.m_fCameraRadius / ( INIT_RADIUS * 0.1f ) );
			}

			g_Camera.Update();
			g_Camera.m_bRotBySort = true;
			g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );	
		}
		
	}
	
	//by simwoosung 에디트창 편집중에는 카메라 방향키 이동블록
	if( nRui->IsEditFocus() == true )
		return 0;

	///--JAPAN_BUG_MODIFY
	float fElapsetime = g_GTimer.GetFrameElapsedSecTime();
	float fMoveRate = fElapsetime * 20.0f;

	if( theKey[DIK_INSERT].Action == n_baPress )// 카메라 트랜스폼 
	{
		g_Camera.m_fCameraRadius		+= ( fMoveRate * ( g_Camera.m_fCameraRadius / ( INIT_RADIUS * 0.5f ) ) );
		g_Camera.Update();
		g_Camera.m_bRotBySort = true;
		g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );					
	}
	else if( theKey[DIK_DELETE].Action == n_baPress )
	{
		g_Camera.m_fCameraRadius		-= ( fMoveRate * ( g_Camera.m_fCameraRadius / ( INIT_RADIUS * 0.5f ) ) );
		g_Camera.Update();
		g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );					
	}
	else if( theKey[DIK_LEFT].Action == n_baPress )
	{
		RLG1("CameraYaw MoveRate %f" , 0.05f * fMoveRate )	;
		g_Camera.m_fCameraYaw			+= ( 0.05f * fMoveRate );
	
		g_Camera.Update();
		g_Camera.m_bRotBySort = true;
		g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );					
	}
	else if( theKey[DIK_RIGHT].Action == n_baPress )
	{
		
		g_Camera.m_fCameraYaw			-= ( 0.05f * fMoveRate );
		g_Camera.Update();
		g_Camera.m_bRotBySort = true;
		g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );					
	}
	else if( theKey[DIK_UP].Action == n_baPress )
	{
		
		g_Camera.m_fCameraPitch			+= ( 0.05f * fMoveRate );
		g_Camera.Update();
		g_Camera.m_bRotBySort = true;
		g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );					
	}
	else if( theKey[DIK_DOWN].Action == n_baPress )
	{
		
		g_Camera.m_fCameraPitch			-= ( 0.05f * fMoveRate );
		g_Camera.Update();
		g_Camera.m_bRotBySort = true;
		g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );					
	}
	
	
	return( 0 );
}

float FloatRandom(float fMin, float fMax)
{
	if (fMin == fMax) 
		return(fMin);
	
	float fRandom = (float)rand() / (float)RAND_MAX;
	
	return((fRandom * (float)fabs(fMax-fMin))+fMin);
}

//-----------------------------------------------------------------------------
int CRockClient::KeyProcess()
{	
	if( !m_SceneManager.GetKeyInput() )
		return (0);
	
	// 키보드 액션은.... Edit에 Focus가 있으면...동작하지 않는다....
	//-------------------------------------------------------------------------
	if( nRui->IsEditFocus() == true )
	{
		ProfileBegin("< MouseProcess() >");
		// 마우스 액션은 EditFocus에 상관없이 동작
		MouseProcess();
		ProfileEnd("< MouseProcess() >");
		
		//by simwoosung
		//법사 마법 사전동작 딜레이 진행도 EditFocus에 상관없이 동작
		g_Pc.GetPlayer()->theActionSystem.ActionProcChantCombo();		
		
		return( 0 );
	}	
	
	ProfileBegin("< MouseProcess() >");
	MouseProcess();
	ProfileEnd("< MouseProcess() >");	
	
	//////////////////////////////////////////////////////////////////////////////////////
	// 콤보 공격을 위한 키 프로세스 Z, X, C
	//////////////////////////////////////////////////////////////////////////////////////
	ProfileBegin("< theActionSystem.ActionProc() >");
	g_Pc.GetPlayer()->theActionSystem.ActionProc();
	ProfileEnd("< theActionSystem.ActionProc() >");
	
	//////////////////////////////////////////////////////////////////////////////////////
	// 유져가 사용하는키 
	//////////////////////////////////////////////////////////////////////////////////////
	if(!UserKeyProcess())
	{	
		return 0;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	// 개발자가 사용하는 키 
	//////////////////////////////////////////////////////////////////////////////////////
	if( g_RockClient.m_EtcInfo.DevMode == 1)
	{
		TestKeyProcess();
	}
	
	
	return 0;
}
//-----------------------------------------------------------------------------
//뷰테스트

HRESULT		CRockClient::Display()
{	
	
	DWORD dExitCode = g_CLoadingBarThread.getExitCode();

	if( m_IsLoadProcessAfter && ( STILL_ACTIVE == g_CLogoDisplayThread.getExitCode() ) )
	{
		g_CLogoDisplayThread.Stop();		
		Sleep(10);		
#ifdef SHUTDOWN_TEST
		ClientFileLogW(L"Login Focus 1 ");
#endif 
		nRui->SendEvent( WID_Login_UserNameEdit, n_emSetFocus, 0, 0, 0, 0 );

#ifdef APPLY_INTRO_ANI
	    nRui->m_Logo2DEffect.m_uNowEffStep = n_LogoEffNone;
#endif
 
	}
		
	ProfileBegin( "Display0913" );
	//..........................................................................................................
	// 게임 디스플레이 루프
	//..........................................................................................................
	switch ( GetGameMode() )
	{
		//......................................................................................................
		// 게임 초기화
		//......................................................................................................
	case GMODE_INITLOAD:	/// 사실상 여기에 들어오지 않음 
		{
			if(32 == m_wPixelFormat)
			{
				g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, g_RockClient.m_SceneManager.GetBGColor() , 1.0f, 0);
			}
			else
			{
				g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, g_RockClient.m_SceneManager.GetBGColor() , 1.0f, 0);
			}
			
			if( SUCCEEDED( g_RockClient.BeginScene() ) )
			{
				LogoDisplay( 255 );
				
				g_RockClient.EndScene();
			}
		}
		break;
		
		//......................................................................................................
		// 게임 로그인 ( 로그인 / 서버 선택 )
		//......................................................................................................
	case GMODE_LOGINPAGE:
		{
			ProfileBegin("GMODE_LOGINPAGE0913");
			
#ifdef _DEBUG
			
#ifdef BOUND_BOX_DISPLAY
			
			if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_F10].Action == n_baDown )
			{
				g_Camera.m_ISBoundCamera = !g_Camera.m_ISBoundCamera; 
			}
			
			if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_F8].Action == n_baDown )
			{
				g_RockClient.m_TopCameraHeight += 10.0f;
			}
			
			if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_F9].Action == n_baDown )
			{
				g_RockClient.m_TopCameraHeight -= 10.0f;
			}
			
#endif
			
#endif

			if( STILL_ACTIVE != dExitCode && m_MapChangeProgress != true )
			{
				
				if(32 == m_wPixelFormat)
				{
					g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, g_RockClient.m_SceneManager.GetBGColor() , 1.0f, 0);
				}
				else
				{
					g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, g_RockClient.m_SceneManager.GetBGColor() , 1.0f, 0);
				}				
				
				if( SUCCEEDED( g_RockClient.BeginScene() ) )
				{
					LoginPageDisplay();
					
					g_RockClient.EndScene();
				}				
				
				///-- SvrListWnd 의 Wheel 처리
				CFrameWnd* fwnd	= (CFrameWnd*)nRui->Chain.GetNormalWndFocusHandle();
				if( fwnd )
				{
					
					m_WndID		= fwnd->GetID();
					m_WndHandle	= fwnd;
					
					POINT curtMousePos;
					GetCursorPos( &curtMousePos );
					ScreenToClient( m_hWnd, &curtMousePos );
					if( ( m_WndID == WID_ServerListWnd ) && ( m_WndHandle->theWnd->IsVisible == true ) && 
						m_WndHandle->FocusCheck( curtMousePos.x, curtMousePos.y, false ) ) 
					{
						if( theMouse.State == n_msWheel && theMouse.WheelValue != 0 )
						{
							m_WndHandle->theWnd->SetWheelValue( theMouse.WheelValue );
						}
					}
				}
				
				nRui->UpdatePcData();
				
				if( m_SceneManager.GetGUIDisplay() )
					nRui->RenderProc();	
				ProfileEnd("GMODE_LOGINPAGE0913");

				if( m_IsLoadProcessAfter && ( m_InitTime == 0 ) )
				{
					m_InitTime = timeGetTime();
				}
				
				if( m_IsLoadProcessAfter )
				{
					DWORD dGapTime = SAFE_TIME_SUB( timeGetTime(), m_InitTime );

					if( dGapTime > 1000 )
					{
						m_IsLoadProcessAfter = FALSE;
						g_LogoTex.Destory();

#ifdef APPLY_INTRO_ANI

						g_EffTex1.Destory();
						g_EffTex2.Destory();
						g_EffTex3.Destory();
						g_LoadingTex.Destory();

#endif

					}
					else
					{
						float fRatio = (float)dGapTime / 1000.0f;
						int nAlpha = 255 - ( 255 * fRatio );

						if( nAlpha < 0 )
						{
							nAlpha = 0;
						}

						if( SUCCEEDED( g_RockClient.BeginScene() ) )
						{
							LogoDisplay(nAlpha);						
							g_RockClient.EndScene();
						}
					}
				}
			}
		}
		break;
		
		//......................................................................................................
		// 캐릭터 선택 ( 캐릭터 생성 / 삭제 / 선택 )
		//......................................................................................................
	case GMODE_SELECT:
		{
			
#ifdef _DEBUG
			
#ifdef BOUND_BOX_DISPLAY
			
			if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_F10].Action == n_baDown )
			{
				g_Camera.m_ISBoundCamera = !g_Camera.m_ISBoundCamera; 
			}
			
#endif			
			
#endif
			if( STILL_ACTIVE != dExitCode && m_MapChangeProgress != true )
			{		

				if(32 == m_wPixelFormat)
				{
					g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, g_RockClient.m_SceneManager.GetBGColor(), 1.0f, 0);
				}
				else
				{
					g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, g_RockClient.m_SceneManager.GetBGColor(), 1.0f, 0);
				}
				
				if( SUCCEEDED( g_RockClient.BeginScene() ) )
				{
					switch( CharSelectWnd.GetState() )
					{	
						//..............................................................................................
						// 캐릭터 선택창에 코어 있는 상태
						//..............................................................................................
					case CSW_SELECT_NONE:
						//..............................................................................................
						// 캐릭터 선택창 캐릭터 있는 상태
						//..............................................................................................
					case CSW_SELECT_CORE:
					case CSW_SELECT_DELETE:
					case CSW_SELECT_EXIST:
	
						CharacSelectPageDisplay();	
						break;
						
						//..............................................................................................
						// 캐릭터 생성 창
						//..............................................................................................
					case CSW_CREATE:
						CharacCreatePageDisplay();
						
						break;
					}
					
					g_RockClient.EndScene();
				}
				
				nRui->UpdatePcData();
				
				if( m_SceneManager.GetGUIDisplay() )
					nRui->RenderProc();	
				
				LoadingBarWnd.MakeScreenAfterImage();
			}
			
		}
		break;
		
		//......................................................................................................
		// 게임 초기화 ( 사실상 여기에 들어오지 않음 )
		//......................................................................................................
	case GMODE_WORLDINIT:	
		{
			nRui->ModeInitialize( GMODE_NORMAL );
		}
		break;
		
		//......................................................................................................
		// 게임 플레이
		//......................................................................................................
	case GMODE_NORMAL:		
		{	
			g_Particle.m_Fade.Process( m_lpD3DDevice );
			
			if( STILL_ACTIVE != dExitCode && m_MapChangeProgress != true )
			{
				
				ProfileBegin(" <CRockClient::Display> --------------------- GMODE_NORMAL ");				
				
				ProfileBegin( "BeginScene_Clear" );
				
				if(32 == m_wPixelFormat)
				{
					g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, g_RockClient.m_SceneManager.GetBGColor(), 1.0f, 0);
				}
				else
				{
					g_RenderManager.Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, g_RockClient.m_SceneManager.GetBGColor(), 1.0f, 0);	
				}
				
				ProfileEnd( "BeginScene_Clear" );			
				
				if( g_RockClient.m_bIsDrawFullScreenUI == false )
				{
					if( SUCCEEDED( g_RockClient.BeginScene() ) )
					{
						ProfileBegin( "KeyProcess" );
						if( 0 != KeyProcess() ){	return -1;		}
						ProfileEnd( "KeyProcess" );
						
						ProfileBegin( "g_Map.Display();" );

					#ifdef HHM_1600X1200_TEST// Radeon 9550 해상도 1600 X 1200 창모드에서 속도 저하(2009/05/15)

						if(m_d3dsdBackBuffer.Width == 1600 && 
							m_d3dsdBackBuffer.Height == 1200 &&
							m_bWindowed)
						{
							if(g_1600x1200)
								g_Map.Display();
						}
						else

					#endif// #define HHM_1600X1200_TEST// Radeon 9550 해상도 1600 X 1200 창모드에서 속도 저하(2009/05/15)

						g_Map.Display();// 게임내 배경과 캐릭터/몬스터 등을 그린다
						ProfileEnd( "g_Map.Display();" );
						
						g_Particle.m_WeatherDrop.m_Fade.Process( g_lpDevice );						
						g_Particle.m_Fade.Render();
						
						ProfileBegin( "EndScene" );
						g_RockClient.EndScene();
						ProfileEnd( "EndScene" );
					}
					
					//				ProfileBegin( "SetCharacterInfo();" );
					//				SetCharacterInfo();
					//				ProfileEnd( "SetCharacterInfo();" );
					
					ProfileBegin( "MiniMapWnd.SetPcMapCoord" );
					MiniMapWnd.SetPcMapCoord( g_Pc.GetPlayer()->m_Mov.curt.x, g_Pc.GetPlayer()->m_Mov.curt.y );
					ProfileEnd( "MiniMapWnd.SetPcMapCoord" );
				}
				
				ProfileBegin( "nRui->SetCursorState();" );
				nRui->SetCursorState();
				ProfileEnd( "nRui->SetCursorState();" );
				
				ProfileBegin( "nRui->UpdatePcData();" );
				if( m_SceneManager.GetGUIDisplay() )
					nRui->UpdatePcData();
				ProfileEnd( "nRui->UpdatePcData();" );
				
				ProfileBegin( "nRui->RenderProc();" );
				if( m_SceneManager.GetGUIDisplay() )
					nRui->RenderProc();
				ProfileEnd( "nRui->RenderProc();" );
				
				LoadingBarWnd.MakeScreenAfterImage();
				
				ProfileEnd( " <CRockClient::Display> --------------------- GMODE_NORMAL " );
			}
			
			
			//////////////////////////////////////////////////////////////////////////////////////
			// 게임 개발용 디스플레이 
			//////////////////////////////////////////////////////////////////////////////////////
			
			/*
			sprintf( buf, "Map=%d, Cell=%d", g_Map.m_NowMapID, g_Pc.m_MainPC.lBlock );
			nRui->TextOut( buf, nRui->Desktop.Width - 148, 130, D3DCOLOR_ARGB( 255, 222, 222, 222 ) );
			
			  sprintf( buf, "X=%d, Z=%d", g_Pc.GetPlayer()->m_Mov.curt.x, g_Pc.GetPlayer()->m_Mov.curt.y );
			  nRui->TextOut( buf, nRui->Desktop.Width - 128, 145, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			//*/
		}
		
		break;
	}
	
	static int center_x	= nRui->Desktop.Width / 2;
	
	if( STILL_ACTIVE != dExitCode && 
		thePacketTransLogDlg.IsActivePDlg() && m_MapChangeProgress != true )
	{
		char buf[128] = {0,};
		Rsprintf( RWCHAR(buf), _RT("FPS=%.2f"), m_fFPS );		
		nRui->TextOut( buf, center_x + 250, 20, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );

#ifdef HHM_CAMERA_FOV_PRINT// fov값 화면에 출력(2009/05/14)
		// hhm
		buf[0] = 0;
		Rsprintf( RWCHAR(buf), _RT("BackBuffer.Width=%d, BackBuffer.Height=%d"), m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height );		
		nRui->TextOut( buf, center_x - 100, 40, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );
		
		buf[0] = 0;
		Rsprintf( RWCHAR(buf), _RT("시야각(FOV) = %f, 종횡비(Aspect) = %f"), g_Camera.m_fFOV, g_Camera.m_fAspect);		
		nRui->TextOut( buf, center_x - 100, 60, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );
		
		buf[0] = 0;
		Rsprintf( RWCHAR(buf), _RT("nRui.Desktop.Width = %d, nRui.Desktop.Height = %d"), nRui->Desktop.Width, nRui->Desktop.Height);		
		nRui->TextOut( buf, center_x - 100, 80, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

#endif// #ifdef HHM_CAMERA_FOV_PRINT// fov값 화면에 출력(2009/05/14)
	}
	


#ifdef ROCKCLIENT_DEV			
	
	if( STILL_ACTIVE != dExitCode && m_MapChangeProgress != true )
	{
		char buf[128] = {0,};
		
		//맵 48 49 맵에서는 ... 시간을 띄운다 .. 
		if(bViewRealHP)
		{
			
			Rsprintf( RWCHAR(buf), _RT("player RealHP [%d] HP[%d]") ,g_Pc.m_MainPC.char_info.theRealHP,nRui->thePcParam.Stat1.theHP );
			nRui->TextOut( buf, center_x + 200, 40, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );
			
			if(NonPlayerInfoWnd.thePickedNpcID != -1)
			{
				Rsprintf( RWCHAR(buf), _RT("UniqueID[%d] RealHP[%d] HP[%d]") ,
					g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID ].lUnique, 
					g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID ].theRealHP ,g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID ].theHP);
				nRui->TextOut( buf, center_x + 200, 60, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );

				Rsprintf( RWCHAR(buf), _RT("NPC posX[%d] posY[%d]") ,
					g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID ].lDestBlock%(g_Map.m_cSize*2),
					g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID ].lDestBlock/(g_Map.m_cSize*2));
				nRui->TextOut( buf, center_x + 200, 120 , D3DCOLOR_ARGB( 255, 255, 200, 200 ) );

			}
			
			if(NonPlayerInfoWnd.thePickedUserID != -1 && NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE)
			{
				Rsprintf( RWCHAR(buf), _RT("UserID[%d] RealHP[%d] HP[%d] MP[%d]") ,
					NonPlayerInfoWnd.thePickedUserID,g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.theRealHP , 
					g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.theHP,
					g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.theSMP);
				nRui->TextOut( buf, center_x + 200, 60, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );
				


			}
			
			Rsprintf( RWCHAR(buf), _RT("ani_Frame  %d ") , g_Pc.GetPlayer()->m_ani_frame );
			nRui->TextOut( buf, center_x + 200, 80, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );
			
			Rsprintf( RWCHAR(buf), _RT("ani_Num  %d ") , nRui->m_BugMop.Number  );
			nRui->TextOut( buf, center_x + 200, 100, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );
			
			



		}
		
		/*	POINT		ptCursor;
		GetCursorPos( &ptCursor );
		::ScreenToClient( g_RockClient.GetApp(), &ptCursor );
		
		  if( ( ptCursor.x != nRui->GetCursor()->pos.x ) || ( ptCursor.y != nRui->GetCursor()->pos.y ) )
		  {
		  if( ptCursor.y > nRui->GetCursor()->pos.y )
		  {
		  sprintf( buf, "MousePos PosX=%d, PosY=%d", ptCursor.x, ptCursor.y );
		  nRui->TextOut( buf, center_x + 250, 40, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );
		  RLG2( "MousePos PosX=%d, PosY=%d", ptCursor.x, ptCursor.y );
		  }
		  else
		  {
		  sprintf( buf, "Cursor PosX=%d, PosY=%d", nRui->GetCursor()->pos.x, nRui->GetCursor()->pos.y );
		  nRui->TextOut( buf, center_x + 250, 60, D3DCOLOR_ARGB( 255, 255, 200, 200 ) );
		  RLG2( "Cursor PosX=%d, PosY=%d", nRui->GetCursor()->pos.x, nRui->GetCursor()->pos.y );
		  }
		  }
		*/
	}
#endif
	//..........................................................................................................
	// 화면 캡처 ( sooree hahaha )
	//..........................................................................................................
	if( m_IsClickScreen == 1 )	//버튼을 눌렀냐 
	{
		m_IsClickScreen = 2;	//사진을 찍고 있다 
		ScreenCapture();
		m_IsClickScreen = 0;	// 버튼을 입력 받을수 있다. 
		
	}
	
	if( ( STILL_ACTIVE != dExitCode && m_MapChangeProgress != true ) )
	{
		// Show the frame on the primary surface.
		if( g_WebShopWnd.IsVisible )
		{
			g_WebShopWnd.Present();
		}
		else
		{
			g_RockClient.Present( NULL, NULL, NULL, NULL );		
		}
	}
	else
	{
	}
	
	ProfileEnd( "Display0913" );
	return	S_OK;
}


//유져가 사용하는 키
bool CRockClient::UserKeyProcess()
{
	
	//중국의 컨트로 + 쉬프트는 IME 전환중복 되서 DIK_W변경 하였습니다. 
#ifdef _DEBUG

	Character* pCharacter = NULL;

	if( theKey[DIK_LMENU].Action == n_baPress && theKey[DIK_Z].Action == n_baUp ) 
	{

	

/*
		SRpCsMixEldaStone dd;
	//	dd.theMsgHeader.theProtocol = rand()/0xFF99;
		int RandSize  = rand()%10000;

		dd.theMsgHeader.theSize += RandSize;
		BYTE *bbdata = new BYTE[RandSize];
		memcpy( &dd.theEldaStone , &a , 10 );
		g_RockClient.Send_CsSRpCsMixEldaStone(dd);
		delete bbdata;
*/

//		g_RockClient.Send_CsMakePetName(a,false);	
	
	}
		
	// g_ItemEldaMixRWndProc.OpenEnchantR_FWnd();
/*
 

	if( theKey[DIK_LMENU].Action == n_baPress && theKey[DIK_Z].Action == n_baUp ) 
	{
		g_ItemEldaMixRWndProc.OpenEnchantR_FWnd(CItemEldaMixRWndProc::eStrengthStone );
// 		g_Particle.SetEffSetCon( 0.0f , 5005 , ESLT_SRC_RAD , EFFECT_SET_LOOP_TIME , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex(),
// 			D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , g_Pc.GetPlayer()->GetDirection() );
//		for(int i = 0; i < 40; i++)
		{
//#ifdef BATCH_RENDER
#if 1
			Model **pModel;
			pModel = g_Billboard.m_pExtraModelList.GetKeyDataPointer( eBDType_Hemisphere );
			pModel = g_Billboard.m_pExtraModelList.GetKeyDataPointer( eBDType_Sphere );
			pModel = g_Billboard.m_pExtraModelList.GetKeyDataPointer( eBDType_Donut );
			pModel = g_Billboard.m_pExtraModelList.GetKeyDataPointer( eBDType_DonutUVOne );
			pModel = g_Billboard.m_pExtraModelList.GetKeyDataPointer( eBDType_Cylinder6 );
			pModel = g_Billboard.m_pExtraModelList.GetKeyDataPointer( eBDType_Cylinder8 );
//			g_Particle.SetEffSetCon( 0.0f , EF_DOGMA_L3_FOOT_QUAKE , ESLT_POS , 5.1f , -1 , -1 , 
//			 D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , g_Pc.GetPlayer()->GetDirection() );

// 			g_Particle.SetEffSetCon( 0.0f , 40099, ESLT_SRC_RAD , EFFECT_SET_LOOP_TIME , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex(),
// 			D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , g_Pc.GetPlayer()->GetDirection() );
// 
// 			g_Particle.SetEffSetCon( 0.0f , 5004, ESLT_SRC_RAD , EFFECT_SET_LOOP_TIME , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex(),
// 			D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , g_Pc.GetPlayer()->GetDirection() );
#endif	//	BATCH_RENDER
		}
		
// 		g_Particle.SetEffSetCon( 0.0f , EF_DOGMA_L3_FOOT_QUAKE , ESLT_POS , 5.1f , -1 , -1 , 
// 			 D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , g_Pc.GetPlayer()->GetDirection() );
//		g_Particle.SetEffSetCon( 0.0f , EF_FIRE_FLAME_KICK , ESLT_SRC_RAD , 2.1f , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , g_Pc.GetPlayer()->GetDirection() );
//		g_Particle.SetEffSetCon( 0.0f , EF_GIANT_GOLEM_ATTACK , ESLT_POS , 1.5f , -1 , -1 , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ), 0.0f );
		//g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) EF_RUGAL_RECALL_SOLTARED , ( Character * ) &g_Map.m_Npc[PickNPC] , NULL , NULL );
		
	}
	*/
/*
	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_1].Action == n_baDown )
	{
		int NPCType = 4261 ;
		int theNPCID = 2;
		
		int theCellIndex = ( g_Pc.GetPlayer()->m_Mov.curt.y * g_Pc.m_cSize ) + g_Pc.GetPlayer()->m_Mov.curt.x ; 
		int npc_tbl_index = -1;
		npc_tbl_index = g_Map.SM__NPC__APPEAR( NPCType, theNPCID, theCellIndex , 100 , 1, 0 , 180 );
	}

	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_2].Action == n_baDown )
	{
		int npc_tbl_index = g_Map.FindNpc(2);
		pCharacter =  g_Map.GetMonster(g_Map.m_NpcTable[npc_tbl_index].lUnique);
		pCharacter->AnimationPlay(n_NPC_Attack2);
		
		g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_HOWLLING_1_ED , 0 , 2.6f , 
			g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_Pc.GetPlayer() ) );
			

	}

	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_3].Action == n_baDown )
	{
		int npc_tbl_index = g_Map.FindNpc(2);
		pCharacter =  g_Map.GetMonster(g_Map.m_NpcTable[npc_tbl_index].lUnique);
		pCharacter->AnimationPlay(n_NPC_Attack2);
		
		
		g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_HOWLLING_1_ED , 0 , 2.6f , 
			g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_Pc.GetPlayer() ) );
		
		
	}
	

	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_3].Action == n_baUp )
	{
		if(NonPlayerInfoWnd.thePickedNpcID != -1)
		{
			g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].AnimationPlay(n_NPC_Attack0);
			
			D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f , 0.0f);
			vPos.x = g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetPosTM()._41;
			vPos.z = g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetPosTM()._43;
			vPos.y = g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetPosTM()._42;
			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_BASIC_ATTACK , ESLT_POS , 4.1f , 
				g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetUnique() , g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetIndex(), vPos , g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetDirection() );
			
		}
	}
	
	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_4].Action == n_baUp )
	{
		if(NonPlayerInfoWnd.thePickedNpcID != -1)
		{
			g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].AnimationPlay(n_NPC_Attack0);
			
			D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f , 0.0f);
			vPos.x = g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetPosTM()._41;
			vPos.z = g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetPosTM()._43;
			vPos.y = g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetPosTM()._42;

			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_BASIC_ATTACK , ESLT_CHARPOS , 4.1f , 
				g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetUnique() , g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
											 g_Pc.GetPlayer()) ,g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetDirection() );
			

		}
	}
	
	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_5].Action == n_baUp )
	{
		if(NonPlayerInfoWnd.thePickedNpcID != -1)
		{
			g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].AnimationPlay(n_NPC_Attack0);
			
			D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f , 0.0f);
			vPos.x = g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetPosTM()._41;
			vPos.z = g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetPosTM()._43;
			vPos.y = g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetPosTM()._42;
			g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_BASIC_ATTACK , ESLT_SRC_RAD , 4.1f , 
				g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetUnique() , g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetIndex(), vPos , g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].GetDirection() );
			
		}
	}
	
*/


/*
	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_3].Action == n_baDown )
	{
		int NPCType = 4261 ;
		int theNPCID = 3;
		int theCellIndex = ( g_Pc.GetPlayer()->m_Mov.curt.y * g_Pc.m_cSize ) + g_Pc.GetPlayer()->m_Mov.curt.x ; 
		int npc_tbl_index = -1;
		npc_tbl_index = g_Map.SM__NPC__APPEAR( NPCType, theNPCID, theCellIndex , 100 , 1, 0 , 180 );
	}

	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_4].Action == n_baDown )
	{
		int npc_tbl_index = g_Map.FindNpc(3);
		pCharacter =  g_Map.GetMonster(g_Map.m_NpcTable[npc_tbl_index].lUnique);
		pCharacter->AnimationPlay(n_NPC_Attack2);
		g_Particle.m_EffectManager.Create(EF_RUGAL_CURSE_1 ,pCharacter ,NULL   );

		g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_CURSE_1_ED , 0 , 2.6f , 
			g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_Pc.GetPlayer() ) );

	
	}


	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_5].Action == n_baDown )
	{
		int NPCType = 4263 ;
		int theNPCID = 4;
		int theCellIndex = ( g_Pc.GetPlayer()->m_Mov.curt.y * g_Pc.m_cSize ) + g_Pc.GetPlayer()->m_Mov.curt.x ; 
		int npc_tbl_index = -1;
		npc_tbl_index = g_Map.SM__NPC__APPEAR( NPCType, theNPCID, theCellIndex , 100 , 1, 0 , 180 );
	}
	
	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_6].Action == n_baDown )
	{
		int npc_tbl_index = g_Map.FindNpc(4);
		pCharacter =  g_Map.GetMonster(g_Map.m_NpcTable[npc_tbl_index].lUnique);
		pCharacter->AnimationPlay(n_NPC_Attack2);
		g_Particle.m_EffectManager.Create(EF_RUGAL_ILLUSIONTREE_1 ,pCharacter ,NULL   );
		//EF_NPC_ENTANGLEED
		g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_ILLUSIONTREE_1_ED  , 0 , 5.6f ,
		g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_Pc.GetPlayer() ) );


		
	}

	
	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_7].Action == n_baDown )
	{
		int NPCType = 4264 ;
		int theNPCID = 5;
		int theCellIndex = ( g_Pc.GetPlayer()->m_Mov.curt.y * g_Pc.m_cSize ) + g_Pc.GetPlayer()->m_Mov.curt.x ; 
		int npc_tbl_index = -1;
		npc_tbl_index = g_Map.SM__NPC__APPEAR( NPCType, theNPCID, theCellIndex , 100 , 1, 0 , 180 );
	}
	
	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_8].Action == n_baDown )
	{
		int npc_tbl_index = g_Map.FindNpc(5);
		pCharacter =  g_Map.GetMonster(g_Map.m_NpcTable[npc_tbl_index].lUnique);
		pCharacter->AnimationPlay(n_NPC_Attack2);
		g_Particle.m_EffectManager.Create(EF_RUGAL_ONECUTOFRUIN ,pCharacter ,NULL   );

		g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_ONECUTOFRUINED , 0 , 2.6f , 
			g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 	g_Pc.GetPlayer() ) );
	}

	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_9].Action == n_baDown )
	{
		int NPCType = 4265 ;
		int theNPCID = 6;
		int theCellIndex = ( g_Pc.GetPlayer()->m_Mov.curt.y * g_Pc.m_cSize ) + g_Pc.GetPlayer()->m_Mov.curt.x ; 
		int npc_tbl_index = -1;
		npc_tbl_index = g_Map.SM__NPC__APPEAR( NPCType, theNPCID, theCellIndex , 100 , 1, 0 , 180 );
	}
	
	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_0].Action == n_baDown )
	{
		int npc_tbl_index = g_Map.FindNpc(6);
		pCharacter =  g_Map.GetMonster(g_Map.m_NpcTable[npc_tbl_index].lUnique);
		pCharacter->AnimationPlay(n_NPC_Attack2);
		g_Particle.m_EffectManager.Create(EF_RUGAL_NAIL_OFDECAY ,pCharacter ,NULL   );

		g_Particle.SetEffSetCon( 0.0f , EF_RUGAL_NAIL_OFDECAYED , 0 , 2.6f , 
			g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 	g_Pc.GetPlayer() ) );

	}
*/

	

#endif



	if( g_RockClient.GetLanguage() == eLang_China ||
		 g_RockClient.GetLanguage() == eLang_Taiwan || 
		 g_RockClient.GetLanguage() == eLang_HongKong )
	{
		if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_W].Action == n_baUp )
		{
			nRui->m_bIsShowUI = !(nRui->m_bIsShowUI);
		}	
	}
	else
	{
		if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_LSHIFT].Action == n_baUp )
		{
			nRui->m_bIsShowUI = !(nRui->m_bIsShowUI);
		}
	}


	
	if( ( theKey[DIK_LCONTROL].Action == n_baPress ) && theKey[DIK_R].Action == n_baUp )
	{
		
#ifdef RIDING_SYSTEM
		
#ifdef SINGAPORE_BLOCK_RIDING
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_COMING_SOON_RIDING ) );
		return 0;
#endif 
		
		//현재 내 라이딩 슬롯에 아이템이 등록되어 있을 경우에만
		if( nRui->thePcInven.WearItem[n_WearSlot_Riding] == NULL )
		{
			SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR( IDS_SYSPR_EMPTY_RIDING_SLOT ) );				 
			return 0;
		}
	
		if( FALSE == CanRideOnDest() )
		{
			return 0;
		}
		
		bool bFlag = g_Pc.GetPlayer()->m_State.bRide;
		bFlag = !bFlag;
		
		if( bFlag )
		{
			nRui->SendEvent( WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
			BOOL bFlag = FALSE;
			Player*	user_pc = g_Pc.GetPlayer();
			
			///--JAPAN_BUG_MODIFY			
			if(user_pc->IsRealDie())
			{
				bFlag = TRUE;    
			}				
			
			g_DelayBarWnd.ResetDelay(DELAY_RIDE_ON, 0, bFlag);
		}
		else
		{
			nRui->SendEvent(WID_DelayBar_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0);
			BOOL bFlag = FALSE;

	
			g_DelayBarWnd.ResetDelay(DELAY_RIDE_OFF, 0, bFlag);
		}

#endif	
	}
	      
#ifdef _DEBUG	
	
	if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_LALT].Action == n_baPress && theKey[DIK_B].Action == n_baDown )
	{
		static bool is_buster = false;
		
		is_buster = !is_buster;
		
		int move_speed = 180;
		    
		if( is_buster )
		{
			move_speed = 18;
			nRui->thePcParam.Stat2.theMoveSpeed = move_speed;					
			g_Pc.GetPlayer()->m_ani_percent = ( (float)move_speed / c_Pc_MoveSpeed );					
			g_Pc.GetPlayer()->m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.GetPlayer()->m_ani_percent );					
		}
		else
		{
			move_speed = 180;
			nRui->thePcParam.Stat2.theMoveSpeed = move_speed;					
			g_Pc.GetPlayer()->m_ani_percent = ( (float)move_speed / c_Pc_MoveSpeed );					
			g_Pc.GetPlayer()->m_move_percent = c_Pc_MoveSpeed / ( c_Pc_MoveSpeed * g_Pc.GetPlayer()->m_ani_percent );					
		}
		
	}
	
#endif	
	
	return TRUE;	
}


bool CRockClient::TestKeyProcess()
{
	   
	//릴리즈 모드 일시 바로리턴
#ifdef _DEBUG
/*
	if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_1].Action == n_baDown )
	{
		//정수 제작 선택창 
		g_ItemSelectPotionWndProc.Open(n_Make_Essence);
	}
	
	if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_2].Action == n_baDown )
	{
		g_ItemSelectPotionWndProc.Open(n_Make_R_Strengthstone);
	}
	
	if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_3].Action == n_baDown )
	{
		g_ItemSelectPotionWndProc.Open(n_Make_M_Strengthstone);
	}
	
	if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_4].Action == n_baDown )
	{
		g_StrengthStoneWndProc->OpenEnchantWnd();
	}

	if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_5].Action == n_baDown )
	{
		g_ItemEnchantWndProc.OpenEnchantWnd();
	}

	/*
	if ( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_1].Action == n_baDown )
	{	
		SNPCBaseInfo* npc_Info = NULL;
		NPCTABLE *pTable = g_Map.GetNpcTable(g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetUnique());
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code );

		if( NULL != npc_Info )
		{
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
					
			SRpScSeeSkill skill;
			skill.theAttackID  = pTable->lUnique;
			skill.theAttackType = 1;
			skill.theTarget = g_Pc.m_MainPC.lUnique;
			skill.theTargetType	= 0;
			skill.theSkillCode	= npc_Info->theSkillCode[1];
			skill.theHP	= 11111;
			skill.theSP	= 22;
			skill.theTargetNum	= 1;
			skill.theTargetList[0].theTargetID = g_Pc.m_MainPC.lUnique;
			skill.theTargetList[0].theTargetType = 0;
			skill.theTargetList[0].theDieFlag	=0;
			skill.theTargetList[0].thePushedCell =0;
			skill.theTargetList[0].theKeepupFlag[0]	= 1 ;
			skill.theTargetList[0].theKeepupFlag[1]	= 1 ;
			skill.theTargetList[0].theKeepupFlag[2]	= 1 ;
			skill.theTargetList[0].theEffectValue[0] = 304;
			skill.theTargetList[0].theEffectValue[1] = 187;
			skill.theTargetList[0].theEffectValue[2] = 4;
			skill.theTargetList[0].theAddEfectFlag	= 1;
						
			Proc_RpScSeeSkill(  &skill );		
		}
	}

		
	
	if( GetGameMode() == GMODE_NORMAL  && 
		theKey[DIK_LSHIFT].Action && theKey[DIK_U].Action == n_baDown )
	{
		int nLevel = rand()%20;		
		int nStep = rand()%30;		
		
		nRui->MiniGameStartUp( 1, (nLevel * 30) + nStep , 120);
	}




/*
	
	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_2].Action == n_baDown )
	{
//		g_ItemEldaMixRWndProc.OpenEnchantR_SWnd(1);
	}
	
	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_3].Action == n_baDown )
	{
		g_ItemEldaMixRWndProc.OpenEnchantR_FWnd();
	}


	if ( theKey[DIK_LCONTROL].Action == n_baPress &&  theKey[DIK_1].Action == n_baDown )
	{
		g_ItemEldaMixAWndProc.Open(0);
	}


	static float Size = 16.0f;

	static OneTime = TRUE;
	static int TempAddEffectCount = 0;
	
	static int TempAddEffectList[] = {
		462 , 645 , 463 , 468 , 469 , 465 , 464 , 646 , 467 , 466 , 647 , 648 , -1
	};	
	
	/*if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_1].Action == n_baUp )// NPC 생성
	{	
		CheckUseVideoMemory();
	}

	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_Q].Action == n_baUp )// NPC 생성
	{
		SRpScPlaytime aMsg;
		aMsg.thePlayAbleFlag = 0 ; 
		aMsg.thePlaytime = 3540;
		Proc_SRpScPlaytime(&aMsg);
	}
	
		
	if( theKey[DIK_LSHIFT].Action == n_baPress && theKey[DIK_1].Action == n_baUp )
	{
		if( g_pAcryEnchantWnd )
		{
			g_pAcryEnchantWnd->Open();
		}
	}

	
	Character * pCharacter = g_Pc.GetPlayer(); 

	if( theKey[DIK_LMENU].Action == n_baPress && theKey[DIK_1].Action == n_baUp ) 
	{
		if(NonPlayerInfoWnd.thePickedNpcID != -1)
		{
			SNPCBaseInfo* npc_Info = NULL;
			
			NPCTABLE *pTable = g_Map.GetNpcTable(g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetUnique());
			
			if( NULL != pTable )
			{
				int PickID = NonPlayerInfoWnd.thePickedNpcID;
				
				D3DXVECTOR3 vec; 
				vec.x = g_Map.m_Npc[PickID].GetPosTM()._41;
				vec.y = g_Map.m_Npc[PickID].GetPosTM()._42 + g_Map.m_Npc[PickID].GetBoundMaxPos().y * 0.5f ;
				vec.z = g_Map.m_Npc[PickID].GetPosTM()._43;

				g_Particle.m_fAddSize = 1; 
				g_Particle.SetEffSetCon( 0.0f , 1175 , ESLT_POS , 5.0f , -1 , -1  , vec , 0.0f );
			
				g_Map.SM__NPC__CHANGE(g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetUnique() , pTable->code+1 );
				
			//	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
			}
		}		
	}
	
	int PickNPC = NonPlayerInfoWnd.thePickedNpcID;

	if( theKey[DIK_LMENU].Action == n_baPress && theKey[DIK_2].Action == n_baUp ) 
	{
		if(NonPlayerInfoWnd.thePickedNpcID != -1)
		{
			SNPCBaseInfo* npc_Info = NULL;
			NPCTABLE *pTable = g_Map.GetNpcTable(g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetUnique());
			
			int Unique = g_Map.m_Npc[PickNPC].GetUnique();
			int Index = g_Map.m_Npc[PickNPC].GetIndex();

			if( NULL != pTable )
			{
				int PickID = NonPlayerInfoWnd.thePickedNpcID;
				
				D3DXVECTOR3 vec; 
				//EF_HOLY_BOMB_ED , EF_EGGBIL_HILLING
				g_Particle.m_fAddSize = 1; 
				

				g_Particle.m_EffectManager.CreateEffect( EF_WRATH_BOMB , ( Character * ) &g_Map.m_Npc[PickNPC] , NULL , NULL );

				
				//	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
			}
		}		
	}

	if( theKey[DIK_LMENU].Action == n_baPress && theKey[DIK_Z].Action == n_baUp ) 
	{
//		g_Particle.SetEffSetCon( 0.0f , EF_WRATH_BOMB , ESLT_SRC_RAD , 2.1f , g_Pc.GetPlayer()->GetUnique() , g_Pc.GetPlayer()->GetIndex() , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , g_Pc.GetPlayer()->GetDirection() );
		
		//g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) EF_RUGAL_RECALL_SOLTARED , ( Character * ) &g_Map.m_Npc[PickNPC] , NULL , NULL );
		
	}


	
	

	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_1].Action == n_baUp ) 
	{
		if(NonPlayerInfoWnd.thePickedNpcID != -1)
		{
			EVENT_DATA_INFO attack_event;
			attack_event.type = SM_ATTACK_NPC;
			attack_event.motion = n_NPC_Attack0;
			attack_event.tbl_Idx = MAX_USER_INTABLE;
			attack_event.race	= g_Pc.m_MainPC.char_info.race;
			attack_event.index	= g_Pc.m_MainPC.nIndex;
			attack_event.unique = g_Pc.m_MainPC.lUnique;
			attack_event.x		= g_Pc.m_MainPC.lDestBlock%(g_Map.m_cSize*2);
			attack_event.y		= g_Pc.m_MainPC.lDestBlock/(g_Map.m_cSize*2);
			attack_event.vPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
			attack_event.vPos.y = g_Pc.GetPlayer()->GetPosTM()._42;
			attack_event.vPos.z = g_Pc.GetPlayer()->GetPosTM()._43;		
			attack_event.damage = 0; 
			attack_event.critical = 0;		
			attack_event.count = 0;	
			attack_event.kill = 0;	
			
			g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].PushQueue(attack_event);		
		}		
	}

	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_2].Action == n_baUp ) 
	{
		if(NonPlayerInfoWnd.thePickedNpcID != -1)
		{
			EVENT_DATA_INFO attack_event;
			attack_event.type = SM_ATTACK_NPC;
			attack_event.motion = n_NPC_Attack1;
			attack_event.tbl_Idx = MAX_USER_INTABLE;
			attack_event.race	= g_Pc.m_MainPC.char_info.race;
			attack_event.index	= g_Pc.m_MainPC.nIndex;
			attack_event.unique = g_Pc.m_MainPC.lUnique;
			attack_event.x		= g_Pc.m_MainPC.lDestBlock%(g_Map.m_cSize*2);
			attack_event.y		= g_Pc.m_MainPC.lDestBlock/(g_Map.m_cSize*2);
			attack_event.vPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
			attack_event.vPos.y = g_Pc.GetPlayer()->GetPosTM()._42;
			attack_event.vPos.z = g_Pc.GetPlayer()->GetPosTM()._43;		
			attack_event.damage = 0; 
			attack_event.critical = 1;		
			attack_event.count = 0;	
			attack_event.kill = 0;	
			
			g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].PushQueue(attack_event);		
		}		
	}

	
	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_3].Action == n_baUp )
	{		
		if(NonPlayerInfoWnd.thePickedNpcID != -1)
		{
			int nEffectId = 0;

			int nNpcCode = g_Map.m_NpcTable[ NonPlayerInfoWnd.thePickedNpcID ].code;

			if( nNpcCode == 4272 )
			{
				nEffectId = EF_JANAD_DASHTRIPLE_ATTACK;
			}
			else if( nNpcCode == 4273 )
			{
				nEffectId = EF_JANAD_DARKSWORDSTORM;
			}
			else if( nNpcCode == 4274 )
			{
				nEffectId = EF_JANAD_MAGMA_BUST;
			}
			else if( nNpcCode == 4270 )
			{
				nEffectId = EF_BARAGOS_DRAGON_BRESS;
			}
			else if( nNpcCode == 4271 )
			{
				nEffectId = EF_BARAGOS_THUNDERBREAK;
			}
			else if( nNpcCode == 4268 )
			{
				nEffectId = EF_UROBOROS_POISON;
			}
			else if( nNpcCode == 4269)
			{
				nEffectId = EF_UROBOROS_DRAG_HOWLLING;
			}
			


			if( nEffectId )
			{
				g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].AnimationPlay(n_NPC_Attack2);
				g_Particle.m_EffectManager.Create( (EFFECT_ID)nEffectId , &g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ] ,
											   g_Pc.GetPlayer() );
			}
		}
	}

	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_4].Action == n_baUp )
	{		
		if(NonPlayerInfoWnd.thePickedNpcID != -1)
		{
			int nEffectId = 0;
			static BOOL IsBerserkFlag = FALSE;

			int nNpcCode = g_Map.m_NpcTable[ NonPlayerInfoWnd.thePickedNpcID ].code;

			if( nNpcCode == 4272 )
			{
				nEffectId = EF_JANAD_CALLDEVILSOUL;
			}
			else if( nNpcCode == 4273 )
			{
				nEffectId = EF_JANAD_DISPEL;				
			}
			else if( nNpcCode == 4274 )
			{
				nEffectId = EF_JANAD_SWORD_FLASH_OF_DEVIL;				
			}
			else if( nNpcCode == 4271 )
			{
				nEffectId = EF_BARAGOS_BERSERK;
				IsBerserkFlag ^= true; 
			}
			else if(nNpcCode == 4270)
			{
				nEffectId = EF_BARAGOS_HOWL_OF_DEVIL;	
			}

			else if( nNpcCode == 4270 ) 
			{
				nEffectId = EF_UROBOROS_BALKAN;
			}
			else if( nNpcCode == 4268 )
			{
				nEffectId = EF_UROBOROS_BALKAN;
			}
			else if( nNpcCode == 4271)
			{
				nEffectId = EF_UROBOROS_RUN;
			}

			if( nEffectId == EF_BARAGOS_BERSERK )
			{
				if( IsBerserkFlag )
				{
					g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].AnimationPlay(n_NPC_Attack3);
					g_Particle.m_EffectManager.Create( (EFFECT_ID)nEffectId , &g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ] , NULL );	
				}
				else
				{
					g_Particle.m_EffectManager.Delete( ( EFFECT_ID )nEffectId , &g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ] , NULL );
				}
			}
			else if( nEffectId )
			{
				g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ].AnimationPlay(n_NPC_Attack3);
				g_Particle.m_EffectManager.Create( (EFFECT_ID)nEffectId , &g_Map.m_Npc[ NonPlayerInfoWnd.thePickedNpcID ] ,NULL  );	
			}
		}
	}



	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_5].Action == n_baUp )
	{
		BOOL IsFlag = g_Particle.m_IsRenderScreenBlur;
		IsFlag = !IsFlag;
		g_Particle.m_IsRenderScreenBlur = IsFlag;
	}

	if( theKey[DIK_LSHIFT].Action == n_baPress && theKey[DIK_1].Action == n_baUp )
	{
		
		g_Pc.GetPlayer()->AnimationPlayNoneBlend( n_Dance_Night + nRui->m_BugMop.Number );
		nRui->m_BugMop.Number++;
		if( nRui->m_BugMop.Number > 7 )
		{
			nRui->m_BugMop.Number = 0 ; 
		}
	}

	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_F7].Action == n_baUp )// NPC 생성
	{
		if(g_Map.m_uMapType == MAP_TYPE_COLONY && g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			g_Map.ReSetMapIndex(0);
		}
	}
	 
	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_F8].Action == n_baUp )// NPC 생성
	{
		if(g_Map.m_uMapType == MAP_TYPE_COLONY && g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			g_Map.ReSetMapIndex(1);
		}
	}
	 
	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_F9].Action == n_baUp )// NPC 생성
	{
		if(g_Map.m_uMapType == MAP_TYPE_COLONY && g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			g_Map.ReSetMapIndex(2);
		}
	}
	
*/

/*
	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_1].Action == n_baDown )
	{	
		ScreenCapture();
	}	

	*/
	//............................................................................................................
	// 임시 이펙트 키 설정 by wxywxy
	//............................................................................................................
		
	if( theKey[DIK_LCONTROL].Action == n_baPress && theKey[DIK_HOME].Action == n_baDown )
	{
		bViewRealHP ^= true;
	}	
	
	if( theKey[DIK_F8].Action == n_baUp )///by soohyun
	{
		///맵 렌더링 - > 와이어프레임 on/off
		g_Map.theWireframe = !g_Map.theWireframe;
	}
#endif 
	
	return true;
}













