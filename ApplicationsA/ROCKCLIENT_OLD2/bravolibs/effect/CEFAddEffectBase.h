//...............................................................................................................
// 
//	CEFAddEffectBase.h														 부가 효과 이펙트
//
//...............................................................................................................
#ifndef __CEFADDEFFECTBASE_H
#define __CEFADDEFFECTBASE_H

#include "CEffectBase.h"

typedef struct s_ADD_EFFECT_DATA
{
	
	D3DXCOLOR	Color;
	int			EffectSetNum;

	void SetData(D3DXCOLOR color,int setnum)
	{
		Color        = color;
		EffectSetNum = setnum;
	}

}ADD_EFFECT_DATA;

class CEFAddEffectBase : public CEffectBase
{
public:
	 
			 CEFAddEffectBase();
    virtual ~CEFAddEffectBase();

	virtual int     Update(float dtime);

	int				InsertAddSkill(int EffectCode);
	int				DeleteAddSkill(int EffectCode);

	ADD_EFFECT_DATA	*GetSetData(int Num);
	int				 DeleteEffect(int Num);
	
	int				SetEffect(int Num);

	static  int		InitDevice(void);
	static  int		DeleteDevice(void);

private:

	std::list	<int>				m_AddEffectList;
	int								m_NowEffect;

	static std::list   <ADD_EFFECT_DATA>	m_AddEffectSetList;
};

#endif
