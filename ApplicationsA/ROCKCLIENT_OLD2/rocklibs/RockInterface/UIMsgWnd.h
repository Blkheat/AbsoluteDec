#if !defined( __RUI_UIMSG_WND_H__ )
#define __RUI_UIMSG_WND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"

enum UIMSG_TYPE
{
	//------------------------------------------------
	//파티 관련 메세지	
	//------------------------------------------------
	MSG_PARTYCALL = 0,		//파티 요청작업
	MSG_PARTYBREAK,			//파티 해체 확인
	MSG_PARTYGIVE,			//리더 양도 확인 	
  	MSG_PARTYIN,			//파티 초대작업
	MSG_PARTYOUT,			//파티 탈퇴작업
	MSG_PARTYRENAME,		//파티 이름변경
	MSG_PARTYGOAWAY,        //파티 강제탈퇴
	
	//------------------------------------------------
	//리그 관련 메시지
	//------------------------------------------------
	MSG_LEAGUECALL,			//리그 요청작업
	MSG_LEAGUEBREAK,		//리그 해체 확인	
  	MSG_LEAGUEIN,			//리그 초대작업
	MSG_LEAGUEOUT,			//리그 탈퇴작업
	MSG_LEAGUENAME,			//리그 이름변경

	//------------------------------------------------
	//교환 관련 메시지
	//------------------------------------------------
	MSG_ACCEPT_EXCHANGE,		

	//------------------------------------------------
	//커뮤니티 관련 메세지
	//------------------------------------------------
	MSG_MFRIENDINVITE,		//친구 초대작업
	MSG_MCHATINVITE,		//채팅 초대작업
	
	//------------------------------------------------
	//프론티어 관련 메세지
	//------------------------------------------------
	MSG_FRONTIERINVITE,		//프론티어 초대작업
	MSG_FRONTIERMYINFO_INPUT,     //프로티어 자기소개
	MSG_FRONTIERNOTICE_INPUT,     //프로티어 공지사항

	//------------------------------------------------
	//군단 관련 메세지
	//------------------------------------------------	
	MSG_CORPORATION_JOIN_1,    // 군단가입
	MSG_CORPORATION_JOIN_2,    // 군단가입
	MSG_CORPORATION_JOIN_3,    // 군단가입
	MSG_CORPORATION_LEAVE,     // 군단탈퇴
			
	//------------------------------------------------
	//소환,출두 아이템사용 관련 메세지
	//------------------------------------------------
	MSG_MOVE_DEST,			//출두 하고자하는 대상
	MSG_CALL_DEST,			//소환 하고자하는 대상
	MSG_CALL_ASK,			//소환 요청작업

	//------------------------------------------------
	//연합 프론티어 관련 메세지
	//------------------------------------------------
	MSG_INCOMBINE_FRONTIER,	  //연합프론티어 초대작업
	
	//------------------------------------------------
	//유료 아이템 관련 메세지
	//------------------------------------------------
	MSG_RE_PETNAME,			//펫 이름 바꾸기 작업
	MSG_RE_PCNAME,			//캐릭터 이름 바꾸기 작업
	MSG_RE_FRONTIER_NAME,	//프론티어 이름 바꾸기 작업
	MSG_RE_GENDER,			//성별 전환 작업
	
};

class CUIMsgWnd : public CWndProc
{
protected:
	TCHAR				m_strParam[256];             //매개 문자열
	TCHAR				m_strSecondParam[256];       //매개 문자열
	
	DWORD				m_dNameID;
	DWORD				m_ItemID;
	BYTE				m_aPetSlot;
	
	CImageBox*		    img;
	CSlot*		     	m_item_slot;

	UINT				m_UIMsg_Type;				 //메시지 타입

	CFrameWnd*		    m_pUIMsgFrame;
	CFrameWnd*		    m_pUIMsgFrameAdd;
	
	CButton*			m_pUIMsgOkBtn;
	CButton*			m_pUIMsgCancleBtn;
	CButton*			m_pUIMsgOkBtnAdd;
	CButton*			m_pUIMsgCancleBtnAdd;
	
	CEditBox*			m_pUIMsgInEdit;				 //입력 에디트박스
	CEditBox*			m_pUIMsgLongInEdit;			 //긴 입력 에디트박스
	CEditBox*			m_pUIMsgLongInEditAdd;		 //긴 입력 에디트박스
	
	
	CTextBox*			m_pUIMsgTextBox[3];			 //텍스트 라인
	
	CTextBox*			m_pUIMsgMultiBox;			 //멀티 라인 텍스트 박스
	CTextBox*			m_pUIMsgMultiBoxAdd;		 //멀티 라인 텍스트 박스

public:
	CUIMsgWnd();
	~CUIMsgWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

protected:
	void BtnClickProc( int WIDBtn );

public:	

	SPcItem				m_pPcItem;

	void UpdateItemSlot();
	void SetParam( TCHAR * strName );
	void SetParamID( DWORD dNameID );
	void SetSecondParam( TCHAR * strParam );
	void SetItemID( DWORD ItemID );
	void SetPetSlot( BYTE aPetSlot );
	void SetMsgType( UINT UIMsgType );
	void LogOff();

};

extern CUIMsgWnd g_UIMsgWnd;

//-----------------------------------------------------------------------------
#endif  __RUI_UIMSG_WND_H__