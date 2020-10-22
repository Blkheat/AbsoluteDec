#include "..\\RockPCH.h"

#include "Rui.h"
#include "..\\..\\brother.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
//#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"



#include "..\\..\\global.h"
#include "..\\..\\RockClient.h"

#include "..\\..\\bravolibs\\obj\\SkillSystem.h"

//#include "pc.h"
#include "ItemEldaMixAWndProc.h"
#include "StringManager.h"
#include "..\\..\\VFileSystem.h"

#include "ItemSelectEldaMixWndProc.h"
#include "InventoryWndProc.h"
#include "ItemEldaMixRWndProc.h"

CItemEldaMixAWndProc	g_ItemEldaMixAWndProc;


//CFontg*				l_Fontg;	
//extern CFontman*	Fontman ;
extern	long	g_nowTime;

const int ELDA_STONE_1 = 6201;	//1~10
const int ELDA_STONE_10 = 6601;	//11~20


/*
1. 색깔 구분 
- 원석 레벨 1lv -> 1lv.....
- Class 14 EldaStone 스탯이 없는것은 기본 엘다로 판단 스탯을 색깔 구분하고 , Grade 로 레벨을 결정한다..
- enum 값을 지정한다. 
- 마우스 우클릭 누르면 자동적으로 엘다혼합 창에 들어감
- 특수한 아이템 서버에서 능력치를 준다 ? 

*/

/*
ELDA_RAW = 0 ,	//원석 
ELDA_GREEN,		//녹색 
ELDA_BLUE,		//푸른색
ELDA_YELLOW,	//노란색 
ELDA_RED,		// 빨강
ELDA_PURPLE,	//보라 
ELDA_VIOLETE,	//자주색 
ELDA_NAVY,		//남색
ELDA_SILVER,	//은색 
ELDA_YELLOW_GREEN,	//연두색 
ELDA_ORANGE,	//주황 
*/

CItemEldaMixAWndProc::CItemEldaMixAWndProc()
{
	m_eCitizen_Item = MILLENA_WEPON;

	m_List.clear();
	m_iWindowState = 0 ;
	m_IsViewchargeWindow = false; 

	int Code = 0 ; 
	int i =0 , j = 0 ;	
	//보석 총 11 개
	//엘다스톤은 총 20개 이다 
	sDataEldaStone DataElda;
	
	for(i = 0 ; i < 11 ; ++i )
	{
		for( int j=0 ; j < 20 ; ++j)
		{
			if(j < 10) 
			{
				Code = ELDA_STONE_1 + (i*10)+j;
			}
			else 
			{
				Code = ELDA_STONE_10 + (i*10)+(j-10);
			}
			
			DataElda.Code = Code;
			DataElda.Color = i;
			DataElda.Grade = j+1;
			
			m_List.push_back(DataElda);
		}
	}

}
CItemEldaMixAWndProc::~CItemEldaMixAWndProc()
{
	m_List.clear();
}
BOOL CItemEldaMixAWndProc::CreateLoadTable()
{
	
	///-- File Size Check
	char FileName[128] = "";
	
	sprintf( FileName, "MixElda.dat");
	
	///-- File Open
	VFileHandle* pFH = g_VFEtc.OpenFile( FileName );
	if( pFH == NULL ) 
	{
		MessageBox( NULL, _T("!MixElda.dat File Not Found!!!"), _T("알림"), MB_OK );
		return false;
	}
	
	int li_file_size = pFH->GetSize();
	int iEldaItemInofSize = sizeof( _SEldaItemInfo );

	///-- 사이즈 정규성 Check
	if( li_file_size % iEldaItemInofSize != 0 )
	{
		return( false );
	}
	
	///-- Set Count
	int iTableCount  = li_file_size / sizeof( _SEldaItemInfo );
	
	///-- read data
	/*
	EldaItemInfo = SAFE_NEW_ARRAY( _SEldaItemInfo , m_iTableCount );	
	if( EldaItemInfo == NULL )
	{
		return FALSE;
	}
	*/
	int i =0 ;
	for( i = 0; i < iTableCount; ++i )
	{
	
		///-- Read Header
		_SEldaItemInfo EldaItemInfo;
		pFH->Read( ( void * )&EldaItemInfo, sizeof( _SEldaItemInfo ));	
		m_EldaCostList.push_back(EldaItemInfo);
	}
	
	g_VFEtc.CloseFile( pFH );
	
	

	
	return TRUE ;
}

//-----------------------------------------------------------------------------
void CItemEldaMixAWndProc::Init()
{
	//초기화 
	int i =0 ; 
	CreateLoadTable();
	int TempEldaInfo[4][6] = {
		ELDA_RAW , ELDA_RED , ELDA_ORANGE ,ELDA_GREEN , ELDA_NAVY ,  ELDA_SILVER ,		//밀레나 웨폰 
		ELDA_RAW , ELDA_YELLOW_GREEN , ELDA_VIOLETE ,ELDA_GREEN , ELDA_NAVY ,  ELDA_SILVER , // 아머 
		ELDA_RAW , ELDA_BLUE , ELDA_YELLOW ,ELDA_GREEN , ELDA_PURPLE ,  ELDA_SILVER ,	//레인 웨폰 
		ELDA_RAW , ELDA_YELLOW_GREEN , ELDA_VIOLETE ,ELDA_GREEN , ELDA_PURPLE ,  ELDA_SILVER //밀레나 아머 
	};	

	memcpy( m_BaseEldaInfo , TempEldaInfo, sizeof(int)*24);
	


	//////////////////////////////////////////////////////////////////////////
	// 테스트 
	//////////////////////////////////////////////////////////////////////////
	for(i=0; i < c_MiX_ELDA_ITEM_NUM ; ++i)
	{
		m_CurSlot[i]->PcItem->Clear();
		m_PreSlot[i]->PcItem->Clear();
		
	}

	for(i=0 ; i < c_MAX_MIXELDA_PAY; ++i)
	{
		mcp_Chargeitem_slot[i]->PcItem->Clear();
	}




	//미리보는 아이템에 0번째 슬롯 에 엘다 Lv1 짜리를 보여준다. 
	SetAddPcItem(m_PreSlot[0] , 6201 , 0 )	;


	start_btn->SetBtnHold( true );
	cancel_btn->SetBtnHold( false );
	
	mb_button_lock = false;	
	
}
int CItemEldaMixAWndProc::GetItemCode(int Grade , int Color)
{
	std::vector <sDataEldaStone>::iterator i = m_List.begin();
	
	for( ; i != m_List.end() ; ++i )
	{
		if( i->Grade  == Grade &&
			i->Color == Color)
		{
			return i->Code;
		}
	}
	return NULL;
}

int CItemEldaMixAWndProc::GetMakeCost(int Grade)
{
	std::vector <_SEldaItemInfo>::iterator i = m_EldaCostList.begin();
	
	for( ; i != m_EldaCostList.end() ; ++i )
	{
		if( i->Grade  == Grade )
		{
			return i->NeedMoney;
		}
	}
	return NULL;
}



BOOL CItemEldaMixAWndProc::SetAddPcItem(CSlot *Slot , int Code, BOOL flag)
{
	SItemBaseInfo* item_Info = NULL;
	int Bundle = 1;
	//생성아이템 
	// flag == false 미리보기 아이템 true 결과아이템 

	item_Info = g_Pc_Manager.GetItemBaseInfoPtr(Code);
		
	if(item_Info == NULL)
	{
		return false;
	}
		
	//미리보기 아이템 생성한다.. 
	if(!flag)
	{
		Slot->theSlotState = n_Slot_Disable;
	}
	
	Slot->PcItem->Class = g_Pc_Manager.GetItemClsss( Code  );
	Slot->PcItem->ItemUID = 1;				
	Slot->PcItem->IType	= n_ITYPE_UNUSE;
	if( g_Pc_Manager.GetItemClsss( Code ) == n_Class_Use )
	{
		Slot->PcItem->IType	= n_ITYPE_USE;//소모품
	}
	else if( g_Pc_Manager.GetItemClsss( Code) == n_Class_Useless )
	{
		Slot->PcItem->IType	= n_ITYPE_UNUSE;//비소모품
	}
	else if( g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Special || 
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Eldastone ||
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Weapon_StrengthStone ||
		g_Pc_Manager.GetItemClsss( Code  ) == n_Class_Armor_StrengthStone)
	{
		Slot->PcItem->IType	= n_ITYPE_SPECIAL;
	}				
	Slot->PcItem->Amount = Bundle;
	Slot->PcItem->Code	= Code ;			
	Slot->PcItem->Tid	= (TEXID)g_Pc_Manager.GetItemTID(  Code   );
	Slot->PcItem->ItemTable	= item_Info;		
				
	return true;
				
}


void CItemEldaMixAWndProc::Open(int State)
{
	//무기
	m_iWindowState = State;
	
	bool bNpcVisible = false ;
	//엔피시 창이 떠있냐 .. 체크 한다 .. 없으면 실행 시키지마 ... 
	CWnd *Wnd = NULL;
	Wnd = Container->Get( WID_NpcChatWnd );
	
	if( Wnd != NULL && Wnd->IsVisible() == false )
	{
		return;
	}


	if(State == 0)
	{
		m_pFrameWndBase->SetCaption(G_STRING_CHAR(IDS_WND_WEPON_MIXELDASTONE));		
		if(nRui->thePcParam.Citizen == MILLENA )
		{
			m_eCitizen_Item = MILLENA_WEPON;
		}
		else 
		{
			m_eCitizen_Item = RAIN_WEPON;					
		}

	}
	//방어구
	else 
	{
		m_pFrameWndBase->SetCaption(G_STRING_CHAR(IDS_WND_DEFENSE_MIXELDASTONE));
		if(nRui->thePcParam.Citizen == MILLENA )
		{
			m_eCitizen_Item = MILLENA_ARMOR;
		}
		else 
		{
			m_eCitizen_Item = RAIN_ARMOR;					
		}
	}
	
	m_pFrameWndBase->Reposition(355 , 355);

	for(int i=0; i < c_MiX_ELDA_ITEM_NUM ; ++i)
	{
		m_CurSlot[i]->PcItem->Clear();
		m_PreSlot[i]->PcItem->Clear();
	}

	for(short i=0 ; i < c_MAX_MIXELDA_PAY; ++i)
	{
		mcp_Chargeitem_slot[i]->PcItem->Clear();
	}

	//미리보는 아이템에 0번째 슬롯 에 엘다 Lv1 짜리를 보여준다. 
	SetAddPcItem(m_PreSlot[0] , 6201 , 0 )	;

	//미리보는 아이템에 0번째 슬롯 에 엘다 Lv1 짜리를 보여준다. 
	
	start_btn->SetBtnHold( true );
	cancel_btn->SetBtnHold( false );
	
	m_MoneyText->SetText( G_STRING_CHAR(IDS_WND_EXPENSE) );	
	
	
	if(g_ItemEldaMixRWndProc.IsVisible)
	{
		nRui->SendEvent( g_ItemEldaMixRWndProc.FrameWnd->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}

	nRui->SendEvent( mpCharge_framewnd->GetID() , n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	nRui->SendEvent( m_pFrameWndBase->GetID() , n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	
	m_IsViewchargeWindow = false; 
	mb_button_lock = false;	
	
}
///---------------------------------------------------------------------------
///-- CloseEnchantWnd
///---------------------------------------------------------------------------
///-- 개조 창을 닫는다.
///-- 각 Resource 를 해제한다. ( Dynamic Texture_M 와 연동되어야 함 ) 
void CItemEldaMixAWndProc::Close()
{
	if( m_pFrameWndBase != NULL)
	{
		CloseCharge_Wnd();
		nRui->SendEvent( m_pFrameWndBase->GetID(), n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
	///TID_ELD_WND_UpGrade 해제
}
///---------------------------------------------------------------------------
///-- Composition
///---------------------------------------------------------------------------
void CItemEldaMixAWndProc::Composition()
{
	CImageBox*		img;
	CButton*		btn = NULL;
	CTextBox*		tbox = NULL;

	
	int center_x	= nRui->Desktop.Width / 2;
	int center_y	= nRui->Desktop.Height / 2;


	SRect mixEldaWnd;
	mixEldaWnd.w	= 348;
	mixEldaWnd.h	= 192+ 30;
	mixEldaWnd.x	= center_x - ( mixEldaWnd.w / 2 );
	mixEldaWnd.y	= center_y - ( mixEldaWnd.h / 2 );

	m_pFrameWndBase = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, WID_None, mixEldaWnd.x, mixEldaWnd.y, mixEldaWnd.w, mixEldaWnd.h, false );	
	m_pFrameWndBase->SetWndTexStyle( fwfTex_Style_Tile9 );
	m_pFrameWndBase->SetClientImage( TID_CTRL_WND_L1 );
	m_pFrameWndBase->SetIconTID( TID_CTRL_ICON_INVEN );	
	m_pFrameWndBase->SetWndTileSizeLR( 40, 29 );
	m_pFrameWndBase->SetWndTileSizeTB( 31, 11 );
	
	m_pFrameWndBase->SetCaption(G_STRING_CHAR(IDS_WND_ELDASTONE_MIX_TITLE));	
	m_pFrameWndBase->SetTitle( true );	
	m_pFrameWndBase->SetWndProc( this );
	m_pFrameWndBase->SetVisible(false);
	m_pFrameWndBase->Initialize();

	///-- Frame Wnd
	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_None, m_pFrameWndBase->GetID() , 0 ,  0 , mixEldaWnd.w, mixEldaWnd.h, false );	
	m_pFrameWnd->SetClientImage( TID_None );
	m_pFrameWnd->SetTitle( false );	
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetMove( false );
//	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->Initialize();

	///---------------------------------------------------------------------------	
	///-- Static
	///---------------------------------------------------------------------------		
	///-- Line
	int iAllHeght = 23;

	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), 1, 116+iAllHeght, mixEldaWnd.w-2, 1, false );
	img ->SetClientImage( TID_CTRL_LINEDOT );
	 
	int sx = 37 , sy = 27;
 
	for(int i =0 ; i < 6 ;  ++i)
	{
		
		m_PreSlot[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrameWnd->GetID() , sx+(i*48), 30+iAllHeght, 35, 35, false );	
		m_PreSlot[i]->SetSlotGroup( n_SGROUP_ELDA_MIX_PRE0 );
		m_PreSlot[i]->SetHID( HID_ITEMTIP );
		m_PreSlot[i]->SetSlotItem(&m_PrePcItem[i]);
		
		img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, m_pFrameWnd->GetID(), sx+(i*48), 27+35+5+iAllHeght, 35, 35, false );       
		img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
		m_CurSlot[i] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None , m_pFrameWnd->GetID() , sx+(i*48), 27+35+5+iAllHeght, 35, 35, false );	
		m_CurSlot[i]->SetSlotGroup( n_SGROUP_SKILL_POTION );
		m_CurSlot[i]->SetHID( HID_ITEMTIP );
		m_CurSlot[i]->SetSlotItem( &m_CurPcItem[i]);
	
	}

	m_MoneyText = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, m_pFrameWnd->GetID(), 1,  26 , 338, 22, false );
	m_MoneyText->SetFontR( n_fRsvFontWndStatic );	
	m_MoneyText->SetAlignText( n_atRight, n_atCenter );
	m_MoneyText->SetText( G_STRING_CHAR(IDS_WND_EXPENSE) );	
	
	///-- 유료 아이템 버튼
	mcp_chargeWindow_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID() , 
		131 , 185 , 87, 25, false );
	mcp_chargeWindow_btn->SetBtnImage( TID_CTRL_BTN87, TID_CTRL_BTN87_OVR, TID_CTRL_BTN87_CLK, TID_None );			
	mcp_chargeWindow_btn->SetFontR( n_fRsvFontWndButton ); 
	mcp_chargeWindow_btn->SetText( G_STRING_CHAR( IDS_CHARGE_ITEM ) );										
	//mcp_chargeWindow_btn->SetVisible(false);
		
	///-- 유료 아이템 등록 윈도우
	mpCharge_framewnd = (CFrameWnd *)nRui->CreateWnd( n_wtFrameWnd, WID_None, m_pFrameWndBase->GetID() , 
		0, 30, mixEldaWnd.w , 70, false );
	mpCharge_framewnd->SetWndTexStyle( fwfTex_Style_Tile9 );
	mpCharge_framewnd->SetClientImage( TID_CTRL_WND_NL1 );
	mpCharge_framewnd->SetWndTileSizeLR( 5, 5 );
	mpCharge_framewnd->SetWndTileSizeTB( 5, 5 );	
	mpCharge_framewnd->SetTitle( false );
	mpCharge_framewnd->SetWndProc( this );
	mpCharge_framewnd->SetVisible( false );
	mpCharge_framewnd->SetMove(false);
	mpCharge_framewnd->Initialize();
	
	///-- Slot
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpCharge_framewnd->GetID(), 
		120 , 14, 35, 35, false );       
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpCharge_framewnd->GetID(), 
		160, 14, 35, 35, false );      
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
	
	img = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, mpCharge_framewnd->GetID(), 
		200, 14, 35, 35, false );     
	img ->SetClientImage( TID_CTRL_SLOT_SUNKEN );
	
	
	mcp_Chargeitem_slot[0] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
		mpCharge_framewnd->GetID(), 120, 14, 35, 35, false );
	mcp_Chargeitem_slot[0]->PcItem = &(mc_Chargeslot_item[0]);
	mcp_Chargeitem_slot[0]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[0]->SetHID( HID_ITEMTIP );
	
	mcp_Chargeitem_slot[1] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
		mpCharge_framewnd->GetID(), 160, 14, 35, 35, false );
	mcp_Chargeitem_slot[1]->PcItem = &(mc_Chargeslot_item[1]);
	mcp_Chargeitem_slot[1]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[1]->SetHID( HID_ITEMTIP );
	
	mcp_Chargeitem_slot[2] = (CSlot*)nRui->CreateWnd( n_wtSlot, WID_None, 
		mpCharge_framewnd->GetID(), 200, 14, 35, 35, false );
	mcp_Chargeitem_slot[2]->PcItem = &(mc_Chargeslot_item[2]);
	mcp_Chargeitem_slot[2]->SetSlotGroup( n_SGROUP_TUNING );
	mcp_Chargeitem_slot[2]->SetHID( HID_ITEMTIP );
	
	CloseCharge_Wnd();	



	///---------------------------------------------------------------------------	
	///-- Control
	///---------------------------------------------------------------------------	
	///-- 시작 버튼
	
	start_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 101, 152, 58, 25, false );
	start_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );
	start_btn->SetFontR( n_fRsvFontWndButton ); 
	start_btn->SetText( G_STRING_CHAR( IDS_WND_COM_START ) ); 
	start_btn->SetBtnHold( true );

	///-- 취소
	cancel_btn = (CButton*)nRui->CreateWnd( n_wtButton, WID_None, m_pFrameWnd->GetID(), 192, 152, 58, 25, false );
	cancel_btn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_CTRL_BTN58_DIS );	
	cancel_btn->SetFontR( n_fRsvFontWndButton ); 
	cancel_btn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ) ); 
	
	///-- None Item 지정



	Init();
}


void CItemEldaMixAWndProc::OpenCharge_Wnd()
{
	SRect WholeFrameRect;
	SRect C_framewndRect;
	SRect Charge_frameRect;
	
	m_pFrameWnd->GetClientSize(&C_framewndRect);
	mpCharge_framewnd->GetClientSize(&Charge_frameRect);
	
	WholeFrameRect = C_framewndRect;
	WholeFrameRect.h += (Charge_frameRect.h-2);  
	
	m_pFrameWndBase->ResetSize(&WholeFrameRect);
	//m_pFrameWnd->SetWndRepositionR(0, 0);
	mpCharge_framewnd->SetWndRepositionR(0, C_framewndRect.h -2);

	nRui->SendEvent( mpCharge_framewnd->GetID(), n_emSetVisible, 
			(EPARAM)true, 0, 0, 0 );
}


void CItemEldaMixAWndProc::CloseCharge_Wnd()
{
	SRect WholeFrameRect;
	SRect C_framewndRect;
	
	m_pFrameWnd->GetClientSize(&C_framewndRect);
	WholeFrameRect = C_framewndRect;
	
	m_pFrameWndBase->ResetSize(&WholeFrameRect);
	//m_pFrameWnd->SetWndRepositionR(0, 0);
	
	//프레임 박스 재갱신 
	m_pFrameWndBase->SetWndPositionA(WholeFrameRect.x , WholeFrameRect.y);

	nRui->SendEvent( mpCharge_framewnd->GetID(), n_emSetVisible, 
		(EPARAM)false, 0, 0, 0 );
	
	
	mcp_Chargeitem_slot[0]->PcItem->Clear();
	mcp_Chargeitem_slot[1]->PcItem->Clear();
	mcp_Chargeitem_slot[2]->PcItem->Clear();
	

}

void CItemEldaMixAWndProc::ClearPreItem()
{
	int i=0 ;
	for(i=0 ; i < c_MiX_ELDA_ITEM_NUM; ++i)
	{
		m_PreSlot[i]->PcItem->Clear();
		m_CurSlot[i]->PcItem->Clear();
	}

	for(i=0 ; i < c_MAX_MIXELDA_PAY; ++i)
	{
		mcp_Chargeitem_slot[i]->PcItem->Clear();
	}

}

///---------------------------------------------------------------------------
///-- Update
///---------------------------------------------------------------------------
///-- 각 엘다 스톤에 따른 Upgrade 결과를 보여준다.
void CItemEldaMixAWndProc::Update()
{
	///-- CheckItemList - Inven 변경에 따른 인첸트 창의 변경
	CheckItemList();
	
	 
}




///---------------------------------------------------------------------------
///-- CheckItemList - Inven 변경에 따른 인첸트 창의 변경
///---------------------------------------------------------------------------
BOOL CItemEldaMixAWndProc::CheckItemList()
{
	int i = 0;
	int j = 0;
	
	bool lb_isFind_item = FALSE;
	bool lb_isFind_elda = FALSE;
	BOOL lb_isFind_Chargeitem[3] = { FALSE, FALSE, FALSE };	

	bool b_isFind_elda[c_MiX_ELDA_ITEM_NUM] ;
	for(i = 0 ; i < c_MiX_ELDA_ITEM_NUM ; ++i)
	{
		b_isFind_elda[i] = FALSE;
	}
	

	for(i = 0 ; i < c_MiX_ELDA_ITEM_NUM ; ++i )
	{
		if( !m_CurSlot[i]->PcItem)
		{
			return FALSE;
		}
	}


	///-----------------------------------------------------------------------
	///-- 전체 인벤토리 Access
	for( j = 0; j < c_MaxInvenItemNum; ++j )
	{	
		if( nRui->GetPcInvenInfo()->InvenItem[j] == NULL )
		{
			continue;
		}
		
		///-- Elda slot
		if( m_CurSlot[0]->PcItem && 
			m_CurSlot[0]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			b_isFind_elda[0] = true;
		}
		///-- Elda slot
		if( m_CurSlot[1]->PcItem && 
			m_CurSlot[1]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			b_isFind_elda[1] = true;
		}
		///-- Elda slot
		if( m_CurSlot[2]->PcItem && 
			m_CurSlot[2]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			b_isFind_elda[2] = true;
		}
		///-- Elda slot
		if( m_CurSlot[3]->PcItem && 
			m_CurSlot[3]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			b_isFind_elda[3] = true;
		}
		///-- Elda slot
		if( m_CurSlot[4]->PcItem && 
			m_CurSlot[4]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			b_isFind_elda[4] = true;
		}
		///-- Elda slot
		if( m_CurSlot[5]->PcItem && 
			m_CurSlot[5]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
		{
			b_isFind_elda[5] = true;
		}
		
		//유료 아이템 
		///-- 유료 아이템 slot
		for( i = 0; i < 3; i++)
		{
			///-- item slot
			if( mcp_Chargeitem_slot[i]->PcItem && 
				mcp_Chargeitem_slot[i]->PcItem->ItemUID == nRui->GetPcInvenInfo()->InvenItem[j]->ItemUID )
			{
				lb_isFind_Chargeitem[i] = TRUE;
				
				///-- 수량 체크
				if( mcp_Chargeitem_slot[i]->PcItem->Amount > nRui->GetPcInvenInfo()->InvenItem[j]->Amount )
				{
					mcp_Chargeitem_slot[i]->PcItem->Clear();
				}
			}
		}



	}

	///-----------------------------------------------------------------------
	///-- item 을 찾지 못 했을 경우
	for(i = 0 ; i <c_MiX_ELDA_ITEM_NUM  ; ++i)
	{	
		if(!b_isFind_elda[i])
			m_CurSlot[i]->PcItem->Clear();
	}

	///------------------------------------------------------------------------
	///-- 유료 item 을 찾지 못 했을 경우
	///-- Item Check
	for( i = 0; i < 3; i++)
	{
		///-- item slot
		if( !lb_isFind_Chargeitem[i] )
		{
			mcp_Chargeitem_slot[i]->PcItem->Clear();
		}
	}


	///-----------------------------------------------------------------------
	///-- 두 Slot 에 모두 Item 이 있다면 시작 준비
	if( m_CurSlot[0]->PcItem->IType != n_ITYPE_NONE && 
		m_CurSlot[1]->PcItem->IType != n_ITYPE_NONE && 
		m_CurSlot[2]->PcItem->IType != n_ITYPE_NONE && 
		m_CurSlot[3]->PcItem->IType != n_ITYPE_NONE && 
		m_CurSlot[4]->PcItem->IType != n_ITYPE_NONE &&
 		m_CurSlot[5]->PcItem->IType != n_ITYPE_NONE )
	{
		start_btn->SetBtnHold( false );	

	}
	else
	{
		start_btn->SetBtnHold( true );
		return FALSE;
	}

	return TRUE;
	
}


//-----------------------------------------------------------------------------
void CItemEldaMixAWndProc::Proc( SEventMessage* EMsg )
{
#ifdef MIX_ELDA_ITEM_NUM 
	if(EMsg->FocusWnd == cancel_btn->GetID())
	{
		if(EMsg->Notify == NM_BTN_CLICK)
		{
			nRui->SendEvent(m_pFrameWndBase->GetID() , n_emSetVisible , (EPARAM)false , 0 , 0 , 0 );
	
		}
	}

	for(int i =0 ; i < c_MiX_ELDA_ITEM_NUM ; ++i)
	{
		if(EMsg->FocusWnd == m_CurSlot[i]->GetID() )
		{
			if( EMsg->Notify == NM_SLOT_CLICK)
			{
				ItemSlotClick(EMsg);
			}
			if(EMsg->Notify == NM_SLOT_DBCLICK)
			{
				ItemSlotDBClick(EMsg);
			}
		}
	}

	for(short i =0 ; i < RP_MAX_MIXELDA_PAY ; ++i)
	{
		if(EMsg->FocusWnd == mcp_Chargeitem_slot[i]->GetID() )
		{
			if( EMsg->Notify == NM_SLOT_CLICK)
			{
				ItemSlotClick(EMsg);
			}
			if(EMsg->Notify == NM_SLOT_DBCLICK)
			{
				ItemSlotDBClick(EMsg);
			}
		}
	}	





	//스타트 하면 버튼 비활성화 .. 
	if(EMsg->FocusWnd == start_btn->GetID() )
	{
		if(EMsg->Notify == NM_BTN_CLICK && !mb_button_lock )
		{
			//서버로 전송 
			SRpCsMixEldaStone TempMsg;
	
			for(int i=0; i < c_MiX_ELDA_ITEM_NUM; ++i )
			{
				TempMsg.theEldaStone[i] = m_CurPcItem[i].ItemUID ;
				
			}

		
			for(short i =0 ; i < RP_MAX_MIXELDA_PAY ; ++i)
			{
				TempMsg.thePayItem[i] = mc_Chargeslot_item[i].ItemUID;
			}



			TempMsg.theMixType = (BYTE)m_eCitizen_Item;
			g_RockClient.Send_CsSRpCsMixEldaStone(TempMsg);

			Close();
			mb_button_lock = true;
			//인벤토리를 disible 시킨다. 
			nRui->ResetCursorItem();
		
		}

	}
	else if(mcp_chargeWindow_btn->GetID() == EMsg->FocusWnd ) 
	{

		if(EMsg->Notify == NM_BTN_CLICK && !mb_button_lock )
		{
			m_IsViewchargeWindow ^= true; 

			if(m_IsViewchargeWindow)
			{
				OpenCharge_Wnd();
			}
			else 
			{
				CloseCharge_Wnd();
			}
			
		}

	}




#endif
}

///---------------------------------------------------------------------------
///-- ItemSlotDBClick
///---------------------------------------------------------------------------
///-- Item 제거
void CItemEldaMixAWndProc::ItemSlotDBClick( SEventMessage* EMsg )
{
	CSlot*		tslot	= NULL;	
	CSlot*		sslot	= NULL;
	
	tslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );				
	sslot = nRui->GetItemSelectedSlot();
	

	if( tslot == m_CurSlot[0] ) 
	{
		for(int i=0; i < c_MiX_ELDA_ITEM_NUM ; ++i)
		{
			m_CurSlot[i]->PcItem->Clear();
			m_PreSlot[i]->PcItem->Clear();
		}
	
		SetAddPcItem(m_PreSlot[0] , 6201 , 0 );

	}
	else if( tslot->PcItem )
	{
		tslot->PcItem->Clear();		
	}

	return;
}

//미리 보기 아이템을 처리해준다. 
void CItemEldaMixAWndProc::PrevItemList(CSlot *Slot)
{
	if( Slot ==  NULL )
		return;
	
	if( Slot->PcItem == NULL ) 
		return;

	//등급만 알면 된다...
	int iGrade = Slot->PcItem->ItemTable->theGrade/100;
	
	for(int i=0; i < c_MiX_ELDA_ITEM_NUM ; ++i) 
	{
		SetAddPcItem(m_PreSlot[i] , GetItemCode(iGrade ,m_BaseEldaInfo[m_eCitizen_Item][i] ));
	}
	
}

void CItemEldaMixAWndProc::ItemSlotClick( SEventMessage* EMsg )
{
	CSlot*		targetslot	= NULL;//현재 장비 슬롯
	CSlot*		srcslot		= NULL;//선택된(pick) 인벤 슬롯
	SPcItem*	tmpPcItem	= NULL;
	int			position	= 0;

	///-- 아이템을 들고 있을 경우
	if( nRui->GetCursorItem() != NULL )
	{
		
		targetslot = (CSlot*)Container->Get( (WNDID)EMsg->FocusWnd );
		srcslot = nRui->GetItemSelectedSlot();

		if( srcslot->PcItem == NULL )
		{
			return;
		}		
		
		if ( srcslot->Group != n_SGROUP_INVENTORY)
		{	
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_OUT_INVENTORY_MOVE));
			return;
		}
	
		if( targetslot->PcItem && (targetslot->PcItem == srcslot->PcItem) )
		{
			nRui->ResetCursorItem();
		}
	
		///-------------------------------------------------------------------
		///-- 
		if(targetslot->Group == n_SGROUP_SKILL_POTION )
		{			
			if( targetslot == m_CurSlot[0]) 
			{
				//첫번째 슬롯에 원석만 올려 놓을수 있다..
				if( srcslot->PcItem->ItemTable->theClass == n_Class_Eldastone )
				{
					if( srcslot->PcItem->ItemTable->theSetID == 0 && srcslot->PcItem->Code < 60000 )
					{
						//슬롯 초기화 
						for(int i=0; i < c_MiX_ELDA_ITEM_NUM ; ++i)
						{
							m_CurSlot[i]->PcItem->Clear();
							m_PreSlot[i]->PcItem->Clear();
						}

						for(short i=0 ; i < c_MAX_MIXELDA_PAY; ++i)
						{
							mcp_Chargeitem_slot[i]->PcItem->Clear();
						}

						PrevItemList(srcslot);
						///-- Copy item
						*targetslot->PcItem = *srcslot->PcItem ;
						targetslot->PcItem->Amount=  1;
						
						char StrCost[256] = {0,};
						char Strtmp[256] = {0,};
						int iCost = GetMakeCost(targetslot->PcItem->ItemTable->theGrade/ 100);
						InventoryWnd.SetEldText(StrCost, iCost);
						Rsprintf(RWCHAR(Strtmp) , _RT("%s : %s") , G_STRING_CHAR(IDS_WND_EXPENSE) , StrCost  );					
						m_MoneyText->SetText(Strtmp);// 합성 엘다스톤 비용
						nRui->ResetCursorItem();
					}
					else 
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ELDA_NOT_COLOR) );	
						return;
					}
				}
				else
				{
					SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_NOT_ELDA_ITEM));	
					return;
				}
				
				//CheckItemAdd(&m_PrePcItem[0] , &m_CurPcItem[0] ,srcslot->PcItem , targetslot->PcItem );
			}
			else 
			{
				for(int i =1 ; i <c_MiX_ELDA_ITEM_NUM; ++i )
				{
					if( targetslot == m_CurSlot[i] )
					{
						if ( m_PrePcItem[i].ItemTable == NULL ) 
						{
							SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_FIRST_UP_ELDASTON));						nRui->ResetCursorItem();
							return;
						}
						
						CheckItemAdd(&m_PrePcItem[i] ,&m_CurPcItem[i] , srcslot->PcItem , targetslot->PcItem );
					}
				}
			}


		}
		//유료 아이템 처리
		else 
		{	
			if(	targetslot  == mcp_Chargeitem_slot[0] || 
				targetslot  == mcp_Chargeitem_slot[1] || 
				targetslot  == mcp_Chargeitem_slot[2] )
			{
				
				int Scroll = srcslot->PcItem->ItemTable->theReHP;												

				//중복 아이템 음 리턴 
				if( ( mcp_Chargeitem_slot[0]->PcItem->IType != n_ITYPE_NONE && 
					mcp_Chargeitem_slot[0]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_Chargeitem_slot[1]->PcItem->IType != n_ITYPE_NONE && 
					mcp_Chargeitem_slot[1]->PcItem->Code == srcslot->PcItem->Code ) ||
					( mcp_Chargeitem_slot[2]->PcItem->IType != n_ITYPE_NONE && 
					mcp_Chargeitem_slot[2]->PcItem->Code == srcslot->PcItem->Code )  )
				{
					return;
				}
				
				//중복 아이템 음 리턴 
				if( ( mcp_Chargeitem_slot[0]->PcItem->IType != n_ITYPE_NONE && 
					mcp_Chargeitem_slot[0]->PcItem->ItemTable->theReHP == Scroll ) ||
					( mcp_Chargeitem_slot[1]->PcItem->IType != n_ITYPE_NONE && 
					mcp_Chargeitem_slot[1]->PcItem->ItemTable->theReHP == Scroll ) ||
					( mcp_Chargeitem_slot[2]->PcItem->IType != n_ITYPE_NONE && 
					mcp_Chargeitem_slot[2]->PcItem->ItemTable->theReHP == Scroll )  )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_OVERRAPEDITEMEFFECT  ) );
					return;
				}
  
				//전용 엘다 보험 이냐 ? 
				if( Scroll == n_Scroll_MixEldaInsurance ||
					Scroll == n_Scroll_MixEldaRateBonus ||
					Scroll == n_Scroll_MixEldaCriBonus )
				{
					//합성엘다 스톤 보험 같은 등급이냐 ?? 
					if(Scroll == n_Scroll_MixEldaInsurance )
					{
						
						if( m_CurSlot[0]->PcItem->IType != n_ITYPE_NONE && 
							(m_CurSlot[0]->PcItem->ItemTable->theGrade != srcslot->PcItem->ItemTable->theGrade )  )
						{
							SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NEED_SAMEGRADE_INSURANCE) );
							return;
						}

					}

					///-- Copy item
					targetslot->SetSlotPcItem( *(srcslot->PcItem) );								
					targetslot->PcItem->Amount = 1;
					
					///-- Del Cursor					
					nRui->ResetCursorItem();	
				}
				else 
				{
					// 그아이템은 올려 놓을수 없습니다. 
					nRui->ResetCursorItem();
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_USE_ITEM ) );
					return;
				}
			}
		}
	}
	
}
void CItemEldaMixAWndProc::GetEldStr(char *Str , DWORD Eld)
{
	//다시 계산한다.  
	
	char  CurNumber[256]={0,};
	char  NumberComma[256]={0,} ;
	
	int Count = 0 ; 

	_ultoa(Eld , Str , 10);

	int Len = strlen(Str);
	
	int FirstPoint =Len%3  ;
	if( FirstPoint == 0 )
	{
		FirstPoint = 3;
	}
	int i = 0 , j = 0;
	
	ZeroMemory(&CurNumber , sizeof(CurNumber));
	
	for(i =0 , j=0 ; i < 256 ; i++ , j++)
	{
		//3자릿수이상 넘으면 . 
		if( Str[i] != NULL && Count == FirstPoint )
		{
			FirstPoint = 3; 
			Count = 0; 
			CurNumber[j++] = ',';
		}
		
		CurNumber[j] = Str[i];
		
		if( Count > 3 )	Count = 0 ; 
		
		Count++;
		
		if(Str[i] == NULL)
		{

			ZeroMemory(Str , i + 2);
#ifdef UNICODE_ROCK
			DXUtil_ConvertAnsiStringToWide(RWCHAR(Str) , CurNumber , -1) ;
#else  
			SAFE_STR_CPY(Str , CurNumber , Rstrlen(CurNumber) + 1  );
#endif
					
			break; 
		}
		
	}	


}


	//미리보기아이템과 대조시킨다 
void CItemEldaMixAWndProc::CheckItemAdd(SPcItem* PrePcItem ,SPcItem* CurPcItem,SPcItem* SrcPcItem , SPcItem *TarGetPcItem)
{
	if(PrePcItem->ItemTable == NULL ||	CurPcItem == NULL || TarGetPcItem->ItemTable != NULL ||SrcPcItem == NULL)
		return;

	//같은 엘다 스톤이냐 그리고 같은 색갈이냐 
	if( (PrePcItem->ItemTable->theClass == SrcPcItem->ItemTable->theClass ))
	{
		if( (PrePcItem->ItemTable->theSetID == SrcPcItem->ItemTable->theSetID) )
		{
			if( SrcPcItem->Amount >= PrePcItem->Amount )
			{
				///-- Copy item
				*TarGetPcItem = *SrcPcItem ;
				TarGetPcItem->Amount = PrePcItem->Amount;
				nRui->ResetCursorItem();
			}
			else
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_NEED_ITEM_COUNT) );	
			}
		}
		else 
		{
			SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR(IDS_SYSPR_ELDA_NOT_COLOR) );	
		}
		
	}
	else
	{
		SystemPrint( n_DarkRed, n_SystemMsg,G_STRING_CHAR(IDS_SYSPR_NOT_ELDA_ITEM));	
	}

}

