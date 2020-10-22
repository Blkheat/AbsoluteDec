#include "..\\..\\RockClient.h"
#include "..\\RockPCH.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\bravolibs\\Sound\\Sound.h"
#include "..\\..\\Pc.h"

#include "Rui.h"
#include "..\\..\\GTimer.h"

#include "define.h"
#include "UiUtil.h"
#include "Combo2DTimingEffect.h"
#include "InventoryWndProc.h"

#include "..\\CRenderManager.h"
#include "..\\..\\StringManager.h"

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

extern long g_nowTime;

CCombo2DTimingEffect::CCombo2DTimingEffect()
{
	m_lEtime = 0;

	m_nMotion = 0;
	
	m_IsActive = false;
	m_uComboState = STATE_COMBO_NONE;	
	
	m_IsRender = false;
	m_IsTempFlag = false;
	m_IsRenderGlow = false;	

	m_BarComKeyRect[COMZKEY] = SRect(0, 0, 33, 35);
	m_BarComKeyRect[COMXKEY] = SRect(0, 0, 33, 35);
	m_BarComKeyRect[COMCKEY] = SRect(0, 0, 33, 35);

	m_BarGaugeRect = SRect(2, 2, 123, 8);	
	m_BarSBgRect = SRect(-2, -2, 131, 16);
	m_BarBgRect = SRect(0, 0, 127, 12);
	
	m_Color = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_TexColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );

	m_fFlashTime = 0.25f;
	m_fCumulativeTime = 0.0f;
	

	m_fRatio = 1.0f;
	
	m_IsRenderComboKey = false;
	
	m_nComCount = 0;
	m_nRace = 0;
	m_lDestID = 0;

	m_finterTime = 0.1f;
	m_fiCumTime = 0.0f;

	m_fGlowRatio = 0.0f;

	
	m_IsComboKeyReg = false;
	m_cComboKey = 0;
	m_bEventFlag = false;
	m_bIsSuccess = false;

	m_nCurrentFrame = 0;

	m_lElasetime = 0;
	m_bIsStart = false;
	m_fComDelayTime = 0.0f;

	m_cRndKey = 0;

	m_cNextRndKey = 0;
	m_nNextRNum = 0;

	m_NextRComKeyRect = SRect(0, 0, 33, 35);
}

CCombo2DTimingEffect::~CCombo2DTimingEffect()
{

}

void CCombo2DTimingEffect::SetComboState(UINT uState)
{
	m_uComboState = uState;
	
	if(m_uComboState == STATE_COMBO_SUCCESS)
	{
		m_IsRenderGlow = true;
		m_fCumulativeTime = 0.0f;
		m_bEventFlag = true;
		m_bIsSuccess = true;
		m_TexColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );		
		m_fComDelayTime = 0.0f;

		m_IsActive = false;
	}

	if( m_uComboState == STATE_COMBO_FAIL )
	{
		if( m_IsActive )
		{
			m_IsRenderGlow = true;
			m_fCumulativeTime = 0.0f;		
			m_bIsSuccess = false;
			m_bEventFlag = true;
			
			m_TexColor = D3DCOLOR_ARGB( 255, 255, 150, 150 );		
			m_fComDelayTime = 0.0f;	
		}

		m_IsActive = false;
	}

	if( m_uComboState == STATE_COMBO_TIMEOVER )
	{
		if( m_nComCount && m_IsActive )
		{
			m_IsRenderGlow = true;
			m_fCumulativeTime = 0.0f;		
			m_bIsSuccess = false;
			m_bEventFlag = true;
			
			m_TexColor = D3DCOLOR_ARGB( 255, 255, 150, 150 );		
			m_fComDelayTime = 0.0f;	
		}

		m_IsActive = false;
	}
}

void CCombo2DTimingEffect::SetComCount(int nCount, int nRace, long lDestID)
{
	m_nComCount = nCount;

	m_nRace = nRace;
	m_lDestID = lDestID;

	if(m_nComCount == 0)
	{
		//�޺�Ű�� ��ϵǾ����� ������
		if(g_Pc.theSkillSystem.theShortCut_SkillCode[2] == 0)
		{
			m_IsComboKeyReg = false;
		}
		else
		{
			m_IsComboKeyReg = true;
		}
	}
}

UINT CCombo2DTimingEffect::GetComboState()
{
	return m_uComboState;
}

void CCombo2DTimingEffect::LogOff()
{
	m_IsRender = false;
}

void CCombo2DTimingEffect::InitEffect()
{
	m_IsRender = false;
	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y    = nRui->Desktop.Height / 2  +  100;
	
	m_BarBgRect.x = center_x - ( m_BarBgRect.w / 2 );
	m_BarBgRect.y = center_y - ( m_BarBgRect.h / 2 );
	
	m_BarGaugeRect.x += m_BarBgRect.x;
	m_BarGaugeRect.y += m_BarBgRect.y;

	m_BarSBgRect.x += m_BarBgRect.x;
	m_BarSBgRect.y += m_BarBgRect.y;

	m_BarComKeyRect[COMXKEY].x = center_x - ( m_BarComKeyRect[COMXKEY].w / 2 );
	m_BarComKeyRect[COMXKEY].y = m_BarBgRect.y + 15;

	m_BarComKeyRect[COMZKEY].x = m_BarComKeyRect[COMXKEY].x - m_BarComKeyRect[COMXKEY].w - 5;
	m_BarComKeyRect[COMZKEY].y = m_BarComKeyRect[COMXKEY].y;

	m_BarComKeyRect[COMCKEY].x = m_BarComKeyRect[COMXKEY].x + m_BarComKeyRect[COMXKEY].w + 5;
	m_BarComKeyRect[COMCKEY].y = m_BarComKeyRect[COMXKEY].y;

	m_BarComKeyCenPoint[COMXKEY] = SPoint2(m_BarComKeyRect[COMXKEY].x + (m_BarComKeyRect[COMXKEY].w / 2),
								   m_BarComKeyRect[COMXKEY].y + (m_BarComKeyRect[COMXKEY].h / 2));
	m_BarComKeyCenPoint[COMZKEY] = SPoint2(m_BarComKeyRect[COMZKEY].x + (m_BarComKeyRect[COMZKEY].w / 2),
								   m_BarComKeyRect[COMZKEY].y + (m_BarComKeyRect[COMZKEY].h / 2));
	m_BarComKeyCenPoint[COMCKEY] = SPoint2(m_BarComKeyRect[COMCKEY].x + (m_BarComKeyRect[COMCKEY].w / 2),
								   m_BarComKeyRect[COMCKEY].y + (m_BarComKeyRect[COMCKEY].h / 2));

	m_fRatio = 1.0f;
	m_fCumulativeTime = 0.0f;
	
	m_NextRComKeyRect.x = m_BarComKeyRect[COMCKEY].x + (m_BarComKeyRect[COMCKEY].w + 15);
	m_NextRComKeyRect.y = m_BarComKeyRect[COMCKEY].y;
	
	m_NextRComKeyCenPoint = SPoint2(m_NextRComKeyRect.x + (m_NextRComKeyRect.w / 2),
								    m_NextRComKeyRect.y + (m_NextRComKeyRect.h / 2) );	
}

void CCombo2DTimingEffect::StartEffect()
{	
	EndEffect();	
	
	m_IsRenderGlow = false;
	m_TexColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_uComboState = STATE_COMBO_NONE;	
	m_nMotion = g_Pc.GetPlayer()->m_ani_index;	
	m_IsActive = true;
	
	m_IsRender = true;		
	m_IsTempFlag = false;

	m_lElasetime = g_nowTime;
	m_bIsStart = true;
	m_bIsSuccess = false;	
	
	if(m_IsComboKeyReg)
	{
		if( m_nComCount < 4 && g_Pc.GetPlayer()->theActionSystem.theTotComboStep > 4 )
		{
			int nCurtComboType = GetCurtComboType();
			if(nCurtComboType == -1)
			{
				return;
			}
		
			int Num = g_Pc.theSkillSystem.theShortCut_SkillCode[2];
			int ComboKey = 
			g_Pc.GetPlayer()->theActionSystem.theCombo_Info[ nCurtComboType ].combo_sequence[ Num - 1 ][ m_nComCount ];

			if( ComboKey == 0 )
			{
				return;
			}
			else
			{
				switch( ComboKey / 10 )
				{
					case 1:
						m_cRndKey = 'Z';
						break;
					case 2:
						m_cRndKey = 'X';
						break;
					case 3:
						m_cRndKey = 'C';
						break;
				}
			}

			m_cNextRndKey = g_Pc.GetPlayer()->theActionSystem.ApllyAndGetRandomKey();
			m_nNextRNum =	g_Pc.GetPlayer()->theActionSystem.theNowRandomKey;
			g_Pc.GetPlayer()->theActionSystem.theNowRandomKey = ComboKey;
#ifdef ROCKCLIENT_DEV			
			PRINT_DLG_LOG("CurNowKey %c nCount < 4  NextRndKey %c NextRNum %d ComboKey %d " ,m_cComboKey , m_cNextRndKey , m_nNextRNum  , ComboKey );
#endif 
		}
		else
		{
			m_cRndKey = m_cNextRndKey;
			m_cNextRndKey = g_Pc.GetPlayer()->theActionSystem.ApllyAndGetRandomKey();
			int iNextRNum = g_Pc.GetPlayer()->theActionSystem.theNowRandomKey;

			g_Pc.GetPlayer()->theActionSystem.theNowRandomKey = m_nNextRNum;

			m_nNextRNum = iNextRNum;
#ifdef ROCKCLIENT_DEV			
			PRINT_DLG_LOG("CurNowKey %c ���� m_cRndKey  %c m_cNextRndKey %c m_nNextRNum %d  iNextRNum %d " 
				,m_cComboKey  , m_cRndKey , m_cNextRndKey  ,  m_nNextRNum  , iNextRNum  );
#endif 
		}

		char comKey	= GetNowComboKey();
		m_cComboKey = comKey;
	}
}

void CCombo2DTimingEffect::EndEffect()
{
	m_uComboState = STATE_COMBO_NONE;
	m_IsActive = false;
	m_IsRenderGlow = false;
	
	m_fRatio = 0.0f;
	m_nCurrentFrame = 0;
	m_lElasetime = g_nowTime;
	m_bIsStart = false;

	m_cRndKey = 0;
}

void CCombo2DTimingEffect::UpdateProc()
{
	m_IsRenderComboKey = false;
	
	if(m_bEventFlag)
	{
		char comKey	= GetNowComboKey();
		m_cComboKey = comKey;	
		
		SComEffect ComEff;
		ComEff.bSuccess = m_bIsSuccess;
		ComEff.comkey = m_cComboKey;
		ComEff.fCumtime = 0.0f;
		m_comEffList.push_back(ComEff);

		m_bEventFlag = false;
	}	
	
	if(m_IsActive)
	{
		if( m_uComboState == STATE_COMBO_NONE )
		{
			int curt_frame = g_Pc.GetPlayer()->m_ani_frame;
			int curt_motion = g_Pc.GetPlayer()->m_ani_index;
			
			int range_startframe = g_Pc_Manager.GetRangeStartFrame( g_Pc.GetPlayer(), curt_motion );
			int range_endframe = g_Pc_Manager.GetRangeEndFrame( g_Pc.GetPlayer(), curt_motion );
			int end_frame = g_Pc.GetPlayer()->m_end_frame;

			///--�޺� üũ �ص� �������� ���� �ִϸ��̼� �ص� �����Ӻ��� �� ���;;; 
			if( range_endframe + c_MotionBlendHalfFrame > end_frame )
			{
				range_endframe = end_frame - c_MotionBlendHalfFrame;
			}
			
			if(g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE) //ĳ���Ͱ� �׾�����
			{			
				g_Pc.GetPlayer()->theActionSystem.InitComboEnable();
				g_Pc.GetPlayer()->theCombo_State = n_Combo_Disable;
				EndEffect();
				return;
			}
			
			if( (range_startframe <= curt_frame) && ( curt_frame <= range_endframe ) )
			{
				m_IsRenderComboKey = true;
				
				char comKey	= GetNowComboKey();
				m_cComboKey = comKey;
			}
			else if( curt_frame > range_endframe )		///-- �ð��ʰ� - ���� ����������
			{
				g_Pc.GetPlayer()->theCombo_State = n_Combo_Disable;
				g_Pc.GetPlayer()->theActionSystem.InitComboEnable();
				SetComboState(STATE_COMBO_TIMEOVER);

				m_IsRenderComboKey = false;
				if( m_IsRenderGlow )
				{
				   m_IsRenderComboKey = true;			   	
				}
			}
			else ///-- ���������Ӻ��� ������
			{
				m_IsRenderComboKey = false;
			}

			m_nCurrentFrame = curt_frame;
		}
	}
	else
	{
		m_IsRenderComboKey = false;
		if( m_IsRenderGlow )
		{
		   m_IsRenderComboKey = true;			   	
		}
	}

	vector < list <SComEffect>::iterator > templist;
	
	if(m_comEffList.size())
	{
	   list	<SComEffect>::iterator	pi = m_comEffList.begin();
	   SComEffect *pComEff = NULL;
	   for( ; pi != m_comEffList.end() ; ++pi )
	   {   
		  pComEff = (SComEffect *)(&(*pi));
		  pComEff->fCumtime += g_GTimer.GetFrameElapsedSecTime();		  
		  if( pComEff->fCumtime > m_fFlashTime )
		  {
			  templist.push_back(pi);	
		  }
	   }	
	}

	for(int i = 0 ; i < templist.size() ; i++)
	{
		m_comEffList.erase(templist[i]);
	}	
	
	//�޺� ī��Ʈ�� 0�� �ƴҰ�� �޽��� Ÿ���� SM_ATTACK_NPC �Ǵ� SM_ATTACK_DELAY �ƴϸ�
	//�޺�Ű üũ�� ���� �ʰ� �ʱ�ȭ. - �̷�ƾ�� �߰��ȵǸ�...;; 
	//2�޺� ���� - ���� ������ �޺�Ű�� ������ ���а� �������������� �޺�ī��Ʈ�� ��� ������..;
	// --;;
	if( ( g_Pc.GetPlayer()->theActionSystem.GetComboCnt() ) &&
		( g_Pc.GetPlayer()->m_curt_event.type != SM_ATTACK_NPC ) && 
		( g_Pc.GetPlayer()->m_curt_event.type != SM_ATTACK_DELAY ) &&
		( g_Pc.GetPlayer()->m_curt_event.type != SM_UNDERATTACK ) )
		///-- ������ ���߻��� �޺�ī��Ʈ�� ������� �� �����ϱ� ���� ����������....;;
		///-- �״�� �����ϸ� ��� Ÿ���� �ٲ� �̵����� ���� ��� �޺��� �״�� �̾����� �ȴ�.
		///-- ���� ��� Ÿ���� �ٲ�� �޺� �ʱ�ȭ ��ƾ�� ����Ǿ� �Ʒ���ƾ�� �ǹ̰� ������;;;
		///-- && ( g_Pc.GetPlayer()->m_curt_event.type != NULL ) )  
	{
		g_Pc.GetPlayer()->theActionSystem.InitComboEnable();				
		g_Pc.GetPlayer()->theCombo_State = n_Combo_Disable;			
	}	

	BOOL CanInputKey = FALSE;
	
	if( g_Pc.GetPlayer()->m_curt_event.type == SM_ATTACK_NPC ||
		g_Pc.GetPlayer()->m_curt_event.type == SM_USE_SKILL )
	{			
		if( g_Pc.GetPlayer()->m_next_event.type != SM_USE_SKILL )
		{
			CanInputKey = TRUE;
		}
	}
	
	if(CanInputKey == FALSE)
	{
		m_IsRenderComboKey = false;
	}
}

void CCombo2DTimingEffect::RenderProc()
{		
	int nTempNum = 4;
	int PosX = 0;
	int PosY = 0;

	if( m_IsRenderGlow )
	{
		if( m_bIsSuccess )
		{
			float fSXpos = 0.0f;
			float fEXpos = 0.0f;		
			
			int nowNum = m_fCumulativeTime / ( m_fFlashTime / nTempNum );
			float fRemainder = m_fCumulativeTime - nowNum * ( m_fFlashTime / nTempNum );
			float fXRatio = fRemainder / ( m_fFlashTime / nTempNum );
			float fYRatio = m_fCumulativeTime/m_fFlashTime;

			if( (nowNum % 2) == 0 )
			{
				fSXpos = -5.0f;
				fEXpos = 10.0f;	
			}
			else
			{
				fSXpos = 10.0f;
				fEXpos = -5.0f;
			}		
		   
		   PosX = fSXpos + ( fXRatio * (fEXpos - fSXpos) );  
		}
		else
		{
		    float fSYpos = 0.0f;
			float fEYpos = 10.0f;
			
			float fYRatio = m_fCumulativeTime/m_fFlashTime;		
			PosY = fSYpos + ( fYRatio * (fEYpos - fSYpos) );  
		}
	}	

	{
		RenderComboKeyProc(PosX, PosY);
	}

	
	if( m_IsRenderGlow )
	{
	   m_fCumulativeTime += g_GTimer.GetFrameElapsedSecTime();	     
	   
	   if(m_fCumulativeTime > m_fFlashTime)
	   {
		  m_fCumulativeTime = 0.0f;		  
		  m_IsRenderGlow = false;
		  m_bIsSuccess = false;
		  
		  m_IsActive = false;
		  m_IsRenderGlow = false;
		
		  m_fRatio = 0.0f;
		  m_nCurrentFrame = 0;
	      m_lElasetime = g_nowTime;
		  m_bIsStart = false;		  
	   }
	}
}

void CCombo2DTimingEffect::RenderComboKeyProc(int PosX, int PosY)
{
	int nLevel = nRui->GetPcParamInfo()->Level;		

	//�޺�Ű�� ��ϵǾ����� ������..;;
	if( false == m_IsComboKeyReg )
	{
		//���� �Ͽ��ٸ�...;; - �޺�Ű�� ��ϵǾ������ʰ� �����ϸ� Ű�� �������� �ʴ´�.
		if( (nRui->thePcParam.MainClass != Wanderer) && 
			( nRui->thePcParam.MainClass != Stranger) )
		{
			return;
		}
	}

	UINT uComboBKeyImg = TID_None;
	UINT uComboKeyImg = TID_None;
	int nKeyIndex = -1;
	
	if( m_IsRenderComboKey )
	{
		if(m_cComboKey == 0)
		{
			return;
		}		
		
		switch(m_cComboKey)
		{
			case 'Z':
				{
					uComboBKeyImg = TID_Combo_SKeyZ;
					uComboKeyImg = TID_Combo_KeyZ;
					nKeyIndex = COMZKEY;
				}
				break;
			case 'X':
				{
					uComboBKeyImg = TID_Combo_SKeyX;
					uComboKeyImg = TID_Combo_KeyX;
					nKeyIndex = COMXKEY;
				}
				break;
			case 'C':
				{
					uComboBKeyImg = TID_Combo_SKeyC;
					uComboKeyImg = TID_Combo_KeyC;
					nKeyIndex = COMCKEY;
				}
				break;
		}

		if(uComboKeyImg == TID_None)
		{
			return;
		}		
		
		BOOL IsRender = TRUE;
		
		if( m_IsRenderGlow && !m_bIsSuccess )
		{
			IsRender = FALSE;
		}

		if( IsRender )
		{
			Render->Draw( TID_Combo_KeyBg, m_BarComKeyRect[nKeyIndex].x + PosX, m_BarComKeyRect[nKeyIndex].y + PosY, 
						  m_BarComKeyRect[nKeyIndex].w, m_BarComKeyRect[nKeyIndex].h, m_Color);
		}
	}
	
	DWORD SrcBlend, DestBlend;
	
	if(m_comEffList.size())
	{
	   g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
	   g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );		
	   
	   g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	   g_RenderManager.SetDestBlend( D3DBLEND_ONE );

	   float fSYpos = 0.0f;
	   float fEYpos = -20.0f;	
	   
	   list	<SComEffect>::iterator	pi = m_comEffList.begin();
	   SComEffect ComEff;
	   float fRatio = 0.0f;
	   int nPosY = 0;
	   
	   UINT tempComboBKeyImg = TID_None;
	   UINT tempComboKeyImg = TID_None;
	   int	tempKeyIndex = -1; 

	   for( ; pi != m_comEffList.end() ; ++pi )
	   {   
		   SRect sOutBgRect = SRect(0,0,0,0);
		   ComEff = (SComEffect)(*pi);
		   
		   m_fFlashTime = 0.25f;

		   if(!ComEff.bSuccess)
		   {
			   m_fFlashTime = 0.75f;
		   }	   
		   
		   fRatio = ComEff.fCumtime/m_fFlashTime;
		   nPosY = fSYpos + ( fRatio * (fEYpos - fSYpos) );

		   switch(ComEff.comkey)
		   {
				case 'Z':
					{
						tempComboBKeyImg = TID_Combo_SKeyZ;
						tempComboKeyImg = TID_Combo_KeyZ;
						tempKeyIndex = COMZKEY;
					}
					break;
				case 'X':
					{
						tempComboBKeyImg = TID_Combo_SKeyX;
						tempComboKeyImg = TID_Combo_KeyX;
						tempKeyIndex = COMXKEY;
					}
					break;
				case 'C':
					{
						tempComboBKeyImg = TID_Combo_SKeyC;	
						tempComboKeyImg = TID_Combo_KeyC;
						tempKeyIndex = COMCKEY;
					}
					break;
			}	   
		   
		   D3DCOLOR bColor;
		   float fARatio = 0.0f;
		   
		   if(ComEff.bSuccess)
		   {
			   sOutBgRect.w = m_BarComKeyRect[tempKeyIndex].w + (fRatio * 100);
			   sOutBgRect.h = m_BarComKeyRect[tempKeyIndex].h + (fRatio * 100);
	   	
				if((sOutBgRect.w % 2) != 0)
					sOutBgRect.w += 1;
				if((sOutBgRect.h % 2) != 0)
					sOutBgRect.h += 1;

			   sOutBgRect.x = m_BarComKeyCenPoint[tempKeyIndex].x - (sOutBgRect.w / 2);
			   sOutBgRect.y = m_BarComKeyCenPoint[tempKeyIndex].y - (sOutBgRect.h / 2);
		   
			   fARatio = (1 - fRatio) * 0.50f;
			   
			   bColor = D3DCOLOR_ARGB((int)(255 * fARatio), 255, 255, 255 );

			   Render->ScretchDraw( tempComboBKeyImg, sOutBgRect.x, sOutBgRect.y + nPosY, 
						 sOutBgRect.w, sOutBgRect.h, bColor);
		   }
		   else
		   {
				sOutBgRect.w = m_BarComKeyRect[tempKeyIndex].w;
				sOutBgRect.h = m_BarComKeyRect[tempKeyIndex].h;

				sOutBgRect.x = m_BarComKeyCenPoint[tempKeyIndex].x - (sOutBgRect.w / 2);
				sOutBgRect.y = m_BarComKeyCenPoint[tempKeyIndex].y - (sOutBgRect.h / 2);

				fARatio = (1 - fRatio) * 0.50f;

				float fSYpos = 0.0f;
				float fEYpos = 10.0f;			
				
				nPosY = fSYpos + ( fRatio * (fEYpos - fSYpos) );
				
				bColor = D3DCOLOR_ARGB((int)(255 * fARatio), 255, 100, 100 );

				g_RenderManager.SetSrcBlend( SrcBlend );
				g_RenderManager.SetDestBlend( DestBlend );

				Render->ScretchDraw( tempComboBKeyImg, sOutBgRect.x, sOutBgRect.y + nPosY, 
						 sOutBgRect.w, sOutBgRect.h, bColor);
				
				Render->ScretchDraw( tempComboKeyImg, sOutBgRect.x, sOutBgRect.y + nPosY, 
						 sOutBgRect.w, sOutBgRect.h, bColor);
		   }	   
		   
	   }	   
	   
	   g_RenderManager.SetSrcBlend( SrcBlend );
	   g_RenderManager.SetDestBlend( DestBlend );
	}
	
	if( m_IsRenderComboKey )
	{
		if(m_IsRenderGlow && m_bIsSuccess )
		{
			g_RenderManager.GetRenderState( D3DRS_SRCBLEND, &SrcBlend );
			g_RenderManager.GetRenderState( D3DRS_DESTBLEND, &DestBlend );		
	   
			g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
			g_RenderManager.SetDestBlend( D3DBLEND_ONE );
		}		

		BOOL IsRender = TRUE;
		
		if( m_IsRenderGlow && !m_bIsSuccess )
		{
			IsRender = FALSE;
		}

		if( IsRender )
		{
			Render->Draw( uComboBKeyImg, m_BarComKeyRect[nKeyIndex].x + PosX, m_BarComKeyRect[nKeyIndex].y + PosY, 
						  m_BarComKeyRect[nKeyIndex].w, m_BarComKeyRect[nKeyIndex].h, m_TexColor );	
			Render->Draw( uComboKeyImg, m_BarComKeyRect[nKeyIndex].x + PosX, m_BarComKeyRect[nKeyIndex].y + PosY, 
						  m_BarComKeyRect[nKeyIndex].w, m_BarComKeyRect[nKeyIndex].h, m_TexColor );	
		}

		if( m_IsRenderGlow && m_bIsSuccess )
		{
			g_RenderManager.SetSrcBlend( SrcBlend );
			g_RenderManager.SetDestBlend( DestBlend );
		}
	}

	if( m_IsRenderComboKey )
	{
		if( ( (m_nComCount >= 3) && ( m_nComCount != g_Pc.GetPlayer()->theActionSystem.theTotComboStep-1 ) ) &&
			( g_Pc.GetPlayer()->theActionSystem.theTotComboStep > 4 ) )
		{
			if(m_cNextRndKey == 0)
			{
				return;
			}		
			
			switch(m_cNextRndKey)
			{
				case 'Z':
					{
						uComboBKeyImg = TID_Combo_SKeyZ;
						uComboKeyImg = TID_Combo_KeyZ;
						nKeyIndex = COMZKEY;
					}
					break;
				case 'X':
					{
						uComboBKeyImg = TID_Combo_SKeyX;
						uComboKeyImg = TID_Combo_KeyX;
						nKeyIndex = COMXKEY;
					}
					break;
				case 'C':
					{
						uComboBKeyImg = TID_Combo_SKeyC;
						uComboKeyImg = TID_Combo_KeyC;
						nKeyIndex = COMCKEY;
					}
					break;
			}

			if(uComboKeyImg == TID_None)
			{
				return;
			}
			
			//m_NextRComKeyRect.x = m_BarComKeyRect[nKeyIndex].x; 
			//m_NextRComKeyRect.y = m_BarComKeyRect[nKeyIndex].y - (m_BarComKeyRect[nKeyIndex].h + 10);
			
			Render->Draw( TID_Combo_KeyBg, m_NextRComKeyRect.x, m_NextRComKeyRect.y, 
						  m_NextRComKeyRect.w, m_NextRComKeyRect.h, D3DCOLOR_ARGB(255, 255, 150, 150 ));

			Render->Draw( uComboBKeyImg, m_NextRComKeyRect.x, m_NextRComKeyRect.y, 
				  m_NextRComKeyRect.w, m_NextRComKeyRect.h, D3DCOLOR_ARGB(255, 255, 150, 150 ));	
			Render->Draw( uComboKeyImg, m_NextRComKeyRect.x, m_NextRComKeyRect.y, 
				  m_NextRComKeyRect.w, m_NextRComKeyRect.h, D3DCOLOR_ARGB(255, 255, 150, 150 ));

		}
	}
}

int CCombo2DTimingEffect::GetCurtComboType()
{
	int RHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1];

	if( RHand_index != -1 && LHand_index == -1 )				// ������ ���⸸ ����
	{		
		if( g_Pc_Manager.m_pItm[RHand_index] )
		{
			if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// ��� 
			{
				return nComboType_TwoHand_Sword; 
			}
			else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // �Ѽհ� 
			{
				return nComboType_OneHand_Sword; 
			}
			else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // ������ 
			{
				return nComboType_Staff; 
			}
			else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // �����
			{
				return nComboType_CrystalBall;	
			}
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// �޼� ���⸸ ����
	{
		if( g_Pc_Manager.m_pItm[LHand_index] )
		{
			if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// Ȱ 
			{
				return -1;			
			}
			else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // ���� 
			{
				return nComboType_Sword_Shield;		
			}
			else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // ������ 
			{
				return nComboType_MagicBook; 
			}
		}
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// ��տ� ���� ����
	{
		if( g_Pc_Manager.m_pItm[RHand_index] && g_Pc_Manager.m_pItm[LHand_index] )
		{
			if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
				g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// �Ѽհ� + ����
			{
				return nComboType_Sword_Shield; 
			}
		}
	}
	
	return -1;
}

int CCombo2DTimingEffect::GetCurtComboTypeSecond()
{
	int RHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1];

	if( nRui->thePcParam.MainClass == Swordman )				// ������ ���⸸ ����
	{		
		// ��� ���� (����) ---------------------------------------------------------------------------------- 
		if( InventoryWnd.theSkillCode_Sword >= c_SkillCode_OndHandSword_Min && 
			InventoryWnd.theSkillCode_Sword <= c_SkillCode_OndHandSword_Max )						// �� 1 ~ 10 (�� ���)
		{
			return nComboType_OneHand_Sword;
		}
		else if( InventoryWnd.theSkillCode_TwohandSword >= c_SkillCode_TwoHandSword_Min && 
				 InventoryWnd.theSkillCode_TwohandSword <= c_SkillCode_TwoHandSword_Max )			// ��� 1 ~ 10 (��� ���)
		{
			return nComboType_TwoHand_Sword;
		}		
	}
	else if( nRui->thePcParam.MainClass == Archer || 
			nRui->thePcParam.MainClass == Sniper )			// �޼� ���⸸ ����
	{
		return -1;	
	}
	else if( nRui->thePcParam.MainClass == Magician || 
			 nRui->thePcParam.MainClass == Prominas )
	{
		return nComboType_MagicBook;
	}
	else if( nRui->thePcParam.MainClass == Shaman ||
			 nRui->thePcParam.MainClass == Psyche )
	{
		return nComboType_CrystalBall;
	}
	else if( nRui->thePcParam.MainClass == Knight )
	{
		return nComboType_Sword_Shield;
	}
	else if( nRui->thePcParam.MainClass == Mercenary )
	{
		return nComboType_OneHand_Sword;
	}
	else if( nRui->thePcParam.MainClass ==  Slayer)
	{
		return nComboType_TwoHand_Sword;
	}
	else if( nRui->thePcParam.MainClass ==  Priest ||
		     nRui->thePcParam.MainClass == Holy_Avenger )
	{
		return nComboType_Staff;
	}
	
	return -1;
}

void CCombo2DTimingEffect::GetNeedSkill( TCHAR * strTemp )
{
	int RHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = g_Pc.GetPlayer()->m_pUserInfo->ArmorInfo.index[1];

	if( nRui->thePcParam.MainClass == Swordman )				// ������ ���⸸ ����
	{		
		// ��� ���� (����) ---------------------------------------------------------------------------------- 
		if( InventoryWnd.theSkillCode_Sword >= c_SkillCode_OndHandSword_Min && 
			InventoryWnd.theSkillCode_Sword <= c_SkillCode_OndHandSword_Max )						// �� 1 ~ 10 (�� ���)
		{
			Rsprintf(RWCHAR(strTemp), G_STRING_CHAR( IDS_ONE_SWORD ) );		
		}
		else if( InventoryWnd.theSkillCode_TwohandSword >= c_SkillCode_TwoHandSword_Min && 
				 InventoryWnd.theSkillCode_TwohandSword <= c_SkillCode_TwoHandSword_Max )			// ��� 1 ~ 10 (��� ���)
		{
			Rsprintf(RWCHAR(strTemp), G_STRING_CHAR( IDS_BIG_SWORD ) );		
		}		
	}
	else if( nRui->thePcParam.MainClass == Shaman ||
			 nRui->thePcParam.MainClass == Psyche )
	{
		Rsprintf(RWCHAR(strTemp), G_STRING_CHAR( IDS_SYNCHRONIZATION ) );	
	}
	else if( nRui->thePcParam.MainClass == Knight )
	{
		Rsprintf(RWCHAR(strTemp), G_STRING_CHAR( IDS_ONE_SWORD ) );		
	}
	else if( nRui->thePcParam.MainClass == Mercenary )
	{
		Rsprintf(RWCHAR(strTemp), G_STRING_CHAR( IDS_ONE_SWORD ) );		
	}
	else if( nRui->thePcParam.MainClass ==  Slayer)
	{
		Rsprintf(RWCHAR(strTemp), G_STRING_CHAR( IDS_BIG_SWORD ) );		
	}
}

int CCombo2DTimingEffect::GetComboKey(int nCurtComboType )
{
	int ComboKey = g_Pc.GetPlayer()->theActionSystem.theCombo_Info[ nCurtComboType ].combo_sequence[ 0 ][ m_nComCount ];				
				
	if( ComboKey == 0 )
	{
		return 0;
	}
	else
	{
		switch( ComboKey / 10 )
		{
			case 1:
				return 'Z';
				break;
			case 2:
				return 'X';
				break;
			case 3:
				return 'C';
				break;
		}
	}

	return 0;
}

char CCombo2DTimingEffect::GetNowComboKey()
{
	//���� �Ͽ��ٸ� ��ϵ� Ű���� ������
	if( (nRui->thePcParam.MainClass != Wanderer) && 
		( nRui->thePcParam.MainClass != Stranger) )
	{
		if( g_Pc.GetPlayer()->theActionSystem.theTotComboStep > 4 )
		{
			char comKey = m_cRndKey;
			return comKey;
		}
		else
		{
			return g_Pc.theSkillSystem.theStrHotKey[m_nComCount];
		}
	}
	else
	{
		int nCurtComboType = GetCurtComboType();
		if(nCurtComboType == -1)
		{
			return 0;
		}
		
		return GetComboKey( nCurtComboType );
	}
}