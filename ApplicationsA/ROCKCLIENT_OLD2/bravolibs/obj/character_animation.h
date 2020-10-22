
//#define		PC_ATTACK_NUMBER		47
//#define		PC_SKILL_NUMBER			87
//#define		PC_DEFENSE_NUMBER		127

enum PC_ANIMATION_INDEX
{
	//-----------------------------------------------------------------------------
	// Desc: �� �ִϸ��̼� 
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
	// ���� ���� �ɱ� 3����
	n_Store_sit0,					
	n_Store_sit1,
	n_Store_sit2,
	// ���� ���� 
	n_ClassChage0,				// �������(���)
	n_ClassChage1,				// �������(���)-�κи��
	n_ClassChage2,				// �������(Ȱ)
	n_ClassChage3,				// �������(Ȱ)-�κи��
	n_ClassChage4,				// �������(�Ѽհ�)
	n_ClassChage5,				// �������(�Ѽհ�)-�κи��
	n_ClassChage6,				// �������(����+��)
	n_ClassChage7,				// �������(����+��)-�κи��
	n_ClassChage8,				// �������(������)
	n_ClassChage9,				// �������(������)-�κи��
	n_ClassChage10,				// �������(������)
	n_ClassChage11,				// �������(������)-�κи��
	n_ClassChage12,				// �������(�����)
	n_ClassChage13,				// �������(�����)-�κи��
//.............................................................................................................	
//  ���� ���� ������ ���� By wxywxy 	
//.............................................................................................................	
	n_SI_OneHand_ReAction0,		// �����ϱ�(��)_������
	n_SI_OneHand_ReAction1,		// �����ϱ�(��)_������
	n_SI_OneHand_ReAction2,		// �����ϱ�(ũ��)_������
	n_SI_OneHand_Die,			// �ױ�_������
	n_SI_OneHand_Run,			// �޸���_������
	n_SI_OneHand_Walk,			// �ȱ�_������
	n_SI_OneHand_Wait0,			// ����ڼ�(��ȭ)_������
	n_SI_OneHand_Wait1,			// ����ڼ�(����)_������
	n_SI_OneHand_Wait2,			// ����â ����ڼ�_������
	n_SI_OneHand_Suddenly,		// ���ߵ���_������
	n_SI_OneHand_Attack0,		// ���ݸ��(�и�ġ)_������
	n_SI_OneHand_Attack1,		// ���ݸ��(������)_������
	n_SI_OneHand_Attack2,		// ���ݸ��(�����)_������
	n_SI_OneHand_Attack3,		// ���ݸ��(��ä)_������
//.............................................................................................................	
	n_Skill_KeepUp_Action,      // ���ӽ�ų �ߵ���� ���� 
	n_Act_Drink_Beer,			// ���ָ��ô� �̸�ǵ���		
	n_Act_Shy_Kiss,				// �β����� �ǻ�_�̸�� ����_������	
	n_Act_Shy_Kissing,			// �β����� �ǻ�_�̸�� ����_���� 
	
	n_Ride_Run,					// �� Ÿ�� �޸��� ���	
	n_Ride_Walk,				// �� Ÿ�� ������ ���
	n_Ride_Wait,				// �� Ÿ�� ������ �Ҷ� ����
	n_Ride_Sudden1,				// �� Ÿ�� ���ߵ��� �ϳ�
	n_Ride_Sudden2,				// �� Ÿ�� ���ߵ��� ��				( 374 )

	n_Sit_Chair,				// ���ڿ� �ɴ� ���
	n_KeepSit_Chair,			// ���ڿ� �ɴ� ����_���� ���
	n_Wind_Sword,				// ���� �ҵ�		

	n_Wrath_Bomb,				// �г�����	
	
	n_Dance_Night,				// ����Ʈ ��
	n_Dance_Mercenary,			// �ӽóʸ� ��
	n_Dance_Sniper,				// �������� ��
	n_Dance_Slayer,				// �����̾� ��
	n_Dance_Psyche,				// ����Ű ��
	n_Dance_Prominas,			// ���ι̳��� ��
	n_Dance_Priest,				// ������Ʈ ��
	n_Dance_Holy_Avenger,		// Ȧ����� ��	

//.............................................................................................................	
	n_Animation_MaxCount
//.............................................................................................................	
//	�� �ִϸ��̼� �ε��� 0 ~ 381 
};


#define ANIMATION_NONE				99999


//-----------------------------------------------------------------------------
// NPC �ִϸ��̼� �ε���
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
	n_NPC_Run0 = 0,					// �ٱ� 0 
	n_NPC_Run1,						// �ٱ� 1
	n_NPC_Walk0,					// �ȱ� 0
	n_NPC_Walk1,					// �ȱ� 1
	n_NPC_Attack0,					// ���� 0		-> �Ϲݰ��� 
	n_NPC_Attack1,					// ���� 1		-> ũ��Ƽ�� ���� 
	n_NPC_Attack2,					// ���� 2		-> ��ų���� 
	n_NPC_Attack3,					// ���� 3
	n_NPC_Attack4,					// ���� 4
	n_NPC_Attack5,					// ���� 5
	n_NPC_Attack6,					// ���� 6
	n_NPC_Attack7,					// ���� 7
	n_NPC_Attack8,					// ���� 8
	n_NPC_Stand0,					// ���ֱ� 0
	n_NPC_Stand1,					// ���ֱ� 1	
	n_NPC_ReAction0,				// �����ϱ� ( ���� ������ 0 )
	n_NPC_ReAction1,				// �����ϱ� ( ���� ������ 0 )
	n_NPC_ReAction2,				// �����ϱ� ( ũ��Ƽ�� ������ 0 )
	n_NPC_Die0,						// �ױ� 0
	n_NPC_Die1,						// �ױ� 1
	n_NPC_Die2,						// �ױ� 2
	n_NPC_Die3						// �ױ� 3

	
};

