#if !defined( __RUI_PLAYERINFO_WNDPROC_H__ )
#define __RUI_PLAYERINFO_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "WndProc.h"
#include "ComboStepBar.h"
#include "FlashFrameWnd.h"


class CPlayerInfoWnd : public CWndProc
{
public:
	CFlashFrameWnd*	FlashFrame;
	
	CImageBox*	    GlowImg;
	CImageBox*	    GlowHPImg;	

	CImageBox*		CharImg;
	CTextBox*		Level;
	CTextBox*		Name;

	CProgressBar*   PrevHP;
	CProgressBar*	HP;
	CProgressBar*	SPMP;	
	CTextBox*		HPText;
	CTextBox*		SMPText;	
	
	CComboStepBar*  pComboStepBar;
	CComEffImgBox*	pComEffimg[6];

	CButton *		m_pFocusBox;
	
	CButton	*		pMyMenuBtn;
	
	//by simwoosung
	int				m_iPrevHP;				//���� HP
	int				m_iDamage;				//����� ��

	//by dongs
	CFrameWnd*	FrameWndTotem;
	CTextBox*	Score;
	CFrameWnd*	FrameWndFameInDunTime;
	CTextBox*	FameInDunTimeText;
	
private:
	POINT			m_pOldPos;
	BOOL			m_BStartEffect;         //����Ʈ ���ۿ���	
	long			m_lPrevTime;			//���� Ÿ��
	
	float			m_fGlowTime;			//�۷ο� Ÿ��
	float			m_fFadeTime;			//���̵� Ÿ��
	float			m_fBarAlpha;			//�� ���İ�	
	float			m_fCumulTime;			//���̵� ���� Ÿ��

public:

	void Start2DEffect(int iHP, int iRealHP);
	void Start2DEffect(int iHP, _STargetInfo aTargetInfo, WORD aSkillCode);

public:

	CPlayerInfoWnd();

	virtual void Init();
	virtual void Composition();
	void CompositionNew();

	virtual void Update();
	void UpdateNew();

	virtual void Proc( SEventMessage* EMsg );
	
	void SetKeepUpSkillCode(int Num,DWORD SkillCode);
	void DeleteKeepUpSkillCode(int Num, WORD aSkillCode);
	void DeleteKeepUpDirectSkillCode(WORD aSkillCode);
	void DeleteAllKeepUpSkillCode();	

	void SortKeepupSkill();
	void SetKeepupSkill( WORD _skillcode );
	void DelKeepupSkill( WORD _skillcode );

	void SetKeepUpItemCode( int aNum, SPayEffect aPayEffect );
	void DeleteKeepUpItemCode( int aNum );
	void SortKeepupItem();
	void SetPayEffect( SPayEffect aPayEffect );
	void DelPayEffect( SPayEffect aPayEffect );
	void SetCheckPayTime(int aNum , SPayEffect aPayEffect );

	
	void RemainTimeShow();
	//--��ġ�δ��ð� 
	void ShowFameInDun();
	void InitFameInDun();


	SPcDataParam* 	thePcParam;
	SPcDataInven*	thePcInven;

	SPcItem			pcitem[RP_MAX_KEEPUP_SKILL];

	CSlot		   *theKeepupSkillSlot[RP_MAX_KEEPUP_SKILL];

	SPcItem			m_KeepupItem[ RP_MAX_PAY_CONTENTS ];

	CSlot		*m_pKeepupItemSlot[ RP_MAX_PAY_CONTENTS ];
	CTextBox	*m_pKeepupItemText[RP_MAX_PAY_CONTENTS ];


	CImageBox * PayImgBg[6];
	void			KeepVisible(BOOL IsVisible);
	void			LogOff();

};



extern	CPlayerInfoWnd	PlayerInfoWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_PLAYERINFO_WNDPROC_H__