//.................................................................................................................
//
// CArrowWeapon.h ( 화살 )																		By wxywxy
//
//.................................................................................................................
#ifndef __CARROWWEAPON_H
#define __CARROWWEAPON_H

#include "CFlyWeaponBase.h"

#define		ARROW_MOVING_TIME		1.0f									// 화살 날라가는 시간
#define		ARROW_ATTACH_TIME		5000.0f									// 화살이 타겟에 박혀있는 시간

class CArrowWeapon : public CFlyWeaponBase
{
public:
	
	 CArrowWeapon();
	~CArrowWeapon();

public:

	 int Update(void);
	 int Render(void);
	 
 	 //............................................................................................................
  	 // 화살 생성
	 //............................................................................................................
	 int Create(BOOL ISPC,int table_index,BYTE link,D3DXVECTOR3 Curt,D3DXVECTOR3 Dest);

private:

	 DWORD	m_TimeRange;
};


#endif
