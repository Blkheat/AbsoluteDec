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

// 콤보 시퀀스 정의 

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
	nComboType_TwoHand_Sword = 0,		// 대검 
	nComboType_OneHand_Sword,			// 한손검 
	nComboType_Sword_Shield,			// 검 + 방패 
	nComboType_CrystalBall, 			// 수정구 
	nComboType_MagicBook, 				// 마법서 
	nComboType_Staff					// 스태프 

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


// 혹시 필요하게 될 지 모를 콤보 스킬 자체에 대한 정보
//	typedef struct SSkill_Info
//	{
//		int			gauge_point;		// 이 스킬 콤보 성공시 차게 되는 게이지 포인트 
//		int			skill_index;		// 스킬 인덱스 
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
    BOOL				m_bActAutoCombo;				///-- 자동콤보 작동 여부
	
	D3DXVECTOR3			m_vPlayerPos;
	D3DXVECTOR3			m_vOldPlayerPos;
	
	D3DXMATRIX			m_Matchar;

public:	
	void				SetActAutoCombo(BOOL bFlag)		{ m_bActAutoCombo = bFlag; }
	BOOL				IsActAutoCombo()				{ return m_bActAutoCombo; }
	void				ComboAutoSystem();

	///-- By siwoosung 현재 랜덤키를 적용하고 그것을 가져옴
	char				ApllyAndGetRandomKey();
	int					GetRandomComboAniIndex( int &aSequenceNum, int aPressKey );

public:

	SCombo_Info			theCombo_Info[c_Max_ComboType];
 
	int					theCurtComboType;				// 무기에 따른 콤보 타입 	
	int					theSuccessComboKeyType;			// 성공 시킨 콤보 타입을 저장한다 
	int					theSuccessComboCnt;				// 성공 시킨 콤보 횟수를 저장한다 
	int					theSuccessComboStatus;			// 콤보 성공 여부 판단
	int					theSuccessComboSeq;				// 콤보 성공 시퀀스 

	int					theCurtChantType;				// 영창 타입 
	int					theSuccessChantCnt;				// 성공 시킨 영창 횟수를 저장한다 
	int					theSuccessChantStatus;			// 영창 성공 여부 판단
	
	int					theSpellingTime;				// 영창 시간 
	long				thePreTime;						// 타임 바를 위한 시간 체크 

	///-- By simwoosung
	int					theArcherTime;
	int					theSuccessArcherCnt;
	int					theArcherSpellTime;
	
	///-- By simwoosung -- 랜덤키 적용 변수
 	int					theTotComboStep;				//총 랜덤콤보 스텝
	int					theNowRandomKey;				//현재 적용된 랜덤키	
	
public:
	int					theComboLevel;					// 1, 2, 3, 4, 5, EX 
	int					theComboGauge;					// 1,1,1,1,1   1,1,1,1,1   ... 총 5개씩 6칸 
	WORD				theComboSkillCode0;				// 대검 콤보를 하기 위한 '대검술' 스킬 코드값 
	WORD				theComboSkillCode1;				// 활 
	WORD				theComboSkillCode2;				// 검 콤보를 하기 위한 '검술' 스킬 코드값 
	WORD				theComboSkillCode7;				// 수정구 콤보를 하기 위한 '프로이마' 스킬 코드값
	
	int					theProimaLevel;					// 1, 2, 3, 4, 5, EX 
	int					theProimaGauge;					// 1,1,1,1,1   1,1,1,1,1   ... 총 5개씩 6칸 
	WORD				theChantSkillCode;				// 영창를 하기 위한 '프로이마의 이해' 스킬 코드값 
		
	int					theChantTime;					// TimeBar 

	// 인터 페이스 컨트롤 
//	CProgressBar*		theComboGaugeInfo_level;		// 인터페이스 레벨 표시  
//	CComEffProgBar*		theComboGaugeInfo_gauge;		// 인터페이스 게이지 표시
	CComboStepBar*		theComboStepInfo_gauge;			// 콤보값 표시


	CProgressBar*		theChantTimeBar;				// 영창 타임 바 
	CImageBox*			theChantCompleteImg;			// 영창 성공  
	CButton*			theChantPoint1;					// 영창 포인트 1
	CButton*			theChantPoint2;					// 영창 포인트 2
	CButton*			theChantPoint3;					// 영창 포인트 3
	CButton*			theChantPoint4;					// 영창 포인트 4
	CButton*			theChantPoint5;					// 영창 포인트 5
	
	DWORD				theUseSkillCode;				// 사용스킬코드

public:
	CActionSystem();
	virtual ~CActionSystem();

	// 콤 보 카운트 얻기 By wxywxy
	int			GetComboCnt(void)			{ return theSuccessComboCnt; }
	void		SetComboCnt(int nCount)		{ theSuccessComboCnt = nCount; }
	// 콤 보 상태 얻기 By simwoosung
	int			GetComboStatus()			{ return theSuccessComboStatus; }
	bool		IsEnableComboSkill();		//콤보스킬이 가능한지 여부
	int			GetEnableSkillLevel();		//사용가능한 스킬레벨 얻기함수
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


	// 콤보 시스템 ComboSystem
	int			InitComboEnable();										// c_MaxComboNum 만큼 현재 사용할 수 있는 콤보 시퀀스를 enable 시킨다.
	int			GetComboAniIndex( int &aSequenceNum, int aPressKey );	// 눌려진 키로 해당 시퀀스에 콤보 성공 여부 판단하여 성공시 시퀀스에 맞는 애니메이션값을 얻는다 
	bool		SetComboType();										// 콤보 타입 정의 (무기에 따른 콤보 타입 설정)
	int			GetAnimationIndex( int aComboKey );
	int			GetUseSP();
	int			GetNowSP();
	void		SetComboGauge();
	
	// 영창 시스템 ChantComboSystem
	void		InitChantEnable();
	void		SetChantState( int aLevel, int aChantTime );			// 영창 포인트 타임바에 따라 변화 
	void		SetProimaGauge();
	void		SetConditionGauge();
	int			SetRandomTypeChantPercent( int aChantPercent, float aTimeRate );		// 영창 타입에 따른 진행률 변화 
	
	///-- 아쳐의 바 시스템 변수설정	
	///-- By simwoosung
	void		InitArcherEnable();	
	
	
	int			GetSkillLevel( WORD aSkillCode );						// 코드값을 가지고 레벨을 리턴하는 루틴 적용 
	int			GetNeedGageLevel( BYTE aNeedGage ); 

};

#endif // !defined(AFX_ACTIONSYSTEM_H__0C090E59_CF3B_4DDE_9169_9329460CCCBA__INCLUDED_)



