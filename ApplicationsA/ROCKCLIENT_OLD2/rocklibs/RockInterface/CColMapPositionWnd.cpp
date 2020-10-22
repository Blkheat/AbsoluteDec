

#include "Rui.h"
#include "CColMapPositionWnd.h"
#include "MiniMapWndProc.h"
#include "MessageWndProc.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\MapDataManager.h"
#include "..\\..\\StringManager.h"
#include "..\\..\\bravolibs\\obj\\player_manager.h"

CColMapPositionWnd  * g_ColMapPositionWnd = NULL;

CColMapPositionWnd::CColMapPositionWnd()
{
	m_WIDToolTipDestWnd = 0;
	SAFE_STR_CPY( m_strToolTipItemName, _RT(""), 256 );	

	m_pMapViewWnd = NULL;
	memset(m_pBatchCButton, 0, sizeof(CCheckButton *) * 3);
	memset(m_pArrowLineList, 0, sizeof(CArrowLineList *) * 3);
	
	memset(m_pBachNPCInfo, 0, sizeof(SNPCBaseInfo *) * 3);
	memset(&m_LinkColItem, 0, sizeof(SPcItem));

	m_nSelectPosition = -1;
	m_ColCallType = 0;	

	m_pOutFrameWnd = NULL;
	m_pCloseBtn = NULL;
	m_pOkBtn = NULL;
	m_pCancleBtn = NULL;
	m_pMapName = NULL;

	m_NowMapTexId = TID_None;
	m_nNowMapId = 0;

	m_tilesize = 16;
	
	m_IsButton_lock = FALSE;
}

CColMapPositionWnd::~CColMapPositionWnd()
{

}

void CColMapPositionWnd::Initialize()
{
	m_WIDToolTipDestWnd = 0;
	SAFE_STR_CPY( m_strToolTipItemName, _RT(""), 256 );
	memset(m_pBachNPCInfo, 0, sizeof(SNPCBaseInfo *) * 3);
	memset(&m_LinkColItem, 0, sizeof(SPcItem));

	m_nSelectPosition = -1;
	m_ColCallType = 0;

	m_NowMapTexId = TID_None;
	m_nNowMapId = 0;	

	m_pBatchCButton[0]->SetVisible(false);
	m_pBatchCButton[1]->SetVisible(false);
	m_pBatchCButton[2]->SetVisible(false);

	m_pArrowLineList[0]->SetVisible(false);
	m_pArrowLineList[1]->SetVisible(false);
	m_pArrowLineList[2]->SetVisible(false);	
	
	m_pArrowLineList[0]->Initialize();
	m_pArrowLineList[1]->Initialize();
	m_pArrowLineList[2]->Initialize();

	m_pBatchCButton[0]->SetBtnHold( false );
	m_pBatchCButton[1]->SetBtnHold( false );
	m_pBatchCButton[2]->SetBtnHold( false );

	nRui->SendEvent( m_pBatchCButton[0]->GetID(), n_emSetUnChecked , 0, 0, 0, 0 );
	nRui->SendEvent( m_pBatchCButton[1]->GetID(), n_emSetUnChecked , 0, 0, 0, 0 );
	nRui->SendEvent( m_pBatchCButton[2]->GetID(), n_emSetUnChecked , 0, 0, 0, 0 );
}

void CColMapPositionWnd::Composition()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

	SRect sCRLRect;
	CImageBox *	 img = NULL;
	CImageBox9*  img9 = NULL;
	CTextBox  *  tbox = NULL;

	sCRLRect.w	= 532;
	sCRLRect.h	= 622;
	sCRLRect.x	= center_x - ( sCRLRect.w / 2 );
	sCRLRect.y	= center_y - ( sCRLRect.h / 2 );
	
	///-- Frame Wnd
	m_pOutFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, 
									sCRLRect.x, sCRLRect.y, sCRLRect.w, sCRLRect.h, false );
	m_pOutFrameWnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pOutFrameWnd->SetClientImage( TID_CTRL_WND_L1 );
	m_pOutFrameWnd->SetWndTileSizeLR( 40, 29 );
	m_pOutFrameWnd->SetWndTileSizeTB( 31, 11 );

	m_pOutFrameWnd->SetCaption( "공성아이템 배치창" );	 //HAN_MSG
	m_pOutFrameWnd->SetTitle( true );
	m_pOutFrameWnd->SetWndProc( this );
	m_pOutFrameWnd->SetMove( false );
	m_pOutFrameWnd->SetVisible( false );
	m_pOutFrameWnd->Initialize();

	m_pCloseBtn = m_pCloseBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pOutFrameWnd->GetID(), 
											 502, 6, 13, 13, false );   
	m_pCloseBtn->SetBtnImage( TID_CTRL_BTN_X, TID_CTRL_BTN_X_OVR, TID_CTRL_BTN_X_CLK, TID_None );

	m_pMapName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pOutFrameWnd->GetID(), 20, 40, 110, 13, false );
	m_pMapName->SetFontR( n_fRsvFontWndStatic );
	m_pMapName->SetTextColor( 255, 0, 0 );	
	m_pMapName->SetAlignText( n_atLeft, n_atCenter );
	m_pMapName->SetText( _RT("") );	
	
	m_pOkBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pOutFrameWnd->GetID() , 168, 582, 58, 25, false );        
	m_pOkBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pOkBtn->SetFontR( n_fRsvFontWndButton );   
	m_pOkBtn->SetText( _RT("확인") );				// HAN_MSG  
	

	m_pCancleBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None , m_pOutFrameWnd->GetID() , 311, 582, 58, 25, false );        
	m_pCancleBtn->SetBtnImage( TID_CTRL_BTN58 , TID_CTRL_BTN58_OVR , TID_CTRL_BTN58_CLK , TID_CTRL_BTN58_DIS );
	m_pCancleBtn->SetFontR( n_fRsvFontWndButton );   
	m_pCancleBtn->SetText( _RT("취소") );			// HAN_MSG

	m_pMapViewWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, m_pOutFrameWnd->GetID(), 
									10, 58, 512, 512, false );
	m_pMapViewWnd->SetWndTexStyle( fwfTex_Style_Normal );
	m_pMapViewWnd->SetClientImage( TID_None );
	m_pMapViewWnd->SetWndProc( this );	
	m_pMapViewWnd->Initialize();

	m_pBatchCButton[0] = (CCheckButton *)nRui->CreateWnd( n_wtCheckButton, WID_None, m_pMapViewWnd->GetID(), 
		0, 0, 10, 10, false );  
	m_pBatchCButton[0]->SetClientImage( TID_CTRL_RAD_DIS );
	m_pBatchCButton[0]->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_CTRL_RAD_CHK2);
	m_pBatchCButton[0]->SetDirectCheck(false);
	m_pBatchCButton[0]->SetVisible(false);

	m_pBatchCButton[1] = (CCheckButton *)nRui->CreateWnd( n_wtCheckButton, WID_None, m_pMapViewWnd->GetID(), 
		0, 0, 10, 10, false );  
	m_pBatchCButton[1]->SetClientImage( TID_CTRL_RAD_DIS );
	m_pBatchCButton[1]->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_CTRL_RAD_CHK2);
	m_pBatchCButton[1]->SetDirectCheck(false);
	m_pBatchCButton[1]->SetVisible(false);

	m_pBatchCButton[2] = (CCheckButton *)nRui->CreateWnd( n_wtCheckButton, WID_None, m_pMapViewWnd->GetID(), 
		0, 0, 10, 10, false );  
	m_pBatchCButton[2]->SetClientImage( TID_CTRL_RAD_DIS );
	m_pBatchCButton[2]->SetBtnImage(TID_None, TID_None, TID_CTRL_RAD_CHK2, TID_CTRL_RAD_CHK2);
	m_pBatchCButton[2]->SetDirectCheck(false);
	m_pBatchCButton[2]->SetVisible(false);

	m_pArrowLineList[0] = (CArrowLineList *)nRui->CreateWnd( n_wtArrowLineList, WID_None, m_pMapViewWnd->GetID(), 
		0, 0, 0, 0, false );  
	m_pArrowLineList[0]->SetClientImage( TID_None );
	m_pArrowLineList[0]->SetFocusCheck(false);
	m_pArrowLineList[0]->SetVisible(false);

	m_pArrowLineList[1] = (CArrowLineList *)nRui->CreateWnd( n_wtArrowLineList, WID_None, m_pMapViewWnd->GetID(), 
		0, 0, 0, 0, false );  
	m_pArrowLineList[1]->SetClientImage( TID_None );
	m_pArrowLineList[1]->SetFocusCheck(false);
	m_pArrowLineList[1]->SetVisible(false);

	m_pArrowLineList[2] = (CArrowLineList *)nRui->CreateWnd( n_wtArrowLineList, WID_None, m_pMapViewWnd->GetID(), 
		0, 0, 0, 0, false );  
	m_pArrowLineList[2]->SetClientImage( TID_None );
	m_pArrowLineList[2]->SetFocusCheck(false);
	m_pArrowLineList[2]->SetVisible(false);

	Initialize();
}

void CColMapPositionWnd::Update()
{
	//툴팁 이름표시 - 뷰 체크
	if(m_WIDToolTipDestWnd != nRui->GetFocusWnd())
	{
		SetFocusItemBtn(WID_None, _RT(""));
	}
}

void CColMapPositionWnd::Proc( SEventMessage* EMsg )
{
	int i = 0;

	if( m_ColCallType == n_CoAttackArams ) ///-- 공성병기일 시에
	{
		for( i = 0 ; i < 3 ; i++ )
		{
			m_pArrowLineList[i]->SetVisible(false);
		}
	}	
	
	for( i = 0 ; i < 3 ; i++ )
	{
		if(m_pBatchCButton[i]->GetID() == EMsg->FocusWnd)
		{			
			if( m_ColCallType == n_CoAttackArams ) ///-- 공성병기일 시에
			{
				m_pArrowLineList[i]->SetVisible( true );
			}

			if(m_pBachNPCInfo[i])
			{
				SetFocusItemBtn( m_pBatchCButton[i]->GetID(), (TCHAR *)(m_pBachNPCInfo[i]->theName) );
			}
			
			if( EMsg->Notify == NM_BTN_CLICK )
			{
				nRui->SendEvent( m_pBatchCButton[0]->GetID(), n_emSetUnChecked , 0, 0, 0, 0 );
				nRui->SendEvent( m_pBatchCButton[1]->GetID(), n_emSetUnChecked , 0, 0, 0, 0 );
				nRui->SendEvent( m_pBatchCButton[2]->GetID(), n_emSetUnChecked , 0, 0, 0, 0 );
				
				nRui->SendEvent( m_pBatchCButton[i]->GetID(), n_emSetChecked , 0, 0, 0, 0 );
				m_nSelectPosition = i;
			}					
		}
	}

	if( EMsg->FocusWnd == m_pCloseBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();		
		}	
	}
	else if( EMsg->FocusWnd == m_pCancleBtn->GetID() )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			Close();			
		}	
	}
	else if( EMsg->FocusWnd == m_pOkBtn->GetID() && !m_IsButton_lock )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			if( m_ColCallType == n_CoAttackArams || m_ColCallType == n_CoGuardTower )
			{
				if( m_nSelectPosition == -1 )
				{
					MessageBoxWnd.Open( "배치 위치가 선택되어 있지 않습니다." , 1000, NULL );		///-- HAN_MSG
					return;
				}
		
				g_RockClient.Send_RpCsUse( m_LinkColItem.ItemUID, (BYTE)m_nSelectPosition );
			}
			else
			{
				g_RockClient.Send_RpCsUse( m_LinkColItem.ItemUID );
			}

			m_IsButton_lock = TRUE;
			Close();
		}	
	}
}

void CColMapPositionWnd::SetColItem(SPcItem * pItem, UINT nColCallType)
{
	if(pItem == NULL)
	{
		Close();
		return;
	}

	if(pItem->ItemTable == NULL)
	{
		Close();
		return;
	}

	if(nColCallType == 0)
	{
		Close();
		return;
	}
	
	m_LinkColItem = (*pItem);
	m_ColCallType = nColCallType;

	m_NowMapTexId = MiniMapWnd.Map->GetMapClientImage();
	m_pMapViewWnd->SetClientImage( m_NowMapTexId );

	m_nNowMapId = g_Map.m_NowMapID;
	m_pMapName->SetText( G_STRING_CHAR( G_MAP_STRID(m_nNowMapId) ) );

	BatchColItemType();	
	///-- 이곳에서 실제로 각 공성아이템에 따라 윈도우들을 속성설정 및 배치한다
}

void CColMapPositionWnd::Open()
{
	Initialize();
	nRui->SendEvent( m_pOutFrameWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	nRui->SendEvent( m_pMapViewWnd->GetID(), n_emSetVisible, (EPARAM)true, 0, 0, 0 );

	m_IsButton_lock = FALSE;
}

void CColMapPositionWnd::Close()
{
	Initialize();
	nRui->SendEvent( m_pOutFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( m_pMapViewWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

void CColMapPositionWnd::LogOff()
{
	Initialize();
	nRui->SendEvent( m_pOutFrameWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( m_pMapViewWnd->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
}

void CColMapPositionWnd::BatchColItemType()
{
	DWORD dBaseCallNpcCode = (DWORD)m_LinkColItem.ItemTable->theReSP;	
	
	switch( m_ColCallType )
	{
		case n_CoPosition:			///-- 진지
			{
				m_pBachNPCInfo[0] = g_Pc_Manager.GetNPCBaseInfoPtr( dBaseCallNpcCode );
				BatchCoPositionGui();
			}
			break;
		case n_CoAttackArams:	    ///-- 공성병기	
			{
				///-- 배치와 이동타입에 따른 분류
				m_pBachNPCInfo[0] = g_Pc_Manager.GetNPCBaseInfoPtr( dBaseCallNpcCode );
				m_pBachNPCInfo[1] = g_Pc_Manager.GetNPCBaseInfoPtr( dBaseCallNpcCode + 1 );
				m_pBachNPCInfo[2] = g_Pc_Manager.GetNPCBaseInfoPtr( dBaseCallNpcCode + 2 );
				BatchCoAttackAramsGui();
			}
			break;
		case n_CoBuilding:			///-- 성벽/성문/코어
			{
				m_pBachNPCInfo[0] = g_Pc_Manager.GetNPCBaseInfoPtr( dBaseCallNpcCode );
				m_pBachNPCInfo[1] = g_Pc_Manager.GetNPCBaseInfoPtr( dBaseCallNpcCode + 1 );
				m_pBachNPCInfo[2] = g_Pc_Manager.GetNPCBaseInfoPtr( dBaseCallNpcCode + 2 );
				BatchCoBuildingGui();
			}
			break;
		case n_CoGuardTower:		///-- 가드타워
			{
				///-- 배치포인트에 대한 분류
				m_pBachNPCInfo[0] = g_Pc_Manager.GetNPCBaseInfoPtr( dBaseCallNpcCode );
				m_pBachNPCInfo[1] = g_Pc_Manager.GetNPCBaseInfoPtr( dBaseCallNpcCode + 1 );
				m_pBachNPCInfo[2] = g_Pc_Manager.GetNPCBaseInfoPtr( dBaseCallNpcCode + 2 );
				BatchCoGuardTowerGui();
			}
			break;
	}
}

void CColMapPositionWnd::BatchCoPositionGui()
{
	SNPCMoveInfo * pNPCMoveInfo = NULL;
	///-- 진지 포지션은 한군데로 정해져있다.
	if( m_pBachNPCInfo[0] )
	{
		WORD nNPCCode = m_pBachNPCInfo[0]->theCode;
		pNPCMoveInfo = g_Pc_Manager.m_SourceDataManager.FindNpcMoveInfoPtr( nNPCCode, (WORD)m_nNowMapId);
		if(pNPCMoveInfo)
		{
			///-- 설치 좌표값을 가져온다.
			DWORD dCell = pNPCMoveInfo->theCellInfo[0].theCell;
			POINT sPoint;
			SRect ComRect;
			///-- 셀좌료를 미니맵상 2D좌표로 변환시킨다.
			ConvertCellTo2DPoint(dCell, sPoint);
			m_pBatchCButton[0]->GetClientSize(&ComRect);
			m_pBatchCButton[0]->SetWndRepositionR((sPoint.x - ComRect.w / 2), (sPoint.y - ComRect.h / 2));			
			m_pBatchCButton[0]->SetBtnHold( true );
			m_pBatchCButton[0]->SetVisible(true);
		}
	}
}

void CColMapPositionWnd::BatchCoAttackAramsGui()
{
	SNPCMoveInfo * pNPCMoveInfo = NULL;
	///-- 공성 병기는 3가지 시작위치와 이동경로를 가지고있다.
	
	for( int i = 0 ; i < 3 ; i++ )
	{
		if( m_pBachNPCInfo[i] )
		{
			WORD nNPCCode = m_pBachNPCInfo[i]->theCode;
			pNPCMoveInfo = g_Pc_Manager.m_SourceDataManager.FindNpcMoveInfoPtr( nNPCCode, (WORD)m_nNowMapId);
			if(pNPCMoveInfo)
			{
				///-- 설치 좌표값을 가져온다. - 여기선 시작좌표
				DWORD dCell = pNPCMoveInfo->theCellInfo[0].theCell;
				POINT sPoint;
				SRect ComRect;			

				///-- 셀좌료를 미니맵상 2D좌표로 변환시킨다.
				ConvertCellTo2DPoint(dCell, sPoint);
				m_pBatchCButton[i]->GetClientSize(&ComRect);
				m_pBatchCButton[i]->SetWndRepositionR((sPoint.x - ComRect.w / 2), (sPoint.y - ComRect.h / 2));
				nRui->SendEvent( m_pBatchCButton[i]->GetID(), n_emSetUnChecked , 0, 0, 0, 0 );
				m_pBatchCButton[i]->SetBtnHold( false );
				m_pBatchCButton[i]->SetVisible( true );

				SRect MapViewRect;		
				m_pMapViewWnd->GetClientSize(&MapViewRect);

				///-- 이동경로를 채운다
				for(int j = 0; j < 19 ; j++)
				{
					if( pNPCMoveInfo->theCellInfo[j].theCell == 0 )
					{
						break;
					}

					DWORD dCell = pNPCMoveInfo->theCellInfo[j].theCell;
					POINT sPoint;
					ConvertCellTo2DPoint(dCell, sPoint);
					
					sPoint.x += MapViewRect.x;
					sPoint.y += MapViewRect.y;	
					m_pArrowLineList[i]->AddPoint(sPoint);	
				}

				m_pArrowLineList[i]->MakePolygonList();
			}
		}
	}
}

void CColMapPositionWnd::BatchCoBuildingGui()
{
	SNPCMoveInfo * pNPCMoveInfo = NULL;
	///-- 성문/성벽 은 3가지 셋트 - 성문, 성벽, 코어로 되어있다.
	for( int i = 0 ; i < 3 ; i++ )
	{
		if( m_pBachNPCInfo[i] )
		{
			WORD nNPCCode = m_pBachNPCInfo[i]->theCode;
			pNPCMoveInfo = g_Pc_Manager.m_SourceDataManager.FindNpcMoveInfoPtr( nNPCCode, (WORD)m_nNowMapId);
			if(pNPCMoveInfo)
			{
				///-- 설치 좌표값을 가져온다. - 여기선 시작좌표
				DWORD dCell = pNPCMoveInfo->theCellInfo[0].theCell;
				POINT sPoint;
				SRect ComRect;			

				///-- 셀좌료를 미니맵상 2D좌표로 변환시킨다.
				ConvertCellTo2DPoint(dCell, sPoint);
				m_pBatchCButton[i]->GetClientSize(&ComRect);
				m_pBatchCButton[i]->SetWndRepositionR((sPoint.x - ComRect.w / 2), (sPoint.y - ComRect.h / 2));
				m_pBatchCButton[i]->SetBtnHold( true );
				m_pBatchCButton[i]->SetVisible(true);		
			}
		}
	}
}

void CColMapPositionWnd::BatchCoGuardTowerGui()
{
	SNPCMoveInfo * pNPCMoveInfo = NULL;
	///-- 가드타워는 이동경로가 없는 3가지 위치로 잡혀있다.
	for( int i = 0 ; i < 3 ; i++ )
	{
		if( m_pBachNPCInfo[i] )
		{
			WORD nNPCCode = m_pBachNPCInfo[i]->theCode;
			pNPCMoveInfo = g_Pc_Manager.m_SourceDataManager.FindNpcMoveInfoPtr( nNPCCode, (WORD)m_nNowMapId);
			if(pNPCMoveInfo)
			{
				///-- 설치 좌표값을 가져온다. - 여기선 시작좌표
				DWORD dCell = pNPCMoveInfo->theCellInfo[0].theCell;
				POINT sPoint;
				SRect ComRect;			

				///-- 셀좌료를 미니맵상 2D좌표로 변환시킨다.
				ConvertCellTo2DPoint(dCell, sPoint);
				m_pBatchCButton[i]->GetClientSize(&ComRect);
				m_pBatchCButton[i]->SetWndRepositionR((sPoint.x - ComRect.w / 2), (sPoint.y - ComRect.h / 2));
				m_pBatchCButton[i]->SetBtnHold( false );
				m_pBatchCButton[i]->SetVisible( true );		
			}
		}
	}
}

void CColMapPositionWnd::ConvertCellTo2DPoint(DWORD dCell, POINT & sPoint)
{
	POINT  Tex_Pos;
	POINT  Grid_Pos;
	
	int		nPosX, nPosY;

	int iRealMapSize = MiniMapWnd.Map->GetRealMapSize();
	int iTexMapSize = MiniMapWnd.Map->GetTexMapSize();

	nPosX = dCell % g_Pc.m_cSize;
	nPosY = dCell / g_Pc.m_cSize;
	
	float xfPos = ( nPosX * CLIENT_TILESIZE )+( CLIENT_TILESIZE / 2 );
	float zfPos = ( nPosY * CLIENT_TILESIZE )+( CLIENT_TILESIZE / 2 );

	Grid_Pos.x = ( xfPos / m_tilesize );
	Grid_Pos.y = ( zfPos / m_tilesize );
	Grid_Pos.y = iRealMapSize - Grid_Pos.y;		///-- 좌표 뒤집기

	Tex_Pos.x = ( iTexMapSize * Grid_Pos.x ) / iRealMapSize;
	Tex_Pos.y = ( iTexMapSize * Grid_Pos.y ) / iRealMapSize;
	
	sPoint = Tex_Pos;
}

void CColMapPositionWnd::SetFocusItemBtn(UINT WIDToolTipDestWnd, void *strToolTipItemName)
{
	if( m_WIDToolTipDestWnd != WIDToolTipDestWnd )  
	{
		m_WIDToolTipDestWnd = WIDToolTipDestWnd;
		SAFE_STR_CPY(m_strToolTipItemName , strToolTipItemName, 256 );
	}
}

void CColMapPositionWnd::RenderToolItemName()
{
	if( m_WIDToolTipDestWnd != WID_None )
	{
    	CWnd *pFDestWnd = Container->Get( m_WIDToolTipDestWnd );
		SRect WcSize;
		pFDestWnd->GetClientSize( &WcSize );
		
		WcSize.x += 1;
		WcSize.y -= 1;
		
		if( pFDestWnd->GetWindowType() == n_wtCheckButton )
		{
			WcSize.x += 1;
			WcSize.y += 1;
		}	
		
		nRui->GetHelpTip()->SetUpPosString(&WcSize, m_strToolTipItemName, D3DCOLOR_XRGB(255, 255, 255 ));
		nRui->GetHelpTip()->RenderProc(1.0f, D3DCOLOR_XRGB(252, 230, 170) );  
	}	
}

void CColMapPositionWnd::Check_BatchItem()
{
	int i = 0;
	int j = 0;
	
	bool lb_isFind_item = FALSE;
	
	if( !m_LinkColItem.ItemTable ) 
	{
		return;
	}

	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( j = 0; j < c_MaxInvenItemNum; ++j )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
		{
			continue;
		}
		
		if( m_LinkColItem.ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			lb_isFind_item = TRUE;
			break;
		}
	}
	
	///-----------------------------------------------------------------------
	///-- item 을 찾지 못 했을 경우
	if( !lb_isFind_item )
	{
		Close();
	}
}
