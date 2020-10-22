#include "RockPCH.h"
#include "RockWater.h"
#include "..\\CRenderManager.h"

#define	RW_MAX( x, y )		( ( x >= y )? x:y )
#define	RW_MIN( x, y )		( ( x < y )? x:y )

struct	FVF_ROCKWATER
{
	float	x, y, z;
	DWORD	color;
	float	u0, v0;
	float	u1, v1;
};

#define	D3DFVF_ROCKWATER	( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2 )


CRockWater::CRockWater()
{
	theHeightList = NULL;
	theUpDownFlagList = NULL;

	theVB = NULL;
	theIB = NULL;

	CleanUp();
}	

CRockWater::~CRockWater()
{
	CleanUp();
}

void	CRockWater::CleanUp()
{
	ZeroMemory( &theWaterInfo, sizeof( theWaterInfo ) );
	theWaterInfo.theRunDirs = ( RW_RD_NONE << 4 | RW_RD_NONE );
	
	theVtxCount = 0;
	theIdxCount = 0;

	theInterval = 4;

	SAFE_DELETE_ARRAY(theHeightList);

	SAFE_DELETE_ARRAY(theUpDownFlagList);

	if ( theVB )
	{
		theVB->Release();
		theVB = NULL;
	}
	if ( theIB )
	{
		theIB->Release();
		theIB = NULL;
	}

	D3DXMatrixIdentity( &theMatWorld );

	for( int i = 0; i < 2; ++i )
	{
		theWaterInfo.theTextureIndexs = 0xff;

		theTexture[i] = NULL;	/// 외부에서 생성된 텍스쳐의 포인터만 가지고 있는 것이므로 Release() 하지 않습니다. 

		D3DXMatrixIdentity( &theMatTexturePos[i] );
	}
}

/// Texture는 외부에서 미리 생성하여 포인터만 넘깁니다. 
#ifdef DIRECT_VERSION_9_MJH
bool	CRockWater::Create( LPDIRECT3DDEVICE9 aD3DDevice, LPDIRECT3DTEXTURE9 aTex0, LPDIRECT3DTEXTURE9 aTex1, SRockWaterInfoPtr aWaterInfoPtr )
#else
bool	CRockWater::Create( LPDIRECT3DDEVICE8 aD3DDevice, LPDIRECT3DTEXTURE8 aTex0, LPDIRECT3DTEXTURE8 aTex1, SRockWaterInfoPtr aWaterInfoPtr )
#endif // DIRECT_VERSION_9_MJH
{
	CleanUp();

	/// 물 면적이 넓을 수록 삼각형 크기도 커진다. 
	int size = RW_MAX( ( aWaterInfoPtr->theWidth ), ( aWaterInfoPtr->theHeight ) );
	theInterval = ( int )( size / 20 ) + 2.0f;
	theInterval = RW_MAX( theInterval, 4 );		/// 최소 4 * 16.0f 
	theInterval = RW_MIN( theInterval, 10 );	/// 최대 10 * 16.0f 

	thePatchWidth = (int)( aWaterInfoPtr->theWidth / theInterval + 0.999f );		/// 짤리지 않게.. 올림 처리 
	thePatchHeight = (int)( aWaterInfoPtr->theHeight / theInterval + 0.999f );

	if( thePatchWidth <= 0 || thePatchHeight <= 0 )
	{
		return false;
	}

	theHeightList = SAFE_NEW_ARRAY( float , ( thePatchWidth + 1 ) * ( thePatchHeight + 1 ) );	
	theUpDownFlagList = SAFE_NEW_ARRAY( bool , ( thePatchWidth + 1 ) * ( thePatchHeight + 1 ) );	

	int heightVariance = 1;
	if( aWaterInfoPtr->theHeightVariance > 0 )
	{
		heightVariance = aWaterInfoPtr->theHeightVariance;
	}
	for( int i = 0; i < ( thePatchWidth + 1 ) * ( thePatchHeight + 1 ); ++i )
	{
		theHeightList[i] = aWaterInfoPtr->theHeightBase + rand() % heightVariance;
		theUpDownFlagList[i] = ( rand() % 2 == 0 )? true:false;
	}

	short	vertexCount = thePatchWidth * thePatchHeight * 4;
	short	indexCount = thePatchWidth * thePatchHeight * 6;

	if( FAILED( g_RenderManager.CreateVertexBuffer( vertexCount * sizeof( FVF_ROCKWATER ), D3DUSAGE_DYNAMIC, D3DFVF_ROCKWATER, D3DPOOL_SYSTEMMEM, &theVB ) ) )
	{
		MessageBox( NULL, "CreateVertexBuffer Failed", "RockWater", MB_OK );
		return false;
	}

	if( FAILED( g_RenderManager.CreateIndexBuffer( indexCount * sizeof( WORD ), D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_SYSTEMMEM, &theIB ) ) )
	{
		MessageBox( NULL, "CreateIndexBuffer Failed", "RockWater", MB_OK );
		return false;
	}

	/// 처음 생성할 때 인덱스 버퍼를 만들고 렌더링에서는 다시 건들지 않는다. 
	vertexCount = 0;
	indexCount = 0;
	WORD*	indices;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( theIB->Lock( 0, 0, ( void** )&indices, 0 ) ) )
#else
	if( FAILED( theIB->Lock( 0, 0, ( BYTE** )&indices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		MessageBox( NULL, "IB Lock Failed", "RockWater", MB_OK );
		return false;
	}
	for( int z = 0; z < thePatchHeight; ++z )
	{
		for( int x = 0; x < thePatchWidth; ++x )
		{
			vertexCount += 4;
			indices[indexCount++]			= ( WORD )( vertexCount - 4 );
			indices[indexCount++]			= ( WORD )( vertexCount - 3 );
			indices[indexCount++]			= ( WORD )( vertexCount - 1 );
			indices[indexCount++]			= ( WORD )( vertexCount - 2 );
			indices[indexCount++]			= ( WORD )( vertexCount - 1 );
			indices[indexCount++]			= ( WORD )( vertexCount - 3 );
		}
	}
	if( FAILED( theIB->Unlock() ) )
	{
		MessageBox( NULL, "IB Unlock Failed", "RockWater", MB_OK );
		return false;
	}
	theVtxCount = vertexCount;
	theIdxCount = indexCount;


	theWaterInfo = *aWaterInfoPtr;
	theTexture[0] = aTex0;
	theTexture[1] = aTex1;

	return true;
}

bool CRockWater::Update()
{
	if( theWaterInfo.theHeightVariance > 0 )
	{
		float heuristic = theWaterInfo.theHeightVariance * 0.001f;
		heuristic = RW_MAX( heuristic, 0.002f );
		heuristic = RW_MIN( heuristic, 0.02f );

		float ht = 0.0f;
		for( int i = 0; i < ( thePatchWidth + 1 ) * ( thePatchHeight + 1 ); ++i )
		{
			ht = ( theWaterInfo.theHeightBase + theWaterInfo.theHeightVariance - theHeightList[i] ) * 0.01f + heuristic;
			if( theUpDownFlagList[i] )
			{
				theHeightList[i] += ht;
				if( theHeightList[i] >= ( theWaterInfo.theHeightBase + theWaterInfo.theHeightVariance ) )
				{
					theUpDownFlagList[i] = false;
				}
			}
			else
			{
				theHeightList[i] -= ht;
				if( theHeightList[i] <= theWaterInfo.theHeightBase )
				{
					theUpDownFlagList[i] = true;
				}
			}
		}
	}

	const float	offset = 16.0f;		/// 셀 좌표계에서 설정된 한 셀의 폭 
	int		x, z;
	int		cx, cz;

	float	maxX = ( theWaterInfo.theLeftX + theWaterInfo.theWidth ) * offset;
	float	maxZ = ( theWaterInfo.theBottomZ + theWaterInfo.theHeight ) * offset;

	WORD			vertexCount = 0;
	FVF_ROCKWATER*	vertices;

	if( !theVB )
	{
		return false;
	}
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( theVB->Lock( 0, 0, ( void** )&vertices, 0 ) ) )
#else
	if( FAILED( theVB->Lock( 0, 0, ( BYTE** )&vertices, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		MessageBox( NULL, "VB Lock Failed", "RockWater", MB_OK );
		return false;
	}

	for( z = 0; z < thePatchHeight; ++z )
	{
		cz = ( int )( theWaterInfo.theBottomZ + z * theInterval );

		for( x = 0; x < thePatchWidth; ++x )
		{
			cx = ( int )( theWaterInfo.theLeftX + x * theInterval );

			vertices[vertexCount].x		= cx * offset;
			vertices[vertexCount].y		= theHeightList[ ( thePatchWidth + 1 ) * z + x ];
			vertices[vertexCount].z		= cz * offset;
			vertices[vertexCount].color = D3DCOLOR_ARGB(128, 255, 255, 255);
			vertices[vertexCount].u0	= 0.0f;
			vertices[vertexCount].v0	= 1.0f;
			vertices[vertexCount].u1	= 0.0f;
			vertices[vertexCount].v1	= 1.0f;
			++vertexCount;

			vertices[vertexCount].x		= cx * offset;
			vertices[vertexCount].y		= theHeightList[ ( thePatchWidth + 1 ) * ( z + 1 ) + x ];
			vertices[vertexCount].z		= ( cz + 1 * theInterval ) * offset;
			vertices[vertexCount].color = D3DCOLOR_ARGB(128, 255, 255, 255);
			vertices[vertexCount].u0	= 0.0f;
			vertices[vertexCount].v0	= 0.0f;
			vertices[vertexCount].u1	= 0.0f;
			vertices[vertexCount].v1	= 0.0f;
			++vertexCount;

			vertices[vertexCount].x		= ( cx + 1 * theInterval ) * offset;
			vertices[vertexCount].y		= theHeightList[ ( thePatchWidth + 1 ) * ( z + 1 ) + ( x + 1 ) ];
			vertices[vertexCount].z		= ( cz + 1 * theInterval ) * offset;
			vertices[vertexCount].color = D3DCOLOR_ARGB(128, 255, 255, 255);
			vertices[vertexCount].u0	= 1.0f;
			vertices[vertexCount].v0	= 0.0f;
			vertices[vertexCount].u1	= 1.0f;
			vertices[vertexCount].v1	= 0.0f;
			++vertexCount;

			vertices[vertexCount].x		= ( cx + 1 * theInterval ) * offset;
			vertices[vertexCount].y		= theHeightList[ ( thePatchWidth + 1 ) * z + ( x + 1 ) ];
			vertices[vertexCount].z		= cz * offset;
			vertices[vertexCount].color = D3DCOLOR_ARGB(128, 255, 255, 255);
			vertices[vertexCount].u0	= 1.0f;
			vertices[vertexCount].v0	= 1.0f;
			vertices[vertexCount].u1	= 1.0f;
			vertices[vertexCount].v1	= 1.0f;
			++vertexCount;

			/// 끝자락 경계선에서는 설정된 영역에서 vertex가 벗어나지 않도록 체크 
			if( x == ( thePatchWidth - 1 ) || z == ( thePatchHeight - 1 ) ) 
			{
				for( int idx = vertexCount - 4; idx < vertexCount; ++idx )
				{
					vertices[idx].x	= RW_MIN( ( vertices[idx].x ), ( maxX ) );
					vertices[idx].z	= RW_MIN( ( vertices[idx].z ), ( maxZ ) );
				}
			}
		}
	}

	if( FAILED( theVB->Unlock() ) )
	{
		MessageBox( NULL, "VB Unlock Failed", "RockWater", MB_OK );
		return false;
	}

	return true;
}

#ifdef DIRECT_VERSION_9_MJH
bool CRockWater::Render( LPDIRECT3DDEVICE9 aD3DDevice )
#else
bool CRockWater::Render( LPDIRECT3DDEVICE8 aD3DDevice )
#endif // DIRECT_VERSION_9_MJH
{
	float speed;

	int i = -1;
	while( ++i < 2 )
	{
		if( theTexture[i] == NULL )	/// 텍스쳐가 지정되지 않았으면 애니메이션 계산할 필요 없음. 
		{
			continue;
		}

		/// 하위 4bit가 0번 텍스쳐 속도. 상위 4bit가 1번 텍스쳐 속도. 0 ~ 15의 값을 가질 수 있다. 
		speed = 0.0002f * ( ( theWaterInfo.theRunSpeeds >> ( 4 * i ) ) & 0xf ) + 0.0001f;

		switch ( ( ( theWaterInfo.theRunDirs >> ( 4 * i ) ) & 0xf ) )
		{
			case RW_RD_LEFT:
				theMatTexturePos[i]._31	= ( theMatTexturePos[i]._31 >= 1.0f )? 0.0f : theMatTexturePos[i]._31 + speed;
				theMatTexturePos[i]._32	= 0.0f;
				break;
			case RW_RD_LEFTUP:
				theMatTexturePos[i]._31	= ( theMatTexturePos[i]._31 >= 1.0f )? 0.0f : theMatTexturePos[i]._31 + speed;
				theMatTexturePos[i]._32	= ( theMatTexturePos[i]._32 >= 1.0f )? 0.0f : theMatTexturePos[i]._32 + speed;
				break;
			case RW_RD_LEFTDOWN:
				theMatTexturePos[i]._31	= ( theMatTexturePos[i]._31 >= 1.0f )? 0.0f : theMatTexturePos[i]._31 + speed;
				theMatTexturePos[i]._32	= ( theMatTexturePos[i]._32 <= 0.0f )? 1.0f : theMatTexturePos[i]._32 - speed;
				break;
			case RW_RD_RIGHT:
				theMatTexturePos[i]._31	= ( theMatTexturePos[i]._31 <= 0.0f )? 1.0f : theMatTexturePos[i]._31 - speed;
				theMatTexturePos[i]._32	= 0.0f;
				break;
			case RW_RD_RIGHTUP:
				theMatTexturePos[i]._31	= ( theMatTexturePos[i]._31 <= 0.0f )? 1.0f : theMatTexturePos[i]._31 - speed;
				theMatTexturePos[i]._32	= ( theMatTexturePos[i]._32 >= 1.0f )? 0.0f : theMatTexturePos[i]._32 + speed;
				break;
			case RW_RD_RIGHTDOWN:
				theMatTexturePos[i]._31	= ( theMatTexturePos[i]._31 <= 0.0f )? 1.0f : theMatTexturePos[i]._31 - speed;
				theMatTexturePos[i]._32	= ( theMatTexturePos[i]._32 <= 0.0f )? 1.0f : theMatTexturePos[i]._32 - speed;
				break;
			case RW_RD_NONE:	/// 흐르지 않는 설정
				if( i == 0 )	/// 0번 텍스쳐는 위쪽으로 아주 천천히 흐르고 
				{
					theMatTexturePos[i]._31	= 0.0f;
					theMatTexturePos[i]._32	= ( theMatTexturePos[i]._32 >= 1.0f )? 0.0f : theMatTexturePos[i]._32 + 0.0001f;
				}
				else			/// 1번 텍스쳐는 아래쪽으로 아주 천천히 흐르도록 
				{
					theMatTexturePos[i]._31	= 0.0f;
					theMatTexturePos[i]._32	= ( theMatTexturePos[i]._32 <= 0.0f )? 1.0f : theMatTexturePos[i]._32 - 0.0001f;
				}
				break;
			case RW_RD_UP:
				theMatTexturePos[i]._31	= 0.0f;
				theMatTexturePos[i]._32	= ( theMatTexturePos[i]._32 >= 1.0f )? 0.0f : theMatTexturePos[i]._32 + speed;
				break;
			case RW_RD_DOWN:
				theMatTexturePos[i]._31	= 0.0f;
				theMatTexturePos[i]._32	= ( theMatTexturePos[i]._32 <= 0.0f )? 1.0f : theMatTexturePos[i]._32 - speed;
				break;
		}
	}

	///--JAPAN_BUG_MODIFY
	g_RenderManager.SetCullMode( D3DCULL_NONE );
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetAlphaTestEnable( FALSE );

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCCOLOR );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressU( 1 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressV( 1 , D3DTADDRESS_WRAP );

	/// 그림자 등을 처리할 때 이값이 바뀌어 있는 경우가 있음.
	g_RenderManager.SetTextureCoordIndex (  1 , 1 );

	/// single texture
	if( theTexture[1] == NULL )	
	{
		g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_BLENDDIFFUSEALPHA );
	}
	/// multi texture
	else						
	{ 
		g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	}

	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
	
	///--JAPAN_BUG_MODIFY
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );	

	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE  );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_DIFFUSE );
	
	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADD );
	g_RenderManager.SetTextureColorArg1( 1 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 1 , D3DTA_CURRENT );

	g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 1 , D3DTA_DIFFUSE );

	g_RenderManager.SetTexture( 0, theTexture[0] );
	g_RenderManager.SetTexture( 1, theTexture[1] );

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT2 ); /// Texture Tranform 하겠다고 설정
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_COUNT2 );

	g_RenderManager.SetTransform( D3DTS_TEXTURE0, &theMatTexturePos[0] );
	g_RenderManager.SetTransform( D3DTS_TEXTURE1, &theMatTexturePos[1] );
	g_RenderManager.SetTransform( D3DTS_WORLD, &theMatWorld );

	g_RenderManager.SetStreamSource( 0, theVB, sizeof( FVF_ROCKWATER ) );
	g_RenderManager.SetFVF( D3DFVF_ROCKWATER );
	g_RenderManager.SetIndices( theIB, 0 );
	
	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, theVtxCount, 0, theIdxCount/3 );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
	g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );

	g_RenderManager.SetTexture( 1, NULL );	/// 멀티텍스쳐링 초기화 

	return true;
}

/// for editing with map tool --------------------------->
void CRockWater::ResetHeight()
{
	int heightVariance = 1;
	if( theWaterInfo.theHeightVariance > 0 )
	{
		heightVariance = theWaterInfo.theHeightVariance;
	}
	for( int i = 0; i < ( thePatchWidth + 1 ) * ( thePatchHeight + 1 ); ++i )
	{
		theHeightList[i] = theWaterInfo.theHeightBase + rand() % heightVariance;
		theUpDownFlagList[i] = ( rand() % 2 == 0 )? true:false;
	}
}
SRockWaterInfoPtr CRockWater::GetWaterInfo()
{
	return &theWaterInfo;
}

void CRockWater::SetHeightBase( float aHeightBase )
{
	theWaterInfo.theHeightBase = aHeightBase;
	ResetHeight();
}

void CRockWater::SetHeightVariance( BYTE aHeightVariance )
{
	theWaterInfo.theHeightVariance = aHeightVariance;
	ResetHeight();
}

void CRockWater::SetRunDirs( BYTE aIndex, BYTE aRunDir )
{
	aIndex = aIndex % 2;	/// 0 or 1
	aRunDir = aRunDir & 0xf;

	BYTE var0 = theWaterInfo.theRunDirs & 0xf;
	BYTE var1 = ( theWaterInfo.theRunDirs >> 4 ) & 0xf;

	if( aIndex == 0 )
	{
		theWaterInfo.theRunDirs = ( var1 << 4 ) | aRunDir;
	}
	else
	{
		theWaterInfo.theRunDirs = ( aRunDir << 4 ) | var0;
	}
}

void CRockWater::SetRunSpeeds( BYTE aIndex, BYTE aRunSpeed )
{
	aIndex = aIndex % 2;	/// 0 or 1
	aRunSpeed = aRunSpeed & 0xf;

	BYTE var0 = theWaterInfo.theRunSpeeds & 0xf;
	BYTE var1 = ( theWaterInfo.theRunSpeeds >> 4 ) & 0xf;

	if( aIndex == 0 )
	{
		theWaterInfo.theRunSpeeds = ( var1 << 4 ) | aRunSpeed;
	}
	else
	{
		theWaterInfo.theRunSpeeds = ( aRunSpeed << 4 ) | var0;
	}
}
#ifdef DIRECT_VERSION_9_MJH
void CRockWater::SetTextureIndexs( BYTE aIndex, BYTE aTextureIndex, LPDIRECT3DTEXTURE9 aTexture )
#else
void CRockWater::SetTextureIndexs( BYTE aIndex, BYTE aTextureIndex, LPDIRECT3DTEXTURE8 aTexture )
#endif // DIRECT_VERSION_9_MJH
{
	aIndex = aIndex % 2;	/// 0 or 1
	aTextureIndex = aTextureIndex & 0xf;

	BYTE var0 = theWaterInfo.theTextureIndexs & 0xf;
	BYTE var1 = ( theWaterInfo.theTextureIndexs >> 4 ) & 0xf;

	if( aIndex == 0 )
	{
		theWaterInfo.theTextureIndexs = ( var1 << 4 ) | aTextureIndex;
	}
	else
	{
		theWaterInfo.theTextureIndexs = ( aTextureIndex << 4 ) | var0;
	}

	theTexture[aIndex] = aTexture;
}



