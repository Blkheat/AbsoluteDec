// SkillLevelUpWnd.h: interface for the CSkillLevelUpWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLLEVELUPWND_H__A5F4FE5F_631B_4FC4_99BC_78F74CA97EAF__INCLUDED_)
#define AFX_SKILLLEVELUPWND_H__A5F4FE5F_631B_4FC4_99BC_78F74CA97EAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "StateButton.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"
#include "HelpItemTip.h"

#include "SkillWndProc.h"




class CSkillLevelUpWnd : public CWndProc
{
private:
	CFrameWnd*		m_pSkillLevelUpFWnd;

	CTextBox*		m_pTitle;

	CButton*		m_pCloseBtn;

	CImageBox*		m_pSkillIcon;

	CTextBox*		m_pLevelUpDesc;

	CButton*		m_pOKBtn;
	CButton*		m_pCancelBtn;

	// 현재 스킬 설명 
	CTextBox*		m_CurtSkillName;
	CTextBox*		m_CurtSkillDesc;
	CTextBox*		m_CurtSkillNeedItem;
	CTextBox*		m_CurtSkillLevel;
	CTextBox*		m_CurtSkillGageType;
	CTextBox*		m_CurtSkillGageValue;
	CTextBox*		m_CurtSkillWastePoint;		
	CTextBox*		m_CurtSkillDelay;
	CTextBox*		m_CurtSkillEffect[3];
	CTextBox*		m_CurtSkillAddEffect;



//	CTextBox*		m_CurtSkillProimaType;

	// 다음 레벨 스킬 설명 
	CTextBox*		m_NextSkillName;
	CTextBox*		m_NextSkillLeveOrPoint;
	CTextBox*		m_NextSkillDesc;
	CTextBox*		m_NextSkillNeedItem;
	CTextBox*		m_NextSkillLevel;
	CTextBox*		m_NextSkillGageType;
	CTextBox*		m_NextSkillGageValue;
	CTextBox*		m_NextSkillWastePoint;
	CTextBox*		m_NextSkillDelay;
	CTextBox*		m_NextSkillEffect[3];
	CTextBox*		m_NextSkillAddEffect;

//	CTextBox*		m_NextSkillProimaType;

	SPcItem*		m_pPcItem;

	CFontg*			m_pFontg;

	
	void			DrawCurtLvSkillInfo();
	void			DrawNextLvSkillInfo();
		
	
	int				GetNumLine( int aNumStr, int aCount );

	
public:
	CSkillLevelUpWnd(){}
	virtual ~CSkillLevelUpWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );


	//효과 3 개 리펙 
	bool GetEffectStrInfo(SPcItem* PcItem ,SSkillBaseInfo* SkillTable , TCHAR *StrInfo , int *effectType , int Num);
	bool GetEffectName(int effectType , TCHAR *StrName , int &SeeType , int Nation);
	
	bool GetAddEffectInfo(SPcItem* PcItem ,SSkillBaseInfo* SkillTable , TCHAR *StrInfo);
	bool GetAddEffectName(TCHAR *Str, int iAddEffect);

	void			Open( SPcItem* pPcItem );

	void	CompositionNew();
	void	CompositionOld();
	
	int GetActiveSKillLevel(SSkillBaseInfo* SkillTable , int StateLevel = 0);
	
};


extern	CSkillLevelUpWnd	g_SkillLevelUpWnd;


#endif // !defined(AFX_SKILLLEVELUPWND_H__A5F4FE5F_631B_4FC4_99BC_78F74CA97EAF__INCLUDED_)
