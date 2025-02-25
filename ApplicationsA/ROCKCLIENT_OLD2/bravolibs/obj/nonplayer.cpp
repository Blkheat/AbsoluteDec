#include "RockPCH.h"

#include	<NETWORK\Protocol.h>
#include	<Base\\D3DBase.h>
#include	"..\\quadlist.h"
#include	"..\\RockClient.h"
#include	"ObjBase.H"
#include	"NonPlayer.H"
#include	"..\\Pc.H"
#include	"..\\Camera.H"
#include	"..\\Effect.h"
#include	<stdio.h>
#include	<assert.H>
#include	<mmsystem.h>
#include	<SOUND\Sound.H>
#include	"..\\effect\CEffectManager.h"
#include "..\\global.h"
#include "..\\..\\GTimer.h"

#include "RockInterface\\NonPlayerInfoWnd.h"
#include "RockInterface\\NonPlayerInfoAddWnd.h"
#include "RockInterface\\MainMenuWndProc.h"


#define		SELECT_ROTATE_TIME			50.0f	// 선택동그라미 회전시간(0.05초)
#define		SELECT_SCALE_SIZE			0.1f	// 선택동그라미 스케일변화값

extern	int	GetMax( int x, int xx );
extern		DWORD	FloatToDWORD( float f );

extern		long				g_nowTime;
extern		int					g_Shadow;
extern		int					g_chr_anicount;

extern D3DXVECTOR3			g_AniVertex[3000];	// 망토 에니시 저장하는 변수 ( 그림자 처리시 사용 ) By wxywxy

const int c_MobAttackFrame( 14 );

// 로그인 관련
int	             NonPlayer::m_StaticAniCount = 0;
CLASS_VIEW_DATA	*NonPlayer::m_pCharInfo      = NULL;			



void NonPlayer::Clear()
{	
	m_CharAlpha.Clear();
	
	m_ISApproachWeapon = FALSE;
	m_AttackMotion     = 0;
	m_KeepUpChecker.Clear();
	
	theIsAttack = true;
	
	m_Char_WearType = CHAR_WEAR_NORMAL;
	
	m_IsCDataQActed = TRUE;
	
	m_OneTimeJump = false;

	m_nNpcCode = -1;

}


void NonPlayer::Destroy()
{	
	m_Shadow.Clear();	
	
	//-----------------------------------------------------------------------------
	// 소팅테이블에서 삭제
	//-----------------------------------------------------------------------------
	//	g_Pc_Manager.ClearSortInfo( m_bPC, m_nTableIndex );
	
	if( m_bPC )
		g_Map.m_SortTable.Delete( ID_PC , m_nTableIndex , 0 );
	else
		g_Map.m_SortTable.Delete( ID_NPC , m_nTableIndex , 0 );
	
	//-----------------------------------------------------------------------------
	// 할당된 메모리삭제
	//-----------------------------------------------------------------------------
	Clear();
	Free();
	
	for( int i = 0; i < c_Max_RideItemType ; i++ )
	{
		m_Ride_Item_index[i] = -1;
	}

}


bool NonPlayer::CreateNonPlayer(bool IsPet)
{
	SetRealDie(FALSE);
	
	m_CharAlpha.Clear();
	
	m_ISApproachWeapon = FALSE;
	m_AttackMotion     = 0;
#ifdef NPC_MOVE_TO_SKILL
#else 
	if( m_bPC || IsPet )
#endif 
	
	{
		if( theFindWay == NULL )
		{
			theFindWay = SAFE_NEW( CFindWay );			
			theFindWay->m_pRefCharacter = (void *)this;
		}
	}
	
	if( m_bPC )
	{
		m_uShadowSizeType = NSHADOW_SIZE_128;
		m_nShadowSize = 128;
	}
	else
	{
		m_uShadowSizeType = NSHADOW_SIZE_128;
		m_nShadowSize = 128;
		///-- NPC사이즈에 따라서 사용할 공용 쉐도우 렌더타겟
		///종류를 선택한다.
		int nNpcSize = 1;
			
		if( m_nTableIndex != -1 )
		{
			nNpcSize = g_Map.m_NpcTable[m_nTableIndex].theNpcSize;
		}
		
		if(  3 <= nNpcSize )
		{
			m_uShadowSizeType = NSHADOW_SIZE_256;
			m_nShadowSize = 256;
		}

		if( m_State.nCharacterType == nCharacterTypeBigMoster ||
			m_State.nCharacterType == nCharacterTypeBulding )
		{
			m_uShadowSizeType = NSHADOW_SIZE_256;
			m_nShadowSize = 256;
		}		
	}
	
	//-----------------------------------------------------------------------------
	// 캐릭터 정보 초기화
	//-----------------------------------------------------------------------------	
	int	nItmCount;// = g_Pc_Manager.GetItmCount( this );	
	int	chrAttrIndex = this->GetAttrIndex();
	
	nItmCount = g_Pc_Manager.GetItmCount( this );
	
	for( int i = 0; i < nItmCount; ++i )
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
	
	m_vScale = g_Pc_Manager.GetChrScale( this );
	
	//-----------------------------------------------------------------------------
	// 캐릭터 속성 초기화
	//-----------------------------------------------------------------------------
	if( m_bPC == true )
	{	
		// 유저변수할당
		if( m_pUserInfo == NULL )
			m_pUserInfo = SAFE_NEW( USER_INFO );		
		
		m_pUserInfo->ArmorInfo.RHand = g_Pc_Manager.m_PcAttr[g_Pc.m_PcTable[m_nTableIndex].nIndex].r_hand_node;
		m_pUserInfo->ArmorInfo.LHand = g_Pc_Manager.m_PcAttr[g_Pc.m_PcTable[m_nTableIndex].nIndex].l_hand_node;			
		
		m_pUserInfo->bAttack = true;
		
		SetWearItem();
	}
	
	if( ( m_bPC == false ) && ( m_State.nCharacterType == nCharacterTypePlayerNPC ) && ( m_nTableIndex != -1 ) )
	{
		// 유저변수할당
		if( m_pUserInfo == NULL )
			m_pUserInfo = SAFE_NEW( USER_INFO );
		
		m_pUserInfo->ArmorInfo.RHand = g_Pc_Manager.m_PcAttr[g_Map.m_NpcTable[m_nTableIndex].index].r_hand_node;
		m_pUserInfo->ArmorInfo.LHand = g_Pc_Manager.m_PcAttr[g_Map.m_NpcTable[m_nTableIndex].index].l_hand_node;			
		
		m_pUserInfo->bAttack = true;
		
		SetPlayer_NpcWearItem();
	}
	else if( ( m_bPC == false ) && ( m_State.nCharacterType == nCharacterTypePlayerNPC ) && ( m_nTableIndex == -1 ) )
	{		
		// 유저변수할당
		if( m_pUserInfo == NULL )
			m_pUserInfo = SAFE_NEW( USER_INFO );

		m_pUserInfo->ArmorInfo.RHand = g_Pc_Manager.m_PcAttr[chrAttrIndex].r_hand_node;
		m_pUserInfo->ArmorInfo.LHand = g_Pc_Manager.m_PcAttr[chrAttrIndex].l_hand_node;		
		
		m_pUserInfo->bAttack = true;
		
		SetPlayer_NpcChrImage();	
	}
	
	if( -1 == m_TempIndex )
	{
		if( m_bPC )
			g_Map.m_SortTable.Insert( ID_PC , m_nTableIndex , 0 , FALSE );
		else
			g_Map.m_SortTable.Insert( ID_NPC , m_nTableIndex , 0 , FALSE );
	}
	
	// 애니메이션기본셋팅
	if( m_bPC == true || ( m_bPC == false && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		if( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman )
		{
			AnimationPlay( rand()%2 ? n_Suddenly9 : n_Stand_Peace9 );
		} 
		else if( chrAttrIndex == n_Rain_Man || chrAttrIndex == n_Rain_Woman )
		{
			AnimationPlay( rand()%2 ? n_Suddenly9 : n_Stand_Peace9 );
		}	
	}		
	else
	{
		AnimationPlay( n_NPC_Stand0 );	
	}
	
	// 현재시간저장
	m_PreTime	= g_nowTime;
	m_PreMTime	= g_nowTime;
	m_SimTime	= g_nowTime;	
	
#ifdef BOUND_BOX_DISPLAY

	m_CollisionBox.InitDeviceObjects(g_lpDevice);
	m_CollisionBox.RestoreDeviceObjects();
	m_CollisionBox.Update(GetBoundMinPos(),GetBoundMaxPos());
	
#endif
	
	// 급한 시연을 위해 인단은 클라이언트에서 수동으로 NPC 각도를 돌려놓는다 ^ ^;;
	if( !m_bPC )
	{
		if( g_Map.m_NpcTable[m_nTableIndex].code < c_NonplayerCode_Min )			
		{
			SetDirection( RandomNum( 0.0f, 6.28f ) );
		}
	}
	
	return m_bIsLoaded = true;	
}

void NonPlayer::SetPlayer_NpcChrImage()
{
	SItemBaseInfo* item_Info = NULL;
	int chrAttrIndex = GetAttrIndex();

	m_Char_WearType = CHAR_WEAR_NORMAL;

	if( m_nNpcCode != -1 )
	{
		SPlayerNPCData PNPCData = g_Pc_Manager.m_PlayerNPCDataManager.FindPlayerNPCData( m_nNpcCode );
		if( PNPCData.CoatArmor_Code )
		{
			m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, PNPCData.CoatArmor_Code );
			SetItemBuffer( n_Wear_CoatArmor );

			///-- 셋트 아이템 웨어처리
			SItemBaseInfo* pSItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( PNPCData.CoatArmor_Code );
			if( pSItemBaseInfo &&  n_Type_SetCloth == pSItemBaseInfo->theType)
			{
				m_Char_WearType = CHAR_WEAR_SETITEM;				
				g_Particle.m_EffectManager.CreateDefenceEnChantEffect(this , 255 );
			}
			else
			{
				m_Char_WearType = CHAR_WEAR_NORMAL;	
			}
		}

		if( PNPCData.PantsArmor_Code )
		{
			m_pItm_index[n_Wear_PantsArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, PNPCData.PantsArmor_Code );
			SetItemBuffer( n_Wear_PantsArmor );				
		}
		
		if( PNPCData.Gloves_Code )
		{
			m_pItm_index[n_Wear_Gloves] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, PNPCData.Gloves_Code );
			SetItemBuffer( n_Wear_Gloves );				
		}
		
		if( PNPCData.Shoes_Code )
		{
			m_pItm_index[n_Wear_Shoes] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, PNPCData.Shoes_Code );
			SetItemBuffer( n_Wear_Shoes );			
		}

		if( PNPCData.Helmet_Code )
		{
			m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, PNPCData.Helmet_Code );
			SetItemBuffer( n_Wear_Hair );
			
			item_Info = g_Pc_Manager.GetItemBaseInfoPtr( PNPCData.Helmet_Code );
			if( item_Info && item_Info->theType == n_Type_KeepupHair )	// 헤어도 동시에 그려줘야 된다 (악세사리 형)
			{
				m_Basic_DrawItm[n_Wear_Hair] = 1;		
				m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, PNPCData.Hair_Code ); 
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
			m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, PNPCData.Hair_Code );
			SetItemBuffer( n_Wear_Hair );	
		}

		if( PNPCData.Face_Code ) 
		{
			m_pItm_index[n_Wear_Face] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, PNPCData.Face_Code );
			SetItemBuffer( n_Wear_Face );	
		}

		PreTextureLoad();
	}
}

void NonPlayer::ReSetWearItem()
{
	int	nItmCount;// = g_Pc_Manager.GetItmCount( this );	
	int	chrAttrIndex = this->GetAttrIndex();
	
	nItmCount = g_Pc_Manager.GetItmCount( this );
	
	for( int i = 0; i < nItmCount; ++i )
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

void NonPlayer::SetWearItem()
{	
	SItemBaseInfo* item_Info = NULL;
	int chrAttrIndex = GetAttrIndex();
	// 이전에 있던 링크 이펙트를 모두 지워 Boa 요 By wxywxy
	//	int Idx = g_Pc.FindUser(g_Pc.m_PcTable[m_nTableIndex].char_info.thePCID);
	g_Particle.m_EffectManager.DeleteLink( ( Character * )this );
	g_Particle.m_EffectManager.DeleteWeaponEnChantEffect( ( Character * ) this );	
	
	bool IsEquipWeapon = false;
	
	m_Char_WearType = CHAR_WEAR_NORMAL;
	
	// nRui->PcParam.theWearItem[10] 아이템 코드에 따라서 보여줄 7가지 m_pItm_Index를 세팅한다 

	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_CoatArmor].theCode )
	{
		m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_CoatArmor].theCode);
		SetItemBuffer( n_Wear_CoatArmor );
		
		///-- 셋트 아이템 웨어처리
		SItemBaseInfo* pSItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_CoatArmor].theCode );
		if( pSItemBaseInfo &&  (n_Type_SetCloth == pSItemBaseInfo->theType	|| n_Type_Skin_Suit == pSItemBaseInfo->theType))
		{
			m_Char_WearType = CHAR_WEAR_SETITEM;
			g_Particle.m_EffectManager.CreateDefenceEnChantEffect(this , 255 );
		}
		else
		{
			m_Char_WearType = CHAR_WEAR_NORMAL;	
		}		
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_CoatArmor].theCode , this );
	}
	
	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_PantsArmor].theCode )
	{
		m_pItm_index[n_Wear_PantsArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_PantsArmor].theCode );
		SetItemBuffer( n_Wear_PantsArmor );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_PantsArmor].theCode , this );
	}	
	
	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Gloves].theCode )
	{
		m_pItm_index[n_Wear_Gloves] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Gloves].theCode );
		SetItemBuffer( n_Wear_Gloves );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Gloves].theCode , this );
	}
	
	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Shoes].theCode )
	{
		m_pItm_index[n_Wear_Shoes] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Shoes].theCode );
		SetItemBuffer( n_Wear_Shoes );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Shoes].theCode , this );
	}
	
	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Helmet].theCode )
	{
		m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Helmet].theCode );
		SetItemBuffer( n_Wear_Hair );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Helmet].theCode , this );
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Helmet].theCode );
		if( item_Info && (item_Info->theType == n_Type_KeepupHair || n_Type_Skin_KeepypHair == item_Info->theType))
		{
			m_Basic_DrawItm[n_Wear_Hair] = 1;		
			m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theHair ); 
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
		m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theHair );
		SetItemBuffer( n_Wear_Hair );	
	}
	
	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theFace ) 
	{
		m_pItm_index[n_Wear_Face] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theFace );
		SetItemBuffer( n_Wear_Face );	
	}
	
	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Serve].theCode )
	{
		m_pItm_index[n_Wear_Serve] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Serve].theCode ); // 아이템 코드에 맞는 텍스쳐 번호 로딩  
		SetItemBuffer( n_Wear_Serve );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Serve].theCode , this );
	}
	
	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Shield].theCode )
	{
		IsEquipWeapon = true;
		
		m_pUserInfo->ArmorInfo.index[1] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Shield].theCode );
		m_pUserInfo->ArmorInfo.Code[1] = g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Shield].theCode;
		m_pUserInfo->bAttack = true;
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Shield].theCode , this );
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Shield].theCode );
		
		if( item_Info )
		{
			if( n_Type_Bow == item_Info->theType || n_Type_MagicBook == item_Info->theType )
			{
				// 칼검기(잔상)만들기 위해서 높이값 구하기
				m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]]->m_AxisTop;
				m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]]->m_AxisBottom;					
				
				g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );
			}
		}
		
	}
	
	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode )
	{
		//		item_Info = g_Pc_Manager.GetItemBaseInfo( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode );
		
		//		if(			item_Info->theType == 1 ||			// 검
		//					item_Info->theType == 2 ||			// 대검		
		//					item_Info->theType == 5 ||			// 스태프 
		//					item_Info->theType == 6 )			// 수정구 
		//		{
		
		IsEquipWeapon = true;
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode , this );
		
		m_pUserInfo->ArmorInfo.index[0] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode );
		m_pUserInfo->ArmorInfo.Code[0] = g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode;
	
		// 칼검기(잔상)만들기 위해서 높이값 구하기
		m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisTop;
		m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisBottom;					
		
		g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );
		
		//.................................................................................................
		// 근접 무기 체크 By wxywxy
		//.................................................................................................
		m_ISApproachWeapon = FALSE;			// 근접 무기 유/무
		m_AttackMotion	   = 0;				// 아이템 공격모션 번호
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode);
		
		if( NULL != item_Info )
		{
			if( item_Info->theType == n_Type_ShortWeapon )
			{
				m_ISApproachWeapon = TRUE;
				m_AttackMotion	   = item_Info->theAttackMotion;
			}
		}
		//.................................................................................................
		
		m_pUserInfo->bAttack = true;
	}
	
#ifdef RIDING_SYSTEM	
	  
	if( m_State.bRide )
	{	
		g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[0] = -1;
		g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[1] = -1;
		g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[2] = -1;		
		
		if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_RidingItem1].theCode )
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[0] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_RidingItem1].theCode );
			g_Map.m_Npc[m_State.TransIndex].SetRideItemBuffer( 0 );
			
		}
		
		if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_RidingItem2].theCode )
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[1] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_RidingItem2].theCode );
			g_Map.m_Npc[m_State.TransIndex].SetRideItemBuffer( 1 );
		}
		
		if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_RidingItem3].theCode )
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[2] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_RidingItem3].theCode );
			g_Map.m_Npc[m_State.TransIndex].SetRideItemBuffer( 2 );
		}
	}
	
#endif
	
	if( m_bPC && IsEquipWeapon && !m_State.bRide )
	{
		///-- 아이템을 교체할시에... 해당 아이템이 연결된 노드가 애니메이션에 따라 
		///-- 틀리게 설정되어있다. 바로 장비를 장착한 애니로 전환 
		///-- 블렌딩을 피하고 아이템을 렌더링한다. 
		AnimationPlay( n_Stand_Peace9 );
		m_ani_frame = c_MotionBlendHalfFrame;
		m_blend_frame = c_MotionBlendFrame;	
	}
	
	//PreCharTextureCreate();
	PreTextureLoad();
	
	return;
}

void NonPlayer::SetPlayer_NpcWearItem()
{
	SItemBaseInfo* item_Info = NULL;
	int chrAttrIndex = GetAttrIndex();
	
	bool IsEquipWeapon = false;
	
	m_Char_WearType = CHAR_WEAR_NORMAL;
	
	if( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_CoatArmor] )
	{
		m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_CoatArmor] );
		SetItemBuffer( n_Wear_CoatArmor );
		
		///-- 셋트 아이템 웨어처리
		SItemBaseInfo* pSItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_CoatArmor] );
		if( pSItemBaseInfo &&  n_Type_SetCloth == pSItemBaseInfo->theType)
		{
			m_Char_WearType = CHAR_WEAR_SETITEM;	
			
			g_Particle.m_EffectManager.CreateDefenceEnChantEffect(this , 255 );
		}
		else
		{
			m_Char_WearType = CHAR_WEAR_NORMAL;	
		}		
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_CoatArmor] , this );
	}
	
	if( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_PantsArmor] )
	{
		m_pItm_index[n_Wear_PantsArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_PantsArmor] );
		SetItemBuffer( n_Wear_PantsArmor );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_PantsArmor] , this );
	}
	
	if( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Gloves] )
	{
		m_pItm_index[n_Wear_Gloves] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Gloves] );
		SetItemBuffer( n_Wear_Gloves );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Gloves] , this );
	}
	
	if( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Shoes] )
	{
		m_pItm_index[n_Wear_Shoes] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Shoes] );
		SetItemBuffer( n_Wear_Shoes );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Shoes] , this );
	}
	
	if( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Helmet] )
	{
		m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Helmet] );
		SetItemBuffer( n_Wear_Hair );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Helmet] , this );
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Helmet] );
		if( item_Info && item_Info->theType == n_Type_KeepupHair )	// 헤어도 동시에 그려줘야 된다 (악세사리 형)
		{
			m_Basic_DrawItm[n_Wear_Hair] = 1;		
			m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theHair ); 
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
		m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theHair );
		SetItemBuffer( n_Wear_Hair );	
	}
	
	if( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theFace ) 
	{
		m_pItm_index[n_Wear_Face] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theFace );
		SetItemBuffer( n_Wear_Face );	
	}
	
	if( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Serve] )
	{
		m_pItm_index[n_Wear_Serve] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Serve] ); // 아이템 코드에 맞는 텍스쳐 번호 로딩  
		SetItemBuffer( n_Wear_Serve );	
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Serve] , this );
	}
	
	if( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Shield] )
	{
		IsEquipWeapon = true;
		
		m_pUserInfo->ArmorInfo.index[1] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Shield] );
		m_pUserInfo->bAttack = true;
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Shield] , this );
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Shield] );
		
		if( item_Info )
		{
			if( n_Type_Bow == item_Info->theType || n_Type_MagicBook == item_Info->theType )
			{
				// 칼검기(잔상)만들기 위해서 높이값 구하기
				m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]]->m_AxisTop;
				m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]]->m_AxisBottom;					
				
				g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );
			}
		}		
	}
	
	if( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Weapon] )
	{
		IsEquipWeapon = true;
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Weapon] , this );
		
		m_pUserInfo->ArmorInfo.index[0] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Weapon] );
		
		// 칼검기(잔상)만들기 위해서 높이값 구하기
		m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisTop;
		m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisBottom;					
		
		g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );
		
		//.................................................................................................
		// 근접 무기 체크 By wxywxy
		//.................................................................................................
		m_ISApproachWeapon = FALSE;			// 근접 무기 유/무
		m_AttackMotion	   = 0;				// 아이템 공격모션 번호
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Weapon] );
		
		if( NULL != item_Info )
		{
			if( item_Info->theType == n_Type_ShortWeapon )
			{
				m_ISApproachWeapon = TRUE;
				m_AttackMotion	   = item_Info->theAttackMotion;
			}
		}
		//.................................................................................................
		
		m_pUserInfo->bAttack = true;
	}
	
//	PreCharTextureCreate();
	PreTextureLoad();
	
	return;
}

//-----------------------------------------------------------------------------
// Desc: 캐릭터 정보대로 구성하기
//-----------------------------------------------------------------------------
void NonPlayer::CreateItem()
{		

}


//-----------------------------------------------------------------------------
// Desc: 생성 처리
//-----------------------------------------------------------------------------
void NonPlayer::RespawnProcess()
{	
	//-----------------------------------------------------------------------------
	// 생성 애니메이션이 끝나지 않았어야 한다
	//-----------------------------------------------------------------------------
	if( m_ani_frame != m_end_frame )
	{
		// NPC
		if( m_bPC == false )
		{				
			// 땅(1), 하늘(2) 에서 반투명으로 나타나기
			//			if( g_Pc_Manager.m_NpcAttr[GetAttrIndex()].respawn == 1 || 
			//				g_Pc_Manager.m_NpcAttr[GetAttrIndex()].respawn == 2 )
			{		
				float value = (float)m_ani_frame/(float)m_end_frame;
				//for( int i = 0; i < m_itm_count; ++i )
				
				int itm_count = 0;
				
				itm_count = g_Pc_Manager.GetItmCount( this );
				
				for( int i = 0; i < itm_count; ++i )
				{
					if( m_pMaterial[i].Diffuse.a < 1.0f )					
						m_pMaterial[i].Diffuse.a = value*2.0f;					
				}
			}		
		}	
	}
	//-----------------------------------------------------------------------------
	// 애니메이션끝 - 서있기 자세로, 블렌딩없애기
	//-----------------------------------------------------------------------------
	else
	{
		int itm_count = 0;
		
		itm_count = g_Pc_Manager.GetItmCount( this );
		
		//for( int i = 0; i < m_itm_count; ++i )
		for( int i = 0; i < itm_count; ++i )
			m_pMaterial[i].Diffuse.a = 1.0f;		
		m_curt_event.Clear();
		m_SimTime = g_nowTime;		
		AnimationPlay( n_NPC_Stand0 );
	}
}


//-----------------------------------------------------------------------------
// Desc: 소멸 처리
//-----------------------------------------------------------------------------
int NonPlayer::DieProcess()
{		
	float value = SAFE_TIME_SUB( SAFE_TIME_SUB( g_nowTime , m_PreTime ) , NPC_DIE_TIME ) ;
	
	if( SAFE_TIME_COMPARE( m_curt_event.time , <= , g_nowTime ) && m_ani_frame == (int)m_end_frame )
	{
		int	nItmCount;		
		nItmCount = g_Pc_Manager.GetItmCount( this );
		
		for( int i = 0; i < nItmCount; ++i )
		{
			if( m_bPC == true )
			{
				if( m_pMaterial[i].Diffuse.a >= 0.55f )
				{
					value = (float)1.0f-((float)value/(float)NPC_DIE_BLENDTIME);
					m_pMaterial[i].Diffuse.a = value;
				}
			}
			else
			{
				if( m_pMaterial[i].Diffuse.a >= 0.0f )
				{
					value = (float)1.0f-((float)value/(float)NPC_DIE_BLENDTIME);
					m_pMaterial[i].Diffuse.a = value;
				}
				else	
				{
					g_Map.SM__NPC__DISAPPEAR( 0, g_Map.m_NpcTable[m_nTableIndex].lUnique );					
				}
			}
			
		}
		
		m_bShadowDisplay = false;
	}

	return 0;	

	// 죽는 애니메이션중이고.. 마지막 프레임이라면..
	//	else if( m_ani_index == n_NPC_Die0 && m_ani_frame == (int)m_end_frame )
	if( SAFE_TIME_COMPARE( m_curt_event.time , <= , g_nowTime ) && m_ani_frame == (int)m_end_frame )
	{
		//-----------------------------------------------------------------------------
		// 죽는 애니메이션이후에 블렌딩없이그냥 사라짐
		//-----------------------------------------------------------------------------
		//		if( g_Pc_Manager.m_NpcAttr[GetAttrIndex()].die == 2 && m_ani_frame == m_end_frame )		
		//			return SM_NPC_DIE;	
		//-----------------------------------------------------------------------------
		// 땅(1), 하늘(1) 에서 반투명으로 사라지기
		//-----------------------------------------------------------------------------
		//			if( g_Pc_Manager.m_NpcAttr[GetAttrIndex()].die == 1 )
		{
			float value = SAFE_TIME_SUB( SAFE_TIME_SUB( g_nowTime , m_PreTime ) , NPC_DIE_TIME );
			
			int itm_count = 0;
			
			itm_count = g_Pc_Manager.GetItmCount( this );
			
			//for( int i = 0; i < m_itm_count; ++i )
			for( int i = 0; i < itm_count; ++i )
			{
				if( m_pMaterial[i].Diffuse.a >= 0.0f )
				{
					value = (float)1.0f-((float)value/(float)NPC_DIE_BLENDTIME);
					m_pMaterial[i].Diffuse.a = value;
				}
			}
		}

		// 그림자 끄기 
		m_bShadowDisplay = false;
		//		return -1;
	}

	// 파괴오브젝트는 리턴시킨다.. 
	// 죽는 애니메이션을 하지 않았다면.. 프레임증가시키면 안되기 때문에...
	//	if( GetAttrIndex() == DESTROY_OBJECT_INDEX && m_ani_index != NPC_DEATH0 )
	//		return -1;

	return 0;
}

void NonPlayer::PushQueue(EVENT_DATA_INFO & data, BOOL IsAllClear)
{	
	data.lStartTime = timeGetTime();	
	
	///-- 죽는 이벤트가 들어오면 기존공격 에대한 상황을 일괄처리한다.
	if( data.type == SM_STATE_DIE )
	{
		IsAllClear = TRUE;
	}

	///-- 현재 동작 대상이 펫이고 이벤트가 CM_MOVE_ATTACK이 들어올 경우
	if( data.type == CM_MOVE_ATTACK )
	{
		///-- 대상과 현재 펫의 사거리를 체크해서... CM_MOVE_ATTACK을 처리할것인지
		///-- 바로 어택요청 패킷을 보낼것인지 결정한다.
		
		int range = 4;
	
		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].code );
		if(npc_Info)
		{
			range = npc_Info->theAttackRange;
		}
		//해당 위치에 도달하면 .. 공격 메세지를 보낸다 . 
		bool bFind = FindNpcTile( m_Mov.curt.x, m_Mov.curt.y, range, data );	
		if(bFind)
		{
			CM__ATTACK__NPC( data );
			return;
		}
	}
	
	//현재 스킬중 이고 현재들온 메세지가 SM_MOVE_ATTACK 이면 이동해라 .. 
#ifdef NPC_MOVE_TO_SKILL
	if( (m_curt_event.type == SM_USE_SKILL || m_curt_event.type == SM_MOVE_SKILL ) 
		&& data.type == SM_MOVE_ATTACK )
#else 
	if( m_curt_event.type == SM_USE_SKILL && data.type == SM_MOVE_ATTACK )
#endif 
	{
		data.type = SM_MOVE_BROADCAST;
		theAttack_Event.Clear();
		IsAllClear = FALSE;
	}
	
	if( IsAllClear )
	{
		AllActQueue();
		SetQueue( data );
		return;
	}
	
	if(  m_curt_event.type == SM_ATTACK_NPC || 
		( m_curt_event.type == SM_USE_SKILL && ( m_curt_event.status != SM_CHANT_SPELLING || !m_bPC ) ) )
	{	
		if( m_curt_event.type == SM_USE_SKILL )
		{
#ifdef NPC_MOVE_TO_SKILL
			if( data.type != SM_USE_SKILL &&  data.type != SM_MOVE_SKILL  )
#else 
			if( data.type != SM_USE_SKILL )
#endif 
			{
				SetUnderAttackEventMgr( data );
				return;
			}
		}		
#ifdef NPC_MOVE_TO_SKILL
		if(  m_next_event.type == SM_ATTACK_NPC || m_next_event.type == SM_USE_SKILL || m_next_event.type == SM_MOVE_SKILL  ) 	
#else 
		if(  m_next_event.type == SM_ATTACK_NPC || m_next_event.type == SM_USE_SKILL ) 
#endif 
	
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

void NonPlayer::AllActQueue()
{

	if(  m_curt_event.type == SM_ATTACK_NPC || 
		( m_curt_event.type == SM_USE_SKILL && ( m_curt_event.status != SM_CHANT_SPELLING || !m_bPC ) ) )
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
		m_next_event.type == SM_USE_SKILL 
#ifdef NPC_MOVE_TO_SKILL
		|| m_next_event.type == SM_MOVE_SKILL		
#endif
		)
	{
		SetQueue( m_next_event );
		m_next_event.Clear();
		SetUnderAttackEventMgr( m_curt_event );		
	}
}

//-----------------------------------------------------------------------------
// Desc: Character message processing
//-----------------------------------------------------------------------------
void NonPlayer::SetQueue( const EVENT_DATA_INFO& data )
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
	
	//.......................................................................................................
	// When NPC dies NPC.that is (left-handed affect) output
	//.......................................................................................................
	if( FALSE == m_bPC &&
		data.type == SM_STATE_DIE )
	{
		SNPCBaseInfo* npc_Info = NULL;
		
		NPCTABLE *pTable = g_Map.GetNpcTable( GetUnique() );
		
		if( NULL != pTable )
		{
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( pTable->code ); 
			
			if( NULL != npc_Info )
			{
				if( npc_Info->theLefeEff != -1 )
				{
					g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) npc_Info->theLefeEff , ( Character * ) this , NULL , NULL );
				}
			}
		}
	}
	
	//Do you have a message to handle? Can this be? You see ...
#ifdef NPC_MOVE_TO_SKILL
	if( m_temp_event.type == SM_ATTACK_NPC || m_temp_event.type == SM_USE_SKILL || m_temp_event.type == SM_MOVE_SKILL )
#else 
	if( m_temp_event.type == SM_ATTACK_NPC || m_temp_event.type == SM_USE_SKILL )
#endif 
	
	{
		//		RLGS("m_temp_event deleted??? 설마 이거 때문이랴??!!!");
		RLGS("NonPlayer.cpp --- line:641");				
#ifdef ROCKCLIENT_DEV					
		//		SystemPrint( n_DarkRed, n_SystemMsg, "SetQueue 몹이 때렸음~~~ SM_ATTACK_NPC" );				
#endif		
		SetUnderAttackEventMgr( m_temp_event );

	}
	else if( m_temp_event.type == SM_STATE_DIE )
	{
		SM__STATE__DIE( m_temp_event );
	}
	
	
#ifdef ROCKCLIENT_DEV					
	if( data.type == SM_STATE_DIE )
	{
		RLGS( "NonPlayer 죽었삼~~ ㅜ ㅜ" );				
	}
#endif
	
	m_temp_event.Clear();
	m_temp_event = data;	
	QueueProcess();
}


void NonPlayer::QueueProcess()
{
	if( m_temp_event.type != NULL )
	{
		int nCurstatus = m_curt_event.status;		
		int nTemstatus = m_temp_event.status;		
		
		EVENT_DATA_INFO data;
		data = m_temp_event;
		m_temp_event.Clear();
		
		//		RLG1("NonPlayer data.type = %d", data.type);
		//		RLG1("NonPlayer m_curt_event.type = %d", m_curt_event.type);
		
		
		if( m_curt_event.type == SM_SLIDE_NPC || m_curt_event.type == SM_STATE_DIE )
		{
#ifdef ROCKCLIENT_DEV						
			if( data.type == SM_ATTACK_NPC || data.type == SM_USE_SKILL )
			{
				if( m_bPC )
				{
					RLGS( "!NonPlayer 가 죽어 있는 상태임...but SM_ATTACK_NPC 메시지가 날아옴!!!" );
				}
				else
				{				
					RLGS( "!Monster 가 죽어 있는 상태임...but SM_ATTACK_NPC 메시지가 날아옴!!!" );
				}
			}
#endif 
			m_save_event.Clear();
			m_next_event.Clear();
			return;
		}
		
		// 다른 유저는 주는대로 처리하니까 모든 경우가 다 생겨날 수 있을듯...ㅡㅡ; 흠...
		if( data.type == SM_MOVE_BROADCAST || data.type == SM_MOVE_ATTACK || data.type == CM_MOVE_ATTACK ||
			data.type == SM_UNDERATTACK || data.type == SM_SLIDE_NPC || data.type == SM_STATE_DIE || data.type == SM_SELF_DIE )
		{
			if( data.type != SM_SELF_DIE )
			{
				// 죽을때 죽더라도 내가 때린놈은 죽이던지 때리던지 해야지...^ ^V
				if( m_curt_event.type == SM_ATTACK_NPC || 
					( m_curt_event.type == SM_USE_SKILL && m_curt_event.status == SM_USE_SKILL ) )
				{
					if( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )				// 유저가 때림  
					{
						if( m_ani_frame <= g_Pc_Manager.GetImpactFrame(this, m_ani_index) && m_curt_event.damage >= 0 )
						{						
							RLGS("NonPlayer.cpp --- line:691");				
							SetUnderAttackEventMgr( m_curt_event );
							m_curt_event.Clear();
						}
					}
					else
					{
						if( m_ani_frame <= m_NPCImpactFrame && m_curt_event.damage >= 0 )
						{
							RLGS("NonPlayer.cpp --- line:703");				
#ifdef ROCKCLIENT_DEV					
							//		SystemPrint( n_DarkRed, n_SystemMsg, "QueueProcess c_MobAttackFrame 몹이 때렸음~~~ SM_ATTACK_NPC" );				
#endif		
							SetUnderAttackEventMgr( m_curt_event );
							m_curt_event.Clear();
						}
					}				
				}
			}
			else
			{
				data.type = SM_STATE_DIE;
			}
			
			if( m_next_event.type == SM_ATTACK_NPC || 
				( m_next_event.type == SM_USE_SKILL && m_next_event.status == SM_USE_SKILL ) )
			{
				EVENT_DATA_INFO next_attack;
				
				next_attack = m_next_event;
				
				m_next_event.Clear();
				
				RLGS("NonPlayer.cpp --- line:710");				
				SetUnderAttackEventMgr( next_attack );
				
			}
		}
		
		switch( data.type )
		{		
		case CM_MOVE_ATTACK:		
		case SM_MOVE_ATTACK:
		case SM_MOVE_BROADCAST:
#ifdef NPC_MOVE_TO_SKILL
		case SM_MOVE_SKILL:
#endif 
			{				
				if(theFindWay)
				{
					m_Mov.vStPos.x = m_Mov.vCVtPos.x;
					m_Mov.vStPos.z = m_Mov.vCVtPos.z;
				}
				if(m_State.nPetState == nPetState_Pet)
				{
					if( SM__MOVE__BROADCAST( data ) == false )
						return;
				}
#ifdef NPC_MOVE_TO_SKILL
				else if(m_State.nActiveState == nActiveTypeMove )
				{
					//여기서 이펙트 뿌린다 . 
					g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) 510 , this , NULL , NULL );
					//g_Particle.m_EffectManager.Delete( ( EFFECT_ID ) item_Info->theLinkEff , ( Character * ) pSrc , NULL );
					m_State.bWalk = false; 
					m_ani_percent = 0.1f;
					

					if( SM__MOVE__BROADCAST( data ) == false )
						return;
				}
#endif 
				else
				{
					if( SM__MOVE( data ) == false )
						return;
				}
			}
			break;		
		case SM_ATTACK_NPC:		// 공격
			{
#ifdef ROCKCLIENT_DEV					
				//		SystemPrint( n_DarkRed, n_SystemMsg, "몹이 때렸음~~~ data.type = SM_ATTACK_NPC" );				
				RLGS( "몹이 때렸음~~~ data.type = SM_ATTACK_NPC" );				
#endif		
				
				if( SM__ATTACK__NPC( data ) == false )
					return;
			}
			break;	
		case SM_UNDERATTACK:	// ReAction 
			{
				if( SM__UNDERATTACK( data ) == false )
					return;
			}
			break;
		case SM_STATE_DIE:	
			{
				if( SM__STATE__DIE( data ) == false )
					return;
			}
			break;
		case SM_SLIDE_NPC:		// 밀려나기
			{
				//				DebugPrint("SM__SLIDE__NPC\n");
				SM__SLIDE__NPC( data );
			}
			break;
		case SM_USE_SKILL:
			{
				SM__USE__SKILL( data );
			}
			break;
		case SM_ITEM_DROPING:
			{
				SM__ITEM__DROP( data );
			}
			break;
		case SM_ATTACK_DELAY:
			{
				if( m_curt_event.type != SM_MOVE_BROADCAST )
				{
					//				SystemPrint( n_DarkRed, n_SystemMsg, "==SM_ATTACK_DELAY== m_curt_event.type = %d", m_curt_event.type );
					m_curt_event = data;			
				}
			}		
			break;	
		case SM_BOW_ATTACK:
			{
				SM__BOW_ATTACK( data );
			}
			
			// 로그인 관련 ( 제자리에서 액션 테이블 처리하기 ) By wxywxy
		case SM_ANI_QUEUE_PROCESS:
			{
				SM__ANI_QUEUE_PROCESS( data );
			}
			
		case SM_ACT:	
			{
				SM_ACT_PROCESS( data );
			}
			break;
			
		}	
		
		// 현재시간저장
		m_PreTime = g_nowTime;
	}	
}

//.................................................................................................................
// 개인 상점 처리
//.................................................................................................................
bool NonPlayer::SM_USER_STORE(EVENT_DATA_INFO &data)
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



// 로그인 관련 ( 제자리에서 액션 테이블 처리하기 ) By wxywxy
bool NonPlayer::SM__ANI_QUEUE_PROCESS( EVENT_DATA_INFO& data )
{
	//SM_ANI_QUEUE_PROCESS
	m_curt_event = data;
	
	AnimationPlay( data.motion );
	
	return true;
}

bool NonPlayer::SM__CHANT__READY( EVENT_DATA_INFO& data )
{
	m_curt_event = data;
	
	//	SetDirection( data.vPos.x, data.vPos.z );
	AnimationPlay( data.motion );
	
	if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
	{
		m_IsCDataQActed = FALSE;
	}
	
	return true;
}

bool NonPlayer::SM__CHANT__SPELL( EVENT_DATA_INFO& data )
{
	m_curt_event = data;
	
	AnimationPlay( data.motion );
	
	return true;
}

bool NonPlayer::SM__RESTSKILL( EVENT_DATA_INFO& data )
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

bool NonPlayer::SM__USE__SKILL( EVENT_DATA_INFO& data )
{
	switch( data.status )
	{
	case SM_CHANT_READY:
		{
			// 영창 시작
			if(m_bPC)
			{
				g_Particle.m_EffectManager.GetSkillStartSound( data.count , (Character *)this );
			}
			
			m_SpellEffect.SetSpellEffect( data.count , 0 , GetUnique() , GetIndex() );
			
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
	
	m_IsCDataQActed = FALSE;
	
	// use skill
	m_curt_event = data;
	
	SSkillBaseInfo* skill_Info;
	skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( data.count ); 
	if( skill_Info == NULL )
		return false;
	
	//............................................................................................................
	// 위치가 고정되있는 몬스터 체크 By wxywxy
	//............................................................................................................
	BOOL ISSetDir = TRUE; 
	
	if( !m_bPC )
	{
		if( g_Map.m_NpcTable[ m_nTableIndex ].code >= c_NotMoveNPCCode_Min && g_Map.m_NpcTable[ m_nTableIndex ].code <= c_NotMoveNPCCode_Max )
		{
			ISSetDir = FALSE;
		}

		if( m_State.nCharacterType == nCharacterTypeBulding )
		{
			ISSetDir = FALSE;
		}
	}
	
	if( ISSetDir )
	{
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
	}
	//............................................................................................................
	
	if( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		int chrAttrIndex = GetAttrIndex();
		int chrClassType = 0;
		
		if( m_bPC )
		{
			chrClassType = g_Pc.m_PcTable[ m_nTableIndex ].char_info.theMainClass;
		}
		else if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
		{
			chrClassType = g_Map.m_NpcTable[ m_nTableIndex ].theExPcInfo.theMainClass;
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
			chrClassType == Swordman || chrClassType == Knight || 
			chrClassType == Mercenary || chrClassType == Slayer ) 
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
			chrClassType == Magician || chrClassType == Prominas || chrClassType == Priest || chrClassType == Holy_Avenger ) 		 
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
		
	}
	else
	{			
		if(!m_State.bRide)
		{			
			
#ifdef UPDATE_NPCSKILL		
			///-- 스킬에 따라 모션을 분리한다.	
			SNPCBaseInfo* npc_info = NULL;		
			npc_info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].code );

			if(npc_info == NULL)
			{
				return false;
			}

			for(int i = 0 ; i < NPCSKILLNUM; ++i)
			{
				if( npc_info->theSkillCode[i] == data.count )
				{
					int nResult = 0;			
					
					if( i == 0 )
					{
						nResult = AnimationPlay( n_NPC_Attack2 );
					}
					else
					{
						nResult = AnimationPlay( n_NPC_Attack3 );			
					}

					if(nResult == -1)
					{
						///-- 스킬은 적용되는데 애니메이션이 없는경우 -  예: 가드타워
						m_IsSkillNoAni = TRUE;
					}			
				}
			}

			if( m_State.nActiveState == nActiveTypeSKill )
			{
		//		m_ani_percent = 0.1f; 
			}


#else
			int nResult = 0;			
			nResult = AnimationPlay( n_NPC_Attack2 );
			if(nResult == -1)
			{
				///-- 스킬은 적용되는데 애니메이션이 없는경우 -  예: 가드타워
				m_IsSkillNoAni = TRUE;
			}	
#endif
		}
	}
	
	return true;
}

bool NonPlayer::SM_ACT_PROCESS( EVENT_DATA_INFO& data )
{
	m_curt_event = data;	
	
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


bool NonPlayer::SM__BOW_ATTACK( EVENT_DATA_INFO& data )
{
	switch( data.status )
	{
	case SM_BOW_READY:
		m_curt_event = data;
		
		if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
		{
			m_IsCDataQActed = FALSE;
		}
		
		AnimationPlay( data.motion );
		break;
		
	case SM_BOW_WAIT:
		
		m_curt_event = data;
		AnimationPlay( data.motion );
		
		break;
		
	default:
		break;
	}
	
	return TRUE;
}

bool NonPlayer::SM__ITEM__DROP( EVENT_DATA_INFO& data )
{	
	m_curt_event = data;
	
	AnimationPlay( n_NPC_ReAction0 );
	
	return true;
}


//-----------------------------------------------------------------------------
// Desc: 이동
//-----------------------------------------------------------------------------
bool NonPlayer::SM__MOVE( EVENT_DATA_INFO& data )
{	
	//-----------------------------------------------------------------------------
	// 목적지 저장
	//-----------------------------------------------------------------------------
	m_Mov.dest.x = data.x;
	m_Mov.dest.y = data.y;
	
	// 현재이벤트에 따라서 처리
	switch( m_curt_event.type )
	{
	case SM_MOVE_BROADCAST:		// 이동		
		return false;		
		break;
	}
	
	//-----------------------------------------------------------------------------
	// 애니메이션 플레이
	//-----------------------------------------------------------------------------		
	if( m_bPC == true )
	{
		if(!m_State.bRide)
			AnimationPlay( n_Run9 );
	}
	else
	{
		if( m_State.nCharacterType == nCharacterTypePlayerNPC )
		{
			AnimationPlay( n_Walk9 );
		}
		else
		{
			AnimationPlay( n_NPC_Walk0 );
		}
	}
	
	//-----------------------------------------------------------------------------	
	// 이벤트저장
	//-----------------------------------------------------------------------------	
	m_curt_event = data;
	
#ifdef NPC_MOVE_TO_SKILL
	if( m_bPC || m_State.nPetState == nPetState_Pet || m_State.nActiveState == nActiveTypeMove )
#else 
	if( m_bPC || m_State.nPetState == nPetState_Pet )
#endif 
	{
		int nCount = GetMax( abs((long)m_Mov.dest.x- (long)m_Mov.curt.x), abs((long)m_Mov.dest.y- (long)m_Mov.curt.y) );
		
		if( FindPath( (short)m_Mov.curt.x, (short)m_Mov.curt.y, nCount ) == true )
		{
			float step_x, step_y;
			GetMoveStep( m_Mov.path.x, m_Mov.path.y, m_Mov.curt.x, m_Mov.curt.y, step_x, step_y );
			m_Mov.next.x = m_Mov.curt.x+(int)((step_x*GetRunFrame())/CLIENT_TILESIZE);
			m_Mov.next.y = m_Mov.curt.y+(int)((step_y*GetRunFrame())/CLIENT_TILESIZE);
		}
	}
	// 해당방향으로 이동
	//	SetMoveStep( m_Mov.dest );
	//	SetRotate( m_Mov.dest );
	//	GetNextTile();
	//	SetHeightStep();
	return true;
}

bool NonPlayer::SM__MOVE( EVENT_DATA_INFO& data, bool bFindNpc )
{	
	if( !m_bPC )
	{	
		if( m_State.nCharacterType == nCharacterTypePlayerNPC )
		{
			AnimationPlay( n_Walk9 );
		}
		else
		{
			AnimationPlay( n_NPC_Walk0 );
		}
	}
	
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
		if( FindPath( (short)m_Mov.curt.x, (short)m_Mov.curt.y, count ) == true )
		{			
			//-----------------------------------------------------------------------------
			// 멈춰있다면
			//-----------------------------------------------------------------------------
#ifdef NPC_MOVE_TO_SKILL
			if( m_curt_event.type != SM_MOVE_BROADCAST && m_curt_event.type != SM_ATTACK_DELAY 
				&& m_curt_event.type != SM_MOVE_SKILL )
#else 
			if( m_curt_event.type != SM_MOVE_BROADCAST && m_curt_event.type != SM_ATTACK_DELAY)
#endif 
	
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
									if(m_State.PetIndex == MAX_USER_INTABLE)
									{
										CM__ATTACK__NPC( m_save_event );
									}
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
									if(m_State.PetIndex == MAX_USER_INTABLE)
									{
										CM__ATTACK__NPC( m_save_event );
									}		
								}
							}
						}					
					}						
					g_Map.SetMapData( NPC_BLOCK, m_nTableIndex, g_Map.m_NpcTable[m_nTableIndex].lDestBlock, false );
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
				if( !m_bPC )
				{	
					if( m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						AnimationPlay( n_Run9 );
					}
					else
					{
						AnimationPlay( n_NPC_Run0 );
					}
				}					
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
				
				dAttackDestCell = data.cell;
				ChangeState(CAttackState::Instance());
				
				theIsAttack = false;
				theAttackDelayTime = g_nowTime;
				
				return false;
			}
		}
		//-----------------------------------------------------------------------------
		// 길찾기 실패했다면.. 초기화한다.
		//-----------------------------------------------------------------------------
		else
		{			
			m_curt_event.Clear();
			
			if( !m_bPC )
			{	
				if( m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					AnimationPlay( n_Stand_Peace9 );
				}
				else
				{
					AnimationPlay( n_NPC_Stand0 );
				}
			}			
			
			m_Mov.curt.x = g_Pc.GetDestX();
			m_Mov.curt.y = g_Pc.GetDestY();
			m_Mov.curt.height = g_Pc.GetMapHeight( m_Mov.curt.x, m_Mov.curt.y );
			m_Mov.path = m_Mov.next = m_Mov.dest = m_Mov.curt;			
			return false;
		}		
		
		// 이동이벤트 저장
		//		SystemPrint( n_DarkRed, n_SystemMsg, "SM_MOVE() => SM_MOVE_BROADCAST" );
#ifdef NPC_MOVE_TO_SKILL
		if( data.type == SM_MOVE_SKILL )
		{
			m_save_event = data;
			m_curt_event.type = SM_MOVE_SKILL; 
			m_curt_event.x    = m_Mov.dest.x;
			m_curt_event.y    = m_Mov.dest.y;
		}
		else
		{
			m_curt_event.type = SM_MOVE_BROADCAST;
			m_curt_event.x    = m_Mov.dest.x;
			m_curt_event.y    = m_Mov.dest.y;
		}

#else 

		m_curt_event.type = SM_MOVE_BROADCAST;
		m_curt_event.x    = m_Mov.dest.x;
		m_curt_event.y    = m_Mov.dest.y;

#endif 
		//m_save_event.Clear();		
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// Desc: 다음타일찾기
//-----------------------------------------------------------------------------
int NonPlayer::GetNextPath( DWORD& next_x, DWORD& next_y, EVENT_DATA_INFO& data )
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
			
			SNPCBaseInfo* npc_Info;
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].code );
			if(npc_Info)
			{
				range = npc_Info->theAttackRange;
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
	return 1;
}

void NonPlayer::CheckTargetTile( EVENT_DATA_INFO& data )
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
			
			real_range = GetMax( abs((float)nPosX - (float)data.x ), abs((float)nPosY - (float)data.y ) );
		}
	}
	
	return;
}

bool NonPlayer::CM__ATTACK__NPC( EVENT_DATA_INFO& data )
{
	int nADisErrCnt = 0;
	
	///-- By simwoosung -- 공격시 서버와 클라이언트 거리에러 발생 확인용 변수값 보관 
	if( &m_save_event == &data )
	{
		nADisErrCnt = m_save_event.nADisErrCnt;
	}
	
	//	RLGS( "***********************************CM__ATTACK__NPC()" );
	// 공격보다 이동이 우선..컨트롤시 갑갑하게 느껴진다는 의겸 수렴. 
	if( theAttack_Event.type == SM_MOVE_ATTACK )		
	{
		//		SystemPrint( n_DarkRed, n_SystemMsg, "==return FALSE== CM__ATTACK__NPC() => theAttack_Event,GetActiveCsStartMoveFlag()" );
		RLG1( "theAttack_Event.type = %d", theAttack_Event.type );
		
		data.Clear();
		
		return FALSE;
	}	
	
	if( g_Pc.GetPlayer()->m_State.PetIndex == m_nTableIndex )
	{
		DWORD aCell =  g_Map.m_NpcTable[m_nTableIndex].lDestBlock;
		DWORD aPetID = 	g_Map.m_NpcTable[m_nTableIndex].lUnique;
		g_RockClient.Send_CsPetStop( aPetID, aCell );
	}
	
	data.type = SM_ATTACK_NPC;
	
	if( data.motion == 0 )
		data.motion = n_NPC_Attack0;
	
	int attack_type = 0;
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
	}
	else								// 유저
	{
		if( g_Pc.m_Pc[data.tbl_Idx].m_curt_event.type == SM_STATE_DIE )
		{
			//			SystemPrint( n_DarkRed, n_SystemMsg, "이미 죽어있는 타겟입니다" );
			data.Clear();
			return false;
		}
		
		attack_type	 = n_PC;
		data.x		 = g_Pc.m_PcTable[data.tbl_Idx].lDestBlock%g_Pc.m_cSize;
		data.y		 = g_Pc.m_PcTable[data.tbl_Idx].lDestBlock/g_Pc.m_cSize;
		data.vPos.x	 = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._41;
		data.vPos.y  = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._42;
		data.vPos.z  = g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._43;
	}
	
	//	int range = CLIENT_TILESIZE * 5;
	int range = 4;
	
	SNPCBaseInfo* npc_Info;
	npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].code );
	if(npc_Info)
	{
		range = npc_Info->theAttackRange;
	}
	
	bool bFind = FindNpcTile( m_Mov.curt.x, m_Mov.curt.y, range, data );	
	
	if( bFind == false )							
	{			
		EVENT_DATA_INFO attackNPC_event;
		attackNPC_event.type = CM_MOVE_ATTACK;
		attackNPC_event.motion = n_NPC_Attack0;
		attackNPC_event.tbl_Idx = data.tbl_Idx;

		///-- 펫이 이동해서 공격해야 하므로... 현재 타입을 0으로 셋팅한다.
		m_curt_event.type = 0;
		
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
			g_Pc.GetPlayer()->PetSetAttackQueue(attackNPC_event);

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
			g_Pc.GetPlayer()->PetSetAttackQueue(attackNPC_event);
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
	
	m_attack_type = race_type;
	theAttackedID = data.unique;	
	
	int attackedIdx = MAX_NPC_INTABLE;
	if( m_attack_type == n_NPC )
	{
		attackedIdx = g_Map.FindNpc(theAttackedID);
		if( MAX_NPC_INTABLE == attackedIdx )
		{
			theAttackedID = 0;
			ChangeState(CNormalState::Instance());
			return false;
		}	
	}
	else
	{
		attackedIdx = g_Pc.FindUser(theAttackedID);
		if( MAX_USER_INTABLE == attackedIdx )
		{
			theAttackedID = 0;
			ChangeState(CNormalState::Instance());
			return false;
		}	
	}	
	
	int npc_tbl_index = g_Pc.GetPlayer()->m_State.PetIndex;
	DWORD aPetID = (DWORD)g_Map.m_NpcTable[npc_tbl_index].lUnique;
	
	///--PRINT_DLG_LOG("ScPetAttack");
	g_RockClient.Send_CsPetAttack(aPetID, theAttackedID, m_attack_type);
	
	theAttackedID = 0;
	
	data.Clear();
	
#else
	
	SetDirection( data.vPos.x, data.vPos.z );
	AnimationPlay( data.motion );
	
	if( m_save_event.type == SM_MOVE_ATTACK )
	{
		m_curt_event = data;		
	}
	else
	{
		SetQueue(data);
	} 	
#endif	

	m_save_event.nADisErrCnt = nADisErrCnt;

	return true; 
} 

bool NonPlayer::FindPath( short now_x, short now_y, int count )
{	
	if( theFindWay == NULL )
	{
		return false;
	}
	
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
	
	// 목적지가 변경되었다
	if( theFindWay->GoalReached == 0 )
	{
		m_Mov.dest.x = theFindWay->NearPos.X;
		m_Mov.dest.y = theFindWay->NearPos.Y;
	}
	// 목적지가 변경되었다
	if( theFindWay->ChangedEndPos == TRUE )
	{
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

bool NonPlayer::FindPath( short now_x, short now_y, EVENT_DATA_INFO& data, int count )
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Desc: 공격하려는 대상이 살아있나? 내 공격범위 안에 있나?
//-----------------------------------------------------------------------------
bool NonPlayer::FindNpcTile( int _nCurtX, int _nCurtY, int _nRange, EVENT_DATA_INFO& data )
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
		
		// 블럽타입, Position value저장
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
				///-- 서버쪽과 �?瑩獵?작업이 필요할듯....				
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
	
	int real_range = GetMax( abs((long)nPosX - (long)m_Mov.curt.x ), abs((long)nPosY - (long)m_Mov.curt.y ) );
	
	int nComRange = _nRange;
	nComRange -= ( data.nADisErrCnt * 2 );
	nComRange = max(1, nComRange);

	if( real_range <= nComRange )
	{		
		return true;
	}
	
	return false;
}

bool NonPlayer::GetNextTile()
{
	// 이동 할 수 있는 곳인지 타일 검사 
	// FindDestTile()
	// 길찾기 시작
	// FindPath()
	// 다음 타일 계산 
	/*		float step_x, step_y;
	GetMoveStep( m_Mov.path.x, m_Mov.path.y, m_Mov.curt.x, m_Mov.curt.y, step_x, step_y );
	m_Mov.next.x = m_Mov.curt.x+(int)((step_x*GetRunFrame())/CLIENT_TILESIZE);
	m_Mov.next.y = m_Mov.curt.y+(int)((step_y*GetRunFrame())/CLIENT_TILESIZE);
	*/	
	// 이동중 처리 루틴 
	// FindNextTile()
	
	if( m_Mov.curt.x == m_Mov.dest.x && m_Mov.curt.y == m_Mov.dest.y )
	{
		return false;
	}
	else if( m_Mov.next.x == m_Mov.dest.x && m_Mov.next.y == m_Mov.dest.y )
	{
		return false;
	}
	
	//-----------------------------------------------------------------------------
	// 다음 이동할 타일 검색하기
	//-----------------------------------------------------------------------------	
	int nCount	= GetMax( abs((long)m_Mov.dest.x- (long)m_Mov.curt.x), abs((long)m_Mov.dest.y- (long)m_Mov.curt.y) );
	
	if( FindPath( (short)m_Mov.curt.x, (short)m_Mov.curt.y, nCount ) == true )
	{			
		int nRunFrame	= GetRunFrame();		
		int nCurtFrame	= nRunFrame - m_move_frame;
		if( m_move_frame > nRunFrame )
			nCurtFrame = (nRunFrame*2) - m_move_frame;
		
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
			return false;
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
		
		long lBlock		= ( m_Mov.curt.y * g_Pc.GetMapSize() ) + m_Mov.curt.x;
		long lDestBlock = ( dwPosY*g_Pc.GetMapSize() ) + dwPosX;
		
		int nAngle = GetAngle( lBlock, lDestBlock );
		if( nAngle != -1 )
			m_byAngle = nAngle;
		
		if(m_bPC)
		{
			g_Pc.m_PcTable[m_nTableIndex].lBlock = lBlock;
			g_Pc.m_PcTable[m_nTableIndex].lDestBlock = lDestBlock;		
		}
		else if(!m_bPC && m_State.nPetState == nPetState_Pet )
		{
			g_Map.m_NpcTable[m_nTableIndex].lBlock = lBlock;
			g_Map.m_NpcTable[m_nTableIndex].lDestBlock = lDestBlock;
		}
#ifdef NPC_MOVE_TO_SKILL
		else if(!m_bPC && m_State.nActiveState == nActiveTypeMove )
		{
			g_Map.m_NpcTable[m_nTableIndex].lBlock = lBlock;
			g_Map.m_NpcTable[m_nTableIndex].lDestBlock = lDestBlock;
		}
#endif 

		if( ( m_State.IllusionIndex[0] == MAX_USER_INTABLE ) &&
			( m_State.nCharState == nCharState_Detect_Illusion )			
			)
		{
			CM__MOVE(m_Mov.curt.x, m_Mov.curt.y, dwPosX, dwPosY);
		}
		
		m_Mov.next.x = dwPosX;
		m_Mov.next.y = dwPosY;
	}
	
	return true;
}

bool NonPlayer::SM__MOVE__BROADCAST( EVENT_DATA_INFO& data )
{
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
#ifdef NPC_MOVE_TO_SKILL
	case SM_MOVE_SKILL:
#endif 
		{
			if( data.type == SM_MOVE_ATTACK )
			{
				m_save_event = data;
				
				theAttack_Event.Clear();
				
				return SM__MOVE( data, true );
			}
			else
			{
				return SM__MOVE( data, false );			
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
				if( m_curt_event.type == SM_MOVE_BROADCAST 
#ifdef NPC_MOVE_TO_SKILL
					||m_curt_event.type == SM_MOVE_SKILL
#endif 
					)
				{	
					// 이동값수정
					m_Mov.dest.x = data.x;
					m_Mov.dest.y = data.y;
					m_save_event.Clear();
				}
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

//----------------------------------------------------------------------------
// Desc: 서버에 메세지 보내기
//-----------------------------------------------------------------------------
bool NonPlayer::CM__MOVE( int _nCurtX, int _nCurtY, int _nNextX, int _nNextY )
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
	
	// 현재 바라보는각
	int nAngle = GetAngle( lBlock, lDestBlock );
	if( -1 != nAngle )
		m_byAngle = nAngle;
	
	// 블럭값 테이블에 저장.
	g_Map.m_NpcTable[m_nTableIndex].lBlock = lBlock;
	g_Map.m_NpcTable[m_nTableIndex].lDestBlock = lDestBlock;
	
	return true;
}


//-----------------------------------------------------------------------------
// Desc: 공격
//-----------------------------------------------------------------------------
bool NonPlayer::SM__ATTACK__NPC( EVENT_DATA_INFO& data )
{	
	//-----------------------------------------------------------------------------
	// 현재 행동 무시
	//-----------------------------------------------------------------------------
	SetBreakEvent();
	
	//-----------------------------------------------------------------------------
	// 생성 애니메이션 중이였다면..
	//-----------------------------------------------------------------------------
	//	if( m_curt_event.type == SM_NPC_RESPAWN )
	//	{
	//for( int i = 0; i < m_itm_count; ++i )
	//		for( int i = 0; i < g_Pc_Manager.GetItmCount( this ); ++i )
	//			m_pMaterial[i].Diffuse.a = 1.0f;				
	//	}
	
	//-----------------------------------------------------------------------------
	// 이벤트 저장
	//-----------------------------------------------------------------------------
	//	if( m_curt_event.type == SM_UNDERATTACK )
	//		m_save_event.type = SM_UNDERATTACK;
	
	m_IsCDataQActed = FALSE;
	
	m_curt_event = data;
	
	//-----------------------------------------------------------------------------
	// 해당 타일 방향으로 회전하기	
	//-----------------------------------------------------------------------------	
	//	SetDirection( data.vPos.x, data.vPos.z );	
	//............................................................................................................
	// 위치가 고정되있는 몬스터 체크 By wxywxy
	//............................................................................................................
	BOOL ISSetDir = TRUE; 
	
	if( !m_bPC )
	{
		if( g_Map.m_NpcTable[ m_nTableIndex ].code >= c_NotMoveNPCCode_Min && g_Map.m_NpcTable[ m_nTableIndex ].code <= c_NotMoveNPCCode_Max )
		{
			ISSetDir = FALSE;
		}
	}
	
	if( ISSetDir )
	{
		if( data.race < 0 )	
		{
			if( m_bPC == false && data.tbl_Idx == m_nTableIndex )
			{
			}
			else
			{
				SetDirection( g_Map.m_Npc[data.tbl_Idx].GetPosTM()._41, g_Map.m_Npc[data.tbl_Idx].GetPosTM()._43 );		
			}
		}
		else
		{
			if( data.tbl_Idx == MAX_USER_INTABLE )
			{
				SetDirection( g_Pc.GetPlayer()->GetPosTM()._41, g_Pc.GetPlayer()->GetPosTM()._43 );
			}
			else
			{
				if( m_bPC == true && data.tbl_Idx == m_nTableIndex )
				{
				}
				else
				{
					SetDirection( g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._41, g_Pc.m_Pc[data.tbl_Idx].GetPosTM()._43 );
				}
			}
		}
	}
	//............................................................................................................
	
	//if( m_State.bRide == true )	
	//	g_Map.m_Npc[m_State.horse_index].SetPosTM( m_matChr );	
	
	//-----------------------------------------------------------------------------
	// 현재 각도 갱신
	//-----------------------------------------------------------------------------
	//	int angle = GetAngle( m_Mov.curt.x, m_Mov.curt.y, data.x, data.y );
	//	if( angle != -1 && m_byAngle != angle )
	//	{				
	//		m_byAngle = angle;				
	//	if( m_State.bRide == true )
	//		g_Map.m_Npc[m_State.horse_index].m_byAngle = angle;		
	//	}
	
	//-----------------------------------------------------------------------------
	// 애니메이션 바꾸기
	// NPC는 인덱스값이 5이상이면 마법애니메이션이다		
	//-----------------------------------------------------------------------------
	int index = m_curt_event.motion;
	
	if( m_bPC == true )	
	{
		m_SwordTail.Clear();				// 검기 초기화 by wxywxy
	}
	else
	{
		// 스킬
		/*		if( index >= 5 )
		index = (index-5)+NPC_MAGIG_NUMBER;
		// 일반
		else
		index += NPC_ATTACK_NUMBER; */
	}
	
	AnimationPlay( index );	
	
	//..............................................................................................
	// NPC 공격시 사운드
	//..............................................................................................
	if( m_bPC == FALSE )
	{
		int type = rand() % 3;
		
		switch( type )
		{
			//......................................................................................
			// 공격 0
			//......................................................................................
		case 0:
			{	
				SNPCBaseInfo *pInfo = g_Pc_Manager.GetNPCBaseInfo( m_nTableIndex );
				
				//g_Sound.Play( pInfo->theAttackSnd1 , NULL );
			}
			break;
			
			//......................................................................................
			// 공격 1
			//......................................................................................
		case 1:
			{
				SNPCBaseInfo *pInfo = g_Pc_Manager.GetNPCBaseInfo( m_nTableIndex );
				
				//g_Sound.Play( pInfo->theAttackSnd2 , NULL );
			}
			break;
			
			
			//......................................................................................
			// 공격소리 없음
			//......................................................................................
		default:
			
			return true;
		}
	}
	
	return true;
}


bool NonPlayer::SM__UNDERATTACK( EVENT_DATA_INFO& data )
{
	//	if( m_curt_event.type == SM_MOVE_BROADCAST || m_curt_event.type == SM_ATTACK_NPC )
	//		return false;
	
	// 데미지에 따른 ReAction
	// 약, 강, 크리티컬 
	// MaxHealth 30% 이하는 약데미지 이상은 강데미지, 크리티컬은 따로 처리...
	
	/*	int motion = rand()%3;
	
	  if( m_bPC == true )
	  {	
	  if( motion == 0 )
	  motion = n_ReAction_Critical9;
	  else 
	  motion = (rand()%2) ? n_ReAction_LightDmg9 : n_ReAction_StrongDmg9;
	  }
	  else
	  {
	  if( motion == 0 )
	  motion = n_ReAction_Critical9;
	  else 
	  motion = (rand()%2) ? n_NPC_ReAction0 : n_NPC_ReAction1;
	  }
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

bool NonPlayer::SM__STATE__DIE( EVENT_DATA_INFO& data )
{	
	m_curt_event = data;
	
	if( m_bPC == FALSE )
	{
		SNPCBaseInfo *pInfo = g_Pc_Manager.GetNPCBaseInfo( m_nTableIndex );
		
		g_Sound.Play( pInfo->theAttackSnd3 , GetPosTM()._41 ,
											 GetPosTM()._42 ,
											 GetPosTM()._43 );
		
		// should we put an item for every NPC ... ㅡ ㅡ; Hmmm ... it's not pouring all at once ... once ...
		// drop the item on death
		int idx = 0;
		std::vector<s_AppearItem>::iterator ir;							
		ir = g_Map.theAppearItemList.begin();
		for( ; ir != g_Map.theAppearItemList.end() ; ++ir )
		{
			if( (*ir).NPCID == g_Map.m_NpcTable[m_nTableIndex].lUnique )
			{
				g_Map.SM__ITEM__APPEAR( (*ir).ItemID, (*ir).Cell, (*ir).ItemCode );
				
				g_Particle.SetEffSetCon( 0.0f , EF_ITEM_DROP , 100 , 1.0f , -1 , -1 , 
					D3DXVECTOR3( m_matChr._41 , m_matChr._42 + 1.0f , m_matChr._43 ) , D3DXToRadian( rand() % 360 ) );
				
				g_Map.theAppearItemList.erase( ir );
				ir--;
			}
		}
		
		CItemTransButton* pInVenButton = g_main_buttons.GetInVenButton();
		NPCTABLE *pNpcTable = g_Map.GetNpcTable(this);
		if(pNpcTable)
		{
			pInVenButton->SetActivateItemTransQueueInfo(pNpcTable->lUnique);
		}
	}
	
	// 애니메이션 플레이하기
	if( AnimationPlay( data.motion ) == -1 )
		return false;
	
	if( NonPlayerInfoWnd.thePickedNpcID == m_nTableIndex )
	{
		NonPlayerInfoWnd.thePickedNpcID = -1;
		nRui->SendEvent( WID_NonPlayerInfo_Wnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		nRui->SendEvent( WID_NonPlayerInfoAddWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
		
		if(g_NonPlayerInfoAdd2Wnd.IsVisible)
		{
			g_NonPlayerInfoAdd2Wnd.Close();
		}
	}					
	
	return true;
}

//-----------------------------------------------------------------------------
// Desc: 그림자 생성
//-----------------------------------------------------------------------------
BOOL	NonPlayer::ShadowMake(bool bFlag)
{		
	if( bFlag == false && m_State.nRideState == nRideState_Ride )
	{
		return FALSE;
	}	
	
	if( g_Shadow == 2 && !m_Shadow.m_IsInitTex )
	{
		m_Shadow.Clear();
		
		if( !m_Shadow.Create(m_nShadowSize) )
		{
			m_Shadow.Clear();
			return FALSE;
		}
		
		m_Shadow.m_fCumulativeTime = 0.0f;
		return FALSE;
	}	
	
	if( GetRange( XBLOCK_RANGE/2 ) == false )
		return FALSE;
	
	if( g_Shadow == 2 && m_Shadow.pRenderToSurface == NULL )
	{
		m_bShadowDisplay = false;
		return FALSE;
	}
	
	//-----------------------------------------------------------------------------
	// 죽어있거나, 플래그 false면 그림자X
	//-----------------------------------------------------------------------------
	if( m_curt_event.type != SM_STATE_DIE && m_bShadowDisplay == true )
	{					
		///-- 본에 연결된 매트릭스를 갱신하는 AniRender()함수가 Display 함수 이전에 존재하기 때문에
		///-- 본에 연결된 매트릭스가 엇갈리게 된다. 이를 여기서 다시 구한다.
		if(g_Shadow == 2)
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
			if( ISDisplayWeaponShadow && m_pUserInfo)
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
			if( SUCCEEDED(m_Shadow.pRenderToSurface->BeginScene(m_Shadow.pSurface,NULL)) )
			{								
				g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 0.0f, 0 );
				
				g_RenderManager.SetTransform( D3DTS_VIEW,	&m_Shadow.matWorldToLight );
				g_RenderManager.SetTransform( D3DTS_PROJECTION,	&m_Shadow.matShadowProj );
				
				if(m_State.bRide)
				{
					g_Map.m_Npc[m_State.TransIndex].RenderShadow(m_Shadow);				
				}
				
				//D3DXMatrixMultiply( &matShadow, &matCharLocalToLight, &m_Shadow.matShadowProj );				
				g_RenderManager.SetTransform( D3DTS_WORLD,	&matTemp );						
				
				//-----------------------------------------------------------------------------
				// 몸통
				//-----------------------------------------------------------------------------		
				int mod_index = 0;			
				int itm_count = 0;
				
				itm_count = g_Pc_Manager.GetItmCount( this );			
				
				for( int i = 0; i < itm_count; ++i )
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
				
				if( ISDisplayWeaponShadow && m_pUserInfo )
				{
					D3DXMATRIX	matItemLocalToLight;
					
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
							//D3DXMatrixMultiply( &matItemLocalToLight, &matWorld, &m_Shadow.matWorldToLight );							
							// 라이트 x 프로젝션 = 그림자행렬
							//D3DXMatrixMultiply( &matShadow, &matItemLocalToLight, &m_Shadow.matShadowProj );							
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
	}
	
	return TRUE;
}

void NonPlayer::ApplyWeaponShaowProj()
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

void	NonPlayer::ShadowDisplay(bool bFlag)
{	
	if( bFlag == false && m_State.nRideState == nRideState_Ride )
	{
		return;
	}
	
	if( GetRange( XBLOCK_RANGE/2 ) == false )
		return;		
	//-----------------------------------------------------------------------------
	// 그림자 렌더링
	//-----------------------------------------------------------------------------
	if( m_bShadowDisplay == true && m_curt_event.type != SM_STATE_DIE )
	{			
		D3DXMATRIX	matOffset;					
		D3DXMatrixIdentity( &matOffset );
		matOffset._11 =  0.5f;
		matOffset._22 = -0.5f;	
		matOffset._31 =  0.5f;
		matOffset._32 =  0.5f;		
		
		// 텍스쳐행렬만들기
		D3DXMatrixMultiply( &m_Shadow.matShadowUV, &g_Camera.m_matInvView, &m_Shadow.matWorldToLight );
		
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
			
			if( !m_bPC )
			{				
				SNPCBaseInfo* npc_Info = NULL;
				npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[ m_nTableIndex ].code ); 
				
				if( npc_Info )
				{
					block_size = npc_Info->theSize;
				}
				
				if( g_Map.IsGiantUnit( this ) )
				{
					D3DXVECTOR3 vMax = g_Map.m_Npc[m_nTableIndex].GetBoundMaxVec();
					D3DXVECTOR3 vMin = g_Map.m_Npc[m_nTableIndex].GetBoundMinVec();
					
					float fWidht = 0.0f;
					float fDel =  vMax.x - vMin.x;
					if( fWidht < fDel )
					{
						fWidht = fDel;
					}
					fDel =  vMax.y - vMin.y;
					if( fWidht < fDel )
					{
						fWidht = fDel;
					}
					fDel =  vMax.z - vMin.z;
					if( fWidht < fDel )
					{
						fWidht = fDel;
					}

					fWidht /= 28.0f;
					fWidht += 0.5f;
					int nSize = (int)fWidht;					
					
					block_size = nSize;
				}
			}
			else
			{
				if(m_State.bRide)
				{
					SNPCBaseInfo* npc_Info = NULL;
					npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_State.TransIndex].code ); 
					
					if( npc_Info )
					{
						block_size = npc_Info->theSize;
					}
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
}

//-----------------------------------------------------------------------------
// Desc: 선택동그라미그리기
//-----------------------------------------------------------------------------
void NonPlayer::SelectDisplay()
{
	

}


//-----------------------------------------------------------------------------
// Desc: 애니메이션 프로세스
//-----------------------------------------------------------------------------
int NonPlayer::AniProcess(bool bFlag)
{
	if( SAFE_TIME_COMPARE( m_PreTime , > , g_nowTime ) )
	{
		m_PreTime = g_nowTime;
		///-- 여기서 m_PreTime이 g_nowTime보다 크면
		///-- g_nowTime - m_PreTime이 -값 ulong 형이므로 엄청 큰값으로 전환된다.
		///-- 따라서 m_middle_time값이 꼬이게 되고 
		///-- 미들타임을 사용하는 스키닝 정점 계산부분에서
		///-- 계산이 잘못되 화면이 갈라지는 현상이 일어난다.
	}	
	
	if( bFlag == false && m_State.nRideState == nRideState_Ride )
	{
		return -1;
	}
	
	if(m_State.nPetState == nPetState_Pet && m_pCurrentState)
	{
		m_pCurrentState->Execute((Character *)this);		
	}
	
	float ani_time = 0.0f;
	ani_time = GetAniTime();
	
	// 이동 프레임 업데이트 
	
	if( m_State.nRideState != nRideState_Ride)
	{
		///-- ani_time이 float 형이므로 비교계산이 불가능하다.
		if( SAFE_TIME_SUB( g_nowTime , m_PreMTime ) >= ani_time  )		
		{
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

			///--JAPAN_BUG_MODIFY
			float fRemain = (float)SAFE_TIME_SUB( g_nowTime , m_PreMTime ) - (float)(add_frame * ani_time);
			float fRatio =  (float)fRemain / ani_time;
			fRatio = max( 0.0f, fRatio );
			
#ifdef NPC_MOVE_TO_SKILL
			if( m_curt_event.type == SM_MOVE_BROADCAST  || m_curt_event.type == SM_MOVE_SKILL )	
#else 
			if( m_curt_event.type == SM_MOVE_BROADCAST )	
#endif 
			{
				MoveRatio( fRatio );				
				AniProcessMoveEvent();
			}

			m_PreMTime = g_nowTime;
			
			///--add_frame = (int)(SAFE_TIME_SUB( g_nowTime , m_PreTime ) / c_MoveTime );			
			///--JAPAN_BUG_MODIFY
			Rotate( fRatio, add_frame );		
		}
		else
		{
			m_move_middle_time = SAFE_TIME_SUB( g_nowTime , m_PreMTime ) / ani_time;			
			// 보간하여 (true) 캐릭터이동하기
			AniProcessMove( true );
		}
		
		m_matChr._42 = g_Map.PHF_GetHeight( m_matChr._41 , m_matChr._43 );		

		BOOL Is4FootUnit = FALSE;

		if( !m_bPC  )	///-- NPC일 경우
		{
			DWORD dNPCCode = g_Map.m_NpcTable[m_nTableIndex].code;	///-- NPC가 4족보행인지 여부판단...
			SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

			if(npc_Info)
			{
				if( npc_Info->theGender == n_4FootUnit )
				{
					Is4FootUnit = TRUE;
				}
			}
		}

		if( Is4FootUnit )		///-- 4족 보행 유닛일경우 x축회전
		{
			D3DXMATRIX matRotX, matRotY;			
			D3DXMATRIX matPos = m_matChr;			
			
			D3DXVECTOR3 vNowPos = D3DXVECTOR3(m_matChr._41, m_matChr._42, m_matChr._43);
			vNowPos.y = g_Map.PHF_GetHeight( vNowPos.x , vNowPos.z );
			D3DXVECTOR3 vForwardPos = vNowPos + ( 8.0f * m_vDir);
			vForwardPos.y = g_Map.PHF_GetHeight( vForwardPos.x , vForwardPos.z );

			float fTempYRadian = 0.0f;
			float fXRadian = 0.0f;

			GetXAndYRadian(fXRadian, fTempYRadian, vNowPos, vForwardPos);
			m_Mov.now_Xrad = fXRadian;			
			
			D3DXMatrixRotationX( &matRotX , m_Mov.now_Xrad );
			D3DXMatrixRotationY( &matRotY , m_Mov.now_Yrad );		
			D3DXMatrixMultiply( &m_matChr , &matRotX , &matRotY );

			m_matChr._41 = matPos._41;
			m_matChr._42 = matPos._42;
			m_matChr._43 = matPos._43;
		}
	}
	
	if( m_OneTimeJump )
	{
		m_OneTimeJump = false;
		m_PreTime = g_nowTime;
		return -1;
	}
	
	//	float ani_time = 0.0f;
	//	ani_time = GetAniTime();
	
	//-----------------------------------------------------------------------------
	// 활동중
	//-----------------------------------------------------------------------------
	///-- ani_time이 float 형이므로 비교계산이 불가능하다.
	if( SAFE_TIME_SUB( g_nowTime , m_PreTime ) >= ani_time  )
	{	
		// 스키프레임 계산
		int skip_count = (int)(SAFE_TIME_SUB( g_nowTime, m_PreTime ) / ani_time );
		//for( int i = 0; i < skip_count; ++i )
		{
			//-----------------------------------------------------------------------------
			// NPC 생성
			//-----------------------------------------------------------------------------			
			//			if( m_curt_event.type == SM_NPC_RESPAWN )			
			//				RespawnProcess();			
			//-----------------------------------------------------------------------------
			// NPC 죽음
			//-----------------------------------------------------------------------------
			//			else if( m_curt_event.type == SM_STATE_DIE )
			if( m_curt_event.type == SM_STATE_DIE && m_curt_event.status == SM_STATE_DIE &&
				m_ani_frame == (int)m_end_frame )
			{
				int Res = DieProcess();
			}
			//-----------------------------------------------------------------------------
			// 애니메이션 & 이벤트처리하기
			//-----------------------------------------------------------------------------			
			//			if( !(m_curt_event.type == SM_STATE_DIE && m_ani_frame == (int)m_end_frame) )
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
						else if( ( m_ani_frame == c_MotionBlendHalfFrame + m_start_blend_frame ) 
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
				
				// 예외처리하는지 검사하기
				if( UpdateFrame() == false )
				{					
					// 애니메이션처리
					if( m_ani_frame + c_MotionBlendHalfFrame == (int)m_end_frame || 
						m_ani_frame + c_MotionBlendFrame > (int)m_end_frame )					
					{									
						AniProcessEvent();
						
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
		m_step_time   = 0.0f;
		m_middle_time = 0.0f;

		if( !( m_curt_event.type == SM_STATE_DIE && m_ani_frame == (int)m_end_frame ) )
		{			
			DWORD AniTime = c_MoveTime;

			if( m_curt_event.type == SM_MOVE_SKILL )

			if( m_curt_event.type == SM_MOVE_BROADCAST 
#ifdef NPC_MOVE_TO_SKILL
				|| m_curt_event.type == SM_MOVE_SKILL 
#endif 
				)
			{
				AniTime = ( c_MoveTime * m_ani_percent );
			}
			else
			{
				AniTime = ani_time;
			}
			
			if( AniTime == 0 )
			{
				m_PreTime = g_nowTime;
			}
			else
			{
				m_PreTime = g_nowTime - ( (int)SAFE_TIME_SUB( g_nowTime , m_PreTime ) % AniTime );
			}
		}
	}	
	// 보간
	else
	{
		// 보간타임저장
		m_middle_time = SAFE_TIME_SUB( g_nowTime , m_PreTime ) / ani_time;
	}
	
	g_chr_anicount++;
	
	if(m_State.bRide)
	{
		g_Map.m_Npc[m_State.TransIndex].SetPosTM(GetPosTM());		
		g_Map.m_Npc[m_State.TransIndex].AniProcess(true);		
	}
	
	return -1;
}

bool NonPlayer::FindDestTile( EVENT_DATA_INFO& data, bool bNpc )
{
	// 맨처음 픽킹할때 갈 수 있는 곳으로 1차 클라이언트 허용, 2차 서버에서 허용해서 가야 되는 곳이면 일단 무조건 간다 
	return true;
}

//-----------------------------------------------------------------------------
// Desc: 캐릭터 이동하기
//-----------------------------------------------------------------------------
void NonPlayer::AniProcessMove( bool middle )
{		
	//-----------------------------------------------------------------------------
	// 이동처리
	//-----------------------------------------------------------------------------

#ifdef NPC_MOVE_TO_SKILL
	if( m_curt_event.type == SM_MOVE_BROADCAST ||  m_curt_event.type == SM_MOVE_SKILL )
#else 
	if( m_curt_event.type == SM_MOVE_BROADCAST )
#endif 
	{				
		// 말타고 있다면 먼지 날리기
		/*if( m_State.bRide == true && m_ani_frame%2 == 0 )
		{
		D3DXVECTOR3 tempv;
		tempv = D3DXVECTOR3( g_Pc.m_Pc[m_nTableIndex].GetPosTM()._41, g_Map.m_fBaseHeightWater, g_Pc.m_Pc[m_nTableIndex].GetPosTM()._43 );
		// 땅이 물이 아니면 물튀기자. 
		if( g_Map.m_pAttrMap[m_Mov.curt.y][m_Mov.curt.x].byType == MATYPE_WATER )
		g_Particle.DisplayUpdate( 55, tempv );
		else
		{
		if( g_Pc.FindUser( g_Pc.m_PcTable[m_nTableIndex].lUnique ) != MAX_USER_INTABLE ) // 테이블에 없지 않을때.
		g_Particle.DisplayUpdate( 6, g_Pc.m_PcTable[m_nTableIndex].lUnique, -1, -1 );
		}
	}*/
		
		//-----------------------------------------------------------------------------
		// 캐릭터이동
		//-----------------------------------------------------------------------------
		if( middle == false )
		{		
			// 다음 타일로 넘어가는 순간에 길찾기 (1타일에 한번씩 길찾기를 한다.) 			
#ifdef NPC_MOVE_TO_SKILL
			if( ( m_bPC || m_State.nPetState == nPetState_Pet || m_State.nActiveState == nActiveTypeMove) 
				&& m_move_frame == (GetRunFrame()/2+1) )
#else 
			if( ( m_bPC || m_State.nPetState == nPetState_Pet ) && m_move_frame == (GetRunFrame()/2+1) )
#endif 
			{														
				GetNextTile();
#ifdef NPC_MOVE_TO_SKILL
				if( (m_State.nPetState == nPetState_Pet || m_State.nActiveState == nActiveTypeMove ) &&  
					!GetNextPath( m_Mov.curt.x, m_Mov.curt.y, m_save_event ) )
#else 
				if( m_State.nPetState == nPetState_Pet &&  
					!GetNextPath( m_Mov.curt.x, m_Mov.curt.y, m_save_event ) )
#endif 
				{
					m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );					
					CM__MOVE(m_Mov.curt.x, m_Mov.curt.y, m_Mov.curt.x, m_Mov.curt.y);
					m_curt_event.Clear();				
					
					if( m_save_event.type == SM_MOVE_ATTACK )
					{
						CM__ATTACK__NPC( m_save_event );						
					}		
					
					//g_Map.SetMapData( NPC_BLOCK, m_nTableIndex, g_Map.m_NpcTable[m_nTableIndex].lDestBlock, false );
				}
			}
			
			Move( m_move_middle_time );
		}
		else
		{			
			MiddleMove( m_move_middle_time );				
		}
	}
}
//-----------------------------------------------------------------------------
// Desc: 저장된 이벤트 처리
//-----------------------------------------------------------------------------
int NonPlayer::AniProcessMoveEvent()
{
	//-----------------------------------------------------------------------------
	// 현재 이벤트 처리
	//-----------------------------------------------------------------------------
	switch( m_curt_event.type )
	{		
		// 이동		

#ifdef NPC_MOVE_TO_SKILL
	case SM_MOVE_SKILL:
#endif 
	case SM_MOVE_BROADCAST:
		{				
			// 현재타일저장
			m_Mov.curt.x = (int)( (m_matChr._41) / CLIENT_TILESIZE );
			m_Mov.curt.y = (int)( (m_matChr._43) / CLIENT_TILESIZE );
	
			// 목적지에 도착했다면.
			if( m_Mov.dest.x == m_Mov.curt.x && m_Mov.dest.y == m_Mov.curt.y && m_move_frame == GetRunFrame() )
			{
				if( m_bPC )
				{
					// 높이값보정
					SetHeightStep( true );
					
					// 현재 Position value 저장
					m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );	
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

					///--JAPAN_BUG_MODIFY						
					///-- Is illusion?
					///-- 일단 이부분은 보류하자. 
					if( m_State.nCharState == nCharState_Illusion || 
						m_State.nCharState == nCharState_Detect_Illusion )
					{
						///-- 마스터의 인덱스가 플레이어 자신일때						
						if( m_State.IllusionIndex[0] == MAX_USER_INTABLE )
						{
							DWORD dCell = g_Pc.m_PcTable[m_nTableIndex].lDestBlock;
							DWORD dPCID = g_Pc.m_PcTable[m_nTableIndex].lUnique;
							float fAngle = m_Mov.now_Yrad;
							if( dPCID )
							{
								g_RockClient.Send_RpCsMove( dPCID, dCell, fAngle );
							}
						}						
					}
					
					if( m_save_event.type == SM_MOVE_TURN )		
					{
						
						SetDirection( 0.0f );
						
						m_save_event.Clear();						
					}
					
					g_Map.SetMapData( PC_BLOCK, m_nTableIndex, g_Pc.m_PcTable[m_nTableIndex].lDestBlock, true );
				}
#ifdef NPC_MOVE_TO_SKILL
				else if( m_State.nActiveState == nActiveTypeMove)
				{
					
					//g_Particle.m_EffectManager.CreateEffect( ( EFFECT_ID ) 501 , this , NULL , NULL );
					g_Particle.m_EffectManager.Delete( ( EFFECT_ID ) 510 , this , NULL );
					m_State.nActiveState = nActiveTypeSKill;

					SetHeightStep( true );
					
					// 현재 Position value 저장
					m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );	
					m_curt_event.Clear();				
					
					if( m_save_event.type == SM_MOVE_TURN )		
					{
						SetDirection( 0.0f );						
						m_save_event.Clear();						
					}
					
					// 애니메이션 플레이 설정. 걷기,뛰기가 변경되었을 수 있기 때문에					
					if( m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						AnimationPlay( n_Stand_Peace9 );
					}
					else
					{
						AnimationPlay( n_NPC_Stand0 );
					}
					
					//도착했으니 스킬동작 시킨다 . 
					
					g_Map.SetMapData( NPC_BLOCK, m_nTableIndex, g_Map.m_NpcTable[m_nTableIndex].lDestBlock, false );
					
					m_save_event.type = SM_USE_SKILL;
					m_save_event.status = SM_USE_SKILL;
											
					//m_next_event = m_save_event;
												
					m_ani_percent = 1.0f;
					m_move_percent = 1.0f;
					
					PushQueue(m_save_event , true );
					m_save_event.Clear();
				}
#endif 
				else if(m_State.nPetState == nPetState_Pet)
				{
					// 높이값보정
					SetHeightStep( true );
					
					// 현재 Position value 저장
					m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );	
					m_curt_event.Clear();				
					
					if( m_save_event.type == SM_MOVE_TURN )		
					{
						SetDirection( 0.0f );						
						m_save_event.Clear();						
					}
					
					// 애니메이션 플레이 설정. 걷기,뛰기가 변경되었을 수 있기 때문에					
					if( m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						AnimationPlay( n_Stand_Peace9 );
					}
					else
					{
						AnimationPlay( n_NPC_Stand0 );
					}

					///-- 펫은 일단 무브패킷이 없으므로 보류하자.
					/*if( g_Pc.GetPlayer()->m_State.PetIndex == m_nTableIndex )
					{
						DWORD aCell =  g_Map.m_NpcTable[m_nTableIndex].lDestBlock;
						DWORD aPetID = 	g_Map.m_NpcTable[m_nTableIndex].lUnique;					
						g_RockClient.Send_CsPetStop( aPetID , aCell );
					}*/					
					
					if( m_save_event.type != SM_MOVE_BROADCAST )					
					{
						if( m_save_event.type == SM_MOVE_ATTACK )
						{
							CM__ATTACK__NPC( m_save_event );						
						}
					}
					
					g_Map.SetMapData( NPC_BLOCK, m_nTableIndex, g_Map.m_NpcTable[m_nTableIndex].lDestBlock, false );
				}
				else
				{
					///-- 현재 경로가 마지막 경로이라면 멈춘다. 일반 NPC 
					if( theCurtPath == theDestPath )
					{
						// 높이값보정
						SetHeightStep( true );
						
						// 카메라보정
						//						if( g_Pc.m_MainPC.lUnique == m_State.Unite_unique )
						//							g_Camera.Update();
						
						// 애니메이션 플레이 설정. 걷기,뛰기가 변경되었을 수 있기 때문에					
						if( m_State.nCharacterType == nCharacterTypePlayerNPC )
						{
							AnimationPlay( n_Stand_Peace9 );
						}
						else
						{
							AnimationPlay( n_NPC_Stand0 );
						}						
						
						// 현재 Position value 저장
						m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );	
						m_curt_event.Clear();
						
						g_Map.SetMapData( NPC_BLOCK, m_nTableIndex, g_Map.m_NpcTable[m_nTableIndex].lDestBlock, false );
					}
					else
					{
						++theCurtPath;
						
						EVENT_DATA_INFO	data;	
						data.x = thePath[theCurtPath] % g_Pc.m_cSize;
						data.y = thePath[theCurtPath] / g_Pc.m_cSize;
						data.type = SM_MOVE_BROADCAST;	
						
						///-- 다시 이동시작...
						// 임시 이동 테스트 ( 다른 유저와 NPC )
						if( m_bIsLoaded == true )
							SetQueue( data );
					}				
					
					g_Map.m_NpcTable[m_nTableIndex].lDestBlock = thePath[theCurtPath];
				}
				
			}
			// 도착하지 않았다면.. 계속 이동
			else
			{
				if( m_bPC )
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
					
					// 다음타일 이동값										
					SetRotate( m_Mov.path );
					SetMoveStep( m_Mov.path );
					SetHeightStep();					
				}
				else if(m_State.nPetState == nPetState_Pet)
				{
					if( m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						AnimationPlay( n_Run9 );
					}
					else
					{
						AnimationPlay( n_NPC_Walk0 );
					}					

					SetRotate( m_Mov.path );
					SetMoveStep( m_Mov.path );
					SetHeightStep();			
				}
#ifdef NPC_MOVE_TO_SKILL
				else if(m_State.nActiveState== nActiveTypeMove )
				{
					if( m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						AnimationPlay( n_Run9 );
					}
					else
					{
						AnimationPlay( n_NPC_Walk0 );
					}					
					
					SetRotate( m_Mov.path );
					SetMoveStep( m_Mov.path );
					SetHeightStep();			
				}
#endif 
				else
				{
					// 애니메이션 플레이 설정. 걷기,뛰기가 변경되었을 수 있기 때문에					
					if( m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						AnimationPlay( n_Walk9 );
					}
					else
					{
						AnimationPlay( n_NPC_Walk0 );
					}				
					
					// 이동, 회전, 높이값 처리
					SetRotate( m_Mov.dest );
					SetMoveStep( m_Mov.dest );
					SetHeightStep();
					
					g_Map.m_NpcTable[m_nTableIndex].lDestBlock = thePath[theCurtPath];
				}
			}
		}
		break;
	default:
		break;
	}	
	return 0;

}



//-----------------------------------------------------------------------------
// Desc: 저장된 이벤트 처리
//-----------------------------------------------------------------------------
int NonPlayer::AniProcessEvent()
{		
	
	EVENT_DATA_INFO queue_event;
	
	//-----------------------------------------------------------------------------
	// 현재 이벤트 처리
	//-----------------------------------------------------------------------------
	switch( m_curt_event.type )
	{			
	case SM_BOW_ATTACK:
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
		break;
		
		// 공격		
	case SM_ATTACK_NPC:
		{				
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )									
			{				
				BOOL IsArcherReady = FALSE;

				if( !m_bPC && m_ani_index == n_Attack10_BowReady0 )
				{
					IsArcherReady = TRUE;
				}

				if( m_bPC && m_ani_index == n_Attack10_BowReady0 )
				{
					IsArcherReady = TRUE;
				}
			
				if( IsArcherReady )
				{
					AnimationPlay( n_Attack10_BowShoot0 );
				}
				else
				{
					if( m_next_event.type == SM_ATTACK_NPC ||
						m_next_event.type == SM_USE_SKILL ||
						m_next_event.type == SM_MOVE_BROADCAST )
					{
						//					RLGS("SM_ATTACK_NPC : SetQueue(m_next_event);");
						SetQueue(m_next_event);
						m_next_event.Clear();
					}
					else
					{
						SNPCBaseInfo* npc_Info = NULL;
						
						if(!m_bPC)
						{
							npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].code );
						}

						if( npc_Info && m_State.nPetState == nPetState_Pet && 
							PET_AI_REDUCEDAMAGE != npc_Info->theAttackType &&
							m_State.PetIndex == MAX_USER_INTABLE )
						{
							queue_event = m_curt_event;
							queue_event.type = SM_ATTACK_DELAY;
							SetQueue( queue_event );
							m_Attack_DelayTime = g_nowTime;
				
							if( m_State.nCharacterType == nCharacterTypePlayerNPC )
							{
								AnimationPlay( n_Stand_Battle9 );
							}
							else
							{
								AnimationPlay( n_NPC_Stand0 );
							}
						}	
						else
						{
							m_curt_event.Clear();
							m_save_event.Clear();
						}					
						//RLGS("SM_ATTACK_NPC : m_curt_event.Clear();");
						
						if( m_bPC || (!m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ))
						{
							if(!m_State.bRide)
							{
								AnimationPlay( n_Stand_Battle9 );
							}
						}
					}					
				}
				m_SimTime = g_nowTime;
			}					
		}
		break;			
		// 밀려나기
	case SM_SLIDE_NPC:
		{
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				m_curt_event.Clear();
				m_save_event.Clear();
				m_SimTime = g_nowTime;
			}
		}
		break;
		// 맞았음
	case SM_UNDERATTACK:
		{
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				m_curt_event.Clear();
				m_save_event.Clear();				

				m_SimTime = g_nowTime;
			}			
		}
		break;
	case SM_STATE_DIE:
		{ 
			if( m_ani_frame > (int)m_end_frame )
			{			
				m_ani_frame = (int) m_end_frame;			
				
				m_curt_event.time = SAFE_TIME_ADD( g_nowTime , NPC_DIE_TIME );
				m_curt_event.status = SM_STATE_DIE;
				m_save_event.Clear();
				m_next_event.Clear();
				
				if( m_bPC == TRUE )
				{
					D3DXVECTOR3 vPos( GetPosTM()._41 , GetPosTM()._42 , GetPosTM()._43 );
					
					// 유저 죽을때 이펙트
					//					g_Particle.SetEffSetCon( 0.0f , EF_DIE_NPC , 100 , 1.4f , -1 , -1 , vPos );
				}
				else
				{
					g_Map.SetMapData( CLEAR_BLOCK, m_nTableIndex, g_Map.m_NpcTable[m_nTableIndex].lDestBlock );											
				}				
				
				//				if( m_bPC ) 
				//				{
				//					g_Pc.DisappearPC( g_Pc.m_PcTable[m_nTableIndex].lUnique );					
				//				}
				//				else
				//				{
				//					g_Map.SM__NPC__DISAPPEAR( 0, g_Map.m_NpcTable[m_nTableIndex].lUnique);
				//				}				
			}
			m_SimTime = g_nowTime;
		}	
		break;
		
		// 로그인 관련 제자리에서 에니메이션 테이블 처리 By wxywxy
	case SM_ANI_QUEUE_PROCESS:
		{	
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				if( NonPlayer::m_pCharInfo	!= NULL )
				{
					if( NonPlayer::m_StaticAniCount >= NonPlayer::m_pCharInfo->AniCount )
					{
						m_curt_event.Clear();
						
						AnimationPlay( n_Stand_Select9 );
						
						m_pUserInfo->bAttack = FALSE;
						
						NonPlayer::m_StaticAniCount = 0;
						NonPlayer::m_pCharInfo = NULL;
						
						if( g_Pc.m_PcTable[ m_nTableIndex ].char_info.theCitizen == MILLENA )
						{
#ifdef DECO_RENEWAL_MJH
#else
							m_Mov.now_Yrad = D3DXToRadian( 90 ); 
#endif // DECO_RENEWAL_MJH
							
						}
						else if( g_Pc.m_PcTable[ m_nTableIndex ].char_info.theCitizen == RAIN )
						{
							m_Mov.now_Yrad = -D3DXToRadian( 90 );
						}
					}
					else
					{
						if( g_Pc.m_PcTable[ m_nTableIndex ].char_info.theCitizen == MILLENA )
						{
							if( m_pCharInfo->AniCount >= 3 )
							{
								
								switch( NonPlayer::m_StaticAniCount )
								{
								case 0:
#ifdef DECO_RENEWAL_MJH
#else 
									m_Mov.now_Yrad = D3DXToRadian( 135 );
#endif // DECO_RENEWAL_MJH			
									break;
								case 1:
#ifdef DECO_RENEWAL_MJH
#else 
									m_Mov.now_Yrad = D3DXToRadian( 45 );
#endif // DECO_RENEWAL_MJH		
									break;
								case 2:
#ifdef DECO_RENEWAL_MJH
#else 
									m_Mov.now_Yrad = D3DXToRadian( 90 );
#endif // DECO_RENEWAL_MJH			
									break;
								}
							}
							else
							{
#ifdef DECO_RENEWAL_MJH
#else 
								m_Mov.now_Yrad = D3DXToRadian( 90 );
#endif // DECO_RENEWAL_MJH		
							}
						}
						else if( g_Pc.m_PcTable[ m_nTableIndex ].char_info.theCitizen == RAIN )
						{
							switch( CharSelectWnd.m_ViewClass )
							{
							case Psyche:
								{
									switch( NonPlayer::m_StaticAniCount )
									{
									case 0:
										m_Mov.now_Yrad = -D3DXToRadian( 135 );
										break;
									case 1:
										m_Mov.now_Yrad = -D3DXToRadian( 45 );
										break;
									case 2:
										m_Mov.now_Yrad = -D3DXToRadian( 90 );
										break;
									}
									break;
									
									default:
										m_Mov.now_Yrad = -D3DXToRadian( 90 );
										break;
								}
							}
						}
						
						m_curt_event.motion = NonPlayer::m_pCharInfo->AniList[ NonPlayer::m_StaticAniCount ];
						
						SetQueue(m_curt_event);
						
						NonPlayer::m_StaticAniCount++;
					}
				}
				else
				{
					m_curt_event.Clear();
					
					AnimationPlay( n_Stand_Select9 );
				}
				
				m_SimTime = g_nowTime;
			}
		}
		break;
	// 유져 & 몬스터 다 취급한다 스킬을 쓸때  AniProcessEvent 계속돈다 . 	
	case SM_USE_SKILL:
		{
			if( m_curt_event.status == SM_CHANT_READY )
			{
				if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
				{
					if(m_bPC)
					{
						m_curt_event.status = SM_CHANT_SPELLING;					
						m_curt_event.motion = SetChantMotion( SM_CHANT_SPELLING, 0 ); // 임시로 받기전에..					
						SetQueue(m_curt_event);
						m_SimTime = g_nowTime;
					}
					else if(!m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						m_curt_event.status = SM_USE_SKILL;					
						
						///-- 아쳐나 스나이퍼일 경우
						if( g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theMainClass == Archer ||
							g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theMainClass == Sniper ) 
						{
							SSkillBaseInfo* skill_Info = NULL;
							skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( m_curt_event.count );
							if(skill_Info)
							{
								m_curt_event.motion = skill_Info->theActionMotion;
							}
						}
						else
						{
							m_curt_event.motion = SetChantMotion( SM_USE_SKILL, 0 ); // 임시로 받기전에..					
						}

						m_SpellEffect.Clear();
						SetQueue(m_curt_event);
						m_SimTime = g_nowTime;
					}
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
					if( g_Pc.m_PcTable[m_nTableIndex].lDestBlock != m_next_event.cell )
					{
						g_Map.SetMapData( CLEAR_BLOCK, m_nTableIndex, g_Pc.m_PcTable[m_nTableIndex].lDestBlock, true );
						
						EVENT_DATA_INFO	data;
						data.x = m_next_event.cell % g_Pc.m_cSize;
						data.y = m_next_event.cell / g_Pc.m_cSize;
						data.type = SM_MOVE_BROADCAST;
						SetQueue( data );
						g_Pc.m_PcTable[m_nTableIndex].lDestBlock = m_next_event.cell;
					}
					else
					{
						m_curt_event.Clear();
					}
					
					m_SimTime = g_nowTime;
					
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
				//현재 하고있는동작이 다끝났냐 ??  
				if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
				{
					if( !m_IsApplyConAniSkill && !m_IsSkillNoAni )
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
							SNPCBaseInfo* npc_Info = NULL;
							
							if( !m_bPC )
							{
								npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].code );
							}

							if( npc_Info && m_State.nPetState == nPetState_Pet && PET_AI_REDUCEDAMAGE != npc_Info->theAttackType &&
								m_State.PetIndex == MAX_USER_INTABLE )
							{
								SSkillBaseInfo* skill_Info;
								skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( m_curt_event.count );
								if( skill_Info == NULL )
									return false;
								if( ( skill_Info->theSkillType2 == n_DeathBlow ||
									  skill_Info->theSkillType2 == n_AttackMagic || 
									  skill_Info->theSkillType2 == n_NoFrameDeathBlow ) &&
									( skill_Info->theActTarget == n_AT_TARGET ) )
								{
									
									queue_event = m_curt_event;
									queue_event.type = SM_ATTACK_DELAY;
									
									SetQueue( queue_event );							
									m_Attack_DelayTime = g_nowTime;	
									
									m_save_event.Clear();
									m_next_event.Clear();
									
									if( m_State.nCharacterType == nCharacterTypePlayerNPC )
									{
										AnimationPlay( n_Stand_Battle9 );
									}
									else
									{
										AnimationPlay( n_NPC_Stand0 );
									}						
								}
								else
								{
									m_curt_event.Clear();
									m_save_event.Clear();
									m_next_event.Clear();
								}
							}
							else
							{
								m_curt_event.Clear();
								m_save_event.Clear();
								m_next_event.Clear();			
								
								//돌진 후 스킬 종료 시점에서 .. 
								if( m_State.nActiveState ==  nActiveTypeSKill)
									m_State.nActiveState = nActiveTypeNormal;
								
								if( (m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) ) 
									&& !m_State.bRide)
								{
									AnimationPlay( n_Stand_Battle9 );	
								}
							}
							
							m_SimTime = g_nowTime;
						}
					}
				}
			}
		}
		break;
		// 아이템 떨어질ㄸㅐ 애니 메이션 
	case SM_ITEM_DROPING:
		{
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				m_curt_event.Clear();
				m_SimTime = g_nowTime;
			}
		}
		break;
	case SM_ATTACK_DELAY:
		{
			if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_Weapon_DelayTime , m_Attack_DelayTime ) ) )		// 기본 평균 1.5초의 대기 시간
			{
				if( m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					m_curt_event.motion = n_Stand_Battle9;
				}
				else
				{
					m_curt_event.motion = n_NPC_Attack0;
				}				
				
				CM__ATTACK__NPC( m_curt_event );				
				m_SimTime = g_nowTime;				
			}
			
			m_SimTime = g_nowTime;
		}
		// 심심할때 하는 행동
		break;
	case SM_SIMSIM:
		{
			if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
			{
				m_curt_event.Clear();
				m_SimTime = g_nowTime;
			}
		} 
		break;		
		
		// 동작
	case SM_ACT:
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
						m_pre_ani_number = m_ani_number;
						m_blend_frame = 0;
						m_middle_time = 0.0f;
					}
				}
			}
		}
		break;
	}
	
	//-----------------------------------------------------------------------------
	// 아무것도 하고 있지 않을때는 심심심한 행동을 처리한다
	//-----------------------------------------------------------------------------
	if( m_curt_event.type == NULL )
	{
		//라이딩 대상은 돌발동작을 마스터의 돌발동작을 따른다.
		if( m_State.nRideState != nRideState_Ride )
		{
			if( SetSimSim() == true )
				m_ani_frame = 0;
		}
		else
		{
			m_SimTime = g_nowTime;
		}
	}
	//-----------------------------------------------------------------------------
	// 사운드플래그 false 
	//-----------------------------------------------------------------------------
	m_bSoundPlay1 = false;
	m_bSoundPlay2 = false;
	return 0;
}

// 각각의 이벤트 별로 매 프레임마다 체크해서 꼭 처리해야 될 루틴들만 처리 
void NonPlayer::UpdateAniProcess()
{
	//	int tgNum = 0;
	int End_Frame = 0 ;
	// 해당 프레임에 한번만 실행됨 
	SoundPlay();
	
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
		
		if( m_curt_event.type == SM_ATTACK_NPC || m_curt_event.type == SM_USE_SKILL )
		{	
			//유져나 .. 유져 리소를 쓰는 넘들 .이다 . 
			if( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )				// 유저가 때림  
			{
				if( m_ani_index == n_Attack10_BowReady0)
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
					m_IsCDataQActed = TRUE;
					RLGS("NonPlayer.cpp --- line:2309");				
					SetUnderAttackEventMgr( m_curt_event );					
					m_curt_event.damage = 0;
				}
				
				//................................................................................................
				// 공격 시작 0 프레임에서 처리 부분 By wxywxy
				//................................................................................................
				if( 1 == m_ani_frame )
					g_Particle.m_EffectManager.AttackStartFrame( ( Character * )this );
				//................................................................................................
				
				
			}
			else  //NPC 이다 . 
			{				
				if(m_IsSkillNoAni)
				{
					SetUnderAttackEventMgr( m_curt_event );
					m_IsSkillNoAni = FALSE;
				}
				else
				{
					End_Frame	=  g_Pc_Manager.GetEndFrame(this,m_ani_index) -4 ;
					if( ( m_ani_frame == m_NPCImpactFrame && m_curt_event.damage >= 0 ) || 
						(m_ani_frame == End_Frame && (m_curt_event.damage >= 0) && (m_bNotImpact == false) ) )
					{
						m_IsCDataQActed = TRUE;
						m_bNotImpact = true; 
#ifdef _DEBUG
						RLGS("NonPlayer.cpp --- line:2326");				
#endif 
						SetUnderAttackEventMgr( m_curt_event );					
						m_curt_event.damage = 0;
					}
				}
			}
			
		}
	}	
	
}


void NonPlayer::SetUnderAttackEventMgr( EVENT_DATA_INFO& aEvent )
{
	if(aEvent.IsActProc == TRUE)
	{
		return;
	}

	aEvent.IsActProc = TRUE;	
	
	EVENT_DATA_INFO underattack_event;	
	
	int tgNum = 0;
	
	if( aEvent.kill )
	{
		RLGS( "m_curt_event.kill SetUnderAttackEventMgr()" );
		RLG2( "Event tbl_Idx = %d, race = %d", aEvent.tbl_Idx, aEvent.race );
	}
	
	switch( aEvent.type )
	{
	case SM_ATTACK_NPC:
		{			
			RLG1( "-=================================aEvent.damage = %d=======--", aEvent.damage );				
			
			// NPC 공격 당함
			if( m_curt_event.race < 0 )
			{
				g_Particle.m_EffectManager.StandAttack(
					( Character * ) this , ( Character * ) &g_Map.m_Npc[aEvent.tbl_Idx] ,
					SM_ATTACK_NPC , aEvent.cell , aEvent.kill , aEvent.damage , aEvent.critical,
					aEvent.lStartTime );
			}
			// 다른 플레이어 가 공격 당함
			else
			{
				// 플레이 공격 당함 
				if( aEvent.unique == g_Pc.m_MainPC.lUnique )
				{
					RLG4( "SetUnderAttackEventMgr Attacked's  PlayerHP = %d, RealHP = %d, aEvent.number = %d, aEvent.code = %d", nRui->thePcParam.Stat1.theHP, g_Pc.m_MainPC.char_info.theRealHP, aEvent.number, aEvent.code );		
					
					g_Particle.m_EffectManager.StandAttack(
						( Character * ) this , ( Character * ) g_Pc.GetPlayer() ,
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
			
			// 콤보 시퀀스 완성시에 추가 효과 스킬 적용 - 이동불가 시에만 적용
			if( (aEvent.code == 4801) && ( aEvent.kill == false ) )
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
				skill_event.lStartTime = aEvent.lStartTime;
				
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
				skill_event.target_list = SAFE_NEW_ARRAY( _STargetInfo , 1 );								
				memcpy( skill_event.target_list, &starget_info, sizeof(_STargetInfo) );
				
				g_Pc.theSkillSystem.SetDamageValue( aEvent.code, &starget_info );
				g_Pc.theSkillSystem.UseSkillTargetMgr( this , skill_event, starget_info );	
				g_Pc.theSkillSystem.UseSkillTargetInfoMgr_AddEffect( this, starget_info, skill_event.count, TRUE );
			}
			
			if( aEvent.unique == g_Pc.m_MainPC.lUnique )
			{
				RLG2( "SetUnderAttackEventMgr Attacked's  PlayerHP = %d, RealHP = %d", nRui->thePcParam.Stat1.theHP, g_Pc.m_MainPC.char_info.theRealHP );		
			}
		}
		break;
	case SM_USE_SKILL:
		{
			if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
			{
				///--PRINT_DLG_LOG("유저형 NPC 스킬시전 - 스킬 코드 %d 대상수 %d", aEvent.count, aEvent.number );
			}			
#ifdef _DEBUG	
			DebugPrint( "-------------NonPlayer Skill Impact Frame %d----------------\n",aEvent.skill_unique);
#endif 
			SSkillBaseInfo* skill_Info;
			
			skill_Info = g_Pc_Manager.GetSkillBaseInfoPtr( aEvent.count ); 
			
			if( skill_Info == NULL )	
				return;
			
			if( aEvent.unique == g_Pc.m_MainPC.lUnique )
			{
				RLG4( "SetUnderAttackEventMgr Attacked's  PlayerHP = %d, RealHP = %d, aEvent.number = %d, skill_Info->theEffectType = %d", nRui->thePcParam.Stat1.theHP, g_Pc.m_MainPC.char_info.theRealHP, aEvent.number, skill_Info->theEffectType );		
			}
			
			if( aEvent.number == 0 )	
			{
				if( skill_Info->theActionEffect != 0 )
				{
					EFFECT_EXTRA_INFO TempInfo;
					
					TempInfo.SkillCode   = aEvent.count;
					TempInfo.SkillUnique = aEvent.skill_unique;
					TempInfo.lStartTime  = aEvent.lStartTime;
					
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
					
					for( tgNum = 0; tgNum < aEvent.number; ++tgNum )
					{
						_STargetInfo* tg_Info = (_STargetInfo*) aEvent.target_list;
						
						if( tg_Info )
						{
							g_Pc.theSkillSystem.UseSkillTargetMgr( this , aEvent, tg_Info[tgNum] );													
							
							if( aEvent.unique == g_Pc.m_MainPC.lUnique )
							{
								RLG2( "SetUnderAttackEventMgr Attacked's  PlayerHP = %d, RealHP = %d", nRui->thePcParam.Stat1.theHP, g_Pc.m_MainPC.char_info.theRealHP );		
							}
						}
						else
						{
							int a = 0;
						}
					}
				}
				break;
				
				// 1 = 1 : N
			case 1:
				{
					if( aEvent.number == 1 )
					{
						for( tgNum = 0; tgNum < aEvent.number; ++tgNum )
						{
							_STargetInfo* tg_Info = (_STargetInfo*) aEvent.target_list;
							
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

void NonPlayer::ApplyShaowProj(SHADOW_INFO & sShadow)
{
	D3DXMATRIX	matRideLocalToLight;
	D3DXMATRIX	matShadow;	
	D3DXVECTOR4 vOut;
		
	D3DXMatrixMultiply( &matRideLocalToLight, &m_matChr, &sShadow.matWorldToLight );
		
	// 스케일	
	D3DXMATRIX	matScale;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixMultiply( &matRideLocalToLight, &matRideLocalToLight, &matScale );	
	
	//-----------------------------------------------------------------------------
	// 죽어있거나, 플래그 false면 그림자X
	//-----------------------------------------------------------------------------
	if( m_curt_event.type != SM_STATE_DIE && m_bShadowDisplay == true )
	{	
		int add_count = 0, itm_index = 0;
		int itm_count = 0;
		
		itm_count = g_Pc_Manager.GetItmCount( this );
		
		//-----------------------------------------------------------------------------
		// 프로젝션
		//-----------------------------------------------------------------------------
		if( g_Shadow == 2 )
		{					
			int VtxCount = 0;
			
			for( int i = 0; i < itm_count+add_count; ++i )
			{																		
				VtxCount = g_Pc_Manager.GetVtxCount( this, i );
				//				}
				for( int j = 0; j < VtxCount; ++j )
				{					
					D3DXVec3Transform( &vOut, &GetVertex( i, j ), &matRideLocalToLight );
					
					if( (float)fabs(vOut.x/vOut.z) > sShadow.fXMax )
						sShadow.fXMax = (float)fabs(vOut.x/vOut.z);
					if( (float)fabs(vOut.y/vOut.z) > sShadow.fYMax )
						sShadow.fYMax = (float)fabs(vOut.y/vOut.z);				
				}
			}			
		}	
	}
}

void NonPlayer::RenderShadow(SHADOW_INFO & sShadow)
{
	//-----------------------------------------------------------------------------
	// 죽어있거나, 플래그 false면 그림자X
	//-----------------------------------------------------------------------------
	if( m_curt_event.type != SM_STATE_DIE && m_bShadowDisplay == true )
	{					
		D3DXMATRIX	matTemp = m_matChr;	
		
		// 스케일	
		D3DXMATRIX	matScale;
		D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z );
		D3DXMatrixMultiply( &matTemp, &matScale, &matTemp );
		
		int add_count = 0, itm_index = 0;
		int itm_count = 0;
		
		itm_count = g_Pc_Manager.GetItmCount( this );		
		
		//-----------------------------------------------------------------------------
		// 렌더링
		//-----------------------------------------------------------------------------
		if( g_Shadow == 2 )
		{
			g_RenderManager.SetTransform( D3DTS_WORLD,	&matTemp );
	
			//-----------------------------------------------------------------------------
			// 야생마는 몸통만 렌더링한다, 페어리는 날개안그린다
			// 스티드는.... 타고 있는 사람이 있다면 함께 렌더링한다
			//-----------------------------------------------------------------------------		
			int mod_index = 0;
#ifdef DIRECT_VERSION_9_MJH
			LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
#else
			LPDIRECT3DVERTEXBUFFER8 pVB = NULL;
#endif // DIRECT_VERSION_9_MJH
			
			//-----------------------------------------------------------------------------
			// 몸통렌더링				
			//-----------------------------------------------------------------------------						
			for( int i = 0; i < itm_count+add_count; ++i )
			{												
				// 유저
				if( m_bPC == true )
				{						
					mod_index = g_Pc_Manager.m_pItm[m_pItm_index[i]]->m_mod_index;
					
					if( g_Pc_Manager.GetVtxCount( this, i ) == 0 )
						continue;						
					//..........................................................................................
					// 셋트 아이템 하의 부츠 장갑 그려주기 막기 by simwoosung
					//..........................................................................................
					if( CHAR_WEAR_SETITEM == m_Char_WearType )
					{
						if( n_Wear_PantsArmor == i || n_Wear_Gloves == i || n_Wear_Shoes == i )
						{
							continue;
						}
					}
				}
				// NPC
				else
				{
					if( i >= itm_count )
					{							
						itm_index = (int)(i-itm_count);
						
						// 다리는 할 필요 없다
						if( itm_index == 2 )
							continue;						
						
						mod_index = g_Pc_Manager.m_pItm[mod_index]->m_mod_index;								
					}
					else
					{
						mod_index = g_Pc_Manager.GetModIndex( this, i );
					}						
				}					
				if( i >= itm_count )
					g_RenderManager.SetStreamSource( 0, pVB, sizeof(OBJECTVERTEX) );
				else					
					g_RenderManager.SetStreamSource( 0, m_pVtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );
				
				// 렌더링
				g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
				DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
					g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
			}
			
		}
	}
}

void NonPlayer::UpdateHPData()
{
	if(m_bPC)
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_lPreTime , HPREFRESH_TIME ) , < , g_nowTime ) )
		{
			if( !IsRealDie() )
			{
				////-- 이부분에서 실제 HP를 보정시킨다.
				if( g_Pc.m_PcTable[m_nTableIndex].char_info.theHP != 
					g_Pc.m_PcTable[m_nTableIndex].char_info.theRealHP )
				{
					///--PRINT_DLG_LOG("theHP = %d theRealHP %d", 
					///--	g_Pc.m_PcTable[m_nTableIndex].char_info.theHP, 
					///--	g_Pc.m_PcTable[m_nTableIndex].char_info.theRealHP);
					///--PRINT_DLG_LOG("NonPlayer HP 동기화");
					
					g_Pc.m_PcTable[m_nTableIndex].char_info.theHP 
						= g_Pc.m_PcTable[m_nTableIndex].char_info.theRealHP;
					
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
				g_Particle.m_EffectManager.DeleteOtherPlayerEffect( (Character *)this );
				
				// 죽을때 효과
				g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , GetUnique() , -1 );
				///--PRINT_DLG_LOG("NonPlayer 죽음 보정작업");
			}
			
			m_lPreTime = g_nowTime;
		}
	}
	else
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_lPreTime , HPREFRESH_TIME ) , < , g_nowTime ) )
		{
			if( !IsRealDie() )
			{
				////-- 이부분에서 실제 HP를 보정시킨다.
				if( g_Map.m_NpcTable[m_nTableIndex].theHP != 
					g_Map.m_NpcTable[m_nTableIndex].theRealHP )
				{
					///--PRINT_DLG_LOG("theHP = %d theRealHP %d", 
					///--	g_Map.m_NpcTable[m_nTableIndex].theHP, 
					///--	g_Map.m_NpcTable[m_nTableIndex].theRealHP);
					///--PRINT_DLG_LOG("Monster HP 동기화");
					
					g_Map.m_NpcTable[m_nTableIndex].theHP 
						= g_Map.m_NpcTable[m_nTableIndex].theRealHP;
					
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
				
				if( m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					die_event.motion = n_Die9;
				}
				else
				{
					die_event.motion = n_NPC_Die0 + rand()%2;
				}			
				
				PushQueue( die_event, TRUE );
				
				///--PRINT_DLG_LOG("Monster 죽음 보정작업");
			}
			
			m_lPreTime = g_nowTime;
		}
	}
}

