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
	



	//����â
	//WID_Stat_Xprism		
	CImageBox*		IconImg;//���¾�����
	CTextBox*		StatText;//����
	CTextBox*		FameText;//��
	CTextBox*		NameText;//�̸�
	CTextBox*		StateText;//����
	CTextBox*		ContributionText;//���� ���嵵
	CTextBox*		GenderText;//����
	CTextBox*		GuildText;//���
	CTextBox*		ProfessionText;//����
		
	CTextBox*		HPText;
	CTextBox*		MPText;
	CTextBox*		SPText;
	CProgressBar*	HPGauge;
	CProgressBar*	MPGauge;
	CProgressBar*	SPGauge;
	CProgressBar*	ExpGauge;

	CTextBox*		PowerText;//�Ŀ�
	CTextBox*		VitalText;//ü��
	CTextBox*		SympathyText;//����
	CTextBox*		IntText;//���� 
	CTextBox*		StaminaText;//������ 
	CTextBox*		DexText;//��ø 	
	CTextBox*		CharismaText;//�ŷ�
	CTextBox*		LuckText;//���

	CTextBox*		PhysicAttText;//�������ݷ�
	CTextBox*		PhysicDefText;//��������
	CTextBox*		MagicDefText;//��������

	CTextBox*		MagicAttText2;//�������ݷ�
	CTextBox*		MagicDefText2;//��������
	CTextBox*		PhysicDefText2;//��������

	
	
	CTextBox*		HitText;//����
	CTextBox*		EvasionText;//ȸ��
	CTextBox*		MagicUSpeedText;//�����ӵ�
	CTextBox*		MagicSRateText;//����������
	CTextBox*		LevelText;//����

	CTextBox*		AbilityUpPointText;//�ɷ¾� ����Ʈ
	CTextBox*		SkillUpPointText;//��ų�� ����Ʈ
	CTextBox*		MoveSpeedText;//�̵��ӵ� ����Ʈ
	CTextBox*		WeightText;//ü��
	
	///-- SubClass
	CTextBox*		SubClass;	///-- ��������
	CTextBox*		SubExp;		///-- ���������� ����ġ
	CTextBox*		ExpText;	///-- ���������� ����ġ


	CFrameWnd*		pMainFrame;// ���� ������
	CTextBox*		Static_Text[25];	///-- Static Text

	CTextBox*		WarRecord;
	DWORD	NeedExp;
	DWORD   CurExp;

	///-- �߱� PK���� �κ�
	CTextBox*		m_TxtPKPoint;

	void	SetCitizen(int citizen);	

	void SetToolTipDestWnd( UINT WIDToolTipDestWnd );
	void RenderToolTipName();


	void GetAttackSpeedStr( TCHAR* aAttackSpeedStr, int aAttackSpeed );
	void GetMoveSpeedStr( TCHAR* aMoveSpeedStr, int aMoveSpeed );
	DWORD FameColor(DWORD Fame); 

private:

	int m_Citizen;			  // ����
};


extern	CCharStateWnd	CharStateWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_CHARSTATE_WNDPROC_H__