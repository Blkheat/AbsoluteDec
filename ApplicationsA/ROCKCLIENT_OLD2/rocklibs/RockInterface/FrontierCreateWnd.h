#ifndef __FRONTIERCREATEWND_H__
#define __FRONTIERCREATEWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"

#ifdef DECO_RENEWAL_GUILD_MJH
#include "FrontierMainWndProc.h"

#define CREATE_MEMBER_MAX_COUNT				10

#endif // #ifdef DECO_RENEWAL_GUILD_MJH

class CFrontierCreateWnd : public CWndProc
{
protected:
	CFrameWnd*		m_pFrameWnd;
	
	CEditBox*		m_pPNameEdit;			///-- 프론티어 이름
	CEditBox*		m_pPCMemeberEdit[3];	///-- 창단 멤버 3명
	
	CButton*		m_pCloseBtn;			///-- 닫기 버튼
	CButton*		m_pConfirmBtn;			///-- 확인 버튼
	CButton*		m_pCancleBtn;			///-- 취소 버튼

public:
	CFrontierCreateWnd();
	~CFrontierCreateWnd();

	virtual void Init();
	virtual void Composition();	
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	
	BOOL ConfirmInputContent();
	void Open();
	void Close();
};
#ifdef DECO_RENEWAL_GUILD_MJH

class CFrontierCreateScrollWnd : public CWndProc
{
protected:
	DWORD m_CurTime;
	DWORD m_NowTime;
	DWORD m_Cumulative;
	DWORD m_MilliSec;
	

	CFrameWnd*		m_pFrameWnd;
	CTextBox*	    m_pNamebox[10];         ///-- 길드 인원 텍스트
	CButton*		m_pCloseBtn;			///-- 닫기 버튼
	CTextBox*	    m_pCreateTime;          ///-- 창단 제한 시간

	
	
public:
	CFrontierCreateScrollWnd();
	~CFrontierCreateScrollWnd();

	virtual void Init();
	virtual void Composition();	
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:

	time_t          m_FrontierLimitTime; //길드 생성제한시간 

	void Open();
	void Close();
	void FrontierMemberInfoList();
	void FrontierLimitTime();
	int  GetSec();

};

extern CFrontierCreateScrollWnd g_FrontierCreateScrollWnd;

#endif // DECO_RENEWAL_GUILD_MJH

extern CFrontierCreateWnd g_FrontierCreateWnd;

//-----------------------------------------------------------------------------
#endif	__FRONTIERCREATEWND_H__