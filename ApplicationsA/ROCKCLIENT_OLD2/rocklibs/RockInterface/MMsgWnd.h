#if !defined( __MMSG_WND_H__ )
#define __MMSG_WND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"

enum MMSG_TYPE
{
	//------------------------------------------------
	//�޽��� ���� �޼���	
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
	CEditBox*	m_pMMsgInEdit;		 //�Է� ����Ʈ�ڽ�
	CTextBox*	m_pMMsgTextBox;	 //�ؽ�Ʈ ����
	CImageBox*	m_pMMsgImgBox;

	TCHAR		m_strParam[MAX_SIZE_256];	 //�Ű� ���ڿ�
	UINT		m_Msg_Type;		 //�޽��� Ÿ��


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