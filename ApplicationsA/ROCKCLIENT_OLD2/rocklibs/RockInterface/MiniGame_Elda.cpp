// MiniGame_Elda.cpp: implementation of the CMiniGame_Elda class.
//
//////////////////////////////////////////////////////////////////////

#include "MiniGame_Elda.h"
#include "Rui.h"
#include "..\\..\\VFileSystem.h" 
#include "RockClient.h"
#include	<SOUND\\Sound.H>
#include "..\\..\\StringManager.h"
#include "UiUtil.h"

extern Sound_Manager		g_Sound;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	//#define _DEV_VERSION 
#endif



#define ELDA_CELL_SIZE			35	
#define ELDA_CELL_SIZE_GAP		36	
#define ELDA_CELL_SIZE_SOURCE	55	


#define POS_SOURCE_RENDER_X		117
#define POS_SOURCE_RENDER_Y		63

#define POS_GOAL_RENDER_X		362
#define POS_GOAL_RENDER_Y		213

#define POS_WORK_RENDER_X		58
#define POS_WORK_RENDER_Y		213

#define FRAME_PER_SECOUND		40


int li_output_cells_x;
int li_output_cells_y;
	

bool lb_prev_slide_out = false;
bool lb_prev_slide_in = false;
bool lb_prev_MSG_Ready = false;
bool lb_prev_MSG_Go = false;
bool lb_prev_time_over = false;
bool lb_prev_game_end = false;

int devi_timeslice = 25;
bool devb_pause		=	false;
bool devb_cheat_src =	false;


//bool lb_prev_combo = false;
//bool lb_prev_interj = false;
//bool lb_prev_shd = false;

CFontg*				l_Font;
CFontg*				l_TitleFont = NULL;
CFontg*				l_NFont = NULL;				
extern CFontman*	Fontman ;
extern	CRockClient			g_RockClient;


///---------------------------------------------------------------------------
///-- operator=
///---------------------------------------------------------------------------
ELDA_CELL& ELDA_CELL::operator=(const ELDA_CELL &rhs)
{
	if(this == &rhs)
		return *this;
	
	mi_color = rhs.mi_color;
	mi_sub_color = rhs.mi_sub_color;
	me_type = rhs.me_type;

	return *this;
}

///---------------------------------------------------------------------------
///-- operator==
///---------------------------------------------------------------------------
bool ELDA_CELL::operator==(const ELDA_CELL &rhs) const
{
	if( mi_color == rhs.mi_color && mi_sub_color == rhs.mi_sub_color && me_type == rhs.me_type)
	{
		return true;
	}
	return false;
}

///---------------------------------------------------------------------------
///-- Constructor
///---------------------------------------------------------------------------
CMiniGame_Elda::CMiniGame_Elda()
{
	///-- temp
	theBaseRect.x = 175;
	theBaseRect.y = 115;
	theBaseRect.w = 674;
	theBaseRect.h = 443;
	thebCheatBlock = false;

	_clear();
	Gen_Source_Cells();
	theImgID = TID_ELD_BACKGRIUND_TOP;
	
	pButtonTxt = SAFE_NEW( CTextBox( &(nRui->Desktop) ) );	
	
	memset(m_strTitleName , NULL , MAX_SIZE_256);
}

///---------------------------------------------------------------------------
///-- Destructor
///---------------------------------------------------------------------------
CMiniGame_Elda::~CMiniGame_Elda()
{
	SAFE_DELETE(pButtonTxt);
}
///---------------------------------------------------------------------------
///-- _clear
///---------------------------------------------------------------------------
void CMiniGame_Elda::_clear()
{
	int i,j;
		
	theGameMode = n_M_None;

	theTimeTurn = 0;


	mb_abort_bt_ovr = 0;

	///-- Effect 초기화
	
	lb_prev_game_end = false;
	lb_prev_time_over = false;
	lb_prev_MSG_Ready = false;	
	lb_prev_MSG_Go = false;
	lb_prev_slide_out = false;
	lb_prev_slide_in = false;

	theReady	= false;
	theGo		= false;
	theTimeOver	= false;
	thePerfect	= false;
	theGameEnd	= false;
	theMiss = false;
	
	
	theprev_time  = timeGetTime();
	the_accum_frame = 0;
	the_proc_frame2 =0 ;					///-- 처리해야될 Ani Frame
	the_accum_frame2 =0 ;					///-- 누적되는 frame count
	theTimeTurn =0;
	theTimeTotal =0;
	theScore = 0;
	theMissCount_100 = 0;
	theMissCount_50 = 0;
	theHitCount = 0;
	theFocus_frame  =0 ;

	theSlideCell_out = false;	
	theSlideCell_in  = false;	
	theShadowDrop = false;
	theComboMsg = false;	
	theComboMsg_frame = 0;
	theComboCnt = 0;					///-- 현재 까지 연속된 Combo 의 횟수 
	theComboEnable = true;				///-- 현재 Column 에서 Combo 가능한지 여부
	theInterjection = false;			///-- interjection View

	
	theLimitColor = 0;	
	theTimeTotal = 0;					///-- 전체 게임에 남은 시간.
	theTimeTurn = 0;					///-- 한턴에 남은 시간
	///-- 작업 중인 Col
	theWorkingCol = 0;
	the_proc_frame = 0;		
	theLimitCellsRow = 0;
	theLimitSourceNum = 0;	
	
	///-- Source Cells
	for( i = 0 ; i < ELDA_SOURCE_CELL_I; ++i )
	{
		for( j = 0 ; j < ELDA_SOURCE_CELL_J ; ++j )
		{
			theSourceCells[i][j].mi_color = 0;
			theSourceCells[i][j].mi_sub_color = 0;
			theSourceCells[i][j].me_type = n_ELDA_None;
		}
	}

	for( i = 0 ; i < ELDA_MAX_CELL_I; ++i )		
	{
		for( j = 0 ; j < ELDA_MAX_CELL_J; ++j )		
		{
			///-- Working Cells
			theWorkCells[i][j].mi_color = 0;
			theWorkCells[i][j].mi_sub_color = 0;
			theWorkCells[i][j].me_type = n_ELDA_None;
			
			///-- Goal Cells
			theGoalCells[i][j].mi_color = 0;
			theGoalCells[i][j].mi_sub_color = 0;
			theGoalCells[i][j].me_type = n_ELDA_None;
			
			///-- Effect Cell Hit
			theEff_WorkCells[i][j] = -1;
			theEff_Blinking[i][j] = -1; ///--- TEmp
		}		
	}
	


}

///---------------------------------------------------------------------------
///-- Set_Level
///---------------------------------------------------------------------------
///-- Level 을 설정하고 그에 따른 각종 값 들을 설정한다.
void CMiniGame_Elda::_Set_Level()
{
	///-- Polygon 설정 
	/*if( theLevel < 4 )
	{
		theLimitSourceNum = 6;
	}
	else if( theLevel < 8 )
	{
		theLimitSourceNum = 7;
	}
	else if( theLevel < 11 )
	{
		theLimitSourceNum = 8;
	}
	else if( theLevel < 16 )
	{
		theLimitSourceNum = 9;
	}
	else if( theLevel < 18 )
	{
		theLimitSourceNum = 10;
	}
	else if( theLevel < 20 )
	{
		theLimitSourceNum = 11;
	}
	else 
	{
		theLimitSourceNum = 12;
	}*/
	
}
///---------------------------------------------------------------------------
///-- _EffSet_Shadow
///---------------------------------------------------------------------------
void CMiniGame_Elda::_EffSet_Shadow( const int ai_col )
{
	theShadowDrop = true;
	theShadow_frame = 0;
	
	theShadow_Eff_mov_x = ( (float)rand() / (float)0x7fff) * 4.0f - 2.0f; //-- 0~9까지의 random 값		
	theShadow_Eff_mov_y = -8.0f;

	theShadow_Eff_x = float( theBaseRect.x + POS_GOAL_RENDER_X + ( ai_col + ELDA_MAX_CELL_J - theLimitCellsCol )* ELDA_CELL_SIZE_GAP );	
	theShadow_Eff_y = float( theBaseRect.y + POS_GOAL_RENDER_Y );

	///-- Sound
	g_Sound.Play( 175, 0.0f, 0.0f, 0.0f );	
}

///---------------------------------------------------------------------------
///-- _EffSet_Source_Slide
///---------------------------------------------------------------------------
void CMiniGame_Elda::_EffSet_Source_Slide( )
{
	if( theSlideCell_out || theSlideCell_in )
	{
		return;
	}
	theSlideCell_out = true;
	theSlideCell_in = false;
	theSlide_frame = 0;
	theSlide_speed = -3.0f;

	///-- Sound
	g_Sound.Play( 177, 0.0f, 0.0f, 0.0f );	
}

///---------------------------------------------------------------------------
///-- _EffSet_Ready_Msg
///---------------------------------------------------------------------------
void CMiniGame_Elda::_EffSet_Ready_Msg()
{
	theReady = true;
	lb_prev_MSG_Ready = theReady;	
	theReady_frame = 0;
}
///---------------------------------------------------------------------------
///-- _EffSet_Go_Msg
///---------------------------------------------------------------------------
void CMiniGame_Elda::_EffSet_Go_Msg()
{
	theGo= true;
	lb_prev_MSG_Go = theGo;
	theGo_frame = 0;
}

///---------------------------------------------------------------------------
///-- _EffSet_End_Msg
///---------------------------------------------------------------------------
void CMiniGame_Elda::_EffSet_End_Msg()
{
	theGameEnd= true;
	theGameEnd_frame = 0;
}
///---------------------------------------------------------------------------
///-- _EffSet_Perfect_Msg
///---------------------------------------------------------------------------
void CMiniGame_Elda::_EffSet_Perfect_Msg()
{
	thePerfect= true;
	thePerfect_frame = 0;
}
///---------------------------------------------------------------------------
///-- _EffSet_TimeOver_Msg
///---------------------------------------------------------------------------
void CMiniGame_Elda::_EffSet_TimeOver_Msg()
{
	theTimeOver= true;
	theTimeOver_frame = 0;
}
///---------------------------------------------------------------------------
///-- _EffSet_Combo_Msg
///---------------------------------------------------------------------------
void CMiniGame_Elda::_EffSet_Combo_Msg()
{
	theComboMsg = true;
	theComboMsg_frame = 0;
}
///---------------------------------------------------------------------------
///-- _EffSet_CellHit
///---------------------------------------------------------------------------
void CMiniGame_Elda::_EffSet_CellHit( const int ai_row, const int ai_col )
{
	if ( ELDA_MAX_CELL_I <= ai_row)
	{
		return;
	}
	
	if ( ELDA_MAX_CELL_J <= ai_col)
	{
		return;
	}
	
	///-- Frame Setting
	theEff_WorkCells[ ai_row ][ ai_col ] = 0;

	///-- Play
	g_Sound.Play( 173, 0.0f, 0.0f, 0.0f );	
}
///---------------------------------------------------------------------------
///-- _EffSet_Miss
///---------------------------------------------------------------------------
void CMiniGame_Elda::_EffSet_Miss()
{
	theMiss = true;
	theMiss_frame = 0;

	
	///-- Sound
	g_Sound.Play( 174, 0.0f, 0.0f, 0.0f );
}

///---------------------------------------------------------------------------
///-- _TIDGet_Cell_Blinking
///---------------------------------------------------------------------------
TEXID CMiniGame_Elda::_TIDGet_Cell_Blinking( const ELDA_CELL_TYPE ae_type, const int ai_frame ) const
{
	TEXID l_texid;

	switch( ae_type ) 
	{			
	case n_ELDA_RB:	
		l_texid = TID_ELDEFF_RB_1;	
		break;
	case n_ELDA_LB:
		l_texid = TID_ELDEFF_LB_1;		
		break;
	case n_ELDA_LT:		
		l_texid = TID_ELDEFF_LT_1;
		break;
	case n_ELDA_RT:
		l_texid = TID_ELDEFF_RT_1;
		break;
	default:				
		return 0;	
	}
	
	if(	ai_frame < 25)
	{
		l_texid += (ai_frame / 6 );
	}	
	return l_texid;
}

///---------------------------------------------------------------------------
///-- _GetTid_Cell
///---------------------------------------------------------------------------
///-- 해당 Cell 에 해당되는 TID 를 Return 한다.
TEXID CMiniGame_Elda::_TIDGet_Cell( const ELDA_CELL_TYPE ae_type, const int ai_color_idx ) const
{
	TEXID l_texid;
	switch( ae_type ) 
	{			
	case n_ELDA_RB:	
		l_texid = TID_ELD_RB_1;	
		break;
	case n_ELDA_LB:
		l_texid = TID_ELD_LB_1;		
		break;
	case n_ELDA_LT:		
		l_texid = TID_ELD_LT_1;
		break;
	case n_ELDA_RT:
		l_texid = TID_ELD_RT_1;
		break;
	default:				
		return 0;	
	}
	
	if(	ai_color_idx < 12)
	{
		l_texid += ai_color_idx;
	}	
	return l_texid;
}
///---------------------------------------------------------------------------
///-- _TIDGet_CellHit
///---------------------------------------------------------------------------
TEXID CMiniGame_Elda::_TIDGet_CellHit( const char ac_type ) const
{
	switch( ac_type  ) 
	{			
	case n_ELDA_RB:	
		return TID_ELD_EFF_HIT_RB;		
	
	case n_ELDA_LB:
		return TID_ELD_EFF_HIT_LB;	
			
	case n_ELDA_BoxRL:
	case n_ELDA_LT:		
		return TID_ELD_EFF_HIT_LT;

	case n_ELDA_BoxLR:	
	case n_ELDA_RT:
		return TID_ELD_EFF_HIT_RT;
	
	default:
		return 0;	
	}

}
///---------------------------------------------------------------------------
///-- _ColorGet_Idx
///---------------------------------------------------------------------------
///-- 해당 Index 의 Color 값을 반환한다.
D3DCOLOR CMiniGame_Elda::_ColorGet_Idx( const int ai_color_idx ) const
{
	D3DCOLOR li_color;

	///-- Color Setting
	switch( ai_color_idx )
	{
		///-- RED
	case 0 : li_color = D3DCOLOR_ARGB( 100, 208, 5, 55 );
		break;
		///-- OrangeRED
	case 1 : li_color = D3DCOLOR_ARGB( 100, 210, 80, 5);
		break;
		///-- YELLOW
	case 2 : li_color = D3DCOLOR_ARGB( 100, 210, 190, 20);
		break;
		///-- BLACK
	case 3 : li_color = D3DCOLOR_ARGB( 100, 50, 50, 50 );
		break;
		///-- GREEN
	case 4 : li_color = D3DCOLOR_ARGB( 100, 108, 209 , 8 );
		break;
		///-- DARK GREEN
	case 5 : li_color = D3DCOLOR_ARGB( 100, 0, 179, 78);
		break;
		///-- NAVY
	case 6 : li_color = D3DCOLOR_ARGB( 100, 0, 141, 217);
		break;
		///-- Gray
	case 7 : li_color = D3DCOLOR_ARGB( 100, 90, 90, 90);
		break;
		///--Gray
	case 8 : li_color = D3DCOLOR_ARGB( 100, 5, 11, 201);
		break;
		///-- Black
	case 9 : li_color = D3DCOLOR_ARGB( 100, 135, 5, 200 );
		break;
		///-- light blue
	case 10 : li_color = D3DCOLOR_ARGB( 100, 208, 0, 180 );
		break;
		///-- purple
	case 11 : li_color = D3DCOLOR_ARGB( 100, 34, 117, 121);
		break;		
		///-- BLACK : 완성된 컬러
	case 12 : li_color = 0xffE0EEEE;
		break;
		///-- WHITE : 나와선 안되는 컬러
	default : li_color = 0xffff00ff;
		break;
	}
	return li_color;
}

///---------------------------------------------------------------------------
///-- _Render_Eff_Blinking_Cells
///---------------------------------------------------------------------------
void CMiniGame_Elda::_Render_Eff_Blinking_Cells()
{
	int i,j;
	int v;
	
	TEXID l_tid;
	
	for( i = 0 ; i < ELDA_MAX_CELL_I; ++i )
	{
		for( j = 0 ; j < ELDA_MAX_CELL_J ; ++j )
		{
			///-- Frame 있으며 Cell 이 있다면 Render
			if( theEff_Blinking[i][j] >0 && theWorkCells[i][j].me_type != n_ELDA_None )
			{
				li_output_cells_x = theBaseRect.x + POS_WORK_RENDER_X + ( j + ELDA_MAX_CELL_J - theLimitCellsCol ) * ELDA_CELL_SIZE_GAP;
				li_output_cells_y = theBaseRect.y + POS_WORK_RENDER_Y + i * ELDA_CELL_SIZE_GAP;				
				
				if( theWorkCells[i][j].me_type == n_ELDA_BoxRL)
				{
					l_tid = _TIDGet_Cell_Blinking( n_ELDA_LT , theEff_Blinking[i][j] );
					Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE , ELDA_CELL_SIZE , D3DCOLOR_ARGB(255, 255, 255, 255) );			
					
					l_tid = _TIDGet_Cell_Blinking( n_ELDA_RB , theEff_Blinking[i][j] );
					Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE , ELDA_CELL_SIZE , D3DCOLOR_ARGB(255, 255, 255, 255) );			
				}
				else if( theWorkCells[i][j].me_type == n_ELDA_BoxLR)
				{
					l_tid = _TIDGet_Cell_Blinking( n_ELDA_RT , theEff_Blinking[i][j] );
					Render->Draw_CloneImg( l_tid, theImgID,  li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE , ELDA_CELL_SIZE , D3DCOLOR_ARGB(255, 255, 255, 255) );			
					
					l_tid = _TIDGet_Cell_Blinking( n_ELDA_LB , theEff_Blinking[i][j] );
					Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE , ELDA_CELL_SIZE , D3DCOLOR_ARGB(255, 255, 255, 255) );			
				}
				else
				{
					l_tid = _TIDGet_Cell_Blinking( ELDA_CELL_TYPE( theWorkCells[i][j].me_type ) , theEff_Blinking[i][j] );
					Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE , ELDA_CELL_SIZE , D3DCOLOR_ARGB(255, 255, 255, 255) );			
				}
				

				++theEff_Blinking[i][j];

				///-- Frame 종료
				if( theEff_Blinking[i][j] >24)
				{
					theEff_Blinking[i][j] = -1;
				}
			}
		}
	}

	///-----------------------------------------------------------------------
	///-- Blinking Setting

	i = int(( (float)rand() / (float)0x7fff) *ELDA_MAX_CELL_I) ; //-- 0~9까지의 random 값
	j = int(( (float)rand() / (float)0x7fff) *ELDA_MAX_CELL_J) ; //-- 0~9까지의 random 값
	v = int(( (float)rand() / (float)0x7fff) *10) ; //-- 0~4까지의 random 값
	if( theEff_Blinking[i][j] -1 && v == 1)
	{
		theEff_Blinking[i][j] = 1;
	}


}
///---------------------------------------------------------------------------
///-- _Render_Eff_Miss
///---------------------------------------------------------------------------
void CMiniGame_Elda::_Render_Eff_Miss()
{
	int MAX_FRAME_MISS = 25;
	int l_alpha;	
	int  l_size = 0;
	int li_frame = the_proc_frame;

	if( theMiss )
	{		
		///-- Frame 종료
		if( theMiss_frame + li_frame > MAX_FRAME_MISS )
		{
			li_frame  = MAX_FRAME_MISS - theMiss_frame;
			theMiss = false;
		}
		
		///-- Frame 진행
		theMiss_frame += li_frame;
		
		///-----------------------------------------------------------------------
		///-- Alpha 실제 Render 되는 시간과 직접적으로 연관된다.
		l_alpha = (25 - theMiss_frame) * 15;
		if( l_alpha > 255) l_alpha = 255;
		
		
		li_output_cells_x = theBaseRect.x + 280;
		li_output_cells_y = theBaseRect.y + 200;
		Render->Draw( TID_MSG_Miss, li_output_cells_x, li_output_cells_y, 
			113, 50, D3DCOLOR_ARGB( l_alpha, 255, 255, 255) );	
	}	
}


///---------------------------------------------------------------------------
///-- _Render_Eff_MSGGo
///---------------------------------------------------------------------------
void CMiniGame_Elda::_Render_Eff_MSGGo()
{
	int l_alpha;	
	int  l_size = 0;

	if( theGo )
	{		
		///-- Frame 진행
		theGo_frame += the_proc_frame;
		
		///-- Frame 종료
		if( theGo_frame > 20)
		{
			theGo = false;
		}
		
		///-----------------------------------------------------------------------
		///-- Alpha 실제 Render 되는 시간과 직접적으로 연관된다.
		l_alpha = (31 - theGo_frame) * 25;
		if( l_alpha > 255) l_alpha = 255;
		
		li_output_cells_x = theBaseRect.x + 270;
		li_output_cells_y = theBaseRect.y + 90;
		Render->Draw( TID_MSG_GO,  li_output_cells_x, li_output_cells_y, 
			150, 50, D3DCOLOR_ARGB( l_alpha, 255, 255, 255) );	
	}	
}


///---------------------------------------------------------------------------
///-- _Render_Eff_Miss
///---------------------------------------------------------------------------
void CMiniGame_Elda::_Render_Eff_MSGEnd()
{
	int l_alpha;	
	int  l_size = 0;

	if( theGameEnd )
	{		
		///-- Frame 진행
		theGameEnd_frame += the_proc_frame;
		
		///-- Frame 종료
		if( theGameEnd_frame > 80)
		{
			theGameEnd = false;
			theGameEnd_frame = 80;
		}
		
		///-- Alpha 실제 Render 되는 시간과 직접적으로 연관된다.
		if( theGameEnd_frame < 40)
		{
			l_alpha = theGameEnd_frame * 10;
			if( l_alpha > 255) l_alpha = 255;
		}
		else
		{
			l_alpha = ( 80 - theGameEnd_frame ) * 10;
			if( l_alpha > 255) l_alpha = 255;
			if( l_alpha < 0) l_alpha = 0;			
		}
		
		li_output_cells_x = theBaseRect.x + 130;
		li_output_cells_y = theBaseRect.y + 80;
		Render->Draw( TID_MSG_MISSION,  li_output_cells_x, li_output_cells_y, 
			183, 50, D3DCOLOR_ARGB( l_alpha, 255, 255, 255) );	

		Render->Draw( TID_MSG_SUCCESS,  li_output_cells_x + 220, li_output_cells_y, 
			200, 50, D3DCOLOR_ARGB( l_alpha, 255, 255, 255) );	
	}	
}

///---------------------------------------------------------------------------
///-- _Render_Eff_Miss
///---------------------------------------------------------------------------
void CMiniGame_Elda::_Render_Eff_MSGTimeOver()
{
	int l_alpha;	
	int  l_size = 0;

	if( theTimeOver )
	{		
		///-- Frame 진행
		theTimeOver_frame += the_proc_frame;
		
		///-- Frame 종료
		if( theTimeOver_frame > 80)
		{
			theTimeOver = false;
		}
		
		///-- Alpha 실제 Render 되는 시간과 직접적으로 연관된다.
		if( theTimeOver_frame < 40)
		{
			l_alpha = theTimeOver_frame * 10;
			if( l_alpha > 255) l_alpha = 255;
		}
		else
		{
			l_alpha = ( 80 - theTimeOver_frame ) * 10;
			if( l_alpha > 255) l_alpha = 255;
			if( l_alpha < 0) l_alpha = 0;			
		}
		
		li_output_cells_x = theBaseRect.x + 220;
		li_output_cells_y = theBaseRect.y + 90;
		Render->Draw( TID_MSG_TIMEOVER, li_output_cells_x, li_output_cells_y, 
			239, 50, D3DCOLOR_ARGB( l_alpha, 255, 255, 255) );	
	}	
}

///---------------------------------------------------------------------------
///-- _Render_Eff_Miss
///---------------------------------------------------------------------------
void CMiniGame_Elda::_Render_Eff_MSGReady()
{
	int l_alpha;	
	int  l_size = 0;

	if( theReady )
	{		
		///-- Frame 진행
		theReady_frame += the_proc_frame;
		
		///-- Frame 종료
		if( theReady_frame > 120)
		{
			theReady_frame = 120;
			theReady = false;
		}
		
		///-- Alpha 실제 Render 되는 시간과 직접적으로 연관된다.
		if( theReady_frame < 60)
		{
			l_alpha = theReady_frame * 5;
			if( l_alpha > 255) l_alpha = 255;
		}
		else
		{
			l_alpha = ( 120 - theReady_frame ) * 5;
			if( l_alpha > 255) l_alpha = 255;
			if( l_alpha < 0) l_alpha = 0;			
		}
		
		
		
		li_output_cells_x = theBaseRect.x + 255;
		li_output_cells_y = theBaseRect.y + 90;
		Render->Draw( TID_MSG_READY, li_output_cells_x, li_output_cells_y, 
			155, 50, D3DCOLOR_ARGB( l_alpha, 255, 255, 255) );	
	}	
}

///---------------------------------------------------------------------------
///-- _Render_Eff_ComboMsg
///---------------------------------------------------------------------------
void CMiniGame_Elda::_Render_Eff_ComboMsg()
{	
	int l_alpha;
	float l_size = float( theComboMsg_frame )  * 3.5f;
	TEXID l_tid;
	///l_size = 0;
	
	if( theComboMsg )
	{		
		///-- Frame 진행
		theComboMsg_frame += the_proc_frame;

		///-- Frame 종료
		if( theComboMsg_frame > 60)
		{
			theComboMsg_frame = 60;
			theComboMsg = false;
		}

		///-----------------------------------------------------------------------
		///-- Alpha 실제 Render 되는 시간과 직접적으로 연관된다.
		l_alpha = int ( (35 - theComboMsg_frame) * 12.5 );
		if( l_alpha < 0 ) l_alpha = 0;
		if( l_alpha > 255 ) l_alpha = 255;
		
		///-------------------------------------------------------------------
		///-- Combo Count
		if( theComboCnt>= 2)
		{
			li_output_cells_x = theBaseRect.x + 250;
			li_output_cells_y = theBaseRect.y + 300;
			
			switch( theComboCnt )
			{
			case 2: l_tid = TID_Number_2; 
				break;
			case 3: l_tid = TID_Number_3; 
				break;
			case 4: l_tid = TID_Number_4; 
				break;
			case 5: l_tid = TID_Number_5; 
				break;
			case 6: l_tid = TID_Number_6; 
				break;
			case 7: l_tid = TID_Number_7; 
				break;				
			}
		
			Render->Draw( l_tid, li_output_cells_x, li_output_cells_y, 
				40, 55, D3DCOLOR_ARGB( l_alpha, 255, 255, 255) );			
			/*Render->Draw_CloneImg( l_tid, li_output_cells_x - l_size/2, li_output_cells_y - l_size/2, 
				ELDA_CELL_SIZE + l_size, ELDA_CELL_SIZE + l_size , D3DCOLOR_ARGB( l_alpha, 255, 255, 255) );			*/
		}
		
		///-------------------------------------------------------------------
		///-- Combo String
		if( theComboCnt )
		{
			
			li_output_cells_y = theBaseRect.y + 300;
			

			switch( theComboCnt )
			{
			case 1: ///-- Good
				li_output_cells_x = theBaseRect.x + 270;
				l_tid = TID_MSG_Good; 
				break;
				
			default: ///-- Combo
				li_output_cells_x = theBaseRect.x + 300;
				l_tid = TID_MSG_Combo; 
				break;
			}
			
			Render->Draw( l_tid, li_output_cells_x, li_output_cells_y, 
				147, 50, D3DCOLOR_ARGB( l_alpha, 255, 255, 255) );			
		}
		///-------------------------------------------------------------------
		///-- Message		
		if( theComboCnt>= 4)
		{
			
			li_output_cells_x = int( theBaseRect.x + 250.0f  - l_size*4 );
			li_output_cells_y = int( theBaseRect.y + 200.0f  - l_size/2 );
			switch( theComboCnt )
			{
			default: ///-- Combo
			case 4: ///-- Great!
				l_tid = TID_MSG_Great; 
				break;
			case 5: ///-- Excellent!
				l_tid = TID_MSG_Excellent; 
				break;
			case 6: ///-- Slendid!
				l_tid = TID_MSG_Slendid; 
				break;
			case 7: ///-- Superb!
				l_tid = TID_MSG_Superb; 
				break;
			}
			Render->Draw( l_tid, li_output_cells_x, li_output_cells_y , 
				int(200 + l_size * 8) , int (34 + l_size ), D3DCOLOR_ARGB( l_alpha, 255, 255, 255) );			
		}

	}
}
///---------------------------------------------------------------------------
///-- _Render_Eff_Shadow
///---------------------------------------------------------------------------
///-- Shadow Hit 과 Drop 을 Ani
int MAX_FRAME_SHADOW = 30;
void CMiniGame_Elda::_Render_Eff_Shadow()
{
	int i;
	int l_alpha = 255;		
	int l_size;
	int li_shd_size;
	int li_frame = the_proc_frame;
	
	if( theShadowDrop )
	{

		///-- Check Frame 
		if( theShadow_frame + li_frame > MAX_FRAME_SHADOW )
		{
			li_frame = MAX_FRAME_SHADOW - theShadow_frame;	///-- Frame 초과 방지			
			theShadowDrop = false;
		}
		///-- Frame 진행
		theShadow_frame += li_frame;		
	
		
		///-------------------------------------------------------------------
		///-- Shadow Drop
		///-- Alpha
		if( theShadow_frame > 10 )
		{
			l_alpha = int ( ( 30 - theShadow_Eff_mov_y ) * 25 );
			if( l_alpha < 0 ) l_alpha = 0;
			if( l_alpha > 255 ) l_alpha = 255;
		}
		
		///-- cells
		for( i = 0 ; i < theLimitCellsRow; ++i )			
		{	
			Render->Draw_CloneImg( TID_ELD_SHADOW, theImgID, int (theShadow_Eff_x), int(theShadow_Eff_y + i * ELDA_CELL_SIZE_GAP), ELDA_CELL_SIZE, ELDA_CELL_SIZE, D3DCOLOR_ARGB( l_alpha, 255, 255, 255 ) );			
		}
		
		///-- Move
		theShadow_Eff_x += theShadow_Eff_mov_x * li_frame;
		theShadow_Eff_y += theShadow_Eff_mov_y * li_frame;		
		theShadow_Eff_mov_y += 2.0f * li_frame;
		

		///-------------------------------------------------------------------
		///-- Shadow Hit
		
		
		///-- Alpha 실제 Render 되는 시간과 직접적으로 연관된다.
		l_alpha = (10 - theShadow_frame) * 25;
		if( l_alpha < 0 ) l_alpha = 0;
		
		l_size =  theShadow_frame * 5;
		li_shd_size = theLimitCellsRow * ELDA_CELL_SIZE_GAP;

		Render->Draw( TID_ELD_SHADOWHIT, 
			theBaseRect.x + POS_GOAL_RENDER_X + ( theWorkingCol + ELDA_MAX_CELL_J - theLimitCellsCol ) * ELDA_CELL_SIZE_GAP - l_size /2,		///-- x
			theBaseRect.y + POS_GOAL_RENDER_Y - ( l_size * theLimitCellsRow ) /2,						///-- y
			ELDA_CELL_SIZE + l_size, 
			li_shd_size + ( l_size * theLimitCellsRow), D3DCOLOR_ARGB( l_alpha, 255, 255, 255 ) );			
		
		
	}
}

///---------------------------------------------------------------------------
///-- _Render_Eff_Cell_Hit
///---------------------------------------------------------------------------
///-- Cell 이 Working 영역에 추가 될 때의 Effect
int MAX_FRAME_CELL_HIT = 40;
void CMiniGame_Elda::_Render_Eff_Cell_Hit()
{
	int i,j;
	TEXID l_tid;
	int l_alpha;
	int l_size ;
	int li_frame = the_proc_frame;
	
	///-- 모든 Cell Access
	for( i = 0 ; i < theLimitCellsRow ; ++i )
	{
		for( j = 0 ; j < theLimitCellsCol ; ++j )
		{
			if( theEff_WorkCells[i][j] >=0 )
			{
				///-----------------------------------------------------------------------
				///-- Alpha 실제 Render 되는 시간과 직접적으로 연관된다.
				l_alpha = ( 10 - theEff_WorkCells[i][j] ) * 35;
				if( l_alpha < 0 ) l_alpha = 0;
				if( l_alpha > 255 ) l_alpha = 255;
				
				///-- Check Frame
				if( theEff_WorkCells[i][j] + li_frame > MAX_FRAME_CELL_HIT )
				{
					li_frame = MAX_FRAME_CELL_HIT - theEff_WorkCells[i][j];		///-- Frame 초과 방지
					theEff_WorkCells[i][j] = -1;
				}
				else
				{
					///-- Frame 진행
					theEff_WorkCells[i][j] += li_frame;
					
					l_tid = _TIDGet_CellHit( theWorkCells[i][j].me_type );
					
					l_size = theEff_WorkCells[i][j] * 10;
					
					li_output_cells_x = theBaseRect.x + POS_WORK_RENDER_X + ( j +ELDA_MAX_CELL_J - theLimitCellsCol )  * ELDA_CELL_SIZE_GAP;			
					li_output_cells_y = theBaseRect.y + POS_WORK_RENDER_Y + i * ELDA_CELL_SIZE_GAP;
					
					li_output_cells_x -= l_size/ 2;
					li_output_cells_y -= l_size/ 2;
					
					switch( theWorkCells[i][j].me_type )
					{
					case n_ELDA_RT:		
					case n_ELDA_BoxLR:
						li_output_cells_x  -= l_size/ 5;
						li_output_cells_y += l_size/ 5;
						break;
						
					case n_ELDA_RB:
						li_output_cells_x  -= l_size/ 5;
						li_output_cells_y -= l_size/ 5;
						break;
						
					case n_ELDA_LT:
					case n_ELDA_BoxRL:
						li_output_cells_x  += l_size/ 5;
						li_output_cells_y += l_size/ 5;
						break;
						
					case n_ELDA_LB:
						li_output_cells_x  += l_size/ 5;
						li_output_cells_y -= l_size/ 5;
						break;
					}
					
					Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE + l_size, ELDA_CELL_SIZE + l_size, D3DCOLOR_ARGB( l_alpha,255,255,255));			
				}
			
			}
		}
	}
	
}
///---------------------------------------------------------------------------
///-- _Render_Eff_Source_Slide
///---------------------------------------------------------------------------
///-- 위치가 결정
#define MAX_FRAME_SLIDE_IN  30
#define MAX_FRAME_SLIDE_OUT 30

void CMiniGame_Elda::_Render_Eff_Source_Slide()
{
	int i, j;
	int li_frame = the_proc_frame;
	int slide_width;
		
	///-- 사라질 때
	if( theSlideCell_out )
	{	
		///-- Check Frame 
		if( theSlide_frame + li_frame > MAX_FRAME_SLIDE_OUT  )
		{			
			li_frame = MAX_FRAME_SLIDE_OUT  - theSlide_frame; ///-- Frame 초과 방지
			theSlideCell_out = false;
			if( theGameMode != n_M_End)
			{
				g_Sound.Play( 177, 0.0f, 0.0f, 0.0f );	
				theSlideCell_in = true;
			}
			theSlide_frame = 0;			
			theSlide_x = float( theBaseRect.x + 600 );	
			return;
		}
		///-- Frame 진행
		theSlide_frame += li_frame; 

		///-- 위치 결정
		theSlide_speed = -3.0f;		
		theSlide_x = 200 ;///+ ( - theLimitSourceNum * 10 );
		slide_width = 10;

		for( i = 0 ; i < theSlide_frame; ++i )
		{
			slide_width +=1;
			theSlide_speed -= 1.0f;
			theSlide_x += theSlide_speed;
		}
	}
	///-- 나타날 때
	else if( theSlideCell_in ) 
	{	
		
		///-- Check Frame 
		if( theSlide_frame + li_frame > MAX_FRAME_SLIDE_IN )
		{
			li_frame = MAX_FRAME_SLIDE_IN  - theSlide_frame; ///-- Frame 초과 방지
			theSlideCell_in = false;
			///return;
		}
		///-- Frame 진행
		theSlide_frame += li_frame; 

		///-- 위치 결정
		theSlide_speed = 3.0f;		
		theSlide_x = 200 ;
		slide_width = 10;
		
		for( i = theSlide_frame ; i <= MAX_FRAME_SLIDE_IN; ++i )
		{
			theSlide_speed += 1.0f;
			slide_width += 1;
			theSlide_x += theSlide_speed;
		}
	}	

	slide_width *= 4;
	
	///-- render
	if( theSlideCell_out || theSlideCell_in )
	{		
		///-- Slide Source Cells 
		for( i = 0 ; i < ELDA_SOURCE_CELL_I ; ++i )
		{
			for( j = 0 ; j < ELDA_SOURCE_CELL_J ; ++j )
			{							
				if( theSourceCells[i][j].me_type != n_ELDA_None )
				{					
					
					li_output_cells_x = int (theSlide_x + theSourceCellsPos[i][j].x );
					li_output_cells_y = theSourceCellsPos[i][j].y;
					
					///-- Mask 바깥에 Draw 금지
					if( li_output_cells_x > theBaseRect.x +20 &&  li_output_cells_x < theBaseRect.x + 570 )
					{	
						Render->Draw_CloneImg( TID_ELD_EFF_SLIDE, theImgID, li_output_cells_x, li_output_cells_y, slide_width, ELDA_CELL_SIZE , _ColorGet_Idx( theSourceCells[i][j].mi_color ) );			
					}
					
				}
			}
		}
	}

}
///---------------------------------------------------------------------------
///-- _Render_Cell
///---------------------------------------------------------------------------
void CMiniGame_Elda::_Render_Cell( const ELDA_CELL * const ac_cell , const DWORD ai_color) 
{
	TEXID l_tid;
	
	///-- n_ELDA_BoxRL : LT ( 1 ) , RB ( 2 )	
	if( ac_cell->me_type == n_ELDA_BoxRL )
	{
		l_tid = _TIDGet_Cell( n_ELDA_LT, ac_cell->mi_sub_color );
		Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE , ELDA_CELL_SIZE , ai_color);			
				
		l_tid = _TIDGet_Cell( n_ELDA_RB, ac_cell->mi_color );
		Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE , ELDA_CELL_SIZE , ai_color );			
	}
	///-- n_ELDA_BoxLR : LB ( 3 ) , RT ( 4 )
	else if( ac_cell->me_type == n_ELDA_BoxLR )
	{
		l_tid = _TIDGet_Cell( n_ELDA_RT, ac_cell->mi_sub_color );
		Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE , ELDA_CELL_SIZE , ai_color );			
				
		l_tid = _TIDGet_Cell( n_ELDA_LB, ac_cell->mi_color );
		Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE , ELDA_CELL_SIZE , ai_color );			
	}
	///-- Single Polygon
	else if( ac_cell->me_type != n_ELDA_None )
	{				
		l_tid = _TIDGet_Cell( ELDA_CELL_TYPE( ac_cell->me_type ), ac_cell->mi_color );
		Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y, ELDA_CELL_SIZE , ELDA_CELL_SIZE , ai_color );			
	}	
}
///---------------------------------------------------------------------------
///-- _Render_Goal_Focus
///---------------------------------------------------------------------------
///-- 현재 조합해야 되는 Cell 을 표시한다.
void CMiniGame_Elda::_Render_Goal_Focus()
{
	int i ;
	int li_hit = theHitCount ;
	TEXID l_tid;
	int l_size = 0;
	int l_alpha;
	
	theFocus_frame += the_proc_frame;

	if( theFocus_frame  > 80 )
	{
		theFocus_frame =0;
	}
	if( theFocus_frame<40 )
	{
		l_alpha = ( 40 - theFocus_frame ) * 6;
	}
	else
	{
		l_alpha = ( theFocus_frame - 40 ) * 6;
	}
	
	


	// 워킹이 타입이 암것두 없으면 매인 컬러 
	// 타입이 있으면 서브 컬러 .. 출력. . 


	for( i = ELDA_MAX_CELL_I -1 ; i >= 0; --i )
	{
		if( theGoalCells[i][theWorkingCol].me_type != theWorkCells[i][theWorkingCol].me_type )
		{
			///-- Position
			li_output_cells_x = theBaseRect.x + POS_GOAL_RENDER_X + ( theWorkingCol +ELDA_MAX_CELL_J - theLimitCellsCol )  * ELDA_CELL_SIZE_GAP - 6;			
			li_output_cells_y = theBaseRect.y + POS_GOAL_RENDER_Y + i * ELDA_CELL_SIZE_GAP -6;
			
			
			if( theGoalCells[i][theWorkingCol].me_type == n_ELDA_BoxLR)
			{
				m_type = n_ELDA_BoxLR;

				///-- LB
				if( theWorkCells[i][theWorkingCol].me_type == n_ELDA_None )
				{
					m_type = n_ELDA_LB;
					m_color = theGoalCells[i][theWorkingCol].mi_color;
					l_tid = TID_ELD_EFF_HIT_LB ;
				}
				///-- RT
				else
				{
					m_type = n_ELDA_RT;
					m_color = theGoalCells[i][theWorkingCol].mi_sub_color;
					l_tid = TID_ELD_EFF_HIT_RT ;
				}
				
			}
			else if( theGoalCells[i][theWorkingCol].me_type == n_ELDA_BoxRL)
			{	
				m_type = n_ELDA_BoxRL;
				///-- RB
				if( theWorkCells[i][theWorkingCol].me_type == n_ELDA_None )
				{
					m_type = n_ELDA_RB;
					m_color = theGoalCells[i][theWorkingCol].mi_color;
					l_tid = TID_ELD_EFF_HIT_RB ;					
				}
				///-- LT
				else
				{
					m_type = n_ELDA_LT;
					m_color = theGoalCells[i][theWorkingCol].mi_sub_color;
					l_tid = TID_ELD_EFF_HIT_LT;				
				}
				
			}
			else
			{
				l_tid = _TIDGet_CellHit ( theGoalCells[i][theWorkingCol].me_type );				
			}

			///-- 시각적 보정
			if( l_tid == TID_ELD_EFF_HIT_RB )
			{
				li_output_cells_x += 2;
			}
	



			Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x+1, li_output_cells_y, 45 + l_size, 45 + l_size, D3DCOLOR_ARGB( l_alpha,255,255,255));			
			Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x-1, li_output_cells_y, 45 + l_size, 45 + l_size, D3DCOLOR_ARGB( l_alpha,255,255,255));			
			Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y+1, 45 + l_size, 45 + l_size, D3DCOLOR_ARGB( l_alpha,255,255,255));			
			Render->Draw_CloneImg( l_tid, theImgID, li_output_cells_x, li_output_cells_y-1, 45 + l_size, 45 + l_size, D3DCOLOR_ARGB( l_alpha,255,255,255));			
		


			return;
			
		}
		
		
		
		
	}
	
	
	
}
///---------------------------------------------------------------------------
///-- _SetMovement
///---------------------------------------------------------------------------
void CMiniGame_Elda::_SetMovement()
{	
	int i,j;
	int movment;
	
	LONG lx = 0;
	LONG ly = 0;

	if( theLevel > 15 )
	{		
		movment = theLevel - 10;
		for( i = 0 ; i < ELDA_SOURCE_CELL_I; ++i )						
		{
			for( j = 0 ; j < ELDA_SOURCE_CELL_J; ++j )
			{				
				//lx = int(( (float)rand() / (float)0x7fff) * 5 );
				//ly = int(( (float)rand() / (float)0x7fff) * 5 );

				//lx -= 2;
				//ly -= 2;

				lx = (LONG)RandomNum(-1, 1);
				ly = (LONG)RandomNum(-1, 1);
				
				theSource_Mov[i][j].x = lx; //-- random 값
				theSource_Mov[i][j].y = ly; //-- random 값
			}
		}
	}
}
///---------------------------------------------------------------------------
///-- Logic_Flow_Manager
///---------------------------------------------------------------------------
///-- 각 상황과 Animation 타이밍이 연계되어 변화되는 것을 처리해줌.
///-- Render 에서  Logic 관련을 제어 하는 부분을 모두 제거해줌
///-- 각 Animation 의 bool 값을 참조한다.
void CMiniGame_Elda::Logic_Flow_Manager()
{
	
	///-- ReadyMsg 가 끝나면 GO Msg 와 함께 Game 시작 
	if (lb_prev_MSG_Ready && !theReady)
	{
		_EffSet_Source_Slide();
		_EffSet_Go_Msg();	
		 theGameMode = n_M_On_GAME;
	}

	///-- Slide Out 이 끝나면 Source Cells 를 Regen 한다.
	if( lb_prev_slide_out && !theSlideCell_out)
	{	
		Gen_Source_Cells();
		theTimeTurn = 0;
	}

	///-- Slide In 이 끝나면 Time Check 를 시작한다.
	///-- Level 에 따라 이동량을 정한다.
	if( lb_prev_slide_in && !theSlideCell_in)
	{		
		theTimeTurn = 0;
		_SetMovement();
	}
	
	///-- Game 종료
	///-- n_M_Destroy 가 Set 되면 외부에서 Destroy 한다.
	if( ( lb_prev_time_over && !theTimeOver ) ||
		( lb_prev_game_end && !theGameEnd ) ) 
	{
		theGameMode = n_M_Destroy;
		Resource->DestroyDynamic( TID_ELD_BACKGRIUND_TOP );	
		Resource->DestroyDynamic( TID_Number_2 );	
		Resource->DestroyDynamic( TID_MSG_MISSION );	
		Resource->DestroyDynamic( TID_ELD_SHADOWHIT );	
		Resource->DestroyDynamic( TID_MINIGAMETEMP_3 );	
		Resource->DestroyDynamic( TID_MINIGAMETEMP_2 );	
	}

	///-- Update
	lb_prev_game_end = theGameEnd;
	lb_prev_time_over = theTimeOver;
	lb_prev_MSG_Ready = theReady;
	lb_prev_MSG_Go = theGo;
	lb_prev_slide_out = theSlideCell_out;
	lb_prev_slide_in = theSlideCell_in;
}

///---------------------------------------------------------------------------
///-- Logic_Pick_Source_Cell
///---------------------------------------------------------------------------
///-- Source 영역의 Cell이 선택되었을 경우 처리
///-- 무결성 검사
///-- Source 영역의 Cell 제거 ( Effect 발생 )
///-- 작업 영역에 Cell 추가가능한지 검사 
///-- 추가 ( Effect 발생 )
///-- 줄 단위 검사

void CMiniGame_Elda::Logic_Pick_Source_Cell( const int ai_row, const int ai_col )
{
	int li_add_row;

	///-- Game Mode Check
	if ( theGameMode != n_M_On_GAME || theSlideCell_in || theSlideCell_out )
	{
		return;
	}

	///-- 무결성 검사
	if( ai_row < 0 || ai_row >= ELDA_SOURCE_CELL_I || ai_col < 0 || ai_col >= ELDA_SOURCE_CELL_J)
	{
		return ; 
	}
	
	///-----------------------------------------------------------------------
	///-- Working 영역 Cell 추가 
	if( theSourceCells[ai_row][ai_col].me_type == n_ELDA_None )
	{
		return;
	}

	///-- 추가
	li_add_row = Add_WorkingCell( &theSourceCells[ai_row][ai_col] );	
	
	///-- Cell 추가 불가. Combo 초기화
	if( li_add_row == -1) 
	{
		_Process_Miss();		
		_EffSet_Miss();
	
	}
	///-- Cell 추가 . Effect
	else
	{
		
		_Process_Hit();		
		_EffSet_CellHit( li_add_row, theWorkingCol );		
	}
	

	///-----------------------------------------------------------------------
	///-- Source 영역에서 제거 ( Effect )
	theSourceCells[ai_row][ai_col].me_type = n_ELDA_None;


	///-----------------------------------------------------------------------
	///-- Working Column 이 완성되었을 때의 처리
	if( Check_Column_Complete() )
	{

		///-- Combo Effect
		if( theComboEnable )
		{

			theTimeTurn=0;					//콤보 성공시 시간 바로 정지 by.jjplusi011

			++theComboCnt ;
			_EffSet_Combo_Msg();
		}
		///-- Column Complete
		_Process_Complete_Column();
		
		///-- Set Source Slide
		_EffSet_Source_Slide();
		
		///-- Set Shadow Animation
		if( theWorkingCol < theLimitCellsCol-1)
		_EffSet_Shadow( theWorkingCol+1 );

		///Shadow_Increase();
		
		///-- Complete 검사
		if( ++theWorkingCol >= theLimitCellsCol ) 
		{	
			///-- Game Complete
			
			_Process_Complete_Total();
		}

		
	}	

}
///---------------------------------------------------------------------------
///-- Prepare_Game
///---------------------------------------------------------------------------
///-- Game 시작 전 준비 수행 한번만 수행된다.
void CMiniGame_Elda::Logic_Prepare_Game( const int ai_pmap_index , const int ai_game_type, 
										 BYTE theGameTimeRatio )
{
	switch( ai_game_type ) 	
	{
		default:
	case 0:///-- Elda Craft
		theImgID = TID_ELD_BACKGRIUND_TOP;
		Rstrcpy( RWCHAR(m_strTitleName), G_STRING_CHAR( IDS_ITEM_REMODEL ) );					
		break;
	case 1:///-- Weapon Craft
		theImgID = TID_MINIGAMETEMP_2;
		Rstrcpy( RWCHAR(m_strTitleName), G_STRING_CHAR( IDS_WEAPON_MAKE ) );				
		break;
	case 2:///-- Armor Craft
		theImgID = TID_MINIGAMETEMP_3;
		Rstrcpy( RWCHAR(m_strTitleName), G_STRING_CHAR( IDS_ARMOR_MAKE ) );					
		break;	
	}

	///-- 초기화
	_clear();

	///-- 목표 영역 생성
	Gen_Goal_Cells( ai_pmap_index , theGameTimeRatio);
	
	//_Set_Level();	

	///-- Game 시작
	theGameMode = n_M_On_Ready;

	///-- Ready 출력
	_EffSet_Ready_Msg();
	
	pButtonTxt->CreateWnd( n_wtTextBox, WID_None, WID_None, 
		theBaseRect.x + 524, theBaseRect.y + 404, 73, 22, false );
	pButtonTxt->SetAlignText(n_atCenter, n_atCenter);
	pButtonTxt->SetTextColor(0 , 0 , 0);
	pButtonTxt->SetText( G_STRING_CHAR( IDS_BREAK ) );							
	pButtonTxt->SetClientImage(TID_None);
	pButtonTxt->SetFocusCheck(false);
	



}

///---------------------------------------------------------------------------
///-- RenderProc
///---------------------------------------------------------------------------
void CMiniGame_Elda::RenderProc()
{	
	l_Font			= Fontman->GetFont( n_ftGulimChe, 14, n_fwBold, n_fdOutline );
	l_NFont			= Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );	
	
	int li_cur_time;	
	TCHAR l_caTemp[100] = {0,};

	static int li_pr_sec = 0;
	int li_sec;
	int li_msec;
	int i,j;
	///-- 절대 Render 를 수행해서는 안되는 경우
	///-- Dynamic Release 를 수행 다음에는 결코 수행해서는 안된다.
	if( theGameMode == n_M_Destroy )
	{
		return;
	}
	///-----------------------------------------------------------------------
	///-- Draw Background	
	Render->Draw_CloneImg( TID_ELD_BACKGRIUND_BTM, theImgID, theBaseRect.x, theBaseRect.y + 200, theBaseRect.w, 243, D3DCOLOR_ARGB( 255, 255, 255, 255) );	
	Render->Draw_CloneImg( TID_ELD_BOARD, theImgID, theBaseRect.x + 100, theBaseRect.y + 63, 512, 128, D3DCOLOR_ARGB( 255, 255, 255, 255) );	
	
	///-- Cell render
	for( i = 0 ; i < theLimitCellsRow; ++i )		
	{
		for( j = 0 ; j < theLimitCellsCol; ++j )		
		{
			///---------------------------------------------------------------
			///-- Goal
			li_output_cells_x = theBaseRect.x + POS_GOAL_RENDER_X + ( j + ELDA_MAX_CELL_J - theLimitCellsCol ) * ELDA_CELL_SIZE_GAP;
			li_output_cells_y = theBaseRect.y + POS_GOAL_RENDER_Y + i * ELDA_CELL_SIZE_GAP;
			
			///-- Render Shadow
			if( theWorkingCol < j )
			{		
				Render->Draw_CloneImg( TID_ELD_SHADOW, theImgID, li_output_cells_x , li_output_cells_y , ELDA_CELL_SIZE , ELDA_CELL_SIZE , D3DCOLOR_ARGB( 255, 255, 255, 255) );			
			}
			///-- Gray
			else if( theWorkingCol == j )
			{
				_Render_Cell( &theGoalCells[i][j] , D3DCOLOR_ARGB ( 255, 255, 255, 255 ) );				

			}
			///-- Normal
			else
			{			
				_Render_Cell( &theGoalCells[i][j] , D3DCOLOR_ARGB ( 255, 150, 150, 150 ) );				
			}			
			
			///---------------------------------------------------------------
			///-- Work 
			li_output_cells_x = theBaseRect.x + POS_WORK_RENDER_X + ( j + ELDA_MAX_CELL_J - theLimitCellsCol ) * ELDA_CELL_SIZE_GAP;
			li_output_cells_y = theBaseRect.y + POS_WORK_RENDER_Y + i * ELDA_CELL_SIZE_GAP;
			
			_Render_Cell( &theWorkCells[i][j] ,D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}		
	}
	///-----------------------------------------------------------------------
	///-- Source Cells
	
	for( i = 0 ; i < ELDA_SOURCE_CELL_I ; ++i )
	{
		for( j = 0 ; j < ELDA_SOURCE_CELL_J ; ++j )
		{			
			//li_output_cells_x = theSlide_x + theBaseRect.x + j * ELDA_CELL_SIZE_SOURCE;
			//li_output_cells_y = theBaseRect.y + POS_SOURCE_RENDER_Y + i * ELDA_CELL_SIZE_GAP;
			li_output_cells_x = int( theSlide_x + theSourceCellsPos[i][j].x ); 
			li_output_cells_y = theSourceCellsPos[i][j].y;
			
			///-- Mask 바깥에 Draw 금지
			if( li_output_cells_x > theBaseRect.x + 20 && li_output_cells_x < theBaseRect.x + 570 )
			{
				_Render_Cell( &theSourceCells[i][j] , D3DCOLOR_ARGB( 255, 255, 255, 255 ) );	
			}
		}
	}

	///-----------------------------------------------------------------------
	///-- Button
	if( mb_abort_bt_ovr == 1 )
	{
		li_output_cells_x = theBaseRect.x + 524 ;			
		li_output_cells_y = theBaseRect.y + 404;
	
		//여기서 헬프팁은 띄어주믄 어떨까 ?? 
		// 기본 툴팁 
		SRect WcSize;//WndClientSize
		SHelpTip* HTip = Resource->GetHelpTipInfo( 472 );			
		if( HTip )
		{
			WcSize.x = li_output_cells_x;
			WcSize.y = li_output_cells_y;
			WcSize.w = 0;
			WcSize.h = 0;
			nRui->GetHelpTip()->Set( 472, &WcSize, HTip->Desc, HTip->Color );
			nRui->GetHelpTip()->RenderProc();
		}

		Render->Draw_CloneImg( TID_ABORT_OVR, theImgID, li_output_cells_x, li_output_cells_y, 
			73, 22, D3DCOLOR_ARGB( 255, 255, 255, 255) );	
	}
	else if ( mb_abort_bt_ovr == 2 )
	{
		li_output_cells_x = theBaseRect.x + 524 ;			
		li_output_cells_y = theBaseRect.y + 404;
		
		Render->Draw_CloneImg( TID_ABORT_CLK, theImgID, li_output_cells_x, li_output_cells_y, 
			73, 22, D3DCOLOR_ARGB( 255, 255, 255, 255) );	
	}
	
	pButtonTxt->RenderProc();

	///-----------------------------------------------------------------------
	///-- Animation Process
	
	the_proc_frame = 0;
	the_proc_frame2 = 0;

	if( !devb_pause )
	{
		li_cur_time  = timeGetTime();

		///-- 40 frame / second		
		the_accum_frame += ( li_cur_time - theprev_time  );
		the_proc_frame = the_accum_frame / devi_timeslice; ///--   /25 초당 40 Frame 기준		
		the_accum_frame -= the_proc_frame * devi_timeslice;

		///-- 10 frame / second
		the_accum_frame2 += ( li_cur_time - theprev_time  );
		the_proc_frame2 = the_accum_frame2 / 50; 
		the_accum_frame2 -= the_proc_frame2 * 50;

		theprev_time  = li_cur_time;
	}
	
	///-- Source Cell Silde
	_Render_Eff_Source_Slide();
	
	///-- Source 영역 Shadow Mask
	Render->Draw_CloneImg( TID_ELD_BACKGRIUND_TOP, theImgID, theBaseRect.x, theBaseRect.y, theBaseRect.w, 200, D3DCOLOR_ARGB( 255, 255, 255, 255) );	
	
	_RenderTitle();

	Render->DrawText( l_NFont, RCHAR(G_STRING_CHAR( IDS_LIMIT_TIME )) , 
					  theBaseRect.x + 527, theBaseRect.y + 28, D3DCOLOR_XRGB( 0, 0, 0 ));

	///-----------------------------------------------------------------------
	///-- Text
	li_sec = theTimeTotal / FRAME_PER_SECOUND ;
	li_msec = theTimeTotal * 100 / FRAME_PER_SECOUND - li_sec * 100;

	///-- Time
	Rsprintf( RWCHAR(l_caTemp), _RT( "%d " ), li_sec );	
	Render->DrawText( l_Font, l_caTemp, theBaseRect.x + 588, theBaseRect.y + 26, D3DCOLOR_XRGB( 255, 255, 255 ));
	
	///-- Score
	Rsprintf( RWCHAR(l_caTemp), _RT( "%d " ), theScore);	
	Render->DrawText( l_Font, l_caTemp, theBaseRect.x +295, theBaseRect.y + 408, D3DCOLOR_XRGB( 255, 255, 255 ));
	
	li_sec = theTimeTurn / FRAME_PER_SECOUND ;
	li_msec = theTimeTurn * 100 / FRAME_PER_SECOUND - li_sec * 100;

	///-- Time
	if( li_pr_sec != li_sec && !theSlideCell_out && !theSlideCell_in )
	{
		///-- Sound
		g_Sound.Play( 176, 0.0f, 0.0f, 0.0f );	
	}
	li_pr_sec = li_sec;
	
	
	///-- TIME BAR
	if( li_sec < 1 && !theSlideCell_out && !theSlideCell_in )
	{
		Render->Draw_CloneImg( TID_ELD_TIMEBAR1, theImgID, theBaseRect.x + 328, theBaseRect.y + 251, 14, 31, D3DCOLOR_ARGB( 255, 255, 255, 255) );	
	}
	
	if( li_sec < 2 && !theSlideCell_out && !theSlideCell_in )
	{
		Render->Draw_CloneImg( TID_ELD_TIMEBAR2, theImgID, theBaseRect.x + 328, theBaseRect.y+ 288, 14, 31, D3DCOLOR_ARGB( 255, 255, 255, 255) );	
	}
	
	if( li_sec < 3 && !theSlideCell_out && !theSlideCell_in )
	{
		Render->Draw_CloneImg( TID_ELD_TIMEBAR4, theImgID, theBaseRect.x + 328, theBaseRect.y+ 325, 14, 31, D3DCOLOR_ARGB( 255, 255, 255, 255) );	
	}


	///Rsprintf( RWCHAR(l_caTemp), _RT( "%d " ), li_sec );	
	///Render->DrawText( l_Font, l_caTemp, theBaseRect.x +320, theBaseRect.y + 60, D3DCOLOR_XRGB( 255, 255, 255 ));

	///---------------------------------------------------------------------------
	///-- _DEV_VERSION
	///---------------------------------------------------------------------------

#ifdef _DEV_VERSION
	Rsprintf( RWCHAR(l_caTemp), _RT( "Map Index %d " ), theGameId);	
	Render->DrawText( l_Font, l_caTemp, theBaseRect.x +10, theBaseRect.y + theBaseRect.h  + 10, D3DCOLOR_XRGB( 255, 255, 255 ));

	Rsprintf( RWCHAR(l_caTemp), _RT( "Miss_100: %d " ), theMissCount_100);	
	Render->DrawText( l_Font, l_caTemp, theBaseRect.x +10, theBaseRect.y + theBaseRect.h  + 35, D3DCOLOR_XRGB( 255, 255, 255 ));
	
	Rsprintf( RWCHAR(l_caTemp), _RT( "Miss_50 : %d " ), theMissCount_50);	
	Render->DrawText( l_Font, l_caTemp, theBaseRect.x +10, theBaseRect.y + theBaseRect.h  + 60, D3DCOLOR_XRGB( 255, 255, 255 ));
	
	///-- theLevel
	Rsprintf( RWCHAR(l_caTemp), _RT( "Level : %d " ), theLevel);	
	Render->DrawText( l_Font, l_caTemp, theBaseRect.x +10, theBaseRect.y + theBaseRect.h +85, D3DCOLOR_XRGB( 255, 255, 255 ));
		
	///-- theLimitSourceNum
	Rsprintf( RWCHAR(l_caTemp), _RT( "Source Num :%d " ), theLimitSourceNum);	
	Render->DrawText( l_Font, l_caTemp, theBaseRect.x +10, theBaseRect.y + theBaseRect.h +110, D3DCOLOR_XRGB( 255, 255, 255 ));
	
	
#endif

	///-- Level 에 따른 소스셀 무빙 효과
	if( theLevel >15 && !lb_prev_slide_out && !lb_prev_slide_in && theGameMode == n_M_On_GAME )
	{
		_Process_SrcCell_Move();
	}
	

	///-- Working Goal Cell Blinking Effect
	_Render_Eff_Blinking_Cells();
	
	///-- Goal Focus;
	_Render_Goal_Focus();


	///-- Shadow Hit Animamtion
	_Render_Eff_Shadow();
	
	///-- Cell Hit Animamtion
	_Render_Eff_Cell_Hit();
	
	///-- Combo Message
	_Render_Eff_ComboMsg();
	
	///-- Render_Eff_Miss
	_Render_Eff_Miss();

	///-- Render Go
	_Render_Eff_MSGGo();

	///-- Render Ready
	_Render_Eff_MSGReady();
	
	///-- Render_Eff_MSGEnd
	_Render_Eff_MSGEnd();

	///-- Render 
	_Render_Eff_MSGTimeOver();
	
	///-- frame
	if( the_proc_frame )
	{
		///-- Temp time Process
		_Time_Process();		
		
		///-- Flow Manager TEMP
		Logic_Flow_Manager();
	}	

}

void CMiniGame_Elda::_RenderTitle()
{
	if( g_RockClient.GetLanguage() == eLang_Japan )
	{
		l_TitleFont = Fontman->GetFont( n_ftDotum, 11, n_fwBold, n_fdNormal, true );    ///-- japan
	}
	else
	{
		l_TitleFont = Fontman->GetFont( n_ftDotum, 10, n_fwNormal, n_fdNormal, true );  ///-- korea
	}

	Render->DrawText( l_TitleFont, m_strTitleName, theBaseRect.x + 79, theBaseRect.y +14, 
					 D3DCOLOR_XRGB( 70, 70, 70 ));
}


///---------------------------------------------------------------------------
///-- EventProc
///---------------------------------------------------------------------------
N_EVENTRESULT CMiniGame_Elda::EventProc( SKeys* Keys, SMouse* Mouse )
{
	int i,j;
	if( g_RockClient.m_EtcInfo.DevMode == 2)
	{
		if( (Keys[DIK_I].Action == n_baDown)  )
		{
			//작업 폴더의 첫번째 블럭을 가져와 소에서 찾아 온다.. 몇번재 꺼인가 ? 
			// 성공한것은 쌩간당..
			for( i = 0 ; i < ELDA_SOURCE_CELL_I ; ++i )
			{
				for(int j = 0 ; j < ELDA_SOURCE_CELL_J ; ++j )
				{
					if(theSourceCells[i][j].me_type == m_type && 
						(theSourceCells[i][j].mi_color == m_color || 	theSourceCells[i][j].mi_sub_color == m_color)
						)
					{
						Logic_Pick_Source_Cell( i, j );
					}
				}
			}
		}
	}

		
	///-----------------------------------------------------------------------
	///-- Development Version Cheat
#ifdef _DEBUG 

	if( g_RockClient.m_EtcInfo.DevMode == 1 )
	{
			
		if( (Keys[DIK_I].Action == n_baDown)  )
		{
			//작업 폴더의 첫번째 블럭을 가져와 소에서 찾아 온다.. 몇번재 꺼인가 ? 
			// 성공한것은 쌩간당..
			for( i = 0 ; i < ELDA_SOURCE_CELL_I ; ++i )
			{
				for(int j = 0 ; j < ELDA_SOURCE_CELL_J ; ++j )
				{
					if(theSourceCells[i][j].me_type == m_type && 
						(theSourceCells[i][j].mi_color == m_color || 	theSourceCells[i][j].mi_sub_color == m_color)
						)
					{
							Logic_Pick_Source_Cell( i, j );
					}
				}
			}
		}

		
		/*	
		if( Keys[DIK_2].Action == n_baDown )
		{
			Logic_Pick_Source_Cell( 1, 0 );
		}
		if( Keys[DIK_3].Action == n_baDown )
		{
			Logic_Pick_Source_Cell( 0, 1 );
		}
		if( Keys[DIK_4].Action == n_baDown )
		{
			Logic_Pick_Source_Cell( 1, 1 );
		}
		if( Keys[DIK_5].Action == n_baDown )
		{
			Logic_Pick_Source_Cell( 0, 2 );
		}
		*/
		
		///-- Pause
		if( Keys[DIK_P].Action == n_baDown )
		{
			devb_pause = !devb_pause;
		}
		///-- CHEAT
		if( Keys[DIK_L].Action == n_baDown )
		{
			devb_cheat_src = !devb_cheat_src;
		}
		///-- SPEED	
	
		if( Keys[DIK_6].Action == n_baDown )
		{
			devi_timeslice = 5;
		}
		if( Keys[DIK_7].Action == n_baDown )
		{
			devi_timeslice = 25;
		}
		
		if( Keys[DIK_8].Action == n_baDown )
		{
			devi_timeslice = 50;
		}
		if( Keys[DIK_9].Action == n_baDown )
		{
			devi_timeslice = 150;
		}
			
		if( Keys[DIK_0].Action == n_baDown )
		{
			Gen_Source_Cells();
		}
	}

#endif
	
	///-----------------------------------------------------------------------
	///-- Mouse Event	
	if(	Mouse->uiLButton == n_baDown)
	{
		for( i = 0 ; i < ELDA_SOURCE_CELL_I ; ++i )
		{
			for( j = 0 ; j < ELDA_SOURCE_CELL_J ; ++j )
			{

				if( Mouse->Aposition.x > theSourceCellsPos[i][j].x + theSlide_x  - 5 && 
					Mouse->Aposition.x < theSourceCellsPos[i][j].x + ELDA_CELL_SIZE + theSlide_x + 5&&
					Mouse->Aposition.y > theSourceCellsPos[i][j].y - 5 && 
					Mouse->Aposition.y < theSourceCellsPos[i][j].y + ELDA_CELL_SIZE + 5 )
				{
					if( theSourceCells[i][j].me_type != n_ELDA_None )
					{
						Logic_Pick_Source_Cell( i, j );
					}					
				}
			}
		}

		///-- Abort Button Click
		if( Mouse->Aposition.x > 524 + theBaseRect.x && Mouse->Aposition.x < 597 + theBaseRect.x &&
			Mouse->Aposition.y > 404 + theBaseRect.y && Mouse->Aposition.y < 426 + theBaseRect.y )
		{
			mb_abort_bt_ovr = 2;
		}
		
	}
	if(	Mouse->uiLButton == n_baUp)
	{
		mb_abort_bt_ovr = 0;

		if( Mouse->Aposition.x > 524 + theBaseRect.x && Mouse->Aposition.x < 597 + theBaseRect.x &&
			Mouse->Aposition.y > 404 + theBaseRect.y && Mouse->Aposition.y < 426 + theBaseRect.y )
		{
			///-- Game Abort
			_Process_Abort( 0 );			
		}
	}

	///-- Abort Button Over
	if(	Mouse->State == n_msMove)
	{
		if( Mouse->Aposition.x > 524 + theBaseRect.x && Mouse->Aposition.x < 597 + theBaseRect.x &&
			Mouse->Aposition.y > 404 + theBaseRect.y && Mouse->Aposition.y < 426 + theBaseRect.y )
		{
			mb_abort_bt_ovr = 1;
		}
		else
		{
			mb_abort_bt_ovr = 0;
		}
	}
	
	
	return n_ERESULT_UIPROC;
}

///---------------------------------------------------------------------------
///-- Add_WorkingCell
///---------------------------------------------------------------------------
///-- 선택된 Cell 을 Working 영역에 추가한다.
///-- return value 
///--	-1 : 추가 할 수 없음
///--	n  : 추가된 Row
int CMiniGame_Elda::Add_WorkingCell( const ELDA_CELL * const ac_cell )
{
	int i;
	ELDA_CELL lc_CompareTemp;

	///-- Cell 이 있을 곳을 밑으로 부터 검사
	for( i = theLimitCellsRow -1 ; i >= 0 ; --i )		
	{
		///-- 틀린 곳이 있다면 그 곳이 삽입될 Cell 이다
		if( theWorkCells[i][theWorkingCol].me_type != theGoalCells[i][theWorkingCol].me_type)
		{
			///---------------------------------------------------------------
			///-- 조합 뒤 검사 한다.
			lc_CompareTemp = theWorkCells[i][theWorkingCol] ; 


			///-- None
			if( lc_CompareTemp.me_type == n_ELDA_None )
			{
				lc_CompareTemp = *ac_cell;
				lc_CompareTemp.mi_sub_color = 0;
			}
			///-- RB
			else if ( lc_CompareTemp.me_type == n_ELDA_RB)
			{
				if( ac_cell->me_type == n_ELDA_LT)
				{
					lc_CompareTemp.me_type = n_ELDA_BoxRL;
					lc_CompareTemp.mi_sub_color = ac_cell->mi_color;
				}
				else
				{
					return -1;
				}
			}
			///-- LB
			else if ( lc_CompareTemp.me_type == n_ELDA_LB)
			{
				if( ac_cell->me_type == n_ELDA_RT)
				{
					lc_CompareTemp.me_type = n_ELDA_BoxLR;
					lc_CompareTemp.mi_sub_color = ac_cell->mi_color;
				}
				else
				{
					return -1;
				}
			}
			///---------------------------------------------------------------
			///-- 성공
			if( lc_CompareTemp == theGoalCells[i][theWorkingCol])			
			{
				///-- 삽입
				theWorkCells[i][theWorkingCol] = lc_CompareTemp;
				return i;
			}
			if( lc_CompareTemp.me_type == n_ELDA_LB && theGoalCells[i][theWorkingCol].me_type == n_ELDA_BoxLR 
				&& lc_CompareTemp.mi_color == theGoalCells[i][theWorkingCol].mi_color )
			{
				///-- 삽입				
				theWorkCells[i][theWorkingCol] = lc_CompareTemp;
				return i;
			}
			if( lc_CompareTemp.me_type == n_ELDA_RB && theGoalCells[i][theWorkingCol].me_type == n_ELDA_BoxRL 
				&& lc_CompareTemp.mi_color == theGoalCells[i][theWorkingCol].mi_color )
			{
				///-- 삽입				
				theWorkCells[i][theWorkingCol] = lc_CompareTemp;
				return i;
			}
			
			///-- 실패
			return -1;			
		}
	}
	return -1;
}


///---------------------------------------------------------------------------
///-- Check_Column_Complete
///---------------------------------------------------------------------------
///-- 하나의 Column 이 모두 완성 되었는지 판단
bool CMiniGame_Elda::Check_Column_Complete() const
{
	int i;
	bool lb_complete = true;

	///-- 모든 Cell 검사
	for( i = 0; i < theLimitCellsRow ; ++i )
	{
		if( !( theWorkCells[i][theWorkingCol] == theGoalCells[i][theWorkingCol] ) )
		{
			lb_complete = false;
		}
	}	
	return lb_complete;
}

///---------------------------------------------------------------------------
///-- Complete_Column_Process
///---------------------------------------------------------------------------
///-- 한줄이 완성되었을 때의 처리
void CMiniGame_Elda::_Process_Complete_Column()	
{
	SEldacraftGameCell cells[5];
	int li_endcode;	
	int i;
	int li_score;
	

	///-----------------------------------------------------------------------
	///-- Score
	li_score = theHitCount * 20;
	theScore += li_score;	
	theScore += theComboCnt * li_score; 

	///-----------------------------------------------------------------------
	///-- Network
	///-- Check Ending
	if( theWorkingCol +1 >= theLimitCellsCol )
	{
		li_endcode = 1;
	}
	else
	{
		li_endcode =0;
	}
	///-- Cells 
	for( i = 0 ; i < 5; ++i )
	{
		cells[i].theColor = theWorkCells[i][theWorkingCol].mi_color;
		cells[i].theSubColor = theWorkCells[i][theWorkingCol].mi_sub_color;
		cells[i].theType = theWorkCells[i][theWorkingCol].me_type;
	}

	if( theMissCount_100 > 255) theMissCount_100 = 255;
	if( theMissCount_50 > 255) theMissCount_50 = 255;
	if( theComboCnt > 255) theComboCnt = 255;

	///-- !!!!
	g_RockClient.Send_CsEldacraftGame( theMissCount_100, theMissCount_50, theComboCnt, theScore, 
									   (BYTE)li_endcode, cells);

	///-----------------------------------------------------------------------
	///-- Prepaer Next Turn

	///-- Color 변경
	for( i = 0; i < theLimitCellsRow ; ++i )
	{
		if( theWorkCells[i][theWorkingCol].me_type != n_ELDA_None)
		{
			theWorkCells[i][theWorkingCol].mi_color = 12;		

			if( theWorkCells[i][theWorkingCol].me_type == n_ELDA_BoxRL || 
				theWorkCells[i][theWorkingCol].me_type == n_ELDA_BoxLR )
			{
				theWorkCells[i][theWorkingCol].mi_sub_color = 12;
			}
		}
	}		

	///-- flag
	theMissCount_100 = 0;
	theMissCount_50 = 0;
	theColumnScore = 0;
	theHitCount = 0;	
	theComboEnable = true;	
}

///---------------------------------------------------------------------------
///-- Complete_Total_Process
///---------------------------------------------------------------------------
///-- 전체가 완성되었을 때의 처리
void CMiniGame_Elda::_Process_Complete_Total()
{
	theGameMode = n_M_End;
	_EffSet_End_Msg();
}	

///---------------------------------------------------------------------------
///-- _Process_Hit
///---------------------------------------------------------------------------
void CMiniGame_Elda::_Process_Hit()
	///-- Point
{
	theScore += 5;
	
	///-- Hit Count
	++theHitCount;

}
///---------------------------------------------------------------------------
///-- _Process_Miss
///---------------------------------------------------------------------------
///-- Miss 할 때의 Process
///-- 0 이하일 경우 Server에 Miss 를 보내지 않는다.
void CMiniGame_Elda::_Process_Miss()
{
	///-- Point
	theScore -= 10;
	

	///-- 50 점 Miss
	if( theScore == -5 )
	{
		++theMissCount_50;
	}

	if( theScore < 0 )
	{
		theScore  = 0;		
	}
	else
	{
		++theMissCount_100 ;
	}
	theComboEnable = false;
	theComboCnt = 0;	
	_EffSet_Source_Slide();
	
}
///---------------------------------------------------------------------------
///-- _Process_Abort
///---------------------------------------------------------------------------
///-- 게임이 중도에 취소될 경우
void CMiniGame_Elda::_Process_Abort(const int ai_code)
{
	int i;
	SEldacraftGameCell cells[5];
	theTimeTotal = 0;
	
	
	///-- Effect
	if( ai_code )
	{	
		theGameMode = n_M_End;			
		_EffSet_TimeOver_Msg();
	}
	else
	{
		theGameMode = n_M_Destroy;
		Resource->DestroyDynamic( TID_ELD_BACKGRIUND_TOP );	
		Resource->DestroyDynamic( TID_Number_2 );	
		Resource->DestroyDynamic( TID_MSG_MISSION );	
		Resource->DestroyDynamic( TID_ELD_SHADOWHIT );	
		Resource->DestroyDynamic( TID_MINIGAMETEMP_3 );	
		Resource->DestroyDynamic( TID_MINIGAMETEMP_2 );	
		
	}

	///-- Cells 
	for( i = 0 ; i < 5; ++i )
	{
		cells[i].theColor = theWorkCells[i][theWorkingCol].mi_color;
		cells[i].theSubColor = theWorkCells[i][theWorkingCol].mi_sub_color;
		cells[i].theType = theWorkCells[i][theWorkingCol].me_type;
	}
	
	///-- !!!!
	g_RockClient.Send_CsEldacraftGame( theMissCount_100, theMissCount_50, 0, theScore, 2, cells);

}
///---------------------------------------------------------------------------
///-- Gen_Goal_Cells
///---------------------------------------------------------------------------
///-- 목표 영역을 만든다.
void CMiniGame_Elda::Gen_Goal_Cells( int ai_index , BYTE theGameTimeRatio )
{
	int i,j;

	ELDA_Data_Header ls_data_header;
	ELDA_Data_Set	ls_data_set;
	
///////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
	FILE* fp;
	
	fp = _tfopen( _T("Interface\\eld_puzmap.dat"), _T("rb") );//VSS올리때...옵션 주의(읽기전용)일때....
	if( fp == NULL )
	{
		MessageBox( NULL, _T("!eld_puzmap.dat 파일 열기 실패!"), _T("알림"), MB_OK );
		return;
	}

	///-- read header
	fread( ( void * )&ls_data_header, sizeof( ELDA_Data_Header ), 1, fp);

	///-- Seek
	fseek( fp, ai_index * sizeof( ELDA_Data_Set ) , SEEK_CUR );
		
	///-- Read Header
	fread( ( void * )&ls_data_set, sizeof( ELDA_Data_Set ), 1, fp);

	fclose( fp );
	
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFInterface.OpenFile( "eld_puzmap.dat" );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!eld_puzmap.dat 파일 열기 실패!"), _T("알림"), MB_OK );
		return;
	}
	
	///-- Read Header
	pFH->Read( ( void * )&ls_data_header, sizeof( ELDA_Data_Header ));

	///-- Seek
	pFH->Seek( ai_index * sizeof( ELDA_Data_Set ) , SEEK_CUR );

	///-- Read Header
	pFH->Read( ( void * )&ls_data_set, sizeof( ELDA_Data_Set ));
	
	g_VFInterface.CloseFile( pFH );

///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	///-----------------------------------------------------------------------
	///-- Setting
	theLimitCellsCol = ls_data_set._max_col;
	theLimitCellsRow = 5;
	theGameId = ls_data_set._id;
	theLevel	= ls_data_set._level;
	///-- Open Beta 시 변경 
	
	theTimeTotal = (int)(ls_data_set._limit_time * (float)(theGameTimeRatio/100.0f) 
						  * FRAME_PER_SECOUND);
	
	///-- theTimeTotal = 40 * FRAME_PER_SECOUND;
	theLimitColor = ls_data_set._max_color;
	theLimitSourceNum = ls_data_set._max_color;
	
	int nLevel = theLevel + 1;
	TCHAR buf[256] = {0,};

	if( 1 <= nLevel && nLevel <= 5 )
	{
		Rsprintf(RWCHAR(buf), _RT(" Easy"));
	}
	else if( 6 <= nLevel && nLevel <= 10 )
	{
		Rsprintf(RWCHAR(buf), _RT(" Normal"));
	}
	else if( 11 <= nLevel && nLevel <= 15 )
	{
		Rsprintf(RWCHAR(buf), _RT(" Hard"));
	}
	else if( 16 <= nLevel )
	{
		Rsprintf(RWCHAR(buf), _RT(" Hell"));
	}

#ifdef ROCKCLIENT_DEV
	Rsprintf(RWCHAR(buf),_RT("%s - %d"), RWCHAR(buf), nLevel);
#endif
	
	Rstrcat(RWCHAR(m_strTitleName), RWCHAR(buf));	

	///-- Cell 복사
	for( i = 0 ; i < ELDA_MAX_CELL_I ; ++i )
	{
		for( j = 0 ; j < ELDA_MAX_CELL_J ; ++j )
		{
			theGoalCells[i][j] = ls_data_set._Cells[i][j];
		}
	}	
}
///---------------------------------------------------------------------------
///-- Gen_Source_Cells
///---------------------------------------------------------------------------
///-- Source 영역을 random 으로 생성한다.
///-- Level 에 따른 생성 개수 틀림
void CMiniGame_Elda::Gen_Source_Cells()
{
	int i,j;
	int ii,jj;
	int v;
	ELDA_CELL le_stream[ ELDA_SOURCE_CELL_I * ELDA_SOURCE_CELL_J]; 
	int li_str_idx = 0;

	///-- Cell 이 있을 곳을 밑으로 부터 검사
	for( i = theLimitCellsRow -1 ; i >= 0 ; --i )		
	{
		///-- 각 Cell 을 Stream 으로
		if( theGoalCells[i][theWorkingCol].me_type != n_ELDA_None )
		{
			///-- Box LR
			if( theGoalCells[i][theWorkingCol].me_type == n_ELDA_BoxLR )
			{
				le_stream[ li_str_idx ].me_type = n_ELDA_LB;
				le_stream[ li_str_idx ].mi_color = theGoalCells[i][theWorkingCol].mi_color;

				le_stream[ ++li_str_idx ].me_type = n_ELDA_RT;
				le_stream[ li_str_idx ].mi_color = theGoalCells[i][theWorkingCol].mi_sub_color;

				++li_str_idx;
			}
			///-- Box RL
			else if( theGoalCells[i][theWorkingCol].me_type == n_ELDA_BoxRL )
			{
				le_stream[ li_str_idx ].me_type = n_ELDA_RB;
				le_stream[ li_str_idx ].mi_color = theGoalCells[i][theWorkingCol].mi_color;

				le_stream[ ++li_str_idx ].me_type = n_ELDA_LT;
				le_stream[ li_str_idx ].mi_color = theGoalCells[i][theWorkingCol].mi_sub_color;

				++li_str_idx;
			}
			///-- Normal
			else
			{
				le_stream[ li_str_idx ].me_type = theGoalCells[i][theWorkingCol].me_type;
				le_stream[ li_str_idx ].mi_color = theGoalCells[i][theWorkingCol].mi_color;
				
				++li_str_idx;				
			}
		}
	}

	///-- random stream
	while( li_str_idx < theLimitSourceNum)
	{
		///-- type
		v = int(( (float)rand() / (float)0x7fff) *4.0f) +1; //-- 0~9까지의 random 값
		le_stream[ li_str_idx ].me_type = (char)v;

		///-- color
		v = int(( (float)rand() / (float)0x7fff) * theLimitColor); //-- 0~9까지의 random 값
		le_stream[ li_str_idx ].mi_color = (char)v;

		++li_str_idx;
	}
	
	
	///-- Clear Source Cells
	for( j = 0 ; j < ELDA_SOURCE_CELL_J ; ++j )
	{
		for( i = 0 ; i < ELDA_SOURCE_CELL_I ; ++i )
		{		
			theSourceCells[i][j].me_type = n_ELDA_None;
		}
	}


	///-- Fill Source Cells
	li_str_idx = 0;
	
	for( j = 0 ; j < ELDA_SOURCE_CELL_J ; ++j )
	{
		for( i = 0 ; i < ELDA_SOURCE_CELL_I ; ++i )
		{		
			///-- 완료체크
			if( le_stream[ li_str_idx ].me_type == n_ELDA_None)
			{
				return;
			}
			///-- Random 하게 물리적 위치
			do {
				ii = int(( (float)rand() / (float)0x7fff) * ELDA_SOURCE_CELL_I) ; 
				jj = int(( (float)rand() / (float)0x7fff) * ELDA_SOURCE_CELL_J) ; 
			}while ( theSourceCells[ii][jj].me_type != n_ELDA_None );
			
			
///---------------------------------------------------------------------------
///-- _DEV_VERSION
///---------------------------------------------------------------------------
#ifdef _DEV_VERSION
			if(devb_cheat_src)
			{
				ii=i;
				jj=j;
			}
#endif
			
			///-- type
			
			theSourceCells[ii][jj] = le_stream[ li_str_idx ];	
			v = int(( (float)rand() / (float)0x7fff) * 21) - 10; //-- -5~5까지의 random 값
			theSourceCellsPos[ii][jj].x = theBaseRect.x + jj * 65 + v;
			v = int(( (float)rand() / (float)0x7fff) * 11) - 5; //-- -5~5까지의 random 값
			theSourceCellsPos[ii][jj].y = theBaseRect.y + POS_SOURCE_RENDER_Y + ii * 50 + 20 + v;			
			li_str_idx ++;
		}
	}
	
}

///---------------------------------------------------------------------------
///-- _Time_Process
///---------------------------------------------------------------------------
///-- 시간에 따른 처리를 한다.
void CMiniGame_Elda::_Time_Process()
{
	///-- Game 진행 중일 때만 시간 감소
	if( theGameMode == n_M_On_GAME)
	{
		theTimeTurn += the_proc_frame;
		theTimeTotal -=the_proc_frame;

		if( theTimeTotal < 0 )
		{
			_Process_Abort(1);
		}
	}

	///-- 3초 지남 
	if( theTimeTurn > 120 )
	{
		theTimeTurn = 0;	
		_EffSet_Source_Slide();
		
		theComboEnable = false;
		theComboCnt = 0;				
	}	
}
///---------------------------------------------------------------------------
///-- _Process_SrcCell_Move
///---------------------------------------------------------------------------
void CMiniGame_Elda::_Process_SrcCell_Move()
{
	int i,j;
	int fm;
	int lb_conc;	
	SRect	newRect;	///-- Base Rect
	newRect.h = ELDA_CELL_SIZE;
	newRect.w = ELDA_CELL_SIZE;
	int k,l;
	//int temp_mvx, temp_mvy;
	
	for( fm = 0 ; fm < the_proc_frame2  ; ++fm )
	{
		for( i = 0 ; i < ELDA_SOURCE_CELL_I; ++i )
		{
			for( j = 0 ; j < ELDA_SOURCE_CELL_J ; ++j )
			{
				if( theSourceCells[i][j].me_type != n_ELDA_None)
				{
					///-- x 좌표
					newRect.x = theSourceCellsPos[i][j].x + theSource_Mov[i][j].x;
					///-- y 좌표
					newRect.y = theSourceCellsPos[i][j].y + theSource_Mov[i][j].y;					
					
					///-- 충돌 체크
					lb_conc = false;
					
					if( newRect.x > theBaseRect.x +	365 || newRect.x < theBaseRect.x + 50 ) 
					{
						theSource_Mov[i][j].x = -theSource_Mov[i][j].x;
						lb_conc = true;
					}
					
					if( newRect.y > theBaseRect.y + 145 || newRect.y < theBaseRect.y + 80 ) 
					{
						theSource_Mov[i][j].y = -theSource_Mov[i][j].y;
						lb_conc = true;
					}
					
					if( !lb_conc )
					{
						for( k = 0 ; k < ELDA_SOURCE_CELL_I; ++k )
						{
							for( l = 0 ; l < ELDA_SOURCE_CELL_J ; ++l )
							{
								///-- 충돌한다면
								if( theSourceCells[k][l].me_type != n_ELDA_None &&
									!( k == i && l == j )&&
									newRect.x + ELDA_CELL_SIZE >= theSourceCellsPos[k][l].x && 
									newRect.x <= theSourceCellsPos[k][l].x + ELDA_CELL_SIZE &&						
									newRect.y + ELDA_CELL_SIZE >= theSourceCellsPos[k][l].y && 
									newRect.y <= theSourceCellsPos[k][l].y + ELDA_CELL_SIZE
									)
								{
									lb_conc = true;
									theSource_Mov[i][j].y = -theSource_Mov[i][j].y;	
									theSource_Mov[i][j].x = -theSource_Mov[i][j].x;	
									l = ELDA_SOURCE_CELL_J; ///- 루프 종료
									k = ELDA_SOURCE_CELL_I; ///- 루프 종료
								}
							}
						}

						///-- 이동
						if( !lb_conc )
						{
							theSourceCellsPos[i][j].y = newRect.y;
							theSourceCellsPos[i][j].x = newRect.x;
						}
					}
				}
				
			}
		}
	}
}