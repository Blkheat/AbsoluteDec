// NpcChatWnd.h: interface for the CNpcChatWnd class.
// NPC Click Window
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCCHATWND_H__67AD65FB_A1CF_467E_A236_777E333D76C1__INCLUDED_)
#define AFX_NPCCHATWND_H__67AD65FB_A1CF_467E_A236_777E333D76C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct fPos
{
	float x;
	float z;

	fPos(){
		x =0 ; 
		z =0 ; 

	}

};

#ifdef C_NPCCHAT_RENEWAL
	const int MAX_NPC_CHAT_STRING = 12;
#else 
	const int MAX_NPC_CHAT_STRING = 14;
#endif 




struct SNpcDesc
{
	DWORD	dIndex;
	char	strDesc[256];
	
	SNpcDesc()
	{
		memset(this, 0, sizeof(SNpcDesc));
	}
};

typedef vector < SNpcDesc  > SNpcDescList;
typedef vector < SNpcDesc  > ::iterator SNpcDescListPos;



class CNpcChatWnd : public CWndProc
{
public:	
	WORD			theNpcCode;

	int				theNpcTableIdx;

	CFrameWnd*		FrameWnd;
	CImageBox*		theNpcPictureImg;
	CButton*		m_NextBtn;
	CTextBox*		theLevel;
	CTextBox*		theName;

	CTextBox*		NPCCitizen;
	CTextBox*		NPCName;
	CTextBox*		NPCSex;
	CTextBox*		NPCLevel;
	CTextBox*		NPCMission;
	
	CTextBox*		m_AskText;
	CTextBox*		m_AskTextEx[MAX_NPC_CHAT_STRING];

	SNpcDescList	m_NpcDescList;

	bool			bPayScriptStart;
	fPos			CurPos;
	fPos			TempPos;
	bool			OneRun;
	CScrollBar*		m_pScrollBar;

	int m_StartScorll;
	

public:
	CNpcChatWnd(){}
	virtual ~CNpcChatWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

	void Open();
	
	void CompositionNew();	//서브직업 들어가면서 리뉴얼 
	void CompositionOld();	//봉인의 성지 들어간 버젼 
	void CompositionFirst();	//봉인의 성지 들어가기 이전버젼 
	
	void SetListScroll( int _linenum );
	

	void SetSayString( void* aSayString );
	void SetAnswerBtnString( int aBtnWIDIndex, char* aAnswerString );

	void CloseNpcChatWnd( bool aIsSendCancel = true );	

	//스트링 데이터를 가공을 한다.
	void InsertNpcDesc(void *NpcString);
	void UpdateNpcDesc();
		

};

extern	CNpcChatWnd		NpcChatWnd;





#endif // !defined(AFX_NPCCHATWND_H__67AD65FB_A1CF_467E_A236_777E333D76C1__INCLUDED_)





