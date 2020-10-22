#ifndef __MSETTINGTABWND_H__
#define __MSETTINGTABWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"

class CMSettingTabWnd : public CWndProc
{
protected:
	CFrameWnd *				m_pMSettingTabWnd;

	CTextBox *				m_pMSOpenSettingTxt;
	CTextBox *				m_pMSLocalTxt;
	CCheckButton *			m_pMSLocalCheckBox;
	CTextBox *				m_pMSLevelTxt;
	CCheckButton *			m_pMSLevelCheckBox;
	CTextBox *				m_pMSJobTxt;
	CCheckButton *			m_pMSJobCheckBox;
	CTextBox *				m_pMSStateSettingTxt;
	CTextBox *				m_pMSOnlineTxt;
	CRadioButton *			m_pMSOnlineRadioBtn;
	CTextBox *				m_pMSRestTxt;
	CRadioButton *			m_pMSRestRadioBtn;
	CTextBox *				m_pMSMeatTxt;
	CRadioButton *			m_pMSMeatRadioBtn;
	CTextBox *				m_pMSOfflineTxt;
	CRadioButton *			m_pMSOfflineRadioBtn;
	CTextBox *				m_pGuildAWordTxt;
	CButton *				m_pGuildAWordInputBtn;
	CButton *				m_pGuildAWordSaveBtn;
	
	CTextBox *				m_pServerNameTxt;
	CTextBox *				m_pChannelNumTxt;

	CTextBox*				m_pMSOptionTxt;
	CTextBox*				m_pMSWhisperBlockTxt;
	CCheckButton*			m_pMSWhisperBlockBox;

	//........................................................................................................
	// ��Ƽ(����) �ź�
	//........................................................................................................
	CTextBox*		m_pPartyCallTxt;
	CCheckButton*	m_pPartyCallBtn;

	//........................................................................................................
	// �ŷ� �ź�
	//........................................................................................................
	CTextBox*		m_pExChangeTxt;
	CCheckButton*	m_pExChangeBtn;
		
	
	CButton *				m_pSettingCompleteBtn;		//�����Ϸ� ��ư	

	BOOL					m_IsSelLocal;				//�������� ����
	BOOL					m_IsSelLevel;				//�������� ����
	BOOL					m_IsSelJob;					//�������� ����

	BOOL					m_IsWhisperBlock;		//��ü �Ӹ� ����
	BOOL					m_IsPartyCall;
	BOOL					m_IsExChange;
	
	UINT					m_MessenState;				//�޽��� ����ǥ��
	
	TCHAR					m_strTemp[MAX_SIZE_256];
	TCHAR					m_strServerName[MAX_SIZE_128];		//������
	int						m_nChannelNum;
	
public:
	CMSettingTabWnd();
	~CMSettingTabWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	void ResetSettingData();
	void SetSelectLocal(BOOL IsSelLocal, bool bReNewFlag);
	void SetSelectLevel(BOOL IsSelLevel, bool bReNewFlag);
	void SetSelectJob(BOOL IsSelJob, bool bReNewFlag);
	void SetMessenState(UINT uMessenState, bool bReNewFlag);

	void SetServerName( TCHAR *strName );
	void SetChannelNum( int nNum );
	BOOL GetIsPartyCall(){return m_IsPartyCall;}
	BOOL GetIsExChange(){return m_IsExChange;}
	BOOL GetIsWhisperBlock(){return m_IsWhisperBlock;}

};

//-----------------------------------------------------------------------------
#endif	__MSETTINGTABWND_H__