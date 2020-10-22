#ifndef __CSPELLEFFECT_H
#define __CSPELLEFFECT_H

#include "CIndexList.h"

//...............................................................................................................
//
//  Level   1			2			3			4			5			6
//	Hit		0			1			2			3			4			5
//			링크0		링크1		링크2	    링크3		링크4		링크5
//						오라0		오라1		오라2		오라3		오라4
// 
//...............................................................................................................

typedef struct 
{

	int		LinkEffect[6];
	int		SpotEffect[6];
	float   SpotTime[6];

}SPELL_EFFECT_DATA;

class CSpellEffectBaseData
{
public:
	 CSpellEffectBaseData();
	~CSpellEffectBaseData();

public:

	int	GetSpellEffect(DWORD SkillCode,int Level,int *pSpotEffectNum,float *pSpotTime,int *pLinkEffectNum);				// 무기종류 , 레벨로 영창 이펙트 번호 찾기
	int Load(char *Path);

private:

	CIndexList<SPELL_EFFECT_DATA>	m_EffectList;

};

class CSpellEffect
{
public:
	
	 CSpellEffect();
	~CSpellEffect();

public:

	void SetSpellEffect(DWORD SkillCode,int Level,long lUnique,int nIndex);				// 영창 이펙트 발동 ( 무기종류 / 영창 레벨 )
	void Clear(void);																	// 영창 이펙트 끄기

private:

	int		m_OldEffectNum;
	int		m_Index;
	long	m_lUnique;
	int     m_OldSpotNum;
	int     m_OldLinkNum;
};

#endif