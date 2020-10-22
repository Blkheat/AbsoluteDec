#include "CEFCommonFlySill.h"

CEFCommonFlySkill::CEFCommonFlySkill()
{
	m_Type[ 0 ] = CommonFlySkill_Effect_Type::NONE;
	m_Type[ 1 ] = CommonFlySkill_Effect_Type::NONE;

	m_EffectNum[ 0 ] = -1;
	m_EffectNum[ 1 ] = -1;

	m_ISRender  = FALSE;
	m_ISSkill   = FALSE;
	
	m_IsAttacked = TRUE;
	m_IsConEffect = TRUE;
}

CEFCommonFlySkill::~CEFCommonFlySkill()
{

}

int CEFCommonFlySkill::Render(float dtime)
{
	if( FALSE == m_ISRender )
		return TRUE;
	
	D3DXMATRIX matLocal;	

	D3DXMatrixIdentity( &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetZEnable( D3DZB_TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );

	for( int i = 0 ; i < 2 ; ++i )
	{
		switch( m_Type[ i ] )
		{
			case CommonFlySkill_Effect_Type::NONE:
			case CommonFlySkill_Effect_Type::PARTICLE_SPOT:  
			case CommonFlySkill_Effect_Type::BILLBOARD_SPOT:
				
				continue;

				break;

			case CommonFlySkill_Effect_Type::BILLBOARD_LINK:
				
#ifdef BATCH_RENDER
				{
					m_Billboard[ i ].DrawBatchBillboard();
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
				}
				
#else	//	BATCH_RENDER
				m_Billboard[ i ].DrawBillboard();
#endif	//	BATCH_RENDER
				

				break;

			case CommonFlySkill_Effect_Type::PARTICLE_LINK:  
				
				m_Particle[ i ].Render();
				
				break;
		}
	}

	g_RenderManager.SetCullMode( D3DCULL_CCW );
	
	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	
	g_RenderManager.SetAlphaBlendEnable( FALSE );


	return TRUE;
}

int CEFCommonFlySkill::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		if( m_ISSkill )
		{
			SetDestroy(TRUE);
		}
		else
		{
			SetDestroy(TRUE, TRUE);
		}

		return TRUE;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );
	
	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
	
	float       Range     = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) ) / ( float ) m_RangeTime ) );

	if( Range > 1.0f ) 
	{
		Range = 1.0f;
	}

	m_vPos = m_vStartPos + ( vDistance * Range ); 
	
	for( int i = 0 ; i < 2 ; ++i )
	{
		switch( m_Type[ i ] )
		{
			case CommonFlySkill_Effect_Type::NONE:
				
				continue;

				break;

			case CommonFlySkill_Effect_Type::BILLBOARD_LINK:
				
				m_Billboard[ i ].m_vLoc = m_vPos;
				g_Billboard.DisplayManager( m_Billboard[ i ] , dtime );

				break;

			case CommonFlySkill_Effect_Type::BILLBOARD_SPOT:
	
				g_Billboard.SetManager( 0.0f , m_EffectNum[ i ] , m_vPos );
				
				break;

			case CommonFlySkill_Effect_Type::PARTICLE_LINK:  
				
				m_Particle[ i ].SetLinkPos( m_vPos );
				m_Particle[ i ].UpdateLink( dtime );
				
				break;

			case CommonFlySkill_Effect_Type::PARTICLE_SPOT:  
				
				g_Particle.DisplayUpdate( m_EffectNum[ i ] , m_vPos );
			
				break;
		}
	}

	//............................................................................................................
	// 파이어볼 캐릭터에 충돌 처리 ( 이펙트 .... T.T )
	//............................................................................................................
	if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( m_RangeTime , m_OldTimer ) ) )
	{
		SetDestroy();
	
		SetAfterEffect();
	}

	return TRUE;
}

int CEFCommonFlySkill::SetAfterEffect(void)
{
	if( m_ISSkill )
	{
		//................................................................................	
		// 어택 사운드
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );
		
		g_Particle.SetEffSetCon( 0.0f , m_AttackedNum , 0 , 5.1f , m_pDestChr->GetUnique() , 
								 m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );
		
		if(m_pExtraInfo)
		{
			m_pExtraInfo->AfterEffect = m_AttackedNum;
			m_pExtraInfo->DestType = 0;
			m_pExtraInfo->lifeTime = 5.1f;
		}
		
		if(m_IsAttacked)
		{
			g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
		}

		if( m_pSrcChr == g_Pc.GetPlayer() )
			g_Particle.RockCamera( ( rand() % 200 ) + 300 );
	}
	else
	{
		g_Particle.m_EffectManager.StandTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );
	}
	

	return TRUE;
}

void CEFCommonFlySkill::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		if( m_ISSkill )
		{
			SetDestroy(TRUE);
		}
		else
		{
			SetDestroy(TRUE, TRUE);
		}

		return;
	}

	D3DXMATRIX matBone;
	m_pDestChr->GetBoneLinkTM( 2 , matBone );

	D3DXVECTOR3 vDestPos( matBone._41 ,
						  matBone._42 ,
						  matBone._43 );

	D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;

	m_RangeTime	  = PositionToTimer( m_Time , m_vStartPos , vDestPos );

	//................................................................................	
	// 발동 사운드
	//................................................................................	
	if( m_pExtraInfo != NULL && m_ISSkill )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );

	m_OldTimer    = g_nowTime;
}

void CEFCommonFlySkill::SetInit(int   FrontNum,CommonFlySkill_Effect_Type FrontType,int TailNum,CommonFlySkill_Effect_Type TailType,
						   	    int AttackedNum,DWORD RangeTime,Character *pSrc,int BoneNum,BOOL ISSKILL, BOOL IsAttacked)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( pSrc ) )
		return;
	
	m_Type[ 0 ]      = FrontType;
	m_Type[ 1 ]      = TailType;

	m_EffectNum[ 0 ] = FrontNum;
	m_EffectNum[ 1 ] = TailNum;
	m_AttackedNum    = AttackedNum;

	m_RangeTime      = RangeTime;
	m_BoneNum	     = BoneNum;
	
	D3DXMATRIX matBone;
	
	pSrc->GetBoneLinkTM( BoneNum , matBone );

	m_vStartPos.x = matBone._41;
	m_vStartPos.y = matBone._42;
	m_vStartPos.z = matBone._43;
	
	m_Time           = RangeTime;

	for( int i = 0; i < 2 ; ++i )
	{
		switch( m_Type[ i ] )
		{
			case CommonFlySkill_Effect_Type::BILLBOARD_LINK:
				{				
					CBillboardData *pData = NULL ;
	
					if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( m_EffectNum[ i ] ) ) )
					{
						m_Type[ i ] = CommonFlySkill_Effect_Type::NONE;
					}		
					else
					{
						m_Billboard[ i ].MakeLink( pData );

						m_ISRender = TRUE;
					}
				}
				
				break;

			case CommonFlySkill_Effect_Type::PARTICLE_LINK:
				{
					CParticleData	*pData = NULL;
					

					if( ( pData = g_Particle.m_ParticleData.GetKeyDataPointer( m_EffectNum[ i ] ) ) == NULL )
					{
						m_Type[ i ] = CommonFlySkill_Effect_Type::NONE;
					}
					else
					{
						m_Particle[ i ].m_pReference	    = pData;
						
						m_Particle[ i ].m_vMoveLocation	= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
						m_Particle[ i ].m_bIsSuppressed	= false;
						m_Particle[ i ].m_bIsCreate		= true;
						m_Particle[ i ].m_lUnique		= -1;
						m_Particle[ i ].m_iIndex		= -1;
						m_Particle[ i ].m_fSize			= 1.0f;
						
						D3DXMatrixIdentity( &m_Particle[ i ].m_LinkMat );

						m_ISRender = TRUE;
					}
	
				}

				break;
		}
	}

	m_ISSkill = ISSKILL;
	m_IsAttacked = IsAttacked;
}
