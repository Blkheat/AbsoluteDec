#include "CEFLightStorm.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFLightStorm::CEFLightStorm()
{
	m_IsConEffect = TRUE;
	
	m_InitTime = 0;
	m_PrevTime = 0;
	m_PrevTime2 = 0;
	m_GapTime = 0;

	m_RunningTime = 5100;

	m_fRadius = 0.0f;

	m_ISDisplay = FALSE;	
}

CEFLightStorm::~CEFLightStorm()
{

}

void CEFLightStorm::Init(void)
{
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	m_fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );
	
	m_InitTime = g_nowTime;
	m_PrevTime = g_nowTime;
	m_PrevTime2 = g_nowTime;
	m_GapTime = 0;
	
	m_ISDamageProcess = TRUE;
	m_ISDisplay = TRUE;

	D3DXVECTOR3  vPos = D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );
	vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
	g_Particle.SetEffSetCon( 0.0f , EF_LIGHT_LANDSPARK , ESLT_POS , 1.1f , -1 , -1 , vPos, 0.0f );
}

int CEFLightStorm::Update(float dtime)
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
						Info.AfterEffect = EF_LIGHTNING_ATTACKED2;
						Info.DestType = 0;
						Info.lifeTime = 5.1f;
						
						g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );					
						
						g_Particle.SetEffSetCon( 0.0f , EF_LIGHTNING_ATTACKED2 , 0 , 5.1f , 
							pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
					}

					Info.Clear();
				}
			}
		
			m_ISDamageProcess = FALSE;
		}
	}
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , m_RunningTime ) )
	{
		SetDestroy();
		return TRUE;
	}	
	
	return TRUE;
}

int	CEFLightStorm::Render(float dtime)
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_PrevTime ) , >= , m_GapTime ) )
	{
		for(int i = 0; i < 1 ; i++)
		{
			int j = 0;
			
			for(j = 0; j < 1 ; j++)
			{
				float fR = Random( 0.0f , m_fRadius/ 3.0f );								
				D3DXVECTOR3  vPos = D3DXVECTOR3( fR , 0.0f , 0.0f );
				D3DXMATRIX   matRot;
				D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
				///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
				D3DXVec3TransformCoord( &vPos , &vPos , &matRot );
				vPos += D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		

				vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
				int nRndHeight = Random(-10, 10);
				g_Particle.SetEffSetCon( 0.0f , EF_LIGHT_STORM , ESLT_POS , 2.1f , -1 , -1 , 
					vPos, 0.0f, -1, nRndHeight );
			}

			for(j = 0; j < 4 ; j++)
			{
				float fR = Random( m_fRadius/ 3.0f , (m_fRadius/ 3.0f) * 2.0f );
				
				D3DXVECTOR3  vPos = D3DXVECTOR3( fR , 0.0f , 0.0f );
				D3DXMATRIX   matRot;
				D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
				///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
				D3DXVec3TransformCoord( &vPos , &vPos , &matRot );
				vPos += D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		

				vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
				int nRndHeight = Random(-10, 10);
				g_Particle.SetEffSetCon( 0.0f , EF_LIGHT_STORM_02 , ESLT_POS , 2.1f , -1 , -1 , 
					vPos, 0.0f, -1, nRndHeight );
			}

			for(j = 0; j < 8 ; j++)
			{
				float fR = Random( (m_fRadius/ 3.0f) * 2.0f , m_fRadius );
				
				D3DXVECTOR3  vPos = D3DXVECTOR3( fR , 0.0f , 0.0f );
				D3DXMATRIX   matRot;
				D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
				///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
				D3DXVec3TransformCoord( &vPos , &vPos , &matRot );
				vPos += D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		

				vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
				int nRndHeight = Random(-10, 10);
				g_Particle.SetEffSetCon( 0.0f , EF_LIGHT_STORM_03 , ESLT_POS , 2.1f , -1 , -1 , 
					vPos, 0.0f, -1, nRndHeight );
			}
		}	
		
		m_PrevTime = g_nowTime;
		m_GapTime = Random(70, 100);
	}

	///-- 1.1초 간격마다 EF_LIGHT_LANDSPARK를 다시 그려줌
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_PrevTime2 ) , > , 1100 ) && 
		SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , < , SAFE_TIME_SUB( m_RunningTime , 500 ) ) )
	{
		D3DXVECTOR3  vPos = D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );
		vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );
		g_Particle.SetEffSetCon( 0.0f , EF_LIGHT_LANDSPARK , ESLT_POS , 1.1f , -1 , -1 , vPos, 0.0f );
		
		m_PrevTime2 = g_nowTime;
	}
	
	return TRUE;
}



