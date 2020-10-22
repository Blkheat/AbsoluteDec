// PetMainWndProc.h: interface for the CPetMainWndProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__RUI_PETMAIN_WNDPROC_H__)
#define __RUI_PETMAIN_WNDPROC_H__

//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#define MAX_PET_NAME 32

#include "RockPCH.h"
#include "RockClient.h"
#include "CharImgButton.h" 
#include "WndProc.h"

#define MERCE_SLOT_NUM 3		///-- 용병 슬롯 번호


#define MAX_PET_SLOT_NUM 4

enum NAbilityType
{
	n_atAttacker = 0 ,
	n_atDefence,
};

struct SPatternName
{
	TCHAR cAbility[MAX_PET_NAME];
	TCHAR cProbability[MAX_PET_NAME];
	TCHAR cTypeName[MAX_PET_NAME];

	SPatternName()
	{
		memset(&cAbility,NULL,sizeof(cAbility));
		memset(&cProbability,NULL,sizeof(cProbability));
		memset(&cTypeName,NULL,sizeof(cTypeName));
	};
};

struct SPetText
{
	CCharImgBox *	pChaImgBox;				// 펫 얼굴
	CTextBox*		NameTxt;				// 이름	
	CTextBox*		LevelTxt;				// 레벨    
	CTextBox*		TypeTxt;				// 종류   
	CTextBox*		HpTxt;					// 활력 
	CTextBox*		CpTxt;					// 정신력  
	CTextBox*		ExpTxt;					// 경험치 
	CTextBox*		AbilityTxt;				//능력
	CTextBox*		ProbabilityTxt;			//확율 ..
	CTextBox*		NameAbilityTxt;			//능력
	CTextBox*		NameProbabilityTxt;		//확율 ..
	CTextBox*		PetDieTimeTxt;			//펫 사망 
	CTextBox*		pRemainTimeTxt;			//시간제 - 남은시간

	CImageBox*		ImgReCall;
	CImageBox*		ImgInven;

	CButton*		ReCallBtn;				//소환
	CButton*		CallOffBtn;				//소화해제 
	CButton*		InvenBtn;				//소지품 정보 	

	CSlot*	KeepupItemSlot[RP_MAX_PET_USEDITEM];
	CTextBox* KeepupItemText[RP_MAX_PET_USEDITEM];
	float	fRemainTime[RP_MAX_PET_USEDITEM];

};

class CPetMainWndProc : public CWndProc  
{
public:
	//==============================================================================
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	//==============================================================================
	CFrameWnd*		m_pFrameWnd;	//펫정보창 
	CButton*		m_pCloseBtn;		
	//==============================================================================
	//소환 해제 창 
	CFrameWnd*		m_pFrameOffWnd; 
	CButton*			m_OkBtn;
	CButton*			m_CancelBtn;
	//================================================================
	int CurPetBtn;

	void Open();
	void Close();
	void ClearPetMainWnd();
	void ClearPetData();
	void CreatePet();
	void LogOff();
	void PetUpdateState();
	void CheckCurState(int PetSlot);

	void SetKeepUpItemCode( int aNum, SPayEffect aPayEffect , int PetSlotNum );
	void DelPayEffect( SPayEffect aPayEffect , int PetSlotNum);
	void SetPayEffect( SPayEffect aPayEffect ,int PetSlotNum);
	void SortKeepupItem(int PetSlotNum);
	void DeleteKeepUpItemCode( int aNum ,int PetSlotNum);
	
	SPetText m_PetText[MAX_PET_SLOT_NUM]; //펫 저장공간 3개로 제한둔다 이것은 안늘어 나는것이여 ~~
	SPcDataInven*	PcInven;
	SPatternName m_TypeName[2]; 	//공격형 , 방어형 두가지로 나뉜다 .. 
	
	CPetMainWndProc(){}
	virtual ~CPetMainWndProc(){}

};

class CPetCreateWnd : public CWndProc  
{
public:

	TCHAR PetName[256];
	int	BufLen;
	//==============================================================================
	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	//==============================================================================
	CFrameWnd*		m_pFrameWnd;			
	CButton*			m_OkBtn;
	CButton*			m_CancelBtn;
	CTextBox*			m_EffectText;
	CEditBox*		m_NameEdit;
	
	void Open();
	void Close();
	int IsCheckInSpace(void *Str);
	bool CreateNameAndVerification();
	bool IsCheckSymbol(void *SrcStr ,void *DstStr);

	CPetCreateWnd();
	virtual ~CPetCreateWnd(){}
};

extern CPetMainWndProc g_PetMainWndProc;
extern CPetCreateWnd	g_PetCreateWnd;

#endif __RUI_PETMAIN_WNDPROC_H__
