#include "RockPCH.h"
#include "VFileSystem.h"

#include		<NETWORK\\Protocol.h>
#include		"BASE\\D3DBASE.H"
#include		"Billboard.h"
#include		"..\\quadlist.h"
#include		"..\\Pc.h"
#include		"..\\Effect.h"
#include		"..\\Camera.h"
#include		<assert.h>
#include		"EffectVertex.h"
#include		"..\\Rockclient.h"

#include "..\\..\\CRenderManager.h"

extern CRockClient			g_RockClient;

CBillboard::CBillboard() 
{ 
	m_nHeight       = 0;
	
	m_iEffSetCon    = -1; 
	m_iCurLoop      = 0; 
	m_fCurLife      = 0.0f; 
	m_vLoc          = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ); 
	m_vLinkBone		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_fHeight       = 0.0f; 
	m_fDelayTime    = 0.0f;

	m_LoopBlending	= 0;
	m_AddRadian	    = 0.0f;		// 증감되는 각
	m_BoneLink		= 0;
	
	m_pSrc			= NULL;
	m_fSizeX		= 16.0f;
	m_fSizeZ		= 16.0f;

	m_fSetRadian    = 0.0f;

	m_SceneMode		= g_RockClient.m_SceneManager.GetEffectCreateMode();

	m_vAttackSrcDir = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_uSetType		= 0;
	m_pAttackUnit	= NULL;
	m_IsDirAttatack = FALSE;
	m_IsGUnitLinkBoneAndSizeFix = FALSE;

	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixIdentity( &m_matPostWorld );
	D3DXMatrixIdentity( &m_matLocalWorld );
	D3DXMatrixIdentity( &m_mLinkBoneMatrix );

	Timer().Begin();
	
	m_fScale = 1.0f;

	m_IsInitUpdate = FALSE;
	
	m_nPrevUpdateTime = g_nowTime;
	m_nDelUpdateTime = 40;
	m_fDelCumtime = 0.0f;
	m_IsUpdate = FALSE;

	m_nDirectEffType = ESLT_NONE;
	
	m_aDirectLinkBone = 0;

}

CBillboard::~CBillboard()
{
	
}

int	CBillboard::UpdateBillboard( float TimeD )
{
	int		    Index = 0;
	D3DXMATRIX	mat;
	float		height	= 0.0f;
	float		rad		= 0.0f;

	D3DXMatrixIdentity(&mat);
	
	m_Timer.Update(TimeD);

	int nEffSetCon = m_iEffSetCon;
	int nEffType = ESLT_NONE;
	int nEffSetType = -1;
	D3DXVECTOR3 vEffDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fAddSize = 0.0f;

	if( nEffSetCon != -1 )
	{
		nEffType = g_Particle.m_pEffSetCon[ m_iEffSetCon ].m_iType;
		nEffSetType = g_Particle.m_pEffSetCon[ m_iEffSetCon ].m_iSetType;
		vEffDir = g_Particle.m_pEffSetCon[ m_iEffSetCon ].m_vDir;
		fAddSize = g_Particle.m_pEffSetCon[ m_iEffSetCon ].m_fAddSize;
	}

	m_IsUpdate = FALSE;
	
	BOOL IsLinkChr = FALSE;

	if( m_IsInitUpdate )
	{
		if( m_lUnique != -1 )
		{
			///-- 캐릭터에 연결되어 있는 빌보드 는 
			///-- 월드위치정보와 추가 위치정보(로컬 위치정보)를 
			///-- 따로 가지고잇어야 할듯....
			IsLinkChr = TRUE;
		}		
		
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_nPrevUpdateTime ) , > , m_nDelUpdateTime ) )
		{
			m_nPrevUpdateTime = g_nowTime;
			TimeD += m_fDelCumtime;
			m_fDelCumtime = 0.0f;
			m_IsUpdate = TRUE;
		}
		else
		{
			m_fDelCumtime += TimeD;
			m_IsUpdate = FALSE;
			
			if( !IsLinkChr )
			{
				return TRUE;
			}
		}
	}
	else
	{
		m_IsInitUpdate = TRUE;
		m_nPrevUpdateTime = g_nowTime;
		m_IsUpdate = TRUE;
	}
	
	height = 0.0f;
	rad    = 0.0f;
	
	//.........................................................................................................
	// 캐릭터 소스로 찾는 방식
	//.........................................................................................................
	if( nEffType == ESLT_POS )
	{
		rad = m_fSetRadian;
		height = m_nHeight;
	}
	else if( nEffType == ESLT_SRC_RAD )
	{
		rad = m_fSetRadian;
	}
	else if( m_lUnique != -1 )	
	{
		if( m_pSrc != NULL )
		{
			if( m_pSrc->m_nTableIndex == -1 )
			{
				m_iCurLoop = m_pBillboardData->m_iLoop;
				return FALSE;
			}
	
			if( m_pSrc->m_bIsVisible == FALSE )
				return TRUE;	
			
			//long nDelUpdateTime = m_pSrc->m_nDelVATime;
			//m_nDelUpdateTime = max( 40, nDelUpdateTime);

			rad     = m_pSrc->GetDirection();
			
			if( nEffType == ESLT_CPOS_ICAMERA )
			{
				mat._41 = m_pSrc->GetBonCenterTM()._41;
				mat._42 = m_pSrc->GetBonCenterTM()._42;
				mat._43 = m_pSrc->GetBonCenterTM()._43;
			}
			else
			{
				mat		= m_pSrc->GetBonCenterTM();
			}

			height	= ( m_pSrc->GetBoundMaxVec().y - m_pSrc->GetBoundMinVec().y );
			
			if( g_Map.IsGiantUnit( m_pSrc ) )
			{
				if( m_IsDirAttatack ||
					nEffSetType == EF_ATTACK_MISS )
				{
					height	= ( m_pSrc->GetBoundMaxPos().y - m_pSrc->GetBoundMinVec().y );
					//height *= 0.3f;
					height	= 38.0f;					
				}
				else
				{
					height	= ( m_pSrc->GetBoundMaxPos().y - m_pSrc->GetBoundMinVec().y );					
				}
			}					
		}
		else
		{
			m_iCurLoop = m_pBillboardData->m_iLoop;
			return FALSE;
		}
	}
	
	//.........................................................................................................
	if( nEffType == ESLT_SRC_RAD )
	{
		D3DXVECTOR3	TempPos = m_vLoc;
		
		// 위치 수정.
		mat._41	+= TempPos.x;
		mat._42	+= TempPos.y;
		mat._43	+= TempPos.z;

	}
	else if( m_pBillboardData->m_bCharSightRot && 
		     ( m_BoneLink == 0 && m_aDirectLinkBone == 0 ) )
	{
		if( nEffType == ESLT_POS )
		{
			// 위치 수정.
			mat._41	+= m_vLoc.x;
			mat._42	+= m_vLoc.y + height * m_pBillboardData->m_fHeight;
			mat._43	+= m_vLoc.z;			
		}
		else
		{
			D3DXMATRIX matRotY;			
			D3DXMatrixRotationY( &matRotY , rad );			
			D3DXVECTOR3	TempPos;
			D3DXVec3TransformCoord( &TempPos , &m_vLoc , &matRotY );
			
			// 위치 수정. ( 시전시 위치 보정 )
			mat._41	+= TempPos.x;
			mat._42	+= m_vLoc.y;		
			
			if( !m_IsDirAttatack )
			{
				mat._42	+= height * m_pBillboardData->m_fHeight;
			}
			else
			{
				mat._42	+= height;
			}
			
			mat._43	+= TempPos.z;
		}
	}
	else if( ( m_BoneLink != 0 || m_aDirectLinkBone != 0 )
		      && !m_IsDirAttatack ) ///-- 일반적인 ESLT_CHARPOS이고 본링크가 걸릴경우
	{
		D3DXVECTOR3	TempPos = m_vLoc;

		if( nEffType == ESLT_CHARPOS 
			&& !ISZERO(m_fSetRadian) )
		{
			D3DXMATRIX matRotY;			
			D3DXMatrixRotationY( &matRotY , m_fSetRadian );		
			D3DXVec3TransformCoord( &TempPos , &m_vLoc , &matRotY );
		}		
		
		if( nEffType == ESLT_CHARWHOLELINK )
		{
			mat = m_mLinkBoneMatrix;
		}		
		
		mat._41	= TempPos.x + m_vLinkBone.x;
		
		if( height == 0.0f )
			mat._42	= TempPos.y + m_vLinkBone.y;
		else
		{
			mat._42	= TempPos.y + m_vLinkBone.y;			
			mat._42	+= height * m_pBillboardData->m_fHeight;		
		}

		mat._43	= TempPos.z + m_vLinkBone.z;
	}
	else
	{
		D3DXVECTOR3	TempPos = m_vLoc;

		if( nEffType == ESLT_CPOS_ICAMERA )
		{
			D3DXMATRIX matRotY;			
			D3DXMatrixRotationY( &matRotY , -g_Camera.m_fCameraYaw );		
			D3DXVec3TransformCoord( &TempPos , &m_vLoc , &matRotY );
		}	
		
		// 위치 수정.
		mat._41	+= TempPos.x;		
		mat._42	+= TempPos.y;		

		if( !m_IsDirAttatack )
		{
			mat._42	+= height * m_pBillboardData->m_fHeight;
		}
		else
		{
			mat._42	+= height;			
		}

		mat._43	+= TempPos.z;
	}

	///-- 여기까지 월드 위치 구하는 부분

	if( IsLinkChr && !m_IsUpdate )
	{
		if( nEffType == ESLT_CHARWHOLELINK )
		{
			D3DXMatrixMultiply( &m_matWorld, &m_matLocalWorld, &mat );
		}
		else
		{
			m_matWorld = m_matLocalWorld;		
			m_matWorld._41 = m_matLocalWorld._41 + mat._41;
			m_matWorld._42 = m_matLocalWorld._42 + mat._42;
			m_matWorld._43 = m_matLocalWorld._43 + mat._43;
		}

		return TRUE;
	}	
	
	D3DXMATRIX	matworldtemp1, matworldtemp2, matworldtemp3, matworldtemp4, matworldtemp5, matworldtemp6, matworldtemp7, matworldtemp8;
	D3DXMatrixIdentity(&matworldtemp1);
	D3DXMatrixIdentity(&matworldtemp2);
	D3DXMatrixIdentity(&matworldtemp3);
	D3DXMatrixIdentity(&matworldtemp4);
	D3DXMatrixIdentity(&matworldtemp5);
	D3DXMatrixIdentity(&matworldtemp6);
	D3DXMatrixIdentity(&matworldtemp7);
	D3DXMatrixIdentity(&matworldtemp8);

	//y축 회전 매트릭스.
	{
		if( m_pBillboardData->m_bYAxisRot )
		{
			D3DXMatrixRotationY( &matworldtemp1, 
			m_AddRadian + m_pBillboardData->m_fAngleY + 
			(m_fCurLife) * m_pBillboardData->m_fRotVel * (1.0f/m_pBillboardData->m_fLife) * 6.28f);
		}
		else if( m_pBillboardData->m_bXZCamRot || m_pBillboardData->m_bXYZCamRot )
			D3DXMatrixRotationY( &matworldtemp1, -g_Camera.m_fCameraYaw ); // 카메라 쪽으로 회전.
		else if( m_pBillboardData->m_bCharSightRot )
			D3DXMatrixRotationY( &matworldtemp1, rad + m_pBillboardData->m_fAngleY );		// rad 만큼 회전
		else
		{
			if( m_pBillboardData->m_fAngleY != 0.0f )
			{
				D3DXMatrixRotationY( &matworldtemp1, m_pBillboardData->m_fAngleY );		// rad 만큼 회전
			}
		}
	}

	//x축 회전 매트릭스.
	{
		if( m_pBillboardData->m_bXAxisRot )
			D3DXMatrixRotationX( &matworldtemp2, m_AddRadian + m_pBillboardData->m_fAngleX + (m_fCurLife) * m_pBillboardData->m_fRotVel * (1.0f/m_pBillboardData->m_fLife) * 6.28f);
		else if( m_pBillboardData->m_bYZCamRot || m_pBillboardData->m_bXYZCamRot )
		{
			if( !m_pBillboardData->m_bYAxisRot )
				D3DXMatrixRotationX( &matworldtemp2, g_Camera.m_fNewPitch ); // 카메라 쪽으로 회전.
		}
		else
		{
			if( m_pBillboardData->m_fAngleX != 0.0f )
				D3DXMatrixRotationX( &matworldtemp2, m_pBillboardData->m_fAngleX );
		}
	}

	//z축 회전 매트릭스.
	{
		if( m_pBillboardData->m_bZAxisRot )
			D3DXMatrixRotationZ( &matworldtemp3, m_AddRadian + m_pBillboardData->m_fAngleZ + (m_fCurLife) * m_pBillboardData->m_fRotVel * (1.0f/m_pBillboardData->m_fLife) * 6.28f);
		else
		{
			if( m_pBillboardData->m_fAngleZ != 0.0f )
				D3DXMatrixRotationZ( &matworldtemp3, m_pBillboardData->m_fAngleZ );
		}
	}

	if( m_pBillboardData->m_bSizePump )
	{
		if( m_bPumping )
		{
			D3DXMatrixScaling( &matworldtemp4, (m_fCurLife) * (m_vEndSize.x - m_vStartSize.x) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.x
												, (m_fCurLife) * (m_vEndSize.y - m_vStartSize.y) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.y
												, (m_fCurLife) * (m_vEndSize.z - m_vStartSize.z) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.z ); // 라이프에 맞게 스케일링
		}
		else
		{
			D3DXMatrixScaling( &matworldtemp4, (m_pBillboardData->m_fLife - m_fCurLife) * (m_vEndSize.x - m_vStartSize.x) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.x
												, (m_pBillboardData->m_fLife - m_fCurLife) * (m_vEndSize.y - m_vStartSize.y) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.y
												, (m_pBillboardData->m_fLife - m_fCurLife) * (m_vEndSize.z - m_vStartSize.z) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.z ); // 라이프에 맞게 스케일링
		}
	}
	else
	{
		D3DXMatrixScaling( &matworldtemp4, (m_fCurLife) * (m_vEndSize.x - m_vStartSize.x) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.x
									, (m_fCurLife) * (m_vEndSize.y - m_vStartSize.y) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.y
									, (m_fCurLife) * (m_vEndSize.z - m_vStartSize.z) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.z ); // 라이프에 맞게 스케일링
	}

	m_fSizeX = matworldtemp4._11;
	m_fSizeZ = matworldtemp4._33;

	switch( m_pBillboardData->m_iSequence )
	{
	case 0:
		m_matWorld		= matworldtemp4 * (matworldtemp3 * matworldtemp1 * matworldtemp2);
		break;
	case 1:
		m_matWorld		= matworldtemp4 * (matworldtemp1 * matworldtemp3 * matworldtemp2);
		break;
	case 2:
		m_matWorld		= matworldtemp4 * (matworldtemp3 * matworldtemp2 * matworldtemp1);
		break;
	case 3:
		m_matWorld		= matworldtemp4 * (matworldtemp2 * matworldtemp3 * matworldtemp1);
		break;
	case 4:
		m_matWorld		= matworldtemp4 * (matworldtemp1 * matworldtemp2 * matworldtemp3);
		break;
	case 5:
		m_matWorld		= matworldtemp4 * (matworldtemp2 * matworldtemp1 * matworldtemp3);
		break;
	}


	if( m_pBillboardData->m_bCharSightRot )
	{
		if( !m_IsDirAttatack )		
		{
			D3DXMatrixRotationY( &matworldtemp6, rad );
			// 여기서 (0.0f 0.0f -SightLength) 보는 방향 으로 부터 SightLength 떨어 진거리 측정을 위해 
			D3DXMatrixTranslation( &matworldtemp5, 0.0f, 0.0f, (0.0f - m_fRadius) );
			D3DXMatrixMultiply( &matworldtemp5, &matworldtemp5, &matworldtemp6 );		

			m_matWorld._41		= matworldtemp5._41;
			m_matWorld._42		= matworldtemp5._42;
			m_matWorld._43		= matworldtemp5._43;
		}
		else
		{
			D3DXMatrixIdentity(&matworldtemp6);
			D3DXMatrixIdentity(&matworldtemp5);	
		}
	}
	else
	{
		D3DXMATRIX	mTat = mat;
		
		if( nEffType == ESLT_CPOS_ICAMERA )
		{
			D3DXMatrixRotationY( &matworldtemp6, -g_Camera.m_fCameraYaw );			
			
			if(!m_IsDirAttatack)
			{
				D3DXMatrixTranslation( &matworldtemp5, 0.0f, 0.0f, (0.0f - m_fRadius) );
			}
			else
			{
				D3DXMatrixIdentity(&matworldtemp5);
			}

			D3DXMatrixMultiply( &matworldtemp5, &matworldtemp5, &matworldtemp6 );
			D3DXMatrixMultiply( &m_matWorld, &m_matWorld, &matworldtemp5 );		

			matworldtemp5._41 = 0.0f;
			matworldtemp5._42 = 0.0f;
			matworldtemp5._43 = 0.0f;
		}
		else
		{
			if( ( nEffType == ESLT_POS && !ISZERO(rad) ) ||
				( nEffType == ESLT_CHARPOS && !ISZERO(m_fSetRadian) ) )
			{
				D3DXMatrixRotationY( &matworldtemp5, rad );		
				D3DXMatrixMultiply( &m_matWorld, &m_matWorld, &matworldtemp5 );	
			}
			else
			{
				D3DXMatrixRotationY( &matworldtemp6, -g_Camera.m_fCameraYaw );
				D3DXMatrixRotationX(&matworldtemp7, g_Camera.m_fNewPitch);
				// 여기서 (0.0f 0.0f -SightLength) 보는 방향 으로 부터 SightLength 떨어 진거리 측정을 위해 
				
				if(!m_IsDirAttatack)
				{
					D3DXMatrixTranslation( &matworldtemp5, 0.0f, 0.0f, (0.0f - m_fRadius) );
				}
				else
				{
					D3DXMatrixIdentity(&matworldtemp5);
				}

				D3DXMatrixMultiply( &matworldtemp5, &matworldtemp5, &matworldtemp7 );
				D3DXMatrixMultiply( &matworldtemp5, &matworldtemp5, &matworldtemp6 );

				m_matWorld._41		= matworldtemp5._41;
				m_matWorld._42		= matworldtemp5._42;
				m_matWorld._43		= matworldtemp5._43;
			}
		}		
	}

	//......................................................................................................
	// 플레이어 회전에 대해서 이동벡터에 곱해준다 
	//......................................................................................................
	D3DXVECTOR3	 vAddMove;

	if( m_pBillboardData->m_bCharSightRot )
	{
		D3DXVec3TransformCoord( &vAddMove , &m_vMovVec , &matworldtemp5 );
	}
	else
	{
		vAddMove = m_vMovVec;
		
		if(  nEffType == ESLT_CPOS_ICAMERA ||
			( nEffType == ESLT_POS && !ISZERO(rad) ) ||
			( nEffType == ESLT_CHARPOS && !ISZERO(m_fSetRadian) ) )
		{
			D3DXVec3TransformCoord( &vAddMove , &m_vMovVec , &matworldtemp5 );
		}
	}

	if( m_pBillboardData->m_bMovePump )
	{
		if( m_bMoving )
		{
			m_matWorld._41 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * vAddMove.x;
			m_matWorld._42 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * vAddMove.y;
			m_matWorld._43 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * vAddMove.z;
		}
		else
		{
			m_matWorld._41 += (m_pBillboardData->m_fLife - m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * vAddMove.x;
			m_matWorld._42 += (m_pBillboardData->m_fLife - m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * vAddMove.y;
			m_matWorld._43 += (m_pBillboardData->m_fLife - m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * vAddMove.z;
		}
	}
	else
	{
		m_matWorld._41 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * vAddMove.x;
		m_matWorld._42 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * vAddMove.y;
		m_matWorld._43 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * vAddMove.z;
	}
	//......................................................................................................

	m_fScale = 1.0f;
	
	if( !ISZERO(fAddSize) )
	{
		m_fScale += fAddSize;
	}

	if( m_pSrc && m_pSrc->m_nTableIndex != -1 && 
		m_pSrc->m_bIsVisible && ( m_BoneLink == 0 || m_IsDirAttatack )&&
		m_pBillboardData->m_iType != 23 )
	{
		if( g_Map.IsGiantUnit( m_pSrc ) )
		{
			D3DXVECTOR3	vVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			float fSize = (float)g_Map.m_NpcTable[m_pSrc->m_nTableIndex].theNpcSize;
			
			if( m_IsDirAttatack )
			{
				vVec = vEffDir;
				vVec.y = 0.0f;
				D3DXVec3Normalize( &vVec , &vVec );
				vVec *= (fSize * 6.5);
			}
			else if( nEffSetType == EF_ATTACK_MISS )
			{
				D3DXVECTOR3	vDest = g_Camera.m_vEye;
				vDest.y = 0.0f;
				vVec = vDest - D3DXVECTOR3(mat._41, 0.0f, mat._43);	
				D3DXVec3Normalize( &vVec , &vVec );
				vVec *= (fSize * 10);				
			}
			else
			{
				if(!m_IsGUnitLinkBoneAndSizeFix)
				{
					D3DXVECTOR3 vMax = g_Map.m_Npc[m_pSrc->m_nTableIndex].GetBoundMaxVec();
					D3DXVECTOR3 vMin = g_Map.m_Npc[m_pSrc->m_nTableIndex].GetBoundMinVec();
						
					float fWidht = 0.0f;
					float fDel =  vMax.x - vMin.x;
					if( fWidht < fDel )
					{
						fWidht = fDel;
					}
					fDel =  vMax.y - vMin.y;
					if( fWidht < fDel )
					{
						fWidht = fDel;
					}
					fDel =  vMax.z - vMin.z;
					if( fWidht < fDel )
					{
						fWidht = fDel;
					}

					fWidht /= 28.0f;
					fWidht += 0.5f;
					int nSize = (int)fWidht;
					
					m_fScale *= (float)(nSize);
				}
			}
			
			m_matWorld._41 += vVec.x;
			m_matWorld._42 += vVec.y;
			m_matWorld._43 += vVec.z;
		}
	}
	
	m_matLocalWorld = m_matWorld;

	if( nEffType == ESLT_CHARWHOLELINK )
	{
		D3DXMatrixMultiply( &m_matWorld, &m_matWorld, &mat );
	}
	else
	{
		m_matWorld._41 += mat._41;
		m_matWorld._42 += mat._42;
		m_matWorld._43 += mat._43;
	}

	m_Color.r += m_ColorDelta.r * TimeD;
	m_Color.g += m_ColorDelta.g * TimeD;
	m_Color.b += m_ColorDelta.b * TimeD;

	m_fAlpha += m_fAlphaDelta   * TimeD;

	return TRUE;
}


BOOL CBillboard::ChkBatchAble()
{
	switch(m_pBillboardData->m_iType)
	{
	case eBDType_Shield:
	case eBDType_VerticalPlane:
	case eBDType_HorizonPlane:
	case eBDType_VerticalLongPlane:
	case eBDType_HorizonLongPlane:
	case eBDType_Spot:
	case eBDType_Spot1:
	case eBDType_Spot2:
	case eBDType_Sword:
	case eBDType_CrossPlane:
	case eBDType_Cylinder:
	case eBDType_CylinderUp:
	case eBDType_CylinderDown:
	default:	
		return TRUE;
		
//  case eBDType_Hemisphere:
// 	case eBDType_Sphere:
// 	case eBDType_Donut:
// 	case eBDType_DonutUVOne:
// 	case eBDType_Cylinder6:
// 	case eBDType_Cylinder8:
	case 23:
		return FALSE;
	}
	return FALSE;
}

void		CBillboard::DrawBatchBillboard(BOOL bApyPostMat, BOOL bNoDraw)
{
#ifdef BATCH_RENDER
	switch( g_RockClient.m_SceneManager.GetEffectDisplayMode() )	
	{
		// 노멀 모드는 전체 파티클 출력
		case SCENE_NORMAL:
			{
			}
			break;

		// 시퀀드 모드는 시퀀스 모드만 
		case SCENE_SQUENCE:
		
			if( SCENE_SQUENCE != m_SceneMode )
				return;
				
			break;
	}


	D3DXMATRIX matWorld = m_matWorld;
	D3DXMATRIX matScale;

	D3DXMatrixScaling( &matScale, m_fScale, m_fScale, m_fScale );
	
	D3DXMatrixMultiply( &matWorld , &matScale , &matWorld );
	
	if(bApyPostMat)
	{
		D3DXMatrixMultiply( &matWorld , &matWorld , &m_matPostWorld );
	}
	
	D3DXVECTOR3 vPos = D3DXVECTOR3(matWorld._41, matWorld._42, matWorld._43);	
	if( m_pBillboardData->m_iType == 23 )
	{
		vPos.y = g_Map.PHF_GetHeight(vPos.x , vPos.z);
	}
	
	if( g_RockClient.GetGameMode() == GMODE_NORMAL && m_uSetType != EF_PICK && m_uSetType != EF_PICK_X )
	{
		
		vPos.y = 0.0f;
		D3DXVECTOR3 vPlayerPos = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41,
											  0.0f,
											  g_Pc.GetPlayer()->GetPosTM()._43 );
		
		int nPlayerDistance = (int)D3DXVec3Length( &(vPlayerPos - vPos) );			
		
		if( nPlayerDistance > g_Max_Effect_Display )
		{
			if(bNoDraw == FALSE)	///-- 그려야 하는 빌보드라면..
			  return;
		}
	}
	// 특정스킬 이펙트 깨짐현상 수정
#ifdef BATCH_EFFECT_SOLUTION

	if( !m_pBillboardData->m_bTexAni )
		g_RenderManager.SetTexture( 0, m_pBillboardData->GetTexture() );
	else
	{
		if (m_Timer.GetTime() < 0)					return;
		
		CAnimSequence **ppData = g_Anim.m_AniData.GetKeyDataPointer( m_pBillboardData->m_iTexType );
		
		if( ppData != NULL )
		{
			g_RenderManager.SetTexture( 0, (*ppData)->GetCurFrameTexture(m_Timer) );
		}
	}
#endif //BATCH_EFFECT_SOLUTION
	if(bNoDraw == TRUE)
	{
		return;
	}

	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( m_pBillboardData->m_SrcBlend );
	g_RenderManager.SetDestBlend( m_pBillboardData->m_DestBlend );

	BOOL bBatchRending = g_RenderManager.GetBeginBatchRendering();
//	CBillboardData*		prevBillboardData	= g_RenderManager.GetPrevBillboardData();
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
	LPDIRECT3DTEXTURE9	curBillboardTexture = m_pBillboardData->GetTexture();
#else
	LPDIRECT3DTEXTURE8	prevBillboardTexture = g_RenderManager.GetPrevBillboardTexture();
	LPDIRECT3DTEXTURE8	curBillboardTexture = m_pBillboardData->GetTexture();
#endif // DIRECT_VERSION_9_MJH
	int iCurBType = m_pBillboardData->m_iType;
	DWORD dwColor;
	int i;

	if( !m_pBillboardData->m_bTexAni )
	{
//		g_RenderManager.SetTexture( 0, m_pBillboardData->GetTexture() );
		
		if(bBatchRending)
		{
//			if(prevBillboardData)
			if(prevBillboardTexture)
			{
// 				if(prevBillboardData->GetTexture() != m_pBillboardData->GetTexture() 
// 					|| prevBillboardData->m_iType != m_pBillboardData->m_iType)
//				if(prevBillboardData->GetTexture() != m_pBillboardData->GetTexture() )
				if(g_RenderManager.m_PrevHashCode != m_pBillboardData->m_HashCode )
				{
//					g_RenderManager.SetTexture( 0, g_RenderManager.GetPrevBillboardTexture() );
					g_RenderManager.SetTexture( 0, prevBillboardTexture );
					g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
					g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
					g_RenderManager.EndBatchPrimitive();
					if(iCurBType == 23)
					{
						g_RenderManager.SetTexture( 0, curBillboardTexture );
						bBatchRending = FALSE;
					}
					else
						g_RenderManager.BeginBatchPrimitive();
				}
			}
		}
		else if(ChkBatchAble())
		{
			g_RenderManager.BeginBatchPrimitive();
			bBatchRending = TRUE;
		}
		else
		{
			g_RenderManager.SetTexture( 0, curBillboardTexture );
		}

		g_RenderManager.SetBeginBatchRendering(bBatchRending);
//		g_RenderManager.SetPrevBillboardData(m_pBillboardData);
		g_RenderManager.m_PrevSrcBlend	= m_pBillboardData->m_SrcBlend;
		g_RenderManager.m_PrevDestBlend = m_pBillboardData->m_DestBlend;
		g_RenderManager.m_PrevHashCode	= m_pBillboardData->m_HashCode;
		g_RenderManager.SetPrevBillboardTexture(curBillboardTexture);
	}
	else
	{
		if (m_Timer.GetTime() < 0)					return;
		
		if(bBatchRending)
		{
//			g_RenderManager.SetTexture( 0, g_RenderManager.GetPrevBillboardTexture() );
			g_RenderManager.SetTexture( 0, prevBillboardTexture );
			g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
			g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
			g_RenderManager.EndBatchPrimitive();
			g_RenderManager.SetBeginBatchRendering(FALSE);
		}
		
		CAnimSequence **ppData = g_Anim.m_AniData.GetKeyDataPointer( m_pBillboardData->m_iTexType );
		
		if( ppData != NULL )
		{
			g_RenderManager.SetTexture( 0, (*ppData)->GetCurFrameTexture(m_Timer) );
		}
	}

	
	if(bBatchRending)
	{
		dwColor = D3DCOLOR_COLORVALUE( m_Color.r, m_Color.g, m_Color.b, m_fAlpha  );
	}
	
	g_RenderManager.SetTransform( D3DTS_WORLD, &g_Particle.m_matIdentity );

	switch( iCurBType  )
	{
		case eBDType_Shield:
		case eBDType_VerticalPlane:
		case eBDType_HorizonPlane:
		case eBDType_VerticalLongPlane:
		case eBDType_HorizonLongPlane:
		case eBDType_Spot:
		case eBDType_Spot1:
		case eBDType_Spot2:
		case eBDType_Sword:
		case eBDType_CrossPlane:
		case eBDType_Cylinder:
		case eBDType_CylinderUp:
		case eBDType_CylinderDown:
			{
				VERTEX_XYZ_DIFFUSE_TEX1* defalutVertex = g_Billboard.GetDefaultVectexByType(iCurBType);

				for(i = 0; i < g_Billboard.m_pVerticesCount[iCurBType]; i++)
				{
					D3DXVec3TransformCoord( &g_Billboard.m_pVertices[iCurBType][i].v , &defalutVertex[i].v, &matWorld );
					g_Billboard.m_pVertices[iCurBType][i].color = dwColor;
				}
				
				g_RenderManager.AddBatchIndexedVertex(g_Billboard.m_pVertices[iCurBType], 
					g_Billboard.m_pVerticesCount[iCurBType],
					g_Billboard.m_pIndices[iCurBType], 
				g_Billboard.m_pIndicesCount[iCurBType]);
				
// 				g_RenderManager.SetTexture( 0, prevBillboardTexture );
// 				g_RenderManager.SetSrcBlend( g_RenderManager.m_PrevSrcBlend );
// 				g_RenderManager.SetDestBlend( g_RenderManager.m_PrevDestBlend );
// 				g_RenderManager.EndBatchPrimitive();
			}
			break;
		default:
			{
				
				Model **pModel = g_Billboard.m_pExtraModelList.GetKeyDataPointer( m_pBillboardData->m_iType );
				
				if( pModel != NULL )
				{
					for(i = 0; i < (*pModel)->m_header.m_num_vertex; i++)
					{
						D3DXVec3TransformCoord( &(*pModel)->m_pBatchVertex[i].v 
							,&(*pModel)->m_pBatchDefaultVertex[i].v, &matWorld );
						(*pModel)->m_pBatchVertex[i].color = dwColor;
					}
					
					g_RenderManager.AddBatchIndexedVertex((*pModel)->m_pBatchVertex, 
						(*pModel)->m_header.m_num_vertex,
						(*pModel)->m_pIndexList, 
						(*pModel)->m_header.m_num_face*3);
				}
				/*
 				g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );	
				Model **pModel = g_Billboard.m_pExtraModelList.GetKeyDataPointer( m_pBillboardData->m_iType );
 				
 				if( pModel != NULL )
 				{
 					g_RenderManager.SetIndices( (*pModel)->m_pIB , 0 );
 					g_RenderManager.SetStreamSource( 0, (*pModel)->m_pVB, sizeof(OBJECTVERTEX) );
 					g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );
 					
 					g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
 						0, (*pModel)->m_header.m_num_vertex,
 						0, (*pModel)->m_header.m_num_face );
					
				}
				*/
			}
			break;
				
// 		case eBDType_Hemisphere:
// 		case eBDType_Sphere:
// 			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
// 			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
// 			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(VERTEX_XYZ_DIFFUSE_TEX1) );
// 			g_RenderManager.SetFVF( D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1 );
// 			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , SPHERE_VERTEX_COUNT , 0 , SPHERE_VERTEX_COUNT - 2 );
// 			break;
// 
// 		case eBDType_Donut:
// 		case eBDType_DonutUVOne:
// 			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
// 			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
// 			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(VERTEX_XYZ_DIFFUSE_TEX1) );
// 			g_RenderManager.SetFVF( D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1 );
// 			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 62 , 0 , 60 );
// 			break;
// 
// 		case eBDType_Cylinder6:
// 			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
// 			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
// 			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(VERTEX_XYZ_DIFFUSE_TEX1) );
// 			g_RenderManager.SetFVF( D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1 );
// 			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 14 , 0 , 12 );
// 			break;
// 
// 		case eBDType_Cylinder8:
// 			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
// 			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
// 			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(VERTEX_XYZ_DIFFUSE_TEX1) );
// 			g_RenderManager.SetFVF( D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1 );
// 			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 18 , 0 , 16 );
// 			break;
// 	
		case 23:

			g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_CLAMP );
			g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_CLAMP );
			g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_CLAMP );

			g_RenderManager.SetTextureBorderColor( 0 , D3DCOLOR_ARGB(0, 0, 0, 0) );
			
			g_Map.DisplayOnTile( m_matWorld._41 , m_matWorld._43 , m_fSizeX , m_fSizeZ );

			g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
			g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );
			g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_WRAP );
			
			break;
			
	}

	if(m_pBillboardData->m_bTexAni && iCurBType != 23)
	{
//		g_RenderManager.SetTexture( 0, curBillboardTexture );
		g_RenderManager.SetSrcBlend( m_pBillboardData->m_SrcBlend );
		g_RenderManager.SetDestBlend( m_pBillboardData->m_DestBlend );
		g_RenderManager.EndBatchPrimitive();
		g_RenderManager.SetBeginBatchRendering(FALSE);
	}
	

// 	if(prevBillboardData)
// 	{
// 		g_RenderManager.SetTexture( 0, prevBillboardData->GetTexture() );
// 		g_RenderManager.SetSrcBlend( prevBillboardData->m_SrcBlend );
// 		g_RenderManager.SetDestBlend( prevBillboardData->m_DestBlend );
// 		g_RenderManager.EndBatchPrimitive();
// 		g_RenderManager.SetBeginBatchRendering(FALSE);
// 	}
	
#endif	//	BATCH_RENDER	
}

void		CBillboard::DrawBillboard(BOOL bApyPostMat, BOOL bNoDraw)
{
	switch( g_RockClient.m_SceneManager.GetEffectDisplayMode() )	
	{
		// 노멀 모드는 전체 파티클 출력
		case SCENE_NORMAL:
			{
			}
			break;

		// 시퀀드 모드는 시퀀스 모드만 
		case SCENE_SQUENCE:
		
			if( SCENE_SQUENCE != m_SceneMode )
				return;
				
			break;
	}
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 mtrl;
#else
	D3DMATERIAL8 mtrl;
#endif // DIRECT_VERSION_9_MJH

	float	r     = m_Color.r;
	float	g     = m_Color.g;
	float	b     = m_Color.b;
	float	alpha = m_fAlpha;

	D3DUtil_InitMaterial( mtrl , r, g,	b, alpha );

	mtrl.Power = 1.0f;
	
	g_RenderManager.SetMaterial( &mtrl );

	D3DXMATRIX matWorld = m_matWorld;
	D3DXMATRIX matScale;

	D3DXMatrixScaling( &matScale, m_fScale, m_fScale, m_fScale );
	
	D3DXMatrixMultiply( &matWorld , &matScale , &matWorld );
	
	if(bApyPostMat)
	{
		D3DXMatrixMultiply( &matWorld , &matWorld , &m_matPostWorld );
	}
	
	D3DXVECTOR3 vPos = D3DXVECTOR3(matWorld._41, matWorld._42, matWorld._43);	
	if( m_pBillboardData->m_iType == 23 )
	{
		vPos.y = g_Map.PHF_GetHeight(vPos.x , vPos.z);
	}
	
	if( g_RockClient.GetGameMode() == GMODE_NORMAL && m_uSetType != EF_PICK && m_uSetType != EF_PICK_X )
	{
		
		vPos.y = 0.0f;
		D3DXVECTOR3 vPlayerPos = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41,
											  0.0f,
											  g_Pc.GetPlayer()->GetPosTM()._43 );
		
		int nPlayerDistance = (int)D3DXVec3Length( &(vPlayerPos - vPos) );			
		
		if( nPlayerDistance > g_Max_Effect_Display )
		{
			if(bNoDraw == FALSE)	///-- 그려야 하는 빌보드라면..
			  return;
		}
	}

	if( !m_pBillboardData->m_bTexAni )
		g_RenderManager.SetTexture( 0, m_pBillboardData->GetTexture() );
	else
	{
		if (m_Timer.GetTime() < 0)					return;
		
		CAnimSequence **ppData = g_Anim.m_AniData.GetKeyDataPointer( m_pBillboardData->m_iTexType );
		
		if( ppData != NULL )
		{
			g_RenderManager.SetTexture( 0, (*ppData)->GetCurFrameTexture(m_Timer) );
		}
	}

	g_RenderManager.SetAlphaBlendEnable(  TRUE );
	g_RenderManager.SetSrcBlend( m_pBillboardData->m_SrcBlend );
	g_RenderManager.SetDestBlend( m_pBillboardData->m_DestBlend );

	if(bNoDraw == TRUE)
	{
		return;
	}

	switch( m_pBillboardData->m_iType  )
	{
		case eBDType_Shield:
			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );	
			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(BILLBOARDVERTEX) );
			
			g_RenderManager.SetFVF( D3DFVF_BILLBOARDVERTEX );

			g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 6, 0, 4);
			break;
		
		case eBDType_VerticalPlane:
		case eBDType_HorizonPlane:
		case eBDType_VerticalLongPlane:
		case eBDType_HorizonLongPlane:
			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(BILLBOARDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BILLBOARDVERTEX );
			g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2);
			break;

#ifndef BATCH_RENDER
		case eBDType_Hemisphere:
		case eBDType_Sphere:
			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(BILLBOARDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BILLBOARDVERTEX );
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , SPHERE_VERTEX_COUNT , 0 , SPHERE_VERTEX_COUNT - 2 );
			break;

		case eBDType_Donut:
		case eBDType_DonutUVOne:
			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(BILLBOARDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BILLBOARDVERTEX );
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 62 , 0 , 60 );
			break;
#endif	//	BATCH_RENDER
		case eBDType_Spot:
		case eBDType_Spot1:
		case eBDType_Spot2:
			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(BILLBOARDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BILLBOARDVERTEX );
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , 40 , 0 , 20 );
			break;

		case eBDType_Sword:
		case eBDType_CrossPlane:
			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(BILLBOARDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BILLBOARDVERTEX );
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , 8 , 0 , 4 );
			break;
		
#ifndef BATCH_RENDER
		case eBDType_Cylinder6:
			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(BILLBOARDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BILLBOARDVERTEX );
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 14 , 0 , 12 );
			break;

		case eBDType_Cylinder8:
			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(BILLBOARDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BILLBOARDVERTEX );
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 18 , 0 , 16 );
			break;
#endif	//	BATCH_RENDER

		case eBDType_Cylinder:
		case eBDType_CylinderUp:
		case eBDType_CylinderDown:
			g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );
			g_RenderManager.SetIndices(g_Billboard.m_pIB[m_pBillboardData->m_iType], 0);
			g_RenderManager.SetStreamSource( 0, g_Billboard.m_pVB[m_pBillboardData->m_iType], sizeof(BILLBOARDVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_BILLBOARDVERTEX );
			g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 50, 0, 24 * 2);
			break;
		
		case 23:

			g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_CLAMP );
			g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_CLAMP );
			g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_CLAMP );

			g_RenderManager.SetTextureBorderColor( 0 , D3DCOLOR_ARGB(0, 0, 0, 0) );
			
			g_Map.DisplayOnTile( m_matWorld._41 , m_matWorld._43 , m_fSizeX , m_fSizeZ );

			g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
			g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );
			g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_WRAP );
			
			break;
			
		default:
			{
				g_RenderManager.SetTransform(D3DTS_WORLD, &matWorld );	
				Model **pModel = g_Billboard.m_pExtraModelList.GetKeyDataPointer( m_pBillboardData->m_iType );
				
				if( pModel != NULL )
				{
					g_RenderManager.SetIndices( (*pModel)->m_pIB , 0 );
					g_RenderManager.SetStreamSource( 0, (*pModel)->m_pVB, sizeof(OBJECTVERTEX) );
					g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );
					
					g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
						0, (*pModel)->m_header.m_num_vertex,
						0, (*pModel)->m_header.m_num_face );
					
				}
				
			}
			break;
	}

}


void CBillboardData::LoadTexture( char* path )
{
//	char temp[512] = {0,};
//	sprintf( temp, "텍스쳐 읽기 실패:%s", path );

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if( FAILED(g_RenderManager.CreateTextureFromFileEx( path, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
														D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0xff000000, NULL, NULL, &m_pTexture )))
	{
		if( !m_bTexAni )
		{
			DebugPrint( "----- Load Texture ( CBillboardData ) : %s ----\n" , path );
//			MessageBox( NULL, temp, "오류", NULL );
			m_pTexture = NULL;
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFEffectTex.OpenFile( path );	
	if( pFH )
	{
		g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),
														D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
														D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0xff000000, NULL, NULL, &m_pTexture ); 																
		g_VFInterface.CloseFile( pFH );	
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

}

void CBillboardData::LoadData(char *FileName)
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	FILE		*fp;

	char Path   [ 1024 ] = "";
	char TxtPath[ 1024 ] = "";

	wsprintf( Path    , "%s\\Effect\\%s"    , g_RockClient.GetRootDir() , FileName );
	wsprintf( TxtPath , "%s\\Effect\\Tex\\" , g_RockClient.GetRootDir()			   );
	
	fp = fopen( Path, "rb");

	assert( fp != NULL && "!이펙트 빌보드 파일 읽기 실패" );
	{
		fread( &m_iType, sizeof(m_iType), 1, fp );

		fread( &m_cTexName, sizeof(m_cTexName), 1, fp );
		fread( &m_bTexAni, sizeof(m_bTexAni), 1, fp );
		fread( &m_iTexType, sizeof(m_iTexType), 1, fp );

		fread( &m_bInfinityLoop, sizeof(m_bInfinityLoop), 1, fp );
		fread( &m_iLoop, sizeof(m_iLoop), 1, fp );
		fread( &m_fLife, sizeof(m_fLife), 1, fp );

		fread( &m_bSizePump, sizeof(m_bSizePump), 1, fp );
		fread( &m_vStartSize, sizeof(m_vStartSize), 1, fp );
		fread( &m_vEndSize, sizeof(m_vEndSize), 1, fp );

		fread( &m_bMovePump, sizeof(m_bMovePump), 1, fp );
		fread( &m_fHeight, sizeof(m_fHeight), 1, fp );
		fread( &m_vMovVec, sizeof(m_vMovVec), 1, fp );

		fread( &m_fRadius, sizeof(m_fRadius), 1, fp );
		fread( &m_bCharSightRot, sizeof(m_bCharSightRot), 1, fp );

		fread( &m_bXZCamRot, sizeof(m_bXZCamRot), 1, fp );
		fread( &m_bYZCamRot, sizeof(m_bYZCamRot), 1, fp );
		fread( &m_bXYZCamRot, sizeof(m_bXYZCamRot), 1, fp );

		fread( &m_fRotVel, sizeof(m_fRotVel), 1, fp );
		fread( &m_bXAxisRot, sizeof(m_bXAxisRot), 1, fp );
		fread( &m_bYAxisRot, sizeof(m_bYAxisRot), 1, fp );
		fread( &m_bZAxisRot, sizeof(m_bZAxisRot), 1, fp );

		fread( &m_iSequence, sizeof(m_iSequence), 1, fp );
		fread( &m_fAngleX, sizeof(m_fAngleX), 1, fp );
		fread( &m_fAngleY, sizeof(m_fAngleY), 1, fp );
		fread( &m_fAngleZ, sizeof(m_fAngleZ), 1, fp );

		fread( &m_bBlend, sizeof(m_bBlend), 1, fp );
		fread( &m_bBlendLoop, sizeof(m_bBlendLoop), 1, fp );

		fread( &m_iMyType, sizeof(m_iMyType), 1, fp );

		fread( &m_ColorStart    , sizeof( m_ColorStart    ) , 1 , fp );
		fread( &m_ColorVar		, sizeof( m_ColorVar      ) , 1 , fp );
		fread( &m_ColorEnd		, sizeof( m_ColorEnd      ) , 1 , fp );
			
		fread( &m_fAlphaStart	, sizeof( m_fAlphaStart   ) , 1 , fp );
		fread( &m_fAlphaVar		, sizeof( m_fAlphaVar     ) , 1 , fp );
		fread( &m_fAlphaEnd		, sizeof( m_fAlphaEnd     ) , 1 , fp );
			
		//............................................................................................................
		// 블렌딩 모드 
		//............................................................................................................
		fread( &m_SrcBlend		, sizeof( m_SrcBlend      ) , 1 , fp );
		fread( &m_DestBlend		, sizeof( m_DestBlend     ) , 1 , fp );

		fclose( fp );

		if( m_bTexAni == FALSE )
		{
			//m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( m_cTexName );
			CStringToCode	HashString( m_cTexName );
			m_HashCode = HashString.GetHashCode();	
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#else  ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
		
	VFileHandle* pFH = g_VFEffectRoot.OpenFile( FileName );
	assert( pFH != NULL && "!이펙트 빌보드 파일 읽기 실패" );
	{
		pFH->Read( &m_iType, sizeof(m_iType) );

		pFH->Read( &m_cTexName, sizeof(m_cTexName) );
		pFH->Read( &m_bTexAni, sizeof(m_bTexAni) );
		pFH->Read( &m_iTexType, sizeof(m_iTexType) );

		pFH->Read( &m_bInfinityLoop, sizeof(m_bInfinityLoop) );
		pFH->Read( &m_iLoop, sizeof(m_iLoop) );
		pFH->Read( &m_fLife, sizeof(m_fLife) );

		pFH->Read( &m_bSizePump, sizeof(m_bSizePump) );
		pFH->Read( &m_vStartSize, sizeof(m_vStartSize) );
		pFH->Read( &m_vEndSize, sizeof(m_vEndSize) );

		pFH->Read( &m_bMovePump, sizeof(m_bMovePump) );
		pFH->Read( &m_fHeight, sizeof(m_fHeight) );
		pFH->Read( &m_vMovVec, sizeof(m_vMovVec) );

		pFH->Read( &m_fRadius, sizeof(m_fRadius) );
		pFH->Read( &m_bCharSightRot, sizeof(m_bCharSightRot) );

		pFH->Read( &m_bXZCamRot, sizeof(m_bXZCamRot) );
		pFH->Read( &m_bYZCamRot, sizeof(m_bYZCamRot) );
		pFH->Read( &m_bXYZCamRot, sizeof(m_bXYZCamRot) );

		pFH->Read( &m_fRotVel, sizeof(m_fRotVel) );
		pFH->Read( &m_bXAxisRot, sizeof(m_bXAxisRot) );
		pFH->Read( &m_bYAxisRot, sizeof(m_bYAxisRot) );
		pFH->Read( &m_bZAxisRot, sizeof(m_bZAxisRot) );

		pFH->Read( &m_iSequence, sizeof(m_iSequence) );
		pFH->Read( &m_fAngleX, sizeof(m_fAngleX) );
		pFH->Read( &m_fAngleY, sizeof(m_fAngleY) );
		pFH->Read( &m_fAngleZ, sizeof(m_fAngleZ) );

		pFH->Read( &m_bBlend, sizeof(m_bBlend) );
		pFH->Read( &m_bBlendLoop, sizeof(m_bBlendLoop) );

		pFH->Read( &m_iMyType, sizeof(m_iMyType) );

		pFH->Read( &m_ColorStart    , sizeof( m_ColorStart    ) );
		pFH->Read( &m_ColorVar		, sizeof( m_ColorVar      ) );
		pFH->Read( &m_ColorEnd		, sizeof( m_ColorEnd      ) );
			
		pFH->Read( &m_fAlphaStart	, sizeof( m_fAlphaStart   ) );
		pFH->Read( &m_fAlphaVar		, sizeof( m_fAlphaVar     ) );
		pFH->Read( &m_fAlphaEnd		, sizeof( m_fAlphaEnd     ) );
			
		//............................................................................................................
		// 블렌딩 모드 
		//............................................................................................................
		pFH->Read( &m_SrcBlend		, sizeof( m_SrcBlend      ) );
		pFH->Read( &m_DestBlend		, sizeof( m_DestBlend     ) );

		g_VFEffectRoot.CloseFile( pFH );

		if( m_bTexAni == FALSE )
		{
			//m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( m_cTexName );
			CStringToCode	HashString( m_cTexName );
			m_HashCode = HashString.GetHashCode();
			
			//if( NULL == m_pTexture )
			//	DebugPrint( "Billboard -> %s\n" , FileName );
			//LoadTexture( m_cTexName );
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////	
#endif ////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

}

#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9 CBillboardData::GetTexture()
#else
LPDIRECT3DTEXTURE8 CBillboardData::GetTexture()
#endif // DIRECT_VERSION_9_MJH
{
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 lTexture = NULL;
#else
	LPDIRECT3DTEXTURE8 lTexture = NULL;
#endif // DIRECT_VERSION_9_MJH

	if(m_HashCode)
	{		
		lTexture = g_Particle.m_EffectTextureManager.GetTexture( m_HashCode );
		return lTexture;
	}
	
	return NULL;
}

void CBillboard::ReStart(void)
{ 
	m_iCurLoop     = 0; 
	m_fCurLife     = 0; 
//	m_fDelayTime   = 0.0f;
	m_LoopBlending = 0;
	m_AddRadian	   = 0.0f;		// 증감되는 각

	ColorReset();
	Timer().Begin(); 
}

void CBillboard::ColorReset(void)
{
	if( m_LoopBlending )
	{
		m_ColorDelta.r = ( m_pBillboardData->m_ColorStart.r - m_Color.r ) / m_pBillboardData->m_fLife;
		m_ColorDelta.g = ( m_pBillboardData->m_ColorStart.g - m_Color.g ) / m_pBillboardData->m_fLife;
		m_ColorDelta.b = ( m_pBillboardData->m_ColorStart.b - m_Color.b ) / m_pBillboardData->m_fLife;
		
		m_fAlphaDelta  = ( m_pBillboardData->m_fAlphaStart - m_fAlpha   ) / m_pBillboardData->m_fLife;
	}
	else
	{
		m_Color.r = m_pBillboardData->m_ColorStart.r + RANDOM_NUM * m_pBillboardData->m_ColorVar.r;
		m_Color.g = m_pBillboardData->m_ColorStart.g + RANDOM_NUM * m_pBillboardData->m_ColorVar.g;
		m_Color.b = m_pBillboardData->m_ColorStart.b + RANDOM_NUM * m_pBillboardData->m_ColorVar.b;
		
		m_Color.a = 1.0f;
		
		Clamp( m_Color.r, 0.0f, 3.0f );
		Clamp( m_Color.g, 0.0f, 3.0f );
		Clamp( m_Color.b, 0.0f, 3.0f );
		
		m_ColorDelta.r = ( m_pBillboardData->m_ColorEnd.r - m_Color.r ) / m_pBillboardData->m_fLife;
		m_ColorDelta.g = ( m_pBillboardData->m_ColorEnd.g - m_Color.g ) / m_pBillboardData->m_fLife;
		m_ColorDelta.b = ( m_pBillboardData->m_ColorEnd.b - m_Color.b ) / m_pBillboardData->m_fLife;
		
		m_fAlpha = m_pBillboardData->m_fAlphaStart + RANDOM_NUM * m_pBillboardData->m_fAlphaVar;
		
		Clamp( m_fAlpha, 0.0f , 3.0f );
		
		m_fAlphaDelta = ( m_pBillboardData->m_fAlphaEnd - m_fAlpha) / m_pBillboardData->m_fLife;
	}
}

void CBillboard::MakeLink(CBillboardData *pData)
{
	m_pBillboardData	= pData;
	m_lUnique			= -1;
	m_iIndex			= -1;
	m_fDelayTime		= 0;
	
	m_vStartSize	    = m_pBillboardData->m_vStartSize;
	m_vEndSize			= m_pBillboardData->m_vEndSize;
	
	m_vMovVec		    = m_pBillboardData->m_vMovVec;
	m_fRadius			= m_pBillboardData->m_fRadius;
	
	m_vLoc				= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_iEffSetCon		= -1;
	
	ReStart();
}



// 개별적인 업데이트
int	CBillboard::UpdateBillboardNoneSet( float TimeD )
{
	static int		    Index = 0;
	static D3DXMATRIX	mat;
	static float		height	= 0.0f;
	static float		rad		= 0.0f;

	D3DXMatrixIdentity(&mat);
	m_Timer.Update(TimeD);
	height = 0.0f;
	rad    = 0.0f;

	int nEffSetCon = m_iEffSetCon;
	int nEffType = ESLT_NONE;
	int nEffSetType = -1;
	float fAddSize = 0.0f;

	if( nEffSetCon != -1 )
	{
		nEffType = g_Particle.m_pEffSetCon[ m_iEffSetCon ].m_iType;
		nEffSetType = g_Particle.m_pEffSetCon[ m_iEffSetCon ].m_iSetType;
		fAddSize = g_Particle.m_pEffSetCon[ m_iEffSetCon ].m_fAddSize;
	}

	if( m_nDirectEffType !=  ESLT_NONE )
	{
		nEffType = m_nDirectEffType;
	}

	if( nEffType == ESLT_CHARWHOLELINK )
	{
		m_fCurLife += TimeD;
	}

	m_IsUpdate = FALSE;
	
	BOOL IsLinkChr = FALSE;

	if( m_IsInitUpdate )
	{
		if( m_lUnique != -1 )
		{
			///-- 캐릭터에 연결되어 있는 빌보드 는 
			///-- 월드위치정보와 추가 위치정보(로컬 위치정보)를 
			///-- 따로 가지고잇어야 할듯....
			IsLinkChr = TRUE;
		}		
		
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_nPrevUpdateTime ) , > , m_nDelUpdateTime ) )
		{
			m_nPrevUpdateTime = g_nowTime;
			TimeD += m_fDelCumtime;
			m_fDelCumtime = 0.0f;
			m_IsUpdate = TRUE;
		}
		else
		{
			m_fDelCumtime += TimeD;
			m_IsUpdate = FALSE;
			
			if( !IsLinkChr )
			{
				return TRUE;
			}
		}
	}
	else
	{
		m_IsInitUpdate = TRUE;
		m_nPrevUpdateTime = g_nowTime;
		m_IsUpdate = TRUE;
	}

	if( nEffType == ESLT_CHARWHOLELINK )
	{
		mat = m_mLinkBoneMatrix;		
	}
	else
	{
		// 위치 수정.
		mat._41	+= m_vLoc.x;
		mat._42	+= m_vLoc.y;
		mat._43	+= m_vLoc.z;
	}

	///-- 여기까지가 
	///-- 여기까지 월드 위치 구하는 부분

	if( IsLinkChr && !m_IsUpdate )
	{
		if( nEffType == ESLT_CHARWHOLELINK )
		{
			D3DXMatrixMultiply( &m_matWorld, &m_matLocalWorld, &mat );			
		}
		else
		{
			m_matWorld = m_matLocalWorld;		
			m_matWorld._41 = m_matLocalWorld._41 + mat._41;
			m_matWorld._42 = m_matLocalWorld._42 + mat._42;
			m_matWorld._43 = m_matLocalWorld._43 + mat._43;
		}		

		return TRUE;
	}

	m_fScale = 1.0f;

	if( !ISZERO(fAddSize) )
	{
		m_fScale += fAddSize;
	}

	D3DXMATRIX	matworldtemp1, matworldtemp2, matworldtemp3, matworldtemp4, matworldtemp5, 
				matworldtemp6, matworldtemp7, matworldtemp8;
	D3DXMatrixIdentity(&matworldtemp1);
	D3DXMatrixIdentity(&matworldtemp2);
	D3DXMatrixIdentity(&matworldtemp3);
	D3DXMatrixIdentity(&matworldtemp4);
	D3DXMatrixIdentity(&matworldtemp5);
	D3DXMatrixIdentity(&matworldtemp6);
	D3DXMatrixIdentity(&matworldtemp7);
	D3DXMatrixIdentity(&matworldtemp8);

	//y축 회전 매트릭스.
	{
		if( m_pBillboardData->m_bYAxisRot )
			D3DXMatrixRotationY( &matworldtemp1, m_AddRadian + m_pBillboardData->m_fAngleY + (m_fCurLife) * m_pBillboardData->m_fRotVel * (1.0f/m_pBillboardData->m_fLife) * 6.28f);
		else if( m_pBillboardData->m_bXZCamRot || m_pBillboardData->m_bXYZCamRot )
			D3DXMatrixRotationY( &matworldtemp1, -g_Camera.m_fCameraYaw ); // 카메라 쪽으로 회전.
		else if( m_pBillboardData->m_bCharSightRot )
			D3DXMatrixRotationY( &matworldtemp1, rad + m_pBillboardData->m_fAngleY );		// rad 만큼 회전
		else
		{
			if( m_pBillboardData->m_fAngleY != 0.0f )
			{
				D3DXMatrixRotationY( &matworldtemp1, m_pBillboardData->m_fAngleY );		// rad 만큼 회전
			}
		}
	}

	//x축 회전 매트릭스.
	{
		if( m_pBillboardData->m_bXAxisRot )
			D3DXMatrixRotationX( &matworldtemp2, m_AddRadian + m_pBillboardData->m_fAngleX + (m_fCurLife) * m_pBillboardData->m_fRotVel * (1.0f/m_pBillboardData->m_fLife) * 6.28f);
		else if( m_pBillboardData->m_bYZCamRot || m_pBillboardData->m_bXYZCamRot )
		{
			if( !m_pBillboardData->m_bYAxisRot )
				D3DXMatrixRotationX( &matworldtemp2, g_Camera.m_fNewPitch ); // 카메라 쪽으로 회전.
		}
		else
		{
			if( m_pBillboardData->m_fAngleX != 0.0f )
				D3DXMatrixRotationX( &matworldtemp2, m_pBillboardData->m_fAngleX );
		}
	}

	//z축 회전 매트릭스.
	{
		if( m_pBillboardData->m_bZAxisRot )
			D3DXMatrixRotationZ( &matworldtemp3, m_AddRadian + m_pBillboardData->m_fAngleZ + (m_fCurLife) * m_pBillboardData->m_fRotVel * (1.0f/m_pBillboardData->m_fLife) * 6.28f);
		else
		{
			if( m_pBillboardData->m_fAngleZ != 0.0f )
				D3DXMatrixRotationZ( &matworldtemp3, m_pBillboardData->m_fAngleZ );
		}
	}

	if( m_pBillboardData->m_bSizePump )
	{
		if( m_bPumping )
		{
			D3DXMatrixScaling( &matworldtemp4, (m_fCurLife) * (m_vEndSize.x - m_vStartSize.x) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.x
												, (m_fCurLife) * (m_vEndSize.y - m_vStartSize.y) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.y
												, (m_fCurLife) * (m_vEndSize.z - m_vStartSize.z) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.z ); // 라이프에 맞게 스케일링
		}
		else
		{
			D3DXMatrixScaling( &matworldtemp4, (m_pBillboardData->m_fLife - m_fCurLife) * (m_vEndSize.x - m_vStartSize.x) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.x
												, (m_pBillboardData->m_fLife - m_fCurLife) * (m_vEndSize.y - m_vStartSize.y) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.y
												, (m_pBillboardData->m_fLife - m_fCurLife) * (m_vEndSize.z - m_vStartSize.z) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.z ); // 라이프에 맞게 스케일링
		}
	}
	else
	{
		D3DXMatrixScaling( &matworldtemp4, (m_fCurLife) * (m_vEndSize.x - m_vStartSize.x) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.x
									, (m_fCurLife) * (m_vEndSize.y - m_vStartSize.y) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.y
									, (m_fCurLife) * (m_vEndSize.z - m_vStartSize.z) * (1.0f/m_pBillboardData->m_fLife) + m_vStartSize.z ); // 라이프에 맞게 스케일링
	}

	m_fSizeX = matworldtemp4._11;
	m_fSizeZ = matworldtemp4._33;

	switch( m_pBillboardData->m_iSequence )
	{
	case 0:
		m_matWorld		= matworldtemp4 * (matworldtemp3 * matworldtemp1 * matworldtemp2);
		break;
	case 1:
		m_matWorld		= matworldtemp4 * (matworldtemp1 * matworldtemp3 * matworldtemp2);
		break;
	case 2:
		m_matWorld		= matworldtemp4 * (matworldtemp3 * matworldtemp2 * matworldtemp1);
		break;
	case 3:
		m_matWorld		= matworldtemp4 * (matworldtemp2 * matworldtemp3 * matworldtemp1);
		break;
	case 4:
		m_matWorld		= matworldtemp4 * (matworldtemp1 * matworldtemp2 * matworldtemp3);
		break;
	case 5:
		m_matWorld		= matworldtemp4 * (matworldtemp2 * matworldtemp1 * matworldtemp3);
		break;
	}


	if( m_pBillboardData->m_bCharSightRot )
	{
		D3DXMatrixRotationY( &matworldtemp6, rad );
		// 여기서 (0.0f 0.0f -SightLength) 보는 방향 으로 부터 SightLength 떨어 진거리 측정을 위해 
		D3DXMatrixTranslation( &matworldtemp5, 0.0f, 0.0f, (0.0f - m_fRadius) );
		D3DXMatrixMultiply( &matworldtemp5, &matworldtemp5, &matworldtemp6 );

		m_matWorld._41		= matworldtemp5._41;
		m_matWorld._42		= matworldtemp5._42;
		m_matWorld._43		= matworldtemp5._43;
	}
	else
	{
		D3DXMatrixRotationY( &matworldtemp6, -g_Camera.m_fCameraYaw );
		D3DXMatrixRotationX(&matworldtemp7, g_Camera.m_fNewPitch);
		// 여기서 (0.0f 0.0f -SightLength) 보는 방향 으로 부터 SightLength 떨어 진거리 측정을 위해 
		D3DXMatrixTranslation( &matworldtemp5, 0.0f, 0.0f, (0.0f - m_fRadius) );

		D3DXMatrixMultiply( &matworldtemp5, &matworldtemp5, &matworldtemp7 );
		D3DXMatrixMultiply( &matworldtemp5, &matworldtemp5, &matworldtemp6 );

		m_matWorld._41		= matworldtemp5._41;
		m_matWorld._42		= matworldtemp5._42;
		m_matWorld._43		= matworldtemp5._43;
	}

	if( m_pBillboardData->m_bMovePump )
	{
		if( m_bMoving )
		{
			m_matWorld._41 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * m_vMovVec.x;
			m_matWorld._42 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * m_vMovVec.y;
			m_matWorld._43 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * m_vMovVec.z;
		}
		else
		{
			m_matWorld._41 += (m_pBillboardData->m_fLife - m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * m_vMovVec.x;
			m_matWorld._42 += (m_pBillboardData->m_fLife - m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * m_vMovVec.y;
			m_matWorld._43 += (m_pBillboardData->m_fLife - m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * m_vMovVec.z;
		}
	}
	else
	{
		m_matWorld._41 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * m_vMovVec.x;
		m_matWorld._42 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * m_vMovVec.y;
		m_matWorld._43 += (m_fCurLife) * (1.0f/m_pBillboardData->m_fLife) * m_vMovVec.z;
	}

	m_matLocalWorld = m_matWorld;

	if( nEffType == ESLT_CHARWHOLELINK )
	{
		D3DXMatrixMultiply( &m_matWorld, &m_matWorld, &mat );
	}
	else
	{
		m_matWorld._41 += mat._41;
		m_matWorld._42 += mat._42;
		m_matWorld._43 += mat._43;
	}

	m_Color.r += m_ColorDelta.r * TimeD;
	m_Color.g += m_ColorDelta.g * TimeD;
	m_Color.b += m_ColorDelta.b * TimeD;

	m_fAlpha += m_fAlphaDelta   * TimeD;

	return TRUE;
}
