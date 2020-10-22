#ifndef __CHATMAINPROC_H__
#define __CHATMAINPROC_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"

#include "Define.h"
#include "Fontman.h"
#include "Render.h"

#include "TextOutputBase.h"
#include "ChatPartWndList.h"

class CChatMainProc
{
protected:
	UINT					m_OutState;
	SChatTxtData			m_TempChatData;
	SChatTxtData			m_ChatTxtData[MAX_CHATDATA_NUM + 1];
	int						m_nInsertIndex;
	//실제 채팅창에 존재하는 데이터들
	
	SChatTxtData*			m_pViewChatTxtData[MAX_CHATDATA_NUM + 1];
	//존재하는 데이터중에서-실제보여줘야하는 데이터들	

	TCHAR					m_strTempBuf1[512];
	TCHAR					m_strTempBuf2[512];
	TCHAR					m_strTempName[512];

	SStringInfo				m_StrInfo;

public:	
	//-----------------------------------------------------------------------------
	//	COutState_Act 일때 나오는 윈도우
	//-----------------------------------------------------------------------------
	CChatActPrintWnd		m_ChatActPrintWnd;		//채팅대화 활성화 출력창
	//-----------------------------------------------------------------------------
	//	COutState_NonAct 일때 나오는 윈도우
	//-----------------------------------------------------------------------------
	CChatNonActPrintWnd		m_ChatNonActPrintWnd;	//채팅대화 비활성화 출력창
	
	CChatMainProc();
	virtual ~CChatMainProc();
	void   Composition();					//파트 구성요소 만들기	
	void   Update();
	void   Init();		
	
	void   UpdateChatTxtData();

	UINT   GetOutState();
	void   SetOutState(UINT uState);

	void   InsertChatContent(UINT uChatState, void * pStrName, 
							void *pStrContent, bool bFlag = true );
	void   InsertChatData();
	void   UpdateViewChatTxtData();	
	
	SChatTxtData ** GetViewChatTxtDataList()	{ return m_pViewChatTxtData; }
	
	void LogOff();
	void ReRPosition(int Rx, int Ry);
};

extern CChatMainProc g_ChatMainProc;

//-----------------------------------------------------------------------------
#endif	__CHATMAINPROC_H__