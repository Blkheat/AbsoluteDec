// UserTime.cpp: implementation of the UserTime class.
//
//////////////////////////////////////////////////////////////////////




//#include	"RockClient.h"
#include "stringmanager.h"
#include "brother.h"
#include "UserTime.h"
#include <mmsystem.h>
#include "ListTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int c_Sec = 1 ;
const int c_Min = 60 ;
const int c_Our = c_Min*c_Min;

//체크 설정 시간 
//건강 플레이시간
const int NormalTime = c_Our * 3 ;
const int abNormalTime = c_Our * 3 ;
const int SomeNormalTime = c_Our * 5;
const int ManyNormalTime = c_Our * 5;

// 증가 설정 시간
const int CheckNormal = c_Our;
const int CheckabNormal = c_Our/2 ;  //c_Min*30;
const int ChecksomeNormal = CheckabNormal / 2; //c_Min*15;
const int CheckManyNormal = 0;

extern long		 g_nowTime;
UserTime g_UserTime;


UserTime::~UserTime()
{

}


UserTime::UserTime()
{


}

void UserTime::Init()
{
	g_ListTime.Init();
	m_Time = 0 ; 
	m_iCurShowTime  = CheckNormal;
	m_Lock = false ; 
	m_Show = false; 
	m_Cumulative  = 0;
	memset(bShowTimeCheck , NULL , sizeof(BOOL) * 5 );

}

int UserTime::GetCount()
{
	return m_iCurShowTime;
}
void UserTime::SetLock(BOOL Lock)
{
	
	if(!m_Lock)
	{
		Init();
		m_Lock = Lock;
		return;
	}
	
	m_Lock = Lock;
}

void UserTime::SetTime(int dTime)
{
	//서버에서 받아온 시간으로 메세지타임 재설정... 
	//어느 범주 안에 포함되어야 하나 .. 
	m_Time = dTime;
	
	//재작성 최소 3시간이 초기값이니 .. 
	//현재 m_iCurShowTime
	// 현제 타임이 3시간 이전이냐 ? 
	// 들어오는 dTime 이 시간이 변경 될수 있기 때문에.. 값이 들어오면 
	// 출력시간을 재조정을 한다. 
	// 값이 들어오면 화면에 보여 줬냐 ? 확인한다
	/*. 
	if( dTime <= NormalTime )
	{
		//1시간씩 마다 메세지 출력하라
		m_iPreeShowTime = m_iCurShowTime;
		m_iCurShowTime += CheckNormal;
	
	}
	//정각 3시면 .. 
	else if( dTime == abNormalTime )
	{
		m_iPreeShowTime = m_iCurShowTime;
		m_iCurShowTime += CheckabNormal;
	
	}
	// 3시간 이후부터 5시간 이전까지 .. 
	else if( (dTime > abNormalTime  && dTime <= SomeNormalTime  )  && dTime >= m_iCurShowTime )
	{
		m_iPreeShowTime = m_iCurShowTime;
		m_iCurShowTime += CheckabNormal;
	
	}
	// 5시간 이후 
	else if(dTime > m_iCurShowTime  )
	{
		m_iPreeShowTime = m_iCurShowTime;
		m_iCurShowTime += ChecksomeNormal;

	}
*/

}

void UserTime::Update()
{

	g_ListTime.Update();

	m_NowTime = timeGetTime();
	m_Cumulative +=  (m_NowTime - m_CurTime);
	m_CurTime = m_NowTime;
	
	if(GetSec() >= 1)
	{
		CheckTime();
		m_Cumulative -= (GetSec()*1000);
	}
}
int  UserTime::GetSec()
{
	return ((int)m_Cumulative/1000);
}

void UserTime::CheckTime()
{
	//3시간 이내에는 한시간 마다 출력해준다. 
	//3시간 이 되면 .. 
	//3시간 너으면 ~5시간 까지 매 30분 마다 출력 
	//5시간 넘으면 15분마다 한번씩 알림 .. 

	
	if( !m_Lock ) return;
	
	m_Time += GetSec();

	/*TCHAR Str[256]="";
	sprintf(Str, "현재까지 사용한 시간 %d 메세지 출력시간 %d" , m_Time , m_iCurShowTime  );
	SystemPrint( n_DarkRed, n_SystemMsg, Str );
		*/

	if( m_Time  <  NormalTime )
	{
	 
		if(!bShowTimeCheck[1])
		{	
			bShowTimeCheck[1]= true; 
			m_iCurShowTime = CheckNormal ; 
			WarningMsg(1); 
		}
		else if(m_Time > m_iCurShowTime )
		{
			m_iCurShowTime += CheckNormal;
			WarningMsg(1);
			
		}
	}
	//정각 3시면 .. 
	else if( m_Time >= abNormalTime &&  m_Time <= abNormalTime + 300 && (!bShowTimeCheck[2]) )
	{
	 	if( (!bShowTimeCheck[2]) )
		{
			bShowTimeCheck[2] = true; 
			WarningMsg(2);
		}
	
	}
	// 3시간 이후부터 5시간 이전까지 .. 
	else if( (m_Time >= abNormalTime  && m_Time  <= SomeNormalTime  ))
	{
	
		if( !bShowTimeCheck[3] )
		{
			bShowTimeCheck[3] = true; 
			m_iCurShowTime = abNormalTime;
			m_iCurShowTime += CheckabNormal;
			WarningMsg(3); 
		}
		else if(m_Time > m_iCurShowTime )
		{	
			m_iCurShowTime += CheckabNormal;
			WarningMsg(3);
		
		}

	}
	// 5시간 이후 
	else //if(Time > ManyNormalTime  )
	{
		if( !bShowTimeCheck[4] )
		{
			bShowTimeCheck[4] = true; 
			m_iCurShowTime = ManyNormalTime;
			m_iCurShowTime += ChecksomeNormal;
			WarningMsg(3);
		
		}
		else if( m_Time > m_iCurShowTime )
		{
			m_iCurShowTime += ChecksomeNormal;
			WarningMsg(4);
		}	
	
	}


}

void UserTime::WarningMsg(int Num)
{
	switch(Num)
	{
		case 1: SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PLAY_TIME_NORMAL ) , m_Time/c_Our ); break;
		case 2:	SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PLAY_TIME_ABNORMAL ) ); break;
		case 3: SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PLAY_TIME_SOME_ABNORMAL ) ); break;
		case 4: SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PLAY_TIME_MANY_ABNORMAL ) ); break;
	}
		
}