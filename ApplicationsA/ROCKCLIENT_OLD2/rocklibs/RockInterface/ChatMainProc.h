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
	//���� ä��â�� �����ϴ� �����͵�
	
	SChatTxtData*			m_pViewChatTxtData[MAX_CHATDATA_NUM + 1];
	//�����ϴ� �������߿���-������������ϴ� �����͵�	

	TCHAR					m_strTempBuf1[512];
	TCHAR					m_strTempBuf2[512];
	TCHAR					m_strTempName[512];

	SStringInfo				m_StrInfo;

public:	
	//-----------------------------------------------------------------------------
	//	COutState_Act �϶� ������ ������
	//-----------------------------------------------------------------------------
	CChatActPrintWnd		m_ChatActPrintWnd;		//ä�ô�ȭ Ȱ��ȭ ���â
	//-----------------------------------------------------------------------------
	//	COutState_NonAct �϶� ������ ������
	//-----------------------------------------------------------------------------
	CChatNonActPrintWnd		m_ChatNonActPrintWnd;	//ä�ô�ȭ ��Ȱ��ȭ ���â
	
	CChatMainProc();
	virtual ~CChatMainProc();
	void   Composition();					//��Ʈ ������� �����	
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