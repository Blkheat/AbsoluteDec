#include "CEFFireWork.h"
#include "..\\..\\rocklibs\\RockInterface\\Rui.h"
#include "..\\Effect.h"
#include "..\\Camera.h"
#include <SOUND\Sound.H>
#include "..\\..\\CRenderManager.h"
#include "..\\..\\StringManager.h"
#include <list>

using namespace std;

extern long g_nowTime;

CEffectModel CEFFireWork::m_BaseModel;

CEFFireWork::CEFFireWork()
{
	m_State = FIREWORK_WAIT;

	m_pvStartPosList = NULL;
	m_pvEndPosList = NULL;
}

CEFFireWork::~CEFFireWork()
{	
	SAFE_DELETE_ARRAY(m_pvStartPosList);
	SAFE_DELETE_ARRAY(m_pvEndPosList);
}

int CEFFireWork::InitDevice(void)
{
	CStringToCode	HashString( "firework.tga" );
	static unsigned long HashCode = HashString.GetHashCode();
	
	CEFFireWork::m_BaseModel.InitDevice( "firework.msh" , HashCode );

	return TRUE;
}

int CEFFireWork::DeleteDevice(void)
{
	CEFFireWork::m_BaseModel.DeleteDevice();

	return TRUE;
}


void CEFFireWork::Init(void)
{
	D3DXVECTOR3 vChrFront;
	
	vChrFront.x   = m_pSrcChr->GetRealRHandTM()._41 + m_pSrcChr->m_vDir.x * 8.0f;
	vChrFront.z   = m_pSrcChr->GetRealRHandTM()._43 + m_pSrcChr->m_vDir.z * 8.0f;
	vChrFront.y   = g_Map.Get__MapHeightF( vChrFront.x , vChrFront.z );

	m_vStartPos   = vChrFront;
	m_vDestPos    = m_vStartPos;

	switch( m_ID )
	{
		case EF_FIREFLOWER0:
			m_SpotParticleNum = 701;
			m_vDestPos.y += 60.0f;
			m_RangeTime	  = 1000;
			break;
		case EF_FIREFLOWER1:
			m_SpotParticleNum = 704;
			m_vDestPos.y += 45.0f;
			m_RangeTime	  = 1000;
			break;
		case EF_FIREHEARTFLOWER:
		case EF_FIREENGLOVEFLOWER:
		case EF_FIRENATLOVEFLOWER:	
			m_SpotParticleNum = 24665;
			m_vDestPos.y += 45.0f;
			m_RangeTime	  = 1000;
			break;
	}
	
	m_vDistance   = m_vDestPos - m_vStartPos;
	m_vPos        = m_vStartPos;

	int AddColor = Random( 0 , 2 );
	
	m_SpotParticleNum += AddColor;	

	m_TailParticleNum = 707 + AddColor;	

	m_Count       = 0;
	m_OldTimer    = g_nowTime;
}

int CEFFireWork::Render(float dtime)
{
	if( m_State == FIREWORK_WAIT )
	{
		D3DXMATRIX matLocal;

		D3DXMatrixIdentity( &matLocal );		

		matLocal._41 = m_vStartPos.x + Random( -0.2f , 0.2f );
		matLocal._42 = m_vStartPos.y + Random( -0.2f , 0.2f );
		matLocal._43 = m_vStartPos.z + Random( -0.2f , 0.2f );
	
		g_RenderManager.SetTransform( D3DTS_WORLD , &matLocal );
		
		g_RenderManager.SetCullMode( D3DCULL_CCW );
		
		g_RenderManager.SetZWriteEnable( TRUE );
		g_RenderManager.SetZEnable(TRUE );
		
		g_RenderManager.SetAlphaBlendEnable( FALSE );
		
		CEFFireWork::m_BaseModel.Render();
	}

	return TRUE;
}

int CEFFireWork::Update(float dtime)
{
	switch( m_State )
	{
		case FIREWORK_WAIT:
			
			if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) , >= , 1000 ) )
			{
				m_State = FIREWORK_SHOT;

				m_OldTimer = g_nowTime;
			
				g_Particle.DisplayUpdateNotApplyDis( 710 , m_vPos );
				g_Sound.Play( 292 , m_vPos.x , m_vPos.y , m_vPos.z );
			}
			
			return TRUE;
		case FIREWORK_BOOM:
			{
				
			}
			return TRUE;
		case FIREWORK_HEART_BOOM:
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) , >= , 1000 ) )
				{
					///-- Boom_Heart_FireFlower();
					SetDestroy();
				}
			}
			return TRUE;
		case FIREWORK_BIG_BOOM1:
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) , >= , 250 ) )
				{
					m_State     = FIREWORK_BIG_BOOM2;			
					m_OldTimer = g_nowTime;	
				
					D3DXMATRIX matTrans;	
					D3DXMatrixTranslation( &matTrans, m_vPos.x, m_vPos.y + 20.0f, m_vPos.z );
					
					D3DXVECTOR3 vMaxRad(0.0f, 55.0f, 0.0f);
					D3DXVECTOR3 vMinRad(0.0f, 5.0f, 0.0f);
					
					for( int i = 0 ; i < 10 ; i++ )
					{
						float fRad = (2 * D3DX_PI) * ( i / 10.0f ) + (D3DX_PI / 10.0f);
						D3DXMATRIX	matRot;
						D3DXMatrixRotationZ( &matRot, fRad );
						D3DXVec3TransformCoord( &m_pvStartPosList[i] , &vMinRad , &matRot );
						D3DXVec3TransformCoord( &m_pvEndPosList[i] , &vMaxRad , &matRot );

						D3DXMatrixRotationY( &matRot, -g_Camera.m_fCameraYaw );
						D3DXVec3TransformCoord( &m_pvStartPosList[i] , &m_pvStartPosList[i] , &matRot );
						D3DXVec3TransformCoord( &m_pvEndPosList[i] , &m_pvEndPosList[i] , &matRot );

						D3DXVec3TransformCoord( &m_pvStartPosList[i] , &m_pvStartPosList[i] , &matTrans );
						D3DXVec3TransformCoord( &m_pvEndPosList[i] , &m_pvEndPosList[i] , &matTrans );
					}

					g_Sound.Play( 293 , m_vPos.x , m_vPos.y , m_vPos.z );
				}
				else
				{
					DWORD dGapTime = SAFE_TIME_SUB(g_nowTime, m_OldTimer);
					float fRatio = (float)dGapTime / 250.0f;

					if( fRatio > 1.0f )
					{
						fRatio = 1.0f;
					}

					D3DXVECTOR3 vTempPos;				

					for( int i = 0 ; i < 10 ; i++ )
					{
						int nSpotPNum = 701 + Random(0, 2);
						D3DXVec3Lerp( &vTempPos , &m_pvStartPosList[i] , &m_pvEndPosList[i] , fRatio );
						g_Particle.DisplayUpdateNotApplyDis( nSpotPNum , vTempPos );
					}				
				}
			}
			return TRUE;
		case FIREWORK_BIG_BOOM2:
			{
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) , >= , 500 ) )
				{
					SetDestroy();	
				}
				else
				{
					DWORD dGapTime = SAFE_TIME_SUB(g_nowTime, m_OldTimer);
					float fRatio = (float)dGapTime / 500.0f;

					if( fRatio > 1.0f )
					{
						fRatio = 1.0f;
					}

					D3DXVECTOR3 vTempPos;			

					for( int i = 0 ; i < 10 ; i++ )
					{
						int nSpotPNum = 701 + Random(0, 2);
						D3DXVec3Lerp( &vTempPos , &m_pvStartPosList[i] , &m_pvEndPosList[i] , fRatio );
						g_Particle.DisplayUpdateNotApplyDis( nSpotPNum , vTempPos );
					}			
				}
			}
			return TRUE;
		case FIREWORK_SMALL_BOOM1:
			{	
				if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_OldTimer ) , >= , 500 ) )
				{
					D3DXMATRIX matLocal;
					D3DXMATRIX matTrans;
					D3DXVECTOR3 vTemp;

					D3DXMatrixTranslation( &matTrans, m_vPos.x, m_vPos.y + 15.0f, m_vPos.z );

					float fRadius = 30;
					float fDelRadian = D3DX_PI / 18.0f;

					int nSpotPNum = 704 + Random(0, 2);
					
					for( int i = 0; i < 36 ; i++ )
					{
						D3DXMatrixIdentity( &matLocal );
						
						float fRadian = fDelRadian * (float)i;
						vTemp.y = sin( fRadian ) * fRadius;
						vTemp.x = cos( fRadian ) * fRadius;
						vTemp.z = 0.0f;

						///-- 여기에 빌보드 행렬을 곱해준다.
						D3DXMATRIX matRotY;			
						D3DXMatrixRotationY( &matRotY , -g_Camera.m_fCameraYaw );
						D3DXMatrixMultiply( &matLocal, &matLocal, &matRotY );				
						///-- 이것을 불꽃을 터트리는 부분으로 옯겨준다.		
						D3DXMatrixMultiply( &matLocal, &matLocal, &matTrans );

						///--구해진 위치를 로컬로 옮겨준다.
						D3DXVec3TransformCoord(&vTemp, &vTemp, &matLocal);

						g_Particle.DisplayUpdateNotApplyDis( nSpotPNum , vTemp );			
					}
			
					g_Sound.Play( 293 , m_vPos.x , m_vPos.y , m_vPos.z );
					
					SetDestroy();	
				}	
			}
			return TRUE;
	}
	
	float Range = ( ( ( float ) SAFE_TIME_SUB( g_nowTime , m_OldTimer ) / ( float ) m_RangeTime ) );
				
	if( Range > 1.0f ) 
	{
		Range = 1.0f;
		
		m_vPos = m_vStartPos + ( m_vDistance * Range );
		
		if( m_ID == EF_FIREHEARTFLOWER )
		{
			Boom_Heart_FireFlower();
			
		    m_State = FIREWORK_HEART_BOOM;
			m_OldTimer = g_nowTime;
		}
		else if( m_ID == EF_FIREFLOWER0 ) 
		{
			m_State     = FIREWORK_BIG_BOOM1;			
			m_OldTimer = g_nowTime;	
			
			m_pvStartPosList = SAFE_NEW_ARRAY( D3DXVECTOR3, 10 );
			m_pvEndPosList = SAFE_NEW_ARRAY( D3DXVECTOR3, 10 );

			D3DXMATRIX matTrans;	
			D3DXMatrixTranslation( &matTrans, m_vPos.x, m_vPos.y + 20.0f, m_vPos.z );
			
			D3DXVECTOR3 vMaxRad(0.0f, 35.0f, 0.0f);
			D3DXVECTOR3 vMinRad(0.0f, 5.0f, 0.0f);
			
			for( int i = 0 ; i < 10 ; i++ )
			{
				float fRad = (2 * D3DX_PI) * ( i / 10.0f );
				D3DXMATRIX	matRot;
				D3DXMatrixRotationZ( &matRot, fRad );
				D3DXVec3TransformCoord( &m_pvStartPosList[i] , &vMinRad , &matRot );
				D3DXVec3TransformCoord( &m_pvEndPosList[i] , &vMaxRad , &matRot );

				D3DXMatrixRotationY( &matRot, -g_Camera.m_fCameraYaw );
				D3DXVec3TransformCoord( &m_pvStartPosList[i] , &m_pvStartPosList[i] , &matRot );
				D3DXVec3TransformCoord( &m_pvEndPosList[i] , &m_pvEndPosList[i] , &matRot );

				D3DXVec3TransformCoord( &m_pvStartPosList[i] , &m_pvStartPosList[i] , &matTrans );
				D3DXVec3TransformCoord( &m_pvEndPosList[i] , &m_pvEndPosList[i] , &matTrans );
			}

			g_Sound.Play( 293 , m_vPos.x , m_vPos.y , m_vPos.z );
		}
		else if( m_ID == EF_FIREFLOWER1 )
		{
			m_State     = FIREWORK_SMALL_BOOM1;			
			m_OldTimer = g_nowTime;

			D3DXMATRIX matLocal;
			D3DXMATRIX matTrans;
			D3DXVECTOR3 vTemp;

			D3DXMatrixTranslation( &matTrans, m_vPos.x, m_vPos.y + 15.0f, m_vPos.z );

			float fRadius = 20;
			float fDelRadian = D3DX_PI / 18.0f;

			int nSpotPNum = 704 + Random(0, 2);
			
			for( int i = 0; i < 36 ; i++ )
			{
				D3DXMatrixIdentity( &matLocal );
				
				float fRadian = fDelRadian * (float)i;
				vTemp.y = sin( fRadian ) * fRadius;
				vTemp.x = cos( fRadian ) * fRadius;
				vTemp.z = 0.0f;

				///-- 여기에 빌보드 행렬을 곱해준다.
				D3DXMATRIX matRotY;			
				D3DXMatrixRotationY( &matRotY , -g_Camera.m_fCameraYaw );
				D3DXMatrixMultiply( &matLocal, &matLocal, &matRotY );				
				///-- 이것을 불꽃을 터트리는 부분으로 옯겨준다.		
				D3DXMatrixMultiply( &matLocal, &matLocal, &matTrans );

				///--구해진 위치를 로컬로 옮겨준다.
				D3DXVec3TransformCoord(&vTemp, &vTemp, &matLocal);

				g_Particle.DisplayUpdateNotApplyDis( nSpotPNum , vTemp );			
			}
	
			g_Sound.Play( 293 , m_vPos.x , m_vPos.y , m_vPos.z );
		}
		else if( m_ID == EF_FIREENGLOVEFLOWER ||
				 m_ID == EF_FIRENATLOVEFLOWER )
		{
			SStringInfo	tempStrInfo;
			
			static FONTID IdxBuf[256];	
			SFont* tmpFont = NULL;
			int NumStr = 0;

			if( m_ID == EF_FIREENGLOVEFLOWER )
			{
				NumStr = nRui->GetFontg()->Confirm( RCHAR(G_STRING_CHAR( IDS_ENG_ILOVEYOU )) , IdxBuf );			
				nRui->GetFontg()->GetStringInfo( RCHAR(G_STRING_CHAR( IDS_ENG_ILOVEYOU )) , &tempStrInfo );
			}
			else
			{
				NumStr = nRui->GetFontg()->Confirm( RCHAR(G_STRING_CHAR( IDS_NAT_ILOVEYOU )), IdxBuf );			
				nRui->GetFontg()->GetStringInfo( RCHAR(G_STRING_CHAR( IDS_NAT_ILOVEYOU )) , &tempStrInfo );
			}

			int nStartX = -( tempStrInfo.Width / 2 );
			
			for(int ni = 0 ; ni < NumStr ; ni++ )
			{
				if( IdxBuf[ni] == 32 )
				{
					nStartX += 3; 
				}
			}
			
			for(int ni = 0 ; ni < NumStr ; ni++ )
			{
				tmpFont = nRui->GetFontg()->GetFontasm( IdxBuf[ni] );			
				
				if( tmpFont )
				{
					list < POINT > posList;

					BYTE* tmpFontData;
					WORD* TexData;

					int nTextureSize = tmpFont->nTexWidth;
					int nWidth = tmpFont->Riw;
					int nHeight = tmpFont->Rih;
						
					//버퍼에서 폰트 텍스쳐로 복사
					D3DLOCKED_RECT FontTextureRect;
					tmpFont->pTexture->LockRect( 0, &FontTextureRect, NULL, 0 );
					tmpFontData = (BYTE*)FontTextureRect.pBits;			

					for( int i = 0; i < nTextureSize; ++i )
					{
						TexData = (WORD*)( &tmpFontData[ i * FontTextureRect.Pitch ] );
						
						for( int j = 0; j < nTextureSize; ++j )//width
						{
							WORD dValue = TexData[j];
							if( dValue != 0x0fff )
							{
								POINT Pos;
								Pos.x = j;
								Pos.y = ( nTextureSize -1 ) - i;
								posList.push_back(Pos);
							}					
						}		
					}

					tmpFont->pTexture->UnlockRect( 0 );
			
					list < POINT > ::iterator ipos = posList.begin();	
					
					D3DXMATRIX matLocal;
					D3DXMATRIX matTrans;
					D3DXVECTOR3 vTemp;		
					
					D3DXMatrixTranslation( &matTrans, m_vPos.x, m_vPos.y + 15.0f, m_vPos.z );
					
					for( ; ipos != posList.end() ; ipos++ )
					{
						D3DXMatrixIdentity( &matLocal );
						
						POINT Pos = (POINT)(*ipos);
						///-- 3D 위치로 전환
						vTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					
						vTemp.x = ( nStartX + Pos.x ) * 2.0f;
						vTemp.y = ( Pos.y - (nHeight / 2) ) * 2.0f;
		
						///-- 여기에 빌보드 행렬을 곱해준다.
						D3DXMATRIX matRotY;			
						D3DXMatrixRotationY( &matRotY , -g_Camera.m_fCameraYaw );
						D3DXMatrixMultiply( &matLocal, &matLocal, &matRotY );				
						///-- 이것을 불꽃을 터트리는 부분으로 옯겨준다.		
						D3DXMatrixMultiply( &matLocal, &matLocal, &matTrans );

						///--구해진 위치를 로컬로 옮겨준다.
						D3DXVec3TransformCoord(&vTemp, &vTemp, &matLocal);

						int nSpotPNum = 24668 + Random(0, 2);						
						int IsBoom = Random(0, 1);
						
						if( IsBoom )
						{
							int nSpotPNum2 = 704 + Random(0, 2);						
							g_Particle.DisplayUpdateNotApplyDis( nSpotPNum2 , vTemp );
						}
						g_Particle.DisplayUpdateNotApplyDis( nSpotPNum , vTemp );				
					}

					posList.clear();					
				}

				if( IdxBuf[ni] == 32 )
				{
					nStartX += 3;
				}
				else
				{
					nStartX += tmpFont->w;
				}
			}

			g_Sound.Play( 293 , m_vPos.x , m_vPos.y , m_vPos.z );
			
			SetDestroy();
		}

		return TRUE;
	}

	m_vPos = m_vStartPos + ( m_vDistance * Range );  
	
	switch( m_ID )
	{
		case EF_FIREFLOWER0:
			m_vPos.x += sin( Range * 20.0f ) * 3.0f;
			m_vPos.z += cos( Range * 20.0f ) * 3.0f;
			break;

		case EF_FIREFLOWER1:
		case EF_FIREHEARTFLOWER:
		case EF_FIREENGLOVEFLOWER:
		case EF_FIRENATLOVEFLOWER:	
			m_vPos.x += sin( Range * 15.0f ) * 2.0f;
			m_vPos.z += cos( Range * 15.0f ) * 2.0f;
			break;
	}
	
	g_Particle.DisplayUpdateNotApplyDis( m_TailParticleNum , m_vPos );

	return TRUE;
}

void CEFFireWork::Boom_Heart_FireFlower()
{
	///-- 하트 포지션으로 위치를 구해서 파티클을 터트려 준다.
	D3DXMATRIX matLocal;
	D3DXMATRIX matTrans;
	D3DXMATRIX matTrans2;
	D3DXMatrixIdentity( &matLocal );
	D3DXMatrixTranslation( &matTrans, m_vPos.x, m_vPos.y + 20.0f, m_vPos.z );
	D3DXMatrixTranslation( &matTrans2, m_vPos.x, m_vPos.y + 25.0f, m_vPos.z );

	///-- 시작위치
	float fSatrRadius = 0.0f;
	float fEndRadius = 20.0f;

	float fSatrRadius2 = 5.0f;
	float fEndRadius2 = 25.0f;

	float fDelRadian = D3DX_PI / 18.0f;

	float fStartRadian = ( D3DX_PI / 2.0f );
				
	D3DXVECTOR3 vTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			

	for( int i = 0; i <= 18 ; i++ )
	{
		D3DXMatrixIdentity( &matLocal );
		
		float fRadius = fSatrRadius + fEndRadius * ( (float)i / 18.0f) + i;
		float fRadian = fStartRadian - fDelRadian * (float)i;
		float fRadius2 = fSatrRadius2 + fEndRadius2 * ( (float)i / 18.0f) + i;

		vTemp.y = sin( fRadian ) * fRadius;
		vTemp.x = cos( fRadian ) * fRadius;
		vTemp.z = 0.0f;

		///-- 여기에 빌보드 행렬을 곱해준다.
		D3DXMATRIX matRotY;			
		D3DXMatrixRotationY( &matRotY , -g_Camera.m_fCameraYaw );
		D3DXMatrixMultiply( &matLocal, &matLocal, &matRotY );				
		///-- 이것을 불꽃을 터트리는 부분으로 옯겨준다.		
		D3DXMatrixMultiply( &matLocal, &matLocal, &matTrans );

		///--구해진 위치를 로컬로 옮겨준다.
		D3DXVec3TransformCoord(&vTemp, &vTemp, &matLocal);

		g_Particle.DisplayUpdateNotApplyDis( m_SpotParticleNum , vTemp );

		vTemp.y = sin( fRadian ) * fRadius2;
		vTemp.x = cos( fRadian ) * fRadius2;
		vTemp.z = 0.0f;

		D3DXMatrixIdentity( &matLocal );		
		D3DXMatrixMultiply( &matLocal, &matLocal, &matRotY );
		D3DXMatrixMultiply( &matLocal, &matLocal, &matTrans2 );

		int nSpotPNum = 24665 + Random(0, 2);
		
		///--구해진 위치를 로컬로 옮겨준다.
		D3DXVec3TransformCoord(&vTemp, &vTemp, &matLocal);
		g_Particle.DisplayUpdateNotApplyDis( nSpotPNum , vTemp );
	}

	for(int i = 0; i <= 18 ; i++ )
	{
		if( i == 0 || i == 18 )
		{
			continue;
		}
		
		D3DXMatrixIdentity( &matLocal );
		
		float fRadius = fSatrRadius + fEndRadius * ( (float)i / 18.0f) + i;
		float fRadian = fStartRadian + fDelRadian * ( (float)i );
		float fRadius2 = fSatrRadius2 + fEndRadius2 * ( (float)i / 18.0f) + i;

		vTemp.y = sin( fRadian ) * fRadius;
		vTemp.x = cos( fRadian ) * fRadius;
		vTemp.z = 0.0f;

		///-- 여기에 빌보드 행렬을 곱해준다.				
		D3DXMATRIX matRotY;			
		D3DXMatrixRotationY( &matRotY , -g_Camera.m_fCameraYaw );
		D3DXMatrixMultiply( &matLocal, &matLocal, &matRotY );				
		///-- 이것을 불꽃을 터트리는 부분으로 옯겨준다.		
		D3DXMatrixMultiply( &matLocal, &matLocal, &matTrans );

		///--구해진 위치를 로컬로 옮겨준다.
		D3DXVec3TransformCoord(&vTemp, &vTemp, &matLocal);

		g_Particle.DisplayUpdateNotApplyDis( m_SpotParticleNum , vTemp );

		vTemp.y = sin( fRadian ) * fRadius2;
		vTemp.x = cos( fRadian ) * fRadius2;
		vTemp.z = 0.0f;

		D3DXMatrixIdentity( &matLocal );		
		D3DXMatrixMultiply( &matLocal, &matLocal, &matRotY );
		D3DXMatrixMultiply( &matLocal, &matLocal, &matTrans2 );

		int nSpotPNum = 24665 + Random(0, 2);
		
		///--구해진 위치를 로컬로 옮겨준다.
		D3DXVec3TransformCoord(&vTemp, &vTemp, &matLocal);
		g_Particle.DisplayUpdateNotApplyDis( nSpotPNum , vTemp );
	}

	g_Sound.Play( 293 , m_vPos.x , m_vPos.y , m_vPos.z );
}

