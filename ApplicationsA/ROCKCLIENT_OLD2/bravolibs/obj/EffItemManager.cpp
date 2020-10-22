
#include "..\\..\\RockClient.h"
#include "EffItemManager.h"
#include "etcstruct.h"
#include "..\\..\\VFileSystem.h"

#ifdef DIRECT_VERSION_9_MJH
extern LPDIRECT3DDEVICE9		g_lpDevice;
#else
extern LPDIRECT3DDEVICE8		g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern long g_nowTime;

CEffItemManager g_EffItemManager;

extern int g_nTextureDetail;

CEffItemManager::CEffItemManager()
{
	ClearData();
}

CEffItemManager::~CEffItemManager()
{
	ClearData();
}

BOOL CEffItemManager::LoadData(void)
{
	ClearData();

	char FileName[ 1024 ] = "";		
	wsprintf( FileName , "%s" , EFFITEM_DATA_FILE );

	VFileHandle* pFH = g_VFChrRoot.OpenFile( FileName );
	VFileHandle* pTFH = NULL;

	if( pFH == NULL ) 
	{
		MessageBox( NULL, "!EffItem.dat 파일 열기 실패!!!", "알림", MB_OK );		
		return FALSE;
	}

	int nfile_size = pFH->GetSize();

	///-- 사이즈 정규성 Check
	if( nfile_size % sizeof( SEffItemInfo ) != 0 )
	{
		MessageBox( NULL, "!EffItem.dat size Error", "Error", MB_OK );		
		g_VFChrRoot.CloseFile( pFH );
		return FALSE;
	}
	///-- Set Count
	int n_count = nfile_size / sizeof( SEffItemInfo );

	for( int i = 0; i < n_count ; ++i )
	{
		SEffItemInfo EffItemInfo;	
		pFH->Read( ( void * )&EffItemInfo, sizeof( SEffItemInfo ));
		
		DWORD nCode = EffItemInfo.nCode;
		m_ItemInfoList.insert(map <DWORD , SEffItemInfo>::value_type(nCode, EffItemInfo));

		///--텍스쳐 데이터 로딩
		DWORD nTexNum = EffItemInfo.TexCode;
		TexListPos tPos = m_TexList.find(nTexNum);
		///-- 텍스쳐가 존재하지 않을경우
		if(tPos == m_TexList.end())
		{
			SEItemTexture sEitemTex;
			sEitemTex.dTexCode = nTexNum;
			m_TexList.insert(map <DWORD, SEItemTexture>::value_type(nTexNum, sEitemTex));				
		}
	}

	g_VFChrRoot.CloseFile( pFH );

	return TRUE;
}

void CEffItemManager::ClearData()
{
	m_ItemInfoList.clear();

	ModelListPos mPos = m_ModelList.begin();	
	for( ; mPos != m_ModelList.end(); mPos++ )
	{
		SModelList sModel = (*mPos).second;
		SAFE_DELETE( sModel.pModel );
		SAFE_DELETE( sModel.pMoveModel );
	}
	m_ModelList.clear();	
	
	TexListPos tPos = m_TexList.begin();	
	for( ; tPos != m_TexList.end(); tPos++ )
	{
		SEItemTexture sEitemTex = (*tPos).second;
		SAFE_RELEASE( sEitemTex.lpTexture );		
	}	
	m_TexList.clear();

	AniListPos aPos = m_AniList.begin();
	for( ; aPos != m_AniList.end(); aPos++ )
	{
		Bone * pBone = (*aPos).second;
		SAFE_DELETE( pBone );		
	}	
	m_AniList.clear();
}

SEffItemInfo CEffItemManager::FindItemInfo(DWORD nCode)
{
	SEffItemInfo ItemInfo;
	ItemInfoListPos iPos = m_ItemInfoList.find(nCode);
	
	if(iPos == m_ItemInfoList.end())
	{
		return ItemInfo;	
	}
	
	ItemInfo = (*iPos).second;	
	return ItemInfo;
}

SModelList CEffItemManager::FindModelList(DWORD ModelCode)
{
	SModelList sModel;
	ModelListPos mPos = m_ModelList.find(ModelCode);

	if(mPos == m_ModelList.end())
	{
		return sModel;	
	}
	
	sModel = (*mPos).second;		
	return sModel;
}

SModelList CEffItemManager::FindModelList(SEffItemInfo & sEItmInfo)
{	
	char FileName[ 1024 ] = "";
	VFileHandle* pTFH = NULL;	
	SModelList sModel;
	ModelListPos mPos = m_ModelList.find(sEItmInfo.ModelCode);

	if(mPos == m_ModelList.end())
	{
		///-- 이동모델이 존재하지 않을경우에는
		if(!sEItmInfo.isChange)
		{
			sModel.pModel = SAFE_NEW( Model );							
			if(!sModel.pModel)
			{
				return sModel;
			}
			sprintf(FileName,"%u.msh", sEItmInfo.ModelCode);
			if( !sModel.pModel->ReadVF( &g_VFEffectStatic, FileName , false ) )
			{
				SAFE_DELETE( sModel.pModel );			
				return sModel;
			}
		}
		else
		{
			sModel.pModel = SAFE_NEW( Model );
			sModel.pMoveModel = SAFE_NEW( Model );
			if(!sModel.pModel || !sModel.pMoveModel)
			{
				return sModel;
			}
			sprintf(FileName,"%u_idle.msh", sEItmInfo.ModelCode);
			if( !sModel.pModel->ReadVF( &g_VFEffectStatic, FileName , false ) )
			{
				SAFE_DELETE( sModel.pModel );
				SAFE_DELETE( sModel.pMoveModel );				
				return sModel;
			}
			sprintf(FileName,"%u_run.msh", sEItmInfo.ModelCode);
			if( !sModel.pMoveModel->ReadVF( &g_VFEffectStatic, FileName , false ) )
			{
				SAFE_DELETE( sModel.pModel );
				SAFE_DELETE( sModel.pMoveModel );				
				return sModel;
			}
		}
		
		m_ModelList.insert(map <DWORD, SModelList>::value_type(sEItmInfo.ModelCode, sModel));		
		return sModel;	
	}
	
	sModel = (*mPos).second;		
	return sModel;
}

Bone * CEffItemManager::FineAniList(DWORD AniCode)
{
	Bone * pBone = NULL;
	AniListPos aPos = m_AniList.find(AniCode);

	if(aPos == m_AniList.end())
	{
		return NULL;	
	}
	
	pBone = (*aPos).second;		
	return pBone;
}

Bone * CEffItemManager::FineAniList(SEffItemInfo & sEItmInfo)
{
	char FileName[ 1024 ] = "";
	VFileHandle* pTFH = NULL;
	Bone * pBone = NULL;

#ifdef NEW_EFFITEMTYPE	
	AniListPos aPos = m_AniList.find(sEItmInfo.AniCode);

	if(aPos == m_AniList.end())
	{
		///-- 애니 데이터 로딩
		DWORD nAniNum = sEItmInfo.AniCode;
		if( !sEItmInfo.isVtxAni && nAniNum )
		{
			pBone = SAFE_NEW( Bone );				
			if( !pBone )
			{
				return NULL;
			}				
			sprintf(FileName,"%u.ani", sEItmInfo.AniCode );
			if( !pBone->ReadVF( &g_VFEffectStatic , FileName ) )
			{
				SAFE_DELETE( pBone );					
				return NULL;
			}				
			
			m_AniList.insert(map <DWORD, Bone *>::value_type(nAniNum, pBone));			
		}
		
		return pBone;
	}
	
	pBone = (*aPos).second;	
#endif	
	
	return pBone;
}

SEItemTexture * CEffItemManager::FindEItemTexture(SEffItemInfo & sEItmInfo)
{
	SEItemTexture * pEItemTexture = NULL;


	return pEItemTexture;
}

void CEffItemManager::SetTexture(DWORD dTexCode)
{
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 lTex = NULL;
#else
	LPDIRECT3DTEXTURE8 lTex = NULL;
#endif // DIRECT_VERSION_9_MJH
	char FileName[ 1024 ] = "";
	VFileHandle* pTFH = NULL;

	SEItemTexture sEitemTex = m_TexList[dTexCode];
	if(dTexCode == sEitemTex.dTexCode)
	{
		if(m_TexList[dTexCode].lpTexture)
		{
			m_TexList[dTexCode].lPrevTime = g_nowTime;
			g_RenderManager.SetTexture(0, m_TexList[dTexCode].lpTexture);
		}
		else
		{
			
			int l_nDevSkip = 0;
			l_nDevSkip = GetPrivateProfileInt( "Texture", "Character_Skip", 0, "./dev.ini");
			if(l_nDevSkip)
			{
				g_RenderManager.SetTexture(0, NULL);
				return;
			}		
			
			sprintf(FileName,"%u.tga", dTexCode);
			pTFH = g_VFEffectStatic.OpenFile( FileName );
			if( pTFH == NULL )
			{
				return;
			}

			///--PRINT_DLG_LOG("날개 텍스쳐 로딩");
			
			UINT uWidth = D3DX_DEFAULT;
			UINT uHeight = D3DX_DEFAULT;
			D3DFORMAT fFormat = D3DFMT_UNKNOWN;
			
			D3DXIMAGE_INFO imageinfo;			
			if( FAILED( D3DXGetImageInfoFromFileInMemory( pTFH->GetData(), pTFH->GetSize(), &imageinfo ) ) )
			{       
				return;
			}
			
			uWidth = imageinfo.Width;
			uHeight = imageinfo.Height;
			fFormat = imageinfo.Format;
			
			int nLevel = pow((float)2, (float)g_nTextureDetail);
			uWidth /= nLevel; 
			uHeight /= nLevel;
			
			DWORD aFilter = D3DX_FILTER_LINEAR;
			if( g_nTextureDetail == 0 )
			{
				aFilter = D3DX_FILTER_NONE;
			}
			
			if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( pTFH->GetData(), pTFH->GetSize(),	/// 데이터 포인터, 사이즈 
																		uWidth, uHeight,
																		//64, 64,
																		1, 0, fFormat, D3DPOOL_MANAGED,
																		//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																		aFilter,
																		D3DX_DEFAULT,
																		0, NULL, NULL, &m_TexList[dTexCode].lpTexture ) ) )
			{
				g_VFEffectStatic.CloseFile( pTFH );	/// 파일 반드시 닫아야 한다.			
				return;
			}
			g_VFEffectStatic.CloseFile( pTFH );

			m_TexList[dTexCode].lPrevTime = g_nowTime;			
			g_RenderManager.SetTexture(0, m_TexList[dTexCode].lpTexture);

			///PRINT_DLG_LOG( "!이펙트 아이템 텍스쳐 로드 %d", dTexCode );
		}
	}
}

void	CEffItemManager::DeleteNotUsedTex()
{
	int i = 0;
	
	TexListPos tPos = m_TexList.begin();

	for( ; tPos != m_TexList.end(); ++tPos )
	{
		SEItemTexture * pEitemTex = (SEItemTexture *)(&((*tPos).second));
		if( pEitemTex->lpTexture && 
			SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , pEitemTex->lPrevTime ) , > , (600000 * 3) ) )	///-- 3분
		{
			SAFE_RELEASE(pEitemTex->lpTexture);
			///PRINT_DLG_LOG( "!이펙트 아이템 텍스쳐 해제 %d", pEitemTex->dTexCode );
		}
	}

	ModelListPos mPos = m_ModelList.begin();
	vector < ModelListPos > tempMlist;
	for( ; mPos != m_ModelList.end(); mPos++ )
	{
		SModelList * psModel = (SModelList *)(&(*mPos).second);

		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , psModel->pModel->m_lPrevTime ) , > , (10000) ) ) 
			///-- 10초 텍스쳐 이외의 리소스는 로딩타임이 짧아서 무리없음
		{
			SAFE_DELETE( psModel->pModel );
			SAFE_DELETE( psModel->pMoveModel );
			tempMlist.push_back(mPos);			
			///PRINT_DLG_LOG( "미사용 날개 데이타해제" );
		}
	}
	
	for( i = 0 ; i < tempMlist.size() ; i++)
	{
		m_ModelList.erase(tempMlist[i]);
	}	
}

void	CEffItemManager::AllDeleteUsedTex()
{
	TexListPos tPos = m_TexList.begin();

	for( ; tPos != m_TexList.end(); ++tPos )
	{
		SEItemTexture * pEitemTex = (SEItemTexture *)(&((*tPos).second));
		if( pEitemTex->lpTexture )	
		{
			SAFE_RELEASE(pEitemTex->lpTexture);		
		}
	}
}
