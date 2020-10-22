#ifndef		_____ROCKCLIENT_MAIN_____
#define		_____ROCKCLIENT_MAIN_____

#define		DEV
#include "RockPCH.h"
#include "RockInterface\\Input.h"
#include "RockInterface\\Rui.h"
#include "RockInterface\\FrameWnd.h"
#include "RockInterface\\Define.h"

#include "base\\d3dbase.h"
#include "NetworkMgr.h"
#include "Global.h"

#include "GMCmdDlg.h"
#include "PacketTransLogDlg.h"
#include "bravolibs\effect\CFixSizeBillboard.h"

#include "bravolibs\obj\CClassView.h"
#include "RockInterface\\CLoginShutter.h"
#include "CAction.h"
#include "bravolibs\obj\CFaceExp.h"
#include "CSceneManager.h"
#include "Squence\\CSquenceManager.h"
#include "CWebDialog.h"
#include "bravolibs\obj\WinLock.h"
RB_USING
#include "RockResourceManager.h"
  

#include	"NPGameLib.h"


/*------------------------------------------------------------
- 12/18/2006 이후 부터 이제 여기서 각나별로 컴파일 옵션을 설정 .. 
- 인도네시아는 basedatastructures.h  설정한다
- FrameworkProtocols.h <--- 화일에서 인도네시아 클라이언트 옵션 ON 
- 인도네시아 : 버젼 체크 꼭 해야함 RockClient.cpp 컴파일시 +1 씩증가.
- 각 나라별로 리소스 관리 항상 체크 ... 하자 ..  
//-----------------------------------------------------*/
//--------------------------------------------------------------------------
//#define	GAMETYPE_TEST
//#define	GAMETYPE_REAL


//API 오류 검출 .. 필히 검사 해보자 .. 


///-- 공통 디파인
#define FRONTIER
#define WHISPER_GM
#define REMOVE_PDBFILE			///--  배포된 PDB파일을 지울때 사용
#define NEW_CLIENT_MAPTYPE		///--  새로운 맵 데이터 로딩 - 맵타입 지정가능	

#ifdef TIME_SUIT_UPDATE
	#define C_TIME_SUIT_UPDATE
#endif

#ifdef DROP_WASTEPAYITEM
	#define	C_DROP_WASTEPAYITEM// 드랍이 된다는건 거래가 가능하다
#endif


#ifdef SUBJOB_RENEWAL	
	#define C_SUBJOB_RENEW
	#define	C_SUBJOB_RENEW_SLOTHELPTIP	//슬롯에 헬프팁 나오게 하기 5.22 - DX9 
	#define	C_NPCCHAT_RENEWAL// 서브직업 변경으로 NPC UI 변경
#endif 

#define MEGAPHONE_RENEW			//메가폰  하드코딩 되어있는거 타입으로 돌림 5.22
#define KEEPUP_PAYITEM_RENEWAL		// 유료아이템 30일 사용시 시간표시 리뉴얼 


#ifdef CHANGE_QUEST_CONDITION
	#define QUEST_COUTLIST_RENEWAL		//퀘스트 카운트 4개 에서 8개 확장
#endif 

//#define SERVERDOWN_ATTACK		// 패킷 왕창 보내기

//#define WIN_MINIMIZE_LOOP
//#define AUTO_COMBO //-- 자동콤보 적용루틴
//#define DEFENSE_EFFECT // 방어구
//	#define SINGAPORE_BLOCK_PET	//-펫사용  못하게 
//	#define SINGAPORE_BLOCK_RIDING //-- 라이딩 사용못하게 . 
//#define APPLY_BATTLE_BGSOUND		///-- 전투 배경음 적용


#if defined( INDONESIA_VERSION ) && !defined( INDONESIA )
#	error select 1 define indonesia ON
#elif defined (KOREA_VERSION ) && defined(INDONESIA)
#	error select 1 define Indonesia OFF
#elif defined (JAPAN_VERSION ) && defined(INDONESIA)
#	error select 1 define Indonesia OFF
#elif defined (CHINA_VERSION ) && defined(INDONESIA)
#	error select 1 define Indonesia OFF
#elif defined (GLOBAL_VERSION ) && defined(INDONESIA) 
#	error select 1 define Indonesia OFF
#elif defined (GLOBAL_VERSION ) && !defined(GLOBAL_LOGIN_VERSION)
#	error select 1 define GLOBAL_LOGIN_VERSION ON plz
#elif defined (TAIWAN_VERSION ) && defined(INDONESIA)
#	error select 1 define Indonesia ON
#endif

#if (defined TAIWAN_VERSION  || defined  HONGKONG_VERSION ) && !defined (UNICODE_ROCK)
#	error select 1 define UNICODE_ROCK ON
#endif

 

// 글로벌---------------------------------------------------------------------------------------------------------------
#ifdef GLOBAL_VERSION

	#define ONLY_ENGLISH_SET
	#define	APPLY_GUIDE	
	#define	NOT_TALK					//타국가 는 말을 못하게 한다.. 
	#define C_PAY_SYSTEM				//서버에 디파인이 있지만 클라이언트를 따로쓴다.. 
	#define APPLY_BOLD_FONT				///-- 폰트 볼드체 적용
	#define	LOGINPAGE_BGIMG_CHANGE		//카메라 이동 없이 바로 캐릭터 화면 보임
	#define ROCK_WINDOWS_MODE			//창 풀스크린모드 
	#define SNIPER_MOD_MOTION			//--스나이퍼 모션 줄임
	#define	ADD_MOTION_DANCE
	#define	CLINET_SKILL_LERNCOST		//습득비용 서버에서 사용하는지 확인한다. (서버에서 사용하는지 확인한다. )
	#define	HOLY_SKILLICON_MODIFY		///-- 신수 스킬 툴팁에 표현

	#define	EDITBOX_COMMA				///-- 에디터박스 콤마 표시 

	#define	LAUNCHER_UPGREADE_VERSION	// setup.ini를 etc.rvf 이동시킴.. 
	#define APPLY_ENGLISH_WINDOW		///-- 영문 게임 리뉴얼
	#define APPLY_ENGLISH_ORIGIN_SIZE	///-- 영문게임 -- 글자 원래 크기로 보여주는 루틴 

	#define NEW_EMBLEM_ITEM_APPLY		///--  새로운 엠블렌 적용 방식 - 테이블 사용	
	#define KEEPUP_PAYITEM_RENEWAL

	//#define APPLY_SKILL_VELOCITY		///-- 공속이 스킬 속도에 영향을 미치는지 여부루틴
	//#define APPLY_BATTLE_BGSOUND		///-- 전투 배경음 적용
	//#define TEST_REALEASE_CLIENT
	///#define TEST_GAME_NECK			///-- 게임중 렉현상 테스트

	#define QUEST_RENEWAL				///퀘스트 리뉴얼 .. 
	#define	EXPANSION_QUICK_SLOT		//확장 슬롯 30개늘림. . 
	#define OPTION_RENEWAL				//옵션창 변경 

	#define NPC_MOVE_TO_SKILL				//이동후 스킬
	#define	HELPTIP_DATA_RENEWAL		//기존 txt 파일을 -> 데이타로 변경 매크로값으로 뽑음
	#define	BATTLEROOM_HELPBTN_ERASE	// 대전장에 헬프팁 제거 
	#define	CUR_INDUN_CHECK		     	//대전장 들어갈시  현재 인스턴트 던젼인지 체크한다.. 
	#define KULUBEACH_QUEST_NAME        //쿨루비치 퀘스트 네임 변경 


	#ifdef SEALED_TEMPLE_200811// 봉인의 성지

		#define C_SEALED_SKILL_LEARN		//스킬 습득 추가 확장 12.14
		#define C_SEALED_SKILLPOTION_RENEW	//스킬포션창 리뉴얼 12.14 
		#define C_SEALED_ELDASTONE_DUST		// 엘다스톤 가루 12.14 
		#define	C_SEALED_NPC_CHAT_RENEW		// NPC 대화창 리뉴얼 12.14 
		#define C_SEALED_STORE				// 봉인의 성지 관련 상점창 수정	12.14 
		#define NPC_CHAT_EXTENSION		// NPC 스크립트 대화 스트링길이 2048 늘림

	#else 

		//글로벌은 봉인의 성지가 아직 안들어 갔기 땜시 .. 
		//서브직 리뉴얼관려으로 이 디파인이 들어간다 . 
		#define NPC_CHAT_EXTENSION		// NPC 스크립트 대화 스트링길이 2048 늘림
		#define C_SEALED_NPC_CHAT_RENEW

	#endif// #ifdef SEALED_TEMPLE_200811// 봉인의 성지 

	#ifdef SELL_ITEMSHOP
		#define	CSELL_ITEMSHOP				// 상점에서 아이템 팔 때 20%으로 수 (서버에서 사용하는지 확인한다. )
	#endif

#endif// #ifdef GLOBAL_VERSION
// 글로벌---------------------------------------------------------------------------------------------------------------



// 타이완---------------------------------------------------------------------------------------------------------------
#ifdef TAIWAN_VERSION
	
	#define KULUBEACH_QUEST_NAME        //쿨루비치 퀘스트 네임 변경
	#define GANJA_WORD_BLOCK_20090630	// 캐릭터 생성시 이름이 간자체인경우 리턴. 번체만 사용
	#define	APP_IME_BLOCK_CHS			// 중국어 간체 막기 .. 

	#define SERVERLIST_TEXT_NON_PVP		//대만 적용함 NONPVP 서버리스트에 출력 5.22 - DX9
	#define	LAUNCHER_UPGREADE_VERSION	// setup.ini를 etc.rvf 이동시킴..	
	#define EXPANSION_SERVERTAB			//서버리스트 확장
	#define	APPLY_GUIDE	
//	#define	NOT_TALK					//타국가 는 말을 못하게 한다.. 
	#define C_PAY_SYSTEM			//서버에 디파인이 있지만 클라이언트를 따로쓴다.. 
	#define APPLY_BOLD_FONT				///-- 폰트 볼드체 적용
	//#define	LOGINPAGE_BGIMG_CHANGE		//카메라 이동 없이 바로 캐릭터 화면 보임
	#define ROCK_WINDOWS_MODE			//창 풀스크린모드 
	#define SNIPER_MOD_MOTION			//--스나이퍼 모션 줄임
	#define	ADD_MOTION_DANCE
	#define	CLINET_SKILL_LERNCOST		//습득비용 서버에서 사용하는지 확인한다. (서버에서 사용하는지 확인한다. )
	#define	HOLY_SKILLICON_MODIFY		///-- 신수 스킬 툴팁에 표현

	///--#define APPLY_SKILL_VELOCITY
	#define QUEST_RENEWAL				///퀘스트 리뉴얼 .. 
	#define	EXPANSION_QUICK_SLOT		//확장 슬롯 30개늘림. . 
	#define OPTION_RENEWAL				//옵션창 변경 

	#define NEW_EMBLEM_ITEM_APPLY		///--  새로운 엠블렌 적용 방식 - 테이블 사용
	
	//#define APPLY_BATTLE_BGSOUND		///-- 전투 배경음 적용

	#ifdef SELL_ITEMSHOP
		#define	CSELL_ITEMSHOP			// 상점에서 아이템 팔 때 20%으로 수 (서버에서 사용하는지 확인한다. )
	#endif

	#define NPC_MOVE_TO_SKILL				//이동후 스킬
	#define	HELPTIP_DATA_RENEWAL		//기존 txt 파일을 -> 데이타로 변경 매크로값으로 뽑음
	#define	BATTLEROOM_HELPBTN_ERASE	// 대전장에 헬프팁 제거 
	
//	#define NEXT_SELECT_TO_TAB

	#define	CUR_INDUN_CHECK			//대전장 들어갈시  현재 인스턴트 던젼인지 체크한다.. 
	
	

	#ifdef SEALED_TEMPLE_200811// 봉인의 성지

		#define C_SEALED_SKILL_LEARN		//스킬 습득 추가 확장 12.14
		#define C_SEALED_SKILLPOTION_RENEW	//스킬포션창 리뉴얼 12.14 
		#define C_SEALED_ELDASTONE_DUST		// 엘다스톤 가루 12.14 
		#define	C_SEALED_NPC_CHAT_RENEW		// NPC 대화창 리뉴얼 12.14 
		#define C_SEALED_STORE				// 봉인의 성지 관련 상점창 수정	12.14 
		#define NPC_CHAT_EXTENSION

	#endif 

	//  #define ITEMLIMITFUNC_NOTQUICKSLOT	// 퀵슬롯에 올라가는 아이템 막기.   by.jjplusi011		12.30
	//	#define FIRSTLOADING_IMAGEFIXING	// 첫 로딩시 이미지 고정하기.	by.jjplusi011	09.01.06

	#define HOLYLAND_OF_SEAL_LOADING_IMAGE	//봉인의성지 맵들간의 로딩이미지	by.jjplusi011 09.01.12
	
	#ifndef _DEBUG
		#define	C_HACK_SHIELD			//--핵쉴드 
	#endif	

#endif// #ifdef TAIWAN_VERSION
// 타이완---------------------------------------------------------------------------------------------------------------



// 홍콩-----------------------------------------------------------------------------------------------------------------
#ifdef HONGKONG_VERSION
	
	//#define GANJA_WORD_BLOCK_20090630	// 캐릭터 생성시 이름이 간자체인경우 리턴. 번체만 사용
	//#define	APP_IME_BLOCK_CHS			// 중국어 간체 막기 .. 

	#define	LAUNCHER_UPGREADE_VERSION	// setup.ini를 etc.rvf 이동시킴.. 

	#define KULUBEACH_QUEST_NAME        //쿨루비치 퀘스트 네임 변경
	#define	APPLY_GUIDE	
	#define	NOT_TALK					//타국가 는 말을 못하게 한다.. 
	#define C_PAY_SYSTEM			//서버에 디파인이 있지만 클라이언트를 따로쓴다.. 
	#define APPLY_BOLD_FONT				///-- 폰트 볼드체 적용
	//#define	LOGINPAGE_BGIMG_CHANGE		//카메라 이동 없이 바로 캐릭터 화면 보임
	#define ROCK_WINDOWS_MODE			//창 풀스크린모드 
	#define SNIPER_MOD_MOTION			//--스나이퍼 모션 줄임
	#define	ADD_MOTION_DANCE
	#define	CLINET_SKILL_LERNCOST		//습득비용 서버에서 사용하는지 확인한다. (서버에서 사용하는지 확인한다. )
	#define	HOLY_SKILLICON_MODIFY		///-- 신수 스킬 툴팁에 표현


	//#define	EDITBOX_COMMA			///-- 에디터박스 콤마 표시 

	///--#define APPLY_SKILL_VELOCITY
	#define QUEST_RENEWAL				///퀘스트 리뉴얼 .. 
	#define	EXPANSION_QUICK_SLOT		//확장 슬롯 30개늘림. . 
	#define OPTION_RENEWAL				//옵션창 변경 

	#define NEW_EMBLEM_ITEM_APPLY		///--  새로운 엠블렌 적용 방식 - 테이블 사용


	#define	HELPTIP_DATA_RENEWAL		//기존 txt 파일을 -> 데이타로 변경 매크로값으로 뽑음
	#define	BATTLEROOM_HELPBTN_ERASE	// 대전장에 헬프팁 제거 
	
//  #define ITEMLIMITFUNC_NOTQUICKSLOT	// 퀵슬롯에 올라가는 아이템 막기.   by.jjplusi011		12.30
//	#define FIRSTLOADING_IMAGEFIXING	// 첫 로딩시 이미지 고정하기.	by.jjplusi011	09.01.06

	#define HOLYLAND_OF_SEAL_LOADING_IMAGE	//봉인의성지 맵들간의 로딩이미지	by.jjplusi011 09.01.12
	#define NPC_MOVE_TO_SKILL				//이동후 스킬
	#define	CUR_INDUN_CHECK			//대전장 들어갈시  현재 인스턴트 던젼인지 체크한다.. 
	
	#define SERVERLIST_TEXT_NON_PVP	//대만 적용함 NONPVP 서버리스트에 출력 5.22 - DX9

	#ifdef SEALED_TEMPLE_200811// 봉인의 성지

		#define C_SEALED_SKILL_LEARN		//스킬 습득 추가 확장 12.14
		#define C_SEALED_SKILLPOTION_RENEW	//스킬포션창 리뉴얼 12.14 
		#define C_SEALED_ELDASTONE_DUST		// 엘다스톤 가루 12.14 
		#define	C_SEALED_NPC_CHAT_RENEW		// NPC 대화창 리뉴얼 12.14 
		#define C_SEALED_STORE				// 봉인의 성지 관련 상점창 수정	12.14 
		#define NPC_CHAT_EXTENSION

	#endif// #ifdef SEALED_TEMPLE_200811// 봉인의 성지

	#define EXPANSION_SERVERTAB			//서버리스트 확장
	
	#ifndef _DEBUG	
		#define C_GAME_GUARD				//엔프로텍트
	#endif 

	#ifdef SELL_ITEMSHOP
		#define	CSELL_ITEMSHOP			// 상점에서 아이템 팔 때 20%으로 수 (서버에서 사용하는지 확인한다. )
	#endif

#endif// #ifdef HONGKONG_VERSION
// 홍콩-----------------------------------------------------------------------------------------------------------------



// 대한민국-------------------------------------------------------------------------------------------------------------
#ifdef KOREA_VERSION	

//	#define TEST_INSTALL_VERSION		///테스트판으로 나갈시 .. 꼭필요 . 

	#ifdef SKILL_LEARNCOST

		#define	CLINET_SKILL_LERNCOST		//습득비용 서버에서 사용하는지 확인한다. (서버에서 사용하는지 확인한다. )

	#endif// #ifdef SKILL_LEARNCOST

	//#define KULUBEACH_QUEST_NAME      //쿨루비치 퀘스트 네임변경
	#define	APPLY_GUIDE					///-- 모험백서
	#define	NOT_TALK					//타국가 는 말을 못하게 한다.. 
	#define C_PAY_SYSTEM				//서버에 디파인이 있지만 클라이언트를 따로쓴다.. 
	#define APPLY_BOLD_FONT				///-- 폰트 볼드체 적용
	#define	LOGINPAGE_BGIMG_CHANGE		//카메라 이동 없이 바로 캐릭터 화면 보임
	#define ROCK_WINDOWS_MODE			//창 풀스크린모드 
//	#define CREATE_CHRACTER_BTN			/// - 생성버튼 비활성화 .

	#define	HOLY_SKILLICON_MODIFY		///-- 신수 스킬 툴팁에 표현
	#define SNIPER_MOD_MOTION			//--스나이퍼 모션 줄임
	#define	ADD_MOTION_DANCE

	#define	EDITBOX_COMMA				///-- 에디터박스 콤마 표시 
	#define	APPLY_INTRO_ANI				///-- 인트로 애니 적용여부
	#define NEW_EMBLEM_ITEM_APPLY		///--  새로운 엠블렌 적용 방식 - 테이블 사용
	#define KEEPUP_PAYITEM_RENEWAL

	///--#define APPLY_SKILL_VELOCITY
	#define QUEST_RENEWAL				///퀘스트 리뉴얼 .. 
	#define	EXPANSION_QUICK_SLOT		//확장 슬롯 30개늘림. . 
	#define OPTION_RENEWAL				//옵션창 변경 

	#define NPC_MOVE_TO_SKILL				//이동후 스킬
//	#define NEXT_SELECT_TO_TAB
	#define	BATTLEROOM_HELPBTN_ERASE	// 대전장에 헬프팁 제거 
	#define	CUR_INDUN_CHECK			//대전장 들어갈시  현재 인스턴트 던젼인지 체크한다.. 

	#ifdef SEALED_TEMPLE_200811// 봉인의 성지

		#define C_SEALED_SKILL_LEARN		//스킬 습득 추가 확장 12.14
		#define C_SEALED_SKILLPOTION_RENEW	//스킬포션창 리뉴얼 12.14 
		#define C_SEALED_ELDASTONE_DUST		// 엘다스톤 가루 12.14 
		#define	C_SEALED_NPC_CHAT_RENEW		// NPC 대화창 리뉴얼 12.14 
		#define C_SEALED_STORE				// 봉인의 성지 관련 상점창 수정	12.14 
		#define NPC_CHAT_EXTENSION		// NPC 스크립트 대화 스트링길이 2048 늘림

	#endif// #ifdef SEALED_TEMPLE_200811// 봉인의 성지
	
	#define	HELPTIP_DATA_RENEWAL		//기존 txt 파일을 -> 데이타로 변경 매크로값으로 뽑음

//  #define ITEMLIMITFUNC_NOTQUICKSLOT	// 퀵슬롯에 올라가는 아이템 막기.   by.jjplusi011		12.30
//	#define FIRSTLOADING_IMAGEFIXING	// 첫 로딩시 이미지 고정하기.	by.jjplusi011	09.01.06

	#define HOLYLAND_OF_SEAL_LOADING_IMAGE	//봉인의성지 맵들간의 로딩이미지	by.jjplusi011 09.01.12

	//#define APPLY_BATTLE_BGSOUND		///-- 전투 배경음 적용

	#define UPGRADE_WING_HELMET

	#ifndef _DEBUG

		//#define	C_HACK_SHIELD			//--핵쉴드 
		
		#ifdef TEST_INSTALL_VERSION
			#define	LAUNCHER_UPGREADE_VERSION	//etc에서 서버IP 받아옴 
		#endif	//TEST_INSTALL_VERSION

	#endif	//_DEBUG

#endif// #ifdef KOREA_VERSION
// 대한민국-------------------------------------------------------------------------------------------------------------


// 일본-----------------------------------------------------------------------------------------------------------------
#ifdef JAPAN_VERSION

//	#define	NOT_TALK	//타국가 는 말을 못하게 한다..        

	//테섭 임시로 ... 유료아이템 안보이게 처리한다.
	#define KULUBEACH_QUEST_NAME        //쿨루비치 퀘스트 네임 변경
	#define	APPLY_GUIDE	
	#define C_PAY_SYSTEM				//서버에 디파인이 있지만 클라이언트를 따로쓴다.. 
	#define JAPAN_HPZERO_BUG_MODIFY		///-- 일본 몹 0버그 찾기 위해 만든부분
	#define REAPPEAR_DIFFICULT_BUG_LOG  ///-- 재현 어려운 버그 로깅부분
	#define SNIPER_MOD_MOTION			//--스나이퍼 모션 줄임
	#define	ADD_MOTION_DANCE
	#define	APPLY_INTRO_ANI				//-- 인트로 애니 적용여부
	#define	HOLY_SKILLICON_MODIFY			///-- 신수 스킬 툴팁에 표현	


	#define NEW_EMBLEM_ITEM_APPLY		///--  새로운 엠블렌 적용 방식 - 테이블 사용
	#define KEEPUP_PAYITEM_RENEWAL
	
	#define ROCK_WINDOWS_MODE			///-- 창 풀스크린모드 
	#define SAVE_PINFO_LOG_BUTTON		///-- 디버그 모드(GM)에서 패킷관련정보를 기록한다.

	#define	BATTLEMAP_OPTIMIZE			// 61,62 맵 들어갈때 최적화 한다. 

	//#define TEST_NO_HACK_SHIELD			///-- 핵쉴드 없이 클라이언트 테스트

	#define QUEST_RENEWAL				///퀘스트 리뉴얼 .. 
	#define	EXPANSION_QUICK_SLOT		//확장 슬롯 30개늘림. . 
	#define OPTION_RENEWAL				//옵션창 변경 
	#define	BATTLEROOM_HELPBTN_ERASE	// 대전장에 헬프팁 제거 
	#define	CUR_INDUN_CHECK			//대전장 들어갈시  현재 인스턴트 던젼인지 체크한다.. 

	#ifdef SEALED_TEMPLE_200811

		#define C_SEALED_SKILL_LEARN		//스킬 습득 추가 확장 12.14
		#define C_SEALED_SKILLPOTION_RENEW	//스킬포션창 리뉴얼 12.14 
		#define C_SEALED_ELDASTONE_DUST		// 엘다스톤 가루 12.14 
		#define	C_SEALED_NPC_CHAT_RENEW		// NPC 대화창 리뉴얼 12.14 
		#define C_SEALED_STORE				// 봉인의 성지 관련 상점창 수정	12.14 
		#define NPC_CHAT_EXTENSION		// NPC 스크립트 대화 스트링길이 2048 늘림

	#endif 

	//  #define ITEMLIMITFUNC_NOTQUICKSLOT	// 퀵슬롯에 올라가는 아이템 막기.   by.jjplusi011

	#define NPC_MOVE_TO_SKILL				//이동후 스킬
 
	#ifdef SKILL_LEARNCOST
		#define	CLINET_SKILL_LERNCOST		//습득비용 서버에서 사용하는지 확인한다. (서버에서 사용하는지 확인한다. )
	#endif 	

	#ifndef _DEBUG
		#define	C_HACK_SHIELD
	#endif

#endif// #ifdef JAPAN_VERSION
// 일본-----------------------------------------------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////
#ifdef INDONESIA_VERSION			
//////////////////////////////////////////////////////////////////////////
#define ONLY_ENGLISH_SET
#define	NOT_TALK					//타국가 는 말을 못하게 한다.. 	


#define	LOGINPAGE_BGIMG_CHANGE		//카메라 이동 없이 바로 캐릭터 화면 보임 

//테섭 임시로 ... 유료아이템 안보이게 처리한다.
#define	APPLY_GUIDE	
#define C_PAY_SYSTEM				//서버에 디파인이 있지만 클라이언트를 따로쓴다.. 
#define SNIPER_MOD_MOTION			//--스나이퍼 모션 줄임
#define	ADD_MOTION_DANCE
#define	APPLY_INTRO_ANI				//-- 인트로 애니 적용여부
#define	HOLY_SKILLICON_MODIFY		///-- 신수 스킬 툴팁에 표현	

#endif


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef CHINA_VERSION
//////////////////////////////////////////////////////////////////////////
	#define	APPLY_GUIDE	
	#define	NOT_TALK					//타국가 는 말을 못하게 한다.. 
	#define C_PAY_SYSTEM				//서버에 디파인이 있지만 클라이언트를 따로쓴다.. 
	#define EXPANSION_SERVERTAB			//서버리스트 확장(중국만 적용)
	#define ROCK_WINDOWS_MODE			//창 풀스크린모드 
	#define	LAUNCHER_UPGREADE_VERSION	// setup.ini를 etc.rvf 이동시킴.. 
	#define	CLINET_SKILL_LERNCOST		//습득비용 서버에서 사용하는지 확인한다. (서버에서 사용하는지 확인한다. )
	#define	CSELL_ITEMSHOP				// 상점에서 아이템 팔 때 20%으로 수 (서버에서 사용하는지 확인한다. )
	//#define C_KEEPING_CHARGE			//창고 이용시 수수료 (서버에서 사용하는지 확인한다. )
	#define SNIPER_MOD_MOTION	
	//	#define	ADD_MOTION_DANCE
	#define	HOLY_SKILLICON_MODIFY		///-- 신수 스킬 툴팁에 표현

	#define	EDITBOX_COMMA				///-- 에디터박스 콤마 표시 
	#define NEW_EMBLEM_ITEM_APPLY		///--  새로운 엠블렌 적용 방식 - 테이블 사용

	#ifdef PK_SYSTEM
	#define CPK_SYSTEM
	#endif

	#ifndef _DEBUG
	#define	C_HACK_SHIELD
	#endif

#endif




extern void All_New_g_Variable_Ptr();
extern void All_Delete_g_Variable_Ptr();

#define SEND_DEBUG_LOG g_RockClient.SendDebugLog

enum	BRAVOCURSORTYPE
{
	BC_NORMAL	= 0,
	BC_ATTACKENABLE,
	BC_MOVEDISABLE,
	BC_EVENT,
	BC_ITEM,
	BC_HORSE,
	BC_PERSON,
	BC_CAM,
};

#define	MAX_CURSOR		3
#define	SIZE_CURSORTEX	64
#define	SCALE_CURSOR	2.0f
#define	MAXFRAME_CURSOR	1
 


#define	NETWORK_CLIENT		/// 요 정의가 있으면 서버에 접속. 없으면 로컬에서 싱글모드로만 진행 

enum	NLoginStep
{
	nNotConnect,			/// 초기 화면 id와 passwd를 입력받으면 connect 후 account login 전송(하기 직전까지 상태) 
	nError,
	nAccountLogin,			/// 서버에 Account Login 을 전송하고 기다리는 상태 
	nServerSelectWait,		/// Login 인증 성공후, 서버로부터 ServerSet List를 전송 받은 상태. 사용자가 서버를 선택하기를 대기 
	nServerSelectOK,		/// 사용자가 서버 선택 OK. 해당 게임 서버에 접속 
	nCharacterSelectWait,	/// 게임서버에서 캐릭터 리스트를 전송 받은 상태. 사용자가 캐릭터 선택하도록 대기 
	nCharacterSelectOK,		/// 캐릭터 선택 후, 게임 서버에 캐릭터 선택 프로토콜 전송 
	nAllPrepareWait,		/// 서버에서 최종 로그인 준비 완료되었음을 알려줌. 
	nAllPrepareOK,			/// 서버에 클라이언트도 최종 로그인 준비 완료되었음을 전송 
	nInGamePlay				/// 게임 월드에 진입 
};

//-----------------------------------------------------------------------------
//-- 몬스터 Chatting Type
enum NMonsterChatType
{
	n_Monster_Attack,		///-- 몬스터가 공격할 때의 메세지		
	n_Monster_Attacked,		///-- 몬스터가 공격당할 때의 메세지
	n_Monster_Wait,			///-- 몬스터가 대기할 때의 메세지
	n_Monster_Move,			///-- 몬스터가 움직일 때의 메세지
	n_Monster_Rengen,		///-- 몬스터가 리젠 될 때의 메세지
	n_Monster_Skill,		///-- 몬스터가 스킬 사용할 때의 메세지
	n_Monster_Die			///-- 몬스터가 죽을 때의 메세지
};

#define MAX_MAP_NUM 100 

typedef enum NActionSystemGageType
{
	n_NoneGage = 0,
	n_ComboGage,
	n_ConditionGage,
	n_ProimaGage

} NActionSystemGageType;


typedef enum NMouseControlType
{
	n_MouseType_Normal = 0,
	n_MouseType_UseSkill

} NMouseControlType;

const int c_PickSameAttack_DelayTime( 1500 );
const int c_PickOtherAttack_DelayTime( 200 );

const int c_GameHackWaitTime( 180000 );	
const int c_NotRepairitem_Start( 12301 );
struct SGameTypeInfo
{
	NFrom theFrom;
	char theUserID[128];
	char theUserPass[128];

	SGameTypeInfo()
	{
		theFrom = n_Normal;
		memset(theUserID , NULL , 128 );
		memset(theUserPass , NULL , 128);
	}
};

struct SPacketInfoPerSec
{
	BOOL   IsSavePacketInfoPerSec;
	DWORD  dPrePInfoSaveTime;
	int    nRecvPacCount;
	DWORD  dRecvPacSize;
	int    nSendPacCount;
	DWORD  dSendPacSize;

	SPacketInfoPerSec()
	{
		memset( this, 0, sizeof(SPacketInfoPerSec) );
	}
};



typedef struct _sNpcIndexlist
{
	int		index; 
	float	Distance;
	int		type;
	_sNpcIndexlist()
	{
		index = 0 ;
		Distance =0.0f;
		type = 0; 
	};

}sNpcIndexlist;


extern void Drv_SetGamma(HDC hDC, float gamma, int overbright, float contrast);
	   
class		CRockClient		: public CD3DApplication, public CNetworkMgr
{	
/****************************************************************************************************
	* Major
****************************************************************************************************/
public:
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DDEVICE9		m_lpD3DDevice;	
	LPDIRECT3D9				m_lpD3D;
#else
	LPDIRECT3DDEVICE8		m_lpD3DDevice;	
	LPDIRECT3D8				m_lpD3D;
#endif // DIRECT_VERSION_9_MJH
	
	bool					m_bConnect;
protected:
	DWORD			m_dwGameMode;
	int				ml_language;	///-- 0 : Korean / 1 : Japan / 2 : Reserved / 3 : Reserved
	char 			m_szRootDir[MAX_PATH];// 생성자에서 설정
	wchar_t			m_szRootDirW[MAX_PATH];
	bool			m_bSaveInfo;

	char			theCurrentMapPath[256];

	int			m_IsClickScreen;	//0 버튼입력을 받는다 1:버튼을 눌렀다 2 사진을 찍고 있다. 
	
public:
	CRockClient();
	~CRockClient();
//Hack Shield
public:
	
	BOOL HS_Init();// 핵쉴드 초기화
	BOOL HS_StartService();
	BOOL HS_StopService();
	BOOL HS_UnInit();
	bool HackClose(TCHAR *szMsg ,int flag = 0);
	void HS_PauseService();
	void HS_ResumeService();
	BOOL HS_ShadowUserCreate();
	BOOL HS_FolderPermission();
	
//nProtect 
	CNPGameLib	*pNpgl;
	GG_AUTH_DATA m_AuthData;
	BOOL NP_Init();
	void NP_Send(char *UserID);

	virtual	int	 GetGameVersion();

//	int __stdcall AhnHS_Callback(long lCode, long lParamSize, void* pParam);
public: 
    HRESULT DeleteDeviceObjects();  //by simwoosung - 디바이스에 연결된 객체 해제
	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();	
	
public:
	

	HWND			GetApp()							{ return m_hWnd; }
#ifdef DIRECT_VERSION_9_MJH
	D3DCAPS9 *		GetCaps()							{ return &m_d3dCaps; }	
#else
	D3DCAPS8 *		GetCaps()							{ return &m_d3dCaps; }	
#endif // DIRECT_VERSION_9_MJH
	inline DWORD	GetGameMode()						{ return m_dwGameMode; }
	inline void		SetGameMode(DWORD mode)				{ m_dwGameMode = mode; }
	char*			GetRootDir()						{ return m_szRootDir; }
	wchar_t*		GetRootDirW()						{ return m_szRootDirW; }
	int				GetDisplaySize()					{ return( ( m_dwCreationWidth > 800 )? 2:0 );	}

	// 0 : eLang_Korea, 1: eLang_Japan, 2 : eLang_China, 3 : eLang_Taiwan
	int				GetLanguage() const { return ml_language; };		

	void			SetCurrentMapPath( char* MapPath )	{ strcpy( theCurrentMapPath, MapPath ); }/// ....sh
	char*			GetCurrentMapPath()					{ return theCurrentMapPath; }

	bool			MapCreate(DWORD idx);	
	bool			MapCreate(DWORD idx,int x,int y);

	void			MapClear(DWORD idx);
	bool 			MapChange(DWORD idx);
	int				GetLoadngCount()					{ return m_nLoadingCount; }
	void			SetLoadngCount(DWORD count)			{ m_nLoadingCount = count; }
	int				m_nLoadingCount;
	void			DisplayCursor();
	FLOAT			GetFrame()							{ return m_fFPS; }
	void			SetEWindowsMode(UINT EWindowsMode) { m_EWindowsMode = EWindowsMode; }
	UINT			GetEWindowsMode()					{ return m_EWindowsMode; } 			
	void			OnCrash();	/// 자동 버그 리포트 by sooree
	
	void			BeginLoadingBarThread();
	void			LoadingBarSafeStopAfterProcess();

	CFixSizeBillboard m_FixSizeBillboard;				// 사이즈 고정되있는 빌보드
	SGameTypeInfo	  m_GameTypeInfo;					// 게임 타입인포	

	void	ResetNowTextureDetail();
	BOOL			LoadSetup();
	BOOL			InitLanguege();
	void			DeleteEmblemFile();					//Only China 
	

public:

	CSceneManager	m_SceneManager;						// 전체 화면 출력 설정 관련

	bool			m_bCursorItem;
	int				m_CursorIndex;
	int				m_CursorUnique;
	void			SetCursor();
	void			InitCursor() { m_bCursorItem = false; m_CursorIndex = -1; m_CursorUnique = -1; }

public:	
	bool			m_bGameEnd;
	void			InitLoading();
	bool			SelectLoading();
	void			LogoDisplay(int nAlpha);

	///----------------------------------------------
	/// sooree network
	///----------------------------------------------
	char			m_UserName[ 1024 ];
	char			m_UserSysID[64];

	char			m_UserPWD [ 1024 ];
	char			m_SysID[33];

	BOOL			m_IsReConnect;
	BOOL			m_IsMapLoding;									// 지속 스킬 땜시 캐릭터 자신 이펙트지우는것 체크

	BOOL			LoginProc( char* aUserName, char* aPassWord, UINT uForm = n_Normal );// Login Packet Send
	BOOL			ReLoginProc(void);

	
	BOOL				theUserSelectExit;

	NLoginStep			theLoginStep;
	Void		SetLoginStep( NLoginStep aLoginStep );
	inline	NLoginStep	GetLoginStep()							{	return( theLoginStep );	}

	LRESULT				UserInputProc();

protected:
	HRESULT	InitState(); // Create() 호출. 여기서 CRui* 생성
	HRESULT FrameMove()			{ if( m_bGameEnd == false ) return Main();		return E_FAIL;	}
	HRESULT Render()			{ if( m_bGameEnd == false ) return Display();	return E_FAIL;	}
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	HRESULT			GetEnvironment(){ return Render3DEnvironment();}
	HRESULT			Main();
	HRESULT			Display();
//뷰테스트

	CGMCmdDlg				theGMCmdDlg;
	CPacketTransLogDlg      thePacketTransLogDlg;
	
	BOOL					m_IsLoadProcessAfter;
	DWORD					m_InitTime;

	//-------------------------------------------------------------------------
	int				MouseProcess();///....sh
	int				KeyProcess();///....sh
	void			CheckWarpzone();///....sh
	bool			UserKeyProcess();//유져가 사용하는키 
	bool			TestKeyProcess();//테스트 할때사용하는키 
	void			SetCharacterInfo();	

	void			SetPlayerMessage( TCHAR* str );					//-- 플레이어의 대화창 생성
	void			SetNonPlayerMessage( DWORD pcid, TCHAR* str );	//-- NPC 대화창 생성( 타 유저 ) 
	void			SetMonsterMessage( DWORD pcid, NMonsterChatType atype , int RandomRatio = 0 );	//-- Monster 대화창 생성 RandomRatio 1~10 까지지정

	
	int				PickPc();
	int				GetPickPcIdx();
	SPcInfo*		GetPickPcInfo();
	SPcInfo*		FindNPCPcInfo(long unique);		
	
	int				m_PickPcIdx;
	int				m_PickIndex;

	SPcInfo			m_PcInfo[MAX_CHARACTER];//
	
#ifdef ADD_APPLYWIDE_MAP
	std::vector <SPcInfo > m_PcIndexList;
#endif //ADD_APPLYWIDE_MAP
	
	SPcDataParam*	m_PcParam;
	SPcDataInven*	m_PcInven;
	UCHAR uHackMsg[128];

	bool			m_MapChangeProgress;
	
	WNDID			m_WndID;//현재...포커스를 가지고 있는 창의 ID
	CFrameWnd*		m_WndHandle;
	//-------------------------------------------------------------------------
	int				m_bTakeoff; // 날개와 관련하여 추가함

///	LPD3DXSPRITE	m_pSprCursor;
	RECT			m_rcCursor;
	BYTE			m_byCurCursor;
	BYTE			m_byCurPush;
	BYTE			m_byCurAni;
	BYTE			m_byCurFrame;

	// cursor direction - 0 - normal 1 - npc 2 - pc
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	m_pTextureCursor;//[MAX_CURSOR];
#else
	LPDIRECT3DTEXTURE8	m_pTextureCursor;//[MAX_CURSOR];
#endif // DIRECT_VERSION_9_MJH


	CRITICAL_SECTION	m_csFlag;
	float				m_fPrevTime;

	unsigned long		GetDisplayWidth()	{ return m_dwCreationWidth; }
	unsigned long		GetDisplayHeight()	{ return m_dwCreationHeight; }
#ifdef DIRECT_VERSION_9_MJH
	HRESULT				ConfirmDevice(D3DCAPS9*,DWORD,D3DFORMAT);
#else
	HRESULT				ConfirmDevice(D3DCAPS8*,DWORD,D3DFORMAT);
#endif // DIRECT_VERSION_9_MJH
	long				m_PickUnique;		// 피킹 되고 있는 유니크 번호 by wxywxy	

public:

	/// sooree
#ifdef DIRECT_VERSION_9_MJH
	HRESULT ScreenGrab(LPDIRECT3DDEVICE9 pDev, char *fileName , char *OutPutFileName );// 스크린샷
#else
	HRESULT ScreenGrab(LPDIRECT3DDEVICE8 pDev, char *fileName , char *OutPutFileName );// 스크린샷
#endif // DIRECT_VERSION_9_MJH
	HRESULT ScreenCapture();

	SKeys		theKey[256];
	SMouse		theMouse;

	CSlangFilter	m_SlangFilter;
	
	//..........................................................................................................
	// 사교동작 관련
	//..........................................................................................................
	CAction			m_Action;
	int				SetAction(Character *pSrc,int MotionIndex,BOOL ISKeeping);
	//..........................................................................................................
	
	// 로그인 관련
	CClassView		m_ClassView;
	CLoginShutter	m_LoginShutter;

	CSquenceManager	m_SquenceManager;
	
	//------------------------------------------------------------------------------------------------
	// 라이딩 관련
	//------------------------------------------------------------------------------------------------
	void			SetRideOn(int nPcIndex);
	void			SetRideOff(int nPcIndex);
	BOOL			CanRideOnDest();

	//비디오 메모리 .. 
	void CheckFreeVideoMemory();
	void CheckUseVideoMemory();
	
	//아이템 생성 
	void CreateItemPcItem(SPcItem** tmpPcItem , URpdItem URpdItemInfo);

	//핵쉴드 
	DWORD m_dwNewTimeHack;
	BOOL	m_bHackStart;

	//setup.ini파일 읽어드릴 변수 
	int m_iFileDataCount;
	ReadFileData m_FileDataInfo[128];
	BOOL GetRockProfileString(IN LPCSTR lpAppName , IN LPCSTR lpKeyName, IN LPCSTR lpDefault, OUT LPSTR lpReturnedString, IN DWORD nSize = NULL, IN LPCSTR lpFileName = NULL);
	int GetRockProfileInt( IN LPCSTR lpAppName, IN LPCSTR lpKeyName, IN INT nDefault , IN LPCSTR lpFileName =NULL );

	bool IsHolySkill(const int Code);
	
	BOOL CanAttackPlayer( Character * pCharacter );
	
public:

	CFaceExp		m_FaceExp;

public:

	//..........................................................................................................
	// 화면 처리 루틴
	//..........................................................................................................
	
	HRESULT	LoginStepProcess(void);									// 로그인 처리 루틴
	HRESULT	CharacterSelectStepProcess(void);						// 캐릭터 선택 처리 루틴
	
	//..........................................................................................................
	// 화면 표시 루틴
	//..........................................................................................................

	void LoginPageDisplay(void);									// 로그인 화면 표시
	void CharacCreatePageDisplay(void);								// 캐릭터 생성 화면 표시
	void CharacSelectPageDisplay(void);								// 캐릭터 선택 화면 표시

public:

	int				GetServSetCount(void)			{ return theServerSetNum; }
	int				GetAreaInfoNum(void)			{ return theAreaInfoNum; }
	int				SetAreaInfoNum(int Area)		{ theAreaInfoNum = Area; }

	//int				GetWorldCount(void)			{ return theWorldNum; }
	//int				GetChannelCount(void)			{ return theChannelNum; }
	int				GetCharacterCount(void)			{ return theCharacterNum; }

	SServerSetData			  *GetServerList(void)			{ return theServerSetData; }
	///SWorldData			*GetWorldList(void)			{ return theWorldData; }
	///SChannelData		*GetChannelList(void)		{ return theChannelData; }


	SRpdCharacterDataWithCore *GetCharacterList(void)		{ return theCharacterList; }

public:
	
	void				CloseLoginState(void);		// 로그인 소켓 닫고 로그인 상태로 돌리기
	void				LogOff(void);				// 로그 오프
	void				SelecttoServerList(void);	// 캐릭터선택에서 서버리스트로 전환

	//........................................................................................................
	// 임시 스킬 테스트 ( CMD -> /생성이펙트 스킬코드 데미지맞는NPC개수)
	//........................................................................................................
	void CreateExampleEffect(DWORD Code,int Num);
	
protected:	
	
	//by simwoosung 프로토콜 체크
	void    CheckProtocol(void * aMsg, BOOL IsRecv);
	
	///----------------------------------------------------------------------------------------
	/// sooree network
	///----------------------------------------------------------------------------------------

	virtual	void	Proc_OnSocketClose();
	virtual	void	Proc_RpScProtocols( void* aMsg );	/// game protocols procedure
	//--------------------------------------------------
	// Framework Related
	//--------------------------------------------------
	virtual	void	Proc_RpScErrorFramework( SRpScErrorFramework* aMsg );
	virtual void	Proc_RpScServerSetList( SRpScServerSetList* aMsg );
	virtual	void	Proc_RpScCharacterList( SRpScCharacterList* aMsg );
	virtual	bool 	Proc_RpScPrepareLogin( SRpScPrepareLogin* aMsg );
	virtual	void	Proc_RpScPCLogin( SRpScPCLogin* aMsg );// 게임서버 접속시 선택한 캐릭터의 정보(속성, 스탯, 아이템 등)
	virtual	void	Proc_RpScCreateCharacter(SRpScCreateCharacter* aMsg);
	virtual	void	Proc_RpScDeleteCharacter(SRpScDeleteCharacter* aMsg);

	

	
	//--------------------------------------------------
	// Game Protocols
	//--------------------------------------------------
	void	Proc_RpScAppearPC( SRpScAppearPC* aMsg );
	void	Proc_RpScDisappearPC( SRpScDisappearPC* aMsg );
	void	Proc_RpScStartMove( SRpScStartMove* aMsg );
	void	Proc_RpScSeeStartMove( SRpScSeeStartMove* aMsg );
	void	Proc_RpScStop( SRpScStop* aMsg );
	void	Proc_RpScAppearNPC( SRpScAppearNPC* aMsg );			
	void	Proc_RpScDisappearNPC( SRpScDisappearNPC* aMsg );	
public:
	virtual bool	Proc_RpScHackShield(SRpScHackShield * aMsg);
	virtual bool	Proc_RpScGameGuard(SRpScGameGuard * aMsg);
public:
	void	Proc_RpScNPCMove( SRpScNPCMove* aMsg );				
	void	Proc_RpScAnswerWarp( SRpScAnswerWarp* aMsg );
	void 	Proc_RpScAlive( SRpScAlive* aMsg );
	void 	Proc_RpScSeeAliveOnSpot( SRpScSeeAliveOnSpot* aMsg );
	void	Proc_RpScStatus( SRpScStatus* aMsg );
	void	Proc_RpScChangeMainClass( SRpScChangeMainClass* aMsg );
	void	Proc_RpScChangeSubClass( SRpScChangeSubClass* aMsg );
	void	Proc_RpScNPCStat( SRpScNPCStat* aMsg );
	void	Proc_RpScSeeOtherStatus( SRpScSeeOtherStatus* aMsg );
	void	Proc_RpScRecoveryStat( SRpScRecoveryStat* aMsg );
	void	Proc_RpScUpdateGage( SRpScUpdateGage* aMsg );
	void	Proc_RpScDirection( SRpScDirection *aMsg );
	void	Proc_RpScUpdateMaxStat( SRpScUpdateMaxStat *aMsg );
	void	Proc_ScUpdateFame( SRpScUpdateFame * aMsg );
	void	Proc_ScErrorGame( SRpScErrorGame* aMsg );


	//-----------------------------------------------------------
	// Attack or Skill Protocols
	//-----------------------------------------------------------
	bool	GetSkillProtocolResult( int aResult );

	void	Proc_RpScAttack( SRpScAttack* aMsg );
	void	Proc_RpScAttacked( SRpScAttacked* aMsg );
	void	Proc_RpScSeeAttack( SRpScSeeAttack* aMsg );
	void	Proc_RpScSpelling( SRpScSpelling* aMsg );
	void	Proc_RpScSeeSpelling( SRpScSeeSpelling* aMsg );
	void	Proc_RpScSkill( SRpScSkill* aMsg );// 스킬 사용 결과값
	void	Proc_RpScSeeSkill( SRpScSeeSkill* aMsg );// 몹 스킬 발동
	void	Proc_RpScAppearSkill( SRpScAppearSkill* aMsg );
	void	Proc_RpScDisappearSkill( SRpScDisappearSkill* aMsg );
	void	Proc_RpScSeeKeepupSkillEnd( SRpScSeeKeepupSkillEnd* aMsg );
	void	Proc_RpScSeeKeepupSkillPreEnd( SRpScSeeKeepupSkillPreEnd* aMsg );
	void	Proc_RpScGetSkill( SRpScGetSkill* aMsg );
	void	Proc_RpScTakeSkill( SRpScTakeSkill* aMsg );
	void	Proc_ScSkillUp( SRpScSkillUp* aMsg);
	//.........................................................................
	// skill By wxywxy	
	//.........................................................................		
	void	Proc_RpScSeeAuroraSkill(SRpScSeeAuraSkillPtr aMsg );
	void	Proc_RpScKeepupSkillStart(SRpScKeepupSkillStartPtr aMsg);
	

	
	
	//--------------------------------------------------
	// Chatting Protocols
	//--------------------------------------------------
	void	Proc_RpScSay( SRpScSay* aMsg );
	void	Proc_RpScWhisper( SRpScWhisper* aMsg );
	void	Proc_RpScRecvWhisper( SRpScRecvWhisper* aMsg );
	

	//--------------------------------------------------
	// Item Protocols
	//--------------------------------------------------
	bool	GetItemProtocolResult( int aResult );

	void	Proc_RpScPickup( SRpScPickup* aMsg );
	void	Proc_RpScPickupEld( SRpScPickupEld* aMsg );
	void	Proc_RpScDrop( SRpScDrop* aMsg );
	void	Proc_RpScDropEld( SRpScDropEld* aMsg );
	void	Proc_RpScUse( SRpScUse* aMsg );
	void	Proc_RpScSeeUse( SRpScSeeUse* aMsg );
	void	Proc_RpScEquip( SRpScEquip* aMsg );
	void	Proc_RpScChangeEquip( SRpScChangeEquip* aMsg );
	void	Proc_RpScSeeEquip( SRpScSeeEquip* aMsg );
	void	Proc_RpScTakeoff( SRpScTakeoff* aMsg );
	void	Proc_RpScSeeTakeoff( SRpScSeeTakeoff* aMsg );
	void	Proc_RpScBuy( SRpScBuy* aMsg );
	void	Proc_RpScSell( SRpScSell* aMsg );
	void	Proc_RpScCombine( SRpScCombine* aMsg );
	void	Proc_RpScChangeSlot( SRpScChangeSlot* aMsg );
	void	Proc_RpScLinkQuickSlot( SRpScLinkQuickSlot* aMsg );
	void	Proc_RpScUnlinkQuickSlot( SRpScUnlinkQuickSlot* aMsg );
	void	Proc_RpScAppearItem( SRpScAppearItem* aMsg );
	void	Proc_RpScDisappearItem( SRpScDisappearItem* aMsg );
	void	Proc_RpScStatus2( SRpScStatus2* aMsg );
	void	Proc_RpScChangeAvatarSkin(SRpScChangeAvatarSkin* aMsg);
#ifdef WIDE_PAY_INVENTORY //유료인벤확장	
	void    Proc_SRpScPayInven( SRpScPayInven * aMsg ); 
	void    Proc_SRpScPayInvenAdd( SRpScPayInvenAdd * aMsg );
#endif //WIDE_PAY_INVENTORY

	//--------------------------------------------------
	// Exp, LevelUp Protocols
	//--------------------------------------------------	
	void	Proc_RpScGetExp( SRpScGetExp* aMsg );
	void	Proc_RpScLevelup( SRpScLevelup* aMsg );
	void	Proc_RpScSeeLevelup( SRpScSeeLevelup* aMsg );
	void	Proc_RpScGetSkillPoint( SRpScGetSkillPoint* aMsg );
	void	Proc_RpScPoint( SRpScPoint* aMsg );

	//--------------------------------------------------
	// Npc Chatting, Script, Quest Protoclos
	//--------------------------------------------------
	void	Proc_RpScScriptStart( SRpScScriptStart* aMsg );
	void	Proc_RpScDialogScript( SRpScDialogScript* aMsg );
	void	Proc_RpScAskScript( SRpScAskScript* aMsg );
	void	Proc_RpScItemShopInfo( SRpScItemShopInfo* aMsg );
	
	//--------------------------------------------------
	// Quest
	//--------------------------------------------------
	void	Proc_RpScQuestWindow( SRpScQuestWindow* aMsg );
	void	Proc_RpScQuestStatus( SRpScQuestStatus* aMsg );
	void	Proc_RpScQuestCount( SRpScQuestCount* aMsg );
	
	//--------------------------------------------------
	// GM
	//--------------------------------------------------
	void	Proc_RpScGMCmd( SRpScGMCmd* aMsg );
	void	Proc_RpScGMCmdErr( SRpScGMCmdErr* aMsg );

	void	Proc_RpScAnnouncement( SRpScAnnouncement* aMsg );
	void	Proc_ScNPCInfo(RpNPCInfo *aMsg);

	//-------------------------------------------------------
	// PvP Announce
	//--------------------------------------------------------
	void	Proc_ScPvPAnnouncement( SRpScPvPAnnouncement *aMsg);

	//--------------------------------------------------
	// Archer
	//--------------------------------------------------
	void    Proc_RpScArcherReady(SRpScArcherReady *aMsg);
	void    Proc_RpScSeeArcherReady(SRpScSeeArcherReady *aMsg);
	
    //--------------------------------------------------
	// Party            //by Simwoosung
	//--------------------------------------------------
    void	Proc_RpScPartyAskJoin( SRpScPartyAskJoin *aMsg ); 
    void    Proc_RpScPartyAnsJoin( SRpScPartyAnsJoin *aMsg );
    void    Proc_RpScPartyCreate( SRpScPartyCreate * aMsg );
	void    Proc_RpScPartyJoin( SRpScPartyJoin * aMsg );
	void    Proc_RpScPartyInfo( SRpScPartyInfo * aMsg );
	void    Proc_RpScPartyMemberInfo( SRpScPartyMemberInfo * aMsg );
	void	Proc_RpScPartyChangeName( SRpScPartyChangeName * aMsg );
    void    Proc_RpScPartyChangeLeader( SRpScPartyChangeLeader * aMsg );
	void    Proc_RpScPartyLeave( SRpScPartyLeave * aMsg );
    void    Proc_RpScPartyTarget( SRpScPartyTarget * aMsg );
	void    Proc_RpScPartyGiveGage( SRpScPartyGiveGage * aMsg );
	void    Proc_RpScPartyUpdateGage( SRpScPartyUpdateGage * aMsg );
	void    Proc_RpScPartyChat( SRpScPartyChat * aMsg );
	void    Proc_RpScPartyItemMgr( SRpScPartyItemMgr * aMsg );
	void    Proc_RpScPartyGetItem( SRpScPartyGetItem * aMsg );
    void    Proc_RpScPartyGetEld( SRpScPartyGetEld  * aMsg );
	void    Proc_RpScPartyExpMgr( SRpScPartyExpMgr * aMsg );
	void	Proc_RpScPartyPosition( SRpScPartyPosition * aMsg );
	
	//....................................................................
	// NPC Priority
	//....................................................................
	void	Proc_RpScNPCPriority( SRpScNPCPriority * aMsg );
	
	//--------------------------------------------------
	// Exchange Protocols
	//--------------------------------------------------
	bool	GetExchangeResult( int aResult );
	void	Proc_RpScAskExchange( SRpScAskExchange *aMsg );
	void	Proc_RpScAnsExchange( SRpScAnsExchange *aMsg );
	void	Proc_RpScSuggestResult( SRpScSuggestResult *aMsg );
	void	Proc_RpScSuggestExItem( SRpScSuggestExItem *aMsg );
	void	Proc_RpScResultExchange( SRpScResultExchange *aMsg );

	//--------------------------------------------------
	// Warehouse Protocols
	//--------------------------------------------------
	void	Proc_RpScOpenBank( SRpScOpenBank *aMsg );
	void	Proc_RpScDeposit( SRpScDeposit *aMsg );
	void	Proc_RpScDraw( SRpScDraw *aMsg );
	void	Proc_RpScDepositEld( SRpScDepositEld *aMsg );
	void	Proc_RpScDrawEld( SRpScDrawEld *aMsg );
	void	Proc_RpScWHChangeSlot( SRpScWHChangeSlot *aMsg );

#ifdef DECO_RENEWAL_GUILD_MJH

	//--------------------------------------------------
	// GuildInventory Protocols
	//--------------------------------------------------
	void	Proc_RpScOpenFrontierBank( SRpScFrontierBankOpen *aMsg );
	void	Proc_RpScDepositFrontier( SRpScDepositFrontierBank *aMsg );
	void	Proc_RpScDrawFrontier( SRpScDrawFrontierBank *aMsg );
	void	Proc_RpScChangeSlotFrontier( SRpScChangeSlotFrontierBank *aMsg );

#endif //DECO_RENEWAL_GUILD_MJH

	///-----------------------------------------------------------------------
	///-- Item 개조 
	///-----------------------------------------------------------------------
	void	Proc_RpScEldacraft( SRpScEldacraft *aMsg);
	void	Proc_RpScSetStone( SRpScSetStone *aMsg );
	void	Proc_RpScEldacraftGame( SRpScEldacraftGame *aMsg );
	void	Proc_ScSubLevelup( SRpScSubLevelup * aMsg);

	///-----------------------------------------------------------------------
	///-- 날씨 관련 By wxywxy
	///-----------------------------------------------------------------------
	void Proc_RpScWeather(SRpScWeatherPtr aMsg);

	///-- Repair Open Window
	void Proc_RpScOpenRepairWnd( SRpScOpenRepairSkill *aMsg );
	void Proc_RpScUpdateDurabilityItem( SRpScUpdateDurabilityItem *aMsg);
	void Proc_RpScReplyRepair( SRpScReplyRepair *aMsg );
	
	///-- Make System
	void Proc_RpScMakeItem( SRpScMakeItem * aMsg);
	void Proc_ScMakeItemResult( SRpScMakeItemResult * aMsg ); 

	
	
	///-- GM Elda Cheat
	void Proc_ScEldUpgrade( SRpScEldUpgrade * aMsg );
	///-----------------------------------------------------------------------
	///-- community Protocols - 친구 , 차단 , 메신져 채팅 by simwoosung 
	///-----------------------------------------------------------------------
	void Proc_ScMsngrError( SRpScMsngrError * aMsg );
	void Proc_ScMsngrMemberState( SRpScMsngrMemberState * aMsg );
	void Proc_ScMsngrMemberInsertAsk( SRpScMsngrMemberInsertAsk * aMsg );
	void Proc_ScMsngrMemberDelete( SRpScMsngrMemberDelete * aMsg );
	void Proc_ScMsngrMemberBlock( SRpScMsngrMemberBlock * aMsg );
	void Proc_ScMsngrChangeOpt( SRpScMsngrChangeOpt * aMsg );
	void Proc_ScMsngrSay( SRpScMsngrSay * aMsg );
	void Proc_ScMsngrChatSystemMsg( SRpScMsngrChatSystemMsg * aMsg );
	void Proc_ScMsngrChatMemberState( SRpScMsngrChatMemberState * aMsg );
	void Proc_ScMsngrChatCreateRoom( SRpScMsngrChatCreateRoom * aMsg );
	void Proc_ScMsngrChatDestroyRoom( SRpScMsngrChatDestroyRoom * aMsg );
	void Proc_ScMsngrChatInviteAsk( SRpScMsngrChatInviteAsk * aMsg );
	void Proc_ScMsngrChatJoinRoom( SRpScMsngrChatJoinRoom * aMsg );
	void Proc_ScMsngrChatSay( SRpScMsngrChatSay * aMsg );
	
	///-----------------------------------------------------------------------
	///-- 사교동작
	///-----------------------------------------------------------------------
	void Proc_ScExpression(SRpScExpression *aMsg);

	///-----------------------------------------------------------------------
	///-- 콤보 로그 갱신
	///-----------------------------------------------------------------------
	void Proc_ScSaveLog(SRpScSaveLog *aMsg);

	//============================================================================================================
	// PVP 시스템 관련 
	//============================================================================================================
	void Proc_ScFightRoomList( SRpScFightRoomList* aMsg );	
	void Proc_ScNewWaitingFighter( SRpScNewWaitingFighter* aMsg );
	void Proc_ScDelWaitingFighter( SRpScDelWaitingFighter* aMsg );
	void Proc_ScMakeFightRoom( SRpScMakeFightRoom* aMsg );
	void Proc_ScSeeMakeFightRoom( SRpScSeeMakeFightRoom* aMsg );
	void Proc_ScSeeDeleteFightRoom( SRpScSeeDeleteFightRoom* aMsg );
	void Proc_ScUpdateFightRoom( SRpScUpdateFightRoom* aMsg );
	void Proc_ScJoinFightRoom( SRpScJoinFightRoom* aMsg );
	void Proc_ScJoinedFighter( SRpScJoinedFighter* aMsg );
	void Proc_ScOutFightRoom( SRpScOutFightRoom* aMsg );
	void Proc_ScChangeRoomMaster( SRpScChangeRoomMaster* aMsg );
	void Proc_ScChangeFightInfo( SRpScChangeFightInfo* aMsg );
	void Proc_ScSelectTeam( SRpScSelectTeam* aMsg );
	void Proc_ScFightReady( SRpScFightReady* aMsg );
	void Proc_ScFightStart( SRpScFightStart* aMsg );
	void Proc_ScScFightGo( SRpScFightGo* aMsg );
	void Proc_ScFightResult( SRpScFightResult* aMsg );
	void Proc_ScUpdateContribution( SRpScUpdateContribution* aMsg );
	void Proc_ScFightBackRoom( SRpScFightBackRoom* aMsg );
	void Proc_ScFightHistory( SRpScFightHistory* aMsg );

	//......................................................................................................
	// 개인 상점 관련
	//......................................................................................................
	void Proc_ScOpenPCShopWindow(SRpScOpenPCShopWindow *aMsg );		// 상점창 열기
	void Proc_ScOpenPCShop(SRpScOpenPCShop *aMsg);					// 상점 열기
	void Proc_ScSeeOpenPCShop(SRpScSeeOpenPCShop *aMsg);			// 다른 사람 상점 열기
	void Proc_ScSeeClosePCShop(SRpScSeeClosePCShop *aMsg);			// 개인 상점 닫기
	void Proc_ScInPCShop(SRpScInPCShop *aMsg);						// 개인 상점 보기
	void Proc_ScBuyInPCShop(SRpScBuyInPCShop *aMsg);				// 상점에서 물건 사기
	void Proc_ScBuyInMyShop (SRpScBuyInMyShop *aMsg);				// 자기 상점 물건 변경
	
	// Commercial Item 관련 
	void Proc_ScOpenPayShop( SRpScOpenPayShop *aMsg ); 
	void Proc_ScGetPayItem( SRpScGetPayItem *aMsg );
	void Proc_ScUsePayItem( SRpScUsePayItem *aMsg );
	void Proc_ScEndPayItem( SRpScEndPayItem *aMsg );
	void Proc_ScCheckPayTime ( SRpScCheckPayTime *aMsg);
	void Proc_ScSeeChangeFace( SRpScSeeChangeFace *aMsg );
	void Proc_ScSeeChangeHair( SRpScSeeChangeHair *aMsg ); 
	void Proc_ScSeePayEffect( SRpScSeePayEffect *aMsg );
	void Proc_ScUpdateLuck( SRpScUpdateLuck *aMsg );

	// 다른 유저 아이템 사용 관련
	void Proc_RpScSeePayEffect(SRpScSeePayEffect *aMsg);
	

	// Frontier 관련 
	bool GetFrontierProtocolResult( int aResult ); 
	void Proc_RpScAnsMakeFrontier( SRpScAnsMakeFrontier* aMsg );
	void Proc_RpScMsngrAuthorizeFrontier( SRpScMsngrAuthorizeFrontier* aMsg );
	void Proc_RpScMsngrProposeJoinFrontier( SRpScMsngrProposeJoinFrontier* aMsg );
	void Proc_RpScMsngrAnswerJoinFrontier( SRpScMsngrAnswerJoinFrontier* aMsg );
	void Proc_RpScMsngrJoinFrontier( SRpScMsngrJoinFrontier* aMsg );
	void Proc_RpScMsngrSeeJoinFrontier( SRpScMsngrSeeJoinFrontier* aMsg ); 
	void Proc_RpScSeeFrontier( SRpScSeeFrontier* aMsg ); 
	void Proc_RpScMsngrLeaveFrontier( SRpScMsngrLeaveFrontier* aMsg );
	void Proc_RpScMsngrFrontierInfo( SRpScMsngrFrontierInfo* aMsg );
	void Proc_RpScMsngrFrontierMemberInfo( SRpScMsngrFrontierMemberInfo* aMsg ); 
	void Proc_RpScMsngrEmblem( SRpScMsngrEmblem* aMsg );
	void Proc_RpScPayDues( SRpScPayDues* aMsg );
	void Proc_RpScDivideDues( SRpScDivideDues* aMsg );
	void Proc_RpScMsngrFrontierSay( SRpScMsngrFrontierSay* aMsg );
	void Proc_RpScMsngrCreateFrontierPet( SRpScMsngrCreateFrontierPet* aMsg );
	void Proc_RpScMsngrDeleteFrontierPet( SRpScMsngrDeleteFrontierPet* aMsg );
	void Proc_RpScMsngrFrontierPetStatus( SRpScMsngrFrontierPetStatus* aMsg );
	void Proc_RpScMsngrFrontierPetExp( SRpScMsngrFrontierPetExp* aMsg );
	void Proc_RpScMsngrFrontierPetLevelup( SRpScMsngrFrontierPetLevelup* aMsg );
	void Proc_RpScMsngrCallFrontierPet( SRpScMsngrCallFrontierPet* aMsg );
	void Proc_RpScMsngrCancelCallFrontierPet( SRpScMsngrCancelCallFrontierPet* aMsg );	
	void Proc_RpScMsngrBuyFrontierItem( SRpScMsngrBuyFrontierItem* aMsg );
	void Proc_RpScMsngrUpdateFrontierFame( SRpScMsngrUpdateFrontierFame* aMsg );	
	void Proc_RpScMsngrAddFrontierMember( SRpScMsngrAddFrontierMember* aMsg) ;
#ifdef DECO_RENEWAL_GUILD_MJH
	void Proc_RpScChangeFrontierNotice( SRpScChangeFrontierNotice* aMsg );
#endif // DECO_RENEWAL_GUILD_MJH

	// 연합 Frontier 관련
	void Proc_RpScMsngrReqCombine( SRpScMsngrReqCombine * aMsg);
	void Proc_RpScMsngrAnsCombine( SRpScMsngrAnsCombine * aMsg);
	void Proc_RpScMsngrCombine( SRpScMsngrCombine * aMsg);
	void Proc_RpScMsngrSeperateF( SRpScMsngrSeperateF * aMsg);


	// 메가폰 유료아이템용
	void Proc_RpScShout( SRpScShout* aMsg );
	
	// Pet 관련
	bool GetPetProtocolResult(int aResult);
	void Proc_RpScMakePetName( SRpScMakePetName * aMsg );
	void Proc_RpScAddPet( SRpScAddPet * aMsg );
	void Proc_RpScPetStatus( SRpScPetStatus * aMsg );
	void Proc_RpScPetLevelUP( SRpScPetLevelUP * aMsg );
	void Proc_RpScPetStat( SRpScPetStat * aMsg );
	void Proc_RpScPetDie( SRpScPetDie * aMsg );
	void Proc_RpScDeletePet( SRpScDeletePet * aMsg );
	void Proc_RpScMoveItemPCtoPet( SRpScMoveItemPCtoPet * aMsg );
	void Proc_RpScMoveItemPettoPC( SRpScMoveItemPettoPC * aMsg );
	void Proc_RpScUsePetItem( SRpScUsePetItem * aMsg );
	void Proc_RpScCallPet( SRpScCallPet * aMsg );
	void Proc_RpScCancelCallPet( SRpScCancelCallPet * aMsg );
	void Proc_RpScAppearPet( SRpScAppearPet * aMsg );
	void Proc_RpScDisappearPet( SRpScDisappearPet * aMsg );
	void Proc_RpScPetStartMove( SRpScPetStartMove * aMsg );
	void Proc_RpScPetStop( SRpScPetStop * aMsg );
	void Proc_RpScPetAttack( SRpScPetAttack * aMsg );
	void Proc_RpScPetAlive(SRpScPetAlive * aMsg);
	void Proc_RpSRpScUsePetPayItem(SRpScUsePetPayItem * aMsg);
	void Proc_RpSRpScEndPetPayItem(SRpScEndPetPayItem * aMsg);
	void Proc_RpSRpScPetCheckIn(SRpScPetCheckIn * aMsg);
	void Proc_RpSRpScPetCheckOut(SRpScPetCheckOut * aMsg);

	//인던타임
	void Proc_RpScIndunTime(SRpScIndunTime * aMsg);
	
	// 라이딩 관련
	void Proc_RpScRideOn( SRpScRideOn * aMsg );
	void Proc_RpScRideOff( SRpScRideOff * aMsg );

	// 토템 스코어 
	void Proc_ScTotemScore(SRpScTotemScore *aMsg);

	// 소환 요청
	void Proc_RpScAskMove(SRpScAskMove *aMsg);

	// 전체귓말 차단 .. 
	void Proc_RpScDenyMessage( SRpScDenyMessage * aMsg);

	//엘다 혼합 
	void Proc_RpScMixEldaStone(SRpScMixEldaStone* aMsg); 

	//포션 제조 
	void Proc_RpScMakeSkillPotion(SRpScMakeSkillPotion * aMsg);

	//플레이 타임 체크 
	void Proc_SRpScPlaytime(SRpScPlaytime * aMsg);

	//Colony 관련
	void Proc_RpScColonyList(SRpScColonyList * aMsg);
	void Proc_RpScAnsColonyInfo(SRpScAnsColonyInfo * aMsg);
	void Proc_RpScDeclareWar(SRpScDeclareWar * aMsg);
	void Proc_RpScCancelWar(SRpScCancelWar * aMsg);
	void Proc_RpScWarAnnounce(SRpScWarAnnounce * aMsg);
	void Proc_RpScColonyInfo(SRpScColonyInfo * aMsg);

	//중국 PK 관련부분 작업
	void Proc_RpScDeletedPKAttackUser(SRpScDeletedPKAttackUser *aMsg);
	void Proc_RpScPKAttack(SRpScPKAttack *aMsg);
	void Proc_RpScUpdatePKPoint(SRpScUpdatePKPoint *aMsg);	

	//유료 아이템 관련
	void Proc_RpScChangePetName(SRpScChangePetName * aMsg); 
	void Proc_RpScChangeName(SRpScChangeName * aMsg);	
	void Proc_RpScChangeGender(SRpScChangeGender * aMsg);
	void Proc_RpScChangeFrontierPCName(SRpScChangeFrontierPCName * aMsg);
	
	//PVP 활성화 관련
	void Proc_RpScCheckSoldierTime(SRpScCheckSoldierTime * aMsg);	///-- 용병 남은 시간 갱신 프로토콜
	void Proc_RpScNPCAttackedMsg(SRpScNPCAttackedMsg * aMsg);
	void Proc_RpScNPCLevelUP(SRpScNPCLevelUP * aMsg);
	void Proc_RpScEnterPvPField(SRpScEnterPvPField * aMsg);
	void Proc_RpScUpdatePvPField(SRpScUpdatePvPField * aMsg);

	//장신구 가공 주문서 관련
	void Proc_RpScMakeAddAccessory(SRpScMakeAddAccessory * aMsg);

	//남은 점령시간 안내관련
	void Proc_RpScOccupationTime(SRpScOccupationTime * aMsg);

	//------------------------------------------------------------------------------------
	//  엘다스톤 변경 관련
	//------------------------------------------------------------------------------------
	void Proc_RpScChangeEldaStone(SRpScChangeEldaStone * aMsg);

	//------------------------------------------------------------------------------------
	//  서브직업 매뉴얼 
	//------------------------------------------------------------------------------------
	
	void Proc_SRpScMakeStrengthstone( SRpScMakeStrengthstone * aMsg);
	void Proc_SRpScMakeEssence(SRpScMakeEssence * aMsg);
	void Proc_SRpScUseStrengthstone(SRpScUseStrengthstone * aMsg );

	//------------------------------------------------------------------------------------
	//  이벤트관련
	//------------------------------------------------------------------------------------
#ifdef TAIWAN30MINEVENT
	void Proc_SRpScTaiwan30minEvent( SRpScTaiwan30minEvent * aMsg );
	void Proc_RpScTaiwan30minEventStart( SRpScTaiwan30minEventStart * aMsg );
#endif //TAIWAN30MINEVENT

public:		
	///-- By simwoosung
	///-- 유저형 NPC인지 체크
	BOOL	ISCheckPlayerNPC( SRpScAppearNPC* aMsg );	
	
	// 지연되어야 할 메세지 처리 
	void	DelayProc_RpScAnswerWarp( SRpScAnswerWarp* aMsg );
	void	DelayProc_RpScAppearPC( SRpScAppearPC* aMsg );
	void	DelayProc_RpScFightRoomList( SRpScFightRoomList* aMsg );

	// 신수 관련 작업 By wxywxy
	void	Proc_RpScAppearHoly( SRpScAppearNPC* aMsg );
	void	Proc_RpScDisappearHoly( SRpScDisappearNPC* aMsg );
	
	// 펫 관련 작업
	void    DelayProc_RpScAppearPet( SRpScAppearPet* aMsg );
	void	DelayProc_RpScDisAppearPet( SRpScDisappearPet * aMsg );
	
	void    SetProcessAttackEvent( EVENT_DATA_INFO & data_event, 
								   BOOL IsPlayer = TRUE, BOOL IsAttackedDest = FALSE, 
								   int nAttackType = 0, DWORD dAttackID= 0,
								   int nAttackedType = 0, int nAtedIndex = 0);

	// 엘다스톤 가루 변환 
	void	Proc_RpScMakeEldastone( SRpScMakeEldastone* aMsg);


#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 GetTexture(unsigned long	 m_HashCode);
#else
	LPDIRECT3DTEXTURE8 GetTexture(unsigned long	 m_HashCode);
#endif // DIRECT_VERSION_9_MJH

	void SendDebugLog(char *clog, ... );

protected:	
	//--------------------------------------------------------------------
	// SystemParamInfo Setting 
	//--------------------------------------------------------------------
	int		theSetMouseSpeedInfo[3];


public:		
	// 한곳에서 관리해 주기 위해서...쓰레드에서 그려주니까...d3d error 발생...수정
	HRESULT			BeginScene();
	HRESULT			EndScene();
	HRESULT			Present( RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion );

	bool			m_bIsBeginScene;
	bool			m_bIsPresent;
	EtcInfo			m_EtcInfo;
	//By Simwoosung
	BOOL			m_IsDeviceCursor; 
    BOOL			m_IsCursor;
	bool			m_isCursorInit;
	TCHAR			m_strTemp[512];
	bool			m_IsMouseRotate;
	POINT			m_lastPos;
		
	std::vector <sNpcIndexlist > m_NpcIndexList;
#ifdef BOUND_BOX_DISPLAY
	
	float			m_TopCameraHeight;

#endif	
	
	/// by NiceguY 
	NMouseControlType	m_MouseControlType;	

	bool			m_bIsDrawFullScreenUI;
	long			m_PickAttack_DelayTime;

	void Illusion_CsStartMove();
	
	BOOL KillProcess(char * strExeName);

	char strOsVersion[512];
	void GetOsVersion();	

//...........................................................................................................
// 시스템 키 막기
//...........................................................................................................
private:

	STICKYKEYS m_StartupStickyKeys;
	TOGGLEKEYS m_StartupToggleKeys;
	FILTERKEYS m_StartupFilterKeys;    
	
	CWinLock		m_WinLock;
 };


extern	CRockClient			g_RockClient;
extern	HANDLE				g_hLoadingThread;
#ifdef DIRECT_VERSION_9_MJH
extern	LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern	LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

#ifdef JAPAN_HPZERO_BUG_MODIFY
	extern	bool g_bFindRHpZeroAliveMob;
#endif

extern  BOOL ThreadClosed();





#endif
