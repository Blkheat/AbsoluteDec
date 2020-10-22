#pragma once

//#define		SHUTDOWN_TEST

#define MAX_VERTEXCOUNT		4000
#define MAX_INDEXCOUNT		3000
#define CREATE_PAR_TIME		(1/30)
#define MIXELDA_ITEMCODE_MIN	60000
#define MAX_LEVEL 110		// 레벨 제한은 110 까지
#define ELDA_ANIFRAME_SYNC
#define ANIMFRAME_NUM		30
#define ELDA_WEAPON_EFFECT
#define CPU_SHARE_RATE		//마우스 포커스가 외부로 나가면 실행창 점유율 감소시킴

//----------------------------------------------------------------------------------------------------------------------
// 파  일   명 : GlobalDefine.h
// 용       도 : 공통 디파인
//----------------------------------------------------------------------------------------------------------------------
// #define BATCH_RENDER                            //배치랜더링
// #define BATCH_OPTIMIZATION                      //배치랜더링
// #define HHM_CAMERA_FOV// 백버퍼 종횡비에 따라 시야각 변경(2009/04/13)
// #define HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
// #define HHM_TEXTBOX_ENABLE// 활성화에 따라 텍스트 색 변경. 비활성색(128, 128, 128) (2009/05/13)
// #define HHM_SLIDEBAR_ENABLE// 활성화에 따라 EventProc() 변경(2009/05/13)
// #define HHM_CHARACTER_SELECT_3D_PICK// 캐릭터 선택창에서 버튼으로 하던 캐릭터 선택을 3D 픽킹으로 바꿈(2009/06/12)
// #define HHM_IF_CHAR_DEL_OPEN_DONT_BT_CLICK// 캐릭터 삭제창이 열렸을 때는 버튼들이 클릭되면 안된다(2009/06/15)

//#define HHM_1600X1200_TEST// Radeon 9550 해상도 1600 X 1200 창모드에서 속도 저하(2009/05/15)
//#define HHM_DRAW_FRUSTUM_LINE// 절두체 그리기(2009/05/19)
//#define APPLYWIDE_MAP// 심우성
//#define HHM_HACK_SHIELD_UPDATE_20090703// 한국/일본/타이완 핵쉴드 업데이트
#define PET_PAY_USEITEM_RENEWAL            // 펫 유료아이템 지속시간 증가 1시간 ->30일 2009/07/09 
#define PET_PAY_USEITEM_HELPTIP            // 펫 유료지속아이템 헬프팁 시간표시  2009/7/13


//----------------------------------------------------------------------------------------------------------------------
// 파  일   명 : GlobalDefine.h
// 용       도 : 작업 디파인
// 최초 작성자 : 문종화
// 최초 작성일 : 2009.04.06
//----------------------------------------------------------------------------------------------------------------------
#ifdef PAYINVENITEM
	#define WIDE_PAY_INVENTORY                    // 유료인벤토리 확장   2009/08/10
#endif
#ifdef WIDE_PAY_INVENTORY
const int c_MaxInvenItemNum( RP_MAX_PAYINVENTORY+RP_MAX_INVENTORY );
#else
const int c_MaxInvenItemNum( RP_MAX_INVENTORY );	
#endif

// 대한민국-------------------------------------------------------------------------------------------------------------
#ifdef KOREA_VERSION
	#ifdef KOREA_TEST_VERSION 
		// 봉인의 성지의 보스몹(도그마 베인, 바라고스, 우로보로스, 루갈, 자냐드)의 이펙트 출력 개수 제한(프레임저하 방지)
		#define HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207 
		//#define DECO_RENEWAL_CORPORATION_MJH      //데코 시즌2 군단시스템 리뉴얼           
		//#define DECO_RENEWAL_GUILD_MJH            //데코 시즌2 길드시스템 리뉴얼 
		//#define DECO_RENEWAL_MJH                  //데코 시즌2 케릭터선택창 리뉴얼 
		//#define RETURN_SERVERLIST_MJH             // 서버선택창으로 되돌리기
		//#define UI_CONTROL_RENEWAL                // UI 컨트롤 리뉴얼     2009/11/23
		#define PET_TIME_TEST                     // 시간제 펫 테스트 2009 12/10
		#define PREMIUMZONE_KEY_HELPTIP           // 프리미엄존 헬프팁   2009 10/24
		#define PREMIUMZONE_KEY                   // 프리미엄존   2009 10/24
		#define HHM_HACK_SHIELD_UPDATE_20090703// 한국/일본/타이완 핵쉴드 업데이트
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703
		#define PAYITEM_EFFECT_TEST               // 부가토토/케라마코 이펙트 출력 테스트  2009/11/20 
		#define SKILLICON_TEST                    // 스킬아이콘 중복 테스트   2009/11/19
		#define TEXFILTERING_TEST                 // 필터링 버그 수정  2009/10/19	
		#define PET_NPCNAME_BUG                   // 펫보관함열쇠사용시 npc네임 버그 수정    2009/10/15
		#define BATCH_EFFECT_SOLUTION             // 이펙트 최적화 관련 깨짐현상 수정 2009/10/10
		//#define DISPLAY_16BIT_SCREENSHOT_DOWN   // 디스플레이 16비트 변경시 클라이언트 다운 수정 2009/09/16
		#define ITEM6331_BUG_TEST                 // 강화주문석 개인상점 버그수정  2009/09/16
		#define CREATE_CHARACTER_EVENT_ITEMSELL   // 신규케릭터 생성이벤트 개인상점 판매금지수정  2009/09/15
		//#define WIDE_PAY_INVENTORY                // 유료 인벤토리 확장  2009/07/24

		//#define FAMOUSE_FIGURES_KJK				  // 데코 시즌2 명성 시스템 리뉴얼(김정기 : 2009/12/29)
 
	#endif// #ifdef KOREA_TEST_VERSION

#endif// #ifdef KOREA_VERSION
// 대한민국-------------------------------------------------------------------------------------------------------------





// 일본-----------------------------------------------------------------------------------------------------------------
#ifdef JAPAN_VERSION

		
		#define PREMIUMZONE_KEY_HELPTIP           // 프리미엄존 헬프팁   2009 10/24
		#define PREMIUMZONE_KEY                   // 프리미엄존   2009 10/24
		#define TEXFILTERING_TEST                 // 필터링 버그 수정  2009/10/19
		#define PET_NPCNAME_BUG                   // 펫보관함열쇠사용시 npc네임 버그 수정    2009/10/15
//		#define BATCH_EFFECT_SOLUTION             // 이펙트 최적화 관련 깨짐현상 수정 2009/10/10
		#define ITEM6331_BUG_TEST                 // 강화주문석 개인상점 버그수정  2009/09/16
		#define PET_TIME_TEST                     // 시간제 펫 테스트 2009 12/10
		#define PAYITEM_EFFECT_TEST               // 부가토토/케라마코 이펙트 출력 테스트  2009/11/20
		#define SKILLICON_TEST                    // 스킬아이콘 중복 테스트   2009/11/19
		#define HHM_HACK_SHIELD_UPDATE_20090703// 한국/일본/타이완 핵쉴드 업데이트(20090714)
		#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// 유료아이템 중 클래스가 9/15/17이라면 거래가능
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703

	#ifdef JAPAN_TEST_VERSION
		
	#endif// #ifdef JAPAN_TEST_VERSION
#endif// #ifdef JAPAN_VERSION
// 일본-----------------------------------------------------------------------------------------------------------------





// 글로벌---------------------------------------------------------------------------------------------------------------
#ifdef GLOBAL_VERSION
	
		#define ITEM6331_BUG_TEST                 // 강화주문석 개인상점 버그수정  2009/09/16
		#define PAYITEM_EFFECT_TEST               // 부가토토/케라마코 이펙트 출력 테스트  2009/11/20
		#define SKILLICON_TEST                    // 스킬아이콘 중복 테스트   2009/11/19
		#define TEXFILTERING_TEST                 // 필터링 버그 수정  2009/10/19
		#define PET_NPCNAME_BUG                   // 펫보관함열쇠사용시 npc네임 버그 수정    2009/10/15
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703

		// 봉인의 성지의 보스몹(도그마 베인, 바라고스, 우로보로스, 루갈, 자냐드)의 이펙트 출력 개수 제한(프레임저하 방지)
		#define HHM_SEALED_TEMPLE_BOSSMOB_EFFECT_LIMIT_20091207


	#ifdef GLOBAL_TEST_VERSION

		
	#endif// #ifdef GLOBAL_TEST_VERSION
#endif// #ifdef GLOBAL_VERSION
// 글로벌---------------------------------------------------------------------------------------------------------------




// 타이완---------------------------------------------------------------------------------------------------------------
#ifdef TAIWAN_VERSION

		#define HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090731
		#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// 유료아이템 중 클래스가 9/15/17이라면 거래가능
		#define PAYITEM_EFFECT_TEST                // 부가토토/케라마코 이펙트 출력 테스트  2009/11/20
		#define SKILLICON_TEST                     // 스킬아이콘 중복 테스트   2009/11/19
		#define TEXFILTERING_TEST                  // 필터링 버그 수정  2009/10/19
		#define PET_NPCNAME_BUG                    // 펫보관함열쇠사용시 npc네임 버그 수정    2009/10/15
		#define ITEM6331_BUG_TEST                  // 강화주문석 개인상점 버그수정  2009/09/16
		#define HHM_HACK_SHIELD_UPDATE_20090703// 한국/일본/타이완 핵쉴드 업데이트(20090731)

	#ifdef TAIWAN_TEST_VERSION



	#endif// #ifdef TAIWAN_TEST_VERSION
#endif// #ifdef TAIWAN_VERSION
// 타이완-------------------------------------------------------------------------------------------------------------





// 홍콩-----------------------------------------------------------------------------------------------------------------
#ifdef HONGKONG_VERSION

		#define HHM_GAME_GUARD_UPDATE_20090908// 게임가드 업데이트(20090925)
	#ifdef HONGKONG_TEST_VERSION

		#define PAYITEM_EFFECT_TEST                // 부가토토/케라마코 이펙트 출력 테스트  2009/11/20
		#define SKILLICON_TEST                     // 스킬아이콘 중복 테스트   2009/11/19
		#define TEXFILTERING_TEST                  // 필터링 버그 수정  2009/10/19	
		#define PET_NPCNAME_BUG                    // 펫보관함열쇠사용시 npc네임 버그 수정    2009/10/15
		#define ITEM6331_BUG_TEST                  // 강화주문석 개인상점 버그수정  2009/09/16
		#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// 유료아이템 중 클래스가 9/15/17이라면 거래가능
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703
	
	#endif// #ifdef HONGKONG_TEST_VERSION
#endif// #ifdef HONGKONG_VERSION
// 홍콩---------------------------------------------------------------------------------------------------------------------