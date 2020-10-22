#include  "..\\..\\RockClient.h"

#include	<assert.H>
#include	<stdio.h>
#include	<sys/stat.h> 

#include "..\\..\\bravolibs\obj\etcstruct.h"
#include "..\\..\\VFileSystem.h"

#include "EmblemTexManager.h"
#include "..\\..\\CRenderManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern		LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern		LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

CEmblemTexManager g_EmblemTexManager;

///------------------------------------------------------------------------------------
///			emb 파일의 구조형식
///			헤더글자 4개 - Rock - char형 4개
///			월드 인덱스 번호 - BYTE
///			프론티어 코드 - DWORD 
///			버전 번호 - BYTE
///			JPG 파일 길이 - WORD
///         실제 JPG이미지 내용
///------------------------------------------------------------------------------------

CEmblemTexManager::CEmblemTexManager()
{
	m_MEmbTexture = NULL;
	m_REmbTexture = NULL;
}

CEmblemTexManager::~CEmblemTexManager()
{
	Clear();	
}

void CEmblemTexManager::Clear()
{
	EmblemTexListPos Pos = m_EmblemTexList.begin();
	
	for( ; Pos != m_EmblemTexList.end(); Pos++ )
	{
		VESIONTEX *pVesionTex = (*Pos).second;
		
		if(pVesionTex->lpTexture)
		{
			pVesionTex->lpTexture->Release();
			pVesionTex->lpTexture = NULL;
		}
		
		SAFE_DELETE(pVesionTex->pRenderTexture);
		SAFE_DELETE(pVesionTex);
	}

	m_EmblemTexList.clear();
}

void CEmblemTexManager::LoadEmblemFiles()
{
	Clear();
	TCHAR	file_path[256]	= {0,};

	//이쪽에서 emb폴더에 있는 엠블렌 파일을 전부 불러온다.
	//data/emb 폴더를 찾는다.

	WIN32_FIND_DATA fileFindData;
	m_hFileFind = NULL;
	BOOL bFind;

	wsprintf( file_path, "%s\\data\\emb", g_RockClient.GetRootDir() );
	DWORD dAttr =  ::GetFileAttributes(file_path);
	
	if( FILE_ATTRIBUTE_DIRECTORY != dAttr )	//해당 폴더가 존재하지 않으면...
	{
		if(!::CreateDirectory(file_path, NULL))	 //디렉토리를 생성한다.
		{
			DWORD dErr = GetLastError();
			if( dErr != ERROR_ALREADY_EXISTS )	
			{
				return;
			}
			else
			{
				SetFileAttributes( file_path, FILE_ATTRIBUTE_DIRECTORY );
			}
		}
	}

	bFind = TRUE;

	sprintf( file_path, "%s\\data\\emb\\*.emb", g_RockClient.GetRootDir() );
	m_hFileFind = FindFirstFile( file_path, &fileFindData);
	while( ( m_hFileFind != INVALID_HANDLE_VALUE ) && bFind ) 
	{
		if( !(fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			sprintf( file_path, "%s\\data\\emb\\%s", g_RockClient.GetRootDir(), fileFindData.cFileName );
			ReadEmbFile( file_path, (char *)fileFindData.cFileName);
		}
		bFind = FindNextFile( m_hFileFind, &fileFindData );
	}
	
	FindClose( m_hFileFind );
	m_hFileFind = NULL;

	char FileName[ 1024 ] = "";		
	wsprintf( FileName , "%s" , "millena_emb.tga" );

	VFileHandle* pFH = g_VFEtc.OpenFile( FileName );	

	if( pFH )
	{
		if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_NONE,
																	D3DX_DEFAULT,
																	0, NULL, NULL, &m_MEmbTexture ) ) )
		{
			g_VFEtc.CloseFile( pFH );			
		}			
		
		g_VFEtc.CloseFile( pFH );	
	}

	wsprintf( FileName , "%s" , "rain_emb.tga" );

	pFH = g_VFEtc.OpenFile( FileName );

	if( pFH )
	{
		if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
																	D3DX_DEFAULT, D3DX_DEFAULT, 
																	1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
																	D3DX_FILTER_NONE,
																	D3DX_DEFAULT,
																	0, NULL, NULL, &m_REmbTexture ) ) )
		{
			g_VFEtc.CloseFile( pFH );			
		}			
		
		g_VFEtc.CloseFile( pFH );	
	}
	
	return;
}

void CEmblemTexManager::ReadEmbFile( char * file_path, char * file_Name)
{
	FILE * infile;
	
	//파일열기 
	if ((infile = fopen(file_path, "rb")) == NULL)
	{
		return;
	}

	//파일 크기가 char*4 + BYTE + DWORD + BYTE + WORD 보다 작으면 리턴
	fseek(infile, 0, SEEK_END);
	int nSize = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	int nCSize = sizeof(char)*4 + sizeof(BYTE) + sizeof(DWORD) + sizeof(BYTE) + sizeof(WORD);
	if(nSize < nCSize)
	{
		fclose(infile);
		DeleteFile(file_path);
		return;
	}
	
	//헤더부분 비교
	char cheader[4] = "";
	fread( ( void * )cheader, sizeof(char)*4, 1, infile);

	if( cheader[0] != 'R' || cheader[1] != 'o' || 
		cheader[2] != 'c' || cheader[3] != 'k' )
	{
		fclose(infile);
		DeleteFile(file_path);
		return;
	}
	
	DWORD dFontierCode = 0;
	TCHAR Temp[256]	= {0,};
	::GetFileNameOnly( file_Name, Temp );
	
	///-- 이부분에서 정수형을 DWORD형으로 변환시킨다.
	int nTemp = atoi(Temp);
	dFontierCode = nTemp;
	if(0 == dFontierCode)	//숫자이름의 파일이 아니면
	{
		fclose(infile);
		DeleteFile(file_path);
		return;
	}

	//프론티어 코드와 파일명이 틀리면 지운다.
	DWORD dCFontierCode = 0;
	fread( &dCFontierCode, sizeof(DWORD), 1, infile);

	if(dFontierCode != dCFontierCode)
	{
		fclose(infile);
		DeleteFile(file_path);
		return;
	}

	//월드 인덱스를 가져온다.
	BYTE bWorldIndex = 0;
	fread( &bWorldIndex, sizeof(BYTE), 1, infile);	

	//버전 정보를 받아온다.
	BYTE bVersion = 0;
	fread( &bVersion, sizeof(BYTE), 1, infile);

	//JPG파일 길이를 받아온다.
	WORD wJPGSize = 0;
	fread( &wJPGSize, sizeof(WORD), 1, infile);

	if(0 == wJPGSize)
	{
		fclose(infile);
		DeleteFile(file_path);
		return;
	}

	char * pJPGImage = SAFE_NEW_ARRAY( char , wJPGSize );	
	if(NULL == pJPGImage)
	{
		fclose(infile);
		DeleteFile(file_path);
		return;
	}

	fread( pJPGImage, sizeof(char) * wJPGSize, 1, infile);

	//JPG를 이용한 엠블렌 텍스쳐를 생성한다.
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 lpTexture = NULL;
	LPDIRECT3DSURFACE9 lpSurface = NULL;
#else
	LPDIRECT3DTEXTURE8 lpTexture = NULL;
	LPDIRECT3DSURFACE8 lpSurface = NULL;
#endif // DIRECT_VERSION_9_MJH

	D3DXIMAGE_INFO imageinfo;

	if( FAILED( D3DXGetImageInfoFromFileInMemory( pJPGImage, wJPGSize, &imageinfo ) ) )
	{       
		return;
	}

	if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( (void *)pJPGImage, (UINT)wJPGSize,	/// 데이터 포인터, 사이즈 
																imageinfo.Width, imageinfo.Height, 
																1, 0, imageinfo.Format, D3DPOOL_MANAGED,
																//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																D3DX_FILTER_NONE,
																D3DX_DEFAULT,
																0, NULL, NULL, &lpTexture ) ) )
	{
			fclose(infile);
			DeleteFile(file_path);
			SAFE_DELETE_ARRAY(pJPGImage);			
			return;
	}

	/*LPDIRECT3DTEXTURE8 lpDestTexture = NULL;
	LPDIRECT3DSURFACE8 lpDestSurface = NULL;
	
	if( FAILED( g_RenderManager.CreateTexture( imageinfo.Width, imageinfo.Height, 1, 
						0, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &lpDestTexture ) ) )
	{
		SAFE_DELETE_ARRAY(pJPGImage);
		return;
	}

	if( FAILED( lpTexture->GetSurfaceLevel( 0, &lpSurface ) ) )
	{
		SAFE_DELETE_ARRAY(pJPGImage);
		return;
	}

	if( FAILED( lpDestTexture->GetSurfaceLevel( 0, &lpDestSurface ) ) )
	{
		SAFE_DELETE_ARRAY(pJPGImage);
		return;
	}

	D3DXLoadSurfaceFromSurface( lpDestSurface , NULL, NULL, lpSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0 );

	SAFE_RELEASE(lpTexture);

	lpTexture = lpDestTexture;*/
	
	SAFE_DELETE_ARRAY(pJPGImage);

	VESIONTEX *pVersionTex = SAFE_NEW( VESIONTEX );	
	if(NULL == pVersionTex)
	{
		fclose(infile);
		DeleteFile(file_path);
		return;
	}

	pVersionTex->bEmblemVer = bVersion;
	pVersionTex->bWorldIndex = bWorldIndex;
	pVersionTex->lpTexture = lpTexture;	

	m_EmblemTexList.insert(map <DWORD , VESIONTEX *>::value_type(dCFontierCode, pVersionTex));
	
	fclose(infile);

	char	file_TempPath[256]	= {0,};
	
	if(nTemp < 0)
	{
		sprintf( file_TempPath, "%s\\data\\emb\\%u.emb", g_RockClient.GetRootDir(), dCFontierCode );
		rename( file_path, file_TempPath );
	}
}

void CEmblemTexManager::ConFirmEmblemVersion( DWORD	dFrontierID, BYTE bEmblemVer )
{
	//해당 프론티어의 엠블렌을 가지고있는지 여부를 확인한다.
	
	EmblemTexListPos pos = m_EmblemTexList.find(dFrontierID);
	
	//해당 엠블렌을 가지고 있지 않으면
	if(pos == m_EmblemTexList.end())
	{
#ifdef FRONTIER
		//서버로 엠블렌 정보를 요청한다.
		g_RockClient.Send_CsMsngrEmblem(dFrontierID);
		return;
#endif 
	}

	VESIONTEX *pVersionTex = (*pos).second;
	//해당 엠블렌과 버전 정보가 다르면
#ifdef DECO_RENEWAL_GUILD_MJH
#else
	if(bEmblemVer != pVersionTex->bEmblemVer)
	{
#ifdef FRONTIER		
		g_RockClient.Send_CsMsngrEmblem(dFrontierID);
		return;
#endif 
	}			
#endif // DECO_RENEWAL_GUILD_MJH


	//해당 엠블렌과 월드인덱스가 다르면
	if(nRui->m_WorldIndex != pVersionTex->bWorldIndex)
	{
#ifdef FRONTIER		
		g_RockClient.Send_CsMsngrEmblem(dFrontierID);
		return;
#endif 
	}
}

#ifdef DECO_RENEWAL_GUILD_MJH
void CEmblemTexManager::UpdateEmblenVersion(DWORD dFrontierID, BYTE	itemIndex ,char * cEmblem )
{
	//해당 프론티어의 엠블렌을 가지고 있는지 여부를 확인한다.

	EmblemTexListPos pos = m_EmblemTexList.find(dFrontierID);
	VESIONTEX *pVersionTex = NULL;
	//char cEmblem[50] = {0,};
	WORD wEmblemSize = 3126; //변경

	//SItemBaseInfo* item_Info;
	//item_Info = g_Pc_Manager.GetItemBaseInfoIndexPtr( itemIndex );

	//sprintf( cEmblem, "%s\\data\\etc.rvf\\%d.jpg", g_RockClient.GetRootDirW() ,item_Info->theFileCode1 );
	//sprintf( cEmblem, "%d.jpg",item_Info->theFileCode1 );
	
	//해당 엠블렌을 가지고 있지 않으면
	if(pos == m_EmblemTexList.end())
	{
		pVersionTex = SAFE_NEW( VESIONTEX );
		m_EmblemTexList.insert(map <DWORD , VESIONTEX *>::value_type(dFrontierID, pVersionTex));
		pos = m_EmblemTexList.find(dFrontierID);
	}
	else
	{
		pVersionTex = (*pos).second;
	}

	if(NULL == pVersionTex)
	{
		return;
	}
	
	if(pVersionTex->lpTexture)
	{
		pVersionTex->lpTexture->Release();
		pVersionTex->lpTexture = NULL;
	}

	SAFE_DELETE(pVersionTex->pRenderTexture);

	char * pJPGImage = SAFE_NEW_ARRAY( char , wEmblemSize );	
	if( pJPGImage == NULL )
	{
		return;
	}

	memcpy(pJPGImage, cEmblem, sizeof(char) * wEmblemSize);

	//pVersionTex->bEmblemVer = bVersion;

	D3DXIMAGE_INFO imageinfo;

	if( FAILED( D3DXGetImageInfoFromFileInMemory( pJPGImage, wEmblemSize, &imageinfo ) ) )
	{       
		return;
	}

	if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( (void *)pJPGImage, (UINT)wEmblemSize,	/// 데이터 포인터, 사이즈 
																imageinfo.Width, imageinfo.Height, 
																1, 0, imageinfo.Format, D3DPOOL_MANAGED,
																//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																D3DX_FILTER_LINEAR,
																D3DX_DEFAULT,
																0, NULL, NULL, &(pVersionTex->lpTexture) ) ) )
	{
			
			SAFE_DELETE(pVersionTex);	
			m_EmblemTexList.erase(pos);			
			SAFE_DELETE_ARRAY(pJPGImage);
			return;
	}	

	MakeTexture( pVersionTex , pVersionTex->lpTexture );	
	//..........................................................................................................
	
	//파일 저장 루틴

	RTCHAR	file_path[256]	= {0,};

#ifdef UNICODE_ROCK
	Rsprintf( file_path, _RT("%s\\data\\emb\\%u.emb"), g_RockClient.GetRootDirW(), dFrontierID);
#else 
	sprintf( file_path, "%s\\data\\emb\\%u.emb", g_RockClient.GetRootDir(), dFrontierID);
#endif 
	
	
	FILE * outfile;
	
	//파일열기 

#ifdef UNICODE_ROCK
	if((outfile = _wfopen(file_path, _RT("wb") ) ) == NULL)
#else 
	if ((outfile = fopen(file_path, "wb")) == NULL)
#endif 
	{
		return;
	}

	//헤더 쓰기
	char cheader[4] = "";
	cheader[0] = 'R';
	cheader[1] = 'o';
	cheader[2] = 'c';
	cheader[3] = 'k';

	fwrite(&cheader, sizeof(char)*4, 1, outfile);	

	//프론티어 코드 쓰기
	fwrite(&dFrontierID, sizeof(DWORD), 1, outfile);

	//월드 인덱스 쓰기
	fwrite(&(nRui->m_WorldIndex), sizeof(BYTE), 1, outfile);

	//버전 정보쓰기
	//fwrite(&bVersion, sizeof(BYTE), 1, outfile);

	//JPG파일 길이 쓰기
	fwrite(&wEmblemSize, sizeof(WORD), 1, outfile);

	//실제 JPG파일 저장
	fwrite(pJPGImage, sizeof(char) * wEmblemSize, 1, outfile);

	SAFE_DELETE_ARRAY(pJPGImage);
	
	fclose(outfile);
	
}
#else
			
void CEmblemTexManager::UpdateEmblenVersion(DWORD dFrontierID, BYTE	bVersion, 
											WORD wEmblemSize, char * cEmblem)
{
	//해당 프론티어의 엠블렌을 가지고 있는지 여부를 확인한다.

	EmblemTexListPos pos = m_EmblemTexList.find(dFrontierID);
	VESIONTEX *pVersionTex = NULL;
	
	//해당 엠블렌을 가지고 있지 않으면
	if(pos == m_EmblemTexList.end())
	{
		pVersionTex = SAFE_NEW( VESIONTEX );
		m_EmblemTexList.insert(map <DWORD , VESIONTEX *>::value_type(dFrontierID, pVersionTex));
		pos = m_EmblemTexList.find(dFrontierID);
	}
	else
	{
		pVersionTex = (*pos).second;
	}

	if(NULL == pVersionTex)
	{
		return;
	}
	
	if(pVersionTex->lpTexture)
	{
		pVersionTex->lpTexture->Release();
		pVersionTex->lpTexture = NULL;
	}

	SAFE_DELETE(pVersionTex->pRenderTexture);

	char * pJPGImage = SAFE_NEW_ARRAY( char , wEmblemSize );	
	if( pJPGImage == NULL )
	{
		return;
	}

	memcpy(pJPGImage, cEmblem, sizeof(char) * wEmblemSize);

	pVersionTex->bEmblemVer = bVersion;

	D3DXIMAGE_INFO imageinfo;

	if( FAILED( D3DXGetImageInfoFromFileInMemory( pJPGImage, wEmblemSize, &imageinfo ) ) )
	{       
		return;
	}

	if( FAILED ( g_RenderManager.CreateTextureFromFileInMemoryEx( (void *)pJPGImage, (UINT)wEmblemSize,	/// 데이터 포인터, 사이즈 
																imageinfo.Width, imageinfo.Height, 
																1, 0, imageinfo.Format, D3DPOOL_MANAGED,
																//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																//D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
																D3DX_FILTER_LINEAR,
																D3DX_DEFAULT,
																0, NULL, NULL, &(pVersionTex->lpTexture) ) ) )
	{
			
			SAFE_DELETE(pVersionTex);	
			m_EmblemTexList.erase(pos);			
			SAFE_DELETE_ARRAY(pJPGImage);
			return;
	}	

	MakeTexture( pVersionTex , pVersionTex->lpTexture );	
	//..........................................................................................................
	
	//파일 저장 루틴

	RTCHAR	file_path[256]	= {0,};

#ifdef UNICODE_ROCK
	Rsprintf( file_path, _RT("%s\\data\\emb\\%u.emb"), g_RockClient.GetRootDirW(), dFrontierID);
#else 
	sprintf( file_path, "%s\\data\\emb\\%u.emb", g_RockClient.GetRootDir(), dFrontierID);
#endif 
	
	
	FILE * outfile;
	
	//파일열기 

#ifdef UNICODE_ROCK
	if((outfile = _wfopen(file_path, _RT("wb") ) ) == NULL)
#else 
	if ((outfile = fopen(file_path, "wb")) == NULL)
#endif 
	{
		return;
	}

	//헤더 쓰기
	char cheader[4] = "";
	cheader[0] = 'R';
	cheader[1] = 'o';
	cheader[2] = 'c';
	cheader[3] = 'k';

	fwrite(&cheader, sizeof(char)*4, 1, outfile);	

	//프론티어 코드 쓰기
	fwrite(&dFrontierID, sizeof(DWORD), 1, outfile);

	//월드 인덱스 쓰기
	fwrite(&(nRui->m_WorldIndex), sizeof(BYTE), 1, outfile);

	//버전 정보쓰기
	fwrite(&bVersion, sizeof(BYTE), 1, outfile);

	//JPG파일 길이 쓰기
	fwrite(&wEmblemSize, sizeof(WORD), 1, outfile);

	//실제 JPG파일 저장
	fwrite(pJPGImage, sizeof(char) * wEmblemSize, 1, outfile);

	SAFE_DELETE_ARRAY(pJPGImage);
	
	fclose(outfile);
}
#endif // DECO_RENEWAL_GUILD_MJH


VESIONTEX * CEmblemTexManager::FindForntierVersionTex(DWORD dFrontierID)
{
	EmblemTexListPos pos = m_EmblemTexList.find(dFrontierID);
	VESIONTEX *pVersionTex = NULL;
	
	//해당 엠블렌을 가지고 있지 않으면
	if(pos == m_EmblemTexList.end())
	{
		return NULL;	
	}
	
	pVersionTex = (*pos).second;
	if( !pVersionTex->pRenderTexture )
	{
		MakeTexture(pVersionTex, pVersionTex->lpTexture);
	}

	return pVersionTex;
}

void CEmblemTexManager::RenderFrontierImg(VESIONTEX * pVesionTex, int nx, int ny, int nw, int nh, float zPos)
{
	if(pVesionTex == NULL)
	{
		return;
	}

	if( pVesionTex->pRenderTexture == NULL )
	{
		MakeTexture(pVesionTex, pVesionTex->lpTexture);
		return;
	}
	
	SVertex vt[4];

	vt[0].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	vt[3].diffuse = vt[2].diffuse = vt[1].diffuse = vt[0].diffuse; 

	vt[0].x = nx - 0.5f;
	vt[0].y = ny - 0.5f;		
	vt[1].x = vt[0].x + nw;
	vt[1].y = vt[0].y;
	vt[2].x = vt[0].x + nw;
	vt[2].y = vt[0].y + nh;
	vt[3].x = vt[0].x;
	vt[3].y = vt[0].y + nh;
	vt[0].z = vt[1].z = vt[2].z = vt[3].z = zPos;

	vt[0].tu = 0.0f;
	vt[0].tv = 0.0f;
	vt[1].tu = 0.5f;
	vt[1].tv = 0.0f;
	vt[2].tu = 0.5f;
	vt[2].tv = 0.5f;			
	vt[3].tu = 0.0f;
	vt[3].tv = 0.5f;	

	g_RenderManager.SetTexture( 0, pVesionTex->pRenderTexture->GetTexture() );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, vt, sizeof( SVertex ) );
}

void CEmblemTexManager::RenderFrontierImg(VESIONTEX * pVesionTex, int nx, int ny, int nw, int nh)
{
	if(pVesionTex == NULL)
	{
		return;
	}

	if( pVesionTex->pRenderTexture == NULL )
	{
		MakeTexture(pVesionTex, pVesionTex->lpTexture);
		return;
	}

	SVertex vt[4];

	vt[0].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	vt[3].diffuse = vt[2].diffuse = vt[1].diffuse = vt[0].diffuse; 

	vt[0].x = nx - 0.5f;
	vt[0].y = ny - 0.5f;		
	vt[1].x = vt[0].x + nw;
	vt[1].y = vt[0].y;
	vt[2].x = vt[0].x + nw;
	vt[2].y = vt[0].y + nh;
	vt[3].x = vt[0].x;
	vt[3].y = vt[0].y + nh;

	vt[0].tu = 0.0f;
	vt[0].tv = 0.0f;
	vt[1].tu = 0.5f;
	vt[1].tv = 0.0f;
	vt[2].tu = 0.5f;
	vt[2].tv = 0.5f;			
	vt[3].tu = 0.0f;
	vt[3].tv = 0.5f;

	g_RenderManager.SetTexture( 0, pVesionTex->pRenderTexture->GetTexture() );
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, vt, sizeof( SVertex ) );
}

void CEmblemTexManager::DeleteEmblenFile(DWORD dFrontierID)
{


}

#ifdef DIRECT_VERSION_9_MJH
int	CEmblemTexManager::MakeTexture(VESIONTEX *pData,LPDIRECT3DTEXTURE9 pTex)
#else
int	CEmblemTexManager::MakeTexture(VESIONTEX *pData,LPDIRECT3DTEXTURE8 pTex)
#endif // DIRECT_VERSION_9_MJH
{
	if( pData == NULL || pTex == NULL )
		return FALSE;

	pData->pRenderTexture = SAFE_NEW( CRenderSurface );	

	if( NULL == pData->pRenderTexture )
		return FALSE;

	if( !pData->pRenderTexture->Init( 64 , 64 , g_RockClient.m_lpD3D , g_lpDevice ) )
	{
		SAFE_DELETE(pData->pRenderTexture);
		
		return FALSE;
	}	

	bool bIsBeginScene = g_RockClient.m_bIsBeginScene;	
	if( bIsBeginScene )
	{
		g_RockClient.EndScene();
	}	
	
	if( FAILED( pData->pRenderTexture->BeginScene() ) )
	{
		if( bIsBeginScene )
		{
			g_RockClient.BeginScene();
		}		
		
		SAFE_DELETE(pData->pRenderTexture);		
		return FALSE;
	}
	
	g_RenderManager.Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
							D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , 1.0f, 0L );
				
	SVertex vt[4];

	vt[0].diffuse = vt[1].diffuse = 
	vt[2].diffuse = vt[3].diffuse = vt[0].diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	

	vt[0].x = 0.0f - 0.5f;
	vt[0].y = 0.0f - 0.5f;		
	vt[1].x = vt[0].x + 32.0f;
	vt[1].y = vt[0].y;
	vt[2].x = vt[0].x + 32.0f;
	vt[2].y = vt[0].y + 32.0f;
	vt[3].x = vt[0].x;
	vt[3].y = vt[0].y + 32.0f;

	vt[0].tu = 0.0f;
	vt[0].tv = 0.0f;
	vt[1].tu = 1.0f;
	vt[1].tv = 0.0f;
	vt[2].tu = 1.0f;
	vt[2].tv = 1.0f;			
	vt[3].tu = 0.0f;
	vt[3].tv = 1.0f;

	DWORD dAlphaBlendEnable = g_RenderManager.GetAlphaBlendEnable();
	DWORD dColOp = g_RenderManager.GetTextureColorOP( 0 );
	DWORD dColArg1 = g_RenderManager.GetTextureColorArg1( 0 );
	DWORD dColArg2 = g_RenderManager.GetTextureColorArg2( 0 );

	DWORD dAlpOp = g_RenderManager.GetTextureAlphaOP( 0 );
	DWORD dAlpArg1 = g_RenderManager.GetTextureAlphaArg1( 0 );
	DWORD dAlpArg2 = g_RenderManager.GetTextureAlphaArg2( 0 );	

	DWORD dFogEnable = g_RenderManager.GetFogEnable();
	DWORD dLighting = g_RenderManager.GetLighting();

	DWORD dZWriteEnable = g_RenderManager.GetZWriteEnable();
	DWORD dZEnable = g_RenderManager.GetZEnable();

	DWORD dZFunc = g_RenderManager.GetZFunc();
	DWORD dFVF = g_RenderManager.GetFVF();

	DWORD dZBias = g_RenderManager.GetZBias();

	g_RenderManager.SetAlphaBlendEnable( FALSE );

	g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureColorArg2( 0 , D3DTA_DIFFUSE );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
	g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
	g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_DIFFUSE );
	
	g_RenderManager.DisableTextureStageState( 1 );
	g_RenderManager.DisableTextureStageState( 2 );

	g_RenderManager.SetFogEnable( FALSE );
	g_RenderManager.SetLighting( FALSE );

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );

	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
	g_RenderManager.SetZBias( 0 );

	g_RenderManager.SetTexture( 0, pTex );
	g_RenderManager.SetFVF( ( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 ) );	
	g_RenderManager.DrawPrimitiveUP( D3DPT_TRIANGLEFAN , 2, vt, sizeof( SVertex ) );	
	
	pData->pRenderTexture->EndScene();

	if( bIsBeginScene )
	{
		g_RockClient.BeginScene();
	}
	
	g_RenderManager.SetAlphaBlendEnable( dAlphaBlendEnable );

	g_RenderManager.SetTextureColorOP  ( 0 , dColOp );
	g_RenderManager.SetTextureColorArg1( 0 , dColArg1 );
	g_RenderManager.SetTextureColorArg2( 0 , dColArg2 );
	
	g_RenderManager.SetTextureAlphaOP  ( 0 , dAlpOp );
	g_RenderManager.SetTextureAlphaArg1( 0 , dAlpArg1 );
	g_RenderManager.SetTextureAlphaArg2( 0 , dAlpArg2 );

	g_RenderManager.SetFogEnable( dFogEnable );
	g_RenderManager.SetLighting( dLighting );

	g_RenderManager.SetZWriteEnable( dZWriteEnable );
	g_RenderManager.SetZEnable( dZEnable );

	g_RenderManager.SetZFunc( dZFunc );
	g_RenderManager.SetZBias( dZBias );

	g_RenderManager.SetFVF( dFVF );

	return TRUE;
}
