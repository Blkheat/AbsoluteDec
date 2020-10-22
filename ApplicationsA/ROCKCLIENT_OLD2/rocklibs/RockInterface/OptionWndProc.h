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
	
	// 일본 요청으로 강제 최적화 맵벗어나면 .. 원상복구 
	bool	m_bCompulsionFrm; 


	int		m_ShdowType;					// 0 : 없음 , 1 : 동그라미 , 2 : 실시간
	float   m_fGamma;						// 화면밝기
	int		m_GlowRange;					// 0 ~ 30 : 뽀샤시
	int		m_ObjView;						// 0 ~ 3  : 배경물 시야	
	BOOL	m_ISFarLand;					// 원경
	BOOL	m_ISLandEffect;					// 배경 이펙트
	BOOL	m_ISEffectSound;				// 이펙트 사운드 출력 여부
	BOOL	m_ISBGMSound;					// 배경음악 사운드 출력 여부
	int		m_EffectSoundVol;				// 0 ~ 100 : 이펙트 사운드
	int 	m_BGMSoundVol;					// 0 ~ 100 : 배경음악 사운드
	BOOL	m_ISWeather;					// 날씨표현 여부
	BOOL	m_ISHelpView;					// 도움말 나오기 여부
	BOOL	m_ISCharShowID;					// 캐릭터ID on~off
	BOOL    m_IsFrameOptimize;				// 프레임 최적화
	BOOL    m_IsSkEffSys;					// 스킬효력 메시지출력 여부


	int		m_TexDetailType;				// 0: 상, 1: 중 , 2: 하
	BOOL	m_IsEffectMinimize;				// 이펙트 옵션	
	int		m_IsEWindowsMode;			// 0: 풀스크린 , 1: 창모드
	//........................................................................................................
	// 프레임
	//........................................................................................................
	CFrameWnd		*m_pFrame;	//기본 프레임
	CFrameWnd		*m_pFrameDisplay;
	CFrameWnd		*m_PFrameEtc;
	
	CButton	*m_DisplayButton;
	CButton	*m_EtcButton;

	//........................................................................................................
	// 원경 지형 그리기 버튼
	//........................................................................................................
	CRadioButton	*m_pFarLandOffBtn;
	CRadioButton	*m_pFarLandOnBtn;

	CButton*		m_CloseBtn;
	//........................................................................................................
	// 그림자 타입 버튼
	//........................................................................................................
	CRadioButton	*m_pShdowOffBtn;
	CRadioButton	*m_pShdowLowBtn;
	CRadioButton	*m_pShdowHighBtn;

	CRadioButton	*m_pWeatherOffBtn;
	CRadioButton	*m_pWeatherOnBtn;

	CRadioButton	*m_pHelpOffBtn;
	CRadioButton	*m_pHelpOnBtn;
	
	//........................................................................................................
	// 화면밝기 슬라이드 바
	//........................................................................................................
	CSlideBar		*m_pLightBar;

	//........................................................................................................
	// 뽀샤시 슬라이드 바
	//........................................................................................................
	CTextBox		*m_pGlowText;
	CTextBox		*m_pGlowMinText;
	CTextBox		*m_pGlowMaxText;
	CSlideBar		*m_pGlowSBar;

	//........................................................................................................
	// 배경물 시야 슬라이드 바
	//........................................................................................................
	CSlideBar		*m_pObjViewSBar;

	//........................................................................................................
	// 이펙트 사운드 슬라이드 바
	//........................................................................................................
	CSlideBar		*m_pEffectSoundSBar;

	//........................................................................................................
	// 배경 사운드 슬라이드 바
	//........................................................................................................
	CSlideBar		*m_pBGMSoundSBar;

	//........................................................................................................
	// 캐릭터 아이디 버튼 
	//........................................................................................................
	CRadioButton *m_pCharIDOnBtn;
	CRadioButton *m_pCharIDOffBtn;

	//........................................................................................................
	// 프레임 최적화 버튼 
	//........................................................................................................
	CRadioButton *m_pFrameOtOnBtn;
	CRadioButton *m_pFrameOtOffBtn;

	//........................................................................................................
	// 텍스쳐 디테일 타입 버튼
	//........................................................................................................
	CRadioButton	*m_pTexDetailLowBtn;	
	CRadioButton	*m_pTexDetailNormalBtn;
	CRadioButton	*m_pTexDetailHightBtn;	
	
	//........................................................................................................
	// 이펙트 추가 옵션 버튼 
	//........................................................................................................
	CRadioButton *m_pEffNormalBtn;
	CRadioButton *m_pEffMinimizeBtn;
	
	//........................................................................................................
	// 창모드 
	//........................................................................................................
	CRadioButton *m_pFullScrBtn;
	CRadioButton *m_pWindowFullScrBtn;
	
	//........................................................................................................
	// 스킬 효력 시스템 메시지 출력 여부 
	//........................................................................................................
	CRadioButton *m_pSkEffSysOnBtn;
	CRadioButton *m_pSkEffSysOffBtn;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)

public:	
	int m_iOldW;
	int m_iOldH;
	CComboBox* m_pResolutionCBox;
	vector< pair<DWORD, DWORD> > m_vecResolution;
	bool m_bGlow;

public:
	bool CheckDisplay();// 디스플레이 정보 체크
	bool HasSameResolution(pair<DWORD, DWORD> kPair);// 같은 해상도가 있는지 확인?
	void ResolutionPushBack(DWORD dwW, DWORD dwH);
	void SwitchGlow(bool bOn);// 뽀샤시 온/오프
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)

};



extern	COptionWnd	g_OptionWnd;
//-----------------------------------------------------------------------------
#endif  __RUI_OPTION_WNDPROC_H__