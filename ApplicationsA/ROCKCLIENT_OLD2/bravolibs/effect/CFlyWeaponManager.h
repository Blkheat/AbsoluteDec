//.................................................................................................................
//
// CFlyWeaponManager.h ( ���ư��� ���� �Ŵ��� )														By wxywxy
//
//.................................................................................................................
#ifndef __CFLYWEAPONMANAGER_H
#define __CFLYWEAPONMANAGER_H

#include "CFlyWeaponBase.h"
#include <list>
#include <iostream>

enum	FlyWeaponType 
{
	ARROW_WEAPON_TYPE	= 0											// �⺻ ȭ��
};

class CFlyWeaponManager
{
public:
	 
	 CFlyWeaponManager();
	~CFlyWeaponManager();

public:

	int Update(void);
	int Render(void);
	
	int ResetMemory(void);											// ����Ʈ ����

	int Create(BOOL ISPC,int table_index,BYTE link,D3DXVECTOR3 Curt,D3DXVECTOR3 Dest,FlyWeaponType WeaponType);

	int Clear( bool bPC, int table_index );

private:

	std::list	<CFlyWeaponBase *>		m_List;
};

#endif