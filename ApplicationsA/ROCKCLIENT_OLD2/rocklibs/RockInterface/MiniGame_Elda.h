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
	n_M_On_Ready,		///-- ���� ������ ���� 
	n_M_On_Go,			///-- Go ����
	n_M_On_GAME,		///-- Game ������ 
	n_M_End,			///-- Game ���� : ���� �޽����� ���δ� 
	n_M_Destroy			///-- Game ���� : Rui ���� â�� �Ҹ��Ų��. 
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
///-- �ΰ��� �ﰢ���� ��� �� �� �ִ� �������� Cell
class ELDA_CELL
{
public:
	ELDA_CELL::ELDA_CELL()
	{
		mi_color = 0;
		mi_sub_color = 0;
		me_type = n_ELDA_None;
	}

	char mi_color;				///-- Cell �� Color
	char mi_sub_color;			///-- ���� Cell �� Color
	char me_type;		///-- ���� Cell �� type
 
	ELDA_CELL& operator=(const ELDA_CELL &);
	bool operator==(const ELDA_CELL &) const;	
	
};


///---------------------------------------------------------------------------
///-- ELDA_Data_Header
///---------------------------------------------------------------------------
class ELDA_Data_Header
{
public:
	signed long int _version;			///-- ���� Puzzle Map �� Version
	signed long int _mapcount;		///-- �����ϴ� Map �� �� ����	
	signed long int _countperlevel;	///-- Level ��  Map �� ���� 
	signed long int _maxlevel;		///-- �ִ� Level ( 1���� ���� �� )	
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
	
	signed long int _id;	///-- ���� puzmap �� id (������)
	char _level;			///-- ���� Puzmap �� level
	char _max_col;			///-- ���� Puzmap �� Row
	char _max_color;		///-- ���� Puzmap �� Max Color
	char _limit_time;		///-- ���� Puzmap �� ���ѽð�

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
							 BYTE theGameTimeRatio = 100 );						///-- Game �� �����ϱ� ���� �غ�

	///-- Logic Flow Manager
	void Logic_Flow_Manager();											///-- �帧 ��ȭ Manager

	///-- Logic Pick
	void Logic_Pick_Source_Cell( const int ai_row, const int ai_col );	///-- ���� �Է� Game ó��	
	
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
	
	
	TEXID _TIDGet_Cell( const ELDA_CELL_TYPE ae_type , const int ai_color_idx ) const;			///-- �ش� Type �� Tid return 
	TEXID _TIDGet_Cell_Blinking( const ELDA_CELL_TYPE ae_type, const int ai_frame ) const;			///-- �ش� Type �� Blinking Tid return 
	TEXID _TIDGet_CellHit( const char ac_type ) const;
	D3DCOLOR _ColorGet_Idx( const int ai_color_idx ) const;		///-- �ش� Idx �� Color �� return
	
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
	

	///-- Game Logic ����
public:
	ELDA_GAME_MODE theGameMode;			///-- ���� Game �� Mode
private:

	int theScore;						///-- Game ��ü Score
	int theColumnScore;					///-- ���� column clear Score
	int theHitCount;					///-- ���� Column ���� Hit�� ��
	int theMissCount_100;				///-- ���� Column ���� (100��)Miss �� ��
	int theMissCount_50;				///-- ���� Column ���� (50��)Miss �� ��
	
	
	int theComboCnt;					///-- ���� ���� ���ӵ� Combo �� Ƚ�� 
	bool theComboEnable;				///-- ���� Column ���� Combo �������� ����
	int theTimeTotal;					///-- ��ü ���ӿ� ���� �ð�.
	int theTimeTurn;					///-- ���Ͽ� ���� �ð�	
	int theprev_time ;
	

	///-- Game Setting ����
	int theLevel;						///-- ���� Game �� Level		
	int theGameId;						///-- ���� Game �� Level		
	int theLimitCellsCol;				///-- ���� Level ������ Cells Column ����
	int theLimitCellsRow;				///-- ���� Level ������ Cells Row ����
	int theLimitColor;					///-- ���� Level ������ Color ����
	int theLimitSourceNum;				///-- ���� Level ������ Source ����

	int theWorkingCol;					///-- ���� �������� �÷�
	int theImgID;						///-- Texture ����
	
	// ġƮ 
	char m_color;				///-- Cell �� Color
	char m_type;		///-- ���� Cell �� type
	bool thebCheatBlock;
	int m_CheatI;
	int m_CheatJ;


	///-- Cells Data		
	ELDA_CELL theGoalCells[ELDA_MAX_CELL_I][ELDA_MAX_CELL_J];				///-- ��ǥ ����
	ELDA_CELL theWorkCells[ELDA_MAX_CELL_I][ELDA_MAX_CELL_J];				///-- �۾� ����
	ELDA_CELL theSourceCells[ELDA_SOURCE_CELL_I][ELDA_SOURCE_CELL_J];		///-- �ҽ� ����
	POINT	theSourceCellsPos[ELDA_SOURCE_CELL_I][ELDA_SOURCE_CELL_J];		///-- �ҽ� Cell ���� ��ǥ
	POINT	theSource_Mov[ELDA_SOURCE_CELL_I][ELDA_SOURCE_CELL_J];			///-- �ҽ� Cell ���� �̵���

/*
	int theNBaseX;			///-- Base X
	int theNBaseY;			///-- Base Y
	int theNBaseWidth;		///-- Base Width
	int theNBaseHight;		///-- Base Height
*/

	///-- Effect ����
	int the_proc_frame;					///-- ó���ؾߵ� Ani Frame
	int the_accum_frame;					///-- �����Ǵ� frame count
	
	int the_proc_frame2;					///-- ó���ؾߵ� Ani Frame
	int the_accum_frame2;					///-- �����Ǵ� frame count

	int theEff_WorkCells[ELDA_MAX_CELL_I][ELDA_MAX_CELL_J];	///-- �۾� ���� Effect Frame �� �����ϰ� �ִ�. -1�� Effect ����
	int theEff_Blinking[ELDA_MAX_CELL_I][ELDA_MAX_CELL_J];	///-- �۾� ���� Blinking Effect Frame �� �����ϰ� �ִ�. -1�� Effect ����
	
	///-- Shadow
	bool theShadowDrop;					///-- Shadow ������ �������� Animaiton
	float theShadow_Eff_x;				///-- Shadow ani x
	float theShadow_Eff_y;				///-- Shadow ani x
	float theShadow_Eff_mov_x;			///-- Shadow ani ��ȭ��
	float theShadow_Eff_mov_y;			///-- Shadow ani ��ȭ��
	int theShadow_frame;				///-- ���� ������

	///-- Slide
	bool theSlideCell_out;				///-- Source Cell ���� Animation Out
	bool theSlideCell_in;				///-- Source Cell ���� Animation In
	float theSlide_x;					///-- Source ������ Render ��ǥ
	float theSlide_y;					///-- Source ������ Render ��ǥ
	int theSlide_frame;					///-- Source Slide ���� ������
	float theSlide_speed;				///-- Slide ��ȭ��

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