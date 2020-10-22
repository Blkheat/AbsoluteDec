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
	CFrameWnd*			 m_pPartyInforFrame;			//파티 전체 정보 프레임
	int					 m_nSelectMemberIndex;			//선택된 파티멤버의 NPC테이블 인덱스

protected:	
	UINT				 m_uPartyType;	
	
	SPARTY_INFO          m_PartyInfo;		
	BOOL		         m_bIsMemberParty;				//자신이 파티에 가입해있는지 여부
	bool				 m_bIsMinimize;					//최소화 여부
	bool				 m_bViewItemHowMenu;			//아이템 정책 메뉴의 뷰여부 	

	//상단 컨트롤 윈도우 프레임
	CFrameWnd*			 m_pPartyTitleWnd;				//타이틀 윈도우 프레임
	CButton*			 m_pGetItemViewBtn;				//아이템 분배정책 윈도우 뷰버튼
	CLimitLenTextBox*	 m_pPartyNameTxt;				//파티이름 문자
	CButton*			 m_pPartyMinBtn;				//파티윈도우  최소화 버튼


	//하단 컨트롤 윈도우 프레임
	CFrameWnd*			 m_pPartyDownWnd;				//하단 윈도우 프레임
	CButton*			 m_pLeagueViewBtn;				//전체리그 멤버 보기창 버튼	
	CButton*			 m_pPartyAddMemberBtn;			//파티원 추가 버튼	
	CButton*			 m_pPartyBreakBtn;				//파티 탈퇴 버튼

	//상단 윈도우 위쪽 아이템 정책프레임
	CFrameWnd*			 m_pPartyGetItemMenuWnd;
	CButton *			 m_pPartyGetItemFreeBtn;		//아이템 자유획득 버튼
	CButton *			 m_pPartyGetItemSequenceBtn;	//아이템 순서대로 버튼
	CButton *			 m_pPartyGetCountryBtn;			//아이템 마을방식 버튼	

public:
	CPartyMemeberWnd	 m_PartyMemberWnd[MAX_PARTY_MEMBERNUM];		//파티멤버 윈도우	
	
protected:	
	TCHAR				 m_tmpStr[128];

	list < SPARTYMEMBER_INFO * >  m_PartyMemberList;	//파티 멤버리스트	
	
	//지역 툴팁 렌더링 요소
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

	BOOL	MakePCharImage( UINT dPCID );// 캐릭터 얼굴 생성
	void	DeletePCharImage( UINT dPCID );
	void	ChangePCharImage( UINT dPCID );	
	void	UpdatePartyWnd();

public:
	list< SPARTYMEMBER_INFO * >::iterator GetBeginMember();
	list< SPARTYMEMBER_INFO * >::iterator GetEndMember();
	list < SPARTYMEMBER_INFO * > * GetPartyMemberList()	{ return &m_PartyMemberList; }
	
	void GiveGagePartyMember(int index);					   //파티멤버 게이지전송
	void SelectPartyMember(int index);						   //파티 멤버 선택	
	
	void SelectPartyMemberWnd(int nTableindex);				   //파티멤버 윈도우 선택표시

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