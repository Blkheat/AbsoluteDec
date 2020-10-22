//----------------------------------------------------------------------------------------------------------------------
// 파일명 : CharSelectWndProc.h
// 용  도 : 캐릭터 생성창
//----------------------------------------------------------------------------------------------------------------------
#ifndef __RUI_CHAR_SELECT_WNDPROC_H
#define __RUI_CHAR_SELECT_WNDPROC_H

#include "WndProc.h"
#include "..\\bravolibs\obj\character.H"

#define MAX_PC_SLOT_NUM 5	

//............................................................................................................
// 캐릭터 생성 창에서 나오는 캐릭터 초기 옷~
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
	SEQ_NONE		 = 0 ,					// 캐릭터 선택창 상태 아님
	SEQ_CAMERA_ANI       ,					// 카메라 에니메이션
	SEQ_SELECT_CHAR		 ,				    // 캐릭터 선택창
	SEQ_MILENA_CREATE    ,					// 밀레나 생성창
	SEQ_MILENA_CLASS     ,					// 밀레나 직업군
	SEQ_RAIN_CREATE		 ,					// 레인 생성창
	SEQ_RAIN_CLASS		 ,					// 레인 직업군
	SEQ_SELECT_ANI		    ,				// 캐릭터 선택창 이동중
	SEQ_CAMERA_ZOOM_MILLENA ,				// 줌 밀레나
	SEQ_CAMERA_ZOOM_OUT_MILLENA ,			// 줌 아웃 밀레나
	SEQ_CAMERA_ZOOM_RAIN        ,           // 줌 레인
	SEQ_CAMERA_ZOOM_OUT_RAIN				// 줌 아웃 레인
};


typedef enum CSW_STATE
{
	CSW_SELECT_NONE  = 0 ,					// 캐릭터 선택창 정보 없음
	CSW_SELECT_CORE      ,					// 캐릭터 선택창 코어
	CSW_SELECT_EXIST     ,					// 캐릭터 선택창 정보 있음
	CSW_SELECT_DELETE    ,					// 지워진 캐릭터
	CSW_CREATE								// 캐릭터 생성창
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
	CTextBox*		PcNameTxt;				// 이름	
	CTextBox*		PcLevelTxt;				// 레벨    
	CTextBox*		PcClassTxt;				// 종류   
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
	
	int StartCameraPosition(void);					// 카메라 첫 위치
	int StartSelectCameraWalkPostion(void);			// 카메라 선택 위치로 에니메이션
	void SelectCameraWalkEnd();						// 선택화면으로 카메라 워킹 끝났을때 처리
	
	int  StartMillenaCameraWalkPostion(void);
	void CreateMillenaWalkEnd();					// 밀레나 생성창 카메라 워킹 끝났을때 처리
	int	 StartRainCameraWalkPosition(void);
	void CreateRainWalkEnd();

	void SetNoneCharacterSelectBtn(void);			// 캐릭터 선택창 ( 아무것도 선택 안함 )
	void SetCloseAllBtn(void);						// 전체 버튼 닫기
	void SetCreateCharacterBtn(void);				// 생성창 버튼

	void StartReturnMillenaToSelect(void);						// 밀레나 생성창 -> 선택창으로
	void StartReturnMillenaToSelectCameraWalkEnd(void);			// 밀레나 생성창 -> 선택창으로 ( 끝났을때 )


	void StartMillenaToRain(void);					// 밀레나 -> 레인 
	void StartMillenaToRainCameraWalkEnd(void);		// 밀레나 -> 레인 ( 끝났을때 )

	
	void StartRainToMillena(void);					// 레인 -> 밀레나  
	void StartRainToMillenaCameraWalkEnd(void);		// 레인 -> 밀레나 ( 끝났을때 )

	
	void StartReturnRainToSelect(void);				 // 레인 생성창 -> 선택창으로
	void StartReturnRainToSelectCameraWalkEnd(void); // 레인 생성창 -> 선택창으로 ( 끝났을때 )

	BOOL ItemNormalCheck(void *_pc , TCHAR *PcName);


public:

	WEAR_SET	m_WearSet[ 4 ];						// 0 : 전사 ( 남 ) , 1 : 전사 ( 여 ) , 2 : 법사 ( 남 ) , 3 : 법사 ( 여 )

	void StartUp(void);								// 시작시 세팅
	void CloseAll(void);							// 닫기
	
	CSW_STATE GetState(void) { return m_State; }	// 상태 얻기

	
	// 생성된 캐릭터 셋팅
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
	int		  GetCoreNum(void);									// 코어 번호 얻기
	void	  DeletePlayer(void);

	Character  *pCharList[5];									// 캐릭터 

	void CreateClassCharacter();								// 클래스 캐릭터 생성

private:

	int			m_Core;
	CSW_STATE	m_State;
	
	int			m_SelectNum;									// 캐릭터 선택 번호

	float		m_fRotY;										// 캐릭터 회전

private:

	CFrameWnd*		m_SelectWnd;								// 캐릭터 선택창 프레임
	
	CImageBox*		m_SelectNameBoxImage;						// 캐릭터 선택 이름 배경 그림
	CImageBox*		m_SelectCreateBoxImage;						// 캐릭터 선택 생성 배경 그림

	CButton*		m_Select_Start_Btn;							// 캐릭터 선택 시작 버튼
	CButton*		m_Select_Create_Btn;						// 캐릭터 선택 생성 버튼
	CTextBox*		m_Select_Name_Edit;							// 캐릭터 이름 에디트 박스
	CTextBox*		m_Select_Level_Edit;						// 캐릭터 레벨 에디트 박스

	CTextBox*		m_nationtext_millena;						// 캐릭터 레벨 에디트 박스
	CTextBox*		m_nationtext_rain;						// 캐릭터 레벨 에디트 박스
	CTextBox*		m_nationtext_select;						// 캐릭터 레벨 에디트 박스


	CButton*		m_Select_Server_Btn0;						// 캐릭터 선택 서버 버튼
	CButton*		m_Select_Delete_Btn;						// 삭제 버튼

	CButton*		m_Create_Rot_Left_Btn;						// 캐릭터 생성 왼쪽    버튼
	CButton*		m_Create_Rot_Right_Btn;						// 캐릭터 생성 오른 쪽 버튼
	

	CButton*		m_Create_Millena_Btn;						// 캐릭터 생성 밀레나 버튼
	CButton*		m_Create_Rain_Btn;							// 캐릭터 생성 레인 버튼

	//CRadioButton*	m_Create_Millena_Btn;						// 캐릭터 생성 밀레나 버튼
	//CRadioButton*	m_Create_Rain_Btn;							// 캐릭터 생성 레인 버튼
	
	CButton*		m_Create_Male_Btn;							// 캐릭터 생성 남자 버튼
	CButton*		m_Create_Female_Btn;						// 캐릭터 생성 여자 버튼

	CButton*		m_pClassViewBtn;							// 클래스 미리보기

	CButton*		m_Create_Hair_Left_Btn;						// 캐릭터 생성 머리 왼쪽   버튼
	CButton*		m_Create_Hair_Right_Btn;					// 캐릭터 생성 머리 오른쪽 버튼

	CButton*		m_Create_Face_Left_Btn;						// 캐릭터 생성 얼굴 왼쪽   버튼
	CButton*		m_Create_Face_Right_Btn;					// 캐릭터 생성 얼굴 오른쪽 버튼

	CButton*		m_Create_Create_Btn;						// 캐릭터 생성 생성 버튼
	CButton*		m_Create_Cancel_Btn;						// 캐릭터 생성 취소 버튼


	CButton*		m_ClassViewReturn;							// 클래스 미리보기 뒤로가기

	// 족보 버튼
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
	CEditBox*		m_Create_Name_Edit;							// 캐릭터 생성 이름 에디트 박스

	CImageBox*		m_Create_HelpImage;							// 캐릭터 생성 도움말

	CImageBox*		m_CreateDlgmage;							// 캐릭터 생성 뒷 배경
	
	CTextBox*		m_SelectClassWnd;

	CButton*        m_pSelectBtn[ 5 ];							// 캐릭터 선택 박스

	CCheckButton*	m_pZoomBtn;									// 줌 버튼

	CTextBox*		m_pHelpText;								// 도움말 텍스트 박스
	CScrollBar*		m_pHelpScrollBar;							// 도움말 스크롤바
	
	
	CImageBox*		m_ClassViewImage;					// 클래스 뷰 뒷 배경	
	CImageBox*		m_ClassViewImageSiGong;				//한국 미리보기 이미지 시공창가 

	BOOL			m_ISZoom;									// 줌 여부

	WORD			m_MillenaHairCodeTable[2][TABLE_HAIR_MAX_COUNT];
	WORD			m_MillenaFaceCodeTable[2][TABLE_FACE_MAX_COUNT];

	WORD			m_RainHairCodeTable[2][TABLE_HAIR_MAX_COUNT];
	WORD			m_RainFaceCodeTable[2][TABLE_FACE_MAX_COUNT];

	WORD			m_pSelectHairCode[2][TABLE_HAIR_MAX_COUNT];	
	WORD			m_pSelectFaceCode[2][TABLE_FACE_MAX_COUNT];	

	int				m_SelectHairNum;							// 머리 선택 번호
	int				m_SelectFaceNum;							// 얼굴 선택 번호

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
	
	CButton*		m_Select_exit_Btn;						// 종료 버튼
	CImageBox*		m_SelectCharBoxImage;                 // 배경이미지
	
	TCHAR* SetRenewalClassNameString( BYTE _mainclass );  // 클레스 이름불러오기
	int SetRenewalClassImg( BYTE _mainclass );            // 클레스 이미지불러오기
#endif // DECO_RENEWAL_MJH
	void SetMainClassNameString( BYTE _mainclass );
	BOOL			m_ISChangeSkyBox;

public:	
	BYTE			m_ViewClass;        // 클래스 미리 보기
	D3DXVECTOR3     GetPos(int x,int z, bool bFlag = false);
	void			SetChangeSkyBox(BOOL IsChange){m_ISChangeSkyBox = IsChange;}
									
	
	CButton *		GetSelect_Create_Btn()	{ return m_Select_Create_Btn; }
#ifdef DECO_RENEWAL_MJH
	int GetCharacterSelectNum();
#endif // DECO_RENEWAL_MJH
	
public:
	
	CSW_SEQUENCE_STATE	m_SEQState;								// 창 상태
};


extern	CCharSelectWnd		CharSelectWnd;


#endif