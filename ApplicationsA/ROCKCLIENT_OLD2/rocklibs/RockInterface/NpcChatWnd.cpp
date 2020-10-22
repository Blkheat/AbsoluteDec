// NpcChatWnd.cpp: implementation of the CNpcChatWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "..\\RockPCH.h"

#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "StateButton.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"


#include "..\\..\\bravolibs\\network\\protocol.h"

#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"
#include "..\\..\\quadlist.h"
#include "..\\..\\pc.h"

#include "..\\..\\bravolibs\\obj\\Nonplayer.h"
#include "..\\..\\bravolibs\\map\\field.h"
#include "..\\..\\Map.h"


#include "NpcChatWnd.h"

#include "InventoryWndProc.h"
#include "StoreWndProc.h"
#include "WarehouseWndProc.h"

#include "stringmanager.h"
#include "ItemSelectEldaMixWndProc.h"
#include "ItemSelectPotionWndProc.h"
#include "ItemEldaMixRWndProc.h"
#include "ItemEldaMixAWndProc.h"
#include "CAcryEnchantWnd.h"
#include "CAcryEnchantRWnd.h"
#include "ItemEldaDust.h"
#include "MapDataManager.h"

#include "EssenceWndProc.h"
#include "ItemEnchantWndProc.h"
#include "SpellStoneWndProc.h"
#include "StrengthStoneWndProc.h"


CNpcChatWnd		NpcChatWnd;

extern long		g_nowTime;


#ifdef C_SEALED_NPC_CHAT_RENEW
	#ifdef C_NPCCHAT_RENEWAL
		const int c_SelectBtnNum(6);
	#else 
		const int c_SelectBtnNum(10);
	#endif 
#else 
	#ifdef C_NPCCHAT_RENEWAL
		const int c_SelectBtnNum(6);
	#else 
		const int c_SelectBtnNum(4);
	#endif 		
#endif 





//-----------------------------------------------------------------------------
void CNpcChatWnd::Init()
{
	theNpcTableIdx = 0;
	theNpcCode = 0;
	m_StartScorll = 0 ; 

	bPayScriptStart = false;
	theLevel	  = NULL;
	theName	      = NULL;
	m_NpcDescList.clear();
#ifdef C_SEALED_NPC_CHAT_RENEW
	#ifdef C_NPCCHAT_RENEWAL
		theNpcPictureImg = NULL;
	#else 

	#endif 
#else 
	theNpcPictureImg = NULL;
#endif 
}

//-----------------------------------------------------------------------------

void CNpcChatWnd::Composition()
{

#ifdef C_SEALED_NPC_CHAT_RENEW
	#ifdef C_NPCCHAT_RENEWAL 
		CompositionNew();	
	#else 
		CompositionOld();
	#endif 
#else 
	#ifdef C_NPCCHAT_RENEWAL 
		CompositionNew();	
	#else 
		CompositionFirst();
	#endif 

#endif 
};


void CNpcChatWnd::CompositionNew()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

//	CTextBox*		tb;	
	CButton*		btn;	
	CImageBox9*		img9;
	CImageBox*      img;
	CTextBox*		tbox;

	int i =0  ;
	
	char strTemp[64] = {0,};


	SRect NpcChatWnd;
	NpcChatWnd.w	= 750;
	NpcChatWnd.h	= 400;
	NpcChatWnd.x	= center_x - ( NpcChatWnd.w / 2 );
	NpcChatWnd.y	= center_y - ( NpcChatWnd.h / 2 );
	
	FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_NpcChatWnd, WID_None, NpcChatWnd.x, NpcChatWnd.y, NpcChatWnd.w, NpcChatWnd.h, false );
	FrameWnd->SetClientImage( WID_None );
	FrameWnd->SetCaption( _RT("NPC Chat Window") );
	FrameWnd->SetWndProc( this );
	FrameWnd->SetTitle( false );
	FrameWnd->SetMove( false );
//	FrameWnd->SetZorder( n_zoTop );
	FrameWnd->SetVisible( false );
	FrameWnd->Initialize();
	
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_NpcChatWnd,  -2, -2, (NpcChatWnd.w-2)+4, (409)+4, false );
	img9->SetClientImage( 3984 );
	img9->SetWndTileSizeLR( 19, 18 );
	img9->SetWndTileSizeTB( 25, 20 );
	img9->SetFocusCheck( false );
	img9->SetAdjustPosition( 0, 0 );
	img9->SetClientColor(0,0,0);
	img9->SetClientAlpah(100);

	///-- Back Wnd
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_NpcChatWnd,  0, 0, (NpcChatWnd.w-2), 409, false );
	img9->SetClientImage( 3984 );
	img9->SetWndTileSizeLR( 19, 18 );
	img9->SetWndTileSizeTB( 25, 20 );
	img9->SetFocusCheck( false );
	img9->SetAdjustPosition( 0, 0 );
	img9->SetClientColor(230,245,222);
	img9->SetClientAlpah(220);
	
	///-- Content Wnd - Right
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_NpcChatWnd, NpcChatWnd.w - 492  , 230, 490, 162, false );
	img9->SetWndTileSizeLR( 19, 18 );
	img9->SetWndTileSizeTB( 25, 20 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_NPCCHAT_CONT_WND_L1 );
	img9->SetAdjustPosition( 0, 0 );
	
	///-- Content Wnd - Left
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_NpcChatWnd , 2 , 230, 250, 162, false );
	img9->SetWndTileSizeLR( 19, 18 );
	img9->SetWndTileSizeTB( 25, 20 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_NPCCHAT_CONT_WND_L1 );
	img9->SetAdjustPosition( 0, 0 );

	///-- NPC Image
	theNpcPictureImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NpcChat_PictureImg, WID_NpcChatWnd, 23, 0, 200 , 200, false );
	theNpcPictureImg->SetClientImage( 446 );
	theNpcPictureImg->SetFitImg(true);

	///-- NPC Name Box	228 , 26
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_NpcChatWnd, 10 , 200, 228, 26, false );
	img->SetClientImage( TID_NPCCHAT_NAME_BOX );
	img->SetFocusCheck( false );
	img->SetScaleTrans(true);

	///--- NPC Level
	theLevel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_NpcChat_LevelText, WID_NpcChatWnd,  20 , 206 , 23, 12, false );
	theLevel->SetFontR( n_fRsvFontNPCChat2 );
	theLevel->SetTextColor( 240, 240, 200 );
	theLevel->SetBlank( 0, 0 );	
	theLevel->SetText("99")	;

	///--- NPC Name
	theName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_NpcChat_NameText, WID_NpcChatWnd, 25 , 205 , 228, 12, false );	
	theName->SetFontR( n_fRsvFontWndStatic );
	theName->SetBlank( 0, 0 );	
	theName->SetText("가나다라마마사");
	theName->SetTextColor( 0, 0, 0 );

	int LeftWHeight = 242 ; 
	
	///--- 작은창 에 있는것 . 
	NPCCitizen = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NpcChatWnd, 10 , LeftWHeight  , 200, 12, false );	
	NPCCitizen->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	NPCCitizen->SetClientImage( TID_None );	
	NPCCitizen->SetBlank( 0, 0 );	
	NPCCitizen->SetText("");
	NPCCitizen->SetAlignText( n_atLeft, n_atCenter );
	NPCCitizen->SetTextColor(240,240,200);
	//NPCCitizen->SetLimitLength(30);
	
	NPCName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NpcChatWnd, 10 , LeftWHeight+18   , 200, 12, false );	
	NPCName->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	NPCName->SetClientImage( TID_None );	
	NPCName->SetBlank( 0, 0 );	
	NPCName->SetText("이름 : 레인");
	NPCName->SetAlignText( n_atLeft, n_atCenter );
	NPCName->SetTextColor(240,240,200);
	
	NPCSex = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NpcChatWnd, 10 , LeftWHeight+18+ 18   , 200, 12, false );	
	NPCSex->SetClientImage( TID_None );	
	NPCSex->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	NPCSex->SetBlank( 0, 0 );	
	NPCSex->SetText("성별 : 레인");
	NPCSex->SetAlignText( n_atLeft, n_atCenter );
	NPCSex->SetTextColor(240,240,200);

	NPCLevel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NpcChatWnd, 10 , LeftWHeight+18+18+18   , 200, 12, false );	
	NPCLevel->SetClientImage( TID_None );	
	NPCLevel->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	NPCLevel->SetBlank( 0, 0 );	
	NPCLevel->SetText("레벨 : 99");
	NPCLevel->SetAlignText( n_atLeft, n_atCenter );
	NPCLevel->SetTextColor(240,240,200);

		
	tbox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NpcChatWnd, 10 , LeftWHeight+18+18+18+18   , 200, 42, false );	
	tbox->SetClientImage( TID_None );	
	tbox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	tbox->SetBlank( 0, 0 );	
	Rsprintf(RWCHAR(strTemp) , _RT("%s:") , G_STRING_CHAR(IDS_WND_MISSION) );
	tbox->SetText(strTemp);
	tbox->SetAlignText( n_atLeft, n_atCenter );
	tbox->SetTextColor(240,240,200);
	
	SStringInfo *stringInfo = tbox->GetStringInfo(); 
		
	NPCMission = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NpcChatWnd, 15 + stringInfo->Width  , LeftWHeight+18+18+18+18   , 200, 42, false );	
	NPCMission->SetClientImage( TID_None );	
	NPCMission->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	NPCMission->SetBlank( 0, 0 );	
	NPCMission->SetText("");
	NPCMission->SetAlignText( n_atLeft, n_atCenter );
	NPCMission->SetTextColor(240,240,200);
	NPCMission->SetMultiLine(true);

	///-- Content Text
	for(  i = 0 ; i < MAX_NPC_CHAT_STRING ; ++i )
	{
		m_AskTextEx[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NpcChatWnd,  NpcChatWnd.w - 480 , 20 + (i*14 ), 477, 12, false );
		m_AskTextEx[i]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_AskTextEx[i]->SetClientImage( TID_None );	
		m_AskTextEx[i]->SetTextColor( 100, 100, 100);
		m_AskTextEx[i]->SetHID( HID_None );
		m_AskTextEx[i]->SetAlignText(n_atLeft , n_atCenter );
		m_AskTextEx[i]->SetText( _RT("!"));
	}

//	InsertNpcDesc(NULL);
//	UpdateNpcDesc();



	//...........................................................................................................
	// 스크롤
	//...........................................................................................................
	m_pScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None , WID_NpcChatWnd, NpcChatWnd.w - 20 , 30, 13, 170, false );
	m_pScrollBar->InitSlide( stVertSld, 0, 100 - 17, 0, 13 );//Type, min, max, value, 썸의 폭
	m_pScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	m_pScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );


	///-- Close Button
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_Ask_CloseBtn, WID_NpcChatWnd, NpcChatWnd.w - 27, 2, 23, 24, false );
	btn->SetBtnImage( TID_NPCCHAT_CLOSE_BT, TID_NPCCHAT_CLOSE_OVR, TID_NPCCHAT_CLOSE_CLK, TID_None );

	///-- Next Button
	m_NextBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_Ask_NextBtn, WID_NpcChatWnd, NpcChatWnd.w - 127 , 200, 110, 27, false );
//	m_NextBtn->SetBtnImage( TID_NpcChat_Ask_NextBtn_Focus, TID_NpcChat_Ask_NextBtn_Focus, TID_NpcChat_Ask_NextBtn_Select, TID_NpcChat_Ask_NextBtn_Select );
	m_NextBtn->SetBtnImage( TID_NPCCHAT_NEXT_BT , TID_NPCCHAT_NEXT_OVR, TID_NPCCHAT_NEXT_CLK, TID_NPCCHAT_NEXT_CLK );
	m_NextBtn->SetFontR( n_fRsvFontNPCChat2 );
	Rsprintf( RWCHAR(strTemp), _RT("  %s"), G_STRING_CHAR( IDS_WND_NEXTCHAT ) );
	m_NextBtn->SetText( strTemp );
	m_NextBtn->SetAlignText( n_atLeft, n_atCenter );		
	m_NextBtn->SetTextColor( 250, 250, 250 );

	int Height = 242;
	
	for( i=0; i < c_SelectBtnNum; ++i)
	{
		btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_AnswerBtn1+i, WID_NpcChatWnd, NpcChatWnd.w - 475 , Height , 470, 20, false );  
		btn->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );
		btn->SetFontR( n_fRsvFontNPCChat2 );
		btn->SetAlignText( n_atLeft, n_atCenter );
	//	btn->SetVisible( false );
		btn->SetTextColor( 0, 0, 0 );
		btn->SetText(_RT(""));
		Height += 18;
	}
}


void CNpcChatWnd::Open()
{
	
	nRui->SendEvent( WID_NpcChatWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
#ifdef C_NPCCHAT_RENEWAL
#else 
	//theNpcPictureImg->SetClientImage( TID_None );					
#endif
	SetSayString( _RT("") );	
	OneRun = true; 



#ifdef NPC_CHAT_EXTENSION
	m_StartScorll = 0 ; 
	m_pScrollBar->SetValue( 0 );
	SetListScroll(0);
#endif 


}

//-----------------------------------------------------------------------------
void CNpcChatWnd::Update()
{
	static TCHAR tmpStr[128] = {0,};

	/* 깜빡임 없엠
	if( m_NextBtn->IsVisible() )
	{
		static DWORD OldTimer  = timeGetTime();
		static BOOL	 ISTwinkle = FALSE;
	}
	*/
	
	if( bPayScriptStart == TRUE)
	{
		//NPC 이미지 와 레벨과  위치는 필요 없고.. 
		SNPCBaseInfo* npc_Info;

		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( theNpcCode ); 
#ifdef PET_NPCNAME_BUG
		if ( npc_Info == NULL )	{ return; }  //if( npc_Info ) return;
#endif //PET_NPCNAME_BUG
		Rsprintf( RWCHAR(tmpStr), _RT("%d"), npc_Info->theLevel );
		theLevel->SetText( tmpStr );		
		//-- NPC Name
		Rsprintf(RWCHAR(tmpStr), _RT("%s"), RWCHAR(npc_Info->theName) );
		theName->SetText( tmpStr );	
		
#ifdef C_NPCCHAT_RENEWAL

		int MapNation = G_MAP_NATION( g_Pc.m_MainPC.lCurMap ); 

		if(  MapNation == 0 ) 
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_NATION) , G_STRING_CHAR(IDS_RAIN )  );
			NPCCitizen->SetText(tmpStr);

		}
		else if(MapNation == 1 ) 
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_NATION) , G_STRING_CHAR(IDS_MILLENA )  );
			NPCCitizen->SetText(tmpStr);
		}
		else
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_NATION) , G_STRING_CHAR(IDS_WND_NETURAL_PEOPLE)  ); 
			NPCCitizen->SetText(tmpStr);
		}
		
		Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_NAME) , RWCHAR(npc_Info->theName)  ); 
		NPCName->SetText(tmpStr);
		NPCName->SetLimitLength(33);

		if( npc_Info->theGender== 1 )//수컷 
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_GENDER) , G_STRING_CHAR( IDS_MAN ) );
			NPCSex->SetText( tmpStr );
		}
		else if( npc_Info->theGender == 2 ) // 암컷 
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_GENDER) , G_STRING_CHAR( IDS_WOMAN ) ); 
			NPCSex->SetText( tmpStr );
		}
		else 
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_GENDER) , G_STRING_CHAR( IDS_WND_NOGENDER ) ); 
			NPCSex->SetText( tmpStr );
		}

		Rsprintf( RWCHAR(tmpStr), _RT("%s:%d"), G_STRING_CHAR(IDS_CHARSTATE_LEVEL) , npc_Info->theLevel  ); 
		NPCLevel->SetText(tmpStr);

		Rsprintf( RWCHAR(tmpStr), _RT("%s"), npc_Info->theMoveDlg  ); 
		NPCMission->SetText(tmpStr);
#endif 
	}
	else
	{
		
		//-- NPC Level
		Rsprintf(RWCHAR(tmpStr), _RT("%d"), g_Map.m_NpcTable[theNpcTableIdx].theLevel );
		theLevel->SetText( tmpStr );		
		//-- NPC Name
		Rsprintf(RWCHAR(tmpStr), _RT("%s"), RWCHAR(g_Map.m_NpcTable[theNpcTableIdx].szName) );
		theName->SetText( tmpStr );	
		
		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[theNpcTableIdx].code ); 
		
		if( npc_Info == NULL )
		{
			return;
		}
#ifdef C_NPCCHAT_RENEWAL

		int MapNation = G_MAP_NATION( g_Pc.m_MainPC.lCurMap ); 
		
		if(  MapNation == 0 ) 
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_NATION) , G_STRING_CHAR(IDS_RAIN )  );
			NPCCitizen->SetText(tmpStr);
			
		}
		else if(MapNation == 1 ) 
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_NATION) , G_STRING_CHAR(IDS_MILLENA )  );
			NPCCitizen->SetText(tmpStr);
		}
		else
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_NATION) , G_STRING_CHAR(IDS_WND_NETURAL_PEOPLE)  ); 
			NPCCitizen->SetText(tmpStr);
		}
		
		Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_NAME) , RWCHAR(npc_Info->theName)  ); 
		NPCName->SetText(tmpStr);
		NPCName->SetLimitLength(33);
	
		if( npc_Info->theGender== 1 )//수컷 
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_GENDER) , G_STRING_CHAR( IDS_MAN ) );
			NPCSex->SetText( tmpStr );
		}
		else if( npc_Info->theGender == 2 ) // 암컷 
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_GENDER) , G_STRING_CHAR( IDS_WOMAN ) ); 
			NPCSex->SetText( tmpStr );
		}
		else 
		{
			Rsprintf( RWCHAR(tmpStr), _RT("%s:%s"), G_STRING_CHAR(IDS_CHARSTATE_GENDER) , G_STRING_CHAR( IDS_WND_NOGENDER ) ); 
			NPCSex->SetText( tmpStr );
		}

		Rsprintf( RWCHAR(tmpStr), _RT("%s:%d"), G_STRING_CHAR(IDS_CHARSTATE_LEVEL) , npc_Info->theLevel  ); 
		NPCLevel->SetText(tmpStr);
				
		Rsprintf( RWCHAR(tmpStr), _RT("%s"), RWCHAR(npc_Info->theMoveDlg)  ); 
		NPCMission->SetText(tmpStr);

#endif 

		if( g_Map.m_Npc[theNpcTableIdx].m_curt_event.type == SM_STATE_DIE )
		{
			CloseNpcChatWnd();
		}
		else
		{
			int nPosX = 0;
			int nPosY = 0;
			float distance = 0.0f;
			
			nPosX	= g_Map.m_NpcTable[theNpcTableIdx].lDestBlock%g_Pc.m_cSize;
			nPosY	= g_Map.m_NpcTable[theNpcTableIdx].lDestBlock/g_Pc.m_cSize;
			
			CurPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
			CurPos.z = g_Pc.GetPlayer()->GetPosTM()._43;
			TempPos.x = g_Map.m_Npc[theNpcTableIdx].GetPosTM()._41;
			TempPos.z  = g_Map.m_Npc[theNpcTableIdx].GetPosTM()._43;
			
			distance = DIST( CurPos.x, CurPos.z , TempPos.x, TempPos.z  );
			
			float range = 120.0f;
			if( g_Map.m_NpcTable[theNpcTableIdx].code == 5301 )		// 천년의 나무는 좀 더 멀리서 ^ ^;;
			{
				range = 240.0f;
			}
			
			if( distance > range )
			{
				CloseNpcChatWnd();				
				nRui->SendEvent( WID_InventoryWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
#ifdef WIDE_PAY_INVENTORY //유료인벤확장
				nRui->SendEvent( WID_PayInventory_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );//유료인벤확장
#endif
			}
		}
	}

	return;
}

void CNpcChatWnd::CloseNpcChatWnd( bool aIsSendCancel )
{
	nRui->SendEvent( WID_NpcChatWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_StoreWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_SkillLearn_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( WID_REPAIR_WND, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	
	g_WarehouseWnd.CloseWarehouseWnd();
	g_ItemSelectPotionWndProc.Close();
	
#ifdef C_SEALED_ELDASTONE_DUST
	g_ItemEldaDustWndProc.Close();
#endif 
	
#ifdef C_SEALED_NPC_CHAT_RENEW
	#ifdef C_NPCCHAT_RENEWAL
		theNpcPictureImg->SetClientImage( TID_None );					
	#endif 
#else 
		theNpcPictureImg->SetClientImage( TID_None );					
#endif 


#ifdef MIX_ELDASTONE
	//g_ItemSelectEldaMixWndProc.Close();
	g_ItemEldaMixAWndProc.Close();
//	g_ItemEldaMixRWndProc.Close();
#endif 

#ifdef C_SUBJOB_RENEW
	if( g_EssenceWndProc )
	{
		g_EssenceWndProc->Close();
		g_SpellStoneWndProc->Close();
		g_StrengthStoneWndProc->CloseEnchantWnd();
		g_ItemEnchantWndProc.CloseEnchantWnd();
	}
#endif 



	if( g_pAcryEnchantWnd )
	{
		g_pAcryEnchantWnd->Close();
	}
	
	

	nRui->SendEvent( WID_ItemBuyWnd, n_emSetVisible, false, 0, 0, 0 );
	nRui->SendEvent( WID_ItemSellWnd, n_emSetVisible, false, 0, 0, 0 );
	nRui->SetModal( WID_None );

	for(int i = 0 ; i < c_SelectBtnNum ; ++i )
	{
		nRui->SendEvent( WID_NpcChat_AnswerBtn1 + i , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}

	if( aIsSendCancel )
	{
		g_RockClient.Send_RpCsScriptEnd( theNpcCode );		
	}
	
	if( nRui->GetCursorItem() )
	{
		nRui->ResetCursorItem();
	}

	m_NpcDescList.clear();

	
	theNpcCode = 0;	
	bPayScriptStart = false;



	return;
}

//-----------------------------------------------------------------------------
void CNpcChatWnd::Proc( SEventMessage* EMsg )
{
	int BtnPos = WID_NpcChat_AnswerBtn1;
	BtnPos = abs((long)WID_NpcChat_AnswerBtn1 - (long)EMsg->FocusWnd) ;
	int i = 0 ; 
#ifdef NPC_CHAT_EXTENSION
	if( EMsg->FocusWnd == m_pScrollBar->GetID() ) 
	{
		if( ( EMsg->Notify == NM_SLD_MOVEBAR ) || 
			( EMsg->Notify == NM_SLD_CLICKMOVEBAR ) || 
			( EMsg->Notify == NM_SLD_BTNMOVERBAR ) )
		{
			SetListScroll( (int)EMsg->fParam );			
		}
	}	
#endif 


	switch( EMsg->FocusWnd )
	{
	case WID_NpcChat_Ask_NextBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
//			nRui->SendEvent( WID_NpcChat_Ask_NextBtn, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
//			TID_NpcChat_Ask_NextBtn_Select // Hold 시킨다??
			static long delay_time = false;

			if( SAFE_TIME_COMPARE( g_nowTime , > , SAFE_TIME_ADD( delay_time , 300 ) ) )
			{
				delay_time = g_nowTime;
				g_RockClient.Send_RpCsAnswerScript( 255 );
			}						
		}
		break;
	case WID_NpcChat_Ask_CloseBtn:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			CloseNpcChatWnd(); 

			// 이 아래 것들은...흠...서버에서 닫으라고 해야 하지 않을까??? ㅡㅡ; 흠...
			StoreWnd.FrameWnd->SetMove( true );
			InventoryWnd.FrameWnd->SetMove( true );
		}
		break;
	case WID_NpcChat_AnswerBtn1:
	case WID_NpcChat_AnswerBtn2:
	case WID_NpcChat_AnswerBtn3:
	case WID_NpcChat_AnswerBtn4:
	case WID_NpcChat_AnswerBtn5:
	case WID_NpcChat_AnswerBtn6:
	case WID_NpcChat_AnswerBtn7:
	case WID_NpcChat_AnswerBtn8:
	case WID_NpcChat_AnswerBtn9:
	case WID_NpcChat_AnswerBtn10:
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			g_RockClient.Send_RpCsAnswerScript( BtnPos );

			for(i = 0 ; i < c_SelectBtnNum ; ++i)
			{
				nRui->SendEvent( WID_NpcChat_AnswerBtn1 + i, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
			}
		}
		break;

	default:
		break;
	} 

	return;
}

//-----------------------------------------------------------------------------

void CNpcChatWnd::SetSayString(void* pSayString )
{
//	CWnd* Wnd = NULL;
//	Wnd = Container->Get( WID_NpcChat_AskText );	
//	Wnd->ReceiveEvent( n_emSetText, (EPARAM)aSayString, 15, 25, 50 );			
#ifdef UNICODE_ROCK
	char* aSayString = (char*)pSayString;
#else 
	WCHAR* aSayString = (WCHAR*)pSayString;
#endif 

	



#ifdef NPC_CHAT_EXTENSION

	m_pScrollBar->SetValue( 0 );
	SetListScroll(0);
	InsertNpcDesc(aSayString);
	UpdateNpcDesc();	
#else 
	m_AskText->SetText( aSayString );
	m_AskText->SetTextColor( 15, 25, 50 );
	m_AskText->SetDrawTextTime( 25 );
#endif 
	
	return;
}


void CNpcChatWnd::SetAnswerBtnString( int aBtnWIDIndex, char* paAnswerString )
{	
	CWnd* Wnd = NULL;
	Wnd = Container->Get( aBtnWIDIndex );	
	int StrNum = Rstrlen(RWCHAR(paAnswerString));
	int i = 0 ;

#ifdef UNICODE_ROCK
	WCHAR *	aAnswerString = RWCHAR(paAnswerString);
#else
	char*	aAnswerString = RWCHAR(paAnswerString);
#endif

	for(i = 0 ; i < StrNum ; ++i )
	{
		if( aAnswerString[i] == _RT('`'))
		{
			aAnswerString[i] = _RT(',');
		}
	}

	char strTemp[255] = {0,};
	Rsprintf( RWCHAR(strTemp), _RT("    %s"), RWCHAR(aAnswerString) );
	Wnd->ReceiveEvent( n_emSetText, (EPARAM)strTemp, 250, 250, 220 );			

	return;
}

void CNpcChatWnd::SetWheelValue( short _wheel )
{
#ifdef NPC_CHAT_EXTENSION
	int value = 0;
	int Max   = 0;	
	
	Max = m_pScrollBar->GetSldMax();
	m_pScrollBar->GetValue( &value );
	
	if( ( _wheel < 0 ) && ( value < Max ) )
	{
		value += 1;
	}
	else if( ( _wheel > 0 ) && ( value > 0 ) )
	{
		value -= 1;
	}
	
	m_pScrollBar->SetValue( value );
	SetListScroll( value );

	UpdateNpcDesc();
#endif 

}

void CNpcChatWnd::SetListScroll(int _linenum )
{
	m_StartScorll = _linenum;
	UpdateNpcDesc();
}

void CNpcChatWnd::InsertNpcDesc(void *pNpcString)
{
	int i = 0 ;

	RTCHAR *NpcString = (RTCHAR*)pNpcString;

	char Temp[1024] = {0,};
	
	int m_StrCount = 0 ; 
	int StrCount1024 = 0 ; 
	int iStrSize = 0 ; 
	int LineCount = 0 ; 
	
	if( Rstrcmp( NpcString  , _RT("") ) == 0 )
		return;
	
	m_NpcDescList.clear();
	SAFE_STR_CPY( Temp , NpcString , 1000 );
	//	Rstrncpy( Temp  ,  NpcString , 1020 );
	int size = Rstrlen(NpcString ) ; 
	
	int iRealSize =0 ; 
		
	for( i  =0 ; i < size ;  )
	{
		SNpcDesc NpcDesc;
		
		iRealSize = 0 ; 

		size += iRealSize ; 

		iStrSize  = nRui->GetStrWidthMax(m_AskTextEx[0]->GetFontg() ,  RWCHAR(Temp) + StrCount1024  , iRealSize , 450 );
		
		if( iStrSize == 0 && iRealSize == 0  ) 
		{
			break;  
		}

		Rstrncpy(RWCHAR(NpcDesc.strDesc) , RWCHAR(Temp)+StrCount1024 , iStrSize );
			
		StrCount1024 += iRealSize; 
		m_StrCount += iRealSize;
		
		NpcDesc.dIndex = LineCount;
		m_NpcDescList.push_back(NpcDesc);
		
		LineCount++;
#ifdef UNICODE_ROCK
		if(StrCount1024 > 400)
#else
		if(StrCount1024 > 800)
#endif 
		{
			SAFE_STR_CPY( Temp , NpcString+m_StrCount , 1000 );
			StrCount1024 = 0 ; 
		}
	}
	
	
}


void CNpcChatWnd::UpdateNpcDesc()
{
	int i =0  ;	
	int iLineCount = 0 ; 

	SNpcDescListPos lPos  = m_NpcDescList.begin(); 
	for(i =0 ; i < MAX_NPC_CHAT_STRING ; ++i) 
	{
		m_AskTextEx[i]->SetText(_RT(""));
	}

	for(i = m_StartScorll ; i < m_NpcDescList.size() ; i++)
	{
		if( m_NpcDescList[i].dIndex ==  (m_StartScorll + iLineCount))
		{
			m_AskTextEx[iLineCount]->SetText( m_NpcDescList[i].strDesc);
			if((iLineCount++) == MAX_NPC_CHAT_STRING-1  )
			{
				break; 
			}
		}
	}
/*
	for( i = iLineCount  ; i < MAX_NPC_CHAT_STRING ; ++i   )
	{
		m_AskTextEx[i]->SetText(_RT(""));
	}
*/	
}





void CNpcChatWnd::CompositionOld()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

//	CTextBox*		tb;	
	CButton*		btn;	
	CImageBox9*		img9;
	CImageBox*      img;
	int i =0  ;

	char strTemp[64] = {0,};


	SRect NpcChatWnd;
	NpcChatWnd.w	= 522;
	NpcChatWnd.h	= 460;
	NpcChatWnd.x	= center_x - ( NpcChatWnd.w / 2 );
	NpcChatWnd.y	= center_y - ( NpcChatWnd.h / 2 );
	
	FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_NpcChatWnd, WID_None, NpcChatWnd.x, NpcChatWnd.y, NpcChatWnd.w, NpcChatWnd.h, false );
	FrameWnd->SetClientImage( WID_None );
	FrameWnd->SetCaption( _RT("NPC Chat Window") );
	FrameWnd->SetWndProc( this );
	FrameWnd->SetTitle( false );
	FrameWnd->SetVisible( false );
	FrameWnd->SetMove( false );
//	FrameWnd->SetZorder( n_zoTop );
	FrameWnd->Initialize();
	
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_NpcChatWnd,  -2, -2, 520+4, (228*2)+4, false );
	img9->SetClientImage( 3984 );
	img9->SetWndTileSizeLR( 19, 18 );
	img9->SetWndTileSizeTB( 25, 20 );
	img9->SetFocusCheck( false );
	img9->SetAdjustPosition( 0, 0 );
	img9->SetClientColor(0,0,0);
	img9->SetClientAlpah(100);


	///-- Back Wnd
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_NpcChatWnd,  0, 0, 520, 228*2, false );
	img9->SetClientImage( 3984 );
	img9->SetWndTileSizeLR( 19, 18 );
	img9->SetWndTileSizeTB( 25, 20 );
	img9->SetFocusCheck( false );
	img9->SetAdjustPosition( 0, 0 );
	img9->SetClientColor(230,245,222);
	img9->SetClientAlpah(220);
	
	///-- Content Wnd
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_NpcChatWnd, 10, 250, 500, 200, false );
	img9->SetWndTileSizeLR( 19, 18 );
	img9->SetWndTileSizeTB( 25, 20 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_NPCCHAT_CONT_WND_L1 );
	img9->SetAdjustPosition( 0, 0 );
	
	///-- NPC Image
//	theNpcPictureImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NpcChat_PictureImg, WID_NpcChatWnd, 0, 0, 256, 256, false );
//	theNpcPictureImg->SetClientImage( TID_None );

	///-- NPC Name Box
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_NpcChatWnd, -13 , -13, 228, 26, false );
	img->SetClientImage( TID_NPCCHAT_NAME_BOX );
	img->SetFocusCheck( false );
	
	///--- NPC Level
	theLevel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_NpcChat_LevelText, WID_NpcChatWnd,  -5, -7, 23, 12, false );
	theLevel->SetFontR( n_fRsvFontNPCChat2 );
	theLevel->SetTextColor( 240, 240, 200 );
	theLevel->SetBlank( 0, 0 );	
	theLevel->SetText("99")	;

	///--- NPC Name
	theName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_NpcChat_NameText, WID_NpcChatWnd, 37, -7 , 182, 12, false );	
	theName->SetFontR( n_fRsvFontWndStatic );
	theName->SetBlank( 0, 0 );	
	theName->SetText("가나다라마마사");



	///-- Content Text

#ifdef NPC_CHAT_EXTENSION

	for(  i = 0 ; i < MAX_NPC_CHAT_STRING ; ++i )
	{
		m_AskTextEx[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_NpcChatWnd,  20, 20 + (i*14 ), 477, 12, false );
		m_AskTextEx[i]->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
		m_AskTextEx[i]->SetClientImage( TID_None );	
		m_AskTextEx[i]->SetTextColor( 100, 100, 100);

		m_AskTextEx[i]->SetHID( HID_None );
		m_AskTextEx[i]->SetAlignText(n_atLeft , n_atCenter );
		m_AskTextEx[i]->SetText( _RT("테스트로 출력테스트로 출력!!!"));
	}
   
//	InsertNpcDesc(NULL);
//	UpdateNpcDesc();



	//...........................................................................................................
	// 스크롤
	//...........................................................................................................
	m_pScrollBar = (CScrollBar*)nRui->CreateWnd( n_wtScrollBar, WID_None , WID_NpcChatWnd, NpcChatWnd.w - 20 , 30, 13, 190, false );
	m_pScrollBar->InitSlide( stVertSld, 0, 100 - 17, 0, 13 );//Type, min, max, value, 썸의 폭
	m_pScrollBar->SetClientImage( TID_CTRL_SCR_BG );
	m_pScrollBar->SetThumbImage( TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR, TID_CTRL_SCR_BAR_CLK, TID_None );
	m_pScrollBar->SetScrollBtnA( TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT, TID_CTRL_SCR2_UBT_CLK, TID_None );
	m_pScrollBar->SetScrollBtnB( TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT, TID_CTRL_SCR2_DBT_CLK, TID_None );

#else 

	m_AskText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_NpcChat_AskText, WID_NpcChatWnd,  20, 40, 477, 168, false );
	m_AskText->SetFontR( n_fRsvFontWndStatic );
	m_AskText->SetText( _RT("테스트로 출력테스트로 출력!!!"));
	m_AskText->SetMultiLine( true );
	m_AskText->SetBlank( 0, 0 );
	m_AskText->SetLineInterval(10);
	m_AskText->SetHID( HID_None );



#endif 
	
	



	///-- Close Button
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_Ask_CloseBtn, WID_NpcChatWnd, 495, 2, 23, 24, false );
	btn->SetBtnImage( TID_NPCCHAT_CLOSE_BT, TID_NPCCHAT_CLOSE_OVR, TID_NPCCHAT_CLOSE_CLK, TID_None );

	///-- Next Button
	m_NextBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_Ask_NextBtn, WID_NpcChatWnd, 400, 220, 110, 27, false );
//	m_NextBtn->SetBtnImage( TID_NpcChat_Ask_NextBtn_Focus, TID_NpcChat_Ask_NextBtn_Focus, TID_NpcChat_Ask_NextBtn_Select, TID_NpcChat_Ask_NextBtn_Select );
	m_NextBtn->SetBtnImage( TID_NPCCHAT_NEXT_BT , TID_NPCCHAT_NEXT_OVR, TID_NPCCHAT_NEXT_CLK, TID_NPCCHAT_NEXT_CLK );
	m_NextBtn->SetFontR( n_fRsvFontNPCChat2 );
	Rsprintf( RWCHAR(strTemp), _RT("  %s"), G_STRING_CHAR( IDS_WND_NEXTCHAT ) );
	m_NextBtn->SetText( strTemp );
	m_NextBtn->SetAlignText( n_atLeft, n_atCenter );		
	m_NextBtn->SetTextColor( 250, 250, 250 );

	int Height = 262;
	
	for( i=0; i < c_SelectBtnNum; ++i)
	{
		btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_AnswerBtn1+i, WID_NpcChatWnd, 25, Height , 470, 20, false );  
		btn->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );
		btn->SetFontR( n_fRsvFontNPCChat2 );
		btn->SetAlignText( n_atLeft, n_atCenter );
		btn->SetVisible( false );
		btn->SetTextColor( 0, 0, 0 );
		Height += 18;
	}



}


void CNpcChatWnd::CompositionFirst()
{
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;

//	CTextBox*		tb;	
	CButton*		btn;	
	CImageBox9*		img9;
	CImageBox*      img;

	char strTemp[64] = {0,};
	
	SRect NpcChatWnd;
	NpcChatWnd.w	= 732;
	NpcChatWnd.h	= 253;
	NpcChatWnd.x	= center_x - ( NpcChatWnd.w / 2 );
	NpcChatWnd.y	= center_y -  NpcChatWnd.h  ;
	
	
	FrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_NpcChatWnd, WID_None, NpcChatWnd.x, NpcChatWnd.y, NpcChatWnd.w, NpcChatWnd.h, false );
	FrameWnd->SetClientImage( WID_None );
	FrameWnd->SetCaption( _RT("NPC Chat Window") );
	FrameWnd->SetWndProc( this );
	FrameWnd->SetTitle( false );
	FrameWnd->SetVisible( false );
	FrameWnd->SetMove( false );
	FrameWnd->SetZorder( n_zoTop );
	FrameWnd->Initialize();

	///-- Back Wnd
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_NpcChatWnd,  212, 25, 520, 228, false );
	img9->SetWndTileSizeLR( 74, 70 );
	img9->SetWndTileSizeTB( 200, 8 );

	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_NPCCHAT_WND_L1 );
	img9->SetAdjustPosition( 0, 0 );

	///-- Content Wnd
	img9 = (CImageBox9*)nRui->CreateWnd( n_wtImageBox9, WID_None, WID_NpcChatWnd, 299, 149, 423, 94, false );
	img9->SetWndTileSizeLR( 19, 18 );
	img9->SetWndTileSizeTB( 25, 20 );
	img9->SetFocusCheck( false );
	img9->SetClientImage( TID_NPCCHAT_CONT_WND_L1 );
	img9->SetAdjustPosition( 0, 0 );
	
	///-- NPC Image
	theNpcPictureImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_NpcChat_PictureImg, WID_NpcChatWnd, 0, 0, 256, 256, false );
	theNpcPictureImg->SetClientImage( TID_None );

	///-- NPC Name Box
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_NpcChatWnd,  41, 232, 228, 26, false );
	img->SetClientImage( TID_NPCCHAT_NAME_BOX );
	img->SetFocusCheck( false );

	///-- Content Text
	m_AskText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_NpcChat_AskText, WID_NpcChatWnd,  249, 61, 455, 62, false );
	m_AskText->SetFontR( n_fRsvFontNPCChat );
	m_AskText->SetText( _RT("테스트로 출력.테스트로 출력.테스트로 출력.테스트로 출력.테스트로 출력.테스트로 출력.테스트로 출력테스트로 출력!!!"));
	m_AskText->SetMultiLine( true );
	m_AskText->SetBlank( 0, 0 );
	m_AskText->SetLineInterval(4);
	m_AskText->SetHID( HID_None );

	///-- Close Button
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_Ask_CloseBtn, WID_NpcChatWnd, 697, 32, 23, 24, false );
	btn->SetBtnImage( TID_NPCCHAT_CLOSE_BT, TID_NPCCHAT_CLOSE_OVR, TID_NPCCHAT_CLOSE_CLK, TID_None );

	///-- Next Button
	m_NextBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_Ask_NextBtn, WID_NpcChatWnd, 610, 112, 110, 27, false );
//	m_NextBtn->SetBtnImage( TID_NpcChat_Ask_NextBtn_Focus, TID_NpcChat_Ask_NextBtn_Focus, TID_NpcChat_Ask_NextBtn_Select, TID_NpcChat_Ask_NextBtn_Select );
	m_NextBtn->SetBtnImage( TID_NPCCHAT_NEXT_BT , TID_NPCCHAT_NEXT_OVR, TID_NPCCHAT_NEXT_CLK, TID_NPCCHAT_NEXT_CLK );
	m_NextBtn->SetFontR( n_fRsvFontNPCChat2 );
	Rsprintf( RWCHAR(strTemp), _RT("  %s"), G_STRING_CHAR( IDS_WND_NEXTCHAT ) );
	m_NextBtn->SetText( strTemp );
	m_NextBtn->SetAlignText( n_atLeft, n_atCenter );		
	m_NextBtn->SetTextColor( 250, 250, 250 );

	///--- NPC Level
	theLevel = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_NpcChat_LevelText, WID_NpcChatWnd,  48, 239, 23, 12, false );
	theLevel->SetFontR( n_fRsvFontNPCChat2 );
	theLevel->SetTextColor( 240, 240, 200 );
	theLevel->SetBlank( 0, 0 );	
	
	///--- NPC Name
	theName = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_NpcChat_NameText, WID_NpcChatWnd, 79, 239, 182, 12, false );	
	theName->SetFontR( n_fRsvFontNPCChat2 );
	theName->SetBlank( 0, 0 );	
	

	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_AnswerBtn1, WID_NpcChatWnd, 310, 159, 399, 20, false );  
	btn->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );
	btn->SetFontR( n_fRsvFontNPCChat2 );
	btn->SetAlignText( n_atLeft, n_atCenter );
	btn->SetVisible( false );
	btn->SetTextColor( 110, 110, 110 );
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_AnswerBtn2, WID_NpcChatWnd, 310, 177, 399, 20, false ); 
	btn->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );
	btn->SetFontR( n_fRsvFontNPCChat2 );
	btn->SetAlignText( n_atLeft, n_atCenter );
	btn->SetVisible( false );
	btn->SetTextColor( 110, 110, 110 );
	
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_AnswerBtn3, WID_NpcChatWnd, 310, 195, 399, 20, false ); 
	btn->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );
	btn->SetFontR( n_fRsvFontNPCChat2 );
	btn->SetAlignText( n_atLeft, n_atCenter );
	btn->SetVisible( false );
	btn->SetTextColor( 110, 110, 110 );

	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_AnswerBtn4, WID_NpcChatWnd, 310, 213, 399, 20, false );  
	btn->SetBtnImage( TID_None, TID_CTRL_TEXT_OVER, TID_CTRL_TEXT_OVER, TID_None );
	btn->SetFontR( n_fRsvFontNPCChat2 );
	btn->SetAlignText( n_atLeft, n_atCenter );
	btn->SetVisible( false );
	btn->SetTextColor( 110, 110, 110 );
/*
	btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_NpcChat_AnswerBtn5, WID_NpcChatWnd, 330, 254, 207, 27, false );		
	btn->SetBtnImage( TID_NpcChat_AnswerBtn_Normal, TID_NpcChat_AnswerBtn_Normal, TID_NpcChat_AnswerBtn_Select, TID_None );
	btn->SetAlignText( n_atLeft, n_atCenter );
	btn->SetVisible( false );
	btn->SetTextColor( 110, 110, 110 );
*/
	

}
