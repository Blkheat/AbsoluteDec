//.................................................................................................................
//
//  CDamageEffect.h																					By wxywxy
//
//  데미지 처리시 이펙트 / 사운드 처리
//.................................................................................................................
#ifndef __CDAMAGEEFFECT_H
#define __CDAMAGEEFFECT_H

#define DAMAGE_EFFECT_FILENAME		"Chr\\DamageEffect.dat"

typedef enum DamageState
{
	DS_WEAK					 = 0 ,								// 약
	DS_STRONG					 ,								// 강	
	DS_CRITICAL													// 크리티컬
};

typedef struct s_USER_DEMAGE_SET
{
	int	  EffectNum[3];
	float LifeTime[3];
	int	  SoundNum[3];

	s_USER_DEMAGE_SET()
	{
		EffectNum[ 0 ] = EffectNum[ 1 ] = EffectNum[ 2 ] = -1;
		LifeTime[ 0 ]  = LifeTime[ 1 ]  = LifeTime[ 2 ]  = 0;
		SoundNum [ 0 ] = SoundNum [ 0 ] = SoundNum [ 0 ] = -1;
	}

}USER_DEMAGE_SET;

typedef struct s_NPC_DEMAGE_SET
{
	int	  EffectNum[3];
	float LifeTime[3];
	
	int	  SoundNum[3];
	int	  Type;

	s_NPC_DEMAGE_SET()
	{
		Clear();
	}

	void Clear(void)
	{
		EffectNum[ 0 ] = EffectNum[ 1 ] = EffectNum[ 2 ] = -1;
		LifeTime [ 0 ] = LifeTime [ 1 ] =  LifeTime[ 2 ] = 0;
	
		SoundNum [ 0 ] = SoundNum [ 0 ] = SoundNum [ 0 ] = -1;
		Type     = -1;
	}

}NPC_DEMAGE_SET;

class CDamageEffect
{
public:

 	  CDamageEffect();
	 ~CDamageEffect();

private:
	
	  USER_DEMAGE_SET					m_UserSet[4];
	  std::vector	<NPC_DEMAGE_SET>	m_NpcSet;

public:
	  
	  int Load(char *Path);
	  
	  // NPC 공격
	  int AttackedNPC_Effect(long unique,DWORD index,long Target_unique,DWORD Target_index,DWORD Damage,BYTE IsCritical,D3DXVECTOR3 vPos,BYTE IsSuccess = TRUE );

	  // 유저 공격 
	  int AttackedUser_Effect(long Target_unique,DWORD Target_index,DWORD Damage,BYTE IsCritical,D3DXVECTOR3 vPos,BYTE IsSuccess = TRUE );

private:

	  DamageState DamageLevel(long Target_unique,DWORD Target_index,DWORD Damage,BYTE IsCritical);
	  int FindWeaponType(int WeaponType);

};

#endif