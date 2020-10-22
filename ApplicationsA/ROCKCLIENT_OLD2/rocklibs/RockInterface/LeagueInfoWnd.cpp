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

#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "LeagueInfoWnd.h"
#include "PartyInforWnd.h"
#include "stringmanager.h"
#include "PartyPopupWnd.h"

CLeagueInfoWnd g_LeagueInfoWnd;

CLeagueInfoWnd::CLeagueInfoWnd()
{
	memset( &m_pPartyMemInfoList, 0, sizeof(SPARTYMEMBER_INFO *) * (MAX_LEAGUE_MEMBER_NUM + 1) );

	m_pLeagueFrameWnd = NULL;
	m_CloseBtn = NULL;

	m_nInsertIndex = 0;
}

CLeagueInfoWnd::~CLeagueInfoWnd()
{
	Init();
}

void CLeagueInfoWnd::Init()
{
	for(int i = 0; i < MAX_LEAGUE_MEMBER_NUM; i++)
	{
		SAFE_DELETE(m_pPartyMemInfoList[i]);
	}

	m_nInsertIndex = 0;
}

void CLeagueInfoWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	CImageBox9*		img9 = NULL;
	
	SRect LeagueWnd;
	LeagueWnd.w	= 376;
	LeagueWnd.h	= 426;
	LeagueWnd.x	= center_x - ( LeagueWnd.w / 2 );
	LeagueWnd.y	= center_y - ( LeagueWnd.h / 2 );
	
	m_pLeagueFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
		LeagueWnd.x, LeagueWnd.y, LeagueWnd.w, LeagueWnd.h, false );
	m_pLeagueFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pLeagueFrameWnd->SetClientImage( TID_CTRL_WND_L1 );	
	m_pLeagueFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pLeagueFrameWnd->SetWndTileSizeTB( 31, 11 );	
	m_pLeagueFrameWnd->SetCaption( G_STRING_CHAR( IDS_LEAGUE_MEMBER ) );										
	m_pLeagueFrameWnd->SetWndProc( this );	
	m_pLeagueFrameWnd->SetTitle( true );  
    m_pLeagueFrameWnd->SetVisible( false );
    m_pLeagueFrameWnd->SetMove( true );
	m_pLeagueFrameWnd->SetZorder( n_zoTop );
    m_pLeagueFrameWnd->Initialize();

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, 
										m_pLeagueFrameWnd->GetID(), 3, 33, 370, 388, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );

	m_CloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, 
				  WID_None, m_pLeagueFrameWnd->GetID(), 357, 6, 13, 13, false );
	m_CloseBtn->SetBtnImage(TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, 
							TID_CTRL_BTN_X_CLK, TID_None );

	int nXPos = 4;
	int nYPos = 36;

	for(int i = 0 ; i < MAX_LEAGUE_MEMBER_NUM ; i++)
	{
		if( i >=  MAX_LEAGUE_MEMBER_NUM/2 )
		{
			nXPos = 188;
		}

		if(i ==  MAX_LEAGUE_MEMBER_NUM/2)
		{
			nYPos = 36;
		}

		m_LeagueDataList[i].pBackImg = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, 
										m_pLeagueFrameWnd->GetID(), nXPos, nYPos, 95, 25, false );       	
		m_LeagueDataList[i].pBackImg->SetWndTileSizeLR( 3, 3 );
		m_LeagueDataList[i].pBackImg->SetWndTileSizeTB( 3, 3 );
		m_LeagueDataList[i].pBackImg->SetFocusCheck( false );
		m_LeagueDataList[i].pBackImg->SetClientImage( TID_CTRL_BLACKBACK_L1 );
		m_LeagueDataList[i].pBackImg->SetAdjustPosition( 0, 0 );		

		m_LeagueDataList[i].pMaster = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, 
										m_pLeagueFrameWnd->GetID(), nXPos + 5, nYPos + 7, 11, 9, false );
		m_LeagueDataList[i].pMaster->SetFocusCheck( false );
		m_LeagueDataList[i].pMaster->SetClientImage( TID_PARTY_MEMBER_MASTER );
		m_LeagueDataList[i].pMaster->SetVisible(false);
		
		
		m_LeagueDataList[i].pNameTxt = (CLimitLenTextBox*)nRui->CreateWnd( n_wtLimitLenTextBox, 
										WID_None, m_pLeagueFrameWnd->GetID(), 
		nXPos, nYPos, 95, 25, false );
		m_LeagueDataList[i].pNameTxt->SetClientImage(TID_None);
		m_LeagueDataList[i].pNameTxt->SetAlignText( n_atLeft, n_atCenter );
		m_LeagueDataList[i].pNameTxt->SetBlank( 20, 0, 0, 0 );		
		m_LeagueDataList[i].pNameTxt->SetText(_RT(""));	
		m_LeagueDataList[i].pNameTxt->SetTextColor(255, 255, 255);

		m_LeagueDataList[i].pButton = (CButton9*)nRui->CreateWnd( n_wtButton9, WID_None, 
										m_pLeagueFrameWnd->GetID(), nXPos, nYPos, 95, 25, false );       	
		m_LeagueDataList[i].pButton->SetWndTileSizeLR( 2, 2 );
		m_LeagueDataList[i].pButton->SetWndTileSizeTB( 2, 2 );
		m_LeagueDataList[i].pButton->SetClientImage( TID_None );
		m_LeagueDataList[i].pButton->SetBtnImage(TID_None, TID_CTRL_YSELECT_L1, 
												 TID_CTRL_YSELECT_L1, TID_None);
		m_LeagueDataList[i].pButton->SetAdjustPosition( 0, 0 );

		img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, 
										m_pLeagueFrameWnd->GetID(), nXPos + 95, nYPos, 25, 25, false );       	
		img9->SetWndTileSizeLR( 3, 3 );
		img9->SetWndTileSizeTB( 3, 3 );
		img9->SetFocusCheck( false );
		img9->SetClientImage( TID_CTRL_BLACKBACK_L1 );
		img9->SetAdjustPosition( 0, 0 );
		
		
		m_LeagueDataList[i].pLevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, 
									WID_None, m_pLeagueFrameWnd->GetID(), 
		nXPos + 95, nYPos, 25, 25, false );
		m_LeagueDataList[i].pLevelTxt->SetClientImage(TID_None);
		m_LeagueDataList[i].pLevelTxt->SetAlignText( n_atLeft, n_atCenter);
		m_LeagueDataList[i].pLevelTxt->SetBlank( 3, 0, 0, 0 );		
		m_LeagueDataList[i].pLevelTxt->SetText(_RT(""));
		m_LeagueDataList[i].pLevelTxt->SetTextColor(255, 255, 255);

		img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, 
										m_pLeagueFrameWnd->GetID(), nXPos + 95 + 25, nYPos, 59, 25, false );       	
		img9->SetWndTileSizeLR( 3, 3 );
		img9->SetWndTileSizeTB( 3, 3 );
		img9->SetFocusCheck( false );
		img9->SetClientImage( TID_CTRL_BLACKBACK_L1 );
		img9->SetAdjustPosition( 0, 0 );
		
		m_LeagueDataList[i].pJobTxt = (CLimitLenTextBox*)nRui->CreateWnd( n_wtLimitLenTextBox, 
										WID_None, m_pLeagueFrameWnd->GetID(), 
		nXPos + 95 + 25, nYPos, 59, 25, false );
		m_LeagueDataList[i].pJobTxt->SetClientImage(TID_None);
		m_LeagueDataList[i].pJobTxt->SetAlignText( n_atLeft, n_atCenter );
		m_LeagueDataList[i].pJobTxt->SetBlank( 5, 0, 0, 0 );		
		m_LeagueDataList[i].pJobTxt->SetText(_RT(""));	
		m_LeagueDataList[i].pJobTxt->SetTextColor(255, 255, 255);		

		nYPos += 25;
	}
	
	/*for( i = 0 ; i < MAX_LEAGUE_MEMBER_NUM ; i++ )
	{
		SPARTYMEMBER_INFO * pSrcMember = InsertLeagueMember((UINT)(i + 1));
		if(pSrcMember)
		{
			TCHAR strTemp[256] = {0,};
			int nNum = RandomNum(3, RP_MAX_PC_NAME-1);
			RandomNameMake(strTemp, nNum);
			_tcscpy(pSrcMember->m_strPCName, strTemp);

			nNum = RandomNum(60, 180);
			pSrcMember->m_bLevel = nNum;
			
			nNum = RandomNum(0, 13);
			pSrcMember->m_bMainClass = 	nNum;
			
			SPARTYMEMBER_INFO * pDestMember = g_PartyInforWnd.FindPartyMember((UINT)(i + 1));
			if(pDestMember)
			{
				memcpy(pDestMember, pSrcMember, sizeof(SPARTYMEMBER_INFO));
			}
		}
	}

	SetLeagueMemberField();*/
}

void CLeagueInfoWnd::Update()
{

}

void CLeagueInfoWnd::Proc( SEventMessage* EMsg )
{
	///-- 윈도우 닫기 버튼
	if( EMsg->FocusWnd == m_CloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}
	
	for(int i = 0 ; i < MAX_LEAGUE_MEMBER_NUM ; i++)
	{
		if( EMsg->FocusWnd == m_LeagueDataList[i].pButton->GetID() )
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				ChangePartyMember(i);
			}

			if( EMsg->Notify == NM_BTN_RCLICK )
			{
				//해당 멤버윈도우에 멤버가 없다면 팝업동작안함
				if(m_pPartyMemInfoList[i] && m_pPartyMemInfoList[i]->m_dPCID != g_Pc.m_MainPC.lUnique)
				  {
					  POINT		ptCursor;
					  GetCursorPos( &ptCursor );
					  ::ScreenToClient( g_RockClient.GetApp(), &ptCursor );
					  ptCursor.x -= 5;
					  ptCursor.y -= 5;
					  
					  nRui->SendEvent( WID_Party_PopupWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );		  
				   
					  g_PartyPopupWnd.GetPartyPopupWnd()->SetAbsolutePosition(ptCursor.x, ptCursor.y);
					  //현재 파티장인지 아닌지에 따라 팝업윈도우 활성메뉴가 달라진다.
					  g_PartyPopupWnd.SetMemberInfo(m_pPartyMemInfoList[i]);
					  
					  bool fMwnu1, fMwnu2, fMwnu3, fMwnu4;
					  fMwnu1 = fMwnu2 = fMwnu3 = fMwnu4 = true;
					  
					  //리그 장이 아니면


					if( g_Pc.m_MainPC.lUnique == g_PartyInforWnd.GetPartyInfo()->m_dLeaderID )
					{
						fMwnu1 = true;	
					}
					else 
					{
	 					fMwnu2 = false;	
						fMwnu1 = false;  
					}

					fMwnu3 = false; 						

					g_PartyPopupWnd.SetActiveMemnu(fMwnu1, fMwnu2, fMwnu3, fMwnu4);

				 }   
			}
		}
	}
}

void CLeagueInfoWnd::Open()
{
	nRui->SendEvent( m_pLeagueFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CLeagueInfoWnd::Close()
{
	nRui->SendEvent( m_pLeagueFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

SPARTYMEMBER_INFO * CLeagueInfoWnd::InsertLeagueMember(DWORD dPCID)
{
		
	if( m_nInsertIndex < MAX_LEAGUE_MEMBER_NUM )
	{
		SPARTYMEMBER_INFO * pLPartymemInfo = SAFE_NEW( SPARTYMEMBER_INFO );		
		m_pPartyMemInfoList[m_nInsertIndex] = pLPartymemInfo;
		m_nInsertIndex++;
		memset(pLPartymemInfo, 0 ,sizeof(SPARTYMEMBER_INFO));
		pLPartymemInfo->m_dPCID = dPCID;

		SPARTYMEMBER_INFO * pPartymemInfo = g_PartyInforWnd.InsertPartyMember(pLPartymemInfo->m_dPCID);
		if(pPartymemInfo)
		{
			memcpy(pPartymemInfo, pLPartymemInfo, sizeof(SPARTYMEMBER_INFO));
			long lunique = (long)dPCID;
			//현재 가시영역에 존재하는지 여부를 찾음
			PCTABLE *pPcTable = g_Pc.FindPcTable(lunique);
			if( pPcTable )
			{
				pPartymemInfo->m_bIsViewRange = TRUE;			
				if( g_PartyInforWnd.MakePCharImage(dPCID) )
				{
					pPartymemInfo->m_bIsSee = TRUE;
				}
			}
			else
			{
				pPartymemInfo->m_bIsViewRange = FALSE; 
			}
			pPartymemInfo->m_bIsLogOut = FALSE;
		}

		SetLeagueMemberField();
		return pLPartymemInfo;
	}

	return NULL;
}

void CLeagueInfoWnd::SetLeagueMemberField()
{

	for(int i = 0 ; i < MAX_LEAGUE_MEMBER_NUM ; i++)
	{
		///-- 리그멤버정보창이 있으면
		if( m_pPartyMemInfoList[i] )
		{
			///-- 파티창에 리그멤버가 존재해있으면
			if(g_PartyInforWnd.FindPartyMember(m_pPartyMemInfoList[i]->m_dPCID))
			{
				m_LeagueDataList[i].pBackImg->SetClientImage(TID_CTRL_BLUEBACK_L1);
				m_LeagueDataList[i].pNameTxt->SetTextColor(0, 0, 0);
			}
			else
			{
				m_LeagueDataList[i].pBackImg->SetClientImage(TID_CTRL_BLACKBACK_L1);
				m_LeagueDataList[i].pNameTxt->SetTextColor(255, 255, 255);
			}			
			
			TCHAR strTemp[256] = {0,};
			m_LeagueDataList[i].pNameTxt->SetText(m_pPartyMemInfoList[i]->m_strPCName);
			m_LeagueDataList[i].pNameTxt->SetLimitLength(10);
			Rsprintf(RWCHAR(strTemp), _RT("%d"), m_pPartyMemInfoList[i]->m_bLevel);
			m_LeagueDataList[i].pLevelTxt->SetText(strTemp);
			::GetClassText(m_pPartyMemInfoList[i]->m_bMainClass, strTemp);
			m_LeagueDataList[i].pJobTxt->SetText(strTemp);
			m_LeagueDataList[i].pJobTxt->SetLimitLength(6);
			
			//멤버가 로그오프중이면
			if(m_pPartyMemInfoList[i]->m_bIsLogOut)
			{
				m_LeagueDataList[i].pLevelTxt->SetText(_RT(""));
				m_LeagueDataList[i].pJobTxt->SetText(_RT("LogOff"));
			}

			//방장여부
			if(g_PartyInforWnd.GetPartyInfo()->m_dLeaderID == m_pPartyMemInfoList[i]->m_dPCID)
			{
				m_LeagueDataList[i].pMaster->SetVisible(true);
			}
			else
			{	
				m_LeagueDataList[i].pMaster->SetVisible(false);
			}
		
		}
		///-- 없으면
		else
		{
			m_LeagueDataList[i].pBackImg->SetClientImage(TID_CTRL_BLACKBACK_L1);
			m_LeagueDataList[i].pNameTxt->SetTextColor(255, 255, 255);
			m_LeagueDataList[i].pNameTxt->SetText(_RT(" "));
			m_LeagueDataList[i].pLevelTxt->SetText(_RT(" "));
			m_LeagueDataList[i].pJobTxt->SetText(_RT(" "));
			m_LeagueDataList[i].pMaster->SetVisible(false);
		}
	}
	
}

void CLeagueInfoWnd::ChangePartyMember(int nIndex)
{
	///구성 멤버가 존재하면
	if(m_pPartyMemInfoList[nIndex] && m_pPartyMemInfoList[nIndex]->m_dPCID != g_Pc.m_MainPC.lUnique)
	{
		///-- 파티내에 해당 멤버가 존재하면
		SPARTYMEMBER_INFO * pSrcMember = g_PartyInforWnd.FindPartyMember(m_pPartyMemInfoList[nIndex]->m_dPCID);
		if(pSrcMember)
		{
			//해당멤버를 제거한다.
			g_PartyInforWnd.DeletePartyMember(m_pPartyMemInfoList[nIndex]->m_dPCID);	
		}
		else	///-- 존재하지 않으면
		{
			///추가하기전에 사이즈를 구한다.
			int nSize = g_PartyInforWnd.GetTotPartyMemNumber();
			if( MAX_PARTY_MEMBERNUM == nSize )
			{
				///마지막 구성멤버를 제거한다.
				SPARTYMEMBER_INFO * pMember = g_PartyInforWnd.GetPartyMemberList()->back();
				g_PartyInforWnd.DeletePartyMember(pMember->m_dPCID);
			}

			///그리고 해당 멤버를 추가한다.
			SPARTYMEMBER_INFO * pDestMember = g_PartyInforWnd.InsertPartyMember(m_pPartyMemInfoList[nIndex]->m_dPCID);
			if(pDestMember)
			{
				memcpy(pDestMember, m_pPartyMemInfoList[nIndex], sizeof(SPARTYMEMBER_INFO));
			}
		}
	}

	SetLeagueMemberField();
}

SPARTYMEMBER_INFO * CLeagueInfoWnd::FindLeagueMember(DWORD dPCID)
{
	for(int i = 0 ; i < MAX_LEAGUE_MEMBER_NUM ; i++)
	{
		if(m_pPartyMemInfoList[i])
		{
			if(m_pPartyMemInfoList[i]->m_dPCID == dPCID)
			{
				return m_pPartyMemInfoList[i];
			}
		}
		else
		{
			break;
		}
	}

	return NULL;
}

SPARTYMEMBER_INFO * CLeagueInfoWnd::FindLeagueMember(TCHAR *strName)
{
	for(int i = 0 ; i < MAX_LEAGUE_MEMBER_NUM ; i++)
	{
		if(m_pPartyMemInfoList[i])
		{
			if( Rstrcmp(m_pPartyMemInfoList[i]->m_strPCName, strName) == 0 )
			{
				return m_pPartyMemInfoList[i];
			}
		}
		else
		{
			break;
		}
	}	
	
	return NULL;
}

void CLeagueInfoWnd::ReSetPartyMember(bool bFlag)
{
		
	for(int i = 0 ; i < MAX_LEAGUE_MEMBER_NUM ; i++)
	{
		if(m_pPartyMemInfoList[i])
		{
			///-- 파티내에 해당 멤버가 존재하면
			SPARTYMEMBER_INFO * pSrcMember = NULL; 
				
			if(bFlag)
			{
				pSrcMember = g_PartyInforWnd.FindPartyMember(m_pPartyMemInfoList[i]->m_dPCID);				
			}
			else
			{
				pSrcMember = g_PartyInforWnd.FindPartyMember(m_pPartyMemInfoList[i]->m_strPCName);
			}
			
			if(pSrcMember)
			{
				memcpy(pSrcMember, m_pPartyMemInfoList[i], sizeof(SPARTYMEMBER_INFO));
				if(!pSrcMember->m_bIsLogOut)
				{
					long lunique = (long)pSrcMember->m_dPCID;
					//현재 가시영역에 존재하는지 여부를 찾음
					PCTABLE *pPcTable = g_Pc.FindPcTable(lunique);
					if( pPcTable )
					{
						pSrcMember->m_bIsViewRange = TRUE;			
						if( g_PartyInforWnd.MakePCharImage(pSrcMember->m_dPCID) )
						{
							pSrcMember->m_bIsSee = TRUE;
						}
					}
					else
					{
						pSrcMember->m_bIsViewRange = FALSE; 
					}
			
					pSrcMember->m_bIsLogOut = FALSE;
				}
			}
		}
		else
		{
			break;
		}
	}
	
	SetLeagueMemberField();
}

void CLeagueInfoWnd::AllDelete()
{
	for(int i = 0 ; i < MAX_LEAGUE_MEMBER_NUM ; i++)
	{
		if(m_pPartyMemInfoList[i])
		{
			g_PartyInforWnd.DeletePCharImage(m_pPartyMemInfoList[i]->m_dPCID);
		}
		else
		{
			break;
		}
	}

	Init();
}

void CLeagueInfoWnd::DeleteLeagueMember(DWORD dPCID)
{
	int index = -1;
	
	for(int i = 0 ; i < MAX_LEAGUE_MEMBER_NUM ; i++)
	{
		if(m_pPartyMemInfoList[i])
		{
			if(m_pPartyMemInfoList[i]->m_dPCID == dPCID)
			{
				index = i;
				SAFE_DELETE(m_pPartyMemInfoList[i]);
				
				g_PartyInforWnd.DeletePartyMember(dPCID);
				break;
			}
		}
		else
		{
			break;
		}
	}
	
	if(-1 != index)
	{
		for(int i = index ; i < MAX_LEAGUE_MEMBER_NUM ; i++)
		{
			m_pPartyMemInfoList[i] = m_pPartyMemInfoList[i + 1];
		}

		m_nInsertIndex--;
		SetLeagueMemberField();	
	}
	
	SetLeagueMemberField();
}

void CLeagueInfoWnd::LogOff()
{
	Init();
	Close();
}