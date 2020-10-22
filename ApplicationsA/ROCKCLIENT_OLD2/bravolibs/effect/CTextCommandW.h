//..................................................................................................................
//
//	CTextCommandW.h																			By wxywxy
//
//..................................................................................................................

#pragma warning( disable : 4786 )
#pragma warning( disable : 4800 )

#ifndef __CTEXTCOMMANDW_H
#define __CTEXTCOMMANDW_H

#include "VFileSystem.h"

#include <vector>
#include <string>


using namespace std;

class CTextCommandW
{
public:
	 
	 CTextCommandW();
	~CTextCommandW();

private:
	
	 WCHAR	m_String[1024];
	 FILE  *m_fp;
	 VFileHandle* m_pFH;
	 CVFileSystem * m_pFileSystem;

public:

	FILE *Open(char *FileName);
	VFileHandle *OpenVF(char *FileName);
	WCHAR *GetLine(void);
	WCHAR *GetArg(int x);
	WCHAR *GetString(void)		{ return m_String; }
	int   Close(void);
	FILE *GetFileHandle(void)	{ return m_fp;     }
	VFileHandle *GetFileHandleVF(void)	{ return m_pFH;     }
	
	
	int   MakeArgList(void);

	void  ExceptString(void);
	bool  EqualArgToString(int x,WCHAR *pStr);
	
	BOOL   ISArgEmpty(void)		{ return ! ( BOOL ) wcslen( m_String ); }

	int	  GetArgValue(int x,float &Value);
	int	  GetArgValue(int x,DWORD &Value);
	int	  GetArgValue(int x,int   &Value);
	int	  GetArgValue(int x,BYTE  &Value);
	int	  GetArgValue(int x,WORD  &Value);

	int	  GetArgSize(void)			{ return m_ArgList.size(); }
	
	VFileHandle *OpenCharVF(char *FileName);
	int   CloseChar(void);

	VFileHandle *OpenVF(CVFileSystem * pFileSystem, char *FileName);
	int   Close(CVFileSystem * pFileSystem);

private:
	
	vector <wstring>	m_ArgList;
};

#endif