#ifndef __MESSENONECHATWND_H__
#define __MESSENONECHATWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "MLineEditBox.h"

#define MAX_MCHATSTRDATA_NUM 30

#define MINCHATWND_LINE_NUM 6
#define MIDCHATWND_LINE_NUM	12
#define MAXCHATWND_LINE_NUM 18

struct SMChatStrData
{
	TCHAR		* pStrChar;
	UINT	  	txtREDColor;
	UINT		txtGREENColor;
	UINT		txtBLUEColor;
	
	SMChatStrData()
	{
		pStrChar = NULL;
		txtREDColor = 255;
		txtGREENColor = 255;
		txtBLUEColor = 255;	
	}
};

class CMessenOneChatWnd : public CWndProc
{
protected:
	bool				m_IsOpenInstant;		//ó�� ������ ����
	long				m_lPrevTime;			//���� ä���Է� �ð�
	
	CFrameWnd *			m_pMessenChatBgWnd;		//��ü������
	
	//���� Ÿ��Ʋ ������
	CFrameWnd *			m_pUpTitileWnd;			
	CButton *			m_pMCCloseBtn;
	CTextBox *			m_pMCTitleName;	
	
	CFrameWnd *			m_pMCOutChatMinWnd;		//�ּ� ��ȭ���â ������
	CScrollBar *		m_pMCMinScrollBar;
	CTextBox *   		m_pMMinChatSrtField[MINCHATWND_LINE_NUM];	

	CFrameWnd *			m_pMCOutChatMidWnd;		//�߰� ��ȭ���â ������
	CScrollBar *		m_pMCMidScrollBar;
	CTextBox *   		m_pMMidChatSrtField[MIDCHATWND_LINE_NUM];
	
	CFrameWnd *			m_pMCOutChatMaxWnd;		//�ִ� ��ȭ���â ������
	CScrollBar *		m_pMCMaxScrollBar;
	CTextBox *   		m_pMMaxChatSrtField[MAXCHATWND_LINE_NUM];

	CFrameWnd *			m_pDownControlWnd;		//���� ��Ʈ�� ������
	CWnd*				m_pDownControlBg;		//���� ��Ʈ�� ����̹��� 		 
	CButton*			m_pSizeAddBtn;			//������ Ȯ���ư
	CButton*			m_pSizeSubBtn;			//������ ��ҹ�ư	
	CButton *			m_pMCMsgSendBtn;
	CMLineEditBox *		m_pMCInputMultiEdBox;
	
		
		

	SMChatStrData		m_MChatStrDataList[MAX_MCHATSTRDATA_NUM + 1];
	
	
	UINT				m_WndSizeState;	
	
	int					m_nStartListNum;	
	int					m_nInsertIndex;		//�����Ͱ� ���� �ε���

	UINT				m_uFrameWndID;
	UINT				m_uTitileWndID;
	UINT				m_uTitleNameID;
	UINT				m_uCloseBtnID;

	UINT				m_uOutChatMinWndID;
	UINT				m_uMinScrollBarID;

	UINT				m_uOutChatMidWndID;
	UINT				m_uMidScrollBarID;

	UINT				m_uOutChatMaxWndID;
	UINT				m_uMaxScrollBarID;

	UINT				m_uDownControlWndID;
	UINT				m_uDownControlBgID;
	UINT				m_uSizeAddBtnID;
	UINT				m_uSizeSubBtnID;
	UINT				m_uMsgSendBtnID;	
	UINT				m_uInputMultiEdBoxID;



	TCHAR 				m_strInterDestName[512];
	TCHAR				m_strChatstring[120];
	TCHAR				m_strTemp[512];

public:
	CMessenOneChatWnd();
	~CMessenOneChatWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

public:
	virtual void	InsertChatContent(TCHAR * pStrName, TCHAR *pStrContent);
	virtual void	InsertNotifyContent(TCHAR *pStrContent, UINT Red, UINT Green, UINT Blue);

	void	InsertChatStrData(TCHAR * pStrChar, UINT Red, UINT Green, UINT Blue);
	void    SetMChatStrField();
	void	SetListScroll( int _linenum );

	CFrameWnd * GetFrameWnd();
	UINT	GetFrameWndID();
	UINT	GetInputMultiEdID();
	virtual void	SetInterDestName(void * strName);
	TCHAR * GetInterDestName();

	virtual void    SetVisible(bool Flag);
	void	SetSizeState(UINT uState);
	int		GetStrLineNum(bool bCompareFlag);
	CTextBox ** GetStrFieldList();
	CScrollBar * GetScrollBar();
};

//-----------------------------------------------------------------------------
#endif	__MESSENONECHATWND_H__












