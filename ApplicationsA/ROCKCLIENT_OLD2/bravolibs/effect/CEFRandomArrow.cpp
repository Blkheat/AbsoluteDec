#include "CEFRandomArrow.h"
#include "CEFArrow.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include	<SOUND\Sound.H>
#include "..\\..\RockClient.h"
#include "..\\..\\map.h"
#include "..\\..\\CRenderManager.h"

extern long		 g_nowTime;

CEFRandomArrow::CEFRandomArrow()
{
	m_IsConEffect = TRUE;
	
	m_fRadius = 0.0f;
	
	m_InitTime = 0;
	m_PrevTime = 0;
	m_GapTime = 0;
	
	m_ISDisplay = FALSE;
	m_ISShot = FALSE;
	
	m_fXRadian = 0.0f;
}

CEFRandomArrow::~CEFRandomArrow()
{


}

int	CEFRandomArrow::CheckFrame(void)
{
	if( m_ISShot == TRUE )
	{
		return TRUE;
	}	
	
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
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

	if(m_Src_Ani_Index != m_pSrcChr->m_ani_index || GetSrcAniCount() == 56 )
	{
		m_pSrcChr->m_IsApplyVA = TRUE;
		m_pSrcChr->m_IsNextApplyVA = TRUE;
		m_pSrcChr->BoneRender();		
		//................................................................................	
		// �ߵ� ����
		//................................................................................	
		if( m_pExtraInfo != NULL )
			g_Particle.m_EffectManager.GetSkillImfactSound( m_pExtraInfo->SkillCode , m_pSrcChr );
		
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
		
		m_InitTime = g_nowTime;
		m_PrevTime = g_nowTime;
		m_GapTime = 0;

		// ��ų ����
		m_InitTime = g_nowTime;

		WORD aSkillCode = 0;
		
		if(m_pExtraInfo)
		{
		   aSkillCode = m_pExtraInfo->SkillCode;
		   m_fRadius = g_Particle.m_EffectManager.GetAttackRange( m_pExtraInfo->SkillCode );	   
		}		

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
		m_ISShot = TRUE;	

		float fYRadian = 0.0f;
		m_fXRadian = 0.0f;
		
		GetXAndYRadian(m_fXRadian, fYRadian, vStart, vEnd);
	}	
	
	return TRUE;
}

void CEFRandomArrow::Init()
{
	if( !g_Particle.m_EffectManager.CheckCharacter( m_pSrcChr ) )
	{
		SetDestroy(TRUE);
		return;
	}	
	
	m_Src_Ani_Index = m_pSrcChr->m_ani_index;

		///-- �̰����� ��ų�� ������ �˾ƿ�
	SSkillBaseInfo* skill_Info = NULL;
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

int CEFRandomArrow::Update(float dtime)
{
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;
	
	if( !m_ISShot )
	{
		if( g_Particle.m_EffectManager.CheckDie( m_pSrcChr ) )
		{
			SetDestroy(TRUE);
			return TRUE;
		}
	}
	
	return TRUE;
}

int	CEFRandomArrow::Render(float dtime)
{
	if( m_ISShot )
	{
		D3DXMATRIX		  matLocal;
		D3DXMatrixIdentity( &matLocal );
		D3DXMATRIX		  matIden;
		D3DXMatrixIdentity( &matIden );

		g_RenderManager.SetZEnable( D3DZB_TRUE );	
		g_RenderManager.SetAlphaBlendEnable( TRUE );

		float  Range = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , m_InitTime ) / ( float ) m_RangeTime ) );

		if( Range >= 1.0f )
		{
			Range = 1.0f;
			SetAfterEffect();
			SetDestroy();		
			return TRUE;
		}

		D3DXMATRIX	m_matRotX, m_matRotY;
		
		for(int i = 0; i < 7 ; i++)
		{
			D3DXMatrixRotationX( &m_matRotX , m_fXRadian + D3DXToRadian( -90 ) );
			D3DXMatrixRotationY( &m_matRotY , m_fYRadian[i] );
			D3DXMatrixMultiply( &matLocal , &m_matRotX , &m_matRotY );

			D3DXVECTOR3 vPos;
			D3DXVec3Lerp( &vPos , &m_vStartPos[i] , &m_vEndPos[i] , Range );
			matLocal._41 = vPos.x;
			matLocal._42 = vPos.y;
			matLocal._43 = vPos.z;

			g_RenderManager.SetCullMode( D3DCULL_CCW );
			g_RenderManager.SetZWriteEnable( TRUE );		

			m_Billboard.m_vLoc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_Billboard.m_matPostWorld = matLocal;
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
			
			g_Particle.DisplayUpdate( PT_RANDOMARROW_TAIL , (vPos- (m_vDir[i] * 5.0f)) );
		}

		g_RenderManager.SetCullMode( D3DCULL_NONE );
		g_RenderManager.SetZWriteEnable( FALSE );
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
	
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetZWriteEnable( FALSE );
	g_RenderManager.SetAlphaBlendEnable( FALSE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	
	return TRUE;	
}

int CEFRandomArrow::SetAfterEffect(void)
{
	Character			*pTempDest = NULL;
	EFFECT_EXTRA_INFO    Info;

	if( m_pExtraInfo != NULL )
	{
		// 1:N ������
		int Count = m_pExtraInfo->GetTargetCount();			
		
		for( int i = 0 ; i < Count ; ++i )
		{
			if( m_pExtraInfo->GetTargetInfo( &pTempDest , &Info , i ) )
			{
				Info.AfterEffect = EF_RANDOM_ARROWED;
				Info.DestType = 0;
				Info.lifeTime = 5.1f;
				
				g_Particle.m_EffectManager.EffectTargetHit( m_pSrcChr , pTempDest , &Info );		
				
				g_Particle.SetEffSetCon( 0.0f , EF_RANDOM_ARROWED , 0 , 5.1f , 
										pTempDest->GetUnique() , pTempDest->GetIndex(), 
										::GetCharPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pSrcChr) );			
			}

			Info.Clear();
		}
	}

	return TRUE;
}