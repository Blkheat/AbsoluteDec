// ListTime.cpp: implementation of the ListTime class.
//
//////////////////////////////////////////////////////////////////////

#include "RockInterface\\Rui.h"
#include <windows.h>
#include "ListTime.h"
#include "brother.h"
#include "StringManager.h"
//#include <mmsystem.h>

//#include "RockClient.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ListTime g_ListTime;

ListTime::ListTime()
{

}

ListTime::~ListTime()
{

}
void ListTime::Init()
{
	
	m_CurTime = timeGetTime();
	m_NowTime = timeGetTime();
	m_Cumulative = 0;
	m_MilliSec = 0 ; 

	ZeroMemory(&m_PayMessageTime , sizeof(m_PayMessageTime));
	PayTimeItemList = nRui->GetPcInvenInfo()->PayTimeItemList;

	for(int i = 0 ;  i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i )
	{
		if( PayTimeItemList[i] )
		{
			m_PayMessageTime[i].ItemUID = PayTimeItemList[i]->theItemID ;
			m_PayMessageTime[i].Time = PayTimeItemList[i]->theTime;
		}
		else
		{
			break;
		}
	}

}


void ListTime::Update()
{
	m_NowTime = timeGetTime();
	m_Cumulative +=  m_NowTime - m_CurTime;
	m_CurTime = m_NowTime;

	int i = 0 ; 
	if( GetSec() >= 1 )
	{
		//Todo 남은 시간을 계산하고 싶은변수 m_MilliSec;
		
		//유료버프 사용시 
		for(i = 0 ; i < RP_MAX_PAY_CONTENTS; ++i )
		{
			if( nRui->GetPcInvenInfo()->PayEffect[i] != NULL)
			{
				nRui->GetPcInvenInfo()->PayEffect[i]->theTime -= m_MilliSec;
				if((int)nRui->GetPcInvenInfo()->PayEffect[i]->theTime <= 0  )
				{
					nRui->GetPcInvenInfo()->PayEffect[i]->theTime = 0 ;
				}
			}
		}
		
				
		//유료 아이템 사용시 랩터안장 등. . 
		for(int i =0 ; i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i)
		{
			if( nRui->GetPcInvenInfo()->PayTimeItemList[i] == NULL)
			{
				break;
			}
			else if( PayTimeItemList[i] )
			{
				if(nRui->GetPcInvenInfo()->PayTimeItemList[i]->theTime != 0 )
				{	
					//Todo 추가 코드 
					nRui->GetPcInvenInfo()->PayTimeItemList[i]->theTime -= m_MilliSec;
					if((int)nRui->GetPcInvenInfo()->PayTimeItemList[i]->theTime <= 0  )
					{
						nRui->GetPcInvenInfo()->PayTimeItemList[i]->theTime = 0 ;
					}

					WarningMessage(PayTimeItemList[i]);
				}
			}
		}

		m_Cumulative -= (m_MilliSec*1000);
	}

}

void ListTime::WarningMessage(SPayTimeItem *PayItem)
{

	tm TmpTime = GetTimeTm(PayItem->theTime);
	int i = 0 ; 

	//1일이상 남은 경우 1 일 단위로, 
	if(	TmpTime.tm_mday > 1 &&  TmpTime.tm_mday  < 10 )
	{
		for( i= 0 ;  i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i )
		{

			if( m_PayMessageTime[i].ItemUID == 0 )
			{
				break;
			}

			if(m_PayMessageTime[i].ItemUID != 0 && 	m_PayMessageTime[i].ItemUID == PayItem->theItemID)
			{				
				if( PayItem->theTime < m_PayMessageTime[i].Time )
				{
					m_PayMessageTime[i].Time = PayItem->theTime-86400;

					if((int)m_PayMessageTime[i].Time <= 0  )
					{
						m_PayMessageTime[i].Time = 0;
					}

					SItemBaseInfo* item_Info = NULL;

					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( PayItem->theItemCode );
					
					if(item_Info)
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PAYITEM_USE_TIME_DAY) , item_Info->theName , (int)TmpTime.tm_mday ); 
					}

					break;
				}
			}
			
		}
	}
	//1일 이내인 경우 1시간단위로, 
	else if(  TmpTime.tm_mday < 1 && TmpTime.tm_hour > 1 )
	{
		for( i= 0 ;  i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i )
		{
			if( m_PayMessageTime[i].ItemUID == 0 )
			{
				break;
			}

			if(m_PayMessageTime[i].ItemUID != 0 && 	m_PayMessageTime[i].ItemUID == PayItem->theItemID)
			{				
				if( PayItem->theTime < m_PayMessageTime[i].Time )
				{
					m_PayMessageTime[i].Time = PayItem->theTime-3600;
					if((int)m_PayMessageTime[i].Time <= 0  )
					{
						m_PayMessageTime[i].Time = 0;
					}

					SItemBaseInfo* item_Info = NULL;
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( PayItem->theItemCode );	

					if(item_Info)
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PAYITEM_USE_TIME_HOUR) , item_Info->theName , TmpTime.tm_hour ); 
					}

					break;
				}
			}
		}
	}
  	else if( TmpTime.tm_mday == 0 && TmpTime.tm_hour < 1 )
	{	
		//1시간 이내인 경우 10분 단위로 안내한다.
		for( i= 0 ;  i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i )
		{
			if(m_PayMessageTime[i].ItemUID != 0 && 	m_PayMessageTime[i].ItemUID == PayItem->theItemID)
			{	
				if( m_PayMessageTime[i].ItemUID == 0 )
				{
					break;
				}
				if( PayItem->theTime < m_PayMessageTime[i].Time )
				{
					m_PayMessageTime[i].Time = PayItem->theTime-600;
					if((int)m_PayMessageTime[i].Time <= 0  )
					{
						m_PayMessageTime[i].Time = 0;
					}
					SItemBaseInfo* item_Info = NULL;
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( PayItem->theItemCode );	
					if(item_Info)
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PAYITEM_USE_TIME_MIN)  , item_Info->theName , TmpTime.tm_min); 
					}
					break;
				}
			}
		}
	}
	else 
	{
		return;
	}

}


int ListTime::GetSec()
{

	m_MilliSec = m_Cumulative/1000;

	return m_MilliSec;
}

DWORD ListTime::GetTimePayEffect(DWORD Code)
{

	int i=0;
	for(i = 0 ; i < RP_MAX_PAY_CONTENTS; ++i )
	{
		if( nRui->GetPcInvenInfo()->PayEffect[i] != NULL  && 
			nRui->GetPcInvenInfo()->PayEffect[i]->theItemCode == Code)
		{
			return nRui->GetPcInvenInfo()->PayEffect[i]->theTime;
		}
	}

	return 0;
}


DWORD ListTime::GetTimePayItem(DWORD ItemUID)
{
	for(int i =0 ; i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i)
	{
		if( PayTimeItemList[i] 
			&& PayTimeItemList[i]->theItemID == ItemUID )
		{
			return PayTimeItemList[i]->theTime;
		}
	}
	return 0;
}

void ListTime::DeletePayItem(DWORD ItemUID)	
{
	int i = 0 , j = 0 ; 
	SPayTimeItem tmpPayTimeItemList[RP_MAX_TIMELIMIT_PAY_CONTENTS];
	ZeroMemory(&tmpPayTimeItemList , sizeof(tmpPayTimeItemList));

	for(i =0 ; i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i)
	{
		if( PayTimeItemList[i] && PayTimeItemList[i]->theItemID == ItemUID )
		{
			DelMsgTime(ItemUID);
			SAFE_DELETE(PayTimeItemList[i]);
			break; 
		}
	}

	for(i =0 ; i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i)
	{
		if( PayTimeItemList[i])
		{
			CopyMemory(&tmpPayTimeItemList[j++] , PayTimeItemList[i] , sizeof(SPayTimeItem));
		}
	}	

	for(i =0 ; i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i)
	{
		if( tmpPayTimeItemList[i].theItemCode != 0 )
		{
			if(PayTimeItemList[i] == NULL )
			{
				 PayTimeItemList[i] = SAFE_NEW(SPayTimeItem);
				 CopyMemory(PayTimeItemList[i] , &tmpPayTimeItemList[i]  , sizeof(SPayTimeItem));
			}
			else
			{
				CopyMemory(PayTimeItemList[i] , &tmpPayTimeItemList[i]  , sizeof(SPayTimeItem));
			}
		}
		else 
			break; 

	}


}

tm ListTime::GetTimeTm(DWORD dwTime)
{
	tm Time;
	memset(&Time , NULL, sizeof(tm));

	Time.tm_sec		= (int)dwTime%60;
	Time.tm_min		= ((int)dwTime/60)%60;
	Time.tm_hour	= ((int)dwTime/3600)%24;
	Time.tm_mday	= ((int)dwTime/3600)/24;

	return Time;
}

void ListTime::GetTimeEffectStr(DWORD Code , TCHAR *str)
{
	tm Time = GetTimeTm(GetTimePayEffect(Code)) ;
	
	if (Time.tm_mday != 0  )
	{
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s/%d%s/%d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			Time.tm_mday , G_STRING_CHAR(IDS_HELPTIP_DAY), Time.tm_hour ,G_STRING_CHAR(IDS_WND_CLOCK_HOUR) ,Time.tm_min , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}
	else if(Time.tm_hour != 0  )
	{
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s/%d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			Time.tm_hour ,G_STRING_CHAR(IDS_WND_CLOCK_HOUR) ,Time.tm_min , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}
	else if(Time.tm_min	!= 0 )
	{
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			Time.tm_min , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}
	else 
	{
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			1 , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}

}


void ListTime::GetTimeRemainStr(DWORD ItemUID , TCHAR *str)
{
	tm Time = GetTimeTm(GetTimePayItem(ItemUID)) ;

	if (Time.tm_mday != 0  )
	{
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s/%d%s/%d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			Time.tm_mday , G_STRING_CHAR(IDS_HELPTIP_DAY), Time.tm_hour ,G_STRING_CHAR(IDS_WND_CLOCK_HOUR) ,Time.tm_min , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}
	else if(Time.tm_hour != 0  )
	{
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s/%d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			Time.tm_hour ,G_STRING_CHAR(IDS_WND_CLOCK_HOUR) ,Time.tm_min , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}
	else if(Time.tm_min	!= 0 )
	{
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			Time.tm_min , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}
	else 
	{
		Rsprintf(RWCHAR(str) , _RT("%s : %d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			1 , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
	}

	//else if(Time.tm_sec	)
}
//////////////////////////////////////////////////////////////////////////
int ListTime::FirstShowItem(DWORD dwTime , TCHAR *ItemName)
{

	tm Time = GetTimeTm(dwTime) ;

	if (Time.tm_mday != 0  )
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PAYITEM_USE_TIME_DAY) , ItemName ,Time.tm_mday  ); 
	}
	else if(Time.tm_hour != 0  )
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PAYITEM_USE_TIME_HOUR) , ItemName , Time.tm_hour ); 
	}
	else if(Time.tm_min	!= 0 )
	{
		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_PAYITEM_USE_TIME_MIN)  , ItemName  , Time.tm_min); 
	}

	return TRUE;
}


void ListTime::InsertMessageTime(DWORD ItemUID , DWORD dwTime)
{
	for(int i= 0 ;  i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i )
	{
		if(m_PayMessageTime[i].ItemUID == 0 )
		{
			m_PayMessageTime[i].ItemUID = ItemUID;
			m_PayMessageTime[i].Time = dwTime;
			break;
		}
	}
}

void ListTime::UpdateMessageTime()
{
	/*
	for(int i= 0 ;  i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i )
	{
		if( PayTimeItemList[i])
		{
			for(int j = 0 ; j< RP_MAX_TIMELIMIT_PAY_CONTENTS; ++j)
			{
				if( m_PayMessageTime[j])
				{
					m_PayMessageTime[j].ItmeUID = PayTimeItemList[i].theItemID ;
					
				}
			}
			
		}

	}
*/
}






void ListTime::DelMsgTime(DWORD ItemUID)
{
	MessageTime tmpMsgTime[RP_MAX_TIMELIMIT_PAY_CONTENTS];
	ZeroMemory(&tmpMsgTime , sizeof( tmpMsgTime));
	int i = 0 ,j =0 ;

	for( i = 0 ; i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i)
	{
		if(m_PayMessageTime[i].ItemUID == ItemUID )
		{
			m_PayMessageTime[i].ItemUID =0; 
			m_PayMessageTime[i].Time	=0;
			break; 
		}
	}

	j = 0 ; 
	for( i = 0 ; i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i)
	{
		if(m_PayMessageTime[i].ItemUID != 0 )
		{
			tmpMsgTime[j].ItemUID = m_PayMessageTime[i].ItemUID;
			tmpMsgTime[j].Time = m_PayMessageTime[i].ItemUID ;
			++j;
		}
	}

	for( i = 0 ; i < RP_MAX_TIMELIMIT_PAY_CONTENTS ; ++i)
	{
		if(m_PayMessageTime[i].ItemUID != 0 )
		{
			m_PayMessageTime[i].ItemUID = tmpMsgTime[i].ItemUID ;
			m_PayMessageTime[i].Time  = tmpMsgTime[i].Time ;
		}
	}
}