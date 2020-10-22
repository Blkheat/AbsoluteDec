//----------------------------------------------------------------------------------------------------------------------
// 파일명 : LoadingWndProc.cpp
//----------------------------------------------------------------------------------------------------------------------
#include "LoadingWndProc.h"
#include "..\\..\\Effect.h"


CLoadingBarWnd	LoadingBarWnd;


//-----------------------------------------------------------------------------
void CLoadingBarWnd::Init()
{
	IsProgress = false;
	IsChange = true;
	Delta = 0;
	Percent = 0;
	ml_warpmap_ID = 0;

	m_pBackBuffer = NULL;
	m_pBackSurface = NULL;
	m_pBackTexture = NULL;
	m_pRenderToSurface = NULL;
	m_pTex = NULL;
	m_pSurface = NULL;
	m_pVB = NULL;
	m_pIB = NULL;
	m_pBackVB = NULL;
	
	m_IsMakeImg = false;

	m_pMsgScAnswerWarp = NULL;
	m_pMsgScFightRoomList = NULL;

	m_pHelpTex = NULL;

	IsFirstLoading=true;

	return;
}

//-----------------------------------------------------------------------------
void CLoadingBarWnd::Composition()
{
	float fClientW = 0.0f, fClientH = 0.0f;
	
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	fClientW = nRui->Desktop.Width;
	fClientH = nRui->Desktop.Height;
	
#else
	
	if( nRui->GetResolution() == n_800X600 )
	{
		fClientW = 800.0f;
		fClientH = 600.0f;
	}
	else if( nRui->GetResolution() == n_1024X768 )
	{
		fClientW = 1024.0f;
		fClientH = 768.0f;
	}
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	// 백버퍼 크기에 따라 위치와 크기를 바꿔야함...지금은 1024*768기준으로 작업. 
	CFrameWnd* fw = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_LoadingWnd, WID_None, 0, 0, fClientW, fClientH, false );
	fw->SetClientImage( TID_LOGIN_BACK );
	fw->SetCaption( _RT("LoadingBar") );
	fw->SetWndProc( this );
	fw->SetTitle( false );
	fw->SetVisible( false );
	fw->SetZorder( n_zoTop );
	fw->Initialize();
	fw->SetMove( false );
	
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)
	
	fw->SetExtentWH(167.0f, 561.0f);
	
#endif // #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/22)

	CImageBox* LoadingBG = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_LoadingBG, WID_LoadingWnd, 167, 561, 728, 101, false );
	LoadingBG->SetClientImage( TID_LoadingClient );
	
	
	m_pHelpTex = (CTextBox*)nRui->CreateWnd(n_wtTextBox, WID_None, WID_LoadingWnd, 237 , 638, 594, 38, false );  

//	m_pHelpTex = (CRITextBox*)nRui->CreateWnd(n_wtRITextBox, WID_None, WID_LoadingWnd, 70 + 167, 37 + 561 + 40, 594, 38, false );  
	m_pHelpTex->SetFontR( n_fRsvFontWndStatic2 );
	m_pHelpTex->SetFontg(n_ftGulimChe, 12, n_fwNormal, n_fdShadow);
	m_pHelpTex->SetTextColor(255, 0, 0);
	m_pHelpTex->SetText("");//test	
	m_pHelpTex->SetHID( HID_None );	
	m_pHelpTex->SetMultiLine( true);
	m_pHelpTex->SetLineInterval(4);

	LoadingBar = (CProgressBar*)nRui->CreateWnd( n_wtProgressBar, WID_LoadingBar, WID_LoadingWnd, 67 + 167, 37 + 561, 594, 19, false );
	LoadingBar->SetClientImage( TID_None );
	LoadingBar->SetBarImage( TID_LoadingBar );
	LoadingBar->SetBarBlank( 0, 0 );
	LoadingBar->InitProgress( 0 );
//	LoadingBar->SetVisible( true );

	InitDeviceObjects();

	return;
}

//-----------------------------------------------------------------------------
void CLoadingBarWnd::Update()
{
	if( g_RockClient.m_bGameEnd )
		return;	
	
	if( IsVisible && !IsChange )
	{
		if( nRui->Desktop.Tflag.One_10 >= 1.0f )
		{
			if( Percent != 100 )
			{
				if( Percent == 98 )
				{
					Delta = 100;
					Percent = 100;
					nRui->SendEvent( WID_LoadingBar, n_emSetPercent, (EPARAM)Delta, 0, 0, 0 );
				}
				else
				{	
					if( Delta + 2 < Percent )
					{
						Delta += 2;
						nRui->SendEvent( WID_LoadingBar, n_emSetPercent, (EPARAM)Delta, 0, 0, 0 );	
					}
				}
			}
		}				
	}

	return;
}

	
//-----------------------------------------------------------------------------
void CLoadingBarWnd::Open()
{
	IsVisible = true;
	IsProgress = true;
	IsChange = false;
	Delta = 0;
	Percent	= 10;
	nRui->SendEvent( WID_LoadingBar, n_emSetPercent, (EPARAM)0, 0, 0, 0 );

	return;
}

void CLoadingBarWnd::Close()
{
	nRui->SendEvent( WID_LoadingWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_LoadingBar, n_emSetPercent, (EPARAM)0, 0, 0, 0 );
	IsVisible	= false;
	IsProgress	= false;
	IsChange	= false;
}

///-- 페이드 시키기전에 한번 배경이미지를 배경텍스쳐에 렌더링시킨다.
HRESULT CLoadingBarWnd::MakeScreenAfterImage()
{
	if( m_IsMakeImg == false || m_pBackSurface == NULL || m_pBackBuffer == NULL )
	{
		return -1;
	}

	m_IsMakeImg = false;

	
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetAlphaTestEnable( FALSE );
	g_RenderManager.SetFogEnable( FALSE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_NONE );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_NONE );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_NONE );

	g_RenderManager.SetAlphaBlendEnable(  FALSE );

	HRESULT			hr;
#ifdef DIRECT_VERSION_9_MJH
	RECT DestRect; // 대상 표면 NULL 시 원래표면 보다 크게 복사됨 

	SetRect( &DestRect, 0, 0, nRui->Desktop.Width, nRui->Desktop.Height ); // MJH
	
	if( D3D_OK != g_RenderManager.StretchRect( m_pBackBuffer, NULL , m_pBackSurface, &DestRect , D3DTEXF_NONE ) ) 
#else
	if( D3D_OK != g_RenderManager.CopyRects( m_pBackBuffer, NULL , 0 , m_pBackSurface, NULL) ) 
#endif // DIRECT_VERSION_9_MJH	
	{	
		return S_FALSE;
	}
	
	bool bIsBeginScene = g_RockClient.m_bIsBeginScene;	
	if( bIsBeginScene )
	{
		g_RockClient.EndScene();
	}	
	
	if( SUCCEEDED( ( hr = m_pRenderToSurface->BeginScene( m_pSurface, NULL ) ) ) )
	{
		g_RenderManager.SetTexture( 0, m_pBackTexture );
		g_RenderManager.SetFVF( D3DFVF_VERTEXT_LOAD_2D );
		g_RenderManager.SetIndices( m_pIB, 0 );
		g_RenderManager.SetStreamSource( 0, m_pVB, sizeof( VERTEX_LOAD_2D ) );
		g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );
#ifdef DIRECT_VERSION_9_MJH
		m_pRenderToSurface->EndScene( D3DX_FILTER_LINEAR );
#else
		m_pRenderToSurface->EndScene();
#endif // DIRECT_VERSION_9_MJH	
	}

	if( bIsBeginScene )
	{
		g_RockClient.BeginScene();
	}

	///--g_RockClient.Present( NULL, NULL, NULL, NULL );	

	if( m_pMsgScAnswerWarp )
	{
		LoadingBarWnd.Open();
		g_RockClient.DelayProc_RpScAnswerWarp( m_pMsgScAnswerWarp );		
	}
	else if( m_pMsgScFightRoomList )
	{
		LoadingBarWnd.Open();
		g_RockClient.DelayProc_RpScFightRoomList( m_pMsgScFightRoomList );		
	}

	return S_OK;
}

// 페이드 이미지 그린다	
void CLoadingBarWnd::DrawScreenAfterImage()
{
	
	DWORD dLighting = g_RenderManager.GetLighting();
	DWORD dAlphaTestEnable = g_RenderManager.GetAlphaTestEnable();
	DWORD dFogEnable = g_RenderManager.GetFogEnable();

	DWORD dColOp = g_RenderManager.GetTextureColorOP( 0 );
	DWORD dColArg1 = g_RenderManager.GetTextureColorArg1( 0 );
	DWORD dColArg2 = g_RenderManager.GetTextureColorArg2( 0 );	

	DWORD dAlpOp = g_RenderManager.GetTextureAlphaOP( 0 );
	DWORD dAlpArg1 = g_RenderManager.GetTextureAlphaArg1( 0 );
	DWORD dAlpArg2 = g_RenderManager.GetTextureAlphaArg2( 0 );

	DWORD dMipFilter = g_RenderManager.GetTextureMipFilter( 0 );
	DWORD dMagFilter = g_RenderManager.GetTextureMagFilter( 0 );
	DWORD dMinFilter = g_RenderManager.GetTextureMinFilter( 0 );	

	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	
	g_RenderManager.SetLighting( FALSE );
	g_RenderManager.SetAlphaTestEnable( FALSE );
	g_RenderManager.SetFogEnable( FALSE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_DIFFUSE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );

	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );
	
	g_RenderManager.SetTextureMipFilter ( 0 , D3DTEXF_NONE );
	g_RenderManager.SetTextureMagFilter ( 0 , D3DTEXF_NONE );
	g_RenderManager.SetTextureMinFilter ( 0 , D3DTEXF_NONE );

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	g_RenderManager.SetTexture( 0, m_pTex );
	g_RenderManager.SetFVF( D3DFVF_VERTEXT_LOAD_2D );

	g_RenderManager.SetIndices( m_pIB, 0 );
	g_RenderManager.SetStreamSource( 0, m_pBackVB , sizeof( VERTEX_LOAD_2D ) );
	g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP , 0 , 4 , 0 , 2 );	

	g_RenderManager.SetLighting( dLighting );
	g_RenderManager.SetAlphaTestEnable( dAlphaTestEnable );
	g_RenderManager.SetFogEnable( dFogEnable );

	g_RenderManager.SetTextureColorOP  ( 0 , dColOp );
	g_RenderManager.SetTextureColorArg1( 0 , dColArg1 );
	g_RenderManager.SetTextureColorArg2( 0 , dColArg2 );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , dAlpOp );
	g_RenderManager.SetTextureAlphaArg1( 0 , dAlpArg1 );
	g_RenderManager.SetTextureAlphaArg2( 0 , dAlpArg2 );

	g_RenderManager.SetTextureMipFilter ( 0 , dMipFilter );
	g_RenderManager.SetTextureMagFilter ( 0 , dMagFilter );
	g_RenderManager.SetTextureMinFilter ( 0 , dMinFilter );

	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );

	return;
}

// 페이드 아웃용 텍스처 생성(CGlow 참고)
HRESULT CLoadingBarWnd::InitDeviceObjects()
{
	// 구형 VGA(Radeon 9550 등)는 2^ 크기로 생성해야 됨. 최신형은 상관없음.
	float fTexSize = 1024.0f;
	float fBackW = static_cast<float>(nRui->Desktop.Width);
	float fBackH = static_cast<float>(nRui->Desktop.Height);
	
#ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/27)

	if(fBackW > 1024.0f || fBackH > 1024.0f)
		fTexSize = 1024.0f * 2.0f;
	
#endif// #ifdef HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/27)

	HRESULT			hr;
#ifdef DIRECT_VERSION_9_MJH
	if( D3D_OK != g_RenderManager.GetBackBuffer( 0, 0,D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer ) )
#else
	if( D3D_OK != g_RenderManager.GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer ) )
#endif // DIRECT_VERSION_9_MJH 
	{
		return FALSE;
	}

	D3DSURFACE_DESC Desc;
	
	m_pBackBuffer->GetDesc( &Desc );

	if( FAILED( hr = g_RenderManager.CreateTexture( fTexSize, fTexSize, 1, D3DUSAGE_RENDERTARGET,  Desc.Format, D3DPOOL_DEFAULT, &m_pBackTexture ) ) )
	{
		return FALSE;
	}

	if( D3D_OK != m_pBackTexture->GetSurfaceLevel( 0 , &m_pBackSurface ) )
	{
		return FALSE;
	}

	if( FAILED( hr = g_RenderManager.CreateRenderToSurface( fTexSize, fTexSize, Desc.Format, FALSE, D3DFMT_UNKNOWN, &m_pRenderToSurface ) ) )
	{
		return FALSE;
	}
	
	if( FAILED( hr = g_RenderManager.CreateTexture( fTexSize, fTexSize, 1 , D3DUSAGE_RENDERTARGET , Desc.Format , D3DPOOL_DEFAULT, &m_pTex ) ) )
	{
		return FALSE;
	}	
	if( FAILED( hr = m_pTex->GetSurfaceLevel( 0, &m_pSurface ) ) )
	{
		return FALSE;
	}
	
	//............................................................................................................
	// 복사 버텍스 버퍼 만들기
	//............................................................................................................
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( VERTEX_LOAD_2D ) * 4 ), D3DUSAGE_WRITEONLY, 
												D3DFVF_VERTEXT_LOAD_2D, D3DPOOL_MANAGED, &m_pVB ) ) )
	{
		return FALSE;		
	}
	
	VERTEX_LOAD_2D   *pVertice = NULL;
	VERTEX_LOAD_2D    VertexList[ 4 ];
	
	float tu = fBackW / fTexSize;
	float tv = fBackH / fTexSize;
	
	VertexList[0].pos.x = 0.0f - 0.5f;
	VertexList[0].pos.y = fBackH - 0.5f;
	VertexList[0].tu0 = 0.0f;
	VertexList[0].tv0 = tv;
	
	VertexList[1].pos.x = 0.0f - 0.5f;
	VertexList[1].pos.y = 0.0f - 0.5f;
	VertexList[1].tu0 = 0.0f;
	VertexList[1].tv0 = 0.0f;
	
	VertexList[2].pos.x = fBackW - 0.5f;
	VertexList[2].pos.y = fBackH - 0.5f;
	VertexList[2].tu0 = tu;
	VertexList[2].tv0 = tv;
	
	VertexList[3].pos.x = fBackW - 0.5f;
	VertexList[3].pos.y = 0.0f - 0.5f;
	VertexList[3].tu0 = tu;
	VertexList[3].tv0 = 0.0f;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;
	}	

	memcpy( pVertice , VertexList , sizeof( VERTEX_LOAD_2D ) * 4 );

	m_pVB->Unlock();

	//............................................................................................................
	// 백버퍼 버텍스 버퍼 만들기
	//............................................................................................................
	if( FAILED( g_RenderManager.CreateVertexBuffer( ( sizeof( VERTEX_LOAD_2D ) * 4 ), D3DUSAGE_WRITEONLY, 
												D3DFVF_VERTEXT_GLOW_2D, D3DPOOL_MANAGED, &m_pBackVB ) ) )
	{
		return FALSE;		
	}
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pBackVB->Lock(0, 0, (void**)&pVertice, 0 ) ) )
#else
	if( FAILED ( m_pBackVB->Lock(0, 0, (BYTE**)&pVertice, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;
	}	

	memcpy( pVertice , VertexList , sizeof( VERTEX_LOAD_2D ) * 4 );

	m_pBackVB->Unlock();

	//............................................................................................................
	// 인덱스 버퍼 만들기
	//............................................................................................................
	if( FAILED( g_RenderManager.CreateIndexBuffer( ( sizeof( WORD ) * 4 ), D3DUSAGE_WRITEONLY, 
												D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB )) )
	{
		return FALSE;		
	}

	WORD   *pIndexBuffer = NULL;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED ( m_pIB->Lock(0, 0, (void**)&pIndexBuffer, 0 ) ) )
#else
	if( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&pIndexBuffer, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return FALSE;
	}

	for( int i = 0 ; i < 4 ; i++ )
	{
		pIndexBuffer[ i ] = ( WORD ) i;
	}

	m_pIB->Unlock();

	return S_OK;
}

//===================================================================================================================
// 사용시에 RockClient.cpp 에 추가 
HRESULT CLoadingBarWnd::InvalidateDeviceObjects()
{	
	SAFE_RELEASE( m_pBackBuffer );
	SAFE_RELEASE( m_pBackSurface );
	SAFE_RELEASE( m_pBackTexture );
	SAFE_RELEASE( m_pVB );
	SAFE_RELEASE( m_pIB );
	SAFE_RELEASE( m_pBackVB );
	SAFE_RELEASE( m_pRenderToSurface );
	SAFE_RELEASE( m_pTex );
	SAFE_RELEASE( m_pSurface );

	return S_OK;
}

HRESULT CLoadingBarWnd::RestoreDeviceObjects()
{
	InitDeviceObjects();

	return S_OK;
}

void CLoadingBarWnd::SetPercent(int nPercent)
{		
	if( nPercent % 2 != 0 )
	{
		nPercent += 1; 
	}

	if( nPercent > 100 )
	{
		nPercent = 100;
	}
	
	Delta = Percent;	
	Percent = nPercent;
}

//===================================================================================================================

void CLoadingBarWnd::DrawTexture( float x, float y, float w, float h, 
					  float tx, float ty, float tw, float th,
#ifdef DIRECT_VERSION_9_MJH
					  D3DCOLOR color , LPDIRECT3DDEVICE9 pDevice )
#else
					  D3DCOLOR color , LPDIRECT3DDEVICE8 pDevice )
#endif // DIRECT_VERSION_9_MJH
{
	static VERTEX_LOAD_2D	v[4];
	
	v[0].color = v[1].color = v[2].color = v[3].color = color;
	
	v[0].pos.x = (float)x - 0.5f;
	v[0].pos.y = (float)y - 0.5f;
	v[1].pos.x = v[0].pos.x + w;
	v[1].pos.y = v[0].pos.y;
	v[2].pos.x = v[0].pos.x + w;
	v[2].pos.y = v[0].pos.y + h;
	v[3].pos.x = v[0].pos.x;
	v[3].pos.y = v[0].pos.y + h;

	v[0].tu0 = (float)tx;
	v[0].tv0 = (float)ty;
	v[1].tu0 = v[0].tu0 + tw;
	v[1].tv0 = v[0].tv0;
	v[2].tu0 = v[0].tu0 + tw;
	v[2].tv0 = v[0].tv0 + th;
	v[3].tu0 = v[0].tu0;
	v[3].tv0 = v[0].tv0 + th;

	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, v , sizeof( VERTEX_LOAD_2D ) );
}