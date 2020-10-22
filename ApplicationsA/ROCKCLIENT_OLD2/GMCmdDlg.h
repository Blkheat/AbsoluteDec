// GMCmdDlg.h: interface for the CGMCmdDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__GMCMDDLG_H_INCLUDED__)
#define __GMCMDDLG_H_INCLUDED__

#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/// 운영자용 클라이언트 컴파일 옵션 
#ifdef _DEBUG
	#define	GM_CLIENT 
#endif



struct	SCmdInfo
{
	char  	theCmdStr[64];	
	DWORD	theCmdCode;
	int		theNParamNum;
	int		theSParamNum;
};

class CGMCmdDlg  
{
public:

	CGMCmdDlg();
	virtual ~CGMCmdDlg();

	void	CreateDlg( HINSTANCE hParentInst, HWND hParentWnd );
	
	void	DestroyDlg();
	void	ActiveDlg();
	LRESULT DlgMsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	BOOL	CommandSend( char* aCmd, char* aCmdParam );
	void	OnEditEnterKey();
	void	OnEditUPKey();

	void	OnScGMCmd( DWORD aCommand, int* aNParamList, char* aSParam );
	void	OnScGMCmdErr( DWORD aCommand, int aErrorCode );

	inline	HWND GetEditCmdHandle()	{	return theEditCmd;		}
	
	//by simwoosung
    BOOL    GetNonPlayerInfo(char *pSParam);
	int		GetPickNonPlayerID();
	//선택 대상으로부터 정보 얻어오는 함수
	void    GetCitizen(int iCitizen , char * strCitizen);
	void    GetClass(int iClass , char * strClass);
	void    GetConnect(int iConnect ,char * strConnect);
	bool	ViewHelp( TCHAR *acp_Str );
	void	ReadHelpFromFile( int ai_code );

private:
	bool		_Create_GM_data_VF();
	bool		_Create_GM_data();

	HWND		theMainDlgHandle;
	HWND		theParentWndHandle;
	HINSTANCE	theParentInst;
	HBRUSH		theMainDlgBgBrush;
	RECT		theMainDlgRect;
	HFONT		theFont;

	HWND		theEditCmd;
	HWND		theEditHidden;
	HWND		theListLog;

	char		theBufStrCmdOnly[32];
	char		theBufStrCmdParam[96];
	SCmdInfo*	theBufCmdInfo;
	int			theBufMode;
	
	HWND		m_hLogSaveBtn;					//로그 파일저장 버튼
};

#endif 
