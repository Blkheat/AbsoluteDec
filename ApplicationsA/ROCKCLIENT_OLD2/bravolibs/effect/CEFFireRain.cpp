#include "CEFFireRain.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		g_nowTime;


CEFFireRain::CEFFireRain()
{
	m_IsConEffect = TRUE;
}

CEFFireRain::~CEFFireRain()
{

}

//�ʱ�ȭ ��Ű�� 
void CEFFireRain::Init(void)
{
	//................................................................................	
	// �ߵ� ����
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	//���̺� ��ƼŬ �ʿ� 


	//��ƼŬ ���̺������� ���̾� �̹����� ���̾� ������ �ʿ��ϴ� .. 
	CParticleData *pData = NULL; 

	if((pData = g_Particle.m_ParticleData.GetKeyDataPointer(PT_FIREBALL)) == NULL)
		return;

	m_FireParticle.m_pReference = pData; 
	
	m_FireParticle.m_vMoveLocation	= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_FireParticle.m_bIsSuppressed	= false;
	m_FireParticle.m_bIsCreate		= true;
	m_FireParticle.m_lUnique		= -1;
	m_FireParticle.m_iIndex			= -1;
	m_FireParticle.m_fSize			= 1.5f;			

	// ��ų ����
	m_OldTime    = g_nowTime;
	
	if( m_EffectTargetInfo.vPos.x == 0.0f &&  m_EffectTargetInfo.vPos.z == 0.0f )
	{
		m_EffectDrop.Init( g_Pc.GetPlayer()->GetPosTM()._41  , g_Pc.GetPlayer()->GetPosTM()._43		, 150.0f , 
		D3DXToRadian( -25 ) ,  700 , 1200 , 8 , 500 , g_nowTime , 2000 , TRUE );	
	}
	else 
	{
		WORD aSkillCode = 0;
		float fRadius = 0.0f;
		
		if(m_pExtraInfo)
		{
		   aSkillCode = m_pExtraInfo->SkillCode;
		   fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );
		}

		m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 150.0f , 
		D3DXToRadian( -25 ) , 700 , 1200 , 8 , fRadius , g_nowTime , 2000 , TRUE );	
	}

	//m_EffectDrop.Init( m_EffectTargetInfo.vPos.x , m_EffectTargetInfo.vPos.z , 150.0f , 0.0f , 800 , 1000 , 10 , g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode ) , g_nowTime , 2000 );

	m_ISDamageProcess = TRUE;
		
	//��ƼŬ ��Ʈ���� �ʱ�ȭ 
	D3DXMatrixIdentity( &m_FireParticle.m_LinkMat );
}


int CEFFireRain::Update(float dtime)
{

	if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
	{
		///-- ���� ��ü�� �״´��ص� 2.1���ı����� ������ó���� ���ش�.		
		/*	
		SetDestroy();
		return TRUE;*/
	}

	//�������� ���𰡸� �ð����� ... 
	m_EffectDrop.Update( ( DWORD )g_nowTime );

	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	//������ ó�� . 
	if( m_ISDamageProcess )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 1000 ) )
		{
			if( m_pExtraInfo != NULL )
			{
				// 1:N ������
				int Count = m_pExtraInfo->GetTargetCount();		
				
				for( int i = 0 ; i < Count ; ++i )
				{
					//�������� ������ ���� ó���� ���� �Ծ ������ ���� Ÿ������ .. 
					if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
					{
						
						Info.AfterEffect = EF_FIREBALL_ATTACKED0;
						Info.DestType = 0;
						Info.lifeTime = 2.1f;
						g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info);

						g_Particle.SetEffSetCon( 0.0f , EF_FIREBALL_ATTACKED0 , 0 , 2.1f , 
												 pTempDest->GetUnique() , pTempDest->GetIndex(), 
												 ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );					
												
						if( m_pSrcChr == g_Pc.GetPlayer() )
							g_Particle.RockCamera( (rand()% 200 ) + 300);

					}
					Info.Clear();
				}
			}
			m_ISDamageProcess = FALSE;
		}
	}

	///-- �����ϴ� ��ƾ�� ����... ���ѹݺ��̶� 
	///-- �̷��� ���� ������ �������� ����	
	///-- �ϴ� ������Ŵ By simwoosung
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTime ) , > , 5100 ) )
	{
		m_EffectDrop.SetIsLoop(FALSE);
		return TRUE;
	}

	return TRUE;
}


int CEFFireRain::Render(float dtime)
{
	DWORD dCullMode = g_RenderManager.GetCullMode();
	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend();

	if( m_EffectDrop.GetDataSize() )
	{
		DROP_EFFECT_DATA *pData = NULL;
		D3DXMATRIX		  matLocal;

		D3DXMatrixIdentity( &matLocal );

		m_EffectDrop.StartPosition();	
		
		D3DXMatrixIdentity( &matLocal );
		
		g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
		
		g_RenderManager.SetCullMode( D3DCULL_NONE );
		
		g_RenderManager.SetZWriteEnable( FALSE );
		g_RenderManager.SetZEnable( D3DZB_TRUE );
		
		g_RenderManager.SetAlphaBlendEnable( TRUE );
		g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
		g_RenderManager.SetDestBlend( D3DBLEND_ONE );

		while( ( pData = m_EffectDrop.GetDropData() ) != NULL )
		{			
			if( pData->State == DROPST_DROP	|| 	pData->State == DROPST_END )
			{					
				//�ٴڿ� ���� ������ .. 
				if( pData->State == DROPST_END )
				{
					g_Particle.SetEffSetCon( 0.0f , EF_FOOT_QUAKE0 , 100 , 2.6f , -1 , -1 , pData->vPos );
				}				

				g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );


				m_FireParticle.SetLinkPos( pData->vPos );
				m_FireParticle.UpdateLink( dtime );
				m_FireParticle.Render();

				//���̾ ������ �ް� ���� .. 
				g_Particle.DisplayUpdate( PT_FIREBALL_TAIL , pData->vPos );
			}
			
		}

		g_RenderManager.SetCullMode( D3DCULL_CCW );
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );
		g_RenderManager.SetAlphaBlendEnable( FALSE );

	}
	else
	{
	
		g_RenderManager.SetCullMode( dCullMode );
		g_RenderManager.SetZWriteEnable( dZWriteEnable );
		g_RenderManager.SetZEnable( dZEnable );
		g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
		g_RenderManager.SetSrcBlend( dSrcBlend );
		g_RenderManager.SetDestBlend( dDestBlend );		
		
		SetDestroy();
		return TRUE;
	}

	g_RenderManager.SetCullMode( dCullMode );
	g_RenderManager.SetZWriteEnable( dZWriteEnable );
	g_RenderManager.SetZEnable( dZEnable );
	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );

	return TRUE;
}