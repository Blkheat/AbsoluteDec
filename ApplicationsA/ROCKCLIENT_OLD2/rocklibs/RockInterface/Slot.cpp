///////////////////////////////////////////////////////////////////////////////
///
///		File		: Slot.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-06-09
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "Rui.h"

#include "Slot.h"
#include "FrameWnd.h"

//#include "..\\..\\Bravolibs\\Obj\\SkillSystem.h"
//#include "..\\..\\Pc.h"


//-----------------------------------------------------------------------------
CSlot::CSlot( SDesktop* DT ) : CTextBox( DT )
{
	IsScaleTrans = false;
	IsButtonSlot = false;
	IsDelayShow = true; 

	m_Bx = 0;
	m_By = 0;

	Clear();	
}

//-----------------------------------------------------------------------------
CSlot::~CSlot()
{	

}

void CSlot::Clear()
{
	Group	= n_SGROUP_NONE;//소속( 퀵슬롯창, 스킬창, 인벤토리창(2), 교환창(2), 상점창, 은행창 )
	SlotNum	= 0;
	Class	= n_ICLASS_NONE;
	Type	= 0;

	PcItem	= NULL;
	
	SlotAddr = NULL;
	
	IsHighlight					= true;
	HLDuration					= false;
	HLState						= n_NONE_HIGHLIGHT;
	HLImg[n_NONE_HIGHLIGHT]		= TID_None;
	HLImg[n_FOCUS_HIGHLIGHT]	= TID_SlotHighLight;
	HLImg[n_PRESS_HIGHLIGHT]	= TID_SlotHighLight;

	AlignX = n_atRight;
	AlignY = n_atBottom;
	
	theSlotState = n_Slot_Enable;

	return;
}

//-----------------------------------------------------------------------------

SEventMessage* CSlot::EventProc( SEventMessage* EMsg )
{
	CFrameWnd*	fwnd			= NULL;
	CSlot*		slot			= NULL;


	int Mx = EMsg->Mouse.Aposition.x;
	int My = EMsg->Mouse.Aposition.y;
	
	int x = 0;

	switch( EMsg->Mouse.Action )
	{
	case MA_FOCUS:
		{
			OnMouseFocus( Mx, My );
			HLState = n_FOCUS_HIGHLIGHT;
			
			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_SLOT_FOCUS;
		}
		break;
	case MA_LDOWN:
		{
			OnMouseLButtonDown( Mx, My );		
			HLState = n_PRESS_HIGHLIGHT;

			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_SLOT_PRESS;

			CFrameWnd* fwnd = (CFrameWnd*)Pwnd;
			if( ( GetParentID() != WID_None ) && ( fwnd->theWnd != NULL ) )
			{
				fwnd->theWnd->Proc( &UpdateEMsg );
			}
		}
		break;
	case MA_LPRESS:
		{
			OnMouseLButtonPress( Mx, My );

			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_SLOT_PRESS;
		}
		break;
	case MA_LUP:
		{
			OnMouseLButtonUp( Mx, My );
			HLState = n_FOCUS_HIGHLIGHT;

			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_SLOT_CLICK;

			//드래그 상태에서 UP으로 바뀔 경우....UP처리가 되지 않기때문에....
			if( pDT->SelectedWnd != WID_None )
			{
				fwnd = (CFrameWnd*)Pwnd;
				if( ( GetParentID() != WID_None ) && ( fwnd->theWnd != NULL ) )
				{
					fwnd->theWnd->Proc( EMsg );
					EMsg->Notify = NM_NONE;
					return( NULL );	
				}
			}
		}
		break;
	case MA_LDRAG:
		{
			if( pDT->CurFocusWnd != WID_None )
			{
				slot = (CSlot*)Container->Get( pDT->CurFocusWnd );
			
				if( slot->GetWindowType() == n_wtSlot )
				{
					slot->SetHighlight( n_FOCUS_HIGHLIGHT );
				}
			}
			
			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_SLOT_DRAG;
			EMsg->fParam	= (EPARAM)pDT->CurFocusWnd;

			//드래그 중일때는 부모 윈도우로 통지가 안되기 때문에.... 메세지 처리를 못한다.
			//그래서 지금은 편법으로 사용....
			fwnd = (CFrameWnd*)Pwnd;
			if( ( GetParentID() != WID_None ) && ( fwnd->theWnd != NULL ) )
			{
				fwnd->theWnd->Proc( EMsg );
			}
		}
		break;
	case MA_LDROP:
		{
			HLState = n_NONE_HIGHLIGHT;

			EMsg->FocusWnd	= Iid;//pDT->SelectedWnd;
			EMsg->Notify	= NM_SLOT_DROP;
			EMsg->fParam	= (EPARAM)pDT->CurFocusWnd;
			//EMsg->sParam	= (EPARAM)this;
			
			fwnd = (CFrameWnd*)Pwnd;
			//if( ( GetParentID() != WID_None ) && ( fwnd->WndProc != NULL ) )
			if( ( GetParentID() != WID_None ) && ( fwnd->theWnd != NULL ) )
			{
				fwnd->theWnd->Proc( EMsg );
			}
		}
		break;
	case MA_RUP:
		{
			OnMouseRButtonUp( Mx, My );		
			HLState = n_PRESS_HIGHLIGHT;

			EMsg->FocusWnd	= Iid;
			EMsg->Notify	= NM_SLOT_DBCLICK;

			//드래그 상태에서 UP으로 바뀔 경우....UP처리가 되지 않기때문에....
			if( pDT->SelectedWnd != WID_None )
			{
				fwnd = (CFrameWnd*)Pwnd;
				if( ( GetParentID() != WID_None ) && ( fwnd->theWnd != NULL ) )
				{
					fwnd->theWnd->Proc( EMsg );
					EMsg->Notify = NM_NONE;
					return( NULL );	
				}
			}			
		}
		break;
	}

	return( EMsg );
}

//-----------------------------------------------------------------------------
void CSlot::SetSlotGroup( N_SLOTGROUP group )
{
	Group = group;
}

//-----------------------------------------------------------------------------
void CSlot::SetSlotGroup( N_SLOTGROUP group, int slotnum )
{
	Group	= group;
	SlotNum	= slotnum;
}

//-----------------------------------------------------------------------------
void CSlot::SetSlotNum( int slotnum )
{
	SlotNum = slotnum;
}

//-----------------------------------------------------------------------------
void CSlot::SetWearMark( int mark )
{
	Class = mark;
}

void CSlot::SetWearType( int type )
{
	Type = type;
}

//-----------------------------------------------------------------------------
void CSlot::SetSlotItem( SPcItem* pcitem )
{
	PcItem = pcitem;
}

void CSlot::SetSlotPcItem( SPcItem aPcItem )
{
	*(PcItem) = aPcItem;
}

void CSlot::SetScaleTrans(bool bFlag)
{
	IsScaleTrans = bFlag;
}

void CSlot::SetButtonSlot(bool bFlag)
{
	IsButtonSlot = bFlag;
}

//-----------------------------------------------------------------------------
SPcItem* CSlot::GetSlotItem()
{
	return( PcItem );
}

//-----------------------------------------------------------------------------
SPcItem* CSlot::GetSlotItemMove()
{
	SPcItem* rPcItem = PcItem;
	PcItem = NULL;

	return( rPcItem );
}

//-----------------------------------------------------------------------------
void CSlot::SetHighlightImg( TEXID focus_img, TEXID click_img )
{
	HLImg[n_FOCUS_HIGHLIGHT] = focus_img;
	HLImg[n_PRESS_HIGHLIGHT] = click_img;
}

//-----------------------------------------------------------------------------
void CSlot::SetHighlight( N_HIGHLIGHT hl_state )
{
	HLState		= hl_state;
}

//-----------------------------------------------------------------------------
void CSlot::SetHighlight( N_HIGHLIGHT hl_state, bool hl_duration )
{
	HLState		= hl_state;
	HLDuration	= hl_duration;
}

//-----------------------------------------------------------------------------
void CSlot::SetHighlight( bool hl_duration )
{
	HLState		= n_FOCUS_HIGHLIGHT;
	HLDuration	= hl_duration;
}

//-----------------------------------------------------------------------------
void CSlot::RenderProc()
{	
	int li_color_red = 255;
	int li_color_green = 255;
	int li_color_blue = 255;
	UpdateHandler();

	if( pDT->CurFocusWnd != WID_None )
	{
		if( ( pDT->CurFocusWnd != Iid ) && ( nRui->theCursorState == n_CT_HAND ) )
		{
			nRui->theCursorState = n_CT_NORMAL;
		}
	}	

	if( PcItem != NULL )
	{
		///-- Disable
		if( theSlotState == n_Slot_Disable )
		{	
			li_color_red = 128;
			li_color_green = 128;
			li_color_blue = 180;
			///-- Render->Draw( PcItem->Tid, Cx, Cy, Cw, Ch, D3DCOLOR_XRGB( 128, 128, 180 ) );
		}
		
		///-- 내구력 감소 붉은색 표현
		if( ( PcItem->IType == n_ITYPE_WEAR || PcItem->IType == n_ITYPE_RIDE || PcItem->IType == n_ITYPE_RIDE_WEAR ) 
			&& PcItem->ItemTable && PcItem->ItemTable->theMaxEndurance &&						
			PcItem->WearTable.theEndurance * 100 / PcItem->ItemTable->theMaxEndurance <= 50 )
		{
			int li_time = ( timeGetTime() % 2000 ) / 1000;
			
			///-- 장착 불가
			if( li_time == 1 )
			{					
				li_color_red = 255;
				li_color_green = 128;
				li_color_blue = 128;					
			}			
		}
		
		///-- Render Icon
		
		if(!IsScaleTrans)
		{
			Render->Draw( PcItem->Tid, Cx + m_Bx, Cy + m_By, Cw, Ch, 
				    D3DCOLOR_XRGB( li_color_red, li_color_green, li_color_blue ) );
		}
		else
		{
			Render->ScretchDraw( PcItem->Tid, Cx + m_Bx, Cy + m_By, Cw, Ch, 
				    D3DCOLOR_XRGB( li_color_red, li_color_green, li_color_blue ) );
		}

		if( Group == n_SGROUP_STORE )
		{
			if( PcItem->Amount < 255 )
			{
				//아이템의 갯수 표시( 번들 아이템일경우 )
				Rsprintf( RWCHAR(Text), _RT("%d"), PcItem->Amount );
				SetAlignText();
				Render->DrawAlignText( theFontg, Text, Ax, Ay, Tx, Ty, Tw, Th, Tcolor );
			}
		}
		else
		{
#ifdef PREMIUMZONE_KEY
			if( PcItem->Amount > 1 && ( PcItem->ItemTable->theCode < 21560 || PcItem->ItemTable->theCode > 21563 ) )
#else
			if( PcItem->Amount > 1 )
#endif
			{
				//아이템의 갯수 표시( 번들 아이템일경우 )
				Rsprintf( RWCHAR(Text), _RT("%d"), PcItem->Amount );
				SetAlignText();
				Render->DrawAlignText( theFontg, Text, Ax, Ay, Tx, Ty, Tw, Th, Tcolor );
			}
		}
		
	}

	if( ( HLDuration == false ) && ( pDT->CurFocusWnd != Iid ) )
	{
		HLState = n_NONE_HIGHLIGHT;
	}
	
	if( IsHighlight )
	{
		if(!IsScaleTrans)
		{
			Render->Draw( HLImg[HLState], Cx + m_Bx, Cy + m_By, Cw, Ch, D3DCOLOR_XRGB( 255, 255, 255 ) );
		}
		else
		{
			Render->ScretchDraw( HLImg[HLState], Cx+ m_Bx, Cy + m_By, Cw, Ch, D3DCOLOR_XRGB( 255, 255, 255 ) );
		}
	}


	//
	if( ( PcItem != NULL ) && ( PcItem->IsDelay == true ) )
	{				
		float fRatio = ( PcItem->CumulativeTime / PcItem->ProgressTime );
		
		if( PcItem->IType == n_ITYPE_USE && PcItem->ItemTable )
		{			
			BYTE delayGroup = PcItem->ItemTable->theDelayGroup;
			fRatio = nRui->cumItemDelayTime[delayGroup] / nRui->realItemDelayTime[delayGroup];
		}		
		
		if( fRatio > 1.0f )
		{
			fRatio = 1.0f;
		}
		
		PcItem->STNum =  (int)(55 * fRatio);
		
		if( IsDelayShow ) 
		{
			if( !IsScaleTrans )
			{
				Render->Draw( TID_ST_START + PcItem->STNum, Cx, Cy, Cw, Ch, D3DCOLOR_XRGB( 255, 255, 255 ) );	
			}
			else
			{
				Render->ScretchDraw( TID_ST_START + PcItem->STNum, Cx, Cy, Cw, Ch, D3DCOLOR_XRGB( 255, 255, 255 ) );				
			}
		}
		
		CheckDelay();

		//By simwoosung 스킬관련 부분만 이곳에서 시간누적 
		//아이템은 다른부분에서 시간누적 시켜준다. 
		/*if(PcItem->IType == n_ITYPE_SKILL) 
		{
		  PcItem->CumulativeTime += nRui->Desktop.Tflag.ElapseTime;//경과 시간 누적
		}*/
		
	
	}
}

void CSlot::CheckDelay()
{
	if( PcItem->CumulativeTime > PcItem->ProgressTime )
	{
		//경과시간이 딜레이 타임보다 크다면...초기화
		PcItem->DelayTime		= 0.0f;
		PcItem->ProgressTime	= 0.0f;
		PcItem->CumulativeTime	= 0.0f;
		PcItem->STNum			= 0;
		PcItem->IsDelay			= false;
	}
}

//-----------------------------------------------------------------------------
IRESULT CSlot::ReceiveEvent( NEventMessage Event, EPARAM fParam, EPARAM sParam, EPARAM xParam, EPARAM exParam )
{
	return 0;
}

//-----------------------------------------------------------------------------
void CSlot::SetAlignText()
{
	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;
	Tw = Cw - ( BlankRight + BlankLeft );
	Th = Ch - ( BlankBottom + BlankTop );

	theFontg->GetStringInfo( Text, &StrInfo );
	Ax = GetAlignPosX();
	Ay = GetAlignPosY();
}

//-----------------------------------------------------------------------------
void CSlot::SetAdjustPosition( int PosX, int PosY )//OR
{
	//*
	Tx = Cx + BlankLeft;
	Ty = Cy + BlankTop;
	Tw = Cw - ( BlankRight + BlankLeft );
	Th = Ch - ( BlankBottom + BlankTop );

	theFontg->GetStringInfo( Text, &StrInfo );
	Ax = GetAlignPosX();
	Ay = GetAlignPosY();
	//*/
}

//-----------------------------------------------------------------------------
void CSlot::UpdateHandler()
{
}

//-----------------------------------------------------------------------------
void CSlot::OnMouseFocus( int Mx, int My )
{
}

//-----------------------------------------------------------------------------
void CSlot::OnMouseLButtonDown( int Mx, int My )
{
	if(IsButtonSlot)
	{
	   m_Bx = 1;
	   m_By = 1;
	}
}

//-----------------------------------------------------------------------------
void CSlot::OnMouseLButtonPress( int Mx, int My )
{

}

//-----------------------------------------------------------------------------
void CSlot::OnMouseLButtonUp( int Mx, int My )
{
	if(IsButtonSlot)
	{
	   m_Bx = 0;
	   m_By = 0;
	}
}

//-----------------------------------------------------------------------------
void CSlot::SetDisableHighlight()
{
	IsHighlight = false;
}

//-----------------------------------------------------------------------------
void CSlot::SetDelayTimeDisplay( bool aIsDraw, DWORD aDelayTime )// 단위 : ms
{
	PcItem->IsDelay = aIsDraw;
	
	// 테이블 값은 ms단위 -> 초단위로 바꿈( ProgressTime )  : 300ms -> 0.3sec
	PcItem->DelayTime		= (float)aDelayTime * 100.0f; // ms	
	PcItem->ProgressTime	= (float)aDelayTime * 100.0f;

	PcItem->CumulativeTime	= 0.0f;
}
