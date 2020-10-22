#ifndef __DEFINE_H__
#define __DEFINE_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9.h>
#else
#include <d3dx8.h>
#endif // DIRECT_VERSION_9_MJH

#include "DefineWID.h"
#include "DefineTID.h"

//#include "..\\..\\Pc.h"
//#include "..\\..\\bravolibs\\map\\field.h"


#define _PI				3.1415926535897932384626f 
#define _PI_DIV_180		0.0174532925199432957692f  // PI/180
#define _180_DIV_PI		57.295779513082320876798f  // 180/PI

#define DEG2RAD(a)	( _PI_DIV_180*(a) )
#define RAD2DEG(a)	( _180_DIV_PI*(a) )

//-----------------------------------------------------------------------------
// Disc : RUI Ÿ�� ����
//-----------------------------------------------------------------------------
typedef UINT			WNDID;
typedef UINT			TEXID;
typedef UINT			HLPID;

typedef UINT			FILEID;
typedef UINT			IEVENT;
typedef UINT			EPARAM;
typedef int				IRESULT;

typedef unsigned short	FONTID;


typedef unsigned char	ICOLOR;
typedef unsigned char	ICOLOR3[3];
typedef unsigned char	ICOLOR4[4];

typedef int				ISIZE[4];
typedef int				IPOS2[2];
typedef int				IPOS3[3];


/*
#define MB_NONE		0x0000
#define MB_DOWN		0x0001
#define MB_PRESS	0x0002
#define MB_UP		0x0003

struct stInputState
{
	bool Keys[256];//Ű����
	
	int Lmb;
	int Rmb;
	int Mx;
	int My;

	stInputState()
	{
		memset( &Keys, 0, sizeof(bool)*256 );

		Lmb = MB_NONE;
		Rmb = MB_NONE;
		Mx = 0;
		My = 0;
	}
};
//*/

//-----------------------------------------------------------------------------
// Disc : KeyEvent
//-----------------------------------------------------------------------------

#define KA_NONE			0x0000
#define KA_DOWN			0x0001
#define KA_PRESS		0x0002
#define KA_UP			0x0003

#define KE_NONE			0x0000
#define KE_EXIT			0x0001
#define KE_CHAT			0x0002
#define KE_FONT_CHANGE	0x0003
#define KE_DISPLAY_INFO 0x0004


//-----------------------------------------------------------------------------
// Disc : MouseEvent
//-----------------------------------------------------------------------------

#define ME_NONE			0x0000
//#define ME_FOCUS		0x10000000
#define ME_MOVE			0x0100
//#define ME_ACT_STOP	0x00000000

#define ME_LBNONE		0x0000
#define ME_LBDOWN		0x0001
#define ME_LBPRESS		0x0002
#define ME_LBUP			0x0004
#define ME_LBDBLCLICK	0x0008

#define ME_RBNONE		0x0000
#define ME_RBDOWN		0x0010
#define ME_RBPRESS		0x0020
#define ME_RBUP			0x0040
#define ME_RBDBLCLICK	0x0080


#define MA_NONE			0x0000
#define MA_FOCUS		0x0001

#define MA_LDOWN		0x0002
#define MA_LPRESS		0x0003
#define MA_LUP			0x0004

#define MA_RDOWN		0x0005
#define MA_RPRESS		0x0006
#define MA_RUP			0x0007

#define MA_LDRAG		0x0008
#define MA_LDROP		0x0009

#define MA_RDRAG		0x000b
#define MA_RDROP		0x000c

#define MA_LDBLCLICK	0x000e
#define MA_RDBLCLICK	0x000f


enum NMouseState
{
	n_msNone,
	n_msStop,
	n_msMove,
	n_msWheel,
};


enum NMouseAction
{

};


enum NButtonState
{
	n_bsNormal,
	n_bsFocus,
	n_bsSelect,
	n_bsHold
};


enum NButtonAction
{
	n_baNone,
	n_baDown,
	n_baPress,
	n_baUp,
	n_baDblClick
};


enum NAlignText
{
	n_atLeft,
	n_atRight,
	n_atTop,
	n_atBottom,
	n_atCenter,
};

enum N_PROGRESS_DIRECTION
{
	n_PD_LEFT,
	n_PD_RIGHT,
	n_PD_UP,
	n_PD_DOWN,
};

enum NColorIngredients
{
	A,
	R,
	G,
	B
};


enum NCheckMark
{
	n_cmChecked,
	n_cmUnChecked
};


enum NMessageColor
{
	n_mcNormal	= D3DCOLOR_ARGB( 255, 210, 190, 140 ),
	n_mcWarning	= D3DCOLOR_ARGB( 255, 240, 140, 60 ),
	n_mcError	= D3DCOLOR_ARGB( 255, 250, 60, 40 ),
	n_mcInfo	= D3DCOLOR_ARGB( 255, 160, 200, 220 ),
	n_mcInfo2	= D3DCOLOR_ARGB( 255, 200, 220, 255 ),
	n_mcInfo3	= D3DCOLOR_ARGB( 255, 160, 230, 250 )
};

enum NFontType
{
	n_ftGulimChe = 0,
	n_ftDotum,
};

enum NFontResv
{
	n_fRsvFontTitle = 0,
	n_fRsvFontWndButton,
	n_fRsvFontWndStatic,
	n_fRsvFontWndStatic2,
	n_fRsvFontWndStatic3,
	n_fRsvFontNPCChat,
	n_fRsvFontNPCChat2,
	n_fRsvNotice,
	n_fRsvMessageBox,
	n_fRsvImageNum
};
enum NFontCharset
{
	n_fcAnsi		= ANSI_CHARSET,
	n_fcDefault		= DEFAULT_CHARSET,
	n_fcSymbol		= SYMBOL_CHARSET,
	n_fcHangul		= HANGUL_CHARSET,			// define 129 ( HANGEUL_CHARSET, HANGUL_CHARSET )
	n_fcJapan		= SHIFTJIS_CHARSET,			///-- Japan
	n_fcGB2312		= GB2312_CHARSET,			///-- China
	n_fcBig5		= CHINESEBIG5_CHARSET,		///-- Taiwan
	n_fcEct
};


enum NFontWeights// Font Weights
{
	n_fwDontcare	= FW_DONTCARE,
	n_fwThin		= FW_THIN,
	n_fwExtralight	= FW_EXTRALIGHT,
	n_fwLight		= FW_LIGHT,
	n_fwNormal		= FW_NORMAL,
	n_fwMedium		= FW_MEDIUM,
	n_fwSemibold	= FW_SEMIBOLD,
	n_fwBold		= FW_BOLD,
	n_fwExtrabold	= FW_EXTRABOLD,
	n_fwHeavy		= FW_HEAVY,
};


enum NFontDecoration
{	
	n_fdNormal,		//0
	n_fdShadow,		//1
	n_fdOutline,	//2	
	n_fdUserColor
};


enum NWindowType
{
	n_wtWnd,
	n_wtFrameWnd,
	n_wtDialog,
	
	n_wtButton,
	n_wtButton9,
	n_wtImageBox,
	n_wtImageBox9,
	n_wtImageAni,
	n_wtNoImageBox,
	n_wtEblemImageBox,
	n_wtTextBox,
	n_wtEditBox,
	n_wtProgressBar,
	n_wtCheckButton,
	n_wtRadioButton,
	n_wtSlideBar,
	n_wtScrollBar,
	n_wtListBox,
	n_wtComboBox,
	n_wtSpin,
	n_wtStateButton,

	n_wtUpGauge,
	n_wtGaugeBar,
	n_wtMiniMap,
	n_wtChatOutput,
	n_wtSysMsgOutput,
	n_wtItemBox,
	n_wtQuickSlot,
	n_wtSlot,

	n_wtPropertySheet,
	n_wtListView,
	n_wtTreeView,

	n_wtChaImgButton,	
	n_wtFlashWnd,
	n_wtItemTransButton,
	n_wtComEffImgBox,
	n_wtComEffProgBar,
	n_wtComboStepBar,	
	n_wtFlashFrameWnd,
	
    n_wtLocalEntireMap,
	n_wtMLineEditBox,
	n_wtLimitLenTextBox,
	n_wtFrameTextBox,

	n_wtChatOutList,
	n_wtSystemOutList,

	n_wtChatEditBox,
	n_wtRITextBox,
	n_wtCharImgBox,
	n_wtArrowLineList,
};


enum NDepthOrder
{
	n_zoNone,
	n_zoTop,
	n_zoNormal,
	n_zoBottom
};


enum NItemClass
{
	n_itExpendableItem, 
	n_itUnexpendableItem, 
	n_itEquipmentItem,
};

enum NChatState
{
	n_NormarChat,		 //�Ϲ�
	n_WhisperChat,		 //�ͼӸ�
	n_FriendChat,		 //ģ��
	n_PartyChat,		 //��Ƽ
	n_GuildChat,		 //���
	
	n_WholeServerChat,	 //��ü ����ä��
	n_WholeChannelChat,	 //��ü ä��ä��
	n_WholeAreaChat,	//��ü ����ä��
	n_AnnounChat,		 //����
	n_HelpChat,			 //����
};


enum NPetStateInfo
{
	n_Pet_Die,			//�״� 
	n_Pet_Live,		//���  
	n_Pet_Custody,	//���� 
};



enum NEventMessage
{
	//None Event
	n_emNone,

	//Desktop Event
	n_emTextOutput,
	n_emMessageBox,
	n_emChangeCursor,
	n_emResetFont,
	n_emBackGround,
	n_emForeGround,

	//Wnd
	n_emSetVisible,
	n_emSetClientImage,
	n_emSetClientColor,
	n_emSetGroupWnd,
	n_emSetSelectedWnd,

	//TextBox
	n_emSetText,
	n_emGetText,
	n_emSetTextAlpha,
	n_emSetDecorationFont,
	n_emMultiLine,
	n_emTextIntegral,
	n_emGetTextLine,

	//Button
	n_emSetHold,

	//CheckBtn
	n_emSetChecked,
	n_emSetUnChecked,
	n_emGetCheckMark,

	//ProgressBar
	n_emInitProgress,
	n_emSetPercent,
	n_emSetPercentAccum,
	n_emGetPercentage,

	//SlideBar
	n_emSetSldScope,
	n_emSetSldValue,
	n_emGetSldValue,
	n_emResetSldExtentSync,

	//ListBox, ComboBox
	n_emAddItem,
	n_emGetSelectedItem,

	//EditBox
	n_emSetFocus,
	n_emClearText,
	n_emEditDisable,
	
	//FrameWnd
	n_emCloseFrameWnd,
	n_emEditReturnAction,
	n_emEditSpaceAction,
	n_emEditTabAction,
	n_emEditImmConvStatus,
	n_emSetCaption,

	//Spin
	n_emUpBtnDisable,
	n_emDownBtnDisable,
	n_emUpBtnEnable,
	n_emDownBtnEnable,
};



// Disc : NotifyMessage
#define NM_NONE					0


#define NM_MSG_OK				1
#define NM_MSG_CANCEL			2
#define NM_MSG_TIMEOVER			3


#define NM_FWND_CLICK			10
#define NM_FWND_RCLICK			11

#define NM_BTN_FOCUS			100
#define NM_BTN_DOWN				101
#define NM_BTN_PRESS			102
#define NM_BTN_UP				103
#define NM_BTN_CLICK			104
#define NM_BTN_RCLICK			105
#define NM_BTN_HOLD				106
#define NM_BTN_DCLICK			107


#define NM_EDT_RETURN_ACTION	200
#define NM_EDT_TAB_ACTION		201
#define NM_EDT_IMMCONVSTATUS	202
#define NM_EDT_SPACE_ACTION		203

#define NM_LB_SELECTED			300
#define NM_CB_TEXT_EXCHANGE		400

#define NM_SLD_MOVEBAR			500
#define NM_SLD_CLICKMOVEBAR		501
#define NM_SLD_BTNMOVERBAR		502

#define NM_SPIN_UP				601
#define NM_SPIN_DOWN			602

#define NM_IB_DBLCLICK			700
#define NM_IB_ITEMMOVE_READY	701
#define NM_IB_ITEMMOVE_CANCEL	702
#define NM_IB_ITEMMOVE_ON		703


#define NM_SLOT_FOCUS			708
#define NM_SLOT_PRESS			709
#define NM_SLOT_CLICK			710
#define NM_SLOT_DBCLICK			711
#define NM_SLOT_DRAG			712
#define NM_SLOT_DROP			713




struct SPoint2
{
	int x;
	int y;
	
	SPoint2()
	{
		x = y = 0.0f;
	}
	
	SPoint2(int nx, int ny)
	{
		x = nx;
		y = ny;
	}

	void operator=(struct SPoint2 &temp)
	{
		x  = temp.x;
		y  = temp.y;		
	}
};


struct SPoint3
{
	float x;
	float y;
	float z;
	
	SPoint3()
	{
		x = y = z = 0.0f;
	}
};


struct SRect
{
	int x;
	int y;
	int w;
	int h;

	SRect()
	{
		x = y = w = h = 0;
	}

	SRect(int nx, int ny, int nw, int nh)
	{
		x = nx;
		y = ny;
		w = nw;
		h = nh;
	}

	void operator=(struct SRect &temp)
	{
		x  = temp.x;
		y  = temp.y;
		w  = temp.w;
		h  = temp.h;		
	}
};

struct SFRect
{
	float x;
	float y;
	float w;
	float h;

	SFRect()
	{
		x = y = w = h = 0;
	}

	SFRect(float nx, float ny, float nw, float nh)
	{
		x = nx;
		y = ny;
		w = nw;
		h = nh;
	}

	void operator=(struct SFRect &temp)
	{
		x  = temp.x;
		y  = temp.y;
		w  = temp.w;
		h  = temp.h;		
	}
};


struct SStringInfo
{
	int Width;
	int Height;
	int NumStr;
	int nCLen;
	int Line;
};


struct SVertex
{
	float x, y, z, w;
	DWORD diffuse;
	float tu, tv;

	SVertex()
	{
		x = y = z = 0.0f;
		w = 1.0f;
		diffuse = 0.0f;
		tu = tv = 0.0f;
	}
};


struct STexture
{
	int ImgId;

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 pTexture;
#else
	LPDIRECT3DTEXTURE8 pTexture;
#endif // DIRECT_VERSION_9_MJH
	
	SVertex vt[4];

	int x;
	int y;
	int w;
	int h;
	
	STexture()
	{
		pTexture = NULL;
	}
};


struct SImage
{
	TCHAR	FileName[80];

	int		Width;
	int		Height;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 pTexture;
#else
	LPDIRECT3DTEXTURE8 pTexture;
#endif // DIRECT_VERSION_9_MJH
	long		m_lPreTime;			// ���������ýð�
	
	SImage()
	{
		pTexture = NULL;
	}
};


struct SFont
{
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 pTexture;
#else
	LPDIRECT3DTEXTURE8 pTexture;
#endif // DIRECT_VERSION_9_MJH
	SVertex vt[4];
	long		m_lPreTime;			// ���������ýð�
	
	int x;
	int y;
	int w;
	int h;

	//by simwoosung
	int Riw;				//���� �����ڰ� �ʺ�
	int Rih;				//���� �����ڰ� ����
	
	int nTexWidth;
	int nTexHeight;

	D3DCOLOR color;
};


struct SKeys
{
	NButtonAction	Action;
};


struct SMouse
{
	NMouseState		State;

	IEVENT			Event;
	IEVENT			Action;

	short			WheelValue;//

	POINT			Aposition;// ���� ��ġ Absolut Position
	POINT			Rposition;// ��� ��ġ Relative Positon

	NButtonAction	LButton;
	NButtonAction	RButton;

	NButtonAction	uiLButton;
	NButtonAction	uiRButton;
};


struct STimeFlag
{
	int		FPS;
	double	ElapseTime;
	
    float	Four_1;
    float	Three_1;
    float	Two_1;
    float	One_1;
    float	One_2;
    float	One_4;
	float	One_7;
	float	One_10;
    float	One_20;
    float	One_30;
    float	One_40;
    float	One_50;
    float	One_60;

    STimeFlag()
    {
        Four_1	=	Three_1	=	Two_1	=	0.0f;
        One_1	=	One_2	=	One_4	=	One_7	= 0.0f;
		One_10	=	One_20	=   One_30	=	0.0f;
		One_40	=	One_50	=	One_60	=	0.0f;
    }
};


struct SDesktop
{
	HINSTANCE	hInst;
	HWND		hWnd;
	HWND		hCurWnd;
	WNDID		FocusEditWnd;
	
	int			Width;
	int			Height;
	int			Bits;
	
	STimeFlag	Tflag;			//Time Flag
	WNDID		DraggingWnd;	//
	WNDID		CurFocusWnd;
	WNDID		SelectedWnd;

	TEXID		ItemImg;

	SDesktop()
	{
		FocusEditWnd = WID_None;
	}
};


struct SEventMessage
{
	WNDID	FocusWnd;
		
	IEVENT	Notify;//���� �޼���
	
	SKeys	Keys[256];	
	SMouse	Mouse;

	EPARAM	fParam;	//f - first parameter
	EPARAM	sParam;	//s - second parameter
};


struct SValue
{
	int value;
	int min;
	int max;

	SValue()
	{
		value	= 0;
		min		= 0;
		max		= 0;
	}
};

//#define MAX_CHARACTER	(MAX_USER_INTABLE + MAX_NPC_INTABLE)
#define MAX_CHARACTER	(128 + 256 + 1)



enum NCHARTYPE
{
	enNONE,
	enPLAYER,	//�÷��̾� �ڽŸ� 
	enNPC,			//���� �÷��̾� .. 
	enMONSTER,
	enItem
};


enum NBalloonType
{
	n_BT_NORMAL,
	n_BT_MYSHOP,	
};


enum NCursorType
{
	n_CT_NORMAL,	//���			: �⺻Ŀ��
	n_CT_FOOT,		//��			: �̵� ������ �ʵ� ���� Ŀ���� ��ġ�� ��
	n_CT_FOOT_X,	//��X			: �̵� �Ұ����� �ʵ� ���� Ŀ���� ��ġ�� ��
	n_CT_PORTAL,	//��Ż			: ��Ż�� ��ġ�� ���� Ŀ���� ��ġ�� ��
	n_CT_HAND,		//��			: �ٴ��̳�, �κ��丮�� ������ ���� Ŀ���� ��ġ�� ��
	n_CT_MOUTH,		//��			: ��ȭ�� ������ NPC ���� Ŀ���� ��ġ�� ��
	n_CT_TXTREAD,	//���б�		: ǥ�����̳� �۾��� �ִ� ������Ʈ ���� Ŀ���� ��ġ�� ��
	n_CT_TXTREAD_X,	//���б�X		: �ʹ� �־ ���� �� ���� ��ġ�� �ִ� ǥ�����̳� �۾��� Ŀ���� ��ġ�� ��
	n_CT_SWORD,		//Į			: Į�� ����ϰ�, ���ݰ����� ��� ���� Ŀ���� ��ġ�� ��
	n_CT_ARROW,		//ȭ��			: Ȱ�� ����ϰ�, ���ݰ����� ��� ���� Ŀ���� ��ġ�� �������Ÿ� ���� ��쿡�� ������ ǥ��
	n_CT_ARROW_X,	//ȭ��X			: Ȱ�� ����ϰ�, ���ݰ����� ��� ���� Ŀ���� ��ġ�� �������Ÿ� ���� ��쿡�� ������ ǥ��
	n_CT_MAGIC,		//����			: ������ �����ϰ�, ���ݰ����� ��� ���� Ŀ���� ��ġ�� �������Ÿ� ���� ��쿡�� ������ ǥ��
	n_CT_MAGIC_X,	//����X			: ������ �����ϰ�, ���ݰ����� ��� ���� Ŀ���� ��ġ�� �������Ÿ� ���� ��쿡�� ������ ǥ��
	n_CT_SKILL,		//��ų			: ����� �ʿ��� ��ų�� ��� ��, �� ����� ������ ��
	n_CT_ITEMDROP,	//�����۵��	: �κ��丮 �ȿ��� �������� ����, �κ��丮 �ۿ� Ŀ���� ��ġ�� ��
	n_CT_SHOP,		//���λ��� Ŀ��	: ���λ���
	n_CT_NONE,		//����			: ��Ŀ��		
};


struct SPcInfo
{
	bool			existence;		//���� ����

	UINT			distance;		//ī�޶���� �Ÿ�
	BYTE			direction;		//����(8����)
	int				angle;
	UINT			char_distance;	

	int				npc_code;		//npc code -> Ŀ�� ���¸� �ٲٱ� ���� �߰�
	long			id;				//unique
	NCHARTYPE		ctype;			//ĳ���� Ÿ��
	int				type_idx;		//Ÿ�Ժ� �ε���

	TCHAR			name[32];		//���̵�

	int				race;			//NPC�� ���������� ������..����....-1:��, -2:���̺������, -3:��
	NBalloonType	btype;			//��ǳ�� Ÿ��
	D3DCOLOR		color;
	TCHAR			message[128];	//��ȭ �޼���( ��ǳ�� )

	SPoint3			wpos;			//������� ĳ���� ��ġ
	SPoint2			spos;			//ȭ���(Ŭ���̾�Ʈ ����)�� ��ġ

	D3DXVECTOR3		min;
	D3DXVECTOR3		max;

	void		   *pCharacter;		// ĳ���� ������ by wxywxy
	TCHAR           partyname[128];  //��Ƽ�� by simwoosung
	
	char			FrontierName[RP_MAX_FRONTIER_NAME];
	DWORD			FrontierID;		//����Ƽ�� ���̵�
	BYTE			Status;
	char			FrontierNick[RP_MAX_FRONTIER_NICK];
	
	///-- �߱� ī���۾���
	BOOL			IsChaosChar;

	SPcInfo()
	{
		existence	= false;
		distance	= 0;

		name[0]		= '\0';
		message[0]	= '\0';

		memset( &wpos, 0, sizeof( SPoint3 ) );
		memset( &spos, 0, sizeof( SPoint2 ) );

		direction	= 0xff;
		angle	= 0xff;
		race	= 0xff;

		pCharacter = NULL;
		//��Ƽ��
		partyname[0] = '\0';

		FrontierID = 0;
		FrontierName[0] = '\0';
		FrontierNick[0] = '\0';

		IsChaosChar = FALSE;
	}
};


struct SCursor
{
	TEXID		img[6];
	SPoint2		pos;
	D3DCOLOR	color;

	SCursor()
	{
		img[0] = 0;
		img[1] = 0;

		pos.x = 0;
		pos.y = 0;

		color = D3DCOLOR_XRGB( 255, 255, 255 );
	}
};


struct IColorText
{
	int Red;
	int Green;
	int Blue;
	
	IColorText()
	{
		Red = Green = Blue = 0 ; 
	}
};


struct EtcInfo
{
	int DevMode;
	IColorText ChatRGB;
	IColorText TitleChatRGB;
	int	NpcStringIndex ; 
	
	EtcInfo()
	{
		Clear();
	}
	
	void Clear()
	{
		NpcStringIndex = 0 ; 
		DevMode = 0; 
	}
};


struct SDlgBorder
{
	int		width;

	TEXID	north;
	TEXID	north_east;
	TEXID	east;
	TEXID	south_east;
	TEXID	south;
	TEXID	south_west;
	TEXID	west;
	TEXID	north_west;
	TEXID	tail;
	int		tail_w;
	int		tail_h;

	SDlgBorder()
	{
		width		= 0;

		north		= 0;
		north_east	= 0;
		east		= 0;
		south_east	= 0;
		south		= 0;
		south_west	= 0;
		west		= 0;
		north_west	= 0;
		tail		= 0;
		tail_w		= 0;
		tail_h		= 0;
	}
};

struct SPcDataParam
{
	char			PCName[RP_MAX_PC_NAME];
	char			AccountID[RP_MAX_ACCOUNT_ID];

	WORD			Citizen;	// RAIN, MILLENA
	WORD			Gender;		// MALE, FEMALE
	WORD			Face;
	WORD			Hair;
	BYTE			MainClass;	// �������� 30��
	BYTE			SubClass;	// �������� 15��
#ifdef EXP_INT64
	__int64			Exp;
#else 
	DWORD			Exp;		//����ġ
#endif 
	DWORD			Fame;
	BYTE			Level;		//���� -- 1~255 �Ŀ� Ȯ���� ����Ͽ� �� �����ְ� Ȯ��
	WORD			Map;
	DWORD			Cell;
	
	DWORD			SubExp;		///-- �������� ����ġ
	BYTE			SubLevel;	///-- �������� ����

	SRpdSTAT1		Stat1;
	SRpdSTAT2		Stat2;

	DWORD			MaxHP;		// �⺻ �� + ���� ������ + Item + Skill
	DWORD			MaxSP;
	DWORD			MaxMP;

	int			Power;		// �Ŀ�
	int			Vital;		// ü�� 
	int			Sympathy;	// ����
	int			Intel;		// ���� 
	int			Stamina;	// ������ 
	int			Dex;		// ��ø 
	int			Charisma;	// ī������, �ŷ�
	char			Luck;		// ���  -10 ~ 10
	WORD			Point;		// �ɷ�ġ �ø��� ���ʽ� ����Ʈ 
	WORD			SkillPoint; // ��ų������ ���Ǵ� ����Ʈ
	WORD			TotalSkillPoint;	// ������ų ����Ʈ	

	int				WinFCount;				// ���� 
	int 			TotalFCount;			// �� ���� 
	
	DWORD			Money;
	WORD			Weight;
	
    //by simwoosung
	char            PartyName[256];
	BYTE			GageType;
	
	//By simwoosung - Max Gage�� �������� �����
	BYTE			theMaxGage1;
	BYTE			theMaxGage2;

	DWORD			Contribution;

	DWORD			thePKPoint;		//-- PK����Ʈ	
	
// ������ 2009/12/29
// �������� ���� ���� ��ġ�� �����ϱ� ���� ���� �߰�
#ifdef FAMOUSE_FIGURES_KJK
	int				TotalFame;	// ���� ��ġ
#endif

	SPcDataParam()
	{
       GageType = 0;
	   SAFE_STR_CPY(PartyName,_RT(""), 256);

	   theMaxGage1 = 0;
	   theMaxGage2 = 0;
	   
	   thePKPoint = 0;
	}

	int GetMaxGage()
	{
		int max = 0;
		if(max < theMaxGage1)
		{
		  max = theMaxGage1;
		}
		if(max < theMaxGage2)
		{
		  max = theMaxGage2;
		}
		return max;
	}	
};


enum N_ITEMTYPE
{
	n_ITYPE_NONE,				// 
								// ������ �з� ��( Class )
	//N_ITYPE_WEAR_WEAPON,		// 1. ����� (��, ���, Ȱ, ������, ����, ������)		
	//N_ITYPE_WEAR_ARMOR,			// 2. ��, ���ʷ� (��.����, �尩, �Ź�, ����, ���, ����)
	//N_ITYPE_WEAR_ACCESSORY_SUB,	// 3. �������� (�Ͱ���, �����, ����, ��Ŭ����)
	//N_ITYPE_WEAR_ACCESSORY,		// 4. ��ű���
	n_ITYPE_WEAR,
	n_ITYPE_USE,				// 5. �Ҹ�ǰ( ��밡�� )
	n_ITYPE_UNUSE,				// 6. ��Ҹ�ǰ( ��� �Ұ��� )
	n_ITYPE_SPECIAL,			// 7. Ư�� ������
	n_ITYPE_QUEST,				//����Ʈ ������
	
	n_ITYPE_SKILL,
	n_ITYPE_PET,	
	n_ITYPE_PET_USE,			// �� ��� ������ 
	n_ITYPE_RIDE,				// �� ������		
	n_ITYPE_RIDE_WEAR,			// �� ��� ������	
	n_ITYPE_SKIN_WEAR,			// ��Ų ���
	
};


enum N_ITEMCLASS
{
/*	n_ICLASS_WEAR_HEAR,			//�Ӹ�	
	n_ICLASS_WEAR_WEAPON,		//����
	n_ICLASS_WEAR_COAT,			//����
	n_ICLASS_WEAR_PANTS,		//����
	n_ICLASS_WEAR_GLOVE,		//�尩
	n_ICLASS_WEAR_BOOTS,		//����
	n_ICLASS_WEAR_SUB,			//����
	n_ICLASS_WEAR_ACCESSORY,	//��ű�
	n_ICLASS_WEAR_SHIELD,		//����
	n_ICLASS_USE,				// 5. �Ҹ�ǰ( ��밡�� )
	n_ICLASS_UNUSE,				// 6. ��Ҹ�ǰ( ��� �Ұ��� )
	n_ICLASS_SPECIAL,			// 7. Ư�� ������
	n_ICLASS_FACE,				//��
	n_ICLASS_QUEST,				//����Ʈ
	n_ICLASS_ELDASTONE,			// ���ٽ���

enum NItemClass
{
	n_Class_Helmet = 0,			// �Ӹ�
	n_Class_Weapon,				// ����
	n_Class_CoatArmor,			// ������� - ����
	n_Class_PantsArmor,			// ������� - ����
	n_Class_Gloves,				// ������� - �尩
	n_Class_Shoes,				// ������� - ����
	n_Class_Serve,				// �������(����, ����, ��)
	n_Class_Accessory,			// ��ű�
	n_Class_Shield,				// ����
	n_Class_Use,				// �Ҹ�ǰ
	n_Class_Useless,			// ��Ҹ�ǰ
	n_Class_Special,			// Ư�������� 
	n_Class_Face,				// �� 
	n_Class_Quest,				// ����Ʈ ������ 
	n_Class_Eldastone,			// ���ٽ���
	n_Class_FrontierPet,		// �ż��о�� 
};
*/
	n_ICLASS_NONE = 255,
};


struct SPcItem
{
	N_ITEMTYPE	IType;			//��ų, ��������, �Ҹ�ǰ������, ��Ҹ�ǰ������, Ư��������
	BYTE		Class;			//N_ITEMCLASS

	DWORD		ItemUID;		//��ų�� ���...UID����
	WORD		Code;
	WORD		Amount;			//������ ����(��ų, ���������ϰ�� 1�� ���� )
	TEXID		Tid;			//������ �̹���
	TEXID		TidEx;			//���� �̹���
	
	SRpdWearItemBase	WearTable;//���������ϰ�� ��� ( �������� �޾ƿ� ����ü�� ������ �����ؾ� �Ѵ�. )
	SItemBaseInfo*		ItemTable;
	SSkillBaseInfo*		SkillTable;
	
	//������ ǥ��....
	float			DelayTime;
	float			ProgressTime;
	float			CumulativeTime;
	int				STNum;
	bool			IsDelay;
	
	int	HolyLevel;	// �ż��� �������� ���������� �����ֱ� ������ �������� �ʿ� 
	int Nation;		//��ų���� �� ���� ������ �ʿ� �������� �ʿ� 

	SPcItem()
	{
		Clear();
	}

	void Clear()
	{	
		memset(&WearTable, 0, sizeof(SRpdWearItemBase));
		
		IType		= n_ITYPE_NONE;
		Class		= 255;//
		ItemUID		= 0;
		Code		= 0;
		Amount		= 0;
		Tid			= TID_None;
		TidEx		= TID_None;		
		
		ItemTable	= NULL;
		SkillTable	= NULL;
		
		DelayTime		= 0.0f;
		ProgressTime	= 0.0f;
		CumulativeTime	= 0.0f;
		STNum			= 0;
		IsDelay			= false;
		HolyLevel = 0 ; 
		Nation = 0 ; 
	}
	
	void operator=(const SPcItem &TempItem)
	{
		IType = TempItem.IType;
		Class = TempItem.Class;
		
		ItemUID = TempItem.ItemUID;
		Code    = TempItem.Code;
		Amount  = TempItem.Amount;
		Tid     = TempItem.Tid;
		TidEx   = TempItem.TidEx;
		
		memcpy( &WearTable , &TempItem.WearTable , sizeof( SRpdWearItemBase ) );
		ItemTable  = TempItem.ItemTable;
		SkillTable = TempItem.SkillTable;
		
		DelayTime      = TempItem.DelayTime;      
		ProgressTime   = TempItem.ProgressTime;
		CumulativeTime = TempItem.CumulativeTime; 
		STNum          = TempItem.STNum;          
		IsDelay        = TempItem.IsDelay;     
		HolyLevel		= TempItem.HolyLevel;
		Nation			= TempItem.Nation;
	}

#ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625

	bool IsItemCode60000Above() const	{ return WearTable.theEldaStone >= MIXELDA_ITEMCODE_MIN ? true : false; }

#endif// #ifdef HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625


#ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824

	bool IsItemCode12180_12188_Above() const	{ return ItemTable->theCode >= 12180 && ItemTable->theCode <= 12188 ? true : false ;}

#endif// #ifdef HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824
};


typedef struct SFrontier_MyInfo
{
	DWORD				FrontierID;
	BYTE				FrontierGrade;
	char				FrontierNick[ RP_MAX_FRONTIER_NICK ];
	DWORD				FrontierDues;
//	time_t		        FrontierLimitTime;

	SFrontier_MyInfo()
	{
		Clear();
	}

	void Clear()
	{	
		memset( &FrontierNick, 0, sizeof( char ) * RP_MAX_FRONTIER_NICK );
		
		FrontierID = 0;
		FrontierGrade = 0;
		FrontierDues = 0;
		//FrontierLimitTime = 0;
	}

} SFrontier_MyInfo;

struct SComBineFrontierInfo
{
	DWORD	dCombinedFID;
	char	strCombinedFName[RP_MAX_FRONTIER_NAME];
	char	strCombinedFMasterName[RP_MAX_PC_NAME];
	BYTE	aCombinedFMemberNum;
	DWORD	aCombinedFFame;
	
	SComBineFrontierInfo()
	{
		Clear();
	}

	void Clear()
	{	
		memset( this, 0, sizeof( SComBineFrontierInfo ) );
	}
};

struct STotemScore
{
	bool bShow;
	BYTE Millena;
	BYTE Rain;

	STotemScore()
	{
		bShow = 0 ; 
		Millena = 0 ; 
		Rain = 0 ; 
	}
};

struct SInDunData
{
	bool bShow;
	int iStartCount;
	DWORD dwOldTime; 
	DWORD dwStartTime; 

	SInDunData()
	{
		iStartCount = 0 ;
		dwOldTime = 0 ; 
		dwStartTime = 0 ; 
		bShow  = 0 ; 

	}
};


typedef struct SPetData
{	

	BYTE	PetType;						//������ �̳� ������̳� 
	BYTE	PetSlot;							//������â�� ���° �����̳� ??
	BYTE	Level;				
	BYTE	MaxInvenNum;				//�� ����� �κ� �⺻ 4�� 
	BYTE	ItemNum;						//���� �κ��� ����ִ� ������ ���� 
	
	WORD	NpcCode;						//NPC �ڵ� 
	WORD	HP;									//ü�� 
	WORD	MaxHP;
	WORD	CP;									//���ŷ� 
	WORD	MaxCP;
		
	DWORD	Exp;								//���� exp	
	DWORD	MaxExp;
//	bool		Live;								//��� �ֳ� ?? 
//	bool		Custody;						//���� ���̳� ? 
	NPetStateInfo	PetState;						//���� ���� n_Pet_Die 0 �׾���  n_Pet_Live 0  ��� n_Pet_Custody2 ����  ... 


	DWORD PetID;						//�ֺ� �����鿡�� ����� �Ѵ� .. ?
	DWORD PCID;

	SPcItem *InvenItem[RP_MAX_PET_ITEM];				//�κ� �ִ밳 ���� 20 

	SPcItem	KeepupItem[ RP_MAX_PET_USEDITEM ];		//�������� ������ 
	SPayEffect* PayEffect[RP_MAX_PET_USEDITEM];			//����Ʈ ������ 

	TCHAR	Name[64];

	time_t		DieTime;					//���� ��� �Ͻ� 
	time_t		CheckInTime;					//���� ���� �Ͻ� 
	time_t		CheckInPetShop;		// ������ �� �ð� 
	time_t		UpdateTime;			// �ֱ� ������Ʈ �ð� 
	
	DWORD		theLifeTime;		// �ð��� - �����ð�
	DWORD		theStartTime;		// �����ð��� ���۵� �ð�

	int		Ability;
	int		ProbAbility;
	bool	IsVisible;

	SPetData()
	{
		Clear();
	}

	~SPetData()
	{

		for(int  i = 0 ; i < RP_MAX_PET_ITEM ; ++i)
		{
			SAFE_DELETE(InvenItem[i]);
		}

		for(int j = 0 ; j < RP_MAX_PET_USEDITEM ; ++j)
		{
			SAFE_DELETE(PayEffect[j]);
		}
	}

	void Clear()
	{
	
		memset( InvenItem  , 0 , sizeof( SPcItem *   ) * RP_MAX_PET_ITEM  );
		memset(Name , 0 ,sizeof( char  ) * 64  )		;
		memset(PayEffect, 0 , sizeof(SPayEffect*	) * RP_MAX_PET_USEDITEM  );
		memset(KeepupItem, 0 , sizeof(SPayEffect	) * RP_MAX_PET_USEDITEM  );

		
		PetSlot =	0 ;		
	    NpcCode =	0;	
		Level =		 0 ;		
		Exp =		 0 ;			
		HP =			0 ;				
		CP =			 0 ;				
		DieTime =  0 ;
		CheckInPetShop = 0 ; 
		UpdateTime = 0 ;

		MaxInvenNum = 4 ;
		ItemNum =		0 ;		
	
		PetID =				 0 ;
		PCID = 0 ;
		
		Ability =	0  ;
		ProbAbility =  0  ;
		IsVisible  = 0 ; 
		//Live = 0 ; 
		PetState = n_Pet_Die;
		DieTime =0 ; 
		CheckInPetShop = 0 ;	
		CheckInTime = 0 ;		
		UpdateTime = 0 ;
	
		theLifeTime = 0;
		theStartTime = 0;
	}


}SPetData;

struct SQuestTime
{
	WORD	wdQuestCode;
	int		iCountTime;
	DWORD	dwStartTime;
	DWORD	dwEndTime;
	

	SQuestTime()
	{
		Clear();
	}

	void Clear()
	{
		wdQuestCode = 0; 
		iCountTime = 0 ; 
		dwStartTime = 0 ; 
		dwEndTime = 0 ; 
	}
};


struct SPcDataInven
{
	SPcItem*	WearItem[RP_MAX_EQUIP];
#ifdef PAYINVENITEM
	SPcItem*	InvenItem[RP_MAX_PAYINVENTORY+RP_MAX_INVENTORY];
#else
	SPcItem*	InvenItem[RP_MAX_INVENTORY];
#endif
	SPcItem*	InvenQuest[RP_MAX_QUEST_ITEM];
	SPcItem*	InvenHorse[RP_MAX_QUEST_ITEM];
	
	SPcItem*	SkillItem[RP_MAX_USER_SKILL];
	SPcItem*	QuickSlot[RP_MAX_QUICK_SLOT];

	WORD		KeepupSkill[RP_MAX_KEEPUP_SKILL];
	
	SQuestFlag*	QuestFlag[RP_MAX_QUEST_FLAG];
	SQuestCount* QuestCount[RP_MAX_QUEST_COUNT];
	SQuestTime*	QuestTime[RP_MAX_QUEST_COUNT];

	SPayEffect* PayEffect[RP_MAX_PAY_CONTENTS];	
	SPayTimeItem *PayTimeItemList[RP_MAX_TIMELIMIT_PAY_CONTENTS];
	

	// ��� ���� 
	SFrontier_MyInfo*	FrontierMyInfo;	
	SRpFrontierInfo*	FrontierInfo;						// ����Ƽ�� ����	
	char				FrontierCaller[RP_MAX_PC_NAME];		// �ż� ��ȯ�� 
	SRpFrontierPetInfo*	FrontierPetInfo;					// �ż� ���� 
	DWORD				FrontierPetID;						// �ż� ���̵�
	DWORD				FrontierPetMaxExp;					// �ż� �ִ� ����ġ

	
	///-- ��������Ƽ�� ����
	SComBineFrontierInfo * pComBineFrontierInfo;			// ��������Ƽ�� ����

	//dongs �� �߰� .. 
	SPetData* thePetData[4];
	
	// ���� ���ھ� 
	STotemScore					TotemScore;
	// ��ġ �δ��ð� 
	SInDunData					InDunData;

	SPcDataInven()
	{
		memset( WearItem   , 0 , sizeof( SPcItem*    ) * RP_MAX_EQUIP      );
		memset( InvenItem  , 0 , sizeof( SPcItem*    ) * c_MaxInvenItemNum  );
		memset( InvenQuest , 0 , sizeof( SPcItem*    ) * RP_MAX_QUEST_ITEM );
		memset( InvenHorse , 0 , sizeof( SPcItem*    ) * RP_MAX_QUEST_ITEM );		
		
		memset( SkillItem  , 0 , sizeof( SPcItem*     ) * RP_MAX_USER_SKILL );
		memset( QuickSlot  , 0 , sizeof( SPcItem*     ) * RP_MAX_QUICK_SLOT );
		memset( QuestFlag  , 0 , sizeof( SQuestFlag*  ) * RP_MAX_QUEST_FLAG );
		memset( QuestCount , 0 , sizeof( SQuestCount* ) * RP_MAX_QUEST_COUNT );		
		memset( PayEffect , 0 ,  sizeof( SPayEffect*  ) * RP_MAX_PAY_CONTENTS );	
		memset( PayTimeItemList , 0 ,  sizeof( SPayTimeItem*) * RP_MAX_TIMELIMIT_PAY_CONTENTS );
		memset( thePetData , 0 , sizeof( SPetData*    ) * 4  );	
		
		memset( QuestTime , 0 , sizeof( SQuestTime*)  * RP_MAX_QUEST_COUNT);

		FrontierMyInfo = NULL;
		FrontierInfo = NULL;
		memset( FrontierCaller, 0, sizeof( char ) * RP_MAX_PC_NAME );		
		FrontierPetInfo = NULL;
		FrontierPetID   = 0;
		FrontierPetMaxExp = 0; 
		pComBineFrontierInfo = NULL;	
	}

/*
WORD			theWeight;

	DWORD			theWearItemNum : 4;		// 0 - 15;
	DWORD			theInvenItem1Num : 6;	// 0 - 63;
	DWORD			theInvenItem2Num : 6;
	DWORD			theSkillNum:7;
	DWORD			theKeepupSkillNum:4;
	BYTE			theQuickSlotNum;

	SRpdWearItemBase	theWearItem[RP_MAX_EQUIP];
	SRpdWearItemBase	theInvenItem1[c_MaxInvenItemNum];
	SRpdWasteItem	theInvenItem2[c_MaxInvenItemNum];	// InvenItem�� ���հ����� RP_MAX_INVENTORY �̴�.

	// Skill
	WORD			theSkill[RP_MAX_USER_SKILL];		// ���� ���׷��̵� �� ��ų�� ����
	SRpdKeepupSkill	theKeepupSkill[RP_MAX_KEEPUP_SKILL];

	SRpdQuickSlot	theQuickSlot[RP_MAX_QUICK_SLOT];	// skill code ��
	//*/
};



enum N_FUNCTIONKEY
{
	n_FK1,
	n_FK2,
	n_FK3,
	n_FK4,
	n_FK5,
	n_FK6,
	n_FK7,
	n_FK8,
	n_FK9,
	n_FK0,
	n_FKF1,
	n_FKF2,
	n_FKF3,
	n_FKF4,
	n_FKF5,
	n_FKF6,
	n_FKF7,
	n_FKF8,
	n_FKF9,
	n_FKF0,
	n_FKSF1,
	n_FKSF2,
	n_FKSF3,
	n_FKSF4,
	n_FKSF5,
	n_FKSF6,
	n_FKSF7,
	n_FKSF8,
	n_FKSF9,
	n_FKSF0
};

#define MAX_STORE_SERVICE_ITEM	60

#define MAX_STORE_SHOP			100	

enum N_DEALINGITEM
{						//Item Class
	n_STORE_NONE,		//
	n_STORE_WEAPON,		//����( ����, ���� )
	n_STORE_ARMOR,		//��( ����, ����, �尩, ���� )
	n_STORE_SUNDRIES,	//��ȭ( �������, ��ű� )
	n_STORE_USE,		//�Ҹ�ǰ( �Ҹ�ǰ, ��Ҹ�ǰ )
	n_STORE_POSITION,	//����
	n_STORE_ATTCKARMS,	//��������
	n_STORE_BULDING,	//����/����
	n_STORE_GTOWER,		//����Ÿ��
	n_STORE_ETC_DESTORY,	//���μ��� �ĸ������  
	n_STORE_ETC_SEALED,		//���μ��� ���ξ����� 

};

struct SStoreItem
{
	WORD	Code;
	WORD	Amount;						// 0�̸� ���� ���

	BYTE	DealingItemType;
	
	SItemBaseInfo*	Table;				// ������ ����Ʈ ���������θ� ���� ( ������ �ʾƵ� �� )	By wxywxy		

	SStoreItem()
	{
		Table = NULL;
	}
};

struct SStoreShop
{
	WORD	OwnerCode;//NPC Code
	DWORD	OwnerUID;
	int		NumItem;
	SStoreItem Item[MAX_STORE_SERVICE_ITEM];

	///--JAPAN_BUG_MODIFY
	SStoreShop()
	{
		memset(this, 0, sizeof(SStoreShop));
	}
};

struct SStoreList
{
	int NumShop;
	SStoreShop Shop[MAX_STORE_SHOP];
};


enum N_QUESTTAB
{
	n_QUEST_ALL_TAB,
	n_QUEST_NORMAL_TAB,
	n_QUEST_STORY_TAB,
	n_QUEST_CHANGE_TAB,
};

struct SFWord
{
	TCHAR	Slang[20];
	TCHAR	Standard[20];
};

//by simwoosung

enum ChatWndSizeState
{
	ChatWndSizeNone = 0,
	ChatWndSizeMin,
	ChatWndSizeMid,
	ChatWndSizeMax,
};

struct STextData
{
	TCHAR		Text[256];		//��� �ؽ�Ʈ
	ICOLOR4		Rcolor;			//Ref Color		

	STextData()
	{
		SAFE_STR_CPY( Text, _RT(""), 256 );		
		Rcolor[A] = Rcolor[R] = Rcolor[G] = Rcolor[B] = 255;		
	}
	STextData( TCHAR* str )
	{
		SAFE_STR_CPY( Text, str, 256 );
		Rcolor[A] = Rcolor[R] = Rcolor[G] = Rcolor[B] = 255;			
	}
	void operator=(const STextData &temp)
	{
		SAFE_STR_CPY( Text, temp.Text, 256 );
		Rcolor[A] = temp.Rcolor[A];
		Rcolor[R] = temp.Rcolor[R];
		Rcolor[G] = temp.Rcolor[G];
		Rcolor[B] = temp.Rcolor[B];			
	}
};

struct STransTxtData
{
	STextData  TextData;	
	SPoint2	   SPos;		//������ġ
	SPoint2	   EPos;		//������ġ
	
	SPoint2	   NPos;        //������ġ
	bool	   IsRender;	//��������

	UINT       NowState;
	
	STransTxtData()
	{
		IsRender = false;
		NowState = 0;
	}
};

struct STipString
{
	TCHAR		str[256];
	int			x;
	int			y;
	D3DCOLOR	color;
	int			SetMode;	//���� ��� 0 �Ϲ� ������ 1 �Ѱ��� ���� 4�� ���� .. 

	STipString()
	{
		SAFE_STR_CPY( str, _RT(""), 256 );
		x	= 0;
		y	= 0;
		SetMode = 0 ;	
		color = D3DCOLOR_XRGB( 255, 255, 255 );
	}
};

#define MAX_ITEMTIP_WIDTH		512
#define BV						12		//BLANK 
#define RV						14		//RETURN

struct SColAreaDetailInfo
{
	WORD	aColonyCode;
	char	strFrontierName[RP_MAX_COMBINED_FRONTIERNAME];
	char	strMasterPCName[RP_MAX_PC_NAME];
	BYTE	aMemberNum;
	time_t	aWarTime;		// ���������ɽð� or �̹� ��������� �ð�
	char	strAttackFName[RP_MAX_COMBINED_FRONTIERNAME];

	SColAreaDetailInfo()
	{
		memset(this, 0, sizeof(SColAreaDetailInfo));
	}
};

struct SColAreaData
{
	WORD aColonyCode;	 //�ݷδ� �� �ڵ�
	SColAreaDetailInfo * pColAreaDetailInfo;
	SColAreaData()
	{
		memset(this, 0, sizeof(SColAreaData));
	}
};

struct ReadFileData
{	

	TCHAR AppName[64];
	TCHAR KeyName[64];
	TCHAR DataStr[512];
	
	ReadFileData()
	{
		memset( &AppName, NULL , sizeof(AppName));
		memset( &KeyName, NULL , sizeof(KeyName));
		memset( &DataStr, NULL , sizeof(DataStr));

	}

};
	
//-----------------------------------------------------------------------------
#endif  __DEFINE_H__