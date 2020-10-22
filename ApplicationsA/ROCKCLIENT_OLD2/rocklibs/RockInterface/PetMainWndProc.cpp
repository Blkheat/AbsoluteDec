// PetMainWndProc.cpp: implementation of the CPetMainWndProc class.
//
//////////////////////////////////////////////////////////////////////
#include "RockPCH.h"
#include "..\\..\\RockClient.h"
#include "Rui.h"
//////////////////////////////////////////////////////////////////////////
#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "UpGauge.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Spin.h"
#include "Wnd.h"
//////////////////////////////////////////////////////////////////////////
#include "MessageWndProc.h"
#include "..\\..\\RockClient.h"


#include "PetInventoryWnd.h"
#include "PetInfoWnd.h"


#include "PetMainWndProc.h"


#include <time.h>
#include "DelayActList.h"
//////////////////////////////////////////////////////////////////////////


#define TEXT_COLOR_TITLE 255, 255, 200
#define TEXT_COLOR_STATIC 225, 225, 225
#define TEXT_COLOR_VALUE1 225, 225, 255
#define TEXT_COLOR_VALUE2 225, 255, 225

///���� ���� ���� //////////////////////////////////////////////////////////////////////////


const DWORD c_Max_DrawKeepupItemTime( 1000.0f * 60.0f * 60.0f * 24.0f );		// 24 Hours 



CPetMainWndProc g_PetMainWndProc;
CPetCreateWnd	g_PetCreateWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



//
//struct SPetInfo
//{	
//
//	BYTE	PetType;						//������ �̳� ������̳� 
//	BYTE	PetSlot;							//������â�� ���° �����̳� ??
//	char	Name[MAX_PET_NAME];
//	WORD	NpcCode;						//NPC �ڵ� 
//	BYTE		Level;			
//	DWORD	Exp;								//���� exp
//	WORD	HP;									//ü�� 
//	WORD	CP;									//���ŷ� 
//	DWORD DieDate;							//���� ���� ��¥ 
//	BYTE	MaxInvenNum;				//�� ����� �κ� �⺻ 4�� 
//	BYTE	ItemNum;						//���� �κ��� ����ִ� ������ ���� 
//
//
//	SPcItem *KeepUpSkill[3];				//���� ��ų 
//	SPcItem *InvenItem[20];					//�κ� �ִ밳 ���� 20 
//	SPcItem *WearItem[3];					// �Ӹ� ���� �� 
//
//
//	int		Ability;
//	int		ProbAbility;
void CPetMainWndProc::ClearPetMainWnd()
{
	int i =0 ; 
	int PetInc =0; 
	
	CurPetBtn = 0;
	
	for( i = 0 ; i < MAX_PET_SLOT_NUM ; ++i )
	{
		if( PcInven->thePetData[i] == NULL )
		{
			m_PetText[i].NameAbilityTxt->SetText(_RT(""));
			m_PetText[i].NameProbabilityTxt->SetText( _RT("") );
			m_PetText[i].NameTxt->SetText( _RT("") );		
			m_PetText[i].LevelTxt->SetText( _RT(""));
			m_PetText[i].TypeTxt->SetText( _RT(""));
			m_PetText[i].HpTxt->SetText( _RT("") );
			m_PetText[i].CpTxt->SetText( _RT("") );
			m_PetText[i].ExpTxt->SetText( _RT("") );
			m_PetText[i].AbilityTxt->SetText( _RT("") );
			m_PetText[i].ProbabilityTxt->SetText( _RT("") );
			m_PetText[i].PetDieTimeTxt->SetText( _RT("") );	
			
			m_PetText[i].ReCallBtn->SetVisible(true);
			m_PetText[i].CallOffBtn->SetVisible(false);			
			
			for( PetInc = 0 ; PetInc < RP_MAX_PET_USEDITEM ; ++PetInc )
			{
				m_PetText[i].KeepupItemSlot[PetInc]->PcItem =  NULL;
				m_PetText[i].KeepupItemSlot[PetInc]->SetVisible(false);
				m_PetText[i].KeepupItemText[PetInc]->SetVisible(false);
				m_PetText[i].fRemainTime[PetInc] = 0; 
			}
			
			///-- �뺴 ������ �̸��� ��� ����ֱ�
			if( i == MERCE_SLOT_NUM )
			{
				m_PetText[i].NameTxt->SetVisible(true);
				m_PetText[i].PetDieTimeTxt->SetVisible(false);
				m_PetText[i].NameTxt->SetText( G_STRING_CHAR( IDS_SOLDIERSLOT ) );		
				m_PetText[i].ExpTxt->SetVisible(false);
				
				if( m_PetText[i].pRemainTimeTxt )
				{
					m_PetText[i].pRemainTimeTxt->SetVisible(false);
				}
			}	
		}
	}
}

void CPetMainWndProc::Init()
{
	CWndProc * pWnd = &g_PetCreateWnd;	
	
	CurPetBtn = 0 ; 	
	
	PcInven =  nRui->GetPcInvenInfo();
	
	//������ , ����� �ΰ����� ������ .. 
	
	Rstrcpy(RWCHAR(m_TypeName[n_atAttacker].cTypeName) ,G_STRING_CHAR( IDS_PET_ATTACKER ));
	Rstrcpy(RWCHAR(m_TypeName[n_atAttacker].cAbility) ,	G_STRING_CHAR( IDS_PET_ABILITY_ATTACK ));
	Rstrcpy(RWCHAR(m_TypeName[n_atAttacker].cProbability), G_STRING_CHAR( IDS_PET_PROBABILITY_ATTACK ));
	
	
	Rstrcpy(RWCHAR(m_TypeName[n_atDefence].cTypeName) , G_STRING_CHAR( IDS_PET_ALLEVIANT  ) ) ;
	Rstrcpy(RWCHAR(m_TypeName[n_atDefence].cAbility) , G_STRING_CHAR( IDS_PET_ABILITY_ABSORPTION  ));
	Rstrcpy(RWCHAR(m_TypeName[n_atDefence].cProbability), G_STRING_CHAR( IDS_PET_PROBABILITY_ABSORTION ));
	
	for( int i = 0; i < MAX_PET_SLOT_NUM ; ++i )
	{
		m_PetText[i].NameAbilityTxt->SetText(_RT(""));
		m_PetText[i].NameProbabilityTxt->SetText( _RT("") );
		m_PetText[i].NameTxt->SetText( _RT("") );		
		m_PetText[i].LevelTxt->SetText( _RT(""));
		m_PetText[i].TypeTxt->SetText( _RT(""));
		m_PetText[i].HpTxt->SetText( _RT("") );
		m_PetText[i].CpTxt->SetText( _RT("") );
		m_PetText[i].ExpTxt->SetText( _RT("") );
		m_PetText[i].AbilityTxt->SetText( _RT("") );
		m_PetText[i].ProbabilityTxt->SetText( _RT("") );
		m_PetText[i].PetDieTimeTxt->SetText( _RT("") );		
		
		//		m_PetText[i].ReCallBtn->SetBtnHold(true);
		//		m_PetText[i].InvenBtn->SetBtnHold(true);
		//		m_PetText[i].ImgReCall->SetVisible(false);
		//		m_PetText[i].ImgInven->SetVisible(false);		
		
		m_PetText[i].ReCallBtn->SetVisible(true);
		m_PetText[i].CallOffBtn->SetVisible(false);
		
		for(int PetInc = 0 ; PetInc < RP_MAX_PET_USEDITEM ; ++PetInc)
		{
			m_PetText[i].KeepupItemSlot[PetInc]->PcItem =  NULL;
			m_PetText[i].KeepupItemSlot[PetInc]->SetVisible(false);
			m_PetText[i].KeepupItemText[PetInc]->SetVisible(false);
			m_PetText[i].fRemainTime[PetInc] = 0.0f; 
		}
		
		///-- �뺴 ������ �̸��� ��� ����ֱ�
		if( i == MERCE_SLOT_NUM )
		{
			m_PetText[i].NameTxt->SetVisible(true);
			m_PetText[i].PetDieTimeTxt->SetVisible(false);
			m_PetText[i].NameTxt->SetText( G_STRING_CHAR( IDS_SOLDIERSLOT ) );		
			
			if( m_PetText[i].pRemainTimeTxt )
			{
				m_PetText[i].pRemainTimeTxt->SetText( _RT("") );
			}
		}
	}
}

void CPetMainWndProc::Proc( SEventMessage* EMsg )
{
	
	int i =0;
	char strbuf[128] = "";
	
	//	RLG1("PetMain Proc %d ",EMsg->FocusWnd);
	
	//�ݱ� ��ư 
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_PETMAIN_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
		}
	}	
	
	for( i = 0 ; i < MAX_PET_SLOT_NUM  ; ++i )
	{
		if(	PcInven->thePetData[i] != NULL )
		{
			
			if( EMsg->FocusWnd == m_PetText[i].ReCallBtn->GetID())
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					
					if( PcInven->thePetData[i] != NULL)
					{
						
						//PVP �������� ���ȯ �Ұ��� 
						if( g_Pc.GetPlayer()->theMapType == n_FightZone	)
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PVP_NOT_RECALL)); 
						}
						
						if(g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE)
						{
							SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NOT_WILL_RECALL) ) ;   //" ��� �ϸ� ��ȯ �Ҽ� �����ϴ�."
							return;
						}						
						
						switch(PcInven->thePetData[i]->PetState) 
						{
						case n_Pet_Die:
							{				
								SystemPrint( n_DarkGreen, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_NOT_RECALL) ) ;   //" ��ȯ �Ҽ� �����ϴ�." 		
								return;	
							}
							break;
						case n_Pet_Live:break;
						case n_Pet_Custody:
							{
								SystemPrint( n_DarkGreen, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_CUSTODY_NOT_RECALL) );    //"���� ���� ���� ��ȯ �Ҽ������ϴ�. " 		
								return;
							}
							break;						
							
						}
						
						//��ȯ ���� ���� ���� �ִٸ� ... ??
						//�θ��� ����...
						
						for( int j = 0 ; j < MAX_PET_SLOT_NUM ; ++j )
						{
							if( PcInven->thePetData[j] != NULL )
							{
								list< SDelayDisappearPet * >::iterator i = g_DelayActList.m_DelayDisappearPetList.begin();
								
								for( ; i != g_DelayActList.m_DelayDisappearPetList.end() ; ++i )
								{	
									
									if( ( *i ) && ( *i )->DisappearPet.thePetID ==  PcInven->thePetData[j]->PetID )
									{
										return;
									}			
								}
							}						
						}
						
						g_RockClient.Send_CsCallPet( PcInven->thePetData[i]->PetSlot );
						nRui->SendEvent( m_pFrameOffWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );							
					}										
				}				
			}	
			
			//��ȯ ���� ��ư 
			else if( EMsg->FocusWnd == m_PetText[i].CallOffBtn->GetID() )
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					//��ȯ ���� â 
					nRui->SendEvent( m_pFrameOffWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );		
					return;
				}
			}
			else if (EMsg->FocusWnd == m_PetText[i].InvenBtn->GetID())
			{
				if( EMsg->Notify == NM_BTN_CLICK )
				{
					if( PcInven->thePetData[i] != NULL)
					{
						
						//�ΰ� ��ǥ�� �޾ƿ� .. 
						
						g_PetInventoryWnd.FrameWnd->Reposition(EMsg->Mouse.Aposition.x,EMsg->Mouse.Aposition.y);
						
						if(g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE)
						{
							SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_DIE_NOT_OPEN_INVEN)) ; 
							return;
						}		
						
						switch(PcInven->thePetData[i]->PetState)
						{
						case n_Pet_Die:	break;
						case n_Pet_Live: break;
						case n_Pet_Custody:	
							{
								
								SystemPrint( n_DarkGreen, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_CSTODY_NOT_OPEN_INVEN)) ;  		
								return;
								
								break;						
							}
						}						
						
						//���° ������ �˷��ֱ� ���� .. 
						if(g_PetInventoryWnd.IsVisible)
						{
							g_PetInventoryWnd.Close();
						}
						else
						{	
#ifdef PET_TIME_TEST //�ð��� ���ǰ��� �κ��丮 ���Ұ�
							if( PcInven->thePetData[i]->NpcCode > 6190  && PcInven->thePetData[i]->NpcCode < 6197 )
							{
								SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_INVENTORY_NOTOPEN)) ;
								return ;
							}
#else 
#endif // PET_TIME_TEST 
							g_PetInventoryWnd.m_CurSlotIndex = i;
							g_PetInventoryWnd.Open();
						}
					}
				}
			}
			
		}	// Get Enable 
	} //for 
	
	  /********************************************************************
	  - ��ȯ ���� ��ư 
	  *********************************************************************/
	  
	  
	  if(EMsg->FocusWnd == m_OkBtn->GetID())
	  {		
		  if( EMsg->Notify == NM_BTN_CLICK )
		  {			
			  //���� ��ȯ��ư�� ���̳� ? 
			  g_RockClient.Send_CsCancelCallPet();
			  nRui->SendEvent( m_pFrameOffWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
		  }		
	  }
	  else if(EMsg->FocusWnd == m_CancelBtn->GetID())
	  {
		  if( EMsg->Notify == NM_BTN_CLICK )
		  {
			  nRui->SendEvent( m_pFrameOffWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );		
		  }		
	  }
}	

void CPetMainWndProc::Update()
{
	int i = 0 , j =0 ; 
	static TCHAR tmpStr[128] = {0,};
	static TCHAR tmpStr2[128] = {0,};
	
	PetUpdateState();
	
	for( i =0 ; i < MAX_PET_SLOT_NUM ; i++)
	{
		
		if( PcInven->thePetData[i]  != NULL && PcInven->thePetData[i]->Level)
		{
			int nNpcCode = PcInven->thePetData[i]->NpcCode;			
			
			bool bIsAlive = true;
			
			if( PcInven->thePetData[i]->PetState == n_Pet_Die )
			{
				bIsAlive = false;
			}
			
			m_PetText[i].pChaImgBox->SetRenderCharImgProperty(nNpcCode, c_NpcRace_Mob, 
				CHARIMAGE_64SIZE, false , bIsAlive );				
			
			CheckCurState(i);
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d"), PcInven->thePetData[i]->Level);
			m_PetText[i].LevelTxt->SetText(tmpStr);
			
			Rsprintf( RWCHAR(tmpStr), _RT("%s"), RWCHAR(m_TypeName[PcInven->thePetData[i]->PetType].cTypeName));
			m_PetText[i].TypeTxt->SetText(tmpStr);
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d / %d") ,  PcInven->thePetData[i]->HP ,PcInven->thePetData[i]->MaxHP);
			m_PetText[i].HpTxt->SetText(tmpStr);
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d / %d") ,  PcInven->thePetData[i]->CP ,PcInven->thePetData[i]->MaxCP );
			m_PetText[i].CpTxt->SetText(tmpStr);
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d / %d") ,  PcInven->thePetData[i]->Exp , PcInven->thePetData[i]->MaxExp);
			m_PetText[i].ExpTxt->SetText(tmpStr);
			
			//���ݷ�, �����������			
			Rsprintf( RWCHAR(tmpStr), _RT("%s:"), RWCHAR(m_TypeName[PcInven->thePetData[i]->PetType].cAbility ) );
			m_PetText[i].NameAbilityTxt->SetText(tmpStr);
			
			Rsprintf( RWCHAR(tmpStr), _RT("%s:"), RWCHAR(m_TypeName[PcInven->thePetData[i]->PetType].cProbability ));
			m_PetText[i].NameProbabilityTxt->SetText(tmpStr);
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d") , PcInven->thePetData[i]->Ability );
			m_PetText[i].AbilityTxt->SetText(tmpStr);
			
			Rsprintf( RWCHAR(tmpStr), _RT("%d") ,  PcInven->thePetData[i]->ProbAbility );
			m_PetText[i].ProbabilityTxt->SetText(tmpStr);
			
			m_PetText[i].pChaImgBox->SetVisible(true);
			
			if( i == MERCE_SLOT_NUM )
			{
				if( m_PetText[i].pRemainTimeTxt )
				{
					
					///-- ���� �ð� �����ֱ�
					DWORD dCumulTime = SAFE_TIME_SUB( timeGetTime() , PcInven->thePetData[i]->theStartTime );
					DWORD dReaminTime = SAFE_TIME_SUB( PcInven->thePetData[i]->theLifeTime , dCumulTime );
					
					::GetRemainTimeStr( tmpStr , dReaminTime );						
					m_PetText[i].pRemainTimeTxt->SetText( tmpStr );
					
				}
			}
			
		}
		//�굥��Ÿ�� ������ ... 
		else 
		{	
			//ĳ�����ǽ� �̹��� �������� ���ƶ� 
			m_PetText[i].pChaImgBox->SetVisible(false);
		}
		
	}	
}

void CPetMainWndProc::CheckCurState(int PetSlot)
{
	static TCHAR tmpStr[128] = {0,};
	static TCHAR tmpStr2[128] = {0,};
	static TCHAR tmpStr3[128] = {0,};
	struct tm  tmCur;
	
	memset(tmpStr, 0 ,sizeof(tmpStr));
	memset(tmpStr2, 0 ,sizeof(tmpStr2));
	memset(&tmCur, 0 , sizeof(tmCur));
				
	//��� ������ �̸��� ����ϰ� ������ �ð��� ��� �Ѵ� .. 
	switch(PcInven->thePetData[PetSlot]->PetState) 
	{
	case n_Pet_Die:
		{
			Rsprintf( RWCHAR(tmpStr),  G_STRING_CHAR(IDS_PET_DIE_WND)  );	
			Rstrcat(RWCHAR(tmpStr) ,  _RT(" (") );
			
			m_PetText[PetSlot].PetDieTimeTxt->SetTextColor( 236,0,0);	
			tmCur =  *localtime(&PcInven->thePetData[PetSlot]->DieTime);
		}
		break;
	case n_Pet_Live:
		{
#ifdef PET_TIME_TEST // �ð��� ���ǰ��� �̸��� ����ð��� ǥ�����ش�
			if( PcInven->thePetData[PetSlot]->NpcCode > 6190  && PcInven->thePetData[PetSlot]->NpcCode < 6195 )
			{
				Rsprintf( RWCHAR(tmpStr), _RT("%s"), RWCHAR(PcInven->thePetData[PetSlot]->Name)  );
				Rstrcat(RWCHAR(tmpStr) ,  _RT(" (") );
				Rstrcat(RWCHAR(tmpStr) , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) );
				Rstrcat(RWCHAR(tmpStr) ,  _RT(":") );
				
				tmCur =  *localtime(&PcInven->thePetData[PetSlot]->DieTime);
			}
			else
			{
				Rsprintf( RWCHAR(tmpStr), _RT("%s"), RWCHAR(PcInven->thePetData[PetSlot]->Name));
				m_PetText[PetSlot].NameTxt->SetText(tmpStr);
				return;
			}
#else 
			Rsprintf( RWCHAR(tmpStr), _RT("%s"), RWCHAR(PcInven->thePetData[PetSlot]->Name));
			m_PetText[PetSlot].NameTxt->SetText(tmpStr);
			return;
#endif // PET_TIME_TEST
			
		}
		break;
	case n_Pet_Custody:
		{
			Rsprintf( RWCHAR(tmpStr), G_STRING_CHAR(IDS_PET_CUSTODY_WND) );	
			Rstrcat(RWCHAR(tmpStr) ,  _RT(" (") );
			m_PetText[PetSlot].PetDieTimeTxt->SetTextColor( 0, 200, 50  );	
			tmCur =  *localtime(&PcInven->thePetData[PetSlot]->CheckInTime );
		}
		break;
	}
	
	//�⵵ ��... �̺κ� ... 0�� �� �Ѿ�� �߰����ּ��� ^^ 09�� ������ ó�� �˴ϴ� ... 
	//100�⵵ ���� ���� ~~
	
	Rsprintf( RWCHAR(tmpStr2), _RT("%d") , tmCur.tm_year+1900 );					
	
	_tcsncpy(tmpStr3,tmpStr2+2 ,2 );
	Rstrcat(RWCHAR(tmpStr),RWCHAR(tmpStr3));
	
	
	//�� 
	if(tmCur.tm_mon+1 < 10)
	{
		Rsprintf( RWCHAR(tmpStr2), _RT("/0%d") , tmCur.tm_mon+1);
	}
	else
	{
		Rsprintf( RWCHAR(tmpStr2), _RT("/%d") , tmCur.tm_mon+1);
	}
	
	Rstrcat(RWCHAR(tmpStr),RWCHAR(tmpStr2) );
	//�� 
	if(tmCur.tm_mday < 10)
	{
		Rsprintf( RWCHAR(tmpStr2), _RT("/0%d") ,tmCur.tm_mday);
	}
	else
	{
		Rsprintf( RWCHAR(tmpStr2), _RT("/%d") ,tmCur.tm_mday);
	}
	//�ð� 
	Rstrcat(RWCHAR(tmpStr),RWCHAR(tmpStr2) );
	
	Rsprintf( RWCHAR(tmpStr2), _RT(" %d:%d)") ,tmCur.tm_hour,tmCur.tm_min);
	
	Rstrcat(RWCHAR(tmpStr),RWCHAR(tmpStr2) );
	
	m_PetText[PetSlot].PetDieTimeTxt->SetText(tmpStr);
#ifdef PET_TIME_TEST
	m_PetText[PetSlot].NameTxt->SetText(tmpStr); //�ð��� ��
#endif // PET_TIME_TEST
	
	
}

void CPetMainWndProc::Composition()
{
	int center_x = nRui->Desktop.Width / 2;
	int center_y = nRui->Desktop.Height / 2;
	
	char temp[256] = {0,};
	
	CImageBox* img = NULL;	
	CImageBox9* img9 = NULL;
	CButton* btn = NULL;
	CTextBox* text = NULL;
	
	
	SRect mainwnd;
	mainwnd.w	= 300;
	mainwnd.h	= 568;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	mainwnd.w = 400;

#endif	

	SRect sTempRect;

	
	mainwnd.x	= center_x - ( mainwnd.w / 2 );
	mainwnd.y	= center_y - ( mainwnd.h / 2 );
	
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_PETMAIN_WND, WID_None, 
											   mainwnd.x, mainwnd.y, mainwnd.w, mainwnd.h, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );	
	//	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_STATUS );								// ����Ƽ��â ǥ�� ������ 
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 ); 	
	m_pFrameWnd->SetCaption( G_STRING_CHAR(IDS_PET_INFO_WND) );				// ����Ƽ�� ����â 
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();	
	
	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X

	sTempRect.x	= 277;
	sTempRect.y	= 6;
	sTempRect.w	= 13;
	sTempRect.h	= 13;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 377;

#endif	

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None,WID_PETMAIN_WND, 
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );  	
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );
	
	///---------------------------------------------------------------------------	
	///-- Static Img
	///---------------------------------------------------------------------------	
	///-- Line

	sTempRect.x	= 1;
	sTempRect.y	= 105;
	sTempRect.w	= 294;
	sTempRect.h	= 1;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 394;

#endif	

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PETMAIN_WND, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );   
	img ->SetClientImage( TID_CTRL_LINEDOT );

	sTempRect.x	= 1;
	sTempRect.y	= 272;
	sTempRect.w	= 294;
	sTempRect.h	= 1;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 394;

#endif	
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PETMAIN_WND, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );   
	img ->SetClientImage( TID_CTRL_LINEDOT );	

	// �̹��� ���� ��� �� ���ҽ� ������ ���� ȿ�� (����Ʈ �ڽ�, �޹������ �򸮴� ���� �̹����� )
	///-- Image9
	//
	const UINT TEXT_HEIGHT_INTERVAL = 15;
	UINT Text_StartY = 50;
	UINT Text_StartX = 89;
	UINT Text_StartRX = 132;

	int nimgWidth = 297; 

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	nimgWidth	= 397;
	Text_StartRX = 202;

#endif	

	for(int i = 0 ; i < MAX_PET_SLOT_NUM ; ++i)
	{
	   /*
	  ���� 
	  ���� 
	  ü�� 
	  ���ŷ�
	  ���ݷ�  -- �ٲ�� �ؽ�Ʈ ���� ������ ��ƾ��Ѵ� . 
	  ������  -- �ٲ�� �ؽ�Ʈ ���� ������ ��ƾ��Ѵ� .
	  ����ġ 
		*/
		
		Text_StartY = 50 + ( i * 128 );	
		
		int nBgHeigth = 110;
		
		if( i == MERCE_SLOT_NUM )
		{
			nBgHeigth = 120;
		}			
		
		img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_PETMAIN_WND, 
											 1, Text_StartY-3, nimgWidth, nBgHeigth, false );       	
		img9->SetWndTileSizeLR( 9, 9 );					// ������ �����ÿ� ���� 
		img9->SetWndTileSizeTB( 8, 8 );					// ������ �����ÿ� ���� 
		img9->SetFocusCheck( true );					// �̹��� ���� �ʼ� 
		img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
		img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 ���� �ʼ� ȣ��  
		
		
		// �� ��� �ڽ� 
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PETMAIN_WND, 
										   7, Text_StartY+(TEXT_HEIGHT_INTERVAL*1)-5, 70, 70, false );   
		img ->SetClientImage( TID_C_BGSLOT_PET_FACE );
		
		
		//�� ���ǽ� �̹��� .. 
		m_PetText[i].pChaImgBox = (CCharImgBox*)nRui->CreateWnd( n_wtCharImgBox, WID_None, 
			WID_PETMAIN_WND, 7+4, Text_StartY+(TEXT_HEIGHT_INTERVAL*1)-2, 64, 64, false );   
		m_PetText[i].pChaImgBox ->SetClientImage( TID_None );
		m_PetText[i].pChaImgBox->SetFocusCheck(false);		
		
		/********************************************************************
		- �� ������ ���� �Ф�  dongS
		- ����� �� ��������� ���� ���� ��ų ... 
		*********************************************************************/
		
		// ���� ������ ��� ���� ǥ�� 
		for( int PetInc = 0 ; PetInc < RP_MAX_PET_USEDITEM; ++PetInc )
		{
			
			m_PetText[i].KeepupItemSlot[PetInc] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, WID_PETMAIN_WND,5 +(24*PetInc)  ,Text_StartY+(TEXT_HEIGHT_INTERVAL*5)+4   , 24, 24, false );
			m_PetText[i].KeepupItemSlot[PetInc]->SetSlotGroup( n_SGROUP_KEEPUP_COMMERCIALITEM );
			m_PetText[i].KeepupItemSlot[PetInc]->SetHID( HID_ITEMTIP );
				//m_PetText[i].KeepupItemSlot[PetInc]->SetVisible( false );
			m_PetText[i].KeepupItemSlot[PetInc]->SetScaleTrans( true );
			m_PetText[i].KeepupItemSlot[PetInc]->SetDelayShow(false);
			m_PetText[i].KeepupItemText[PetInc] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETMAIN_WND, 5 +(24*PetInc)  ,Text_StartY+(TEXT_HEIGHT_INTERVAL*5)+4  , 24, 22, false );  
			m_PetText[i].KeepupItemText[PetInc]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
			m_PetText[i].KeepupItemText[PetInc]->SetClientImage( TID_None );	
			m_PetText[i].KeepupItemText[PetInc]->SetAlignText( n_atCenter, n_atBottom );
			m_PetText[i].KeepupItemText[PetInc]->SetText( _RT("") );
			m_PetText[i].KeepupItemText[PetInc]->SetFocusCheck(false);
			m_PetText[i].KeepupItemText[PetInc]->SetVisible(false);
			
			
		}
		
		// Pet ��� �̹��� 
		//		img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None,  WID_PETMAIN_WND, 7 ,  Text_StartY+(TEXT_HEIGHT_INTERVAL*1), 71, 76, false );       	
		//		img9->SetWndTileSizeLR( 9, 9 );					// ������ �����ÿ� ���� 
		//		img9->SetWndTileSizeTB( 8, 8 );					// ������ �����ÿ� ���� 
		//		img9->SetFocusCheck( true );					// �̹��� ���� �ʼ� 
		//		img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
		//		img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 ���� �ʼ� ȣ��  
		//			
		//////////////////////////////////////////////////////////////////////////
		//���� txt
		//////////////////////////////////////////////////////////////////////////
		
		
		///-- ���� 
		text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND, Text_StartX, Text_StartY+(TEXT_HEIGHT_INTERVAL*0), 80, 13, false );
		text->SetFontR( n_fRsvFontWndStatic );
		text->SetTextColor( TEXT_COLOR_STATIC );	
		text->SetAlignText( n_atLeft, n_atCenter );	
		Rsprintf( RWCHAR(temp), _RT("%s:"), G_STRING_CHAR( IDS_CHARSTATE_LEVEL2 ));				// IDS_CHARSTATE_LEVEL2
		text->SetText( temp );
		
		///-- ���� 
		text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND, Text_StartX, Text_StartY+(TEXT_HEIGHT_INTERVAL*1), 80, 13, false );
		text->SetFontR( n_fRsvFontWndStatic );
		text->SetTextColor( TEXT_COLOR_STATIC );	
		text->SetAlignText( n_atLeft, n_atCenter );	
		Rsprintf( RWCHAR(temp), _RT("%s:"), G_STRING_CHAR( IDS_PET_CLASS ));				//IDS_FRONTIERMAIN_CREATURETYPE
		text->SetText( temp );
		
		///-- ü�� 
		text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETMAIN_WND, Text_StartX, Text_StartY+(TEXT_HEIGHT_INTERVAL*2), 80, 13, false );
		text->SetFontR( n_fRsvFontWndStatic );
		text->SetTextColor( TEXT_COLOR_STATIC );	
		text->SetAlignText( n_atLeft, n_atCenter );	
		Rsprintf( RWCHAR(temp), _RT("%s:"), G_STRING_CHAR( IDS_PET_HP ) );				
		text->SetText( temp );
		
		// ���ŷ� 
		text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETMAIN_WND, Text_StartX, Text_StartY+(TEXT_HEIGHT_INTERVAL*3), 80, 13, false );
		text->SetFontR( n_fRsvFontWndStatic );
		text->SetTextColor( TEXT_COLOR_STATIC );	
		text->SetAlignText( n_atLeft, n_atCenter );	
		Rsprintf( RWCHAR(temp), _RT("%s:"), G_STRING_CHAR( IDS_PET_CP ) );				
		text->SetText( temp );		
		
		// ����ġ 
		text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETMAIN_WND, Text_StartX, Text_StartY+(TEXT_HEIGHT_INTERVAL*4), 80, 13, false );
		text->SetFontR( n_fRsvFontWndStatic );
		text->SetTextColor( TEXT_COLOR_STATIC );	
		text->SetAlignText( n_atLeft, n_atCenter );	
		Rsprintf( RWCHAR(temp), _RT("%s:"),G_STRING_CHAR( IDS_PET_EXP ) );				
		text->SetText( temp );
		
		int nStartCount = 5;
		
		if( i == MERCE_SLOT_NUM )
		{
			nStartCount = 4;			
			text->SetVisible(false);
		}
		
		// ���ݷ� 
		//�� ������� 3���� ���ѵд� �̰��� �ȴþ� ���°��̿� ~~
		//�����
		
		m_PetText[i].NameAbilityTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETMAIN_WND, Text_StartX, Text_StartY+( TEXT_HEIGHT_INTERVAL * nStartCount ), 80, 13, false );;
		m_PetText[i].NameAbilityTxt->SetFontR( n_fRsvFontWndStatic );
		m_PetText[i].NameAbilityTxt->SetTextColor( TEXT_COLOR_STATIC );	
		m_PetText[i].NameAbilityTxt->SetAlignText( n_atLeft, n_atCenter );	
		Rsprintf( RWCHAR(temp), _RT("%s:"), _RT("���ݷ�") );				
		m_PetText[i].NameAbilityTxt->SetText( temp );
		m_PetText[i].NameAbilityTxt->SetHID(HID_None);
		
		nStartCount++;
		
		//������ 
		m_PetText[i].NameProbabilityTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETMAIN_WND, Text_StartX, Text_StartY+( TEXT_HEIGHT_INTERVAL * nStartCount ), 80, 13, false );;
		m_PetText[i].NameProbabilityTxt->SetFontR( n_fRsvFontWndStatic );
		m_PetText[i].NameProbabilityTxt->SetTextColor( TEXT_COLOR_STATIC );	
		m_PetText[i].NameProbabilityTxt->SetAlignText( n_atLeft, n_atCenter );	
		Rsprintf( RWCHAR(temp), _RT("%s:"), _RT("������") );				
		m_PetText[i].NameProbabilityTxt->SetText( temp );
		m_PetText[i].NameProbabilityTxt->SetHID(HID_None);
		
		//////////////////////////////////////////////////////////////////////////
		//������ txt
		//////////////////////////////////////////////////////////////////////////
		/*
		
		  ���� 
		  ���� 
		  ü�� 
		  ���ŷ�
		  ���ݷ�  -- �ٲ�� �ؽ�Ʈ ���� ������ ��ƾ��Ѵ� . 
		  ������  -- �ٲ�� �ؽ�Ʈ ���� ������ ��ƾ��Ѵ� .
		  ����ġ 
		*/
		//-����ð�
		m_PetText[i].PetDieTimeTxt= (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND, 
																0 , (Text_StartY-16), 300, 13, false );
		m_PetText[i].PetDieTimeTxt->SetFontR(n_fRsvFontWndStatic );
		m_PetText[i].PetDieTimeTxt->SetTextColor( 255, 0, 0 );	
		m_PetText[i].PetDieTimeTxt->SetClientImage( TID_None );	
		m_PetText[i].PetDieTimeTxt->SetAlignText( n_atCenter, n_atCenter );		
		m_PetText[i].PetDieTimeTxt->SetText( _RT("�ϳ���") );		
		m_PetText[i].PetDieTimeTxt->SetFocusCheck( false );		
		
		///--�̸� 
		m_PetText[i].NameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND,
														   0, (Text_StartY-16), 300, 13, false );
		m_PetText[i].NameTxt->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
		m_PetText[i].NameTxt->SetTextColor( 0, 0, 0 );	
		m_PetText[i].NameTxt->SetClientImage( TID_None );	
		m_PetText[i].NameTxt->SetAlignText( n_atCenter, n_atCenter );		
		m_PetText[i].NameTxt->SetText( _RT("�ϳ���") );		
		m_PetText[i].NameTxt->SetFocusCheck( false );
		
		///-- ���� 
		m_PetText[i].LevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND, 
															Text_StartRX, Text_StartY+(TEXT_HEIGHT_INTERVAL*0), 100, 13, false );
		m_PetText[i].LevelTxt->SetFontR( n_fRsvFontWndStatic );
		m_PetText[i].LevelTxt->SetTextColor( TEXT_COLOR_STATIC );	
		m_PetText[i].LevelTxt->SetAlignText( n_atCenter, n_atCenter );	
		m_PetText[i].LevelTxt->SetText( _RT("99"));
		
		
		m_PetText[i].TypeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND, 
															Text_StartRX, Text_StartY+(TEXT_HEIGHT_INTERVAL*1), 100, 13, false );
		m_PetText[i].TypeTxt->SetFontR( n_fRsvFontWndStatic );
		m_PetText[i].TypeTxt->SetTextColor( TEXT_COLOR_STATIC );	
		m_PetText[i].TypeTxt->SetAlignText( n_atCenter, n_atCenter );	
		m_PetText[i].TypeTxt->SetText( _RT("������"));
		
		
		m_PetText[i].HpTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND, 
														Text_StartRX, Text_StartY+(TEXT_HEIGHT_INTERVAL*2), 100, 13, false );
		m_PetText[i].HpTxt->SetFontR( n_fRsvFontWndStatic );
		m_PetText[i].HpTxt->SetTextColor( TEXT_COLOR_STATIC );	
		m_PetText[i].HpTxt->SetAlignText( n_atCenter, n_atCenter );	
		m_PetText[i].HpTxt->SetText( _RT("999/999") );
		
		
		m_PetText[i].CpTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND, 
														 Text_StartRX, Text_StartY+(TEXT_HEIGHT_INTERVAL*3), 100, 13, false );
		m_PetText[i].CpTxt->SetFontR( n_fRsvFontWndStatic );
		m_PetText[i].CpTxt->SetTextColor( TEXT_COLOR_STATIC );	
		m_PetText[i].CpTxt->SetAlignText( n_atCenter, n_atCenter );	
		m_PetText[i].CpTxt->SetText( _RT("999/999") );
		
		m_PetText[i].ExpTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND, 
														  Text_StartRX, Text_StartY+(TEXT_HEIGHT_INTERVAL*4), 100, 13, false );
		m_PetText[i].ExpTxt->SetFontR( n_fRsvFontWndStatic );
		m_PetText[i].ExpTxt->SetTextColor( TEXT_COLOR_STATIC );	
		m_PetText[i].ExpTxt->SetAlignText( n_atCenter, n_atCenter );	
		m_PetText[i].ExpTxt->SetText( _RT("9999/9999") );
		
		nStartCount = 5;
		
		if( i == MERCE_SLOT_NUM )
		{
			nStartCount = 4;			
			m_PetText[i].ExpTxt->SetVisible(false);
		}
		
		m_PetText[i].AbilityTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND, 
															  Text_StartRX, Text_StartY+(TEXT_HEIGHT_INTERVAL*nStartCount), 100, 13, false );
		m_PetText[i].AbilityTxt->SetFontR( n_fRsvFontWndStatic );
		m_PetText[i].AbilityTxt->SetTextColor( TEXT_COLOR_STATIC );	
		m_PetText[i].AbilityTxt->SetAlignText( n_atCenter, n_atCenter );	
		m_PetText[i].AbilityTxt->SetText( _RT("999") );
		
		nStartCount++;
		
		m_PetText[i].ProbabilityTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETMAIN_WND, 
																  Text_StartRX, Text_StartY+(TEXT_HEIGHT_INTERVAL*nStartCount), 100, 13, false );
		m_PetText[i].ProbabilityTxt->SetFontR( n_fRsvFontWndStatic );
		m_PetText[i].ProbabilityTxt->SetTextColor( TEXT_COLOR_STATIC );	
		m_PetText[i].ProbabilityTxt->SetAlignText( n_atCenter, n_atCenter );	
		m_PetText[i].ProbabilityTxt->SetText( _RT("999") );
		
		if( i == MERCE_SLOT_NUM )
		{
			m_PetText[i].pRemainTimeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETMAIN_WND,
				7+14 , Text_StartY+ ( TEXT_HEIGHT_INTERVAL * 7 ) - 5, 100, 13, false );
			m_PetText[i].pRemainTimeTxt->SetFontR( n_fRsvFontWndStatic );
			m_PetText[i].pRemainTimeTxt->SetTextColor( 50, 255, 50 );	
			m_PetText[i].pRemainTimeTxt->SetAlignText( n_atLeft, n_atCenter );	
			m_PetText[i].pRemainTimeTxt->SetText( _RT("�����ð� 1�� 5��") );
		}
		else
		{
			m_PetText[i].pRemainTimeTxt = NULL;
		}


		int nBtnXPos = 242;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
		nBtnXPos = 342;

#endif	
		
		//////////////////////////////////////////////////////////////////////////
		//��ư ��� �̹��� 
		//////////////////////////////////////////////////////////////////////////
		///--��� �̹��� 	
		m_PetText[i].ImgReCall = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PETMAIN_WND, 
			nBtnXPos, Text_StartY+(TEXT_HEIGHT_INTERVAL*1), 35, 35, false );
		m_PetText[i].ImgReCall->SetClientImage( TID_C_BGSLOT_INVEN_ONE );
		
		m_PetText[i].ImgInven = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None,  WID_PETMAIN_WND, 
			nBtnXPos, Text_StartY+(TEXT_HEIGHT_INTERVAL*4), 35, 35, false );
		m_PetText[i].ImgInven->SetClientImage( TID_C_BGSLOT_INVEN_ONE );
		
		///-- ��ȯ â 
		m_PetText[i].ReCallBtn  = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PETMAIN_WND, 
			nBtnXPos + 5, Text_StartY+(TEXT_HEIGHT_INTERVAL*1)+6, 24, 22, false );     
		m_PetText[i].ReCallBtn->SetBtnImage( TID_icon_pet , TID_icon_pet_bright ,TID_icon_pet_dark , TID_None );
		m_PetText[i].ReCallBtn->SetHID( HID_PET_RECALL_BTN );
		//--��ȯ ����
		m_PetText[i].CallOffBtn  = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PETMAIN_WND, 
			nBtnXPos + 3, Text_StartY+(TEXT_HEIGHT_INTERVAL*1)+6, 26, 22, false );     
		m_PetText[i].CallOffBtn->SetBtnImage( TID_icon_pet_cancel, TID_icon_pet_cancel_bright,TID_icon_pet_cancel_dark , TID_None );
		m_PetText[i].CallOffBtn->SetHID( HID_PET_CALLOFF_BTN );
		
		///-- ����ǰâ
		m_PetText[i].InvenBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None,WID_PETMAIN_WND, 
			nBtnXPos + 5, Text_StartY+(TEXT_HEIGHT_INTERVAL*4)+6, 24, 22, false ); 
		m_PetText[i].InvenBtn->SetBtnImage( TID_icon_pet_bag, TID_icon_pet_bag_bright, TID_icon_pet_bag_dark, TID_None );
		m_PetText[i].InvenBtn->SetHID( HID_PET_INVEN_BTN );
		
		///-- �뺴 ������ ����ǰâ�� �������� �ʴ´�.
		if( i == MERCE_SLOT_NUM )
		{
			m_PetText[i].ImgInven->SetVisible( false );
			m_PetText[i].InvenBtn->SetVisible( false );
		}	
	}
	
	for(short i =0 ; i < MAX_PET_SLOT_NUM ; ++i )
	{
		for(int PetInc = 0 ; PetInc < RP_MAX_PET_USEDITEM ; ++PetInc)
		{
			m_PetText[i].KeepupItemSlot[PetInc]->PcItem =  NULL;
		}		
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	
	
	/********************************************************************
	- ��ȯ ���� �Ͻðڽ��ϱ� ??
	*********************************************************************/
	
	m_pFrameOffWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_PET_CALLOFF_WND, WID_None, 394, 150, 214, 95, false );
	m_pFrameOffWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameOffWnd->SetClientImage( TID_CTRL_WND_L1 );
	///fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pFrameOffWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameOffWnd->SetWndTileSizeTB( 31, 11 );	
	m_pFrameOffWnd->SetVisible(false);
	m_pFrameOffWnd->SetCaption(   G_STRING_CHAR(IDS_PET_CALLOFF_WND));  
	m_pFrameOffWnd->SetWndProc( this );	
	m_pFrameOffWnd->SetTitle( true );
	m_pFrameOffWnd->SetMove( true );	
	m_pFrameOffWnd->Initialize();	
	
	
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PET_CALLOFF_WND, 5, 35 , 214, 30, false );
	text->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	text->SetTextColor( 0,120, 120 );	
	text->SetClientImage( TID_None );	
	text->SetAlignText( n_atCenter, n_atCenter );		
	text->SetText( _T(G_STRING_CHAR(IDS_PET_ISCALLOFF)) );
	
	
	m_OkBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PET_CALLOFF_WND,  46, 66, 58, 25, false );      
	m_OkBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_OkBtn->SetFontR( n_fRsvFontWndButton );
	m_OkBtn->SetText( G_STRING_CHAR(IDS_WND_COM_OK));	
	//	m_OkBtn->SetHID( 455 );
	
	m_CancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None,WID_PET_CALLOFF_WND, 112, 66, 58, 25, false ); 
	m_CancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_CancelBtn->SetFontR( n_fRsvFontWndButton );
	m_CancelBtn->SetText( G_STRING_CHAR(IDS_WND_COM_CANCEL));	
	//	m_CancelBtn->SetHID( 456 );
	
	
	Init();
	ClearPetMainWnd();
	
}

//���� ����¸� ������Ʈ ��Ų�� �ѹ��� 
void CPetMainWndProc::PetUpdateState()
{
	int i = 0;
	
	for( i = 0 ; i < MAX_PET_SLOT_NUM ; ++i)
	{
		if( PcInven->thePetData[i] != NULL )		
		{
			switch(PcInven->thePetData[i]->PetState)
			{
			case n_Pet_Die:
				{
					m_PetText[i].NameTxt->SetVisible(false);
					m_PetText[i].PetDieTimeTxt->SetVisible(true);
					
				}
				break;
			case n_Pet_Live:
				{
					m_PetText[i].NameTxt->SetVisible(true);
					m_PetText[i].PetDieTimeTxt->SetVisible(false);
					
				}
				break;
			case n_Pet_Custody:
				{
					m_PetText[i].NameTxt->SetVisible(false);
					m_PetText[i].PetDieTimeTxt->SetVisible(true);
				}
				break;	
			}
			
			
			if( m_PetText[i].pRemainTimeTxt )
			{
				//m_PetText[i].pRemainTimeTxt->SetVisible(true);
			}
		}
		
		///-- �뺴 ������ �̸��� ��� ����ֱ�
		if( i == MERCE_SLOT_NUM )
		{
			m_PetText[i].NameTxt->SetVisible(true);
			m_PetText[i].PetDieTimeTxt->SetVisible(false);			
		}		
	}
}


void CPetMainWndProc::ClearPetData()
{
	int i =0 ; 
	
	for( i = 0; i < MAX_PET_SLOT_NUM ; ++i)
	{
		if(PcInven->thePetData[i] != NULL)
		{
			//���̹��� ���� ���ش� .. 
			
			g_Pc_Manager.m_CharImage.Delete( PcInven->thePetData[i]->NpcCode ,c_NpcRace_Mob, false);
			g_Pc_Manager.m_CharImage.Delete( PcInven->thePetData[i]->NpcCode,c_NpcRace_Mob, true);
			
			PcInven->thePetData[i]->Clear();
			SAFE_DELETE(PcInven->thePetData[i]);
		}	
	}
}


void CPetMainWndProc::LogOff()
{
	
	//	
	Close();
	
	g_PetInfoWnd.Close();
	g_PetInfoWnd.ClearPetInfo();
	g_PetInventoryWnd.Close();	
	
	ClearPetData();
	ClearPetMainWnd();
	
	//SPetData ���½�Ų�� .. 
}


void CPetMainWndProc::Open()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
}

void CPetMainWndProc::Close()
{
	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}



//RP_MAX_PET_USEDITEM
//========================================================================================================
void CPetMainWndProc::SetKeepUpItemCode( int aNum, SPayEffect aPayEffect , int PetSlotNum )
{
	
	PcInven->thePetData[PetSlotNum]->KeepupItem[aNum].IType		= n_ITYPE_SPECIAL;
	PcInven->thePetData[PetSlotNum]->KeepupItem[aNum].Class		= n_Class_Use;
	PcInven->thePetData[PetSlotNum]->KeepupItem[aNum].ItemUID		= 0;
	PcInven->thePetData[PetSlotNum]->KeepupItem[aNum].Code			= aPayEffect.theItemCode;
	PcInven->thePetData[PetSlotNum]->KeepupItem[aNum].Tid			= (TEXID) g_Pc_Manager.GetItemTID( aPayEffect.theItemCode );
	PcInven->thePetData[PetSlotNum]->KeepupItem[aNum].TidEx		= (TEXID) g_Pc_Manager.GetItemTID( aPayEffect.theItemCode );
	PcInven->thePetData[PetSlotNum]->KeepupItem[aNum].Amount		= 1;
	PcInven->thePetData[PetSlotNum]->KeepupItem[aNum].ItemTable	= g_Pc_Manager.GetItemBaseInfoPtr( aPayEffect.theItemCode );
	
	m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem = &PcInven->thePetData[PetSlotNum]->KeepupItem[aNum];
	m_PetText[PetSlotNum].KeepupItemSlot[aNum]->SetVisible( TRUE );
	m_PetText[PetSlotNum].KeepupItemText[aNum]->SetVisible( TRUE );
	
	// ���� �ð� ǥ�� 
	SItemBaseInfo* item_Info = NULL;
	item_Info = m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->ItemTable;
	
	//���̺� ����� �ð��� �����´� .. 
	// AllUseTime  �ѽð� ���� ���Ѵ� 
	//	item_Info->theReGage = 30 ;
	
	DWORD AllUseTime = item_Info->theReGage * 1000.0f;							// Sec
	AllUseTime += item_Info->theReMPSpeed * 1000.0f * 60.0f;					// Min
	AllUseTime += item_Info->theReSPSpeed * 1000.0f * 60.0f * 60.0f;			// Hour
	AllUseTime += item_Info->theReHPSpeed * 1000.0f * 60.0f * 60.0f * 24.0f;	// Day 

#ifdef PET_PAY_USEITEM_RENEWAL

	m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->IsDelay = true;
	m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->DelayTime = AllUseTime / 1000.0f;
	m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->ProgressTime = AllUseTime;	
	m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->CumulativeTime = AllUseTime - aPayEffect.theTime;	// ��� 
	
#else 
	if( AllUseTime <= c_Max_DrawKeepupItemTime ) // �����������
	{
		//1�ð� �����϶� 
		m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->IsDelay = true;
		m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->DelayTime = AllUseTime / 1000.0f;
		m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->ProgressTime = AllUseTime;	
		m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->CumulativeTime = AllUseTime - aPayEffect.theTime;	// ��� 
	}

	else
	{	
		m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->IsDelay = false;
		m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->DelayTime = AllUseTime / 1000;			// ���� ��� �ð� ���� 
		m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->ProgressTime = AllUseTime / 1000.0f;		// �ʴ����� ������ �ִٰ� ���� �ð��� 1�ð� ������ ���������� �����ֱ� ���ؼ� * 1000.0f �� ����Ѵ�. 
		m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->CumulativeTime = AllUseTime - aPayEffect.theTime;	// ��� 
		m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem->Amount = ( aPayEffect.theTime / c_Max_DrawKeepupItemTime ) + 1;
	}
#endif //PET_PAY_USEITEM_RENEWAL	
	 
	return;
}


void CPetMainWndProc::DeleteKeepUpItemCode( int aNum ,int PetSlotNum)
{
	m_PetText[PetSlotNum].KeepupItemSlot[aNum]->PcItem = NULL;
	m_PetText[PetSlotNum].KeepupItemSlot[aNum]->SetVisible( FALSE );
	m_PetText[PetSlotNum].KeepupItemText[aNum]->SetVisible( FALSE );
	m_PetText[PetSlotNum].fRemainTime[aNum] = 0.0f;
}


//���ӽ�ų�� ���� ��Ų�� .. 
void CPetMainWndProc::SortKeepupItem(int PetSlotNum)
{
	
	int i = 0;
	int	idx = 0;
	SPayEffect tmpPayEffect[ RP_MAX_PET_USEDITEM ];
	
	
	memset( tmpPayEffect, 0, sizeof( SPayEffect ) * RP_MAX_PET_USEDITEM );
	
	for( i = 0; i < RP_MAX_PET_USEDITEM; ++i )
	{
		if( PcInven->thePetData[PetSlotNum]->PayEffect[i] )
		{
			SItemBaseInfo* item_Info = NULL;
			item_Info = m_PetText[PetSlotNum].KeepupItemSlot[i]->PcItem->ItemTable;			
			
			//AllUseTIme �� 1���� ��� ���� ���°� ... dongs // 
			
			float AllUseTime = item_Info->theReGage * 1000.0f;							// Sec
			AllUseTime += item_Info->theReMPSpeed * 1000.0f * 60.0f;					// Min
			AllUseTime += item_Info->theReSPSpeed * 1000.0f * 60.0f * 60.0f;			// Hour
			AllUseTime += item_Info->theReHPSpeed * 1000.0f * 60.0f * 60.0f * 24.0f;	// Day 
			
			
			//UseTime �� 1�� �ð����� ũ�ų� && �����̰� true �̸� ...???????
			if( AllUseTime > c_Max_DrawKeepupItemTime && m_PetText[PetSlotNum].KeepupItemSlot[i]->PcItem->IsDelay == true )
			{																											//�����ð� * �ִ�ð� �� 
				PcInven->thePetData[PetSlotNum]->PayEffect[i]->theTime = 
					AllUseTime - (m_PetText[PetSlotNum].KeepupItemSlot[i]->PcItem->CumulativeTime + c_Max_DrawKeepupItemTime);
			}
			else
			{
				PcInven->thePetData[PetSlotNum]->PayEffect[i]->theTime = AllUseTime - m_PetText[PetSlotNum].KeepupItemSlot[i]->PcItem->CumulativeTime;
			}
			//tmp�� �ϴ� ���� ��Ű�� .. 
			tmpPayEffect[idx] = *(PcInven->thePetData[PetSlotNum]->PayEffect[i] );			
			++idx;
			
			//�ʱ�ȭ .. 
			PcInven->thePetData[PetSlotNum]->PayEffect[i]->theItemCode = 0;			
			PcInven->thePetData[PetSlotNum]->PayEffect[i]->theTime  = 0;			
			DeleteKeepUpItemCode( i , PetSlotNum );
			SAFE_DELETE( PcInven->thePetData[PetSlotNum]->PayEffect[i] );
		}
	}	
	
	for( i = 0; i < idx; ++i )
	{
		SetPayEffect( tmpPayEffect[i] , PetSlotNum);
	}
	
	return; 
}
//���� ������ ��� 
//-----------------------------------------------------------------------------
void CPetMainWndProc::SetPayEffect( SPayEffect aPayEffect ,int PetSlotNum)
{
	for( int i = 0; i < RP_MAX_PET_USEDITEM; ++i )
	{
		if( PcInven->thePetData[PetSlotNum]->PayEffect[i] == NULL )
		{
			PcInven->thePetData[PetSlotNum]->PayEffect[i] = SAFE_NEW( SPayEffect );			
			PcInven->thePetData[PetSlotNum]->PayEffect[i]->theItemCode = aPayEffect.theItemCode;
			PcInven->thePetData[PetSlotNum]->PayEffect[i]->theTime  = aPayEffect.theTime;
			
			SetKeepUpItemCode( i , aPayEffect , PetSlotNum );
			
			break;
		}
	}
	
	return;
}

//-----------------------------------------------------------------------------
void CPetMainWndProc::DelPayEffect( SPayEffect aPayEffect , int PetSlotNum)
{		
	for( int i = 0; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		if(PcInven->thePetData[PetSlotNum]->PayEffect[i] )
		{
			if(PcInven->thePetData[PetSlotNum]->PayEffect[i]->theItemCode == aPayEffect.theItemCode )
			{
				PcInven->thePetData[PetSlotNum]->PayEffect[i]->theItemCode = 0;
				PcInven->thePetData[PetSlotNum]->PayEffect[i]->theTime  = 0;
				
				DeleteKeepUpItemCode( i , PetSlotNum);
				break;
			}
		}
	}
	
	SortKeepupItem(PetSlotNum);
	
	return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//���̸� ���� .. 



CPetCreateWnd::CPetCreateWnd()
{
	
	memset( PetName ,0, sizeof(TCHAR) * 256 );
	BufLen = 256;	
}

void CPetCreateWnd::Init()
{
	
}

void CPetCreateWnd::Update()
{
	
	
}

void CPetCreateWnd::Composition()
{
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;
	
	CTextBox*		tbox = NULL;
	CImageBox*		img;
	CImageBox9*		img9;
	
	char temp[256] = {0,};
	
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_PETCREATE_WND, WID_None, 394, 315, 235, 138, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	///fw->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->SetCaption( G_STRING_CHAR(IDS_NEW_CREATE) );  
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetMove( true );	
	m_pFrameWnd->Initialize();	
	
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_PETCREATE_WND, 1, 45, 233, 56, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None,WID_PETCREATE_WND,  1, 44, 233, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None,WID_PETCREATE_WND, 1, 101, 233, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_PETCREATE_WND, 67, 50, 100, 23, false );       	
	img9->SetWndTileSizeLR( 9, 9 );
	img9->SetWndTileSizeTB( 8, 8 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );
	
	
	//	///-- Image9
	//	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_PETCREATE_WND, 14, 74, 212, 23, false );       	
	//	img9->SetWndTileSizeLR( 9, 9 );
	//	img9->SetWndTileSizeTB( 8, 8 );
	//	img9->SetFocusCheck( false );
	//	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	//	img9->SetAdjustPosition( 0, 0 );
	
	///-- ���̵� ( Static Text )
	//	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_PETCREATE_WND, 28, 36, 23, 17, false );
	//	tbox->SetTextColor(,0,0 );	
	//	tbox->SetFontR( n_fRsvFontWndStatic2 );
	//	tbox->SetAlignText( n_atLeft, n_atCenter );
	//	tbox->SetText( G_STRING_CHAR(IDS_MSGBOX_INPUT_NAME) );
	
	m_EffectText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None,WID_PETCREATE_WND, 10, 74 , 10, 23, false );
	m_EffectText->SetFontR( n_fRsvFontWndStatic );
	m_EffectText->SetTextColor(255,0,0 );	
	m_EffectText->SetAlignText( n_atLeft, n_atCenter );	
	m_EffectText->SetText( G_STRING_CHAR(IDS_SYSPR_ONCE_NAME_NOTCHANGE) );
	
	m_NameEdit = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, WID_PETCREATE_WND, 67, 55 , 120, 17, false );
	m_NameEdit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_NameEdit->SetTextSize( 16 );	
	m_NameEdit->SetClientImage( TID_None );
	m_NameEdit->SetClientColor( 0, 0, 0 );
	m_NameEdit->SetAlignText( n_atLeft, n_atCenter );
	m_NameEdit->SetBlank( 2, 0, 0, 0 );
	m_NameEdit->SetEditStyle( esText );
	m_NameEdit->SetText( _RT("") );
	m_NameEdit->SetReturnActionClear( false );
	m_NameEdit->InitEditBox();	
	m_NameEdit->SetHID(HID_PET_NAME_CREATE_ED ); //dongs �߰� 
	
	m_OkBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PETCREATE_WND,  56, 108, 58, 25, false );      
	m_OkBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_OkBtn->SetFontR( n_fRsvFontWndButton );
	m_OkBtn->SetText( G_STRING_CHAR(IDS_WND_COM_OK));	
	m_OkBtn->SetHID( HID_PET_CREAT_BTN );
	
	m_CancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_PETCREATE_WND, 122, 108, 58, 25, false ); 
	m_CancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );
	m_CancelBtn->SetFontR( n_fRsvFontWndButton );
	m_CancelBtn->SetText( G_STRING_CHAR(IDS_WND_COM_CANCEL));	
	m_CancelBtn->SetHID( HID_PET_CANCEL_BTN );
	
	
	return;
}

void CPetCreateWnd::Proc( SEventMessage* EMsg )
{
	//	RLGS("CPetCreateWnd");
	
	switch( EMsg->Notify )
	{
	case NM_EDT_RETURN_ACTION:
		if( (WNDID)EMsg->FocusWnd == m_NameEdit->GetID() )
		{
			
			//	nRui->SendEvent(  m_NameEdit->GetID(), n_emGetText, (EPARAM)PetName, BufLen, 0, 0 );
			m_NameEdit->GetText(PetName,BufLen);
			nRui->SetEnterFlag( n_nfEditEnter );
			
			if(CreateNameAndVerification())
			{ 
				//	_tcscat(PetName,_RT(" "));
				g_RockClient.Send_CsMakePetName(PetName,false);	
				
				Close();
			}
			else
			{
				
				nRui->SendEvent( m_NameEdit->GetID() , 	n_emSetFocus, 0, 0, 0, 0 );
				nRui->SendEvent( m_NameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
				nRui->SendEvent( m_NameEdit->GetID() , 	n_emEditDisable, 0, 0, 0, 0 );
			}
			
			
		}
		break;
		
	case NM_BTN_CLICK:
		if( (WNDID)EMsg->FocusWnd == m_OkBtn->GetID() )
		{
			//	nRui->SendEvent( m_NameEdit->GetID(), n_emGetText, (EPARAM)PetName, BufLen, 0, 0 );
			m_NameEdit->GetText(PetName,BufLen);
			if(CreateNameAndVerification())
			{
				//_tcscat(PetName,_RT(" "));
				g_RockClient.Send_CsMakePetName(PetName,false);	
				
				Close();
			}
			else
			{
				
				nRui->SendEvent( m_NameEdit->GetID() , 	n_emSetFocus, 0, 0, 0, 0 );
				nRui->SendEvent( m_NameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
				nRui->SendEvent( m_NameEdit->GetID() , 	n_emEditDisable, 0, 0, 0, 0 );
			}
			
		}
		
		if( (WNDID)EMsg->FocusWnd == m_CancelBtn->GetID() )
		{
			Rstrcpy(PetName,_RT(" "));
			g_RockClient.Send_CsMakePetName(PetName,true);	
			Close();
		}
		
		
		break;
	}
	
	
}

bool CPetCreateWnd::CreateNameAndVerification()
{
	
	
	//����˻� 
	if( IsCheckInSpace( PetName ) )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_REINPUT_ID ), 1000, NULL );
		return false;
	}
	
	
	if( Rstrlen( RWCHAR(PetName) ) <= 0 )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_INPUT_NAME ) , 1500 , NULL );		
		return false;
	}
	
	//Ư������ ���� �˻� 
	//	if( !g_RockClient.m_SlangFilter.IsValidString( PetName ) )
	//	{
	//		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
	//		
	//		return false;
	//	}
	//	
	//	// ����Ҽ� ���� �ܾ� �˻� 
	//	if( g_RockClient.m_SlangFilter.ISFindFilterName( PetName ) )
	//	{
	//		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
	//		
	//		return false;
	//	}
	// ";%<>- ��� ���� 
	
	if(IsCheckSymbol(G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ), RWCHAR(PetName)))
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_PET_CREATE_NOT_WRITE ) , 1500 , NULL );
		return false;
	}

	if( !g_RockClient.m_SlangFilter.IsValidString( PetName ) )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SPECIAL_TEXT ) , 1500 , NULL );
		return false;
	}		
	
	if( g_RockClient.m_SlangFilter.ISFindFilterName( PetName ) )
	{
		MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_INVALID_TEXT ) , 1500 , NULL );
		return false;
	}
	
	// ���͸� 
	if( g_RockClient.m_SlangFilter.Filter( PetName ) )
	{
		//�弳�� ������ 
		if( g_RockClient.m_SlangFilter.ISSlangFound() )
		{
			MessageBoxWnd.Open( G_STRING_CHAR( IDS_MSGBOX_NAME_SLANG_TEXT ) , 1500 , NULL );
			return false;
		}
	}
	
	
	return true;
	
}

void CPetCreateWnd::Close()
{
	
		nRui->SendEvent( m_NameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
		nRui->SendEvent( m_pFrameWnd->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );

}


void CPetCreateWnd::Open()
{
		
		nRui->SendEvent( m_pFrameWnd->GetID() , n_emSetVisible, (EPARAM)true, 0, 0, 0 );
		nRui->SendEvent( m_NameEdit->GetID() , 	n_emSetFocus, 0, 0, 0, 0 );
		nRui->SendEvent( m_NameEdit->GetID() , 	n_emClearText, 0, 0, 0, 0 );
				
}


int CPetCreateWnd::IsCheckInSpace(void *pStr)
{
	RTCHAR *Str = RWCHAR(pStr);

	int Len = Rstrlen( Str );

	for( int i = 0 ; i < Len ; ++i )
	{
		if( Str[ i ] == ' ' )
			return TRUE;
	}

	return FALSE;
}

bool CPetCreateWnd::IsCheckSymbol(void *pSrcStr , void *pDstStr)
{
	RTCHAR *SrcStr = RWCHAR(pSrcStr);
	RTCHAR *DstStr = RWCHAR(pDstStr);

	int SrcLen = Rstrlen(SrcStr);
	int DstLen = Rstrlen(DstStr);
	
	int Dinx = 0 ; 
	int Sinx = 0 ; 

	for( Sinx  =0 ; Sinx< SrcLen ; ++Sinx )
	{
		for(Dinx = 0 ; Dinx < DstLen ; ++Dinx)
		{
			if( SrcStr[Sinx] == DstStr[Dinx])
			{
				return true;				
			}
			
		}
		
	}
	return false; 

}