#include "RockPCH.h"
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <TCHAR.h>
#include "CWebDialog.h"
#include "CRenderManager.h"


CWebDialog::CWebDialog()
{
	m_Ready = FALSE;
	DeleteAllMemory();
}

CWebDialog::~CWebDialog()
{
/*
	//Destroy();
	if( NULL != m_hExp && m_Ready )
	{
		SendMessage( m_hExp , WM_CLOSE , NULL , NULL );

		m_hExp = NULL;
	}
*/
	m_Ready = FALSE;

}

void CWebDialog::DeleteAllMemory(void)
{
	m_hExp   = NULL;

	m_Px	 = 0;	
	m_Py     = 0;
	m_Width  = 0;
	m_Height = 0;
}

void CWebDialog::InitWebDialog()
{

}


bool CWebDialog::SetDialog(int px,int py,int width,int height, HINSTANCE hInst,HWND hParent)
{
	m_hParent = hParent;
	
	WNDCLASSEXW		wc;
	WCHAR ClassName[] = L"Browser Example";
	
	// Register the class of our window to host the browser. 'WindowProc' is our message handler
	// and 'ClassName' is the class name. You can choose any class name you want.
	ZeroMemory(&wc, sizeof(WNDCLASSEXW));
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.hInstance = hInst;
	wc.lpfnWndProc = WebWindowProc;
	wc.lpszClassName = &ClassName[0];
	RegisterClassExW(&wc);





	if (m_hExp = CreateWindowExW( 0, &ClassName[0], L"Web", WS_CHILD ,px, py , width , height , hParent ,
		NULL, hInst, 0))
	{
		ShowWindow( m_hExp , SW_HIDE );
		UpdateWindow( m_hExp );
		::SetForegroundWindow( m_hExp ); 
		MoveWindow( m_hExp , 0 , 0 , 0 , 0 , FALSE );
		
		m_Px	 = px;
		m_Py     = py;
		m_Width  = width;
		m_Height = height;
		
		m_Rect.left   = px;
		m_Rect.top    = py;
		m_Rect.right  = px + width;
		m_Rect.bottom = py + height;
		
		
		return true;
	}
	return false; 

}

int	CWebDialog::SetURL(wchar_t *addr, TCHAR *post)
{
	m_Ready = FALSE;

	if( m_hExp != NULL )
	{

		if( 0 == DisplayHTMLPage( m_hExp , addr , post ) )
		{
			m_Ready = TRUE;
		}
	}
	
	return m_Ready;
}

int CWebDialog::Show(void)
{
	if( m_Ready )
	{
		MoveWindow( m_hExp , m_Px , m_Py , m_Width , m_Height , FALSE );
		ShowWindow( m_hExp , SW_SHOWNORMAL );
	}


	//...........................................................................................................
    // 모드 변환 
	//...........................................................................................................
	g_RockClient.InvalidateDeviceObjects();

	m_OldD3D = g_RockClient.m_d3dpp;

	g_RockClient.m_d3dpp.SwapEffect						 = D3DSWAPEFFECT_COPY;
#ifdef DIRECT_VERSION_9_MJH
	g_RockClient.m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#else
	g_RockClient.m_d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif // DIRECT_VERSION_9_MJH
	

	HRESULT hr = g_RenderManager.Reset( &g_RockClient.m_d3dpp );

    if( FAILED( hr ) )
	{
		hr = g_RenderManager.TestCooperativeLevel();

	   switch( hr )
	   {
			case D3DERR_DEVICELOST:				
			break;

			case D3DERR_DEVICENOTRESET:		
			break;		
	   }

	   return false;
   }
	   
	g_RockClient.RestoreDeviceObjects();
	//...........................................................................................................


	return TRUE;
}

int CWebDialog::Hide(void)
{
	if( m_Ready )
	{
		MoveWindow( m_hExp , 0 , 0 , 0 , 0 , FALSE );
		ShowWindow( m_hExp , SW_HIDE );
	}

	return TRUE;
}

int CWebDialog::Destroy(void)
{
	
	if( NULL != m_hExp && m_Ready )
	{
		::SetFocus(m_hExp);
		::SetForegroundWindow( m_hExp );

		ClientClose(m_hExp);

	}

	m_Ready = FALSE  ;


	//...........................................................................................................
    // 모드 변환 
	//...........................................................................................................
	g_RockClient.InvalidateDeviceObjects();

	g_RockClient.m_d3dpp = m_OldD3D;

	HRESULT hr = g_RenderManager.Reset( &g_RockClient.m_d3dpp );

    if( FAILED( hr ) )
	{
	
		hr = g_RenderManager.TestCooperativeLevel();

	   switch( hr )
	   {
			case D3DERR_DEVICELOST:
		
			break;

			case D3DERR_DEVICENOTRESET:
		
			break;
	   }

	   return false;
   }

	g_RockClient.RestoreDeviceObjects();
	//...........................................................................................................

	return TRUE;
}

int CWebDialog::PreLoad(HINSTANCE hInst,HWND hParent)
{
	m_hParent = hParent;
	
	WNDCLASSEXW		wc;
	WCHAR	ClassName[] = L"Browser Example";
	
	ZeroMemory(&wc, sizeof(WNDCLASSEXW));
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.hInstance = hInst;
	wc.lpfnWndProc = WebWindowProc;
	wc.lpszClassName = &ClassName[0];
	RegisterClassExW(&wc);

	HWND hWnd;

	//if (( hWnd = CreateWindowExW( 0, &ClassName[0], L"Web", WS_CHILDWINDOW | WS_CLIPCHILDREN ,
	if (( hWnd = CreateWindowExW( 0, &ClassName[0], L"Web", WS_CHILD ,
						0, 0 , 0 , 0 , hParent ,
						NULL, hInst, 0)))
	{
		ShowWindow( hWnd , SW_HIDE );
		UpdateWindow( hWnd );

		DisplayHTMLPage( hWnd , L"about:blank" , "" );
		ClientClose( hWnd , NULL);

		return TRUE;
	}
	
	return FALSE;
}
