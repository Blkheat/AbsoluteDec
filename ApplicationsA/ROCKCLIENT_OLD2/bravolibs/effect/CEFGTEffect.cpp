#include "CEFGTEffect.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"
	 
CEFGTEffect::CEFGTEffect()
{
	m_IsConEffect = TRUE;
	
	m_nTargetCount = 0;
	m_ppTagetList = NULL;
	m_pEFFECT_EXTRA_INFOList = NULL;
	m_pXRadianList = NULL;
	m_pYRadianList = NULL;
	m_pIsDamageActList = NULL;
	m_pRangeTimeList = NULL;
	m_pmatLocalList = NULL;

	m_uBillType = 0;
	m_uTaleType = 0;
	m_uAttackedEffect = 0;
	m_nSrcBoneNum = 0;
	m_uAttackedType = 0;

	m_InitTime = 0;
}

CEFGTEffect::~CEFGTEffect()
{
	SAFE_DELETE_ARRAY(m_ppTagetList);
	SAFE_DELETE_ARRAY(m_pEFFECT_EXTRA_INFOList);
	SAFE_DELETE_ARRAY(m_pXRadianList);
	SAFE_DELETE_ARRAY(m_pYRadianList);
	SAFE_DELETE_ARRAY(m_pIsDamageActList);
	SAFE_DELETE_ARRAY(m_pRangeTimeList);
	SAFE_DELETE_ARRAY(m_pmatLocalList);
}

int	CEFGTEffect::CheckFrame()
{

	return TRUE;
}

int CEFGTEffect::Render(float dtime)
{
	BOOL bEndFlag = TRUE;
	int i = 0;

	for( i = 0 ; i < m_nTargetCount ; ++i )
	{
		if( !m_pIsDamageActList[i] )
		{			
			bEndFlag = FALSE;
			break;
		}
	}

	if(bEndFlag)
	{
		SetDestroy();
		return TRUE;		
	}

	D3DXMATRIX matIdent;
	D3DXMatrixIdentity( &matIdent );

	for( i = 0 ; i < m_nTargetCount ; ++i )
	{
		if( !m_pIsDamageActList[i] )
		{	
			if(m_uBillType)
			{		
				g_RenderManager.SetTransform( D3DTS_WORLD , &matIdent );
			
				g_RenderManager.SetCullMode( D3DCULL_CCW );
				g_RenderManager.SetZWriteEnable( TRUE );				
				m_LinkBillboard.m_vLoc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_LinkBillboard.m_matPostWorld = m_pmatLocalList[i];

				g_Billboard.DisplayManager( m_LinkBillboard , dtime );	
#ifdef BATCH_RENDER
				m_LinkBillboard.DrawBatchBillboard(TRUE);
#ifdef DIRECT_VERSION_9_MJH
				LPDIRECT3DTEXTURE9	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#else
				LPDIRECT3DTEXTURE8	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
#endif // DIRECT_VERSION_9_MJH
				if(prevBillboardTexture)
				{
					g_RenderManager.SetTexture( 0, prevBillboardTexture );
					g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
					g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
					g_RenderManager.EndBatchPrimitive();
					g_RenderManager.SetBeginBatchRendering(FALSE);
	}
#else	//	BATCH_RENDER
				m_LinkBillboard.DrawBillboard(TRUE);			
#endif	//	BATCH_RENDER
				
			}
			else
			{
				g_RenderManager.SetTransform( D3DTS_WORLD , &m_pmatLocalList[i] );
				g_RenderManager.SetAlphaBlendEnable( FALSE );
				g_RenderManager.SetZWriteEnable( TRUE );
				g_RenderManager.SetZEnable( TRUE );
				g_RenderManager.SetCullMode( D3DCULL_CCW );

				CEFArrow::Render( 0 );				
			}			
			
			if( m_uTaleType && EF_COLONYGTOWER_L3_SKILL != m_ID )
			{
				g_RenderManager.SetTransform( D3DTS_WORLD , &matIdent );
				
				D3DXVECTOR3 vPos = D3DXVECTOR3(m_pmatLocalList[i]._41, m_pmatLocalList[i]._42, m_pmatLocalList[i]._43);

				g_RenderManager.SetCullMode( D3DCULL_NONE );
				g_RenderManager.SetZWriteEnable( FALSE );	
			
				g_Particle.DisplayUpdate( m_uTaleType , vPos );

				g_RenderManager.SetCullMode( D3DCULL_CCW );		
				g_RenderManager.SetZWriteEnable( TRUE );
			}

			if(EF_COLONYGTOWER_L3_SKILL == m_ID)
			{
				g_RenderManager.SetCullMode( D3DCULL_NONE );
				
				D3DXVECTOR3 vDir = g_Camera.m_vDir;
				D3DXVec3Normalize( &vDir , &vDir );

				D3DXVECTOR3 vPos = D3DXVECTOR3(m_pmatLocalList[i]._41, m_pmatLocalList[i]._42, m_pmatLocalList[i]._43);
				
				g_RenderManager.SetTransform( D3DTS_WORLD , &matIdent );				
				g_RenderManager.SetTexture( 0 , g_Particle.m_EffectTextureManager.GetTexture(m_HashCode) );	
				m_Tail.Update( vPos , m_vStartPos , 1.0f , vDir , g_lpDevice );

				g_RenderManager.SetAlphaBlendEnable( TRUE );
				g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
				g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );

				m_Tail.Render( g_lpDevice );
			}
		}
	}
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	
	return TRUE;
}	
						
int CEFGTEffect::Update(float dtime)
{
	int i = 0;

	for( i = 0 ; i < m_nTargetCount ; ++i )
	{
		if( !m_pIsDamageActList[i] )
		{
			if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_ppTagetList[i] ) )
			{
				//공격 주체는 사라지고 대상만 남아있을 경우
				if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) &&
					 g_Particle.m_EffectManager.CheckCharacter( m_ppTagetList[i] ) )
				{
					g_Particle.m_EffectManager.DestTargetHit( m_ppTagetList[i] , &m_pEFFECT_EXTRA_INFOList[i] );
					m_pIsDamageActList[i] = TRUE;
				}				
			}
		}
	}

	BOOL bEndFlag = TRUE;

	for( i = 0 ; i < m_nTargetCount ; ++i )
	{
		if( !m_pIsDamageActList[i] )
		{			
			bEndFlag = FALSE;
			break;
		}
	}

	if(bEndFlag)
	{
		SetDestroy();
		return TRUE;		
	}

	D3DXMATRIX matBone;
	
	for( i = 0 ; i < m_nTargetCount ; ++i )
	{
		if( !m_pIsDamageActList[i] )
		{			
			m_ppTagetList[i]->GetBoneLinkTM( 2 , matBone );
			D3DXVECTOR3 vDestPos( matBone._41 ,
								  matBone._42 ,
								  matBone._43 );

			D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;	
			float Range = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , m_InitTime ) / ( float ) m_pRangeTimeList[i] ) );

			D3DXVECTOR3 vPos      = m_vStartPos + ( vDistance * Range );	
			GetXAndYRadian(m_pXRadianList[i], m_pYRadianList[i], vPos, vDestPos);

			if( Range >= 1.0f ) 
			{
				Range = 1.0f;

				if( m_pExtraInfo != NULL )
					g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_ppTagetList[i] );

				if(m_uAttackedType == ESLT_SRC_RAD)
				{
					g_Particle.SetEffSetCon( 0.0f , m_uAttackedEffect , m_uAttackedType , 2.1f , m_ppTagetList[i]->GetUnique() , 
										 m_ppTagetList[i]->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr ), m_pYRadianList[i] + D3DXToRadian( 180 ) );
				}
				else
				{	
					g_Particle.SetEffSetCon( 0.0f , m_uAttackedEffect , m_uAttackedType , 2.1f , m_ppTagetList[i]->GetUnique() , 
										 m_ppTagetList[i]->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
				}

				m_pEFFECT_EXTRA_INFOList[i].AfterEffect = m_uAttackedEffect;
				m_pEFFECT_EXTRA_INFOList[i].DestType = m_uAttackedType;
				m_pEFFECT_EXTRA_INFOList[i].lifeTime = 2.1f;	
				
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_ppTagetList[i] , &m_pEFFECT_EXTRA_INFOList[i] );
							
				m_pIsDamageActList[i] = TRUE;
				continue;
			}		

			D3DXMATRIX matRotX, matRotY;

			if( m_uBillType )
			{
				D3DXMatrixRotationX( &matRotX , m_pXRadianList[i] + D3DXToRadian( 90 ) );
			}
			else
			{
				D3DXMatrixRotationX( &matRotX , m_pXRadianList[i] );
			}			
			
			D3DXMatrixRotationY( &matRotY , m_pYRadianList[i] );
			D3DXMatrixMultiply( &m_pmatLocalList[i] , &matRotX , &matRotY );
			
			m_pmatLocalList[i]._41 = vPos.x;
			m_pmatLocalList[i]._42 = vPos.y;
			m_pmatLocalList[i]._43 = vPos.z;
		}
	}	
	
	return TRUE;
}

int CEFGTEffect::SetAfterEffect(void)
{
	
	
	return TRUE;
}

void CEFGTEffect::Init(void)
{
	if( m_pExtraInfo == NULL )
	{
		SetDestroy();
		return;
	}
	
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		// 1:N 데미지
		int Count = m_pExtraInfo->GetTargetCount();
		for( int i = 0 ; i < Count ; ++i )
		{
			if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
			{
				g_Particle.m_EffectManager.DestTargetHit(pTempDest , &Info );
			}

			Info.Clear();
		}
		
		SetDestroy();
		return;
	}

	CBillboardData *pData = NULL;
	///-- 연결 모델 빌보드 데이터 가져옴 
	if( m_uBillType && ( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( m_uBillType ) ) ) )
	{
		SetDestroy();
		return;
	}

	if(m_uBillType)
	{
		m_LinkBillboard.MakeLink(pData);
		m_LinkBillboard.m_vLoc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_InitTime = g_nowTime;

	D3DXMATRIX matBone;
	m_pSrcChr->GetBoneLinkTM( m_nSrcBoneNum , matBone, FALSE );
	m_vStartPos = D3DXVECTOR3(matBone._41, matBone._42, matBone._43);

	if( m_pExtraInfo == NULL )
	{
		SetDestroy();
		return;	
	}

	m_nTargetCount = m_pExtraInfo->GetTargetCount();

	if(m_nTargetCount == 0)
	{
		SetDestroy();
		return;
	}

	m_ppTagetList = SAFE_NEW_ARRAY( Character* , m_nTargetCount );	
	m_pEFFECT_EXTRA_INFOList = SAFE_NEW_ARRAY( EFFECT_EXTRA_INFO , m_nTargetCount );	
	m_pXRadianList = SAFE_NEW_ARRAY( float , m_nTargetCount );	
	m_pYRadianList = SAFE_NEW_ARRAY( float , m_nTargetCount ); 	
	m_pIsDamageActList = SAFE_NEW_ARRAY( BOOL , m_nTargetCount );	
	m_pRangeTimeList = SAFE_NEW_ARRAY( DWORD , m_nTargetCount );	
	m_pmatLocalList = SAFE_NEW_ARRAY( D3DXMATRIX , m_nTargetCount );	

	memset(m_ppTagetList, 0, sizeof(Character*) * m_nTargetCount);
	memset(m_pEFFECT_EXTRA_INFOList, 0, sizeof(EFFECT_EXTRA_INFO) * m_nTargetCount);
	memset(m_pXRadianList, 0, sizeof(float) * m_nTargetCount);
	memset(m_pYRadianList, 0, sizeof(float) * m_nTargetCount);
	memset(m_pIsDamageActList, 0, sizeof(BOOL) * m_nTargetCount);
	memset(m_pRangeTimeList, 0, sizeof(DWORD) * m_nTargetCount);
	memset(m_pmatLocalList, 0, sizeof(D3DXMATRIX) * m_nTargetCount);

	int i = 0;	
	
	for( i = 0 ; i < m_nTargetCount ; ++i )
	{
		if( !m_pExtraInfo->GetTargetInfo( &m_ppTagetList[i] , &m_pEFFECT_EXTRA_INFOList[i] , i ) )
		{
			m_pIsDamageActList[i] = TRUE;
		}			
	}

	if(EF_COLONYGTOWER_L3_SKILL == m_ID)
	{
		CStringToCode	HashString( "icerain_01.tga" );
		m_HashCode = HashString.GetHashCode();
		m_Tail.InitDevice( g_lpDevice );
	}

	for( i = 0 ; i < m_nTargetCount ; ++i )
	{
		if( !m_pIsDamageActList[i] )
		{
			m_ppTagetList[i]->GetBoneLinkTM( 2 , matBone );
			D3DXVECTOR3 vDestPos( matBone._41 ,
								  matBone._42 ,
								  matBone._43 );

			if(EF_COLONYGTOWER_L3_SKILL == m_ID)
			{
				m_pRangeTimeList[i] = PositionToTimer( 25 , m_vStartPos , vDestPos );
			}
			else
			{
				m_pRangeTimeList[i] = PositionToTimer( 50 , m_vStartPos , vDestPos );
			}
			
			m_pXRadianList[i] = 0.0f;
			m_pYRadianList[i] = m_pSrcChr->m_radian;
			GetXAndYRadian(m_pXRadianList[i], m_pYRadianList[i], m_vStartPos, vDestPos);
			
			D3DXMATRIX matRotX, matRotY;

			if( m_uBillType )
			{
				D3DXMatrixRotationX( &matRotX , m_pXRadianList[i] + D3DXToRadian( 90 ) );
			}
			else
			{
				D3DXMatrixRotationX( &matRotX , m_pXRadianList[i] );
			}			
			
			D3DXMatrixRotationY( &matRotY , m_pYRadianList[i] );
			D3DXMatrixMultiply( &m_pmatLocalList[i] , &matRotX , &matRotY );

			m_pmatLocalList[i]._41 = m_vStartPos.x;
			m_pmatLocalList[i]._42 = m_vStartPos.y;
			m_pmatLocalList[i]._43 = m_vStartPos.z;
		}			
	}

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

}
