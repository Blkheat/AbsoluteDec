//.................................................................................................................
//
// CArrowWeapon.h ( ȭ�� )																		By wxywxy
//
//.................................................................................................................
#ifndef __CARROWWEAPON_H
#define __CARROWWEAPON_H

#include "CFlyWeaponBase.h"

#define		ARROW_MOVING_TIME		1.0f									// ȭ�� ���󰡴� �ð�
#define		ARROW_ATTACH_TIME		5000.0f									// ȭ���� Ÿ�ٿ� �����ִ� �ð�

class CArrowWeapon : public CFlyWeaponBase
{
public:
	
	 CArrowWeapon();
	~CArrowWeapon();

public:

	 int Update(void);
	 int Render(void);
	 
 	 //............................................................................................................
  	 // ȭ�� ����
	 //............................................................................................................
	 int Create(BOOL ISPC,int table_index,BYTE link,D3DXVECTOR3 Curt,D3DXVECTOR3 Dest);

private:

	 DWORD	m_TimeRange;
};


#endif
