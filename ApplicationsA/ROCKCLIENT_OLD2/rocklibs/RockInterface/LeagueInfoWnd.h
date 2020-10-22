#ifndef __LEAGUEINFOWND_H__
#define __LEAGUEINFOWND_H__
//-----------------------------------------------------------------------------

#include "WndProc.h"
#include "LimitLenTextBox.h"
#include "PartyBaseInfo.h"

struct	SLeague_Data
{
	CImageBox9		  * pBackImg;	
	CLimitLenTextBox  * pNameTxt;
	CTextBox		  * pLevelTxt;
	CLimitLenTextBox  * pJobTxt;
	CImageBox 		  * pMaster;		
	SPARTYMEMBER_INFO * pPartyMemberInfo;
	CButton9		  * pButton;	

	SLeague_Data()
	{
		pNameTxt = NULL;
		pLevelTxt = NULL;
		pJobTxt = NULL;
		pPartyMemberInfo = NULL;	
	}
};

class CLeagueInfoWnd : public CWndProc
{
protected:
	SPARTYMEMBER_INFO	* m_pPartyMemInfoList[MAX_LEAGUE_MEMBER_NUM + 1];

	CFrameWnd *			m_pLeagueFrameWnd;			
	SLeague_Data 		m_LeagueDataList[MAX_LEAGUE_MEMBER_NUM];
	CButton *			m_CloseBtn;
	
	int					m_nInsertIndex;

public:
	CLeagueInfoWnd();
	virtual ~CLeagueInfoWnd();

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	void Open();
	void Close();

	SPARTYMEMBER_INFO * InsertLeagueMember(DWORD dPCID);
	void SetLeagueMemberField();
	void ChangePartyMember(int nIndex);
	SPARTYMEMBER_INFO * FindLeagueMember(DWORD dPCID);
	SPARTYMEMBER_INFO * FindLeagueMember(TCHAR *strName);
	void ReSetPartyMember(bool bFlag);

	int GetLeagueMemSize()	{ return m_nInsertIndex; }
	SPARTYMEMBER_INFO	**	GetLeagueMemList()	{ return m_pPartyMemInfoList; }

	void AllDelete();
	void DeleteLeagueMember(DWORD dPCID);
	void LogOff();
};

extern CLeagueInfoWnd g_LeagueInfoWnd;

//-----------------------------------------------------------------------------
#endif	__LEAGUEINFOWND_H__