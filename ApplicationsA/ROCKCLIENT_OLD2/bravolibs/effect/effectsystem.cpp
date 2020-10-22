#include "RockPCH.h"
#include "VFileSystem.h"

#include	"..\\quadlist.h"
#include	<NETWORK\\Protocol.h>

#include	"..\\Pc.h"

#include	"BASE\\D3DBASE.H"
#include	"EffectVertex.h"
#include	"EffectSystem.h"

#include	"..\\Camera.h"
#include	"..\\RockClient.H"
#include	"..\\Effect.h"
#include	<OBJ\\objmath.H>

#include "..\\..\\CRenderManager.h"

extern bool g_IsEffectMinimize;

//카메라와 파티클 각자의 거리를 계산 해주는 인라인 함수 
inline float Distance( float x , float z ) { return (float)sqrt((double)(x*x + z*z)); }
inline float Distance3D( D3DXVECTOR3 &v ){ return (float)sqrt((double)(v.x*v.x + v.y*v.y + v.z*v.z)); }

//...............................................................................................................
// Random ( int )
//...............................................................................................................
int Random(int iMin, int iMax)
{
	if (iMin == iMax) return(iMin);
	return((rand() % (abs(iMax-iMin)+1))+iMin);
}

//...............................................................................................................
// Random ( float )
//...............................................................................................................
float Random(float fMin, float fMax)
{
	if (fMin == fMax) 
		return(fMin);
	
	float fRandom = (float)rand() / (float)RAND_MAX;
	
	return((fRandom * (float)fabs(fMax-fMin))+fMin);
}


CParticleSystem::CParticleSystem()
{
	m_vAddLocation      = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	
	m_vLocation			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vPrevLocation		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vMoveLocation		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vAttractLocation	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vVelocity			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_fAge				= 0.0f;
	
	m_bApplyVel			= FALSE;
	m_IsPosFix			= TRUE;
	
	m_vAttackSrcDir		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_pAttackUnit		= NULL;
	m_IsDirAttatack		= FALSE;
	m_IsGUnitLinkBoneAndSizeFix	= FALSE;
	
	m_vStartPos			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	
	/*
	m_pParticleHead		= NULL; 
	m_pParticleTail		= NULL;
	
	  m_pSystemNext		= NULL;
	  m_pSystemPrev		= NULL;
	*/
	m_bLinkOk			= false;
	
	m_fEmissionResidue	= 0.0f;
	//	m_bIsReady			= false;
	m_bIsVisible		= false;
	
	m_bIsReady			= true;
	m_bIsRender			= true;
	
	m_fRotTheta			= 0;
	m_nIdx				= 0;
	
	number				= 0;
	m_iLink				= 0;
	
	m_pReference		= NULL;
	
	Timer().Begin();
	
	m_iEffSetCon		= -1;
	m_bLink				= false;
	
	fThetaHoriz			= 0.0f;
	fThetaVert			= 180.0f;
	
	m_pSrc				= NULL;
	m_fSetRadian        = 0.0f;
	
	D3DXMatrixIdentity( &m_LinkMat );
	D3DXMatrixIdentity( &m_mLinkBoneMatrix );
	
	m_SceneMode		= g_RockClient.m_SceneManager.GetEffectCreateMode();
	
	m_nPlayerDistance		= 0;	

	m_ParticlePool.Reserve( 256 );
	
	m_aDirectLinkBone = 0;
	
	m_IsParticleApplyDis = TRUE;

#ifdef BATCH_OPTIMIZATION
	m_fCreateParTime = 0;
#endif	//	BATCH_OPTIMIZATION
}

CParticleSystem::~CParticleSystem()
{
	Destroy();
}

HRESULT	CParticleData::Create(int effect_create )
{
	int effect_data_num	= effect_create;
	/*
	0 번도 쓰자 by wxywxy
	
	  if ( effect_data_num == 0 )
	  return	S_OK;
	*/
	
	return S_OK;
}

void CParticleData::FileOpen(char *FileName)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	FILE		*fp;
	
	char Path[ 1024 ] = "";
	
	wsprintf( Path , "%s\\Effect\\%s" , g_RockClient.GetRootDir() , FileName );
	
	if( NULL == (fp = fopen( Path, "rb")) )
	{
		char szError[256];
		sprintf( szError, "!%s 파일읽기 실패", Path );
		///		MessageBox( NULL, szError, "", MB_OK );
	}
	
	fread( &m_fLife, sizeof(m_fLife), 1, fp );
	fread( &m_fLifeVar, sizeof(m_fLifeVar), 1, fp );
	fread( &m_fSpeed, sizeof(m_fSpeed), 1, fp );
	fread( &m_fSpeedVar, sizeof(m_fSpeedVar), 1, fp );
	fread( &m_uParticlesPerSec, sizeof(m_uParticlesPerSec), 1, fp );
	fread( &m_fTheta, sizeof(m_fTheta), 1, fp );
	fread( &m_fSizeStart, sizeof(m_fSizeStart), 1, fp );
	fread( &m_fSizeVar, sizeof(m_fSizeVar), 1, fp );
	fread( &m_fSizeEnd, sizeof(m_fSizeEnd), 1, fp );
	fread( &m_vGravityStart, sizeof(m_vGravityStart), 1, fp );
	fread( &m_vGravityVar, sizeof(m_vGravityVar), 1, fp );
	fread( &m_vGravityEnd, sizeof(m_vGravityEnd), 1, fp );
	fread( &m_ColorStart, sizeof(m_ColorStart), 1, fp );
	fread( &m_ColorVar, sizeof(m_ColorVar), 1, fp );
	fread( &m_ColorEnd, sizeof(m_ColorEnd), 1, fp );
	fread( &m_fAlphaStart, sizeof(m_fAlphaStart), 1, fp );
	fread( &m_fAlphaVar, sizeof(m_fAlphaVar), 1, fp );
	fread( &m_fAlphaEnd, sizeof(m_fAlphaEnd), 1, fp );
	fread( &m_bIsColliding, sizeof(m_bIsColliding), 1, fp );
	fread( &m_bIsAttractive, sizeof(m_bIsAttractive), 1, fp );
	fread( &m_bIsMoving, sizeof(m_bIsMoving), 1, fp );
	fread( &m_bIsInstantPlay, sizeof(m_bIsInstantPlay), 1, fp );
	fread( &m_iLocation_Select, sizeof(m_iLocation_Select), 1, fp );
	fread( &m_fRadius, sizeof(m_fRadius), 1, fp );
	fread( &m_fPlane_x, sizeof(m_fPlane_x), 1, fp );
	fread( &m_fPlane_y, sizeof(m_fPlane_y), 1, fp );
	fread( &m_fPlane_z, sizeof(m_fPlane_z), 1, fp );
	fread( &m_fHeight, sizeof(m_fHeight), 1, fp );
	fread( &m_iRenderingLev, sizeof(m_iRenderingLev), 1, fp );
	fread( &m_fTotalLife, sizeof(m_fTotalLife), 1, fp );
	fread( &m_fMovingRadius, sizeof(m_fMovingRadius), 1, fp );
	fread( &m_fMovingTime, sizeof(m_fMovingTime), 1, fp );
	fread( &m_cTexName, sizeof(m_cTexName), 1, fp );
	fread( &m_bTexAni, sizeof(m_bTexAni), 1, fp );
	fread( &m_iTexType, sizeof(m_iTexType), 1, fp );
	fread( &fParticlesNeeded, sizeof(fParticlesNeeded), 1, fp );
	fread( &m_iMovingDir, sizeof(m_iMovingDir), 1, fp );
	fread( &m_fAttractPower, sizeof(m_fAttractPower), 1, fp );
	fread( &m_iMovingType, sizeof(m_iMovingType), 1, fp );
	fread( &m_iMovingSpeedType, sizeof(m_iMovingSpeedType), 1, fp );
	fread( &m_iParType, sizeof(m_iParType), 1, fp );
	
	fread( &m_SrcBlend , sizeof( m_SrcBlend ) , 1 , fp );
	fread( &m_DestBlend , sizeof( m_DestBlend ) , 1 , fp );
	
	fclose( fp );	

	m_iParType = GetIndexKeyInFileName( Path );
	
	if( m_bTexAni == TRUE )
	{
		m_ppAniTextrue = g_Anim.m_AniData.GetKeyDataPointer( m_iTexType );
	}
	else
	{
		//m_pTexture = g_Particle.m_EffectTextureManager.GetTexture( m_cTexName );
		CStringToCode	HashString( m_cTexName );
		m_HashCode = HashString.GetHashCode();
	}
	
	//LoadTexture( path2 );
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else ///////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandle* pFH = g_VFEffectRoot.OpenFile( FileName );
	//	if( pFH == NULL ) return false;
	assert( pFH != NULL && "!%s 파일읽기 실패" );
	{
		pFH->Read( &m_fLife, sizeof(m_fLife) );
		pFH->Read( &m_fLifeVar, sizeof(m_fLifeVar) );
		pFH->Read( &m_fSpeed, sizeof(m_fSpeed) );
		pFH->Read( &m_fSpeedVar, sizeof(m_fSpeedVar) );
		pFH->Read( &m_uParticlesPerSec, sizeof(m_uParticlesPerSec) );
		pFH->Read( &m_fTheta, sizeof(m_fTheta) );
		pFH->Read( &m_fSizeStart, sizeof(m_fSizeStart) );
		pFH->Read( &m_fSizeVar, sizeof(m_fSizeVar) );
		pFH->Read( &m_fSizeEnd, sizeof(m_fSizeEnd) );
		pFH->Read( &m_vGravityStart, sizeof(m_vGravityStart) );
		pFH->Read( &m_vGravityVar, sizeof(m_vGravityVar) );
		pFH->Read( &m_vGravityEnd, sizeof(m_vGravityEnd) );
		pFH->Read( &m_ColorStart, sizeof(m_ColorStart) );
		pFH->Read( &m_ColorVar, sizeof(m_ColorVar) );
		pFH->Read( &m_ColorEnd, sizeof(m_ColorEnd) );
		pFH->Read( &m_fAlphaStart, sizeof(m_fAlphaStart) );
		pFH->Read( &m_fAlphaVar, sizeof(m_fAlphaVar) );
		pFH->Read( &m_fAlphaEnd, sizeof(m_fAlphaEnd) );
		pFH->Read( &m_bIsColliding, sizeof(m_bIsColliding) );
		pFH->Read( &m_bIsAttractive, sizeof(m_bIsAttractive) );
		pFH->Read( &m_bIsMoving, sizeof(m_bIsMoving) );
		pFH->Read( &m_bIsInstantPlay, sizeof(m_bIsInstantPlay) );
		pFH->Read( &m_iLocation_Select, sizeof(m_iLocation_Select) );
		pFH->Read( &m_fRadius, sizeof(m_fRadius) );
		pFH->Read( &m_fPlane_x, sizeof(m_fPlane_x) );
		pFH->Read( &m_fPlane_y, sizeof(m_fPlane_y) );
		pFH->Read( &m_fPlane_z, sizeof(m_fPlane_z) );
		pFH->Read( &m_fHeight, sizeof(m_fHeight) );
		pFH->Read( &m_iRenderingLev, sizeof(m_iRenderingLev) );
		pFH->Read( &m_fTotalLife, sizeof(m_fTotalLife) );
		pFH->Read( &m_fMovingRadius, sizeof(m_fMovingRadius) );
		pFH->Read( &m_fMovingTime, sizeof(m_fMovingTime) );
		pFH->Read( &m_cTexName, sizeof(m_cTexName) );
		pFH->Read( &m_bTexAni, sizeof(m_bTexAni) );
		pFH->Read( &m_iTexType, sizeof(m_iTexType) );
		pFH->Read( &fParticlesNeeded, sizeof(fParticlesNeeded) );
		pFH->Read( &m_iMovingDir, sizeof(m_iMovingDir) );
		pFH->Read( &m_fAttractPower, sizeof(m_fAttractPower) );
		pFH->Read( &m_iMovingType, sizeof(m_iMovingType) );
		pFH->Read( &m_iMovingSpeedType, sizeof(m_iMovingSpeedType) );
		pFH->Read( &m_iParType, sizeof(m_iParType) );
		
		pFH->Read( &m_SrcBlend , sizeof( m_SrcBlend ) );
		pFH->Read( &m_DestBlend , sizeof( m_DestBlend ) );
		
		g_VFEffectRoot.CloseFile( pFH );
		
		m_iParType = GetIndexKeyInFileName( FileName );
		
		if( m_bTexAni == TRUE )
		{
			m_ppAniTextrue = g_Anim.m_AniData.GetKeyDataPointer( m_iTexType );
		}
		else
		{
			//m_pTexture     = g_Particle.m_EffectTextureManager.GetTexture( m_cTexName );
			CStringToCode	HashString( m_cTexName );
			m_HashCode = HashString.GetHashCode();
		}
		
		if( NULL == m_HashCode && m_ppAniTextrue == NULL )
		{
			if( m_bTexAni )
			{
				DebugPrint( "Particle -> %s ( Ani Tex : %d )\n" , FileName , m_iTexType );
			}
			else
			{
				DebugPrint( "Particle -> %s ( Tex : %s )\n" , FileName , m_cTexName );
			}
		}
		
		//LoadTexture( m_cTexName );
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
}

void CParticleData::LoadTexture( char* path )
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	char temp[512] = {0,};
	sprintf( temp, "!텍스쳐 읽기 실패:%s", path );
	
	if( m_iRenderingLev != 2 )
	{
		if( FAILED(g_RenderManager.CreateTextureFromFileEx( path, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
			D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR, 
			0xff000000, NULL, NULL, &m_pTexture )))
			//												D3DCOLOR_XRGB(37,64,20), NULL, NULL, &m_pTexture )))
		{
			m_pTexture = NULL;
			
			DebugPrint( "----- Load Texture ( CParticleData ) : %s ----\n" , path );			
			
			///			MessageBox( NULL, temp, "오류", NULL );
		}
	}
	else
		m_pTexture = NULL;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else ///////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	VFileHandle* pFH = g_VFEffectTex.OpenFile( path );
	if( pFH ) 
	{
		if( m_iRenderingLev != 2 )
		{
			if( FAILED( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
				D3DX_DEFAULT, D3DX_DEFAULT, 
				1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
				D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
				D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
				0xff000000 , NULL, NULL, &m_pTexture ) ) )
			{
				m_pTexture = NULL;
				DebugPrint( "----- Load Texture ( CParticleData ) : %s ----\n" , path );
			}
		}
		g_VFEffectTex.CloseFile( pFH );
	}
	else
	{
		m_pTexture = NULL;
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
}
#ifdef DIRECT_VERSION_9_MJH
LPDIRECT3DTEXTURE9 CParticleData::GetTexture()
#else
LPDIRECT3DTEXTURE8 CParticleData::GetTexture()
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

void	CParticleSystem::Update( CParticle* pParticle,FLOAT fTimeDelta)
{
	std::list	<CParticle *>::iterator i = m_ParticleList.begin();
	
	for( ; i!= m_ParticleList.end() ; )
	{
		if( (*i)->Update( fTimeDelta ) == FALSE )
		{
			CParticle * pData = (*i);			
	
			m_ParticlePool.DeleteMember( pData );
			
			(*i) = NULL;
			
			m_ParticleList.erase( i++ );			
		}
		else
		{
			i++;
		}
	}
}

void	CParticleSystem::Destroy()
{
	std::list	<CParticle *>::iterator i = m_ParticleList.begin();
	
	for( ; i!= m_ParticleList.end() ; )
	{
		CParticle * pData = (*i);		
	
		m_ParticlePool.DeleteMember( pData );
			
		(*i) = NULL;
		
		m_ParticleList.erase( i++ );
	}
}

BOOL	CParticleSystem::Update( FLOAT fTimeDelta )
{
	static int Index = 0;

	int nEffSetCon = m_iEffSetCon;
	int nEffType = ESLT_NONE;
	int nEffSetType = -1;
	float fAddSize = 0.0f;

	D3DXVECTOR3	TempVec = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	if( nEffSetCon != -1 )
	{
		nEffType = g_Particle.m_pEffSetCon[ nEffSetCon ].m_iType;
		nEffSetType = g_Particle.m_pEffSetCon[ nEffSetCon ].m_iSetType;
		fAddSize = g_Particle.m_pEffSetCon[ nEffSetCon ].m_fAddSize;
	}	
	
	m_Timer.Update(fTimeDelta);
	m_uAliveParticle = 0;
	
	std::list	<CParticle *>::iterator i = m_ParticleList.begin();	
	
	for( ; i!= m_ParticleList.end() ; )
	{
		CParticle *pData = NULL;
		pData =	(*i);
		
		if( pData == NULL )
		{
			i++;
			continue;
		}
		
		if( pData->Update( fTimeDelta ) == FALSE )
		{
			CParticle * pData = (*i);			

			m_ParticlePool.DeleteMember( pData );
				
			(*i) = NULL;
			
			m_ParticleList.erase( i++ );
		}
		else
		{
			// 파티클 개수 카운트
			m_uAliveParticle++;
			
			i++;
		}
	}
	
	m_fAge += fTimeDelta;
	
	if( m_pReference->m_fTotalLife != 0 )
	{
		if( m_bIsCreate )
		{
			if( m_fAge >= m_pReference->m_fTotalLife )
			{
				m_bIsCreate = FALSE;
				m_bIsSuppressed = TRUE;
			}
		}
	}
	
	float fTempSize = m_fSize;
	
	if( !ISZERO(fAddSize) )
	{
		fTempSize += fAddSize;
	}

#ifdef BATCH_OPTIMIZATION
//	if( m_bIsCreate && m_fCreateParTime > 0.01/*CREATE_PAR_TIME*/)
	if( m_bIsCreate)
#else
	if( m_bIsCreate )
#endif	//	BATCH_OPTIMIZATION
	{
		D3DXMATRIX	mat;
		float		height	= 0.0f;
		float		rad		= 0.0f;
		
		D3DXVECTOR3	loc		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		
		D3DXMatrixIdentity(&mat);	
		
		height = 0.0f;
		rad    = 0.0f;
		loc.x  = loc.y = loc.z = 0.0f;
		
		if( nEffType == ESLT_SRC_RAD )
		{
			rad = m_fSetRadian;
		}
		else
			if( m_lUnique != -1 )
			{
				if( m_pSrc != NULL )
				{
					if( m_pSrc->m_nTableIndex == -1 )
					{
						m_bIsSuppressed = TRUE;
						
						return FALSE;
					}
					
					if( m_pSrc->m_bIsVisible == FALSE )
						return TRUE;
					
					
					float fAddheight = m_vAddLocation.y;			
					
					rad		= m_pSrc->GetDirection();				
					mat		= m_pSrc->GetBonCenterTM();				
					
					mat._42 +=  fAddheight;						
					
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
					
					if( ( m_iLink == 0 && m_aDirectLinkBone == 0 ) || m_IsDirAttatack )
					{
						loc.x = mat._41;
						loc.y = mat._42;
						loc.z = mat._43;					
					}
					// 오른손 왼손 이펙트 ( 파티클이 따라오지 않고 그자리에서 생성 )
					else
					{
						BOOL bFlag = TRUE;

						if( m_aDirectLinkBone )
						{
							bFlag = m_pSrc->GetDirectBoneLinkTM( m_aDirectLinkBone , mat );
						}
						else
						{
							bFlag = m_pSrc->GetBoneLinkTM( m_iLink , mat, FALSE );
						}
						
						if( bFlag )
						{
							loc.x = mat._41;
							loc.y = mat._42;
							loc.z = mat._43; 
						}
						else
						{
							m_bIsSuppressed = TRUE;						
							return FALSE;
						}
					}
					
				}
				else
				{
					m_bIsSuppressed = TRUE;
					return FALSE;
				}
			}
			
			if( m_lUnique != -1 && m_pSrc != NULL && 
			   ( ( m_iLink == 0 && m_aDirectLinkBone == 0 ) || m_IsDirAttatack ) )
			{
				if( g_Map.IsGiantUnit( m_pSrc ) )
				{
					D3DXVECTOR3	vVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					float fSize = (float)g_Map.m_NpcTable[m_pSrc->m_nTableIndex].theNpcSize;
					if( m_IsDirAttatack )
					{
						vVec = m_vAttackSrcDir;
						vVec.y = 0.0f;
						D3DXVec3Normalize( &vVec , &vVec );
						vVec *= (fSize * 6.5);
					}
					else if(nEffSetType == EF_ATTACK_MISS)
					{
						D3DXVECTOR3	vDest = g_Camera.m_vEye;
						vDest.y = 0.0f;
						vVec = vDest - D3DXVECTOR3(mat._41, 0.0f, mat._43);	
						D3DXVec3Normalize( &vVec , &vVec );
						vVec *= (fSize * 10);								
					}
					else
					{
						if( !m_IsGUnitLinkBoneAndSizeFix )
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
							
							fTempSize *= (float)(nSize);
						}
					}
					
					loc += vVec;			
				}
			}			
			
			if( m_pReference->m_bIsMoving )
			{
				int x;
				if( m_pReference->m_iMovingDir == 0 )
				{
					x = 1;
				}
				else if ( m_pReference->m_iMovingDir == 1 )
				{
					x = -1;
				}
				else
					x = 0;
				
				float first;
				float second;
				
				fThetaHoriz += 50.0f * fTimeDelta * m_pReference->m_fMovingTime * x;
				fThetaVert  += 30.0f * fTimeDelta * m_pReference->m_fMovingTime * x;
				
				if ( fThetaHoriz >= 360.0f )
					fThetaHoriz -= 360.0f;
				
				if ( fThetaVert >= 360.0f ) 
					fThetaVert -= 360.0f;
				
				if( m_pReference->m_iMovingSpeedType == 0 )
				{
					first = fThetaHoriz* 0.01745f;
					second = fThetaVert* 0.01745f;
				}
				else
				{
					second = fThetaHoriz* 0.01745f;
					first = fThetaVert* 0.01745f;
				}
				
				//수정
				if( m_pReference->m_iMovingType == 3 || m_pReference->m_iMovingType == 4 )
				{
					m_vLocation.x = 0.0f;
					m_vLocation.y = 0.0f;
					m_vLocation.z = (m_pReference->m_fMovingRadius);
				}
				else
				{
					m_vLocation.x = (m_pReference->m_fMovingRadius);
					m_vLocation.y = 0.0f;
					m_vLocation.z = 0.0f;
				}
				
				D3DXMATRIX matWorld;
				D3DXMATRIX matWorldtemp;
				D3DXMatrixTranslation(&matWorld, m_vLocation.x, 
					m_vLocation.y,
					m_vLocation.z );
				
				if( m_pReference->m_iMovingType == 0 )
				{
					D3DXMatrixRotationY( &matWorldtemp, first );
					D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
				}
				else if( m_pReference->m_iMovingType == 1 )
				{
					D3DXMatrixRotationY( &matWorldtemp, first );
					D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
					D3DXMatrixRotationX( &matWorldtemp, second );
					D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
				}
				else if( m_pReference->m_iMovingType == 2 )
				{
					D3DXMatrixRotationY( &matWorldtemp, first );
					D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
					D3DXMatrixRotationZ( &matWorldtemp, second );
					D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
				}
				else if( m_pReference->m_iMovingType == 3 )
				{
					D3DXMatrixRotationX( &matWorldtemp, first );
					D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
					D3DXMatrixRotationY( &matWorldtemp, second );
					D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
				}
				else if( m_pReference->m_iMovingType == 4 )
				{
					D3DXMatrixRotationX( &matWorldtemp, first );
					D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
					D3DXMatrixRotationZ( &matWorldtemp, second );
					D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
				}
				else if( m_pReference->m_iMovingType == 5 )
				{
					D3DXMatrixRotationZ( &matWorldtemp, first );
					D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
				}
				
				m_vLocation.x = matWorld._41;
				m_vLocation.y = matWorld._42;
				m_vLocation.z = matWorld._43;
				
				//..................................................................................................
				// 시전자쪽으로 회전
				//..................................................................................................
				D3DXMATRIX matRotY;
				D3DXMatrixRotationY( &matRotY , rad );
				
				D3DXVec3TransformCoord( &m_vLocation , &m_vLocation , &matRotY  );
				//..................................................................................................
				
			}
			else
			{
				if(m_pReference->m_iLocation_Select == 0 )
				{
					m_vLocation.x = 0.0f;
					m_vLocation.y = 0.0f;
					m_vLocation.z = 0.0f;
				}
				
				if(m_pReference->m_iLocation_Select == 1 )
				{
					float	x = RANDOM_NUM;
					m_vLocation.x = m_pReference->m_fRadius * (float)cos( x * 180.0f );
					m_vLocation.y = 0.0f;
					m_vLocation.z = m_pReference->m_fRadius * (float)sin( x * 180.0f );
				}
				
				if(m_pReference->m_iLocation_Select == 2 )
				{
					m_vLocation.x = RANDOM_NUM * m_pReference->m_fPlane_x;
					m_vLocation.y = RANDOM_NUM * m_pReference->m_fPlane_y;
					m_vLocation.z = RANDOM_NUM * m_pReference->m_fPlane_z;
				}
				
			}
			
			D3DXMATRIX matWorld;
			D3DXMATRIX matWorldtemp;
			D3DXMatrixTranslation(&matWorld, m_vLocation.x, 
				m_vLocation.y,
				m_vLocation.z );
			
			D3DXMatrixRotationY( &matWorldtemp, m_fRotTheta );
			D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
			m_vLocation.x = matWorld._41;
			m_vLocation.y = matWorld._42;
			m_vLocation.z = matWorld._43;

			///-- 여기까지 이동부 업데이트 루틴

			
			float delta = fTimeDelta;
			
			///-- 이 루틴은 뭐지???? - 이루틴때문에 누적타임을 적용하기 곤란...;;;
			///-- 초당 파티클수가 정확하게 안들어간 이유가 이거바꾸면 이펙트 데이타도 다 손보아 할지 모르므로
			///-- 손대기도 난해.. ㅡㅡ;
			if( fTimeDelta > 0.5f )
				delta = 0.002f;
			
			float NeededPar = m_pReference->fParticlesNeeded;
			if( NeededPar > 6.0f )
			{
				NeededPar = 6.0f;
			}
			NeededPar += m_pReference->m_uParticlesPerSec * delta;
			
			///파티클 시스템과 카메라의 간격을 계산한다. -- 생성파티클개수 재조정
			//---------------------------------------------------------------------------
			//---------------------------------------------------------------------------
			BOOL bParCreate = TRUE;
			if( g_RockClient.GetGameMode() == GMODE_NORMAL )
			{
				D3DXVECTOR3 vPos = m_vLocation;
				vPos	+= loc;			
				if( !m_IsDirAttatack )
				{
					vPos.y	+= height * m_pReference->m_fHeight;
				}
				
				vPos	+= m_vMoveLocation;

				if(m_bLinkOk)
				{
					TempVec = vPos;				
				}

				vPos.y = 0.0f;
				
				D3DXVECTOR3 vPlayerPos = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41,
					0.0f,
					g_Pc.GetPlayer()->GetPosTM()._43 );
				
				m_nPlayerDistance = (int)D3DXVec3Length( &(vPlayerPos - vPos) );	
				
				if( m_nPlayerDistance > g_Max_Effect_Display )
				{
					bParCreate = FALSE;
					m_fEmissionResidue = 0.0f;
				}
				
				float fRatio = 0.0f;
				fRatio = (m_nPlayerDistance - 24) / (float)g_Max_Effect_Display;				
		
				if( m_IsParticleApplyDis )
				{
					if( g_IsEffectMinimize )
					{
						if(fRatio < 0.1f)
						{
							fRatio = 0.1f; 
						}
						
						NeededPar -= ((NeededPar * fRatio) * 0.75f * 5.0f);	
					}
					else
					{
						if(fRatio < 0.0f)
						{
							fRatio = 0.0f; 
						}
						
						NeededPar -= ((NeededPar * fRatio) * 0.75f);	
					}
				}
				
				if( NeededPar < 0.01f )
				{
					NeededPar = 0.01f;
				}
			}
			
			NeededPar += m_fEmissionResidue;
			//---------------------------------------------------------------------------
			//---------------------------------------------------------------------------
			
			unsigned int uParticlesCreated = (unsigned int)NeededPar;
			
			if ( !m_bIsSuppressed )
			{
				m_fEmissionResidue = NeededPar - (float)uParticlesCreated;
			}
			else
			{
				m_fEmissionResidue = NeededPar;
				uParticlesCreated = 0;
			}	

			loc.x += m_vAddLocation.x;
			loc.z += m_vAddLocation.z;
			
			if( m_bIsRender && bParCreate )
			{
				for( unsigned int i = 0; i < uParticlesCreated; i++ )
				{
					CParticle*	pAddParticle = NULL;					

					pAddParticle = m_ParticlePool.NewMember();
					
					if (pAddParticle == NULL)
					
					pAddParticle->m_fAge		=	0.0f;
					pAddParticle->m_fLifetime = m_pReference->m_fLife + RANDOM_NUM * m_pReference->m_fLifeVar;
					Clamp( pAddParticle->m_fLifetime, 0.0f, MAX_LIFETIME );
					
					float	ReverseLife = 1.0f / pAddParticle->m_fLifetime;
					
					pAddParticle->m_Color.r = m_pReference->m_ColorStart.r + RANDOM_NUM * m_pReference->m_ColorVar.r;
					pAddParticle->m_Color.g = m_pReference->m_ColorStart.g + RANDOM_NUM * m_pReference->m_ColorVar.g;
					pAddParticle->m_Color.b = m_pReference->m_ColorStart.b + RANDOM_NUM * m_pReference->m_ColorVar.b;
					
					pAddParticle->m_Color.a = 3.0f;
					
					Clamp( pAddParticle->m_Color.r, 0.0f, 3.0f );
					Clamp( pAddParticle->m_Color.g, 0.0f, 3.0f );
					Clamp( pAddParticle->m_Color.b, 0.0f, 3.0f );
					
					pAddParticle->m_ColorDelta.r = (m_pReference->m_ColorEnd.r - pAddParticle->m_Color.r) * ReverseLife;
					pAddParticle->m_ColorDelta.g = (m_pReference->m_ColorEnd.g - pAddParticle->m_Color.g) * ReverseLife;
					pAddParticle->m_ColorDelta.b = (m_pReference->m_ColorEnd.b - pAddParticle->m_Color.b) * ReverseLife;
					
					pAddParticle->m_fAlpha = m_pReference->m_fAlphaStart + RANDOM_NUM * m_pReference->m_fAlphaVar;
					Clamp( pAddParticle->m_fAlpha, MIN_ALPHA, MAX_ALPHA );
					pAddParticle->m_fAlphaDelta = (m_pReference->m_fAlphaEnd - pAddParticle->m_fAlpha) * ReverseLife;
					
					pAddParticle->m_fSize = m_pReference->m_fSizeStart + RANDOM_NUM * m_pReference->m_fSizeVar;
					Clamp( pAddParticle->m_fSize, MIN_SIZE, MAX_SIZE );
					pAddParticle->m_fSizeDelta = (m_pReference->m_fSizeEnd - pAddParticle->m_fSize) * ReverseLife;
					
					pAddParticle->m_fSize		*= fTempSize;
					pAddParticle->m_fSizeDelta	*= fTempSize;
					
					pAddParticle->m_vGravity = m_pReference->m_vGravityStart + RANDOM_NUM * m_pReference->m_vGravityVar;
					
					pAddParticle->m_vGravityDelta = ( m_pReference->m_vGravityEnd - pAddParticle->m_vGravity ) * ReverseLife;
					
					pAddParticle->m_vGravity		*= fTempSize;
					pAddParticle->m_vGravityDelta	*= fTempSize;				
					
					D3DXVECTOR3 tempVelocity = (m_vLocation - m_vPrevLocation)/fTimeDelta;
					pAddParticle->m_vLocation = m_vPrevLocation + tempVelocity * RANDOM_NUM * fTimeDelta;
					
					float RandomYaw = RANDOM_NUM * 6.28318f;
					float RandomPitch = RANDOM_NUM * m_pReference->m_fTheta * 0.01745f ;
					
					pAddParticle->m_vVelocity.y = cosf(RandomPitch);
					pAddParticle->m_vVelocity.x = sinf(RandomPitch) * cosf(RandomYaw);
					pAddParticle->m_vVelocity.z = sinf(RandomPitch) * sinf(RandomYaw);	
					
					float fNewSpeed = m_pReference->m_fSpeed + RANDOM_NUM * m_pReference->m_fSpeedVar;
					Clamp( fNewSpeed, MIN_SPEED, MAX_SPEED );
					pAddParticle->m_vVelocity *= fNewSpeed ;
					
					pAddParticle->m_vVelocity *= fTempSize;
					
					pAddParticle->SetParent(this);
					
					pAddParticle->m_vLocation	+= loc;
					
					if( !m_IsDirAttatack )
					{
						pAddParticle->m_vLocation.y	+= height * m_pReference->m_fHeight;
					}
					else
					{
						pAddParticle->m_vLocation.y	+= height;
					}
					
					pAddParticle->m_vLocation	+= m_vMoveLocation;
					
					m_ParticleList.push_back( pAddParticle );
				}
				
				m_vPrevLocation = m_vLocation;
			}
//		m_fCreateParTime -= 0.01;
//		m_fCreateParTime -= CREATE_PAR_TIME;
//		m_fCreateParTime = 0;
	}	
// #ifdef BATCH_OPTIMIZATION
// 	else
// 		m_fCreateParTime += fTimeDelta;
// #endif	//	BATCH_OPTIMIZATION
	///-- 한번만 실행되는 케이스
	if( m_pReference->m_bIsInstantPlay )
	{
		m_bIsCreate = false;
		number++;
		if(( number > 1 )&&( m_uAliveParticle == 0 ))
		{
			m_bIsSuppressed = true;
			number = 0;
		}
	}	
	
	i = m_ParticleList.begin();
	
	D3DXMATRIX matWorld;
	D3DXVECTOR3	v;
		
	for( ; i!= m_ParticleList.end() ; ++i )
	{
		CParticle *pData = (*i);
		
		if( m_bLinkOk )
		{ 
			//D3DXVec3TransformCoord(&v, &(pData->m_vLocation), &m_LinkMat );	
			v = TempVec;
		}
		else
		{ 
			v = pData->m_vLocation; 
		}
			
		D3DXMatrixTranslation(&matWorld, v.x, v.y, v.z );
			
		pData->matLocal	=	matWorld;		
		
		if( m_pReference->m_iRenderingLev != LINE_RENDER )
			D3DXMatrixMultiply( &(pData->matLocal), &g_Particle.m_matInverseCamera, &(pData->matLocal) );
		
	}
	
	m_bLinkOk = false;
	
	return	TRUE;
}

void CParticleSystem::AddParticle(CParticle* pParticle)
{

}


void CParticleSystem::DeleteSystem()
{

}

HRESULT CParticleSystem::Render()
{
	// 씬메니저의 상황에 따른 파티클 출력 여부
	
	
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
			return S_OK;
		
		break;
	}
	
	if( g_RockClient.GetGameMode() == GMODE_NORMAL )
	{
		if( m_nPlayerDistance > g_Max_Effect_Display )
		{
			return S_OK;
		}
	}

	DWORD dColOp = g_RenderManager.GetTextureColorOP( 0 );
	DWORD dColArg1 = g_RenderManager.GetTextureColorArg1( 0 );
	DWORD dColArg2 = g_RenderManager.GetTextureColorArg2( 0 );

	DWORD dAlpOp = g_RenderManager.GetTextureAlphaOP( 0 );
	DWORD dAlpArg1 = g_RenderManager.GetTextureAlphaArg1( 0 );
	DWORD dAlpArg2 = g_RenderManager.GetTextureAlphaArg2( 0 );

#ifdef BATCH_RENDER
	DWORD dwSrcBlend	= g_RenderManager.GetSrcBlend();
	DWORD dwDestBlend	= g_RenderManager.GetDestBlend();
#endif	//	BATCH_RENDER

#ifdef BATCH_RENDER
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );

	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_DIFFUSE );		

	g_RenderManager.SetLighting( FALSE );

	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA);
	g_RenderManager.SetDestBlend( D3DBLEND_ONE );
#else	//	BATCH_RENDER
	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.SetLighting( TRUE );

#endif	//	BATCH_RENDER

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	

#ifdef BATCH_RENDER
//	RenderLoop();
	RenderBatchDraw();
#else	//	BATCH_RENDER
	RenderLoop();	
#endif	//	BATCH_RENDER

	g_RenderManager.SetTextureColorOP  ( 0 , dColOp );
	g_RenderManager.SetTextureColorArg1( 0 , dColArg1 );
	g_RenderManager.SetTextureColorArg2( 0 , dColArg2 );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , dAlpOp );
	g_RenderManager.SetTextureAlphaArg1( 0 , dAlpArg1 );
	g_RenderManager.SetTextureAlphaArg2( 0 , dAlpArg2 );

#ifdef BATCH_RENDER
	g_RenderManager.SetSrcBlend(dwSrcBlend);
	g_RenderManager.SetDestBlend(dwDestBlend);
#endif	//	BATCH_RENDER	
	return S_OK;
}
 
HRESULT	CParticleSystem::RenderBatchDraw()
{
#ifdef BATCH_RENDER
	if( m_ParticleList.size() <= 0 )
		return S_OK;
	
	static D3DXVECTOR3 v0( -0.5f , -0.5f , 0.0f );
	static D3DXVECTOR3 v1( -0.5f ,  0.5f , 0.0f );
	static D3DXVECTOR3 v2(  0.5f , -0.5f , 0.0f );
	static D3DXVECTOR3 v3(  0.5f ,  0.5f , 0.0f );
	static D3DXMATRIX  matWorld;
	static int		   frame = 0;
	static D3DXMATRIX  matSize;
	
	if( m_pReference->m_iTexSpeed == 0 )
	{
		m_pReference->m_iTexSpeed = 1;
	}
	
	frame++;
	
	int temp = (int)( frame / m_pReference->m_iTexSpeed );
	
	if( (m_pReference->m_iTexCount - 1) < temp )
	{	
		frame = 0;
	}
	
	if( m_pReference->m_iTexCount == 1 )
	{ 
		temp = 0;
	}	
	
	if( !m_pReference->m_bTexAni )
	{
		g_RenderManager.BeginBatchPrimitive();
		g_RenderManager.SetTexture( 0, m_pReference->GetTexture() );
	}
	else
	{
		if( m_Timer.GetTime() < 0 )
			return E_FAIL;
		
		// CAnimSequence **ppData = g_Anim.m_AniData.GetKeyDataPointer( m_pReference->m_iTexType );
		
		if( m_pReference->m_ppAniTextrue != NULL )
		{
			g_RenderManager.SetTexture( 0, (*m_pReference->m_ppAniTextrue)->GetCurFrameTexture( m_Timer ) );
		}
	}
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	
//	VERTEX_XYZ_DIFFUSE_TEX1 *pVertices = NULL;
	
	std::list	<CParticle *>::iterator i = m_ParticleList.begin();
	
	g_RenderManager.SetTransform( D3DTS_WORLD, &g_Particle.m_matIdentity );
	
	int Count = 0;	
	
	D3DXVECTOR3 vPos;
	int			LastRenderCount = 0;
	int			RenderCount     = m_ParticleList.size();
	DWORD		Color = 0;
	static WORD		dwIndex[6] = { 0, 1, 2, 2, 1, 3 };
	for( ; i!= m_ParticleList.end() ; ++i )
	{
		D3DXMatrixScaling( &matSize  , (*i)->m_fSize, (*i)->m_fSize, (*i)->m_fSize);
		D3DXMatrixMultiply( &matWorld , &matSize , &(*i)->matLocal );

		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[0].v , &v0 , &matWorld );
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[1].v , &v1 , &matWorld );
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[2].v , &v2 , &matWorld );
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[3].v , &v3 , &matWorld );
		g_Particle.m_pParticleMemVB[0].color =
			g_Particle.m_pParticleMemVB[1].color =
			g_Particle.m_pParticleMemVB[2].color =
			g_Particle.m_pParticleMemVB[3].color =
			D3DCOLOR_COLORVALUE( (*i)->m_Color.r,
			(*i)->m_Color.g , (*i)->m_Color.b , (*i)->m_fAlpha  );
		g_RenderManager.AddBatchIndexedVertex(&g_Particle.m_pParticleMemVB[0], 4, dwIndex, 6);

		if( m_pReference->m_bTexAni )
			g_RenderManager.EndBatchPrimitive();
	}
	if( !m_pReference->m_bTexAni )
		g_RenderManager.EndBatchPrimitive();

#endif	//	BATCH_RENDER
	return	S_OK;
}

HRESULT	CParticleSystem::RenderLock()
{
	if( m_ParticleList.size() <= 0 )
		return S_OK;
	
	static D3DXVECTOR3 v0( -0.5f , -0.5f , 0.0f );
	static D3DXVECTOR3 v1( -0.5f ,  0.5f , 0.0f );
	static D3DXVECTOR3 v2(  0.5f , -0.5f , 0.0f );
	static D3DXVECTOR3 v3(  0.5f ,  0.5f , 0.0f );
	static D3DXMATRIX  matWorld;
	static int		   frame = 0;
	static D3DXMATRIX  matSize;
	
	if( m_pReference->m_iTexSpeed == 0 )
	{
		m_pReference->m_iTexSpeed = 1;
	}
	
	frame++;
	
	int temp = (int)( frame / m_pReference->m_iTexSpeed );
	
	if( (m_pReference->m_iTexCount - 1) < temp )
	{	
		frame = 0;
	}
	
	if( m_pReference->m_iTexCount == 1 )
	{ 
		temp = 0;
	}	
	
	if( !m_pReference->m_bTexAni )
	{
		g_RenderManager.SetTexture( 0, m_pReference->GetTexture() );
	}
	else
	{
		if( m_Timer.GetTime() < 0 )
			return E_FAIL;
		
		// CAnimSequence **ppData = g_Anim.m_AniData.GetKeyDataPointer( m_pReference->m_iTexType );
		
		if( m_pReference->m_ppAniTextrue != NULL )
		{
			g_RenderManager.SetTexture( 0, (*m_pReference->m_ppAniTextrue)->GetCurFrameTexture( m_Timer ) );
		}
	}
	g_RenderManager.SetFVF( D3DFVF_VERTEX_XYZ_DIFFUSE_TEX1 );
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	
	g_RenderManager.SetSrcBlend( m_pReference->m_SrcBlend );
	g_RenderManager.SetDestBlend( m_pReference->m_DestBlend );
	
	VERTEX_XYZ_DIFFUSE_TEX1 *pVertices = NULL;
	
	std::list	<CParticle *>::iterator i = m_ParticleList.begin();
	
	g_RenderManager.SetTransform( D3DTS_WORLD, &g_Particle.m_matIdentity );
	
	int Count = 0;	
	
	D3DXVECTOR3 vPos;
	int			LastRenderCount = 0;
	int			RenderCount     = m_ParticleList.size();
	DWORD		Color = 0;
	for( ; i!= m_ParticleList.end() ; ++i )
	{
		D3DXMatrixScaling( &matSize  , (*i)->m_fSize, (*i)->m_fSize, (*i)->m_fSize);
		
		D3DXMatrixMultiply( &matWorld , &matSize , &(*i)->matLocal );
#ifdef BATCH_RENDER
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[ Count		].v , &v0 , &matWorld );
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[ Count + 1 ].v , &v1 , &matWorld );
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[ Count + 2 ].v , &v2 , &matWorld );
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[ Count + 3 ].v , &v3 , &matWorld );
#else	//	BATCH_RENDER
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[ Count		].position , &v0 , &matWorld );
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[ Count + 1 ].position , &v1 , &matWorld );
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[ Count + 2 ].position , &v2 , &matWorld );
		D3DXVec3TransformCoord( &g_Particle.m_pParticleMemVB[ Count + 3 ].position , &v3 , &matWorld );
#endif	//	BATCH_RENDER
		g_Particle.m_pParticleMemVB[ Count	    ].color =
			g_Particle.m_pParticleMemVB[ Count + 1  ].color =
			g_Particle.m_pParticleMemVB[ Count + 2  ].color =
			g_Particle.m_pParticleMemVB[ Count + 3  ].color = D3DCOLOR_COLORVALUE( (*i)->m_Color.r , (*i)->m_Color.g , (*i)->m_Color.b , (*i)->m_fAlpha  );
		
		Count += 4;
		
		LastRenderCount++;
		
		if( LastRenderCount >= g_Particle.m_ParticleLockMaxCount )
		{
#ifdef DIRECT_VERSION_9_MJH
			if( FAILED( g_Particle.m_pParticleVB->Lock( 0, 0 , (void**)&pVertices, 0  ) ) )
#else
			if( FAILED( g_Particle.m_pParticleVB->Lock( 0, 0 , (BYTE**)&pVertices, 0  ) ) )
#endif // DIRECT_VERSION_9_MJH
				return E_FAIL;
			
			memcpy( pVertices , g_Particle.m_pParticleMemVB , sizeof( VERTEX_XYZ_DIFFUSE_TEX1 ) * 4 * g_Particle.m_ParticleLockMaxCount );
			
			g_Particle.m_pParticleVB->Unlock();
			
			g_RenderManager.SetIndices( g_Particle.m_pParticleIB , 0 );
			g_RenderManager.SetStreamSource( 0, g_Particle.m_pParticleVB , sizeof( VERTEX_XYZ_DIFFUSE_TEX1 ) );
			g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , LastRenderCount * 4 , 0 , LastRenderCount * 2 );

			RenderCount     -= LastRenderCount;
			LastRenderCount = 0;
			Count			= 0;
			
			if( RenderCount == 0 )
			{
				return S_OK;
			}
		}
	}
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( g_Particle.m_pParticleVB->Lock( 0, sizeof( VERTEX_XYZ_DIFFUSE_TEX1 ) * 4 * LastRenderCount , (void**)&pVertices, 0  ) ) )
#else
	if( FAILED( g_Particle.m_pParticleVB->Lock( 0, sizeof( VERTEX_XYZ_DIFFUSE_TEX1 ) * 4 * LastRenderCount , (BYTE**)&pVertices, 0  ) ) )
#endif // DIRECT_VERSION_9_MJH	
		return E_FAIL;
	
	memcpy( pVertices , g_Particle.m_pParticleMemVB , sizeof( VERTEX_XYZ_DIFFUSE_TEX1 ) * 4 * RenderCount );
	
	g_Particle.m_pParticleVB->Unlock();
	
	g_RenderManager.SetIndices( g_Particle.m_pParticleIB , 0 );
	g_RenderManager.SetStreamSource( 0, g_Particle.m_pParticleVB , sizeof( VERTEX_XYZ_DIFFUSE_TEX1 ) );
	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , 4 * LastRenderCount , 0 , LastRenderCount * 2 );
	
	return	S_OK;
}

// 
HRESULT	CParticleSystem::RenderLoop()
{
	static  int frame = 0;
	
	if( m_pReference->m_iTexSpeed == 0 )
	{
		m_pReference->m_iTexSpeed = 1;
	}
	
	frame++;
	
	int temp = (int)( frame / m_pReference->m_iTexSpeed );
	
	if( (m_pReference->m_iTexCount - 1) < temp )
	{	
		frame = 0;
	}
	
	if( m_pReference->m_iTexCount == 1 )
	{ 
		temp = 0;
	}
	
	if( !m_pReference->m_bTexAni )
	{
		g_RenderManager.SetTexture( 0, m_pReference->GetTexture() );
	}
	else
	{
		if( m_Timer.GetTime() < 0 )
			return E_FAIL;
		
		// CAnimSequence **ppData = g_Anim.m_AniData.GetKeyDataPointer( m_pReference->m_iTexType );
		
		if( m_pReference->m_ppAniTextrue != NULL )
		{
			g_RenderManager.SetTexture( 0, (*m_pReference->m_ppAniTextrue)->GetCurFrameTexture( m_Timer ) );
		}
	}

	DWORD dLighting = g_RenderManager.GetLighting();
	DWORD dFVF = g_RenderManager.GetFVF();	
	DWORD dSrcBlend = g_RenderManager.GetSrcBlend();
	DWORD dDestBlend = g_RenderManager.GetDestBlend(); 
	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	
	switch( m_pReference->m_iRenderingLev )
	{
	case POINT_RENDER:
		{
			return S_OK;
		}
	case LINE_RENDER:
		{
			return S_OK;
		}
	case SPACE_RENDER:
		{
			g_RenderManager.SetLighting( TRUE );
			g_RenderManager.SetIndices(g_Particle.m_pSpaceIB, 0);
			g_RenderManager.SetStreamSource( 0, g_Particle.m_pSpaceVB, sizeof(SPACEVERTEX) );
			g_RenderManager.SetFVF( D3DFVF_SPACEVERTEX );
			
			break;
		}
	}
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 mtrl;
#else
	D3DMATERIAL8 mtrl;
#endif // DIRECT_VERSION_9_MJH
	
	g_RenderManager.SetAlphaBlendEnable( TRUE );
	
	g_RenderManager.SetSrcBlend( m_pReference->m_SrcBlend );
	g_RenderManager.SetDestBlend( m_pReference->m_DestBlend );
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 preMaterial;
#else
	D3DMATERIAL8 preMaterial;
#endif // DIRECT_VERSION_9_MJH	
	g_RenderManager.GetMaterial( &preMaterial );
	
	std::list	<CParticle *>::iterator i = m_ParticleList.begin();
	
	for( ; i!= m_ParticleList.end() ; ++i )
	{
		D3DXMATRIX	matwoldtemp;
		CParticle * pData = (*i);
		
		D3DUtil_InitMaterial( mtrl, (*i)->m_Color.r , (*i)->m_Color.g ,	(*i)->m_Color.b , (*i)->m_fAlpha );
		
		g_RenderManager.SetMaterial( &mtrl );
		
		switch( m_pReference->m_iRenderingLev )
		{
		case POINT_RENDER:
			{
				return S_OK;
			}
			
		case LINE_RENDER:
			{
				return S_OK;
			}
			
		case SPACE_RENDER:
			{
				D3DXMatrixIdentity(&matwoldtemp);
				D3DXMatrixScaling(&matwoldtemp , (*i)->m_fSize, (*i)->m_fSize, (*i)->m_fSize);
				D3DXMatrixMultiply(&matwoldtemp, &matwoldtemp   , &(*i)->matLocal );
				
				g_RenderManager.SetTransform(D3DTS_WORLD, &matwoldtemp );
				g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
				
				g_RenderManager.SetFillMode( D3DFILL_SOLID );
			}
			break;
		}
	}

	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );
	g_RenderManager.SetSrcBlend( dSrcBlend );
	g_RenderManager.SetDestBlend( dDestBlend );
	g_RenderManager.SetMaterial( &preMaterial );
	g_RenderManager.SetLighting( dLighting );
	g_RenderManager.SetFVF( dFVF );
	
	//...........................................................................................................
	//옵션 off
	//...........................................................................................................
	switch( m_pReference->m_iRenderingLev )
	{
	case POINT_RENDER:
		{
			break;
		}
		
	case LINE_RENDER:
		{
			break;
		}
	}
	//...........................................................................................................
	
	return	S_OK;
}

HRESULT	CParticleSystem::InvalidateDeviceObjects()
{
	m_bIsReady	= false;
	
	return	S_OK;
}

bool CParticleSystem::IsColliding()
{
	return m_pReference->m_bIsColliding;
}

bool CParticleSystem::IsAttractive()
{
	return m_pReference->m_bIsAttractive;
}

void CParticleSystem::LoadReset()
{
	
	m_vLocation			= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vPrevLocation		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vMoveLocation		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vAttractLocation	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

	m_bIsSuppressed		= true;
	m_bIsRender			= true;
}

BOOL	CParticleSystem::UpdateLink( FLOAT fTimeDelta, BOOL IsInit )
{
	int nEffSetCon = m_iEffSetCon;
	int nEffType = ESLT_NONE;
	int nEffSetType = -1;	

	if( nEffSetCon != -1 )
	{
		nEffType = g_Particle.m_pEffSetCon[ nEffSetCon ].m_iType;
		nEffSetType = g_Particle.m_pEffSetCon[ nEffSetCon ].m_iSetType;
	}	
	
	m_Timer.Update(fTimeDelta);
	
	m_uAliveParticle = 0;
	
	std::list	<CParticle *>::iterator i = m_ParticleList.begin();
	
	for( ; i!= m_ParticleList.end() ; )
	{
		if( (*i)->Update( fTimeDelta ) == FALSE )
		{
			CParticle * pData = (*i);		

			m_ParticlePool.DeleteMember( pData );
		    
			(*i) = NULL;
			
			m_ParticleList.erase( i++ );
		}
		else
		{
			m_uAliveParticle++;
			
			i++;
		}
	}
	
	m_fAge += fTimeDelta;
	
	if( m_pReference->m_fTotalLife != 0 )
	{
		if( m_bIsCreate )
		{
			if( m_fAge >= m_pReference->m_fTotalLife )
			{
				m_bIsCreate = FALSE;
				m_bIsSuppressed = TRUE;
			}
		}
	}
	
	if( m_bIsCreate )
	{
		static D3DXMATRIX	mat;
		static float		height	= 0.0f;
		static float		rad		= 0.0f;
		static D3DXVECTOR3	loc		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		 
		
		D3DXMatrixIdentity(&mat);
		height = 0.0f;
		rad = 0.0f;
		loc.x = loc.y = loc.z = 0.0f;
		
		if( m_lUnique != -1 )
		{
			if( m_pSrc != NULL )
			{
				// 보이지 않는 캐릭터이면 시간만 때우기
				if( m_pSrc->m_bIsVisible == FALSE )
					return TRUE;
				
				if( m_pSrc->m_nTableIndex == -1 )
				{
					m_bIsSuppressed = TRUE;
					
					return FALSE;
				}
				
				rad		= m_pSrc->GetDirection();				
				mat		= m_pSrc->GetBonCenterTM();				
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
				
				if( m_iLink == 0 && m_aDirectLinkBone == 0 )
				{
					loc.x = mat._41;
					loc.y = mat._42;
					loc.z = mat._43;
				}				
			}
			else
			{
				m_bIsSuppressed = TRUE;
				
				return FALSE;
			}
		}
		else
		{
			if(!m_IsPosFix)
			{
				loc = m_vStartPos;
			}
		}
		
		if( m_pReference->m_bIsMoving )
		{
			int x;
			if( m_pReference->m_iMovingDir == 0 )
			{
				x = 1;
			}
			else if ( m_pReference->m_iMovingDir == 1 )
			{
				x = -1;
			}
			else
				x = 0;
			float first;
			float second;
			
			fThetaHoriz += 50.0f * fTimeDelta * m_pReference->m_fMovingTime * x;
			fThetaVert  += 30.0f * fTimeDelta * m_pReference->m_fMovingTime * x;
			
			if ( fThetaHoriz >= 360.0f )
				fThetaHoriz -= 360.0f;
			
			if ( fThetaVert >= 360.0f ) 
				fThetaVert -= 360.0f;
			
			if( m_pReference->m_iMovingSpeedType == 0 )
			{
				first = fThetaHoriz* 0.01745f;
				second = fThetaVert* 0.01745f;
			}
			else
			{
				second = fThetaHoriz* 0.01745f;
				first = fThetaVert* 0.01745f;
			}
			
			if( m_pReference->m_iMovingType != 5 )
			{
				m_vLocation.x = 0.0f;
				m_vLocation.y = 0.0f;
				m_vLocation.z = (m_pReference->m_fMovingRadius);
			}
			else
			{
				m_vLocation.x = 0.0f;
				m_vLocation.y = (m_pReference->m_fMovingRadius);
				m_vLocation.z = 0.0f;
			}
			
			static D3DXMATRIX matWorld;
			static D3DXMATRIX matWorldtemp;
			
			D3DXMatrixTranslation(&matWorld, m_vLocation.x, 
				m_vLocation.y,
				m_vLocation.z );
			
			if( m_pReference->m_iMovingType == 0 )
			{
				D3DXMatrixRotationY( &matWorldtemp, first );
				D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
			}
			else if( m_pReference->m_iMovingType == 1 )
			{
				D3DXMatrixRotationY( &matWorldtemp, first );
				D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
				D3DXMatrixRotationX( &matWorldtemp, second );
				D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
			}
			else if( m_pReference->m_iMovingType == 2 )
			{
				D3DXMatrixRotationY( &matWorldtemp, first );
				D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
				D3DXMatrixRotationZ( &matWorldtemp, second );
				D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
			}
			else if( m_pReference->m_iMovingType == 3 )
			{
				D3DXMatrixRotationX( &matWorldtemp, first );
				D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
				D3DXMatrixRotationY( &matWorldtemp, second );
				D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
			}
			else if( m_pReference->m_iMovingType == 4 )
			{
				D3DXMatrixRotationX( &matWorldtemp, first );
				D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
				D3DXMatrixRotationZ( &matWorldtemp, second );
				D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
			}
			else if( m_pReference->m_iMovingType == 5 )
			{
				D3DXMatrixRotationZ( &matWorldtemp, first );
				D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
			}
			
			m_vLocation.x = matWorld._41;
			m_vLocation.y = matWorld._42;
			m_vLocation.z = matWorld._43;
		}
		else
		{
			if(m_pReference->m_iLocation_Select == 0 )
			{
				m_vLocation.x = 0.0f;
				m_vLocation.y = 0.0f;
				m_vLocation.z = 0.0f;
			}
			
			if(m_pReference->m_iLocation_Select == 1 )
			{
				float	x = RANDOM_NUM;
				m_vLocation.x = m_pReference->m_fRadius * (float)cos( x * 180.0f );
				m_vLocation.y = 0.0f;
				m_vLocation.z = m_pReference->m_fRadius * (float)sin( x * 180.0f );
			}
			
			if(m_pReference->m_iLocation_Select == 2 )
			{
				m_vLocation.x = RANDOM_NUM * m_pReference->m_fPlane_x;
				m_vLocation.y = RANDOM_NUM * m_pReference->m_fPlane_y;
				m_vLocation.z = RANDOM_NUM * m_pReference->m_fPlane_z;
			}
			
		}
		
		D3DXMATRIX matWorld;
		D3DXMATRIX matWorldtemp;
		D3DXMatrixTranslation(&matWorld, m_vLocation.x, 
			m_vLocation.y,
			m_vLocation.z );	
		D3DXMatrixRotationY( &matWorldtemp, m_fRotTheta );
		D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldtemp );
		m_vLocation.x = matWorld._41;
		m_vLocation.y = matWorld._42;
		m_vLocation.z = matWorld._43;
		
		float delta = fTimeDelta;
		if( fTimeDelta > 0.5f )
			delta = 0.002f;
		
		if(IsInit)
		{
			delta = 1.0f;
		}
		
		float NeededPar = m_pReference->fParticlesNeeded;
		if( NeededPar > 6.0f )
		{
			NeededPar = 6.0f;
		}
		NeededPar += m_pReference->m_uParticlesPerSec * delta;
		
		///파티클 시스템과 카메라의 간격을 계산한다. -- 생성파티클개수 재조정
		//---------------------------------------------------------------------------
		//---------------------------------------------------------------------------
		BOOL bParCreate = TRUE;
		
		if( g_RockClient.GetGameMode() == GMODE_NORMAL )
		{
			D3DXVECTOR3 vPos = m_vLocation;
			
			vPos	+= loc;
			
			if( !m_IsDirAttatack )
			{
				vPos.y	+= height * m_pReference->m_fHeight;
			}		
			
			vPos	+= m_vMoveLocation;		
			
			if(m_IsPosFix)
			{
				vPos.x += m_LinkMat._41;
				vPos.y += m_LinkMat._42;
				vPos.z += m_LinkMat._43;
			}		
			
			vPos.y = 0.0f;
			
			D3DXVECTOR3 vPlayerPos = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41,
				0.0f,
				g_Pc.GetPlayer()->GetPosTM()._43 );
			
			m_nPlayerDistance = (int)D3DXVec3Length( &(vPlayerPos - vPos) );
			
			if( m_nPlayerDistance > g_Max_Effect_Display )
			{
				bParCreate = FALSE;
				m_fEmissionResidue = 0.0f;
			}
			
			float fRatio = 0.0f;
			fRatio = (m_nPlayerDistance - 24) / (float)g_Max_Effect_Display;
			
			if( m_IsParticleApplyDis )
			{
				if( g_IsEffectMinimize )
				{
					if(fRatio < 0.1f)
					{
						fRatio = 0.1f; 
					}
					
					NeededPar -= ((NeededPar * fRatio) * 0.75f * 5.0f);	
				}
				else
				{
					if(fRatio < 0.0f)
					{
						fRatio = 0.0f; 
					}
					
					NeededPar -= ((NeededPar * fRatio) * 0.75f);	
				}
			}
			
			if( NeededPar < 0.01f )
			{
				NeededPar = 0.01f;
			}
		}
		
		NeededPar += m_fEmissionResidue;
		//---------------------------------------------------------------------------
		//---------------------------------------------------------------------------
		
		unsigned int uParticlesCreated = (unsigned int)NeededPar;
		
		if ( !m_bIsSuppressed )
		{
			m_fEmissionResidue = NeededPar - (float)uParticlesCreated;
		}
		else
		{
			m_fEmissionResidue = NeededPar;
			uParticlesCreated = 0;
		}
		
		if( m_bIsRender && bParCreate )
		{
			for( unsigned int i = 0; i < uParticlesCreated; i++ )
			{
				CParticle * pAddParticle = NULL;
				
				pAddParticle = m_ParticlePool.NewMember();
				
				pAddParticle->m_fAge		=	0.0f;
				pAddParticle->m_fLifetime = m_pReference->m_fLife + RANDOM_NUM * m_pReference->m_fLifeVar;
				Clamp( pAddParticle->m_fLifetime, MIN_LIFETIME, MAX_LIFETIME );
				
				float	ReverseLife = 1.0f / pAddParticle->m_fLifetime;
				
				pAddParticle->m_Color.r = m_pReference->m_ColorStart.r + RANDOM_NUM * m_pReference->m_ColorVar.r;
				pAddParticle->m_Color.g = m_pReference->m_ColorStart.g + RANDOM_NUM * m_pReference->m_ColorVar.g;
				pAddParticle->m_Color.b = m_pReference->m_ColorStart.b + RANDOM_NUM * m_pReference->m_ColorVar.b;
				
				pAddParticle->m_Color.a = 3.0f;
				
				Clamp( pAddParticle->m_Color.r, 0.0f, 3.0f );
				Clamp( pAddParticle->m_Color.g, 0.0f, 3.0f );
				Clamp( pAddParticle->m_Color.b, 0.0f, 3.0f );
				
				pAddParticle->m_ColorDelta.r = (m_pReference->m_ColorEnd.r - pAddParticle->m_Color.r) * ReverseLife;
				pAddParticle->m_ColorDelta.g = (m_pReference->m_ColorEnd.g - pAddParticle->m_Color.g) * ReverseLife;
				pAddParticle->m_ColorDelta.b = (m_pReference->m_ColorEnd.b - pAddParticle->m_Color.b) * ReverseLife;
				
				pAddParticle->m_fAlpha = m_pReference->m_fAlphaStart + RANDOM_NUM * m_pReference->m_fAlphaVar;
				Clamp( pAddParticle->m_fAlpha, MIN_ALPHA, MAX_ALPHA );
				pAddParticle->m_fAlphaDelta = (m_pReference->m_fAlphaEnd - pAddParticle->m_fAlpha) * ReverseLife;
				
				pAddParticle->m_fSize = m_pReference->m_fSizeStart + RANDOM_NUM * m_pReference->m_fSizeVar;
				Clamp( pAddParticle->m_fSize, MIN_SIZE, MAX_SIZE );
				pAddParticle->m_fSizeDelta = (m_pReference->m_fSizeEnd - pAddParticle->m_fSize) * ReverseLife;
				
				pAddParticle->m_fSize		*= m_fSize;
				pAddParticle->m_fSizeDelta	*= m_fSize;
				
				pAddParticle->m_vGravity = m_pReference->m_vGravityStart + RANDOM_NUM * m_pReference->m_vGravityVar;
				pAddParticle->m_vGravityDelta = ( m_pReference->m_vGravityEnd - pAddParticle->m_vGravity ) * ReverseLife;
				
				pAddParticle->m_vGravity		*= m_fSize;
				pAddParticle->m_vGravityDelta	*= m_fSize;
				
				D3DXVECTOR3 tempVelocity = (m_vLocation - m_vPrevLocation)/fTimeDelta;
				pAddParticle->m_vLocation = m_vPrevLocation + tempVelocity * RANDOM_NUM * fTimeDelta;
				
				float RandomYaw = RANDOM_NUM * 6.28318f;
				float RandomPitch = RANDOM_NUM * m_pReference->m_fTheta * 0.01745f ;
				
				pAddParticle->m_vVelocity.y = cosf(RandomPitch);
				pAddParticle->m_vVelocity.x = sinf(RandomPitch) * cosf(RandomYaw);
				pAddParticle->m_vVelocity.z = sinf(RandomPitch) * sinf(RandomYaw);
				
				if(m_bApplyVel)
				{
					pAddParticle->m_vDirection = m_vVelocity;
					pAddParticle->m_vDirection *= m_fSize;					
				}
				
				float fNewSpeed = m_pReference->m_fSpeed + RANDOM_NUM * m_pReference->m_fSpeedVar;
				Clamp( fNewSpeed, MIN_SPEED, MAX_SPEED );
				pAddParticle->m_vVelocity *= fNewSpeed ;
				
				pAddParticle->m_vVelocity *= m_fSize;
				
				pAddParticle->SetParent(this);
				
				pAddParticle->m_vLocation	+= loc;
				
				if( !m_IsDirAttatack )
				{
					pAddParticle->m_vLocation.y	+= height * m_pReference->m_fHeight;
				}
				else
				{
					pAddParticle->m_vLocation.y	+= height;
				}
				
				pAddParticle->m_vLocation	+= m_vMoveLocation;
				
				m_ParticleList.push_back( pAddParticle );
			}
			
			m_vPrevLocation = m_vLocation;
		}
	}
	
	///-- 한번만 실행되는 케이스
	if( m_pReference->m_bIsInstantPlay )
	{
		m_bIsCreate = false;
		number++;
		if(( number > 1 )&&( m_uAliveParticle == 0 ))
		{
			m_bIsSuppressed = true;
			number = 0;
		}
	}
	
	i = m_ParticleList.begin();
	
	D3DXMATRIX matWorld;
	D3DXVECTOR3	v;
	
	for( ; i!= m_ParticleList.end() ; ++i )
	{
		CParticle *pData = (*i);		
		
		D3DXMatrixIdentity( &matWorld );
		
		v = pData->m_vLocation;			

		if(m_IsPosFix)
		{
			D3DXVec3TransformCoord( &v, &(pData->m_vLocation), &m_LinkMat );
		}
			
		D3DXMatrixTranslation( &(pData->matLocal), v.x, v.y, v.z );				
		
		if( m_pReference->m_iRenderingLev != LINE_RENDER )
			D3DXMatrixMultiply( &(pData->matLocal), &g_Particle.m_matInverseCamera, &(pData->matLocal) );
	}
	
	m_bLinkOk = false;
	
	return	TRUE;
}
