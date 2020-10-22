#pragma once

//#define		SHUTDOWN_TEST

#define MAX_VERTEXCOUNT		4000
#define MAX_INDEXCOUNT		3000
#define CREATE_PAR_TIME		(1/30)
#define MIXELDA_ITEMCODE_MIN	60000
#define MAX_LEVEL 110		// ���� ������ 110 ����
#define ELDA_ANIFRAME_SYNC
#define ANIMFRAME_NUM		30
#define ELDA_WEAPON_EFFECT
#define CPU_SHARE_RATE		//���콺 ��Ŀ���� �ܺη� ������ ����â ������ ���ҽ�Ŵ

//----------------------------------------------------------------------------------------------------------------------
// ��  ��   �� : GlobalDefine.h
// ��       �� : ���� ������
//----------------------------------------------------------------------------------------------------------------------
// #define BATCH_RENDER                            //��ġ������
// #define BATCH_OPTIMIZATION                      //��ġ������
// #define HHM_CAMERA_FOV// ����� ��Ⱦ�� ���� �þ߰� ����(2009/04/13)
// #define HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/23)
// #define HHM_TEXTBOX_ENABLE// Ȱ��ȭ�� ���� �ؽ�Ʈ �� ����. ��Ȱ����(128, 128, 128) (2009/05/13)
// #define HHM_SLIDEBAR_ENABLE// Ȱ��ȭ�� ���� EventProc() ����(2009/05/13)
// #define HHM_CHARACTER_SELECT_3D_PICK// ĳ���� ����â���� ��ư���� �ϴ� ĳ���� ������ 3D ��ŷ���� �ٲ�(2009/06/12)
// #define HHM_IF_CHAR_DEL_OPEN_DONT_BT_CLICK// ĳ���� ����â�� ������ ���� ��ư���� Ŭ���Ǹ� �ȵȴ�(2009/06/15)

//#define HHM_1600X1200_TEST// Radeon 9550 �ػ� 1600 X 1200 â��忡�� �ӵ� ����(2009/05/15)
//#define HHM_DRAW_FRUSTUM_LINE// ����ü �׸���(2009/05/19)
//#define APPLYWIDE_MAP// �ɿ켺
//#define HHM_HACK_SHIELD_UPDATE_20090703// �ѱ�/�Ϻ�/Ÿ�̿� �ٽ��� ������Ʈ
#define PET_PAY_USEITEM_RENEWAL            // �� ��������� ���ӽð� ���� 1�ð� ->30�� 2009/07/09 
#define PET_PAY_USEITEM_HELPTIP            // �� �������Ӿ����� ������ �ð�ǥ��  2009/7/13


//----------------------------------------------------------------------------------------------------------------------
// ��  ��   �� : GlobalDefine.h
// ��       �� : �۾� ������
// ���� �ۼ��� : ����ȭ
// ���� �ۼ��� : 2009.04.06
//----------------------------------------------------------------------------------------------------------------------
#ifdef PAYINVENITEM
	#define WIDE_PAY_INVENTORY                    // �����κ��丮 Ȯ��   2009/08/10
#endif
#ifdef WIDE_PAY_INVENTORY
const int c_MaxInvenItemNum( RP_MAX_PAYINVENTORY+RP_MAX_INVENTORY );
#else
const int c_MaxInvenItemNum( RP_MAX_INVENTORY );	
#endif

// ���ѹα�-------------------------------------------------------------------------------------------------------------
#ifdef KOREA_VERSION
	#ifdef KOREA_TEST_VERSION 
		// ������ ������ ������(���׸� ����, �ٶ��, ��κ��ν�, �簥, �ڳĵ�)�� ����Ʈ ��� ���� ����(���������� ����)
		#define HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207 
		//#define DECO_RENEWAL_CORPORATION_MJH      //���� ����2 ���ܽý��� ������           
		//#define DECO_RENEWAL_GUILD_MJH            //���� ����2 ���ý��� ������ 
		//#define DECO_RENEWAL_MJH                  //���� ����2 �ɸ��ͼ���â ������ 
		//#define RETURN_SERVERLIST_MJH             // ��������â���� �ǵ�����
		//#define UI_CONTROL_RENEWAL                // UI ��Ʈ�� ������     2009/11/23
		#define PET_TIME_TEST                     // �ð��� �� �׽�Ʈ 2009 12/10
		#define PREMIUMZONE_KEY_HELPTIP           // �����̾��� ������   2009 10/24
		#define PREMIUMZONE_KEY                   // �����̾���   2009 10/24
		#define HHM_HACK_SHIELD_UPDATE_20090703// �ѱ�/�Ϻ�/Ÿ�̿� �ٽ��� ������Ʈ
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703
		#define PAYITEM_EFFECT_TEST               // �ΰ�����/�ɶ��� ����Ʈ ��� �׽�Ʈ  2009/11/20 
		#define SKILLICON_TEST                    // ��ų������ �ߺ� �׽�Ʈ   2009/11/19
		#define TEXFILTERING_TEST                 // ���͸� ���� ����  2009/10/19	
		#define PET_NPCNAME_BUG                   // �꺸���Կ������ npc���� ���� ����    2009/10/15
		#define BATCH_EFFECT_SOLUTION             // ����Ʈ ����ȭ ���� �������� ���� 2009/10/10
		//#define DISPLAY_16BIT_SCREENSHOT_DOWN   // ���÷��� 16��Ʈ ����� Ŭ���̾�Ʈ �ٿ� ���� 2009/09/16
		#define ITEM6331_BUG_TEST                 // ��ȭ�ֹ��� ���λ��� ���׼���  2009/09/16
		#define CREATE_CHARACTER_EVENT_ITEMSELL   // �ű��ɸ��� �����̺�Ʈ ���λ��� �Ǹű�������  2009/09/15
		//#define WIDE_PAY_INVENTORY                // ���� �κ��丮 Ȯ��  2009/07/24

		//#define FAMOUSE_FIGURES_KJK				  // ���� ����2 �� �ý��� ������(������ : 2009/12/29)
 
	#endif// #ifdef KOREA_TEST_VERSION

#endif// #ifdef KOREA_VERSION
// ���ѹα�-------------------------------------------------------------------------------------------------------------





// �Ϻ�-----------------------------------------------------------------------------------------------------------------
#ifdef JAPAN_VERSION

		
		#define PREMIUMZONE_KEY_HELPTIP           // �����̾��� ������   2009 10/24
		#define PREMIUMZONE_KEY                   // �����̾���   2009 10/24
		#define TEXFILTERING_TEST                 // ���͸� ���� ����  2009/10/19
		#define PET_NPCNAME_BUG                   // �꺸���Կ������ npc���� ���� ����    2009/10/15
//		#define BATCH_EFFECT_SOLUTION             // ����Ʈ ����ȭ ���� �������� ���� 2009/10/10
		#define ITEM6331_BUG_TEST                 // ��ȭ�ֹ��� ���λ��� ���׼���  2009/09/16
		#define PET_TIME_TEST                     // �ð��� �� �׽�Ʈ 2009 12/10
		#define PAYITEM_EFFECT_TEST               // �ΰ�����/�ɶ��� ����Ʈ ��� �׽�Ʈ  2009/11/20
		#define SKILLICON_TEST                    // ��ų������ �ߺ� �׽�Ʈ   2009/11/19
		#define HHM_HACK_SHIELD_UPDATE_20090703// �ѱ�/�Ϻ�/Ÿ�̿� �ٽ��� ������Ʈ(20090714)
		#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// ��������� �� Ŭ������ 9/15/17�̶�� �ŷ�����
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703

	#ifdef JAPAN_TEST_VERSION
		
	#endif// #ifdef JAPAN_TEST_VERSION
#endif// #ifdef JAPAN_VERSION
// �Ϻ�-----------------------------------------------------------------------------------------------------------------





// �۷ι�---------------------------------------------------------------------------------------------------------------
#ifdef GLOBAL_VERSION
	
		#define ITEM6331_BUG_TEST                 // ��ȭ�ֹ��� ���λ��� ���׼���  2009/09/16
		#define PAYITEM_EFFECT_TEST               // �ΰ�����/�ɶ��� ����Ʈ ��� �׽�Ʈ  2009/11/20
		#define SKILLICON_TEST                    // ��ų������ �ߺ� �׽�Ʈ   2009/11/19
		#define TEXFILTERING_TEST                 // ���͸� ���� ����  2009/10/19
		#define PET_NPCNAME_BUG                   // �꺸���Կ������ npc���� ���� ����    2009/10/15
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703

		// ������ ������ ������(���׸� ����, �ٶ��, ��κ��ν�, �簥, �ڳĵ�)�� ����Ʈ ��� ���� ����(���������� ����)
		#define HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207


	#ifdef GLOBAL_TEST_VERSION

		
	#endif// #ifdef GLOBAL_TEST_VERSION
#endif// #ifdef GLOBAL_VERSION
// �۷ι�---------------------------------------------------------------------------------------------------------------




// Ÿ�̿�---------------------------------------------------------------------------------------------------------------
#ifdef TAIWAN_VERSION

		#define HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090731
		#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// ��������� �� Ŭ������ 9/15/17�̶�� �ŷ�����
		#define PAYITEM_EFFECT_TEST                // �ΰ�����/�ɶ��� ����Ʈ ��� �׽�Ʈ  2009/11/20
		#define SKILLICON_TEST                     // ��ų������ �ߺ� �׽�Ʈ   2009/11/19
		#define TEXFILTERING_TEST                  // ���͸� ���� ����  2009/10/19
		#define PET_NPCNAME_BUG                    // �꺸���Կ������ npc���� ���� ����    2009/10/15
		#define ITEM6331_BUG_TEST                  // ��ȭ�ֹ��� ���λ��� ���׼���  2009/09/16
		#define HHM_HACK_SHIELD_UPDATE_20090703// �ѱ�/�Ϻ�/Ÿ�̿� �ٽ��� ������Ʈ(20090731)

	#ifdef TAIWAN_TEST_VERSION



	#endif// #ifdef TAIWAN_TEST_VERSION
#endif// #ifdef TAIWAN_VERSION
// Ÿ�̿�-------------------------------------------------------------------------------------------------------------





// ȫ��-----------------------------------------------------------------------------------------------------------------
#ifdef HONGKONG_VERSION

		#define HHM_GAME_GUARD_UPDATE_20090908// ���Ӱ��� ������Ʈ(20090925)
	#ifdef HONGKONG_TEST_VERSION

		#define PAYITEM_EFFECT_TEST                // �ΰ�����/�ɶ��� ����Ʈ ��� �׽�Ʈ  2009/11/20
		#define SKILLICON_TEST                     // ��ų������ �ߺ� �׽�Ʈ   2009/11/19
		#define TEXFILTERING_TEST                  // ���͸� ���� ����  2009/10/19	
		#define PET_NPCNAME_BUG                    // �꺸���Կ������ npc���� ���� ����    2009/10/15
		#define ITEM6331_BUG_TEST                  // ��ȭ�ֹ��� ���λ��� ���׼���  2009/09/16
		#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// ��������� �� Ŭ������ 9/15/17�̶�� �ŷ�����
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703
	
	#endif// #ifdef HONGKONG_TEST_VERSION
#endif// #ifdef HONGKONG_VERSION
// ȫ��---------------------------------------------------------------------------------------------------------------------