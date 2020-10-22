//-----------------------------------------------------------------------------
// File: D3DApp.cpp
//
// Desc: Application class for the Direct3D samples framework library.
//
// Copyright (c) 1998-2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
//#define STRICT
#include "RockPCH.h"

#include <windows.h>
#include <windowsx.h>
#include <basetsd.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <D3D9.h>

#include <ddraw.h>
#include <dinput.h>

#include "D3DApp.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "D3DRes.h"
#include "Dsetup.h"
#include "profile\profile.h"
#include "..\\..\\CRenderManager.h"
#include "CNetProcThread.h"


#pragma comment( lib, "Dsetup.lib" )


DEVMODE				g_UserMode;
DEVMODE				g_tempUserMode;
PCSystemInfo		g_PcSystemInfo;

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern BOOL	g_HoldGame;

void	RTrim(char* pStr)
{
	char* p = &(pStr[strlen(pStr) - 1]);
	while (p != pStr)
	{
		if (*p != ' ' && *p != '\r' && *p != '\n' && *p != 't') break;
		*p = '\0';
		--p;
	}
}

/// 0등급 : 고사양 / 1등급 : 중간사양 / 2등급 : 저사양 
int GetVideoCardGrade(char* szCardName)
{
	int grade = 2;

	char szUCardName[128];
	strcpy(szUCardName, szCardName);
	strupr(szUCardName);// 모든 소문자를 대문자로 변경
	RTrim(szUCardName);

	char szCardModel[128];
	char* p;

	if ((p = strstr(szUCardName, "GEFORCE")) != NULL)
	{
		strcpy(szCardModel, p);

		/// 지포스면 : FX, TI, MX 일단 구분, 지포스 4인지 2인지 구분 
		if ((p = strstr(szCardModel, "FX")) != NULL)
		{
			grade = 0;
		}
		else if ((p = strstr(szCardModel, "TI")) != NULL)
		{
			grade = 0;
		}
		else if ((p = strstr(szCardModel, "GEFORCE2")) != NULL)
		{
			grade = 2;
		}
		else
		{
			grade = 1;	/// Geforce4 MX 계열 
		}
	}
	else if ((p = strstr(szUCardName, "RADEON")) != NULL)
	{
		strcpy(szCardModel, p);

		/// 라데온이면 바로 뒤에 나오는 4자리수 읽기 
		char szSub[8];
		ZeroMemory(szSub, sizeof(szSub));
		strncpy(szSub, p + 7, 4);
		int n = atoi(szSub);

		if (n >= 9100 && n != 9200)	/// 9100 이상이면 지포스 FX나 TI 계열과 동일하게 간주...(9200 제외)
		{
			grade = 0;
		}
		else if (n < 8000)
		{
			grade = 2;		/// Radeon 7000~ 
		}
		else
		{
			grade = 1;		/// Radeon 8500, 9000, 9200 
		}
	}
	else if ((p = strstr(szUCardName, "INTEL")) != NULL)
	{
		grade = 2;
	}
	else
	{
		grade = 2;
	}

	return grade;
}

int GetDirectXVersion()
{
	int major, minor;

	DWORD version, revision;
	DirectXSetupGetVersion(&version, &revision);
	if (version == 0x00000000)
	{
		major = 3;
		minor = 0;
	}
	else if (version == 0x00040005)
	{
		major = 5;
		minor = 0;
	}
	else if (version == 0x00040006)
	{
		major = 6;
		minor = 0;
	}
	else if (version == 0x00040007)
	{
		major = 7;
		minor = 0;
	}
	else if (version == 0x00040008)
	{
		major = 8;
		if (revision > 0x00010000)
			minor = 1;
		else
			minor = 0;
	}
	else if (version == 0x00040009)
	{
		major = 9;
		minor = 0;
	}
	else
	{
		major = 0;
		minor = 0;
	}

	return(major * 10 + minor);
}


//-----------------------------------------------------------------------------
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------
static CD3DApplication* g_pD3DApp = NULL;




//-----------------------------------------------------------------------------
// Name: CD3DApplication()
// Desc: Constructor
//-----------------------------------------------------------------------------
CD3DApplication::CD3DApplication()
{
	g_pD3DApp = this;

#ifdef CPU_SHARE_RATE
	m_bFocus = TRUE;
#endif	//	CPU_SHARE_RATE

	m_dwNumAdapters = 0;
	m_dwAdapter = 0L;
	m_pD3D = NULL;
	m_pd3dDevice = NULL;
	m_hWnd = NULL;
	m_hWndFocus = NULL;

	m_hWindowWnd = NULL;
	m_hFullmodeWnd = NULL;

	m_bActive = FALSE;
	m_bReady = FALSE;
	m_dwCreateFlags = 0L;

	m_bFrameMoving = TRUE;
	m_bSingleStep = FALSE;
	m_fFPS = 0.0f;
	m_strDeviceStats[0] = _T('\0');
	m_strFrameStats[0] = _T('\0');

	m_bFullScreen = FALSE;
	m_EWindowsMode = MODE_NORMAL_FULLMODE;

#ifdef DIRECT_VERSION_9_MJH
	m_strWindowTitle = _T("D3D9Application");
#else
	m_strWindowTitle = _T("D3D8Application");
#endif // DIRECT_VERSION_9_MJH
	m_dwCreationWidth = 1024;
	m_dwCreationHeight = 768;
	m_wPixelFormat = 16;
	m_bUseDepthBuffer = TRUE;
	m_dwMinDepthBits = 16;
	m_dwMinStencilBits = 0;
	m_bShowCursorWhenFullscreen = FALSE;

}

CD3DApplication::~CD3DApplication()
{
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
}

//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pD3DApp->MsgProc(hWnd, uMsg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: Create()
// Desc:
//-----------------------------------------------------------------------------

DWORD CD3DApplication::GetWindowModeStyle()
{

	DWORD m_dwWinStyle = 0; //WS_POPUP|WS_SYSMENU|WS_VISIBLE;

	if (m_EWindowsMode == MODE_NORMAL_FULLMODE)
	{
		m_dwWinStyle = WS_POPUP |
			WS_CAPTION | WS_SYSMENU | /* WS_THICKFRAME | */
			WS_MINIMIZEBOX | WS_VISIBLE;
	}
	else
	{
		if (m_bFullScreen)
		{
			m_dwWinStyle = WS_POPUP | WS_VISIBLE;
		}
		else
		{
			m_dwWinStyle = WS_POPUP |
				WS_CAPTION | WS_SYSMENU | /* WS_THICKFRAME | */
				WS_MINIMIZEBOX | WS_VISIBLE;
		}
	}

	return m_dwWinStyle;
}
HRESULT CD3DApplication::Create(HINSTANCE hInstance)
{
	HRESULT hr;


	// Unless a substitute hWnd has been specified, create a window to
	// render into
	if (m_hWnd == NULL)
	{
		// Register the windows class
//        WNDCLASS wndClass = { CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, WndProc, 0, 0, hInstance,
		ClientFilelog("DX9 test");


#ifdef UNICODE_ROCK
		//MAKEINTRESOURCE(IDI_ROCKCON1)
		WNDCLASSW wndClass = { 0, WndProc, 0, 0, hInstance,
			LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON)),
			LoadCursor(NULL, IDC_SIZEALL),
			(HBRUSH)GetStockObject(WHITE_BRUSH),
			NULL, _RT("D3D Window") };
		RegisterClassW(&wndClass);
#else

		WNDCLASS wndClass = { 0, WndProc, 0, 0, hInstance,
			LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON)),
			LoadCursor(NULL, IDC_SIZEALL),
			(HBRUSH)GetStockObject(WHITE_BRUSH),
			NULL, _T("D3D Window") };
		RegisterClass(&wndClass);

#endif

		SetLastError(0);

#ifdef SHUTDOWN_TEST
		if (GetLastErorrCode()) RMessageBox(NULL, _RT("Create 01-1"), _RT("RockSoft"), MB_OK);
#endif 			
		// Set the window's initial style
		m_dwWindowStyle = GetWindowModeStyle();

		// Set the window's initial width
		RECT rc;
		SetRect(&rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight);
		AdjustWindowRect(&rc, m_dwWindowStyle, false); // 메뉴가 없다면...반드시 false를 준다..

		// Create the render window
#ifdef UNICODE_ROCK
		m_hWnd = CreateWindowExW(0, _RT("D3D Window"), RWCHAR(m_strWindowTitle), m_dwWindowStyle,
			0, 0,
			(rc.right - rc.left), (rc.bottom - rc.top), 0L,
			NULL /* LoadMenu( hInstance, MAKEINTRESOURCE(IDR_MENU) ) */,
			hInstance, 0L);
#else
		m_hWnd = CreateWindowEx(0, _T("D3D Window"), m_strWindowTitle, m_dwWindowStyle,
			0, 0,
			(rc.right - rc.left), (rc.bottom - rc.top), 0L,
			NULL /* LoadMenu( hInstance, MAKEINTRESOURCE(IDR_MENU) ) */,
			hInstance, 0L);
#endif
	}

	// The focus window can be a specified to be a different window than the
	// device window.  If not, use the device window as the focus window.
	if (m_hWndFocus == NULL)
		m_hWndFocus = m_hWnd;

	m_hInstance = hInstance;
	// SetFocus
	::SetForegroundWindow(m_hWnd);
	// Save window properties
	m_dwWindowStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	GetWindowRect(m_hWnd, &m_rcWindowBounds);
	GetClientRect(m_hWnd, &m_rcWindowClient);

	// Create the Direct3D object
#ifdef DIRECT_VERSION_9_MJH
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
#else
	m_pD3D = Direct3DCreate8(D3D_SDK_VERSION);
#endif // DIRECT_VERSION_9_MJH
	if (m_pD3D == NULL)
	{
		return DisplayErrorMsg(D3DAPPERR_NODIRECT3D, MSGERR_APPMUSTEXIT, "NULL pD3D");
	}
#ifdef SHUTDOWN_TEST
	if (GetLastErorrCode()) RMessageBox(NULL, _RT("Create 01-2"), _RT("RockSoft"), MB_OK);
#endif 	

	ZeroMemory(&g_UserMode, sizeof(g_UserMode));
	::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &g_UserMode);

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/06)

	g_UserMode.dmPelsWidth = m_dwCreationWidth;
	g_UserMode.dmPelsHeight = m_dwCreationHeight;

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/06)

	long result_value = 0;

	ZeroMemory(&g_tempUserMode, sizeof(g_tempUserMode));
	g_tempUserMode = g_UserMode;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/06)

#else

	g_tempUserMode.dmPelsWidth = 1024;
	g_tempUserMode.dmPelsHeight = 768;

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/05/06)

	g_tempUserMode.dmBitsPerPel = 32;
	g_tempUserMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

	if (g_UserMode.dmBitsPerPel == 16)
	{
		//result_value = DISP_CHANGE_SUCCESSFUL;
		m_wPixelFormat = 16;
		g_tempUserMode.dmBitsPerPel = 16;
	}
	else
	{
#ifndef INDONESIA_VERSION

		//		result_value = ChangeDisplaySettings( &tempUserMode, 0 );
#endif
	}

	// Build a list of Direct3D adapters, modes and devices. The
	// ConfirmDevice() callback is used to confirm that only devices that
	// meet the app's requirements are considered.
	if (FAILED(hr = BuildDeviceList()))
	{
		SAFE_RELEASE(m_pD3D);
		return DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT, "Fail BuildDeviceList");
	}

	// Initialize the app's custom scene stuff
	if (FAILED(hr = OneTimeSceneInit()))
	{
		SAFE_RELEASE(m_pD3D);
		return DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT, "Fail OneTimeSceneInit");
	}

#ifdef SHUTDOWN_TEST
	if (GetLastErorrCode()) RMessageBox(NULL, _RT("Create 01-3"), _RT("RockSoft"), MB_OK);
#endif 	
	// Initialize the 3D environment for the app
	if (FAILED(hr = Initialize3DEnvironment()))
	{
		SAFE_RELEASE(m_pD3D);
		return DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT, "Fail Initialize3DEnvironment");
	}

	// Setup the app so it can support single-stepping
	DXUtil_Timer(TIMER_START);

	// The app is ready to go
	m_bReady = TRUE;

	if (m_bFullScreen)
	{
		ToggleFullscreen(0);
	}

	Sleep(100);

	if (FAILED(hr = InitState()))
	{
		SAFE_RELEASE(m_pD3D);
		return DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT, "Fail InitState");
	}
	SetLastError(0);
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SortModesCallback()
// Desc: Callback function for sorting display modes (used by BuildDeviceList).
//-----------------------------------------------------------------------------
int SortModesCallback(const VOID* arg1, const VOID* arg2)
{
	D3DDISPLAYMODE* p1 = (D3DDISPLAYMODE*)arg1;
	D3DDISPLAYMODE* p2 = (D3DDISPLAYMODE*)arg2;

	if (p1->Format > p2->Format)   return -1;
	if (p1->Format < p2->Format)   return +1;
	if (p1->Width < p2->Width)    return -1;
	if (p1->Width > p2->Width)    return +1;
	if (p1->Height < p2->Height)   return -1;
	if (p1->Height > p2->Height)   return +1;

	return 0;
}

//-----------------------------------------------------------------------------
// Name: BuildDeviceList()
// Desc: 적합한 디바이스 드라이버를 찾아낸다.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::BuildDeviceList()
{
	const DWORD dwNumDeviceTypes = 2;
	const TCHAR* strDeviceDescs[] = { _T("HAL"), _T("REF") };
	const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

	BOOL bHALExists = FALSE;// 디바이스가 HAL인지 레퍼런스 래스터라이저 인지를 나타낸다.

	// 디바이스 드라이버가 윈도우모드로 렌더 가능한지? 부두 2 같은 그래픽카드에서는 윈도우 모드에서의
	// 렌더가 불가능.
	BOOL bHALIsWindowedCompatible = FALSE;

	// bHALIsWindowedCompatible이 TRUE이고, 그 다바이스가 현재 데스크탑의 모드를 지원한다면 TRUE
	BOOL bHALIsDesktopCompatible = FALSE;

	// ConfirmDevice()에 의해서 검증되고, HAL 디바이스가 사용되었으면 TRUE
	BOOL bHALIsSampleCompatible = FALSE;

	// Loop through all the adapters on the system (usually, there's just one
	// unless more than one graphics card is present).
	// 아래 for()문의 2가지 주요 목적
	// 1. D3DModeInfo와 D3DDeviceInfo, D3DadapterInfo 구조체의 값을 채운다.
	// 2. 640 X 480 X 16 모드(해상도와 칼라비트수)를 기본모드로 선택한다.
	for (UINT iAdapter = 0; iAdapter < m_pD3D->GetAdapterCount(); iAdapter++)
	{
		// Fill in adapter info
		D3DAdapterInfo* pAdapter = &m_Adapters[m_dwNumAdapters];
		m_pD3D->GetAdapterIdentifier(iAdapter, 0, &pAdapter->d3dAdapterIdentifier);
		m_pD3D->GetAdapterDisplayMode(iAdapter, &pAdapter->d3ddmDesktop);
		pAdapter->dwNumDevices = 0;
		pAdapter->dwCurrentDevice = 0;

		// Enumerate all display modes on this adapter
		D3DDISPLAYMODE modes[100];
		D3DFORMAT      formats[20];
		DWORD dwNumFormats = 0;
		DWORD dwNumModes = 0;

		// 어댑터에서 지원되는 가능한 모드들의 개수
#ifdef DIRECT_VERSION_9_MJH
		DWORD dwNumAdapterModes = m_pD3D->GetAdapterModeCount(iAdapter, pAdapter->d3ddmDesktop.Format);
#else
		DWORD dwNumAdapterModes = m_pD3D->GetAdapterModeCount(iAdapter);
#endif // DIRECT_VERSION_9_MJH

		// Add the adapter's current desktop format to the list of formats
		formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

		for (UINT iMode = 0; iMode < dwNumAdapterModes; iMode++)
		{
			// Get the display mode attributes
			D3DDISPLAYMODE DisplayMode;
#ifdef DIRECT_VERSION_9_MJH
			m_pD3D->EnumAdapterModes(iAdapter, pAdapter->d3ddmDesktop.Format, iMode, &DisplayMode);
#else
			m_pD3D->EnumAdapterModes(iAdapter, iMode, &DisplayMode);
#endif // DIRECT_VERSION_9_MJH

			// Filter out low-resolution modes
			if (DisplayMode.Width < 640 || DisplayMode.Height < 400)
				continue;

			// Check if the mode already exists (to filter out refresh rates)
			// 같은 해상도이고 화면 리프래쉬율만 다를 경우에는 같은 것으로 처리한다는 것으로
			// 화면 리프래쉬율은 상관하지 않겠다는 것이다.
			DWORD m;
			for (m = 0L; m < dwNumModes; m++)
			{
				if ((modes[m].Width == DisplayMode.Width) &&
					(modes[m].Height == DisplayMode.Height) &&
					(modes[m].Format == DisplayMode.Format))
					break;
			}

			// If we found a new mode, add it to the list of modes
			if (m == dwNumModes)
			{
				modes[dwNumModes].Width = DisplayMode.Width;
				modes[dwNumModes].Height = DisplayMode.Height;
				modes[dwNumModes].Format = DisplayMode.Format;
				modes[dwNumModes].RefreshRate = 0;
				dwNumModes++;

				// Check if the mode's format already exists
				DWORD f;
				for (f = 0; f < dwNumFormats; f++)
				{
					if (DisplayMode.Format == formats[f])
						break;
				}

				// If the format is new, add it to the list
				if (f == dwNumFormats)
					formats[dwNumFormats++] = DisplayMode.Format;
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////
		//	CHECK VERSION | DRIVERS
		OSVERSIONINFO           osvi;
		ZeroMemory(&osvi, sizeof(osvi));
		osvi.dwOSVersionInfoSize = sizeof(osvi);

		GetVersionEx(&osvi);  // Assume this function 

		g_PcSystemInfo.DXVersion = GetDirectXVersion();
		strcpy(g_PcSystemInfo.CardName, m_Adapters[0].d3dAdapterIdentifier.Description);
		g_PcSystemInfo.DriverVersion = m_Adapters[0].d3dAdapterIdentifier.DriverVersion;
		g_PcSystemInfo.OSVersion = osvi.dwPlatformId;

		{
			LPDIRECTDRAW7	lpDD = NULL;
			DDSCAPS2		ddsCaps2;
			DWORD			dwTotal, dwFree;
			HRESULT			hr;

			// DirectDraw 객체를 생성하는 함수이다
			hr = DirectDrawCreateEx(
				NULL,// 드라이버의 GUID의 포인터를 설정. NULL을 사용하면 현재 활성화된 드라이버가 선택 
				(VOID**)&lpDD,
				IID_IDirectDraw7,
				NULL);
			if (SUCCEEDED(hr))
			{
				ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));
				ddsCaps2.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;
				hr = lpDD->GetAvailableVidMem(&ddsCaps2, // 사용 가능한 비디오 메모리 측정
					&dwTotal,// 바이트 단위로 나타내는, 이용 가능한 디스플레이 메모리의 총용량을 저장하는
							// 변수의 주소. 취득한 값은, 총비디오 메모리 용량을 반영한다. 이 용량에는,
							// 프라이머리 표면용 및 디스플레이 드라이버가 확보하는 Private 캐쉬용의 비디오
							// 메모리는 포함되지 않는다. 
					&dwFree);// 디스플레이 메모리의 현재의 빈 용량을 저장 하는 변수의 주소. 
				if (SUCCEEDED(hr))
				{
					g_PcSystemInfo.VidioMemory = dwTotal / (1024 * 1024);
				}

				if (lpDD) lpDD->Release();
			}
		}

		DWORD dUseableVirtualMemory = 0;

		{
			MEMORYSTATUS MemStatus;
			MemStatus.dwLength = sizeof(MemStatus);
			GlobalMemoryStatus(&MemStatus);// 메모리 양
			g_PcSystemInfo.SystemMemory = MemStatus.dwTotalPhys / (1024 * 1024);
			dUseableVirtualMemory = MemStatus.dwAvailVirtual / (1024 * 1024);
		}
		///	CHECK VERSION | DRIVERS /////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////
		/// Initialize Client Option(INI 파일에서 정보를 읽어온다)
		int optInit = GetPrivateProfileInt(
			"Client",// 정보를 읽어올 섹션 이름
			"InitSetup",// 정보를 읽어올 키이름
			0,// INI 파일이 없거나 섹션, 키 등이 없어 값을 읽을 수 없을 때 디폴트값
			"./setup.ini");
		if (optInit == 0)
		{
			optInit = 1;

			char buf[32];
			int optShadow = 0;
			int optGlow = 0;
			int optObjView = 1;
			int optLODTerrain = 1;
			int optTextureDetail = 0;

			/// 1. 그래픽 카드 
			/// 고사양  : 실시간 그림자, 글로우 on 
			/// 중간사양: 실시간 그림자, 글로우 off 
			/// 저사양  : 둥그런 그림자, 글로우 off 
			int grade = GetVideoCardGrade(g_PcSystemInfo.CardName);
			switch (grade)
			{
			case 0: optShadow = 2;		optGlow = 30;		break;
			case 1: optShadow = 2;		optGlow = 0;		break;
			case 2: optShadow = 1;		optGlow = 0;		break;
			}

			/// 2. 비디오 메모리 
			/// 50M 미만(32M) : 오브젝트 뷰 = 1, 원경 off 
			/// 50M ~ 100M 미만(64M) : 오브젝트 뷰 = 2, 원경 on 
			/// 100M 이상(128M) : 오브젝트 뷰 = 3, 원경 on 
			if (g_PcSystemInfo.VidioMemory < 50)
			{
				optObjView = 1;		optLODTerrain = 0;
				optTextureDetail = 2;
			}
			else if (g_PcSystemInfo.VidioMemory < 100)
			{
				optObjView = 2;		optLODTerrain = 1;
				optTextureDetail = 1;
			}
			else
			{
				optObjView = 3;		optLODTerrain = 1;
				optGlow = 30;	/// MX 계열이나 9000, 9200 이래도 비디오 메모리 128M 이상이면 글로우 ON
			}

			/// 3. 시스템 메모리 : 시스템 메모리가 딸리면 앞서 설정된 옵션도 재설정 
			/// 128 이하 (128M) : 그림자 off, 글로우 off, 오브젝트뷰=0, 원경 off 
			/// 256 이하 (256M) : 글로우 off, 오브젝트뷰=1, 원경 on 
			if (g_PcSystemInfo.SystemMemory <= 128)
			{
				optShadow = 0;		optGlow = 0;		/// 최저사양 
				optObjView = 0;		optLODTerrain = 0;
			}
			else if (g_PcSystemInfo.SystemMemory <= 256)
			{
				optGlow = 0;	optObjView = 1;		optLODTerrain = 1;
			}

			wsprintf(buf, "%d", optInit);
			WritePrivateProfileString("Client", "InitSetup", buf, "./setup.ini");
			wsprintf(buf, "%d", optShadow);
			WritePrivateProfileString("Client", "Shadow", buf, "./setup.ini");
			wsprintf(buf, "%d", optGlow);
			WritePrivateProfileString("Client", "Glow", buf, "./setup.ini");
			wsprintf(buf, "%d", optObjView);
			WritePrivateProfileString("Client", "ObjView", buf, "./setup.ini");
			wsprintf(buf, "%d", optLODTerrain);
			WritePrivateProfileString("Client", "LODTerrain", buf, "./setup.ini");
			wsprintf(buf, "%d", optTextureDetail);
			WritePrivateProfileString("Client", "TexDetail", buf, "./setup.ini");
		}
		//////////////////////////////////////////////////////////////////////////////////////


		// Sort the list of display modes (by format, then width, then height)
		qsort(modes, dwNumModes, sizeof(D3DDISPLAYMODE), SortModesCallback);

		// Add devices to adapter
		for (UINT iDevice = 0; iDevice < dwNumDeviceTypes; iDevice++)
		{
			// Fill in device info
			D3DDeviceInfo* pDevice;
			pDevice = &pAdapter->devices[pAdapter->dwNumDevices];
			pDevice->DeviceType = DeviceTypes[iDevice];
			m_pD3D->GetDeviceCaps(iAdapter, DeviceTypes[iDevice], &pDevice->d3dCaps);
			pDevice->strDesc = strDeviceDescs[iDevice];
			pDevice->dwNumModes = 0;
			pDevice->dwCurrentMode = 0;
			pDevice->bCanDoWindowed = FALSE;
			pDevice->bWindowed = FALSE;
			pDevice->MultiSampleType = D3DMULTISAMPLE_NONE;

			// Examine each format supported by the adapter to see if it will
			// work with this device and meets the needs of the application.
			BOOL  bFormatConfirmed[20];
			DWORD dwBehavior[20];
			D3DFORMAT fmtDepthStencil[20];

			for (DWORD f = 0; f < dwNumFormats; f++)
			{
				bFormatConfirmed[f] = FALSE;
				fmtDepthStencil[f] = D3DFMT_UNKNOWN;

				// Skip formats that cannot be used as render targets on this device
				if (FAILED(m_pD3D->CheckDeviceType(iAdapter, pDevice->DeviceType,
					formats[f], formats[f], FALSE)))
					continue;

				if (pDevice->DeviceType == D3DDEVTYPE_HAL)
				{
					// This system has a HAL device
					bHALExists = TRUE;
#ifdef DIRECT_VERSION_9_MJH
					if (pDevice->d3dCaps.Caps2)
#else
					if (pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED)
#endif // DIRECT_VERSION_9_MJH      
					{
						// HAL can run in a window for some mode
						bHALIsWindowedCompatible = TRUE;

						if (f == 0)
						{
							// HAL can run in a window for the current desktop mode
							bHALIsDesktopCompatible = TRUE;
						}
					}
				}

				// Confirm the device/format for HW vertex processing
				if (pDevice->d3dCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
				{
					if (pDevice->d3dCaps.DevCaps&D3DDEVCAPS_PUREDEVICE)
					{
						dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;// | D3DCREATE_PUREDEVICE; ....sh

						if (SUCCEEDED(ConfirmDevice(&pDevice->d3dCaps, dwBehavior[f],
							formats[f])))
							bFormatConfirmed[f] = TRUE;
					}

					if (FALSE == bFormatConfirmed[f])
					{
						dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;

						if (SUCCEEDED(ConfirmDevice(&pDevice->d3dCaps, dwBehavior[f],
							formats[f])))
							bFormatConfirmed[f] = TRUE;
					}

					if (FALSE == bFormatConfirmed[f])
					{
						dwBehavior[f] = D3DCREATE_MIXED_VERTEXPROCESSING;

						if (SUCCEEDED(ConfirmDevice(&pDevice->d3dCaps, dwBehavior[f],
							formats[f])))
							bFormatConfirmed[f] = TRUE;
					}
				}

				// Confirm the device/format for SW vertex processing
				if (FALSE == bFormatConfirmed[f])
				{
					dwBehavior[f] = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

					if (SUCCEEDED(ConfirmDevice(&pDevice->d3dCaps, dwBehavior[f],
						formats[f])))
						bFormatConfirmed[f] = TRUE;
				}

				// Find a suitable depth/stencil buffer format for this device/format
				if (bFormatConfirmed[f] && m_bUseDepthBuffer)
				{
					if (!FindDepthStencilFormat(iAdapter, pDevice->DeviceType,
						formats[f], &fmtDepthStencil[f]))
					{
						DWORD dwMinDepthBits = m_dwMinDepthBits;
						DWORD dwMinStencilBits = m_dwMinStencilBits;
						m_dwMinDepthBits = 16;
						m_dwMinStencilBits = 0;

						if (!FindDepthStencilFormat(iAdapter, pDevice->DeviceType,
							formats[f], &fmtDepthStencil[f]))
						{
							bFormatConfirmed[f] = FALSE;
						}
						m_dwMinDepthBits = dwMinDepthBits;
						m_dwMinStencilBits = dwMinStencilBits;
					}
				}
			}

			// Add all enumerated display modes with confirmed formats to the
			// device's list of valid modes
			for (DWORD m = 0L; m < dwNumModes; m++)
			{
				for (DWORD f = 0; f < dwNumFormats; f++)
				{
					if (modes[m].Format == formats[f])
					{
						if (bFormatConfirmed[f] == TRUE)
						{
							// Add this mode to the device's list of valid modes
							pDevice->modes[pDevice->dwNumModes].Width = modes[m].Width;
							pDevice->modes[pDevice->dwNumModes].Height = modes[m].Height;
							pDevice->modes[pDevice->dwNumModes].Format = modes[m].Format;
							pDevice->modes[pDevice->dwNumModes].dwBehavior = dwBehavior[f];
							pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];
							pDevice->dwNumModes++;

							if (pDevice->DeviceType == D3DDEVTYPE_HAL)
								bHALIsSampleCompatible = TRUE;
						}
					}
				}
			}

			// Select any 640x480 mode for default (but prefer a 16-bit mode)
			// D3DFW 기본은 640 X 480이나, 사용자 폭/높이로 변경했다.
			for (DWORD m = 0; m < pDevice->dwNumModes; m++)
			{
				if (pDevice->modes[m].Width == m_dwCreationWidth && pDevice->modes[m].Height == m_dwCreationHeight)
				{
					pDevice->dwCurrentMode = m;

					if (m_wPixelFormat == 16)
					{
						if (pDevice->modes[m].Format == D3DFMT_R5G6B5 ||
							pDevice->modes[m].Format == D3DFMT_X1R5G5B5 ||
							pDevice->modes[m].Format == D3DFMT_A1R5G5B5)
						{
							break;
						}
					}
					else if (m_wPixelFormat == 32)
					{
						if (pDevice->modes[m].Format == D3DFMT_A8R8G8B8 ||
							pDevice->modes[m].Format == D3DFMT_X8R8G8B8)
							//							pDevice->modes[m].Format == D3DFMT_R8G8B8 )
						{
							break;
						}
					}
				}
			}

			// Check if the device is compatible with the desktop display mode
			// (which was added initially as formats[0])
#ifdef DIRECT_VERSION_9_MJH
			if (bFormatConfirmed[0] && (pDevice->d3dCaps.Caps2 /*& D3DCAPS2_CANAUTOGENMIPMAP*/))
#else
			if (bFormatConfirmed[0] && (pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED))
#endif // DIRECT_VERSION_9_MJH
			{
				pDevice->bCanDoWindowed = TRUE;
				pDevice->bWindowed = TRUE;
			}

			// If valid modes were found, keep this device
			if (pDevice->dwNumModes > 0)
				pAdapter->dwNumDevices++;
		}

		// If valid devices were found, keep this adapter
		if (pAdapter->dwNumDevices > 0)
			m_dwNumAdapters++;
	}

	// Return an error if no compatible devices were found
	if (0L == m_dwNumAdapters)
		return D3DAPPERR_NOCOMPATIBLEDEVICES;

	// Pick a default device that can render into a window
	// (This code assumes that the HAL device comes before the REF
	// device in the device array).
	for (DWORD a = 0; a < m_dwNumAdapters; a++)
	{
		for (DWORD d = 0; d < m_Adapters[a].dwNumDevices; d++)
		{
			if (m_Adapters[a].devices[d].bWindowed)
			{
				m_Adapters[a].dwCurrentDevice = d;
				m_dwAdapter = a;
				m_bWindowed = TRUE;

				// Display a warning message
				if (m_Adapters[a].devices[d].DeviceType == D3DDEVTYPE_REF)
				{
					if (!bHALExists)
						DisplayErrorMsg(D3DAPPERR_NOHARDWAREDEVICE, MSGWARN_SWITCHEDTOREF, "!bHALExists");
					else if (!bHALIsSampleCompatible)
						DisplayErrorMsg(D3DAPPERR_HALNOTCOMPATIBLE, MSGWARN_SWITCHEDTOREF, "!bHALIsSampleCompatible");
					else if (!bHALIsWindowedCompatible)
						DisplayErrorMsg(D3DAPPERR_NOWINDOWEDHAL, MSGWARN_SWITCHEDTOREF, "!bHALIsWindowedCompatible");
					else if (!bHALIsDesktopCompatible)
						DisplayErrorMsg(D3DAPPERR_NODESKTOPHAL, MSGWARN_SWITCHEDTOREF, "!bHALIsDesktopCompatible");
					else // HAL is desktop compatible, but not sample compatible
						DisplayErrorMsg(D3DAPPERR_NOHALTHISMODE, MSGWARN_SWITCHEDTOREF, "HAL is desktop compatible, but not sample compatible");
				}

				return S_OK;
			}
		}
	}

	return D3DAPPERR_NOWINDOWABLEDEVICES;
}




//-----------------------------------------------------------------------------
// Name: FindDepthStencilFormat()
// Desc: Finds a depth/stencil format for the given device that is compatible
//       with the render target format and meets the needs of the app.
//-----------------------------------------------------------------------------
BOOL CD3DApplication::FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType,
	D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat)
{
	if (m_dwMinDepthBits <= 16 && m_dwMinStencilBits == 0)
	{
		// 지정된 깊이/스텐실 버퍼의 포맷이 디바이스 상에서 지원 가능한 것인지를 결정
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)))
		{
			// 특정 디스플레이 모드에서 깊이/스텐실 버퍼의 포맷이 렌더타겟의 포맷과 호환 가능한 것인지 결정
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D16)))
			{
				*pDepthStencilFormat = D3DFMT_D16;
				return TRUE;
			}
		}
	}

	if (m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D15S1)))
			{
				*pDepthStencilFormat = D3DFMT_D15S1;
				return TRUE;
			}
		}
	}

	if (m_dwMinDepthBits <= 24 && m_dwMinStencilBits == 0)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24X8)))
			{
				*pDepthStencilFormat = D3DFMT_D24X8;
				return TRUE;
			}
		}
	}

	if (m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 8)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24S8)))
			{
				*pDepthStencilFormat = D3DFMT_D24S8;
				return TRUE;
			}
		}
	}

	if (m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 4)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24X4S4)))
			{
				*pDepthStencilFormat = D3DFMT_D24X4S4;
				return TRUE;
			}
		}
	}

	if (m_dwMinDepthBits <= 32 && m_dwMinStencilBits == 0)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D32)))
			{
				*pDepthStencilFormat = D3DFMT_D32;
				return TRUE;
			}
		}
	}

	return FALSE;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message handling function.
//-----------------------------------------------------------------------------
LRESULT CD3DApplication::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam)
{
	HRESULT hr;

	switch (uMsg)
	{
		//		case WM_PAINT:
					// Handle paint messages when the app is not ready
		//          if( m_pd3dDevice && !m_bReady )
		//			{
		//				if( m_bWindowed )
		//					g_RockClient.Present( NULL, NULL, NULL, NULL );
		//			}
		//			break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		break;

	case WM_ENTERSIZEMOVE:
		// Halt frame movement while the app is sizing or moving
		if (m_bFrameMoving)
			DXUtil_Timer(TIMER_STOP);
		break;

	case WM_SIZE:
		// Check to see if we are losing our window...
		if (SIZE_MAXHIDE == wParam || SIZE_MINIMIZED == wParam)
			m_bActive = FALSE;
		else
			m_bActive = TRUE;
		break;

	case WM_EXITSIZEMOVE:
		if (m_bFrameMoving)
			DXUtil_Timer(TIMER_START);

		if (m_bActive && m_bWindowed)
		{
			RECT rcClientOld;
			rcClientOld = m_rcWindowClient;

			// Update window properties
			GetWindowRect(m_hWnd, &m_rcWindowBounds);
			GetClientRect(m_hWnd, &m_rcWindowClient);

			if (rcClientOld.right - rcClientOld.left !=
				m_rcWindowClient.right - m_rcWindowClient.left ||
				rcClientOld.bottom - rcClientOld.top !=
				m_rcWindowClient.bottom - m_rcWindowClient.top)
			{
				// A new window size will require a new backbuffer
				// size, so the 3D structures must be changed accordingly.
				m_bReady = FALSE;

				m_d3dpp.BackBufferWidth = m_rcWindowClient.right - m_rcWindowClient.left;
				m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

				// Resize the 3D environment
				if (FAILED(hr = Resize3DEnvironment()))
				{
					DisplayErrorMsg(D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT, "Fail Resize3DEnvironment");
					return 0;
				}
				m_bReady = TRUE;
			}
		}

		break;

	case WM_SETCURSOR:
		break;

	case WM_MOUSEMOVE:
		if (m_bActive && m_bReady && m_pd3dDevice != NULL)
		{

		}
		break;

	case WM_ENTERMENULOOP:
		// Pause the app when menus are displayed
//            Pause(TRUE);
		break;

	case WM_EXITMENULOOP:
		//            Pause(FALSE);
		break;

	case WM_CONTEXTMENU:
		// No context menus allowed in fullscreen mode
		if (m_bWindowed == FALSE)
			break;

		// Handle the app's context menu (via right mouse click)
//            TrackPopupMenuEx( GetSubMenu( LoadMenu( 0, MAKEINTRESOURCE(IDR_POPUP) ), 0 ),
//                              TPM_VERTICAL, LOWORD(lParam), HIWORD(lParam), hWnd, NULL );
		break;

	case WM_NCHITTEST:
		// Prevent the user from selecting the menu in fullscreen mode
		if (!m_bWindowed)
			return HTCLIENT;

		break;

	case WM_POWERBROADCAST:
		switch (wParam)
		{
#ifndef PBT_APMQUERYSUSPEND
#define PBT_APMQUERYSUSPEND 0x0000
#endif
		case PBT_APMQUERYSUSPEND:
			// At this point, the app should save any data for open
			// network connections, files, etc., and prepare to go into
			// a suspended mode.
			return TRUE;

#ifndef PBT_APMRESUMESUSPEND
#define PBT_APMRESUMESUSPEND 0x0007
#endif
		case PBT_APMRESUMESUSPEND:
			// At this point, the app should recover any data, network
			// connections, files, etc., and resume running from when
			// the app was suspended.
			return TRUE;
		}
		break;

	case WM_SYSCOMMAND:
		// Prevent moving/sizing and power loss in fullscreen mode
		switch (wParam)
		{
		case SC_MOVE:
		case SC_SIZE:
		case SC_MAXIMIZE:
		case SC_KEYMENU:
		case SC_MONITORPOWER:
			//                    if( FALSE == m_bWindowed )	/// 윈도우 상태라고 하더라도 무조건 리턴 
			return 1;
			break;
			//스크린 세이버 동작 
		case SC_SCREENSAVE:
		{
			return 0;
		}
		break;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			/*                case IDM_TOGGLESTART:
								// Toggle frame movement
								m_bFrameMoving = !m_bFrameMoving;
								DXUtil_Timer( m_bFrameMoving ? TIMER_START : TIMER_STOP );
								break;

							case IDM_SINGLESTEP:
								// Single-step frame movement
								if( FALSE == m_bFrameMoving )
									DXUtil_Timer( TIMER_ADVANCE );
								else
									DXUtil_Timer( TIMER_STOP );
								m_bFrameMoving = FALSE;
								m_bSingleStep  = TRUE;
								break;

							case IDM_CHANGEDEVICE:
								// Prompt the user to select a new device or mode
								if( m_bActive && m_bReady )
								{
									Pause(TRUE);

									if( FAILED( hr = UserSelectNewDevice() ) )
										return 0;

									Pause(FALSE);
								}
								return 0;

							case IDM_TOGGLEFULLSCREEN:
								// Toggle the fullscreen/window mode
								if( m_bActive && m_bReady )
								{
									if( FAILED( ToggleFullscreen() ) )
									{
										DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
										return 0;
									}
								}
								return 0;
			*/
		case IDM_EXIT:
			// Recieved key/menu command to exit app
			ClientClose(hWnd, 1);
			return 0;
		}
		break;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	case WM_DISPLAYCHANGE:// 디스플레이 등록 정보 중 해상도 변경
	{
		//g_UserMode.dmPelsWidth = LOWORD(lParam);
		//g_UserMode.dmPelsHeight = HIWORD(lParam);
	}
	break;

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	case WM_CLOSE:
		Cleanup3DEnvironment();
		// DestroyMenu( GetMenu(hWnd) );
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		return 0;
	}
#ifdef UNICODE_ROCK
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
#else
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
#endif

}




//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Initialize3DEnvironment()
{
	HRESULT hr;

	// 이 값들은 BuildDeviceList()를 실행하는 동안 채워졌다.
	// m_dwAdapter : 선택 어댑터를 나타내는 인덱스
	D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
	D3DDeviceInfo*  pDeviceInfo = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
	D3DModeInfo*    pModeInfo = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

	// Prepare window for possible windowed/fullscreen change
	AdjustWindowForChange();

	// Set up the presentation parameters
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.Windowed = pDeviceInfo->bWindowed;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;//pDeviceInfo->MultiSampleType; ....sh
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//m_d3dpp.SwapEffect             = D3DSWAPEFFECT_COPY; 
	m_d3dpp.EnableAutoDepthStencil = m_bUseDepthBuffer;
	m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
	m_d3dpp.hDeviceWindow = m_hWnd;
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	if (m_bWindowed)
	{
		m_d3dpp.BackBufferWidth = m_rcWindowClient.right - m_rcWindowClient.left;
		m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
		m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
	}
	else
	{
		m_d3dpp.BackBufferWidth = pModeInfo->Width;
		m_d3dpp.BackBufferHeight = pModeInfo->Height;
		m_d3dpp.BackBufferFormat = pModeInfo->Format;

#ifdef DIRECT_VERSION_9_MJH
		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#else
		m_d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif // DIRECT_VERSION_9_MJH
		//m_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	}

	// Create the device
	hr = m_pD3D->CreateDevice(m_dwAdapter, pDeviceInfo->DeviceType,
		//								m_hWndFocus, pModeInfo->dwBehavior, &m_d3dpp,
		m_hWndFocus, pModeInfo->dwBehavior | D3DCREATE_MULTITHREADED, &m_d3dpp,
		&m_pd3dDevice);
	if (SUCCEEDED(hr))
	{
		// When moving from fullscreen to windowed mode, it is important to
		// adjust the window size after recreating the device rather than
		// beforehand to ensure that you get the window size you want.  For
		// example, when switching from 640x480 fullscreen to windowed with
		// a 1000x600 window on a 1024x768 desktop, it is impossible to set
		// the window size to 1000x600 until after the display mode has
		// changed to 1024x768, because windows cannot be larger than the
		// desktop.
		if (m_bWindowed)
		{
			SetWindowPos(m_hWnd, HWND_NOTOPMOST,
				m_rcWindowBounds.left, m_rcWindowBounds.top,
				(m_rcWindowBounds.right - m_rcWindowBounds.left),
				(m_rcWindowBounds.bottom - m_rcWindowBounds.top),
				SWP_SHOWWINDOW);
		}

		g_lpDevice = m_pd3dDevice;

		// Store device Caps
		g_RenderManager.GetDeviceCaps(&m_d3dCaps);


		m_dwCreateFlags = pModeInfo->dwBehavior;

		// Store device description
		if (pDeviceInfo->DeviceType == D3DDEVTYPE_REF)
			strcpy(m_strDeviceStats, TEXT("REF"));
		else if (pDeviceInfo->DeviceType == D3DDEVTYPE_HAL)
			strcpy(m_strDeviceStats, TEXT("HAL"));
		else if (pDeviceInfo->DeviceType == D3DDEVTYPE_SW)
			strcpy(m_strDeviceStats, TEXT("SW"));

		//if( pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING && pModeInfo->dwBehavior & D3DCREATE_PUREDEVICE )
		if (pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING && pModeInfo->dwBehavior & D3DCREATE_PUREDEVICE)
		{
			if (pDeviceInfo->DeviceType == D3DDEVTYPE_HAL)
				_tcscat(m_strDeviceStats, TEXT(" (pure hw vp)"));
			else
				_tcscat(m_strDeviceStats, TEXT(" (simulated pure hw vp)"));
		}
		else if (pModeInfo->dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		{
			if (pDeviceInfo->DeviceType == D3DDEVTYPE_HAL)
				_tcscat(m_strDeviceStats, TEXT(" (hw vp)"));
			else
				_tcscat(m_strDeviceStats, TEXT(" (simulated hw vp)"));
		}
		else if (pModeInfo->dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING)
		{
			if (pDeviceInfo->DeviceType == D3DDEVTYPE_HAL)
				_tcscat(m_strDeviceStats, TEXT(" (mixed vp)"));
			else
				_tcscat(m_strDeviceStats, TEXT(" (simulated mixed vp)"));
		}
		else if (pModeInfo->dwBehavior & D3DCREATE_SOFTWARE_VERTEXPROCESSING)
		{
			_tcscat(m_strDeviceStats, TEXT(" (sw vp)"));
		}

		if (pDeviceInfo->DeviceType == D3DDEVTYPE_HAL)
		{
			_tcscat(m_strDeviceStats, TEXT(": "));
			_tcscat(m_strDeviceStats, pAdapterInfo->d3dAdapterIdentifier.Description);
		}

		// Store render target surface desc

#ifdef DIRECT_VERSION_9_MJH
		LPDIRECT3DSURFACE9 pBackBuffer;
		g_RenderManager.GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
#else
		LPDIRECT3DSURFACE8 pBackBuffer;
		g_RenderManager.GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
#endif // DIRECT_VERSION_9_MJH
		pBackBuffer->GetDesc(&m_d3dsdBackBuffer);
		pBackBuffer->Release();

		// Set up the fullscreen cursor
		if (m_bShowCursorWhenFullscreen && !m_bWindowed)
		{
			HCURSOR hCursor;
#ifdef _WIN64
			hCursor = (HCURSOR)GetClassLongPtr(m_hWnd, GCLP_HCURSOR);
#else
			hCursor = (HCURSOR)GetClassLong(m_hWnd, GCL_HCURSOR);
#endif
			D3DUtil_SetDeviceCursor(m_pd3dDevice, hCursor, FALSE);
			g_RenderManager.ShowCursor(TRUE);
		}

		// Initialize the app's device-dependent objects
		hr = InitDeviceObjects();
		if (SUCCEEDED(hr))
		{
			hr = RestoreDeviceObjects();
			if (SUCCEEDED(hr))
			{
				m_bActive = TRUE;
				return S_OK;
			}
			else
			{
				MessageBox(NULL, "Fail RestoreDeviceObjects()", "Error", MB_ICONWARNING | MB_OK);
			}
		}
		else
		{
			MessageBox(NULL, "Fail InitDeviceObjects()", "Error", MB_ICONWARNING | MB_OK);
		}

		// Cleanup before we try again
		InvalidateDeviceObjects();
		DeleteDeviceObjects();
		SAFE_RELEASE(m_pd3dDevice);
	}
	else
	{
		MessageBox(NULL, "Fail CreateDevice", "Error", MB_ICONWARNING | MB_OK);

		ClientFilelog("m_d3dpp.Windowed = %d \n", m_d3dpp.Windowed);
		ClientFilelog("m_d3dpp.BackBufferCount = %d \n", m_d3dpp.BackBufferCount);
		ClientFilelog("m_d3dpp.MultiSampleType = %d \n", m_d3dpp.MultiSampleType);
		ClientFilelog("m_d3dpp.SwapEffect = %d \n", m_d3dpp.SwapEffect);
		ClientFilelog("m_d3dpp.EnableAutoDepthStencil = %d \n", m_d3dpp.EnableAutoDepthStencil);
		ClientFilelog("m_d3dpp.AutoDepthStencilFormat = %d \n", m_d3dpp.AutoDepthStencilFormat);
		ClientFilelog("m_d3dpp.hDeviceWindow = %d \n", m_d3dpp.hDeviceWindow);
		ClientFilelog("m_d3dpp.FullScreen_RefreshRateInHz = %d \n", m_d3dpp.FullScreen_RefreshRateInHz);
		ClientFilelog("m_d3dpp.BackBufferWidth = %d \n", m_d3dpp.BackBufferWidth);
		ClientFilelog("m_d3dpp.BackBufferHeight = %d \n", m_d3dpp.BackBufferHeight);
		ClientFilelog("m_d3dpp.BackBufferFormat = %d \n", m_d3dpp.BackBufferFormat);
		ClientFilelog("m_dwAdapter = %d \n", m_dwAdapter);
		ClientFilelog("pDeviceInfo->DeviceType = %d \n", pDeviceInfo->DeviceType);
		ClientFilelog("m_hWndFocus = %d \n", m_hWndFocus);
	}

	// If that failed, fall back to the reference rasterizer
	if (pDeviceInfo->DeviceType == D3DDEVTYPE_HAL)
	{
		// Let the user know we are switching from HAL to the reference rasterizer
		DisplayErrorMsg(hr, MSGWARN_SWITCHEDTOREF, "");

		// Select the default adapter
		m_dwAdapter = 0L;
		pAdapterInfo = &m_Adapters[m_dwAdapter];

		// Look for a software device
		for (UINT i = 0L; i < pAdapterInfo->dwNumDevices; i++)
		{
			if (pAdapterInfo->devices[i].DeviceType == D3DDEVTYPE_REF)
			{
				pAdapterInfo->dwCurrentDevice = i;
				pDeviceInfo = &pAdapterInfo->devices[i];
				m_bWindowed = pDeviceInfo->bWindowed;
				break;
			}
		}

		// Try again, this time with the reference rasterizer
		if (pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice].DeviceType ==
			D3DDEVTYPE_REF)
		{
			hr = Initialize3DEnvironment();
		}
	}

	return hr;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Resize3DEnvironment()
{
	HRESULT hr;

	// Release all vidmem objects
	if (FAILED(hr = InvalidateDeviceObjects()))
		return hr;

	// Reset the device
	if (FAILED(hr = g_RenderManager.Reset(&m_d3dpp)))
	{
		return hr;
	}

	// Store render target surface desc
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSURFACE9 pBackBuffer;
	g_RenderManager.GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
#else
	LPDIRECT3DSURFACE8 pBackBuffer;
	g_RenderManager.GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
#endif // DIRECT_VERSION_9_MJH
	pBackBuffer->GetDesc(&m_d3dsdBackBuffer);
	pBackBuffer->Release();

	// Set up the fullscreen cursor
	if (m_bShowCursorWhenFullscreen && !m_bWindowed)
	{
		HCURSOR hCursor;
#ifdef _WIN64
		hCursor = (HCURSOR)GetClassLongPtr(m_hWnd, GCLP_HCURSOR);
#else
		hCursor = (HCURSOR)GetClassLong(m_hWnd, GCL_HCURSOR);
#endif
		D3DUtil_SetDeviceCursor(m_pd3dDevice, hCursor, FALSE);
		g_RenderManager.ShowCursor(TRUE);
	}

	// Initialize the app's device-dependent objects
	hr = RestoreDeviceObjects();

	if (FAILED(hr))
		return hr;

	// If the app is paused, trigger the rendering of the current frame
	if (FALSE == m_bFrameMoving)
	{
		m_bSingleStep = TRUE;
		DXUtil_Timer(TIMER_START);
		DXUtil_Timer(TIMER_STOP);
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ToggleFullScreen()
// Desc: Called when user toggles between fullscreen mode and windowed mode
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ToggleFullscreen(bool bToggle /* = 0  */)
{
	// Get access to current adapter, device, and mode
	D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
	D3DDeviceInfo*  pDeviceInfo = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
	D3DModeInfo*    pModeInfo = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

	// Need device change if going windowed and the current device
	// can only be fullscreen
	if (!m_bWindowed && !pDeviceInfo->bCanDoWindowed)
		return ForceWindowed();

	m_bReady = FALSE;

	if (bToggle)
	{
		m_bFullScreen = !m_bFullScreen;
	}

	if (m_EWindowsMode == MODE_NORMAL_FULLMODE)
	{
		m_bWindowed = !m_bWindowed;
	}
	else
	{
		long result_value = 0;

		if (m_bFullScreen)
		{
			result_value = ChangeDisplaySettings(&g_tempUserMode, 0);
		}
		else
		{
			result_value = ChangeDisplaySettings(&g_UserMode, 0);
		}

		HRESULT hr;

		// Release all vidmem objects
		if (FAILED(hr = InvalidateDeviceObjects()))
			return hr;

		// Reset the device
		if (FAILED(hr = g_RenderManager.Reset(&m_d3dpp)))
		{
			return hr;
		}

		// Initialize the app's device-dependent objects
		if (FAILED(hr = RestoreDeviceObjects()))
			return hr;

		::SetFocus(m_hWnd);

		goto ModifySize;
	}

	pDeviceInfo->bWindowed = m_bWindowed;

	// Prepare window for windowed/fullscreen change
	AdjustWindowForChange();

	// Set up the presentation parameters
	m_d3dpp.Windowed = pDeviceInfo->bWindowed;
	m_d3dpp.MultiSampleType = pDeviceInfo->MultiSampleType;
	m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
	m_d3dpp.hDeviceWindow = m_hWnd;

	if (m_bWindowed)
	{
		m_d3dpp.BackBufferWidth = m_rcWindowClient.right - m_rcWindowClient.left;
		m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
		m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
	}
	else
	{
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

		m_d3dpp.BackBufferWidth = g_UserMode.dmPelsWidth;
		m_d3dpp.BackBufferHeight = g_UserMode.dmPelsHeight;

#else
		m_d3dpp.BackBufferWidth = pModeInfo->Width;
		m_d3dpp.BackBufferHeight = pModeInfo->Height;

#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

		m_d3dpp.BackBufferFormat = pModeInfo->Format;
#ifdef DIRECT_VERSION_9_MJH
		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#else
		m_d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif // DIRECT_VERSION_9_MJH
		//m_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	}

	// Resize the 3D device
	if (FAILED(Resize3DEnvironment()))
	{
		if (m_bWindowed)
			return ForceWindowed();
		else
			return E_FAIL;
	}

	// When moving from fullscreen to windowed mode, it is important to
	// adjust the window size after resetting the device rather than
	// beforehand to ensure that you get the window size you want.  For
	// example, when switching from 640x480 fullscreen to windowed with
	// a 1000x600 window on a 1024x768 desktop, it is impossible to set
	// the window size to 1000x600 until after the display mode has
	// changed to 1024x768, because windows cannot be larger than the
	// desktop.

ModifySize:

	if (m_bWindowed)
	{
		SetWindowPos(m_hWnd, HWND_NOTOPMOST,
			m_rcWindowBounds.left, m_rcWindowBounds.top,
			(m_rcWindowBounds.right - m_rcWindowBounds.left),
			(m_rcWindowBounds.bottom - m_rcWindowBounds.top),
			SWP_SHOWWINDOW);

		/*if( m_EWindowsMode == MODE_NORMAL_FULLMODE )
		{
			SetWindowPos( m_hWnd, HWND_NOTOPMOST,
						  m_rcWindowBounds.left, m_rcWindowBounds.top,
						  ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
						  ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
						  SWP_SHOWWINDOW );
		}
		else
		{
			if(m_bFullScreen)
			{
				SetWindowPos( m_hWnd, HWND_NOTOPMOST,
						   m_rcWindowClient.right - m_rcWindowBounds.right + 3,
						   m_rcWindowClient.bottom - m_rcWindowBounds.bottom,
						  ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
						  ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
						  SWP_SHOWWINDOW );
			}
			else
			{
				SetWindowPos( m_hWnd, HWND_NOTOPMOST,
						  0, 0,
						  ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
						  ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
						  SWP_SHOWWINDOW );
			}
		}*/
	}

	m_bReady = TRUE;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: ForceWindowed()
// Desc: Switch to a windowed mode, even if that means picking a new device
//       and/or adapter
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ForceWindowed()
{
	HRESULT hr;
	D3DAdapterInfo* pAdapterInfoCur = &m_Adapters[m_dwAdapter];
	D3DDeviceInfo*  pDeviceInfoCur = &pAdapterInfoCur->devices[pAdapterInfoCur->dwCurrentDevice];
	BOOL bFoundDevice = FALSE;

	if (pDeviceInfoCur->bCanDoWindowed)
	{
		bFoundDevice = TRUE;
	}
	else
	{
		// Look for a windowable device on any adapter
		D3DAdapterInfo* pAdapterInfo;
		DWORD dwAdapter;
		D3DDeviceInfo* pDeviceInfo;
		DWORD dwDevice;
		for (dwAdapter = 0; dwAdapter < m_dwNumAdapters; dwAdapter++)
		{
			pAdapterInfo = &m_Adapters[dwAdapter];
			for (dwDevice = 0; dwDevice < pAdapterInfo->dwNumDevices; dwDevice++)
			{
				pDeviceInfo = &pAdapterInfo->devices[dwDevice];
				if (pDeviceInfo->bCanDoWindowed)
				{
					m_dwAdapter = dwAdapter;
					pDeviceInfoCur = pDeviceInfo;
					pAdapterInfo->dwCurrentDevice = dwDevice;
					bFoundDevice = TRUE;
					break;
				}
			}
			if (bFoundDevice)
				break;
		}
	}

	if (!bFoundDevice)
		return E_FAIL;

	pDeviceInfoCur->bWindowed = TRUE;
	m_bWindowed = TRUE;

	// Now destroy the current 3D device objects, then reinitialize

	m_bReady = FALSE;

	// Release all scene objects that will be re-created for the new device
	InvalidateDeviceObjects();
	DeleteDeviceObjects();

	// Release display objects, so a new device can be created
	if (m_pd3dDevice->Release() > 0L)
		return DisplayErrorMsg(D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT);

	// Create the new device
	if (FAILED(hr = Initialize3DEnvironment()))
		return DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT, "Fail Initialize3DEnvironment");
	m_bReady = TRUE;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: AdjustWindowForChange()
// Desc: Prepare the window for a possible change between windowed mode and
//       fullscreen mode.  This function is virtual and thus can be overridden
//       to provide different behavior, such as switching to an entirely
//       different window for fullscreen mode (as in the MFC sample apps).
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::AdjustWindowForChange()
{
	m_dwWindowStyle = GetWindowModeStyle();

	if (m_bWindowed)
	{
		// Set windowed-mode style
		SetWindowLong(m_hWnd, GWL_STYLE, m_dwWindowStyle);
		SetClassLong(m_hWnd, GCL_HCURSOR,
			HandleToLong(LoadCursor(NULL, IDC_SIZEALL)));
	}
	else
	{
		// Set fullscreen-mode style
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE);
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UserSelectNewDevice()
// Desc: Displays a dialog so the user can select a new adapter, device, or
//       display mode, and then recreates the 3D environment if needed
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::UserSelectNewDevice()
{
	/*
		HRESULT hr;

		// Can't display dialogs in fullscreen mode
		if( m_bWindowed == FALSE )
		{
			if( FAILED( ToggleFullscreen() ) )
			{
				DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
				return E_FAIL;
			}
		}

		// Prompt the user to change the mode
		if( IDOK != DialogBoxParam( (HINSTANCE)GetModuleHandle(NULL),
									MAKEINTRESOURCE(IDD_SELECTDEVICE), m_hWnd,
									SelectDeviceProc, (LPARAM)this ) )
			return S_OK;

		// Get access to the newly selected adapter, device, and mode
		DWORD dwDevice;
		dwDevice  = m_Adapters[m_dwAdapter].dwCurrentDevice;
		m_bWindowed = m_Adapters[m_dwAdapter].devices[dwDevice].bWindowed;

		// Release all scene objects that will be re-created for the new device
		InvalidateDeviceObjects();
		DeleteDeviceObjects();

		// Release display objects, so a new device can be created
		if( m_pd3dDevice->Release() > 0L )
			return DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );

		// Inform the display class of the change. It will internally
		// re-create valid surfaces, a d3ddevice, etc.
		if( FAILED( hr = Initialize3DEnvironment() ) )
			return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );

		// If the app is paused, trigger the rendering of the current frame
		if( FALSE == m_bFrameMoving )
		{
			m_bSingleStep = TRUE;
			DXUtil_Timer( TIMER_START );
			DXUtil_Timer( TIMER_STOP );
		}
	*/
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SelectDeviceProc()
// Desc: Windows message handling function for the device select dialog
//-----------------------------------------------------------------------------
INT_PTR CALLBACK CD3DApplication::SelectDeviceProc(HWND hDlg, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	/*
		// Get access to the UI controls
		HWND hwndAdapterList        = GetDlgItem( hDlg, IDC_ADAPTER_COMBO );
		HWND hwndDeviceList         = GetDlgItem( hDlg, IDC_DEVICE_COMBO );
		HWND hwndFullscreenModeList = GetDlgItem( hDlg, IDC_FULLSCREENMODES_COMBO );
		HWND hwndWindowedRadio      = GetDlgItem( hDlg, IDC_WINDOW );
		HWND hwndFullscreenRadio    = GetDlgItem( hDlg, IDC_FULLSCREEN );
		HWND hwndMultiSampleList    = GetDlgItem( hDlg, IDC_MULTISAMPLE_COMBO );
		BOOL bUpdateDlgControls     = FALSE;

		// Static state for adapter/device/mode selection
		static CD3DApplication* pd3dApp;
		static DWORD  dwOldAdapter, dwNewAdapter;
		static DWORD  dwOldDevice,  dwNewDevice;
		static DWORD  dwOldMode,    dwNewMode;
		static BOOL   bOldWindowed, bNewWindowed;
		static D3DMULTISAMPLE_TYPE OldMultiSampleType, NewMultiSampleType;

		// Working variables
		D3DAdapterInfo* pAdapter;
		D3DDeviceInfo*  pDevice;

		// Handle the initialization message
		if( WM_INITDIALOG == msg )
		{
			// Old state
			pd3dApp      = (CD3DApplication*)lParam;
			dwOldAdapter = pd3dApp->m_dwAdapter;
			pAdapter     = &pd3dApp->m_Adapters[dwOldAdapter];

			dwOldDevice  = pAdapter->dwCurrentDevice;
			pDevice      = &pAdapter->devices[dwOldDevice];

			dwOldMode    = pDevice->dwCurrentMode;
			bOldWindowed = pDevice->bWindowed;
			OldMultiSampleType = pDevice->MultiSampleType;

			// New state is initially the same as the old state
			dwNewAdapter = dwOldAdapter;
			dwNewDevice  = dwOldDevice;
			dwNewMode    = dwOldMode;
			bNewWindowed = bOldWindowed;
			NewMultiSampleType = OldMultiSampleType;

			// Set flag to update dialog controls below
			bUpdateDlgControls = TRUE;
		}

		if( WM_COMMAND == msg )
		{
			// Get current UI state
			bNewWindowed  = Button_GetCheck( hwndWindowedRadio );

			if( IDOK == LOWORD(wParam) )
			{
				// Handle the case when the user hits the OK button. Check if any
				// of the options were changed
				if( dwNewAdapter != dwOldAdapter || dwNewDevice  != dwOldDevice  ||
					dwNewMode    != dwOldMode    || bNewWindowed != bOldWindowed ||
					NewMultiSampleType != OldMultiSampleType )
				{
					pd3dApp->m_dwAdapter = dwNewAdapter;

					pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
					pAdapter->dwCurrentDevice = dwNewDevice;

					pAdapter->devices[dwNewDevice].dwCurrentMode = dwNewMode;
					pAdapter->devices[dwNewDevice].bWindowed     = bNewWindowed;
					pAdapter->devices[dwNewDevice].MultiSampleType = NewMultiSampleType;

					EndDialog( hDlg, IDOK );
				}
				else
					EndDialog( hDlg, IDCANCEL );

				return TRUE;
			}
			else if( IDCANCEL == LOWORD(wParam) )
			{
				// Handle the case when the user hits the Cancel button
				EndDialog( hDlg, IDCANCEL );
				return TRUE;
			}
			else if( CBN_SELENDOK == HIWORD(wParam) )
			{
				if( LOWORD(wParam) == IDC_ADAPTER_COMBO )
				{
					dwNewAdapter = ComboBox_GetCurSel( hwndAdapterList );
					pAdapter     = &pd3dApp->m_Adapters[dwNewAdapter];

					dwNewDevice  = pAdapter->dwCurrentDevice;
					dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
					bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
				}
				else if( LOWORD(wParam) == IDC_DEVICE_COMBO )
				{
					pAdapter     = &pd3dApp->m_Adapters[dwNewAdapter];

					dwNewDevice  = ComboBox_GetCurSel( hwndDeviceList );
					dwNewMode    = pAdapter->devices[dwNewDevice].dwCurrentMode;
					bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
				}
				else if( LOWORD(wParam) == IDC_FULLSCREENMODES_COMBO )
				{
					dwNewMode = ComboBox_GetCurSel( hwndFullscreenModeList );
				}
				else if( LOWORD(wParam) == IDC_MULTISAMPLE_COMBO )
				{
					DWORD dwItem = ComboBox_GetCurSel( hwndMultiSampleList );
					NewMultiSampleType = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
				}
			}
			// Keep the UI current
			bUpdateDlgControls = TRUE;
		}

		// Update the dialog controls
		if( bUpdateDlgControls )
		{
			// Reset the content in each of the combo boxes
			ComboBox_ResetContent( hwndAdapterList );
			ComboBox_ResetContent( hwndDeviceList );
			ComboBox_ResetContent( hwndFullscreenModeList );
			ComboBox_ResetContent( hwndMultiSampleList );

			pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
			pDevice  = &pAdapter->devices[dwNewDevice];

			// Add a list of adapters to the adapter combo box
			for( DWORD a=0; a < pd3dApp->m_dwNumAdapters; a++ )
			{
				// Add device name to the combo box
				DWORD dwItem = ComboBox_AddString( hwndAdapterList,
								 pd3dApp->m_Adapters[a].d3dAdapterIdentifier.Description );

				// Set the item data to identify this adapter
				ComboBox_SetItemData( hwndAdapterList, dwItem, a );

				// Set the combobox selection on the current adapater
				if( a == dwNewAdapter )
					ComboBox_SetCurSel( hwndAdapterList, dwItem );
			}

			// Add a list of devices to the device combo box
			for( DWORD d=0; d < pAdapter->dwNumDevices; d++ )
			{
				// Add device name to the combo box
				DWORD dwItem = ComboBox_AddString( hwndDeviceList,
												   pAdapter->devices[d].strDesc );

				// Set the item data to identify this device
				ComboBox_SetItemData( hwndDeviceList, dwItem, d );

				// Set the combobox selection on the current device
				if( d == dwNewDevice )
					ComboBox_SetCurSel( hwndDeviceList, dwItem );
			}

			// Add a list of modes to the mode combo box
			for( DWORD m=0; m < pDevice->dwNumModes; m++ )
			{
				DWORD BitDepth = 16;
				if( pDevice->modes[m].Format == D3DFMT_X8R8G8B8 ||
					pDevice->modes[m].Format == D3DFMT_A8R8G8B8 ||
					pDevice->modes[m].Format == D3DFMT_R8G8B8 )
				{
					BitDepth = 32;
				}

				// Add mode desc to the combo box
				TCHAR strMode[80];
				_stprintf( strMode, _T("%ld x %ld x %ld"), pDevice->modes[m].Width,
														   pDevice->modes[m].Height,
														   BitDepth );
				DWORD dwItem = ComboBox_AddString( hwndFullscreenModeList, strMode );

				// Set the item data to identify this mode
				ComboBox_SetItemData( hwndFullscreenModeList, dwItem, m );

				// Set the combobox selection on the current mode
				if( m == dwNewMode )
					ComboBox_SetCurSel( hwndFullscreenModeList, dwItem );
			}

			// Add a list of multisample modes to the multisample combo box
			for( m=0; m <= 16; m++ )
			{
				TCHAR strDesc[50];

				D3DFORMAT fmt;
				if( bNewWindowed )
					fmt = pd3dApp->m_Adapters[dwNewAdapter].d3ddmDesktop.Format;
				else
					fmt = pDevice->modes[dwNewMode].Format;

				if ( m == 1 ) // 1 is not a valid multisample type
					continue;

				if( SUCCEEDED( pd3dApp->m_pD3D->CheckDeviceMultiSampleType( dwNewAdapter,
					pDevice->DeviceType, fmt, bNewWindowed, (D3DMULTISAMPLE_TYPE)m ) ) )
				{
					// Add device name to the combo box
					DWORD dwItem = ComboBox_AddString( hwndMultiSampleList, strDesc );

					// Set the item data to identify this multisample type
					ComboBox_SetItemData( hwndMultiSampleList, dwItem, m );

					// Set the combobox selection on the current multisample type
					if( (D3DMULTISAMPLE_TYPE)m == NewMultiSampleType || m == 0 )
						ComboBox_SetCurSel( hwndMultiSampleList, dwItem );
				}
			}
			DWORD dwItem = ComboBox_GetCurSel( hwndMultiSampleList );
			NewMultiSampleType = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData( hwndMultiSampleList, dwItem );
			EnableWindow( hwndMultiSampleList, ComboBox_GetCount( hwndMultiSampleList ) > 1);
			EnableWindow( hwndWindowedRadio, pDevice->bCanDoWindowed );

			if( bNewWindowed )
			{
				Button_SetCheck( hwndWindowedRadio,   TRUE );
				Button_SetCheck( hwndFullscreenRadio, FALSE );
				EnableWindow( hwndFullscreenModeList, FALSE );
			}
			else
			{
				Button_SetCheck( hwndWindowedRadio,   FALSE );
				Button_SetCheck( hwndFullscreenRadio, TRUE );
				EnableWindow( hwndFullscreenModeList, TRUE );
			}
			return TRUE;
		}
	*/
	return FALSE;
}




//-----------------------------------------------------------------------------
// Name: Run()
// Desc:
//-----------------------------------------------------------------------------
INT CD3DApplication::Run()
{
	//	ProfileBegin("Run0913");
		// Load keyboard accelerators
	HACCEL hAccel = LoadAccelerators(NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL));

	// Now we're ready to recieve and process Windows messages.
	BOOL bGotMsg;
	MSG  msg;
	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{

		///---------------------------------------------------------------------------
		///-- Debug Mode CPU 점유율 감소
		///---------------------------------------------------------------------------
		///-- sleep(5) 부터는 Frame 저하가 생긴다.
#ifdef _DEBUG
		int l_nDevSkip = GetPrivateProfileInt("Frame", "Loop_Sleep", 0, "./dev.ini");
		if (l_nDevSkip)
		{
			//Sleep(4);
		}

#endif _DEBUG

		///-- 메인 루프안에 Sleep()함수 돌리면 
		///-- 마우스를 움직일때 프레임저하현상이 일어난다.	

		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
#ifdef UNICODE_ROCK
		if (m_bActive)
			bGotMsg = PeekMessageW(&msg, NULL, 0U, 0U, PM_REMOVE);
		else
			bGotMsg = GetMessageW(&msg, NULL, 0U, 0U);

		if (bGotMsg)
		{
			// Translate and dispatch the message
			if (0 == TranslateAcceleratorW(m_hWnd, hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
#else
		if (m_bActive)
			bGotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
		else
			bGotMsg = GetMessage(&msg, NULL, 0U, 0U);

		if (bGotMsg)
		{
			// Translate and dispatch the message
			if (0 == TranslateAccelerator(m_hWnd, hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
#endif
		else
		{
			// Render a frame during idle time (no messages are waiting)
			if (m_bActive && m_bReady)
			{
#ifdef CPU_SHARE_RATE
				if (!m_bFocus)
				{
					// Yield some CPU time to other processes
					Sleep(100); // 100 milliseconds
				//	DebugPrint( "!\r\n 포커스 잃음" );	
				}
				// 				else
				// 				{
								//	DebugPrint( "!\r\n 포커스 얻음" );	
				// 				}
#endif	//	CPU_SHARE_RATE

				if (!g_HoldGame)
				{
					if (FAILED(Render3DEnvironment()))
					{
						ClientClose(m_hWnd, 1);
					}
				}
			}
		}
	}

	//	ProfileEnd("Run0913");
	return (INT)msg.wParam;
}




//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Render3DEnvironment()
{
	//	ProfileBegin( "Render3DEnvironment0913" );
	HRESULT hr;

	m_bDeviceLost = FALSE;

	// Test the cooperative level to see if it's okay to render
	if (FAILED(hr = g_RenderManager.TestCooperativeLevel()))
	{
		// If the device was lost, do not render until we get it back        
		if (D3DERR_DEVICELOST == hr)
		{
			//			ProfileEnd( "Render3DEnvironment0913" );
			m_bDeviceLost = TRUE;

			// Frame move the scene
			/*if( FAILED( hr = FrameMove() ) )
			{
				return hr;
			}*/

			return S_OK;
		}

		// Check if the device needs to be resized.
		if (D3DERR_DEVICENOTRESET == hr)
		{
			// If we are windowed, read the desktop mode and use the same format for
			// the back buffer
			if (m_bWindowed)
			{
				D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
				m_pD3D->GetAdapterDisplayMode(m_dwAdapter, &pAdapterInfo->d3ddmDesktop);
				m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
			}

			//			ProfileBegin( "Resize3DEnvironment0913" );
			if (FAILED(hr = Resize3DEnvironment()))
			{
				//				ProfileEnd( "Resize3DEnvironment0913" );
				//				ProfileEnd( "Render3DEnvironment0913" );
				return hr;
			}
			//			ProfileEnd( "Resize3DEnvironment0913" );
		}
		//		ProfileEnd( "Render3DEnvironment0913" );
		return hr;
	}

	// Get the app's time, in seconds. Skip rendering if no time elapsed
	FLOAT fAppTime = DXUtil_Timer(TIMER_GETAPPTIME);
	FLOAT fElapsedAppTime = DXUtil_Timer(TIMER_GETELAPSEDTIME);
	if ((0.0f == fElapsedAppTime) && m_bFrameMoving)
		return S_OK;

	// FrameMove (animate) the scene
	if (m_bFrameMoving || m_bSingleStep)
	{
		// Store the time for the app
		m_fTime = fAppTime;
		m_fElapsedTime = fElapsedAppTime;

		// Frame move the scene
		if (FAILED(hr = FrameMove()))
		{
			//			ProfileEnd( "Render3DEnvironment0913" );
			return hr;
		}

		m_bSingleStep = FALSE;
	}

	// Render the scene as normal

	if (FAILED(hr = Render()))
	{
		//		ProfileEnd( "Render3DEnvironment0913" );
		return hr;
	}

	// Keep track of the frame count
	{
		static FLOAT fLastTime = 0.0f;
		static DWORD dwFrames = 0L;
		FLOAT fTime = DXUtil_Timer(TIMER_GETABSOLUTETIME);
		++dwFrames;

		// Update the scene stats once per second
		if (fTime - fLastTime > 1.0f)
		{
			m_fFPS = dwFrames / (fTime - fLastTime);
			fLastTime = fTime;
			dwFrames = 0L;

			/*	by sooree : 실제 게임에서는 필요 없음.
			// Get adapter's current mode so we can report
			// bit depth (back buffer depth may be unknown)
			D3DDISPLAYMODE mode;
			m_pD3D->GetAdapterDisplayMode(m_dwAdapter, &mode);

			_stprintf( m_strFrameStats, _T("%.02f fps (%dx%dx%d)"), m_fFPS,
					   m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height,
					   mode.Format==D3DFMT_X8R8G8B8?32:16 );
			if( m_bUseDepthBuffer )
			{
				D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
				D3DDeviceInfo*  pDeviceInfo  = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
				D3DModeInfo*    pModeInfo    = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];
			}
			*/
		}
	}
	//	ProfileEnd( "Render3DEnvironment0913" );
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
VOID CD3DApplication::Pause(BOOL bPause)
{
	static DWORD dwAppPausedCount = 0L;

	dwAppPausedCount += (bPause ? +1 : -1);
	m_bReady = (dwAppPausedCount ? FALSE : TRUE);

	// Handle the first pause request (of many, nestable pause requests)
	if (bPause && (1 == dwAppPausedCount))
	{
		// Stop the scene from animating
		if (m_bFrameMoving)
			DXUtil_Timer(TIMER_STOP);
	}

	if (0 == dwAppPausedCount)
	{
		// Restart the timers
		if (m_bFrameMoving)
			DXUtil_Timer(TIMER_START);
	}
}




//-----------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
VOID CD3DApplication::Cleanup3DEnvironment()
{
	m_bActive = FALSE;
	m_bReady = FALSE;

	if (m_pd3dDevice)
	{
		InvalidateDeviceObjects();
		DeleteDeviceObjects();

		m_pd3dDevice->Release();
		if (m_pD3D)
			m_pD3D->Release();

		m_pd3dDevice = NULL;
		m_pD3D = NULL;
	}

	FinalCleanup();

#ifndef INDONESIA_VERSION
	if (g_UserMode.dmBitsPerPel != 32 || (m_bFullScreen && m_EWindowsMode == MODE_WINDOW_FULLMODE))
	{
		ChangeDisplaySettings(&g_UserMode, 0);
	}
#endif

	return;
}




//-----------------------------------------------------------------------------
// Name: DisplayErrorMsg()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::DisplayErrorMsg(HRESULT hr, DWORD dwType, TCHAR *strExtraInfo)
{
	TCHAR strMsg[512];

	switch (hr)
	{
	case D3DAPPERR_NODIRECT3D:
		strcpy(strMsg, _T("Could not initialize Direct3D. You may\n")
			_T("want to check that the latest version of\n")
			_T("DirectX is correctly installed on your\n")
			_T("system.  Also make sure that this program\n")
			_T("was compiled with header files that match\n")
			_T("the installed DirectX DLLs."));
		break;

	case D3DAPPERR_NOCOMPATIBLEDEVICES:
		strcpy(strMsg, _T("Could not find any compatible Direct3D\n")
			_T("devices."));
		break;

	case D3DAPPERR_NOWINDOWABLEDEVICES:
		strcpy(strMsg, _T("This Program cannot run in a desktop\n")
			_T("window with the current display settings.\n")
			_T("Please change your desktop settings to a\n")
			_T("16- or 32-bit display mode and re-run this\n")
			_T("Program."));
		break;

	case D3DAPPERR_NOHARDWAREDEVICE:
		strcpy(strMsg, _T("No hardware-accelerated Direct3D devices\n")
			_T("were found."));
		break;

	case D3DAPPERR_HALNOTCOMPATIBLE:
		strcpy(strMsg, _T("This Program requires functionality that is\n")
			_T("not available on your Direct3D hardware\n")
			_T("accelerator."));
		break;

	case D3DAPPERR_NOWINDOWEDHAL:
		strcpy(strMsg, _T("Your Direct3D hardware accelerator cannot\n")
			_T("render into a window.\n")
			_T("Press F2 while the app is running to see a\n")
			_T("list of available devices and modes."));
		break;

	case D3DAPPERR_NODESKTOPHAL:
		strcpy(strMsg, _T("Your Direct3D hardware accelerator cannot\n")
			_T("render into a window with the current\n")
			_T("desktop display settings.\n")
			_T("Press F2 while the app is running to see a\n")
			_T("list of available devices and modes."));
		break;

	case D3DAPPERR_NOHALTHISMODE:
		strcpy(strMsg, _T("This Program requires functionality that is\n")
			_T("not available on your Direct3D hardware\n")
			_T("accelerator with the current desktop display\n")
			_T("settings.\n")
			_T("Press F2 while the app is running to see a\n")
			_T("list of available devices and modes."));
		break;

	case D3DAPPERR_MEDIANOTFOUND:
		strcpy(strMsg, _T("Could not load required media."));
		break;

	case D3DAPPERR_RESIZEFAILED:
		strcpy(strMsg, _T("Could not reset the Direct3D device."));
		break;

	case D3DAPPERR_NONZEROREFCOUNT:
		strcpy(strMsg, _T("A D3D object has a non-zero reference\n")
			_T("count (meaning things were not properly\n")
			_T("cleaned up)."));
		break;

	case E_OUTOFMEMORY:
		strcpy(strMsg, _T("Not enough memory."));
		break;

	case D3DERR_OUTOFVIDEOMEMORY:
		strcpy(strMsg, _T("Not enough video memory."));
		break;

	default:
		strcpy(strMsg, _T("Generic application error. Enable\n")
			_T("debug output for detailed information."));
	}

	if (strcmp(strExtraInfo, "") != 0)
	{
		_tcscat(strMsg, _T("\n"));
		_tcscat(strMsg, strExtraInfo);
	}

	if (MSGERR_APPMUSTEXIT == dwType)
	{
		_tcscat(strMsg, _T("\n\nThis Program will now exit."));

		MessageBox(NULL, strMsg, m_strWindowTitle, MB_ICONERROR | MB_OK);

		// Close the window, which shuts down the app
		if (m_hWnd)
			ClientClose(m_hWnd, 1);
	}
	else
	{
		if (MSGWARN_SWITCHEDTOREF == dwType)
			_tcscat(strMsg, _T("\n\nSwitching to the reference rasterizer,\n")
				_T("a software device that implements the entire\n")
				_T("Direct3D feature set, but runs very slowly."));
		MessageBox(NULL, strMsg, m_strWindowTitle, MB_ICONWARNING | MB_OK);
	}

	return hr;
}

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
//-----------------------------------------------------------------------------
// Name: HandlePossibleSizeChange()
// Desc: Reset the device if the client area size has changed.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::HandlePossibleSizeChange()
{
	HRESULT hr = S_OK;
	RECT rcClientOld;
	rcClientOld = m_rcWindowClient;

	// Update window properties
	GetWindowRect(m_hWnd, &m_rcWindowBounds);
	GetClientRect(m_hWnd, &m_rcWindowClient);

	if (rcClientOld.right - rcClientOld.left !=
		m_rcWindowClient.right - m_rcWindowClient.left ||
		rcClientOld.bottom - rcClientOld.top !=
		m_rcWindowClient.bottom - m_rcWindowClient.top)
	{
		// A new window size will require a new backbuffer
		// size, so the 3D structures must be changed accordingly.
		Pause(true);

		m_d3dpp.BackBufferWidth = m_rcWindowClient.right - m_rcWindowClient.left;
		m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

		if (m_pd3dDevice != NULL)
		{
			// Reset the 3D environment
			//if( FAILED( hr = Reset3DEnvironment() ) )
			if (FAILED(hr = Resize3DEnvironment()))
			{
				if (hr != D3DERR_OUTOFVIDEOMEMORY)
					hr = D3DERR_DEVICENOTRESET;
				//hr = D3DAPPERR_RESETFAILED;
				DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT);
			}
		}
		Pause(false);
	}
	return hr;
}

#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
