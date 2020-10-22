#ifndef __MESSENMULTICHATWND_H__
#define __MESSENMULTICHATWND_H__
//-----------------------------------------------------------------------------

#include "MessenOneChatWnd.h"
#include "FlashFrameWnd.h"


#define MAX_MCHAT_MEMNUM_EXCEPT_ME  5

class CMessenMultiChatWnd : public CMessenOneChatWnd
{
protected:	
	TCHAR 		m_strChatMasterName[MAX_SIZE_512];			//ä�ù� �������
	bool		m_IsActiveWnd;						//ä��â�� Ȱ������ - ������ ���� �����ϸ� ��Ȱ��ȭ�ȴ�.
	bool		m_IsViewAllMember;					//��ü��� ��������
	bool		m_bIsMinimize;						//�ּ�ȭ ����
	
	CButton*	m_pMChatMinBtn;						//��Ƽä�������� �ּ�ȭ ��ư	
	UINT		m_uMChatMinBtnID;					//�ּ�ȭ��ư ���̵�	
	
	CTextBox *  m_pViewMemNameTxt;	
	CTextBox *  m_pAddViewMemNameTxt[MAX_MCHAT_MEMNUM_EXCEPT_ME - 1];
	
	CFrameWnd*  m_pMemberViewFrameWnd;				//������� ������
	CButton *   m_pMemberViewBtn;					//������� ��ư
	CButton *   m_pMemberHideBtn;					//������� ���߱��ư
	CWnd *		m_pMemberViewBgWnd;					//��ü��� ���� ����̹���

	UINT		m_MemberViewFrameWndID;				//������� �����Ӿ��̵�
	UINT		m_uMemberViewBtnID;					//������� ��ư ���̵�
	UINT		m_uMemberHideBtnID;					//������� ���߱��ư���̵�
	UINT		m_uMemberViewBgWndID;				//��ü��� ���� �����̵�

	TCHAR		* m_pStrMemNameList[MAX_MCHAT_MEMNUM_EXCEPT_ME + 1];
	int			m_nNumOfMemList;
	
	//�ּ�ȭ ������ ������ �������
	CFlashFrameWnd * m_pMChatMinWnd;
	CTextBox *		 m_pMCMinTitleName;	
	CButton*		 m_pMChatMaxBtn;			//��Ƽä�������� �ִ�ȭ ��ư
	CButton*		 m_pMChatCloseBtn;			//�ּ�ȭ ���¿��� ä��Ż�� ��ư
	
	UINT			 m_uMChatMinWndID;
	UINT			 m_uMCMinTitleNameID;	
	UINT			 m_uMChatMaxBtnID;
	UINT			 m_uMChatCloseBtnID;
	
	long			 m_lPrevTime;			//���� ä���Է� �ð�

public:
	CMessenMultiChatWnd();
	~CMessenMultiChatWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

	virtual void InsertChatContent(TCHAR * pStrName, TCHAR *pStrContent);
	virtual void InsertNotifyContent(TCHAR *pStrContent, UINT Red, UINT Green, UINT Blue);

	void	SetChatMasterName(TCHAR * strName);
	TCHAR * GetChatMasterName();
	void	SetAllMemberWndListView(bool bFlag);
	virtual void    SetVisible(bool Flag);
	void    SetMinimize(bool bFlag);

	void	InsertChatMemName(TCHAR * strName, bool bReNewFlag);
	void	DeleteChatMemName(TCHAR * strName, bool bReNewFlag);

	void	SetMemNameTxtList();
	void    SetActiveWnd(bool bFlag);
	bool	GetActiveWnd();

	bool    ExistMemName(TCHAR *strName);
};

//-----------------------------------------------------------------------------
#endif	__MESSENMULTICHATWND_H__
