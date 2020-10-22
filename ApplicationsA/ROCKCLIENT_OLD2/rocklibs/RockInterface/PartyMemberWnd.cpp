#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

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
#include "Wnd.h"

#include	"..\\..\\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>

#include "..\\..\\GTimer.h"
#include	"..\\..\\Pc.h"
#include "..\\..\\global.h"

#include "UiUtil.h"
#include "NonPlayerInfoWnd.h"

#include "UIMsgWnd.h"
#include "PartyInforWnd.h"
#include "PartyPopupWnd.h"
#include "LeagueInfoWnd.h"
#include "PartyMemberWnd.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

CPartyMemeberWnd::CPartyMemeberWnd()
{
	m_pPartyMemWnd = NULL;
	m_pPartyMemSelectImg = NULL;
	
	m_pPartyMemCharImgBtn = NULL;		
	m_pPartyMemMasterImg = NULL ;		
	m_pPartyMemFKeyTypeImg = NULL;		
	m_pPartyMemLevelTxt = NULL;		
	m_pPartyMemComboImg[0] = NULL;
	m_pPartyMemComboImg[1] = NULL;	
	m_pPartyMemComboImg[2] = NULL;	
	m_pPartyMemComboImg[3] = NULL;	
	m_pPartyMemComboImg[4] = NULL;	
	m_pPartyMemComboImg[5] = NULL;	
	
	m_pPartyMemNameTxt = NULL;					
    m_pPartyMemHPGauge = NULL;				
	m_pPartyMemSPMPGauge = NULL;			

	m_pMemberInfo = NULL;

	m_HPText = NULL;
	m_SMPText = NULL;
	memset(m_tmpStr , NULL , MAX_SIZE_128 );
}

CPartyMemeberWnd::~CPartyMemeberWnd()
{

}

void CPartyMemeberWnd::Init()
{

}

void CPartyMemeberWnd::Composition()
{

}

void CPartyMemeberWnd::Update()
{
	if(m_pMemberInfo)
	{
		static TCHAR tmpStr[128] = {0,};

		SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
		
		UpdateMemberUnitInfo();
		
		TEXID enLevelID[6] = 
		{
			TID_PARTY_MEMBER_COMBO_ENLEVEL1,		
			TID_PARTY_MEMBER_COMBO_ENLEVEL2,		
			TID_PARTY_MEMBER_COMBO_ENLEVEL3,		
			TID_PARTY_MEMBER_COMBO_ENLEVEL4,		
			TID_PARTY_MEMBER_COMBO_ENLEVEL5,		
			TID_PARTY_MEMBER_COMBO_ENLEVEL6,	
		};		
		
		int iFindIndex = -1;
		for(int i = 0 ; i < MAX_USER_INTABLE;  i++)
		{
			if( g_Pc.m_PcTable[i].char_info.thePCID == m_pMemberInfo->m_dPCID)         
			{
				iFindIndex = i;
				break;
			}
		}

		m_pPartyMemNameTxt->SetText(m_pMemberInfo->m_strPCName);	
		m_pPartyMemNameTxt->SetLimitLength(RP_MAX_PC_NAME - sizeof(RTCHAR) );		

		if(iFindIndex != -1)
		{
			if(m_pMemberInfo)
				m_pMemberInfo->m_bLevel = g_Pc.m_PcTable[iFindIndex].char_info.theLevel;
			
			Rsprintf( RWCHAR(m_tmpStr), _RT("%d"), g_Pc.m_PcTable[iFindIndex].char_info.theLevel );
			m_pPartyMemLevelTxt->SetText(m_tmpStr);

			int percent = ((float)g_Pc.m_PcTable[iFindIndex].char_info.theHP / 
						   (float)g_Pc.m_PcTable[iFindIndex].char_info.theMaxHP) * 100 ;
			
			if( ( 0 < percent) &&  ( percent< 30 ) )
			{
				m_pPartyMemWnd->SetActiveFlash(true);
			}
			else
			{
				m_pPartyMemWnd->SetActiveFlash(false);
			}
			
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), g_Pc.m_PcTable[iFindIndex].char_info.theHP  , g_Pc.m_PcTable[iFindIndex].char_info.theMaxHP );
			m_HPText->SetText(tmpStr);
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d/%d"), g_Pc.m_PcTable[iFindIndex].char_info.theSMP, g_Pc.m_PcTable[iFindIndex].char_info.theMaxSMP );
			m_SMPText->SetText(tmpStr);



			m_pPartyMemHPGauge->SetPercent( percent );
			m_pPartyMemSPMPGauge->SetPercent( ((float)g_Pc.m_PcTable[iFindIndex].char_info.theSMP / 
				(float)g_Pc.m_PcTable[iFindIndex].char_info.theMaxSMP) * 100 );	
			
			int Gage = m_pMemberInfo->m_bGage;
			
			SetComboGauge(Gage, TRUE);
			
			//사용가능한 콤보레벨 표시하기
			int nEnableSkillLevel = ::GetNSkillLevel(m_pMemberInfo->m_bMaxGage);
			
			for(int i = 0 ; i < 6 ; i++)
			{
				if( i < nEnableSkillLevel )
				{
					m_pPartyMemComboImg[i]->SetClientBgImage(enLevelID[i]);				
				}
				else
				{
					m_pPartyMemComboImg[i]->SetClientBgImage(TID_None);				
				}
			}	
		}
		else
		{
			m_pPartyMemWnd->SetActiveFlash(false);
			
			Rsprintf( RWCHAR(m_tmpStr), _RT("%d"), m_pMemberInfo->m_bLevel );
			m_pPartyMemLevelTxt->SetText(m_tmpStr);	
			
			m_pPartyMemHPGauge->SetPercent(0);
			m_pPartyMemSPMPGauge->SetPercent(0);
			m_HPText->SetText(_RT("?"));
			m_SMPText->SetText(_RT("?"));
			
			SetComboGauge(0, FALSE);
		}	
	}
}

void CPartyMemeberWnd::UpdateMemberUnitInfo()
{
	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();
	
	SRect WcSize;
	
	m_pPartyMemCharImgBtn->GetClientSize( &WcSize );
	m_pPartyMemCharImgBtn->SetClientImage( TID_None );
	m_pPartyMemCharImgBtn->SetBtnImage( TID_None, TID_None, 
									   TID_None, TID_None );
	m_pPartyMemCharImgBtn->SetHID(HID_None);
	m_pPartyMemCharImgBtn->SetIsCharImgRender(false);

	m_pPartyMemMasterImg->SetVisible(FALSE);

	m_pPartyMemHPGauge->SetVisible(TRUE);
	m_pPartyMemSPMPGauge->SetVisible(TRUE);

	if(m_pMemberInfo)
	{
		m_pPartyMemCharImgBtn->SetClientImage(TID_None);
		
		//가시영역에 있고 로그아웃중이 아니라면
		if(	(m_pMemberInfo->m_bIsViewRange) &&  !(m_pMemberInfo->m_bIsLogOut) )
		{
			m_pPartyMemCharImgBtn->SetClientImage(TID_None);
			m_pPartyMemCharImgBtn->SetIsCharImgRender(true);
			m_pPartyMemCharImgBtn->SetRenderCharImgProperty((WcSize.x), (WcSize.y),
			m_pMemberInfo->m_dPCID, c_PcRace_User,  CHARIMAGE_32SIZE );			
		}
		else
		{
			m_pPartyMemCharImgBtn->SetClientImage(TID_PARTY_MEMBER_NOSEE);	
		}

		if(m_pMemberInfo->m_bRecon)
		{
			m_pMemberInfo->m_bRecon++;
			if( 3 == m_pMemberInfo->m_bRecon)
			{	
				long lunique = (long)m_pMemberInfo->m_dPCID;	
				PCTABLE *pPcTable = g_Pc.FindPcTable(lunique);
				if(pPcTable)
				{			
					if( g_PartyInforWnd.MakePCharImage(m_pMemberInfo->m_dPCID) )
					{
						m_pMemberInfo->m_bIsViewRange = TRUE;
						m_pMemberInfo->m_bIsSee = TRUE;
						SPARTYMEMBER_INFO *pMemberInfo = 
							g_LeagueInfoWnd.FindLeagueMember(m_pMemberInfo->m_dPCID);
						if(pMemberInfo)
						{	
							pMemberInfo->m_bIsViewRange = TRUE;
							pMemberInfo->m_bIsSee = TRUE;
						}
					}
				}
				else
				{
					m_pMemberInfo->m_bIsViewRange = FALSE;					
				}
				m_pMemberInfo->m_bRecon = 0;
			}
		}		
		
		//로그오프일 경우
		if( m_pMemberInfo->m_bIsLogOut )
		{
		   m_pPartyMemCharImgBtn->SetClientImage(TID_PARTY_MEMBER_LOGOFF);			
		}
		
		//방장여부
		if(pPartyInfo->m_dLeaderID == m_pMemberInfo->m_dPCID)
		{
			m_pPartyMemMasterImg->SetVisible(TRUE);
		}	
	}

}

void CPartyMemeberWnd::SetComboGauge(int iGage, BOOL BFlag)
{
  if(!BFlag)
  {
     m_pPartyMemComboImg[0]->SetClientImage( TID_None );
	 m_pPartyMemComboImg[1]->SetClientImage( TID_None );
	 m_pPartyMemComboImg[2]->SetClientImage( TID_None );
	 m_pPartyMemComboImg[3]->SetClientImage( TID_None );
	 m_pPartyMemComboImg[4]->SetClientImage( TID_None );
     m_pPartyMemComboImg[5]->SetClientImage( TID_None );
	 return;
  } 
  
  int level = 0;

  // 총 게이지 칸 => 5(1) + 5(2) + 5(3) + 5(4) + 5(5) + 5(Ex) = 30 
  if( iGage >= 5 && iGage < 10 ) level = 1;
  else if( iGage >= 10 && iGage < 15 ) level = 2;
  else if( iGage >= 15 && iGage < 20 ) level = 3;
  else if( iGage >= 20 && iGage < 25 ) level = 4;
  else if( iGage >= 25 && iGage < 30 ) level = 5;
  else if( iGage >= 30 ) level = 6;	// Ex Level

  switch( level )
	{
	case 0:
		{
			m_pPartyMemComboImg[0]->SetClientImage( TID_None );
			m_pPartyMemComboImg[1]->SetClientImage( TID_None );
			m_pPartyMemComboImg[2]->SetClientImage( TID_None );
			m_pPartyMemComboImg[3]->SetClientImage( TID_None );
			m_pPartyMemComboImg[4]->SetClientImage( TID_None );
			m_pPartyMemComboImg[5]->SetClientImage( TID_None );
			// 0 - 5
		}
		break;
	case 1:
		{
			m_pPartyMemComboImg[0]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL1 );
			m_pPartyMemComboImg[1]->SetClientImage( TID_None );
			m_pPartyMemComboImg[2]->SetClientImage( TID_None );
			m_pPartyMemComboImg[3]->SetClientImage( TID_None );
			m_pPartyMemComboImg[4]->SetClientImage( TID_None );
			m_pPartyMemComboImg[5]->SetClientImage( TID_None );
			// 5 - 10
		}
		break;
	case 2:
		{
			m_pPartyMemComboImg[0]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL1 );
			m_pPartyMemComboImg[1]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL2 );
			m_pPartyMemComboImg[2]->SetClientImage( TID_None );
			m_pPartyMemComboImg[3]->SetClientImage( TID_None );
			m_pPartyMemComboImg[4]->SetClientImage( TID_None );
			m_pPartyMemComboImg[5]->SetClientImage( TID_None );
			// 10 - 15
		}
		break;
	case 3:
		{
			m_pPartyMemComboImg[0]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL1 );
			m_pPartyMemComboImg[1]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL2 );
			m_pPartyMemComboImg[2]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL3 );
			m_pPartyMemComboImg[3]->SetClientImage( TID_None );
			m_pPartyMemComboImg[4]->SetClientImage( TID_None );
			m_pPartyMemComboImg[5]->SetClientImage( TID_None );
			// 15 - 20
		}
		break;
	case 4:
		{
			m_pPartyMemComboImg[0]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL1 );
			m_pPartyMemComboImg[1]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL2 );
			m_pPartyMemComboImg[2]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL3 );
			m_pPartyMemComboImg[3]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL4 );
			m_pPartyMemComboImg[4]->SetClientImage( TID_None );
			m_pPartyMemComboImg[5]->SetClientImage( TID_None );
			// 20 - 25
		}
		break;
	case 5:
		{
			m_pPartyMemComboImg[0]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL1 );
			m_pPartyMemComboImg[1]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL2 );
			m_pPartyMemComboImg[2]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL3 );
			m_pPartyMemComboImg[3]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL4 );
			m_pPartyMemComboImg[4]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL5 );
			m_pPartyMemComboImg[5]->SetClientImage( TID_None );
			// 25 - 30
		}
		break;
	case 6:
		{
			m_pPartyMemComboImg[0]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL1 );
			m_pPartyMemComboImg[1]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL2 );
			m_pPartyMemComboImg[2]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL3 );
			m_pPartyMemComboImg[3]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL4 );
			m_pPartyMemComboImg[4]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL5 );
			m_pPartyMemComboImg[5]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL6 );
     		// 30 - 
		}
		break;
	default:
		break;
	}  

}

void CPartyMemeberWnd::Proc( SEventMessage* EMsg )
{
	CWnd* Wnd = NULL;
	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();

	if( m_pPartyMemWnd->GetID() == EMsg->FocusWnd )
	{
       //멤버윈도우에 멤버가 없으면 툴팁이 표시가 안된다.
	   if(m_pMemberInfo && !m_pMemberInfo->m_bIsLogOut)		
	   {
		   ::GetAreaStrName(m_tmpStr, (UINT)(m_pMemberInfo->m_wMap));
		   g_PartyInforWnd.SetFocusMemberWnd( EMsg->FocusWnd
	   								          ,m_tmpStr );
	   }	   
	   
	   // 멤버윈도우 선택	   
	   if( EMsg->Notify == NM_FWND_CLICK )
	   {
		  for(int i = 0 ; i < g_PartyInforWnd.GetTotPartyMemNumber() ; i++)
		  {		
		   	 if( i == m_nIndex )
			 {
				g_PartyInforWnd.m_PartyMemberWnd[i].m_pPartyMemSelectImg->SetVisible(true);
			 }
			 else
			 {
				g_PartyInforWnd.m_PartyMemberWnd[i].m_pPartyMemSelectImg->SetVisible(false);	
			 }		
		  }

		  if(m_pMemberInfo) //멤버가 있는 창이면
		  {
				int nUserIndex = g_Pc.FindUser(m_pMemberInfo->m_dPCID);
				if( nUserIndex == MAX_USER_INTABLE )
				{					
					return;
				}
			
				if( NonPlayerInfoWnd.thePickedUserID != nUserIndex )		//현재 픽킹이 선택이 되어있지않으면..
				{
					g_Pc.m_nPickedUser = nUserIndex;
				
					TCHAR strTemp[256] = {0,};
					nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );				
					Wnd = Container->Get( WID_NonPlayerInfo_LevelText );
					Rsprintf( RWCHAR(strTemp), _RT("%d"), g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.theLevel );
					Wnd->ReceiveEvent(n_emSetText, (EPARAM)strTemp, 240, 240, 200 );		
					Wnd = Container->Get( WID_NonPlayerInfo_NameText );
					Wnd->ReceiveEvent(n_emSetText, (EPARAM)g_Pc.m_PcTable[g_Pc.m_nPickedUser].char_info.thePCName, 240, 240, 200 );				

					NonPlayerInfoWnd.thePickedNpcID = -1;
					NonPlayerInfoWnd.thePickedUserID = g_Pc.m_nPickedUser;				
					NonPlayerInfoWnd.SetPickCharacter( ( Character * )&g_Pc.m_Pc[g_Pc.m_nPickedUser] );
					
					g_PartyInforWnd.m_nSelectMemberIndex = g_Pc.m_nPickedUser;
				}
				else  //현재 픽킹이 선택되어 있으면....		
				{
					
					RLG1( "g_Pc.GetPlayer()->m_curt_event.type = %d", g_Pc.GetPlayer()->m_curt_event.type);
					
					if( (SM_SLIDE_NPC != g_Pc.GetPlayer()->m_curt_event.type) &&
						(SM_STATE_DIE != g_Pc.GetPlayer()->m_curt_event.type ) &&
						//(SM_ATTACK_DELAY != g_Pc.GetPlayer()->m_curt_event.type ) &&
						//(SM_MOVE_ATTACK != g_Pc.GetPlayer()->m_curt_event.type ) &&
						(SM_ATTACK_NPC != g_Pc.GetPlayer()->m_curt_event.type) && //현재 공격중이 아니고....										
					    NonPlayerInfoWnd.IsVisible ) //NPC창이 보여있으면	
					{					
						
						//이동 부가 루틴이 적용되지 않은 경우에만
					//	if( 1 != g_Pc.GetPlayer()->theAddEffect_State[n_TypeNotMove] )
						if( !g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotMove) )
						{
							if( ( SM_USE_SKILL != g_Pc.GetPlayer()->m_curt_event.type )	//스킬이 아니거나	
								||
								( SM_USE_SKILL == g_Pc.GetPlayer()->m_curt_event.type  //스킬이어도 앉는 스킬이라면...
							      && ( SM_RESTSKILL_SITDOWN == g_Pc.GetPlayer()->m_curt_event.status ||
						               SM_RESTSKILL_KEEPING == g_Pc.GetPlayer()->m_curt_event.status ||
						               SM_RESTSKILL_STANDUP == g_Pc.GetPlayer()->m_curt_event.status ) ) )
							{
														
								//파티 대상으로 자동이동
								DWORD dDestCell = g_Pc.m_PcTable[nUserIndex].lDestBlock;
								DWORD dInputDestCell = g_Pc.GetPlayer()->GetInputedDestCell();
								
								if( (0 != dDestCell) && (INVALID_DWORD_VALUE == dInputDestCell) )
								{
									g_Pc.GetPlayer()->SetInputedDestCell(dDestCell);								
									g_Pc.GetPlayer()->Illusion_CsStartMove( dDestCell );
									g_Pc.GetPlayer()->Pet_CsStartMove( dDestCell );
								}
							}
						}
					}
				}
			}
	   }

	   if( EMsg->Notify == NM_FWND_RCLICK )
	   {
		   //해당 멤버윈도우에 멤버가 없다면 팝업동작안함
		   if(m_pMemberInfo)
		   {
			  POINT		ptCursor;
			  GetCursorPos( &ptCursor );
			  ::ScreenToClient( g_RockClient.GetApp(), &ptCursor );
			  ptCursor.x -= 5;
			  ptCursor.y -= 5;
		      
			  nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		  
		   
		      g_PartyPopupWnd.GetPartyPopupWnd()->SetAbsolutePosition(ptCursor.x, ptCursor.y);
		      //현재 파티장인지 아닌지에 따라 팝업윈도우 활성메뉴가 달라진다.
			  g_PartyPopupWnd.SetMemberInfo(m_pMemberInfo);
			  
			  bool fMwnu1, fMwnu2, fMwnu3, fMwnu4;
			  fMwnu1 = fMwnu2 = fMwnu3 = fMwnu4 = true;
			  if( g_Pc.m_MainPC.lUnique != pPartyInfo->m_dLeaderID )
			  {
				 fMwnu1 = false;				 
				 fMwnu2 = false;
			  }
  
	
			  if( m_pMemberInfo->m_bIsLogOut )
			  {
				 fMwnu3 = false;
			  }

			  if( !(m_pMemberInfo->m_bIsViewRange) )
			  {
				 fMwnu3 = false;
			  }	

			  g_PartyPopupWnd.SetActiveMemnu(fMwnu1, fMwnu2, fMwnu3, fMwnu4);

		   }   
	   }
	}	  

}

void CPartyMemeberWnd::Composition(int index, int iXPos, int iYPos)
{
   m_nIndex = index;	
   
   m_pPartyMemWnd = (CFlashFrameWnd*)nRui->CreateWnd( n_wtFlashFrameWnd, WID_None, 
				             WID_PartyInforWnd, iXPos, iYPos, 152,  53, false );
   m_pPartyMemWnd->SetClientImage( TID_PARTY_MEMBER_NORMAL );
   m_pPartyMemWnd->SetCaption( _RT("") );
   m_pPartyMemWnd->SetWndProc( this );
   m_pPartyMemWnd->SetTitle( false );
   m_pPartyMemWnd->SetVisible( false );
   m_pPartyMemWnd->SetMove( false );
   m_pPartyMemWnd->SetZorder( n_zoTop );
   m_pPartyMemWnd->Initialize();
   m_pPartyMemWnd->SetIntervalTime(0.2f);
   m_pPartyMemWnd->SetFlashImage(TID_PARTY_MEMBER_WARNING);	
   m_pPartyMemWnd->SetFlashColorOption(D3DTOP_MODULATE);
   m_pPartyMemWnd->SetFlashColor(D3DCOLOR_ARGB(63, 255, 128, 128 ));
   m_pPartyMemWnd->SetActiveFlash(false);
   m_pPartyMemWnd->SetFlashImgSize(0, 0, 152, 53); 
   
   m_pPartyMemSelectImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
                                              m_pPartyMemWnd->GetID(), 0, 0, 152,  53, false );
   m_pPartyMemSelectImg->SetClientImage( TID_PARTY_MEMBER_SELECT );
   m_pPartyMemSelectImg->SetZorder( n_zoBottom );
   m_pPartyMemSelectImg->SetFocusCheck(false);
   m_pPartyMemSelectImg->SetVisible(false);
   
   m_pPartyMemCharImgBtn = (CCharImgButton*)nRui->CreateWnd( n_wtChaImgButton, 
							 WID_None, m_pPartyMemWnd->GetID(), 5, 16, 30, 31, false );
   m_pPartyMemCharImgBtn->SetClientImage( TID_PARTY_MEMBER_NOSEE );
   m_pPartyMemCharImgBtn->SetBtnImage( TID_None, TID_None, 
									   TID_None, TID_None );  
   m_pPartyMemCharImgBtn->SetFocusCheck(false);
   m_pPartyMemCharImgBtn->SetCButtonType(CBTYPE_PARTY);

   m_pPartyMemMasterImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
                                              m_pPartyMemWnd->GetID(), 2, 5, 11, 9, false );
   m_pPartyMemMasterImg->SetClientImage( TID_PARTY_MEMBER_MASTER );
   m_pPartyMemMasterImg->SetZorder( n_zoTop );
   m_pPartyMemMasterImg->SetFocusCheck(false);

   m_pPartyMemLevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pPartyMemWnd->GetID(), 
		25, 7, 17, 12, false );	
   m_pPartyMemLevelTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
   m_pPartyMemLevelTxt->SetClientImage( TID_None );
   m_pPartyMemLevelTxt->SetAlignText( n_atRight, n_atCenter );	
   m_pPartyMemLevelTxt->SetText(_RT(""));	
   m_pPartyMemLevelTxt->SetTextColor( 255, 255, 255 );
   m_pPartyMemLevelTxt->SetFocusCheck(false);

   m_pPartyMemComboImg[0] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
                                              m_pPartyMemWnd->GetID(), 38, 44, 5, 3, false );
   m_pPartyMemComboImg[0]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL1 );
   m_pPartyMemComboImg[0]->SetFocusCheck(false);
   
   m_pPartyMemComboImg[1] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
                                              m_pPartyMemWnd->GetID(), 38, 40, 5, 3, false );
   m_pPartyMemComboImg[1]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL2 );
   m_pPartyMemComboImg[1]->SetFocusCheck(false);	

   m_pPartyMemComboImg[2] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
                                              m_pPartyMemWnd->GetID(), 38, 36, 5, 3, false );
   m_pPartyMemComboImg[2]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL3 );
   m_pPartyMemComboImg[2]->SetFocusCheck(false);
   
   m_pPartyMemComboImg[3] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
                                              m_pPartyMemWnd->GetID(), 38, 32, 5, 3, false );   
   m_pPartyMemComboImg[3]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL4 );
   m_pPartyMemComboImg[3]->SetFocusCheck(false);  
   
   m_pPartyMemComboImg[4] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
                                              m_pPartyMemWnd->GetID(), 38, 27, 5, 4, false );
   m_pPartyMemComboImg[4]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL5 );
   m_pPartyMemComboImg[4]->SetFocusCheck(false);

   m_pPartyMemComboImg[5] = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
                                              m_pPartyMemWnd->GetID(), 38, 21, 5, 5, false );
   m_pPartyMemComboImg[5]->SetClientImage( TID_PARTY_MEMBER_COMBO_LEVEL6 );
   m_pPartyMemComboImg[5]->SetFocusCheck(false);	
   
   
   m_pPartyMemNameTxt = (CLimitLenTextBox*)nRui->CreateWnd( n_wtLimitLenTextBox, WID_None, 
						m_pPartyMemWnd->GetID(), 48, 13, 96, 10, false );
   m_pPartyMemNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
   m_pPartyMemNameTxt->SetClientImage( TID_None );
   m_pPartyMemNameTxt->SetAlignText( n_atLeft, n_atCenter );	
   m_pPartyMemNameTxt->SetText(_RT(""));	
   m_pPartyMemNameTxt->SetLimitLength(RP_MAX_PC_NAME - sizeof(RTCHAR));	
   m_pPartyMemNameTxt->SetTextColor( 255, 255, 122 );
   m_pPartyMemNameTxt->SetFocusCheck(false);
   
   	
   m_pPartyMemHPGauge = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_None, 
				           m_pPartyMemWnd->GetID(), 46, 27, 101, 7, false );
   m_pPartyMemHPGauge->SetClientImage( TID_None );
   m_pPartyMemHPGauge->SetBarImage( TID_PARTY_MEMBER_HPGAUGE );
   m_pPartyMemHPGauge->SetBarBlank( 0, 0 );
   m_pPartyMemHPGauge->InitProgress( 100 );
   m_pPartyMemHPGauge->SetFocusCheck(false);
   
   
   m_pPartyMemSPMPGauge = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_None, 
				             m_pPartyMemWnd->GetID(), 46, 37, 101, 7, false );
   m_pPartyMemSPMPGauge->SetClientImage( TID_None );
   m_pPartyMemSPMPGauge->SetBarImage( TID_PARTY_MEMBER_SPMPGAUGE );
   m_pPartyMemSPMPGauge->SetBarBlank( 0, 0 );
   m_pPartyMemSPMPGauge->InitProgress( 100 );
   m_pPartyMemSPMPGauge->SetFocusCheck(false);


   m_HPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pPartyMemWnd->GetID(), 
	   46, 26, 101, 7, false );  
   m_HPText->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
   m_HPText->SetImageNum( true );
   m_HPText->SetClientImage( TID_None );
   m_HPText->SetClientColor( 180, 175, 160 );
   m_HPText->SetAlignText( n_atRight, n_atCenter );
   m_HPText->SetText( _RT("1000/1000") );
   m_HPText->SetTextColor( 255, 220, 220 );
   m_HPText->SetHID( HD_S_HP );
   m_HPText->SetFocusCheck(false);
   
   
   //	SMPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_PlayerInfo_SMPText, WID_PlayerInfo_Wnd, 110, 57, 100, 12, false );
   m_SMPText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pPartyMemWnd->GetID(), 
	   46, 36, 101, 7, false );   
   m_SMPText->SetFontg( n_ftGulimChe, 5, n_fwNormal, n_fdNormal );
   m_SMPText->SetImageNum( true );
   m_SMPText->SetClientImage( TID_None );
   m_SMPText->SetClientColor( 180, 175, 160 );
   m_SMPText->SetAlignText( n_atRight, n_atCenter );
   m_SMPText->SetText( _RT("100/100") );
   m_SMPText->SetTextColor( 220, 220, 255 );
   m_SMPText->SetHID( HD_S_MP );
   m_SMPText->SetFocusCheck(false);

}

void CPartyMemeberWnd::SetPartyMemberInfo( SPARTYMEMBER_INFO * pMemberInfo )
{
   m_pMemberInfo = pMemberInfo;
}

SPARTYMEMBER_INFO * CPartyMemeberWnd::GetPartyMemberInfo()
{
   return m_pMemberInfo;	 
}

