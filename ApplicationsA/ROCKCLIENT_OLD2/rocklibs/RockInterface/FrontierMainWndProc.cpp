#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"
#include "MainMenuWndProc.h"

#include "..\\..\\GTimer.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"


#include "FrontierMainWndProc.h"
#include "StringManager.h"
#include "InventoryWndProc.h"
#include "MessageBox.h"
#include "MessageWndProc.h"

#include "CWebShopWnd.h"
#include "UiUtil.h"

#include "ChatMainProc.h"
#include "NonPlayerInfoAddWnd.h"
#include "UIMsgWnd.h"
#include "MapDataManager.h"


CFrontierMainWnd	g_FrontierMainWnd;
CFrontierMemberListWnd	g_FrontierMemberListWnd;
CFrontierMemberEditWnd  g_FrontierMemberEditWnd;
CFrontierAddPersons g_FrontierAddPersons;
#ifdef DECO_RENEWAL_GUILD_MJH
CFrontierMenuBtnWnd  g_FrontierMenuBtnWnd;
#endif // DECO_RENEWAL_GUILD_MJH


#define TEXT_COLOR_TITLE 255, 255, 200


#ifdef DECO_RENEWAL_GUILD_MJH
	#define TEXT_COLOR_STATIC 0, 0, 0
	#define TEXT_COLOR_VALUE1 0, 0, 0
	#define TEXT_COLOR_VALUE2 0, 0, 0
#else 
	#define TEXT_COLOR_STATIC 225, 225, 225
	#define TEXT_COLOR_VALUE1 225, 225, 255
	#define TEXT_COLOR_VALUE2 225, 255, 225
#endif // DECO_RENEWAL_GUILD_MJH

const int FRONTIER_NEED_MEMBER = 50;

extern long g_nowTime;

//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
// Sort Function 
//=============================================================================================================
//=============================================================================================================

static bool	g_IsAscend_Sort = false; // or Descend_Sort	

int SortMemberNameList( const void *arg1, const void *arg2 );
int SortMemberNickNameList( const void *arg1, const void *arg2 );
int SortMemberGradeList( const void *arg1, const void *arg2 );
int SortMemberAccumulationFeeList( const void *arg1, const void *arg2 );
int SortMemberFameList( const void *arg1, const void *arg2 );
#ifdef DECO_RENEWAL_GUILD_MJH
int SortMemberLevelList( const void *arg1, const void *arg2 );
int SortMemberMapPositionList( const void *arg1, const void *arg2 );
#endif // DECO_RENEWAL_GUILD_MJH

int SortMemberNameList( const void *arg1, const void *arg2 )
{
	SFMemberInfo* a1 = ( SFMemberInfo* ) arg1;
	SFMemberInfo* a2 = ( SFMemberInfo* ) arg2;

	int comp_result = 0;
	

	if( g_IsAscend_Sort )
	{
		comp_result = Rstrcmp( a1->Name, a2->Name );

		if( comp_result < 0 )
		{
			return -1;
		}
		else if( comp_result > 0 )
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		comp_result = Rstrcmp( a1->Name, a2->Name );

		if( comp_result < 0 )
		{
			return 1;			
		}
		else if( comp_result > 0 )
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

#ifdef DECO_RENEWAL_GUILD_MJH

int SortMemberLevelList( const void *arg1, const void *arg2 )
{
	SFMemberInfo* a1 = ( SFMemberInfo* ) arg1;
	SFMemberInfo* a2 = ( SFMemberInfo* ) arg2;

	if( g_IsAscend_Sort )
	{
		if( a1->Level == a2->Level )
		{
			return 0;
		}
		else if( a1->Level < a2->Level )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( a1->Level == a2->Level )
		{
			return 0;
		}
		else if( a1->Level > a2->Level )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

int SortMemberMapPositionList( const void *arg1, const void *arg2 )
{
	SFMemberInfo* a1 = ( SFMemberInfo* ) arg1;
	SFMemberInfo* a2 = ( SFMemberInfo* ) arg2;

	if( g_IsAscend_Sort )
	{
		if( a1->TheMap == a2->TheMap )
		{
			return 0;
		}
		else if( a1->TheMap < a2->TheMap )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( a1->TheMap == a2->TheMap )
		{
			return 0;
		}
		else if( a1->TheMap > a2->TheMap )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

#endif // DECO_RENEWAL_GUILD_MJH

int SortMemberNickNameList( const void *arg1, const void *arg2 )
{
	SFMemberInfo* a1 = ( SFMemberInfo* ) arg1;
	SFMemberInfo* a2 = ( SFMemberInfo* ) arg2;


	int comp_result = 0;

	if( g_IsAscend_Sort )
	{
		comp_result = Rstrcmp( a1->NickName, a2->NickName );

		if( comp_result < 0 )
		{
			return -1;
		}
		else if( comp_result > 0 )
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		comp_result = Rstrcmp( a1->NickName, a2->NickName );

		if( comp_result < 0 )
		{
			return 1;			
		}
		else if( comp_result > 0 )
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

int SortMemberGradeList( const void *arg1, const void *arg2 )
{
	SFMemberInfo* a1 = ( SFMemberInfo* ) arg1;
	SFMemberInfo* a2 = ( SFMemberInfo* ) arg2;

	
	if( g_IsAscend_Sort )
	{
		if( a1->Grade == a2->Grade )
		{
			return 0;
		}
		else if( a1->Grade < a2->Grade )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( a1->Grade == a2->Grade )
		{
			return 0;
		}
		else if( a1->Grade > a2->Grade )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

int SortMemberAccumulationFeeList( const void *arg1, const void *arg2 )
{
	SFMemberInfo* a1 = ( SFMemberInfo* ) arg1;
	SFMemberInfo* a2 = ( SFMemberInfo* ) arg2;

	
	if( g_IsAscend_Sort )
	{
		if( a1->AccumulationFee == a2->AccumulationFee )
		{
			return 0;
		}
		else if( a1->AccumulationFee < a2->AccumulationFee )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( a1->AccumulationFee == a2->AccumulationFee )
		{
			return 0;
		}
		else if( a1->AccumulationFee > a2->AccumulationFee )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	return 0;
}

int SortMemberFameList( const void *arg1, const void *arg2 )
{
	SFMemberInfo* a1 = ( SFMemberInfo* ) arg1;
	SFMemberInfo* a2 = ( SFMemberInfo* ) arg2;


	if( g_IsAscend_Sort )
	{
		if( a1->Fame == a2->Fame )
		{
			return 0;
		}
		else if( a1->Fame < a2->Fame )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if( a1->Fame == a2->Fame )
		{
			return 0;
		}
		else if( a1->Fame > a2->Fame )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	return 0;
}
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================
//========================================================================================================


//-----------------------------------------------------------------------------
void CFrontierMainWnd::Init()
{
	ClearFrontierMainWnd();
	
	RefreshFrontierMyInfo();
//	RefreshFrontierMyFrontier();
//	RefreshFrontierMyFrontierPet();
	
	return;
}

void CFrontierMainWnd::CheckChangeMasterMagorName(char* aOldName, char* aNewName)
{
	if( nRui->GetPcInvenInfo()->FrontierInfo != NULL )
	{
		if( Rstrcmp( RWCHAR(nRui->GetPcInvenInfo()->FrontierInfo->theMasterName), RWCHAR(aOldName) ) == 0 )
		{
			SAFE_STR_CPY( nRui->GetPcInvenInfo()->FrontierInfo->theMasterName, aNewName, RP_MAX_PC_NAME );
			return;
		}
		
		for( int i = 0; i < 3; ++i )
		{
			if( Rstrcmp( RWCHAR(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i]), RWCHAR(aOldName) ) == 0 )
			{
				SAFE_STR_CPY( nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i], aNewName, RP_MAX_PC_NAME );
				return;
			}
		}
	}

}

void CFrontierMainWnd::RefreshFrontierMyInfo()
{
	if( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL )
	{
		m_pNickNameTxt->SetText( _RT("") );			
		m_pGradeTxt->SetText( _RT("") );			
		m_pAccumulationFeeTxt->SetText( _RT("") );	
		
		m_bIsMaster = false;			
		m_bIsMajor = false;	
		m_pSecessionBtn->SetBtnHold( true );
		m_pPaymentBtn->SetBtnHold( true );
#ifdef DECO_RENEWAL_GUILD_MJH
		m_pGuildMenuBtn->SetBtnHold( true );
#endif // DECO_RENEWAL_GUILD_MJH
		

		return;
	}

	if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID == 0 )
		return;
	
	char strTemp[128] = {0,};

	m_pNickNameTxt->SetText( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierNick );			

	GetGradeTypeStr( strTemp, nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade );
	m_pGradeTxt->SetText( strTemp );			

	_ultoR( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierDues , RWCHAR(strTemp), 10 );
	
	m_pAccumulationFeeTxt->SetText( strTemp );			

	m_bIsMaster = false;			
	m_bIsMajor = false;	
	m_pPaymentBtn->SetBtnHold( false );
	m_pSecessionBtn->SetBtnHold( false );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pGuildMenuBtn->SetBtnHold( false );
#endif // DECO_RENEWAL_GUILD_MJH
	
	if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master )
	{
		m_bIsMaster = true;			
	}
	else if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major )
	{
		m_bIsMajor = true;			
	}
	else if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Member )
	{
		m_pPaymentBtn->SetBtnHold( true );
	}	

	if( nRui->GetPcInvenInfo()->FrontierInfo )
	{
		if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
		{
			m_pPaymentBtn->SetBtnHold( true );		
		}
	}
}

void CFrontierMainWnd::RefreshFrontierMyFrontier()
{
	if( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL ||
		nRui->GetPcInvenInfo()->FrontierInfo == NULL )
	{
		m_pFrontierNameTxt->SetText( _RT("") );	
		m_pMasterNameTxt->SetText( _RT("") );
		m_pMajorNamesTxt->SetText( _RT("") );
		m_pMemberNumTxt->SetText( _RT("") );
		m_pSavedEldTxt->SetText( _RT("") );
		m_pFameTxt->SetText( _RT("") );
		m_pColonyTxt->SetText( _RT("") );
#ifdef DECO_RENEWAL_GUILD_MJH
		m_pGuildMaterNameTxt->SetText( _RT("") );
		m_pGuildMemberNumTxt->SetText( _RT("") );
		m_pGuildTotalFameTxt->SetText( _RT("") );
		m_pGuildLevelTxt->SetText( _RT("") );
		m_pGuildHelpTxt->SetText( _RT("") );
		m_pTotalFameTxt->SetText( _RT("")  );
#endif // DECO_RENEWAL_GUILD_MJH
		
		m_pEmblem->SetVisible( false );
		m_pEldShareBtn->SetBtnHold( true );
		m_pViewAllBtn->SetBtnHold( true );


		m_pIncPersonsBtn->SetBtnHold( true ) ;

		
		return;
	}
	
	TCHAR strTemp[128] = {0,};
	TCHAR strEld[128] = {0,};

	if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
	{
		Rsprintf( RWCHAR(strTemp), _RT("(%s) %s"), G_STRING_CHAR( IDS_WND_PRE ), RWCHAR(nRui->GetPcInvenInfo()->FrontierInfo->theFrontierName) );
		m_pFrontierNameTxt->SetText( strTemp );	
	}
	else
	{
		m_pFrontierNameTxt->SetText( nRui->GetPcInvenInfo()->FrontierInfo->theFrontierName );	
	}	
	
	m_pMasterNameTxt->SetText( nRui->GetPcInvenInfo()->FrontierInfo->theMasterName );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pGuildMaterNameTxt->SetText( nRui->GetPcInvenInfo()->FrontierInfo->theMasterName );
	Rsprintf( RWCHAR(strTemp), _RT("%d"), RWCHAR( nRui->GetPcInvenInfo()->FrontierInfo->theTotalFame ));
	m_pTotalFameTxt->SetText( strTemp  );
	
	m_pNoticeInfoTxt->SetText( nRui->GetPcInvenInfo()->FrontierInfo->theNotice );
#endif // DECO_RENEWAL_GUILD_MJH

	Rsprintf( RWCHAR(strTemp), _RT(" 1.%s ## 2.%s ## 3.%s"),
		RWCHAR(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[0]), 
		RWCHAR(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[1]), 
		RWCHAR(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[2]) );
	m_pMajorNamesTxt->SetText( strTemp );

	Rsprintf( RWCHAR(strTemp), _RT("%d / %d"), g_FrontierMemberListWnd.m_FrontierMemberInfoList.size(), m_MaxMemberNum );
	m_pMemberNumTxt->SetText( strTemp );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pGuildMemberNumTxt->SetText( strTemp );
#endif // DECO_RENEWAL_GUILD_MJH
#ifdef UNICODE_ROCK
	_ultow( nRui->GetPcInvenInfo()->FrontierInfo->theDues , RWCHAR(strEld), 10 );
#else
	_ultoa( nRui->GetPcInvenInfo()->FrontierInfo->theDues , strEld, 10 );
#endif	
 
	Rsprintf( RWCHAR(strTemp), _RT("%s %s"), RWCHAR(strEld) , G_STRING_CHAR( IDS_FRONTIERMAIN_ELD ) );	
	m_pSavedEldTxt->SetText( strTemp );
	
	Ritoa( nRui->GetPcInvenInfo()->FrontierInfo->theFame, RWCHAR(strTemp), 10 );
	m_pFameTxt->SetText( strTemp );

#ifdef COLONY_UPDATE
	if(nRui->GetPcInvenInfo()->FrontierInfo->theColony)
	{
		::GetAreaStrName(strTemp, (UINT)(nRui->GetPcInvenInfo()->FrontierInfo->theColony));
		m_pColonyTxt->SetText( strTemp );
	}
#endif

	m_pEmblem->SetVisible( true );	
	m_pViewAllBtn->SetBtnHold( false );

	if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master )
	{
		m_pEldShareBtn->SetBtnHold( false );
		m_pIncPersonsBtn->SetBtnHold( false );


	}
	else
	{
		m_pEldShareBtn->SetBtnHold( true );
		m_pIncPersonsBtn->SetBtnHold( true );
	}	

	if( nRui->GetPcInvenInfo()->FrontierInfo )
	{
		if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
		{
			m_pEldShareBtn->SetBtnHold( true );
			m_pIncPersonsBtn->SetBtnHold( true );
		}
	}
}

BOOL CFrontierMainWnd::IsFullFrontierMember()
{
	return g_FrontierMemberListWnd.m_FrontierMemberInfoList.size() == m_MaxMemberNum ? TRUE : FALSE;
}

void CFrontierMainWnd::RefreshFrontierMyFrontierPet()
{
	
	m_pStatusTxt->SetTextColor( TEXT_COLOR_VALUE1 );	
	
	if( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL ||
		nRui->GetPcInvenInfo()->FrontierPetInfo == NULL )
	{
		m_pStatusTxt->SetText( _RT("") );
		m_pNameTxt->SetText( _RT("") );
		m_pTypeTxt->SetText( _RT("") );
		m_pLevelTxt->SetText( _RT("") );
		m_pExpTxt->SetText( _RT("") );
		m_pEnergyTxt->SetText( _RT("") );
		
		m_pSummonsBtn->SetBtnHold( true );
		m_pCallOffBtn->SetBtnHold( true );
		m_pEnergyUpBtn->SetBtnHold( true );
		
		return;
	}

	char strTemp[128] = {0,};

	if( nRui->GetPcInvenInfo()->FrontierCaller[0] == '\0' )
	{
		Rsprintf( RWCHAR(strTemp), _RT("%s"), G_STRING_CHAR( IDS_FRONTIERMAIN_CALLOFF ) );			
	}
	else
	{
		Rsprintf( RWCHAR(strTemp), _RT("%s <%s>") , G_STRING_CHAR( IDS_FRONTIERMAIN_SUMMONS ), RWCHAR(nRui->GetPcInvenInfo()->FrontierCaller) );			
	}

	if( nRui->GetPcInvenInfo()->FrontierPetInfo->thePetHP == 0 )
	{
		m_pStatusTxt->SetTextColor( 255, 0, 0 );
		Rsprintf( RWCHAR(strTemp), _RT("%s"), G_STRING_CHAR( IDS_PET_DIE_WND ) );		
	}

	m_pStatusTxt->SetText( strTemp );

	m_pNameTxt->SetText( nRui->GetPcInvenInfo()->FrontierPetInfo->thePetName );

	SNPCBaseInfo*	npc_Info = NULL;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( nRui->GetPcInvenInfo()->FrontierPetInfo->thePetCode ); 

	if( npc_Info )
	{
		SAFE_STR_CPY( strTemp, npc_Info->theName, 128 );	
	}
	else
	{
		SAFE_STR_CPY( strTemp, _RT("FrontierPet"), 128 );	
	}
	m_pTypeTxt->SetText( strTemp );

	Ritoa( nRui->GetPcInvenInfo()->FrontierPetInfo->thePetLevel, RWCHAR(strTemp), 10 );
	m_pLevelTxt->SetText( strTemp );

	if( nRui->GetPcInvenInfo()->FrontierPetMaxExp == 0 )
	{
		int level = nRui->GetPcInvenInfo()->FrontierPetInfo->thePetLevel;
		DWORD PetMaxExp = ((level+1)*(level+1))*75;
		nRui->GetPcInvenInfo()->FrontierPetMaxExp =  PetMaxExp;
	}

	Rsprintf(RWCHAR(strTemp) ,_RT("%d / %d") , nRui->GetPcInvenInfo()->FrontierPetInfo->thePetExp , 
								nRui->GetPcInvenInfo()->FrontierPetMaxExp);

	m_pExpTxt->SetText( strTemp );

//	itoa( nRui->GetPcInvenInfo()->FrontierPetInfo->thePetHP, strTemp, 10 );
	Rsprintf( RWCHAR(strTemp), _RT("%d / 4320"), RWCHAR(nRui->GetPcInvenInfo()->FrontierPetInfo->thePetHP) );
	m_pEnergyTxt->SetText( strTemp );

	if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master ||
#ifdef DECO_RENEWAL_GUILD_MJH
		nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major  ||
		nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Assist ||
		nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Senior )
#else
		nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major )
#endif // DECO_RENEWAL_GUILD_MJH

	{
		if( nRui->GetPcInvenInfo()->FrontierCaller[0] == '\0' )
		{
			m_pSummonsBtn->SetBtnHold( false );
		}
		else
		{
			// 마스터는 강제 소환 기능 추가 
			if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master )
			{
				m_pSummonsBtn->SetBtnHold( false );
			}
			else
			{
				m_pSummonsBtn->SetBtnHold( true );
			}
		}		

		if( Rstrcmp( RWCHAR(nRui->GetPcInvenInfo()->FrontierCaller), RWCHAR(nRui->GetPcParamInfo()->PCName) ) == 0 )
		{
			m_pSummonsBtn->SetBtnHold( true );
			m_pCallOffBtn->SetBtnHold( false );			
		}
		else
		{
			m_pCallOffBtn->SetBtnHold( true );
		}

		m_pEnergyUpBtn->SetBtnHold( false );
	}
	else
	{
		m_pSummonsBtn->SetBtnHold( true );
		m_pCallOffBtn->SetBtnHold( true );
		m_pEnergyUpBtn->SetBtnHold( true );
	}
	
	if( nRui->GetPcInvenInfo()->FrontierInfo )
	{
		if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
		{
			m_pSummonsBtn->SetBtnHold( true );
			m_pCallOffBtn->SetBtnHold( true );
			m_pEnergyUpBtn->SetBtnHold( true );
		}
	}

	if( g_Pc.GetPlayer()->theMapType == n_FightZone )
	{
		m_pSummonsBtn->SetBtnHold( true );
		m_pCallOffBtn->SetBtnHold( true );
		m_pEnergyUpBtn->SetBtnHold( true );
	}

	///-- 신수의 체력이 0일경우 - 사망한 상태 - 소환불가
	if( nRui->GetPcInvenInfo()->FrontierPetInfo->thePetHP == 0 )
	{
		m_pSummonsBtn->SetBtnHold( true );
		m_pCallOffBtn->SetBtnHold( true );
		m_pEnergyUpBtn->SetBtnHold( true );		
	}
}

void CFrontierMainWnd::RefreshFrontierCombineFrontier()
{
#ifdef COLONY_UPDATE
	if( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL ||
		nRui->GetPcInvenInfo()->pComBineFrontierInfo == NULL )
	{
		m_pCoFronName->SetText( _RT("") );
		m_pCoFMasterName->SetText( _RT("") );
		m_pCoFMemNum->SetText( _RT("") );
		m_pCoFFame->SetText( _RT("") );
		
		m_pCoFBreakBtn->SetBtnHold( true );		
		return;
	}

	char strTemp[128] = {0,};
	
	m_pCoFronName->SetText( nRui->GetPcInvenInfo()->pComBineFrontierInfo->strCombinedFName );
	m_pCoFMasterName->SetText( nRui->GetPcInvenInfo()->pComBineFrontierInfo->strCombinedFMasterName );
	
	Rsprintf( RWCHAR(strTemp), _RT("%d / %d"), nRui->GetPcInvenInfo()->pComBineFrontierInfo->aCombinedFMemberNum , m_MaxMemberNum );
	
	m_pCoFMemNum->SetText(strTemp);
	
	Ritoa( nRui->GetPcInvenInfo()->pComBineFrontierInfo->aCombinedFFame , RWCHAR(strTemp), 10 );
	m_pCoFFame->SetText( strTemp );

	// 마스터만이 프론티어 해제가능하게 함
	if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master )
	{
		m_pCoFBreakBtn->SetBtnHold( false );
	}
	else
	{
		m_pCoFBreakBtn->SetBtnHold( true );
	}

#endif
}

void CFrontierMainWnd::RefreshMyFrontierGrade( int aGrade, char* aPCName )
{
	if( nRui->GetPcInvenInfo()->FrontierInfo == NULL )
		return;
	
	int i = 0;

	if( aGrade == n_FT_Master )
	{
		SAFE_STR_CPY( nRui->GetPcInvenInfo()->FrontierInfo->theMasterName, aPCName, RP_MAX_PC_NAME );
		//마스터 이름이 매이져이 있는지 검사 한다 
		for( i = 0; i < 3; ++i )
		{
			//있으면 
			if( Rstrcmp( RWCHAR(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i]), RWCHAR(aPCName )) == 0 )
			{
				SAFE_STR_CPY(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i] , _RT(""),RP_MAX_PC_NAME);
			}
		}


	}
	// 세명중에 자신이 있는지 확인후 없으면 빈칸에 추가 
	else if( aGrade == n_FT_Major )			
	{
		for( i = 0; i < 3; ++i )
		{
			if( Rstrcmp( RWCHAR(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i]), RWCHAR(aPCName) ) == 0 )
			{
				return;
			}
		}
		
		for( i = 0; i < 3; ++i )
		{
			if( Rstrcmp(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i], _RT("") ) == 0 )
			{
				SAFE_STR_CPY( nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i], aPCName, RP_MAX_PC_NAME );
				break;
			}
		}
	}
	// 등급이 낮아졌을때 Master Or Major 에 자신의 이름이 있다면 삭제 
	else				
	{
		if( Rstrcmp( RWCHAR(nRui->GetPcInvenInfo()->FrontierInfo->theMasterName), RWCHAR(aPCName)  ) == 0 )
		{
			SAFE_STR_CPY(nRui->GetPcInvenInfo()->FrontierInfo->theMasterName , _RT("") , RP_MAX_PC_NAME);
			
		}
		else
		{
			for( i = 0; i < 3; ++i )
			{
				if( Rstrcmp( RWCHAR(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i]), RWCHAR(aPCName) ) == 0 )
				{
					SAFE_STR_CPY(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i], _RT("") , RP_MAX_PC_NAME );
					break;
				}
			}
		}
	}

	SortMyFrontierGrade();
}

void CFrontierMainWnd::SortMyFrontierGrade()
{	
	if( nRui->GetPcInvenInfo()->FrontierInfo == NULL )
		return;

	for( int i = 0; i < 3; ++i )
	{
		if( Rstrcmp ( nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i] , _RT("")) == 0  )
		{
			int next = i + 1;
			
			for( int j = next; j < 3; ++j )
			{
				if( Rstrcmp(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[next], _RT(""))!= 0 )
				{
					SAFE_STR_CPY( nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[i], 
						nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[next], RP_MAX_PC_NAME );
					
					SAFE_STR_CPY(nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[next] , _RT("") , RP_MAX_PC_NAME);
					break;
				}
			}						
		}
	}	
}
#ifdef DECO_RENEWAL_GUILD_MJH

void CFrontierMainWnd::SetInfoState( N_FRONTIER_INFO_STATE state )
{
	int i = 0;
	InfoState = state;

	CloseControlType();

	if( InfoState == n_FRONTIER_CREATURE_INFO )
	{
		for( i = 0; i < 6; ++i )
		{
			Creaturetext[i]->SetVisible( TRUE );
		}
		m_pStatusTxt->SetVisible( TRUE ); 
		m_pNameTxt->SetVisible( TRUE ); 
		m_pTypeTxt->SetVisible( TRUE ); 
		m_pLevelTxt->SetVisible( TRUE ); 
		m_pExpTxt->SetVisible( TRUE ); 
		m_pEnergyTxt->SetVisible( TRUE ); 
		m_pSummonsBtn->SetVisible( TRUE ); 
		m_pCallOffBtn->SetVisible( TRUE ); 
		m_pEnergyUpBtn->SetVisible( TRUE );
	}
	else if( InfoState == n_FRONTIER_MEMBER_INFO )
	{
		Membertext[0]->SetVisible( TRUE ); 
		Membertext[1]->SetVisible( TRUE ); 
		Membertext[2]->SetVisible( TRUE );

		m_pGuildMaterNameTxt->SetVisible( TRUE );
		m_pGuildMemberNumTxt->SetVisible( TRUE );
		m_pMajorNamesTxt->SetVisible( TRUE );
		m_pViewAllBtn->SetVisible( TRUE );
		m_pEldShareBtn->SetVisible( TRUE );
	}
	else if ( InfoState == n_FRONTIER_NOTICE_INFO)
	{
		m_pNoticeInfoTxt->SetVisible( TRUE );
	}

}
void CFrontierMainWnd::CloseControlType()
{
	int i = 0;
	for( i = 0; i < 6; ++i )
	{
		Creaturetext[i]->SetVisible( FALSE );
	}

	m_pStatusTxt->SetVisible( FALSE ); 
	m_pNameTxt->SetVisible( FALSE ); 
	m_pTypeTxt->SetVisible( FALSE ); 
	m_pLevelTxt->SetVisible( FALSE ); 
	m_pExpTxt->SetVisible( FALSE ); 
	m_pEnergyTxt->SetVisible( FALSE ); 
	m_pSummonsBtn->SetVisible( FALSE ); 
	m_pCallOffBtn->SetVisible( FALSE ); 
	m_pEnergyUpBtn->SetVisible( FALSE );

	Membertext[0]->SetVisible( FALSE ); 
	Membertext[1]->SetVisible( FALSE ); 
	Membertext[2]->SetVisible( FALSE ); 

	m_pGuildMaterNameTxt->SetVisible( FALSE );
	m_pGuildMemberNumTxt->SetVisible( FALSE );
	m_pMajorNamesTxt->SetVisible( FALSE );

	m_pViewAllBtn->SetVisible( FALSE );
	m_pEldShareBtn->SetVisible( FALSE );

	m_pNoticeInfoTxt->SetVisible( FALSE );

}
#endif // DECO_RENEWAL_GUILD_MJH
//-----------------------------------------------------------------------------
void CFrontierMainWnd::ClearFrontierMainWnd()
{
	m_pNickNameTxt->SetText( _RT("") );			
	m_pGradeTxt->SetText( _RT("") );			
	m_pAccumulationFeeTxt->SetText( _RT("") );			

	m_bIsMaster = false;			
	m_bIsMajor = false;	
	m_pSecessionBtn->SetBtnHold( true );
	m_pPaymentBtn->SetBtnHold( true );
	
	m_pFrontierNameTxt->SetText( _RT("") );	
	m_pMasterNameTxt->SetText( _RT("") );
	m_pMajorNamesTxt->SetText( _RT("") );
	m_pMemberNumTxt->SetText( _RT("") );
	m_pSavedEldTxt->SetText( _RT("") );
	m_pFameTxt->SetText( _RT("") );
#ifdef DECO_RENEWAL_GUILD_MJH

	m_pGuildMenuBtn->SetBtnHold( true );
	m_pGuildMaterNameTxt->SetText( _RT("") );
	m_pGuildMemberNumTxt->SetText( _RT("") );
	m_pGuildTotalFameTxt->SetText( _RT("") );
	m_pGuildLevelTxt->SetText( _RT("") );
	m_pGuildHelpTxt->SetText( _RT("") );
	m_pTotalFameTxt->SetText( _RT("")  );
#endif // DECO_RENEWAL_GUILD_MJH

	m_pEmblem->SetVisible( false );
	m_pEldShareBtn->SetBtnHold( true );
	m_pViewAllBtn->SetBtnHold( true );

	m_pIncPersonsBtn->SetBtnHold( true );


	m_pStatusTxt->SetText( _RT("") );
	m_pNameTxt->SetText( _RT("") );
	m_pTypeTxt->SetText( _RT("") );
	m_pLevelTxt->SetText( _RT("") );
	m_pExpTxt->SetText( _RT("") );
	m_pEnergyTxt->SetText( _RT("") );

	m_pSummonsBtn->SetBtnHold( true );
	m_pCallOffBtn->SetBtnHold( true );
	m_pEnergyUpBtn->SetBtnHold( true );

	return;
}


void CFrontierMainWnd::LogOff()
{
	g_FrontierMainWnd.Close();
	g_FrontierMemberListWnd.Close();
	g_FrontierMemberEditWnd.Close();	

	ClearFrontierMainWnd();
	g_FrontierMemberListWnd.ClearFrontierMemberListWnd();	
}

void CFrontierMainWnd::CompositionNew()
{
	
	int center_x = nRui->Desktop.Width / 2;
	int center_y = nRui->Desktop.Height / 2;
	
	char temp[100] = {0,};

	CImageBox* img = NULL;	
	CImageBox9* img9 = NULL;
	CButton* btn = NULL;
	CTextBox* text = NULL;
#ifdef DECO_RENEWAL_GUILD_MJH	
	CStateButton* sbtn = NULL;
#endif // DECO_RENEWAL_GUILD_MJH	
	SRect mainwnd;
	mainwnd.w	= 296;

	int nLineWidth = 294;
	int nCloseBtnXPos = 277;

	int nXPosition = 75;
	int nXButtonPos = 223;
	int nButtonW = 58;

	int nXEmbPos = 220;	

#ifdef APPLY_ENGLISH_WINDOW
	
	mainwnd.w	= 366;
	nLineWidth = 364;
	nCloseBtnXPos = 347;
	nXPosition = 145;
	nXButtonPos = 293;
	nXEmbPos = 293;	

#endif

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	mainwnd.w	= 476;
	nLineWidth = 474;
	nCloseBtnXPos = 457;

	nXPosition = 205;
	nXButtonPos = 373;
	nXEmbPos = 380;	

	nButtonW = 88;

#endif

#ifdef COLONY_UPDATE
	mainwnd.h	= 498;
#else
	mainwnd.h	= 400;
#endif	
	
	mainwnd.x	= center_x - ( mainwnd.w / 2 );
	mainwnd.y	= center_y - ( mainwnd.h / 2 );


	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_FRONTIERMAIN_WND, WID_None, mainwnd.x, mainwnd.y, mainwnd.w, mainwnd.h, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );		
//	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_STATUS );								// 프론티어창 표시 아이콘 
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 ); 	
	m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_FRONTIERMAIN ) );				// 프론티어 정보창 
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nCloseBtnXPos, 6, 13, 13, false );  	
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	
	///---------------------------------------------------------------------------	
	///-- Static Img
	///---------------------------------------------------------------------------	
	///-- Line
#ifdef DECO_RENEWAL_GUILD_MJH
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMAIN_WND, 1, 185, nLineWidth, 1, false );   
#else 
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMAIN_WND, 1, 105, nLineWidth, 1, false );   
#endif // DECO_RENEWAL_GUILD_MJH
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMAIN_WND, 1, 272, nLineWidth, 1, false );   
	img ->SetClientImage( TID_CTRL_LINEDOT );

#ifdef COLONY_UPDATE	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMAIN_WND, 1, 392, nLineWidth, 1, false );   
	img ->SetClientImage( TID_CTRL_LINEDOT );
#endif

	// 이미지 범용 사용 및 리소스 사이즈 절감 효과 (리스트 박스, 뒷배경으로 깔리는 여러 이미지들 )
	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_FRONTIERMAIN_WND, 1, 48, nLineWidth, 55, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 사용시 필수 호출
#ifdef DECO_RENEWAL_GUILD_MJH
	img9->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH
	

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_FRONTIERMAIN_WND, 1, 125, nLineWidth, 145, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 사용시 필수 호출  
#ifdef DECO_RENEWAL_GUILD_MJH
	img9->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_FRONTIERMAIN_WND, 1, 290, nLineWidth, 100, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 사용시 필수 호출  

#ifdef COLONY_UPDATE	
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_FRONTIERMAIN_WND, 1, 411, nLineWidth, 70, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 사용시 필수 호출
#endif	
	
	///-----------------------------------------------------------------------
	///-- Static Text
	//========================================================================================================
	//========================================================================================================
	// MY INFO 
#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 120, 198, 80, 15, false );
#else 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 33, 80, 15, false );
#endif // DECO_RENEWAL_GUILD_MJH
//	text->SetFontR( n_fRsvFontTitle );
	text->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	text->SetTextColor( 70, 70, 100 );	
	text->SetClientImage( TID_None );	
	text->SetAlignText( n_atLeft, n_atCenter );		
	text->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_MYINFO ) );							// MY INFO 
	text->SetFocusCheck( false );

	///-- 호칭 
#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 223, 80, 13, false );
#else 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 53, 80, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_NICKNAME ) );				// 호칭  
	text->SetText( temp );

	///-- 등급 
#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 145, 223, 80, 13, false );
#else 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 68, 80, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_GRADE ) );					// 등급  
	text->SetText( temp );

#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 243, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), _RT( "기여도") );		// 기여도 
	text->SetText( temp );
#endif // DECO_RENEWAL_GUILD_MJH

	///-- 누적회비 
#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 145, 243, 80, 13, false );
#else 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 83, 80, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_ACCUMULATIONFEE ) );		// 누적회비   
	text->SetText( temp );

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pNickNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 75, 223, 146, 13, false );
#else 
	m_pNickNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 53, 146, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH	
	m_pNickNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNickNameTxt->SetClientImage( TID_None );
//	m_pNickNameTxt->SetClientColor( 180, 175, 160 );
	m_pNickNameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pNickNameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pNickNameTxt->SetText( _RT("카리스마단장") );
	m_pNickNameTxt->SetHID( HID_None );

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pGradeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 205, 223, 146, 13, false );
#else 
	m_pGradeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 68, 146, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	m_pGradeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pGradeTxt->SetClientImage( TID_None );
//	m_pGradeTxt->SetClientColor( 180, 175, 160 );
	m_pGradeTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pGradeTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pGradeTxt->SetText( _RT("Master") );
	m_pGradeTxt->SetHID( HID_None );

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pGuildHelpTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 75, 243, 146, 13, false );
	m_pGuildHelpTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pGuildHelpTxt->SetClientImage( TID_None );
//	m_pGuildHelpTxt->SetClientColor( 180, 175, 160 );
	m_pGuildHelpTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pGuildHelpTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pGuildHelpTxt->SetText( _RT("10 point") );
	m_pGuildHelpTxt->SetHID( HID_None );
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pAccumulationFeeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 205, 243, 146, 13, false );
#else 
	m_pAccumulationFeeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 83, 146, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	m_pAccumulationFeeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pAccumulationFeeTxt->SetClientImage( TID_None );
//	m_pAccumulationFeeTxt->SetClientColor( 180, 175, 160 );
	m_pAccumulationFeeTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pAccumulationFeeTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pAccumulationFeeTxt->SetText( _RT("1234567 엘드") );
	m_pAccumulationFeeTxt->SetHID( HID_None );


	m_pSecessionBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 53, nButtonW, 20, false ); 
	m_pSecessionBtn->SetClientImage( TID_None );
	m_pSecessionBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pSecessionBtn->SetFontR( n_fRsvFontWndButton );	
	m_pSecessionBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_SECESSION ) );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pSecessionBtn->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

	m_pPaymentBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 78, nButtonW, 20, false ); 
	m_pPaymentBtn->SetClientImage( TID_None );
	m_pPaymentBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pPaymentBtn->SetFontR( n_fRsvFontWndButton );	
	m_pPaymentBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_PAYMENT ) );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pPaymentBtn->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH


	//========================================================================================================
	//========================================================================================================
	// MY FRONTIER 
#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 100, 38, 80, 13, false );
#else 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 110, 80, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
//	text->SetFontR( n_fRsvFontTitle );
	text->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	text->SetTextColor( 70, 70, 100 );	
	text->SetClientImage( TID_None );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	text->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_MYFRONTIER ) );		// MY FRONTIER 

	///-- Name
#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 90, 60, 80, 13, false );
#else 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 130, 80, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_FRONTIERNAME ) );		// Name    
	text->SetText( temp );

	///-- Master
#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 90, 160, 80, 13, false );
#else 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 145, 80, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_MASTER ) );		// Master
	text->SetText( temp );
//=================================================================================================================
// 프론티어 탭창 
//=================================================================================================================
#ifdef DECO_RENEWAL_GUILD_MJH
	Membertext[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 297, 80, 13, false );
	Membertext[0]->SetFontR( n_fRsvFontWndStatic );
	Membertext[0]->SetTextColor( TEXT_COLOR_STATIC );	
	Membertext[0]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_MASTER ) );		// Master
	Membertext[0]->SetText( temp );
	Membertext[0]->SetVisible( false );


	m_pGuildMaterNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 70, 297, 146, 13, false );
	m_pGuildMaterNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pGuildMaterNameTxt->SetClientImage( TID_None );
	m_pGuildMaterNameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pGuildMaterNameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pGuildMaterNameTxt->SetText( _RT("무적길마") );
	m_pGuildMaterNameTxt->SetHID( HID_None );
	m_pGuildMaterNameTxt->SetVisible( false );

	m_pGuildMemberNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 70, 377, 146, 13, false );
	m_pGuildMemberNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pGuildMemberNumTxt->SetClientImage( TID_None );
	m_pGuildMemberNumTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pGuildMemberNumTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pGuildMemberNumTxt->SetText( _RT("100만명") );
	m_pGuildMemberNumTxt->SetHID( HID_None );
	m_pGuildMemberNumTxt->SetVisible( false );

#endif // DECO_RENEWAL_GUILD_MJH
//=================================================================================================================
	///-- Major
#ifdef DECO_RENEWAL_GUILD_MJH
	Membertext[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 317, 80, 13, false );
#else 
	Membertext[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 165, 80, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	Membertext[1]->SetFontR( n_fRsvFontWndStatic );
	Membertext[1]->SetTextColor( TEXT_COLOR_STATIC );	
	Membertext[1]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_MAJOR ) );		// Major
	Membertext[1]->SetText( temp );
#ifdef DECO_RENEWAL_GUILD_MJH
	Membertext[1]->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH


	///-- Member
#ifdef DECO_RENEWAL_GUILD_MJH
	Membertext[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 377, 80, 13, false );
#else 
	Membertext[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 205, 80, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	Membertext[2]->SetFontR( n_fRsvFontWndStatic );
	Membertext[2]->SetTextColor( TEXT_COLOR_STATIC );	
	Membertext[2]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_MEMBER ) );		// Member
	Membertext[2]->SetText( temp );
#ifdef DECO_RENEWAL_GUILD_MJH
	Membertext[2]->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH


	///-- 보유엘드 
#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 90, 120, 80, 13, false );
#else 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 220, 80, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_SAVEDELD ) );		// 보유엘드
	text->SetText( temp );
	
	///-- 명성치
#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 90, 100, 80, 13, false );
#else 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 235, 80, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_FAME ) );		// 명성치 
	text->SetText( temp );

	///-- 콜로니 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 250, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_COLONY ) );		// 콜로니 
	text->SetText( temp );
#ifdef DECO_RENEWAL_GUILD_MJH
	text->SetVisible( false );                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef DECO_RENEWAL_GUILD_MJH //길드 누적 명성
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 90, 80, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), _RT("누적명성") );		 
	text->SetText( temp );

	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 90, 140, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), _RT("길드원") );		 
	text->SetText( temp );
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pFrontierNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 200 , 60, 146, 13, false );
#else 
	m_pFrontierNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition , 130, 146, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	m_pFrontierNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pFrontierNameTxt->SetClientImage( TID_None );
	m_pFrontierNameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pFrontierNameTxt->SetTextColor( TEXT_COLOR_VALUE2 );
	m_pFrontierNameTxt->SetText( _RT("락소프트최고길드짱짱짱!!") );
	m_pFrontierNameTxt->SetHID( HID_None );

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pGuildTotalFameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 200, 80, 40, 13, false );
	m_pGuildTotalFameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pGuildTotalFameTxt->SetClientImage( TID_None );
	m_pGuildTotalFameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pGuildTotalFameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pGuildTotalFameTxt->SetText( _RT("길드누적명성") );
	m_pGuildTotalFameTxt->SetHID( HID_None );

	m_pTotalFameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 250, 80, 40, 13, false );
	m_pTotalFameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pTotalFameTxt->SetClientImage( TID_None );
	m_pTotalFameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pTotalFameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pTotalFameTxt->SetText( _RT("누적명성") );
	m_pTotalFameTxt->SetHID( HID_None );

	m_pGuildLevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 140, 40, 13, false );
	m_pGuildLevelTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pGuildLevelTxt->SetClientImage( TID_None );
	m_pGuildLevelTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pGuildLevelTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pGuildLevelTxt->SetText( _RT("Level 1") );
	m_pGuildLevelTxt->SetHID( HID_None );
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pMasterNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 200, 160, 146, 13, false );
#else 
	m_pMasterNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 145, 146, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	m_pMasterNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMasterNameTxt->SetClientImage( TID_None );
	m_pMasterNameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pMasterNameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pMasterNameTxt->SetText( _RT("나이스가이짱") );
	m_pMasterNameTxt->SetHID( HID_None );

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pMajorNamesTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 60, 317, 146, 40, false );
#else 
	m_pMajorNamesTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 160, 146, 40, false );
#endif // DECO_RENEWAL_GUILD_MJH
	m_pMajorNamesTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMajorNamesTxt->SetClientImage( TID_None );
	m_pMajorNamesTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pMajorNamesTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pMajorNamesTxt->SetMultiLine( true );
	m_pMajorNamesTxt->SetLineInterval( 2 );
	m_pMajorNamesTxt->SetText( _RT(" 1.나이스가이짱 ## 2.고장수리짱짱 ## 3.우희짱짱짱짱") );
	m_pMajorNamesTxt->SetHID( HID_None );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pMajorNamesTxt->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

 
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pMemberNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 200, 140, 146, 13, false );
#else 
	m_pMemberNumTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 205, 146, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	m_pMemberNumTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMemberNumTxt->SetClientImage( TID_None );
	m_pMemberNumTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pMemberNumTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pMemberNumTxt->SetText( _RT("28 / 30") );
	m_pMemberNumTxt->SetHID( HID_None );


#ifdef DECO_RENEWAL_GUILD_MJH
	m_pSavedEldTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 160, 120, 146, 13, false );
#else 
	m_pSavedEldTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 220, 146, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	m_pSavedEldTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pSavedEldTxt->SetClientImage( TID_None );
	m_pSavedEldTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pSavedEldTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pSavedEldTxt->SetText( _RT("1234567890 엘드") );
	m_pSavedEldTxt->SetHID( HID_None );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pFameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 200, 100, 146, 13, false );
#else 
	m_pFameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 235, 146, 13, false );
#endif // DECO_RENEWAL_GUILD_MJH
	m_pFameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pFameTxt->SetClientImage( TID_None );
	m_pFameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pFameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pFameTxt->SetText( _RT("1234567890") );
	m_pFameTxt->SetHID( HID_None );

	m_pColonyTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 250, 146, 13, false );
	m_pColonyTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pColonyTxt->SetClientImage( TID_None );
	m_pColonyTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pColonyTxt->SetTextColor( 200, 180, 180 );
	m_pColonyTxt->SetText( G_STRING_CHAR( IDS_WND_NOTYET_SERVICE ) );
	m_pColonyTxt->SetHID( HID_None );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pColonyTxt->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pEmblem = (CEmblemImageBox*)nRui->CreateWnd( n_wtEblemImageBox, WID_None, WID_FRONTIERMAIN_WND, 
												   nXEmbPos , 130 , 64, 64, false );   
#else 
	m_pEmblem = (CEmblemImageBox*)nRui->CreateWnd( n_wtEblemImageBox, WID_None, WID_FRONTIERMAIN_WND, 
												   nXEmbPos , 130 , 64, 64, false ); 
#endif // DECO_RENEWAL_GUILD_MJH
	m_pEmblem->SetClientImage( TID_None );
	m_pEmblem->SetScaleTrans( true );


	m_pIncPersonsBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 190 + 5 , nButtonW, 20, false ); 
	m_pIncPersonsBtn->SetClientImage( TID_None );
	m_pIncPersonsBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pIncPersonsBtn->SetFontR( n_fRsvFontWndButton );	
	m_pIncPersonsBtn->SetText( G_STRING_CHAR(IDS_WND_PERSON_ADD));	
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pIncPersonsBtn->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pViewAllBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 320 , nButtonW, 20, false ); 
#else 
	m_pViewAllBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 215 +5 , nButtonW, 20, false ); 
#endif // DECO_RENEWAL_GUILD_MJH
	m_pViewAllBtn->SetClientImage( TID_None );
	m_pViewAllBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pViewAllBtn->SetFontR( n_fRsvFontWndButton );	
	m_pViewAllBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_VIEWALL ) );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pViewAllBtn->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pEldShareBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 345 , nButtonW, 20, false ); 
#else 
	m_pEldShareBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 240 +5 , nButtonW, 20, false ); 
#endif // DECO_RENEWAL_GUILD_MJH
	m_pEldShareBtn->SetClientImage( TID_None );
	m_pEldShareBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pEldShareBtn->SetFontR( n_fRsvFontWndButton );	
	m_pEldShareBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_ELDSHARE ) );	
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pEldShareBtn->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pGuildMenuBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, 15, 160 , nButtonW, 20, false ); 
	m_pGuildMenuBtn->SetClientImage( TID_None );
	m_pGuildMenuBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pGuildMenuBtn->SetFontR( n_fRsvFontWndButton );	
	m_pGuildMenuBtn->SetText( _RT("길드메뉴") );
#endif // DECO_RENEWAL_GUILD_MJH


#ifdef DECO_RENEWAL_GUILD_MJH

//========================================================================================================
// 길드정보 탭창 추가
//========================================================================================================
	///-- 신수정보
	sbtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_CREATURE_INFO_WND, WID_FRONTIERMAIN_WND, 15, 276, 62, 16, false );
	sbtn->SetBtnImage( TID_CTRL_BTN62, TID_CTRL_BTN62, TID_CTRL_BTN62_OVR, TID_None );
	sbtn->SetFontR( n_fRsvFontWndButton ); 
	sbtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_CREATURE ) ); 
	

	///-- 회원 정보
	sbtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_MEMBER_INFO_WND, WID_FRONTIERMAIN_WND, 77, 276, 62, 16, false );	
	sbtn->SetBtnImage( TID_CTRL_BTN62, TID_CTRL_BTN62, TID_CTRL_BTN62_OVR, TID_None );
	sbtn->SetFontR( n_fRsvFontWndButton ); 
	sbtn->SetText( "회원정보" );
		
	///-- 공지 사항
	sbtn = (CStateButton*)nRui->CreateWnd( n_wtStateButton, WID_NOTICE_INFO_WND, WID_FRONTIERMAIN_WND, 139, 276, 62, 16, false );	
	sbtn->SetBtnImage( TID_CTRL_BTN62, TID_CTRL_BTN62, TID_CTRL_BTN62_OVR, TID_None );
	sbtn->SetFontR( n_fRsvFontWndButton ); 
	sbtn->SetText( G_STRING_CHAR( IDS_NOTICE_T2 ) );

	nRui->SetGroupWnd( 3, WID_CREATURE_INFO_WND,WID_CREATURE_INFO_WND, WID_MEMBER_INFO_WND,WID_NOTICE_INFO_WND  );

#endif // DECO_RENEWAL_GUILD_MJH	
//========================================================================================================
//========================================================================================================
	// 신수  God's Creature ( 신의 창조물 )
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 277, 80, 13, false );
//	text->SetFontR( n_fRsvFontTitle );
	text->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	text->SetTextColor( 70, 70, 100 );	
	text->SetClientImage( TID_None );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	text->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_CREATURE ) );		// 신수  
//	text->SetFocusCheck( false );
#ifdef DECO_RENEWAL_GUILD_MJH
	text->SetVisible( false );
#endif // DECO_RENEWAL_GUILD_MJH
	///-- 상태 
	Creaturetext[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 297, 80, 13, false );
	Creaturetext[0]->SetFontR( n_fRsvFontWndStatic );
	Creaturetext[0]->SetTextColor( TEXT_COLOR_STATIC );	
	Creaturetext[0]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_CREATURESTATUS ) );		// 상태  
	Creaturetext[0]->SetText( temp );

	///-- 이름 
	Creaturetext[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 312, 80, 13, false );
	Creaturetext[1]->SetFontR( n_fRsvFontWndStatic );
	Creaturetext[1]->SetTextColor( TEXT_COLOR_STATIC );	
	Creaturetext[1]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_CHARSTATE_NAME2 ) );					// 이름   
	Creaturetext[1]->SetText( temp );

	///-- 종류 
	Creaturetext[2] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 327, 80, 13, false );
	Creaturetext[2]->SetFontR( n_fRsvFontWndStatic );
	Creaturetext[2]->SetTextColor( TEXT_COLOR_STATIC );	
	Creaturetext[2]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_CREATURETYPE ) );		// 종류   
	Creaturetext[2]->SetText( temp );

	///-- 레벨 
	Creaturetext[3] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 342, 80, 13, false );
	Creaturetext[3]->SetFontR( n_fRsvFontWndStatic );
	Creaturetext[3]->SetTextColor( TEXT_COLOR_STATIC );	
	Creaturetext[3]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_CREATURELEVEL ) );		// 레벨    
	Creaturetext[3]->SetText( temp );

	///-- 경험치 
	Creaturetext[4] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 357, 80, 13, false );
	Creaturetext[4]->SetFontR( n_fRsvFontWndStatic );
	Creaturetext[4]->SetTextColor( TEXT_COLOR_STATIC );	
	Creaturetext[4]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_CREATUREEXP ) );		// 경험치 
	Creaturetext[4]->SetText( temp );

	///-- 활력 
	Creaturetext[5] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 372, 80, 13, false );
	Creaturetext[5]->SetFontR( n_fRsvFontWndStatic );
	Creaturetext[5]->SetTextColor( TEXT_COLOR_STATIC );	
	Creaturetext[5]->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_CREATUREENERGY ) );		// 활력  
	Creaturetext[5]->SetText( temp );
		
	m_pStatusTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 297, 146, 13, false );
	m_pStatusTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pStatusTxt->SetClientImage( TID_None );
	m_pStatusTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pStatusTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pStatusTxt->SetText( _RT("소환중 < 나이스가이짱 >") );
	m_pStatusTxt->SetHID( HID_None );

	m_pNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 312, 146, 13, false );
	m_pNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNameTxt->SetClientImage( TID_None );
	m_pNameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pNameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pNameTxt->SetText( _RT("나이스 신수가 훤허네") );
	m_pNameTxt->SetHID( HID_None );

	m_pTypeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 327, 146, 13, false );
	m_pTypeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pTypeTxt->SetClientImage( TID_None );
	m_pTypeTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pTypeTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pTypeTxt->SetText( _RT("티타므로") );
	m_pTypeTxt->SetHID( HID_None );

	m_pLevelTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 342, 146, 13, false );
	m_pLevelTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pLevelTxt->SetClientImage( TID_None );
	m_pLevelTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pLevelTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pLevelTxt->SetText( _RT("99 Lv") );
	m_pLevelTxt->SetHID( HID_None );

	m_pExpTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 357, 146, 13, false );
	m_pExpTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pExpTxt->SetClientImage( TID_None );
	m_pExpTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pExpTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pExpTxt->SetText( _RT("99 Lv")	);
	m_pExpTxt->SetHID( HID_None );

	m_pEnergyTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 372, 146, 13, false );
	m_pEnergyTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pEnergyTxt->SetClientImage( TID_None );
	m_pEnergyTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pEnergyTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pEnergyTxt->SetText( _RT("3999 Lv") );
	m_pEnergyTxt->SetHID( HID_None );

	m_pSummonsBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 312, nButtonW, 20, false ); 
	m_pSummonsBtn->SetClientImage( TID_None );
	m_pSummonsBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pSummonsBtn->SetFontR( n_fRsvFontWndButton );	
	m_pSummonsBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_SUMMONS ) );

	m_pCallOffBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 337, nButtonW, 20, false ); 
	m_pCallOffBtn->SetClientImage( TID_None );
	m_pCallOffBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pCallOffBtn->SetFontR( n_fRsvFontWndButton );	
	m_pCallOffBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_CALLOFF ) );

	m_pEnergyUpBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 362, nButtonW, 20, false ); 
	m_pEnergyUpBtn->SetClientImage( TID_None );
	m_pEnergyUpBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pEnergyUpBtn->SetFontR( n_fRsvFontWndButton );	
	m_pEnergyUpBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_ENERGYUP ) );
#ifdef DECO_RENEWAL_GUILD_MJH
	///-----------------------------------------------------------------------
	///-- 공지사항
	m_pNoticeInfoTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 312, 240, 60, false );
	m_pNoticeInfoTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNoticeInfoTxt->SetClientImage( TID_None );
	m_pNoticeInfoTxt->SetAlignText( n_atCenter, n_atCenter );	
	m_pNoticeInfoTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pNoticeInfoTxt->SetMultiLine( true );
	m_pNoticeInfoTxt->SetLineInterval( 3 );
	m_pNoticeInfoTxt->SetText( _RT("공지사항") );
	m_pNoticeInfoTxt->SetHID( HID_None );
	m_pNoticeInfoTxt->SetVisible( FALSE );
	
#endif // DECO_RENEWAL_GUILD_MJH
	
#ifdef COLONY_UPDATE
	///-----------------------------------------------------------------------
	///-- Static Text
	//========================================================================================================
	//========================================================================================================
	// 연합 FRONTIER
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 397, 80, 13, false );
	text->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	text->SetTextColor( 70, 70, 100 );	
	text->SetClientImage( TID_None );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	text->SetText( _RT("연합 FRONTIER") );		// HAN_MSG

	///-- 이름
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 417, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_FRONTIERNAME ) );		// Name
	text->SetText( temp );

	///-- Master
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 432, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_MASTER ) );		// Master
	text->SetText( temp );

	///-- Member
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 447, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_MEMBER ) );		// Member
	text->SetText( temp );

	///-- 명성치 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, 15, 462, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_FAME ) );		// 명성치 
	text->SetText( temp );

	m_pCoFronName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 417, 146, 13, false );
	m_pCoFronName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pCoFronName->SetClientImage( TID_None );
	m_pCoFronName->SetAlignText( n_atLeft, n_atCenter );	
	m_pCoFronName->SetTextColor( TEXT_COLOR_VALUE2 );
	m_pCoFronName->SetText( _RT("") );
	m_pCoFronName->SetHID( HID_None );

	m_pCoFMasterName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 432, 146, 13, false );
	m_pCoFMasterName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pCoFMasterName->SetClientImage( TID_None );
	m_pCoFMasterName->SetAlignText( n_atLeft, n_atCenter );	
	m_pCoFMasterName->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pCoFMasterName->SetText( _RT("") );
	m_pCoFMasterName->SetHID( HID_None );

	m_pCoFMemNum = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 447, 146, 13, false );
	m_pCoFMemNum->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pCoFMemNum->SetClientImage( TID_None );
	m_pCoFMemNum->SetAlignText( n_atLeft, n_atCenter );	
	m_pCoFMemNum->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pCoFMemNum->SetText( _RT("") );
	m_pCoFMemNum->SetHID( HID_None );
	
	m_pCoFFame = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMAIN_WND, nXPosition, 462, 146, 13, false );
	m_pCoFFame->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pCoFFame->SetClientImage( TID_None );
	m_pCoFFame->SetAlignText( n_atLeft, n_atCenter );	
	m_pCoFFame->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pCoFFame->SetText( _RT("") );
	m_pCoFFame->SetHID( HID_None );

	m_pCoFBreakBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMAIN_WND, nXButtonPos, 432, nButtonW, 20, false ); 
	m_pCoFBreakBtn->SetClientImage( TID_None );
	m_pCoFBreakBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pCoFBreakBtn->SetFontR( n_fRsvFontWndButton );	
	m_pCoFBreakBtn->SetText( _RT("연합해지") );   ///HAN_MSG


#endif

}

void CFrontierMainWnd::CompositionOld()
{

}


//-----------------------------------------------------------------------------
void CFrontierMainWnd::Composition()
{

	CompositionNew();

	return;
}

//-----------------------------------------------------------------------------
void CFrontierMainWnd::Update()
{
	static TCHAR tmpStr[128] = {0,};

/*	static int	 cur_percent( 0 );
	static BOOL  IsTwinkle = FALSE;
	static DWORD OldTimer  = timeGetTime(); 
	static BOOL  OnTwinkle = FALSE;

	if( thePcParam->Point > 0 )
	{
	}
	else
	{
		IsTwinkle = FALSE;
	}
	
	if( IsTwinkle == TRUE )
	{
		if( OnTwinkle == TRUE )
		{
			nRui->SendEvent( WID_MainMenu_LVgage, n_emSetPercent     , (EPARAM)0, 0, 0, 0 );
			nRui->SendEvent( WID_MainMenu_LVgage, n_emSetClientImage , (EPARAM)TID_MainMenu_LVHighlight, 0, 0, 0 );
		}
		else
		{
			nRui->SendEvent( WID_MainMenu_LVgage, n_emSetPercent     , (EPARAM)0, 0, 0, 0 );
			nRui->SendEvent( WID_MainMenu_LVgage, n_emSetClientImage , (EPARAM)TID_None, 0, 0, 0 );
		}
	}
	else
	{
			nRui->SendEvent( WID_MainMenu_LVgage, n_emSetPercent     , (EPARAM)cur_percent, 0, 0, 0 );
			nRui->SendEvent( WID_MainMenu_LVgage, n_emSetClientImage , (EPARAM)TID_None, 0, 0, 0 );
	}
*/

	return;
}

void CFrontierMainWnd::Open()
{
	nRui->SendEvent( WID_FRONTIERMAIN_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	

}

void CFrontierMainWnd::Close()
{
	nRui->SendEvent( WID_FRONTIERMAIN_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

}

//-----------------------------------------------------------------------------
void CFrontierMainWnd::Proc( SEventMessage* EMsg )
{
	static TCHAR tmpStr[40] = {0,};

	static long s_DelayTime_Summons = g_nowTime;
	static long s_DelayTime_CallOff = g_nowTime;

	
	if( EMsg->FocusWnd == m_pCloseBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_FRONTIERMAIN_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
	}	
	else if( EMsg->FocusWnd == m_pSecessionBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			g_MessageBox.SetStrParam( nRui->GetPcParamInfo()->PCName );
			g_MessageBox.OpenMessageBox( n_MESSAGEBOX_FRONTIER_SECESSION, G_STRING_CHAR( IDS_MSGBOX_ASK_SECESSION ), 250, 100 );						
		}
	}	
	else if( EMsg->FocusWnd == m_pPaymentBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_MoneyThrowWnd.Open( n_ELDINPUT_FRONTIER_PAYMENT );
		}
	}	
	else if( EMsg->FocusWnd == m_pViewAllBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_FrontierMemberListWnd.Open();			
		}
	}	
	else if( EMsg->FocusWnd == m_pEldShareBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_MoneyThrowWnd.Open( n_ELDINPUT_FRONTIER_ELDSHARE );
		}
	}
	else if( EMsg->FocusWnd == m_pIncPersonsBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_FrontierAddPersons.Open();
		}
	}
	else if( EMsg->FocusWnd == m_pSummonsBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( s_DelayTime_Summons , 3000 ) ) )
			{
				g_RockClient.Send_CsMsngrCallFrontierPet();
				s_DelayTime_Summons = g_nowTime;
			}
		}
	}	
	else if( EMsg->FocusWnd == m_pCallOffBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( s_DelayTime_CallOff , 3000 ) ) )
			{
				g_RockClient.Send_CsMsngrCancelCallFrontierPet();
				s_DelayTime_CallOff = g_nowTime;
			}
		}
	}	
	else if( EMsg->FocusWnd == m_pEnergyUpBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			// 인벤에서 활력제 아이템 찾아서 사용하기 
 			CheckEnergyUpItem();
		}
	}
#ifdef 	COLONY_UPDATE
	else if( EMsg->FocusWnd == m_pCoFBreakBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			///-- 연합해제 버튼 클릭할 경우
			g_MessageBox.OpenMessageBox( n_MESSAGEBOX_FRONTIER_SECESSION, "정말로 연합을 해제 하시겠습니까?", 250, 100 );
		}
	}
#endif
#ifdef DECO_RENEWAL_GUILD_MJH

	else if( EMsg->FocusWnd == m_pGuildMenuBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_FrontierMenuBtnWnd.Open();
		}
	}

	else if( EMsg->FocusWnd == WID_CREATURE_INFO_WND ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetInfoState( n_FRONTIER_CREATURE_INFO  );
		}
	}
	else if( EMsg->FocusWnd == WID_MEMBER_INFO_WND ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetInfoState( n_FRONTIER_MEMBER_INFO  );
		}
	}
	else if( EMsg->FocusWnd == WID_NOTICE_INFO_WND ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			SetInfoState( n_FRONTIER_NOTICE_INFO  );
		}
	}

#endif // DECO_RENEWAL_GUILD_MJH
	
	return;
}

void CFrontierMainWnd::CheckEnergyUpItem()
{
//	static int nItemCode[3] = { 6190 };
	int nItemCode = 6190;			// c_ItemCode_FrontierPet_EnergyUp( 6190 );
 

	for( int i = 0; i < c_MaxInvenItemNum; ++i )
	{
		if( nRui->GetPcInvenInfo()->InvenItem[i] == NULL )
		{
			continue;
		}				
		
		// 활력제 찾기 
//		if( nItemCode[j] == nRui->GetPcInvenInfo()->InvenItem[i]->Code )
		if( nItemCode == nRui->GetPcInvenInfo()->InvenItem[i]->Code )
		{
			g_RockClient.Send_RpCsUse( nRui->GetPcInvenInfo()->InvenItem[i]->ItemUID );
			return;
		}
	}

	// 신수 활력제가 없는 경우 
	SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOTHING_PETHPITEM ) );

	return;
}

void CFrontierMainWnd::GetGradeTypeStr( char* aGradeTypeStr, int aGrade )
{
	if( aGrade == n_FT_Master )
	{
		Rsprintf( RWCHAR(aGradeTypeStr), G_STRING_CHAR( IDS_FRONTIERMAIN_MASTER ) );
	}
	else if( aGrade == n_FT_Major )
	{
		Rsprintf( RWCHAR(aGradeTypeStr), G_STRING_CHAR( IDS_FRONTIERMAIN_MAJOR ) );
	}
	else if( aGrade == n_FT_Assist )
	{
		Rsprintf( RWCHAR(aGradeTypeStr), G_STRING_CHAR( IDS_FRONTIERMAIN_ASSIST ) );
	}
	else if( aGrade == n_FT_Member )
	{
		Rsprintf( RWCHAR(aGradeTypeStr), G_STRING_CHAR( IDS_FRONTIERMAIN_MEMBER ) );
	}
#ifdef DECO_RENEWAL_GUILD_MJH
	else if( aGrade == n_FT_Senior )
	{
		Rsprintf( RWCHAR(aGradeTypeStr), _RT("Senior") );
	}
#endif // DECO_RENEWAL_GUILD_MJH

}



//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================

//-----------------------------------------------------------------------------
void CFrontierMemberListWnd::Init()
{
	m_StartListNum = 0;

	m_FM_Draw_SortType = n_FM_Sort_Name;

	return;
}

void CFrontierMemberListWnd::ClearFrontierMemberListWnd()
{
	m_StartListNum = 0;

	m_FM_Draw_SortType = n_FM_Sort_Name;

	m_FrontierMemberInfoList.clear();

	return;
}

//-----------------------------------------------------------------------------
void CFrontierMemberListWnd::Composition()
{
	int center_x = nRui->Desktop.Width / 2;
	int center_y = nRui->Desktop.Height / 2;
	
	char temp[100] = {0,};
	int i = 0;

	CImageBox* img = NULL;	
	CImageBox9* img9 = NULL;
	CButton* btn = NULL;
	CTextBox* text = NULL;
	
	SRect mainwnd;
#ifdef DECO_RENEWAL_GUILD_MJH
	mainwnd.w	= 770;
#else 
	mainwnd.w	= 520;
#endif // DECO_RENEWAL_GUILD_MJH
	mainwnd.h	= 238;  // c_Max_Draw_MemberList * 22 + 40;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	mainwnd.w	= 720;
	mainwnd.h	= 238;

#endif

	mainwnd.x	= center_x - ( mainwnd.w / 2 );
	mainwnd.y	= center_y - ( mainwnd.h / 2 );

	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_FRONTIERMEMBERLIST_WND, WID_None, 
											   mainwnd.x, mainwnd.y, mainwnd.w, mainwnd.h, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );	
//	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_STATUS );								// 프론티어 회원 정보창 표시 아이콘 
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 ); 	
	m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_FRONTIERMEMBERLIST ) );			// 프론티어 회원 정보창 
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();
  

	SRect sTempRect;

#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.x	= 744;
#else 
	sTempRect.x	= 494;
#endif // DECO_RENEWAL_GUILD_MJH
	sTempRect.y	= 6;
	sTempRect.w	= 13;
	sTempRect.h	= 13;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 694;

#endif

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBERLIST_WND, 
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );  	
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	sTempRect.x	= 3;
	sTempRect.y	= 32;
	sTempRect.w	= 500;
#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.w	= 750;
#else 
	sTempRect.w	= 500;
#endif // DECO_RENEWAL_GUILD_MJH
	sTempRect.h	= 198;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 700;

#endif
	
	///---------------------------------------------------------------------------	
	///-- Static Img
	///---------------------------------------------------------------------------	
	// 이미지 범용 사용 및 리소스 사이즈 절감 효과 (리스트 박스, 뒷배경으로 깔리는 여러 이미지들 )
	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_FRONTIERMEMBERLIST_WND, 
										 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 사용시 필수 호출  
	
	///-- Line	column 4, row 8	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 123, 32, 1, 198, false );   
//	img->SetClientImage( TID_CTRL_LINEDOT );
	img->SetClientImage( TID_CLIENT );
	img->SetClientColor( 200, 200, 225 );
	img->SetScaleTrans( true );

#ifdef DECO_RENEWAL_GUILD_MJH
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 493, 32, 1, 198, false );   
#else 
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 243, 32, 1, 198, false );   
#endif // DECO_RENEWAL_GUILD_MJH
	img->SetClientImage( TID_CLIENT );
	img->SetClientColor( 200, 200, 225 );
	img->SetScaleTrans( true );

#ifdef DECO_RENEWAL_GUILD_MJH
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 573, 32, 1, 198, false );   
#else 
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 323, 32, 1, 198, false );   
#endif // DECO_RENEWAL_GUILD_MJH
	img->SetClientImage( TID_CLIENT );
	img->SetClientColor( 200, 200, 225 );
	img->SetScaleTrans( true );
	
#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.x	= 673;
#else 
	sTempRect.x	= 423;
#endif // DECO_RENEWAL_GUILD_MJH
	sTempRect.y	= 32;
	sTempRect.w	= 1;
	sTempRect.h	= 198;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 523;

#endif

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );   
	img->SetClientImage( TID_CLIENT );
	img->SetClientColor( 200, 200, 225 );
	img->SetScaleTrans( true );

	int ypos = 54;
#ifdef DECO_RENEWAL_GUILD_MJH
	int nLineWidth = 750;
#else 
	int nLineWidth = 500;
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	nLineWidth = 700;

#endif

	for( i = 0; i < c_Max_Draw_MemberList; ++i )
	{
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBERLIST_WND,
										   3, ypos, nLineWidth, 1, false );   
		img->SetClientImage( TID_CLIENT );
		img->SetClientColor( 200, 200, 225 );
		img->SetScaleTrans( true );

		ypos += 22;
	}

#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.x	= 753;
#else 
	sTempRect.x	= 503;
#endif // DECO_RENEWAL_GUILD_MJH
	sTempRect.y	= 32;
	sTempRect.w	= 13;
	sTempRect.h	= 198;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 703;

#endif
	
	// 스크롤바 ( Quest_List )
	m_pListScroll = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None , WID_FRONTIERMEMBERLIST_WND, 
												  sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );
	m_pListScroll->InitSlide( stVertSld, 0, 48, 0, 13 );		//Type, min, max, value, 썸의 폭


	m_pListScroll->SetClientImage( TID_CTRL_SCR_BG );
	m_pListScroll->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pListScroll->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pListScroll->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

	///-----------------------------------------------------------------------
	///-- Static Text
	//========================================================================================================
	//========================================================================================================	
	m_pSortMemberListBtn[ n_FM_Sort_Name ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
											  WID_FRONTIERMEMBERLIST_WND, 3, 33, 120, 22, false ); 
	m_pSortMemberListBtn[ n_FM_Sort_Name ]->SetClientImage( TID_None );
	m_pSortMemberListBtn[ n_FM_Sort_Name ]->SetBtnImage( TID_None, TID_MFJobFieldBtnFocus, TID_MFJobFieldBtnFocus, TID_None );		
	m_pSortMemberListBtn[ n_FM_Sort_Name ]->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_pSortMemberListBtn[ n_FM_Sort_Name ]->SetAlignText( n_atCenter, n_atCenter );
	m_pSortMemberListBtn[ n_FM_Sort_Name ]->SetTextColor( TEXT_COLOR_TITLE );
	m_pSortMemberListBtn[ n_FM_Sort_Name ]->SetText( G_STRING_CHAR( IDS_CHARSTATE_NAME2 ) );	
	m_pSortMemberListBtn[ n_FM_Sort_Name ]->SetScaleTrans( true );

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pSortMemberListBtn[ n_FM_Sort_Level ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
											  WID_FRONTIERMEMBERLIST_WND, 123, 33, 80, 22, false ); 
	m_pSortMemberListBtn[ n_FM_Sort_Level ]->SetClientImage( TID_None );
	m_pSortMemberListBtn[ n_FM_Sort_Level ]->SetBtnImage( TID_None, TID_MFJobFieldBtnFocus, TID_MFJobFieldBtnFocus, TID_None );		
	m_pSortMemberListBtn[ n_FM_Sort_Level ]->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_pSortMemberListBtn[ n_FM_Sort_Level ]->SetAlignText( n_atCenter, n_atCenter );
	m_pSortMemberListBtn[ n_FM_Sort_Level ]->SetTextColor( TEXT_COLOR_TITLE );
	m_pSortMemberListBtn[ n_FM_Sort_Level ]->SetText( G_STRING_CHAR( IDS_CHARSTATE_LEVEL2 ) );	
	m_pSortMemberListBtn[ n_FM_Sort_Level ]->SetScaleTrans( true );

	m_pSortMemberListBtn[ n_FM_Sort_Map ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, 
											  WID_FRONTIERMEMBERLIST_WND, 203, 33, 170, 22, false ); 
	m_pSortMemberListBtn[ n_FM_Sort_Map ]->SetClientImage( TID_None );
	m_pSortMemberListBtn[ n_FM_Sort_Map ]->SetBtnImage( TID_None, TID_MFJobFieldBtnFocus, TID_MFJobFieldBtnFocus, TID_None );		
	m_pSortMemberListBtn[ n_FM_Sort_Map ]->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_pSortMemberListBtn[ n_FM_Sort_Map ]->SetAlignText( n_atCenter, n_atCenter );
	m_pSortMemberListBtn[ n_FM_Sort_Map ]->SetTextColor( TEXT_COLOR_TITLE );
	m_pSortMemberListBtn[ n_FM_Sort_Map ]->SetText( G_STRING_CHAR( IDS_WND_AREA2 ) );	
	m_pSortMemberListBtn[ n_FM_Sort_Map ]->SetScaleTrans( true );
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pSortMemberListBtn[ n_FM_Sort_NickName ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBERLIST_WND, 373, 33, 120, 22, false ); 
#else 
	m_pSortMemberListBtn[ n_FM_Sort_NickName ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBERLIST_WND, 123, 33, 120, 22, false ); 
#endif // DECO_RENEWAL_GUILD_MJH
	m_pSortMemberListBtn[ n_FM_Sort_NickName ]->SetClientImage( TID_None );
	m_pSortMemberListBtn[ n_FM_Sort_NickName ]->SetBtnImage( TID_None, TID_MFJobFieldBtnFocus, TID_MFJobFieldBtnFocus, TID_None );		
	m_pSortMemberListBtn[ n_FM_Sort_NickName ]->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_pSortMemberListBtn[ n_FM_Sort_NickName ]->SetAlignText( n_atCenter, n_atCenter );
	m_pSortMemberListBtn[ n_FM_Sort_NickName ]->SetTextColor( TEXT_COLOR_TITLE );
	m_pSortMemberListBtn[ n_FM_Sort_NickName ]->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_NICKNAME ) );	
	m_pSortMemberListBtn[ n_FM_Sort_NickName ]->SetScaleTrans( true );

#ifdef DECO_RENEWAL_GUILD_MJH
	m_pSortMemberListBtn[ n_FM_Sort_Grade ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBERLIST_WND, 493, 33, 80, 22, false ); 
#else 
	m_pSortMemberListBtn[ n_FM_Sort_Grade ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBERLIST_WND, 243, 33, 80, 22, false ); 
#endif // DECO_RENEWAL_GUILD_MJH
	m_pSortMemberListBtn[ n_FM_Sort_Grade ]->SetClientImage( TID_None );
	m_pSortMemberListBtn[ n_FM_Sort_Grade ]->SetBtnImage( TID_None, TID_MFJobFieldBtnFocus, TID_MFJobFieldBtnFocus, TID_None );		
	m_pSortMemberListBtn[ n_FM_Sort_Grade ]->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_pSortMemberListBtn[ n_FM_Sort_Grade ]->SetAlignText( n_atCenter, n_atCenter );
	m_pSortMemberListBtn[ n_FM_Sort_Grade ]->SetTextColor( TEXT_COLOR_TITLE );
	m_pSortMemberListBtn[ n_FM_Sort_Grade ]->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_GRADE ) );	
	m_pSortMemberListBtn[ n_FM_Sort_Grade ]->SetScaleTrans( true );

#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.x	= 573;
#else 
	sTempRect.x	= 323;
#endif // DECO_RENEWAL_GUILD_MJH
	sTempRect.y	= 33;
	sTempRect.w	= 100;
	sTempRect.h	= 22;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 200;

#endif

	m_pSortMemberListBtn[ n_FM_Sort_AccumulationFee ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBERLIST_WND, 
														 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false ); 
	m_pSortMemberListBtn[ n_FM_Sort_AccumulationFee ]->SetClientImage( TID_None );
	m_pSortMemberListBtn[ n_FM_Sort_AccumulationFee ]->SetBtnImage( TID_None, TID_MFJobFieldBtnFocus, TID_MFJobFieldBtnFocus, TID_None );		
	m_pSortMemberListBtn[ n_FM_Sort_AccumulationFee ]->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_pSortMemberListBtn[ n_FM_Sort_AccumulationFee ]->SetAlignText( n_atCenter, n_atCenter );
	m_pSortMemberListBtn[ n_FM_Sort_AccumulationFee ]->SetTextColor( TEXT_COLOR_TITLE );
	m_pSortMemberListBtn[ n_FM_Sort_AccumulationFee ]->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_ACCUMULATIONFEE ) );	
	m_pSortMemberListBtn[ n_FM_Sort_AccumulationFee ]->SetScaleTrans( true );

#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.x	= 673;
#else 
	sTempRect.x	= 423;
#endif // DECO_RENEWAL_GUILD_MJH
	sTempRect.y	= 33;
	sTempRect.w	= 80;
	sTempRect.h	= 22;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 523;
	sTempRect.w	= 180;

#endif

	m_pSortMemberListBtn[ n_FM_Sort_Fame ] = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBERLIST_WND, 
													    sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false ); 
	m_pSortMemberListBtn[ n_FM_Sort_Fame ]->SetClientImage( TID_None );
	m_pSortMemberListBtn[ n_FM_Sort_Fame ]->SetBtnImage( TID_None, TID_MFJobFieldBtnFocus, TID_MFJobFieldBtnFocus, TID_None );		
	m_pSortMemberListBtn[ n_FM_Sort_Fame ]->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	m_pSortMemberListBtn[ n_FM_Sort_Fame ]->SetAlignText( n_atCenter, n_atCenter );
	m_pSortMemberListBtn[ n_FM_Sort_Fame ]->SetTextColor( TEXT_COLOR_TITLE );
	m_pSortMemberListBtn[ n_FM_Sort_Fame ]->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_FAME ) );	
	m_pSortMemberListBtn[ n_FM_Sort_Fame ]->SetScaleTrans( true );

	ypos = 54;

	for( i = 0; i < c_Max_Draw_MemberList; ++i )
	{
		m_DrawMemberList[ i ].NameBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBERLIST_WND, 3, ypos, 120, 22, false );
		m_DrawMemberList[ i ].NameBtn->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_DrawMemberList[ i ].NameBtn->SetBtnImage( TID_None, TID_MFJobFieldBtnFocus, TID_MFJobFieldBtnFocus, TID_None );		
		m_DrawMemberList[ i ].NameBtn->SetTextColor( TEXT_COLOR_VALUE1 );	
		m_DrawMemberList[ i ].NameBtn->SetClientImage( TID_None );	
		m_DrawMemberList[ i ].NameBtn->SetAlignText( n_atCenter, n_atCenter );		
		m_DrawMemberList[ i ].NameBtn->SetText( _RT( "=[NiceguY]=" ) );							
//		m_DrawMemberList[ i ].NameBtn->SetFocusCheck( false );
#ifdef DECO_RENEWAL_GUILD_MJH
		m_DrawMemberList[ i ].LevelTxt = (CTextBox*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBERLIST_WND, 123, ypos, 80, 22, false );
		m_DrawMemberList[ i ].LevelTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_DrawMemberList[ i ].LevelTxt->SetTextColor( TEXT_COLOR_VALUE1 );	
		m_DrawMemberList[ i ].LevelTxt->SetClientImage( TID_None );	
		m_DrawMemberList[ i ].LevelTxt->SetAlignText( n_atCenter, n_atCenter );		
		m_DrawMemberList[ i ].LevelTxt->SetText( _RT( "100" ) );

		m_DrawMemberList[ i ].MapTxt = (CTextBox*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBERLIST_WND, 203, ypos, 170, 22, false );
		m_DrawMemberList[ i ].MapTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_DrawMemberList[ i ].MapTxt->SetTextColor( TEXT_COLOR_VALUE1 );	
		m_DrawMemberList[ i ].MapTxt->SetClientImage( TID_None );	
		m_DrawMemberList[ i ].MapTxt->SetAlignText( n_atCenter, n_atCenter );			
		m_DrawMemberList[ i ].MapTxt->SetText( _RT( "망각의 벌판 -레테평원-" ) );
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef DECO_RENEWAL_GUILD_MJH
		m_DrawMemberList[ i ].NickNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 373, ypos, 120, 22, false );
#else 
		m_DrawMemberList[ i ].NickNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 123, ypos, 120, 22, false );
#endif // DECO_RENEWAL_GUILD_MJH
		m_DrawMemberList[ i ].NickNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_DrawMemberList[ i ].NickNameTxt->SetTextColor( TEXT_COLOR_VALUE1 );	
		m_DrawMemberList[ i ].NickNameTxt->SetClientImage( TID_None );	
		m_DrawMemberList[ i ].NickNameTxt->SetAlignText( n_atCenter, n_atCenter );		
		m_DrawMemberList[ i ].NickNameTxt->SetText( _RT( "카리스마된장" ) );							

#ifdef DECO_RENEWAL_GUILD_MJH
		m_DrawMemberList[ i ].GradeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 493, ypos, 80, 22, false );
#else 
		m_DrawMemberList[ i ].GradeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 243, ypos, 80, 22, false );
#endif // DECO_RENEWAL_GUILD_MJH
		m_DrawMemberList[ i ].GradeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_DrawMemberList[ i ].GradeTxt->SetTextColor( TEXT_COLOR_VALUE1 );	
		m_DrawMemberList[ i ].GradeTxt->SetClientImage( TID_None );	
		m_DrawMemberList[ i ].GradeTxt->SetAlignText( n_atCenter, n_atCenter );		
		m_DrawMemberList[ i ].GradeTxt->SetText( _RT( "Master" ) );							

		int nWidth = 100;
#ifdef DECO_RENEWAL_GUILD_MJH
		int nXpos = 673;
#else 
		int nXpos = 423;
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef APPLY_ENGLISH_ORIGIN_SIZE

		nWidth = 200;

#endif	
#ifdef DECO_RENEWAL_GUILD_MJH
		m_DrawMemberList[ i ].AccumulationFeeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 
												   573, ypos, nWidth, 22, false );
#else 
		m_DrawMemberList[ i ].AccumulationFeeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 
												   323, ypos, nWidth, 22, false );
#endif // DECO_RENEWAL_GUILD_MJH
		m_DrawMemberList[ i ].AccumulationFeeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_DrawMemberList[ i ].AccumulationFeeTxt->SetTextColor( TEXT_COLOR_VALUE1 );	
		m_DrawMemberList[ i ].AccumulationFeeTxt->SetClientImage( TID_None );	
		m_DrawMemberList[ i ].AccumulationFeeTxt->SetAlignText( n_atCenter, n_atCenter );		
		m_DrawMemberList[ i ].AccumulationFeeTxt->SetText( _RT( "1234567890 엘드" ) );							

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
		nXpos = 523;
		nWidth = 180;

#endif				
		
		m_DrawMemberList[ i ].FameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBERLIST_WND,
												   nXpos, ypos, nWidth, 22, false );
		m_DrawMemberList[ i ].FameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_DrawMemberList[ i ].FameTxt->SetTextColor( TEXT_COLOR_VALUE1 );	
		m_DrawMemberList[ i ].FameTxt->SetClientImage( TID_None );	
		m_DrawMemberList[ i ].FameTxt->SetAlignText( n_atCenter, n_atCenter );		
		m_DrawMemberList[ i ].FameTxt->SetText( _RT( "1234567890" ) );							
		
		ypos += 22;
	}
	

/*
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 15, 33, 80, 15, false );
//	text->SetFontR( n_fRsvFontTitle );
	text->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdNormal );
	text->SetTextColor( 70, 70, 100 );	
	text->SetClientImage( TID_None );	
	text->SetAlignText( n_atLeft, n_atCenter );		
	text->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_MYINFO ) );							// MY INFO 
	text->SetFocusCheck( false );

	///-- 호칭 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 15, 53, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_NICKNAME ) );				// 호칭  
	text->SetText( temp );

	m_pNickNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBERLIST_WND, 75, 53, 146, 13, false );
	m_pNickNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNickNameTxt->SetClientImage( TID_None );
//	m_pNickNameTxt->SetClientColor( 180, 175, 160 );
	m_pNickNameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pNickNameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pNickNameTxt->SetText( "카리스마단장" );
	m_pNickNameTxt->SetHID( HID_None );
*/

	return;
}



//-----------------------------------------------------------------------------
void CFrontierMemberListWnd::Update()
{
	return;
}

//-----------------------------------------------------------------------------
void CFrontierMemberListWnd::Proc( SEventMessage* EMsg )
{
	static TCHAR tmpStr[40] = {0,};
//	static int nItemCode[3] = { 9999 };

	int i = 0;


	if( EMsg->FocusWnd == m_pCloseBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_FRONTIERMEMBERLIST_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
	}	
	else if( EMsg->FocusWnd == m_pSortMemberListBtn[ n_FM_Sort_Name ]->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			m_FM_Draw_SortType = n_FM_Sort_Name;
			SortDrawMemberList();
		}
	}
#ifdef DECO_RENEWAL_GUILD_MJH
	else if( EMsg->FocusWnd == m_pSortMemberListBtn[ n_FM_Sort_Level ]->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			m_FM_Draw_SortType = n_FM_Sort_Level;
			SortDrawMemberList();
		}
	}
	else if( EMsg->FocusWnd == m_pSortMemberListBtn[ n_FM_Sort_Map ]->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			m_FM_Draw_SortType = n_FM_Sort_Map;
			SortDrawMemberList();
		}
	}
#endif // DECO_RENEWAL_GUILD_MJH
	else if( EMsg->FocusWnd == m_pSortMemberListBtn[ n_FM_Sort_NickName ]->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			m_FM_Draw_SortType = n_FM_Sort_NickName;
			SortDrawMemberList();
		}
	}	
	else if( EMsg->FocusWnd == m_pSortMemberListBtn[ n_FM_Sort_Grade ]->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			m_FM_Draw_SortType = n_FM_Sort_Grade;
			SortDrawMemberList();
		}
	}	
	else if( EMsg->FocusWnd == m_pSortMemberListBtn[ n_FM_Sort_AccumulationFee ]->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			m_FM_Draw_SortType = n_FM_Sort_AccumulationFee;
			SortDrawMemberList();
		}
	}	
	else if( EMsg->FocusWnd == m_pSortMemberListBtn[ n_FM_Sort_Fame ]->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			m_FM_Draw_SortType = n_FM_Sort_Fame;
			SortDrawMemberList();
		}
	}	
	else if( EMsg->FocusWnd == m_pListScroll->GetID() ) 
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || ( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || ( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetFMemberListScroll( (int)EMsg->fParam );			
		}
	}	
	else
	{
#ifdef DECO_RENEWAL_GUILD_MJH // 리뉴얼버전은 일반회원도 정보열람가능
#else		
		if( g_FrontierMainWnd.m_bIsMaster || g_FrontierMainWnd.m_bIsMajor )			// 회원 정보 수정은 마스터 또는 메이저들의 고유 권한 
		{
#endif // DECO_RENEWAL_GUILD_MJH
			for( i = 0; i < c_Max_Draw_MemberList; ++i )
			{
				int MemberNum = 0;
				MemberNum = m_DrawMemberList[ i ].NameBtn->GetID();
				if( EMsg->FocusWnd == m_DrawMemberList[ i ].NameBtn->GetID() )
				{
					if( EMsg->Notify == NM_BTN_RCLICK )
					{
						g_FrontierMemberEditWnd.Open( m_FrontierMemberInfoList[ m_StartListNum + i ] );			// 회원 정보 수정창 열기 
						nRui->SetModal( WID_FRONTIERMEMBEREDIT_WND );
						break;
					}
				}
			}
#ifdef DECO_RENEWAL_GUILD_MJH
#else
		}
#endif // DECO_RENEWAL_GUILD_MJH
	}

	return;
}

void CFrontierMemberListWnd::Open()
{
	nRui->SendEvent( WID_FRONTIERMEMBERLIST_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	g_IsAscend_Sort = false;
	m_FM_Draw_SortType = n_FM_Sort_Grade;
	SortDrawMemberList();	

	Init();

}

void CFrontierMemberListWnd::Close()
{
	nRui->SendEvent( WID_FRONTIERMEMBERLIST_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );

}

void CFrontierMemberListWnd::SortDrawMemberList()
{
	switch( m_FM_Draw_SortType )
	{
	case n_FM_Sort_Name:
		{
			qsort( (void *) &m_FrontierMemberInfoList[0], (size_t) m_FrontierMemberInfoList.size(), sizeof( SFMemberInfo ), SortMemberNameList );
		}
		break;
	case n_FM_Sort_NickName:
		{
			qsort( (void *) &m_FrontierMemberInfoList[0], (size_t) m_FrontierMemberInfoList.size(), sizeof( SFMemberInfo ), SortMemberNickNameList );
		}
		break;
	case n_FM_Sort_Grade:
		{
			qsort( (void *) &m_FrontierMemberInfoList[0], (size_t) m_FrontierMemberInfoList.size(), sizeof( SFMemberInfo ), SortMemberGradeList );
		}
		break;
	case n_FM_Sort_AccumulationFee:
		{
			qsort( (void *) &m_FrontierMemberInfoList[0], (size_t) m_FrontierMemberInfoList.size(), sizeof( SFMemberInfo ), SortMemberAccumulationFeeList );
		}
		break;
	case n_FM_Sort_Fame:
		{
			qsort( (void *) &m_FrontierMemberInfoList[0], (size_t) m_FrontierMemberInfoList.size(), sizeof( SFMemberInfo ), SortMemberFameList );
		}
		break;
#ifdef DECO_RENEWAL_GUILD_MJH
	case n_FM_Sort_Level:
		{
			qsort( (void *) &m_FrontierMemberInfoList[0], (size_t) m_FrontierMemberInfoList.size(), sizeof( SFMemberInfo ), SortMemberLevelList );
		}
		break;
	case n_FM_Sort_Map:
		{
			qsort( (void *) &m_FrontierMemberInfoList[0], (size_t) m_FrontierMemberInfoList.size(), sizeof( SFMemberInfo ), SortMemberMapPositionList );
		}
		break;
#endif // DECO_RENEWAL_GUILD_MJH
	default:
		break;
	}

	g_IsAscend_Sort = !( g_IsAscend_Sort );

	m_StartListNum = 0;

	RefreshMemberInfoList();

	return;
}

void CFrontierMemberListWnd::RefreshMemberInfoList()
{
	if( nRui->GetPcInvenInfo()->FrontierMyInfo == NULL )
		return;

	char strTemp[128] = {0, };

	int MemberListNum = m_FrontierMemberInfoList.size();

	for( int i = 0; i < c_Max_Draw_MemberList; ++i )
	{
		if( MemberListNum > ( m_StartListNum + i ) )
		{
			m_DrawMemberList[ i ].NameBtn->SetText( m_FrontierMemberInfoList[ m_StartListNum + i ].Name );			

			if( nRui->GetPcInvenInfo()->FrontierInfo->theStatus == n_Preliminary_Frontier )
			{
				m_DrawMemberList[ i ].NameBtn->SetBtnHold( true );
			}
#ifdef DECO_RENEWAL_GUILD_MJH // 리뉴얼버전은 일반길드원도 회원정보 열람가능
			else
			m_DrawMemberList[ i ].NameBtn->SetBtnHold( false );
#else
			else
			{
				if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master ||
					nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major )
				{
					m_DrawMemberList[ i ].NameBtn->SetBtnHold( false );
				}
				else
				{
					m_DrawMemberList[ i ].NameBtn->SetBtnHold( true );
				}
			}
#endif // DECO_RENEWAL_GUILD_MJH
			m_DrawMemberList[ i ].NickNameTxt->SetText( m_FrontierMemberInfoList[ m_StartListNum + i ].NickName );										

			g_FrontierMainWnd.GetGradeTypeStr( strTemp, m_FrontierMemberInfoList[ m_StartListNum + i ].Grade );
			m_DrawMemberList[ i ].GradeTxt->SetText( strTemp );						
			
			Ritoa( m_FrontierMemberInfoList[ m_StartListNum + i ].AccumulationFee , RWCHAR(strTemp), 10 );
			m_DrawMemberList[ i ].AccumulationFeeTxt->SetText( strTemp );		

			Ritoa( m_FrontierMemberInfoList[ m_StartListNum + i ].Fame, RWCHAR(strTemp), 10 );
			m_DrawMemberList[ i ].FameTxt->SetText( strTemp );
#ifdef DECO_RENEWAL_GUILD_MJH
			//현재 레벨정보랑 위치정보는 메신져서버에서 받아오기때문에 오프라인이거나 설정에서 비공개로 하면
			// 정보 표시불가능 ( ? ) 표시만가능
			if( m_FrontierMemberInfoList[ m_StartListNum + i ].Level != -1 )
			{
				Ritoa( m_FrontierMemberInfoList[ m_StartListNum + i ].Level, RWCHAR(strTemp), 10 );
				m_DrawMemberList[ i ].LevelTxt->SetText( strTemp ); // 레벨정보
			}
			else	
			m_DrawMemberList[ i ].LevelTxt->SetText(G_STRING_CHAR(IDS_WND_DO_NOT_KNOW1));	

			if( m_FrontierMemberInfoList[ m_StartListNum + i ].TheMap != -1 )
			{
				Rsprintf( RWCHAR(strTemp) , _RT("%s") , G_STRING_CHAR( g_MapDataManager.GetStrID( m_FrontierMemberInfoList[ m_StartListNum + i ].TheMap ) ) );
				m_DrawMemberList[ i ].MapTxt->SetText( strTemp ); // 위치정보
			}
			else
			m_DrawMemberList[ i ].MapTxt->SetText(G_STRING_CHAR(IDS_WND_DO_NOT_KNOW3));

#endif // DECO_RENEWAL_GUILD_MJH
		}
		else
		{
	
			m_DrawMemberList[ i ].NameBtn->SetText( _RT( "" ) );					
			m_DrawMemberList[ i ].NameBtn->SetBtnHold( true );
			m_DrawMemberList[ i ].NickNameTxt->SetText( _RT( "" ) );										
			m_DrawMemberList[ i ].GradeTxt->SetText( _RT( "" ) );										
			m_DrawMemberList[ i ].AccumulationFeeTxt->SetText( _RT( "" ) );		
			m_DrawMemberList[ i ].FameTxt->SetText( _RT( "" ) );
#ifdef DECO_RENEWAL_GUILD_MJH
			m_DrawMemberList[ i ].LevelTxt->SetText( _RT( "" ) );		
			m_DrawMemberList[ i ].MapTxt->SetText( _RT( "" ) );
#endif // DECO_RENEWAL_GUILD_MJH
		}
	}
	
	return;
}


//-----------------------------------------------------------------------------
void CFrontierMemberListWnd::SetWheelValue( short _wheel )
{
	int value = 0;
	int Max   = 0;
	

	Max = m_pListScroll->GetSldMax();
	m_pListScroll->GetValue( &value );
	
	if( ( _wheel < 0 ) && ( value < Max ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	m_pListScroll->SetValue( value );
	SetFMemberListScroll( value );
	
	return;
}


void CFrontierMemberListWnd::SetFMemberListScroll( int aLinenum )
{
	m_StartListNum = aLinenum * 1;
	
	RefreshMemberInfoList();

	return;
}

//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================
//================================================================================================================


//-----------------------------------------------------------------------------

void CFrontierMemberEditWnd::Init()
{
	Rsprintf( RWCHAR(m_FrontierGradeStr[ n_Frontier_Master_Grade ]), G_STRING_CHAR( IDS_FRONTIERMAIN_MASTER ) );
	Rsprintf( RWCHAR(m_FrontierGradeStr[ n_Frontier_Major_Grade ]), G_STRING_CHAR( IDS_FRONTIERMAIN_MAJOR ) );
	Rsprintf( RWCHAR(m_FrontierGradeStr[ n_Frontier_Assist_Grade ]), G_STRING_CHAR( IDS_FRONTIERMAIN_ASSIST ) );
	Rsprintf( RWCHAR(m_FrontierGradeStr[ n_Frontier_Senior_Grade ]), _RT( "Senior") );
	Rsprintf( RWCHAR(m_FrontierGradeStr[ n_Frontier_Member_Grade ]), G_STRING_CHAR( IDS_FRONTIERMAIN_MEMBER ) );

	return;
}
//-----------------------------------------------------------------------------
void CFrontierMemberEditWnd::Composition()
{
	int center_x = nRui->Desktop.Width / 2;
	int center_y = nRui->Desktop.Height / 2;
	
	char temp[100] = {0,};

	CImageBox* img = NULL;	
	CImageBox9* img9 = NULL;
	CButton* btn = NULL;
	CTextBox* text = NULL;
	
	SRect mainwnd;
	mainwnd.w	= 220;
#ifdef DECO_RENEWAL_GUILD_MJH
	mainwnd.h	= 302;
#else 
	mainwnd.h	= 202;
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	mainwnd.w	= 340;

#endif
	
	mainwnd.x	= center_x - ( mainwnd.w / 2 );
	mainwnd.y	= center_y - ( mainwnd.h / 2 );


	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_FRONTIERMEMBEREDIT_WND, WID_None, mainwnd.x, mainwnd.y, mainwnd.w, mainwnd.h, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );	
//	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_STATUS );									// 프론티어창 표시 아이콘 
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 ); 	
	m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_WND_FRONTIERMEMBEREDIT ) );				// 프론티어 정보 수정창 
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	SRect sTempRect;

	sTempRect.x	= 194;
	sTempRect.y	= 6;
	sTempRect.w	= 13;
	sTempRect.h	= 13;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 314;	

#endif	

	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- X
	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
											 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );  	
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	sTempRect.x	= 1;
	sTempRect.y	= 137;
	sTempRect.w	= 220;
	sTempRect.h	= 1;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 338;	

#endif	
	
	///---------------------------------------------------------------------------	
	///-- Static Img
	///---------------------------------------------------------------------------	
	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );   
	img ->SetClientImage( TID_CTRL_LINEDOT );

	sTempRect.x	= 1;
#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.y	= 262;
#else 
	sTempRect.y	= 162;
#endif // DECO_RENEWAL_GUILD_MJH
	sTempRect.w	= 219;
	sTempRect.h	= 1;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 338;	

#endif	

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );   
	img ->SetClientImage( TID_CTRL_LINEDOT );

	sTempRect.x	= 1;
	sTempRect.y	= 32;
	sTempRect.w	= 219;
#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.h	= 165;
#else 
	sTempRect.h	= 105;
#endif // DECO_RENEWAL_GUILD_MJH

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 338;	

#endif	

	// 이미지 범용 사용 및 리소스 사이즈 절감 효과 (리스트 박스, 뒷배경으로 깔리는 여러 이미지들 )
	///-- Image9
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
										 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 사용시 필수 호출  

	sTempRect.x	= 72;
	sTempRect.y	= 55;
	sTempRect.w	= 105;
	sTempRect.h	= 17;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x = 197;

#endif

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );   
	img->SetClientImage( TID_CLIENT );
	img->SetClientColor( 0, 0, 5 );
	img->SetScaleTrans( true );

	sTempRect.x	= 72;
	sTempRect.y	= 75;

	sTempRect.w	= 130;
	sTempRect.h	= 17;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x = 197;

#endif

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
									   sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );   
	img->SetClientImage( TID_CLIENT );
	img->SetClientColor( 30, 30, 75 );
	img->SetScaleTrans( true );

	sTempRect.x	= 1;
#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.y	= 263;
#else 
	sTempRect.y	= 163;
#endif // DECO_RENEWAL_GUILD_MJH
	sTempRect.w	= 220;
	sTempRect.h	= 35;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.w	= 338;	

#endif	
	
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
										 sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false );       	
	img9->SetWndTileSizeLR( 9, 9 );					// 윈도우 생성시와 동일 
	img9->SetWndTileSizeTB( 8, 8 );					// 윈도우 생성시와 동일 
	img9->SetFocusCheck( false );					// 이미지 사용시 필수 
	img9->SetClientImage( TID_CTRL_CONTBACK_L1 );
	img9->SetAdjustPosition( 0, 0 );				// n_wtImageBox9 사용시 필수 호출  

	///-----------------------------------------------------------------------
	///-- Static Text
	//========================================================================================================
	//========================================================================================================
	///-- 캐릭터 이름 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 15, 38, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_CHARSTATE_NAME2 ) );						// 캐릭터 이름 
	text->SetText( temp );
	
	///-- 호칭 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 15, 58, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_NICKNAME ) );				// 호칭  
	text->SetText( temp );
	
	///-- 등급 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 15, 78, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_GRADE ) );					// 등급  
	text->SetText( temp );

	///-- 누적회비 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 15, 98, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_ACCUMULATIONFEE ) );		// 누적회비   
	text->SetText( temp );
	
	///-- 명성치 
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 15, 118, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	Rsprintf( RWCHAR(temp),_RT( "%s:"), G_STRING_CHAR( IDS_FRONTIERMAIN_FAME ) );		// 명성치 
	text->SetText( temp );
#ifdef DECO_RENEWAL_GUILD_MJH
	text = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 85, 138, 80, 13, false );
	text->SetFontR( n_fRsvFontWndStatic );
	text->SetTextColor( TEXT_COLOR_STATIC );	
	text->SetAlignText( n_atLeft, n_atCenter );	
	text->SetText( _RT( "자기소개") );                                                  // 자기소개

	m_pMyinfoTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
											   15, 158, 300, 34, false );
	m_pMyinfoTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMyinfoTxt->SetClientImage( TID_None );
	m_pMyinfoTxt->SetAlignText( n_atCenter, n_atCenter );	
	m_pMyinfoTxt->SetTextColor( 255, 50, 50 );
	m_pMyinfoTxt->SetMultiLine( true );
	m_pMyinfoTxt->SetLineInterval( 2 );
	m_pMyinfoTxt->SetText( _RT("") );
	m_pMyinfoTxt->SetHID( HID_None );
#endif // DECO_RENEWAL_GUILD_MJH

	sTempRect.x	= 136;
#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.y	= 235;
#else
	sTempRect.y	= 140; 
#endif // DECO_RENEWAL_GUILD_MJH
	
	sTempRect.w	= 58;
	sTempRect.h	= 20;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x = 101;
	sTempRect.w	= 138;

#endif

	m_pKickBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
											sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false ); 
	m_pKickBtn->SetClientImage( TID_None );
	m_pKickBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pKickBtn->SetFontR( n_fRsvFontWndButton );	
	m_pKickBtn->SetText( G_STRING_CHAR( IDS_WND_FRONTIERMEMBEREDIT_KICK ) );
#ifdef DECO_RENEWAL_GUILD_MJH

	m_pWhisperBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
											26, 205, sTempRect.w, sTempRect.h, false ); 
	m_pWhisperBtn->SetClientImage( TID_None );
	m_pWhisperBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pWhisperBtn->SetFontR( n_fRsvFontWndButton );	
	m_pWhisperBtn->SetText( G_STRING_CHAR( IDS_WND_WHISPER ) );

	m_pPartyinvitationBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
											136, 205, sTempRect.w, sTempRect.h, false ); 
	m_pPartyinvitationBtn->SetClientImage( TID_None );
	m_pPartyinvitationBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pPartyinvitationBtn->SetFontR( n_fRsvFontWndButton );	
	m_pPartyinvitationBtn->SetText( G_STRING_CHAR( IDS_WND_PRATY_REQUEST ) );

	m_pMasterchangeBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
											26, 235, sTempRect.w, sTempRect.h, false ); 
	m_pMasterchangeBtn->SetClientImage( TID_None );
	m_pMasterchangeBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pMasterchangeBtn->SetFontR( n_fRsvFontWndButton );	
	m_pMasterchangeBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_MASTER ) ); 

#endif // DECO_RENEWAL_GUILD_MJH 

	sTempRect.x	= 26;
#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.y	= 270;
#else 
	sTempRect.y	= 170;
#endif // DECO_RENEWAL_GUILD_MJH
	sTempRect.w	= 58;
	sTempRect.h	= 20;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 86;

#endif

	m_pOKBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
										  sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false ); 
	m_pOKBtn->SetClientImage( TID_None );
	m_pOKBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pOKBtn->SetFontR( n_fRsvFontWndButton );	
	m_pOKBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );
	
	sTempRect.x	= 136;
#ifdef DECO_RENEWAL_GUILD_MJH
	sTempRect.y	= 270;
#else 
	sTempRect.y	= 170;
#endif // DECO_RENEWAL_GUILD_MJH
	sTempRect.w	= 58;
	sTempRect.h	= 20;

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	sTempRect.x	= 196;

#endif

	m_pCancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIERMEMBEREDIT_WND, 
											  sTempRect.x, sTempRect.y, sTempRect.w, sTempRect.h, false ); 
	m_pCancelBtn->SetClientImage( TID_None );
	m_pCancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pCancelBtn->SetFontR( n_fRsvFontWndButton );	
	m_pCancelBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );
	
	int nXPos = 75; 

#ifdef APPLY_ENGLISH_ORIGIN_SIZE
	
	nXPos = 195;

#endif
	
	m_pMemberNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, nXPos, 38, 146, 13, false );
	m_pMemberNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMemberNameTxt->SetClientImage( TID_None );
	m_pMemberNameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pMemberNameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pMemberNameTxt->SetText( _RT("나이수가이짱") );
	m_pMemberNameTxt->SetHID( HID_None );

	m_pNickNameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, nXPos, 58, 146, 13, false );
	m_pNickNameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNickNameTxt->SetClientImage( TID_None );
	m_pNickNameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pNickNameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pNickNameTxt->SetText( _RT("카리스마단장") );
	m_pNickNameTxt->SetHID( HID_None );
	m_pNickNameTxt->SetVisible( false );
	
	m_pGradeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, nXPos, 78, 146, 13, false );
	m_pGradeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pGradeTxt->SetClientImage( TID_None );
	m_pGradeTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pGradeTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pGradeTxt->SetText( "Master" );
	m_pGradeTxt->SetHID( HID_None );
	m_pGradeTxt->SetVisible( false );

	m_pAccumulationFeeTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, nXPos, 98, 146, 13, false );
	m_pAccumulationFeeTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pAccumulationFeeTxt->SetClientImage( TID_None );
	m_pAccumulationFeeTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pAccumulationFeeTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pAccumulationFeeTxt->SetText( _RT("1234567 엘드") );
	m_pAccumulationFeeTxt->SetHID( HID_None );

	m_pFameTxt = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, nXPos, 118, 146, 13, false );
	m_pFameTxt->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pFameTxt->SetClientImage( TID_None );
	m_pFameTxt->SetAlignText( n_atLeft, n_atCenter );	
	m_pFameTxt->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pFameTxt->SetText( _RT("1234567890") );
	m_pFameTxt->SetHID( HID_None );
	
	m_pNickNameEdit = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, nXPos, 58, 100, 13, false );
	m_pNickNameEdit->InitEditBox();
	m_pNickNameEdit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pNickNameEdit->SetTextSize( 14 );
	m_pNickNameEdit->SetClientImage( TID_None );
	m_pNickNameEdit->SetClientColor( 5, 5, 5 );
	m_pNickNameEdit->SetAlignText( n_atLeft, n_atCenter );
	m_pNickNameEdit->SetBlank( 4, 0, 0, 0 );
	m_pNickNameEdit->SetEditStyle( esText );
	m_pNickNameEdit->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pNickNameEdit->SetText( _RT("") );		
	m_pNickNameEdit->SetText( _RT("카리스마단장") );
	m_pNickNameEdit->SetReturnActionClear( false );


	m_pGradeCBox = (CComboBox*)nRui->CreateWnd( n_wtComboBox, WID_None, WID_FRONTIERMEMBEREDIT_WND, nXPos, 75, 130, 100, false );	
	m_pGradeCBox->SetListboxTileSizeTB( 5, 5 );
	m_pGradeCBox->SetListboxTileSizeLR( 5, 5 );	
	m_pGradeCBox->SetListboxImage9( TID_CTRL_OUTLINE_L1 );	
	m_pGradeCBox->GetTextBox()->SetTextColor( TEXT_COLOR_VALUE1 );
	m_pGradeCBox->AddItem( G_STRING_CHAR( IDS_FRONTIERMAIN_MAJOR ), n_Frontier_Major_Grade );
	m_pGradeCBox->AddItem( G_STRING_CHAR( IDS_FRONTIERMAIN_ASSIST ), n_Frontier_Assist_Grade );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pGradeCBox->AddItem( _RT("Senior"), n_Frontier_Senior_Grade );
#endif // DECO_RENEWAL_GUILD_MJH
	m_pGradeCBox->AddItem( G_STRING_CHAR( IDS_FRONTIERMAIN_MEMBER ), n_Frontier_Member_Grade );
	m_pGradeCBox->SetSelectText( G_STRING_CHAR( IDS_FRONTIERMAIN_MASTER ) );

	return;
}



//-----------------------------------------------------------------------------
void CFrontierMemberEditWnd::Update()
{
	static TCHAR tmpStr[128] = {0,};

	return;
}

//-----------------------------------------------------------------------------
void CFrontierMemberEditWnd::Proc( SEventMessage* EMsg )
{
	static TCHAR strTemp[40] = {0,};


	if( EMsg->FocusWnd == m_pCloseBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SendEvent( WID_FRONTIERMEMBEREDIT_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			nRui->SetModal( WID_None );
			IsVisible = false;
		}
	}	
	else if( EMsg->FocusWnd == m_pNickNameEdit->GetID() )
	{
		if( EMsg->Notify == NM_EDT_RETURN_ACTION ) 
		{			
			nRui->SetEnterFlag( n_nfEditEnter );
			nRui->SetEnterWndID( m_pNickNameEdit->GetID() );
		}
	}
	else if( EMsg->FocusWnd == m_pGradeCBox->GetID() )
	{		
		if( EMsg->Notify == NM_CB_TEXT_EXCHANGE )	
		{
//			m_pGradeCBox->GetCurSelIdx();			///-- 선택된 Text의 순차적인 Index
//			m_pGradeCBox->GetCurSelValue();			///-- 선택된 Text의 Value( AddItem 의 두번째 인자로 지정된 값 )

			m_GradeType = (NFrontierGradeType) m_pGradeCBox->GetCurSelValue();			
		}
	}
	else if( EMsg->FocusWnd == m_pKickBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{			
			m_pMemberNameTxt->GetText( strTemp, 22 ); 
			g_MessageBox.SetStrParam( strTemp );
			g_MessageBox.OpenMessageBox( n_MESSAGEBOX_FRONTIER_SECESSION, G_STRING_CHAR( IDS_MSGBOX_ASK_REALLYKICK ), 250, 100 );						
		}
	}	
	else if( EMsg->FocusWnd == m_pOKBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			char PCName[RP_MAX_PC_NAME];
			char NickName[RP_MAX_FRONTIER_NICK];
			
			m_pMemberNameTxt->GetText( PCName, RP_MAX_PC_NAME ); 			
			m_pNickNameEdit->GetText( NickName, RP_MAX_FRONTIER_NICK ); 

			if( g_RockClient.m_SlangFilter.Filter( ( char * ) NickName ) )
			{
				Rsprintf( RWCHAR(NickName), _RT("%s"), RWCHAR(g_RockClient.m_SlangFilter.GetString()) );
			}

/*			if( m_MemberInfo.Grade == n_FT_Master )
			{
				if( m_GradeType > n_FT_Master )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );
					return;
				}
			}
			else if( m_MemberInfo.Grade == n_FT_Major )
			{
				if( m_GradeType > n_FT_Major )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );
					return;
				}
			}

			if( m_GradeType == n_FT_Major )
			{
				if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade != n_FT_Master )
				{
ㅁㅁ					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOT_AUTHORITY ) );
					return;
				}
				
				if( nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[0][0] != '\0' && 
					nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[1][0] != '\0' &&
					nRui->GetPcInvenInfo()->FrontierInfo->theMajorName[2][0] != '\0' )
				{					
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_NOMOREMAJOR ) );	
					return;
				}
			}
*/				
			g_RockClient.Send_CsMsngrFrontierMemberInfo( PCName, NickName, (BYTE) m_GradeType );

			nRui->SendEvent( WID_FRONTIERMEMBEREDIT_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			nRui->SetModal( WID_None );
		}
	}	
	else if( EMsg->FocusWnd == m_pCancelBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			nRui->SetModal( WID_None );
			nRui->SendEvent( WID_FRONTIERMEMBEREDIT_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			IsVisible = false;
		}
	}
#ifdef DECO_RENEWAL_GUILD_MJH
	else if( EMsg->FocusWnd == m_pWhisperBtn->GetID() ) 				// 귓속말 
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				Close();
				char PCName[RP_MAX_PC_NAME];				
				m_pMemberNameTxt->GetText( PCName, 22 );
				g_ChatMainProc.m_ChatActPrintWnd.SetWhisDest(PCName);
				g_ChatMainProc.m_ChatActPrintWnd.SetChatState(n_WhisperChat);
				g_ChatMainProc.SetOutState(COutState_Act);				
			}
		}
	else if( EMsg->FocusWnd == m_pPartyinvitationBtn->GetID() ) //파티 초대 
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				Close();
				char PCName[RP_MAX_PC_NAME];				
				m_pMemberNameTxt->GetText( PCName, 22 );			
				g_ChatMainProc.m_ChatActPrintWnd.ExcuteInvitePatry(PCName);
			}
		}
	else if( EMsg->FocusWnd == m_pMasterchangeBtn->GetID() ) //길드장위임 
		{
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				char PCName[RP_MAX_PC_NAME];				
				m_pMemberNameTxt->GetText( PCName, 22 );
				g_MessageBox.SetStrParam( PCName );
				Rsprintf( RWCHAR(strTemp),  G_STRING_CHAR( IDS_SYSCHANGE_PCNAME ) , RWCHAR(PCName) );
				g_MessageBox.OpenMessageBox( n_MESSAGEBOX_CHANGE_GUILDMASTER, strTemp, 250, 100 );
			}
		}

#endif // DECO_RENEWAL_GUILD_MJH
	
	return;
}
 

void CFrontierMemberEditWnd::Open( SFMemberInfo aMemberInfo )
{
	nRui->SendEvent( WID_FRONTIERMEMBEREDIT_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	Init();
	
	char strTemp[128] = {0, };

	m_MemberInfo = aMemberInfo;

	m_pMemberNameTxt->SetText( aMemberInfo.Name );
	m_pNickNameEdit->SetText( aMemberInfo.NickName );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pMyinfoTxt->SetText( aMemberInfo.theIntroduce );
#endif // DECO_RENEWAL_GUILD_MJH	
	g_FrontierMainWnd.GetGradeTypeStr( strTemp, aMemberInfo.Grade );
	m_pGradeTxt->SetText( strTemp );						

	m_GradeType = (NFrontierGradeType) aMemberInfo.Grade;
	
	Ritoa( aMemberInfo.AccumulationFee, RWCHAR(strTemp), 10 );
	m_pAccumulationFeeTxt->SetText( strTemp );		
	
	Ritoa( aMemberInfo.Fame, RWCHAR(strTemp), 10 );
	m_pFameTxt->SetText( strTemp );		
	
	m_pGradeCBox->SetSelectText( m_FrontierGradeStr[ m_GradeType ] );		

	m_pNickNameEdit->SetFocusCheck( true );
	m_pGradeCBox->SetFocusCheck( true );
	m_pKickBtn->SetBtnHold( false );
	m_pOKBtn->SetBtnHold( false );
#ifdef DECO_RENEWAL_GUILD_MJH
	m_pPartyinvitationBtn->SetBtnHold( false );
	m_pMasterchangeBtn->SetBtnHold( false );
	m_pWhisperBtn->SetBtnHold( false );

	if( nRui->GetPcInvenInfo()->FrontierMyInfo )
	{
		// 자신보다 등급이 높거나 같은 회원은 수정이 불가능하다. 
		if( m_MemberInfo.Grade == n_FT_Master )
		{
			m_pNickNameEdit->SetFocusCheck( false );
			m_pGradeCBox->SetFocusCheck( false );
			m_pKickBtn->SetBtnHold( true );
			m_pOKBtn->SetBtnHold( true );
			m_pMasterchangeBtn->SetBtnHold( true );

			if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master ) 
			{
				// 자신을 수정하는것은 가능 
				if( Rstrcmp( RWCHAR(aMemberInfo.Name), RWCHAR(nRui->GetPcParamInfo()->PCName) ) == 0 )
				{
					m_pNickNameEdit->SetFocusCheck( true );
					m_pOKBtn->SetBtnHold( false );
				}
			}
		}
		// 리뉴얼버전의경우 메니져는 회원의 등급만 변경가능
		else if( m_MemberInfo.Grade == n_FT_Major )
		{
			if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade >= n_FT_Major )
			{
				m_pNickNameEdit->SetFocusCheck( false );
				m_pGradeCBox->SetFocusCheck( false );
				m_pKickBtn->SetBtnHold( true );
				m_pOKBtn->SetBtnHold( true );
				m_pMasterchangeBtn->SetBtnHold( true );
			}
		}
		// 일반 회원의경우는 정보 열람만가능 변경은 불가능함
		else 
		{		
			if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major )
			{
				m_pNickNameEdit->SetFocusCheck( false );
				m_pMasterchangeBtn->SetBtnHold( true );
			}
			else if(nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade >= n_FT_Assist )
			{
				m_pNickNameEdit->SetFocusCheck( false );
				m_pGradeCBox->SetFocusCheck( false );
				m_pKickBtn->SetBtnHold( true );
				m_pOKBtn->SetBtnHold( true );
				m_pMasterchangeBtn->SetBtnHold( true );
			}
		}
	}
#else
	if( nRui->GetPcInvenInfo()->FrontierMyInfo )
	{
		// 자신보다 등급이 높거나 같은 회원은 수정이 불가능하다. 
		if( m_MemberInfo.Grade == n_FT_Master )
		{
			m_pNickNameEdit->SetFocusCheck( false );
			m_pGradeCBox->SetFocusCheck( false );
			m_pKickBtn->SetBtnHold( true );
			m_pOKBtn->SetBtnHold( true );

			if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master ) 
			{
				// 자신을 수정하는것은 가능 
				if( Rstrcmp( RWCHAR(aMemberInfo.Name), RWCHAR(nRui->GetPcParamInfo()->PCName) ) == 0 )
				{
					m_pNickNameEdit->SetFocusCheck( true );
					m_pOKBtn->SetBtnHold( false );
				}
			}
		}
		else if( m_MemberInfo.Grade == n_FT_Major )
		{
			if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major ) 
			{
				m_pKickBtn->SetBtnHold( true );
			}
		}
		if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major ) 
		{
			m_pNickNameEdit->SetFocusCheck( false );
			m_pGradeCBox->SetFocusCheck( false );
			m_pOKBtn->SetBtnHold( true );
		}
	}
#endif // DECO_RENEWAL_GUILD_MJH
	
}


void CFrontierMemberEditWnd::Close()
{
	nRui->SendEvent( WID_FRONTIERMEMBEREDIT_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SetModal( WID_None );
}
 
// 

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CFrontierAddPersons::Init()
{

}

void CFrontierAddPersons::Composition()
{
	int center_x = nRui->Desktop.Width / 2;
	int center_y = nRui->Desktop.Height / 2;
	
	CImageBox *img;

	SRect mainwnd;
	mainwnd.w	= 235;
	mainwnd.h	= 138;
	mainwnd.x	= center_x - ( mainwnd.w / 2 );
	mainwnd.y	= center_y - ( mainwnd.h / 2 );


	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, mainwnd.x, mainwnd.y, mainwnd.w, mainwnd.h, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );	
//	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_STATUS );									// 프론티어창 표시 아이콘 
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 ); 	
	m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_CHARSTATE_GUILD ) );				
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	///-- Line
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 1, 45, 233, 56, false );
	img->SetClientImage( TID_CTRL_BACK );
	img->SetFocusCheck( false );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(),  1, 44, 233, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 1, 101, 233, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	
	///-----------------------------------------------------------------------
	///-- Static Text
	//========================================================================================================
	//========================================================================================================
	///-- 캐릭터 이름 
	m_pDescTxt[0] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 15, 46 , 220, 13, false );
	m_pDescTxt[0]->SetFontR( n_fRsvFontWndStatic );
	m_pDescTxt[0]->SetTextColor( 0,0,0 );	
	m_pDescTxt[0]->SetAlignText( n_atCenter, n_atCenter );	
	m_pDescTxt[0]->SetText( G_STRING_CHAR(IDS_WND_NEED_ELD) ); 
	
	m_pDescTxt[1] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 15, 60 , 220, 13, false );
	m_pDescTxt[1]->SetFontR( n_fRsvFontWndStatic );
	m_pDescTxt[1]->SetTextColor( 0,0,0  );	
	m_pDescTxt[1]->SetAlignText( n_atCenter, n_atCenter );	
	m_pDescTxt[1]->SetText( _RT("100,000,000 입니다.") );
	
	m_pOKBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(),  56, 108, 58, 25, false );      
	m_pOKBtn->SetClientImage( TID_None );
	m_pOKBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pOKBtn->SetFontR( n_fRsvFontWndButton );	
	m_pOKBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );	

	m_pCancelBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 122, 108, 58, 25, false ); 
	m_pCancelBtn->SetClientImage( TID_None );
	m_pCancelBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pCancelBtn->SetFontR( n_fRsvFontWndButton );	
	m_pCancelBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) );	
			

}

void CFrontierAddPersons::Update()
{

}

void CFrontierAddPersons::Proc( SEventMessage* EMsg)
{
	
	if( EMsg->FocusWnd == m_pOKBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_CsMsngrAddFrontierMember(nRui->GetPcParamInfo()->PCName , nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID );							
			Close();
		}
	}
	else if( EMsg->FocusWnd == m_pCancelBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}

}

void CFrontierAddPersons::Open()
{
	//프론티어 인원증가
	
	if( g_FrontierMainWnd.m_MaxMemberNum >= FRONTIER_NEED_MEMBER ) 
	{
		g_RockClient.GetFrontierProtocolResult(n_Error_FrontierMemberFull);
		return;
	}

	int membernum = g_FrontierMainWnd.m_MaxMemberNum;
	membernum -= 30;
	//테이블화 해야한다..
#ifdef CHINA_VERSION
	double fee = 1000000.0f;
#else
	double fee = 10000000.0f;
#endif
	for( int i = 0; i < membernum; ++i )
	{
		fee = (fee *= 12.0f)/10.0f;
	}
	DWORD dwfee = 0;
	
	dwfee = static_cast<DWORD>(fee);
	TCHAR EldStr[256]= "";
	TCHAR Str[256]="";

	InventoryWnd.SetEldText(EldStr , dwfee );
	Rsprintf(RWCHAR(Str ), G_STRING_CHAR(IDS_WND_FRON_ADDEPRSONS_ELD) , RWCHAR(EldStr) );	
	m_pDescTxt[1]->SetText(Str );

	nRui->SendEvent( m_pFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );

}

void CFrontierAddPersons::Close()
{
	nRui->SendEvent( m_pFrameWnd->GetID() , n_emSetVisible, (EPARAM)false , 0, 0, 0 );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifdef DECO_RENEWAL_GUILD_MJH
//=================================================================================================================
// 길드 버튼 윈도우 
//=================================================================================================================

void CFrontierMenuBtnWnd::Init()
{

}

void CFrontierMenuBtnWnd::Composition()
{
	int center_x = nRui->Desktop.Width / 2;
	int center_y = nRui->Desktop.Height / 2;

	SRect mainwnd;
	mainwnd.w	= 200;
	mainwnd.h	= 172;
	mainwnd.x	= center_x - ( mainwnd.w / 2 );
	mainwnd.y	= center_y - ( mainwnd.h / 2 );


	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_FRONTIER_BTN_WND, WID_None, mainwnd.x, mainwnd.y, mainwnd.w, mainwnd.h, false );
	m_pFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWnd->SetClientImage( TID_CTRL_WND_L1 );	
//	m_pFrameWnd->SetIconTID( TID_CTRL_ICON_STATUS );									// 프론티어창 표시 아이콘 
	m_pFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pFrameWnd->SetWndTileSizeTB( 31, 11 ); 	
	m_pFrameWnd->SetCaption( G_STRING_CHAR( IDS_CHARSTATE_GUILD ) );				
	m_pFrameWnd->SetTitle( true );
	m_pFrameWnd->SetWndProc( this );	
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIER_BTN_WND, 180, 6, 13, 13, false );  	
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	m_pSecessionBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIER_BTN_WND, 10, 53, 180, 20, false ); 
	m_pSecessionBtn->SetClientImage( TID_None );
	m_pSecessionBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pSecessionBtn->SetFontR( n_fRsvFontWndButton );	
	m_pSecessionBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_SECESSION ) );

	m_pPaymentBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIER_BTN_WND, 10, 78, 180, 20, false ); 
	m_pPaymentBtn->SetClientImage( TID_None );
	m_pPaymentBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pPaymentBtn->SetFontR( n_fRsvFontWndButton );	
	m_pPaymentBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_PAYMENT ) );

	m_pMyinfoBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIER_BTN_WND, 10, 103, 180, 20, false ); 
	m_pMyinfoBtn->SetClientImage( TID_None );
	m_pMyinfoBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pMyinfoBtn->SetFontR( n_fRsvFontWndButton );	
	m_pMyinfoBtn->SetText( G_STRING_CHAR( IDS_FRONTIERMAIN_MYINFO ) );

	m_pNoticeBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, WID_FRONTIER_BTN_WND, 10, 128, 180, 20, false ); 
	m_pNoticeBtn->SetClientImage( TID_None );
	m_pNoticeBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );		
	m_pNoticeBtn->SetFontR( n_fRsvFontWndButton );	
	m_pNoticeBtn->SetText( G_STRING_CHAR( IDS_NOTICE_T2 ) );
	

}

void CFrontierMenuBtnWnd::Update()
{

}

void CFrontierMenuBtnWnd::Proc( SEventMessage* EMsg)
{

	if( EMsg->FocusWnd == m_pCloseBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();
		}
	}
	else if( EMsg->FocusWnd == m_pSecessionBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_MessageBox.SetStrParam( nRui->GetPcParamInfo()->PCName ); //탈퇴
			g_MessageBox.OpenMessageBox( n_MESSAGEBOX_FRONTIER_SECESSION, G_STRING_CHAR( IDS_MSGBOX_ASK_SECESSION ), 250, 100 );	
		}
	}
	else if( EMsg->FocusWnd == m_pPaymentBtn->GetID() ) 
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_MoneyThrowWnd.Open( n_ELDINPUT_FRONTIER_PAYMENT ); //납부
		}
	}
	else if( EMsg->FocusWnd == m_pMyinfoBtn->GetID() )  //자기소개
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			
			if( !g_UIMsgWnd.IsVisible )
			{
				g_UIMsgWnd.SetMsgType(MSG_FRONTIERMYINFO_INPUT); 
				nRui->SendEvent( WID_UIMsgWndAdd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				nRui->SendEvent( WID_UIMsgLongInAddEdit, n_emSetFocus, 0, 0, 0, 0 );
			}
		
		}
	}
	else if( EMsg->FocusWnd == m_pNoticeBtn->GetID() )  //공지사항 변경
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			
			if( !g_UIMsgWnd.IsVisible )
			{
				g_UIMsgWnd.SetMsgType(MSG_FRONTIERNOTICE_INPUT);
				nRui->SendEvent( WID_UIMsgWndAdd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
				nRui->SendEvent( WID_UIMsgLongInAddEdit, n_emSetFocus, 0, 0, 0, 0 );
			}
		
		}
	}
	

}

void CFrontierMenuBtnWnd::Open()
{

	nRui->SendEvent( WID_FRONTIER_BTN_WND, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	//공지사항은 길드장과 관리자만 변경가능
	if( nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Master ||
		nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierGrade == n_FT_Major)
	{
		m_pNoticeBtn->SetBtnHold( FALSE );
	}
	else
	{
		m_pNoticeBtn->SetBtnHold( TRUE );
	}
	

}

void CFrontierMenuBtnWnd::Close()
{
	nRui->SendEvent( WID_FRONTIER_BTN_WND , n_emSetVisible, (EPARAM)false , 0, 0, 0 );
}
#endif //DECO_RENEWAL_GUILD_MJH