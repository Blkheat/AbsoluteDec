// PlayerEtcPrc.cpp: implementation of the PlayerEtcPrc class.
//
//////////////////////////////////////////////////////////////////////

#include "PlayerEtcPrc.h"
#include "Rui.h"
#include "..\\..\\RockClient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern long g_nowTime;
PlayerEtcPrc g_PlayerEtcPrc;

PlayerEtcPrc::PlayerEtcPrc()
{
	Init();
}

PlayerEtcPrc::~PlayerEtcPrc()
{
}

void PlayerEtcPrc::Init()
{
	m_bStartCheck= false;
	m_PrevTime = g_nowTime;
}

void PlayerEtcPrc::Update()
{
	//공포를 맞을때 발생한다. 
	PlayerPosRandom();
}



void PlayerEtcPrc::PlayerPosRandom()
{
	//공포를 가 걸렸는지 찾는다. 
	int i=0;
	bool bStartFlag = false;
	SSkillBaseInfo* skill_Info = NULL ;	

	for(i=0; i < RP_MAX_KEEPUP_SKILL; ++i )
	{
		if( nRui->GetPcInven()->KeepupSkill[i] != 0 )
		{
			skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( nRui->GetPcInven()->KeepupSkill[i] ); 
			if( skill_Info->theAddEffect == n_AddEff_Panic)
			{
			//	bStartFlag = true; 
			
				//캐릭터 주변 반경 50이내로 구한다 . 
				//누를때마다 갈수있는지 판단한다.. 
				//실 좌표이고 블럭은 8pixel 이다
				//스타스트 했냐 ?
				int x = g_Pc.GetPlayer()->m_Mov.curt.x;
				int y = g_Pc.GetPlayer()->m_Mov.curt.y; 
				
				//	if(!bStartCheck)
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_PrevTime ) , >= , 1000 ) )
				{
					//float Radius = (unsigned long)rand()%10+1.0f;
					bool CheckBlock = true;
					while(CheckBlock)
					{								
						float RandX = 15*cos(RANDOM_NUM*180.0f);
						float RandY = 15*sin(RANDOM_NUM*180.0f);
						
						int NextX = x + RandX;
						int NextY = y + RandY;
						
						if( g_Map.m_Block[NextY][NextX].GetClear() == true)
						{
							long DestBlock = NextY *g_Pc.GetMapSize() + NextX;
							//내가 원하는 좌표로 마우스 찍은 좌표로 보내기.. 
							g_Pc.GetPlayer()->SetInputedDestCell(DestBlock);
							g_Pc.GetPlayer()->Illusion_CsStartMove( DestBlock , true );	
							m_bStartCheck = true; 
							CheckBlock = false;
						}
						
					}
					m_PrevTime = g_nowTime;
				}
			}
		}
	}
	/*
	if(bStartFlag)	
	{
		//캐릭터 주변 반경 50이내로 구한다 . 
		//누를때마다 갈수있는지 판단한다.. 
		//실 좌표이고 블럭은 8pixel 이다
		//스타스트 했냐 ?
		int x = g_Pc.GetPlayer()->m_Mov.curt.x;
		int y = g_Pc.GetPlayer()->m_Mov.curt.y; 
				
		//	if(!bStartCheck)
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_PrevTime ) , >= , 1000 ) )
		{
			//float Radius = (unsigned long)rand()%10+1.0f;
			bool CheckBlock = true;
			while(CheckBlock)
			{								
				float RandX = 15*cos(RANDOM_NUM*180.0f);
				float RandY = 15*sin(RANDOM_NUM*180.0f);
				
				int NextX = x + RandX;
				int NextY = y + RandY;
				
				if( g_Map.m_Block[NextY][NextX].GetClear() == true)
				{
					long DestBlock = NextY *g_Pc.GetMapSize() + NextX;
					//내가 원하는 좌표로 마우스 찍은 좌표로 보내기.. 
					g_Pc.GetPlayer()->SetInputedDestCell(DestBlock);
					g_Pc.GetPlayer()->Illusion_CsStartMove( DestBlock );	
					m_bStartCheck = true; 
					CheckBlock = false;
				}
				
			}
			m_PrevTime = g_nowTime;
		}
	}
	*/

}