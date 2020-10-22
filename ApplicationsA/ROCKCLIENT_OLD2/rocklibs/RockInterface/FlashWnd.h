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
	float		m_fCumulativeTime;			//���� Ÿ��

	UINT		m_FlashColorOp;				//�������� Į��ɼ�
	D3DCOLOR	m_FlashColor;				//�������� Į��
	DWORD		m_SrcBlend;					//�ҽ�   ���尪
	DWORD		m_DestBlend;				//����Ʈ ���尪

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