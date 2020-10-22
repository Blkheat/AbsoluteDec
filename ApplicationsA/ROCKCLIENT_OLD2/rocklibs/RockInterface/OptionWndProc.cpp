#include "..\\RockPCH.h"
#include "..\\..\\RockClient.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\Effect.h"
#include "..\\..\\bravolibs\sound\sound.h"
//#include "pc.h"

#include "OptionWndProc.h"
#include "SystemMenuWndProc.h"
#include "stringmanager.h"

COptionWnd	g_OptionWnd;

extern int  g_Shadow;
extern int  g_LODRange;
extern BOOL g_ViewLODTerrain;
extern BOOL g_EnableEffectSound;
extern BOOL g_EnableBGMSound;
extern LONG	g_EffectSoundVol;
extern LONG	g_BGMSoundVol;
extern CSystemMenuWnd	SystemMenuWnd;
extern bool g_ISWeatherVisible;
extern bool	g_IsHelpView;
extern bool g_ISCharShowIDVisible;
extern bool g_IsFrameOptimize;
extern float g_fGamma;
extern bool	g_IsEffectMinimize;
extern int	g_nTextureDetail;
extern int	g_IsEWindowsMode;
extern bool g_IsSkEffSys;
extern int g_Max_Effect_Display;

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)

#define STD_RESOLUTION_WIDTH 1024
#define STD_RESOLUTION_HEIGHT 768

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)

//.............................................................................................................
//
//.............................................................................................................
COptionWnd::COptionWnd()
{
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)

	m_iOldW = 0;
	m_iOldH = 0;
	m_pResolutionCBox = 0;
	m_bGlow = true;

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::Init()
{
	m_bCompulsionFrm = false;
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::Composition()
{
#ifdef OPTION_RENEWAL
	CompositionNew();
#else 
	CompositionOld();
#endif 

}

void COptionWnd::CompositionNew()
{
	Init();
	//.........................................................................................................
	// Frame
	//.........................................................................................................

	//버튼이 과 텍스트 짝을 이룬다 . X 
	int ButtonX[2] = { 130,244 };
	int TextPosX[2] = { 146,260 };

	//int ButtonY[]
	CImageBox* img = NULL;
	CTextBox* TBox = NULL;


	int center_x = nRui->Desktop.Width / 2;
	int center_y = nRui->Desktop.Height / 2;

	SRect R;

	R.h = 479;
	R.w = 368;

	R.x = center_x - (R.w / 2);
	R.y = center_y - (R.h / 2);

	m_pFrame = (CFrameWnd *)nRui->CreateWnd(n_wtFrameWnd, WID_None, WID_None, R.x, R.y, R.w, R.h, false);
	m_pFrame->SetWndTexStyle(fwfTex_Style_Tile9);
	m_pFrame->SetClientImage(TID_CTRL_WND_L1);
	m_pFrame->SetIconTID(TID_CTRL_ICON_STATUS);
	m_pFrame->SetWndTileSizeLR(40, 29);
	m_pFrame->SetWndTileSizeTB(31, 11);

	m_pFrame->SetCaption(G_STRING_CHAR(IDS_WND_SETTING));
	m_pFrame->SetTitle(true);

	m_pFrame->SetWndProc(this);
	m_pFrame->SetVisible(false);
	m_pFrame->SetZorder(n_zoTop);
	m_pFrame->Initialize();


	img = (CImageBox*)nRui->CreateWnd(n_wtImageBox, WID_None, m_pFrame->GetID(), 0, 30 + 20, 368, 1, false);
	img->SetClientImage(TID_CTRL_LINEDOT);

	///-- 일반 button		size 78,16
	m_DisplayButton = (CButton*)nRui->CreateWnd(n_wtButton, WID_None, m_pFrame->GetID(), 20, 30, 62, 20, false);
	m_DisplayButton->SetBtnImage(TID_CTRL_BTN62, TID_CTRL_BTN62_OVR, TID_CTRL_BTN62_OVR, TID_None);
	m_DisplayButton->SetFontR(n_fRsvFontWndButton);
	m_DisplayButton->SetText(_RT("Display"));

	///-- 퀘스트 button
	m_EtcButton = (CButton*)nRui->CreateWnd(n_wtButton, WID_None, m_pFrame->GetID(), 20 + 62, 30, 62, 20, false);
	m_EtcButton->SetBtnImage(TID_CTRL_BTN62, TID_CTRL_BTN62_OVR, TID_CTRL_BTN62_OVR, TID_None);
	m_EtcButton->SetFontR(n_fRsvFontWndButton);
	m_EtcButton->SetText(_RT("ETC"));

	m_pFrameDisplay = (CFrameWnd *)nRui->CreateWnd(n_wtFrameWnd, WID_OptWnd, m_pFrame->GetID(), 0, 50, R.w, R.h - 80, false);
	m_pFrameDisplay->SetClientImage(TID_None);
	m_pFrameDisplay->SetWndProc(this);
	//m_pFrameDisplay->SetVisible( false );
	m_pFrameDisplay->SetZorder(n_zoTop);
	m_pFrameDisplay->Initialize();


	m_PFrameEtc = (CFrameWnd *)nRui->CreateWnd(n_wtFrameWnd, WID_None, m_pFrame->GetID(), 0, 50, R.w, R.h - 80, false);
	m_PFrameEtc->SetClientImage(TID_None);
	m_PFrameEtc->SetWndProc(this);
	m_PFrameEtc->SetVisible(false);
	m_PFrameEtc->SetZorder(n_zoTop);
	m_PFrameEtc->Initialize();

	//	m_pFrame->SetMoveLinkFrameWnd(m_pFrameDisplay);
	//	m_pFrame->SetMoveLinkFrameWnd2(m_PFrameEtc);

		//.........................................................................................................
		// Text Label
		//.........................................................................................................

	SRect sTempRect;
	/*
		// 그래픽
		TBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_OptWnd,  126+30, 35 , 42, 10, false );
		TBox->SetFontR( n_fRsvFontWndStatic );
		TBox->SetText( G_STRING_CHAR( IDS_WND_OT_GRAPHIC ));
		TBox->SetTextColor( 0 , 0 , 0 );
		*/

		//그림자 
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61, 40, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_SHADOW));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_SHADOW);
	//없음 
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 117 + 25, 61, 40, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_NONE));
	TBox->SetTextColor(0, 0, 0);
	//저화질 
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 179 + 25, 61, 40, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_LOW_QUAL));
	TBox->SetTextColor(0, 0, 0);
	//고화질 
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 241 + 25, 61, 40, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_HIGH_QUAL));
	TBox->SetTextColor(0, 0, 0);

	//.........................................................................................................
	// 그림자 체크 버튼
	//.........................................................................................................
	m_pShdowOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_OptWnd_ShdowOffBtn, WID_OptWnd,
		105 + 25, 60, 10, 10, false);
	m_pShdowOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pShdowOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pShdowOffBtn->SetDirectCheck(false);

	m_pShdowLowBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_OptWnd_ShdowLowBtn, WID_OptWnd,
		167 + 25, 60, 10, 10, false);
	m_pShdowLowBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pShdowLowBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pShdowLowBtn->SetDirectCheck(false);

	m_pShdowHighBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_OptWnd_ShdowHighBtn, WID_OptWnd,
		229 + 25, 60, 10, 10, false);
	m_pShdowHighBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pShdowHighBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pShdowHighBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(3, WID_OptWnd_ShdowOffBtn,
		WID_OptWnd_ShdowOffBtn,
		WID_OptWnd_ShdowLowBtn,
		WID_OptWnd_ShdowHighBtn);

	//........................................................................................................
	// 텍스쳐 디테일 타입 버튼
	//........................................................................................................
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + 22, 43, 11, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_TEXTURE));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_TEXTURE_SET);

	//하 
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		117 + 25, 83, 40, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_LOW));
	TBox->SetTextColor(0, 0, 0);
	//중 
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		179 + 25, 61 + 22, 40, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_MIDDLE));
	TBox->SetTextColor(0, 0, 0);
	//상
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		241 + 25, 61 + 22, 40, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_HIGH));
	TBox->SetTextColor(0, 0, 0);

	m_pTexDetailLowBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		105 + 25, 60 + 22, 10, 10, false);
	m_pTexDetailLowBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pTexDetailLowBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pTexDetailLowBtn->SetDirectCheck(false);

	m_pTexDetailNormalBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		167 + 25, 60 + 22, 10, 10, false);
	m_pTexDetailNormalBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pTexDetailNormalBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pTexDetailNormalBtn->SetDirectCheck(false);

	m_pTexDetailHightBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		229 + 25, 60 + 22, 10, 10, false);
	m_pTexDetailHightBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pTexDetailHightBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pTexDetailHightBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(3, m_pTexDetailHightBtn->GetID(),
		m_pTexDetailLowBtn->GetID(),
		m_pTexDetailNormalBtn->GetID(),
		m_pTexDetailHightBtn->GetID());

	//.........................................................................................................
	// 프레임 최적화 
	//.........................................................................................................
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 2), 47, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetText(G_STRING_CHAR(IDS_WND_FRAME_OPTIMIZE));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_FRAME_SET);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		TextPosX[0], 61 + 44, 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		TextPosX[1], 61 + 44, 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	m_pFrameOtOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		ButtonX[0], 60 + 44, 10, 10, false);
	m_pFrameOtOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pFrameOtOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pFrameOtOnBtn->SetDirectCheck(false);

	m_pFrameOtOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		ButtonX[1], 60 + 44, 10, 10, false);
	m_pFrameOtOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pFrameOtOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pFrameOtOffBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pFrameOtOffBtn->GetID(),
		m_pFrameOtOnBtn->GetID(),
		m_pFrameOtOffBtn->GetID());

	int AddLine = 22;

	//.........................................................................................................
	// 이펙트 옵션
	//.........................................................................................................
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (AddLine * 3), 47, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetText(G_STRING_CHAR(IDS_EFFECT_OPTION));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_EFFECT_SET);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		TextPosX[0], 61 + AddLine * 3, 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_MINIMIZE));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		TextPosX[1], 61 + AddLine * 3, 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_NORMAL));
	TBox->SetTextColor(0, 0, 0);


	m_pEffMinimizeBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		ButtonX[0], 60 + AddLine * 3, 10, 10, false);
	m_pEffMinimizeBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pEffMinimizeBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pEffMinimizeBtn->SetDirectCheck(false);


	m_pEffNormalBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		ButtonX[1], 60 + AddLine * 3, 10, 10, false);
	m_pEffNormalBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pEffNormalBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pEffNormalBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pEffNormalBtn->GetID(),
		m_pEffNormalBtn->GetID(),
		m_pEffMinimizeBtn->GetID());




	//화면 모드 
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 4), 47, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_SCREEN_MODE));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		TextPosX[0], 61 + (22 * 4), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_FULLSCREEN));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		TextPosX[1], 61 + (22 * 4), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_WINDOWS_FULLSCREEN));
	TBox->SetTextColor(0, 0, 0);

	m_pFullScrBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		ButtonX[0], 60 + (22 * 4), 10, 10, false);
	m_pFullScrBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pFullScrBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pFullScrBtn->SetDirectCheck(false);


	m_pWindowFullScrBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		ButtonX[1], 60 + (22 * 4), 10, 10, false);
	m_pWindowFullScrBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pWindowFullScrBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pWindowFullScrBtn->SetDirectCheck(false);


	nRui->SetGroupWnd(2, m_pFullScrBtn->GetID(),
		m_pFullScrBtn->GetID(),
		m_pWindowFullScrBtn->GetID());


	//////////////////////////////////////////////////////////////////////////
	//원경 지형 
	//////////////////////////////////////////////////////////////////////////
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 5), 45, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_FAR_OBJECT));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_DISTANTVIEW_SET);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		TextPosX[0], 61 + (22 * 5), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		TextPosX[1], 61 + (22 * 5), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	//.........................................................................................................
	// 지형 체크 버튼
	//.........................................................................................................
	m_pFarLandOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_OptWnd_FarLandOnBtn, WID_OptWnd,
		ButtonX[0], 60 + (22 * 5), 10, 10, false);
	m_pFarLandOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pFarLandOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pFarLandOffBtn->SetDirectCheck(false);

	m_pFarLandOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_OptWnd_FarLandOffBtn, WID_OptWnd,
		ButtonX[1], 60 + (22 * 5), 10, 10, false);
	m_pFarLandOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pFarLandOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pFarLandOnBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, WID_OptWnd_FarLandOffBtn,
		WID_OptWnd_FarLandOffBtn,
		WID_OptWnd_FarLandOnBtn);

	//////////////////////////////////////////////////////////////////////////
	//날씨 
	//////////////////////////////////////////////////////////////////////////

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 6), 45, 11, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_WEATHER));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_WEATHER_SET);
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		TextPosX[0], 61 + (22 * 6), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		TextPosX[1], 61 + (22 * 6), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	//.........................................................................................................
	// 날씨 표현 
	//.........................................................................................................
	m_pWeatherOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		ButtonX[0], 61 + (22 * 6), 10, 10, false);
	m_pWeatherOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pWeatherOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pWeatherOnBtn->SetDirectCheck(false);

	m_pWeatherOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		ButtonX[1], 61 + (22 * 6), 10, 10, false);
	m_pWeatherOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pWeatherOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pWeatherOffBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pWeatherOffBtn->GetID(),
		m_pWeatherOffBtn->GetID(),
		m_pWeatherOnBtn->GetID());


	//////////////////////////////////////////////////////////////////////////
	// 화면 밝기 
	//////////////////////////////////////////////////////////////////////////

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 7), 41, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_GAMMA_LIGHT));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_LIGHT_SCRREN_SET);
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		110 + 25, 61 + (22 * 7), 23, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MIN));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		242 + 55, 61 + (22 * 7), 23, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MAX));
	TBox->SetTextColor(0, 0, 0);

	m_pLightBar = (CSlideBar *)nRui->CreateWnd(n_wtSlideBar, WID_None, WID_OptWnd,
		135 + 40, 61 + (22 * 7), 103, 7, false);

	m_pLightBar->SetClientImage(TID_C_WND_OPT_SB_BAR);
	m_pLightBar->SetThumbImage(TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK);
	m_pLightBar->InitSlide(stHorzSld, 0, 8, 3, 7);

	////////////////////////////////////////////////////////////////////////////
	// 뽀샤시 
	//////////////////////////////////////////////////////////////////////////

	m_pGlowText = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 8), 41, 10, false);
	m_pGlowText->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	m_pGlowText->SetClientImage(TID_None);
	m_pGlowText->SetAlignText(n_atLeft, n_atCenter);
	m_pGlowText->SetText(G_STRING_CHAR(IDS_WND_OT_GLOW));
	m_pGlowText->SetTextColor(0, 0, 0);
	m_pGlowText->SetHID(HID_BLUR__SET);

	m_pGlowMinText = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		110 + 25, 61 + (22 * 8), 23, 10, false);
	m_pGlowMinText->SetFontR(n_fRsvFontWndStatic);
	m_pGlowMinText->SetText(G_STRING_CHAR(IDS_WND_OT_MIN));
	m_pGlowMinText->SetTextColor(0, 0, 0);

	m_pGlowMaxText = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		242 + 55, 61 + (22 * 8), 23, 10, false);
	m_pGlowMaxText->SetFontR(n_fRsvFontWndStatic);
	m_pGlowMaxText->SetText(G_STRING_CHAR(IDS_WND_OT_MAX));
	m_pGlowMaxText->SetTextColor(0, 0, 0);

	m_pGlowSBar = (CSlideBar *)nRui->CreateWnd(n_wtSlideBar, WID_OptWnd_GlowSlideBar, WID_OptWnd,
		135 + 40, 60 + (22 * 8), 103, 7, false);

	m_pGlowSBar->SetClientImage(TID_C_WND_OPT_SB_BAR);
	m_pGlowSBar->SetThumbImage(TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK);
	m_pGlowSBar->InitSlide(stHorzSld, 0, 60, 0, 7);

	//////////////////////////////////////////////////////////////////////////
	//배경 시야 
	//////////////////////////////////////////////////////////////////////////

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 9), 47, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_VIEW_RANGE));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_BGROUND_SET);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		110 + 25, 61 + (22 * 9), 23, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MIN));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		242 + 55, 61 + (22 * 9), 23, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MAX));
	TBox->SetTextColor(0, 0, 0);

	//.........................................................................................................
	// 배경 시야 슬라이더 바
	//.........................................................................................................
	m_pObjViewSBar = (CSlideBar *)nRui->CreateWnd(n_wtSlideBar, WID_OptWnd_ObjViewSlideBar, WID_OptWnd,
		135 + 40, 61 + (22 * 9), 103, 7, false);
	m_pObjViewSBar->SetClientImage(TID_C_WND_OPT_SB_BAR);
	m_pObjViewSBar->SetThumbImage(TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK);
	m_pObjViewSBar->InitSlide(stHorzSld, 0, 3, 0, 7);

#ifdef HHM_USER_RESOLUTION_SELECT// Change Resolution(2009/05/06)

	//.........................................................................................................
	// 해상도
	//.........................................................................................................
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 10), 47, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText("Resolution");
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_RESOLUTION_SET);


	m_pResolutionCBox = (CComboBox*)nRui->CreateWnd(n_wtComboBox, WID_None, WID_OptWnd,
		130, 58 + (22 * 10), 130, 100, false);
	m_pResolutionCBox->SetListboxTileSizeTB(5, 5);
	m_pResolutionCBox->SetListboxTileSizeLR(5, 5);
	m_pResolutionCBox->SetListboxImage9(TID_CTRL_OUTLINE_L1);
	m_pResolutionCBox->GetTextBox()->SetTextColor(0, 0, 0);
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// Change Resolution(2009/05/06)



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////
	// Sound & Etc
	///////////////////////////////////////////////////////////////////////////

	///사운드 
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		126 + 30, 61, 42, 11, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_SOUND));
	TBox->SetTextColor(0, 0, 0);

	//////////////////////////////////////////////////////////////////////////
	//효과음 
	//////////////////////////////////////////////////////////////////////////
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(), 31, 61 + 22, 47, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_EFFECT));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_EFFECT_SOUND_SET);
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		110 + 25, 61 + 22, 14, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MIN));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		242 + 55, 61 + 22, 23, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MAX));
	TBox->SetTextColor(0, 0, 0);

	m_pEffectSoundSBar = (CSlideBar *)nRui->CreateWnd(n_wtSlideBar, WID_OptWnd_EffectSoundSlideBar, m_PFrameEtc->GetID(),
		135 + 40, 61 + 22, 103, 7, false);

	m_pEffectSoundSBar->SetClientImage(TID_C_WND_OPT_SB_BAR);
	m_pEffectSoundSBar->SetThumbImage(TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK);
	m_pEffectSoundSBar->InitSlide(stHorzSld, 0, 100, 0, 7);

	///////////////////////////////////////////////////////////////////////////
	// 배경음악 
	//////////////////////////////////////////////////////////////////////////

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(), 31, 61 + (22 * 2), 47, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_BGM));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_BG_SOUND_SET);
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		110 + 25, 61 + (22 * 2), 14, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MIN));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		242 + 55, 61 + (22 * 2), 23, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MAX));
	TBox->SetTextColor(0, 0, 0);

	m_pBGMSoundSBar = (CSlideBar *)nRui->CreateWnd(n_wtSlideBar, WID_OptWnd_BGMSoundSlideBar, m_PFrameEtc->GetID(),
		135 + 40, 61 + (22 * 2), 103, 7, false);

	m_pBGMSoundSBar->SetClientImage(TID_C_WND_OPT_SB_BAR);
	m_pBGMSoundSBar->SetThumbImage(TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK);
	m_pBGMSoundSBar->InitSlide(stHorzSld, 0, 100, 0, 7);

	//////////////////////////////////////////////////////////////////////////
	// 기타 설정 
	//////////////////////////////////////////////////////////////////////////

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		126 + 30, 61 + (22 * 3), 42, 11, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_GAME));
	TBox->SetTextColor(0, 0, 0);

	//.........................................................................................................
	// 캐릭터 ID 
	//.........................................................................................................
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(), 31, 61 + (22 * 4), 47, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_CHARID));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_SHOW_ID_SET);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		TextPosX[0], 61 + (22 * 4), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		TextPosX[1], 61 + (22 * 4), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	m_pCharIDOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, m_PFrameEtc->GetID(),
		ButtonX[0], 60 + (22 * 4), 10, 10, false);
	m_pCharIDOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pCharIDOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pCharIDOnBtn->SetDirectCheck(false);

	m_pCharIDOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, m_PFrameEtc->GetID(),
		ButtonX[1], 60 + (22 * 4), 10, 10, false);
	m_pCharIDOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pCharIDOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pCharIDOffBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pCharIDOffBtn->GetID(),
		m_pCharIDOnBtn->GetID(),
		m_pCharIDOffBtn->GetID());


	//////////////////////////////////////////////////////////////////////////
	// 도움말 
	//////////////////////////////////////////////////////////////////////////

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(), 31, 61 + (22 * 5), 47, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetText(G_STRING_CHAR(IDS_WND_HELP));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_HELPTIP_SET);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		TextPosX[0], 61 + (22 * 5), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		TextPosX[1], 61 + (22 * 5), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	m_pHelpOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, m_PFrameEtc->GetID(),
		ButtonX[0], 60 + (22 * 5), 10, 10, false);
	m_pHelpOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pHelpOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pHelpOnBtn->SetDirectCheck(false);

	m_pHelpOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, m_PFrameEtc->GetID(),
		ButtonX[1], 60 + (22 * 5), 10, 10, false);
	m_pHelpOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pHelpOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pHelpOffBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pHelpOnBtn->GetID(),
		m_pHelpOnBtn->GetID(),
		m_pHelpOffBtn->GetID());


	//////////////////////////////////////////////////////////////////////////
	// 스킬 효력 메세지 
	//////////////////////////////////////////////////////////////////////////

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(), 31, 61 + (22 * 6), 47, 10, false);
	TBox->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdNormal);
	TBox->SetClientImage(TID_None);
	TBox->SetText(G_STRING_CHAR(IDS_SKILLEFF_MESSAGE));
	TBox->SetTextColor(0, 0, 0);
	TBox->SetHID(HID_SKILL_EFFECT_MES_SET);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		TextPosX[0], 61 + (22 * 6), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, m_PFrameEtc->GetID(),
		TextPosX[1], 61 + (22 * 6), 80, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	m_pSkEffSysOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, m_PFrameEtc->GetID(),
		ButtonX[0], 60 + (22 * 6), 10, 10, false);
	m_pSkEffSysOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pSkEffSysOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pSkEffSysOnBtn->SetDirectCheck(false);

	m_pSkEffSysOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, m_PFrameEtc->GetID(),
		ButtonX[1], 60 + (22 * 6), 10, 10, false);
	m_pSkEffSysOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pSkEffSysOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pSkEffSysOffBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pSkEffSysOnBtn->GetID(),
		m_pSkEffSysOnBtn->GetID(),
		m_pSkEffSysOffBtn->GetID());
	//.........................................................................................................
	// OK 버튼
	//.........................................................................................................
	CButton*		btn;
	btn = (CButton*)nRui->CreateWnd(n_wtButton, WID_OptWnd_OKBtn, m_pFrame->GetID(),
		109, 448, 58, 25, false);
	btn->SetBtnImage(TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None);
	btn->SetFontR(n_fRsvFontWndButton);
	btn->SetText(G_STRING_CHAR(IDS_WND_COM_OK));
	//.........................................................................................................
	// Cancel 버튼
	//.........................................................................................................
	btn = (CButton*)nRui->CreateWnd(n_wtButton, WID_OptWnd_CancelBtn, m_pFrame->GetID(),
		198, 448, 58, 25, false);
	btn->SetBtnImage(TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None);
	btn->SetFontR(n_fRsvFontWndButton);
	btn->SetText(G_STRING_CHAR(IDS_WND_COM_CANCEL));
	//.........................................................................................................
	// 닫기 버튼
	//.........................................................................................................
	m_CloseBtn = (CButton*)nRui->CreateWnd(n_wtButton, WID_None, m_pFrame->GetID(),
		287 + 50, 6, 13, 13, false);
	m_CloseBtn->SetBtnImage(TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None);
	m_CloseBtn->SetVisible(TRUE);
}


//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::Update()
{
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::Proc(SEventMessage* EMsg)
{
	switch (EMsg->FocusWnd)
	{
		//.....................................................................................................	
		// 그림자 Off 체크
		//.....................................................................................................	
	case WID_OptWnd_ShdowOffBtn:

		if (EMsg->Notify == NM_BTN_CLICK)
		{
			nRui->SendEvent(WID_OptWnd_ShdowOffBtn, n_emSetChecked, 0, 0, 0, 0);
			ShadowProcess(0);
		}

		break;

		//.....................................................................................................	
		// 그림자 Low 체크
		//.....................................................................................................	
	case WID_OptWnd_ShdowLowBtn:

		if (EMsg->Notify == NM_BTN_CLICK)
		{
			nRui->SendEvent(WID_OptWnd_ShdowLowBtn, n_emSetChecked, 0, 0, 0, 0);
			ShadowProcess(1);
		}

		break;

		//.....................................................................................................	
		// 그림자 High 체크
		//.....................................................................................................	
	case WID_OptWnd_ShdowHighBtn:

		if (EMsg->Notify == NM_BTN_CLICK)
		{
			nRui->SendEvent(WID_OptWnd_ShdowHighBtn, n_emSetChecked, 0, 0, 0, 0);
			ShadowProcess(2);
		}

		break;

		//.....................................................................................................	
		// 뽀샤시 슬라이더 바
		//.....................................................................................................	
	case WID_OptWnd_GlowSlideBar:
		if (EMsg->Notify == NM_SLD_MOVEBAR)
		{
			GlowProcess(EMsg->sParam);
		}
		break;

		//.....................................................................................................	
		// 배경 슬라이더 바
		//.....................................................................................................	
	case WID_OptWnd_ObjViewSlideBar:
		if (EMsg->Notify == NM_SLD_MOVEBAR)
		{
			//m_pGlowSBar->GetValue( &m_GlowRange );
			ObjViewProcess(EMsg->sParam);
		}
		break;

		//.....................................................................................................	
		// 지형 Off 체크
		//.....................................................................................................	
	case WID_OptWnd_FarLandOffBtn:

		if (EMsg->Notify == NM_BTN_CLICK)
		{
			nRui->SendEvent(WID_OptWnd_FarLandOffBtn, n_emSetChecked, 0, 0, 0, 0);
			FarLandProcess(0);
		}

		break;

		//.....................................................................................................	
		// 지형 Low 체크
		//.....................................................................................................	
	case WID_OptWnd_FarLandOnBtn:

		if (EMsg->Notify == NM_BTN_CLICK)
		{
			nRui->SendEvent(WID_OptWnd_FarLandOnBtn, n_emSetChecked, 0, 0, 0, 0);
			FarLandProcess(1);
		}

		break;

		//.....................................................................................................	
		// 이펙트 사운드 슬라이더 바
		//.....................................................................................................	
	case WID_OptWnd_EffectSoundSlideBar:
		if (EMsg->Notify == NM_SLD_MOVEBAR)
		{
			EffectSoundProcess(EMsg->sParam);
		}
		break;

		//.....................................................................................................	
		// 배경 사운드 슬라이더 바
		//.....................................................................................................	
	case WID_OptWnd_BGMSoundSlideBar:
		if (EMsg->Notify == NM_SLD_MOVEBAR)
		{
			BGMSoundProcess(EMsg->sParam);
		}
		break;

		//.....................................................................................................	
		// OK 버튼
		//.....................................................................................................	
	case WID_OptWnd_OKBtn:

		if (EMsg->Notify == NM_BTN_CLICK)
		{
			OKBtn();
		}

		break;

		//.....................................................................................................	
		// Cancel 버튼
		//.....................................................................................................	
	case WID_OptWnd_CancelBtn:

		if (EMsg->Notify == NM_BTN_CLICK)
		{
			CancelBtn();
		}

		break;

	default:
#ifdef OPTION_RENEWAL		
		if (m_DisplayButton->GetID() == EMsg->FocusWnd)// Display
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pFrameDisplay->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0);
				nRui->SendEvent(m_PFrameEtc->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0);
			}
		}
		else if (m_EtcButton->GetID() == EMsg->FocusWnd)// Etc
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pFrameDisplay->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0);
				nRui->SendEvent(m_PFrameEtc->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0);
			}
		}
#endif 

		if (m_CloseBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				CancelBtn();
			}
		}
		else if (m_pWeatherOnBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pWeatherOnBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);

				g_ISWeatherVisible = TRUE;
			}
		}
		else if (m_pWeatherOffBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pWeatherOffBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);

				g_ISWeatherVisible = FALSE;
			}
		}
		else if (m_pHelpOnBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pHelpOnBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
				g_IsHelpView = true;
			}
		}
		else if (m_pHelpOffBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pHelpOffBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
				g_IsHelpView = false;
			}
		}
		else if (m_pCharIDOnBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pCharIDOnBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
				g_ISCharShowIDVisible = true;
			}
		}
		else if (m_pCharIDOffBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pCharIDOffBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
				g_ISCharShowIDVisible = false;
			}
		}
		// 프레임 최적화 
		else if (m_pFrameOtOnBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pFrameOtOnBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
				g_IsFrameOptimize = true;
				g_RockClient.ResetNowTextureDetail();
			}
		}
		else if (m_pFrameOtOffBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pFrameOtOffBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
				g_IsFrameOptimize = false;
				g_RockClient.ResetNowTextureDetail();
			}
		}
		else if (m_pLightBar->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_SLD_MOVEBAR)
			{
				GammaLightProcess(EMsg->sParam);
			}
		}
		//이펙트 최소화
		else if (m_pEffNormalBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pEffNormalBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
				g_IsEffectMinimize = false;
				g_Max_Effect_Display = 450;
			}
		}
		else if (m_pEffMinimizeBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pEffMinimizeBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
				g_IsEffectMinimize = true;
				g_Max_Effect_Display = 250;
			}
		}
		else if (m_pTexDetailLowBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				TextureDetailProcess(2);
				nRui->SendEvent(m_pTexDetailLowBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
			}
		}
		else if (m_pTexDetailNormalBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				TextureDetailProcess(1);
				nRui->SendEvent(m_pTexDetailNormalBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
			}
		}
		else if (m_pTexDetailHightBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				TextureDetailProcess(0);
				nRui->SendEvent(m_pTexDetailHightBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
			}
		}
#ifdef ROCK_WINDOWS_MODE

		else if (m_pFullScrBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pFullScrBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
				g_IsEWindowsMode = 0;

				if (MODE_NORMAL_FULLMODE == g_RockClient.GetEWindowsMode())
				{
					if (!g_RockClient.m_bFullScreen)
					{
						g_RockClient.ToggleFullscreen();

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)

						CheckDisplay();

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)
					}
				}
			}
		}
		else if (m_pWindowFullScrBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				nRui->SendEvent(m_pWindowFullScrBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
				g_IsEWindowsMode = 1;

				if (MODE_NORMAL_FULLMODE == g_RockClient.GetEWindowsMode())
				{
					if (g_RockClient.m_bFullScreen)
					{
						g_RockClient.ToggleFullscreen();

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)

						CheckDisplay();

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)
					}
				}
			}
		}
#endif 

		else if (m_pSkEffSysOnBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				g_IsSkEffSys = true;
				nRui->SendEvent(m_pSkEffSysOnBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
			}
		}
		else if (m_pSkEffSysOffBtn->GetID() == EMsg->FocusWnd)
		{
			if (EMsg->Notify == NM_BTN_CLICK)
			{
				g_IsSkEffSys = false;
				nRui->SendEvent(m_pSkEffSysOffBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
			}
		}


		break;
	}
}

void COptionWnd::CompulsionFrm()
{

	g_IsEffectMinimize = true;
	g_Max_Effect_Display = 250;

	g_IsFrameOptimize = true;
	//	g_RockClient.ResetNowTextureDetail();

}

void COptionWnd::RollBackCompulsionFrm()
{

	g_IsEffectMinimize = false;
	g_Max_Effect_Display = 450;

	g_IsFrameOptimize = false;
	//	g_RockClient.ResetNowTextureDetail();

}


//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::OKBtn(void)
{
	char setupFile[MAX_PATH];
	wsprintf(setupFile, ".\\%s", "setup.ini");
	char buf[32];

	/// 뽀샤시 
	wsprintf(buf, "%d", g_Particle.m_GlowAlpha);
	WritePrivateProfileString("Client", "Glow", buf, setupFile);

	/// 오브젝트 시야 거리 + 지형 디테일 
	wsprintf(buf, "%d", g_LODRange);
	WritePrivateProfileString("Client", "ObjView", buf, setupFile);

	/// 원경 지형 그릴 것인가 말것인가? 
	wsprintf(buf, "%d", g_ViewLODTerrain);
	WritePrivateProfileString("Client", "LODTerrain", buf, setupFile);

	/// BGM 플레이 여부 
	wsprintf(buf, "%d", g_Sound.ISMP3Enable());
	WritePrivateProfileString("Client", "BGM", buf, setupFile);

	/// Effect Sound 플레이 여부 
	wsprintf(buf, "%d", g_Sound.ISWaveEnable());
	WritePrivateProfileString("Client", "EffectSound", buf, setupFile);

	/// 이펙트 사운드 볼륨
	wsprintf(buf, "%d", g_Sound.GetWaveVolume());
	WritePrivateProfileString("Client", "EffectSoundVol", buf, setupFile);

	/// 배경음악 볼륨	
	wsprintf(buf, "%d", g_Sound.GetMP3Volume());
	WritePrivateProfileString("Client", "BGMVol", buf, setupFile);

	/// 그림자	
	wsprintf(buf, "%d", g_Shadow);
	WritePrivateProfileString("Client", "Shadow", buf, setupFile);

	/// Weather
	wsprintf(buf, "%d", g_ISWeatherVisible);
	WritePrivateProfileString("Client", "Weather", buf, setupFile);

	/// HelpView
	wsprintf(buf, "%d", g_IsHelpView);
	WritePrivateProfileString("Client", "HelpView", buf, setupFile);

	/// CharShowID
	wsprintf(buf, "%d", g_ISCharShowIDVisible);
	WritePrivateProfileString("Client", "CharShowID", buf, setupFile);

	/// FrameOptimize
	wsprintf(buf, "%d", g_IsFrameOptimize);
	WritePrivateProfileString("Client", "FrameOptimize", buf, setupFile);

	/// Gamma	
	int nValue = ((g_fGamma - 0.8f) * 10.0f) + 0.5f;
	wsprintf(buf, "%d", nValue);
	WritePrivateProfileString("Client", "Gamma", buf, setupFile);

	/// FrameOptimize
	wsprintf(buf, "%d", g_IsEffectMinimize);
	WritePrivateProfileString("Client", "EffectMinimize", buf, setupFile);

	/// TexDetail
	wsprintf(buf, "%d", g_nTextureDetail);
	WritePrivateProfileString("Client", "TexDetail", buf, setupFile);

	/// Window 0 풀스크린 1 창모드 
	wsprintf(buf, "%d", g_IsEWindowsMode);
	WritePrivateProfileString("Client", "Window", buf, setupFile);

	/// 이펙트 효력 메시지 출력여부
	wsprintf(buf, "%d", g_IsSkEffSys);
	WritePrivateProfileString("Client", "SkillEffSys", buf, setupFile);

	m_pFrame->SetVisible(FALSE);

#ifdef HHM_USER_RESOLUTION_SELECT// Change Resolution(2009/05/06)

	int iSelectIndex = m_pResolutionCBox->GetCurSelIdx();
	if (iSelectIndex == -1)// 선택하지 않았다
	{
		wsprintf(buf, "%d", STD_RESOLUTION_WIDTH);
		WritePrivateProfileString("Client", "Width", buf, setupFile);

		wsprintf(buf, "%d", STD_RESOLUTION_HEIGHT);
		WritePrivateProfileString("Client", "Height", buf, setupFile);

		return;
	}

	if (iSelectIndex >= static_cast<int>(m_vecResolution.size()))// 해상도 맵 초기화 범위를 넘는 인덱스
		return;

	// 선택한 해상도를 setup.ini에 쓴다.
	pair<DWORD, DWORD> kPair = m_vecResolution[iSelectIndex];
	wsprintf(buf, "%d", kPair.first);
	WritePrivateProfileString("Client", "Width", buf, setupFile);

	wsprintf(buf, "%d", kPair.second);
	WritePrivateProfileString("Client", "Height", buf, setupFile);

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// Change Resolution(2009/05/06)
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::CancelBtn(void)
{
	g_Shadow = m_ShdowType;
	g_Particle.m_GlowAlpha = m_GlowRange;
	g_LODRange = m_ObjView;
	g_ViewLODTerrain = m_ISFarLand;
	g_ISWeatherVisible = m_ISWeather;
	g_IsHelpView = m_ISHelpView;
	g_ISCharShowIDVisible = m_ISCharShowID;
	g_IsFrameOptimize = m_IsFrameOptimize;
	g_fGamma = m_fGamma;
	g_IsSkEffSys = m_IsSkEffSys;

	g_IsEffectMinimize = m_IsEffectMinimize;
	if (g_IsEffectMinimize)
	{
		g_Max_Effect_Display = 250;
	}
	else
	{
		g_Max_Effect_Display = 450;
	}

	if (g_nTextureDetail != m_TexDetailType)
	{
		g_nTextureDetail = m_TexDetailType;
		g_RockClient.ResetNowTextureDetail();
	}

	if (g_IsEWindowsMode != m_IsEWindowsMode)
	{
		g_IsEWindowsMode = m_IsEWindowsMode;
		g_RockClient.ToggleFullscreen();
	}

	::Drv_SetGamma(NULL, g_fGamma, 10, 1.0f);

	D3DXCOLOR Color = D3DCOLOR_ARGB(g_Particle.m_GlowAlpha, 255, 255, 255);
	g_Particle.m_Glow.SetColor(Color);

	g_Sound.SetWaveEnable(m_ISEffectSound);

	if (m_ISEffectSound == 0)
	{
		g_Sound.AllStop();
		g_Sound.SetWaveVolume(0);
	}
	else
	{
		g_Sound.SetWaveVolume(m_EffectSoundVol);
	}

	g_Sound.SetMP3Enable(m_ISBGMSound);

	if (m_ISBGMSound == 0)
	{
		g_Sound.m_BackSound.Stop();
		g_Sound.SetMP3Volume(0);
	}
	else
	{
		g_Sound.SetMP3Volume(m_BGMSoundVol);
	}

	m_pFrame->SetVisible(FALSE);
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::GlowProcess(int Value)
{
	D3DXCOLOR Color = D3DCOLOR_ARGB(Value, 255, 255, 255);

	g_Particle.m_GlowAlpha = Value;
	g_Particle.m_Glow.SetColor(Color);
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::ShadowProcess(int Value)
{
	g_Shadow = Value;
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::ObjViewProcess(int Value)
{
	g_LODRange = (int)Value;

	g_Map.theTerrain.SetLODQuality(g_LODRange);
	g_Obj_Manager.SetLodRange(g_LODRange);
	g_Map.UpdateTerrain(g_Map.GetPreCX(), g_Map.GetPreCY(), TRUE);
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::FarLandProcess(int Value)
{
	g_ViewLODTerrain = (BOOL)Value;
}

void COptionWnd::GammaLightProcess(int Value)
{
	g_fGamma = (float)Value * 0.1f + 0.8f;
	::Drv_SetGamma(NULL, g_fGamma, 10, 1.0f);
}

void COptionWnd::TextureDetailProcess(int Value)
{
	if (g_nTextureDetail != Value)
	{
		g_nTextureDetail = Value;
		g_RockClient.ResetNowTextureDetail();
	}
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::BGMSoundProcess(int Value)
{
	if (g_Sound.ISMP3Enable())
	{
		if (Value == 0)
		{
			g_Sound.SetMP3Enable(FALSE);
			g_Sound.m_BackSound.Stop();
			g_Sound.SetMP3Volume(0);
		}
		else
		{
			g_Sound.SetMP3Volume(Value);
		}
	}
	else
	{
		if (Value == 0)
		{
			g_Sound.SetMP3Enable(FALSE);
			g_Sound.m_BackSound.Stop();
			g_Sound.SetMP3Volume(0);
		}
		else
		{
			g_Sound.SetMP3Enable(TRUE);
			g_Sound.SetMP3Volume(Value);
			g_Sound.PlayMP3(TRUE);
		}

	}
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::EffectSoundProcess(int Value)
{
	if (g_Sound.ISWaveEnable())
	{
		if (Value == 0)
		{
			g_Sound.SetWaveEnable(FALSE);
			g_Sound.AllStop();
			g_Sound.SetWaveVolume(0);
		}
		else
		{
			g_Sound.SetWaveVolume(Value);
		}
	}
	else
	{
		if (Value == 0)
		{
			g_Sound.SetWaveEnable(FALSE);
			g_Sound.AllStop();
			g_Sound.SetWaveVolume(0);
		}
		else
		{
			g_Sound.SetWaveEnable(TRUE);
			g_Sound.SetWaveVolume(Value);
		}

	}
}

//.............................................................................................................
//
//.............................................................................................................
void COptionWnd::Open(void)
{
	m_ShdowType = g_Shadow;
	m_GlowRange = g_Particle.m_GlowAlpha;
	m_ObjView = g_LODRange;
	m_ISFarLand = g_ViewLODTerrain;
	m_ISEffectSound = g_Sound.ISWaveEnable();
	m_ISBGMSound = g_Sound.ISMP3Enable();
	m_EffectSoundVol = g_Sound.GetWaveVolume();
	m_BGMSoundVol = g_Sound.GetMP3Volume();
	m_ISWeather = g_ISWeatherVisible;
	m_ISHelpView = g_IsHelpView;
	m_ISCharShowID = g_ISCharShowIDVisible;
	m_IsFrameOptimize = g_IsFrameOptimize;
	m_fGamma = g_fGamma;
	m_TexDetailType = g_nTextureDetail;
	m_IsEffectMinimize = g_IsEffectMinimize;
	m_IsEWindowsMode = g_IsEWindowsMode;
	m_IsSkEffSys = g_IsSkEffSys;

	switch (m_ShdowType)
	{
	case 0:
		nRui->SendEvent(WID_OptWnd_ShdowOffBtn, n_emSetChecked, 0, 0, 0, 0);
		break;
	case 1:
		nRui->SendEvent(WID_OptWnd_ShdowLowBtn, n_emSetChecked, 0, 0, 0, 0);
		break;
	case 2:
		nRui->SendEvent(WID_OptWnd_ShdowHighBtn, n_emSetChecked, 0, 0, 0, 0);
		break;
	}

	if (m_ISWeather)
	{
		nRui->SendEvent(m_pWeatherOnBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}
	else
	{
		nRui->SendEvent(m_pWeatherOffBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}

	if (m_ISHelpView)
	{
		nRui->SendEvent(m_pHelpOnBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}
	else
	{
		nRui->SendEvent(m_pHelpOffBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}


	if (m_ISCharShowID)
	{
		nRui->SendEvent(m_pCharIDOnBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}
	else
	{
		nRui->SendEvent(m_pCharIDOffBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}

	if (m_IsFrameOptimize)
	{
		nRui->SendEvent(m_pFrameOtOnBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}
	else
	{
		nRui->SendEvent(m_pFrameOtOffBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}

	if (m_IsEffectMinimize)
	{
		nRui->SendEvent(m_pEffMinimizeBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}
	else
	{
		nRui->SendEvent(m_pEffNormalBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}
#ifdef ROCK_WINDOWS_MODE
	if (!m_IsEWindowsMode)
	{
		nRui->SendEvent(m_pFullScrBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}
	else
	{
		nRui->SendEvent(m_pWindowFullScrBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
	}
#endif 

	switch (m_TexDetailType)
	{
	case 0:
		nRui->SendEvent(m_pTexDetailHightBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
		break;
	case 1:
		nRui->SendEvent(m_pTexDetailNormalBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
		break;
	case 2:
		nRui->SendEvent(m_pTexDetailLowBtn->GetID(), n_emSetChecked, 0, 0, 0, 0);
		break;
	}

	m_pGlowSBar->SetValue(m_GlowRange);
	m_pObjViewSBar->SetValue(m_ObjView);
	int nValue = ((m_fGamma - 0.8f) * 10.0f) + 0.5f;
	m_pLightBar->SetValue(nValue);

	if (m_ISFarLand)
	{
		nRui->SendEvent(WID_OptWnd_FarLandOnBtn, n_emSetChecked, 0, 0, 0, 0);
	}
	else
	{
		nRui->SendEvent(WID_OptWnd_FarLandOffBtn, n_emSetChecked, 0, 0, 0, 0);
	}

	if (m_ISEffectSound == 0)
	{
		m_pEffectSoundSBar->SetValue(0);
	}
	else
	{
		m_pEffectSoundSBar->SetValue(m_EffectSoundVol);
	}

	if (m_ISBGMSound == 0)
	{
		m_pBGMSoundSBar->SetValue(0);
	}
	else
	{
		m_pBGMSoundSBar->SetValue(m_BGMSoundVol);
	}

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)

	CheckDisplay();

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/05/06)

	m_pFrame->SetZorder(n_zoTop);
	SystemMenuWnd.Close();
	m_pFrame->SetVisible(TRUE);
}



void COptionWnd::CompositionOld()
{
	Init();
	//.........................................................................................................
	// Frame
	//.........................................................................................................
	int center_x = nRui->Desktop.Width / 2;
	int center_y = nRui->Desktop.Height / 2;

	SRect R;

	R.w = 318;
	R.h = 479;

#if defined(APPLY_ENGLISH_ORIGIN_SIZE) || defined(JAPAN_VERSION)

#define RENEWAL_OPTIONWINDOW

#endif	

#ifdef RENEWAL_OPTIONWINDOW

	R.w = 368;

#endif

	R.x = center_x - (R.w / 2);
	R.y = center_y - (R.h / 2);

	m_pFrame = (CFrameWnd *)nRui->CreateWnd(n_wtFrameWnd, WID_OptWnd, WID_None, R.x, R.y, R.w, R.h, false);
	m_pFrame->SetWndTexStyle(fwfTex_Style_Tile9);
	m_pFrame->SetClientImage(TID_CTRL_WND_L1);
	m_pFrame->SetIconTID(TID_CTRL_ICON_STATUS);
	m_pFrame->SetWndTileSizeLR(40, 29);
	m_pFrame->SetWndTileSizeTB(31, 11);

	m_pFrame->SetCaption(G_STRING_CHAR(IDS_WND_SETTING));
	m_pFrame->SetTitle(true);

	m_pFrame->SetWndProc(this);
	m_pFrame->SetVisible(false);
	m_pFrame->SetZorder(n_zoTop);
	m_pFrame->Initialize();

	//.........................................................................................................
	// Text Label
	//.........................................................................................................
	CTextBox*		TBox = NULL;

	SRect sTempRect;

	sTempRect.x = 126;
	sTempRect.y = 35;
	sTempRect.w = 42;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 126 + 30;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_GRAPHIC));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 31;
	sTempRect.y = 61;
	sTempRect.w = 43;
	sTempRect.h = 10;

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_SHADOW));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 117;
	sTempRect.y = 61;
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 117 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_NONE));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 179;
	sTempRect.y = 61;
	sTempRect.w = 23;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 179 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_LOW_QUAL));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 241;
	sTempRect.y = 61;
	sTempRect.w = 23;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 241 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_HIGH_QUAL));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 60;
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif

	//.........................................................................................................
	// 그림자 체크 버튼
	//.........................................................................................................
	m_pShdowOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_OptWnd_ShdowOffBtn, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pShdowOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pShdowOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pShdowOffBtn->SetDirectCheck(false);

	sTempRect.x = 167;
	sTempRect.y = 60;
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 167 + 25;

#endif

	m_pShdowLowBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_OptWnd_ShdowLowBtn, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pShdowLowBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pShdowLowBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pShdowLowBtn->SetDirectCheck(false);

	sTempRect.x = 229;
	sTempRect.y = 60;
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 229 + 25;

#endif	

	m_pShdowHighBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_OptWnd_ShdowHighBtn, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pShdowHighBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pShdowHighBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pShdowHighBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(3, WID_OptWnd_ShdowOffBtn,
		WID_OptWnd_ShdowOffBtn,
		WID_OptWnd_ShdowLowBtn,
		WID_OptWnd_ShdowHighBtn);

	//........................................................................................................
	// 텍스쳐 디테일 타입 버튼
	//........................................................................................................
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + 22, 43, 11, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_TEXTURE));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 117;
	sTempRect.y = 83;
	sTempRect.w = 15;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 117 + 25;

#endif	


	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_LOW));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 179;
	sTempRect.y = 61 + 22;
	sTempRect.w = 15;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 179 + 25;

#endif	

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_MIDDLE));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 241;
	sTempRect.y = 61 + 22;
	sTempRect.w = 15;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 241 + 25;

#endif	

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_HIGH));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 60 + 22;
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif	

	m_pTexDetailLowBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pTexDetailLowBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pTexDetailLowBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pTexDetailLowBtn->SetDirectCheck(false);

	sTempRect.x = 167;
	sTempRect.y = 60 + 22;
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 167 + 25;

#endif	

	m_pTexDetailNormalBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pTexDetailNormalBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pTexDetailNormalBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pTexDetailNormalBtn->SetDirectCheck(false);

	sTempRect.x = 229;
	sTempRect.y = 60 + 22;
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 229 + 25;

#endif		

	m_pTexDetailHightBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pTexDetailHightBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pTexDetailHightBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pTexDetailHightBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(3, m_pTexDetailHightBtn->GetID(),
		m_pTexDetailLowBtn->GetID(),
		m_pTexDetailNormalBtn->GetID(),
		m_pTexDetailHightBtn->GetID());

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 2), 41, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_GAMMA_LIGHT));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 110;
	sTempRect.y = 61 + (22 * 2);
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 110 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MIN));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 242;
	sTempRect.y = 61 + (22 * 2);
	sTempRect.w = 23;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 242 + 55;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MAX));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 135;
	sTempRect.y = 61 + (22 * 2);
	sTempRect.w = 103;
	sTempRect.h = 7;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 135 + 40;

#endif

	//.........................................................................................................
	// 화면밝기 슬라이더 바
	//.........................................................................................................
	m_pLightBar = (CSlideBar *)nRui->CreateWnd(n_wtSlideBar, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);

	m_pLightBar->SetClientImage(TID_C_WND_OPT_SB_BAR);
	m_pLightBar->SetThumbImage(TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK);
	m_pLightBar->InitSlide(stHorzSld, 0, 8, 3, 7);


	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 3), 41, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_GLOW));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 110;
	sTempRect.y = 61 + (22 * 3);
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 110 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MIN));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 242;
	sTempRect.y = 61 + (22 * 3);
	sTempRect.w = 23;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 242 + 55;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MAX));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 135;
	sTempRect.y = 60 + (22 * 3);
	sTempRect.w = 103;
	sTempRect.h = 7;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 135 + 40;

#endif

	//.........................................................................................................
	// 뽀샤시 슬라이더 바
	//.........................................................................................................
	m_pGlowSBar = (CSlideBar *)nRui->CreateWnd(n_wtSlideBar, WID_OptWnd_GlowSlideBar, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);

	m_pGlowSBar->SetClientImage(TID_C_WND_OPT_SB_BAR);
	m_pGlowSBar->SetThumbImage(TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK);
	m_pGlowSBar->InitSlide(stHorzSld, 0, 60, 0, 7);


	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 4), 47, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_VIEW_RANGE));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 61 + (22 * 4);
	sTempRect.w = 26;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MIN));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 245;
	sTempRect.y = 61 + (22 * 4);
	sTempRect.w = 17;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 245 + 55;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MAX));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 135;
	sTempRect.y = 60 + (22 * 4);
	sTempRect.w = 103;
	sTempRect.h = 7;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 135 + 40;

#endif

	//.........................................................................................................
	// 배경 시야 슬라이더 바
	//.........................................................................................................
	m_pObjViewSBar = (CSlideBar *)nRui->CreateWnd(n_wtSlideBar, WID_OptWnd_ObjViewSlideBar, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pObjViewSBar->SetClientImage(TID_C_WND_OPT_SB_BAR);
	m_pObjViewSBar->SetThumbImage(TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK);
	m_pObjViewSBar->InitSlide(stHorzSld, 0, 3, 0, 7);


	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 5), 45, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_FAR_OBJECT));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 122;
	sTempRect.y = 61 + (22 * 5);
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 122 + 25;

#endif


	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 184;
	sTempRect.y = 61 + (22 * 5);
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 184 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 60 + (22 * 5);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif

	//.........................................................................................................
	// 지형 체크 버튼
	//.........................................................................................................
	m_pFarLandOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_OptWnd_FarLandOnBtn, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pFarLandOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pFarLandOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pFarLandOffBtn->SetDirectCheck(false);

	sTempRect.x = 167;
	sTempRect.y = 60 + (22 * 5);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 167 + 25;

#endif

	m_pFarLandOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_OptWnd_FarLandOffBtn, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pFarLandOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pFarLandOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pFarLandOnBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, WID_OptWnd_FarLandOffBtn,
		WID_OptWnd_FarLandOffBtn,
		WID_OptWnd_FarLandOnBtn);


	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 61 + (22 * 6), 45, 11, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_WEATHER));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 122;
	sTempRect.y = 61 + (22 * 6);
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 122 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 184;
	sTempRect.y = 61 + (22 * 6);
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 184 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 60 + (22 * 6);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif

	//.........................................................................................................
	// 날씨 표현 
	//.........................................................................................................
	m_pWeatherOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pWeatherOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pWeatherOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pWeatherOnBtn->SetDirectCheck(false);

	sTempRect.x = 167;
	sTempRect.y = 60 + (22 * 6);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 167 + 25;

#endif

	m_pWeatherOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pWeatherOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pWeatherOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pWeatherOffBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pWeatherOffBtn->GetID(),
		m_pWeatherOffBtn->GetID(),
		m_pWeatherOnBtn->GetID());

	sTempRect.x = 126;
	sTempRect.y = 219;
	sTempRect.w = 42;
	sTempRect.h = 11;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 126 + 30;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_SOUND));
	TBox->SetTextColor(0, 0, 0);

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 245, 47, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_EFFECT));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 110;
	sTempRect.y = 245;
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 110 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MIN));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 242;
	sTempRect.y = 245;
	sTempRect.w = 23;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 242 + 55;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MAX));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 135;
	sTempRect.y = 244;
	sTempRect.w = 103;
	sTempRect.h = 7;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 135 + 40;

#endif

	//.........................................................................................................
	// 이펙트 사운드 슬라이더 바
	//.........................................................................................................
	m_pEffectSoundSBar = (CSlideBar *)nRui->CreateWnd(n_wtSlideBar, WID_OptWnd_EffectSoundSlideBar, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);

	m_pEffectSoundSBar->SetClientImage(TID_C_WND_OPT_SB_BAR);
	m_pEffectSoundSBar->SetThumbImage(TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK);
	m_pEffectSoundSBar->InitSlide(stHorzSld, 0, 100, 0, 7);


	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 245 + 22, 47, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_BGM));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 110;
	sTempRect.y = 245 + 22;
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 110 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MIN));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 242;
	sTempRect.y = 245 + 22;
	sTempRect.w = 23;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 242 + 55;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_MAX));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 135;
	sTempRect.y = 244 + 22;
	sTempRect.w = 103;
	sTempRect.h = 7;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 135 + 40;

#endif

	//.........................................................................................................
	// 배경 사운드 슬라이더 바
	//.........................................................................................................
	m_pBGMSoundSBar = (CSlideBar *)nRui->CreateWnd(n_wtSlideBar, WID_OptWnd_BGMSoundSlideBar, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);

	m_pBGMSoundSBar->SetClientImage(TID_C_WND_OPT_SB_BAR);
	m_pBGMSoundSBar->SetThumbImage(TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK, TID_C_WND_OPT_TICK);
	m_pBGMSoundSBar->InitSlide(stHorzSld, 0, 100, 0, 7);

	sTempRect.x = 126;
	sTempRect.y = 293;
	sTempRect.w = 42;
	sTempRect.h = 11;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 126 + 30;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_GAME));
	TBox->SetTextColor(0, 0, 0);

	int AddCheckBtnWidth = 30;

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 319, 47, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_HELP));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 122;
	sTempRect.y = 319;
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 122 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 184 + AddCheckBtnWidth;
	sTempRect.y = 319;
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 184 + AddCheckBtnWidth + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 318;
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif

	m_pHelpOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pHelpOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pHelpOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pHelpOnBtn->SetDirectCheck(false);

	sTempRect.x = 167 + AddCheckBtnWidth;
	sTempRect.y = 318;
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 167 + AddCheckBtnWidth + 25;

#endif

	m_pHelpOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pHelpOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pHelpOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pHelpOffBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pHelpOnBtn->GetID(),
		m_pHelpOnBtn->GetID(),
		m_pHelpOffBtn->GetID());

	//.........................................................................................................
	// 캐릭터 ID 
	//.........................................................................................................
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 319 + 22, 47, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_CHARID));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 122;
	sTempRect.y = 319 + 22;
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 122 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 184 + AddCheckBtnWidth;
	sTempRect.y = 319 + 22;
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 184 + AddCheckBtnWidth + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 318 + 22;
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif

	m_pCharIDOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pCharIDOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pCharIDOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pCharIDOnBtn->SetDirectCheck(false);

	sTempRect.x = 167 + AddCheckBtnWidth;
	sTempRect.y = 318 + 22;
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 167 + AddCheckBtnWidth + 25;

#endif

	m_pCharIDOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pCharIDOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pCharIDOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pCharIDOffBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pCharIDOffBtn->GetID(),
		m_pCharIDOnBtn->GetID(),
		m_pCharIDOffBtn->GetID());

	//.........................................................................................................
	// 프레임 최적화 
	//.........................................................................................................
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 319 + (22 * 2), 47, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_FRAME_OPTIMIZE));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 122;
	sTempRect.y = 319 + (22 * 2);
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 122 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 184 + AddCheckBtnWidth;
	sTempRect.y = 319 + (22 * 2);
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 184 + AddCheckBtnWidth + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 318 + (22 * 2);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif

	m_pFrameOtOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pFrameOtOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pFrameOtOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pFrameOtOnBtn->SetDirectCheck(false);

	sTempRect.x = 167 + AddCheckBtnWidth;
	sTempRect.y = 318 + (22 * 2);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 167 + AddCheckBtnWidth + 25;

#endif

	m_pFrameOtOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pFrameOtOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pFrameOtOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pFrameOtOffBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pFrameOtOffBtn->GetID(),
		m_pFrameOtOnBtn->GetID(),
		m_pFrameOtOffBtn->GetID());
	//.........................................................................................................
	// 이펙트 옵션
	//.........................................................................................................
	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 319 + (22 * 3), 47, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_EFFECT_OPTION));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 120;
	sTempRect.y = 319 + (22 * 3);
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 120 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_MINIMIZE));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 184 + AddCheckBtnWidth;
	sTempRect.y = 319 + (22 * 3);
	sTempRect.w = 14;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 184 + AddCheckBtnWidth + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_NORMAL));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 318 + (22 * 3);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif

	m_pEffMinimizeBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pEffMinimizeBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pEffMinimizeBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pEffMinimizeBtn->SetDirectCheck(false);

	sTempRect.x = 167 + AddCheckBtnWidth;
	sTempRect.y = 318 + (22 * 3);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 167 + AddCheckBtnWidth + 25;

#endif

	m_pEffNormalBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pEffNormalBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pEffNormalBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pEffNormalBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pEffNormalBtn->GetID(),
		m_pEffNormalBtn->GetID(),
		m_pEffMinimizeBtn->GetID());

#ifdef ROCK_WINDOWS_MODE

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 319 + (22 * 4), 47, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_WND_SCREEN_MODE));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 120;
	sTempRect.y = 319 + (22 * 4);
	sTempRect.w = 60;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 120 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_FULLSCREEN));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 184 + AddCheckBtnWidth;
	sTempRect.y = 319 + (22 * 4);
	sTempRect.w = 60;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 184 + AddCheckBtnWidth + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_WINDOWS_FULLSCREEN));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 318 + (22 * 4);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif

	m_pFullScrBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pFullScrBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pFullScrBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pFullScrBtn->SetDirectCheck(false);
#ifdef CHINA_VERSION
	m_pFullScrBtn->SetHID(HID_WINDOWS_MODE);
#endif    

	sTempRect.x = 167 + AddCheckBtnWidth;
	sTempRect.y = 318 + (22 * 4);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 167 + AddCheckBtnWidth + 25;

#endif

	m_pWindowFullScrBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pWindowFullScrBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pWindowFullScrBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pWindowFullScrBtn->SetDirectCheck(false);
#ifdef CHINA_VERSION
	m_pWindowFullScrBtn->SetHID(HID_WINDOWS_MODE);
#endif

	nRui->SetGroupWnd(2, m_pFullScrBtn->GetID(),
		m_pFullScrBtn->GetID(),
		m_pWindowFullScrBtn->GetID());

	int nMultyValue = 5;
	int nBtnHeight = 225 + 66 + 121 + 15;
#else 	

	int nMultyValue = 4;
	int nBtnHeight = 225 + 66 + 121;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd, 31, 319 + (22 * nMultyValue), 47, 10, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetText(G_STRING_CHAR(IDS_SKILLEFF_MESSAGE));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 120;
	sTempRect.y = 319 + (22 * nMultyValue);
	sTempRect.w = 60;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 120 + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_ON));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 184 + AddCheckBtnWidth;
	sTempRect.y = 319 + (22 * nMultyValue);
	sTempRect.w = 60;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 184 + AddCheckBtnWidth + 25;

#endif

	TBox = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	TBox->SetFontR(n_fRsvFontWndStatic);
	TBox->SetAlignText(n_atLeft, n_atCenter);
	TBox->SetText(G_STRING_CHAR(IDS_WND_OT_OFF));
	TBox->SetTextColor(0, 0, 0);

	sTempRect.x = 105;
	sTempRect.y = 318 + (22 * nMultyValue);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 105 + 25;

#endif

	m_pSkEffSysOnBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pSkEffSysOnBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pSkEffSysOnBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pSkEffSysOnBtn->SetDirectCheck(false);

	sTempRect.x = 167 + AddCheckBtnWidth;
	sTempRect.y = 318 + (22 * nMultyValue);
	sTempRect.w = 10;
	sTempRect.h = 10;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 167 + AddCheckBtnWidth + 25;

#endif

	m_pSkEffSysOffBtn = (CRadioButton *)nRui->CreateWnd(n_wtRadioButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_pSkEffSysOffBtn->SetClientImage(TID_CTRL_RAD_DIS);
	m_pSkEffSysOffBtn->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_None);
	m_pSkEffSysOffBtn->SetDirectCheck(false);

	nRui->SetGroupWnd(2, m_pSkEffSysOnBtn->GetID(),
		m_pSkEffSysOnBtn->GetID(),
		m_pSkEffSysOffBtn->GetID());

	nBtnHeight += 22;

	sTempRect.x = 84;
	sTempRect.y = nBtnHeight;
	sTempRect.w = 58;
	sTempRect.h = 25;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 84 + 25;

#endif

	//.........................................................................................................
	// OK 버튼
	//.........................................................................................................
	CButton*		btn;
	btn = (CButton*)nRui->CreateWnd(n_wtButton, WID_OptWnd_OKBtn, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	btn->SetBtnImage(TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None);
	btn->SetFontR(n_fRsvFontWndButton);
	btn->SetText(G_STRING_CHAR(IDS_WND_COM_OK));

	sTempRect.x = 172;
	sTempRect.y = nBtnHeight;
	sTempRect.w = 58;
	sTempRect.h = 25;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 172 + 25;

#endif

	//.........................................................................................................
	// Cancel 버튼
	//.........................................................................................................
	btn = (CButton*)nRui->CreateWnd(n_wtButton, WID_OptWnd_CancelBtn, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	btn->SetBtnImage(TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None);
	btn->SetFontR(n_fRsvFontWndButton);
	btn->SetText(G_STRING_CHAR(IDS_WND_COM_CANCEL));

	sTempRect.x = 287;
	sTempRect.y = 6;
	sTempRect.w = 13;
	sTempRect.h = 13;

#ifdef RENEWAL_OPTIONWINDOW

	sTempRect.x = 287 + 50;

#endif

	//.........................................................................................................
	// 닫기 버튼
	//.........................................................................................................
	m_CloseBtn = (CButton*)nRui->CreateWnd(n_wtButton, WID_None, WID_OptWnd,
		sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false);
	m_CloseBtn->SetBtnImage(TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None);
	m_CloseBtn->SetVisible(TRUE);


}

#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/04/23)

bool COptionWnd::CheckDisplay()
{
	m_pResolutionCBox->ClearItem();// ???? ????.
	m_vecResolution.clear();

	DEVMODE	CurrentDevMode;// ?? ????? ??
	ZeroMemory(&CurrentDevMode, sizeof(CurrentDevMode));
	::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &CurrentDevMode);

	BOOL bRetVal = FALSE;
	DWORD dwMode = 0;

	DEVMODE	devmode;// ?? ????? ??
	ZeroMemory(&devmode, sizeof(devmode));

	while (true)
	{
		if (!EnumDisplaySettings(NULL, dwMode, &devmode))
		{
			++dwMode;
			break;
		}

		DWORD dwPelsWidth = devmode.dmPelsWidth,// ?? ???
			dwPelsHeight = devmode.dmPelsHeight;// ?? ???

		if (dwPelsWidth < 800 && dwPelsHeight < 600)
		{
			++dwMode;
			continue;
		}

		ResolutionPushBack(dwPelsWidth, dwPelsHeight);

		pair<DWORD, DWORD> kPair(dwPelsWidth, dwPelsHeight);
		if (!HasSameResolution(kPair))// ?? ?? ???? ??? ???? ???
		{
			m_vecResolution.push_back(kPair);

			char buf[128] = { 0 };
			Rsprintf(RWCHAR(buf), _RT("%d x %d"), dwPelsWidth, dwPelsHeight);
			m_pResolutionCBox->AddItem(buf);
		}

		++dwMode;
	}// while(true)

	char buf[128] = { 0 };
	pair<DWORD, DWORD> kPair(g_RockClient.GetDisplayWidth(), g_RockClient.GetDisplayHeight());
	if (HasSameResolution(kPair))
		Rsprintf(RWCHAR(buf), _RT("%d x %d"), g_RockClient.GetDisplayWidth(), g_RockClient.GetDisplayHeight());
	else// ??? ??? ???.
		Rsprintf(RWCHAR(buf), _RT("%d x %d"), STD_RESOLUTION_WIDTH, STD_RESOLUTION_HEIGHT);

	m_pResolutionCBox->SetSelectText(buf);

	return true;
}

// ?? ???? ??? ???
bool COptionWnd::HasSameResolution(pair<DWORD, DWORD> kPair)
{
	if (m_vecResolution.empty())
		return false;

	vector< pair<DWORD, DWORD> >::iterator it = find(m_vecResolution.begin(), m_vecResolution.end(), kPair);
	if (it != m_vecResolution.end())
		return true;

	return false;
}

//
void COptionWnd::ResolutionPushBack(DWORD dwW, DWORD dwH)
{
	pair<DWORD, DWORD> kPair(dwW, dwH);
	if (!HasSameResolution(kPair))// ?? ?? ???? ??? ???? ???
	{
		m_vecResolution.push_back(kPair);

		char buf[128] = { 0 };
		Rsprintf(RWCHAR(buf), _RT("%d x %d"), dwW, dwH);
		m_pResolutionCBox->AddItem(buf);
	}
}

// 뽀샤시 온/오프
void COptionWnd::SwitchGlow(bool bOn)
{
	if (m_pGlowText)
		m_pGlowText->SetEnable(bOn);

	if (m_pGlowMinText)
		m_pGlowMinText->SetEnable(bOn);

	if (m_pGlowMaxText)
		m_pGlowMaxText->SetEnable(bOn);

	if (m_pGlowSBar)
		m_pGlowSBar->SetEnable(bOn);
}

#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 선택(2009/04/23)
