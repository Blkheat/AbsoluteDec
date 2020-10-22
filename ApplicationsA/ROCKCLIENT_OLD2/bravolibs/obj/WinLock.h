// WinLock.h: interface for the CWinLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINLOCK_H__D3F3B88E_427F_44CF_930B_01139D83BB65__INCLUDED_)
#define AFX_WINLOCK_H__D3F3B88E_427F_44CF_930B_01139D83BB65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// from codeproject_source...
typedef struct _THREAD_DATA
{
	HDESK hDesk;
	char  szDesktopName[20];

} THREAD_DATA;



class CWinLock  
{
public:
	CWinLock();
	virtual ~CWinLock();
	
	void SetHInstance( HINSTANCE hInstance );

	int Desktop_Show_Hide( BOOL bShowHide );
	int StartButton_Show_Hide( BOOL bShowHide );
	int Taskbar_Show_Hide( BOOL bShowHide );
	int Clock_Show_Hide( BOOL bShowHide );
	int Keys_Enable_Disable( BOOL bEnableDisable );
	int AltTab1_Enable_Disable( BOOL bEnableDisable );
	int AltTab2_Enable_Disable( HWND hWnd, BOOL bEnableDisable );
	int TaskSwitching_Enable_Disable( BOOL bEnableDisable );
	int TaskManager_Enable_Disable( BOOL bEnableDisable );
	int CtrlAltDel_Enable_Disable( BOOL bEnableDisable );
	int Thread_Desktop( LPTHREAD_START_ROUTINE ThreadFunc, THREAD_DATA *td );
	int Process_Desktop( char *szDesktopName, char *szPath );
	
};

#endif // !defined(AFX_WINLOCK_H__D3F3B88E_427F_44CF_930B_01139D83BB65__INCLUDED_)




