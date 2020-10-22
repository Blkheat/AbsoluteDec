// ActionSystem.h: interface for the CActionSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIONSYSTEM_H__0C090E59_CF3B_4DDE_9169_9329460CCCBA__INCLUDED_)
#define AFX_ACTIONSYSTEM_H__0C090E59_CF3B_4DDE_9169_9329460CCCBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <string.h>
#include <..\\rocklibs\\RockInterface\\ComEffProgBar.h>
#include <..\\rocklibs\\RockInterface\\ComboStepBar.h>
#include <..\\rocklibs\\RockInterface\\Button.h>

// �޺� ������ ���� 

/*
const int c_ComboZKey1( 101 );
const int c_ComboZKey2( 102 );
const int c_ComboZKey3( 103 );
const int c_ComboZKey4( 104 );
const int c_ComboZKey5( 105 );

const int c_ComboXKey1( 201 );
const int c_ComboXKey2( 202 );
const int c_ComboXKey3( 203 );
const int c_ComboXKey4( 204 );
const int c_ComboXKey5( 205 );

const int c_ComboCKey1( 301 );
const int c_ComboCKey2( 302 );
const int c_ComboCKey3( 303 );
const int c_ComboCKey4( 304 );
const int c_ComboCKey5( 305 );
*/
const int c_Max_ComboType( 6 );
const int c_Max_ComboNum( 20 );
const int c_Max_ComboSeqNum( 8 );


/*
typedef enum NComboType
{
	nComboType_TwoHand_Sword = 0,		// ��� 
	nComboType_OneHand_Sword,			// �Ѽհ� 
	nComboType_Sword_Shield,			// �� + ���� 
	nComboType_CrystalBall, 			// ������ 
	nComboType_MagicBook, 				// ������ 
	nComboType_Staff					// ������ 

} NComboType;
*/

typedef enum NCombo_KeyType
{
	n_Z_Key = 0,
	n_X_Key,
	n_C_Key	
} NCombo_KeyType;

typedef enum NCombo_StateType
{	
	n_Combo_Enable = 0,
	n_Combo_Success,
	n_Combo_Disable,
	n_Combo_Skill_Enable

} NCombo_StateType;

typedef enum NChant_StateType
{	
	n_Chant_Cancel = 0,
	n_Chant_Start,
	n_Chant_Success,
	n_Chant_Fail,
	n_Chant_Complete
} NChant_StateType;


// Ȥ�� �ʿ��ϰ� �� �� �� �޺� ��ų ��ü�� ���� ����
//	typedef struct SSkill_Info
//	{
//		int			gauge_point;		// �� ��ų �޺� ������ ���� �Ǵ� ������ ����Ʈ 
//		int			skill_index;		// ��ų �ε��� 
//	} SSkill_Info;

typedef struct SCombo_Info
{
	int			combo_index[c_Max_ComboNum];
	int			combo_sequence[c_Max_ComboNum][c_Max_ComboSeqNum];
	bool		combo_enable[c_Max_ComboNum];
	

	SCombo_Info()
	{
		memset( combo_index,	0, sizeof(int) * c_Max_ComboNum );
		memset( combo_sequence, 0, sizeof(int) * c_Max_ComboNum * c_Max_ComboSeqNum );
		memset( combo_enable,	1, sizeof(bool) * c_Max_ComboNum );
	}

} SCombo_Info;


class CActionSystem  
{
private:
    BOOL				m_bActAutoCombo;				///-- �ڵ��޺� �۵� ����
	
	D3DXVECTOR3			m_vPlayerPos;
	D3DXVECTOR3			m_vOldPlayerPos;
	
	D3DXMATRIX			m_Matchar;

public:	
	void				SetActAutoCombo(BOOL bFlag)		{ m_bActAutoCombo = bFlag; }
	BOOL				IsActAutoCombo()				{ return m_bActAutoCombo; }
	void				ComboAutoSystem();

	///-- By siwoosung ���� ����Ű�� �����ϰ� �װ��� ������
	char				ApllyAndGetRandomKey();
	int					GetRandomComboAniIndex( int &aSequenceNum, int aPressKey );

public:

	SCombo_Info			theCombo_Info[c_Max_ComboType];
 
	int					theCurtComboType;				// ���⿡ ���� �޺� Ÿ�� 	
	int					theSuccessComboKeyType;			// ���� ��Ų �޺� Ÿ���� �����Ѵ� 
	int					theSuccessComboCnt;				// ���� ��Ų �޺� Ƚ���� �����Ѵ� 
	int					theSuccessComboStatus;			// �޺� ���� ���� �Ǵ�
	int					theSuccessComboSeq;				// �޺� ���� ������ 

	int					theCurtChantType;				// ��â Ÿ�� 
	int					theSuccessChantCnt;				// ���� ��Ų ��â Ƚ���� �����Ѵ� 
	int					theSuccessChantStatus;			// ��â ���� ���� �Ǵ�
	
	int					theSpellingTime;				// ��â �ð� 
	long				thePreTime;						// Ÿ�� �ٸ� ���� �ð� üũ 

	///-- By simwoosung
	int					theArcherTime;
	int					theSuccessArcherCnt;
	int					theArcherSpellTime;
	
	///-- By simwoosung -- ����Ű ���� ����
 	int					theTotComboStep;				//�� �����޺� ����
	int					theNowRandomKey;				//���� ����� ����Ű	
	
public:
	int					theComboLevel;					// 1, 2, 3, 4, 5, EX 
	int					theComboGauge;					// 1,1,1,1,1   1,1,1,1,1   ... �� 5���� 6ĭ 
	WORD				theComboSkillCode0;				// ��� �޺��� �ϱ� ���� '��˼�' ��ų �ڵ尪 
	WORD				theComboSkillCode1;				// Ȱ 
	WORD				theComboSkillCode2;				// �� �޺��� �ϱ� ���� '�˼�' ��ų �ڵ尪 
	WORD				theComboSkillCode7;				// ������ �޺��� �ϱ� ���� '�����̸�' ��ų �ڵ尪
	
	int					theProimaLevel;					// 1, 2, 3, 4, 5, EX 
	int					theProimaGauge;					// 1,1,1,1,1   1,1,1,1,1   ... �� 5���� 6ĭ 
	WORD				theChantSkillCode;				// ��â�� �ϱ� ���� '�����̸��� ����' ��ų �ڵ尪 
		
	int					theChantTime;					// TimeBar 

	// ���� ���̽� ��Ʈ�� 
//	CProgressBar*		theComboGaugeInfo_level;		// �������̽� ���� ǥ��  
//	CComEffProgBar*		theComboGaugeInfo_gauge;		// �������̽� ������ ǥ��
	CComboStepBar*		theComboStepInfo_gauge;			// �޺��� ǥ��


	CProgressBar*		theChantTimeBar;				// ��â Ÿ�� �� 
	CImageBox*			theChantCompleteImg;			// ��â ����  
	CButton*			theChantPoint1;					// ��â ����Ʈ 1
	CButton*			theChantPoint2;					// ��â ����Ʈ 2
	CButton*			theChantPoint3;					// ��â ����Ʈ 3
	CButton*			theChantPoint4;					// ��â ����Ʈ 4
	CButton*			theChantPoint5;					// ��â ����Ʈ 5
	
	DWORD				theUseSkillCode;				// ��뽺ų�ڵ�

public:
	CActionSystem();
	virtual ~CActionSystem();

	// �� �� ī��Ʈ ��� By wxywxy
	int			GetComboCnt(void)			{ return theSuccessComboCnt; }
	void		SetComboCnt(int nCount)		{ theSuccessComboCnt = nCount; }
	// �� �� ���� ��� By simwoosung
	int			GetComboStatus()			{ return theSuccessComboStatus; }
	bool		IsEnableComboSkill();		//�޺���ų�� �������� ����
	int			GetEnableSkillLevel();		//��밡���� ��ų���� ����Լ�
	void		InitData();

	bool		ReadComboSeqeunce( const char* file_pos );

	void		ActionProc();
	void		ActionProcChantCombo();
	
	void		ComboSystem( int aPressKey );
	void		TimingHitSystem();
	void		ChantComboSystem( bool aChantCombo );
	void		ArcherComboSystem();
	
	int			GetComboLevel(void);
	int			GetComboLevel( int aComboType, bool IsArcher = false );
	int			GetChantLevel( int aSpellingTime );


	// �޺� �ý��� ComboSystem
	int			InitComboEnable();										// c_MaxComboNum ��ŭ ���� ����� �� �ִ� �޺� �������� enable ��Ų��.
	int			GetComboAniIndex( int &aSequenceNum, int aPressKey );	// ������ Ű�� �ش� �������� �޺� ���� ���� �Ǵ��Ͽ� ������ �������� �´� �ִϸ��̼ǰ��� ��´� 
	bool		SetComboType();										// �޺� Ÿ�� ���� (���⿡ ���� �޺� Ÿ�� ����)
	int			GetAnimationIndex( int aComboKey );
	int			GetUseSP();
	int			GetNowSP();
	void		SetComboGauge();
	
	// ��â �ý��� ChantComboSystem
	void		InitChantEnable();
	void		SetChantState( int aLevel, int aChantTime );			// ��â ����Ʈ Ÿ�ӹٿ� ���� ��ȭ 
	void		SetProimaGauge();
	void		SetConditionGauge();
	int			SetRandomTypeChantPercent( int aChantPercent, float aTimeRate );		// ��â Ÿ�Կ� ���� ����� ��ȭ 
	
	///-- ������ �� �ý��� ��������	
	///-- By simwoosung
	void		InitArcherEnable();	
	
	
	int			GetSkillLevel( WORD aSkillCode );						// �ڵ尪�� ������ ������ �����ϴ� ��ƾ ���� 
	int			GetNeedGageLevel( BYTE aNeedGage ); 

};

#endif // !defined(AFX_ACTIONSYSTEM_H__0C090E59_CF3B_4DDE_9169_9329460CCCBA__INCLUDED_)



