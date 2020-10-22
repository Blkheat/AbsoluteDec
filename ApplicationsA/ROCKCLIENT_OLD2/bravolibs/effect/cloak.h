
#ifndef			__CLOAK_H__
#define			__CLOAK_H__


#ifdef DIRECT_VERSION_9_MJH
#include		<D3DX9.H>
#else
#include		<D3DX8.H>
#endif // DIRECT_VERSION_9_MJH
#include		"EffectVertex.h"


#define PI			3.1415926535

#define	MAX_ROW			7
#define	MAX_COL			13



struct FORCE
{
	float fx, fy, fz;
};
/*
struct CLOAKPARTICLE
{
	float x, y, z;		// 파티클의 위치
	float vx, vy, vz;	// 파티클들이 가지는 속도
	float ax, ay, az;	// 파티클들이 가지는 가속도
	float mass;			// 파티클들의 질량
	FORCE localforce;	// 파티클들이 갖는 현재 힘
	FORCE fixforce;		// 파티클들의 기본 힘
};


class		CCloak
{
public:
	D3DXVECTOR3			m_vFixed[3];			// 고정점 ( 0왼쪽 어깨, 1척추, 2오른쪽 어깨 )
	LPDIRECT3DTEXTURE8	m_pCloakTex;
	LPDIRECT3DVERTEXBUFFER8		m_pCloakVB;
	LPDIRECT3DINDEXBUFFER8		m_pCloakIB;

	CLOAKPARTICLE		pa[MAX_ROW*MAX_COL];	// 망토를 만들 ROW*COL의 점들
	FORCE		GlobalForce;			// 전체에 영향을 미치는 힘( 예로 바람 )
	FORCE		springforce[MAX_ROW*MAX_COL];	// 각점들이 가지고 있는 힘( 어느방향으로 힘을 받고 있나 )

	float		m_fColLength[MAX_COL][MAX_ROW-1];
	float		m_fRowLength[MAX_COL-1][MAX_ROW];


	HRESULT		InitCloak();			// 파티클 초기화 및 텍스쳐 로딩
	void		SetPhysicsWorld() { GlobalForce.fy = -0.2f; }
	void		ResetSimulation();		// 파티클 초기화
	void		SetParticleLength();	// 파티클 들의 간격 설정

	void		CreateParticle( int i, float mass, float x, float y, float z, float vx, float vy, float vz );	// 파티클 초기화
	void		MoveParticle(CLOAKPARTICLE * p, float fTimeD);	// 파티클 이동
	void		SetFixed();
	void		CalculateFixed();
	
	void		UpdateLocalForce( float fTimeD );			// 파티클 이동하기전 힘의 의한 파티클 이동
	void		Render( float fTimeD );		// 망토 그리기

	CCloak();
	~CCloak();
};
*/
#endif