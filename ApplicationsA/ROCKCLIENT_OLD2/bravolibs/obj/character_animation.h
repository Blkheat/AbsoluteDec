
//#define		PC_ATTACK_NUMBER		47
//#define		PC_SKILL_NUMBER			87
//#define		PC_DEFENSE_NUMBER		127

enum PC_ANIMATION_INDEX
{
	//-----------------------------------------------------------------------------
	// Desc: 락 애니메이션 
	//-----------------------------------------------------------------------------
	n_Standard = 0,
	n_ReAction_LightDmg0,
	n_ReAction_LightDmg1,
	n_ReAction_LightDmg2,
	n_ReAction_LightDmg3,
	n_ReAction_LightDmg5,
	n_ReAction_LightDmg6,
	n_ReAction_LightDmg7,
	n_ReAction_LightDmg9,
	n_ReAction_StrongDmg0,
	n_ReAction_StrongDmg1,
	n_ReAction_StrongDmg2,
	n_ReAction_StrongDmg3,
	n_ReAction_StrongDmg5,
	n_ReAction_StrongDmg6,
	n_ReAction_StrongDmg7,
	n_ReAction_StrongDmg9,
	n_ReAction_Critical0,
	n_ReAction_Critical1,
	n_ReAction_Critical2,
	n_ReAction_Critical3,
	n_ReAction_Critical5,
	n_ReAction_Critical6,
	n_ReAction_Critical7,
	n_ReAction_Critical9,
	n_Die0,
	n_Die1,
	n_Die2,
	n_Die3,
	n_Die5,
	n_Die6,
	n_Die7,
	n_Die9,
	n_Run0,
	n_Run1,
	n_Run2,
	n_Run3,
	n_Run5,
	n_Run6,
	n_Run7,
	n_Run9,
	n_Walk0,
	n_Walk1,
	n_Walk2,
	n_Walk3,
	n_Walk5,
	n_Walk6,
	n_Walk7,
	n_Walk9,
	n_Stand_Peace0,
	n_Stand_Peace1,
	n_Stand_Peace2,
	n_Stand_Peace3,
	n_Stand_Peace5,
	n_Stand_Peace6,
	n_Stand_Peace7,
	n_Stand_Peace9,
	n_Stand_Battle0,
	n_Stand_Battle1,
	n_Stand_Battle2,
	n_Stand_Battle3,
	n_Stand_Battle5,
	n_Stand_Battle6,
	n_Stand_Battle7,
	n_Stand_Battle9,
	n_Stand_Select0,
	n_Stand_Select1,
	n_Stand_Select2,
	n_Stand_Select3,
	n_Stand_Select5,
	n_Stand_Select6,
	n_Stand_Select7,
	n_Stand_Select9,
	n_Suddenly0,
	n_Suddenly1,
	n_Suddenly2,
	n_Suddenly3,
	n_Suddenly5,
	n_Suddenly6,
	n_Suddenly7,
	n_Suddenly9,
	n_Attack0_Basic0,
	n_Attack0_Basic1,
	n_Attack0_Basic2,
	n_Attack0_Basic3,
	n_Attack0_Basic4,
	n_Attack0_KeyZ0,
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
	n_Attack0_KeyC4,
	n_Attack1_Basic0,
	n_Attack1_Basic1,
	n_Attack1_Basic2,
	n_Attack1_Basic3,
	n_Attack1_Basic4,
	n_Attack1_KeyZ0,
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
	n_Attack1_KeyC4,
	n_Attack2_Basic0,
	n_Attack2_Basic1,
	n_Attack2_Basic2,
	n_Attack2_Basic3,
	n_Attack2_Basic4,
	n_Attack2_KeyZ0,
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
	n_Attack2_KeyC4,
	n_Attack3_Basic0,
	n_Attack3_Basic1,
	n_Attack3_Basic2,
	n_Attack3_Basic3,
	n_Attack3_Basic4,
	n_Attack3_KeyZ0,
	n_Attack3_KeyZ1,
	n_Attack3_KeyZ2,
	n_Attack3_KeyZ3,
	n_Attack3_KeyZ4,
	n_Attack3_KeyX0,
	n_Attack3_KeyX1,
	n_Attack3_KeyX2,
	n_Attack3_KeyX3,
	n_Attack3_KeyX4,
	n_Attack3_KeyC0,
	n_Attack3_KeyC1,
	n_Attack3_KeyC2,
	n_Attack3_KeyC3,
	n_Attack3_KeyC4,
	n_Attack5_Basic0,
	n_Attack5_Basic1,
	n_Attack5_Basic2,
	n_Attack5_Basic3,
	n_Attack5_Basic4,
	n_Attack5_ChantReady0,
	n_Attack5_ChantReady1,
	n_Attack5_ChantReady2,
	n_Attack5_ChantGrade0,
	n_Attack5_ChantGrade1,
	n_Attack5_ChantGrade2,
	n_Attack5_ChantShoot0,
	n_Attack5_ChantShoot1,
	n_Attack5_ChantShoot2,
	n_Attack6_Basic0,
	n_Attack6_Basic1,
	n_Attack6_Basic2,
	n_Attack6_Basic3,
	n_Attack6_Basic4,
	n_Attack6_ChantReady0,
	n_Attack6_ChantReady1,
	n_Attack6_ChantReady2,
	n_Attack6_ChantGrade0,
	n_Attack6_ChantGrade1,
	n_Attack6_ChantGrade2,
	n_Attack6_ChantShoot0,
	n_Attack6_ChantShoot1,
	n_Attack6_ChantShoot2,
	n_Attack7_Basic0,
	n_Attack7_Basic1,
	n_Attack7_Basic2,
	n_Attack7_Basic3,
	n_Attack7_Basic4,
	n_Attack7_ChantReady0,
	n_Attack7_ChantReady1,
	n_Attack7_ChantReady2,
	n_Attack7_ChantGrade0,
	n_Attack7_ChantGrade1,
	n_Attack7_ChantGrade2,
	n_Attack7_ChantShoot0,
	n_Attack7_ChantShoot1,
	n_Attack7_ChantShoot2,
	n_Attack7_KeyZ0,
	n_Attack7_KeyZ1,
	n_Attack7_KeyZ2,
	n_Attack7_KeyZ3,
	n_Attack7_KeyZ4,
	n_Attack7_KeyX0,
	n_Attack7_KeyX1,
	n_Attack7_KeyX2,
	n_Attack7_KeyX3,
	n_Attack7_KeyX4,
	n_Attack7_KeyC0,
	n_Attack7_KeyC1,
	n_Attack7_KeyC2,
	n_Attack7_KeyC3,
	n_Attack7_KeyC4,
	n_Attack9_Basic0,
	n_Attack9_Basic1,
	n_Attack9_Basic2,
	n_Attack9_Basic3,
	n_Attack9_Basic4,
	n_Skill_AimSlash,
	n_Skill_VitalSlash,
	n_Skill_CollectSlash,
	n_Skill_RemoveWeapon,
	n_Skill_RestSitdown,
	n_Skill_RestKepping,
	n_Skill_RestStandup,
	n_Attack10_BowReady0,
	n_Attack10_BowReady1,
	n_Attack10_BowReady2,
	n_Attack10_BowWait0,
	n_Attack10_BowWait1,
	n_Attack10_BowWait2,
	n_Attack10_BowShoot0,
	n_Attack10_BowShoot1,
	n_Attack10_BowShoot2,
	n_Skill_GlareShoot,
	n_Skill_FootShoot,
	n_Skill_Heal_Arrow,
	n_Skill_Quick_Shot,
	n_Skill_Intensive,
	n_Skill_Possessed,
	n_Skill_PossessedCancel,
	n_Skill_FireSmack0,
	n_Skill_FireSmack1,
	n_Skill_FireSmack2,
	n_Skill_FootLightning,
	n_Skill_AbsoluteDP,
	n_Skill_StrengthFoot,
	n_Extra_Motion0,
	n_Extra_Motion1,
	n_Extra_Motion2,
	n_Extra_Motion3,
	n_Extra_Motion4,
	n_Extra_Motion5,
	n_Skill_Shiled_Attack,
	n_Skill_Foot_Quake,
	n_Skill_Attacked_Focus,
	n_Skill_Sword_Flash,
	n_Skill_Sword_Circle, 
	n_Skill_KeepUp_Action_2, 
	n_Skill_KeepUp_Action_0, 
	n_Skill_KeepUp_Action_3, 
	n_Skill_KeepUp_Action_5, 
	n_Skill_KeepUp_Action_6, 
	n_Skill_KeepUp_Action_7, 
	n_Skill_Defence,
	n_Attack5_KeyZ0,
	n_Attack5_KeyZ1,
	n_Attack5_KeyZ2,
	n_Attack5_KeyZ3,
	n_Attack5_KeyZ4,
	n_Attack5_KeyX0,
	n_Attack5_KeyX1,
	n_Attack5_KeyX2,
	n_Attack5_KeyX3,
	n_Attack5_KeyX4,
	n_Attack5_KeyC0,
	n_Attack5_KeyC1,
	n_Attack5_KeyC2,
	n_Attack5_KeyC3,
	n_Attack5_KeyC4,
	n_Attack6_KeyZ0,
	n_Attack6_KeyZ1,
	n_Attack6_KeyZ2,
	n_Attack6_KeyZ3,
	n_Attack6_KeyZ4,
	n_Attack6_KeyX0,
	n_Attack6_KeyX1,
	n_Attack6_KeyX2,
	n_Attack6_KeyX3,
	n_Attack6_KeyX4,
	n_Attack6_KeyC0,
	n_Attack6_KeyC1,
	n_Attack6_KeyC2,
	n_Attack6_KeyC3,
	n_Attack6_KeyC4,
	n_Skill_Hide, 
	n_Skill_Assassination, 
	n_Skill_Disappear, 
	n_Skill_RainOfMay, 
	n_Skill_Sniping, 
	n_Skill_Daze, 
	n_Skill_Illusion, 
	n_Skill_SmokeWave, 
	n_Skill_EnergyHeatWave, 
	n_Skill_CycloneHeatWave, 
	n_Skill_Scud, 
	n_Skill_SwordEnergy, 
	n_Skill_LimitBreak,
	n_Act_Request,
	n_Act_Bow,
	n_Act_Rudeness,
	n_Act_Lie0,
	n_Act_Lie1,
	n_Act_Clap,
	n_Act_HandGreeting,
	n_Act_Shy,
	n_Act_Cry,
	n_Act_Compliments,
	n_Act_There,
	n_Act_Dance,
	n_Act_Kiss,
	n_Act_HaHa,
	n_Act_Thank,
	n_Act_Attack,
	n_Act_Nod,
	n_Act_No,
	n_Act_Wait,
	n_Skill_FastCure,
	n_Act_Sit0,
	n_Act_Sit1,
	// 개인 상점 앉기 3동작
	n_Store_sit0,					
	n_Store_sit1,
	n_Store_sit2,
	// 전직 관련 
	n_ClassChage0,				// 전직모션(대검)
	n_ClassChage1,				// 전직모션(대검)-부분모션
	n_ClassChage2,				// 전직모션(활)
	n_ClassChage3,				// 전직모션(활)-부분모션
	n_ClassChage4,				// 전직모션(한손검)
	n_ClassChage5,				// 전직모션(한손검)-부분모션
	n_ClassChage6,				// 전직모션(방패+검)
	n_ClassChage7,				// 전직모션(방패+검)-부분모션
	n_ClassChage8,				// 전직모션(마법서)
	n_ClassChage9,				// 전직모션(마법서)-부분모션
	n_ClassChage10,				// 전직모션(지팡이)
	n_ClassChage11,				// 전직모션(지팡이)-부분모션
	n_ClassChage12,				// 전직모션(흑수정)
	n_ClassChage13,				// 전직모션(흑수정)-부분모션
//.............................................................................................................	
//  유료 무기 아이템 관련 By wxywxy 	
//.............................................................................................................	
	n_SI_OneHand_ReAction0,		// 반응하기(약)_아이템
	n_SI_OneHand_ReAction1,		// 반응하기(강)_아이템
	n_SI_OneHand_ReAction2,		// 반응하기(크리)_아이템
	n_SI_OneHand_Die,			// 죽기_아이템
	n_SI_OneHand_Run,			// 달리기_아이템
	n_SI_OneHand_Walk,			// 걷기_아이템
	n_SI_OneHand_Wait0,			// 대기자세(평화)_아이템
	n_SI_OneHand_Wait1,			// 대기자세(전투)_아이템
	n_SI_OneHand_Wait2,			// 선택창 대기자세_아이템
	n_SI_OneHand_Suddenly,		// 돌발동작_아이템
	n_SI_OneHand_Attack0,		// 공격모션(뿅망치)_아이템
	n_SI_OneHand_Attack1,		// 공격모션(지적봉)_아이템
	n_SI_OneHand_Attack2,		// 공격모션(물고기)_아이템
	n_SI_OneHand_Attack3,		// 공격모션(부채)_아이템
//.............................................................................................................	
	n_Skill_KeepUp_Action,      // 지속스킬 발동모션 통합 
	n_Act_Drink_Beer,			// 맥주마시는 이모션동작		
	n_Act_Shy_Kiss,				// 부끄럽게 뽀뽀_이모션 동작_선동작	
	n_Act_Shy_Kissing,			// 부끄럽게 뽀뽀_이모션 동작_유지 
	
	n_Ride_Run,					// 말 타고 달릴때 모션	
	n_Ride_Walk,				// 말 타고 걸을때 모션
	n_Ride_Wait,				// 말 타고 쉼쉬기 할때 동작
	n_Ride_Sudden1,				// 말 타고 돌발동작 하나
	n_Ride_Sudden2,				// 말 타고 돌발동작 둘				( 374 )

	n_Sit_Chair,				// 의자에 앉는 모션
	n_KeepSit_Chair,			// 의자에 앉는 동작_유지 모션
	n_Wind_Sword,				// 윈드 소드		

	n_Wrath_Bomb,				// 분노폭발	
	
	n_Dance_Night,				// 나이트 댄스
	n_Dance_Mercenary,			// 머시너리 댄스
	n_Dance_Sniper,				// 스나이퍼 댄스
	n_Dance_Slayer,				// 슬레이어 댄스
	n_Dance_Psyche,				// 사이키 댄스
	n_Dance_Prominas,			// 프로미나스 댄스
	n_Dance_Priest,				// 프리스트 댄스
	n_Dance_Holy_Avenger,		// 홀리어벤져 댄스	

//.............................................................................................................	
	n_Animation_MaxCount
//.............................................................................................................	
//	총 애니메이션 인덱스 0 ~ 381 
};


#define ANIMATION_NONE				99999


//-----------------------------------------------------------------------------
// NPC 애니메이션 인덱스
//-----------------------------------------------------------------------------
#define		NPC_ATTACK_NUMBER	10
#define		NPC_ATTACK_COUNT	4
#define		NPC_MAGIG_NUMBER	20
#define		NPC_MAGIC_COUNT		3

/*
Run					000
Walk				100
Attack				200
Idle				300
Down				400
Damage/Critical		500/600
Death				700
*/

enum NPC_ANIMATION_INDEX
{
	n_NPC_Run0 = 0,					// 뛰기 0 
	n_NPC_Run1,						// 뛰기 1
	n_NPC_Walk0,					// 걷기 0
	n_NPC_Walk1,					// 걷기 1
	n_NPC_Attack0,					// 공격 0		-> 일반공격 
	n_NPC_Attack1,					// 공격 1		-> 크리티컬 공격 
	n_NPC_Attack2,					// 공격 2		-> 스킬공격 
	n_NPC_Attack3,					// 공격 3
	n_NPC_Attack4,					// 공격 4
	n_NPC_Attack5,					// 공격 5
	n_NPC_Attack6,					// 공격 6
	n_NPC_Attack7,					// 공격 7
	n_NPC_Attack8,					// 공격 8
	n_NPC_Stand0,					// 서있기 0
	n_NPC_Stand1,					// 서있기 1	
	n_NPC_ReAction0,				// 반응하기 ( 약한 데미지 0 )
	n_NPC_ReAction1,				// 반응하기 ( 강한 데미지 0 )
	n_NPC_ReAction2,				// 반응하기 ( 크리티컬 데미지 0 )
	n_NPC_Die0,						// 죽기 0
	n_NPC_Die1,						// 죽기 1
	n_NPC_Die2,						// 죽기 2
	n_NPC_Die3						// 죽기 3

	
};

