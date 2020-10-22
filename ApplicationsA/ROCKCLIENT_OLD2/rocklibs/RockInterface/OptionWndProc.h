#if !defined( __RUI_OPTION_WNDPROC_H__ )
#define __RUI_OPTION_WNDPROC_H__
//-----------------------------------------------------------------------------
#include "RockClient.h"
#include "WndProc.h"

#include <vector>
using namespace std;


class CSlideBar;
class CComboBox;
class COptionWnd : public CWndProc
{
public:
	COptionWnd();
	~COptionWnd(){}

	virtual void Init();
	virtual void Composition();
	void CompositionOld();
	void CompositionNew();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

	void	Open(void);	
	void	CompulsionFrm();
	void	RollBackCompulsionFrm();

	void	SetCompulsion(bool flag ){m_bCompulsionFrm = flag;};
	bool	GetCompulsion(){return m_bCompulsionFrm;};
private:
	
	void	OKBtn(void);
	void	CancelBtn(void);
	void	GlowProcess(int Value);
	void	ShadowProcess(int Value);
	void	ObjViewProcess(int Value);
	void	FarLandProcess(int Value);
	void	BGMSoundProcess(int Value);
	void	EffectSoundProcess(int Value);
	void	GammaLightProcess( int Value );
	void	TextureDetailProcess(int Value);

public:
	
	// �Ϻ� ��û���� ���� ����ȭ �ʹ���� .. ���󺹱� 
	bool	m_bCompulsionFrm; 


	int		m_ShdowType;					// 0 : ���� , 1 : ���׶�� , 2 : �ǽð�
	float   m_fGamma;						// ȭ����
	int		m_GlowRange;					// 0 ~ 30 : �ǻ���
	int		m_ObjView;						// 0 ~ 3  : ��湰 �þ�	
	BOOL	m_ISFarLand;					// ����
	BOOL	m_ISLandEffect;					// ��� ����Ʈ
	BOOL	m_ISEffectSound;				// ����Ʈ ���� ��� ����
	BOOL	m_ISBGMSound;					// ������� ���� ��� ����
	int		m_EffectSoundVol;				// 0 ~ 100 : ����Ʈ ����
	int 	m_BGMSoundVol;					// 0 ~ 100 : ������� ����
	BOOL	m_ISWeather;					// ����ǥ�� ����
	BOOL	m_ISHelpView;					// ���� ������ ����
	BOOL	m_ISCharShowID;					// ĳ����ID on~off
	BOOL    m_IsFrameOptimize;				// ������ ����ȭ
	BOOL    m_IsSkEffSys;					// ��ųȿ�� �޽������ ����


	int		m_TexDetailType;				// 0: ��, 1: �� , 2: ��
	BOOL	m_IsEffectMinimize;				// ����Ʈ �ɼ�	
	int		m_IsEWindowsMode;			// 0: Ǯ��ũ�� , 1: â���
	//........................................................................................................
	// ������
	//........................................................................................................
	CFrameWnd		*m_pFrame;	//�⺻ ������
	CFrameWnd		*m_pFrameDisplay;
	CFrameWnd		*m_PFrameEtc;
	
	CButton	*m_DisplayButton;
	CButton	*m_EtcButton;

	//........................................................................................................
	// ���� ���� �׸��� ��ư
	//........................................................................................................
	CRadioButton	*m_pFarLandOffBtn;
	CRadioButton	*m_pFarLandOnBtn;

	CButton*		m_CloseBtn;
	//........................................................................................................
	// �׸��� Ÿ�� ��ư
	//........................................................................................................
	CRadioButton	*m_pShdowOffBtn;
	CRadioButton	*m_pShdowLowBtn;
	CRadioButton	*m_pShdowHighBtn;

	CRadioButton	*m_pWeatherOffBtn;
	CRadioButton	*m_pWeatherOnBtn;

	CRadioButton	*m_pHelpOffBtn;
	CRadioButton	*m_pHelpOnBtn;
	
	//........................................................................................................
	// ȭ���� �����̵� ��
	//........................................................................................................
	CSlideBar		*m_pLightBar;

	//........................................................................................................
	// �ǻ��� �����̵� ��
	//........................................................................................................
	CTextBox		*m_pGlowText;
	CTextBox		*m_pGlowMinText;
	CTextBox		*m_pGlowMaxText;
	CSlideBar		*m_pGlowSBar;

	//........................................................................................................
	// ��湰 �þ� �����̵� ��
	//........................................................................................................
	CSlideBar		*m_pObjViewSBar;

	//........................................................................................................
	// ����Ʈ ���� �����̵� ��
	//........................................................................................................
	CSlideBar		*m_pEffectSoundSBar;

	//........................................................................................................
	// ��� ���� �����̵� ��
	//........................................................................................................
	CSlideBar		*m_pBGMSoundSBar;

	//........................................................................................................
	// ĳ���� ���̵� ��ư 
	//........................................................................................................
	CRadioButton *m_pCharIDOnBtn;
	CRadioButton *m_pCharIDOffBtn;

	//........................................................................................................
	// ������ ����ȭ ��ư 
	//........................................................................................................
	CRadioButton *m_pFrameOtOnBtn;
	CRadioButton *m_pFrameOtOffBtn;

	//........................................................................................................
	// �ؽ��� ������ Ÿ�� ��ư
	//........................................................................................................
	CRadioButton	*m_pTexDetailLowBtn;	
	CRadioButton	*m_pTexDetailNormalBtn;
	CRadioButton	*m_pTexDetailHightBtn;	
	
	//........................................................................................................
	// ����Ʈ �߰� �ɼ� ��ư 
	//........................................................................................................
	CRadioButton *m_pEffNormalBtn;
	CRadioButton *m_pEffMinimizeBtn;
	
	//........................................................................................................
	// â��� 
	//........................................................................................................
	CRadioButton *m_pFullScrBtn;
	CRadioButton *m_pWindowFullScrBtn;
	
	//........................................................................................................
	// ��ų ȿ�� �ý��� �޽��� ��� ���� 
	//........................................................................................................
	CRadioButton *m_pSkEffSysOnBtn;
	CRadioButton *m_pSkEffSysOffBtn;

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/06)

public:	
	int m_iOldW;
	int m_iOldH;
	CComboBox* m_pResolutionCBox;
	vector< pair<DWORD, DWORD> > m_vecResolution;
	bool m_bGlow;

public:
	bool CheckDisplay();// ���÷��� ���� üũ
	bool HasSameResolution(pair<DWORD, DWORD> kPair);// ���� �ػ󵵰� �ִ��� Ȯ��?
	void ResolutionPushBack(DWORD dwW, DWORD dwH);
	void SwitchGlow(bool bOn);// �ǻ��� ��/����
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/05/06)

};



extern	COptionWnd	g_OptionWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_OPTION_WNDPROC_H__