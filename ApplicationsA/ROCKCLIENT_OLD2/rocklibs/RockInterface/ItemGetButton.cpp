#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "Rui.h"
#include "..\\..\\GTimer.h"

#include	"..\\..\\quadlist.h"
#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	<Obj\\ObjBase.h>
#include	<Map\\Field.h>

#include	"..\\..\\Pc.h"
#include "..\\..\\global.h"

#include "ItemGetButton.h"

#include "..\\..\\CRenderManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

CItemTransButton::CItemTransButton( SDesktop* DT ) : CButton( DT )
{
	m_NPCTItemQueueInfo.IsActive = true;
	m_2DPosQueueInfo.IsActive = true;
	
	m_fMonGetStep_01ExTime = 0.6f;
	m_fMonGetStep_02ExTime = 0.2f;
	m_fMonGetStep_03ExTime = 0.6f;
	m_fMonGetStep_04ExTime = 0.25f;

	m_fNpcGetStep_01ExTime = 0.4f;	 
	m_fNpcGetStep_02ExTime = 0.6f;	 
	m_fNpcGetStep_03ExTime = 0.1f;

	m_fGlowTime = 0.1f;
	
	m_pItemTransQueueInfo = NULL;	

	m_bButtonGlow = false;

	m_IsInit = false;	
}

CItemTransButton::~CItemTransButton()
{
	SItemTransQueueInfo *pItemTransQueueInfo = NULL;
	
	if(m_MonsterTItemQueueInfoList.size())
	{
	   list	< SItemTransQueueInfo * >::iterator	pi = m_MonsterTItemQueueInfoList.begin();
	   
	   for( ; pi != m_MonsterTItemQueueInfoList.end() ; ++pi )
	   {   
		  pItemTransQueueInfo = (SItemTransQueueInfo *)(*pi);		  	  
		  SAFE_DELETE(pItemTransQueueInfo);		  
	   }	
	}

	m_MonsterTItemQueueInfoList.clear();

	m_NPCTItemQueueInfo.ItemTransQueue.Clear();	
	m_2DPosQueueInfo.ItemTransQueue.Clear();
}

void CItemTransButton::InitData()
{
	SItemTransQueueInfo *pItemTransQueueInfo = NULL;
	
	if(m_MonsterTItemQueueInfoList.size())
	{
	   list	< SItemTransQueueInfo * >::iterator	pi = m_MonsterTItemQueueInfoList.begin();
	   
	   for( ; pi != m_MonsterTItemQueueInfoList.end() ; ++pi )
	   {   
		  pItemTransQueueInfo = (SItemTransQueueInfo *)(*pi);		  	  
		  SAFE_DELETE(pItemTransQueueInfo);
	   }	
	}
	m_MonsterTItemQueueInfoList.clear();	
	
	m_NPCTItemQueueInfo.ItemTransQueue.Clear();
	m_2DPosQueueInfo.ItemTransQueue.Clear();

	m_pItemTransQueueInfo = NULL;

	m_bButtonGlow = false;
	m_fBGCumulTime = 0.0f;

	m_IsInit = true;

}

void CItemTransButton::UpdateProc()
{
	vector < list < SItemTransQueueInfo * >::iterator > templist;
	SItemTransQueueInfo *pItemTransQueueInfo = NULL;
	
	if(m_MonsterTItemQueueInfoList.size())
	{
	   list	< SItemTransQueueInfo * >::iterator	pi = m_MonsterTItemQueueInfoList.begin();
	   
	   for( ; pi != m_MonsterTItemQueueInfoList.end() ; ++pi )
	   {   
		  pItemTransQueueInfo = (SItemTransQueueInfo *)(*pi);		  	  
		  if( ( 0 == pItemTransQueueInfo->ItemTransQueue.GetCurQueueSize() ) &&
			  ( ITemTransEffStep_None == pItemTransQueueInfo->uiNowEffStep ) )
		  {
			  templist.push_back(pi);	
		  }
	   }	
	}

	for(int i = 0 ; i < templist.size() ; i++)
	{
		pItemTransQueueInfo = (SItemTransQueueInfo *)(*templist[i]);
		SAFE_DELETE(pItemTransQueueInfo);
		(*templist[i]) = NULL;
		
		m_MonsterTItemQueueInfoList.erase(templist[i]);
	}
}

void CItemTransButton::RenderProc()
{
	if(!m_IsInit)
	{
		InitData();		
	}
	
	UpdateProc();
	
	DWORD SrcBlend, DestBlend;	

	if(m_bButtonGlow)
	{
	   g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	   g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );		
	   
  	   g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
 	   g_RenderManager.SetDestBlend( D3DBLEND_ONE );
	   SetClientImage(TID_OPT_QUEST_BT_CLK);	
	}

	CButton::RenderProc();

	if(m_bButtonGlow)
	{
  	   g_RenderManager.SetSrcBlend( SrcBlend );
 	   g_RenderManager.SetDestBlend( DestBlend );
	   SetClientImage(TID_None);	
	   
	   m_fBGCumulTime += g_GTimer.GetFrameElapsedSecTime();
	   if(m_fBGCumulTime > m_fGlowTime)
	   {
		  m_fBGCumulTime = 0.0f;
		  m_bButtonGlow = false; 
	   }
	}

	SItemTransQueueInfo *pItemTransQueueInfo = NULL;
	
	if(m_MonsterTItemQueueInfoList.size())
	{
	   list	< SItemTransQueueInfo * >::iterator	pi = m_MonsterTItemQueueInfoList.begin();
	   
	   for( ; pi != m_MonsterTItemQueueInfoList.end() ; ++pi )
	   {   
		  pItemTransQueueInfo = (SItemTransQueueInfo *)(*pi);		  	  
		  RenderItemTransProc( pItemTransQueueInfo );
	   }	
	}

	RenderItemTransProc( &m_NPCTItemQueueInfo );
	RenderItemTransProc( &m_2DPosQueueInfo );
}

void CItemTransButton::RenderItemTransProc( SItemTransQueueInfo * pItemTransQueueInfo )
{
	m_pItemTransQueueInfo = pItemTransQueueInfo;

	if(NULL == m_pItemTransQueueInfo)
	{
		return;
	}

	if(false == m_pItemTransQueueInfo->IsActive)
	{
		return;
	}
	
	if( m_pItemTransQueueInfo->uiNowEffStep == ITemTransEffStep_None )
	{
		if(m_pItemTransQueueInfo->ItemTransQueue.GetCurQueueSize())
		{
			m_pItemTransQueueInfo->ItemTransQueue.PopData(m_pItemTransQueueInfo->ItemTransInfo);	
			m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_01;
			//연출동작
		}
		else
		{
			return;
		}	
	}
	
	m_pItemTransQueueInfo->fCumulativeTime += g_GTimer.GetFrameElapsedSecTime();	

	switch(m_pItemTransQueueInfo->uiNowEffStep)
	{
		case ITemTransEffStep_01:
			{
				if(m_pItemTransQueueInfo->ItemTransInfo.Is3DPos)
				{
					if(m_pItemTransQueueInfo->ItemTransInfo.isMonster)
					{
						RenderMonGetEffStep_01();					
					}
					else
					{
						RenderNpcGetEffStep_01();
					}
				}
				else
				{
					if(m_pItemTransQueueInfo->ItemTransInfo.isMonster)
					{
						Render2DPosGetEffStep_01();			
					}
					else
					{
						RenderNpcGetEffStep_01();
					}
				}
			}
			break;
		case ITemTransEffStep_02:
			{
				if(m_pItemTransQueueInfo->ItemTransInfo.Is3DPos)
				{
					if(m_pItemTransQueueInfo->ItemTransInfo.isMonster)
					{
						RenderMonGetEffStep_02();					
					}
					else
					{
						RenderNpcGetEffStep_02();
					}
				}
				else
				{
					if(m_pItemTransQueueInfo->ItemTransInfo.isMonster)
					{
						Render2DPosGetEffStep_02();					
					}
					else
					{
						RenderNpcGetEffStep_02();
					}
				}
			}
			break;
		case ITemTransEffStep_03:
			{
				if(m_pItemTransQueueInfo->ItemTransInfo.Is3DPos)
				{
					if(m_pItemTransQueueInfo->ItemTransInfo.isMonster)
					{
						RenderMonGetEffStep_03();					
					}
					else
					{
						RenderNpcGetEffStep_03();
					}
				}
				else
				{
					if(m_pItemTransQueueInfo->ItemTransInfo.isMonster)
					{
						Render2DPosGetEffStep_03();					
					}
					else
					{
						RenderNpcGetEffStep_03();
					}
				}
			}
			break;
		case ITemTransEffStep_04:
			{
				if(m_pItemTransQueueInfo->ItemTransInfo.Is3DPos)
				{
					RenderMonGetEffStep_04();
				}
				else
				{
					Render2DPosGetEffStep_04();	
				}
			}
			break;
	}	

	DWORD SrcBlend, DestBlend;
	
	if(m_pItemTransQueueInfo->bRenderGlow)
	{
	   g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	   g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );		
	   
	   g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	   g_RenderManager.SetDestBlend( D3DBLEND_ONE );
	}

	if(m_pItemTransQueueInfo->ItemTransInfo.isMoney)
	{
	   Render->ScretchDraw( TID_EldIMG , 
							m_pItemTransQueueInfo->nCx, 
							m_pItemTransQueueInfo->nCy, 
							m_pItemTransQueueInfo->nCw, 
							m_pItemTransQueueInfo->nCh, 
							m_pItemTransQueueInfo->dColor );
	}
	else
	{
	   Render->ScretchDraw( m_pItemTransQueueInfo->ItemTransInfo.pcInfo.Tid , 
							m_pItemTransQueueInfo->nCx, 
							m_pItemTransQueueInfo->nCy, 
							m_pItemTransQueueInfo->nCw, 
							m_pItemTransQueueInfo->nCh, 
							m_pItemTransQueueInfo->dColor );
	}
	
	if(m_pItemTransQueueInfo->bRenderGlow)
	{
	   g_RenderManager.SetSrcBlend( SrcBlend );
	   g_RenderManager.SetDestBlend( DestBlend );

	   m_pItemTransQueueInfo->fGCumulTime += g_GTimer.GetFrameElapsedSecTime();
	   if(m_pItemTransQueueInfo->fGCumulTime > m_fGlowTime)
	   {
		  m_pItemTransQueueInfo->fGCumulTime = 0.0f;
		  m_pItemTransQueueInfo->bRenderGlow = false; 
	   }
	}
}

void CItemTransButton::RenderMonGetEffStep_01()
{
	D3DXVECTOR3		tpos;
#ifdef DIRECT_VERSION_9_MJH
	D3DVIEWPORT9	viewport;
#else
	D3DVIEWPORT8	viewport;
#endif // DIRECT_VERSION_9_MJH	
	D3DXMATRIX		matchar;
	D3DXMATRIX		matproj;
	D3DXMATRIX		matview;
	D3DXMATRIX		matworld;

	D3DXMatrixIdentity( &matproj );
	D3DXMatrixIdentity( &matview );
	D3DXMatrixIdentity( &matworld );
	g_RenderManager.GetTransform( D3DTS_PROJECTION, &matproj );
	g_RenderManager.GetTransform( D3DTS_VIEW, &matview );
	g_RenderManager.GetViewport( &viewport );

	D3DXVECTOR3 upPos = m_pItemTransQueueInfo->ItemTransInfo.upPosition;

	D3DXVec3Project( &tpos, &upPos, &viewport, &matproj, &matview, &matworld );

	m_pItemTransQueueInfo->SPos.x = tpos.x;
	m_pItemTransQueueInfo->SPos.y = tpos.y;

	m_pItemTransQueueInfo->EPos.x = m_pItemTransQueueInfo->SPos.x;
	m_pItemTransQueueInfo->EPos.y = m_pItemTransQueueInfo->SPos.y - 15;		
	

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fMonGetStep_01ExTime;	

	if(m_pItemTransQueueInfo->fCumulativeTime > m_fMonGetStep_01ExTime)
	{
		m_pItemTransQueueInfo->fCumulativeTime -= m_fMonGetStep_01ExTime;
		fRatio = 1.0f;
		m_pItemTransQueueInfo->bRenderGlow = true;
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_02;

		m_pItemTransQueueInfo->TempPos.x = m_pItemTransQueueInfo->EPos.x;
		m_pItemTransQueueInfo->TempPos.y = m_pItemTransQueueInfo->EPos.y;
	}	

	m_pItemTransQueueInfo->nCw = 13 + ( fRatio * (52 - 13));
	m_pItemTransQueueInfo->nCh = 13 + ( fRatio * (52 - 13));
	if((m_pItemTransQueueInfo->nCw % 2) != 0)
		m_pItemTransQueueInfo->nCw += 1;
	if((m_pItemTransQueueInfo->nCh % 2) != 0)
		m_pItemTransQueueInfo->nCh += 1;

	int xPos = m_pItemTransQueueInfo->SPos.x + 
			  ( fRatio * (m_pItemTransQueueInfo->EPos.x - m_pItemTransQueueInfo->SPos.x) );
	int yPos = m_pItemTransQueueInfo->SPos.y + 
			  ( fRatio * (m_pItemTransQueueInfo->EPos.y - m_pItemTransQueueInfo->SPos.y) );

	m_pItemTransQueueInfo->nCx = xPos - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = yPos - ( m_pItemTransQueueInfo->nCh/2 );
	
	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB((int)(1 * 255), 255, 255, 255 );
}

void CItemTransButton::RenderMonGetEffStep_02()
{
	D3DXVECTOR3		tpos;
#ifdef DIRECT_VERSION_9_MJH
	D3DVIEWPORT9	viewport;
#else
	D3DVIEWPORT8	viewport;
#endif // DIRECT_VERSION_9_MJH	
	D3DXMATRIX		matchar;
	D3DXMATRIX		matproj;
	D3DXMATRIX		matview;
	D3DXMATRIX		matworld;

	D3DXMatrixIdentity( &matproj );
	D3DXMatrixIdentity( &matview );
	D3DXMatrixIdentity( &matworld );
	g_RenderManager.GetTransform( D3DTS_PROJECTION, &matproj );
	g_RenderManager.GetTransform( D3DTS_VIEW, &matview );
	g_RenderManager.GetViewport( &viewport );

	D3DXVECTOR3 upPos = m_pItemTransQueueInfo->ItemTransInfo.upPosition;

	D3DXVec3Project( &tpos, &upPos, &viewport, &matproj, &matview, &matworld );	

	
	m_pItemTransQueueInfo->TempPos.x = tpos.x;
	m_pItemTransQueueInfo->TempPos.y = tpos.y;	

	m_pItemTransQueueInfo->TempPos.y -= 15;

	POINT sPos = m_pItemTransQueueInfo->TempPos;

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fMonGetStep_02ExTime;

	if(m_pItemTransQueueInfo->fCumulativeTime > m_fMonGetStep_02ExTime)
	{
		m_pItemTransQueueInfo->fCumulativeTime -= m_fMonGetStep_02ExTime;
		fRatio = 1.0f;		
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_03;

		m_pItemTransQueueInfo->TempPos.x = sPos.x;
		m_pItemTransQueueInfo->TempPos.y = sPos.y;
	}

	m_pItemTransQueueInfo->nCw = 52 - ( fRatio * (52 - 35));
	m_pItemTransQueueInfo->nCh = 52 - ( fRatio * (52 - 35));
	if((m_pItemTransQueueInfo->nCw % 2) != 0)
		m_pItemTransQueueInfo->nCw += 1;
	if((m_pItemTransQueueInfo->nCh % 2) != 0)
		m_pItemTransQueueInfo->nCh += 1;

	m_pItemTransQueueInfo->nCx = sPos.x - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = sPos.y - ( m_pItemTransQueueInfo->nCh/2 );

	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
}

void CItemTransButton::RenderMonGetEffStep_03()
{
	
	D3DXVECTOR3		tpos;
#ifdef DIRECT_VERSION_9_MJH
	D3DVIEWPORT9	viewport;
#else
	D3DVIEWPORT8	viewport;
#endif // DIRECT_VERSION_9_MJH	
	D3DXMATRIX		matchar;
	D3DXMATRIX		matproj;
	D3DXMATRIX		matview;
	D3DXMATRIX		matworld;

	D3DXMatrixIdentity( &matproj );
	D3DXMatrixIdentity( &matview );
	D3DXMatrixIdentity( &matworld );
	g_RenderManager.GetTransform( D3DTS_PROJECTION, &matproj );
	g_RenderManager.GetTransform( D3DTS_VIEW, &matview );
	g_RenderManager.GetViewport( &viewport );

	D3DXVECTOR3 upPos = m_pItemTransQueueInfo->ItemTransInfo.upPosition;

	D3DXVec3Project( &tpos, &upPos, &viewport, &matproj, &matview, &matworld );

	
	m_pItemTransQueueInfo->TempPos.x = tpos.x;
	m_pItemTransQueueInfo->TempPos.y = tpos.y;

	m_pItemTransQueueInfo->TempPos.y -= 15;
	
	POINT sPos = m_pItemTransQueueInfo->TempPos;

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fMonGetStep_03ExTime;

	if(m_pItemTransQueueInfo->fCumulativeTime > m_fMonGetStep_03ExTime)
	{
		m_pItemTransQueueInfo->fCumulativeTime -= m_fMonGetStep_03ExTime;
		fRatio = 1.0f;		
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_04;

		m_pItemTransQueueInfo->TempPos.x = sPos.x;
		m_pItemTransQueueInfo->TempPos.y = sPos.y;
	}

	m_pItemTransQueueInfo->nCw = 35;
	m_pItemTransQueueInfo->nCh = 35;

	m_pItemTransQueueInfo->nCx = sPos.x - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = sPos.y - ( m_pItemTransQueueInfo->nCh/2 );

	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
}

void CItemTransButton::RenderMonGetEffStep_04()
{
	POINT sPos = m_pItemTransQueueInfo->TempPos;	

	POINT ePos;
	ePos.x = Cx + (Cw / 2);
	ePos.y = Cy + (Ch / 2);

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fMonGetStep_04ExTime;
	float fARatio = 1 - (m_pItemTransQueueInfo->fCumulativeTime / m_fMonGetStep_04ExTime);
	
	if(m_pItemTransQueueInfo->fCumulativeTime > m_fMonGetStep_04ExTime)
	{
		m_pItemTransQueueInfo->fCumulativeTime = 0.0f;
		fRatio = 1.0f;
		fARatio = 0.0f;
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_None;
		m_bButtonGlow = true;
		m_fBGCumulTime = 0.0f;
	}

    m_pItemTransQueueInfo->nCw = 35; 
	m_pItemTransQueueInfo->nCh = 35;

	//여기서 속도를 구해서 누적시킴	
	D3DXVECTOR2 dir;
	dir.x = ePos.x - sPos.x;
	dir.y = ePos.y - sPos.y;
	D3DXVec2Normalize(&dir, &dir);
	
	int xPos = sPos.x + 2500 * (dir.x * m_pItemTransQueueInfo->fCumulativeTime);
	int yPos = sPos.y + 2500 * (dir.y * m_pItemTransQueueInfo->fCumulativeTime);

	m_pItemTransQueueInfo->nCx = xPos - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = yPos - ( m_pItemTransQueueInfo->nCh/2 );

	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB((int)(fARatio * 255), 255, 255, 255 );	
}

void CItemTransButton::RenderNpcGetEffStep_01()
{
	POINT sPos;
	sPos.x = Cx + Cw;
	sPos.y = Cy - (Ch + Ch/2);

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fNpcGetStep_01ExTime;

	if( m_pItemTransQueueInfo->fCumulativeTime > m_fNpcGetStep_01ExTime)
	{
		m_pItemTransQueueInfo->fCumulativeTime -= m_fNpcGetStep_01ExTime;
		fRatio = 1.0f;		
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_02;
		m_pItemTransQueueInfo->bRenderGlow = true;

		m_pItemTransQueueInfo->TempPos.x = sPos.x;
		m_pItemTransQueueInfo->TempPos.y = sPos.y;
	}

	m_pItemTransQueueInfo->nCw = 13 + ( fRatio * (35 - 13));
	m_pItemTransQueueInfo->nCh = 13 + ( fRatio * (35 - 13));
	if((m_pItemTransQueueInfo->nCw % 2) != 0)
		m_pItemTransQueueInfo->nCw += 1;
	if((m_pItemTransQueueInfo->nCh % 2) != 0)
		m_pItemTransQueueInfo->nCh += 1;

	m_pItemTransQueueInfo->nCx = sPos.x - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = sPos.y - ( m_pItemTransQueueInfo->nCh/2 );

	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB((int)(fRatio * 255), 255, 255, 255 );
}

void CItemTransButton::RenderNpcGetEffStep_02()
{
	POINT sPos = m_pItemTransQueueInfo->TempPos;	

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fNpcGetStep_02ExTime;

	if(m_pItemTransQueueInfo->fCumulativeTime > m_fNpcGetStep_02ExTime)
	{
		m_pItemTransQueueInfo->fCumulativeTime -= m_fNpcGetStep_02ExTime;
		fRatio = 1.0f;		
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_03;

		m_pItemTransQueueInfo->TempPos.x = sPos.x;
		m_pItemTransQueueInfo->TempPos.y = sPos.y;
	}

	m_pItemTransQueueInfo->nCw = 35;
	m_pItemTransQueueInfo->nCh = 35;

	m_pItemTransQueueInfo->nCx = sPos.x - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = sPos.y - ( m_pItemTransQueueInfo->nCh/2 );

	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
}


void CItemTransButton::RenderNpcGetEffStep_03()
{
	
	POINT sPos = m_pItemTransQueueInfo->TempPos;
	POINT ePos;
	ePos.x = Cx + (Cw / 2);
	ePos.y = Cy + (Ch / 2);

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fNpcGetStep_03ExTime;
	float fARatio = 1 - (m_pItemTransQueueInfo->fCumulativeTime / m_fNpcGetStep_03ExTime);

	if( m_pItemTransQueueInfo->fCumulativeTime > m_fNpcGetStep_03ExTime )
	{
		m_pItemTransQueueInfo->fCumulativeTime = 0.0f;
		fRatio = 1.0f;		
		fARatio = 0.0f;
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_None;
		m_bButtonGlow = true;
		m_fBGCumulTime = 0.0f;		
	}

	m_pItemTransQueueInfo->nCw = 35 - ( fRatio * (35 - 13));
	m_pItemTransQueueInfo->nCh = 35 - ( fRatio * (35 - 13));
	if((m_pItemTransQueueInfo->nCw % 2) != 0)
		m_pItemTransQueueInfo->nCw += 1;
	if((m_pItemTransQueueInfo->nCh % 2) != 0)
		m_pItemTransQueueInfo->nCh += 1;

	int xPos = sPos.x + ( fRatio *(ePos.x - sPos.x) );
	int yPos = sPos.y + ( fRatio *(ePos.y - sPos.y) );

	m_pItemTransQueueInfo->nCx = xPos - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = yPos - ( m_pItemTransQueueInfo->nCh/2 );

	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB((int)(fARatio * 255), 255, 255, 255 );
}

void CItemTransButton::Render2DPosGetEffStep_01()
{
	m_pItemTransQueueInfo->SPos.x = m_pItemTransQueueInfo->ItemTransInfo.upPos.x;
	m_pItemTransQueueInfo->SPos.y = m_pItemTransQueueInfo->ItemTransInfo.upPos.y;

	m_pItemTransQueueInfo->EPos.x = m_pItemTransQueueInfo->SPos.x;
	m_pItemTransQueueInfo->EPos.y = m_pItemTransQueueInfo->SPos.y - 15;	

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fMonGetStep_01ExTime;	

	if(m_pItemTransQueueInfo->fCumulativeTime > m_fMonGetStep_01ExTime)
	{
		m_pItemTransQueueInfo->fCumulativeTime -= m_fMonGetStep_01ExTime;
		fRatio = 1.0f;
		m_pItemTransQueueInfo->bRenderGlow = true;
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_02;

		m_pItemTransQueueInfo->TempPos.x = m_pItemTransQueueInfo->EPos.x;
		m_pItemTransQueueInfo->TempPos.y = m_pItemTransQueueInfo->EPos.y;
	}	

	m_pItemTransQueueInfo->nCw = 13 + ( fRatio * (52 - 13));
	m_pItemTransQueueInfo->nCh = 13 + ( fRatio * (52 - 13));
	if((m_pItemTransQueueInfo->nCw % 2) != 0)
		m_pItemTransQueueInfo->nCw += 1;
	if((m_pItemTransQueueInfo->nCh % 2) != 0)
		m_pItemTransQueueInfo->nCh += 1;

	int xPos = m_pItemTransQueueInfo->SPos.x + 
			  ( fRatio * (m_pItemTransQueueInfo->EPos.x - m_pItemTransQueueInfo->SPos.x) );
	int yPos = m_pItemTransQueueInfo->SPos.y + 
			  ( fRatio * (m_pItemTransQueueInfo->EPos.y - m_pItemTransQueueInfo->SPos.y) );

	m_pItemTransQueueInfo->nCx = xPos - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = yPos - ( m_pItemTransQueueInfo->nCh/2 );
	
	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB((int)(1 * 255), 255, 255, 255 );
}

void CItemTransButton::Render2DPosGetEffStep_02()
{
	POINT sPos = m_pItemTransQueueInfo->TempPos;

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fMonGetStep_02ExTime;

	if(m_pItemTransQueueInfo->fCumulativeTime > m_fMonGetStep_02ExTime)
	{
		m_pItemTransQueueInfo->fCumulativeTime -= m_fMonGetStep_02ExTime;
		fRatio = 1.0f;		
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_03;

		m_pItemTransQueueInfo->TempPos.x = sPos.x;
		m_pItemTransQueueInfo->TempPos.y = sPos.y;
	}

	m_pItemTransQueueInfo->nCw = 52 - ( fRatio * (52 - 35));
	m_pItemTransQueueInfo->nCh = 52 - ( fRatio * (52 - 35));
	if((m_pItemTransQueueInfo->nCw % 2) != 0)
		m_pItemTransQueueInfo->nCw += 1;
	if((m_pItemTransQueueInfo->nCh % 2) != 0)
		m_pItemTransQueueInfo->nCh += 1;

	m_pItemTransQueueInfo->nCx = sPos.x - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = sPos.y - ( m_pItemTransQueueInfo->nCh/2 );

	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
}

void CItemTransButton::Render2DPosGetEffStep_03()
{
	POINT sPos = m_pItemTransQueueInfo->TempPos;

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fMonGetStep_03ExTime;

	if(m_pItemTransQueueInfo->fCumulativeTime > m_fMonGetStep_03ExTime)
	{
		m_pItemTransQueueInfo->fCumulativeTime -= m_fMonGetStep_03ExTime;
		fRatio = 1.0f;		
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_04;

		m_pItemTransQueueInfo->TempPos.x = sPos.x;
		m_pItemTransQueueInfo->TempPos.y = sPos.y;
	}

	m_pItemTransQueueInfo->nCw = 35;
	m_pItemTransQueueInfo->nCh = 35;

	m_pItemTransQueueInfo->nCx = sPos.x - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = sPos.y - ( m_pItemTransQueueInfo->nCh/2 );

	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
}

void CItemTransButton::Render2DPosGetEffStep_04()
{
	POINT sPos = m_pItemTransQueueInfo->TempPos;	

	POINT ePos;
	ePos.x = Cx + (Cw / 2);
	ePos.y = Cy + (Ch / 2);

	float fRatio = m_pItemTransQueueInfo->fCumulativeTime / m_fMonGetStep_04ExTime;
	float fARatio = 1 - (m_pItemTransQueueInfo->fCumulativeTime / m_fMonGetStep_04ExTime);
	
	if(m_pItemTransQueueInfo->fCumulativeTime > m_fMonGetStep_04ExTime)
	{
		m_pItemTransQueueInfo->fCumulativeTime = 0.0f;
		fRatio = 1.0f;
		fARatio = 0.0f;
		m_pItemTransQueueInfo->uiNowEffStep = ITemTransEffStep_None;
		m_bButtonGlow = true;
		m_fBGCumulTime = 0.0f;
	}

    m_pItemTransQueueInfo->nCw = 35; 
	m_pItemTransQueueInfo->nCh = 35;

	//여기서 속도를 구해서 누적시킴	
	D3DXVECTOR2 dir;
	dir.x = ePos.x - sPos.x;
	dir.y = ePos.y - sPos.y;
	D3DXVec2Normalize(&dir, &dir);
	
	int xPos = sPos.x + 2500 * (dir.x * m_pItemTransQueueInfo->fCumulativeTime);
	int yPos = sPos.y + 2500 * (dir.y * m_pItemTransQueueInfo->fCumulativeTime);

	m_pItemTransQueueInfo->nCx = xPos - ( m_pItemTransQueueInfo->nCw/2 );
	m_pItemTransQueueInfo->nCy = yPos - ( m_pItemTransQueueInfo->nCh/2 );

	m_pItemTransQueueInfo->dColor = D3DCOLOR_ARGB((int)(fARatio * 255), 255, 255, 255 );
}

void CItemTransButton::InsertItemTransInfo(SItemTransInfo ItemTranseInfo)
{
	if(!ItemTranseInfo.Is3DPos)
	{
		if(ItemTranseInfo.isMonster)	//몬스터이면...
		{
			m_2DPosQueueInfo.ItemTransQueue.PushData(ItemTranseInfo);
		}
		else
		{	
			m_NPCTItemQueueInfo.ItemTransQueue.PushData(ItemTranseInfo);
		}	

		return;
	}	
	
	if(ItemTranseInfo.isMonster)		//몬스터이면...
	{
		SItemTransQueueInfo *pItemTransQueueInfo;
		
		pItemTransQueueInfo = FindMonsterTransItemQueueInfo(ItemTranseInfo.interNpcId);
		if(pItemTransQueueInfo)			//현재 큐리스트가 존재하면
		{
			pItemTransQueueInfo->ItemTransQueue.PushData(ItemTranseInfo);
		}
		else							//존재하지 않으면
		{
			pItemTransQueueInfo = SAFE_NEW( SItemTransQueueInfo );			
			
			pItemTransQueueInfo->IsActive = false;
			pItemTransQueueInfo->nNPCId = ItemTranseInfo.interNpcId;
			pItemTransQueueInfo->ItemTransQueue.PushData(ItemTranseInfo);
			
			m_MonsterTItemQueueInfoList.push_back(pItemTransQueueInfo);
		}
	}
	else								//일반 NPC이면
	{
		m_NPCTItemQueueInfo.ItemTransQueue.PushData(ItemTranseInfo);
	}
}

SItemTransQueueInfo * CItemTransButton::FindMonsterTransItemQueueInfo(int nInterNpcId)
{
	if(m_MonsterTItemQueueInfoList.size())
	{
		list < SItemTransQueueInfo * >::iterator	pi = m_MonsterTItemQueueInfoList.begin();	
		SItemTransQueueInfo *pItemTransQueueInfo;
		for( ; pi != m_MonsterTItemQueueInfoList.end() ; ++pi )
		{   
		   pItemTransQueueInfo = (SItemTransQueueInfo *)(*pi);
		   if( nInterNpcId == pItemTransQueueInfo->nNPCId )
		   {
			   return pItemTransQueueInfo;
		   }
	   }	
	}	
	
	return NULL;
}

void CItemTransButton::SetActivateItemTransQueueInfo(int nInterNpcId)
{
	SItemTransQueueInfo * pItemTransQueueInfo = NULL;

	pItemTransQueueInfo = FindMonsterTransItemQueueInfo(nInterNpcId);

	if(pItemTransQueueInfo)
	{
		pItemTransQueueInfo->IsActive = true;
	}
}
