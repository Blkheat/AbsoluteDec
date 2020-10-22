#if !defined( __RUI_PARTYBASEINFO_WND_H__ )
#define __RUI_PARTYBASEINFO_WND_H__
//-----------------------------------------------------------------------------

struct SPARTYMEMBER_INFO
{
    DWORD m_dPCID;								//��Ƽ��� ���̵�		
	char  m_strPCName[RP_MAX_PC_NAME];			//��� ����
	DWORD m_dCitizen;
	DWORD m_dGender;
	DWORD m_dFace;
	DWORD m_dHair;
	BYTE  m_bMainClass;
	DWORD m_bFrame;
	BYTE  m_bLevel;								//��� ����
	BYTE  m_bGage;								//��� ������
	BYTE  m_bMaxGage;							//��� �ִ��������	
	WORD  m_wMap;								//��� ��
	DWORD m_dCell;								//��� ��

	BOOL  m_bWasMember;							//��Ƽ ��������� ����
	BOOL  m_bIsSee;								//������� �˰� �ִ�������
	BOOL  m_bIsViewRange;						//���� ������ �ִ��� ����
	BOOL  m_bIsLogOut;							//�α׿��� ����
	
	int   m_bRecon;								//�α׿����ߴٰ� �����ߴ��� ����

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
    DWORD m_dPartyID;							//��Ƽ ���̵�
    char  m_strPartyName[256];					//��Ƽ ����
	DWORD m_dLeaderID;							//��Ƽ �������̵�    
    BYTE  m_bItemMgr;							//������ �й���å

	BYTE  m_ClassBonus;							//Ŭ���� ���ʽ�
	BYTE  m_MemberNumBonus;						//����� ���ʽ�	
	BYTE  m_LevelPenalty;						//���� �г�Ƽ
	
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