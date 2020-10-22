// SkillSystem.h: interface for the CSkillSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLSYSTEM_H__64FE5D31_A704_434D_BC02_1B1052972577__INCLUDED_)
#define AFX_SKILLSYSTEM_H__64FE5D31_A704_434D_BC02_1B1052972577__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "character.h"

// 하드 코딩된 스킬 코드들 이곳에 모두 정의...^ ^;;
const WORD c_SkillCode_Basic_PickUp( 4995 );			// 줍기 (공용)
const WORD c_SkillCode_BasicAttack_Millena( 4996 );		// 기본 공격 (밀레나)
const WORD c_SkillCode_BasicAttack_Rain( 4997 );		// 기본 공격 (레인)
const WORD c_SkillCode_MillenaBlessing( 4998 );			// 밀레나의 가호 
const WORD c_SkillCode_RainBlessing( 4999 );			// 레인의 가호 
const WORD c_SkillCode_BreathControl_Min( 11 );			// 호흡조절 최소 
const WORD c_SkillCode_BreathControl_Max( 15 );			// 호흡조절 최대 
const WORD c_SkillCode_Meditation_Min( 1511 );			// 명상 최소 
const WORD c_SkillCode_Meditation_Max( 1515 );			// 명상 최대 
const WORD c_SkillCode_BasicCure_Min( 1 );				// 기초치료 최소 
const WORD c_SkillCode_BasicCure_Max( 5 );				// 기초치료 최대 
const WORD c_SkillCode_MasicPharmacy_Min( 1501 );		// 마법약 지식 최소 
const WORD c_SkillCode_MasicPharmacy_Max( 1505 );		// 마법약 지식 최대 
const WORD c_SkillCode_ProimaOfUnderstand_Min( 1551 );	// 프로이마의 이해 최소 
const WORD c_SkillCode_ProimaOfUnderstand_Max( 1556 );	// 프로이마의 이해 최대 
const WORD c_SkillCode_Synchronization_Min( 1801 );		// 동기화 최소 
const WORD c_SkillCode_Synchronization_Max( 1806 );		// 동기화 최대 
const WORD c_SkillCode_ArcherOfMind_Min( 251 );			// 아처의 마음가짐 최소 
const WORD c_SkillCode_ArcherOfMind_Max( 256 );			// 아처의 마음가짐 최대 
const WORD c_SkillCode_OndHandSwordShip_Min( 101 );		// 검술 최소 
const WORD c_SkillCode_OndHandSwordShip_Max( 106 );		// 검술 최대 
const WORD c_SkillCode_TwoHandSwordShip_Min( 111 );		// 대검술 최소 
const WORD c_SkillCode_TwoHandSwordShip_Max( 116 );		// 대검술 최대 
const WORD c_SkillCode_OndHandSword_Min( 3801 );		// 검 최소 
const WORD c_SkillCode_OndHandSword_Max( 3810 );		// 검 최대 
const WORD c_SkillCode_TwoHandSword_Min( 3811 );		// 대검 최소 
const WORD c_SkillCode_TwoHandSword_Max( 3820 );		// 대검 최대 
const WORD c_SkillCode_Bow_Min( 3821 );					// 활 최소 
const WORD c_SkillCode_Bow_Max( 3830 );					// 활 최대 
const WORD c_SkillCode_Shield_Min( 3861 );				// 방패 최소 
const WORD c_SkillCode_Shield_Max( 3865 );				// 방패 최대 
const WORD c_SkillCode_MagicBook_Min( 3831 );			// 마법서 최소 
const WORD c_SkillCode_MagicBook_Max( 3840 );			// 마법서 최대 
const WORD c_SkillCode_Staff_Min( 3851 );				// 지팡이 최소 
const WORD c_SkillCode_Staff_Max( 3860 );				// 지팡이 최대 
const WORD c_SkillCode_CrystalBall_Min( 3841 );			// 수정구 최소 
const WORD c_SkillCode_CrystalBall_Max( 3850 );			// 수정구 최대 
const WORD c_SkillCode_LightArmor_Min( 3901 );			// 경장비 최소 
const WORD c_SkillCode_LightArmor_Max( 3910 );			// 경장비 최대 
const WORD c_SkillCode_MiddleArmor_Min( 3911 );			// 중장비 최소 
const WORD c_SkillCode_MiddleArmor_Max( 3920 );			// 중장비 최대 
const WORD c_SkillCode_HeavyArmor_Min( 3921 );			// 헤비장비 최소 
const WORD c_SkillCode_HeavyArmor_Max( 3930 );			// 헤비장비 최대 
const WORD c_SkillCode_ServeArmor_Min( 3961 );			// 서브장비 최소 
const WORD c_SkillCode_ServeArmor_Max( 3970 );			// 서브장비 최대 
const WORD c_SkillCode_MasicArmor_Min( 3931 );			// 마도복 최소 
const WORD c_SkillCode_MasicArmor_Max( 3940 );			// 마도복 최대 
const WORD c_SkillCode_PriestArmor_Min( 3951 );			// 사제복 최소 
const WORD c_SkillCode_PriestArmor_Max( 3960 );			// 사제복 최대 
const WORD c_SkillCode_SpiritArmor_Min( 3941 );			// 강신복 최소 
const WORD c_SkillCode_SpiritArmor_Max( 3950 );			// 강신복 최대 
const WORD c_SkillCode_UserSellStore( 4987 );			// 개인 상점

///-- Sub Class Skill
const WORD c_SkillCode_EldaExtract_Min( 4001 );			// 엘다스톤 추출최소
const WORD c_SkillCode_EldaExtract_Max( 4010 );			// 엘다스톤 추출최대
const WORD c_SkillCode_EldaRemodel_Min( 4011 );			// 엘다스톤 가공최소
const WORD c_SkillCode_EldaRemodel_Max( 4020 );			// 엘다스톤 가공최대
const WORD c_SkillCode_ItemRepair_Min( 4031 );			// 아아템 수리 최소
const WORD c_SkillCode_ItemRepair_Max( 4040 );			// 아이템 수리 최대

const WORD c_SkillCode_WeaponEnchant_Min( 4021 );		// 무기 가공술 최소
const WORD c_SkillCode_WeaponEnchant_Max( 4030 );		// 무기 가공술 최대
const WORD c_SkillCode_ArmorEnchant_Min( 4061 );		// 방어구 가공술 최소
const WORD c_SkillCode_ArmorEnchant_Max( 4070 );		// 방어구 가공술 최대

const WORD c_SkillCode_WeaponUpgrade_Min( 4041 );		// 무기 강화술 최소
const WORD c_SkillCode_WeaponUpgrade_Max( 4050 );		// 무기 강화술 최대
const WORD c_SkillCode_ArmorUpgrade_Min( 4051 );		// 방어구 강화술 최소
const WORD c_SkillCode_ArmorUpgrade_Max( 4060 );		// 방어구 강화술 최대

const WORD c_SkillCode_BasicOndHandSwordShip( 4993 );	// 초급 무술 
const WORD c_SkillCode_BasicProimaOfUnderstand( 4994 );	// 초급 지식 

const WORD c_SkillCode_Teleport_Min( 1961 );			// 텔레포트 최소 
const WORD c_SkillCode_Teleport_Max( 1965 );			// 텔레포트 최대 

//const WORD c_SkillCode_Possess_Arc( 1811 );				// 빙의 (아크)
//const WORD c_SkillCode_Possess_Arac( 1812 );			// 빙의 (아라크)
//const WORD c_SkillCode_Possess_Barasac( 1813 );			// 빙의 (바라사크)
//const WORD c_SkillCode_Possess_Kease( 1814 );			// 빙의 (케이아스) 
//const WORD c_SkillCode_Possess_Terasac( 1815 ) ;		// 빙의 (테사라트)

const WORD c_SkillCode_Possess_Shaman_Min( 1811 );		// 샤먼 빙의 최소 
const WORD c_SkillCode_Possess_Shaman_Max( 1812 );		// 샤먼 빙의 최대 

//#ifdef EXPANSION_QUICK_SLOT
	const int c_Max_SkillKey_Num( 30 );
//#else
//	const int c_Max_SkillKey_Num( 20 );
//#endif


const DWORD c_DamageFlag_Subtract( 1000000000 );

#include <map>
#pragma warning(disable:4786)
using namespace std;

typedef enum NSkillKey_QuickSlot
{
	// 액티브 스킬 (물리 공격)
	n_SkillKey_1 = 0,
	n_SkillKey_2,
	n_SkillKey_3,
	n_SkillKey_4,
	n_SkillKey_5,
	n_SkillKey_6,
	n_SkillKey_7,
	n_SkillKey_8,
	n_SkillKey_9,
	n_SkillKey_0,
	n_SkillKey_F1,
	n_SkillKey_F2,
	n_SkillKey_F3,
	n_SkillKey_F4,
	n_SkillKey_F5,
	n_SkillKey_F6,
	n_SkillKey_F7,
	n_SkillKey_F8,
	n_SkillKey_F9,
	n_SkillKey_F0,
	n_SkillKey_SF1,
	n_SkillKey_SF2,
	n_SkillKey_SF3,
	n_SkillKey_SF4,
	n_SkillKey_SF5,
	n_SkillKey_SF6,
	n_SkillKey_SF7,
	n_SkillKey_SF8,
	n_SkillKey_SF9,
	n_SkillKey_SF0
	
} NSkillKey_QuickSlot;


typedef enum NSkillKey_ShortCut
{
	n_SkillKey_LSHIFT = 0,
	n_SkillKey_LCONTROL 	

} NSkillKey_ShortCut;


/*
typedef enum NSkillType
{
	// 액티브 스킬 (물리 공격)
	n_TwinSlash = 1,
	n_WristBreak,
	n_BearCrash,
	n_TwinShot,
	n_AnkleShot,
	// 액티브 스킬 (마법 공격)
	n_FireBall,
	n_Lighting,
	n_BubbleCrash,
	n_IceSpear,
	n_RockPress,
	n_Bramble,
	// 액티브 스킬 (기타)
	n_Dash,
	n_PathFind,
	n_Respiration,
	n_Meditation,
	// 패시브 스킬 (무기)
	n_WindSword,
	n_StrangerBook,
	n_OneHandSword,
	n_Shield,
	n_TwoHandSword,
	n_Bow,
	n_MagicBook_Fire_Lighting,
	n_MagicBook_Water_Ice,
	n_MagicBook_Earth_Nature,
	n_Staff,
	n_Crystal,
	// 패시브 스킬 (방어구)
	n_MillenaArmor,
	n_RainArmor,
	n_LightArmor,
	n_MediumArmor,
	n_HeavyArmor,
	n_ProminasArmor,				// 마도복 프로미나스 
	n_PristArmor,					// 사제복 프리스트
	n_ShamonArmor, 				// 강신복 샤먼 
	// 패시브 스킬 (아이템)

	// 패시브 스킬 (기타)
	n_MillenaBless,
	n_RainBless,
	n_Combo,
	n_Chant
	
} NSkillType;
*/


class CSkillSystem  
{
public:
	WORD			theQuickSlot_SkillCode[c_Max_SkillKey_Num];		// NUMBER 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
	WORD			theShortCut_SkillCode[3];						// LCONTROL, SPACE , Combo Index
	BOOL			theQuickSlot_DelayTime[c_Max_SkillKey_Num];		// NUMBER 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12

	BOOL			theTargetIsPlayer;
	BOOL			theTargetIsPlayerCell;	
	WORD			theQuickSlotIndex;

	DWORD			theTargetLand_SkillCode;
	char			theStrHotKey[9];								// 콤보키 조합 스트링

	BOOL			theIsUsedSkill;
	BOOL			theUseSpellSkill;
	long			theUseSPellTime;
	long			theSkillDelay_time;
	map				<DWORD , long> UseSkillList;
	
public:
	CSkillSystem();
	virtual ~CSkillSystem();

	///--JAPAN_BUG_MODIFY
	bool UseSkillMgr( int aPressKey, DWORD dSkillCode );// 스킬 사용
	bool UseSkillWndMgr( int aSkillCode );

	void UseSkillTargetMgr( Character* pSrcChr, EVENT_DATA_INFO& aSkillEvent, _STargetInfo aTargetInfo );
	///--JAPAN_BUG_MODIFY
	void UseSkillTargetInfoMgr( Character* pSrcChr, _STargetInfo aTargetInfo, WORD aSkillCode, BOOL bCreateEffect );
	void UseSkillTargetInfoMgr_AddEffect( Character* pSrcChr, _STargetInfo aTargetInfo, WORD aSkillCode, BOOL bCreateEffect );
	void UseSkillEffectValueMgr( DWORD aTargetID, BYTE aTargetType, WORD aSkillCode, Byte aEffectType, long aEffectValue, WORD aEffectMaxValue, BOOL aDieFlag );
	void UseItemEffectValueMgr( DWORD aTargetID, BYTE aTargetType, WORD aItemCode, Byte aEffectType, long aEffectValue, WORD aEffectMaxValue, BOOL aDieFlag );
	void UseSkillAddEffectValueMgr( Character* pSrcChr, BOOL aIsStart, DWORD aTargetID, BYTE aTargetType, WORD aSkillCode );


	int GetSkillCode( int aPressKey );
	
	bool CheckEnableUseSkill( DWORD aSkillCode );
	bool CheckTargetStatus( SSkillBaseInfo* aSkill_Info );
	bool CheckAttackTarget( SSkillBaseInfo* aSkill_Info );	
	bool CheckAttackFrame( SSkillBaseInfo* aSkill_Info );
	bool CheckDelayTime( SSkillBaseInfo* aSkill_Info );
	bool CheckAttackRange( SSkillBaseInfo* aSkill_Info );
	bool CheckNeedStatus( SSkillBaseInfo* aSkill_Info );
	bool CheckNeedItem( SSkillBaseInfo* aSkill_Info );
	
	
	void SaveShortcutSkill();
	void LoadShortcutSkill();

	void SetDamageValue( WORD aSkillCode, _STargetInfo *aTargetInfo, BYTE aEffectType = 0 , DWORD aAttackID = 0 );
	void SetDamageValue( DWORD aTargetID, BYTE aTargetType, WORD aSkillCode, WORD aEffectType, DWORD& aEffectValue );
	bool SetMultiAttackDamageValue( EVENT_DATA_INFO& aEvent, BYTE theAttackType, DWORD theAttackID);

	//..............................................................................................................
	// 아이스 레인과 같은 지속시간은 있지만 단발성 스킬 처리 할것들 정리 by wxywxy
	//..............................................................................................................
	BOOL ISSkipExtraSkill(WORD SkillCode);
	
	///-- By simwoosung;
	void InitCharDamage(Character * pChar );

private:
	
	// 스킬 이펙트 생성 By wxywxy
	void CreateSkill(Character *pSrc,Character *pDest,int skilledIdx,SSkillBaseInfo* pSkillBaseInfo,EVENT_DATA_INFO &underattack_event,EVENT_DATA_INFO& aSkillEvent, _STargetInfo aTargetInfo);


};

#endif // !defined(AFX_SKILLSYSTEM_H__64FE5D31_A704_434D_BC02_1B1052972577__INCLUDED_)
