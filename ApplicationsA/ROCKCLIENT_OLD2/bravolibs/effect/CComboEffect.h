#ifndef __CCOMBOEFFECT_H
#define __CCOMBOEFFECT_H

//...............................................................................................................
//
// ���� :	0		1		2		3		4		Ex
//         ����
//...............................................................................................................


typedef struct 
{
	int level;
	int EffectNum[ 3 ];				// 0 : ���� ������ , 1 : ���� ������ , 2 : ������ ������

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
