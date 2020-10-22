#ifndef __FLASHWND_H__
#define __FLASHWND_H__
//-----------------------------------------------------------------------------

#include "Wnd.h"

class CFlashWnd : public CWnd
{
public:
	bool		m_bSwitchFlag;

protected:
	bool		m_bIsActiveFlash;

	float		m_fViewIntTime;
	float		m_fNoViewIntTime;
	float		m_fCumulativeTime;			//누적 타임

	UINT		m_FlashColorOp;				//점멸적용 칼라옵션
	D3DCOLOR	m_FlashColor;				//점멸적용 칼라값
	DWORD		m_SrcBlend;					//소스   블랜드값
	DWORD		m_DestBlend;				//데스트 블랜드값

public:	
	CFlashWnd() {}
	CFlashWnd( SDesktop* DT );	 
	virtual ~CFlashWnd();

	virtual void RenderProc();

	void SetFlashColorOption(UINT ColorOp);
	void SetFlashBlendOption(DWORD SrcBlend, DWORD DestBlend);

public:
	 void SetActiveFlash(bool bflag);
	 void SetIntervalTime(float fTime);
};


#endif	__FLASHWND_H__