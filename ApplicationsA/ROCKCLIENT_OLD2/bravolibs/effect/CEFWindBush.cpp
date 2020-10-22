#include "CEFWindBush.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFWindBush::CEFWindBush()
{
	m_IsConEffect = TRUE;
	
	m_InitTime = 0;
	m_PrevTime = 0;
	m_GapTime = 0;

	m_fRadius = 0.0f;

	m_ISDisplay = FALSE;	
}

CEFWindBush::~CEFWindBush()
{

}

void CEFWindBush::Init(void)
{
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	m_fRadius = 56.0f;
	//m_fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );
	
	m_InitTime = g_nowTime;
	m_PrevTime = g_nowTime;
	m_GapTime = 0;
	
	m_ISDamageProcess = TRUE;
	m_ISDisplay = TRUE;
}

int CEFWindBush::Update(float dtime)
{
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_ISDamageProcess )
	{
		///-- 이펙트 시전후.. 1초가 지나면 데미지 처리
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , 1000 ) )
		{
			
			if( m_pExtraInfo != NULL )
			{
				// 1:N 데미지
				int Count = m_pExtraInfo->GetTargetCount();			
				
				for( int i = 0 ; i < Count ; ++i )
				{
					if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
					{
						Info.AfterEffect = EF_WIND_BUSHED;
						Info.DestType = 0;
						Info.lifeTime = 5.1f;
						
						g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );					
						
						g_Particle.SetEffSetCon( 0.0f , EF_WIND_BUSHED , 0 , 5.1f , 
							pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
					}

					Info.Clear();
				}
			}
		
			m_ISDamageProcess = FALSE;
		}
	}
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , 4100 ) )
	{
		SetDestroy();
		return TRUE;
	}	

	return TRUE;
}

int	CEFWindBush::Render(float dtime)
{
	int nRnd = rand()%2;
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_PrevTime ) , >= , m_GapTime ) )
	{
		for(int i = 0; i < 3 ; i++)
		{
			UINT uEffID = EF_WIND_BUSH;
			switch(i)
			{
				case 0:
					{
						uEffID = EF_WIND_BUSH;
						if( nRnd == 0 )
						{
							continue;
						}
					}
					break;
				case 1:
					{
						uEffID = EF_WIND_DUST_BUSH;
					}
					break;
				case 2:
					{
						uEffID = EF_STONE_RAIN;
					}	
					break;
			}			
			
			float fR = Random( -m_fRadius , m_fRadius );
			D3DXVECTOR3  vPos = D3DXVECTOR3( fR , 0.0f , 0.0f );
			D3DXMATRIX   matRot;
			D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
			///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
			D3DXVec3TransformCoord( &vPos , &vPos , &matRot );
			vPos += D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		

			vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );

			g_Particle.SetEffSetCon( 0.0f , uEffID , ESLT_POS , 2.1f , -1 , -1 , vPos, 0.0f );
		}
		
		m_PrevTime = g_nowTime;
		m_GapTime = Random(100, 200);
	}	
	
	return TRUE;
}