#include "CEFBowDoubleShot.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"

extern long g_nowTime;

CEFBowDoubleShot::CEFBowDoubleShot()
{
	m_IsConEffect = TRUE;
	m_ISShot = FALSE;
	m_TileRate = 0;
	
	m_dNpcRealFileCode = 0;
}


CEFBowDoubleShot::~CEFBowDoubleShot()
{
}

int	CEFBowDoubleShot::CheckFrame(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return TRUE;
	}
	
	if( m_ISShot == TRUE )
	{
		return TRUE;
	}
	
	if( m_SrcAniFrame < m_pSrcChr->m_ani_frame )
	{
		m_SrcAniFrame = m_pSrcChr->m_ani_frame;	
	}

	if(m_Src_Ani_Index != m_pSrcChr->m_ani_index)
	{
		m_pSrcChr->m_start_blend_frame = 0;
	}

	int nCheckFrame = 56;

	if( m_dNpcRealFileCode )
	{
		if( m_dNpcRealFileCode == 21100019 )
		{
			nCheckFrame = 25;
		}
	}
	
	if( m_Src_Ani_Index != m_pSrcChr->m_ani_index || GetSrcAniCount() == nCheckFrame )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();		
		
		m_ISShot = TRUE;
		m_vStartPos   = D3DXVECTOR3( m_pSrcChr->GetRealLHandTM()._41 , 
									 m_pSrcChr->GetRealLHandTM()._42 , 	
									 m_pSrcChr->GetRealLHandTM()._43   ); 		
		
		
		D3DXMATRIX matBone;
		m_pDestChr->GetBoneLinkTM( 2 , matBone );

		D3DXVECTOR3 vDestPos( matBone._41 ,
							  matBone._42 ,
							  matBone._43 );
		
		D3DXVECTOR3 vDistance = vDestPos - m_vStartPos;
		m_TileRate = (D3DXVec3Length(&vDistance) ) ;
		
		m_TileRate = (m_TileRate*0.0625f)*0.5f;
		
		CBillboardData *pData = NULL;
		///-- ����ȭ�� ��� �� ������ ������ ������ 
		if( NULL == ( pData = g_Billboard.m_BillboardData.GetKeyDataPointer( 24231 ) ) )
		{
			SetDestroy(TRUE);
			return TRUE;
		}

		///-- ������ �������� ��ũ
		m_Billboard.MakeLink( pData );
		m_Billboard.m_vLoc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX matX; 
		D3DXMATRIX matY ; 
		
		D3DXMatrixRotationX (&matX ,D3DXToRadian( -90 ) );
		D3DXMatrixRotationY( &matY , m_pSrcChr->m_radian  );
		D3DXMatrixMultiply(&m_matLocal , &matX , &matY );

		
		
		D3DXVECTOR3	vDir;
		D3DXVec3Normalize(&vDir,&vDistance);
		D3DXVec3Cross(&m_vRightV,&vDir,&D3DXVECTOR3(0.0f,1.0f,0.0f));

		m_matLocal._41 = m_vStartPos.x;
		m_matLocal._42 = m_vStartPos.y;
		m_matLocal._43 = m_vStartPos.z;
		
		m_RangeTime	  = PositionToTimer( 50 , m_vStartPos , vDestPos );
		
		m_OldTimer    = g_nowTime;

		if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
	}

	return TRUE;
}

void CEFBowDoubleShot::Init(void)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{
		SetDestroy(TRUE);
		return;
	}

	m_Src_Ani_Index = m_pSrcChr->m_ani_index;
	
	if( !m_pSrcChr->m_bPC && m_pSrcChr->m_State.nCharacterType != nCharacterTypePlayerNPC )
	{
		m_dNpcRealFileCode = g_Pc_Manager.GetNPCRealFileCode( g_Map.m_NpcTable[ m_pSrcChr->m_nTableIndex ].code );
	}
	
	if( m_dNpcRealFileCode )
	{
		return;
	}

	///-- �̰����� ��ų�� ������ �˾ƿ�
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( m_pSrcChr->m_curt_event.count );
	if( skill_Info != NULL )
	{
		if(skill_Info->theNeedSpelling)
		{
#ifdef SNIPER_MOD_MOTION			
			m_pSrcChr->m_ani_frame = 56 - 10;
#else
			m_pSrcChr->m_ani_frame = 30;
#endif									
		}
	}
}

int	CEFBowDoubleShot::Render(float dtime)
{
	if( m_ISShot )
	{		
				
		D3DXMATRIX matBone;		
		m_pDestChr->GetBoneLinkTM( 2 , matBone );
		D3DXVECTOR3 vDestPos( matBone._41 ,
						      matBone._42 ,
						      matBone._43 );
		
		D3DXVECTOR3 vDistance   = vDestPos - m_vStartPos;
				
		float       Range       = ( ( ( float ) ( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) ) / ( float ) m_RangeTime ) );
		
		float		RangeRadius = D3DXToRadian( 180*m_TileRate ) * Range;

		//............................................................................................................
		// ĳ���Ϳ� �浹 ó�� ( ����Ʈ .... T.T )
		//............................................................................................................
		if( Range >= 1.0f ) 
		{
			Range = 1.0f;
			
			SetAfterEffect();
			SetDestroy();
			
			return TRUE;
		}

		D3DXVECTOR3 vLeftPos;
		D3DXVECTOR3	vRightPos; 
		D3DXVECTOR3	vPos[2];

		vLeftPos = (vDistance*Range) + (m_vRightV * (sinf( RangeRadius)*4) );
		vRightPos= (vDistance*Range) - (m_vRightV * (sinf( RangeRadius)*4) );

		vPos[0] = m_vStartPos + vLeftPos ;  
		vPos[1]	= m_vStartPos + vRightPos;

		D3DXMATRIX	matIden;
		D3DXMatrixIdentity(&matIden);
		
		for(int i = 0 ; i < 2 ; ++i)
		{
		
			m_matLocal._41 = vPos[i].x;
			m_matLocal._42 = vPos[i].y;
			m_matLocal._43 = vPos[i].z;

			g_RenderManager.SetCullMode( D3DCULL_CCW );
			g_RenderManager.SetZWriteEnable( TRUE );		
			
			m_Billboard.m_vLoc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_Billboard.m_matPostWorld = m_matLocal;
			g_Billboard.DisplayManager( m_Billboard , dtime );	
#ifdef BATCH_RENDER
			m_Billboard.DrawBatchBillboard(TRUE);
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
			m_Billboard.DrawBillboard(TRUE);
#endif	//	BATCH_RENDER
			
			
			g_RenderManager.SetTransform( D3DTS_WORLD , &matIden );
			
			g_RenderManager.SetCullMode( D3DCULL_NONE );
			g_RenderManager.SetZWriteEnable( FALSE );

			g_Particle.DisplayUpdate( PT_FOOT_BOW_TAIL , vPos[i] );
		}

	}
	else
	{	
		if(m_pSrcChr->m_pre_ani_number != -1)
		{
			return TRUE;
		}		
		
		// ����Ʈ �������� 1 �̶� ���⼭ ����ش� ����
		g_RenderManager.SetTransform( D3DTS_WORLD, &m_pSrcChr->GetRHandTM() );
								
		g_RenderManager.SetCullMode( D3DCULL_CCW );
		
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable( TRUE );
		
		g_RenderManager.SetAlphaBlendEnable( FALSE );

		g_Particle.m_ArrowDisplay.Render( g_lpDevice );
	}

	return TRUE;
}

int CEFBowDoubleShot::Update(float dtime)
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr , m_pDestChr ) )
	{		
		SetDestroy(TRUE);
		return TRUE;
	}

	if( FALSE == m_ISShot )
	{
		return TRUE;
	}	
	
////////////////////////////////////////////////////////////////////////////////////////////////////
/*

		D3DXVECTOR3 vDir, vStart, vEnd;
		vStart = D3DXVECTOR3(  m_pSrcChr->GetRealLHandTM()._41 , 
			     			   m_pSrcChr->GetRealLHandTM()._42 , 	
							   m_pSrcChr->GetRealLHandTM()._43   ); 
			
		vEnd.x = m_EffectTargetInfo.vPos.x;
		vEnd.z = m_EffectTargetInfo.vPos.z;
		vEnd.y = 0.0f;

		float fDelHeight = vStart.y - g_Map.PHF_GetHeight( vStart.x , vStart.z );
		D3DXVECTOR3 vTemp = vStart;
		vTemp.y = 0.0f;

		vDir = vEnd - vTemp;
		D3DXVec3Normalize(&vDir, &vDir);
		vEnd += (vDir * 20.0f);	

		float fLenght = D3DXVec3Length(&(vEnd - vStart));

		D3DXVECTOR3 vRightV;
		D3DXVECTOR3 vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXVec3Cross(&vRightV, &vUp, &vDir);

		D3DXVECTOR3 vLeft, vRight;

		vLeft = vEnd - (vRightV * m_fRadius);
		vRight = vEnd + (vRightV * m_fRadius);
		
		D3DXVECTOR3 vDistance = vLeft - vTemp;
		D3DXVec3Normalize(&vDistance, &vDistance);
		float fsRadian =   D3DXVec3Dot( &vDistance , &D3DXVECTOR3(0.0f, 0.0f, -1.0f) );
		fsRadian = acos((double)fsRadian);
		if(vDistance.x > 0.0f)
		{
		   fsRadian = (2.0f * D3DX_PI) - fsRadian;
		}
			
		D3DXVECTOR3 vDistance2 = vRight - vTemp;
		D3DXVec3Normalize(&vDistance2, &vDistance2);
		float fDelRadian = D3DXVec3Dot( &vDistance , &vDistance2 );
		fDelRadian = acos((double)fDelRadian);

		D3DXMATRIX matRot;

		for(int i = 0; i < 7 ; i++)
		{
			//������ ������ = ������ + ���ʰ� ������ ���� �� ��  * (�� 7�� / 6.0?)
			float fRadian = fsRadian + fDelRadian * (i / 6.0f);
			//Tops ���ʺ��� - ��ŸƮ ���� 
			D3DXVECTOR3 vTPos = vLeft - vTemp;
			// Y ������ ȸ���ϴµ� �󸶸�ŭ ȸ���Ұ��̳� ? 
			D3DXMatrixRotationY( &matRot , fDelRadian * (i / 6.0f) );
			//������ ���� �ִ��� �����̴� Y�࿭ �� �� ? 
			D3DXVec3TransformCoord( &vTPos , &vTPos , &matRot );
			// ���� �Ÿ� + ��ŸƮ �� ���� End ���ΰ� ? 
			vTPos += vTemp;
			//�� ȭ���� ��Ÿ ������ = ĳ������ ��ŸƮ �����̴� . 
			m_vStartPos[i] = vStart;
			//�� ȭ���� ������ ������ vTPos
			m_vEndPos[i] = vTPos;
			//���� ���̰� + (�ʰ�  + ĳ���� �޼��� ���̸� ���� �����ش� . 
			m_vEndPos[i].y = g_Map.PHF_GetHeight( vEnd.x , vEnd.z ) + fDelHeight;
			//�� ������ �ٽ� ������ ? = ���� �� - ��ŸƮ ���� 
			D3DXVECTOR3 vDir = m_vEndPos[i] - m_vStartPos[i];
			//������ �ٽ� ������ ����� �� ?
			D3DXVec3Normalize(&vDir, &vDir);
			//���� 	+  ������������ ������ *�����ϰ� 
			m_vStartPos[i] += (vDir * 11.0f);
			//w���� ���Ⱚ�� = ���� ��ȭ���� �����̴� .
			m_vDir[i] = vDir;
			// Y�� ������ ���� ����δ� .. ?
			m_fYRadian[i] = fRadian;
		}

		vEnd.y = g_Map.PHF_GetHeight( vEnd.x , vEnd.z ) + fDelHeight;

		m_RangeTime	= PositionToTimer( 50 , vStart , vEnd );

*/



	return TRUE;
}

int CEFBowDoubleShot::SetAfterEffect(void)

{
	if( m_pExtraInfo != NULL )
		g_Particle.m_EffectManager.GetSkillAttackedSound( m_pExtraInfo->SkillCode , m_pDestChr );


	g_Particle.SetEffSetCon( 0.0f , EF_FAST_BOWED , ESLT_CHARPOS , 2.0f , 
		m_pDestChr->GetUnique() , m_pDestChr->GetIndex(), ::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );

	if(m_pExtraInfo)
	{
		m_pExtraInfo->AfterEffect = EF_FAST_BOWED;
		m_pExtraInfo->DestType = ESLT_CHARPOS;
		m_pExtraInfo->lifeTime = 2.0f;
	}
	
	g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , m_pDestChr , m_pExtraInfo );


	return TRUE;
}
