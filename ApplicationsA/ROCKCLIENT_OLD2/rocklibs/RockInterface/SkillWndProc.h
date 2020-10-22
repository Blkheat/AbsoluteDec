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
	// ������ ���� ��Ʈ�� 
	CFrameWnd*				m_pSkillFWnd;

	CButton*				m_pCloseBtn;						//�ݱ� ��ư	
	
	CStateButton*			m_pActiveSBtn;						//��Ƽ�� ��
	CStateButton*			m_pPassiveSBtn;						//�нú� ��	   
	CStateButton*			m_pSubClassSBtn;					//�������� ��

	CImageBox9*				m_pInBgImg;							//���ι���̹���
	
	SSkillPack				m_SkillPack[c_Max_SkillSlot];		//��ų ���� ����
	CImageBox*				m_pDiaImg[4];						//������ �̹���

	CScrollBar*				m_pScrollBar;						//��ũ�ѹ�

	CTextBox*				m_pSkillPointTxt;					//���� ��ų����Ʈ-�ؽ�Ʈ
	CTextBox*				m_pSkillPointNumTxt;				//���� ��ų����Ʈ-�ѹ��ؽ�Ʈ
	
	CTextBox*				m_pTotSkillPointTxt;				//�� ��ų����Ʈ-�ؽ�Ʈ
	CTextBox*				m_pTotSkillPointNumTxt;				//�� ��ų����Ʈ-�ѹ��ؽ�Ʈ
		
public:
	int 					m_SkillPointNum;					//���� ��ų����Ʈ �ѹ�
	int						m_TotSkillPointNum;					//�� ��ų����Ʈ �ѹ�

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
	
	CImageBox*		slotImg[10];									//���� ����̹���	

	NSkillTabState			m_SkillTabState;						//��ų �� ����	
	
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

