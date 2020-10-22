#ifndef __MESSENNOTIFYWND_H__
#define __MESSENNOTIFYWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "FlashWnd.h"
#include "LimitLenTextBox.h"
#include "FrameTextBox.h"

enum NotifyWndState
{
	NState_None = 0,
	NState_UpAction,
	NState_DownAction,
};

class CMessenNotifyWnd : public CWndProc
{
protected:
	TCHAR 						m_strInterDestName[512];
	int							m_nIndex;

	CFrameWnd*					m_pFrameWnd;
	CFlashWnd*					m_pNotifyFlashImg;
	CButton*					m_pCloseBtn;
	CLimitLenTextBox*			m_pDestNameTxt;
	CButton*					m_pMouseOverBtn;
	
	CFrameTextBox*				m_FocusOriginTxt;	

	UINT						m_uFrameID;
	UINT						m_uFlashImgID;
	UINT						m_uCloseBtnID;
	UINT						m_uDestNameTxtID;
	UINT						m_uMouseOverBtnID;

	UINT						m_uFocusOriginTxtID;

	TCHAR						m_strTemp[MAX_SIZE_512];

	UINT						m_WndState;
	
	int							m_SAlpha;
	int							m_EAlPha;
	POINT						m_Spoint;
	POINT						m_Epoint;

	float						m_fUpActionTime;
	float						m_fDownActionTime;
	float						m_fCumulativeTime;			//누적 타임
	
public:
	CMessenNotifyWnd();
	~CMessenNotifyWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	void Composition(int nIndex, int nXpos, int nYpos);
	CFrameWnd * GetFrameWnd();
	CFlashWnd * GetNotifyFlashImg();
	UINT		GetFrameID();

	void		MoveAction(UINT uState, POINT sPoint, POINT ePoint);
	void		MoveAction();

	void	SetInterDestName(void * strName);
	TCHAR * GetInterDestName();

	void  AfterActionThisWndClick();
};

//-----------------------------------------------------------------------------
#endif	__MESSENNOTIFYWND_H__