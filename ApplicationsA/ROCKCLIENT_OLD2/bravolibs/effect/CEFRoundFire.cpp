#include "CEFRoundFire.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFRoundFire::CEFRoundFire()
{
	m_fRadius = 0.0f;
	m_InitTime  = 0;
	m_PrevTime = 0;
	m_RunningTime = 300;	
}

CEFRoundFire::~CEFRoundFire()
{

}

void CEFRoundFire::Init()
{
	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	//파티클 파이볼구조의 파이어 이미지와 파이어 꼬리가 필요하다 .. 
	CParticleData *pData = NULL;
	CParticleData *pData2 = NULL; 

	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(PT_ROUND_FIRE2)) == NULL)
		return;
	
	if((pData2 = g_Particle.m_ParticleData.GetKeyDataPointer(PT_ROUND_FIRE1)) == NULL)
		return;

	m_fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );
	float fMinRadius = 10.0f;	
	
	m_InitTime = g_nowTime;
	m_PrevTime = g_nowTime;
	
	m_ISDamageProcess = TRUE;
	m_ISDisplay = TRUE;

	D3DXVECTOR3 vMaxRad(m_fRadius, 0.0f, 0.0f);
	D3DXVECTOR3 vMinRad(fMinRadius, 0.0f, 0.0f);	

	for(int i = 0 ; i < 18 ; i++ )
	{
		m_FireParticle[0][i].MakeLink(pData2);			
		m_FireParticle[1][i].MakeLink(pData);
		m_FireParticle[0][i].m_IsPosFix = FALSE;
		m_FireParticle[1][i].m_IsPosFix = FALSE;
		
		float fRad = (2 * D3DX_PI) * ( i / 18.0f );
		D3DXMATRIX	matRot;
		D3DXMatrixRotationY( &matRot, fRad );
		D3DXVec3TransformCoord( &m_vStartPos[i] , &vMinRad , &matRot );
		D3DXVec3TransformCoord( &m_vEndPos[i] , &vMaxRad , &matRot );
	}

	D3DXVECTOR3 vPos = D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );
	g_Particle.SetEffSetCon( 0.0f , EF_FIRE_SMASH_LAND , ESLT_POS , 4.1f , -1 , -1 , vPos, 0.0f );

	for(int i = 0; i < 10 ; i++)
	{
		float fR = Random( -m_fRadius , m_fRadius );
		D3DXVECTOR3  vPos = D3DXVECTOR3( fR , 0.0f , 0.0f );
		D3DXMATRIX   matRot;
		D3DXMatrixRotationY( &matRot , D3DXToRadian( Random( 0 , 360 ) ) );
		///-- 반경Radius 이내로 360 회전된 위치를 구한다.	
		D3DXVec3TransformCoord( &vPos , &vPos , &matRot );
		vPos += D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		

		vPos.y = g_Map.PHF_GetHeight( vPos.x , vPos.z );

		g_Particle.SetEffSetCon( 0.0f , EF_FIRE_RANDOM_FIRE , ESLT_POS , 2.1f , -1 , -1 , vPos, 0.0f );
	}

	m_ISDamageProcess = TRUE;
	m_ISDisplay = TRUE;
}

int CEFRoundFire::Update(float dtime)
{
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_ISDamageProcess )
	{	
		if( m_pExtraInfo != NULL )
		{
			// 1:N 데미지
			int Count = m_pExtraInfo->GetTargetCount();			
			
			for( int i = 0 ; i < Count ; ++i )
			{
				if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
				{
					Info.AfterEffect = EF_ROUND_FIRED;
					Info.DestType = 0;
					Info.lifeTime = 5.1f;
					
					g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );					
					
					g_Particle.SetEffSetCon( 0.0f , EF_ROUND_FIRED , 0 , 5.1f , 
						pTempDest->GetUnique() , pTempDest->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				}

				Info.Clear();
			}
		}
	
		m_ISDamageProcess = FALSE;
	}
	
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_InitTime ) , > , m_RunningTime ) )
	{
		SetDestroy();		
		return TRUE;
	}	
	
	return TRUE;
}

int	CEFRoundFire::Render(float dtime)
{
	D3DXMATRIX		  matLocal;
	D3DXMatrixIdentity( &matLocal );		
	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );	
	
	D3DXVECTOR3 vPos;
	float fRange = SAFE_TIME_SUB(g_nowTime , m_InitTime)/(float)m_RunningTime;	
	if(fRange > 1.0f)
	{
	    fRange = 1.0f;
	}

	g_RenderManager.DisableTextureStageState( 1 );

	DWORD dFVF = g_RenderManager.GetFVF();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dLighting = g_RenderManager.GetLighting();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend(); 

	DWORD dColOp = g_RenderManager.GetTextureColorOP( 0 );
	DWORD dColArg1 = g_RenderManager.GetTextureColorArg1( 0 );
	DWORD dColArg2 = g_RenderManager.GetTextureColorArg2( 0 );

	DWORD dAlpOp = g_RenderManager.GetTextureAlphaOP( 0 );
	DWORD dAlpArg1 = g_RenderManager.GetTextureAlphaArg1( 0 );
	DWORD dAlpArg2 = g_RenderManager.GetTextureAlphaArg2( 0 );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	g_RenderManager.SetZWriteEnable( FALSE );

	float fHeight = g_Map.PHF_GetHeight( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z );
	int i = 0;

	for(i = 0 ; i < 18 ; i++ )
	{
		D3DXVec3Lerp( &vPos , &m_vStartPos[i] , &m_vEndPos[i] , fRange );
		vPos += D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		
		vPos.y = fHeight + 5.0f;

		m_FireParticle[0][i].SetLinkPos( vPos );		
		m_FireParticle[0][i].UpdateLink( dtime );
		m_FireParticle[0][i].Render();
	}

	for(i = 0 ; i < 18 ; i++ )
	{
		D3DXVec3Lerp( &vPos , &m_vStartPos[i] , &m_vEndPos[i] , fRange );
		vPos += D3DXVECTOR3( m_EffectTargetInfo.vPos.x , 0.0f , m_EffectTargetInfo.vPos.z );		
		vPos.y = fHeight + 5.0f;

		m_FireParticle[1][i].SetLinkPos( vPos );		
		m_FireParticle[1][i].UpdateLink( dtime );
		m_FireParticle[1][i].Render();
	}

	g_RenderManager.SetFVF( dFVF );
	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
	g_RenderManager.SetCullMode( dCullMode );
	g_RenderManager.SetZEnable( dZEnable );
	g_RenderManager.SetZWriteEnable( dZWriteEnable );
	g_RenderManager.SetLighting( dLighting );
	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );

	g_RenderManager.SetTextureColorOP  ( 0 , dColOp );
	g_RenderManager.SetTextureColorArg1( 0 , dColArg1 );
	g_RenderManager.SetTextureColorArg2( 0 , dColArg2 );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , dAlpOp );
	g_RenderManager.SetTextureAlphaArg1( 0 , dAlpArg1 );
	g_RenderManager.SetTextureAlphaArg2( 0 , dAlpArg2 );
	
	return TRUE;	
}