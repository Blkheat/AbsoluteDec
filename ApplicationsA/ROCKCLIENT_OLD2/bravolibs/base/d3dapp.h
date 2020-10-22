//-----------------------------------------------------------------------------
// File: D3DApp.h
// Desc: Application class for the Direct3D samples framework library.
// Copyright (c) 1998-2000 Microsoft Corporation. All rights reserved.
// 참고 서적 : Beginning Direct3D Game Programming(민프레스) - Wolfgang F.Engel
// 프레임 워크란 일종의 프로그램의 흐름과 골격을 형성하는 클래스를 말한다. 
// 즉, 프레임 워크를 제공하는 클래스는 프로그램의 전체적인 구조와 흐름을 확정 지으며,
// 프로그래머가 정의해야할 함수의 이름까지도 확정짓는다. 이와같은 프로그래밍 방식은 C++ 기능을 충분히 살리면서,
// 많은 효율성과 편의성을 제공하고 있다. 프로그래머가 프레임워크를 이용하면, 이미 안정화와 통일된 구조를
// 사용하게 되며, 전체적인 흐름에 신경쓰지 않고, 프레임워크에서 제공을 해주는 인터페이스(공통 함수)를 이용하여서
// 부분적인 프로그래밍을 하게 된다. 이로써 프로그래머는 설계에 대한 부담을 최소화 할수 있어서,
// 기능적인 부분에 더욱 신경을 써서 프로그래밍을 하게 된다. DirectX SDK에서는 CD3DApplication이라는 클래스가
// 이에 해당 되며, 3D의 기본적인 Direct3D의 셋팅과 종료 부분 및 갱신 부분을 이미 클래스의 인터페이스로써
// 제공하고 있다. 이 부분은 C++의 가상함수라는 개념이 도입되어 있으며, C++의 이와 같은 기능을 이용하여
// 재정의(overriding)하여 사용하게 된다.
//-----------------------------------------------------------------------------
#ifndef D3DAPP_H
#define D3DAPP_H

// basetsd.h defines INT_PTR (used below).  It is not included by default
// under VC 5.0.  If you are using VC6 or later, it is included via Windows.h.
#include <basetsd.h> 

#pragma	comment	( lib, "ddraw.lib" )
#pragma	comment	( lib, "d3dxof.lib" )
#ifdef DIRECT_VERSION_9_MJH
#pragma	comment	( lib, "d3dx9.lib" )
#pragma	comment	( lib, "d3d9.lib" ) 
#else
#pragma	comment	( lib, "d3dx8.lib" )
#pragma	comment	( lib, "d3d8.lib" ) 
#endif // DIRECT_VERSION_9_MJH
#pragma	comment	( lib, "winmm.lib" ) 
#pragma	comment	( lib, "dxguid.lib" )


#define		_DELETE(p)				{ if ( p )	{ delete	(p);		(p) = NULL; } }
#define		_DELETE_ARRAY(p)		{ if ( p )	{ delete[]	(p);		(p)	= NULL; } }
#define		_RELEASE(p)				{ if ( p )	{ (p)->Release();		(p)	= NULL; } }


//-----------------------------------------------------------------------------
// Error codes
//-----------------------------------------------------------------------------
enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESIZEFAILED        0x8200000c


//-----------------------------------------------------------------------------
// Name: struct PCSystemInfo
// Desc: Structure for holding Player Computer System Information
//-----------------------------------------------------------------------------
#pragma pack(1)
struct PCSystemInfo
{
	char			CardName[128];
	LARGE_INTEGER	DriverVersion;
	DWORD			OSVersion;
	DWORD			DXVersion;
	DWORD			VidioMemory;
	DWORD			SystemMemory;
};
#pragma pack()
extern PCSystemInfo		g_PcSystemInfo;


//-----------------------------------------------------------------------------
// Name: struct D3DModeInfo
// Desc: Structure for holding information about a display mode
//-----------------------------------------------------------------------------
struct D3DModeInfo
{
    DWORD      Width;      // Screen width in this mode
    DWORD      Height;     // Screen height in this mode
    D3DFORMAT  Format;     // Pixel format in this mode
    DWORD      dwBehavior; // Hardware / Software / Mixed vertex processing
    D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
};




//-----------------------------------------------------------------------------
// Name: struct D3DDeviceInfo
// Desc: Structure for holding information about a Direct3D device, including
//       a list of modes compatible with this device
//-----------------------------------------------------------------------------
struct D3DDeviceInfo
{
    // Device data
    D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
#ifdef DIRECT_VERSION_9_MJH
	D3DCAPS9     d3dCaps;         // Capabilities of this device
#else
	D3DCAPS8     d3dCaps;         // Capabilities of this device
#endif // DIRECT_VERSION_9_MJH
    const TCHAR* strDesc;         // Name of this device
    BOOL         bCanDoWindowed;  // Whether this device can work in windowed mode

    // Modes for this device
    DWORD        dwNumModes;
    D3DModeInfo  modes[150];

    // Current state
    DWORD        dwCurrentMode;
    BOOL         bWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleType;
};




//-----------------------------------------------------------------------------
// Name: struct D3DAdapterInfo
// Desc: Structure for holding information about an adapter, including a list
//       of devices available on this adapter
//-----------------------------------------------------------------------------
struct D3DAdapterInfo
{
    // Adapter data
#ifdef DIRECT_VERSION_9_MJH
	D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;
#else
	D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
#endif // DIRECT_VERSION_9_MJH
    D3DDISPLAYMODE d3ddmDesktop;      // Desktop display mode for this adapter

    // Devices for this adapter
    DWORD          dwNumDevices;
    D3DDeviceInfo  devices[5];

    // Current state
    DWORD          dwCurrentDevice;
};




//-----------------------------------------------------------------------------
// Name: class CD3DApplication
// Desc: A base class for creating sample D3D8 applications. To create a simple
//       Direct3D application, simply derive this class into a class (such as
//       class CMyD3DApplication) and override the following functions, as 
//       needed:
//          OneTimeSceneInit()    - To initialize app data (alloc mem, etc.)
//          InitDeviceObjects()   - To initialize the 3D scene objects
//          FrameMove()           - To animate the scene
//          Render()              - To render the scene
//          DeleteDeviceObjects() - To cleanup the 3D scene objects
//          FinalCleanup()        - To cleanup app data (for exitting the app)
//          MsgProc()             - To handle Windows messages
//-----------------------------------------------------------------------------
class CD3DApplication
{
protected:
    // Internal variables for the state of the app
    D3DAdapterInfo    m_Adapters[10];// 어댑터 정보
    DWORD             m_dwNumAdapters;// 사용 가능한 어댑터 개수
    DWORD             m_dwAdapter;// 선택 어댑터를 나타내는 인덱스. BuildDeviceList()에서 셋팅
    BOOL              m_bWindowed;
    BOOL              m_bActive;
	BOOL			  m_bFocus;
    BOOL              m_bReady;		

    // Internal variables used for timing
    BOOL              m_bFrameMoving;
    BOOL              m_bSingleStep;
   
    // Internal functions to manage and render the 3D scene
    HRESULT BuildDeviceList();// 적합한 디바이스 드라이버를 찾아낸다.
    BOOL    FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
                D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );
    HRESULT Initialize3DEnvironment();
    HRESULT Resize3DEnvironment();   
    HRESULT ForceWindowed();
    HRESULT UserSelectNewDevice();
    VOID    Cleanup3DEnvironment();
    HRESULT Render3DEnvironment();// 게임 내의 모든 이동과 3D 렌더링 
    virtual HRESULT AdjustWindowForChange();// 윈도우모드와 전체화면 모드의 윈도우스타일 설정
    static INT_PTR CALLBACK SelectDeviceProc( HWND hDlg, UINT msg, 
                WPARAM wParam, LPARAM lParam );

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	HRESULT HandlePossibleSizeChange();
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)


public:

	HINSTANCE		m_hInstance;
    HWND            m_hWnd;              // The main app window
	HWND            m_hWindowWnd;        // The window 
	HWND            m_hFullmodeWnd;        // The window 

    D3DPRESENT_PARAMETERS m_d3dpp;         // Parameters for CreateDevice/Reset
	WORD			m_wPixelFormat;	   // PixelFormat used to create window
	D3DSURFACE_DESC   m_d3dsdBackBuffer;   // Surface desc of the backbuffer

	BOOL			m_bDeviceLost;
	BOOL			m_bFullScreen;

    HRESULT ToggleFullscreen( bool bToggle = 1 );
	DWORD GetWindowModeStyle();
protected:


    // Main objects used for creating and rendering the 3D scene
    HWND              m_hWndFocus;         // The D3D focus window (usually same as m_hWnd)
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3D9       m_pD3D;              // The main D3D object
    LPDIRECT3DDEVICE9 m_pd3dDevice;        // The D3D rendering device
    D3DCAPS9          m_d3dCaps;           // Caps for the device
#else
	LPDIRECT3D8       m_pD3D;              // The main D3D object
    LPDIRECT3DDEVICE8 m_pd3dDevice;        // The D3D rendering device
    D3DCAPS8          m_d3dCaps;           // Caps for the device
#endif // DIRECT_VERSION_9_MJH
    DWORD             m_dwCreateFlags;     // Indicate sw or hw vertex processing
    DWORD             m_dwWindowStyle;     // Saved window style for mode switches
    RECT              m_rcWindowBounds;    // Saved window bounds for mode switches
    RECT              m_rcWindowClient;    // Saved client area size for mode switches

    // Variables for timing
    FLOAT             m_fTime;             // Current time in seconds
    FLOAT             m_fElapsedTime;      // Time elapsed since last frame
    FLOAT             m_fFPS;              // Instanteous frame rate
    TCHAR             m_strDeviceStats[90];// String to hold D3D device stats
    TCHAR             m_strFrameStats[40]; // String to hold frame stats

    // Overridable variables for the app
	UINT			  m_EWindowsMode;
    TCHAR*            m_strWindowTitle;    // Title for the app's window
    BOOL              m_bUseDepthBuffer;   // Whether to autocreate depthbuffer
    DWORD             m_dwMinDepthBits;    // Minimum number of bits needed in depth buffer
    DWORD             m_dwMinStencilBits;  // Minimum number of bits needed in stencil buffer
    DWORD             m_dwCreationWidth;   // Width used to create window
    DWORD             m_dwCreationHeight;  // Height used to create window	
    BOOL              m_bShowCursorWhenFullscreen; // Whether to show cursor when fullscreen
	
    // Overridable functions for the 3D scene created by the app
#ifdef DIRECT_VERSION_9_MJH
    virtual HRESULT ConfirmDevice(D3DCAPS9*,DWORD,D3DFORMAT)   { return S_OK; }
#else
	virtual HRESULT ConfirmDevice(D3DCAPS8*,DWORD,D3DFORMAT)   { return S_OK; }
#endif // DIRECT_VERSION_9_MJH
    virtual HRESULT OneTimeSceneInit()                         { return S_OK; }
    virtual HRESULT InitDeviceObjects()                        { return S_OK; }
	virtual HRESULT InitState()					               { return S_OK; }

	// 사용자의 월드, 프로젝션, 뷰, 텍스쳐를 초기화 
    virtual HRESULT RestoreDeviceObjects()                     { return S_OK; }

	// 모든 장면의 사물들의 위치값, 회전값 변경을 담당 
    virtual HRESULT FrameMove()                                { return S_OK; }
	virtual HRESULT Render()                                   { return S_OK; }

	// 해제 함수들
    virtual HRESULT InvalidateDeviceObjects()                  { return S_OK; }
    virtual HRESULT DeleteDeviceObjects()                      { return S_OK; }
    virtual HRESULT FinalCleanup()                             { return S_OK; }

public:
    // Functions to create, run, pause, and clean up the application
    virtual HRESULT Create( HINSTANCE hInstance );// 내부에서 주요 함수인 CreateWindow()를 호출하여 윈도우 생성
    virtual INT     Run();// 윈도우 메시지 처리와 3D 렌더링 함수 호출 하는 역할 

	// this 포인터에 정적(static)으로 링크된 함수
    virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual VOID    Pause( BOOL bPause );// 렌더링 메소드의 호출을 멈추는 역할

    // Internal constructor
    CD3DApplication();
	virtual ~CD3DApplication();

	// Internal error handling function
    HRESULT DisplayErrorMsg( HRESULT hr, DWORD dwType, TCHAR *strExtraInfo = NULL );

	BOOL IsWindowed()	{ return m_bWindowed; }
};




#endif



