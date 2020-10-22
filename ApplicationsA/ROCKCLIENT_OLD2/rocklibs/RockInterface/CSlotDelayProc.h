// SlotDelayProc.h: interface for the CSlotDelayProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLOTDELAYPROC_H__3156FE3C_620F_49E1_B33C_C85B7B60642A__INCLUDED_)
#define AFX_SLOTDELAYPROC_H__3156FE3C_620F_49E1_B33C_C85B7B60642A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSlotDelayProc  
{
protected:	
	// By sim------------------------------------------------------------------------
	BOOL       m_BLoading;
	WORD       m_WUsecode;
	SPcItem ** m_pPInvenItem;
	int		   m_iTotCount;	
    //-------------------------------------------------------------------------------

	//---------dong-------
	BOOL		m_BPetLoading;
	WORD       m_WPetUsecode;
	SPcItem ** m_pPetPInvenItem;
   

    //시간 체크 멤버변수
	long			m_lNowTime;
	long			m_lpreTime;
	
	//스펠 시전 스킬코드
	WORD       m_WSpellSkillCode;

	
public:
	CSlotDelayProc();
	virtual ~CSlotDelayProc();

	// By sim
	void SetUseDelayTime(BOOL BFlag, SPcItem ** pPInvenItem ,WORD _usecode,
						 int iTotCount);

	//by dongs 
	void SetPetUseDelayTime(BOOL BFlag, SPcItem ** pPInvenItem ,WORD _usecode );


	void SetPInvenItem(SPcItem ** pPInvenItem)
	{ m_pPInvenItem = pPInvenItem; }
	void InitiallizeUseDelayTime();
	void InitiallizeSkillDelayTime();

	virtual void Update();
	
	void InitDelayTime( SRpdDelayTime * pDelayTime , int nDelayTimeNum );
	void SetUseCheckDelayTime(DWORD theItemID);

	WORD GetSpellSkillCode() { return m_WSpellSkillCode; }		
	void SetSpellSkillCode(WORD WSpellSkillCode);
	void InitiallizeSkillDelayTime(WORD WSpellSkillCode);
};

extern CSlotDelayProc g_SlotDelayProc;

#endif // !defined(AFX_SLOTDELAYPROC_H__3156FE3C_620F_49E1_B33C_C85B7B60642A__INCLUDED_)