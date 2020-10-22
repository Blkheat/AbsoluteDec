#if !defined( __RUI_PARTYINFOR_WND_H__ )
#define __RUI_PARTYINFOR_WND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "LimitLenTextBox.h"
#include "PartyMemberWnd.h"
#include "PartyBaseInfo.h"

extern BOOL IsMyPartyAndLeagueMember( UINT dPCID );

class CPartyInforWnd : public CWndProc
{
public:
	CFrameWnd*			 m_pPartyInforFrame;			//��Ƽ ��ü ���� ������
	int					 m_nSelectMemberIndex;			//���õ� ��Ƽ����� NPC���̺� �ε���

protected:	
	UINT				 m_uPartyType;	
	
	SPARTY_INFO          m_PartyInfo;		
	BOOL		         m_bIsMemberParty;				//�ڽ��� ��Ƽ�� �������ִ��� ����
	bool				 m_bIsMinimize;					//�ּ�ȭ ����
	bool				 m_bViewItemHowMenu;			//������ ��å �޴��� �俩�� 	

	//��� ��Ʈ�� ������ ������
	CFrameWnd*			 m_pPartyTitleWnd;				//Ÿ��Ʋ ������ ������
	CButton*			 m_pGetItemViewBtn;				//������ �й���å ������ ���ư
	CLimitLenTextBox*	 m_pPartyNameTxt;				//��Ƽ�̸� ����
	CButton*			 m_pPartyMinBtn;				//��Ƽ������  �ּ�ȭ ��ư


	//�ϴ� ��Ʈ�� ������ ������
	CFrameWnd*			 m_pPartyDownWnd;				//�ϴ� ������ ������
	CButton*			 m_pLeagueViewBtn;				//��ü���� ��� ����â ��ư	
	CButton*			 m_pPartyAddMemberBtn;			//��Ƽ�� �߰� ��ư	
	CButton*			 m_pPartyBreakBtn;				//��Ƽ Ż�� ��ư

	//��� ������ ���� ������ ��å������
	CFrameWnd*			 m_pPartyGetItemMenuWnd;
	CButton *			 m_pPartyGetItemFreeBtn;		//������ ����ȹ�� ��ư
	CButton *			 m_pPartyGetItemSequenceBtn;	//������ ������� ��ư
	CButton *			 m_pPartyGetCountryBtn;			//������ ������� ��ư	

public:
	CPartyMemeberWnd	 m_PartyMemberWnd[MAX_PARTY_MEMBERNUM];		//��Ƽ��� ������	
	
protected:	
	TCHAR				 m_tmpStr[128];

	list < SPARTYMEMBER_INFO * >  m_PartyMemberList;	//��Ƽ �������Ʈ	
	
	//���� ���� ������ ���
	UINT				 m_WIDToolTipDestWnd;
	TCHAR				 m_strToolTipArea[256];

		
public:
	CPartyInforWnd();
	~CPartyInforWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
protected:
	void	CompositionPartyMemberWnd();	
	void	ClearPartyInfo();

public:
	void	SetPartyType(UINT uType);
	UINT	GetPartyType()	{ return m_uPartyType; }
	
	void	SetItemMgr(UINT ItemMgr);	
	void	SetVisible(BOOL bFlag);
	void    SetMinimize(bool bFlag);
	void	ClearAllInfo();
	void	ClearAllPartyMemberList();
	UINT	GetToolTipDestWnd()	{ return m_WIDToolTipDestWnd; }
	void    SetFocusMemberWnd( UINT WIDToolTipDestWnd, void *strToolTipName );
	void	RenderToolTipArea();

public:
	BOOL	IsMemberParty()			{  return m_bIsMemberParty;  }
	SPARTY_INFO * GetPartyInfo()	{  return &m_PartyInfo;  }
	SPARTYMEMBER_INFO * InsertPartyMember( UINT dPCID );
	SPARTYMEMBER_INFO * FindPartyMember( UINT dPCID );
	SPARTYMEMBER_INFO * FindPartyMember( char * strPCName );
	BOOL	DeletePartyMember( UINT dPCID );

	BOOL	MakePCharImage( UINT dPCID );// ĳ���� �� ����
	void	DeletePCharImage( UINT dPCID );
	void	ChangePCharImage( UINT dPCID );	
	void	UpdatePartyWnd();

public:
	list< SPARTYMEMBER_INFO * >::iterator GetBeginMember();
	list< SPARTYMEMBER_INFO * >::iterator GetEndMember();
	list < SPARTYMEMBER_INFO * > * GetPartyMemberList()	{ return &m_PartyMemberList; }
	
	void GiveGagePartyMember(int index);					   //��Ƽ��� ����������
	void SelectPartyMember(int index);						   //��Ƽ ��� ����	
	
	void SelectPartyMemberWnd(int nTableindex);				   //��Ƽ��� ������ ����ǥ��

	void GiveGagePartyMember(SPARTYMEMBER_INFO *pMemberInfo);
	void LogOff();
	int  GetTotPartyMemNumber();
	void RestorePMemberCharImg();
	void ReFreshPMemberCharImg();
protected:
	void UpdatePartyMemberWnd();
	
};

extern CPartyInforWnd g_PartyInforWnd;

//-----------------------------------------------------------------------------
#endif  __RUI_PARTYINFOR_WND_H__