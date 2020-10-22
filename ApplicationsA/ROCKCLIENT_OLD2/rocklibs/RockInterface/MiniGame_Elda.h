#ifndef __CMiniGame_Elda__
#define __CMiniGame_Elda__


//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"

#include "Define.h"
#include "Fontman.h"
#include "Render.h"
#include "Rui.h"


#pragma pack(1)

#define ELDA_MAX_CELL_I			5
#define ELDA_MAX_CELL_J			7

#define ELDA_SOURCE_CELL_I		2
#define ELDA_SOURCE_CELL_J		6


///---------------------------------------------------------------------------
///-- MINI_GAME_MODE
///---------------------------------------------------------------------------
enum ELDA_GAME_MODE
{
	n_M_None = 0,	
	n_M_On_Ready,		///-- 시작 직전의 상태 
	n_M_On_Go,			///-- Go 진행
	n_M_On_GAME,		///-- Game 진행중 
	n_M_End,			///-- Game 종료 : 종료 메시지를 보인다 
	n_M_Destroy			///-- Game 종료 : Rui 에서 창을 소멸시킨다. 
};
///-----------------------------------------------------------------------------
///-- ELDA_CELL_TYPE
///-----------------------------------------------------------------------------
enum ELDA_CELL_TYPE
{
	n_ELDA_None =0,
	n_ELDA_LT,
	n_ELDA_RB,
	n_ELDA_LB,
	n_ELDA_RT,
	n_ELDA_BoxRL,
	n_ELDA_BoxLR,
	n_ELDA_Shadow
};


///---------------------------------------------------------------------------
///-- ELDA_CELL
///---------------------------------------------------------------------------
///-- 두개의 삼각형이 들어 갈 수 있는 정방형의 Cell
class ELDA_CELL
{
public:
	ELDA_CELL::ELDA_CELL()
	{
		mi_color = 0;
		mi_sub_color = 0;
		me_type = n_ELDA_None;
	}

	char mi_color;				///-- Cell 의 Color
	char mi_sub_color;			///-- 윗쪽 Cell 의 Color
	char me_type;		///-- 현재 Cell 의 type
 
	ELDA_CELL& operator=(const ELDA_CELL &);
	bool operator==(const ELDA_CELL &) const;	
	
};


///---------------------------------------------------------------------------
///-- ELDA_Data_Header
///---------------------------------------------------------------------------
class ELDA_Data_Header
{
public:
	signed long int _version;			///-- 현재 Puzzle Map 의 Version
	signed long int _mapcount;		///-- 포함하는 Map 의 총 개수	
	signed long int _countperlevel;	///-- Level 당  Map 의 개수 
	signed long int _maxlevel;		///-- 최대 Level ( 1부터 시작 함 )	
};
///---------------------------------------------------------------------------
///-- ELDA_Data_Set
///---------------------------------------------------------------------------
class ELDA_Data_Set
{
public:
	ELDA_Data_Set::ELDA_Data_Set()
	{		
		_id = 0;
		_level	= 0;		
		_max_col	= 0;	
		_max_color	= 0;	
		_limit_time	=0;	

		ZeroMemory(_Cells, 35 * sizeof( ELDA_CELL ) );
	}
	
	signed long int _id;	///-- 현재 puzmap 의 id (순차적)
	char _level;			///-- 현재 Puzmap 의 level
	char _max_col;			///-- 현재 Puzmap 의 Row
	char _max_color;		///-- 현재 Puzmap 의 Max Color
	char _limit_time;		///-- 현재 Puzmap 의 제한시간

	ELDA_CELL _Cells[5][7];	
};

///---------------------------------------------------------------------------
///-- CMiniGame_Elda
///---------------------------------------------------------------------------
class CMiniGame_Elda  
{
public:
	CMiniGame_Elda();
	virtual ~CMiniGame_Elda();

	///-----------------------------------------------------------------------
	///-- Public Method

	///-- Render / Time Proc
	void RenderProc();

	///-- Msg Proc
	N_EVENTRESULT EventProc( SKeys* Keys, SMouse* Mouse );

	///-- Logic Prepare Game
	void Logic_Prepare_Game( const int ai_pmap_index ,  const int ai_game_type, 
							 BYTE theGameTimeRatio = 100 );						///-- Game 을 시작하기 전의 준비

	///-- Logic Flow Manager
	void Logic_Flow_Manager();											///-- 흐름 변화 Manager

	///-- Logic Pick
	void Logic_Pick_Source_Cell( const int ai_row, const int ai_col );	///-- 유저 입력 Game 처리	
	
	///-- Gen_Goal_Cells
	void Gen_Goal_Cells( const int ai_pmap_index, BYTE theGameTimeRatio );
	
	///-- Gen_Source_Cells
	void Gen_Source_Cells();

	///-- Add_WorkingCell
	int Add_WorkingCell( const ELDA_CELL * const ac_cell );

	
	bool Check_Column_Complete() const;
	

private:

	///-----------------------------------------------------------------------
	///-- Private Method ( Function )

	void _clear();
	void _Set_Level();

	void _Render_Cell( const ELDA_CELL * const ac_cell, const DWORD ai_color ); 
	void _Render_Goal_Focus();
	void _Render_Eff_Blinking_Cells();
	void _Render_Eff_Shadow();
	void _Render_Eff_Cell_Hit();
	void _Render_Eff_Source_Slide();
	void _Render_Eff_ComboMsg();
	void _Render_Eff_Miss();
	void _Render_Eff_MSGGo();
	void _Render_Eff_MSGReady();
	void _Render_Eff_MSGEnd();
	void _Render_Eff_MSGTimeOver();
	
	//by simwoosung
	void _RenderTitle();
	
	
	TEXID _TIDGet_Cell( const ELDA_CELL_TYPE ae_type , const int ai_color_idx ) const;			///-- 해당 Type 의 Tid return 
	TEXID _TIDGet_Cell_Blinking( const ELDA_CELL_TYPE ae_type, const int ai_frame ) const;			///-- 해당 Type 의 Blinking Tid return 
	TEXID _TIDGet_CellHit( const char ac_type ) const;
	D3DCOLOR _ColorGet_Idx( const int ai_color_idx ) const;		///-- 해당 Idx 의 Color 값 return
	
	void _EffSet_Shadow( const int ai_col );						///-- Shadow Animation
	void _EffSet_CellHit( const int ai_row, const int ai_col );	///-- Cell Hit Animation
	void _EffSet_Source_Slide( );
	void _EffSet_Combo_Msg( );
	void _EffSet_Miss();
	void _EffSet_Ready_Msg();
	void _EffSet_Go_Msg();
	void _EffSet_End_Msg();
	void _EffSet_Perfect_Msg();
	void _EffSet_TimeOver_Msg();

	
	void _Process_Complete_Column();	
	void _Process_Complete_Total();
	void _Process_Abort(const int ai_code);	///-- code 1: Time Over  0 : Abort
	void _Process_Miss();
	void _Process_Hit();
	void _Time_Process();

	void _SetMovement();
	void _Process_SrcCell_Move();
	
	///-----------------------------------------------------------------------
	///-- Private Member Variable
	
	SRect	theBaseRect;	///-- Base Rect
	

	///-- Game Logic 관련
public:
	ELDA_GAME_MODE theGameMode;			///-- 현재 Game 의 Mode
private:

	int theScore;						///-- Game 전체 Score
	int theColumnScore;					///-- 현재 column clear Score
	int theHitCount;					///-- 현재 Column 에서 Hit한 수
	int theMissCount_100;				///-- 현재 Column 에서 (100점)Miss 한 수
	int theMissCount_50;				///-- 현재 Column 에서 (50점)Miss 한 수
	
	
	int theComboCnt;					///-- 현재 까지 연속된 Combo 의 횟수 
	bool theComboEnable;				///-- 현재 Column 에서 Combo 가능한지 여부
	int theTimeTotal;					///-- 전체 게임에 남은 시간.
	int theTimeTurn;					///-- 한턴에 남은 시간	
	int theprev_time ;
	

	///-- Game Setting 관련
	int theLevel;						///-- 현재 Game 의 Level		
	int theGameId;						///-- 현재 Game 의 Level		
	int theLimitCellsCol;				///-- 현재 Level 에서의 Cells Column 개수
	int theLimitCellsRow;				///-- 현재 Level 에서의 Cells Row 개수
	int theLimitColor;					///-- 현재 Level 에서의 Color 개수
	int theLimitSourceNum;				///-- 현재 Level 에서의 Source 개수

	int theWorkingCol;					///-- 현재 조합중인 컬럼
	int theImgID;						///-- Texture 선택
	
	// 치트 
	char m_color;				///-- Cell 의 Color
	char m_type;		///-- 현재 Cell 의 type
	bool thebCheatBlock;
	int m_CheatI;
	int m_CheatJ;


	///-- Cells Data		
	ELDA_CELL theGoalCells[ELDA_MAX_CELL_I][ELDA_MAX_CELL_J];				///-- 목표 영역
	ELDA_CELL theWorkCells[ELDA_MAX_CELL_I][ELDA_MAX_CELL_J];				///-- 작업 영역
	ELDA_CELL theSourceCells[ELDA_SOURCE_CELL_I][ELDA_SOURCE_CELL_J];		///-- 소스 영역
	POINT	theSourceCellsPos[ELDA_SOURCE_CELL_I][ELDA_SOURCE_CELL_J];		///-- 소스 Cell 들의 좌표
	POINT	theSource_Mov[ELDA_SOURCE_CELL_I][ELDA_SOURCE_CELL_J];			///-- 소스 Cell 들의 이동량

/*
	int theNBaseX;			///-- Base X
	int theNBaseY;			///-- Base Y
	int theNBaseWidth;		///-- Base Width
	int theNBaseHight;		///-- Base Height
*/

	///-- Effect 관련
	int the_proc_frame;					///-- 처리해야될 Ani Frame
	int the_accum_frame;					///-- 누적되는 frame count
	
	int the_proc_frame2;					///-- 처리해야될 Ani Frame
	int the_accum_frame2;					///-- 누적되는 frame count

	int theEff_WorkCells[ELDA_MAX_CELL_I][ELDA_MAX_CELL_J];	///-- 작업 영역 Effect Frame 을 저장하고 있다. -1은 Effect 없음
	int theEff_Blinking[ELDA_MAX_CELL_I][ELDA_MAX_CELL_J];	///-- 작업 영역 Blinking Effect Frame 을 저장하고 있다. -1은 Effect 없음
	
	///-- Shadow
	bool theShadowDrop;					///-- Shadow 한줄이 떨어지는 Animaiton
	float theShadow_Eff_x;				///-- Shadow ani x
	float theShadow_Eff_y;				///-- Shadow ani x
	float theShadow_Eff_mov_x;			///-- Shadow ani 변화량
	float theShadow_Eff_mov_y;			///-- Shadow ani 변화량
	int theShadow_frame;				///-- 진행 프레임

	///-- Slide
	bool theSlideCell_out;				///-- Source Cell 들의 Animation Out
	bool theSlideCell_in;				///-- Source Cell 들의 Animation In
	float theSlide_x;					///-- Source 영역의 Render 좌표
	float theSlide_y;					///-- Source 영역의 Render 좌표
	int theSlide_frame;					///-- Source Slide 진행 프레임
	float theSlide_speed;				///-- Slide 변화량

	///-- Combo
	bool theComboMsg;					///-- Messaga View
	int theComboMsg_frame;				///-- Combo Message Frame

	bool theInterjection;				///-- interjection View
	int theInterjection_frame;			///-- Combo Message Frame

	bool theMiss;						///-- Miss View
	int theMiss_frame;					///-- Miss Frame

	bool theReady;
	int theReady_frame;

	bool theGo;
	int theGo_frame;

	bool theTimeOver;
	int theTimeOver_frame;

	bool thePerfect;
	int thePerfect_frame;

	bool theGameEnd;
	int theGameEnd_frame;
	
	///-- Focus Cell 
	int theFocus_frame;


	int mb_abort_bt_ovr; ///-- 0 : Normal , 1 : Over , 2 : Click

	//By simwoosung
	CTextBox *pButtonTxt;
	TCHAR	  m_strTitleName[256];

};
#pragma pack()

///---------------------------------------------------------------------------
#endif __CMiniGame_Elda__