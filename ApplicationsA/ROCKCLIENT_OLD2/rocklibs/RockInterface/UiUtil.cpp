#include "..\\RockPCH.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\brother.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "..\\..\\Map.h"

#include "..\\..\\PC.h"
#include "..\\..\\Camera.h"
#include "..\\..\\Map.h"
#include "..\\..\\bravolibs\\obj\\Character_Item.H"
#include "..\\..\\bravolibs\\obj\\Player_Manager.H"
#include "..\\..\\bravolibs\\obj\\Line_Vertex.H"

#include "..\\..\\bravolibs\\obj\\player.h"
#include "..\\..\\bravolibs\\obj\\nonplayer.h"

#include "..\\..\\Effect.h"

#include "ChatBoxWndProc.h"
#include "Define.h"
#include "Rui.h"
#include "UiUtil.h"
#include "StoreWndProc.h"
#include "stringmanager.h"
#include "..\\..\\MapDataManager.h"

extern long g_nowTime;

bool IsNonPlayerCode( WORD code )
{
	if(  ( code >= c_NonplayerCode_Min ) && ( code <= c_NonplayerCode_Max )  )
		return true;

	return false;
}

bool IsObjectNPCCode( WORD code )
{
	if(  ( code >= c_ObjectNPCCode_Min ) && ( code <= c_ObjectNPCCode_Max )  )
		return true;

	return false;
}

BOOL  IsSameNationUnit( void * pPtr )
{
	if( pPtr == NULL )
	{
		return FALSE;
	}

	Character * pChar = (Character *)pPtr;

	if( !(pChar->m_bPC) )
	{
		DWORD dNPCCode = g_Map.m_NpcTable[pChar->m_nTableIndex].code;	
		SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

		if(npc_Info)
		{
			if( npc_Info->theGender == n_Millena_Unit )
			{
				if(  g_Pc.m_MainPC.char_info.theCitizen == MILLENA )
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
			else if( npc_Info->theGender == n_Rain_Unit )
			{
				if(  g_Pc.m_MainPC.char_info.theCitizen == MILLENA )
				{
					return FALSE;
				}
				else
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

int  GetNSkillLevel(int nGageValue)
{
	int Level = 0;
	
	// 총 게이지 칸 => 5(1) + 5(2) + 5(3) + 5(4) + 5(5) + 5(Ex) = 30 
	if( nGageValue >= 5 && nGageValue < 10 ) Level = 1;
	else if( nGageValue >= 10 && nGageValue < 15 ) Level = 2;
	else if( nGageValue >= 15 && nGageValue < 20 ) Level = 3;
	else if( nGageValue >= 20 && nGageValue < 25 ) Level = 4;
	else if( nGageValue >= 25 && nGageValue < 30 ) Level = 5;
	else if( nGageValue >= 30 ) Level = 6;	// Ex Level

	return Level;
}

//----------------------------------------------------------------------------
//by simwoosung
//채팅창에 메시지를 찍는다. 
//BOOL bMessWndFlag - ChatMessageWnd창 출력여부
//----------------------------------------------------------------------------
void ChatMessagePrint(TCHAR * tstrMessage, D3DCOLOR tcolor, BOOL bMessWndFlag )
{

}

void ConvertStrInLimitLength(RTCHAR *strTemp, int size)
{
	RTCHAR temp[512] = _RT("");
	
	Rstrcpy( temp , strTemp );
	Rstrcpy( strTemp , _RT(""));

	int iIdx = 0;
	int iLen = Rstrlen( RWCHAR(temp) );

	RTCHAR TCh[3] = {0,};
	RTCHAR Ch[3] = {0,};

	int i = 0;
	for( i = 0 ; i < size ; i++ )
	{
#ifdef UNICODE_ROCK
		iIdx = (int)temp[i];
		TCh[0] = iIdx;
		Rstrcat(strTemp, TCh);
#else
		DetectionDBCS( &i, temp, Ch);
		strcat(strTemp, Ch);
#endif
	}

	if( i < iLen )
	{
		Rstrcat(RWCHAR(strTemp), _RT(".."));
	}
}

///-- 다른나라 국가의 글자로 보이게 만들어 주기
void ConvertOtherConLanguage(RTCHAR *strTemp)
{
	int iIdx = 0;
	int iLen = Rstrlen( strTemp );
	RTCHAR *pChar = (RTCHAR *)strTemp;
	DWORD uVaule = 0;

	for(int i = 0 ; i < iLen ; i++ )
	{
		RTCHAR cValue = (RTCHAR)(*pChar);

#ifdef UNICODE_ROCK



#else		
		if( IsDBCSLeadByteEx( CP_ACP, cValue ) )
		{
			
			uVaule = cValue;
			uVaule += RandomNum(10,20);
			uVaule %= 178;
			*pChar = (RTCHAR)uVaule;						
			i++;
			pChar += 2;		
			continue;
		}
#endif		
		uVaule = cValue;
		uVaule += RandomNum(30,40);
		uVaule %= 178;
		*pChar = (RTCHAR)uVaule;
		pChar++;	
	}	
}

void ConvertTemKeyRestKey(RTCHAR *pString)		///-- `키를 ,로 바꾸어주는 코드
{
	int iIdx = 0;
	int iLen = Rstrlen( pString );
	RTCHAR *pChar = (RTCHAR *)pString;
	DWORD uVaule = 0;

	for(int i = 0 ; i < iLen ; i++ )
	{
		RTCHAR cValue = (RTCHAR)(*pChar);

#ifdef UNICODE_ROCK
		

#else		
		if( IsDBCSLeadByteEx( CP_ACP, cValue ) )
		{
			i++;
			pChar += 2;		
			continue;
		}
#endif		
		if(cValue == '`')
		{
			*pChar = (RTCHAR)',';
		}

		pChar++;	
	}
}


RTCHAR * GetStringReturnInStringW(RTCHAR ch, bool bInch, int nSize, void *pstrIn, void *pstrOut, BOOL IsMarkUp )
{
	RTCHAR wtemp[256] = {0,};
	RTCHAR temps[512] = {0,};
	RTCHAR wtemps[256] ={0,};

	RTCHAR *strIn = RWCHAR(pstrIn);
	RTCHAR *strOut = RWCHAR(pstrOut);

	Rstrcpy(wtemp , strIn);
	int iIdx = 0;
	int iLen = Rstrlen( wtemp );

//	RTCHAR WCH[3] = _RT(" ");
	RTCHAR WCH[3] = {0,};
	RTCHAR WCHTEMP[3] = {0,};
	RTCHAR wchSpace = 32;
	int Count = 0; 
	
	ZeroMemory(WCH , sizeof(RTCHAR)*3);

	int i = 0;
	for( i = 0 ; i < nSize ; i++ )
	{
		iIdx = (int)wtemp[i];

		if(iIdx < 0x0100) 	++Count;
		else Count+=2;
	
		if( IsMarkUp )
		{
			if( iIdx == '<' )
			{
				WCH[0] = iIdx;

				Rstrcat(strOut, WCH);
				if(iIdx < 0x0100) 	++Count;
				else Count+=2;
				i++;
				if( i < iLen )
				{
					iIdx = (int)wtemp[i];
					WCH[0] = iIdx;
					Rstrcat(strOut, WCH);
					continue;
				}
			}
			
			int nNext = i + 1;
			if( nNext < iLen)
			{
				iIdx = (int)wtemp[i];
					WCHTEMP[0] = iIdx;
				if( WCHTEMP[0] == '>' )
				{
			//		Rstrcat(strOut, WCH);				
					Rstrcat(strOut, WCHTEMP);
					if(iIdx < 0x0100) 	++Count;
					else Count+=2;
					i++;
					continue;
				}
			}
		}

		if( iIdx == 0 )
		{
			return NULL;
		}
	
		if((Count > nSize))
		{
			WCH[0] = iIdx;				
			Rstrcat( strOut, WCH );				
			return &(strIn[i + 1]);
		}
		else if(iIdx == ch)
		{
			if(bInch)
			{
				WCH[0] = iIdx;				
				Rstrcat( strOut, WCH );				
			}			
			return &(strIn[i + 1]);
		}
		
		WCH[0] = iIdx;	
	
		if(WCH[0] == wchSpace)
		{
			Rsscanf( &(strIn[i + 1]) , _RT("%s"), wtemps );
			int iTempLen = Rstrlen( wtemps )*2;
			
			if( (Count + 2 ) >= nSize )
			{
				return &(strIn[i + 1]);
			}
			/*
			if( (Count + iTempLen) >= nSize )
			{
				return &(strIn[i + 1]);
			}*/
			
		}

		if( (i == 0) && (iIdx == wchSpace) )
			continue;
		
		Rstrcat( strOut , WCH );
	}
	
	if( nSize < iLen )
	{
		return &(strIn[i + 1]);
	}
	
	return NULL;
}





TCHAR * GetStringReturnInString(char ch, bool bInch, int nSize, TCHAR *strIn, TCHAR *strOut, BOOL IsMarkUp )
{
	TCHAR temp[512] = _T("");
	TCHAR temps[512] = _T("");
	
	_tcscpy(temp, strIn);

	int iIdx = 0;
	int iLen = strlen( (char *)temp );
	

	TCHAR TCh[3] =_T(" ");
	char Ch[3] = "";
	char TempCh[3] = "";
  
	char cSpace = 32;
	int i = 0;
	for( i = 0 ; i < nSize ; i++ )
	{

		
		DetectionDBCS( &i, temp, Ch);

		if( IsMarkUp )
		{
			if( Ch[0] == '<' )
			{
				_tcscat(strOut, Ch);
				i++;
				if( i < iLen )
				{
					DetectionDBCS( &i, temp, Ch);
					_tcscat(strOut, Ch);
					continue;
				}
			}

			int nNext = i + 1;
			if( nNext < iLen)
			{
				DetectionDBCS( &nNext, temp, TempCh);
				if( TempCh[0] == '>' )
				{
					_tcscat(strOut, Ch);				
					_tcscat(strOut, TempCh);
					i++;
					continue;
				}
			}
		}
		
		if(Ch[0] == ch)
		{
			if(bInch)
			{
				_tcscat(strOut, Ch);
			}
			return &strIn[i + 1];
		}
		
		if(Ch[0] == cSpace)
		{
			sscanf(&(strIn[i + 1]), "%s", temps);
			int iTempLen = strlen( (char *)temps );
#if defined(JAPAN_VERSION) || defined(CHINA_VERSION) || defined(TAIWAN_VERSION)  || defined(HONGKONG_VERSION)
			if( iTempLen )
			{
				if( i + 2  >= nSize )
				{
					return &strIn[i + 1];
				}
			}
#else 
			if( (i + iTempLen) >= nSize )
			{
				return &strIn[i + 1];
			}
#endif
			
		/*
			
		*/

		}
		
		if( (i == 0) && (Ch[0] == cSpace) )
			continue;
		
		_tcscat(strOut, Ch);

	}	

	if( nSize < iLen )
	{
		return &strIn[i];
	}
	
	return NULL;
}
	

int DetectionDBCS( int* Count, char* Str, char *Db )
{
   	static char Ch;
	int *Idx = NULL;

    //------------------------------------------------------------
    //첫바이트가 0x80이상이면 DBCS....
    //------------------------------------------------------------
    if( IsDBCSLeadByteEx( CP_ACP, Str[*Count] ) )
    {	//DBCS
	    Db[0] = Str[*Count];  
	    Db[1] = Str[(*Count)+1];
		Db[2] = 0;

		if( strlen( Db ) )
		{
			Idx = (int*)Db;
		}
	    (*Count)++;
    }
    else
    {   //ASCII
	    Ch    = Str[(*Count)];

	    Db[0] = Ch;
	    Db[1] = 0;

	    if( strlen( Db ) )
		{
			Idx = (int*)Db;
		}
    }
    
	if( Idx )
	{
		return( *Idx );
	}
	
	return 0;
}

void GetTCharStartIndexString(TCHAR *In, int sIndex, TCHAR * Out)
{
	int index = 0;

	TCHAR temp[512] = {0,};
	Rstrcpy(RWCHAR(temp), RWCHAR(In));

	int iIdx = 0;
	int iLen = strlen( (char *)temp );

	TCHAR TCh[3] =_T(" ");
	char Ch[3] = "";

	for(int i = 0 ; i < iLen ; i++, index++ )
	{
#ifdef UNICODE_ROCK
		iIdx = (int)temp[i];
		
		TCh[0] = iIdx;		
		
		if( index >= sIndex )
		{
			wcscat(RWCHAR(Out), RWCHAR(TCh));
		}
#else
		DetectionDBCS( &i, temp, Ch);
				
		if( index >= sIndex )
		{
			strcat(Out, Ch);
		}		
#endif
	}
}

void ConvertTimeToString(time_t	tTime,  TCHAR *pStr)
{
	struct tm *tmCur = localtime( &tTime );
	char tmpStr[256] = "";
	char tmpStr2[256] ="";

	Rsprintf( RWCHAR(tmpStr), _RT("%d") , tmCur->tm_year+1900 );					
		
	Rstrncpy(RWCHAR(tmpStr2), RWCHAR(tmpStr) + 2 , 2 );
	Rstrcat( RWCHAR(pStr), RWCHAR(tmpStr2));

	//달 
	if(tmCur->tm_mon+1 < 10)
	{
		Rsprintf( RWCHAR(tmpStr), _RT("/0%d") , tmCur->tm_mon+1);
	}
	else
	{
		Rsprintf( RWCHAR(tmpStr), _RT("/%d") , tmCur->tm_mon+1);
	}

	Rstrcat( RWCHAR(pStr), RWCHAR(tmpStr) );
	//날 
	if(tmCur->tm_mday < 10)
	{
		Rsprintf( RWCHAR(tmpStr), _RT("/0%d") ,tmCur->tm_mday);
	}
	else
	{
		Rsprintf( RWCHAR(tmpStr), _RT("/%d") ,tmCur->tm_mday);
	}
	//시간 
	Rstrcat( RWCHAR(pStr), RWCHAR(tmpStr) );
	Rsprintf( RWCHAR(tmpStr), _RT(" %d:%d)") ,tmCur->tm_hour,tmCur->tm_min);
	Rstrcat(RWCHAR(pStr) , RWCHAR(tmpStr) );
}

void RandomNameMake(TCHAR *pStr, int size)
{
	int i = 0;
	for( i = 0 ; i < size ; i++)
	{
		int iAscii = RandomNum((int)'A',(int)'z');
		if( (90 < iAscii) && (iAscii < 97) )
		{
			i--;		
		}
		else
		{
			pStr[i] = (char)iAscii;
		}
	}	
	pStr[i] = '\0';
}

void  GetAreaStrName(TCHAR * strTemp, UINT uiMapID)
{
	
	Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( G_MAP_STRID( uiMapID ) ) );
	
	return;
}

void GetClassText(int iClass, TCHAR * strTemp)
{
	switch(iClass)
	{
		case Wanderer:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_WANDERER ) );			break;					
		case Stranger:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_STRANGER ) );			break;					
		case Swordman:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_SWORDMAN ) );			break;					
		case Archer:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_ARCHER ) );				break;					
		case Magician:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_MAGICIAN ) );			break;					
		case Shaman:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_SHAMAN ) );				break;					
		case Knight:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_KNIGHT ) );				break;					
		case Mercenary:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_MERCENARY ) );			break;					
		case Slayer:
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_SLAYER ) );				break;					
		case Sniper:
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_SNIPER ) );				break;					
		case Prominas:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_PROMINAS ) );			break;					
		case Priest:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_PRIEST ) );				break;					
		case Holy_Avenger:		
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_HOLYAVENGER ) );			break;					
		case Psyche:			
			Rsprintf( RWCHAR(strTemp), G_STRING_CHAR( IDS_CLASS_PSYCHE ) );				break;						
	}	
}

int	GetPKStepFromPKPoint(DWORD dPKPoint)
{
	if( dPKPoint < 600 )
	{
		return 0;
	}
	else if( 600 <= dPKPoint && dPKPoint < 1000 )
	{
		return 1;	
	}
	else if( 1000 <= dPKPoint && dPKPoint < 1400 )
	{
		return 2;
	}
	else if( 1400 <= dPKPoint && dPKPoint < 1800 )
	{
		return 3;
	}
	else if( 1800 <= dPKPoint && dPKPoint < 2000 )
	{
		return 4;
	}
	else
	{
		return 5;
	}
}

int	GetDecreaseEXPRatioFromPKPoint(DWORD dPKPoint)
{
	if( dPKPoint < 600 )
	{
		return 0;
	}
	else if( 600 <= dPKPoint && dPKPoint < 1000 )
	{
		return 20;	
	}
	else if( 1000 <= dPKPoint && dPKPoint < 1400 )
	{
		return 30;
	}
	else if( 1400 <= dPKPoint && dPKPoint < 1800 )
	{
		return 40;
	}
	else if( 1800 <= dPKPoint && dPKPoint < 2000 )
	{
		return 50;
	}
	else
	{
		return 60;
	}
}

float GetBuyShopRatioFromPKPoint(DWORD dPKPoint)
{
	if( dPKPoint < 600 )
	{
		return 1.0f;
	}
	else if( 600 <= dPKPoint && dPKPoint < 1000 )
	{
		return 1.5f;	
	}
	else if( 1000 <= dPKPoint && dPKPoint < 1400 )
	{
		return 2.0f;	
	}

	return 1.0f;
}

float GetSellShopRatioFromPKPoint(DWORD dPKPoint)
{
	if( dPKPoint < 600 )
	{
		return SELLSTOR_RATIO;
	}
	else if( 600 <= dPKPoint && dPKPoint < 1000 )
	{
		return 0.05f;	
	}
	else if( 1000 <= dPKPoint && dPKPoint < 1400 )
	{
		return 0.01f;	
	}

	return 0.0f;
}

void GetRemainTimeStr( TCHAR * tmpStr , DWORD dReaminTime )
{
	///-- 남은 시간 구하기
	DWORD dTemp = dReaminTime/1000;
	DWORD dHour = dTemp / 3600;

	int nHour = (int)dHour;

	DWORD dMin = SAFE_TIME_SUB( dTemp , (dHour * 3600) );
	dMin = dMin / 60;

	int nMin = (int)dMin;

	DWORD dSec = SAFE_TIME_SUB( SAFE_TIME_SUB( dTemp , (dHour * 3600) ) , (dMin * 60) );

	if(dSec)
	{
		nMin += 1;
	}

	if( nMin == 60 )
	{
		nHour +=1;
		nMin = 0;
	}

	Rsprintf(RWCHAR(tmpStr) , _RT("%s : %d%s %d%s") , G_STRING_CHAR(IDS_HELPTIP_REMAIN_TIME) , 
			nHour ,G_STRING_CHAR(IDS_WND_CLOCK_HOUR) ,nMin , G_STRING_CHAR( IDS_WND_CLOCK_MIN) ); 
}