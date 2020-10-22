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

#define MERCE_SLOT_NUM 3		///-- �뺴 ���� ��ȣ


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
	CCharImgBox *	pChaImgBox;				// �� ��
	CTextBox*		NameTxt;				// �̸�	
	CTextBox*		LevelTxt;				// ����    
	CTextBox*		TypeTxt;				// ����   
	CTextBox*		HpTxt;					// Ȱ�� 
	CTextBox*		CpTxt;					// ���ŷ�  
	CTextBox*		ExpTxt;					// ����ġ 
	CTextBox*		AbilityTxt;				//�ɷ�
	CTextBox*		ProbabilityTxt;			//Ȯ�� ..
	CTextBox*		NameAbilityTxt;			//�ɷ�
	CTextBox*		NameProbabilityTxt;		//Ȯ�� ..
	CTextBox*		PetDieTimeTxt;			//�� ��� 
	CTextBox*		pRemainTimeTxt;			//�ð��� - �����ð�

	CImageBox*		ImgReCall;
	CImageBox*		ImgInven;

	CButton*		ReCallBtn;				//��ȯ
	CButton*		CallOffBtn;				//��ȭ���� 
	CButton*		InvenBtn;				//����ǰ ���� 	

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
	CFrameWnd*		m_pFrameWnd;	//������â 
	CButton*		m_pCloseBtn;		
	//==============================================================================
	//��ȯ ���� â 
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
	
	SPetText m_PetText[MAX_PET_SLOT_NUM]; //�� ������� 3���� ���ѵд� �̰��� �ȴþ� ���°��̿� ~~
	SPcDataInven*	PcInven;
	SPatternName m_TypeName[2]; 	//������ , ����� �ΰ����� ������ .. 
	
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
