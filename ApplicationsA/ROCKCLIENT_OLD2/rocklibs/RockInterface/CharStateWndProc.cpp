#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"
#include "MainMenuWndProc.h"

#include "..\\..\\GTimer.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"


#include "CharStateWndProc.h"
#include "AbilityUpWndProc.h"
#include "StringManager.h"

CCharStateWnd	CharStateWnd;

#define TEXT_COLOR_STATIC 90, 90, 90
#define TEXT_COLOR_VALUE1 90, 90, 120
#define TEXT_COLOR_VALUE2 120, 90, 90

const DWORD c_FameLv1_Range_Min( 0 );
const DWORD c_FameLv1_Range_Max( 9 );
const DWORD c_FameLv2_Range_Min( 10 );
const DWORD c_FameLv2_Range_Max( 29 );
const DWORD c_FameLv3_Range_Min( 30 );
const DWORD c_FameLv3_Range_Max( 59 );
const DWORD c_FameLv4_Range_Min( 60 );
const DWORD c_FameLv4_Range_Max( 99 );
const DWORD c_FameLv5_Range_Min( 100 );
const DWORD c_FameLv5_Range_Max( 149 );
const DWORD c_FameLv6_Range_Min( 150 );
const DWORD c_FameLv6_Range_Max( 219 );
const DWORD c_FameLv7_Range_Min( 220 );
const DWORD c_FameLv7_Range_Max( 499 );
const DWORD c_FameLv8_Range_Min( 500 );
const DWORD c_FameLv8_Range_Max( 999 );
const DWORD c_FameLv9_Range_Min( 1000 );
const DWORD c_FameLv9_Range_Max( 14999 );
const DWORD c_FameLv10_Range_Min( 15000 );
const DWORD c_FameLv10_Range_Max( 29999 );
const DWORD c_FameLv11_Range_Min( 30000 );
const DWORD c_FameLv11_Range_Max( 59999 );
const DWORD c_FameLv12_Range_Min( 60000 );
const DWORD c_FameLv12_Range_Max( 149999 );
const DWORD c_FameLv13_Range_Min( 150000 );
const DWORD c_FameLv13_Range_Max( 299999 );
const DWORD c_FameLv14_Range_Min( 300000 );
const DWORD c_FameLv14_Range_Max( 499999 );
const DWORD c_FameLv15_Range_Min( 500000 );
const DWORD c_FameLv15_Range_Max( 999999 );
const DWORD c_FameLv16_Range_Min( 1000000 );
const DWORD c_FameLv16_Range_Max( 9999999 );
const DWORD c_FameLv17_Range_Min( 10000000 );

//-----------------------------------------------------------------------------
void CCharStateWnd::Init()
{
	thePcParam	= nRui->GetPcParamInfo();
	theFontg = Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdShadow );
	
	//-------------------------------------------------------------------------
	int cur_percent( 0 );
	
	
	
	cur_percent	= 0;
	
	//nRui->SendEvent( WID_Stat_LVgage, n_emSetPercent, (EPARAM)cur_percent, 0, 0, 0 );
	///nRui->SendEvent( WID_MainMenu_LVgage, n_emSetPercent, (EPARAM)cur_percent, 0, 0, 0 );
}
//-----------------------------------------------------------------------------
void CCharStateWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	char temp[100] = "";
	CImageBox*		img;
	
	CTextBox* TextBox = NULL;
	
	CButton*		btn;
	
	
	SRect statwnd;
#ifdef APPLY_BOLD_FONT
	statwnd.w	= 286;
#else
	statwnd.w	= 256;
#endif	
	statwnd.h	= 440;
	statwnd.x	= center_x - ( statwnd.w / 2 );
	statwnd.y	= center_y - ( statwnd.h / 2 );
	
	pMainFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_StatWnd, WID_None, statwnd.x, statwnd.y, statwnd.w, statwnd.h, false );
	pMainFrame->SetWndTexStyle( fwfTex_Style_Tile9 );
	pMainFrame->SetClientImage( TID_CTRL_WND_L1 );	
	pMainFrame->SetIconTID( TID_CTRL_ICON_STATUS );
	pMainFrame->SetWndTileSizeLR( 40, 29 );
	pMainFrame->SetWndTileSizeTB( 31, 11 ); 	
	pMainFrame->SetCaption( G_STRING_CHAR( IDS_WND_STATE ) );
	pMainFrame->SetTitle( true );
	
	pMainFrame->SetWndProc( this );	
	pMainFrame->SetVisible( false );
	pMainFrame->Initialize();
	
	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------	
	///-- Line
#ifdef APPLY_BOLD_FONT
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 1, 131, 284, 1, false ); 
#else
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 1, 131, 254, 1, false );   
#endif	
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
#ifdef APPLY_BOLD_FONT
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 1, 217, 284, 1, false );   
#else
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 1, 217, 254, 1, false );   
#endif	
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
#ifdef APPLY_BOLD_FONT
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 1, 305, 284, 1, false ); 
#else
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 1, 305, 254, 1, false );   
#endif	
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
#ifdef APPLY_BOLD_FONT	
	NationImg[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 209, 55, 59, 74, false );   
	NationImg[0]->SetClientImage( TID_CTRL_FLAG_MILLENA );
	
	NationImg[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 209, 55, 59, 74, false );   
	NationImg[1]->SetClientImage( TID_CTRL_FLAG_RAIN );

#else
	NationImg[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 189, 55, 59, 74, false );   
	NationImg[0]->SetClientImage( TID_CTRL_FLAG_MILLENA );
	
	NationImg[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 189, 55, 59, 74, false );   
	NationImg[1]->SetClientImage( TID_CTRL_FLAG_RAIN );
#endif	
	
	///-- Bar BG
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 50, 182+4, 191, 9, false );   
	img ->SetClientImage( TID_C_WND_STATE_SB_HP_BG );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 50, 199+4, 191, 9, false );   
	img ->SetClientImage( TID_C_WND_STATE_SB_HP_BG );
	
#ifdef APPLY_BOLD_FONT	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 40, 275, 221, 23, false );  
#else
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_StatWnd, 20, 275, 221, 23, false );   
#endif	
	img ->SetClientImage( TID_C_WND_STATE_SB_EXP_BG );
	
	///-----------------------------------------------------------------------
	///-- Static Text
	
	///-- 국가
	
	TextBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 38, 80, 13, false );
	TextBox->SetFontR( n_fRsvFontWndStatic );
	TextBox->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_NATION ) );
	TextBox->SetText( temp );
	
	
	///-- 국가 공헌도
	Static_Text[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 115, 38, 80, 13, false );
	Static_Text[0]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[0]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_GAMERESULT_CONTRIBUTION ) );
	Static_Text[0]->SetText( temp );
	
	
	///-- 이름
	Static_Text[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 53, 80, 13, false );
	Static_Text[1]->SetFontR( n_fRsvFontWndStatic );	
	Static_Text[1]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_NAME ) );
	Static_Text[1]->SetText( temp );
	
	
	///-- 성별
	Static_Text[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 68, 80, 13, false );
	Static_Text[2]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[2]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_GENDER ) );
	Static_Text[2]->SetText( temp );
	
	
	///-- 직업
	Static_Text[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 83, 80, 13, false );
	Static_Text[3]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[3]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_CLASS ) );
	Static_Text[3]->SetText( temp );	
	
	///-- 명성
	Static_Text[4] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 98, 80, 13, false );
	Static_Text[4]->SetFontR( n_fRsvFontWndStatic );	
	Static_Text[4]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_RESPECT ) );
	Static_Text[4]->SetText( temp );	
	
	///-- 길드
	Static_Text[5] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 113, 80, 13, false );
	Static_Text[5]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[5]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_GUILD ) );
	Static_Text[5]->SetText( temp );	
	
	///-- 보조직업
	Static_Text[6] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 138, 80, 13, false );
	Static_Text[6]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[6]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_SUBCLASS ) );
	Static_Text[6]->SetText( temp );	
	
#ifdef C_SUBJOB_RENEW
	Static_Text[6]->SetVisible(false);
#endif 


	///-- 경험치
	Static_Text[7] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 153, 80, 13, false );
	Static_Text[7]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[7]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_EXP ) );
	Static_Text[7]->SetText( temp );	
#ifdef C_SUBJOB_RENEW
	Static_Text[7]->SetVisible(false);
#endif 

	///-- 전적 
	Static_Text[24] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 170, 80, 13, false );
	Static_Text[24]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[24]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:") , G_STRING_CHAR(IDS_WND_WAR_RECOD));
	Static_Text[24]->SetText( temp );	
	
	
	
	///-- HP
	Static_Text[8] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 182+4, 80, 13, false );
	Static_Text[8]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[8]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_HP ) );
	Static_Text[8]->SetText( temp );	
	
	///-- SP / MP
	Static_Text[9] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 198+4, 80, 13, false );
	Static_Text[9]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[9]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_SP ) );
	Static_Text[9]->SetText( temp );	
	
	///-- 레벨
	Static_Text[10] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 224, 80, 13, false );
	Static_Text[10]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[10]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_LEVEL2 ) );
	Static_Text[10]->SetText( temp );	
	
	///-- 능력업 포인트
	Static_Text[11] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 240, 80, 13, false );
	Static_Text[11]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[11]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_UPPOINT ) );
	Static_Text[11]->SetText( temp );	
	
	///-- 스킬포인트
	Static_Text[12] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 256, 80, 13, false );
	Static_Text[12]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[12]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_SKILLPOINT ) );
	Static_Text[12]->SetText( temp );	
	
	///-- 파워 / 교감
	Static_Text[13] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 320, 80, 13, false );
	Static_Text[13]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[13]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_POWER ) );
	Static_Text[13]->SetText( temp );	
	
	///-- 체력
	Static_Text[14] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 335, 80, 13, false );
	Static_Text[14]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[14]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_VITAL ) );
	Static_Text[14]->SetText( temp );	
	
	///-- 지구력 / 지능 
	Static_Text[15] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 350, 80, 13, false );
	Static_Text[15]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[15]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_STAMINA ) );
	Static_Text[15]->SetText( temp );	
	
	///-- 민첩
#ifdef APPLY_BOLD_FONT
	Static_Text[16] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 155, 320, 80, 13, false );
#else
	Static_Text[16] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 140, 320, 80, 13, false );
#endif	
	Static_Text[16]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[16]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_DEX ) );
	Static_Text[16]->SetText( temp );
	
	///-- 매력
#ifdef APPLY_BOLD_FONT
	Static_Text[17] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 155, 335, 80, 13, false );
#else
	Static_Text[17] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 140, 335, 80, 13, false );
#endif	
	Static_Text[17]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[17]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_CHARISMA ) );
	Static_Text[17]->SetText( temp );	
	
	///-- 행운
#ifdef APPLY_BOLD_FONT
	Static_Text[18] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 155, 350, 80, 13, false );
#else
	Static_Text[18] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 140, 350, 80, 13, false );
#endif	

	Static_Text[18]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[18]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_LUCK ) );
	Static_Text[18]->SetText( temp );	
	
	///-- 물리공격력 / 마법공격력
	Static_Text[19] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 375, 80, 13, false );
	Static_Text[19]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[19]->SetAlignText( n_atLeft, n_atCenter );	
	
	switch(g_RockClient.GetLanguage())
	{
	case eLang_Singapore:
	case eLang_English:
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ) );
		break; 
	default:
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_PHYSICAL_ATK ) );
		
	}


	Static_Text[19]->SetText( temp );	
	
	///-- 물리방어력 / 마법방어력
	Static_Text[20] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 390, 80, 13, false );
	Static_Text[20]->SetFontR( n_fRsvFontWndStatic );	
	Static_Text[20]->SetAlignText( n_atLeft, n_atCenter );	

	switch(g_RockClient.GetLanguage())
	{
	case eLang_Singapore:
	case eLang_English:
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ) );
		break; 
	default:
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_PHYSICAL_DEF ) );
		
	}
	
	Static_Text[20]->SetText( temp );	
	
	
	///-- 마법방어력 / 물리방어력
	Static_Text[21] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 15, 405, 80, 13, false );
	Static_Text[21]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[21]->SetAlignText( n_atLeft, n_atCenter );	

	switch(g_RockClient.GetLanguage())
	{
	case eLang_Singapore:
	case eLang_English:
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MAGIC_DEF ) );
		break; 
	default:
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ) );
		
	}

	Static_Text[21]->SetText( temp );	

	
	///-- 공격속도
#ifdef APPLY_BOLD_FONT
	Static_Text[22] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 155, 375, 80, 13, false );
#else
	Static_Text[22] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 140, 375, 80, 13, false );
#endif
	Static_Text[22]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[22]->SetAlignText( n_atLeft, n_atCenter );	

	switch(g_RockClient.GetLanguage())
	{
		case eLang_Singapore:
		case eLang_English:
			Static_Text[22]->SetHID(HD_S_ATTACK_SPEED);
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_ATTACK_SPEED ) );
			break; 
		default:
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_ATTSPEED ) );
			break; 
	}
	
	Static_Text[22]->SetText( temp );
	
	
	///-- 이동속도
#ifdef APPLY_BOLD_FONT
	Static_Text[23] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 155, 390, 80, 13, false );
#else
	Static_Text[23] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 140, 390, 80, 13, false );
#endif	
	Static_Text[23]->SetFontR( n_fRsvFontWndStatic );
	Static_Text[23]->SetAlignText( n_atLeft, n_atCenter );
	
	switch(g_RockClient.GetLanguage())
	{
		case eLang_Singapore:
		case eLang_English:
			Static_Text[23]->SetHID(HD_S_MOVE_SPEED);
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MOVE_SPEED ) );
			break; 
		default:
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MOVSPEED ) );
			break; 
		
	}

	Static_Text[23]->SetText( temp );
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X
#ifdef APPLY_BOLD_FONT
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Stat_CloseBtn, WID_StatWnd, 267, 6, 13, 13, false ); 
#else	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Stat_CloseBtn, WID_StatWnd, 237, 6, 13, 13, false );  	
#endif	
	btn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	
	//아이콘
	IconImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_Stat_SIconImg, WID_StatWnd, 78, 84, 13, 13, false );
	IconImg->SetClientImage( TID_CLIENT );
	IconImg->SetClientColor( 255, 0, 0 );
	IconImg->SetVisible( FALSE );
	
	///-----------------------------------------------------------------------
	
	//국가
	StateText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_StateText, WID_StatWnd, 58, 38, 146, 13, false );
	StateText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	StateText->SetClientImage( TID_None );
	StateText->SetClientColor( 180, 175, 160 );
	StateText->SetAlignText( n_atLeft, n_atCenter );	
	StateText->SetTextColor( TEXT_COLOR_VALUE1 );	
	StateText->SetHID( HD_S_NATION );
	
	///-- 국가 공헌도
	int MaxStr = Static_Text[0]->GetStringInfo()->Width;
	
	ContributionText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, MaxStr+ 115 + 2 ,  38, 100 , 13, false );
	ContributionText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	ContributionText->SetClientImage( TID_None );
	ContributionText->SetClientColor( 180, 175, 160 );
	ContributionText->SetAlignText( n_atLeft, n_atCenter);	
	ContributionText->SetTextColor( TEXT_COLOR_VALUE1 );
	

	//이름
	NameText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_NameText, WID_StatWnd, 58, 53, 146, 13, false );
	NameText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	NameText->SetClientImage( TID_None );
	NameText->SetClientColor( 180, 175, 160 );
	NameText->SetAlignText( n_atLeft, n_atCenter );	
	NameText->SetTextColor( TEXT_COLOR_VALUE2 );
	NameText->SetHID( HD_S_NAME );
	
	//성별
	GenderText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_GenderText, WID_StatWnd, 58, 68, 146, 13, false );
	GenderText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	GenderText->SetClientImage( TID_None );
	GenderText->SetClientColor( 180, 175, 160 );
	GenderText->SetAlignText( n_atLeft, n_atCenter );		
	GenderText->SetTextColor( TEXT_COLOR_VALUE1 );
	GenderText->SetHID( HD_S_GENDER );
	//직업
	ProfessionText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_ProfessionText, WID_StatWnd, 58, 83, 146, 13, false );
	ProfessionText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	ProfessionText->SetClientImage( TID_None );
	ProfessionText->SetClientColor( 180, 175, 160 );
	ProfessionText->SetAlignText( n_atLeft, n_atCenter );	
	ProfessionText->SetTextColor( TEXT_COLOR_VALUE1 );
	ProfessionText->SetHID( HD_S_JOB );
	
	//명성
	FameText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_FameText, WID_StatWnd, 58, 98, 146, 13, false );
	FameText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	FameText->SetClientImage( TID_None );
	FameText->SetClientColor( 180, 175, 160 );
	FameText->SetAlignText( n_atLeft, n_atCenter );	
	FameText->SetTextColor( TEXT_COLOR_VALUE1 );
	FameText->SetHID( HD_S_FAME );
	
	//길드
	GuildText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_GuildText, WID_StatWnd, 76, 113, 146, 13, false );
	GuildText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	GuildText->SetClientImage( TID_None );
	GuildText->SetClientColor( 180, 175, 160 );
	GuildText->SetAlignText( n_atLeft, n_atCenter );	
	GuildText->SetTextColor( TEXT_COLOR_VALUE1 );	
	
	//상태
	StatText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_StatText, WID_StatWnd, 96, 98, 128, 13, false );
	StatText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	StatText->SetClientImage( TID_None );
	StatText->SetClientColor( 180, 175, 160 );
	StatText->SetAlignText( n_atLeft, n_atCenter );	
	StatText->SetTextColor( TEXT_COLOR_VALUE1 );
	StatText->SetHID( HD_S_STATE );
	StatText->SetVisible( FALSE );
	
	
	///-- 보조직업
	SubClass = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_SubClass_TEXT, WID_StatWnd, 82, 138, 146, 13, false );
	SubClass->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SubClass->SetClientImage( TID_None );
	SubClass->SetClientColor( 180, 175, 160 );
	SubClass->SetAlignText( n_atLeft, n_atCenter );	
	SubClass->SetTextColor( TEXT_COLOR_VALUE2 );
#ifdef C_SUBJOB_RENEW
	SubClass->SetVisible(false);
#endif 

	
	///-- 보조직업 경험치
	SubExp = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_SubClass_EXP_TEXT, WID_StatWnd, 82, 153, 146, 13, false );
	SubExp->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SubExp->SetClientImage( TID_None );
	SubExp->SetClientColor( 180, 175, 160 );
	SubExp->SetAlignText( n_atLeft, n_atCenter );
	SubExp->SetText( _RT("0") );
	SubExp->SetTextColor( TEXT_COLOR_VALUE2 );	

#ifdef C_SUBJOB_RENEW
	SubExp->SetVisible(false);
#endif 

	
	//레벨
#ifdef APPLY_BOLD_FONT
	LevelText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_LevelText, WID_StatWnd, 69, 224, 25, 12, false );
#else
	LevelText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_LevelText, WID_StatWnd, 59, 224, 25, 12, false );
#endif	
	LevelText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	LevelText->SetClientImage( TID_None );
	LevelText->SetClientColor( 180, 175, 160 );
	LevelText->SetAlignText( n_atLeft, n_atCenter );
	LevelText->SetText( _RT("65535") );
	LevelText->SetTextColor( TEXT_COLOR_VALUE2 );
	
	//능력업 포인트	
#ifdef APPLY_BOLD_FONT
	AbilityUpPointText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_AbilityUpPointText, WID_StatWnd, 109, 240, 25, 12, false );
#else
	AbilityUpPointText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_AbilityUpPointText, WID_StatWnd, 99, 240, 25, 12, false );
#endif	
	AbilityUpPointText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	AbilityUpPointText->SetClientImage( TID_None );
	AbilityUpPointText->SetClientColor( 180, 175, 160 );
	AbilityUpPointText->SetAlignText( n_atLeft, n_atCenter );
	AbilityUpPointText->SetText( _RT("65535") );
	AbilityUpPointText->SetTextColor( TEXT_COLOR_VALUE1 );
	AbilityUpPointText->SetHID( HD_S_ATTPOINT );
	
	//스킬업 포인트
#ifdef APPLY_BOLD_FONT
	SkillUpPointText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_SkillUpPointText, WID_StatWnd, 109, 256, 25, 12, false );
#else
	SkillUpPointText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_SkillUpPointText, WID_StatWnd, 99, 256, 25, 12, false );
#endif
	SkillUpPointText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	SkillUpPointText->SetClientImage( TID_None );
	SkillUpPointText->SetClientColor( 180, 175, 160 );
	SkillUpPointText->SetAlignText( n_atLeft, n_atCenter );
	SkillUpPointText->SetText( _RT("65535") );
	SkillUpPointText->SetTextColor( TEXT_COLOR_VALUE1 );
	SkillUpPointText->SetHID( HD_S_SKPOINT );
	
	
	HPGauge = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_Stat_HPGauge, WID_StatWnd, 50, 182+4, 191, 9, false );
	HPGauge->SetClientImage( TID_None );
	HPGauge->SetClientColor( 255, 255, 255 );
	HPGauge->SetBarImage( TID_C_WND_STATE_SB_HPBAR );	
	HPGauge->InitProgress( 100 );
	HPGauge->SetHID( HD_S_HPGRAPH );
	//HPGauge->SetBarColor( 255, 0, 0 );
	
	
	MPGauge = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_Stat_MPGauge, WID_StatWnd, 50, 198+4, 191, 9, false );  
	MPGauge->SetClientImage( TID_None );
	MPGauge->SetClientColor( 255, 255, 255 );
	MPGauge->SetBarImage( TID_C_WND_STATE_SB_SPBAR );	
	MPGauge->InitProgress( 100 );
	MPGauge->SetHID( HD_S_MPGRAPH );
	//MPGauge->SetBarColor( 0, 255, 0 );
	
	SPGauge = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_Stat_SPGauge, WID_StatWnd, 50, 198+4, 191, 9, false );  
	SPGauge->SetClientImage( TID_None );
	SPGauge->SetClientColor( 255, 255, 255 );
	SPGauge->SetBarImage( TID_C_WND_STATE_SB_SPBAR );	
	SPGauge->InitProgress( 100 );
	SPGauge->SetHID( HD_S_SPGRAPH );
	
#ifdef APPLY_BOLD_FONT
	ExpGauge = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_Stat_LVgage, WID_StatWnd, 43, 278, 215, 17, false ); 
#else
	ExpGauge = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_Stat_LVgage, WID_StatWnd, 23, 278, 215, 17, false );  
#endif	
	ExpGauge ->SetClientImage( TID_None );
	ExpGauge ->SetClientColor( 255, 255, 255 );
	ExpGauge ->SetBarImage( TID_C_WND_STATE_SB_EXPBAR );	
	ExpGauge ->InitProgress( 100 );

	//SPGauge->SetBarColor( 0, 0, 255 );
	
	
	//-------------------------------------------------------------------------
	// HP
	HPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_HPText, WID_StatWnd, 148, 183+4, 90, 5, false );		
	HPText->SetFontR( n_fRsvImageNum );	
	HPText->SetHID( HD_S_HP );
	HPText->SetText( _RT("100/100") );	
	// MP
	MPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MPText, WID_StatWnd, 148, 199+4, 90, 5, false );		
	MPText->SetFontR( n_fRsvImageNum );	
	MPText->SetHID( HD_S_MP );
	MPText->SetText( _RT("100/100") );	
	// SP
	SPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_SPText, WID_StatWnd, 148, 199+4, 90, 5, false );	
	SPText->SetFontR( n_fRsvImageNum );	
	SPText->SetHID( HD_S_SP );
	SPText->SetText( _RT("100/100") );	
	
	// 2전(1승/1패)
	WarRecord = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 53, 168, 200, 17, false );
	WarRecord->SetFontR( n_fRsvFontWndStatic );
	WarRecord->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp) , _RT("") ); 
	WarRecord->SetText( temp );	
	WarRecord->SetTextColor(255,0,0);
	
#ifndef NEW_PVP_DECO
	Static_Text[24]->SetVisible(false);
	WarRecord->SetVisible(false);
#endif
	
	
	//파워 ( 밀레나 )
		
#ifdef APPLY_BOLD_FONT
		PowerText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_AttText, WID_StatWnd, 93, 320, 25, 12, false );
#else		
		PowerText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_AttText, WID_StatWnd, 83, 320, 25, 12, false );
#endif		
		PowerText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		PowerText->SetClientImage( TID_None );
		PowerText->SetClientColor( 180, 175, 160 );
		PowerText->SetAlignText( n_atLeft, n_atCenter );
		PowerText->SetText( _RT("255") );
		PowerText->SetTextColor( TEXT_COLOR_VALUE1 );
		PowerText->SetHID( HD_S_POWER );
		
		//교감 ( 레인 )
#ifdef APPLY_BOLD_FONT
		SympathyText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagText, WID_StatWnd, 93, 320, 25, 12, false );
#else
		SympathyText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagText, WID_StatWnd, 83, 320, 25, 12, false );
#endif	
		SympathyText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		SympathyText->SetClientImage( TID_None );
		SympathyText->SetClientColor( 180, 175, 160 );
		SympathyText->SetAlignText( n_atLeft, n_atCenter );
		SympathyText->SetText( _RT("255") );
		SympathyText->SetTextColor( TEXT_COLOR_VALUE1 );
		SympathyText->SetHID( HD_S_SYMPATHY );
	
		
	//체력
#ifdef APPLY_BOLD_FONT
	VitalText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_DefText, WID_StatWnd, 93, 335, 25, 12, false );
#else
	VitalText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_DefText, WID_StatWnd, 83, 335, 25, 12, false );
#endif	
	VitalText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	VitalText->SetClientImage( TID_None );
	VitalText->SetClientColor( 180, 175, 160 );
	VitalText->SetAlignText( n_atLeft, n_atCenter );
	VitalText->SetText( _RT("255") );
	VitalText->SetTextColor( TEXT_COLOR_VALUE1 );
	VitalText->SetHID( HD_S_DEFEND );
	
	//지능
#ifdef APPLY_BOLD_FONT
	IntText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_RegText, WID_StatWnd, 93, 350, 25, 12, false );
#else
	IntText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_RegText, WID_StatWnd, 83, 350, 25, 12, false );
#endif	
	IntText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	IntText->SetClientImage( TID_None );
	IntText->SetClientColor( 180, 175, 160 );
	IntText->SetAlignText( n_atLeft, n_atCenter );
	IntText->SetText( _RT("255") );
	IntText->SetTextColor( TEXT_COLOR_VALUE1 );
	IntText->SetHID( HD_S_RESIST );
	
	//지구력
#ifdef APPLY_BOLD_FONT
	StaminaText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_AgiText, WID_StatWnd, 93, 350, 25, 12, false );
#else
	StaminaText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_AgiText, WID_StatWnd, 83, 350, 25, 12, false );
#endif	
	StaminaText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	StaminaText->SetClientImage( TID_None );
	StaminaText->SetClientColor( 180, 175, 160 );
	StaminaText->SetAlignText( n_atLeft, n_atCenter );
	StaminaText->SetText( _RT("255") );
	StaminaText->SetTextColor( TEXT_COLOR_VALUE1 );
	StaminaText->SetHID( HD_S_FOCUS );
	
	//민첩
#ifdef APPLY_BOLD_FONT
	DexText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_DexText, WID_StatWnd, 238, 320, 25, 12, false );
#else	
	DexText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_DexText, WID_StatWnd, 208, 320, 25, 12, false );
#endif	
	DexText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	DexText->SetClientImage( TID_None );
	DexText->SetClientColor( 180, 175, 160 );
	DexText->SetAlignText( n_atLeft, n_atCenter );
	DexText->SetText( _RT("255") );
	DexText->SetTextColor( TEXT_COLOR_VALUE1 );
	DexText->SetHID( HD_S_REACTION );
	
	//매력
#ifdef APPLY_BOLD_FONT	
	CharismaText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_ChaText, WID_StatWnd, 238, 335, 25, 12, false );
#else
	CharismaText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_ChaText, WID_StatWnd, 208, 335, 25, 12, false );
#endif	
	CharismaText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	CharismaText->SetClientImage( TID_None );
	CharismaText->SetClientColor( 180, 175, 160 );
	CharismaText->SetAlignText( n_atLeft, n_atCenter );
	CharismaText->SetText( _RT("255") );
	CharismaText->SetTextColor( TEXT_COLOR_VALUE1 );
	CharismaText->SetHID( HD_S_CHARM );
	
	//행운
#ifdef APPLY_BOLD_FONT
	LuckText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_LukText, WID_StatWnd, 238, 350, 25, 12, false );
#else
	LuckText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_LukText, WID_StatWnd, 208, 350, 25, 12, false );
#endif	
	LuckText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	LuckText->SetClientImage( TID_None );
	LuckText->SetClientColor( 180, 175, 160 );
	LuckText->SetAlignText( n_atLeft, n_atCenter );
	LuckText->SetText( _RT("?") );
	LuckText->SetTextColor( TEXT_COLOR_VALUE1 );
	LuckText->SetHID( HD_S_LUCK );
	
	//명중
#ifdef APPLY_BOLD_FONT
	HitText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_HitText, WID_StatWnd, 93, 365, 37, 12, false );
#else
	HitText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_HitText, WID_StatWnd, 83, 365, 37, 12, false );
#endif	
	HitText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	HitText->SetClientImage( TID_None );
	HitText->SetClientColor( 180, 175, 160 );
	HitText->SetAlignText( n_atLeft, n_atCenter );
	HitText->SetText( _RT("") );
	HitText->SetTextColor( TEXT_COLOR_VALUE1 );
	HitText->SetVisible( false );
	
	/*
	CTextBox*		PhysicAttText;//물리공격력
	CTextBox*		PhysicDefText;//물리방어력
	CTextBox*		MagicDefText;//마법방어력
	
	  CTextBox*		MagicAttText2;//마법공격력
	  CTextBox*		MagicDefText2;//마법방어력
	  CTextBox*		PhysicDefText2;//물리방어력
	*/
	
	// 밀레나 레인 ( 박스가 위치 조정이 안되네 T.T 짭짭 )
	{
		//물리공격력
#ifdef APPLY_BOLD_FONT		
		PhysicAttText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_PhysicAttText, WID_StatWnd, 90, 375, 37, 12, false );
#else
		PhysicAttText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_PhysicAttText, WID_StatWnd, 83, 375, 37, 12, false );
#endif
		PhysicAttText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		PhysicAttText->SetClientImage( TID_None );
		PhysicAttText->SetClientColor( 180, 175, 160 );
		PhysicAttText->SetAlignText( n_atLeft, n_atCenter );
		PhysicAttText->SetText( _RT("65535") );
		PhysicAttText->SetTextColor( TEXT_COLOR_VALUE1 );
		
		//물리방어력
#ifdef APPLY_BOLD_FONT
		PhysicDefText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_PhysicDefText, WID_StatWnd, 90, 390, 37, 12, false );
#else
		PhysicDefText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_PhysicDefText, WID_StatWnd, 83, 390, 37, 12, false );
#endif	
		PhysicDefText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		PhysicDefText->SetClientImage( TID_None );
		PhysicDefText->SetClientColor( 180, 175, 160 );
		PhysicDefText->SetAlignText( n_atLeft, n_atCenter );
		PhysicDefText->SetText( _RT("65535") );
		PhysicDefText->SetTextColor( TEXT_COLOR_VALUE1 );
		
		//마법방어력
#ifdef APPLY_BOLD_FONT
		MagicDefText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagicDefText, WID_StatWnd, 90, 405, 37, 12, false );
#else		
		MagicDefText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagicDefText, WID_StatWnd, 83, 405, 37, 12, false );
#endif	
		MagicDefText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		MagicDefText->SetClientImage( TID_None );
		MagicDefText->SetClientColor( 180, 175, 160 );
		MagicDefText->SetAlignText( n_atLeft, n_atCenter );
		MagicDefText->SetText( _RT("65535") );
		MagicDefText->SetTextColor( TEXT_COLOR_VALUE1 );
	}
	
	{
		//마법공격력
#ifdef APPLY_BOLD_FONT
		MagicAttText2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagicAttText2, WID_StatWnd, 90, 375, 37, 12, false );
#else
		MagicAttText2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagicAttText2, WID_StatWnd, 83, 375, 37, 12, false );
#endif	
		MagicAttText2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		MagicAttText2->SetClientImage( TID_None );
		MagicAttText2->SetClientColor( 180, 175, 160 );
		MagicAttText2->SetAlignText( n_atLeft, n_atCenter );
		MagicAttText2->SetText( _RT("65535") );
		MagicAttText2->SetTextColor( TEXT_COLOR_VALUE1 );
		
		//마법방어력
#ifdef APPLY_BOLD_FONT		
		MagicDefText2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagicDefText2, WID_StatWnd, 90, 390, 37, 12, false );
#else		
		MagicDefText2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagicDefText2, WID_StatWnd, 83, 390, 37, 12, false );
#endif		
		MagicDefText2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		MagicDefText2->SetClientImage( TID_None );
		MagicDefText2->SetClientColor( 180, 175, 160 );
		MagicDefText2->SetAlignText( n_atLeft, n_atCenter );
		MagicDefText2->SetText( _RT("65535") );
		MagicDefText2->SetTextColor( TEXT_COLOR_VALUE1 );
		
		//물리방어력
#ifdef APPLY_BOLD_FONT
		PhysicDefText2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_PhysicDefText2, WID_StatWnd, 90, 405, 37, 12, false );
#else
		PhysicDefText2 = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_PhysicDefText2, WID_StatWnd, 83, 405, 37, 12, false );
#endif	
		PhysicDefText2->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		PhysicDefText2->SetClientImage( TID_None );
		PhysicDefText2->SetClientColor( 180, 175, 160 );
		PhysicDefText2->SetAlignText( n_atLeft, n_atCenter );
		PhysicDefText2->SetText( _RT("65535") );
		PhysicDefText2->SetTextColor( TEXT_COLOR_VALUE1 );
	}	
	
	//-------------------------------------------------------------------------
	//회피
	EvasionText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_EvasionText, WID_StatWnd, 198, 365, 37, 12, false );
	EvasionText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	EvasionText->SetClientImage( TID_None );
	EvasionText->SetClientColor( 180, 175, 160 );
	EvasionText->SetAlignText( n_atLeft, n_atCenter );
	EvasionText->SetText( _RT("++++++=") );
	EvasionText->SetTextColor( TEXT_COLOR_VALUE1 );
	EvasionText->SetVisible( false );
	
	//이동속도 포인트
#ifdef APPLY_BOLD_FONT	
	MoveSpeedText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MoveSpeedText, WID_StatWnd, 215, 390, 25, 12, false );
#else
	MoveSpeedText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MoveSpeedText, WID_StatWnd, 198, 390, 25, 12, false );
#endif	
	MoveSpeedText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	MoveSpeedText->SetClientImage( TID_None );
	MoveSpeedText->SetClientColor( 180, 175, 160 );
	MoveSpeedText->SetAlignText( n_atLeft, n_atCenter );
	MoveSpeedText->SetText( _RT("6553511") );
	MoveSpeedText->SetTextColor( TEXT_COLOR_VALUE1 );

	switch(g_RockClient.GetLanguage()) 
	{
	case eLang_English:
	case eLang_Singapore:
		MoveSpeedText->SetHID( HD_S_MOVE_SPEED );
		break; 
	default:break;
		
	}
	

	
	//공격속도
#ifdef APPLY_BOLD_FONT
	MagicUSpeedText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagicUSpeedText, WID_StatWnd, 215, 375, 37, 12, false );
#else
	MagicUSpeedText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagicUSpeedText, WID_StatWnd, 198, 375, 37, 12, false );
#endif	
	MagicUSpeedText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	MagicUSpeedText->SetClientImage( TID_None );
	MagicUSpeedText->SetClientColor( 180, 175, 160 );
	MagicUSpeedText->SetAlignText( n_atLeft, n_atCenter );
	MagicUSpeedText->SetText( _RT("65535") );
	MagicUSpeedText->SetTextColor( TEXT_COLOR_VALUE1 );
	
	switch(g_RockClient.GetLanguage()) 
	{
		case eLang_English:
		case eLang_Singapore:
			MagicUSpeedText->SetHID( HD_S_ATTACK_SPEED );
		break; 
		default:break;
		
	}
	
	
	///-- Highlight Image 및 state up창 호출
	//HighlightImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_Stat_LVHighlight, WID_StatWnd,  23, 278, 215, 17, false );  
	//HighlightImg->SetClientImage( TID_C_WND_STATE_SB_BAR_TW );//TID_MainMenu_LVHighlight

	SRect sTempRect;

	sTempRect.x	= 183;
	sTempRect.y	= 224;
	sTempRect.w	= 58;
	sTempRect.h	= 45;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 158;
	sTempRect.w	= 108;

#endif	
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Stat_LVBtn, WID_StatWnd, 
									 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false ); 
	btn->SetClientImage( TID_None );
	btn->SetBtnImage( TID_C_WND_ABUPBT, TID_C_WND_ABUPBT, TID_C_WND_ABUPBT_OVR, TID_None );
	btn->SetFontR( n_fRsvFontWndButton );
	btn->SetText( G_STRING_CHAR( IDS_ABILITY_UP_BTN ) );	
#ifndef MIX_ELDASTONE
	btn->SetHID(HID_CHARSTAT_STR);
#endif 
	
	///-- 경험치
	ExpText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd,  24, 280, 205, 17, false );  
	ExpText->SetFontR( n_fRsvFontWndStatic2 );
	ExpText->SetAlignText( n_atRight, n_atCenter );
	ExpText->SetFocusCheck( false );	
	
	
	/*
	//시전성공률
	MagicSRateText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_MagicSRateText, WID_StatWnd, 75, 357, 37, 12, false );
	MagicSRateText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	MagicSRateText->SetClientImage( TID_None );
	MagicSRateText->SetClientColor( 180, 175, 160 );
	MagicSRateText->SetAlignText( n_atLeft, n_atCenter );
	MagicSRateText->SetText( _RT("65535") );
	MagicSRateText->SetTextColor( TEXT_COLOR_VALUE1 );
	
	  
		//체중
		WeightText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_Stat_WeightText, WID_StatWnd, 199, 356, 25, 12, false );
		WeightText->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		WeightText->SetClientImage( TID_None );
		WeightText->SetClientColor( 180, 175, 160 );
		WeightText->SetAlignText( n_atLeft, n_atCenter );
		WeightText->SetText( _RT("65535") );
		WeightText->SetTextColor( TEXT_COLOR_VALUE1 );
		WeightText->SetHID( 96 );
		//tb->SetHID( 96 );
		
		  /*
		  //#define WID_Stat_Xprism				132//엑스프리즘
		  btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_Stat_Xprism, WID_StatWnd, 159, 257, 38, 47, false );
		  btn->SetClientImage( TID_None );
		  btn->SetBtnImage( TID_Stat_Xprism, TID_Stat_Xprism, TID_Stat_Xprism, TID_None );
		  
	*/
	///--@@@@
	/*
	UpGage = (CUpGauge*)nRui->CreateWnd( n_wtUpGauge, WID_Stat_LVgage, WID_StatWnd, 145, 185, 70, 70, false );
	UpGage->SetClientImage( TID_None );
	UpGage->SetBarImage( TID_Stat_EXGageFULL );
	UpGage->SetProgressDirection( n_PD_UP );
	UpGage->InitProgress( 0 );
	*/

#ifdef CPK_SYSTEM
	CTextBox * pStaticTextBox = NULL;
	#ifdef APPLY_BOLD_FONT
		pStaticTextBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 155, 405, 80, 13, false );
	#else
		pStaticTextBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 140, 405, 80, 13, false );
	#endif
		
	pStaticTextBox->SetFontR( n_fRsvFontWndStatic );
	pStaticTextBox->SetAlignText( n_atLeft, n_atCenter );
	pStaticTextBox->SetText(_RT("PK Point:"));					///-- HAN_MSG	
	
	#ifdef APPLY_BOLD_FONT	
		m_TxtPKPoint = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 215, 405, 25, 12, false );
	#else
		m_TxtPKPoint = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_StatWnd, 198, 405, 25, 12, false );
	#endif	
	m_TxtPKPoint->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_TxtPKPoint->SetClientImage( TID_None );
	m_TxtPKPoint->SetClientColor( 180, 175, 160 );
	m_TxtPKPoint->SetAlignText( n_atLeft, n_atCenter );
	m_TxtPKPoint->SetText( _RT("") );
	m_TxtPKPoint->SetTextColor( TEXT_COLOR_VALUE1 );
	
#endif
	
}

void CCharStateWnd::SetCitizen(int citizen)
{
	m_Citizen = citizen; 
	char temp[100];
	
	
	//.......................................................................................................
	// 분류되는 버튼 모두 닫기
	//.......................................................................................................
	MPText->SetVisible( FALSE );
	SPText->SetVisible( FALSE );
	
	SPGauge->SetVisible( FALSE );
	MPGauge->SetVisible( FALSE );
	SympathyText->SetVisible( FALSE );
	PowerText->SetVisible( FALSE );
	
	PhysicAttText->SetVisible( FALSE );//물리공격력
	PhysicDefText->SetVisible( FALSE );//물리방어력
	MagicDefText->SetVisible( FALSE );//마법방어력
	
	MagicAttText2->SetVisible( FALSE );//마법공격력
	MagicDefText2->SetVisible( FALSE );//마법방어력
	PhysicDefText2->SetVisible( FALSE );//물리방어력
	
	//.......................................................................................................
	// 국가에 따른 배경창 선택 & 버튼 
	//.......................................................................................................
	switch( m_Citizen )
	{
	case MILLENA:
		
		
		SPText->SetVisible( TRUE );
		SPGauge->SetVisible( TRUE );
		PowerText->SetVisible( TRUE );
		
		PhysicAttText->SetVisible( TRUE );//물리공격력
		PhysicDefText->SetVisible( TRUE );//물리방어력
		MagicDefText->SetVisible( TRUE );//마법방어력
		
		IntText->SetVisible( FALSE );//마법방어력
		StaminaText->SetVisible( TRUE );//마법방어력
		
		///-- SP / MP
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_SP ) );
		Static_Text[9]->SetText( temp );
		///-- 파워 / 교감
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_POWER ) );
		Static_Text[13]->SetText( temp );
		///-- 지구력 / 지능 
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_STAMINA ) );
		Static_Text[15]->SetText( temp );

		if( g_RockClient.GetLanguage() == eLang_Singapore ||
			g_RockClient.GetLanguage() == eLang_English )
		{
			///-- 물리공격력 / 마법공격력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_PHYSICAL_ATK ) );
			Static_Text[19]->SetText( temp );
			PhysicAttText->SetHID(HD_S_PHYSICAL_ATTACK);
			///-- 물리방어력 / 마법방어력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_PHYSICAL_DEF ) );
			Static_Text[20]->SetText( temp );
			PhysicDefText->SetHID(HD_S_PHYSICAL_DEFENSE);
			///-- 마법방어력 / 물리방어력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MAGIC_DEF ) );
			Static_Text[21]->SetText( temp );
			MagicDefText->SetHID(HD_S_MAGIC_DEFENSE);
		}
		else  
		{
			///-- 물리공격력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_PHYATT ) );
			Static_Text[19]->SetText( temp );
			///-- 물리방어력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ) );
			Static_Text[20]->SetText( temp );
			///-- 마법방어력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ) );
			Static_Text[21]->SetText( temp );
		}
		
		break;	
	case RAIN:
		
		MPText->SetVisible( TRUE );
		MPGauge->SetVisible( TRUE );
		SympathyText->SetVisible( TRUE );
		
		MagicAttText2 ->SetVisible( TRUE );//마법공격력
		MagicDefText2 ->SetVisible( TRUE );//마법방어력
		PhysicDefText2->SetVisible( TRUE );//물리방어력
		
		IntText->SetVisible( TRUE );
		StaminaText->SetVisible( FALSE );
		
		///-- SP / MP
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MP ) );
		Static_Text[9]->SetText( temp );
		///-- 파워 / 교감
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_SYMPATHY ) );
		Static_Text[13]->SetText( temp );
		///-- 지구력 / 지능 
		Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_INTELLI ) );
		Static_Text[15]->SetText( temp );
		
		if( g_RockClient.GetLanguage() == eLang_Singapore ||
			g_RockClient.GetLanguage() == eLang_English )
		{
		
			///마법공격력			
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MAGIC_ATK ) );
			Static_Text[19]->SetText( temp  );
			MagicAttText2->SetHID(HD_S_MAGIC_ATTACK);
			// 마법방어력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MAGIC_DEF ) );
			Static_Text[20]->SetText( temp );
			MagicDefText2->SetHID(HD_S_MAGIC_DEFENSE);
			///-- 마법방어력 / 물리방어력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_PHYSICAL_DEF ) );
			Static_Text[21]->SetText( temp );
			PhysicDefText2->SetHID(HD_S_PHYSICAL_DEFENSE);
			
		}
		else
		{
			///마법공격력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MAGATT ) );
			Static_Text[19]->SetText( temp  );
			///마법방어력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_MAGDEF ) );
			Static_Text[20]->SetText( temp );
			///물리방어력
			Rsprintf(RWCHAR( temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_PHYDEF ) );
			Static_Text[21]->SetText( temp );
			
		}
		break;	
	}
	
}

DWORD CCharStateWnd::FameColor(DWORD Fame)
{
	DWORD Color = 0 ;
	int Number  =0 ; 	
	
	if( Fame <= c_FameLv10_Range_Max )
	{
		Number = 0 ; 
	}
	//걸출한 
	else if( c_FameLv11_Range_Min <= Fame && c_FameLv11_Range_Max >= Fame )
	{
		Number = 1 ; 
	}
	//위대한 
	else if( c_FameLv12_Range_Min <= Fame && c_FameLv12_Range_Max >= Fame )
	{
		Number = 2 ; 
	}
	//추앙받는
	else if( c_FameLv13_Range_Min <= Fame && c_FameLv13_Range_Max >= Fame )
	{
		Number = 3 ; 
	}
	//영웅적인 
	else if( c_FameLv14_Range_Min <= Fame && c_FameLv14_Range_Max >= Fame )
	{
		Number = 4; 
	}
	//전설의
	else if( c_FameLv15_Range_Min <= Fame && c_FameLv15_Range_Max >= Fame )
	{
		Number = 5 ; 
	}
	//불멸의 
	else if( c_FameLv16_Range_Min <= Fame && c_FameLv16_Range_Max >= Fame )
	{
		Number = 6 ; 
	}
	else 
	{
		Number = 6 ; 
	}	
	
	switch(Number) {
	case 0:	Color = D3DCOLOR_XRGB( 230, 230, 88 ); break; 
	case 1: Color = D3DCOLOR_XRGB( 0  , 255 ,  255 ); break; 
	case 2: Color = D3DCOLOR_XRGB( 247, 148 ,  28 );break;
	case 3: Color = D3DCOLOR_XRGB( 0, 255 ,  106 );break; 
	case 4: Color = D3DCOLOR_XRGB(  255, 0 , 255 ); break; 
	case 5: Color = D3DCOLOR_XRGB( 0,  0 ,  255); break; //파랑 
	case 6: Color = D3DCOLOR_XRGB( 255  ,0  ,  0 ); break;// 빨강 
	default: 
		Color = D3DCOLOR_XRGB( 230, 230, 88 ); 
		break; 
		
	}	
	
	return Color;
}

//-----------------------------------------------------------------------------
void CCharStateWnd::Update()
{
	if( nRui->GetFocusWnd() != WID_Stat_LVBtn )
		
	{
		SetToolTipDestWnd(WID_None);
	}	
	
	static TCHAR tmpStr[128] = {0,};
	
	
	static int	 cur_percent( 0 );
	static BOOL  IsTwinkle = FALSE;
	static DWORD OldTimer  = timeGetTime(); 
	static BOOL  OnTwinkle = FALSE;
	
	if( thePcParam->Point > 0 )
	{
		IsTwinkle = TRUE;
		///MainMenuLVgageWnd.pStateOpenBtn->SetVisible( TRUE );
		
		
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , OldTimer ) , > , 500 ) )
		{
			OldTimer = timeGetTime();
			
			OnTwinkle = !OnTwinkle;
		}
		
	}
	else
	{
		IsTwinkle = FALSE;
		///MainMenuLVgageWnd.pStateOpenBtn->SetVisible( FALSE );
	}

	/*
	if( IsTwinkle == TRUE )
	{
	if( OnTwinkle == TRUE )
	{
	nRui->SendEvent( WID_MainMenu_LVgage, n_emSetPercent     , (EPARAM)0, 0, 0, 0 );
	nRui->SendEvent( WID_MainMenu_LVgage, n_emSetClientImage , (EPARAM)TID_MainMenu_LVHighlight, 0, 0, 0 );
	}
	else
	{
	nRui->SendEvent( WID_MainMenu_LVgage, n_emSetPercent     , (EPARAM)0, 0, 0, 0 );
	nRui->SendEvent( WID_MainMenu_LVgage, n_emSetClientImage , (EPARAM)TID_None, 0, 0, 0 );
	}
	}
	else
	{
	nRui->SendEvent( WID_MainMenu_LVgage, n_emSetPercent     , (EPARAM)cur_percent, 0, 0, 0 );
	nRui->SendEvent( WID_MainMenu_LVgage, n_emSetClientImage , (EPARAM)TID_None, 0, 0, 0 );
	}
	*/

	if( IsVisible == true )
	{

		//---------------------------------------------------------------------
		NameText->SetText( thePcParam->PCName );//이름

// 김정기 2009-12-30
// Deco리뉴얼 버전의 명성 시스템 설정 
#ifdef FAMOUSE_FIGURES_KJK
		
		// 처음 수치는 사용가능한 명성치 이고 두번째 나오는 수치는 캐릭의 누적 명성치
		// Deco1에서 있는 명성치에 따른 호칭은 사라진다.
		Rsprintf( RWCHAR(tmpStr), _RT("%d / %d"), thePcParam->Fame, thePcParam->TotalFame );
		
#else
		
		//		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Fame );
		
		if( c_FameLv1_Range_Min <= thePcParam->Fame && c_FameLv1_Range_Max >= thePcParam->Fame )
		{
			//			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_1 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_1 ), thePcParam->Fame, c_FameLv2_Range_Min );
		}
		else if( c_FameLv2_Range_Min <= thePcParam->Fame && c_FameLv2_Range_Max >= thePcParam->Fame )
		{
			//			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_2 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_2 ), thePcParam->Fame, c_FameLv3_Range_Min );
		}
		else if( c_FameLv3_Range_Min <= thePcParam->Fame && c_FameLv3_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_3 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_3 ), thePcParam->Fame, c_FameLv4_Range_Min );
		}
		else if( c_FameLv4_Range_Min <= thePcParam->Fame && c_FameLv4_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_4 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_4 ), thePcParam->Fame, c_FameLv5_Range_Min );
		}
		else if( c_FameLv5_Range_Min <= thePcParam->Fame && c_FameLv5_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_5 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_5 ), thePcParam->Fame, c_FameLv6_Range_Min );
		}
		else if( c_FameLv6_Range_Min <= thePcParam->Fame && c_FameLv6_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_6 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_6 ), thePcParam->Fame, c_FameLv7_Range_Min );
		}
		else if( c_FameLv7_Range_Min <= thePcParam->Fame && c_FameLv7_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_7 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_7 ), thePcParam->Fame, c_FameLv8_Range_Min );
		}
		else if( c_FameLv8_Range_Min <= thePcParam->Fame && c_FameLv8_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_8 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_9 ), thePcParam->Fame, c_FameLv9_Range_Min );
		}
		else if( c_FameLv9_Range_Min <= thePcParam->Fame && c_FameLv9_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_9 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_10 ), thePcParam->Fame, c_FameLv10_Range_Min );
		}
		else if( c_FameLv10_Range_Min <= thePcParam->Fame && c_FameLv10_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_10 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_14 ), thePcParam->Fame, c_FameLv11_Range_Min );
		}
		else if( c_FameLv11_Range_Min <= thePcParam->Fame && c_FameLv11_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_11 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_15 ), thePcParam->Fame, c_FameLv12_Range_Min );
		}
		else if( c_FameLv12_Range_Min <= thePcParam->Fame && c_FameLv12_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_12 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_16 ), thePcParam->Fame, c_FameLv13_Range_Min );
		}
		else if( c_FameLv13_Range_Min <= thePcParam->Fame && c_FameLv13_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_13 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_18 ), thePcParam->Fame, c_FameLv14_Range_Min );
		}
		else if( c_FameLv14_Range_Min <= thePcParam->Fame && c_FameLv14_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_14 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_19 ), thePcParam->Fame, c_FameLv15_Range_Min );
		}
		else if( c_FameLv15_Range_Min <= thePcParam->Fame && c_FameLv15_Range_Max >= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_15 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_17 ), thePcParam->Fame, c_FameLv16_Range_Min );
		}
		else if( c_FameLv16_Range_Min <= thePcParam->Fame )
		{
			//			sprintf( tmpStr, G_STRING_CHAR( IDS_CHARFAME_16 ), thePcParam->PCName );
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_CHARFAME_20 ), thePcParam->Fame, c_FameLv17_Range_Min );
		}		

#endif	
		
		// 명성치 텍스트 설정 
		FameText->SetText( tmpStr );
	

		if( thePcParam->Citizen == 0 )//국가
		{
			StateText->SetText( G_STRING_CHAR( IDS_RAIN ) );
			NationImg[1]->SetVisible( true );
			NationImg[0]->SetVisible( false );
		}
		else if( thePcParam->Citizen == 1 )
		{
			StateText->SetText( G_STRING_CHAR( IDS_MILLENA ) );
			NationImg[0]->SetVisible( true );
			NationImg[1]->SetVisible( false );
		}
		
		if( thePcParam->Gender == 0 )//성별
		{
			GenderText->SetText( G_STRING_CHAR( IDS_MAN ) );
		}
		else
		{
			GenderText->SetText( G_STRING_CHAR( IDS_WOMAN ) );
		}
		
		switch( thePcParam->MainClass )
		{
			//기본, 초급, 중금 직업군(Lv 1~100)
			//(기본직업)
		case Wanderer:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_WANDERER ) );		break;//= 0, 방랑자
		case Stranger:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_STRANGER ) );		break;//= 1, 이방인
			//(초급직업군)
		case Swordman:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_SWORDMAN ) );		break;//= 10, 소드맨
		case Archer:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_ARCHER ));			break;//= 11, 아쳐
		case Magician:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_MAGICIAN ) );		break;//= 12, 메지션
		case Shaman:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_SHAMAN ) );			break;//= 13, 소서러
			//(중급직업군)
		case Knight:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_KNIGHT ) );		break;//= 20, 나이트
		case Mercenary:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_MERCENARY ) );		break;//= 21, 머시너리
		case Slayer:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_SLAYER ) );		break;//= 22,  슬레이어
		case Sniper:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_SNIPER ) );		break;//= 23, 소울 브레이커
		case Prominas:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_PROMINAS ) );	break;//= 24, 프로미나스
		case Priest:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_PRIEST ) );		break;//= 25, 프리스트
		case Holy_Avenger:	ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_HOLYAVENGER ) );	break;//= 26, 홀리 어벤져
		case Psyche:		ProfessionText->SetText( G_STRING_CHAR( IDS_CLASS_PSYCHE ) );		break;//= 27, 사이키s			
		}
		///-- Sub Class
		switch(	thePcParam->SubClass )
		{
		case 	NoSubClass	: Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR( IDS_NONE ) );break;
		case 	EldaCraft	: Rsprintf( RWCHAR(tmpStr), _RT("%s %s %d"), G_STRING_CHAR( IDS_SUBCLASS_ELDA_CRAFT ), G_STRING_CHAR( IDS_LEVEL_SIMP ), thePcParam->SubLevel );break;
		case 	ArmCraft	: Rsprintf( RWCHAR(tmpStr), _RT("%s %s %d"), G_STRING_CHAR( IDS_SUBCLASS_WEAPON_CRAFT ), G_STRING_CHAR( IDS_LEVEL_SIMP ), thePcParam->SubLevel );break;
		case 	ArmorCraft	: Rsprintf( RWCHAR(tmpStr), _RT("%s %s %d"), G_STRING_CHAR( IDS_SUBCLASS_ARMOR_CRAFT ), G_STRING_CHAR( IDS_LEVEL_SIMP ), thePcParam->SubLevel );break;
		case 	Artisan		: Rsprintf( RWCHAR(tmpStr), _RT("%s %s %d"), G_STRING_CHAR( IDS_NONE ), G_STRING_CHAR( IDS_LEVEL_SIMP ), thePcParam->SubLevel );break;
		case 	Merchant	: Rsprintf( RWCHAR(tmpStr), _RT("%s %s %d"), G_STRING_CHAR( IDS_NONE ), G_STRING_CHAR( IDS_LEVEL_SIMP ), thePcParam->SubLevel );break;
		case 	Leader		: Rsprintf( RWCHAR(tmpStr), _RT("%s %s %d"), G_STRING_CHAR( IDS_NONE ), G_STRING_CHAR( IDS_LEVEL_SIMP ), thePcParam->SubLevel );break;
		case 	Negotiator	: Rsprintf( RWCHAR(tmpStr), _RT("%s %s %d"), G_STRING_CHAR( IDS_NONE ), G_STRING_CHAR( IDS_LEVEL_SIMP ), thePcParam->SubLevel );break;
		case 	Cook		: Rsprintf( RWCHAR(tmpStr), _RT("%s %s %d"), G_STRING_CHAR( IDS_NONE ), G_STRING_CHAR( IDS_LEVEL_SIMP ), thePcParam->SubLevel );break;
		case 	Scientist	: Rsprintf( RWCHAR(tmpStr), _RT("%s %s %d"), G_STRING_CHAR( IDS_NONE ), G_STRING_CHAR( IDS_LEVEL_SIMP ), thePcParam->SubLevel );break;
		default: Rsprintf( RWCHAR(tmpStr),  G_STRING_CHAR( IDS_NONE ) );break;	
		}
		SubClass->SetText( tmpStr );	
		
		
		int NeedSubExp	= g_Pc_Manager.GetStatNeedSubExp( thePcParam->SubClass, thePcParam->SubLevel );
		
		///-- Exp
#ifdef EXP_INT64

		LONGLONG NeedExp	= g_Pc_Manager.GetStatNeedExp64( thePcParam->MainClass, thePcParam->Level );
		LONGLONG CurExp		= thePcParam->Exp;	
		cur_percent	= (int)(100 * ( (double)CurExp / (double)NeedExp ));
		
		Rsprintf( RWCHAR(tmpStr), _RT( " %I64u  /  %I64u " ), CurExp, NeedExp);
#else 
		
		DWORD NeedExp	= g_Pc_Manager.GetStatNeedExp( thePcParam->MainClass, thePcParam->Level );
		DWORD CurExp	= thePcParam->Exp;	
		cur_percent	= (int)(100 * ( (double)CurExp / NeedExp ));

		Rsprintf( RWCHAR(tmpStr), _RT( " %u  /  %u " ), CurExp, NeedExp);
#endif  
		
		ExpText->SetText( tmpStr );
		
		///-- Sub Exp
		Rsprintf( RWCHAR(tmpStr), _RT( " %u  /  %u " ), thePcParam->SubExp, NeedSubExp);
		SubExp->SetText( tmpStr );	
		
		
#ifdef NEW_PVP_DECO
		static char temp[100]={0,		};
		int ToTalCount = nRui->thePcParam.TotalFCount;
		int WinCount = nRui->thePcParam.WinFCount;
		
		float  WinPercent = ((float)WinCount/(float)ToTalCount)*100.f;
		
		if( ToTalCount == 0)
			WinPercent = 0.0f;
		
		
		Rsprintf(RWCHAR(temp), G_STRING_CHAR(IDS_WND_WIN_LOSE), ToTalCount , WinCount , ToTalCount-WinCount , WinPercent ); 
		WarRecord->SetText( temp );	
#endif	
		
		if(nRui->thePcInven.FrontierInfo)
		{
			GuildText->SetText(nRui->thePcInven.FrontierInfo->theFrontierName);
		}
		else
		{	
			//아직 정의되지 않았다.
			GuildText->SetText( G_STRING_CHAR( IDS_NONE ) );//길드*
		}
		
		
		StatText->SetText( _RT("") );//상태 - // Status
		
		IconImg;		//상태아이콘
		
						/*
						// 지속마법의 남은 시간(초) 저장
						WORD			thePoison;	// 일정시간마다 일정량의 HP 감소
						WORD			theBlind;	// 명중률과 회피율 감소, 크리티컬 미스율 상승
						WORD			theSlow;	// 이동속도, 공격속도 감소
						WORD			theHorror;	// 공포대상으로부터 도망. 조작불능
						WORD			theCurse;	// 공격력, 방어력 감소(물리, 마법 2계열)
						WORD			theStun;	// 일시적 조작불능
						WORD			theSilence;	// 주문영창 불능
						WORD			theDying;	// 일정시간후 사망
						WORD			theInsensible;	// 일정시간후, 혹은 충격시 회복
						WORD			theRegenerate;	// 일정시간마다 일정량의 HP 증가
						WORD			theBless;	// 공격력, 방어력 상승
						WORD			theStamina;	// 주문영창의 실패율 감소, 마법공격력 상승
		//*/
		
		//---------------------------------------------------------------------
		///-- 국가공헌도
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Contribution );
		ContributionText->SetText( tmpStr );
		
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Power );//파워
		PowerText->SetText( tmpStr );
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Vital );//방어
		VitalText->SetText( tmpStr );
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Sympathy );//교감
		SympathyText->SetText( tmpStr );
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Intel );//지능
		IntText->SetText( tmpStr );
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Stamina );//지구력
		StaminaText->SetText( tmpStr );
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Dex );//민첩
		DexText->SetText( tmpStr );
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Charisma );//매력
		CharismaText->SetText( tmpStr );
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Luck );//행운
		LuckText->SetText( tmpStr );
		
		//---------------------------------------------------------------------
		// 기본 수치에 아이템의 조합에 따라....그 맥스값을 구한다.
		Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), (int)thePcParam->Stat1.theHP, thePcParam->MaxHP );
		HPText->SetText( tmpStr );
		Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), (int)thePcParam->Stat1.theMP, thePcParam->MaxMP );
		MPText->SetText( tmpStr );
		Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), (int)thePcParam->Stat1.theSP, thePcParam->MaxSP );
		SPText->SetText( tmpStr );
		
		
		static int	PercentageHPValue = 0;
		static int	PercentageMPValue = 0;
		static int	PercentageSPValue = 0;
		if( thePcParam->MaxHP )
			PercentageHPValue = (int)( ( thePcParam->Stat1.theHP * 100 ) / thePcParam->MaxHP );
		if( thePcParam->MaxMP )
			PercentageMPValue = (int)( ( thePcParam->Stat1.theMP * 100 ) / thePcParam->MaxMP );
		if( thePcParam->MaxSP )
			PercentageSPValue = (int)( ( thePcParam->Stat1.theSP * 100 ) / thePcParam->MaxSP );
		
		
		HPGauge->SetPercent( PercentageHPValue );		
		MPGauge->SetPercent( PercentageMPValue );
		SPGauge->SetPercent( PercentageSPValue );		
		
		//---------------------------------------------------------------------
		// 기본 수치에 아이템의 조합에 따라....그 합계를 계산한다.
		
		//물리공격력
		Rsprintf( RWCHAR(tmpStr), _RT("%d - %d"), thePcParam->Stat2.theMinAttackPower, thePcParam->Stat2.theMaxAttackPower );
		PhysicAttText->SetText( tmpStr );
		
		//물리방어력
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Stat2.theAttackDef );
		PhysicDefText ->SetText( tmpStr );
		PhysicDefText2->SetText( tmpStr );
		
		//마법공격력
		Rsprintf( RWCHAR(tmpStr), _RT("%d - %d"), thePcParam->Stat2.theMinMagicPower, thePcParam->Stat2.theMaxMagicPower );
		MagicAttText2->SetText( tmpStr );
		
		//마법방어력
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Stat2.theMagicRes );
		MagicDefText ->SetText( tmpStr );
		MagicDefText2->SetText( tmpStr );
		
		
		///Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Stat2.theAttackHit );
		///HitText->SetText( tmpStr );//명중
		///Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Stat2.theAttackEvade );
		///EvasionText->SetText( tmpStr );//회피
		
		// 시전속도
		//		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Stat2.theAttackSpeed );		// 수치로 표현 
		GetAttackSpeedStr( tmpStr, thePcParam->Stat2.theAttackSpeed );
		MagicUSpeedText->SetText( tmpStr );
		
		//		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Stat2.theMagicHit );
		//		MagicSRateText->SetText( tmpStr );//시전성공률
		
		//레벨
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Level );
		LevelText->SetText( tmpStr );
		
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Point );
		AbilityUpPointText->SetText( tmpStr );//능력업 포인트
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->SkillPoint );
		SkillUpPointText->SetText( tmpStr );//스킬업 포인트 
		
		//이동속도 포인트		
		//		Rsprintf( RWCHAR(tmpStr), _RT("%d"), thePcParam->Stat2.theMoveSpeed );		// 수치로 표현 
		GetMoveSpeedStr( tmpStr, thePcParam->Stat2.theMoveSpeed );
		MoveSpeedText->SetText( tmpStr );
		
		ExpGauge->SetPercent( cur_percent );
		
		if( IsTwinkle == TRUE )
		{
			if( OnTwinkle == TRUE )
			{
				//HighlightImg->SetVisible( true );
				///--@@@@ UpGage->SetPercent( 0 );
				///--@@@@UpGage->SetClientImage( TID_Stat_EXGageSPOT );
			}
			else
			{
				//HighlightImg->SetVisible( false );
				///--@@@@UpGage->SetPercent( 0 );
				///--@@@@UpGage->SetClientImage( TID_None );
			}
		}
		else
		{
			//HighlightImg->SetVisible( false );
			//ExpGauge->SetPercent( cur_percent );
			///--@@@@UpGage->SetPercent( cur_percent );
			///--@@@@UpGage->SetClientImage( TID_None );
		}

#ifdef CPK_SYSTEM
		Rsprintf( RWCHAR(tmpStr), _RT("%u"), thePcParam->thePKPoint );
		m_TxtPKPoint->SetText( tmpStr );
#endif

	}
}

//-----------------------------------------------------------------------------
void CCharStateWnd::Proc( SEventMessage* EMsg )
{
	static TCHAR		tmpStr[40] = {0,};
	
	switch( EMsg->FocusWnd )
	{
	case WID_Stat_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_StatWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
		break;
	case WID_Stat_LVBtn://
		
		SetToolTipDestWnd( WID_Stat_LVBtn );
		
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			AbilityUpWnd.LvPoint		= thePcParam->Point;
			AbilityUpWnd.cLvPoint		= thePcParam->Point;
			
			AbilityUpWnd.cLvMin			= 0;
			AbilityUpWnd.cLvMax			= thePcParam->Point;
			
			
			//초기값....min과 같다....
			AbilityUpWnd.Power.value	= thePcParam->Power;
			AbilityUpWnd.Vital.value	= thePcParam->Vital;
			AbilityUpWnd.Stamina.value	= thePcParam->Stamina;
			AbilityUpWnd.Dex.value		= thePcParam->Dex;
			AbilityUpWnd.Sympathy.value	= thePcParam->Sympathy;
			AbilityUpWnd.Intel.value	= thePcParam->Intel;
			
			AbilityUpWnd.Power.min		= thePcParam->Power;
			AbilityUpWnd.Vital.min	= thePcParam->Vital;
			AbilityUpWnd.Stamina.min		= thePcParam->Stamina;
			AbilityUpWnd.Dex.min	= thePcParam->Dex;
			AbilityUpWnd.Sympathy.min	= thePcParam->Sympathy;
			AbilityUpWnd.Intel.min		= thePcParam->Intel;
			
			//max는 현재 값에서...UP할 수 있는 포인트를 더한값....
			AbilityUpWnd.Power.max		= AbilityUpWnd.Power.min + AbilityUpWnd.LvPoint;
			AbilityUpWnd.Vital.max	= AbilityUpWnd.Vital.min + AbilityUpWnd.LvPoint;
			AbilityUpWnd.Stamina.max		= AbilityUpWnd.Stamina.min + AbilityUpWnd.LvPoint;
			AbilityUpWnd.Dex.max	= AbilityUpWnd.Dex.min + AbilityUpWnd.LvPoint;
			AbilityUpWnd.Sympathy.max	= AbilityUpWnd.Sympathy.min + AbilityUpWnd.LvPoint;
			AbilityUpWnd.Intel.max		= AbilityUpWnd.Intel.min + AbilityUpWnd.LvPoint;
			
			
			AbilityUpWnd.Power_spin->SetValue( AbilityUpWnd.Power.value, AbilityUpWnd.Power.min, AbilityUpWnd.Power.max );
			AbilityUpWnd.Vital_spin->SetValue( AbilityUpWnd.Vital.value, AbilityUpWnd.Vital.min, AbilityUpWnd.Vital.max );
			AbilityUpWnd.Stamina_spin->SetValue( AbilityUpWnd.Stamina.value, AbilityUpWnd.Stamina.min, AbilityUpWnd.Stamina.max );
			AbilityUpWnd.Dex_spin->SetValue( AbilityUpWnd.Dex.value, AbilityUpWnd.Dex.min, AbilityUpWnd.Dex.max );
			AbilityUpWnd.Sympathy_spin->SetValue( AbilityUpWnd.Sympathy.value, AbilityUpWnd.Sympathy.min, AbilityUpWnd.Sympathy.max );
			AbilityUpWnd.Int_spin->SetValue( AbilityUpWnd.Intel.value, AbilityUpWnd.Intel.min, AbilityUpWnd.Intel.max );
			
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d"), AbilityUpWnd.LvPoint );
			AbilityUpWnd.Point_Txt->SetText( tmpStr );
			
			AbilityUpWnd.Select_Race( thePcParam->Citizen );
			
			nRui->SendEvent( WID_AbilityUpWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
			//능력업창을 모달리스로 전환
			//nRui->SetModal( WID_AbilityUpWnd );
		}
		break;
	}
}

void CCharStateWnd::SetToolTipDestWnd( UINT WIDToolTipDestWnd )
{
	if( m_WIDToolTipDestWnd != WIDToolTipDestWnd )
	{
		m_WIDToolTipDestWnd = WIDToolTipDestWnd;
		m_fCumulTime = 0.0f;
		m_fRatio = 0.0f;
	}
}

///---------------------------------------------------------------------------
///-- RenderToolTipName
///---------------------------------------------------------------------------
///-- 4차 까지 쓰던 EXP Tooltip Routine
void CCharStateWnd::RenderToolTipName()
{
/* 5차 부터 사용하지 않음
if(!IsVisible)
{
SetToolTipDestWnd(WID_None);
return;
}

  static TCHAR strTemp[256] = _T("Exp");
  static TCHAR strToolTip[512] = _T("");
  float fFadingTime = 0.5f;
  int iTempXpos = 0;
  int iTempYpos = 0;
  int iToolTipXpos = 0;
  int iToolTipYpos = 0;
  
	if( m_WIDToolTipDestWnd != WID_None )
	{
	CWnd *pFDestWnd = Container->Get( WID_Stat_LVgage );
	SRect WcSize;
	pFDestWnd->GetClientSize( &WcSize );
	
	  int iNeedExp	= g_Pc_Manager.GetStatNeedExp( thePcParam->MainClass, thePcParam->Level );
	  int iCurExp	= thePcParam->Exp;
	  
		sprintf(strToolTip, _T("%d/%d"), iCurExp, iNeedExp);
		
		  D3DCOLOR tColor = D3DCOLOR_ARGB( (int)(m_fRatio * 255), 255, 200, 0 );	
		  
			SStringInfo StrInfoTemp, StrInfoToolTip;		
			theFontg->GetStringInfo( strTemp, &StrInfoTemp );
			theFontg->GetStringInfo( strToolTip, &StrInfoToolTip );
			
			  iTempXpos = WcSize.x + ((WcSize.w - StrInfoTemp.Width) / 2 );
			  iTempYpos = WcSize.y + ((WcSize.h - StrInfoTemp.Height) / 2 ) - (StrInfoTemp.Height / 2);
			  
				iToolTipXpos = WcSize.x + ((WcSize.w - StrInfoToolTip.Width) / 2 );
				iToolTipYpos = WcSize.y + ((WcSize.h - StrInfoToolTip.Height) / 2 ) + (StrInfoToolTip.Height / 2);		
				
				  Render->DrawText( theFontg, strTemp , iTempXpos, iTempYpos, tColor);	
				  Render->DrawText( theFontg, strToolTip , iToolTipXpos, iToolTipYpos, tColor);		
				  
					if( m_fRatio <= 1.0f )
					{
					m_fCumulTime += g_GTimer.GetFrameElapsedSecTime();
					m_fRatio = ( m_fCumulTime / fFadingTime );		
					if( m_fRatio > 1.0f )
					m_fRatio = 1.0f;
					}		
					}
	*/
}

//-----------------------------------------------------------------------------

void CCharStateWnd::GetAttackSpeedStr( TCHAR* aAttackSpeedStr, int aAttackSpeed )
{
/*
공격속도 표현.
0~2 = 매우 빠름
3~5 = 약간 빠름
6~8 = 빠름
9~12 = 보통
13~15 = 조금 느림
16~20 = 느림
	*/
	
	if( aAttackSpeed >= 0 && aAttackSpeed <= 2 )
	{
		Rsprintf( RWCHAR(aAttackSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_VFAST ) );
	}
	else if( aAttackSpeed >= 3 && aAttackSpeed <= 5 )
	{
		Rsprintf( RWCHAR(aAttackSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_FAST ) );
	}
	else if( aAttackSpeed >= 6 && aAttackSpeed <= 8 )
	{
		Rsprintf( RWCHAR(aAttackSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_LFAST ) );
	}
	else if( aAttackSpeed >= 9 && aAttackSpeed <= 12 )
	{
		Rsprintf( RWCHAR(aAttackSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_NORMAL ) );
	}
	else if( aAttackSpeed >= 13 && aAttackSpeed <= 15 )
	{
		Rsprintf( RWCHAR(aAttackSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_LSLOW ) );
	}
	else if( aAttackSpeed >= 16 && aAttackSpeed <= 18 )
	{
		Rsprintf( RWCHAR(aAttackSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_SLOW ) );
	}	
	else if( aAttackSpeed >= 19 && aAttackSpeed <= 20 )
	{
		Rsprintf( RWCHAR(aAttackSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_VSLOW ) );
	}	
	else 
	{
		Rsprintf( RWCHAR(aAttackSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_NORMAL ) );
	}
	
	return;
}

void CCharStateWnd::GetMoveSpeedStr( TCHAR* aMoveSpeedStr, int aMoveSpeed )
{
/*	
이동속도 표현.
0 ~ 100 = 매우 빠름
101~140 = 약간 바름
141~160 = 빠름
161~200 = 보통 
201~240 = 조금 느림
241~280 = 느림 
281~360 = 매우 느림 
	*/
	if( aMoveSpeed >= 0 && aMoveSpeed <= 100 )
	{
		Rsprintf( RWCHAR(aMoveSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_VFAST ) );
	}
	else if( aMoveSpeed >= 101 && aMoveSpeed <= 140 )
	{
		Rsprintf( RWCHAR(aMoveSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_FAST ));
	}
	else if( aMoveSpeed >= 141 && aMoveSpeed <= 160 )
	{
		Rsprintf( RWCHAR(aMoveSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_LFAST ) );
	}
	else if( aMoveSpeed >= 161 && aMoveSpeed <= 200 )
	{
		Rsprintf( RWCHAR(aMoveSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_NORMAL ) );
	}
	else if( aMoveSpeed >= 201 && aMoveSpeed <= 240 )
	{
		Rsprintf( RWCHAR(aMoveSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_LSLOW ) );
	}
	else if( aMoveSpeed >= 241 && aMoveSpeed <= 280 )
	{
		Rsprintf( RWCHAR(aMoveSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_SLOW ) );
	}	
	else if( aMoveSpeed >= 281 && aMoveSpeed <= 360 )
	{
		Rsprintf( RWCHAR(aMoveSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_VSLOW ) );
	}	
	else 
	{
		Rsprintf( RWCHAR(aMoveSpeedStr), G_STRING_CHAR( IDS_CHARSTATE_SPD_NORMAL ) );
	}
	
	return;
}