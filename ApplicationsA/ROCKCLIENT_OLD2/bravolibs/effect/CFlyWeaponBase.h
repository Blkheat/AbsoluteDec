//.................................................................................................................
//
// CFlyWeaponBase.h ( 날아가는 무기 베이스 클래스 )													By wxywxy
//
//.................................................................................................................
#ifndef __CFLYWEAPONBASE_H
#define __CFLYWEAPONBASE_H

#include "RockPCH.h"

//.................................................................................................................
// 날아가는 무기 상태
//.................................................................................................................
enum	CFlyWeaponState
{
	FLYWEAPON_STATE_MOVE	= 0,										// 날라가는중
	FLYWEAPON_STATE_ATTACH     ,										// 타켓에 붙어있는 상태
	FLYWEAPON_STATE_DESTROY												// 없어질때		
};

class CFlyWeaponBase
{
public:

	 CFlyWeaponBase();
	~CFlyWeaponBase();

	 virtual int Update(void) { return TRUE; }
	 virtual int Render(void) { return TRUE; }
	 
	 //............................................................................................................
	 // 화살 생성
	 //............................................................................................................
	 virtual int Create(BOOL ISPC,int table_index,BYTE link,D3DXVECTOR3 Curt,D3DXVECTOR3 Dest) { return TRUE; }

	 CFlyWeaponState GetState(void)		{ return m_State; }				// 상태 얻기

	 void SelfDestroy(void)				{ m_State = FLYWEAPON_STATE_DESTROY; }	// 지우기

public:

	D3DXVECTOR3	GetTargetPos(void);
	BOOL        GetTargetPos(D3DXVECTOR3& vPos);
	float       GetTargetRot(void);
	int	        GetTargetEvent(void);



protected:

	CFlyWeaponState		m_State;										// 상태

public:

	BYTE			m_btLink;											// 본노드
	float			m_fTargetRot;										// 타겟의회전값
	long			m_DestTime;											// 도착시간	
	bool			m_bPC;												// 유저 = 1 , NPC = 0
	int				m_Index;											// 캐릭터 인덱스
	D3DXVECTOR3		m_vDestPos;											// 도착위치
	D3DXVECTOR3		m_vCurtPos;											// 시작위치
	D3DXMATRIX		m_matWorld;											// 현재위치

};

#endif
