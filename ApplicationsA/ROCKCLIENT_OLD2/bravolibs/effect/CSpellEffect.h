#ifndef __CSPELLEFFECT_H
#define __CSPELLEFFECT_H

#include "CIndexList.h"

//...............................................................................................................
//
//  Level   1			2			3			4			5			6
//	Hit		0			1			2			3			4			5
//			��ũ0		��ũ1		��ũ2	    ��ũ3		��ũ4		��ũ5
//						����0		����1		����2		����3		����4
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

	int	GetSpellEffect(DWORD SkillCode,int Level,int *pSpotEffectNum,float *pSpotTime,int *pLinkEffectNum);				// �������� , ������ ��â ����Ʈ ��ȣ ã��
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

	void SetSpellEffect(DWORD SkillCode,int Level,long lUnique,int nIndex);				// ��â ����Ʈ �ߵ� ( �������� / ��â ���� )
	void Clear(void);																	// ��â ����Ʈ ����

private:

	int		m_OldEffectNum;
	int		m_Index;
	long	m_lUnique;
	int     m_OldSpotNum;
	int     m_OldLinkNum;
};

#endif