#include "RockPCH.h"
#include "VFileSystem.h"

#include "AnimSequence.h"
#include "..\\Rockclient.h"
#include <assert.h>

#ifdef DIRECT_VERSION_9_MJH
extern LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern CRockClient			g_RockClient;

void CAnimSequence::LoadAni( char* dir )
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FILE		*fp;
		
	char FileName[ 1024 ] = "";
	char texName [ 1024 ] = "";
	char path[ 1024 ] = "";
	
	wsprintf( FileName , "%s\\Effect\\tex\\ani\\%s" , g_RockClient.GetRootDir() , dir );
	wsprintf( texName  , "%s\\Effect\\tex\\ani\\"   , g_RockClient.GetRootDir() );

	fp = fopen( FileName, "rb");

	assert( fp != NULL && "!이펙트 빌보드 파일 읽기 실패" );
	{
		int Count = 0;
		fread( &Count, sizeof(Count), 1, fp );
		fread( &m_fTimeGap, sizeof(m_fTimeGap), 1, fp );
		for( int i = 0; i < Count; i++ )
		{
			char name[ 256 ] = {0,};
			char path[ 256 ] = {0,};
			fread( &name, sizeof(name), 1, fp );
			sprintf( path, "%s\\%s", texName, name );
			AddFrame( path, m_fTimeGap );
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else ///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFEffectTexAni.OpenFile( dir );
//	if( pFH == NULL ) return false;
	assert( pFH != NULL && "!이펙트 빌보드 파일 읽기 실패" );
	{
		int Count = 0;

		pFH->Read( &Count, sizeof(Count) );
		pFH->Read( &m_fTimeGap, sizeof(m_fTimeGap) );

		for( int i = 0; i < Count; i++ )
		{
			char name[ 256 ] = {0,};

			pFH->Read( &name, sizeof(name) );
			AddFrame( name, m_fTimeGap );
		}
	}
	
	g_VFEffectTexAni.CloseFile( pFH );
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return;
}


bool CAnimSequence::AddFrame(const char *strFilename, float fTime)
{
	if (fTime <= 0) return(false);

	CAnimFrame *newframe = SAFE_NEW( CAnimFrame() );		

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// create a texture for this frame
	if( FAILED( g_RenderManager.CreateTextureFromFile( strFilename, &newframe->m_pTexture )))
	{
		SAFE_DELETE( newframe );	
		return(false);
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else ///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VFileHandle* pFH = g_VFEffectTexAni.OpenFile( strFilename );
	if( pFH )
	{
		g_RenderManager.CreateTextureFromFileInMemoryEx( pFH->GetData(), pFH->GetSize(),	/// 데이터 포인터, 사이즈 
														D3DX_DEFAULT, D3DX_DEFAULT, 
														1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														D3DX_FILTER_TRIANGLE | D3DX_FILTER_MIRROR,
														0, NULL, NULL, &newframe->m_pTexture );
		g_VFEffectTexAni.CloseFile( pFH );
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// add to vector
	newframe->m_fTime = fTime;
	m_vFrames.push_back(newframe);

	return(true);
}

CAnimSequence::CAnimSequence()
{

}

CAnimSequence::~CAnimSequence()
{
	ClearAllFrames();
}


void CAnimSequence::DeleteFrame(int index)
{
	CAnimFrame *pData = *(m_vFrames.begin() + index);
	SAFE_DELETE( pData );
	*(m_vFrames.begin() + index) = NULL;	

	m_vFrames.erase(m_vFrames.begin() + index);
}

void CAnimSequence::ClearAllFrames()
{
	if( m_vFrames.size() <= 0 ) return;
	
	std::vector<CAnimFrame *>::iterator i = m_vFrames.begin();
	
	for ( ; i != m_vFrames.end(); ++i)
	{
		CAnimFrame *pData = (*i);
		SAFE_DELETE( pData );
		(*i) = NULL;
	}

	m_vFrames.clear();
}



int CAnimSequence::GetCurFrame(CTimer &timer)
{
	int iCurFrame = 0; float fTimeCount = 0.0f;
	for (std::vector<CAnimFrame *>::iterator i = m_vFrames.begin(); i != m_vFrames.end(); ++i)
	{
		fTimeCount += (*i)->m_fTime;
		int timevalue = timer.GetTime() / ( (*i)->m_fTime * m_vFrames.size() );
		if (timer.GetTime() - timevalue * ( (*i)->m_fTime * m_vFrames.size() )<= fTimeCount)
			break;
		iCurFrame++;
	}

	if( GetSize() <= iCurFrame )
		return GetSize() - 1;

	return(iCurFrame);
}
