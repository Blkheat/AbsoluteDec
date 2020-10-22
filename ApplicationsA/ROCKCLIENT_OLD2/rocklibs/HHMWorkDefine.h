//----------------------------------------------------------------------------------------------------------------------
// 파  일   명 : HHMWorkDefine.h
// 용       도 : 작업 디파인
// 최초 작성자 : 함현민
// 최초 작성일 : 2009.04.01
//----------------------------------------------------------------------------------------------------------------------
#ifndef HHM_WORK_DEFINE_H
//#define HHM_WORK_DEFINE_H

#include "Protocols\\BaseDataStructures.h"  


// HHM Define DX8-------------------------------------------------------------------------------------------------------
#define HHM_CAMERA_FOV// 백버퍼 종횡비에 따라 시야각 변경(2009/04/13)
#define HHM_USER_RESOLUTION_SELECT// 해상도 변경(2009/04/23)
#define HHM_TEXTBOX_ENABLE// 활성화에 따라 텍스트 색 변경. 비활성색(128, 128, 128) (2009/05/13)
#define HHM_SLIDEBAR_ENABLE// 활성화에 따라 EventProc() 변경(2009/05/13)
#define HHM_CHARACTER_SELECT_3D_PICK// 캐릭터 선택창에서 버튼으로 하던 캐릭터 선택을 3D 픽킹으로 바꿈(2009/06/12)
#define HHM_IF_CHAR_DEL_OPEN_DONT_BT_CLICK// 캐릭터 삭제창이 열렸을 때는 버튼들이 클릭되면 안된다(2009/06/15)

//#define HHM_1600X1200_TEST// Radeon 9550 해상도 1600 X 1200 창모드에서 속도 저하(2009/05/15)
//#define HHM_DRAW_FRUSTUM_LINE// 절두체 그리기(2009/05/19)
//#define APPLYWIDE_MAP// 심우성

//#define HHM_HACK_SHIELD_UPDATE_20090703// 한국/일본/타이완 핵쉴드 업데이트


// 대한민국-------------------------------------------------------------------------------------------------------------
#ifdef KOREA_VERSION
	#ifdef KOREA_TEST_VERSION

		#define HHM_HACK_SHIELD_UPDATE_20090703// 한국/일본/타이완 핵쉴드 업데이트
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703

	#endif// #ifdef KOREA_TEST_VERSION
#endif// #ifdef KOREA_VERSION
// 대한민국-------------------------------------------------------------------------------------------------------------





// 일본-----------------------------------------------------------------------------------------------------------------
#ifdef JAPAN_VERSION
	#ifdef JAPAN_TEST_VERSION
	#endif// #ifdef JAPAN_TEST_VERSION
		
	#define HHM_HACK_SHIELD_UPDATE_20090703// 한국/일본/타이완 핵쉴드 업데이트(20090714)
	#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// 유료아이템 중 클래스가 9/15/17이라면 거래가능
	#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703
#endif// #ifdef JAPAN_VERSION
// 일본-----------------------------------------------------------------------------------------------------------------





// 글로벌---------------------------------------------------------------------------------------------------------------
#ifdef GLOBAL_VERSION
	#ifdef GLOBAL_VERSION

		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703

	#endif// #ifdef GLOBAL_VERSION
#endif// #ifdef GLOBAL_VERSION
// 글로벌---------------------------------------------------------------------------------------------------------------




// 타이완---------------------------------------------------------------------------------------------------------------
#ifdef TAIWAN_VERSION
		
		#define HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090731
		#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// 유료아이템 중 클래스가 9/15/17이라면 거래가능

	#ifdef TAIWAN_TEST_VERSION
		#define HHM_HACK_SHIELD_UPDATE_20090703// 한국/일본/타이완 핵쉴드 업데이트(20090731)
	#endif// #ifdef TAIWAN_TEST_VERSION

#endif// #ifdef TAIWAN_VERSION
// 타이완-------------------------------------------------------------------------------------------------------------





// 홍콩-----------------------------------------------------------------------------------------------------------------
#ifdef HONGKONG_VERSION
	#ifdef HONGKONG_TEST_VERSION

		#define HHM_GAME_GUARD_UPDATE_20090908// 게임가드 업데이트(20090925)
		
	#endif// #ifdef HONGKONG_TEST_VERSION
	
	#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// 유료아이템 중 클래스가 9/15/17이라면 거래가능

	// 60,000번 이상의 코드를 가진 합성엘다스톤이 박힌 아이템은 거래불가(개인상점 등록 불가)/드랍시 파괴/보관 불가
	#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703

#endif// #ifdef HONGKONG_VERSION
// 홍콩-----------------------------------------------------------------------------------------------------------------




#endif// #ifndef HHM_WORK_DEFINE_H
