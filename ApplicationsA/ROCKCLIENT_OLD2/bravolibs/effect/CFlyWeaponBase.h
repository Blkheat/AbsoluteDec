//.................................................................................................................
//
// CFlyWeaponBase.h ( ���ư��� ���� ���̽� Ŭ���� )													By wxywxy
//
//.................................................................................................................
#ifndef __CFLYWEAPONBASE_H
#define __CFLYWEAPONBASE_H

#include "RockPCH.h"

//.................................................................................................................
// ���ư��� ���� ����
//.................................................................................................................
enum	CFlyWeaponState
{
	FLYWEAPON_STATE_MOVE	= 0,										// ���󰡴���
	FLYWEAPON_STATE_ATTACH     ,										// Ÿ�Ͽ� �پ��ִ� ����
	FLYWEAPON_STATE_DESTROY												// ��������		
};

class CFlyWeaponBase
{
public:

	 CFlyWeaponBase();
	~CFlyWeaponBase();

	 virtual int Update(void) { return TRUE; }
	 virtual int Render(void) { return TRUE; }
	 
	 //............................................................................................................
	 // ȭ�� ����
	 //............................................................................................................
	 virtual int Create(BOOL ISPC,int table_index,BYTE link,D3DXVECTOR3 Curt,D3DXVECTOR3 Dest) { return TRUE; }

	 CFlyWeaponState GetState(void)		{ return m_State; }				// ���� ���

	 void SelfDestroy(void)				{ m_State = FLYWEAPON_STATE_DESTROY; }	// �����

public:

	D3DXVECTOR3	GetTargetPos(void);
	BOOL        GetTargetPos(D3DXVECTOR3& vPos);
	float       GetTargetRot(void);
	int	        GetTargetEvent(void);



protected:

	CFlyWeaponState		m_State;										// ����

public:

	BYTE			m_btLink;											// �����
	float			m_fTargetRot;										// Ÿ����ȸ����
	long			m_DestTime;											// �����ð�	
	bool			m_bPC;												// ���� = 1 , NPC = 0
	int				m_Index;											// ĳ���� �ε���
	D3DXVECTOR3		m_vDestPos;											// ������ġ
	D3DXVECTOR3		m_vCurtPos;											// ������ġ
	D3DXMATRIX		m_matWorld;											// ������ġ

};

#endif
