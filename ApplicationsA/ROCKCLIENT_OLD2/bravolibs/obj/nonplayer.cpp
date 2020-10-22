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


#define		SELECT_ROTATE_TIME			50.0f	// ���õ��׶�� ȸ���ð�(0.05��)
#define		SELECT_SCALE_SIZE			0.1f	// ���õ��׶�� �����Ϻ�ȭ��

extern	int	GetMax( int x, int xx );
extern		DWORD	FloatToDWORD( float f );

extern		long				g_nowTime;
extern		int					g_Shadow;
extern		int					g_chr_anicount;

extern D3DXVECTOR3			g_AniVertex[3000];	// ���� ���Ͻ� �����ϴ� ���� ( �׸��� ó���� ��� ) By wxywxy

const int c_MobAttackFrame( 14 );

// �α��� ����
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
	// �������̺��� ����
	//-----------------------------------------------------------------------------
	//	g_Pc_Manager.ClearSortInfo( m_bPC, m_nTableIndex );
	
	if( m_bPC )
		g_Map.m_SortTable.Delete( ID_PC , m_nTableIndex , 0 );
	else
		g_Map.m_SortTable.Delete( ID_NPC , m_nTableIndex , 0 );
	
	//-----------------------------------------------------------------------------
	// �Ҵ�� �޸𸮻���
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
		///-- NPC����� ���� ����� ���� ������ ����Ÿ��
		///������ �����Ѵ�.
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
	// ĳ���� ���� �ʱ�ȭ
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
			// ������, ���͸���
			m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( this, i );
		}
		
		m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( this, i );
		
		// ���ؽ����� �Ҵ�		
		m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount(this, i) );
		
		if( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
		{
			// �⺻ �ǻ� 
			m_Basic_DrawItm[i] = 0;	
			m_Basic_Material[i] = m_pMaterial[i];	
		}
	}	
	
	m_vScale = g_Pc_Manager.GetChrScale( this );
	
	//-----------------------------------------------------------------------------
	// ĳ���� �Ӽ� �ʱ�ȭ
	//-----------------------------------------------------------------------------
	if( m_bPC == true )
	{	
		// ���������Ҵ�
		if( m_pUserInfo == NULL )
			m_pUserInfo = SAFE_NEW( USER_INFO );		
		
		m_pUserInfo->ArmorInfo.RHand = g_Pc_Manager.m_PcAttr[g_Pc.m_PcTable[m_nTableIndex].nIndex].r_hand_node;
		m_pUserInfo->ArmorInfo.LHand = g_Pc_Manager.m_PcAttr[g_Pc.m_PcTable[m_nTableIndex].nIndex].l_hand_node;			
		
		m_pUserInfo->bAttack = true;
		
		SetWearItem();
	}
	
	if( ( m_bPC == false ) && ( m_State.nCharacterType == nCharacterTypePlayerNPC ) && ( m_nTableIndex != -1 ) )
	{
		// ���������Ҵ�
		if( m_pUserInfo == NULL )
			m_pUserInfo = SAFE_NEW( USER_INFO );
		
		m_pUserInfo->ArmorInfo.RHand = g_Pc_Manager.m_PcAttr[g_Map.m_NpcTable[m_nTableIndex].index].r_hand_node;
		m_pUserInfo->ArmorInfo.LHand = g_Pc_Manager.m_PcAttr[g_Map.m_NpcTable[m_nTableIndex].index].l_hand_node;			
		
		m_pUserInfo->bAttack = true;
		
		SetPlayer_NpcWearItem();
	}
	else if( ( m_bPC == false ) && ( m_State.nCharacterType == nCharacterTypePlayerNPC ) && ( m_nTableIndex == -1 ) )
	{		
		// ���������Ҵ�
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
	
	// �ִϸ��̼Ǳ⺻����
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
	
	// ����ð�����
	m_PreTime	= g_nowTime;
	m_PreMTime	= g_nowTime;
	m_SimTime	= g_nowTime;	
	
#ifdef BOUND_BOX_DISPLAY

	m_CollisionBox.InitDeviceObjects(g_lpDevice);
	m_CollisionBox.RestoreDeviceObjects();
	m_CollisionBox.Update(GetBoundMinPos(),GetBoundMaxPos());
	
#endif
	
	// ���� �ÿ��� ���� �δ��� Ŭ���̾�Ʈ���� �������� NPC ������ �������´� ^ ^;;
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

			///-- ��Ʈ ������ ����ó��
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
			if( item_Info && item_Info->theType == n_Type_KeepupHair )	// �� ���ÿ� �׷���� �ȴ� (�Ǽ��縮 ��)
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
			// ������, ���͸���
			m_pItm_index[i]	= g_Pc_Manager.GetInitItmIndex( this, i );
		}
		
		m_pMaterial[i]	= g_Pc_Manager.GetItmMaterial( this, i );
		
		// ���ؽ����� �Ҵ�		
		m_pVtxBuffer[i].CreateBuffer( g_Pc_Manager.GetVtxCount(this, i) );
		
		if( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
		{
			// �⺻ �ǻ� 
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
	// ������ �ִ� ��ũ ����Ʈ�� ��� ���� Boa �� By wxywxy
	//	int Idx = g_Pc.FindUser(g_Pc.m_PcTable[m_nTableIndex].char_info.thePCID);
	g_Particle.m_EffectManager.DeleteLink( ( Character * )this );
	g_Particle.m_EffectManager.DeleteWeaponEnChantEffect( ( Character * ) this );	
	
	bool IsEquipWeapon = false;
	
	m_Char_WearType = CHAR_WEAR_NORMAL;
	
	// nRui->PcParam.theWearItem[10] ������ �ڵ忡 ���� ������ 7���� m_pItm_Index�� �����Ѵ� 

	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_CoatArmor].theCode )
	{
		m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_CoatArmor].theCode);
		SetItemBuffer( n_Wear_CoatArmor );
		
		///-- ��Ʈ ������ ����ó��
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
		m_pItm_index[n_Wear_Serve] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Serve].theCode ); // ������ �ڵ忡 �´� �ؽ��� ��ȣ �ε�  
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
				// Į�˱�(�ܻ�)����� ���ؼ� ���̰� ���ϱ�
				m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]]->m_AxisTop;
				m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]]->m_AxisBottom;					
				
				g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );
			}
		}
		
	}
	
	if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode )
	{
		//		item_Info = g_Pc_Manager.GetItemBaseInfo( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode );
		
		//		if(			item_Info->theType == 1 ||			// ��
		//					item_Info->theType == 2 ||			// ���		
		//					item_Info->theType == 5 ||			// ������ 
		//					item_Info->theType == 6 )			// ������ 
		//		{
		
		IsEquipWeapon = true;
		
		g_Particle.m_EffectManager.CreateItemLinkEffect( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode , this );
		
		m_pUserInfo->ArmorInfo.index[0] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode );
		m_pUserInfo->ArmorInfo.Code[0] = g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[n_WearSlot_Weapon].theCode;
	
		// Į�˱�(�ܻ�)����� ���ؼ� ���̰� ���ϱ�
		m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisTop;
		m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisBottom;					
		
		g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );
		
		//.................................................................................................
		// ���� ���� üũ By wxywxy
		//.................................................................................................
		m_ISApproachWeapon = FALSE;			// ���� ���� ��/��
		m_AttackMotion	   = 0;				// ������ ���ݸ�� ��ȣ
		
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
		///-- �������� ��ü�ҽÿ�... �ش� �������� ����� ��尡 �ִϸ��̼ǿ� ���� 
		///-- Ʋ���� �����Ǿ��ִ�. �ٷ� ��� ������ �ִϷ� ��ȯ 
		///-- ������ ���ϰ� �������� �������Ѵ�. 
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
		
		///-- ��Ʈ ������ ����ó��
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
		if( item_Info && item_Info->theType == n_Type_KeepupHair )	// �� ���ÿ� �׷���� �ȴ� (�Ǽ��縮 ��)
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
		m_pItm_index[n_Wear_Serve] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Map.m_NpcTable[m_nTableIndex].theExPcInfo.theWearItem[n_WearSlot_Serve] ); // ������ �ڵ忡 �´� �ؽ��� ��ȣ �ε�  
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
				// Į�˱�(�ܻ�)����� ���ؼ� ���̰� ���ϱ�
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
		
		// Į�˱�(�ܻ�)����� ���ؼ� ���̰� ���ϱ�
		m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisTop;
		m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisBottom;					
		
		g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );
		
		//.................................................................................................
		// ���� ���� üũ By wxywxy
		//.................................................................................................
		m_ISApproachWeapon = FALSE;			// ���� ���� ��/��
		m_AttackMotion	   = 0;				// ������ ���ݸ�� ��ȣ
		
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
// Desc: ĳ���� ������� �����ϱ�
//-----------------------------------------------------------------------------
void NonPlayer::CreateItem()
{		

}


//-----------------------------------------------------------------------------
// Desc: ���� ó��
//-----------------------------------------------------------------------------
void NonPlayer::RespawnProcess()
{	
	//-----------------------------------------------------------------------------
	// ���� �ִϸ��̼��� ������ �ʾҾ�� �Ѵ�
	//-----------------------------------------------------------------------------
	if( m_ani_frame != m_end_frame )
	{
		// NPC
		if( m_bPC == false )
		{				
			// ��(1), �ϴ�(2) ���� ���������� ��Ÿ����
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
	// �ִϸ��̼ǳ� - ���ֱ� �ڼ���, �������ֱ�
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
// Desc: �Ҹ� ó��
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

	// �״� �ִϸ��̼����̰�.. ������ �������̶��..
	//	else if( m_ani_index == n_NPC_Die0 && m_ani_frame == (int)m_end_frame )
	if( SAFE_TIME_COMPARE( m_curt_event.time , <= , g_nowTime ) && m_ani_frame == (int)m_end_frame )
	{
		//-----------------------------------------------------------------------------
		// �״� �ִϸ��̼����Ŀ� �������̱׳� �����
		//-----------------------------------------------------------------------------
		//		if( g_Pc_Manager.m_NpcAttr[GetAttrIndex()].die == 2 && m_ani_frame == m_end_frame )		
		//			return SM_NPC_DIE;	
		//-----------------------------------------------------------------------------
		// ��(1), �ϴ�(1) ���� ���������� �������
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

		// �׸��� ���� 
		m_bShadowDisplay = false;
		//		return -1;
	}

	// �ı�������Ʈ�� ���Ͻ�Ų��.. 
	// �״� �ִϸ��̼��� ���� �ʾҴٸ�.. ������������Ű�� �ȵǱ� ������...
	//	if( GetAttrIndex() == DESTROY_OBJECT_INDEX && m_ani_index != NPC_DEATH0 )
	//		return -1;

	return 0;
}

void NonPlayer::PushQueue(EVENT_DATA_INFO & data, BOOL IsAllClear)
{	
	data.lStartTime = timeGetTime();	
	
	///-- �״� �̺�Ʈ�� ������ �������� ������ ��Ȳ�� �ϰ�ó���Ѵ�.
	if( data.type == SM_STATE_DIE )
	{
		IsAllClear = TRUE;
	}

	///-- ���� ���� ����� ���̰� �̺�Ʈ�� CM_MOVE_ATTACK�� ���� ���
	if( data.type == CM_MOVE_ATTACK )
	{
		///-- ���� ���� ���� ��Ÿ��� üũ�ؼ�... CM_MOVE_ATTACK�� ó���Ұ�����
		///-- �ٷ� ���ÿ�û ��Ŷ�� ���������� �����Ѵ�.
		
		int range = 4;
	
		SNPCBaseInfo* npc_Info;
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_nTableIndex].code );
		if(npc_Info)
		{
			range = npc_Info->theAttackRange;
		}
		//�ش� ��ġ�� �����ϸ� .. ���� �޼����� ������ . 
		bool bFind = FindNpcTile( m_Mov.curt.x, m_Mov.curt.y, range, data );	
		if(bFind)
		{
			CM__ATTACK__NPC( data );
			return;
		}
	}
	
	//���� ��ų�� �̰� ������ �޼����� SM_MOVE_ATTACK �̸� �̵��ض� .. 
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
		///-- ���� ������� ������ �ϰ��־��ٸ�... ���ʽ��� ����Ʈ�� �������ش�
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
	
	///-- ���� ������� ������ �ϰ��־��ٸ�... ���ʽ��� ����Ʈ�� �������ش�
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
		//		RLGS("m_temp_event deleted??? ���� �̰� �����̷�??!!!");
		RLGS("NonPlayer.cpp --- line:641");				
#ifdef ROCKCLIENT_DEV					
		//		SystemPrint( n_DarkRed, n_SystemMsg, "SetQueue ���� ������~~~ SM_ATTACK_NPC" );				
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
		RLGS( "NonPlayer �׾���~~ �� ��" );				
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
					RLGS( "!NonPlayer �� �׾� �ִ� ������...but SM_ATTACK_NPC �޽����� ���ƿ�!!!" );
				}
				else
				{				
					RLGS( "!Monster �� �׾� �ִ� ������...but SM_ATTACK_NPC �޽����� ���ƿ�!!!" );
				}
			}
#endif 
			m_save_event.Clear();
			m_next_event.Clear();
			return;
		}
		
		// �ٸ� ������ �ִ´�� ó���ϴϱ� ��� ��찡 �� ���ܳ� �� ������...�Ѥ�; ��...
		if( data.type == SM_MOVE_BROADCAST || data.type == SM_MOVE_ATTACK || data.type == CM_MOVE_ATTACK ||
			data.type == SM_UNDERATTACK || data.type == SM_SLIDE_NPC || data.type == SM_STATE_DIE || data.type == SM_SELF_DIE )
		{
			if( data.type != SM_SELF_DIE )
			{
				// ������ �״��� ���� �������� ���̴��� �������� �ؾ���...^ ^V
				if( m_curt_event.type == SM_ATTACK_NPC || 
					( m_curt_event.type == SM_USE_SKILL && m_curt_event.status == SM_USE_SKILL ) )
				{
					if( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )				// ������ ����  
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
							//		SystemPrint( n_DarkRed, n_SystemMsg, "QueueProcess c_MobAttackFrame ���� ������~~~ SM_ATTACK_NPC" );				
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
					//���⼭ ����Ʈ �Ѹ��� . 
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
		case SM_ATTACK_NPC:		// ����
			{
#ifdef ROCKCLIENT_DEV					
				//		SystemPrint( n_DarkRed, n_SystemMsg, "���� ������~~~ data.type = SM_ATTACK_NPC" );				
				RLGS( "���� ������~~~ data.type = SM_ATTACK_NPC" );				
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
		case SM_SLIDE_NPC:		// �з�����
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
			
			// �α��� ���� ( ���ڸ����� �׼� ���̺� ó���ϱ� ) By wxywxy
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
		
		// ����ð�����
		m_PreTime = g_nowTime;
	}	
}

//.................................................................................................................
// ���� ���� ó��
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



// �α��� ���� ( ���ڸ����� �׼� ���̺� ó���ϱ� ) By wxywxy
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
			///-- ����̳� ȣ�������� ������ ǥ�ð� ������ �ʾƼ� �ش� ��ƾ�� �߰��Ѵ�.
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
			// ��â ����
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
	// ��ġ�� �������ִ� ���� üũ By wxywxy
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
		
		if(	// �з��� ��-��, ������ ������, ��ó, �ҿ� �극��Ŀ  
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
			(	// �з��� ��-��, ������ ������, �ҵ��, ����Ʈ, �ӽóʸ�, �����̾�  
			( chrAttrIndex == n_Millena_Man || chrAttrIndex == n_Millena_Woman ) &&
			( chrClassType == Wanderer || 
			chrClassType == Swordman || chrClassType == Knight || 
			chrClassType == Mercenary || chrClassType == Slayer ) 
			) ||
			(	// ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ� 
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
		
		if(	// ���� ��-��, ������ ��Ʈ������, �Ҽ���, ���ӳ� 
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
			///-- ��ų�� ���� ����� �и��Ѵ�.	
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
						///-- ��ų�� ����Ǵµ� �ִϸ��̼��� ���°�� -  ��: ����Ÿ��
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
				///-- ��ų�� ����Ǵµ� �ִϸ��̼��� ���°�� -  ��: ����Ÿ��
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
// Desc: �̵�
//-----------------------------------------------------------------------------
bool NonPlayer::SM__MOVE( EVENT_DATA_INFO& data )
{	
	//-----------------------------------------------------------------------------
	// ������ ����
	//-----------------------------------------------------------------------------
	m_Mov.dest.x = data.x;
	m_Mov.dest.y = data.y;
	
	// �����̺�Ʈ�� ���� ó��
	switch( m_curt_event.type )
	{
	case SM_MOVE_BROADCAST:		// �̵�		
		return false;		
		break;
	}
	
	//-----------------------------------------------------------------------------
	// �ִϸ��̼� �÷���
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
	// �̺�Ʈ����
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
	// �ش�������� �̵�
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
	
	// �̵��� �� �ִ� ������ �˻�
	if( FindDestTile( data, bFindNpc ) == true )
	{				
		//-----------------------------------------------------------------------------
		// ������ ����������	
		//-----------------------------------------------------------------------------
		m_Mov.dest.x = data.x;
		m_Mov.dest.y = data.y;				
		
		//-----------------------------------------------------------------------------
		// ��ã�⸦ �ؼ� ĳ���� ����, ȸ������ ���� �����ؾ� �Ѵ�. 		
		//-----------------------------------------------------------------------------
		int count = GetMax(abs((long)m_Mov.dest.x- (long)m_Mov.curt.x), abs((long)m_Mov.dest.y- (long)m_Mov.curt.y));
		if( FindPath( (short)m_Mov.curt.x, (short)m_Mov.curt.y, count ) == true )
		{			
			//-----------------------------------------------------------------------------
			// �����ִٸ�
			//-----------------------------------------------------------------------------
#ifdef NPC_MOVE_TO_SKILL
			if( m_curt_event.type != SM_MOVE_BROADCAST && m_curt_event.type != SM_ATTACK_DELAY 
				&& m_curt_event.type != SM_MOVE_SKILL )
#else 
			if( m_curt_event.type != SM_MOVE_BROADCAST && m_curt_event.type != SM_ATTACK_DELAY)
#endif 
	
			{		
				// ���� Ÿ�� ���
				float step_x, step_y;
				GetMoveStep( m_Mov.path.x, m_Mov.path.y, m_Mov.curt.x, m_Mov.curt.y, step_x, step_y );
				m_Mov.next.x = m_Mov.curt.x+(int)((step_x*GetRunFrame())/CLIENT_TILESIZE);
				m_Mov.next.y = m_Mov.curt.y+(int)((step_y*GetRunFrame())/CLIENT_TILESIZE);
				
				// �̵�Ÿ�� �˻�
				int Res = GetNextPath( m_Mov.curt.x, m_Mov.curt.y, data );
				if( Res == 0 )		// ��ǥ ���� �� ��ǥ���� �ٰ� ������ �׸� �������� �Ǵ��϶�...
				{
					m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );
					
					// ���ο� ���� ����� ���� Ŭ�� ������...
					if( data.type == SM_MOVE_ATTACK )						
					{
						if( m_save_event.race < 0 )
						{
							// ���� �������� ���� ������ ������ ���� ������ �ϹǷ� ���ϰ� ���´�. 
							if( m_curt_event.race < 0 &&							// ���ο� ���� ����� ���� �����̰� 
								m_save_event.tbl_Idx == m_curt_event.tbl_Idx )		// ���� �����̸� ���� ���� 								
							{
								// ���� ���� ����� ���� Ŭ������ ���� ���� �Ұ� 
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
							// ���� �������� ���� ������ ������ ���� ������ �ϹǷ� ���ϰ� ���´�. 
							if( m_curt_event.race >= 0 &&							// ���ο� ���� ����� ���� �����̰� 
								m_curt_event.tbl_Idx != 0 &&						// ��ó�� ���� �����Ҷ��� �Ǿ���...								
								m_save_event.tbl_Idx == m_curt_event.tbl_Idx )		// ���� �����̸� ���� ���� 								
							{
								// ���� ���� ����� ���� Ŭ������ ���� ���� �Ұ� 
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
			// �̵��߿� ȣ��� ��쿡 ���			
			//-----------------------------------------------------------------------------
			else
			{								
				// ������ �̵� �޼��� ������				
				//				CM__MOVE( m_Mov.curt.x, m_Mov.curt.y, m_Mov.dest.x, m_Mov.dest.y );
				// �ٱ�
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
		// ��ã�� �����ߴٸ�.. �ʱ�ȭ�Ѵ�.
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
		
		// �̵��̺�Ʈ ����
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
// Desc: ����Ÿ��ã��
//-----------------------------------------------------------------------------
int NonPlayer::GetNextPath( DWORD& next_x, DWORD& next_y, EVENT_DATA_INFO& data )
{
	//-----------------------------------------------------------------------------
	// �������ۿ� ���� ó��
	//-----------------------------------------------------------------------------
	bool bFind = false;
	switch( data.type )
	{		
	case SM_MOVE_ATTACK:		// �Ϲݰ���
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
			
			// �̵� ���ݽ� 1 Ÿ�� ��ȭ�Ҷ����� Ÿ���� �̵��Ͽ����� Ÿ�� ��ġ�� �缳���Ѵ�. 
			if( bFind == false )
			{
				CheckTargetTile( data );		// ó�� �����Ǿ� �ִ� �̵� ���� Ÿ���� �̵��Ͽ����� üũ�Ѵ�. 
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
		// NPCã��
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
			// ����ã��
			int	nTarget = g_Pc.FindUser( data.unique );
			if( nTarget == MAX_USER_INTABLE )
				return;
			
			// ��ġ����
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
	
	///-- By simwoosung -- ���ݽ� ������ Ŭ���̾�Ʈ �Ÿ����� �߻� Ȯ�ο� ������ ���� 
	if( &m_save_event == &data )
	{
		nADisErrCnt = m_save_event.nADisErrCnt;
	}
	
	//	RLGS( "***********************************CM__ATTACK__NPC()" );
	// ���ݺ��� �̵��� �켱..��Ʈ�ѽ� �����ϰ� �������ٴ� �ǰ� ����. 
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
			//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
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
	else								// ����
	{
		if( g_Pc.m_Pc[data.tbl_Idx].m_curt_event.type == SM_STATE_DIE )
		{
			//			SystemPrint( n_DarkRed, n_SystemMsg, "�̹� �׾��ִ� Ÿ���Դϴ�" );
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

		///-- ���� �̵��ؼ� �����ؾ� �ϹǷ�... ���� Ÿ���� 0���� �����Ѵ�.
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
	// �Ÿ�üũ�ϱ�
	//-----------------------------------------------------------------------------
	if( count >= g_Pc.GetMapSize() )			
		return false;
	
	//-----------------------------------------------------------------------------
	// ��ã�����
	//-----------------------------------------------------------------------------	
	theFindWay->StartPos.X	= now_x;
	theFindWay->StartPos.Y	= now_y;
	theFindWay->EndPos.X	= m_Mov.dest.x;
	theFindWay->EndPos.Y	= m_Mov.dest.y;	
	theFindWay->_FindWay();
	
	// �������� ����Ǿ���
	if( theFindWay->GoalReached == 0 )
	{
		m_Mov.dest.x = theFindWay->NearPos.X;
		m_Mov.dest.y = theFindWay->NearPos.Y;
	}
	// �������� ����Ǿ���
	if( theFindWay->ChangedEndPos == TRUE )
	{
		m_Mov.dest.x = theFindWay->EndPos.X;
		m_Mov.dest.y = theFindWay->EndPos.Y;
	}
	// ���� �̵�
	if( theFindWay->DirectMove == TRUE )
	{				
		m_Mov.path.x = theFindWay->EndPos.X;
		m_Mov.path.y = theFindWay->EndPos.Y;
	}
	// ��ã�� �̵�
	else
	{
		m_Mov.path.x = theFindWay->JuncPos.X;
		m_Mov.path.y = theFindWay->JuncPos.Y;
	}
	//	DebugPrint("\r\n ����Ÿ�� %d %d (��ã�� Ÿ�� %d %d)", now_x, now_y, m_Mov.path.x, m_Mov.path.y );
	
	return true;
}

bool NonPlayer::FindPath( short now_x, short now_y, EVENT_DATA_INFO& data, int count )
{	
	//-----------------------------------------------------------------------------
	// �Ÿ�üũ�ϱ�
	//-----------------------------------------------------------------------------
	if( count >= g_Pc.GetMapSize() )			
		return false;
	
	//-----------------------------------------------------------------------------
	// ��ã�����
	//-----------------------------------------------------------------------------	
	theFindWay->StartPos.X	= now_x;
	theFindWay->StartPos.Y	= now_y;
	theFindWay->EndPos.X	= m_Mov.dest.x;
	theFindWay->EndPos.Y	= m_Mov.dest.y;	
	theFindWay->_FindWay();
	
	// ������ ������ ������ ���Ⱑ ���� ���� ���� ������...���ݱ��� ������� �� ã�� �ֳ�...�Ѥ�;
	// ī������ ĳ���� ��ġ�� �����ִ� ���� �����̴ϱ� ��ã�⸦ ���ص� ������...�츮�� �� ã�ƾ� �ǳ�...��
	// �������� ����Ǿ���
	if( theFindWay->GoalReached == 0 )
	{
		m_Mov.dest.x = theFindWay->NearPos.X;
		m_Mov.dest.y = theFindWay->NearPos.Y;
	}
	// �������� ����Ǿ���
	if( theFindWay->ChangedEndPos == TRUE )
	{
		//		RLG4( "������ ����(ChangedEndPos) m_Mov.dest.x=%d, y=%d, theFindWay->NearPos.X=%d, Y=%d", m_Mov.dest.x, m_Mov.dest.y, theFindWay->NearPos.X, theFindWay->NearPos.Y );
		
		m_Mov.dest.x = theFindWay->EndPos.X;
		m_Mov.dest.y = theFindWay->EndPos.Y;
	} 
	// ���� �̵�
	if( theFindWay->DirectMove == TRUE )
	{				
		m_Mov.path.x = theFindWay->EndPos.X;
		m_Mov.path.y = theFindWay->EndPos.Y;
	}
	// ��ã�� �̵�
	else
	{
		m_Mov.path.x = theFindWay->JuncPos.X;
		m_Mov.path.y = theFindWay->JuncPos.Y;
	}
	//	DebugPrint("\r\n ����Ÿ�� %d %d (��ã�� Ÿ�� %d %d)", now_x, now_y, m_Mov.path.x, m_Mov.path.y );
	
	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Desc: �����Ϸ��� ����� ����ֳ�? �� ���ݹ��� �ȿ� �ֳ�?
//-----------------------------------------------------------------------------
bool NonPlayer::FindNpcTile( int _nCurtX, int _nCurtY, int _nRange, EVENT_DATA_INFO& data )
{
	//-----------------------------------------------------------------------------
	// Ÿ���� ��ġ
	//-----------------------------------------------------------------------------
	int nPosX, nPosY;
	BYTE byBlock = 0;
	float distance = 0.0f;

	///--JAPAN_BUG_MODIFY
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	if( data.race < 0 )
	{
		// NPCã��
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
		
		// ��Ÿ��, Position value����
		//		byBlock	= g_Pc_Manager.GetNpcBlockType( nTarget );
		vPos.x = g_Map.m_Npc[nTarget].GetPosTM()._41;
		vPos.z = g_Map.m_Npc[nTarget].GetPosTM()._43;

		nPosX = (DWORD)( vPos.x / (float)CLIENT_TILESIZE );
		nPosY = (DWORD)( vPos.z / (float)CLIENT_TILESIZE );	
		
		// �������� ��쿣 ������ ���� ����� �ϹǷ� �Ÿ� �� ���� 
		if( data.race == c_NpcRace_Item )
		{
			//			_nRange = CLIENT_TILESIZE * 4;
			_nRange = 4;
		}
		else
		{
			if( IsNonPlayerCode( g_Map.m_NpcTable[nTarget].code ) )
			{
				if( g_Map.m_NpcTable[nTarget].code == 5301 )		// õ���� ������ �� �� �ָ��� ^ ^;;
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
				///-- �̰� �̴�� �����ϸ�... �Ÿ��� �ʹ� �ָ� �������մٰ� ����
				///-- �����ʰ� �?��ִ?�۾��� �ʿ��ҵ�....				
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
			// ����ã��
			int	nTarget = g_Pc.FindUser( data.unique );
			if( nTarget == MAX_USER_INTABLE )
				return false;
			
			// ��ġ����
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
	// �̵� �� �� �ִ� ������ Ÿ�� �˻� 
	// FindDestTile()
	// ��ã�� ����
	// FindPath()
	// ���� Ÿ�� ��� 
	/*		float step_x, step_y;
	GetMoveStep( m_Mov.path.x, m_Mov.path.y, m_Mov.curt.x, m_Mov.curt.y, step_x, step_y );
	m_Mov.next.x = m_Mov.curt.x+(int)((step_x*GetRunFrame())/CLIENT_TILESIZE);
	m_Mov.next.y = m_Mov.curt.y+(int)((step_y*GetRunFrame())/CLIENT_TILESIZE);
	*/	
	// �̵��� ó�� ��ƾ 
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
	// ���� �̵��� Ÿ�� �˻��ϱ�
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
		
		// ������ġ
		float	fStepX, fStepY;
		GetMoveStep( m_Mov.path.x, m_Mov.path.y, m_Mov.curt.x, m_Mov.curt.y, fStepX, fStepY );		
		DWORD	dwPosX = (fCurtX+(fStepX*nRunFrame))/CLIENT_TILESIZE;
		DWORD	dwPosY = (fCurtY+(fStepY*nRunFrame))/CLIENT_TILESIZE;
		
		//-----------------------------------------------------------------------------
		// ����Ÿ�ϰ� ����Ÿ���� �Ÿ��� ����ؼ�...
		// 1Ÿ���̳���� ����Ÿ���� �������� �ȴ�
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
		// ���� Ÿ�ϰ� ����Ÿ���� ����ؼ�..
		// 0Ÿ���̸� ����.. 2Ÿ���̻��̸� �߰������� ����
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
	// �̵������� �������� �˻�
	//-----------------------------------------------------------------------------
	switch( m_curt_event.type )
	{
	case NULL:					// ��������
	case SM_SIMSIM:				// �ɽ��� ����
	case SM_MOVE_BROADCAST:		// �̵� ���� ���� 	
	case SM_ATTACK_DELAY:		// ���� ��� �ڼ� 
	case SM_MOVE_ATTACK:		// �̵� ���� 
	case CM_MOVE_ATTACK:		// �̵� ���� 		
	case SM_ATTACK_NPC:			// ���� 
	case SM_USE_SKILL:			// ���� 
	case SM_UNDERATTACK:		// ���׼�	
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
			// �̵������� ������ üũ
			//-----------------------------------------------------------------------------
			if( FindDestTile( data ) == true )
			{		
				// �̵����϶��� �������� �ٲٱ�
				if( m_curt_event.type == SM_MOVE_BROADCAST 
#ifdef NPC_MOVE_TO_SKILL
					||m_curt_event.type == SM_MOVE_SKILL
#endif 
					)
				{	
					// �̵�������
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
// Desc: ������ �޼��� ������
//-----------------------------------------------------------------------------
bool NonPlayer::CM__MOVE( int _nCurtX, int _nCurtY, int _nNextX, int _nNextY )
{		
	//-----------------------------------------------------------------------------
	// ����Ÿ���̵� ��°� ���� ��Ÿ�Ϻ��� ū���� ������
	//-----------------------------------------------------------------------------
	if( _nNextX >= g_Pc.GetMapSize() || _nNextY > g_Pc.GetMapSize() )
	{
		return false;
	}
	
	//-----------------------------------------------------------------------------			
	// ������ �̵��� ���� Ÿ�� ������
	//-----------------------------------------------------------------------------			
	// ����, ���� �� ���
	long lBlock		= (_nCurtY*g_Pc.GetMapSize())+_nCurtX;
	long lDestBlock = (_nNextY*g_Pc.GetMapSize())+_nNextX;
	long lPathBlock = (m_Mov.path.y*g_Pc.GetMapSize())+m_Mov.path.x;
	//	long ReallBlock	= (m_Mov.curt.y*g_Pc.GetMapSize())+m_Mov.curt.x;
	
	// ���� �ٶ󺸴°�
	int nAngle = GetAngle( lBlock, lDestBlock );
	if( -1 != nAngle )
		m_byAngle = nAngle;
	
	// ���� ���̺� ����.
	g_Map.m_NpcTable[m_nTableIndex].lBlock = lBlock;
	g_Map.m_NpcTable[m_nTableIndex].lDestBlock = lDestBlock;
	
	return true;
}


//-----------------------------------------------------------------------------
// Desc: ����
//-----------------------------------------------------------------------------
bool NonPlayer::SM__ATTACK__NPC( EVENT_DATA_INFO& data )
{	
	//-----------------------------------------------------------------------------
	// ���� �ൿ ����
	//-----------------------------------------------------------------------------
	SetBreakEvent();
	
	//-----------------------------------------------------------------------------
	// ���� �ִϸ��̼� ���̿��ٸ�..
	//-----------------------------------------------------------------------------
	//	if( m_curt_event.type == SM_NPC_RESPAWN )
	//	{
	//for( int i = 0; i < m_itm_count; ++i )
	//		for( int i = 0; i < g_Pc_Manager.GetItmCount( this ); ++i )
	//			m_pMaterial[i].Diffuse.a = 1.0f;				
	//	}
	
	//-----------------------------------------------------------------------------
	// �̺�Ʈ ����
	//-----------------------------------------------------------------------------
	//	if( m_curt_event.type == SM_UNDERATTACK )
	//		m_save_event.type = SM_UNDERATTACK;
	
	m_IsCDataQActed = FALSE;
	
	m_curt_event = data;
	
	//-----------------------------------------------------------------------------
	// �ش� Ÿ�� �������� ȸ���ϱ�	
	//-----------------------------------------------------------------------------	
	//	SetDirection( data.vPos.x, data.vPos.z );	
	//............................................................................................................
	// ��ġ�� �������ִ� ���� üũ By wxywxy
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
	// ���� ���� ����
	//-----------------------------------------------------------------------------
	//	int angle = GetAngle( m_Mov.curt.x, m_Mov.curt.y, data.x, data.y );
	//	if( angle != -1 && m_byAngle != angle )
	//	{				
	//		m_byAngle = angle;				
	//	if( m_State.bRide == true )
	//		g_Map.m_Npc[m_State.horse_index].m_byAngle = angle;		
	//	}
	
	//-----------------------------------------------------------------------------
	// �ִϸ��̼� �ٲٱ�
	// NPC�� �ε������� 5�̻��̸� �����ִϸ��̼��̴�		
	//-----------------------------------------------------------------------------
	int index = m_curt_event.motion;
	
	if( m_bPC == true )	
	{
		m_SwordTail.Clear();				// �˱� �ʱ�ȭ by wxywxy
	}
	else
	{
		// ��ų
		/*		if( index >= 5 )
		index = (index-5)+NPC_MAGIG_NUMBER;
		// �Ϲ�
		else
		index += NPC_ATTACK_NUMBER; */
	}
	
	AnimationPlay( index );	
	
	//..............................................................................................
	// NPC ���ݽ� ����
	//..............................................................................................
	if( m_bPC == FALSE )
	{
		int type = rand() % 3;
		
		switch( type )
		{
			//......................................................................................
			// ���� 0
			//......................................................................................
		case 0:
			{	
				SNPCBaseInfo *pInfo = g_Pc_Manager.GetNPCBaseInfo( m_nTableIndex );
				
				//g_Sound.Play( pInfo->theAttackSnd1 , NULL );
			}
			break;
			
			//......................................................................................
			// ���� 1
			//......................................................................................
		case 1:
			{
				SNPCBaseInfo *pInfo = g_Pc_Manager.GetNPCBaseInfo( m_nTableIndex );
				
				//g_Sound.Play( pInfo->theAttackSnd2 , NULL );
			}
			break;
			
			
			//......................................................................................
			// ���ݼҸ� ����
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
	
	// �������� ���� ReAction
	// ��, ��, ũ��Ƽ�� 
	// MaxHealth 30% ���ϴ� �൥���� �̻��� ��������, ũ��Ƽ���� ���� ó��...
	
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
	
	// �ִϸ��̼� �÷����ϱ�
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
		
		// should we put an item for every NPC ... �� ��; Hmmm ... it's not pouring all at once ... once ...
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
	
	// �ִϸ��̼� �÷����ϱ�
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
// Desc: �׸��� ����
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
	// �׾��ְų�, �÷��� false�� �׸���X
	//-----------------------------------------------------------------------------
	if( m_curt_event.type != SM_STATE_DIE && m_bShadowDisplay == true )
	{					
		///-- ���� ����� ��Ʈ������ �����ϴ� AniRender()�Լ��� Display �Լ� ������ �����ϱ� ������
		///-- ���� ����� ��Ʈ������ �������� �ȴ�. �̸� ���⼭ �ٽ� ���Ѵ�.
		if(g_Shadow == 2)
		{
			BoneRender();
		}
		
		// ����Ʈ���� �� ���Ʈ����
		D3DXMATRIX	matCharLocalToLight;
		D3DXMATRIX	matShadow;
		D3DXVECTOR4	vOut;
		D3DXVECTOR3 vPos;
		m_Shadow.fXMax = 0.0f; 
		m_Shadow.fYMax = 0.0f;	
		
		D3DXMATRIX	matTemp;	
		
		///-- �ü��� ��ġ�� ����Boa��
		if( g_Shadow == 2 )	
		{	// �������Ǳ׸���
			vPos = D3DXVECTOR3(m_matChr._41+m_vLight.x, m_matChr._42+m_vLight.y, m_matChr._43+m_vLight.z);	
		}
		else
		{	// ���׶��׸���	
			vPos = D3DXVECTOR3(m_matChr._41, m_matChr._42+20.0f, m_matChr._43-1.0f);
		}	
		
		///-- ��, ���, ������ - �交�͸� ���Ѵ�.
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

		///-- ������ 	
		///-- ���� �������� matLocalToLight�� ĳ���� �ڽ��̴�.   
		D3DXMATRIX	matScale;
		D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z );		
		D3DXMatrixMultiply( &matTemp, &matScale, &matTemp );
		
		// ���ö���Ʈ
		D3DXMatrixMultiply( &matCharLocalToLight, &matTemp, &m_Shadow.matWorldToLight );
		
		// ��������
		if( g_Shadow == 2 && m_IsApplyVA )
		{		
			///-- ���� �׸��ڸ� �׸��� ����Ǵ� �������� ����� ���Ѵ�.
			D3DXMatrixPerspectiveFovLH( &m_Shadow.matShadowProj, D3DX_PI/4, 1.3f, 1.0f, 10000.0f );
			
			int itm_count = 0;
			
			itm_count = g_Pc_Manager.GetItmCount( this );
			
			for( int i = 0; i < itm_count; ++i )
			{					
				
				//..........................................................................................
				// ��Ʈ ������ ���� ���� �尩 ���� by simwoosung
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
			
			if(m_State.bRide)	///-- ���̵� ���̸� �������� ��Ŀ� ���̵� ���ؽ��� �����Ѵ�.
			{
				g_Map.m_Npc[m_State.TransIndex].ApplyShaowProj(m_Shadow);
			}
			
			///-- ���� ���⵵ �������� ��Ŀ� ���ؽ��� �����Ų��.
			//.................................................................................................
			// �׸��� ǥ�ÿ��� ( ����� ) By wxywxy
			//.................................................................................................
			static BOOL ISDisplayWeaponShadow = TRUE;
			
			ISDisplayWeaponShadow = TRUE;
			
			switch( m_ani_index )
			{
				//.............................................................................................
				// ���� Į ���� ����
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
			
			///-- ���� �׸��ڰ� �׷�����....
			if( ISDisplayWeaponShadow && m_pUserInfo)
			{
				// ������ ���ؽ��� �������ǿ� �����Ų��.
				ApplyWeaponShaowProj();
			}
			
			///-- ��ũ ����Ʈ �������� ���ؽ��� �������ǿ� �����Ų��.
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
			
			///-- ���� �������� ����� ���Ѵ�.
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
			// ������
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
				// ����
				//-----------------------------------------------------------------------------		
				int mod_index = 0;			
				int itm_count = 0;
				
				itm_count = g_Pc_Manager.GetItmCount( this );			
				
				for( int i = 0; i < itm_count; ++i )
				{						
					//..........................................................................................
					// ��Ʈ ������ ���� ���� �尩 �׷��ֱ� ���� by simwoosung
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
					// ��ũ�� ���� 
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
							
							// ��Ʈ����
							D3DXMATRIX matWorld;
							if( i == 0 )
								matWorld = m_matRHand;
							else if( i == 1 )
								matWorld = m_matLHand;
							
							// ������					
							D3DXMatrixMultiply( &matWorld, &matScale, &matWorld );							
							// �������Ʈ��ġ
							//D3DXMatrixMultiply( &matItemLocalToLight, &matWorld, &m_Shadow.matWorldToLight );							
							// ����Ʈ x �������� = �׸������
							//D3DXMatrixMultiply( &matShadow, &matItemLocalToLight, &m_Shadow.matShadowProj );							
							g_RenderManager.SetTransform( D3DTS_WORLD,	&matWorld );		
							
							// ������
							mod_index = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[i]]->m_mod_index;										
							g_RenderManager.SetStreamSource( 0, g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[i]]->m_pNormalVB, sizeof(OBJECTVERTEX) );
							g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );	
							
							DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
								g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );							
						
						}
					}
				}
				
				//.................................................................................................
				// ��ũ ����Ʈ �׸��� ����ֱ�
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

	///-- ������ 	
	///-- ���� �������� matLocalToLight�� ĳ���� �ڽ��̴�.   
	D3DXMATRIX	matScale;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z );
	//-----------------------------------------------------------------------------		
	// ��ũ�� ���� 
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
			
			// ��Ʈ����
			D3DXMATRIX matWorld;
			if( i == 0 )
				matWorld = m_matRHand;
			else if( i == 1 )
				matWorld = m_matLHand;	
			
			// ������					
			D3DXMatrixMultiply( &matWorld, &matScale, &matWorld );			
			// �������Ʈ��ġ
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
	// �׸��� ������
	//-----------------------------------------------------------------------------
	if( m_bShadowDisplay == true && m_curt_event.type != SM_STATE_DIE )
	{			
		D3DXMATRIX	matOffset;					
		D3DXMatrixIdentity( &matOffset );
		matOffset._11 =  0.5f;
		matOffset._22 = -0.5f;	
		matOffset._31 =  0.5f;
		matOffset._32 =  0.5f;		
		
		// �ؽ�����ĸ����
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
		
		// �ؽ��� ���
		g_RenderManager.SetTransform( D3DTS_TEXTURE0, &m_Shadow.matShadowUV );
		
		// ������
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
// Desc: ���õ��׶�̱׸���
//-----------------------------------------------------------------------------
void NonPlayer::SelectDisplay()
{
	

}


//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼� ���μ���
//-----------------------------------------------------------------------------
int NonPlayer::AniProcess(bool bFlag)
{
	if( SAFE_TIME_COMPARE( m_PreTime , > , g_nowTime ) )
	{
		m_PreTime = g_nowTime;
		///-- ���⼭ m_PreTime�� g_nowTime���� ũ��
		///-- g_nowTime - m_PreTime�� -�� ulong ���̹Ƿ� ��û ū������ ��ȯ�ȴ�.
		///-- ���� m_middle_time���� ���̰� �ǰ� 
		///-- �̵�Ÿ���� ����ϴ� ��Ű�� ���� ���κп���
		///-- ����� �߸��� ȭ���� �������� ������ �Ͼ��.
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
	
	// �̵� ������ ������Ʈ 
	
	if( m_State.nRideState != nRideState_Ride)
	{
		///-- ani_time�� float ���̹Ƿ� �񱳰���� �Ұ����ϴ�.
		if( SAFE_TIME_SUB( g_nowTime , m_PreMTime ) >= ani_time  )		
		{
			float fAdd_Frame = (float)(SAFE_TIME_SUB( g_nowTime , m_PreMTime ) / ani_time );
			int add_frame = (int)fAdd_Frame; 		
			
			for( int i = 0; i < add_frame; ++i ) 
			{	
				m_move_middle_time = 0.0f; 
				
				//m_move_frame += add_frame;
				++m_move_frame;
				
				// �������� (false) ĳ�����̵��ϱ�			
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
			// �����Ͽ� (true) ĳ�����̵��ϱ�
			AniProcessMove( true );
		}
		
		m_matChr._42 = g_Map.PHF_GetHeight( m_matChr._41 , m_matChr._43 );		

		BOOL Is4FootUnit = FALSE;

		if( !m_bPC  )	///-- NPC�� ���
		{
			DWORD dNPCCode = g_Map.m_NpcTable[m_nTableIndex].code;	///-- NPC�� 4���������� �����Ǵ�...
			SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

			if(npc_Info)
			{
				if( npc_Info->theGender == n_4FootUnit )
				{
					Is4FootUnit = TRUE;
				}
			}
		}

		if( Is4FootUnit )		///-- 4�� ���� �����ϰ�� x��ȸ��
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
	// Ȱ����
	//-----------------------------------------------------------------------------
	///-- ani_time�� float ���̹Ƿ� �񱳰���� �Ұ����ϴ�.
	if( SAFE_TIME_SUB( g_nowTime , m_PreTime ) >= ani_time  )
	{	
		// ��Ű������ ���
		int skip_count = (int)(SAFE_TIME_SUB( g_nowTime, m_PreTime ) / ani_time );
		//for( int i = 0; i < skip_count; ++i )
		{
			//-----------------------------------------------------------------------------
			// NPC ����
			//-----------------------------------------------------------------------------			
			//			if( m_curt_event.type == SM_NPC_RESPAWN )			
			//				RespawnProcess();			
			//-----------------------------------------------------------------------------
			// NPC ����
			//-----------------------------------------------------------------------------
			//			else if( m_curt_event.type == SM_STATE_DIE )
			if( m_curt_event.type == SM_STATE_DIE && m_curt_event.status == SM_STATE_DIE &&
				m_ani_frame == (int)m_end_frame )
			{
				int Res = DieProcess();
			}
			//-----------------------------------------------------------------------------
			// �ִϸ��̼� & �̺�Ʈó���ϱ�
			//-----------------------------------------------------------------------------			
			//			if( !(m_curt_event.type == SM_STATE_DIE && m_ani_frame == (int)m_end_frame) )
			else
			{					
				//-----------------------------------------------------------------------------
				// ���� �÷���
				//-----------------------------------------------------------------------------
				for( int i = 0; i < skip_count; ++i )
				{	
					//					m_ani_frame += skip_count;				
					++m_ani_frame;
					++m_blend_frame;
					
					// ������ �ߺ� m_ani_frame Skip ó��
					///-- ���� ���� ���̶��
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
				
				// ����ó���ϴ��� �˻��ϱ�
				if( UpdateFrame() == false )
				{					
					// �ִϸ��̼�ó��
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
		// �ʱ�ȭ
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
	// ����
	else
	{
		// ����Ÿ������
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
	// ��ó�� ��ŷ�Ҷ� �� �� �ִ� ������ 1�� Ŭ���̾�Ʈ ���, 2�� �������� ����ؼ� ���� �Ǵ� ���̸� �ϴ� ������ ���� 
	return true;
}

//-----------------------------------------------------------------------------
// Desc: ĳ���� �̵��ϱ�
//-----------------------------------------------------------------------------
void NonPlayer::AniProcessMove( bool middle )
{		
	//-----------------------------------------------------------------------------
	// �̵�ó��
	//-----------------------------------------------------------------------------

#ifdef NPC_MOVE_TO_SKILL
	if( m_curt_event.type == SM_MOVE_BROADCAST ||  m_curt_event.type == SM_MOVE_SKILL )
#else 
	if( m_curt_event.type == SM_MOVE_BROADCAST )
#endif 
	{				
		// ��Ÿ�� �ִٸ� ���� ������
		/*if( m_State.bRide == true && m_ani_frame%2 == 0 )
		{
		D3DXVECTOR3 tempv;
		tempv = D3DXVECTOR3( g_Pc.m_Pc[m_nTableIndex].GetPosTM()._41, g_Map.m_fBaseHeightWater, g_Pc.m_Pc[m_nTableIndex].GetPosTM()._43 );
		// ���� ���� �ƴϸ� ��Ƣ����. 
		if( g_Map.m_pAttrMap[m_Mov.curt.y][m_Mov.curt.x].byType == MATYPE_WATER )
		g_Particle.DisplayUpdate( 55, tempv );
		else
		{
		if( g_Pc.FindUser( g_Pc.m_PcTable[m_nTableIndex].lUnique ) != MAX_USER_INTABLE ) // ���̺� ���� ������.
		g_Particle.DisplayUpdate( 6, g_Pc.m_PcTable[m_nTableIndex].lUnique, -1, -1 );
		}
	}*/
		
		//-----------------------------------------------------------------------------
		// ĳ�����̵�
		//-----------------------------------------------------------------------------
		if( middle == false )
		{		
			// ���� Ÿ�Ϸ� �Ѿ�� ������ ��ã�� (1Ÿ�Ͽ� �ѹ��� ��ã�⸦ �Ѵ�.) 			
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
// Desc: ����� �̺�Ʈ ó��
//-----------------------------------------------------------------------------
int NonPlayer::AniProcessMoveEvent()
{
	//-----------------------------------------------------------------------------
	// ���� �̺�Ʈ ó��
	//-----------------------------------------------------------------------------
	switch( m_curt_event.type )
	{		
		// �̵�		

#ifdef NPC_MOVE_TO_SKILL
	case SM_MOVE_SKILL:
#endif 
	case SM_MOVE_BROADCAST:
		{				
			// ����Ÿ������
			m_Mov.curt.x = (int)( (m_matChr._41) / CLIENT_TILESIZE );
			m_Mov.curt.y = (int)( (m_matChr._43) / CLIENT_TILESIZE );
	
			// �������� �����ߴٸ�.
			if( m_Mov.dest.x == m_Mov.curt.x && m_Mov.dest.y == m_Mov.curt.y && m_move_frame == GetRunFrame() )
			{
				if( m_bPC )
				{
					// ���̰�����
					SetHeightStep( true );
					
					// ���� Position value ����
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
					///-- �ϴ� �̺κ��� ��������. 
					if( m_State.nCharState == nCharState_Illusion || 
						m_State.nCharState == nCharState_Detect_Illusion )
					{
						///-- �������� �ε����� �÷��̾� �ڽ��϶�						
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
					
					// ���� Position value ����
					m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );	
					m_curt_event.Clear();				
					
					if( m_save_event.type == SM_MOVE_TURN )		
					{
						SetDirection( 0.0f );						
						m_save_event.Clear();						
					}
					
					// �ִϸ��̼� �÷��� ����. �ȱ�,�ٱⰡ ����Ǿ��� �� �ֱ� ������					
					if( m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						AnimationPlay( n_Stand_Peace9 );
					}
					else
					{
						AnimationPlay( n_NPC_Stand0 );
					}
					
					//���������� ��ų���� ��Ų�� . 
					
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
					// ���̰�����
					SetHeightStep( true );
					
					// ���� Position value ����
					m_Mov.Init( m_Mov.curt.x, m_Mov.curt.y );	
					m_curt_event.Clear();				
					
					if( m_save_event.type == SM_MOVE_TURN )		
					{
						SetDirection( 0.0f );						
						m_save_event.Clear();						
					}
					
					// �ִϸ��̼� �÷��� ����. �ȱ�,�ٱⰡ ����Ǿ��� �� �ֱ� ������					
					if( m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						AnimationPlay( n_Stand_Peace9 );
					}
					else
					{
						AnimationPlay( n_NPC_Stand0 );
					}

					///-- ���� �ϴ� ������Ŷ�� �����Ƿ� ��������.
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
					///-- ���� ��ΰ� ������ ����̶�� �����. �Ϲ� NPC 
					if( theCurtPath == theDestPath )
					{
						// ���̰�����
						SetHeightStep( true );
						
						// ī�޶���
						//						if( g_Pc.m_MainPC.lUnique == m_State.Unite_unique )
						//							g_Camera.Update();
						
						// �ִϸ��̼� �÷��� ����. �ȱ�,�ٱⰡ ����Ǿ��� �� �ֱ� ������					
						if( m_State.nCharacterType == nCharacterTypePlayerNPC )
						{
							AnimationPlay( n_Stand_Peace9 );
						}
						else
						{
							AnimationPlay( n_NPC_Stand0 );
						}						
						
						// ���� Position value ����
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
						
						///-- �ٽ� �̵�����...
						// �ӽ� �̵� �׽�Ʈ ( �ٸ� ������ NPC )
						if( m_bIsLoaded == true )
							SetQueue( data );
					}				
					
					g_Map.m_NpcTable[m_nTableIndex].lDestBlock = thePath[theCurtPath];
				}
				
			}
			// �������� �ʾҴٸ�.. ��� �̵�
			else
			{
				if( m_bPC )
				{
					// �ִϸ��̼� �÷��� ����. �ȱ�,�ٱⰡ ����Ǿ��� �� �ֱ� ������
					if(!m_State.bRide)
					{
						AnimationPlay( n_Run9 );
					}
					else
					{
						AnimationPlay( n_Ride_Run );
						g_Map.m_Npc[m_State.TransIndex].AnimationPlay( n_NPC_Run0 );
					}					
					
					// ����Ÿ�� �̵���										
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
					// �ִϸ��̼� �÷��� ����. �ȱ�,�ٱⰡ ����Ǿ��� �� �ֱ� ������					
					if( m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						AnimationPlay( n_Walk9 );
					}
					else
					{
						AnimationPlay( n_NPC_Walk0 );
					}				
					
					// �̵�, ȸ��, ���̰� ó��
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
// Desc: ����� �̺�Ʈ ó��
//-----------------------------------------------------------------------------
int NonPlayer::AniProcessEvent()
{		
	
	EVENT_DATA_INFO queue_event;
	
	//-----------------------------------------------------------------------------
	// ���� �̺�Ʈ ó��
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
		
		// ����		
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
		// �з�����
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
		// �¾���
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
					
					// ���� ������ ����Ʈ
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
		
		// �α��� ���� ���ڸ����� ���ϸ��̼� ���̺� ó�� By wxywxy
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
	// ���� & ���� �� ����Ѵ� ��ų�� ����  AniProcessEvent ��ӵ��� . 	
	case SM_USE_SKILL:
		{
			if( m_curt_event.status == SM_CHANT_READY )
			{
				if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
				{
					if(m_bPC)
					{
						m_curt_event.status = SM_CHANT_SPELLING;					
						m_curt_event.motion = SetChantMotion( SM_CHANT_SPELLING, 0 ); // �ӽ÷� �ޱ�����..					
						SetQueue(m_curt_event);
						m_SimTime = g_nowTime;
					}
					else if(!m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						m_curt_event.status = SM_USE_SKILL;					
						
						///-- ���ĳ� ���������� ���
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
							m_curt_event.motion = SetChantMotion( SM_USE_SKILL, 0 ); // �ӽ÷� �ޱ�����..					
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
						
						// ��â ��
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
					m_curt_event.motion = n_Skill_RestKepping;			// ��� ���� ��� 
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
				//���� �ϰ��ִµ����� �ٳ����� ??  
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
								
								//���� �� ��ų ���� �������� .. 
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
		// ������ ���������� �ִ� ���̼� 
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
			if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_Weapon_DelayTime , m_Attack_DelayTime ) ) )		// �⺻ ��� 1.5���� ��� �ð�
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
		// �ɽ��Ҷ� �ϴ� �ൿ
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
		
		// ����
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
	// �ƹ��͵� �ϰ� ���� �������� �ɽɽ��� �ൿ�� ó���Ѵ�
	//-----------------------------------------------------------------------------
	if( m_curt_event.type == NULL )
	{
		//���̵� ����� ���ߵ����� �������� ���ߵ����� ������.
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
	// �����÷��� false 
	//-----------------------------------------------------------------------------
	m_bSoundPlay1 = false;
	m_bSoundPlay2 = false;
	return 0;
}

// ������ �̺�Ʈ ���� �� �����Ӹ��� üũ�ؼ� �� ó���ؾ� �� ��ƾ�鸸 ó�� 
void NonPlayer::UpdateAniProcess()
{
	//	int tgNum = 0;
	int End_Frame = 0 ;
	// �ش� �����ӿ� �ѹ��� ����� 
	SoundPlay();
	
	// ����Ʈ ���� ������ üũ  By wxywxy
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
			//������ .. ���� ���Ҹ� ���� �ѵ� .�̴� . 
			if( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )				// ������ ����  
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

				///-- �̺�Ʈ�� ó���Ǿ��ϴµ�.. ����Ʈ �������� ������������ �̹� �Ѿ����
				///-- �ش� �̺�Ʈ�� ó���ؾ߸� �Ѵ�.
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
				// ���� ���� 0 �����ӿ��� ó�� �κ� By wxywxy
				//................................................................................................
				if( 1 == m_ani_frame )
					g_Particle.m_EffectManager.AttackStartFrame( ( Character * )this );
				//................................................................................................
				
				
			}
			else  //NPC �̴� . 
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
			
			// NPC ���� ����
			if( m_curt_event.race < 0 )
			{
				g_Particle.m_EffectManager.StandAttack(
					( Character * ) this , ( Character * ) &g_Map.m_Npc[aEvent.tbl_Idx] ,
					SM_ATTACK_NPC , aEvent.cell , aEvent.kill , aEvent.damage , aEvent.critical,
					aEvent.lStartTime );
			}
			// �ٸ� �÷��̾� �� ���� ����
			else
			{
				// �÷��� ���� ���� 
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
					// �˱� , �Ѱ赹�� ����Ʈ �߰� By wxywxy
					g_Particle.SetEffSetCon( 0.0f , EF_SPREAD_SPOT , 0 , 1.1f , GetUnique() , GetIndex() );
				}				
			}	
			
			// �޺� ������ �ϼ��ÿ� �߰� ȿ�� ��ų ���� - �̵��Ұ� �ÿ��� ����
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
				
				for( int i = 0; i < 3; ++i )			// ��ų ȿ�� ó�� 3���� ó��  
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
				///--PRINT_DLG_LOG("������ NPC ��ų���� - ��ų �ڵ� %d ���� %d", aEvent.count, aEvent.number );
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
					// ����Ʈ �Ŵ����� ��ų Ÿ�Թ� �� ���� �Է�
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
			
			// ����Ʈ ������ 
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
							// ����Ʈ ���� ��ƾ �� �ɱ� ( ����Ʈ ���� �����ʰ� ť�� ���� & ��� )
							g_Particle.m_EffectManager.EffectCreateLock();	
							
							for( tgNum = 0; tgNum < ( aEvent.number - 1 ); ++tgNum )
							{		
								g_Pc.theSkillSystem.UseSkillTargetMgr( this , aEvent, tg_Info[tgNum] );													
							}
							
							// ����Ʈ ���� ��ƾ �� ���� ( ť�� ������ ����Ʈ ���� )
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
		
	// ������	
	D3DXMATRIX	matScale;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixMultiply( &matRideLocalToLight, &matRideLocalToLight, &matScale );	
	
	//-----------------------------------------------------------------------------
	// �׾��ְų�, �÷��� false�� �׸���X
	//-----------------------------------------------------------------------------
	if( m_curt_event.type != SM_STATE_DIE && m_bShadowDisplay == true )
	{	
		int add_count = 0, itm_index = 0;
		int itm_count = 0;
		
		itm_count = g_Pc_Manager.GetItmCount( this );
		
		//-----------------------------------------------------------------------------
		// ��������
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
	// �׾��ְų�, �÷��� false�� �׸���X
	//-----------------------------------------------------------------------------
	if( m_curt_event.type != SM_STATE_DIE && m_bShadowDisplay == true )
	{					
		D3DXMATRIX	matTemp = m_matChr;	
		
		// ������	
		D3DXMATRIX	matScale;
		D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z );
		D3DXMatrixMultiply( &matTemp, &matScale, &matTemp );
		
		int add_count = 0, itm_index = 0;
		int itm_count = 0;
		
		itm_count = g_Pc_Manager.GetItmCount( this );		
		
		//-----------------------------------------------------------------------------
		// ������
		//-----------------------------------------------------------------------------
		if( g_Shadow == 2 )
		{
			g_RenderManager.SetTransform( D3DTS_WORLD,	&matTemp );
	
			//-----------------------------------------------------------------------------
			// �߻����� ���븸 �������Ѵ�, ���� �����ȱ׸���
			// ��Ƽ���.... Ÿ�� �ִ� ����� �ִٸ� �Բ� �������Ѵ�
			//-----------------------------------------------------------------------------		
			int mod_index = 0;
#ifdef DIRECT_VERSION_9_MJH
			LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
#else
			LPDIRECT3DVERTEXBUFFER8 pVB = NULL;
#endif // DIRECT_VERSION_9_MJH
			
			//-----------------------------------------------------------------------------
			// ���뷻����				
			//-----------------------------------------------------------------------------						
			for( int i = 0; i < itm_count+add_count; ++i )
			{												
				// ����
				if( m_bPC == true )
				{						
					mod_index = g_Pc_Manager.m_pItm[m_pItm_index[i]]->m_mod_index;
					
					if( g_Pc_Manager.GetVtxCount( this, i ) == 0 )
						continue;						
					//..........................................................................................
					// ��Ʈ ������ ���� ���� �尩 �׷��ֱ� ���� by simwoosung
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
						
						// �ٸ��� �� �ʿ� ����
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
				
				// ������
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
				////-- �̺κп��� ���� HP�� ������Ų��.
				if( g_Pc.m_PcTable[m_nTableIndex].char_info.theHP != 
					g_Pc.m_PcTable[m_nTableIndex].char_info.theRealHP )
				{
					///--PRINT_DLG_LOG("theHP = %d theRealHP %d", 
					///--	g_Pc.m_PcTable[m_nTableIndex].char_info.theHP, 
					///--	g_Pc.m_PcTable[m_nTableIndex].char_info.theRealHP);
					///--PRINT_DLG_LOG("NonPlayer HP ����ȭ");
					
					g_Pc.m_PcTable[m_nTableIndex].char_info.theHP 
						= g_Pc.m_PcTable[m_nTableIndex].char_info.theRealHP;
					
					m_lModifyHpTime = timeGetTime();
				}
			}
			
			///-- �����δ� ���� �����ε�.. ���� �׾����� �ʴٸ�...
			if( IsRealDie() && m_curt_event.type != SM_STATE_DIE )
			{
				///-- �״°� �����۾�
				EVENT_DATA_INFO die_event;
				die_event.type = SM_STATE_DIE;
				die_event.loop = false;
				
				die_event.motion = n_Die9;
				g_Particle.DelEffSetCon( EF_REST_BREATH , 0 , GetUnique() , GetIndex() );
				PushQueue( die_event, TRUE );
				
				// ������ ������ �ִ� ����Ʈ ��� �����
				g_Particle.m_EffectManager.DeleteOtherPlayerEffect( (Character *)this );
				
				// ������ ȿ��
				g_Particle.SetEffSetCon( 0.0f , EF_PLAYER_DIE , 0 , 2.4f , GetUnique() , -1 );
				///--PRINT_DLG_LOG("NonPlayer ���� �����۾�");
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
				////-- �̺κп��� ���� HP�� ������Ų��.
				if( g_Map.m_NpcTable[m_nTableIndex].theHP != 
					g_Map.m_NpcTable[m_nTableIndex].theRealHP )
				{
					///--PRINT_DLG_LOG("theHP = %d theRealHP %d", 
					///--	g_Map.m_NpcTable[m_nTableIndex].theHP, 
					///--	g_Map.m_NpcTable[m_nTableIndex].theRealHP);
					///--PRINT_DLG_LOG("Monster HP ����ȭ");
					
					g_Map.m_NpcTable[m_nTableIndex].theHP 
						= g_Map.m_NpcTable[m_nTableIndex].theRealHP;
					
					m_lModifyHpTime = timeGetTime();
				}
			}
			
			///-- �����δ� ���� �����ε�.. ���� �׾����� �ʴٸ�...
			if( IsRealDie() && m_curt_event.type != SM_STATE_DIE )
			{
				///-- �״°� �����۾�
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
				
				///--PRINT_DLG_LOG("Monster ���� �����۾�");
			}
			
			m_lPreTime = g_nowTime;
		}
	}
}

