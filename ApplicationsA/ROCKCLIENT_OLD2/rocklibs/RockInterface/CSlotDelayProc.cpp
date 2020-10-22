// SlotDelayProc.cpp: implementation of the CSlotDelayProc class.
//
//////////////////////////////////////////////////////////////////////

#include "..\\RockPCH.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"
#include "Rui.h"
#include "MainMenuWndProc.h"
#include "LoadingWndProc.h"
#include "CSlotDelayProc.h"

CSlotDelayProc g_SlotDelayProc;
extern long			g_nowTime; 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSlotDelayProc::CSlotDelayProc()
{
  	m_BLoading = FALSE;
	m_WUsecode = 0;
	m_pPInvenItem = NULL;
	m_iTotCount = 0;

	m_WPetUsecode = 0 ; 
	m_BPetLoading = NULL;
	m_pPetPInvenItem = NULL;


	
	m_WSpellSkillCode = 0;
}

CSlotDelayProc::~CSlotDelayProc()
{

}

void CSlotDelayProc::SetSpellSkillCode(WORD WSpellSkillCode)
{
	m_WSpellSkillCode = WSpellSkillCode;
}

void CSlotDelayProc::Update()
{
   SPcItem ** pPInvenItem = nRui->GetPcInvenInfo()->InvenItem;	
 
   m_lNowTime = g_nowTime;

   float dElapseTime = (float)(m_lNowTime - m_lpreTime);

   for(int i = 0 ; i < c_Max_ItemDelayInfo ; i++)
   {
		if(!(nRui->enableDelayTime[i]))
		{
			nRui->cumItemDelayTime[i] += dElapseTime;
			if( nRui->cumItemDelayTime[i] >= nRui->realItemDelayTime[i] )
			{
				nRui->cumItemDelayTime[i] = 0.0f;
				nRui->realItemDelayTime[i] = 0.0f;
				nRui->enableDelayTime[i] = true;
				
				for( int j = 0; j < c_MaxInvenItemNum ; ++j )
				{
					if( pPInvenItem[j] != NULL )
					{
						if( pPInvenItem[j]->IType == n_ITYPE_USE && pPInvenItem[j]->ItemTable)
						{
							if( pPInvenItem[j]->ItemTable->theDelayGroup == i )
							{					
								pPInvenItem[j]->IsDelay = false;		
							}					
						}				
					}			
				}
				
//				for(int pet =0 ; pet < 3 ;++pet)
//				{
//					SPcItem ** pPetPInvenItem = nRui->GetPcInvenInfo()->thePetData[pet]->InvenItem;
//			
//					for( j = 0; j < RP_MAX_PET_ITEM ; ++j )
//					{
//						if( pPetPInvenItem[j] != NULL )
//						{
//							if( pPetPInvenItem[j]->IType == n_ITYPE_PET_USE && pPetPInvenItem[j]->ItemTable)
//							{
//								if( pPetPInvenItem[j]->ItemTable->theDelayGroup == i )
//								{					
//									pPetPInvenItem[j]->IsDelay = false;		
//								}					
//							}				
//						}			
//					}
//				}
			}		
		}

		if(nRui->useCheckDelayTime[i])
		{
			nRui->useCheckCumDelayTime[i] += dElapseTime;
			if( nRui->useCheckCumDelayTime[i] >= 500.0f )	//체크루틴 0.5초후에 체크를 풀어줌
			{
				nRui->useCheckDelayTime[i] = false;
				nRui->useCheckCumDelayTime[i] = 0.0f;
			}
		}
   } 


   if(m_BLoading && !LoadingBarWnd.IsVisible)
   {
		SetUseDelayTime(FALSE, m_pPInvenItem, m_WUsecode, m_iTotCount);	
   }

    if(m_BPetLoading && !LoadingBarWnd.IsVisible)
	{
		for(int pet =0 ; pet < 3 ;++pet)
		{
			SPcItem ** pPetPInvenItem = nRui->GetPcInvenInfo()->thePetData[pet]->InvenItem;
			if( pPetPInvenItem != NULL ) 
			{
				SetPetUseDelayTime(FALSE, m_pPetPInvenItem, m_WPetUsecode);	
			}
		}
	}
  

   //스킬 딜레이 시간누적
   SPcItem ** pSkillItem = nRui->GetPcInvenInfo()->SkillItem;

   for(int i = 0; i < RP_MAX_USER_SKILL ; i++)
   {
      if( ( pSkillItem[i] != NULL) && ( pSkillItem[i]->IType == n_ITYPE_SKILL) )
	  {
	     if( pSkillItem[i]->IsDelay == true)
		 {
			pSkillItem[i]->CumulativeTime += dElapseTime;
		 }
	  }
   }
 
   m_lpreTime = m_lNowTime;
}
void CSlotDelayProc::SetPetUseDelayTime(BOOL BFlag, SPcItem ** pPInvenItem ,WORD _usecode)
{
	/*
	m_BPetLoading = BFlag;
	 m_pPetPInvenItem = pPInvenItem;
 
   if(m_pPInvenItem == NULL)
	   return;

   if(BFlag)
   {
	   m_WPetUsecode = _usecode;
	   return;
   }

   int iDelayGroup = -1;
   float fDelayTime = 0.0f;	
    int i = 0 ;
   for( i = 0 ; i < RP_MAX_PET_ITEM ; ++i )
   {
		if(pPInvenItem[i]  != NULL)
		{
			if( pPInvenItem[i]->IType == n_ITYPE_PET_USE && pPInvenItem[i]->ItemTable)
			{
				if( pPInvenItem[i]->Code == _usecode )
				{					
					iDelayGroup = pPInvenItem[i]->ItemTable->theDelayGroup;
					fDelayTime = pPInvenItem[i]->ItemTable->theDelayTime;
					
					nRui->enableDelayTime[iDelayGroup] = false;
					nRui->realItemDelayTime[iDelayGroup] = fDelayTime * 100.0f;
					nRui->cumItemDelayTime[iDelayGroup] = 0.0f;
					
					break;
				}
			}
		}

   }

   	for( i = 0; i < RP_MAX_PET_ITEM ; ++i )
	{
		if( pPInvenItem[i] != NULL )
		{
			if( pPInvenItem[i]->IType == n_ITYPE_PET_USE && pPInvenItem[i]->ItemTable)
			{
				if( pPInvenItem[i]->ItemTable->theDelayGroup == iDelayGroup )
				{					
				    pPInvenItem[i]->IsDelay = true;		
				}					
			}				
		}			
	}

*/


}


void CSlotDelayProc::SetUseDelayTime(BOOL BFlag, SPcItem ** pPInvenItem ,WORD _usecode,
						 int iTotCount)
{
   m_BLoading = BFlag;
   m_iTotCount = iTotCount;
   m_pPInvenItem = pPInvenItem;
   
   if(m_pPInvenItem == NULL)
	   return;

   if(BFlag)
   {
	   m_WUsecode = _usecode;
	   return;
   }

   int iDelayGroup = -1;
   float fDelayTime = 0.0f;	


   for( int i = 0; i < c_MaxInvenItemNum ; ++i )
   {
		if( pPInvenItem[i] != NULL )
		{
			if( pPInvenItem[i]->IType == n_ITYPE_USE && pPInvenItem[i]->ItemTable)
			{
				if( pPInvenItem[i]->Code == _usecode )
				{					
				   iDelayGroup = pPInvenItem[i]->ItemTable->theDelayGroup;
				   fDelayTime = pPInvenItem[i]->ItemTable->theDelayTime;
				   
				   nRui->enableDelayTime[iDelayGroup] = false;
				   nRui->realItemDelayTime[iDelayGroup] = fDelayTime * 1000.0f;				   
				   nRui->cumItemDelayTime[iDelayGroup] = 0.0f;

				   break;
				}
			}
		}
	}

   	for(int i = 0; i < c_MaxInvenItemNum ; ++i )
	{
		if( pPInvenItem[i] != NULL )
		{
			if( pPInvenItem[i]->IType == n_ITYPE_USE && pPInvenItem[i]->ItemTable)
			{
				if( pPInvenItem[i]->ItemTable->theDelayGroup == iDelayGroup )
				{					
				    pPInvenItem[i]->IsDelay = true;		
				}					
			}				
		}			
	}

}

void CSlotDelayProc::InitiallizeUseDelayTime()
{	
	///-- Null Check
	SPcItem ** pPInvenItem = nRui->GetPcInvenInfo()->InvenItem;

	if(pPInvenItem == NULL)
	{
		return;
	}
	
	for( int i = 0; i < c_MaxInvenItemNum ; ++i )
	{
		if( pPInvenItem[i] != NULL  ) 
		{
			for(int j = 0 ; j < c_Max_ItemDelayInfo; ++j)
			{
				if( pPInvenItem[i]->ItemTable && pPInvenItem[i]->ItemTable->theDelayGroup == j )
					nRui->theItemDalayTime[j] = g_nowTime - 10000000;
			}
		}
	}

	for(int i = 0 ; i < c_Max_ItemDelayInfo ; i++)
	{
		nRui->realItemDelayTime[i] = 0.0f;
		nRui->cumItemDelayTime[i] = 0.0f;
		nRui->enableDelayTime[i] = true;
		nRui->useCheckDelayTime[i] = false;
		nRui->useCheckCumDelayTime[i] = 0.0f;

		for( int j = 0; j < c_MaxInvenItemNum ; ++j )
		{
			if( pPInvenItem[j] != NULL )
			{
				if( pPInvenItem[j]->IType == n_ITYPE_USE && pPInvenItem[j]->ItemTable)
				{
					if( pPInvenItem[j]->ItemTable->theDelayGroup == i )
					{					
						pPInvenItem[j]->IsDelay = false;		
					}					
				}				
			}			
		}
	}
}

void CSlotDelayProc::InitiallizeSkillDelayTime()
{
   CSlot  ** QuickSlot = MainMenuWnd.GetQSlotList();

   if( QuickSlot == NULL )
   {
      return;
   }
	
   for(int i = 0; i < c_MaxMainMenuItemDrawNum ; i++)
   {
      if( (QuickSlot[i]->PcItem != NULL) && (QuickSlot[i]->PcItem->IType == n_ITYPE_SKILL) )
	  {
	     QuickSlot[i]->SetDelayTimeDisplay( false, 10.0f );
		 g_Pc.theSkillSystem.theQuickSlot_DelayTime[i] = false;	  
	  }
   }

   SPcItem ** pSkillItem = nRui->GetPcInvenInfo()->SkillItem;

   for(int i = 0; i < RP_MAX_USER_SKILL ; i++)
   {
      if( ( pSkillItem[i] != NULL) && ( pSkillItem[i]->IType == n_ITYPE_SKILL) )
	  {
	     if( pSkillItem[i]->IsDelay == true)
		 {
			pSkillItem[i]->IsDelay = false;
			pSkillItem[i]->DelayTime = 0.0f;
			pSkillItem[i]->ProgressTime = 0.0f;
			pSkillItem[i]->CumulativeTime = 0.0f;
		 }
	  }
   }

}

void CSlotDelayProc::InitiallizeSkillDelayTime(WORD WSpellSkillCode)
{
   SPcItem ** pSkillItem = nRui->GetPcInvenInfo()->SkillItem;

   for(int i = 0; i < RP_MAX_USER_SKILL ; i++)
   {
      if( ( pSkillItem[i] != NULL) && ( pSkillItem[i]->IType == n_ITYPE_SKILL) )
	  {
	     if( pSkillItem[i]->Code == WSpellSkillCode)
		 {
			pSkillItem[i]->IsDelay = false;
			pSkillItem[i]->DelayTime = 0.0f;
			pSkillItem[i]->ProgressTime = 0.0f;
			pSkillItem[i]->CumulativeTime = 0.0f;
			break;
		 }
	  }
   }

   m_WSpellSkillCode = 0;
}

void CSlotDelayProc::InitDelayTime( SRpdDelayTime * DelayTime , int nDelayTimeNum )
{
	InitiallizeUseDelayTime();
	InitiallizeSkillDelayTime();
	
	if(DelayTime == NULL)
		return;

	SPcItem ** pSkillItem = nRui->GetPcInvenInfo()->SkillItem;
	CSkillSystem*	SkillSystem = g_Pc.GetSkillSystem();
	SPcItem ** pPInvenItem = nRui->GetPcInvenInfo()->InvenItem;

	int i, j;
	i = j = 0;	
	
	for( i = 0 ; i < nDelayTimeNum ; i++)
	{
		SRpdDelayTime * pDelayTime = &(DelayTime[i]);
		
		if( pDelayTime->theType == n_SKILL )			   //스킬일 경우
		{
			for( j = 0 ; j < RP_MAX_USER_SKILL ; j++)
			{
				if( ( pSkillItem[j] != NULL ) && 
					( pSkillItem[j]->IType == n_ITYPE_SKILL ) &&
					( pSkillItem[j]->SkillTable ) )
				{
					if( pSkillItem[j]->Code == pDelayTime->theCode )
					{
						SPcItem* PcItem = pSkillItem[j];
						
						PcItem->IsDelay = true;
						DWORD dDelayTime = PcItem->SkillTable->theDelayTime;
						// 테이블 값은 100ms단위 -> ms단위로 바꿈
						PcItem->DelayTime		=  (float)dDelayTime * 100.0f ; // ms
						PcItem->ProgressTime	=  (float)dDelayTime * 100.0f;

						float fCumTime = PcItem->DelayTime - ((float)(pDelayTime->theDelayTime));
						PcItem->CumulativeTime	= fCumTime;
						
						if( PcItem->CumulativeTime < 0.0f )
						{
							PcItem->CumulativeTime = 0.0f;
						}

						for(int k = 0; k < c_Max_SkillKey_Num ; k++)
						{
							if(SkillSystem->theQuickSlot_SkillCode[k] == pDelayTime->theCode )
							{
								SkillSystem->theQuickSlot_DelayTime[k] = false;
							}
						}

						break;
					}
				}			
			}
		}
		else if(pDelayTime->theType == n_ITEM )				//아이템일경우
		{
		   int iDelayGroup = -1;
		   float fDelayTime = 0.0f;
			
		   SItemBaseInfo* item_Info;
		   item_Info = g_Pc_Manager.GetItemBaseInfoPtr( pDelayTime->theCode ); 
		   if( item_Info == NULL )
		   {
			  continue;
		   }

		   iDelayGroup = item_Info->theDelayGroup;
		   fDelayTime = item_Info->theDelayTime;

		   nRui->enableDelayTime[iDelayGroup] = false;
		   nRui->realItemDelayTime[iDelayGroup] = fDelayTime * 1000.0f;						   
  		   nRui->cumItemDelayTime[iDelayGroup] = 
		   nRui->realItemDelayTime[iDelayGroup] - ((float)(pDelayTime->theDelayTime));

		   if( nRui->cumItemDelayTime[iDelayGroup] < 0.0f )
		   {
     		  nRui->cumItemDelayTime[iDelayGroup] = 0.0f;
		   }
			
		   //같은 딜레이 그룹 추출
		   /*for( j = 0; j < c_MaxInvenItemNum ; ++j )
		   {
				if( pPInvenItem[j] != NULL )
				{
					if( pPInvenItem[j]->IType == n_ITYPE_USE && pPInvenItem[j]->ItemTable)
					{
						if( pPInvenItem[j]->Code == pDelayTime->theCode )
						{					
						   iDelayGroup = pPInvenItem[j]->ItemTable->theDelayGroup;
						   fDelayTime = pPInvenItem[j]->ItemTable->theDelayTime;
				   
						   nRui->enableDelayTime[iDelayGroup] = false;
						   nRui->realItemDelayTime[iDelayGroup] = fDelayTime * 1000.0f;						   
  						   nRui->cumItemDelayTime[iDelayGroup] = 
						   nRui->realItemDelayTime[iDelayGroup] - ((float)(pDelayTime->theDelayTime));

						   if( nRui->cumItemDelayTime[iDelayGroup] < 0.0f )
						   {
								nRui->cumItemDelayTime[iDelayGroup] = 0.0f;
						   }
						   break;
						}
					}
				}
			}*/

		   if(iDelayGroup == -1)
		   {
				continue;
		   }			
			
			for( j = 0; j < c_MaxInvenItemNum ; ++j )
			{
				if( pPInvenItem[j] != NULL )
				{
					if( pPInvenItem[j]->IType == n_ITYPE_USE && pPInvenItem[j]->ItemTable)
					{
						if( pPInvenItem[j]->ItemTable->theDelayGroup == iDelayGroup )
						{					
							pPInvenItem[j]->IsDelay = true;		
						}				
					}				
				}			
			}
		}
	}
}

void CSlotDelayProc::SetUseCheckDelayTime(DWORD theItemID)
{
   SPcItem ** pPInvenItem = nRui->GetPcInvenInfo()->InvenItem;	
	
   for( int i = 0; i < c_MaxInvenItemNum ; ++i )
   {
		if( pPInvenItem[i] != NULL )
		{
			if( pPInvenItem[i]->IType == n_ITYPE_USE && pPInvenItem[i]->ItemTable)
			{
				if( pPInvenItem[i]->ItemUID == theItemID )
				{					
				   nRui->useCheckDelayTime[pPInvenItem[i]->ItemTable->theDelayGroup] = false;
				   nRui->useCheckCumDelayTime[pPInvenItem[i]->ItemTable->theDelayGroup] = 0.0f;
				   break;
				}
			}
		}
	}
}