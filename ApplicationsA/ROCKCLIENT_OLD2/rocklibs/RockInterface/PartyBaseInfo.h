#if !defined( __RUI_PARTYBASEINFO_WND_H__ )
#define __RUI_PARTYBASEINFO_WND_H__
//-----------------------------------------------------------------------------

struct SPARTYMEMBER_INFO
{
    DWORD m_dPCID;								//파티멤버 아이디		
	char  m_strPCName[RP_MAX_PC_NAME];			//멤버 네임
	DWORD m_dCitizen;
	DWORD m_dGender;
	DWORD m_dFace;
	DWORD m_dHair;
	BYTE  m_bMainClass;
	DWORD m_bFrame;
	BYTE  m_bLevel;								//멤버 레벨
	BYTE  m_bGage;								//멤버 게이지
	BYTE  m_bMaxGage;							//멤버 최대게이지양	
	WORD  m_wMap;								//멤버 맵
	DWORD m_dCell;								//멤버 셀

	BOOL  m_bWasMember;							//파티 멤버였는지 여부
	BOOL  m_bIsSee;								//생김새를 알고 있는지여부
	BOOL  m_bIsViewRange;						//가시 영역에 있는지 여부
	BOOL  m_bIsLogOut;							//로그오프 여부
	
	int   m_bRecon;								//로그오프했다가 재접했는지 여부

	SPARTYMEMBER_INFO()
	{ 
       m_dPCID = 0;
	   m_bWasMember = TRUE;
	   m_bIsSee = FALSE;
	   m_bIsViewRange = FALSE;
	   m_bIsLogOut = FALSE;

	   m_bMaxGage = 0;
	   m_bRecon = FALSE;
	}	
};

enum PARTY_WND_TYPE
{
	NORMAL_PARTY = 0,
	LEAGUE_PARTY,
};

struct SPARTY_INFO
{
    DWORD m_dPartyID;							//파티 아이디
    char  m_strPartyName[256];					//파티 네임
	DWORD m_dLeaderID;							//파티 리더아이디    
    BYTE  m_bItemMgr;							//아이템 분배정책

	BYTE  m_ClassBonus;							//클래스 보너스
	BYTE  m_MemberNumBonus;						//멤버수 보너스	
	BYTE  m_LevelPenalty;						//레벨 패널티
	
    SPARTY_INFO()
	{
	  memset(this, 0, sizeof(SPARTY_INFO));	  
		
	  m_ClassBonus = 0;
      m_MemberNumBonus = 0;
	  m_LevelPenalty = 0;
	}
};

#define MAX_PARTY_MEMBERNUM		5
#define MAX_LEAGUE_MEMBER_NUM	30

//-----------------------------------------------------------------------------
#endif	__RUI_PARTYBASEINFO_WND_H__