#ifndef __CCOMBOEFFECT_H
#define __CCOMBOEFFECT_H

//...............................................................................................................
//
// 레벨 :	0		1		2		3		4		Ex
//         없음
//...............................................................................................................


typedef struct 
{
	int level;
	int EffectNum[ 3 ];				// 0 : 근접 전투계 , 1 : 간접 전투계 , 2 : 마법계 전투계

}ComboEffectSet;


class CComboEffect
{
public:
	 
	 CComboEffect();
	~CComboEffect();

public:

 	 void SetComboEffect(int level);

private:
	
	 int  GetClass(void);
	 void Clear(int Type);

private:

	 int				 m_OldLevel;

	 ComboEffectSet		*m_pList;
 
};

#endif
