#if !defined( __RUI_UIMSG_WND_H__ )
#define __RUI_UIMSG_WND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"

enum UIMSG_TYPE
{
	//------------------------------------------------
	//��Ƽ ���� �޼���	
	//------------------------------------------------
	MSG_PARTYCALL = 0,		//��Ƽ ��û�۾�
	MSG_PARTYBREAK,			//��Ƽ ��ü Ȯ��
	MSG_PARTYGIVE,			//���� �絵 Ȯ�� 	
  	MSG_PARTYIN,			//��Ƽ �ʴ��۾�
	MSG_PARTYOUT,			//��Ƽ Ż���۾�
	MSG_PARTYRENAME,		//��Ƽ �̸�����
	MSG_PARTYGOAWAY,        //��Ƽ ����Ż��
	
	//------------------------------------------------
	//���� ���� �޽���
	//------------------------------------------------
	MSG_LEAGUECALL,			//���� ��û�۾�
	MSG_LEAGUEBREAK,		//���� ��ü Ȯ��	
  	MSG_LEAGUEIN,			//���� �ʴ��۾�
	MSG_LEAGUEOUT,			//���� Ż���۾�
	MSG_LEAGUENAME,			//���� �̸�����

	//------------------------------------------------
	//��ȯ ���� �޽���
	//------------------------------------------------
	MSG_ACCEPT_EXCHANGE,		

	//------------------------------------------------
	//Ŀ�´�Ƽ ���� �޼���
	//------------------------------------------------
	MSG_MFRIENDINVITE,		//ģ�� �ʴ��۾�
	MSG_MCHATINVITE,		//ä�� �ʴ��۾�
	
	//------------------------------------------------
	//����Ƽ�� ���� �޼���
	//------------------------------------------------
	MSG_FRONTIERINVITE,		//����Ƽ�� �ʴ��۾�
	MSG_FRONTIERMYINFO_INPUT,     //����Ƽ�� �ڱ�Ұ�
	MSG_FRONTIERNOTICE_INPUT,     //����Ƽ�� ��������

	//------------------------------------------------
	//���� ���� �޼���
	//------------------------------------------------	
	MSG_CORPORATION_JOIN_1,    // ���ܰ���
	MSG_CORPORATION_JOIN_2,    // ���ܰ���
	MSG_CORPORATION_JOIN_3,    // ���ܰ���
	MSG_CORPORATION_LEAVE,     // ����Ż��
			
	//------------------------------------------------
	//��ȯ,��� �����ۻ�� ���� �޼���
	//------------------------------------------------
	MSG_MOVE_DEST,			//��� �ϰ����ϴ� ���
	MSG_CALL_DEST,			//��ȯ �ϰ����ϴ� ���
	MSG_CALL_ASK,			//��ȯ ��û�۾�

	//------------------------------------------------
	//���� ����Ƽ�� ���� �޼���
	//------------------------------------------------
	MSG_INCOMBINE_FRONTIER,	  //��������Ƽ�� �ʴ��۾�
	
	//------------------------------------------------
	//���� ������ ���� �޼���
	//------------------------------------------------
	MSG_RE_PETNAME,			//�� �̸� �ٲٱ� �۾�
	MSG_RE_PCNAME,			//ĳ���� �̸� �ٲٱ� �۾�
	MSG_RE_FRONTIER_NAME,	//����Ƽ�� �̸� �ٲٱ� �۾�
	MSG_RE_GENDER,			//���� ��ȯ �۾�
	
};

class CUIMsgWnd : public CWndProc
{
protected:
	TCHAR				m_strParam[256];             //�Ű� ���ڿ�
	TCHAR				m_strSecondParam[256];       //�Ű� ���ڿ�
	
	DWORD				m_dNameID;
	DWORD				m_ItemID;
	BYTE				m_aPetSlot;
	
	CImageBox*		    img;
	CSlot*		     	m_item_slot;

	UINT				m_UIMsg_Type;				 //�޽��� Ÿ��

	CFrameWnd*		    m_pUIMsgFrame;
	CFrameWnd*		    m_pUIMsgFrameAdd;
	
	CButton*			m_pUIMsgOkBtn;
	CButton*			m_pUIMsgCancleBtn;
	CButton*			m_pUIMsgOkBtnAdd;
	CButton*			m_pUIMsgCancleBtnAdd;
	
	CEditBox*			m_pUIMsgInEdit;				 //�Է� ����Ʈ�ڽ�
	CEditBox*			m_pUIMsgLongInEdit;			 //�� �Է� ����Ʈ�ڽ�
	CEditBox*			m_pUIMsgLongInEditAdd;		 //�� �Է� ����Ʈ�ڽ�
	
	
	CTextBox*			m_pUIMsgTextBox[3];			 //�ؽ�Ʈ ����
	
	CTextBox*			m_pUIMsgMultiBox;			 //��Ƽ ���� �ؽ�Ʈ �ڽ�
	CTextBox*			m_pUIMsgMultiBoxAdd;		 //��Ƽ ���� �ؽ�Ʈ �ڽ�

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