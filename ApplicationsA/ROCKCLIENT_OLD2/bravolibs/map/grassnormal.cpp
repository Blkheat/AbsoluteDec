#include "RockPCH.h"

#include	<windows.h>
#include	<stdio.h>
#include	<time.h>
#ifdef DIRECT_VERSION_9_MJH
#include	<d3d9.h>
#include	<d3dx9.h>
#else
#include	<d3d8.h>
#include	<d3dx8.h>
#endif // DIRECT_VERSION_9_MJH
#include	<mmsystem.h>
#include	"..\\quadlist.h"
#include	"GrassNormal.h"
#include	"GTexture.h"
#include    "..\\..\\CRenderManager.h"


#ifdef DIRECT_VERSION_9_MJH
extern	LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern	LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

bool	CGrassData::Load( short nTexIndex, float fHeight )
{
	m_nTexIdx = nTexIndex;
	m_fHeight = fHeight;

	if ( FAILED ( g_RenderManager.CreateIndexBuffer(12*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB) ) )
	{
///		MessageBox(NULL, "Error - Create(IB)", NULL, MB_OK);
		return	false;
	}
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(8*sizeof(VTXGRSNORMAL), D3DUSAGE_WRITEONLY, D3DFVF_GRSNORMAL, D3DPOOL_MANAGED, &m_pVB) ) )
	{
///		MessageBox(NULL, "Error - Create(VB)", NULL, MB_OK);
		return	false;
	}

	WORD *	m_pIdxList;
	VTXGRSNORMAL *		m_pVtxList;
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0, 0, (void**)&m_pIdxList, 0 ) ) )
#else
	if ( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&m_pIdxList, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock", "IB", MB_OK);
		return	false;
	}
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0, 0, (void**)&m_pVtxList, 0 ) ) )
#else
	if ( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&m_pVtxList, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - Lock", "VB", MB_OK);
		return	false;
	}
	WORD		wIdxCount	= 0;
	WORD		wVtxCount	= 0;

	/* FIRST FACE */
	m_pVtxList[0].x		= 0.0f;
	m_pVtxList[0].y		= 0.0f;
	m_pVtxList[0].z		= 4.0f;
	m_pVtxList[0].tu	= 0.01f;
	m_pVtxList[0].tv	= 1.0f;
	wVtxCount	++;
	m_pVtxList[1].x		= 8.0f;
	m_pVtxList[1].y		= 0.0f;
	m_pVtxList[1].z		= 4.0f;
	m_pVtxList[1].tu	= 1.0f;
	m_pVtxList[1].tv	= 1.0f;
	wVtxCount	++;
	m_pVtxList[2].x		= 0.0f;
	m_pVtxList[2].y		= m_fHeight;
	m_pVtxList[2].z		= 4.0f;
	m_pVtxList[2].tu	= 0.01f;
	m_pVtxList[2].tv	= 0.01f;
	wVtxCount	++;
	m_pVtxList[3].x		= 8.0f;
	m_pVtxList[3].y		= m_fHeight;
	m_pVtxList[3].z		= 4.0f;
	m_pVtxList[3].tu	= 1.0f;
	m_pVtxList[3].tv	= 0.01f;
	wVtxCount	++;

	m_pIdxList[wIdxCount++]	= wVtxCount-4;
	m_pIdxList[wIdxCount++]	= wVtxCount-3;
	m_pIdxList[wIdxCount++]	= wVtxCount-2;
	m_pIdxList[wIdxCount++]	= wVtxCount-3;
	m_pIdxList[wIdxCount++]	= wVtxCount-1;
	m_pIdxList[wIdxCount++]	= wVtxCount-2;

	// SECOND FACE 
	m_pVtxList[4].x		= 4.0f;
	m_pVtxList[4].y		= 0.0f;
	m_pVtxList[4].z		= 0.0f;
	m_pVtxList[4].tu	= 0.01f;
	m_pVtxList[4].tv	= 1.0f;
	wVtxCount	++;
	m_pVtxList[5].x		= 4.0f;
	m_pVtxList[5].y		= 0.0f;
	m_pVtxList[5].z		= 8.0f;
	m_pVtxList[5].tu	= 1.0f;
	m_pVtxList[5].tv	= 1.0f;
	wVtxCount	++;
	m_pVtxList[6].x		= 4.0f;
	m_pVtxList[6].y		= m_fHeight;
	m_pVtxList[6].z		= 0.0f;
	m_pVtxList[6].tu	= 0.01f;
	m_pVtxList[6].tv	= 0.01f;
	wVtxCount	++;
	m_pVtxList[7].x		= 4.0f;
	m_pVtxList[7].y		= m_fHeight;
	m_pVtxList[7].z		= 8.0f;
	m_pVtxList[7].tu	= 1.0f;
	m_pVtxList[7].tv	= 0.01f;
	wVtxCount	++;
 
	m_pIdxList[wIdxCount++]	= wVtxCount-4;
	m_pIdxList[wIdxCount++]	= wVtxCount-3;
	m_pIdxList[wIdxCount++]	= wVtxCount-2;
	m_pIdxList[wIdxCount++]	= wVtxCount-3;
	m_pIdxList[wIdxCount++]	= wVtxCount-1;
	m_pIdxList[wIdxCount++]	= wVtxCount-2;

	m_pIB->Unlock();
	m_pVB->Unlock();

	static	int	animCount	= 0;

	if ( animCount%4 == 0 )
	{
		float	fMaxAngle	= 20;	
		float	fMinAngle	= -20;
		float	fCurAngle	= 10;
		LONG	lFps		= 80;
		bool	bInc		= true;

		for ( int i = 0; i < 2; i ++ )
		{
			m_Key[i].fMaxAngle	= fMaxAngle;
			m_Key[i].fMinAngle	= fMinAngle;
			m_Key[i].fCurAngle	= fCurAngle;
			m_Key[i].lFps		= lFps;
			m_Key[i].lPreCount	= 0;
			m_Key[i].bDirInc	= bInc;
		}
		animCount	++;
	}
	else if ( animCount%4 == 1 )
	{
		float	fMaxAngle	= 20;	
		float	fMinAngle	= -20;
		float	fCurAngle	= 10;
		LONG	lFps		= 90;
		bool	bInc		= false;

		for ( int i = 0; i < 2; i ++ )
		{
			m_Key[i].fMaxAngle	= fMaxAngle;
			m_Key[i].fMinAngle	= fMinAngle;
			m_Key[i].fCurAngle	= fCurAngle;
			m_Key[i].lFps		= lFps;
			m_Key[i].lPreCount	= 0;
			m_Key[i].bDirInc	= bInc;
		}
		animCount	++;
	}
	else if ( animCount%4 == 2 )
	{
		float	fMaxAngle	= 18;	
		float	fMinAngle	= -16;
		float	fCurAngle	= 3;
		LONG	lFps		= 85;
		bool	bInc		= false;

		for ( int i = 0; i < 2; i ++ )
		{
			m_Key[i].fMaxAngle	= fMaxAngle;
			m_Key[i].fMinAngle	= fMinAngle;
			m_Key[i].fCurAngle	= fCurAngle;
			m_Key[i].lFps		= lFps;
			m_Key[i].lPreCount	= 0;
			m_Key[i].bDirInc	= bInc;
		}
		animCount	++;
	}
	else if ( animCount%4 == 3 )
	{
		float	fMaxAngle	= 18;	
		float	fMinAngle	= -23;
		float	fCurAngle	= 17;
		LONG	lFps		= 120;
		bool	bInc		= true;

		for ( int i = 0; i < 2; i ++ )
		{
			m_Key[i].fMaxAngle	= fMaxAngle;
			m_Key[i].fMinAngle	= fMinAngle;
			m_Key[i].fCurAngle	= fCurAngle;
			m_Key[i].lFps		= lFps;
			m_Key[i].lPreCount	= 0;
			m_Key[i].bDirInc	= bInc;
		}
		animCount	++;
	}

	return	true;
}

void	CGrassData::Update()
{
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pIB->Lock(0, 0, (void**)&m_pIdxList, 0 ) ) )
#else
	if ( FAILED ( m_pIB->Lock(0, 0, (BYTE**)&m_pIdxList, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - IB Lock", NULL, MB_OK);
		return;
	}
#ifdef DIRECT_VERSION_9_MJH
	if ( FAILED ( m_pVB->Lock(0, 0, (void**)&m_pVtxList, 0 ) ) )
#else
	if ( FAILED ( m_pVB->Lock(0, 0, (BYTE**)&m_pVtxList, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
///		MessageBox(NULL, "Error - VB Lock", NULL, MB_OK);
		return;
	}

	float			fBAngle	= 90.0f;
	double			fAngle	= 0.0f;
	float			fx, fy;
	WORD			wIdxCount	= 0;
	WORD			wVtxCount	= 0;

	for ( WORD i = 0; i < 4; i ++ )
		m_Key[i].UpdateFrame();

	fAngle	= (fBAngle-m_Key[0].fCurAngle ) * 0.01745;
	fx		= cosf((float)fAngle) * m_fHeight;
	fy		= sinf((float)fAngle) * m_fHeight;

	m_pVtxList[wVtxCount].x	= 0.0f;
	m_pVtxList[wVtxCount].y	= 0.0f;
	m_pVtxList[wVtxCount].z	= 4.0f;
	wVtxCount	++;
	m_pVtxList[wVtxCount].x	= 8.0f;
	m_pVtxList[wVtxCount].y	= 0.0f;
	m_pVtxList[wVtxCount].z	= 4.0f;
	wVtxCount	++;
	m_pVtxList[wVtxCount].x	= fx;
	m_pVtxList[wVtxCount].y	= fy;
	m_pVtxList[wVtxCount].z	= 4.0f;
	wVtxCount	++;
	m_pVtxList[wVtxCount].x	= fx + 10.0f;
	m_pVtxList[wVtxCount].y	= fy;
	m_pVtxList[wVtxCount].z	= 4.0f;
	wVtxCount	++;

	m_pIdxList[wIdxCount++]	= wVtxCount-4;
	m_pIdxList[wIdxCount++]	= wVtxCount-3;
	m_pIdxList[wIdxCount++]	= wVtxCount-2;
	m_pIdxList[wIdxCount++]	= wVtxCount-3;
	m_pIdxList[wIdxCount++]	= wVtxCount-1;
	m_pIdxList[wIdxCount++]	= wVtxCount-2;

	fAngle	= (fBAngle-m_Key[1].fCurAngle) * 0.01745;
	fx		= cosf((float)fAngle) * m_fHeight;
	fy		= sinf((float)fAngle) * m_fHeight;

	m_pVtxList[wVtxCount].x	= 4.0f;
	m_pVtxList[wVtxCount].y	= 0.0f;
	m_pVtxList[wVtxCount].z	= 0.0f;
	wVtxCount	++;
	m_pVtxList[wVtxCount].x	= 4.0f;
	m_pVtxList[wVtxCount].y	= 0.0f;
	m_pVtxList[wVtxCount].z	= 8.0f;
	wVtxCount	++;
	m_pVtxList[wVtxCount].x	= fx + (m_fHeight/4);
	m_pVtxList[wVtxCount].y	= fy;
	m_pVtxList[wVtxCount].z	= 0.0f;
	wVtxCount	++;
	m_pVtxList[wVtxCount].x	= fx + (m_fHeight/4	);

	m_pVtxList[wVtxCount].y	= fy;
	m_pVtxList[wVtxCount].z	= 8.0f;
	wVtxCount	++;

	m_pIdxList[wIdxCount++]	= wVtxCount-4;
	m_pIdxList[wIdxCount++]	= wVtxCount-3;
	m_pIdxList[wIdxCount++]	= wVtxCount-2;
	m_pIdxList[wIdxCount++]	= wVtxCount-3;
	m_pIdxList[wIdxCount++]	= wVtxCount-1;
	m_pIdxList[wIdxCount++]	= wVtxCount-2;

	m_pIB->Unlock();
	m_pVB->Unlock();
}

void	CGrassList::Display()
{
	if ( m_pData == NULL )
		return;

//	Update();

	g_RenderManager.SetTexture(0, g_GTexture.GetTexture(m_pData->m_nTexIdx));

	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matPos);

	g_RenderManager.SetStreamSource(0, m_pData->m_pVB, sizeof(VTXGRSNORMAL));
	g_RenderManager.SetFVF(D3DFVF_GRSNORMAL);
	g_RenderManager.SetIndices(m_pData->m_pIB, 0);

	g_RenderManager.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 8, 0, 4);
}


bool	KEYGRSNORMAL::UpdateFrame()
{
//	if ( lFps == 0 )
//		return	false;

	LONG	lCurCount	= timeGetTime();

	float value;
	value = (float)(((float)lCurCount - (float)lFps) / 4500.0f);


	if ( fCurAngle < fMinAngle )
	{
		fCurAngle = fMinAngle;
		bDirInc	= true;
	}
	else if ( fCurAngle > fMaxAngle )
	{
		fCurAngle = fMaxAngle;
		bDirInc	= false;
	}


	if (bDirInc)
	{
		fCurAngle += (fMaxAngle - fMinAngle) * value;
	}
	else
	{
		fCurAngle -= (fMaxAngle - fMinAngle) * value;
	}
	lFps = lCurCount;

	return	true;
}
