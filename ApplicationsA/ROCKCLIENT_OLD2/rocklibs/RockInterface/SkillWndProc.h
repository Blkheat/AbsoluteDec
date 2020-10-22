#if !defined( __RUI_SKILL_WNDPROC_H__ )
#define __RUI_SKILL_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "WndProc.h"
#include "Slot.h"

#include	"..\\..\\Bravolibs\\Obj\\SkillSystem.h"

const int c_Max_SkillSlot( 10 );

typedef enum NSkillTabState
{
	nSkillTab_Active,
	nSkillTab_Passive, 
	nSkillTab_SubClass 

} NSkillTabState;

typedef struct SSkillPack
{
	CSlot*				Slot;			
	CProgressBar*		LevelBar;
	CTextBox*			LevelTxt;
	CButton*			LevelBtn;

} SSkillPack;


class CStateButton;
class CSkillWnd : public CWndProc
{
private:
	// 윈도우 구성 컨트롤 
	CFrameWnd*				m_pSkillFWnd;

	CButton*				m_pCloseBtn;						//닫기 버튼	
	
	CStateButton*			m_pActiveSBtn;						//액티브 탭
	CStateButton*			m_pPassiveSBtn;						//패시브 탭	   
	CStateButton*			m_pSubClassSBtn;					//서브직업 탭

	CImageBox9*				m_pInBgImg;							//내부배경이미지
	
	SSkillPack				m_SkillPack[c_Max_SkillSlot];		//스킬 슬롯 구성
	CImageBox*				m_pDiaImg[4];						//마름모 이미지

	CScrollBar*				m_pScrollBar;						//스크롤바

	CTextBox*				m_pSkillPointTxt;					//남은 스킬포인트-텍스트
	CTextBox*				m_pSkillPointNumTxt;				//남은 스킬포인트-넘버텍스트
	
	CTextBox*				m_pTotSkillPointTxt;				//총 스킬포인트-텍스트
	CTextBox*				m_pTotSkillPointNumTxt;				//총 스킬포인트-넘버텍스트
		
public:
	int 					m_SkillPointNum;					//남은 스킬포인트 넘버
	int						m_TotSkillPointNum;					//총 스킬포인트 넘버

	BOOL					m_InsertHighlight;	

public:
	CSkillWnd()
	{
		m_StartSlotNum = 0;
		m_SkillTabState	= nSkillTab_Active;	
		m_InsertHighlight = FALSE;
	}
	~CSkillWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );

	void UpdateSkillSlotItem();

	void SetSkillItem();
	void SetSkillSystem( CSkillSystem* _skillsystem );

	void InitPassiveSkillCode();
	void SetPassiveSkillCode( WORD aSkillCode );

private:
	
	CImageBox*		slotImg[10];									//슬롯 배경이미지	

	NSkillTabState			m_SkillTabState;						//스킬 탭 상태	
	
	SPcItem*				m_pSkill_Active[RP_MAX_USER_SKILL];
	SPcItem*				m_pSkill_Passive[RP_MAX_USER_SKILL];
	SPcItem*				m_pSkill_SubSkill[RP_MAX_USER_SKILL];

	SPcDataParam*			m_pPcParam;
	SPcDataInven*			m_pPcInven;
	
	CSkillSystem*			m_pSkillSystem;

	int						m_StartSlotNum;

	
	void SetSkillTabState( NSkillTabState aState );	
	void SetSlotScroll( int aLineNum );
	
	void SkillSlotClick( SEventMessage* EMsg );
	void SkillSlotDBClick( SEventMessage* EMsg );
	void SkillSlotDrag( SEventMessage* EMsg );
	void SkillSlotDrop( SEventMessage* EMsg );

	/*void SkillShortCutSlotClick( SEventMessage* EMsg );
	void SkillShortCutSlotDBClick( SEventMessage* EMsg );
	void SkillShortCutSlotDrag( SEventMessage* EMsg );
	void SkillShortCutSlotDrop( SEventMessage* EMsg );*/
	
	bool CompareSlot( CSlot* cslot, CSlot* tslot );

	bool CheckEnableLevelUpBtn( SPcItem* pPcItem );

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern	CSkillWnd	g_SkillWnd;

//-----------------------------------------------------------------------------
#endif  __RUI_SKILL_WNDPROC_H__

