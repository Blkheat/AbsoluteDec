#if !defined( __RUI_SKILL_LEARN_PROC_H__ )
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


enum N_SKILLLEARN_SCROLLFOCUS
{
	n_SkillLearnListScroll = 0,
	n_SkillLearnDescScroll
};

typedef enum N_SKILLLEARN_LISTTYPE
{
	n_NotNeedLevel = 0, 
	n_LearnSkill, 
	n_NotNeedSkill, 
	n_BlankSkill

}; N_SKILLLEARN_LISTTYPE;


struct SSkillLearnField
{	
	N_SKILLLEARN_LISTTYPE	ListType;			

	WORD					SkillCode;
	WORD					NeedSkillCode_1;
	WORD					NeedSkillCode_2;
	DWORD					LernCost;

	int						NeedItemCode; 
	int						NeedItemNum;
	int						SkillPoint;

	CTextBox*				SkillName;		// 스킬명	
	CTextBox*				NeedSkill1;	// 필요 스킬 1 
	CTextBox*				NeedSkill2;	// 필요 스킬 2	
	CTextBox*				TxtLernCost;	//비용
	CTextBox*				TxtNeedSkillPoint;	//필요 스킬포인트 
	CTextBox*				TxtNeedItemName;	//필요 아이템 
	CTextBox*				TetNeedItemNum;		//필요 아이템 갯수

	TCHAR					Desc[512];		//설명
};


class CSkillLearnWnd : public CWndProc
{
public:
	CSkillLearnWnd()
	{
		StartListNum	= 0;
		SelectedBtn		= -1;

		ScrollFocus		= n_SkillLearnListScroll;
	}	
	~CSkillLearnWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	virtual void SetWheelValue( short _wheel );
	
	void SetClassSkillLearnInfo();
	void Open();
	
	void CompositionOld();
	void CompositionNew();


	
private:
	
	SSkillLessonInfo	theClassSkillLessonInfo[200];

	N_SKILLLEARN_SCROLLFOCUS ScrollFocus;	

	CTextBox*			ClassName;		// 직업명

	SSkillLearnField	SkillLearnField[4];
	
	//------------------------------------------------------------------------
	CButton*			SelBtn[4];		

	CButton*			LearnBtn;

	CScrollBar*			ListScroll;			
	CScrollBar*			DescScroll;		

	CTextBox*			SkillDescText;	
	//------------------------------------------------------------------------

	int					StartListNum;
	int					SelectedBtn;
	int					LearnedSkillCode;


	void SetSkillSelect( WORD aSkillCode );
	void SetSkillField();
	void SetSkillListScroll( int _linenum );
	void SetDecsTextScroll( int _linenum );

	void SkillBntSelected( int aNumBtn );
	
	void SetSkillLearnField( );

	bool IsEnableSkillLearn( BYTE aMainClass, WORD aNeedLearnClass );
};


extern	CSkillLearnWnd	SkillLearnWnd;

//-----------------------------------------------------------------------------
#endif  __RUI_SKILL_LEARN_PROC_H__