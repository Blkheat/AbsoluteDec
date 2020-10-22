//----------------------------------------------------------------------------------------------------------------------
// ���ϸ� : CharSelectWndProc.h
// ��  �� : ĳ���� ����â
//----------------------------------------------------------------------------------------------------------------------
#ifndef __RUI_CHAR_SELECT_WNDPROC_H
#define __RUI_CHAR_SELECT_WNDPROC_H

#include "WndProc.h"
#include "..\\bravolibs\obj\character.H"

#define MAX_PC_SLOT_NUM 5	

//............................................................................................................
// ĳ���� ���� â���� ������ ĳ���� �ʱ� ��~
//............................................................................................................
typedef struct s_WearSet
{
	int	UpperItemIndex;
	int LowerItemIndex;
	int HandItemIndex;
	int FootItemIndex;

	s_WearSet()
	{
		UpperItemIndex =	
		LowerItemIndex =
		HandItemIndex  =	
		FootItemIndex  = -1;	
	}

}WEAR_SET,*LPWEAR_SET;


typedef enum CSW_SEQUENCE_STATE
{
	SEQ_NONE		 = 0 ,					// ĳ���� ����â ���� �ƴ�
	SEQ_CAMERA_ANI       ,					// ī�޶� ���ϸ��̼�
	SEQ_SELECT_CHAR		 ,				    // ĳ���� ����â
	SEQ_MILENA_CREATE    ,					// �з��� ����â
	SEQ_MILENA_CLASS     ,					// �з��� ������
	SEQ_RAIN_CREATE		 ,					// ���� ����â
	SEQ_RAIN_CLASS		 ,					// ���� ������
	SEQ_SELECT_ANI		    ,				// ĳ���� ����â �̵���
	SEQ_CAMERA_ZOOM_MILLENA ,				// �� �з���
	SEQ_CAMERA_ZOOM_OUT_MILLENA ,			// �� �ƿ� �з���
	SEQ_CAMERA_ZOOM_RAIN        ,           // �� ����
	SEQ_CAMERA_ZOOM_OUT_RAIN				// �� �ƿ� ����
};


typedef enum CSW_STATE
{
	CSW_SELECT_NONE  = 0 ,					// ĳ���� ����â ���� ����
	CSW_SELECT_CORE      ,					// ĳ���� ����â �ھ�
	CSW_SELECT_EXIST     ,					// ĳ���� ����â ���� ����
	CSW_SELECT_DELETE    ,					// ������ ĳ����
	CSW_CREATE								// ĳ���� ����â
};


#define TABLE_HAIR_MAX_COUNT				10
#define TABLE_FACE_MAX_COUNT				6


typedef enum CSW_POS
{
	CSW_SELECT_POS0 = 0 ,
	CSW_SELECT_POS1 = 1 ,
	CSW_SELECT_POS2 = 2 ,
	CSW_SELECT_POS3 = 3 ,
	CSW_SELECT_POS4 = 4 ,
	CSW_SELECT_POS  = 5 ,
	CSW_MILLENA_POS = 6 ,
	CSW_RAIN_POS    = 7 
};

typedef struct s_SelectPosInfo
{

	int			CelPos[ 2 ];
	DWORD       Unique;	
	
	DWORD		DestCell;

	void SetPos(int x,int z,DWORD code)
	{
		CelPos[ 0 ] = x;
		CelPos[ 1 ] = z;

		Unique   = code;
		DestCell = z * 256 + x;
	}

}SELECT_POSINFO;








class CFrameWnd;
class CImageBox;
class CButton;
class CTextBox;
class CRadioButton;
class CImageBox9;
class CEditBox;
class CCheckButton;
class CScrollBar;
#ifdef DECO_RENEWAL_MJH
struct SPcText
{
	CImageBox*	    PcImgBox;
	CTextBox*		PcNameTxt;				// �̸�	
	CTextBox*		PcLevelTxt;				// ����    
	CTextBox*		PcClassTxt;				// ����   
};

#endif // DECO_RENEWAL_MJH
class CCharSelectWnd : public CWndProc
{
public:

	 CCharSelectWnd();

	virtual void Composition();
	virtual void Update();
	virtual void Proc( SEventMessage* EMsg );

public:
	
	SELECT_POSINFO	PosInfo[ 9 ];

	TCHAR Tmp[256];

public:
	
	int StartCameraPosition(void);					// ī�޶� ù ��ġ
	int StartSelectCameraWalkPostion(void);			// ī�޶� ���� ��ġ�� ���ϸ��̼�
	void SelectCameraWalkEnd();						// ����ȭ������ ī�޶� ��ŷ �������� ó��
	
	int  StartMillenaCameraWalkPostion(void);
	void CreateMillenaWalkEnd();					// �з��� ����â ī�޶� ��ŷ �������� ó��
	int	 StartRainCameraWalkPosition(void);
	void CreateRainWalkEnd();

	void SetNoneCharacterSelectBtn(void);			// ĳ���� ����â ( �ƹ��͵� ���� ���� )
	void SetCloseAllBtn(void);						// ��ü ��ư �ݱ�
	void SetCreateCharacterBtn(void);				// ����â ��ư

	void StartReturnMillenaToSelect(void);						// �з��� ����â -> ����â����
	void StartReturnMillenaToSelectCameraWalkEnd(void);			// �з��� ����â -> ����â���� ( �������� )


	void StartMillenaToRain(void);					// �з��� -> ���� 
	void StartMillenaToRainCameraWalkEnd(void);		// �з��� -> ���� ( �������� )

	
	void StartRainToMillena(void);					// ���� -> �з���  
	void StartRainToMillenaCameraWalkEnd(void);		// ���� -> �з��� ( �������� )

	
	void StartReturnRainToSelect(void);				 // ���� ����â -> ����â����
	void StartReturnRainToSelectCameraWalkEnd(void); // ���� ����â -> ����â���� ( �������� )

	BOOL ItemNormalCheck(void *_pc , TCHAR *PcName);


public:

	WEAR_SET	m_WearSet[ 4 ];						// 0 : ���� ( �� ) , 1 : ���� ( �� ) , 2 : ���� ( �� ) , 3 : ���� ( �� )

	void StartUp(void);								// ���۽� ����
	void CloseAll(void);							// �ݱ�
	
	CSW_STATE GetState(void) { return m_State; }	// ���� ���

	
	// ������ ĳ���� ����
	void      SelectCharacter(void);
#ifdef DECO_RENEWAL_MJH
	void      SelectCharacter(int SelNum);
	void      CharacterInfoList(void);
#endif //DECO_RENEWAL_MJH

	void	  SelectCreateCharacter(int i);

	void      DeleteCharacter(void);
	
	void      ControlAllColse(void);
	void      ControlStateOpen(void);

	void      CreateCharacter(BYTE Citizen,BYTE Gender);
	void	  ChangeFaceAndHair(void);
	void	  ChangeType(void);
	void	  ChangeRotY(float Angle);
	void	  CreateOK(void);
	void	  CreateCharacterOK(void);

	void      SetCreatePos(void);

	void	  DeleteOK(void);

	SRpdCharacterDataWithCore  *m_pSelectChar;

	int GetItemIndex(int nFileName);
	
	// 
	int		  CameraSelectPosition(void);
	void      CM_MOVE(int i,int npos);
	void      SelectCharacterMove(int Num);	
	void	  EndZoomProcess();
	void	  ZoomOutEndProcess();
	int		  GetCoreNum(void);									// �ھ� ��ȣ ���
	void	  DeletePlayer(void);

	Character  *pCharList[5];									// ĳ���� 

	void CreateClassCharacter();								// Ŭ���� ĳ���� ����

private:

	int			m_Core;
	CSW_STATE	m_State;
	
	int			m_SelectNum;									// ĳ���� ���� ��ȣ

	float		m_fRotY;										// ĳ���� ȸ��

private:

	CFrameWnd*		m_SelectWnd;								// ĳ���� ����â ������
	
	CImageBox*		m_SelectNameBoxImage;						// ĳ���� ���� �̸� ��� �׸�
	CImageBox*		m_SelectCreateBoxImage;						// ĳ���� ���� ���� ��� �׸�

	CButton*		m_Select_Start_Btn;							// ĳ���� ���� ���� ��ư
	CButton*		m_Select_Create_Btn;						// ĳ���� ���� ���� ��ư
	CTextBox*		m_Select_Name_Edit;							// ĳ���� �̸� ����Ʈ �ڽ�
	CTextBox*		m_Select_Level_Edit;						// ĳ���� ���� ����Ʈ �ڽ�

	CTextBox*		m_nationtext_millena;						// ĳ���� ���� ����Ʈ �ڽ�
	CTextBox*		m_nationtext_rain;						// ĳ���� ���� ����Ʈ �ڽ�
	CTextBox*		m_nationtext_select;						// ĳ���� ���� ����Ʈ �ڽ�


	CButton*		m_Select_Server_Btn0;						// ĳ���� ���� ���� ��ư
	CButton*		m_Select_Delete_Btn;						// ���� ��ư

	CButton*		m_Create_Rot_Left_Btn;						// ĳ���� ���� ����    ��ư
	CButton*		m_Create_Rot_Right_Btn;						// ĳ���� ���� ���� �� ��ư
	

	CButton*		m_Create_Millena_Btn;						// ĳ���� ���� �з��� ��ư
	CButton*		m_Create_Rain_Btn;							// ĳ���� ���� ���� ��ư

	//CRadioButton*	m_Create_Millena_Btn;						// ĳ���� ���� �з��� ��ư
	//CRadioButton*	m_Create_Rain_Btn;							// ĳ���� ���� ���� ��ư
	
	CButton*		m_Create_Male_Btn;							// ĳ���� ���� ���� ��ư
	CButton*		m_Create_Female_Btn;						// ĳ���� ���� ���� ��ư

	CButton*		m_pClassViewBtn;							// Ŭ���� �̸�����

	CButton*		m_Create_Hair_Left_Btn;						// ĳ���� ���� �Ӹ� ����   ��ư
	CButton*		m_Create_Hair_Right_Btn;					// ĳ���� ���� �Ӹ� ������ ��ư

	CButton*		m_Create_Face_Left_Btn;						// ĳ���� ���� �� ����   ��ư
	CButton*		m_Create_Face_Right_Btn;					// ĳ���� ���� �� ������ ��ư

	CButton*		m_Create_Create_Btn;						// ĳ���� ���� ���� ��ư
	CButton*		m_Create_Cancel_Btn;						// ĳ���� ���� ��� ��ư


	CButton*		m_ClassViewReturn;							// Ŭ���� �̸����� �ڷΰ���

	// ���� ��ư
	CRadioButton*   m_MagicianBtn;
	CRadioButton*   m_ShamanBtn;
	CRadioButton*   m_ProminasBtn;
	CRadioButton*   m_PriestBtn;
	CRadioButton*   m_Holy_AvengerBtn;
	CRadioButton*   m_SummonerBtn;
	CRadioButton*   m_ArcherBtn;
	CRadioButton*   m_SwordManBtn;
	CRadioButton*   m_KnightBtn;
	CRadioButton*   m_MercenaryBtn;
	CRadioButton*   m_SlayerBtn;
	CRadioButton*   m_Soul_BreakerBtn;

	CImageBox9*      m_HelpImage;
	CImageBox9*      m_HelpContentImage;
	CEditBox*		m_Create_Name_Edit;							// ĳ���� ���� �̸� ����Ʈ �ڽ�

	CImageBox*		m_Create_HelpImage;							// ĳ���� ���� ����

	CImageBox*		m_CreateDlgmage;							// ĳ���� ���� �� ���
	
	CTextBox*		m_SelectClassWnd;

	CButton*        m_pSelectBtn[ 5 ];							// ĳ���� ���� �ڽ�

	CCheckButton*	m_pZoomBtn;									// �� ��ư

	CTextBox*		m_pHelpText;								// ���� �ؽ�Ʈ �ڽ�
	CScrollBar*		m_pHelpScrollBar;							// ���� ��ũ�ѹ�
	
	
	CImageBox*		m_ClassViewImage;					// Ŭ���� �� �� ���	
	CImageBox*		m_ClassViewImageSiGong;				//�ѱ� �̸����� �̹��� �ð�â�� 

	BOOL			m_ISZoom;									// �� ����

	WORD			m_MillenaHairCodeTable[2][TABLE_HAIR_MAX_COUNT];
	WORD			m_MillenaFaceCodeTable[2][TABLE_FACE_MAX_COUNT];

	WORD			m_RainHairCodeTable[2][TABLE_HAIR_MAX_COUNT];
	WORD			m_RainFaceCodeTable[2][TABLE_FACE_MAX_COUNT];

	WORD			m_pSelectHairCode[2][TABLE_HAIR_MAX_COUNT];	
	WORD			m_pSelectFaceCode[2][TABLE_FACE_MAX_COUNT];	

	int				m_SelectHairNum;							// �Ӹ� ���� ��ȣ
	int				m_SelectFaceNum;							// �� ���� ��ȣ

	BYTE			m_Citizen;
	BYTE			m_Gender;

#ifdef DECO_RENEWAL_MJH
	CTextBox*	tbox[8];				///-- Static Text
	CTextBox*	jobtbox[8];				///-- Static Text
	CImageBox*	jobtimg[8];				///-- Static Image
	SPcText m_PcText[MAX_PC_SLOT_NUM];
#else
	CTextBox*	tbox[7];				///-- Static Text
	CTextBox*	jobtbox[7];				///-- Static Text
	CImageBox*	jobtimg[7];				///-- Static Image
#endif // DECO_RENEWAL_MJH

	CImageBox*	nationimg;				///-- Static Image
	CTextBox*	racetext;				///-- Static Text
	CTextBox*	racehelptext;			///-- Static Text
	CTextBox*	selectchartext;			///-- Static Text	
		
#ifdef DECO_RENEWAL_MJH

	CTextBox*   m_Guildinfotext;          ///-- Static Text
	CTextBox*   m_Chracterinfotext;       ///-- Static Text
	CTextBox*   m_JoinGuildtext;          ///-- Static Text
	CTextBox*   m_JoinCorpstext;          ///-- Static Text
	CTextBox*   m_MapPositiontext;        ///-- Static Text
	CTextBox*   m_FameText;               ///-- Static Text
	CTextBox*   m_PcFameText;             ///-- Static Text
	CTextBox*   m_GuildFameText;          ///-- Static Text
	
	CButton*		m_Select_exit_Btn;						// ���� ��ư
	CImageBox*		m_SelectCharBoxImage;                 // ����̹���
	
	TCHAR* SetRenewalClassNameString( BYTE _mainclass );  // Ŭ���� �̸��ҷ�����
	int SetRenewalClassImg( BYTE _mainclass );            // Ŭ���� �̹����ҷ�����
#endif // DECO_RENEWAL_MJH
	void SetMainClassNameString( BYTE _mainclass );
	BOOL			m_ISChangeSkyBox;

public:	
	BYTE			m_ViewClass;        // Ŭ���� �̸� ����
	D3DXVECTOR3     GetPos(int x,int z, bool bFlag = false);
	void			SetChangeSkyBox(BOOL IsChange){m_ISChangeSkyBox = IsChange;}
									
	
	CButton *		GetSelect_Create_Btn()	{ return m_Select_Create_Btn; }
#ifdef DECO_RENEWAL_MJH
	int GetCharacterSelectNum();
#endif // DECO_RENEWAL_MJH
	
public:
	
	CSW_SEQUENCE_STATE	m_SEQState;								// â ����
};


extern	CCharSelectWnd		CharSelectWnd;


#endif