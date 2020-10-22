#if !defined( __RUI_CHARSTATE_WNDPROC_H__ )
#define __RUI_CHARSTATE_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "UpGauge.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Spin.h"
#include "Wnd.h"


#include "WndProc.h"


class CCharStateWnd : public CWndProc
{
protected:
	UINT			m_WIDToolTipDestWnd;
	float			m_fCumulTime;
	float			m_fRatio;
	CFontg*			theFontg;
public:
	CCharStateWnd()
	{
		m_WIDToolTipDestWnd = WID_None;
		m_fCumulTime = 0.0f;
		m_fRatio = 0.0f;

		theFontg = NULL;

		m_TxtPKPoint = NULL;
	}
	~CCharStateWnd(){}

	virtual void Init();
	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );
	
	
	SPcDataParam* 	thePcParam;
	
	///--@@@@ CUpGauge*	UpGage;
	CImageBox*	HighlightImg;
	CImageBox*	NationImg[2];
	



	//상태창
	//WID_Stat_Xprism		
	CImageBox*		IconImg;//상태아이콘
	CTextBox*		StatText;//상태
	CTextBox*		FameText;//명성
	CTextBox*		NameText;//이름
	CTextBox*		StateText;//국가
	CTextBox*		ContributionText;//국가 공헌도
	CTextBox*		GenderText;//성별
	CTextBox*		GuildText;//길드
	CTextBox*		ProfessionText;//직업
		
	CTextBox*		HPText;
	CTextBox*		MPText;
	CTextBox*		SPText;
	CProgressBar*	HPGauge;
	CProgressBar*	MPGauge;
	CProgressBar*	SPGauge;
	CProgressBar*	ExpGauge;

	CTextBox*		PowerText;//파워
	CTextBox*		VitalText;//체력
	CTextBox*		SympathyText;//교감
	CTextBox*		IntText;//지능 
	CTextBox*		StaminaText;//지구력 
	CTextBox*		DexText;//민첩 	
	CTextBox*		CharismaText;//매력
	CTextBox*		LuckText;//행운

	CTextBox*		PhysicAttText;//물리공격력
	CTextBox*		PhysicDefText;//물리방어력
	CTextBox*		MagicDefText;//마법방어력

	CTextBox*		MagicAttText2;//마법공격력
	CTextBox*		MagicDefText2;//마법방어력
	CTextBox*		PhysicDefText2;//물리방어력

	
	
	CTextBox*		HitText;//명중
	CTextBox*		EvasionText;//회피
	CTextBox*		MagicUSpeedText;//시전속도
	CTextBox*		MagicSRateText;//시전성공률
	CTextBox*		LevelText;//레벨

	CTextBox*		AbilityUpPointText;//능력업 포인트
	CTextBox*		SkillUpPointText;//스킬업 포인트
	CTextBox*		MoveSpeedText;//이동속도 포인트
	CTextBox*		WeightText;//체중
	
	///-- SubClass
	CTextBox*		SubClass;	///-- 보조직업
	CTextBox*		SubExp;		///-- 보조직업의 경험치
	CTextBox*		ExpText;	///-- 보조직업의 경험치


	CFrameWnd*		pMainFrame;// 메인 프레임
	CTextBox*		Static_Text[25];	///-- Static Text

	CTextBox*		WarRecord;
	DWORD	NeedExp;
	DWORD   CurExp;

	///-- 중국 PK관련 부분
	CTextBox*		m_TxtPKPoint;

	void	SetCitizen(int citizen);	

	void SetToolTipDestWnd( UINT WIDToolTipDestWnd );
	void RenderToolTipName();


	void GetAttackSpeedStr( TCHAR* aAttackSpeedStr, int aAttackSpeed );
	void GetMoveSpeedStr( TCHAR* aMoveSpeedStr, int aMoveSpeed );
	DWORD FameColor(DWORD Fame); 

private:

	int m_Citizen;			  // 국가
};


extern	CCharStateWnd	CharStateWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_CHARSTATE_WNDPROC_H__