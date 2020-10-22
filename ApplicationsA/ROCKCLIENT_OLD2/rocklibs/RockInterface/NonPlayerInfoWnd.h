// NonPlayerInfoWnd.h: interface for the CNonPlayerInfoWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NONPLAYERINFOWND_H__BD8C0F60_8C53_43B0_A7B0_E51A9C5CE071__INCLUDED_)
#define AFX_NONPLAYERINFOWND_H__BD8C0F60_8C53_43B0_A7B0_E51A9C5CE071__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "WndProc.h"

struct SkillField
{
	DWORD SkillCode;
	int	  KeepupSkillPos;

	SkillField()
	{
		memset( this, 0, sizeof(SkillField) );
	}
};

class CNonPlayerInfoWnd : public CWndProc  
{	
public:
	CFrameWnd*  theFrame;
	
	CImageBox*	theGlowImg;
	CImageBox*	theGlowHPImg;	

	int				thePickedNpcID;
	int				thePickedUserID;

	CTextBox*		theLevel;
	CTextBox*		theName;
	CProgressBar*	thePrevHP;
	CProgressBar*	theHP;
	CProgressBar*	theSMP;
	
	CTextBox*		theHPText;
	CTextBox*		theSMPText;


	BOOL			 m_ISDisplay;
	Character		*m_pSelectSrc;
	Character		*m_pSrc;
    
    int				m_iPrevHP;				//���� HP
	int				m_iDamage;				//����� ��

private:
	POINT			m_pOldPos;
	BOOL			m_BStartEffect;         //����Ʈ ���ۿ���	
	long			m_lPrevTime;			//���� Ÿ��
	
	float			m_fGlowTime;			//�۷ο� Ÿ��
	float			m_fFadeTime;			//���̵� Ÿ��
	float			m_fBarAlpha;			//�� ���İ�	
	float			m_fCumulTime;			//���̵� ���� Ÿ��
	
	CButton *		m_pAddButton;					

public:	
	void Start2DEffect(int iHP, int iRealHP);
	void Start2DEffect(int iHP, _STargetInfo aTargetInfo, WORD aSkillCode);

public:

	CNonPlayerInfoWnd();
	virtual ~CNonPlayerInfoWnd();

	virtual void Init();
	virtual void Composition();
	void CompositionOld();
	void CompositionNew();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	void SetPickCharacter(Character *pChar);
	
	void DeleteAllKeepUpSkill(void);
	void DeleteKeepUpSkillCode(int Num);	
	void SetKeepUpSkillCode(int Num,DWORD SkillCode , int HolyLevel = 0);
	BOOL IsItemKeepSkill(DWORD SkillCode);
	BOOL IsNotViewKeepSkill(DWORD SkillCode);

	SPcItem			pcitem[RP_MAX_KEEPUP_SKILL];
	//	CImageBox*		theKeepupSkillImg[10];
	CSlot*			theKeepupSkillSlot[RP_MAX_KEEPUP_SKILL];
	SkillField		theKeepupSkillField[RP_MAX_KEEPUP_SKILL];	

};

extern CNonPlayerInfoWnd NonPlayerInfoWnd;


#endif // !defined(AFX_NONPLAYERINFOWND_H__BD8C0F60_8C53_43B0_A7B0_E51A9C5CE071__INCLUDED_)
