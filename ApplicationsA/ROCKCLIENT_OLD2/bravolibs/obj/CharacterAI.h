
#ifndef _CHARACTER_AI_H_
#define _CHARACTER_AI_H_

#ifdef DIRECT_VERSION_9_MJH
#include <d3d9.h>
#else
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

class Character;

enum PetAIState
{
	N_Normal = 0,
	N_Trans,
	N_Attack,
};

class CState
{
public:

PetAIState	nSate;

public:
	virtual ~CState(){}
	
	virtual void Enter(Character *) = 0;
	virtual void Execute(Character *) = 0;
	virtual void Exit(Character *) = 0;
};

class CNormalState : public CState
{
public:
	CNormalState() { nSate = N_Normal; }
	virtual ~CNormalState(){}
	
	virtual void Enter(Character *);
	virtual void Execute(Character *);
	virtual void Exit(Character *);

	static CNormalState * Instance();
};

class CTransState : public CState
{

private:
	bool			bOnetime;	

public:
	CTransState() { nSate = N_Trans; }
	virtual ~CTransState(){}
	
	virtual void Enter(Character *);
	virtual void Execute(Character *);
	virtual void Exit(Character *);

	static CTransState * Instance();
};

class CAttackState : public CState
{
private:
	bool	bOnetime;
	int     m_nAttackIndex;

public:
	CAttackState() { nSate = N_Attack; }
	virtual ~CAttackState(){}
	
	virtual void Enter(Character *);
	virtual void Execute(Character *);
	virtual void Exit(Character *);

	static CAttackState * Instance();
};

#endif
