//----------------------------------------------------------------------------------------------------------------------
// ��  ��   �� : HHMWorkDefine.h
// ��       �� : �۾� ������
// ���� �ۼ��� : ������
// ���� �ۼ��� : 2009.04.01
//----------------------------------------------------------------------------------------------------------------------
#ifndef HHM_WORK_DEFINE_H
//#define HHM_WORK_DEFINE_H

#include "Protocols\\BaseDataStructures.h"  


// HHM Define DX8-------------------------------------------------------------------------------------------------------
#define HHM_CAMERA_FOV// ����� ��Ⱦ�� ���� �þ߰� ����(2009/04/13)
#define HHM_USER_RESOLUTION_SELECT// �ػ� ����(2009/04/23)
#define HHM_TEXTBOX_ENABLE// Ȱ��ȭ�� ���� �ؽ�Ʈ �� ����. ��Ȱ����(128, 128, 128) (2009/05/13)
#define HHM_SLIDEBAR_ENABLE// Ȱ��ȭ�� ���� EventProc() ����(2009/05/13)
#define HHM_CHARACTER_SELECT_3D_PICK// ĳ���� ����â���� ��ư���� �ϴ� ĳ���� ������ 3D ��ŷ���� �ٲ�(2009/06/12)
#define HHM_IF_CHAR_DEL_OPEN_DONT_BT_CLICK// ĳ���� ����â�� ������ ���� ��ư���� Ŭ���Ǹ� �ȵȴ�(2009/06/15)

//#define HHM_1600X1200_TEST// Radeon 9550 �ػ� 1600 X 1200 â��忡�� �ӵ� ����(2009/05/15)
//#define HHM_DRAW_FRUSTUM_LINE// ����ü �׸���(2009/05/19)
//#define APPLYWIDE_MAP// �ɿ켺

//#define HHM_HACK_SHIELD_UPDATE_20090703// �ѱ�/�Ϻ�/Ÿ�̿� �ٽ��� ������Ʈ


// ���ѹα�-------------------------------------------------------------------------------------------------------------
#ifdef KOREA_VERSION
	#ifdef KOREA_TEST_VERSION

		#define HHM_HACK_SHIELD_UPDATE_20090703// �ѱ�/�Ϻ�/Ÿ�̿� �ٽ��� ������Ʈ
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703

	#endif// #ifdef KOREA_TEST_VERSION
#endif// #ifdef KOREA_VERSION
// ���ѹα�-------------------------------------------------------------------------------------------------------------





// �Ϻ�-----------------------------------------------------------------------------------------------------------------
#ifdef JAPAN_VERSION
	#ifdef JAPAN_TEST_VERSION
	#endif// #ifdef JAPAN_TEST_VERSION
		
	#define HHM_HACK_SHIELD_UPDATE_20090703// �ѱ�/�Ϻ�/Ÿ�̿� �ٽ��� ������Ʈ(20090714)
	#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// ��������� �� Ŭ������ 9/15/17�̶�� �ŷ�����
	#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703
#endif// #ifdef JAPAN_VERSION
// �Ϻ�-----------------------------------------------------------------------------------------------------------------





// �۷ι�---------------------------------------------------------------------------------------------------------------
#ifdef GLOBAL_VERSION
	#ifdef GLOBAL_VERSION

		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703

	#endif// #ifdef GLOBAL_VERSION
#endif// #ifdef GLOBAL_VERSION
// �۷ι�---------------------------------------------------------------------------------------------------------------




// Ÿ�̿�---------------------------------------------------------------------------------------------------------------
#ifdef TAIWAN_VERSION
		
		#define HHM_12180_12188_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090824
		#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090731
		#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// ��������� �� Ŭ������ 9/15/17�̶�� �ŷ�����

	#ifdef TAIWAN_TEST_VERSION
		#define HHM_HACK_SHIELD_UPDATE_20090703// �ѱ�/�Ϻ�/Ÿ�̿� �ٽ��� ������Ʈ(20090731)
	#endif// #ifdef TAIWAN_TEST_VERSION

#endif// #ifdef TAIWAN_VERSION
// Ÿ�̿�-------------------------------------------------------------------------------------------------------------





// ȫ��-----------------------------------------------------------------------------------------------------------------
#ifdef HONGKONG_VERSION
	#ifdef HONGKONG_TEST_VERSION

		#define HHM_GAME_GUARD_UPDATE_20090908// ���Ӱ��� ������Ʈ(20090925)
		
	#endif// #ifdef HONGKONG_TEST_VERSION
	
	#define HHM_PAYITEM_CLASS_9_15_17_DEAL_POSSIBLE_20090707// ��������� �� Ŭ������ 9/15/17�̶�� �ŷ�����

	// 60,000�� �̻��� �ڵ带 ���� �ռ����ٽ����� ���� �������� �ŷ��Ұ�(���λ��� ��� �Ұ�)/����� �ı�/���� �Ұ�
	#define HHM_60000_ABOVE_ITEM_COMPOSITION_NOT_EXCHANGE_20090625// 20090703

#endif// #ifdef HONGKONG_VERSION
// ȫ��-----------------------------------------------------------------------------------------------------------------




#endif// #ifndef HHM_WORK_DEFINE_H
