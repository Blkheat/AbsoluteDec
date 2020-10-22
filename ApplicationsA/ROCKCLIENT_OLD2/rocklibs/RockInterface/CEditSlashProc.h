// EditSlashProc.h: interface for the CEditSlashProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITSLASHPROC_H__119E8C8B_9CFA_4943_B1DD_444AAC10F53E__INCLUDED_)
#define AFX_EDITSLASHPROC_H__119E8C8B_9CFA_4943_B1DD_444AAC10F53E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum SlashChatCommand
{
    nNormalChatCommand = 0,
	nWhisperChatCommand,
    nYellChatCommand,
    nPartyChatCommand,
};

enum SlashExcuteCommand
{
    nNoneCommand = 0,
    nNoSupportCommand,
	nInvitePatryCommand,
	nLeavePartyCommand,
    nChangePnameCommand,
	nGiveLearderCommand,
	nChangeHowRootCommand,
	nExchangeEnableCommand,	
	nExchangeNotEnableCommand
};

class CEditSlashProc  
{
public:
	BOOL					m_BCheckSlashCommand;    
	SlashChatCommand        m_nChatCommandType;
	SlashExcuteCommand		m_nExcuteCommandType;
	TCHAR					m_TWhisDest[256];

protected:	
	CEditBox *      m_pEditBox;
	TCHAR           m_TstrBuf[256];
	TCHAR           m_Title[256];
	D3DCOLOR        m_Tcolor;
	
	TCHAR			m_TInviteDest[256];
	TCHAR           m_LeavePcName[256];
	TCHAR			m_TPartyName[256];

	TCHAR			m_TGiveLeader[256];
	TCHAR			m_TChangeHowRoot[256];


public:
	CEditSlashProc();
	virtual ~CEditSlashProc();	
	
	void		SetEditBox(CEditBox * pEditBox)   {  m_pEditBox = pEditBox;   }
	int			GetSizeText(HWND hEdit, int iTextLimitLen);
	int			RenderTitle(int Ax, int Ay, CFontg * theFontg, int & CaretH);

	void		CheckSlashCommand(HWND hEdit, int iTextLimitLen);
	void		CheckCommandType(int index);
	int			CheckChatCommandType(TCHAR *strCmd, int index);
	void        CheckExcuteCommandType(TCHAR *strCmd, int index);
	BOOL		FindParameter(TCHAR * strTemp, int & index, BOOL BFlag, char *strCmp);

    int			DetectionDBCS( int* Count, char* Str, char *Db, BOOL bFlag );
	BOOL		EqualString(char *str1, char *str2);

	void		CheckNormalChat();
	void		CheckWisperChat(int index);
	void		CheckWhisperChatSecond();
	void		CheckYellChat();	
	void		CheckPartyChat();
	
	void		CheckInviteParty(int index);
	void		CheckInvitePartySecond();
	void		CheckLeaveParty(int index);
	void        CheckLeavePartySecond();
	void		CheckChangePName(int index);
	void		CheckChangePNameSecond();
	
	void		CheckGiveLearder(int index);
	void        CheckGiveLeaderSecond();
    void        CheckChangeHowRoot(int index);
	void        CheckChangeHowRootSecond();        

	void		CheckExchangeEnable( int aIindex );
	void		CheckExchangeNotEnable( int aIindex );
	
	int			ProcessExcuteCommand(HWND HEdit);
	
	void		ExcuteInvitePatry();
    void		ExcuteLeaveParty();
    void   	    ExcuteChangePname();

	void		ExcuteGiveLearder();
	void		ExcuteChangeHowRoot();

	void		ExcuteExchangeEnable();
	void		ExcuteExchangeNotEnable();

public:
	void		SetInviteDestName(TCHAR *destName);
	
};

extern CEditSlashProc g_CEditSlashProc; 

#endif // !defined(AFX_EDITSLASHPROC_H__119E8C8B_9CFA_4943_B1DD_444AAC10F53E__INCLUDED_)
