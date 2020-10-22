// PetInfoWnd.h: interface for the CPetInfoWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__RUI_PETINFO_WND_H__)
#define __RUI_PETINFO_WND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FlashFrameWnd.h"
#include "CharImgButton.h"
#include "WndProc.h"




class CPetInfoWnd  : public CWndProc
{


public:
	CPetInfoWnd(){};
	virtual ~CPetInfoWnd(){};

	CFlashFrameWnd* FlashFrame;
	
	CCharImgBox * m_pChaImgBox;
	
	CImageBox* GlowImg;
	CFrameWnd* m_MenuFrame;
	
	CFrameWnd* m_KeepUpFrame;


	CTextBox*	Level;     
	CTextBox*	Name;
	CProgressBar* HP;   //ü�� 
	CProgressBar* CP;	//���ŷ� 

	CButton*	m_pCallOffBtn; //��ȯ���� 
	CButton*	m_pPetInvenBtn; // �� �κ� 
	CButton*	m_pPetEmotionBtn; // �� �̸�� 
	CButton* m_PetMenubtn; //����� �Ŵ� 
	
	CTextBox* HPText;
	CTextBox* CPText;
	


	//==============================================================================
	virtual void Init();
	virtual void Composition();
	void CompositionOld();
	void CompositionNew();
	DWORD GetTimePetPayEffect(DWORD Code);
	void GetTimePetEffectStr(DWORD Code , TCHAR *str);
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	//==============================================================================
	
	/********************************************************************
	- ���� ��ų 
	*********************************************************************/
	CSlot* m_pKeepupItemSlot[RP_MAX_PET_USEDITEM];
	CTextBox* m_pKeepupItemText[RP_MAX_PET_USEDITEM];
		


	//////////////////////////////////////////////////////////////////////////
	void Open(int PetSlot);
	void Close();
	
	void ClearPetInfo();


	bool m_IsMenuVisible;
	

	
	SPcDataInven*	PcInven;
	int m_CurPetNum;
	long m_lPrevTime;


	void MenuComPosition();


};

extern CPetInfoWnd g_PetInfoWnd;

#endif __RUI_PETINFO_WND_H__
