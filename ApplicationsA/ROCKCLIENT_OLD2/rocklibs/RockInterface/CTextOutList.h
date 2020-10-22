#ifndef __CTEXTOUTLIST_H__
#define __CTEXTOUTLIST_H__
//-----------------------------------------------------------------------------

#include "TextOutputBase.h"
#include "Wnd.h"
#include "SlideBar.h"
#include "ScrollBar.h"
#include "RITextBox.h"

//------------------------------------------------------------------------------
// CChatOutList  채팅관련 출력 컴포넌트
//------------------------------------------------------------------------------
class CChatOutList : public CWnd
{
public:
	CRITextBox*				m_pRITextBox;
	CTextBox*				m_pTextBox;
	CTextBox*				m_pTextBox2;

private:
	UINT					m_uChatOutActState;
	UINT					m_uFilterState;

	bool					m_IsFocus;

	int						m_nLHeight;

	CWnd*					m_pLinkScrollBarBg;
	CScrollBar*				m_pLinkScrollbar;						

	SRect					m_FocusRect;
	SChatTxtData*			m_pTempFocusChatData;	
	
	SChatTxtData**			m_ppRefChatTxtData;		
	//메인프록에서 필터링된 채팅창데이터 리스트	
	SChatTxtData*			m_pViewChatTxtData[MAX_CHATDATA_NUM + 1]; 
	//실제 CChatOutList에서 걸러진 두번째 채팅창데이터 리스트 
	int						m_nOutPutLine;
	int						m_nLineCount;
	int						m_nDelValue;	//nLineCount와 출력시작 차이값
		
public:	
	CChatOutList() {}
	CChatOutList( SDesktop* DT );	 
	virtual ~CChatOutList();

	virtual SEventMessage* EventProc( SEventMessage* EMsg );
	
	virtual WNDID FocusCheck( int Mx, int My, bool IsChildCheck );
	virtual void RenderProc();

	void SetChatOutActState(UINT uState);
	void SetFilterState(UINT uState);	
	
	void SetRefpViewChatTxtData(SChatTxtData	**ppRefChatTxtData);
	
	void SetOutputLevel(UINT uState);
	void ResetSize();
	void InitChatOutList();

	SChatTxtData * GetFocusChatData()	{ return m_pTempFocusChatData; }

	void CalcRenderOutBound();
	void GetStringInfo( TCHAR *strTemp , SStringInfo * StrInfo );

	void UpdateViewChatTxtData();

	void SetDelValue(int nValue);	
	int  GetOutPutLine()	{ return m_nOutPutLine; }
	int  GetLineCount()		{ return m_nLineCount; }

	void SetLinkScroll(CScrollBar* pLinkScrollbar, CWnd * pLinkScrollBarBg = NULL);
};


class CSystemOutList : public CWnd
{
public:
	CRITextBox*				m_pRITextBox;

private:
	int						m_nLHeight;

	BOOL					m_IsAutoScroll;
	
	CWnd*					m_pLinkScrollBarBg;
	CScrollBar*				m_pLinkScrollbar;

	SSystemTxtData*			m_pRefSystemTxtData;
	//참조 시스템 메시지 배열
	SSystemTxtData*			m_pViewSystemTxtData[MAX_SYSTEMDATA_NUM + 1];
	//실제보여지는 시스템 메세지

	int						m_nOutPutLine;
	int						m_nLineCount;
	int						m_nDelValue;	//nLineCount와 출력시작 차이값

public:	
	CSystemOutList() {}
	CSystemOutList( SDesktop* DT );	 
	virtual ~CSystemOutList();

	virtual void RenderProc();

	void CalcRenderOutBound();
	void SetRefpViewSystemTxtData(SSystemTxtData	*pRefSystemTxtData);

	void SetOutputLevel(UINT uState);
	void ResetSize();
	void InitSystemOutList();
	void UpdateViewSystemTxtData();
	
	void SetDelValue(int nValue);
	int  GetOutPutLine()	{ return m_nOutPutLine; }
	int  GetLineCount()		{ return m_nLineCount; }

	void GetStringInfo( TCHAR *strTemp , SStringInfo * StrInfo );

	void SetLinkScroll(CScrollBar* pLinkScrollbar, CWnd * pLinkScrollBarBg = NULL);

	void SetAutoScroll(BOOL bFlag) { m_IsAutoScroll = bFlag; }
};

//-----------------------------------------------------------------------------
#endif	__CTEXTOUTLIST_H__