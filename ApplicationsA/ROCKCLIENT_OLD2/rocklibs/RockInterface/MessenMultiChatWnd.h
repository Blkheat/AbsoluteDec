#ifndef __MESSENMULTICHATWND_H__
#define __MESSENMULTICHATWND_H__
//-----------------------------------------------------------------------------

#include "MessenOneChatWnd.h"
#include "FlashFrameWnd.h"


#define MAX_MCHAT_MEMNUM_EXCEPT_ME  5

class CMessenMultiChatWnd : public CMessenOneChatWnd
{
protected:	
	TCHAR 		m_strChatMasterName[MAX_SIZE_512];			//채팅방 방장네임
	bool		m_IsActiveWnd;						//채팅창의 활성유무 - 방장이 방을 해제하면 비활성화된다.
	bool		m_IsViewAllMember;					//전체멤버 보기유무
	bool		m_bIsMinimize;						//최소화 여부
	
	CButton*	m_pMChatMinBtn;						//멀티채팅윈도우 최소화 버튼	
	UINT		m_uMChatMinBtnID;					//최소화버튼 아이디	
	
	CTextBox *  m_pViewMemNameTxt;	
	CTextBox *  m_pAddViewMemNameTxt[MAX_MCHAT_MEMNUM_EXCEPT_ME - 1];
	
	CFrameWnd*  m_pMemberViewFrameWnd;				//멤버보기 프레임
	CButton *   m_pMemberViewBtn;					//멤버보기 버튼
	CButton *   m_pMemberHideBtn;					//멤버보기 감추기버튼
	CWnd *		m_pMemberViewBgWnd;					//전체멤버 보기 배경이미지

	UINT		m_MemberViewFrameWndID;				//멤버보기 프레임아이디
	UINT		m_uMemberViewBtnID;					//멤버보기 버튼 아이디
	UINT		m_uMemberHideBtnID;					//멤버보기 감추기버튼아이디
	UINT		m_uMemberViewBgWndID;				//전체멤버 보기 배경아이디

	TCHAR		* m_pStrMemNameList[MAX_MCHAT_MEMNUM_EXCEPT_ME + 1];
	int			m_nNumOfMemList;
	
	//최소화 프레임 윈도우 구성요소
	CFlashFrameWnd * m_pMChatMinWnd;
	CTextBox *		 m_pMCMinTitleName;	
	CButton*		 m_pMChatMaxBtn;			//멀티채팅윈도우 최대화 버튼
	CButton*		 m_pMChatCloseBtn;			//최소화 상태에서 채팅탈퇴 버튼
	
	UINT			 m_uMChatMinWndID;
	UINT			 m_uMCMinTitleNameID;	
	UINT			 m_uMChatMaxBtnID;
	UINT			 m_uMChatCloseBtnID;
	
	long			 m_lPrevTime;			//이전 채팅입력 시간

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
