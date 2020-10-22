#include "CEFIceField.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFIceField::CEFIceField()
{	
	m_IsConEffect = TRUE;
	
	m_InitTime = 0;
	m_PrevTime = 0;
	m_GapTime = 0;

	m_fRadius = 0.0f;

	m_ISDisplay = FALSE;
}

CEFIceField::~CEFIceField()
{

}

void CEFIceField::Init(void)
{
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	m_fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );
	
	m_InitTime = g_nowTime;
	m_PrevTime = g_nowTime;
	m_GapTime = 0;
	
	m_ISDamageProcess = TRUE;
	m_ISDisplay = TRUE;

	D3DXVECTOR3  vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vPos.x = m_EffectTargetInfo.vPos.x;
	vPos.z = m_EffectTargetInfo.vPos.z;

	g_Particle.SetEffSetCon( 0.0f , EF_ICE_FIELD , ESLT_POS , 4.1f , -1 , -1 , vPos, 0.0f );

	for(int i = 0; i < 20 ; i++)
	{
		float fR = Random( -m_fRadius , m_fRadius );
		D3DXVECTOR3  vPos = D3DXVECTOR3( fR , 0.0f , 0.0f );
		D3DXMATRIX   matRot;
		D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
		///-- �ݰ�Radius �̳��� 360 ȸ���� ��ġ�� ���Ѵ�.	
		D3DXVec3TransformCoord( &vPos , &vPos , &matRot );
		vPos += D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		

		vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );

		g_Particle.SetEffSetCon( 0.0f , EF_ICE_FIELD_SMOG , ESLT_POS , 2.1f , -1 , -1 , vPos, 0.0f );
	}	
}

int CEFIceField::Update(float dtime)
{
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_ISDamageProcess )
	{
		if( m_pExtraInfo != NULL )
		{
			// 1:N ������
			int Count = m_pExtraInfo->GetTargetCount();
			
			for( int i = 0 ; i < Count ; ++i )
			{
				if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
				{
					g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );
				}

				Info.Clear();
			}
		}	
		
		m_ISDamageProcess = FALSE;
	}

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , 5100 ) )
	{
		SetDestroy();
		return TRUE;
	}

	return TRUE;
}

int	CEFIceField::Render(float dtime)
{
	return TRUE;
}



