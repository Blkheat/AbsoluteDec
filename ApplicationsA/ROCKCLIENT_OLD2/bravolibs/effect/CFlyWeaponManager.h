//.................................................................................................................
//
// CFlyWeaponManager.h ( 날아가는 무기 매니저 )														By wxywxy
//
//.................................................................................................................
#ifndef __CFLYWEAPONMANAGER_H
#define __CFLYWEAPONMANAGER_H

#include "CFlyWeaponBase.h"
#include <list>
#include <iostream>

enum	FlyWeaponType 
{
	ARROW_WEAPON_TYPE	= 0											// 기본 화살
};

class CFlyWeaponManager
{
public:
	 
	 CFlyWeaponManager();
	~CFlyWeaponManager();

public:

	int Update(void);
	int Render(void);
	
	int ResetMemory(void);											// 리스트 비우기

	int Create(BOOL ISPC,int table_index,BYTE link,D3DXVECTOR3 Curt,D3DXVECTOR3 Dest,FlyWeaponType WeaponType);

	int Clear( bool bPC, int table_index );

private:

	std::list	<CFlyWeaponBase *>		m_List;
};

#endif