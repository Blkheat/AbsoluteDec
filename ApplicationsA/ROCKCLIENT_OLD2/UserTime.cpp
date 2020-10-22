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

//üũ ���� �ð� 
//�ǰ� �÷��̽ð�
const int NormalTime = c_Our * 3 ;
const int abNormalTime = c_Our * 3 ;
const int SomeNormalTime = c_Our * 5;
const int ManyNormalTime = c_Our * 5;

// ���� ���� �ð�
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
	//�������� �޾ƿ� �ð����� �޼���Ÿ�� �缳��... 
	//��� ���� �ȿ� ���ԵǾ�� �ϳ� .. 
	m_Time = dTime;
	
	//���ۼ� �ּ� 3�ð��� �ʱⰪ�̴� .. 
	//���� m_iCurShowTime
	// ���� Ÿ���� 3�ð� �����̳� ? 
	// ������ dTime �� �ð��� ���� �ɼ� �ֱ� ������.. ���� ������ 
	// ��½ð��� �������� �Ѵ�. 
	// ���� ������ ȭ�鿡 ���� ��� ? Ȯ���Ѵ�
	/*. 
	if( dTime <= NormalTime )
	{
		//1�ð��� ���� �޼��� ����϶�
		m_iPreeShowTime = m_iCurShowTime;
		m_iCurShowTime += CheckNormal;
	
	}
	//���� 3�ø� .. 
	else if( dTime == abNormalTime )
	{
		m_iPreeShowTime = m_iCurShowTime;
		m_iCurShowTime += CheckabNormal;
	
	}
	// 3�ð� ���ĺ��� 5�ð� �������� .. 
	else if( (dTime > abNormalTime  && dTime <= SomeNormalTime  )  && dTime >= m_iCurShowTime )
	{
		m_iPreeShowTime = m_iCurShowTime;
		m_iCurShowTime += CheckabNormal;
	
	}
	// 5�ð� ���� 
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
	//3�ð� �̳����� �ѽð� ���� ������ش�. 
	//3�ð� �� �Ǹ� .. 
	//3�ð� ������ ~5�ð� ���� �� 30�� ���� ��� 
	//5�ð� ������ 15�и��� �ѹ��� �˸� .. 

	
	if( !m_Lock ) return;
	
	m_Time += GetSec();

	/*TCHAR Str[256]="";
	sprintf(Str, "������� ����� �ð� %d �޼��� ��½ð� %d" , m_Time , m_iCurShowTime  );
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
	//���� 3�ø� .. 
	else if( m_Time >= abNormalTime &&  m_Time <= abNormalTime + 300 && (!bShowTimeCheck[2]) )
	{
	 	if( (!bShowTimeCheck[2]) )
		{
			bShowTimeCheck[2] = true; 
			WarningMsg(2);
		}
	
	}
	// 3�ð� ���ĺ��� 5�ð� �������� .. 
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
	// 5�ð� ���� 
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