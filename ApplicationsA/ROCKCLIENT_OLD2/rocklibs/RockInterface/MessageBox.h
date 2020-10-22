#if !defined( __RUI_MESSAGEBOX__ )
#define __RUI_MESSAGEBOX__
//-----------------------------------------------------------------------------
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
#include "Spin.h"
#include "Wnd.h"

#include "WndProc.h"

enum N_MESSAGEBOX_INPUT_STATE
{
	n_MESSAGEBOX_NONE				= 0,
	n_MESSAGEBOX_INVENTORY_ITEMDROP ,				///-- 인벤토리에서 아이템 버리기 
	n_MESSAGEBOX_INVENTORY_W_ITEMDROP,				///-- 인벤 웨어 에서 버리기
	n_MESSAGEBOX_INVENTORY_R_ITEMDROP,
	n_MESSAGEBOX_FRONTIER_SECESSION,				///-- 길드에서 탈퇴하기 
	n_MESSAGEBOX_FRONTIER_KICK,						///-- 길드 강제 탈퇴 처리
	n_MESSAGEBOX_SEPERATE_COMBINE_FRON,				///-- 연합길드 해제
	n_MESSAGEBOX_CHANGE_ELDA_COLOR,					///-- 엘다색깔 변경 
	n_MESSAGEBOX_CHANGE_GUILDMASTER,                ///-- 길드마스터 변경
	n_MESSAGEBOX_CHANGE_NOTICE,                     ///-- 공지사항변경
	n_MESSAGEBOX_CHANGE_MYINFO,                     ///-- 자기소개서 작성
	
};

typedef enum NMB_BtnType
{
	n_MB_OK = 0,
	n_MB_OKCANCEL,
	n_MB_YESNO,
	n_MB_YESNOCANCEL,

} NMB_BtnType;


///---------------------------------------------------------------------------
///-- CMessageBox 
///---------------------------------------------------------------------------
class CMessageBox : public CWndProc
{
public:
	
	///--Composition
	virtual void Composition();
	
	///-- Proc
	virtual void Proc( SEventMessage* EMsg );
	
	///-- MessageBox
	virtual void OpenMessageBox( N_MESSAGEBOX_INPUT_STATE ae_instate, void *papt_str, const int al_width, const int al_height , void *apt_title_D = NULL, NMB_BtnType aBtnType = n_MB_OKCANCEL );

	///-- SetParam 수정 가능
	virtual void SetParam( const DWORD a32_param1, const DWORD a32_param2 = 0, const DWORD a32_param3 = 0 );
	virtual void SetStrParam( char* aParam1, char* aParam2 = NULL, char* aParam3 = NULL );
	
	void OkClose();
	
protected: 
	
	void _SetMessageBoxSize( const int al_width, const int al_height );
	
	
	CFrameWnd*				mpC_framewnd;
	CButton*				mpC_BT_OK;
	CButton*				mpC_BT_CANCEL;
	CTextBox*				mpc_TextBox;

	CImageBox9*				m_Img9BK;
	
	N_MESSAGEBOX_INPUT_STATE me_instate;

	DWORD					m32_param[3];
	char					mStr_param[3][255];
	
	NMB_BtnType				m_BtnType;
};


extern	CMessageBox		g_MessageBox;

//-----------------------------------------------------------------------------
#endif  __RUI_MESSAGEBOX__