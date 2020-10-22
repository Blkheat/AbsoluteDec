#include "RockPCH.h"

#include	<NETWORK\Protocol.h>
#include	"..\\quadlist.h"
#include	"ObjBase.H"
#include	"Player.H"
#include	"..\\Effect.h"
#include	"..\\PC.h"
#include	"..\\Camera.H"
#include	"FindWay.H"
#include	"..\..\CLoadingBarThread.h"
#include	"..\..\RockClient.h"
#include	<assert.H>


#include	"RockInterface\\WndProc.h"
#include	"RockInterface\\FrameWnd.h"
#include	"RockInterface\\RUI.h"
#include	"RockInterface\\NpcChatWnd.h"
#include	"RockInterface\\NonPlayerInfoWnd.h"
#include	"RockInterface\\MainMenuWndProc.h"
#include	"RockInterface\\LoadingWndProc.h"
#include	"RockInterface\\ExchangeWndProc.h"
#include	"RockInterface\Announcement.h"
#include	"RockInterface\AliveWnd.h"
#include	"RockInterface\BattleZoneWndProc.h"

#include	"..\\effect\CEffectManager.h"
#include	"stringmanager.h"
#include "..\\..\\GTimer.h"

///-- 클라이언트에서 서버로 보낸느 어택관련 패킷 최소값 조정

// 기타
extern		long				g_nowTime;
extern		int					g_chr_anicount;
extern		int					g_Shadow;
extern		BOOL			    g_LogPrint;

extern		DWORD	FloatToDWORD( float f );
extern		int		GetMax( int a, int b );

// 길찾기
extern		void	endpath( void );
extern		short	initpath( void );
extern		bool	GotoPath( short x1, short y1, short x2, short y2, short *x, short *y );

extern D3DXVECTOR3			g_AniVertex[3000];	// 망토 에니시 저장하는 변수 ( 그림자 처리 ) By wxywxy


// 상수
#define		SEND_MOVEFRAME				5		// 서버에 이동메세지보내는 프레임
#define		UPDATECHECK_TIME		2000.0f		// 유저, NPC 검색시간
#define		MOVE_LATENCY_TIME		  10.0f		// 동기화타임 (0.01초)
#define		ATTACKWAIT_FRAME			2		// 공격딜레이타임( ATTACKWAIT_FRAME * ANI_TIME )

///--JAPAN_BUG_MODIFY
#define		CHECK_CSMOVETIME		250	

extern bool AniDemo;	/// 임시. 데모 
extern int Ani_Type; 

#define  MYSHADOW g_Map.m_ShadowList[m_uShadowSizeType]

void Player::Clear()
{			
	m_aLastAttackSendTime = (DWORD)g_nowTime;
	m_aMinAttackDelayTime = 800;

	theTAtSave_Event.Clear();
	
	m_bIsVisible = TRUE;
	m_AttackType = 0;
	m_AttackStep = 1;	
	m_bWait		 = false;	
	m_bLatency	 = false;
	m_bSendMove	 = true;
	
	m_OneTimeJump = false;
	
	//-----------------------------------------------------------------------------
	// Desc: 캐릭터 메터리얼 초기화
	//
	// 머리카락은 고유 색깔이 있기 때문에 초기화 시키면 안된다
	// 나중에 아이템마다 색깔 정보가 들어가게되면 참조해서 처리하면된다
	//
	// Copyright (c) 2003.11.3 By KStaR79
	//-----------------------------------------------------------------------------	
	
	int nItmCount = 0;
	
	nItmCount = g_Pc_Manager.GetItmCount( this );
	
	for( int i = 0; i < nItmCount; ++i )
	{
		//		switch( i )
		//		{
		//		case HAIR_INDEX:	// 머리카락
		//			break;
		//		default:
		//			{
		m_pMaterial[i].Diffuse.r = 1.0f;
		m_pMaterial[i].Diffuse.g = 1.0f;
		m_pMaterial[i].Diffuse.b = 1.0f;
		m_pMaterial[i].Diffuse.a = 1.0f;
		//			}
		//			break;
		//		}		
	}
	
	theCombo_State = n_Combo_Enable;
	
	theWarpTime = 0;
	
	theIsAttack = true;
	theAttackDelayTime = g_nowTime;
	
	ZeroMemory( theAddEffect_SkillCode, sizeof( WORD ) * RP_MAX_KEEPUP_SKILL );
	ZeroMemory( theAddEffect_State, sizeof( BYTE ) * MAX_ADDEFFECT_NUM );
	
	m_CharAlpha.Clear();
	
	
	theMapType = n_NormalZone;
	theFightZoneReady = true;
	m_nViewRefDestIndex = -1;	
	m_KeepUpChecker.Clear();
	
	IsMyPlayerRender = TRUE;

	
	m_Char_WearType = CHAR_WEAR_NORMAL;
	
	m_IsCDataQActed = TRUE;
	
	///--JAPAN_BUG_MODIFY
	m_PreSendCsMoveTime = timeGetTime();

	///-- 중국 PK 관련 모드 작업
	m_PKMode = n_PKOFFMode;
	ClearAllMyAttackerList();

	m_ComBoAttackList.clear();
	
	return;
}


void Player::Destroy()
{		
	Clear();
	Free();	
	endpath();
}

//-----------------------------------------------------------------------------
// Desc: PC파일읽기
//-----------------------------------------------------------------------------
void Player::CreatePlayer( BYTE _byType )
{
	theActionSystem.InitData();
	theActionSystem.ReadComboSeqeunce( g_RockClient.GetRootDir() );
	
	if( theFindWay == NULL )
	{
		theFindWay = SAFE_NEW( CFindWay );		
		theFindWay->m_pRefCharacter = (void *)this;
	}
	
	//-----------------------------------------------------------------------------
	// 유저변수 메모리할당
	//-----------------------------------------------------------------------------
	if( m_pUserInfo == NULL )
		m_pUserInfo = SAFE_NEW( USER_INFO );		
	
	//-----------------------------------------------------------------------------
	// 캐릭터정보초기화
	//-----------------------------------------------------------------------------
	//	int	nItmCount = g_Pc_Manager.GetItmCount( this );	/// .chr 파일을 이용
	//	int	nItmCount = c_Max_WearItemType;		/// 눈에 보여줘야 될 itm 갯수 (헤어, 상의, 하의, 장갑, 부츠, 서브장비)
	/// 헤어와 얼굴은 서버에서 받은 정보로...change...
	
	for( int i = 0; i < c_Max_WearItemType; ++i )
	{			 
		if( n_Wear_Serve == i )
		{
			m_pItm_index[i]	= -1;			
			//			continue;
		}
		else
		{
			// 아이템 할당
			m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( this, i );
		}
		
		// 메테리얼 할당
		m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( this, i );
		
		// 버텍스버퍼 할당
		m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount(this, i) );
		
		// 기본 의상 
		m_Basic_DrawItm[i] = 0;
		//		m_Basic_Itm_index[i] = m_pItm_index[i];
		m_Basic_Material[i] = m_pMaterial[i];
		//		m_Basic_VtxBuffer[i].CreateBuffer( g_Pc_Manager.GetBasicVtxCount(this, i) );		
	}
	// 스케일
	m_vScale = g_Pc_Manager.GetChrScale( this );
	
	//-----------------------------------------------------------------------------
	// 캐릭터 생성
	//-----------------------------------------------------------------------------
	switch( _byType )
	{
	case 0:	// 게임에 들어간다
		{
			// 무기 링크되는 본위치 저장
			m_pUserInfo->ArmorInfo.RHand = g_Pc_Manager.m_PcAttr[g_Pc.m_MainPC.nIndex].r_hand_node;
			m_pUserInfo->ArmorInfo.LHand = g_Pc_Manager.m_PcAttr[g_Pc.m_MainPC.nIndex].l_hand_node;
			
			// 아이템정보저장
			//			SetWearItem();		
		}
		break;
	case 1:	// 캐릭터 선택화면
		{
			// 무기 링크되는 본위치 저장
			// 아이템정보저장
			SetTempItem();
		}
		break;
	case 2:	// 캐릭터 생성창
		{
			// 아이템정보저장
			SetTempItem();
		}
		break;
	}
	
	// 기본애니메이션 
	AnimationPlay( n_Stand_Peace9 );	
	
	// 현재시간저장
	m_PreTime	= g_nowTime;
	m_PreMTime = g_nowTime;
	m_SimTime	= g_nowTime;	
	
	m_State.Clear();
}


void Player::ReSetWearItem()
{
	for( int i = 0; i < c_Max_WearItemType ; ++i )
	{
		if( n_Wear_Serve == i )
		{
			m_pItm_index[i]	= -1;			
			//			continue;
		}
		else
		{		
			// 아이템, 메터리얼
			m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( this, i );
		}
		
		m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( this, i );
		
		// 버텍스버퍼 할당		
		m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount(this, i) );
		
		if( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
		{
			// 기본 의상 
			m_Basic_DrawItm[i] = 0;	
			m_Basic_Material[i] = m_pMaterial[i];	
		}
	}

	SetWearItem();
}

//-----------------------------------------------------------------------------
// 게임 캐릭터 생성
//-----------------------------------------------------------------------------
void Player::SetWearItem()
{	
	// 이전에 있던 링크 이펙트를 모두 지워 Boa 요 By wxywxy
	g_Particle.m_EffectManager.DeleteLink( ( Character * ) this );
	g_Particle.m_EffectManager.DeleteWeaponEnChantEffect( ( Character * ) this );
	
	SItemBaseInfo* item_Info = NULL;
	int chrAttrIndex = GetAttrIndex();
	
	bool IsEquipWeapon = false;
	
	// nRui->PcParam.theWearItem[10] 아이템 코드에 따라서 보여줄 7가지 m_pItm_Index를 세팅한다 
	bool IsSkinSuit = false;
	bool IsSkinHelmel = false;
	bool IsSKinserve = false ;


	if( nRui->thePcInven.WearItem[n_WearSlot_SKIN_Suit] )
	{
		
		m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_SKIN_Suit]->Code );
		SetItemBuffer( n_Wear_CoatArmor );
		
		///-- 셋트 아이템 웨어처리
		SItemBaseInfo* pSItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_SKIN_Suit]->Code );
		if( pSItemBaseInfo &&  n_Type_Skin_Suit == pSItemBaseInfo->theType)
		{
			m_Char_WearType = CHAR_WEAR_SETITEM;
			
			//방어구 인챈시 이펙트 .. 달기 .. 
			g_Particle.m_EffectManager.CreateDefenceEnChantEffect(this , 255 );
			
		}
		else
		{
			m_Char_WearType = CHAR_WEAR_NORMAL;	
		}
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_SKIN_Suit]->Code , ( Character *) this );
	}
	else 

	if( nRui->thePcInven.WearItem[n_WearSlot_CoatArmor] )
	{
		m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_CoatArmor]->Code );
		SetItemBuffer( n_Wear_CoatArmor );
		
		///-- 셋트 아이템 웨어처리
		SItemBaseInfo* pSItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_CoatArmor]->Code );
		if( pSItemBaseInfo &&  n_Type_SetCloth == pSItemBaseInfo->theType)
		{
			m_Char_WearType = CHAR_WEAR_SETITEM;
			
			//방어구 인챈시 이펙트 .. 달기 .. 
			g_Particle.m_EffectManager.CreateDefenceEnChantEffect(this , 255 );
			
		}
		else
		{
			m_Char_WearType = CHAR_WEAR_NORMAL;	
		}
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_CoatArmor]->Code , ( Character *) this );
	}
	else
	{
		m_pItm_index[n_Wear_CoatArmor]	= g_Pc_Manager.GetInitItmIndex( this, n_Wear_CoatArmor );
		SetItemBuffer( n_Wear_CoatArmor );
		
		m_Char_WearType = CHAR_WEAR_NORMAL;
	}
	
	if( nRui->thePcInven.WearItem[n_WearSlot_PantsArmor] )
	{
		m_pItm_index[n_Wear_PantsArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_PantsArmor]->Code );
		SetItemBuffer( n_Wear_PantsArmor );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_PantsArmor]->Code , ( Character *) this );
		
	}
	else
	{
		m_pItm_index[n_Wear_PantsArmor]	= g_Pc_Manager.GetInitItmIndex( this, n_Wear_PantsArmor );
		SetItemBuffer( n_Wear_PantsArmor );	
	}
	
	if( nRui->thePcInven.WearItem[n_WearSlot_Gloves] )
	{
		m_pItm_index[n_Wear_Gloves] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_Gloves]->Code );
		SetItemBuffer( n_Wear_Gloves );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_Gloves]->Code , ( Character *) this );
	}
	else
	{
		m_pItm_index[n_Wear_Gloves]	= g_Pc_Manager.GetInitItmIndex( this, n_Wear_Gloves );
		SetItemBuffer( n_Wear_Gloves );	
	}
	
	if( nRui->thePcInven.WearItem[n_WearSlot_Shoes] )
	{
		m_pItm_index[n_Wear_Shoes] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_Shoes]->Code );
		SetItemBuffer( n_Wear_Shoes );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_Shoes]->Code , ( Character *) this );
	}
	else
	{
		m_pItm_index[n_Wear_Shoes]	= g_Pc_Manager.GetInitItmIndex( this, n_Wear_Shoes );
		SetItemBuffer( n_Wear_Shoes );
	}
	
	

	if(nRui->thePcInven.WearItem[n_WearSlot_SKIN_Helmet])
	{

		m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_SKIN_Helmet]->Code );
		SetItemBuffer( n_Wear_Hair );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_SKIN_Helmet]->Code , ( Character *) this );
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_SKIN_Helmet]->Code );
		if( item_Info && item_Info->theType == n_Type_Skin_KeepypHair )	// 헤어도 동시에 그려줘야 된다 (악세사리 형)
		{
			m_Basic_DrawItm[n_Wear_Hair] = 1;		
			m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_MainPC.char_info.theHair ); 
			SetBasicItemBuffer( n_Wear_Hair );
		}
		else
		{
			m_Basic_DrawItm[n_Wear_Hair] = 0;		
			m_Basic_Itm_index[n_Wear_Hair] = 0;		
		}		
	}
	else
	if( nRui->thePcInven.WearItem[n_WearSlot_Helmet] )
	{
		m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_Helmet]->Code );
		SetItemBuffer( n_Wear_Hair );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_Helmet]->Code , ( Character *) this );
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Helmet]->Code );
		if( item_Info && item_Info->theType == n_Type_KeepupHair )	// 헤어도 동시에 그려줘야 된다 (악세사리 형)
		{
			m_Basic_DrawItm[n_Wear_Hair] = 1;		
			m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_MainPC.char_info.theHair ); 
			SetBasicItemBuffer( n_Wear_Hair );
		}
		else
		{
			m_Basic_DrawItm[n_Wear_Hair] = 0;		
			m_Basic_Itm_index[n_Wear_Hair] = 0;		
		}		
	}
	else
	{
		m_Basic_Itm_index[n_Wear_Hair] = 0;
		m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_MainPC.char_info.theHair );
		SetItemBuffer( n_Wear_Hair );	
	}
	
	if( g_Pc.m_MainPC.char_info.theFace ) 
	{
		m_pItm_index[n_Wear_Face] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_MainPC.char_info.theFace );
		SetItemBuffer( n_Wear_Face );	
	}
	else
	{
		m_pItm_index[n_Wear_Face]	= g_Pc_Manager.GetInitItmIndex( this, n_Wear_Face );
		SetItemBuffer( n_Wear_Face );	
	}
	

	if( nRui->thePcInven.WearItem[n_WearSlot_SKIN_Serve] )
	{
		m_pItm_index[n_Wear_Serve] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_SKIN_Serve]->Code ); // 아이템 코드에 맞는 텍스쳐 번호 로딩  
		
		SetItemBuffer( n_Wear_Serve );	// By wxywxy
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_SKIN_Serve]->Code , ( Character *) this );
	}
	else
	if( nRui->thePcInven.WearItem[n_WearSlot_Serve] )
	{
		m_pItm_index[n_Wear_Serve] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_Serve]->Code ); // 아이템 코드에 맞는 텍스쳐 번호 로딩  
		
		SetItemBuffer( n_Wear_Serve );	// By wxywxy
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_Serve]->Code , ( Character *) this );
	}
	//	else
	//	{
	//		m_pItm_index[n_Wear_Serve]	= g_Pc_Manager.GetInitItmIndex( this, n_Wear_Serve );
	//	}
	
	m_pUserInfo->ArmorInfo.index[0] = -1;
	m_pUserInfo->ArmorInfo.index[1] = -1;
	
	if( nRui->thePcInven.WearItem[n_WearSlot_Shield] )
	{
		IsEquipWeapon = true; 
		
		m_pUserInfo->ArmorInfo.index[1] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code );
		m_pUserInfo->ArmorInfo.Code[1] = nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code;
		m_pUserInfo->bAttack = true;
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code , ( Character *) this );
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code );
		if( item_Info && item_Info->theType != n_Type_Shield ) 
		{
			//			m_Weapon_DelayTime = c_WeaponDelayTime - (item_Info->theAttackSpeed * 100);		
			m_Weapon_DelayTime = nRui->GetPcParamInfo()->Stat2.theAttackSpeed * 100;		
		}
		
		if( n_Type_Bow == item_Info->theType || n_Type_MagicBook == item_Info->theType  
			|| n_Type_Shield == item_Info->theType)
		{
			// 칼검기(잔상)만들기 위해서 높이값 구하기
			m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]]->m_AxisTop;
			m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]]->m_AxisBottom;					
			
			g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );
		}
		
		
	}
	
	
	if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] )
	{
		//		item_Info = g_Pc_Manager.GetItemBaseInfo( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
		
		//		if(			item_Info->theType == 1 ||			// 검
		//					item_Info->theType == 2 ||			// 대검		
		//					item_Info->theType == 5 ||			// 스태프 
		//					item_Info->theType == 6 )			// 수정구 
		//		{
		IsEquipWeapon = true;
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code , ( Character *) this );
		
		m_pUserInfo->ArmorInfo.index[0] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );			
		m_pUserInfo->ArmorInfo.Code[0] = nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code;
		
		
		if( (m_pUserInfo->ArmorInfo.index[0] != -1) && (m_pUserInfo->ArmorInfo.index[0] <= c_Chr_Itm_Count + c_Npc_Itm_Count) )
		{
			if( g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]] )
			{
				m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisTop;
				m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisBottom;					
			}
		}
		
		g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );
		
		//.................................................................................................
		// 근접 무기 체크 By wxywxy
		//.................................................................................................
		m_ISApproachWeapon = FALSE;			// 근접 무기 유/무
		m_AttackMotion	   = 0;				// 아이템 공격모션 번호
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
		
		if( NULL != item_Info )
		{
			if( item_Info->theType == n_Type_ShortWeapon )
			{
				m_ISApproachWeapon = TRUE;
				m_AttackMotion	   = item_Info->theAttackMotion;
			}
		}
		//.................................................................................................
		
		
		/*
		// 칼검기(잔상)만들기 위해서 높이값 구하기
		int mod_index = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_mod_index;
		
		  int num;
		  
			for( num = 0; num < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++num )
			{
			if( m_pUserInfo->vSwordTop.z < g_Pc_Manager.m_pMod[mod_index]->m_pVertex[num].m_vertex.z )
			m_pUserInfo->vSwordTop = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[num].m_vertex;
			
			  if( m_pUserInfo->vSwordBottom.z > g_Pc_Manager.m_pMod[mod_index]->m_pVertex[num].m_vertex.z )					
			  m_pUserInfo->vSwordBottom = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[num].m_vertex;
			  }
		*/
		m_pUserInfo->bAttack = true;
		
		//			item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
		
		//			if( item_Info )
		//			{
		//				m_Weapon_DelayTime = c_WeaponDelayTime - (item_Info->theAttackSpeed * 100);		
		m_Weapon_DelayTime = nRui->GetPcParamInfo()->Stat2.theAttackSpeed * 100;		
		//			}
		
		//		}
		//		else if(	item_Info->theType == 3 ||			// 활 
		//					item_Info->theType == 4 )			// 마법서 
		//		{
		//			m_pUserInfo->ArmorInfo.index[0] = -1;
		//			m_pUserInfo->ArmorInfo.index[1] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
		//		}
	}
#ifdef RIDING_SYSTEM	
	
	if( m_State.bRide )
	{	
		g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[0] = -1;
		g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[1] = -1;
		g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[2] = -1;		
		
		if( nRui->thePcInven.WearItem[n_WearSlot_RidingItem1] )
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[0] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_RidingItem1]->Code );
			g_Map.m_Npc[m_State.TransIndex].SetRideItemBuffer( 0 );	
		}
		
		if( nRui->thePcInven.WearItem[n_WearSlot_RidingItem2] )
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[1] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_RidingItem2]->Code );
			g_Map.m_Npc[m_State.TransIndex].SetRideItemBuffer( 1 );
		}
		
		if( nRui->thePcInven.WearItem[n_WearSlot_RidingItem3] )
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[2] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, nRui->thePcInven.WearItem[n_WearSlot_RidingItem3]->Code );
			g_Map.m_Npc[m_State.TransIndex].SetRideItemBuffer( 2 );
		}
	}
	
#endif
	
	if( IsEquipWeapon && !m_State.bRide )
	{
		///-- 아이템을 교체할시에... 해당 아이템이 연결된 노드가 애니메이션에 따라 
		///-- 틀리게 설정되어있다. 바로 장비를 장착한 애니로 전환 
		///-- 블렌딩을 피하고 아이템을 렌더링한다. 
		AnimationPlay( n_Stand_Peace9 );
		m_ani_frame = c_MotionBlendHalfFrame;
		m_blend_frame = c_MotionBlendFrame;
	}	
	
	// 길찾기 버퍼생성
	initpath();		// 있어야 되나??? 없애도 될듯...ㅎ
	
	PreCharTextureCreate();
	
	return;
}


//-----------------------------------------------------------------------------
// 선택화면 캐릭터 생성
//-----------------------------------------------------------------------------
void Player::SetTempItem()
{	
	// 캐릭터 생성창이 켜져 있다면 할 필요 없다
	if( g_Pc.m_MakePlayerIndex != -1 )
		return;
	
	// 아이템 재정렬하기(장착6종류, 악세사리2종류)(악세사리는 나중에처리)		
	//	for( int i = 0; i < MAX_EQUIPED; ++i )
	//	{
	//	}
}





//-----------------------------------------------------------------------------
// Desc: 큐에 데이타 넣기
//-----------------------------------------------------------------------------
void Player::SetQueue( const EVENT_DATA_INFO& data )
{		
	if( m_curt_event.type == SM_ACT )
	{
		m_curt_event.Clear();
	}
	
	if( m_curt_event.type == SM_STATE_DIE )
	{
		if( data.type == SM_STATE_DIE )
		{
			return;
		}
	}
	
	// 거래 중일 때는 공격 불가, NPC 대화 불가 
	if( g_ExchangeWnd.m_bIsAccept ) 
	{
		if( data.type == CM_MOVE_ATTACK )
		{
			if( data.race == c_NpcRace_Item  )
			{
				SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_GET_ON_EX ) );		
			}
			else
			{
				int nTarget = g_Map.FindNpc( data.unique );	
				if( nTarget == MAX_NPC_INTABLE )
					return;
				
				if( g_Map.m_NpcTable[nTarget].code >= c_NonplayerCode_Min && g_Map.m_NpcTable[nTarget].code <= c_NonplayerCode_Max )
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_NPCCHAT_ON_EX ) );		
				}
				else
				{
					SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_ATTACK_ON_EX ) );		
				}
			}						
			
			return; 
		}
	}
	
	// 같은 대상 시간차 연속 클릭 공격 불가 
	if( data.type == CM_MOVE_ATTACK && m_temp_event.type == CM_MOVE_ATTACK &&
		m_temp_event.skill_unique == data.skill_unique && m_temp_event.cell == data.cell )
	{
		return;
	}
	
	// 설마 처리할 메세지가 있는데 또 겹쳐서 날아온다? 이럴수 있을까? 있네...ㅡㅡ;
	if( m_temp_event.type == SM_ATTACK_NPC || m_temp_event.type == SM_USE_SKILL )
	{
		///		RLGS("m_temp_event deleted??? 설마 이거 때문이랴??!!!");
		//		RLGS("Player.cpp --- line:325");				
		//		RLG1("data.type = %d", data.type);						
		//		if( data.type == SM_MOVE_BROADCAST || data.type == SM_MOVE_ATTACK || data.type == CM_MOVE_ATTACK )
		//		{
		//			return;
		//		}
		//		else
		//		{
		SetUnderAttackEventMgr( m_temp_event );
		RLGS( "내가 공격이나 스킬을 내가 동시에 시전??!!" );				
		//		}
	}
	else if( m_temp_event.type == SM_STATE_DIE )
	{
		SM__STATE__DIE( m_temp_event );
	}
	
#ifdef ROCKCLIENT_DEV					
	if( data.type == SM_STATE_DIE )
	{
		RLGS( "Player 죽었삼~~ ㅜ ㅜ" );				
	}
#endif
	
	m_temp_event.Clear();
	m_temp_event = data;
	
	QueueProcess();	
}


//-----------------------------------------------------------------------------
// Desc: 큐클리어(워프등에 사용)
//-----------------------------------------------------------------------------
void Player::QueueClear()
{		
	// 플래그 초기화
	m_curt_event.Clear();
	m_save_event.Clear();
	m_next_event.Clear();
	m_temp_event.Clear();
	Clear();
	
	// 서있기 애니메이션
	AnimationPlay( n_Stand_Peace9 );		
}


//-----------------------------------------------------------------------------
// Desc: 큐처리 프로세스
//-----------------------------------------------------------------------------
void Player::QueueProcess()
{		
	
#ifdef CONTROL_ATTACK_VELOCITY 	
	
	ComBoAttackListPos iPos = m_ComBoAttackList.begin();
	if( iPos != m_ComBoAttackList.end() )
	{
		m_aLastAttackSendTime = 800;
		
		switch( g_Pc.m_MainPC.char_info.theMainClass )
		{
			case Slayer:
				{
					m_aLastAttackSendTime = 900;
				}
				break;
			case Archer:
			case Sniper:
				{
					m_aLastAttackSendTime = 1500;
				}
				break;
		}		
		
		long lAttackGapTime = SAFE_TIME_SUB( g_nowTime , m_aLastAttackSendTime );
		long lCompareTime = m_aMinAttackDelayTime / 2;
		if( SAFE_TIME_COMPARE( lAttackGapTime , > , lCompareTime ) )
		{
			///--PRINT_DLG_LOG("콤보 간격타임 %d", g_nowTime - m_aLastAttackSendTime );
			CM__ATTACK__NPC( (EVENT_DATA_INFO)(*iPos) );
			m_ComBoAttackList.erase(iPos);
		}
	}
	
#endif
	
	EVENT_DATA_INFO queue_event;
	queue_event = m_temp_event;
	m_temp_event.Clear();
	
	//	RLG1("Player queue_event.type = %d", queue_event.type);
	//	RLG1("Player m_curt_event.type = %d", m_curt_event.type);
	
	if( m_curt_event.type == SM_SLIDE_NPC || m_curt_event.type == SM_STATE_DIE )
	{
#ifdef ROCKCLIENT_DEV					
		if( queue_event.type == SM_ATTACK_NPC )
		{
			RLGS( "!Player 가 죽어 있는 상태임...but SM_ATTACK_NPC 메시지가 날아옴!!!" );
		}
#endif 
		m_save_event.Clear();
		m_next_event.Clear();
		return;
	}
	
	if( queue_event.type == SM_MOVE_BROADCAST || queue_event.type == SM_MOVE_ATTACK || queue_event.type == CM_MOVE_ATTACK || 
		queue_event.type == SM_UNDERATTACK || queue_event.type == SM_SLIDE_NPC || queue_event.type == SM_STATE_DIE || queue_event.type == SM_SELF_DIE )
	{
		if( queue_event.type != SM_SELF_DIE )
		{
			// 죽을때 죽더라도 내가 때린놈은 죽이던지 때리던지 해야지...^ ^V
			if( m_curt_event.type == SM_ATTACK_NPC || 
				( m_curt_event.type == SM_USE_SKILL && m_curt_event.status == SM_USE_SKILL ) )
			{
				
				// 이미 처리가 안되어 있을때...처리 한다. 
				if( m_ani_frame <= g_Pc_Manager.GetImpactFrame(this, m_ani_index) && m_curt_event.damage >= 0 )
				{
					RLGS("Player.cpp --- line:399");
					RLG1("queue_event.type = %d", queue_event.type);		
					
					SetUnderAttackEventMgr( m_curt_event );
					m_curt_event.Clear();
				}	
			}
		}
		else
		{
			queue_event.type = SM_STATE_DIE;
		}
		
		if( m_next_event.type == SM_ATTACK_NPC || 
			( m_next_event.type == SM_USE_SKILL && m_next_event.status == SM_USE_SKILL ) )
		{
			EVENT_DATA_INFO next_attack;
			
			next_attack = m_next_event;
			
			m_next_event.Clear();
			
			RLGS("Player.cpp --- line:414");				
			
			SetUnderAttackEventMgr( next_attack );
		}
	}
	
	// 같은 대상 시간차 연속 클릭 공격 불가 
	if( queue_event.type == CM_MOVE_ATTACK && m_curt_event.type == CM_MOVE_ATTACK &&
		m_curt_event.skill_unique == queue_event.skill_unique && m_curt_event.cell == queue_event.cell )
	{
		return;
	}


	switch( queue_event.type )
	{
	case CM_MOVE_ATTACK:		
	case SM_MOVE_ATTACK:
	case SM_MOVE_BROADCAST:
		{
			if(theFindWay)
			{
				m_Mov.vStPos.x = m_Mov.vCVtPos.x;
				m_Mov.vStPos.z = m_Mov.vCVtPos.z;				
			}
			
			SM__MOVE__BROADCAST( queue_event );
		}
		break;
	case SM_ATTACK_NPC:
		{
			SM__ATTACK__NPC( queue_event );
			
			theCombo_State = n_Combo_Enable;			//콤보 재 시도 가능
			//by simwoosung
			if( theActionSystem.GetComboStatus() == n_ComboComplete )
			{
				theActionSystem.InitComboEnable();
				theCombo_State = n_Combo_Skill_Enable;		//콤보완성시 초기화
				nRui->m_Combo2DTimingEffect.SetComboState( STATE_COMBO_NONE );
			}
			else if( theActionSystem.IsEnableComboSkill() )	
			{
				nRui->m_Combo2DTimingEffect.SetComboState( STATE_COMBO_NONE );				
				nRui->m_Combo2DTimingEffect.SetComCount( queue_event.count, queue_event.race , queue_event.unique );				
				nRui->m_Combo2DTimingEffect.StartEffect();				
			}		
		}
		break;
	case SM_UNDERATTACK:
		{
			if( m_curt_event.type == SM_ATTACK_DELAY )
			{
				if( m_curt_event.type != SM_MOVE_BROADCAST )
				{
					m_save_event = m_curt_event;
				}
			}
			SM__UNDERATTACK( queue_event );
		}
		break;
	case SM_STATE_DIE:
		{
			SM__STATE__DIE( queue_event );
		}
		break;
	case SM_SLIDE_NPC:
		{			
			SM__SLIDE__NPC( queue_event );
		}
		break;
	case SM_USE_SKILL:
		{
			if( queue_event.status == SM_RESTSKILL_SITDOWN ||
			//	queue_event.status == SM_CHANT_READY || 
				queue_event.status == SM_SELLSKILL_SITDOWN )
			{
				Pet_CsOwnerBackMove();
			}			
			
			SM__USE__SKILL( queue_event );		
			theCombo_State = n_Combo_Enable;					// 콤보 재 시도 가능 				
		}
		break;
	case SM_ATTACK_DELAY:
		{
			if( m_curt_event.type != SM_MOVE_BROADCAST )
			{
				//				SystemPrint( n_DarkRed, n_SystemMsg, "==SM_ATTACK_DELAY== m_curt_event.type = %d", m_curt_event.type );
				m_curt_event = queue_event;			
			}
		}		
		break;
		
    case SM_BOW_ATTACK:
		{
			SM__BOW_ATTACK( queue_event );
		}
		break;
		
	case SM_ACT:	
		{
			SM_ACT_PROCESS( queue_event );
		}
		break;
		
		// 시퀀스 처리시 에니메이션
	case SM_ANI_SQUENCE:
		{
			SM_ANI_SQUENCE_PROCESS( queue_event );
		}
	}	
}

//.................................................................................................................
// 시퀀스 처리시 에니메이션
//.................................................................................................................
bool Player::SM_ANI_SQUENCE_PROCESS(EVENT_DATA_INFO &data)
{
	m_curt_event = data;
	
	AnimationPlay( data.motion );
	
	return true;
}

//.................................................................................................................
// 개인 상점 처리
//.................................................................................................................
bool Player::SM_USER_STORE(EVENT_DATA_INFO &data)
{
	m_curt_event = data;
	
	switch( data.status )
	{
	case SM_SELLSKILL_SITDOWN:
		AnimationPlay( n_Store_sit0 );
		theIsDrawWeapon       = false;
		theDrawWeaponFadeTime = g_nowTime;
		break;
		
	case SM_SELLSKILL_KEEPING:
		AnimationPlay( n_Store_sit1 );
		break;
		
	case SM_SELLSKILL_STANDUP:
		AnimationPlay( n_Store_sit2 );
		break;
		
	default:
		AnimationPlay( data.motion );
		RLGS( "------------ Not Found Staus In SM_USER_STORE ------------" );
		break;
	}
	
	return TRUE;
}

bool Player::SM_ACT_PROCESS( EVENT_DATA_INFO& data )
{
	m_curt_event = data;
	//여기서 두개를 처리한다 .. 
	// 카운트 여기서는 라이딩이 있냐 없냐 .. 처리 .. 
	
	if(!m_State.bRide) 
	{
		AnimationPlay( data.motion );
	}
	else 
	{
		AnimationPlay( data.motion );
		g_Map.m_Npc[m_curt_event.tbl_Idx].AnimationPlay(m_curt_event.number);
		
	}
	
	return true;
}

bool Player::SM__CHANT__READY( EVENT_DATA_INFO& data )
{
	m_curt_event = data;
	
	//	SetDirection( data.vPos.x, data.vPos.z );
	AnimationPlay( data.motion );	
	
	return true;
}

bool Player::SM__CHANT__SPELL( EVENT_DATA_INFO& data )
{
	nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	theActionSystem.theUseSkillCode = m_curt_event.count;
		
	theActionSystem.InitChantEnable();
	
	m_curt_event = data;
	
	AnimationPlay( data.motion );
	
	return true;
}


bool Player::SM__RESTSKILL( EVENT_DATA_INFO& data )
{
	m_curt_event = data;
	
	switch( data.status )
	{
	case SM_RESTSKILL_SITDOWN:
		{
			AnimationPlay( n_Skill_RestSitdown );
			theIsDrawWeapon = false;
			theDrawWeaponFadeTime = g_nowTime;			
			g_Particle.m_EffectManager.SetRestSkill( ( Character * ) this );
			///-- By simwoosung
			///-- 명상이나 호흡조절이 아이콘 표시가 나오지 않아서 해당 루틴을 추가한다.
			SetUnderAttackEventMgr(data);			
		}		
		break;
	case SM_RESTSKILL_KEEPING:
		{
			AnimationPlay( n_Skill_RestKepping );
		}
		break;
	case SM_RESTSKILL_STANDUP:
		{
			AnimationPlay( n_Skill_RestStandup );			
			g_Particle.m_EffectManager.DeleteRestSkill( ( Character * ) this );
		}
		break;
	default:
		AnimationPlay( data.motion );
		break;
	}	
	
	return true;
}

bool Player::SM__BOW_ATTACK( EVENT_DATA_INFO& data )
{
	switch( data.status )
	{
	case SM_BOW_READY:
		{			
			m_curt_event = data;
			AnimationPlay( data.motion );
		}
		break;
		
	case SM_BOW_WAIT:
		{
			m_curt_event = data;
			AnimationPlay( data.motion );
		}	
		break;
		
	default:
		break;
	}
	
	return TRUE;
}

bool Player::SM__USE__SKILL( EVENT_DATA_INFO& data )
{

	switch( data.status )
	{
	case SM_CHANT_READY:	
		{
			g_Particle.m_EffectManager.GetSkillStartSound( data.count , (Character *)this );
			
			return SM__CHANT__READY( data );
		}
	case SM_CHANT_SPELLING:
		return SM__CHANT__SPELL( data );
	case SM_RESTSKILL_SITDOWN:
	case SM_RESTSKILL_KEEPING:
	case SM_RESTSKILL_STANDUP:
		return SM__RESTSKILL( data );		
		
	case SM_SELLSKILL_SITDOWN:
	case SM_SELLSKILL_KEEPING:
	case SM_SELLSKILL_STANDUP:	
		return SM_USER_STORE( data );
		
	default:
		break;
	}	
	
	// use skill
	m_IsCDataQActed = FALSE;
	
	m_curt_event = data;
	//Pet_CsOwnerBackMove();
	
	//	AnimationPlay( data.motion );
	
	int chrAttrIndex = g_Pc.GetPlayer()->GetAttrIndex();
	int chrClassType = nRui->thePcParam.MainClass;
	
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( data.count ); 
	if( skill_Info == NULL )
		return false;
	
	if( data.race == n_Cell )
	{
		if( skill_Info->theActTarget == n_AT_TARGETLAND )
		{
			SetDirection( data.vPos.x, data.vPos.z );
		}
	}
	else
	{
		SetDirection( data.vPos.x, data.vPos.z );
	}
	
	
	if( skill_Info->theNeedSpelling == 0 )
		g_Particle.m_EffectManager.GetSkillStartSound( data.count , this , FALSE );
	
	
	if(	// 밀레나 남-녀, 직업은 원더러, 아처, 소울 브레이커  
		( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || 
		chrClassType == Archer || chrClassType == Sniper ) 
		) 			
	{
		if(!m_State.bRide)
		{
			if( data.motion == 0 )
			{ 
				AnimationPlay( n_Attack9_Basic0 );
			}
			else
			{
				AnimationPlay( data.motion );				
				if(skill_Info->theNeedSpelling)
				{
					SetUnderAttackEventMgr( m_curt_event );				
					m_start_blend_frame = m_ani_frame;
					m_PreTime = g_nowTime;					
					m_OneTimeJump = true;
				}
			}
		}
	}
	
	if( 
		(	// 밀레나 남-녀, 직업은 원더러, 소드맨, 나이트, 머시너리, 슬레이어  
		( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
		( chrClassType == Wanderer || 
		chrClassType == Swordman || chrClassType == Knight 
		|| chrClassType == Mercenary || chrClassType == Slayer ) 
		) ||
		(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
		( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
		( chrClassType == Stranger || 
		chrClassType == Shaman || chrClassType == Psyche ) 
		)
		)	
	{	
		if( skill_Info->theNeedSpelling == 0 )
		{
			if(!m_State.bRide)
			{
				if( data.motion == 0 )
				{ 
					AnimationPlay( n_Attack9_Basic0 );
				}
				else
				{
					AnimationPlay( data.motion );
				}
			}
		}
	}
	
	if(	// 레인 남-녀, 직업은 스트레인저, 소서러, 서머너 
		( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman ) &&
		( chrClassType == Stranger || 
		chrClassType == Magician || chrClassType == Prominas
		|| chrClassType == Priest || chrClassType == Holy_Avenger ) 		 
		)
	{	
		if(!m_State.bRide)
		{
			if( data.motion == 0 )
			{ 
				AnimationPlay( SetChantMotion( SM_USE_SKILL, 0 ) );
			}
			else
			{
				AnimationPlay( data.motion );
			}
		}
	}
	
	return true;
}


//-----------------------------------------------------------------------------
// Desc: 이동
//-----------------------------------------------------------------------------
bool Player::SM__MOVE__BROADCAST( EVENT_DATA_INFO& data )
{
	//-----------------------------------------------------------------------------
	// 같은 위치을 클릭했다면 무시	
	//-----------------------------------------------------------------------------
	if( data.type == SM_MOVE_BROADCAST ) 
	{
		if( data.x == m_Mov.curt.x && data.y == m_Mov.curt.y )
			return false;
		if( data.x == m_Mov.next.x && data.y == m_Mov.next.y )
			return false;
		if(	data.x == m_Mov.dest.x && data.y == m_Mov.dest.y )
			return false;	
	}
	
	//-----------------------------------------------------------------------------
	// 이동가능한 상태인지 검사
	//-----------------------------------------------------------------------------
	switch( m_curt_event.type )
	{
	case NULL:					// 정지상태
	case SM_SIMSIM:				// 심심한 상태
	case SM_MOVE_BROADCAST:		// 이동 중인 상태 	
	case SM_ATTACK_DELAY:		// 공격 대기 자세 
	case SM_MOVE_ATTACK:		// 이동 공격 
	case CM_MOVE_ATTACK:		// 이동 공격 		
	case SM_ATTACK_NPC:			// 공격 
	case SM_USE_SKILL:			// 공격 
	case SM_UNDERATTACK:		// 리액션	
	case SM_BOW_ATTACK:	
		{
			if( data.type == SM_MOVE_ATTACK )
			{
				m_save_event = data;
				
				theAttack_Event.Clear();
				
				return SM__MOVE( data, true );
			}
			else
			{
				return SM__MOVE( data );			
			}
		}
		break;
	default:
		{
			//-----------------------------------------------------------------------------
			// 이동가능한 곳인지 체크
			//-----------------------------------------------------------------------------
			if( FindDestTile( data ) == true )
			{		
				// 이동중일때는 목적지만 바꾸기
				if( m_curt_event.type == SM_MOVE_BROADCAST )
				{	
					// 이동값수정
					m_Mov.dest.x = data.x;
					m_Mov.dest.y = data.y;
					m_save_event.Clear();
					
					//-----------------------------------------------------------------------------
					// 도착할 타일의 거리 비교해서.. 3이상이면 bWalk = false.. 뛰기
					//-----------------------------------------------------------------------------
					//					bool bWalk = false;
					//					int x = g_Pc.GetDestX()-data.x;
					//					int y = g_Pc.GetDestY()-data.y;
					//					if( abs(x) < 2 && abs(y) < 2 )
					//						bWalk = true;
					//					g_Pc.CM__TOGGLE__RUN( bWalk );
				}
				// 어떠한 행동을 하고 있다면 이동메세지를 저장하기
				else
				{				
					m_save_event = data;
				}						
			}
			return false;
		}
		break;
	}		
	return true;
}


bool Player::CM__ATTACK__NPC( EVENT_DATA_INFO& data )
{
	m_aLastAttackSendTime = (DWORD)g_nowTime; 
	
	int nADisErrCnt = 0;
	
	///-- By simwoosung -- 공격시 서버와 클라이언트 거리에러 발생 확인용 변수값 보관 
	if( &m_save_event == &data )
	{
		nADisErrCnt = m_save_event.nADisErrCnt;
	}	
	
	//	RLGS( "***********************************CM__ATTACK__NPC()" );
	// 공격보다 이동이 우선..컨트롤시 갑갑하게 느껴진다는 의겸 수렴. 
	if( ( theAttack_Event.type == SM_MOVE_ATTACK ) || 
		( data.type != SM_MOVE_ATTACK && g_RockClient.GetActiveCsStartMoveFlag() ) )
	{
		//		SystemPrint( n_DarkRed, n_SystemMsg, "==return FALSE== CM__ATTACK__NPC() => theAttack_Event,GetActiveCsStartMoveFlag()" );
		RLG1( "theAttack_Event.type = %d", theAttack_Event.type );
		RLG1( "g_RockClient.GetActiveCsStartMoveFlag() = %d", g_RockClient.GetActiveCsStartMoveFlag() );
		RLG1( "m_curt_event.type = %d", m_curt_event.type );
		
		data.Clear();
		
		return FALSE;
	}
	
	//.........................................................................................................
	// 자기 자신에게 사용되는 스킬후 자동공격 루틴이 실행되면 자기한테 공격하는거 막아주기 By wxywxy
	//.........................................................................................................
	if( data.race >= 0 && data.unique == GetUnique() )
	{
		data.Clear();
		
		return FALSE;
	}
	//.........................................................................................................
	
	g_RockClient.Send_RpCsStop( g_Pc.m_MainPC.lDestBlock, m_Mov.now_Yrad );
	
	//방어형 펫을 소유할 경우
	//뒤쪽으로 방어형 펫을 이동시킨다.
	if( m_State.nPetState == nPetState_Master )
	{
		//펫이 방어형 펫인지 여부 확인 - 공격 속도가 0이면 방어형
		SNPCBaseInfo* npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_State.PetIndex].code );		
		
		if( npc_Info && PET_AI_REDUCEDAMAGE == npc_Info->theAttackType )
		{
			Pet_CsStartMove(0, true, &data);	
		}	
	}
	
	//	EVENT_DATA_INFO underattack_event;
	//	underattack_event.type = SM_UNDERATTACK;
	
	//	SystemPrint( n_DarkRed, n_SystemMsg, "CM__ATTACK__NPC() => data.type = SM_ATTACK_NPC;" );
	data.type = SM_ATTACK_NPC;
	
	if( data.motion == 0 )
		data.motion = n_Attack9_Basic0;
	
	int attack_type = 0;
	bool isPickUp = false;
	bool isNONPlayer = false;
	bool isStorePlayer = false;				// 상점 따라가기
	
	int race_type = 0;
	
	if( data.race < 0 )
	{
		race_type = n_NPC;
	}
	else
	{
		race_type = n_PC;
	}	
	
	if( race_type == n_NPC )					// NPC	
	{			
		if( g_Map.m_Npc[data.tbl_Idx].m_curt_event.type == SM_STATE_DIE )
		{
			//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			data.Clear();
			return false;
		}
		
		attack_type	 = n_NPC;
		data.x		 = g_Map.m_NpcTable[data.tbl_Idx].lDestBlock%g_Pc.m_cSize;
		data.y		 = g_Map.m_NpcTable[data.tbl_Idx].lDestBlock/g_Pc.m_cSize;
		data.vPos.x	 = g_Map.m_Npc[data.tbl_Idx].GetPosTM()._41;
		data.vPos.y  = g_Map.m_Npc[data.tbl_Idx].GetPosTM()._42;
		data.vPos.z  = g_Map.m_Npc[data.tbl_Idx].GetPosTM()._43;
		
		//		if( g_Map.m_Npc[data.tbl_Idx].ReadyUnderAttackMotion() )
		//		{
		//			underattack_event.motion = g_Map.m_Npc[data.tbl_Idx].SetUnderAttackMotion( 100, 1 );
		//			g_Map.m_Npc[data.tbl_Idx].SetQueue(underattack_event);	
		//		}
		if( g_Map.m_NpcTable[data.tbl_Idx].code >= c_NonplayerCode_Min && g_Map.m_NpcTable[data.tbl_Idx].code <= c_NonplayerCode_Max )
		{
			isNONPlayer = true;
		}
		else if( g_Map.m_NpcTable[data.tbl_Idx].race == c_NpcRace_Item )
		{
			isPickUp = true;
		}
		
	}
	else								// 유저
	{
		if( g_Pc.m_Pc[data.tbl_Idx].m_curt_event.type == SM_STATE_DIE )
		{
			//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			data.Clear();
			return false;
		}
		// 상점 스킬중이면 
		else if( g_Pc.m_Pc[data.tbl_Idx].m_curt_event.status == SM_SELLSKILL_SITDOWN ||
			g_Pc.m_Pc[data.tbl_Idx].m_curt_event.status == SM_SELLSKILL_KEEPING ||
			g_Pc.m_Pc[data.tbl_Idx].m_curt_event.status == SM_SELLSKILL_STANDUP   )
		{
			isStorePlayer = true;
		}
		// 같은종족이며 상점 스킬이 아니면 중간에 중단
		else if( !g_RockClient.CanAttackPlayer( &g_Pc.m_Pc[data.tbl_Idx] ) )		
		{
			// Approach Weapon By wxywxy
			if( g_Pc.GetPlayer()->m_ISApproachWeapon )
			{
				switch( g_Pc.m_Pc[data.tbl_Idx].m_curt_event.status )
				{
				case SM_SELLSKILL_SITDOWN:
				case SM_SELLSKILL_KEEPING:
				case SM_SELLSKILL_STANDUP:
					data.Clear();
					return false;
				}
			}
			else
				if( theMapType != n_FightZone )
				{
					data.Clear();
					return false;
				}
		}
		
		attack_type	 = n_PC;
		data.x		 = g_Pc.m_PcTable[data.tbl_Idx].lDestBlock%g_Pc.m_cSize;
		data.y		 = g_Pc.m_PcTable[data.tbl_Idx].lDestBlock/g_Pc.m_cSize;
		data.vPos.x	 = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._41;
		data.vPos.y  = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._42;
		data.vPos.z  = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._43;
		
		//		if( g_Pc.m_Pc[data.tbl_Idx].ReadyUnderAttackMotion() )
		//		{
		//			underattack_event.motion = g_Pc.m_Pc[data.tbl_Idx].SetUnderAttackMotion( 100, 1 );
		//			g_Pc.m_Pc[data.tbl_Idx].SetQueue(underattack_event);
		//		}
	}
	
	//	int range = CLIENT_TILESIZE * 5;
	int range = 4;
	
	//	if( !isNONPlayer )		
	{
		SItemBaseInfo* item_Info = NULL;	
		
		if( m_pUserInfo->bAttack == true )		// 무기 착용시 
		{
			if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] != NULL )
			{
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
				if( item_Info && item_Info->theClass == n_Class_Weapon )
				{
					//					range = (item_Info->theRange + 1) * CLIENT_TILESIZE;		// 타일당 8 정도의 실제 사정 거리
					range = item_Info->theRange; 					
				}
			}
			else if( nRui->thePcInven.WearItem[n_WearSlot_Shield] != NULL )
			{
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code );
				if( item_Info && item_Info->theClass == n_Class_Weapon )
				{
					//					range = (item_Info->theRange + 1) * CLIENT_TILESIZE;		// 타일당 8 정도의 실제 사정 거리
					range = item_Info->theRange; 
				}
			}
		}
	}
	
	bool bFind = FindNpcTile( m_Mov.curt.x, m_Mov.curt.y, range, data );	
	
	if( bFind == false )							
	{			
		RLGS( ".............. CM__ATTACK__NPC ........... FindNpcTile = FALSE" );
		//QueueClear();
		
		EVENT_DATA_INFO attackNPC_event;
		attackNPC_event.type = CM_MOVE_ATTACK;
		attackNPC_event.motion = n_Attack9_Basic0;
		attackNPC_event.tbl_Idx = data.tbl_Idx;
		
		///-- By simwoosung - 위 루틴대로 가면 NPC가 움직이게되면 추적중지상태로 바뀐다.
		///-- if( m_curt_event.type && m_curt_event.type != SM_MOVE_BROADCAST )		
		///-- 내가 왜 저렇게 짠거지..??? 이유가 있을거 같은데...;;;
		///-- 기획팀 의견으로는 자동으로 쫓아가는 루틴이 없어져야 한다는데...
		///-- 내가 실플레이 할때는 자동추적이 편한데;;;;; 일단 풀자
		{
			if( race_type == n_NPC )
			{
				attackNPC_event.race	= g_Map.m_NpcTable[data.tbl_Idx].race;
				attackNPC_event.index	= g_Map.m_NpcTable[data.tbl_Idx].index;
				attackNPC_event.unique	= g_Map.m_NpcTable[data.tbl_Idx].lUnique;
				attackNPC_event.x		= g_Map.m_NpcTable[data.tbl_Idx].lDestBlock%(g_Map.m_cSize*2);
				attackNPC_event.y		= g_Map.m_NpcTable[data.tbl_Idx].lDestBlock/(g_Map.m_cSize*2);
				attackNPC_event.vPos.x	= g_Map.m_Npc[data.tbl_Idx].GetPosTM()._41;
				attackNPC_event.vPos.y	= g_Map.m_Npc[data.tbl_Idx].GetPosTM()._42;
				attackNPC_event.vPos.z	= g_Map.m_Npc[data.tbl_Idx].GetPosTM()._43;
				attackNPC_event.cell	= g_Map.m_NpcTable[data.tbl_Idx].lDestBlock; 
				g_Pc.GetPlayer()->PushQueue( attackNPC_event, TRUE );
				g_Pc.GetPlayer()->PetSetAttackQueue( attackNPC_event, false );				
			}
			else
			{
				attackNPC_event.race	= g_Pc.m_PcTable[data.tbl_Idx].char_info.race;
				attackNPC_event.index	= g_Pc.m_PcTable[data.tbl_Idx].nIndex;
				attackNPC_event.unique	= g_Pc.m_PcTable[data.tbl_Idx].lUnique;
				attackNPC_event.x		= g_Pc.m_PcTable[data.tbl_Idx].lDestBlock%(g_Map.m_cSize*2);
				attackNPC_event.y		= g_Pc.m_PcTable[data.tbl_Idx].lDestBlock/(g_Map.m_cSize*2);
				attackNPC_event.vPos.x	= g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._41;
				attackNPC_event.vPos.y	= g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._42;
				attackNPC_event.vPos.z	= g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._43;
				attackNPC_event.cell	= g_Pc.m_PcTable[data.tbl_Idx].lDestBlock;
				g_Pc.GetPlayer()->PushQueue( attackNPC_event, TRUE );
				g_Pc.GetPlayer()->PetSetAttackQueue( attackNPC_event, false );
			}
		}
		
		//		data.Clear();
		//		m_curt_event.Clear();
		//		AnimationPlay( n_Stand_Battle9 );
		//		SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_CANT_ATTACK_DISTANCE ) );		

		return false;		
	}
	
	
#ifdef	NETWORK_CLIENT	
	if( data.unique == 0 )
	{
		DebugPrint( "\nerror AttackedID: %d\n", data.unique );
	}
	
	if( isNONPlayer )
	{
		NpcChatWnd.theNpcTableIdx = data.tbl_Idx;
		NpcChatWnd.theNpcCode = g_Map.m_NpcTable[data.tbl_Idx].code;
		
		DWORD NPCID = (DWORD)g_Map.m_NpcTable[data.tbl_Idx].lUnique;
		WORD ScriptCode = (WORD)g_Map.m_NpcTable[data.tbl_Idx].code;
		g_RockClient.Send_RpCsScriptStart(NPCID, ScriptCode);
		g_Pc.GetPlayer()->Pet_CsStartMove(g_Pc.m_MainPC.lDestBlock);
		
	}	
	else if( isPickUp )
	{
		g_RockClient.Send_RpCsPickup( g_Map.m_NpcTable[data.tbl_Idx].lUnique );
		g_Pc.GetPlayer()->Pet_CsStartMove(g_Pc.m_MainPC.lDestBlock);
		//		g_Map.SM__ITEM__DISAPPEAR( g_Map.m_NpcTable[data.tbl_Idx].lUnique );
	}
	// 상점 플레이서 선택시
	else if( isStorePlayer )
	{
		///나랑 대상의 국가가 같을시에만 상점을 열게한다.
		if( nRui->thePcParam.Citizen == g_Pc.m_PcTable[ data.tbl_Idx ].char_info.theCitizen )
		{	
			g_RockClient.Send_CsInPCShop( g_Pc.m_Pc[data.tbl_Idx].GetUnique() );
		}
		
		g_Pc.GetPlayer()->Pet_CsStartMove(g_Pc.m_MainPC.lDestBlock);		
	}
	else
	{
		//라이딩 중이면 공격 불가
		if(m_State.bRide)
		{
			return true;
		}		

		BOOL IsArcherReady = FALSE;
		
		// 아처이면 서버에 준비패킷 보내고 컨디션 게이지 띄워보아요
		if( IsArcherReady )
		{
			g_RockClient.Send_RpCsArcherReady( data.unique, attack_type );	
			SetDirection( data.vPos.x, data.vPos.z );
			
			EVENT_DATA_INFO temp_event;
			
			temp_event.Clear();
			
			temp_event = data;
			
			data.Clear();	
			
			m_save_event = temp_event;
			
			temp_event.Clear();
		}
		else
		{
			
			if(attack_type == n_PC)		///-- 공격 대상이 유저이며
			{
				///-- 은신, 월영으로 보이지 않을경우 공격방지
				if(g_Pc.m_Pc[data.tbl_Idx].m_CharAlpha.GetState() == CHAR_NONE_DISPLAY)
				{
					return true;
				}				
			}			
#ifdef _DEBUG
			PRINT_DLG_LOG("Send Attack -> Server   %d ", data.tbl_Idx );
#endif 
			Illusion_CsAttack( data.unique, attack_type, data.motion, data.critical, data.count, data.status, data.number );				
			
			if( m_State.nPetState == nPetState_Master )
			{
				if( SM_ATTACK_NPC != g_Map.m_Npc[m_State.PetIndex].m_curt_event.type && 
					SM_ATTACK_DELAY != g_Map.m_Npc[m_State.PetIndex].m_curt_event.type &&
					SM_USE_SKILL!= g_Map.m_Npc[m_State.PetIndex].m_curt_event.type)
				{
					EVENT_DATA_INFO attackevent;			
					
					if( attack_type	== n_NPC )
					{
						attackevent.tbl_Idx = data.tbl_Idx;
						attackevent.race	= g_Map.m_NpcTable[attackevent.tbl_Idx].race;
						attackevent.index	= g_Map.m_NpcTable[attackevent.tbl_Idx].index;
						attackevent.unique	= g_Map.m_NpcTable[attackevent.tbl_Idx].lUnique;
						attackevent.x		= g_Map.m_NpcTable[attackevent.tbl_Idx].lDestBlock%(g_Map.m_cSize*2);
						attackevent.y		= g_Map.m_NpcTable[attackevent.tbl_Idx].lDestBlock/(g_Map.m_cSize*2);
						attackevent.vPos.x	= g_Map.m_Npc[attackevent.tbl_Idx].GetPosTM()._41;
						attackevent.vPos.y	= g_Map.m_Npc[attackevent.tbl_Idx].GetPosTM()._42;
						attackevent.vPos.z	= g_Map.m_Npc[attackevent.tbl_Idx].GetPosTM()._43;
						attackevent.cell	= g_Map.m_NpcTable[attackevent.tbl_Idx].lDestBlock;
						attackevent.type = CM_MOVE_ATTACK;					
						g_Pc.GetPlayer()->PetSetAttackQueue( attackevent );						
					}
					else
					{
						attackevent.tbl_Idx = data.tbl_Idx;
						attackevent.race	= g_Pc.m_PcTable[attackevent.tbl_Idx].char_info.race;
						attackevent.index	= g_Pc.m_PcTable[attackevent.tbl_Idx].nIndex;
						attackevent.unique	= g_Pc.m_PcTable[attackevent.tbl_Idx].lUnique;
						attackevent.x		= g_Pc.m_PcTable[attackevent.tbl_Idx].lDestBlock%(g_Map.m_cSize*2);
						attackevent.y		= g_Pc.m_PcTable[attackevent.tbl_Idx].lDestBlock/(g_Map.m_cSize*2);
						attackevent.vPos.x	= g_Pc.m_Pc[attackevent.tbl_Idx].GetPosTM()._41;
						attackevent.vPos.y	= g_Pc.m_Pc[attackevent.tbl_Idx].GetPosTM()._42;
						attackevent.vPos.z	= g_Pc.m_Pc[attackevent.tbl_Idx].GetPosTM()._43;
						attackevent.cell	= g_Pc.m_PcTable[attackevent.tbl_Idx].lDestBlock;
						attackevent.type = CM_MOVE_ATTACK;					
						g_Pc.GetPlayer()->PetSetAttackQueue( attackevent );						
					}
					
					//g_Map.m_Npc[m_State.PetIndex].theAttackedID = data.unique;
					//g_Map.m_Npc[m_State.PetIndex].ChangeState(AttackState::Instance());
				}
			}
			data.Clear();	
		}
	}
	
#else
	
	SetDirection( data.vPos.x, data.vPos.z );
	AnimationPlay( data.motion );
	
	if( m_save_event.type == SM_MOVE_ATTACK )
	{
		m_curt_event = data;		// m_curt_event = m_save_event;
	}
	else
	{
		SetQueue(data);
	} 	
#endif

	m_save_event.nADisErrCnt = nADisErrCnt;
	
	return true; 
}

void Player::PushQueue(EVENT_DATA_INFO & data, BOOL IsAllClear)
{
	data.lStartTime = timeGetTime();	
	
	///-- 죽는 이벤트가 들어오면 기존공격 에대한 상황을 일괄처리한다.
	if( data.type == SM_STATE_DIE )
	{
		IsAllClear = TRUE;
	}

	if( m_curt_event.type == SM_USE_SKILL && data.type == SM_MOVE_ATTACK )
	{
		data.type = SM_MOVE_BROADCAST;
		theAttack_Event.Clear();
		IsAllClear = FALSE;
	}
	
	if(IsAllClear)
	{
		AllActQueue();
		SetQueue( data );
		return;
	}	
	
	if(  m_curt_event.type == SM_ATTACK_NPC || 
		( m_curt_event.type == SM_USE_SKILL && m_curt_event.status != SM_CHANT_SPELLING ) )
	{	
		if( m_next_event.type == SM_ATTACK_NPC || 
			m_next_event.type == SM_USE_SKILL )
		{
			SetUnderAttackEventMgr( m_curt_event );
			SetQueue( m_next_event );
			m_next_event = data;		
		}
		else
		{
			m_next_event = data;
		}
	}
	else
	{	
		///-- 현재 스펠시전 동작을 하고있었다면... 스필시전 이펙트를 해제해준다
		if( m_curt_event.type == SM_USE_SKILL && 
			( m_curt_event.status == SM_CHANT_SPELLING || m_curt_event.status == SM_CHANT_READY ) )
		{
			m_SpellEffect.Clear();
		}		
		
		SetQueue( data );
	}
}

void Player::AllActQueue()
{
	
	if(  m_curt_event.type == SM_ATTACK_NPC || 
		( m_curt_event.type == SM_USE_SKILL && m_curt_event.status != SM_CHANT_SPELLING ) )
	{
		SetUnderAttackEventMgr( m_curt_event );
	}
	
	///-- 현재 스펠시전 동작을 하고있었다면... 스필시전 이펙트를 해제해준다
	if( m_curt_event.type == SM_USE_SKILL && 
		( m_curt_event.status == SM_CHANT_SPELLING || m_curt_event.status == SM_CHANT_READY )  )
	{
		m_SpellEffect.Clear();
	}
	
	if( m_next_event.type == SM_ATTACK_NPC || 
		m_next_event.type == SM_USE_SKILL )
	{
		SetQueue( m_next_event );
		m_next_event.Clear();
		SetUnderAttackEventMgr( m_curt_event );
	}
}

bool Player::SM__ATTACK__NPC( EVENT_DATA_INFO& data )
{
	//	RLG1("\n-=-=-SM__ATTACK__NPC-=-=-\n", data.count);
	
	//	SystemPrint( n_DarkRed, n_SystemMsg, "SM__ATTACK__NPC() => data.type = SM_ATTACK_NPC;" );
	data.type = SM_ATTACK_NPC;
	
	if( data.motion == 0 )
		data.motion = n_Attack9_Basic0;
	
	if( data.critical )
	{
		// 아처는 크리티컬 없음
		if( nRui->thePcParam.MainClass != Archer && 
			nRui->thePcParam.MainClass != Sniper )
			data.motion = n_Attack9_Basic4;
	}
	
	int attack_type = 0;
	
	if( data.race < 0 )					// NPC	
	{			
		attack_type	 = n_NPC;
		data.x		 = g_Map.m_NpcTable[data.tbl_Idx].lDestBlock%g_Pc.m_cSize;
		data.y		 = g_Map.m_NpcTable[data.tbl_Idx].lDestBlock/g_Pc.m_cSize;
		data.vPos.x	 = g_Map.m_Npc[data.tbl_Idx].GetPosTM()._41;
		data.vPos.y  = g_Map.m_Npc[data.tbl_Idx].GetPosTM()._42;
		data.vPos.z  = g_Map.m_Npc[data.tbl_Idx].GetPosTM()._43;
		
		SetDirection( data.vPos.x, data.vPos.z );
	}
	else								// 유저
	{
		// 받아치기 당했음 ( 여러명에게 공격시 차후 루틴 고려 By wxywxy )
		if( MAX_USER_INTABLE == data.tbl_Idx )
		{
			attack_type	 = n_PC;
			data.x		 = g_Pc.GetMainPC().lBlock%g_Pc.m_cSize;
			data.y		 = g_Pc.GetMainPC().lBlock/g_Pc.m_cSize;
			data.vPos.x	 = GetPosTM()._41;
			data.vPos.y  = GetPosTM()._42;
			data.vPos.z  = GetPosTM()._43;
		}
		else
		{
			attack_type	 = n_PC;
			data.x		 = g_Pc.m_PcTable[data.tbl_Idx].lDestBlock%g_Pc.m_cSize;
			data.y		 = g_Pc.m_PcTable[data.tbl_Idx].lDestBlock/g_Pc.m_cSize;
			data.vPos.x	 = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._41;
			data.vPos.y  = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._42;
			data.vPos.z  = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._43;
			
			SetDirection( data.vPos.x, data.vPos.z );
		}
	}
	
	//	if( data.motion == n_Attack9_Basic4 )
	//	{
	//		data.motion = n_Attack9_Basic4;
	//	}
	
	//	RLG2( "SM__ATTACK__NPC Animation Num = %d -> %d", m_curt_event.motion, data.motion );
	
	AnimationPlay( data.motion );
	
	m_IsCDataQActed = FALSE;
	
	m_curt_event = data;
	
	return true; 
} 



bool Player::SM__UNDERATTACK( EVENT_DATA_INFO& data )
{
	// 데미지에 따른 ReAction
	// 약, 강, 크리티컬 
	// MaxHealth 20% 이하는 약데미지 이상은 강데미지, 크리티컬은 따로 처리...
	
	/*	int motion = rand()%3;
	
	  if( motion == 0 )
	  motion = n_ReAction_Critical9;
	  else 
	  motion = (rand()%2) ? n_ReAction_LightDmg9 : n_ReAction_StrongDmg9;
	*/
	m_curt_event = data;
	
	// 애니메이션 플레이하기
	if(!m_State.bRide)
	{
		if( AnimationPlay( data.motion ) == -1 )
			//	if( AnimationPlay( motion ) == -1 )
			return false;
	}
	
	return true;
}


bool Player::SM__STATE__DIE( EVENT_DATA_INFO& data )
{
	m_save_event.Clear();
	m_next_event.Clear();
	
	m_curt_event = data;
	
	// 애니메이션 플레이하기
	if( AnimationPlay( data.motion ) == -1 )
		return false;
	
	// 지속성 스킬 모두 지우기
	g_Particle.m_EffectManager.DeleteAllKeepupSkillOnPlayer();
	
	//배틀존이 아닐경우에만 죽을때나오는 윈도우를 뛰워준다.
	if( theMapType == n_FightZone )
	{
		if( !g_BZ_GameResultWnd.IsVisible )
		{
			g_AliveWnd.Open( n_AnswerWndType_Observer_RoomOut );				
		}
	}
	else
	{
		g_AliveWnd.Open( n_AnswerWndType_Alive );				
	}
	
	nRui->SendEvent( WID_ChantHitBar_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	m_SpellEffect.Clear();
	
	return true;
}


int Player::DieProcess()
{		
	float value = SAFE_TIME_SUB( SAFE_TIME_SUB( g_nowTime , m_PreTime ) , NPC_DIE_TIME );
	
	if( SAFE_TIME_COMPARE( m_curt_event.time , <= , g_nowTime ) && m_ani_frame == (int)m_end_frame )
	{
		int	nItmCount;
		
		nItmCount = c_Max_WearItemType;		
		
		for( int i = 0; i < nItmCount; ++i )
		{
			//			if( n_Wear_Serve == i )
			//			{
			//				m_pItm_index[i]	= -1;			
			//				continue;
			//			}
			
			if( m_pMaterial[i].Diffuse.a >= 0.55f )
			{
				value = (float)1.0f-((float)value/(float)NPC_DIE_BLENDTIME);
				m_pMaterial[i].Diffuse.a = value;
			}
		}
		m_bShadowDisplay = false;
	}
	return 0;
}


bool Player::SM__MOVE( EVENT_DATA_INFO& data, bool bFindNpc )
{	
	// 이동할 수 있는 곳인지 검사
	if( FindDestTile( data, bFindNpc ) == true )
	{				
		//-----------------------------------------------------------------------------
		// 도착할 목적지저장	
		//-----------------------------------------------------------------------------
		m_Mov.dest.x = data.x;
		m_Mov.dest.y = data.y;				
		
		//-----------------------------------------------------------------------------
		// 길찾기를 해서 캐릭터 보폭, 회전각을 새로 갱신해야 한다. 		
		//-----------------------------------------------------------------------------
		int count = GetMax(abs((long)m_Mov.dest.x- (long)m_Mov.curt.x), abs((long)m_Mov.dest.y- (long)m_Mov.curt.y));
		if( FindPath( (short)m_Mov.curt.x, (short)m_Mov.curt.y, data, count ) == true )
		{			
			//-----------------------------------------------------------------------------
			// 멈춰있다면
			//-----------------------------------------------------------------------------
			if( m_curt_event.type != SM_MOVE_BROADCAST )
			{		
				// 다음 타일 계산
				float step_x, step_y;
				GetMoveStep( m_Mov.path.x, m_Mov.path.y, m_Mov.curt.x, m_Mov.curt.y, step_x, step_y );
				m_Mov.next.x = m_Mov.curt.x+(int)((step_x*GetRunFrame())/CLIENT_TILESIZE);
				m_Mov.next.y = m_Mov.curt.y+(int)((step_y*GetRunFrame())/CLIENT_TILESIZE);
				
				// 이동타일 검사
				int Res = GetNextPath( m_Mov.curt.x, m_Mov.curt.y, data );
				if( Res == 0 )		// 목표 지점 및 목표물에 다가 왔으니 그만 움직여도 되느니라...
				{
					m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );
					
					// 새로운 공격 대상을 더블 클릭 했을때...
					if( data.type == SM_MOVE_ATTACK )						
					{
						if( m_save_event.race < 0 )
						{
							// 현재 공격중인 대상과 같으면 딜레이 없이 공격을 하므로 못하게 막는다. 
							if( m_curt_event.race < 0 &&							// 새로운 공격 대상이 같은 몬스터이고 
								m_save_event.tbl_Idx == m_curt_event.tbl_Idx )		// 같은 몬스터이면 공격 실패 								
							{
								// 같은 공격 대상은 더블 클릭으로 연속 공격 불가 
							}
							else
							{
								if( m_curt_event.type != SM_ATTACK_NPC ||
									m_curt_event.type != SM_BOW_ATTACK ||
									m_curt_event.type != SM_SLIDE_NPC ||				
									m_curt_event.type != SM_STATE_DIE )			
								{
									//									g_RockClient.Send_RpCsStop( g_Pc.m_MainPC.lDestBlock );
									
									//									SystemPrint( n_DarkRed, n_SystemMsg, "SM_MOVE() => SM_MOVE_ATTACK" );
									CM__ATTACK__NPC( m_save_event );			
								}
							}
						}
						else
						{
							// 현재 공격중인 대상과 같으면 딜레이 없이 공격을 하므로 못하게 막는다. 
							if( m_curt_event.race >= 0 &&							// 새로운 공격 대상이 같은 몬스터이고 
								m_curt_event.tbl_Idx != 0 &&						// 맨처음 공격 시작할때는 되야쥐...								
								m_save_event.tbl_Idx == m_curt_event.tbl_Idx )		// 같은 몬스터이면 공격 실패 								
							{
								// 같은 공격 대상은 더블 클릭으로 연속 공격 불가 
							}
							else
							{
								if( m_curt_event.type != SM_ATTACK_NPC ||
									m_curt_event.type != SM_BOW_ATTACK ||
									m_curt_event.type != SM_SLIDE_NPC ||				
									m_curt_event.type != SM_STATE_DIE )			
								{
									//									g_RockClient.Send_RpCsStop( g_Pc.m_MainPC.lDestBlock );
									
									CM__ATTACK__NPC( m_save_event );			
								}
							}
						}					
					}						
					g_Map.SetMapData( PC_BLOCK, MAX_USER_INTABLE, g_Pc.m_MainPC.lDestBlock, true );
					return false;
				}
			}
			//-----------------------------------------------------------------------------
			// 이동중에 호출된 경우에 사용			
			//-----------------------------------------------------------------------------
			else
			{								
				// 서버에 이동 메세지 날리기				
				//				CM__MOVE( m_Mov.curt.x, m_Mov.curt.y, m_Mov.dest.x, m_Mov.dest.y );
				// 뛰기
				if(!m_State.bRide)
					AnimationPlay( n_Run9 );	
			}			
			
			if( data.type == CM_MOVE_ATTACK )
			{
				if( !theIsAttack )
				{
					if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( theAttackDelayTime , 250 ) ) )
					{
						theIsAttack = true;
						theAttackDelayTime = g_nowTime;
					}
					else
					{
						return false;
					}
				}
				
				data.type = SM_MOVE_ATTACK;
				theAttack_Event = data;				
				Illusion_CsStartMove( data.cell );
				Pet_CsStartMove( data.cell );
				
				theIsAttack = false;
				theAttackDelayTime = g_nowTime;				

				return false;
			}
			
			//-----------------------------------------------------------------------------
			// 도착할 타일의 거리 비교해서.. 3이상이면 bWalk = false.. 뛰기
			//-----------------------------------------------------------------------------
			//			bool bWalk = false;
			//			int x = g_Pc.GetDestX()-data.x;
			//			int y = g_Pc.GetDestY()-data.y;
			//			if( abs(x) < 2 && abs(y) < 2 )
			//				bWalk = true;
			//			g_Pc.CM__TOGGLE__RUN( bWalk );
		}
		//-----------------------------------------------------------------------------
		// 길찾기 실패했다면.. 초기화한다.
		//-----------------------------------------------------------------------------
		else
		{			
			m_curt_event.Clear();
			AnimationPlay( n_Stand_Peace9 );
			m_Mov.curt.x = g_Pc.GetDestX();
			m_Mov.curt.y = g_Pc.GetDestY();
			m_Mov.curt.height = g_Pc.GetMapHeight( m_Mov.curt.x, m_Mov.curt.y );
			m_Mov.path = m_Mov.next = m_Mov.dest = m_Mov.curt;			
			return false;
		}
		
		// 이동준비. 공격단계초기화
		//		m_bWait = false;		
		//		m_AttackType = 0;
		//		m_AttackStep = 1;			
		
		// 이동이벤트 저장
		//		SystemPrint( n_DarkRed, n_SystemMsg, "SM_MOVE() => SM_MOVE_BROADCAST" );
		m_curt_event.type = SM_MOVE_BROADCAST;
		m_curt_event.x    = m_Mov.dest.x;
		m_curt_event.y    = m_Mov.dest.y;
		//m_save_event.Clear();		
	}
	
	return true;
}


//-----------------------------------------------------------------------------
// Desc: 도착가능한 타일인지 검색
//-----------------------------------------------------------------------------
bool Player::FindDestTile( EVENT_DATA_INFO& data, bool bNpc )
{
	// 맨처음 픽킹할때 갈 수 있는 곳으로 1차 클라이언트 허용, 2차 서버에서 허용해서 가야 되는 곳이면 일단 무조건 간다 
	return true;
	
	/*	if( bNpc == false && g_Map.m_Block[data.x][data.y].GetClear() == false )	
	return false;	
	
	  //-----------------------------------------------------------------------------
	  // 클릭한 곳 주변 타일을 검사하여서 이동타일 찾기
	  //-----------------------------------------------------------------------------
	  int		xx		= data.x;
	  int		yy		= data.y;
	  bool	bFind	= false;
	  for( int i = 0; i < 8; ++i )
	  {		
	  // 이동 가능한 타일인가?
	  if( g_Map.m_Block[xx][yy].GetClear() == true )
	  {
	  bFind = true;
	  break;
	  }
	  
		// 주변타일 찾기
		switch( i )
		{
		case 0:
		yy++;
		break;
		case 1:
		xx++;
		break;
		case 2:
		yy--;
		break;
		case 3:
		yy--;
		break;
		case 4:
		xx--;
		break;
		case 5:
		xx--;
		break;
		case 6:
		yy++;
		break;
		case 7:
		yy++;
		break;
		}
		}
		if( bFind == true )
		{
		data.x	= xx;
		data.y	= yy;		
		} 
		return bFind;
	*/
	/*
	// NPC 검색
	if( bNpc == true )
	{		
	// 이동하려는 곳에 있는 타겟이.. 내가 공격하려는 타겟과 같니?
	//if( g_Map.m_Block[data.x][data.y].FindTarget( data.index, data.unique ) == false )
	//{			
	//	if( g_Map.m_Block[x][y].GetClear() == false )
	//		return false;
	//}		
	}
	// 타일검색
	else
	{
	//-----------------------------------------------------------------------------
	// 클릭한 곳이 가지 못하는 곳이라면 false
	//-----------------------------------------------------------------------------	
	if( g_Map.m_Block[data.x][data.y].GetClear() == false )
	return false;
	//-----------------------------------------------------------------------------
	// 클릭한 곳 주변 8타일이 가지 못하는 곳이라면 false
	//-----------------------------------------------------------------------------
	else
	{
	int		xx = data.x;
	int		yy = data.y;
	bool	bFind = false;
	for( int i = 0; i < 8; ++i )
	{		
				// 이동 가능한 곳인지 검사
				if( g_Map.m_Block[xx][yy].GetClear() == true )
				{
				bFind = true;
				break;
				}
				
				  // 해당지점에서 한바퀴돌면서 검색하기
				  switch( i )
				  {
				  case 0:
				  yy++;
				  break;
				  case 1:
				  xx++;
				  break;
				  case 2:
				  yy--;
				  break;
				  case 3:
				  yy--;
				  break;
				  case 4:
				  xx--;
				  break;
				  case 5:
				  xx--;
				  break;
				  case 6:
				  yy++;
				  break;
				  case 7:
				  yy++;
				  break;
				  }
				  }
				  if( bFind == false )
				  return false;
				  }			
				  }	
				  return true;
*/
}


//-----------------------------------------------------------------------------
// Desc: 길찾기 (이동중에 다음 타일을 찾을때)
// 더이상 찾을 타일이 없으면 리턴 0, 
// 현재 타일이 도착지이면 1을 리턴, 
// 현재 타일이 사정거리 안에 있으면 2를 리턴 한다 
// 이동중 CM__MOVE는 여기에서만 날린다 
//-----------------------------------------------------------------------------
int Player::FindNextTile()
{	
	if( !GetNextPath( m_Mov.curt.x, m_Mov.curt.y, m_save_event ) )
	{
		m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );
		CM__MOVE(m_Mov.curt.x, m_Mov.curt.y, m_Mov.curt.x, m_Mov.curt.y);
		m_curt_event.Clear();			
		
		if(!m_State.bRide)
		{
			AnimationPlay( n_Stand_Peace9 );
		}
		else
		{
			AnimationPlay( n_Ride_Wait );
			g_Map.m_Npc[m_State.TransIndex].AnimationPlay( n_NPC_Stand0 );
		}		
		
		// 피킹 이펙트 지우기 By wxywxy
		g_Particle.DelEffSetCon( EF_PICK , 100  , -1 , -1 );
		
		if( m_save_event.type == SM_MOVE_ATTACK )
		{
			//			g_RockClient.Send_RpCsStop( g_Pc.m_MainPC.lDestBlock );
			
			//			SystemPrint( n_DarkRed, n_SystemMsg, "FindNextTile() => SM_MOVE_ATTACK" );
			CM__ATTACK__NPC( m_save_event );						
		}
		g_Map.SetMapData( PC_BLOCK, MAX_USER_INTABLE, g_Pc.m_MainPC.lDestBlock, true );
		return 2;
	}
	// 현재 타일이 도착지이면 1을 리턴하고 길찾기 안함 
	else if( m_Mov.curt.x == m_Mov.dest.x && m_Mov.curt.y == m_Mov.dest.y )
	{
		CM__MOVE(m_Mov.curt.x, m_Mov.curt.y, m_Mov.dest.x, m_Mov.dest.y);
		return 1;
	}
	// 다음타일이 도착타일이면 0을 리턴하고 길찾기 안함 
	else if( m_Mov.next.x == m_Mov.dest.x && m_Mov.next.y == m_Mov.dest.y )
	{
		CM__MOVE(m_Mov.next.x, m_Mov.next.y, m_Mov.dest.x, m_Mov.dest.y);
		return 0;
	}		
	
	// 다음 이동할 타일 검색하기
	int nCount	= GetMax( abs((long)m_Mov.dest.x- (long)m_Mov.curt.x), abs((long)m_Mov.dest.y- (long)m_Mov.curt.y) );
	int nRes	= FindPath( (short)m_Mov.curt.x, (short)m_Mov.curt.y, m_save_event, nCount );
	if( nRes == 1 )
	{		
		//-----------------------------------------------------------------------------
		// 현재위치와 다음위치값을 계산
		//-----------------------------------------------------------------------------
		int nRunFrame	= GetRunFrame();				
		int nCurtFrame	= nRunFrame - m_move_frame;
		if( m_move_frame > nRunFrame )
		{			
			nCurtFrame = (nRunFrame*2) - m_move_frame;
		}
		
		float fCurtX = m_matChr._41+(m_Mov.step_x*nCurtFrame);
		float fCurtY = m_matChr._43+(m_Mov.step_y*nCurtFrame);
		
		// 다음위치
		float	fStepX, fStepY;
		GetMoveStep( m_Mov.path.x, m_Mov.path.y, m_Mov.curt.x, m_Mov.curt.y, fStepX, fStepY );		
		DWORD	dwPosX = (fCurtX+(fStepX*nRunFrame))/CLIENT_TILESIZE;
		DWORD	dwPosY = (fCurtY+(fStepY*nRunFrame))/CLIENT_TILESIZE;
		
		//-----------------------------------------------------------------------------
		// 도착타일과 다음타일의 거리를 계산해서...
		// 1타일이내라면 다음타일은 목적지가 된다
		//-----------------------------------------------------------------------------
		int nRangeX = abs((long)m_Mov.curt.x-(long)m_Mov.dest.x);
		int nRangeY = abs((long)m_Mov.curt.y-(long)m_Mov.dest.y);
		if( nRangeX == 0 && nRangeY == 0 )
			return 0;
		else if( nRangeX <= 1 && nRangeY <= 1 )
		{
			dwPosX = m_Mov.dest.x;
			dwPosY = m_Mov.dest.y;
		}		
		
		//-----------------------------------------------------------------------------
		// 현재 타일과 다음타일을 계산해서..
		// 0타일이면 실패.. 2타일이상이면 중간값으로 보정
		//-----------------------------------------------------------------------------
		nRangeX = abs((long)m_Mov.curt.x-(long)dwPosX);
		nRangeY = abs((long)m_Mov.curt.y-(long)dwPosY);
		if( nRangeX >= 2 || nRangeY >= 2 )
		{
			if( dwPosX > m_Mov.curt.x )
				dwPosX = m_Mov.curt.x + 1;
			else if( dwPosX < m_Mov.curt.x )
				dwPosX = m_Mov.curt.x - 1;
			
			if( dwPosY > m_Mov.curt.y )
				dwPosY = m_Mov.curt.y + 1;
			else if( dwPosY < m_Mov.curt.y )
				dwPosY = m_Mov.curt.y - 1;
		}
		
		CM__MOVE(m_Mov.curt.x, m_Mov.curt.y, dwPosX, dwPosY);
		m_Mov.next.x = dwPosX;
		m_Mov.next.y = dwPosY;			
	}
	return nRes;
}

//-----------------------------------------------------------------------------
// Desc: 다음타일찾기
//-----------------------------------------------------------------------------
int Player::GetNextPath( DWORD& next_x, DWORD& next_y, EVENT_DATA_INFO& data )
{
	//-----------------------------------------------------------------------------
	// 다음동작에 따른 처리
	//-----------------------------------------------------------------------------
	bool bFind = false;
	switch( data.type )
	{		
	case SM_MOVE_ATTACK:		// 일반공격
		{		
			//			int range = 32;
			int range = 4;
			
			SItemBaseInfo* item_Info = NULL;	
			if( m_pUserInfo->bAttack == true )		// 무기 착용시 
			{
				if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] != NULL )
				{
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Weapon]->Code );
					if( item_Info && item_Info->theClass == n_Class_Weapon )
					{
						//						range = (item_Info->theRange + 1) * 8 - (CLIENT_TILESIZE / 2);		// 타일당 8 정도의 실제 사정 거리
						range = item_Info->theRange; 
					}
				}
				else if( nRui->thePcInven.WearItem[n_WearSlot_Shield] != NULL )
				{
					item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code );
					if( item_Info && item_Info->theClass == n_Class_Weapon )
					{
						//						range = (item_Info->theRange + 1) * 8 - (CLIENT_TILESIZE / 2);		// 타일당 8 정도의 실제 사정 거리
						range = item_Info->theRange; 
					}
				}
			}
			
			bFind = FindNpcTile( next_x, next_y, range, data );					
			
			// 이동 공격시 1 타일 변화할때마다 타겟이 이동하였으면 타겟 위치를 재설정한다. 
			if( bFind == false )
			{
				CheckTargetTile( data );		// 처음 설정되어 있는 이동 공격 타겟이 이동하였는지 체크한다. 
			}
		}
	}
	
	if( bFind == true )
	{
		m_Mov.path = m_Mov.dest = m_Mov.next;
		return 0;
	}
	
	//-----------------------------------------------------------------------------
	// 다음타일이 이동불가능한 타일일 경우는...
	// A*를 이용해서 다음 타일 찾기
	//-----------------------------------------------------------------------------	
	/*	if( g_Map.m_Block[next_x][next_y].GetClear() == false )		
	{		
	// A* 길찾기
	short	x, y;
	if( GotoPath( m_Mov.next.x, m_Mov.next.y, m_Mov.path.x, m_Mov.path.y, &x, &y ) == true )
	{			
	next_x	= x;
	next_y	= y;
	}
	else
	{
	m_curt_event.Clear();
	m_save_event.Clear();
	return -1;
	}
} */
	return 1;
}

void Player::CheckTargetTile( EVENT_DATA_INFO& data )
{		
	int nPosX, nPosY;
	int nTarget = 0;
	int real_range = 0;
	
	if( data.race < 0 )				// n_NPC 
	{
		// NPC찾기
		nTarget = g_Map.FindNpc( data.unique );	
		if( nTarget == MAX_NPC_INTABLE )
			return;
		
		nPosX = g_Map.m_NpcTable[nTarget].lDestBlock%g_Pc.m_cSize;
		nPosY = g_Map.m_NpcTable[nTarget].lDestBlock/g_Pc.m_cSize;
		
		real_range = GetMax( abs((long)nPosX - (long)data.x ), abs((long)nPosY - (long)data.y ) );
		
		if(  real_range >= 4 && 
			( m_curt_event.type && m_curt_event.type != SM_MOVE_BROADCAST ) )	// 4셀이상 위치 변경시 재 공격 
			//		if( data.x != nPosX || data.y != nPosY )
		{
			EVENT_DATA_INFO attackNPC_event;
			attackNPC_event.type = CM_MOVE_ATTACK;
			attackNPC_event.motion = n_Attack9_Basic0;
			attackNPC_event.tbl_Idx = data.tbl_Idx;
			attackNPC_event.race = data.race;
			attackNPC_event.index = data.index;
			attackNPC_event.unique = data.unique;
			attackNPC_event.x = nPosX;
			attackNPC_event.y = nPosY;
			attackNPC_event.vPos.x = g_Map.m_Npc[nTarget].GetPosTM()._41;
			attackNPC_event.vPos.y = g_Map.m_Npc[nTarget].GetPosTM()._42;
			attackNPC_event.vPos.z = g_Map.m_Npc[nTarget].GetPosTM()._43;
			attackNPC_event.cell = g_Map.m_NpcTable[nTarget].lDestBlock; 
			g_Pc.GetPlayer()->PushQueue( attackNPC_event, TRUE );
			g_Pc.GetPlayer()->PetSetAttackQueue( attackNPC_event, false );
		}
	}
	else							// n_PC 
	{
		if( data.unique == g_Pc.m_MainPC.lUnique )		
			return;	
		else
		{
			// 유저찾기
			int	nTarget = g_Pc.FindUser( data.unique );
			if( nTarget == MAX_USER_INTABLE )
				return;
			
			// 위치저장
			nPosX = g_Pc.m_PcTable[nTarget].lDestBlock%g_Pc.m_cSize;
			nPosY = g_Pc.m_PcTable[nTarget].lDestBlock/g_Pc.m_cSize;
			
			real_range = GetMax( abs((long)nPosX - (long)data.x ), abs((long)nPosY - (long)data.y ) );
			
			if( real_range >= 4 && 
				( m_curt_event.type && m_curt_event.type != SM_MOVE_BROADCAST ) )
				//		 	if( data.x != nPosX || data.y != nPosY )
			{
				EVENT_DATA_INFO attackPC_event;
				attackPC_event.type = CM_MOVE_ATTACK;
				attackPC_event.motion = n_Attack9_Basic0;
				attackPC_event.tbl_Idx = data.tbl_Idx;
				attackPC_event.race = data.race;
				attackPC_event.index = data.index;
				attackPC_event.unique = data.unique;
				attackPC_event.x = nPosX;
				attackPC_event.y = nPosY;
				attackPC_event.vPos.x = g_Pc.m_Pc[nTarget].GetPosTM()._41;
				attackPC_event.vPos.y = g_Pc.m_Pc[nTarget].GetPosTM()._42;
				attackPC_event.vPos.z = g_Pc.m_Pc[nTarget].GetPosTM()._43;
				attackPC_event.cell = g_Pc.m_PcTable[nTarget].lDestBlock; 
				g_Pc.GetPlayer()->PushQueue( attackPC_event, TRUE );
				g_Pc.GetPlayer()->PetSetAttackQueue( attackPC_event, false );
			}
		}
	}
	
	return;
}

#define		MAX_MOVE_RANGE	16		// 시야선택타일거리

bool Player::FindPath( short now_x, short now_y, EVENT_DATA_INFO& data, int count )
{	
	//-----------------------------------------------------------------------------
	// 거리체크하기
	//-----------------------------------------------------------------------------
	if( count >= g_Pc.GetMapSize() )			
		return false;
	
	//-----------------------------------------------------------------------------
	// 길찾기시작
	//-----------------------------------------------------------------------------	
	theFindWay->StartPos.X	= now_x;
	theFindWay->StartPos.Y	= now_y;
	theFindWay->EndPos.X	= m_Mov.dest.x;
	theFindWay->EndPos.Y	= m_Mov.dest.y;	
	theFindWay->_FindWay();
	
	// 목적지 변경이 있으면 동기가 맞지 않을 수도 있지만...지금까진 변경없이 다 찾고 있네...ㅡㅡ;
	// 카엠에선 캐릭터 위치를 보내주는 데로 움직이니까 길찾기를 안해도 되지만...우리는 다 찾아야 되넹...ㅋ
	// 목적지가 변경되었다
	if( theFindWay->GoalReached == 0 )
	{
		m_Mov.dest.x = theFindWay->NearPos.X;
		m_Mov.dest.y = theFindWay->NearPos.Y;
	}
	// 목적지가 변경되었다
	if( theFindWay->ChangedEndPos == TRUE )
	{
		//		RLG4( "목적지 변경(ChangedEndPos) m_Mov.dest.x=%d, y=%d, theFindWay->NearPos.X=%d, Y=%d", m_Mov.dest.x, m_Mov.dest.y, theFindWay->NearPos.X, theFindWay->NearPos.Y );
		
		m_Mov.dest.x = theFindWay->EndPos.X;
		m_Mov.dest.y = theFindWay->EndPos.Y;
	} 
	// 직선 이동
	if( theFindWay->DirectMove == TRUE )
	{				
		m_Mov.path.x = theFindWay->EndPos.X;
		m_Mov.path.y = theFindWay->EndPos.Y;
	}
	// 길찾기 이동
	else
	{
		m_Mov.path.x = theFindWay->JuncPos.X;
		m_Mov.path.y = theFindWay->JuncPos.Y;
	}
	//	DebugPrint("\r\n 시작타일 %d %d (길찾기 타일 %d %d)", now_x, now_y, m_Mov.path.x, m_Mov.path.y );
	
	return true;
}


//----------------------------------------------------------------------------
// Desc: 서버에 메세지 보내기
//-----------------------------------------------------------------------------
bool Player::CM__MOVE( int _nCurtX, int _nCurtY, int _nNextX, int _nNextY )
{		
	//-----------------------------------------------------------------------------
	// 다음타일이동 쏘는게 현재 맵타일보다 큰값을 보낸다
	//-----------------------------------------------------------------------------
	if( _nNextX >= g_Pc.GetMapSize() || _nNextY > g_Pc.GetMapSize() )
	{
		return false;
	}
	
	//-----------------------------------------------------------------------------			
	// 서버에 이동할 다음 타일 날리기
	//-----------------------------------------------------------------------------			
	// 이전, 다음 블럭 계산
	long lBlock		= (_nCurtY*g_Pc.GetMapSize())+_nCurtX;
	long lDestBlock = (_nNextY*g_Pc.GetMapSize())+_nNextX;
	long lPathBlock = (m_Mov.path.y*g_Pc.GetMapSize())+m_Mov.path.x;
	//	long ReallBlock	= (m_Mov.curt.y*g_Pc.GetMapSize())+m_Mov.curt.x;
	
	
	/// sooree network 
#ifdef	NETWORK_CLIENT
	// 서버에 보내는 것은 다음 타일을 보내줘서 미리 감지할 수 있게 하여 오차를 줄인다 
	// (Next)갈 곳을 미리 보내주다가 나중에는 (Curt)현재 위치를 보내준다 
	
	///--JAPAN_BUG_MODIFY 
	///-- CHECK_CSMOVETIME이 경과한 후에야 한번씩 보내준다.
	///-- 이부분을 서버에서 체크 스피드핵을 쓰지않았는데도 너무 이동속도가 빠르면
	///-- 스피드핵으로 간주하기 때문
	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( timeGetTime() , m_PreSendCsMoveTime ) , >= , CHECK_CSMOVETIME ) )
	{
		g_RockClient.Send_RpCsMove( g_Pc.m_MainPC.lUnique, lDestBlock, m_Mov.now_Yrad );
		m_PreSendCsMoveTime  = timeGetTime();
	}
	
	//	RLG3( "*****CsMove : lDestBlock = %d (%u, %u)*****", lDestBlock, _nNextX, _nNextY );
	//	g_RockClient.Send_RpCsMove( lBlock );
	//	RLG3( "*****CsMove : lBlock = %d (%u, %u)*****", lBlock, _nCurtX, _nCurtY );
	
	// 길찾기와 동기화가 좀 안 맞는 부분이 있는것은 추후 수정...ㅡㅡ;;
	// 실제적으로 이동한 현재 위치를 보내준다 
	//	g_RockClient.Send_RpCsMove( ReallBlock );
	//	RLG3( "*****CsMove : ReallBlock = %d (%u, %u)*****", ReallBlock, m_Mov.curt.x, m_Mov.curt.y );
#endif
	
	// 현재 바라보는각
	int nAngle = GetAngle( lBlock, lDestBlock );
	if( -1 != nAngle )
		m_byAngle = nAngle;
	
	//	RLG1( "CM__MOVE : m_Mov.now_Yrad = %3.3f", m_Mov.now_Yrad );
	//	RLG1( "CM__MOVE : m_byAngle = %d", m_byAngle );
	
	// 블럭값 테이블에 저장.
	g_Pc.SetlBlock( lBlock );
	g_Pc.SetlDestBlock( lDestBlock );
	g_Pc.lReceiveBlock = lDestBlock;		// 임시로 NPC 테스트 하기 위해서..
	
	// 말탔다면 말위치와 각도갱신
	//	if( true == m_State.bRide && -1 != m_State.horse_index )
	//	{
	//		g_Map.m_Npc[m_State.horse_index].m_byAngle			= m_byAngle;
	//		g_Map.m_NpcTable[m_State.horse_index].lDestBlock	= lDestBlock;
	//	}
	// 합체한 상태
	//	if( -1 != m_State.Unite_unique )
	//	{
	//		int nUser = g_Pc.FindUser( m_State.Unite_unique );
	//		if( MAX_USER_INTABLE == nUser )
	//			return true;
	//		g_Pc.m_Pc[nUser].m_byAngle			= m_byAngle;
	//		g_Pc.m_PcTable[nUser].lDestBlock	= lDestBlock;
	//	}
	return true;
}


bool Player::CM__READY__WARP( long aNowTime, NAnswerWarp aMapType )
{
	theMapType = aMapType;
	theWarpTime = aNowTime;
	
	return true;
}

//-----------------------------------------------------------------------------
// Desc: 공격
//-----------------------------------------------------------------------------
void Player::AttackSelectProcess()
{	
	return;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Desc: 공격하려는 대상이 살아있나? 내 공격범위 안에 있나?
//-----------------------------------------------------------------------------
bool Player::FindNpcTile( int _nCurtX, int _nCurtY, int _nRange, EVENT_DATA_INFO& data )
{
	//-----------------------------------------------------------------------------
	// 타겟의 위치
	//-----------------------------------------------------------------------------
	int nPosX, nPosY;
	BYTE byBlock = 0;
	float distance = 0.0f;

	///--JAPAN_BUG_MODIFY
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	if( data.race < 0 )
	{
		// NPC찾기
		int nTarget = 0;
		if( data.race == c_NpcRace_Item )
		{
			nTarget = g_Map.FindNpc( data.unique, c_NpcRace_Item );	
			if( nTarget == MAX_NPC_INTABLE )
				return false;
		}
		else
		{
			nTarget = g_Map.FindNpc( data.unique );	
			if( nTarget == MAX_NPC_INTABLE )
				return false;
		}
		
		// 블럽타입, 위치값저장
		//		byBlock	= g_Pc_Manager.GetNpcBlockType( nTarget );
		vPos.x = g_Map.m_Npc[nTarget].GetPosTM()._41;
		vPos.z = g_Map.m_Npc[nTarget].GetPosTM()._43;

		nPosX = (DWORD)( vPos.x / (float)CLIENT_TILESIZE );
		nPosY = (DWORD)( vPos.z / (float)CLIENT_TILESIZE );
		
		// 아이템일 경우엔 가까이 가서 집어야 하므로 거리 재 설정 
		if( data.race == c_NpcRace_Item )
		{
			//			_nRange = CLIENT_TILESIZE * 4;
			_nRange = 4;
		}
		else
		{
			if( IsNonPlayerCode( g_Map.m_NpcTable[nTarget].code ) )
			{
				if( g_Map.m_NpcTable[nTarget].code == 5301 )		// 천년의 나무는 좀 더 멀리서 ^ ^;;
				{
					_nRange = 20;
				}
				else
				{
					//					_nRange = CLIENT_TILESIZE * 10;
					_nRange = 10;
				}
			}
			else if( g_Map.IsGiantUnit( &g_Map.m_Npc[nTarget] ) )
			{
				///-- By simwoosung
				///-- 이거 이대로 적용하면... 거리가 너무 멀리 떨어져잇다고 나옴
				///-- 서버쪽과 맟춰주는 작업이 필요할듯....				
				int nNpcSize = g_Map.m_NpcTable[nTarget].theNpcSize;
				int nAdd = nNpcSize - 3;							
				_nRange += nAdd;
			}
		}
	}
	else
	{
		if( data.unique == g_Pc.m_MainPC.lUnique )		
			return true;	
		else
		{
			// 유저찾기
			int	nTarget = g_Pc.FindUser( data.unique );
			if( nTarget == MAX_USER_INTABLE )
				return false;
			
			// 위치저장
			vPos.x = g_Pc.m_Pc[nTarget].GetPosTM()._41;
			vPos.z = g_Pc.m_Pc[nTarget].GetPosTM()._43;

			nPosX = (DWORD)( vPos.x / (float)CLIENT_TILESIZE );
			nPosY = (DWORD)( vPos.z / (float)CLIENT_TILESIZE );	
		}
	}
	
	//	if( distance < (float)_nRange )
	//	{
	//		return true;
	//	}
	
	int real_range = GetMax( abs((long)nPosX - (long)m_Mov.curt.x ), abs((long)nPosY - (long)m_Mov.curt.y ) );

	int nComRange = _nRange;
	nComRange -= ( data.nADisErrCnt * 2 );
	nComRange = max(1, nComRange);

	if( real_range <= nComRange )
	{		
		if( data.nADisErrCnt )
		{
			///--PRINT_DLG_LOG( "Range Test Error Count %d", data.nADisErrCnt );
		}
		
		return true;
	}	

	return false;
}



//-----------------------------------------------------------------------------
// Desc: 말위치에 따라서 이동할 곳 찾기
//-----------------------------------------------------------------------------
bool Player::GetHorsePos( int& x, int& y, int index )
{
	//-----------------------------------------------------------------------------
	// 말 현재위치
	//-----------------------------------------------------------------------------
	x = g_Map.m_NpcTable[index].lDestBlock%g_Pc.m_cSize;
	y = g_Map.m_NpcTable[index].lDestBlock/g_Pc.m_cSize;
	
	//-----------------------------------------------------------------------------
	// 거리계산해서 1타일 이내라면 성공
	//-----------------------------------------------------------------------------
	if( m_curt_event.type == NULL )
	{
		int nRangeX = x-m_Mov.curt.x;
		int nRangeY = y-m_Mov.curt.y;					
		if( abs(nRangeX) <= 1 && abs(nRangeY) <= 1 )
		{
			if( !(abs(nRangeX) == 1 && abs(nRangeY) == 1) )
			{	
				x = m_Mov.curt.x;
				y = m_Mov.curt.y;
				return true;
			}			
		}
	}
	
	//-----------------------------------------------------------------------------
	// 말 위치, 방향에 따라서 도착타일값 구하기
	//-----------------------------------------------------------------------------
	int nAngle = GetAngle( m_Mov.curt.x, m_Mov.curt.y, x, y );	
	int nDest1 = x, nDest2 = y;
	g_EtcTable.GetHorseRidePos( g_Map.m_Npc[index].m_byAngle, nAngle, nDest1, nDest2, g_Pc.m_cSize );
	
	//-----------------------------------------------------------------------------
	// 얻어낸 두타일중에서 가까운곳부터 검색하기
	//-----------------------------------------------------------------------------
	int nRange1 = GetMax( abs((int)m_Mov.curt.x-nDest1%g_Pc.m_cSize), abs((int)m_Mov.curt.y-nDest1/g_Pc.m_cSize) );
	int nRange2 = GetMax( abs((int)m_Mov.curt.x-nDest2%g_Pc.m_cSize), abs((int)m_Mov.curt.y-nDest2/g_Pc.m_cSize) );
	if( nRange1 > nRange2 )
	{
		int nTemp = nDest1;
		nDest1 = nDest2;
		nDest2 = nTemp;
	}	
	if( g_Map.m_Block[nDest1%g_Pc.m_cSize][nDest1/g_Pc.m_cSize].GetClear() == true )	
	{
		x = nDest1%g_Pc.m_cSize;
		y = nDest1/g_Pc.m_cSize;
		return true;
	}
	else if( g_Map.m_Block[nDest2%g_Pc.m_cSize][nDest2/g_Pc.m_cSize].GetClear() == true )	
	{
		x = nDest2%g_Pc.m_cSize;
		y = nDest2/g_Pc.m_cSize;
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------
// Desc: 말처리
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Desc: 그림자 만들기
//-----------------------------------------------------------------------------
BOOL	Player::ShadowMake()
{
	if( g_Shadow == 2 && !m_Shadow.m_IsInitTex )
	{
		m_Shadow.Clear();
		m_Shadow.Create(m_nShadowSize);
		m_Shadow.m_fCumulativeTime = 0.0f;
		return FALSE;
	}		
	
	if( g_Shadow == 2 && m_Shadow.pRenderToSurface == NULL )
	{
		m_bShadowDisplay = false;
		return FALSE;
	}
	
	///-- 본에 연결된 매트릭스를 갱신하는 AniRender()함수가 Display 함수 이전에 존재하기 때문에
	///-- 본에 연결된 매트릭스가 엇갈리게 된다. 이를 여기서 다시 구한다.
	if( g_Shadow == 2 )
	{
		BoneRender();
	}
	
	// 라이트에서 본 뷰메트릭스
	D3DXMATRIX	matCharLocalToLight;
	D3DXMATRIX	matShadow;
	D3DXVECTOR4	vOut;
	D3DXVECTOR3 vPos;
	m_Shadow.fXMax = 0.0f; 
	m_Shadow.fYMax = 0.0f;
	
	D3DXMATRIX	matTemp;
	
	///-- 시선의 위치를 구해Boa요
	if( g_Shadow == 2 )	
	{	// 프로젝션그림자
		vPos = D3DXVECTOR3(m_matChr._41+m_vLight.x, m_matChr._42+m_vLight.y, m_matChr._43+m_vLight.z);	
	}
	else
	{	// 동그란그림자	
		vPos = D3DXVECTOR3(m_matChr._41, m_matChr._42+20.0f, m_matChr._43-1.0f);
	}	
	
	///-- 눈, 대상, 업벡터 - 뷰벡터를 구한다.
	D3DXMatrixLookAtLH( &m_Shadow.matWorldToLight, &vPos,
		&D3DXVECTOR3(m_matChr._41, m_matChr._42, m_matChr._43), 
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
	
	if(m_State.bRide)
	{
		matTemp = m_matRideChr;		
	}
	else
	{
		matTemp = m_matChr;
	}		
	
	///-- 스케일 	
	///-- 여기 구해지는 matLocalToLight는 캐릭터 자신이다.   
	D3DXMATRIX	matScale;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z );	
	D3DXMatrixMultiply( &matTemp, &matScale, &matTemp );
	
	// 로컬라이트
	D3DXMatrixMultiply( &matCharLocalToLight, &matTemp, &m_Shadow.matWorldToLight );
	
	// 프로젝션
	if( g_Shadow == 2 && m_IsApplyVA )
	{		
		///-- 실제 그림자를 그릴때 적용되는 프로젝션 행렬을 구한다.
		D3DXMatrixPerspectiveFovLH( &m_Shadow.matShadowProj, D3DX_PI/4, 1.3f, 1.0f, 10000.0f );
		
		int itm_count = 0;
		
		itm_count = g_Pc_Manager.GetItmCount( this );
		
		for( int i = 0; i < itm_count; ++i )
		{					
			//..........................................................................................
			// 셋트 아이템 하의 부츠 장갑 막기 by simwoosung
			//..........................................................................................
			if(  CHAR_WEAR_SETITEM == m_Char_WearType  )
			{
				if( n_Wear_PantsArmor == i || n_Wear_Gloves == i || n_Wear_Shoes == i )
				{
					continue;
				}
			}			
			
			for( int j = 0; j < g_Pc_Manager.GetVtxCount( this, i ); ++j )
			{									
				D3DXVec3Transform( &vOut, &GetVertex( i, j ), &matCharLocalToLight );				
				
				if( (float)fabs(vOut.x/vOut.z) > m_Shadow.fXMax )
				{
					m_Shadow.fXMax = (float)fabs(vOut.x/vOut.z);					
				}
				
				if( (float)fabs(vOut.y/vOut.z) > m_Shadow.fYMax )
				{
					m_Shadow.fYMax = (float)fabs(vOut.y/vOut.z);
				}				
			}
		}
		
		if(m_State.bRide)	///-- 라이딩 중이면 프로젝션 행렬에 라이딩 버텍스도 적용한다.
		{
			g_Map.m_Npc[m_State.TransIndex].ApplyShaowProj(m_Shadow);
		}
		
		///-- 소지 무기도 프로젝션 행렬에 버텍스를 적용시킨다.
		//.................................................................................................
		// 그림자 표시여부 ( 무기류 ) By wxywxy
		//.................................................................................................
		static BOOL ISDisplayWeaponShadow = TRUE;
		
		ISDisplayWeaponShadow = TRUE;
		
		switch( m_ani_index )
		{
			//.............................................................................................
			// 명상시 칼 무기 안함
			//.............................................................................................
		case n_Skill_RestSitdown:
		case n_Skill_RestKepping:
		case n_Skill_RestStandup:
			
			ISDisplayWeaponShadow = FALSE;
			
			break;
			
		default:	
			break;
			
		}
		
		if(m_State.bRide)
			ISDisplayWeaponShadow = FALSE;
		
		if( m_curt_event.type == SM_USE_SKILL )
		{
			switch( m_curt_event.status )
			{
			case SM_SELLSKILL_SITDOWN:
			case SM_SELLSKILL_KEEPING:
			case SM_SELLSKILL_STANDUP:
				
				ISDisplayWeaponShadow = FALSE;
				
				break;
			}
		}
		
		///-- 무기 그림자가 그려지면....
		if( ISDisplayWeaponShadow )
		{
			// 무기의 버텍스를 프로젝션에 적용시킨다.
			ApplyWeaponShaowProj();
		}
		
		///-- 링크 이펙트 아이템의 버텍스를 프로젝션에 적용시킨다.
		if(!m_State.bRide)
		{
			std::list	<CEffectBase *> ::iterator	x = m_EffectList.begin();
			
			for( ; x != m_EffectList.end() ; ++x )
			{
				if( (*x)->ISLinkItem() )
				{
					(*x)->ApplyShaowProj();
				}
			}
		}
		
		///-- 최종 프로젝션 행렬을 구한다.
		// 32 = 92, 64 = 96, 128 = 98
		if( m_Shadow.m_ShadowSize == 32 )
		{
			m_Shadow.matShadowProj._11 = 0.92f/m_Shadow.fXMax;
			m_Shadow.matShadowProj._22 = 0.92f/m_Shadow.fYMax;	
		}
		else if( m_Shadow.m_ShadowSize == 64 )
		{
			m_Shadow.matShadowProj._11 = 0.96f/m_Shadow.fXMax;
			m_Shadow.matShadowProj._22 = 0.96f/m_Shadow.fYMax;
		}
		else
		{
			m_Shadow.matShadowProj._11 = 0.98f/m_Shadow.fXMax;
			m_Shadow.matShadowProj._22 = 0.98f/m_Shadow.fYMax;
		}
		
		//-----------------------------------------------------------------------------
		// 렌더링
		//-----------------------------------------------------------------------------		
		
		
		if( SUCCEEDED(m_Shadow.pRenderToSurface->BeginScene(m_Shadow.pSurface, NULL)) )
		{								
			g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 0.0f, 0L );
			
			g_RenderManager.SetTransform( D3DTS_VIEW,	&m_Shadow.matWorldToLight );
			g_RenderManager.SetTransform( D3DTS_PROJECTION,	&m_Shadow.matShadowProj );	
			
			if(m_State.bRide)
			{
				g_Map.m_Npc[m_State.TransIndex].RenderShadow(m_Shadow);				
			}
			
			g_RenderManager.SetTransform( D3DTS_WORLD,	&matTemp );	
			
			//-----------------------------------------------------------------------------
			// 몸통
			//-----------------------------------------------------------------------------		
			int mod_index = 0;			
			int itm_count = 0;
			
			itm_count = g_Pc_Manager.GetItmCount( this );			
			int i = 0;
			for(  i = 0; i < itm_count; ++i )
			{						
				//..........................................................................................
				// 셋트 아이템 하의 부츠 장갑 그려주기 막기 by simwoosung
				//..........................................................................................
				if(  CHAR_WEAR_SETITEM == m_Char_WearType  )
				{
					if( n_Wear_PantsArmor == i || n_Wear_Gloves == i || n_Wear_Shoes == i )
					{
						continue;
					}
				}				
				
				if( g_Pc_Manager.GetVtxCount( this, i ) == 0 )
					continue;
				
				mod_index = g_Pc_Manager.m_pItm[m_pItm_index[i]]->m_mod_index;				
				g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );				
				g_RenderManager.SetStreamSource( 0, m_pVtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );
				DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
					g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
			}
			
			if( ISDisplayWeaponShadow )
			{
				D3DXMATRIX	matItemLocalToLight;
				
				//-----------------------------------------------------------------------------		
				// 링크된 무기 
				//-----------------------------------------------------------------------------		
				for( i = 0; i < LINK_ITEM_COUNT; ++i )
				{
					if( m_pUserInfo->ArmorInfo.index[i] != -1 )
					{
						if( i == 1 )
						{
							int itm_index = m_pUserInfo->ArmorInfo.index[i];
							if( g_Pc_Manager.m_pItm[itm_index]->m_class	== 1 && 
								g_Pc_Manager.m_pItm[itm_index]->m_type	== 3 )
								continue;
						}
						
						if( m_curt_event.type == SM_ACT )
						{
							if( !m_curt_event.weaponView[i] )
							{
								continue;
							}
						}
						
						// 메트릭스
						D3DXMATRIX matWorld;
						if( i == 0 )
							matWorld = m_matRHand;
						else if( i == 1 )
							matWorld = m_matLHand;	
						
						// 스케일					
						D3DXMatrixMultiply( &matWorld, &matScale, &matWorld );						
						// 현재라이트위치

						g_RenderManager.SetTransform( D3DTS_WORLD,	&matWorld );
						
						// 렌더링
						mod_index = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[i]]->m_mod_index;										
						g_RenderManager.SetStreamSource( 0, g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[i]]->m_pNormalVB, sizeof(OBJECTVERTEX) );
						g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );	
						
						DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
							g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );		
						
					}
				}
			}
			
			//.................................................................................................
			// 링크 이펙트 그림자 찍어주기
			//.................................................................................................
			if(!m_State.bRide)
			{
				std::list	<CEffectBase *> ::iterator	x = m_EffectList.begin();
				
				for( ; x != m_EffectList.end() ; ++x )
				{
					if( (*x)->ISLinkItem() )
					{
						(*x)->RenderShadow();
					}
				}
			}
			
			g_Camera.ReloadProjTM();
#ifdef DIRECT_VERSION_9_MJH
			m_Shadow.pRenderToSurface->EndScene( D3DX_FILTER_LINEAR );
#else
			m_Shadow.pRenderToSurface->EndScene();
#endif // DIRECT_VERSION_9_MJH			
		}		
		
		g_Camera.ReloadProjTM();
		
		if(!m_Shadow.m_IsRender)
		{
			if(!m_Shadow.m_IsOneTimeRender)
			{
				m_Shadow.m_IsOneTimeRender = true;
			}
			else
			{
				m_Shadow.m_fCumulativeTime += g_GTimer.GetFrameElapsedSecTime();
			}

			if( m_Shadow.m_fCumulativeTime >= Shadow_ReNewTime )
			{
				m_Shadow.m_fCumulativeTime = 0.0f;
				m_Shadow.m_IsRender = true;
			}
		}
	}
	
	return TRUE;
}

void	Player::ApplyWeaponShaowProj()
{
	D3DXMATRIX	matItemLocalToLight;
	D3DXVECTOR4 vOut;
	
	///-- 스케일 	
	///-- 여기 구해지는 matLocalToLight는 캐릭터 자신이다.   
	D3DXMATRIX	matScale;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z );
	
	//-----------------------------------------------------------------------------		
	// 링크된 무기 
	//-----------------------------------------------------------------------------		
	for(int i = 0; i < LINK_ITEM_COUNT; ++i )
	{
		if( m_pUserInfo->ArmorInfo.index[i] != -1 )
		{
			if( i == 1 )
			{
				int itm_index = m_pUserInfo->ArmorInfo.index[i];
				if( g_Pc_Manager.m_pItm[itm_index]->m_class	== 1 && 
					g_Pc_Manager.m_pItm[itm_index]->m_type	== 3 )
					continue;
			}
			
			if( m_curt_event.type == SM_ACT )
			{
				if( !m_curt_event.weaponView[i] )
				{
					continue;
				}
			}
			
			// 메트릭스
			D3DXMATRIX matWorld;
			if( i == 0 )
				matWorld = m_matRHand;
			else if( i == 1 )
				matWorld = m_matLHand;
			
			// 스케일					
			D3DXMatrixMultiply( &matWorld, &matScale, &matWorld );			
			// 현재라이트위치
			D3DXMatrixMultiply( &matItemLocalToLight, &matWorld, &m_Shadow.matWorldToLight );	
			
			
			for( int j = 0; j < g_Pc_Manager.GetVtxCount( this, i, 1 ); ++j )
			{									
				D3DXVec3Transform(	&vOut, &g_Pc_Manager.GetLinkVertex( m_pUserInfo->ArmorInfo.index[i], j ), 
					&matItemLocalToLight );
				
				if( (float)fabs(vOut.x/vOut.z) > m_Shadow.fXMax )
				{
					m_Shadow.fXMax = (float)fabs(vOut.x/vOut.z);
				}
				
				if( (float)fabs(vOut.y/vOut.z) > m_Shadow.fYMax )
				{
					m_Shadow.fYMax = (float)fabs(vOut.y/vOut.z);
				}				
			}
		}
	}
}

void	Player::ShadowDisplay()
{		
	if( m_bShadowDisplay == false )
		return;
	
	
	D3DXMATRIX	matOffset, matCameraToWorld;						
	D3DXMatrixInverse( &matCameraToWorld, NULL, &g_Camera.m_matView );
	D3DXMatrixIdentity( &matOffset );
	matOffset._11 =  0.5f;
	matOffset._22 = -0.5f;
	matOffset._31 =  0.5f;
	matOffset._32 =  0.5f;
	
	// 텍스쳐행렬만들기
	D3DXMatrixMultiply( &m_Shadow.matShadowUV, &matCameraToWorld, &m_Shadow.matWorldToLight );			
	
	if( g_Shadow == 2 )
	{
		D3DXMatrixMultiply( &m_Shadow.matShadowUV, &m_Shadow.matShadowUV, &m_Shadow.matShadowProj );
		if(!m_Shadow.m_IsRender)
		{
			return;
		}
	}
	else
	{
		D3DXMatrixMultiply( &m_Shadow.matShadowUV, &m_Shadow.matShadowUV, &g_Pc.m_ShadowTable.matMiddle );
	}
	
	D3DXMatrixMultiply( &m_Shadow.matShadowUV, &m_Shadow.matShadowUV, &matOffset );	
	
	// 텍스쳐 행렬
	g_RenderManager.SetTransform( D3DTS_TEXTURE0, &m_Shadow.matShadowUV );
	
	// 렌더링
	if( g_Shadow == 2 )
	{
		int block_size = 1;
		
		if(m_State.bRide)
		{
			SNPCBaseInfo* npc_Info = NULL;
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_State.TransIndex].code ); 
			
			if( npc_Info )
			{
				block_size = npc_Info->theSize;
			}
		}
		
		g_RenderManager.SetTexture( 0, m_Shadow.pTex );		
		g_Map.DisplayShadowTile( (int)(m_matChr._41/8), (int)(m_matChr._43/8), block_size ); 
	}
	else
	{
		g_RenderManager.SetTexture( 0, g_Pc.m_TexTable.pShadow );	
		g_Map.DisplayShadowTile2( (int)(m_matChr._41/8), (int)(m_matChr._43/8) );
	}	
}



//-----------------------------------------------------------------------------
// Desc: 바라보는 방향으로 머리 회전하기
//-----------------------------------------------------------------------------
void	Player::SetHeadRotate()
{	
	//-----------------------------------------------------------------------------
	// 게임모드가 노말이 아니라면... 리턴
	//-----------------------------------------------------------------------------
	
	//-----------------------------------------------------------------------------			
	// 애니메이션 인덱스
	//-----------------------------------------------------------------------------			
	int	ani;
	if( m_nTableIndex == MAX_USER_INTABLE )
		ani = g_Pc.m_MainPC.nIndex;
	else
		ani = g_Pc.m_PcTable[m_nTableIndex].nIndex;
	
	//-----------------------------------------------------------------------------
	// 현재 타겟위치 찾기	
	//-----------------------------------------------------------------------------			
	bool bFind = false;
	float x, y;
	switch( m_curt_event.type )
	{
	case NULL:
		bFind = true;
		break;
	case SM_MOVE_BROADCAST:
		bFind = true;
		break;
	case MODE_DOWN:
		bFind = true;
		break;
	}
	if( bFind == true )
	{
		// NPC		
		if( g_Map.m_nPickedNpc != -1 )
		{
			bFind = true;
			x = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._41;
			y = g_Map.m_Npc[g_Map.m_nPickedNpc].GetPosTM()._43;
		}
		// PC
		else if( g_Pc.m_nPickedUser != -1 && g_Pc.m_nPickedUser != MAX_USER_INTABLE )
		{
			bFind = true;
			x = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._41;
			y = g_Pc.m_Pc[g_Pc.m_nPickedUser].GetPosTM()._43;
		}
		// ITEM
		else
		{
			bFind = false;
		}
	}
	
	
	//-----------------------------------------------------------------------------			
	// 찾지못했다면... 리턴
	//-----------------------------------------------------------------------------			
	if( bFind == false )
	{
		if( m_pUserInfo->HeadCurFrame != (g_Pc_Manager.m_HeadAni[ani].m_ani_frame+1)/2 )
		{
			m_pUserInfo->HeadTime		= g_nowTime;
			m_pUserInfo->HeadPreFrame	= m_pUserInfo->HeadCurFrame;
			m_pUserInfo->HeadCurFrame	= (g_Pc_Manager.m_HeadAni[ani].m_ani_frame+1)/2;
			
			//-----------------------------------------------------------------------------	
			// 서버에 방송하기
			//-----------------------------------------------------------------------------
		}	
		return;
	}
	
	
	//-----------------------------------------------------------------------------	
	// 해당 위치의 라디안값	
	//-----------------------------------------------------------------------------	
	x -= m_matChr._41;
	y -= m_matChr._43;
	float rad	= atan2(y, x);			
	int frame	= (rad+m_Mov.now_Yrad)/0.01745f;
	
	// 90도 이상일 경우...	
	if( frame > 90 )		
		frame = 0;
	
	// 값이 크거나.. 작을 경우 값 보정	
	else
	{
		frame = (frame/10)+g_Pc_Manager.m_HeadAni[ani].m_ani_frame;
		if( frame >= g_Pc_Manager.m_HeadAni[ani].m_ani_frame )
			frame = g_Pc_Manager.m_HeadAni[ani].m_ani_frame-1;
		else if( frame < 0 )
			frame = 0;
	}
	
	//-----------------------------------------------------------------------------
	// 현재랑 같이 다를 경우... 프레임 저장한다
	//-----------------------------------------------------------------------------	
	if( m_pUserInfo->HeadCurFrame != frame )
	{					
		//-----------------------------------------------------------------------------	
		// 이전 프레임, 다음 프레임 저장
		//-----------------------------------------------------------------------------	
		m_pUserInfo->HeadTime		= g_nowTime;
		m_pUserInfo->HeadPreFrame	= m_pUserInfo->HeadCurFrame;
		m_pUserInfo->HeadCurFrame	= frame;
		
		//-----------------------------------------------------------------------------	
		// 서버에 방송하기
		//-----------------------------------------------------------------------------
	}		
}


void Player::AniProcess()
{
	// 맵 로딩후 NPC 및 다른 유저 정보 받는 중에 맞아서 죽어 버리므로 약간의 간격을 둔다 
	if( theWarpTime != 0 )
	{
		DWORD dExitCode = g_CLoadingBarThread.getExitCode();
		
		if( STILL_ACTIVE != dExitCode && g_RockClient.m_MapChangeProgress != true )
		{
			if( theMapType == n_FightZone )
			{
				g_RockClient.Send_CsFightReadyWarp();
				theWarpTime = 0;				
			}
			else
			{
				g_RockClient.Send_RpCsReadyWarp();
				theWarpTime = 0;
			}		
		}
	}
	
	//-----------------------------------------------------------------------------
	// 멈췄다가 이동을 해서 동기화타임체크할 경우
	//-----------------------------------------------------------------------------
	if( m_bLatency == true )
	{		
		if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_PreMTime , MOVE_LATENCY_TIME ) ) )
		{
			// 시간저장, 플래그 변경
			m_PreMTime  = g_nowTime;
			m_bLatency  = false;	
			
			// 이동
			AnimationPlay( n_Run9 );		
			
			// 회전하기, 높이보폭값
			SetRotate( m_Mov.path );
			SetHeightStep();
			SetMoveStep( m_Mov.path );
			return;
		}
	}	
	
	float ani_time = 0.0f;
	ani_time = GetAniTime();
	
	// 이동 프레임 업데이트	

	///-- ani_time이 float 형이므로 비교계산이 불가능하다.
	if(  SAFE_TIME_SUB( g_nowTime , m_PreMTime ) >= ani_time  )						
	{
		//		int add_frame = (int)((g_nowTime-m_PreTime) / c_MoveTime );
		float fAdd_Frame = (float)(SAFE_TIME_SUB( g_nowTime , m_PreMTime ) / ani_time );
		int add_frame = (int)fAdd_Frame;
		
		for( int i = 0; i < add_frame; ++i ) 
		{				
			m_move_middle_time = 0.0f; 
			
			//m_move_frame += add_frame;
			++m_move_frame;
			
			// 보간없이 (false) 캐릭터이동하기			
			AniProcessMove( false );		
			AniProcessMoveEvent();
			
			if( m_move_frame >= GetRunFrame() )
				m_move_frame = 0;
			
		}

		float fRemain =  (float)SAFE_TIME_SUB( g_nowTime , m_PreMTime ) - (float)(add_frame * ani_time);
		float fRatio =  (float)fRemain / ani_time;
		fRatio = max( 0.0f, fRatio );

		if( m_curt_event.type == SM_MOVE_BROADCAST )
		{			
			MoveRatio( fRatio );
			AniProcessMoveEvent();
			g_Camera.Update();
		}		

		m_PreMTime = g_nowTime;
		
		//add_frame = (int)(SAFE_TIME_SUB( g_nowTime , m_PreTime ) / c_MoveTime );
		///-- add_frame = (int)( (g_nowTime-m_PreTime) / ( c_MoveTime * m_ani_percent ) );
		
		///--JAPAN_BUG_MODIFY
		Rotate( fRatio, add_frame );		
	}
	else
	{
		m_move_middle_time = SAFE_TIME_SUB( g_nowTime , m_PreMTime ) / ani_time;			
		
		// 보간하여 (true) 캐릭터이동하기
		AniProcessMove( true );
	}

	if( g_RockClient.m_SceneManager.GetSceneMode() != SCENE_SQUENCE )
	{
		m_matChr._42 = g_Map.PHF_GetHeight( m_matChr._41 , m_matChr._43 );
	}
	
	//	float ani_time = 0.0f;
	//	ani_time = GetAniTime();
	
	if( m_OneTimeJump )
	{
		m_OneTimeJump = false;
		m_PreTime = g_nowTime;
		return;
	}
	
	//-----------------------------------------------------------------------------
	// 다음 애니프레임 그리기(보간X)
	//-----------------------------------------------------------------------------
	if( SAFE_TIME_SUB( g_nowTime , m_PreTime ) >= ani_time  )
	{
		//-----------------------------------------------------------------------------
		// 애니메이션처리(스킵프레임만큼)
		//-----------------------------------------------------------------------------
		// 스킵프레임 얻어내기		
		int skip_count = (int)(SAFE_TIME_SUB( g_nowTime , m_PreTime ) / ani_time );
		//for( int i = 0; i < skip_count; ++i )
		{
			if( m_curt_event.type == SM_STATE_DIE && m_curt_event.status == SM_STATE_DIE &&
				m_ani_frame == (int)m_end_frame )
			{
				int Res = DieProcess();
			}
			else
			{
				//-----------------------------------------------------------------------------
				// 사운드 플레이
				//-----------------------------------------------------------------------------
				for( int i = 0; i < skip_count; ++i )
				{
					//					m_ani_frame += skip_count;				
					++m_ani_frame;
					++m_blend_frame;
					
					// 블렌딩시 중복 m_ani_frame Skip 처리
					///-- 현재 블렌딩 중이라면
					if( m_blend_frame < c_MotionBlendFrame )
					{
						if( m_ani_frame < c_MotionBlendHalfFrame + m_start_blend_frame )
						{
							UpdateAniProcess();		
						}
						else if( ( m_ani_frame == c_MotionBlendHalfFrame + m_start_blend_frame) 
							&& ( m_blend_frame == c_MotionBlendHalfFrame ) )
						{
							UpdateAniProcess();		
						}
						else
						{
							m_ani_frame = c_MotionBlendHalfFrame + m_start_blend_frame;
						}
					}
					else
					{
						UpdateAniProcess();		
					}
				}	
				
				//-----------------------------------------------------------------------------
				// 예외처리하는지 검사하기
				//-----------------------------------------------------------------------------
				if( UpdateFrame() == false )
				{					
					if( m_ani_frame == (int)((m_end_frame - c_MotionBlendFrame )/2)+1 || 
						m_ani_frame > (int)m_end_frame - c_MotionBlendFrame )
					{
						// 이벤트처리
						AniProcessEvent();
						
						// 애니메이션 끝났다면 첫프레임으로(-1로 해야 보간되어서 첫프레임이 된다)
						if( m_ani_frame > (int)m_end_frame )
						{
							if( m_curt_event.type == SM_STATE_DIE )
								m_ani_frame	= m_end_frame;		
							else 
								m_ani_frame	= 0;											
						}
						
					}
				}
			}
		}
		//-----------------------------------------------------------------------------
		// 초기화
		//-----------------------------------------------------------------------------
		m_step_time    = 0.0f;	
		m_middle_time  = 0.0f;			
		if( !(m_curt_event.type == SM_STATE_DIE && m_ani_frame == (int)m_end_frame) )
		{			
			DWORD AniTime = c_MoveTime;
			
			if( m_curt_event.type == SM_MOVE_BROADCAST )
			{
				AniTime = ( c_MoveTime * m_ani_percent );
			}
			else
			{
				AniTime = ani_time;
			}
			
			if( AniTime == 0 )
				m_PreTime = g_nowTime;
			else
				m_PreTime = g_nowTime-( (int)SAFE_TIME_SUB( g_nowTime , m_PreTime ) % AniTime );
		}
	}	
	// 보간할 경우
	else
	{
		// 보간타임저장		
		m_middle_time = SAFE_TIME_SUB( g_nowTime , m_PreTime ) / ani_time;
	}
	
	//-----------------------------------------------------------------------------
	// 바라보는 곳으로 머리회전하기
	//-----------------------------------------------------------------------------
	//	SetHeadRotate();
	g_chr_anicount++;
	
	if(m_State.bRide)
	{
		g_Map.m_Npc[m_State.TransIndex].SetPosTM(GetPosTM());		
		g_Map.m_Npc[m_State.TransIndex].AniProcess(true);		
	}	
}


//-----------------------------------------------------------------------------
// Desc: 이동처리
//-----------------------------------------------------------------------------
void Player::AniProcessMove( bool middle )
{	
	int Res = 1;
	
	//-----------------------------------------------------------------------------
	// 이동
	//-----------------------------------------------------------------------------
	if( m_curt_event.type == SM_MOVE_BROADCAST && m_bLatency == false )
	{	
		// 말타고 있다면 먼지 날리기
		/*if( m_State.bRide == true && m_ani_frame%2 == 0 )
		{
		D3DXVECTOR3 tempv;
		tempv = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41, g_Map.m_fBaseHeightWater, g_Pc.GetPlayer()->GetPosTM()._43 );
		// 땅이 물이 아니면 물튀기자. 
		if( g_Map.m_pAttrMap[m_Mov.curt.y][m_Mov.curt.x].byType == MATYPE_WATER )
		g_Particle.DisplayUpdate( 55, tempv );
		else
		g_Particle.DisplayUpdate( 6, g_Pc.m_MainPC.lUnique, -1, -1 );
	}*/
		
		if( m_ani_frame < c_MotionBlendFrame )
			m_move_middle_time = 0.0f;
		
		//-----------------------------------------------------------------------------
		// 캐릭터이동
		//-----------------------------------------------------------------------------
		if( middle == false )
		{			
			// 다음 타일로 넘어가는 순간에 길찾기 (1타일에 한번씩 길찾기를 한다.) 			
			if( m_move_frame == (GetRunFrame()/2+1) )
			{												
				// 다음타일 길찾기 
				Res = FindNextTile();
				switch( Res )
				{
				case -1:
					{			// 서버에 도착한 곳 알려주기
					}
					break;
				case 0:			// 다음 타일이 도착지 일때...
					{	
					}
				case 1:			// 현재 타일이 도착지 일때...
					{
					}
					break;
				case 2:			// 이동중에 사정거리 안에 들어왔을때...
					{
					}
					break;
				}	
			}
			//-----------------------------------------------------------------------------
			// 정상이동
			//-----------------------------------------------------------------------------
			Move( m_move_middle_time );
		}
		//-----------------------------------------------------------------------------
		// 보간이동
		//-----------------------------------------------------------------------------
		else
		{			
			MiddleMove( m_move_middle_time );				
		}
		
		g_Camera.Update();
	}	
}

//-----------------------------------------------------------------------------
// Desc: 이벤트 처리
//-----------------------------------------------------------------------------
bool Player::AniProcessMoveEvent()
{
	long ReallBlock	= 0;
	
	//-----------------------------------------------------------------------------
	// 현재 이벤트 처리
	//-----------------------------------------------------------------------------
	switch( m_curt_event.type )
	{
	case SM_MOVE_BROADCAST:
		{
			// 현재타일저장	
			m_Mov.curt.x = (int)( (m_matChr._41) / CLIENT_TILESIZE );
			m_Mov.curt.y = (int)( (m_matChr._43) / CLIENT_TILESIZE );
			//ReallBlock	= (m_Mov.curt.y*g_Pc.GetMapSize())+m_Mov.curt.x;
			//g_Pc.SetlBlock( ReallBlock );
			g_Map.UpdateTerrain( m_Mov.curt.x, m_Mov.curt.y );
			
			// 목적지에 도착
			if( m_Mov.ValueUpdate() == true && m_move_frame == GetRunFrame() )
			{
				// 피킹 이펙트 지우기 By wxywxy
				g_Particle.DelEffSetCon( EF_PICK , 100  , -1 , -1 );
				
				//				m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );				
				SetHeightStep( true );
				//				g_Camera.Update();
				
				if( m_save_event.type != SM_MOVE_BROADCAST )					
				{					
					CM__MOVE( m_Mov.curt.x, m_Mov.curt.y, m_Mov.curt.x, m_Mov.curt.y );
					m_curt_event.Clear();	
					
					if(!m_State.bRide)
					{
						AnimationPlay( n_Stand_Peace9 );
					}
					else
					{
						AnimationPlay( n_Ride_Wait );
						g_Map.m_Npc[m_State.TransIndex].AnimationPlay( n_NPC_Stand0 );
					}

					///-- JAPAN_BUG_MODIFY - 이부분에서 스탑을 날려주면... 짧은거리를 이동시
					///-- ScStop이 날라와서 버벅대는 현상이 생긴다. 해서 무브패킷으로 보냄					
					g_RockClient.Send_RpCsMove( g_Pc.m_MainPC.lUnique, 
												g_Pc.m_MainPC.lDestBlock, g_Pc.GetPlayer()->m_Mov.now_Yrad );

					
					if( m_save_event.type == SM_MOVE_ATTACK )
					{
						//						g_RockClient.Send_RpCsStop( g_Pc.m_MainPC.lDestBlock );
						
						//						SystemPrint( n_DarkRed, n_SystemMsg, "AniProcessMoveEvent() => SM_MOVE_ATTACK" );
						CM__ATTACK__NPC( m_save_event );						
					}
				}
				
				g_Map.SetMapData( PC_BLOCK, MAX_USER_INTABLE, g_Pc.m_MainPC.lDestBlock, true );
			}
			// 도착하지 않았다면
			else
			{						
				// 애니메이션 플레이 설정. 걷기,뛰기가 변경되었을 수 있기 때문에
				if(!m_State.bRide)
				{
					AnimationPlay( n_Run9 );
				}
				else
				{
					AnimationPlay( n_Ride_Run );
					g_Map.m_Npc[m_State.TransIndex].AnimationPlay( n_NPC_Run0 );
				}
				
				// 목적지에 도착했는데, 그 사이에 새로운 목적지가 생신된 경우
				if( m_Mov.curt.x == m_Mov.path.x && m_Mov.curt.y == m_Mov.path.y )
				{
					// 다음타일 검색						
					if( FindNextTile() == false )							
					{
						// 피킹 이펙트 지우기 By wxywxy
						g_Particle.DelEffSetCon( EF_PICK , 100  , -1 , -1 );						
						
						m_curt_event.Clear();
						m_save_event.Clear();						
						
						if(!m_State.bRide)
						{
							AnimationPlay( n_Stand_Peace9 );
						}
						else
						{
							AnimationPlay( n_Ride_Wait );
							g_Map.m_Npc[m_State.TransIndex].AnimationPlay( n_NPC_Stand0 );
						}

					///-- JAPAN_BUG_MODIFY - 이부분에서 스탑을 날려주면... 짧은거리를 이동시
					///-- ScStop이 날라와서 버벅대는 현상이 생긴다. 해서 무브패킷으로 보냄					
					g_RockClient.Send_RpCsMove( g_Pc.m_MainPC.lUnique, 
												g_Pc.m_MainPC.lDestBlock, g_Pc.GetPlayer()->m_Mov.now_Yrad );

					}
				}
				// 다음타일 이동값		
				SetMoveStep( m_Mov.path );
				SetRotate( m_Mov.path );
				SetHeightStep();
			}	
		}
		break;
	}	
	
	return true;
}



//-----------------------------------------------------------------------------
// Desc: 이벤트 처리
//-----------------------------------------------------------------------------
bool Player::AniProcessEvent()
{	
	EVENT_DATA_INFO queue_event;
	
	// 일단 딜레이만 없애보자...ㅡㅡ;
	//	m_Weapon_DelayTime = 500;
	
	//-----------------------------------------------------------------------------
	// 시야에 있는 유저, NPC 검색하기
	//-----------------------------------------------------------------------------
	if( m_curt_event.type != SM_MOVE_BROADCAST )
	{		
		g_Pc.UpdateUserTable();
		g_Map.UpdateNpc();		
	}
	
	long lDelayTime = m_Weapon_DelayTime;
	long lAttackGapTime = 0;
	long lAttackAddTime = 0;

#ifdef CONTROL_ATTACK_VELOCITY
	
	m_aLastAttackSendTime = 800;
		
	switch( g_Pc.m_MainPC.char_info.theMainClass )
	{
		case Slayer:
			{
				m_aLastAttackSendTime = 900;
			}
			break;
		case Archer:
		case Sniper:
			{
				m_aLastAttackSendTime = 1500;
			}
			break;
	}	
	
	lAttackGapTime = SAFE_TIME_SUB( m_Attack_DelayTime , m_aLastAttackSendTime );
	lAttackAddTime = (long)m_aMinAttackDelayTime - lAttackGapTime;

	lAttackAddTime = max( 0, lAttackAddTime );

#endif
	
	if( AniDemo )	/// 임시. 데모
	{
		if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
		{
			int ani_index[15] = {0,};
			
			if( Ani_Type == 0 )
			{
				int combo_index[15] = {	n_Attack0_KeyZ0,
					n_Attack0_KeyZ1,
					n_Attack0_KeyZ2,
					n_Attack0_KeyZ3,
					n_Attack0_KeyZ4,
					n_Attack0_KeyX0,
					n_Attack0_KeyX1,
					n_Attack0_KeyX2,
					n_Attack0_KeyX3,
					n_Attack0_KeyX4,
					n_Attack0_KeyC0,
					n_Attack0_KeyC1,
					n_Attack0_KeyC2,									
					n_Attack0_KeyC3,
					n_Attack0_KeyC4 };
				
				memcpy( ani_index, combo_index, sizeof(int)*15 );
			}
			else if( Ani_Type == 1 )
			{
				int combo_index[15] = {	n_Attack1_KeyZ0,
					n_Attack1_KeyZ1,
					n_Attack1_KeyZ2,
					n_Attack1_KeyZ3,
					n_Attack1_KeyZ4,
					n_Attack1_KeyX0,
					n_Attack1_KeyX1,
					n_Attack1_KeyX2,
					n_Attack1_KeyX3,
					n_Attack1_KeyX4,
					n_Attack1_KeyC0,
					n_Attack1_KeyC1,
					n_Attack1_KeyC2,									
					n_Attack1_KeyC3,
					n_Attack1_KeyC4 };
				
				memcpy( ani_index, combo_index, sizeof(int)*15 );
			}
			else if( Ani_Type == 2 )
			{
				int combo_index[15] = {	n_Attack2_KeyZ0,
					n_Attack2_KeyZ1,
					n_Attack2_KeyZ2,
					n_Attack2_KeyZ3,
					n_Attack2_KeyZ4,
					n_Attack2_KeyX0,
					n_Attack2_KeyX1,
					n_Attack2_KeyX2,
					n_Attack2_KeyX3,
					n_Attack2_KeyX4,
					n_Attack2_KeyC0,
					n_Attack2_KeyC1,
					n_Attack2_KeyC2,									
					n_Attack2_KeyC3,
					n_Attack2_KeyC4 };
				
				memcpy( ani_index, combo_index, sizeof(int)*15 );
			}
			
			static int ani_num = 0;
			ani_num = ++ani_num % 15;
			
			//		m_next_event = m_curt_event;
			//		m_next_event.motion = ani_index[ani_num];
			
			AnimationPlay( ani_index[ani_num] );
			
			m_bSoundPlay1 = false;
			m_bSoundPlay2 = false;
			return true;
		}
		else
			return true;
	}
	
	if( m_curt_event.type == SM_MOVE_BROADCAST )
	{
		m_SimTime = g_nowTime;
	}
	else if( m_curt_event.type == SM_BOW_ATTACK )
	{
		if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
		{
			if( m_curt_event.status == SM_BOW_READY )
			{
				m_curt_event.status = SM_BOW_WAIT;
				m_curt_event.motion = n_Attack10_BowWait0; 
				AnimationPlay( m_curt_event.motion );
				SetQueue(m_curt_event);
			}
			else if( m_curt_event.status == SM_BOW_WAIT )
			{
				AnimationPlay( m_ani_index );
			}
		}		
		
		m_SimTime = g_nowTime;
	}
	else if( m_curt_event.type == SM_ATTACK_NPC )
	{
		if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
		{
			BOOL IsArcherReady = FALSE;

			if(  m_ani_index == n_Attack10_BowReady0 )
			{
				IsArcherReady = TRUE;
			}
		
			if( nRui->m_Combo2DTimingEffect.IsActive() &&
				STATE_COMBO_NONE == nRui->m_Combo2DTimingEffect.GetComboState() )
			{
				theActionSystem.InitComboEnable();
				theCombo_State = n_Combo_Disable;
				nRui->m_Combo2DTimingEffect.SetComboState(STATE_COMBO_TIMEOVER);				
			}			
			
			if( IsArcherReady )
			{
				AnimationPlay( n_Attack10_BowShoot0 );
			}
			else
			{
				if( m_next_event.type == SM_ATTACK_NPC ||	 //다음동작 공격동작 또는
					m_next_event.type == SM_USE_SKILL ||  
					m_next_event.type == SM_MOVE_BROADCAST )    //스킬 사용동작
				{
					//				CM__ATTACK__NPC( m_next_event );
					//				SM__ATTACK__NPC( m_next_event );
					//				DebugPrint("\n콤보 플레이\n");
					RLGS( "Set Next Event SM_ATTACK_NPC Or SM_USE_SKILL" );
					
					SetQueue( m_next_event );				
					m_next_event.Clear();
				}
				else
				{			
			//		m_curt_event.Clear();					
					queue_event = m_curt_event;
					queue_event.type = SM_ATTACK_DELAY;
					SetQueue( queue_event );
					m_Attack_DelayTime = g_nowTime;	
#ifdef ROCKCLIENT_DEV
					PRINT_DLG_LOG("Ready attack delay");
#endif 
					if(!m_State.bRide)
					{
						AnimationPlay( n_Stand_Battle9 );
					}					
				}
			}

			m_SimTime = g_nowTime;
		}
	}	
	else if( m_curt_event.type == SM_UNDERATTACK )
	{
		if( m_save_event.type == SM_ATTACK_DELAY )
		{
			long lCompareTime = m_Attack_DelayTime + lAttackAddTime;
			
			if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( lDelayTime , lCompareTime ) ) )		// 기본 평균 1.5초의 대기 시간
			{
				if( theTAtSave_Event.type == CM_MOVE_ATTACK )
				{
					PushQueue( theTAtSave_Event, TRUE );
					PetSetAttackQueue( theTAtSave_Event, false );

					theTAtSave_Event.Clear();
					m_save_event.Clear();
				}
				else
				{
					m_save_event.count = 0;
					m_save_event.motion = n_Attack9_Basic0;					
					
					CM__ATTACK__NPC( m_save_event );
				}

				m_SimTime = g_nowTime;
			}
		}
		
		if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
		{
			if( m_save_event.type == SM_ATTACK_DELAY )
			{
				m_save_event.count = 0;							// count 를 스킬 코드와 같이 쓰기 때문에 기본공격 보낼때는 초기화 시켜줘야 함 
				m_save_event.motion = n_Attack9_Basic0;				
				
				CM__ATTACK__NPC( m_save_event );
				
				//				SetQueue(m_save_event);
				//				m_save_event.Clear();
				m_next_event.Clear();				
			}
			else
			{
				m_curt_event.Clear();
			}			
			
			if(!m_State.bRide)
			{
				AnimationPlay( n_Stand_Battle9 );
			}
			
			m_SimTime = g_nowTime;
		}
	}
	else if( m_curt_event.type == SM_STATE_DIE )
	{ 
		if( m_ani_frame > (int)m_end_frame )
		{	
			m_ani_frame = (int) m_end_frame;
			
			m_curt_event.time = SAFE_TIME_ADD( g_nowTime , NPC_DIE_TIME );
			m_curt_event.status = SM_STATE_DIE;
			
			m_save_event.Clear();
			m_next_event.Clear();
			
			D3DXVECTOR3 vPos( GetPosTM()._41 , GetPosTM()._42 , GetPosTM()._43 );
			
			//			NPC 죽을때 이펙트
			//			g_Particle.SetEffSetCon( 0.0f , EF_DIE_NPC , 100 , 1.4f , -1 , -1 , vPos );
		}
		m_SimTime = g_nowTime;
	}	
	else if( m_curt_event.type == SM_SLIDE_NPC )
	{ 
		if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
		{	
			m_curt_event.Clear();
			m_save_event.Clear();
			m_next_event.Clear();			
			AnimationPlay( n_Stand_Battle9 );	
			m_SimTime = g_nowTime;
		}
	}
	else if( m_curt_event.type == SM_USE_SKILL )
	{
		if( m_curt_event.status == SM_CHANT_READY )
		{
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				m_curt_event.status = SM_CHANT_SPELLING;		
				
				m_curt_event.motion = SetChantMotion( SM_CHANT_SPELLING, 0 );				
				
				SetQueue(m_curt_event);
				//				m_curt_event.Clear();
				m_SimTime = g_nowTime;
			}
		}
		else if( m_curt_event.status == SM_CHANT_SPELLING )
		{
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				if( m_next_event.type == SM_USE_SKILL )
				{
					SetQueue( m_next_event );				
					m_next_event.Clear();
					
					// 영창 끝
					m_SpellEffect.Clear();
				}
				else
				{
					//						int rand_ani_idx = m_ani_index;// + rand()%5;
					//						AnimationPlay( rand_ani_idx );
					AnimationPlay( m_ani_index );
					m_SimTime = g_nowTime;					
				}
			}

		}
		else if( m_curt_event.status == SM_RESTSKILL_SITDOWN )
		{
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				m_curt_event.status = SM_RESTSKILL_KEEPING;
				m_curt_event.motion = n_Skill_RestKepping;			// 명상 유지 모션 
				SetQueue(m_curt_event);
				//				m_curt_event.Clear();
				m_SimTime = g_nowTime;
			}
		}
		else if( m_curt_event.status == SM_RESTSKILL_KEEPING )
		{
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				AnimationPlay( m_ani_index );
				m_SimTime = g_nowTime;
			}
		}
		else if( m_curt_event.status == SM_RESTSKILL_STANDUP )
		{
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				m_curt_event.Clear();			
				AnimationPlay( n_Stand_Battle9 );	
				m_SimTime = g_nowTime;
				
				if( g_Pc.GetPlayer()->m_next_event.type == SM_MOVE_ATTACK )
				{
					g_Pc.GetPlayer()->SetQueue( g_Pc.GetPlayer()->m_next_event );
					g_Pc.GetPlayer()->m_next_event.Clear();
				}
				else
				{
					if( g_Pc.m_MainPC.lDestBlock != g_Pc.GetPlayer()->m_next_event.cell )
					{
						g_Map.SetMapData( CLEAR_BLOCK, MAX_USER_INTABLE, g_Pc.m_MainPC.lDestBlock, true );
						
						/// 예약된 위치와 응답이 같은 경우에만 처리 
						if( g_Pc.GetPlayer()->GetInputedDestCell() == g_Pc.GetPlayer()->m_next_event.cell )
						{
							g_Map.SetupPickedX( g_Pc.GetPlayer()->m_next_event.cell % g_Pc.m_cSize );
							g_Map.SetupPickedY( g_Pc.GetPlayer()->m_next_event.cell / g_Pc.m_cSize );
							
							g_Pc.SetTile();
						}
					}
					
					g_Pc.GetPlayer()->SetInputedDestCell( INVALID_DWORD_VALUE );			
				}
				
				theIsDrawWeapon = true;
				theDrawWeaponFadeTime = g_nowTime;
				g_Particle.DelEffSetCon( EF_REST_BREATH , 0 , GetUnique() , GetIndex() );
			}
		}
		else if( m_curt_event.status == SM_SELLSKILL_SITDOWN )
		{
			m_SimTime = g_nowTime;
			
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				m_curt_event.status = SM_SELLSKILL_KEEPING;
				m_curt_event.motion = n_Store_sit1;
				SetQueue(m_curt_event);
			}
			
		}
		else if( m_curt_event.status == SM_SELLSKILL_KEEPING )
		{
			m_SimTime = g_nowTime;
			
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				AnimationPlay( m_ani_index );
			}
		}
		else if( m_curt_event.status == SM_SELLSKILL_STANDUP )
		{
			m_SimTime = g_nowTime;
			
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				m_curt_event.Clear();			
				AnimationPlay( n_Stand_Peace9 );	
				theIsDrawWeapon       = true;
				theDrawWeaponFadeTime = g_nowTime;
			}
		}
		else if( m_curt_event.status == SM_USE_SKILL )
		{
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				if(!m_IsApplyConAniSkill)
				{
					if( m_next_event.type == SM_USE_SKILL ||
						m_next_event.type == SM_ATTACK_NPC ||
						m_next_event.type == SM_MOVE_BROADCAST )
					{
						SetQueue( m_next_event );				
						m_next_event.Clear();
					}
					else
					{
						SSkillBaseInfo* skill_Info;
						skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( m_curt_event.count );
						if( skill_Info == NULL )
							return false;
						
						g_Pc.GetSkillSystem()->theIsUsedSkill = FALSE;
						map < DWORD, long >::iterator iPos = 
						g_Pc.GetSkillSystem()->UseSkillList.find((DWORD)m_curt_event.count);	
						if(iPos != g_Pc.GetSkillSystem()->UseSkillList.end())
						{
							g_Pc.GetSkillSystem()->UseSkillList.erase(iPos);
						}
						
						if( ( skill_Info->theSkillType2 == n_BadSupport ||
							  skill_Info->theSkillType2 == n_DeathBlow ||
							  skill_Info->theSkillType2 == n_AttackMagic || 
							  skill_Info->theSkillType2 == n_NoFrameDeathBlow ) 
							&& ( skill_Info->theActTarget == n_AT_TARGET )
						 ///-- 발구르기 같은 자기 주변의 대상타겟 스킬을 사용할시에는 
						 ///-- 좀더 디테일한 룰의 적용이 필요할 듯하다.
						 ///-- || skill_Info->theActTarget == n_AT_SELFLAND ) 
							)
						{
							queue_event = m_curt_event;
							queue_event.type = SM_ATTACK_DELAY;		
							
							if( NonPlayerInfoWnd.thePickedNpcID != -1 )
							{
								queue_event.tbl_Idx = NonPlayerInfoWnd.thePickedNpcID;
								queue_event.race	= g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].race;
								queue_event.index	= g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].index;
								queue_event.unique  = g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lUnique;
								queue_event.x		= g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lDestBlock%(g_Map.m_cSize*2);
								queue_event.y		= g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lDestBlock/(g_Map.m_cSize*2);
								queue_event.vPos.x  = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._41;
								queue_event.vPos.y  = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._42;
								queue_event.vPos.z  = g_Map.m_Npc[NonPlayerInfoWnd.thePickedNpcID].GetPosTM()._43;
								queue_event.cell	= g_Map.m_NpcTable[NonPlayerInfoWnd.thePickedNpcID].lDestBlock; 
								
								SetQueue( queue_event );
								PetSetAttackQueue( queue_event );
								
							}
							else if( NonPlayerInfoWnd.thePickedUserID != -1 && NonPlayerInfoWnd.thePickedUserID != MAX_USER_INTABLE)
							{
								queue_event.tbl_Idx = NonPlayerInfoWnd.thePickedUserID;
								queue_event.race	= g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].char_info.race;
								queue_event.index	= g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].nIndex;
								queue_event.unique  = g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lUnique;
								queue_event.x		= g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lDestBlock%(g_Map.m_cSize*2);
								queue_event.y		= g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lDestBlock/(g_Map.m_cSize*2);
								queue_event.vPos.x  = g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._41;
								queue_event.vPos.y  = g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._42;
								queue_event.vPos.z  = g_Pc.m_Pc[NonPlayerInfoWnd.thePickedUserID].GetPosTM()._43;
								queue_event.cell	= g_Pc.m_PcTable[NonPlayerInfoWnd.thePickedUserID].lDestBlock; 
								
								SetQueue( queue_event );
								PetSetAttackQueue( queue_event );
							}
							else
							{
								m_curt_event.Clear();
							}
							
							m_Attack_DelayTime = g_nowTime;
							
							m_save_event.Clear();
							m_next_event.Clear();			
							
							if(!m_State.bRide)
							{
								AnimationPlay( n_Stand_Battle9 );
							}						
						}
						else
						{					
							m_curt_event.Clear();
							m_save_event.Clear();
							m_next_event.Clear();			
						}
						
						m_SimTime = g_nowTime;
					}
				}
			}
		}
	}
	else if( m_curt_event.type == SM_ATTACK_DELAY )
	{
		// 공격시간이 되었는지 확인하고 되었으면 공격 시작 
		//long attack_delay_time = g_Pc_Manager.GetWeaponDelayTime(); 
		//RLGS( "Wait SM_ATTACK_DELAY!!!!" );		
		//long lGapTime = SAFE_TIME_SUB( g_nowTime , m_Attack_DelayTime )
		long lCompareTime = m_Attack_DelayTime + lAttackAddTime;

		if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( lDelayTime , lCompareTime ) ) )		// 기본 평균 1.5초의 대기 시간
		{
		//	PRINT_DLG_LOG("SM_ATTACK_DELAY NPC->SM_ATACK_DEALY ");
			PRINT_DLG_LOG("Delay Auto Attack Time %d", g_nowTime - (lDelayTime + lCompareTime ) );			
			//if( g_Pc.GetSkillSystem()->theIsUsedSkill == FALSE )
			{
				//by Simwoosung
				if( STATE_COMBO_SUCCESS != nRui->m_Combo2DTimingEffect.GetComboState() )
				{
					if( theTAtSave_Event.type == CM_MOVE_ATTACK )
					{
						PushQueue( theTAtSave_Event, TRUE );
						PetSetAttackQueue( theTAtSave_Event, false );

						theTAtSave_Event.Clear();
						m_curt_event.Clear();
					}
					else
					{
						m_curt_event.motion = n_Attack9_Basic0;
						m_curt_event.count = 0;
						m_curt_event.number = 0;
						m_curt_event.status = n_NotCombo;
					
						CM__ATTACK__NPC( m_curt_event );
					}						
				}
				else 
				{
					//by sdh 확실하지가 않타 .. 너무 복잡하다 ㅠㅠ OTL.. 
					// 하지만 방법이 없다면 .. 
					// 대상체가 존재 하지 않는다.. 
					//몹이 죽었는데도 콤보가 계속 실행될때... 피버그 발생되고 몹이 죽지도 않네
#ifdef ROCKCLIENT_DEV

						PRINT_DLG_LOG("Server AttackMessage Waiting");
#endif
					/*
						nRui->m_Combo2DTimingEffect.SetComboState(STATE_COMBO_FAIL);
						g_Pc.GetPlayer()->theCombo_State = n_Combo_Disable;		
						g_Pc.GetPlayer()->theActionSystem.InitComboEnable();				
					
						m_curt_event.motion = n_Attack9_Basic0;
						m_curt_event.count = 0;
						m_curt_event.number = 0;
						m_curt_event.status = n_NotCombo;
						
						CM__ATTACK__NPC( m_curt_event );
					*/	
						
					//	m_curt_event.Clear();
									
					//PRINT_DLG_LOG("SM_ATTACK_DELAY ComboState %d" , nRui->m_Combo2DTimingEffect.GetComboState());
				}
			}
			
			m_SimTime = g_nowTime;
		}
	}
	else if( m_curt_event.type == SM_SIMSIM )
	{
		if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
		{
			m_curt_event.Clear();
			m_SimTime = g_nowTime;
			
			if(!m_State.bRide)
			{
				AnimationPlay( n_Stand_Peace9 );
			}
			else
			{
				AnimationPlay( n_Ride_Wait );
				g_Map.m_Npc[m_State.TransIndex].AnimationPlay( n_NPC_Stand0 );
			}
		}
	}
	//-----------------------------------------------------------------------------
	// 아무것도 하고 있지 않을때는 심심심한 행동을 처리한다
	//-----------------------------------------------------------------------------
	else if( m_curt_event.type == NULL )
	{					
		
		if( SetSimSim() == true )
			m_ani_frame = 0;
	}
	else if( m_curt_event.type == SM_ACT )
	{
		if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
		{
			if( m_curt_event.status == SM_ACT_START )
			{
				if( m_curt_event.count )
				{
					BOOL IsSameAni = FALSE;
					
					if( m_curt_event.index == m_curt_event.motion )
					{
						IsSameAni = TRUE;						
					}
					
					m_curt_event.motion = m_curt_event.index;
					m_SimTime = g_nowTime;
					AnimationPlay( m_curt_event.motion );
					
					if(IsSameAni)
					{
						m_ani_frame	= 0;
						m_pre_end_frame = m_end_frame - c_MotionBlendHalfFrame;
						m_pre_ani_number = m_ani_number;
						m_blend_frame = 0;
						m_middle_time = 0.0f;
					}
				}
				else
				{
					m_curt_event.Clear();
					m_SimTime = g_nowTime;
					
					if(!m_State.bRide)
					{
						AnimationPlay( n_Stand_Peace9 );
					}
					else
					{
						AnimationPlay( n_Ride_Wait );
						g_Map.m_Npc[m_State.TransIndex].AnimationPlay( n_NPC_Stand0 );
					}
					
					theIsDrawWeapon = true;
					theDrawWeaponFadeTime = g_nowTime;
				}
			}
			else if( m_curt_event.status == SM_ACT_KEEPING )
			{
				BOOL IsSameAni = FALSE;
				
				if( m_curt_event.index == m_curt_event.motion )
				{
					IsSameAni = TRUE;					
				}
				
				AnimationPlay( m_curt_event.motion );									
				m_SimTime = g_nowTime;

				if(IsSameAni)
				{
					m_ani_frame	= 0;
					m_pre_end_frame = m_end_frame - c_MotionBlendHalfFrame;
					m_pre_end_frame = m_ani_frame;
					m_pre_ani_number = m_ani_number;
					m_blend_frame = 0;
					m_middle_time = 0.0f;
				}
			}
		}
	}
	else if( m_curt_event.type == SM_ANI_SQUENCE )
	{
		m_SimTime = g_nowTime;
		
		if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
		{
			// 다음 에니가 있을때 	
			int AniIndex = g_RockClient.m_SquenceManager.GetPlayerNextAni();
			
			if( AniIndex != -1 )
			{
				AnimationPlay( m_curt_event.motion );	
				g_RockClient.m_SquenceManager.ResetWaitFlag( SWT_ANI );
			}
			// 다음 에니가 없을때
			else 
			{
				m_curt_event.Clear();
				AnimationPlay( n_Stand_Peace9 );									
				g_RockClient.m_SquenceManager.ResetWaitFlag( SWT_ANI );
			}
			
		}
	}
	
	//-----------------------------------------------------------------------------
	// 사운드플래그 false 
	//-----------------------------------------------------------------------------
	m_bSoundPlay1 = false;
	m_bSoundPlay2 = false;
	return true;
}

// 각각의 이벤트 별로 매 프레임마다 체크해서 꼭 처리해야 될 루틴들만 처리 
void Player::UpdateAniProcess()
{
	//	int tgNum = 0;
	
	// 해당 프레임에 한번만 실행됨 
	SoundPlay();
	
	// 시퀀스
	g_RockClient.m_SquenceManager.AniFrameUpdate();
	
	// 이펙트 관련 프레임 체크  By wxywxy
	EffectCheck();
	
	if( m_curt_event.type == SM_ATTACK_NPC || ( m_curt_event.type == SM_USE_SKILL && 
		m_curt_event.status != SM_RESTSKILL_SITDOWN &&
		m_curt_event.status != SM_RESTSKILL_KEEPING &&
		m_curt_event.status != SM_RESTSKILL_STANDUP && 
		m_curt_event.status != SM_CHANT_READY && 												
		m_curt_event.status != SM_CHANT_SPELLING &&
		m_curt_event.status != SM_SELLSKILL_SITDOWN &&
		m_curt_event.status != SM_SELLSKILL_KEEPING &&
		m_curt_event.status != SM_SELLSKILL_STANDUP    ) 
		)
	{
		if( m_ani_index == n_Attack10_BowReady0 )
		{
			return;
		}

		int nImpactFrame = g_Pc_Manager.GetImpactFrame(this, m_ani_index);	
		
		BOOL IsEventProcess = FALSE;

		if( m_ani_frame == nImpactFrame && m_curt_event.damage >= 0 )
		{
			IsEventProcess = TRUE;
		}

		///-- 이벤트는 처리되야하는데.. 임팩트 프레임을 현재프레임이 이미 넘어섰을때
		///-- 해당 이벤트를 처리해야만 한다.
		if( m_ani_frame > nImpactFrame && !m_curt_event.IsActProc )
		{
			IsEventProcess = TRUE;
		}

		if( IsEventProcess )
		{		
			RLGS( "NonPlayer.cpp --- line:2784" );						
			
			m_IsCDataQActed = TRUE;

			SetUnderAttackEventMgr( m_curt_event );
			
			int attr_index = g_Pc.GetAttrIndex( g_Pc.m_MainPC.char_info.theCitizen , g_Pc.m_MainPC.char_info.theGender );
			
			if( attr_index == n_Millena_Man || attr_index == n_Millena_Woman )
			{
				if( m_curt_event.count != 0 && m_curt_event.type != SM_USE_SKILL )
				{
					g_Particle.m_Combo2DEffect.SetCombo( 0 , m_curt_event.count, m_curt_event.comcomple );					
				}			
			}
			else if( attr_index == n_Rain_Man || attr_index == n_Rain_Woman )
			{
				if( m_curt_event.count != 0 && m_curt_event.type != SM_USE_SKILL )
				{
					g_Particle.m_Combo2DEffect.SetCombo( 0 , m_curt_event.count, m_curt_event.comcomple );					
				}			
			}

			m_curt_event.damage = 0;
		}
		
		//....................................................................................................
		// 공격 시작 0 프레임에서 처리 부분
		//....................................................................................................
		if( 1 == m_ani_frame )
			g_Particle.m_EffectManager.AttackStartFrame( ( Character * )this );
		//....................................................................................................
	}
}

// 화살 공격 처리
bool Player::CM__ATTACK__BOW( int Level , HIT_TYPE HitType )
{	
	EVENT_DATA_INFO data;
	
	data = m_save_event;
	m_save_event.Clear();
	
	data.motion = n_Attack10_BowShoot0;
	
	int attack_type = 0;
	bool isPickUp   = false;
	bool isNONPlayer = false;
	
	//방어형 펫을 소유할 경우
	//뒤쪽으로 방어형 펫을 이동시킨다.
	if( m_State.nPetState == nPetState_Master )
	{
		//펫이 방어형 펫인지 여부 확인 - 공격 속도가 0이면 방어형
		SNPCBaseInfo* npc_Info = NULL;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_State.PetIndex].code );		
		
		if( npc_Info && PET_AI_REDUCEDAMAGE == npc_Info->theAttackType )
		{
			Pet_CsStartMove(0, true, &data);	
		}	
	}
	
	if( data.race < 0 )					// NPC	
	{			
		if( g_Map.m_Npc[data.tbl_Idx].m_curt_event.type == SM_STATE_DIE )
		{
			m_curt_event.Clear();
			AnimationPlay( n_Stand_Battle9 );
			
			return false;
		}
		
		attack_type	 = n_NPC;
		
		data.x		 = g_Map.m_NpcTable[data.tbl_Idx].lDestBlock%g_Pc.m_cSize;
		data.y		 = g_Map.m_NpcTable[data.tbl_Idx].lDestBlock/g_Pc.m_cSize;
		data.vPos.x	 = g_Map.m_Npc[data.tbl_Idx].GetPosTM()._41;
		data.vPos.y  = g_Map.m_Npc[data.tbl_Idx].GetPosTM()._42;
		data.vPos.z  = g_Map.m_Npc[data.tbl_Idx].GetPosTM()._43;		

		if( g_Map.m_NpcTable[data.tbl_Idx].code >= c_NonplayerCode_Min && g_Map.m_NpcTable[data.tbl_Idx].code <= c_NonplayerCode_Max )
		{
			isNONPlayer = true;
		}
		else if( g_Map.m_NpcTable[data.tbl_Idx].race == c_NpcRace_Item )
		{
			isPickUp = true;
		}
		
	}
	else								// 유저
	{
		if( g_Pc.m_Pc[data.tbl_Idx].m_curt_event.type == SM_STATE_DIE )
		{
			m_curt_event.Clear();
			AnimationPlay( n_Stand_Battle9 );			
			return false;
		}
		
		attack_type	 = n_PC;
		data.x		 = g_Pc.m_PcTable[data.tbl_Idx].lDestBlock%g_Pc.m_cSize;
		data.y		 = g_Pc.m_PcTable[data.tbl_Idx].lDestBlock/g_Pc.m_cSize;
		data.vPos.x	 = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._41;
		data.vPos.y  = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._42;
		data.vPos.z  = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._43;
	}
	
	int   range = 0;
	
	if( nRui->thePcInven.WearItem[n_WearSlot_Shield] )
	{
		SItemBaseInfo* item_Info = NULL;		
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code );
		
		if( item_Info == NULL )
		{
			m_curt_event.Clear();
			AnimationPlay( n_Stand_Battle9 );
			
			return false;		
		}
		else
		{
			range = item_Info->theRange;
		}
	}
	else
	{
		m_curt_event.Clear();
		AnimationPlay( n_Stand_Battle9 );
		
		return false;		
	}
	
	bool bFind = true;
	
	if( bFind == false )							
	{	
		RLGS( ".............. CM__ATTACK__BOW ........... FindNpcTile = FALSE" );
			
		EVENT_DATA_INFO attackNPC_event;
		attackNPC_event.type = CM_MOVE_ATTACK;
		attackNPC_event.motion = n_Attack9_Basic0;
		attackNPC_event.tbl_Idx = data.tbl_Idx;
		
		if( m_curt_event.type && m_curt_event.type != SM_MOVE_BROADCAST )
		{
			if( attack_type == n_NPC )
			{
				attackNPC_event.race	= g_Map.m_NpcTable[data.tbl_Idx].race;
				attackNPC_event.index	= g_Map.m_NpcTable[data.tbl_Idx].index;
				attackNPC_event.unique	= g_Map.m_NpcTable[data.tbl_Idx].lUnique;
				attackNPC_event.x		= g_Map.m_NpcTable[data.tbl_Idx].lDestBlock%(g_Map.m_cSize*2);
				attackNPC_event.y		= g_Map.m_NpcTable[data.tbl_Idx].lDestBlock/(g_Map.m_cSize*2);
				attackNPC_event.vPos.x	= g_Map.m_Npc[data.tbl_Idx].GetPosTM()._41;
				attackNPC_event.vPos.y	= g_Map.m_Npc[data.tbl_Idx].GetPosTM()._42;
				attackNPC_event.vPos.z	= g_Map.m_Npc[data.tbl_Idx].GetPosTM()._43;
				attackNPC_event.cell	= g_Map.m_NpcTable[data.tbl_Idx].lDestBlock; 
				g_Pc.GetPlayer()->PushQueue( attackNPC_event, TRUE );
				g_Pc.GetPlayer()->PetSetAttackQueue(attackNPC_event, false);
			}
			else
			{
				attackNPC_event.race	= g_Pc.m_PcTable[data.tbl_Idx].char_info.race;
				attackNPC_event.index	= g_Pc.m_PcTable[data.tbl_Idx].nIndex;
				attackNPC_event.unique	= g_Pc.m_PcTable[data.tbl_Idx].lUnique;
				attackNPC_event.x		= g_Pc.m_PcTable[data.tbl_Idx].lDestBlock%(g_Map.m_cSize*2);
				attackNPC_event.y		= g_Pc.m_PcTable[data.tbl_Idx].lDestBlock/(g_Map.m_cSize*2);
				attackNPC_event.vPos.x	= g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._41;
				attackNPC_event.vPos.y	= g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._42;
				attackNPC_event.vPos.z	= g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._43;
				attackNPC_event.cell	= g_Pc.m_PcTable[data.tbl_Idx].lDestBlock;
				g_Pc.GetPlayer()->PushQueue( attackNPC_event, TRUE );
				g_Pc.GetPlayer()->PetSetAttackQueue(attackNPC_event, false);
			}
		}
		
		return false;
	}
	
	if( data.unique == 0 )
	{
		DebugPrint( "\nerror AttackedID: %d\n", data.unique );
	}
	
	if( isNONPlayer )
	{
		NpcChatWnd.theNpcTableIdx = data.tbl_Idx;
		NpcChatWnd.theNpcCode = g_Map.m_NpcTable[data.tbl_Idx].code;
		
		DWORD NPCID = (DWORD)g_Map.m_NpcTable[data.tbl_Idx].lUnique;
		WORD ScriptCode = (WORD)g_Map.m_NpcTable[data.tbl_Idx].code;
		g_RockClient.Send_RpCsScriptStart(NPCID, ScriptCode);
		g_Pc.GetPlayer()->Pet_CsStartMove(g_Pc.m_MainPC.lDestBlock);
	}	
	else if( isPickUp )
	{
		g_RockClient.Send_RpCsPickup( g_Map.m_NpcTable[data.tbl_Idx].lUnique );
		//		g_Map.SM__ITEM__DISAPPEAR( g_Map.m_NpcTable[data.tbl_Idx].lUnique );
		g_Pc.GetPlayer()->Pet_CsStartMove(g_Pc.m_MainPC.lDestBlock);
	}
	else
	{
		//라이딩 중이면 공격 불가
		if(m_State.bRide)
		{
			return true;
		}		
		
		if(attack_type == n_NPC)		///-- 공격 대상이 유저이며
		{
			///-- 은신, 월영으로 보이지 않을경우 공격방지
			if(g_Pc.m_Pc[data.tbl_Idx].m_CharAlpha.GetState() == CHAR_NONE_DISPLAY)
			{
				return true;
			}				
		}
		
		// 부가효과 (공격불가) 처리 
		if( !(g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack)) )
		{
			switch( HitType )
			{
			case CRITICAL_ZONE:
				{
					g_RockClient.Send_RpCsAttack( g_Pc.m_MainPC.lUnique, data.unique, attack_type, data.motion, data.critical, ( BYTE ) Level , n_ComboComplete , data.number );				
				}
				break;
			case HIT_ZONE:
				{
					g_RockClient.Send_RpCsAttack( g_Pc.m_MainPC.lUnique, data.unique, attack_type, data.motion, data.critical, 0 , n_Combo , data.number );				
				}
				break;
			case NONE_HIT_ZONE:
				{
					g_RockClient.Send_RpCsAttack( g_Pc.m_MainPC.lUnique, data.unique, attack_type, data.motion, data.critical, 0 , n_NotCombo , data.number );				
				}
				break;
			}
		}
		
		if( m_State.nPetState == nPetState_Master )
		{
			if( SM_ATTACK_NPC != g_Map.m_Npc[m_State.PetIndex].m_curt_event.type && 
				SM_ATTACK_DELAY != g_Map.m_Npc[m_State.PetIndex].m_curt_event.type &&
				SM_USE_SKILL!= g_Map.m_Npc[m_State.PetIndex].m_curt_event.type)
			{
				EVENT_DATA_INFO attackevent;			
				
				if( attack_type == n_NPC )
				{
					attackevent.tbl_Idx = data.tbl_Idx;
					attackevent.race	= g_Map.m_NpcTable[attackevent.tbl_Idx].race;
					attackevent.index	= g_Map.m_NpcTable[attackevent.tbl_Idx].index;
					attackevent.unique	= g_Map.m_NpcTable[attackevent.tbl_Idx].lUnique;
					attackevent.x		= g_Map.m_NpcTable[attackevent.tbl_Idx].lDestBlock%(g_Map.m_cSize*2);
					attackevent.y		= g_Map.m_NpcTable[attackevent.tbl_Idx].lDestBlock/(g_Map.m_cSize*2);
					attackevent.vPos.x	= g_Map.m_Npc[attackevent.tbl_Idx].GetPosTM()._41;
					attackevent.vPos.y	= g_Map.m_Npc[attackevent.tbl_Idx].GetPosTM()._42;
					attackevent.vPos.z	= g_Map.m_Npc[attackevent.tbl_Idx].GetPosTM()._43;
					attackevent.cell	= g_Map.m_NpcTable[attackevent.tbl_Idx].lDestBlock;	
					attackevent.type = CM_MOVE_ATTACK;					
					g_Pc.GetPlayer()->PetSetAttackQueue( attackevent );	
				}
				else
				{
					attackevent.tbl_Idx = data.tbl_Idx;
					attackevent.race	= g_Pc.m_PcTable[attackevent.tbl_Idx].char_info.race;
					attackevent.index	= g_Pc.m_PcTable[attackevent.tbl_Idx].nIndex;
					attackevent.unique	= g_Pc.m_PcTable[attackevent.tbl_Idx].lUnique;
					attackevent.x		= g_Pc.m_PcTable[attackevent.tbl_Idx].lDestBlock%(g_Map.m_cSize*2);
					attackevent.y		= g_Pc.m_PcTable[attackevent.tbl_Idx].lDestBlock/(g_Map.m_cSize*2);
					attackevent.vPos.x	= g_Pc.m_Pc[attackevent.tbl_Idx].GetPosTM()._41;
					attackevent.vPos.y	= g_Pc.m_Pc[attackevent.tbl_Idx].GetPosTM()._42;
					attackevent.vPos.z	= g_Pc.m_Pc[attackevent.tbl_Idx].GetPosTM()._43;
					attackevent.cell	= g_Pc.m_PcTable[attackevent.tbl_Idx].lDestBlock;
					attackevent.type = CM_MOVE_ATTACK;					
					g_Pc.GetPlayer()->PetSetAttackQueue( attackevent );	
				}
			}
		}		
	}
	
	data.Clear();	
	
	return TRUE;
}


void Player::SetUnderAttackEventMgr( EVENT_DATA_INFO& aEvent )
{
	if(aEvent.IsActProc == TRUE)
	{
		return;
	}

	if( ( aEvent.type == SM_USE_SKILL ) && ( g_Pc.GetSkillSystem()->theIsUsedSkill ) )
	{
		g_Pc.GetSkillSystem()->theIsUsedSkill = FALSE;
	}

	aEvent.IsActProc = TRUE;
	
	EVENT_DATA_INFO underattack_event;	
	
	int tgNum = 0;	
	
	switch( aEvent.type )
	{
	case SM_ATTACK_NPC:
		{			
			m_NowAInfo.dAttackType = SM_ATTACK_NPC;
			m_NowAInfo.dSkillCode = 0;			
			
			if( aEvent.race < 0 )
			{
				g_Particle.m_EffectManager.StandAttack(					
					( Character * ) this , ( Character * ) &g_Map.m_Npc[aEvent.tbl_Idx] ,					
					SM_ATTACK_NPC , aEvent.cell , aEvent.kill , aEvent.damage , aEvent.critical,
					aEvent.lStartTime );
				
			}
			// 다른 유저 공격
			else
			{
				// 받아치기 당했음
				if( aEvent.tbl_Idx == MAX_USER_INTABLE )
				{
					g_Particle.m_EffectManager.StandAttack(						
						( Character * ) this , ( Character * ) this ,						
						SM_ATTACK_NPC , aEvent.cell , aEvent.kill , aEvent.damage , aEvent.critical,
						aEvent.lStartTime );
				}
				else
				{
					g_Particle.m_EffectManager.StandAttack(						
						( Character * ) this , ( Character * ) &g_Pc.m_Pc[aEvent.tbl_Idx] ,						
						SM_ATTACK_NPC , aEvent.cell , aEvent.kill , aEvent.damage , aEvent.critical,
						aEvent.lStartTime );
				}
			}
			
			if( aEvent.number ) 
			{		
				if( MultiAttackTargetMgr( aEvent ) )
				{
					// 검기 , 한계돌파 이펙트 추가 By wxywxy
					g_Particle.SetEffSetCon( 0.0f , EF_SPREAD_SPOT , 0 , 1.1f , GetUnique() , GetIndex() );
				}	
			}	
			
			// 콤보 시퀀스 완성시에 추가 효과 스킬 적용 
			if( aEvent.code && ( aEvent.kill == false ) )
			{
				SSkillBaseInfo* skill_Info;
				skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aEvent.code ); 
				if( skill_Info == NULL )
					return;				
				
				EVENT_DATA_INFO skill_event;
				
				skill_event.type	= SM_USE_SKILL;
				skill_event.status  = SM_USE_SKILL;	
				skill_event.motion	= skill_Info->theActionMotion; 
				skill_event.count	= aEvent.code;
				skill_event.tbl_Idx = aEvent.tbl_Idx;
				skill_event.race	= aEvent.race;
				skill_event.index	= aEvent.index;
				skill_event.unique	= aEvent.unique;
				skill_event.x		= aEvent.x;
				skill_event.y		= aEvent.y;
				skill_event.vPos.x	= aEvent.vPos.x;
				skill_event.vPos.y	= aEvent.vPos.y;
				skill_event.vPos.z	= aEvent.vPos.z;
				
				_STargetInfo		starget_info;
				
				starget_info.theDieFlag			= aEvent.kill;
				starget_info.thePushedCell		= aEvent.cell;
				starget_info.theEffectMaxValue	= 0;
				
				if( aEvent.race < 0 )
				{
					starget_info.theTargetType	= n_NPC;
					starget_info.theTargetID	= g_Map.m_NpcTable[aEvent.tbl_Idx].lUnique;					
				}
				else
				{
					starget_info.theTargetType	= n_PC;
					
					if( aEvent.tbl_Idx == MAX_USER_INTABLE )
					{
						starget_info.theTargetID = g_Pc.m_MainPC.lUnique;
					}
					else
					{
						starget_info.theTargetID = g_Pc.m_PcTable[aEvent.tbl_Idx].lUnique;
					}
				}
				
				for( int i = 0; i < 3; ++i )			// 스킬 효력 처리 3가지 처리  
				{
					if( skill_Info->theEffectInfo[i].theEffectType )
					{
						starget_info.theEffectValue[i] = skill_Info->theEffectInfo[i].theEffectValue;
						starget_info.theKeepupFlag[i] = 1;
					}
					else
					{
						starget_info.theEffectValue[i] = 0;
						starget_info.theKeepupFlag[i] = 0;
					}
				}
				
				if( skill_Info->theAddEffect )
				{
					starget_info.theAddEfectFlag = 1;
				}
				else
				{
					starget_info.theAddEfectFlag = 0;
				}				
				
				skill_event.number = 1;
				skill_event.target_list = SAFE_NEW_ARRAY( _STargetInfo, 1 );						
				memcpy( skill_event.target_list, &starget_info, sizeof(_STargetInfo) );
				
				g_Pc.theSkillSystem.SetDamageValue( aEvent.code, &starget_info );
				g_Pc.theSkillSystem.UseSkillTargetMgr( this , skill_event, starget_info );	
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( this, starget_info, skill_event.count, TRUE );				
			}
		}
		break;
	case SM_USE_SKILL:
		{
#ifdef ROCKCLIENT_DEV
			RLG1( "-------------Player Skill Impact Frame %d----------------\n",aEvent.skill_unique);
#endif 
			SSkillBaseInfo* skill_Info;
			
			skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aEvent.count ); 
			
			if( skill_Info == NULL )	
				return;
			
			m_NowAInfo.dAttackType = SM_USE_SKILL;
			m_NowAInfo.dSkillCode = aEvent.count;			
			
			if( aEvent.number == 0 )	
			{
				if( skill_Info->theActionEffect != 0 )
				{
					EFFECT_EXTRA_INFO TempInfo;
					
					TempInfo.SkillCode   = aEvent.count;
					TempInfo.SkillUnique = aEvent.skill_unique;
					//.......................................................................................................
					// 이펙트 매니저에 스킬 타입및 셀 값을 입력
					//.......................................................................................................
					switch( aEvent.race )
					{
					case n_PC:
						g_Particle.m_EffectManager.SetPC();	
						break;
					case n_NPC:
						g_Particle.m_EffectManager.SetNPC();	
						break;
					case n_Cell:
						g_Particle.m_EffectManager.SetCell( aEvent.x , aEvent.y , aEvent.vPos );
						break;
						
					}
					//.......................................................................................................
					
					if( skill_Info->theActTarget != n_AT_TARGET )
					{
						if( skill_Info->theActionKeepupTime == TRUE )
						{
							g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) skill_Info->theKeepupEffect, this , NULL , &TempInfo );
						}
						else
						{
							g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) skill_Info->theActionEffect , this , NULL , &TempInfo );
						}
					}
				}
				
				return;			
			}
			
			// 이펙트 종류별 
			switch( skill_Info->theEffectType )
			{
				// 0 = 1 : 1
			case 0:
				// 1 = N : N
			case 2:
				{
					_STargetInfo* tg_Info = (_STargetInfo*) aEvent.target_list;
					
					for( tgNum = 0; tgNum < aEvent.number; ++tgNum )
					{							
						if( tg_Info )
						{
							g_Pc.theSkillSystem.UseSkillTargetMgr( this , aEvent, tg_Info[tgNum] );													
						}
					}
				}
				break;
				
				// 1 = 1 : N
			case 1:
				{
					if( aEvent.number == 1 )
					{
						_STargetInfo* tg_Info = (_STargetInfo*) aEvent.target_list;
						
						for( tgNum = 0; tgNum < aEvent.number; ++tgNum )
						{								
							if( tg_Info )
							{
								g_Pc.theSkillSystem.UseSkillTargetMgr( this , aEvent, tg_Info[tgNum] );													
							}
						}
					}
					else
					{							
						_STargetInfo* tg_Info = (_STargetInfo*) aEvent.target_list;
						
						if( tg_Info )
						{
							// 이펙트 생성 루틴 락 걸기 ( 이펙트 생성 하지않고 큐에 저장 & 대기 )
							g_Particle.m_EffectManager.EffectCreateLock();	
							
							for( tgNum = 0; tgNum < ( aEvent.number - 1 ); ++tgNum )
							{		
								g_Pc.theSkillSystem.UseSkillTargetMgr( this , aEvent, tg_Info[tgNum] );													
							}
							
							// 이펙트 생성 루틴 락 해제 ( 큐에 저장후 이펙트 생성 )
							g_Particle.m_EffectManager.EffectCreateUnLock();
							
							g_Pc.theSkillSystem.UseSkillTargetMgr( this , aEvent, tg_Info[tgNum] );													
						}							
					}						
				}
				break;
			}
		}
		break;
		
	default:
		break;
	}	
	
	return;
}

//-------------------------------------------------------------------------------------------------------------
// Illusion Control ...........................................................................................
// 
//------ AppearPC, DisappearPC 시에 분신 타입과 주인의 ID, 분신의 Index 받아서 저장.
//------ 해당 분신들의 정보를 주인의 값으로 채워 넣는다.
//------ 맨 처음 생성시에 주인의 상태와 똑같이 생성 (후에 일어나는 아이템 및 스킬에 관한 처리 필요없음) 
//------ Illusion_CsStartMove() 이용, 분신들 움직임 처리. 
//------ ScAttack, ScSkill 에서 분신들의 스킬 시전도 같이 처리,  
//			사정거리에 맞지 않는다면, 움직인 다음 공격 및 스킬 처리 
//-------------------------------------------------------------------------------------------------------------
// bMove 값은 강제 이동 시키기 처리를 위해
// 자신의 분신들 이동 처리 
void Player::Illusion_CsStartMove(DWORD aDestCell , bool bMove /* = false */)
{
	///-- 이동 불가		
	if( !bMove )
	{
		if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotMove))
		{
			return;
		}	
	}
	
	int idx = 0; 
	int i = 0;
	int table_index = 0;

	DWORD dDCell = aDestCell;
	
	int nNum = RandomNum( -1 , (int)c_Max_Illusion_Num );

	if( nNum == -1 )
	{
		g_RockClient.Send_RpCsStartMove( g_Pc.m_MainPC.lUnique, 
			dDCell, 0 ); /// aMoveSpeed 일단 0으로...
	}
	
	for( idx = 0; idx < c_Max_Illusion_Num; ++idx )
	{			
		table_index = g_Pc.GetPlayer()->m_State.IllusionIndex[idx];
		
		if( table_index != -1 )
		{
			for( int i = 0; i < 25; ++i )
			{
				DWORD PosX = aDestCell % g_Pc.m_cSize;
				DWORD PosY = aDestCell / g_Pc.m_cSize;
				
				PosX += RandomNum( -2, 2 );
				PosY += RandomNum( -2, 2 );
				
				if( ( g_Map.m_pAttrMap[PosY][PosX].byAttr != MATTR_OBJ ) && ( g_Map.m_Block[PosX][PosY].GetClear() ) )
				{		
					aDestCell = PosX + ( PosY * g_Pc.m_cSize );
					break;
				}
			}
			
			g_RockClient.Send_RpCsStartMove( g_Pc.m_PcTable[table_index].lUnique, aDestCell, 0 ); 
		}

		if( nNum == idx )
		{
			g_RockClient.Send_RpCsStartMove( g_Pc.m_MainPC.lUnique, 
				dDCell, 0 ); /// aMoveSpeed 일단 0으로...
		}
	}
	
	if( nNum == c_Max_Illusion_Num )
	{
		g_RockClient.Send_RpCsStartMove( g_Pc.m_MainPC.lUnique, 
				dDCell, 0 ); /// aMoveSpeed 일단 0으로...
	}
	
	return;
}

void Player::Pet_CsStartMove(DWORD aDestCell, bool m_IsDefenceBack, EVENT_DATA_INFO *pData)
{
	if( m_State.nPetState != nPetState_Master )
		return;
	
	if( g_Map.m_Npc[m_State.PetIndex].GetState() && 
		N_Attack == g_Map.m_Npc[m_State.PetIndex].GetState()->nSate )
		return;
	
	if(!m_IsDefenceBack)
	{
		g_Map.m_Npc[m_State.PetIndex].dDestCell = aDestCell;
		g_Map.m_Npc[m_State.PetIndex].ChangeState(CTransState::Instance());
		///--PRINT_DLG_LOG( "펫 이동 수행 목적지 %d%", aDestCell );
	}
	else
	{		
		if( pData->race < 0 )
		{
			g_Map.m_Npc[m_State.PetIndex].vDestPos.x = g_Map.m_Npc[pData->tbl_Idx].GetPosTM()._41;
			g_Map.m_Npc[m_State.PetIndex].vDestPos.y = g_Map.m_Npc[pData->tbl_Idx].GetPosTM()._42;
			g_Map.m_Npc[m_State.PetIndex].vDestPos.z = g_Map.m_Npc[pData->tbl_Idx].GetPosTM()._43;
		}
		else
		{
			g_Map.m_Npc[m_State.PetIndex].vDestPos.x = g_Pc.m_Pc[pData->tbl_Idx].GetPosTM()._41;
			g_Map.m_Npc[m_State.PetIndex].vDestPos.y = g_Pc.m_Pc[pData->tbl_Idx].GetPosTM()._42;
			g_Map.m_Npc[m_State.PetIndex].vDestPos.z = g_Pc.m_Pc[pData->tbl_Idx].GetPosTM()._43;
		}		
		
		g_Map.m_Npc[m_State.PetIndex].m_IsDefenceBack = true;
		g_Map.m_Npc[m_State.PetIndex].ChangeState(CTransState::Instance());
	}
}

void Player::Pet_CsOwnerBackMove()
{
	if( m_State.nPetState != nPetState_Master )
		return;	
	
	g_Map.m_Npc[m_State.PetIndex].m_IsOwnerBack = true;
	g_Map.m_Npc[m_State.PetIndex].ChangeState(CTransState::Instance());
}

void Player::PetSetAttackQueue( EVENT_DATA_INFO& data, bool bAttackFlag )
{
	if( m_State.nPetState != nPetState_Master )
		return;
	
	//대상이 아이템 일때
	if( data.race == c_NpcRace_Item )
	{
		//Pet_CsStartMove( theAttack_Event.cell );
		return;
	}
	
	int race_type = 0;
	
	if( data.race < 0 )
	{
		race_type = n_NPC;
	}
	else
	{
		race_type = n_PC;
	}	
	
	if( race_type == n_NPC )
	{   
		//대상이 NPC일때
		if( g_Map.m_NpcTable[data.tbl_Idx].code >= c_NonplayerCode_Min && g_Map.m_NpcTable[data.tbl_Idx].code <= c_NonplayerCode_Max )
		{
			//Pet_CsStartMove( theAttack_Event.cell );
			return;
		}
	}
	else
	{
		//대상이 상점 오픈일 경우 
		if( g_Pc.m_Pc[data.tbl_Idx].m_curt_event.status == SM_SELLSKILL_SITDOWN ||
			g_Pc.m_Pc[data.tbl_Idx].m_curt_event.status == SM_SELLSKILL_KEEPING ||
			g_Pc.m_Pc[data.tbl_Idx].m_curt_event.status == SM_SELLSKILL_STANDUP )
		{
			//Pet_CsStartMove( theAttack_Event.cell );
			return;
		}
		
		SItemBaseInfo* item_Info = NULL;

		if( nRui->thePcInven.WearItem[n_WearSlot_Weapon] && 		 
		    !g_RockClient.CanAttackPlayer( &g_Pc.m_Pc[data.tbl_Idx] ) )
		{
			if(nRui->thePcInven.WearItem[n_WearSlot_Weapon]->ItemTable->theClass == n_Class_Weapon  && 
				nRui->thePcInven.WearItem[n_WearSlot_Weapon]->ItemTable->theType == n_Type_ShortWeapon )
				return;
		}		

	}
	
	//펫이 방어형 펫인지 여부 확인 - 공격 속도가 0이면 방어형
	SNPCBaseInfo* npc_Info = NULL;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_State.PetIndex].code );		
	
	if( npc_Info && (PET_AI_REDUCEDAMAGE == npc_Info->theAttackType) )
	{
		//Pet_CsStartMove( theAttack_Event.cell );
		return;
	}

	///-- 펫 상태가 은신상태일때는 공격하지 않는다.
	if( g_Map.m_Npc[m_State.PetIndex].m_CharAlpha.GetState() != CHAR_NORMAL )
	{
		return;
	}

	if( bAttackFlag )
	{
		///-- 공격형 펫의 정신력이 0이면 공격을 하지 않는다.
		if( g_Map.m_NpcTable[m_State.PetIndex].theSMP == 0 )
		{
			return;
		}
		
		///-- 현재 펫이 공격중이면 리턴
		if( g_Map.m_Npc[m_State.PetIndex].m_curt_event.type == SM_ATTACK_DELAY ||
			g_Map.m_Npc[m_State.PetIndex].m_curt_event.type == SM_ATTACK_NPC ||
			g_Map.m_Npc[m_State.PetIndex].m_curt_event.type == SM_USE_SKILL )
		{
			return;
		}
		
		data.type = CM_MOVE_ATTACK;		
		data.motion = n_NPC_Attack0;
		
		g_Map.m_Npc[m_State.PetIndex].PushQueue(data, TRUE);
	}
	else
	{
		Pet_CsStartMove( theAttack_Event.cell );
	}
}

void Player::Illusion_CsAttack( DWORD aAttackedID, BYTE aAttackedType, WORD aMotion, BYTE aComboType, BYTE aComboCount, BYTE aComboStatus, BYTE aComboSeq )
{
	// 부가효과 (공격불가) 처리 
	if( g_Pc.GetPlayer()->GetAddEffectState(n_TypeNotAttack))
	{
		return;
	}	
	
	int idx = 0; 
	int i = 0;
	int table_index = 0;
	
	g_RockClient.Send_RpCsAttack( g_Pc.m_MainPC.lUnique, aAttackedID, 
					aAttackedType, aMotion, aComboType, aComboCount, aComboStatus, aComboSeq );
	
	for( idx = 0; idx < c_Max_Illusion_Num ; ++idx )
	{
		table_index = g_Pc.GetPlayer()->m_State.IllusionIndex[idx];
		
		if( table_index != -1 )
		{
			g_RockClient.Send_RpCsAttack( g_Pc.m_PcTable[table_index].lUnique, aAttackedID, 
					aAttackedType, aMotion, aComboType, aComboCount, aComboStatus, aComboSeq );				
		}
	}
	
	return;
}

// 스킬 사용
void Player::Illusion_CsSkill( DWORD aTarget, BYTE aTargetType, WORD aSkillCode )
{
	
	int idx = 0; 
	int i = 0;
	int table_index = 0;

	DWORD dTarget = aTarget;
	
	// 스킬 사용 서버에 요청
	g_RockClient.Send_RpCsSkill( g_Pc.m_MainPC.lUnique, dTarget, aTargetType, aSkillCode );	
	
	SSkillBaseInfo* skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aSkillCode ); 
	if( skill_Info == NULL )
		return;
	
	EVENT_DATA_INFO attackNPC_event;
	attackNPC_event.type = CM_MOVE_ATTACK;
	attackNPC_event.motion = n_Attack9_Basic0;	

	int nIndex = 0;

	if( skill_Info->theSkillType2 == n_DeathBlow || 
		skill_Info->theSkillType2 == n_AttackMagic || 
		skill_Info->theSkillType2 == n_NoFrameDeathBlow )
	{
		if( aTargetType == n_NPC )
		{
			nIndex = g_Map.FindNpc( aTarget );
			if( nIndex == MAX_NPC_INTABLE )
			{
				return;
			}			
			attackNPC_event.race	= g_Map.m_NpcTable[nIndex].race;
			attackNPC_event.index	= g_Map.m_NpcTable[nIndex].index;
			attackNPC_event.unique	= g_Map.m_NpcTable[nIndex].lUnique;
			attackNPC_event.x		= g_Map.m_NpcTable[nIndex].lDestBlock%(g_Map.m_cSize*2);
			attackNPC_event.y		= g_Map.m_NpcTable[nIndex].lDestBlock/(g_Map.m_cSize*2);
			attackNPC_event.vPos.x	= g_Map.m_Npc[nIndex].GetPosTM()._41;
			attackNPC_event.vPos.y	= g_Map.m_Npc[nIndex].GetPosTM()._42;
			attackNPC_event.vPos.z	= g_Map.m_Npc[nIndex].GetPosTM()._43;
			attackNPC_event.cell	= g_Map.m_NpcTable[nIndex].lDestBlock; 			
			g_Pc.GetPlayer()->PetSetAttackQueue( attackNPC_event );				
		}
		else
		{
			nIndex = g_Pc.FindUser( aTarget );
			if( nIndex == MAX_USER_INTABLE )
			{
				return;
			}			
			attackNPC_event.race		= g_Pc.m_PcTable[ nIndex ].char_info.race;
			attackNPC_event.index		= g_Pc.m_PcTable[ nIndex ].nIndex;
			attackNPC_event.unique		= g_Pc.m_PcTable[ nIndex ].lUnique;
			attackNPC_event.x			= g_Pc.m_PcTable[ nIndex ].lDestBlock%(g_Map.m_cSize*2);
			attackNPC_event.y			= g_Pc.m_PcTable[ nIndex ].lDestBlock/(g_Map.m_cSize*2);
			attackNPC_event.vPos.x		= g_Pc.m_Pc[ nIndex ].GetPosTM()._41;
			attackNPC_event.vPos.y		= g_Pc.m_Pc[ nIndex ].GetPosTM()._42;
			attackNPC_event.vPos.z		= g_Pc.m_Pc[ nIndex ].GetPosTM()._43;
			attackNPC_event.cell		= g_Pc.m_PcTable[ nIndex ].lDestBlock;		
			g_Pc.GetPlayer()->PetSetAttackQueue( attackNPC_event );	
		}
	}
	
	for( idx = 0; idx < c_Max_Illusion_Num; ++idx )
	{
		table_index = g_Pc.GetPlayer()->m_State.IllusionIndex[idx];
		
		if( table_index != -1 )
		{
			// 마스터가 자기 자신에게 쓰는 스킬일 경우 분신도 자기 자신에게 사용해야 한다. 
			if( dTarget == g_Pc.m_MainPC.lUnique )
			{
				aTarget = g_Pc.m_PcTable[table_index].lUnique;
			}			
			
			if( skill_Info->theSkillType2 == n_DeathBlow || 
				skill_Info->theSkillType2 == n_AttackMagic || 
				skill_Info->theSkillType2 == n_NoFrameDeathBlow )
			{
				g_RockClient.Send_RpCsSkill( g_Pc.m_PcTable[table_index].lUnique, aTarget, 
											 aTargetType, aSkillCode );
			}
		}		
	}
	
	return;
}


// 자신의 파티원들의 분신들을 찾아내서 구별할 수 있도록 설정한다. ( nCharState_Detect_Illusion )
void Player::DetectPartyIllusion( DWORD aPartyID, BOOL bParty )
{
	int party_index = g_Pc.FindUser( aPartyID );
	if( party_index == MAX_USER_INTABLE )
		return;
	
	int idx = 0;
	int illusion_index = -1;
	
	if( bParty )			// 파티 생성시 같은 파티원들의 분신을 구별할 수 있게 한다.  
	{
		if( g_Pc.m_Pc[party_index].m_State.nCharState == nCharState_Master )
		{
			for( idx = 0; idx < c_Max_Illusion_Num; ++idx )
			{
				illusion_index = g_Pc.m_Pc[party_index].m_State.IllusionIndex[idx];
				
				if( illusion_index != -1 )
				{
					g_Pc.m_Pc[illusion_index].m_State.nCharState = nCharState_Detect_Illusion; 				
				}
			}
		}
	}
	else					// 파티 해제시 예전 파티원의 분신을 구별 할 수 없다.  
	{
		if( g_Pc.m_Pc[party_index].m_State.nCharState == nCharState_Master )
		{
			for( idx = 0; idx < c_Max_Illusion_Num; ++idx )
			{
				illusion_index = g_Pc.m_Pc[party_index].m_State.IllusionIndex[idx];
				
				if( illusion_index != -1 )
				{
					g_Pc.m_Pc[illusion_index].m_State.nCharState = nCharState_Illusion; 				
				}
			}
		}
	}
	
	return;
}

void Player::UpdateHPData()
{
	if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_lPreTime , HPREFRESH_TIME ) , < , g_nowTime ) )
	{
		///--JAPAN_BUG_MODIFY
		if( !IsRealDie() )
		{
			///-- 이부분에서 실제 HP를 보정시킨다.
			if( nRui->thePcParam.Stat1.theHP != 
				g_Pc.m_MainPC.char_info.theRealHP )
			{
				///--PRINT_DLG_LOG("theHP = %d theRealHP %d", 
				///--	nRui->thePcParam.Stat1.theHP, 
				///--	g_Pc.m_MainPC.char_info.theRealHP);
				///--PRINT_DLG_LOG("Player HP 동기화");
				
				nRui->thePcParam.Stat1.theHP = g_Pc.m_MainPC.char_info.theRealHP;
				m_lModifyHpTime = timeGetTime();
			}
		}
		
		///-- 실제로는 죽은 상태인데.. 현재 죽어있지 않다면...
		if( IsRealDie() && m_curt_event.type != SM_STATE_DIE )
		{
			///-- 죽는거 보정작업
			EVENT_DATA_INFO die_event;
			die_event.type = SM_STATE_DIE;
			die_event.loop = false;
			
			die_event.motion = n_Die9;
			g_Particle.DelEffSetCon( EF_REST_BREATH , 0 , GetUnique() , GetIndex() );
			PushQueue( die_event, TRUE );
			
			// 죽을때 가지고 있는 이펙트 모두 지우기
			g_Particle.m_EffectManager.DeletePlayerEffect( (Character *)this );
			
			// 죽을때 효과
			g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , GetUnique() , -1 );
			///--PRINT_DLG_LOG("Player 죽음 보정작업");
		}
		
		m_lPreTime = g_nowTime;
	}
}

bool Player::GetAddEffectState(NAddEffectType EffectType)
{

	if( theAddEffect_State[EffectType] == 1 )
	{
		return TRUE;
	}
		
	return FALSE;
}

BOOL Player::IsMyAttacker(DWORD nAPCId)
{
	AttackerListPos iPos = m_MyAttackerList.begin();

	for( ; iPos != m_MyAttackerList.end() ; iPos++ )
	{
		DWORD dAPCId = (DWORD)(*iPos);
		if( nAPCId == dAPCId )
		{
			return TRUE;
		}
	}

	return FALSE;
}	
				
BOOL Player::InsertMyAttackerList(DWORD nAPCId)
{
	if( !IsMyAttacker(nAPCId) )
	{
		m_MyAttackerList.push_back(nAPCId);
		return TRUE;
	}

	return FALSE;
}	
		
BOOL Player::DeleteMyAttackerList(DWORD nAPCId)
{
	AttackerListPos iPos = m_MyAttackerList.begin();
	
	for( ; iPos != m_MyAttackerList.end() ; iPos++ )
	{
		DWORD dAPCId = (DWORD)(*iPos);
		if( nAPCId == dAPCId )
		{
			m_MyAttackerList.erase(iPos);
			return TRUE;
		}
	}

	return FALSE;
}	
		
void Player::ClearAllMyAttackerList()
{
	m_MyAttackerList.clear();
}

void Player::ExcutePKModeAction()
{
	///-- 현재 플레이어의 PK모드를 받아온다.
	BYTE dPKMode = m_PKMode;
	switch( dPKMode )
	{
		case n_PKOFFMode:
			{
				///-- n_PKONMode 모드로 전환시켜 달라고 요청
				g_RockClient.Send_CsPKMode(n_PKONMode);
			}
			break;
		case n_PKONMode:
			{
				///-- n_PKOFFMode 모드로 전환시켜 달라고 요청
				g_RockClient.Send_CsPKMode(n_PKOFFMode);
			}
			break;
		case n_DPKMode:
		case n_APKMode:
			{
				///-- PK 관련 토글 불가 시스템 메시지 출력
				SystemPrint( n_DarkRed, n_SystemMsg , G_STRING_CHAR(IDS_SYS_CANCHANGE_PKMODE) );	
			}
			break;
	}
}	
