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
	n_MESSAGEBOX_INVENTORY_ITEMDROP ,				///-- �κ��丮���� ������ ������ 
	n_MESSAGEBOX_INVENTORY_W_ITEMDROP,				///-- �κ� ���� ���� ������
	n_MESSAGEBOX_INVENTORY_R_ITEMDROP,
	n_MESSAGEBOX_FRONTIER_SECESSION,				///-- ��忡�� Ż���ϱ� 
	n_MESSAGEBOX_FRONTIER_KICK,						///-- ��� ���� Ż�� ó��
	n_MESSAGEBOX_SEPERATE_COMBINE_FRON,				///-- ���ձ�� ����
	n_MESSAGEBOX_CHANGE_ELDA_COLOR,					///-- ���ٻ��� ���� 
	n_MESSAGEBOX_CHANGE_GUILDMASTER,                ///-- ��帶���� ����
	n_MESSAGEBOX_CHANGE_NOTICE,                     ///-- �������׺���
	n_MESSAGEBOX_CHANGE_MYINFO,                     ///-- �ڱ�Ұ��� �ۼ�
	
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

	///-- SetParam ���� ����
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