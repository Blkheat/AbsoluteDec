#include "RockPCH.h"
#include "CSocialActWnd.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "MessageWndProc.h"
#include "CDelayBarWnd.h"
#include "stringmanager.h"
#include "CUserStoreSellWnd.h"



CSocialActWnd		g_SocialActWnd;


#ifdef MOTION_DANCE
	int AddFrameSizeH = 227 + 36;
#else
	int AddFrameSizeH = 227;
#endif



CSocialActWnd::CSocialActWnd()
{


	// 간청 
	m_ActInfo[n_ActIdx_Request].ActIdx = n_ActIdx_Request;
	m_ActInfo[n_ActIdx_Request].ImgTID = TID_ACT_REQUEST;
	m_ActInfo[n_ActIdx_Request].HelpID = 281; // HID_ACT_REQUEST;

	// 경례 
	m_ActInfo[n_ActIdx_Bow].ActIdx = n_ActIdx_Bow;
	m_ActInfo[n_ActIdx_Bow].ImgTID = TID_ACT_BOW;
	m_ActInfo[n_ActIdx_Bow].HelpID = 282; // HID_ACT_BOW;

	// 메롱 
	m_ActInfo[n_ActIdx_Rudeness].ActIdx = n_ActIdx_Rudeness;
	m_ActInfo[n_ActIdx_Rudeness].ImgTID = TID_ACT_RUDENESS;
	m_ActInfo[n_ActIdx_Rudeness].HelpID = 283; // HID_ACT_RUDENESS;

	// 눕기 
	m_ActInfo[n_ActIdx_Lie].ActIdx = n_ActIdx_Lie;
	m_ActInfo[n_ActIdx_Lie].ImgTID = TID_ACT_LIE;
	m_ActInfo[n_ActIdx_Lie].HelpID = 284; // HID_ACT_LIE;

	// 박수
	m_ActInfo[n_ActIdx_Clap].ActIdx = n_ActIdx_Clap;
	m_ActInfo[n_ActIdx_Clap].ImgTID = TID_ACT_CLAP;
	m_ActInfo[n_ActIdx_Clap].HelpID = 285; // HID_ACT_CLAP;

	// 안녕
	m_ActInfo[n_ActIdx_HandGreeting].ActIdx = n_ActIdx_HandGreeting;
	m_ActInfo[n_ActIdx_HandGreeting].ImgTID = TID_ACT_HANDGREETING;
	m_ActInfo[n_ActIdx_HandGreeting].HelpID = 286; // HID_ACT_HANDGREETING;

	// 수줍 
	m_ActInfo[n_ActIdx_Shy].ActIdx = n_ActIdx_Shy;
	m_ActInfo[n_ActIdx_Shy].ImgTID = TID_ACT_SHY;
	m_ActInfo[n_ActIdx_Shy].HelpID = 287; // HID_ACT_SHY;

	// 엉엉
	m_ActInfo[n_ActIdx_Cry].ActIdx = n_ActIdx_Cry;
	m_ActInfo[n_ActIdx_Cry].ImgTID = TID_ACT_CRY;
	m_ActInfo[n_ActIdx_Cry].HelpID = 288; // HID_ACT_CRY;

	// 인사
	m_ActInfo[n_ActIdx_Compliments].ActIdx = n_ActIdx_Compliments;
	m_ActInfo[n_ActIdx_Compliments].ImgTID = TID_ACT_COMPLIMENTS;
	m_ActInfo[n_ActIdx_Compliments].HelpID = 289; // HID_ACT_COMPLIMENTS;

	// 저기
	m_ActInfo[n_ActIdx_There].ActIdx = n_ActIdx_There;
	m_ActInfo[n_ActIdx_There].ImgTID = TID_ACT_THERE;
	m_ActInfo[n_ActIdx_There].HelpID = 290; // HID_ACT_THERE;

	// 춤 
#ifdef MOTION_DANCE
	m_ActInfo[n_ActIdx_Dance].ActIdx = 22;
	m_ActInfo[n_ActIdx_Dance].ImgTID = TID_ACT_DANCE;
	m_ActInfo[n_ActIdx_Dance].HelpID = 291; // HID_ACT_DANCE;
#else
	// 앉기 
	m_ActInfo[n_ActIdx_Dance].ActIdx = n_ActIdx_SitDown ;
	m_ActInfo[n_ActIdx_Dance].ImgTID = TID_ACT_SITDOWN;
	m_ActInfo[n_ActIdx_Dance].HelpID = 299; // HID_ACT_SITDOWN;

#endif

	// 키스
	m_ActInfo[n_ActIdx_Kiss].ActIdx = n_ActIdx_Kiss ;	// 댄스가 빠지는 바람에 임시로...
	m_ActInfo[n_ActIdx_Kiss].ImgTID = TID_ACT_KISS;
	m_ActInfo[n_ActIdx_Kiss].HelpID = 292; // HID_ACT_KISS;

	// 하하
	m_ActInfo[n_ActIdx_HaHa].ActIdx = n_ActIdx_HaHa ;
	m_ActInfo[n_ActIdx_HaHa].ImgTID = TID_ACT_HAHA;
	m_ActInfo[n_ActIdx_HaHa].HelpID = 293; // HID_ACT_HAHA;

	// 감사 
	m_ActInfo[n_ActIdx_Thank].ActIdx = n_ActIdx_Thank;
	m_ActInfo[n_ActIdx_Thank].ImgTID = TID_ACT_THANK;
	m_ActInfo[n_ActIdx_Thank].HelpID = 294; // HID_ACT_THANK;

	// 공격 
	m_ActInfo[n_ActIdx_Attack].ActIdx = n_ActIdx_Attack ;
	m_ActInfo[n_ActIdx_Attack].ImgTID = TID_ACT_ATTACK;
	m_ActInfo[n_ActIdx_Attack].HelpID = 295; // HID_ACT_ATTACK;

	// 응 
	m_ActInfo[n_ActIdx_Nod].ActIdx = n_ActIdx_Nod ;
	m_ActInfo[n_ActIdx_Nod].ImgTID = TID_ACT_NOD;
	m_ActInfo[n_ActIdx_Nod].HelpID = 296; // HID_ACT_NOD;

	// 아니 
	m_ActInfo[n_ActIdx_No].ActIdx = n_ActIdx_No ;
	m_ActInfo[n_ActIdx_No].ImgTID = TID_ACT_NO;
	m_ActInfo[n_ActIdx_No].HelpID = 297; // HID_ACT_NO;

	// 대기  
	m_ActInfo[n_ActIdx_Wait].ActIdx = n_ActIdx_Wait;
	m_ActInfo[n_ActIdx_Wait].ImgTID = TID_ACT_WAIT;
	m_ActInfo[n_ActIdx_Wait].HelpID = 298; // HID_ACT_WAIT;

	// 앉기 
	m_ActInfo[n_ActIdx_SitDown].ActIdx = n_ActIdx_SitDown ;
	m_ActInfo[n_ActIdx_SitDown].ImgTID = TID_ACT_SITDOWN;
	m_ActInfo[n_ActIdx_SitDown].HelpID = 299; // HID_ACT_SITDOWN;

	
	m_RideActInfo[n_ActIdx_Ride_Sudden2].ActIdx = 22;			//모션테이블 인덱스 .. 
	m_RideActInfo[n_ActIdx_Ride_Sudden2].ImgTID = TID_ride_kicking;
	m_RideActInfo[n_ActIdx_Ride_Sudden2].HelpID = 300; // HID_ACT_HORSE_FOREFOOT;
	
	m_RideActInfo[n_ActIdx_Ride_None1].ActIdx = 22;			//모션테이블 인덱스 .. 
	m_RideActInfo[n_ActIdx_Ride_None1].ImgTID = TID_ride_blank;
	
	
	m_RideActInfo[n_ActIdx_Ride_None2].ActIdx = 22;			//모션테이블 인덱스 .. 
	m_RideActInfo[n_ActIdx_Ride_None2].ImgTID = TID_ride_blank;


//
//	if(g_Pc.GetPlayer()->m_State.bRide)
//		{
//			g_Pc.GetPlayer()->AnimationPlay(n_Ride_Sudden2);
//			g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].AnimationPlay(n_NPC_Attack0);
//		}


	m_StartDrawSlotNum = 0;


}

CSocialActWnd::~CSocialActWnd()
{
}

void CSocialActWnd::Init()
{
	

}

void CSocialActWnd::Composition()
{
	
	CImageBox * img = NULL;

	//by simswoosung NextButton과 PrevButton을 아직 사용안하시는듯해서
	//프레임 포커스 영역의 높이를 조정합니다.	
	//.........................................................................................................
	// FrameWnd
	//.........................................................................................................
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
				   279, 67, 117, AddFrameSizeH  , false );
	m_pFrameWnd->SetClientImage( TID_None );
	m_pFrameWnd->SetCaption( _RT( "Social Action Window" ) );
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetTitle( false );
	m_pFrameWnd->SetMove( false );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->SetZorder( n_zoTop );
	m_pFrameWnd->Initialize();

	///-- BG Image9
	m_BGImg = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, m_pFrameWnd->GetID(), 0, 0, 117 , AddFrameSizeH , false );	
	m_BGImg->SetWndTileSizeLR( 5, 5 );
	m_BGImg->SetWndTileSizeTB( 5, 5 );
	m_BGImg->SetFocusCheck( false );
	m_BGImg->SetClientImage( TID_SOCIALACT_BG_L1 );
	m_BGImg->SetAdjustPosition( 0, 0 );
	
	
	int slot_num = 0;

	//말 빈공간 이미지 .. 3개 뿌림 
		//추후에 라이딩 모션 추가 를 위해 라이딩쪽은 새로운 슬롯으로 ... 난 복잡한것을 거부한다 ...

	//추후에 라이딩 모션 추가 를 위해 라이딩쪽은 새로운 슬롯으로 ... 난 복잡한것을 거부한다 ...

	int xpos = 5;
	int ypos = 4;

	for( slot_num = 0 ; slot_num < c_Max_Draw_RideActSlot_Num ; ++slot_num)
	{
		if( xpos > 79 )
		{
			xpos = 5;
			ypos += 37; 
		}
		

		m_RideActSlot[slot_num].ActImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), xpos, ypos, 34, 34, false );	
		m_RideActSlot[slot_num].ActImg->SetClientImage( m_RideActInfo[slot_num].ImgTID );		
		m_RideActSlot[slot_num].ActImg->SetFocusCheck( false );

		//빈 공백이 아니면 .. 
	
		
		m_RideActSlot[slot_num].ActBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), xpos, ypos, 34, 34, false );	
		m_RideActSlot[slot_num].ActBtn->SetBtnImage( TID_None, TID_SOCIALACT_BTN_FOCUS, TID_SOCIALACT_BTN_CLICK, TID_None );
		m_RideActSlot[slot_num].ActBtn->SetFontR( n_fRsvFontWndButton );   
		m_RideActSlot[slot_num].ActBtn->SetText( _RT("") );	
		m_RideActSlot[slot_num].ActBtn->SetHID( m_RideActInfo[slot_num].HelpID );				
		
			
		m_RideActSlot[slot_num].ActInfoIdx = slot_num;

		xpos += 37; 
	}

		  
	xpos = 5;
	ypos = 4;
	for( slot_num = 0; slot_num < c_Max_Draw_SocialActSlot_Num; ++slot_num )
	{
		if( xpos > 79 )
		{
			xpos = 5;
			ypos += 37; 
		}
	
		m_ActSlot[slot_num].ActImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), xpos, ypos, 34, 34, false );	
		m_ActSlot[slot_num].ActImg->SetClientImage( m_ActInfo[slot_num].ImgTID );		
		m_ActSlot[slot_num].ActImg->SetFocusCheck( false );

		m_ActSlot[slot_num].ActBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), xpos, ypos, 34, 34, false );	
		m_ActSlot[slot_num].ActBtn->SetBtnImage( TID_None, TID_SOCIALACT_BTN_FOCUS, TID_SOCIALACT_BTN_CLICK, TID_None );
		m_ActSlot[slot_num].ActBtn->SetFontR( n_fRsvFontWndButton );   
		m_ActSlot[slot_num].ActBtn->SetText( _RT("") );	
		m_ActSlot[slot_num].ActBtn->SetHID( m_ActInfo[slot_num].HelpID );				

		m_ActSlot[slot_num].ActInfoIdx = slot_num;

		xpos += 37; 
	}	


//
	m_pPrevPageBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 77, 227, 20, 18, false );	
	m_pPrevPageBtn->SetBtnImage( TID_SOCIALACT_PREV_NORMAL, TID_SOCIALACT_PREV_FOCUS, TID_SOCIALACT_PREV_CLICK, TID_SOCIALACT_PREV_CLICK );
	m_pPrevPageBtn->SetFontR( n_fRsvFontWndButton );   
	m_pPrevPageBtn->SetText( _RT("") );
	m_pPrevPageBtn->SetVisible(false );

//
	m_pNextPageBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 97, 227, 20, 18, false );	
	m_pNextPageBtn->SetBtnImage( TID_SOCIALACT_NEXT_NORMAL, TID_SOCIALACT_NEXT_FOCUS, TID_SOCIALACT_NEXT_CLICK, TID_SOCIALACT_NEXT_CLICK );
	m_pNextPageBtn->SetFontR( n_fRsvFontWndButton );   
	m_pNextPageBtn->SetText( _RT("") );	
	m_pNextPageBtn->SetVisible(false);

	return;

}


void CSocialActWnd::Update()
{
	
/*
	if( m_StartDrawSlotNum <= 0 )
	{
		m_StartDrawSlotNum = 0;
		
		m_pPrevPageBtn->SetBtnHold( true );
//		m_pPrevPageBtn->SetVisible( false );
	}
	else if( m_StartDrawSlotNum > c_Max_Draw_SocialActSlot_Num )
	{
		if( ( m_StartDrawSlotNum + c_Max_Draw_SocialActSlot_Num ) >= c_Max_SocialActSlot_Num )
		{
			m_pNextPageBtn->SetBtnHold( true );
//			m_pNextPageBtn->SetVisible( false )
		}
	}	
*/
	//	g_Map.m_Npc[g_Pc.GetPlayer()->m_State.TransIndex].AnimationPlay(n_NPC_Attack0);
	
	//말을 타면 사교동작 이모션들이 사라진다 
	
	
	for(int slot_num=0 ; slot_num <  c_Max_Draw_RideActSlot_Num ; ++slot_num )
	{
		if( g_Pc.GetPlayer()->m_State.bRide) 		
		{
			if(m_RideActInfo[slot_num].ImgTID != TID_ride_blank)
			{
				m_RideActSlot[slot_num].ActBtn->SetVisible(true);
			}
			m_RideActSlot[slot_num].ActImg->SetVisible(true);
		
		}
		else 
		{
			m_RideActSlot[slot_num].ActBtn->SetVisible(false);
			m_RideActSlot[slot_num].ActImg->SetVisible(false);
		}

	}

	for( short slot_num=0 ; slot_num <  c_Max_Draw_SocialActSlot_Num ; ++slot_num )
	{
		if( g_Pc.GetPlayer()->m_State.bRide) 		
		{	
			
			m_pFrameWnd->SetClientSize(117,40);
			m_BGImg->SetClientSize(117,40);
			

			m_ActSlot[slot_num].ActBtn->SetVisible(false);
			m_ActSlot[slot_num].ActImg->SetVisible(false);
			
		}
		else 
		{
			m_pFrameWnd->SetClientSize(117,AddFrameSizeH );
			m_BGImg->SetClientSize(117,AddFrameSizeH);
			m_ActSlot[slot_num].ActBtn->SetVisible(true);
			m_ActSlot[slot_num].ActImg->SetVisible(true);
		}
	}
		
	//말을 타면 말전용 이모션이 나옴 .. 캬하하 
	//현재 동작은 말 1개 만 나온 상태에 추후 나오면 숫자 와 디판인을 추가해줘야함 .. 



	if( g_UserStoreSellWnd.IsVisible )
	{
		Close();
	}

	m_pPrevPageBtn->SetVisible( false );
	m_pNextPageBtn->SetVisible( false );

	
	return;
}


void CSocialActWnd::Proc( SEventMessage* EMsg )
{	
	int slot_num = 0;
	
	for( slot_num = 0; slot_num < c_Max_Draw_SocialActSlot_Num; ++slot_num )
	{	
		if( EMsg->FocusWnd == m_ActSlot[slot_num].ActBtn->GetID() )
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{	
				if( g_Pc.GetSkillSystem()->theIsUsedSkill )
				{
					return;
				}

				if( g_DelayBarWnd.IsVisible && DELAY_RIDE_ON == g_DelayBarWnd.GetDelay_Type() )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANNOT_THEFUNCTION_INRIDING ) );
					return;
				}
			
				#ifdef MOTION_DANCE
				if( n_ActIdx_Dance == slot_num )
				{
					if( g_Pc.GetDanceMotion( &g_Pc.m_MainPC ) == -1 ) 
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_AFTER_CLASSUP_USE ) );
						return;
					}
				}
				#endif

	
				switch( g_Pc.GetPlayer()->m_curt_event.type )
				{
					case NULL:
					case SM_SIMSIM:
					case SM_ACT:
						g_RockClient.Send_CsExpression( g_Pc.GetPlayer()->GetUnique(), ( BYTE ) m_ActInfo[ m_ActSlot[slot_num].ActInfoIdx ].ActIdx + 1 );
						break;
				}
			}
		}		
	}	

	for( slot_num = 0; slot_num < c_Max_Draw_RideActSlot_Num; ++slot_num )
	{	
		if( EMsg->FocusWnd == m_RideActSlot[slot_num].ActBtn->GetID() )
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{	
				if( g_Pc.GetPlayer()->m_State.bRide )
				{
					SNPCBaseInfo* npc_Info = NULL;
					DWORD dNPCCode = g_Map.m_NpcTable[ g_Pc.GetPlayer()->m_State.TransIndex ].code;
					npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

					if( !npc_Info )
					{
						return;
					}

					///-- 말이 아닐경우
					if( npc_Info->theTargetRange != 0 )
					{
						return;
					}
				}				
				
				if(m_RideActInfo[slot_num].ImgTID != TID_ride_blank)
				{	
					switch( g_Pc.GetPlayer()->m_curt_event.type )
					{
					case NULL:
					case SM_SIMSIM:
					case SM_ACT:
						g_RockClient.Send_CsExpression( g_Pc.GetPlayer()->GetUnique(), ( BYTE ) m_RideActInfo[ m_RideActSlot[slot_num].ActInfoIdx ].ActIdx );
						break;
					}
				}
			}
		}		
	}	






	if( EMsg->FocusWnd == m_pPrevPageBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			m_StartDrawSlotNum -= c_Max_Draw_SocialActSlot_Num;

			if( m_StartDrawSlotNum <= 0 )
			{
				m_StartDrawSlotNum = 0;

				m_pPrevPageBtn->SetBtnHold( true );

				// SetActSlot(); 
			}
		}
	}
	else if( EMsg->FocusWnd == m_pNextPageBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			m_StartDrawSlotNum += c_Max_Draw_SocialActSlot_Num;

			if( m_StartDrawSlotNum >= c_Max_SocialActSlot_Num )
			{
				m_StartDrawSlotNum -= c_Max_Draw_SocialActSlot_Num;				

				m_pNextPageBtn->SetBtnHold( true );

				// SetActSlot(); 
			}
		}
	}



	return;
}

void CSocialActWnd::Open()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );			

	return;
}

void CSocialActWnd::Close()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );			

	return;
}

void CSocialActWnd::SetActSlot()
{
	return;
}
