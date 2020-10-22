
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
	float x, y, z;		// ��ƼŬ�� ��ġ
	float vx, vy, vz;	// ��ƼŬ���� ������ �ӵ�
	float ax, ay, az;	// ��ƼŬ���� ������ ���ӵ�
	float mass;			// ��ƼŬ���� ����
	FORCE localforce;	// ��ƼŬ���� ���� ���� ��
	FORCE fixforce;		// ��ƼŬ���� �⺻ ��
};


class		CCloak
{
public:
	D3DXVECTOR3			m_vFixed[3];			// ������ ( 0���� ���, 1ô��, 2������ ��� )
	LPDIRECT3DTEXTURE8	m_pCloakTex;
	LPDIRECT3DVERTEXBUFFER8		m_pCloakVB;
	LPDIRECT3DINDEXBUFFER8		m_pCloakIB;

	CLOAKPARTICLE		pa[MAX_ROW*MAX_COL];	// ���並 ���� ROW*COL�� ����
	FORCE		GlobalForce;			// ��ü�� ������ ��ġ�� ��( ���� �ٶ� )
	FORCE		springforce[MAX_ROW*MAX_COL];	// �������� ������ �ִ� ��( ����������� ���� �ް� �ֳ� )

	float		m_fColLength[MAX_COL][MAX_ROW-1];
	float		m_fRowLength[MAX_COL-1][MAX_ROW];


	HRESULT		InitCloak();			// ��ƼŬ �ʱ�ȭ �� �ؽ��� �ε�
	void		SetPhysicsWorld() { GlobalForce.fy = -0.2f; }
	void		ResetSimulation();		// ��ƼŬ �ʱ�ȭ
	void		SetParticleLength();	// ��ƼŬ ���� ���� ����

	void		CreateParticle( int i, float mass, float x, float y, float z, float vx, float vy, float vz );	// ��ƼŬ �ʱ�ȭ
	void		MoveParticle(CLOAKPARTICLE * p, float fTimeD);	// ��ƼŬ �̵�
	void		SetFixed();
	void		CalculateFixed();
	
	void		UpdateLocalForce( float fTimeD );			// ��ƼŬ �̵��ϱ��� ���� ���� ��ƼŬ �̵�
	void		Render( float fTimeD );		// ���� �׸���

	CCloak();
	~CCloak();
};
*/
#endif