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
- 12/18/2006 ���� ���� ���� ���⼭ �������� ������ �ɼ��� ���� .. 
- �ε��׽þƴ� basedatastructures.h  �����Ѵ�
- FrameworkProtocols.h <--- ȭ�Ͽ��� �ε��׽þ� Ŭ���̾�Ʈ �ɼ� ON 
- �ε��׽þ� : ���� üũ �� �ؾ��� RockClient.cpp �����Ͻ� +1 ������.
- �� ���󺰷� ���ҽ� ���� �׻� üũ ... ���� ..  
//-----------------------------------------------------*/
//--------------------------------------------------------------------------
//#define	GAMETYPE_TEST
//#define	GAMETYPE_REAL


//API ���� ���� .. ���� �˻� �غ��� .. 


///-- ���� ������
#define FRONTIER
#define WHISPER_GM
#define REMOVE_PDBFILE			///--  ������ PDB������ ���ﶧ ���
#define NEW_CLIENT_MAPTYPE		///--  ���ο� �� ������ �ε� - ��Ÿ�� ��������	

#ifdef TIME_SUIT_UPDATE
	#define C_TIME_SUIT_UPDATE
#endif

#ifdef DROP_WASTEPAYITEM
	#define	C_DROP_WASTEPAYITEM// ����� �ȴٴ°� �ŷ��� �����ϴ�
#endif


#ifdef SUBJOB_RENEWAL	
	#define C_SUBJOB_RENEW
	#define	C_SUBJOB_RENEW_SLOTHELPTIP	//���Կ� ������ ������ �ϱ� 5.22 - DX9 
	#define	C_NPCCHAT_RENEWAL// �������� �������� NPC UI ����
#endif 

#define MEGAPHONE_RENEW			//�ް���  �ϵ��ڵ� �Ǿ��ִ°� Ÿ������ ���� 5.22
#define KEEPUP_PAYITEM_RENEWAL		// ��������� 30�� ���� �ð�ǥ�� ������ 


#ifdef CHANGE_QUEST_CONDITION
	#define QUEST_COUTLIST_RENEWAL		//����Ʈ ī��Ʈ 4�� ���� 8�� Ȯ��
#endif 

//#define SERVERDOWN_ATTACK		// ��Ŷ ��â ������

//#define WIN_MINIMIZE_LOOP
//#define AUTO_COMBO //-- �ڵ��޺� �����ƾ
//#define DEFENSE_EFFECT // ��
//	#define SINGAPORE_BLOCK_PET	//-����  ���ϰ� 
//	#define SINGAPORE_BLOCK_RIDING //-- ���̵� �����ϰ� . 
//#define APPLY_BATTLE_BGSOUND		///-- ���� ����� ����


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

 

// �۷ι�---------------------------------------------------------------------------------------------------------------
#ifdef GLOBAL_VERSION

	#define ONLY_ENGLISH_SET
	#define	APPLY_GUIDE	
	#define	NOT_TALK					//Ÿ���� �� ���� ���ϰ� �Ѵ�.. 
	#define C_PAY_SYSTEM				//������ �������� ������ Ŭ���̾�Ʈ�� ���ξ���.. 
	#define APPLY_BOLD_FONT				///-- ��Ʈ ����ü ����
	#define	LOGINPAGE_BGIMG_CHANGE		//ī�޶� �̵� ���� �ٷ� ĳ���� ȭ�� ����
	#define ROCK_WINDOWS_MODE			//â Ǯ��ũ����� 
	#define SNIPER_MOD_MOTION			//--�������� ��� ����
	#define	ADD_MOTION_DANCE
	#define	CLINET_SKILL_LERNCOST		//������ �������� ����ϴ��� Ȯ���Ѵ�. (�������� ����ϴ��� Ȯ���Ѵ�. )
	#define	HOLY_SKILLICON_MODIFY		///-- �ż� ��ų ������ ǥ��

	#define	EDITBOX_COMMA				///-- �����͹ڽ� �޸� ǥ�� 

	#define	LAUNCHER_UPGREADE_VERSION	// setup.ini�� etc.rvf �̵���Ŵ.. 
	#define APPLY_ENGLISH_WINDOW		///-- ���� ���� ������
	#define APPLY_ENGLISH_ORIGIN_SIZE	///-- �������� -- ���� ���� ũ��� �����ִ� ��ƾ 

	#define NEW_EMBLEM_ITEM_APPLY		///--  ���ο� ���� ���� ��� - ���̺� ���	
	#define KEEPUP_PAYITEM_RENEWAL

	//#define APPLY_SKILL_VELOCITY		///-- ������ ��ų �ӵ��� ������ ��ġ���� ���η�ƾ
	//#define APPLY_BATTLE_BGSOUND		///-- ���� ����� ����
	//#define TEST_REALEASE_CLIENT
	///#define TEST_GAME_NECK			///-- ������ ������ �׽�Ʈ

	#define QUEST_RENEWAL				///����Ʈ ������ .. 
	#define	EXPANSION_QUICK_SLOT		//Ȯ�� ���� 30���ø�. . 
	#define OPTION_RENEWAL				//�ɼ�â ���� 

	#define NPC_MOVE_TO_SKILL				//�̵��� ��ų
	#define	HELPTIP_DATA_RENEWAL		//���� txt ������ -> ����Ÿ�� ���� ��ũ�ΰ����� ����
	#define	BATTLEROOM_HELPBTN_ERASE	// �����忡 ������ ���� 
	#define	CUR_INDUN_CHECK		     	//������ ����  ���� �ν���Ʈ �������� üũ�Ѵ�.. 
	#define KULUBEACH_QUEST_NAME        //����ġ ����Ʈ ���� ���� 


	#ifdef SEALED_TEMPLE_200811// ������ ����

		#define C_SEALED_SKILL_LEARN		//��ų ���� �߰� Ȯ�� 12.14
		#define C_SEALED_SKILLPOTION_RENEW	//��ų����â ������ 12.14 
		#define C_SEALED_ELDASTONE_DUST		// ���ٽ��� ���� 12.14 
		#define	C_SEALED_NPC_CHAT_RENEW		// NPC ��ȭâ ������ 12.14 
		#define C_SEALED_STORE				// ������ ���� ���� ����â ����	12.14 
		#define NPC_CHAT_EXTENSION		// NPC ��ũ��Ʈ ��ȭ ��Ʈ������ 2048 �ø�

	#else 

		//�۷ι��� ������ ������ ���� �ȵ�� ���� ���� .. 
		//������ ������������� �� �������� ���� . 
		#define NPC_CHAT_EXTENSION		// NPC ��ũ��Ʈ ��ȭ ��Ʈ������ 2048 �ø�
		#define C_SEALED_NPC_CHAT_RENEW

	#endif// #ifdef SEALED_TEMPLE_200811// ������ ���� 

	#ifdef SELL_ITEMSHOP
		#define	CSELL_ITEMSHOP				// �������� ������ �� �� 20%���� �� (�������� ����ϴ��� Ȯ���Ѵ�. )
	#endif

#endif// #ifdef GLOBAL_VERSION
// �۷ι�---------------------------------------------------------------------------------------------------------------



// Ÿ�̿�---------------------------------------------------------------------------------------------------------------
#ifdef TAIWAN_VERSION
	
	#define KULUBEACH_QUEST_NAME        //����ġ ����Ʈ ���� ����
	#define GANJA_WORD_BLOCK_20090630	// ĳ���� ������ �̸��� ����ü�ΰ�� ����. ��ü�� ���
	#define	APP_IME_BLOCK_CHS			// �߱��� ��ü ���� .. 

	#define SERVERLIST_TEXT_NON_PVP		//�븸 ������ NONPVP ��������Ʈ�� ��� 5.22 - DX9
	#define	LAUNCHER_UPGREADE_VERSION	// setup.ini�� etc.rvf �̵���Ŵ..	
	#define EXPANSION_SERVERTAB			//��������Ʈ Ȯ��
	#define	APPLY_GUIDE	
//	#define	NOT_TALK					//Ÿ���� �� ���� ���ϰ� �Ѵ�.. 
	#define C_PAY_SYSTEM			//������ �������� ������ Ŭ���̾�Ʈ�� ���ξ���.. 
	#define APPLY_BOLD_FONT				///-- ��Ʈ ����ü ����
	//#define	LOGINPAGE_BGIMG_CHANGE		//ī�޶� �̵� ���� �ٷ� ĳ���� ȭ�� ����
	#define ROCK_WINDOWS_MODE			//â Ǯ��ũ����� 
	#define SNIPER_MOD_MOTION			//--�������� ��� ����
	#define	ADD_MOTION_DANCE
	#define	CLINET_SKILL_LERNCOST		//������ �������� ����ϴ��� Ȯ���Ѵ�. (�������� ����ϴ��� Ȯ���Ѵ�. )
	#define	HOLY_SKILLICON_MODIFY		///-- �ż� ��ų ������ ǥ��

	///--#define APPLY_SKILL_VELOCITY
	#define QUEST_RENEWAL				///����Ʈ ������ .. 
	#define	EXPANSION_QUICK_SLOT		//Ȯ�� ���� 30���ø�. . 
	#define OPTION_RENEWAL				//�ɼ�â ���� 

	#define NEW_EMBLEM_ITEM_APPLY		///--  ���ο� ���� ���� ��� - ���̺� ���
	
	//#define APPLY_BATTLE_BGSOUND		///-- ���� ����� ����

	#ifdef SELL_ITEMSHOP
		#define	CSELL_ITEMSHOP			// �������� ������ �� �� 20%���� �� (�������� ����ϴ��� Ȯ���Ѵ�. )
	#endif

	#define NPC_MOVE_TO_SKILL				//�̵��� ��ų
	#define	HELPTIP_DATA_RENEWAL		//���� txt ������ -> ����Ÿ�� ���� ��ũ�ΰ����� ����
	#define	BATTLEROOM_HELPBTN_ERASE	// �����忡 ������ ���� 
	
//	#define NEXT_SELECT_TO_TAB

	#define	CUR_INDUN_CHECK			//������ ����  ���� �ν���Ʈ �������� üũ�Ѵ�.. 
	
	

	#ifdef SEALED_TEMPLE_200811// ������ ����

		#define C_SEALED_SKILL_LEARN		//��ų ���� �߰� Ȯ�� 12.14
		#define C_SEALED_SKILLPOTION_RENEW	//��ų����â ������ 12.14 
		#define C_SEALED_ELDASTONE_DUST		// ���ٽ��� ���� 12.14 
		#define	C_SEALED_NPC_CHAT_RENEW		// NPC ��ȭâ ������ 12.14 
		#define C_SEALED_STORE				// ������ ���� ���� ����â ����	12.14 
		#define NPC_CHAT_EXTENSION

	#endif 

	//  #define ITEMLIMITFUNC_NOTQUICKSLOT	// �����Կ� �ö󰡴� ������ ����.   by.jjplusi011		12.30
	//	#define FIRSTLOADING_IMAGEFIXING	// ù �ε��� �̹��� �����ϱ�.	by.jjplusi011	09.01.06

	#define HOLYLAND_OF_SEAL_LOADING_IMAGE	//�����Ǽ��� �ʵ鰣�� �ε��̹���	by.jjplusi011 09.01.12
	
	#ifndef _DEBUG
		#define	C_HACK_SHIELD			//--�ٽ��� 
	#endif	

#endif// #ifdef TAIWAN_VERSION
// Ÿ�̿�---------------------------------------------------------------------------------------------------------------



// ȫ��-----------------------------------------------------------------------------------------------------------------
#ifdef HONGKONG_VERSION
	
	//#define GANJA_WORD_BLOCK_20090630	// ĳ���� ������ �̸��� ����ü�ΰ�� ����. ��ü�� ���
	//#define	APP_IME_BLOCK_CHS			// �߱��� ��ü ���� .. 

	#define	LAUNCHER_UPGREADE_VERSION	// setup.ini�� etc.rvf �̵���Ŵ.. 

	#define KULUBEACH_QUEST_NAME        //����ġ ����Ʈ ���� ����
	#define	APPLY_GUIDE	
	#define	NOT_TALK					//Ÿ���� �� ���� ���ϰ� �Ѵ�.. 
	#define C_PAY_SYSTEM			//������ �������� ������ Ŭ���̾�Ʈ�� ���ξ���.. 
	#define APPLY_BOLD_FONT				///-- ��Ʈ ����ü ����
	//#define	LOGINPAGE_BGIMG_CHANGE		//ī�޶� �̵� ���� �ٷ� ĳ���� ȭ�� ����
	#define ROCK_WINDOWS_MODE			//â Ǯ��ũ����� 
	#define SNIPER_MOD_MOTION			//--�������� ��� ����
	#define	ADD_MOTION_DANCE
	#define	CLINET_SKILL_LERNCOST		//������ �������� ����ϴ��� Ȯ���Ѵ�. (�������� ����ϴ��� Ȯ���Ѵ�. )
	#define	HOLY_SKILLICON_MODIFY		///-- �ż� ��ų ������ ǥ��


	//#define	EDITBOX_COMMA			///-- �����͹ڽ� �޸� ǥ�� 

	///--#define APPLY_SKILL_VELOCITY
	#define QUEST_RENEWAL				///����Ʈ ������ .. 
	#define	EXPANSION_QUICK_SLOT		//Ȯ�� ���� 30���ø�. . 
	#define OPTION_RENEWAL				//�ɼ�â ���� 

	#define NEW_EMBLEM_ITEM_APPLY		///--  ���ο� ���� ���� ��� - ���̺� ���


	#define	HELPTIP_DATA_RENEWAL		//���� txt ������ -> ����Ÿ�� ���� ��ũ�ΰ����� ����
	#define	BATTLEROOM_HELPBTN_ERASE	// �����忡 ������ ���� 
	
//  #define ITEMLIMITFUNC_NOTQUICKSLOT	// �����Կ� �ö󰡴� ������ ����.   by.jjplusi011		12.30
//	#define FIRSTLOADING_IMAGEFIXING	// ù �ε��� �̹��� �����ϱ�.	by.jjplusi011	09.01.06

	#define HOLYLAND_OF_SEAL_LOADING_IMAGE	//�����Ǽ��� �ʵ鰣�� �ε��̹���	by.jjplusi011 09.01.12
	#define NPC_MOVE_TO_SKILL				//�̵��� ��ų
	#define	CUR_INDUN_CHECK			//������ ����  ���� �ν���Ʈ �������� üũ�Ѵ�.. 
	
	#define SERVERLIST_TEXT_NON_PVP	//�븸 ������ NONPVP ��������Ʈ�� ��� 5.22 - DX9

	#ifdef SEALED_TEMPLE_200811// ������ ����

		#define C_SEALED_SKILL_LEARN		//��ų ���� �߰� Ȯ�� 12.14
		#define C_SEALED_SKILLPOTION_RENEW	//��ų����â ������ 12.14 
		#define C_SEALED_ELDASTONE_DUST		// ���ٽ��� ���� 12.14 
		#define	C_SEALED_NPC_CHAT_RENEW		// NPC ��ȭâ ������ 12.14 
		#define C_SEALED_STORE				// ������ ���� ���� ����â ����	12.14 
		#define NPC_CHAT_EXTENSION

	#endif// #ifdef SEALED_TEMPLE_200811// ������ ����

	#define EXPANSION_SERVERTAB			//��������Ʈ Ȯ��
	
	#ifndef _DEBUG	
		#define C_GAME_GUARD				//��������Ʈ
	#endif 

	#ifdef SELL_ITEMSHOP
		#define	CSELL_ITEMSHOP			// �������� ������ �� �� 20%���� �� (�������� ����ϴ��� Ȯ���Ѵ�. )
	#endif

#endif// #ifdef HONGKONG_VERSION
// ȫ��-----------------------------------------------------------------------------------------------------------------



// ���ѹα�-------------------------------------------------------------------------------------------------------------
#ifdef KOREA_VERSION	

//	#define TEST_INSTALL_VERSION		///�׽�Ʈ������ ������ .. ���ʿ� . 

	#ifdef SKILL_LEARNCOST

		#define	CLINET_SKILL_LERNCOST		//������ �������� ����ϴ��� Ȯ���Ѵ�. (�������� ����ϴ��� Ȯ���Ѵ�. )

	#endif// #ifdef SKILL_LEARNCOST

	//#define KULUBEACH_QUEST_NAME      //����ġ ����Ʈ ���Ӻ���
	#define	APPLY_GUIDE					///-- ����鼭
	#define	NOT_TALK					//Ÿ���� �� ���� ���ϰ� �Ѵ�.. 
	#define C_PAY_SYSTEM				//������ �������� ������ Ŭ���̾�Ʈ�� ���ξ���.. 
	#define APPLY_BOLD_FONT				///-- ��Ʈ ����ü ����
	#define	LOGINPAGE_BGIMG_CHANGE		//ī�޶� �̵� ���� �ٷ� ĳ���� ȭ�� ����
	#define ROCK_WINDOWS_MODE			//â Ǯ��ũ����� 
//	#define CREATE_CHRACTER_BTN			/// - ������ư ��Ȱ��ȭ .

	#define	HOLY_SKILLICON_MODIFY		///-- �ż� ��ų ������ ǥ��
	#define SNIPER_MOD_MOTION			//--�������� ��� ����
	#define	ADD_MOTION_DANCE

	#define	EDITBOX_COMMA				///-- �����͹ڽ� �޸� ǥ�� 
	#define	APPLY_INTRO_ANI				///-- ��Ʈ�� �ִ� ���뿩��
	#define NEW_EMBLEM_ITEM_APPLY		///--  ���ο� ���� ���� ��� - ���̺� ���
	#define KEEPUP_PAYITEM_RENEWAL

	///--#define APPLY_SKILL_VELOCITY
	#define QUEST_RENEWAL				///����Ʈ ������ .. 
	#define	EXPANSION_QUICK_SLOT		//Ȯ�� ���� 30���ø�. . 
	#define OPTION_RENEWAL				//�ɼ�â ���� 

	#define NPC_MOVE_TO_SKILL				//�̵��� ��ų
//	#define NEXT_SELECT_TO_TAB
	#define	BATTLEROOM_HELPBTN_ERASE	// �����忡 ������ ���� 
	#define	CUR_INDUN_CHECK			//������ ����  ���� �ν���Ʈ �������� üũ�Ѵ�.. 

	#ifdef SEALED_TEMPLE_200811// ������ ����

		#define C_SEALED_SKILL_LEARN		//��ų ���� �߰� Ȯ�� 12.14
		#define C_SEALED_SKILLPOTION_RENEW	//��ų����â ������ 12.14 
		#define C_SEALED_ELDASTONE_DUST		// ���ٽ��� ���� 12.14 
		#define	C_SEALED_NPC_CHAT_RENEW		// NPC ��ȭâ ������ 12.14 
		#define C_SEALED_STORE				// ������ ���� ���� ����â ����	12.14 
		#define NPC_CHAT_EXTENSION		// NPC ��ũ��Ʈ ��ȭ ��Ʈ������ 2048 �ø�

	#endif// #ifdef SEALED_TEMPLE_200811// ������ ����
	
	#define	HELPTIP_DATA_RENEWAL		//���� txt ������ -> ����Ÿ�� ���� ��ũ�ΰ����� ����

//  #define ITEMLIMITFUNC_NOTQUICKSLOT	// �����Կ� �ö󰡴� ������ ����.   by.jjplusi011		12.30
//	#define FIRSTLOADING_IMAGEFIXING	// ù �ε��� �̹��� �����ϱ�.	by.jjplusi011	09.01.06

	#define HOLYLAND_OF_SEAL_LOADING_IMAGE	//�����Ǽ��� �ʵ鰣�� �ε��̹���	by.jjplusi011 09.01.12

	//#define APPLY_BATTLE_BGSOUND		///-- ���� ����� ����

	#define UPGRADE_WING_HELMET

	#ifndef _DEBUG

		//#define	C_HACK_SHIELD			//--�ٽ��� 
		
		#ifdef TEST_INSTALL_VERSION
			#define	LAUNCHER_UPGREADE_VERSION	//etc���� ����IP �޾ƿ� 
		#endif	//TEST_INSTALL_VERSION

	#endif	//_DEBUG

#endif// #ifdef KOREA_VERSION
// ���ѹα�-------------------------------------------------------------------------------------------------------------


// �Ϻ�-----------------------------------------------------------------------------------------------------------------
#ifdef JAPAN_VERSION

//	#define	NOT_TALK	//Ÿ���� �� ���� ���ϰ� �Ѵ�..        

	//�׼� �ӽ÷� ... ��������� �Ⱥ��̰� ó���Ѵ�.
	#define KULUBEACH_QUEST_NAME        //����ġ ����Ʈ ���� ����
	#define	APPLY_GUIDE	
	#define C_PAY_SYSTEM				//������ �������� ������ Ŭ���̾�Ʈ�� ���ξ���.. 
	#define JAPAN_HPZERO_BUG_MODIFY		///-- �Ϻ� �� 0���� ã�� ���� ����κ�
	#define REAPPEAR_DIFFICULT_BUG_LOG  ///-- ���� ����� ���� �α�κ�
	#define SNIPER_MOD_MOTION			//--�������� ��� ����
	#define	ADD_MOTION_DANCE
	#define	APPLY_INTRO_ANI				//-- ��Ʈ�� �ִ� ���뿩��
	#define	HOLY_SKILLICON_MODIFY			///-- �ż� ��ų ������ ǥ��	


	#define NEW_EMBLEM_ITEM_APPLY		///--  ���ο� ���� ���� ��� - ���̺� ���
	#define KEEPUP_PAYITEM_RENEWAL
	
	#define ROCK_WINDOWS_MODE			///-- â Ǯ��ũ����� 
	#define SAVE_PINFO_LOG_BUTTON		///-- ����� ���(GM)���� ��Ŷ���������� ����Ѵ�.

	#define	BATTLEMAP_OPTIMIZE			// 61,62 �� ���� ����ȭ �Ѵ�. 

	//#define TEST_NO_HACK_SHIELD			///-- �ٽ��� ���� Ŭ���̾�Ʈ �׽�Ʈ

	#define QUEST_RENEWAL				///����Ʈ ������ .. 
	#define	EXPANSION_QUICK_SLOT		//Ȯ�� ���� 30���ø�. . 
	#define OPTION_RENEWAL				//�ɼ�â ���� 
	#define	BATTLEROOM_HELPBTN_ERASE	// �����忡 ������ ���� 
	#define	CUR_INDUN_CHECK			//������ ����  ���� �ν���Ʈ �������� üũ�Ѵ�.. 

	#ifdef SEALED_TEMPLE_200811

		#define C_SEALED_SKILL_LEARN		//��ų ���� �߰� Ȯ�� 12.14
		#define C_SEALED_SKILLPOTION_RENEW	//��ų����â ������ 12.14 
		#define C_SEALED_ELDASTONE_DUST		// ���ٽ��� ���� 12.14 
		#define	C_SEALED_NPC_CHAT_RENEW		// NPC ��ȭâ ������ 12.14 
		#define C_SEALED_STORE				// ������ ���� ���� ����â ����	12.14 
		#define NPC_CHAT_EXTENSION		// NPC ��ũ��Ʈ ��ȭ ��Ʈ������ 2048 �ø�

	#endif 

	//  #define ITEMLIMITFUNC_NOTQUICKSLOT	// �����Կ� �ö󰡴� ������ ����.   by.jjplusi011

	#define NPC_MOVE_TO_SKILL				//�̵��� ��ų
 
	#ifdef SKILL_LEARNCOST
		#define	CLINET_SKILL_LERNCOST		//������ �������� ����ϴ��� Ȯ���Ѵ�. (�������� ����ϴ��� Ȯ���Ѵ�. )
	#endif 	

	#ifndef _DEBUG
		#define	C_HACK_SHIELD
	#endif

#endif// #ifdef JAPAN_VERSION
// �Ϻ�-----------------------------------------------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////
#ifdef INDONESIA_VERSION			
//////////////////////////////////////////////////////////////////////////
#define ONLY_ENGLISH_SET
#define	NOT_TALK					//Ÿ���� �� ���� ���ϰ� �Ѵ�.. 	


#define	LOGINPAGE_BGIMG_CHANGE		//ī�޶� �̵� ���� �ٷ� ĳ���� ȭ�� ���� 

//�׼� �ӽ÷� ... ��������� �Ⱥ��̰� ó���Ѵ�.
#define	APPLY_GUIDE	
#define C_PAY_SYSTEM				//������ �������� ������ Ŭ���̾�Ʈ�� ���ξ���.. 
#define SNIPER_MOD_MOTION			//--�������� ��� ����
#define	ADD_MOTION_DANCE
#define	APPLY_INTRO_ANI				//-- ��Ʈ�� �ִ� ���뿩��
#define	HOLY_SKILLICON_MODIFY		///-- �ż� ��ų ������ ǥ��	

#endif


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef CHINA_VERSION
//////////////////////////////////////////////////////////////////////////
	#define	APPLY_GUIDE	
	#define	NOT_TALK					//Ÿ���� �� ���� ���ϰ� �Ѵ�.. 
	#define C_PAY_SYSTEM				//������ �������� ������ Ŭ���̾�Ʈ�� ���ξ���.. 
	#define EXPANSION_SERVERTAB			//��������Ʈ Ȯ��(�߱��� ����)
	#define ROCK_WINDOWS_MODE			//â Ǯ��ũ����� 
	#define	LAUNCHER_UPGREADE_VERSION	// setup.ini�� etc.rvf �̵���Ŵ.. 
	#define	CLINET_SKILL_LERNCOST		//������ �������� ����ϴ��� Ȯ���Ѵ�. (�������� ����ϴ��� Ȯ���Ѵ�. )
	#define	CSELL_ITEMSHOP				// �������� ������ �� �� 20%���� �� (�������� ����ϴ��� Ȯ���Ѵ�. )
	//#define C_KEEPING_CHARGE			//â�� �̿�� ������ (�������� ����ϴ��� Ȯ���Ѵ�. )
	#define SNIPER_MOD_MOTION	
	//	#define	ADD_MOTION_DANCE
	#define	HOLY_SKILLICON_MODIFY		///-- �ż� ��ų ������ ǥ��

	#define	EDITBOX_COMMA				///-- �����͹ڽ� �޸� ǥ�� 
	#define NEW_EMBLEM_ITEM_APPLY		///--  ���ο� ���� ���� ��� - ���̺� ���

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
 


#define	NETWORK_CLIENT		/// �� ���ǰ� ������ ������ ����. ������ ���ÿ��� �̱۸��θ� ���� 

enum	NLoginStep
{
	nNotConnect,			/// �ʱ� ȭ�� id�� passwd�� �Է¹����� connect �� account login ����(�ϱ� �������� ����) 
	nError,
	nAccountLogin,			/// ������ Account Login �� �����ϰ� ��ٸ��� ���� 
	nServerSelectWait,		/// Login ���� ������, �����κ��� ServerSet List�� ���� ���� ����. ����ڰ� ������ �����ϱ⸦ ��� 
	nServerSelectOK,		/// ����ڰ� ���� ���� OK. �ش� ���� ������ ���� 
	nCharacterSelectWait,	/// ���Ӽ������� ĳ���� ����Ʈ�� ���� ���� ����. ����ڰ� ĳ���� �����ϵ��� ��� 
	nCharacterSelectOK,		/// ĳ���� ���� ��, ���� ������ ĳ���� ���� �������� ���� 
	nAllPrepareWait,		/// �������� ���� �α��� �غ� �Ϸ�Ǿ����� �˷���. 
	nAllPrepareOK,			/// ������ Ŭ���̾�Ʈ�� ���� �α��� �غ� �Ϸ�Ǿ����� ���� 
	nInGamePlay				/// ���� ���忡 ���� 
};

//-----------------------------------------------------------------------------
//-- ���� Chatting Type
enum NMonsterChatType
{
	n_Monster_Attack,		///-- ���Ͱ� ������ ���� �޼���		
	n_Monster_Attacked,		///-- ���Ͱ� ���ݴ��� ���� �޼���
	n_Monster_Wait,			///-- ���Ͱ� ����� ���� �޼���
	n_Monster_Move,			///-- ���Ͱ� ������ ���� �޼���
	n_Monster_Rengen,		///-- ���Ͱ� ���� �� ���� �޼���
	n_Monster_Skill,		///-- ���Ͱ� ��ų ����� ���� �޼���
	n_Monster_Die			///-- ���Ͱ� ���� ���� �޼���
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
	char 			m_szRootDir[MAX_PATH];// �����ڿ��� ����
	wchar_t			m_szRootDirW[MAX_PATH];
	bool			m_bSaveInfo;

	char			theCurrentMapPath[256];

	int			m_IsClickScreen;	//0 ��ư�Է��� �޴´� 1:��ư�� ������ 2 ������ ��� �ִ�. 
	
public:
	CRockClient();
	~CRockClient();
//Hack Shield
public:
	
	BOOL HS_Init();// �ٽ��� �ʱ�ȭ
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
    HRESULT DeleteDeviceObjects();  //by simwoosung - ����̽��� ����� ��ü ����
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
	void			OnCrash();	/// �ڵ� ���� ����Ʈ by sooree
	
	void			BeginLoadingBarThread();
	void			LoadingBarSafeStopAfterProcess();

	CFixSizeBillboard m_FixSizeBillboard;				// ������ �������ִ� ������
	SGameTypeInfo	  m_GameTypeInfo;					// ���� Ÿ������	

	void	ResetNowTextureDetail();
	BOOL			LoadSetup();
	BOOL			InitLanguege();
	void			DeleteEmblemFile();					//Only China 
	

public:

	CSceneManager	m_SceneManager;						// ��ü ȭ�� ��� ���� ����

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
	BOOL			m_IsMapLoding;									// ���� ��ų ���� ĳ���� �ڽ� ����Ʈ����°� üũ

	BOOL			LoginProc( char* aUserName, char* aPassWord, UINT uForm = n_Normal );// Login Packet Send
	BOOL			ReLoginProc(void);

	
	BOOL				theUserSelectExit;

	NLoginStep			theLoginStep;
	Void		SetLoginStep( NLoginStep aLoginStep );
	inline	NLoginStep	GetLoginStep()							{	return( theLoginStep );	}

	LRESULT				UserInputProc();

protected:
	HRESULT	InitState(); // Create() ȣ��. ���⼭ CRui* ����
	HRESULT FrameMove()			{ if( m_bGameEnd == false ) return Main();		return E_FAIL;	}
	HRESULT Render()			{ if( m_bGameEnd == false ) return Display();	return E_FAIL;	}
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	HRESULT			GetEnvironment(){ return Render3DEnvironment();}
	HRESULT			Main();
	HRESULT			Display();
//���׽�Ʈ

	CGMCmdDlg				theGMCmdDlg;
	CPacketTransLogDlg      thePacketTransLogDlg;
	
	BOOL					m_IsLoadProcessAfter;
	DWORD					m_InitTime;

	//-------------------------------------------------------------------------
	int				MouseProcess();///....sh
	int				KeyProcess();///....sh
	void			CheckWarpzone();///....sh
	bool			UserKeyProcess();//������ ����ϴ�Ű 
	bool			TestKeyProcess();//�׽�Ʈ �Ҷ�����ϴ�Ű 
	void			SetCharacterInfo();	

	void			SetPlayerMessage( TCHAR* str );					//-- �÷��̾��� ��ȭâ ����
	void			SetNonPlayerMessage( DWORD pcid, TCHAR* str );	//-- NPC ��ȭâ ����( Ÿ ���� ) 
	void			SetMonsterMessage( DWORD pcid, NMonsterChatType atype , int RandomRatio = 0 );	//-- Monster ��ȭâ ���� RandomRatio 1~10 ��������

	
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
	
	WNDID			m_WndID;//����...��Ŀ���� ������ �ִ� â�� ID
	CFrameWnd*		m_WndHandle;
	//-------------------------------------------------------------------------
	int				m_bTakeoff; // ������ �����Ͽ� �߰���

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
	long				m_PickUnique;		// ��ŷ �ǰ� �ִ� ����ũ ��ȣ by wxywxy	

public:

	/// sooree
#ifdef DIRECT_VERSION_9_MJH
	HRESULT ScreenGrab(LPDIRECT3DDEVICE9 pDev, char *fileName , char *OutPutFileName );// ��ũ����
#else
	HRESULT ScreenGrab(LPDIRECT3DDEVICE8 pDev, char *fileName , char *OutPutFileName );// ��ũ����
#endif // DIRECT_VERSION_9_MJH
	HRESULT ScreenCapture();

	SKeys		theKey[256];
	SMouse		theMouse;

	CSlangFilter	m_SlangFilter;
	
	//..........................................................................................................
	// �米���� ����
	//..........................................................................................................
	CAction			m_Action;
	int				SetAction(Character *pSrc,int MotionIndex,BOOL ISKeeping);
	//..........................................................................................................
	
	// �α��� ����
	CClassView		m_ClassView;
	CLoginShutter	m_LoginShutter;

	CSquenceManager	m_SquenceManager;
	
	//------------------------------------------------------------------------------------------------
	// ���̵� ����
	//------------------------------------------------------------------------------------------------
	void			SetRideOn(int nPcIndex);
	void			SetRideOff(int nPcIndex);
	BOOL			CanRideOnDest();

	//���� �޸� .. 
	void CheckFreeVideoMemory();
	void CheckUseVideoMemory();
	
	//������ ���� 
	void CreateItemPcItem(SPcItem** tmpPcItem , URpdItem URpdItemInfo);

	//�ٽ��� 
	DWORD m_dwNewTimeHack;
	BOOL	m_bHackStart;

	//setup.ini���� �о�帱 ���� 
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
	// ȭ�� ó�� ��ƾ
	//..........................................................................................................
	
	HRESULT	LoginStepProcess(void);									// �α��� ó�� ��ƾ
	HRESULT	CharacterSelectStepProcess(void);						// ĳ���� ���� ó�� ��ƾ
	
	//..........................................................................................................
	// ȭ�� ǥ�� ��ƾ
	//..........................................................................................................

	void LoginPageDisplay(void);									// �α��� ȭ�� ǥ��
	void CharacCreatePageDisplay(void);								// ĳ���� ���� ȭ�� ǥ��
	void CharacSelectPageDisplay(void);								// ĳ���� ���� ȭ�� ǥ��

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
	
	void				CloseLoginState(void);		// �α��� ���� �ݰ� �α��� ���·� ������
	void				LogOff(void);				// �α� ����
	void				SelecttoServerList(void);	// ĳ���ͼ��ÿ��� ��������Ʈ�� ��ȯ

	//........................................................................................................
	// �ӽ� ��ų �׽�Ʈ ( CMD -> /��������Ʈ ��ų�ڵ� �������´�NPC����)
	//........................................................................................................
	void CreateExampleEffect(DWORD Code,int Num);
	
protected:	
	
	//by simwoosung �������� üũ
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
	virtual	void	Proc_RpScPCLogin( SRpScPCLogin* aMsg );// ���Ӽ��� ���ӽ� ������ ĳ������ ����(�Ӽ�, ����, ������ ��)
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
	void	Proc_RpScSkill( SRpScSkill* aMsg );// ��ų ��� �����
	void	Proc_RpScSeeSkill( SRpScSeeSkill* aMsg );// �� ��ų �ߵ�
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
#ifdef WIDE_PAY_INVENTORY //�����κ�Ȯ��	
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
	///-- Item ���� 
	///-----------------------------------------------------------------------
	void	Proc_RpScEldacraft( SRpScEldacraft *aMsg);
	void	Proc_RpScSetStone( SRpScSetStone *aMsg );
	void	Proc_RpScEldacraftGame( SRpScEldacraftGame *aMsg );
	void	Proc_ScSubLevelup( SRpScSubLevelup * aMsg);

	///-----------------------------------------------------------------------
	///-- ���� ���� By wxywxy
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
	///-- community Protocols - ģ�� , ���� , �޽��� ä�� by simwoosung 
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
	///-- �米����
	///-----------------------------------------------------------------------
	void Proc_ScExpression(SRpScExpression *aMsg);

	///-----------------------------------------------------------------------
	///-- �޺� �α� ����
	///-----------------------------------------------------------------------
	void Proc_ScSaveLog(SRpScSaveLog *aMsg);

	//============================================================================================================
	// PVP �ý��� ���� 
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
	// ���� ���� ����
	//......................................................................................................
	void Proc_ScOpenPCShopWindow(SRpScOpenPCShopWindow *aMsg );		// ����â ����
	void Proc_ScOpenPCShop(SRpScOpenPCShop *aMsg);					// ���� ����
	void Proc_ScSeeOpenPCShop(SRpScSeeOpenPCShop *aMsg);			// �ٸ� ��� ���� ����
	void Proc_ScSeeClosePCShop(SRpScSeeClosePCShop *aMsg);			// ���� ���� �ݱ�
	void Proc_ScInPCShop(SRpScInPCShop *aMsg);						// ���� ���� ����
	void Proc_ScBuyInPCShop(SRpScBuyInPCShop *aMsg);				// �������� ���� ���
	void Proc_ScBuyInMyShop (SRpScBuyInMyShop *aMsg);				// �ڱ� ���� ���� ����
	
	// Commercial Item ���� 
	void Proc_ScOpenPayShop( SRpScOpenPayShop *aMsg ); 
	void Proc_ScGetPayItem( SRpScGetPayItem *aMsg );
	void Proc_ScUsePayItem( SRpScUsePayItem *aMsg );
	void Proc_ScEndPayItem( SRpScEndPayItem *aMsg );
	void Proc_ScCheckPayTime ( SRpScCheckPayTime *aMsg);
	void Proc_ScSeeChangeFace( SRpScSeeChangeFace *aMsg );
	void Proc_ScSeeChangeHair( SRpScSeeChangeHair *aMsg ); 
	void Proc_ScSeePayEffect( SRpScSeePayEffect *aMsg );
	void Proc_ScUpdateLuck( SRpScUpdateLuck *aMsg );

	// �ٸ� ���� ������ ��� ����
	void Proc_RpScSeePayEffect(SRpScSeePayEffect *aMsg);
	

	// Frontier ���� 
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

	// ���� Frontier ����
	void Proc_RpScMsngrReqCombine( SRpScMsngrReqCombine * aMsg);
	void Proc_RpScMsngrAnsCombine( SRpScMsngrAnsCombine * aMsg);
	void Proc_RpScMsngrCombine( SRpScMsngrCombine * aMsg);
	void Proc_RpScMsngrSeperateF( SRpScMsngrSeperateF * aMsg);


	// �ް��� ��������ۿ�
	void Proc_RpScShout( SRpScShout* aMsg );
	
	// Pet ����
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

	//�δ�Ÿ��
	void Proc_RpScIndunTime(SRpScIndunTime * aMsg);
	
	// ���̵� ����
	void Proc_RpScRideOn( SRpScRideOn * aMsg );
	void Proc_RpScRideOff( SRpScRideOff * aMsg );

	// ���� ���ھ� 
	void Proc_ScTotemScore(SRpScTotemScore *aMsg);

	// ��ȯ ��û
	void Proc_RpScAskMove(SRpScAskMove *aMsg);

	// ��ü�Ӹ� ���� .. 
	void Proc_RpScDenyMessage( SRpScDenyMessage * aMsg);

	//���� ȥ�� 
	void Proc_RpScMixEldaStone(SRpScMixEldaStone* aMsg); 

	//���� ���� 
	void Proc_RpScMakeSkillPotion(SRpScMakeSkillPotion * aMsg);

	//�÷��� Ÿ�� üũ 
	void Proc_SRpScPlaytime(SRpScPlaytime * aMsg);

	//Colony ����
	void Proc_RpScColonyList(SRpScColonyList * aMsg);
	void Proc_RpScAnsColonyInfo(SRpScAnsColonyInfo * aMsg);
	void Proc_RpScDeclareWar(SRpScDeclareWar * aMsg);
	void Proc_RpScCancelWar(SRpScCancelWar * aMsg);
	void Proc_RpScWarAnnounce(SRpScWarAnnounce * aMsg);
	void Proc_RpScColonyInfo(SRpScColonyInfo * aMsg);

	//�߱� PK ���úκ� �۾�
	void Proc_RpScDeletedPKAttackUser(SRpScDeletedPKAttackUser *aMsg);
	void Proc_RpScPKAttack(SRpScPKAttack *aMsg);
	void Proc_RpScUpdatePKPoint(SRpScUpdatePKPoint *aMsg);	

	//���� ������ ����
	void Proc_RpScChangePetName(SRpScChangePetName * aMsg); 
	void Proc_RpScChangeName(SRpScChangeName * aMsg);	
	void Proc_RpScChangeGender(SRpScChangeGender * aMsg);
	void Proc_RpScChangeFrontierPCName(SRpScChangeFrontierPCName * aMsg);
	
	//PVP Ȱ��ȭ ����
	void Proc_RpScCheckSoldierTime(SRpScCheckSoldierTime * aMsg);	///-- �뺴 ���� �ð� ���� ��������
	void Proc_RpScNPCAttackedMsg(SRpScNPCAttackedMsg * aMsg);
	void Proc_RpScNPCLevelUP(SRpScNPCLevelUP * aMsg);
	void Proc_RpScEnterPvPField(SRpScEnterPvPField * aMsg);
	void Proc_RpScUpdatePvPField(SRpScUpdatePvPField * aMsg);

	//��ű� ���� �ֹ��� ����
	void Proc_RpScMakeAddAccessory(SRpScMakeAddAccessory * aMsg);

	//���� ���ɽð� �ȳ�����
	void Proc_RpScOccupationTime(SRpScOccupationTime * aMsg);

	//------------------------------------------------------------------------------------
	//  ���ٽ��� ���� ����
	//------------------------------------------------------------------------------------
	void Proc_RpScChangeEldaStone(SRpScChangeEldaStone * aMsg);

	//------------------------------------------------------------------------------------
	//  �������� �Ŵ��� 
	//------------------------------------------------------------------------------------
	
	void Proc_SRpScMakeStrengthstone( SRpScMakeStrengthstone * aMsg);
	void Proc_SRpScMakeEssence(SRpScMakeEssence * aMsg);
	void Proc_SRpScUseStrengthstone(SRpScUseStrengthstone * aMsg );

	//------------------------------------------------------------------------------------
	//  �̺�Ʈ����
	//------------------------------------------------------------------------------------
#ifdef TAIWAN30MINEVENT
	void Proc_SRpScTaiwan30minEvent( SRpScTaiwan30minEvent * aMsg );
	void Proc_RpScTaiwan30minEventStart( SRpScTaiwan30minEventStart * aMsg );
#endif //TAIWAN30MINEVENT

public:		
	///-- By simwoosung
	///-- ������ NPC���� üũ
	BOOL	ISCheckPlayerNPC( SRpScAppearNPC* aMsg );	
	
	// �����Ǿ�� �� �޼��� ó�� 
	void	DelayProc_RpScAnswerWarp( SRpScAnswerWarp* aMsg );
	void	DelayProc_RpScAppearPC( SRpScAppearPC* aMsg );
	void	DelayProc_RpScFightRoomList( SRpScFightRoomList* aMsg );

	// �ż� ���� �۾� By wxywxy
	void	Proc_RpScAppearHoly( SRpScAppearNPC* aMsg );
	void	Proc_RpScDisappearHoly( SRpScDisappearNPC* aMsg );
	
	// �� ���� �۾�
	void    DelayProc_RpScAppearPet( SRpScAppearPet* aMsg );
	void	DelayProc_RpScDisAppearPet( SRpScDisappearPet * aMsg );
	
	void    SetProcessAttackEvent( EVENT_DATA_INFO & data_event, 
								   BOOL IsPlayer = TRUE, BOOL IsAttackedDest = FALSE, 
								   int nAttackType = 0, DWORD dAttackID= 0,
								   int nAttackedType = 0, int nAtedIndex = 0);

	// ���ٽ��� ���� ��ȯ 
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
	// �Ѱ����� ������ �ֱ� ���ؼ�...�����忡�� �׷��ִϱ�...d3d error �߻�...����
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
// �ý��� Ű ����
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
