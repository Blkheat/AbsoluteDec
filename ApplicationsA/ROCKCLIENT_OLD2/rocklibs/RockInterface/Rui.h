///////////////////////////////////////////////////////////////////////////////
///
///		File		: Rui.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __RUI_H__
#define __RUI_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#include "Define.h"
#include "DefineHID.h"
#include "..\\..\\CLogo2DEffect.h"

#include "Util.h"
#include "Render.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"
#include "Input.h"
#include "Fontman.h"

#include "Wnd.h"
#include "Slot.h"

#include "HelpStringTip.h"
#include "HelpItemTip.h"
#include "DlgBalloon.h"
#include "NonPlayerInfoMiniBar.h"
#include "Combo2DTimingEffect.h"
#include "BZoneIN2DEffect.h"

#include "CharIdRenderer.h"

//#include "..\\..\\pc.h"
#include ".\\..\\bravolibs\\obj\\player_manager.h"
#include "..\\..\\bravolibs\\obj\\player.h"
#include "..\\..\\bravolibs\\obj\\nonplayer.h"
#include "defineHID_ID.h"
enum NEnterFlag
{
	n_nfUIEnter,
	n_nfEditEnter
};


enum NEffect
{
	n_efNone,
	n_efFadeIn,
	n_efFadeOut
};

enum NVIEW_PCID
{
	n_VIEW_NONE_PCID,
	n_VIEW_ALL_PCID,
	n_VIEW_PICK_PCID
};

enum N_RESOLUTION
{
	n_800X600,
	n_1024X768	
};

enum N_EVENTRESULT
{
	n_ERESULT_NONE,
	n_ERESULT_UIPROC,
	n_ERESULT_ITEM_DROP,
	n_ERESULT_ERROR
};


struct SPanel
{
	bool		IsAction;
	NEffect		Effect;
	TEXID		Img;
	ISIZE		Size;
	D3DCOLOR	Color;
};


struct SMessageStr
{
	TCHAR		Text[256];
	int			x;
	int			y;
	D3DCOLOR	Color;
};

struct MopInfo
{
	int Number;
	int PosX;
	int PosY;
	float fMopSkill;
	float fMopAttack;
	float fMopAttacked;
	float fMopWait;
	float fMopDie;
	float fMopMove;
	
};



struct KeyData
{
	bool	bflag;
	DWORD	dwTime;

	KeyData()
	{
		Clear();
	}
	void Clear()
	{
		bflag =false;
		dwTime = 0;
	}
	void SetLock(DWORD Time)
	{
		dwTime = Time;
		bflag = true;
	}
	void SetUnLock()
	{
		Clear();
	}
	bool GetCheckTime(DWORD dwCurTime)
	{
		if(bflag)
		{
			if( SAFE_TIME_COMPARE( dwCurTime , > , SAFE_TIME_ADD( dwTime , 3000 ) ) )
			{
				SetUnLock();
				return true;
			}
			
			return false;
		}
		else
		{

			return true;
		}
	}

};




struct KeyLock
{
	KeyData QuickSlotKey;
	KeyData Inventory;
	KeyData Warehouse;
	KeyData GuildInven;
};



#define CHT_IMEFILENAME1    "TINTLGNT.IME" // New Phonetic
#define CHT_IMEFILENAME2    "CINTLGNT.IME" // New Chang Jie
#define CHT_IMEFILENAME3    "MSTCIPHA.IME" // Phonetic 5.1
#define CHS_IMEFILENAME1    "PINTLGNT.IME" // MSPY1.5/2/3
#define CHS_IMEFILENAME2    "MSSCIPYA.IME" // MSPY3 for OfficeXP

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
#define MAKEIMEVERSION( major, minor ) \
( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

#define IMEID_CHT_VER42 ( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )	// New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
#define IMEID_CHT_VER43 ( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )	// New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
#define IMEID_CHT_VER44 ( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )	// New ChanJie IME98b          : 4.4.x.x // WinXP
#define IMEID_CHT_VER50 ( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )	// New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
#define IMEID_CHT_VER51 ( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )	// New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
#define IMEID_CHT_VER52 ( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )	// New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
#define IMEID_CHT_VER60 ( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )	// New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
#define IMEID_CHS_VER41	( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )	// MSPY1.5	// SCIME97 or MSPY1.5 (w/Win98, Office97)
#define IMEID_CHS_VER42	( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )	// MSPY2	// Win2k/WinME
#define IMEID_CHS_VER53	( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )	// MSPY3	// WinXP

#define LCID_INVARIANT MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT)




const WORD c_Max_ItemDelayInfo( 50 );

///-- Mini_game Class 
class CMiniGame_Elda;
				
///---------------------------------------------------------------------------
///-- CRui
///---------------------------------------------------------------------------
extern int g_nCandidate_Page_Size;

class CRui
{
public:
	CRui(){}
#ifdef DIRECT_VERSION_9_MJH
	CRui( LPDIRECT3DDEVICE9 D3DDevice );
#else
	CRui( LPDIRECT3DDEVICE8 D3DDevice );
#endif // DIRECT_VERSION_9_MJH
	~CRui();

	//-------------------------------------------------------------------------
	bool CreateDesktop( HINSTANCE hInst, HWND hWnd, int Width, int Height, int Bits );
	bool CreateWnd();
	CWnd* CreateWnd( NWindowType WType, WNDID Iid, WNDID Pid, float Xpos, float Ypos, int Width, int Height, bool isRatio );

#ifdef UI_CONTROL_RENEWAL
	CWnd* CreateControl( DWORD Defineid );
	CWnd* CreateControlRepeetY( DWORD Defineid, DWORD Count, DWORD Num );
	CWnd* CreateControlRepeetX( DWORD Defineid, DWORD Count, DWORD Num );
	CWnd* CreateControlGetID( DWORD Defineid, WNDID Pid );
#endif // UI_CONTROL_RENEWAL
	
	N_EVENTRESULT EventProc( SKeys* Keys, SMouse* Mouse );
	void BillUIRenderProc(); //by simwoosung ������ UI �������ϴ� �Լ�
	void LastRenderProc();   //by simwoosung �ø��� ������ �ٷ� �������� �Լ� - Ŀ��������
	void RenderProc();
	void RenderLogo(int nAlpha);

	IRESULT SendEvent( WNDID Wid, NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );
	IRESULT ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam );
	//-------------------------------------------------------------------------
	
	void ModeInitialize( DWORD gamemode );
	
	void TextOut( void* str, int x, int y, D3DCOLOR color );
	void TextOutChat( void* str, D3DCOLOR color );
	
	void MessageBox( int msg_id, TCHAR* text, TCHAR* caption, TEXID icon_type );
	void SetDlgBalloon( NCHARTYPE ctype, DWORD id );
	void DeleteDlgBalloon( NCHARTYPE ctype, DWORD id );
	
			
	//-------------------------------------------------------------------------
	
	WNDID IsFocus( int Mx, int My );
	bool IsEditFocus();
	HWND GetWndHandle();
	HWND GetCurrentWndHandle();

	N_RESOLUTION GetResolution();//�ػ� ������
	//-------------------------------------------------------------------------

	//�׷�ȭ....
	void SetGroupWnd( int Count, WNDID SelectedWnd, ... );
	void SetEnterFlag( NEnterFlag EnterFlag );
	void SetEnterWndID( WNDID EnterWndID );


	//MessageBox
	void SetModal( WNDID modal_wid );

	// UIData Update
	void Update2DEffect();
	void UpdatePcData();
	
	// PcData
	void InitMyPcData();//SPcDataParam, SPcDataInven�� ����Ÿ �ʱ�ȭ....
	void SetMyPcData( SRpdPCData* pcdata, int pcdata_size );// ���� ���ӽ� ���۹��� ĳ���� �����͸� �����Ѵ�.
	void SetPcInfo( SPcInfo* pcinfo );
	void SetPickPcIdx( int index );

	SPcInfo*		GetPcInfo();
	SPcDataParam*	GetPcParamInfo();
	SPcDataInven*	GetPcInvenInfo();
	
	

	//
	void SetCursorType( NCursorType cursor_type );
	void SetCursorItem( SPcItem* pcitem );
	SPcItem* GetCursorItem();
	
	//by simwoosung
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DSURFACE9 m_pCursorBitmap;
#else
	LPDIRECT3DSURFACE8 m_pCursorBitmap;
#endif // DIRECT_VERSION_9_MJH
	bool SetDeviceCursor(UINT Tid, bool IsItem);
	
	void SetItemSelectedSlot( CSlot* slot );
	CSlot* GetItemSelectedSlot();

	void ResetCursorItem();


	/// �ӽ� : by sooree 
	void				EnableBGround( bool aFlag )		{	theBGround.IsAction = aFlag;	}
	void				EnableFGround( bool aFlag )		{	theBGround.IsAction = aFlag;	}

	
	
	void	MiniGameStartUp( const int ai_game_num, const int ai_arg1, BYTE theGameTimeRatio = 100 );	///-- MiniGame Start 	
	void	MiniGameEnd( const int ai_arg1 );								///-- MiniGame End 
	void	ItemPickUp();


	bool	m_IsSurpportHCursor;
	BYTE	m_WorldIndex;
	BYTE	m_ChannelIndex;

	// dongs NPC ��� ������ ��� �ϱ� ���� ..
	int thePcinfoCount;
	int TestNumFame;

	//bool	LoadStoreTable( char* file_pos );
	SDesktop			Desktop;
	SPcItem*			thePcItem;

#ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)
	
	float				m_fResolutionRateW;// 1024 �� �������� Ŭ���̾�Ʈ ������ �� ������
	float				m_fResolutionRateH;// 768 �� �������� Ŭ���̾�Ʈ ������ ���� ������
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/22)

	//SStoreList			theStoreList;

	SPcInfo*			thePcInfo;
	SPcInfo				thePickedPcInfo;

	
	int					thePickPcIdx;
	NVIEW_PCID			theViewPcInfo;

	CChain				Chain;
	
	//Ŀ�� ����
	BYTE				theCursorState;
	void SetCursorState();

	// �ٸ� ���� ���� ��
	CNonPlayerInfoMiniBar theNonPlayerInfoMiniBar;

	CSlot				theSlot;  //ItemSelectedSlot

	///-- Mini_Game 
	bool m_bOnMiniGame;				///-- flag
	CMiniGame_Elda *theMiniGame;	///-- Pointer of Instance
		
	void Refresh_ItemHelpTip(){ theItemTip->Refresh(); };
	
	//�޺�Ÿ�̹� ����
	CCombo2DTimingEffect	m_Combo2DTimingEffect;
	//��Ʋ�� ���� ����Ʈ ����
	CBZoneIn2DEffect		m_BZoneIn2DEffect;

	//ĳ���� ���̵� ������
	CCharIdRenderer			m_CharIdRenderer;	

	bool					m_bIsShowUI;

	MopInfo					m_BugMop;	//��� ���� ���Ҹ� ���� Ȯ�� .. 


	//Ư�� Ű �� ������ ����Ҷ� .. ���� �ɾ �������� ���������� �ٸ�ó���� ���� ���� . 
	KeyLock					m_KeyLock;



	SPcDataParam *      GetPcParam()	{	return &thePcParam;  }
	SPcDataInven *		GetPcInven()	{	return &thePcInven;  }

	SPcDataParam		thePcParam;// ĳ���� �Ӽ�, ���� ��
	SPcDataInven		thePcInven;
	
	CLogo2DEffect		m_Logo2DEffect;		

private:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9	theD3DDevice;
#else
	LPDIRECT3DDEVICE8	theD3DDevice;
#endif // DIRECT_VERSION_9_MJH

	N_RESOLUTION		theResolution;//�ػ�

	//-------------------------------------------------------------------------
	SPanel				theBGround;
	SPanel				theFGround;

	NCursorType			theCursorType;
	int					theCursorAniIdx;
	SCursor*			theCursor;
	SDlgBorder*			theBorder;

	SEventMessage		theEMsg;
	//-------------------------------------------------------------------------
	
	
	WNDID				DurationWnd;	//���� ������
	WNDID				ModalWnd;		//��� ������
	WNDID				ActiveWnd;		//Ȱ�� ������
	WNDID				FocusWnd;		//���� ���콺 ��ġ�� ��Ŀ�� ������ 
	WNDID				FocusPwnd;		//IsFocus( int Mx, int My ) ��....����...Event .... Focus Parents Wnd 
										//��Ŀ�� üũ�� �ܼ���..�ֻ��� �����츸 �˻��ϱ� ����....
	
	bool				isDragging;

	bool				isShowInfo;

	NEnterFlag			theEnterFlag;
	WNDID				theEnterWndID;
	
	SMessageStr			theMessage[256];
	int					theNumMsg;
	CFontg*				theFontg1;
	CFontg*				theFontg2;	
	CFontg*				theFontg3;	


	CHelpTip*			theHelpTip;
	CHelpItem*			theItemTip;

	int					theNumDlg;
	CDlgBalloon*		theDlgBalloon[MAX_DLGBALLOON];	

	//-------------------------------------------------------------------------
	WNDID FindFocusWindow( int Mx, int My, bool IsChildCheck );
	void MakeEventMessage( SKeys* Keys, SMouse* Mouse, WNDID FocusWnd );
	void KeyEventProc();
	void UserKeyProc();		//������ ����ϴ�Ű 
	void CtrlUserKeyProc(bool& IsWnd);	//��Ʈ�� + X êâ�� ��Ĩ 
	void TestKeyProc(bool& IsWnd);		//�����ڰ� ����ϴ�Ű 

	//-------------------------------------------------------------------------

	void SortDlgBalloon();
	
	void GetTimeFlag();
	
	//-------------------------------------------------------------------------
	void DrawPoint();
	void DrawPcItem();
	void DrawWindows(); 
	void DrawToolTip();
	void DrawMessage();
	void DrawInfo();	
	void DrawBackGround();
	void DrawForeGround();	
	void DrawSystemMsg();
	void DrawAutoHelpTip();

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	void ShowInfo( bool ShowFlag );
	void ShowCursor( bool ShowFlag );
	void ShowBackGround( bool ActionFlag );
	void ShowForeGround( bool ActionFlag );
	//-------------------------------------------------------------------------
public:
	void DrawCursor();

	void DrawCharacterID();
	//by simwoosung
    void DrawCharacterDetailInfoBillId(int index);   //������ ���� - ĳ���� ����   
	void DrawCharacterDetailInfoUIId(int index);     //RHW-UI ���� - ĳ���� ����
	void DrawDlgBalloon(); //��ǳ�� 
	void DrawGameUI();
	void UpdateMaxGage(WORD aSkill_Code, bool IsGet);

protected:
	void UpdateGetMaxGage(int nGageStep, int nGageIndex);
	void UpdatePutMaxGage(int nGageStep, int nGageIndex);

public:

	long	theItemDalayTime[c_Max_ItemDelayInfo];					// ������ ��� ������ üũ 50����
	float   realItemDelayTime[c_Max_ItemDelayInfo];					// ���� ������ Ÿ��
	float   cumItemDelayTime[c_Max_ItemDelayInfo];					// ���� ������ Ÿ��
	bool	enableDelayTime[c_Max_ItemDelayInfo];					// ������ Ÿ�� ��밡�ɿ���
	bool	useCheckDelayTime[c_Max_ItemDelayInfo];					// �׷� ������ ���üũ����
	float   useCheckCumDelayTime[c_Max_ItemDelayInfo];				// �׷� ������ ���üũ����Ÿ��

	void InitDlgBalloon();

	//by simwoosung
	WNDID			GetFocusWnd()		{ return FocusWnd; }
	NCursorType		GetCursorType()		{ return theCursorType; }
	SCursor *		GetCursor()		    { return theCursor; }
	CHelpTip*		GetHelpTip()        { return theHelpTip; }
	SEventMessage*	GetEMsg()			{ return &theEMsg; }
	CFontg*         GetFontg()			{ return theFontg2; }
	void			GetUserKeyProc() {UserKeyProc();}
	//by dongs 
	BOOL		GetIsKeepUpFindSKill(const int Code);


	void			LogOff();
	void			SetState();
	void			ReleaseState();

	///---------------------------------------------------------------------------	
	///-- Japan IME �۾� �� �߰�.
	///---------------------------------------------------------------------------
public:
	void		DrawIMECovert();	
	void		GetIMEAttrPos( HIMC ahIMC );
	int			GetDrawStrWidth( CFontg* pFontg, TCHAR* pStr, long aNumStr );
	int			GetDrawStrHeight( CFontg* pFontg, TCHAR* pStr );
	
	int			GetStrWidthMax(CFontg* pFontg , void *pStr , int &RealSize,int MaxSize);
	

	bool Get_Cdi_visible() const { return mb_candidate; };
	int Get_Cdi_start_idx() const { return ml_candi_start_idx; };
	int Get_Cdi_sel_idx() const { return ml_candi_sel_idx; };	
	int Get_Cdi_count() const { return ml_candi_count; };
	const char * const Get_Cid_Str( const int al_idx ) const { return mac_candi_str[al_idx]; }; 
	const char * const Get_Cid_cmpStr( const int al_idx ) const { return mac_comp_str; }; 
	int Get_Language(){ return ml_language; }; 
	
	void Set_Cdi_Str( const int al_index, char * apc_str ); 
	void Set_Cdi_cmpStr( char * apc_str );
	void Set_Cdi_visible( const bool ab_flag ) { mb_candidate = ab_flag; };
	void Set_Cdi_Values( const int al_count, const int al_st_idx, const int al_sel_idx );
	void Render_Candidate();
	void Set_Language( int al_language ){ ml_language = al_language;};
	void Set_Cdi_Pos( const int x, const int y ) { ml_candi_x = x, ml_candi_y = y; };
	void Complete_Cdi_List();		///-- Candidate List ���� �Ϸ� Render �غ�

	void ReSetTIDImage();
	
	
	//NewIME
	std::wstring m_reading;
	DWORD		m_dwId[2];
	HKL			m_hkl;
	HINSTANCE	m_hDllIme;
	LANGID		m_langId;
	WORD		m_codePage;
	bool		m_bUnicodeIme;
	bool		m_bVerticalReading;
	bool		m_bCheckChinaCHS;
	int	GetCharsetFromLang(LANGID langid);
	int GetCodePageFromLang( LANGID langid );
	int GetCodePageFromCharset(int charset);
	bool GetCheckIME_Unicode();
	bool GetReadingWindowOrientation();
	void GetImeID();
	void SetupImeApi(HWND hWnd);
	void GetPrivateReadingString(HWND hEdit);
	void LanguageChange(HWND hWnd , WPARAM wParam, LPARAM lParam);
	UINT (WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
	BOOL (WINAPI * _ShowReadingWindow)( HIMC, BOOL );
	bool bVerticalReading;

	void SetReadingStr(RTCHAR *Str);
	void SetReadingVisible(bool bFlag){ bVerticalReading = bFlag;};
	void Render_Reading();
	bool ReadingProcess();
	WCHAR StrTextReading[128];
	WCHAR StrTextReadingTemp[4][20];


	void SetStringReset(){m_reading.resize(0);};

private:	
	///-- !!!!JIME	
	///-- RockClient.h ���� �ִ�. ������ rockclient.cpp
	int				ml_language;	///-- 0 : Korean / 1 : Japan / 2 : Reserved / 3 : Reserved
	char mac_comp_str[1024];						///-- �������� String
	char mac_candi_str[10][100];					///-- Candidate List	//����,�Ϻ� ������������ 9 �߱� 10
	bool mb_candidate;								///-- Candidate â ���� �Ǵ�.
	int	 ml_candi_start_idx;						///-- ���� ������ Page �� ù idx
	int	 ml_candi_sel_idx;							///-- ���� ���õ� idx	
	int	 ml_candi_count;							///-- Candidate Character Count
	int	 ml_candi_x, ml_candi_y;					///-- Candidate Coordinate
	
	int  ml_width_max;								///-- List �� �ʺ�
	int	 ml_page_y;									///-- List Page ��ȣ
	int  ml_list_y;									///-- List �� ������ǥ 

	CFontg*			mc_font_candi;	
	CFontg*			mc_font_candisel;	

	int				m_convertedPos[2];
	
	long			m_lEdPrevtime;
};

extern CRui	*nRui;
//-----------------------------------------------------------------------------
#endif	__RUI_H__