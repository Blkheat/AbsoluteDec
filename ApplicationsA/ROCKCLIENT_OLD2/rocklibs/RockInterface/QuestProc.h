//----------------------------------------------------------------------------------------------------------------------
// ����Ʈâ
//----------------------------------------------------------------------------------------------------------------------
#if !defined( __RUI_QUEST_WNDPROC_H__ )
#define __RUI_SYSTEMMENU_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Spin.h"
#include "StateButton.h"
#include "Wnd.h"
#include "WndProc.h"


#ifdef QUEST_RENEWAL 
	#define QUEST_FIELD_NUM 19
#else 
	#define QUEST_FIELD_NUM 5
#endif 


#define MAX_QUEST_COUNT_LIST 8

#define MAX_QUEST_CONDITION 4

enum N_SCROLLFOCUS
{
	n_QuestScroll,
	n_DescScroll,
	n_QuestCountScroll
};


struct SQuestCountList
{
	int		Index;
	int		QFieldIdx; 
	char	TextStr[256];
	
	SQuestCountList()
	{
		Index = 0 ;
		QFieldIdx =0; 
		memset(&TextStr , NULL , sizeof(TextStr));
	}
	
};

struct SQuestField
{
	SQuestFlag*		QuestFlag;

	CTextBox*		QuestName;				// ����Ʈ ��
	CTextBox*		Client;					// �Ƿ���
	CTextBox*		TimeLimit;				// ����
	CTextBox*		RecmdLevel;				// ���巹�� recommendation level
	CTextBox*		CompleteItem[MAX_QUEST_CONDITION];		// �Ϸ����� 	

	TCHAR			Desc[1024];				//����
	SQuestCountList	m_QuestCountList[MAX_QUEST_COUNT_LIST];
};


						


class CQuestWnd : public CWndProc
{
public:
	CQuestWnd()
	{
		NumAllFlags		= 0;
		NumNormalFlags	= 0;
		NumStoryFlags	= 0;
		NumChangeFlags	= 0;
		OldNumAllFlags	= 0;

		memset( QuestAllFlags, NULL, sizeof( SQuestFlag* ) * RP_MAX_QUEST_FLAG );
		memset( QuestNormalFlags, NULL, sizeof( SQuestFlag* ) * RP_MAX_QUEST_FLAG );
		memset( QuestStoryFlags, NULL, sizeof( SQuestFlag* ) * RP_MAX_QUEST_FLAG );
		memset( QuestChangeFlags, NULL, sizeof( SQuestFlag* ) * RP_MAX_QUEST_FLAG );

		StartListNum	= 0;
		SelectedBtn		= -1;

		ScrollFocus		= n_QuestScroll;
		
		InsertHighlight	= false;
	}	
	~CQuestWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );


	void CompositionOld();
	void CompositionNew();
	
	void SetInsertHighlight( bool aIsHighlight ) { InsertHighlight = aIsHighlight; }
	void SetQuestInfo();
	void OpenQuestWnd();
	
	//�������� ó��
	void SetQuestWindow( WORD _quest_code, BYTE _status );
	void SetQuestStatus( WORD _quest_code, WORD _flag, BYTE _display , DWORD _Time	);

	void SetCompleteItemText( int aQFieldIdx, SQuestInfo* aQuestInfo );
	void SetCompleteItemTextNew( int aQFieldIdx, SQuestInfo* aQuestInfo );

	void SetSQuestCount( SQuestCount* aQuestCount );
	void SetCompleteItemText( SRpScQuestCount* aQuestCount );
	void SetCompleteItemTextNew( SRpScQuestCount* aQuestCount );

	bool CheckQuestComplete( int aQFieldIdx, SQuestInfo* aQuestInfo );
	
	
	//dongs ���� ����Ʈ ������ �ٽ� �����Ѵ� ��) ������ ���� ��� .. 
	void SetRenewalQuestInfo() {  QuestBntSelected( this->SelectedBtn );	}
	void SetQuestField();	
	
private:
	
	N_QUESTTAB		TabState;
	N_SCROLLFOCUS	ScrollFocus;

	SQuestField		QField[QUEST_FIELD_NUM];

	SQuestInfo*		QuestInfoTable;
	SPcDataInven*	PcInven;
	
	//------------------------------------------------------------------------
	CStateButton*	TabSBtn[4];
	CButton*		SelBtn[QUEST_FIELD_NUM];		// ����Ʈ ���� ��ư
	CScrollBar*		ListScroll;		// ����Ʈ ���� ��ũ��

	CTextBox*		QuestDescText;	// ����Ʈ ���� �ڽ�
	CTextBox*		QuestReceiveText;	// ����Ʈ ���� �ڽ�
	CTextBox*		StartNpc;
	CScrollBar*		DescScroll;		// ����Ʈ ����Ʈ ��ũ��
	//------------------------------------------------------------------------
	
	CButton*		CancelBtn; 				

	int				StartListNum;
	int				SelectedBtn;

	int				QuestCntStartListNum;

	CScrollBar*		QuestCountScroll;		// ����Ʈ ����Ʈ ��ũ��
	CImageBox9*		QuestCntScollFocus;


	int				NumAllFlags;
	int				OldNumAllFlags;				//by.jjplusi011
	int				NumNormalFlags;
	int				NumStoryFlags;
	int				NumChangeFlags;
	
	SQuestFlag*		QuestAllFlags[RP_MAX_QUEST_FLAG];
	SQuestFlag*		QuestNormalFlags[RP_MAX_QUEST_FLAG];
	SQuestFlag*		QuestStoryFlags[RP_MAX_QUEST_FLAG];
	SQuestFlag*		QuestChangeFlags[RP_MAX_QUEST_FLAG];

	bool			InsertHighlight;

	///void SetQuestTab( N_QUESTTAB _quest_tab );
	void SetQuestSelect( WORD _quest_code );

	void SetQuestListScroll( int _linenum );
	void SetDecsTextScroll( int _linenum );
	int  DeleteQuestCode(WORD quest_code,int index);
	SPreQuestInfo *GetQuest(WORD quest_code);
	int  FindQuestNpc(int &quest_code,int index);
	void QuestTimeCheck(int SelBtn);

	//����Ʈ ī��Ʈ ����Ʈ ���� 4���� 8���� Ȯ�� ��ũ�� ž�� 
	void SetQuestCountListScroll(int _linenum );



	void QuestBntSelected( int _num_btn );
	void QuestCancel( WORD _quest_code );
	
	void SetRewardStr( WORD aQuestCode, BYTE aQuestFlag, char* pStrReward );
	void SetRewardStrNew( WORD aQuestCode, BYTE aQuestFlag, char* pStrReward );
};


extern	CQuestWnd	QuestWnd;


//-----------------------------------------------------------------------------
#endif  __RUI_QUEST_WNDPROC_H__