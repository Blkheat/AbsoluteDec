#if !defined( __MMSG_WND_H__ )
#define __MMSG_WND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"

enum MMSG_TYPE
{
	//------------------------------------------------
	//메신져 관련 메세지	
	//------------------------------------------------
	MSG_MINPUTFRIEND = 0,
	MSG_MDELETEFRIEND,
	MSG_MINPUTBLOCK,
	MSG_MDELETEBLOCK,
};

class CMMsgWnd : public CWndProc
{
protected:
	CFrameWnd * m_pMMsgFrame;

	CButton*	m_pMMsgOkBtn;
	CButton*	m_pMMsgCancleBtn;
	CEditBox*	m_pMMsgInEdit;		 //입력 에디트박스
	CTextBox*	m_pMMsgTextBox;	 //텍스트 라인
	CImageBox*	m_pMMsgImgBox;

	TCHAR		m_strParam[MAX_SIZE_256];	 //매개 문자열
	UINT		m_Msg_Type;		 //메시지 타입


public:
	CMMsgWnd();
	~CMMsgWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	void SetParam( TCHAR * strName );
	UINT GetMsgType()	{ return m_Msg_Type; }
	void SetMsgType( UINT UIMsgType );
	void BtnClickProc( int WIDBtn );
	
	void SetVisible(bool bFlag);
};

//-----------------------------------------------------------------------------
#endif  __MMSG_WND_H__