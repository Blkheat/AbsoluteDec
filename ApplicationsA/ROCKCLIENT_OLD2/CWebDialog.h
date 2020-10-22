#ifndef __CWEBDIALOG_H
#define __CWEBDIALOG_H

#include "RockClient.h"
#include "stringmanager.h"
#include "brother.h"
#include "RockClient.h"


#include "quadlist.h"
#include "bravolibs\\network\\Protocol.h"
#include "Pc.h"

#include "Windows.h"
#include "Exdisp.h"
#include "IExplore.h"

class CWebDialog
{
public:
	 
	 CWebDialog();
	~CWebDialog();

public:
	void InitWebDialog();
	bool SetDialog(int px,int py,int width,int height,HINSTANCE hInst,HWND hParent);
	int	SetURL(wchar_t *addr, TCHAR *post);
	int Show(void);
	int Hide(void);
	int Destroy(void);

	int GetX(void)			{ return m_Px;     }
	int GetY(void)			{ return m_Py;     }
	int GetWidth(void)		{ return m_Width;  }
	int GetHeight(void)		{ return m_Height; }

	HWND GethExp(void)		{ return m_hExp;   }
	
	int  PreLoad(HINSTANCE hInst,HWND hParent);

private:

	void	DeleteAllMemory(void);

private:

	HWND	m_hExp;
	HWND	m_hParent;

	int		m_Px;
	int		m_Py;
	int		m_Width;
	int		m_Height;

	D3DPRESENT_PARAMETERS m_OldD3D;

public:

	BOOL	m_Ready;
	RECT	m_Rect;
};

#endif
