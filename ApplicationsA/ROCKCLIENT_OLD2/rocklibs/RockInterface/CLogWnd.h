#ifndef __CLOGWND_H
#define __CLOGWND_H

#include "WndProc.h"
#include "FrameWnd.h"
#include "RadioButton.h"
#include "ScrollBar.h"
#include "..\\..\\bravolibs\obj\ActionSystem.h"

#include <string>
#include <vector>

#define COMBO_LIST_MAXCOUNT		20
#define SULOG_LIST_MAXCOUNT     4

typedef struct s_COMBO_LOG_DATA
{
	BYTE		 Index;
#ifdef UNICODE_ROCK
	std::wstring  StrSequence;
	std::wstring  StrDamage;
#else
	std::string  StrSequence;
	std::string  StrDamage;
#endif
	
}COMBO_LOG_DATA,*LPCOMBO_LOG_DATA;

typedef struct s_COMBO_BONUS_DATA
{

	BYTE	Index;
	float	Bonus;
	WORD	SkillCode;

}COMBO_BONUS_DATA,*LPCOMBO_BONUS_DATA;

typedef struct s_COMBO_BONUS_INFO
{

	COMBO_BONUS_DATA	Info[c_Max_ComboNum];

}COMBO_BONUS_INFO,*LPCOMBO_BONUS_INFO;

typedef struct s_SUB_LOG_DATA
{

	int         Level;
	int         Type;
#ifdef UNICODE_ROCK
	std::wstring StrLine[7];
#else
	std::string	StrLine[7];
#endif
	

	s_SUB_LOG_DATA()
	{
		Level = 0;
		Type  = -1;
	}

}SUB_LOG_DATA,*LPSUB_LOG_DATA;

class CLogWnd : public CWndProc
{
typedef
	enum DisplayType  { COMBO_TAB , SUBCLASS_TAB };

public:

	 CLogWnd();
	~CLogWnd();

public:
	CFrameWnd		*m_pFrame;
	
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:

	void	ClearLog(void);
	void	Open(void);
	void	SetCombo(int Num,BOOL ISCheck);
	void	InitComboTextList(void);
	void	InitSubtextList(void);
	void	LoadData(void);
	void	ResetLog(char *pData);

	void	ComboControl(void);
	void	SubJobControl(void);

	void	SetSavedLog(int aSlot, BYTE ISOn = TRUE );
	void    SetComboHotKey(int Num);
	void	SetSubTabType(void);
	
	void	SetWheelValue( short _wheel );
	
	WNDID	GetFrameID(void)	{ return m_pFrame->GetID();}
	
	COMBO_LOG_DATA *GetComboLogData(int SlotNum);
	int				GetComboLogSize();
	BYTE			GetSavedLog(int aSlot);

	bool	CheckComboEnable( int aIndex );
	
	//by simwoosung
	int		GetComboTotCount(int Num);
	void	SetEnableCombo(int nClassType);
	void    SetComboRegKey(int nNum, int nCount = 0);	
	
private:
	
	void	ComboSelect(int Num);
	void	ComboKeyReg(void);
	void	ComboKeyCancel(void);
	
	void    SetJobLevel(int Num);
	void    SetJobType(int Num);
	int		GetEldaStoneLevel(int Value);
	void	SetSubSort(void);
	int		SubSortLevel(int Level);
	int		SubSortType(int Type);
	int		IsAddTable(WORD Code);
	int		IsAddTableType(int Type );

public:

	char			 m_theSaveLog[70];

private:
	
	CRadioButton	*m_pComboTab;
	CRadioButton	*m_pSubClassTab;
	
	CScrollBar		*m_pComboScroll;
	CScrollBar		*m_pSubJobScroll;

	CTextBox*		TComboLabel0;
	CTextBox*		TComboLabel1;

	CTextBox*		TComboContext[COMBO_LIST_MAXCOUNT][2];
	CButton*		m_pComboBtnList[COMBO_LIST_MAXCOUNT];

	CImageBox*		m_pComboSelectImg;
	CImageBox*		m_pComboRegImg;
	CImageBox*		m_pLineImg[2];
	CImageBox9*		m_img9;
	

	CButton*		m_pComboRegBtn;
	CButton*		m_pComboRegCancelBtn;
	CButton*		m_pOKBtn;

	CButton*		m_pCloseBtn;

	CRadioButton	*m_SubTab[3];
	CTextBox*		 m_SubTabTextBox[3];

	CRadioButton	*m_pLevelBtn[4];
	CTextBox		*m_pLevelImage;
	CTextBox		*m_pLevelTextBox[4];

	CTextBox*		TSubContext[SULOG_LIST_MAXCOUNT][7];

private:

	DisplayType						m_DisplayType;					

	std::vector	<COMBO_LOG_DATA>	m_ComboTextList;
	std::vector <COMBO_BONUS_INFO>	m_ComboBonusList;

	int								m_ComboTopNum;
	int								m_ComboSelectNum;

	std::vector <SUB_LOG_DATA>		m_SubLogData;
	std::vector <int>				m_SubSortList;

	int								m_SubTopNum;
	
	int								m_SubTabType[3];
	int								m_SubTabNum;
	int								m_SubLevel;
	
	bool							m_bEnableCombo;
};

extern	CLogWnd		g_LogWnd;

#endif
